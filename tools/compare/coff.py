"""COFF objects (VC6 .obj) for the match compare: parse sections, symbols and relocations, and lay one object
out as a flat image ("link" it) so the same disassembly/normalisation as for DLLs applies.

Layout: every section at its own 16-byte aligned offset from OBJ_BASE; relocations are applied. External
symbols resolve as follows (`resolve(name) -> (address, kind)`):
  - `eqd_fn_0x...` / `eqd_va_0x...` (the EQD_MATCH expansion of eqaddr.h; also EQ_METHOD's eqd_tc<n><va, ...>::m
    and EQ_CDECL_CPP's eqd_cc<va>::f, whose decorated names carry the VA): the client VA itself, so the code
    reads exactly like a build linked at the client's layout (the field is then *not* a relocation for the
    normaliser: the number is the client's);
  - absolute symbols (section -1, or a CRT absolute like `__except_list`, ABSOLUTE_SYMBOLS): their value;
  - anything else: a fresh address in an extern area, recorded in `externs` (address -> name) for the caller
    to key (other rewrites, CRT functions, imports `__imp__X@n`).
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

OBJ_BASE = 0x10000000
EXTERN_BASE = 0x1F000000
# For an original image that covers OBJ_BASE (eqmain.dll, eqgfx_dx7.dll at 0x10000000): compare.MatchContext.
ALT_OBJ_BASE = 0x40000000
ALT_EXTERN_BASE = 0x4F000000
IMAGE_SCN_CNT_CODE = 0x20
IMAGE_SCN_LNK_REMOVE = 0x800
IMAGE_SCN_LNK_INFO = 0x200
REL_DIR32, REL_DIR32NB, REL_REL32 = 0x06, 0x07, 0x14
# Externals the CRT defines as absolute values, not addresses: LIBC's exsup.obj sets `__except_list` = 0 (the
# SEH chain head at fs:[0]; VC6 C++ EH code reads `fs:__except_list`). They resolve to the value, unrelocated,
# like an absolute symbol defined in the object itself (section -1).
ABSOLUTE_SYMBOLS = {"__except_list": 0}
_EQD = re.compile(r"^_?eqd_(fn|va)_0[xX]([0-9a-fA-F]+)(?:@\d+)?$")
# EQ_THISCALL in a C++ match build (eqaddr.h): a call to the member `m` of eqd_tc<N><VA, ...>, e.g.
# `?m@?$eqd_tc2@$0EMEOOH@XPAUCEverQuest@@HM@@QAEXHM@Z`; the VA is the first template argument in MSVC's number
# encoding ($0 + hex digits written A-P, or $00-$09 for 1-10).
_EQD_TC = re.compile(r"^\?m@\?\$eqd_tc(?:\d+|x)@\$0(?:([A-P]+)@|(\d))")
# EQ_CDECL_CPP / EQ_STDCALL_CPP in a C++ match build (eqaddr.h): the static member `f` of eqd_cc<VA>, a function
# with C++ linkage (VC6 keeps the EH states around a call to it), e.g. `?f@?$eqd_cc@$0BAAFMCNF@@@SAXXZ`.
_EQD_CC = re.compile(r"^\?f@\?\$eqd_cc@\$0(?:([A-P]+)@|(\d))")


@dataclass
class Section:
    name: str
    data: bytes
    chars: int
    relocs: list                    # (offset, symbol index, type)


@dataclass
class Symbol:
    name: str
    value: int
    section: int                    # 1-based, 0 = external/undefined, <0 = absolute/debug
    type: int
    storage: int


@dataclass
class Obj:
    sections: list
    symbols: list                   # index -> Symbol or None (aux records)


def parse(path) -> Obj:
    d = Path(path).read_bytes()
    _, nsec, _, psym, nsym, opt, _ = struct.unpack_from("<HHIIIHH", d, 0)
    strtab = psym + 18 * nsym
    syms: list = []
    i = 0
    while i < nsym:
        raw, val, sec, typ, cls, naux = struct.unpack_from("<8sIhHBB", d, psym + 18 * i)
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            nm = d[strtab + off:d.index(b"\0", strtab + off)].decode("latin-1")
        else:
            nm = raw.rstrip(b"\0").decode("latin-1")
        syms.append(Symbol(nm, val, sec, typ, cls))
        syms.extend([None] * naux)
        i += 1 + naux
    secs = []
    for k in range(nsec):
        name, _, _, size, praw, prel, _, nrel, _, ch = struct.unpack_from("<8sIIIIIIHHI", d, 20 + opt + 40 * k)
        relocs = [struct.unpack_from("<IIH", d, prel + 10 * r) for r in range(nrel)]
        data = d[praw:praw + size] if praw else bytes(size)
        secs.append(Section(name.rstrip(b"\0").decode("latin-1"), data, ch, relocs))
    return Obj(secs, syms)


def eqd_va(name: str):
    """(kind 'fn'|'va', client VA) for an EQD_MATCH symbol name, else None."""
    m = _EQD.match(name)
    if m:
        return m.group(1), int(m.group(2), 16)
    m = _EQD_TC.match(name) or _EQD_CC.match(name)
    if m:
        return "fn", (int("".join("0123456789ABCDEF"[ord(c) - 65] for c in m.group(1)), 16) if m.group(1)
                      else int(m.group(2)) + 1)
    return None


@dataclass
class Linked:
    data: bytearray
    base: int
    code_ranges: list
    symbols: dict = field(default_factory=dict)       # address -> name (defined functions/data)
    by_name: dict = field(default_factory=dict)
    externs: dict = field(default_factory=dict)       # extern-area address -> symbol name
    relocs: set = field(default_factory=set)          # DIR32 fields that point into the object/externs
    reloc_target: dict = field(default_factory=dict)  # field address -> symbol name (DIR32 and REL32)
    func_sizes: dict = field(default_factory=dict)    # function address -> its section's size
    rel32: set = field(default_factory=set)           # REL32 fields (the rest of reloc_target is DIR32)


def delinked_placement(obj: Obj) -> dict:
    """{section number: VA} for a delinked object (ghidra/scripts/delink.py): each section sits where its defined
    `_eqd_fn_`/`_eqd_va_` symbols say. ValueError if a section's symbols disagree (several packed ranges in
    one section: a unit object, not one function) or a section has none."""
    out: dict = {}
    for sym in obj.symbols:
        if not sym or sym.section <= 0:
            continue
        e = eqd_va(sym.name)
        if not e:
            continue
        at = e[1] - sym.value
        if out.setdefault(sym.section, at) != at:
            raise ValueError(f"section {sym.section} ({obj.sections[sym.section - 1].name}) holds several "
                             f"ranges ({out[sym.section]:#x} and {at:#x}); use one object per function")
    for k, s in enumerate(obj.sections, 1):
        if k not in out and s.data and not s.chars & (IMAGE_SCN_LNK_REMOVE | IMAGE_SCN_LNK_INFO):
            raise ValueError(f"section {k} ({s.name}) has no eqd symbol to place it")
    return out


def link(obj: Obj, placement: dict | None = None, base: int = OBJ_BASE, extern_base: int = EXTERN_BASE,
         client_vas: dict | None = None) -> Linked:
    """Lay the object out flat from `base` (OBJ_BASE), or with `placement` ({section number: VA}, e.g.
    delinked_placement) each listed section at that VA (others are left out); then apply the relocations.
    Unresolved externals get addresses from `extern_base` up. `client_vas`: {symbol name: client VA} for symbols
    that resolve to the client's address like an `eqd_*` symbol, defined in the object or not (vftables)."""
    flat_base = base
    off = 0
    place = {}
    if placement:
        base = min(placement.values()) & ~0xFFF
        for k, va in placement.items():
            place[k] = va - base
        off = max(place[k] + len(obj.sections[k - 1].data) for k in place)
    else:
        base = flat_base
        for k, s in enumerate(obj.sections, 1):
            if s.chars & (IMAGE_SCN_LNK_REMOVE | IMAGE_SCN_LNK_INFO) or s.name.startswith(".debug"):
                continue
            off = (off + 15) & ~15
            place[k] = off
            off += len(s.data)
    data = bytearray(((off + 0xFFF) & ~0xFFF) + 0x1000)
    code_ranges = []
    for k, o in place.items():
        s = obj.sections[k - 1]
        data[o:o + len(s.data)] = s.data
        if s.chars & IMAGE_SCN_CNT_CODE:
            code_ranges.append((base + o, len(s.data)))
    lk = Linked(data, base, code_ranges)
    ext_next = extern_base
    ext_addr: dict = {}

    def addr_of(sym: Symbol):
        nonlocal ext_next
        if client_vas and sym.name in client_vas:
            return client_vas[sym.name], False
        if sym.section > 0 and sym.section in place:
            return base + place[sym.section] + sym.value, True
        e = eqd_va(sym.name)
        if e:
            return e[1], False
        if sym.section == -1:                         # IMAGE_SYM_ABSOLUTE
            return sym.value, False
        if sym.section == 0 and sym.name in ABSOLUTE_SYMBOLS:
            return ABSOLUTE_SYMBOLS[sym.name], False
        if sym.name not in ext_addr:
            ext_addr[sym.name] = ext_next
            lk.externs[ext_next] = sym.name
            ext_next += 0x10
        return ext_addr[sym.name], True

    for sym in obj.symbols:
        if sym and sym.section > 0 and sym.section in place and sym.storage in (2, 3) and \
                not sym.name.startswith((".", "$")):
            a = base + place[sym.section] + sym.value
            lk.symbols.setdefault(a, sym.name)
            lk.by_name.setdefault(sym.name, a)
            if sym.type & 0x20:
                lk.func_sizes[a] = len(obj.sections[sym.section - 1].data) - sym.value
    for k, o in place.items():
        for roff, si, typ in obj.sections[k - 1].relocs:
            sym = obj.symbols[si]
            fva = base + o + roff
            tgt, relocated = addr_of(sym)
            cur = struct.unpack_from("<i", data, o + roff)[0]
            if typ == REL_DIR32:
                struct.pack_into("<I", data, o + roff, (tgt + cur) & 0xFFFFFFFF)
                if relocated:
                    lk.relocs.add(fva)
            elif typ == REL_REL32:
                struct.pack_into("<I", data, o + roff, (tgt + cur - (fva + 4)) & 0xFFFFFFFF)
                lk.rel32.add(fva)
            elif typ == REL_DIR32NB:
                continue
            lk.reloc_target[fva] = sym.name
    return lk
