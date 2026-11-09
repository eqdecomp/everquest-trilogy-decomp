import hashlib
import json
from pathlib import Path

from conftest import REPO, link_sources
from eqd_unicorn import load_fixture


def test_crc_reloads_stat_size_after_fread(harness, tmp_path):
    fixture_path = Path(__file__).with_name("calculateFileCRC32_guess.json")
    fixture = load_fixture(fixture_path)
    fixture["cases"] = [{
        "name": "fread mutates stat size before crc",
        "args": ["mutated.dat", "@out"],
        "callees": {
            "0x00523BE0": {
                "name": "__stat", "conv": "cdecl", "args": ["cstr", "ptr"], "ret": 0,
                "writes": [{
                    "arg": 1, "offset": 0,
                    "hex": "000000000000000000000000000000000000000020000000000000000000000000000000",
                }],
            },
            "0x00522038": {
                "name": "fread", "conv": "cdecl", "args": ["ptr", "u32", "u32", "ptr"], "ret": 1,
                "writes": [{"arg": 0, "offset": 20, "hex": "08000000"}],
            },
        },
        "expect": {"ret": 1},
    }]
    fixture.pop("fuzz", None)

    stat_record = []
    fixture["cases"][0]["callees"]["0x00523BE0"]["ret"] = (
        lambda _index, call: (stat_record.append(call.args[1]) or 0)
    )
    fixture["callees"]["@alloc_fn"]["ret"] = lambda _index, _call: stat_record[-1]

    sources = [REPO / "src/EverQuest/EQMisc.cpp"] + link_sources()
    report = harness.difftest(fixture, sources, name="crc_live_size", base_dir=REPO).run()
    assert report.ok, report.summary()
    for side in (report.cases[0].original, report.cases[0].rewrite):
        calls = {call.name: call.args for call in side.trace}
        assert calls["GlobalAlloc"][1] == 32
        assert calls["fread"][1] == 32
        assert calls["crc32mem"][1] == 8

    source = (REPO / "src/EverQuest/EQMisc.cpp").read_text(encoding="utf-8")
    body_start = source.index("int calculateFileCRC32_guess(")
    body_end = source.index("\n// FUNCTION:", body_start)
    body = source[body_start:body_end]
    needle = "stat_record + 0x14"
    local_at = body.rfind(needle)
    assert local_at >= 0 and body.count(needle) == 3
    at = body_start + local_at
    mutated_source = tmp_path / "EQMisc_mutated.cpp"
    mutated_source.write_text(source[:at] + "stat_record - 0x14" + source[at + len(needle):], encoding="utf-8")
    negative = harness.difftest(
        fixture,
        [mutated_source] + link_sources(),
        name="crc_live_size_stale_reload_negative",
        base_dir=REPO,
    ).run()
    assert not negative.ok, "compiled stale-size mutation unexpectedly passed"
    original_crc = [call.args[1] for call in negative.cases[0].original.trace if call.name == "crc32mem"]
    mutated_crc = [call.args[1] for call in negative.cases[0].rewrite.trace if call.name == "crc32mem"]
    assert original_crc == [8]
    assert mutated_crc == [32]
    assert any(
        "trace:" in diff or "writes:" in diff
        for case in negative.failures
        for diff in case.diffs
    ), negative.summary()

    def crc_call(side):
        return [list(call.args) for call in side.trace if call.name == "crc32mem"]

    evidence = {
        "function": "0x004A3ED7 calculateFileCRC32_guess",
        "source_body_sha256": hashlib.sha256(source[body_start:body_end].encode("utf-8")).hexdigest(),
        "fixture": "tests/unicorn/EverQuest/EQMisc/calculateFileCRC32_guess.json",
        "dynamic_case": "fread mutates stat size before crc",
        "positive_sizes": {"GlobalAlloc": 32, "fread": 32, "crc32mem": 8},
        "compiled_negative": {
            "mutation": "third `stat_record + 0x14` becomes `stat_record - 0x14`",
            "crc_calls": {
                "original": crc_call(negative.cases[0].original),
                "mutated_rewrite": crc_call(negative.cases[0].rewrite),
            },
            "diffs": [diff for case in negative.failures for diff in case.diffs],
        },
    }
    evidence_path = tmp_path / "crc_live_size_evidence.json"
    evidence_path.write_text(json.dumps(evidence, indent=2) + "\n", encoding="utf-8")
