r"""S5: replace TU-local copies of a struct with the generated layout header (STRUCT-LAYOUT phase, plan section 2).

    ..\.venv\Scripts\python.exe structs\s5_unify.py --repo TREE --layout L.json --census accesses.jsonl
        [--copies FILE:LINE ...] [--apply] [--report OUT.json]

For every copy in the layout's `copy_maps` (keys "src/x.c:LINE ...", or --copies), in one TU:

  1. the local definition at LINE (`typedef struct X {...} X;`, or `struct X {...};` plus a forward
     `typedef struct X X;`) is replaced by `#include "<header without include/>"`; the header's `aliases`
     typedefs keep the local name X working (`struct X` tag uses become `X`);
  2. every member access of the old view (census rows form=field on a base of struct X, same line) is mapped by
     OFFSET and TYPE onto the generated layout (the copy_maps names are checked against it):
       same canonical type and shape at that offset      -> the new member (or union role)   x->flags -> x->flagsDword_guess
       an element of a new array (same element type)     -> member[i]                        x->worldZ -> x->position_guess[2]
       a new member starts there but the type differs    -> cast at use, lvalue-safe         (*(int*)&x->activeAI_guess)
                                                            arrays / struct views             ((unsigned long*)x->position_guess)
       nothing starts there (padding of the new layout)  -> (*(T*)((char*)x + 0xNN))
     Members inside struct-typed union roles / overlays / by-value members count by their dotted path
     (x->flags -> x->flagBytes_guess.flags). Volatility must match too: a volatile old member maps onto a volatile
     new one, else it gets a cast at use that keeps the volatile access ((*(volatile u8*)&x->flagBytes_guess.flags)).
     A line whose `->member` / `.member` count differs from the census rows for it is refused (ambiguous), and so is
     the whole TU (nothing is written for it).
A copy whose map value says "NOT MAPPED" is skipped. Dry run prints the plan; --apply writes (newline='').
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
from gen_layout_h import load_layout, _base_start  # noqa: E402
from propagate import Types, read_raw, write_raw, load_census, strip_parens, norm_ws  # noqa: E402


class UnifyError(Exception):
    pass


def header_closure(repo: Path, rel: str) -> list:
    """`rel` and the project headers it includes (quoted includes that resolve next to the including file or under
    include/), transitively; system headers are skipped. Included files come first."""
    seen, order = set(), []

    def visit(r):
        if r in seen:
            return
        seen.add(r)
        fi = C.parse_file(r, read_raw(repo / r))
        d = r.rsplit("/", 1)[0] if "/" in r else ""
        for inc in fi.includes:
            for cand in ((d + "/" + inc) if d else inc, "include/" + inc):
                if (repo / cand).is_file():
                    visit(Path(cand).as_posix())
                    break
        order.append(fi)
    visit(rel)
    return order


def new_layout(repo: Path, doc):
    """The generated layout as the compiler sees it: the header plus its project includes, so a by-value member of
    an included type (gfx_types.h Vector3) has its size and every later offset is right."""
    idx = C.Index()
    for fi in header_closure(repo, doc.header):
        idx.add(fi)
    idx.resolve_aliases()
    d = [x for x in idx.by_name.get(doc.family, []) if x.parent is None and x.file == doc.header][0]
    return idx, idx.layout(d)


def old_def(fi: C.FileInfo, line: int):
    cands = [d for d in fi.structs if d.parent is None and abs(d.line - line) <= 2]
    if not cands:
        raise UnifyError(f"{fi.rel}:{line}: no struct definition there")
    return cands[0]


def def_span(text: str, masked: str, d) -> tuple[int, int]:
    """Text span of the definition (typedef struct X {...} X; / struct X {...};) incl. a forward typedef."""
    starts = [0] + [i + 1 for i, c in enumerate(text) if c == "\n"]
    brace = masked.find("{", starts[d.line - 1])
    m = re.search(r"(?:\btypedef\s+)?\bstruct(?:\s+[A-Za-z_]\w*)?\s*$", masked[:brace])
    if not m:
        raise UnifyError(f"{d.file}:{d.line}: definition start not found")
    a = m.start()
    rb = C.match_close(masked, brace)
    b = masked.find(";", rb) + 1
    nl = masked.find("\n", b)
    if nl >= 0 and not masked[b:nl].strip():
        b = nl + 1
    if d.tag:
        for mf in re.finditer(r"^[ \t]*typedef\s+struct\s+" + re.escape(d.tag) + r"\s+\w+\s*;[^\n]*\n", masked, re.M):
            if mf.end() <= a and not masked[mf.end():a].strip():
                a = mf.start()
    ls = masked.rfind("\n", 0, a) + 1
    if not masked[ls:a].strip():
        a = ls
    return a, b


class OldMember:
    def __init__(self, name, offset, type_text, dims, is_struct, volatile=False):
        self.name, self.offset, self.type, self.dims, self.is_struct = name, offset, type_text, dims, is_struct
        self.volatile = volatile

    @property
    def ctype(self) -> str:
        """The type a cast at use spells (a volatile member keeps its volatile access)."""
        return f"volatile {self.type}" if self.volatile else self.type


def old_members(idx: C.Index, lay, rel) -> list:
    """Top-level members of the old view (anonymous unions flattened), from Layout.direct."""
    out = []
    vol = {m.path for m in list(lay.members) + list(lay.aggregates) if m.volatile}
    for name, (off, tt, dims) in lay.direct.items():
        base, ptr = C.split_type(tt)
        sd = idx.find_struct(base, rel) if ptr == 0 else None
        out.append(OldMember(name, off, " ".join(tt.split()), [x for x in (dims or []) if x is not None],
                             sd is not None, name in vol))
    return out


INT_EQUIV = {"long": "int", "unsigned long": "unsigned int"}


def ceq(types: Types, t: str) -> str:
    """Codegen-equivalent spelling: long == int, unsigned long == unsigned int (32-bit, same signedness)."""
    c = types.canon(t)
    b, p = C.split_type(c)
    return C.fmt_type(INT_EQUIV.get(b, b), p)


def same_role(types, old_t, new_t, c_file: bool) -> bool:
    a, b = ceq(types, old_t), ceq(types, new_t)
    if a == b:
        return True
    # C: void* converts to and from any object pointer implicitly; loads/stores/compares are identical
    return c_file and a == "void*" and b.endswith("*")


def plan_member(om, new_lay, types: Types, c_file: bool = True):
    """Returns (kind, value): name | elem | cast | arrcast | raw."""
    o = om.offset
    odims = om.dims
    best = []
    new_vol = {m.path for m in list(new_lay.members) + list(new_lay.aggregates) if m.volatile}
    # direct members first: by-value struct views and arrays (overlay roles) are not in the flattened list
    for name, (noff, ntt, ndims_raw) in new_lay.direct.items():
        ndims = [x for x in (ndims_raw or []) if x is not None]
        if noff == o and ndims == odims and types.canon(ntt) == types.canon(om.type) \
                and (name in new_vol) == om.volatile:
            return "name", name
    for nm in new_lay.members:
        if nm.bits is not None or nm.is_pad:
            continue
        ndims = [x for x in nm.dims if x is not None]
        # volatility is part of the role: `volatile u8 flags` maps only onto a volatile member (struct-typed union
        # roles carry it: x->flags -> x->flagsBytes_guess.flags), else it becomes a volatile cast at use
        same = same_role(types, om.type, nm.type, c_file) and not om.is_struct and nm.volatile == om.volatile
        if nm.offset == o and same and ndims == odims:
            best.insert(0, ("name", nm.path))
            continue
        if not odims and len(ndims) == 1 and nm.offset <= o < nm.offset + nm.size \
                and same and nm.elem_size and (o - nm.offset) % nm.elem_size == 0:
            best.append(("elem", f"{nm.path}[{(o - nm.offset) // nm.elem_size}]"))
        if len(odims) == 1 and len(ndims) == 1 and nm.offset == o and same and ndims[0] >= odims[0]:
            best.append(("name", nm.path))
    if best:
        # an exact type match beats an equivalent one (union roles: int role over unsigned primary)
        exact = [b for b in best if b[0] == "name" and ceq(types, member_type(new_lay, b[1])) == ceq(types, om.type)
                 and types.canon(member_type(new_lay, b[1])) == types.canon(om.type)]
        return exact[0] if exact else best[0]
    starts = [nm for nm in new_lay.members if nm.offset == o and not nm.is_pad and nm.bits is None]
    if starts:
        nm = starts[0]
        isarr = bool([x for x in nm.dims if x is not None])
        return ("arrcast" if odims else "cast"), (om.ctype, nm.path, isarr)
    return "raw", (om.ctype, o, bool(odims))


def member_type(lay, path):
    for m in lay.members:
        if m.path == path:
            return m.type
    return ""


def unify_tu(repo: Path, rel: str, line: int, doc, rows: list, types: Types, new_lay, copy_map: dict, log,
             allow_casts=False):
    p = repo / rel
    text = read_raw(p)
    masked = C.mask(text)
    fi = C.parse_file(rel, text)
    d = old_def(fi, line)
    names = set(d.names)
    idx = C.Index()
    idx.add(fi)
    idx.resolve_aliases()
    dd = [x for x in idx.by_name.get(d.name, []) if x.line == d.line][0]
    old = idx.layout(dd)
    plan = {}
    for om in old_members(idx, old, rel):
        kind, val = plan_member(om, new_lay, types, rel.endswith(".c"))
        plan[om.name] = (kind, val, om)
    # check against copy_maps simple names
    mism = []
    for k, v in copy_map.items():
        if k in plan and plan[k][0] in ("name", "elem"):
            want = v.split(" ")[0]
            if re.fullmatch(r"[A-Za-z_]\w*(\[\d+\])?", want) and want != plan[k][1]:
                mism.append(f"{k}: map says {want}, offset/type gives {plan[k][1]}")
    # sites
    starts = [0] + [i + 1 for i, c in enumerate(text) if c == "\n"]
    byline = collections.defaultdict(list)
    for r in rows:
        if r.get("form") == "field" and r.get("base_struct") in names and r["file"] == rel:
            byline[r["line"]].append(r)
    edits = []
    refused = []
    for ln, rs in sorted(byline.items()):
        lo, hi = starts[ln - 1], (starts[ln] if ln < len(starts) else len(text))
        want = collections.Counter(r["member"].split(".")[0] for r in rs)
        for mem, n in want.items():
            occ = [m for m in re.finditer(r"(->|\.)\s*" + re.escape(mem) + r"\b", masked[lo:hi])]
            occ = [m for m in occ if not (m.group(1) == "." and lo + m.start() > 0 and masked[lo + m.start() - 1].isdigit())]
            if len(occ) != n:
                bases = {norm_ws(r.get("base_expr") or "") for r in rs if r["member"].split(".")[0] == mem}
                occ = [m for m in occ if norm_ws(text[_base_start(masked, lo + m.end() - len(mem)):lo + m.start()])
                       in bases]
            if len(occ) != n:
                refused.append(f"{rel}:{ln}: {len(occ)} `{mem}` accesses on the line vs {n} census rows")
                continue
            if mem not in plan:
                refused.append(f"{rel}:{ln}: member {mem} not in the old layout")
                continue
            kind, val, om = plan[mem]
            if kind not in ("name", "elem") and not allow_casts:
                refused.append(f"{rel}:{ln}: {mem} ({om.type} at 0x{om.offset:X}) has no same-typed member: needs a cast "
                               f"at use ({kind}), which raises the source_checks cast count (--allow-casts)")
                continue
            for m in occ:
                a = lo + m.start()
                b = lo + m.end()
                arrow = m.group(1)
                if kind in ("name", "elem"):
                    if val != mem:
                        edits.append((a, b, f"{arrow}{val}", ln, mem, val))
                    continue
                bs = _base_start(masked, b - len(mem))
                base = text[bs:a]
                if arrow == ".":
                    addr = f"&{base}"
                    basep = f"(&{base})"
                else:
                    basep = base
                if kind == "cast":
                    t, nmpath, isarr = val
                    new = f"(*({t}*)&{basep}->{nmpath})" if not isarr else f"(*({t}*){basep}->{nmpath})"
                elif kind == "arrcast":
                    t, nmpath, isarr = val
                    new = f"(({t}*){basep}->{nmpath})" if isarr else f"(({t}*)&{basep}->{nmpath})"
                else:
                    t, o, isarr = val
                    new = (f"(({t}*)((char*){basep} + 0x{o:X}))" if isarr else f"(*({t}*)((char*){basep} + 0x{o:X}))")
                edits.append((bs, b, new, ln, mem, new))
    # accesses through a cast to the old type, `((Actor_guess *)x)->liglist` (the census does not type those bases)
    done = {(e[0], e[1]) for e in edits}
    rx = re.compile(r"\(\s*\(\s*(?:struct\s+)?(" + "|".join(map(re.escape, sorted(names))) + r")\s*\*\s*\)")
    for mc in rx.finditer(masked):
        close = C.match_close(masked, mc.start())
        ma = re.match(r"\s*->\s*([A-Za-z_]\w*)\b", masked[close + 1:])
        if not ma:
            continue
        mem = ma.group(1)
        a0 = close + 1 + ma.start(1) - 0
        arrow_start = close + 1 + (ma.group(0).index("->"))
        b0 = close + 1 + ma.end(1)
        ln = C.LineIndex(text).line(a0)
        if any(x[0] <= arrow_start < x[1] for x in done):
            continue
        if mem not in plan:
            refused.append(f"{rel}:{ln}: member {mem} (cast base) not in the old layout")
            continue
        kind, val, om = plan[mem]
        if kind not in ("name", "elem"):
            refused.append(f"{rel}:{ln}: {mem} via a cast base needs a cast at use ({kind})")
            continue
        if val != mem:
            edits.append((arrow_start, b0, f"->{val}", ln, mem, val))
    if refused:
        raise UnifyError("; ".join(refused[:6]))
    a, b = def_span(text, masked, d)
    # TU-local definitions of types the header now provides (identical layout) are removed with it
    removed = []
    for st in doc.types:
        loc = [x for x in fi.structs if x.parent is None and st.name in x.names and x is not d]
        for x in loc:
            xl = idx.layout([y for y in idx.by_name.get(x.name, []) if y.line == x.line][0])
            want = {f.name: f.offset for f in st.fields}
            got = {k: v[0] for k, v in xl.direct.items()}
            if got != want or (st.size is not None and xl.size != st.size):
                raise UnifyError(f"{rel}:{x.line}: local {st.name} differs from the header's ({got} vs {want})")
            ta, tb = def_span(text, masked, x)
            removed.append((ta, tb, x.line, st.name))
    inc = doc.header[len("include/"):] if doc.header.startswith("include/") else doc.header
    nl = "\r\n" if "\r\n" in text else "\n"
    comment = (f"/* {d.name}: the actor layout is {doc.family} (generated: {inc}); the old local view's members map "
               f"by offset. */{nl}")
    first = min([r_[0] for r_ in removed] + [a])
    if first < a:
        # a removed header type is used before the actor definition: the include goes where the first one was
        for ta, tb, tl, tn in removed:
            txt = (f"/* {tn}: now in {inc}. */{nl}" + (f'#include "{inc}"{nl}' if ta == first else ""))
            edits.append((ta, tb, txt, tl, "<type>", tn))
        edits.append((a, b, comment, d.line, "<def>", "<include>"))
    else:
        for ta, tb, tl, tn in removed:
            edits.append((ta, tb, f"/* {tn}: now in {inc}. */{nl}", tl, "<type>", tn))
        edits.append((a, b, comment + f'#include "{inc}"{nl}', d.line, "<def>", "<include>"))
    # tag uses -> typedef names
    for tag in [d.tag] if d.tag else []:
        for m in re.finditer(r"\bstruct\s+" + re.escape(tag) + r"\b", masked):
            if a <= m.start() < b:
                continue
            edits.append((m.start(), m.end(), tag, C.LineIndex(text).line(m.start()), "<tag>", tag))
    edits.sort(key=lambda e: e[0])
    for e1, e2 in zip(edits, edits[1:]):
        if e2[0] < e1[1]:
            raise UnifyError(f"{rel}:{e2[3]}: overlapping edits ({e1[4]} / {e2[4]})")
    for e in edits:
        log.append({"file": rel, "line": e[3], "member": e[4], "old": text[e[0]:e[1]] if e[4] != "<def>" else
                    f"<definition of {d.name}, {len(old.direct)} members>", "new": e[2] if e[4] != "<def>" else
                    f'#include "{inc}"'})
    out = text
    for e in sorted(edits, key=lambda e: e[0], reverse=True):
        out = out[:e[0]] + e[2] + out[e[1]:]
    return out, mism, len([e for e in edits if e[4] not in ("<def>", "<tag>", "<type>")])


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--repo", required=True)
    ap.add_argument("--layout", required=True)
    ap.add_argument("--census", required=True)
    ap.add_argument("--copies", nargs="*")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--report")
    ap.add_argument("--allow-casts", action="store_true", help="write cast-at-use forms for members without a same-typed target")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    doc = load_layout(a.layout)
    idx_new, new_lay = new_layout(repo, doc)
    types = Types(C.load_index(repo, subdirs=("include",)))
    rows = load_census(a.census)
    maps = doc.raw.get("copy_maps") or {}
    todo = []
    for k, v in maps.items():
        m = re.match(r"([^:\s]+):(\d+)", k)
        if not m:
            continue
        if isinstance(v, str) and "NOT MAPPED" in v or isinstance(v, dict) and any("NOT MAPPED" in str(x) for x in v.values()):
            print(f"skip {k}: NOT MAPPED")
            continue
        todo.append((m.group(1), int(m.group(2)), v if isinstance(v, dict) else {}))
    if a.copies:
        want = {c for c in a.copies}
        todo = [t for t in todo if f"{t[0]}:{t[1]}" in want or t[0] in want]
    report = []
    rc = 0
    for rel, line, cmap in todo:
        log: list = []
        try:
            out, mism, n = unify_tu(repo, rel, line, doc, rows, types, new_lay, cmap, log, a.allow_casts)
        except UnifyError as ex:
            print(f"REFUSE {rel}:{line}: {ex}")
            report.append({"file": rel, "line": line, "status": "refused", "reason": str(ex)})
            rc = 1
            continue
        print(f"{rel}:{line}: {n} member access(es) rewritten" + (f"; copy_map disagreements: {mism}" if mism else ""))
        for e in log:
            if e["member"] not in ("<def>",):
                print(f"   {e['line']}: {' '.join(e['old'].split())}  ->  {e['new']}")
        report.append({"file": rel, "line": line, "status": "rewrite", "sites": n, "edits": log, "map_mismatch": mism})
        if a.apply:
            write_raw(repo / rel, out)
    if a.report:
        Path(a.report).write_text(json.dumps(report, indent=1), encoding="utf-8")
    return rc


if __name__ == "__main__":
    sys.exit(main())
