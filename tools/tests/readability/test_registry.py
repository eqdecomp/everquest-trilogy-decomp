"""registry.py on the synthetic repo: sightings from every source, canonical names, each conflict kind."""
import collections

import registry as R


def kinds(reg):
    return collections.Counter(k for k, *_ in reg.conflicts())


def test_sightings_and_canonical(repo):
    reg = R.build(repo, "trilogy")
    e = reg.entries[0x00401100]
    assert e.ann_name == "FUN_00401100"
    assert {s.source for s in e.sightings} >= {"ann", "queue", "fixture", "csv:mac", "define"}
    assert e.canonical == "FUN_00401100"                       # the source definition wins
    assert reg.entries[0x00401000].canonical == "Widget::Total_guess"
    assert reg.entries[0x00402000].csv_best.name == "UseWidget_guess"    # low -> _guess, as apply_names
    g = reg.entries[0x006E0020]
    assert g.kind == "global" and set(g.define_names) == {"g_scale", "g_scale_factor"}
    assert reg.entries[0x00401500].kind == "fn"                # only a call macro, no definition
    assert reg.lookup("Total_guess") == [0x00401000]


def test_conflicts(repo):
    reg = R.build(repo, "trilogy")
    c = kinds(reg)
    assert c["define_names"] == 1                              # 0x006E0020: g_scale / g_scale_factor
    assert c["spelling"] == 1                                  # 0x006E0020 / 0x006e0020
    assert c["csv_unapplied"] == 1                             # 00401100 FUN_ in source, mac name exists
    assert c["queue_fun"] == 0 and c["queue_differs"] == 0
    det = {k: d for k, v, d in reg.conflicts()}
    assert "Widget::Accumulate_guess" in det["csv_unapplied"]


def test_eqmain_queue_fun_and_client_split(repo):
    reg = R.build(repo, "eqmain")
    assert set(reg.entries) >= {0x10001000, 0x10040000}
    assert 0x00401000 not in reg.entries                       # trilogy's sources are not eqmain's
    assert kinds(reg)["queue_fun"] == 1                        # named in source, FUN_ in the queue


def test_macro_shadows_and_fixture_differs(repo):
    (repo / "src/EverQuest/other.cpp").write_text(
        (repo / "src/EverQuest/other.cpp").read_text().replace("#define WidgetTotal", "#define FUN_00401100"),
        encoding="utf-8")
    p = repo / "tests/unicorn/EverQuest/other/UseWidget.json"
    p.write_text(p.read_text().replace('"function": "UseWidget_guess"', '"function": "UseWidgetOld"'),
                 encoding="utf-8")
    c = kinds(R.build(repo, "trilogy"))
    assert c["macro_shadows"] == 1 and c["fixture_differs"] == 1


def test_wrapped_defines_are_not_names(repo):
    p = repo / "src/EverQuest/other.cpp"
    p.write_text(p.read_text() + "#define SCALE_PLUS(o) (EQ_GLOBAL(int, 0x006E0020) + (o))\n", encoding="utf-8")
    e = R.build(repo, "trilogy").entries[0x006E0020]
    assert "SCALE_PLUS" not in e.define_names                  # a use of the VA, not another name for it
    assert any(s.name == "SCALE_PLUS" for s in e.by("define_use"))


def test_write_tables(repo, tmp_path):
    reg = R.build(repo, "trilogy")
    R.write_tables(reg, tmp_path / "out")
    t = (tmp_path / "out/trilogy.tsv").read_text(encoding="utf-8").splitlines()
    assert t[0].startswith("va\tkind\tname") and any(l.startswith("00401100\tfn\tFUN_00401100") for l in t)
    assert "define_names" in (tmp_path / "out/trilogy.conflicts.tsv").read_text(encoding="utf-8")
