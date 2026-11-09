r"""Lay out same-named / similar struct definitions side by side, by offset.

    ..\.venv\Scripts\python.exe structs\unify_diff.py --repo TREE --name REGEX --out DIR [--search] [--client C]
        [--max-copies N]

--name is matched against every name of a definition (typedef alias or tag) with re.fullmatch, so
`Actor_guess|T3dActor_guess` picks exactly those two names; use `.*Actor_guess` for a family, or --search for
re.search semantics. --client limits the files to src/<prefix> of one client (default: all of src/ + include/).

Each copy is laid out with MSVC rules (natural alignment capped by the #pragma pack in effect, /Zp8 default;
arrays, nested structs/unions, bitfields approximately; unknown types are assumed 4 bytes and flagged). The
output (DIR/unify.md, DIR/unify.json) has:

  * a copy table: name, file:line, size, pack, exact?, issues
  * a per-offset table, one column per copy; a cell is `name : type [size]` of the member starting there,
    `(pad name 0x..-0x..)` inside a placeholder byte array, `(in name+0x..)` inside a bigger member,
    `(hole)` in alignment padding, `-- past end` beyond the copy's size
  * flags per row:
      NAME   two copies name the member differently (placeholder names like unknown0x10 / pad / fieldNN do not count)
      WIDTH  two copies give the member different sizes (dword vs byte at the same offset)
      KIND   same size but int vs float vs pointer (a float copy compiles to x87, an int copy to mov)
      SPELL  same size and kind, different type text (u32 / DWORD / unsigned int; pointer to different structs)
      SPLIT  a member starts here in one copy but this offset is inside a larger member of another (dword flags
             at 0x0C vs byte at 0x0F)
      FILL   a copy has a real member here while another has padding or a hole (a copy that simply ends earlier is a partial
             view: shown as `-- past end`, not flagged)
  * size differences between copies
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import cparse as C  # noqa: E402


def collect(idx: C.Index, rx: re.Pattern, search: bool, client_prefix: str | None):
    out = []
    seen = set()
    for d in idx.all_defs():
        if client_prefix and not d.file.startswith(client_prefix):
            continue
        if d.parent is not None and not d.tag:
            continue
        names = d.names
        hit = [n for n in names if (rx.search(n) if search else rx.fullmatch(n))]
        if not hit or d.uid in seen:
            continue
        seen.add(d.uid)
        out.append(d)
    out.sort(key=lambda d: (d.name, d.file, d.line))
    return out


def cell_for(lay: C.Layout, off: int):
    """-> (kind, text, member|None). kind: member | pad | inside | hole | past_end"""
    starting = [m for m in lay.members if m.offset == off and not m.is_pad]
    if starting:
        txt = " / ".join(fmt_member(m) for m in starting)
        return "member", txt, starting[0]
    if off >= lay.size:
        return "past_end", "-- past end", None
    for m in lay.members:
        if m.is_pad and m.offset <= off < m.offset + m.size:
            return "pad", f"(pad {m.path} 0x{m.offset:X}-0x{m.offset + m.size:X})", m
    for m in lay.members:
        if m.offset < off < m.offset + max(m.size, 1) and not m.is_pad:
            return "inside", f"(in {m.path}+0x{off - m.offset:X})", m
    return "hole", "(hole)", None


def fmt_member(m: C.Member) -> str:
    t = m.type
    if m.dims:
        t += "".join(f"[{'?' if x is None else x}]" for x in m.dims)
    s = f"{m.path} : {t}"
    if m.bits:
        s += f" :{m.bits}@bit{m.bit_offset}"
    else:
        s += f" [{m.size}]"
    if m.guessed:
        s += " (size?)"
    return s


def row_flags(cells):
    members = [(i, m) for i, (k, _, m) in enumerate(cells) if k == "member"]
    flags = []
    named = {}
    for i, m in members:
        if not m.unknown_name:
            named.setdefault(m.path, []).append(i)
    if len(named) >= 2:
        flags.append("NAME")
    if len(members) >= 2:
        sizes = {(m.size if not m.bits else -m.bits) for _, m in members}
        if len(sizes) > 1:
            flags.append("WIDTH")
        else:
            kinds = {m.kind for _, m in members}
            if len(kinds) > 1:
                flags.append("KIND")
            elif len({C.norm_type(m.type) for _, m in members}) > 1:
                flags.append("SPELL")
    if members and any(k == "inside" for k, _, _ in cells):
        flags.append("SPLIT")
    if members and any(k in ("pad", "hole") for k, _, _ in cells):
        # only a real fill when the member is not itself a placeholder
        if any(not m.unknown_name for _, m in members):
            flags.append("FILL")
    return flags


def build(idx: C.Index, defs: list):
    copies = []
    for n, d in enumerate(defs):
        lay = idx.layout(d)
        copies.append({"id": n, "name": d.name, "names": d.names, "file": d.file, "line": d.line, "size": lay.size,
                       "align": lay.align, "pack": lay.pack, "exact": lay.exact, "issues": lay.issues,
                       "kind": lay.kind, "members": len([m for m in lay.members if not m.is_pad]),
                       "layout": lay})
    offsets = sorted({m.offset for c in copies for m in c["layout"].members if not m.is_pad})
    rows = []
    for off in offsets:
        cells = [cell_for(c["layout"], off) for c in copies]
        rows.append({"offset": off, "flags": row_flags(cells),
                     "cells": [{"kind": k, "text": t,
                                "member": ({"name": m.path, "type": m.type, "size": m.size, "kind": m.kind,
                                            "bits": m.bits} if (m is not None and k == "member") else None)}
                               for k, t, m in cells]})
    return copies, rows


def short(c) -> str:
    base = c["file"].rsplit("/", 1)[-1]
    return f"{c['name']} @{base}:{c['line']}"


def write_md(path: Path, args, copies, rows):
    L = [f"# unify_diff: `{args.name}`\n", f"tree `{args.repo}`; {len(copies)} copies.\n"]
    L.append("## Copies\n")
    L.append("| # | name | file:line | size | pack | members | exact | issues |")
    L.append("|---|---|---|---|---|---|---|---|")
    for c in copies:
        L.append(f"| {c['id']} | {c['name']} | {c['file']}:{c['line']} | 0x{c['size']:X} | {c['pack']} | {c['members']} | "
                 f"{'yes' if c['exact'] else 'NO'} | {'; '.join(c['issues'])[:120]} |")
    sizes = sorted({c["size"] for c in copies})
    L.append("")
    if len(sizes) > 1:
        L.append("**Size differs across copies:** " + ", ".join(f"0x{s:X}" for s in sizes) + "\n")
    else:
        L.append(f"Sizes agree (0x{sizes[0]:X}).\n" if sizes else "No copies.\n")
    flagged = [r for r in rows if r["flags"]]
    cnt = {}
    for r in rows:
        for f in r["flags"]:
            cnt[f] = cnt.get(f, 0) + 1
    L.append("## Disagreements\n")
    L.append(f"{len(rows)} offsets; flagged rows: {len(flagged)} ({', '.join(f'{k} {v}' for k, v in sorted(cnt.items()))}).\n")
    if flagged:
        L.append("| offset | flags | " + " | ".join(f"#{c['id']}" for c in copies) + " |")
        L.append("|---|---|" + "---|" * len(copies))
        for r in flagged:
            L.append(f"| 0x{r['offset']:X} | {' '.join(r['flags'])} | " + " | ".join(
                x["text"].replace("|", "/") for x in r["cells"]) + " |")
        L.append("")
    L.append("## Per-offset table (all rows)\n")
    L.append("| offset | flags | " + " | ".join(f"#{c['id']} {short(c)}" for c in copies) + " |")
    L.append("|---|---|" + "---|" * len(copies))
    for r in rows:
        L.append(f"| 0x{r['offset']:X} | {' '.join(r['flags'])} | " + " | ".join(
            x["text"].replace("|", "/") for x in r["cells"]) + " |")
    L.append("")
    path.write_text("\n".join(L) + "\n", encoding="utf-8")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--repo", required=True)
    ap.add_argument("--name", required=True, help="regex over struct names (fullmatch; --search for search)")
    ap.add_argument("--out", required=True)
    ap.add_argument("--search", action="store_true")
    ap.add_argument("--client", choices=("trilogy", "eqmain", "eqgfx"))
    ap.add_argument("--max-copies", type=int, default=40)
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    prefix = None
    if a.client == "eqmain":
        prefix = "src/eqmain/"
    elif a.client == "eqgfx":
        prefix = "src/eqgfx/"
    idx = C.load_index(repo)
    rx = re.compile(a.name)
    defs = collect(idx, rx, a.search, prefix)
    if a.client == "trilogy":
        defs = [d for d in defs if not d.file.startswith(("src/eqmain/", "src/eqgfx/"))]
    if not defs:
        print("no struct definition matches", a.name, file=sys.stderr)
        return 1
    if len(defs) > a.max_copies:
        print(f"{len(defs)} matches, keeping the first {a.max_copies} (--max-copies)", file=sys.stderr)
        defs = defs[:a.max_copies]
    copies, rows = build(idx, defs)
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    args = argparse.Namespace(name=a.name, repo=str(repo))
    write_md(out / "unify.md", args, copies, rows)
    js = {"name": a.name, "repo": str(repo),
          "copies": [{k: v for k, v in c.items() if k != "layout"} for c in copies],
          "rows": [{"offset": r["offset"], "flags": r["flags"], "cells": r["cells"]} for r in rows]}
    (out / "unify.json").write_text(json.dumps(js, indent=1), encoding="utf-8")
    nflag = sum(1 for r in rows if r["flags"])
    print(f"{len(copies)} copies, sizes {sorted({c['size'] for c in copies})}, {len(rows)} offsets, {nflag} flagged")
    return 0


if __name__ == "__main__":
    sys.exit(main())
