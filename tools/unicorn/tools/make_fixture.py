r"""Write a Unicorn fixture skeleton for one rewritten function, prefilled from its rewrite and the export.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\unicorn\tools\make_fixture.py trilogy 0x00410C80
        [--source src\EverQuest\Display.cpp] [--out tests\unicorn\...\X.json] [--force] [--no-build] [--run]

What it fills in (everything else is a `_todo` entry in the fixture; delete `_todo` when done):
- `function` / `original` / `sources`: the `// FUNCTION: <CLIENT> <VA>` annotation's name and file, plus the
  files that define what the test DLL can't link without (found by linking: each unresolved external is
  looked up among the other sources' annotations and definitions, repeated until it links).
- `conv`, `args`, `ret` from the rewrite's prototype (a `Class::` method is `thiscall` unless the class
  declares it `static`; `__stdcall`/`__fastcall`/`__cdecl` keywords win), checked against the export's stack
  purge. Types: pointers `ptr` (`char *` `cstr`), `float`, `double`, `__int64` `i64`, `short`/`u16`/`char`/
  `u8`/`bool` by width and sign, everything else `int`/`u32`.
- `this` and pointer args point at fixture `objects` (random fill) sized from the headers'
  `C_ASSERT(sizeof(T) == N)` when the type is known, else 0x100.
- `callees`: every call of the **original** (export call graph) except the CRT defaults
  (`crt_defaults.json`, automatic): fixed-address prototypes from the `EQ_CDECL/EQ_STDCALL/EQ_THISCALL` macros
  in include\ and src\ (exact arg types), else annotated rewrites' prototypes, else the export (stack purge:
  `ret N` -> thiscall when the call site loads ECX, else stdcall; cdecl arg count from the caller's
  `add esp, N`). Scripted with `ret: 0` (`ret_float` for float returns). Library callees get the
  `rewrite_aliases` entry for the test DLL's own copy (`__EH_prolog` passes through). Imports are keyed
  `dll!name` (args: TODO); indirect calls are listed in `_todo` (build an `@fn` object).
- three starter `cases` (typical / zeros / null pointers) and a `fuzz` block (seed from the VA, 100 cases,
  pointer args fixed to their objects); `_todo` also lists the globals the original references.

`--run` runs the new fixture once and prints the summary (it may fail: fill in the values). Output path:
tests\unicorn\<source path without src\ and extension>\<Name>.json (`_guess` dropped); an existing file is
never overwritten without --force.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import eqd_repo  # noqa: E402
import mutations as mu  # noqa: E402
from eqd_unicorn import BuildError, annotations as ann, undecorate  # noqa: E402

TOOLS = Path(__file__).resolve().parents[2]

# C type words -> fixture type (by width/sign); pointers are handled before this.
INT_TYPES = {
    "char": "char", "signed char": "char", "unsigned char": "u8", "BYTE": "u8", "u8": "u8", "s8": "char",
    "UCHAR": "u8", "uchar": "u8", "bool": "bool",
    "short": "short", "signed short": "short", "unsigned short": "u16", "WORD": "u16", "u16": "u16",
    "s16": "short", "USHORT": "u16", "ushort": "u16", "short int": "short", "unsigned short int": "u16",
    "int": "int", "signed int": "int", "long": "int", "signed long": "int", "long int": "int", "s32": "int",
    "BOOL": "int", "INT": "int", "LONG": "int",
    "unsigned": "u32", "unsigned int": "u32", "unsigned long": "u32", "DWORD": "u32", "UINT": "u32",
    "ULONG": "u32", "u32": "u32", "size_t": "u32", "uint": "u32", "dword": "u32",
    "float": "float", "double": "double", "__int64": "i64", "unsigned __int64": "u64", "s64": "i64",
    "u64": "u64", "void": "void",
}
QUALIFIERS = {"const", "volatile", "struct", "class", "union", "enum", "register", "static", "inline", "__inline",
              "virtual", "__cdecl", "__stdcall", "__fastcall", "__thiscall"}
# Ghidra's names in the export -> the test DLL's symbol for the same library function.
LIB_SYMBOLS = {"operator_new": "??2@YAPAXI@Z", "operator_delete": "??3@YAXPAX@Z",
               "operator_delete_guess": "??3@YAXPAX@Z"}
PASSTHROUGH_LIB = {"__EH_prolog"}

_MACRO = re.compile(r"EQ_(CDECL|STDCALL|THISCALL)\s*\(\s*([^,]+?)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*\(([^()]*)\)\s*\)")
# EQ_METHOD(n, ret, va, (this_type, A1, ..), obj): a __thiscall method (include/trilogy/eqthiscall.h)
_METHOD = re.compile(r"EQ_METHOD\s*\(\s*\d+\s*,\s*([^,]+?)\s*,\s*(0x[0-9A-Fa-f]+)\s*,\s*\(([^()]*)\)\s*,")
_SIZE = re.compile(r"C_ASSERT\s*\(\s*sizeof\s*\(\s*(?:struct\s+|class\s+)?(\w+)\s*\)\s*==\s*(0x[0-9A-Fa-f]+|\d+)\s*\)")
OUT_NAMES = re.compile(r"out|buf|dest|dst|result|ret", re.I)
_UNRESOLVED = re.compile(r'unresolved external symbol\s+"?([^\s"]+)"?')


# Callees that never return (by the export's short name; Ghidra's `noreturn` tag counts too): a call to one ends
# the run ("noreturn": true). VC6 emits no cleanup after such a call, so a stub that returns runs into garbage.
NORETURN_NAMES = {"_exit", "exit", "__exit", "_c_exit", "abort", "_abort", "_amsg_exit", "__amsg_exit", "__purecall",
                  "_purecall", "_CxxThrowException", "__CxxThrowException@8", "ExitProcess", "_ExitProcess@4",
                  "FatalAppExitA", "RaiseException"}


def is_noreturn(f) -> bool:
    return "noreturn" in (f.get("tags") or []) or (f.get("short_name") or f.get("name") or "") in NORETURN_NAMES


def code_purge(repo, va: int):
    """The bytes the original's function at `va` pops (its `ret N`, read from the code by the harness), else
    None (older tools, no workspace, or a function whose code doesn't settle it)."""
    try:
        return repo.harness().purge_of(va)
    except Exception:           # noqa: BLE001
        return None


def stub_pops(spec) -> int:
    from eqd_unicorn.machine import Callee
    try:
        return Callee(key=0, conv=spec.get("conv", "cdecl"), args=list(spec.get("args", []))).pops()
    except Exception:           # noqa: BLE001
        return -1


def known_purge(f):
    """The export's stack purge in bytes, or None when Ghidra didn't know it: older exports store its
    UNKNOWN_STACK_DEPTH_CHANGE sentinel (0x7FFFFFFF), which once made a 537-million-argument fixture."""
    n = f.get("stack_purge")
    return n if isinstance(n, int) and 0 <= n <= 0x400 else None


def ctype(t: str) -> str:
    """A C type (one parameter's type, without its name) -> fixture type."""
    t = " ".join(t.replace("*", " * ").replace("&", " & ").split())
    if "*" in t or "&" in t or "[" in t:
        base = [w for w in t.split() if w not in QUALIFIERS | {"*", "&"}]
        return "cstr" if base in (["char"], ["CHAR"], ["LPSTR"], ["LPCSTR"]) and t.count("*") == 1 else "ptr"
    words = [w for w in t.split() if w not in QUALIFIERS]
    key = " ".join(words)
    if key in INT_TYPES:
        return INT_TYPES[key]
    if key in ("LPSTR", "LPCSTR"):
        return "cstr"
    if key.startswith(("LP", "P")) and key[1:2].isupper() or key.endswith(("_t", "Proc", "Callback")):
        return "ptr" if key.endswith("_t") or key.startswith(("LP", "P")) else "u32"
    return "int"


def pointee(t: str) -> str | None:
    words = [w for w in t.replace("*", " * ").split() if w not in QUALIFIERS | {"*", "&"}]
    return words[0] if len(words) == 1 else None


def split_params(s: str) -> list[str]:
    out, depth, cur = [], 0, ""
    for c in s:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += c
    if cur.strip():
        out.append(cur.strip())
    return [] if out in ([], ["void"]) else out


def param_name_type(p: str) -> tuple[str, str]:
    """`EQPlayer *viewer` -> ("viewer", "EQPlayer *"); unnamed `int` -> ("", "int")."""
    p = re.sub(r"\s*=.*$", "", p).strip()                       # default argument
    m = re.match(r"^(.*?)([A-Za-z_]\w*)\s*(\[[^\]]*\])?$", p)
    if m and m.group(1).strip() and m.group(1).strip() not in QUALIFIERS and \
            not (m.group(1).strip() in ("unsigned", "signed", "long", "short") and m.group(2) in INT_TYPES):
        return m.group(2), m.group(1).strip() + (" *" if m.group(3) else "")
    return "", p


class Prototype:
    def __init__(self, head: str, name: str):
        """`head`: the definition's tokens up to the body, joined by spaces (mutations.function_body)."""
        self.name = name
        self.head = head
        toks = mu.tokenize(head)
        texts = [t.text for t in toks]
        if "(" not in texts:
            raise ValueError(f"no parameter list in {head!r}")
        i = texts.index("(")
        j = mu._match(toks, i, "(", ")", 1)
        if j is None:
            raise ValueError(f"unbalanced parameter list in {head!r}")
        s = i - 1                                   # the qualified name: id (:: id)*, maybe ~
        while s >= 2 and texts[s - 1] in ("::", "~"):
            s -= 2 if texts[s - 1] == "::" else 1
        if s >= 1 and texts[s - 1] == "~":
            s -= 1
        self.ret_text = " ".join(texts[:s])
        self.params_text = head[toks[i].end:toks[j].start]
        self.params = [param_name_type(p) for p in split_params(self.params_text)]

    @property
    def conv_keyword(self):
        for k, c in (("__stdcall", "stdcall"), ("__fastcall", "fastcall"), ("__cdecl", "cdecl")):
            if k in self.ret_text.split():
                return c
        return None

    @property
    def ret(self):
        t = " ".join(w for w in self.ret_text.split() if w not in ("static", "virtual", "inline", "__inline",
                                                                  "__cdecl", "__stdcall", "__fastcall"))
        if not t or t == "void":
            return "void"
        return ctype(t)


def struct_sizes(repo_root: Path) -> dict:
    out = {}
    for h in (repo_root / "include").rglob("*.h"):
        for m in _SIZE.finditer(h.read_text(encoding="utf-8", errors="replace")):
            out.setdefault(m.group(1), int(m.group(2), 0))
    return out


def macro_prototypes(repo_root: Path) -> dict:
    """{va: (conv, ret type text, [param type texts])} from EQ_CDECL/EQ_STDCALL/EQ_THISCALL/EQ_METHOD uses
    (THISCALL's (this_type, int) pair and METHOD's this_type dropped)."""
    out = {}
    files = list((repo_root / "include").rglob("*.h")) + list((repo_root / "src").rglob("*.c*"))
    for f in files:
        text = f.read_text(encoding="utf-8", errors="replace")
        for m in _MACRO.finditer(text):
            conv, ret, va, params = m.group(1).lower(), m.group(2), int(m.group(3), 16), split_params(m.group(4))
            if conv == "thiscall":
                params = params[2:]
            out.setdefault(va, (conv, ret, params))
        for m in _METHOD.finditer(text):
            out.setdefault(int(m.group(2), 16), ("thiscall", m.group(1), split_params(m.group(3))[1:]))
    return out


def is_static_method(repo_root: Path, cls: str, method: str) -> bool:
    pat = re.compile(rf"\bstatic\b[^;{{}}]*\b{re.escape(method)}\s*\(")
    for h in list((repo_root / "include").rglob("*.h")) + list((repo_root / "src").rglob("*.h")):
        text = h.read_text(encoding="utf-8", errors="replace")
        if re.search(rf"\b(class|struct)\s+{re.escape(cls)}\b", text) and pat.search(text):
            return True
    return False


def obj_name(s: str, taken: set) -> str:
    base = re.sub(r"_guess$", "", s).lower() or "obj"
    base = re.sub(r"\W", "_", base)
    n, i = base, 2
    while n in taken:
        n, i = f"{base}{i}", i + 1
    taken.add(n)
    return n


_EXPORTS: dict = {}


def load_export(client):
    if client not in _EXPORTS:
        sys.path.insert(0, str(TOOLS / "context"))
        import pack_context                                    # noqa: E402  (no Java needed)
        _EXPORTS[client] = (pack_context, pack_context.Export(client))
    return _EXPORTS[client]


def call_site_info(pc, asm, site: str):
    """(ECX loaded before the call?, bytes the caller pops right after it or None)."""
    ins = asm["instructions"] if asm else []
    idx = next((i for i, x in enumerate(ins) if x[0] == site), None)
    if idx is None:
        return None, None
    ecx = pc.ecx_walk(ins, idx).startswith("set")
    pops = None
    if idx + 1 < len(ins):
        t = ins[idx + 1][2]
        m = re.match(r"ADD ESP,(0x[0-9a-fA-F]+|\d+)$", t)
        if m:
            pops = int(m.group(1), 0)
        elif t == "POP ECX":
            pops = 4 + (4 if idx + 2 < len(ins) and ins[idx + 2][2] == "POP ECX" else 0)
    return ecx, pops


def callee_specs(repo, ex, pc, va: int, defaults: dict, todo: list):
    """-> (callees, rewrite_aliases)."""
    addr = f"{va:08X}"
    cg = ex.callgraph.get(addr, {"callees": []})
    asm = ex.asm(addr) if addr in ex.functions else None
    default_keys = {int(k, 16) for k in (defaults.get("callees") or {}) if k.lower().startswith("0x")}
    macros = macro_prototypes(repo.root)
    rewrites = {a.va: a for a in repo.annotations()}
    callees, aliases, seen = {}, {}, set()
    for c in cg["callees"]:
        kind = c["kind"]
        if kind == "indirect":
            todo.append(f"indirect call at {c['site']} (`{c.get('operand')}`): store an \"@fn\" object in the "
                        f"pointer/vtable slot via objects/memory and key the callee \"@fn\"")
            continue
        if kind == "call_no_function":
            todo.append(f"call at {c['site']} to {c.get('to')} (no function in the export): script it by VA")
            continue
        if kind == "import":
            dll, _, fn = (c.get("name") or "?::?").partition("::")
            # through an import thunk (`call thunk` -> `jmp [IAT]`): keyed by the thunk's VA, as the rewrite
            # reaches it through EQ_STDCALL(.., thunk VA, ..); straight through the IAT: "dll!name"
            key = f"0x{int(c['to'], 16):08x}" if c.get("to") else f"{dll.lower()}!{fn}"
            if key not in seen:
                seen.add(key)
                callees[key] = {"name": fn, "conv": "stdcall", "args": [], "ret": 0}
                todo.append(f"callee {key} ({dll}!{fn}): import, fill in `args` (stdcall pops 4 bytes per arg) "
                            f"and `ret`")
            continue
        to = c.get("to")
        if not to:
            continue
        cva = int(to, 16)
        if cva in default_keys or cva in seen:
            continue
        seen.add(cva)
        f = ex.functions.get(to, {})
        name = f.get("name") or to
        key = f"0x{cva:08x}"
        spec = {"name": name}
        if f.get("class") == "library":
            short = f.get("short_name") or name
            sym = LIB_SYMBOLS.get(short, short if short.startswith("_") else None)
            if short in PASSTHROUGH_LIB:
                spec.update({"args": [], "passthrough": True})
            else:
                spec.update({"conv": "cdecl", "args": ["u32"] * len(f.get("parameters") or []), "ret": 0})
                todo.append(f"callee {key} {name}: library code, scripted; check args/ret (or passthrough if pure)")
            if sym:
                aliases[sym] = key
            if is_noreturn(f) and not spec.get("passthrough"):
                spec["noreturn"] = True
            callees[key] = spec
            continue
        if cva in macros:
            conv, ret, params = macros[cva]
            spec["conv"] = conv
            spec["args"] = [ctype(p) for p in params]
            rt = ctype(ret) if ret.strip() != "void" else "void"
            how = "EQ_ macro"
        elif cva in rewrites:
            a = rewrites[cva]
            text = Path(a.path).read_text(encoding="utf-8", errors="replace")
            try:
                proto = Prototype(mu.function_body(text, cva, repo.client)[2], a.name)
                conv = proto.conv_keyword or ("thiscall" if "::" in a.name and not is_static_method(
                    repo.root, a.name.split("::")[-2], a.name.split("::")[-1]) else "cdecl")
                spec["conv"] = conv
                spec["args"] = [ctype(t) for _, t in proto.params]
                rt = proto.ret
                how = "rewrite"
            except ValueError:
                how = None
        else:
            how = None
        cpurge = code_purge(repo, cva)
        if how is None:
            purge = cpurge if cpurge is not None else known_purge(f)
            ecx, pops = call_site_info(pc, asm, c["site"])
            if purge:
                spec["conv"] = "thiscall" if ecx else "stdcall"
                spec["args"] = ["u32"] * (purge // 4)
            else:
                spec["conv"] = "cdecl"
                n = pops // 4 if pops is not None else len(f.get("parameters") or [])
                spec["args"] = ["u32"] * n
            rt = "int"
            todo.append(f"callee {key} {name}: conv/args from the export ({spec['conv']}, {len(spec['args'])} "
                        f"args, as u32): check types (pointers into stack frames: ignore/cstr/mem:N) and `ret`")
        if rt in ("float", "double"):
            spec["ret_float"] = 0.0
        elif rt != "void":
            spec["ret"] = 0
        if is_noreturn(f):
            spec["noreturn"] = True
        elif cpurge is not None and stub_pops(spec) not in (-1, cpurge):
            todo.append(f"callee {key} {name}: its code returns with `ret {cpurge}` but {spec.get('conv')} with "
                        f"{len(spec.get('args', []))} args pops {stub_pops(spec)}: fix conv/args (a by-value object "
                        "built on the stack before the call is an arg too)")
        callees[key] = spec
    return callees, aliases


def sources_closure(repo, sources: list[Path], va: int, log) -> list[Path]:
    """Add the files defining the externals the test DLL can't resolve, until it links (max 6 rounds)."""
    h = repo.harness()
    srcs = list(sources)
    anns = repo.annotations()
    all_src = eqd_repo.clients.sources(repo.root, repo.client)       # this image's sources only
    for _ in range(6):
        try:
            h.rewrite_image(f"mk{va:08x}", repo.build_sources({"sources": [repo.rel(s) for s in srcs]}))
            return srcs
        except BuildError as e:
            missing = sorted(set(_UNRESOLVED.findall(str(e))))
            if not missing:
                raise
            added = False
            for sym in missing:
                name = undecorate(sym)
                hits = {Path(a.path).resolve() for a in anns if a.name == name}
                if not hits:
                    base = name.split("::")[-1]
                    pat = re.compile(rf"^[^\s#/][^;(]*\b{re.escape(base)}\b\s*(\(|=|;|\[)", re.M)
                    hits = {p.resolve() for p in all_src
                            if pat.search(p.read_text(encoding="utf-8", errors="replace"))} - set(srcs)
                    hits = set(sorted(hits)[:1])
                new = [p for p in sorted(hits) if p not in srcs]
                if new:
                    log(f"  links {sym}: + {', '.join(repo.rel(p) for p in new)}")
                    srcs += new
                    added = True
            if not added:
                raise BuildError(f"unresolved and not found in src: {', '.join(missing)}\n{e}")
    return srcs


def eax_ret_evidence(pc, ex, addr: str, fx: dict, todo: list) -> None:
    """Cross-check fx['ret'] with the context packer's EAX evidence (callers reading EAX after the call):
    a `void` rewrite of a function whose callers use EAX gets `ret` u32/u8/u16 and a TODO, since a void
    fixture compares nothing and would hide the mistake."""
    if addr not in ex.functions or not hasattr(pc, "eax_summary"):
        return
    try:
        line, _, v = pc.eax_summary(ex, ex.functions[addr], ex.callgraph.get(addr) or {"callers": []},
                                    ex.asm(addr))
    except Exception:                                       # evidence is advisory; never block the skeleton
        return
    if fx["ret"] == "void" and v["verdict"] == "value":
        if v["ret"] in ("u32", "u8", "u16"):
            fx["ret"] = v["ret"]
            todo.append(f"`ret` set to {v['ret']} although the rewrite is void: the original returns a value "
                        f"({line}). Make the rewrite return it (fix the type), don't set `ret` back to void")
        else:
            todo.append(f"the rewrite is void but the original returns a value ({line}): fix the rewrite "
                        "and set `ret` (float/double)")
    elif fx["ret"] != "void" and v["verdict"] == "void":
        todo.append(f"EAX evidence says the original is likely void ({line}): check that `ret` "
                    f"{fx['ret']} is real")


def make(repo, client: str, va: int, source=None, build=True, log=print) -> tuple[dict, Path]:
    anns = [a for a in (ann.scan([repo.src_path(source)], client.upper()) if source else repo.annotations())
            if a.va == va]
    if len(anns) != 1:
        raise SystemExit(f"{len(anns)} `// FUNCTION: {client.upper()} {va:#010x}` annotations"
                         + (f" in {source}" if source else " under src\\") + "; pass --source")
    a = anns[0]
    src = Path(a.path).resolve()
    text = src.read_text(encoding="utf-8", errors="replace")
    proto = Prototype(mu.function_body(text, va, client)[2], a.name)
    todo: list = []
    sizes = struct_sizes(repo.root)
    pc, ex = load_export(client)
    f = ex.functions.get(f"{va:08X}", {})

    # convention
    parts = a.name.split("::")
    conv = proto.conv_keyword
    if conv is None:
        conv = "thiscall" if len(parts) > 1 and not is_static_method(repo.root, parts[-2], parts[-1]) else "cdecl"
    args, objects, taken = [], {}, set()
    starter = {"typical": [], "zeros": [], "null": []}
    fuzz_args = []
    for i, (pname, ptype) in enumerate(proto.params):
        t = ctype(ptype)
        if t == "cstr" and "const" not in ptype.split() and OUT_NAMES.search(pname or ""):
            t = "ptr"                                       # an output buffer, not an input string
        if t in ("void",):
            continue
        args.append({"name": pname or f"a{i}", "type": t})
        if t == "ptr":
            pt = pointee(ptype)
            o = obj_name(pname or pt or f"a{i}", taken)
            size = sizes.get(pt or "", None)
            objects[o] = {"size": f"{size:#x}" if size else "0x100", "fill": "random"}
            if not size:
                todo.append(f"object {o}: size unknown ({ptype.strip()}), 0x100 assumed")
            starter["typical"].append(f"@{o}")
            starter["zeros"].append(f"@{o}")
            starter["null"].append(0)
            fuzz_args.append({"choices": [f"@{o}"]})
        elif t == "cstr":
            starter["typical"].append("abc")
            starter["zeros"].append("")
            starter["null"].append("abc")
            fuzz_args.append({})
        elif t in ("float", "double"):
            starter["typical"].append(1.5)
            starter["zeros"].append(0.0)
            starter["null"].append(1.5)
            fuzz_args.append({})
        else:
            starter["typical"].append(1)
            starter["zeros"].append(0)
            starter["null"].append(1)
            fuzz_args.append({})
    slots = sum(2 if x["type"] in ("double", "i64", "u64") else 1 for x in args)
    purge = known_purge(f)
    want = 4 * slots if conv in ("thiscall", "stdcall") else 0
    if conv == "fastcall":
        want = 4 * max(0, slots - 2)
    if purge is not None and purge != want:
        todo.append(f"the export says the original pops {purge} bytes (`ret {purge}`); {conv} with these args "
                    f"pops {want}: check the convention and the arg types")

    fx: dict = {"function": a.name, "original": f"0x{va:08x}"}
    srcs = [src]
    if build:
        try:
            srcs = sources_closure(repo, srcs, va, log)
        except BuildError as e:
            todo.append(f"the test DLL doesn't build yet: {str(e).strip().splitlines()[0][:200]}")
    fx["sources"] = [repo.rel(s) for s in srcs]
    fx["conv"] = conv
    if conv == "thiscall":
        cls = parts[-2]
        o = obj_name(cls, taken)
        size = sizes.get(cls)
        objects = {o: {"size": f"{size:#x}" if size else "0x100", "fill": "random"}, **objects}
        if not size:
            todo.append(f"object {o} (this): size of {cls} unknown, 0x100 assumed")
        fx["this"] = f"@{o}"
    fx["args"] = args
    ctor = len(parts) > 1 and parts[-1] == parts[-2]
    fx["ret"] = "ptr" if ctor else proto.ret                # VC6 constructors return `this` in EAX
    eax_ret_evidence(pc, ex, f"{va:08X}", fx, todo)
    fx["objects"] = objects
    fx["memory"] = []
    callees, aliases = callee_specs(repo, ex, pc, va, repo.crt_defaults(), todo)
    fx["callees"] = callees
    if aliases:
        fx["rewrite_aliases"] = aliases
    cases = [{"name": "typical", "args": starter["typical"]}, {"name": "zeros", "args": starter["zeros"]}]
    if any(x["type"] == "ptr" for x in args):
        cases.append({"name": "null pointers", "args": starter["null"]})
        todo.append("case 'null pointers': drop it if the original never checks for null (both sides fault "
                    "the same way, which matches, but it tests little)")
    fx["cases"] = cases
    fx["fuzz"] = {"seed": va & 0xFFFFFF, "count": 100, "args": fuzz_args, "memory": []}
    for g in f.get("globals", [])[:20]:
        rec = ex.globals.get(g, {}) if hasattr(ex, "globals") else {}
        todo.append(f"global 0x{g} {rec.get('name') or ''} ({rec.get('type') or '?'}, {rec.get('size') or '?'} "
                    f"bytes): seed it in `memory` / vary it in `fuzz.memory` if the function reads it")
    todo.append("set fields the function reads in objects (`init`) and `fuzz.memory` (see the context bundle's "
                "displacement list); add cases for every branch; then delete `_todo`")
    fx["_todo"] = todo

    rel = src.relative_to(repo.root / "src").with_suffix("") if src.is_relative_to(repo.root / "src") \
        else Path(src.stem)
    short = re.sub(r"_guess$", "", parts[-1].lstrip("~")) or f"F{va:08x}"
    if parts[-1].startswith("~"):
        short = "dtor_" + short
    elif len(parts) > 1 and parts[-1] == parts[-2]:
        short = "ctor"
    out = repo.tests_dir / rel / f"{short}.json"
    return fx, out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("client")
    ap.add_argument("va")
    ap.add_argument("--source", help="the rewrite's file (default: search src\\ for the annotation)")
    ap.add_argument("--repo", help="decomp repo (default: the one around the cwd, else <workspace>\\eq-<client>)")
    ap.add_argument("--out", help="fixture path (default tests\\unicorn\\<subsystem path>\\<Name>.json)")
    ap.add_argument("--force", action="store_true", help="overwrite an existing file")
    ap.add_argument("--no-build", action="store_true", help="don't link to find the sources' closure")
    ap.add_argument("--run", action="store_true", help="run the new fixture once and print the summary")
    ap.add_argument("--stdout", action="store_true", help="print the fixture instead of writing it")
    a = ap.parse_args(argv)
    try:
        repo = eqd_repo.Repo(a.repo, a.client)
    except eqd_repo.RepoError as e:
        print(f"make_fixture: {e}", file=sys.stderr)
        return 2
    va = int(a.va, 16)
    fx, out = make(repo, a.client, va, a.source, build=not a.no_build)
    text = json.dumps(fx, indent=2) + "\n"
    if a.stdout:
        print(text)
        return 0
    out = Path(a.out).resolve() if a.out else out
    if out.exists() and not a.force:
        stem = out.with_name(f"{out.stem}_{va:08x}.json")
        if stem.exists():
            print(f"make_fixture: {out} and {stem} exist (--force overwrites, --out picks another name)",
                  file=sys.stderr)
            return 2
        out = stem
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(text, encoding="utf-8")
    print(f"wrote {repo.rel(out)}: {fx['conv']}, {len(fx['args'])} args, ret {fx['ret']}, "
          f"{len(fx['callees'])} callees, {len(fx['_todo'])} TODOs")
    for t in fx["_todo"]:
        print(f"  TODO {t}")
    if a.run:
        try:
            r = repo.run(out)
            print(r.summary())
        except Exception as e:                                  # noqa: BLE001  (show, don't crash)
            print(f"run failed: {type(e).__name__}: {e}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
