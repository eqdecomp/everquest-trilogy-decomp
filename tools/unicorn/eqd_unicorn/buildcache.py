r"""Content-addressed cache of test DLLs (vc6.build_dll), shared by processes and worktrees.

Most fixtures of a subsystem build the same sources (EQMisc.cpp: 168 fixtures), so one DLL serves all of
them; and an unchanged source isn't rebuilt by the next run or pytest process.

Key, in two levels:
1. **Recipe** (what the build was asked to do): the cache format and the build code itself (this file +
   vc6.py), the toolchain (path + size/mtime of every file under its Bin, Lib and Include dirs), the cl option
   group of every source (DEFAULT_CFLAGS + its `// CFLAGS:` lines), every source's absolute path and content,
   include dirs, defines, extra libs (content when they are files), the fixed link inputs. Directory
   `<cache>\<rr>\<recipe>\`.
2. **Headers**: every file the preprocessor opened (`cl /E` on the same command line; VC6 has no
   /showIncludes), outside the toolchain, with its content hash, stored with the entry (`deps.json`). A
   lookup re-hashes them: the first entry of the recipe whose headers all still match is a hit. Editing a
   header a source includes (directly or not) is a miss; editing one it doesn't include is still a hit.

Writes are atomic: a DLL is built in the caller's own build dir (per-process temp folder), copied into
`<cache>\tmp\<pid>-<random>\` and renamed into place; a complete entry is never modified afterwards. When two
processes build the same thing at once, the second rename fails and that process uses the first one's entry.
An entry is only stored when the sources and headers are unchanged after the build (an edit during the build
would make the key lie).

Location: `EQD_UNICORN_CACHE` (a folder; `off`/`0`/empty disables the cache), else
`<workspace>\build\unicorn_cache` (paths.local.ini). Without a workspace and without the variable there is
no cache and build_dll falls back to its per-name stamp files.

Not detected: a new header that *shadows* an existing one earlier in the include search path (same name,
nothing tracked changes). Run `--clear` (below) after such a move.

Size: one entry is the DLL + map (~100-500 KB). Each header edit that touches every source adds one entry per
source set, so the cache grows by a few MB per wave. CLI (when no tests are running, a pruned entry in use
fails that test):
    python tools\unicorn\tools\unicorn_cache.py [--dir D] --stats | --clear | --prune [--max-mb 500] [--days 14]
"""
from __future__ import annotations

import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import time
import uuid
from pathlib import Path

CACHE_FORMAT = 1
DLL = "rewrite.dll"
MAP = "rewrite.map"
DEPS = "deps.json"
_OFF = {"", "0", "off", "no", "none", "false"}

STATS = {"hit": 0, "miss": 0, "stored": 0, "unstored": 0}       # per process, for tests and timing

_TOOLCHAIN: dict = {}
_FILEHASH: dict = {}


def default_cache_dir() -> Path | None:
    """EQD_UNICORN_CACHE, else <workspace>/build/unicorn_cache, else None (no cache)."""
    env = os.environ.get("EQD_UNICORN_CACHE")
    if env is not None:
        return None if env.strip().lower() in _OFF else Path(env)
    from .workspace import Workspace          # late: workspace imports vc6, which imports this module
    try:
        return Workspace().build / "unicorn_cache"
    except FileNotFoundError:
        return None


def _norm(p) -> str:
    return os.path.normcase(os.path.abspath(str(p)))


def toolchain_id(vc6: Path) -> str:
    """Path plus size and mtime of every toolchain file that can change a build (compiler, linker, libs,
    system headers). Computed once per process."""
    vc6 = Path(vc6)
    key = _norm(vc6)
    if key not in _TOOLCHAIN:
        h = hashlib.sha256(key.encode())
        for sub in (vc6 / "Bin", vc6 / "Lib", vc6 / "Include", vc6 / "MFC" / "Include", vc6 / "ATL" / "Include",
                    vc6.parent / "Common" / "MSDev98" / "Bin"):
            if not sub.is_dir():
                continue
            for root, dirs, files in os.walk(sub):
                dirs.sort()
                for f in sorted(files):
                    st = os.stat(os.path.join(root, f))
                    h.update(f"{os.path.relpath(os.path.join(root, f), vc6)}|{st.st_size}|{st.st_mtime_ns}\n".encode())
        _TOOLCHAIN[key] = h.hexdigest()
    return _TOOLCHAIN[key]


def _code_id() -> str:
    """The build recipe's own code: any change to it invalidates every entry."""
    here = Path(__file__).resolve().parent
    h = hashlib.sha256(str(CACHE_FORMAT).encode())
    for f in ("buildcache.py", "vc6.py"):
        h.update((here / f).read_bytes().replace(b"\r\n", b"\n"))     # same code in any checkout
    return h.hexdigest()


_CODE_ID: list = []


def code_id() -> str:
    if not _CODE_ID:
        _CODE_ID.append(_code_id())
    return _CODE_ID[0]


def file_hash(path) -> str | None:
    """sha256 of a file's content (memoised per process by size + mtime), None if it doesn't exist."""
    try:
        st = os.stat(path)
    except OSError:
        return None
    k = (_norm(path), st.st_size, st.st_mtime_ns)
    if k not in _FILEHASH:
        try:
            _FILEHASH[k] = hashlib.sha256(Path(path).read_bytes()).hexdigest()
        except OSError:
            return None
    return _FILEHASH[k]


def recipe_key(groups: dict, sources, include_dirs, defines, libs, vc6, fixed) -> str:
    """`groups`: {(toolchain, cl option tuple): [sources]} as build_dll compiles them (toolchain "" = `vc6`, else
    another VC98 folder, for sources with a TOOLCHAIN line or folder rule); `fixed`: the build's constant inputs
    (link options, the _fltused source, system libs)."""
    h = hashlib.sha256()
    h.update(code_id().encode())
    h.update(toolchain_id(vc6).encode())
    for tc in sorted({k[0] for k in groups if k[0]}):
        h.update(f"tc|{toolchain_id(Path(tc))}\n".encode())
    h.update(repr(sorted((list(k), [_norm(s) for s in v]) for k, v in groups.items())).encode())
    h.update(repr([_norm(d) for d in include_dirs]).encode())
    h.update(repr(list(defines)).encode())
    h.update(repr(fixed).encode())
    for lib in libs:
        h.update(f"lib|{lib}|{file_hash(lib) if Path(lib).is_file() else ''}\n".encode())
    for s in sources:
        h.update(f"src|{_norm(s)}|\n".encode())
        h.update(Path(s).read_bytes())
    return h.hexdigest()


def lookup(cache_dir: Path, recipe: str) -> Path | None:
    """The entry for `recipe` whose recorded headers are all unchanged, or None."""
    rdir = Path(cache_dir) / recipe[:2] / recipe
    try:
        entries = sorted(rdir.iterdir(), key=lambda p: p.stat().st_mtime, reverse=True)
    except OSError:
        return None
    for e in entries:
        try:
            deps = json.loads((e / DEPS).read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        if all(file_hash(p) == h for p, h in deps["headers"]) and (e / DLL).is_file() and (e / MAP).is_file():
            try:
                os.utime(e)                     # last use, for --prune
            except OSError:
                pass
            STATS["hit"] += 1
            return e
    return None


def headers_from_preprocessor(text: str, cwd, exclude) -> list[str]:
    """Files named by `#line N "file"` in cl /E output, normalised, minus `exclude` (normalised paths or
    folder prefixes ending in os.sep)."""
    out = set()
    for m in re.finditer(r'^#line\s+\d+\s+"((?:[^"\\]|\\.)*)"', text, re.M):
        p = m.group(1).replace("\\\\", "\\")
        n = _norm(os.path.join(str(cwd), p))
        if n in exclude or any(n.startswith(x) for x in exclude if x.endswith(os.sep)):
            continue
        out.add(n)
    return sorted(out)


def hash_headers(paths) -> list:
    return [[p, file_hash(p)] for p in paths]


def store(cache_dir: Path, recipe: str, dll: Path, mapf: Path, headers: list, meta: dict) -> Path | None:
    """Copy a finished build into the cache atomically. -> the entry (ours or a concurrent twin), or None."""
    cache_dir = Path(cache_dir)
    rdir = cache_dir / recipe[:2] / recipe
    dep_key = hashlib.sha256(json.dumps(headers, sort_keys=True).encode()).hexdigest()[:20]
    final = rdir / dep_key
    if (final / DEPS).is_file():
        return final
    tmp = cache_dir / "tmp" / f"{os.getpid()}-{uuid.uuid4().hex}"
    try:
        tmp.mkdir(parents=True)
        shutil.copyfile(dll, tmp / DLL)
        shutil.copyfile(mapf, tmp / MAP)
        (tmp / "meta.json").write_text(json.dumps(meta, indent=1), encoding="utf-8")
        (tmp / DEPS).write_text(json.dumps({"headers": headers}, indent=1), encoding="utf-8")   # last
        rdir.mkdir(parents=True, exist_ok=True)
        os.rename(tmp, final)
        STATS["stored"] += 1
        return final
    except OSError:
        shutil.rmtree(tmp, ignore_errors=True)
        return final if (final / DEPS).is_file() else None


# --- maintenance ------------------------------------------------------------------------------------------

def entries(cache_dir: Path) -> list[Path]:
    cache_dir = Path(cache_dir)
    out = []
    if not cache_dir.is_dir():
        return out
    for shard in cache_dir.iterdir():
        if shard.name == "tmp" or not shard.is_dir():
            continue
        for rdir in shard.iterdir():
            if rdir.is_dir():
                out += [e for e in rdir.iterdir() if (e / DEPS).is_file()]
    return out


def _size(p: Path) -> int:
    return sum(f.stat().st_size for f in p.rglob("*") if f.is_file())


def stats(cache_dir: Path) -> dict:
    es = entries(cache_dir)
    return {"dir": str(cache_dir), "entries": len(es), "recipes": len({e.parent for e in es}),
            "bytes": sum(_size(e) for e in es)}


def _remove(e: Path):
    """Rename first (atomic: a concurrent lookup sees the entry whole or not at all), then delete."""
    gone = e.parent / f".del-{os.getpid()}-{uuid.uuid4().hex[:8]}"
    try:
        os.rename(e, gone)
    except OSError:
        return False
    shutil.rmtree(gone, ignore_errors=True)
    try:
        e.parent.rmdir()                       # the recipe dir, when empty
    except OSError:
        pass
    return True


def prune(cache_dir: Path, max_bytes: int | None = None, max_age_days: float | None = None) -> int:
    """Drop entries unused for `max_age_days`, then least recently used ones until under `max_bytes`; also
    stale temp folders (> 1 day). -> entries removed."""
    cache_dir = Path(cache_dir)
    now = time.time()
    es = sorted(entries(cache_dir), key=lambda e: e.stat().st_mtime)
    removed = 0
    if max_age_days is not None:
        for e in [e for e in es if now - e.stat().st_mtime > max_age_days * 86400]:
            removed += _remove(e)
            es.remove(e)
    if max_bytes is not None:
        sizes = {e: _size(e) for e in es}
        total = sum(sizes.values())
        for e in es:
            if total <= max_bytes:
                break
            if _remove(e):
                removed += 1
                total -= sizes[e]
    tmp = cache_dir / "tmp"
    if tmp.is_dir():
        for t in tmp.iterdir():
            if now - t.stat().st_mtime > 86400:
                shutil.rmtree(t, ignore_errors=True)
    return removed


def clear(cache_dir: Path) -> int:
    n = 0
    for e in entries(cache_dir):
        n += _remove(e)
    shutil.rmtree(Path(cache_dir) / "tmp", ignore_errors=True)
    return n


def main(argv=None) -> int:
    import argparse
    ap = argparse.ArgumentParser(description="Unicorn test-DLL cache: stats, prune, clear.")
    ap.add_argument("--dir", help="cache folder (default: EQD_UNICORN_CACHE, else <workspace>\\build\\unicorn_cache)")
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--stats", action="store_true", help="entries, recipes, size (default action)")
    g.add_argument("--clear", action="store_true", help="remove every entry")
    g.add_argument("--prune", action="store_true", help="remove old / least recently used entries")
    ap.add_argument("--max-mb", type=float, default=500, help="--prune: size limit (default 500)")
    ap.add_argument("--days", type=float, default=14, help="--prune: drop entries unused this long (default 14)")
    a = ap.parse_args(argv)
    d = Path(a.dir) if a.dir else default_cache_dir()
    if d is None:
        print("no cache folder (EQD_UNICORN_CACHE is off, or no workspace)", file=sys.stderr)
        return 1
    if a.clear:
        print(f"removed {clear(d)} entries from {d}")
    elif a.prune:
        print(f"removed {prune(d, int(a.max_mb * 1024 * 1024), a.days)} entries from {d}")
    s = stats(d)
    print(f"{s['dir']}: {s['entries']} entries, {s['recipes']} recipes, {s['bytes'] / 1048576:.1f} MB")
    return 0
