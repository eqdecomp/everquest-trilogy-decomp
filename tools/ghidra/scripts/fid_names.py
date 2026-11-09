r"""Library functions that Function ID leaves out because they are too short to score.

    fid_names.py trilogy|takp       -> eq-<client>\names\<program>.fid.csv (trilogy) or build\names\<client>\

Ghidra's FID only accepts a match above a score threshold that grows with the function's size, so the
VC6 CRT's short helpers (__EH_prolog, __NLG_Notify, _fopen, ...) stay unmatched even though
vc6_x86.fidb has them. This script hashes every function without a FID match and looks the exact full
hash up in vc6_x86.fidb. A row is written when all records with that hash carry one name and no other
function of the client has the same hash. Rows are library code: the note starts with "library:", and
apply_names.py tags them LIBRARY. Confidence: "certain" from 10 code units up, "high" below (tiny
functions can share a hash with a different routine of the same shape).
Reads the project only; run it after apply_fid.py.
"""
import argparse
import csv
import re
from collections import defaultdict

import eqd_ghidra as eqd
from build_vc6_fid import FID_NAME

FIELDS = ["address", "name", "confidence", "source", "note", "from_address", "file"]
MIN_CODE_UNITS = 5
CERTAIN_CODE_UNITS = 10


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=["trilogy", "takp"])
    args = ap.parse_args()
    ws = eqd.Workspace()
    pyghidra = eqd.start(ws)
    from java.io import File
    from ghidra.feature.fid.db import FidFileManager
    from ghidra.feature.fid.service import FidService

    mgr = FidFileManager.getInstance()
    fid_file = mgr.addUserFidFile(File(str(ws.reports / FID_NAME)))
    db = fid_file.getFidDB(False)
    service = FidService()
    project = eqd.open_project(ws, args.client)
    rows, report = [], {"client": args.client, "hashed": 0, "db_hits": 0, "ambiguous_name": 0,
                        "hash_not_unique_in_client": 0}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            fid_addrs = eqd.fid_addresses(program)
            by_hash = defaultdict(list)
            for f in program.getFunctionManager().getFunctions(True):
                if f.isThunk() or f.isExternal():
                    continue
                h = service.hashFunction(f)
                if h is None or h.getCodeUnitSize() < MIN_CODE_UNITS:
                    continue
                by_hash[h.getFullHash()].append((f, h))
            for full, funcs in by_hash.items():
                todo = [(f, h) for f, h in funcs if f.getEntryPoint() not in fid_addrs]
                if not todo:
                    continue
                report["hashed"] += len(todo)
                recs = list(db.findFunctionsByFullHash(full))
                if not recs:
                    continue
                report["db_hits"] += len(todo)
                names = {re.sub(r"^FID_conflict:", "", r.getName()) for r in recs}
                if len(names) != 1:
                    report["ambiguous_name"] += len(todo)
                    continue
                if len(funcs) != 1:
                    report["hash_not_unique_in_client"] += len(todo)
                    continue
                f, h = todo[0]
                libs = sorted({str(db.getLibraryForFunction(r).getLibraryFamilyName()) for r in recs})
                n = h.getCodeUnitSize()
                rows.append({
                    "address": f"{f.getEntryPoint().getOffset():08X}",
                    "name": names.pop(),
                    "confidence": "certain" if n >= CERTAIN_CODE_UNITS else "high",
                    "source": "fid",
                    "note": f"library: VC6 CRT ({', '.join(libs)}). Exact FID full hash, {n} code units: too "
                            f"short for Ghidra's FID score threshold",
                    "from_address": "", "file": ""})
    finally:
        project.close()
        db.close()
        mgr.removeUserFile(fid_file)
    stem = eqd.CLIENTS[args.client][1].rsplit(".", 1)[0]
    out_dir = ws.root / f"eq-{args.client}" / "names"
    if not out_dir.is_dir():
        out_dir = ws.build / "names" / args.client
    out = out_dir / f"{stem}.fid.csv"
    with out.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, FIELDS, lineterminator="\n")
        w.writeheader()
        for r in sorted(rows, key=lambda r: r["address"]):
            w.writerow(r)
    report["rows"] = len(rows)
    ws.write_report(f"{args.client}.fidnames.json", report)
    print(report, "->", out)


if __name__ == "__main__":
    main()
