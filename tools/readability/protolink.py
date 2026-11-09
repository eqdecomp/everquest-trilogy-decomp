"""Prototype journal and link check for gate.py (T0): C++ decorated names and cross-TU symbol resolution.

T0 compiles each changed TU and compares it with the base build; it never links. Two gaps follow:

1. A prototype change (`void f(int)` -> `void f(long)` in a shared header) changes the decorated name
   (`?f@@YAXH@Z` -> `?f@@YAXJ@Z`) in the definer and in every caller. The callers' code is the same, but a call
   target keyed by name differs: a false T0 failure.
2. Real link breakage is invisible: a caller still bound to the old prototype (LNK2001 unresolved), a rename that
   collides with another TU's definition (LNK2005, the hooks DLL break), C vs C++ linkage mismatches.

`journal()` derives {old decorated -> new decorated} pairs per TU from the before/after objects: definitions
matched by annotated VA (`// FUNCTION:`), then definitions and references whose undecorated name is the same on
both sides (one on each side). gate.py's diff treats a journaled pair as the same target.

`link_check()` resolves the external symbols of the affected objects of one client (the changed TUs plus every
TU of that client whose text names one of the symbols, built once and cached): each journaled symbol and each
external definition the change added or removed must be defined exactly once if anything references it, the old
side of a journaled pair must not still be defined (then a caller switched overloads: a different function),
and references must not be left without a definition when the project defines that name (C or C++ spelling).
It is a symbol-table resolver over COFF objects, not LINK: no libraries, no section layout.
"""
from __future__ import annotations

import collections
import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

IMAGE_SCN_LNK_COMDAT = 0x1000
SEL_NODUPLICATES = 1
_IDENT = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


@dataclass
class ExtSyms:
    defs: dict = field(default_factory=dict)     # name -> 'strong' | 'comdat' (mergeable) | 'common'
    refs: set = field(default_factory=set)       # undefined externals
    funcs: set = field(default_factory=set)      # defined names that are functions


def ext_symbols(path) -> ExtSyms:
    """External definitions and references of one COFF object (COMDAT selection read from the section aux)."""
    d = Path(path).read_bytes()
    _, nsec, _, psym, nsym, opt, _ = struct.unpack_from("<HHIIIHH", d, 0)
    chars = [struct.unpack_from("<I", d, 20 + opt + 40 * k + 36)[0] for k in range(nsec)]
    strtab = psym + 18 * nsym
    sel, syms = {}, []
    i = 0
    while i < nsym:
        raw, val, sec, typ, cls, naux = struct.unpack_from("<8sIhHBB", d, psym + 18 * i)
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            nm = d[strtab + off:d.index(b"\0", strtab + off)].decode("latin-1")
        else:
            nm = raw.rstrip(b"\0").decode("latin-1")
        if cls == 3 and naux and val == 0 and 0 < sec <= nsec and chars[sec - 1] & IMAGE_SCN_LNK_COMDAT \
                and sec not in sel:
            sel[sec] = d[psym + 18 * (i + 1) + 14]
        syms.append((nm, val, sec, typ, cls))
        i += 1 + naux
    out = ExtSyms()
    for nm, val, sec, typ, cls in syms:
        if cls != 2:
            continue
        if sec > 0:
            comdat = chars[sec - 1] & IMAGE_SCN_LNK_COMDAT and sel.get(sec, SEL_NODUPLICATES) != SEL_NODUPLICATES
            out.defs[nm] = "comdat" if comdat else "strong"
            if typ & 0x20:
                out.funcs.add(nm)
        elif sec == 0 and val:
            out.defs.setdefault(nm, "common")
        elif sec == 0:
            out.refs.add(nm)
    return out


def is_cpp(name: str) -> bool:
    return name.startswith("?")


def simple_id(name: str, undecorate) -> str | None:
    """The identifier to grep sources for (`?Total@Widget@@QAEHH@Z` -> Total, `_f@8` -> f), else None."""
    u = undecorate(name).split("::")[-1].lstrip("~")
    return u if _IDENT.match(u) and len(u) > 2 else None


def pair_names(before: dict, after: set, va_b: dict, va_a: dict, undecorate) -> dict:
    """{before name: after name} for names only on one side each: the same annotated VA, else the same
    undecorated name (exactly one on each side). `before`: {normalized name (renames applied): raw name}."""
    only_b = [n for n in before if n not in after]
    only_a = [n for n in after if n not in before]
    out = {}
    by_va = {va_a[n]: n for n in only_a if n in va_a}
    for n in only_b:
        m = by_va.get(va_b.get(n, -1))
        if m is not None and m not in out.values():
            out[n] = m
    gb, ga = collections.defaultdict(list), collections.defaultdict(list)
    for n in only_b:
        if n not in out:
            gb[undecorate(n)].append(n)
    for n in only_a:
        if n not in out.values():
            ga[undecorate(n)].append(n)
    for u, bs in gb.items():
        if len(bs) == 1 and len(ga.get(u, ())) == 1 and bs[0].startswith(("?", "_")) and \
                not bs[0].startswith("??_"):
            out[bs[0]] = ga[u][0]
    return out


def resolve(objs: dict, focus: set, pairs: dict, project: set, undecorate) -> list[str]:
    """Link problems of `focus` symbols over `objs` ({label: ExtSyms}). `project`: names known to be defined by the
    project (base definitions, journaled names): an unresolved reference to one of them is an error."""
    defs, refs, by_u = collections.defaultdict(list), collections.defaultdict(list), collections.defaultdict(set)
    for lab, o in sorted(objs.items()):
        for n, k in o.defs.items():
            defs[n].append((lab, k))
            by_u[undecorate(n)].add(n)
        for n in o.refs:
            refs[n].append(lab)
    out = []
    for s in sorted(focus):
        ds = defs.get(s, [])
        hard = [l for l, k in ds if k == "strong"]
        merge = [l for l, k in ds if k == "comdat"]
        if len(hard) > 1 or (hard and merge):
            out.append(f"LNK2005 duplicate definition of {s} ({undecorate(s)}): {', '.join(hard + merge)}")
        rs = refs.get(s, [])
        if rs and not ds:
            others = sorted(n for n in by_u.get(undecorate(s), ()) if n != s)
            if not others and s not in project:
                continue                                    # not the project's (CRT, import): LINK's business
            hint = ""
            if others:
                o = others[0]
                kind = "C vs C++ linkage mismatch" if is_cpp(o) != is_cpp(s) else "a different signature"
                hint = f"; defined as {o} in {', '.join(l for l, _ in defs[o])} ({kind})"
            out.append(f"LNK2001 unresolved external {s} ({undecorate(s)}) referenced by "
                       f"{', '.join(sorted(set(rs))[:6])}{hint}")
    for old, new in sorted(pairs.items()):
        if old in defs and new in defs:
            out.append(f"journaled {old} -> {new} but {old} is still defined in "
                       f"{', '.join(l for l, _ in defs[old])}: the callers now call a different overload")
    return out
