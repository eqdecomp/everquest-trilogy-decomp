"""permute/: unit extraction (tu.py) and the search driver with a fake scorer (no game files); the type probe
against VC6 when the workspace has it."""
import concurrent.futures as cf
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "permute"))
sys.path.insert(0, str(ROOT / "unicorn"))

import permute as P  # noqa: E402
import tu  # noqa: E402

SRC = """// x.cpp
// CFLAGS: /O2
#include "a.h"

static int helper(int v)
{
    return v + 1;
}

// FUNCTION: TRILOGY 0x00401000
int Foo::Bar(int a)
{
    return helper(a) * 2;
}

// FUNCTION: TRILOGY 0x00401100
Foo::~Foo()
{
    if (p)
        p->q = 0;
}

struct S { int x; };
int table[3] = { 1, 2, 3 };

// FUNCTION: TRILOGY 0x00401200
void Foo::Unrelated(void)
{
    x = 1;
}
"""


def test_find_functions_names():
    names = [f.name for f in tu.find_functions(SRC)]
    assert names == ["helper", "Foo::Bar", "Foo::~Foo", "Foo::Unrelated"]


def test_extract_keeps_what_the_target_needs():
    f = tu.find_target(SRC, 10)
    assert f.name == "Foo::Bar"
    u = tu.extract(SRC, f)
    text = u.text()
    assert "static int helper" in text and "Foo::Bar" in text
    assert "Unrelated" not in text and "~Foo" not in text
    assert "// CFLAGS: /O2" in text and "int table[3]" in text and "struct S" in text
    assert "0x00401100" not in text                      # dropped functions lose their annotations
    assert u.body == "{\n    return helper(a) * 2;\n}"
    assert tu.whole(SRC, f).text() == SRC


class SyncPool:
    """concurrent.futures stand-in that runs each call at submit."""

    def submit(self, fn, *args, **kw):
        f = cf.Future()
        try:
            f.set_result(fn(*args, **kw))
        except Exception as e:  # noqa: BLE001
            f.set_exception(e)
        return f


def test_search_and_minimise_with_a_fake_scorer(monkeypatch):
    """The score is 0 when `b = 2;` comes before `a = 1;`: the search must find it from swaps, and the
    minimiser must drop unrelated changes."""
    body = "{\n    int x;\n    a = 1;\n    b = 2;\n    c = x + 3;\n    d = 4;\n}"

    class Fake:
        text = ""

    def fake_score(text, want_diff=False):
        i = text.index("{")
        bd = text[i:]
        good = bd.index("b = 2;") < bd.index("a = 1;")
        return {"score": 0 if good else 20, "verdict": "x", "ndiff": 0, "structural": False, "bytes": good,
                "first_diff": "", "error": "", "listing": []}

    monkeypatch.setattr(P, "worker_score", fake_score)
    unit = tu.Unit("void f(void)\n", body, "\n", False)
    t = P.Target(0x401000, "f", "src/x.c", unit.text(), None, unit, True)

    class FD:
        params = []
    t.fdef = FD()
    # a pool that scores at submit: the search sees results in submission order, so a seed gives one run
    # (with threads the completion order, and so the path, varied). Some paths end with a mutation the
    # minimiser can't revert because a later one rewrote its lines (seeds 1 and 6 here: a scope_block the
    # swap happened inside); revert_step is all-or-nothing by design.
    pool = SyncPool()
    base, best, stats = P.search(t, pool, budget_s=20, max_iter=2000, jobs=2, seed=2, types={}, log=lambda *a: None,
                                 stop_at=0)
    assert base.score == 20 and best.score == 0
    best = P.minimise(t, pool, base, best, log=lambda *a: None)
    assert best.body.index("b = 2;") < best.body.index("a = 1;")
    assert "c = x + 3;\n    d = 4;" in best.body or best.body.count("\n") == body.count("\n")


def _vc6():
    try:
        ws = P.Workspace(ROOT)
    except FileNotFoundError:
        return None
    cl = ws.vc6() / "Bin" / "CL.EXE"
    return ws.vc6() if cl.is_file() else None


@pytest.mark.skipif(_vc6() is None, reason="needs the workspace's VC6")
@pytest.mark.parametrize("ext", [".cpp", ".c"])
def test_type_probe_with_vc6(tmp_path, ext):
    from eqd_unicorn.vc6 import vc6_env
    vc6 = _vc6()
    head = ("struct Node { struct Node *next; unsigned short v; float f; };\n"
            "int use(int);\nint f(struct Node *p)\n")
    bd = "{\n    use(p->next->v);\n    return (int)p->f;\n}"
    unit = tu.Unit(head, bd, "\n", False)

    class FD:
        params = []
    t = P.Target(0, "f", "x" + ext, unit.text(), FD(), unit, ext == ".cpp")

    def run(text):
        src = tmp_path / ("p" + ext)
        src.write_text(text)
        p = subprocess.run([str(vc6 / "Bin" / "CL.EXE"), "/nologo", "/c", "/W3", f"/Fo{tmp_path}\\", str(src)],
                           env=vc6_env(vc6), capture_output=True, text=True, cwd=tmp_path)
        return p.stdout + p.stderr
    types = P.probe_types(t, run)
    assert types.get("p -> next") in ("struct Node *", "Node *")
    assert types.get("p -> next -> v") == "unsigned short"
    if ext == ".cpp":
        assert types.get("p -> f") == "float"


def test_revert_step_is_all_or_nothing():
    before = "{\n    a = 1;\n    b = 2;\n    c = 3;\n    d = 4;\n    e = 5;\n    f = 6;\n}"
    after = "{\n    b = 2;\n    a = 1;\n    c = 3;\n    d = 4;\n    e = 5;\n    f = 6;\n}"
    later = after.replace("f = 6;", "++f;")
    assert P.revert_step(later, before, after) == before.replace("f = 6;", "++f;")
    touched = after.replace("a = 1;", "a = 7;")
    assert P.revert_step(touched, before, after) is None


def _res(verdict="differs", ratio=0.9, ndiff=5, structural=False, bytes_identical=False):
    return P.compare.Result(0x401000, "f", "x.c", verdict, ratio=ratio, ndiff=ndiff, structural=structural,
                            bytes_identical=bytes_identical)


def test_score_orders_like_the_acceptance_check():
    """status, then compare ratio, then the differing instruction count; never a lower ratio for a lower
    score. The old count-based score preferred the same-count candidate (60 < 1100) of 0x10010AB0."""
    ident = P.score_of(_res("identical", 1.0, 0, bytes_identical=True))
    norm = P.score_of(_res("identical_normalised", 1.0, 0))
    one_longer = P.score_of(_res(ratio=0.9808, ndiff=10, structural=True))    # unmodified 0x10010AB0
    same_count = P.score_of(_res(ratio=0.9057, ndiff=6))                      # last round's winner
    assert (ident, norm) == (0, 1)
    assert norm < one_longer < same_count
    assert P.score_of(_res(ratio=0.95, ndiff=3)) < P.score_of(_res(ratio=0.95, ndiff=4))
    assert P.score_of(_res(ratio=0.9501, ndiff=900)) < P.score_of(_res(ratio=0.95, ndiff=0))
    assert P.score_of(_res(ratio=1.0, ndiff=2)) > norm                         # labels only differ
    assert P.score_of(_res("error")) is None and P.score_of(None) is None
    rs = [_res(ratio=r, ndiff=n, structural=s) for r in (0.8, 0.9, 0.9909, 0.9991) for n in (0, 7, 44)
          for s in (False, True)]
    for x in rs:
        for y in rs:
            if P.score_of(x) < P.score_of(y):
                assert x.ratio >= y.ratio


def test_not_worse():
    d = lambda st, r: {"status": st, "ratio": r}                              # noqa: E731
    assert P.not_worse(d("diff", 0.9), d("structural", 0.9))                  # diff/structural: by ratio only
    assert not P.not_worse(d("diff", 0.9), d("diff", 0.8999))
    assert not P.not_worse(d("identical", 1.0), d("normalised", 1.0))
    assert not P.not_worse(d("normalised", 1.0), d("diff", 1.0))
    assert P.not_worse(d("structural", 0.5), d("identical", 1.0))
    assert not P.not_worse(d("diff", 0.9), d("error", 0.0))


def test_whole_source_judge():
    def e(st, r, n=3):
        sc = P.score_of(_res({"identical": "identical", "normalised": "identical_normalised"}.get(st, "differs"),
                             r, n, bytes_identical=st == "identical"))
        return {"status": st, "ratio": r, "ndiff": n, "score": sc, "name": "g"}
    T, O = 0x401000, 0x401100
    base = {T: e("structural", 0.9808, 10), O: e("diff", 0.97)}
    ok, why, t1 = P.judge(base, {T: e("diff", 0.99, 2), O: e("diff", 0.97)}, T)
    assert ok and why == "ok" and t1["ratio"] == 0.99
    ok, why, _ = P.judge(base, {T: e("diff", 0.9057, 6), O: e("diff", 0.97)}, T)    # unit better, source not
    assert not ok and "target not better" in why
    ok, why, _ = P.judge(base, {T: e("identical", 1.0, 0), O: e("diff", 0.969)}, T)   # another function loses
    assert not ok and "0x00401100" in why
    ok, why, _ = P.judge(base, {T: e("identical", 1.0, 0)}, T)
    assert not ok and "missing" in why
