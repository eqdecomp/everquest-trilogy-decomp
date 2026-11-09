"""stackcmp.py / reccmp_map.py: stack operand parsing and hints on hand-made diffs (no game files, no VC6); an
end-to-end run of reccmp on two tiny VC6 /Zi DLLs when VC6 and reccmp are installed."""
import os
import subprocess
import sys
from pathlib import Path
from types import SimpleNamespace

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

import reccmp_map as M  # noqa: E402
import stackcmp as S  # noqa: E402


# --- reccmp_map ------------------------------------------------------------------------------------------------

SRC = """#include "x.h"

// FUNCTION: TRILOGY 0x004197a7
// ORIGINAL FILE: Source/EQ_Character.cpp (address range)
// __thiscall. Bit 0 of the flags.
int EQ_Character::Flag1(void)
{
    return 1;
}

// FUNCTION: TRILOGY 0x004197b2
int EQ_Character::Flag2(void)
{
    return 2; // FUNCTION: not a marker
}
"""


def test_reccmp_text_moves_markers_next_to_the_definition():
    out = M.reccmp_text(SRC)
    a, b = SRC.split("\n"), out.split("\n")
    assert len(a) == len(b)
    for x, y in zip(a, b):                       # code lines never move
        if x.strip() and not x.strip().startswith("//"):
            assert x == y
    i = b.index("int EQ_Character::Flag1(void)")
    assert b[i - 1] == "// FUNCTION: TRILOGY 0x004197a7" and b[i - 2] == b[i - 3] == ""
    assert "ORIGINAL FILE" not in out
    j = b.index("int EQ_Character::Flag2(void)")
    assert b[j - 1] == "// FUNCTION: TRILOGY 0x004197b2"


def test_reccmp_parser_reads_the_mapped_markers_by_line():
    pytest.importorskip("reccmp")
    from reccmp.parser.parser import DecompParser
    p = DecompParser()
    p.reset_and_set_filename("x.cpp")
    p.read(SRC)
    raw = {s.offset: s for s in p.iter_symbols("TRILOGY")}
    assert raw[0x4197A7].lookup_by_name                     # the problem: comment read as the name
    p.reset_and_set_filename("x.cpp")
    p.read(M.reccmp_text(SRC))
    syms = {s.offset: s for s in p.iter_symbols("TRILOGY")}
    assert set(syms) == {0x4197A7, 0x4197B2}
    assert not any(s.lookup_by_name for s in syms.values())
    assert syms[0x4197A7].line_number == SRC.split("\n").index("int EQ_Character::Flag1(void)") + 1


# --- stack operands --------------------------------------------------------------------------------------------

@pytest.mark.parametrize("insn,want", [
    ("mov eax, dword ptr [ebp - 0x14]", ("ebp", -0x14, "")),
    ("mov ecx, dword ptr [ebp + eax*4 - 0x10]", ("ebp", -0x10, "eax*4")),
    ("lea ecx, [esp + 8]", ("esp", 8, "")),
    ("mov dword ptr [esp], eax", ("esp", 0, "")),
    ("mov dword ptr fs:[0], ecx", None),
    ("mov eax, dword ptr [ecx + 0x7c]", None),
    ("push ebp", None),
])
def test_stack_ref(insn, want):
    r = S.stack_ref(insn)
    assert (None if r is None else (r.reg, r.off, r.index)) == want


def diff(orig, recomp, codes=None):
    oi = [(f"0x{0x401000 + i:x}", t) for i, t in enumerate(orig)]
    ri = [(f"0x{0x10001000 + i:x}", t) for i, t in enumerate(recomp)]
    return SimpleNamespace(codes=codes or [("equal" if orig == recomp else "replace", 0, len(orig), 0, len(recomp))],
                           orig_inst=oi, recomp_inst=ri)


def test_analyse_same_frame():
    ins = ["push ebp", "mov ebp, esp", "sub esp, 0x10", "mov eax, dword ptr [ebp - 0x10]", "leave", "ret"]
    res = S.analyse(diff(ins, ins), {-0x10: ("a", "T_INT4[4]")})
    assert res["stack"] == "same" and res["complete"] and res["hints"] == []
    assert res["ebp"] == [{"orig": -16, "rewrite": -16, "kind": "same", "symbol": "a", "type": "T_INT4[4]"}]


def test_analyse_moved_slot_and_frame_size():
    o = ["push ebp", "mov ebp, esp", "sub esp, 0x18", "mov eax, dword ptr [ebp - 0x14]", "mov dword ptr [ebp - 4], eax"]
    r = ["push ebp", "mov ebp, esp", "sub esp, 0x14", "mov eax, dword ptr [ebp - 0x10]", "mov dword ptr [ebp - 4], eax"]
    codes = [("equal", 0, 2, 0, 2), ("replace", 2, 4, 2, 4), ("equal", 4, 5, 4, 5)]
    res = S.analyse(diff(o, r, codes), {-0x10: ("count", "T_INT4")})
    assert res["stack"] == "differs"
    assert res["frame"] == {"orig": 0x18, "rewrite": 0x14}
    moved = [e for e in res["ebp"] if e["kind"] == "moved"]
    assert moved == [{"orig": -0x14, "rewrite": -0x10, "kind": "moved", "symbol": "count", "type": "T_INT4"}]
    assert any("frame differs at ebp-0x14" in h and "ebp-0x10" in h and "`count`" in h for h in res["hints"])
    assert any(h.startswith("frame size differs") for h in res["hints"])


def test_analyse_conflict_and_esp_delta_and_structural():
    o = ["mov eax, dword ptr [ebp - 8]", "mov ecx, dword ptr [ebp - 8]", "mov edx, dword ptr [esp + 0xc]",
         "mov edx, dword ptr [esp + 0x10]", "nop"]
    r = ["mov eax, dword ptr [ebp - 8]", "mov ecx, dword ptr [ebp - 0xc]", "mov edx, dword ptr [esp + 0x10]",
         "mov edx, dword ptr [esp + 0x14]", "nop", "nop"]
    codes = [("replace", 0, 4, 0, 4), ("replace", 4, 5, 4, 6)]
    res = S.analyse(diff(o, r, codes), {})
    assert {e["kind"] for e in res["ebp"]} == {"conflict"}
    assert res["esp_deltas"] == {"+0x4": 2}
    assert any("esp operands off by +0x4 in 2 place(s)" in h for h in res["hints"])
    assert not res["complete"] and len(res["structural"]) == 1


def test_analyse_operand_only_on_one_side():
    o = ["cmp eax, 0x591d48"]
    r = ["cmp dword ptr [ebp - 4], 0x14"]
    res = S.analyse(diff(o, r), {-4: ("i", "T_INT4")})
    assert res["stack"] == "differs"
    assert res["operand_mismatch"][0]["rewrite"] == r[0]
    assert "only in the rewrite" in res["hints"][0] and "`i`" in res["hints"][0]


def test_read_nearmiss(tmp_path):
    f = tmp_path / "nearmiss.txt"
    f.write_text("# va\tndiff\tname\tsource\tflags\tfirst difference\n"
                 "0x004F4FA0\t5\tHeap_x\tsrc/Network/rdp/heap.c\t/W3 /O2\t#3: a | b\n")
    assert S.read_nearmiss(f) == [{"va": 0x4F4FA0, "ndiff": 5, "name": "Heap_x", "source": "src/Network/rdp/heap.c",
                                   "flags": "/W3 /O2", "first": "#3: a | b"}]


# --- end to end: reccmp on two VC6 /Zi DLLs -------------------------------------------------------------------

FN_A = """int __cdecl frame_fn(int x)
{
    int a[4];
    int b[4];
    int i;
    for (i = 0; i < 4; i++) { a[i] = x + i; b[i] = x - i; }
    return a[x & 3] + b[(x >> 2) & 3];
}
"""
FN_B = FN_A.replace("int a[4];\n    int b[4];", "int a[4];\n    int b[5];")


def _vc6():
    from eqd_unicorn.vc6 import DEFAULT_VC6
    cand = [os.environ.get("EQD_VC6")]
    try:
        from eqd_unicorn import Workspace
        cand.append(str(Workspace(ROOT).vc6()))
    except Exception:  # noqa: BLE001
        pass
    cand.append(DEFAULT_VC6)
    for c in cand:
        if c and (Path(c) / "Bin" / "CL.EXE").is_file():
            return Path(c)
    return None


def _dll(vc6, d: Path, name: str, src: str):
    from eqd_unicorn.vc6 import vc6_env
    env = vc6_env(vc6)
    c = d / f"{name}.c"
    c.write_text(src)
    subprocess.run([str(vc6 / "Bin" / "CL.EXE"), "/nologo", "/c", "/O1", "/Zi", f"/Fd{d / (name + '_c.pdb')}",
                    f"/Fo{d / (name + '.obj')}", str(c)], env=env, cwd=d, check=True, capture_output=True)
    subprocess.run([str(vc6 / "Bin" / "LINK.EXE"), "/nologo", "/DLL", "/NOENTRY", "/DEBUG", "/PDBTYPE:CON",
                    "/INCREMENTAL:NO", f"/PDB:{d / (name + '.pdb')}", f"/OUT:{d / (name + '.dll')}",
                    "/EXPORT:frame_fn", str(d / (name + ".obj"))], env=env, cwd=d, check=True, capture_output=True)
    return d / f"{name}.dll", d / f"{name}.pdb"


def test_reccmp_stack_hints_on_vc6_dlls(tmp_path):
    pytest.importorskip("reccmp")
    import pefile
    vc6 = _vc6()
    if vc6 is None:
        pytest.skip("VC6 not found")
    a_dll, a_pdb = _dll(vc6, tmp_path, "orig", FN_A)
    b_dll, b_pdb = _dll(vc6, tmp_path, "rew", FN_B)
    assert a_pdb.read_bytes().startswith(b"Microsoft C/C++ program database 2.00")
    pe = pefile.PE(str(a_dll))
    va = pe.OPTIONAL_HEADER.ImageBase + pe.DIRECTORY_ENTRY_EXPORT.symbols[0].address
    csv_text = f"address,type,name,symbol\n{va:x},function,frame_fn,_frame_fn\n"
    same = S.StackCompare(a_dll, a_dll, a_pdb, csv_text, tmp_path / "m.csv").hints(va)
    assert same["stack"] == "same" and same["reccmp_accuracy"] == 1.0
    assert {e["symbol"] for e in same["ebp"]} >= {"a", "b"}
    res = S.StackCompare(a_dll, b_dll, b_pdb, csv_text, tmp_path / "m.csv").hints(va)
    assert res["stack"] == "differs", res
    assert res["frame"]["orig"] != res["frame"]["rewrite"]
    assert any(e["kind"] == "moved" and e["symbol"] in ("a", "b") for e in res["ebp"]), res["ebp"]
