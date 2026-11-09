"""make_fixture / mutate_check against the eq-trilogy repo next to this checkout (read-only: nothing is written
into it). Skips without the workspace, the repo, the original binary or VC6."""
import hashlib
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn" / "tools"))

import eqd_repo  # noqa: E402

VA = 0x00410C80                                   # CDisplay::IsInvisible_guess, src/EverQuest/Display.cpp


@pytest.fixture(scope="module")
def repo():
    root = eqd_repo.find_root()
    if root is None or not (root / "eq-trilogy" / "tests" / "unicorn" / "conftest.py").is_file():
        pytest.skip("eq-trilogy not found")
    r = eqd_repo.Repo(root / "eq-trilogy")
    if not r.fixtures_for(VA):
        pytest.skip(f"no fixture for {VA:#x} in eq-trilogy")
    return r


def _harness_or_skip(repo):
    try:
        return repo.harness()
    except eqd_repo.RepoError as e:
        pytest.skip(str(e))


def _digest(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def test_make_fixture_skeleton(repo):
    pytest.importorskip("pefile")
    import make_fixture as mf
    try:
        fx, out = mf.make(repo, "trilogy", VA, build=False, log=lambda *a: None)
    except SystemExit as e:
        pytest.skip(str(e))
    assert fx["original"] == "0x00410c80" and fx["sources"] == ["src/EverQuest/Display.cpp"]
    assert fx["conv"] == "thiscall" and [a["type"] for a in fx["args"]] == ["ptr", "ptr"] and fx["ret"] == "int"
    assert fx["objects"]["viewer"]["size"] == "0x134"                     # C_ASSERT(sizeof(EQPlayer) == 0x134)
    assert fx["callees"]["0x0042577f"]["conv"] == "thiscall"
    assert fx["cases"][0]["args"] == ["@viewer", "@target"] and fx["_todo"]
    assert out == repo.tests_dir / "EverQuest" / "Display" / "IsInvisible.json"


def test_mutate_check_catches_and_leaves_no_trace(repo, tmp_path, capsys):
    _harness_or_skip(repo)
    import mutate_check as mc
    src = repo.root / "src" / "EverQuest" / "Display.cpp"
    before = _digest(src)
    fx = repo.fixtures_for(VA)[0]
    rc = mc.main([str(fx), "--repo", str(repo.root), "--max", "3", "--work", str(tmp_path),
                  "--json", str(tmp_path / "r.json")])
    out = capsys.readouterr().out
    assert rc == 0, out
    assert "3/3 caught, 0 survived" in out
    assert _digest(src) == before
    assert [p for p in tmp_path.iterdir() if p.is_dir()] == []              # sandbox removed


def test_mutate_check_interrupt_cleans_up(repo, tmp_path, monkeypatch):
    _harness_or_skip(repo)
    import mutate_check as mc
    src = repo.root / "src" / "EverQuest" / "Display.cpp"
    before = _digest(src)
    real, calls = mc.run_all, []

    def flaky(*a, **kw):
        calls.append(1)
        if len(calls) == 2:                         # baseline ran; the first mutant is in its copy now
            raise KeyboardInterrupt
        return real(*a, **kw)

    monkeypatch.setattr(mc, "run_all", flaky)
    rc = mc.main([str(repo.fixtures_for(VA)[0]), "--repo", str(repo.root), "--max", "3", "--work", str(tmp_path)])
    assert rc == 130 and _digest(src) == before
    assert [p for p in tmp_path.iterdir() if p.is_dir()] == []


def test_make_fixture_eax_evidence_fixes_void():
    pytest.importorskip("unicorn")
    import make_fixture as mf

    class FakePc:
        def __init__(self, v):
            self.v = v

        def eax_summary(self, ex, f, cg, asm):
            return "EAX read after the call at 2/2 direct call sites -> x", {}, self.v

    class FakeEx:
        functions = {"00401366": {}}
        callgraph = {"00401366": {"callers": []}}

        def asm(self, addr):
            return None

    fx, todo = {"ret": "void"}, []
    mf.eax_ret_evidence(FakePc({"verdict": "value", "ret": "u8"}), FakeEx(), "00401366", fx, todo)
    assert fx["ret"] == "u8" and "returns a value" in todo[0]
    fx, todo = {"ret": "int"}, []
    mf.eax_ret_evidence(FakePc({"verdict": "void", "ret": None}), FakeEx(), "00401366", fx, todo)
    assert fx["ret"] == "int" and "likely void" in todo[0]
    fx, todo = {"ret": "void"}, []
    mf.eax_ret_evidence(FakePc({"verdict": "inconclusive", "ret": None}), FakeEx(), "00401366", fx, todo)
    assert fx["ret"] == "void" and not todo
