"""Generate C declarations for Ghidra from client headers, without changing the source tree.

The shared declaration reader removes methods; inheritance is emitted as named base
subobjects so inherited field types, packing and unions remain inspectable. Ghidra
independently computes layouts, which build_gdt checks against source and canonical assertions.
"""
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "structs"))
import cparse as C

LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)

# Platform SDK types the source headers get from <windef.h>/<windows.h> (directly or
# from the including translation unit), which the umbrella does not include. Emitted
# only when the generated declarations use the name and no client header defines it.
# Layouts match the 32-bit SDK (LONG is 32 bits).
WINDOWS_TYPES = {
    "RECT": "typedef struct tagRECT { long left; long top; long right; long bottom; } RECT;",
    "POINT": "typedef struct tagPOINT { long x; long y; } POINT;",
    "SIZE": "typedef struct tagSIZE { long cx; long cy; } SIZE;",
    # DECLARE_HANDLE(HWND): source members spell it struct HWND__ *.
    "HWND__": "struct HWND__ { int unused; };",
}


def header_paths(repo, client):
    """All nested client headers plus their local include closure (relative to repo)."""
    repo = Path(repo).resolve()
    todo = list((repo / "include" / client).rglob("*.h"))
    seen = set()
    while todo:
        path = todo.pop().resolve()
        if path in seen:
            continue
        if not path.is_relative_to(repo / "include"):
            raise ValueError(f"include outside the include tree: {path}")
        seen.add(path)
        for inc in LOCAL_INCLUDE.findall(path.read_text(encoding="utf-8-sig")):
            target = next((p for p in (path.parent / inc, repo / "include" / inc) if p.is_file()), None)
            if target is None:
                raise ValueError(f"{path}: missing local include {inc}")
            todo.append(target)
    return sorted(p.relative_to(repo).as_posix() for p in seen)


def typedef_name(stmt):
    """Name declared by a typedef statement: the function pointer name or the last declarator."""
    m = re.search(r"\(\s*(?:__\w+\s*)*\*+\s*(\w+)\s*\)\s*\(", stmt)
    if m:
        return m.group(1)
    m = re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;\s*$", stmt)
    return m.group(1) if m else None


def load_headers(repo, client):
    idx = C.Index()
    for rel in header_paths(repo, client):
        idx.add(C.parse_file(rel, (Path(repo) / rel).read_text(encoding="utf-8-sig")))
    idx.resolve_aliases()
    return idx


def generate(repo, client, out):
    idx = load_headers(repo, client)
    defs = [d for d in idx.all_defs() if not d.parent]
    names = {}
    for d in defs:
        for name in set(d.names):
            if name in names and names[name] is not d:
                raise ValueError(f"duplicate type {name}: {d.file}, {names[name].file}")
            names[name] = d
    lines = ["/* Generated parser umbrella. Source headers remain unchanged. */"]
    # Numeric defines are evaluated by the source reader; expression dimensions below
    # must resolve exactly, never with its unknown-size fallback.
    for name in sorted(idx.defines):
        val = idx.eval_const(name)
        if val is not None:
            lines.append(f"#define {name} {val}")
    # Preserve real enum values and callback signatures rather than replacing them
    # with integer/pointer stand-ins.
    enums, typedefs = [], []
    # Ghidra files every parsed type under the umbrella's name; build_gdt moves each
    # one to its declaring header's basename (the categories earlier archives used).
    categories = {name: Path(d.file).name for name, d in names.items()}
    categories.update({(d.tag or d.name): Path(d.file).name for d in defs})
    for fi in idx.files.values():
        # Ghidra turns each #define into an enum named define_<NAME>.
        for name in fi.defines:
            categories.setdefault("define_" + name, Path(fi.rel).name)
        for m in re.finditer(r"\b(?:typedef\s+)?enum\s+(\w*)\s*\{", fi.code):
            end = C.match_close(fi.code, m.end() - 1)
            semi = fi.code.find(";", end)
            enums.append(fi.text[m.start():semi + 1])
            for name in [m.group(1)] + re.findall(r"\w+", fi.code[end + 1:semi]):
                if name:
                    categories.setdefault(name, Path(fi.rel).name)
        for m in re.finditer(r"\btypedef\s+[^;{}]+;", fi.code):
            stmt = fi.text[m.start():m.end()]
            declared = typedef_name(fi.code[m.start():m.end()])
            # C_ASSERT expansions and template member typedefs (eqd_ret<R>::type)
            # are not types. Match the declared name, not any word, so a callback
            # with a parameter called "type" keeps its typedef.
            if declared in ("__C_ASSERT__", "type"):
                continue
            # Struct aliases are generated consistently below, including aliases
            # to forward-only tags. Keep function pointer typedefs verbatim.
            if not re.search(r"\b(?:struct|class|union)\b", stmt) or "(" in stmt:
                if declared not in names:
                    typedefs.append(stmt)
                    if declared:
                        categories.setdefault(declared, Path(fi.rel).name)
    # Forward declarations also cover pointer-only source types with no known body.
    forwards = {n: d.kind for n, d in names.items()}
    alias_tags = {}
    for fi in idx.files.values():
        for kind, name in re.findall(r"\b(struct|class|union)\s+(\w+)\s*;", fi.code):
            forwards.setdefault(name, "union" if kind == "union" else "struct")
            categories.setdefault(name, Path(fi.rel).name)
        # typedef struct Tag Alias; where Tag has no body in the client headers
        for kind, tag, name in re.findall(r"\btypedef\s+(struct|class|union)\s+(\w+)\s+(\w+)\s*;", fi.code):
            if name not in names:
                forwards.setdefault(name, "union" if kind == "union" else "struct")
                alias_tags.setdefault(name, tag)
                categories.setdefault(name, Path(fi.rel).name)
                categories.setdefault(tag, Path(fi.rel).name)
    for name, kind in sorted(forwards.items()):
        d = names.get(name)
        tag = (d.tag or d.name) if d else alias_tags.get(name, name)
        lines.append(f"typedef {kind} {tag} {name};")
    lines.extend(enums)
    lines.extend(dict.fromkeys(typedefs))
    emitted, busy = set(), set()

    def body(d):
        result = []
        for i, base in enumerate(d.bases):
            bd = idx.find_struct(base, d.file)
            if bd is None:
                raise ValueError(f"{d.name}: unknown base {base}")
            # A base is a complete subobject: flattening its declarations would
            # incorrectly apply the derived class's packing inside that base.
            result.append(f"{base} _base_{i};")
        if d.has_virtual and not d.bases:
            result.append("void **__vptr;")
        for rm in d.raw:
            if rm.inline:
                decl = rm.inline.kind + " {\n" + "\n".join(body(rm.inline)) + "\n}"
            else:
                decl = re.sub(r"\bclass\b", "struct", rm.type)
                if rm.funcptr:
                    raise ValueError(f"{d.name}.{rm.name}: inline function pointer needs an explicit adapter")
            suffix = ""
            for dim in rm.dims:
                value = idx.eval_const(dim, d.file)
                if value is None:
                    raise ValueError(f"{d.name}.{rm.name}: unresolved array bound {dim}")
                suffix += f"[{value}]"
            if rm.bits is not None:
                width = idx.eval_const(rm.bits, d.file)
                if width is None:
                    raise ValueError(f"{d.name}.{rm.name}: unresolved bitfield width")
                suffix += f" : {width}"
            result.append(f"{decl} {'*' * rm.ptr}{rm.name or ''}{suffix};")
        return result

    def emit(d):
        if d.uid in emitted:
            return
        if d.uid in busy:
            raise ValueError(f"by-value dependency cycle: {d.name}")
        busy.add(d.uid)
        def deps(obj):
            for base in obj.bases:
                yield idx.find_struct(base, obj.file)
            for rm in obj.raw:
                if rm.inline:
                    yield from deps(rm.inline)
                elif not rm.ptr and not rm.funcptr:
                    target = idx.find_struct(C.split_type(rm.type)[0], obj.file)
                    if target is not None:
                        yield target
        for dep in deps(d):
            if dep is not None:
                emit(dep)
        lines.extend([f"/* {d.file}:{d.line} */", f"#pragma pack(push, {d.pack})",
                      f"{d.kind} {d.tag or d.name} {{", *body(d), "};", "#pragma pack(pop)"])
        busy.remove(d.uid)
        emitted.add(d.uid)
    for d in defs:
        emit(d)
    used = set(re.findall(r"\w+", "\n".join(lines[1:])))
    sdk = [name for name in WINDOWS_TYPES if name in used and name not in names]
    lines[1:1] = [WINDOWS_TYPES[name] for name in sdk]
    for name in sdk:
        # The program's Windows archive files these under /WinDef.h.
        categories[name] = "WinDef.h"
        if "tag" + name in WINDOWS_TYPES[name]:
            categories["tag" + name] = "WinDef.h"
    idx.categories = categories
    Path(out).parent.mkdir(parents=True, exist_ok=True)
    Path(out).write_text("\n".join(lines) + "\n", encoding="utf-8")
    return idx, sorted(names)
