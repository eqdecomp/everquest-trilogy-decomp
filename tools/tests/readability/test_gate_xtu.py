"""gate.py: a call to a function another TU defines and annotates is keyed by that VA (compare/xtu.py), on tiny
VC6-built C/C++ TUs (skips without VC6): the EQ_Bag case (a derived constructor whose TU-local stand-in base calls
the real base constructor/destructor by address, rewritten as the layout class that calls them by symbol) passes,
and the keying hides nothing: a definition annotated at another VA, an unannotated definition, an unannotated
overload and a definition no TU has all fail. obj_signatures' thunk and label keys are tested on hand-made COFF."""
import struct
import subprocess

import pytest

import gate

EQADDR = """\
#ifndef TRILOGY_EQADDR_H
#define TRILOGY_EQADDR_H
#ifdef EQD_MATCH
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)eqd_fn_##va)
#else
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)(va))
#endif
#define EQ_CDECL(ret, va, params)           EQ_FUNC(ret, __cdecl, va, params)
#ifdef __cplusplus
template<class R> struct eqd_ret { typedef R type; };
template<> struct eqd_ret<void> { typedef int type; };
template<unsigned VA, class R, class T> struct eqd_tc0 {
    typename eqd_ret<R>::type m();
};
#define EQD_P0(t) t
#define EQ_METHOD(n, ret, va, params, obj) ((eqd_tc##n<va, ret, EQD_P##n params> *)(obj))->m
#endif
#endif
"""

OVERLAY_H = """\
#ifndef OVERLAY_H
#define OVERLAY_H
class Overlay_guess
{
public:
    int state;
    int handle;
    Overlay_guess();
    Overlay_guess(int mode);
    ~Overlay_guess();
};

class Bag_guess : public Overlay_guess
{
public:
    int slot;
    Bag_guess(int unused);
};
#endif
"""

OVERLAY_CPP = """\
#include "trilogy/eqaddr.h"
#include "trilogy/overlay.h"

// FUNCTION: TRILOGY 0x{ctor}
Overlay_guess::Overlay_guess()
{{
    state = 1;
    handle = 0;
}}

// FUNCTION: TRILOGY 0x0045005d
Overlay_guess::~Overlay_guess()
{{
    EQ_CDECL(void, 0x00450463, (int))(handle);
}}
"""

OVERLAY_MODE_CPP = """\
#include "trilogy/overlay.h"

Overlay_guess::Overlay_guess(int mode)
{
    state = mode;
    handle = 0;
}
"""

BAG_STANDIN = """\
#include "trilogy/eqaddr.h"
#include "trilogy/overlay.h"

class BagOverlayBase_guess
{
public:
    BagOverlayBase_guess()
    {
        EQ_METHOD(0, void, 0x00450049, (void*), this)();
    }

    ~BagOverlayBase_guess()
    {
        EQ_METHOD(0, void, 0x0045005D, (void*), this)();
    }
};

class BagCtor_guess : public BagOverlayBase_guess
{
public:
    BagCtor_guess(int unused);
};

// FUNCTION: TRILOGY 0x00416d0c
BagCtor_guess::BagCtor_guess(int unused)
{
    *(int*)((char*)this + 0x8) = unused;
    EQ_METHOD(0, void, 0x00401234, (void*), this)();     // a C++ call: may throw, so the base unwinds
}
"""

BAG_REAL = """\
#include "trilogy/eqaddr.h"
#include "trilogy/overlay.h"

// FUNCTION: TRILOGY 0x00416d0c
Bag_guess::Bag_guess(int unused){init}
{{
    *(int*)((char*)this + 0x8) = unused;
    EQ_METHOD(0, void, 0x00401234, (void*), this)();     // a C++ call: may throw, so the base unwinds
}}
"""

SCALE_C = """\
// FUNCTION: TRILOGY 0x00403000
int Scale_guess(int v)
{
    return v * 3;
}
"""

USER_C = """\
#include "trilogy/eqaddr.h"
int Scale_guess(int v);

// FUNCTION: TRILOGY 0x00404000
int Use_guess(int x)
{
    return CALL(x) + 1;
}
"""


def _vc6_or_skip():
    try:
        cl = gate.Workspace().vc6("trilogy") / "Bin" / "CL.EXE"
    except FileNotFoundError as e:
        pytest.skip(str(e))
    if not cl.is_file():
        pytest.skip(f"VC6 not found at {cl}")


def make_repo(root, ctor_va="00450049", overlay=True, mode_tu=False):
    files = {
        "include/trilogy/eqaddr.h": EQADDR,
        "include/trilogy/overlay.h": OVERLAY_H,
        "src/EverQuest/bag.cpp": BAG_STANDIN,
        "src/EverQuest/scale.c": SCALE_C,
        "src/EverQuest/user.c": USER_C.replace("CALL(x)", "EQ_CDECL(int, 0x00403000, (int))(x)"),
        "tests/unicorn/README.txt": "fixtures\n",
    }
    if overlay:
        files["src/EverQuest/overlay.cpp"] = OVERLAY_CPP.format(ctor=ctor_va)
    if mode_tu:
        files["src/EverQuest/overlay_mode.cpp"] = OVERLAY_MODE_CPP
    for rel, text in files.items():
        p = root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8")
    for cmd in (["init", "-q"], ["config", "user.email", "t@example.com"], ["config", "user.name", "t"],
                ["config", "core.autocrlf", "false"], ["add", "-A"], ["commit", "-qm", "base"]):
        subprocess.run(["git", *cmd], cwd=root, check=True, capture_output=True)
    return root


def real_form(root, init=""):
    (root / "src/EverQuest/bag.cpp").write_text(BAG_REAL.format(init=init), encoding="utf-8")


def run(repo, tmp_path, files):
    return gate.run_gate(repo, "HEAD", files, cache=tmp_path / "cache", jobs=2)


def bag(r):
    (t,) = [t for t in r.tus if t.rel == "src/EverQuest/bag.cpp"]
    return t


def test_real_form_ctor_passes(tmp_path):
    """The EQ_Bag case: base ctor/dtor by symbol (another TU, annotated) == the stand-in's calls by address; the
    stand-in's inline destructor (a COMDAT `jmp` thunk the unwind funclet called) is seen through."""
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r")
    real_form(repo)
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert r.ok, r.report()
    notes = " ".join(bag(r).notes)
    assert "??0Overlay_guess@@QAE@XZ=00450049" in notes and "??1Overlay_guess@@QAE@XZ=0045005D" in notes, notes
    assert "??1BagOverlayBase_guess@@QAE@XZ" in notes, notes


def test_without_xtu_keying_the_real_form_fails(tmp_path, monkeypatch):
    """Control: the same edit with the cross-TU maps switched off fails (what S5 hit)."""
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r")
    real_form(repo)
    monkeypatch.setattr(gate, "cross_tu_maps", lambda *a, **k: {})
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert not r.ok and any(k == "00416D0C" for k, _n, _w in bag(r).changed), r.report()


def test_definition_at_another_va_fails(tmp_path):
    """The base ctor's definition is annotated at 0x00450050, the stand-in called 0x00450049: a difference."""
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r", ctor_va="00450050")
    real_form(repo)
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert not r.ok and any(k == "00416D0C" for k, _n, _w in bag(r).changed), r.report()
    assert "??0Overlay_guess@@QAE@XZ=00450050" in " ".join(bag(r).notes)


def test_unannotated_definition_fails(tmp_path):
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r")
    p = repo / "src/EverQuest/overlay.cpp"
    p.write_text(p.read_text(encoding="utf-8").replace("// FUNCTION: TRILOGY 0x00450049\n", ""), encoding="utf-8")
    subprocess.run(["git", "commit", "-qam", "no annotation"], cwd=repo, check=True, capture_output=True)
    real_form(repo)
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert not r.ok and any(k == "00416D0C" for k, _n, _w in bag(r).changed), r.report()


def test_no_defining_tu_fails(tmp_path):
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r", overlay=False)
    real_form(repo)
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert not r.ok and any(k == "00416D0C" for k, _n, _w in bag(r).changed), r.report()


def test_unannotated_overload_is_not_keyed(tmp_path):
    """`Overlay_guess(int)` (another TU, no annotation) shares the undecorated name of the annotated default ctor:
    a call to it keeps its symbol and fails."""
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r", mode_tu=True)
    real_form(repo, init=" : Overlay_guess(0)")
    r = run(repo, tmp_path, ["src/EverQuest/bag.cpp"])
    assert not r.ok and any(k == "00416D0C" for k, _n, _w in bag(r).changed), r.report()
    assert "??0Overlay_guess@@QAE@H@Z" not in " ".join(bag(r).notes)


def test_c_call_by_address_to_call_by_name(tmp_path):
    """C: `EQ_CDECL(int, 0x00403000, ...)(x)` -> `Scale_guess(x)` (scale.c annotates 0x00403000) passes;
    the same call to a VA the definition does not carry fails."""
    _vc6_or_skip()
    repo = make_repo(tmp_path / "r")
    p = repo / "src/EverQuest/user.c"
    p.write_text(USER_C.replace("CALL(x)", "Scale_guess(x)"), encoding="utf-8")
    r = run(repo, tmp_path, ["src/EverQuest/user.c"])
    assert r.ok, r.report()
    assert "_Scale_guess=00403000" in " ".join(r.tus[0].notes), r.report()

    repo2 = make_repo(tmp_path / "r2")
    p2 = repo2 / "src/EverQuest/user.c"
    p2.write_text(USER_C.replace("CALL(x)", "EQ_CDECL(int, 0x00403010, (int))(x)"), encoding="utf-8")
    subprocess.run(["git", "commit", "-qam", "other va"], cwd=repo2, check=True, capture_output=True)
    p2.write_text(USER_C.replace("CALL(x)", "Scale_guess(x)"), encoding="utf-8")
    r2 = run(repo2, tmp_path / "c2", ["src/EverQuest/user.c"])
    assert not r2.ok and r2.tus[0].changed, r2.report()


# --- obj_signatures on hand-made COFF: thunks and labels -----------------------------------------------------------

TEXT = 0x60000020
COMDAT = 0x1000


def make_obj(path, sections, symbols):
    """sections: [(name, data, chars, [(offset, symbol index, type)])]; symbols: [(name, value, section, type,
    storage)] (no aux records)."""
    nsec = len(sections)
    pos = 20 + 40 * nsec
    blobs, offs = [], []
    for name, data, chars, relocs in sections:
        rel = b"".join(struct.pack("<IIH", o, i, t) for o, i, t in relocs)
        offs.append((pos, pos + len(data) if relocs else 0))
        blobs.append(data + rel)
        pos += len(data) + len(rel)
    strtab = bytearray(4)
    syms = b""
    for name, value, sec, typ, storage in symbols:
        nb = name.encode()
        raw = nb.ljust(8, b"\0") if len(nb) <= 8 else b"\0\0\0\0" + struct.pack("<I", len(strtab))
        if len(nb) > 8:
            strtab += nb + b"\0"
        syms += raw + struct.pack("<IhHBB", value, sec, typ, storage, 0)
    struct.pack_into("<I", strtab, 0, len(strtab))
    out = struct.pack("<HHIIIHH", 0x14C, nsec, 0, pos, len(symbols), 0, 0)
    for (name, data, chars, relocs), (praw, prel) in zip(sections, offs):
        out += struct.pack("<8sIIIIIIHHI", name.encode(), 0, 0, len(data), praw, prel, 0, len(relocs), 0, chars)
    out += b"".join(blobs) + syms + bytes(strtab)
    path.write_bytes(out)
    return path


CALLER = b"\xe8\0\0\0\0\xc3"             # call rel32; ret
JMP = b"\xe9\0\0\0\0"


def sigs(tmp_path, name, sections, symbols, ann=None, xtu=None):
    info = {}
    fn, data, _n = gate.obj_signatures(make_obj(tmp_path / name, sections, symbols), ann or {"caller": 0x401000},
                                       {}, None, xtu, info)
    return fn, data, info


def test_call_through_comdat_thunk_equals_direct_call(tmp_path):
    direct = sigs(tmp_path, "d.obj", [(".text", CALLER, TEXT, [(1, 1, 0x14)])],
                  [("_caller", 0, 1, 0x20, 2), ("_eqd_fn_0x0045005D", 0, 0, 0x20, 2)])
    thunk_secs = [(".text", CALLER, TEXT, [(1, 1, 0x14)]), (".text", JMP, TEXT | COMDAT, [(1, 2, 0x14)])]
    thunk_syms = [("_caller", 0, 1, 0x20, 2), ("??1Base@@QAE@XZ", 0, 2, 0x20, 2), ("_eqd_fn_0x0045005D", 0, 0, 0x20, 2)]
    via = sigs(tmp_path, "t.obj", thunk_secs, thunk_syms)
    assert direct[0] == via[0] == {"00401000": direct[0]["00401000"]} and via[2]["thunks"] == {"??1Base@@QAE@XZ"}
    # not a COMDAT: a function of its own, compared as one
    plain = sigs(tmp_path, "p.obj", [thunk_secs[0], (".text", JMP, TEXT, [(1, 2, 0x14)])], thunk_syms)
    assert plain[0]["00401000"] != direct[0]["00401000"] and not plain[2]["thunks"]
    # annotated: a function of its own
    ann = sigs(tmp_path, "a.obj", thunk_secs, thunk_syms, ann={"caller": 0x401000, "Base::~Base": 0x401100})
    assert ann[0]["00401000"] != direct[0]["00401000"] and "00401100" in ann[0] and not ann[2]["thunks"]
    # address taken (DIR32): a function pointer, not seen through
    taken = sigs(tmp_path, "x.obj", thunk_secs + [(".data", bytes(4), 0xC0000040, [(0, 1, 0x06)])], thunk_syms)
    assert not taken[2]["thunks"] and taken[0]["00401000"] != direct[0]["00401000"]


def test_xtu_map_keys_external_by_va(tmp_path):
    secs = [(".text", CALLER, TEXT, [(1, 1, 0x14)])]
    by_va = sigs(tmp_path, "v.obj", secs, [("_caller", 0, 1, 0x20, 2), ("_eqd_fn_0x00450049", 0, 0, 0x20, 2)])
    sym = [("_caller", 0, 1, 0x20, 2), ("??0Base@@QAE@XZ", 0, 0, 0x20, 2)]
    assert sigs(tmp_path, "s.obj", secs, sym, xtu={"??0Base@@QAE@XZ": 0x450049})[0] == by_va[0]
    assert sigs(tmp_path, "s2.obj", secs, sym, xtu={"??0Base@@QAE@XZ": 0x450050})[0] != by_va[0]
    assert sigs(tmp_path, "s3.obj", secs, sym, xtu={"??0Base@@QAE@H@Z": 0x450049})[0] != by_va[0]
    assert sigs(tmp_path, "s4.obj", secs, sym)[0] != by_va[0]


def test_label_keys_do_not_shift_with_an_extra_comdat(tmp_path):
    """A switch-table label is keyed by its function, not by the section ordinal: a COMDAT that appears in front
    (here a thunk) does not change the function's signature."""
    table = b"\xff\x24\x85\0\0\0\0" + b"\xc3" + bytes(4)        # jmp [table*4]; ret; table: dd label
    sw = (".text", table, TEXT, [(3, 1, 0x06), (8, 2, 0x06)])
    syms = [("_caller", 0, 0, 0x20, 2), ("$L1", 8, 0, 0, 6), ("$L2", 7, 0, 0, 6)]

    def build(name, extra):
        secs = extra + [sw]
        n = len(secs)
        s = [(nm, v, n, t, st) for nm, v, _sec, t, st in syms]
        if extra:
            s += [("??1Junk@@QAE@XZ", 0, 1, 0x20, 2), ("_eqd_fn_0x00450049", 0, 0, 0x20, 2)]
            secs[0] = (secs[0][0], secs[0][1], secs[0][2], [(1, len(s) - 1, 0x14)])
        return sigs(tmp_path, name, secs, s)[0]
    assert build("n.obj", []) == build("e.obj", [(".text", JMP, TEXT | COMDAT, [])])
