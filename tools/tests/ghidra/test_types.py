"""Routing and fail-closed header preparation; no JVM or VC6 compiler required."""
import json
import sys
from pathlib import Path

import pytest

SCRIPTS = Path(__file__).resolve().parents[2] / "ghidra" / "scripts"
sys.path.insert(0, str(SCRIPTS))
import eqd_ghidra as eqd
import gdt_headers as headers
import build_gdt as build
import apply_typed_symbols as symbols


@pytest.fixture
def workspace(tmp_path, monkeypatch):
    monkeypatch.setenv("EQD_ROOT", str(tmp_path))
    monkeypatch.delenv("EQD_REPO", raising=False)
    monkeypatch.delenv("EQD_CONFIG", raising=False)
    monkeypatch.delenv("EQD_REPORT_DIR", raising=False)
    (tmp_path / "paths.local.ini").write_text("[workspace]\nbuild=build/out\n")
    return eqd.Workspace()


@pytest.mark.parametrize("client", ["trilogy", "eqgfx", "eqmain"])
def test_three_clients_share_repository(workspace, client):
    assert workspace.repo(client) == workspace.root / "eq-trilogy"
    assert workspace.archive(client) == workspace.reports / f"{client}.gdt"
    assert workspace.typed_symbols(client) == workspace.root / "eq-trilogy" / "ghidra-types" / f"{client}.symbols.csv"


def test_config_routes_inputs_outputs(workspace, monkeypatch):
    cfg = workspace.root / "session.ini"
    cfg.write_text("[workspace]\nghidra_reports=build/session\n[repos]\neqgfx=source-copy\n"
                   "[ghidra_types]\neqgfx_archive=build/archive.gdt\neqgfx_symbols=build/symbols.csv\n")
    monkeypatch.setenv("EQD_CONFIG", str(cfg))
    ws = eqd.Workspace()
    assert ws.repo("eqgfx") == ws.root / "source-copy"
    assert ws.archive("eqgfx") == ws.root / "build/archive.gdt"
    assert ws.typed_symbols("eqgfx") == ws.root / "build/symbols.csv"
    assert ws.reports == ws.root / "build/session"


def test_environment_worktree_and_report_override(workspace, monkeypatch):
    monkeypatch.setenv("EQD_REPO", str(workspace.root / "worktree"))
    monkeypatch.setenv("EQD_REPORT_DIR", str(workspace.root / "reports"))
    ws = eqd.Workspace()
    assert ws.repo("eqmain") == ws.root / "worktree"
    assert ws.reports == ws.root / "reports"


def write_header(repo, rel, text):
    path = repo / "include" / rel
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def test_nested_headers_and_dependency_closure(tmp_path):
    write_header(tmp_path, "eqmain/top.h", '#include "layout/child.h"\n')
    write_header(tmp_path, "eqmain/layout/child.h", '#include "trilogy/common.h"\n')
    write_header(tmp_path, "eqmain/layout/unreferenced.h", 'struct Extra { int x; };\n')
    write_header(tmp_path, "trilogy/common.h", 'typedef unsigned long u32;\n')
    assert headers.header_paths(tmp_path, "eqmain") == [
        "include/eqmain/layout/child.h", "include/eqmain/layout/unreferenced.h",
        "include/eqmain/top.h", "include/trilogy/common.h"]


def test_missing_include_fails(tmp_path):
    write_header(tmp_path, "eqgfx/layout/x.h", '#include "missing.h"\n')
    with pytest.raises(ValueError, match="missing local include"):
        headers.load_headers(tmp_path, "eqgfx")


def test_umbrella_preserves_storage_and_callback(tmp_path):
    source = '''
typedef int (__cdecl *Callback)(int value);
#pragma pack(push, 1)
class Base { public: char lead; int value; void method(); };
#pragma pack(pop)
class Child : public Base {
public:
    union { int code; float value; };
    Callback callback;
    Child();
};
C_ASSERT(sizeof(Base) == 5);
C_ASSERT(offsetof(Child, callback) == 12);
'''
    write_header(tmp_path, "trilogy/layout/child.h", source)
    out = tmp_path / "generated/trilogy.h"
    idx, names = headers.generate(tmp_path, "trilogy", out)
    text = out.read_text()
    assert set(names) == {"Base", "Child"}
    assert "Base _base_0;" in text
    assert "#pragma pack(push, 1)" in text
    assert "union {" in text
    assert "typedef int (__cdecl *Callback)(int value);" in text
    assert "method(" not in text and "Child();" not in text
    assert "C_ASSERT" not in text
    assert len(build.assertions(idx)[0]) == 1
    assert len(build.assertions(idx)[1]) == 1
    assert (tmp_path / "include/trilogy/layout/child.h").read_text() == source


def test_callback_parameter_named_type_is_kept(tmp_path):
    source = ("struct Dev;\n"
              "typedef long (__stdcall *SetStateFn)(struct Dev *self, int type, long value);\n"
              "template<class R> struct ret { typedef R type; };\n"
              "struct Dev { SetStateFn SetState; };\n")
    write_header(tmp_path, "eqgfx/layout/dev.h", source)
    out = tmp_path / "eqgfx.h"
    headers.generate(tmp_path, "eqgfx", out)
    text = out.read_text()
    assert "typedef long (__stdcall *SetStateFn)(struct Dev *self, int type, long value);" in text
    assert "typedef R type;" not in text
    assert headers.typedef_name("typedef char __C_ASSERT__[1];") == "__C_ASSERT__"


def test_unresolved_array_bound_fails(tmp_path):
    write_header(tmp_path, "eqgfx/x.h", "struct X { char data[UNKNOWN]; };\n")
    with pytest.raises(ValueError, match="unresolved array bound"):
        headers.generate(tmp_path, "eqgfx", tmp_path / "x.h")


def test_unknown_assertion_fails(tmp_path):
    write_header(tmp_path, "eqgfx/x.h", "C_ASSERT(1 == 1);\n")
    with pytest.raises(ValueError, match="uncheckable C_ASSERT"):
        build.assertions(headers.load_headers(tmp_path, "eqgfx"))


def test_multiline_assertions_and_comments(tmp_path):
    write_header(tmp_path, "eqgfx/x.h", "struct X { int field; };\n"
                 "C_ASSERT(\nsizeof(X) == 4\n); // checked\n"
                 "C_ASSERT(offsetof(X, field) == 0); /* checked */\n"
                 "C_ASSERT(offsetof(X, field) + 4 == 4);\n")
    sizes, offsets = build.assertions(headers.load_headers(tmp_path, "eqgfx"))
    assert sizes[0][:2] == ("X", 4)
    assert offsets[0][:3] == ("X", "field", 0)
    assert offsets[1][:3] == ("X", "field", 0)


def test_canonical_overlay_and_scoped_field_renames(tmp_path):
    canonical = tmp_path / "canonical"
    overlays = tmp_path / "overlays"
    canonical.mkdir()
    overlays.mkdir()
    doc = {"family": "X", "client": "trilogy", "size": "0x4", "fields": [{"name": "old", "offset": "0x0"}]}
    (canonical / "x.json").write_text(json.dumps(doc))
    (canonical / "x.pre_names.json").write_text(json.dumps(doc))
    (canonical / "mapping.json").write_text('{}')
    doc["size"] = "0x8"
    (overlays / "x.json").write_text(json.dumps(doc))
    edits = tmp_path / "fields.json"
    edits.write_text(json.dumps({"mapping": {"old": "new"}, "choices": [
        {"layout": "x.json", "family": "X", "name": "old"}]}))
    docs = build.canonical_layouts(canonical, overlays, edits)
    assert len(docs) == 1
    assert docs[0]["size"] == "0x8"
    assert docs[0]["fields"][0]["name"] == "new"


def test_symbol_csv_and_pointer_signatures(tmp_path):
    csv = tmp_path / "x.csv"
    csv.write_text('address,kind,name,type,note\n006E4E44,global,g_main,EQ_MAIN *,rollout\n')
    assert symbols.read_list(csv)[0]["address"] == "006E4E44"
    class FakeTypes:
        def parse(self, text):
            return text.strip()
    assert symbols.parse_signature(FakeTypes(), "void __thiscall (EQ_MAIN *this, EQPlayer *player)") == (
        "void", "__thiscall", [("this", "EQ_MAIN *"), ("player", "EQPlayer *")])


def test_startup_failure_reports_without_replacing_archive(workspace, monkeypatch):
    write_header(workspace.repo("eqgfx"), "eqgfx/layout/x.h", "struct X { int x; };\n")
    archive = workspace.archive("eqgfx")
    archive.write_bytes(b"previous validated archive")
    def fail_start(ws):
        raise RuntimeError("JVM unavailable")
    monkeypatch.setattr(eqd, "start", fail_start)
    monkeypatch.setattr(sys, "argv", ["build_gdt.py", "eqgfx"])
    with pytest.raises(RuntimeError, match="JVM unavailable"):
        build.main()
    assert archive.read_bytes() == b"previous validated archive"
    report = json.loads((workspace.reports / "eqgfx.gdt.json").read_text())
    assert not report["validated"]
    assert report["phase"] == "startup"
    assert report["problems"] == ["RuntimeError: JVM unavailable"]


def test_canonical_missing_type_fails_validation(monkeypatch):
    monkeypatch.setattr(build, "find_type", lambda dtm, name: None)
    doc = {"family": "Missing", "client": "eqgfx", "_source": "canonical.json"}
    counts, problems, omitted = build.check_canonical(None, [doc], "eqgfx")
    assert counts["layouts"] == 0
    assert omitted == ["Missing"]
    assert problems == ["canonical.json:Missing: canonical type omitted"]


def test_canonical_mismatch_checks_union_offsets_and_widths(monkeypatch):
    class DT:
        def getLength(self):
            return 8
    monkeypatch.setattr(build, "find_type", lambda dtm, name: DT())
    monkeypatch.setattr(build, "component", lambda dt, field: (4, DT()))
    doc = {"family": "X", "client": "eqgfx", "_source": "canonical.json", "size": "0x10",
           "fields": [{"name": "scalar", "offset": "0x0", "width": 4,
                       "union": [{"name": "array", "width": 1, "count": [2, 4]}]}]}
    counts, problems, omitted = build.check_canonical(None, [doc], "eqgfx")
    assert not omitted
    assert counts == {"layouts": 1, "sizes": 1, "offsets": 2, "widths": 2}
    assert len(problems) == 4  # size, two offsets, scalar width


def test_canonical_warn_mode_does_not_block():
    failures = ["canonical.json:X: size 0x8, canonical 0x10"]
    assert build.canonical_outcome("fail", failures) == (failures, [])
    assert build.canonical_outcome("warn", failures) == ([], failures)


def test_windows_sdk_rect_is_declared_when_used(tmp_path):
    write_header(tmp_path, "eqmain/layout/w.h", "struct W { RECT rect; };\n")
    out = tmp_path / "eqmain.h"
    headers.generate(tmp_path, "eqmain", out)
    text = out.read_text()
    assert "typedef struct tagRECT { long left; long top; long right; long bottom; } RECT;" in text
    assert text.index("tagRECT") < text.index("struct W {")
    assert "tagPOINT" not in text


def test_typedef_alias_of_forward_only_tag(tmp_path):
    write_header(tmp_path, "trilogy/layout/f.h", "typedef struct HitRoot HitRoot;\n"
                 "typedef struct OtherTag Other;\nstruct Dev { HitRoot *root; Other *other; };\n")
    out = tmp_path / "trilogy.h"
    headers.generate(tmp_path, "trilogy", out)
    text = out.read_text()
    assert "typedef struct HitRoot HitRoot;" in text
    assert "typedef struct OtherTag Other;" in text


def test_types_are_categorized_by_declaring_header(tmp_path):
    write_header(tmp_path, "trilogy/player.h", "typedef unsigned char u8;\n"
                 "typedef enum Mode { MODE_A } Mode;\nstruct Player { u8 x; RECT r; };\n")
    write_header(tmp_path, "trilogy/layout/item.h", "typedef struct Item_ { int x; } Item;\n")
    idx, _ = headers.generate(tmp_path, "trilogy", tmp_path / "trilogy.h")
    assert idx.categories["Player"] == "player.h"
    assert idx.categories["u8"] == "player.h"
    assert idx.categories["Mode"] == "player.h"
    assert idx.categories["Item"] == idx.categories["Item_"] == "item.h"
    assert idx.categories["RECT"] == idx.categories["tagRECT"] == "WinDef.h"
    write_header(tmp_path, "trilogy/layout/item.h", "#define ITEM_MAX 4\ntypedef struct Item_ { int x; } Item;\n")
    idx, _ = headers.generate(tmp_path, "trilogy", tmp_path / "trilogy.h")
    assert idx.categories["define_ITEM_MAX"] == "item.h"
