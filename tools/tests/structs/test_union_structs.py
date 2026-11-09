"""Struct-typed union members, qualifiers and named sub-struct overlays (S2b open question 1).

Minimal reproductions of the three blocked layouts:
  rdp_state_tri 0x118   a 0x94-byte serial state struct as a union role of the existing 0x50-byte `serial` member
                        (hand-written rdp.h: extend mode inserts the `types` struct and wraps the by-value member)
  rdp_message(_tri)     flagsWord_guess u16 with a struct role whose byte 1 is the (volatile) `flags` byte
  inflate_blocks_state  0x38 union role typed by a function-pointer typedef from the prelude (explicit width)
"""
import json
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "structs"))
sys.path.insert(0, str(ROOT / "unicorn"))

import cparse as C  # noqa: E402
import census as CE  # noqa: E402
import gen_layout_h as G  # noqa: E402
import propagate as P  # noqa: E402
import s5_unify as S5  # noqa: E402

COMMON = """\
#ifndef COMMON_H
#define COMMON_H
#include <stddef.h>
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e) ? 1 : -1]
#endif
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef signed long s32;
#endif
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


def census(repo, client, out):
    cen = CE.ClientCensus(repo, client)
    cen.load()
    cen.run("guess")
    CE.write_reports(cen, out)
    return out / "accesses.jsonl"


# ---------------------------------------------------------------------------------------- case 1: rdp_state_tri

RDP_H = """\
/* rdp.h - hand written */
#ifndef RDP_H
#define RDP_H
#include "common.h"

typedef struct RdpList_guess
{
    void *head;             /* 0x00 */
    void *tail;             /* 0x04 */
    u32 count;              /* 0x08 */
    u32 a;                  /* 0x0C */
    u32 b;                  /* 0x10 */
} RdpList_guess;

typedef struct RdpSerialQueue_guess
{
    u8  lock[0x18];         /* 0x00  CRITICAL_SECTION */
    u32 value;              /* 0x18 */
    u8  unknown1C[0x1C];    /* 0x1C */
    RdpList_guess list;     /* 0x38 */
    u32 queued_bytes;       /* 0x4C */
} RdpSerialQueue_guess;
C_ASSERT(sizeof(RdpSerialQueue_guess) == 0x50);

/* rdp.c: the rdp instance (one per socket). Only partly known. */
typedef struct RdpState_guess
{
    u32 socket;                     /* 0x00 */
    u8  unknown004[0x14];           /* 0x04 */
    RdpSerialQueue_guess serial;    /* 0x18 */
    u8  unknown068[0x44];           /* 0x68 */
} RdpState_guess;
C_ASSERT(offsetof(RdpState_guess, serial) == 0x18);
#endif
"""

SERIAL_TYPE = {"name": "RdpSerialStateTri_guess", "size": "0x94", "fields": [
    {"offset": "0x0", "name": "lock", "type": "u8", "count": 24, "width": 1},
    {"offset": "0x18", "name": "value", "type": "u32", "width": 4},
    {"offset": "0x1C", "name": "file", "type": "void*", "width": 4, "union": [{"name": "handle", "type": "void*"}]},
    {"offset": "0x38", "name": "list", "type": "RdpList_guess", "width": 20},
    {"offset": "0x4C", "name": "queued_bytes", "type": "u32", "width": 4},
    {"offset": "0x90", "name": "state", "type": "u32", "width": 4}]}

RDP_STATE = {
    "family": "RdpState_guess", "client": "trilogy", "kind": "struct", "header": "include/trilogy/rdp.h",
    "size": "0xAC", "pack": 8, "types": [SERIAL_TYPE],
    "fields": [
        {"offset": "0x0", "name": "socket", "type": "u32", "width": 4},
        {"offset": "0x18", "name": "serial", "type": "RdpSerialQueue_guess", "width": 80,
         "union": [{"name": "serialState_guess", "type": "RdpSerialStateTri_guess"}]}],
    "keep_cast_sites": []}

RDP_SRC = """\
#include "trilogy/common.h"
#include "trilogy/rdp.h"

// FUNCTION: TRILOGY 0x004F0000
void SerialUse_guess(RdpState_guess* s)
{
    RdpSerialStateTri_guess* st;
    RdpList_guess* l;
    *(u32*)((char*)s + 0x64) = 0;
    *(u32*)((char*)s + 0xA8) = 1;
    *(void**)((char*)s + 0x34) = 0;
    *(int*)((char*)s + 0x34) = -1;
    st = (RdpSerialStateTri_guess*)((char*)s + 0x18);
    l = (RdpList_guess*)((char*)s + 0x50);
}
"""


@pytest.fixture
def rdp_repo(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/trilogy/common.h", COMMON, crlf=True)
    write(r / "include/trilogy/rdp.h", RDP_H, crlf=True)
    write(r / "src/Network/rdp/rdp.c", RDP_SRC, crlf=True)
    (tmp_path / "rdp_state.json").write_text(json.dumps(RDP_STATE), encoding="utf-8")
    return r


def test_struct_union_member_width_comes_from_types():
    doc = G.parse_layout(json.loads(json.dumps(RDP_STATE)), "rdp_state.json")
    u = doc.fields[1].union[0]
    assert u.width == 0x94 and doc.fields[1].size == 0x94 and doc.fields[1].end == 0xAC
    bad = json.loads(json.dumps(RDP_STATE))
    bad["fields"][1]["union"][0]["width"] = 0x50
    with pytest.raises(G.LayoutError, match="!= sizeof"):
        G.parse_layout(bad, "x.json")
    bad = json.loads(json.dumps(RDP_STATE))
    bad["types"][0]["size"] = None
    with pytest.raises(G.LayoutError, match="without a size"):
        G.parse_layout(bad, "x.json")
    bad = json.loads(json.dumps(RDP_STATE))
    bad["size"] = "0xA0"                                           # the 0x94 role ends at 0xAC
    with pytest.raises(G.LayoutError, match="past size"):
        G.parse_layout(bad, "x.json")
    bad = json.loads(json.dumps(RDP_STATE))
    bad["types"][0]["fields"][-1]["offset"] = "0x92"               # sub-struct field past its own size
    with pytest.raises(G.LayoutError, match="past size"):
        G.parse_layout(bad, "x.json")


def test_generate_struct_union_member(tmp_path):
    """Generate mode: the types struct is emitted first, the union holds the existing view and the wider role,
    padding resumes after the widest member, nested member paths are verified."""
    r = tmp_path / "t"
    write(r / "include/trilogy/common.h", COMMON)
    write(r / "include/trilogy/rdplist.h", '#include "common.h"\n' + RDP_H.split("#include \"common.h\"\n")[1]
          .split("/* rdp.c")[0])
    d = json.loads(json.dumps(RDP_STATE))
    d["header"] = "include/trilogy/rdp_state.h"
    d["includes"] = ["common.h", "rdplist.h"]
    d["fields"][1]["comment"] = "queue view"
    doc = G.parse_layout(d, "rdp_state.json")
    text = G.generate(doc, "\n")
    assert text.index("typedef struct RdpSerialStateTri_guess") < text.index("typedef struct RdpState_guess")
    assert ("    union\n    {\n        RdpSerialQueue_guess serial;             /* 0x018* queue view */\n"
            "        RdpSerialStateTri_guess serialState_guess;  /* 0x018* */\n    };") in text
    assert "unknown0x068" not in text and "C_ASSERT(sizeof(RdpSerialStateTri_guess) == 0x094);" in text
    lay = G.verify(text, doc.header, doc, r, 0xAC)
    paths = {m.path: m.offset for m in lay.members}
    assert paths["serialState_guess.file"] == paths["serialState_guess.handle"] == 0x18 + 0x1C
    assert paths["serialState_guess.state"] == 0x18 + 0x90 and paths["serial.queued_bytes"] == 0x18 + 0x4C
    assert {m.path: m.offset for m in lay.aggregates}["serialState_guess.list"] == 0x18 + 0x38
    # a sub-struct whose nested member lands elsewhere than the layout says is caught
    bad = text.replace("RdpList_guess list;", "u32 extra; RdpList_guess list;", 1)
    with pytest.raises(G.LayoutError, match="list"):
        G.verify(bad, doc.header, doc, r, 0xAC)
    # check_nested on its own: the role's member paths are walked through the types struct
    lay2 = C.Layout(lay.name, lay.kind, lay.size, lay.align, lay.pack,
                    [m for m in lay.members if m.path != "serialState_guess.state"], lay.direct,
                    aggregates=lay.aggregates)
    with pytest.raises(G.LayoutError, match="serialState_guess.state missing"):
        G.check_nested(lay2, doc, doc.header)


def test_extend_inserts_types_and_wraps_by_value_member(rdp_repo, tmp_path):
    """Extend mode on the hand-written rdp.h: the missing types struct goes above the family's comment block, the
    by-value `serial` line is wrapped in a union with the role, the role's extra bytes come from the padding."""
    assert G.main([str(tmp_path / "rdp_state.json"), "--repo", str(rdp_repo), "--extend"]) == 0
    h = rd(rdp_repo / "include/trilogy/rdp.h")
    assert "\r\n" in h and "\n" not in h.replace("\r\n", "")
    i_type = h.index("typedef struct RdpSerialStateTri_guess")
    assert h.index("} RdpSerialQueue_guess;") < i_type < h.index("/* rdp.c: the rdp instance")
    assert ("    union\r\n    {\r\n        RdpSerialQueue_guess serial;    /* 0x18 */\r\n"
            "        RdpSerialStateTri_guess serialState_guess;  /* 0x018* */\r\n    };\r\n") in h
    assert "unknown068" not in h and "C_ASSERT(offsetof(RdpState_guess, serial) == 0x18);" in h
    assert "        void*   handle;" in h                              # the sub-struct's own union role
    # a second run is a no-op
    before = h
    assert G.main([str(tmp_path / "rdp_state.json"), "--repo", str(rdp_repo), "--extend"]) == 0
    assert rd(rdp_repo / "include/trilogy/rdp.h") == before


def test_extend_type_needing_a_later_struct_is_refused(rdp_repo, tmp_path):
    h = rd(rdp_repo / "include/trilogy/rdp.h")
    lst = h[h.index("typedef struct RdpList_guess"):h.index("} RdpList_guess;\r\n") + len("} RdpList_guess;\r\n")]
    h = h.replace(lst, "").replace("#endif\r\n", lst + "#endif\r\n")
    h = h.replace("    RdpList_guess list;     /* 0x38 */", "    u8  list[0x14];         /* 0x38 */")
    write(rdp_repo / "include/trilogy/rdp.h", h)
    doc = G.load_layout(tmp_path / "rdp_state.json")
    with pytest.raises(G.LayoutError, match="defines RdpList_guess at line .* move it up"):
        G.extend(h, "include/trilogy/rdp.h", doc, rdp_repo)


def test_extend_role_wider_than_member_needs_following_padding(rdp_repo, tmp_path):
    h = rd(rdp_repo / "include/trilogy/rdp.h").replace("    u8  unknown068[0x44];           /* 0x68 */\r\n", "")
    write(rdp_repo / "include/trilogy/rdp.h", h)
    d = json.loads(json.dumps(RDP_STATE))
    d["size"] = None
    doc = G.parse_layout(d, "x.json")
    with pytest.raises(G.LayoutError, match="no padding starts at 0x68"):
        G.extend(h, "include/trilogy/rdp.h", doc, rdp_repo)


def test_extend_refuses_overlay_instead_of_dropping_it(rdp_repo, tmp_path):
    d = json.loads(json.dumps(RDP_STATE))
    d["fields"].insert(1, {"offset": "0x4", "name": "a_guess", "type": "u32", "width": 4,
                           "overlay": [{"name": "pair_guess", "type": "u32", "count": 2, "width": 4}]})
    doc = G.parse_layout(d, "x.json")
    with pytest.raises(G.LayoutError, match="overlay"):
        G.extend(rd(rdp_repo / "include/trilogy/rdp.h"), "include/trilogy/rdp.h", doc, rdp_repo)


def test_propagate_through_struct_union_member(rdp_repo, tmp_path):
    assert G.main([str(tmp_path / "rdp_state.json"), "--repo", str(rdp_repo), "--extend"]) == 0
    cen = census(rdp_repo, "trilogy", tmp_path / "census")
    pr = P.Propagator(rdp_repo, [G.load_layout(tmp_path / "rdp_state.json")], P.load_census(cen), set())
    all_sites = [s for s in pr.collect() if s.func == "SerialUse_guess"]
    sites = {s.offset: s for s in all_sites if s.type != "int"}
    got = {o: (s.new or s.reason) for o, s in sites.items()}
    assert got[0x64] == "s->serial.queued_bytes"                   # primary view first
    assert got[0xA8] == "s->serialState_guess.state"               # only the role reaches 0x90 of the state
    assert got[0x34] == "s->serialState_guess.file"
    assert got[0x18] == "&s->serialState_guess"                    # whole struct-typed role
    assert got[0x50] == "&s->serial.list"                          # by-value struct inside the primary
    ints = [s for s in all_sites if s.type == "int"]
    assert [s.reason for s in ints] == ["type_differs"]


# ---------------------------------------------------------------------------------------- case 2: rdp_message

MSG_EQMAIN = {
    "family": "RdpMsgOutgoing_eqmain_guess", "client": "eqmain", "kind": "struct",
    "header": "include/eqmain/layout/rdp_message.h", "size": None, "pack": 8, "includes": ["eqmain/common.h"],
    "types": [{"name": "RdpOutgoingFlagBytes_guess", "size": "0x2", "fields": [
        {"offset": "0x1", "name": "flags", "type": "u8", "qualifiers": ["volatile"], "width": 1}]}],
    "fields": [
        {"offset": "0xC", "name": "unknown0C", "type": "u32", "width": 4},
        {"offset": "0x10", "name": "flagsWord_guess", "type": "unsigned short", "width": 2,
         "union": [{"name": "flagBytes_guess", "type": "struct RdpOutgoingFlagBytes_guess"}]},
        {"offset": "0x12", "name": "field12", "type": "u16", "width": 2},
        {"offset": "0x20", "name": "hdr_len", "type": "int", "width": 4},
        {"offset": "0x28", "name": "hdr", "type": "u8", "count": 1, "width": 1}],
    "copy_maps": {"src/eqmain/Network/rdp/msg_outgoing.c:3": {"flags": "flagBytes_guess.flags"}}}

MSG_SRC = """\
#include "eqmain/common.h"

typedef struct RdpMsgOutgoing_eqmain_guess
{
    u8  unknown00[0x11];    /* 0x00 */
    volatile u8 flags;      /* 0x11 */
    u16 field12;            /* 0x12 */
} RdpMsgOutgoing_eqmain_guess;

// FUNCTION: EQMAIN 0x10030f30
int msg_outgoing_init_guess(RdpMsgOutgoing_eqmain_guess *msg)
{
    if (msg->flags & 2)
        msg->field12 = 1;
    return msg->flags & 8;
}
"""

MSG_RAW = """\
#include "eqmain/common.h"
#include "eqmain/layout/rdp_message.h"

// FUNCTION: EQMAIN 0x10031000
int raw_guess(RdpMsgOutgoing_eqmain_guess *msg)
{
    *(u16*)((char*)msg + 0x10) = 0x4200;
    return *(u8*)((char*)msg + 0x11) & 2;
}
"""


@pytest.fixture
def msg_repo(tmp_path):
    r = tmp_path / "tree"
    write(r / "include/eqmain/common.h", COMMON)
    write(r / "src/eqmain/Network/rdp/msg_outgoing.c", MSG_SRC, crlf=True)
    (tmp_path / "rdp_message.json").write_text(json.dumps(MSG_EQMAIN), encoding="utf-8")
    assert G.main([str(tmp_path / "rdp_message.json"), "--repo", str(r)]) == 0
    return r


def test_generate_volatile_byte_in_struct_role(msg_repo):
    h = rd(msg_repo / "include/eqmain/layout/rdp_message.h")
    assert "    volatile u8 flags;              /* 0x001* */" in h
    assert ("    union\n    {\n        unsigned short flagsWord_guess;    /* 0x010* */\n"
            "        struct RdpOutgoingFlagBytes_guess flagBytes_guess;    /* 0x010* */\n    };") in h
    assert "u8      unknown0x000[1];" in h                        # padding inside the 2-byte view
    doc = G.load_layout(Path(msg_repo).parent / "rdp_message.json")
    lay = G.verify(h, doc.header, doc, msg_repo, None)
    m = {x.path: x for x in lay.members}
    assert m["flagBytes_guess.flags"].offset == 0x11 and m["flagBytes_guess.flags"].volatile
    assert not m["flagsWord_guess"].volatile and m["field12"].offset == 0x12


def test_qualifiers_spelled_in_type_and_bad_ones():
    t, q = G.split_quals("volatile const u8", ["volatile"])
    assert (t, q) == ("u8", "const volatile")
    d = json.loads(json.dumps(MSG_EQMAIN))
    d["types"][0]["fields"][0] = {"offset": "0x1", "name": "flags", "type": "volatile u8", "width": 1}
    doc = G.parse_layout(d, "x.json")
    assert doc.types[0].fields[0].ctype == "volatile u8" and doc.types[0].fields[0].type == "u8"
    d["fields"][1]["union"][0]["qualifiers"] = ["register"]
    with pytest.raises(G.LayoutError, match="only const / volatile"):
        G.parse_layout(d, "x.json")


def test_s5_unify_maps_volatile_flags_into_struct_role(msg_repo, tmp_path):
    cen = census(msg_repo, "eqmain", tmp_path / "census")
    lj = tmp_path / "rdp_message.json"
    rc = S5.main(["--repo", str(msg_repo), "--layout", str(lj), "--census", str(cen), "--apply"])
    assert rc == 0
    src = rd(msg_repo / "src/eqmain/Network/rdp/msg_outgoing.c")
    assert '#include "eqmain/layout/rdp_message.h"\r\n' in src and "volatile u8 flags;" not in src
    assert "if (msg->flagBytes_guess.flags & 2)" in src and "return msg->flagBytes_guess.flags & 8;" in src
    assert "msg->field12 = 1;" in src


def test_s5_unify_volatility_mismatch_needs_a_volatile_cast(msg_repo, tmp_path):
    d = json.loads(json.dumps(MSG_EQMAIN))
    del d["types"][0]["fields"][0]["qualifiers"]                   # the new role's flags is NOT volatile
    lj = tmp_path / "rdp_message.json"
    lj.write_text(json.dumps(d), encoding="utf-8")
    assert G.main([str(lj), "--repo", str(msg_repo)]) == 0
    cen = census(msg_repo, "eqmain", tmp_path / "census")
    assert S5.main(["--repo", str(msg_repo), "--layout", str(lj), "--census", str(cen), "--apply"]) == 1
    assert S5.main(["--repo", str(msg_repo), "--layout", str(lj), "--census", str(cen), "--apply",
                    "--allow-casts"]) == 0
    src = rd(msg_repo / "src/eqmain/Network/rdp/msg_outgoing.c")
    assert "if ((*(volatile u8*)&msg->flagBytes_guess.flags) & 2)" in src


def test_propagate_refuses_plain_access_to_volatile_role(msg_repo, tmp_path):
    write(msg_repo / "src/eqmain/Network/rdp/raw.c", MSG_RAW, crlf=True)
    cen = census(msg_repo, "eqmain", tmp_path / "census")
    pr = P.Propagator(msg_repo, [G.load_layout(tmp_path / "rdp_message.json")], P.load_census(cen), set(),
                      files=["src/eqmain/Network/rdp/raw.c"])
    got = {s.offset: (s.new or s.reason, s.detail) for s in pr.collect()}
    assert got[0x10][0] == "msg->flagsWord_guess"
    assert got[0x11][0] == "volatile" and "flagBytes_guess.flags" in got[0x11][1]


# ---------------------------------------------------------------------------------------- case 3: inflate

INFLATE = {
    "family": "inflate_blocks_state", "client": "eqmain", "kind": "struct",
    "header": "include/eqmain/layout/inflate_blocks_state.h", "size": "0x40", "pack": 8,
    "includes": ["eqmain/common.h"],
    "prelude": ["typedef unsigned int (__cdecl *InflateBlocksCheckFn_guess)(unsigned int, unsigned char *, "
                "unsigned int);"],
    "fields": [
        {"offset": "0x0", "name": "mode", "type": "unsigned int", "width": 4},
        {"offset": "0x34", "name": "write", "type": "unsigned char*", "width": 4,
         "union": [{"name": "tail", "type": "unsigned char*"}]},
        {"offset": "0x38", "name": "checkfn", "type": "InflateBlocksCheckFn_guess", "width": 4,
         "union": [{"name": "hook", "type": "InflateBlocksCheckFn_guess", "width": 4}]},
        {"offset": "0x3C", "name": "check", "type": "unsigned int", "width": 4,
         "union": [{"name": "hook_state", "type": "unsigned int"}]}]}


def test_generate_funcptr_typedef_role(tmp_path):
    r = tmp_path / "t"
    write(r / "include/eqmain/common.h", COMMON)
    doc = G.parse_layout(json.loads(json.dumps(INFLATE)), "inflate.json")
    text = G.generate(doc, "\n")
    assert "InflateBlocksCheckFn_guess hook;" in text
    lay = G.verify(text, doc.header, doc, r, 0x40)
    assert lay.direct["hook"][0] == 0x38 and lay.direct["hook_state"][0] == 0x3C


def test_struct_role_from_another_header_is_size_checked(tmp_path):
    r = tmp_path / "t"
    write(r / "include/eqmain/common.h", COMMON)
    write(r / "include/eqmain/layout/other.h", '#include "eqmain/common.h"\n'
          "typedef struct Other_guess { u32 a; u32 b; } Other_guess;\n")
    d = json.loads(json.dumps(INFLATE))
    d["size"] = None
    d["includes"] = ["eqmain/common.h", "other.h"]
    d["fields"][-1]["union"] = [{"name": "other_guess", "type": "Other_guess"}]   # no width: 4 assumed
    doc = G.parse_layout(d, "x.json")
    with pytest.raises(G.LayoutError, match="other_guess .* is 0x8 bytes"):
        G.verify(G.generate(doc, "\n"), doc.header, doc, r, None)
    d["fields"][-1]["union"][0]["width"] = 8
    doc = G.parse_layout(d, "x.json")
    G.verify(G.generate(doc, "\n"), doc.header, doc, r, None)


# ---------------------------------------------------------------------------------------- named overlay

def test_named_overlay_struct_view_spans_following_fields(tmp_path):
    """An `overlay` names a sub-struct view over this field and the following ones (union { struct {...};
    view; }): members inside the view are reachable by dotted path, the plain fields keep their names."""
    r = tmp_path / "t"
    write(r / "include/eqmain/common.h", COMMON)
    d = {"family": "Ov_guess", "client": "eqmain", "kind": "struct", "header": "include/eqmain/layout/ov.h",
         "size": "0x10", "pack": 8, "includes": ["eqmain/common.h"],
         "types": [{"name": "Pair_guess", "size": "0x8", "fields": [
             {"offset": "0x0", "name": "lo", "type": "u32", "width": 4},
             {"offset": "0x4", "name": "hi", "type": "u32", "qualifiers": ["volatile"], "width": 4}]}],
         "fields": [{"offset": "0x4", "name": "a", "type": "u32", "width": 4,
                     "overlay": [{"name": "pair_guess", "type": "Pair_guess"}]},
                    {"offset": "0x8", "name": "b", "type": "u16", "width": 2}]}
    doc = G.parse_layout(d, "ov.json")
    assert doc.fields[0].overlay[0].width == 8
    text = G.generate(doc, "\n")
    lay = G.verify(text, doc.header, doc, r, 0x10)
    m = {x.path: x for x in lay.members}
    assert m["pair_guess.hi"].offset == 8 and m["pair_guess.hi"].volatile and m["b"].offset == 8


# ---------------------------------------------------------------------------------------- VC6

def _vc6():
    try:
        from eqd_unicorn import Workspace
        vc6 = Workspace(ROOT).vc6("trilogy")
    except Exception:  # noqa: BLE001
        return None
    return vc6 if (vc6 / "Bin" / "CL.EXE").is_file() else None


USE_C = """\
#include "trilogy/rdp.h"
#include "eqmain/layout/rdp_message.h"
#include "eqmain/layout/ov.h"

int use(RdpState_guess *s, RdpMsgOutgoing_eqmain_guess *m, Ov_guess *o)
{
    s->serialState_guess.file = 0;
    s->serialState_guess.handle = s->serial.list.head;
    s->serialState_guess.state = s->serial.queued_bytes;
    m->flagsWord_guess = 0x4200;
    o->pair_guess.lo = o->a;
    return (m->flagBytes_guess.flags & 2) + (int)sizeof(RdpSerialStateTri_guess) + o->b;
}
"""


@pytest.mark.parametrize("lang", ["c", "cpp"])
def test_vc6_compiles_generated_struct_unions(rdp_repo, tmp_path, lang):
    vc6 = _vc6()
    if vc6 is None:
        pytest.skip("VC6 not found (paths.local.ini [toolchain] vc6)")
    from eqd_unicorn.vc6 import vc6_env
    assert G.main([str(tmp_path / "rdp_state.json"), "--repo", str(rdp_repo), "--extend"]) == 0
    write(rdp_repo / "include/eqmain/common.h", COMMON)
    lj = tmp_path / "rdp_message.json"
    lj.write_text(json.dumps(MSG_EQMAIN), encoding="utf-8")
    assert G.main([str(lj), "--repo", str(rdp_repo)]) == 0
    ov = {"family": "Ov_guess", "client": "eqmain", "kind": "struct", "header": "include/eqmain/layout/ov.h",
          "size": "0x10", "pack": 8, "includes": ["eqmain/common.h"],
          "types": [{"name": "Pair_guess", "size": "0x8", "fields": [
              {"offset": "0x0", "name": "lo", "type": "u32", "width": 4},
              {"offset": "0x4", "name": "hi", "type": "volatile u32", "width": 4}]}],
          "fields": [{"offset": "0x4", "name": "a", "type": "u32", "width": 4,
                      "overlay": [{"name": "pair_guess", "type": "Pair_guess"}]},
                     {"offset": "0x8", "name": "b", "type": "u16", "width": 2}]}
    oj = tmp_path / "ov.json"
    oj.write_text(json.dumps(ov), encoding="utf-8")
    assert G.main([str(oj), "--repo", str(rdp_repo)]) == 0
    src = tmp_path / f"use.{lang}"
    src.write_text(USE_C, encoding="utf-8")
    inc = rdp_repo / "include"
    cmd = [str(vc6 / "Bin" / "CL.EXE"), "/nologo", "/c", "/W3", "/WX", f"/I{inc}", f"/I{inc / 'trilogy'}",
           f"/Fo{tmp_path / 'use.obj'}", str(src)]
    p = subprocess.run(cmd, env=vc6_env(vc6), capture_output=True, text=True, errors="replace", cwd=tmp_path)
    assert p.returncode == 0, p.stdout + p.stderr
    assert (tmp_path / "use.obj").is_file()
