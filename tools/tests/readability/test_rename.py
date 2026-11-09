"""rename.py on the synthetic repo: every place a name lives, the refusals, locals/labels, macros."""
import csv
import json

import pytest

import registry as R
import rename as rn


def apply(repo, client, old, new, **kw):
    plan = rn.plan_rename(repo, client, old, new, **kw)
    rn.write_plan(repo, plan)
    return plan


def text(repo, rel):
    return (repo / rel).read_text(encoding="utf-8")


def test_function_rename_everywhere(repo):
    plan = apply(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    w = text(repo, "src/EverQuest/widget.cpp")
    assert "int Accumulate_guess(int param_1)" in w and "FUN_00401100" not in w   # definition + comment
    o = text(repo, "src/EverQuest/other.cpp")
    assert "#define WidgetTotal EQ_CDECL" in o                 # not derived from the old name: kept
    q = [json.loads(l) for l in text(repo, "queue/queue.jsonl").splitlines()]
    assert [x["name"] for x in q if x["address"] == "00401100"] == ["Accumulate_guess"]
    f = json.loads(text(repo, "tests/unicorn/EverQuest/widget/FUN_00401100.json"))
    assert f["function"] == "Accumulate_guess"
    c = json.loads(text(repo, "tests/unicorn/EverQuest/other/UseWidget.json"))
    assert c["callees"]["0x00401100"]["name"] == "Accumulate_guess"            # the caller's callee spec
    rows = list(csv.DictReader(open(repo / "names/eqgame.manual.csv", encoding="utf-8")))
    assert {"address": "00401100", "name": "Accumulate", "confidence": "low"}.items() <= rows[-1].items()
    assert rows[0]["note"] == "a note, with a comma"           # other rows untouched
    assert R.build(repo, "trilogy").entries[0x00401100].canonical == "Accumulate_guess"
    assert plan.journal()[0] == {"client": "trilogy", "va": "00401100", "kind": "fn",
                                 "old": "FUN_00401100", "new": "Accumulate_guess"}


def test_member_rename_touches_header_and_callers(repo):
    apply(repo, "trilogy", "0x00401000", "Sum_guess")
    assert "int Sum_guess(int extra);" in text(repo, "include/trilogy/widget.h")
    assert "int Widget::Sum_guess(int extra)" in text(repo, "src/EverQuest/widget.cpp")
    assert "w->Sum_guess(1)" in text(repo, "src/EverQuest/other.cpp")
    assert "->Sum_guess(param_1)" in text(repo, "src/EverQuest/widget.cpp")


def test_derived_macros_follow_in_their_style():
    assert rn.macro_style_new("EQGFX_FUN_100070BC_GUESS", "FUN_100070bc", "PickTest_guess") == "EQGFX_PICK_TEST_GUESS"
    assert rn.macro_style_new("EQGFX_PICK_TEST_GUESS", "PickTest_guess", "FUN_100070bc") == "EQGFX_FUN_100070BC_GUESS"
    assert rn.macro_style_new("t3dAllocateMemory_call", "t3dAllocateMemory", "t3dAllocate") == "t3dAllocate_call"
    assert rn.macro_style_new("WRAP_ROWS_GUESS", "WrapRows_guess", "WrapLines_guess") == "WRAP_LINES_GUESS"
    assert rn.macro_style_new("EQGFX_ALLOCATE_AI", "t3dAllocateMemory", "t3dAllocate") is None


def test_call_macro_named_after_function_is_renamed(repo):
    p = repo / "src/EverQuest/other.cpp"
    p.write_text(text(repo, "src/EverQuest/other.cpp").replace("WidgetTotal", "FUN_00401100_call"), encoding="utf-8")
    apply(repo, "trilogy", "0x00401100", "Accumulate_guess")
    o = text(repo, "src/EverQuest/other.cpp")
    assert "#define Accumulate_call EQ_CDECL(int, 0x00401100" in o and "Accumulate_call(2)" in o


def test_refuses_macro_shadowing_a_definition(repo):
    with pytest.raises(rn.Refused, match="#define name"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "WidgetTotal")
    with pytest.raises(rn.Refused, match="#define name"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "g_widget_guess")   # a header's macro


def test_refuses_existing_identifier_and_ambiguity(repo):
    with pytest.raises(rn.Refused, match="already"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "UseWidget_guess")
    with pytest.raises(rn.Refused, match="already occurs"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "count_ref")
    p = repo / "src/EverQuest/other.cpp"
    p.write_text(text(repo, "src/EverQuest/other.cpp") + "\n// FUNCTION: TRILOGY 0x00403000\nint FUN_00401100b(void)"
                 " { return 0; }\n// FUNCTION: TRILOGY 0x00403100\nint Widget2_Total_guess(void) { return 1; }\n",
                 encoding="utf-8")
    with pytest.raises(rn.Refused, match="not an identifier"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "9bad")
    with pytest.raises(rn.Refused, match="keep the class"):
        rn.plan_rename(repo, "trilogy", "0x00401000", "Other::Sum")


def test_refuses_new_define_clash(repo):
    p = repo / "src/EverQuest/other.cpp"
    p.write_text(text(repo, "src/EverQuest/other.cpp")
                 + "#define g_scale_new EQ_GLOBAL(int, 0x006E0099)\n", encoding="utf-8")
    with pytest.raises(rn.Refused, match="#define name"):
        rn.plan_rename(repo, "trilogy", "g_scale_factor", "g_scale_new")
    assert rn.define_clashes("#define A 1\n#define A 2\n") == ["A"]
    assert rn.define_clashes("#define A 1\n#undef A\n#define A 2\n#define B 1\n#define B  1\n") == []


def test_global_rename_and_unify(repo):
    plan = apply(repo, "trilogy", "g_scale_factor", "g_scale", allow_existing=False)
    assert plan.kind == "global"
    o = text(repo, "src/EverQuest/other.cpp")
    assert "#define g_scale EQ_GLOBAL(int, 0x006e0020)" in o and "+ g_scale;" in o
    assert "#define g_scale EQ_GLOBAL(int, 0x006E0020)" in text(repo, "src/EverQuest/widget.cpp")   # untouched
    assert set(R.build(repo, "trilogy").entries[0x006E0020].define_names) == {"g_scale"}


def test_local_and_label_rename_stay_inside_the_function(repo):
    apply(repo, "trilogy", "param_1", "amount", in_va="0x00401100")
    w = text(repo, "src/EverQuest/widget.cpp")
    assert "int FUN_00401100(int amount)" in w and "Total_guess(amount)" in w
    apply(repo, "trilogy", "LAB_00401130", "done", in_va="0x00401100")
    w = text(repo, "src/EverQuest/widget.cpp")
    assert "goto done;" in w and "\ndone:\n" in w
    apply(repo, "trilogy", "extra", "bonus", in_va="0x00401000")
    assert "int Total_guess(int extra);" in text(repo, "include/trilogy/widget.h")       # the prototype keeps it


def test_local_used_by_a_macro_is_refused(repo):
    with pytest.raises(rn.Refused, match="inside #define BUMP"):
        rn.plan_rename(repo, "trilogy", "count_ref", "total", in_va="0x00401100")


def test_local_shadowing_refused_but_other_functions_locals_fine(repo):
    with pytest.raises(rn.Refused, match="#define name"):
        rn.plan_rename(repo, "trilogy", "local_8", "g_scale", in_va="0x00401100")
    with pytest.raises(rn.Refused, match="file-scope name"):
        rn.plan_rename(repo, "trilogy", "local_8", "Widget", in_va="0x00401100")
    p = repo / "src/EverQuest/widget.cpp"
    p.write_text(text(repo, "src/EverQuest/widget.cpp").replace("return this->count + extra",
                                                                "int sum = 0;\n    return sum + this->count + extra"),
                 encoding="utf-8")
    rn.plan_rename(repo, "trilogy", "local_8", "sum", in_va="0x00401100")    # `sum` lives in another function
    rn.plan_rename(repo, "trilogy", "local_8", "count", in_va="0x00401100")  # a struct field's name is fine
    assert rn.file_scope_idents("struct S { int count; };\nenum E { RED = 1, BLUE };\nint g(int x);\n") \
        >= {"S", "E", "RED", "BLUE", "g"}
    assert "count" not in rn.file_scope_idents("struct S { int count; };")


def test_comments_only_get_code_like_names(repo):
    apply(repo, "trilogy", "param_1", "rows", in_va="0x00401100")
    p = repo / "src/EverQuest/widget.cpp"
    p.write_text(text(repo, "src/EverQuest/widget.cpp").replace("// Sums count", "// Sums rows of param count"),
                 encoding="utf-8")
    apply(repo, "trilogy", "rows", "param_1", in_va="0x00401100")
    assert "// Sums rows of param count" in text(repo, "src/EverQuest/widget.cpp")      # prose untouched
    assert rn.code_like("param_1") and rn.code_like("WrapText") and not rn.code_like("rows")


def test_strings_never_touched(repo):
    p = repo / "src/EverQuest/widget.cpp"
    p.write_text(text(repo, "src/EverQuest/widget.cpp") + 'const char *s = "FUN_00401100";\n', encoding="utf-8")
    apply(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    assert '"FUN_00401100"' in text(repo, "src/EverQuest/widget.cpp")


def test_dry_run_cli_writes_nothing(repo, capsys):
    before = text(repo, "src/EverQuest/widget.cpp")
    rc = rn.main(["--repo", str(repo), "-c", "trilogy", "FUN_00401100", "Accumulate_guess", "--dry-run"])
    assert rc == 0 and text(repo, "src/EverQuest/widget.cpp") == before
    assert "widget.cpp" in capsys.readouterr().out
    assert rn.main(["--repo", str(repo), "-c", "trilogy", "FUN_00401100", "WidgetTotal"]) == 2


def test_crlf_preserved(repo):
    p = repo / "src/EverQuest/widget.cpp"
    p.write_bytes(p.read_bytes().replace(b"\n", b"\r\n"))
    apply(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    b = p.read_bytes()
    assert b.count(b"\r\n") == b.count(b"\n") and b"Accumulate_guess" in b


def test_map_locals_reuse_registry(repo, tmp_path):
    """--map with several local renames in one function: the registry is kept between them (only the text of
    the changed file is refreshed), and each rename sees the previous one."""
    src = repo / "src/EverQuest/loc.cpp"
    src.write_text('#include "trilogy/eqaddr.h"\n\n// FUNCTION: TRILOGY 0x00407000\n'
                   'int Loc_guess(int param_1, int param_2)\n{\n    int iVar1;\n    iVar1 = param_1 + param_2;\n'
                   '    return iVar1;\n}\n', encoding="utf-8")
    m = tmp_path / "m.json"
    m.write_text(json.dumps([{"old": "param_1", "new": "a", "in": "0x00407000"},
                             {"old": "param_2", "new": "b", "in": "0x00407000"},
                             {"old": "iVar1", "new": "a", "in": "0x00407000"},          # refused: a exists now
                             {"old": "iVar1", "new": "sum", "in": "0x00407000"}]), encoding="utf-8")
    j = tmp_path / "j.jsonl"
    assert rn.main(["--repo", str(repo), "-c", "trilogy", "--map", str(m), "--journal", str(j), "-q"]) == 2
    t = src.read_text(encoding="utf-8")
    assert "int Loc_guess(int a, int b)" in t and "sum = a + b;" in t and "return sum;" in t
    assert len(j.read_text(encoding="utf-8").splitlines()) == 3


def test_local_may_share_a_field_name(repo):
    src = repo / "src/EverQuest/fld.cpp"
    src.write_text('#include "trilogy/eqaddr.h"\nstruct Rec_guess\n{\n    int count;\n};\nint g_total;\n\n'
                   '// FUNCTION: TRILOGY 0x00407100\nint Fld_guess(Rec_guess *param_1)\n{\n    return param_1->count;\n}\n',
                   encoding="utf-8")
    plan = rn.plan_rename(repo, "trilogy", "param_1", "count", in_va="0x00407100")
    assert "int Fld_guess(Rec_guess *count)" in plan.texts["src/EverQuest/fld.cpp"]
    with pytest.raises(rn.Refused):
        rn.plan_rename(repo, "trilogy", "param_1", "g_total", in_va="0x00407100")   # a file-scope global


def test_local_named_like_a_macro_parameter(repo):
    """A function-like macro's parameter is a bound name: a local may take it. A free name in a macro body
    (the macro uses the local) still refuses."""
    src = repo / "src/EverQuest/mac.cpp"
    src.write_text('#include "trilogy/eqaddr.h"\n#define GET_A(character, n) ((character) + (n))\n'
                   '#define USES_ROLL() (roll + 1)\n\n// FUNCTION: TRILOGY 0x00407200\n'
                   'int Mac_guess(int param_1)\n{\n    int roll = 0;\n    return GET_A(param_1, 2) + USES_ROLL();\n}\n',
                   encoding="utf-8")
    plan = rn.plan_rename(repo, "trilogy", "param_1", "character", in_va="0x00407200")
    assert "int Mac_guess(int character)" in plan.texts["src/EverQuest/mac.cpp"]
    with pytest.raises(rn.Refused):
        rn.plan_rename(repo, "trilogy", "roll", "dice", in_va="0x00407200")


def test_local_rename_leaves_same_named_members_alone(repo):
    # wave 1: parameter `gender` and the member `->gender`: only the parameter is renamed
    p = repo / "src/EverQuest/widget.cpp"
    t = p.read_text(encoding="utf-8").replace(
        "    count_ref = param_1;\n", "    count_ref = param_1;\n    local_8 = param_1 + this->param_1 + Widget::param_1 + gw.param_1;\n")
    p.write_text(t, encoding="utf-8")
    apply(repo, "trilogy", "param_1", "amount", in_va="0x00401100")
    w = text(repo, "src/EverQuest/widget.cpp")
    assert "local_8 = amount + this->param_1 + Widget::param_1 + gw.param_1;" in w
    assert "int FUN_00401100(int amount)" in w


def test_stale_fixture_function_follows_the_rename(repo):
    # a fixture of the VA kept an older name ("Stale_guess") that no longer matches the source
    p = repo / "tests/unicorn/EverQuest/widget/FUN_00401100.json"
    p.write_text(p.read_text(encoding="utf-8").replace('"function": "FUN_00401100"', '"function": "Stale_guess"'),
                 encoding="utf-8")
    apply(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    assert json.loads(text(repo, "tests/unicorn/EverQuest/widget/FUN_00401100.json"))["function"] == "Accumulate_guess"


def _set_aliases(repo, rel, aliases, crlf=False):
    p = repo / rel
    d = json.loads(p.read_text(encoding="utf-8"))
    d["rewrite_aliases"] = aliases
    s = json.dumps(d, indent=2) + "\n"
    p.write_bytes((s.replace("\n", "\r\n") if crlf else s).encode())


def test_alias_keys_follow_a_function_rename(repo):
    # 10 rewrite_aliases keys still said FUN_<va> after renames (pytest failures, fixed by hand c1b659b5)
    rel = "tests/unicorn/EverQuest/other/UseWidget.json"
    _set_aliases(repo, rel, {"_strlen": "0x005223d0", "_FUN_00401100": "0x00401100",
                             "?FUN_00401100@@YAHH@Z": "0x00409999", "FUN_00401100": "0x00401100",
                             "_FUN_00401100x": "0x00401101", "?FUN_00401100@Widget@@QAEHH@Z": "0x00401100"},
                 crlf=True)
    plan = apply(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    raw = (repo / rel).read_bytes()
    assert raw.count(b"\r\n") == raw.count(b"\n")                                 # CRLF kept
    al = json.loads(raw)["rewrite_aliases"]
    assert list(al.items()) == [("_strlen", "0x005223d0"), ("_Accumulate_guess", "0x00401100"),
                                ("?FUN_00401100@@YAHH@Z", "0x00409999"),          # review #8: its value is another VA
                                ("Accumulate_guess", "0x00401100"),
                                ("_FUN_00401100x", "0x00401101"),                 # another name
                                ("?FUN_00401100@Widget@@QAEHH@Z", "0x00401100")]  # a method: not this function
    assert sum(1 for e in plan.edits[rel] if e[4] == "alias") == 2


def test_alias_keys_of_a_method_and_stale_keys(repo):
    rel = "tests/unicorn/EverQuest/other/UseWidget.json"
    _set_aliases(repo, rel, {"?Total_guess@Widget@@QAEHH@Z": "0x00401000",
                             "?FUN_00401000@Widget@@QAEHXZ": "0x00401000",         # stale: an earlier rename
                             "?Total_guess@Other@@QAEHH@Z": "0x00405000",          # another class
                             "?Total_guess@@YAHH@Z": "0x00405004",                 # a free function
                             "_Total_guess": "0x00405008"})
    apply(repo, "trilogy", "Widget::Total_guess", "Sum_guess")
    al = json.loads(text(repo, rel))["rewrite_aliases"]
    assert list(al) == ["?Sum_guess@Widget@@QAEHH@Z", "?Sum_guess@Widget@@QAEHXZ", "?Total_guess@Other@@QAEHH@Z",
                        "?Total_guess@@YAHH@Z", "_Total_guess"]


def test_alias_keys_that_would_merge_are_refused(repo):
    rel = "tests/unicorn/EverQuest/other/UseWidget.json"
    _set_aliases(repo, rel, {"?Total_guess@Widget@@QAEHH@Z": "0x00401000",
                             "?FUN_00401000@Widget@@QAEHH@Z": "0x00401000"})
    with pytest.raises(rn.Refused, match="would all become"):
        rn.plan_rename(repo, "trilogy", "Widget::Total_guess", "Sum_guess")


def test_alias_key_clash_is_refused(repo):
    rel = "tests/unicorn/EverQuest/other/UseWidget.json"
    _set_aliases(repo, rel, {"_FUN_00401100": "0x00401100", "_Accumulate_guess": "0x00401100"})
    before = (repo / rel).read_bytes()
    with pytest.raises(rn.Refused, match="rewrite_aliases has both"):
        rn.plan_rename(repo, "trilogy", "FUN_00401100", "Accumulate_guess")
    assert (repo / rel).read_bytes() == before


def test_alias_new_key_forms():
    k = rn.alias_new_key
    assert k("_Old@8", 0x401000, "Old", "New") == "_New@8"                      # __stdcall
    assert k("@Old@8", 0x401000, "Old", "New") == "@New@8"                      # __fastcall
    assert k("__Old", 0x401000, "_Old", "New") == "_New"                        # a name with a leading _
    assert k("?Old@In@Out@@QAEXXZ", 0x401000, "Out::In::Old", "Out::In::New") == "?New@In@Out@@QAEXXZ"
    assert k("?Old@Out@@QAEXXZ", 0x401000, "Out::In::Old", "Out::In::New") is None
    assert k("fun_00401000", 0x401000, "Named_guess", "New_guess") == "New_guess"
    assert k("FUN_00401001", 0x401000, "Named_guess", "New_guess") is None
    assert k("_Old@x", 0x401000, "Old", "New") is None


def test_alias_new_key_value_flat_and_crt():
    """review #8: keys match by name AND, when the value is a VA, by that VA; a method's flattened free form
    `Class_Method` follows the rename; a CRT name's key is renamed only when its value is the renamed VA."""
    k = rn.alias_new_key
    assert k("_Old", 0x401000, "Old", "New", "0x00401000") == "_New"
    assert k("_Old", 0x401000, "Old", "New", "0x00401004") is None                # same name, another VA
    assert k("_Old", 0x401000, "Old", "New", "Old") == "_New"                      # not a VA: the name decides
    assert k("_Widget_Total_guess", 0x401000, "Widget::Total_guess", "Widget::Sum_guess") == "_Widget_Sum_guess"
    assert (k("Widget_Total_guess@4", 0x401000, "Widget::Total_guess", "Widget::Sum_guess", "0x401000")
            == "Widget_Sum_guess@4")
    assert (k("?Widget_Total_guess@@YAHH@Z", 0x401000, "Widget::Total_guess", "Widget::Sum_guess")
            == "?Widget_Sum_guess@@YAHH@Z")
    assert k("_Widget_Total_guess", 0x401000, "Widget::Total_guess", "Widget::Sum_guess", "0x401008") is None
    assert k("_Total_guess", 0x401000, "Widget::Total_guess", "Widget::Sum_guess") is None   # still not a method
    assert k("__EH_prolog", 0x52189C, "_EH_prolog", "EhProlog_guess", "0x0052189c") == "_EhProlog_guess"
    raw = '{"rewrite_aliases": {"_strlen": "0x005223d0"}}'
    assert rn._alias_edits(raw, json.loads(raw), 0x5223D0, "strlen", "StrLen_guess", "f.json")    # VA matches
    raw = '{"rewrite_aliases": {"_strlen": "0x005223d4"}}'
    assert rn._alias_edits(raw, json.loads(raw), 0x5223D0, "strlen", "StrLen_guess", "f.json") == []
    raw = '{"rewrite_aliases": {"_strlen": "strlen"}}'
    with pytest.raises(rn.Refused, match="C runtime name"):
        rn._alias_edits(raw, json.loads(raw), 0x5223D0, "strlen", "StrLen_guess", "f.json")
