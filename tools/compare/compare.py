r"""Instruction-level comparison of rewrites against the original binary (are they "instruction-identical"?).

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\compare\compare.py                        every src\**\*.cpp / *.c
    ..\.venv\Scripts\python.exe tools\compare\compare.py src\Network\rdp\heap.c -v      with diffs
    ..\.venv\Scripts\python.exe tools\compare\compare.py --va 0x4F7C70 --show            both listings
    ..\.venv\Scripts\python.exe tools\compare\compare.py --json ..\build\trilogy\compare.json

Each source is built on its own into a test DLL exactly like the Unicorn harness does (eqd_unicorn.vc6.build_dll:
DEFAULT_CFLAGS + the source's `// CFLAGS:` lines) into <build>\<client>\compare\. For every
`// FUNCTION: <CLIENT> 0x...` annotation the original's instructions (recursive descent, as the harness) and
the rewrite's are disassembled in address order and normalised, then compared. See README.md for the
normalisation rules and the verdicts.
"""
from __future__ import annotations

import argparse
import csv
import difflib
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "unicorn"))
sys.path.insert(0, str(HERE))

import capstone  # noqa: E402
import pefile  # noqa: E402
from capstone import x86_const as X  # noqa: E402

from eqd_unicorn import Workspace, annotations as ann, build_dll, clients, load_pe, read_map, undecorate  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402
from eqd_unicorn.extent import function_extent  # noqa: E402
from eqd_unicorn import externs  # noqa: E402
from eqd_unicorn.vc6 import BuildError, cflags_for, source_vc6, vc6_env  # noqa: E402

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True
_HEX = re.compile(r"0x[0-9a-f]+")
_BRANCH = {"jmp", "call", "loop", "loope", "loopne", "jecxz", "jcxz"}


# --- images ---------------------------------------------------------------------------------------------------

@dataclass
class Side:
    """One image plus what is known about the addresses in it."""
    img: object                                     # eqd_unicorn.pe.Image
    relocs: set = field(default_factory=set)        # VAs of base-relocation fixups (DLL only)
    funcs: dict = field(default_factory=dict)       # VA -> key "F:<original va>" / "F:<symbol>" of functions
    is_rewrite: bool = False
    orig: object = None                             # rewrite: the original's Side (keys fixed client addresses)
    rel32: set = field(default_factory=set)         # match mode: REL32 relocation fields (masked in bytes)
    eqd_calls: set = field(default_factory=set)     # match mode: fields of calls to an EQD_MATCH function symbol

    def in_code(self, va):
        return any(lo <= va < lo + n for lo, n in self.img.code_ranges)


def reloc_sites(path, base) -> set:
    pe = pefile.PE(str(path), fast_load=False)
    out = set()
    delta = base - pe.OPTIONAL_HEADER.ImageBase
    for blk in getattr(pe, "DIRECTORY_ENTRY_BASERELOC", []):
        for e in blk.entries:
            if e.type == 3:                         # IMAGE_REL_BASED_HIGHLOW
                out.add(e.rva + pe.OPTIONAL_HEADER.ImageBase + delta)
    pe.close()
    return out


# --- one instruction -> token --------------------------------------------------------------------------------

@dataclass
class Ins:
    va: int
    size: int
    mnemonic: str
    text: str                       # normalised operand text, addresses replaced by keys
    insn: object
    drop: bool = False              # folded away
    note: str = ""
    target: int | None = None       # a branch inside the function: its target VA (labels: relabel)


def _value_sites(insn):
    """(operand index, value, byte offset of the 4-byte field in the instruction or None)."""
    out = []
    for i, op in enumerate(insn.operands):
        if op.type == X.X86_OP_IMM:
            out.append((i, op.imm & 0xFFFFFFFF, insn.imm_offset if insn.imm_size == 4 else None))
        elif op.type == X.X86_OP_MEM:
            out.append((i, op.mem.disp & 0xFFFFFFFF, insn.disp_offset if insn.disp_size == 4 else None))
    return out


def address_key(side: Side, va: int, field_va, extent: set, entry: int, data_size: int) -> str | None:
    """A canonical key for an address the instruction mentions, or None for a plain number.

    Rewrite: only relocated fields are DLL addresses; a fixed client address (EQ_GLOBAL, EQ_CDECL, ...) is an
    unrelocated number and is keyed like the original's. Original: any value inside the exe image."""
    img = side.img
    if side.is_rewrite and field_va in side.relocs:
        if va in extent:
            return f"L{va - entry:+x}"            # placeholder; branch labels are handled separately
        if va in side.funcs:
            return side.funcs[va]
        if va in img.imports:
            return "I:" + img.imports[va]
        if side.in_code(va):
            return "CODE"                          # switch tables after the function
        return f"R:{va:x}:{data_size}"            # the rewrite's own data: resolved against the original's
    if side.is_rewrite:                            # an unrelocated number: a fixed client address or a value
        return address_key(side.orig, va, None, frozenset(), 0, data_size) if side.orig else None
    if not img.contains(va) or va < img.base + 0x1000:
        return None
    if va in side.funcs:
        return side.funcs[va]
    if va in img.imports:
        return "I:" + img.imports[va]
    if side.in_code(va):
        return "CODE" if va not in extent else f"L{va - entry:+x}"
    return None                                    # client data: kept as its number (EQ_GLOBAL matches it)


def tokenise(side: Side, entry: int, extent: set) -> list[Ins]:
    addrs = sorted(extent)
    index = {a: i for i, a in enumerate(addrs)}
    out = []
    for a in addrs:
        insn = next(_MD.disasm(side.img.read(a, 16), a, 1))
        text = insn.op_str
        m = insn.mnemonic
        if m in _BRANCH or (insn.group(capstone.CS_GRP_JUMP)):
            op = insn.operands[0] if insn.operands else None
            if op is not None and op.type == X.X86_OP_IMM:
                tgt = op.imm & 0xFFFFFFFF
                if tgt in index and m != "call":               # a branch inside the function
                    out.append(Ins(a, insn.size, m, f"@{index[tgt]}", insn, target=tgt))
                    continue
                elif tgt in side.funcs:
                    text = side.funcs[tgt]
                else:                                          # a call/tail jump to an unknown address
                    text = f"?:{tgt:x}" if side.is_rewrite else f"F:{tgt:x}"
                out.append(Ins(a, insn.size, m, text, insn,
                               note="eqd" if (a + insn.size - 4) in side.eqd_calls else ""))
                continue
        # Replace every address-like value by its key, longest numbers first so 0x10 doesn't eat 0x1000.
        reps = {}
        for i, v, off in _value_sites(insn):
            fva = a + off if off is not None else None
            op = insn.operands[i]
            size = op.size if op.type == X.X86_OP_MEM else 4
            key = address_key(side, v, fva, extent, entry, size)
            if key is not None:
                reps[v] = key
        for v in sorted(reps, reverse=True):
            text = re.sub(rf"(?<![0-9a-fx])0x0*{v:x}(?![0-9a-f])", reps[v], text)
        out.append(Ins(a, insn.size, m, text, insn))
    return out


def relabel(seq: list[Ins]) -> None:
    """Branch labels = the target's index among the instructions not folded away (a dropped target counts as
    the next kept one), so a folded `xor edx, edx` / `mov reg, VA` doesn't renumber the labels after it."""
    kept = [i for i in seq if not i.drop]
    pos = {ins.va: k for k, ins in enumerate(kept)}
    nxt, after = len(kept), {}
    for ins in reversed(seq):
        if not ins.drop:
            nxt = pos[ins.va]
        after[ins.va] = nxt
    for ins in seq:
        if ins.target is not None:
            ins.text = f"@{after.get(ins.target, len(kept))}"


# --- folding: `mov reg, VA; call reg` == `call VA`, the dummy EDX of EQ_THISCALL ------------------------------

_REG32 = {X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX, X.X86_REG_EBX, X.X86_REG_ESI, X.X86_REG_EDI,
          X.X86_REG_EBP}
_SUBREGS = {
    X.X86_REG_EAX: {X.X86_REG_EAX, X.X86_REG_AX, X.X86_REG_AL, X.X86_REG_AH},
    X.X86_REG_ECX: {X.X86_REG_ECX, X.X86_REG_CX, X.X86_REG_CL, X.X86_REG_CH},
    X.X86_REG_EDX: {X.X86_REG_EDX, X.X86_REG_DX, X.X86_REG_DL, X.X86_REG_DH},
    X.X86_REG_EBX: {X.X86_REG_EBX, X.X86_REG_BX, X.X86_REG_BL, X.X86_REG_BH},
    X.X86_REG_ESI: {X.X86_REG_ESI, X.X86_REG_SI},
    X.X86_REG_EDI: {X.X86_REG_EDI, X.X86_REG_DI},
    X.X86_REG_EBP: {X.X86_REG_EBP, X.X86_REG_BP},
}


def _rw(insn):
    try:
        r, w = insn.regs_access()
    except capstone.CsError:
        return set(), set()
    return set(r), set(w)


def _is_call_reg(ins: Ins, reg) -> bool:
    ops = ins.insn.operands
    return ins.mnemonic in ("call", "jmp") and len(ops) == 1 and ops[0].type == X.X86_OP_REG and ops[0].reg == reg


def fold(seq: list[Ins]) -> int:
    """Fold `mov r32, <function key>` + later `call r32` / `jmp r32` (only reads of r32 until it's
    redefined) into `call <key>` / `jmp <key>` and drop the mov (EQ_CDECL/EQ_STDCALL/EQ_THISCALL calls; VC6
    calls a constant function pointer through a register). Then drop an `xor edx, edx` whose value nothing
    reads before the next transfer of control, when that is a folded call (EQ_THISCALL's dummy EDX
    argument). Applied to both sides. Returns the number of folds."""
    n = 0
    for i, ins in enumerate(seq):
        ops = ins.insn.operands
        if ins.mnemonic != "mov" or len(ops) != 2 or ops[0].type != X.X86_OP_REG or ops[0].reg not in _REG32:
            continue
        src = ins.text.split(", ", 1)[1] if ", " in ins.text else ""
        if ops[1].type == X.X86_OP_IMM:
            key = src if src.startswith(("F:", "I:")) else (f"F:{ops[1].imm & 0xFFFFFFFF:x}"
                                                            if re.fullmatch(r"0x[0-9a-f]{6,8}", src) else None)
        elif ops[1].type == X.X86_OP_MEM and src.startswith("dword ptr [I:"):
            key = src[len("dword ptr ["):-1]
        else:
            key = None
        if key is None:
            continue
        reg, sub = ops[0].reg, _SUBREGS[ops[0].reg]
        calls, ok = [], True
        for j in range(i + 1, len(seq)):
            r, w = _rw(seq[j].insn)
            volatile = reg in (X.X86_REG_EAX, X.X86_REG_ECX, X.X86_REG_EDX)
            if _is_call_reg(seq[j], reg):
                calls.append(j)
                if volatile:                 # the call clobbers it: what follows reads the return value
                    break
                continue
            if r & sub:
                ok = False
                break
            if w & sub:
                break
            if seq[j].mnemonic == "call" and volatile:
                break
        if ok and calls:
            ins.drop, ins.note = True, f"folded into call {key}"
            for j in calls:
                seq[j].text = key if key.startswith("F:") else f"dword ptr [{key}]"
                seq[j].note = "folded"
            n += 1
    for i, ins in enumerate(seq):
        if ins.drop or ins.mnemonic != "xor" or ins.text != "edx, edx":
            continue
        for j in range(i + 1, len(seq)):
            if seq[j].mnemonic in ("call", "jmp", "ret") or seq[j].insn.group(capstone.CS_GRP_JUMP):
                if seq[j].note in ("folded", "eqd"):   # the first transfer is a folded / EQD_MATCH call
                    ins.drop, ins.note = True, "dummy edx of a folded call"
                    n += 1
                break
            r, w = _rw(seq[j].insn)
            if (r | w) & _SUBREGS[X.X86_REG_EDX]:
                break
    return n


# --- data references: the rewrite's literals vs the original's ------------------------------------------------

def _cstr(img, va, limit=256):
    if not img.contains(va):
        return None
    b = img.read(va, limit)
    end = b.find(b"\0")
    if end < 0:
        return None
    s = b[:end]
    return s if s and all(32 <= c < 127 or c in (9, 10, 13) for c in s) else None


def resolve_data(orig: list[str], rew: list[str], o_side: Side, r_side: Side) -> tuple[list[str], int]:
    """Where an aligned pair differs only in that the rewrite references its own data (R:<va>:<size>) where the
    original references client data, accept it when the contents agree (same C string, or the same bytes for
    the operand size: float constants). Returns the rewrite tokens with matched keys replaced."""
    rmap: dict = {}
    sm = difflib.SequenceMatcher(None, orig, rew, autojunk=False)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != "replace" or i2 - i1 != j2 - j1:
            continue
        for o, r in zip(orig[i1:i2], rew[j1:j2]):
            keys = re.findall(r"R:([0-9a-f]+):(\d+)", r)
            if not keys:
                continue
            # pattern: the rewrite token with each R:<va>:<n> slot turned into a capture of a client address
            rx = re.escape(r)
            for rk, sz in keys:
                rx = rx.replace(re.escape(f"R:{rk}:{sz}"), r"(0x[0-9a-f]+)")
            m = re.fullmatch(rx, o)
            if not m:
                continue
            for (rk, sz), ov in zip(keys, m.groups()):
                rva, ova, sz = int(rk, 16), int(ov, 16), int(sz)
                s_r, s_o = _cstr(r_side.img, rva), _cstr(o_side.img, ova)
                if s_r is not None and s_o is not None:
                    same = s_r == s_o
                else:
                    same = o_side.img.contains(ova) and r_side.img.read(rva, sz or 4) == o_side.img.read(ova, sz or 4)
                if same and rmap.get(f"R:{rk}:{sz}", ov) == ov:
                    rmap[f"R:{rk}:{sz}"] = ov
    out = []
    for t in rew:
        for k, v in rmap.items():
            t = t.replace(k, v)
        out.append(t)
    return out, len(rmap)


# --- driver ---------------------------------------------------------------------------------------------------

@dataclass
class Result:
    va: int
    name: str
    source: str
    verdict: str                    # identical | identical_normalised | differs | error
    ratio: float = 0.0
    folds: int = 0
    data: int = 0
    orig_count: int = 0
    rew_count: int = 0
    first_diff: str = ""
    ndiff: int = 0                  # instructions that differ (same count) / lines in non-equal blocks
    structural: bool = False        # instruction counts differ
    prefix: int = 0                 # leading normalised instructions equal to the original's (dense score)
    first_diff_at: int | None = None  # byte offset of the first differing original instruction from the entry
    bytes_identical: bool | None = None   # same bytes outside relocation/rel32-out fields (match mode)
    flags: str = ""                 # match mode: the cl options the object was built with
    target: dict | None = None      # match mode with --target-obj: the check against the delinked object
    diff: list = field(default_factory=list)
    orig_listing: list = field(default_factory=list)
    rew_listing: list = field(default_factory=list)
    o_span: list = field(default_factory=list)      # (va, size) of each compared instruction, both sides
    r_span: list = field(default_factory=list)

    def as_json(self):
        d = {k: v for k, v in self.__dict__.items()
             if k not in ("diff", "orig_listing", "rew_listing", "o_span", "r_span")}
        if self.target is None:
            d.pop("target")
        d["va"] = f"0x{self.va:08X}"
        return d


def _tokens(seq):
    return [f"{i.mnemonic} {i.text}".strip() for i in seq if not i.drop]


EXTRA_SIZES: dict = {}          # original VA -> byte size, from --size (on top of the fixtures' `size`)


def fixture_sizes(repo: Path, client="trilogy") -> dict:
    """{original VA: size} from the `size` of this image's fixtures (tests/unicorn/**/*.json, eqmain's under
    tests/unicorn/eqmain/) plus --size: the extent escape hatch difftest has, for a function whose extent can't
    be decoded (a switch table with no bound)."""
    root = Path(repo) / "tests" / "unicorn"
    out = {}
    for p in sorted(root.rglob("*.json")) if root.is_dir() else ():
        rel = p.relative_to(root).as_posix()
        if "negative" in rel.split("/") or "/" not in rel or not clients.owns(client, rel):
            continue
        try:
            txt = p.read_text(encoding="utf-8")
            if '"size"' not in txt:
                continue
            d = json.loads(txt)
            if d.get("size") and d.get("original"):
                va = d["original"]
                out[int(str(va), 16) if isinstance(va, str) else int(va)] = int(str(d["size"]), 0)
        except (ValueError, OSError, AttributeError, TypeError):
            continue
    out.update(EXTRA_SIZES)
    return out


def compare_function(o_side: Side, r_side: Side, va: int, r_entry: int, o_stops, r_stops,
                     o_size: int | None = None) -> Result:
    """`o_size`: limit the original to [va, va + o_size) (see fixture_sizes)."""
    o_ext = function_extent(o_side.img.read, va, o_stops, size=o_size, in_image=o_side.img.contains)
    r_ext = function_extent(r_side.img.read, r_entry, r_stops, in_image=r_side.img.contains)
    o_seq, r_seq = tokenise(o_side, va, o_ext), tokenise(r_side, r_entry, r_ext)
    raw_equal = _tokens(o_seq) == _tokens(r_seq)
    folds = fold(o_seq) + fold(r_seq) if not raw_equal else 0
    if folds:
        relabel(o_seq)
        relabel(r_seq)
    o_t, r_t = _tokens(o_seq), _tokens(r_seq)
    r_t, ndata = resolve_data(o_t, r_t, o_side, r_side)
    res = Result(va, "", "", "", orig_count=len(o_t), rew_count=len(r_t), folds=folds, data=ndata)
    res.orig_listing = [f"{i.va:08x}  {i.mnemonic} {i.text}{'   ; ' + i.note if i.note else ''}" for i in o_seq]
    res.rew_listing = [f"{i.va:08x}  {i.mnemonic} {i.text}{'   ; ' + i.note if i.note else ''}" for i in r_seq]
    res.o_span, res.r_span = [(i.va, i.size) for i in o_seq], [(i.va, i.size) for i in r_seq]
    # Similarity on tokens without branch labels: one extra instruction renumbers every later label.
    unlabel = re.compile(r"@\d+")
    res.ratio = round(difflib.SequenceMatcher(None, [unlabel.sub("@", t) for t in o_t],
                                              [unlabel.sub("@", t) for t in r_t], autojunk=False).ratio(), 4)
    sm = difflib.SequenceMatcher(None, o_t, r_t, autojunk=False)
    res.structural = len(o_t) != len(r_t)
    if not res.structural:
        res.ndiff = sum(a != b for a, b in zip(o_t, r_t))
    else:
        res.ndiff = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal")
    res.bytes_identical = _bytes_equal(o_side, r_side, o_seq, r_seq)
    res.prefix = next((k for k, (a, b) in enumerate(zip(o_t, r_t)) if a != b), min(len(o_t), len(r_t)))
    if o_t != r_t:
        o_kept = [i for i in o_seq if not i.drop]
        res.first_diff_at = (o_kept[res.prefix].va - va if res.prefix < len(o_kept)
                             else (o_kept[-1].va + o_kept[-1].size - va if o_kept else 0))
    if o_t == r_t:
        res.verdict = "identical" if raw_equal else "identical_normalised"
    else:
        res.verdict = "differs"
        for tag, i1, i2, j1, j2 in sm.get_opcodes():
            if tag != "equal":
                a = o_t[i1] if i1 < len(o_t) else "<end>"
                b = r_t[j1] if j1 < len(r_t) else "<end>"
                res.first_diff = f"#{i1}: original `{a}` | rewrite `{b}`"
                break
        res.diff = list(difflib.unified_diff(o_t, r_t, "original", "rewrite", lineterm="", n=2))
    return res


def _bytes_equal(o_side: Side, r_side: Side, o_seq, r_seq) -> bool:
    """Byte-identical outside the fields a link fills in: the rewrite's relocation fields (and rel32 fields of
    calls/jumps leaving the function), masked at the same offsets on both sides. Needs the same instruction
    sizes in the same order and nothing folded away."""
    if any(i.drop for i in r_seq) or any(i.drop for i in o_seq) or len(o_seq) != len(r_seq):
        return False
    fields = r_side.relocs | set(getattr(r_side, "rel32", ()))
    r_ext = {i.va for i in r_seq}
    for o, r in zip(o_seq, r_seq):
        if o.size != r.size:
            return False
        ob, rb = bytearray(o_side.img.read(o.va, o.size)), bytearray(r_side.img.read(r.va, r.size))
        masks = [f - r.va for f in range(r.va, r.va + r.size) if f in fields]
        ops = r.insn.operands
        if r.mnemonic in _BRANCH or r.insn.group(capstone.CS_GRP_JUMP):
            if ops and ops[0].type == X.X86_OP_IMM and (ops[0].imm & 0xFFFFFFFF) not in r_ext \
                    and r.size >= 5:
                masks.append(r.size - 4)
        for m in masks:
            ob[m:m + 4] = rb[m:m + 4] = bytes(4)
        if ob != rb:
            return False
    return True


def function_starts(ws: Workspace, client: str, repo: Path) -> set:
    """Every function of the client's export plus every address in its names/<stem>.*.csv lists."""
    starts = set()
    exp = ws.build / "export" / client / "functions.json"
    if exp.is_file():
        starts.update(int(f["address"], 16) for f in json.loads(exp.read_text(encoding="utf-8"))["functions"])
    stem = clients.get(client).names or "*"
    for f in (repo / "names").glob(f"{stem}.*.csv"):
        with open(f, newline="", encoding="utf-8") as fh:
            for row in csv.DictReader(fh):
                try:
                    starts.add(int(row["address"], 16))
                except (KeyError, ValueError, TypeError):
                    pass
    return starts


_CXX_OPS = {"??2@YAPAXI@Z": "operator_new", "??3@YAXPAX@Z": "operator_delete"}


def library_names(ws: Workspace, client: str) -> dict:
    """Test-DLL symbol -> client VA for the CRT/library functions the DLL links its own copy of (`_malloc`,
    `??2@YAPAXI@Z`, ...): matched by name against the client export (`malloc`, `_malloc`, `strcpy_guess`,
    `operator_new`). Names that occur twice in the export are left out."""
    exp = ws.build / "export" / client / "functions.json"
    if not exp.is_file():
        return {}
    seen: dict = {}
    for f in json.loads(exp.read_text(encoding="utf-8"))["functions"]:
        n = re.sub(r"_guess$", "", f["name"])
        if n.startswith("FUN_") or "::" in n:
            continue
        for k in {n, "_" + n}:
            seen.setdefault(k, set()).add(int(f["address"], 16))
    out = {k: next(iter(v)) for k, v in seen.items() if len(v) == 1}
    for sym, n in _CXX_OPS.items():
        if n in out:
            out[sym] = out[n]
    return out


def load_defaults(path, repo: Path) -> tuple[dict, list]:
    """From a fixture-defaults JSON: (rewrite_aliases as {DLL CRT symbol: client VA}, "link" sources)."""
    if not path or not Path(path).is_file():
        return {}, []
    d = json.loads(Path(path).read_text(encoding="utf-8"))
    aliases = {k: int(v, 16) for k, v in (d.get("rewrite_aliases") or {}).items()
               if isinstance(v, str) and v.lower().startswith("0x")}
    return aliases, [repo / x for x in d.get("link", [])]


_MAP_SYM = re.compile(r"^\s*[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\s+(?:f\s+)?(?:i\s+)?(\S+)\s*$")
_LST_TITLE = re.compile(r"^\s*TITLE\s+(.+?)\s*$")
_LST_FILE = re.compile(r"^; File (.+?)\s*$")
_LST_PROC = re.compile(r"^(\S+)\s+PROC\b")
_LST_LINE = re.compile(r"^; (\d+)\s+:")


def map_objects(mapf: Path) -> dict:
    """Decorated symbol -> object file name (lower case), from a VC6 /MAP."""
    out = {}
    for line in Path(mapf).read_text(errors="replace").splitlines():
        m = _MAP_SYM.match(line)
        if m:
            out.setdefault(m.group(1), m.group(3).lower())
    return out


def listing_lines(src: Path, ws: Workspace, repo: Path, out_dir: Path, client: str | None = None) -> dict:
    """{decorated symbol: first source line of its body}: the source compiled once more with a /FAs listing
    (same flags), as the hook build does to tell overloads apart."""
    d = out_dir / "listing"
    d.mkdir(parents=True, exist_ok=True)
    lst, obj = d / (src.stem + ".asm"), d / (src.stem + ".obj")
    vc6 = source_vc6(src, ws.vc6(client))
    cmd = [str(vc6 / "Bin" / "CL.EXE")] + cflags_for(src) + [f"/I{repo / 'include'}", "/FAs", f"/Fa{lst}",
                                                             f"/Fo{obj}", str(src)]
    if subprocess.run(cmd, env=vc6_env(vc6), capture_output=True).returncode:
        return {}
    want = os.path.normcase(str(src.resolve()))
    cur, pending, out = None, None, {}
    for line in lst.read_text(errors="replace").splitlines():
        m = _LST_TITLE.match(line) or _LST_FILE.match(line)
        if m:
            cur = os.path.normcase(str(Path(m.group(1)).resolve()))
            continue
        m = _LST_PROC.match(line)
        if m:
            pending = m.group(1)
            continue
        m = _LST_LINE.match(line)
        if m and pending:
            if cur == want:
                out.setdefault(pending, int(m.group(1)))
            pending = None
    return out


def build_images(repo: Path, ws: Workspace, sources, out_dir: Path, log=print, link=(),
                 client: str | None = None) -> tuple[dict, dict]:
    """-> ({source: (dll, map)}, {source: error}). All sources in one DLL (cross-file calls resolve, like the
    hook DLL); when that fails, one DLL per source, so one broken file only loses its own functions. `link`:
    extra sources linked into every DLL (the defaults' "link", e.g. the client's own __ftol)."""
    inc = [repo / "include"]
    link = [x for x in link if x not in sources]
    tag = hashlib.sha1("|".join(sorted(str(s) for s in sources)).encode()).hexdigest()[:8]
    if len({Path(s).stem.lower() for s in list(sources) + link}) == len(sources) + len(link):
        try:
            dm = build_dll(f"cmp_all_{tag}", list(sources) + link, out_dir, include_dirs=inc, vc6=ws.vc6(client))
            return {s: dm for s in sources}, {}
        except BuildError as e:
            log(f"compare: building all sources together failed, building one DLL per source\n  {str(e)[:400]}")
    images, errors = {}, {}
    for s in sources:
        rel = Path(s).resolve()
        rel = rel.relative_to(repo).as_posix() if rel.is_relative_to(repo) else rel.name
        try:
            images[s] = build_dll("cmp_" + re.sub(r"[^A-Za-z0-9]+", "_", str(Path(rel).with_suffix(""))),
                                  [s] + link, out_dir, include_dirs=inc, vc6=ws.vc6(client))
        except BuildError as e:
            errors[s] = str(e)
    return images, errors


# The test DLLs are linked at 0x10000000 (vc6.LINK_BASE): an image that covers that address (eqmain.dll,
# eqgfx_dx7.dll) gets the rewrite loaded here instead, so the two address ranges never mix in the normaliser.
REWRITE_LOAD_BASE = 0x30000000


def rewrite_base(o_side, size=0x01000000):
    """None (the DLL's own base) unless the original image overlaps [LINK_BASE, LINK_BASE + size)."""
    from eqd_unicorn.vc6 import LINK_BASE
    img = o_side.img
    return REWRITE_LOAD_BASE if img.base < LINK_BASE + size and LINK_BASE < img.end else None


def _load_rewrite(dll, mapf, srcs, anns, ws, repo, out_dir, aliases, libnames, o_side, client=None):
    """The rewrite image of one DLL, with every annotation of its sources resolved to its symbol:
    -> (Side, {(source, va): [entry VAs]})."""
    r_img = load_pe(dll, base=rewrite_base(o_side))
    read_map(mapf, r_img)
    r_side = Side(r_img, relocs=reloc_sites(dll, r_img.base), is_rewrite=True, orig=o_side)
    objs = map_objects(mapf)
    entry_of = {}
    for src in srcs:
        lines = None
        obj = (src.stem + ".obj").lower()
        for a in anns[src]:
            hits = [v for v, sym in r_img.symbols.items()
                    if undecorate(sym) == a.name and objs.get(sym, obj) == obj]
            if len(hits) > 1:                       # overloads: the first body below the annotation
                lines = lines if lines is not None else listing_lines(src, ws, repo, out_dir, client)
                after = sorted((lines.get(r_img.symbols[v], 0), v) for v in hits
                               if lines.get(r_img.symbols[v], 0) > a.line)
                hits = [after[0][1]] if after else hits
            entry_of[(src, a.va)] = hits
    for (_, va), hits in entry_of.items():
        if len(hits) == 1:
            r_side.funcs[hits[0]] = f"F:{va:x}"     # another rewrite: keyed by its original address
    for sva, sym in r_img.symbols.items():
        if sva in r_side.funcs or not r_side.in_code(sva):
            continue                                # data symbols: literals, resolved by content
        if sym in aliases:                          # the DLL's CRT -> the client's (crt_defaults.json)
            r_side.funcs[sva] = f"F:{aliases[sym]:x}"
        elif sym in libnames:                       # other library code: same name in the client export
            r_side.funcs[sva] = f"F:{libnames[sym]:x}"
        else:
            r_side.funcs[sva] = f"F:{sym}"
    return r_side, entry_of


def compare_sources(repo: Path, sources, client="trilogy", out_dir=None, aliases=None, only_va=None,
                    log=print, link=()) -> list[Result]:
    ws = Workspace(repo)
    o_side = Side(load_pe(ws.binary(client)))
    starts = function_starts(ws, client, repo)
    sizes = fixture_sizes(repo, client)
    libnames = library_names(ws, client)
    o_side.funcs = {s: f"F:{s:x}" for s in starts}
    out_dir = Path(out_dir or ws.build / client / "compare")
    aliases = aliases or {}
    sources = [Path(s).resolve() for s in sources]
    images, errors = build_images(repo, ws, sources, out_dir, log, [Path(x).resolve() for x in link], client)
    anns = {s: ann.scan([s], client.upper()) for s in sources}
    results = []
    loaded: dict = {}
    for src in sources:
        rel = src.relative_to(repo).as_posix() if src.is_relative_to(repo) else str(src)
        todo = [a for a in anns[src] if only_va is None or a.va == only_va]
        if not todo:
            continue
        if src in errors:
            results += [Result(a.va, a.name, rel, "error", first_diff=f"build failed: {errors[src][:300]}")
                        for a in todo]
            continue
        dll, mapf = images[src]
        if dll not in loaded:
            loaded[dll] = _load_rewrite(dll, mapf, [s for s in sources if images.get(s) == (dll, mapf)], anns,
                                        ws, repo, out_dir, aliases, libnames, o_side, client)
        r_side, entry_of = loaded[dll]
        r_stops = frozenset(r_side.img.symbols)
        for a in todo:
            hits = entry_of[(src, a.va)]
            if len(hits) != 1:
                results.append(Result(a.va, a.name, rel, "error",
                                      first_diff=f"{len(hits)} symbols named {a.name} (overloads?)"))
                continue
            try:
                res = compare_function(o_side, r_side, a.va, hits[0], frozenset(starts - {a.va}),
                                       r_stops - {hits[0]}, sizes.get(a.va))
            except Exception as e:                  # noqa: BLE001
                res = Result(a.va, a.name, rel, "error", first_diff=f"{type(e).__name__}: {e}")
            res.name, res.source = a.name, rel
            results.append(res)
    return results


# --- match mode: COFF objects built with EQD_MATCH ------------------------------------------------------------

_EQ_MACRO = re.compile(r"\bEQ_(CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY)\s*\(")
# `// EXTERNAL: <CLIENT> <va> <name>` (eqd_unicorn/externs.py): its VA token counts as a function VA too, except a
# vftable's (`??_7Class@@6B@`), which is data: no eqd_fn_ declaration, a new vftable leaves the shared header alone.
_EXTERNAL_MARKER = re.compile(r"^[ \t]*//[ \t]*EXTERNAL:[ \t]*\w+[ \t]+(0[xX][0-9a-fA-F]+)[ \t]+(?!\?\?_7)\S", re.M)


def _macro_args(text: str, start: int) -> list[str]:
    """Top-level comma-separated arguments of the macro call whose '(' is at text[start - 1]."""
    depth, cur, args, i = 0, [], [], start
    while i < len(text):
        c = text[i]
        if c == "(":
            depth += 1
        elif c == ")":
            if depth == 0:
                args.append("".join(cur).strip())
                return args
            depth -= 1
        elif c == "," and depth == 0:
            args.append("".join(cur).strip())
            cur = []
            i += 1
            continue
        cur.append(c)
        i += 1
    return args


_C_COMMENT_OR_LITERAL = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)


def blank_comments(text: str) -> str:
    """`text` with every comment replaced by spaces (newlines kept, so offsets and lines stay put)."""
    return _C_COMMENT_OR_LITERAL.sub(
        lambda m: m.group(0) if m.group(0)[0] in "\"'" else re.sub(r"[^\n]", " ", m.group(0)), text)


def match_symbols(repo: Path) -> tuple[set, set, list]:
    """Every VA token the eqaddr.h macros are used with in include/ and src/: (function VAs, data VAs,
    problems). Tokens as written (`0x004A3D01`); the EQD_MATCH expansion pastes them into symbol names.
    Uses inside comments (documentation) don't count; a code line starting with `*` (a dereference) does.
    EQ_CDECL_CPP / EQ_STDCALL_CPP (their symbol is eqd_cc<va>::f, eqaddr.h) and `// EXTERNAL:` markers count as
    function VAs as well, though no eqd_fn_<va> declaration is needed for them: VC6's code for a translation unit
    can change with the declarations it sees, so the generated header must not change when a call moves from
    EQ_CDECL to EQ_CDECL_CPP or from EQ_METHOD to a declared constructor (keep the VA token as written)."""
    fns, data, problems = set(), set(), []
    files = [p for d in ("include", "src") for p in (repo / d).rglob("*")
             if p.suffix.lower() in (".h", ".c", ".cpp") and p.is_file() and p.name != "eqaddr.h"]
    for f in files:
        raw = f.read_text(encoding="utf-8", errors="replace")
        fns.update(_EXTERNAL_MARKER.findall(raw))
        text = blank_comments(raw)
        for m in _EQ_MACRO.finditer(text):
            args = _macro_args(text, m.end())
            if m.group(1) == "METHOD":                  # EQ_METHOD(n, ret, va, params, obj)
                args = args[1:]
            if len(args) < 2:
                continue
            va = args[1]
            if not re.fullmatch(r"0[xX][0-9a-fA-F]+", va):
                if not re.fullmatch(r"[A-Za-z_]\w*", va):    # a macro parameter is fine (expanded later)
                    problems.append(f"{f.relative_to(repo).as_posix()}: EQ_{m.group(1)}(..., {va}) is not a hex VA")
                continue
            (data if m.group(1) in ("GLOBAL", "ARRAY") else fns).add(va)
    return fns, data, problems


def write_match_header(path: Path, fns, data):
    out = ["/* Generated by tools/compare/compare.py --match: the EQD_MATCH symbols of eqaddr.h. Do not edit. */",
           "#ifdef __cplusplus", 'extern "C" {', "#endif"]
    out += [f"void __cdecl eqd_fn_{v}(void);" for v in sorted(fns, key=lambda t: (int(t, 16), t))]
    out += [f"extern char eqd_va_{v};" for v in sorted(data, key=lambda t: (int(t, 16), t))]
    out += ["#ifdef __cplusplus", "}", "#endif", ""]
    text = "\n".join(out)
    if not path.is_file() or path.read_text(encoding="utf-8") != text:
        path.write_text(text, encoding="utf-8")


def match_obj_name(repo: Path, src: Path, suffix=False) -> str:
    """The object file name of `src` in a match build: its repo path without the extension (= objdiff_config.obj_name),
    or with it (`suffix`: for a source whose stem another source of the same build shares, verify.c / verify.cpp)."""
    if src.is_relative_to(repo):
        rel = src.relative_to(repo)
        base = (rel if suffix else rel.with_suffix("")).as_posix()
    else:
        base = src.name if suffix else src.stem
    return re.sub(r"[^A-Za-z0-9]+", "_", base) + ".obj"


def compile_match_obj(ws: Workspace, repo: Path, src: Path, out_dir: Path, header: Path,
                      client: str | None = None, like=None, name: str | None = None) -> tuple[Path | None, str, list]:
    """-> (obj or None, compiler output, flags used). The client's own toolchain (eqmain: VC6 SP5) unless the
    source names another (`// TOOLCHAIN:` line, or the image's folder rule: eqmain's rdp is SP4). `like`: the
    repo source `src` is a copy of (permute's candidates), for the image's default flags and folder rules.
    `name`: the object file name (default match_obj_name)."""
    obj = out_dir / (name or match_obj_name(repo, src))
    flags = cflags_for(src, like=like)
    vc6 = source_vc6(src, ws.vc6(client), like)
    cmd = [str(vc6 / "Bin" / "CL.EXE")] + flags + ["/DEQD_MATCH", f"/FI{header}",
                                                  f"/I{repo / 'include'}", f"/Fo{obj}", str(src)]
    p = subprocess.run(cmd, env=vc6_env(vc6), capture_output=True, text=True, errors="replace",
                       cwd=out_dir)
    return (obj if p.returncode == 0 else None), p.stdout + p.stderr, flags


class MatchContext:
    """What every match-mode comparison against one client needs, loaded once: the original image and its
    function starts, library names, import names, the generated EQD_MATCH header and the CRT aliases.
    Used by compare_match and by permute/permute.py (which scores many candidate objects of one function)."""

    sizes: dict = {}                # original VA -> size override (fixture_sizes); set in __init__

    def __init__(self, repo: Path, client="trilogy", out_dir=None, aliases=None, log=print):
        self.repo, self.client = Path(repo).resolve(), client
        self.ws = Workspace(self.repo)
        self.o_side = Side(load_pe(self.ws.binary(client)))
        self.starts = function_starts(self.ws, client, self.repo)
        self.sizes = fixture_sizes(self.repo, client)
        self.libnames = library_names(self.ws, client)
        self.o_side.funcs = {s: f"F:{s:x}" for s in self.starts}
        self.imp_by_name = {}
        for v in self.o_side.img.imports.values():
            self.imp_by_name.setdefault(v.split("!", 1)[1], v)
        self.out_dir = Path(out_dir or self.ws.build / client / "match").resolve()
        self.out_dir.mkdir(parents=True, exist_ok=True)
        self.aliases = aliases or {}
        fns, data, problems = match_symbols(self.repo)
        for pr in problems:
            log(f"compare --match: {pr}")
        self.header = self.out_dir / "eqd_match_syms.h"
        write_match_header(self.header, fns, data)
        self.log = log

    def externals(self, src: Path, names) -> dict:
        """{decorated name: VA} for the undefined external symbols `names` of `src`'s object that an `// EXTERNAL:`
        marker maps (eqd_unicorn/externs.py): in `src` itself, or in a header under the repo's include/.
        Problems (markers that disagree, an ambiguous overload) are logged once per source."""
        client = getattr(self, "client", None)
        own = externs.scan([src], client) if Path(src).is_file() else []
        inc = Path(getattr(self, "repo", ".")) / "include"
        hdr = externs.header_markers([inc], client) if inc.is_dir() else []
        if not own and not hdr:
            return {}
        out, problems = externs.resolve(names, own, hdr)
        seen = self.__dict__.setdefault("_ext_logged", set())
        for p in problems:
            if (str(src), p) not in seen:
                seen.add((str(src), p))
                getattr(self, "log", print)(f"compare --match: {Path(src).name}: {p}")
        return out

    def compile(self, src: Path, out_dir=None, like=None) -> tuple[Path | None, str, list]:
        """-> (obj or None, compiler output, flags): `src` built with EQD_MATCH into out_dir (`like`: the repo
        source it stands in for, compile_match_obj)."""
        return compile_match_obj(self.ws, self.repo, Path(src), Path(out_dir or self.out_dir), self.header,
                                 self.client, like)

    def compile_kept(self, src: Path) -> tuple[Path | None, str, list]:
        """compile() into out_dir, for an object that must stay valid while other sources are built (compare_match
        builds every source before it compares, cross_tu then builds the defining TUs). A source whose default
        object name another source already took in this context (verify.c and verify.cpp both give ..._verify.obj)
        gets the name with its extension, so no build overwrites an object still in use."""
        src = Path(src).resolve()
        owners = self.__dict__.setdefault("_obj_owner", {})
        name = match_obj_name(self.repo, src)
        n = 1
        while owners.setdefault(name, src) != src:
            n += 1
            name = match_obj_name(self.repo, src, suffix=True)
            if n > 2:
                name = name[:-4] + f"_{n}.obj"
        return compile_match_obj(self.ws, self.repo, src, Path(self.out_dir), self.header, self.client, name=name)

    def cross_tu(self, objs: dict, anns: dict, log=print) -> dict:
        """{decorated name: original VA} for the calls of the objects `objs` ({source: obj}) to functions that
        another TU defines and annotates (xtu.py): from those objects (`anns`: {source: annotations}), and for a
        reference no given source defines, from the client sources whose annotations name it (built here, in
        out_dir). Only these keys replace a call's symbol (compare_obj `xtu`)."""
        import xtu as X
        refs = set()
        maps = []
        for src, obj in objs.items():
            if obj is not None:
                refs |= X.external_refs(obj)
                maps.append(X.annotated_defs(obj, anns.get(src, ()), undecorate))
        known = X.merge(maps)
        missing = {r for r in refs if r not in known}
        if missing:
            if getattr(self, "_ann_all", None) is None:
                self._ann_all = X.scan_sources(clients.sources(self.repo, self.client), self.client.upper())
            index = X.annotation_index(self._ann_all)
            have = {str(Path(s).resolve()) for s in objs}
            for path in sorted(X.defining_sources(missing, index, undecorate)):
                src = Path(path).resolve()
                if str(src) in have:
                    continue
                have.add(str(src))
                obj, text, _f = self.compile_kept(src)
                if obj is None:
                    log(f"compare --match: {src.name} does not build; calls into it keep their symbol")
                    continue
                maps.append(X.annotated_defs(obj, [a for a in self._ann_all
                                                   if Path(a.path).resolve() == src], undecorate))
        return X.merge(maps)

    def compare_obj(self, obj: Path, src: Path, anns: list, by_name: dict, todo=None, rel=None,
                    flags=(), target=None, xtu=None) -> list[Result]:
        """Compare the annotated functions `todo` (default: all `anns`) of the object built from `src`.
        `by_name`: annotated name -> {original VAs}, for keying calls to other rewrites.
        `xtu`: {decorated name: VA} (cross_tu): when given, a call to an EXTERNAL symbol is keyed by the VA only
        when the symbol is in it (the definition's annotation), not by its undecorated name (`by_name`), so an
        unannotated overload of an annotated name keeps its symbol.
        `target`: delinked objects (a folder of `<VA>.obj`, or one .obj): each function is also checked
        against its own (target_check)."""
        import coff
        o_side = self.o_side
        rel = rel or Path(src).name
        parsed = coff.parse(obj)
        # a vftable the source marks: its reference reads the client's address (VC6 defines ??_7 in the object)
        vftables = {s.name for s in parsed.symbols if s and externs.is_vftable(s.name)}
        lk = coff.link(parsed, **self.obj_layout(), client_vas=self.externals(Path(src), vftables) if vftables else None)
        img = Image(Path(obj).name, lk.base, lk.data, lk.base, code_ranges=lk.code_ranges)
        for a_, n_ in lk.symbols.items():
            img.add_symbol(a_, n_)
        r_side = Side(img, relocs=set(lk.relocs), is_rewrite=True, orig=o_side)
        r_side.rel32 = {f for f, n in lk.reloc_target.items() if f not in lk.relocs}
        r_side.eqd_calls = {f for f, n in lk.reloc_target.items() if (coff.eqd_va(n) or ("",))[0] == "fn"}
        for f, n in lk.reloc_target.items():
            e = coff.eqd_va(n)
            if e and e[0] == "fn":
                r_side.funcs[e[1]] = f"F:{e[1]:x}"
        # annotated functions of this object -> entry; other symbols -> keys
        entry_of = {}
        lines = None
        for a in anns:
            hits = [v for v, sym in lk.symbols.items() if v in lk.func_sizes and undecorate(sym) == a.name]
            if len(hits) > 1:
                lines = lines if lines is not None else listing_lines(Path(src), self.ws, self.repo, self.out_dir,
                                                                      self.client)
                after = sorted((lines.get(lk.symbols[v], 0), v) for v in hits if lines.get(lk.symbols[v], 0) > a.line)
                if after:
                    hits = [after[0][1]]
                else:   # no listing lines (vlong_guess's two ctors): the n-th annotation of this name in the
                    same = sorted(x.line for x in anns if x.name == a.name)    # file takes the n-th symbol in
                    if len(same) == len(hits):                                  # object (= definition) order
                        hits = [sorted(hits)[same.index(a.line)]]
            entry_of[a.va] = hits
            if len(hits) == 1:
                r_side.funcs[hits[0]] = f"F:{a.va:x}"
        # functions the source only declares, at a client VA (`// EXTERNAL:` markers, externs.py)
        ext = self.externals(Path(src), set(lk.externs.values()))
        for addr, sym in list(lk.symbols.items()) + list(lk.externs.items()):
            if addr in r_side.funcs or (addr in lk.symbols and addr not in lk.func_sizes):
                continue
            u = undecorate(sym)
            if sym.startswith("__imp_"):
                nm = re.sub(r"@\d+$", "", sym[len("__imp_"):]).lstrip("_")
                if nm in self.imp_by_name:
                    img.imports[addr] = self.imp_by_name[nm]
                continue
            if xtu is not None and addr in lk.externs and sym in xtu:
                r_side.funcs[addr] = f"F:{xtu[sym]:x}"
            elif addr in lk.externs and sym in ext:
                r_side.funcs[addr] = f"F:{ext[sym]:x}"
            elif u in by_name and len(by_name[u]) == 1 and (xtu is None or addr not in lk.externs):
                r_side.funcs[addr] = f"F:{next(iter(by_name[u])):x}"
            elif sym in self.aliases:
                r_side.funcs[addr] = f"F:{self.aliases[sym]:x}"
            elif sym in self.libnames:
                r_side.funcs[addr] = f"F:{self.libnames[sym]:x}"
            elif addr in lk.externs:
                r_side.funcs[addr] = f"F:{sym}"
        r_stops = frozenset(a_ for a_ in lk.symbols if a_ in lk.func_sizes)
        results = []
        for a in (anns if todo is None else todo):
            hits = entry_of[a.va]
            if len(hits) != 1:
                results.append(Result(a.va, a.name, rel, "error",
                                      first_diff=f"{len(hits)} symbols named {a.name} (overloads?)"))
                continue
            try:
                res = compare_function(o_side, r_side, a.va, hits[0], frozenset(self.starts - {a.va}),
                                       r_stops - {hits[0]}, self.sizes.get(a.va))
            except Exception as e:                  # noqa: BLE001
                res = Result(a.va, a.name, rel, "error", first_diff=f"{type(e).__name__}: {e}")
            res.name, res.source = a.name, rel
            res.flags = " ".join(f for f in flags if f not in ("/nologo", "/c"))
            if target is not None and res.verdict != "error":
                tpath = Path(target)
                tpath = tpath / f"{a.va:08X}.obj" if tpath.is_dir() else tpath
                res.target = self.target_check(a.va, tpath, lk, r_side, hits[0], r_stops - {hits[0]})
            results.append(res)
        return results

    def obj_layout(self) -> dict:
        """coff.link keywords: the rewrite object's flat layout must not overlap the original image (eqmain.dll
        sits at coff.OBJ_BASE)."""
        import coff
        img = self.o_side.img
        if img.base < coff.EXTERN_BASE + 0x01000000 and coff.OBJ_BASE < img.end:
            return {"base": coff.ALT_OBJ_BASE, "extern_base": coff.ALT_EXTERN_BASE}
        return {}

    def target_check(self, va: int, tobj: Path, r_lk, r_side: Side, r_entry: int, r_stops) -> dict:
        """The rewrite's object against the delinked object of the original function (ghidra/scripts/
        delink.py) instead of the exe. The delinked object linked at its own VA must give the original's
        bytes (`bytes_ok`: its relocations reproduce the client); the match compare against it gives
        `status`; and the relocation fields inside the instructions must sit at the same offsets from the
        entry on both sides (`missing`: the rewrite has one the delinked object lacks, i.e. Ghidra has no
        reference there; `extra`: the reverse, a client address the rewrite has as a plain number, or a
        number Ghidra took for an address) and resolve to the same client address where the rewrite's
        side names one (`value_differs`). `ok`: all of that, with status `identical`."""
        import coff
        if not tobj.is_file():
            return {"ok": False, "status": "missing"}
        try:
            t = coff.parse(tobj)
            placement = coff.delinked_placement(t)
            t_lk = coff.link(t, placement)
        except (ValueError, OSError, struct.error) as e:
            return {"ok": False, "status": "error", "error": str(e)}
        img = self.o_side.img
        timg = Image(img.name, img.base, bytearray(img.data), img.preferred_base, img.imports, img.symbols,
                     img.by_name, img.code_ranges)
        bad_bytes = []
        for k, at in placement.items():
            n = len(t.sections[k - 1].data)
            new = bytes(t_lk.data[at - t_lk.base:at - t_lk.base + n])
            old = img.read(at, n)
            bad_bytes += [at + i for i in range(n) if new[i] != old[i]]
            timg.data[at - img.base:at - img.base + n] = new
        t_side = Side(timg, funcs=self.o_side.funcs)
        try:
            tres = compare_function(t_side, r_side, va, r_entry, frozenset(self.starts - {va}), r_stops,
                                    self.sizes.get(va))
        except Exception as e:                      # noqa: BLE001
            return {"ok": False, "status": "error", "error": f"{type(e).__name__}: {e}"}
        status = match_status(tres)

        def fields(span, entry, rt):
            inside = {a for v, n in span for a in range(v, v + n)}
            return {f - entry: f for f in rt if f in inside and f + 3 in inside}
        r_f = fields(tres.r_span, r_entry, r_lk.reloc_target)
        t_f = fields(tres.o_span, va, t_lk.reloc_target)
        missing, extra = sorted(set(r_f) - set(t_f)), sorted(set(t_f) - set(r_f))
        value_differs = []
        for off in sorted(set(r_f) & set(t_f)):
            want = self._reloc_value(r_lk, r_f[off], r_side)
            if want is not None and want != self._reloc_value(t_lk, t_f[off]):
                value_differs.append(off)
        out = {"ok": not bad_bytes and status == "identical" and not missing and not extra and not value_differs,
               "status": status, "bytes_ok": not bad_bytes, "relocs": len(t_f)}
        if bad_bytes:
            out["bad_bytes"] = [f"{x:#010x}" for x in bad_bytes[:8]]
        for k, v in (("missing", missing), ("extra", extra), ("value_differs", value_differs)):
            if v:
                out[k] = [f"+{x:#x}" for x in v]
        if status != "identical":
            out["first_diff"] = tres.first_diff
        return out

    @staticmethod
    def _reloc_value(lk, fva, r_side=None):
        """The address a relocation field resolves to after coff.link. For the rewrite (`r_side` given) only
        when that is a client address: an `eqd_*` symbol, or a function the normaliser keys by its original
        VA (another rewrite, a CRT alias); else None (its own data, imports: compared by the normaliser)."""
        import coff
        raw = int.from_bytes(lk.data[fva - lk.base:fva - lk.base + 4], "little")
        val = (fva + 4 + raw) & 0xFFFFFFFF if fva in lk.rel32 else raw
        if r_side is None or coff.eqd_va(lk.reloc_target[fva]):
            return val
        m = re.fullmatch(r"F:([0-9a-f]+)", r_side.funcs.get(val, ""))
        return int(m.group(1), 16) if m else None


def compare_match(repo: Path, sources, client="trilogy", out_dir=None, aliases=None, only_va=None,
                  log=print, target=None) -> list[Result]:
    """Match mode: each source compiled to a COFF object with EQD_MATCH (client calls/globals are external
    symbols, so `call rel32` / absolute operands with relocations), laid out by coff.link with the client's
    symbols at their own VAs, and compared function by function with the original."""
    ctx = MatchContext(repo, client, out_dir, aliases, log)
    sources = [Path(s).resolve() for s in sources]
    anns = {s: ann.scan([s], client.upper()) for s in sources}
    by_name: dict = {}
    for s in sources:
        for a in anns[s]:
            by_name.setdefault(a.name, set()).add(a.va)
    results = []
    built = {}
    for src in sources:
        if any(only_va is None or a.va == only_va for a in anns[src]):
            built[src] = ctx.compile_kept(src)
    xtu = ctx.cross_tu({s: b[0] for s, b in built.items()}, anns, log)
    for src in sources:
        rel = src.relative_to(ctx.repo).as_posix() if src.is_relative_to(ctx.repo) else str(src)
        todo = [a for a in anns[src] if only_va is None or a.va == only_va]
        if not todo:
            continue
        obj, text, flags = built[src]
        if obj is None:
            err = "; ".join(ln.strip() for ln in text.splitlines() if "error" in ln)[:400]
            results += [Result(a.va, a.name, rel, "error", first_diff=f"match build failed: {err}") for a in todo]
            continue
        results += ctx.compare_obj(obj, src, anns[src], by_name, todo, rel, flags, target, xtu=xtu)
    return results


def match_status(r: Result) -> str:
    """identical (bytes outside relocations) | normalised (same instructions after normalisation, bytes
    differ: dummy EDX, literal placement) | diff (same instruction count) | structural | error."""
    if r.verdict == "error":
        return "error"
    if r.verdict.startswith("identical"):
        return "identical" if r.bytes_identical else "normalised"
    return "structural" if r.structural else "diff"


def target_summary(results) -> str:
    """--target-obj counts, over the byte-identical functions (the check's subject) and over all."""
    ident = [r for r in results if r.target is not None and match_status(r) == "identical"]
    tc = {}
    for r in ident:
        k = "pass" if r.target["ok"] else ("missing" if r.target["status"] == "missing" else "fail")
        tc[k] = tc.get(k, 0) + 1
    why = {}
    for r in ident:
        if not r.target["ok"] and r.target["status"] != "missing":
            for k in ("bytes_ok", "missing", "extra", "value_differs", "status"):
                if (k == "bytes_ok" and not r.target["bytes_ok"]) or (k in ("missing", "extra", "value_differs")
                                                                       and r.target.get(k)) \
                        or (k == "status" and r.target["status"] != "identical"):
                    why[k] = why.get(k, 0) + 1
    others = sum(1 for r in results if r.target is not None and r.target.get("ok")
                 and match_status(r) != "identical")
    return (f"target-obj check on the {len(ident)} byte-identical functions: {tc.get('pass', 0)} pass, "
            f"{tc.get('fail', 0)} fail, {tc.get('missing', 0)} without an object"
            + (f" (failures: {', '.join(f'{k} {v}' for k, v in sorted(why.items()))})" if why else "")
            + (f"; {others} more pass that are not identical against the exe" if others else ""))


def default_sources(repo: Path, client: str = "trilogy"):
    """The client's own sources: src/**/*.c/.cpp without other images' sub-folders (trilogy skips src/eqmain/;
    eqmain takes only src/eqmain/)."""
    return clients.sources(repo, client)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("sources", nargs="*", help="source files (default: every src\\**\\*.c/.cpp of the repo)")
    ap.add_argument("--repo", default=".", help="decomp repo (default: the current directory)")
    ap.add_argument("--client", default="trilogy")
    ap.add_argument("--va", help="only this function (original VA)")
    ap.add_argument("--aliases", help="fixture-defaults JSON with rewrite_aliases and link (default: the "
                                      "client's, tests/unicorn/crt_defaults.json / crt_defaults_eqmain.json)")
    ap.add_argument("--out", help="build folder for the DLLs (default <build>\\<client>\\compare)")
    ap.add_argument("--json", help="write all results to this file")
    ap.add_argument("-v", "--verbose", action="store_true", help="print the diff of each differing function")
    ap.add_argument("--show", action="store_true", help="print both normalised listings")
    ap.add_argument("--size", action="append", default=[], metavar="VA=BYTES",
                    help="limit the original function at VA to BYTES (like a fixture's `size`; for an extent "
                         "that can't be decoded). Repeatable; fixtures' `size` is used automatically")
    ap.add_argument("--match", action="store_true", help="match mode: COFF objects built with EQD_MATCH "
                                                         "(default out <build>/<client>/match)")
    ap.add_argument("--nearmiss", help="match mode: write functions with 1..5 differing instructions here")
    ap.add_argument("--target-obj", help="match mode: also check each function against its delinked object "
                                         "(a folder of <VA>.obj from ghidra/scripts/delink.py, or one .obj)")
    a = ap.parse_args(argv)
    for spec in a.size:
        va_, _, n_ = spec.partition("=")
        EXTRA_SIZES[int(va_, 16)] = int(n_, 0)
    repo = Path(a.repo).resolve()
    sources = [Path(s).resolve() for s in a.sources] or default_sources(repo, a.client)
    aliases, link = load_defaults(a.aliases or repo / clients.get(a.client).crt_defaults, repo)
    only = int(a.va, 16) if a.va else None
    if a.match:
        results = compare_match(repo, sources, a.client, a.out, aliases, only, target=a.target_obj)
    else:
        results = compare_sources(repo, sources, a.client, a.out, aliases, only, link=link)
    counts: dict = {}
    for r in results:
        counts[r.verdict] = counts.get(r.verdict, 0) + 1
        extra = []
        if r.folds:
            extra.append(f"{r.folds} folded")
        if r.data:
            extra.append(f"{r.data} data")
        tag = f" ({', '.join(extra)})" if extra else ""
        line = f"0x{r.va:08X}  {r.verdict:<20} {r.ratio:6.3f}  {r.orig_count:4d}/{r.rew_count:<4d} {r.name}{tag}"
        if r.verdict in ("differs", "error"):
            line += f"\n            {r.first_diff}"
        print(line)
        if r.target is not None and not r.target.get("ok"):
            print("            target: " + ", ".join(f"{k} {v}" for k, v in r.target.items() if k != "ok"))
        if a.verbose and r.diff:
            print("\n".join("            " + d for d in r.diff))
        if a.show:
            print("    original:\n" + "\n".join("      " + x for x in r.orig_listing))
            print("    rewrite:\n" + "\n".join("      " + x for x in r.rew_listing))
    total = len(results)
    same = counts.get("identical", 0) + counts.get("identical_normalised", 0)
    print(f"\n{same}/{total} instruction-identical ({counts.get('identical', 0)} as is, "
          f"{counts.get('identical_normalised', 0)} after folding), {counts.get('differs', 0)} differ, "
          f"{counts.get('error', 0)} errors")
    if a.match:
        mc: dict = {}
        for r in results:
            mc[match_status(r)] = mc.get(match_status(r), 0) + 1
        near = [r for r in results if match_status(r) == "diff" and 1 <= r.ndiff <= 5]
        print(f"match: {mc}; near misses (1-5 instructions differ, same count): {len(near)}")
        if a.target_obj:
            print(target_summary(results))
        if a.nearmiss:
            lines = [f"0x{r.va:08X}\t{r.ndiff}\t{r.name}\t{r.source}\t{r.flags}\t{r.first_diff}"
                     for r in sorted(near, key=lambda r: (r.ndiff, r.va))]
            Path(a.nearmiss).parent.mkdir(parents=True, exist_ok=True)
            Path(a.nearmiss).write_text("# va\tndiff\tname\tsource\tflags\tfirst difference\n"
                                        + "\n".join(lines) + "\n", encoding="utf-8")
    if a.json:
        for r in results:
            r.match = match_status(r) if a.match else None
        Path(a.json).write_text(json.dumps({"counts": counts, "results": [
            dict(r.as_json(), **({"match": r.match} if a.match else {})) for r in results]},
            indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
