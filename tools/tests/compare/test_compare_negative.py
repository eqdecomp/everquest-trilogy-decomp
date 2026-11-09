"""Negative controls for `compare.py --match` (the tooling review): before "identical" can be trusted, the
judge has to say "not identical" on planted bad cases. Hand-assembled cases need nothing; the end-to-end case
builds one VC6 object (skipped without VC6) and plants one wrong byte in a copy used as the "original".
"""
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


def sides(o_code, r_code, r_relocs=()):
    od, rd = bytearray(0x4000), bytearray(0x4000)
    od[0x1000:0x1000 + len(o_code)] = o_code
    rd[0x1000:0x1000 + len(r_code)] = r_code
    o = C.Side(Image("o", O_BASE, od, O_BASE, code_ranges=[(O_FN, 0x2000)]))
    o.funcs = {CALLEE: f"F:{CALLEE:x}"}
    r = C.Side(Image("r", R_BASE, rd, R_BASE, code_ranges=[(R_FN, 0x1000)]), relocs=set(r_relocs),
               is_rewrite=True, orig=o)
    return o, r


def status(o_code, r_code, r_relocs=()):
    res = C.compare_function(*sides(o_code, r_code, r_relocs), O_FN, R_FN, frozenset({CALLEE}), frozenset())
    return C.match_status(res)


MOV_ADD = b"\x8b\x44\x24\x04\x83\xc0\x01\xc3"          # mov eax, [esp+4]; add eax, 1; ret


def test_control_identical_passes():
    assert status(MOV_ADD, MOV_ADD) == "identical"


@pytest.mark.parametrize("planted, want", [
    (b"\x8b\x44\x24\x04\x83\xc0\x02\xc3", "diff"),                 # wrong immediate
    (b"\x8b\x44\x24\x08\x83\xc0\x01\xc3", "diff"),                 # wrong stack slot
    (b"\x8b\x4c\x24\x04\x83\xc1\x01\x8b\xc1\xc3", "structural"),   # extra instruction
    (b"\x8b\x44\x24\x04\x05\x01\x00\x00\x00\xc3", "normalised"),   # same instruction, other encoding
])
def test_planted_bad_case_is_not_identical(planted, want):
    assert status(MOV_ADD, planted) == want


def test_wrong_global_is_not_masked_as_a_relocation():
    # mov eax, [0x6E4DA8] vs [0x6E4DAC]: unrelocated fixed client addresses are compared, never masked
    o = b"\xa1" + struct.pack("<I", 0x6E4DA8) + b"\xc3"
    assert status(o, o) == "identical"
    assert status(o, b"\xa1" + struct.pack("<I", 0x6E4DAC) + b"\xc3") == "diff"


MATCH_SRC = r"""
void __cdecl eqd_fn_0x00402000(int);
extern char eqd_va_0x006E4DA8;
int user(int a)
{
    eqd_fn_0x00402000(a);
    return *(int *)&eqd_va_0x006E4DA8 + 1;
}
"""


def test_match_mode_object_against_a_planted_original(tmp_path):
    """A VC6 object laid out by coff.link is compared with a copy of itself (identical) and with copies that have
    one wrong byte: in an immediate, in the data address, in the call target. None may come out identical."""
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
    size = lk.func_sizes[entry]
    code = bytes(lk.data[entry - lk.base:entry - lk.base + size])

    def run(orig_code):
        od = bytearray(0x2000)
        od[0x1000:0x1000 + len(orig_code)] = orig_code
        o = C.Side(Image("o", O_BASE, od, O_BASE, code_ranges=[(O_FN, 0x1000)]))
        o.funcs = {0x402000: "F:402000"}
        img = Image("m.obj", lk.base, lk.data, lk.base, code_ranges=lk.code_ranges)
        r = C.Side(img, relocs=set(lk.relocs), is_rewrite=True, orig=o)
        r.rel32 = {f for f in lk.reloc_target if f not in lk.relocs}
        for f, n in lk.reloc_target.items():
            e = coff.eqd_va(n)
            if e and e[0] == "fn":
                r.funcs[e[1]] = f"F:{e[1]:x}"
        res = C.compare_function(o, r, O_FN, entry, frozenset({0x402000}), frozenset())
        return C.match_status(res), res

    def at_orig(c):
        c = bytearray(c)
        call = c.index(b"\xe8")
        c[call + 1:call + 5] = struct.pack("<i", 0x402000 - (O_FN + call + 5))
        return c, call

    good, call = at_orig(code)
    st, res = run(bytes(good))
    assert st == "identical", res.diff
    data = good.index(struct.pack("<I", 0x6E4DA8))
    inc = good.index(0x40, data + 4)                       # `+ 1` compiles to inc eax (0x40) after the load
    for what, off, flip in (("immediate (inc -> dec)", inc, 0x08), ("data address", data, 0x04),
                            ("call target", call + 1, 0x04)):
        bad = bytearray(good)
        bad[off] ^= flip
        st, res = run(bytes(bad))
        assert st != "identical", f"planted wrong {what} judged identical"
