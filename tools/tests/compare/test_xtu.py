"""compare.py --match: a call to another TU's annotated function is keyed by the definition's VA (xtu.py), by its
decorated name only. Hand-made COFF for the keying (no VC6, no game files); VC6 for MatchContext.cross_tu (skips
without it): the defining TU is found by its annotation and built, an unannotated overload is left out."""
import sys
from pathlib import Path
from types import SimpleNamespace

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

import compare as C  # noqa: E402
import xtu as X  # noqa: E402
from test_target_obj import CALL_OFF, CALLEE, FN, GLOBAL, GLOBAL_OFF, TEXT, code, context, make_obj  # noqa: E402

SYM = "?Callee_guess@@YAXH@Z"


def rewrite_by_symbol(tmp_path):
    """`user` calling CALLEE by its C++ symbol (defined in another TU), not by `eqd_fn_<va>`."""
    syms = [("_user", 0, 1, 0x20, 2), (SYM, 0, 0, 0x20, 2), (f"_eqd_va_0x{GLOBAL:08X}", 0, 0, 0, 2)]
    return make_obj(tmp_path / "rw.obj", [(".text", code(b"\0\0\0\0", b"\0\0\0\0"), TEXT,
                                           [(CALL_OFF, 1, 0x14), (GLOBAL_OFF, 2, 0x06)])], syms)


def result(tmp_path, by_name, xtu):
    ann = SimpleNamespace(va=FN, name="user", line=1)
    (res,) = context().compare_obj(rewrite_by_symbol(tmp_path), Path("rw.cpp"), [ann], by_name, xtu=xtu)
    return res


def test_symbol_keyed_by_its_definitions_va(tmp_path):
    res = result(tmp_path, {}, {SYM: CALLEE})
    assert C.match_status(res) == "identical", res.diff


def test_definition_at_another_va_differs(tmp_path):
    res = result(tmp_path, {}, {SYM: CALLEE + 0x10})
    assert res.verdict == "differs" and f"F:{CALLEE + 0x10:x}" in "\n".join(res.diff), res.diff


def test_undecorated_name_alone_is_not_enough_with_xtu(tmp_path):
    """With the decorated map, an undecorated name match (an overload of an annotated name) keeps its symbol."""
    res = result(tmp_path, {"Callee_guess": {CALLEE}}, {"?Callee_guess@@YAXJ@Z": CALLEE})
    assert res.verdict == "differs" and f"F:{SYM}" in "\n".join(res.diff), res.diff
    # without a map (permute's scoring) the old undecorated rule still applies
    assert C.match_status(result(tmp_path, {"Callee_guess": {CALLEE}}, None)) == "identical"


def test_annotated_defs_rules(tmp_path):
    """Only external function definitions, an annotated name with one VA, defined once in the object."""
    a = lambda name, va: SimpleNamespace(name=name, va=va)      # noqa: E731
    obj = make_obj(tmp_path / "d.obj", [(".text", b"\xc3" * 4, TEXT, [])],
                   [("?f@@YAXH@Z", 0, 1, 0x20, 2), ("?g@@YAXH@Z", 1, 1, 0x20, 2), ("?g@@YAXJ@Z", 2, 1, 0x20, 2),
                    ("?s@@YAXXZ", 3, 1, 0x20, 3), ("?u@@YAXXZ", 0, 0, 0x20, 2)])
    m = X.annotated_defs(obj, [a("f", 0x401000), a("g", 0x402000), a("s", 0x403000), a("u", 0x404000)])
    assert m == {"?f@@YAXH@Z": 0x401000}
    assert X.annotated_defs(obj, [a("f", 0x401000), a("f", 0x401010)]) == {}       # two VAs for one name
    assert X.merge([{"a": 1, "b": 2}, {"a": 1, "b": 3}]) == {"a": 1}
    assert X.external_refs(obj) == {"?u@@YAXXZ"}


# --- MatchContext.cross_tu with VC6 ------------------------------------------------------------------------------

HDR = """\
class Overlay_guess
{
public:
    int state;
    Overlay_guess();
    Overlay_guess(int mode);
    ~Overlay_guess();
};
class Bag_guess : public Overlay_guess
{
public:
    Bag_guess(int unused);
    Bag_guess(char mode);
};
void Notify_guess(Overlay_guess* o);
"""

FILES = {
    "include/trilogy/overlay.h": HDR,
    "src/EverQuest/overlay.cpp": '#include "trilogy/overlay.h"\n\n// FUNCTION: TRILOGY 0x00450049\n'
                                 "Overlay_guess::Overlay_guess()\n{\n    state = 1;\n}\n\n"
                                 "// FUNCTION: TRILOGY 0x0045005d\nOverlay_guess::~Overlay_guess()\n{\n    state = 0;\n}\n",
    "src/EverQuest/mode.cpp": '#include "trilogy/overlay.h"\n\n'
                              "Overlay_guess::Overlay_guess(int mode)\n{\n    state = mode;\n}\n",
    "src/EverQuest/bag.cpp": '#include "trilogy/overlay.h"\n\n// FUNCTION: TRILOGY 0x00416d0c\n'
                             "Bag_guess::Bag_guess(int unused)\n{\n    state = unused;\n    Notify_guess(this);\n}\n\n"
                             "// FUNCTION: TRILOGY 0x00416e00\n"
                             "Bag_guess::Bag_guess(char mode) : Overlay_guess(mode)\n{\n}\n",
}


def test_cross_tu_builds_the_defining_tu(tmp_path):
    from eqd_unicorn import Workspace
    try:
        ws = Workspace()
        if not (ws.vc6("trilogy") / "Bin" / "CL.EXE").is_file():
            pytest.skip("VC6 not found")
    except FileNotFoundError as e:
        pytest.skip(str(e))
    repo = tmp_path / "repo"
    for rel, text in FILES.items():
        (repo / rel).parent.mkdir(parents=True, exist_ok=True)
        (repo / rel).write_text(text, encoding="utf-8")
    ctx = object.__new__(C.MatchContext)
    ctx.ws, ctx.repo, ctx.client = ws, repo.resolve(), "trilogy"
    ctx.out_dir = tmp_path / "out"
    ctx.out_dir.mkdir()
    ctx.header = ctx.out_dir / "eqd_match_syms.h"
    C.write_match_header(ctx.header, set(), set())
    bag = (repo / "src/EverQuest/bag.cpp").resolve()
    obj, text, _f = ctx.compile(bag)
    assert obj is not None, text
    anns = {bag: C.ann.scan([bag], "TRILOGY")}
    refs = X.external_refs(obj)
    assert {"??0Overlay_guess@@QAE@XZ", "??0Overlay_guess@@QAE@H@Z", "??1Overlay_guess@@QAE@XZ"} <= refs, refs
    m = ctx.cross_tu({bag: obj}, anns, log=lambda *a: None)
    assert m["??0Overlay_guess@@QAE@XZ"] == 0x00450049 and m["??1Overlay_guess@@QAE@XZ"] == 0x0045005D, m
    assert "??0Overlay_guess@@QAE@H@Z" not in m                 # unannotated overload (mode.cpp): not keyed
    assert (ctx.out_dir / "src_EverQuest_overlay.obj").is_file()  # the defining TU was built
    assert not (ctx.out_dir / "src_EverQuest_mode.obj").is_file()  # mode.cpp has no annotation: never a candidate


# --- objects kept across builds (compare_match builds every source, then cross_tu the defining TUs) --------------

def bare_context(tmp_path, ws=None):
    ctx = object.__new__(C.MatchContext)
    ctx.ws, ctx.repo, ctx.client = ws, (tmp_path / "repo").resolve(), "eqgfx"
    ctx.out_dir = tmp_path / "out"
    ctx.out_dir.mkdir(exist_ok=True)
    ctx.header = ctx.out_dir / "eqd_match_syms.h"
    return ctx


def test_compile_kept_gives_a_shared_stem_its_own_object(tmp_path, monkeypatch):
    """verify.c and verify.cpp both map to ..._verify.obj: the second source built in one context gets the name with
    its extension, so the first one's object is not overwritten while compare_match still needs it (regression:
    eqgfx getFileSize_guess 0x10052A80 in verify.c compared against verify.cpp's object -> error)."""
    names = []
    monkeypatch.setattr(C, "compile_match_obj",
                        lambda ws, repo, src, out_dir, header, client=None, like=None, name=None:
                        (names.append(name) or out_dir / name, "", []))
    ctx = bare_context(tmp_path)
    c_src, cpp_src = (ctx.repo / "src/eqgfx/ff/verify.c"), (ctx.repo / "src/eqgfx/ff/verify.cpp")
    assert ctx.compile_kept(c_src)[0].name == "src_eqgfx_ff_verify.obj"
    assert ctx.compile_kept(cpp_src)[0].name == "src_eqgfx_ff_verify_cpp.obj"
    assert ctx.compile_kept(c_src)[0].name == "src_eqgfx_ff_verify.obj"        # the same source keeps its name
    assert ctx.compile_kept(cpp_src)[0].name == "src_eqgfx_ff_verify_cpp.obj"
    # the default (objdiff, permute, compile()) name is unchanged
    import objdiff_config
    assert C.match_obj_name(ctx.repo, c_src) == objdiff_config.obj_name(ctx.repo, "src/eqgfx/ff/verify.c")


def test_compare_match_and_cross_tu_build_with_compile_kept():
    import inspect
    assert "ctx.compile_kept(src)" in inspect.getsource(C.compare_match)
    assert "self.compile_kept(src)" in inspect.getsource(C.MatchContext.cross_tu)


def test_compile_kept_objects_both_survive_with_vc6(tmp_path):
    from eqd_unicorn import Workspace
    import coff
    try:
        ws = Workspace()
        if not (ws.vc6("trilogy") / "Bin" / "CL.EXE").is_file():
            pytest.skip("VC6 not found")
    except FileNotFoundError as e:
        pytest.skip(str(e))
    ctx = bare_context(tmp_path, ws)
    ctx.client = "trilogy"
    C.write_match_header(ctx.header, set(), set())
    srcs = {"verify.c": "// FUNCTION: TRILOGY 0x10052a80\nint getFileSize_guess(int a)\n{\n    return a + 1;\n}\n",
            "verify.cpp": "// FUNCTION: TRILOGY 0x10052b00\nint verifyHeader_guess(int a)\n{\n    return a * 3;\n}\n"}
    objs = {}
    for name, text in srcs.items():
        p = ctx.repo / "src/eqgfx/ff" / name
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")
        obj, out, _f = ctx.compile_kept(p)
        assert obj is not None, out
        objs[name] = obj
    assert objs["verify.c"] != objs["verify.cpp"]
    defined = {n: {s.name for s in coff.parse(o).symbols if s and s.section > 0} for n, o in objs.items()}
    assert "_getFileSize_guess" in defined["verify.c"], defined
    assert "?verifyHeader_guess@@YAHH@Z" in defined["verify.cpp"], defined
