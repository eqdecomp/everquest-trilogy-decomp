r"""VC6 permuter: random source mutations of one near-miss function, scored by compiling in match mode.

    cd eq-trilogy        (or a snapshot of it, see --snapshot)
    ..\.venv\Scripts\python.exe tools\permute\permute.py --va 0x00414E7A
    ..\.venv\Scripts\python.exe tools\permute\permute.py --nearmiss ..\build\match\nearmiss.txt --budget 600
    ..\.venv\Scripts\python.exe tools\permute\permute.py --snapshot --nearmiss ..\build\match\nearmiss.txt
    ..\.venv\Scripts\python.exe tools\permute\permute.py -c eqmain --nearmiss ..\build\match\eqmain-nearmiss.txt

For the function annotated `// FUNCTION: TRILOGY <va>` (`-c eqmain`: EQMAIN, its sources under src/eqmain/, built
with that image's toolchain and flags, as compare.py --match -c eqmain):
1. its source is cut down to a translation unit with the target and what it needs (tu.py), checked to give
   the target the same code as the full source (else the full source is used);
2. candidates (the body with 1+ random mutations, mutate.py) are compiled like `compare.py --match`
   (`/DEQD_MATCH`, the source's CFLAGS, the generated eqd_match_syms.h) and compared with the original by
   compare.MatchContext: score 0 = byte-identical, 1 = instruction-identical (bytes differ: EQ_THISCALL's
   dummy EDX), else ordered by compare ratio, then differing instruction count (score_of), None = no build;
3. hill climbing with a pool of equal-best candidates (plateau drift), parallel compiles;
4. the best candidate is minimised (hunks that don't help are reverted), checked like at landing (the whole
   source compiled as compare.py --match does: no function of it may get a worse status or a lower ratio,
   else the earlier bests are tried; FullCheck), verified with the function's Unicorn fixtures (the repo's
   tests/unicorn conftest, EQD_TOOLS = this tools tree) and written as a unified diff
   against the source to <out>\<VA>.diff, plus a line in <out>\summary.tsv.
The decomp repo itself is never written to. See README.md.
"""
from __future__ import annotations

import argparse
import concurrent.futures as cf
import difflib
import hashlib
import json
import os
import random
import re
import shutil
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
TOOLS = HERE.parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(TOOLS / "compare"))
sys.path.insert(0, str(TOOLS / "unicorn"))

import compare  # noqa: E402
import tu  # noqa: E402
from ctoken import norm  # noqa: E402
from eqd_unicorn import Workspace, annotations as ann, clients  # noqa: E402
from mutate import MUTATIONS, Body, mutate, temp_candidates  # noqa: E402

CLIENT = "trilogy"          # the image (-c); set by main() and in every worker (worker_init)
SNAPSHOT_DIRS = ("include", "src", "names", "hooks", "tests/unicorn")


# --- target --------------------------------------------------------------------------------------------------

@dataclass
class Target:
    va: int
    name: str
    rel: str                 # source path relative to the repo (posix)
    text: str                # the full source
    fdef: object             # tu.FuncDef
    unit: object             # tu.Unit (what gets compiled)
    cpp: bool

    @property
    def body(self):
        return self.unit.body

    def full_text(self, body):
        return self.text[:self.fdef.body_start] + body + self.text[self.fdef.body_end:]


def all_sources(repo: Path):
    """The image's own sources (clients.sources: eqgame.exe skips src/eqmain/, eqmain takes only it)."""
    return clients.sources(repo, CLIENT)


def set_client(client: str) -> None:
    global CLIENT
    CLIENT = client


def default_dir(ws: Workspace, *parts) -> Path:
    """<build>/<parts> for eqgame.exe (as before), <build>/<parts[0]>/<client>/<rest> for another image."""
    d = ws.build / parts[0]
    return d.joinpath(*parts[1:]) if CLIENT == "trilogy" else d.joinpath(CLIENT, *parts[1:])


def find_annotation(repo: Path, va: int, source=None):
    for src in ([repo / source] if source else all_sources(repo)):
        for a in ann.scan([src], CLIENT.upper()):
            if a.va == va:
                return src, a
    raise SystemExit(f"no // FUNCTION: {CLIENT.upper()} 0x{va:08X} annotation in {repo / 'src'}")


def load_target(repo: Path, va: int, source=None) -> Target:
    src, a = find_annotation(repo, va, source)
    text = src.read_text(encoding="utf-8", errors="replace")
    fdef = tu.find_target(text, a.line)
    if fdef is None:
        raise SystemExit(f"{src}: no definition after the annotation of 0x{va:08X}")
    unit = tu.extract(text, fdef)
    return Target(va, a.name, src.relative_to(repo).as_posix(), text, fdef, unit, src.suffix.lower() == ".cpp")


def by_name_of(repo: Path) -> dict:
    """annotated name -> {VAs} over every source (as `compare.py --match` over the whole repo keys calls)."""
    out: dict = {}
    for a in ann.scan(all_sources(repo), CLIENT.upper()):
        out.setdefault(a.name, set()).add(a.va)
    return out


# --- scoring (worker processes) ------------------------------------------------------------------------------

_W: dict = {}


def worker_init(repo, ctx_dir, work, by_name, aliases, va, filename, client="trilogy", like=None):
    """`like`: the repo source the candidates stand in for (its image's flags and toolchain folder rules)."""
    set_client(client)
    ctx = compare.MatchContext(Path(repo), CLIENT, ctx_dir, aliases, log=lambda *a: None)
    wdir = Path(work) / f"w{os.getpid()}"
    wdir.mkdir(parents=True, exist_ok=True)
    ctx.out_dir = wdir                       # overload listings of this worker
    _W.update(ctx=ctx, wdir=wdir, by_name=by_name, va=va, filename=filename, like=like)


# `register` pinning is banned in the decomp's sources (docs/RULES.md, fake matches), so the search never adds it
SEARCHED = frozenset(n for n in MUTATIONS if n != "register")
RATIO_UNIT = 1000      # score units per 0.0001 of compare ratio (above it: the instruction diff count)
NEAR = 50 * RATIO_UNIT  # side steps: candidates within this of the best (0.005 of ratio)
STATUS_RANK = {"identical": 0, "normalised": 1, "diff": 2, "structural": 2, "error": 3}


def score_of(res) -> int | None:
    """Lower is better, ordered like the acceptance check of a match change (compare.py --match, function by
    function against a baseline: status first, then ratio): 0 byte-identical, 1 normalised (instruction-
    identical, bytes differ: EQ_THISCALL's dummy EDX), else 2 + RATIO_UNIT per 0.0001 the compare ratio is below
    1 + the differing instruction count (a tie-break between equal ratios). None: error / no build.
    So a candidate with a lower score never has a lower ratio. The count alone is a poor guide: it compares
    labelled tokens position by position (one extra instruction renumbers every later branch label) and
    used to add 1000 for a different instruction count, while the ratio is an alignment of label-free tokens;
    a same-count candidate full of register swaps scored better than a one-instruction-longer near match."""
    if res is None or res.verdict == "error":
        return None
    if res.verdict.startswith("identical"):
        return 0 if res.bytes_identical else 1
    return 2 + RATIO_UNIT * round((1 - res.ratio) * 10000) + min(res.ndiff, RATIO_UNIT - 1)


def not_worse(before: dict, after: dict) -> bool:
    """`after` passes the acceptance check against `before` ({"status", "ratio"} of one function, compare.py
    --match): its status is not worse and its ratio not lower."""
    rb, ra = STATUS_RANK.get(before["status"], 3), STATUS_RANK.get(after["status"], 3)
    return ra <= rb and (ra < 2 or after["ratio"] >= before["ratio"])


def worker_score(text: str, want_diff=False) -> dict:
    ctx, wdir = _W["ctx"], _W["wdir"]
    src = wdir / _W["filename"]
    src.write_text(text, encoding="utf-8")
    try:
        obj, out, flags = ctx.compile(src, wdir, _W.get("like"))
    except Exception as e:                      # noqa: BLE001  (e.g. BuildError for a bad CFLAGS line)
        return {"score": None, "error": f"{type(e).__name__}: {e}"}
    if obj is None:
        err = "; ".join(ln.strip() for ln in out.splitlines() if "error" in ln)[:300]
        return {"score": None, "error": err}
    try:
        anns = ann.scan([src], CLIENT.upper())
        todo = [a for a in anns if a.va == _W["va"]]
        res = ctx.compare_obj(obj, src, anns, _W["by_name"], todo, src.name, flags)[0]
    except Exception as e:                      # noqa: BLE001
        return {"score": None, "error": f"{type(e).__name__}: {e}"}
    finally:
        try:
            obj.unlink()
        except OSError:
            pass
    d = {"score": score_of(res), "status": compare.match_status(res), "ratio": res.ratio,
         "verdict": res.verdict, "ndiff": res.ndiff, "structural": res.structural,
         "bytes": res.bytes_identical, "first_diff": res.first_diff, "error": "",
         "listing": [ln.split("  ", 1)[1] for ln in res.rew_listing]}
    if want_diff:
        d["diff"] = res.diff
    return d


def worker_compile_output(text: str) -> str:
    """Compile and return the compiler's messages (type probes)."""
    ctx, wdir = _W["ctx"], _W["wdir"]
    src = wdir / ("probe_" + _W["filename"])
    src.write_text(text, encoding="utf-8")
    obj, out, flags = ctx.compile(src, wdir, _W.get("like"))
    return out


# --- type probes -----------------------------------------------------------------------------------------------

_PROBE_RX = [re.compile(r"\((\d+)\) : error C2440: 'initializing' : cannot convert from '(.+?)' to 'struct eqd_probe_s'"),
             re.compile(r"\((\d+)\) : warning C4047: 'initializing' : 'struct eqd_probe_s \*' differs in levels of "
                        r"indirection from '(.+?)'"),
             re.compile(r"\((\d+)\) : warning C4133: 'initializing' : incompatible types - from '(.+?)' to "
                        r"'struct eqd_probe_s \*'")]


def usable_type(t: str) -> str | None:
    t = t.strip()
    if not t or "[" in t or "(" in t or "::*" in t or t == "void" or t.startswith("void ") and "*" not in t:
        return None
    if re.match(r"^(struct|class|union)\b", t) and "*" not in t:
        return None                                  # a struct copy: not a temporary worth making
    if "*" not in t:
        t = re.sub(r"^const\s+", "", t)
    return t


def probe_types(target: Target, run_compile, batch=60) -> dict:
    """{normalised expression: type} for the subexpressions add_temp may hoist, asked from the compiler:
    `{ struct eqd_probe_s eqd_pN = (expr); }` in front of the statement makes cl name the type (C++: C2440;
    C: C4047/C4133 with a pointer probe, integers and pointers only)."""
    B = Body(target.body, target.fdef.params, target.cpp)
    cands = temp_candidates(B, typed=False)
    per_stmt: dict = {}
    for st, blk, idx, j, e, lo, hi in cands:
        per_stmt.setdefault(st.a, (st, set()))[1].add(B.span(j, e))
    items = [(st, x) for st, xs in per_stmt.values() for x in sorted(xs)]
    types: dict = {}
    for k in range(0, len(items), batch):
        chunk = items[k:k + batch]
        inserts: dict = {}
        for n, (st, x) in enumerate(chunk):
            decl = "struct eqd_probe_s eqd_p%d" % n if target.cpp else "struct eqd_probe_s *eqd_p%d" % n
            inserts.setdefault(B.S[st.a].start, []).append(f"{{ {decl} = ({x}); }}\n")
        body = B.text
        for pos in sorted(inserts, reverse=True):
            body = body[:pos] + "".join(inserts[pos]) + body[pos:]
        text = "struct eqd_probe_s { int eqd_x; };\n" + target.unit.text(body)
        line_of = {}
        for n in range(len(chunk)):
            p = text.find(f"eqd_p{n} =")
            line_of[text.count("\n", 0, p) + 1] = chunk[n][1]
        out = run_compile(text)
        for ln in out.splitlines():
            for rx in _PROBE_RX:
                m = rx.search(ln)
                if m and int(m.group(1)) in line_of:
                    t = usable_type(m.group(2))
                    if t:
                        types[norm(line_of[int(m.group(1))])] = t
    return types


# --- search ----------------------------------------------------------------------------------------------------

@dataclass
class Cand:
    body: str
    steps: tuple = ()           # ((mutation, body before, body after), ...) from the unmodified body
    score: int | None = None
    info: dict = field(default_factory=dict)

    @property
    def muts(self):
        return [s[0] for s in self.steps]


def _h(s: str) -> str:
    return hashlib.sha1(s.encode()).hexdigest()


def search(target: Target, pool, budget_s: float, max_iter: int, jobs: int, seed: int, types: dict,
           allow_unsafe=True, log=print, stop_at=1, history=None):
    """Hill climbing: each candidate is a best (or near-best) one with 1-4 more mutations; equal scores join
    the pool (plateau drift). -> (base, best with the fewest steps, stats). `history`: a list that gets each
    new best in turn (the fallbacks when the best fails the whole-source check)."""
    rng = random.Random(seed)
    base = Cand(target.body)
    base.info = pool.submit(worker_score, target.unit.text(), True).result()
    base.score = base.info["score"]
    if base.score is None:
        raise RuntimeError(f"the unmodified function doesn't build: {base.info.get('error')}")
    best = [base]
    near = [base]                                  # within NEAR of the best, for an occasional side step
    seen = {_h(base.body)}
    stats = {"iterations": 0, "compiled": 0, "failed": 0, "dupes": 0, "by_mut": {}}
    t0 = time.time()
    inflight: dict = {}
    params = target.fdef.params
    while True:
        elapsed = time.time() - t0
        done_budget = elapsed > budget_s or stats["iterations"] >= max_iter or best[0].score <= stop_at
        while not done_budget and len(inflight) < jobs * 2:
            parent = rng.choice(near if rng.random() < 0.15 else best)
            body, steps = parent.body, list(parent.steps)
            n = 1
            while n < 4 and rng.random() < 0.4:
                n += 1
            for _ in range(n):
                r = mutate(body, rng, params, target.cpp, types, allow_unsafe, only=SEARCHED)
                if r:
                    steps.append((r[1], body, r[0]))
                    body = r[0]
            h = _h(body)
            stats["iterations"] += 1
            if h in seen:
                stats["dupes"] += 1
                if stats["dupes"] > 50 * (stats["compiled"] + 10):
                    done_budget = True          # nothing new left to try
                continue
            seen.add(h)
            inflight[pool.submit(worker_score, target.unit.text(body))] = Cand(body, tuple(steps))
        if not inflight:
            break
        done, _ = cf.wait(list(inflight), return_when=cf.FIRST_COMPLETED)
        for fut in done:
            c = inflight.pop(fut)
            try:
                c.info = fut.result()
            except Exception as e:                  # noqa: BLE001
                c.info = {"score": None, "error": str(e)}
            c.score = c.info["score"]
            stats["compiled"] += 1
            if c.steps:
                s = stats["by_mut"].setdefault(c.steps[-1][0], [0, 0])
                s[0] += 1
                s[1] += c.score is None
            if c.score is None:
                stats["failed"] += 1
                continue
            if c.score < best[0].score:
                best = [c]
                if history is not None:
                    history.append(c)
                log(f"    {time.time() - t0:6.1f}s  score {c.score:8d}  {c.info.get('status')} ratio "
                    f"{c.info.get('ratio')} ndiff {c.info.get('ndiff')}  ({', '.join(c.muts)})")
                near = [x for x in near if x.score <= c.score + NEAR] + [c]
            elif c.score == best[0].score and len(best) < 64:
                best.append(c)
            elif c.score <= best[0].score + NEAR:
                near.append(c)
                if len(near) > 64:
                    near.pop(rng.randrange(len(near)))
    stats["seconds"] = round(time.time() - t0, 1)
    return base, min(best, key=lambda c: len(c.steps)), stats


def revert_step(body: str, before: str, after: str, ctx=2) -> str | None:
    """`body` without the change one mutation made (before -> after): every hunk of that change is put back,
    found by its text with `ctx` lines of context; None when a hunk isn't found exactly once (a later
    mutation touched it). All hunks or none, so a mutation is never half reverted."""
    a, b = before.splitlines(keepends=True), after.splitlines(keepends=True)
    ops = [o for o in difflib.SequenceMatcher(None, a, b, autojunk=False).get_opcodes() if o[0] != "equal"]
    edits = []
    for tag, i1, i2, j1, j2 in ops:
        c0, c1 = max(0, j1 - ctx), min(len(b), j2 + ctx)
        old = "".join(b[c0:c1])
        if not old or body.count(old) != 1:
            return None
        s = body.index(old) + len("".join(b[c0:j1]))
        edits.append((s, s + len("".join(b[j1:j2])), "".join(a[i1:i2])))
    edits.sort()
    if any(x[1] > y[0] for x, y in zip(edits, edits[1:])):
        return None
    out = body
    for s, e, r in reversed(edits):
        out = out[:s] + r + out[e:]
    return out


def minimise(target: Target, pool, base: Cand, best: Cand, log=print) -> Cand:
    """Drop the mutations of the best candidate's path that don't matter: revert one whole mutation at a time
    (revert_step) and keep the reversion when the score doesn't get worse."""
    cur = best
    tried = set()
    changed = True
    while changed:
        changed = False
        trials = []
        for k, (name, before, after) in enumerate(cur.steps):
            body = revert_step(cur.body, before, after)
            if body is None or body == cur.body or _h(body) in tried:
                continue
            tried.add(_h(body))
            trials.append((k, body, pool.submit(worker_score, target.unit.text(body))))
        for k, body, fut in trials:
            r = fut.result()
            if r["score"] is not None and r["score"] <= cur.score:
                steps = cur.steps[:k] + cur.steps[k + 1:]
                cur = Cand(body, steps, r["score"], r)
                changed = True
                break
    if len(cur.steps) < len(best.steps):
        log(f"    minimised: {len(best.steps)} -> {len(cur.steps)} mutations ({', '.join(cur.muts)})")
    return cur


# --- verification ----------------------------------------------------------------------------------------------

def fixtures_for(repo: Path, va: int) -> list[Path]:
    root = repo / "tests" / "unicorn"
    out = []
    for p in sorted(root.rglob("*.json")):
        if "negative" in p.parts or p.parent == root:
            continue
        try:
            d = json.loads(p.read_text(encoding="utf-8"))
        except (ValueError, OSError):
            continue
        o = d.get("original") if isinstance(d, dict) else None
        if isinstance(o, str) and o.lower().startswith("0x") and int(o, 16) == va:
            out.append(p)
    return out


def run_fixtures(repo: Path, target: Target, body: str, work: Path, ub: Path, tools: Path, log=print):
    """Run the target's Unicorn fixtures against the source with `body` (in a copy of the repo's src/include/
    names/hooks and the fixtures). -> (True/False/None, detail); None: no fixture."""
    fx = fixtures_for(repo, target.va)
    if not fx:
        return None, "no fixture"
    vroot = work
    if vroot.exists():
        shutil.rmtree(vroot, ignore_errors=True)
    for d in ("include", "src", "names", "hooks"):
        if (repo / d).is_dir():
            shutil.copytree(repo / d, vroot / d)
    (vroot / "tests" / "unicorn").mkdir(parents=True, exist_ok=True)
    for f in (repo / "tests" / "unicorn").glob("*"):
        if f.is_file():
            shutil.copy2(f, vroot / "tests" / "unicorn" / f.name)
    rels = []
    for p in fx:
        rel = p.relative_to(repo)
        (vroot / rel).parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(p, vroot / rel)
        rels.append(rel.as_posix())
    (vroot / target.rel).write_text(target.full_text(body), encoding="utf-8")
    env = dict(os.environ, EQD_TOOLS=str(tools), EQD_UNICORN_BUILD=str(ub))
    p = subprocess.run([sys.executable, "-m", "pytest", "-q", "-p", "no:cacheprovider", *rels], cwd=vroot,
                       env=env, capture_output=True, text=True, errors="replace")
    tail = (p.stdout.strip().splitlines() or [""])[-1]
    if "skipped" in tail and "passed" not in tail:
        return False, f"skipped: {tail}"
    return p.returncode == 0, f"{len(fx)} fixture(s): {tail}" + ("" if p.returncode == 0 else
                                                                 "\n" + p.stdout[-3000:])


# --- driver ----------------------------------------------------------------------------------------------------

def status_of(c) -> str:
    """compare.match_status of a scored candidate (Cand or its info dict)."""
    info = c.info if isinstance(c, Cand) else c
    return info.get("status") or ("error" if info.get("score") is None else "?")


class FullCheck:
    """The acceptance check of a winner, as at landing: the whole source with the new body, compiled in match
    mode like `compare.py --match` (the real file path in a copy of src/, cross-TU call keys) and every
    annotated function in it compared with the unmodified source. The search scores a cut-down unit and keys
    calls by name only, so this is where a unit-vs-source difference shows up."""

    def __init__(self, repo: Path, target: Target, work: Path, by_name, aliases, log=print):
        self.repo, self.target, self.by_name, self.log = repo, target, by_name, log
        self.dir = work / "full"
        if self.dir.exists():
            shutil.rmtree(self.dir, ignore_errors=True)
        shutil.copytree(repo / "src", self.dir / "src")
        self.src = self.dir / target.rel
        self.ctx = compare.MatchContext(repo, CLIENT, self.dir / "obj", aliases, log=lambda *x: None)
        self.xtu = None
        self.refs = None
        self.base = self.results(target.body)

    def results(self, body: str) -> dict | None:
        """{va: {"status", "ratio", "ndiff", "score", "name"}} of every annotated function of the source with
        `body` as the target's body; None when it doesn't build."""
        import xtu as X
        self.src.write_text(self.target.full_text(body), encoding="utf-8")
        obj, out, flags = compare.compile_match_obj(self.ctx.ws, self.repo, self.src, self.ctx.out_dir,
                                                    self.ctx.header, CLIENT, self.repo / self.target.rel)
        if obj is None:
            return None
        anns = ann.scan([self.src], CLIENT.upper())
        refs = X.external_refs(obj)
        if self.xtu is None or not refs <= self.refs:      # the defining TUs are built once per target
            self.xtu = self.ctx.cross_tu({self.src: obj}, {self.src: anns}, log=lambda *x: None)
            self.refs = refs if self.refs is None else self.refs | refs
        res = self.ctx.compare_obj(obj, self.src, anns, self.by_name, None, self.target.rel, flags, xtu=self.xtu)
        return {r.va: {"status": compare.match_status(r), "ratio": r.ratio, "ndiff": r.ndiff,
                       "score": score_of(r), "name": r.name} for r in res}

    def check(self, body: str) -> tuple:
        """-> (accepted, why, the target's whole-source result): the target must score better than unmodified
        and no function of the source may get a worse status or a lower ratio (not_worse)."""
        if self.base is None:
            return False, "the unmodified source doesn't build", None
        new = self.results(body)
        if new is None:
            return False, "the source doesn't build", None
        return judge(self.base, new, self.target.va)


def judge(base: dict, new: dict, va: int) -> tuple:
    """FullCheck's verdict on two {va: {"status", "ratio", "ndiff", "score", "name"}} maps of one source."""
    t0, t1 = base.get(va), new.get(va)
    if t0 is None or t1 is None or t1["score"] is None:
        return False, "target missing from the source's results", t1
    bad = []
    if t0["score"] is not None and t1["score"] >= t0["score"]:
        bad.append(f"target not better in the whole source: {t0['status']} {t0['ratio']} ndiff "
                   f"{t0['ndiff']} -> {t1['status']} {t1['ratio']} ndiff {t1['ndiff']}")
    for v, b in sorted(base.items()):
        n = new.get(v)
        if n is None:
            bad.append(f"0x{v:08X} {b['name']} missing")
        elif v != va and b["score"] is not None and not not_worse(b, n):
            bad.append(f"0x{v:08X} {b['name']}: {b['status']} {b['ratio']} -> {n['status']} {n['ratio']}")
    return not bad, "; ".join(bad) or "ok", t1


def unified(target: Target, body: str) -> str:
    a = target.text.splitlines(keepends=True)
    b = target.full_text(body).splitlines(keepends=True)
    return "".join(difflib.unified_diff(a, b, f"a/{target.rel}", f"b/{target.rel}", n=3))


def make_snapshot(repo: Path, dest: Path, rev: str | None = "HEAD") -> Path:
    """Copy what the permuter reads (SNAPSHOT_DIRS) out of the repo, so edits made there meanwhile don't
    matter: from the commit `rev` (git archive; default HEAD, i.e. without uncommitted work), or the working
    tree when rev is None or the repo isn't a git checkout."""
    import io
    import tarfile
    if dest.exists():
        if any(dest.iterdir()) and not (dest / "SNAPSHOT.txt").is_file():
            raise SystemExit(f"{dest} is not a permute snapshot (no SNAPSHOT.txt): refusing to delete it")
        shutil.rmtree(dest)
    dest.mkdir(parents=True)
    note = f"from {repo}\n"
    data = None
    if rev:
        p = subprocess.run(["git", "-C", str(repo), "archive", "--format=tar", rev, "--",
                            *[d for d in SNAPSHOT_DIRS if (repo / d).is_dir()]], capture_output=True)
        if p.returncode == 0:
            data = p.stdout
            sha = subprocess.run(["git", "-C", str(repo), "rev-parse", rev], capture_output=True, text=True).stdout
            note += f"git {rev} = {sha}"
    if data is not None:
        with tarfile.open(fileobj=io.BytesIO(data)) as tf:
            tf.extractall(dest, filter="data")
    else:
        for d in SNAPSHOT_DIRS:
            if (repo / d).is_dir():
                shutil.copytree(repo / d, dest / d)
        note += "working tree\n"
    (dest / "SNAPSHOT.txt").write_text(note, encoding="utf-8")
    return dest


def _shape(r: dict):
    """A scored listing without the rewrite's own data addresses (they move when other functions go)."""
    return [re.sub(r"R:[0-9a-f]+:", "R:", x) for x in r.get("listing") or []]


def permute_one(repo: Path, va: int, a, ws: Workspace, by_name, aliases, log=print) -> dict:
    target = load_target(repo, va, a.source)
    out_dir = Path(a.out) if a.out else default_dir(ws, "match", "permute")
    out_dir.mkdir(parents=True, exist_ok=True)
    root = Path(a.work) if a.work else default_dir(ws, "permute", "work")
    work = root / f"{va:08X}"
    work.mkdir(parents=True, exist_ok=True)
    filename = Path(target.rel).name
    log(f"0x{va:08X} {target.name}  ({target.rel})")
    with cf.ProcessPoolExecutor(a.jobs, initializer=worker_init,
                                initargs=(str(repo), str(root), str(work), by_name, aliases, va, filename,
                                          CLIENT, str(repo / target.rel))) as pool:
        # the cut-down unit must give the target the same code as the whole source
        full = tu.whole(target.text, target.fdef)
        r_unit = pool.submit(worker_score, target.unit.text()).result()
        r_full = pool.submit(worker_score, full.text()).result()
        if _shape(r_unit) != _shape(r_full) or r_unit["score"] != r_full["score"]:
            log("    cut-down unit differs from the full source: using the full source")
            target.unit = full
        types = {}
        if "add_temp" in MUTATIONS:
            try:
                types = probe_types(target, lambda t: pool.submit(worker_compile_output, t).result())
            except Exception as e:                  # noqa: BLE001
                log(f"    type probe failed: {e}")
        history: list = []
        base, best, stats = search(target, pool, a.budget, a.iters, a.jobs, a.seed, types, not a.safe, log,
                                   history=history)
        log(f"    start {base.score} ({status_of(base)} {base.info.get('ratio')}), best {best.score} "
            f"({status_of(best)} {best.info.get('ratio')}) after {stats['compiled']} compiles "
            f"({stats['failed']} failed) in {stats['seconds']}s; {len(types)} typed subexpressions")
        full_note, full_after = "", None
        if best.score < base.score:
            raw = best
            best = minimise(target, pool, base, best, log)
            best.info = pool.submit(worker_score, target.unit.text(best.body), True).result()
            best.score = best.info["score"]
            if not a.no_full_check:
                # the winner (failing that the raw best, then the earlier bests, latest first) must pass the
                # whole-source acceptance check
                fc = FullCheck(repo, target, work, by_name, aliases, log)
                tries, seen_b = [], set()
                for c in [best, raw] + history[::-1]:
                    if c.score is not None and c.score < base.score and _h(c.body) not in seen_b:
                        seen_b.add(_h(c.body))
                        tries.append(c)
                chosen, notes = None, []
                for c in tries[:a.full_tries]:
                    ok, why, t1 = fc.check(c.body)
                    notes.append(f"score {c.score}: {why}")
                    log(f"    whole source, score {c.score} ({status_of(c)} {c.info.get('ratio')}): "
                        f"{'accepted' if ok else 'REJECTED ' + why}")
                    if ok:
                        chosen, full_after = c, t1
                        break
                full_note = " | ".join(notes)
                if chosen is None:
                    best = base
                else:
                    if chosen is not best:
                        chosen.info = pool.submit(worker_score, target.unit.text(chosen.body), True).result()
                    best = chosen
    result = {"va": f"0x{va:08X}", "name": target.name, "source": target.rel, "before": base.score,
              "after": best.score, "status": status_of(best), "ratio_before": base.info.get("ratio"),
              "ratio_after": best.info.get("ratio"), "status_before": status_of(base), "mutations": best.muts,
              "compiles": stats["compiled"], "failed": stats["failed"], "seconds": stats["seconds"],
              "full_check": full_note, "full_after": full_after,
              "verified": None, "verify": "", "diff_file": "", "first_diff": best.info.get("first_diff", ""),
              "by_mut": stats["by_mut"]}
    if best.score < base.score:
        if not a.no_verify:
            ub = Path(a.unicorn_build or os.environ.get("EQD_UNICORN_BUILD") or ws.build / "permute_ub")
            base_ok, base_detail = run_fixtures(repo, target, target.body, work / "verify_base", ub / f"{va:08X}_base",
                                                TOOLS, log)
            ok, detail = run_fixtures(repo, target, best.body, work / "verify", ub / f"{va:08X}", TOOLS, log)
            if base_ok is False:
                detail = f"unmodified source fails its fixtures too ({base_detail.splitlines()[0]}); {detail}"
                ok = False
            result["verified"], result["verify"] = ok, detail
            log(f"    fixtures: {'PASS' if ok else 'n/a' if ok is None else 'FAIL'} {detail.splitlines()[0]}")
        patch = unified(target, best.body)
        df = out_dir / f"{va:08X}.diff"
        hdr = (f"# permute.py 0x{va:08X} {target.name}: score {base.score} -> {best.score} ({status_of(base)} "
               f"{base.info.get('ratio')} -> {status_of(best)} {best.info.get('ratio')}; whole source: "
               f"{'not checked' if a.no_full_check else 'ok'}); fixtures: {result['verified']}; "
               f"mutations: {', '.join(best.muts)}\n")
        df.write_text(hdr + patch, encoding="utf-8")
        result["diff_file"] = str(df)
        if best.info.get("diff"):
            (work / "best_asm.diff").write_text("\n".join(best.info["diff"]) + "\n", encoding="utf-8")
    (work / "result.json").write_text(json.dumps(result, indent=1), encoding="utf-8")
    line = "\t".join(str(x) for x in (result["va"], target.name, target.rel, base.score, best.score,
                                      result["status"], result["verified"], len(best.muts), stats["compiled"],
                                      stats["seconds"], ",".join(best.muts), result["ratio_before"],
                                      result["ratio_after"], full_note))
    with open(out_dir / "summary.tsv", "a", encoding="utf-8") as fh:
        if fh.tell() == 0:
            fh.write("# va\tname\tsource\tbefore\tafter\tstatus\tverified\tnmut\tcompiles\tseconds\tmutations"
                     "\tratio_before\tratio_after\tfull_check\n")
        fh.write(line + "\n")
    log("    " + line)
    return result


def fixture_index(repo: Path) -> dict:
    """VA -> fixture paths, for every fixture of the repo."""
    out: dict = {}
    root = repo / "tests" / "unicorn"
    for p in sorted(root.rglob("*.json")):
        if "negative" in p.parts or p.parent == root:
            continue
        try:
            o = json.loads(p.read_text(encoding="utf-8")).get("original")
        except (ValueError, OSError, AttributeError):
            continue
        if isinstance(o, str) and o.lower().startswith("0x"):
            out.setdefault(int(o, 16), []).append(p)
    return out


def soundness(repo: Path, ws: Workspace, count: int, steps: int, seed: int, jobs: int, ub: Path, work: Path,
              allow_unsafe=False, log=print) -> list:
    """Self-check of the mutations: `count` random functions that have fixtures get `steps` random mutations
    (safe ones unless allow_unsafe; no type probe, so no add_temp/cse) and their fixtures run. A failure
    whose unmodified source passes is a mutation that changed behavior. -> [(va, mutations, detail)]."""
    rng = random.Random(seed)
    idx = fixture_index(repo)
    anns = [a for a in ann.scan(all_sources(repo), CLIENT.upper()) if a.va in idx]
    rng.shuffle(anns)
    jobs_in = []
    for a in anns:
        if len(jobs_in) >= count:
            break
        try:
            t = load_target(repo, a.va, Path(a.path).resolve().relative_to(repo).as_posix())
        except SystemExit:
            continue
        body, muts = t.body, []
        for _ in range(steps):
            r = mutate(body, rng, t.fdef.params, t.cpp, {}, allow_unsafe)
            if r:
                body, m = r
                muts.append(m)
        if muts:
            jobs_in.append((t, body, muts))
    bad = []

    def one(item):
        t, body, muts = item
        ok, detail = run_fixtures(repo, t, body, work / f"sound_{t.va:08X}", ub / f"sound_{t.va:08X}", TOOLS)
        if ok:
            return None
        if "error" in detail.lower() and "failed" not in detail.lower():
            return (t.va, muts, "build/run error: " + detail.splitlines()[0])
        base_ok, _ = run_fixtures(repo, t, t.body, work / f"sound_{t.va:08X}_b", ub / f"sound_{t.va:08X}_b",
                                  TOOLS)
        if base_ok:
            (work / f"sound_{t.va:08X}.diff").write_text(unified(t, body), encoding="utf-8")
            return (t.va, muts, detail.splitlines()[0])
        return None
    with cf.ThreadPoolExecutor(jobs) as pool:
        for item, r in zip(jobs_in, pool.map(one, jobs_in)):
            log(f"  0x{item[0].va:08X} {item[0].name}: {', '.join(item[2])}: {'FAIL ' + r[2] if r else 'ok'}")
            if r:
                bad.append(r)
    log(f"soundness: {len(jobs_in)} mutated functions, {len(bad)} behave differently")
    return bad


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", default=".", help="decomp repo (default: the current directory)")
    ap.add_argument("-c", "--client", default="trilogy", help="image (default trilogy; eqmain = eqmain.dll). "
                    "Default --out/--work/--snapshot folders of another image go under <build>/.../<client>/")
    ap.add_argument("--va", action="append", help="function(s) to permute (original VA)")
    ap.add_argument("--source", help="the source holding the annotation (default: search src/)")
    ap.add_argument("--nearmiss", help="a compare.py --nearmiss file: permute every VA in it")
    ap.add_argument("--match-json", help="a compare.py --match --json file: permute its entries with --status")
    ap.add_argument("--status", default="diff", help="with --match-json (default: diff)")
    ap.add_argument("--budget", type=float, default=600, help="seconds per function (default 600)")
    ap.add_argument("--iters", type=int, default=10 ** 9, help="max candidates per function")
    ap.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) - 1), help="parallel compiles")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--safe", action="store_true", help="only semantics-preserving mutations (no type/cast/"
                                                        "constant changes)")
    ap.add_argument("--out", help="diffs + summary.tsv (default <build>/match/permute)")
    ap.add_argument("--work", help="scratch (default <build>/permute/work)")
    ap.add_argument("--unicorn-build", help="test DLL folder for verification (default $EQD_UNICORN_BUILD or "
                                            "<build>/permute_ub)")
    ap.add_argument("--no-verify", action="store_true", help="skip the Unicorn fixtures")
    ap.add_argument("--no-full-check", action="store_true", help="skip the whole-source acceptance check of the "
                    "winner (every function of the source: status not worse, compare ratio not lower)")
    ap.add_argument("--full-tries", type=int, default=6, help="candidates (the winner, then earlier bests) the "
                    "whole-source check may try before giving up (default 6)")
    ap.add_argument("--soundness", type=int, metavar="N", help="self-check instead: mutate N random functions "
                    "that have fixtures (safe mutations only) and run their fixtures")
    ap.add_argument("--sound-steps", type=int, default=3, help="mutations per function for --soundness")
    ap.add_argument("--sound-unsafe", action="store_true", help="--soundness with the unsafe mutations too")
    ap.add_argument("--snapshot", nargs="?", const="", help="copy the repo's src/include/names/hooks/tests/unicorn "
                    "(as committed, see --snapshot-rev) to this folder (default <build>/permute/snap) and work there")
    ap.add_argument("--snapshot-rev", default="HEAD", help="commit to snapshot (default HEAD); 'worktree' copies "
                                                           "the working tree with uncommitted edits")
    a = ap.parse_args(argv)
    try:
        sys.stdout.reconfigure(line_buffering=True)     # progress shows up in redirected logs
    except AttributeError:
        pass
    set_client(a.client)
    repo = Path(a.repo).resolve()
    ws = Workspace(repo)
    if a.snapshot is not None:
        dest = Path(a.snapshot) if a.snapshot else default_dir(ws, "permute", "snap")
        repo = make_snapshot(repo, dest.resolve(), None if a.snapshot_rev == "worktree" else a.snapshot_rev)
        print(f"snapshot: {repo}")
    if a.soundness:
        ub = Path(a.unicorn_build or os.environ.get("EQD_UNICORN_BUILD") or ws.build / "permute_ub")
        work = (Path(a.work) if a.work else default_dir(ws, "permute", "work")) / "soundness"
        bad = soundness(repo, ws, a.soundness, a.sound_steps, a.seed, a.jobs, ub, work, a.sound_unsafe)
        return 1 if bad else 0
    vas = [int(v, 16) for v in a.va or []]
    if a.nearmiss:
        for ln in Path(a.nearmiss).read_text(encoding="utf-8").splitlines():
            if ln.strip() and not ln.startswith("#"):
                vas.append(int(ln.split("\t")[0], 16))
    if a.match_json:
        for r in json.loads(Path(a.match_json).read_text(encoding="utf-8"))["results"]:
            if r.get("match") == a.status:
                vas.append(int(r["va"], 16))
    if not vas:
        ap.error("nothing to do: give --va, --nearmiss or --match-json")
    aliases, _ = compare.load_defaults(repo / clients.get(CLIENT).crt_defaults, repo)
    by_name = by_name_of(repo)
    compare.MatchContext(repo, CLIENT, (Path(a.work) if a.work else default_dir(ws, "permute", "work")), aliases,
                         log=print)          # writes the shared eqd_match_syms.h once
    results = []
    for va in dict.fromkeys(vas):
        try:
            results.append(permute_one(repo, va, a, ws, by_name, aliases))
        except (RuntimeError, SystemExit) as e:
            print(f"0x{va:08X}: {e}")
    n0 = sum(r["after"] == 0 for r in results)
    nv = sum(r["after"] == 0 and r["verified"] for r in results)
    imp = sum(r["after"] < r["before"] for r in results)
    print(f"\n{len(results)} functions: {imp} improved, {n0} byte-identical ({nv} fixture-verified)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
