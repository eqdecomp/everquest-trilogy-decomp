r"""Definitions for EQ_METHOD's members in DLL builds (the Unicorn test DLL, the hook DLL).

EQ_METHOD (include/trilogy/eqthiscall.h) calls the client method at `va` as the member m of
eqd_tc<n><va, ...>, which C++ only declares: a real __thiscall call, the same code in every build. The match
build maps the symbol to va (compare/coff.py); a DLL build links this generated object, which defines each
such symbol as `mov eax, va; jmp eax`. (VC6 inlines any m defined in C++, even one with an __asm block, and
the inlined call can evaluate the arguments in another order; a template member cannot be naked.)

    write_stub_obj(objs, out, extra=None) -> out, or None when no object references one

The same stubs serve functions a source only declares under an `// EXTERNAL:` marker (externs.py): `extra`.
"""
import re
import struct
from pathlib import Path

_TC = re.compile(r"^\?m@\?\$eqd_tc\d+@\$0(?:([A-P]+)@|(\d))")
IMAGE_FILE_MACHINE_I386 = 0x14C
IMAGE_SYM_CLASS_EXTERNAL = 2
TEXT_CHARS = 0x60500020       # CNT_CODE | ALIGN_16BYTES | MEM_EXECUTE | MEM_READ


def stub_va(name: str):
    """The client VA in an eqd_tc member's decorated name, else None."""
    m = _TC.match(name)
    if not m:
        return None
    if m.group(1):
        return int("".join("0123456789ABCDEF"[ord(c) - 65] for c in m.group(1)), 16)
    return int(m.group(2)) + 1


def _symbols(path: Path):
    """[(name, section number, storage class)] of a COFF object's symbol table."""
    return [s[1:] for s in _indexed_symbols(path)]


def symbol_indices(path: Path) -> dict:
    """{symbol table index: name} of a COFF object (the index relocations use: aux records count)."""
    return {s[0]: s[1] for s in _indexed_symbols(path)}


def _indexed_symbols(path: Path):
    data = Path(path).read_bytes()
    _, _, _, symptr, nsyms, _, _ = struct.unpack_from("<HHIIIHH", data, 0)
    strtab = symptr + nsyms * 18
    out, i = [], 0
    while i < nsyms:
        raw = data[symptr + i * 18: symptr + i * 18 + 18]
        if raw[:4] == b"\0\0\0\0":
            off = struct.unpack_from("<I", raw, 4)[0]
            end = data.index(b"\0", strtab + off)
            name = data[strtab + off:end].decode("latin-1")
        else:
            name = raw[:8].rstrip(b"\0").decode("latin-1")
        _, sec, _, cls, naux = struct.unpack_from("<IhHBB", raw, 8)
        out.append((i, name, sec, cls))
        i += 1 + naux
    return out


def referenced(objs) -> dict:
    """{decorated name: va} for the eqd_tc members the objects reference and none defines."""
    want, have = {}, set()
    for o in objs:
        for name, sec, cls in _symbols(o):
            va = stub_va(name)
            if va is None or cls != IMAGE_SYM_CLASS_EXTERNAL:
                continue
            if sec == 0:
                want[name] = va
            else:
                have.add(name)
    return {n: v for n, v in want.items() if n not in have}


def write_stub_obj(objs, out, extra=None) -> Path | None:
    """`extra`: more {decorated name: va} to define the same way (externs.build_map: functions a source declares
    with an `// EXTERNAL:` marker)."""
    stubs = referenced(objs)
    stubs.update(extra or {})
    if not stubs:
        return None
    names = sorted(stubs)
    code = b"".join(b"\xB8" + struct.pack("<I", stubs[n]) + b"\xFF\xE0\x90" for n in names)   # 8 bytes each
    strtab = bytearray(4)
    syms = bytearray()
    syms += b".text\0\0\0" + struct.pack("<IhHBB", 0, 1, 0, 3, 1)          # section symbol + aux record
    syms += struct.pack("<IHHIHBBBBBB", len(code), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)[:18].ljust(18, b"\0")
    for k, n in enumerate(names):
        nm = n.encode("latin-1")
        if len(nm) <= 8:
            field = nm.ljust(8, b"\0")
        else:
            field = struct.pack("<II", 0, len(strtab))
            strtab += nm + b"\0"
        syms += field + struct.pack("<IhHBB", k * 8, 1, 0x20, IMAGE_SYM_CLASS_EXTERNAL, 0)
    struct.pack_into("<I", strtab, 0, len(strtab))
    nsyms = 2 + len(names)
    text_ptr = 20 + 40
    sym_ptr = text_ptr + len(code)
    head = struct.pack("<HHIIIHH", IMAGE_FILE_MACHINE_I386, 1, 0, sym_ptr, nsyms, 0, 0)
    sec = struct.pack("<8sIIIIIIHHI", b".text", 0, 0, len(code), text_ptr, 0, 0, 0, 0, TEXT_CHARS)
    out = Path(out)
    out.write_bytes(head + sec + code + bytes(syms) + bytes(strtab))
    return out
