"""Self-tests for the harness on hand-assembled code (no game files, no VC6).

Run: .venv\\Scripts\\python.exe -m pytest eq-trilogy\\tools\\tests\\unicorn
"""
import copy
import faulthandler
import struct
import sys
from pathlib import Path

import pytest


sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn import HarnessError, annotations, apply_defaults, printf_arg_types, undecorate  # noqa: E402
from eqd_unicorn.difftest import DiffTest, Side  # noqa: E402
from eqd_unicorn.extent import function_extent  # noqa: E402
from eqd_unicorn.machine import Machine  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402

ORIG, REW, CALLEE = 0x00500000, 0x30000000, 0x00600000


def setup_module():
    faulthandler.disable()      # see eq-trilogy tests/unicorn/conftest.py


class Asm:
    """Tiny assembler: raw bytes plus rel32 calls and labels' absolute addresses."""

    def __init__(self, base):
        self.base, self.b = base, bytearray()

    def __call__(self, hexbytes):
        self.b += bytes.fromhex(hexbytes)
        return self

    def call(self, target):
        self.b += b"\xE8" + struct.pack("<i", target - (self.base + len(self.b) + 5))
        return self

    def bytes(self):
        return bytes(self.b)


def make(orig: bytes, rew: bytes, fixture: dict, callee: bytes = b"\xCC" * 16) -> DiffTest:
    imgs = [Image.from_bytes("orig", ORIG, orig), Image.from_bytes("rew", REW, rew),
            Image.from_bytes("callees", CALLEE, callee)]
    m = Machine(imgs)
    o = Side("original", ORIG, function_extent(imgs[0].read, ORIG, frozenset({CALLEE})), {})
    r = Side("rewrite", REW, function_extent(imgs[1].read, REW, frozenset({CALLEE})), {})
    fixture.setdefault("cases", [{"args": [5]}, {"args": [-3]}])
    return DiffTest(m, fixture, o, r)


CALLER_FX = {"conv": "cdecl", "args": ["int"], "ret": "int",
             "callees": {hex(CALLEE): {"name": "cb", "conv": "stdcall", "args": ["int"], "ret": [7, 11]}}}


def orig_caller():
    # mov eax,[esp+4]; inc eax; push eax; call cb; add eax,eax; ret
    return Asm(ORIG)("8B442404 40 50").call(CALLEE)("01C0 C3").bytes()


def test_equivalent_code_with_stubbed_callee_passes():
    # mov ecx,[esp+4]; lea eax,[ecx+1]; push eax; call cb; shl eax,1; ret
    rew = Asm(REW)("8B4C2404 8D4101 50").call(CALLEE)("D1E0 C3").bytes()
    rep = make(orig_caller(), rew, dict(CALLER_FX)).run()
    assert rep.ok, rep.summary()
    c = rep.cases[0]
    assert c.original.eax == 14 and [str(x) for x in c.original.trace] == [f"{CALLEE:#010x} cb args=[6]"]
    assert c.original.esp_delta == 4


def test_wrong_callee_argument_is_a_trace_difference():
    rew = Asm(REW)("8B4C2404 51").call(CALLEE)("D1E0 C3").bytes()      # pushes x, not x+1
    rep = make(orig_caller(), rew, dict(CALLER_FX)).run()
    assert not rep.ok and rep.kinds() == {"trace"}, rep.summary()


def test_unscripted_callee_is_an_error():
    rep = make(orig_caller(), orig_caller(), dict(CALLER_FX, callees={})).run()
    assert not rep.ok and "error" in rep.kinds()
    assert "unscripted target" in rep.summary()


def test_passthrough_runs_both_callees_natively():
    callee = bytes.fromhex("8B442404 83C064 C20400")                   # stdcall: return x + 100
    fx = dict(CALLER_FX, callees={hex(CALLEE): {"name": "cb", "conv": "stdcall", "args": ["int"],
                                                 "passthrough": True}})
    rew = Asm(REW)("8B4C2404 8D4101 50").call(CALLEE)("D1E0 C3").bytes()
    rep = make(orig_caller(), rew, fx, callee).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax == (5 + 1 + 100) * 2


def test_passthrough_returning_below_its_call_ends():
    # VC6 __EH_prolog returns with its EH frame pushed (ESP below the call). Before a later fix such a
    # passthrough never ended, so nothing after it was intercepted (the next callee here is int3s).
    # prolog: pop ecx; push -1; push 0; push 0; push ecx; ret
    callee = bytes.fromhex("59 6AFF 6A00 6A00 51 C3").ljust(16, b"\xCC") + b"\xCC" * 16
    # push ebp; mov ebp,esp; call prolog; mov eax,[ebp+8]; push eax; call cb2; mov esp,ebp; pop ebp; ret
    caller = Asm(ORIG)("55 8BEC").call(CALLEE)("8B4508 50").call(CALLEE + 16)("8BE5 5D C3").bytes()
    rcaller = Asm(REW)("55 8BEC").call(CALLEE)("8B4508 50").call(CALLEE + 16)("8BE5 5D C3").bytes()
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int",
          "callees": {hex(CALLEE): {"name": "__EH_prolog", "passthrough": True},
                      hex(CALLEE + 16): {"name": "cb2", "conv": "stdcall", "args": ["int"], "ret": 7}}}
    rep = make(caller, rcaller, fx, callee).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax == 7
    assert [x.name for x in rep.cases[0].original.trace] == ["__EH_prolog", "cb2"]



def big_frame(base, size):
    # push ebp; mov ebp,esp; mov eax,size; call _chkstk; mov ecx,[ebp+8]; mov [esp],ecx; mov eax,[esp];
    # add eax,[ebp+8]; mov esp,ebp; pop ebp; ret  -> 2x, through the bottom of the probed frame
    return (Asm(base)("55 8BEC B8" + struct.pack("<I", size).hex()).call(CALLEE)
            ("8B4D08 890C24 8B0424 034508 8BE5 5D C3").bytes())


def test_alloca_probe_reserves_the_frame_untraced():
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int",
          "callees": {hex(CALLEE): {"name": "_chkstk", "probe": True}}}
    rep = make(big_frame(ORIG, 0x1128), big_frame(REW, 0x2000), fx).run()
    assert rep.ok, rep.summary()
    c = rep.cases[0]
    assert c.original.eax == 10 and c.original.trace == [] and c.original.esp_delta == 4
    fx["callees"][hex(CALLEE)]["probe"] = False
    assert not make(big_frame(ORIG, 0x1128), big_frame(REW, 0x1128), fx).run().ok   # a plain stub

def test_memory_writes_are_compared():
    fx = {"conv": "cdecl", "args": ["ptr"], "ret": "void", "objects": {"o": {"size": 16, "fill": "zero"}},
          "cases": [{"args": ["@o"]}]}
    orig = bytes.fromhex("8B442404 C70005000000 C3")                    # *p = 5
    assert make(orig, orig, dict(fx)).run().ok
    wrong = bytes.fromhex("8B442404 C70006000000 C3")                   # *p = 6
    rep = make(orig, wrong, dict(fx)).run()
    assert rep.kinds() == {"writes"}, rep.summary()


def test_calling_convention_and_callee_saved_registers():
    fx = {"conv": "stdcall", "args": ["int"], "ret": "int", "cases": [{"args": [1]}]}
    orig = bytes.fromhex("8B442404 C20400")                             # ret 4
    assert make(orig, orig, dict(fx)).run().ok
    assert make(orig, bytes.fromhex("8B442404 C3"), dict(fx)).run().kinds() == {"esp"}
    clobber = bytes.fromhex("8B442404 BB01000000 C20400")               # mov ebx, 1
    assert make(orig, clobber, dict(fx)).run().kinds() == {"preserved"}


def test_sub_dword_args_get_garbage_in_the_upper_bits():
    fx = {"conv": "cdecl", "args": ["short"], "ret": "int", "fuzz": {"seed": 1, "count": 20}}
    movsx = bytes.fromhex("0FBF442404 C3")                              # movsx eax, word [esp+4]
    full = bytes.fromhex("8B442404 C3")                                 # mov eax, [esp+4]
    assert make(movsx, movsx, dict(fx)).run().ok
    assert "return" in make(movsx, full, dict(fx)).run().kinds()


def test_short_return_compares_only_ax():
    fx = {"conv": "cdecl", "args": [], "ret": "short", "cases": [{"args": []}]}
    a = bytes.fromhex("B834120000 C3")                                  # mov eax, 0x1234
    b = bytes.fromhex("B83412FFFF C3")                                  # mov eax, 0xFFFF1234
    assert make(a, b, dict(fx)).run().ok


def test_float_return_and_float_callee():
    fx = {"conv": "cdecl", "args": [], "ret": "float", "cases": [{"args": []}]}
    assert make(bytes.fromhex("D9E8 C3"), bytes.fromhex("D9E8 C3"), dict(fx)).run().ok          # fld1
    assert make(bytes.fromhex("D9E8 C3"), bytes.fromhex("D9EE C3"), dict(fx)).run().kinds() == {"return"}
    assert make(bytes.fromhex("D9E8 C3"), bytes.fromhex("C3"), dict(fx)).run().kinds() == {"fpu"}
    fx["callees"] = {hex(CALLEE): {"name": "getf", "ret_float": 2.5}}
    orig = Asm(ORIG).call(CALLEE)("C3").bytes()
    rep = make(orig, Asm(REW).call(CALLEE)("C3").bytes(), dict(fx)).run()
    assert rep.ok and rep.cases[0].original.st0 == 2.5, rep.summary()


def test_imports_resolve_to_shared_stubs():
    # Both images import kernel32!GetTickCount through their own IAT slot.
    orig = Image.from_bytes("orig", ORIG, bytes.fromhex("FF1500105000 C3"), size=0x2000)   # call [0x501000]
    rew = Image.from_bytes("rew", REW, bytes.fromhex("FF1500100030 C3"), size=0x2000)      # call [0x30001000]
    orig.imports[0x501000] = "kernel32.dll!GetTickCount"
    rew.imports[0x30001000] = "kernel32.dll!GetTickCount"
    m = Machine([orig, rew])
    fx = {"conv": "cdecl", "args": [], "ret": "int", "cases": [{"args": []}],
          "callees": {"kernel32.dll!GetTickCount": {"conv": "stdcall", "ret": 1234}}}
    t = DiffTest(m, fx, Side("o", ORIG, function_extent(orig.read, ORIG), {}),
                 Side("r", REW, function_extent(rew.read, REW), {}))
    rep = t.run()
    assert rep.ok and rep.cases[0].original.eax == 1234, rep.summary()
    assert rep.cases[0].original.trace[0].target == "kernel32.dll!GetTickCount"


def switch_code(base):
    code = bytearray(bytes.fromhex(
        "8B442404 83F802 7719 FF2485" + struct.pack("<I", base + 0x28).hex()
        + "B80A000000C3 B814000000C3 B81E000000C3 31C0C3"))
    code += bytes([0x90]) * (0x28 - len(code)) + struct.pack("<III", base + 0x10, base + 0x16, base + 0x1C)
    return bytes(code)


def test_extent_follows_switch_tables():
    img = Image.from_bytes("sw", ORIG, switch_code(ORIG))
    ext = function_extent(img.read, ORIG)
    assert {ORIG + 0x10, ORIG + 0x16, ORIG + 0x1C, ORIG + 0x22} <= ext
    assert all(a < ORIG + 0x25 for a in ext)
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int",
          "fuzz": {"seed": 3, "count": 40, "args": [{"range": [-2, 4]}]}}
    rep = make(switch_code(ORIG), switch_code(REW), fx).run()
    assert rep.ok, rep.summary()
    assert {c.original.eax for c in rep.cases} == {0, 10, 20, 30}


def test_undecorate_and_annotations(tmp_path):
    assert undecorate("?GetLanguageName@CEverQuest@@QAEPADF@Z") == "CEverQuest::GetLanguageName"
    assert undecorate("??0EQPlayer@@QAE@XZ") == "EQPlayer::EQPlayer"
    assert undecorate("??1EQPlayer@@QAE@XZ") == "EQPlayer::~EQPlayer"
    assert undecorate("_strcpy") == "strcpy" and undecorate("_Foo@8") == "Foo"
    src = tmp_path / "a.cpp"
    src.write_text("// FUNCTION: TRILOGY 0x004c8fce\n// NAME: x\nchar* CEverQuest::GetLanguageName(short id)\n{\n}\n"
                   "// FUNCTION: OTHER 0x1\nint f(void) {}\n")
    got = annotations.scan([src], "TRILOGY")
    assert [(a.va, a.name) for a in got] == [(0x4C8FCE, "CEverQuest::GetLanguageName")]


def test_annotations_apply_one_identifier_defines(tmp_path):
    src = tmp_path / "a.cpp"
    src.write_text("#define CEverQuest CEverQuest_HWM_guess\n// FUNCTION: TRILOGY 0x10\nbool CEverQuest::HWM(int a)\n{}\n"
                   "#undef CEverQuest\n// FUNCTION: TRILOGY 0x20\nvoid CEverQuest::Other(void)\n{}\n")
    got = annotations.scan([src], "TRILOGY")
    assert [(a.va, a.name) for a in got] == [(0x10, "CEverQuest_HWM_guess::HWM"), (0x20, "CEverQuest::Other")]


def test_callee_scripts_resolve_objects_and_write_through_args():
    # p = alloc(); *p = 7; fill(p) -> callee writes 4 bytes at its arg 0; return p
    fx = {"conv": "cdecl", "args": [], "ret": "ptr", "objects": {"o": {"size": 16, "fill": "zero"}},
          "cases": [{"args": []}],
          "callees": {hex(CALLEE): {"name": "alloc", "ret": "@o"},
                      hex(CALLEE + 8): {"name": "fill", "args": ["ptr"], "writes": [{"arg": 0, "offset": 4,
                                                                                    "hex": "aabbccdd"}]}}}
    callee = b"\xCC" * 16

    def code(base):
        a = Asm(base).call(CALLEE)("C70007000000 50").call(CALLEE + 8)("58 C3")  # mov [eax],7; push eax; ...; pop eax
        return a.bytes()
    rep = make(code(ORIG), code(REW), fx, callee).run()
    assert rep.ok, rep.summary()
    w = rep.cases[0].original.writes
    obj = rep.cases[0].original.eax
    assert [w[obj + i] for i in range(8)] == [7, 0, 0, 0, 0xAA, 0xBB, 0xCC, 0xDD]


def test_case_callee_with_object_ret_overrides_a_plain_fixture_script():
    # The fixture's script needs no binding (ret 0) and is shared between runs; a case's own entry for the
    # same key ("@o") must still be resolved for that case.
    fx = {"conv": "cdecl", "args": [], "ret": "ptr", "objects": {"o": {"size": 16, "fill": "zero"}},
          "cases": [{"args": [], "callees": {hex(CALLEE): {"name": "alloc", "ret": "@o"}}}],
          "callees": {hex(CALLEE): {"name": "alloc", "ret": 0}}}

    def code(base):
        return Asm(base).call(CALLEE)("C3").bytes()
    rep = make(code(ORIG), code(REW), fx, b"\xCC" * 16).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax >= 0x60000000                      # the object, not 0


def test_callee_keyed_by_object_for_function_pointer_globals():
    # call [FPTR] where the fixture stores the address of object "fn" in FPTR: the callee is keyed "@fn".
    fptr = ORIG + 0x100

    def code(base):
        return Asm(base)("6A05 FF15" + struct.pack("<I", fptr).hex() + "59 C3").bytes()   # push 5; call [fptr]
    fx = {"conv": "cdecl", "args": [], "ret": "int", "cases": [{"args": []}],
          "objects": {"pad": {"size": 0x40}, "fn": {"size": 16}},
          "memory": [{"addr": hex(fptr), "u32": "@fn"}],
          "callees": {"@fn": {"name": "gfx", "args": ["int"], "ret": 42}}}
    orig = code(ORIG) + b"\x90" * (0x104 - len(code(ORIG)))
    rep = make(orig, code(REW), fx).run()
    assert rep.ok, rep.summary()
    tr = rep.cases[0].original.trace
    assert rep.cases[0].original.eax == 42 and len(tr) == 1 and tr[0].args == (5,)
    assert isinstance(tr[0].target, int) and tr[0].target >= 0x60000000
    fx["callees"] = {}
    assert "unscripted" in make(orig, code(REW), fx).run().summary()


def test_case_callees_override_the_fixture_script():
    fx = dict(CALLER_FX, cases=[{"args": [5]},
                                {"args": [5], "callees": {hex(CALLEE): {"name": "cb", "conv": "stdcall",
                                                                         "args": ["int"], "ret": 100}}}])
    rew = Asm(REW)("8B4C2404 8D4101 50").call(CALLEE)("D1E0 C3").bytes()
    rep = make(orig_caller(), rew, fx).run()
    assert rep.ok, rep.summary()
    assert [c.original.eax for c in rep.cases] == [14, 200]
    fx["cases"] = [{"args": [5], "callees": {hex(CALLEE): None}}]      # null: unscripted
    assert "unscripted" in make(orig_caller(), rew, fx).run().summary()


def test_nan_float_callee_args_compare_by_bits():
    # push <float bits>; call cb; pop ecx; ret. The same NaN on both sides must match (NaN != NaN in Python);
    # a different NaN payload is a trace difference.
    def code(base, bits):
        return Asm(base)("68" + struct.pack("<I", bits).hex()).call(CALLEE)("59 C3").bytes()
    fx = {"conv": "cdecl", "args": [], "ret": "void", "cases": [{"args": []}],
          "callees": {hex(CALLEE): {"name": "f", "args": ["float"]}}}
    assert make(code(ORIG, 0x7FC00001), code(REW, 0x7FC00001), dict(fx)).run().ok
    assert make(code(ORIG, 0x7FC00001), code(REW, 0x7FC00002), dict(fx)).run().kinds() == {"trace"}
    assert make(code(ORIG, 0x3F800000), code(REW, 0x3F800000), dict(fx)).run().ok


def test_fuzzed_text_never_looks_like_an_object_reference():
    # A cstr case arg starting with "@" or "0x" is resolved as an object / address, so fuzzed text must not.
    import random
    rng = random.Random("fuzz:text")
    texts = [DiffTest._fuzz_value(rng, "cstr", {}) for _ in range(5000)]
    assert not [t for t in texts if t.startswith("@") or t[:2].lower() == "0x"]


# --- fixture defaults ---------------------------------------------------------------------------------------

def test_apply_defaults_merges_per_entry_and_fixture_wins():
    defaults = {"_comment": "x",
                "callees": {"0x005223D0": {"name": "strlen", "args": ["cstr"], "passthrough": True},
                            "0x00522920": {"name": "strcmp", "args": ["cstr", "cstr"], "passthrough": True}},
                "rewrite_aliases": {"_strlen": "0x005223d0", "_strcmp": "0x00522920"}}
    fx = {"callees": {"0x5223d0": {"name": "mine", "ret": 3}, "0x00522920": None},
          "rewrite_aliases": {"_strcmp": None}}
    got = apply_defaults(fx, defaults)
    assert got["callees"] == {0x5223D0: {"name": "mine", "ret": 3}}
    assert got["rewrite_aliases"] == {"_strlen": "0x005223d0"}
    assert "_comment" not in got and fx["callees"]["0x00522920"] is None       # the fixture is not mutated
    assert apply_defaults({}, defaults)["callees"][0x522920]["name"] == "strcmp"
    assert apply_defaults({"defaults": False}, defaults) == {"defaults": False}


def test_default_passthrough_callee_is_inherited():
    callee = bytes.fromhex("8B442404 83C064 C3")                       # cdecl: return x + 100
    defaults = {"callees": {hex(CALLEE): {"name": "cb", "args": ["int"], "passthrough": True}}}

    def code(base):
        return Asm(base)("FF742404").call(CALLEE)("59 C3").bytes()      # push [esp+4]; call; pop ecx; ret
    fx = apply_defaults({"conv": "cdecl", "args": ["int"], "ret": "int"}, defaults)
    rep = make(code(ORIG), code(REW), fx, callee).run()
    assert rep.ok and rep.cases[0].original.eax == 105, rep.summary()
    assert "unscripted" in make(code(ORIG), code(REW), {"conv": "cdecl", "args": ["int"]}, callee).run().summary()


# --- memory fuzz --------------------------------------------------------------------------------------------

def field_eq(base, value):
    # thiscall, no args: return this->field8 == value
    return Asm(base)("31C0 837908" + f"{value:02x}" + " 0F94C0 C3").bytes()


def test_fuzzed_object_field_finds_a_wrong_constant():
    fx = {"conv": "thiscall", "this": "@o", "args": [], "ret": "int", "objects": {"o": {"size": 16}},
          "cases": [], "fuzz": {"seed": 7, "count": 50}}
    # Random object bytes alone never hit field8 == 3 or 4: the wrong rewrite (== 4) passes.
    assert make(field_eq(ORIG, 3), field_eq(REW, 4), dict(fx)).run().ok
    fx["fuzz"] = dict(fx["fuzz"], memory=[{"addr": "@o", "offset": 8, "type": "u32", "range": [0, 6]}])
    assert make(field_eq(ORIG, 3), field_eq(REW, 3), dict(fx)).run().ok
    rep = make(field_eq(ORIG, 3), field_eq(REW, 4), dict(fx)).run()
    assert rep.kinds() == {"return"}, rep.summary()
    assert {c.original.eax for c in rep.cases} == {0, 1}


def test_fuzzed_memory_fields_types_counts_and_args_stream():
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int", "objects": {"o": {"size": 32, "fill": "zero"}},
          "fuzz": {"seed": 9, "count": 30, "memory": [
              {"addr": "@o", "type": "u8", "count": 4, "choices": [1, 2]},
              {"addr": "@o+0x10", "type": "ptr", "choices": ["@o"]},
              {"addr": "0x500100", "type": "short", "range": [-2, -1]},
              {"addr": "@o+0x18", "size": 8, "p": 0.5}]}}
    cases = [c for _, c in make(b"\xC3", b"\xC3", dict(fx, cases=[])).cases()]
    plain = make(b"\xC3", b"\xC3", dict(fx, cases=[], fuzz=dict(fx["fuzz"], memory=[])))
    assert [c["args"] for c in cases] == [c["args"] for _, c in plain.cases()]   # memory fuzz: own stream
    mem = cases[0]["memory"]
    assert [w["offset"] for w in mem[:4]] == [0, 1, 2, 3] and all(w["hex"] in ("01", "02") for w in mem[:4])
    assert mem[4] == {"addr": "@o+0x10", "offset": 0, "u32": "@o"}
    assert mem[5]["hex"] in ("feff", "ffff")
    assert 5 <= len([c for c in cases if len(c["memory"]) == 7]) <= 25          # p = 0.5
    for bad in ({"addr": "@o", "type": "ptr"}, {"addr": "@o"}, {"type": "u8"}, {"addr": "@o", "type": "cstr"}):
        with pytest.raises(HarnessError):
            make(b"\xC3", b"\xC3", dict(fx, fuzz={"seed": 1, "count": 1, "memory": [bad]})).cases()


def test_fuzzed_global_is_seeded_after_fixture_memory():
    g = ORIG + 0x100

    def code(base):
        return Asm(base)("A1" + struct.pack("<I", g).hex() + " C3").bytes()    # return *(int*)g
    orig = code(ORIG) + b"\x90" * (0x104 - len(code(ORIG)))
    fx = {"conv": "cdecl", "args": [], "ret": "int", "memory": [{"addr": hex(g), "u32": 5}],
          "cases": [{"args": []}],
          "fuzz": {"seed": 2, "count": 10, "memory": [{"addr": hex(g), "type": "int", "choices": [-7, 9]}]}}
    rep = make(orig, code(REW), fx).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax == 5
    assert {c.original.eax for c in rep.cases[1:]} == {0xFFFFFFF9, 9}


# --- faults -------------------------------------------------------------------------------------------------

NULL_FX = {"conv": "cdecl", "args": ["ptr"], "ret": "int", "cases": [{"args": [0]}, {"args": [8]}]}


def test_same_fault_kind_and_address_is_a_match():
    orig = bytes.fromhex("8B442404 8B4004 C3")                          # return p->f4
    rew = bytes.fromhex("8B4C2404 90 8B4104 C3")                        # same, other register and pc
    rep = make(orig, rew, dict(NULL_FX)).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.fault == ("read_unmapped", 4)
    assert rep.cases[0].original.error != rep.cases[0].rewrite.error   # the pc differs
    wrong_addr = bytes.fromhex("8B442404 8B4008 C3")                    # reads p->f8
    rep = make(orig, wrong_addr, dict(NULL_FX)).run()
    assert rep.kinds() == {"error"} and "read_unmapped at 0x4 vs rewrite read_unmapped at 0x8" in rep.summary()


def test_fault_kind_differs():
    orig = bytes.fromhex("8B442404 8B4004 C3")                          # read [p+4]
    write = bytes.fromhex("8B442404 C7400400000000 C3")                 # write [p+4]
    rep = make(orig, write, dict(NULL_FX)).run()
    assert rep.kinds() == {"error"} and "read_unmapped at 0x4 vs rewrite write_unmapped at 0x4" in rep.summary()
    div = bytes.fromhex("31C9 8B442404 99 F7F9 C3")                     # idiv by zero
    rep = make(div, div, dict(NULL_FX)).run()
    assert rep.ok and rep.cases[0].original.fault == ("div0", None), rep.summary()
    assert make(div, orig, dict(NULL_FX)).run().kinds() == {"error"}


def test_writes_and_calls_before_the_same_fault_are_compared():
    fx = {"conv": "cdecl", "args": ["ptr"], "ret": "int", "objects": {"g": {"size": 16, "fill": "zero"}},
          "cases": [{"args": [0], "regs": {"edx": "@g"}}]}
    orig = bytes.fromhex("C70201000000 8B442404 8B00 C3")              # *edx = 1; return *p
    rew = bytes.fromhex("C70202000000 8B442404 8B00 C3")               # *edx = 2; ...
    assert make(orig, orig, dict(fx)).run().ok
    assert make(orig, rew, dict(fx)).run().kinds() == {"writes"}
    fx2 = dict(CALLER_FX, args=["ptr"], cases=[{"args": [0]}])
    with_call = Asm(ORIG)("6A01").call(CALLEE)("8B442404 8B00 C3").bytes()
    with_other = Asm(REW)("6A02").call(CALLEE)("8B442404 8B00 C3").bytes()
    assert make(with_call, with_other, fx2).run().kinds() == {"trace"}


def test_float_callees_with_different_pops_in_one_run():
    # f0: cdecl float, no args (ret 0); then getf: thiscall float with 3 args (ret 12). The fld/ret stub is
    # rewritten per call; a stale translation used to run `ret 0` for getf.
    callee2 = CALLEE + 8
    code = lambda base: Asm(base).call(callee2)("DDD8 6A00 6A00 6A00").call(CALLEE)("DDD8 C3").bytes()  # noqa: E731
    fx = {"conv": "cdecl", "args": [], "ret": "void", "cases": [{"args": [], "regs": {"ecx": "0x1000"}}],
          "callees": {hex(callee2): {"name": "f0", "ret_float": 1.0},
                      hex(CALLEE): {"name": "getf", "conv": "thiscall", "args": ["float", "float", "float"],
                                    "ret_float": 2.5}}}
    rep = make(code(ORIG), code(REW), fx).run()
    assert rep.ok and rep.cases[0].original.error is None and rep.cases[0].original.esp_delta == 4, rep.summary()


def test_every_case_faulting_in_a_passthrough_callee_fails_coverage():
    callee = bytes.fromhex("8B442404 8B4004 C3")                       # cdecl cb(p): return p->f4
    fx = {"conv": "cdecl", "args": ["ptr"], "ret": "int", "cases": [{"args": [0]}, {"args": [8]}],
          "callees": {hex(CALLEE): {"name": "cb", "args": ["ptr"], "passthrough": True}}}

    def code(base, k):      # push [esp+4]; call cb; pop ecx; mov eax,k; ret  (the tail never runs)
        return Asm(base)("FF742404").call(CALLEE)(f"59 B8{k:02x}000000 C3").bytes()
    rep = make(code(ORIG, 1), code(REW, 2), dict(fx), callee).run()
    assert rep.kinds() == {"coverage"} and "pc 0x" in rep.summary(), rep.summary()
    assert make(code(ORIG, 1), code(REW, 2), dict(fx, fault_ok="null p"), callee).run().ok
    assert make(code(ORIG, 1), code(REW, 2), dict(fx), callee).run(stop_on_failure=True).ok    # mutation runs
    # Faults in the function itself (the pc differs per side) stay a match.
    assert make(bytes.fromhex("8B442404 8B4004 C3"), bytes.fromhex("8B4C2404 90 8B4104 C3"), dict(NULL_FX)).run().ok


def test_same_harness_error_on_both_sides_still_fails():
    rew = Asm(REW)("8B442404 40 50").call(CALLEE)("01C0 C3").bytes()
    rep = make(orig_caller(), rew, dict(CALLER_FX, callees={})).run()
    assert not rep.ok and "both sides" in rep.summary()
    loop = bytes.fromhex("EBFE")                                       # jmp $
    rep = make(loop, loop, dict(NULL_FX, max_insns=1000)).run()
    assert not rep.ok and rep.cases[0].original.fault == ("timeout", None)


# --- varargs ------------------------------------------------------------------------------------------------

def printf_caller(base, fmt_a, fmt_b, third=7, text=True):
    """sprintf(0, fmt_a, third, "hi" or 0x55); sprintf(0, fmt_b, 1.5); each call followed by its own add esp."""
    a = Asm(base)
    data = base + 0x80
    a("68" + struct.pack("<I", data + 0x40 if text else 0x55).hex())   # push "hi" / 0x55
    a("6A" + f"{third:02x}")                                           # push third
    a("68" + struct.pack("<I", data).hex())                            # push fmt_a
    a("6A00").call(CALLEE)("83C410")                                   # push 0; call; add esp, 16
    a("68" + struct.pack("<I", 0x3FF80000).hex() + "6A00")             # push double 1.5 (hi, lo)
    a("68" + struct.pack("<I", data + 0x20).hex())                     # push fmt_b
    a("6A00").call(CALLEE)("83C410 C3")
    code = a.bytes()
    code += b"\x90" * (0x80 - len(code))
    return code + fmt_a.encode().ljust(0x20, b"\0") + fmt_b.encode().ljust(0x20, b"\0") + b"hi\0"


VARARGS_FX = {"conv": "cdecl", "args": [], "ret": "void", "cases": [{"args": []}],
              "callees": {hex(CALLEE): {"name": "sprintf", "args": ["ptr", "cstr"], "varargs": "printf"}}}


def test_varargs_printf_records_the_args_of_each_call():
    rep = make(printf_caller(ORIG, "%d %s", "%.2f"), printf_caller(REW, "%d %s", "%.2f"),
               copy.deepcopy(VARARGS_FX)).run()
    assert rep.ok, rep.summary()
    assert [c.args for c in rep.cases[0].original.trace] == [(0, "%d %s", 7, "hi"), (0, "%.2f", 1.5)]
    rep = make(printf_caller(ORIG, "%d %s", "%.2f"), printf_caller(REW, "%d %s", "%.2f", third=8),
               copy.deepcopy(VARARGS_FX)).run()
    assert rep.kinds() == {"trace"}, rep.summary()


def test_varargs_stack_counts_from_the_callers_cleanup():
    fx = copy.deepcopy(VARARGS_FX)
    fx["callees"][hex(CALLEE)]["varargs"] = "stack"
    # "stack" records the extra args as u32: fine for scalars, but a pointer to each image's own literal
    # differs, so this caller pushes 0x55 instead of "hi".
    rep = make(printf_caller(ORIG, "%d %d", "%.2f", text=False), printf_caller(REW, "%d %d", "%.2f", text=False),
               fx).run()
    assert rep.ok, rep.summary()
    assert [c.args for c in rep.cases[0].original.trace] == [(0, "%d %d", 7, 0x55), (0, "%.2f", 0, 0x3FF80000)]
    rep = make(printf_caller(ORIG, "%d %d", "%.2f", text=False),
               printf_caller(REW, "%d %d", "%.2f", third=8, text=False), fx).run()
    assert rep.kinds() == {"trace"}, rep.summary()
    # No cleanup right after the call (deferred to the epilogue): an error, not a guess.
    no_cleanup = Asm(ORIG)("6A00 6A00").call(CALLEE)("C3").bytes()
    rep = make(no_cleanup, no_cleanup, dict(fx, callees={hex(CALLEE): {"args": ["ptr"], "varargs": "stack"}})).run()
    assert "cleanup" in rep.summary() and not rep.ok


def test_printf_arg_types():
    p = printf_arg_types
    assert p("plain %% text") == []
    assert p("%d %i %u %x %X %o") == ["int", "int", "u32", "u32", "u32", "u32"]
    assert p("%-08.3f|%e|%G") == ["double"] * 3
    assert p("%s %c %hd %hu %ld %I64d %I64u") == ["cstr", "char", "short", "u16", "int", "i64", "u64"]
    assert p("%*d %.*s %ls %S %p %n") == ["int", "int", "int", "cstr", "ptr", "ptr", "ptr", "ptr"]
    assert p("%y %d") == ["int"] and p("trailing %") == []


def int_printf_caller(base, fmt, extra):
    """sprintf(0, fmt, *extra) with `add esp, 8 + 4 * len(extra)` after the call."""
    a = Asm(base)
    for v in reversed(extra):
        a("6A" + f"{v:02x}")
    a("68" + struct.pack("<I", base + 0x80).hex() + "6A00").call(CALLEE)("83C4" + f"{8 + 4 * len(extra):02x} C3")
    code = a.bytes()
    return code + b"\x90" * (0x80 - len(code)) + fmt.encode() + b"\0"


def test_varargs_arity_mismatch():
    """The rewrite drops or adds a variable argument. 'printf' records what the format consumes: a missing
    arg reads whatever lies above it on the stack (a trace difference), an unused extra arg is invisible to
    the callee and to the caller (the caller pops it), so it matches. 'stack' records every pushed arg: both
    are trace differences."""
    orig = int_printf_caller(ORIG, "%d %d", [7, 8])
    fewer, more = int_printf_caller(REW, "%d %d", [7]), int_printf_caller(REW, "%d %d", [7, 8, 9])
    for mode, more_ok in (("printf", True), ("stack", False)):
        fx = copy.deepcopy(VARARGS_FX)
        fx["callees"][hex(CALLEE)]["varargs"] = mode
        rep = make(orig, int_printf_caller(REW, "%d %d", [7, 8]), copy.deepcopy(fx)).run()
        assert rep.ok, rep.summary()
        assert rep.cases[0].original.trace[0].args == (0, "%d %d", 7, 8)
        rep = make(orig, fewer, copy.deepcopy(fx)).run()
        assert rep.kinds() == {"trace"}, (mode, rep.summary())
        rep = make(orig, more, copy.deepcopy(fx)).run()
        assert rep.ok == more_ok and (more_ok or rep.kinds() == {"trace"}), (mode, rep.summary())


def test_varargs_stack_cleanup_smaller_than_the_fixed_args_is_an_error():
    fx = copy.deepcopy(VARARGS_FX)
    fx["callees"][hex(CALLEE)].update(varargs="stack", args=["ptr", "cstr", "int"])
    code = int_printf_caller(ORIG, "%d", [])                            # add esp, 8: two slots < three fixed
    rep = make(code, code, fx).run()
    assert not rep.ok and "cleanup" in rep.summary()


# --- RDTSC is deterministic ---------------------------------------------------------------------------------
TWO_RDTSC = "0F31 0F31 C3"                       # the second RDTSC's EDX:EAX is the return value
RDTSC_FX = {"conv": "cdecl", "args": [], "ret": "i64", "cases": [{"args": []}, {"args": []}]}


def test_rdtsc_is_deterministic_and_identical_on_both_sides():
    from eqd_unicorn.machine import RDTSC_START, RDTSC_STEP
    code = Asm(ORIG)(TWO_RDTSC).bytes()
    rep = make(code, Asm(REW)(TWO_RDTSC).bytes(), dict(RDTSC_FX)).run()
    assert rep.ok, rep.summary()
    want = RDTSC_START + RDTSC_STEP
    for c in rep.cases:                           # reset per case: the second case starts over
        for r in (c.original, c.rewrite):
            assert (r.edx << 32 | r.eax) == want and r.error is None


def test_rdtsc_override_start_and_step_with_borrow_across_32_bits():
    # mov ecx,5 ; rdtsc ; add eax,ecx: code after RDTSC in the same block still runs
    code = "B905000000 0F31 01C8 C3"
    fx = dict(RDTSC_FX, rdtsc="0x0001c449ffffff00", rdtsc_step="0x200",
              cases=[{"args": []}, {"args": [], "rdtsc": 7}])
    rep = make(Asm(ORIG)(code).bytes(), Asm(REW)(code).bytes(), fx).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax == 0xffffff05 and rep.cases[0].original.edx == 0x0001c449
    assert rep.cases[1].original.eax == 12 and rep.cases[1].original.edx == 0     # case key wins
    two = make(Asm(ORIG)(TWO_RDTSC).bytes(), Asm(REW)(TWO_RDTSC).bytes(),
               dict(RDTSC_FX, rdtsc="0x1c449ffffff00", rdtsc_step=0x200)).run()
    o = two.cases[0].original
    assert (o.edx << 32 | o.eax) == 0x1c44a00000100        # the 64-bit counter carries into EDX


def test_a_function_reading_the_clock_against_a_base_passes():
    # GetCpuTicks-style: rdtsc minus a 64-bit base in memory (inside the original image's page, so both sides
    # read it): only passes when both sides see the same clock
    base = ORIG + 0x800
    code = f"0F31 2B05{base.to_bytes(4, 'little').hex()} 1B15{(base + 4).to_bytes(4, 'little').hex()} C3"
    fx = dict(RDTSC_FX, rdtsc="0x0000001000001234", memory=[{"addr": hex(base), "hex": "0000000010000000"}])
    rep = make(Asm(ORIG)(code).bytes(), Asm(REW)(code).bytes(), fx).run()
    assert rep.ok, rep.summary()
    o = rep.cases[0].original
    assert (o.edx << 32 | o.eax) == 0x1234


# --- x87 inputs (`fpu`) ----------------------------------------------------------------------------------------
# __ftol-style helper: the value arrives in ST0 (popped), the int64 leaves in EDX:EAX.
# sub esp,8 ; fistp qword [esp] ; pop eax ; pop edx ; ret
FISTP64 = "83EC08 DF3C24 58 5A C3"
FTOL_FX = {"conv": "cdecl", "args": [], "ret": "i64"}


def test_fpu_case_input_reaches_st0_and_i64_expect_reads_edx_eax():
    fx = dict(FTOL_FX, cases=[{"args": [], "fpu": [2.5], "expect": {"ret": 2}},          # round to even
                              {"args": [], "fpu": [-3.0], "expect": {"ret": -3}},
                              {"args": [], "fpu": [{"f64": "0x41f0000000040000"}], "expect": {"ret": 4294967296}},
                              {"args": [], "fpu": ["-inf"], "expect": {"ret": "0x8000000000000000"}}])
    rep = make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(FISTP64).bytes(), fx).run()
    assert rep.ok, rep.summary()
    o = rep.cases[1].original
    assert (o.edx, o.eax, o.fpu_depth) == (0xFFFFFFFF, 0xFFFFFFFD, 0)
    bad = dict(fx, cases=[{"args": [], "fpu": [7.0], "expect": {"ret": 8}}])
    assert make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(FISTP64).bytes(), bad).run().kinds() == {"expect"}


def test_fpu_input_differences_are_caught():
    fx = dict(FTOL_FX, cases=[{"args": [], "fpu": [-3.0]}])
    # returns EAX only, EDX zero: wrong high dword
    low_only = "83EC08 DF3C24 58 5A 31D2 C3"
    assert make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(low_only).bytes(), dict(fx)).run().kinds() == {"return"}
    # leaves the input on the stack (fld st0 first): x87 depth 1 after an integer return
    keeps = "D9C0 " + FISTP64
    assert make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(keeps).bytes(), dict(fx)).run().kinds() == {"fpu"}
    # without an input the helper pops an empty stack: both sides end at depth 7
    empty = dict(FTOL_FX, cases=[{"args": []}])
    assert make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(FISTP64).bytes(), empty).run().kinds() == {"fpu"}


def test_fpu_order_is_st0_first_and_fixture_default():
    # fsubp st(1), st ; ret  ->  ST1 - ST0
    code = "DEE9 C3"
    fx = {"conv": "cdecl", "args": [], "ret": "double", "fpu": [1.0, 5.0],
          "cases": [{"args": []}, {"args": [], "fpu": [5.0, 1.0]}]}
    rep = make(Asm(ORIG)(code).bytes(), Asm(REW)(code).bytes(), fx).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.st0 == 4.0 and rep.cases[1].original.st0 == -4.0


def test_fpu_values_and_bad_entries():
    from eqd_unicorn.difftest import _f80
    from eqd_unicorn.machine import f80_to_float
    for v in (0.0, -0.0, 1.0, -2.75, 5e-324, -2.2250738585072014e-308, 1.7976931348623157e308,
              float("inf"), float("-inf")):
        mant, ex = struct.unpack("<QH", _f80(v))
        got = f80_to_float(mant, ex)
        assert got == v and str(got)[0] == str(v)[0], (v, got)
    assert f80_to_float(*struct.unpack("<QH", _f80("nan"))) != f80_to_float(*struct.unpack("<QH", _f80("nan")))
    assert _f80({"f80": "0x3fff8000000000000000"}) == _f80(1.0)
    assert _f80({"f32": "0x3fc00000"}) == _f80(1.5)
    for bad in ("0x3ff0000000000000", "x", True, {"f80": "0x1" + "0" * 20}):
        with pytest.raises(HarnessError):
            _f80(bad)
    fx = dict(FTOL_FX, cases=[{"args": [], "regs": {"st0": 2}}])
    with pytest.raises(HarnessError, match="fpu"):
        make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(FISTP64).bytes(), fx).run()


def test_fuzzed_fpu_inputs_have_their_own_stream():
    fx = dict(FTOL_FX, cases=[], fuzz={"seed": 3, "count": 20, "fpu": [{"range": [-1e6, 1e6]}]})
    t = make(Asm(ORIG)(FISTP64).bytes(), Asm(REW)(FISTP64).bytes(), fx)
    vals = [c["fpu"][0] for _, c in t.cases()]
    assert len(set(vals)) == 20 and all(isinstance(v, float) and -1e6 <= v <= 1e6 for v in vals)
    rep = t.run()
    assert rep.ok and len(rep.cases) == 20, rep.summary()
    assert len({(c.original.edx << 32) | c.original.eax for c in rep.cases}) > 1


def test_fuzz_salt_changes_only_salted_runs(monkeypatch):
    from eqd_unicorn import difftest
    monkeypatch.delenv("EQD_FUZZ_SALT", raising=False)
    assert difftest._seed_key(123) == "123"          # unsalted = the old stream names, so fixtures replay unchanged
    monkeypatch.setenv("EQD_FUZZ_SALT", "r2")
    assert difftest._seed_key(123) == "123~r2"
