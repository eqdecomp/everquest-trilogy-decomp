r"""Smoke test for the context packer (context\pack_context.py). Needs the Trilogy export; no Ghidra.

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\context
"""
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "context"))
import pack_context as pc   # noqa: E402

try:
    EX = pc.Export("trilogy")
except SystemExit:
    EX = None

pytestmark = pytest.mark.skipif(EX is None, reason="no trilogy export")


@pytest.fixture(scope="module")
def parts():
    repo = EX.ws.root / "eq-trilogy"
    return pc.Headers(repo, "trilogy"), pc.Queue(repo)


def bundle(parts, addr):
    return pc.build_bundle(EX, parts[0], parts[1], addr)


def test_get_language_name(parts):
    t = bundle(parts, "004C8FCE")
    for needle in ("GetLanguageName @ 0x004C8FCE", "certain", "eqclassic", "EverQuest/EverQuest",
                   "// FUNCTION: TRILOGY 0x004c8fce", "// ORIGINAL FILE: EverQuest/EverQuest.cpp",
                   "## Ghidra pseudocode", "switch(", "## Assembly", "## Callers (", "...",
                   "Visual C++ 6.0", "_guess"):
        assert needle in t, needle
    assert len(t) < 30000


def test_bounded_and_structs(parts):
    t = bundle(parts, "004B03EB")      # EQPlayer ctor: big, uses the EQPlayer header
    assert "### EQPlayer" in t
    assert len(t) < 120000


def test_cli_writes_bundle(tmp_path):
    pc.main(["trilogy", "0x4c8fce", "--out", str(tmp_path)])
    files = list(tmp_path.glob("004C8FCE_*.md"))
    assert len(files) == 1 and files[0].stat().st_size > 1000


def test_gfx_slots_named(parts):
    slots = EX.gpa_slots
    assert len(slots) > 200
    assert slots["006E5250"][0] == "t3dFloatFastSine" and slots["006E524C"][0] == "t3dFloatFastCosine"
    hdr = pc.gfx_header_names(EX.ws.root / "eq-trilogy", "trilogy")
    assert hdr and all(slots[a][0] == n for a, n in hdr.items())      # eqgfx.h agrees with the loader
    t = bundle(parts, "004063DC")      # CDisplay::renderDoll: calls t3dFloatFastCosine/Sine
    assert "-> t3dFloatFastSine (GetProcAddress slot)" in t
    assert '= **GetProcAddress "t3dFloatFastCosine"' in t


def test_ecx_evidence(parts):
    t = bundle(parts, "004C8FCE")      # CEverQuest::GetLanguageName: every caller loads ECX, `this` unused
    assert "| ECX evidence | ECX set before the call at 11/11" in t
    assert "likely a `__thiscall` method that ignores `this`" in t
    assert "ECX set before call |" in t
    f = EX.functions["00419867"]       # EQ_Character::Skill: reads ECX
    line, _ = pc.ecx_summary(EX, f, EX.callgraph["00419867"], EX.asm("00419867"))
    assert "reads ECX before writing it" in line
    # `MOV ECX,ESI; JZ 00419FF7; CALL ...; JMP ...; 00419FF7: CALL Skill`: the walk follows the JZ
    sites = dict(pc.ecx_at_call_sites(EX, EX.callgraph["00419867"]["callers"]))
    assert sites["00419FF7"] == "set:00419FE7:MOV ECX,ESI"


def test_ecx_pushed_is_an_argument():
    # pack_physics: callers do `LEA ECX,[EBP + -0x1a]; PUSH ECX` -> an argument, not `this`
    cg = EX.callgraph
    sites = pc.ecx_at_call_sites(EX, cg["004A3534"]["callers"])
    assert sites and all(h.startswith("arg") for _, h in sites)
    line, _ = pc.ecx_summary(EX, EX.functions["004A3534"], cg["004A3534"], EX.asm("004A3534"))
    assert "at 0/3 direct call sites -> likely static" in line and "3 site(s) load ECX only to push" in line


def test_switch_tables(parts):
    t = bundle(parts, "00430831")      # byte index table + jump table
    assert "## Switch tables (1)" in t and "byte index table `00430D2B`" in t
    row = next(l for l in t.splitlines() if l.startswith("| `00430C2F` |"))
    assert "0x1e" in row and "default" in row                  # Ghidra: JA caseD_1e
    t = bundle(parts, "004065C3")      # DEC EAX before the bound check: cases start at 1
    assert "bias -1 from `DEC EAX`" in t
    sw = pc.switch_tables(EX, EX.asm("004F1EF0"))[0]           # XOR ECX,ECX; MOV CL,byte ptr [EAX + T]
    assert sw["index_table"] == "004F2154" and sw["bias"] == -3 and sw["bound"] == 9
    sw = pc.switch_tables(EX, EX.asm("004A11E3"))[0]           # PUSH 0xe; POP EBX; CMP EAX,EBX
    assert sw["bound"] == 0xE and sw["cases"][0][0] == 1


def test_switch_false_bounds_rejected():
    # memcpy: `CMP ECX,0x8; JC ...; NEG ECX; JMP [ECX*4 + T]` has no bound check -> not decoded
    sw = {x["at"]: x for x in pc.switch_tables(EX, EX.asm("00521B10"))}
    assert sw["00521CD2"]["error"] and not sw["00521CD2"]["cases"]
    # _output: `MOV AL,byte ptr [EAX + T]` earlier is not the index table feeding the jump; the state
    # table index is `MOVSX EAX,...; SAR EAX,4; CMP EAX,7; JA` -> plain table, 8 entries
    sw = {x["at"]: x for x in pc.switch_tables(EX, EX.asm("0052876D"))}["005287DB"]
    assert sw["index_table"] is None and sw["bound"] == 7 and sw["default"] == "00528E75"
    # the bias is redone after the JA (`LEA EAX,[ECX + -0x32]` twice) -> still decoded, cases 50..66
    sw = {x["at"]: x for x in pc.switch_tables(EX, EX.asm("0042C654"))}["0042F1C3"]
    assert sw["bound"] == 0x10 and sw["bias"] == -0x32 and sw["cases"][0][0] == 0x32
    # every decoded target lies inside its function
    for a in ("00521B10", "00525EF0", "0052876D", "0043B010"):
        asm = EX.asm(a)
        for s in pc.switch_tables(EX, asm):
            for _, tgt, _ in s["cases"]:
                assert any(int(lo, 16) <= tgt <= int(hi, 16) for lo, hi in asm["ranges"]), (a, s["at"])


def _extent_reports_switches():
    import inspect
    sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))
    from eqd_unicorn.extent import function_extent
    return "switches" in inspect.signature(function_extent).parameters


needs_extent_switches = pytest.mark.skipif(not _extent_reports_switches(),
                                           reason="extent.function_extent has no `switches` yet")


@needs_extent_switches
def test_switch_list_comes_from_extent():
    # send_social: the second switch has no bound check and Ghidra never decoded its cases; extent's
    # unbounded-table scan gets all 81 entries
    sw = {x["at"]: x for x in pc.switch_tables(EX, EX.asm("004CC92F"))}
    assert set(sw) == {"004CCB1B", "004CD9E7"}
    assert sw["004CD9E7"]["how"] == "scan" and len(sw["004CD9E7"]["cases"]) == 81 and not sw["004CD9E7"]["error"]
    assert "no bound check in the code" in sw["004CD9E7"]["note"]
    # the same set extent reports, for the giant with seven switches (one a byte-table switch)
    found = pc.extent_switches(EX, EX.asm("0043B010"))
    assert [j for j, *_ in found] == [int(x["at"], 16) for x in pc.switch_tables(EX, EX.asm("0043B010"))]
    assert len(found) == 7 and sum(h == "bytetable" for *_, h in found) == 1


def test_switch_fallback_without_extent(monkeypatch):
    # an extent that can't report switches (None): the listing's own jumps, as before
    monkeypatch.setattr(pc, "extent_switches", lambda ex, asm: None)
    sw = pc.switch_tables(EX, EX.asm("00430831"))
    assert len(sw) == 1 and sw[0]["index_table"] == "00430D2B" and sw[0]["how"] == "text" and sw[0]["cases"]


def test_truncated_listing_says_so_and_full_files_are_written(tmp_path, parts):
    t = pc.build_bundle(EX, parts[0], parts[1], "0043B010", full_dir=tmp_path)
    assert "**TRUNCATED: this listing stops at line 400 of " in t
    assert "**TRUNCATED: this listing stops at instruction 700 of 22068" in t
    assert "`FULL_ASM.txt`, next to this file" in t and "`FULL_PSEUDO.c`, next to this file" in t
    assert len((tmp_path / pc.FULL_ASM).read_text(encoding="utf-8").splitlines()) == 22068
    assert (tmp_path / pc.FULL_PSEUDO).stat().st_size > 100000
    # no full_dir: the text points at the export instead
    t = pc.build_bundle(EX, parts[0], parts[1], "0043B010")
    assert "TRUNCATED" in t and "export\\asm\\" in t and "next to this file" not in t


def test_stale_full_files_removed(tmp_path, parts):
    pc.build_bundle(EX, parts[0], parts[1], "0043B010", full_dir=tmp_path)
    assert (tmp_path / pc.FULL_ASM).exists()
    t = pc.build_bundle(EX, parts[0], parts[1], "00401011", full_dir=tmp_path)   # 6 instructions
    assert not (tmp_path / pc.FULL_ASM).exists() and not (tmp_path / pc.FULL_PSEUDO).exists()
    assert "TRUNCATED" not in t


@needs_extent_switches
def test_code_outside_the_ghidra_listing(parts):
    # process_network_connections: Ghidra's body ends before the EH epilogue that extent reaches; the bundle
    # lists those 9 instructions (from the binary) so the worker sees all the code the coverage gate counts
    extra = pc.outside_listing(EX, EX.asm("004F0290"))
    assert [a for a, *_ in extra][:2] == ["004F0B69", "004F0B70"] and len(extra) == 9
    assert extra[-1][2] == "RET"
    t = bundle(parts, "004F0290")
    assert "## Code outside the Ghidra listing (9 instructions)" in t and "004F0B80  c3" in t
    # a function Ghidra covers whole gets no such section
    assert pc.outside_listing(EX, EX.asm("004C8FCE")) == []
    assert "Code outside the Ghidra listing" not in bundle(parts, "004C8FCE")
