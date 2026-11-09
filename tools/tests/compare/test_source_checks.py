"""source_checks.py on a synthetic repo: status markers, unmarked oddities, the ratchet floor (no VC6, no game files).

Each check is shown failing on a planted bad case first (negative controls), then passing on the clean one.
"""
import json
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "compare"))

import source_checks as sc  # noqa: E402

CLEAN = """\
// X.cpp - test source
#include "x.h"

extern int g_count;

// FUNCTION: TRILOGY 0x00401000
// ORIGINAL FILE: X.cpp
int Foo(char* p)
{
    int v;

    v = *(int*)(p + 0x10);
    return (short)v + *(int*)0x006E4DE0;
}

// FUNCTION: TRILOGY 0x00401100
int Bar(int a, int b)
{
    if (a) return (a * b) - 1;
    return Foo((char*)b);
}
"""


def repo_with(tmp_path, text, name="X.cpp"):
    (tmp_path / "src").mkdir(exist_ok=True)
    (tmp_path / "src" / name).write_text(text)
    return tmp_path


def check(repo, floor=None, queue=None):
    return sc.check_repo(repo, floor, queue)


def test_counts_casts_raw_addresses_externs(tmp_path):
    rep = check(repo_with(tmp_path, CLEAN))
    assert rep.units["00401000"] == {"cast": 3, "raw_addr": 2}           # (int*)(p+0x10), (short), (int*)0x6E..
    assert rep.units["00401100"] == {"cast": 1}                          # `(a * b) - 1` and `if (a) return` aren't
    assert rep.units["src/X.cpp"] == {"extern": 1}
    assert rep.violations == []


@pytest.mark.parametrize("declaration, expression, expected", [
    ("typedef int {name};", "({name})value", 1),
    ("typedef int {name};", "({name})(int)value", 2),
    ("typedef int (__stdcall *{name})(int arg);", "({name})(value)(arg)", 1),
])
def test_typedef_cast_counts_ignore_spelling(tmp_path, declaration, expression, expected):
    counts = []
    for name in ("gfxVtableSlot50_t_guess", "GfxVtableSlot50_t_guess"):
        src = declaration.format(name=name) + "\n// FUNCTION: TRILOGY 0x10046165\n" \
              "int Invoke(int value, int arg)\n{\n    return " + expression.format(name=name) + ";\n}\n"
        counts.append(check(repo_with(tmp_path, src)).units["10046165"].get("cast", 0))
    assert counts == [expected, expected]


@pytest.mark.parametrize("declaration", ["int (*x)(int);", "int x(int);"])
def test_parenthesized_callable_is_not_a_cast(tmp_path, declaration):
    src = "typedef int other_type;\n" + declaration + "\n// FUNCTION: TRILOGY 0x00401000\n" \
          "int Invoke(int y)\n{\n    return (x)(y);\n}\n"
    rep = check(repo_with(tmp_path, src))
    assert rep.units["00401000"].get("cast", 0) == 0


@pytest.mark.parametrize("body, msg", [
    ("    do {} while (0);\n", "empty do {} while (0) barrier"),
    ("    int unused;\n", "local `unused` is never used"),
    ("    if (a) goto out;\nout:\n", "goto out"),
    ("    __asm { nop }\n", "__asm block"),
])
def test_unmarked_oddity_is_a_violation_and_a_marker_covers_it(tmp_path, body, msg):
    src = "// FUNCTION: TRILOGY 0x00401000\nint Foo(int a)\n{\n" + body + "    return a;\n}\n"
    rep = check(repo_with(tmp_path, src))
    assert [v.msg.split(":")[0] for v in rep.violations] == [msg]          # planted: flagged
    marked = src.replace("{\n", "{\n    /* fake match: forces the frame of the original */\n", 1)
    assert check(repo_with(tmp_path, marked)).violations == []           # comment directly above: covered
    lead = src.replace("int Foo", "// fake match: keeps EDI live across the call\nint Foo")
    assert check(repo_with(tmp_path, lead)).violations == []             # in front of the function: covers it


def test_register_is_banned_even_when_marked(tmp_path):
    src = "// FUNCTION: TRILOGY 0x00401000\n// fake match: pins r to ESI\nint Foo(int a)\n{\n" \
          "    /* fake match: pins r to ESI */\n    register int r = a;\n    return r;\n}\n"
    assert [v.msg for v in check(repo_with(tmp_path, src)).violations] == \
        ["register keyword (banned): not allowed, marked or not (RULES.md Fake matches) (new code)"]


def test_as_original_covers_goto_but_not_a_barrier(tmp_path):
    g = "// FUNCTION: TRILOGY 0x00401000\nint Foo(int a)\n{\n    // as original: shared tail\n" \
        "    if (a) goto out;\nout:\n    return a;\n}\n"
    assert check(repo_with(tmp_path, g)).violations == []
    b = g.replace("if (a) goto out;\nout:", "do {} while (0);")
    assert len(check(repo_with(tmp_path, b)).violations) == 1


def test_a_comment_separated_by_code_does_not_cover(tmp_path):
    src = "// FUNCTION: TRILOGY 0x00401000\nint Foo(int a)\n{\n    /* fake match: keeps a in ESI */\n    a++;\n" \
          "    do {} while (0);\n    return a;\n}\n"
    assert len(check(repo_with(tmp_path, src)).violations) == 1


@pytest.mark.parametrize("comment, msg", [
    ("// @early-stop: regalloc ESI/EDI swap, 2 instructions", None),
    ("// @identity-TODO: a fixture case with count == 0 would prove it", None),
    ("// @dead-code: no xrefs in the export", None),
    ("// @equivalent: block order only; both paths store the same fields", None),
    ("// @earlystop: x", "unknown marker @earlystop"),
    ("// @identity-todo: something", "unknown marker @identity-todo (write @identity-TODO)"),
    ("// @early-stop", "@early-stop needs `: <reason>`"),
    ("// @dead-code:", "@dead-code needs `: <reason>`"),
    ("/* fake match: */", "`fake match:` without a reason"),
])
def test_status_markers(tmp_path, comment, msg):
    src = f"// FUNCTION: TRILOGY 0x00401000\n{comment}\nint Foo(int a)\n{{\n    return a;\n}}\n"
    v = check(repo_with(tmp_path, src)).marker_errors
    assert len(v) == (msg is not None) and all(x.msg.startswith(msg) for x in v)


def test_marker_must_belong_to_an_annotated_function_once(tmp_path):
    src = "// @dead-code: nobody calls it\nstatic int helper(int a)\n{\n    return a;\n}\n" \
          "// FUNCTION: TRILOGY 0x00401000\n// @dead-code: no xrefs\nint Foo(int a)\n{\n" \
          "    // @dead-code: again\n    return a;\n}\n"
    msgs = [x.msg for x in check(repo_with(tmp_path, src)).marker_errors]
    assert msgs == ["@dead-code is not attached to a // FUNCTION: annotated function",
                    "@dead-code twice for one function"]


def test_stale_match_marker_with_queue(tmp_path):
    src = "// FUNCTION: TRILOGY 0x00401000\n// @early-stop: one regalloc swap\nint Foo(int a)\n{\n    return a;\n}\n"
    repo = repo_with(tmp_path, src)
    q = tmp_path / "queue.jsonl"
    q.write_text(json.dumps({"address": "00401000", "match": {"status": "diff"}}) + "\n")
    assert check(repo, queue=q).violations == []
    q.write_text(json.dumps({"address": "00401000", "match": {"status": "identical"}}) + "\n")
    assert "stale" in check(repo, queue=q).violations[0].msg


def test_ratchet_floor_only_goes_down(tmp_path, capsys):
    repo = repo_with(tmp_path, CLEAN)
    floor = tmp_path / sc.FLOOR_NAME
    args = ["--repo", str(repo), "--floor", str(floor)]
    assert sc.main(args + ["--raise-floor"]) == 0                        # creates the floor
    d = json.loads(floor.read_text())
    assert d["units"]["00401000"] == {"cast": 3, "raw_addr": 2} and d["units"]["00401100"] == {"cast": 1}
    assert sc.main(args) == 0
    # planted regression: an existing function gains a cast -> violation, and --update refuses to record it
    repo_with(tmp_path, CLEAN.replace("return Foo((char*)b);", "return Foo((char*)(long)b);"))
    assert sc.main(args) == 1
    assert "00401100] cast 1 -> 2 (above its floor)" in capsys.readouterr().out
    assert sc.main(args + ["--update"]) == 1
    assert json.loads(floor.read_text()) == d                            # unchanged
    # an existing clean unit (the file scope of a new extern) counts too
    repo_with(tmp_path, CLEAN.replace("extern int g_count;", "extern int g_count;\nextern int g_more;"))
    assert sc.main(args) == 1
    # lower: recorded by --update
    repo_with(tmp_path, CLEAN.replace("(short)v", "v"))
    assert sc.main(args) == 0
    assert sc.main(args + ["--update"]) == 0
    assert json.loads(floor.read_text())["units"]["00401000"] == {"cast": 2, "raw_addr": 2}
    # raising is the explicit step
    repo_with(tmp_path, CLEAN)
    assert sc.main(args) == 1
    assert sc.main(args + ["--raise-floor"]) == 0 and sc.main(args) == 0


def test_new_function_counts_are_free_but_its_oddities_are_not(tmp_path):
    repo = repo_with(tmp_path, CLEAN)
    floor = tmp_path / sc.FLOOR_NAME
    sc.main(["--repo", str(repo), "--floor", str(floor), "--raise-floor"])
    new = "\n// FUNCTION: TRILOGY 0x00401200\nint Baz(char* p)\n{\n    return *(int*)(p + 4);\n}\n"
    repo_with(tmp_path, CLEAN + new)
    rep = check(repo, floor)
    assert rep.violations == [] and rep.new_units == ["00401200"]
    repo_with(tmp_path, CLEAN + new.replace("{\n", "{\n    register int r;\n"))
    rep = check(repo, floor)
    assert {v.msg.split(":")[0] for v in rep.violations} == {"register keyword (banned)", "local `r` is never used"}


def test_scoped_guard_object_is_not_an_unused_local(tmp_path):
    guard = "struct Lockit {\n    Lockit();\n    ~Lockit();\n};\n\n"
    body = "// FUNCTION: TRILOGY 0x00401000\nint Foo(int a)\n{\n    {\n        Lockit lk;\n\n        a = a + 1;\n    }\n" \
           "    return a;\n}\n"
    assert check(repo_with(tmp_path, guard + body)).violations == []          # constructor/destructor do the work
    ptr = body.replace("Lockit lk;", "Lockit *lk;")
    assert [v.msg.split(":")[0] for v in check(repo_with(tmp_path, guard + ptr)).violations] == \
        ["local `lk` is never used"]                                         # a pointer to one is still unused
    plain = body.replace("Lockit lk;", "int lk;")
    assert [v.msg.split(":")[0] for v in check(repo_with(tmp_path, guard + plain)).violations] == \
        ["local `lk` is never used"]


