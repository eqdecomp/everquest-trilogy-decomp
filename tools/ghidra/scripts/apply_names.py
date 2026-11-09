r"""Apply name lists to a Ghidra project.

    apply_names.py <client>

Lists are <program>.<source>.csv in eq-<client>\names\ (committed) and build\names\<client>\ (derived
lists for clients without a repo). When several lists name one address the first of "manual", "fid",
"eqclassic", "mac", "strings", "beta", "takp", then any other, wins; the rest are kept in the plate comment.
A winning row whose note starts with "library:" (third-party or CRT code) also gets the LIBRARY tag.

Names with confidence "certain" are applied as is, all others with a "_guess" suffix. "Class::Method"
puts the function in that namespace. Names are stored with source IMPORTED; a function whose current
name is USER_DEFINED (set by hand) is never touched.
"""
import argparse
import csv
import re

import eqd_ghidra as eqd

# fid: exact hash of a CRT object, beats any game-name guess. strings/beta (developer debug strings, in
# this binary / in the 1999 beta) outrank takp (descriptive list names, always low) but not mac: where
# they agree, the Mac symbol carries the class; where they differ, the csv notes say so.
# sibling: the same code in another image of the repo (queue/siblings.py --names; eqmain.dll <- eqgame.exe).
PRIORITY = ["manual", "fid", "eqclassic", "mac", "strings", "beta", "takp", "sibling"]
LIBRARY_NOTE = "library:"
STALE_PLATE = re.compile(r"^\[\w+, \w+\] ")      # first line of comment() below


def read_lists(paths):
    """address -> rows, best first"""
    by_addr = {}
    for path in paths:
        with path.open(newline="", encoding="utf-8") as f:
            for row in csv.DictReader(f):
                by_addr.setdefault(int(row["address"], 16), []).append(row)
    rank = {s: i for i, s in enumerate(PRIORITY)}
    for rows in by_addr.values():
        rows.sort(key=lambda r: rank.get(r["source"], len(rank)))
    return by_addr


def comment(rows):
    lines = []
    for i, r in enumerate(rows):
        lines.append(f"{'' if i == 0 else 'also '}[{r['source']}, {r['confidence']}] "
                     f"{r['name'] + ': ' if i else ''}{r['note']}")
        if r.get("file"):
            lines.append(f"ORIGINAL FILE: {r['file']}")
    return "\n".join(dict.fromkeys(lines))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    args = ap.parse_args()
    ws = eqd.Workspace()
    program_name = eqd.CLIENTS[args.client][1]
    stem = program_name.rsplit(".", 1)[0]
    lists = []
    for d in (ws.repo(args.client) / "names", ws.build / "names" / args.client):
        lists += sorted(d.glob(f"{stem}.*.csv"))
    by_addr = read_lists(lists)

    pyghidra = eqd.start(ws)
    from ghidra.app.util import NamespaceUtils, SymbolPath
    from ghidra.program.model.listing import CodeUnit
    from ghidra.program.model.symbol import SourceType
    from ghidra.util.task import TaskMonitor

    project = eqd.open_project(ws, args.client)
    report = {"client": args.client, "lists": [p.name for p in lists], "applied": 0, "unchanged": 0,
              "overridden_by_priority": 0, "tagged_library": 0, "reset_stale": [],
              "problems": []}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            fm = program.getFunctionManager()
            space = program.getAddressFactory().getDefaultAddressSpace()
            global_ns = program.getGlobalNamespace()
            with pyghidra.transaction(program, "Apply name lists"):
                for offset, rows in sorted(by_addr.items()):
                    row = rows[0]
                    report["overridden_by_priority"] += len(rows) - 1
                    addr = space.getAddress(offset)
                    func = fm.getFunctionAt(addr)
                    if func is None:
                        report["problems"].append(f"{row['address']}: no function entry here")
                        continue
                    path = SymbolPath(row["name"] + ("" if row["confidence"] == "certain" else "_guess"))
                    name, parent = path.getName(), path.getParent()
                    full = path.getPath()
                    if (func.getSymbol().getSource() == SourceType.USER_DEFINED
                            and func.getName(True) != full):
                        report["problems"].append(
                            f"{row['address']}: kept hand-set name {func.getName(True)} (list says {full})")
                        continue
                    if func.getName(True) == full:
                        report["unchanged"] += 1
                    else:
                        try:
                            ns = global_ns if parent is None else NamespaceUtils.createNamespaceHierarchy(
                                parent.getPath(), global_ns, program, SourceType.IMPORTED)
                            func.getSymbol().setNameAndNamespace(name, ns, SourceType.IMPORTED)
                            report["applied"] += 1
                        except Exception as e:      # invalid characters, name clashes
                            report["problems"].append(f"{row['address']}: {full}: {e}")
                            continue
                    program.getListing().setComment(addr, CodeUnit.PLATE_COMMENT, comment(rows))
                    if row["note"].startswith(LIBRARY_NOTE):
                        func.addTag(eqd.LIBRARY_TAG)
                        report["tagged_library"] += 1
                # Names this script applied earlier whose row is gone (a list was regenerated): back to
                # the default name. Recognized by source IMPORTED plus our plate comment.
                for func in fm.getFunctions(True):
                    offset = func.getEntryPoint().getOffset()
                    plate = func.getComment() or ""
                    if (offset in by_addr or func.getSymbol().getSource() != SourceType.IMPORTED
                            or not STALE_PLATE.match(plate)):
                        continue
                    func.setParentNamespace(global_ns)
                    func.setName(None, SourceType.DEFAULT)
                    func.setComment(None)
                    report["reset_stale"].append(f"{offset:08X}")
            program.save("Apply name lists", TaskMonitor.DUMMY)
    finally:
        project.close()
    ws.write_report(f"{args.client}.names.json", report)
    print({k: (v if k != "problems" else v[:20]) for k, v in report.items()}, len(report["problems"]), "problems")


if __name__ == "__main__":
    main()
