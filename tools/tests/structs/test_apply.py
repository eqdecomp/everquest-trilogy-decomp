"""structs/gen_layout_h.py and structs/propagate.py on small synthetic trees (S3 headers, S4a typed-base rewrites)."""
import json
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "structs"))

import cparse as C  # noqa: E402
import census as CE  # noqa: E402
import gen_layout_h as G  # noqa: E402
import propagate as P  # noqa: E402

COMMON = """\
#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#endif
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed long s32;
typedef unsigned long u32;
typedef float f32;
#endif
"""

# player.h style: hand-written, pack(1), CRLF in the work tree
ACTOR_H = """\
/* actor.h - hand written */
#ifndef ACTOR_H
#define ACTOR_H
#include "common.h"
#pragma pack(push, 1)
struct EQPlayer;
typedef struct Actor_guess
{
    void*   actor;              /* 0x000* t3d actor */
    u8      unknown0x004[0x44]; /* 0x004  */
    f32     floor_z;            /* 0x048* floor height */
    u8      unknown0x04C[0x2C]; /* 0x04C  */
    void*   attach_guess[2];    /* 0x078* */
    u8      unknown0x080[0x20]; /* 0x080  */
    s32     tail;               /* 0x0A0* */
} Actor_guess;                  /* size unknown (>= 0xA4) */
C_ASSERT(offsetof(Actor_guess, actor) == 0x000);
C_ASSERT(offsetof(Actor_guess, floor_z) == 0x048);
C_ASSERT(offsetof(Actor_guess, tail) == 0x0A0);

typedef struct EQPlayer
{
    u8      unknown0x00[0x70];
    Actor_guess* actor_info;    /* 0x70 */
} EQPlayer;
#pragma pack(pop)
#endif
"""

LAYOUT = {
    "family": "Actor_guess", "client": "trilogy", "kind": "struct", "header": "include/trilogy/actor.h",
    "size": None, "pack": 1,
    "fields": [
        {"offset": "0x000", "name": "actor", "type": "void*", "width": 4},
        {"offset": "0x048", "name": "floor_z", "type": "f32", "width": 4},
        {"offset": "0x050", "name": "follow_target_guess", "type": "char*", "width": 4, "comment": "follow target",
         "union": [{"name": "follow_player_guess", "type": "struct EQPlayer*"}]},
        {"offset": "0x054", "name": "heading_guess", "type": "float", "width": 4},
        {"offset": "0x058", "name": "flag_guess", "type": "u8", "width": 1},
        {"offset": "0x078", "name": "attach_guess", "type": "void*", "count": 2, "width": 4},
        {"offset": "0x08C", "name": "count_guess", "type": "int", "width": 4},
        {"offset": "0x0A0", "name": "tail", "type": "s32", "width": 4},
    ],
    "keep_cast_sites": [],
}

SRC = """\
#include "trilogy/common.h"
#include "trilogy/actor.h"

#define EQ_GLOBAL(type, va) (*(type *)(va))

// FUNCTION: TRILOGY 0x00401000
void Follow_guess(EQPlayer* self, int i)
{
    char* info;
    char* t;
    u32* slot;
    t = *(char**)((char*)self->actor_info + 0x50);
    *(float*)((char*)self->actor_info + 0x54) = 1.0f;
    *(EQPlayer**)((int)self->actor_info + 0x50) = self;
    *(u32*)((char*)self->actor_info + 0x50) = 0;
    *(signed char*)((char*)self->actor_info + 0x58) = 1;
    *(u8*)((u32)self->actor_info + 0x58) = 2;
    info = (char*)self->actor_info;
    *(float*)(info + 0x54) = 2.0f;
    *(void**)((char*)self->actor_info + 0x7C) = 0;
    *(int*)((char*)self->actor_info + 0x60) = 0;
    *(int*)((char*)self->actor_info + 0x8C + i * 4) = 0;
    slot = (u32*)((char*)self->actor_info + 0x8C);
    t = (char*)((char*)self->actor_info + 0x50);
    *(float*)((char*)self->actor_info
        + 0x48) = 0.0f;
    *(volatile int*)((char*)self->actor_info + 0x8C) = 1;
    *(int*)((char*)self->actor_info + 0x8C) = 3;
    *(s32*)((char*)self->actor_info + 0xA0) = 4;
    *(long*)((char*)self->actor_info + 0xA0) = 5;
}

// FUNCTION: TRILOGY 0x00401100
__declspec(naked) void Naked_guess(EQPlayer* self)
{
    *(float*)((char*)self->actor_info + 0x54) = 1.0f;
}

// FUNCTION: TRILOGY 0x00428686
void HeaderSensitive_guess(EQPlayer* self)
{
    *(float*)((char*)self->actor_info + 0x54) = 1.0f;
}

// FUNCTION: TRILOGY 0x00401200
void NameSensitive_guess(EQPlayer* self)
{
    *(float*)((char*)self->actor_info + 0x54) = 1.0f;
}

// FUNCTION: TRILOGY 0x00401300
void Addr_guess(EQPlayer* self, Actor_guess* a)
{
    int* n;
    n = (int*)((char*)a + 0x8C);
    *(float*)((char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x54) = 0.0f;
}
"""


def write(p: Path, text: str, crlf=False):
    p.parent.mkdir(parents=True, exist_ok=True)
    if crlf:
        text = text.replace("\n", "\r\n")
    with open(p, "w", encoding="utf-8", newline="") as fh:
        fh.write(text)


def rd(p: Path) -> str:
    with open(p, encoding="utf-8", newline="") as fh:
        return fh.read()


@pytest.fixture
def repo(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/common.h", COMMON, crlf=True)
    write(r / "include/trilogy/actor.h", ACTOR_H, crlf=True)
    write(r / "src/EverQuest/a.cpp", SRC, crlf=True)
    (tmp_path / "layout.json").write_text(json.dumps(LAYOUT), encoding="utf-8")
    return r


# ---------------------------------------------------------------------------------------- gen_layout_h: generate

def gen_doc(**kw):
    d = {"family": "Thing_guess", "client": "eqgfx", "kind": "struct", "header": "include/eqgfx/layout/thing.h",
         "size": "0x20", "pack": 8, "aliases": ["OldThing_guess"],
         "fields": [{"offset": "0x4", "name": "count_guess", "type": "int", "width": 4},
                    {"offset": "0x8", "name": "scale_guess", "type": "float", "width": 4,
                     "union": [{"name": "scale_bits", "type": "u32"}]},
                    {"offset": "0x10", "name": "name_guess", "type": "char", "count": 12, "width": 1},
                    {"offset": "0x1C", "name": "flag_guess", "type": "u8", "width": 1}]}
    d.update(kw)
    return G.parse_layout(d, "thing.json")


def test_generate_padding_union_asserts_lf(tmp_path):
    r = tmp_path / "t"
    write(r / "include/eqgfx/common.h", COMMON)
    doc = gen_doc()
    text = G.generate(doc, "\n", "thing.json")
    assert "#pragma pack" not in text                         # pack 8 = natural, no pragma
    assert "u8      unknown0x000[4];" in text and "unknown0x00C[4];" in text
    assert "unknown0x01D[3];" in text                          # tail padding up to size
    assert "    union\n    {\n        float   scale_guess;" in text and "u32     scale_bits;" in text
    assert "C_ASSERT(sizeof(Thing_guess) == 0x020);" in text
    for n, o in (("count_guess", "0x004"), ("scale_guess", "0x008"), ("name_guess", "0x010"), ("flag_guess", "0x01C")):
        assert f"C_ASSERT(offsetof(Thing_guess, {n}) == {o});" in text
    assert "typedef Thing_guess OldThing_guess;" in text and G.GEN_MARKER in text
    assert "\r\n" not in text
    lay = G.verify(text, doc.header, doc, r, 0x20)
    assert lay.size == 0x20 and lay.direct["scale_bits"][0] == 8


def test_generate_pack1_and_misaligned_rejected():
    fields = [{"offset": "0x0", "name": "kind_guess", "type": "u8", "width": 1},
              {"offset": "0x1", "name": "value_guess", "type": "u32", "width": 4}]
    with pytest.raises(G.LayoutError, match="misaligned"):
        G.generate(gen_doc(fields=fields, size="0x5", aliases=[]))
    text = G.generate(gen_doc(fields=fields, size="0x5", aliases=[], pack=1))
    assert "#pragma pack(push, 1)" in text and "#pragma pack(pop)" in text
    assert "C_ASSERT(offsetof(Thing_guess, value_guess) == 0x001);" in text


def test_union_member_wider_than_primary(tmp_path):
    r = tmp_path / "t"
    write(r / "include/eqgfx/common.h", COMMON)
    fields = [{"offset": "0x4", "name": "autosplit", "type": "u8", "width": 1,
               "union": [{"name": "autosplit_int_guess", "type": "int"}]},
              {"offset": "0xC", "name": "pair", "type": "s16", "count": 2, "width": 2,
               "union": [{"name": "pair_bits", "type": "u32", "count": None}]},
              {"offset": "0x10", "name": "tail_guess", "type": "u16", "width": 2}]
    doc = gen_doc(fields=fields, size="0x14", aliases=[])
    assert doc.fields[0].size == 4 and doc.fields[1].size == 4
    text = G.generate(doc, chr(10))
    assert "u8      unknown0x008[4];" in text and "unknown0x005" not in text   # padding resumes after the int
    lay = G.verify(text, doc.header, doc, r, 0x14)
    assert lay.direct["autosplit"][0] == lay.direct["autosplit_int_guess"][0] == 4
    assert lay.direct["pair_bits"][0] == 0xC and lay.direct["tail_guess"][0] == 0x10
    # a 2-aligned slot holding a u32 role is only placeable under pack 1 (character.h style)
    odd = [{"offset": "0xA", "name": "pair", "type": "s16", "count": 2, "width": 2,
            "union": [{"name": "pair_bits", "type": "u32", "count": None}]}]
    with pytest.raises(G.LayoutError, match="misaligned"):
        G.generate(gen_doc(fields=odd, size="0x10", aliases=[]))
    doc1 = gen_doc(fields=odd, size="0x10", aliases=[], pack=1)
    assert G.verify(G.generate(doc1, chr(10)), doc1.header, doc1, r, 0x10).direct["pair_bits"][0] == 0xA
    bad = fields[:1] + [{"offset": "0x6", "name": "x_guess", "type": "u8", "width": 1}]
    with pytest.raises(G.LayoutError, match="overlap"):
        gen_doc(fields=bad, size="0x14")


def test_generate_overlap_needs_union():
    fields = [{"offset": "0x0", "name": "a", "type": "u32", "width": 4},
              {"offset": "0x2", "name": "b", "type": "u16", "width": 2}]
    with pytest.raises(G.LayoutError, match="overlap"):
        gen_doc(fields=fields)


def test_generate_newline_follows_dir_and_never_clobbers(tmp_path, capsys):
    r = tmp_path / "t"
    write(r / "include/eqgfx/common.h", COMMON, crlf=True)
    write(r / "include/eqgfx/other.h", "#pragma once\n", crlf=True)
    lj = tmp_path / "thing.json"
    d = json.loads(json.dumps({"family": "Thing_guess", "header": "include/eqgfx/layout/thing.h", "size": "0x8",
                               "fields": [{"offset": "0x4", "name": "x_guess", "type": "int", "width": 4}]}))
    lj.write_text(json.dumps(d), encoding="utf-8")
    # the target dir (layout/) is empty: LF; put a CRLF header there to flip the convention
    write(r / "include/eqgfx/layout/peer.h", "/* x */\n", crlf=True)
    assert G.main([str(lj), "--repo", str(r)]) == 0
    out = rd(r / "include/eqgfx/layout/thing.h")
    assert out.count("\r\n") == out.count("\n") > 10
    # regenerating a generated header is fine; a hand-written one is refused
    assert G.main([str(lj), "--repo", str(r)]) == 0
    write(r / "include/eqgfx/layout/thing.h", "/* hand written */\n")
    assert G.main([str(lj), "--repo", str(r)]) == 1
    assert "not generated" in capsys.readouterr().err


# ---------------------------------------------------------------------------------------- gen_layout_h: extend

def test_extend_in_place_keeps_existing_bytes_and_crlf(repo, tmp_path):
    before = rd(repo / "include/trilogy/actor.h")
    assert G.main([str(tmp_path / "layout.json"), "--repo", str(repo), "--extend"]) == 0
    after = rd(repo / "include/trilogy/actor.h")
    assert after.count("\r\n") == after.count("\n")             # CRLF kept everywhere
    old_lines = before.split("\r\n")
    new_lines = after.split("\r\n")
    replaced = {"    u8      unknown0x04C[0x2C]; /* 0x04C  */", "    u8      unknown0x080[0x20]; /* 0x080  */"}
    for ln in old_lines:                                      # every untouched line is still there, in order
        if ln not in replaced:
            assert ln in new_lines
    assert [x for x in new_lines if x in old_lines] == [x for x in old_lines if x not in replaced]
    assert "    u8      unknown0x04C[4];    /* 0x04C  */" in new_lines
    assert "    union" in new_lines and "        char*   follow_target_guess; /* 0x050* follow target */" in new_lines
    assert "    u8      flag_guess;         /* 0x058* */" in new_lines
    assert "    u8      unknown0x059[0x1F]; /* 0x059  */" in new_lines
    assert "    int     count_guess;        /* 0x08C* */" in new_lines
    i = new_lines.index("C_ASSERT(offsetof(Actor_guess, tail) == 0x0A0);")
    assert new_lines[i + 1:i + 5] == ["C_ASSERT(offsetof(Actor_guess, follow_target_guess) == 0x050);",
                                      "C_ASSERT(offsetof(Actor_guess, heading_guess) == 0x054);",
                                      "C_ASSERT(offsetof(Actor_guess, flag_guess) == 0x058);",
                                      "C_ASSERT(offsetof(Actor_guess, count_guess) == 0x08C);"]
    idx = C.load_index(repo)
    lay = idx.layout(idx.find_struct("Actor_guess"))
    assert lay.size == 0xA4 and lay.direct["follow_player_guess"][0] == 0x50 and lay.direct["tail"][0] == 0xA0
    # idempotent: a second run adds nothing
    assert G.main([str(tmp_path / "layout.json"), "--repo", str(repo), "--extend"]) == 0
    assert rd(repo / "include/trilogy/actor.h") == after


def test_extend_refuses_referenced_padding(repo, tmp_path, capsys):
    write(repo / "src/EverQuest/b.cpp", '#include "trilogy/actor.h"\nvoid f(Actor_guess* a) { a->unknown0x04C[9] = 0; }\n')
    before = rd(repo / "include/trilogy/actor.h")
    assert G.main([str(tmp_path / "layout.json"), "--repo", str(repo), "--extend"]) == 1
    err = capsys.readouterr().err
    assert "src/EverQuest/b.cpp:2: unknown0x04C[9] lands on no member" in err
    assert rd(repo / "include/trilogy/actor.h") == before
    assert G.main([str(tmp_path / "layout.json"), "--repo", str(repo), "--extend",
                   "--allow-padding-ref", "unknown0x04C"]) == 0


def test_extend_rewrites_padding_refs_and_wraps_unions(repo, tmp_path):
    write(repo / "src/EverQuest/b.cpp", '#include "trilogy/actor.h"\n'
          'void f(Actor_guess* a, EQPlayer* p) {\n'
          '    a->unknown0x04C[0xC] = 1;\n'            # 0x58 -> flag_guess (u8)
          '    p->actor_info->unknown0x04C[2] = 0;\n'  # 0x4E -> stays in unknown0x04C
          '    a->unknown0x04C[0xE] = 0;\n'            # 0x5A -> piece unknown0x059[1]
          '    *(float*)a->unknown0x080 = 2.0f;\n'      # float vs the new int field: refused
          '    *(int*)p->actor_info->unknown0x080 = 3;\n'
          '}\n', crlf=True)
    lay = json.loads(json.dumps(LAYOUT))
    lay["fields"] = [f for f in lay["fields"] if f["name"] not in ("floor_z", "tail", "count_guess")] + [
        {"offset": "0x080", "name": "first_guess", "type": "int", "width": 4},
        {"offset": "0x048", "name": "floor_z", "type": "f32", "width": 4,
         "union": [{"name": "floor_z_bits", "type": "u32"}]},
        {"offset": "0x0A0", "name": "tail", "type": "s32", "width": 4,
         "union": [{"name": "tail_wide_guess", "type": "double"}]}]
    lj = tmp_path / "l2.json"
    lj.write_text(json.dumps(lay), encoding="utf-8")
    # tail + double needs 4 bytes after 0xA4: no padding there -> refused
    assert G.main([str(lj), "--repo", str(repo), "--extend"]) == 1
    lay["fields"] = [f for f in lay["fields"] if f["name"] != "tail"]
    lj.write_text(json.dumps(lay), encoding="utf-8")
    # *(float*)a->unknown0x080 does not match first_guess (int): refused, nothing written
    before = rd(repo / "include/trilogy/actor.h")
    assert G.main([str(lj), "--repo", str(repo), "--extend"]) == 1
    assert rd(repo / "include/trilogy/actor.h") == before
    b = rd(repo / "src/EverQuest/b.cpp").replace("    *(float*)a->unknown0x080 = 2.0f;\r\n", "")
    write(repo / "src/EverQuest/b.cpp", b)
    assert G.main([str(lj), "--repo", str(repo), "--extend"]) == 0
    src = rd(repo / "src/EverQuest/b.cpp")
    assert "    a->flag_guess = 1;\r\n" in src
    assert "    p->actor_info->unknown0x04C[2] = 0;\r\n" in src
    assert "    a->unknown0x059[1] = 0;\r\n" in src
    assert "    p->actor_info->first_guess = 3;\r\n" in src
    h = rd(repo / "include/trilogy/actor.h")
    assert "    union\r\n    {\r\n        f32     floor_z;            /* 0x048* floor height */\r\n" in h
    assert "        u32     floor_z_bits;       /* 0x048* */\r\n    };\r\n" in h
    idx = C.load_index(repo)
    l2 = idx.layout(idx.find_struct("Actor_guess"))
    assert l2.size == 0xA4 and l2.direct["floor_z_bits"][0] == 0x48 and l2.direct["first_guess"][0] == 0x80


def test_generate_types_methods_2d_and_accessor(tmp_path):
    r = tmp_path / "t"
    write(r / "include/trilogy/common.h", COMMON)
    d = {"family": "Big_guess", "kind": "class", "header": "include/trilogy/layout/big.h", "size": "0x40",
         "includes": ["trilogy/common.h", "<windef.h>"], "forward": ["EQPlayer"],
         "methods": ["void Draw_guess(int mode);   /* 0x00401000 */"],
         "globals": [{"va": "0x006E4E44", "name": "g_big_guess", "decl": "Big_guess*", "accessor": "g_big_obj_guess"}],
         "types": [{"name": "Slot_guess", "size": "0x4", "fields": [
             {"offset": "0x0", "name": "type", "type": "char", "width": 1},
             {"offset": "0x2", "name": "value", "type": "short", "width": 2}]}],
         "fields": [{"offset": "0x2", "name": "slots_guess", "type": "Slot_guess", "count": 3, "width": 4},
                    {"offset": "0x10", "name": "rects_guess", "type": "RECT", "count": [2, 1], "width": 16},
                    {"offset": "0x30", "name": "grid_guess", "type": "short", "count": [2, 3], "width": 2}]}
    doc = G.parse_layout(d, "big.json")
    text = G.generate(doc, "\n")
    assert "typedef struct Slot_guess" in text and "C_ASSERT(sizeof(Slot_guess) == 0x004);" in text
    assert "    Slot_guess slots_guess[3];" in text           # 2-aligned sub-struct at 0x2 is fine
    assert "rects_guess[2][1];" in text and "grid_guess[2][3];" in text
    assert "class Big_guess\n{\npublic:" in text and "    void Draw_guess(int mode);   /* 0x00401000 */" in text
    assert "#include <windef.h>" in text and "struct EQPlayer;" in text
    assert "#define g_big_obj_guess EQ_GLOBAL(class Big_guess *, 0x006E4E44)" in text
    lay = G.verify(text, doc.header, doc, r, 0x40)
    assert lay.direct["grid_guess"][0] == 0x30
    bad = json.loads(json.dumps(d))
    bad["fields"][0]["offset"] = "0x1"
    with pytest.raises(G.LayoutError, match="misaligned"):
        G.generate(G.parse_layout(bad, "big.json"))


def test_extend_conflicts(repo, tmp_path):
    text = rd(repo / "include/trilogy/actor.h")
    bad = dict(LAYOUT, fields=[{"offset": "0x046", "name": "x_guess", "type": "u32", "width": 4}])
    with pytest.raises(G.LayoutError, match="overlaps"):
        G.extend(text, "include/trilogy/actor.h", G.parse_layout(bad), repo)
    moved = dict(LAYOUT, fields=[{"offset": "0x04C", "name": "floor_z", "type": "f32", "width": 4}])
    with pytest.raises(G.LayoutError, match="exists at 0x48"):
        G.extend(text, "include/trilogy/actor.h", G.parse_layout(moved), repo)


# batch 2 (eqmain rdp.h RdpState_guess): a by-value struct member (`RdpConnHash_guess hash`) the JSON lists
# too, padding spelled `unknownNNN` (no 0x), and same-named padding of unrelated structs/clients.
RDP_H = """\
#ifndef RDP_H
#define RDP_H
#include "trilogy/common.h"
typedef struct Hash_guess
{
    u16 count;                      /* 0x00 */
    u8  unknown02[2];               /* 0x02 */
    void* buckets;                  /* 0x04 */
} Hash_guess;

typedef struct State_guess
{
    u8  unknown000[0xC];            /* 0x000 */
    u32 socket;                     /* 0x00C */
    Hash_guess hash;                /* 0x010 */
    u8  unknown018[8];              /* 0x018 */
} State_guess;
C_ASSERT(offsetof(State_guess, hash) == 0x10);
C_ASSERT(sizeof(State_guess) == 0x20);
#endif
"""
OTHER_H = """\
#include "trilogy/common.h"
typedef struct Other_guess
{
    u8  unknown000[0x10];           /* 0x000 */
} Other_guess;
"""
RDP_LAYOUT = {"family": "State_guess", "client": "eqmain", "kind": "struct", "header": "include/eqmain/rdp.h",
              "size": "0x20", "pack": 8,
              "fields": [{"offset": "0x0", "name": "wsa_guess", "type": "int", "width": 4},
                         {"offset": "0x4", "name": "udp_guess", "type": "int", "width": 4},
                         {"offset": "0xC", "name": "socket", "type": "u32", "width": 4},
                         {"offset": "0x10", "name": "hash", "type": "Hash_guess", "width": 8},
                         {"offset": "0x18", "name": "rate_guess", "type": "u32", "width": 4}]}
RDP_C = """\
#include "eqmain/rdp.h"
#include "eqmain/other.h"

void tick_guess(State_guess *s, Other_guess *o)
{
    s->unknown018[5] = 1;
    o->unknown000[2] = 0;
}
"""
FOREIGN_CPP = """\
#include "trilogy/slider.h"
void Slide_guess(Slider_guess *p)
{
    p->unknown000[1] = 0;
}
"""


def test_extend_by_value_member_unknownNNN_padding_and_scoped_refs(tmp_path, capsys):
    r = tmp_path / "tree"
    write(r / "include/trilogy/common.h", COMMON)
    write(r / "include/eqmain/rdp.h", RDP_H, crlf=True)
    write(r / "include/eqmain/other.h", OTHER_H)
    write(r / "include/trilogy/slider.h", OTHER_H.replace("Other_guess", "Slider_guess"))
    write(r / "src/eqmain/x.c", RDP_C, crlf=True)
    write(r / "src/EverQuest/z.cpp", FOREIGN_CPP)
    lj = tmp_path / "rdp.json"
    lj.write_text(json.dumps(RDP_LAYOUT), encoding="utf-8")
    # an untyped base in a TU that sees both State_guess and Other_guess: kept as a reference (conservative)
    write(r / "src/eqmain/y.c", '#include "eqmain/rdp.h"\n#include "eqmain/other.h"\n'
          'void w_guess(void) { get_guess()->unknown000[3] = 0; }\n')
    assert G.main([str(lj), "--repo", str(r), "--extend"]) == 1
    err = capsys.readouterr().err
    assert "src/eqmain/y.c:3: unknown000[3]" in err
    assert "x.c" not in err and "z.cpp" not in err             # Other_guess-typed / other-client references
    (r / "src/eqmain/y.c").unlink()
    assert G.main([str(lj), "--repo", str(r), "--extend"]) == 0
    h = rd(r / "include/eqmain/rdp.h")
    assert "    int     wsa_guess;          /* 0x000* */\r\n    int     udp_guess;          /* 0x004* */\r\n" in h
    assert "    u8      unknown008[4];      /* 0x008  */\r\n" in h          # piece keeps the unknownNNN spelling
    assert "    u32     rate_guess;         /* 0x018* */\r\n    u8      unknown01C[4];      /* 0x01C  */\r\n" in h
    assert "    Hash_guess hash;                /* 0x010 */\r\n" in h      # existing by-value member untouched
    assert "    u8  unknown02[2];               /* 0x02 */\r\n" in h       # and so is the nested struct's padding
    assert "C_ASSERT(offsetof(State_guess, rate_guess) == 0x018);" in h
    src = rd(r / "src/eqmain/x.c")
    assert "    s->unknown01C[1] = 1;\r\n" in src and "    o->unknown000[2] = 0;\r\n" in src
    assert rd(r / "src/EverQuest/z.cpp") == FOREIGN_CPP
    idx = C.load_index(r)
    lay = idx.layout(idx.find_struct("State_guess"))
    assert lay.size == 0x20 and lay.direct["hash"][0] == 0x10 and lay.direct["udp_guess"][0] == 4


def test_pad_style():
    assert G.pad_style("unknown0x04C", 0x4C) == ("unknown0x", 3, False)
    assert G.pad_style("unknown04c", 0x4C) == ("unknown", 3, True)
    assert G.pad_style("unknown000", 0) == ("unknown", 3, False)
    assert G.pad_style("padBEEF", 0xBEEF) == ("pad", 4, False)
    assert G.pad_style("reserved", 0x10) is None and G.pad_style("unknown10", 0x20) is None
    assert G.style_name(("unknown", 3, False), 0x1C) == "unknown01C"


# ---------------------------------------------------------------------------------------- propagate

def census_rows(repo, out):
    cen = CE.ClientCensus(repo, "trilogy")
    cen.load()
    cen.run("guess")
    CE.write_reports(cen, out)
    return out / "accesses.jsonl"


@pytest.fixture
def extended(repo, tmp_path):
    assert G.main([str(tmp_path / "layout.json"), "--repo", str(repo), "--extend"]) == 0
    ns = tmp_path / "ns" / "a"
    ns.mkdir(parents=True)
    (ns / "name_sensitive.json").write_text(json.dumps([{"va": "0x00401200", "stage": "local"}]), encoding="utf-8")
    return repo, census_rows(repo, tmp_path / "census"), tmp_path / "ns"


def by_line(sites):
    src = SRC.split("\n")
    out = {}
    for s in sites:
        out.setdefault(src[s.line - 1].strip(), []).append(s)
    return out


def test_propagate_decisions(extended, tmp_path):
    repo, cen, ns = extended
    pr = P.Propagator(repo, [G.load_layout(tmp_path / "layout.json")], P.load_census(cen), P.load_name_sensitive([ns]))
    sites = pr.collect()
    bl = by_line([s for s in sites if s.func == "Follow_guess"])

    def one(prefix):
        hits = [s for k, v in bl.items() if k.startswith(prefix) for s in v]
        assert len(hits) == 1, (prefix, [(s.line, s.reason) for s in sites])
        return hits[0]
    s = one("t = *(char**)")
    assert s.status == "rewrite" and s.new == "self->actor_info->follow_target_guess"
    assert one("*(float*)((char*)self->actor_info + 0x54)").new == "self->actor_info->heading_guess"
    assert one("*(EQPlayer**)((int)").new == "self->actor_info->follow_player_guess"     # union role, int cast
    assert one("*(u32*)((char*)self->actor_info + 0x50)").reason == "type_differs"
    assert one("*(signed char*)").reason == "type_differs"                                # u8 field: movzx vs movsx
    assert one("*(u8*)((u32)").new == "self->actor_info->flag_guess"
    assert one("*(float*)(info + 0x54)").reason == "base_not_typed"
    assert one("*(void**)").new == "self->actor_info->attach_guess[1]"                   # array element
    assert one("*(int*)((char*)self->actor_info + 0x60)").reason == "no_member"
    assert one("*(int*)((char*)self->actor_info + 0x8C + i").reason == "dynamic_offset"
    assert one("slot = (u32*)").reason == "addr_type_differs"
    assert one("t = (char*)((char*)").reason == "addr_untyped" or one("t = (char*)((char*)").reason == "addr_type_differs"
    assert one("*(volatile int*)").reason == "volatile"
    assert one("*(int*)((char*)self->actor_info + 0x8C) = 3").new == "self->actor_info->count_guess"
    assert one("*(s32*)").new == "self->actor_info->tail"
    assert one("*(long*)").new == "self->actor_info->tail"                               # s32 = signed long
    ml = [s for s in sites if s.offset == 0x48][0]                                        # access split over 2 lines
    assert ml.status == "rewrite" and ml.new == "self->actor_info->floor_z"
    reasons = {s.func: s.reason for s in sites if s.func in ("Naked_guess", "HeaderSensitive_guess",
                                                              "NameSensitive_guess")}
    assert reasons == {"Naked_guess": "naked", "HeaderSensitive_guess": "header_sensitive",
                       "NameSensitive_guess": "name_sensitive"}
    addr = [s for s in sites if s.func == "Addr_guess"]
    assert {s.offset: (s.status, s.new or s.reason) for s in addr} == {
        0x8C: ("rewrite", "&a->count_guess"), 0x54: ("rewrite", "EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->heading_guess")}


def test_propagate_apply_crlf_and_report(extended, tmp_path, capsys):
    repo, cen, ns = extended
    src = repo / "src/EverQuest/a.cpp"
    before = rd(src)
    md = tmp_path / "rej.md"
    js = tmp_path / "all.json"
    args = ["--repo", str(repo), "--layout", str(tmp_path / "layout.json"), "--census", str(cen),
            "--name-sensitive", str(ns), "--md", str(md), "--report", str(js)]
    assert P.main(args) == 0                                  # dry run: no edit
    out = capsys.readouterr().out
    assert "REWRITE" in out and "REFUSE" in out and rd(src) == before
    assert P.main(args + ["--apply", "-q"]) == 0
    after = rd(src)
    assert after.count("\r\n") == after.count("\n")           # CRLF preserved
    assert "    t = self->actor_info->follow_target_guess;\r\n" in after
    assert "    self->actor_info->attach_guess[1] = 0;\r\n" in after
    assert "    self->actor_info->floor_z = 0.0f;\r\n" in after      # 2-line access collapsed
    assert "    n = &a->count_guess;\r\n" in after
    assert "*(u32*)((char*)self->actor_info + 0x50) = 0;" in after  # refused site untouched
    rep = md.read_text(encoding="utf-8")
    assert "| type_differs |" in rep and "| naked | 1 |" in rep and "## Refused sites" in rep
    rows = json.loads(js.read_text(encoding="utf-8"))
    assert {r["status"] for r in rows} == {"rewrite", "refused"}
    # the rewritten tree is still census-clean for the struct: a rerun finds nothing left to rewrite
    cen2 = census_rows(repo, tmp_path / "census2")
    pr = P.Propagator(repo, [G.load_layout(tmp_path / "layout.json")], P.load_census(cen2), P.load_name_sensitive([ns]))
    assert not [s for s in pr.collect() if s.status == "rewrite"]


def test_propagate_stale_census(extended, tmp_path):
    repo, cen, ns = extended
    src = repo / "src/EverQuest/a.cpp"
    write(src, rd(src).replace("    t = *(char**)", "    /* moved */\r\n    t = *(char**)"))
    pr = P.Propagator(repo, [G.load_layout(tmp_path / "layout.json")], P.load_census(cen), P.load_name_sensitive([ns]))
    assert any(s.reason == "census_stale" for s in pr.collect())


def test_split_terms_and_postfix():
    assert [t.strip() for _, t in P.split_terms("(char *)p->a->b + 0x4C")] == ["(char *)p->a->b", "0x4C"]
    assert len(P.split_terms("x - -1")) == 2 and len(P.split_terms("p->q")) == 1
    assert P.top_level_ops("p + 1") and not P.top_level_ops("p[i + 1]->x")
    assert P.postfix("self->actor_info") == "self->actor_info"
    assert P.postfix("*pp") == "(*pp)"
    assert P.postfix("(p->x)") == "p->x"


def test_cparse_access_specifier_before_anonymous_union():
    idx = C.Index()
    idx.add(C.parse_file("k.h", "class K\n{\npublic:\n    union\n    {\n        int a;\n        float b;\n    };\n"
                                "    int c;\n};\n"))
    lay = idx.layout(idx.find_struct("K"))
    assert lay.direct["a"][0] == lay.direct["b"][0] == 0 and lay.direct["c"][0] == 4 and lay.size == 8


# ---------------------------------------------------------------------------------------- propagate: S4b / macros / S4c

MAIN_H = """\
#ifndef MAIN_H
#define MAIN_H
#include "trilogy/common.h"
#define EQ_GLOBAL(type, va) (*(type *)(va))
class Main_guess
{
public:
    int     mode_guess;         /* 0x000* */
    float   rate_guess;         /* 0x004* */
    short   slots_guess[4];     /* 0x008* */
};
class Derived_guess : public Main_guess
{
public:
    void m();
};
#define pinstMain_guess EQ_GLOBAL(class Main_guess *, 0x006E4E44)
#endif
"""

SRC_B = """\
#include "trilogy/common.h"
#define EQ_GLOBAL(type, va) (*(type *)(va))
#define g_main_raw EQ_GLOBAL(char *, 0x006E4E44)
#define MAIN_MODE (*(int *)(g_main_raw + 0x0))
#define MAIN_FIELD(type, off) (*(type *)(g_main_raw + (off)))

// FUNCTION: TRILOGY 0x00402000
int Uses_guess(int i)
{
    char* p;
    int r;
    p = (char*)EQ_GLOBAL(char*, 0x006E4E44);
    r = *(int*)(g_main_raw + 0x0);
    r += (int)*(float*)(EQ_GLOBAL(u8*, 0x006E4E44) + 4);
    r += *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xC);
    r += *(short*)(*(u32*)0x006E4E44 + 0xA);
    r += *(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x4);
    r += MAIN_MODE + MAIN_FIELD(int, 0x0);
    return r;
}

// FUNCTION: TRILOGY 0x00402100
int Skipped_guess(void)
{
    return *(int*)(g_main_raw + 0x0);
}
"""


@pytest.fixture
def mrepo(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/common.h", COMMON, crlf=True)
    write(r / "include/trilogy/main.h", MAIN_H, crlf=True)
    write(r / "src/EverQuest/b.cpp", SRC_B, crlf=True)
    lay = {"family": "Main_guess", "kind": "class", "header": "include/trilogy/main.h", "size": "0x10",
           "globals": [{"va": "0x006E4E44", "name": "g_main_raw", "decl": "Main_guess*",
                        "accessor": "pinstMain_guess", "include": ["trilogy/main.h"]}],
           "fields": [{"offset": "0x0", "name": "mode_guess", "type": "int", "width": 4}]}
    (tmp_path / "main.json").write_text(json.dumps(lay), encoding="utf-8")
    return r


def test_propagate_global_accessor_macros_and_skip(mrepo, tmp_path):
    cen = census_rows(mrepo, tmp_path / "census")
    doc = G.load_layout(tmp_path / "main.json")
    pr = P.Propagator(mrepo, [doc], P.load_census(cen), set(), skip_funcs=["0x00402100"])
    sites = pr.collect()
    got = {(s.line, s.offset, s.mode): (s.status, s.new or s.reason) for s in sites}
    assert got[(13, 0, "s4b")] == ("rewrite", "pinstMain_guess->mode_guess")
    assert got[(14, 4, "s4b")] == ("rewrite", "pinstMain_guess->rate_guess")   # u8* base, no cast needed
    assert got[(15, 0xC, "s4b")] == ("rewrite", "pinstMain_guess->slots_guess[2]")
    assert got[(16, 0xA, "s4b")] == ("refused", "abs_base")
    assert got[(17, 4, "s4b")][0] == "refused"                          # int access of a float member
    macro = [s for s in sites if s.mode.startswith("macro")]
    assert any(s.func == "#define MAIN_MODE" and s.status == "rewrite" and s.new == "pinstMain_guess->mode_guess"
               for s in macro)
    assert any(s.reason == "via_macro" for s in macro)                  # MAIN_FIELD(type, off): parameters
    assert [s.reason for s in sites if s.func.endswith("Skipped_guess")] == ["gate_rollback"]
    pr.apply()
    out = rd(mrepo / "src/EverQuest/b.cpp")
    assert '#include "trilogy/main.h"\r\n' in out.split("#define g_main_raw")[0]   # accessor header added
    assert "#define MAIN_MODE (pinstMain_guess->mode_guess)\r\n" in out
    assert "    r = pinstMain_guess->mode_guess;\r\n" in out
    assert "*(short*)(*(u32*)0x006E4E44 + 0xA)" in out


def test_propagate_cast_at_use_and_derived(mrepo, tmp_path):
    src = ('#include "trilogy/main.h"\n'
           '// FUNCTION: TRILOGY 0x00403000\n'
           'void Derived_guess::m()\n{\n    *(int*)((char*)this + 0x0) = 1;\n}\n'
           '// FUNCTION: TRILOGY 0x00403100\n'
           'int f(Main_guess* m, char* other)\n{\n    char* q;\n    q = (char*)m;\n'
           '    other = *(char**)((char*)m + 8);\n'
           '    return *(int*)(q + 0x0) + *(int*)(other + 0x0);\n}\n')
    write(mrepo / "src/EverQuest/c.cpp", src, crlf=True)
    cen = census_rows(mrepo, tmp_path / "census")
    doc = G.load_layout(tmp_path / "main.json")
    rows = P.load_census(cen)
    off = P.Propagator(mrepo, [doc], rows, set(), files=["src/EverQuest/c.cpp"]).collect()
    assert {(s.line, s.mode, s.status) for s in off} >= {(5, "s4a", "rewrite"), (13, "s4c", "refused")}
    on = P.Propagator(mrepo, [doc], rows, set(), files=["src/EverQuest/c.cpp"], cast_at_use=True).collect()
    s4c = [s for s in on if s.mode == "s4c"]
    assert [(s.status, s.new) for s in s4c] == [("rewrite", "((Main_guess*)q)->mode_guess")]
    assert [s.new for s in on if s.line == 5] == ["this->mode_guess"]


# ---------------------------------------------------------------------------------------- s5_unify

import s5_unify as S5  # noqa: E402

GFX_LAYOUT = {
    "family": "T3D_guess", "client": "eqgfx", "kind": "struct", "header": "include/eqgfx/layout/t3d.h",
    "size": "0x20", "pack": 8, "aliases": ["Actor_guess"], "includes": ["<stddef.h>"],
    "prelude": ["#ifndef C_ASSERT", "#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]", "#endif"],
    "fields": [
        {"offset": "0x0", "name": "flags_guess", "type": "unsigned char", "count": 4, "width": 1,
         "union": [{"name": "flagsDword_guess", "type": "unsigned int"}]},
        {"offset": "0x4", "name": "position_guess", "type": "float", "count": 3, "width": 4},
        {"offset": "0x10", "name": "action_guess", "type": "unsigned int", "width": 4,
         "union": [{"name": "action_int_guess", "type": "int"}]},
        {"offset": "0x14", "name": "definition_guess", "type": "struct Def_guess*", "width": 4},
        {"offset": "0x18", "name": "scale_guess", "type": "float", "width": 4}],
    "copy_maps": {"src/eqgfx/a.c:4": {"worldY": "position_guess[1]"},
                  "src/eqgfx/b.c:4": {"location": "cast at use"}}}

GFX_A = """\
#include <stddef.h>

typedef struct Actor_guess
{
    unsigned char pad[3];
    unsigned char hiFlags;      // 0x03
    float worldX;               // 0x04
    float worldY;               // 0x08
    float worldZ;               // 0x0c
    int action;                 // 0x10
    void *definition;           // 0x14
    unsigned long scaleBits;    // 0x18
} Actor_guess;

// FUNCTION: EQGFX 0x10001000
int f(Actor_guess *actor, void **out, void *any)
{
    actor->hiFlags |= 0x40;
    *out = actor->definition;
    if (((Actor_guess *)any)->action < 0)
        return 1;
    return actor->action + (int)actor->worldY;
}
"""

GFX_B = """\
#include <stddef.h>
typedef struct Loc_guess { float v[3]; } Loc_guess;
typedef struct Actor_guess
{
    unsigned char pad[4];
    Loc_guess location;         // 0x04
} Actor_guess;

// FUNCTION: EQGFX 0x10002000
float g(Actor_guess *actor)
{
    return actor->location.v[0];
}
"""


def test_s5_unify_maps_by_offset_and_refuses_casts(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/eqgfx/.keep", "")
    write(r / "src/eqgfx/a.c", GFX_A, crlf=True)
    write(r / "src/eqgfx/b.c", GFX_B, crlf=True)
    lj = tmp_path / "t3d.json"
    lj.write_text(json.dumps(GFX_LAYOUT), encoding="utf-8")
    assert G.main([str(lj), "--repo", str(r)]) == 0
    cen = CE.ClientCensus(r, "eqgfx")
    cen.load()
    cen.run("guess")
    CE.write_reports(cen, tmp_path / "census")
    cj = tmp_path / "census" / "accesses.jsonl"
    rep = tmp_path / "s5.json"
    rc = S5.main(["--repo", str(r), "--layout", str(lj), "--census", str(cj), "--apply", "--report", str(rep)])
    assert rc == 1                                         # b.c refused
    a = rd(r / "src/eqgfx/a.c")
    assert '#include "eqgfx/layout/t3d.h"\r\n' in a and "typedef struct Actor_guess" not in a
    assert "actor->flags_guess[3] |= 0x40;" in a           # u8 at 0x3 -> array element
    assert "*out = actor->definition_guess;" in a          # void* view -> typed pointer (C)
    assert "((Actor_guess *)any)->action_int_guess < 0" in a   # cast base found without census; int role
    assert "actor->action_int_guess + (int)actor->position_guess[1]" in a
    assert rd(r / "src/eqgfx/b.c") == GFX_B.replace("\n", "\r\n")   # refused: Loc_guess view needs a cast
    out = json.loads(rep.read_text(encoding="utf-8"))
    st = {x["file"]: x["status"] for x in out}
    assert st == {"src/eqgfx/a.c": "rewrite", "src/eqgfx/b.c": "refused"}
    assert "needs a cast at use" in [x for x in out if x["file"] == "src/eqgfx/b.c"][0]["reason"]


# batch 2 (t3d_dmsprite_def.h): a by-value member of a type from an included layout header (gfx_types.h
# Vector3_guess) had no size when s5_unify parsed the header alone, so every later offset was wrong.
BOX_TYPES_H = """\
#ifndef GFX_TYPES_H
#define GFX_TYPES_H
typedef struct Vec3_guess
{
    float x;
    float y;
    float z;
} Vec3_guess;
#endif
"""
BOX_H = """\
#ifndef BOX_H
#define BOX_H
#include <stddef.h>
#include "eqgfx/layout/gfx_types.h"
typedef struct Box_guess
{
    unsigned int kind_guess;    /* 0x00 */
    Vec3_guess center_guess;    /* 0x04 */
    unsigned int count_guess;   /* 0x10 */
} Box_guess;
#endif
"""
BOX_LAYOUT = {"family": "Box_guess", "client": "eqgfx", "kind": "struct", "header": "include/eqgfx/layout/box.h",
              "size": "0x14", "pack": 8, "includes": ["eqgfx/layout/gfx_types.h"],
              "fields": [{"offset": "0x0", "name": "kind_guess", "type": "unsigned int", "width": 4},
                         {"offset": "0x4", "name": "center_guess", "type": "Vec3_guess", "width": 12},
                         {"offset": "0x10", "name": "count_guess", "type": "unsigned int", "width": 4}],
              "copy_maps": {"src/eqgfx/box.c:3": {}}}
BOX_C = """\
#include <stddef.h>

typedef struct Box_guess
{
    unsigned int kind;
    float cx;
    float cy;
    float cz;
    unsigned int count;
} Box_guess;

// FUNCTION: EQGFX 0x10003000
unsigned int h(Box_guess *b)
{
    return b->count + (unsigned int)b->cy;
}
"""


def test_s5_unify_parses_the_headers_includes(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/eqgfx/layout/gfx_types.h", BOX_TYPES_H)
    write(r / "include/eqgfx/layout/box.h", BOX_H)
    write(r / "src/eqgfx/box.c", BOX_C)
    lj = tmp_path / "box.json"
    lj.write_text(json.dumps(BOX_LAYOUT), encoding="utf-8")
    _, lay = S5.new_layout(r, G.load_layout(lj))
    assert lay.size == 0x14 and lay.direct["count_guess"][0] == 0x10 and lay.direct["center_guess"][0] == 4
    cen = CE.ClientCensus(r, "eqgfx")
    cen.load()
    cen.run("guess")
    CE.write_reports(cen, tmp_path / "census")
    rc = S5.main(["--repo", str(r), "--layout", str(lj), "--census", str(tmp_path / "census" / "accesses.jsonl"),
                  "--apply"])
    assert rc == 0
    out = rd(r / "src/eqgfx/box.c")
    assert '#include "eqgfx/layout/box.h"' in out and "float cx;" not in out
    assert "return b->count_guess + (unsigned int)b->center_guess.y;" in out


def test_propagate_role_assignment_and_local_alias_resolution(mrepo, tmp_path):
    src = ('#include "trilogy/main.h"\n'
           '// FUNCTION: TRILOGY 0x00404000\n'
           'int setMode_guess(char* m, int v)\n{\n    *(int*)(m + 0) = v;\n    return *(int*)(m + 0);\n}\n')
    write(mrepo / "src/EverQuest/d.cpp", src, crlf=True)
    # another TU with an unrelated local struct of the same name must not be treated as the family
    write(mrepo / "src/EverQuest/e.cpp", '#include "trilogy/common.h"\nstruct Main_guess { char c; int x; };\n'
          '// FUNCTION: TRILOGY 0x00405000\nint h(Main_guess* q) { return *(int*)((char*)q + 4); }\n', crlf=True)
    cen = census_rows(mrepo, tmp_path / "census")
    rows = P.load_census(cen)
    doc = G.load_layout(tmp_path / "main.json")
    fam = [r["family"] for r in rows if r["file"] == "src/EverQuest/d.cpp"][0]
    assert fam.startswith("?char*:m[")
    pr = P.Propagator(mrepo, [doc], rows, set(), files=["src/EverQuest/d.cpp", "src/EverQuest/e.cpp"],
                      cast_at_use=True, roles={fam: "Main_guess"})
    sites = pr.collect()
    assert [(s.file, s.status, s.new) for s in sites] == [
        ("src/EverQuest/d.cpp", "rewrite", "((Main_guess*)m)->mode_guess"),
        ("src/EverQuest/d.cpp", "rewrite", "((Main_guess*)m)->mode_guess")]


def test_propagate_macro_use_sites(mrepo, tmp_path):
    src = ('#include "trilogy/main.h"\n'
           '#define FIELD_AT(type, obj, off) (*(type*)((char*)(obj) + (off)))\n'
           '#define RAW_MAIN EQ_GLOBAL(char*, 0x006E4E44)\n'
           '// FUNCTION: TRILOGY 0x00406000\n'
           'int u(Main_guess* m)\n{\n'
           '    int r = FIELD_AT(int, RAW_MAIN, 0x0) + (int)FIELD_AT(float, RAW_MAIN, 4);\n'
           '    r += FIELD_AT(short, m, 0xA) + FIELD_AT(int, m, 0x4);\n'
           '    return r + FIELD_AT(int, (char*)m + 4, 0x0);\n}\n')
    write(mrepo / "src/EverQuest/f.cpp", src, crlf=True)
    cen = census_rows(mrepo, tmp_path / "census")
    doc = G.load_layout(tmp_path / "main.json")
    rows = P.load_census(cen)
    off = P.Propagator(mrepo, [doc], rows, set(), files=["src/EverQuest/f.cpp"]).collect()
    assert off and all(s.reason == "via_macro" for s in off)
    pr = P.Propagator(mrepo, [doc], rows, set(), files=["src/EverQuest/f.cpp"], macro_uses=True)
    got = sorted((s.line, s.offset, s.status, s.new or s.reason) for s in pr.collect())
    assert (7, 0, "rewrite", "pinstMain_guess->mode_guess") in got
    assert (7, 4, "rewrite", "pinstMain_guess->rate_guess") in got
    assert (8, 0xA, "rewrite", "m->slots_guess[1]") in got
    assert (8, 4, "refused", "type_differs") in got
    pr.apply()
    out = rd(mrepo / "src/EverQuest/f.cpp")
    assert "    int r = pinstMain_guess->mode_guess + (int)pinstMain_guess->rate_guess;\r\n" in out
    assert "    r += m->slots_guess[1] + FIELD_AT(int, m, 0x4);\r\n" in out


# ---------------------------------------------------------------------------------------- rename_members

import rename_members as RN  # noqa: E402

RN_H = """\
#ifndef RN_H
#define RN_H
#include "common.h"
typedef struct A_guess
{
    int     foo;                /* 0x00* */
    int     shared;             /* 0x04* */
} A_guess;
C_ASSERT(offsetof(A_guess, foo) == 0x00);
typedef struct B_guess
{
    int     shared;
} B_guess;
#endif
"""

RN_SRC = """\
#include "trilogy/rn.h"
#define AF(p) ((p)->foo)
#define AS(p) ((p)->shared)

// FUNCTION: TRILOGY 0x00407000
int r(A_guess* a, B_guess* b)
{
    int n = a->foo + AF(a) + (int)offsetof(A_guess, foo);
    n += a->shared + b->shared;
    return n + AS(a);
}
"""


def test_rename_members_unique_shared_macros_and_decls(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/common.h", COMMON, crlf=True)
    write(r / "include/trilogy/rn.h", RN_H, crlf=True)
    write(r / "src/EverQuest/r.cpp", RN_SRC, crlf=True)
    mp = tmp_path / "map.json"
    mp.write_text(json.dumps({"_comment": "x", "A_guess": {"foo": "foo_guess", "shared": "sharedA_guess"}}),
                  encoding="utf-8")
    assert RN.main(["--repo", str(r), "--map", str(mp), "--apply"]) == 0
    src = rd(r / "src/EverQuest/r.cpp")
    hdr = rd(r / "include/trilogy/rn.h")
    assert "#define AF(p) ((p)->foo_guess)\r\n" in src                  # unique: macro body renamed
    assert "#define AS(p) ((p)->shared)\r\n" in src                     # shared name in a macro: refused
    assert "int n = a->foo_guess + AF(a) + (int)offsetof(A_guess, foo_guess);" in src
    assert "n += a->sharedA_guess + b->shared;" in src                  # census-typed base only
    assert "    int     foo_guess;          /* 0x00* */" not in hdr      # the decl keeps its column text
    assert "    int     foo_guess;" in hdr and "C_ASSERT(offsetof(A_guess, foo_guess) == 0x00);" in hdr
    assert "    int     sharedA_guess;" in hdr and "B_guess" in hdr and hdr.count("int     shared;") == 1


def test_generate_overlay_views(tmp_path):
    r = tmp_path / "t"
    write(r / "include/eqgfx/.keep", "")
    d = json.loads(json.dumps(GFX_LAYOUT))
    d["types"] = [{"name": "Pt_guess", "size": "0xC", "fields": [
        {"offset": "0x0", "name": "x", "type": "float", "width": 4},
        {"offset": "0x4", "name": "y", "type": "float", "width": 4},
        {"offset": "0x8", "name": "z", "type": "float", "width": 4}]}]
    d["fields"][1]["overlay"] = [{"name": "pt_guess", "type": "Pt_guess", "width": 12},
                                 {"name": "words_guess", "type": "unsigned long", "count": 4, "width": 4}]
    doc = G.parse_layout(d, "t.json")
    text = G.generate(doc, "\n")
    assert "        struct\n        {\n" in text.replace("    union\n    {\n        struct", "        struct")
    assert "Pt_guess pt_guess;" in text and "unsigned long words_guess[4];" in text
    lay = G.verify(text, doc.header, doc, r, 0x20)
    assert lay.direct["pt_guess"][0] == 4 and lay.direct["words_guess"][0] == 4
    assert lay.direct["action_guess"][0] == 0x10 and lay.size == 0x20      # the 16-byte view covers action too


def test_propagate_whole_function_globals(mrepo, tmp_path):
    src = ('#include "trilogy/main.h"\n#define RAW_MAIN EQ_GLOBAL(char*, 0x006E4E44)\n'
           '// FUNCTION: TRILOGY 0x00408000\nint w(void)\n{\n'
           '    return *(int*)(RAW_MAIN + 0) + *(int*)(RAW_MAIN + 4);\n}\n'
           '// FUNCTION: TRILOGY 0x00408100\nint w2(void)\n{\n    return *(int*)(RAW_MAIN + 0);\n}\n')
    write(mrepo / "src/EverQuest/g.cpp", src, crlf=True)
    cen = census_rows(mrepo, tmp_path / "census")
    doc = G.load_layout(tmp_path / "main.json")
    pr = P.Propagator(mrepo, [doc], P.load_census(cen), set(), files=["src/EverQuest/g.cpp"], whole_function=True)
    got = sorted((s.func_va, s.offset, s.status, s.reason) for s in pr.collect())
    assert got == [("0x00408000", 0, "refused", "partial_function"), ("0x00408000", 4, "refused", "type_differs"),
                   ("0x00408100", 0, "rewrite", "")]
