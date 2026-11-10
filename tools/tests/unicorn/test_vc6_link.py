"""Self-test for vc6.build_dll: a /NOENTRY test DLL must link whatever LIBC and Win32 pieces a rewrite uses.

Needs VC6 (skips without it); no game files. Regression for: float code (__fltused), stdio (sprintf ->
_flsbuf -> __amsg_exit, which LIBC defines in the EXE start-up crt0.obj -> unresolved _main) and Win32 /
LIBC imports (Sleep, MultiByteToWideChar behind _strcmpi) with no CRT start-up object naming kernel32.lib.
"""
import os
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn.pe import load_pe, read_map  # noqa: E402
from eqd_unicorn.vc6 import DEFAULT_VC6, build_dll  # noqa: E402

SRC = r"""
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern "C" float scale(float a, float b) { return a * 0.75f + b; }
extern "C" int fmt(char *out, unsigned short id) { return sprintf(out, "%s%u", "x", id); }
extern "C" int same(const char *a, const char *b) { return _strcmpi(a, b) == 0 && isspace(' '); }
extern "C" void nap(void) { Sleep(1); }
"""


def test_crt_float_stdio_and_win32_link(tmp_path):
    vc6 = Path(os.environ.get("EQD_VC6") or DEFAULT_VC6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip(f"VC6 not found at {vc6}")
    src = tmp_path / "linkcheck.cpp"
    src.write_text(SRC)
    dll, mapf = build_dll("linkcheck", [src], tmp_path / "out", vc6=vc6)
    img = load_pe(dll, base=0x30000000)
    read_map(mapf, img)
    names = set(img.by_name)
    assert {"_scale", "_fmt", "_same", "_nap"} <= names
    assert "_main" not in names
    assert "kernel32.dll!Sleep" in set(img.imports.values())


# --- per-source CFLAGS lines ---------------------------------------------------------------------------------

from eqd_unicorn.vc6 import DEFAULT_CFLAGS, BuildError, cflags_for, source_cflags  # noqa: E402


def test_cflags_lines_are_parsed_in_order(tmp_path):
    src = tmp_path / "a.c"
    src.write_text("/* rdp.\n * CFLAGS: /O2\n */\n// CFLAGS: /Oy- /GX-\n/* CFLAGS: /Zp4 */\nint x;\n"
                   "// not a CFLAGS: line because the marker must start the comment\n")
    assert source_cflags(src) == ["/O2", "/Oy-", "/GX-", "/Zp4"]
    flags = cflags_for(src)
    assert "/O1" not in flags and flags[-4:] == ["/O2", "/Oy-", "/GX-", "/Zp4"]
    assert flags[:-4] == [f for f in DEFAULT_CFLAGS if f != "/O1"]


def test_cflags_append_without_level(tmp_path):
    src = tmp_path / "b.cpp"
    src.write_text("// CFLAGS: /Ot\nint y;\n")
    assert cflags_for(src) == DEFAULT_CFLAGS + ["/Ot"]
    plain = tmp_path / "c.cpp"
    plain.write_text("int z;\n")
    assert cflags_for(plain) == DEFAULT_CFLAGS
    assert "/GX" in DEFAULT_CFLAGS


@pytest.mark.parametrize("bad", ["/MD", "/DFOO", "/Iinc", "/FAs", "-O2"])
def test_cflags_refuses_non_codegen_options(tmp_path, bad):
    src = tmp_path / "d.c"
    src.write_text(f"// CFLAGS: {bad}\n")
    with pytest.raises(BuildError, match="not allow"):
        source_cflags(src)


def test_build_dll_compiles_each_source_with_its_flags(tmp_path):
    """Two sources in one DLL: the /Od one keeps an EBP frame, the default /O1 one doesn't."""
    vc6 = Path(os.environ.get("EQD_VC6") or DEFAULT_VC6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip(f"VC6 not found at {vc6}")
    body = "extern \"C\" int {n}(int a, int b) {{ return a * b + 7; }}\n"
    (tmp_path / "fast.cpp").write_text(body.format(n="fast"))
    (tmp_path / "slow.cpp").write_text("// CFLAGS: /Od\n" + body.format(n="slow"))
    dll, mapf = build_dll("flagcheck", [tmp_path / "fast.cpp", tmp_path / "slow.cpp"], tmp_path / "out", vc6=vc6)
    img = load_pe(dll, base=0x30000000)
    read_map(mapf, img)
    assert img.read(img.by_name["_slow"], 3) == b"\x55\x8b\xec"         # push ebp; mov ebp, esp
    assert img.read(img.by_name["_fast"], 1) != b"\x55"


def test_build_dll_compiles_each_source_with_its_toolchain(tmp_path, monkeypatch):
    """A `// TOOLCHAIN:` source gets its own cl run with that toolchain's CL.EXE; the link stays the build's."""
    from eqd_unicorn import vc6 as v
    own, other = tmp_path / "own", tmp_path / "other"
    for d in (own, other):
        (d / "Bin").mkdir(parents=True)
        (d / "Bin" / "CL.EXE").write_bytes(b"")
    monkeypatch.setattr(v, "_TOOLCHAIN_DIRS", {})
    monkeypatch.setenv("EQD_TOOLCHAIN_OTHER", str(other))
    (tmp_path / "a.cpp").write_text("int a;\n")
    (tmp_path / "b.cpp").write_text("// TOOLCHAIN: other\nint b;\n")
    (tmp_path / "c.cpp").write_text("int c;\n")
    runs = []

    def fake_run(cmd, env, cwd):
        runs.append((Path(cmd[0]), [Path(x).name for x in cmd if x.endswith(".cpp")], env["PATH"]))
        if Path(cmd[0]).name == "LINK.EXE":
            raise v.BuildError("stop")
        return ""
    monkeypatch.setattr(v, "_run", fake_run)
    # fake cl writes no .obj files, so skip the EQ_METHOD stub scan that reads them
    monkeypatch.setattr(v.thiscall_stubs, "write_stub_obj", lambda objs, out, extra=None: None)
    with pytest.raises(v.BuildError):
        build_dll("tc", [tmp_path / n for n in ("a.cpp", "b.cpp", "c.cpp")], tmp_path / "out", vc6=own,
                  cache_dir=False)
    cl = [(exe.parent.parent, srcs, path) for exe, srcs, path in runs if exe.name == "CL.EXE" and srcs]
    assert cl == [(own, ["a.cpp", "c.cpp"], cl[0][2]), (other, ["b.cpp"], cl[1][2])]
    assert cl[1][2].startswith(str(other / "Bin"))
    assert runs[-1][0] == own / "Bin" / "LINK.EXE"
