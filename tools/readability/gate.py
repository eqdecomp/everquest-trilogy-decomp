r"""Level-1 readability gate (T0): every translation unit an edit touches compiles to the same code as on main.

    cd eq-trilogy            (a worktree, a copy, or the main checkout)
    ..\.venv\Scripts\python.exe tools\readability\gate.py [--base main] [FILES...] [--renames F] [--json F] [--protos F]

No fixtures, no mutation, no original binary: both sides are built here and compared with each other, so it
works the same for identical, structural and giant functions.

1. Changed files: FILES, else `git diff --name-only <base>` plus untracked files, without `.check/`
   and other build litter. A changed header pulls in every source that includes it.
2. "Before" tree: `git archive <base> src include` into <build>/readability/base/<sha> (cached).
3. One EQD_MATCH header for both builds, from the VA tokens of BOTH trees (compare.match_symbols, union): a
   header that differs between the builds moves code on its own (00428686 ndiff 37/47/77).
4. Each TU compiled before and after with EQD_MATCH (its client's toolchain and CFLAGS) and normally (no
   EQD_MATCH): both must compile, and no new C4005 (macro redefined) warning.
5. Object compare per function (COFF, /Gy sections): bytes plus relocations, where a relocation's target is
   keyed by what it is, not by its spelling: `eqd_fn_0x...`/`eqd_va_0x...` by VA (any letter case), a
   function of the TU by its annotated VA, a function of ANOTHER TU by the VA its definition is annotated with
   (cross-TU calls, compare/xtu.py: `??1EQ_Overlay@@QAE@XZ` defined in EQ_Overlay.cpp under `// FUNCTION:
   TRILOGY 0x0045005d` keys like `eqd_fn_0x0045005D`; only a decorated name that a built TU of the client defines
   once under an annotated name with one VA; anything else keeps its name), a label by its function and offset, a
   static/string by its section and offset, other symbols by name after the renames (old -> new from the TUs'
   annotations on both sides, plus --renames journal lines from rename.py). A call to a pure COMDAT thunk of the
   TU (an unannotated inline function that is one `jmp rel32`, e.g. a TU-local stand-in's destructor calling the
   real one by address, whose address is never taken) is keyed as its target, and the thunk is no function of
   its own. Data sections are compared the same way. Any difference fails, naming the function. Notes name the
   cross-TU keys used and the thunks only one side has.
6. source_checks per unit (by VA; unannotated ones by name after renames): no count higher than before
   (counts, not label text: a renamed goto label or unused local is the same count), no new marker errors,
   and no marker count LOWER than before: `@early-stop`-style markers, `fake match` comments (`/* fake match */`
   blocks and the lines of `// fake match:` notes), `as original` notes, and the identification / contract
   words in comments (zlib, __cdecl, __thiscall, __fastcall, __stdcall, EAX, Returns). A dropped marker hides
   an oddity; a dropped word is a lost fact (an audit found a comment pass that replaced headers and lost them)).
   A TU that differs only under that shared header is rechecked with each tree's own generated header: when
   both sides compile to the same code there it passes, reported as "header-sensitive, own-header PASS"
   (1005FCB3 in the pilot). Any difference under the own headers still fails.
7. Prototype journal (protolink.py): a definition whose decorated name changed at the same annotated VA, or a
   definition/reference whose undecorated name is the same on both sides (one each), is journaled old -> new
   (`?f@@YAXH@Z` -> `?f@@YAXJ@Z`, a retyped prototype). The diff keys a journaled target as the same symbol, so
   callers of a retyped function don't fail. --protos adds journal lines, --journal-out writes them.
8. Link check (only when something is journaled or a changed TU adds/removes an external definition): the
   changed TUs plus every TU of that client naming one of those symbols are resolved against each other: no
   duplicate definition (LNK2005), no unresolved reference to a project symbol (LNK2001, with the C/C++ linkage
   or signature it is defined with), no journaled old name still defined (a caller switched overloads).
9. Lint: every fixture of a changed TU (or a changed fixture) names a function annotated at its "original"
   in one of its "sources" (findings the base already has are listed, not counted).

Exit 0 = pass, 1 = fail, 2 = setup problem. The before objects are cached per base commit, header and file.
Importable: run_gate(repo, base, files, renames) -> GateResult.
"""
from __future__ import annotations

import argparse
import collections
import concurrent.futures as cf
import hashlib
import io
import json
import re
import subprocess
import sys
import tarfile
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import registry as R  # noqa: E402  (puts compare/, unicorn/, permute/ on sys.path)
import protolink as PL  # noqa: E402

import coff  # noqa: E402
import compare as cmp  # noqa: E402
import source_checks as sc  # noqa: E402
import xtu as X  # noqa: E402
from eqd_unicorn import Workspace, annotations as ann, clients, undecorate  # noqa: E402
from eqd_unicorn.vc6 import cflags_for, source_vc6, vc6_env  # noqa: E402

LITTER = re.compile(r"(^|/)(\.check|__pycache__|\.pytest_cache)/|\.(obj|pdb|ilk|exe|dll|map|lst|cod)$", re.I)
_INC = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)
SKIP_SECTIONS = re.compile(r"^\.(debug|drectve)")
WARN_4005 = re.compile(r"warning C4005", re.I)


@dataclass
class TUResult:
    rel: str
    client: str
    functions: int = 0
    changed: list = field(default_factory=list)      # (key, name, why)
    errors: list = field(default_factory=list)
    checks: list = field(default_factory=list)       # source_checks regressions
    notes: list = field(default_factory=list)        # informational ("header-sensitive, own-header PASS")

    @property
    def ok(self):
        return not (self.changed or self.errors or self.checks)


@dataclass
class GateResult:
    base: str
    files: list
    tus: list = field(default_factory=list)
    lint: list = field(default_factory=list)
    known_lint: list = field(default_factory=list)  # lint findings the base has already (not counted)
    problems: list = field(default_factory=list)
    protos: list = field(default_factory=list)      # prototype journal: {client, tu, va, old, new, kind}
    link: list = field(default_factory=list)        # link check failures
    link_notes: list = field(default_factory=list)

    @property
    def ok(self):
        return not self.problems and not self.lint and not self.link and all(t.ok for t in self.tus)

    def report(self) -> str:
        out = [f"readability gate vs {self.base[:12]}: {len(self.files)} changed file(s), {len(self.tus)} TU(s), "
               f"{sum(t.functions for t in self.tus)} functions"]
        for p in self.problems:
            out.append(f"  PROBLEM {p}")
        for t in self.tus:
            out.append(f"  {'ok  ' if t.ok else 'FAIL'} {t.rel} ({t.client}): {t.functions} functions"
                       + (f", {len(t.changed)} compile differently" if t.changed else ""))
            for n in t.notes:
                out.append(f"       note: {n}")
            for e in t.errors:
                out.append(f"       error: {e}")
            for k, n, why in t.changed[:40]:
                out.append(f"       changed {k} {n}: {why}")
            for c in t.checks[:40]:
                out.append(f"       source_checks: {c}")
        for j in self.protos:
            out.append(f"  proto {j['client']} {j.get('va') or '-'} {j['old']} -> {j['new']} ({j['kind']}, {j['tu']})")
        for n in self.link_notes:
            out.append(f"  link: {n}")
        for l in self.link:
            out.append(f"  LINK FAIL {l}")
        for l in self.lint:
            out.append(f"  LINT {l}")
        if self.known_lint:
            out.append(f"  ({len(self.known_lint)} lint finding(s) already on the base, not counted)")
        out.append("PASS" if self.ok else "FAIL")
        return "\n".join(out)

    def as_json(self):
        return {"ok": self.ok, "base": self.base, "files": self.files, "problems": self.problems,
                "lint": self.lint, "protos": self.protos, "link": self.link, "link_notes": self.link_notes,
                "tus": [{"rel": t.rel, "client": t.client, "ok": t.ok, "functions": t.functions,
                         "changed": t.changed, "errors": t.errors, "checks": t.checks,
                         "notes": t.notes} for t in self.tus]}


# --- git ------------------------------------------------------------------------------------------------------

def git(repo: Path, *args, binary=False):
    p = subprocess.run(["git", *args], cwd=repo, capture_output=True)
    if p.returncode:
        raise RuntimeError(f"git {' '.join(args)}: {p.stderr.decode(errors='replace')[-300:]}")
    return p.stdout if binary else p.stdout.decode("utf-8", errors="replace")


def changed_files(repo: Path, base: str) -> list[str]:
    out = set(git(repo, "diff", "--name-only", base).split())
    out |= set(git(repo, "ls-files", "--others", "--exclude-standard").split())
    out |= set(git(repo, "diff", "--name-only", "--cached", base).split())
    return sorted(f for f in out if not LITTER.search(f))


def base_tree(repo: Path, sha: str, cache: Path) -> Path:
    d = cache / sha[:16]
    if (d / ".complete").is_file():
        return d
    d.mkdir(parents=True, exist_ok=True)
    data = git(repo, "archive", "--format=tar", sha, "src", "include", "tests/unicorn", binary=True)
    with tarfile.open(fileobj=io.BytesIO(data)) as t:
        t.extractall(d, filter="data")
    (d / ".complete").write_text(sha, encoding="utf-8")
    return d


# --- include graph --------------------------------------------------------------------------------------------

def include_graph(root: Path) -> dict:
    """{resolved .c/.cpp/.h path: {resolved local headers it includes}} of src/ and include/."""
    files = [p for d in ("src", "include") if (root / d).is_dir() for p in (root / d).rglob("*")
             if p.suffix.lower() in (".c", ".cpp", ".h") and p.is_file()]
    graph = {}
    for p in files:
        txt = p.read_text(encoding="utf-8", errors="replace")
        deps = set()
        for name in _INC.findall(txt):
            for b in (p.parent, root / "include"):
                c = (b / name)
                if c.is_file():
                    deps.add(c.resolve())
                    break
        graph[p.resolve()] = deps
    return graph


def include_closure(graph: dict, p: Path) -> list:
    seen, todo = set(), [p.resolve()]
    while todo:
        q = todo.pop()
        if q not in seen:
            seen.add(q)
            todo.extend(graph.get(q, ()))
    return sorted(seen)


def dependents(root: Path, headers: list[str], graph: dict | None = None) -> list[str]:
    """Repo-relative .c/.cpp files under src/ that include (transitively) one of `headers`."""
    graph = include_graph(root) if graph is None else graph
    want = {(root / h).resolve() for h in headers}
    memo = {}

    def reaches(p, stack=()):
        if p in memo:
            return memo[p]
        if p in stack:
            return False
        r = bool(graph.get(p, set()) & want) or any(reaches(c, stack + (p,)) for c in graph.get(p, ()))
        memo[p] = r
        return r
    root_r = root.resolve()
    return sorted(p.relative_to(root_r).as_posix() for p in graph
                  if p.suffix.lower() in (".c", ".cpp") and reaches(p))


# --- compile ----------------------------------------------------------------------------------------------------

def compile_tu(ws: Workspace, root: Path, rel: str, client: str, out: Path, header: Path | None):
    """-> (obj or None, compiler output). header None = a normal (non-match) compile."""
    src = root / rel
    out.mkdir(parents=True, exist_ok=True)
    obj = out / (re.sub(r"[^A-Za-z0-9]+", "_", rel) + (".match.obj" if header else ".obj"))
    flags = cflags_for(src)
    vc6 = source_vc6(src, ws.vc6(client))
    cmd = [str(vc6 / "Bin" / "CL.EXE")] + flags + (["/DEQD_MATCH", f"/FI{header}"] if header else []) + \
        [f"/I{root / 'include'}", f"/Fo{obj}", str(src)]
    p = subprocess.run(cmd, env=vc6_env(vc6), capture_output=True, text=True, errors="replace", cwd=out)
    return (obj if p.returncode == 0 and obj.is_file() else None), p.stdout + p.stderr


# --- object signatures ----------------------------------------------------------------------------------------

_ANON_NS = re.compile(r"\?%[^@]*@")


def _anon(sym: str) -> str:
    """MSVC names an anonymous namespace `?%<source path><hash>@`; the path differs between the base and the edited
    tree and the hash changes with any edit of the file (a comment too), so it is no codegen difference."""
    return _ANON_NS.sub("?%anon@", sym)


def _ids_rename(sym: str, renames: dict) -> str:
    """Replace whole identifiers of a (decorated) symbol name: `?Old@Cls@@QAEHXZ`, `_Old`, `_Old@8`."""
    if not renames:
        return sym
    lead = re.match(r"^[_?@]*", sym).group(0)
    body = re.sub(r"[A-Za-z_][A-Za-z0-9_]*", lambda m: renames.get(m.group(0), m.group(0)), sym[len(lead):])
    if lead.endswith("_") and lead != "_":                # `__name`: keep the CRT's double underscore as is
        return sym
    return lead + body


def _norm(sym: str, renames: dict, protos: dict | None = None) -> str:
    """A before-side symbol name as the after side spells it: anonymous namespace, renames, then the prototype
    journal (old decorated -> new decorated)."""
    n = _ids_rename(_anon(sym), renames)
    return protos.get(n, n) if protos else n


def _thunks(o, secs, fsyms, ann_va) -> dict:
    """{symbol index: jump target symbol index} of the TU's pure COMDAT thunks: an unannotated function that fills
    its own COMDAT section with one `jmp rel32` (E9 + a REL32 relocation), e.g. the inline destructor of a TU-local
    base that only calls the real one by address. A call to it is the call to its target (the funclet of a derived
    constructor: `jmp ??1Stand_in` -> `jmp eqd_tc 0x0045005D` is `jmp ??1EQ_Overlay`). A thunk whose address is
    taken (a DIR32 relocation anywhere in the object) is not one: a function pointer's identity would change."""
    out = {}
    for sec, val, idx, name in fsyms:
        s = secs[sec - 1]
        if val or len(s.data) != 5 or s.data[0] != 0xE9 or not s.chars & PL.IMAGE_SCN_LNK_COMDAT                 or undecorate(name) in ann_va:
            continue
        rel = [(off, si, typ) for off, si, typ in s.relocs]
        if len(rel) == 1 and rel[0][0] == 1 and rel[0][2] == coff.REL_REL32 and rel[0][1] != idx:
            out[idx] = rel[0][1]
    if out:
        for s in secs:
            for _off, si, typ in s.relocs:
                if typ == coff.REL_DIR32:
                    out.pop(si, None)
    return out


def obj_signatures(path: Path, ann_va: dict, renames: dict, protos: dict | None = None, xtu: dict | None = None,
                   info: dict | None = None) -> tuple[dict, dict, dict]:
    """({function key: sha}, {data section key: sha}, {function key: name}) of one COFF object.
    `xtu`: {decorated name: VA} of functions defined (and annotated) in OTHER TUs (xtu.annotated_defs): a
    reference to one is keyed by that VA, like an EQD_MATCH call to it. `info` (a dict, filled in): "xtu" =
    the symbols keyed that way, "thunks" = the pure COMDAT thunks seen through (_thunks)."""
    o = coff.parse(path)
    secs = o.sections
    ordinal, seen = {}, collections.Counter()
    for i, s in enumerate(secs, 1):
        seen[s.name] += 1
        ordinal[i] = f"{s.name}#{seen[s.name]}"
    fsyms = []                                  # (section, value, index, name)
    for idx, s in enumerate(o.symbols):
        if s and s.section > 0 and (s.type & 0x20) and s.storage in (2, 3):
            fsyms.append((s.section, s.value, idx, s.name))
    thunks = _thunks(o, secs, fsyms, ann_va)
    info = info if info is not None else {}
    info.setdefault("xtu", set())
    info.setdefault("thunks", set())
    fkey = {}
    names = {}
    used = collections.Counter()
    for sec, val, idx, name in sorted(fsyms):
        u = undecorate(name)
        k = f"{ann_va[u]:08X}" if u in ann_va else _norm(name, renames, protos)
        used[k] += 1
        if used[k] > 1:
            k = f"{k}#{used[k]}"
        fkey[idx] = k
        names[k] = u
    fn_at = collections.defaultdict(list)       # section -> [(value, index)] of its functions
    for sec, val, idx, name in sorted(fsyms):
        fn_at[sec].append((val, idx))

    def target(idx: int, depth: int = 0) -> str:
        s = o.symbols[idx]
        if idx in thunks and depth < 8:
            info["thunks"].add(s.name)
            return target(thunks[idx], depth + 1)
        e = coff.eqd_va(s.name)
        if e:
            return f"{e[0]}:{e[1]:08X}"
        if idx in fkey:
            return "fn:" + fkey[idx]
        if s.section > 0:
            if s.storage == 2:                   # external data defined here: by (renamed) name
                return "sym:" + _norm(s.name, renames, protos)
            inside = [(v, i) for v, i in fn_at.get(s.section, ()) if v <= s.value]
            if inside:                           # a label of a function (switch table, funclet): by the function,
                v, i = inside[-1]                # not by section ordinal (a COMDAT that comes or goes shifts those)
                return f"in:{fkey[i]}+{s.value - v}"
            return f"sec:{ordinal[s.section]}+{s.value}"
        if s.section < 0:
            return f"abs:{s.name}:{s.value}"
        if xtu and s.name in xtu:                # another TU's annotated function: by its VA
            info["xtu"].add(s.name)
            return f"fn:{xtu[s.name]:08X}"
        return "ext:" + _norm(s.name, renames, protos)

    def sig(data: bytes, relocs, lo, hi) -> str:
        h = hashlib.sha1(data[lo:hi])
        for off, si, typ in sorted(relocs):
            if lo <= off < hi:
                h.update(f"|{off - lo}:{typ}:{target(si)}".encode())
        return h.hexdigest()[:16]

    fn_sig = {}
    by_sec = collections.defaultdict(list)
    for sec, val, idx, name in sorted(fsyms):
        by_sec[sec].append((val, idx))
    for sec, items in by_sec.items():
        s = secs[sec - 1]
        for n, (val, idx) in enumerate(items):
            if idx in thunks:                    # seen through at every call: not a function of its own
                continue
            end = items[n + 1][0] if n + 1 < len(items) else len(s.data)
            fn_sig[fkey[idx]] = sig(s.data, s.relocs, val, end)
    data_sig = {}
    for i, s in enumerate(secs, 1):
        if i in by_sec or SKIP_SECTIONS.match(s.name) or s.chars & (coff.IMAGE_SCN_LNK_REMOVE | coff.IMAGE_SCN_LNK_INFO):
            continue
        if s.chars & coff.IMAGE_SCN_CNT_CODE and not s.data:
            continue
        data_sig[ordinal[i]] = sig(s.data, s.relocs, 0, max(len(s.data), 1)) + f"/{len(s.data)}"
    return fn_sig, data_sig, names


def diff_objects(ob: Path, oa: Path, before_src: Path, after_src: Path, client: str, renames: dict, protos=None,
                 xtu_b=None, xtu_a=None, notes=None):
    """-> ([(key, name, why)] of the functions/data that differ, number of functions after). `protos`: the
    prototype journal {old decorated: new decorated} (before side). `xtu_b`/`xtu_a`: each tree's {decorated:
    VA} of other TUs' annotated functions (obj_signatures). `notes` (a list): what was keyed by VA or seen through."""
    ib, ia = {}, {}
    if xtu_b and protos:
        # a journaled reference (old -> new) whose new name the after side does not key by VA stays a name on both
        # sides, so the journal pairs it as before (an undefined new name is the link check's to report)
        xtu_b = {n: v for n, v in xtu_b.items()
                 if (m := protos.get(_norm(n, renames))) is None or m == n or (xtu_a or {}).get(m) is not None}
    fb_, dsb, nb = obj_signatures(ob, _ann_map(before_src, client), renames, protos, xtu_b, ib)
    fa_, dsa, na = obj_signatures(oa, _ann_map(after_src, client), {}, None, xtu_a, ia)
    changed = []
    for k in sorted(set(fb_) | set(fa_)):
        if k not in fa_:
            changed.append((k, nb.get(k, ""), "missing after the edit"))
        elif k not in fb_:
            changed.append((k, na.get(k, ""), "new after the edit (not in the base build)"))
        elif fb_[k] != fa_[k]:
            changed.append((k, na.get(k, ""), "compiled code differs"))
    for k in sorted(set(dsb) | set(dsa)):
        if dsb.get(k) != dsa.get(k):
            changed.append((k, "", "data section differs"))
    if notes is not None:
        for side, inf, m, other in (("before", ib, xtu_b, ia), ("after", ia, xtu_a, ib)):
            if inf["xtu"]:
                notes.append(f"{side}: cross-TU call(s) keyed by the definition's VA: "
                             + ", ".join(f"{s}={m[s]:08X}" for s in sorted(inf["xtu"])))
            only = sorted(inf["thunks"] - other["thunks"])      # a thunk on both sides is no news
            if only:
                notes.append(f"{side} only: COMDAT jmp thunk(s) seen through as their target: {', '.join(only)}")
    return changed, len(fa_)


def _own_header(hdr_dir: Path, fns, data) -> Path:
    """The match header of ONE tree's own VA tokens (content-addressed, like the shared one)."""
    tmp = hdr_dir / "tmp_own.h"
    cmp.write_match_header(tmp, fns, data)
    text = tmp.read_text(encoding="utf-8")
    p = hdr_dir / f"eqd_match_syms_{hashlib.sha1(text.encode()).hexdigest()[:12]}.h"
    if not p.is_file():
        p.write_text(text, encoding="utf-8")
    return p


def _build_before_own(ws, before_root: Path, rel: str, client: str, bdir: Path, header: Path):
    """The base TU under the base tree's own header (cached next to the other before builds)."""
    done = bdir / f"match_own_{header.stem[-12:]}.done"
    if done.is_file():
        meta = json.loads(done.read_text(encoding="utf-8"))
        return Path(meta["obj"]) if meta["obj"] else None
    o, text = compile_tu(ws, before_root, rel, client, bdir / f"match_own_{header.stem[-12:]}", header)
    done.write_text(json.dumps({"obj": str(o) if o else None, "text": text}), encoding="utf-8")
    return o


# --- the gate ---------------------------------------------------------------------------------------------------

def _ann_map(path: Path, client: str) -> dict:
    try:
        return {a.name: a.va for a in ann.scan([path], clients.get(client).token)}
    except (ValueError, OSError):
        return {}


def _renames_from_anns(before: dict, after: dict) -> dict:
    """{old simple name: new simple name} for VAs annotated on both sides under different names."""
    inv_b = collections.defaultdict(set)
    for n, va in before.items():
        inv_b[va].add(n)
    out = {}
    for n, va in after.items():
        for o in inv_b.get(va, ()):
            if o != n and R.simple(o) != R.simple(n):
                out[R.simple(o)] = R.simple(n)
    return out


def marker_counts(res) -> dict:
    """{unit: Counter} of what must never go down in a readability edit: `@marker`s, `fake match` comments (and the
    lines of their notes) and `as original` notes."""
    out = collections.defaultdict(collections.Counter)
    for _line, unit, name, _text in res.markers:
        out[unit]["@" + name] += 1
    for unit, cnt in getattr(res, "notes", {}).items():
        out[unit].update(cnt)
    return out


def check_counts(before_path: Path, after_path: Path, rel: str, client: str, renames: dict) -> list[str]:
    tok = clients.get(client).token
    b = sc.scan_file(before_path, rel, tok)
    a = sc.scan_file(after_path, rel, tok)

    def key(k):
        if ":" in k and not re.fullmatch(r"[0-9A-F]{8}", k):
            p, _, n = k.rpartition(":")
            base, hash_, num = n.partition("#")
            n2 = "::".join(renames.get(x, x) for x in base.split("::"))
            return f"{p}:{n2}{hash_}{num}"
        return k
    bu = {key(k): v for k, v in b.units.items()}
    bm = {key(k): v for k, v in marker_counts(b).items()}
    am = marker_counts(a)
    out = []
    for k, cnt in a.units.items():
        old = bu.get(k, collections.Counter())
        for m, v in cnt.items():
            if v > old.get(m, 0):
                out.append(f"{k}: {m} {old.get(m, 0)} -> {v}")
    for k, cnt in sorted(bm.items()):                    # a marker that disappeared hides an oddity
        new = am.get(k, collections.Counter())
        for m, v in sorted(cnt.items()):
            if m == "kw:returns":                        # lower-case `returns` only counts toward `Returns` (a rewritten summary)
                continue
            now = new.get(m, 0) + (new.get("kw:returns", 0) if m == "kw:Returns" else 0)
            if now < v:
                out.append(f"{k}: marker {m} dropped {v} -> {now}" if not m.startswith("kw:") else
                           f"{k}: comment word {m[3:]} dropped {v} -> {now}")
    if len(a.errors) > len(b.errors):
        out += [f"new marker error: {e.unit} line {e.line}: {e.msg}" for e in a.errors[len(b.errors):]][:10] or \
               [f"marker errors {len(b.errors)} -> {len(a.errors)}"]
    return out


def lint_fixtures(repo: Path, tus: set, changed: list) -> list[str]:
    out = []
    tdir = repo / "tests" / "unicorn"
    if not tdir.is_dir():
        return out
    cache = {}
    for f in sorted(tdir.rglob("*.json")):
        rel = f.relative_to(repo).as_posix()
        try:
            d = json.loads(f.read_text(encoding="utf-8"))
        except (ValueError, OSError):
            continue
        if not isinstance(d, dict) or "function" not in d or "original" not in d:
            continue
        srcs = d.get("sources") or []
        if rel not in changed and not (set(srcs) & tus):
            continue
        client = clients.client_of_path(rel)
        try:
            va = int(str(d["original"]), 16)
        except ValueError:
            continue
        names = set()
        for s in srcs:
            if s not in cache:
                cache[s] = _ann_map(repo / s, client) if (repo / s).is_file() else {}
            names |= {n for n, v in cache[s].items() if v == va}
        fn = d["function"]
        if not names:
            out.append(f"{rel}: no function annotated at {va:08X} in {', '.join(srcs) or '(no sources)'}")
        elif fn not in names and R.simple(fn) not in {R.simple(n) for n in names}:
            out.append(f"{rel}: \"function\" {fn} but {va:08X} is {', '.join(sorted(names))}")
    return out


def tu_journal(ob: Path, oa: Path, before_src: Path, after_src: Path, client: str, rel: str, renames: dict) -> list:
    """Prototype journal entries of one TU from its before/after (match) objects: definitions paired by annotated
    VA, then definitions and references paired by undecorated name."""
    sb, sa = PL.ext_symbols(ob), PL.ext_symbols(oa)
    ann_b, ann_a = _ann_map(before_src, client), _ann_map(after_src, client)
    out = []
    bd = {_norm(n, renames): n for n in sb.defs}
    va_b = {k: ann_b[undecorate(n)] for k, n in bd.items() if undecorate(n) in ann_b}
    va_a = {n: ann_a[undecorate(n)] for n in sa.defs if undecorate(n) in ann_a}
    for old, new in sorted(PL.pair_names(bd, set(sa.defs), va_b, va_a, undecorate).items()):
        va = va_a.get(new)
        out.append({"client": client, "tu": rel, "va": f"{va:08X}" if va is not None else None,
                    "old": old, "new": new, "kind": "def"})
    br = {_norm(n, renames): n for n in sb.refs if not coff.eqd_va(n)}
    ar = {n for n in sa.refs if not coff.eqd_va(n)}
    for old, new in sorted(PL.pair_names(br, ar, {}, {}, undecorate).items()):
        out.append({"client": client, "tu": rel, "va": None, "old": old, "new": new, "kind": "ref"})
    return out


# a header line that can reference or define a symbol: a macro body or an inline function body
_DEF_OR_USE_LINE = re.compile(r"\binline\b|\{")
_DEFINE_LINE = re.compile(r"^\s*#\s*define\s+\w+(?:\([^)]*\))?(.*)$")


def _header_uses(line: str, word) -> bool:
    """True if a header line can define or reference a symbol named by `word`: in a macro's body (not as the macro's
    own name: `#define f EQ_CDECL(int, 0x..., (int))` calls by VA), or on an inline/brace line."""
    if line.lstrip().startswith("//"):
        return False
    m = _DEFINE_LINE.match(line)
    if m:
        return bool(word.search(m.group(1)))
    return bool(_DEF_OR_USE_LINE.search(line) and word.search(line))


def link_candidates(repo: Path, client: str, ids: set, skip: set, graph_fn) -> list[str]:
    """Sources of `client` that name one of `ids` (a word), plus the dependents of headers that use one in a macro
    or an inline body (a bare prototype in a header neither defines nor references anything)."""
    if not ids:
        return []
    word = re.compile(r"\b(?:" + "|".join(sorted(map(re.escape, ids))) + r")\b")
    out, hdrs = set(), []
    for d in ("src", "include"):
        if not (repo / d).is_dir():
            continue
        for p in (repo / d).rglob("*"):
            suf = p.suffix.lower()
            if suf not in (".c", ".cpp", ".h") or not p.is_file():
                continue
            rel = p.relative_to(repo).as_posix()
            if suf != ".h" and clients.client_of_path(rel) != client:
                continue
            txt = p.read_text(encoding="utf-8", errors="replace")
            if not word.search(txt):
                continue
            if suf == ".h":
                if any(_header_uses(l, word) for l in txt.splitlines()):
                    hdrs.append(rel)
            elif d == "src":
                out.add(rel)
    if hdrs:
        out |= {r for r in dependents(repo, hdrs, graph_fn()) if clients.client_of_path(r) == client}
    return sorted(out - skip)


def _build_candidates(ws, root: Path, graph, client: str, cands: list, cache: Path, jobs: int, tag: str) -> dict:
    """{rel: normal object or None (does not build; None also when `root` has no such file)} for the candidate TUs."""
    def build(rel):
        if not (root / rel).is_file():
            return rel, None
        h = hashlib.sha1(f"{client}|{rel}|{tag}|".encode())
        for p in include_closure(graph, root / rel):
            if p.is_file():
                h.update(p.read_bytes())
        d = cache / client / f"{re.sub(r'[^A-Za-z0-9]+', '_', rel)}_{h.hexdigest()[:12]}"
        done = d / "normal.done"
        if done.is_file():
            o = json.loads(done.read_text(encoding="utf-8"))["obj"]
            return rel, Path(o) if o else None
        o, _text = compile_tu(ws, root, rel, client, d, None)
        done.write_text(json.dumps({"obj": str(o) if o else None}), encoding="utf-8")
        return rel, o
    with cf.ThreadPoolExecutor(max_workers=max(1, jobs)) as ex:
        return dict(ex.map(build, cands))


def link_check(ws, repo: Path, client: str, after: dict, before: dict, protos: dict, file_names: set, cache: Path,
               jobs: int, graph_fn, base_root: Path | None = None, base_graph_fn=None) -> tuple[list, list]:
    """(failures, notes) of the symbol resolution for one client. `after`/`before`: {rel: normal object} of the
    changed TUs; `protos`: the client's journal {old: new}; `file_names`: journaled names from --protos.
    A candidate TU (not changed, but naming a focus symbol) that does not build FAILS the check: a stale caller in
    it could be an unresolved external nobody sees; unless it does not build on the base either.
    `base_root`: the base tree; the same resolution is run over its objects and only problems that are NEW against
    it fail (a duplicate or unresolved reference main already has is not this change's)."""
    sa = {rel: PL.ext_symbols(o) for rel, o in after.items()}
    sb = {rel: PL.ext_symbols(o) for rel, o in before.items()}
    a_defs = {n for s in sa.values() for n, k in s.defs.items() if k != "common"}
    b_defs = {n for s in sb.values() for n, k in s.defs.items() if k != "common"}
    added, removed = a_defs - b_defs, b_defs - a_defs
    focus = added | removed | set(protos) | set(protos.values())
    if not focus:
        return [], []
    project = b_defs | removed | set(protos) | set(protos.values()) | file_names
    ids = {i for n in focus if (i := PL.simple_id(n, undecorate))}
    cands = link_candidates(repo, client, ids, set(after), graph_fn)
    objs = dict(sa)
    broken = []
    base_objs = dict(sb)
    base_broken = set()
    if cands:
        for rel, o in _build_candidates(ws, repo, graph_fn(), client, cands, cache, jobs, "after").items():
            if o is None:
                broken.append(rel)
            else:
                objs[rel] = PL.ext_symbols(o)
        if base_root is not None:
            bg = base_graph_fn() if base_graph_fn else graph_fn()
            for rel, o in _build_candidates(ws, base_root, bg, client, cands, cache, jobs, "base").items():
                if o is not None:
                    base_objs[rel] = PL.ext_symbols(o)
                elif (base_root / rel).is_file():
                    base_broken.add(rel)
    problems = PL.resolve(objs, focus, protos, project, undecorate)
    if base_root is not None:
        old = set(PL.resolve(base_objs, focus, protos, project, undecorate))
        problems = [f for f in problems if f not in old]
    fails = [f"{client}: {f}" for f in problems]
    unbuilt = [r for r in broken if r not in base_broken]
    fails += [f"{client}: {r} names a changed symbol but does not build, so its references are unchecked "
              f"(not known to be broken on the base)" for r in unbuilt]
    note = (f"{client}: {len(focus)} symbol(s) resolved over {len(objs)} object(s) ({len(after)} changed, "
            f"{len(cands) - len(broken)} more naming them)"
            + (f"; {len(broken) - len(unbuilt)} do not build on the base either, not checked: "
               f"{', '.join(sorted(set(broken) - set(unbuilt))[:5])}" if len(broken) > len(unbuilt) else ""))
    return fails, [note]


def cross_tu_maps(ws, built, before_root: Path, repo: Path, graph_of, cache: Path, jobs: int) -> dict:
    """{(side, client): {decorated name: VA}} for the external references of the changed TUs' match objects that
    another TU of the client defines and annotates (xtu.py): the sources whose `// FUNCTION:` names match a
    reference are built (normal compile, cached with the link check's) and only their annotated definitions
    count. A defining TU that does not build resolves nothing (its references keep their symbol key)."""
    out = {}
    for side, root, tag in (("before", before_root, "base"), ("after", repo, "after")):
        refs, own = collections.defaultdict(set), collections.defaultdict(dict)
        for rel, builds, _bdir, _checks in built:
            client = clients.client_of_path(rel)
            o = builds[side, "match"][0]
            if o is not None:
                refs[client] |= X.external_refs(o)
            on = builds[side, "normal"][0]
            if on is not None:
                own[client][rel] = on
        for client, rs in refs.items():
            tok = clients.get(client).token
            anns = X.scan_sources(clients.sources(root, client), tok)
            index = X.annotation_index(anns)
            root_r = root.resolve()
            cands = set()
            for p in X.defining_sources(rs, index, undecorate):
                try:
                    cands.add(Path(p).resolve().relative_to(root_r).as_posix())
                except ValueError:
                    continue
            if not cands:
                continue
            by_rel = collections.defaultdict(list)
            for a in anns:
                try:
                    by_rel[Path(a.path).resolve().relative_to(root_r).as_posix()].append(a)
                except ValueError:
                    continue
            objs = {r: own[client][r] for r in cands if r in own[client]}
            rest = sorted(cands - set(objs))
            if rest:
                objs.update(_build_candidates(ws, root, graph_of(root), client, rest, cache, jobs, tag))
            maps = [X.annotated_defs(o, by_rel[r], undecorate) for r, o in sorted(objs.items()) if o is not None]
            m = {k: v for k, v in X.merge(maps).items() if k in rs}
            if m:
                out[side, client] = m
    return out


def run_gate(repo, base="HEAD", files=None, renames=None, jobs=2, log=print, cache=None, protos=None) -> GateResult:
    """`protos`: extra prototype journal entries ({client, va, old, new}), e.g. from --protos files."""
    repo = Path(repo).resolve()
    ws = Workspace(repo)
    sha = git(repo, "rev-parse", base).strip()
    files = sorted(files) if files else changed_files(repo, sha)
    res = GateResult(sha, files)
    cache = Path(cache or ws.build / "readability").resolve()     # compiles run in their output folder
    before_root = base_tree(repo, sha, cache / "base")
    graphs = {}

    def graph_of(root):
        if root not in graphs:
            graphs[root] = include_graph(root)
        return graphs[root]
    tus = {f for f in files if f.startswith("src/") and f.lower().endswith((".c", ".cpp"))}
    hdrs = [f for f in files if f.lower().endswith(".h")]
    if hdrs:
        tus |= set(dependents(repo, hdrs, graph_of(repo))) | \
            set(dependents(before_root, [h for h in hdrs if (before_root / h).is_file()], graph_of(before_root)))
    renames = dict(renames or {})
    for t in sorted(tus):
        client = clients.client_of_path(t)
        if not (repo / t).is_file():
            res.problems.append(f"{t}: deleted (its functions are gone)")
            continue
        if not (before_root / t).is_file():
            res.problems.append(f"{t}: new file (Level 1 adds no code)")
            continue
        renames.update(_renames_from_anns(_ann_map(before_root / t, client), _ann_map(repo / t, client)))
    if res.problems:
        return res
    # one match header for both trees
    fb, db, _ = cmp.match_symbols(before_root)
    fa, da, _ = cmp.match_symbols(repo)
    hdr_dir = cache / "hdr"
    hdr_dir.mkdir(parents=True, exist_ok=True)
    tmp = hdr_dir / "tmp.h"
    cmp.write_match_header(tmp, fb | fa, db | da)
    htext = tmp.read_text(encoding="utf-8")
    hsha = hashlib.sha1(htext.encode()).hexdigest()[:12]
    header = hdr_dir / f"eqd_match_syms_{hsha}.h"
    if not header.is_file():
        header.write_text(htext, encoding="utf-8")
    hb, ha = _own_header(hdr_dir, fb, db), _own_header(hdr_dir, fa, da)     # each tree's own header
    work = cache / "work" / hashlib.sha1(str(repo).encode()).hexdigest()[:10]
    bcache = cache / "before" / sha[:16] / hsha

    def build(rel):
        client = clients.client_of_path(rel)
        tag = re.sub(r"[^A-Za-z0-9]+", "_", rel)
        btxt = (before_root / rel).read_bytes()
        bkey = hashlib.sha1(btxt + client.encode()).hexdigest()[:12]
        bdir = bcache / f"{tag}_{bkey}"
        builds = {}
        for side, root, out in (("before", before_root, bdir), ("after", repo, work / "after")):
            for mode, h in (("match", header), ("normal", None)):
                done = out / f"{mode}.done"
                if side == "before" and done.is_file():
                    meta = json.loads(done.read_text(encoding="utf-8"))
                    builds[side, mode] = (Path(meta["obj"]) if meta["obj"] else None, meta["text"])
                    continue
                o, text = compile_tu(ws, root, rel, client, out / mode, h)
                builds[side, mode] = (o, text)
                if side == "before":
                    done.write_text(json.dumps({"obj": str(o) if o else None, "text": text}), encoding="utf-8")
        # source_checks need no journal: done here, where they overlap with the other TUs' compiles
        return rel, builds, bdir, check_counts(before_root / rel, repo / rel, rel, client, renames)

    protos_by = collections.defaultdict(dict)       # client -> {old decorated: new decorated}

    def judge(item):
        rel, builds, bdir, checks = item
        client = clients.client_of_path(rel)
        r = TUResult(rel, client)
        jp = protos_by.get(client) or None
        xb, xa = xtu_maps.get(("before", client)), xtu_maps.get(("after", client))
        xnotes = []
        for mode in ("match", "normal"):
            ob, tb = builds["before", mode]
            oa, ta = builds["after", mode]
            if oa is None and ob is not None:
                err = "; ".join(l.strip() for l in ta.splitlines() if "error" in l.lower())[:400]
                r.errors.append(f"{mode} build fails: {err}")
            w_b, w_a = len(WARN_4005.findall(tb)), len(WARN_4005.findall(ta))
            if w_a > w_b:
                new = [l.strip() for l in ta.splitlines() if WARN_4005.search(l)]
                r.errors.append(f"{mode} build: C4005 macro redefinition {w_b} -> {w_a}: {new[:3]}")
        ob, oa = builds["before", "match"][0], builds["after", "match"][0]
        if ob is None:
            r.errors.append("match build fails on the base too: nothing to compare")
        if ob is not None and oa is not None:
            r.changed, r.functions = diff_objects(ob, oa, before_root / rel, repo / rel, client, renames, protos=jp,
                                                  xtu_b=xb, xtu_a=xa, notes=xnotes)
            r.notes += xnotes
            if r.changed and not r.errors:
                # fails only under the shared (union) header? Recheck with each tree's own generated header.
                if (hb, ha) != (header, header):
                    obo = _build_before_own(ws, before_root, rel, client, bdir, hb)
                    oao, _t = compile_tu(ws, repo, rel, client, work / "after" / "match_own", ha)
                    if obo is not None and oao is not None:
                        again, _n = diff_objects(obo, oao, before_root / rel, repo / rel, client, renames, protos=jp,
                                                 xtu_b=xb, xtu_a=xa)
                        if not again:
                            r.notes.append(f"header-sensitive, own-header PASS ({len(r.changed)} function(s) differ "
                                           f"only under the shared header: {', '.join(k for k, _, _ in r.changed[:8])})")
                            r.changed = []
        r.checks = checks
        return r

    with cf.ThreadPoolExecutor(max_workers=max(1, jobs)) as ex:
        built = list(ex.map(build, sorted(tus)))
    # prototype journal: --protos entries plus what the before/after objects show
    file_names = collections.defaultdict(set)
    entries = [dict(j, tu="--protos", kind=j.get("kind", "file")) for j in (protos or [])]
    for rel, builds, _bdir, _checks in built:
        ob, oa = builds["before", "match"][0], builds["after", "match"][0]
        if ob is not None and oa is not None:
            entries += tu_journal(ob, oa, before_root / rel, repo / rel, clients.client_of_path(rel), rel, renames)
    best = {}
    for j in entries:
        c = j.get("client") or "trilogy"
        j = dict(j, client=c)
        if j["tu"] == "--protos":
            file_names[c] |= {j["old"], j["new"]}
        prev = protos_by[c].get(j["old"])
        if prev is not None and prev != j["new"]:
            res.link.append(f"{c}: journal conflict: {j['old']} -> {prev} and -> {j['new']} ({j['tu']})")
            continue
        protos_by[c][j["old"]] = j["new"]
        k = (c, j["old"], j["new"])                  # one line per pair; a definition entry wins over references
        if k not in best or (j["kind"] == "def" and best[k]["kind"] != "def"):
            best[k] = j
    res.protos = sorted(best.values(), key=lambda j: (j["client"], j["old"]))
    xtu_maps = cross_tu_maps(ws, built, before_root, repo, graph_of, cache / "link", jobs)
    with cf.ThreadPoolExecutor(max_workers=max(1, jobs)) as ex:
        res.tus = list(ex.map(judge, built))
    # link check per client (only when something is journaled or a definition was added/removed)
    by_client = collections.defaultdict(lambda: ({}, {}))
    for rel, builds, _bdir, _checks in built:
        ob, oa = builds["before", "normal"][0], builds["after", "normal"][0]
        if ob is not None and oa is not None:
            a, b = by_client[clients.client_of_path(rel)]
            a[rel], b[rel] = oa, ob
    for client, (a, b) in sorted(by_client.items()):
        fails, notes = link_check(ws, repo, client, a, b, protos_by.get(client, {}), file_names.get(client, set()),
                                  cache / "link", jobs, lambda: graph_of(repo), before_root,
                                  lambda: graph_of(before_root))
        res.link += fails
        res.link_notes += notes
    old_lint = set(lint_fixtures(before_root, tus, files))
    res.lint = [l for l in lint_fixtures(repo, tus, files) if l not in old_lint]
    res.known_lint = sorted(old_lint)
    return res


def load_protos(path) -> list:
    """Prototype journal lines ({"client", "va", "old", "new"}, decorated names; `kind` "proto" or absent) of a
    jsonl file, e.g. a previous run's --journal-out."""
    out = []
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        if line.strip():
            j = json.loads(line)
            if j.get("kind", "proto") in ("proto", "def", "ref", "file") and "old" in j and "new" in j:
                out.append({"client": j.get("client") or "trilogy", "va": j.get("va"), "old": j["old"],
                            "new": j["new"]})
    return out


def load_journal(path) -> dict:
    out = {}
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        if line.strip():
            j = json.loads(line)
            if j.get("kind") in ("fn", "global", "macro"):
                out[R.simple(j["old"])] = R.simple(j["new"])
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="changed files (default: git diff against --base + untracked)")
    ap.add_argument("--repo", default=".")
    ap.add_argument("--base", default="HEAD", help="the commit to compare with (default HEAD; by hand: main)")
    ap.add_argument("--renames", action="append", default=[], help="rename.py --journal file (repeatable)")
    ap.add_argument("-j", "--jobs", type=int, default=2, help="parallel TUs (default 2)")
    ap.add_argument("--json", help="write the result here")
    ap.add_argument("--cache", help="cache folder (default <build>/readability)")
    ap.add_argument("--protos", action="append", default=[],
                    help="prototype journal jsonl ({client, va, old, new} decorated names; repeatable)")
    ap.add_argument("--journal-out", help="write the prototype journal (what the objects show) here, jsonl")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    renames = {}
    for j in a.renames:
        renames.update(load_journal(j))
    files = [Path(f).resolve().relative_to(repo).as_posix() if Path(f).is_absolute() or Path(f).exists() else f
             for f in a.files]
    try:
        res = run_gate(repo, a.base, files or None, renames, a.jobs, cache=a.cache,
                       protos=[j for f in a.protos for j in load_protos(f)])
    except (RuntimeError, FileNotFoundError) as e:
        print(f"gate: {e}")
        return 2
    print(res.report())
    if a.json:
        Path(a.json).write_text(json.dumps(res.as_json(), indent=1), encoding="utf-8")
    if a.journal_out:
        Path(a.journal_out).write_text("".join(json.dumps(dict(j, kind="proto", how=j["kind"])) + "\n"
                                               for j in res.protos), encoding="utf-8")
    return 0 if res.ok else 1


if __name__ == "__main__":
    sys.exit(main())
