"""gate.py on the synthetic git repo, with VC6 (skips without it): a no-op rename round passes, each kind of real
change fails, `.check/` litter and VA letter case don't count."""
import json
import subprocess

import pytest

import gate
import rename as rn


def _vc6_or_skip():
    try:
        ws = gate.Workspace()
        cl = ws.vc6("trilogy") / "Bin" / "CL.EXE"
    except FileNotFoundError as e:
        pytest.skip(str(e))
    if not cl.is_file():
        pytest.skip(f"VC6 not found at {cl}")


def run(repo, tmp_path, files=None, **kw):
    return gate.run_gate(repo, "HEAD", files, cache=tmp_path / "cache", jobs=2, **kw)


def edit(repo, rel, old, new):
    p = repo / rel
    t = p.read_text(encoding="utf-8")
    assert old in t
    p.write_text(t.replace(old, new), encoding="utf-8")


def reset(repo):
    subprocess.run(["git", "checkout", "-q", "--", "."], cwd=repo, check=True)
    subprocess.run(["git", "clean", "-fdq"], cwd=repo, check=True)


def test_changed_files_skip_check_litter(git_repo):
    (git_repo / ".check").mkdir()
    (git_repo / ".check" / "out.txt").write_text("x", encoding="utf-8")
    (git_repo / "src/EverQuest/.check").mkdir()
    (git_repo / "src/EverQuest/.check/a.json").write_text("{}", encoding="utf-8")
    edit(git_repo, "src/EverQuest/widget.cpp", "// Sums", "// Adds")
    sha = gate.git(git_repo, "rev-parse", "HEAD").strip()
    assert gate.changed_files(git_repo, sha) == ["src/EverQuest/widget.cpp"]


def test_rename_round_passes_and_real_changes_fail(git_repo, tmp_path):
    _vc6_or_skip()
    repo = git_repo
    r = run(repo, tmp_path, ["src/EverQuest/widget.cpp"])            # nothing changed at all
    assert r.ok, r.report()
    assert r.tus[0].functions >= 2

    # renames of every kind: function (by VA macro and by name through the header), label, local, global
    for old, new, kw in (("FUN_00401100", "Accumulate_guess", {}), ("0x00401000", "Sum_guess", {}),
                         ("LAB_00401130", "skip", {"in_va": "0x00401100"}),
                         ("local_8", "total", {"in_va": "0x00401100"}),
                         ("g_scale_factor", "g_scale", {})):
        rn.write_plan(repo, rn.plan_rename(repo, "trilogy", old, new, **kw))
    r = run(repo, tmp_path)
    assert r.ok, r.report()
    assert {t.rel for t in r.tus} == {"src/EverQuest/widget.cpp", "src/EverQuest/other.cpp"}
    reset(repo)

    # a constant: that function compiles differently, by VA
    edit(repo, "src/EverQuest/widget.cpp", "ALLOC_MEMORY(4)", "ALLOC_MEMORY(8)")
    r = run(repo, tmp_path)
    assert not r.ok
    assert [c[0] for c in r.tus[0].changed] == ["00401100"]
    reset(repo)

    # letter case of a VA: same code (one header for both builds)
    edit(repo, "src/EverQuest/widget.cpp", "0x006E0020", "0x006e0020")
    r = run(repo, tmp_path)
    assert r.ok, r.report()
    reset(repo)

    # a macro defined twice with different bodies: C4005 is an error here
    edit(repo, "src/EverQuest/widget.cpp", "#define BUMP()", "#define g_scale 2\n#define BUMP()")
    r = run(repo, tmp_path)
    assert not r.ok and any("C4005" in e for e in r.tus[0].errors)
    reset(repo)

    # same code, one more cast: source_checks count
    edit(repo, "src/EverQuest/widget.cpp", "return local_8 + count_ref;", "return local_8 + (int)count_ref;")
    r = run(repo, tmp_path)
    assert not r.ok and r.tus[0].checks and "cast" in r.tus[0].checks[0], r.report()
    reset(repo)

    # a fixture naming a function its source doesn't define
    p = repo / "tests/unicorn/EverQuest/widget/FUN_00401100.json"
    d = json.loads(p.read_text(encoding="utf-8"))
    d["function"] = "Accumulate_guess"
    p.write_text(json.dumps(d, indent=2), encoding="utf-8")
    r = run(repo, tmp_path)
    assert not r.ok and r.lint and "FUN_00401100" in r.lint[0]
    reset(repo)

    # a build error after the edit
    edit(repo, "src/EverQuest/widget.cpp", "return local_8 + count_ref;", "return local_8 + nothing;")
    r = run(repo, tmp_path)
    assert not r.ok and any("build fails" in e for e in r.tus[0].errors)


def test_header_change_pulls_in_dependents(git_repo, tmp_path):
    _vc6_or_skip()
    edit(git_repo, "include/trilogy/widget.h", "int count;", "int count;   // how many")
    r = run(git_repo, tmp_path)
    assert r.ok, r.report()
    assert {t.rel for t in r.tus} == {"src/EverQuest/widget.cpp", "src/EverQuest/other.cpp"}
    edit(git_repo, "include/trilogy/widget.h", "int count;", "int pad; int count;")      # moves a field
    r = run(git_repo, tmp_path)
    assert not r.ok


def test_journal_renames(tmp_path):
    j = tmp_path / "j.jsonl"
    j.write_text('{"client": "trilogy", "va": "00401000", "kind": "fn", "old": "A::Old_guess", "new": "A::New_guess"}\n'
                 '{"client": "trilogy", "va": "00401000", "kind": "local", "old": "x", "new": "y"}\n', encoding="utf-8")
    assert gate.load_journal(j) == {"Old_guess": "New_guess"}
    assert gate._ids_rename("?Old_guess@A@@QAEHXZ", {"Old_guess": "New_guess"}) == "?New_guess@A@@QAEHXZ"
    assert gate._ids_rename("_Old_guess", {"Old_guess": "New_guess"}) == "_New_guess"


MARKED = """\
// FUNCTION: TRILOGY 0x00401100
// fake match: the original keeps this local
// and a second line of the note
int Fn(int a)
{
    int unused;
    /* fake match */
    do {} while (0);
    // fake match: inner note
    return a;
}
"""


def _write(p, text):
    p.write_text(text, encoding="utf-8")
    return p


def test_dropped_markers_fail(tmp_path):
    before = _write(tmp_path / "b.cpp", MARKED)

    def counts(after_text):
        after = _write(tmp_path / "a.cpp", after_text)
        return gate.check_counts(before, after, "a.cpp", "trilogy", {})

    assert counts(MARKED) == []
    # rewording a note is fine; losing the /* fake match */ block, the note lines or the inner note is not
    assert counts(MARKED.replace("the original keeps this local", "reworded")) == []
    out = counts(MARKED.replace("    /* fake match */\n", ""))
    assert any("marker fake match dropped" in c for c in out), out
    out = counts(MARKED.replace("// and a second line of the note\n", ""))
    assert any("fake match lines dropped" in c for c in out), out
    out = counts(MARKED.replace("    // fake match: inner note\n", ""))
    assert any("marker fake match dropped" in c for c in out), out
    # an @marker and an `as original` note count too
    withat = MARKED.replace("int Fn", "// @early-stop: bounded diff\n// as original: shape\nint Fn")
    before2 = _write(tmp_path / "b2.cpp", withat)
    after = _write(tmp_path / "a2.cpp", MARKED)
    out = gate.check_counts(before2, after, "a2.cpp", "trilogy", {})
    assert any("@early-stop dropped" in c for c in out) and any("as original dropped" in c for c in out), out


def test_header_sensitive_tu_passes_with_own_headers(git_repo, tmp_path, monkeypatch):
    """A difference that shows only under the shared header is rechecked under each tree's own header."""
    _vc6_or_skip()
    # a new VA token on the after side makes the own headers differ from the union one
    edit(git_repo, "src/EverQuest/widget.cpp", "#define g_scale EQ_GLOBAL(int, 0x006E0020)",
         "#define g_scale EQ_GLOBAL(int, 0x006E0020)\n#define g_unused_new EQ_GLOBAL(int, 0x006E0030)")
    real = gate.diff_objects
    calls = []

    def fake(ob, oa, bs, as_, client, renames, **kw):
        calls.append(str(ob))
        changed, n = real(ob, oa, bs, as_, client, renames, **kw)
        if "match_own" not in str(ob) and "match_own" not in str(oa):
            changed = changed + [("00401100", "FUN_00401100", "compiled code differs")]   # union-header artefact
        return changed, n
    monkeypatch.setattr(gate, "diff_objects", fake)
    r = run(git_repo, tmp_path, ["src/EverQuest/widget.cpp"])
    assert r.ok, r.report()
    assert any("header-sensitive, own-header PASS" in n for n in r.tus[0].notes), r.report()
    assert "header-sensitive, own-header PASS" in r.report()

    # when the difference is real (also under the own headers) it still fails
    def fake_real(ob, oa, bs, as_, client, renames, **kw):
        changed, n = real(ob, oa, bs, as_, client, renames, **kw)
        return changed + [("00401100", "FUN_00401100", "compiled code differs")], n
    monkeypatch.setattr(gate, "diff_objects", fake_real)
    r = run(git_repo, tmp_path, ["src/EverQuest/widget.cpp"])
    assert not r.ok and r.tus[0].changed


def test_dropped_comment_keywords_fail(tmp_path):
    src = ("// FUNCTION: TRILOGY 0x00401100\n// zlib 1.1.x's inflate_blocks (infblock.c)\n// __thiscall\n"
           "// Returns 1 when done; callers never read EAX\nint Fn(int a)\n{\n    return a;\n}\n")
    before = _write(tmp_path / "b.cpp", src)

    def out(after_text):
        return gate.check_counts(before, _write(tmp_path / "a.cpp", after_text), "a.cpp", "trilogy", {})
    assert out(src) == []
    assert out(src.replace("int Fn", "// A new summary line\nint Fn")) == []          # adding is fine
    for gone, word in (("// zlib 1.1.x's inflate_blocks (infblock.c)\n", "zlib"), ("// __thiscall\n", "__thiscall"),
                       ("// Returns 1 when done; callers never read EAX\n", "Returns")):
        res = out(src.replace(gone, ""))
        assert any(f"comment word {word} dropped" in c for c in res), (word, res)
    assert any("comment word EAX dropped" in c for c in out(src.replace("never read EAX", "never read it")))
    assert not any("Returns" in c for c in out(src.replace("Returns 1 when done", "returns 1 when done")))   # lower-case rewrite counts


def test_anonymous_namespace_symbol_ignores_path_and_hash():
    import gate
    a = r"??1PackFSDataInput_guess@?%C:\base\x\src\eqgfx\EverQuest\EQ_SoundManager.cpp53854328@@QAE@XZ"
    b = r"??1PackFSDataInput_guess@?%C:\other\x\src\eqgfx\EverQuest\EQ_SoundManager.cpp2975131212@@QAE@XZ"
    assert gate._anon(a) == gate._anon(b) == "??1PackFSDataInput_guess@?%anon@@QAE@XZ"
    assert gate._anon("?Total@Widget@@QAEHXZ") == "?Total@Widget@@QAEHXZ"
