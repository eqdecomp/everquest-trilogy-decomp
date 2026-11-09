r"""Build a validated 32-bit Windows Ghidra archive from all nested client headers.

    build_gdt.py <client> [--out FILE] [--layouts DIR --overlays DIR --field-edits FILE]
                 [--canonical fail|warn]

Repository routing comes from Workspace.repo (registry, EQD_REPO, or [repos]).
The generated C umbrella, archive and report go to the configured reports directory;
the source repository is read-only. Source C_ASSERTs and optional canonical JSON
layouts are checked independently against Ghidra. Failed validation never replaces
an existing archive. Source assertions always block; --canonical warn records
canonical-JSON disagreements as report warnings instead (the source headers are the
byte-verified authority). EQD_CONFIG selects a session-local paths.local.ini copy.
"""
import argparse
import re
import shutil
import tempfile
import json
from pathlib import Path

import eqd_ghidra as eqd
import gdt_headers

SIZE_ASSERT = re.compile(r"^\s*sizeof\s*\(\s*(\w+)\s*\)\s*==\s*(0[xX][0-9a-fA-F]+|\d+)\s*$")
OFFSET_ASSERT = re.compile(r"^\s*offsetof\s*\(\s*(\w+)\s*,\s*(\w+)\s*\)\s*"
                           r"(?:([+-])\s*(0[xX][0-9a-fA-F]+|\d+)\s*)?"
                           r"==\s*(0[xX][0-9a-fA-F]+|\d+)\s*$")
LANGUAGE, COMPILER = "x86:LE:32:default", "windows"
PARSE_ARGS = []                  # no -D: Ghidra turns every define into an enum in the archive


def find_type(dtm, name):
    """The structure/union/enum called name, looking through typedefs."""
    from ghidra.program.model.data import TypeDef
    from java.util import ArrayList
    found = ArrayList()
    dtm.findDataTypes(name, found)
    for dt in found:
        while isinstance(dt, TypeDef):
            dt = dt.getDataType()
        if dt.getName() == name or dt.getName() == f"{name}_":   # tag and typedef may share the name
            return dt
    for dt in found:
        base = dt.getBaseDataType() if isinstance(dt, TypeDef) else dt
        return base
    return None


def check(dtm, sizes, offsets):
    from ghidra.program.model.data import Composite
    problems, checked = [], {"sizes": 0, "offsets": 0}
    for name, want, where in sizes:
        dt = find_type(dtm, name)
        if dt is None:
            problems.append(f"{where}: {name} not in the archive")
            continue
        got = dt.getLength()
        checked["sizes"] += 1
        if got != want:
            problems.append(f"{where}: sizeof({name}) is {got:#x} in Ghidra, header asserts {want:#x}")
    for name, field, want, where in offsets:
        dt = find_type(dtm, name)
        if not isinstance(dt, Composite):
            problems.append(f"{where}: {name} is not a struct in the archive")
            continue
        comp = component(dt, field)
        checked["offsets"] += 1
        if comp is None:
            problems.append(f"{where}: {name}.{field} not found")
        elif comp[0] != want:
            problems.append(f"{where}: offsetof({name}, {field}) is {comp[0]:#x} in Ghidra, "
                            f"header asserts {want:#x}")
    return checked, problems


def component(dt, field):
    """Find a dotted member, including members promoted through anonymous unions."""
    from ghidra.program.model.data import Composite, TypeDef
    while isinstance(dt, TypeDef):
        dt = dt.getDataType()
    if not isinstance(dt, Composite):
        return None
    head, _, tail = field.partition(".")
    for c in dt.getDefinedComponents():
        name = str(c.getFieldName() or "")
        if name == head:
            if not tail:
                return c.getOffset(), c.getDataType()
            sub = component(c.getDataType(), tail)
        elif not name or name.startswith(("_s_", "_u_", "_struct_", "_union_", "_base_")):
            sub = component(c.getDataType(), field)
        else:
            continue
        if sub is not None:
            return c.getOffset() + sub[0], sub[1]
    return None


def assertions(idx):
    sizes, offsets = [], []
    for fi in idx.files.values():
        # Comments may follow a source assertion, or the expression may span lines.
        for m in re.finditer(r"\bC_ASSERT\s*\((.*?)\)\s*;", fi.code, re.S):
            expr = m.group(1)
            where = f"{fi.rel}:{fi.lines.line(m.start())}"
            if s := SIZE_ASSERT.fullmatch(expr):
                sizes.append((s[1], int(s[2], 0), where))
            elif o := OFFSET_ASSERT.fullmatch(expr):
                delta = int(o[4], 0) * (-1 if o[3] == "-" else 1) if o[4] else 0
                offsets.append((o[1], o[2], int(o[5], 0) - delta, where))
            else:
                raise ValueError(f"{where}: uncheckable C_ASSERT: {expr}")
    return sizes, offsets


def canonical_layouts(directory, overlays=None, field_edits=None):
    """Latest layout per filename; historical snapshots and mapping files are excluded."""
    paths = {p.name: p for p in Path(directory).glob("*.json") if ".pre_" not in p.name}
    if overlays:
        paths.update({p.name: p for p in Path(overlays).glob("*.json")})
    edits = json.loads(Path(field_edits).read_text(encoding="utf-8")) if field_edits else {}
    mapping = edits.get("mapping", {})
    choices = {(x["layout"], x["family"], x["name"]) for x in edits.get("choices", [])}
    docs = []
    for name, path in sorted(paths.items()):
        doc = json.loads(path.read_text(encoding="utf-8"))
        if not isinstance(doc, dict) or "family" not in doc:
            continue
        for f in doc.get("fields", []):
            if (name, doc["family"], f["name"]) in choices:
                f["name"] = mapping.get(f["name"], f["name"])
        doc["_source"] = str(path)
        docs.append(doc)
    return docs


def check_canonical(dtm, docs, client):
    checked = {"layouts": 0, "sizes": 0, "offsets": 0, "widths": 0}
    problems, omitted = [], []
    def number(x):
        return int(x, 0) if isinstance(x, str) else x
    for doc in docs:
        if doc["client"] != client:
            continue
        for record, name in [(doc, doc["family"])] + [(t, t["name"]) for t in doc.get("types", [])]:
            dt = find_type(dtm, name)
            where = doc["_source"] + ":" + name
            if dt is None:
                omitted.append(name)
                problems.append(f"{where}: canonical type omitted")
                continue
            checked["layouts"] += 1
            if record.get("size") is not None:
                checked["sizes"] += 1
                if dt.getLength() != number(record["size"]):
                    problems.append(f"{where}: size {dt.getLength():#x}, canonical {record['size']}")
            for f in record.get("fields", []):
                members = [f] + (f.get("union") or []) + (f.get("overlay") or [])
                for member in members:
                    # The first entry defines storage; union roles and overlay views
                    # start at the same offset as that field.
                    result = component(dt, member["name"])
                    checked["offsets"] += 1
                    want = number(f["offset"])
                    if result is None or result[0] != want:
                        problems.append(f"{where}.{member['name']}: offset {None if result is None else hex(result[0])}, canonical {want:#x}")
                    if result is not None and member.get("width") is not None:
                        count = member.get("count") or 1
                        dims = count if isinstance(count, list) else [count]
                        total = number(member["width"])
                        for dim in dims:
                            total *= number(dim)
                        checked["widths"] += 1
                        if result[1].getLength() != total:
                            problems.append(f"{where}.{member['name']}: width {result[1].getLength():#x}, canonical {total:#x}")
    return checked, problems, omitted


def recategorize(dtm, umbrella, categories):
    """Move types the parser filed under /<umbrella> to /<declaring header>.

    Program types from earlier archives live at /player.h/EQPlayer etc.; keeping those
    paths lets apply_types replace them in place instead of adding a second copy.
    Returns (moved count, names left under the umbrella category)."""
    from ghidra.program.model.data import CategoryPath, Array, Composite, Pointer
    root = CategoryPath("/" + umbrella)
    # Callback typedefs bring a function definition in <category>/functions, and
    # #defines become define_<NAME> enums in <category>/defines.
    subs = {str(CategoryPath("/" + umbrella + "/" + s).getPath()): s for s in ("functions", "defines")}
    moved, left = 0, []

    def ours(cat):
        return cat.equals(root) or str(cat.getPath()) in subs
    tx = dtm.startTransaction("Categorize by source header")
    try:
        for dt in list(dtm.getAllDataTypes()):
            cat = dt.getCategoryPath()
            if isinstance(dt, (Array, Pointer)) or not ours(cat):
                continue
            header = categories.get(str(dt.getName()))
            if header is None:
                continue
            sub = subs.get(str(cat.getPath()))
            dt.setCategoryPath(CategoryPath("/" + header + ("/" + sub if sub else "")))
            moved += 1
        # Anonymous unions/structs, nested and pointer-only types follow their owner.
        # Composites (by value, or pointer-only forward tags) move; a move that does
        # not take is not retried.
        changed, tried = True, set()
        while changed:
            changed = False
            for dt in list(dtm.getAllDataTypes()):
                if not isinstance(dt, Composite) or dt.getCategoryPath().equals(root):
                    continue
                for comp in dt.getDefinedComponents():
                    member = comp.getDataType()
                    while member is not None and isinstance(member, (Array, Pointer)):
                        member = member.getDataType()
                    if member is None:
                        continue
                    name = str(member.getName())
                    if (isinstance(member, Composite) and member.getCategoryPath().equals(root)
                            and name not in categories and name not in tried):
                        tried.add(name)
                        member.setCategoryPath(dt.getCategoryPath())
                        if not member.getCategoryPath().equals(root):
                            moved += 1
                            changed = True
        left = [str(dt.getPathName()) for dt in dtm.getAllDataTypes()
                if ours(dt.getCategoryPath()) and not isinstance(dt, (Array, Pointer))]
    finally:
        dtm.endTransaction(tx, True)
    return moved, sorted(left)


WINDOWS_ARCHIVE = "Ghidra/Features/Base/data/typeinfo/win32/windows_vs12_32.gdt"


def adopt_windows_types(dtm, ws, names):
    """Replace the umbrella's SDK stand-ins (RECT, HWND__, ...) with Ghidra's own
    windows_vs12_32 types, so applying the archive resolves to the program's existing
    /WinDef.h types (same source archive) instead of replacing them.
    Returns (adopted names, problems)."""
    if not names:
        return [], []
    from java.io import File
    from ghidra.program.model.data import (CategoryPath, DataTypeConflictHandler, DataTypePath,
                                           FileDataTypeManager)
    win = FileDataTypeManager.openFileArchive(File(str(Path(ws.path("toolchain", "ghidra")) / WINDOWS_ARCHIVE)), False)
    adopted, problems = [], []
    tx = dtm.startTransaction("Adopt Windows SDK types")
    try:
        for name in names:
            src = win.getDataType(DataTypePath(CategoryPath("/WinDef.h"), name))
            ours = dtm.getDataType(DataTypePath(CategoryPath("/WinDef.h"), name))
            if ours is None:
                continue
            if src is None or src.getLength() != ours.getLength():
                problems.append(f"/WinDef.h/{name}: no same-size type in {WINDOWS_ARCHIVE}")
                continue
            dtm.resolve(src, DataTypeConflictHandler.REPLACE_HANDLER)
            adopted.append(name)
    finally:
        dtm.endTransaction(tx, True)
        win.close()
    return adopted, problems


def canonical_outcome(mode, failures):
    """(blocking problems, warnings) for canonical-JSON disagreements under --canonical."""
    return ([], list(failures)) if mode == "warn" else (list(failures), [])


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", help="header folder name, e.g. trilogy")
    ap.add_argument("--out", help="archive to write (default from workspace config/reports)")
    ap.add_argument("--layouts", help="canonical layout JSON directory")
    ap.add_argument("--overlays", help="accepted layout overlays")
    ap.add_argument("--field-edits", help="accepted field rename records")
    ap.add_argument("--canonical", choices=("fail", "warn"), default="fail",
                    help="canonical-JSON disagreements block the archive (fail, default) "
                         "or are reported as warnings (warn); source assertions always block")
    args = ap.parse_args()
    ws = eqd.Workspace()
    repo = ws.repo(args.client)
    src = repo / "include" / args.client
    if not src.is_dir():
        raise SystemExit(f"no headers: {src}")
    out = Path(args.out).resolve() if args.out else ws.archive(args.client)
    out.parent.mkdir(parents=True, exist_ok=True)

    umbrella = ws.reports / "headers" / f"{args.client}.h"
    idx, expected_types = gdt_headers.generate(repo, args.client, umbrella)
    sizes, offsets = assertions(idx)
    docs = canonical_layouts(args.layouts, args.overlays, args.field_edits) if args.layouts else []
    report = {"client": args.client, "headers": sorted(idx.files), "umbrella": str(umbrella),
              "not_parsed": [], "expected_types": expected_types, "archive": str(out),
              "validated": False, "problems": [], "canonical_mode": args.canonical,
              "warnings": []}
    try:
        pyghidra = eqd.start(ws)
    except Exception as e:
        report["phase"] = "startup"
        report["problems"] = [f"{type(e).__name__}: {e}"]
        ws.write_report(f"{args.client}.gdt.json", report)
        raise
    from java.io import File
    from ghidra.app.util.cparser.C import CParserUtils
    from ghidra.program.model.data import FileDataTypeManager, Structure, Enum, TypeDef
    from ghidra.util.task import TaskMonitor

    with tempfile.TemporaryDirectory(prefix="eqd_gdt_") as tmp:
        tmp = Path(tmp)
        files = [str(umbrella)]
        gdt_tmp = tmp / out.name
        dtm = FileDataTypeManager.createFileArchive(File(str(gdt_tmp)), LANGUAGE, COMPILER)
        try:
            results = CParserUtils.parseHeaderFiles(None, files, [str(umbrella.parent)], PARSE_ARGS,
                                                    dtm, TaskMonitor.DUMMY)
            log = str(results.getFormattedParseMessage(None) or "")
            report["parser_log"] = log
            if not results.successful():
                report["problems"] = ["C parser failed"]
                ws.write_report(f"{args.client}.gdt.json", report)
                print(log)
                raise SystemExit("C parser failed; archive not written")
            report["category_moves"], report["uncategorized"] = recategorize(
                dtm, umbrella.name, getattr(idx, "categories", {}))
            sdk = sorted(n for n, h in getattr(idx, "categories", {}).items() if h == "WinDef.h")
            report["sdk_adopted"], sdk_problems = adopt_windows_types(dtm, ws, sdk)
            checked, problems = check(dtm, sizes, offsets)
            problems.extend(sdk_problems)
            report["asserts_checked"] = checked
            report["omitted_types"] = [name for name in expected_types if find_type(dtm, name) is None]
            problems.extend(f"type omitted: {name}" for name in report["omitted_types"])
            canonical, failures, omitted = check_canonical(dtm, docs, args.client)
            report["canonical_checked"] = canonical
            report["canonical_omitted"] = omitted
            blocking, report["warnings"] = canonical_outcome(args.canonical, failures)
            problems.extend(blocking)
            org = dtm.getDataOrganization()
            report["data_organization"] = {"language": LANGUAGE, "compiler": COMPILER,
                                           "pointer": org.getPointerSize(), "long": org.getLongSize(),
                                           "int": org.getIntegerSize(), "big_endian": bool(org.isBigEndian())}
            if org.getPointerSize() != 4 or org.getLongSize() != 4 or org.getIntegerSize() != 4 or org.isBigEndian():
                problems.append("archive does not use 32-bit x86 Windows data organization")
            report["problems"] = problems
            counts = {"structures": 0, "enums": 0, "typedefs": 0}
            for dt in dtm.getAllDataTypes():
                if isinstance(dt, Structure):
                    counts["structures"] += 1
                elif isinstance(dt, Enum):
                    counts["enums"] += 1
                    report.setdefault("enum_values", 0)
                    report["enum_values"] += dt.getCount()
                elif isinstance(dt, TypeDef):
                    counts["typedefs"] += 1
            report["types"] = counts
            if problems:
                ws.write_report(f"{args.client}.gdt.json", report)
                raise SystemExit("layout check failed; archive not written:\n  " + "\n  ".join(problems))
            dtm.save()
        finally:
            dtm.close()
        if out.exists():
            out.unlink()
        shutil.copyfile(gdt_tmp, out)
    report["validated"] = True
    ws.write_report(f"{args.client}.gdt.json", report)
    print(f"wrote {out}: {report['types']}, checked {report['asserts_checked']}, "
          f"canonical {report['canonical_checked']}, {len(report['warnings'])} canonical warnings")


if __name__ == "__main__":
    main()
