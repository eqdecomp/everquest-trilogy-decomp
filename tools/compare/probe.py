r"""Compile a small C/C++ snippet with VC6 (SP4 by default) and print the code of one function: the quickest way
to ask "what does VC6 make of this shape?" and "does A vs B change the code?".

```
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c -f f                 listing of f
..\.venv\Scripts\python.exe tools\compare\probe.py a.c b.c -f f                same / differs + unified diff
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c -f f -V A -V B       one file, built with /DA and /DB
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c --flags "/O2"        extra options (an /O level replaces /O1)
..\.venv\Scripts\python.exe tools\compare\probe.py --check-quirks              rerun docs/vc6_quirks/*.c[pp]
```
Flags: `eqd_unicorn.vc6.DEFAULT_CFLAGS` (the match-mode defaults, `/O1 /W3 /MT /Gy /GX`) plus the snippet's own
`// CFLAGS:` lines plus `--flags`, as `compare.py --match` builds a source. `.c` compiles as C, `.cpp` as C++.

Listing: the function's COMDAT section (`/Gy`) disassembled linearly; relocated fields show the symbol
(`call _g`, `mov eax, dword ptr [_x]`, `fs:[__except_list]`), branches inside the function `@<index>` (the
target's instruction index, as compare.py), and switch tables (relocated dwords where an instruction would
start) `dd @<index>`; whatever follows the first table is shown as `db`.

Quirk files (`docs/vc6_quirks/`): a snippet with `//` directive lines that state its claim as checks.
  // QUIRK: <id>                 stable id (the VC6_QUIRKS.md entry)
  // VARIANTS: A B               build once per name with /D<name> (default: one plain build)
  // KEYWORDS: switch, jump table  what the entry is about (for selecting entries by diff summary)
  // FLAGS: /O2                  extra options for every build of this file (FLAGS@A: only variant A)
  // EXPECT: f has REGEX         some part of f's listing matches (re.M; `(?s)` to span lines)
  // EXPECT: f lacks REGEX
  // EXPECT: f count REGEX == N  (also != < <= > >=)
  // EXPECT: f differs A B       the two variants' listings of f differ (same: identical)
  // EXPECT@A: f has REGEX       only for variant A
"""
from __future__ import annotations

import argparse
import difflib
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "unicorn"))
sys.path.insert(0, str(HERE))

import capstone  # noqa: E402

import coff  # noqa: E402
from eqd_unicorn.vc6 import DEFAULT_CFLAGS, DEFAULT_VC6, OPT_LEVELS, cflags_for, vc6_env  # noqa: E402

QUIRKS_DIR = HERE.parent / "docs" / "vc6_quirks"
_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True
_SENTINEL = 0x5A5A0000


class ProbeError(RuntimeError):
    pass


def find_vc6(vc6=None) -> Path:
    if vc6:
        return Path(vc6)
    if os.environ.get("EQD_VC6"):
        return Path(os.environ["EQD_VC6"])
    try:
        from eqd_unicorn import Workspace
        return Workspace(HERE).vc6()
    except Exception:  # noqa: BLE001 - no workspace: fall back to the default install
        return Path(DEFAULT_VC6)


def merge_flags(base: list, extra) -> list:
    extra = list(extra or [])
    if any(t in OPT_LEVELS for t in extra):
        base = [t for t in base if t not in OPT_LEVELS]
    return base + extra


def compile_snippet(src: Path, out_dir: Path, flags=(), defines=(), vc6=None, tag="") -> Path:
    """-> the .obj of `src` built with DEFAULT_CFLAGS + its CFLAGS lines + `flags` (+ /D`defines`)."""
    vc6 = find_vc6(vc6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        raise ProbeError(f"VC6 not found at {vc6}")
    src = Path(src).resolve()
    out_dir = Path(out_dir).resolve()
    out_dir.mkdir(parents=True, exist_ok=True)
    obj = out_dir / f"{src.stem}{'_' + tag if tag else ''}.obj"
    cmd = ([str(vc6 / "Bin" / "CL.EXE")] + merge_flags(cflags_for(src, DEFAULT_CFLAGS), flags)
           + [f"/D{d}" for d in defines] + [f"/Fo{obj}", str(src)])
    p = subprocess.run(cmd, env=vc6_env(vc6), cwd=out_dir, capture_output=True, text=True, errors="replace")
    if p.returncode != 0:
        raise ProbeError(f"cl failed for {src.name} {' '.join(defines)}:\n{p.stdout}{p.stderr}")
    return obj


def undecorated(name: str) -> str:
    """`_f` / `_f@8` / `@f@8` -> `f`; `?f@@YAHH@Z` -> `f`; `?m@C@@QAEHXZ` -> `C::m`."""
    if name[:3] in ("??0", "??1"):                     # constructor / destructor
        parts = name[3:].split("@@", 1)[0].split("@")
        cls = parts[0]
        return "::".join(list(reversed(parts)) + [("~" if name[2] == "1" else "") + cls])
    if name.startswith("?"):
        parts = name[1:].split("@@", 1)[0].split("@")
        return "::".join(reversed(parts))
    return re.sub(r"@\d+$", "", name.lstrip("_@"))


@dataclass
class Listing:
    name: str           # the object's symbol
    lines: list         # "mnemonic operands" (normalised: @index labels, symbol names)
    offsets: list       # section offset of each line

    def text(self) -> str:
        return "\n".join(self.lines)

    def show(self) -> str:
        return "\n".join(f"  {o:04x}  {ln}" for o, ln in zip(self.offsets, self.lines))


def functions(obj: coff.Obj) -> dict:
    """symbol name -> (section index, value) for every function defined in the object."""
    out = {}
    for s in obj.symbols:
        if s and s.section > 0 and s.type & 0x20 and s.storage in (2, 3):
            out[s.name] = (s.section, s.value)
    return out


def pick(obj: coff.Obj, func: str) -> str:
    fns = functions(obj)
    if func in fns:
        return func
    hits = [n for n in fns if undecorated(n) == func]
    if len(hits) != 1:
        raise ProbeError(f"function {func!r}: {'ambiguous ' + str(hits) if hits else 'not found'} "
                         f"(defined: {', '.join(sorted(undecorated(n) for n in fns))})")
    return hits[0]


def listing(obj: coff.Obj, func: str) -> Listing:
    name = pick(obj, func)
    sec_no, start = functions(obj)[name]
    sec = obj.sections[sec_no - 1]
    data = bytearray(sec.data)
    # Relocated fields: replace each with a sentinel the disassembly prints, then put the symbol back.
    subst = {}
    rel_at = {}
    for k, (off, si, typ) in enumerate(sec.relocs):
        sym = obj.symbols[si]
        addend = struct.unpack_from("<i", data, off)[0]
        nm = sym.name
        if sym.section > 0 and sym.section == sec_no and typ == coff.REL_DIR32:
            nm = f"@L{sym.value + addend:x}"          # own code (switch table entry / table address)
            addend = 0
        elif sym.name.startswith((".", "$")) and sym.section > 0:
            nm = f"{obj.sections[sym.section - 1].name}"
        label = nm + (f"+0x{addend:x}" if addend > 0 else f"-0x{-addend:x}" if addend < 0 else "")
        val = _SENTINEL + 0x100 * (k + 1)
        subst[val] = label
        rel_at[off] = (val, typ, label)
        if typ == coff.REL_REL32:
            struct.pack_into("<I", data, off, (val - (off + 4)) & 0xFFFFFFFF)
        elif typ == coff.REL_DIR32:
            struct.pack_into("<I", data, off, val)
    raw = []                                            # (offset, text, branch target or None)
    off = start
    tables = False
    while off < len(data):
        if off in rel_at and rel_at[off][1] == coff.REL_DIR32 or tables:
            tables = True
            if off in rel_at:
                raw.append((off, f"dd {rel_at[off][2]}", None))
                off += 4
            else:
                end = min((o for o in rel_at if o > off), default=len(data))
                raw.append((off, "db " + " ".join(f"{b:02x}" for b in data[off:end]), None))
                off = end
            continue
        insn = next(_MD.disasm(bytes(data[off:off + 15]), off, 1), None)
        if insn is None:
            raw.append((off, f"db {data[off]:02x}", None))
            off += 1
            continue
        txt = f"{insn.mnemonic} {insn.op_str}".strip()
        tgt = None
        if insn.group(capstone.CS_GRP_JUMP) or insn.mnemonic == "call":
            ops = insn.operands
            if len(ops) == 1 and ops[0].type == capstone.CS_OP_IMM and 0 <= ops[0].imm < len(data) \
                    and not any(insn.address <= o < insn.address + insn.size for o in rel_at):
                tgt = ops[0].imm
        for val, label in subst.items():
            txt = re.sub(rf"0x{val:x}\b", label, txt)
        raw.append((off, txt, tgt))
        off += insn.size
    while raw and (raw[-1][1] in ("nop", "int3") or
                   raw[-1][1].startswith("db ") and set(raw[-1][1][3:].split()) <= {"90", "cc"}):
        raw.pop()                                       # section padding after the last ret / table
    if raw and raw[-1][1].startswith("db "):            # trailing padding inside the last index table
        raw[-1] = (raw[-1][0], re.sub(r"( (90|cc))+$", "", raw[-1][1]), None)
    index = {o: i for i, (o, _, _) in enumerate(raw)}
    lines = []
    for o, txt, tgt in raw:
        if tgt is not None:
            txt = f"{txt.split()[0]} @{index.get(tgt, f'?{tgt:x}')}"
        txt = re.sub(r"@L([0-9a-f]+)", lambda m: f"@{index.get(int(m.group(1), 16), '?' + m.group(1))}", txt)
        lines.append(txt)
    return Listing(name, lines, [o for o, _, _ in raw])


def probe(src, func, out_dir, flags=(), defines=(), vc6=None, tag="") -> Listing:
    obj = compile_snippet(Path(src), Path(out_dir), flags, defines, vc6, tag)
    return listing(coff.parse(obj), func)


def diff(a: Listing, b: Listing, la="A", lb="B") -> str:
    return "\n".join(difflib.unified_diff(a.lines, b.lines, la, lb, lineterm="", n=2))


# --- quirk files ----------------------------------------------------------------------------------------------

_DIRECTIVE = re.compile(r"^\s*//\s*(QUIRK|KEYWORDS|VARIANTS|FLAGS|EXPECT)(?:@(\w+))?\s*:\s*(.*?)\s*$")
_CMP = {"==": lambda a, b: a == b, "!=": lambda a, b: a != b, "<": lambda a, b: a < b,
        "<=": lambda a, b: a <= b, ">": lambda a, b: a > b, ">=": lambda a, b: a >= b}


@dataclass
class Quirk:
    path: Path
    id: str
    variants: list
    flags: list
    keywords: list
    var_flags: dict     # variant -> extra flags
    expects: list       # (variant or None, func, op, arg)


def parse_quirk(path: Path) -> Quirk:
    q = Quirk(Path(path), Path(path).stem, [], [], [], {}, [])
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        m = _DIRECTIVE.match(line)
        if not m:
            continue
        kind, var, rest = m.group(1), m.group(2), m.group(3)
        if kind == "QUIRK":
            q.id = rest
        elif kind == "VARIANTS":
            q.variants = rest.split()
        elif kind == "KEYWORDS":
            q.keywords = [k.strip() for k in rest.split(",") if k.strip()]
        elif kind == "FLAGS" and var:
            q.var_flags.setdefault(var, []).extend(rest.split())
        elif kind == "FLAGS":
            q.flags += rest.split()
        else:
            func, op, arg = (rest.split(None, 2) + ["", ""])[:3]
            if op not in ("has", "lacks", "count", "differs", "same"):
                raise ProbeError(f"{path.name}: bad EXPECT {rest!r}")
            q.expects.append((var, func, op, arg))
    if not q.expects:
        raise ProbeError(f"{path.name}: no EXPECT lines")
    return q


def check_quirk(q: Quirk, out_dir: Path, vc6=None, verbose=False) -> list[str]:
    """-> failure messages (empty = the claim holds)."""
    builds = q.variants or [None]
    cache: dict = {}

    def get(var, func):
        if (var, func) not in cache:
            flags = q.flags + q.var_flags.get(var, [])
            cache[(var, func)] = probe(q.path, func, out_dir, flags, [var] if var else [], vc6, var or "")
        return cache[(var, func)]

    fails = []
    for var, func, op, arg in q.expects:
        if op in ("differs", "same"):
            a, b = arg.split()
            la, lb = get(a, func), get(b, func)
            if (la.lines != lb.lines) != (op == "differs"):
                fails.append(f"{func} {op} {a} {b}: false\n{diff(la, lb, a, b) or '(identical)'}")
            continue
        for v in ([var] if var else builds):
            ls = get(v, func)
            where = f"{func}{'@' + v if v else ''}"
            if op == "count":
                m = re.match(r"(.*?)\s*(==|!=|<=|>=|<|>)\s*(\d+)$", arg)
                if not m:
                    raise ProbeError(f"{q.path.name}: bad count {arg!r}")
                n = len(re.findall(m.group(1), ls.text(), re.M))
                ok = _CMP[m.group(2)](n, int(m.group(3)))
                msg = f"{where} count /{m.group(1)}/ {m.group(2)} {m.group(3)}: got {n}"
            else:
                hit = re.search(arg, ls.text(), re.M) is not None
                ok = hit == (op == "has")
                msg = f"{where} {op} /{arg}/"
            if not ok:
                fails.append(msg + "\n" + ls.show())
    if verbose:
        for (v, f), ls in cache.items():
            print(f"--- {q.id} {f}{'@' + v if v else ''}\n{ls.show()}")
    return fails


def quirk_files(d: Path = QUIRKS_DIR) -> list[Path]:
    return sorted(p for p in Path(d).iterdir() if p.suffix in (".c", ".cpp"))


def check_quirks(d: Path, out_dir: Path, vc6=None, verbose=False, log=print) -> int:
    bad = 0
    for p in quirk_files(d):
        q = parse_quirk(p)
        try:
            fails = check_quirk(q, out_dir / p.stem, vc6, verbose)
        except ProbeError as e:
            fails = [str(e)]
        bad += bool(fails)
        log(f"{'PASS' if not fails else 'FAIL'}  {q.id}  ({p.name})")
        for f in fails:
            log("    " + f.replace("\n", "\n    "))
    log(f"{len(quirk_files(d)) - bad} hold, {bad} fail")
    return 1 if bad else 0


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("sources", nargs="*", type=Path)
    ap.add_argument("-f", "--func", action="append", default=[], help="function (undecorated name or symbol)")
    ap.add_argument("--flags", default="", help="extra cl options (an /O level replaces /O1)")
    ap.add_argument("-D", dest="defines", action="append", default=[], help="define for every build")
    ap.add_argument("-V", "--variant", action="append", default=[], help="build once per variant with /D<name>")
    ap.add_argument("--vc6", help="VC98 folder (default: EQD_VC6, paths.local.ini vc6 = SP4)")
    ap.add_argument("--out", type=Path, help="keep objects here (default: a temporary folder)")
    ap.add_argument("--check-quirks", nargs="?", const=QUIRKS_DIR, type=Path, metavar="DIR")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args(argv)
    tmp = None
    out = a.out
    if out is None:
        tmp = tempfile.mkdtemp(prefix="eqd_probe_")
        out = Path(tmp)
    try:
        if a.check_quirks:
            return check_quirks(a.check_quirks, out, a.vc6, a.verbose)
        if not a.sources:
            ap.error("give a source (or --check-quirks)")
        flags = a.flags.split()
        builds = []                                     # (label, source, defines)
        for s in a.sources:
            for v in a.variant or [None]:
                builds.append((f"{s.name}{'@' + v if v else ''}", s, a.defines + ([v] if v else [])))
        if len(builds) > 2:
            ap.error("at most two builds (two sources, or one source with two variants)")
        objs = [(lab, coff.parse(compile_snippet(s, out, flags, d, a.vc6, f"b{k}")))
                for k, (lab, s, d) in enumerate(builds)]
        funcs = a.func or sorted(undecorated(n) for n in functions(objs[0][1]))
        rc = 0
        for f in funcs:
            ls = [(lab, listing(o, f)) for lab, o in objs]
            if len(ls) == 1:
                print(f"{f} ({ls[0][1].name}, {len(ls[0][1].lines)} lines)\n{ls[0][1].show()}")
                continue
            (la, x), (lb, y) = ls
            same = x.lines == y.lines
            rc |= 0 if same else 1
            print(f"{f}: {'same' if same else 'differs'} ({la}: {len(x.lines)} lines, {lb}: {len(y.lines)})")
            if not same or a.verbose:
                print(diff(x, y, la, lb) if not same else x.show())
        return rc
    except ProbeError as e:
        print(f"probe: {e}", file=sys.stderr)
        return 2
    finally:
        if tmp:
            shutil.rmtree(tmp, ignore_errors=True)


if __name__ == "__main__":
    sys.exit(main())
