r"""objdiff (github.com/encounter/objdiff) for a decomp repo: config, base builds, agreement check, progress.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py write          -> objdiff.json (+ base objects)
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py build OBJ      what objdiff runs to rebuild a base
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py build --all    every base object
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py check [--match-json M]   objdiff vs compare --match
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py report [--out F]         progress snapshot
    ..\.venv\Scripts\python.exe tools\compare\objdiff_config.py --client eqmain report   eqmain.dll (see below)

Units: one per source file (`src\EverQuest\Display.cpp` -> unit `EverQuest/Display`), holding its annotated
functions plus the queued functions of the queue subsystem of the same name that no source has yet; a queue
subsystem without a source is a unit of its own (target only, so it counts as unmatched in the progress).
  target_path  the delinked object of the unit's functions: <build>\<client>\delink\units\<unit>.obj
               (ghidra\scripts\delink.py <client> --units)
  base_path    the match-mode object of the source (compare.MatchContext.compile: EQD_MATCH, SP4, the
               source's flags): <build>\<client>\objdiff\<obj name>.obj
  symbol_mappings  `_eqd_fn_0x<VA>` (the delinked name) -> the rewrite's decorated name, so objdiff pairs them
Paths in objdiff.json are relative to the repo; the build command is `cmd /c ..\.venv\Scripts\python.exe
tools\compare\objdiff_config.py build`, which objdiff runs in the repo with the base path appended.
Another image of the repo (`--client eqmain`): its own sources (src\eqmain\), queue and toolchain; its
objdiff.json goes to <build>\<client>\objdiff_project\ (the repo's own is eqgame.exe's), and the build command
carries `--client <client> --project .`.
objdiff-cli 3.8.2 never finishes on some delinked functions (eqmain FUN_10008150 in unit_10008090, and one in
unit_10018B60; target-only units): `report`/`check` give up after --timeout seconds and say so; `--skip-unit NAME`
(repeatable) leaves a unit out of objdiff.json.
"""
from __future__ import annotations

import argparse
import collections
import datetime
import json
import os
import re
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "unicorn"))
sys.path.insert(0, str(HERE))

SCHEMA = "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json"
OUT_OF_QUEUE = {"library", "skip"}
DEFAULT_CLI = r"downloads\tools\objdiff\v3.8.2\objdiff-cli-windows-x86_64.exe"


@dataclass
class Unit:
    name: str
    source: str | None                                  # repo-relative posix path, or None
    vas: list = field(default_factory=list)             # original VAs of its functions, sorted
    annotated: dict = field(default_factory=dict)       # VA -> annotated name (functions of the source)


def eqd_fn(va: int) -> str:
    return f"_eqd_fn_0x{va:08X}"


def sources_of(repo: Path, client="trilogy"):
    """The image's own sources (clients.sources: eqgame.exe without src/eqmain/, eqmain only that)."""
    from eqd_unicorn import clients
    return clients.sources(repo, client)


def default_project(ws, repo: Path, client: str) -> Path:
    """Where objdiff.json goes: the repo for its primary image, <build>/<client>/objdiff_project otherwise."""
    from eqd_unicorn import clients
    return Path(repo) if clients.get(client).primary else ws.build / client / "objdiff_project"


def plan_units(repo: Path, entries: list, client="trilogy") -> list[Unit]:
    """The units, in name order. A function belongs to the unit of the source that annotates it (first
    one if several), else to its queue subsystem's unit (queued functions only: not library/skip)."""
    from eqd_unicorn import annotations as ann
    repo = Path(repo)
    units: dict = {}
    placed: set = set()
    for src in sources_of(repo, client):
        rel = src.relative_to(repo).as_posix()
        name = src.relative_to(repo / "src").with_suffix("").as_posix()
        u = units.setdefault(name, Unit(name, rel))
        if u.source != rel:                             # Foo.c and Foo.cpp: keep both apart
            name = src.relative_to(repo / "src").as_posix()
            u = units.setdefault(name, Unit(name, rel))
        for a in ann.scan([src], client.upper()):
            if a.va not in placed:
                placed.add(a.va)
                u.annotated[a.va] = a.name
    for e in entries:
        if e.get("removed") or e.get("state") in OUT_OF_QUEUE:
            continue
        va = int(e["address"], 16)
        if va in placed:
            continue
        placed.add(va)
        sub = e.get("subsystem") or "unassigned"
        units.setdefault(sub, Unit(sub, None)).vas.append(va)
    for u in units.values():
        u.vas = sorted(set(u.vas) | set(u.annotated))
    return [units[k] for k in sorted(units) if units[k].vas]


def obj_name(repo: Path, rel_source: str) -> str:
    """The object file name compare.compile_match_obj gives a source."""
    return re.sub(r"[^A-Za-z0-9]+", "_", Path(rel_source).with_suffix("").as_posix()) + ".obj"


def _rel(path: Path, start: Path) -> str:
    try:
        return Path(os.path.relpath(path, start)).as_posix()
    except ValueError:                                  # another drive
        return Path(path).as_posix()


def base_symbols(obj: Path, annotated: dict) -> dict:
    """{`_eqd_fn_0x<VA>`: decorated name} for the annotated functions found in a match object (unique
    undecorated name; overloads that the name alone can't tell apart are left out)."""
    import coff
    from eqd_unicorn import undecorate
    o = coff.parse(obj)
    by_und: dict = collections.defaultdict(set)
    for s in o.symbols:
        if s and s.section > 0 and s.type & 0x20 and s.storage == 2:
            by_und[undecorate(s.name)].add(s.name)
    out = {}
    for va, name in annotated.items():
        hits = by_und.get(name, set())
        if len(hits) == 1:
            out[eqd_fn(va)] = next(iter(hits))
    return out


def config(repo: Path, units: list, delink_dir: Path, base_dir: Path, mappings: dict | None = None,
           project_dir: Path | None = None, client: str = "trilogy") -> dict:
    project_dir = Path(project_dir or repo)
    out_units = []
    for u in units:
        d = {"name": u.name,
             "target_path": _rel(delink_dir / "units" / f"{u.name}.obj", project_dir)}
        if u.source:
            d["base_path"] = _rel(base_dir / obj_name(repo, u.source), project_dir)
        d["metadata"] = {"complete": False, "progress_categories": [u.name.split("/")[0]]}
        if u.source:
            d["metadata"]["source_path"] = u.source
        if mappings and mappings.get(u.name):
            d["symbol_mappings"] = dict(sorted(mappings[u.name].items()))
        out_units.append(d)
    cats = sorted({u.name.split("/")[0] for u in units})
    py = _rel(Path(sys.executable), project_dir).replace("/", "\\")          # the venv running this
    return {
        "$schema": SCHEMA,
        "custom_make": "cmd",
        "custom_args": ["/c", py, _rel(HERE / "objdiff_config.py", project_dir).replace("/", "\\"), "build",
                        "--repo", _rel(repo, project_dir) or "."]
                       + ([] if client == "trilogy" else ["--client", client, "--project", "."]),
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["src/**/*.c", "src/**/*.cpp", "src/**/*.h", "include/**/*.h"],
        "units": out_units,
        "progress_categories": [{"id": c, "name": c} for c in cats],
    }


class Repo:
    """What the subcommands share: workspace, queue, units, folders."""

    def __init__(self, repo, client="trilogy", queue=None, delink_dir=None, base_dir=None):
        from eqd_unicorn import Workspace
        self.repo, self.client = Path(repo).resolve(), client
        self.ws = Workspace(self.repo)
        from eqd_unicorn import clients
        self.queue = Path(queue) if queue else self.repo / clients.get(client).queue
        self.delink_dir = Path(delink_dir or self.ws.build / client / "delink").resolve()
        self.base_dir = Path(base_dir or self.ws.build / client / "objdiff").resolve()
        self._units = None

    @property
    def units(self) -> list:
        if self._units is None:
            if not self.queue.is_file():
                raise SystemExit(f"objdiff units need a queue file, and there is none at {self.queue}")
            entries = [json.loads(ln) for ln in self.queue.read_text(encoding="utf-8").splitlines() if ln.strip()]
            self._units = plan_units(self.repo, entries, self.client)
        return self._units

    def build(self, units=None, log=print) -> dict:
        """Compile the base objects. -> {unit name: obj path or None}."""
        import compare
        ctx = compare.MatchContext(self.repo, self.client, self.base_dir, log=log)
        out = {}
        for u in units or self.units:
            if not u.source:
                continue
            obj, text, _ = ctx.compile(self.repo / u.source, self.base_dir)
            if obj is None:
                log(f"objdiff build: {u.source} failed: "
                    + "; ".join(ln.strip() for ln in text.splitlines() if "error" in ln)[:300])
            out[u.name] = obj
        return out

    def mappings(self, built: dict) -> dict:
        return {u.name: base_symbols(built[u.name], u.annotated) for u in self.units
                if u.source and built.get(u.name)}

    def stale_units(self) -> list:
        """Units whose delinked object is missing or was made for other functions (delink.json)."""
        man = self.delink_dir / "delink.json"
        have = json.loads(man.read_text(encoding="utf-8")).get("units", {}) if man.is_file() else {}
        out = []
        for u in self.units:
            h = have.get(u.name)
            if not h or not h.get("ok") or not (self.delink_dir / h["obj"]).is_file() or \
                    set(h.get("functions", [])) | set(h.get("missing", [])) != {f"{v:08X}" for v in u.vas}:
                out.append(u.name)
        return out


def objdiff_cli(ws) -> Path:
    if ws.cfg.has_option("toolchain", "objdiff_cli"):
        return Path(ws.cfg["toolchain"]["objdiff_cli"])
    return ws.root / DEFAULT_CLI


def run_report(cli: Path, project: Path, out: Path, timeout: float | None = None) -> dict:
    out.parent.mkdir(parents=True, exist_ok=True)
    try:
        p = subprocess.run([str(cli), "--no-color", "report", "generate", "-p", str(project), "-o", str(out),
                            "-f", "json-pretty"], capture_output=True, text=True, errors="replace", timeout=timeout)
    except subprocess.TimeoutExpired:
        raise SystemExit(f"objdiff-cli report did not finish in {timeout:.0f}s: it hangs on some delinked "
                         "functions (see the module doc); leave their units out with --skip-unit")
    if p.returncode != 0:
        raise SystemExit(f"objdiff-cli report failed ({p.returncode}): {(p.stderr or p.stdout)[-800:]}")
    return json.loads(out.read_text(encoding="utf-8"))


def report_functions(rep: dict) -> dict:
    """{VA: (fuzzy_match_percent, unit)} for the delinked functions (`_eqd_fn_0x<VA>`) in a report."""
    out = {}
    for u in rep.get("units", []):
        for f in u.get("functions", []):
            m = re.fullmatch(r"_?eqd_fn_0x([0-9A-Fa-f]+)", f.get("name", ""))
            if m:
                out[int(m.group(1), 16)] = (float(f.get("fuzzy_match_percent", 0.0)), u.get("name"))
    return out


def agreement(match_results: list, objdiff: dict) -> dict:
    """compare.py --match statuses against objdiff's per-function score: `identical` should be 100%,
    anything else below. -> counts per (status, objdiff verdict) and the disagreeing VAs."""
    cells = collections.Counter()
    disagree = {"identical_below_100": [], "not_identical_at_100": []}
    for r in match_results:
        va = int(r["va"], 16)
        st = r.get("match")
        if va not in objdiff or st is None:
            cells[(st, "absent")] += 1
            continue
        pct = objdiff[va][0]
        full = pct >= 100.0
        cells[(st, "100" if full else "<100")] += 1
        if st == "identical" and not full:
            disagree["identical_below_100"].append((r["va"], r.get("name"), pct))
        elif st != "identical" and full:
            disagree["not_identical_at_100"].append((r["va"], r.get("name"), st))
    ident = cells[("identical", "100")] + cells[("identical", "<100")] + cells[("identical", "absent")]
    return {"cells": {f"{k[0]}/{k[1]}": v for k, v in sorted(cells.items(), key=str)},
            "identical": ident, "identical_agree": cells[("identical", "100")], **disagree}


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", choices=["write", "build", "check", "report", "units"])
    ap.add_argument("objs", nargs="*", help="build: base object paths (as in objdiff.json)")
    ap.add_argument("--repo", default=".")
    ap.add_argument("--client", default="trilogy")
    ap.add_argument("--queue")
    ap.add_argument("--delink-dir", help="default <build>\\<client>\\delink")
    ap.add_argument("--base-dir", help="default <build>\\<client>\\objdiff")
    ap.add_argument("--project", help="where objdiff.json goes (default: the repo)")
    ap.add_argument("--config-out", help="write: put objdiff.json here instead (paths stay relative to --project)")
    ap.add_argument("--all", action="store_true", help="build: every unit's base object")
    ap.add_argument("--match-json", help="check: compare.py --match --json results (default: run compare)")
    ap.add_argument("--out", help="report/check: output JSON (report default <build>\\<client>\\progress\\<date>.json)")
    ap.add_argument("--skip-unit", action="append", default=[], help="leave this unit out (repeatable)")
    ap.add_argument("--timeout", type=float, default=900, help="report/check: seconds objdiff-cli may take (900)")
    a = ap.parse_args(argv)
    r = Repo(a.repo, a.client, a.queue, a.delink_dir, a.base_dir)
    if a.skip_unit:
        r._units = [u for u in r.units if u.name not in set(a.skip_unit)]
    project = Path(a.project).resolve() if a.project else default_project(r.ws, r.repo, a.client)

    if a.command == "units":
        for u in r.units:
            print(f"{u.name}\t{u.source or '-'}\t{len(u.vas)} functions ({len(u.annotated)} annotated)")
        return 0
    if a.command == "build":
        if a.all:
            todo = r.units
        else:
            want = {Path(os.path.normcase(os.path.abspath(project / o))) for o in a.objs}
            todo = [u for u in r.units if u.source and
                    Path(os.path.normcase(os.path.abspath(r.base_dir / obj_name(r.repo, u.source)))) in want]
            if len(todo) != len(want):
                print(f"objdiff build: no unit for some of {a.objs}", file=sys.stderr)
        built = r.build(todo)
        return 0 if all(built.values()) and (a.all or len(todo) == len(a.objs)) else 1

    if a.command in ("write", "check", "report"):
        built = r.build()
        cfg = config(r.repo, r.units, r.delink_dir, r.base_dir, r.mappings(built), project, a.client)
        dest = Path(a.config_out) if a.config_out and a.command == "write" else project / "objdiff.json"
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_text(json.dumps(cfg, indent=2) + "\n", encoding="utf-8")
        stale = r.stale_units()
        print(f"wrote {dest}: {len(cfg['units'])} units "
              f"({sum(1 for u in r.units if u.source)} with a source), "
              f"{sum(len(m) for m in r.mappings(built).values())} symbol mappings")
        if stale:
            print(f"  {len(stale)} units have no or a stale delinked object; refresh with: "
                  f"ghidra\\scripts\\delink.py {a.client} --units --no-functions")
        if a.command == "write":
            return 0
        absent = [u["name"] for u in cfg["units"] if not (project / u["target_path"]).is_file()]
        if absent:
            print(f"objdiff {a.command}: skipped, {len(absent)} units have no delinked object yet "
                  f"(e.g. {absent[0]}); run ghidra\\scripts\\delink.py {a.client} --units first")
            return 0 if a.command == "report" else 1
    cli = objdiff_cli(r.ws)
    date = datetime.date.today().isoformat()
    if a.command == "report":
        out = Path(a.out) if a.out else r.ws.build / a.client / "progress" / f"{date}.json"
        rep = run_report(cli, project, out, a.timeout)
        m = rep.get("measures", {})
        print(f"objdiff report -> {out}: matched code {float(m.get('matched_code_percent', 0)):.2f}% "
              f"({m.get('matched_code', 0)}/{m.get('total_code', 0)} bytes), fuzzy "
              f"{float(m.get('fuzzy_match_percent', 0)):.2f}%, functions {m.get('matched_functions', 0)}/"
              f"{m.get('total_functions', 0)}")
        return 0
    # check
    rep = run_report(cli, project, r.base_dir / "objdiff_check_report.json", a.timeout)
    od = report_functions(rep)
    if a.match_json:
        results = json.loads(Path(a.match_json).read_text(encoding="utf-8"))["results"]
    else:
        import compare
        res = compare.compare_match(r.repo, sources_of(r.repo, a.client), a.client, r.base_dir / "compare")
        results = [dict(x.as_json(), match=compare.match_status(x)) for x in res]
    ag = agreement(results, od)
    print(f"objdiff vs compare --match: {ag['identical_agree']}/{ag['identical']} byte-identical functions at "
          f"100% in objdiff; {len(ag['identical_below_100'])} identical below 100%, "
          f"{len(ag['not_identical_at_100'])} not identical at 100%")
    print("  " + ", ".join(f"{k} {v}" for k, v in ag["cells"].items()))
    if a.out:
        Path(a.out).write_text(json.dumps(ag, indent=1) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
