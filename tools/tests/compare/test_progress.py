"""compare/progress.py on a synthetic repo (no game files): counts per image and the table."""
import json
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS / "compare"))
import progress  # noqa: E402


def write(p: Path, text: str):
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text, encoding="utf-8")


def test_counts_per_image(tmp_path):
    repo = tmp_path / "eq-trilogy"
    write(repo / "src/EverQuest/a.cpp", "// FUNCTION: TRILOGY 0x00401000\nint a() { return 0; }\n"
                                        "  // FUNCTION: TRILOGY 0x00401010\nint b() { return 1; }\n")
    write(repo / "src/eqmain/b.cpp", "// FUNCTION: EQMAIN 0x10001000\nint c() { return 2; }\n")
    write(repo / "tests/unicorn/EverQuest/a/a.json", json.dumps({"function": "a", "original": "0x00401000"}))
    write(repo / "tests/unicorn/eqmain/b/c.json", json.dumps({"function": "c", "original": "0x10001000"}))
    write(repo / "tests/unicorn/EverQuest/a/negative/x.json", json.dumps({"original": "0x00409999"}))
    write(repo / "tests/unicorn/crt_defaults.json", json.dumps({"original": "0x00408888"}))
    cmp = tmp_path / "trilogy.json"
    cmp.write_text(json.dumps({"counts": {"identical": 1, "differs": 1},
                               "results": [{"va": "0x00401000", "bytes_identical": True, "ratio": 1.0, "orig_count": 10},
                                           {"va": "0x00401010", "bytes_identical": False, "ratio": 0.5,
                                            "orig_count": 30}]}), encoding="utf-8")
    out = tmp_path / "PROGRESS.md"
    js = tmp_path / "progress.json"
    assert progress.main(["--repo", str(repo), "--compare", f"trilogy={cmp}", "--compare", "eqmain=",
                          "--out", str(out), "--json", str(js)]) == 0
    rows = {r["client"]: r for r in json.loads(js.read_text(encoding="utf-8"))["rows"]}
    assert rows["trilogy"] == {"client": "trilogy", "binary": "eqgame.exe", "rewritten": 2, "tested": 1,
                               "compared": 2, "identical": 1, "instructions": 40, "identical_instructions": 10,
                               "weighted_ratio": 25.0}
    assert rows["eqmain"]["rewritten"] == 1 and rows["eqmain"]["tested"] == 1 and "compared" not in rows["eqmain"]
    text = out.read_text(encoding="utf-8")
    assert "| `eqgame.exe` | 2 | 1 | 1 of 2 (50.0%) | 25.0% | 62.5% |" in text
    assert "| `eqmain.dll` | 1 | 1 | - | - | - |" in text
