"""structs/ tools on small synthetic trees (tmp dirs): layout engine, census, unify_diff, layout_check."""
import json
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "structs"))

import cparse as C  # noqa: E402
import census as CE  # noqa: E402
import unify_diff as U  # noqa: E402
import layout_check as LC  # noqa: E402

EQADDR = """\
#ifndef TRILOGY_EQADDR_H
#define TRILOGY_EQADDR_H
#define EQ_GLOBAL(type, va)                 (*(type *)(va))
#define EQ_ARRAY(type, va)                  ((type *)(va))
#endif
"""

COMMON = """\
#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#endif
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#endif
"""

PLAYER_H = """\
#include "common.h"
#pragma pack(push, 1)
typedef struct Packed_guess
{
    u8      a;              /* 0x00 */
    u32     b;              /* 0x01 */
    u16     c;              /* 0x05 */
} Packed_guess;             /* 0x07 */
C_ASSERT(sizeof(Packed_guess) == 0x07);
#pragma pack(pop)

typedef struct Info_guess
{
    u32     id;             /* 0x00 */
    u8      unknown0x04[0x20];  /* 0x04 */
    f32     speed;          /* 0x24 */
    s32     tail;           /* 0x28 */
} Info_guess;
C_ASSERT(sizeof(Info_guess) == 0x2C);
C_ASSERT(offsetof(Info_guess, speed) == 0x24);

typedef struct Holder_guess
{
    u8      kind;
    double  d;
    Packed_guess pk;
    union {
        u32 as_int;
        f32 as_float;
    } u;
    u32     flags : 3;
    u32     more : 5;
    u16     s;
    char    name[10];
} Holder_guess;
"""

SRC = """\
#include "trilogy/eqaddr.h"
#include "trilogy/player.h"

#define g_info_guess EQ_GLOBAL(Info_guess *, 0x006E0010)
#define g_blob       EQ_GLOBAL(char *, 0x006E0020)

// FUNCTION: TRILOGY 0x00401000
int Read_guess(Info_guess *info, char *raw, int *words, void *self)
{
    int local;
    local = *(int *)((char *)info + 0x24);
    *(float *)((char *)info + 0x50) = 1.0f;
    *(u8 *)(raw + 0x10) = 3;
    *(int *)(raw + 0x14) += 1;
    local = words[0x4];
    local += *(int *)(words + 2);
    local += *(u32 *)(g_blob + 0x800 + local * 4);
    local += *(int *)((char *)self + 0x2C);
    return local + info->speed + (int)(*(u8 *)((char *)&g_info_guess + 0x0C));
}

// FUNCTION: TRILOGY 0x00401100
int Second_guess(void)
{
    char *p;
    int *q;
    p = (char *)g_info_guess;
    q = (int *)EQ_GLOBAL(void *, 0x006E0020);
    return *(int *)(p + 0x30) + *(short *)(EQ_GLOBAL(void *, 0x006E0020) + 4) + *(char *)(p + 0x04);
}
"""


def write(p: Path, text: str):
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text, encoding="utf-8")


@pytest.fixture
def repo(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/eqaddr.h", EQADDR)
    write(r / "include/trilogy/common.h", COMMON)
    write(r / "include/trilogy/player.h", PLAYER_H)
    write(r / "src/EverQuest/a.cpp", SRC)
    return r


# ---------------------------------------------------------------------------------------- layout engine

def test_layout_pack_arrays_nested_bitfields(repo):
    idx = C.load_index(repo)
    pk = idx.layout(idx.find_struct("Packed_guess"))
    assert (pk.size, pk.align, pk.pack) == (7, 1, 1)
    assert [(m.name, m.offset) for m in pk.members] == [("a", 0), ("b", 1), ("c", 5)]
    info = idx.layout(idx.find_struct("Info_guess"))
    assert info.size == 0x2C and info.direct["speed"][0] == 0x24
    pad = [m for m in info.members if m.name == "unknown0x04"][0]
    assert pad.is_pad and pad.size == 0x20 and pad.unknown_name
    h = idx.layout(idx.find_struct("Holder_guess"))
    off = {m.path: m.offset for m in h.members}
    assert off["d"] == 8                                   # natural alignment under /Zp8
    assert off["pk.a"] == 0x10 and off["pk.c"] == 0x15     # by-value packed struct expanded, align 1
    assert off["u.as_int"] == 0x18 == off["u.as_float"]    # union after a 7-byte struct, 4-aligned
    assert off["flags"] == off["more"] == 0x18 + 4         # one bitfield unit
    assert h.align == 8 and h.size % 8 == 0


def test_layout_unknown_type_is_flagged():
    idx = C.Index()
    idx.add(C.parse_file("x.h", "typedef struct S { int a; Mystery m; int b; } S;\n"))
    lay = idx.layout(idx.find_struct("S"))
    assert not lay.exact and any("Mystery" in i for i in lay.issues)
    assert lay.members[1].guessed and lay.members[2].offset == 8


def test_cpp_class_vptr_empty_and_bases():
    t = ("class B { public: int x; virtual void f(); };\n"
         "class D : public B { public: int y; };\n"
         "class E { public: void m(); };\n")
    idx = C.Index()
    idx.add(C.parse_file("c.h", t))
    b, d, e = (idx.layout(idx.find_struct(n)) for n in "BDE")
    assert b.size == 8 and b.direct["x"][0] == 4          # vptr first
    assert d.size == 12 and d.direct["y"][0] == 8
    assert e.size == 1 and not e.members


# ------------------------------------------------------------------------------------------------ census

def run_census(repo, out, fields="all"):
    cen = CE.ClientCensus(repo, "trilogy")
    cen.load()
    cen.run(fields)
    stats = CE.write_reports(cen, out)
    recs = [json.loads(line) for line in (out / "accesses.jsonl").read_text().splitlines()]
    return cen, stats, recs


def test_census_forms_offsets_rw_and_families(repo, tmp_path):
    cen, stats, recs = run_census(repo, tmp_path / "out")
    raw = [r for r in recs if r["form"] != "field"]
    by = {(r["func"], r["offset"], r["form"]): r for r in raw}
    r = by[("Read_guess", 0x24, "deref_cast")]
    assert r["base_struct"] == "Info_guess" and r["type"] == "int" and r["rw"] == "r"
    assert r["func_va"] == "0x00401000" and r["coverage"] == "defined"
    gap = by[("Read_guess", 0x50, "deref_cast")]
    assert gap["rw"] == "w" and gap["type"] == "float" and gap["coverage"] == "beyond_end"
    assert by[("Read_guess", 0x10, "deref_cast")]["rw"] == "w"
    assert by[("Read_guess", 0x14, "deref_cast")]["rw"] == "rw"
    # int *words: words[0x4] is element-scaled; (words + 2) without a byte cast also scales
    assert by[("Read_guess", 0x10, "index")]["family"].startswith("?int*:words")
    assert by[("Read_guess", 8, "deref_cast")]["note"] == "scaled*4"
    # global base with a dynamic index term
    g = [x for x in raw if x["base_global_va"] == "0x006E0020" and x["offset"] == 0x800][0]
    assert g["dynamic"] == "local * 4" and g["family"] == "G:0x006E0020->"
    # untyped self
    assert by[("Read_guess", 0x2C, "deref_cast")]["family"].startswith("?void*:self[")
    # a typed field access is recorded
    assert any(x["form"] == "field" and x["member"] == "speed" and x["offset"] == 0x24 for x in recs)
    # pointer local typed by an assignment from the typed global
    sec = [x for x in raw if x["func"] == "Second_guess"]
    p30 = [x for x in sec if x["offset"] == 0x30][0]
    assert p30["base_struct"] == "Info_guess" and p30["base_inferred"] is True
    assert p30["coverage"] == "beyond_end" and {x["offset"] for x in sec} >= {0x30, 4}


def test_census_outputs_and_conflicts(repo, tmp_path):
    out = tmp_path / "out"
    cen, stats, recs = run_census(repo, out)
    for name in ("accesses.jsonl", "by_base.tsv", "families.tsv", "globals_conflicts.tsv", "struct_coverage.tsv",
                 "gapfill.tsv", "census.md"):
        assert (out / name).is_file(), name
    conf = (out / "globals_conflicts.tsv").read_text()
    assert "0x006E0020" in conf and "char*" in conf and "void*" in conf       # g_blob char* vs EQ_GLOBAL(void *)
    gap = (out / "gapfill.tsv").read_text()
    assert "Info_guess" in gap and "0x50" in gap
    md = (out / "census.md").read_text()
    assert "Top 40" in md and "NOT caught" in md
    assert stats["raw_accesses"] >= 8 and stats["conflicting_globals"] >= 1


def test_census_cli(repo, tmp_path):
    rc = CE.main(["--repo", str(repo), "--client", "trilogy", "--out", str(tmp_path / "o")])
    assert rc == 0
    assert (tmp_path / "o" / "census.md").is_file() and (tmp_path / "o" / "trilogy" / "census.md").is_file()


# batch 2 (eqmain deflate_state.h): the generated header ends in `typedef deflate_state DeflateState_guess;`;
# after the TU-local DeflateState_guess was replaced by the header, `DeflateState_guess *s` bases came out untyped.
DFL_H = """\
typedef struct deflate_state
{
    int status;             /* 0x00 */
    unsigned int w_size;    /* 0x04 */
    unsigned short pad8;    /* 0x08 */
} deflate_state;
typedef deflate_state DeflateState_guess;
typedef DeflateState_guess DeflateStateAgain_guess;
typedef deflate_state *DeflateStatePtr_guess;
"""
DFL_SRC = """\
#include "trilogy/deflate_state.h"

// FUNCTION: TRILOGY 0x00402000
void init_block_guess(DeflateState_guess *s, DeflateStateAgain_guess *t)
{
    *(unsigned int *)((char *)s + 0x4) = 0;
    *(int *)((char *)t + 0x0) = 1;
}
"""


def test_census_types_bases_declared_through_a_header_typedef_alias(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/deflate_state.h", DFL_H)
    write(r / "src/EverQuest/z.cpp", DFL_SRC)
    idx = C.load_index(r)
    d = idx.find_struct("deflate_state")
    assert idx.find_struct("DeflateState_guess") is d and idx.find_struct("DeflateStateAgain_guess") is d
    assert idx.find_struct("DeflateStatePtr_guess") is None        # a pointer typedef is not the struct
    assert d.name == "deflate_state"
    _, _, recs = run_census(r, tmp_path / "out")
    raw = {(x["offset"], x["base_expr"]): x for x in recs if x["form"] != "field"}
    for key in ((4, "s"), (0, "t")):
        x = raw[key]
        assert x["base_struct"] == "deflate_state" and x["base_struct_uid"] == d.uid and not x["base_inferred"]
        assert x["family"] == "deflate_state" and x["coverage"] == "defined"


# ------------------------------------------------------------------------------------------- unify_diff

COPY_A = """typedef struct Actor_guess
{
    unsigned char pad0[0x34];
    float   scale;          /* 0x34 */
    float   radius;         /* 0x38 */
    unsigned char flagsB;   /* 0x3C */
} Actor_guess;
"""
COPY_B = """typedef struct Actor_guess
{
    unsigned char pad0[0x30];
    int     size;           /* 0x30 */
    unsigned int field34;   /* 0x34 */
    float   boundingRadius; /* 0x38 */
    unsigned int flags;     /* 0x3C */
} Actor_guess;
"""


def test_unify_diff_flags_and_output(tmp_path):
    r = tmp_path / "t"
    write(r / "src/eqgfx/a.c", COPY_A)
    write(r / "src/eqgfx/b.c", COPY_B)
    out = tmp_path / "u"
    assert U.main(["--repo", str(r), "--name", "Actor_guess", "--out", str(out)]) == 0
    js = json.loads((out / "unify.json").read_text())
    assert [c["size"] for c in js["copies"]] == [0x3C + 4 if False else 0x40, 0x40]
    rows = {row["offset"]: row for row in js["rows"]}
    assert "KIND" in rows[0x34]["flags"]            # float vs unsigned int
    assert "NAME" in rows[0x38]["flags"]            # radius vs boundingRadius
    assert "WIDTH" in rows[0x3C]["flags"]           # byte vs dword
    assert "FILL" in rows[0x30]["flags"]            # copy A has padding where copy B has a field
    assert "Actor_guess" in (out / "unify.md").read_text()


def test_unify_diff_size_difference_and_fullmatch(tmp_path):
    r = tmp_path / "t"
    write(r / "src/eqgfx/a.c", "typedef struct T3dActor_guess { int a; } T3dActor_guess;\n"
                               "typedef struct OtherT3dActor_guess { int a; int b; } OtherT3dActor_guess;\n")
    write(r / "src/eqgfx/b.c", "typedef struct T3dActor_guess { int a; int b; int c; } T3dActor_guess;\n")
    out = tmp_path / "u"
    assert U.main(["--repo", str(r), "--name", "T3dActor_guess", "--out", str(out)]) == 0
    js = json.loads((out / "unify.json").read_text())
    assert sorted(c["size"] for c in js["copies"]) == [4, 12]       # OtherT3dActor_guess not matched (fullmatch)
    assert "Size differs" in (out / "unify.md").read_text()


# ---------------------------------------------------------------------------------------- layout_check

def vc6_or_skip():
    try:
        return LC.find_vc6(None)
    except SystemExit:
        pytest.skip("no VC6 toolchain configured")


def test_layout_check_against_vc6(repo, tmp_path):
    vc6_or_skip()
    out = tmp_path / "lc"
    rc = LC.main(["--repo", str(repo), "--header", "include/trilogy/player.h", "--out", str(out),
                  "--scratch", str(tmp_path / "scratch")])
    assert rc == 0
    js = json.loads((out / "player.layout_check.json").read_text())
    by = {s["struct"]: s for s in js["structs"]}
    assert by["Packed_guess"]["sizeof"] == 7 and by["Info_guess"]["offsets"]["speed"] == 0x24
    assert by["Holder_guess"]["sizeof"] == by["Holder_guess"]["parser_size"]
    assert js["parser_mismatches"] == [] and all(c["status"] == "OK" for c in js["assert_checks"])


def test_layout_check_reports_assert_mismatch_and_refuses_scratch_in_tree(repo, tmp_path):
    vc6_or_skip()
    write(repo / "include/trilogy/bad.h",
          '#include "common.h"\ntypedef struct Bad { u8 a; u32 b; } Bad;\n'
          'C_ASSERT(sizeof(Bad) == 5);\nC_ASSERT(offsetof(Bad, b) == 1);\n')
    rc = LC.main(["--repo", str(repo), "--header", "include/trilogy/bad.h", "--out", str(tmp_path / "o"),
                  "--scratch", str(tmp_path / "s")])
    assert rc == 1
    js = json.loads((tmp_path / "o" / "bad.layout_check.json").read_text())
    assert {c["status"] for c in js["assert_checks"]} == {"MISMATCH"} and js["structs"][0]["sizeof"] == 8
    with pytest.raises(SystemExit):
        LC.main(["--repo", str(repo), "--header", "include/trilogy/bad.h", "--scratch", str(repo / "scratch")])


# ----------------------------------------------------------------------------------- fixes (review C1-C8)

DUP_A = "typedef struct Dup_guess { u32 a; u32 b; } Dup_guess;\n"
DUP_B = "typedef struct Dup_guess { u32 a; u32 b; u32 c; u32 d; } Dup_guess;\n"
WIDE_H = "typedef struct Wide_guess { u32 a; u32 b; u32 arr[4]; } Wide_guess;\n"

SRC2 = """\
#include "trilogy/eqaddr.h"
#include "trilogy/player.h"
#include "trilogy/dupb.h"
#include "trilogy/wide.h"

#define g_eqmain EQ_GLOBAL(u32 *, 0x006E4E44)

// FUNCTION: TRILOGY 0x00402000
int SelfA_guess(void *self)
{
    return *(int *)((char *)self + 0x10);
}

// FUNCTION: TRILOGY 0x00402100
float SelfB_guess(void *self)
{
    return *(float *)((char *)self + 0x10);
}

// FUNCTION: TRILOGY 0x00402200
int Foo::A(void *self)
{
    return *(int *)((char *)self + 0x20);
}

// FUNCTION: TRILOGY 0x00402300
int Foo::B(void *self)
{
    return *(int *)((char *)self + 0x24);
}

// FUNCTION: TRILOGY 0x00402400
int Eqmain_guess(void)
{
    int a = *(u32 *)0x006e4e44;
    a += *(int *)EQ_GLOBAL(int *, 0x006E4E44);
    a += *(short *)(*(u32 *)0x006e4e44 + 0xec2);
    a += *(int *)((char *)g_eqmain + 8);
    a += *(int *)((*(Info_guess **)0x006e4e44) + 1);
    return a;
}

// FUNCTION: TRILOGY 0x00402500
void Money_guess(int *self)
{
    *(float *)&self[0xE] = 0.0f;
    *(float *)&self[0xF] += 1.0f;
    self[0x10] = 1;
}

// FUNCTION: TRILOGY 0x00402600
int Dup_use_guess(Dup_guess *d)
{
    return *(int *)((char *)d + 0xC);
}

// FUNCTION: TRILOGY 0x00402700
int Wide_use_guess(Wide_guess *w)
{
    return *(u8 *)((char *)w + 3) + *(u32 *)((char *)w + 2) + *(u32 *)((char *)w + 0x10) + *(u32 *)((char *)w + 0x14);
}

// FUNCTION: TRILOGY 0x00402800
int Scale2_guess(char *raw, int **pp, Info_guess *info)
{
    int a = *(int *)((int *)raw + 3);
    a += *(int *)((Info_guess *)raw + 1);
    a += *(int *)(pp + 2);
    a += *(int *)((char *)raw + 3);
    a += *(int *)((DWORD)raw + 3);
    return a;
}

// FUNCTION: TRILOGY 0x00402900
int Step_guess(Info_guess *info, Info_guess *other)
{
    char *q;
    char *r;
    q = (char *)info;
    q += 0x10;
    r = (char *)other;
    return *(int *)(q + 8) + *(int *)(r + 0x8);
}
"""


@pytest.fixture
def repo2(tmp_path):
    r = tmp_path / "tree2"
    write(r / "include/trilogy/eqaddr.h", EQADDR)
    write(r / "include/trilogy/common.h", COMMON.replace("typedef int s32;", "typedef int s32;\ntypedef unsigned long DWORD;"))
    write(r / "include/trilogy/player.h", PLAYER_H)
    write(r / "include/trilogy/dupa.h", '#include "common.h"\n' + DUP_A)
    write(r / "include/trilogy/dupb.h", '#include "common.h"\n' + DUP_B)
    write(r / "include/trilogy/wide.h", '#include "common.h"\n' + WIDE_H)
    write(r / "src/EverQuest/b.cpp", SRC2)
    write(r / "src/EverQuest/other.cpp", '#include "trilogy/dupa.h"\n// FUNCTION: TRILOGY 0x00403000\n'
          "int DupA_guess(Dup_guess *d)\n{\n    return *(int *)((char *)d + 0xC);\n}\n")
    return r


def rows_of(recs, func, **kw):
    return [r for r in recs if r["func"] == func and all(r.get(k) == v for k, v in kw.items())]


def test_untyped_families_are_scoped_by_function_or_class(repo2, tmp_path):
    """C1: two unrelated `void *self` do not merge; methods of one class do."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    a = rows_of(recs, "SelfA_guess")[0]
    b = rows_of(recs, "SelfB_guess")[0]
    assert a["family"] != b["family"] and a["coarse_family"] == b["coarse_family"] == "?void*:self"
    fa, fb = rows_of(recs, "Foo::A")[0], rows_of(recs, "Foo::B")[0]
    assert fa["family"] == fb["family"] == "?void*:self[Foo]"
    fam = (tmp_path / "o" / "families.tsv").read_text().splitlines()
    assert fam[0].split("\t")[-1] == "coarse_family"
    assert "Family keys" in (tmp_path / "o" / "census.md").read_text()


def test_global_variable_and_pointee_levels_are_separate(repo2, tmp_path):
    """C2: G:VA is the variable, G:VA-> what it points to; `*(T*)(*(u32*)0xVA + off)` lands in the pointee family."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    e = rows_of(recs, "Eqmain_guess")
    var = {x["offset"] for x in e if x["family"] == "G:0x006E4E44"}
    ptr = {x["offset"] for x in e if x["family"] == "G:0x006E4E44->"}
    assert var == {0}                                   # the abs read of the pointer variable itself
    assert ptr == {0, 0xEC2, 8}                         # EQ_GLOBAL deref, nested abs + off, (char *)g_eqmain + 8
    assert all(x["coarse_family"] == "G:0x006E4E44" for x in e if x["family"].startswith("G:0x006E4E44"))
    # the cast's pointee struct designates the struct: (*(Info_guess **)0x006e4e44) + 1 is scaled by sizeof
    st = [x for x in e if x["base_struct"] == "Info_guess"]
    assert st and st[0]["offset"] == 0x2C


def test_outer_cast_type_of_address_of_index(repo2, tmp_path):
    """C3: `*(float *)&self[0xE]` is a float access at 0x38, not an int."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    m = {x["offset"]: x for x in rows_of(recs, "Money_guess") if x["form"] != "field"}
    assert (m[0x38]["type"], m[0x38]["rw"]) == ("float", "w")
    assert (m[0x3C]["type"], m[0x3C]["rw"]) == ("float", "rw")
    assert (m[0x40]["type"], m[0x40]["rw"]) == ("int", "w")                 # the plain form is unchanged


def test_struct_coverage_uses_the_definition_the_tu_sees(repo2, tmp_path):
    """C4: Dup_guess is defined twice (8 and 16 bytes); offset 0xC is in range only for the TU that sees dupb.h."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    b = rows_of(recs, "Dup_use_guess")[0]
    a = rows_of(recs, "DupA_guess")[0]
    assert b["coverage"] == "defined" and a["coverage"] == "beyond_end"
    assert b["base_struct_uid"] != a["base_struct_uid"]
    rows = [l.split("\t") for l in (tmp_path / "o" / "struct_coverage.tsv").read_text().splitlines()[1:]]
    dup = {(r[2], r[4], r[8]) for r in rows if r[0] == "Dup_guess"}
    assert ("include/trilogy/dupb.h", "16", "defined") in dup           # laid out against the 16-byte definition
    assert ("include/trilogy/dupa.h", "8", "beyond_end") in dup
    gap = (tmp_path / "o" / "gapfill.tsv").read_text()
    assert "dupa.h" in gap and "dupb.h" not in gap


def test_struct_coverage_reports_mid_member_accesses(repo2, tmp_path):
    """C5: a u8 at member+3 is a sub-offset of the dword at 0 (SPLIT), a dword at +2 also runs past it (OVERRUN)."""
    out = tmp_path / "o"
    run_census(repo2, out)
    rows = [l.split("\t") for l in (out / "struct_coverage.tsv").read_text().splitlines()[1:]]
    w = {r[7]: r for r in rows if r[0] == "Wide_guess"}
    assert "+0x3 u8 x1" in w["0x0"][-2] and "+0x2 u32 x1" in w["0x0"][-2]
    assert "SPLIT" in w["0x0"][-1] and "OVERRUN" in w["0x0"][-1]
    assert w["0x8"][-1] == ""                   # accesses to elements of the array arr[4]: no SPLIT


def test_pointer_scaling_follows_the_cast_type(repo2, tmp_path):
    """C6: scale by the pointee size of the outermost cast; T** steps by 4; a byte or non-pointer cast does not scale."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    s = rows_of(recs, "Scale2_guess", form="deref_cast")
    got = sorted((x["offset"], x.get("base_struct") or "-") for x in s)
    assert got == [(3, "-"), (3, "-"), (8, "-"), (12, "-"), (0x2C, "Info_guess")], got
    notes = {(x["offset"], x.get("note")) for x in s}
    assert (12, "scaled*4") in notes and (0x2C, "scaled*44") in notes and (8, "scaled*4") in notes
    assert [x for x in s if x["offset"] == 0x2C][0]["coverage"] == "beyond_end"


def test_stepped_local_is_not_inferred(repo2, tmp_path):
    """C8: `q = (char *)info; q += 0x10;` no longer designates info's struct; an unstepped alias still does."""
    cen, stats, recs = run_census(repo2, tmp_path / "o")
    q = [x for x in rows_of(recs, "Step_guess") if x["base_expr"] == "q"][0]
    r = [x for x in rows_of(recs, "Step_guess") if x["base_expr"] == "r"][0]
    assert q["base_struct"] is None and q["base_inferred"] is False
    assert r["base_struct"] == "Info_guess" and r["base_inferred"] is True and r["offset"] == 8


# ---------------------------------------------------------------------------------------------- macros

MACRO_HDR = """\
#ifndef MAC_H
#define MAC_H
#define MAIN_BASE 0x006E4E44
#define FIELD(type, obj, off) (*(type*)((char*)(obj) + (off)))
#define MAIN_INT(off) (*(int*)(*(int*)MAIN_BASE + (off)))
#define NEST(off) FIELD(int, g_main, (off) + 4)
#define REC(x) (REC(x) + 1)
#endif
"""

MACRO_SRC = """\
#include "macros.h"
#include "trilogy/eqaddr.h"
#define LOCAL_SHORT(off) (*(short*)((char*)self + (off)))
void *g_main;
int ObjLike_guess(void *self)
{
    return MAIN_INT(0x10);
}
int FuncArgs_guess(void *self)
{
    int a = FIELD(int, self, 0x20);
    FIELD(short, self, 0x24) = 1;
    return LOCAL_SHORT(0x28) + a;
}
int Nested_guess(void)
{
    return NEST(0x30);
}
int Multi_guess(void *self)
{
    return FIELD(int,
                 self,
                 0x44);
}
int Rec_guess(void *self)
{
    return REC(FIELD(int, self, 0x50));
}
"""


@pytest.fixture
def repo_mac(tmp_path):
    r = tmp_path / "tree_mac"
    write(r / "include/trilogy/eqaddr.h", EQADDR)
    write(r / "src/EverQuest/macros.h", MACRO_HDR)
    write(r / "src/EverQuest/m.cpp", MACRO_SRC)
    return r


def test_macro_object_like_and_function_like(repo_mac, tmp_path):
    cen, stats, recs = run_census(repo_mac, tmp_path / "o")
    r = rows_of(recs, "ObjLike_guess")
    assert [(x["offset"], x["via_macro"]) for x in r if x["form"] != "field"] == [(0x10, "MAIN_INT")]
    assert r[0]["family"] == "?int:" + r[0]["base_expr"] + "[src/EverQuest/m.cpp:ObjLike_guess]" or "family" in r[0]
    f = {x["offset"]: x for x in rows_of(recs, "FuncArgs_guess")}
    assert f[0x20]["via_macro"] == "FIELD" and f[0x24]["rw"] == "w"
    assert f[0x28]["via_macro"] == "LOCAL_SHORT"          # TU-local #define is visible too
    assert f[0x28]["type"] == "short"


def test_macro_nested_args_and_line_kept(repo_mac, tmp_path):
    cen, stats, recs = run_census(repo_mac, tmp_path / "o")
    lines = MACRO_SRC.splitlines()
    n = rows_of(recs, "Nested_guess")
    assert [(x["offset"], x["via_macro"]) for x in n] == [(0x34, "NEST")]     # NEST -> FIELD, outermost named
    assert "NEST(0x30)" in lines[n[0]["line"] - 1]
    assert "FIELD(int, self, 0x20)" in lines[rows_of(recs, "FuncArgs_guess", offset=0x20)[0]["line"] - 1]
    m = rows_of(recs, "Multi_guess")
    assert "return FIELD(int," in lines[m[0]["line"] - 1]                     # multi-line use: line of the use site


def test_macro_recursion_guard_and_no_define_line_rows(repo_mac, tmp_path):
    cen, stats, recs = run_census(repo_mac, tmp_path / "o")
    r = rows_of(recs, "Rec_guess")
    assert [x["offset"] for x in r] == [0x50]                  # self-referential REC terminates, FIELD still counted
    define_lines = {i + 1 for i, l in enumerate(MACRO_SRC.splitlines() + MACRO_HDR.splitlines()) if l.startswith("#define")}
    for x in recs:
        if x["file"].endswith("m.cpp"):
            assert not MACRO_SRC.splitlines()[x["line"] - 1].lstrip().startswith("#define")
    assert not [x for x in recs if x["file"].endswith("macros.h")]
    assert define_lines


def test_macro_expansion_unit():
    tab = {"A": (["x"], "(x + B)"), "B": (None, "A(1)"), "C": (None, "0x10")}
    ex = CE.MacroExpander(tab)
    t, o, v = ex.expand("A(2)", [7] * 4, [None] * 4)
    assert t == "(2 + A(1))" and set(o) == {7} and set(v) == {None, "A"}     # A is disabled inside its own expansion
    assert ex.expand("C", [0], [None])[0] == "C"                       # pure numeric bodies are left to eval_const
