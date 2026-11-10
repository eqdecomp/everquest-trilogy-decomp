"""`// EXTERNAL:` markers (eqd_unicorn/externs.py): a function a source declares but does not define, at a client VA.
Marker scanning and resolution against an object's undefined externals (no VC6); a VC6 test DLL links a `jmp va`
stub for the declared constructor (skips without VC6)."""
import os
import struct
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn import externs  # noqa: E402
from eqd_unicorn.pe import Image  # noqa: E402

CTOR = "??0Item_guess@@QAE@XZ"
CTOR_INT = "??0Item_guess@@QAE@H@Z"


def write(path: Path, text: str) -> Path:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")
    return path


def test_scan_reads_client_va_and_name_with_the_files_defines(tmp_path):
    src = write(tmp_path / "a.cpp", "#define Item Item_guess\n"
                "class Item\n{\npublic:\n    // EXTERNAL: TRILOGY 0x004A02E5 Item::Item\n    Item(void);\n};\n"
                "  //EXTERNAL: eqgfx 0x10001000 ??0Other@@QAE@XZ\n"
                "// EXTERNAL: TRILOGY 0x00401000\n")          # no name: not a marker
    marks = externs.scan([src])
    assert [(e.client, e.va, e.name, e.line) for e in marks] == [
        ("TRILOGY", 0x004A02E5, "Item_guess::Item_guess", 5), ("EQGFX", 0x10001000, "??0Other@@QAE@XZ", 8)]
    assert [e.name for e in externs.scan([src], "trilogy")] == ["Item_guess::Item_guess"]


def mark(name, va, path="a.cpp", line=1):
    return externs.External("TRILOGY", va, name, path, line)


def test_resolve_maps_undefined_externals_by_name_or_decorated_symbol():
    names = {CTOR, "?Run@Item_guess@@QAEXXZ", "_free"}
    m, problems = externs.resolve(names, [mark("Item_guess::Item_guess", 0x4A02E5),
                                          mark("Item_guess::Run", 0x4A0400)])
    assert m == {CTOR: 0x4A02E5, "?Run@Item_guess@@QAEXXZ": 0x4A0400} and not problems
    # several referenced overloads of one name: ambiguous unless the marker gives the decorated name
    m, problems = externs.resolve({CTOR, CTOR_INT}, [mark("Item_guess::Item_guess", 0x4A02E5)])
    assert m == {} and "2 referenced overloads" in problems[0]
    m, problems = externs.resolve({CTOR, CTOR_INT}, [mark(CTOR_INT, 0x4A0300)])
    assert m == {CTOR_INT: 0x4A0300} and not problems


def test_resolve_own_markers_win_and_disagreeing_markers_cancel():
    hdr = [mark("Item_guess::Item_guess", 0x4A0000, "item.h")]
    m, _ = externs.resolve({CTOR}, [mark("Item_guess::Item_guess", 0x4A02E5)], hdr)
    assert m == {CTOR: 0x4A02E5}
    m, _ = externs.resolve({CTOR}, [], hdr)
    assert m == {CTOR: 0x4A0000}
    m, problems = externs.resolve({CTOR}, [mark("Item_guess::Item_guess", 1), mark("Item_guess::Item_guess", 2)])
    assert m == {} and "disagree" in problems[0]


def test_header_markers_and_marked(tmp_path):
    write(tmp_path / "inc" / "trilogy" / "item.h", "// EXTERNAL: TRILOGY 0x004A02E5 Item_guess::Item_guess\n")
    write(tmp_path / "inc" / "trilogy" / "other.txt", "// EXTERNAL: TRILOGY 0x00401000 Other::Other\n")
    marks = externs.header_markers([tmp_path / "inc"], "TRILOGY")
    assert [e.name for e in marks] == ["Item_guess::Item_guess"]
    assert externs.header_markers([tmp_path / "inc"], "EQGFX") == []
    assert externs.marked(CTOR, marks) and externs.marked(CTOR_INT, marks)
    assert not externs.marked("??0Other@@QAE@XZ", marks)


def test_stub_target_reads_the_generated_jump():
    img = Image.from_bytes("dll", 0x30000000, b"\xB8\xE5\x02\x4A\x00\xFF\xE0\x90" + b"\x55\x8B\xEC\xC3" + b"\0" * 8)
    assert externs.stub_target(img.read, 0x30000000) == 0x004A02E5
    assert externs.stub_target(img.read, 0x30000008) is None


# --- VC6: the test DLL links a stub for the declared constructor ------------------------------------------------

SRC = """\
class Item_guess
{
public:
    // EXTERNAL: TRILOGY 0x004A02E5 Item_guess::Item_guess
    Item_guess(void);
    int value;
};

extern "C" Item_guess *make_item(void)
{
    return new Item_guess;
}
"""


def vc6_or_skip():
    from eqd_unicorn.vc6 import DEFAULT_VC6
    vc6 = Path(os.environ.get("EQD_VC6") or DEFAULT_VC6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        try:
            from eqd_unicorn import Workspace
            vc6 = Workspace().vc6("trilogy")
        except Exception:  # noqa: BLE001
            pass
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip(f"VC6 not found at {vc6}")
    return vc6


def test_build_dll_links_a_jump_stub_for_the_declared_constructor(tmp_path):
    from eqd_unicorn.pe import load_pe, read_map
    from eqd_unicorn.vc6 import BuildError, build_dll
    vc6 = vc6_or_skip()
    src = write(tmp_path / "item.cpp", SRC)
    dll, mapf = build_dll("externs", [src], tmp_path / "out", vc6=vc6, cache_dir=False)
    img = load_pe(dll, base=0x30000000)
    read_map(mapf, img)
    at = img.by_name[CTOR]
    assert externs.stub_target(img.read, at) == 0x004A02E5
    # the caller calls the declared constructor directly (call rel32 to the stub)
    make = img.by_name["_make_item"]
    code = img.read(make, 0x40)
    calls = [make + i + 5 + struct.unpack_from("<i", code, i + 1)[0] for i in range(len(code) - 5) if code[i] == 0xE8]
    assert at in calls, [hex(c) for c in calls]
    # without the marker the constructor is unresolved
    write(tmp_path / "plain" / "item.cpp", SRC.replace("// EXTERNAL:", "// external:"))
    with pytest.raises(BuildError):
        build_dll("externs_plain", [tmp_path / "plain" / "item.cpp"], tmp_path / "out", vc6=vc6, cache_dir=False)


VFTABLE_SRC = """\
class Slots_guess
{
public:
    virtual void slot0_guess() = 0;
    // EXTERNAL: TRILOGY 0x00535430 ??_7Slots_guess@@6B@
    ~Slots_guess();
    int value;
};

Slots_guess::~Slots_guess()
{
    value = 0;
}
"""


def test_build_dll_stores_the_marked_vftable_unrelocated(tmp_path):
    from eqd_unicorn.pe import load_pe, read_map
    from eqd_unicorn.vc6 import build_dll
    vc6 = vc6_or_skip()
    src = write(tmp_path / "slots.cpp", VFTABLE_SRC)
    dll, mapf = build_dll("externs_vft", [src], tmp_path / "out", vc6=vc6, cache_dir=False)
    img = load_pe(dll, base=0x30000000)                 # not the link base: a relocated field would move
    read_map(mapf, img)
    code = img.read(img.by_name["??1Slots_guess@@QAE@XZ"], 0x20)
    assert b"\xC7\x01" + struct.pack("<I", 0x00535430) in code, code.hex()
