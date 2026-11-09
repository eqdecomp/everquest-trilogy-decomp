r"""Apply the VC6 Function ID database (build_vc6_fid.py) to a client project and tag library code.

    apply_fid.py <trilogy|takp|beta|eqmain|eqgfx> [--tag-only]

Every function with a Function ID match (Ghidra's stock databases or ours) gets the function tag
LIBRARY. Writes build\ghidra\<client>.fid.json.

`--tag-only` skips the database (no vc6_x86.fidb needed, e.g. on a machine where `build_vc6_fid.py` hasn't run):
it only tags the functions that already carry a Function ID bookmark from the analysis' own FID pass.
"""
import argparse

import eqd_ghidra as eqd
from build_vc6_fid import FID_NAME


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=["trilogy", "takp", "beta", "eqmain", "eqgfx"])
    ap.add_argument("--tag-only", action="store_true",
                    help="no database: tag the functions that already have a Function ID bookmark")
    args = ap.parse_args()
    ws = eqd.Workspace()
    fidb = ws.reports / FID_NAME
    if not args.tag_only and not fidb.exists():
        raise SystemExit(f"{fidb} missing; run build_vc6_fid.py build (or pass --tag-only)")

    pyghidra = eqd.start(ws)
    from java.io import File
    from ghidra.feature.fid.cmd import ApplyFidEntriesCommand
    from ghidra.feature.fid.db import FidFileManager
    from ghidra.feature.fid.service import FidService
    from ghidra.util.task import TaskMonitor

    mgr = FidFileManager.getInstance()
    fid_file = None
    if not args.tag_only:
        fid_file = mgr.addUserFidFile(File(str(fidb)))
        fid_file.setActive(True)
    service = FidService()
    project = eqd.open_project(ws, args.client)
    report = {"client": args.client, "fidb": None if args.tag_only else str(fidb), "tag_only": args.tag_only}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            before = eqd.function_stats(program)
            with pyghidra.transaction(program, "Apply VC6 FID"):
                if not args.tag_only:
                    cmd = ApplyFidEntriesCommand(program.getMemory(), service.getDefaultScoreThreshold(),
                                                 service.getDefaultMultiNameThreshold(), False, True)
                    cmd.applyTo(program, TaskMonitor.DUMMY)
                fid_addrs = eqd.fid_addresses(program)
                for f in program.getFunctionManager().getFunctions(True):
                    if f.getEntryPoint() in fid_addrs:
                        f.addTag(eqd.LIBRARY_TAG)
            after = eqd.function_stats(program)
            program.save("Apply VC6 FID", TaskMonitor.DUMMY)
            report["fid_matched_before"] = before["fid_matched"]
            report["fid_matched_after"] = after["fid_matched"]
            report["added_by_vc6_fid"] = after["fid_matched"] - before["fid_matched"]
            report["stats"] = after
    finally:
        project.close()
        if fid_file is not None:
            mgr.removeUserFile(fid_file)
    ws.write_report(f"{args.client}.fid.json", report)
    print(report)


if __name__ == "__main__":
    main()
