r"""Load a client's data type archive into its Ghidra project.

    apply_types.py <client> [--project-dir DIR]

Reads the configured archive (or --archive, built by build_gdt.py) and adds every type to the
program with the replace-existing conflict handler, so a type from the archive replaces an older
copy of itself and running the script twice changes nothing the second time. The types keep the
archive's category paths (/item.h/EQ_Item, ...) and stay linked to the archive for later syncs.
Nothing is applied to functions or data yet. Report: build\ghidra\<client>.types.json.

--project-dir opens <DIR>\<client>.gpr instead of <workspace>\ghidra\<client> (for a scratch copy).
"""
import argparse

import eqd_ghidra as eqd


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    ap.add_argument("--project-dir", help="project folder to use instead of <workspace>\\ghidra\\<client>")
    ap.add_argument("--archive", help="validated archive (default from workspace config/reports)")
    args = ap.parse_args()
    ws = eqd.Workspace()
    from pathlib import Path
    gdt = Path(args.archive).resolve() if args.archive else ws.archive(args.client)
    if not gdt.is_file():
        raise SystemExit(f"no archive: {gdt} (run build_gdt.py {args.client})")

    pyghidra = eqd.start(ws)
    from java.io import File
    from java.util import ArrayList
    from ghidra.program.model.data import (BuiltInDataType, DataTypeConflictHandler, FileDataTypeManager,
                                           Array, Pointer)
    from ghidra.util.task import TaskMonitor

    project = (pyghidra.open_project(args.project_dir, args.client) if args.project_dir
               else eqd.open_project(ws, args.client))
    archive = FileDataTypeManager.openFileArchive(File(str(gdt)), False)
    report = {"client": args.client, "archive": str(gdt), "added": 0, "replaced": 0,
              "unchanged": 0, "types": [], "kept_foreign": [], "problems": []}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            pdtm = program.getDataTypeManager()
            if pdtm.getDataOrganization().getPointerSize() != archive.getDataOrganization().getPointerSize():
                raise SystemExit("archive and program disagree on the pointer size")
            todo = ArrayList()
            archive.getAllDataTypes(todo)
            # arrays, pointers and built-ins come along with the types that use them
            todo = [dt for dt in todo if not isinstance(dt, (Array, Pointer, BuiltInDataType))]
            # look up what the program had before, so types pulled in as members of an earlier one
            # still count as added
            before = {str(dt.getDataTypePath()): pdtm.getDataType(dt.getDataTypePath()) for dt in todo}
            with pyghidra.transaction(program, "Apply data type archive"):
                for dt in todo:
                    path = dt.getDataTypePath()
                    old = before[str(path)]
                    if old is not None and old.isEquivalent(dt):
                        report["unchanged"] += 1
                        continue
                    src = old.getSourceArchive() if old is not None else None
                    if src is not None and str(src.getName()) not in (gdt.stem, str(program.getName())):
                        # e.g. /WinDef.h/RECT from windows_vs12_32: never replace another
                        # archive's type with our stand-in declaration.
                        report["kept_foreign"].append(f"{path} (from {src.getName()})")
                        continue
                    new = pdtm.getDataType(path)
                    if new is None or not new.isEquivalent(dt):
                        new = pdtm.addDataType(dt, DataTypeConflictHandler.REPLACE_HANDLER)
                    if new is None or new.getLength() != dt.getLength():
                        report["problems"].append(f"{path}: not applied as expected")
                        continue
                    report["added" if old is None else "replaced"] += 1
                    report["types"].append(str(path))
            program.save("Apply data type archive", TaskMonitor.DUMMY)
    finally:
        archive.close()
        project.close()
    ws.write_report(f"{args.client}.types.json", report)
    print({k: v for k, v in report.items() if k not in ("types", "problems", "kept_foreign")},
          len(report["kept_foreign"]), "kept foreign,", len(report["problems"]), "problems")
    if report["problems"]:
        raise SystemExit("\n".join(report["problems"][:20]))


if __name__ == "__main__":
    main()
