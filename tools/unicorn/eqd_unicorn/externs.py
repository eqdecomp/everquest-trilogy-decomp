r"""`// EXTERNAL: <CLIENT> 0x<VA> <name>`: a function a source declares and calls but does not define, at a client VA.

A real class can declare its constructor (or any method or free function) without defining it, and a source can
then construct it the plain C++ way (`new Item_guess`, a local object): VC6 calls the declared symbol directly,
like the original calls the client's constructor, where a stand-in (a local wrapper class whose constructor calls
EQ_METHOD) adds a call level. The marker says where the declared function lives:

    class GroundItemNode_guess
    {
    public:
        // EXTERNAL: TRILOGY 0x004A02E5 GroundItemNode_guess::GroundItemNode_guess
        GroundItemNode_guess(void);
        ...
    };

`name` is the qualified name as `undecorate` gives it (`Class::Class`, `Class::~Class`, `Class::Method`,
`function`), with the file's one-identifier `#define`s applied as for `// FUNCTION:` (annotations.py), or the
decorated symbol itself (`??0Item_guess@@QAE@H@Z`) when the object references several overloads of one name.
Only an object's UNDEFINED externals are mapped: a function some object of the build defines is its own.

Where markers count: the source file being compiled, and every header under the build's include directories
(the repo's `include/`). A source's own marker wins over a header's. Two markers of one name with different VAs
at the same level cancel each other (reported).

Who uses the map (decorated name -> VA):
  - compare.py --match (MatchContext.externals): the call is keyed `F:<va>`, like the original's call;
  - DLL builds (vc6.build_dll, the hook DLL): thiscall_stubs.write_stub_obj defines each mapped symbol as
    `mov eax, va; jmp eax`, so the test DLL links and calls the client function;
  - the fixture harness (difftest): a call to such a stub is a call to the client function at va.

A marker can also name a class's vftable, by its decorated symbol:

    // EXTERNAL: TRILOGY 0x00535430 ??_7EQ_Requestor@@6B@

VC6 defines `??_7Class@@6B@` (a COMDAT) in every object whose constructor or destructor stores it, so the map
covers defined symbols here, and the store must read the client's address, not the object's own table:
  - compare.py --match: coff.link resolves the reference to va, like an `eqd_va_` symbol;
  - DLL builds: patch_vftable_refs writes va into each reference and voids its relocation.
The vftable's VA is not a function VA (compare.match_symbols leaves it out of the generated header). The object's
own table stays, unused: its slots still have to link, so declare them pure (`__purecall`) or give them markers.
"""
from __future__ import annotations

import re
import struct
from dataclasses import dataclass
from pathlib import Path

from .pe import undecorate

_EXT = re.compile(r"^\s*//\s*EXTERNAL:\s*(\w+)\s+(0x[0-9a-fA-F]+)\s+(\S+)\s*$")
_DEFINE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*(?://.*)?$")
_UNDEF = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)")
HEADER_SUFFIXES = (".h", ".hpp")
STUB_LEN = 8                    # thiscall_stubs: B8 <va> FF E0 90
REL_I386_ABSOLUTE, REL_I386_DIR32 = 0x00, 0x06


@dataclass(frozen=True)
class External:
    client: str
    va: int
    name: str                   # qualified undecorated name, or a decorated symbol (starts with '?')
    path: str
    line: int


def scan(paths, client: str | None = None) -> list[External]:
    """The EXTERNAL markers of `paths` (all clients, or `client`'s)."""
    out = []
    for p in paths:
        text = Path(p).read_text(encoding="utf-8", errors="replace")
        if "EXTERNAL:" not in text:
            continue
        defines = {}
        for i, ln in enumerate(text.splitlines()):
            if d := _DEFINE.match(ln):
                defines[d.group(1)] = d.group(2)
            elif u := _UNDEF.match(ln):
                defines.pop(u.group(1), None)
            m = _EXT.match(ln)
            if not m or (client and m.group(1).upper() != client.upper()):
                continue
            name = m.group(3)
            if not name.startswith("?"):
                name = "::".join(defines.get(part, part) for part in name.split("::"))
            out.append(External(m.group(1).upper(), int(m.group(2), 16), name, str(p), i + 1))
    return out


_HEADER_CACHE: dict = {}


def header_markers(include_dirs, client: str | None = None) -> list[External]:
    """The markers of every header under `include_dirs` (cached per file by size and mtime)."""
    out = []
    for d in include_dirs:
        for p in sorted(Path(d).rglob("*")):
            if p.suffix.lower() not in HEADER_SUFFIXES or not p.is_file():
                continue
            st = p.stat()
            key = (str(p), st.st_size, st.st_mtime_ns)
            if key not in _HEADER_CACHE:
                _HEADER_CACHE[key] = scan([p])
            out += [e for e in _HEADER_CACHE[key] if not client or e.client == client.upper()]
    return out


def _level(markers) -> tuple[dict, list]:
    """{name: va} of one level of markers; a name with two VAs is dropped and reported."""
    vas: dict = {}
    for e in markers:
        vas.setdefault(e.name, {}).setdefault(e.va, e)
    out, problems = {}, []
    for name, by_va in vas.items():
        if len(by_va) == 1:
            out[name] = next(iter(by_va.values()))
        else:
            where = ", ".join(f"{Path(e.path).name}:{e.line} {e.va:#x}" for e in by_va.values())
            problems.append(f"EXTERNAL {name}: markers disagree ({where}); not mapped")
    return out, problems


def resolve(names, own=(), headers=()) -> tuple[dict, list]:
    """{decorated name: va} for the undefined external symbols `names` of one object, from its source's own
    markers (`own`) over the headers' (`headers`); and the problems (conflicts, ambiguous names)."""
    hdr, problems = _level(headers)
    mine, p2 = _level(own)
    problems += p2
    marks = {**{n: e.va for n, e in hdr.items()}, **{n: e.va for n, e in mine.items()}}
    out = {}
    by_plain: dict = {}
    for s in names:
        if s in marks:                      # a decorated marker
            out[s] = marks[s]
        else:
            by_plain.setdefault(undecorate(s), []).append(s)
    for name, va in marks.items():
        if name.startswith("?"):
            continue
        hits = [s for s in by_plain.get(name, ()) if s not in out]
        if len(hits) == 1:
            out[hits[0]] = va
        elif len(hits) > 1:
            problems.append(f"EXTERNAL {name}: {len(hits)} referenced overloads ({', '.join(sorted(hits))}); "
                            f"give the decorated name")
    return out, problems


def undefined_externals(obj) -> set:
    """Undefined external symbols of a COFF object (a path or bytes)."""
    from .thiscall_stubs import _symbols
    have, want = set(), set()
    for name, sec, cls in _symbols(obj):
        if cls != 2:
            continue
        (want if sec == 0 else have).add(name)
    return want - have


def build_map(objs_by_source, include_dirs=(), client: str | None = None) -> tuple[dict, list]:
    """{decorated name: va} over a DLL build's objects ({source: obj}): each object's undefined externals that its
    source's markers or the include directories' headers map, minus the symbols any object of the build defines."""
    from .thiscall_stubs import _symbols
    headers = header_markers(include_dirs, client)
    own = {src: scan([src], client) for src in objs_by_source}
    objs_by_source = {s: o for s, o in objs_by_source.items() if Path(o).is_file()}
    if not headers and not any(own.values()):
        return {}, []
    defined = set()
    out, problems = {}, []
    for src, obj in objs_by_source.items():
        for name, sec, cls in _symbols(obj):
            if cls == 2 and sec > 0:
                defined.add(name)
    for src, obj in objs_by_source.items():
        m, p = resolve(undefined_externals(obj), own[src], headers)
        problems += p
        for k, v in m.items():
            if out.setdefault(k, v) != v:
                problems.append(f"EXTERNAL {k}: {out[k]:#x} and {v:#x} in different sources; not mapped")
                out[k] = None
    return {k: v for k, v in out.items() if v is not None and k not in defined}, problems


def is_vftable(sym: str) -> bool:
    return sym.startswith("??_7")


def patch_vftable_refs(objs_by_source, include_dirs=(), client: str | None = None) -> list:
    """For a DLL build ({source: obj}): write the marked VA of each vftable into the fields that reference it (DIR32
    relocations in any section) and void those relocations (IMAGE_REL_I386_ABSOLUTE), so the code stores the
    client's address, unrelocated, as the client does. Returns the problems (externs.resolve)."""
    from .thiscall_stubs import _symbols, symbol_indices
    headers = header_markers(include_dirs, client)
    problems = []
    for src, obj in objs_by_source.items():
        own = scan([src], client)
        if not Path(obj).is_file() or not (headers or own):
            continue
        names = {n for n, _s, c in _symbols(obj) if c == 2 and is_vftable(n)}
        if not names:
            continue
        vas, p = resolve(names, own, headers)
        problems += p
        if vas:
            _write_refs(Path(obj), {i: vas[n] for i, n in symbol_indices(obj).items() if n in vas})
    return problems


def _write_refs(path: Path, va_of_symbol: dict) -> None:
    """Patch the object at `path`: {symbol index: va}, see patch_vftable_refs."""
    data = bytearray(path.read_bytes())
    nsec, opt = struct.unpack_from("<H", data, 2)[0], struct.unpack_from("<H", data, 16)[0]
    for k in range(nsec):
        at = 20 + opt + 40 * k
        raw, relocs, nrel = (struct.unpack_from("<I", data, at + 20)[0], struct.unpack_from("<I", data, at + 24)[0],
                             struct.unpack_from("<H", data, at + 32)[0])
        for r in range(nrel):
            roff, si, typ = struct.unpack_from("<IIH", data, relocs + 10 * r)
            if si in va_of_symbol and typ == REL_I386_DIR32:
                struct.pack_into("<I", data, raw + roff, va_of_symbol[si])
                struct.pack_into("<H", data, relocs + 10 * r + 8, REL_I386_ABSOLUTE)
    path.write_bytes(data)


def stub_target(read, addr: int):
    """The client VA a generated stub at `addr` jumps to (`mov eax, va; jmp eax`), else None. `read(addr, n)`."""
    try:
        b = read(addr, STUB_LEN)
    except Exception:  # noqa: BLE001 - outside the image
        return None
    if len(b) >= 7 and b[0] == 0xB8 and b[5:7] == b"\xFF\xE0":
        return int.from_bytes(b[1:5], "little")
    return None


def marked(sym: str, markers) -> bool:
    """Whether a marker of `markers` names the symbol `sym` (decorated or by its undecorated name)."""
    u = undecorate(sym)
    return any(e.name == sym or e.name == u for e in markers)
