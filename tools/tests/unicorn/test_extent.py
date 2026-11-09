r"""extent.function_extent on hand-assembled switches: the bound must come from the compare right before the
jump, never from stale state (no game files needed).

Run: .venv\Scripts\python.exe -m pytest eq-trilogy\tools\tests\unicorn\test_extent.py
"""
import struct
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn.extent import ExtentError, function_extent  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402

BASE = 0x00500000
NCASES = 6
CASE = 6                        # mov eax, imm32; ret


def build(pre: bytes, entries=NCASES, bytetab: bytes = b"", base=BASE):
    """pre + `jmp [eax*4 + table]` + NCASES case blocks + the dword table (entries -> cases 0..) + byte table
    + a final `ret` (the default)."""
    jmp_va = base + len(pre)
    cases = jmp_va + 7
    table = cases + CASE * NCASES
    bt = table + 4 * entries
    ret = bt + len(bytetab)
    pre = pre.replace(b"{RET}", b"\x00")
    # patch the (single) rel8 placeholder: the byte before it is the jcc opcode; rel8 is relative to its end
    code = bytearray(pre)
    for i, c in enumerate(pre):
        pass
    code += b"\xFF\x24\x85" + struct.pack("<I", table)
    for i in range(NCASES):
        code += b"\xB8" + struct.pack("<I", i) + b"\xC3"
    code += b"".join(struct.pack("<I", cases + CASE * (i % NCASES)) for i in range(entries))
    code += bytetab + b"\xC3"
    return bytes(code), dict(jmp=jmp_va, cases=[cases + CASE * i for i in range(NCASES)], table=table, bt=bt,
                             ret=ret)


def jcc(op: int, base_off: int, target_off: int) -> bytes:
    return bytes([op, (target_off - base_off - 2) & 0xFF])


def run(code, **kw):
    img = Image.from_bytes("t", BASE, code)
    sw = []
    ext = function_extent(img.read, BASE, in_image=img.contains, switches=sw, **kw)
    return ext, sw


def cases_followed(ext, lay):
    return [c in ext for c in lay["cases"]]


def with_ja(pre_before: bytes, pre_after: bytes, entries=NCASES, bytetab=b""):
    """pre_before + ja <default ret> + pre_after, with the ja target patched once the layout is known."""
    pre = pre_before + b"\x77\x00" + pre_after
    code, lay = build(pre, entries, bytetab)
    ja_at = len(pre_before)
    code = bytearray(code)
    code[ja_at + 1] = (lay["ret"] - BASE - ja_at - 2) & 0xFF
    return bytes(code), lay


def test_cmp_ja_bounds_the_table():
    code, lay = with_ja(b"\x83\xF8\x02", b"")                       # cmp eax,2; ja default
    ext, sw = run(code)
    assert cases_followed(ext, lay) == [True] * 3 + [False] * 3
    assert sw == [(lay["jmp"], lay["table"], 3, "cmp")]


def test_bound_survives_unrelated_instructions_but_not_a_call():
    code, lay = with_ja(b"\x83\xF8\x02", b"\x90\x89\xC1")           # nop; mov ecx,eax between
    assert run(code)[1][0][3] == "cmp"
    call = b"\xE8\x00\x00\x00\x00"                                       # call $+5 (returns to the next insn)
    code, lay = with_ja(b"\x83\xF8\x02", call)
    ext, sw = run(code)
    assert sw[0][3] == "scan" and sw[0][2] == NCASES                     # the call forgot the bound


def test_reg_reg_cmp_forgets_the_stale_bound():
    # cmp eax,2; ja L; cmp eax,ecx (no immediate); jmp [eax*4+t]: the old code used 3 entries
    code, lay = with_ja(b"\x83\xF8\x02", b"\x39\xC8")
    ext, sw = run(code)
    assert sw[0][3] == "scan" and sw[0][2] == NCASES
    assert cases_followed(ext, lay) == [True] * NCASES


def test_cmp_minus1_je_is_no_bound():
    # cmp eax,-1; je L; jmp [eax*4+t]  (VC6 omits the range check): scan the table instead of 0xFFFFFFFF+1
    pre = b"\x83\xF8\xFF\x74\x00"
    code, lay = build(pre)
    code = bytearray(code)
    code[4] = (lay["ret"] - BASE - 3 - 2) & 0xFF
    ext, sw = run(bytes(code))
    assert sw[0][3] == "scan" and sw[0][2] == NCASES
    assert cases_followed(ext, lay) == [True] * NCASES


def test_scan_stops_at_a_foreign_entry_and_at_function_starts():
    code, lay = build(b"", entries=NCASES + 2)
    code = bytearray(code)
    struct.pack_into("<I", code, lay["table"] - BASE + 4 * NCASES, 0x00900000)      # outside the function
    ext, sw = run(bytes(code))
    assert sw[0][2] == NCASES
    img = Image.from_bytes("t", BASE, bytes(code))
    sw = []
    ext = function_extent(img.read, BASE, stops=frozenset({lay["cases"][2]}), in_image=img.contains,
                          switches=sw)
    assert sw[0][2] == 2 and lay["cases"][2] not in ext


def test_unboundable_table_raises_and_size_limits_it():
    code, lay = build(b"")
    code = bytearray(code)
    for i in range(NCASES):                                              # entries point outside: nothing to scan
        struct.pack_into("<I", code, lay["table"] - BASE + 4 * i, 0x00900000 + i)
    with pytest.raises(ExtentError, match="no bound"):
        run(bytes(code))
    ext, _ = run(bytes(code)[:0] or b"\xC3" + bytes(code[1:]), size=0x20)       # `size` is the escape hatch
    assert ext == {BASE}


def test_byte_index_table_after_the_bound():
    # cmp eax,5; ja default; movzx eax, byte [eax + bt]; jmp [eax*4 + t]  -> max(bt)+1 = 3 entries
    bt = bytes([0, 1, 1, 2, 0, 2])
    code, lay = with_ja(b"\x83\xF8\x05", b"\x0F\xB6\x80" + b"\0\0\0\0", bytetab=bt)
    code = bytearray(code)
    # the byte table is after the dword table, whose position depends on the layout: patch the disp32
    struct.pack_into("<I", code, 3 + 2 + 3, lay["bt"])
    ext, sw = run(bytes(code))
    assert sw == [(lay["jmp"], lay["table"], 3, "bytetable")]
    assert cases_followed(ext, lay) == [True] * 3 + [False] * 3


def test_struct_byte_before_or_outside_the_image_is_not_a_table():
    # mov al,[esi+0x8FBAC] before the compare (stale struct byte), then cmp eax,2; ja; jmp [eax*4+t]
    code, lay = with_ja(b"\x8A\x86" + struct.pack("<I", 0x8FBAC) + b"\x83\xF8\x02", b"")
    ext, sw = run(code)
    assert sw[0][2:] == (3, "cmp")
    # a byte load after the bound whose disp is not an image address (a struct field) is ignored too
    code, lay = with_ja(b"\x83\xF8\x02", b"\x0F\xB6\x86" + struct.pack("<I", 0x6C592))
    img = Image.from_bytes("t", BASE, code)
    sw = []
    function_extent(img.read, BASE, in_image=img.contains, switches=sw)
    assert sw[0][2:] == (3, "cmp")


def test_byte_table_must_load_the_index_register():
    # movzx ecx, byte [eax+bt]: the jump indexes eax, so the byte table is not its index table
    bt = bytes([0, 1, 1, 2, 0, 2])
    code, lay = with_ja(b"\x83\xF8\x05", b"\x0F\xB6\x88" + b"\0\0\0\0", bytetab=bt)
    code = bytearray(code)
    struct.pack_into("<I", code, 3 + 2 + 3, lay["bt"])
    ext, sw = run(bytes(code))
    assert sw[0][2:] == (6, "cmp")


def test_max_insns_default_covers_the_largest_function():
    import inspect
    assert inspect.signature(function_extent).parameters["max_insns"].default >= 100000
