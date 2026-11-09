r"""Build a small test DLL from decomp sources with Visual C++ 6 (cl + link), for the Unicorn harness.

The DLL is linked at LINK_BASE with relocations kept (/FIXED:NO) and a linker map (/MAP), so the harness can
load it at any base and find symbols by name. No entry point is linked (/NOENTRY): the harness never runs
DllMain or the CRT start-up, it only calls the function under test. CRT helpers the rewrite references
(_ftol, memcpy, ...) still come from the static CRT (LIBC).

VCVARS32.BAT is not used; the environment is set up directly so this works from Bash, cmd or pytest.
"""
from __future__ import annotations

import hashlib
import os
import re
import shutil
import subprocess
import time
import uuid
from pathlib import Path

from . import buildcache, externs, thiscall_stubs

DEFAULT_VC6 = r"C:\Program Files (x86)\Microsoft Visual Studio\VC98"
LINK_BASE = 0x10000000

# /O1 (minimize size; no /Oi, so CRT string functions stay calls): measured 2026-10-01 with VC6 SP4 on the
# 64 Phase 1c rewrites, /O1 reproduces 37 of them instruction for instruction, /O2 only 4 (PLAN Phase 1c).
# /GX (C++ EH): the client's game code was built with it (167 functions call __EH_prolog, e.g. every
# `new T(...)` of a class with a constructor). Measured 2026-10-02: neutral for code without EH, all 347
# functions of the then 24 eq-trilogy sources compile to identical object code with and without it.
DEFAULT_CFLAGS = ["/nologo", "/c", "/O1", "/W3", "/MT", "/Gy", "/GX"]

# Per-source flags: a comment line `// CFLAGS: /O2` (or ` * CFLAGS: ...` inside a block comment, or
# `/* CFLAGS: ... */`) anywhere in a source adds options for that file only, after DEFAULT_CFLAGS, so cl's
# last-one-wins rule applies (`/Ot` overrides /O1's /Os, `/GX-` turns EH off). An optimization level
# (/O1 /O2 /Ox /Od) replaces the default /O1 instead of following it. Several CFLAGS lines accumulate.
# Only code-generation options are allowed (/O..., /G..., /Zp<n>, /Op); anything that changes linking
# (/MD, /MT), includes or defines is refused: every object of a DLL must agree on those.
_CFLAGS_LINE = re.compile(r"^\s*(?://|/\*|\*)\s*CFLAGS:\s*(.*?)\s*(?:\*/)?\s*$")
_CFLAGS_OK = re.compile(r"^/(?:O[0-9A-Za-z]+-?|G[0-9A-Za-z]+-?|Zp[1248]?|Op-?)$")
OPT_LEVELS = ("/O1", "/O2", "/Ox", "/Od")

# Linked into every test DLL, see build_dll. (0x9875 is the value the CRT itself uses.)
FLTUSED_SRC = "int _fltused = 0x9875;\n"

# Without the CRT start-up objects nothing names kernel32.lib as a default library, yet rewrites call Win32
# (Sleep, ...) and LIBC itself needs it (RtlUnwind, MultiByteToWideChar, ... behind _strcmpi, _isctype).
SYSTEM_LIBS = ["kernel32.lib", "user32.lib"]

# stdio (sprintf -> _flsbuf -> _ioinit/_file) and a few other LIBC pieces need __amsg_exit, which LIBC
# defines in its EXE start-up crt0.obj (-> unresolved _main). Naming the DLL start-up object instead
# resolves those symbols from dllcrt0.obj; it is never run (/NOENTRY).
LINK_EXTRA = ["/INCLUDE:__DllMainCRTStartup@12"]


class BuildError(RuntimeError):
    pass


def image_cflags(path) -> list[str]:
    """The default options of the image a source belongs to, by its path (`.../src/<prefix>/...`, clients.py
    `cflags`): eqmain.dll's game code is /O2 (inline strlen/strcpy, no /Os `push imm; pop reg`), eqgame.exe's
    the plain defaults. Applied before the file's own CFLAGS lines."""
    from . import clients
    c, _ = clients.image_of_source(path)
    return list(c.cflags) if c is not None else []


def source_cflags(path, like=None) -> list[str]:
    """The extra options of a source: its image's defaults (image_cflags, by the path of `like` when given: a
    copy of a repo source compiled elsewhere, e.g. permute's candidates), then what it asks for with
    `// CFLAGS: ...` lines (in order). BuildError if an option isn't a code-generation option."""
    out = image_cflags(like or path)
    for n, line in enumerate(Path(path).read_text(encoding="utf-8", errors="replace").splitlines(), 1):
        m = _CFLAGS_LINE.match(line)
        if not m:
            continue
        for tok in m.group(1).split():
            if not _CFLAGS_OK.match(tok):
                raise BuildError(f"{path}:{n}: CFLAGS option {tok!r} not allowed (only /O.., /G.., /Zp<n>, /Op)")
            out.append(tok)
    return out


def cflags_for(path, base=None, like=None) -> list[str]:
    """DEFAULT_CFLAGS (or `base`) with the source's own CFLAGS applied: an optimization level replaces the
    base one, everything else is appended. `like`: as source_cflags."""
    base = list(base or DEFAULT_CFLAGS)
    extra = source_cflags(path, like)
    if any(t in OPT_LEVELS for t in extra):
        base = [t for t in base if t not in OPT_LEVELS]
    return base + extra


_TOOLCHAIN_DIRS: dict = {}


def toolchain_dir(key: str) -> Path:
    """paths.local.ini [toolchain] <key> (EQD_TOOLCHAIN_<KEY> overrides it). BuildError if it isn't a VC6."""
    if key not in _TOOLCHAIN_DIRS:
        env = os.environ.get(f"EQD_TOOLCHAIN_{key.upper()}")
        if env:
            d = Path(env)
        else:
            from .workspace import Workspace
            ws = Workspace()
            if not ws.cfg.has_option("toolchain", key):
                raise BuildError(f"TOOLCHAIN {key!r}: no [toolchain] {key} in {ws.root / 'paths.local.ini'}")
            d = Path(ws.cfg["toolchain"][key])
        if not (d / "Bin" / "CL.EXE").is_file():
            raise BuildError(f"TOOLCHAIN {key!r}: no Bin\\CL.EXE under {d}")
        _TOOLCHAIN_DIRS[key] = d
    return _TOOLCHAIN_DIRS[key]


def source_vc6(path, default, like=None) -> Path:
    """The VC98 folder a source compiles with: `default` (its image's toolchain, as the caller resolved it),
    unless the source names another (clients.source_toolchain: its `// TOOLCHAIN: <key>` line, else its image's
    folder rule, by the path of `like` when given). Test DLLs are still linked with the build's own toolchain."""
    from . import clients
    text = Path(path).read_text(encoding="utf-8", errors="replace")
    key = clients.source_toolchain(like or path, text)
    if key is None:
        return Path(default)
    d = toolchain_dir(key)
    return Path(default) if buildcache._norm(d) == buildcache._norm(default) else d


def vc6_env(vc6: Path) -> dict:
    vc6 = Path(vc6)
    msdev = vc6.parent / "Common" / "MSDev98" / "Bin"
    env = dict(os.environ)
    env["PATH"] = os.pathsep.join([str(vc6 / "Bin"), str(msdev), env.get("PATH", "")])
    env["INCLUDE"] = os.pathsep.join([str(vc6 / "Include"), str(vc6 / "MFC" / "Include"), str(vc6 / "ATL" / "Include")])
    env["LIB"] = os.pathsep.join([str(vc6 / "Lib"), str(vc6 / "MFC" / "Lib")])
    return env


def _run(cmd, env, cwd):
    p = subprocess.run(cmd, env=env, cwd=cwd, capture_output=True, text=True, errors="replace")
    if p.returncode != 0:
        raise BuildError(f"{Path(cmd[0]).name} failed ({p.returncode}):\n{p.stdout}{p.stderr}")
    return p.stdout


def build_dll(name: str, sources, out_dir, include_dirs=(), defines=(), vc6=None, cflags=None,
              libs=(), cache_dir=None) -> tuple[Path, Path]:
    """Compile `sources` and link them into a DLL (+ .map). Returns (dll, map).

    With a build cache (`cache_dir`; None = buildcache.default_cache_dir(): EQD_UNICORN_CACHE, else
    <workspace>/build/unicorn_cache; False = none) the result is a cache entry shared by every build of the
    same sources, headers and options, whatever its `name`; a miss builds in a per-process folder under
    `out_dir` and stores the result (buildcache.py). The returned files are read-only: never modify them.
    Without a cache: <out_dir>/<name>.dll, rebuilt when a source, an include dir file or the options changed
    (hash stamp next to the DLL).
    """
    vc6 = Path(vc6 or os.environ.get("EQD_VC6") or DEFAULT_VC6)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        raise BuildError(f"VC6 not found at {vc6}")
    out_dir = Path(out_dir).resolve()
    sources = [Path(s).resolve() for s in sources]
    if len({s.stem.lower() for s in sources}) != len(sources):
        raise BuildError("two sources share a file name; objects would collide")
    include_dirs = [Path(d).resolve() for d in include_dirs]
    cflags = list(cflags or DEFAULT_CFLAGS)
    # Per-source CFLAGS and TOOLCHAIN lines: one cl run per distinct (toolchain, option list), sources keep
    # their order within it. The toolchain is "" for the build's own (`vc6`), else another VC98 folder.
    groups: dict = {}
    for s in sources:
        tc = source_vc6(s, vc6)
        tc = "" if tc == vc6 else str(tc)
        groups.setdefault((tc, tuple(cflags_for(s, cflags))), []).append(s)

    if cache_dir is None:
        cache_dir = buildcache.default_cache_dir()
    if cache_dir:
        return _build_cached(name, sources, groups, out_dir, include_dirs, defines, vc6, libs, Path(cache_dir))

    obj_dir = out_dir / f"{name}_obj"
    obj_dir.mkdir(parents=True, exist_ok=True)
    dll = out_dir / f"{name}.dll"
    mapf = out_dir / f"{name}.map"
    # Stamp: options + every source + every header under the include dirs.
    h = hashlib.sha256(repr((cflags, sorted(groups), [str(s) for s in sources], list(map(str, include_dirs)),
                             list(defines), list(libs), str(vc6), FLTUSED_SRC, SYSTEM_LIBS, LINK_EXTRA)).encode())
    for f in sources + sorted(p for d in include_dirs for p in Path(d).rglob("*") if p.is_file()):
        h.update(str(f).encode())
        h.update(Path(f).read_bytes())
    stamp = out_dir / f"{name}.stamp"
    digest = h.hexdigest()
    if dll.is_file() and mapf.is_file() and stamp.is_file() and stamp.read_text() == digest:
        return dll, mapf
    _compile_and_link(sources, groups, out_dir, obj_dir, dll, mapf, include_dirs, defines, vc6, libs)
    stamp.write_text(digest)
    return dll, mapf


def _cl_base(vc6, flags, include_dirs, defines) -> list:
    return ([str(vc6 / "Bin" / "CL.EXE")] + list(flags) + [f"/I{d}" for d in include_dirs]
            + [f"/D{d}" for d in defines])


def _build_cached(name, sources, groups, out_dir, include_dirs, defines, vc6, libs, cache_dir):
    fixed = (LINK_BASE, FLTUSED_SRC, SYSTEM_LIBS, LINK_EXTRA)
    recipe = buildcache.recipe_key(groups, sources, include_dirs, defines, libs, vc6, fixed)
    hit = buildcache.lookup(cache_dir, recipe)
    if hit is not None:
        return hit / buildcache.DLL, hit / buildcache.MAP
    buildcache.STATS["miss"] += 1

    work = out_dir / f"{name}.{os.getpid()}.{uuid.uuid4().hex[:8]}"
    obj_dir = work / "obj"
    obj_dir.mkdir(parents=True)
    try:
        # Headers: every file the preprocessor opens, with the same options (VC6 has no /showIncludes).
        env = vc6_env(vc6)
        exclude = ({buildcache._norm(s) for s in sources} | {buildcache._norm(vc6) + os.sep}
                   | {buildcache._norm(tc) + os.sep for tc, _ in groups if tc})
        headers = set()
        for (tc, flags), group in groups.items():
            cvc6 = Path(tc) if tc else vc6
            text = _run(_cl_base(cvc6, flags, include_dirs, defines) + ["/E"] + [str(s) for s in group],
                        vc6_env(cvc6) if tc else env, work)
            headers.update(buildcache.headers_from_preprocessor(text, work, exclude))
        before = buildcache.hash_headers(sorted(headers))
        dll, mapf = work / f"{name}.dll", work / f"{name}.map"
        _compile_and_link(sources, groups, work, obj_dir, dll, mapf, include_dirs, defines, vc6, libs)
        # Store only if nothing changed under our feet (the key must describe what was compiled).
        unchanged = (buildcache.hash_headers(sorted(headers)) == before
                     and buildcache.recipe_key(groups, sources, include_dirs, defines, libs, vc6, fixed) == recipe
                     and all(h is not None for _, h in before))
        entry = None
        if unchanged:
            meta = {"name": name, "sources": [str(s) for s in sources], "groups": [list(g) for g in groups],
                    "include_dirs": [str(d) for d in include_dirs], "defines": list(defines),
                    "created": time.strftime("%Y-%m-%dT%H:%M:%S"), "pid": os.getpid()}
            entry = buildcache.store(cache_dir, recipe, dll, mapf, before, meta)
        if entry is not None:
            return entry / buildcache.DLL, entry / buildcache.MAP
        # Not cacheable (an input changed during the build, or the store failed): keep it under out_dir.
        buildcache.STATS["unstored"] += 1
        tag = uuid.uuid4().hex[:8]
        final_dll, final_map = out_dir / f"{name}.{tag}.dll", out_dir / f"{name}.{tag}.map"
        os.replace(dll, final_dll)
        os.replace(mapf, final_map)
        return final_dll, final_map
    finally:
        shutil.rmtree(work, ignore_errors=True)


def _compile_and_link(sources, groups, out_dir, obj_dir, dll, mapf, include_dirs, defines, vc6, libs):
    env = vc6_env(vc6)
    for (tc, flags), group in groups.items():
        cvc6 = Path(tc) if tc else vc6
        _run(_cl_base(cvc6, flags, include_dirs, defines) + [f"/Fo{obj_dir}\\"] + [str(s) for s in group],
             vc6_env(cvc6) if tc else env, out_dir)
    objs = [str(obj_dir / (s.stem + ".obj")) for s in sources]
    # Any float code makes cl reference __fltused, which LIBC only provides through its float start-up
    # (crt0fp -> crt0 -> _main): a /NOENTRY test DLL then fails with "unresolved external _main".
    # Defining it ourselves keeps the CRT start-up out; it never runs anyway (no DllMain, no CRT init).
    flt = obj_dir / "eqd_fltused.c"
    flt.write_text(FLTUSED_SRC)
    _run([str(vc6 / "Bin" / "CL.EXE"), "/nologo", "/c", f"/Fo{obj_dir}\\", str(flt)], env, out_dir)
    objs.append(str(obj_dir / "eqd_fltused.obj"))
    # Vftables the sources mark `// EXTERNAL:`: their stores get the client's address.
    externs.patch_vftable_refs(dict(zip(sources, objs[:-1])), include_dirs)
    # EQ_METHOD calls, and functions the sources declare with an `// EXTERNAL:` marker: `jmp va` stubs.
    ext, _problems = externs.build_map(dict(zip(sources, objs[:-1])), include_dirs)
    stubs = thiscall_stubs.write_stub_obj(objs[:-1], obj_dir / "eqd_thiscall_stubs.obj", ext)
    if stubs:
        objs.append(str(stubs))
    link =[str(vc6 / "Bin" / "LINK.EXE"), "/nologo", "/DLL", "/NOENTRY", "/FIXED:NO",
            f"/BASE:{LINK_BASE:#x}", f"/MAP:{mapf}", f"/OUT:{dll}", "/OPT:NOREF"] + LINK_EXTRA + objs + list(libs) + SYSTEM_LIBS
    _run(link, env, out_dir)
