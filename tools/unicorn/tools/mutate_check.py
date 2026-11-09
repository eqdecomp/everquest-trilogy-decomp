r"""Mutation check: do a function's Unicorn fixtures catch deliberate wrong edits of its rewrite?

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\unicorn\tools\mutate_check.py tests\unicorn\EverQuest\Display\IsInvisible.json
    ..\.venv\Scripts\python.exe tools\unicorn\tools\mutate_check.py --va 0x00410C80 [--source src\EverQuest\Display.cpp]
        [--max N] [--kinds rel,const,...] [--all-cases] [--json out.json] [--list] [-v]
        [--lines A-B ...] [--region-markers [--region Rnn ...] [--max-per-region N]]

Every fixture whose `original` is the function's VA runs (plus the given file, if it lives elsewhere). One
mutant at a time (mutations.py: relational flips incl. off-by-one, constants +-1, operator swaps, swapped
operands, deleted statements and for-increments, negated conditions, early return) is applied to a **copy**
of the sources in a private temp folder; the fixtures are pointed at the copy, never at the repo's files, so
other runs never see a mutated source and nothing needs restoring (the folder is deleted at the end, also
on errors and Ctrl-C). The unmutated copy must pass first.

Scope (giants, docs/PLAN.md 8): `--lines A-B` (repeatable) mutates only inside those source lines.
`--region-markers` spreads the mutants over the `// REGION Rnn 0xA-0xB: label` ... `// END REGION Rnn` blocks of
the source (giant_kit.py's skeleton): `--max-per-region` each (default 3, 0 = all), `--region Rnn` (repeatable)
only those blocks; the result is reported per region. `--max` defaults to 30 + 1 per 100 original
instructions (at most 250; read from the export), or 30 when the size is unknown; 0 = all.

Verdicts per mutant: `caught` (some case differs, or the run errors), `SURVIVED` (every fixture still passes:
add cases that tell them apart, or note why the mutant is equivalent), `no-build` (doesn't compile, not
counted). A mutant stops at its first failing case. Exit status: 0 = nothing survived, 1 = survivors,
2 = setup problem (no fixture, baseline fails, ...).
"""
from __future__ import annotations

import argparse
import copy
import json
import re
import shutil
import sys
import tempfile
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import eqd_repo  # noqa: E402  (imports this checkout's eqd_unicorn first)
import mutations as mu  # noqa: E402
from eqd_unicorn import BuildError, HarnessError, annotations as ann, load_fixture  # noqa: E402


class Setup(Exception):
    pass


def pick_fixtures(repo: eqd_repo.Repo, fixture=None, va=None):
    """-> (va, [fixture paths])."""
    paths = []
    if fixture:
        fixture = Path(fixture).resolve()
        va = eqd_repo.va_of(load_fixture(fixture)["original"])
        paths.append(fixture)
    if va is None:
        raise Setup("give a fixture or --va")
    for p in repo.fixtures_for(va):
        if p.resolve() not in paths:
            paths.append(p.resolve())
    if not paths:
        raise Setup(f"no fixture with \"original\": {va:#010x} under {repo.tests_dir}")
    return va, paths


def target_source(repo, fixtures, va, source=None) -> Path:
    if source:
        p = repo.src_path(source).resolve()
        if not p.is_file():
            raise Setup(f"{source} not found")
        return p
    cands = []
    for f in fixtures:
        for s in repo.build_sources(load_fixture(f)):
            if s.resolve() not in cands:
                cands.append(s.resolve())
    hits = [s for s in cands if any(a.va == va for a in ann.scan([s], repo.client))]
    if len(hits) != 1:
        raise Setup(f"{len(hits)} of the fixtures' sources annotate {va:#x}; pass --source")
    return hits[0]


class Sandbox:
    """Copies of every source the fixtures build, under one temp folder that mirrors the repo layout."""

    def __init__(self, repo, fixtures, work=None):
        self.repo = repo
        base = Path(work) if work else eqd_repo.default_work_dir("mutate", repo.client)
        base.mkdir(parents=True, exist_ok=True)
        self.dir = Path(tempfile.mkdtemp(prefix="m", dir=base))
        self.map: dict = {}                                   # original source -> copy
        for f in fixtures:
            for s in repo.build_sources(load_fixture(f)):
                s = s.resolve()
                if s in self.map:
                    continue
                try:
                    rel = s.relative_to(repo.root)
                except ValueError:
                    rel = Path("_abs") / s.name
                dst = self.dir / rel
                dst.parent.mkdir(parents=True, exist_ok=True)
                shutil.copyfile(s, dst)
                self.map[s] = dst

    def sources_for(self, fx: dict) -> list[Path]:
        return [self.map[s.resolve()] for s in self.repo.build_sources(fx)]

    def close(self):
        shutil.rmtree(self.dir, ignore_errors=True)


def run_all(repo, harness, sandbox, fixtures, stop_on_failure, only=None, case_cover=None) -> tuple[str, str]:
    """-> (verdict, detail): ('pass', ''), ('fail', '<fixture>: <first diff>'), ('no-build', msg),
    ('error', msg). `only`: {fixture: case indices} to run (fixtures missing from it are skipped);
    `case_cover`: filled with {fixture: {case index: original blocks it ran}}."""
    for f in fixtures:
        if only is not None and not only.get(f):
            continue
        fx = load_fixture(f)
        try:
            t = harness.difftest(fx, sandbox.sources_for(fx), base_dir=repo.root)
            cc = case_cover.setdefault(f, {}) if case_cover is not None else None
            r = t.run(stop_on_failure=stop_on_failure, only=only.get(f) if only is not None else None,
                      case_cover=cc)
        except BuildError as e:
            return "no-build", str(e).strip().splitlines()[-1][:200] if str(e).strip() else "build failed"
        except HarnessError as e:
            return "error", f"{f.name}: {e}"
        if not r.ok:
            bad = r.failures[0] if r.failures else None
            d = (bad.diffs[0] if bad and bad.diffs else "no cases")
            return "fail", f"{f.name} case {bad.index if bad else '-'}: {d[:160]}"
    return "pass", ""


_SPAN = re.compile(r"0x([0-9A-Fa-f]+)\s*-\s*0x([0-9A-Fa-f]+)")


def region_spans(text: str) -> dict:
    """{region id: (first VA, end VA)} from the `// REGION Rnn 0xA-0xB: label` markers (end exclusive)."""
    out = {}
    for ln in text.split("\n"):
        m = mu._REGION.match(ln)
        s = _SPAN.match(m.group(2).strip()) if m else None
        if s:
            out[m.group(1)] = (int(s.group(1), 16), int(s.group(2), 16))
    return out


def reaching(case_cover: dict, span) -> dict:
    """{fixture: case indices whose original run entered `span`}, fixtures in order of the share of their cases
    that do (a piece's own fixture first: a mutant stops at its first failing case)."""
    a, b = span
    hit = {f: {i for i, c in cc.items() if any(a <= x < b for x, _n in c)} for f, cc in case_cover.items()}
    order = sorted(hit, key=lambda f: -len(hit[f]) / max(1, len(case_cover[f])))
    return {f: hit[f] for f in order}


def original_insns(client, va):
    """Instruction count of the original function from the workspace's export, or None."""
    try:
        ws = eqd_repo.find_root()
        p = ws / "build" / "export" / client / "functions.json" if ws else None
        if p is None or not p.is_file():
            return None
        key = f"{va:08X}"
        for f in json.loads(p.read_text(encoding="utf-8"))["functions"]:
            if f["address"] == key:
                return f.get("instructions")
    except (OSError, ValueError, KeyError):
        pass
    return None


def region_report(blocks, results, available) -> dict:
    """{region id: checked / available / caught / survived / no_build} in block order."""
    out = {}
    for rid, _a, _b in blocks:
        rs = [r for r in results if r.get("region") == rid]
        out[rid] = {"checked": len(rs), "available": available.get(rid, 0),
                    "caught": sum(r["status"] == "caught" for r in rs),
                    "survived": sum(r["status"] == "SURVIVED" for r in rs),
                    "no_build": sum(r["status"] == "no-build" for r in rs)}
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("fixture", nargs="?", help="a fixture .json (its `original` picks the function)")
    ap.add_argument("--va", help="the function's original VA (instead of a fixture)")
    ap.add_argument("--source", help="the file to mutate (default: the fixtures' source annotating the VA)")
    ap.add_argument("--repo", help="decomp repo (default: the one around the cwd, else <workspace>\\eq-trilogy)")
    ap.add_argument("--client", default="trilogy")
    ap.add_argument("--max", type=int, default=None,
                    help="at most N mutants, spread over kinds (default 30 + 1 per 100 original instructions, "
                         "at most 250; 0 = all)")
    ap.add_argument("--lines", action="append", metavar="A-B",
                    help="only mutate source lines A-B (repeatable; 1-based, inclusive)")
    ap.add_argument("--region-markers", action="store_true",
                    help="spread the mutants over the // REGION blocks and report per region")
    ap.add_argument("--region", action="append", metavar="Rnn",
                    help="with --region-markers: only this block (repeatable)")
    ap.add_argument("--max-per-region", type=int, default=3,
                    help="with --region-markers: mutants per block (default 3; 0 = all)")
    ap.add_argument("--kinds", help=f"comma list of {','.join(mu.KIND_ORDER)}")
    ap.add_argument("--all-cases", action="store_true", help="run every case of a mutant (default: stop at the first failure)")
    ap.add_argument("--no-case-filter", action="store_true",
                    help="with --region-markers: run every case for every mutant (default: first only the cases "
                         "whose original run enters the mutant's region, own fixture first; a mutant that survives "
                         "them is rerun on every case before it counts as SURVIVED)")
    ap.add_argument("--list", action="store_true", help="only list the mutants")
    ap.add_argument("--json", help="write the results here")
    ap.add_argument("--work", help="temp parent folder (default <EQD_UNICORN_BUILD>\\mutate\\<pid>)")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args(argv)

    try:
        repo = eqd_repo.Repo(a.repo, a.client)
        va, fixtures = pick_fixtures(repo, a.fixture, int(a.va, 16) if a.va else None)
        src = target_source(repo, fixtures, va, a.source)
        text = src.read_text(encoding="utf-8", errors="replace")
        ms = mu.mutants(text, va, a.client)
    except (Setup, eqd_repo.RepoError, ValueError) as e:
        print(f"mutate_check: {e}", file=sys.stderr)
        return 2
    if a.kinds:
        keep = set(a.kinds.split(","))
        ms = [m for m in ms if m.kind in keep]
    try:
        ranges = mu.parse_line_ranges(a.lines)
    except ValueError as e:
        print(f"mutate_check: {e}", file=sys.stderr)
        return 2
    ms = mu.in_lines(ms, ranges)
    blocks, available = [], {}
    if a.region_markers:
        blocks = mu.region_blocks(text)
        if a.region:
            unknown = sorted(set(a.region) - {b[0] for b in blocks})
            if unknown:
                print(f"mutate_check: no // REGION marker for {', '.join(unknown)} in {repo.rel(src)}", file=sys.stderr)
                return 2
            blocks = [b for b in blocks if b[0] in set(a.region)]
        if not blocks:
            print(f"mutate_check: no // REGION markers in {repo.rel(src)}", file=sys.stderr)
            return 2
        chosen, available = mu.select_per_region(ms, blocks, a.max_per_region or None)
        if a.max:                                          # an explicit total still caps the whole run
            chosen = mu.select(chosen, a.max)
    else:
        n_max = a.max
        if n_max is None:
            n_max = mu.scaled_max(original_insns(a.client, va))
        chosen = mu.select(ms, n_max or None)
    scope = (f", lines {','.join(f'{x}-{y}' for x, y in ranges)}" if ranges else "") \
        + (f", {len(blocks)} region blocks" if a.region_markers else "")
    print(f"{va:#010x} in {repo.rel(src)}: {len(ms)} mutants{scope}, checking {len(chosen)}; "
          f"fixtures: {', '.join(repo.rel(f) for f in fixtures)}")
    if a.list:
        for m in chosen:
            rid = mu.region_of_line(blocks, m.line) if blocks else None
            print(f"  {m.kind:7} L{m.line}: {m.desc}" + (f"   [{rid}]" if rid else ""))
        return 0

    sandbox = None
    results = []
    filtered = 0
    t0 = time.time()
    interrupted = False
    try:
        try:
            harness = copy.copy(repo.harness())
        except eqd_repo.RepoError as e:
            print(f"mutate_check: {e}", file=sys.stderr)
            return 2
        sandbox = Sandbox(repo, fixtures, a.work)
        # Quoted includes of the copies still find the originals' neighbours (searched after the include dirs).
        harness.include_dirs = list(harness.include_dirs) + sorted({s.parent for s in sandbox.map}
                                                                   - set(harness.include_dirs))
        # Mutants are one-offs: build them privately, never into the shared DLL cache.
        harness.cache_dir = False
        harness.build_dir = sandbox.dir / "_build"
        copy_path = sandbox.map[src]
        case_cover = {}
        verdict, detail = run_all(repo, harness, sandbox, fixtures, stop_on_failure=False, case_cover=case_cover)
        spans = region_spans(text) if blocks and not a.no_case_filter and not a.all_cases else {}
        print(f"unmutated copy: {verdict} ({time.time() - t0:.0f} s)", flush=True)
        if verdict != "pass":
            print(f"mutate_check: the unmutated copy doesn't pass ({verdict}): {detail}", file=sys.stderr)
            return 2
        for i, m in enumerate(chosen, 1):
            copy_path.write_text(m.apply(text), encoding="utf-8")
            rid = mu.region_of_line(blocks, m.line) if blocks else None
            only = reaching(case_cover, spans[rid]) if rid in spans else None
            try:
                verdict, detail = run_all(repo, harness, sandbox, list(only or fixtures),
                                          stop_on_failure=not a.all_cases, only=only)
                if verdict == "pass" and only is not None:      # the filter only ever drops cases: confirm on all
                    filtered += 1
                    verdict, detail = run_all(repo, harness, sandbox, fixtures, stop_on_failure=True)
            finally:
                copy_path.write_text(text, encoding="utf-8")
            status = {"pass": "SURVIVED", "fail": "caught", "error": "caught", "no-build": "no-build"}[verdict]
            results.append({"kind": m.kind, "line": m.line, "desc": m.desc, "status": status, "detail": detail,
                            "region": rid})
            if a.verbose or status == "SURVIVED":
                print(f"  [{i}/{len(chosen)}] {status:8} {m.kind:7} L{m.line}: {m.desc}" + (f" [{rid}]" if rid else "")
                      + (f"   ({detail})" if a.verbose and detail else ""))
    except KeyboardInterrupt:
        interrupted = True
        print("\ninterrupted", file=sys.stderr)
    finally:
        if sandbox is not None:
            sandbox.close()

    caught = sum(r["status"] == "caught" for r in results)
    surv = [r for r in results if r["status"] == "SURVIVED"]
    nob = sum(r["status"] == "no-build" for r in results)
    counted = caught + len(surv)
    print(f"{va:#010x}: {caught}/{counted} caught, {len(surv)} survived, {nob} didn't build "
          f"({time.time() - t0:.0f} s{', interrupted' if interrupted else ''}"
          f"{f'; {filtered} rerun on every case after passing the region cases' if filtered else ''})")
    for r in surv:
        print(f"  SURVIVED {r['kind']:7} L{r['line']}: {r['desc']}" + (f" [{r['region']}]" if r.get("region") else ""))
    per_region = region_report(blocks, results, available) if blocks else {}
    for rid, d in per_region.items():
        print(f"  {rid:6} {d['caught']}/{d['caught'] + d['survived']} caught, {d['survived']} survived"
              + (f", {d['no_build']} didn't build" if d["no_build"] else "")
              + (f"  ({d['available']} mutants available)" if d["available"] > d["checked"] else "")
              + ("  NO MUTANTS (nothing mutable written yet)" if not d["available"] else ""))
    if a.json:
        Path(a.json).write_text(json.dumps({"va": f"{va:#010x}", "source": repo.rel(src),
                                            "fixtures": [repo.rel(f) for f in fixtures], "mutants": len(ms),
                                            "results": results, "caught": caught, "survived": len(surv),
                                            "no_build": nob, "regions": per_region}, indent=1), encoding="utf-8")
    if interrupted:
        return 130
    return 1 if surv else 0


if __name__ == "__main__":
    sys.exit(main())
