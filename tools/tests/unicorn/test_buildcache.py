"""Test-DLL cache (eqd_unicorn.buildcache via vc6.build_dll): hits, invalidation by every input, parallel
builds, maintenance. The VC6 tests skip without VC6; no game files."""
import os
import subprocess
import sys
import textwrap
from pathlib import Path

import pytest

UNICORN = Path(__file__).resolve().parents[2] / "unicorn"
sys.path.insert(0, str(UNICORN))

from eqd_unicorn import buildcache  # noqa: E402
from eqd_unicorn.pe import load_pe, read_map  # noqa: E402
from eqd_unicorn.vc6 import DEFAULT_VC6, build_dll  # noqa: E402

VC6 = Path(os.environ.get("EQD_VC6") or DEFAULT_VC6)
needs_vc6 = pytest.mark.skipif(not (VC6 / "Bin" / "CL.EXE").is_file(), reason=f"VC6 not found at {VC6}")


def test_headers_from_preprocessor_parses_line_directives(tmp_path):
    vc = os.path.normcase(str(tmp_path / "vc")) + os.sep
    text = textwrap.dedent(f'''\
        #line 1 "{str(tmp_path / 'a.cpp').replace(os.sep, os.sep * 2)}"
        #line 1 "{str(tmp_path / 'inc' / 'x.h').replace(os.sep, os.sep * 2)}"
        int x;
        #line 3 "rel.h"
        #line 7 "{str(tmp_path / 'vc' / 'string.h').replace(os.sep, os.sep * 2)}"
        #line 4 "{str(tmp_path / 'inc' / 'x.h').replace(os.sep, os.sep * 2)}"
        ''')
    got = buildcache.headers_from_preprocessor(text, tmp_path, {os.path.normcase(str(tmp_path / "a.cpp")), vc})
    assert got == sorted({os.path.normcase(str(tmp_path / "inc" / "x.h")), os.path.normcase(str(tmp_path / "rel.h"))})


def test_default_cache_dir_env(monkeypatch, tmp_path):
    monkeypatch.setenv("EQD_UNICORN_CACHE", str(tmp_path / "c"))
    assert buildcache.default_cache_dir() == tmp_path / "c"
    for off in ("off", "0", ""):
        monkeypatch.setenv("EQD_UNICORN_CACHE", off)
        assert buildcache.default_cache_dir() is None


# --- builds ------------------------------------------------------------------------------------------------

def _tree(tmp_path):
    """src/f.cpp -> inc/outer.h -> inc/inner.h (VALUE); inc/unused.h is never included."""
    (tmp_path / "inc").mkdir()
    (tmp_path / "src").mkdir()
    (tmp_path / "inc" / "outer.h").write_text('#include "inner.h"\n#define OUTER (VALUE + 1)\n')
    (tmp_path / "inc" / "inner.h").write_text("#define VALUE 40\n")
    (tmp_path / "inc" / "unused.h").write_text("#define UNUSED 1\n")
    (tmp_path / "src" / "f.cpp").write_text('#include "outer.h"\nextern "C" int f(void) { return OUTER; }\n')
    return tmp_path / "src" / "f.cpp", tmp_path / "inc"


def _build(src, inc, tmp_path, name="t", **kw):
    before = dict(buildcache.STATS)
    dll, mapf = build_dll(name, [src], tmp_path / "out", include_dirs=[inc], vc6=VC6,
                          cache_dir=kw.pop("cache_dir", tmp_path / "cache"), **kw)
    hit = buildcache.STATS["hit"] - before["hit"]
    return dll, mapf, hit


def _ret(dll, mapf):
    """f's return value: VC6 /O1 compiles `return N;` to `push N; pop eax; ret` or `mov eax, N; ret`."""
    img = load_pe(dll, base=0x30000000)
    read_map(mapf, img)
    code = img.read(img.by_name["_f"], 6)
    return code[1] if code[0] == 0x6A else int.from_bytes(code[1:5], "little")


@needs_vc6
def test_hit_miss_and_header_invalidation(tmp_path):
    src, inc = _tree(tmp_path)
    d1, m1, hit = _build(src, inc, tmp_path)
    assert not hit and _ret(d1, m1) == 41
    assert d1.parent.parent.parent.parent == tmp_path / "cache"          # <cache>/<rr>/<recipe>/<entry>/
    assert not any((tmp_path / "out").iterdir())                          # per-process build folder removed

    d2, _, hit = _build(src, inc, tmp_path, name="other_name")            # the name is not part of the key
    assert hit and d2 == d1

    (inc / "unused.h").write_text("#define UNUSED 2\n")                   # not included: still a hit
    _, _, hit = _build(src, inc, tmp_path)
    assert hit

    (inc / "inner.h").write_text("#define VALUE 50\n")                    # included through outer.h: miss
    d3, m3, hit = _build(src, inc, tmp_path)
    assert not hit and d3 != d1 and _ret(d3, m3) == 51
    assert d3.parent.parent == d1.parent.parent                           # same recipe, second entry

    (inc / "inner.h").write_text("#define VALUE 40\n")                    # back: the first entry again
    d4, _, hit = _build(src, inc, tmp_path)
    assert hit and d4 == d1


@needs_vc6
def test_source_flags_and_defines_invalidate(tmp_path):
    src, inc = _tree(tmp_path)
    d1, _, _ = _build(src, inc, tmp_path)
    src.write_text("// CFLAGS: /Od\n" + src.read_text())                  # per-source CFLAGS line
    d2, _, hit = _build(src, inc, tmp_path)
    assert not hit and d2.parent.parent != d1.parent.parent
    d3, _, hit = _build(src, inc, tmp_path, defines=["EXTRA=1"])
    assert not hit and d3.parent.parent != d2.parent.parent
    _, _, hit = _build(src, inc, tmp_path, defines=["EXTRA=1"])
    assert hit


@needs_vc6
def test_same_content_elsewhere_is_a_different_recipe(tmp_path):
    """Quoted includes resolve against the source's own folder, so the source path is part of the key."""
    src, inc = _tree(tmp_path)
    d1, _, _ = _build(src, inc, tmp_path)
    (tmp_path / "src2").mkdir()
    src2 = tmp_path / "src2" / "f.cpp"
    src2.write_text(src.read_text())
    d2, _, hit = _build(src2, inc, tmp_path)
    assert not hit and d2.parent.parent != d1.parent.parent


@needs_vc6
def test_no_cache_keeps_the_stamp_behaviour(tmp_path):
    src, inc = _tree(tmp_path)
    dll, mapf, _ = _build(src, inc, tmp_path, cache_dir=False)
    assert dll == (tmp_path / "out" / "t.dll").resolve() and (tmp_path / "out" / "t.stamp").is_file()


@needs_vc6
def test_parallel_builds_share_one_entry(tmp_path):
    src, inc = _tree(tmp_path)
    script = textwrap.dedent(f"""
        import sys; sys.path.insert(0, {str(UNICORN)!r})
        from eqd_unicorn.vc6 import build_dll
        dll, _ = build_dll(sys.argv[1], [{str(src)!r}], {str(tmp_path / 'out')!r}, include_dirs=[{str(inc)!r}],
                           vc6={str(VC6)!r}, cache_dir={str(tmp_path / 'cache')!r})
        print(dll)
        """)
    procs = [subprocess.Popen([sys.executable, "-c", script, f"p{i}"], stdout=subprocess.PIPE, text=True)
             for i in range(4)]
    outs = [p.communicate(timeout=300)[0].strip() for p in procs]
    assert all(p.returncode == 0 for p in procs)
    assert len(set(outs)) == 1 and Path(outs[0]).is_file()
    assert len(buildcache.entries(tmp_path / "cache")) == 1
    assert not any((tmp_path / "cache" / "tmp").iterdir()) if (tmp_path / "cache" / "tmp").exists() else True


@needs_vc6
def test_stats_prune_clear(tmp_path):
    src, inc = _tree(tmp_path)
    _build(src, inc, tmp_path)
    (inc / "inner.h").write_text("#define VALUE 7\n")
    _build(src, inc, tmp_path)
    cache = tmp_path / "cache"
    s = buildcache.stats(cache)
    assert s["entries"] == 2 and s["recipes"] == 1 and s["bytes"] > 0
    old = buildcache.entries(cache)[0]
    os.utime(old, (0, 0))                                                # unused since 1970
    assert buildcache.prune(cache, max_age_days=1) == 1
    assert buildcache.stats(cache)["entries"] == 1
    assert buildcache.prune(cache, max_bytes=0) == 1
    _build(src, inc, tmp_path)
    assert buildcache.clear(cache) == 1 and buildcache.stats(cache)["entries"] == 0
