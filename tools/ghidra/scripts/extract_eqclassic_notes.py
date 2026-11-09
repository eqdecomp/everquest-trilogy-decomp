r"""Extract the client-address notes from the EQClassic source comments.

    extract_eqclassic_notes.py [--out eq-trilogy\names\eqclassic_notes.tsv]

EQClassic (path: paths.local.ini [reference] eqclassic) was reversed against our exact Trilogy eqgame.exe,
so comments such as "sub_4C8FCE", ".text:0047D4E5" or "offset 004C71A3" point into that binary. This
script lists every such note with its comment; it needs no Ghidra. Naming decisions are made by hand in
names\eqgame.eqclassic.csv, using this list as the evidence.
"""
import argparse
import csv
import re
from pathlib import Path

import eqd_ghidra as eqd

# Trilogy eqgame.exe .text (PLAN.md 1.1)
TEXT_START, TEXT_END = 0x401000, 0x533580

TOKENS = [
    ("sub", re.compile(r"\bsub_([0-9A-Fa-f]{6,8})\b")),
    ("text", re.compile(r"\.text:([0-9A-Fa-f]{6,8})\b")),
    ("0x", re.compile(r"\b0x(0{0,2}[0-9A-Fa-f]{6})\b")),
    ("bare", re.compile(r"(?<![\w.:])(00[0-9A-Fa-f]{6})\b")),
]
# "0x" and "bare" tokens are only addresses when the line talks about the client.
CLIENT_WORDS = re.compile(r"eqgame|client|offset|decompil|disasm", re.I)
COMMENT = re.compile(r"//|/\*|\*/|^\s*\*")
SOURCE_SUFFIXES = {".h", ".hpp", ".c", ".cpp"}
MAX_COMMENT = 400


def comment_text(line: str) -> str:
    """The comment part of a source line, without decoration."""
    m = re.search(r"//", line) or re.search(r"/\*", line)
    text = line[m.start():] if m else line
    text = re.sub(r"^\s*(//+\|?|/\*+|\*+/?)", "", text)
    text = re.sub(r"\*/\s*$", "", text)
    return " ".join(text.split())


def surrounding(lines, i):
    """The note's line plus up to two neighbouring pure-comment lines on each side."""
    def pure(j):
        s = lines[j].strip()
        return s.startswith("//") and not re.fullmatch(r"//[-o=|*\s]*", s)
    lo = hi = i
    if not pure(i):  # a code line with a trailing comment stands alone
        return comment_text(lines[i])[:MAX_COMMENT]
    while lo > 0 and i - lo < 2 and pure(lo - 1):
        lo -= 1
    while hi + 1 < len(lines) and hi - i < 2 and pure(hi + 1):
        hi += 1
    text = " / ".join(t for t in (comment_text(lines[j]) for j in range(lo, hi + 1)) if t)
    return text[:MAX_COMMENT]


def extract(root: Path):
    rows = []
    for path in sorted(root.rglob("*")):
        if path.suffix.lower() not in SOURCE_SUFFIXES or not path.is_file():
            continue
        lines = path.read_text(encoding="latin-1").splitlines()
        for i, line in enumerate(lines):
            if not COMMENT.search(line):
                continue
            seen = set()
            for form, rx in TOKENS:
                if form in ("0x", "bare") and not CLIENT_WORDS.search(line):
                    continue
                for m in rx.finditer(line):
                    addr = int(m.group(1), 16)
                    if not TEXT_START <= addr < TEXT_END or addr in seen:
                        continue
                    seen.add(addr)
                    rows.append({
                        "address": f"{addr:08X}",
                        "form": m.group(0),
                        "file": path.relative_to(root).as_posix(),
                        "line": i + 1,
                        "comment": surrounding(lines, i),
                    })
    rows.sort(key=lambda r: (r["address"], r["file"], r["line"]))
    return rows


def main():
    ws = eqd.Workspace()
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", type=Path, default=ws.root / "eq-trilogy" / "names" / "eqclassic_notes.tsv")
    args = ap.parse_args()
    rows = extract(ws.path("reference", "eqclassic"))
    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["address", "form", "file", "line", "comment"], delimiter="\t",
                           lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    print(f"{len(rows)} notes, {len({r['address'] for r in rows})} distinct addresses -> {args.out}")


if __name__ == "__main__":
    main()
