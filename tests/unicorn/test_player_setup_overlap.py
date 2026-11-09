"""Supplemental raw-payload witness for SetupEQPlayers' actor-pointer reload."""
from __future__ import annotations

import hashlib
import json
import os
import struct
import tempfile
from pathlib import Path

from conftest import REPO, link_sources
from eqd_unicorn import load_fixture


FIXTURE = REPO / "tests/unicorn/EverQuest/Display/SetupEQPlayers.json"
SOURCE = REPO / "src/EverQuest/Display.cpp"
RELOAD_BEFORE_CLEAR = (
    "            info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);\n"
    "            info->lastPhysicsTime_guess = 0;"
)


def _fixture_with_nan_payload() -> dict:
    fx = load_fixture(FIXTURE)
    # Unicorn's callee script takes a Python float, which it packs as binary64 before FLD/FSTP. Starting
    # from the exact binary32 payload verifies that x87 load/store preserves the pointer-overlap bytes.
    raw_nan = struct.unpack("<f", bytes.fromhex("0060c07f"))[0]
    fx.pop("fuzz", None)
    fx["cases"] = [{
        "name": "NaN payload overlaps actor pointer; post-clamp reload faults at 0x7e",
        "memory": [{"addr": "@player+0x70", "u32": "@player+0x2a"}],
        "callees": {
            "0x00414245": {
                "name": "CDisplay::GetFloorHeight_guess",
                "conv": "thiscall",
                "this": "ptr",
                "args": ["float", "float", "float"],
                "ret_float": raw_nan,
            }
        },
    }]
    return fx


def _run(harness, fx: dict, source: Path, name: str):
    return harness.difftest(fx, [source, *link_sources()], base_dir=REPO, name=name).run()


def test_nan_payload_requires_actor_reload_before_clear(harness):
    fx = _fixture_with_nan_payload()
    source_text = SOURCE.read_text(encoding="utf-8")
    assert source_text.count(RELOAD_BEFORE_CLEAR) == 1

    accepted = _run(harness, fx, SOURCE, "setup_nan_reload_accepted")
    assert accepted.ok, accepted.summary()
    good = accepted.cases[0]
    assert good.original.fault is not None and good.original.fault[1] == 0x7E, good.original.error
    assert good.rewrite.fault == good.original.fault, good.rewrite.error

    mutant_text = source_text.replace(RELOAD_BEFORE_CLEAR, "            info->lastPhysicsTime_guess = 0;", 1)
    assert mutant_text != source_text
    evidence_root = Path(os.environ.get("EQD_UNICORN_BUILD", REPO / "build" / "trilogy" / "unicorn"))
    evidence_root.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix="setup_deleted_reload_", dir=evidence_root) as temp:
        mutant_source = Path(temp) / "Display_deleted_reload.cpp"
        mutant_source.write_text(mutant_text, encoding="utf-8")
        rejected = _run(harness, fx, mutant_source, "setup_nan_reload_deleted")

    bad = rejected.cases[0]
    assert not bad.ok, "the compiled negative unexpectedly matched the original"
    assert bad.original.fault == good.original.fault, bad.original.error
    assert bad.rewrite.fault is not None and bad.rewrite.fault[1] != good.original.fault[1], bad.rewrite.error

    body_marker = "// FUNCTION: TRILOGY 0x0041527c"
    body_start = mutant_text.index(body_marker)
    body_end = mutant_text.find("\n// FUNCTION:", body_start + len(body_marker))
    mutant_body = mutant_text[body_start:] if body_end < 0 else mutant_text[body_start:body_end]
    payload = {
        "address": "0x0041527C",
        "witness": "binary32 NaN bits 0x7FC06000 overlap bytes +0x72..+0x75 of the actor pointer at player+0x70",
        "accepted": {
            "ok": accepted.ok,
            "original_fault": list(good.original.fault),
            "rewrite_fault": list(good.rewrite.fault),
            "calls": [entry.name for entry in good.original.trace],
        },
        "deleted_reload_negative": {
            "rejected": not bad.ok,
            "original_fault": list(bad.original.fault),
            "rewrite_fault": list(bad.rewrite.fault),
            "differences": bad.diffs,
            "original_calls": [entry.name for entry in bad.original.trace],
            "rewrite_calls": [entry.name for entry in bad.rewrite.trace],
        },
        "compiled_negative_body_sha256": hashlib.sha256(mutant_body.encode("utf-8")).hexdigest(),
        "compiled_negative_body": mutant_body,
        "source_sha256": hashlib.sha256(source_text.encode("utf-8")).hexdigest(),
        "fixture_sha256": hashlib.sha256(FIXTURE.read_bytes()).hexdigest(),
    }
    evidence_dir = Path(os.environ.get("EQD_NEGATIVE_EVIDENCE", evidence_root / "player_setup"))
    evidence_dir.mkdir(parents=True, exist_ok=True)
    (evidence_dir / "supplemental-player-setup.json").write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
