"""Self-tests for callee stubs that unbalance the stack: `noreturn` callees, the stub purge check against
the callee's own `ret N`, and the hints on a fetch from unmapped code. Hand-assembled code, no game files."""
import faulthandler
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn.difftest import DiffTest, Side  # noqa: E402
from eqd_unicorn.extent import function_extent, stack_purge  # noqa: E402
from eqd_unicorn.machine import Machine  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402

ORIG, REW, CALLEE = 0x00500000, 0x30000000, 0x00600000


def setup_module():
    faulthandler.disable()


def call(base, at, target):
    return b"\xE8" + struct.pack("<i", target - (base + at + 5))


def make(orig: bytes, rew: bytes, fixture: dict, callee: bytes = b"\xCC" * 16, purge=True) -> DiffTest:
    imgs = [Image.from_bytes("orig", ORIG, orig), Image.from_bytes("rew", REW, rew),
            Image.from_bytes("callees", CALLEE, callee)]
    m = Machine(imgs)
    o = Side("original", ORIG, function_extent(imgs[0].read, ORIG, frozenset({CALLEE})), {})
    r = Side("rewrite", REW, function_extent(imgs[1].read, REW, frozenset({CALLEE})), {})
    fixture.setdefault("cases", [{"args": [0]}, {"args": [1]}])
    cal = imgs[2]
    purge_of = (lambda va: stack_purge(cal.read, va, in_image=cal.contains) if cal.contains(va) else None) \
        if purge else None
    return DiffTest(m, fixture, o, r, purge_of=purge_of)


def verify_like(base, cleanup: bool) -> bytes:
    """push edi; mov eax,[esp+8]; test eax,eax; jnz done; push 0x2a; call EXIT; [add esp,4]; done: pop edi; ret
    (VC6 emits no cleanup after a call to a noreturn function such as exit: the original has none)."""
    head = bytes.fromhex("57 8B442408 85C0")
    tail_call = bytes.fromhex("6A2A")
    after = bytes.fromhex("83C404") if cleanup else b""
    body_len = len(tail_call) + 5 + len(after)
    code = head + bytes([0x75, body_len]) + tail_call
    code += call(base, len(code), CALLEE) + after + bytes.fromhex("5F C3")
    return code


EXIT_FX = {"conv": "cdecl", "args": ["int"], "ret": "void",
           "callees": {hex(CALLEE): {"name": "exit", "conv": "cdecl", "args": ["int"]}}}


def test_returning_stub_of_a_noreturn_callee_faults_with_a_ret_hint():
    rep = make(verify_like(ORIG, False), verify_like(REW, True), {**EXIT_FX}).run()
    assert not rep.ok
    err = rep.cases[0].original.error
    assert err.startswith("fetch_unmapped") and "hint: `ret`" in err and "noreturn" in err, err
    assert rep.cases[0].rewrite.error is None
    assert rep.cases[1].ok                      # status != 0: exit is never called


def test_noreturn_callee_ends_the_run_on_both_sides():
    fx = {**EXIT_FX, "callees": {hex(CALLEE): {"name": "exit", "conv": "cdecl", "args": ["int"], "noreturn": True}}}
    rep = make(verify_like(ORIG, False), verify_like(REW, True), fx).run()
    assert rep.ok, rep.summary()
    o = rep.cases[0].original
    assert o.fault == ("noreturn", CALLEE) and [str(c) for c in o.trace] == [f"{CALLEE:#010x} exit args=[42]"]


def test_noreturn_on_one_side_only_fails():
    # the rewrite returns instead of calling exit
    rew = bytes.fromhex("57 8B442408 5F C3")
    fx = {**EXIT_FX, "callees": {hex(CALLEE): {"name": "exit", "conv": "cdecl", "args": ["int"], "noreturn": True}}}
    rep = make(verify_like(ORIG, False), rew, fx).run()
    assert not rep.ok and "error" in rep.kinds(), rep.summary()


def byvalue_caller(base) -> bytes:
    """mov eax,[esp+4]; push eax (a by-value object built in place); mov ecx,0x1234; call M; ret
    M is a thiscall method that pops its by-value argument (`ret 4`)."""
    code = bytes.fromhex("8B442404 50 B934120000")
    return code + call(base, len(code), CALLEE) + bytes.fromhex("C3")


METHOD = bytes.fromhex("C20400") + b"\xCC" * 13       # ret 4


def test_stub_pop_count_is_checked_against_the_callees_ret_n():
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int",
          "callees": {hex(CALLEE): {"name": "update", "conv": "thiscall", "args": [], "ret": 0}}}
    rep = make(byvalue_caller(ORIG), byvalue_caller(REW), fx, METHOD).run()
    assert rep.warnings and "ret 4" in rep.warnings[0] and "1 more stack arg" in rep.warnings[0], rep.warnings
    # the stub's ret lands on the pushed arg: the caller "returns" to it
    assert rep.cases[1].original.fault == ("fetch_unmapped", 1)
    fx["callees"][hex(CALLEE)]["args"] = ["ignore"]
    rep = make(byvalue_caller(ORIG), byvalue_caller(REW), fx, METHOD).run()
    assert rep.ok and not rep.warnings, rep.summary()
    assert rep.cases[1].original.esp_delta == 4


def test_purge_warnings_show_with_failures_and_purge_ok_silences_them():
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int",
          "callees": {hex(CALLEE): {"name": "update", "conv": "cdecl", "args": ["int"], "ret": 0}}}
    rew = bytes.fromhex("31C0 C3")                  # differs: makes no call
    rep = make(byvalue_caller(ORIG), rew, fx, METHOD).run()
    assert not rep.ok and "warning: callee 0x00600000 update is stubbed cdecl" in rep.summary(), rep.summary()
    assert "make it stdcall" in rep.warnings[0]
    fx["callees"][hex(CALLEE)]["purge_ok"] = "deliberate"
    assert not make(byvalue_caller(ORIG), rew, fx, METHOD).run().warnings
    # a case's own callee script is checked too
    fx["callees"][hex(CALLEE)] = {"name": "update", "conv": "thiscall", "args": ["ignore"]}
    fx["cases"] = [{"args": [1], "callees": {hex(CALLEE): {"name": "update2", "conv": "stdcall", "args": []}}}]
    rep = make(byvalue_caller(ORIG), byvalue_caller(REW), fx, METHOD).run()
    assert len(rep.warnings) == 1 and rep.warnings[0].startswith("case 0: callee"), rep.warnings


def test_stack_purge_reads_ret_n():
    img = Image.from_bytes("c", CALLEE, bytes.fromhex("85C9 7403 C20800 C20800") + b"\xCC" * 8)
    assert stack_purge(img.read, CALLEE, in_image=img.contains) == 8
    img = Image.from_bytes("c", CALLEE, bytes.fromhex("85C9 7401 C3 C20800") + b"\xCC" * 8)
    assert stack_purge(img.read, CALLEE, in_image=img.contains) is None          # disagreeing rets
    img = Image.from_bytes("c", CALLEE, bytes.fromhex("FF2500106000") + b"\xCC" * 8)
    assert stack_purge(img.read, CALLEE, in_image=img.contains) is None          # jmp [IAT]: unknown


def test_indirect_call_through_an_unset_pointer_gets_a_function_pointer_hint():
    code = bytes.fromhex("8B442404 FFD0 C3")       # mov eax,[esp+4]; call eax; ret
    fx = {"conv": "cdecl", "args": ["u32"], "ret": "int", "cases": [{"args": [0]}]}
    rep = make(code, code, fx, purge=False).run()
    err = rep.cases[0].original.error
    assert err.startswith("fetch_unmapped at 0x0") and "`call eax`" in err and "function pointer" in err, err
    assert rep.cases[0].rewrite.fault == ("fetch_unmapped", 0)


def test_callee_writes_per_call_index():
    """`"call": N` on a write applies it only to that call of the callee (a `_read` stub that returns
    different header fields per read)."""
    buf = 0x00600100

    def body(base):
        # push esi; call rd; mov esi,[buf]; call rd; mov eax,[buf]; add eax,esi; pop esi; ret
        c = b"\x56" + call(base, 1, CALLEE) + bytes.fromhex("8B3500016000")
        return c + call(base, len(c), CALLEE) + bytes.fromhex("A100016000 01F0 5E C3")
    code, rew = body(ORIG), body(REW)
    fx = {"conv": "cdecl", "args": [], "ret": "int", "cases": [{"args": []}],
          "callees": {hex(CALLEE): {"name": "rd", "writes": [
              {"addr": hex(buf), "hex": "01000000", "call": 0},
              {"addr": hex(buf), "hex": "10000000", "call": [1]}]}}}
    rep = make(code, rew, fx, b"\xC3" + b"\x00" * 0x200, purge=False).run()
    assert rep.ok, rep.summary()
    assert rep.cases[0].original.eax == 0x11
