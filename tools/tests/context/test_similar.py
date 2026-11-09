r"""Similar matched functions (context\similar.py). Synthetic listings and sources; one check against the real
Trilogy data when the workspace has it (listing cache in a temp dir).

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\context\test_similar.py
"""
import json
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "context"))
import similar as sm   # noqa: E402


def test_abstract_keeps_keys():
    assert sm.abstract("mov eax, dword ptr [ecx + 0x40]") == "mov R, dword ptr [R + N]"
    assert sm.abstract("call F:4a3d01") == "call F:4a3d01"
    assert sm.abstract("jne @12") == "jne @"
    assert sm.abstract("call dword ptr [I:KERNEL32.dll!GetTickCount]") == \
        "call dword ptr [I:KERNEL32.dll!GetTickCount]"


def test_rank_prefers_same_shape():
    target = ["push esi", "mov esi, ecx", "mov eax, dword ptr [esi + 0x10]", "test eax, eax", "je @7",
              "call F:401000", "pop esi", "ret"]
    same_shape = ["push edi", "mov edi, ecx", "mov eax, dword ptr [edi + 0x20]", "test eax, eax", "je @7",
                  "call F:401000", "pop edi", "ret"]
    other = ["fld dword ptr [esp + 4]", "fmul dword ptr [esp + 8]", "ret"]
    tiny = ["xor eax, eax", "ret"]
    r = sm.rank(target, {"A": other, "B": same_shape, "C": tiny}, k=2)
    assert [x[0] for x in r] == ["B", "C"] and r[0][1] > 0.8
    assert sm.jaccard(frozenset(), frozenset({"x"})) == 0.0


def test_candidates_exclude_benchmark():
    es = [{"address": "00000001", "state": "tested", "match": {"status": "identical"}},
          {"address": "00000002", "state": "tested", "match": {"status": "diff"}},
          {"address": "00000003", "state": "verified", "match": {"status": "normalised"}},
          {"address": "00000004", "state": "todo"}]
    assert sm.candidates_from_queue(es) == ["00000001", "00000003"]
    assert sm.candidates_from_queue(es, exclude={"00000003"}) == ["00000001"]


def test_rewrite_sources(tmp_path):
    src = tmp_path / "src" / "Sub"
    src.mkdir(parents=True)
    (src / "a.cpp").write_text(
        "#include \"x.h\"\n\n"
        "// FUNCTION: TRILOGY 0x00401000\n"
        "int Foo::Bar_guess(int x)\n{\n    return x + 1;\n}\n\n"
        "// FUNCTION: TRILOGY 0x00401010\n"
        "// ORIGINAL FILE: Foo.cpp\n"
        "void Baz_guess()\n{\n}\n")
    got = sm.rewrite_sources(tmp_path, "trilogy")
    assert set(got) == {"00401000", "00401010"}
    rel, code = got["00401000"]
    assert rel == "src/Sub/a.cpp"
    assert code.startswith("// FUNCTION: TRILOGY 0x00401000") and code.rstrip().endswith("return x + 1;\n}")
    assert "Baz_guess" not in code and got["00401010"][1].count("ORIGINAL FILE") == 1


def test_markdown():
    md = sm.markdown([{"address": "00401000", "name": "Foo", "similarity": 0.5, "source": "src/a.cpp",
                       "code": "int f()\n{\n}\n", "listing": ["ret"] * 100}])
    assert "### 1. `Foo` @ 0x00401000" in md and "(100 instructions)" in md
    assert "None found." in sm.markdown([])


def _real():
    try:
        from eqd_unicorn import Workspace
        ws = Workspace()
        repo = ws.root / "eq-trilogy"
        return ws, repo if (repo / "queue" / "queue.jsonl").exists() and ws.binary("trilogy").exists() else None
    except Exception:   # noqa: BLE001
        return None, None


@pytest.mark.skipif(_real()[1] is None, reason="no trilogy workspace")
def test_real_finder(tmp_path):
    ws, repo = _real()
    entries = [json.loads(l) for l in (repo / "queue" / "queue.jsonl").read_text(encoding="utf-8").splitlines()]
    f = sm.Finder("trilogy", repo, entries, cache_dir=tmp_path, exclude={"00419826"})
    assert len(f.cands) > 100 and "00419826" not in f.cands
    top = f.top("00419867", 5)          # EQ_Character::Skill
    assert len(top) == 5 and all(x["code"].lstrip().startswith("// FUNCTION:") for x in top)
    assert top[0]["similarity"] >= top[-1]["similarity"] > 0
    assert (tmp_path / "listings.json").exists()
    for b in sm.benchmark_set("trilogy"):
        assert b not in f.cands
