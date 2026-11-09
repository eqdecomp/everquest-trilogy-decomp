r"""Stack-frame hints for near misses, from reccmp (isledecomp/reccmp, the `reccmp-stackcmp` analysis).

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\compare\stackcmp.py --nearmiss ..\build\match\nearmiss.txt ^
        --json ..\build\match\stackcmp.json
    ..\.venv\Scripts\python.exe tools\compare\stackcmp.py --nearmiss ..\build\match\nearmiss.txt --va 0x004F4FA0 -v
    ..\.venv\Scripts\python.exe tools\compare\stackcmp.py --client eqmain --nearmiss ..\build\match\eqmain-nearmiss.txt

1. **Build.** The near-miss sources (the `source` column of `compare.py --match --nearmiss`) are compiled as in
   match mode (the client's toolchain or the source's own `// TOOLCHAIN:`, `cflags_for`: defaults + the image's
   and the source's `// CFLAGS:`, `/DEQD_MATCH`, the generated `eqd_match_syms.h`) plus `/Zi`, and linked into
   one DLL with `/DEBUG /PDBTYPE:CON` (a self-contained PDB 2.0), at 0x10000000, or 0x30000000 when the
   original image sits there itself (eqmain.dll). A generated stub file defines every `eqd_fn_<va>` / `eqd_va_<va>` symbol. Each source is also
   compiled without `/Zi`; `zi_neutral` says whether the code sections came out identical (they should).
2. **Map.** reccmp gets our sources through `reccmp_map.code_files` (marker blocks moved next to the
   definition, line numbers kept), plus a generated CSV: original size per near miss (from the Ghidra
   export) and `eqd_fn_` / `eqd_va_` / CRT symbols at their client addresses, so calls and globals line up.
3. **Compare.** `reccmp.compare.Compare` diffs each near miss; on the aligned instruction pairs the
   stack operands (`[ebp +- n]`, `[esp +- n]`, also with an index register) are paired up as reccmp-stackcmp
   does, the rewrite's `ebp` slots named from the PDB (`S_BPREL32`).
4. **Hints** go to JSON (`--json`, format in README.md "stackcmp").

Needs reccmp in the venv, VC6 and the workspace (original exe, Ghidra export for sizes).
"""
from __future__ import annotations

import argparse
import csv
import io
import json
import logging
import re
import struct
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "unicorn"))
sys.path.insert(0, str(HERE))

import coff  # noqa: E402
import reccmp_map  # noqa: E402
from eqd_unicorn import Workspace  # noqa: E402
from eqd_unicorn.vc6 import (FLTUSED_SRC, LINK_BASE, LINK_EXTRA, SYSTEM_LIBS, cflags_for, source_vc6,  # noqa: E402
                             vc6_env)
from eqd_unicorn import clients  # noqa: E402

ALT_LINK_BASE = 0x30000000      # the DLL's base when the original image itself is at LINK_BASE (eqmain.dll)

SCHEMA = 1


# --- near-miss list -------------------------------------------------------------------------------------------

def read_nearmiss(path) -> list[dict]:
    """Rows of `compare.py --match --nearmiss` (TSV, `#` header): va, ndiff, name, source, flags, first."""
    out = []
    for line in Path(path).read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        cols = line.split("\t")
        out.append({"va": int(cols[0], 16), "ndiff": int(cols[1]), "name": cols[2], "source": cols[3],
                    "flags": cols[4] if len(cols) > 4 else "", "first": cols[5] if len(cols) > 5 else ""})
    return out


# --- stack operands -------------------------------------------------------------------------------------------

_MEM = re.compile(r"\[([^\]]*)\]")


@dataclass(frozen=True)
class StackRef:
    reg: str            # "ebp" | "esp"
    off: int            # signed displacement
    index: str = ""     # index term, e.g. "eax*4" (array access into a stack slot)

    def __str__(self):
        s = f"{self.reg}{'+' if self.off >= 0 else '-'}{abs(self.off):#x}"
        return s + (f" [+{self.index}]" if self.index else "")

    @property
    def slot(self) -> str:
        return f"{self.reg}{'+' if self.off >= 0 else '-'}{abs(self.off):#x}"


def stack_ref(insn: str) -> StackRef | None:
    """The ebp/esp-based memory operand of a disassembled instruction (reccmp/capstone syntax), or None.
    `[ebp + eax*4 - 0x10]` -> ebp-0x10 [+eax*4] (reccmp-stackcmp's own regex reads that as ebp+0xea)."""
    for m in _MEM.finditer(insn):
        terms = re.findall(r"([+-]?)\s*([^\s+-]+)", m.group(1))
        base, off, index = None, 0, []
        for sign, t in terms:
            if t in ("ebp", "esp") and base is None and sign != "-":
                base = t
            elif re.fullmatch(r"(0x[0-9a-fA-F]+|\d+)", t):
                off += -int(t, 0) if sign == "-" else int(t, 0)
            else:
                index.append(t)
        if base:
            return StackRef(base, off, "+".join(index))
    return None


def _frame_size(insns) -> int | None:
    """N of the first `sub esp, N` in the prologue (first 8 instructions)."""
    for _, text in insns[:8]:
        m = re.fullmatch(r"sub esp, (0x[0-9a-f]+|\d+)", text.strip())
        if m:
            return int(m.group(1), 0)
    return None


@dataclass
class FunctionStack:
    pairs: list = field(default_factory=list)           # (orig addr, orig insn, recomp addr, recomp insn, o, r)
    operand_mismatch: list = field(default_factory=list)
    structural: list = field(default_factory=list)


def walk_diff(rdiff) -> FunctionStack:
    """Pair the stack operands of reccmp's aligned instructions (equal blocks and equal-length replace blocks),
    as reccmp-stackcmp does; unequal blocks are recorded as structural."""
    fs = FunctionStack()
    oi, ri = rdiff.orig_inst, rdiff.recomp_inst
    for tag, i1, i2, j1, j2 in rdiff.codes:
        if tag != "equal" and (i2 - i1) != (j2 - j1):
            fs.structural.append({"orig_addr": oi[i1][0] if i1 < i2 else None,
                                  "rewrite_addr": ri[j1][0] if j1 < j2 else None,
                                  "orig": [t for _, t in oi[i1:i2]][:12], "rewrite": [t for _, t in ri[j1:j2]][:12]})
            continue
        for (oa, ot), (ra, rt) in zip(oi[i1:i2], ri[j1:j2]):
            o, r = stack_ref(ot), stack_ref(rt)
            if o and r:
                fs.pairs.append((oa, ot, ra, rt, o, r))
            elif o or r:
                fs.operand_mismatch.append({"orig_addr": oa, "orig": ot, "rewrite_addr": ra, "rewrite": rt})
    return fs


def type_desc(types, key) -> str:
    """Readable name of a CodeView type key: T_INT4, Foo, T_UCHAR[16], else its hex key."""
    try:
        from reccmp.cvdump.types import get_primitive
        if key.is_scalar():
            return get_primitive(key).name
        ti = types.get(key) if types is not None else None
        if ti is not None and ti.is_array():
            return f"{type_desc(types, ti.array_type)}[{ti.array_length}]"
        if ti is not None and ti.name:
            return ti.name
    except Exception:  # noqa: BLE001
        pass
    try:
        return f"0x{int(key):x}"
    except (TypeError, ValueError):
        return str(key)


def bprel_symbols(node, types=None) -> dict:
    """{ebp offset: (name, type)} from the S_BPREL32 records of a reccmp cvdump node."""
    out = {}
    entry = getattr(node, "symbol_entry", None) if node is not None else None
    for s in (entry.symbols if entry else []):
        if s.symbol_type == "S_BPREL32":
            off = struct.unpack(">l", bytes.fromhex(s.location[1:-1]))[0]
            out[off] = (s.name, type_desc(types, s.data_type))
    return out


def analyse(rdiff, syms: dict) -> dict:
    """Hints for one function from reccmp's raw diff and the rewrite's ebp symbols."""
    fs = walk_diff(rdiff)
    res: dict = {"frame": {"orig": _frame_size(rdiff.orig_inst), "rewrite": _frame_size(rdiff.recomp_inst)}}
    hints = []
    fo, fr = res["frame"]["orig"], res["frame"]["rewrite"]
    if fo != fr:
        hints.append(f"frame size differs: original sub esp, {fo:#x}, rewrite {fr:#x}" if fo is not None and
                     fr is not None else f"frame size differs: original {fo}, rewrite {fr}")
    # ebp: a variable mapping (offsets don't move within the function)
    o2r: dict = {}
    r2o: dict = {}
    for _, _, _, _, o, r in fs.pairs:
        if o.reg == r.reg == "ebp":
            o2r.setdefault(o.off, set()).add(r.off)
            r2o.setdefault(r.off, set()).add(o.off)
    ebp = []
    for ooff in sorted(o2r):
        roffs = sorted(o2r[ooff])
        conflict = len(roffs) > 1 or any(len(r2o[x]) > 1 for x in roffs)
        for roff in roffs:
            name, typ = syms.get(roff, (None, None))
            kind = "conflict" if conflict else ("same" if roff == ooff else "moved")
            ebp.append({"orig": ooff, "rewrite": roff, "kind": kind, "symbol": name, "type": typ})
            o_s, r_s = StackRef("ebp", ooff).slot, StackRef("ebp", roff).slot
            nm = f" (`{name}`)" if name else ""
            if kind == "moved":
                hints.append(f"frame differs at {o_s}: the original's {o_s} is the rewrite's {r_s}{nm}")
            elif kind == "conflict" and roff == roffs[0]:
                hints.append(f"frame differs at {o_s}: maps to several rewrite slots or shares one "
                             f"({', '.join(StackRef('ebp', x).slot for x in roffs)}){nm}; variables split/merged")
    res["ebp"] = ebp
    seen = {e["rewrite"] for e in ebp}
    res["ebp_unseen"] = [{"rewrite": off, "symbol": n, "type": t} for off, (n, t) in sorted(syms.items())
                         if off not in seen and off < 0]
    # esp: offsets move with pushes; report per instruction and as a histogram of deltas
    esp, deltas = [], {}
    for oa, ot, ra, rt, o, r in fs.pairs:
        if (o.reg, o.off) != (r.reg, r.off):
            if o.reg == r.reg == "esp":
                d = r.off - o.off
                deltas[d] = deltas.get(d, 0) + 1
            if not (o.reg == r.reg == "ebp"):
                esp.append({"orig_addr": oa, "orig": ot, "rewrite_addr": ra, "rewrite": rt,
                            "orig_slot": o.slot, "rewrite_slot": r.slot})
    res["esp"] = esp
    res["esp_deltas"] = {f"{d:+#x}": n for d, n in sorted(deltas.items())}
    for d, n in sorted(deltas.items(), key=lambda kv: -kv[1]):
        ex = next(e for e in esp if e["orig_slot"].startswith("esp") and e["rewrite_slot"].startswith("esp") and
                  stack_ref(e["rewrite"]).off - stack_ref(e["orig"]).off == d)
        hints.append(f"esp operands off by {d:+#x} in {n} place(s), first at original {ex['orig_addr']}: "
                     f"{ex['orig_slot']} vs rewrite {ex['rewrite_slot']}")
    for e in esp:
        if not e["orig_slot"].startswith("esp") or not e["rewrite_slot"].startswith("esp"):
            hints.append(f"frame base differs at original {e['orig_addr']}: {e['orig_slot']} vs {e['rewrite_slot']}")
    res["operand_mismatch"] = fs.operand_mismatch
    for e in fs.operand_mismatch[:3]:
        o = stack_ref(e["orig"])
        side = "original" if o else "rewrite"
        r = stack_ref(e["rewrite"])
        nm = syms.get(r.off, (None,))[0] if r is not None and r.reg == "ebp" else None
        hints.append(f"stack operand only in the {side} at original {e['orig_addr']}: "
                     f"`{e['orig']}` vs `{e['rewrite']}`" + (f" (`{nm}`)" if nm else ""))
    res["structural"] = fs.structural
    if fs.structural:
        s0 = fs.structural[0]
        at = f"original {s0['orig_addr']}" if s0["orig_addr"] else f"rewrite {s0['rewrite_addr']}"
        hints.append(f"{len(fs.structural)} structural block(s) (different instruction counts), first at {at}: "
                     "stack pairs there unknown")
    differs = bool(esp or fs.operand_mismatch or any(e["kind"] != "same" for e in ebp) or fo != fr)
    res["stack"] = "differs" if differs else ("same" if fs.pairs else "no_stack_operands")
    res["complete"] = not fs.structural          # False: structural blocks were not compared
    res["hints"] = hints
    return res


# --- build ----------------------------------------------------------------------------------------------------

def _run(cmd, env, cwd) -> tuple[int, str]:
    p = subprocess.run([str(c) for c in cmd], env=env, cwd=cwd, capture_output=True, text=True, errors="replace")
    return p.returncode, p.stdout + p.stderr


def _code_sections(obj: Path) -> list:
    o = coff.parse(obj)
    # compiler-generated labels ($L1234, $T1234) are numbered differently with /Zi: compare them as one name
    names = [re.sub(r"^\$[A-Z]+\d+$", "$label", s.name) if s else None for s in o.symbols]
    return [(s.name, s.data, [(off, names[ix] if ix < len(names) else ix, t) for off, ix, t in s.relocs])
            for s in o.sections if s.chars & 0x20]     # IMAGE_SCN_CNT_CODE


@dataclass
class Build:
    dll: Path | None = None
    pdb: Path | None = None
    mapf: Path | None = None
    errors: dict = field(default_factory=dict)          # source -> compiler output
    zi_neutral: dict = field(default_factory=dict)      # source -> bool
    log: str = ""


def link_base(client: str) -> int:
    """LINK_BASE, unless the client's image covers it (eqmain.dll at 0x10000000): then ALT_LINK_BASE."""
    base = clients.get(client).base
    return ALT_LINK_BASE if base and base <= LINK_BASE < base + 0x10000000 else LINK_BASE


def build(repo: Path, ws: Workspace, sources, out: Path, client: str = "trilogy") -> Build:
    """Near-miss sources (match mode + /Zi) + eqd stubs -> one DLL with a consolidated PDB. Each source with
    its own toolchain (the client's, or its `// TOOLCHAIN:`); the stubs and the link with the client's."""
    import compare as C
    vc6 = ws.vc6(client)
    env = vc6_env(vc6)
    cl, link = vc6 / "Bin" / "CL.EXE", vc6 / "Bin" / "LINK.EXE"
    out.mkdir(parents=True, exist_ok=True)
    (out / "obj").mkdir(exist_ok=True)
    (out / "nozi").mkdir(exist_ok=True)
    fns, data, _ = C.match_symbols(repo)
    header = out / "eqd_match_syms.h"
    C.write_match_header(header, fns, data)
    b = Build()
    objs = []
    for src in sources:
        stem = re.sub(r"[^A-Za-z0-9]+", "_", src.relative_to(repo).with_suffix("").as_posix())
        base = cflags_for(src) + ["/DEQD_MATCH", f"/FI{header}", f"/I{repo / 'include'}"]
        obj = out / "obj" / f"{stem}.obj"
        svc6 = source_vc6(src, vc6)
        scl, senv = (cl, env) if svc6 == vc6 else (svc6 / "Bin" / "CL.EXE", vc6_env(svc6))
        rc, text = _run([scl] + base + ["/Zi", f"/Fd{out / 'obj' / 'vc60.pdb'}", f"/Fo{obj}", src], senv, out)
        if rc:
            b.errors[str(src)] = text
            continue
        objs.append(obj)
        nz = out / "nozi" / f"{stem}.obj"
        rc2, _ = _run([scl] + base + [f"/Fo{nz}", src], senv, out)
        b.zi_neutral[str(src)] = (rc2 == 0 and _code_sections(obj) == _code_sections(nz))
    stubs = out / "obj" / "eqd_stubs.c"
    stubs.write_text("/* Generated by stackcmp.py: definitions for the EQD_MATCH symbols. */\n"
                     + "".join(f"void __cdecl eqd_fn_{v}(void) {{}}\n" for v in sorted(fns, key=lambda t: int(t, 16)))
                     + "".join(f"char eqd_va_{v};\n" for v in sorted(data, key=lambda t: int(t, 16)))
                     + FLTUSED_SRC, encoding="utf-8")
    rc, text = _run([cl, "/nologo", "/c", f"/Fo{out / 'obj'}\\", stubs], env, out)
    if rc:
        b.log = text
        return b
    objs.append(out / "obj" / "eqd_stubs.obj")
    dll, pdb, mapf = out / "stackcmp.dll", out / "stackcmp.pdb", out / "stackcmp.map"
    cmd = [link, "/nologo", "/DLL", "/NOENTRY", "/FIXED:NO", f"/BASE:{link_base(client):#x}", "/DEBUG", "/PDBTYPE:CON",
           "/INCREMENTAL:NO", f"/PDB:{pdb}", f"/MAP:{mapf}", f"/OUT:{dll}", "/OPT:NOREF"] + LINK_EXTRA + objs + SYSTEM_LIBS
    rc, text = _run(cmd, env, out)
    if rc:
        # A rewrite calling another rewrite that isn't in this DLL: keep going, those calls go nowhere.
        rc, text2 = _run(cmd + ["/FORCE:UNRESOLVED"], env, out)
        text += text2
    b.log = text
    if dll.is_file() and pdb.is_file():
        b.dll, b.pdb, b.mapf = dll, pdb, mapf
    return b


# --- reccmp ---------------------------------------------------------------------------------------------------

def orig_sizes(ws: Workspace, client: str) -> dict:
    exp = ws.build / "export" / client / "functions.json"
    if not exp.is_file():
        return {}
    return {int(f["address"], 16): int(f["size"]) for f in json.loads(exp.read_text(encoding="utf-8"))["functions"]
            if f.get("size")}


def data_csv(rows_fn, sizes: dict, mapf: Path, fns, data, aliases: dict, libnames: dict) -> str:
    """CSV for reccmp's data-sources: orig sizes of the near misses, and the DLL's EQD_MATCH stubs / CRT
    copies matched by linker symbol to their client addresses."""
    import compare as C
    present = set(C.map_objects(mapf))
    buf = io.StringIO()
    w = csv.writer(buf, lineterminator="\n")
    w.writerow(["address", "type", "name", "symbol", "size"])
    for va in rows_fn:
        if va in sizes:
            w.writerow([f"{va:x}", "", "", "", sizes[va]])
    for v in fns:
        sym = f"_eqd_fn_{v}"
        if sym in present and int(v, 16) not in rows_fn:
            w.writerow([f"{int(v, 16):x}", "function", "", sym, ""])
    for v in data:
        sym = f"_eqd_va_{v}"
        if sym in present:
            w.writerow([f"{int(v, 16):x}", "global", "", sym, ""])
    for sym, va in list(libnames.items()) + list(aliases.items()):
        if sym in present and va not in rows_fn:
            w.writerow([f"{va:x}", "library", "", sym, ""])
    return buf.getvalue()


class StackCompare:
    """reccmp's Compare of an original image with a rebuilt one (+ its PDB): `hints(va)` -> analyse() result.
    `csv_text`: reccmp data-sources CSV (address,type,name,symbol,size); `code_files`: reccmp TextFiles."""

    def __init__(self, orig: Path, recomp: Path, pdb: Path, csv_text: str = "", csv_path=None, code_files=(),
                 target_id: str = reccmp_map.TARGET_ID):
        from reccmp.compare import Compare
        from reccmp.cvdump import Cvdump, CvdumpAnalysis
        from reccmp.formats import detect_image
        from reccmp.formats.textfile import TextFile
        cv = Cvdump(str(pdb)).lines().globals().publics().symbols().section_contributions().types().run()
        self.analysis = CvdumpAnalysis(cv)
        data = [TextFile(Path(csv_path or "stackcmp-data.csv").resolve(), csv_text)] if csv_text else []
        self.cmp = Compare(detect_image(Path(orig)), detect_image(Path(recomp)), self.analysis, target_id,
                           code_files=list(code_files), data_sources=data)
        self.cmp.run()
        self.nodes = {n.addr: n for n in self.analysis.nodes if getattr(n, "addr", None) is not None}

    def hints(self, va: int) -> dict | None:
        """None when reccmp has no rewrite for `va`; else analyse() + rewrite_va / reccmp_accuracy."""
        m = self.cmp.compare_address(va)
        if m is None or m.rdiff is None or m.recomp_addr is None:
            return None
        res = {"rewrite_va": f"0x{m.recomp_addr:08X}", "reccmp_accuracy": round(m.accuracy, 4)}
        res.update(analyse(m.rdiff, bprel_symbols(self.nodes.get(m.recomp_addr), self.cmp.types)))
        return res


def run(repo: Path, nearmiss: Path, client="trilogy", out=None, only_va=None, log=print, verbose=False) -> dict:
    import compare as C
    if not verbose:
        logging.getLogger("reccmp").setLevel(logging.CRITICAL)
    repo = Path(repo).resolve()
    ws = Workspace(repo)
    out = Path(out or ws.build / client / "stackcmp").resolve()
    rows = [r for r in read_nearmiss(nearmiss) if only_va is None or r["va"] == only_va]
    sources = []
    for r in rows:
        s = (repo / r["source"]).resolve()
        if s not in sources:
            sources.append(s)
    t0 = time.time()
    b = build(repo, ws, sources, out, client)
    report: dict = {"schema": SCHEMA, "tool": "stackcmp", "client": client,
                    "generated": time.strftime("%Y-%m-%dT%H:%M:%S"), "nearmiss": str(nearmiss),
                    "build": {"dll": str(b.dll) if b.dll else None, "pdb": str(b.pdb) if b.pdb else None,
                              "zi_neutral": {Path(k).relative_to(repo).as_posix(): v for k, v in b.zi_neutral.items()},
                              "errors": {Path(k).relative_to(repo).as_posix(): v[-600:] for k, v in b.errors.items()}},
                    "functions": []}
    try:
        import reccmp
        report["reccmp_version"] = reccmp.VERSION
    except Exception:  # noqa: BLE001
        pass
    if b.dll is None:
        report["build"]["log"] = b.log[-2000:]
        for r in rows:
            report["functions"].append(_row(r, "error", error="DLL build failed"))
        return report
    log(f"stackcmp: built {b.dll.name} from {len(sources) - len(b.errors)} source(s) in {time.time() - t0:.0f}s")
    fns, data, _ = C.match_symbols(repo)
    aliases, _ = C.load_defaults(repo / clients.get(client).crt_defaults, repo)
    csv_text = data_csv({r["va"] for r in rows}, orig_sizes(ws, client), b.mapf, fns, data, aliases,
                        C.library_names(ws, client))
    csv_path = out / "stackcmp-data.csv"
    csv_path.write_text(csv_text, encoding="utf-8")
    sc = StackCompare(ws.binary(client), b.dll, b.pdb, csv_text, csv_path, reccmp_map.code_files(sources),
                      target_id=clients.get(client).token)
    log(f"stackcmp: reccmp analysis done in {time.time() - t0:.0f}s")
    for r in rows:
        if str((repo / r["source"]).resolve()) in b.errors:
            report["functions"].append(_row(r, "error", error="source did not build with /Zi"))
            continue
        try:
            res = sc.hints(r["va"])
        except Exception as e:  # noqa: BLE001
            report["functions"].append(_row(r, "error", error=f"{type(e).__name__}: {e}"))
            continue
        if res is None:
            report["functions"].append(_row(r, "no_match", error="reccmp found no rewrite for this address"))
            continue
        fr = _row(r, "ok")
        fr.update(res)
        report["functions"].append(fr)
    return report


def _row(r: dict, status: str, error: str | None = None) -> dict:
    d = {"va": f"0x{r['va']:08X}", "name": r["name"], "source": r["source"], "ndiff": r["ndiff"], "status": status}
    if error:
        d["error"] = error
    return d


def summary_line(f: dict) -> str:
    if f["status"] != "ok":
        return f"{f['va']} {f['name']}: {f['status']} ({f.get('error', '')})"
    head = f"{f['va']} {f['name']}: stack {f['stack']}, reccmp {f['reccmp_accuracy']:.0%}"
    return head + "".join(f"\n    - {h}" for h in f["hints"])


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", default=".", help="decomp repo (default: the current directory)")
    ap.add_argument("--client", default="trilogy")
    ap.add_argument("--nearmiss", required=True, help="near-miss list from compare.py --match --nearmiss")
    ap.add_argument("--va", help="only this function (original VA)")
    ap.add_argument("--out", help="build folder (default <build>\\<client>\\stackcmp)")
    ap.add_argument("--json", help="write the hints here")
    ap.add_argument("-v", "--verbose", action="store_true", help="hints per function and reccmp's own log")
    args = ap.parse_args(argv)
    if args.verbose:
        logging.basicConfig(level=logging.WARNING)
    rep = run(Path(args.repo), Path(args.nearmiss), args.client, args.out,
              int(args.va, 16) if args.va else None, verbose=args.verbose)
    counts: dict = {}
    for f in rep["functions"]:
        k = f["status"] if f["status"] != "ok" else f"stack {f['stack']}"
        counts[k] = counts.get(k, 0) + 1
        if args.verbose or args.va:
            print(summary_line(f))
    bad_zi = [k for k, v in rep["build"]["zi_neutral"].items() if not v]
    print(f"stackcmp: {len(rep['functions'])} function(s): " + ", ".join(f"{v} {k}" for k, v in sorted(counts.items()))
          + (f"; /Zi changed code in {bad_zi}" if bad_zi else ""))
    if args.json:
        Path(args.json).parent.mkdir(parents=True, exist_ok=True)
        Path(args.json).write_text(json.dumps(rep, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
