r"""Rename struct members across a tree (STRUCT-LAYOUT phase: field-name passes after S2/S4).

    ..\.venv\Scripts\python.exe structs\rename_members.py --repo TREE --map rename_map.json [--client trilogy]
        [--apply] [--report OUT.json]

The map is {"Struct": {"old": "new", ...}, ...} (a "_comment"/"notes" key and non-dict values are ignored).

For every (struct, old -> new):
  * member accesses `x->old` / `x.old` in src/ and include/: when no other struct (in any client file) has a member
    called `old`, every member access spelled `->old` / `.old` is renamed (the name is unique, so the base type
    does not matter); when other structs share the name, only accesses whose base the census types as the struct
    (form=field rows, --fields all, matched per line and base text; derived classes and typedef aliases count) are
    renamed and the rest of that line is refused (reported, left as is);
  * `offsetof(Struct, old)` (and aliases) anywhere;
  * the member's own declaration and C_ASSERTs when the struct is defined in a hand-kept header (player.h,
    character.h); generated headers are regenerated from the layout JSON instead (gen_layout_h.py), so a generated
    header that still declares `old` is reported, not edited;
  * macro bodies: a unique name is renamed in #define bodies too; a shared name inside a macro body is refused.
A rename whose new name already exists as a member of the struct is refused. Files keep their line endings.
"""
from __future__ import annotations

import argparse
import collections
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import cparse as C  # noqa: E402
from gen_layout_h import GEN_MARKER  # noqa: E402
from propagate import read_raw, write_raw, norm_ws  # noqa: E402


def load_map(path) -> dict:
    d = json.loads(Path(path).read_text(encoding="utf-8"))
    out = {}
    for k, v in d.items():
        if isinstance(v, dict) and not k.startswith("_"):
            out[k] = {o: n for o, n in v.items() if isinstance(n, str) and o != n}
    return out


def struct_names(idx: C.Index, name: str) -> set:
    """The struct, its typedef aliases and (transitively) derived classes."""
    d = idx.find_struct(name)
    if d is None:
        return {name}
    names = set(d.names)
    for k, v in idx.typedefs.items():
        b, p = C.split_type(v)
        if p == 0 and b in names:
            names.add(k)
    changed = True
    while changed:
        changed = False
        for sd in idx.all_defs():
            if any(b in names for b in sd.bases) and not set(sd.names) <= names:
                names |= set(sd.names)
                changed = True
    return names


def member_owners(idx: C.Index) -> dict:
    """member name -> set of struct names that declare it (direct members, all files)."""
    own = collections.defaultdict(set)
    for d in idx.all_defs():
        for rm in d.raw:
            if rm.name:
                own[rm.name].add(d.name)
            if rm.inline is not None and rm.name is None:
                for sub in rm.inline.raw:
                    if sub.name:
                        own[sub.name].add(d.name)
    return own


class Renamer:
    def __init__(self, repo: Path, mapping: dict, client: str = "trilogy", census_rows=None):
        self.repo = repo
        self.map = mapping
        self.idx = C.load_index(repo, subdirs=("include", "src"))
        self.owners = member_owners(self.idx)
        self.client = client
        self._rows = census_rows
        self.edits = collections.defaultdict(list)     # rel -> [(a, b, new, what)]
        self.report = []

    def rows(self):
        if self._rows is None:
            import census as CE
            cen = CE.ClientCensus(self.repo, self.client)
            cen.load()
            cen.run("all")
            self._rows = cen.records
        return self._rows

    def files(self):
        for sd in ("src", "include"):
            base = self.repo / sd
            if base.is_dir():
                for p in sorted(base.rglob("*")):
                    if p.is_file() and p.suffix.lower() in (".c", ".cpp", ".h", ".hpp"):
                        yield p.relative_to(self.repo).as_posix()

    def run(self):
        texts = {}
        for rel in self.files():
            t = read_raw(self.repo / rel)
            texts[rel] = (t, C.mask(t))
        for sname, ren in sorted(self.map.items()):
            d = self.idx.find_struct(sname)
            if d is None:
                self.report.append({"struct": sname, "status": "refused", "reason": "struct not found"})
                continue
            names = struct_names(self.idx, sname)
            lay = self.idx.layout(d)
            have = set(lay.direct)
            generated = GEN_MARKER in (texts.get(d.file, ("", ""))[0])
            for old, new in sorted(ren.items()):
                if new in have and new not in ren:
                    self.report.append({"struct": sname, "old": old, "new": new, "status": "refused",
                                        "reason": f"{new} already a member of {sname}"})
                    continue
                others = self.owners.get(old, set()) - names
                unique = not others
                rows = None if unique else [r for r in self.rows() if r.get("form") == "field" and
                                            (r.get("member") or "").split(".")[0] == old]
                n_ok = n_bad = 0
                bad_locs = []
                for rel, (t, mk) in texts.items():
                    if old not in t:
                        continue
                    li = None
                    for m in re.finditer(r"(->|\.)(\s*)" + re.escape(old) + r"\b", mk):
                        if m.group(1) == "." and m.start() > 0 and mk[m.start() - 1].isdigit():
                            continue
                        a = m.end() - len(old)
                        ln_start = mk.rfind("\n", 0, a) + 1
                        in_define = re.match(r"[ \t]*#[ \t]*define\b", mk[ln_start:a]) is not None or \
                            self._continued_define(mk, ln_start)
                        if unique:
                            self.edits[rel].append((a, a + len(old), new, f"{sname}.{old}"))
                            n_ok += 1
                            continue
                        if in_define:
                            n_bad += 1
                            bad_locs.append(f"{rel}: shared member in a #define")
                            continue
                        li = li or C.LineIndex(t)
                        line = li.line(a)
                        base = self._base_text(t, mk, m.start())
                        ok = [r for r in rows if r["file"] == rel and r["line"] == line and
                              r.get("base_struct") in names and norm_ws(r.get("base_expr") or "") == norm_ws(base)]
                        if ok:
                            self.edits[rel].append((a, a + len(old), new, f"{sname}.{old}"))
                            n_ok += 1
                        else:
                            n_bad += 1
                            bad_locs.append(f"{rel}:{line}")
                    # offsetof(Struct, old)
                    for m in re.finditer(r"\boffsetof\s*\(\s*(?:struct\s+|class\s+)?(\w+)\s*,\s*(" + re.escape(old) +
                                         r")\b", mk):
                        if m.group(1) in names:
                            self.edits[rel].append((m.start(2), m.end(2), new, f"offsetof {sname}.{old}"))
                            n_ok += 1
                # the declaration in a hand-kept header
                if not generated:
                    t, mk = texts[d.file]
                    a0, b0 = self._body_span(mk, d)
                    for m in re.finditer(r"\b" + re.escape(old) + r"\b", mk[a0:b0]):
                        pos = a0 + m.start()
                        prev = mk[max(0, pos - 3):pos]
                        if re.search(r"(->|\.)\s*$", prev):
                            continue
                        self.edits[d.file].append((pos, pos + len(old), new, f"decl {sname}.{old}"))
                        n_ok += 1
                elif re.search(r"\b" + re.escape(old) + r"\s*(\[|;)", texts[d.file][1]):
                    bad_locs.append(f"{d.file}: generated header still declares {old}: regenerate from the JSON")
                self.report.append({"struct": sname, "old": old, "new": new, "unique": unique,
                                    "shared_with": sorted(others)[:6], "renamed": n_ok, "refused": n_bad,
                                    "refused_at": bad_locs[:20], "status": "ok" if not n_bad else "partial"})
        # overlapping edits (two maps hitting one token) are an error
        for rel in list(self.edits):
            uniq = {}
            for e in self.edits[rel]:
                uniq.setdefault((e[0], e[1], e[2]), e)
            self.edits[rel] = ee = sorted(uniq.values())
            for x, y in zip(ee, ee[1:]):
                if y[0] < x[1]:
                    raise SystemExit(f"rename_members: overlapping edits in {rel}: {x[3]} / {y[3]}")
        return self.report

    @staticmethod
    def _continued_define(mk, ln_start):
        prev = mk.rfind("\n", 0, max(0, ln_start - 1))
        line = mk[prev + 1:ln_start - 1] if ln_start else ""
        return line.rstrip("\r").rstrip().endswith("\\")

    @staticmethod
    def _body_span(mk, d):
        starts = [0] + [i + 1 for i, c in enumerate(mk) if c == "\n"]
        brace = mk.find("{", starts[d.line - 1])
        close = C.match_close(mk, brace)
        # the C_ASSERT block after the struct belongs to it too
        end = close
        rest = mk[close:]
        m = re.match(r"[^;]*;(?:\s*C_ASSERT\s*\([^;]*\)\s*;)*", rest)
        if m:
            end = close + m.end()
        return brace, end

    @staticmethod
    def _base_text(t, mk, arrow_pos):
        from gen_layout_h import _base_start
        k = arrow_pos
        bs = _base_start(mk, k + (2 if mk[k:k + 2] == "->" else 1))
        return t[bs:arrow_pos]

    def apply(self):
        out = {}
        for rel, ee in self.edits.items():
            t = read_raw(self.repo / rel)
            for a, b, new, _ in sorted(ee, reverse=True):
                t = t[:a] + new + t[b:]
            write_raw(self.repo / rel, t)
            out[rel] = len(ee)
        return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--repo", required=True)
    ap.add_argument("--map", required=True)
    ap.add_argument("--client", default="trilogy")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--report")
    a = ap.parse_args(argv)
    rn = Renamer(Path(a.repo).resolve(), load_map(a.map), a.client)
    rep = rn.run()
    for r in rep:
        if r.get("old"):
            print(f"{r['status']:<8} {r['struct']}.{r['old']} -> {r['new']}: {r.get('renamed', 0)} renamed, "
                  f"{r.get('refused', 0)} refused" + (f" (shared with {', '.join(r['shared_with'])})"
                                                      if r.get("shared_with") else "") +
                  ("".join(f"\n           {x}" for x in r.get("refused_at", [])) if r.get("refused_at") else "")
                  + (f" {r['reason']}" if r.get("reason") else ""))
        else:
            print(f"{r['status']:<8} {r['struct']}: {r.get('reason')}")
    if a.report:
        Path(a.report).write_text(json.dumps(rep, indent=1), encoding="utf-8")
    if a.apply:
        ch = rn.apply()
        print(f"applied: {sum(ch.values())} edit(s) in {len(ch)} file(s)")
    return 1 if any(r["status"] == "refused" for r in rep) else 0


if __name__ == "__main__":
    sys.exit(main())
