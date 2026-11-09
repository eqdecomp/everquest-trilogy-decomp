r"""Delink functions of a client into relocatable COFF objects (ghidra-delinker-extension), headless.

    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\delink.py trilogy                 every queued function
    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\delink.py trilogy --va 0x0040628B 0x00407FF1
    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\delink.py trilogy --match-status identical
    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\delink.py trilogy --units          + objdiff unit objects

Output (default <build>\<client>\delink\):
    <VA>.obj               one per function (VA as 8 upper-case hex digits, the queue's address form)
    units\<unit>.obj       with --units: one per objdiff unit (compare\objdiff_config.py: a source file's
                           annotated functions plus the rest of its queue subsystem)
    delink.json            manifest: per function its ranges, relocation count and problems

Footprint of a function: its body from the entry point (the contiguous range holding the entry; a body
split around another function is cut there and flagged), plus defined data right after it up to the next
function (VC6 puts switch tables there, inside the function's own section, as the rewrites' objects have
them). Several functions in one object: the union of their footprints (the exporter packs the ranges; it
keeps PC-relative relocations between them).

Symbols: every function the objects define or reference is named `_eqd_fn_0x<VA>`, every other target
`_eqd_va_0x<VA>` (8 upper-case hex digits), the names match mode gives client symbols (compare\README.md,
`eqaddr.h` with EQD_MATCH), so `compare\coff.py` places them at their own VA. Labels inside a function stay
static. Thunks are not redirected: a call to a thunk stays a call to the thunk's VA.

Read-only on the project: the relocation table synthesizer and the renames run in one transaction that is
rolled back after the export; nothing is saved. Refuses to start while the project is open elsewhere.
Needs the extension in the 12.1.3 user extensions (README.md); PyGhidra loads user extensions itself.
"""
import argparse
import json
import sys
from pathlib import Path

import eqd_ghidra as eqd

TOOLS = Path(__file__).resolve().parents[2]
OUT_OF_QUEUE = {"library", "skip"}
MAX_TRAILING_DATA = 0x1000          # bytes of data after a body still counted as its own (switch tables)


def sym_name(kind: str, va: int) -> str:
    return f"_eqd_{kind}_0x{va:08X}"


def load_queue(path: Path) -> list:
    return [json.loads(ln) for ln in path.read_text(encoding="utf-8").splitlines() if ln.strip()]


def select(entries, vas=None, states=None, subsystem=None, match_status=None) -> list:
    """Queue slice -> sorted VAs. Default: every queued function (not library/skip, not removed)."""
    if vas:
        return sorted(set(vas))
    out = []
    for e in entries:
        if e.get("removed"):
            continue
        if states:
            if e.get("state") not in states:
                continue
        elif e.get("state") in OUT_OF_QUEUE:
            continue
        if subsystem and e.get("subsystem") != subsystem and not str(e.get("subsystem", "")).startswith(subsystem + "/"):
            continue
        if match_status and (e.get("match") or {}).get("status") not in match_status:
            continue
        out.append(int(e["address"], 16))
    return sorted(set(out))


def project_locked(ws, client) -> bool:
    """True if a Ghidra process (or PyGhidra's JVM) has the project open: it holds a lock on `<name>.lock~`.
    Probed by trying to take that lock for an instant (Windows), else by the lock file's presence."""
    import os
    project_dir = ws.ghidra / client
    lock2 = project_dir / f"{client}.lock~"
    if not lock2.exists():
        return (project_dir / f"{client}.lock").exists() and os.name != "nt"
    if os.name != "nt":
        return True
    import msvcrt
    fd = os.open(lock2, os.O_RDONLY)
    try:
        msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
    except OSError:
        return True
    else:
        os.lseek(fd, 0, 0)
        msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)
        return False
    finally:
        os.close(fd)


# --- inside the JVM -----------------------------------------------------------------------------------------

def footprint(program, va):
    """-> (AddressSet, problems) of the function at `va`, or (None, [why])."""
    from ghidra.program.model.address import AddressSet
    af = program.getAddressFactory().getDefaultAddressSpace()
    fm, listing = program.getFunctionManager(), program.getListing()
    entry = af.getAddress(va)
    f = fm.getFunctionAt(entry)
    if f is None:
        return None, ["no function at this address"]
    problems = []
    body = f.getBody()
    rng = body.getRangeContaining(entry)
    nxt = fm.getFunctions(entry.add(1), True)
    nxt = nxt.next().getEntryPoint() if nxt.hasNext() else None
    if body.getNumAddressRanges() > 1:
        problems.append(f"body has {body.getNumAddressRanges()} ranges; only the one at the entry is used")
    start, end = rng.getMinAddress(), rng.getMaxAddress()
    if start != entry:
        problems.append(f"body range starts at {start} before the entry")
        start = entry
    if nxt is not None and end.compareTo(nxt) >= 0:
        problems.append(f"body overlaps the next function at {nxt}")
        end = nxt.subtract(1)
    s = AddressSet(start, end)
    # trailing defined data (switch tables) up to the next function
    a = end.add(1)
    last = end
    while nxt is None or a.compareTo(nxt) < 0:
        cu = listing.getCodeUnitAt(a)
        if cu is None:
            break
        from ghidra.program.model.listing import Data
        if isinstance(cu, Data) and cu.isDefined():
            last = cu.getMaxAddress()
            a = last.add(1)
        elif isinstance(cu, Data) and program.getMemory().getByte(a) in (-52, -112):   # int3 / nop padding
            a = a.add(1)
        else:
            break
        if last.subtract(end) > MAX_TRAILING_DATA:
            break
    if last != end:
        s.add(end.add(1), last)
    return s, problems


def name_targets(program, file_set, log):
    """Rename every relocation target and every non-local symbol inside `file_set` to its eqd name.
    -> number of renamed addresses."""
    from ghidra.program.model.symbol import SourceType
    from ghidra.app.util.predicates.visibility import IsSymbolInsideFunction
    st, fm = program.getSymbolTable(), program.getFunctionManager()
    glob = program.getGlobalNamespace()
    inside = IsSymbolInsideFunction()
    from ghidra.program.model.relocobj import RelocationTable
    targets = set()
    it = RelocationTable.get(program).getRelocations()
    while it.hasNext():
        r = it.next()
        if file_set.contains(r.getAddress()) and r.getTarget() is not None:
            targets.add(r.getTarget())
    for s in st.getAllSymbols(True):
        a = s.getAddress()
        if a.isMemoryAddress() and file_set.contains(a) and not inside.test(s):
            targets.add(a)
    n = 0
    for a in targets:
        f = fm.getFunctionAt(a)
        name = sym_name("fn" if f is not None else "va", a.getOffset())
        try:
            if f is not None:
                if f.getName() != name or f.getParentNamespace() != glob:
                    f.setParentNamespace(glob)
                    f.setName(name, SourceType.USER_DEFINED)
            else:
                s = st.getPrimarySymbol(a)
                if s is None or s.isDynamic() or s.isExternal():
                    lbl = st.createLabel(a, name, glob, SourceType.USER_DEFINED)
                    lbl.setPrimary()
                else:
                    s.setNameAndNamespace(name, glob, SourceType.USER_DEFINED)
            n += 1
        except Exception as e:                       # noqa: BLE001
            log(f"  rename {a}: {e}")
    return n


def exporter_options():
    import jpype
    from ghidra.app.util import Option, SymbolPreference
    from java.util import ArrayList
    machine = jpype.JClass("net.boricj.bft.coff.constants.CoffMachine").IMAGE_FILE_MACHINE_I386
    opts = ArrayList()
    for group, name, value in (
            ("COFF header", "COFF machine", machine),
            ("COFF header", "Generate .comment section", False),
            ("Symbols", "Symbol name preference", SymbolPreference.PRIMARY),
            ("Symbol visibility", "Give dynamic symbols static visibility", True),
            ("Symbol visibility", "Give symbols inside functions static visibility", True),
            ("Symbol visibility", "Regular expression for static symbol names", "^switchD_.+::switchdataD_.+$"),
            ("Relocations", "Trim superfluous PC-relative relocations", True)):
        opts.add(Option(group, name, value))
    return opts


def export_obj(program, path: Path, file_set, monitor):
    from ghidra.app.util.exporter import CoffRelocatableObjectExporter
    from java.io import File
    ex = CoffRelocatableObjectExporter()
    ex.setOptions(exporter_options())
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists():
        path.unlink()
    ok = ex.export(File(str(path)), program, file_set, monitor)
    msgs = str(ex.getMessageLog().toString()).strip()
    return bool(ok), msgs


def _count(program, file_set) -> int:
    from ghidra.program.model.relocobj import RelocationTable
    n = 0
    it = RelocationTable.get(program).getRelocations()
    while it.hasNext():
        n += file_set.contains(it.next().getAddress())
    return n


def run(program, vas, units, out: Path, log=print, functions=True) -> dict:
    from ghidra.app.analyzers import RelocationTableSynthesizerAnalyzer
    from ghidra.app.util.importer import MessageLog
    from ghidra.framework.options import ToolOptions
    from ghidra.program.model.address import AddressSet
    from ghidra.util.task import TaskMonitor
    monitor = TaskMonitor.DUMMY
    manifest = {"functions": {}, "units": {}}
    sets = {}
    for va in sorted(set(vas) | {v for u in units.values() for v in u}):
        s, problems = footprint(program, va)
        key = f"{va:08X}"
        manifest["functions"][key] = {"problems": problems}
        if s is not None:
            sets[va] = s
            manifest["functions"][key]["ranges"] = [f"{r.getMinAddress()}-{r.getMaxAddress()}" for r in s]
            manifest["functions"][key]["size"] = int(s.getNumAddresses())
    union = AddressSet()
    for s in sets.values():
        union.add(s)
    tx = program.startTransaction("delink (rolled back)")
    try:
        an = RelocationTableSynthesizerAnalyzer()
        opts = ToolOptions("delink")
        an.registerOptions(opts, program)
        opts.setBoolean("Redirect thunks to targets", False)
        an.optionsChanged(opts, program)
        mlog = MessageLog()
        log(f"synthesizing relocations over {len(sets)} functions ({union.getNumAddresses()} bytes)", flush=True)
        an.added(program, union, monitor, mlog)
        if mlog.hasMessages():
            log("  synthesizer: " + str(mlog.toString()).strip()[:2000])
        log(f"  renamed {name_targets(program, union, log)} symbols", flush=True)
        done = 0
        for va, s in sets.items():
            if not functions or va not in vas:
                continue
            key = f"{va:08X}"
            ok, msgs = export_obj(program, out / f"{key}.obj", s, monitor)
            m = manifest["functions"][key]
            m.update(obj=f"{key}.obj", ok=ok, relocs=_count(program, s))
            if msgs:
                m["log"] = msgs[:500]
            done += ok
            if done % 200 == 0:
                log(f"  {done} objects", flush=True)
        for name, uvas in units.items():
            us = AddressSet()
            for v in uvas:
                if v in sets:
                    us.add(sets[v])
            if us.isEmpty():
                continue
            ok, msgs = export_obj(program, out / "units" / f"{name}.obj", us, monitor)
            manifest["units"][name] = {"obj": f"units/{name}.obj", "ok": ok,
                                       "functions": [f"{v:08X}" for v in uvas if v in sets],
                                       "missing": [f"{v:08X}" for v in uvas if v not in sets]}
            if msgs:
                manifest["units"][name]["log"] = msgs[:500]
    finally:
        program.endTransaction(tx, False)           # roll back: the project is never changed
    return manifest


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("client")
    ap.add_argument("--va", nargs="+", help="these functions (hex VAs) instead of a queue slice")
    ap.add_argument("--queue", help="queue file (default <workspace>\\eq-<client>\\queue\\queue.jsonl)")
    ap.add_argument("--repo", help="decomp repo for --units (default the client's: EQD_REPO, else its eq-* repo)")
    ap.add_argument("--state", action="append", help="queue slice: only these states (repeatable)")
    ap.add_argument("--subsystem", help="queue slice: this subsystem (and below)")
    ap.add_argument("--match-status", action="append", help="queue slice: only this match status (repeatable)")
    ap.add_argument("--units", action="store_true", help="also write the objdiff unit objects (units\\<unit>.obj)")
    ap.add_argument("--no-functions", action="store_true", help="only the unit objects")
    ap.add_argument("--out", help="output folder (default <build>\\<client>\\delink)")
    a = ap.parse_args()
    ws = eqd.Workspace()
    out = Path(a.out) if a.out else ws.build / a.client / "delink"
    qpath = Path(a.queue) if a.queue else ws.queue(a.client)
    entries = load_queue(qpath)
    vas = select(entries, [int(v, 16) for v in a.va] if a.va else None, a.state, a.subsystem, a.match_status)
    units = {}
    if a.units:
        sys.path.insert(0, str(TOOLS / "compare"))
        import objdiff_config
        repo = Path(a.repo) if a.repo else ws.repo(a.client)
        units = {u.name: u.vas for u in objdiff_config.plan_units(repo, entries, a.client)}
    print(f"delink {a.client}: {len(vas)} functions, {len(units)} units -> {out}", flush=True)
    if project_locked(ws, a.client):
        raise SystemExit(f"the Ghidra project {ws.ghidra / a.client} is open in another process; close it first")
    pyghidra = eqd.start(ws)
    project = eqd.open_project(ws, a.client)
    try:
        with pyghidra.program_context(project, ws.program_path(a.client)) as program:
            manifest = run(program, vas, units, out, functions=not a.no_functions,
                           log=lambda *x, **k: print(*x, **k))
    finally:
        project.close()
    fn = manifest["functions"]
    ok = sum(1 for m in fn.values() if m.get("ok"))
    bad = {k: m for k, m in fn.items() if k in {f"{v:08X}" for v in vas} and not m.get("ok") and not a.no_functions}
    manifest["summary"] = {"requested": len(vas), "objects": ok, "failed": sorted(bad),
                           "with_problems": sorted(k for k, m in fn.items() if m.get("problems")),
                           "units": sum(1 for u in manifest["units"].values() if u.get("ok"))}
    out.mkdir(parents=True, exist_ok=True)
    mpath = out / "delink.json"
    old = json.loads(mpath.read_text(encoding="utf-8")) if mpath.is_file() else {}
    for k in ("functions", "units"):                 # merge with earlier runs (other slices)
        manifest[k] = {**old.get(k, {}), **manifest[k]}
    mpath.write_text(json.dumps(manifest, indent=1) + "\n", encoding="utf-8")
    print(f"{ok} function objects, {manifest['summary']['units']} unit objects, {len(bad)} failed, "
          f"{len(manifest['summary']['with_problems'])} with footprint problems; wrote {mpath}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
