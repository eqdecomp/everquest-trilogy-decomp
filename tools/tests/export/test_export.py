r"""Smoke test for the Ghidra export (export\export_client.py). Cheap checks on an export that already
exists; it does not run Ghidra.

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\export

Set EQD_EXPORT_CLIENT to test another client's export (default trilogy). Skips if there is no export.
"""
import json
import os
import re
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "ghidra" / "scripts"))
import eqd_ghidra as eqd   # noqa: E402  (no Java needed at import)

CLIENT = os.environ.get("EQD_EXPORT_CLIENT", "trilogy")
HEX = re.compile(r"^[0-9A-F]{8}$")
ws = eqd.Workspace()
OUT = ws.build / "export" / CLIENT

pytestmark = pytest.mark.skipif(not (OUT / "manifest.json").exists(), reason=f"no export at {OUT}")


def load(name):
    return json.loads((OUT / name).read_text(encoding="utf-8"))


def shards(kind):
    out = {}
    for p in sorted((OUT / kind).glob("*.jsonl")):
        for line in p.read_text(encoding="utf-8").splitlines():
            r = json.loads(line)
            assert r["address"] not in out, f"{kind}: {r['address']} twice"
            out[r["address"]] = (p.name, r)
    return out


@pytest.fixture(scope="module")
def manifest():
    return load("manifest.json")


@pytest.fixture(scope="module")
def functions():
    return {f["address"]: f for f in load("functions.json")["functions"]}


@pytest.fixture(scope="module")
def callgraph():
    return load("callgraph.json")["functions"]


def test_manifest(manifest, functions):
    assert manifest["schema_version"] == 1
    assert manifest["client"] == CLIENT
    assert manifest["counts"]["functions"] == len(functions)
    assert manifest["meta_fingerprint"] == manifest["fingerprint"]


def test_counts_match_coverage(manifest, functions):
    """Same function set and classes as coverage_report.py (build\\coverage.md)."""
    cov = ws.reports / f"{CLIENT}.coverage.json"
    if not cov.exists():
        pytest.skip("no coverage report")
    cov = json.loads(cov.read_text(encoding="utf-8"))
    assert len(functions) == cov["functions"]
    classes = {}
    for f in functions.values():
        classes[f["class"]] = classes.get(f["class"], 0) + 1
    assert classes == {k: v for k, v in cov["classes"].items() if v}
    row = next((line for line in (ws.build / "coverage.md").read_text(encoding="utf-8").splitlines()
                if line.startswith(f"| {CLIENT} |")), None)
    if row:
        assert int(row.split("|")[2]) == len(functions)


def test_function_records(functions):
    for a, f in functions.items():
        assert HEX.match(a) and HEX.match(f["end"]) and f["end"] >= a
        assert f["size"] > 0 and f["class"] in ("library", "import_thunk", "eh_funclet", "named", "unnamed")
        assert f["shard"].endswith(".jsonl")
        for s in f["strings"] + f["globals"]:
            assert HEX.match(s)


def test_known_function(functions, callgraph):
    f = functions["004C8FCE"]
    assert "GetLanguageName" in f["name"]
    # EQClassic's certain name; since 2026-10-02 the rewrite's CEverQuest:: form (manual, still certain)
    assert f["name_list"]["source"] in ("eqclassic", "manual") and f["name_list"]["confidence"] == "certain"
    assert f["source_file"] and f["source_file"].endswith("EverQuest.cpp")
    assert len(f["strings"]) >= 20               # one language name per case
    assert len(callgraph["004C8FCE"]["callers"]) >= 1


def test_pseudocode(manifest, functions):
    pc = shards("pseudocode")
    if not manifest["decompile"]["complete"]:
        pytest.skip("decompile run not finished")
    assert set(pc) == set(functions)
    for a, (shard, r) in pc.items():
        assert functions[a]["shard"] == shard
    ok = sum(r["status"] == "ok" for _, r in pc.values())
    assert ok >= 0.98 * len(functions)
    shard, r = pc["004C8FCE"]
    assert r["status"] == "ok" and "GetLanguageName" in r["c"] and "switch" in r["c"]


def test_asm(functions):
    asm = shards("asm")
    assert set(asm) == set(functions)
    shard, r = asm["004C8FCE"]
    assert r["instructions"][0][0] == "004C8FCE"
    for addr, raw, text, symbolic in r["instructions"]:
        assert HEX.match(addr) and re.fullmatch(r"([0-9a-f]{2})+", raw) and text and symbolic
    assert sum(len(i[1]) // 2 for i in r["instructions"]) <= functions["004C8FCE"]["size"]


def test_callgraph_symmetric(functions, callgraph):
    assert set(callgraph) == set(functions)
    callers = {(c["function"], c["site"], a) for a, cg in callgraph.items() for c in cg["callers"]}
    for a, cg in callgraph.items():
        for n, e in enumerate(cg["callees"]):
            assert e["order"] == n
            assert e["kind"] in ("call", "import", "indirect", "tailcall", "call_no_function")
            if e["to"] in callgraph:
                assert (a, e["site"], e["to"]) in callers
        sites = [e["site"] for e in cg["callees"]]
        assert sites == sorted(sites)


def test_strings():
    s = {x["address"]: x for x in load("strings.json")["strings"]}
    hit = [x for x in s.values() if "SetActorBoundingRadius" in x["value"]]
    assert hit and any(r["function"] == "00411694" for r in hit[0]["xrefs"])
    assert s["0056B980"]["value"] == "..."


def test_globals():
    g = {x["address"]: x for x in load("globals.json")["globals"]}
    pc = g["006E4DA8"]      # the player profile pointer, typed by apply_typed_symbols.py
    assert pc["type"] and pc["size"] == 4 and len(pc["xrefs"]) > 100
