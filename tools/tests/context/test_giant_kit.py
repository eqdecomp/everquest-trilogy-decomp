r"""Region kit (context\giant_kit.py) and mutate_check's giant scoping (--lines, --max, --region-markers).

The pure-text tests need nothing. The integration tests need the Trilogy export, the original binary and the
fixed extent (eqd_unicorn.extent.function_extent(..., switches=)); they skip without them.

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\context\test_giant_kit.py
"""
import inspect
import re
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "context"))
sys.path.insert(0, str(ROOT / "unicorn" / "tools"))
sys.path.insert(0, str(ROOT / "unicorn"))

import giant_kit as gk  # noqa: E402
import mutate_check as mc  # noqa: E402
import mutations as mu  # noqa: E402

CARD = """# 0042C654 FUN_0042c654 - something
Does: draws.
Regions:
 T1 42C654-42CDD5 1.8K prologue, EQ_Overlay::draw, 50-way race/class help (JT 42C6CF)
 T2 42CDD5-42D35A 1.4K AGNOSTIC   T3 42D35A-42D8BC 1.4K deity ids (BERTOXXULOUS)
 T9 42F852-42FC7C 1.1K city names (JT 42F84B)   T10 42FBF2-42FC7C 0.1K tail notice
 M4 448F0D-44D2CB 17.0K near-copy; split at 44909B, 44986E and 44A0B3
 R14 4420F0-442134 options dispatch; pages: p0 442134 (0.7K), p1 4423D5 (2.8K)
 Last case body 47A011-47B837 4.9K: if x
Fixture inputs: ECX = something 111111-222222
"""


# ---------------------------------------------------------------- pure text

def test_parse_card_regions_anchors_and_stop():
    regs, anchors = gk.parse_card(CARD)
    byid = {r["id"]: r for r in regs}
    assert [r["id"] for r in regs][:5] == ["T1", "T2", "T3", "T9", "T10"]
    assert byid["T1"]["start"] == 0x42C654 and byid["T1"]["end"] == 0x42CDD5
    assert byid["T1"]["label"].startswith("prologue, EQ_Overlay::draw")      # the size token is dropped
    assert byid["T3"]["label"] == "deity ids (BERTOXXULOUS)"
    assert byid["M4"]["end"] == 0x44D2CB
    assert any(r["id"].startswith("u") and r["start"] == 0x47A011 for r in regs)   # a range without an id
    assert not any(r["start"] == 0x111111 for r in regs)                      # after "Fixture inputs"
    assert {0x44909B, 0x44986E, 0x44A0B3} <= set(anchors)
    assert anchors[0x442134] == "p0" and anchors[0x4423D5] == "p1"


def test_parse_card_without_regions():
    assert gk.parse_card("# x\nDoes: nothing\n") == ([], {})


def test_norm_token_keys_labels_by_address():
    assert gk.norm_token("s_A___B_0054543c") == gk.norm_token("s_A_-_B_0054543c") == "@0054543c"
    assert gk.norm_token("WriteTextHD_guess") == "WriteTextHD_guess"


def test_case_value_text():
    assert [gk.sx(v) for v in (0, 9, 10, -1, -0x23F, 0x1C)] == ["0", "9", "0xA", "-1", "-0x23F", "0x1C"]


def test_switch_groups_by_target():
    sw = {"cases": [(0, 0x10), (1, 0x20), (2, 0x10), (3, 0x30)]}
    assert gk.switch_groups(sw) == [(0x10, [0, 2]), (0x20, [1]), (0x30, [3])]


def test_pseudo_chunks_split_statements():
    c = ("\r\nvoid __fastcall f(int p)\r\n\r\n{\r\n  int a;\r\n  short b;\r\n  \r\n  a = g(1,\r\n        2);\r\n"
         "  if (a) {\r\n    h();\r\n  }\r\n  return;\r\n}\r\n")
    header, chunks = gk.pseudo_chunks(c)
    assert any("int a;" in h for h in header) and not any("g(1" in h for h in header)
    texts = ["\n".join(ls) for _n, ls in chunks]
    assert any("g(1," in t and "2);" in t for t in texts)                    # a call over two lines stays whole
    assert chunks[0][0] == len(header) + 1 or chunks[0][0] > 0


def test_region_blocks_and_lines():
    src = "\n".join([
        "// FUNCTION: TRILOGY 0x00401000",               # 1
        "int f(int a)",                                  # 2
        "{",                                             # 3
        "    // REGION R01 0x00401000-0x00401010: head",  # 4
        "    a = a + 1;",                                # 5
        "    // END REGION R01",                          # 6
        "    // REGION R02 0x00401010-0x00401020: tail",  # 7
        "    if (a < 3) a = a * 2;",                     # 8
        "    // END REGION R02",                          # 9
        "    a = a + 7;",                                # 10
        "    return a;",
        "}"])
    assert mu.region_blocks(src) == [("R01", 4, 6), ("R02", 7, 9)]
    assert mu.region_of_line(mu.region_blocks(src), 8) == "R02"
    assert mu.region_of_line(mu.region_blocks(src), 10) is None
    ms = mu.mutants(src, 0x401000, "trilogy")
    assert {m.line for m in ms} >= {5, 8, 10}
    assert {m.line for m in mu.in_lines(ms, mu.parse_line_ranges(["5", "8-9"]))} <= {5, 8, 9}
    assert mu.in_lines(ms, []) == ms
    chosen, counts = mu.select_per_region(ms, mu.region_blocks(src), 1)
    assert len(chosen) == 2 and counts["R01"] >= 1 and counts["R02"] >= 1 and counts[None] >= 1
    assert {mu.region_of_line(mu.region_blocks(src), m.line) for m in chosen} == {"R01", "R02"}
    rep = mc.region_report(mu.region_blocks(src), [{"region": "R01", "status": "caught"},
                                                     {"region": "R02", "status": "SURVIVED"}], counts)
    assert rep["R01"]["caught"] == 1 and rep["R02"]["survived"] == 1 and rep["R02"]["checked"] == 1


def test_line_range_errors_and_scaling():
    assert mu.parse_line_ranges(["3-5", "9"]) == [(3, 5), (9, 9)]
    for bad in ("x", "5-3", "1-"):
        with pytest.raises(ValueError):
            mu.parse_line_ranges([bad])
    assert mu.scaled_max(None) == 30 and mu.scaled_max(50) == 30
    assert mu.scaled_max(3802) == 30 + 38 and mu.scaled_max(22068) == 250 and mu.scaled_max(10 ** 6) == 250


def test_mutate_check_cli_flags(capsys):
    ap_help = inspect.getsource(mc.main)
    for flag in ("--lines", "--region-markers", "--region", "--max-per-region"):
        assert flag in ap_help


# ---------------------------------------------------------------- integration (export + binary)

def _kit(tmp_path, va, **kw):
    try:
        g, s = gk.run("trilogy", va, "all", tmp_path / f"{va:08X}", **kw)
    except RuntimeError as e:
        pytest.skip(str(e))
    except (FileNotFoundError, KeyError) as e:
        pytest.skip(f"no workspace binary/export: {e}")
    return g, s


@pytest.fixture(scope="module")
def tmp_mod(tmp_path_factory):
    return tmp_path_factory.mktemp("giant_kit")


@pytest.mark.parametrize("va,regions,switches", [(0x0046B07E, 6, 0), (0x0042C654, 10, 7)])
def test_kit_covers_every_instruction_and_switch(tmp_mod, va, regions, switches):
    g, s = _kit(tmp_mod, va)
    if g.source != "card":
        pytest.skip("region counts come from the workspace card build/giants/cards/<ADDR>.md (not in this checkout)")
    chk = s["check"]
    assert s["regions"] == regions and s["switches"] == switches
    assert chk["asm_files"] == regions
    assert chk["asm_missing"] == chk["asm_extra"] == chk["asm_duplicated"] == 0
    assert chk["skeleton_mismatch"] == [] and chk["skeleton_missing_switches"] == []
    assert chk["skeleton_regions"] == chk["skeleton_end_regions"] == regions
    out = Path(s["out"])
    for name in ("regions.json", "INDEX.md", "skeleton.cpp", "R01.asm", "R01.pseudo.c"):
        assert (out / name).is_file()
    assert "APPROXIMATE SLICE" in (out / "R01.pseudo.c").read_text(encoding="utf-8")
    sk = (out / "skeleton.cpp").read_text(encoding="utf-8")
    assert f"// FUNCTION: TRILOGY 0x{va:08x}" in sk and "NOT compilable-guaranteed" in sk
    starts = [int(m, 16) for m in re.findall(r"// REGION R\d+ 0x([0-9A-F]{8})-", sk)]
    assert starts == sorted(starts) and len(starts) == regions
    assert sk.count("/* TODO */") >= regions
    # regions tile the extent without overlap; edges are symmetric
    rs = g.regions
    assert all(a["end"] <= b["start"] for a, b in zip(rs, rs[1:]))
    out_n = sum(e["n"] for r in rs for e in r["out"] if e["to"] != "EXT")
    in_n = sum(e["n"] for r in rs for e in r["in"] if e["from"] != "EXT")
    assert out_n == in_n


def test_kit_dispatch_case_labels_equal_the_jump_tables(tmp_mod):
    g, s = _kit(tmp_mod, 0x0042C654)
    first = next(sw for sw in g.switches if sw["jmp"] == 0x0042C6CF)
    assert first["count"] == 50 and first["table"] == 0x42FC7C
    sk = (Path(s["out"]) / "skeleton.cpp").read_text(encoding="utf-8")
    targets = {int(t, 16) for t in re.findall(r"// -> 0x([0-9A-F]{8}) \(R\d+\)", sk.split("SWITCH jmp=0x0042CB22")[0])}
    table = {int.from_bytes(g.img.read(0x42FC7C + 4 * k, 4), "little") for k in range(50)}
    assert table <= targets
    deity = next(sw for sw in g.switches if sw["jmp"] == 0x0042F1C3)           # 17 indices -> 12 targets, bias -50
    assert deity["bias"] == -50 and len({t for _v, t in deity["cases"]}) == 12 and len(deity["cases"]) == 17


def test_kit_85kb_giant_and_derived_regions(tmp_mod):
    g, s = _kit(tmp_mod, 0x0043B010)
    assert s["extent_insns"] == 22068 and s["check"]["asm_missing"] == 0 and s["check"]["asm_duplicated"] == 0
    assert s["check"]["skeleton_mismatch"] == []
    assert max(r["bytes"] for r in g.regions) <= 4096 + 2048                    # the 17 KB block was split
    d, ds = _kit(tmp_mod / "nocard", 0x0046B07E, no_card=True)
    assert ds["source"] == "derived" and ds["regions"] >= 3
    assert ds["check"]["asm_missing"] == ds["check"]["asm_duplicated"] == 0
