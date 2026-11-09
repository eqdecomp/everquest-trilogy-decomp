r"""Permuter bake-off: permute.py against cpp_permuter on the same near misses, the same scorer, the same budget.

    ..\.venv\Scripts\python.exe tools\permute\bakeoff.py --repo SNAP --va 0x00414E7A ... --compiles 200 -j 4
    ..\.venv\Scripts\python.exe tools\permute\bakeoff.py score OBJ SRC        (the --score-cmd client)

Both tools are scored by the same code: `compare.MatchContext.compare_obj` on the object (match mode) and
`permute.score_of` on the result (0 byte-identical, 1 instruction-identical, else by compare ratio, then
differing instruction count). permute.py does that in its own workers; cpp_permuter calls `bakeoff.py score
{obj} {src}` (`--score-cmd`), a thin client that hands the object to a score server this script runs for the
duration (a named pipe, no network; a process pool that loaded the original image once). Both compile the same
translation unit (permute's cut-down unit for the target, `tu.py`, or the whole source when the unit doesn't
give the same code) with the same command line as `compare.py --match` (SP4, the source's CFLAGS,
/DEQD_MATCH, the generated eqd_match_syms.h).

Budget: the same number of compiles per function per tool (`--compiles`: permute.py `--iters`, which also
counts the duplicates it skips without compiling, cpp_permuter `-n`). Results: `<out>\bakeoff.tsv` (one line
per function and tool) and each tool's output folder.
"""
from __future__ import annotations

import os
import sys

PIPE = os.environ.get("EQD_SCORE_PIPE", r"\\.\pipe\eqd_bakeoff_score")
AUTH = b"eqd-bakeoff"


def client(argv) -> int:
    """`bakeoff.py score OBJ SRC [VA]`: print the score of OBJ (last number of the output), via the server."""
    from multiprocessing.connection import Client
    c = Client(PIPE, family="AF_PIPE", authkey=AUTH)
    c.send(("obj", os.path.abspath(argv[0]), os.path.abspath(argv[1]), argv[2] if len(argv) > 2 else None))
    r = c.recv()
    c.close()
    if r.get("score") is None:
        print("error:", r.get("error", "?"))
        return 1
    print(r.get("first_diff", ""))
    print(r["score"])
    return 0


if __name__ == "__main__" and len(sys.argv) > 1 and sys.argv[1] == "score":
    sys.exit(client(sys.argv[2:]))

# --- the bake-off (heavy imports from here on) ------------------------------------------------------------------

import argparse  # noqa: E402
import concurrent.futures as cf  # noqa: E402
import json  # noqa: E402
import re  # noqa: E402
import shutil  # noqa: E402
import subprocess  # noqa: E402
import threading  # noqa: E402
import time  # noqa: E402
from pathlib import Path  # noqa: E402
from types import SimpleNamespace  # noqa: E402

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import permute as P  # noqa: E402
import compare  # noqa: E402  (permute put compare/ and unicorn/ on sys.path)
from eqd_unicorn import Workspace, annotations as ann  # noqa: E402
from eqd_unicorn.vc6 import cflags_for, vc6_env  # noqa: E402
from eqd_unicorn.workspace import find_root  # noqa: E402

_TOOLCHAINS = (find_root(HERE) or HERE.parents[1]) / "toolchains"

_S: dict = {}


def _srv_init(repo, work, by_name, aliases):
    ctx = compare.MatchContext(Path(repo), P.CLIENT, work, aliases, log=lambda *a: None)
    wdir = Path(work) / f"s{os.getpid()}"
    wdir.mkdir(parents=True, exist_ok=True)
    ctx.out_dir = wdir
    _S.update(ctx=ctx, wdir=wdir, by_name=by_name)


def _srv_score(kind, obj, src, va):
    """Score `obj` built from `src` (kind 'obj'), or compile `src` text first (kind 'text', obj = file name)."""
    ctx, wdir = _S["ctx"], _S["wdir"]
    if kind == "text":
        p = wdir / obj
        p.write_text(src, encoding="utf-8")
        src = p
        o, out, flags = ctx.compile(p, wdir)
        if o is None:
            return {"score": None, "error": out[-500:]}
        obj = o
    src = Path(src)
    try:
        anns = ann.scan([src], P.CLIENT.upper())
        todo = [a for a in anns if va is None or a.va == va]
        if len(todo) != 1:
            return {"score": None, "error": f"{len(todo)} annotations for {va} in {src}"}
        res = ctx.compare_obj(Path(obj), src, anns, _S["by_name"], todo, src.name, cflags_for(src))[0]
    except Exception as e:  # noqa: BLE001
        return {"score": None, "error": f"{type(e).__name__}: {e}"}
    return {"score": P.score_of(res), "first_diff": res.first_diff, "ndiff": res.ndiff,
            "listing": [ln.split("  ", 1)[1] for ln in res.rew_listing]}


class ScoreServer:
    """Named-pipe server in front of a process pool of MatchContexts (one per process, loaded once)."""

    def __init__(self, repo, work, by_name, aliases, jobs):
        from multiprocessing.connection import Listener
        self.pool = cf.ProcessPoolExecutor(jobs, initializer=_srv_init,
                                           initargs=(str(repo), str(work), by_name, aliases))
        self.va = None
        self.listener = Listener(PIPE, family="AF_PIPE", authkey=AUTH)
        self.stop = False
        self.t = threading.Thread(target=self._loop, daemon=True)
        self.t.start()

    def _loop(self):
        while not self.stop:
            try:
                conn = self.listener.accept()
            except Exception:  # noqa: BLE001
                if self.stop:
                    return
                continue
            threading.Thread(target=self._one, args=(conn,), daemon=True).start()

    def _one(self, conn):
        try:
            kind, obj, src, va = conn.recv()
            va = int(va, 16) if va else self.va
            conn.send(self.pool.submit(_srv_score, kind, obj, src, va).result())
        except Exception as e:  # noqa: BLE001
            try:
                conn.send({"score": None, "error": str(e)})
            except Exception:  # noqa: BLE001
                pass
        finally:
            conn.close()

    def score_text(self, name, text, va):
        return self.pool.submit(_srv_score, "text", name, text, va).result()

    def close(self):
        self.stop = True
        try:
            from multiprocessing.connection import Client
            Client(PIPE, family="AF_PIPE", authkey=AUTH).close()   # wake accept()
        except Exception:  # noqa: BLE001
            pass
        self.listener.close()
        self.pool.shutdown(cancel_futures=True)


def compile_cmd(ctx, src: Path) -> str:
    """The `compare.py --match` command line for `src` (cpp_permuter template: {src}, {obj})."""
    cl = ctx.ws.vc6() / "Bin" / "CL.EXE"
    flags = cflags_for(src)
    parts = [f'"{cl}"'] + flags + ["/DEQD_MATCH", f'"/FI{ctx.header}"', f'"/I{ctx.repo / "include"}"',
                                   '/Fo{obj}', '{src}']
    return " ".join(parts)


def run_cpp(exe, objdump, src: Path, name: str, ctx, out: Path, n: int, jobs: int, seed: int, extra=()):
    if out.exists():
        shutil.rmtree(out, ignore_errors=True)
    out.mkdir(parents=True)
    me = f'"{sys.executable}" "{Path(__file__).resolve()}" score {{obj}} {{src}}'
    cmd = [str(exe), "-s", str(src), "-f", name, "-c", compile_cmd(ctx, src), "--score-cmd", me,
           "--objdump", str(objdump), "-j", str(jobs), "--seed", str(seed), "-o", str(out / "permuter_out"),
           *extra]
    if "-m" not in extra:
        cmd += ["-n", str(n)]
    t0 = time.time()
    p = subprocess.run(cmd, cwd=out, env=vc6_env(ctx.ws.vc6()), capture_output=True, text=True,
                       errors="replace", timeout=max(600, n * 10))
    secs = round(time.time() - t0, 1)
    (out / "log.txt").write_text(" ".join(cmd) + "\n\n" + p.stdout + "\n" + p.stderr, encoding="utf-8")
    scores = [int(m.group(1)) for d in (out / "permuter_out").glob("output-*")
              if (m := re.match(r"output-(\d+)-\d+$", d.name))]
    comp = re.findall(r"(\d+)\s+compiles?", p.stdout + p.stderr)
    base = re.findall(r"base score[^\d-]*(-?\d+)", p.stdout + p.stderr, re.I)
    return {"best": min(scores) if scores else None, "compiles": int(comp[-1]) if comp else None,
            "base": int(base[0]) if base else None, "seconds": secs, "rc": p.returncode,
            "tail": (p.stdout + p.stderr).strip().splitlines()[-3:]}


def candidates(repo: Path, run: Path, t) -> list:
    """Score-0 candidates of one function from a bake-off run: [(tool, full patched source text, note)]."""
    va = t.va
    out = []
    d = run / "ours" / f"{va:08X}.diff"
    if d.is_file() and "-> 0 (" in d.read_text(encoding="utf-8").splitlines()[0]:
        tmp = run / "verify" / f"{va:08X}_ours_apply"
        shutil.rmtree(tmp, ignore_errors=True)
        (tmp / t.rel).parent.mkdir(parents=True)
        (tmp / t.rel).write_text(t.text, encoding="utf-8")
        p = subprocess.run(["git", "apply", "-p1", str(d)], cwd=tmp, capture_output=True, text=True)
        if p.returncode == 0:
            out.append(("ours", (tmp / t.rel).read_text(encoding="utf-8"),
                        d.read_text(encoding="utf-8").splitlines()[0].split("mutations: ")[-1]))
    for tool in ("cpp", "cppx"):
        outs = sorted((run / tool / f"{va:08X}" / "run" / "permuter_out").glob("output-0-*"))
        if not outs:
            continue
        src = outs[0] / ("source" + Path(t.rel).suffix)
        if not src.is_file():
            src = next(outs[0].glob("source.*"))
        a = [x for x in ann.scan([src], P.CLIENT.upper()) if x.va == va][0]
        unit = src.read_text(encoding="utf-8", errors="replace")
        f = P.tu.find_target(unit, a.line)
        body = unit[f.body_start:f.body_end]
        out.append((tool, t.full_text(body), (outs[0] / "diff.txt").read_text(encoding="utf-8",
                                                                            errors="replace").strip()))
    return out


def verify(repo: Path, run: Path, vas, ub: Path, aliases) -> list:
    """Each score-0 candidate: compare --match on a private copy of the repo with the patched source, and the
    function's Unicorn fixtures (unmodified and patched). Patches + evidence go to <run>/../matches/."""
    mdir = run.parent / "matches"
    mdir.mkdir(exist_ok=True)
    rows = []
    for va in vas:
        t = P.load_target(repo, va)
        for tool, text, note in candidates(repo, run, t):
            body = text[t.fdef.body_start:len(text) - (len(t.text) - t.fdef.body_end)]
            assert t.full_text(body) == text
            vroot = run / "verify" / f"{va:08X}_{tool}"
            shutil.rmtree(vroot, ignore_errors=True)
            for d in ("include", "src", "names", "hooks"):
                shutil.copytree(repo / d, vroot / d)
            (vroot / t.rel).write_text(text, encoding="utf-8")
            res = compare.compare_match(vroot, [vroot / t.rel], P.CLIENT, vroot / "_match", aliases, only_va=va,
                                        log=lambda *x: None)
            r = [x for x in res if x.va == va][0]
            status = compare.match_status(r)
            base_ok, base_d = P.run_fixtures(repo, t, t.body, run / "verify" / f"{va:08X}_fx_base",
                                             ub / f"{va:08X}_base", P.TOOLS)
            ok, det = P.run_fixtures(repo, t, body, run / "verify" / f"{va:08X}_fx_{tool}",
                                     ub / f"{va:08X}_{tool}", P.TOOLS)
            hdr = (f"# bakeoff {tool} 0x{va:08X} {t.name} ({t.rel})\n"
                   f"# compare --match (patched private copy): {status} (bytes_identical={r.bytes_identical}, "
                   f"ndiff={r.ndiff})\n"
                   f"# fixtures: unmodified {base_ok} ({base_d.splitlines()[0]}); patched {ok} "
                   f"({det.splitlines()[0]})\n"
                   f"# snapshot: {(repo / 'SNAPSHOT.txt').read_text(encoding='utf-8').splitlines()[-1]}\n"
                   + "".join(f"# {ln}\n" for ln in note.splitlines()[:1]))
            (mdir / f"{va:08X}_{tool}.diff").write_text(hdr + P.unified(t, body), encoding="utf-8")
            row = (f"0x{va:08X}", t.name, tool, status, ok, base_ok)
            print("\t".join(map(str, row)))
            rows.append(row)
    return rows


def main(argv=None) -> int:
    if argv is None and len(sys.argv) > 1 and sys.argv[1] == "verify":
        ap = argparse.ArgumentParser(description="verify the score-0 candidates of a bake-off run")
        ap.add_argument("verify")
        ap.add_argument("--repo", required=True)
        ap.add_argument("--run", required=True)
        ap.add_argument("--va", action="append", required=True)
        ap.add_argument("--unicorn-build", required=True)
        a = ap.parse_args()
        repo = Path(a.repo).resolve()
        aliases, _ = compare.load_defaults(repo / "tests" / "unicorn" / "crt_defaults.json", repo)
        verify(repo, Path(a.run).resolve(), [int(v, 16) for v in a.va], Path(a.unicorn_build).resolve(), aliases)
        return 0
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", required=True, help="the decomp repo (a snapshot: permute.py --snapshot)")
    ap.add_argument("--va", action="append", required=True)
    ap.add_argument("--compiles", type=int, default=200, help="budget per function and tool")
    ap.add_argument("-j", "--jobs", type=int, default=4)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--out", required=True)
    ap.add_argument("--tools", default="ours,cpp", help="ours, cpp, cppx (exhaustive depth 1, all passes)")
    ap.add_argument("--cpp", default=str(_TOOLCHAINS / "cpp_permuter" / "build" / "cpp_permuter.exe"),
                    help="default <workspace>\\toolchains\\cpp_permuter\\build\\cpp_permuter.exe")
    ap.add_argument("--objdump", default=str(_TOOLCHAINS / "llvm" / "bin" / "llvm-objdump.exe"),
                    help="default <workspace>\\toolchains\\llvm\\bin\\llvm-objdump.exe")
    ap.add_argument("--cpp-args", default="", help="extra cpp_permuter arguments (space separated)")
    ap.add_argument("--without", default="", help="permute.py without these mutations (comma separated), e.g. "
                    "the bake-off passes for the baseline")
    ap.add_argument("--tag", default="ours", help="tool name for permute.py in the table")
    a = ap.parse_args(argv)
    for n in filter(None, a.without.split(",")):
        P.MUTATIONS.pop(n)                       # mutations are drawn in this process
    try:
        sys.stdout.reconfigure(line_buffering=True)
    except AttributeError:
        pass
    repo = Path(a.repo).resolve()
    out = Path(a.out).resolve()
    out.mkdir(parents=True, exist_ok=True)
    tools = a.tools.split(",")
    aliases, _ = compare.load_defaults(repo / "tests" / "unicorn" / "crt_defaults.json", repo)
    by_name = P.by_name_of(repo)
    work = out / "work"
    ctx = compare.MatchContext(repo, P.CLIENT, work, aliases, log=print)
    # permute's workers would race to write their shared eqd_match_syms.h (PermissionError): write it first
    compare.MatchContext(repo, P.CLIENT, out / "ours_work", aliases, log=lambda *x: None)
    srv = ScoreServer(repo, work, by_name, aliases, a.jobs)
    tsv = out / "bakeoff.tsv"
    if not tsv.exists():
        tsv.write_text("# va\tname\ttool\tbase\tbest\tmatched\tcompiles\tseconds\tnote\n", encoding="utf-8")
    try:
        for v in a.va:
            va = int(v, 16)
            srv.va = va
            t = P.load_target(repo, va)
            fname = Path(t.rel).name
            # the unit both tools work on (as permute_one decides it)
            r_unit = srv.score_text(fname, t.unit.text(), va)
            r_full = srv.score_text(fname, P.tu.whole(t.text, t.fdef).text(), va)
            unit = t.unit
            if [x for x in r_unit.get("listing") or []] != [x for x in r_full.get("listing") or []] \
                    or r_unit["score"] != r_full["score"]:
                unit = P.tu.whole(t.text, t.fdef)
            rows = []
            if "ours" in tools:
                args = SimpleNamespace(source=None, out=str(out / "ours"), work=str(out / "ours_work"),
                                       budget=10 ** 6, iters=a.compiles, jobs=a.jobs, seed=a.seed, safe=False,
                                       no_verify=True, unicorn_build=None, no_full_check=True, full_tries=0)
                r = P.permute_one(repo, va, args, ctx.ws, by_name, aliases, log=lambda *x: None)
                rows.append((a.tag, r["before"], r["after"], r["compiles"], r["seconds"],
                             ",".join(r["mutations"])))
            for tool in [x for x in tools if x.startswith("cpp")]:
                d = out / tool / f"{va:08X}"
                d.mkdir(parents=True, exist_ok=True)
                src = d / "src" / fname
                src.parent.mkdir(exist_ok=True)
                src.write_text(unit.text(), encoding="utf-8")
                extra = a.cpp_args.split() if a.cpp_args else []
                if tool == "cppx":
                    extra += ["-m", "exhaustive", "--depth", "1", "--max-candidates", str(a.compiles)]
                r = run_cpp(a.cpp, a.objdump, src, t.name, ctx, d / "run", a.compiles, a.jobs, a.seed, extra)
                base = r["base"] if r["base"] is not None else r_unit["score"]
                best = r["best"] if r["best"] is not None else base
                rows.append((tool, base, min(best, base), r["compiles"], r["seconds"],
                             f"rc={r['rc']} " + " | ".join(r["tail"])[:200]))
            with open(tsv, "a", encoding="utf-8") as fh:
                for tool, b, best, n, s, note in rows:
                    line = f"0x{va:08X}\t{t.name}\t{tool}\t{b}\t{best}\t{best == 0}\t{n}\t{s}\t{note}"
                    fh.write(line + "\n")
                    print(line)
    finally:
        srv.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
