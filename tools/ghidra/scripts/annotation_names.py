r"""Names from the decomp sources: every `// FUNCTION: <CLIENT> 0x...` annotation whose function name differs from
the name the Ghidra export has for that address -> a row for names\<binary>.manual.csv.

    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\annotation_names.py trilogy            report
    .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\annotation_names.py trilogy --write    + update CSV

No Ghidra needed: reads <build>\export\<client>\functions.json and eq-<client>\src\**\*.c/.cpp. The rewrite's
name is the newest hand-checked name, so it wins over the generated lists; `--write` adds a manual row for
each difference, or updates the manual row the annotation contradicts (both are printed). Then run
apply_names.py and the export as usual (ghidra\README.md).

Row: name = the annotation's name without its trailing `_guess` (apply_names.py adds the suffix back for any
confidence but `certain`), confidence `low` when any part is a `_guess`, else `high`, source `manual`, note
`rewrite <src path>: <first comment line under the annotation>`, file = the annotation's
`// ORIGINAL FILE:` when it names a Mac source file. Names are compared ignoring `_guess` suffixes.

Not written (reported as `class conflict`): the method name agrees but the export already has it in another
class (`EQPlayer::SkillUsed` vs a rewrite's stand-in class `EQPlayerMethods_guess::SkillUsed`): the export's
class is the evidence. A flattened export name (`EQSwitch_Reset...`, `GetLanguageName`) is not a class, so
the rewrite's `Class::` form is written.

`--committed` reads the sources as committed (git HEAD), so work in progress in the tree is left out.
"""
from __future__ import annotations

import argparse
import csv
import io
import json
import re
import subprocess
import sys
import tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parents[1] / "unicorn"))

from eqd_unicorn import Workspace, clients  # noqa: E402
from eqd_unicorn import annotations as ann  # noqa: E402

# client -> names\<stem>.*.csv (clients.py)
BINARY = {c.id: c.names for c in clients.CLIENTS.values() if c.names}
FIELDS = ["address", "name", "confidence", "source", "note", "from_address", "file"]
_ORIG_FILE = re.compile(r"^\s*//\s*ORIGINAL FILE:\s*(Source/\S+\.(?:cpp|c|h))")


def bare(name: str) -> str:
    """Name without `_guess` suffixes on any component (classes too: `vlong_guess::vlong_guess`)."""
    return "::".join(re.sub(r"_guess$", "", p) for p in name.split("::"))


def csv_name(name: str) -> str:
    """The CSV form: only the last component's `_guess` goes (apply_names.py re-adds it)."""
    head, _, last = name.rpartition("::")
    last = re.sub(r"_guess$", "", last)
    return f"{head}::{last}" if head else last


def committed_sources(repo: Path, into: Path) -> dict:
    """Copy src/**/*.c/.cpp as of git HEAD into `into` -> {copy: path in the repo}."""
    ls = subprocess.run(["git", "-C", str(repo), "ls-tree", "-r", "-z", "--name-only", "HEAD", "src"],
                        capture_output=True, text=True, check=True).stdout.split("\0")   # -z: paths may hold spaces
    out = {}
    for rel in ls:
        if not rel.lower().endswith((".c", ".cpp")):
            continue
        data = subprocess.run(["git", "-C", str(repo), "show", f"HEAD:{rel}"], capture_output=True,
                              check=True).stdout
        dst = into / rel
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_bytes(data)
        out[dst.resolve()] = repo / rel
    return out


def annotation_rows(repo: Path, client: str, committed: bool = False) -> list[dict]:
    with tempfile.TemporaryDirectory() as tmp:
        if committed:
            where = committed_sources(repo, Path(tmp))
        else:
            where = {p.resolve(): p for p in (repo / "src").rglob("*") if p.suffix.lower() in (".c", ".cpp")}
        return _rows(repo, client, where)


def _rows(repo: Path, client: str, where: dict) -> list[dict]:
    out = []
    for a in ann.scan(sorted(where), client.upper()):
        lines = Path(a.path).read_text(encoding="utf-8", errors="replace").splitlines()
        orig_file, comment = "", ""
        for ln in lines[a.line:a.line + 12]:
            if not ln.strip().startswith("//"):
                break
            m = _ORIG_FILE.match(ln)
            if m:
                orig_file = m.group(1)
            elif not comment and "ORIGINAL FILE" not in ln:
                comment = ln.strip().lstrip("/").strip()
        rel = Path(where[Path(a.path).resolve()]).resolve().relative_to(repo.resolve()).as_posix()
        note = f"rewrite {rel}" + (f": {comment}" if comment else "")
        out.append({"address": f"{a.va:08X}", "name": csv_name(a.name),
                    "confidence": "low" if "_guess" in a.name else "high", "source": "manual",
                    "note": note[:300], "from_address": "", "file": orig_file, "_full": a.name})
    return out


def class_conflict(export_name: str, ann_name: str) -> bool:
    """Same method name, but the export has it in a different class already."""
    e_head, _, e_last = bare(export_name).rpartition("::")
    a_head, _, a_last = bare(ann_name).rpartition("::")
    return bool(e_head) and e_last == a_last and e_head != a_head


def diff(repo: Path, ws: Workspace, client: str, committed: bool = False):
    """-> ([(row, export name, manual row or None)] to write, [(row, export name)] class conflicts)."""
    exp = json.loads((ws.build / "export" / client / "functions.json").read_text(encoding="utf-8"))["functions"]
    names = {int(f["address"], 16): f["name"] for f in exp}
    manual = read_csv(repo / "names" / f"{BINARY[client]}.manual.csv")
    by_addr = {int(r["address"], 16): r for r in manual}
    out, conflicts = [], []
    for row in annotation_rows(repo, client, committed):
        va = int(row["address"], 16)
        cur = names.get(va, "")
        m = by_addr.get(va)
        if bare(cur) == bare(row["_full"]) and (m is None or bare(m["name"]) == bare(row["_full"])):
            continue
        if m is not None and bare(m["name"]) == bare(row["_full"]):
            continue                                # manual row already says so; the export is behind
        if class_conflict(m["name"] if m else cur, row["_full"]):
            conflicts.append((row, m["name"] if m else cur))
            continue
        e_last, a_last = bare(cur).rpartition("::")[2], bare(row["_full"]).rpartition("::")[2]
        if e_last == a_last and not cur.endswith("_guess") and not cur.startswith("FUN_"):
            row["confidence"] = "certain"           # only the class is new: keep a certain name certain
        out.append((row, cur, m))
    return out, conflicts


def read_csv(path: Path) -> list[dict]:
    if not path.is_file():
        return []
    with open(path, newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def write_csv(path: Path, rows):
    """The manual list's style: the note always in quotes, other fields only when they need it."""
    def cell(k, v):
        v = v or ""
        if not v:
            return ""
        if k == "note" and v:
            return '"' + v.replace('"', '""') + '"'
        buf = io.StringIO()
        csv.writer(buf, lineterminator="").writerow([v])
        return buf.getvalue()
    lines = [",".join(FIELDS)] + [",".join(cell(k, r.get(k)) for k in FIELDS) for r in rows]
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("client", choices=sorted(BINARY))
    ap.add_argument("--repo", help="decomp repo (default <workspace>\\eq-<client>)")
    ap.add_argument("--write", action="store_true", help="add/update the rows in names\\<binary>.manual.csv")
    ap.add_argument("--committed", action="store_true", help="read the sources as of git HEAD")
    a = ap.parse_args(argv)
    ws = Workspace()
    repo = Path(a.repo) if a.repo else ws.repo(a.client)
    rows, conflicts = diff(repo, ws, a.client, a.committed)
    for row, cur in conflicts:
        print(f"{row['address']}  {cur:<45} -- {row['_full']}  (class conflict, not written)")
    for row, cur, m in rows:
        extra = f"  (replaces manual row {m['name']})" if m else ""
        print(f"{row['address']}  {cur or '?':<45} -> {row['_full']}{extra}")
    print(f"{len(rows)} annotation names differ from the export")
    if a.write and rows:
        path = repo / "names" / f"{BINARY[a.client]}.manual.csv"
        manual = read_csv(path)
        idx = {int(r["address"], 16): i for i, r in enumerate(manual)}
        for row, _, _ in rows:
            va = int(row["address"], 16)
            if va in idx:
                manual[idx[va]] = row
            else:
                manual.append(row)
        write_csv(path, manual)
        print(f"wrote {path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
