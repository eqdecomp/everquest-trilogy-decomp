r"""Test-DLL cache maintenance (see eqd_unicorn/buildcache.py and unicorn/README.md).

    ..\.venv\Scripts\python.exe tools\unicorn\tools\unicorn_cache.py [--dir D] --stats
    ... --prune [--max-mb 500] [--days 14]     drop entries unused for N days, then LRU down to the size
    ... --clear                                drop everything
    ... --warm [--repo eq-trilogy]             build every fixture's test DLL now (e.g. after a large merge,
                                               before a long run), without running any test

Run --prune/--clear while no tests are running: a removed entry that a running test still needs fails it.
"""
import argparse
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from eqd_unicorn import buildcache  # noqa: E402


def warm(repo_arg=None) -> int:
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import eqd_repo
    from eqd_unicorn import BuildError, load_fixture
    repo = eqd_repo.Repo(repo_arg)
    h = repo.harness()
    t0, seen, failed = time.time(), set(), 0
    for p in repo.fixtures():
        fx = load_fixture(p)
        srcs = tuple(repo.build_sources(fx))
        if srcs in seen:
            continue
        seen.add(srcs)
        try:
            h.rewrite_image(f"t{eqd_repo.va_of(fx['original']):08x}", list(srcs))
        except BuildError as e:
            failed += 1
            print(f"  {repo.rel(p)}: build failed: {str(e).strip().splitlines()[-1][:160]}")
    s = buildcache.STATS
    print(f"warm: {len(seen)} source sets, {s['hit']} already cached, {s['miss']} built, {failed} failed "
          f"({time.time() - t0:.0f} s)")
    return 1 if failed else 0


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--dir", help="cache folder (default: EQD_UNICORN_CACHE, else <workspace>\\build\\unicorn_cache)")
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--stats", action="store_true", help="entries, recipes, size (default)")
    g.add_argument("--clear", action="store_true")
    g.add_argument("--prune", action="store_true")
    g.add_argument("--warm", action="store_true")
    ap.add_argument("--max-mb", type=float, default=500)
    ap.add_argument("--days", type=float, default=14)
    ap.add_argument("--repo", help="--warm: decomp repo (default: around the cwd, else <workspace>\\eq-trilogy)")
    a = ap.parse_args(argv)
    if a.dir:
        import os
        os.environ["EQD_UNICORN_CACHE"] = a.dir
    if a.warm:
        return warm(a.repo)
    rest = (["--dir", a.dir] if a.dir else []) + (["--clear"] if a.clear else []) + \
        (["--prune", "--max-mb", str(a.max_mb), "--days", str(a.days)] if a.prune else [])
    return buildcache.main(rest)


if __name__ == "__main__":
    sys.exit(main())
