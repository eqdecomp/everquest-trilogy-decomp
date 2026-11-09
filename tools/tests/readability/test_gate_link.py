"""gate.py's prototype journal and link check (protolink.py) on tiny synthetic C/C++ TUs, with VC6 (skips without
it): a retyped C++ prototype passes its callers, a half-done one is an unresolved external, a rename that collides
with another TU's definition is a duplicate, a lost extern "C" is a linkage mismatch, and an edit that adds or
removes no definition never runs the link check. The pure resolver is tested without VC6."""
import subprocess
from pathlib import Path

import pytest

import gate
import protolink as PL

HDR = """\
#ifndef SCALE_H
#define SCALE_H
int Scale_guess(int v);
#ifdef __cplusplus
extern "C" {
#endif
int Helper_guess(int v);
#ifdef __cplusplus
}
#endif
#endif
"""

DEF = """\
#include "trilogy/scale.h"

// FUNCTION: TRILOGY 0x00403000
int Scale_guess(int v)
{
    return v * 3;
}
"""

CALLER = """\
#include "trilogy/scale.h"

// FUNCTION: TRILOGY 0x{va}
int Use{n}_guess(int x)
{{
    return Scale_guess(x) + Helper_guess(x + {n});
}}
"""

CDEF = """\
#include "trilogy/scale.h"

// FUNCTION: TRILOGY 0x00405000
int Helper_guess(int v)
{
    return v + 7;
}
"""

COTHER = """\
// FUNCTION: TRILOGY 0x00406000
int Twice_guess(int v)
{
    return v + v;
}
"""

FILES = {
    "include/trilogy/scale.h": HDR,
    "src/EverQuest/scale.cpp": DEF,
    "src/EverQuest/caller1.cpp": CALLER.format(va="00404000", n=1),
    "src/EverQuest/caller2.cpp": CALLER.format(va="00404100", n=2),
    "src/EverQuest/helper.c": CDEF,
    "src/EverQuest/twice.c": COTHER,
    "tests/unicorn/README.txt": "fixtures\n",
}


def _vc6_or_skip():
    try:
        cl = gate.Workspace().vc6("trilogy") / "Bin" / "CL.EXE"
    except FileNotFoundError as e:
        pytest.skip(str(e))
    if not cl.is_file():
        pytest.skip(f"VC6 not found at {cl}")


@pytest.fixture
def lrepo(tmp_path):
    root = tmp_path / "lrepo"
    for rel, text in FILES.items():
        p = root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")
    for cmd in (["init", "-q"], ["config", "user.email", "t@example.com"], ["config", "user.name", "t"],
                ["config", "core.autocrlf", "false"], ["add", "-A"], ["commit", "-qm", "base"]):
        subprocess.run(["git", *cmd], cwd=root, check=True, capture_output=True)
    return root


def run(repo, tmp_path, **kw):
    return gate.run_gate(repo, "HEAD", None, cache=tmp_path / "cache", jobs=2, **kw)


def edit(repo, rel, old, new):
    p = repo / rel
    t = p.read_text(encoding="utf-8")
    assert old in t, (rel, old)
    p.write_text(t.replace(old, new), encoding="utf-8")


def test_retyped_prototype_passes_callers(lrepo, tmp_path, monkeypatch):
    _vc6_or_skip()
    edit(lrepo, "include/trilogy/scale.h", "int Scale_guess(int v);", "int Scale_guess(long v);")
    edit(lrepo, "src/EverQuest/scale.cpp", "int Scale_guess(int v)", "int Scale_guess(long v)")
    r = run(lrepo, tmp_path)
    assert r.ok, r.report()
    assert {t.rel for t in r.tus} >= {"src/EverQuest/scale.cpp", "src/EverQuest/caller1.cpp",
                                      "src/EverQuest/caller2.cpp"}
    pair = [j for j in r.protos if j["kind"] == "def"]
    assert [(j["va"], j["old"], j["new"]) for j in pair] == [("00403000", "?Scale_guess@@YAHH@Z",
                                                                "?Scale_guess@@YAHJ@Z")], r.report()
    assert r.link_notes and not r.link, r.report()
    assert "proto trilogy 00403000 ?Scale_guess@@YAHH@Z -> ?Scale_guess@@YAHJ@Z" in r.report()

    # without the journal the callers still pass: the annotated callee is keyed by its VA on both sides (xtu)
    monkeypatch.setattr(gate, "tu_journal", lambda *a, **k: [])
    r = run(lrepo, tmp_path)
    assert r.ok, r.report()
    # without the journal and the cross-TU keys the callers fail on the call target's name (the gap this closes)
    monkeypatch.setattr(gate, "cross_tu_maps", lambda *a, **k: {})
    r = run(lrepo, tmp_path)
    assert not r.ok
    assert {t.rel for t in r.tus if t.changed} == {"src/EverQuest/caller1.cpp", "src/EverQuest/caller2.cpp"}


def test_half_retyped_prototype_is_unresolved(lrepo, tmp_path):
    """The header changes, the definition keeps `int`: per TU everything compiles the same (the callers' targets
    are paired by undecorated name), but nothing defines the name the callers now call."""
    _vc6_or_skip()
    edit(lrepo, "include/trilogy/scale.h", "int Scale_guess(int v);", "int Scale_guess(long v);")
    r = run(lrepo, tmp_path)
    assert all(t.ok for t in r.tus), r.report()
    assert not r.ok
    assert any("LNK2001 unresolved external ?Scale_guess@@YAHJ@Z" in l and "caller1.cpp" in l and
               "defined as ?Scale_guess@@YAHH@Z" in l and "a different signature" in l for l in r.link), r.report()


def test_rename_onto_another_tus_definition_is_a_duplicate(lrepo, tmp_path):
    """The hooks DLL break: a rename (same code, T0 passes) to a name another TU already defines."""
    _vc6_or_skip()
    edit(lrepo, "src/EverQuest/twice.c", "Twice_guess", "Helper_guess")
    r = run(lrepo, tmp_path)
    assert all(t.ok for t in r.tus), r.report()
    assert not r.ok
    assert any("LNK2005 duplicate definition of _Helper_guess" in l and "helper.c" in l and "twice.c" in l
               for l in r.link), r.report()
    # a fresh name is fine (the link check runs: a definition was added and one removed)
    edit(lrepo, "src/EverQuest/twice.c", "Helper_guess", "Double_guess")
    r = run(lrepo, tmp_path)
    assert r.ok, r.report()
    assert r.link_notes


def test_lost_extern_c_is_a_linkage_mismatch(lrepo, tmp_path):
    _vc6_or_skip()
    edit(lrepo, "include/trilogy/scale.h", 'extern "C" {', "namespace_free {")
    edit(lrepo, "include/trilogy/scale.h", "namespace_free {", "")
    edit(lrepo, "include/trilogy/scale.h", "#ifdef __cplusplus\n}\n#endif\n", "")
    r = run(lrepo, tmp_path)
    assert not r.ok
    assert any("LNK2001 unresolved external ?Helper_guess@@YAHH@Z" in l and "C vs C++ linkage mismatch" in l
               and "_Helper_guess" in l for l in r.link), r.report()


def test_fast_path_skips_the_link_check(lrepo, tmp_path):
    _vc6_or_skip()
    edit(lrepo, "src/EverQuest/caller1.cpp", "{\n    return", "{\n    // a comment\n    return")
    r = run(lrepo, tmp_path)
    assert r.ok, r.report()
    assert not r.link_notes and not r.protos


def test_protos_file_round_trip(tmp_path):
    p = tmp_path / "p.jsonl"
    p.write_text('{"client": "trilogy", "va": "00403000", "old": "?f@@YAXH@Z", "new": "?f@@YAXJ@Z", "kind": "proto"}\n'
                 '{"client": "trilogy", "kind": "fn", "old": "A", "new": "B"}\n', encoding="utf-8")
    assert gate.load_protos(p) == [{"client": "trilogy", "va": "00403000", "old": "?f@@YAXH@Z", "new": "?f@@YAXJ@Z"}]


def test_header_use_lines():
    import re
    w = re.compile(r"\bf_guess\b")
    assert not gate._header_uses("int f_guess(int a);", w)                       # a prototype: nothing to link
    assert not gate._header_uses("#define f_guess EQ_CDECL(int, 0x00401000, (int))", w)   # by VA
    assert gate._header_uses("#define CALL_F(x) f_guess(x)", w)
    assert gate._header_uses("inline int g(int a) { return f_guess(a); }", w)
    assert not gate._header_uses("// inline { f_guess", w)


def _syms(defs=(), refs=(), comdat=()):
    o = PL.ExtSyms()
    o.defs = {n: "strong" for n in defs} | {n: "comdat" for n in comdat}
    o.refs = set(refs)
    return o


def test_resolver():
    und = gate.undecorate
    objs = {"a.c": _syms(defs=["_f"]), "b.c": _syms(defs=["_f"], refs=["_g"]),
            "c.cpp": _syms(comdat=["?h@@YAXXZ"]), "d.cpp": _syms(comdat=["?h@@YAXXZ"], refs=["?k@@YAXH@Z"]),
            "e.cpp": _syms(defs=["?k@@YAXJ@Z", "?k@@YAXH@Z"])}
    out = PL.resolve(objs, {"_f", "?h@@YAXXZ", "_g"}, {}, set(), und)
    assert out == ["LNK2005 duplicate definition of _f (f): a.c, b.c"]      # inline COMDATs merge; _g not ours
    out = PL.resolve(objs, {"_g"}, {}, {"_g"}, und)
    assert out == ["LNK2001 unresolved external _g (g) referenced by b.c"]
    # a journaled pair whose old name is still defined: the callers switched overloads
    out = PL.resolve(objs, {"?k@@YAXH@Z", "?k@@YAXJ@Z"}, {"?k@@YAXH@Z": "?k@@YAXJ@Z"}, set(), und)
    assert out and "different overload" in out[0]
    # pairing: by VA first, then by undecorated name (one each side); renames already applied to `before`
    assert PL.pair_names({"?f@@YAXH@Z": "?f@@YAXH@Z", "_g": "_g"}, {"?f@@YAXJ@Z", "_g"}, {}, {}, und) == \
        {"?f@@YAXH@Z": "?f@@YAXJ@Z"}
    assert PL.pair_names({"?f@@YAXH@Z": "x", "?f@@YAXM@Z": "y"}, {"?f@@YAXJ@Z"}, {}, {}, und) == {}
    assert PL.pair_names({"?f@@YAXH@Z": "x", "?f@@YAXM@Z": "y"}, {"?f@@YAXJ@Z"}, {"?f@@YAXM@Z": 5},
                         {"?f@@YAXJ@Z": 5}, und) == {"?f@@YAXM@Z": "?f@@YAXJ@Z"}


# ------------------------------------------------------------------ link_check without VC6 (fake objects)

def _fake_link_env(monkeypatch, tmp_path, syms, build_fail=()):
    """Objects are marker paths: ext_symbols looks the ExtSyms up by '<root name>/<rel>'."""
    def compile_tu(ws, root, rel, client, out, header):
        Path(out).mkdir(parents=True, exist_ok=True)
        if (Path(root).name, rel) in build_fail:
            return None, "error"
        return Path(Path(root).name) / rel, ""
    monkeypatch.setattr(gate, "compile_tu", compile_tu)
    monkeypatch.setattr(gate, "include_closure", lambda graph, p: [])
    monkeypatch.setattr(gate, "link_candidates", lambda *a, **k: ["src/EverQuest/c1.cpp", "src/EverQuest/c2.cpp"])
    monkeypatch.setattr(PL, "ext_symbols", lambda o: syms[Path(o).as_posix()])
    for root in ("after", "base"):
        for rel in ("src/EverQuest/c1.cpp", "src/EverQuest/c2.cpp", "src/EverQuest/ch.cpp"):
            p = tmp_path / root / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text("x\n")
    return tmp_path / "after", tmp_path / "base"


def _run_link(monkeypatch, tmp_path, syms, build_fail=(), with_base=True):
    after, base = _fake_link_env(monkeypatch, tmp_path, syms, build_fail)
    changed_a = {"src/EverQuest/ch.cpp": Path("a/ch")}
    changed_b = {"src/EverQuest/ch.cpp": Path("b/ch")}
    return gate.link_check(None, after, "trilogy", changed_a, changed_b, {}, set(), tmp_path / "cache", 1,
                           lambda: None, base if with_base else None, lambda: None)


def test_candidate_that_does_not_build_fails_the_link_check(monkeypatch, tmp_path):
    """A stale caller in a TU that no longer builds is an LNK2001 nobody sees: a failure, not a note."""
    syms = {"a/ch": _syms(defs=["_g2"]), "b/ch": _syms(defs=["_g"]),
            "after/src/EverQuest/c1.cpp": _syms(), "base/src/EverQuest/c1.cpp": _syms(),
            "after/src/EverQuest/c2.cpp": _syms(), "base/src/EverQuest/c2.cpp": _syms()}
    fails, notes = _run_link(monkeypatch, tmp_path, syms, build_fail={("after", "src/EverQuest/c1.cpp")})
    assert len(fails) == 1 and "c1.cpp" in fails[0] and "does not build" in fails[0], fails
    # it fails on the base too: not this change's, a note
    fails, notes = _run_link(monkeypatch, tmp_path / "t2", syms, build_fail={("after", "src/EverQuest/c1.cpp"),
                                                                           ("base", "src/EverQuest/c1.cpp")})
    assert not fails and "do not build on the base either" in notes[0], (fails, notes)


def test_preexisting_link_problems_are_not_new(monkeypatch, tmp_path):
    """A duplicate definition main already has (clones within a client) fails nothing; a new one does."""
    c1, c2 = "src/EverQuest/c1.cpp", "src/EverQuest/c2.cpp"
    syms = {"a/ch": _syms(defs=["_n"]), "b/ch": _syms(defs=["_n"]),
            "after/" + c1: _syms(defs=["_n"]), "base/" + c1: _syms(defs=["_n"]),
            "after/" + c2: _syms(), "base/" + c2: _syms()}

    def run(root, with_base):
        after, base = _fake_link_env(monkeypatch, root, syms)
        return gate.link_check(None, after, "trilogy", {"src/EverQuest/ch.cpp": Path("a/ch")},
                               {"src/EverQuest/ch.cpp": Path("b/ch")}, {"_o": "_n"}, set(), root / "cache", 1,
                               lambda: None, base if with_base else None, lambda: None)[0]
    assert any("LNK2005 duplicate definition of _n" in f for f in run(tmp_path / "a", False))
    assert run(tmp_path / "b", True) == []                          # same on the base: not this change's
    syms["after/" + c2] = _syms(defs=["_n"])                         # a third definition appears: NEW (different text)
    fails = run(tmp_path / "c", True)
    assert len(fails) == 1 and "c2.cpp" in fails[0], fails
