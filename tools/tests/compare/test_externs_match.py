"""compare.py --match: a call to a function the source only declares under an `// EXTERNAL:` marker is keyed by the
marker's VA (MatchContext.externals); EQ_CDECL_CPP's eqd_cc<va>::f symbol maps to va (coff.eqd_va). Hand-made COFF
(no VC6, no game files); one VC6 test checks the decorated name of eqd_cc<va>::f (skips without VC6)."""
import sys
from pathlib import Path
from types import SimpleNamespace

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

import coff  # noqa: E402
import compare as C  # noqa: E402
from test_target_obj import CALL_OFF, CALLEE, FN, GLOBAL, GLOBAL_OFF, TEXT, code, context, make_obj  # noqa: E402

CTOR = "??0Item_guess@@QAE@XZ"


def rewrite_calling(tmp_path, sym):
    syms = [("_user", 0, 1, 0x20, 2), (sym, 0, 0, 0x20, 2), (f"_eqd_va_0x{GLOBAL:08X}", 0, 0, 0, 2)]
    return make_obj(tmp_path / "rw.obj", [(".text", code(b"\0\0\0\0", b"\0\0\0\0"), TEXT,
                                           [(CALL_OFF, 1, 0x14), (GLOBAL_OFF, 2, 0x06)])], syms)


def result(tmp_path, source_text, sym=CTOR, repo=None, xtu=None):
    src = tmp_path / "rw.cpp"
    src.write_text(source_text, encoding="utf-8")
    ctx = context()
    ctx.client = "trilogy"
    ctx.log = lambda *a: None
    ctx.repo = repo if repo is not None else tmp_path
    ann = SimpleNamespace(va=FN, name="user", line=1)
    (res,) = ctx.compare_obj(rewrite_calling(tmp_path, sym), src, [ann], {}, xtu=xtu)
    return res


def test_declared_constructor_is_keyed_by_the_markers_va(tmp_path):
    res = result(tmp_path, f"// EXTERNAL: TRILOGY {CALLEE:#010x} Item_guess::Item_guess\n")
    assert C.match_status(res) == "identical", res.diff
    # the same with the cross-TU map in use (compare_match passes one): the marker still applies
    assert C.match_status(result(tmp_path, f"// EXTERNAL: TRILOGY {CALLEE:#010x} Item_guess::Item_guess\n",
                                 xtu={})) == "identical"


def test_without_or_with_a_wrong_marker_the_call_differs(tmp_path):
    res = result(tmp_path, "class Item_guess { public: Item_guess(void); };\n")
    assert res.verdict == "differs" and f"F:{CTOR}" in "\n".join(res.diff), res.diff
    res = result(tmp_path, f"// EXTERNAL: TRILOGY {CALLEE + 0x10:#010x} Item_guess::Item_guess\n")
    assert res.verdict == "differs" and f"F:{CALLEE + 0x10:x}" in "\n".join(res.diff), res.diff
    res = result(tmp_path, f"// EXTERNAL: EQGFX {CALLEE:#010x} Item_guess::Item_guess\n")    # another client's
    assert res.verdict == "differs", res.diff


def test_a_header_under_include_counts_and_the_source_wins(tmp_path):
    repo = tmp_path / "repo"
    (repo / "include" / "trilogy").mkdir(parents=True)
    (repo / "include" / "trilogy" / "item.h").write_text(
        f"// EXTERNAL: TRILOGY {CALLEE:#010x} Item_guess::Item_guess\n", encoding="utf-8")
    assert C.match_status(result(tmp_path, '#include "trilogy/item.h"\n', repo=repo)) == "identical"
    res = result(tmp_path, f"// EXTERNAL: TRILOGY {CALLEE + 0x20:#010x} Item_guess::Item_guess\n", repo=repo)
    assert f"F:{CALLEE + 0x20:x}" in "\n".join(res.diff), res.diff


def test_a_definition_annotated_in_another_tu_wins_over_a_marker(tmp_path):
    res = result(tmp_path, f"// EXTERNAL: TRILOGY {CALLEE + 0x10:#010x} Item_guess::Item_guess\n",
                 xtu={CTOR: CALLEE})
    assert C.match_status(res) == "identical", res.diff


def test_eqd_cc_symbol_maps_to_its_va(tmp_path):
    assert coff.eqd_va("?f@?$eqd_cc@$0BAAFMCNF@@@SAXXZ") == ("fn", 0x1005C2D5)
    assert coff.eqd_va("?f@?$eqd_cc@$00@@SAXXZ") == ("fn", 1)
    assert coff.eqd_va("?g@?$eqd_cc@$0BAAFMCNF@@@SAXXZ") is None
    sym = f"?f@?$eqd_cc@$0{''.join('ABCDEFGHIJKLMNOP'[int(c, 16)] for c in f'{CALLEE:X}')}@@@SAXXZ"
    res = result(tmp_path, "", sym=sym)
    assert C.match_status(res) == "identical", res.diff


def test_vc6_names_eqd_cc_members_as_coff_expects(tmp_path):
    from eqd_unicorn import Workspace
    try:
        ws = Workspace()
        vc6 = ws.vc6("trilogy")
    except FileNotFoundError as e:
        pytest.skip(str(e))
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip("VC6 not found")
    import probe
    src = tmp_path / "cc.cpp"
    src.write_text("template<unsigned VA> struct eqd_cc { static void __cdecl f(void); };\n"
                   "int use(int a) { return ((int (__cdecl *)(int))&eqd_cc<0x1005C2D5>::f)(a) + "
                   "((int (__stdcall *)(int))&eqd_cc<0x00401000>::f)(a); }\n", encoding="utf-8")
    obj = coff.parse(probe.compile_snippet(src, tmp_path / "out", vc6=vc6))
    undefined = {s.name for s in obj.symbols if s and s.section == 0 and s.storage == 2}
    vas = {coff.eqd_va(n) for n in undefined} - {None}
    assert vas == {("fn", 0x1005C2D5), ("fn", 0x00401000)}, undefined


VFTABLE = "??_7Item_guess@@6B@"


def rewrite_storing_vftable(tmp_path):
    """The reference VC6 emits: to the ??_7 table it defines in the object (a section of its own)."""
    syms = [("_user", 0, 1, 0x20, 2), (CTOR, 0, 0, 0x20, 2), (VFTABLE, 0, 2, 0, 2)]
    return make_obj(tmp_path / "rw.obj", [(".text", code(b"\0\0\0\0", b"\0\0\0\0"), TEXT,
                                           [(CALL_OFF, 1, 0x14), (GLOBAL_OFF, 2, 0x06)]),
                                          (".rdata", b"\0\0\0\0", 0x40000040, [])], syms)


def vftable_result(tmp_path, marker):
    src = tmp_path / "rw.cpp"
    src.write_text(f"// EXTERNAL: TRILOGY {CALLEE:#010x} Item_guess::Item_guess\n{marker}", encoding="utf-8")
    ctx = context()
    ctx.client, ctx.log, ctx.repo = "trilogy", (lambda *a: None), tmp_path
    (res,) = ctx.compare_obj(rewrite_storing_vftable(tmp_path), src, [SimpleNamespace(va=FN, name="user", line=1)], {})
    return res


def test_a_marked_vftable_defined_in_the_object_reads_the_clients_address(tmp_path):
    res = vftable_result(tmp_path, f"// EXTERNAL: TRILOGY {GLOBAL:#010x} {VFTABLE}\n")
    assert C.match_status(res) == "identical", res.diff
    assert vftable_result(tmp_path, "").verdict == "differs"
    assert vftable_result(tmp_path, f"// EXTERNAL: TRILOGY {GLOBAL + 4:#010x} {VFTABLE}\n").verdict == "differs"


def test_a_vftable_va_is_not_a_function_va_of_the_generated_header(tmp_path):
    (tmp_path / "src").mkdir()
    (tmp_path / "src" / "a.cpp").write_text(
        f"// EXTERNAL: TRILOGY 0x00402000 Item_guess::Item_guess\n// EXTERNAL: TRILOGY 0x00535430 {VFTABLE}\n",
        encoding="utf-8")
    assert C.match_symbols(tmp_path)[0] == {"0x00402000"}
