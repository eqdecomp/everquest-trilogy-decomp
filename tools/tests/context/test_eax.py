r"""EAX (return value) evidence in the context packer: synthetic listings, plus one check on the export.

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\context\test_eax.py
"""
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "context"))
import pack_context as pc   # noqa: E402


def listing(*texts, start=0x401000):
    """[[addr, bytes, text, sym]] with consecutive fake addresses (4 bytes each)."""
    return [[f"{start + 4 * i:08X}", "90", t, t] for i, t in enumerate(texts)]


def after(*texts):
    """eax_after_call on `CALL f` followed by texts."""
    return pc.eax_after_call(listing("CALL 0x00402000", *texts), 0)


class FakeEx:
    def __init__(self, listings):
        self.listings = listings                       # {va: instructions}
        self.functions = {va: {"name": va} for va in listings}

    def asm(self, va):
        return {"instructions": self.listings[va]}


@pytest.mark.parametrize("texts,kind,reg", [
    (("ADD ESP,0x4", "TEST EAX,EAX", "JZ 0x00401100"), "read", "EAX"),
    (("POP ECX", "MOV dword ptr [ESI + 0x8],EAX"), "read", "EAX"),
    (("PUSH EAX", "CALL 0x00403000"), "read", "EAX"),
    (("MOV ECX,dword ptr [EAX + 0x4]",), "read", "EAX"),          # dereference
    (("MOV ECX,EAX",), "read", "EAX"),
    (("CMP EAX,0x5",), "read", "EAX"),
    (("CDQ",), "read", "EAX"),                                    # implicit read
    (("MOV EAX,dword ptr [EAX]",), "read", "EAX"),
    (("CALL dword ptr [EAX + 0x4]",), "read", "EAX"),
    (("ADD ESP,0x8", "TEST AL,AL", "JNZ 0x00401100"), "read", "AL"),
    (("MOVZX ECX,AL",), "read", "AL"),
    (("MOVZX EAX,AL",), "read", "AL"),
    (("AND EAX,0xff",), "read", "AL"),
    (("MOV word ptr [ESI],AX",), "read", "AX"),
])
def test_caller_reads(texts, kind, reg):
    how = after(*texts)
    assert how.startswith(f"{kind}:{reg}:"), how


@pytest.mark.parametrize("texts", [
    ("ADD ESP,0x4", "MOV EAX,dword ptr [ESI + 0x4]"),
    ("XOR EAX,EAX",),
    ("POP ECX", "CALL 0x00403000"),                               # another call clobbers it
    ("POP EAX",),
    ("LEA EAX,[EBP + -0x1c]", "PUSH EAX"),
    ("SETZ AL",),
    ("OR EAX,0xffffffff",),
])
def test_caller_overwrites(texts):
    assert after(*texts).startswith("over:")


def test_caller_inconclusive():
    assert after("POP ESI", "RET").startswith("ret:")                 # EAX passes through
    assert after("ADD ESP,0x4", "JNZ 0x00401100").startswith("branch:")
    assert after("JMP dword ptr [ECX*0x4 + 0x401200]").startswith("branch:")
    assert after("FSTP dword ptr [ESI]").startswith("fpu:")           # float return in ST0
    assert after("FLD dword ptr [ESI]", "FSTP dword ptr [EDI]", "XOR EAX,EAX").startswith("over:")
    assert after("NOP") == "none"


def test_caller_follows_one_jmp():
    ins = listing("CALL 0x00402000", "JMP 0x0040100c", "XOR EAX,EAX", "PUSH EAX")
    assert pc.eax_after_call(ins, 0) == "read:EAX:0040100C:PUSH EAX"


def test_callee_side_00401366():
    # the small-block allocator: every RET follows a call result or a load into EAX
    ins = [
        ["00401366", "", "MOV EDX,dword ptr [ESP + 0x4]", ""], ["0040136A", "", "CMP EDX,0x80", ""],
        ["00401370", "", "JBE 0x0040137a", ""], ["00401372", "", "PUSH EDX", ""],
        ["00401373", "", "CALL 0x005218c6", ""], ["00401378", "", "POP ECX", ""], ["00401379", "", "RET", ""],
        ["0040137A", "", "LEA EAX,[EDX + -0x1]", ""], ["0040137D", "", "SHR EAX,0x3", ""],
        ["00401380", "", "LEA ECX,[EAX*0x4 + 0x58fb58]", ""],
        ["00401387", "", "MOV EAX,dword ptr [EAX*0x4 + 0x58fb58]", ""], ["0040138E", "", "TEST EAX,EAX", ""],
        ["00401390", "", "JZ 0x00401397", ""], ["00401392", "", "MOV EDX,dword ptr [EAX]", ""],
        ["00401394", "", "MOV dword ptr [ECX],EDX", ""], ["00401396", "", "RET", ""],
        ["00401397", "", "PUSH EDX", ""], ["00401398", "", "CALL 0x0040152f", ""],
        ["0040139D", "", "POP ECX", ""], ["0040139E", "", "RET", ""],
    ]
    assert pc.eax_before_rets(ins) == [("00401379", "call"), ("00401396", "value"), ("0040139E", "call")]
    # no direct callers: the callee side alone decides
    line, _, v = pc.eax_summary(FakeEx({}), {}, {"callers": []}, {"instructions": ins})
    assert v == {"verdict": "value", "ret": "u32"}
    assert "returns a value (callee side only" in line and "3/3 after a fresh EAX write" in line


def test_callee_side_setter_is_scratch():
    ins = listing("MOV EAX,dword ptr [ESP + 0x4]", "MOV dword ptr [ECX + 0x8],EAX", "RET 0x4")
    assert pc.eax_before_rets(ins) == [("00401008", "scratch")]
    ins = listing("MOV dword ptr [ECX + 0x8],EDX", "RET")
    assert pc.eax_before_rets(ins) == [("00401004", "none")]


def caller_ex(*bodies):
    """FakeEx with callers C0, C1, ... each `CALL target` followed by its body, and the callgraph."""
    lst = {f"C{i}": listing("CALL 0x00409000", *b, start=0x500000 + 0x100 * i) for i, b in enumerate(bodies)}
    callers = [{"function": k, "site": v[0][0], "kind": "call"} for k, v in lst.items()]
    return FakeEx(lst), {"callers": callers}


VOID_CALLEE = {"instructions": listing("MOV dword ptr [ECX + 0x8],EDX", "RET")}


def test_summary_verdicts():
    ex, cg = caller_ex(("TEST EAX,EAX",), ("XOR EAX,EAX",), ("POP ESI", "RET"))
    line, sites, v = pc.eax_summary(ex, {}, cg, VOID_CALLEE)
    assert "EAX read after the call at 1/3 direct call sites" in line and "returns a value (don't use void)" in line
    assert v == {"verdict": "value", "ret": "u32"}

    ex, cg = caller_ex(("XOR EAX,EAX",), ("ADD ESP,0x4", "MOV EAX,dword ptr [ESI]"), ("JZ 0x00401000",))
    line, _, v = pc.eax_summary(ex, {}, cg, VOID_CALLEE)
    assert line.endswith("-> likely void") and "returns a value" not in line and v["verdict"] == "void"

    ex, cg = caller_ex(("TEST AL,AL",), ("MOVZX EAX,AL",), ("XOR EAX,EAX",))
    line, _, v = pc.eax_summary(ex, {}, cg, VOID_CALLEE)
    assert "returns a value (callers read AL: bool/char)" in line and v["ret"] == "u8"

    ex, cg = caller_ex(("POP ESI", "RET"), ("JNZ 0x00401000",))
    cg["callers"].append({"function": "C0", "site": "00400000", "kind": "jmp"})
    line, _, v = pc.eax_summary(ex, {}, cg, VOID_CALLEE)
    assert line.endswith("-> inconclusive") and "1 tail-jump/unknown site(s) not checked" in line

    ex, cg = caller_ex(("FSTP dword ptr [ESI]",))
    line, _, v = pc.eax_summary(ex, {}, cg, VOID_CALLEE)
    assert "returns a value in ST0" in line and v["ret"] == "double"


def test_likely_void_never_says_returns_a_value():
    ex, cg = caller_ex(("XOR EAX,EAX",))
    callee = {"instructions": listing("MOV EAX,dword ptr [ECX + 0x4]", "RET")}   # fresh, but nobody reads it
    line, _, v = pc.eax_summary(ex, {}, cg, callee)
    assert v["verdict"] == "void" and "returns a value" not in line and "likely void" in line


try:
    EX = pc.Export("trilogy")
except SystemExit:
    EX = None


@pytest.mark.skipif(EX is None, reason="no trilogy export")
def test_eax_row_in_bundle():
    repo = EX.ws.root / "eq-trilogy"
    t = pc.build_bundle(EX, pc.Headers(repo, "trilogy"), pc.Queue(repo), "00401366")
    row = next(l for l in t.splitlines() if l.startswith("| EAX evidence |"))
    assert "EAX read after the call at 9/9 direct call sites" in row and "returns a value" in row
    lines = t.splitlines()
    assert lines[lines.index(row) - 1].startswith("| ECX evidence |")
