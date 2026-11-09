r"""Build a Function ID database from the VC6 static libraries.

    build_vc6_fid.py import <LIB> [--overwrite]   import + analyze every object of one library
    build_vc6_fid.py build                        populate build\ghidra\vc6_x86.fidb from the imported libs

Each library gets its own project (ghidra\vc6libs\<LIB>\) so the imports can run in parallel. The
installed VC6 is RTM (12.00.8168); the clients were built with a later service pack, so CRT functions
that changed between RTM and that SP will not match.
"""
import argparse
import shutil
import time
from pathlib import Path

import eqd_ghidra as eqd

# name -> path under [toolchain] vc6. The clients link the CRT statically and import no MFC DLL,
# so MFC is left out. MSVCRT.LIB only contributes its startup objects.
LIBS = {
    "LIBC": "Lib/LIBC.LIB",
    "LIBCMT": "Lib/LIBCMT.LIB",
    "LIBCP": "Lib/LIBCP.LIB",
    "LIBCPMT": "Lib/LIBCPMT.LIB",
    "MSVCRT": "Lib/MSVCRT.LIB",
}
FID_NAME = "vc6_x86.fidb"
LIB_VERSION = "6.0"
# toolchain key -> library variant name stored in the database
TOOLCHAINS = {
    "vc6": "SP4-CRT-8797",
    "vc6_rtm": "RTM-CRT-8168",
    "vc6_sp5": "SP5-CRT-8047",
}
DEFAULT_TOOLCHAIN = "vc6"
LANGUAGE = "x86:LE:32:default"
# Analyzer settings from Ghidra's FunctionIDHeadlessPrescript.java
ANALYZERS_OFF = ["Function ID", "Library Identification", "Demangler Microsoft", "Demangler GNU",
                 "PDB Universal", "PDB MSDIA"]
ANALYZERS_ON = ["Scalar Operand References"]


def lib_project_dir(ws, tc, lib) -> Path:
    return ws.ghidra / "vc6libs" / tc / lib


def walk(fs, d=None):
    for f in fs.getListing(d):
        if f.isDirectory():
            yield from walk(fs, f)
        else:
            yield f


def cmd_import(ws, lib, tc, overwrite):
    src = ws.path("toolchain", tc) / LIBS[lib]
    pdir = lib_project_dir(ws, tc, lib)
    if (pdir / f"{lib}.gpr").exists():
        if not overwrite:
            raise SystemExit(f"{pdir} exists; pass --overwrite to rebuild it")
        shutil.rmtree(pdir)
    pdir.mkdir(parents=True, exist_ok=True)

    pyghidra = eqd.start(ws)
    from ghidra.program.model.listing import Program
    from ghidra.util.task import TaskMonitor

    project = pyghidra.open_project(str(pdir), lib, create=True)
    report = {"lib": lib, "toolchain": tc, "variant": TOOLCHAINS[tc], "source": str(src), "objects": 0, "imported": 0, "skipped": 0, "functions": 0}
    t0 = time.time()
    try:
        fs = pyghidra.open_filesystem(str(src))
        try:
            for i, gf in enumerate(walk(fs)):
                report["objects"] += 1
                name = f"{i:04d}_{Path(str(gf.getName())).name}"
                try:
                    results = (pyghidra.program_loader().project(project)
                               .source(fs.getByteProvider(gf, TaskMonitor.DUMMY))
                               .language(LANGUAGE).compiler("windows")
                               .name(name).projectFolderPath("/").load())
                except Exception as e:  # import-library members are not COFF objects
                    report["skipped"] += 1
                    if report["skipped"] <= 5:
                        print(f"skip {gf.getPath()}: {str(e).splitlines()[0][:120]}")
                    continue
                try:
                    program = results.getPrimaryDomainObject()
                    opts = pyghidra.analysis_properties(program)
                    with pyghidra.transaction(program, "FID analysis options"):
                        for n in ANALYZERS_OFF:
                            if opts.contains(n):
                                opts.setBoolean(n, False)
                        for n in ANALYZERS_ON:
                            if opts.contains(n):
                                opts.setBoolean(n, True)
                    pyghidra.analyze(program)
                    report["functions"] += program.getFunctionManager().getFunctionCount()
                    results.save(TaskMonitor.DUMMY)
                    report["imported"] += 1
                finally:
                    results.close()
                if report["imported"] % 100 == 0:
                    print(f"{lib}: {report['imported']} objects, {time.time() - t0:.0f}s", flush=True)
        finally:
            fs.close()
    finally:
        project.close()
    report["seconds"] = round(time.time() - t0, 1)
    ws.write_report(f"vc6lib.{tc}.{lib}.json", report)
    print(report)


def cmd_build(ws, tcs):
    pyghidra = eqd.start(ws)
    from java.io import File
    from java.util import ArrayList
    from ghidra.feature.fid.db import FidFileManager
    from ghidra.feature.fid.service import FidService
    from ghidra.framework.model import ProjectDataUtils
    from ghidra.program.model.lang import LanguageID
    from ghidra.util.task import TaskMonitor

    out = ws.reports / FID_NAME
    if out.exists():
        out.unlink()
    common = ArrayList()
    sym_file = ws.path("toolchain", "ghidra") / "Ghidra/Features/FunctionID/data/common_symbols_win32.txt"
    for line in sym_file.read_text().splitlines():
        if line.strip():
            common.add(line.strip())

    mgr = FidFileManager.getInstance()
    mgr.createNewFidDatabase(File(str(out)))
    fid_file = mgr.addUserFidFile(File(str(out)))
    fid_db = fid_file.getFidDB(True)
    service = FidService()
    report = {"fidb": str(out), "variants": [TOOLCHAINS[t] for t in tcs], "libraries": {}}
    try:
        for tc in tcs:
            variant = TOOLCHAINS[tc]
            for lib in LIBS:
                pdir = lib_project_dir(ws, tc, lib)
                if not (pdir / f"{lib}.gpr").exists():
                    print(f"{tc}/{lib}: not imported, skipped")
                    continue
                project = pyghidra.open_project(str(pdir), lib)
                try:
                    files = ArrayList()
                    for df in ProjectDataUtils.descendantFiles(project.getProjectData().getRootFolder()):
                        files.add(df)
                    r = service.createNewLibraryFromPrograms(
                        fid_db, lib, LIB_VERSION, variant, files, None, LanguageID(LANGUAGE), None,
                        common, TaskMonitor.DUMMY)
                    report["libraries"][f"{tc}/{lib}"] = {
                        "variant": variant,
                        "programs": files.size(),
                        "attempted": r.getTotalAttempted(),
                        "added": r.getTotalAdded(),
                        "excluded": r.getTotalExcluded(),
                    }
                    print(tc, lib, report["libraries"][f"{tc}/{lib}"], flush=True)
                finally:
                    project.close()
        fid_db.saveDatabase("VC6 static libraries", TaskMonitor.DUMMY)
    finally:
        fid_db.close()
        mgr.removeUserFile(fid_file)  # keep it out of the user's Ghidra settings; apply_fid.py attaches it
    ws.write_report("vc6_fid.json", report)


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("import")
    p.add_argument("lib", choices=sorted(LIBS))
    p.add_argument("--toolchain", choices=sorted(TOOLCHAINS), default=DEFAULT_TOOLCHAIN)
    p.add_argument("--overwrite", action="store_true")
    b = sub.add_parser("build")
    b.add_argument("--toolchains", default=",".join(TOOLCHAINS),
                   help="comma-separated toolchain keys to include (default: all)")
    args = ap.parse_args()
    ws = eqd.Workspace()
    if args.cmd == "import":
        cmd_import(ws, args.lib, args.toolchain, args.overwrite)
    else:
        tcs = [t.strip() for t in args.toolchains.split(",") if t.strip()]
        bad = [t for t in tcs if t not in TOOLCHAINS]
        if bad:
            raise SystemExit(f"unknown toolchain(s) {bad}; known: {sorted(TOOLCHAINS)}")
        cmd_build(ws, tcs)


if __name__ == "__main__":
    main()
