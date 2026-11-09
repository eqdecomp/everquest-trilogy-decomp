"""compare.py normalisation on hand-assembled code (no game files, no VC6)."""
import struct
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

from eqd_unicorn.pe import Image  # noqa: E402

import compare as C  # noqa: E402

O_BASE, R_BASE = 0x400000, 0x10000000
O_FN, R_FN = 0x401000, 0x10001000
CALLEE = 0x402000


def rel(src_va, size, tgt):
    return struct.pack("<i", tgt - (src_va + size))


def sides(o_code, r_code, r_relocs=(), o_data=b"", r_data=b""):
    od = bytearray(0x4000)
    od[0x1000:0x1000 + len(o_code)] = o_code
    od[0x3000:0x3000 + len(o_data)] = o_data
    rd = bytearray(0x4000)
    rd[0x1000:0x1000 + len(r_code)] = r_code
    rd[0x3000:0x3000 + len(r_data)] = r_data
    o = C.Side(Image("o", O_BASE, od, O_BASE, code_ranges=[(O_FN, 0x2000)]))
    o.funcs = {CALLEE: f"F:{CALLEE:x}"}
    r = C.Side(Image("r", R_BASE, rd, R_BASE, code_ranges=[(R_FN, 0x1000)]), relocs=set(r_relocs),
               is_rewrite=True, orig=o)
    return o, r


def run(o, r):
    return C.compare_function(o, r, O_FN, R_FN, frozenset({CALLEE}), frozenset())


def orig_call():
    # push 1; call CALLEE; add esp, 4; ret
    return b"\x6a\x01" + b"\xe8" + rel(O_FN + 2, 5, CALLEE) + b"\x83\xc4\x04\xc3"


def test_same_code_is_identical():
    o, r = sides(orig_call(), b"\x6a\x01\xb8" + struct.pack("<I", CALLEE) + b"\xff\xd0\x83\xc4\x04\xc3")
    # rewrite: push 1; mov eax, CALLEE; call eax; add esp, 4; ret  (EQ_CDECL)
    res = run(o, r)
    assert res.verdict == "identical_normalised", res.diff
    assert res.folds == 1


def test_thiscall_dummy_edx_is_folded():
    # original: mov ecx, [0x6e0000]; push 1; call CALLEE; ret
    o_code = b"\x8b\x0d\x00\x00\x6e\x00\x6a\x01\xe8" + rel(O_FN + 8, 5, CALLEE) + b"\xc3"
    # rewrite: mov ecx, [0x6e0000]; xor edx, edx; push 1; mov eax, CALLEE; call eax; ret  (EQ_THISCALL)
    r_code = b"\x8b\x0d\x00\x00\x6e\x00\x33\xd2\x6a\x01\xb8" + struct.pack("<I", CALLEE) + b"\xff\xd0\xc3"
    res = run(*sides(o_code, r_code))
    assert res.verdict == "identical_normalised", res.diff
    assert res.folds == 2


def test_tail_jump_through_register_is_folded():
    o_code = b"\x6a\x01\x59\xe9" + rel(O_FN + 3, 5, CALLEE)              # push 1; pop ecx; jmp CALLEE
    r_code = b"\x6a\x01\xb8" + struct.pack("<I", CALLEE) + b"\x59\xff\xe0"  # push 1; mov eax, VA; pop ecx; jmp eax
    assert run(*sides(o_code, r_code)).verdict == "identical_normalised"


def test_xor_edx_that_is_used_is_kept():
    o_code = orig_call()
    # rewrite: xor edx, edx; push edx; ... a real difference (push edx instead of push 1)
    r_code = b"\x33\xd2\x52\xb8" + struct.pack("<I", CALLEE) + b"\xff\xd0\x83\xc4\x04\xc3"
    res = run(*sides(o_code, r_code))
    assert res.verdict == "differs"


def test_return_value_register_is_not_folded_away():
    """mov eax, VA; call eax; push eax: the push reads the return value, not VA."""
    o_code = b"\xe8" + rel(O_FN, 5, CALLEE) + b"\x50\xc3"                 # call CALLEE; push eax; ret
    r_code = b"\xb8" + struct.pack("<I", CALLEE) + b"\xff\xd0\x50\xc3"
    assert run(*sides(o_code, r_code)).verdict == "identical_normalised"


def test_register_allocation_difference_is_reported():
    o_code = b"\x8b\x44\x24\x04\xc3"                                     # mov eax, [esp+4]; ret
    r_code = b"\x8b\x4c\x24\x04\x8b\xc1\xc3"                             # mov ecx, [esp+4]; mov eax, ecx; ret
    res = run(*sides(o_code, r_code))
    assert res.verdict == "differs" and res.first_diff.startswith("#0")
    assert 0 < res.ratio < 1


@pytest.mark.parametrize("r_text, verdict", [(b"hello\0", "identical_normalised"), (b"hellO\0", "differs")])
def test_string_literal_compared_by_content(r_text, verdict):
    o_code = b"\x68" + struct.pack("<I", O_BASE + 0x3000) + b"\x59\xc3"     # push "hello"; pop ecx; ret
    r_code = b"\x68" + struct.pack("<I", R_BASE + 0x3000) + b"\x59\xc3"
    o, r = sides(o_code, r_code, r_relocs={R_FN + 1}, o_data=b"hello\0", r_data=r_text)
    res = run(o, r)
    assert res.verdict == verdict, res.diff
    assert res.data == (1 if verdict.startswith("identical") else 0)


def test_float_constant_compared_by_bytes():
    o_code = b"\xd9\x05" + struct.pack("<I", O_BASE + 0x3000) + b"\xc3"     # fld dword ptr [const]; ret
    r_code = b"\xd9\x05" + struct.pack("<I", R_BASE + 0x3000) + b"\xc3"
    one = struct.pack("<f", 1.5)
    res = run(*sides(o_code, r_code, r_relocs={R_FN + 2}, o_data=one, r_data=one))
    assert res.verdict == "identical_normalised"
    res = run(*sides(o_code, r_code, r_relocs={R_FN + 2}, o_data=one, r_data=struct.pack("<f", 2.5)))
    assert res.verdict == "differs"


def test_identical_as_is():
    code = b"\x8b\x44\x24\x04\x40\xc3"
    res = run(*sides(code, code))
    assert res.verdict == "identical" and res.folds == 0


# --- match mode: a VC6 object with EQD_MATCH-style symbols, laid out by coff.link -----------------------------

MATCH_SRC = r"""
#ifdef __cplusplus
extern "C" {
#endif
void __cdecl eqd_fn_0x00402000(void);
extern char eqd_va_0x006E4DA8;
#ifdef __cplusplus
}
#endif
int user(int a)
{
    ((void (__cdecl *)(int))eqd_fn_0x00402000)(a);
    return *(int *)&eqd_va_0x006E4DA8 + 1;
}
"""


def test_coff_link_puts_eqd_symbols_at_their_vas(tmp_path):
    import os
    import subprocess
    import coff
    from eqd_unicorn.vc6 import DEFAULT_CFLAGS, DEFAULT_VC6, vc6_env
    vc6 = Path(os.environ.get("EQD_VC6") or DEFAULT_VC6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip(f"VC6 not found at {vc6}")
    src = tmp_path / "m.c"
    src.write_text(MATCH_SRC)
    obj = tmp_path / "m.obj"
    subprocess.run([str(vc6 / "Bin" / "CL.EXE")] + DEFAULT_CFLAGS + [f"/Fo{obj}", str(src)], env=vc6_env(vc6),
                   check=True, capture_output=True, cwd=tmp_path)
    lk = coff.link(coff.parse(obj))
    entry = lk.by_name["_user"]
    code = bytes(lk.data[entry - lk.base:entry - lk.base + 32])
    call = code.index(b"\xe8")
    tgt = entry + call + 5 + struct.unpack_from("<i", code, call + 1)[0]
    assert tgt == 0x402000                                   # call rel32 straight to the client VA
    assert struct.pack("<I", 0x6E4DA8) in code               # absolute operand = the client VA
    assert entry + call + 1 in lk.reloc_target and entry + call + 1 not in lk.relocs
    assert coff.eqd_va("_eqd_fn_0x00402000") == ("fn", 0x402000)


def test_branch_labels_ignore_folded_instructions():
    # original: test eax, eax; je L; mov ecx, [0x6e0000]; push 1; call CALLEE; L: ret
    o_code = b"\x85\xc0\x74\x0d\x8b\x0d\x00\x00\x6e\x00\x6a\x01\xe8" + rel(O_FN + 12, 5, CALLEE) + b"\xc3"
    # rewrite (EQ_THISCALL): test; je L; mov ecx, [..]; xor edx, edx; push 1; mov eax, CALLEE; call eax; L: ret
    r_code = (b"\x85\xc0\x74\x11\x8b\x0d\x00\x00\x6e\x00\x33\xd2\x6a\x01\xb8" + struct.pack("<I", CALLEE)
              + b"\xff\xd0\xc3")
    res = run(*sides(o_code, r_code))
    assert res.verdict == "identical_normalised", res.diff
    assert res.folds == 2


def test_prefix_and_first_mismatch_offset():
    o_code = b"\x55\x8b\x44\x24\x08\xc3"                                 # push ebp; mov eax, [esp+8]; ret
    r_code = b"\x55\x8b\x4c\x24\x08\x8b\xc1\xc3"                         # push ebp; mov ecx, [esp+8]; mov eax, ecx; ret
    res = run(*sides(o_code, r_code))
    assert res.prefix == 1 and res.first_diff_at == 1
    same = run(*sides(o_code, o_code))
    assert same.prefix == 3 and same.first_diff_at is None


def test_match_symbols_counts_dereference_lines_but_not_comments(tmp_path):
    (tmp_path / "src").mkdir()
    (tmp_path / "src" / "a.cpp").write_text(
        "/* EQ_CDECL(void, 0x00400001, (void))\n * EQ_GLOBAL(int, 0x00600001) */\n// EQ_GLOBAL(int, 0x00600002)\n"
        "void f(float* p)\n{\n    *(float*)p = EQ_ARRAY(float, 0x00600003)[1];\n"
        "    *p = EQ_CDECL(float, 0x00400002, (void))();\n}\n")
    fns, data, problems = C.match_symbols(tmp_path)
    assert (fns, data, problems) == ({"0x00400002"}, {"0x00600003"}, [])
