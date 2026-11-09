r"""Original-side coverage (eqd_unicorn/coverage.py, Machine.run(cover=), DiffTest's per-fixture record).

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\unicorn\test_coverage.py
"""
import faulthandler
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn import coverage as cov  # noqa: E402
from eqd_unicorn.difftest import DiffTest, Side  # noqa: E402
from eqd_unicorn.extent import function_extent  # noqa: E402
from eqd_unicorn.machine import Machine  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402

ORIG, REW, CALLEE = 0x00500000, 0x30000000, 0x00600000
# mov eax,[esp+4]; test eax,eax; jz L; inc eax; ret; L: dec eax; ret
BRANCH = bytes.fromhex("8B442404 85C0 7402 40 C3 48 C3")


def setup_module():
    faulthandler.disable()      # see eq-trilogy tests/unicorn/conftest.py


def call(base, target):
    return b"\xE8" + struct.pack("<i", target - (base + 5))


def make(orig, rew, fx, callee=b"\xCC" * 16, coverage_dir=None):
    imgs = [Image.from_bytes("orig", ORIG, orig), Image.from_bytes("rew", REW, rew),
            Image.from_bytes("callees", CALLEE, callee)]
    o = Side("original", ORIG, function_extent(imgs[0].read, ORIG, frozenset({CALLEE})), {})
    r = Side("rewrite", REW, function_extent(imgs[1].read, REW, frozenset({CALLEE})), {})
    return DiffTest(Machine(imgs), fx, o, r, original_image=imgs[0], coverage_dir=coverage_dir)


def test_static_blocks():
    img = Image.from_bytes("orig", ORIG, BRANCH)
    s = cov.Static(img.read, function_extent(img.read, ORIG))
    assert [(b[0] - ORIG, b[1] - ORIG, b[3]) for b in s.blocks] == [(0, 8, 3), (8, 10, 2), (10, 12, 2)]


def test_branch_coverage_and_record(tmp_path):
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int", "cases": [{"args": [5]}],
          "_path": str(tmp_path / "F.json")}
    rep = make(BRANCH, BRANCH, fx, coverage_dir=tmp_path / "cov").run()
    assert rep.ok, rep.summary()
    c = rep.coverage
    assert c["blocks"] == [2, 3] and c["insns"] == [5, 7] and c["rewrite_insns"] == 7
    assert c["uncovered"] == [["0050000A", "0050000C", 2]]
    on_disk = json.loads((tmp_path / "cov" / "00500000" / "F.json").read_text())
    assert on_disk["blocks"] == [2, 3] and len(on_disk["map"]) == 3
    fx["cases"].append({"args": [0]})
    assert make(BRANCH, BRANCH, fx).run().coverage["blocks"] == [3, 3]


def test_no_record_when_stopping_early(tmp_path):
    fx = {"conv": "cdecl", "args": ["int"], "ret": "int", "cases": [{"args": [5]}],
          "_path": str(tmp_path / "F.json")}
    rep = make(BRANCH, BRANCH, fx, coverage_dir=tmp_path).run(stop_on_failure=True)
    assert rep.coverage is None and not (tmp_path / "00500000").exists()


def test_code_after_a_passthrough_or_stubbed_call_counts():
    # mov eax,[esp+4]; inc eax; push eax; call cb; add eax,eax; ret  (one static block around the call)
    code = lambda base: bytes.fromhex("8B442404 40 50") + call(base + 6, CALLEE) + bytes.fromhex("01C0 C3")
    callee = bytes.fromhex("8B442404 83C064 C20400")
    for spec in ({"passthrough": True}, {"ret": 3}):
        fx = {"conv": "cdecl", "args": ["int"], "ret": "int", "cases": [{"args": [5]}],
              "callees": {hex(CALLEE): dict({"name": "cb", "conv": "stdcall", "args": ["int"]}, **spec)}}
        rep = make(code(ORIG), code(REW), fx, callee).run()
        assert rep.ok, rep.summary()
        assert rep.coverage["insns"] == [6, 6] and rep.coverage["blocks"] == [1, 1], spec


def test_spans_merge_consecutive_unhit_blocks():
    blocks = [[0, 4, 2, 2], [4, 8, 2, 0], [8, 12, 3, 0], [12, 16, 1, 1], [16, 20, 2, 0]]
    assert cov.spans(blocks) == [[4, 12, 5], [16, 20, 2]]


def test_rdtsc_mid_block_does_not_change_coverage():
    # mov ecx,1; rdtsc; add eax,ecx; ret  (the RDTSC hook skips the instruction and restarts the block after it)
    code = bytes.fromhex("B901000000 0F31 01C8 C3")
    fx = {"conv": "cdecl", "args": [], "ret": "i64", "cases": [{"args": []}]}
    rep = make(code, code, fx).run()
    assert rep.ok, rep.summary()
    assert rep.coverage["insns"] == [4, 4] and rep.coverage["blocks"] == [1, 1]
