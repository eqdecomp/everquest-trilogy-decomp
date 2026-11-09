r"""Context packer: one markdown bundle per function, for whoever rewrites it.

    .venv\Scripts\python eq-trilogy\tools\context\pack_context.py trilogy 004C8FCE [ADDR ...]
    ... trilogy --subsystem EverQuest/EverQuest [--limit N]
    ... trilogy --next 5          (needs the queue tool; optional)
    ... --quirks nearmiss|all|off  --stack-hints F|off   (VC6 matching notes; see context\README.md)

Reads build\export\<client>\ (export\export_client.py) and eq-<client>\include\<client>\*.h.
Writes build\context\<client>\<ADDR>_<name>.md. Bundles hold decompiler output and bytes of the
client: they live outside every repo and are never committed. See context\README.md.
"""
import argparse
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
TOOLS = HERE.parent
sys.path.insert(0, str(HERE.parent / "ghidra" / "scripts"))
import eqd_ghidra as eqd   # noqa: E402  (no Java needed at import)

MAX_PSEUDO_LINES = 400
MAX_ASM_INSNS = 700
MAX_CALLERS = 40
MAX_CALLEES = 60
MAX_STRING = 200
MAX_GLOBALS = 40
MAX_STRUCT_LINES = 120
MAX_STRUCTS = 6
MAX_PRIOR = 3
FULL_PSEUDO, FULL_ASM = "FULL_PSEUDO.c", "FULL_ASM.txt"     # build_bundle(full_dir=...): untruncated listings
EXTRA_ASM = "EXTRA_ASM.txt"      # ... and the extent code the Ghidra listing lacks, when long

RULES = """\
- Faithful functional: same behavior in every detail, **bugs included**. Binary beats any reference
  (EQClassic/Mac/TAKP names and layouts are evidence, never code to paste: they are GPL/unlicensed).
- Annotate every function with its address (and original file when known), exactly:
  ```cpp
  // FUNCTION: {CLIENT} 0x{addr8}
  // ORIGINAL FILE: <file> (from Mac debug map)      <- only if the bundle gives a source file
  ```
- Compiler: {COMPILER}. No features it lacks, no STL where the original had none, mind the old `for`
  scope rule. Keep code shape close to the original: branch order, early returns, loop forms.
- Match the calling convention in the header (`__thiscall` = `this` in ECX; `__cdecl`/`__stdcall` as in
  the binary).
- Names: a function keeps the name it is given (`_guess` until confirmed); never rename a human-confirmed
  (`manual`/`certain`) name. Parameters and locals get plain descriptive names, no `_guess`.
- Struct/class changes go in `include/` only when the task allows header edits (offsets in comments,
  `C_ASSERT` size checks; flag a conflict with an existing field). Otherwise use a file-local `<Name>_guess`
  struct at the top of the unit file.
- Any behavior fix goes under `#ifdef EQD_FIXES` (off by default). Never edit the compiler, tests,
  harness or comparison tools to force a pass; flag the function instead.
- Skip library code (CRT, DirectX, zlib, Miles, Smacker). Write in `src/<subsystem>/`; add a Unicorn
  test in `tests/unicorn/`.
- After repeated failed attempts, stop and record what blocks the function."""

COMPILERS = {
    "trilogy": "Visual C++ 6.0 SP4 (c2 12.0.8799, LIBCMT 8797), `/O1 /MT`; no Processor Pack",
}


def registry():
    """unicorn/eqd_unicorn/clients.py (the image registry), via eqd_ghidra's loader."""
    return eqd.registry()


def source_of(repo, subsystem):
    """(repo-relative source of a queue subsystem, its text): src/<subsystem>.c or .cpp when it exists, else
    (src/<subsystem>.cpp, ""); (None, "") without a subsystem."""
    if not subsystem:
        return None, ""
    for ext in (".c", ".cpp"):
        p = Path(repo) / "src" / (subsystem + ext)
        if p.is_file():
            return f"src/{subsystem}{ext}", p.read_text(encoding="utf-8", errors="replace")
    return f"src/{subsystem}.cpp", ""


def compiler_of(client, src_rel=None, text=""):
    """The image's compiler, or the one its source `src_rel` builds with when that differs (a `// TOOLCHAIN:`
    line in `text`, else the image's folder rule: eqmain's rdp objects are SP4)."""
    reg = registry()
    c = reg.get(client)
    detail = c.compiler_detail or COMPILERS.get(client, "the client's original compiler")
    key = reg.source_toolchain(src_rel, text) if src_rel else None
    if key and key != c.toolchain:
        return (f"{reg.toolchain_name(key)} for this function's file ({src_rel}: toolchain `{key}`); the rest of "
                f"the image: {detail}")
    return detail


def repo_of(ws, client):
    """The client's decomp repo (eq-trilogy for trilogy and eqmain; EQD_REPO overrides it)."""
    return registry().repo_dir(client, ws.root)


IMAGE_NOTE = """- This function is in **{program}** ({title}), image base 0x{base:08X}, not eqgame.exe: its rewrite goes
  under `src/{prefix}/`, its fixture under `tests/unicorn/{prefix}/`, annotated `// FUNCTION: {token} 0x...`.
  The structs in `include/trilogy/` are shared with eqgame.exe; addresses (globals, functions) are this image's
  own: put the address macros for them in the source file, never in `include/trilogy/`."""

HEX8 = re.compile(r"^[0-9A-Fa-f]{8}$")
ABS_REF = re.compile(r"\[0x([0-9a-fA-F]{1,8})\]")
ECX_REGS = ("ECX", "CX", "CL", "CH")
ECX_WINDOW = 16          # instructions looked at before a call site (stops at the previous call/jmp/ret)
EAX_REGS = ("EAX", "AX", "AL", "AH")
EAX_WINDOW = 12          # instructions looked at after a call site (stops at a branch/call/ret)
EAX_RET_WINDOW = 16      # instructions looked at before each RET of the function itself
MAX_SWITCH_CASES = 1024


# ---------------------------------------------------------------- data access

class Export:
    def __init__(self, client, out=None):
        self.client = client
        ws = eqd.Workspace()
        self.ws = ws
        self.dir = Path(out) if out else ws.build / "export" / client
        if not (self.dir / "manifest.json").exists():
            sys.exit(f"no export at {self.dir}; run export\\export_client.py {client}")
        self.manifest = self._load("manifest.json")
        self.functions = {f["address"]: f for f in self._load("functions.json")["functions"]}
        self.order = list(self.functions)
        self.callgraph = self._load("callgraph.json")["functions"]
        self._strings = self._globals = None
        self._shards = {}

    def _load(self, name):
        return json.loads((self.dir / name).read_text(encoding="utf-8"))

    @property
    def strings(self):
        if self._strings is None:
            self._strings = {s["address"]: s for s in self._load("strings.json")["strings"]}
        return self._strings

    @property
    def globals(self):
        if self._globals is None:
            self._globals = {g["address"]: g for g in self._load("globals.json")["globals"]}
        return self._globals

    def _shard(self, kind, name):
        key = (kind, name)
        if key not in self._shards:
            d = {}
            p = self.dir / kind / name
            if p.exists():
                for line in p.read_text(encoding="utf-8").splitlines():
                    if line.strip():
                        r = json.loads(line)
                        d[r["address"]] = r          # last line wins
            self._shards[key] = d
        return self._shards[key]

    def asm(self, addr):
        return self._shard("asm", self.functions[addr]["shard"]).get(addr)

    def pseudo(self, addr):
        return self._shard("pseudocode", self.functions[addr]["shard"]).get(addr)

    # ---- GetProcAddress slots (eqgfx_dx7.dll entry points and a few others)

    @property
    def gpa_slots(self):
        """{slot VA: (export name, loader address)}: globals filled with GetProcAddress(h, "name").
        Derived from the export at pack time: every function that reads the GetProcAddress IAT entry is
        scanned for `PUSH <string>` ... `CALL <GetProcAddress>` ... `MOV [slot],EAX` (the call through
        the IAT entry itself or through a register loaded from it). Names come from strings.json, so
        they are complete even where Ghidra's labels are truncated."""
        if getattr(self, "_gpa", None) is None:
            self._gpa = gpa_slot_map(self)
        return self._gpa

    # ---- raw bytes of the binary (switch tables; the export has no data bytes)

    @property
    def pe(self):
        if getattr(self, "_pe", None) is None:
            self._pe = PEBytes(self.ws, self.client, self.manifest)
        return self._pe


class PEBytes:
    """Read-only view of the client binary for table bytes. `ok` is False when it is missing or its
    hash differs from the export's."""

    def __init__(self, ws, client, manifest):
        self.ok, self.why, self.data, self.sections, self.base = False, None, b"", [], 0
        try:
            p = ws.binary(client)
        except Exception as e:      # unknown client
            self.why = f"no binary for {client}: {e}"
            return
        if not p.exists():
            self.why = f"binary not found: {p}"
            return
        import hashlib
        import pefile
        data = p.read_bytes()
        want = manifest.get("binary_sha256")
        if want and hashlib.sha256(data).hexdigest() != want:
            self.why = f"{p.name} differs from the exported binary (sha256)"
            return
        pe = pefile.PE(data=data, fast_load=True)
        self.base = pe.OPTIONAL_HEADER.ImageBase
        self.sections = [(self.base + s.VirtualAddress, max(s.Misc_VirtualSize, s.SizeOfRawData),
                          s.PointerToRawData, s.SizeOfRawData) for s in pe.sections]
        self.data, self.ok = data, True

    def read(self, va, n):
        """n bytes at va, or None if not fully backed by file data."""
        for start, vsize, raw, rsize in self.sections:
            if start <= va and va + n <= start + min(vsize, rsize):
                off = raw + va - start
                return self.data[off:off + n]
        return None

    def dwords(self, va, n):
        b = self.read(va, 4 * n)
        return None if b is None else [int.from_bytes(b[i:i + 4], "little") for i in range(0, 4 * n, 4)]


def norm_addr(a):
    a = a.strip()
    if a.lower().startswith("0x"):
        a = a[2:]
    a = a.upper().rjust(8, "0")
    if not HEX8.match(a):
        sys.exit(f"bad address: {a}")
    return a


def safe_name(name):
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", name)[:80]


# ---------------------------------------------------------------- queue (optional, late-bound)

class Queue:
    """Best-effort reader of eq-<client>\\queue\\ entries: states and prior attempts.
    Accepts *.jsonl / *.json holding objects (or lists/dicts of objects) with an `address` field.
    Any problem means 'no queue data'; the bundle just omits the sections."""

    def __init__(self, repo):
        self.entries = {}
        d = repo / "queue"
        if not d.is_dir():
            return
        try:
            for p in sorted(d.rglob("*")):
                if p.suffix == ".jsonl":
                    for line in p.read_text(encoding="utf-8").splitlines():
                        if line.strip():
                            self._add(json.loads(line))
                elif p.suffix == ".json":
                    self._add(json.loads(p.read_text(encoding="utf-8")))
        except Exception:
            self.entries = {}

    def _add(self, o):
        if isinstance(o, list):
            for x in o:
                self._add(x)
        elif isinstance(o, dict):
            if isinstance(o.get("address"), str) and HEX8.match(o["address"]):
                self.entries[o["address"].upper()] = o
            else:
                for k in ("entries", "functions", "items"):
                    if k in o:
                        self._add(list(o[k].values()) if isinstance(o[k], dict) else o[k])

    def subsystem(self, addr):
        """(subsystem, how) from the queue entry, or (None, None)."""
        e = self.entries.get(addr)
        return (e.get("subsystem"), e.get("subsystem_how")) if e else (None, None)

    def state(self, addr):
        e = self.entries.get(addr)
        return (e.get("state") or e.get("status")) if e else None

    def attempts(self, addr):
        e = self.entries.get(addr)
        return (e.get("attempts") or e.get("history") or []) if e else []


# ---------------------------------------------------------------- headers

class Headers:
    """Index of top-level struct/class/union/enum definitions in eq-<client>\\include\\<client>\\*.h."""

    DEF = re.compile(r"^(typedef\s+)?(struct|class|union|enum)\s+(\w+)\b")

    def __init__(self, repo, client):
        self.defs = {}      # name -> (file, lines)
        dirs = [repo / "include" / client]
        try:                # a secondary image (eqmain) also sees the repo's primary headers (include/trilogy)
            prim = [c.id for c in registry().of_repo(registry().get(client).repo) if c.primary]
            dirs += [repo / "include" / p for p in prim if p != client]
        except Exception:   # noqa: BLE001
            pass
        for p in sorted(x for inc in dirs if inc.is_dir() for x in inc.glob("*.h")):
            lines = p.read_text(encoding="utf-8", errors="replace").splitlines()
            i = 0
            while i < len(lines):
                m = self.DEF.match(lines[i])
                if m and ("{" in lines[i] or (i + 1 < len(lines) and lines[i + 1].lstrip().startswith("{"))):
                    start, depth, j, seen = i, 0, i, False
                    while j < len(lines):
                        depth += lines[j].count("{") - lines[j].count("}")
                        seen = seen or "{" in lines[j]
                        if seen and depth <= 0:
                            break
                        j += 1
                    end = j
                    k = end + 1
                    while k < len(lines) and lines[k].strip().startswith("C_ASSERT") and k <= end + 2:
                        end = k
                        k += 1
                    c = start                       # include the leading block comment
                    while c > 0 and (lines[c - 1].strip().startswith(("*", "/*", "//")) or
                                     lines[c - 1].strip().endswith("*/")):
                        c -= 1
                    self.defs.setdefault(m.group(3), (p.name, lines[c:end + 1]))
                    i = end + 1
                else:
                    i += 1

    def pick(self, names, offsets):
        """Return [(name, file, text, note)] for the structs named, bounded."""
        out = []
        for n in names:
            if n not in self.defs or any(o[0] == n for o in out):
                continue
            f, lines = self.defs[n]
            note = ""
            if len(lines) > MAX_STRUCT_LINES:
                lines, note = self._filter(lines, offsets)
            out.append((n, f, "\n".join(lines), note))
            if len(out) >= MAX_STRUCTS:
                break
        return out

    @staticmethod
    def _filter(lines, offsets):
        """Keep the header comment (first 12 lines), the closing lines and any field line whose
        `0xNN` offset comment is one the asm uses."""
        keep = set(range(min(12, len(lines)))) | {len(lines) - 1, len(lines) - 2}
        for i, l in enumerate(lines):
            m = re.search(r"/[*/]\s*(0x[0-9A-Fa-f]+)", l)
            if m and int(m.group(1), 16) in offsets:
                keep.add(i)
        res, last = [], -1
        for i in sorted(keep):
            if i != last + 1:
                res.append("    /* ... */")
            res.append(lines[i])
            last = i
        return res[:MAX_STRUCT_LINES], (f"truncated: {len(lines)} lines; showing the header comment and "
                                        "fields at offsets the function uses")


# ---------------------------------------------------------------- helpers

def plate_comment(c):
    """First /* ... */ block of the decompiler output (name-list evidence, ORIGINAL FILE)."""
    if not c:
        return None
    for m in re.finditer(r"/\*(.*?)\*/", c.replace("\r", ""), re.S):
        t = " ".join(m.group(1).split())
        if not t.startswith("WARNING"):
            return t
    return None


def pseudo_lines(c):
    return [l.rstrip() for l in c.replace("\r", "").strip("\n").split("\n")]


def disp_offsets(asm):
    """Displacements used with a register base: [EAX + 0x40] -> 0x40."""
    offs = set()
    for ins in asm["instructions"]:
        for m in re.finditer(r"\[(?:[A-Z]{3}(?:\s*[+*]\s*\w+)*)\s*\+\s*(0x[0-9a-fA-F]+)\]", ins[2]):
            v = int(m.group(1), 16)
            if 0 < v < 0x10000:
                offs.add(v)
    return offs


def imm(s):
    """Ghidra immediate ('0x10', '-0x9', '0xffffff2e', '5') -> signed 32-bit int, or None."""
    s = s.strip()
    try:
        v = int(s, 0) if not s.startswith("-") else -int(s[1:], 0)
    except ValueError:
        return None
    v &= 0xFFFFFFFF
    return v - (1 << 32) if v & 0x80000000 else v


def a8(v):
    return f"{v & 0xFFFFFFFF:08X}"


def gpa_slot_map(ex):
    """See Export.gpa_slots."""
    iat = {a for a, g in ex.globals.items() if re.match(r"PTR_GetProcAddress_", g.get("name") or "")}
    loaders = sorted({x["function"] for a in iat for x in ex.globals[a]["xrefs"]
                      if x.get("function") in ex.functions})
    iat_ops = {f"dword ptr [0x{a.lower()}]" for a in iat}
    out = {}
    for la in loaders:
        asm = ex.asm(la)
        if not asm:
            continue
        gpa_regs, name, called = set(), None, False
        for _, _, t, _ in asm["instructions"]:
            mn, _, ops = t.partition(" ")
            if mn == "MOV" and ops.split(",", 1)[-1] in iat_ops:
                gpa_regs.add(ops.split(",")[0])
                continue
            if mn == "PUSH":
                s = ex.strings.get(a8(imm(ops) or 0)) if imm(ops) is not None else None
                if s:
                    name, called = s["value"], False
                continue
            if mn == "CALL":
                called = name is not None and (ops in iat_ops or ops in gpa_regs)
                continue
            m = re.match(r"MOV \[0x([0-9a-f]+)\],EAX$", t)
            if m and name and called:
                out.setdefault(a8(int(m.group(1), 16)), (name, la))
                name = None
            elif mn == "MOV" and ops.split(",")[0] in gpa_regs:
                gpa_regs.discard(ops.split(",")[0])
    return out


def gfx_header_names(repo, client):
    """{slot VA: macro name} from include/<client>/eqgfx.h: `#define name EQ_GLOBAL(type, 0xADDR)`
    (or the older `EQGFX_FN(0xADDR, type)`)."""
    p = repo / "include" / client / "eqgfx.h"
    if not p.exists():
        return {}
    t = p.read_text(encoding="utf-8", errors="replace")
    pat = (r"#define[ \t]+(\w+)[ \t]*(?:\\\r?\n)?[ \t]*"
           r"(?:EQGFX_FN\(\s*0x([0-9A-Fa-f]+)|EQ_GLOBAL\(\s*\w+\s*,\s*0x([0-9A-Fa-f]+))")
    return {a8(int(a1 or a2, 16)): n for n, a1, a2 in re.findall(pat, t)}


def gpa_label(ex, gfxh, slot, short=False):
    """Label for a GetProcAddress slot (export name, loader, eqgfx.h macro), or None."""
    if slot not in ex.gpa_slots:
        return None
    name, loader = ex.gpa_slots[slot]
    if short:
        return f"{name} (GetProcAddress slot)"
    lf = ex.functions.get(loader)
    s = f"GetProcAddress \"{name}\" (loader {loader}" + (f" {lf['name']}" if lf else "") + ")"
    h = gfxh.get(slot)
    if h:
        s += f", eqgfx.h `{h}`" if h == name else f", eqgfx.h names it `{h}`"
    return s


def writes_reg(t, regs):
    """True if instruction text t writes one of regs as its first operand (not CMP/TEST/PUSH)."""
    mn, _, ops = t.partition(" ")
    if mn in ("CMP", "TEST", "PUSH", "BT") or mn.startswith(("J", "F")):
        return False
    dst = ops.split(",")[0].strip()
    if mn == "XCHG":
        return any(o.strip() in regs for o in ops.split(","))
    return dst in regs


def reg_mentioned(t, regs):
    return any(re.search(rf"\b{r}\b", t) for r in regs)


def ecx_walk(ins, idx, budget=ECX_WINDOW, hops=2, pushed=False):
    """How ECX was last written before ins[idx], walking back up to `budget` instructions. A CALL or RET
    ends the walk (ECX clobbered / not this path). A JMP means the code at idx is only reached by jumps:
    the walk continues at every jump to that block (up to `hops` times) and all must agree."""
    for j in range(idx - 1, max(-1, idx - 1 - budget), -1):
        a, _, t, _ = ins[j]
        mn = t.split(" ")[0]
        if mn in ("CALL", "RET"):
            return "none"
        if mn == "JMP":
            tgt = f"0x{ins[j + 1][0].lower()}"
            preds = [k for k, x in enumerate(ins) if x[2].startswith("J") and x[2].endswith(" " + tgt)]
            if not hops or not preds:
                return "none"
            res = [ecx_walk(ins, k, budget - (idx - j), hops - 1, pushed) for k in preds]
            return res[0] if all(r.split(":")[0] == res[0].split(":")[0] for r in res) else "none"
        if t in ("PUSH ECX", "PUSH CX"):
            pushed = True
            continue
        if writes_reg(t, ECX_REGS):          # the last write before the call decides
            return "pop" if mn == "POP" else f"{'arg' if pushed else 'set'}:{a}:{t}"
    return "none"


def ecx_at_call_sites(ex, callers):
    """[(site, how)] per direct call site; how = 'set:<addr>:<insn>' (mov/lea/xor/... to ECX after the
    previous call), 'arg:<addr>:<insn>' (ECX loaded, then pushed: an argument, not `this`), 'pop' (the
    last write is a POP ECX, argument cleanup), 'none', or 'tail'/'?' (tail jump / no asm)."""
    res = []
    for c in callers:
        if c.get("kind") != "call":
            res.append((c["site"], "tail"))
            continue
        asm = ex.asm(c["function"]) if c["function"] in ex.functions else None
        ins = asm["instructions"] if asm else []
        idx = next((i for i, x in enumerate(ins) if x[0] == c["site"]), None)
        res.append((c["site"], "?" if idx is None else ecx_walk(ins, idx)))
    return res


def ecx_summary(ex, f, cg, asm):
    """One line of thiscall-vs-static evidence: ECX loads before the callers' calls, and whether the
    function itself reads ECX before writing it (linear scan, a heuristic)."""
    first = None
    for a, _, t, _ in (asm["instructions"] if asm else []):
        if t == "PUSH ECX":          # VC6 reserves a stack slot this way; not a use of ECX
            continue
        if reg_mentioned(t, ECX_REGS):
            mn, _, ops = t.partition(" ")
            src = ops.split(",", 1)[1] if "," in ops else ""
            pure_write = writes_reg(t, ECX_REGS) and mn in ("MOV", "LEA", "MOVZX", "MOVSX", "POP")                 and not reg_mentioned(src, ECX_REGS)
            pure_write = pure_write or (mn == "XOR" and ops.replace(" ", "") in ("ECX,ECX",))
            first = ("writes ECX first", a, t) if pure_write else ("reads ECX before writing it", a, t)
            break
    reads = bool(first) and first[0].startswith("reads")
    sites = ecx_at_call_sites(ex, cg["callers"])
    direct = [s for s in sites if s[1] not in ("tail", "?")]
    nset = sum(1 for s in direct if s[1].startswith("set"))
    parts = []
    if direct:
        if nset == len(direct):
            pops = {imm(t[4:]) for _, _, t, _ in (asm["instructions"] if asm else []) if t.startswith("RET ")}
            if reads or not asm:
                verdict = "likely `__thiscall` (or `__fastcall`)"
            elif pops - {None, 0}:       # callee pops its stack args: __thiscall (or __stdcall), not __cdecl
                verdict = ("but the function doesn't read ECX and pops its own args (`RET`): likely a "
                           "`__thiscall` method that ignores `this`")
            else:
                verdict = ("but the function doesn't read ECX: a method that ignores `this`, or static; "
                           "check whether callers pop the args")
        elif nset == 0:
            verdict = "likely static / `__cdecl` / `__stdcall`"
        else:
            verdict = "mixed: check the sites"
        parts.append(f"ECX set before the call at {nset}/{len(direct)} direct call sites -> {verdict}")
        pops = sum(1 for s in direct if s[1] == "pop")
        if pops:
            parts.append(f"{pops} site(s) only `POP ECX` (argument cleanup)")
        args = sum(1 for s in direct if s[1].startswith("arg"))
        if args:
            parts.append(f"{args} site(s) load ECX only to push it (an argument)")
    else:
        parts.append("no direct call sites to check")
    other = len(sites) - len(direct)
    if other:
        parts.append(f"{other} tail-jump/unknown site(s) not checked")
    if asm:
        parts.append(f"function {first[0]} (`{first[2]}` at `{first[1]}`, linear scan)" if first else
                     "function never touches ECX")
    return "; ".join(parts), dict(sites)


# ---------------------------------------------------------------- EAX evidence (return value or void)

_FPU_NO_ST0_USE = {"FLD", "FILD", "FLDZ", "FLD1", "FLDPI", "FLDL2E", "FLDL2T", "FLDLG2", "FLDLN2", "FLDCW",
                   "FNSTCW", "FSTCW", "FWAIT", "WAIT", "FNINIT", "FINIT", "FNCLEX", "FCLEX", "FFREE", "FBLD"}
_EAX_IMPLICIT_READ = {"CDQ": "EAX", "CWDE": "AX", "CBW": "AL", "CWD": "AX", "STOSD": "EAX", "STOSW": "AX",
                      "STOSB": "AL", "SCASD": "EAX", "SCASW": "AX", "SCASB": "AL", "XLAT": "AL", "SAHF": "AH",
                      "DIV": "EAX", "IDIV": "EAX", "MUL": "EAX", "AAA": "AL", "DAA": "AL", "DAS": "AL", "AAS": "AL"}
_EAX_IMPLICIT_WRITE = {"LODSD", "LODSW", "LODSB", "LAHF", "RDTSC", "CPUID"}
_PURE_WRITE = ("MOV", "LEA", "MOVZX", "MOVSX", "POP", "IN")


def _eax_mentioned(s):
    """The widest EAX-family register named in s ('EAX', 'AX', 'AL', 'AH') or None."""
    for r in EAX_REGS:
        if re.search(rf"\b{r}\b", s):
            return r
    return None


def eax_effect(t):
    """What instruction text t does to EAX: ('read', reg) - uses the value (reg = widest part read),
    ('write', reg) - replaces it without reading it (a CALL clobbers EAX: ('write', 'CALL')),
    ('rmw', reg) - reads and writes it (ADD EAX,..; INC; NEG; SHR ...), or ('none', None)."""
    mn, _, ops = t.partition(" ")
    if mn.startswith("REP"):                         # REP STOSD / REPNE SCASB
        mn, _, ops = ops.partition(" ")
    if mn in _EAX_IMPLICIT_READ and (mn not in ("DIV", "IDIV", "MUL") or "," not in ops):
        # one-operand MUL/DIV read EAX (AL for a byte operand) implicitly
        return "read", ("AL" if mn in ("DIV", "IDIV", "MUL") and "byte ptr" in ops else _EAX_IMPLICIT_READ[mn])
    if mn in _EAX_IMPLICIT_WRITE:
        return "write", "EAX"
    if mn in ("CALL",):
        r = _eax_mentioned(ops)
        return ("read", r) if r else ("write", "CALL")
    if mn in ("FNSTSW", "FSTSW") and ops.strip() == "AX":
        return "write", "AX"
    r = _eax_mentioned(ops)
    if not r:
        return "none", None
    parts = [o.strip() for o in ops.split(",")]
    dst, src = parts[0], ",".join(parts[1:])
    if mn in ("XOR", "SUB", "SBB") and len(parts) == 2 and parts[0] == parts[1] and parts[0] in EAX_REGS:
        return "write", parts[0]                     # XOR EAX,EAX / SBB EAX,EAX: no dependence on the value
    if mn.startswith("SET") and dst in EAX_REGS:
        return "write", dst
    if mn == "OR" and dst == "EAX" and imm(src) == -1:
        return "write", "EAX"                        # OR EAX,0xffffffff = MOV EAX,-1
    if dst in EAX_REGS and mn in _PURE_WRITE and not _eax_mentioned(src):
        return "write", dst
    if mn == "AND" and dst == "EAX" and imm(src) in (0xFF, 0xFFFF):
        return "read", "AL" if imm(src) == 0xFF else "AX"
    if mn in ("CMP", "TEST", "PUSH", "XCHG", "BT", "OUT") or mn.startswith(("J", "F")) or dst not in EAX_REGS:
        return "read", r
    if mn in _PURE_WRITE:                            # MOVZX EAX,AL / MOV EAX,[EAX + 4]: reads the source part
        return "rmw", _eax_mentioned(src)
    return "rmw", r


def eax_after_call(ins, idx, budget=EAX_WINDOW, jumps=1):
    """What the caller does with EAX after the CALL at ins[idx], walking forward over straight-line code:
    'read:<reg>:<addr>:<insn>' (the value is used: reg is EAX/AX/AL/AH), 'over:<addr>:<insn>' (EAX
    written before any read), 'fpu:<addr>:<insn>' (the first x87 instruction consumes ST0: a float/double
    return), 'ret:<addr>' (the caller returns: EAX passes through), 'branch:<addr>' (a conditional or
    indirect jump comes first) or 'none' (window exhausted). An unconditional JMP is followed `jumps` times."""
    i, n, fpu = idx + 1, 0, False
    while i < len(ins) and n < budget:
        a, _, t, _ = ins[i]
        mn, _, ops = t.partition(" ")
        n += 1
        if mn.startswith("RET"):
            return f"ret:{a}"
        if mn == "JMP":
            m = re.fullmatch(r"0x([0-9a-fA-F]+)", ops.strip())
            k = next((k for k, x in enumerate(ins) if m and int(x[0], 16) == int(m.group(1), 16)), None) \
                if jumps else None
            if k is None:
                return f"branch:{a}"
            i, jumps = k, jumps - 1
            continue
        if mn.startswith("J") or mn.startswith("LOOP"):
            return f"branch:{a}"
        if mn.startswith("F") and not fpu:
            if mn in _FPU_NO_ST0_USE:
                fpu = True                               # ST0 pushed down, the return isn't consumed here
            elif mn not in ("FNSTSW", "FSTSW"):
                return f"fpu:{a}:{t}"
        kind, reg = eax_effect(t)
        if kind in ("read", "rmw"):
            return f"read:{reg}:{a}:{t}"
        if kind == "write" and reg != "AH":
            return f"over:{a}:{t}"
        i += 1
    return "none"


def eax_at_call_sites(ex, callers):
    """[(site, how)] per caller site; how as eax_after_call, or 'tail' (a jump, not a call) / '?' (no asm)."""
    res = []
    for c in callers:
        if c.get("kind") != "call":
            res.append((c["site"], "tail"))
            continue
        asm = ex.asm(c["function"]) if c["function"] in ex.functions else None
        ins = asm["instructions"] if asm else []
        idx = next((i for i, x in enumerate(ins) if x[0] == c["site"]), None)
        res.append((c["site"], "?" if idx is None else eax_after_call(ins, idx)))
    return res


def _eax_back(ins, r, budget, hops):
    """Kind of the last EAX write before ins[r] (a RET), walking back: 'value' (MOV/LEA/XOR/POP/SETcc ...
    into EAX), 'call' (a CALL's result left in EAX), 'arith' (ADD/NEG/SHR ... on EAX), 'scratch' (written,
    then moved out to memory or another register, or stored through: a temporary, not a result), 'none' (no write in the
    window or back to the entry) or 'unknown' (block only reached by jumps that couldn't be followed).
    When the block starts after a RET/JMP, every jump to it is followed (`hops` times; worst kind wins)."""
    stored = False
    for j in range(r - 1, max(-1, r - 1 - budget), -1):
        a, _, t, _ = ins[j]
        mn, _, ops = t.partition(" ")
        if mn.startswith("RET") or mn == "JMP":
            tgt = int(ins[j + 1][0], 16)
            preds = [k for k, x in enumerate(ins) if x[2].startswith("J") and
                     re.search(r" 0x([0-9a-fA-F]+)$", x[2]) and int(x[2].rsplit("0x", 1)[1], 16) == tgt]
            if not hops or not preds:
                return "unknown"
            kinds = [_eax_back(ins, k, budget - (r - j), hops - 1) for k in preds]
            order = ("unknown", "none", "scratch", "arith", "call", "value")
            return min(kinds, key=order.index)
        if mn == "CALL":             # CALL [EAX + 0x40] too: EAX is the call's result afterwards
            return "call"
        kind, reg = eax_effect(t)
        if kind == "write":
            return "scratch" if stored else "value"
        if kind == "rmw":            # MOV EAX,[EAX*4 + T] / MOVZX EAX,AL: a load, despite reading EAX
            return "scratch" if stored else ("value" if mn in ("MOV", "MOVZX", "MOVSX") else "arith")
        if kind == "read" and mn in ("MOV", "PUSH", "XCHG", "MOVZX", "MOVSX") and \
                _eax_mentioned(re.sub(r"\[[^\]]*\]", "", ops.split(",", 1)[-1] if mn != "PUSH" else ops)):
            stored = True                                # EAX's value copied out (not just used as an address)
        dst = ops.split(",")[0]
        if kind == "read" and "[" in dst and _eax_mentioned(dst) and mn not in ("CMP", "TEST", "PUSH"):
            stored = True                                # a store through EAX: a pointer temporary
    return "none"


def eax_before_rets(ins, budget=EAX_RET_WINDOW):
    """[(ret addr, kind)] for every RET in the listing (kinds as _eax_back). A linear heuristic."""
    return [(x[0], _eax_back(ins, i, budget, 2)) for i, x in enumerate(ins) if x[2].split(" ")[0] == "RET"]


def eax_summary(ex, f, cg, asm):
    """One line of return-value evidence, the per-site map and a verdict dict
    {'verdict': 'value'|'void'|'inconclusive', 'ret': fixture type suggestion or None}."""
    sites = eax_at_call_sites(ex, cg["callers"]) if cg else []
    direct = [s for s in sites if s[1] not in ("tail", "?")]
    reads = [s[1].split(":")[1] for s in direct if s[1].startswith("read")]
    over = sum(1 for s in direct if s[1].startswith("over"))
    fpu = sum(1 for s in direct if s[1].startswith("fpu"))
    rets = sum(1 for s in direct if s[1].startswith("ret"))
    other = len(direct) - len(reads) - over - fpu - rets
    rk = eax_before_rets(asm["instructions"]) if asm else []
    fresh = sum(1 for _, k in rk if k in ("value", "call"))
    strong = bool(rk) and fresh == len(rk) and any(k == "value" for _, k in rk)
    parts = []
    if direct:
        s = f"EAX read after the call at {len(reads)}/{len(direct)} direct call sites"
        if reads and set(reads) <= {"AL"}:
            s += " (AL only)"
        elif reads and set(reads) <= {"AL", "AX", "AH"}:
            s += " (AX/AL only)"
        det = [f"{n} {w}" for n, w in ((over, "overwrite it first"), (fpu, "use ST0 (x87 return)"),
                                       (rets, "return right after (EAX passes through)"),
                                       (other, "branch or go far first")) if n]
        parts.append(s + (f"; {', '.join(det)}" if det else ""))
    else:
        parts.append("no direct call sites to check")
    if len(sites) - len(direct):
        parts.append(f"{len(sites) - len(direct)} tail-jump/unknown site(s) not checked")
    if rk:
        c = {k: sum(1 for _, x in rk if x == k) for k in ("value", "call", "arith", "scratch", "none", "unknown")}
        names = {"value": "a load/constant", "call": "a call's result", "arith": "arithmetic on EAX",
                 "scratch": "EAX used as a temporary", "none": "EAX not written", "unknown": "not followed"}
        how = ", ".join(f"{n} {names[k]}" for k, n in c.items() if n)
        parts.append(f"RETs (heuristic, last EAX write before each): {fresh}/{len(rk)} after a fresh EAX "
                     f"write ({how})")
    if reads:
        if set(reads) <= {"AL"}:
            verdict, ret, word = "value", "u8", "returns a value (callers read AL: bool/char)"
        elif set(reads) <= {"AL", "AX", "AH"}:
            verdict, ret, word = "value", "u16", "returns a value (callers read AX: short)"
        else:
            verdict, ret, word = "value", "u32", "returns a value (don't use void)"
    elif fpu:
        verdict, ret, word = "value", "double", "returns a value in ST0 (float/double, not EAX; don't use void)"
    elif over:
        verdict, ret, word = "void", None, "likely void"
        if strong:
            word += " (but every RET follows a fresh EAX write: the result may be ignored by every caller)"
    elif strong:
        verdict, ret, word = "value", "u32", "returns a value (callee side only: every RET follows a fresh EAX write)"
    else:
        verdict, ret, word = "inconclusive", None, "inconclusive"
    return "; ".join(parts) + f" -> {word}", dict(sites), {"verdict": verdict, "ret": ret}


def compress_values(vals, hexa=None):
    """[0,1,2,5] -> '0..2, 5' (hex when hexa, default: when any value exceeds 9)."""
    vals = sorted(set(vals))
    hexa = any(abs(v) > 9 for v in vals) if hexa is None else hexa
    fmt = (lambda v: f"{v:#x}" if v >= 0 else f"-{-v:#x}") if hexa else str
    out, i = [], 0
    while i < len(vals):
        j = i
        while j + 1 < len(vals) and vals[j + 1] == vals[j] + 1:
            j += 1
        out.append(fmt(vals[i]) if i == j else f"{fmt(vals[i])}..{fmt(vals[j])}")
        i = j + 1
    return ", ".join(out)


def extent_switches(ex, asm):
    """The function's switches as extent.function_extent finds them: [(jmp VA, table VA, entry count, how)],
    how = 'cmp' (bound), 'bytetable' (byte index table) or 'scan' (no bound in the code). It follows the
    control flow through the binary's bytes, so it sees jumps Ghidra never decoded. None when it can't run
    (no binary, or an extent that doesn't report switches): the caller falls back to the listing's text."""
    return _extent(ex, asm)[1]


def _extent(ex, asm):
    """(extent instruction set, switch list) of the function, from the binary's bytes; (None, None) when the
    extent can't run. Cached per address."""
    addr = asm["address"].upper()
    cache = ex.__dict__.setdefault("_extent_switches", {})
    if addr not in cache:
        cache[addr] = (None, None)
        pe = ex.pe
        if pe.ok:
            try:
                if str(TOOLS / "unicorn") not in sys.path:
                    sys.path.insert(0, str(TOOLS / "unicorn"))
                from eqd_unicorn.extent import function_extent
                starts = ex.__dict__.setdefault("_fn_starts", frozenset(int(a, 16) for a in ex.functions))
                entry = int(addr, 16)

                def read(va, n):                # extent asks for 16 bytes even at the end of a section
                    while n > 0:
                        b = pe.read(va, n)
                        if b is not None:
                            return b
                        n -= 1 if n <= 16 else 4
                    return b""

                def in_image(va):
                    return any(st <= va < st + vs for st, vs, _, _ in pe.sections)

                found = []
                ext = function_extent(read, entry, starts - {entry}, in_image=in_image, switches=found)
                cache[addr] = (frozenset(ext), sorted(found))
            except Exception:       # noqa: BLE001 (ExtentError; TypeError from an extent without `switches`)
                cache[addr] = (None, None)
    return cache[addr]


def outside_listing(ex, asm):
    """Instructions of the function's extent (what the harness runs and the coverage gate counts) that the
    Ghidra listing lacks: typically the case blocks of a switch Ghidra could not decode ("Could not find
    normalized switch variable"), so its body (and the queue `size`) stops short of them. Disassembled from
    the binary with capstone: [(va, bytes hex, text, note)], in address order; [] when none or no binary."""
    ext, _ = _extent(ex, asm)
    if not ext:
        return []
    have = {int(x[0], 16) for x in asm["instructions"]}
    extra = sorted(a for a in ext if a not in have)
    if not extra:
        return []
    import capstone
    md = getattr(ex, "_cs_md", None)
    if md is None:
        md = ex._cs_md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    out = []
    for a in extra:
        b = ex.pe.read(a, 16) or ex.pe.read(a, 1) or b""
        i = next(md.disasm(b, a, 1), None)
        if i is None:
            out.append((a8(a), "", "(undecodable)", ""))
            continue
        note = ""
        m = re.fullmatch(r"0x([0-9a-f]+)", i.op_str)
        if m and (i.mnemonic == "call" or i.mnemonic.startswith("j")):
            t = a8(int(m.group(1), 16))
            f = ex.functions.get(t)
            note = f["name"] if f and i.mnemonic == "call" else ("-> listed above" if int(t, 16) in have else "")
        out.append((a8(a), i.bytes.hex(), f"{i.mnemonic.upper()} {i.op_str}".strip(), note))
    return out


JMP_TABLE = re.compile(r"JMP dword ptr \[(\w+)\*0x4 \+ 0x([0-9a-f]+)\]$")


def switch_tables(ex, asm):
    """Jump tables (and VC6's byte index tables `MOVZX R,byte ptr [I + B]`) of a function, from the binary's
    bytes. Which switches exist and each table's entry count come from extent's switch list; the code before
    a `JMP dword ptr [R*0x4 + T]` is read only for the annotations (case-value bias, bound, default label,
    byte index table address). Returns a list of dicts."""
    ins = asm["instructions"]
    base = int(ex.manifest.get("image_base") or "00400000", 16)
    pos = {int(x[0], 16): k for k, x in enumerate(ins)}
    sites = extent_switches(ex, asm)
    if sites is None:               # fallback: the listing's own jumps, count from the code's bound check
        sites = [(int(a, 16), int(m.group(2), 16), None, "text") for a, _, t, _ in ins
                 for m in [JMP_TABLE.match(t)] if m]
    out = []
    for jva, table, count, how in sites:
        i = pos.get(jva)
        m = JMP_TABLE.match(ins[i][2]) if i is not None else None
        sw = {"at": a8(jva), "table": a8(table), "index_table": None, "bias": 0, "bias_insn": None,
              "bound": None, "default": None, "cases": [], "error": None, "how": how, "count": count,
              "note": None}
        out.append(sw)
        if m is not None:
            _annotate_switch(sw, ins, i, m.group(1), base)
        else:
            sw["note"] = "the jump is not in the Ghidra listing, so bias, bound and default are not known"
        pe = ex.pe
        if not pe.ok:
            sw["error"] = pe.why
            continue
        if count is None:           # fallback path: the bound found in the code
            if sw["bound"] is None or not 0 <= sw["bound"] < MAX_SWITCH_CASES:
                sw["error"] = "no bound check (`CMP idx,N` + `JA`) found next to the jump; table not decoded"
                continue
            count = sw["bound"] + 1 if not sw["index_table"] else None
        if sw["index_table"] and sw["bound"] is not None:
            b = pe.read(int(sw["index_table"], 16), sw["bound"] + 1)
            if b is None:
                sw["error"] = "index table outside the file"
                continue
            targets = pe.dwords(table, max(b) + 1)
            pairs = [(k, targets[b[k]] if targets else None, b[k]) for k in range(len(b))]
        elif how == "bytetable":    # extent saw a byte table the text heuristic didn't: list the slots
            targets = pe.dwords(table, count)
            pairs = [(k, targets[k] if targets else None, k) for k in range(count or 0)]
            sw["note"] = (sw["note"] or "") + ("; " if sw["note"] else "") + \
                "byte index table not located: cases are jump-table slots, not case values"
        else:
            targets = pe.dwords(table, count)
            pairs = [(k, targets[k] if targets else None, k) for k in range(count or 0)]
            if how == "scan":
                sw["note"] = (sw["note"] or "") + ("; " if sw["note"] else "") + \
                    "no bound check in the code: entries read while they point into this function"
        if not targets:
            sw["error"] = "jump table outside the file"
            continue
        sw["cases"] = [(k - sw["bias"], tgt, slot) for k, tgt, slot in pairs]
    return out


def _annotate_switch(sw, ins, i, reg, base):
    """Fills index_table, bound, default, bias of `sw` from the instructions before the JMP at ins[i]."""
    idx_reg = reg
    w0 = max(0, i - 10)
    win = ins[w0:i]
    low8 = {"EAX": "AL", "EBX": "BL", "ECX": "CL", "EDX": "DL"}.get(reg)
    last_w = next((k for k in range(len(win) - 1, -1, -1)       # last write to the jump register
                   if writes_reg(win[k][2], (reg, low8) if low8 else (reg,))), None)
    if last_w is not None:                          # byte index table feeding the jump register
        t2 = win[last_w][2]
        bm = (re.match(rf"MOV[ZS]X {reg},byte ptr \[(\w+) \+ 0x([0-9a-f]+)\]$", t2) or
              (low8 and re.match(rf"MOV {low8},byte ptr \[(\w+) \+ 0x([0-9a-f]+)\]$", t2)))
        if bm and int(bm.group(2), 16) >= base:      # a table in the image, not a struct field
            sw["index_table"], idx_reg = a8(int(bm.group(2), 16)), bm.group(1)
    cmp_k = None
    for k in range(len(win) - 1, -1, -1):          # bound check: CMP idx,imm ; JA default
        cm = re.match(rf"CMP {idx_reg},(\S+)$", win[k][2])
        loads = ()
        if not cm:                                  # unoptimized: CMP [local],N ... MOV idx,[local]
            ld = [x[2].split(",", 1)[1] for x in win[k + 1:] if x[2].startswith(f"MOV {idx_reg},")]
            cm = ld and re.match(rf"CMP {re.escape(ld[-1])},(\S+)$", win[k][2])
            loads = (f"MOV {idx_reg},{ld[-1]}",) if cm else ()
        if not cm:
            continue
        # the CMP only bounds the jump if a JA follows it and nothing rewrites the index before the
        # jump, except reloading it or redoing the write before the CMP (`LEA EAX,[ECX + -0x32]`
        # twice); memcpy's `CMP ECX,8; JC` + `NEG ECX` is not a bound check
        after = win[k + 1:]
        redo = next((x[2] for x in reversed(win[:k]) if writes_reg(x[2], (idx_reg,))), None)
        if not any(re.match(r"JA 0x", x[2]) for x in after) or any(
                writes_reg(x[2], (idx_reg,)) and x[2] not in loads and x[2] != redo and
                (sw["index_table"] is None or x is not win[last_w]) for x in after):
            break
        v = imm(cm.group(1))
        if v is None and re.fullmatch(r"E[A-Z]{2}", cm.group(1)):   # CMP idx,reg: reg's constant
            r2 = cm.group(1)
            p = w0 + k
            for q in range(p - 1, max(-1, p - 11), -1):
                t2 = ins[q][2]
                if t2.startswith(f"MOV {r2},"):
                    v = imm(t2.split(",", 1)[1])
                    break
                if t2 == f"POP {r2}":               # PUSH imm ; POP reg (VC6 /O1 size trick)
                    prev = ins[q - 1][2] if q else ""
                    v = imm(prev[5:]) if prev.startswith("PUSH ") else None
                    break
        if v is None:
            continue
        sw["bound"] = v
        cmp_k = k
        for x in after:
            jm = re.match(r"JA 0x([0-9a-f]+)$", x[2])
            if jm:
                sw["default"] = a8(int(jm.group(1), 16))
                break
        break
    if cmp_k is not None:                           # bias applied to the index before the bound check
        for x in reversed(win[max(0, cmp_k - 6):cmp_k]):
            t2 = x[2]
            bm = (re.match(rf"ADD {idx_reg},(\S+)$", t2) or re.match(rf"SUB {idx_reg},(\S+)$", t2)
                  or re.match(rf"LEA {idx_reg},\[\w+ \+ (\S+)\]$", t2))
            if bm and imm(bm.group(1)) is not None:
                v = imm(bm.group(1))
                sw["bias"], sw["bias_insn"] = (-v if t2.startswith("SUB") else v), t2
                break
            if t2 in (f"DEC {idx_reg}", f"INC {idx_reg}"):
                sw["bias"], sw["bias_insn"] = (-1 if t2.startswith("DEC") else 1), t2
                break
            if writes_reg(t2, (idx_reg,)):
                break


def trunc(s, n):
    return s if len(s) <= n else s[:n] + "...[truncated]"


def code(s):
    return s.replace("```", "'''")


def cell(s):
    return s.replace("|", "\\|").replace("\n", "\\n").replace("\r", "\\r")


# ---------------------------------------------------------------- matching notes (VC6 quirks, stackcmp)

QUIRKS_DOC = HERE.parent / "docs" / "VC6_QUIRKS.md"
QUIRK_MODES = ("nearmiss", "all", "off")
NEAR_MISS = ("diff", "structural")
MAX_QUIRKS = 4
MAX_QUIRK_TEXT = 320          # characters of the claim / the "use" line, cut at a sentence end
MAX_STACK_HINTS = 3
ARTEFACT = r"__except_list|bytes[ _]identical"     # diff summaries saying the miss is not in the code
_QUIRKS = {}

# recorded diff summaries (queue attempts' `diff`) -> (quirk id, weight); the first id is the main one
TEXT_TRIGGERS = [
    (r"regalloc|register|spill|\bslots?\b|ebp frame|frame size|stack", (("vc6q-decl-order-names", 3),
                                                                         ("vc6q-block-scope-slots", 2))),
    (r"jump ?table|switch|dec chain", (("vc6q-switch-table-threshold", 3), ("vc6q-switch-default-label", 2),
                                       ("vc6q-switch-case-order", 2))),
    (r"block order|branch order|layout|polarity|invert|fall.?through", (("vc6q-if-then-first", 3),
                                                                       ("vc6q-switch-case-order", 1))),
    (r"\bEH\b|exception|eh_prolog|fs:\s*\[|unwind|destructor", (("vc6q-eh-frame", 3), ("vc6q-new-ctor-eh", 2))),
    (r"store order|zero stores?|schedul", (("vc6q-store-order", 3),)),
    (r"call order|arg(ument)? order|evaluation order|operand order", (("vc6q-call-order", 3),)),
    (r"\bset(cc|n?(z|e|l|le|g|ge|a|ae|b|be))\b|\bsbb\b", (("vc6q-setcc-compare", 3), ("vc6q-bool-not-call", 2))),
    (r"\bidiv\b|modulo|division|\bcdq\b|0x8000000", (("vc6q-signed-div-pow2", 3),)),
    (r"inlin|comdat|out-of-line", (("vc6q-inline-o1", 3), ("vc6q-const-ref-inline", 2))),
    (r"static guard|guard (bit|byte)|atexit|(function|local) static", (("vc6q-static-guard", 3),)),
    (r"push imm|pop ecx|add esp, ?4", (("vc6q-o1-idioms", 3),)),
]

# features of the original listing -> (quirk id, weight)
FEATURE_QUIRKS = {
    "jump table": (("vc6q-switch-table-threshold", 2), ("vc6q-switch-case-order", 2),
                   ("vc6q-switch-default-label", 1)),
    "byte index table": (("vc6q-switch-byte-index", 3),),
    "dec chain": (("vc6q-switch-table-threshold", 2), ("vc6q-switch-one-case", 1)),
    "EH frame": (("vc6q-eh-frame", 3),),
    "operator new": (("vc6q-new-ctor-eh", 1),),
    "idiv by 2^k": (("vc6q-signed-div-pow2", 2),),
    "and 0x8000000k": (("vc6q-signed-div-pow2", 2),),
    "static guard": (("vc6q-static-guard", 3),),
    "setcc": (("vc6q-setcc-compare", 1),),
    "neg/sbb": (("vc6q-bool-not-call", 1),),
}


def _short(s, n=MAX_QUIRK_TEXT):
    s = " ".join(s.split())
    if len(s) <= n:
        return s
    cut = max(s.rfind(". ", 0, n), s.rfind(": ", 0, n))
    return (s[:cut + 1] if cut > n // 3 else s[:n].rstrip()) + " ..."


def load_quirks(path=None):
    """[{id, keywords, claim, use, repro}] from the `### vc6q-*` entries of docs/VC6_QUIRKS.md (cached)."""
    p = Path(path) if path else QUIRKS_DOC
    if p not in _QUIRKS:
        try:
            text = p.read_text(encoding="utf-8")
        except OSError:
            text = ""
        out = []
        for m in re.finditer(r"^### (vc6q-[\w-]+)[ \t]*\n(.*?)(?=^#{1,3} |\Z)", text, re.M | re.S):
            body = m.group(2)
            kl = re.search(r"^keywords:(.*)$", body, re.M)
            parts = kl.group(1).split("·") if kl else [""]
            repro = next((re.search(r"`([^`]+)`", x).group(1) for x in parts[1:]
                          if x.strip().startswith("repro:") and "`" in x), None)

            def para(tag):
                pm = re.search(rf"\*\*{tag}\.\*\*(.*?)(?:\n[ \t]*\n|\Z)", body, re.S)
                return " ".join(pm.group(1).split()) if pm else ""
            out.append({"id": m.group(1), "keywords": [k.strip() for k in parts[0].split(",") if k.strip()],
                        "claim": para("Claim"), "use": para("Use"), "repro": repro})
        _QUIRKS[p] = out
    return _QUIRKS[p]


def _pow2(v):
    return v is not None and v > 1 and v & (v - 1) == 0


def listing_features(asm):
    """{feature: detail} from the original's instructions: cheap pattern checks, no data flow."""
    ins = [x[2] for x in (asm or {}).get("instructions", [])]
    syms = [x[3] or "" for x in (asm or {}).get("instructions", [])]
    feats = {}
    jt = [i for i, t in enumerate(ins) if re.match(r"JMP dword ptr \[\w+\*0x4 \+ 0x[0-9a-f]+\]$", t)]
    if jt:
        feats["jump table"] = f"{len(jt)} `JMP [R*4 + table]`"
        if any(re.match(r"MOV(ZX|SX)? \w+,byte ptr \[\w+ \+ 0x[0-9a-f]{6,8}\]$", t)
               for i in jt for t in ins[max(0, i - 6):i]):
            feats["byte index table"] = "byte table feeding the jump"
    dec = sum(1 for i in range(len(ins) - 1)
              if re.match(r"(DEC E[A-Z]X|SUB E[A-Z]X,0x0)$", ins[i]) and ins[i + 1].startswith(("JZ ", "JE ")))
    if dec >= 2:
        feats["dec chain"] = f"{dec} `dec; jz` tests"
    if any(s == "CALL __EH_prolog" for s in syms) or any("FS:[0x0]" in t for t in ins):
        feats["EH frame"] = "`__EH_prolog`" if any(s == "CALL __EH_prolog" for s in syms) else "`fs:[0]`"
    if any(re.match(r"CALL operator_new", s) for s in syms):
        feats["operator new"] = "calls `operator new`"
    for i, t in enumerate(ins):
        m = re.match(r"IDIV (E[A-Z]X)$", t)
        if not m:
            continue
        r = m.group(1)
        for j in range(i - 1, max(-1, i - 5), -1):
            if ins[j].startswith(f"MOV {r},"):
                if _pow2(imm(ins[j].split(",", 1)[1])):
                    feats["idiv by 2^k"] = f"`{ins[j]}; ... IDIV {r}`"
                break
            if ins[j] == f"POP {r}":           # `PUSH 8; CDQ; POP ECX; IDIV ECX`
                k = next((k for k in range(j - 1, max(-1, j - 3), -1) if ins[k] != "CDQ"), None)
                if k is not None and ins[k].startswith("PUSH ") and _pow2(imm(ins[k][5:])):
                    feats["idiv by 2^k"] = f"`{ins[k]}; ... POP {r}; IDIV {r}`"
                break
    def signmask(t):          # AND reg,0x80000000 | (2^k - 1): VC6's signed % 2^k
        m = re.match(r"AND E[A-Z]X,(0x[0-9a-f]+)$", t)
        v = int(m.group(1), 16) if m else 0
        return v & 0x80000000 and 0 < (v & 0x7FFFFFFF) and _pow2((v & 0x7FFFFFFF) + 1)
    am = next((t for t in ins if signmask(t)), None)
    if am:
        feats["and 0x8000000k"] = f"`{am}`"
    tests = {m.group(1, 2) for t in ins for m in [re.match(r"TEST byte ptr \[(0x[0-9a-f]+)\],(0x[0-9a-f]+)$", t)]
             if m}
    guards = [g for g in tests if f"OR byte ptr [{g[0]}],{g[1]}" in ins]
    if guards or any(s == "CALL _atexit" for s in syms):
        feats["static guard"] = (f"guard byte `{guards[0][0]}`" if guards else "") + \
            (" + `_atexit`" if any(s == "CALL _atexit" for s in syms) else "")
    sc = sorted({t.split()[0] for t in ins if re.match(r"SET[A-Z]{1,3} ", t)})
    if sc:
        feats["setcc"] = ", ".join(f"`{s}`" for s in sc)
    if any(ins[i].startswith("NEG ") and any(t.startswith("SBB ") for t in ins[i + 1:i + 3])
           for i in range(len(ins))):
        feats["neg/sbb"] = "`neg; sbb`"
    nf = sum(1 for t in ins if re.match(r"F[A-Z0-9]+( |$)", t))
    if nf:
        feats["x87"] = f"{nf} x87 instructions"
    return feats


def select_quirks(quirks, diff_text, feats, limit=MAX_QUIRKS):
    """[(quirk, [reasons])], best first: diff-summary triggers (1-3), listing features (1-3), and the
    entry's own multi-word keywords found in the diff summary (1). Ties keep the doc's order."""
    by_id = {q["id"]: q for q in quirks}
    score, why = {}, {}

    def add(qid, w, r):
        if qid in by_id:
            score[qid] = score.get(qid, 0) + w
            if r not in why.setdefault(qid, []):
                why[qid].append(r)
    low = (diff_text or "").lower()
    for pat, ids in TEXT_TRIGGERS:
        m = re.search(pat, diff_text or "", re.I)
        if m:
            for qid, w in ids:
                add(qid, w, f'diff summary "{m.group(0)}"')
    for q in quirks:
        for k in q["keywords"]:
            if " " in k and k.lower() in low:
                add(q["id"], 1, f'diff summary "{k}"')
    for f in feats:
        for qid, w in FEATURE_QUIRKS.get(f, ()):
            add(qid, w, f"original has {f}")
    order = {q["id"]: i for i, q in enumerate(quirks)}
    best = sorted(score, key=lambda k: (-score[k], order[k]))[:limit]
    return [(by_id[k], why[k]) for k in best]


class StackHints:
    """stackcmp's hints JSON (compare/README.md "Hints JSON", schema 1), indexed by VA. `error` is set
    (and nothing is indexed) when the file is missing, unreadable or of another schema/client."""

    def __init__(self, path, client=None):
        self.path, self.by_va, self.generated, self.error = Path(path), {}, None, None
        try:
            d = json.loads(self.path.read_text(encoding="utf-8"))
        except (OSError, ValueError) as e:
            self.error = f"{type(e).__name__}: {e}"
            return
        if not isinstance(d, dict) or d.get("schema") != 1 or d.get("tool", "stackcmp") != "stackcmp":
            self.error = f"not a stackcmp schema 1 file: {self.path}"
            return
        if client and d.get("client") not in (None, client):
            self.error = f"hints are for client {d.get('client')}, not {client}"
            return
        self.generated = d.get("generated")
        for f in d.get("functions") or []:
            va = str(f.get("va") or "")
            va = va[2:] if va.lower().startswith("0x") else va
            if re.fullmatch(r"[0-9A-Fa-f]{1,8}", va):
                self.by_va[va.upper().rjust(8, "0")] = f

    def get(self, addr):
        return self.by_va.get(addr.upper())


def load_stack_hints(spec, ws, client):
    """spec None: <build>\\match\\stackcmp.json when it exists (where compare/README.md's stackcmp command
    writes it); 'off': none; else that file (warns when unusable)."""
    if spec == "off":
        return None
    if spec is None:
        p = ws.build / "match" / "stackcmp.json"
        if not p.exists():
            return None
    else:
        p = Path(spec)
    h = StackHints(p, client)
    if h.error:
        print(f"no stack hints: {h.error}", file=sys.stderr)
        return None
    return h


STACK_MEANING = {
    "same": "every paired stack operand agrees: the miss is registers or instruction choice, not the frame",
    "differs": "the frame layout differs",
    "no_stack_operands": "no stack operands to compare",
}


def stack_line(e, generated=None):
    """One line from a stackcmp function entry (None unless status `ok`)."""
    if not e or e.get("status") != "ok":
        return None
    st = e.get("stack") or "?"
    bits = [f"stack **{st}**" + (f" ({STACK_MEANING[st]})" if st in STACK_MEANING else "")]
    fr = e.get("frame") or {}
    if fr.get("orig") is not None or fr.get("rewrite") is not None:
        bits.append(f"`sub esp, N`: original {fr.get('orig')}, rewrite {fr.get('rewrite')}")
    if e.get("complete") is False:
        bits.append("partial (structural blocks not paired)")
    hints = [h for h in e.get("hints") or [] if isinstance(h, str)]
    s = "; ".join(bits)
    if hints:
        s += ". " + " / ".join(trunc(h, 220) for h in hints[:MAX_STACK_HINTS])
        if len(hints) > MAX_STACK_HINTS:
            s += f" (+{len(hints) - MAX_STACK_HINTS} more)"
    return f"stackcmp ({(generated or '?')[:10]}): {s}"


def matching_notes(queue, addr, asm, mode="nearmiss", stack=None, quirks=None):
    """Markdown for the "VC6 matching notes" section, or "" when there is nothing to say."""
    e = queue.entries.get(addr) or {}
    m = e.get("match") if isinstance(e.get("match"), dict) else {}
    status = m.get("status")
    picked, feats, artefact = [], {}, None
    if mode == "all" or (mode == "nearmiss" and status in NEAR_MISS):
        atts = queue.attempts(addr)
        text = " ; ".join(str(a.get("diff") or "") for a in atts[-MAX_PRIOR:] if isinstance(a, dict))
        feats = listing_features(asm)
        artefact = re.search(ARTEFACT, text, re.I)
        if not artefact:
            picked = select_quirks(load_quirks() if quirks is None else quirks, text, feats)
    sl = stack_line(stack.get(addr), stack.generated) if stack else None
    if not picked and not sl and not artefact and "x87" not in feats:
        return ""
    out = ["## VC6 matching notes\n"]
    if status:
        mb = e.get("match_best") if isinstance(e.get("match_best"), dict) else {}
        out.append(f"Match now: **{status}**" + (f", {m['ndiff']} differing" if m.get("ndiff") is not None else "")
                   + (f" ({m['flags']})" if m.get("flags") else "")
                   + (f"; best: {mb.get('status')}, {mb.get('ndiff')} differing"
                      if mb and (mb.get("status"), mb.get("ndiff")) != (status, m.get("ndiff")) else "") + ".\n")
    if artefact:
        out.append(f"- The recorded diff summary says the difference is a compare artefact "
                   f"(\"{artefact.group(0)}\"), not code: no quirks picked.")
    if sl:
        out.append(f"- {sl}")
    if picked:
        out.append(f"- VC6 SP4 quirks that may apply (from `docs/VC6_QUIRKS.md`, each reproduced on SP4; full "
                   f"entry and repro there). Picked by the recorded diff summary and the original's code:")
        for q, why in picked:
            out.append(f"  - **{q['id']}** ({'; '.join(why)}). {_short(q['claim'])} **Use:** {_short(q['use'])}")
    if "x87" in feats:
        out.append(f"- Original has x87 code ({feats['x87']}); VC6_QUIRKS has no SP4-checked x87 entry yet "
                   "(gta2_re's `x87_handoff.md` is listed there as not folded).")
    return "\n".join(out) + "\n"


# ---------------------------------------------------------------- bundle

def build_bundle(ex, hdr, queue, addr, similar=None, k_similar=5, prior=True, min_similar=0.0, quirks="nearmiss",
                 stack_hints=None, full_dir=None):
    """quirks: one of QUIRK_MODES (VC6_QUIRKS entries for diff/structural functions, for every function,
    or none); stack_hints: a StackHints (stackcmp) or None; full_dir: where a truncated listing is written in
    full (FULL_PSEUDO.c, FULL_ASM.txt; e.g. a sandboxed copy, which can't read the export)."""
    f = ex.functions[addr]
    if full_dir:                    # a FULL_* file of an earlier function must not survive into this bundle
        Path(full_dir).mkdir(parents=True, exist_ok=True)
        for n in (FULL_PSEUDO, FULL_ASM, EXTRA_ASM):
            Path(full_dir, n).unlink(missing_ok=True)
    cg = ex.callgraph.get(addr, {"callees": [], "callers": [], "address_taken": []})
    ps = ex.pseudo(addr)
    asm = ex.asm(addr)
    client = ex.client
    if getattr(ex, "_gfxh", None) is None:
        ex._gfxh = gfx_header_names(repo_of(ex.ws, client), client)
    gfxh = ex._gfxh
    out = []
    w = out.append

    # ---- header
    w(f"# {f['name']} @ 0x{addr}  ({client})\n")
    nl = f.get("name_list")
    w("## Header\n")
    w("| | |\n|---|---|")
    w(f"| address | `0x{addr}` .. `0x{f['end']}` |")
    w(f"| name | `{f['name']}` |")
    if nl:
        w(f"| name evidence | **{nl['confidence']}** / {nl['source']}" +
          (f" (from {nl['from_address']})" if nl.get("from_address") else "") + " |")
    else:
        w(f"| name evidence | none in `names\\*.csv` (Ghidra name source: {f['name_source']}) |")
    w(f"| class / tags | {f['class']}" + (f" / {', '.join(f['tags'])}" if f["tags"] else "") + " |")
    qs, qhow = queue.subsystem(addr)
    if f.get("subsystem"):
        w(f"| subsystem | {f['subsystem']} |")
    elif qs:
        w(f"| subsystem | {qs} (queue: {qhow or 'inferred'}) |")
    else:
        w("| subsystem | unknown |")
    w(f"| source file (Mac map) | {f.get('source_file') or 'unknown'} |")
    w(f"| size | {f['size']} bytes, {f['instructions']} instructions |")
    w(f"| calling convention | {f['calling_convention']}" +
      (f", callee pops {f['stack_purge']} bytes" if f.get("stack_purge") is not None and 0 <= f["stack_purge"] <= 0x400 else "") + " |")
    ecx_line, ecx_how = ecx_summary(ex, f, cg, asm)
    w(f"| ECX evidence | {cell(ecx_line)} |")
    eax_line, _, _ = eax_summary(ex, f, cg, asm)
    w(f"| EAX evidence | {cell(eax_line)} |")
    w(f"| signature (Ghidra, {f['signature_source']}) | `{f['signature']}` |")
    if ps and ps.get("signature"):
        w(f"| signature (decompiler) | `{ps['signature']}` |")
    q = queue.state(addr)
    if q:
        w(f"| queue state | {q} |")
    w("")
    if nl and nl.get("note"):
        w(f"Name note: {nl['note']}\n")
    if f["name_alternatives"]:
        w("Rejected/alternative names: " + "; ".join(
            f"`{a['name']}` ({a['source']}, {a['confidence']})" for a in f["name_alternatives"]) + "\n")
    plate = plate_comment(ps["c"]) if ps and ps.get("c") else None
    if plate:
        w(f"Decompiler plate comment (name-chain evidence): {plate}\n")
    if f["class"] not in ("named", "unnamed"):
        w(f"> NOTE: classified `{f['class']}`; library/funclet code is normally skipped, not rewritten.\n")

    # ---- rules
    w("## Rules (excerpt)\n")
    src_rel, src_text = source_of(repo_of(ex.ws, client), (queue.entries.get(addr) or {}).get("subsystem"))
    w(RULES.replace("{CLIENT}", client.upper()).replace("{addr8}", addr.lower())
      .replace("{COMPILER}", compiler_of(client, src_rel, src_text)))
    reg = registry().get(client)
    if reg.prefix:
        w(IMAGE_NOTE.format(program=Path(reg.binary or client).name, title=reg.title, base=reg.base,
                            prefix=reg.prefix, token=reg.token))
    w("")
    w("Expected annotation for this function:\n")
    w("```cpp")
    w(f"// FUNCTION: {client.upper()} 0x{addr.lower()}")
    if f.get("source_file"):
        w(f"// ORIGINAL FILE: {f['source_file'].replace('Source/PC Source/', '')} (from Mac debug map)")
    w("```\n")

    # ---- pseudocode
    w("## Ghidra pseudocode\n")
    if not ps:
        w("_No pseudocode record._\n")
    elif ps["status"] != "ok":
        w(f"_Decompiler status `{ps['status']}`: {ps.get('error')}_\n")
    else:
        lines = pseudo_lines(ps["c"])
        note, lead = "", ""
        if len(lines) > MAX_PSEUDO_LINES:
            where = (f"`{FULL_PSEUDO}`, next to this file" if full_dir
                     else f"`export\\pseudocode\\{f['shard']}` (record {addr})")
            if full_dir:
                Path(full_dir, FULL_PSEUDO).write_text(code("\n".join(lines)) + "\n", encoding="utf-8")
            lead = (f"> **TRUNCATED: this listing stops at line {MAX_PSEUDO_LINES} of {len(lines)}.** The "
                    f"complete pseudocode is in {where}; read that file for the rest.\n")
            note = (f"\n_Truncated: showing {MAX_PSEUDO_LINES} of {len(lines)} lines; the complete pseudocode "
                    f"is in {where}._")
            lines = lines[:MAX_PSEUDO_LINES]
        if lead:
            w(lead)
        w("```c")
        w(code("\n".join(lines)))
        w("```" + note + "\n")

    # ---- asm
    w("## Assembly\n")
    if not asm:
        w("_No asm record._\n")
    else:
        ins = asm["instructions"]
        w("Ranges: " + ", ".join(f"`{a}-{b}`" for a, b in asm["ranges"]) + "\n")
        full = len(ins) > MAX_ASM_INSNS and full_dir
        where = (f"`{FULL_ASM}`, next to this file" if full
                 else f"`export\\asm\\{f['shard']}` (record {addr})")
        if len(ins) > MAX_ASM_INSNS:
            w(f"> **TRUNCATED: this listing stops at instruction {MAX_ASM_INSNS} of {len(ins)}** "
              f"(up to `{ins[MAX_ASM_INSNS - 1][0]}`). The complete listing is in {where}; read that file "
              "for the rest.\n")
        w("```asm")
        listing = []
        for a, b, t, sym in (ins if full else ins[:MAX_ASM_INSNS]):
            notes = [sym] if sym and sym != t else []
            for m in ABS_REF.finditer(t):
                g = gpa_label(ex, gfxh, a8(int(m.group(1), 16)), short=True)
                if g:
                    notes.append("-> " + g)
            listing.append(f"{a}  {b:<24} {t}" + (f"    ; {'  '.join(notes)}" if notes else ""))
        for ln in listing[:MAX_ASM_INSNS]:
            w(ln)
        w("```")
        if full:
            Path(full_dir, FULL_ASM).write_text("\n".join(listing) + "\n", encoding="utf-8")
        if len(ins) > MAX_ASM_INSNS:
            w(f"\n_Truncated: showing {MAX_ASM_INSNS} of {len(ins)} instructions; the complete listing is "
              f"in {where}._")
        w("")
        extra = outside_listing(ex, asm)
        extra_at = {int(x[0], 16) for x in extra}
        if extra:
            w(f"## Code outside the Ghidra listing ({len(extra)} instructions)\n")
            w(f"> **PART OF THIS FUNCTION.** Ghidra's body for `{addr}` stops short of these instructions (most "
              "often the case blocks of a switch it could not decode, so the pseudocode lacks those cases too), "
              "but control reaches them from the code above (jump tables, branches). The harness runs them as "
              "this function and the coverage gate counts them: translate them like the rest, and give the "
              "fixture cases that reach them. Disassembled from the binary (capstone syntax); `...` marks a "
              "gap.\n")
            w("```asm")
            lines, prev = [], None
            for a, b, t, n in extra:
                va = int(a, 16)
                if prev is not None and va != prev:
                    lines.append("...")
                lines.append(f"{a}  {b:<24} {t}" + (f"    ; {n}" if n else ""))
                prev = va + len(b) // 2
            if len(lines) > MAX_ASM_INSNS and full_dir:
                Path(full_dir, EXTRA_ASM).write_text("\n".join(lines) + "\n", encoding="utf-8")
                w("\n".join(lines[:MAX_ASM_INSNS]))
                w("```")
                w(f"\n_Truncated: showing {MAX_ASM_INSNS} of {len(lines)} lines; the complete listing is in "
                  f"`{EXTRA_ASM}`, next to this file._\n")
            else:
                w("\n".join(lines))
                w("```\n")
        sws = switch_tables(ex, asm)
        if sws:
            w(f"## Switch tables ({len(sws)})\n")
            w("Decoded from the binary's bytes (the export has none); the switch list is extent's, so it "
              "includes jumps Ghidra never decoded. Case value = table index minus the bias the code adds "
              "before the bound check.\n")
            for sw in sws:
                head = f"### `JMP` at `{sw['at']}`: jump table `{sw['table']}`"
                if sw["index_table"]:
                    head += f", byte index table `{sw['index_table']}`"
                w(head + "\n")
                info = []
                if sw["bound"] is not None:
                    info.append(f"bound: index 0..{sw['bound']:#x}")
                if sw["bias_insn"]:
                    info.append(f"bias {sw['bias']:+d} from `{sw['bias_insn']}`")
                if sw["default"]:
                    info.append(f"default (`JA`) -> `{sw['default']}`")
                if info:
                    w("; ".join(info) + "\n")
                if sw["note"]:
                    w(f"_{sw['note']}._\n")
                if sw["error"]:
                    w(f"_{sw['error']}_\n")
                    continue
                by_t = {}
                for v, tgt, _ in sw["cases"]:
                    by_t.setdefault(tgt, []).append(v)
                hexa = any(abs(v) > 9 for v, _, _ in sw["cases"])
                w("| target | case values | note |\n|---|---|---|")
                for tgt, vals in sorted(by_t.items(), key=lambda kv: min(kv[1])):
                    ta = a8(tgt)
                    note = "default" if ta == sw["default"] else ""
                    if tgt in extra_at:
                        note = (note + " " if note else "") + "not in the Ghidra listing: see 'Code outside'"
                    elif not any(int(s, 16) <= tgt <= int(e, 16) for s, e in asm["ranges"]):
                        note = (note + " " if note else "") + "outside this function"
                    w(f"| `{ta}` | {compress_values(vals, hexa)} | {note} |")
                w("")

    # ---- callers
    callers = cg["callers"]
    w(f"## Callers ({len(callers)} call sites)\n")
    if not callers:
        w("None direct (check address-taken references below).\n")
    else:
        by = {}
        for c in callers:
            by.setdefault(c["function"], []).append(c["site"])
        w("| caller | name | call sites | ECX set before call |\n|---|---|---|---|")
        for i, (fn, sites) in enumerate(by.items()):
            if i >= MAX_CALLERS:
                w(f"| ... | {len(by) - MAX_CALLERS} more callers omitted | | |")
                break
            cf = ex.functions.get(fn)
            hows = [ecx_how.get(s, "?") for s in sites]
            chk = [h for h in hows if h not in ("tail", "?")]
            ecx = (f"{sum(h.startswith('set') for h in chk)}/{len(chk)}" if chk else "-") + \
                next((f" (`{h.split(':', 2)[2]}`)" for h in hows if h.startswith("set")), "")
            w(f"| `{fn}` | {cell(cf['name']) if cf else '?'} | {', '.join('`' + s + '`' for s in sites[:6])}"
              + (" ..." if len(sites) > 6 else "") + f" | {cell(ecx)} |")
        w("")
    if cg["address_taken"]:
        w("Address taken (callbacks / vtables / tables): " + ", ".join(
            f"`{a['from']}`" + (f" in {ex.functions[a['function']]['name']}" if a.get("function") in ex.functions else "")
            + f" ({a['type']})" for a in cg["address_taken"][:12]) + "\n")

    # ---- callees
    callees = cg["callees"]
    w(f"## Callees ({len(callees)} call sites)\n")
    if not callees:
        w("None.\n")
    else:
        seen = {}
        for c in callees:
            key = c["to"] or (c["kind"], c.get("operand"), c.get("name"))
            seen.setdefault(key, []).append(c)
        n = 0
        for key, cs in seen.items():
            n += 1
            if n > MAX_CALLEES:
                w(f"\n_{len(seen) - MAX_CALLEES + 1} more distinct callees omitted._")
                break
            c0 = cs[0]
            sites = ", ".join(f"`{c['site']}`" for c in cs[:5]) + (" ..." if len(cs) > 5 else "")
            if c0["kind"] == "indirect":
                m = ABS_REF.search(c0.get("operand") or "")
                g = gpa_label(ex, gfxh, a8(int(m.group(1), 16))) if m else None
                w(f"- indirect `{c0.get('operand')}`" + (f" = **{g}**" if g else "") + f" at {sites}")
                continue
            to = c0["to"]
            cf = ex.functions.get(to) if to else None
            if not cf:
                w(f"- {c0['kind']} `{c0.get('name')}` at {sites}")
                continue
            cps = ex.pseudo(to)
            nlc = cf.get("name_list")
            bits = [f"`{to}` **{cf['name']}**", f"`{cf['signature']}`", cf["calling_convention"],
                    f"{cf['size']}B",
                    f"name: {nlc['confidence']}/{nlc['source']}" if nlc else f"name: {cf['name_source']}"]
            qs = queue.state(to)
            if qs:
                bits.append(f"state: {qs}")
            w("- " + " | ".join(bits) + f" | sites {sites}")
            if cps and cps["status"] == "ok" and cps.get("c"):
                body = [l.strip() for l in pseudo_lines(cps["c"])
                        if l.strip() and not l.strip().startswith(("/*", "*", "//")) and l.strip() not in ("{", "}")
                        and not l.strip().endswith("*/") and l.strip().endswith(";")]
                one = [l for l in body[1:] if not re.match(r"^[\w\s\*]+\s\**\w+(\[\w+\])?;$", l)]
                if one:
                    w(f"    - body: `{trunc(' '.join(one[:3]), 160)}`" + (" ..." if len(one) > 3 else ""))
        w("")

    # ---- strings
    w(f"## Strings ({len(f['strings'])})\n")
    if f["strings"]:
        w("| address | value |\n|---|---|")
        for a in f["strings"][:60]:
            s = ex.strings.get(a)
            w(f"| `{a}` | `{cell(trunc(s['value'], MAX_STRING)) if s else '?'}` |")
        if len(f["strings"]) > 60:
            w(f"| ... | {len(f['strings']) - 60} more |")
        w("")
    else:
        w("None.\n")

    # ---- globals
    w(f"## Globals ({len(f['globals'])})\n")
    if f["globals"]:
        w("| address | name | type | size | block | writable | xrefs |\n|---|---|---|---|---|---|---|")
        for a in f["globals"][:MAX_GLOBALS]:
            g = ex.globals.get(a, {})
            w(f"| `{a}` | {g.get('name') or '-'} | {g.get('type') or 'undefined'} | {g.get('size') or '-'} | "
              f"{g.get('block') or '-'} | {g.get('writable')} | {len(g.get('xrefs', []))} |")
        if len(f["globals"]) > MAX_GLOBALS:
            w(f"| ... | {len(f['globals']) - MAX_GLOBALS} more | | | | | |")
        w("")
    else:
        w("None.\n")

    # ---- structs
    offs = disp_offsets(asm) if asm else set()
    text = f["signature"] + " " + (ps["signature"] if ps and ps.get("signature") else "") + " " + \
        " ".join(p["type"] for p in f["parameters"])
    cands = []
    if f.get("namespace"):
        cands.append(f["namespace"].split("::")[0])
    cands += re.findall(r"[A-Za-z_]\w*", text)
    if ps and ps.get("c"):
        cands += re.findall(r"\b([A-Z]\w+)\s*\*", ps["c"])
    for c in callees:
        cf = ex.functions.get(c.get("to"))
        if cf and cf.get("namespace"):
            cands.append(cf["namespace"].split("::")[0])
    ordered, seen_n = [], set()
    for c in cands:
        if c in hdr.defs and c not in seen_n:
            seen_n.add(c)
            ordered.append(c)
    picked = hdr.pick(ordered, offs)
    w("## Known structs (`include\\` headers matching this function)\n")
    if offs:
        w("Register displacements used in the asm: " + ", ".join(f"0x{o:X}" for o in sorted(offs)[:60]) +
          (" ..." if len(offs) > 60 else "") + "\n")
    if picked:
        for n, fl, t, note in picked:
            w(f"### {n}  (`{fl}`)\n")
            w("```c\n" + code(t) + "\n```" + (f"\n_{note}_" if note else "") + "\n")
    else:
        w("No header definition matches the signature, class or callees (this image may have no struct "
          "headers yet). Read the fields at the asm displacements through ONE file-local struct per object at the "
          "top of the unit file, named `<Name>_guess` (reuse it if the file already has one); don't edit "
          "`include\\` unless the task allows header edits.\n")

    # ---- similar matched functions (similar.py)
    if similar is not None and k_similar:
        import similar as sim
        w(sim.markdown(similar.top(addr, k_similar, min_similar)))

    # ---- VC6 matching notes (quirks, stack frame)
    notes = matching_notes(queue, addr, asm, quirks, stack_hints)
    if notes:
        w(notes)

    # ---- prior attempts
    atts = queue.attempts(addr) if prior else []
    if atts:
        w(f"## Prior attempts ({len(atts)})\n")
        shown = atts[-MAX_PRIOR:]
        for i, a in enumerate(shown):
            w(f"### Attempt {len(atts) - len(shown) + i + 1}\n")
            if isinstance(a, dict):
                for k, v in a.items():
                    if k in ("diff", "code", "source", "output"):
                        w(f"{k}:\n```\n{code(trunc(str(v), 4000))}\n```")
                    else:
                        w(f"- {k}: {trunc(str(v), 500)}")
            else:
                w(trunc(str(a), 4000))
            w("")

    return "\n".join(out) + "\n"


# ---------------------------------------------------------------- main

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("client")
    ap.add_argument("addresses", nargs="*")
    ap.add_argument("--subsystem", help="every function whose subsystem equals S (or starts with 'S/')")
    ap.add_argument("--next", type=int, metavar="N",
                    help="the queue's next N ready functions (with --subsystem: within it)")
    ap.add_argument("--limit", type=int, help="cap for --subsystem")
    ap.add_argument("--export-dir", help="default <build>\\export\\<client>")
    ap.add_argument("--out", help="default <build>\\context\\<client>")
    ap.add_argument("--similar", type=int, default=5, metavar="K",
                    help="add the K most similar matched functions (similar.py; 0: none)")
    ap.add_argument("--similar-cache", help="listing cache dir (default <build>\\similar\\<client>)")
    ap.add_argument("--quirks", choices=QUIRK_MODES, default="nearmiss",
                    help="VC6_QUIRKS entries: for functions whose queue match is diff/structural (default), "
                         "for every function (todo ones: from the listing only), or off")
    ap.add_argument("--stack-hints", metavar="F",
                    help="stackcmp hints JSON (default <build>\\match\\stackcmp.json when present; 'off': none)")
    a = ap.parse_args(argv)

    ex = Export(a.client, a.export_dir)
    repo = repo_of(ex.ws, a.client)
    hdr = Headers(repo, a.client)
    queue = Queue(repo)
    finder = make_finder(a.client, repo, queue, a.similar_cache) if a.similar else None
    stack = load_stack_hints(a.stack_hints, ex.ws, a.client)
    out = Path(a.out) if a.out else ex.ws.build / "context" / a.client
    out.mkdir(parents=True, exist_ok=True)

    addrs = [norm_addr(x) for x in a.addresses]
    if a.subsystem:
        s = a.subsystem
        # The export only knows map-derived subsystems; the queue also has the inferred ones.
        def sub_of(k, v):
            return v.get("subsystem") or queue.subsystem(k)[0]
        m = [k for k, v in ex.functions.items() if sub_of(k, v) and
             (sub_of(k, v) == s or sub_of(k, v).startswith(s + "/"))]
        if not a.next:
            addrs += m[:a.limit] if a.limit else m
    if a.next:
        addrs += next_from_queue(a.client, a.next, a.subsystem)
    if not addrs:
        ap.error("no functions given")
    done = set()
    for ad in addrs:
        if ad in done:
            continue
        done.add(ad)
        if ad not in ex.functions:
            print(f"skip {ad}: not a function in the export", file=sys.stderr)
            continue
        text = build_bundle(ex, hdr, queue, ad, finder, a.similar, quirks=a.quirks, stack_hints=stack)
        p = out / f"{ad}_{safe_name(ex.functions[ad]['name'])}.md"
        p.write_text(text, encoding="utf-8", newline="\n")
        print(f"wrote {p} ({len(text):,} bytes)")


def make_finder(client, repo, queue, cache=None, exclude=()):
    """similar.Finder over the queue's matched functions, or None (with a note) when it can't be built."""
    try:
        import similar
        return similar.Finder(client, repo, list(queue.entries.values()), cache, exclude)
    except Exception as e:       # noqa: BLE001 (no binary, no repo: the bundle just has no examples)
        print(f"no similar examples: {type(e).__name__}: {e}", file=sys.stderr)
        return None


def next_from_queue(client, n, subsystem=None):
    """Late-bound: ask the queue CLI (queue/queue.py next) for the next N addresses."""
    import subprocess
    qpy = HERE.parent / "queue" / "queue.py"
    if not qpy.exists():
        sys.exit("--next needs queue/queue.py, which does not exist")
    cmd = [sys.executable, str(qpy), "-c", client, "next", "-n", str(n)]
    if subsystem:
        cmd += ["--subsystem", subsystem]
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit("queue next failed: " + r.stderr.strip())
    return [norm_addr(l.split()[0]) for l in r.stdout.splitlines() if l.strip() and HEX8.match(l.split()[0])]


if __name__ == "__main__":
    main()
