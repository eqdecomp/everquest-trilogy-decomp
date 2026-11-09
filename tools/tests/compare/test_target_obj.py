"""compare.py --target-obj, coff placement and absolute symbols, on hand-made COFF objects (no game files, no VC6,
no Ghidra)."""
import struct
import sys
from pathlib import Path
from types import SimpleNamespace

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

from eqd_unicorn.pe import Image  # noqa: E402

import coff  # noqa: E402
import compare as C  # noqa: E402

FN, CALLEE, GLOBAL = 0x401000, 0x402000, 0x6E4DA8
TEXT = 0x60000020                                   # code | execute | read


def make_obj(path, sections, symbols):
    """sections: [(name, data, chars, [(offset, symbol index, type)])]; symbols: [(name, value, section,
    type, storage)] (no aux records). A plain COFF file as VC6 / the delinker write them."""
    nsec = len(sections)
    head = 20 + 40 * nsec
    blobs, offs, pos = [], [], head
    for name, data, chars, relocs in sections:
        rel = b"".join(struct.pack("<IIH", o, i, t) for o, i, t in relocs)
        offs.append((pos, pos + len(data) if relocs else 0))
        blobs.append(data + rel)
        pos += len(data) + len(rel)
    strtab = bytearray(4)
    syms = b""
    for name, value, sec, typ, storage in symbols:
        nb = name.encode()
        if len(nb) <= 8:
            raw = nb.ljust(8, b"\0")
        else:
            raw = b"\0\0\0\0" + struct.pack("<I", len(strtab))
            strtab += nb + b"\0"
        syms += raw + struct.pack("<IhHBB", value, sec, typ, storage, 0)
    struct.pack_into("<I", strtab, 0, len(strtab))
    out = struct.pack("<HHIIIHH", 0x14C, nsec, 0, pos, len(symbols), 0, 0)
    for (name, data, chars, relocs), (praw, prel) in zip(sections, offs):
        out += struct.pack("<8sIIIIIIHHI", name.encode(), 0, 0, len(data), praw, prel, 0, len(relocs), 0, chars)
    out += b"".join(blobs) + syms + bytes(strtab)
    Path(path).write_bytes(out)
    return path


# push 1; call CALLEE; mov eax, [GLOBAL]; add esp, 4; ret
def code(call_field: bytes, global_field: bytes) -> bytes:
    return b"\x6a\x01\xe8" + call_field + b"\xa1" + global_field + b"\x83\xc4\x04\xc3"


ORIG = code(struct.pack("<i", CALLEE - (FN + 7)), struct.pack("<I", GLOBAL))
CALL_OFF, GLOBAL_OFF = 3, 8


def delinked(tmp_path, relocs=((CALL_OFF, 1, 0x14), (GLOBAL_OFF, 2, 0x06)), global_name=f"_eqd_va_0x{GLOBAL:08X}",
             body=None):
    syms = [(f"_eqd_fn_0x{FN:08X}", 0, 1, 0x20, 2), (f"_eqd_fn_0x{CALLEE:08X}", 0, 0, 0x20, 2),
            (global_name, 0, 0, 0, 2)]
    body = body if body is not None else code(b"\0\0\0\0", b"\0\0\0\0")
    return make_obj(tmp_path / f"{FN:08X}.obj", [(".text", body, TEXT, list(relocs))], syms)


def rewrite(tmp_path):
    syms = [("_user", 0, 1, 0x20, 2), (f"_eqd_fn_0x{CALLEE:08X}", 0, 0, 0x20, 2),
            (f"_eqd_va_0x{GLOBAL:08X}", 0, 0, 0, 2)]
    return make_obj(tmp_path / "rw.obj", [(".text", code(b"\0\0\0\0", b"\0\0\0\0"), TEXT,
                                           [(CALL_OFF, 1, 0x14), (GLOBAL_OFF, 2, 0x06)])], syms)


def context(orig=ORIG):
    data = bytearray(0x4000)
    data[0x1000:0x1000 + len(orig)] = orig
    img = Image("o", 0x400000, data, 0x400000, code_ranges=[(0x401000, 0x2000)])
    ctx = object.__new__(C.MatchContext)
    ctx.o_side = C.Side(img)
    ctx.starts = {FN, CALLEE}
    ctx.o_side.funcs = {s: f"F:{s:x}" for s in ctx.starts}
    ctx.aliases, ctx.libnames, ctx.imp_by_name = {}, {}, {}
    ctx.repo = ctx.out_dir = Path(".")
    return ctx


def check(tmp_path, target):
    ann = SimpleNamespace(va=FN, name="user", line=1)
    (res,) = context().compare_obj(rewrite(tmp_path), Path("rw.c"), [ann], {}, target=target)
    assert C.match_status(res) == "identical", res.diff
    return res.target


def test_placement_and_link_reproduce_the_original(tmp_path):
    obj = coff.parse(delinked(tmp_path))
    assert coff.delinked_placement(obj) == {1: FN}
    lk = coff.link(obj, coff.delinked_placement(obj))
    assert bytes(lk.data[FN - lk.base:FN - lk.base + len(ORIG)]) == ORIG
    assert FN + CALL_OFF in lk.rel32 and FN + GLOBAL_OFF not in lk.rel32


def test_identical_function_passes(tmp_path):
    t = check(tmp_path, delinked(tmp_path))
    assert t["ok"] and t["bytes_ok"] and t["relocs"] == 2, t


def test_folder_of_objects_and_missing_object(tmp_path):
    delinked(tmp_path)
    assert check(tmp_path, tmp_path)["ok"]
    assert check(tmp_path, tmp_path / "nothing")["status"] == "missing"


def test_missing_relocation_is_reported(tmp_path):
    # no relocation on the global: the field keeps the absolute address, so the bytes still agree
    body = code(b"\0\0\0\0", struct.pack("<I", GLOBAL))
    t = check(tmp_path, delinked(tmp_path, relocs=[(CALL_OFF, 1, 0x14)], body=body))
    assert not t["ok"] and t["bytes_ok"] and t["missing"] == [f"+{GLOBAL_OFF:#x}"], t


def test_wrong_relocation_target_breaks_the_bytes(tmp_path):
    t = check(tmp_path, delinked(tmp_path, global_name=f"_eqd_va_0x{GLOBAL + 4:08X}"))
    assert not t["ok"] and not t["bytes_ok"] and t["bad_bytes"], t


def test_unit_object_with_packed_ranges_is_refused(tmp_path):
    syms = [(f"_eqd_fn_0x{FN:08X}", 0, 1, 0x20, 2), (f"_eqd_fn_0x{CALLEE:08X}", 0x10, 1, 0x20, 2)]
    obj = coff.parse(make_obj(tmp_path / "u.obj", [(".text", bytes(0x20), TEXT, [])], syms))
    with pytest.raises(ValueError):
        coff.delinked_placement(obj)


def test_except_list_is_the_absolute_zero_of_fs(tmp_path):
    # VC6 C++ EH prologue reads `mov eax, fs:__except_list`: an external that LIBC defines as absolute 0.
    seh = bytes.fromhex("64a100000000c3")                         # mov eax, fs:[0]; ret
    syms = [("_user", 0, 1, 0x20, 2), ("__except_list", 0, 0, 0, 2)]
    obj = make_obj(tmp_path / "seh.obj", [(".text", seh, TEXT, [(2, 1, 0x06)])], syms)
    lk = coff.link(coff.parse(obj))
    assert bytes(lk.data[:len(seh)]) == seh and lk.base + 2 not in lk.relocs
    ann = SimpleNamespace(va=FN, name="user", line=1)
    (res,) = context(seh).compare_obj(obj, Path("seh.c"), [ann], {})
    assert res.verdict == "identical" and C.match_status(res) == "identical", res.diff
    abs_obj = make_obj(tmp_path / "abs.obj", [(".text", seh, TEXT, [(2, 1, 0x06)])],
                       [("_user", 0, 1, 0x20, 2), ("$abs", 0, -1, 0, 3)])
    assert bytes(coff.link(coff.parse(abs_obj)).data[:len(seh)]) == seh   # IMAGE_SYM_ABSOLUTE too
