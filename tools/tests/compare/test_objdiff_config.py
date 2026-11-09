"""objdiff_config.py: unit plan, objdiff.json shape, report parsing and the agreement count (no VC6/objdiff)."""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

import objdiff_config as O  # noqa: E402


def fake_repo(tmp_path):
    repo = tmp_path / "eq-x"
    (repo / "src" / "Game").mkdir(parents=True)
    (repo / "src" / "Game" / "Foo.cpp").write_text(
        "// FUNCTION: TRILOGY 0x00401000\nint CFoo::Bar(int a)\n{ return a; }\n"
        "// FUNCTION: TRILOGY 0x00405000\nint Moved(void)\n{ return 0; }\n")
    entries = [
        {"address": "00401000", "subsystem": "Game/Foo", "state": "tested"},
        {"address": "00402000", "subsystem": "Game/Foo", "state": "todo"},
        {"address": "00403000", "subsystem": "Net/Rdp", "state": "todo"},
        {"address": "00404000", "subsystem": "Net/Rdp", "state": "library"},
        {"address": "00405000", "subsystem": "Net/Rdp", "state": "tested"},     # its source is Game/Foo.cpp
    ]
    return repo, entries


def test_plan_units(tmp_path):
    repo, entries = fake_repo(tmp_path)
    units = {u.name: u for u in O.plan_units(repo, entries)}
    assert set(units) == {"Game/Foo", "Net/Rdp"}
    assert units["Game/Foo"].source == "src/Game/Foo.cpp"
    assert units["Game/Foo"].vas == [0x401000, 0x402000, 0x405000]
    assert units["Game/Foo"].annotated == {0x401000: "CFoo::Bar", 0x405000: "Moved"}
    assert units["Net/Rdp"].source is None and units["Net/Rdp"].vas == [0x403000]


def test_config_paths_are_relative_to_the_project(tmp_path):
    repo, entries = fake_repo(tmp_path)
    units = O.plan_units(repo, entries)
    build = tmp_path / "build" / "trilogy"
    cfg = O.config(repo, units, build / "delink", build / "objdiff",
                   {"Game/Foo": {"_eqd_fn_0x00401000": "?Bar@CFoo@@QAEHH@Z"}})
    by = {u["name"]: u for u in cfg["units"]}
    assert by["Game/Foo"]["target_path"] == "../build/trilogy/delink/units/Game/Foo.obj"
    assert by["Game/Foo"]["base_path"] == "../build/trilogy/objdiff/src_Game_Foo.obj"
    assert by["Game/Foo"]["symbol_mappings"] == {"_eqd_fn_0x00401000": "?Bar@CFoo@@QAEHH@Z"}
    assert "base_path" not in by["Net/Rdp"]
    assert cfg["custom_make"] == "cmd" and cfg["custom_args"][-3:] == ["build", "--repo", "."]
    assert {c["id"] for c in cfg["progress_categories"]} == {"Game", "Net"}


def test_report_functions_and_agreement():
    rep = {"units": [{"name": "Game/Foo", "functions": [
        {"name": "_eqd_fn_0x00401000", "fuzzy_match_percent": 100.0},
        {"name": "_eqd_fn_0x00402000", "fuzzy_match_percent": 97.5},
        {"name": "_eqd_fn_0x00403000"}]}]}
    od = O.report_functions(rep)
    assert od == {0x401000: (100.0, "Game/Foo"), 0x402000: (97.5, "Game/Foo"), 0x403000: (0.0, "Game/Foo")}
    results = [{"va": "0x00401000", "name": "a", "match": "identical"},
               {"va": "0x00402000", "name": "b", "match": "identical"},
               {"va": "0x00403000", "name": "c", "match": "diff"},
               {"va": "0x00409000", "name": "d", "match": "identical"}]
    ag = O.agreement(results, od)
    assert ag["identical"] == 3 and ag["identical_agree"] == 1
    assert [x[0] for x in ag["identical_below_100"]] == ["0x00402000"]
    assert ag["cells"]["identical/absent"] == 1 and ag["cells"]["diff/<100"] == 1
