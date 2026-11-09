r"""VC6 matching notes in the bundle (context\pack_context.py): VC6_QUIRKS selection and stackcmp frame hints.
Synthetic data; the two bundle tests also need the Trilogy export (skipped without it).

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\context\test_quirks.py
"""
import importlib.util
import json
import sys
from pathlib import Path

import pytest

TOOLS = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS / "context"))
import pack_context as pc   # noqa: E402

DOC = """\
# quirks

### vc6q-alpha
keywords: switch, jump table, case order · repro: `vc6_quirks/alpha.c` · from somewhere

**Claim.** Alpha claim. Second sentence.

**SP4.** details.

**Use.** Alpha use.

### vc6q-beta
keywords: regalloc, stack slot · repro: `vc6_quirks/beta.c`

**Claim.** Beta
claim on two lines.

**Use.** Beta use.

## Checked, doesn't hold on SP4
- not an entry
"""


def ins(*texts, syms=None):
    syms = syms or {}
    return {"instructions": [[f"{0x401000 + i:08X}", "90", t, syms.get(i, t)] for i, t in enumerate(texts)]}


def queue(tmp_path, entries):
    q = pc.Queue(tmp_path)          # no queue folder: empty
    q.entries = {e["address"]: e for e in entries}
    return q


def entry(addr="00401000", status="diff", diff="", **kw):
    e = {"address": addr, "state": "tested", "attempts": [{"model": "m", "diff": diff}]}
    if status:
        e["match"] = {"status": status, "ndiff": 3, "flags": "/O1 /W3 /MT /Gy /GX"}
    e.update(kw)
    return e


def stack_file(tmp_path, functions, **top):
    d = {"schema": 1, "tool": "stackcmp", "client": "trilogy", "generated": "2026-10-04T14:02:22",
         "functions": functions}
    d.update(top)
    p = tmp_path / "stackcmp.json"
    p.write_text(json.dumps(d), encoding="utf-8")
    return p


def test_load_quirks_synthetic(tmp_path):
    p = tmp_path / "Q.md"
    p.write_text(DOC, encoding="utf-8")
    qs = pc.load_quirks(p)
    assert [q["id"] for q in qs] == ["vc6q-alpha", "vc6q-beta"]
    a, b = qs
    assert a["keywords"] == ["switch", "jump table", "case order"] and a["repro"] == "vc6_quirks/alpha.c"
    assert a["claim"] == "Alpha claim. Second sentence." and a["use"] == "Alpha use."
    assert b["claim"] == "Beta claim on two lines."


def test_real_doc_parses():
    qs = pc.load_quirks()
    ids = {q["id"] for q in qs}
    assert len(qs) >= 19 and all(q["claim"] and q["use"] and q["keywords"] for q in qs)
    # every id the selectors name exists in the doc
    named = {i for _, ids_ in pc.TEXT_TRIGGERS for i, _ in ids_} | \
        {i for v in pc.FEATURE_QUIRKS.values() for i, _ in v}
    assert named <= ids, named - ids


def test_listing_features():
    f = pc.listing_features(ins(
        "MOVZX EAX,byte ptr [EAX + 0x4c90a0]", "JMP dword ptr [EAX*0x4 + 0x4c9080]",
        "DEC EAX", "JZ 0x401020", "DEC EAX", "JZ 0x401030",
        "CALL 0x0052189c", "MOV EAX,FS:[0x0]",
        "PUSH 0x8", "CDQ", "POP ECX", "IDIV ECX",
        "AND EAX,0x80000007",
        "TEST byte ptr [0x00591828],0x1", "JNZ 0x401090", "OR byte ptr [0x00591828],0x1", "CALL 0x00523ab6",
        "SETLE AL", "NEG AL", "SBB EAX,EAX", "FLD float ptr [ESI + 0x38]", "FSTP float ptr [ESI + 0x3c]",
        syms={6: "CALL __EH_prolog", 16: "CALL _atexit"}))
    assert {"jump table", "byte index table", "dec chain", "EH frame", "idiv by 2^k", "and 0x8000000k",
            "static guard", "setcc", "neg/sbb", "x87"} <= set(f)
    assert f["x87"] == "2 x87 instructions" and "_atexit" in f["static guard"]
    # negatives: idiv by 10, AND with a plain mask, a lone dec, a struct-field byte load
    g = pc.listing_features(ins("MOV ECX,0xa", "IDIV ECX", "AND EAX,0x80000000", "DEC EAX", "JZ 0x401000",
                                "MOVZX EAX,byte ptr [ECX + 0x40]", "JMP dword ptr [EAX*0x4 + 0x4c9080]"))
    assert set(g) == {"jump table"}
    assert pc.listing_features(None) == {}


def test_select_by_diff_text_and_listing(tmp_path):
    qs = pc.load_quirks()
    sel = pc.select_quirks(qs, "structural: EBP frame (count spilled) + block order of the test", {})
    ids = [q["id"] for q, _ in sel]
    assert ids[:2] == ["vc6q-if-then-first", "vc6q-decl-order-names"] and len(ids) <= pc.MAX_QUIRKS
    assert 'diff summary "block order"' in dict((q["id"], w) for q, w in sel)["vc6q-if-then-first"]
    # listing only: a dec chain picks the switch threshold entry
    sel = pc.select_quirks(qs, "structural; 40 diff tokens", {"dec chain": "3"})
    assert sel[0][0]["id"] == "vc6q-switch-table-threshold" and sel[0][1] == ["original has dec chain"]
    assert pc.select_quirks(qs, "", {}) == []
    assert len(pc.select_quirks(qs, "regalloc jump table block order EH idiv inline", {"x87": 1})) == 4


def test_notes_modes(tmp_path):
    asm = ins("DEC EAX", "JZ 0x401010", "DEC EAX", "JZ 0x401020")
    q = queue(tmp_path, [entry(diff="jump table vs chain"), entry("00402000", status=None),
                         entry("00403000", status="identical")])
    t = pc.matching_notes(q, "00401000", asm)
    assert t.startswith("## VC6 matching notes") and "Match now: **diff**, 3 differing" in t
    assert t.count("  - **vc6q-") <= pc.MAX_QUIRKS and "**vc6q-switch-table-threshold**" in t
    assert "**Use:**" in t
    assert pc.matching_notes(q, "00401000", asm, "off") == ""
    assert pc.matching_notes(q, "00402000", asm) == ""             # todo, default mode: nothing
    assert pc.matching_notes(q, "00403000", asm) == ""             # identical: nothing
    t = pc.matching_notes(q, "00402000", asm, "all")               # todo, "all": listing features
    assert "vc6q-switch-table-threshold" in t and "Match now" not in t
    assert pc.matching_notes(q, "00409999", asm, "all").count("vc6q-") >= 1    # not in the queue at all


def test_notes_artefact_and_best(tmp_path):
    q = queue(tmp_path, [entry(diff="bytes identical; status 'diff' only because of fs:[0] vs __except_list",
                               match_best={"status": "normalised", "ndiff": 0})])
    t = pc.matching_notes(q, "00401000", ins("MOV EAX,FS:[0x0]"))
    assert "compare artefact" in t and "vc6q-" not in t and "best: normalised, 0 differing" in t


def test_stack_hints(tmp_path):
    p = stack_file(tmp_path, [
        {"va": "0x00401000", "status": "ok", "stack": "differs", "frame": {"orig": 32, "rewrite": 36},
         "complete": False, "hints": [f"hint {i}" for i in range(5)]},
        {"va": "0x402000", "status": "ok", "stack": "same", "frame": {"orig": None, "rewrite": None}, "hints": []},
        {"va": "0x00403000", "status": "no_match", "hints": ["never shown"]},
    ])
    h = pc.StackHints(p, "trilogy")
    assert not h.error and set(h.by_va) == {"00401000", "00402000", "00403000"}
    line = pc.stack_line(h.get("00401000"), h.generated)
    assert line.startswith("stackcmp (2026-10-04): stack **differs**")
    assert "original 32, rewrite 36" in line and "partial" in line and "hint 2" in line
    assert "hint 3" not in line and "(+2 more)" in line
    assert "registers or instruction choice" in pc.stack_line(h.get("00402000"))
    assert pc.stack_line(h.get("00403000")) is None and pc.stack_line(None) is None
    # in the notes: shown even without a quirk, and for a todo function
    q = queue(tmp_path, [entry("00402000", status=None)])
    t = pc.matching_notes(q, "00402000", ins("NOP"), "nearmiss", h)
    assert "stack **same**" in t and "vc6q-" not in t


def test_stack_hints_rejects_and_default(tmp_path, capsys):
    assert pc.StackHints(tmp_path / "missing.json").error
    assert pc.StackHints(stack_file(tmp_path, [], schema=2)).error
    assert pc.StackHints(stack_file(tmp_path, [], client="other"), "trilogy").error

    class WS:
        build = tmp_path
    assert pc.load_stack_hints(None, WS, "trilogy") is None            # no default file: silently none
    (tmp_path / "match").mkdir()
    p = stack_file(tmp_path, [{"va": "0x401000", "status": "ok", "stack": "same"}])
    p.replace(tmp_path / "match" / "stackcmp.json")
    assert pc.load_stack_hints(None, WS, "trilogy").get("00401000")     # the documented default path
    assert pc.load_stack_hints("off", WS, "trilogy") is None
    assert pc.load_stack_hints(str(tmp_path / "nope.json"), WS, "trilogy") is None
    assert "no stack hints" in capsys.readouterr().err


# ---- the bundle and run_one's bundle (need the export)

try:
    EX = pc.Export("trilogy")
except SystemExit:
    EX = None
needs_export = pytest.mark.skipif(EX is None, reason="no trilogy export")
ADDR = "004C8FCE"           # GetLanguageName: has a jump table


@needs_export
def test_bundle_has_notes(tmp_path):
    q = queue(tmp_path, [entry(ADDR, diff="block order of the default case")])
    h = pc.StackHints(stack_file(tmp_path, [{"va": "0x" + ADDR, "status": "ok", "stack": "same",
                                             "frame": {"orig": 8, "rewrite": 8}, "hints": []}]))
    t = pc.build_bundle(EX, pc.Headers(tmp_path, "trilogy"), q, ADDR, stack_hints=h)
    notes = t[t.index("## VC6 matching notes"):t.index("## Prior attempts")]
    assert "vc6q-if-then-first" in notes and "original has jump table" in notes
    assert "`sub esp, N`: original 8, rewrite 8" in notes
    t = pc.build_bundle(EX, pc.Headers(tmp_path, "trilogy"), q, ADDR, quirks="off")
    assert "## VC6 matching notes" not in t


@needs_export
def test_run_one_bundle_gets_notes(tmp_path):
    if not (TOOLS / "queue" / "run_one.py").exists():
        pytest.skip("queue/run_one.py is not part of this checkout")
    spec = importlib.util.spec_from_file_location("run_one_t", TOOLS / "queue" / "run_one.py")
    ro = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(ro)
    repo = tmp_path / "repo"
    (repo / "queue").mkdir(parents=True)
    (repo / "queue" / "queue.jsonl").write_text(json.dumps(entry(ADDR, diff="regalloc")) + "\n", encoding="utf-8")
    sh = stack_file(tmp_path, [{"va": "0x" + ADDR, "status": "ok", "stack": "differs", "hints": ["frame X"]}])
    t = ro.build_bundle("trilogy", ADDR, repo, repo, str(tmp_path / "cache"), False, "nearmiss", str(sh))
    assert "vc6q-decl-order-names" in t and "stack **differs**" in t and "frame X" in t
    t = ro.build_bundle("trilogy", ADDR, repo, repo, str(tmp_path / "cache"), True, "nearmiss", str(sh))
    assert "## VC6 matching notes" not in t       # benchmark: no history, no stack hints
