"""passes.py report on the synthetic repo (report only: the repo is not changed)."""
import pytest

import passes as ps
import registry as R
from conftest import write

THIRD = """\
#include "trilogy/eqaddr.h"
#define X EQ_GLOBAL(int, 0x006E0030)

// FUNCTION: TRILOGY 0x00405000
int Third_guess(void)
{
    return EQ_GLOBAL(int, 0x006E0020) + EQ_CDECL(int, 0x00401100, (int))(1) + EQ_CDECL(int, 0x00401500, (int))(2);
}
"""


def test_spelling_csv_calls(repo):
    reg = R.build(repo, "trilogy")
    sp = ps.pass_spelling(reg)
    assert [(r[1], r[3], r[4]) for r in sp] == [("src/EverQuest/other.cpp", "0x006e0020", "0x006E0020")]
    cs = ps.pass_csv(repo, reg, check=True)
    assert [(r[1], r[2], r[3]) for r in cs] == [("00401100", "FUN_00401100", "Accumulate_guess")]
    assert cs[0][6].startswith("class differs") and cs[0][6].endswith("ok")
    write(repo / "src/EverQuest/third.cpp", THIRD)
    rows, protos = ps.pass_calls(R.build(repo, "trilogy"))
    by_va = {r[4]: r for r in rows}
    assert set(by_va) == {"006E0020", "00401100", "00401500"}         # bodies only, not the #define line
    assert by_va["006E0020"][5] == "g_scale" and by_va["006E0020"][7] == "global"
    assert by_va["00401100"][5] == "" and by_va["00401500"][5] == ""  # FUN_ / no name: nothing to substitute


def test_opcodes(repo):
    write(repo / "include/trilogy/opcodes.h", "enum EQOpcode\n{\n    OP_Hello = 0x2001,\n    OP_Bye = 0x2002,\n};\n")
    write(repo / "src/EverQuest/EQHandleMsg.cpp", """\
void f(int opcode, int kind)
{
    switch (opcode)
    {
    case 0x2001: break;
    case 0x2002:
        switch (kind) { case 0x1: break; case 0x2: break; }
        break;
    }
}
""")
    rows = ps.pass_opcodes(repo)
    assert [(r[2], r[3], r[5]) for r in rows] == [("0x2001", "OP_Hello", "opcode"), ("0x2002", "OP_Bye", "opcode"),
                                                  ("0x0001", "", "kind"), ("0x0002", "", "kind")]


MIXED = """\
#include "trilogy/eqaddr.h"
extern "C" char eqd_fn_0x00401abc[];
// a comment EQ_GLOBAL(int, 0x006e00aa) stays as written
int Mixed_guess(void)
{
    const char *s = "EQ_GLOBAL(int, 0x006e00bb)";
    return EQ_GLOBAL(int, 0x006e0020) + EQ_CDECL(int, 0X00401abc, (int, char[0x1f]))(1)
        + EQ_METHOD(0, int, 0x00401aBc, (void *), 0)(0) + EQ_GLOBAL(int, 0x006E0030);
}
"""


def test_apply_spelling(repo):
    write(repo / "src/EverQuest/mixed.cpp", MIXED)
    regs = {c: R.build(repo, c) for c in R.CLIENT_IDS}
    edits = ps.spelling_edits(MIXED)
    assert [MIXED[s:e] for s, e, _, _ in edits] == ["0x00401abc", "0x006e0020", "0X00401abc", "0x00401aBc"]
    assert ps.spelling_edits("#define F 0x0040aaaa\n") == []        # not used as a VA: a plain number
    dry = ps.apply_spelling(repo, regs, dry_run=True)
    assert dry["src/EverQuest/mixed.cpp"] == 4
    assert (repo / "src/EverQuest/mixed.cpp").read_text(encoding="utf-8") == MIXED
    ps.apply_spelling(repo, regs)
    out = (repo / "src/EverQuest/mixed.cpp").read_text(encoding="utf-8")
    assert "eqd_fn_0x00401ABC[]" in out and "EQ_CDECL(int, 0x00401ABC, (int, char[0x1f]))" in out
    assert "EQ_METHOD(0, int, 0x00401ABC," in out and "EQ_GLOBAL(int, 0x006E0020)" in out
    assert "0x006e00aa" in out and "0x006e00bb" in out          # comment and string literal untouched
    assert len(out) == len(MIXED)
    regs = {c: R.build(repo, c) for c in R.CLIENT_IDS}
    assert not any(ps.pass_spelling(r) for r in regs.values())  # idempotent: nothing left
    assert ps.apply_spelling(repo, regs) == {}


DEFS = """#include "trilogy/eqaddr.h"
#define CALC_SIZE 0x00401cc8
#define NOT_A_VA 0x00401cc8
#ifdef EQD_MATCH
extern "C" void eqd_fn_0x00401abc(int *);
#endif
extern void __cdecl eqd_fn_0x00401cc8(void);
void Thunk_guess(int *p)
{
    EQ_FUNC(void, __fastcall, 0x00401abc, (int *))(p);
    EQ_CDECL(int, CALC_SIZE, (int))(NOT_A_VA);
}
#define RESOLVE(slot, name) \\
    EQ_GLOBAL(int, slot) = (int)(name)
#define RESOLVE2(s) RESOLVE(s, 0x00500abc)
void Resolve_guess(void)
{
    RESOLVE(0x006e50a8, 0x00575eec);
    RESOLVE2(0x006e50ac);
}
"""


def test_apply_spelling_defines_func_extern(repo):
    write(repo / "src/EverQuest/defs.cpp", DEFS)
    write(repo / "src/EverQuest/user.cpp", "void u(void) { EQ_METHOD(0, void, 0x00401ABC, (void *), 0)(); }\n")
    done = ps.apply_spelling(repo)
    out = (repo / "src/EverQuest/defs.cpp").read_text(encoding="utf-8")
    assert done["src/EverQuest/defs.cpp"] == 6
    assert "RESOLVE(0x006E50A8, 0x00575eec);" in out and "RESOLVE2(0x006E50AC);" in out
    assert "RESOLVE(s, 0x00500abc)" in out                                      # not a VA position
    assert "#define CALC_SIZE 0x00401CC8" in out and "#define NOT_A_VA 0x00401cc8" in out
    assert "EQ_FUNC(void, __fastcall, 0x00401ABC, (int *))" in out
    assert "eqd_fn_0x00401abc" not in out and "#ifdef EQD_MATCH" not in out   # clashing extern dropped
    assert "extern void __cdecl eqd_fn_0x00401CC8(void);" in out             # a (void) one is kept
    assert ps.apply_spelling(repo) == {}


def test_spelling_report_does_not_learn_eqaddr_macros(repo):
    """Registry.files has eqaddr.h, whose `#define EQ_CDECL(ret, va, params) EQ_FUNC(...)` made spelling_scan
    treat EQ_CDECL as a VA-forwarding macro, so every lower-case EQ_CDECL VA came out twice and the report died
    with 'overlapping VA tokens'."""
    write(repo / "src/EverQuest/low.cpp", '#include "trilogy/eqaddr.h"\n'
          "int Low_guess(void)\n{\n    return EQ_CDECL(int, 0x004012ab, (int))(1);\n}\n")
    reg = R.build(repo, "trilogy")
    assert "include/trilogy/eqaddr.h" in reg.files
    _, _, fm = ps.spelling_scan(reg.files.values())
    assert "EQ_CDECL" not in fm
    rows = [r for r in ps.pass_spelling(reg) if r[1] == "src/EverQuest/low.cpp"]
    assert [(r[3], r[4]) for r in rows] == [("0x004012ab", "0x004012AB")]
    # and the edit list itself never repeats one token, whatever the macro table says
    text = reg.files["src/EverQuest/low.cpp"]
    assert len(ps.spelling_edits(text, fmacros={"EQ_CDECL": {1}})) == 1


CALLS_H = """\
#ifndef TRILOGY_CALLS_H
#define TRILOGY_CALLS_H
#include "trilogy/eqaddr.h"
#define StrCopy_guess EQ_CDECL(char *, 0x005219C0, (char *, const char *))
#define g_count_guess (EQ_GLOBAL(int, 0x006E0100))
#define g_count2_guess EQ_GLOBAL(int, 0x006E0100)
#define WidgetCount_guess (EQ_METHOD(1, int, 0x00401800, (int), dst))
#ifdef EQD_MATCH
#define g_cond_guess EQ_GLOBAL(int, 0x006E0104)
#endif
#endif
"""

CALLS = """\
// calls.cpp
#include "trilogy/calls.h"

int Early_guess(void)
{
    return EQ_GLOBAL(int, 0x006E0200);
}

#define g_late_guess EQ_GLOBAL(int, 0x006E0200)

// FUNCTION: TRILOGY 0x00403000
int Calls_guess(char *dst)
{
    /* EQ_CDECL(char *, 0x005219C0, (char *, const char *)) in a comment */
    const char *s = "EQ_GLOBAL(int, 0x006E0100)";
    EQ_CDECL(char *, 0x005219C0, (char*,const char*))(dst, s);
    EQ_CDECL(char *, 0x005219c0, (char *, const char *))(dst, s);
    EQ_CDECL(int, 0x005219C0, (char *, const char *))(dst, s);
    EQ_METHOD(1, int, 0x00401800, (int), dst)(2);
    return EQ_GLOBAL(int, 0x006E0100) + EQ_GLOBAL(int, 0x006E0104) + EQ_GLOBAL(int, 0x006E0200);
}
"""


def test_apply_calls(repo):
    write(repo / "include/trilogy/calls.h", CALLS_H)
    (repo / "src/EverQuest").mkdir(parents=True, exist_ok=True)
    (repo / "src/EverQuest/calls.cpp").write_bytes(CALLS.replace("\n", "\r\n").encode())
    rows, edits = ps.plan_calls(repo)
    mine = [(r[2], r[4], r[5], r[6]) for r in rows if r[1] == "src/EverQuest/calls.cpp"]
    assert mine == [(6, "006E0200", "", "hidden"),                       # used before its #define
                    (16, "005219C0", "StrCopy_guess", "apply"),          # same tokens, other spacing
                    (17, "005219C0", "", "none"),                        # other VA spelling: other symbol
                    (18, "005219C0", "", "none"),                        # other prototype
                    (19, "00401800", "", "none"),                        # (EQ_METHOD(..)): not the same tokens
                    (20, "006E0100", "g_count2_guess", "apply"),         # exact tokens, not the (EQ_GLOBAL(..))
                    (20, "006E0104", "", "none"),                        # its #define is under #ifdef
                    (20, "006E0200", "g_late_guess", "apply")]
    before = (repo / "src/EverQuest/calls.cpp").read_bytes()
    assert ps.write_edits(repo, edits, dry_run=True) == {"src/EverQuest/calls.cpp": 3}
    assert (repo / "src/EverQuest/calls.cpp").read_bytes() == before
    assert ps.main(["--repo", str(repo), "--apply", "calls"]) == 0
    out = (repo / "src/EverQuest/calls.cpp").read_bytes().decode()
    assert out.count("\r\n") == before.count(b"\r\n") and "\n" not in out.replace("\r\n", "")   # CRLF kept
    assert "    StrCopy_guess(dst, s);\r\n" in out
    assert "return g_count2_guess + EQ_GLOBAL(int, 0x006E0104) + g_late_guess;" in out
    assert "    EQ_METHOD(1, int, 0x00401800, (int), dst)(2);\r\n" in out
    assert "/* EQ_CDECL(char *, 0x005219C0, (char *, const char *)) in a comment */" in out
    assert '"EQ_GLOBAL(int, 0x006E0100)"' in out
    assert "return EQ_GLOBAL(int, 0x006E0200);\r\n" in out
    assert ps.plan_calls(repo)[1] == {}                                  # idempotent


DEFS_H = """\
#ifndef TRILOGY_DEFS_H
#define TRILOGY_DEFS_H
#include "trilogy/eqaddr.h"
#define g_alloc_guess EQ_CDECL(void *, 0x00401500, (unsigned int))
#define T3dAlloc_guess EQ_CDECL(void *, 0x00401500, (unsigned int))
#define AllocSized_guess EQ_CDECL(void *, 0x00401500, (int))
#define g_hooked_guess EQ_GLOBAL(int, 0x006E0300)
#define g_hooked2_guess EQ_GLOBAL(int, 0x006E0300)
#endif
"""

USER1 = """\
#include "trilogy/defs.h"
int User1_guess(void)
{
    return (int)g_alloc_guess(4) + (int)T3dAlloc_guess(8) + g_hooked_guess + g_hooked2_guess;
}
"""

USER2 = """\
#include "trilogy/defs.h"
int User2_guess(void)
{
    return (int)T3dAlloc_guess(1) + (int)T3dAlloc_guess(2) + g_hooked_guess;
}
"""

LOCAL = """\
#include "trilogy/eqaddr.h"
#define LocalAlloc_guess EQ_CDECL(void *, 0x00401500, (unsigned int))
#define FREE_THING_guess EQ_CDECL(void, 0x00401600, (void *))
int Local_guess(void)
{
#ifdef LocalAlloc_guess
    void *p = LocalAlloc_guess(2);
#endif
    FREE_THING_guess(p);
    return 0;
}
"""

LOCAL2 = """\
#include "trilogy/defs.h"
#define FreeThing_guess EQ_CDECL(void, 0x00401600, (void *))
#define STR(x) #x
int Local2_guess(void *p)
{
    FreeThing_guess(p);
    FreeThing_guess(p);
    return (int)g_alloc_guess(1);
}
"""


def test_apply_defines(repo):
    write(repo / "include/trilogy/defs.h", DEFS_H)
    write(repo / "src/EverQuest/user1.cpp", USER1)
    (repo / "src/EverQuest/user2.cpp").write_bytes(USER2.replace("\n", "\r\n").encode())
    (repo / "src/EverQuest/local.cpp").write_bytes(LOCAL.encode())             # an LF file
    write(repo / "src/EverQuest/local2.cpp", LOCAL2)
    write(repo / "hooks/decomp/hook.c", "int h(void) { return g_hooked2_guess; }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    # 0x00401500 (unsigned int): T3dAlloc_guess is used most -> kept; g_alloc_guess rewritten everywhere -> dropped;
    # LocalAlloc_guess lives in a .cpp nobody includes, the kept name is not in that TU: renamed there
    assert by["T3dAlloc_guess"][6] == "kept"
    assert by["g_alloc_guess"][2:7] == ("T3dAlloc_guess", 2, 2, 0, "define dropped")
    assert by["LocalAlloc_guess"][6] == "renamed in 1 files; no other #define"
    assert by["AllocSized_guess"][6].startswith("differs")            # another prototype: not merged
    assert by["g_hooked_guess"][6] == "kept"
    assert by["g_hooked2_guess"][6] == "define kept: name used outside src/ and include/"
    # 0x00401600: FREE_THING_guess (1 use) vs FreeThing_guess (2 uses): both .cpp-local
    assert by["FreeThing_guess"][6] == "kept" and by["FREE_THING_guess"][6].startswith("renamed in 1 files")
    crlf = (repo / "src/EverQuest/user2.cpp").read_bytes()
    ps.write_edits(repo, edits)
    h = (repo / "include/trilogy/defs.h").read_text(encoding="utf-8")
    assert "g_alloc_guess" not in h and "#define T3dAlloc_guess" in h and "#define AllocSized_guess" in h
    assert "#define g_hooked2_guess" in h
    u1 = (repo / "src/EverQuest/user1.cpp").read_text(encoding="utf-8")
    assert "(int)T3dAlloc_guess(4) + (int)T3dAlloc_guess(8) + g_hooked_guess + g_hooked_guess;" in u1
    assert (repo / "src/EverQuest/user2.cpp").read_bytes() == crlf         # nothing to change there
    loc = (repo / "src/EverQuest/local.cpp").read_bytes().decode()
    assert "\r" not in loc and loc.count("\n") == LOCAL.count("\n")             # LF stays LF
    assert "#define T3dAlloc_guess EQ_CDECL(void *, 0x00401500, (unsigned int))" in loc
    assert "#ifdef T3dAlloc_guess\n    void *p = T3dAlloc_guess(2);" in loc
    assert "#define FreeThing_guess EQ_CDECL(void, 0x00401600, (void *))" in loc and "FreeThing_guess(p);" in loc
    assert "LocalAlloc_guess" not in loc and "FREE_THING_guess" not in loc
    l2 = (repo / "src/EverQuest/local2.cpp").read_text(encoding="utf-8")
    assert "return (int)T3dAlloc_guess(1);" in l2
    assert ps.plan_defines(repo)[1] == {}                                  # nothing left to merge


def test_defines_crlf_and_dropped_line(repo):
    p = repo / "include/trilogy/defs.h"
    p.write_bytes(DEFS_H.replace("\n", "\r\n").encode())
    (repo / "src/EverQuest/user1.cpp").write_bytes(USER1.replace("\n", "\r\n").encode())
    rows, edits = ps.plan_defines(repo)
    dropped = sum(1 for r in rows if r[6] == "define dropped")
    assert dropped >= 1
    ps.write_edits(repo, edits)
    h = p.read_bytes()
    assert b"g_alloc_guess" not in h and h.count(b"\r\n") == DEFS_H.count("\n") - dropped
    assert b"\n" not in h.replace(b"\r\n", b"")
    u1 = (repo / "src/EverQuest/user1.cpp").read_bytes()
    assert u1.count(b"\r\n") == USER1.count("\n") and b"(int)T3dAlloc_guess(4)" in u1


def test_defines_blocked_uses_keep_the_define(repo):
    write(repo / "include/trilogy/a.h", '#include "trilogy/eqaddr.h"\n#define g_a_guess EQ_GLOBAL(int, 0x006E0400)\n')
    write(repo / "include/trilogy/b.h", '#include "trilogy/eqaddr.h"\n#define g_b_guess EQ_GLOBAL(int, 0x006E0400)\n')
    # ua.cpp sees both (rewritten); ub.cpp sees only b.h (blocked): g_b_guess keeps its #define
    write(repo / "src/EverQuest/ua.cpp", '#include "trilogy/a.h"\n#include "trilogy/b.h"\n'
          "int ua(void) { return g_a_guess + g_b_guess + g_a_guess; }\n")
    write(repo / "src/EverQuest/ub.cpp", '#include "trilogy/b.h"\nint ub(void) { return g_b_guess; }\n')
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["g_a_guess"][6] == "kept"
    assert by["g_b_guess"][3:7] == (2, 1, 1, "define kept: 1 uses not rewritable")
    ua = (repo / "src/EverQuest/ua.cpp").read_bytes().decode()
    pos = ua.index("g_b_guess")
    assert edits == {"src/EverQuest/ua.cpp": [(pos, pos + len("g_b_guess"), "g_a_guess")]}


def test_report_writes_apply_tables(repo, tmp_path):
    write(repo / "include/trilogy/calls.h", CALLS_H)
    write(repo / "src/EverQuest/calls.cpp", CALLS)
    out = tmp_path / "rep"
    before = {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()}
    assert ps.main(["--repo", str(repo), "--out", str(out)]) == 0
    assert {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()} == before   # report mode writes nothing
    md = (out / "passes.md").read_text(encoding="utf-8")
    assert "(c) apply:" in md and "(e) apply (" in md
    assert (out / "c_apply.tsv").read_text(encoding="utf-8").count("\tapply\t") == 3


def test_defines_alias_that_is_a_function_name_elsewhere(repo):
    """(found by preprocessing a scratch copy): rdp tx.c had `#define setLinkedListProperties_guess
    EQ_CDECL(...)` while list.c DEFINES that function; list.c sees the kept name's header, so the first draft
    rewrote the function definition. A word is an alias use only where the alias is provably a macro."""
    write(repo / "include/trilogy/l.h", '#include "trilogy/eqaddr.h"\n'
          "#define SetPropsByAddr_guess EQ_CDECL(void, 0x00401700, (int))\n")
    # (a declaration in a header would block: a TU may #define the alias before including it; see next test)
    write(repo / "src/Network/x/tx.c", '#include "trilogy/l.h"\n'
          "#define SetProps_guess EQ_CDECL(void, 0x00401700, (int))\n"
          "void tx(void) { SetProps_guess(1); SetProps_guess(2); }\n")
    write(repo / "src/Network/x/list.c", '#include "trilogy/l.h"\nvoid SetProps_guess(int x) { }\n')
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["SetPropsByAddr_guess"][6] == "kept"                 # in a header, and not a function's name
    # review #3: the uses are rewritten, but the define stays: should a TU see the alias some way the
    # include graph misses, a use left behind would silently bind to the function
    assert by["SetProps_guess"][3:7] == (2, 2, 0, "define kept: also a non-macro identifier (1 places)")
    assert "src/Network/x/list.c" not in edits and "include/trilogy/l.h" not in edits
    ps.write_edits(repo, edits)
    tx = (repo / "src/Network/x/tx.c").read_text(encoding="utf-8")
    assert "#define SetProps_guess EQ_CDECL" in tx and tx.count("SetPropsByAddr_guess(") == 2
    assert "SetProps_guess(1)" not in tx
    assert "void SetProps_guess(int x) { }" in (repo / "src/Network/x/list.c").read_text(encoding="utf-8")


def test_defines_header_occurrence_blocks(repo):
    """A header's text can follow the alias's #define in whatever TU includes it: an occurrence there that the
    header cannot prove to be the macro keeps the alias (and its use) as it is."""
    write(repo / "include/trilogy/l.h", '#include "trilogy/eqaddr.h"\n'
          "#define SetPropsByAddr_guess EQ_CDECL(void, 0x00401700, (int))\n")
    write(repo / "include/trilogy/m.h", "static void m(void) { SetProps_guess(3); }\n")
    write(repo / "src/Network/x/tx.c", '#include "trilogy/l.h"\n'
          "#define SetProps_guess EQ_CDECL(void, 0x00401700, (int))\n#include \"trilogy/m.h\"\n"
          "void tx(void) { SetProps_guess(1); }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["SetProps_guess"][3:7] == (2, 1, 1, "define kept: 1 uses not rewritable")
    assert "include/trilogy/m.h" not in edits
    assert all(new for _, _, new in edits["src/Network/x/tx.c"])  # its #define stays


@pytest.mark.parametrize("include", ["#include <trilogy/l2.h>", '#include "l2.h"'])
def test_defines_unmodelled_include_blocks(repo, include):
    """review #3: list.c could see the alias through an include the quoted-include graph does not follow
    (an angle include of a repo header via /I; a quoted one that only MSVC's parent-folder search finds): its
    use is not provably 'not the macro', so it blocks and the alias keeps its #define."""
    write(repo / "include/trilogy/l.h", '#include "trilogy/eqaddr.h"\n'
          "#define SetPropsByAddr_guess EQ_CDECL(void, 0x00401700, (int))\n")
    write(repo / "include/trilogy/l2.h", "/* nothing */\n")
    write(repo / "src/Network/x/tx.c", '#include "trilogy/l.h"\n'
          "#define SetProps_guess EQ_CDECL(void, 0x00401700, (int))\n"
          "void tx(void) { SetProps_guess(1); SetProps_guess(2); }\n")
    write(repo / "src/Network/x/list.c", include + "\nvoid lst(void) { SetProps_guess(3); }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["SetProps_guess"][3:7] == (3, 2, 1, "define kept: 1 uses not rewritable")
    assert "src/Network/x/list.c" not in edits
    assert all(new for _, _, new in edits["src/Network/x/tx.c"])


MLC_H = """\
#include "trilogy/eqaddr.h"
#define g_keep_guess EQ_GLOBAL(int, 0x006E0500)
#define g_alias1_guess EQ_GLOBAL(int, 0x006E0500) /* a note
   that goes on */
#define g_keep2_guess EQ_GLOBAL(int, 0x006E0504)
#define g_alias2_guess EQ_GLOBAL(int, 0x006E0504) // a note \\

#define g_keep3_guess EQ_GLOBAL(int, 0x006E0508)
/* a note
 */ #define g_alias3_guess EQ_GLOBAL(int, 0x006E0508)
"""


def test_defines_comment_across_lines_keeps_define(repo):
    """review #2: dropping a #define line whose comment goes on past it (/* */ over a line break, `//` ending
    in a backslash) or that starts inside one would leave a fragment (` that goes on */`) or eat a `*/`."""
    write(repo / "include/trilogy/mlc.h", MLC_H)
    write(repo / "src/EverQuest/mlc.cpp", '#include "trilogy/mlc.h"\nint mlc(void) { return g_keep_guess + g_keep_guess'
          " + g_alias1_guess + g_keep2_guess + g_keep2_guess + g_alias2_guess + g_keep3_guess + g_keep3_guess"
          " + g_alias3_guess; }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    for n in ("g_alias1_guess", "g_alias2_guess", "g_alias3_guess"):
        assert by[n][3:7] == (1, 1, 0, "define kept: multi-line or defined outside src/ and include/"), n
    assert "include/trilogy/mlc.h" not in edits
    ps.write_edits(repo, edits)
    assert (repo / "include/trilogy/mlc.h").read_text(encoding="utf-8") == MLC_H
    src = (repo / "src/EverQuest/mlc.cpp").read_text(encoding="utf-8")
    assert "alias" not in src


def test_defines_cpp_pseudo_guard_is_conditional(repo):
    """review #9: `#ifndef X` / `#define X` at the top of a .cpp is an include guard only when its #endif
    closes the file; here it wraps a real define, which is conditional and so not merged."""
    write(repo / "src/EverQuest/pg.cpp", '#ifndef PG_ONCE\n#define PG_ONCE\n#include "trilogy/eqaddr.h"\n'
          "#define g_one_guess EQ_GLOBAL(int, 0x006E0600)\n#endif\n"
          "#define g_two_guess EQ_GLOBAL(int, 0x006E0600)\n"
          "int pg(void) { return g_one_guess + g_two_guess + g_two_guess; }\n")
    write(repo / "include/trilogy/real.h", '#ifndef REAL_H\n#define REAL_H\n#include "trilogy/eqaddr.h"\n'
          "#define g_r1_guess EQ_GLOBAL(int, 0x006E0610)\n#define g_r2_guess EQ_GLOBAL(int, 0x006E0610)\n#endif\n")
    write(repo / "src/EverQuest/real.cpp", '#include "trilogy/real.h"\nint r(void) { return g_r1_guess + g_r1_guess'
          " + g_r2_guess; }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["g_one_guess"][6].startswith("unstable")
    assert "src/EverQuest/pg.cpp" not in edits
    assert by["g_r1_guess"][6] == "kept" and by["g_r2_guess"][6] == "define dropped"    # a real guard still is one


def test_pick_prefers_registry_then_unstamped_names():
    """review #4: the kept name is chosen by meaning before use count."""
    uses = {"PTR_GetThreadPriority_100400e8": 2, "PTR_GetThreadPriority_guess": 1, "parseColorScale": 8,
            "EQGFX_EFFECT_ANGLE_WRAP_LIMIT": 2, "g_x_0x0052ABCD": 5, "g_x_guess": 1, "g_faded_guess": 0}
    import collections
    u = collections.Counter(uses)
    assert ps._pick(["PTR_GetThreadPriority_100400e8", "PTR_GetThreadPriority_guess"], u, set()) == \
        "PTR_GetThreadPriority_guess"                                              # no hex stamp, though used less
    assert ps._pick(["g_x_0x0052ABCD", "g_x_guess"], u, set()) == "g_x_guess"
    assert ps._pick(["g_faded_guess", "g_x_guess"], u, set()) == "g_x_guess"     # `faded` is no hex stamp
    assert ps._pick(["parseColorScale", "EQGFX_EFFECT_ANGLE_WRAP_LIMIT"], u, set(),
                    preferred={"EQGFX_EFFECT_ANGLE_WRAP_LIMIT"}) == "EQGFX_EFFECT_ANGLE_WRAP_LIMIT"
    assert ps._pick(["parseColorScale", "EQGFX_EFFECT_ANGLE_WRAP_LIMIT"], u, {"EQGFX_EFFECT_ANGLE_WRAP_LIMIT"},
                    preferred={"EQGFX_EFFECT_ANGLE_WRAP_LIMIT"}) == "parseColorScale"   # a function's name: never


def test_defines_keep_the_registry_name(repo):
    """A pooled constant: the name names/*.csv gives the VA is kept over a more used contextual one, and a
    .cpp-local alias is renamed to it."""
    write(repo / "names/eqgame.manual.csv", "address,name,confidence,source,note,from_address,file\n"
          "00402000,UseWidget,low,manual,\"a note, with a comma\",,\n"
          "006E0700,g_half_float_const,certain,manual,,,\n")
    write(repo / "include/trilogy/k.h", '#include "trilogy/eqaddr.h"\n'
          "#define parseColorScale EQ_GLOBAL(float, 0x006E0700)\n"
          "#define g_half_float_const EQ_GLOBAL(float, 0x006E0700)\n")
    write(repo / "src/EverQuest/k1.cpp", '#include "trilogy/k.h"\n'
          "float k1(void) { return parseColorScale + parseColorScale + parseColorScale + g_half_float_const; }\n")
    write(repo / "src/EverQuest/k2.cpp", '#include "trilogy/eqaddr.h"\n'
          "#define ANGLE_WRAP_LIMIT EQ_GLOBAL(float, 0x006E0700)\nfloat k2(void) { return ANGLE_WRAP_LIMIT; }\n")
    rows, edits = ps.plan_defines(repo)
    by = {r[1]: r for r in rows}
    assert by["g_half_float_const"][6] == "kept"
    assert by["parseColorScale"][2:7] == ("g_half_float_const", 3, 3, 0, "define dropped")
    assert by["ANGLE_WRAP_LIMIT"][2] == "g_half_float_const" and by["ANGLE_WRAP_LIMIT"][6].startswith("renamed in 1")


def test_report_counts_defines_to_the_fixed_point(repo, tmp_path):
    """review #5: two aliases of one kept name in one .cpp. Round 1 renames the first there (bringing the kept
    name's #define in) and blocks the second; round 2 rewrites the second. The report counts both rounds, as
    --apply defines writes them, and does not call the second alias's uses blocked."""
    write(repo / "src/EverQuest/k.cpp", '#include "trilogy/eqaddr.h"\n#define K_guess EQ_GLOBAL(int, 0x006E0800)\n'
          "#define K_short_guess EQ_GLOBAL(short, 0x006E0800)\n"         # keeps the VA in the table in round 2
          "int k(void) { return K_guess + K_guess + K_guess + K_guess; }\n")
    write(repo / "src/EverQuest/f.cpp", '#include "trilogy/eqaddr.h"\n#define A1_guess EQ_GLOBAL(int, 0x006E0800)\n'
          "#define A2_guess EQ_GLOBAL(int, 0x006E0800)\nint f(void) { return A1_guess + A2_guess + A2_guess; }\n")
    one = {r[1]: r for r in ps.plan_defines(repo)[0]}
    assert one["A2_guess"][3:7] == (2, 0, 2, "define kept: 2 uses not rewritable")          # first round only
    rows, edits, rounds = ps.report_defines(repo)
    by = {r[1]: r for r in rows}
    assert rounds == 2
    assert by["K_guess"][6] == "kept"                                  # not round 2's 'differs'
    assert by["K_short_guess"][6].startswith("differs")
    assert by["A1_guess"][2:7] == ("K_guess", 2, 2, 0, "renamed in 1 files; no other #define")
    assert by["A2_guess"][2:7] == ("K_guess", 2, 2, 0, "define dropped")
    out = tmp_path / "rep"
    before = {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()}
    assert ps.main(["--repo", str(repo), "--out", str(out)]) == 0
    assert {p: p.read_bytes() for p in repo.rglob("*") if p.is_file()} == before   # still writes nothing
    md = (out / "passes.md").read_text(encoding="utf-8")
    assert "(e) apply (2 rounds to the fixed point" in md and "4 uses rewritten, 0 still blocked" in md
    assert f"({sum(edits.values())} edits in {len(edits)} files)" in md
    assert ps.apply_pass(repo, "defines") == edits                                   # what --apply writes
    f = (repo / "src/EverQuest/f.cpp").read_text(encoding="utf-8")
    assert "A1_guess" not in f and "A2_guess" not in f and "return K_guess + K_guess + K_guess;" in f
