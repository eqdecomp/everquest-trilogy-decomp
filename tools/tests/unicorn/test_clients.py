r"""The image registry (eqd_unicorn/clients.py) and what hangs off it: paths per image, the per-image toolchain and
default compiler options, and the match/compare layouts that keep a 0x10000000 image apart from the test objects.

    .venv\Scripts\python -m pytest eq-trilogy\tools\tests\unicorn\test_clients.py
"""
import sys
from pathlib import Path

import pytest

TOOLS = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS / "unicorn"))
sys.path.insert(0, str(TOOLS / "compare"))

from eqd_unicorn import clients, Workspace  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402
from eqd_unicorn.vc6 import cflags_for, image_cflags  # noqa: E402


def test_registry_basics():
    t, m = clients.get("trilogy"), clients.get("eqmain")
    assert (t.repo, t.queue, t.prefix, t.token, t.primary) == ("eq-trilogy", "queue/queue.jsonl", "", "TRILOGY", True)
    assert (m.repo, m.queue, m.prefix, m.token, m.toolchain) == ("eq-trilogy", "queue/eqmain.jsonl", "eqmain",
                                                                   "EQMAIN", "vc6_sp5")
    assert [c.id for c in clients.of_repo("eq-trilogy")][0] == "trilogy"
    other = clients.get("takp")                         # unregistered: the old single-image defaults
    assert (other.repo, other.queue, other.prefix) == ("eq-takp", "queue/queue.jsonl", "")


@pytest.mark.parametrize("rel, want", [
    ("src/eqmain/unit_10001000.cpp", "eqmain"), ("tests/unicorn/eqmain/unit_1/F.json", "eqmain"),
    ("eqmain/Network/rdp/rdp", "eqmain"), ("src/EverQuest/EQMisc.cpp", "trilogy"),
    ("tests/unicorn/Network/rdp/rdp/x.json", "trilogy"), ("tests/unicorn/crt_defaults_eqmain.json", "trilogy"),
    ("src\\eqmain\\x.cpp", "eqmain"), ("src/EQGfx_Dx8/SRC/x.cpp", "trilogy")])
def test_client_of_path(rel, want):
    assert clients.client_of_path(rel) == want


def test_sources_and_subsystems(tmp_path):
    for rel in ("src/EverQuest/a.cpp", "src/eqmain/unit_1.cpp", "src/eqmain/Network/rdp/heap.c", "src/x.h"):
        (tmp_path / rel).parent.mkdir(parents=True, exist_ok=True)
        (tmp_path / rel).write_text("")
    rel = lambda ps: [p.relative_to(tmp_path).as_posix() for p in ps]     # noqa: E731
    assert rel(clients.sources(tmp_path, "trilogy")) == ["src/EverQuest/a.cpp"]
    assert rel(clients.sources(tmp_path, "eqmain")) == ["src/eqmain/Network/rdp/heap.c", "src/eqmain/unit_1.cpp"]
    assert clients.subsystem("eqmain", "Network/rdp/rdp") == "eqmain/Network/rdp/rdp"
    assert clients.subsystem("eqmain", "eqmain/unit_1") == "eqmain/unit_1"
    assert clients.subsystem("eqmain", "library") == "library"
    assert clients.subsystem("trilogy", "Network/rdp/rdp") == "Network/rdp/rdp"
    assert clients.unassigned("unassigned") and clients.unassigned("eqmain/unassigned")
    assert not clients.unassigned("eqmain/unit_1")


def test_image_default_cflags_by_path():
    """eqmain.dll's sources build /O2 (an optimisation level replaces /O1); eqgame.exe's keep the defaults; a
    file's own CFLAGS lines still come last."""
    assert image_cflags(Path("C:/r/src/eqmain/unit_1.cpp")) == ["/O2"]
    assert image_cflags(Path("C:/x/eqmain_setup/y/src/EverQuest/a.cpp")) == []
    assert image_cflags(Path("C:/x/m123/src/eqmain/Network/rdp/heap.c")) == ["/O2"]   # mutation sandboxes


def test_cflags_for_combines_image_and_file(tmp_path):
    f = tmp_path / "src" / "eqmain" / "x.cpp"
    f.parent.mkdir(parents=True)
    f.write_text("// CFLAGS: /Oy-\nint x;\n")
    flags = cflags_for(f)
    assert "/O1" not in flags and flags[-2:] == ["/O2", "/Oy-"]
    g = tmp_path / "src" / "a.cpp"
    g.write_text("int y;\n")
    assert "/O1" in cflags_for(g) and "/O2" not in cflags_for(g)


def test_workspace_toolchain_and_binary_per_client(tmp_path, monkeypatch):
    (tmp_path / "paths.local.ini").write_text(
        "[workspace]\noriginal = original\nbuild = build\n[toolchain]\nvc6 = C:/sp4/VC98\nvc6_sp5 = C:/sp5/VC98\n")
    monkeypatch.setenv("EQD_ROOT", str(tmp_path))
    monkeypatch.delenv("EQD_REPO", raising=False)
    ws = Workspace(tmp_path)
    assert ws.vc6() == Path("C:/sp4/VC98") and ws.vc6("trilogy") == Path("C:/sp4/VC98")
    assert ws.vc6("eqmain") == Path("C:/sp5/VC98")
    assert ws.binary("eqmain") == tmp_path / "original" / "trilogy" / "eqmain.dll"
    assert ws.repo("eqmain") == tmp_path / "eq-trilogy"
    monkeypatch.setenv("EQD_REPO", str(tmp_path / "wt"))
    assert ws.repo("trilogy") == tmp_path / "wt"


def test_layouts_keep_a_0x10000000_image_apart():
    import coff
    import compare
    eqmain = compare.Side(Image("eqmain.dll", 0x10000000, bytearray(0x146000), 0x10000000))
    eqgame = compare.Side(Image("eqgame.exe", 0x400000, bytearray(0x200000), 0x400000))
    assert compare.rewrite_base(eqmain) == compare.REWRITE_LOAD_BASE and compare.rewrite_base(eqgame) is None
    ctx = compare.MatchContext.__new__(compare.MatchContext)
    ctx.o_side = eqmain
    assert ctx.obj_layout() == {"base": coff.ALT_OBJ_BASE, "extern_base": coff.ALT_EXTERN_BASE}
    ctx.o_side = eqgame
    assert ctx.obj_layout() == {}
    obj = coff.Obj([coff.Section(".text$x", b"\xc3", coff.IMAGE_SCN_CNT_CODE, [])], [])
    assert coff.link(obj).base == coff.OBJ_BASE
    assert coff.link(obj, base=coff.ALT_OBJ_BASE).base == coff.ALT_OBJ_BASE


def test_source_toolchain_line_then_folder_rule():
    st = clients.source_toolchain
    assert st("C:/r/src/eqmain/Network/rdp/tx.c", "") == "vc6"          # eqmain's rdp objects are SP4
    assert st("src/eqmain/unit_1.cpp", "") is None                       # the image's own (SP5)
    assert st("src/Network/rdp/tx.c", "") is None                        # eqgame.exe: no rule
    assert st("src/eqmain/a.cpp", "x\r\n// TOOLCHAIN: vc6_sp5\r\nint;") == "vc6_sp5"
    assert st("src/eqmain/Network/rdp/a.c", "/* TOOLCHAIN: vc6_sp5 */") == "vc6_sp5"   # the line wins
    assert st("src/a.cpp", " * TOOLCHAIN: vc6_rtm\n") == "vc6_rtm"
    assert st("src/a.cpp", "// TOOLCHAIN: vc6 is what rdp uses\n") is None       # prose, not a directive


def test_source_vc6_resolves_keys(tmp_path, monkeypatch):
    from eqd_unicorn import vc6 as v
    sp4, sp5 = tmp_path / "sp4", tmp_path / "sp5"
    for d in (sp4, sp5):
        (d / "Bin").mkdir(parents=True)
        (d / "Bin" / "CL.EXE").write_bytes(b"")
    monkeypatch.setattr(v, "_TOOLCHAIN_DIRS", {})
    monkeypatch.setenv("EQD_TOOLCHAIN_VC6", str(sp4))
    monkeypatch.setenv("EQD_TOOLCHAIN_VC6_SP5", str(sp5))
    rdp = tmp_path / "src" / "eqmain" / "Network" / "rdp" / "x.c"
    rdp.parent.mkdir(parents=True)
    rdp.write_text("int x;\n")
    game = tmp_path / "src" / "eqmain" / "y.cpp"
    game.write_text("int y;\n")
    assert v.source_vc6(rdp, sp5) == sp4 and v.source_vc6(game, sp5) == sp5
    cand = tmp_path / "work" / "x.c"                      # a permute candidate stands in for the rdp source
    cand.parent.mkdir()
    cand.write_text("int x;\n")
    assert v.source_vc6(cand, sp5) == sp5 and v.source_vc6(cand, sp5, like=rdp) == sp4
    game.write_text("// TOOLCHAIN: vc6\nint y;\n")
    assert v.source_vc6(game, sp5) == sp4
    game.write_text("// TOOLCHAIN: nope\nint y;\n")
    with pytest.raises(v.BuildError):
        v.source_vc6(game, sp5)
