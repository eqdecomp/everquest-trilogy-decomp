r"""Recover code Ghidra's analysis missed (PLAN.md 8.1 A5).

    recover_code.py trilogy [--dry-run] [--only send_social,0042A26F,...] [--all] [--project-dir DIR]
                            [--summary FILE] [--no-decompile]

Two kinds of damage, both found while reviewing the large functions:

1. Switches whose jump the decompiler could not follow. VC6 omits the range check when the default
   case is `__assume(0)`, so `movsx eax,word [ebp+8]; jmp [eax*4+tbl]` has no bound, the switch analysis
   gives up ("Too many branches") and the case blocks stay undisassembled. For every
   `jmp [reg*4+table]` that has no COMPUTED_JUMP reference (named functions: --only; a function created
   below: automatically) the script
   - counts the table entries (the bound of the `cmp reg,imm; ja` just before the jump if there is one,
     with a byte index table `mov r8,[reg+disp]` between them resolved to max+1; otherwise the entries
     that point into [function entry, table));
   - adds a COMPUTED_JUMP reference to each distinct target, disassembles the targets, marks the table as
     a pointer array, writes a decompiler jump-table override (what Ghidra's SwitchOverride.java does)
     and refits the function body (CreateFunctionCmd.fixupFunctionBody).
   Named fixes: send_social (004CC92F, jump at 004CD9E7).
2. Functions that exist only as pointers from .rdata/.data (vtable slots, command tables). Every such
   pointer into .text that no function contains is a candidate; each is tried in a transaction that is
   rolled back. A candidate is "clean" when it decodes to a ret-terminated body of >= 5 instructions that
   starts like a function. Only the --only ones (default: 0042A26F, the CharCreate overlay's 8 KB vtable
   slot) are created; --all creates every clean one; the rest are listed with a one-line reason for review.

--dry-run does everything (including the decompiler check) and rolls the transaction back, so the numbers
it prints are the numbers of the real run. Running it twice changes nothing the second time. Every change
is logged ("CHANGE ..." lines and the report's "changes" list). Report: build\ghidra\<client>.recover.json
(or --summary). One process per Ghidra project: close the GUI first.
"""
import argparse
import json
import re
import sys

import eqd_ghidra as eqd

SWITCH_FIXES = {          # name -> (function entry, computed jump)
    "send_social": (0x4CC92F, 0x4CD9E7),
}
DEFAULT_CREATE = (0x42A26F,)
JMP_TABLE = re.compile(r"^JMP\s+(?:dword ptr\s+)?\[(\w+)\*0x4 \+ 0x([0-9a-fA-F]+)\]$")
BYTE_LOAD = re.compile(r"^(?:MOV|MOVZX)\s+(\w+),(?:byte ptr\s+)?\[(\w+) \+ 0x([0-9a-fA-F]+)\]$")
REG32 = {"AL": "EAX", "AH": "EAX", "AX": "EAX", "BL": "EBX", "BH": "EBX", "BX": "EBX", "CL": "ECX",
         "CH": "ECX", "CX": "ECX", "DL": "EDX", "DH": "EDX", "DX": "EDX"}
START_OK = {"PUSH", "SUB", "MOV", "XOR", "CMP", "TEST", "LEA", "MOVZX", "MOVSX", "CALL", "JMP", "RET", "OR",
            "AND", "INC", "DEC", "FLD", "FILD", "FLD1", "FLDZ", "NOP", "XCHG", "CMPSD", "MOVSD"}
MIN_INSNS = 5


class Recover:
    def __init__(self, program, pyghidra, dry):
        from ghidra.util.task import TaskMonitor
        self.p, self.pg, self.dry = program, pyghidra, dry
        self.mon = TaskMonitor.DUMMY
        self.space = program.getAddressFactory().getDefaultAddressSpace()
        self.L = program.getListing()
        self.fm = program.getFunctionManager()
        self.mem = program.getMemory()
        self.changes, self.notes = [], []
        text = self.mem.getBlock(".text")
        self.text_lo, self.text_hi = text.getStart().getOffset(), text.getEnd().getOffset()

    # -- helpers
    def addr(self, v):
        return self.space.getAddress(v)

    def change(self, kind, va, detail):
        line = f"CHANGE {kind} {va:08X} {detail}"
        print(line)
        self.changes.append({"kind": kind, "address": f"{va:08X}", "detail": detail})

    def note(self, text):
        print("note", text)
        self.notes.append(text)

    def u32(self, va):
        return self.mem.getInt(self.addr(va)) & 0xFFFFFFFF

    def u8(self, va):
        return self.mem.getByte(self.addr(va)) & 0xFF

    def in_text(self, v):
        return self.text_lo <= v <= self.text_hi

    def fn_containing(self, va):
        return self.fm.getFunctionContaining(self.addr(va))

    def insn_stats(self, f):
        n = ret = 0
        last = None
        for i in self.L.getInstructions(f.getBody(), True):
            n += 1
            if i.getMnemonicString() == "RET":
                ret += 1
            last = i
        return n, ret, last

    # -- switch recovery
    def unresolved_jumps(self, body):
        """Computed `jmp [reg*4+tbl]` in body with no COMPUTED_JUMP reference: [(insn, reg, table)]."""
        from ghidra.program.model.symbol import RefType
        out = []
        for i in self.L.getInstructions(body, True):
            ft = i.getFlowType()
            if not (ft.isJump() and ft.isComputed()):
                continue
            m = JMP_TABLE.match(str(i))
            if m is None:
                continue
            if any(r.getReferenceType() == RefType.COMPUTED_JUMP for r in i.getReferencesFrom()):
                continue
            out.append((i, m.group(1).upper(), int(m.group(2), 16)))
        return out

    def table_bound(self, jmp, reg):
        """Entry count from the compare before the jump, or None. Handles `cmp r,imm; ja` with an
        optional byte table between (`mov/movzx r8,[r+disp]`, dest = the jump's index register)."""
        cur, bt = jmp.getPrevious(), None
        for _ in range(14):
            if cur is None:
                return None
            mn = cur.getMnemonicString()
            if mn in ("CALL", "RET", "JMP") or cur.getFlowType().isTerminal():
                return None
            m = BYTE_LOAD.match(str(cur))
            if m and bt is None and REG32.get(m.group(1).upper(), m.group(1).upper()) == reg:
                v = int(m.group(3), 16)
                if self.mem.contains(self.addr(v)):
                    bt = v
            if mn == "CMP" and cur.getNumOperands() == 2:
                scal = cur.getScalar(1)
                nxt = cur.getNext()
                if scal is not None and nxt is not None:
                    j = nxt.getMnemonicString()
                    imm = scal.getUnsignedValue()
                    if j in ("JA", "JBE"):
                        bound = imm
                    elif j in ("JNC", "JAE", "JC", "JB"):
                        bound = imm - 1
                    else:
                        return None
                    if not 0 <= bound < 0x400:
                        return None
                    if bt is not None:
                        return max(self.u8(bt + k) for k in range(bound + 1)) + 1, f"cmp {imm:#x} + byte table {bt:#x}"
                    return bound + 1, f"cmp {imm:#x}"
            cur = cur.getPrevious()
        return None

    def scan_table(self, table, lo, stops):
        n = 0
        while n < 1024:
            t = self.u32(table + 4 * n)
            if not (lo <= t < table) or t in stops:
                break
            n += 1
        return n

    def recover_switch(self, f, jmp, reg, table):
        """Add references / override for one unresolved jump. Returns True when something changed."""
        from ghidra.program.model.symbol import RefType, SourceType
        from ghidra.program.model.pcode import JumpTable
        from ghidra.program.model.data import ArrayDataType, PointerDataType, DataUtilities
        from ghidra.app.cmd.disassemble import DisassembleCommand
        from java.util import ArrayList
        entry = f.getEntryPoint().getOffset()
        stops = {x.getEntryPoint().getOffset() for x in self.fm.getFunctions(True)} - {entry}
        jva = jmp.getAddress().getOffset()
        scan = self.scan_table(table, entry, stops)
        bound = self.table_bound(jmp, reg)
        if bound is not None and bound[0] > 0:
            count, how = bound
            if scan != count:
                self.note(f"{jva:08X}: bound ({how}) gives {count} entries, table scan {scan}; using the bound")
        else:
            count, how = scan, "table scan"
        targets = [self.u32(table + 4 * k) for k in range(count)]
        bad = [t for t in targets if not self.in_text(t) or t in stops]
        if count == 0 or bad:
            self.note(f"{jva:08X}: switch table {table:#x} not recoverable (count {count}, bad entries {len(bad)})")
            return False
        distinct = sorted(set(targets))
        added = 0
        for t in distinct:
            ta = self.addr(t)
            have = any(r.getToAddress() == ta and r.getReferenceType() == RefType.COMPUTED_JUMP
                       for r in jmp.getReferencesFrom())
            if not have:
                jmp.addOperandReference(0, ta, RefType.COMPUTED_JUMP, SourceType.ANALYSIS)
                added += 1
        newly = 0
        for t in distinct:
            if self.L.getInstructionAt(self.addr(t)) is None:
                DisassembleCommand(self.addr(t), None, True).applyTo(self.p, self.mon)
                newly += 1
        dl = ArrayList()
        for t in targets:
            dl.add(self.addr(t))
        JumpTable(jmp.getAddress(), dl, True, 0).writeOverride(f)
        tdata = self.L.getDefinedDataAt(self.addr(table))
        if tdata is None or tdata.getLength() != 4 * count:
            try:
                dt = ArrayDataType(PointerDataType(None, self.p.getDataTypeManager()), count, 4,
                                   self.p.getDataTypeManager())
                DataUtilities.createData(self.p, self.addr(table), dt, -1, False,
                                         DataUtilities.ClearDataMode.CLEAR_ALL_CONFLICT_DATA)
            except Exception as e:
                self.note(f"{jva:08X}: table at {table:#x} not marked as data: {e}")
        self.change("switch", jva, f"jmp [{reg.lower()}*4+{table:#x}]: {count} entries ({how}), "
                    f"{len(distinct)} distinct targets, +{added} COMPUTED_JUMP refs, "
                    f"{newly} targets newly disassembled, jump-table override written")
        return True

    def fix_body(self, f):
        from ghidra.app.cmd.function import CreateFunctionCmd
        before = f.getBody().getNumAddresses()
        CreateFunctionCmd.fixupFunctionBody(self.p, f, self.mon)
        after = f.getBody().getNumAddresses()
        if after != before:
            n, ret, _ = self.insn_stats(f)
            self.change("body", f.getEntryPoint().getOffset(),
                        f"{f.getName()} body {before} -> {after} bytes, now {n} instructions, {ret} RET")

    def recover_function_switches(self, f, only_jump=None):
        """Resolve every unresolved table jump of f (repeat: new code can hold more switches)."""
        done = 0
        for _ in range(30):
            todo = self.unresolved_jumps(f.getBody())
            if only_jump is not None:
                todo = [t for t in todo if t[0].getAddress().getOffset() == only_jump]
            progress = False
            for jmp, reg, table in todo:
                if self.recover_switch(f, jmp, reg, table):
                    progress = True
                    done += 1
            if not progress:
                break
            self.fix_body(f)
        return done

    # -- missed functions
    def candidates(self):
        """Pointers in .rdata/.data into .text that no function contains: {target: [(ref address, block)]}."""
        found = {}
        for blk in self.mem.getBlocks():
            if blk.getName() not in (".rdata", ".data"):
                continue
            lo, hi = blk.getStart().getOffset(), blk.getEnd().getOffset()
            from jpype import JByte
            buf = JByte[hi - lo + 1]
            blk.getBytes(blk.getStart(), buf)
            data = memoryview(buf).tobytes()
            for off in range(0, len(data) - 3, 4):
                v = int.from_bytes(data[off:off + 4], "little")
                if self.in_text(v) and self.fn_containing(v) is None:
                    found.setdefault(v, []).append((lo + off, blk.getName()))
        return found

    def neighbours(self, ref_va):
        """How many of the 4 pointers either side of the reference also point into .text."""
        n = 0
        for d in (-8, -4, 4, 8):
            try:
                n += self.in_text(self.u32(ref_va + d))
            except Exception:
                pass
        return n

    def evaluate(self, va):
        """Try to make a function at va and roll it back. Returns a dict."""
        from ghidra.app.cmd.function import CreateFunctionCmd
        from ghidra.app.cmd.disassemble import DisassembleCommand
        res = {"address": f"{va:08X}"}
        tx = self.p.startTransaction("evaluate")
        try:
            if self.L.getInstructionAt(self.addr(va)) is None:
                if self.L.getDefinedDataAt(self.addr(va)) is not None:
                    res["verdict"], res["why"] = "review", "defined data at the target"
                    return res
                DisassembleCommand(self.addr(va), None, True).applyTo(self.p, self.mon)
            ok = CreateFunctionCmd(self.addr(va), True).applyTo(self.p, self.mon)
            f = self.fm.getFunctionAt(self.addr(va))
            if not ok or f is None:
                res["verdict"], res["why"] = "reject", "no function can be created"
                return res
            n, ret, _ = self.insn_stats(f)
            size = int(f.getBody().getNumAddresses())
            first = []
            ins = self.L.getInstructionAt(self.addr(va))
            for _ in range(3):
                if ins is None:
                    break
                first.append(str(ins).lower())
                ins = ins.getNext()
            start_mn = self.L.getInstructionAt(self.addr(va)).getMnemonicString()
            res.update(insns=n, bytes=size, rets=ret, start="; ".join(first))
            problems = []
            if ret == 0:
                problems.append("no RET")
            if n < MIN_INSNS:
                problems.append(f"only {n} instructions")
            if start_mn not in START_OK:
                problems.append(f"starts with {start_mn}")
            res["verdict"] = "clean" if not problems else "review"
            res["why"] = ", ".join(problems) or "ret-terminated body, plausible start"
            return res
        finally:
            self.p.endTransaction(tx, False)

    def create_function(self, va):
        from ghidra.app.cmd.function import CreateFunctionCmd
        from ghidra.app.cmd.disassemble import DisassembleCommand
        if self.fm.getFunctionAt(self.addr(va)) is not None:
            return self.fm.getFunctionAt(self.addr(va))
        if self.L.getInstructionAt(self.addr(va)) is None:
            DisassembleCommand(self.addr(va), None, True).applyTo(self.p, self.mon)
        if not CreateFunctionCmd(self.addr(va), True).applyTo(self.p, self.mon):
            self.note(f"{va:08X}: CreateFunctionCmd failed")
            return None
        f = self.fm.getFunctionAt(self.addr(va))
        if f is None:       # the address went into a function created earlier in this run
            owner = self.fm.getFunctionContaining(self.addr(va))
            self.note(f"{va:08X}: no function starts here after CreateFunctionCmd (inside "
                      f"{owner.getName() if owner else 'nothing'})")
            return None
        n, ret, _ = self.insn_stats(f)
        self.change("function", va, f"created {f.getName()}: {n} instructions, {f.getBody().getNumAddresses()} bytes")
        return f

    # -- decompiler check
    def decompile(self, f):
        from ghidra.app.decompiler import DecompInterface
        d = DecompInterface()
        d.openProgram(self.p)
        try:
            r = d.decompileFunction(f, 600, self.mon)
        finally:
            d.dispose()
        c = r.getDecompiledFunction().getC() if r.decompileCompleted() else ""
        return {"completed": bool(r.decompileCompleted()), "lines": c.count("\n"), "switches": c.count("switch("),
                "warnings": sorted(set(re.findall(r"WARNING: [^\n*]+", c))),
                "case_labels": len(re.findall(r"\bcase\b", c)),
                "error": str(r.getErrorMessage() or "")}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=("trilogy", "eqgfx", "eqmain"))   # eqgfx, eqmain: pointer-only functions
    ap.add_argument("--dry-run", action="store_true", help="do everything, roll back, save nothing")
    ap.add_argument("--only", help="comma list of names (send_social) / addresses (0042A26F) to fix or create")
    ap.add_argument("--all", action="store_true", help="also create every clean missed function")
    ap.add_argument("--project-dir")
    ap.add_argument("--summary", help="report path (default build\\ghidra\\<client>.recover.json)")
    ap.add_argument("--no-decompile", action="store_true")
    args = ap.parse_args()
    ws = eqd.Workspace()

    only = [s.strip() for s in args.only.split(",")] if args.only else None
    want_switch, want_create = dict(SWITCH_FIXES), set(DEFAULT_CREATE)
    if args.client != "trilogy":    # the named fixes and default creates are eqgame.exe addresses
        want_switch, want_create = {}, set()
    if only is not None:
        want_switch = {k: v for k, v in SWITCH_FIXES.items()
                       if k in only or f"{v[0]:08X}" in [o.upper() for o in only]}
        want_create = {int(o, 16) for o in only if re.fullmatch(r"[0-9a-fA-F]{6,8}", o)
                       and int(o, 16) not in {v[0] for v in SWITCH_FIXES.values()}}

    pyghidra = eqd.start(ws)
    project = (pyghidra.open_project(args.project_dir, args.client) if args.project_dir
               else eqd.open_project(ws, args.client))
    report = {"client": args.client, "dry_run": args.dry_run, "only": only}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            from ghidra.util.task import TaskMonitor
            R = Recover(program, pyghidra, args.dry_run)

            # --- candidates first: each is tried and rolled back, before the real transaction
            cands = R.candidates()
            print(f"{len(cands)} pointers from .rdata/.data into uncovered .text")
            evals = []
            for va in sorted(cands):
                e = R.evaluate(va)
                refs = cands[va]
                e["refs"] = [f"{r:08X} ({b})" for r, b in refs[:4]]
                e["ref_count"] = len(refs)
                e["neighbours_in_text"] = R.neighbours(refs[0][0])
                evals.append(e)
            clean = [e for e in evals if e["verdict"] == "clean"]
            print(f"candidates: {len(clean)} clean, {len(evals) - len(clean)} for review/rejected")

            creates = sorted(want_create | ({int(e['address'], 16) for e in clean} if args.all else set()))
            if only is not None and args.all:
                creates = sorted(want_create)
            before_stats = eqd.function_stats(program)["text"]

            tx = program.startTransaction("recover_code")
            ok = False
            try:
                fixed = {}
                for name, (entry, jva) in want_switch.items():
                    f = R.fm.getFunctionAt(R.addr(entry))
                    if f is None:
                        R.note(f"{name}: no function at {entry:08X}")
                        continue
                    n0, _, _ = R.insn_stats(f)
                    R.recover_function_switches(f, only_jump=jva)
                    R.fix_body(f)
                    n1, ret, last = R.insn_stats(f)
                    mx = f.getBody().getMaxAddress().getOffset()
                    fixed[name] = {"entry": f"{entry:08X}", "insns_before": n0, "insns_after": n1,
                                   "max_address": f"{mx:08X}", "rets": ret,
                                   "last_insn": f"{last.getAddress().getOffset():08X} {last}" if last else None}
                    if not args.no_decompile:
                        fixed[name]["decompile"] = R.decompile(f)
                report["switch_fixes"] = fixed
                created = {}
                for va in creates:
                    f = R.create_function(va)
                    if f is None:
                        continue
                    R.recover_function_switches(f)
                    R.fix_body(f)
                    n, ret, last = R.insn_stats(f)
                    created[f"{va:08X}"] = {"name": f.getName(), "insns": n, "bytes": int(f.getBody().getNumAddresses()),
                                           "max_address": f"{f.getBody().getMaxAddress().getOffset():08X}", "rets": ret}
                    if not args.no_decompile:
                        created[f"{va:08X}"]["decompile"] = R.decompile(f)
                report["created"] = created
                report["unresolved_after"] = [
                    {"function": f.getName(), "jump": f"{i.getAddress().getOffset():08X}", "table": f"{t:#x}"}
                    for f in ([R.fm.getFunctionAt(R.addr(e))
                               for e, _ in SWITCH_FIXES.values()] + [R.fm.getFunctionAt(R.addr(v)) for v in creates])
                    if f is not None for i, _, t in R.unresolved_jumps(f.getBody())]
                after_stats = eqd.function_stats(program)["text"]
                ok = True
            finally:
                commit = ok and not args.dry_run and bool(R.changes)
                program.endTransaction(tx, commit)
            if commit:
                program.save("recover_code", TaskMonitor.DUMMY)
                print("saved the project")
            else:
                print("DRY RUN: nothing saved" if args.dry_run else
                      "nothing to change" if ok else "FAILED: rolled back")
            report["text_before"], report["text_after"] = before_stats, after_stats
            report["changes"] = R.changes
            report["notes"] = R.notes
            created_set = {f"{v:08X}" for v in creates}
            for e in evals:
                e["created"] = e["address"] in created_set
                e["reason"] = (f"{e['verdict']}: {e['why']}" +
                               (f"; {e['insns']} insns, {e['bytes']} B; starts '{e['start'][:70]}'" if "insns" in e else "") +
                               f"; pointer {e['refs'][0]}" + (f" (+{e['ref_count'] - 1} more)" if e["ref_count"] > 1 else "") +
                               f", {e['neighbours_in_text']}/4 neighbouring slots also point into .text")
            report["missed_functions"] = evals
    finally:
        project.close()
    from pathlib import Path
    out = Path(args.summary) if args.summary else ws.reports / f"{args.client}.recover.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"wrote {out}")
    tsv = out.with_suffix(".missed.tsv")
    rows = sorted(report.get("missed_functions", []), key=lambda e: (e["verdict"] != "clean", -e.get("bytes", 0)))
    tsv.write_text("address\tverdict\tcreated\tinsns\tbytes\treason\n" + "".join(
        f"{e['address']}\t{e['verdict']}\t{int(e['created'])}\t{e.get('insns', '')}\t{e.get('bytes', '')}\t{e['reason']}\n"
        for e in rows), encoding="utf-8")
    print(f"wrote {tsv}")
    print(f"{len(report.get('changes', []))} changes; missed functions listed: {len(report.get('missed_functions', []))}")


if __name__ == "__main__":
    main()
