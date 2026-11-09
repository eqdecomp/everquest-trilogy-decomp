r"""Create (or recreate) the Ghidra project for one client and run auto-analysis.

    make_project.py <trilogy|takp|mac> [--overwrite]

Writes build\ghidra\<client>.project.json with the analysis time and function counts.
"""
import argparse
import re
import shutil
import time

import eqd_ghidra as eqd


# The default 60 s is too short for the clients' largest functions (tens of KB with big switches);
# when it expires the switch targets stay undisassembled.
ANALYSIS_INTS = {
    "Decompiler Switch Analysis.Analysis Decompiler Timeout (sec)": 900,
}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    ap.add_argument("--overwrite", action="store_true", help="delete an existing project first")
    args = ap.parse_args()

    ws = eqd.Workspace()
    binary = ws.binary(args.client)
    proj_dir = ws.ghidra / args.client
    if (proj_dir / f"{args.client}.gpr").exists():
        if not args.overwrite:
            raise SystemExit(f"{proj_dir} exists; pass --overwrite to rebuild it")
        shutil.rmtree(proj_dir)

    pyghidra = eqd.start(ws)
    from ghidra.program.model.symbol import SymbolType
    from ghidra.util.task import TaskMonitor

    project = eqd.open_project(ws, args.client, create=True)
    report = {"client": args.client, "binary": str(binary), "size": binary.stat().st_size}
    try:
        t0 = time.time()
        results = (pyghidra.program_loader().project(project).source(str(binary))
                   .name(eqd.CLIENTS[args.client][1]).projectFolderPath("/").load())
        try:
            program = results.getPrimaryDomainObject()
            report["format"] = str(program.getExecutableFormat())
            report["language"] = str(program.getLanguageID())
            report["compiler_spec"] = str(program.getCompilerSpec().getCompilerSpecID())
            report["import_seconds"] = round(time.time() - t0, 1)

            # Symbols straight from the file, before analysis adds or demangles anything.
            st = program.getSymbolTable()
            mangled = set()
            mangled_funcs = 0
            loader_syms = 0
            for s in st.getAllSymbols(False):
                loader_syms += 1
                n = s.getName()
                if re.match(r"_?_Z", n):
                    mangled.add(n)
                    if s.getSymbolType() == SymbolType.FUNCTION:
                        mangled_funcs += 1
            report["loader_symbols"] = loader_syms
            report["loader_mangled_cxx_symbols"] = len(mangled)
            report["loader_mangled_cxx_functions"] = mangled_funcs
            report["loader_functions"] = program.getFunctionManager().getFunctionCount()

            opts = pyghidra.analysis_properties(program)
            with pyghidra.transaction(program, "Analysis options"):
                for n, v in ANALYSIS_INTS.items():
                    if opts.contains(n):
                        opts.setInt(n, v)
            t1 = time.time()
            log = pyghidra.analyze(program)
            report["analysis_seconds"] = round(time.time() - t1, 1)
            report["analysis_log_tail"] = log.strip().splitlines()[-15:]
            report.update(eqd.function_stats(program))
            results.save(TaskMonitor.DUMMY)
        finally:
            results.close()
    finally:
        project.close()
    ws.write_report(f"{args.client}.project.json", report)
    print({k: v for k, v in report.items() if k not in ("analysis_log_tail",)})


if __name__ == "__main__":
    main()
