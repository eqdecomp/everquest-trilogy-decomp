"""Compiled call-order witness for ChangeDag's cached dictionary callback and world reload."""
from __future__ import annotations

import hashlib
import json
import os
from pathlib import Path

from conftest import REPO, link_sources
from eqd_unicorn import load_fixture


FIXTURE = REPO / "tests/unicorn/EverQuest/Display/ChangeDag.json"
SOURCE = REPO / "src/EverQuest/Display.cpp"


def _run(harness, fixture: dict, source: Path, name: str):
    diff = harness.difftest(fixture, [source, *link_sources()], base_dir=REPO, name=name)
    original_intercept = diff.m._intercept

    def intercept(target, is_stub):
        state = diff.m.state
        if state is not None:
            key = state.aliases.get(target, target)
            spec = state.callees.get(key)
            if spec is not None and spec.name == "t3dGetPointerFromDictionary":
                call_index = state.counts.get(key, 0)
                for write in spec.writes:
                    if write.get("addr") == 0x60000004:
                        # The first lookup changes world1 to world2; the fallback lookup changes world2 to
                        # world3 before the new DM sprite is created.
                        write["hex"] = "002f0060" if call_index == 0 else "00300060"
        original_intercept(target, is_stub)

    diff.m._intercept = intercept
    return diff.run()


def _calls(trace) -> list[dict]:
    return [{"target": c.target, "name": c.name, "args": list(c.args)} for c in trace]


def test_fallback_reloads_world_after_dictionary_callback(harness):
    fixture = load_fixture(FIXTURE)
    case = next(c for c in fixture["cases"] if c.get("name", "").startswith("fallback uses cached dictionary"))
    fixture["cases"] = [case]
    fixture.pop("fuzz", None)

    source_text = SOURCE.read_text(encoding="utf-8")
    accepted = _run(harness, fixture, SOURCE, "change_dag_reload_accepted")
    assert accepted.ok, accepted.summary()
    good = accepted.cases[0]
    lookup_calls = [c for c in good.original.trace if c.name == "t3dGetPointerFromDictionary"]
    create_call = next(c for c in good.original.trace if c.name == "s3dCreateDMSprite")
    assert len(lookup_calls) == 2
    assert lookup_calls[0].target == lookup_calls[1].target, lookup_calls
    assert [c.args[0] for c in lookup_calls] == [0x60002E00, 0x60002F00], lookup_calls
    assert create_call.args[0] == 0x60003000, create_call

    # ChangeDag (identical since 48e27472) reads the member world_guess at each use instead of a volatile
    # local reload; the negative caches it in a local before the fallback lookup, so creation sees the old world.
    lookup = "            definition = getPointer(world_guess, nameBuffer);\n            /* fake match"
    create = "                sprite = EQ_GLOBAL(s3dCreateDMSprite_guess_t, 0x006E50CC)(world_guess, definition, 0);"
    assert source_text.count(lookup) == 1 and source_text.count(create) == 1
    negative_text = source_text.replace(lookup, "            void* cachedWorld = world_guess;\n"
                                        "            definition = getPointer(cachedWorld, nameBuffer);\n"
                                        "            /* fake match", 1)
    negative_text = negative_text.replace(create, create.replace("(world_guess,", "(cachedWorld,"), 1)
    evidence_root = Path(os.environ.get("EQD_UNICORN_BUILD", REPO / "build" / "trilogy" / "unicorn"))
    evidence_root.mkdir(parents=True, exist_ok=True)
    negative_source = evidence_root / "Display_deleted_fallback_world_reload.cpp"
    negative_source.write_text(negative_text, encoding="utf-8")
    rejected = _run(harness, fixture, negative_source, "change_dag_reload_deleted")
    bad = rejected.cases[0]
    assert not bad.ok, "compiled deletion of the fallback world reload unexpectedly matched"
    original_create = next(c for c in bad.original.trace if c.name == "s3dCreateDMSprite")
    rewrite_create = next(c for c in bad.rewrite.trace if c.name == "s3dCreateDMSprite")
    assert original_create.args[0] == 0x60003000, original_create
    assert rewrite_create.args[0] == 0x60002F00, rewrite_create

    body_start = source_text.index("// FUNCTION: TRILOGY 0x004119a6")
    body_end = source_text.find("\n// FUNCTION:", body_start + 1)
    body = source_text[body_start:] if body_end < 0 else source_text[body_start:body_end]
    negative_start = negative_text.index("// FUNCTION: TRILOGY 0x004119a6")
    negative_end = negative_text.find("\n// FUNCTION:", negative_start + 1)
    negative_body = negative_text[negative_start:] if negative_end < 0 else negative_text[negative_start:negative_end]
    evidence = {
        "address": "0x004119A6",
        "witness": "the fallback dictionary call updates display+4 to 0x60003000 immediately before sprite creation",
        "accepted": {"ok": accepted.ok, "calls": _calls(good.original.trace)},
        "compiled_negative": {
            "rejected": not bad.ok,
            "original_create_world": original_create.args[0],
            "rewrite_create_world": rewrite_create.args[0],
            "differences": bad.diffs,
            "original_calls": _calls(bad.original.trace),
            "rewrite_calls": _calls(bad.rewrite.trace),
            "source_file": str(negative_source),
            "body_sha256": hashlib.sha256(negative_body.encode("utf-8")).hexdigest(),
        },
        "accepted_body_sha256": hashlib.sha256(body.encode("utf-8")).hexdigest(),
        "fixture_sha256": hashlib.sha256(FIXTURE.read_bytes()).hexdigest(),
    }
    evidence_dir = Path(os.environ.get("EQD_NEGATIVE_EVIDENCE", evidence_root / "change_dag"))
    evidence_dir.mkdir(parents=True, exist_ok=True)
    (evidence_dir / "supplemental-change-dag.json").write_text(
        json.dumps(evidence, indent=2) + "\n", encoding="utf-8"
    )
