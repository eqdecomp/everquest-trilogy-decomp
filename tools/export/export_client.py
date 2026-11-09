r"""Export a client's Ghidra project to JSON for the queue builder and context packer.

    export_client.py <client> [--phase all|meta|decompile] [--timeout S] [--limit N]
                              [--retry-failed] [--restart] [--out DIR]

Opens the existing project read-only (no import, no analysis, nothing saved) and writes
build\export\<client>\ (never committed; the script refuses to write inside a git repo):

    manifest.json             schema version, binary hash, fingerprint, counts, decompile progress
    functions.json            one record per function: names, name-list evidence, tags, signature,
                              source-file / subsystem hint, referenced strings and globals
    callgraph.json            callees (call-site order) and callers per function, address-taken refs
    strings.json              strings with their xrefs
    globals.json              non-code addresses with a symbol or a reference: name, type, size, xrefs
    asm\NNNN.jsonl            disassembly with bytes, SHARD_SIZE functions per shard
    pseudocode\NNNN.jsonl     decompiler output, same shards; resumable

The meta phase (everything but pseudocode) takes about a minute and is always rewritten in full. The
decompile phase appends one line per function and flushes it, so an interrupted run (Ctrl-C, crash)
resumes where it stopped. A finished run compacts each shard to one record per function in address
order. If the program changed since the pseudocode was started (names, signatures, functions), the
fingerprint differs and the pseudocode starts over. Schema: export\README.md.
"""
import argparse
import hashlib
import json
import os
import shutil
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "ghidra" / "scripts"))
import eqd_ghidra as eqd                       # noqa: E402
from apply_names import PRIORITY, read_lists   # noqa: E402
from coverage_report import library_span       # noqa: E402

SCHEMA_VERSION = 1
SHARD_SIZE = 100
DEFAULT_TIMEOUT = 600          # seconds per function; the largest Trilogy function is 85 KB
MAX_PAYLOAD_MB = 512           # decompiler output limit (Ghidra default 50)
FSYNC_EVERY = 25
MIN_STRING = 4
MAX_STRING = 4096


def purge_or_none(n):
    """Ghidra's stack purge, or None when unknown (UNKNOWN/INVALID_STACK_DEPTH_CHANGE = Integer.MAX_VALUE(-1))."""
    n = int(n)
    return None if n < 0 or n >= 0x7FFFFFFE else n


def hx(addr):
    """Address (Ghidra Address or int) -> 8-digit upper-case hex, as in the names/*.csv files."""
    off = addr if isinstance(addr, int) else addr.getOffset()
    return f"{int(off) & 0xFFFFFFFF:08X}"


def shard_name(index):
    return f"{index // SHARD_SIZE:04d}.jsonl"


def write_json(path, data):
    tmp = path.with_suffix(path.suffix + ".tmp")
    tmp.write_text(json.dumps(data, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")
    os.replace(tmp, path)
    print(f"wrote {path}")


def inside_git_repo(path):
    p = path.resolve()
    return any((d / ".git").exists() for d in (p, *p.parents))


# --------------------------------------------------------------------------------------------- names

def source_file(rows):
    """Original source file from the winning name row, else from any other row."""
    return next((r["file"] for r in rows if r.get("file")), None)


def subsystem_hint(rows, cls, tags):
    """Coarse grouping for the queue: library, or the original source file without prefix/extension."""
    if cls in ("library", "import_thunk") or "library_span" in tags:
        return "library"
    if rows and rows[0]["note"].startswith("library:"):
        return "library"
    f = source_file(rows)
    if not f:
        return None
    for prefix in ("Source/PC Source/", "Source/", "GameLib/"):
        if f.startswith(prefix):
            f = f[len(prefix):]
            break
    return f.rsplit(".", 1)[0]


def name_lists(ws, client):
    stem = eqd.names_stem(client)
    paths = []
    for d in (ws.repo(client) / "names", ws.build / "names" / client):
        paths += sorted(d.glob(f"{stem}.*.csv"))
    return read_lists(paths), [p.name for p in paths]


# --------------------------------------------------------------------------------------- string read

class StringReader:
    """String at an address: Ghidra's defined string, else printable NUL-terminated ASCII in an
    initialized data block that has no other data defined there (as in export_features.py)."""

    def __init__(self, program):
        from jpype import JArray, JByte
        self.listing = program.getListing()
        self.mem = program.getMemory()
        self.buf = JArray(JByte)(MAX_STRING + 1)

    def at(self, addr):
        """-> (value, defined) or None"""
        data = self.listing.getDataAt(addr)
        if data is not None and data.hasStringValue():
            return str(data.getValue()), True
        block = self.mem.getBlock(addr)
        if block is None or not block.isInitialized() or block.isExecute():
            return None
        if data is not None and data.isDefined():
            return None
        try:
            n = self.mem.getBytes(addr, self.buf)
        except Exception:
            return None
        raw = bytes(b & 0xFF for b in self.buf[:n])
        end = raw.find(bytes(1))
        if end < MIN_STRING or not all(0x20 <= c < 0x7F or c in (9, 10, 13) for c in raw[:end]):
            return None
        return raw[:end].decode("ascii"), False


# -------------------------------------------------------------------------------------------- program

def fingerprint(program):
    """Changes whenever a function appears, moves, resizes, or changes name or signature."""
    h = hashlib.sha1()
    for f in program.getFunctionManager().getFunctions(True):
        h.update(f"{hx(f.getEntryPoint())} {f.getBody().getNumAddresses()} {f.getName(True)} "
                 f"{f.getPrototypeString(True, False)}\n".encode("utf-8"))
    return h.hexdigest()


def func_entry(f):
    return None if f is None else hx(f.getEntryPoint())


def xref_list(program, addr):
    fm = program.getFunctionManager()
    out = []
    for ref in program.getReferenceManager().getReferencesTo(addr):
        frm = ref.getFromAddress()
        if not frm.isMemoryAddress():
            continue
        out.append({"from": hx(frm), "function": func_entry(fm.getFunctionContaining(frm)),
                    "type": str(ref.getReferenceType().getName())})
    out.sort(key=lambda x: x["from"])
    return out


def params(f):
    out = []
    for p in f.getParameters():
        out.append({"name": p.getName(), "type": p.getDataType().getDisplayName(),
                    "storage": str(p.getVariableStorage())})
    return out


def meta(program, client, ws, out):
    """functions.json, callgraph.json, strings.json, globals.json, asm shards."""
    from ghidra.program.model.data import Array, Pointer, TypeDef
    from ghidra.program.model.listing import CodeUnitFormat
    from ghidra.program.model.symbol import SourceType

    t0 = time.time()
    listing = program.getListing()
    fm = program.getFunctionManager()
    mem = program.getMemory()
    st = program.getSymbolTable()
    fid_addrs = eqd.fid_addresses(program)
    reader = StringReader(program)
    cuf = CodeUnitFormat.DEFAULT
    by_addr, list_files = name_lists(ws, client)

    span = library_span(program)
    span_range = (int(span["start"], 16), int(span["end"], 16)) if span else None

    funcs = list(fm.getFunctions(True))
    index = {hx(f.getEntryPoint()): i for i, f in enumerate(funcs)}

    asm_dir = out / "asm"
    if asm_dir.exists():
        shutil.rmtree(asm_dir)
    asm_dir.mkdir(parents=True)

    records, callgraph = [], {}
    string_targets, global_targets = set(), set()
    asm_fh, asm_shard = None, None
    for i, f in enumerate(funcs):
        a = hx(f.getEntryPoint())
        cls = eqd.function_class(f, fid_addrs)
        gtags = sorted(str(t.getName()) for t in f.getTags())
        tags = set(gtags)
        if f.getEntryPoint() in fid_addrs:
            tags.add("fid")
        if f.isThunk():
            tags.add("thunk")
        if cls == "import_thunk":
            tags.add("import_thunk")
        if cls == "eh_funclet":
            tags.add("eh_funclet")
        if cls == "library":
            tags.add("library")
        if (span_range and span_range[0] <= f.getEntryPoint().getOffset() <= span_range[1]
                and cls != "library" and cls != "eh_funclet"):
            tags.add("library_span")
        if f.hasNoReturn():
            tags.add("noreturn")
        if f.hasVarArgs():
            tags.add("varargs")
        rows = by_addr.get(int(f.getEntryPoint().getOffset()), [])

        # one pass over the instructions: asm, callees, data references
        callees, strings, globals_, ins_out = [], [], [], []
        seen_s, seen_g = set(), set()
        for ins in listing.getInstructions(f.getBody(), True):
            site = hx(ins.getAddress())
            ins_out.append([site, bytes(b & 0xFF for b in ins.getBytes()).hex(), str(ins),
                            str(cuf.getRepresentationString(ins))])
            flow = ins.getFlowType()
            is_call = flow.isCall()
            resolved_call = False
            for ref in ins.getReferencesFrom():
                rt = ref.getReferenceType()
                to = ref.getToAddress()
                if rt.isCall() or (is_call and rt.isFlow()):
                    callee = fm.getFunctionAt(to)
                    edge = {"site": site, "to": None, "name": None, "kind": "call"}
                    if callee is None and to.isExternalAddress():
                        sym = st.getPrimarySymbol(to)
                        edge.update(kind="import", name=sym.getName(True) if sym else str(to))
                    elif callee is None:
                        edge.update(to=hx(to), kind="call_no_function")
                    elif callee.isExternal():
                        edge.update(kind="import", name=callee.getName(True))
                    else:
                        edge.update(to=hx(callee.getEntryPoint()), name=callee.getName(True))
                        if callee.isThunk():
                            target = callee.getThunkedFunction(True)
                            if target is not None and target.isExternal():
                                edge.update(kind="import", name=target.getName(True))
                    callees.append(edge)
                    resolved_call = True
                    continue
                if (flow.isJump() and not flow.isConditional() and rt.isFlow() and to.isMemoryAddress()):
                    callee = fm.getFunctionAt(to)
                    if callee is not None and callee.getEntryPoint() != f.getEntryPoint():
                        callees.append({"site": site, "to": hx(to), "name": callee.getName(True),
                                        "kind": "tailcall"})
                    continue
                if not (rt.isData() or rt.isRead() or rt.isWrite()) or not to.isMemoryAddress():
                    continue
                block = mem.getBlock(to)
                if block is None or block.isExecute():
                    continue
                key = hx(to)
                s = reader.at(to)
                if s is not None:
                    string_targets.add(int(to.getOffset()))
                    if key not in seen_s:
                        seen_s.add(key)
                        strings.append(key)
                else:
                    global_targets.add(int(to.getOffset()))
                    if key not in seen_g:
                        seen_g.add(key)
                        globals_.append(key)
            if is_call and not resolved_call:
                callees.append({"site": site, "to": None, "name": None, "kind": "indirect",
                                "operand": ins.getDefaultOperandRepresentation(0)})
        for n, e in enumerate(callees):
            e["order"] = n

        # address-taken: non-call references to the entry (vtables, callbacks, function tables)
        taken = []
        for ref in program.getReferenceManager().getReferencesTo(f.getEntryPoint()):
            rt = ref.getReferenceType()
            if rt.isCall() or rt.isJump() or rt.isFlow():
                continue
            frm = ref.getFromAddress()
            if frm.isMemoryAddress():
                taken.append({"from": hx(frm), "function": func_entry(fm.getFunctionContaining(frm)),
                              "type": str(rt.getName())})
        callgraph[a] = {"callees": callees, "callers": [], "address_taken": taken}

        shard = shard_name(i)
        if shard != asm_shard:
            if asm_fh:
                asm_fh.close()
            asm_fh = (asm_dir / shard).open("w", encoding="utf-8", newline="\n")
            asm_shard = shard
        ranges = [[hx(r.getMinAddress()), hx(r.getMaxAddress())] for r in f.getBody().getAddressRanges()]
        asm_fh.write(json.dumps({"address": a, "name": f.getName(True), "ranges": ranges,
                                 "instructions": ins_out}, ensure_ascii=False) + "\n")

        thunk_target = None
        if f.isThunk():
            t = f.getThunkedFunction(False)
            thunk_target = None if t is None else (t.getName(True) if t.isExternal() else func_entry(t))
        ns = f.getParentNamespace()
        winner = rows[0] if rows else None
        records.append({
            "address": a,
            "end": hx(f.getBody().getMaxAddress()),
            "size": int(f.getBody().getNumAddresses()),
            "instructions": len(ins_out),
            "name": f.getName(True),
            "short_name": f.getName(),
            "namespace": None if ns.isGlobal() else ns.getName(True),
            "name_source": str(f.getSymbol().getSource()),
            "is_default_name": eqd.is_default_name(f.getName()),
            "class": cls,
            "tags": sorted(tags),
            "ghidra_tags": gtags,
            "calling_convention": str(f.getCallingConventionName()),
            "signature": str(f.getPrototypeString(True, False)),
            "signature_source": str(f.getSignatureSource()),
            "typed": f.getSignatureSource() != SourceType.DEFAULT,
            "return_type": f.getReturnType().getDisplayName(),
            "parameters": params(f),
            "stack_purge": purge_or_none(f.getStackPurgeSize()),
            "thunk_target": thunk_target,
            "name_list": None if winner is None else {
                k: (winner.get(k) or None) for k in
                ("name", "confidence", "source", "note", "from_address", "file")},
            "name_alternatives": [{"name": r["name"], "confidence": r["confidence"], "source": r["source"]}
                                  for r in rows[1:]],
            "source_file": source_file(rows),
            "subsystem": subsystem_hint(rows, cls, tags),
            "strings": strings,
            "globals": globals_,
            "shard": shard,
        })
        if (i + 1) % 500 == 0:
            print(f"  meta {i + 1}/{len(funcs)} ({time.time() - t0:.0f} s)")
    if asm_fh:
        asm_fh.close()

    # callers: invert the callee edges
    for a, cg in callgraph.items():
        for e in cg["callees"]:
            if e["to"] in callgraph:
                callgraph[e["to"]]["callers"].append(
                    {"function": a, "site": e["site"], "order": e["order"], "kind": e["kind"]})
    for cg in callgraph.values():
        cg["callers"].sort(key=lambda c: (c["function"], c["site"]))
    for r in records:
        cg = callgraph[r["address"]]
        r["callee_count"] = len(cg["callees"])
        r["caller_count"] = len(cg["callers"])

    # strings: referenced by code (above), referenced by data, and every defined string
    space = program.getAddressFactory().getDefaultAddressSpace()
    data_blocks = [b for b in mem.getBlocks() if not b.isExecute() and b.isInitialized()]
    from ghidra.program.model.address import AddressSet
    nonexec = AddressSet()
    for b in mem.getBlocks():
        if not b.isExecute():
            nonexec.add(b.getStart(), b.getEnd())
    for b in data_blocks:
        for d in listing.getDefinedData(AddressSet(b.getStart(), b.getEnd()), True):
            if d.hasStringValue():
                string_targets.add(int(d.getAddress().getOffset()))
    for to in program.getReferenceManager().getReferenceDestinationIterator(nonexec, True):
        off = int(to.getOffset())
        if off in string_targets or off in global_targets:
            continue
        if reader.at(to) is not None:
            string_targets.add(off)
        else:
            global_targets.add(off)
    # globals also: every symbol in a non-code block (typed globals, labels without references)
    for sym in st.getPrimarySymbolIterator(nonexec, True):
        off = int(sym.getAddress().getOffset())
        if off not in string_targets:
            global_targets.add(off)

    strings_out = []
    for off in sorted(string_targets):
        addr = space.getAddress(off)
        value, defined = reader.at(addr) or (None, False)
        if value is None:
            continue
        data = listing.getDataAt(addr)
        strings_out.append({
            "address": hx(off), "value": value, "defined": defined,
            "type": data.getDataType().getDisplayName() if defined else None,
            "block": mem.getBlock(addr).getName(), "xrefs": xref_list(program, addr)})

    globals_out = []
    for off in sorted(global_targets):
        addr = space.getAddress(off)
        block = mem.getBlock(addr)
        if block is None:
            continue
        sym = st.getPrimarySymbol(addr)
        data = listing.getDataContaining(addr)
        if data is not None and not data.isDefined():
            data = None
        dt = None if data is None else data.getDataType()
        base = dt
        while isinstance(base, (Array, Pointer, TypeDef)):
            base = base.getDataType()
        src = None if base is None else base.getSourceArchive()
        globals_out.append({
            "address": hx(off),
            "name": None if sym is None else sym.getName(True),
            "name_source": None if sym is None else str(sym.getSource()),
            "block": block.getName(),
            "writable": bool(block.isWrite()),
            "initialized": bool(block.isInitialized()),
            "type": None if dt is None else dt.getDisplayName(),
            "base_type": None if base is None else base.getDisplayName(),
            "type_archive": None if src is None else src.getName(),
            "size": None if data is None else int(data.getLength()),
            "data_start": None if data is None else hx(data.getAddress()),
            "xrefs": xref_list(program, addr),
        })
    print(f"  meta done: {len(records)} functions, {len(strings_out)} strings, {len(globals_out)} globals "
          f"({time.time() - t0:.0f} s)")

    write_json(out / "functions.json", {"schema_version": SCHEMA_VERSION, "client": client,
                                        "functions": records})
    write_json(out / "callgraph.json", {"schema_version": SCHEMA_VERSION, "client": client,
                                        "functions": callgraph})
    write_json(out / "strings.json", {"schema_version": SCHEMA_VERSION, "client": client,
                                      "strings": strings_out})
    write_json(out / "globals.json", {"schema_version": SCHEMA_VERSION, "client": client,
                                      "globals": globals_out})
    classes = {}
    for r in records:
        classes[r["class"]] = classes.get(r["class"], 0) + 1
    return {
        "name_lists": list_files,
        "library_span_estimate": span,
        "counts": {
            "functions": len(records), "classes": classes,
            "call_edges": sum(len(c["callees"]) for c in callgraph.values()),
            "indirect_calls": sum(e["kind"] == "indirect" for c in callgraph.values() for e in c["callees"]),
            "strings": len(strings_out), "globals": len(globals_out),
            "asm_shards": (len(records) + SHARD_SIZE - 1) // SHARD_SIZE,
        },
        "meta_seconds": round(time.time() - t0, 1),
    }


# ------------------------------------------------------------------------------------------ decompile

def read_shard(path):
    """Records of a shard; a torn last line (crash mid-write) is cut off the file."""
    if not path.exists():
        return []
    raw = path.read_bytes()
    if raw and not raw.endswith(b"\n"):
        raw = raw[:raw.rfind(b"\n") + 1]
        path.write_bytes(raw)
    out = []
    for line in raw.decode("utf-8").splitlines():
        try:
            out.append(json.loads(line))
        except ValueError:
            pass
    return out


def new_decompiler(program, timeout):
    from ghidra.app.decompiler import DecompileOptions, DecompInterface
    opts = DecompileOptions()
    opts.grabFromProgram(program)
    opts.setMaxPayloadMBytes(MAX_PAYLOAD_MB)
    d = DecompInterface()
    d.setOptions(opts)
    d.toggleCCode(True)
    d.toggleSyntaxTree(False)
    d.setSimplificationStyle("decompile")
    if not d.openProgram(program):
        raise SystemExit(f"decompiler failed to start: {d.getLastMessage()}")
    return d


def decompile(program, out, fp, args):
    from ghidra.util.task import TaskMonitor
    pdir = out / "pseudocode"
    state_path = pdir / "_state.json"
    state = json.loads(state_path.read_text(encoding="utf-8")) if state_path.exists() else None
    if args.restart or state is None or state.get("fingerprint") != fp \
            or state.get("shard_size") != SHARD_SIZE:
        if pdir.exists():
            print("pseudocode: starting over" + ("" if state is None or args.restart else
                                                 " (program changed since the last run)"))
            shutil.rmtree(pdir)
        pdir.mkdir(parents=True)
        state = {"fingerprint": fp, "shard_size": SHARD_SIZE, "started": time.strftime("%Y-%m-%d %H:%M:%S")}
        write_json(state_path, state)

    funcs = list(program.getFunctionManager().getFunctions(True))
    done = {}
    for shard in sorted(pdir.glob("*.jsonl")):
        for r in read_shard(shard):
            done[r["address"]] = r["status"]
    todo = [(i, f) for i, f in enumerate(funcs)
            if hx(f.getEntryPoint()) not in done
            or (args.retry_failed and done[hx(f.getEntryPoint())] != "ok")]
    if args.limit is not None:
        todo = todo[:args.limit]
    print(f"pseudocode: {len(done)} of {len(funcs)} already done, {len(todo)} to decompile")

    d = new_decompiler(program, args.timeout)
    fh, cur, t0, n = None, None, time.time(), 0
    try:
        for i, f in todo:
            shard = shard_name(i)
            if shard != cur:
                if fh:
                    fh.close()
                fh = (pdir / shard).open("a", encoding="utf-8", newline="\n")
                cur = shard
            a = hx(f.getEntryPoint())
            t = time.time()
            rec = {"address": a, "name": f.getName(True), "status": "ok", "error": None,
                   "seconds": None, "signature": None, "c": None}
            try:
                res = d.decompileFunction(f, args.timeout, TaskMonitor.DUMMY)
                df = res.getDecompiledFunction() if res is not None else None
                if df is not None and res.decompileCompleted():
                    rec["signature"] = str(df.getSignature())
                    rec["c"] = str(df.getC())
                else:
                    msg = "" if res is None else str(res.getErrorMessage() or "")
                    timed_out = res is not None and res.isTimedOut()
                    rec.update(status="timeout" if timed_out else "error", error=msg.strip() or None)
                    if res is None or res.failedToStart() or not d.getMajorVersion():
                        d.dispose()
                        d = new_decompiler(program, args.timeout)
            except Exception as e:     # decompiler process died: record, restart it, go on
                rec.update(status="error", error=f"{type(e).__name__}: {e}")
                try:
                    d.dispose()
                except Exception:
                    pass
                d = new_decompiler(program, args.timeout)
            rec["seconds"] = round(time.time() - t, 3)
            fh.write(json.dumps(rec, ensure_ascii=False) + "\n")
            fh.flush()
            n += 1
            if n % FSYNC_EVERY == 0:
                os.fsync(fh.fileno())
            if rec["status"] != "ok" or rec["seconds"] > 30:
                print(f"  {a} {rec['name']}: {rec['status']} {rec['seconds']} s {rec['error'] or ''}")
            if n % 200 == 0:
                print(f"  decompiled {n}/{len(todo)} ({time.time() - t0:.0f} s)")
    except KeyboardInterrupt:
        print("interrupted; run again to resume")
    finally:
        if fh:
            fh.close()
        d.dispose()

    # status, and compaction once complete
    latest = {}
    for shard in sorted(pdir.glob("*.jsonl")):
        for r in read_shard(shard):
            latest[r["address"]] = r
    entries = [hx(f.getEntryPoint()) for f in funcs]
    complete = all(a in latest for a in entries)
    if complete:
        for s in sorted({shard_name(i) for i in range(len(funcs))}):
            recs = [latest[a] for i, a in enumerate(entries) if shard_name(i) == s]
            tmp = pdir / (s + ".tmp")
            with tmp.open("w", encoding="utf-8", newline="\n") as w:
                for r in recs:
                    w.write(json.dumps(r, ensure_ascii=False) + "\n")
            os.replace(tmp, pdir / s)
    statuses = {}
    for a in entries:
        s = latest[a]["status"] if a in latest else "pending"
        statuses[s] = statuses.get(s, 0) + 1
    failed = [{"address": a, "name": latest[a]["name"], "status": latest[a]["status"],
               "error": latest[a]["error"]} for a in entries if a in latest and latest[a]["status"] != "ok"]
    slowest = sorted((r for r in latest.values()), key=lambda r: -r["seconds"])[:10]
    state.update(complete=complete, statuses=statuses, failed=failed,
                 total_seconds=round(sum(r["seconds"] for r in latest.values()), 1),
                 slowest=[{"address": r["address"], "seconds": r["seconds"]} for r in slowest],
                 updated=time.strftime("%Y-%m-%d %H:%M:%S"))
    write_json(state_path, state)
    print(f"pseudocode: {statuses}{' (complete, compacted)' if complete else ''}")
    return {k: state[k] for k in ("complete", "statuses", "failed", "total_seconds", "slowest")}


# ----------------------------------------------------------------------------------------------- main

def open_readonly(project, path):
    from java.lang import Object
    from ghidra.util.task import TaskMonitor
    df = project.getProjectData().getFile(path)
    if df is None:
        raise SystemExit(f"{path} not in the project")
    consumer = Object()
    return df, df.getReadOnlyDomainObject(consumer, -1, TaskMonitor.DUMMY), consumer


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    ap.add_argument("--phase", choices=("all", "meta", "decompile"), default="all")
    ap.add_argument("--timeout", type=int, default=DEFAULT_TIMEOUT, help="decompiler seconds per function")
    ap.add_argument("--limit", type=int, help="decompile at most N more functions, then stop")
    ap.add_argument("--retry-failed", action="store_true", help="decompile timeouts/errors again")
    ap.add_argument("--restart", action="store_true", help="drop existing pseudocode and start over")
    ap.add_argument("--out", type=Path, help="default: <build>\\export\\<client>")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)     # progress shows up when redirected to a log

    ws = eqd.Workspace()
    out = (args.out or ws.build / "export" / args.client).resolve()
    if inside_git_repo(out):
        raise SystemExit(f"refusing to write the export inside a git repo: {out}")
    out.mkdir(parents=True, exist_ok=True)

    pyghidra = eqd.start(ws)
    from ghidra.framework import Application
    project = eqd.open_project(ws, args.client)
    try:
        dfile, program, consumer = open_readonly(project, ws.program_path(args.client))
        try:
            fp = fingerprint(program)
            mpath = out / "manifest.json"
            manifest = json.loads(mpath.read_text(encoding="utf-8")) if mpath.exists() else {}
            manifest.update({
                "schema_version": SCHEMA_VERSION,
                "client": args.client,
                "program": eqd.CLIENTS[args.client][1],
                "binary_sha256": str(program.getExecutableSHA256()),
                "image_base": hx(program.getImageBase()),
                "ghidra_version": str(Application.getApplicationVersion()),
                "project": str(ws.ghidra / args.client),
                "project_file_version": int(dfile.getVersion()),
                "fingerprint": fp,
                "shard_size": SHARD_SIZE,
            })
            if args.phase in ("all", "meta"):
                manifest.update(meta(program, args.client, ws, out))
                manifest["meta_exported"] = time.strftime("%Y-%m-%d %H:%M:%S")
                manifest["meta_fingerprint"] = fp
                write_json(mpath, manifest)
            if args.phase in ("all", "decompile"):
                if manifest.get("meta_fingerprint") != fp:
                    print("note: functions.json is from another program state; run --phase meta too")
                manifest["decompile"] = decompile(program, out, fp, args)
                manifest["decompile"]["timeout"] = args.timeout
                write_json(mpath, manifest)
        finally:
            program.release(consumer)
    finally:
        project.close()


if __name__ == "__main__":
    main()
