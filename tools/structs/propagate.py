r"""S4a typed-base propagation (STRUCT-LAYOUT phase; struct-layout plan sections 2 and 4).

    ..\.venv\Scripts\python.exe structs\propagate.py --repo TREE --layout L.json [...] --census accesses.jsonl
        [--name-sensitive FILE|DIR ...] [--files REL ...] [--apply] [--report OUT.json] [--md OUT.md] [-q]

Rewrites raw-offset accesses whose base is ALREADY a pointer to the layout's struct into member accesses:

    *(T*)((char*)p->actor_info + 0x2A4)    ->  p->actor_info->follow_target_guess
    *(T*)((char*)p->actor_info + 0x264)    ->  p->actor_info->attach_a_guess[1]      (array element)
    (T*)((char*)p->actor_info + 0x2A4)     ->  &p->actor_info->follow_target_guess    (address, T* == &field type)

Candidates come from the census (structs/census.py accesses.jsonl: forms deref_cast and ptr_add whose
`base_struct` is the layout's family). Member offsets and types come from the struct as the tree defines it now
(cparse layout of the header, so run gen_layout_h.py first); the JSON gives the family, its aliases and
`keep_cast_sites`. The census must be of the same tree state (line numbers); a site whose text cannot be found on
its census line is refused as `census_stale`.

A site is rewritten only when ALL hold, else it is refused with one reason:
  base_not_typed      the base is not declared as Family* (an inferred `char* info = (char*)x->actor_info`, a cast)
  scaled_offset       the sum is done in the struct pointer type (no byte/integer cast): offset was scaled
  dynamic_offset      a non-constant term in the offset (`entryNumber * 4`)
  base_arith          top-level + or - inside the base expression
  no_member           no member starts at the offset (padding, middle of a member, beyond the struct)
  type_differs        access type != member type (typedefs resolved; signedness and width exact; char != signed char)
  volatile            the access type or cast is volatile, or the only member of that type there is volatile
                      (a `volatile u8 flags` inside a struct-typed union role: a plain access would turn volatile)
  addr_untyped        address form without an outer `(T*)` cast (the char* sum is used as such)
  addr_type_differs   address form whose `(T*)` is not a pointer to the member type
  naked               the function is __declspec(naked)
  name_sensitive      the function is listed in a name_sensitive.json (or the built-in list: 0x004F0290)
  header_sensitive    0x00428686, or a unit_10003700 TU (code moves when headers change)
  keep_cast           listed in the layout's keep_cast_sites
  asm                 the site is on a line inside an __asm block
  nested              the site contains another rewritten site (rerun after applying)
  census_stale        the census line no longer holds the access
  via_macro           the access comes from a macro whose body depends on a parameter (or spans lines)
  role_not_fixed      (S4c) the base is a parameter or an untyped local the census did not type
  abs_base            (S4b) the global is read through a raw address (`*(u32*)0x006E4E44`): the accessor would
                      turn the absolute operand into a symbol relocation
  partial_function    (--whole-function-globals) another census use of the same global in the function stays raw
  gate_rollback       the function is listed in --skip-funcs (its rewrites failed the T0 gate: CSE / register
                      allocation changed, e.g. a call's `this` and an argument loading one global under two types)

Modes besides S4a (typed base):
  S4b global accessor   rows whose base is the value of a layout `globals[]` VA (`*(T*)(g_eqmain_guess + off)`,
                        `*(T*)(EQ_GLOBAL(u8*, 0x006E4E44) + off)`) -> `<accessor>->field`, where accessor is the
                        global's `accessor` (else its `name`). The base must be a byte pointer or an integer (or carry
                        a byte/integer cast). A TU that does not see the accessor gets `#include "<globals[].include>"`
                        after its last project include.
  S4c cast at use       (--cast-at-use) rows whose base is a local the census typed by assignment (base_inferred:
                        every assignment comes from the struct, no pointer arithmetic on it) ->
                        `((Struct*)x)->field`; --cast-at-use-files limits it to some TUs; --role FAMILY=STRUCT
                        assigns a role to an untyped census family (a parameter whose role a human/S2 decided).
  macro definitions     rows that come from a macro expansion (census `via_macro`): the deref inside the macro's
                        #define is rewritten once when it does not depend on a macro parameter and every use passes
                        the per-function checks; otherwise every use is refused as via_macro, or, with
                        --macro-uses, each invocation whose full expansion is exactly one raw deref matching its census
                        row is replaced as a whole (`HWM_FIELD(int, HWM_EQMAIN, 0xEB8)` -> `pinstEQ_Main_guess->...`;
                        S4a/S4c need the base to be an argument written at the use site).
Derived structs (C++ `: public Family`, transitively) count as the family; their own layout gives the offsets.
Members inside by-value struct members, struct-typed union roles and overlays are reached by their dotted path
(`x->serialState_guess.file`, named unions `x->u.sub.field`); the first declared member of the access type wins
(the primary before its union roles). A pointer to a whole by-value struct member maps to its address
(`(RdpSerialStateTri_guess*)((char*)x + 0x118)` -> `&x->serialState_guess`).

Dry run (default) prints one row per site; --apply edits the files (read/written with newline='', so CRLF and LF
files keep their line endings byte for byte outside the edited spans). --report writes all sites as JSON, --md the
rejected-site report.
"""
from __future__ import annotations

import argparse
import collections
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import cparse as C  # noqa: E402
from gen_layout_h import load_layout  # noqa: E402

NAME_SENSITIVE_BUILTIN = {0x004F0290}
HEADER_SENSITIVE_VAS = {0x00428686}
HEADER_SENSITIVE_FILES = ("unit_10003700",)

BYTE_CASTS = {"char*", "unsigned char*", "signed char*", "u8*", "s8*", "BYTE*", "byte*", "UCHAR*", "uint8_t*",
              "int8_t*", "void*", "CHAR*"}
INT_CASTS = {"int", "unsigned int", "unsigned", "u32", "s32", "DWORD", "long", "unsigned long", "uintptr_t",
             "intptr_t", "size_t", "UINT", "INT", "ULONG", "LONG", "DWORD_PTR", "UINT_PTR", "INT_PTR"}
SPELLING = {"signed int": "int", "signed": "int", "unsigned": "unsigned int", "long int": "long",
            "signed long": "long", "signed long int": "long", "unsigned long int": "unsigned long",
            "short int": "short", "signed short": "short", "signed short int": "short",
            "unsigned short int": "unsigned short"}
FALLBACK_TYPEDEFS = {"BYTE": "unsigned char", "WORD": "unsigned short", "DWORD": "unsigned long", "BOOL": "int",
                     "UINT": "unsigned int", "INT": "int", "LONG": "long", "ULONG": "unsigned long",
                     "UCHAR": "unsigned char", "USHORT": "unsigned short", "SHORT": "short", "FLOAT": "float",
                     "f32": "float", "RECT": "tagRECT", "LPRECT": "tagRECT*", "POINT": "tagPOINT"}
MARKER = re.compile(r"^[ \t]*//[ \t]*FUNCTION:[ \t]*\w+[ \t]+0[xX][0-9a-fA-F]+", re.M)
DEREF_HEAD = re.compile(r"\*[ \t\r\n]*\([ \t\r\n]*([A-Za-z_][\w \t]*?(?:[ \t\r\n]*\*(?:[ \t\r\n]*(?:const|volatile))*)+)"
                        r"[ \t\r\n]*\)[ \t\r\n]*\(")
CAST_HEAD = re.compile(r"\([ \t]*((?:const[ \t]+|volatile[ \t]+|unsigned[ \t]+|signed[ \t]+)*[A-Za-z_]\w*"
                       r"(?:[ \t]+(?:int|long|char|short))?[ \t]*(?:\*[ \t]*)*)\)")
POSTFIX = re.compile(r"^[A-Za-z_]\w*(?:\s*(?:->|\.)\s*[A-Za-z_]\w*|\s*\[[^\[\]]*\]|\s*\((?:[^()]|\([^()]*\))*\))*$")


@dataclass
class Site:
    file: str
    line: int
    func: str
    func_va: str | None
    form: str
    base_expr: str
    offset: int
    type: str
    rw: str
    family: str
    start: int = -1
    end: int = -1
    old: str = ""
    new: str = ""
    member: str = ""
    status: str = "refused"         # rewrite | refused
    reason: str = ""
    detail: str = ""
    mode: str = "s4a"               # s4a | s4b | s4c | macro
    uses: int = 1
    include: str = ""               # header the edit needs in this TU

    def row(self):
        return {k: getattr(self, k) for k in ("file", "line", "func", "func_va", "form", "base_expr", "offset", "type",
                                              "rw", "family", "member", "status", "reason", "detail", "old", "new",
                                              "mode", "uses")}


# ------------------------------------------------------------------------------------------ types

class Types:
    def __init__(self, idx: C.Index):
        self.idx = idx

    def canon(self, t: str) -> str:
        base, ptr = C.split_type(t)
        for _ in range(12):
            nxt = self.idx.typedefs.get(base)
            if nxt is None and self.idx.find_struct(base) is None:
                nxt = FALLBACK_TYPEDEFS.get(base)
            if nxt is None:
                break
            b2, p2 = C.split_type(nxt)
            if b2 == base:
                break
            base, ptr = b2, ptr + p2
        base = SPELLING.get(base, base)
        d = self.idx.find_struct(base)
        if d is not None:
            base = d.name
        return C.fmt_type(base, ptr)


# ------------------------------------------------------------------------------------------ helpers

def load_name_sensitive(paths) -> set:
    out = set(NAME_SENSITIVE_BUILTIN)
    files = []
    for p in paths or []:
        p = Path(p)
        if p.is_dir():
            files += sorted(p.rglob("name_sensitive.json"))
        elif p.is_file():
            files.append(p)
    for f in files:
        try:
            doc = json.loads(f.read_text(encoding="utf-8"))
        except (OSError, ValueError):
            continue
        for e in doc if isinstance(doc, list) else []:
            try:
                out.add(int(str(e["va"]), 16))
            except (KeyError, ValueError, TypeError):
                pass
    return out


def norm_ws(s: str) -> str:
    s = " ".join(s.split())
    s = re.sub(r"\s*(->|\.|\(|\)|\[|\]|,)\s*", r"\1", s)
    return s.replace(", ", ",")


def strip_parens(e: str) -> str:
    e = e.strip()
    while e.startswith("(") and C.match_close(e, 0) == len(e) - 1:
        e = e[1:-1].strip()
    return e


def top_level_ops(e: str) -> bool:
    """True when e has a binary + or - at depth 0 (pointer arithmetic on the base)."""
    return len([t for _, t in split_terms(e) if t.strip()]) > 1


def split_terms(e: str):
    """Split at binary + / - of depth 0 (not `->`, `++`, `--`, unary signs). [(op, text)], first op ''."""
    out = []
    depth = 0
    cur = 0
    op = ""
    prev = ""
    i = 0
    while i < len(e):
        c = e[i]
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        elif depth == 0 and c in "+-":
            two = e[i:i + 2]
            if two in ("->", "++", "--"):
                i += 2
                prev = "x"
                continue
            if prev and prev not in "(*&!~=<>,+-?:|^%/":
                out.append((op, e[cur:i]))
                op = c
                cur = i + 1
                prev = c
                i += 1
                continue
        if not c.isspace():
            prev = c
        i += 1
    out.append((op, e[cur:]))
    return out


def split_sum(content: str):
    """'(char*)base + 0x2A4' -> (cast_text_or_None, base_text, off_int) or None."""
    terms = [(op, t) for op, t in split_terms(content) if t.strip()]
    if len(terms) != 2:
        return None
    (op0, t0), (op1, t1) = terms
    lit = strip_parens(t1.strip())
    if not re.fullmatch(r"0[xX][0-9a-fA-F]+|\d+", lit):
        return None
    off = int(lit, 0) * (-1 if op1 == "-" else 1)
    t0s = t0.strip()
    m = CAST_HEAD.match(t0s)
    if m:
        rest = t0s[m.end():].strip()
        if rest and not rest.startswith(("+", "-")):
            return m.group(1).strip(), rest, off
    return None, t0s, off


def in_asm(masked: str, pos: int) -> bool:
    ls = masked.rfind("\n", 0, pos) + 1
    le = masked.find("\n", pos)
    line = masked[ls:le if le >= 0 else len(masked)]
    if re.search(r"\b_{1,2}asm\b", line):
        return True
    # inside an `__asm { ... }` block
    for m in re.finditer(r"\b__asm\s*\{", masked[max(0, pos - 20000):pos]):
        b = max(0, pos - 20000) + m.end() - 1
        if C.match_close(masked, b) > pos:
            return True
    return False


def func_is_naked(text: str, masked: str, pos: int) -> bool:
    """The `// FUNCTION:` marker (in the raw text; comments are masked) up to the body's `{` holds naked."""
    mk = None
    for mk in MARKER.finditer(text, 0, pos):
        pass
    start = mk.start() if mk else max(0, pos - 4000)
    brace = masked.find("{", start, pos)
    head = masked[start:brace if brace >= 0 else pos]
    return re.search(r"__declspec\s*\(\s*naked\s*\)", head) is not None


# ------------------------------------------------------------------------------------------ the engine

class Propagator:
    def __init__(self, repo: Path, layouts: list, census_rows: list, name_sensitive: set, files=None, log=print,
                 cast_at_use=False, cast_at_use_files=None, macros=True, skip_funcs=(), roles=None, macro_uses=False,
                 whole_function=False):
        self.repo = repo
        self.docs = layouts
        self.rows = census_rows
        if roles:
            rows2 = []
            for r in census_rows:
                st = roles.get(r.get("family"))
                if st and not r.get("base_struct"):
                    r = dict(r, base_struct=st, base_inferred=True, role_assigned=True)
                rows2.append(r)
            self.rows = rows2
        self.name_sensitive = name_sensitive
        self.files = set(files) if files else None
        self.log = log
        self.cast_at_use = cast_at_use
        self.cau_files = set(cast_at_use_files) if cast_at_use_files else None
        self.macros = macros
        self.macro_uses = macro_uses and macros
        self.whole_function = whole_function
        self.skip_funcs = {int(str(x), 16) for x in skip_funcs or ()}
        self.idx = C.load_index(repo, subdirs=("include", "src"))
        self.types = Types(self.idx)
        self.sites: list[Site] = []
        self.family_names = {}
        self.globals = {}          # va int -> (doc, accessor, include)
        self.keep = set()
        for doc in layouts:
            d = self.idx.find_struct(doc.family)
            if d is None:
                raise SystemExit(f"propagate: struct {doc.family} not found in {repo}/include|src")
            names = set(d.names) | set(doc.aliases) | {doc.family}
            doc.def_file = d.file
            for n in names:
                self.family_names[n] = doc
            for g in doc.raw.get("globals") or []:
                acc = g.get("accessor") or g.get("name")
                if acc:
                    inc = g.get("include") or ""
                    self.globals[int(str(g["va"]), 16)] = (doc, acc, ";".join(inc) if isinstance(inc, list) else inc)
            for k in doc.keep_cast_sites:
                self.keep.add((k["file"].replace("\\", "/"), int(k["line"])))
        # derived structs (C++ bases), transitively
        changed = True
        while changed:
            changed = False
            for sd in self.idx.all_defs():
                if sd.name in self.family_names:
                    continue
                for bn in sd.bases:
                    if bn in self.family_names:
                        for n in sd.names:
                            self.family_names[n] = self.family_names[bn]
                        changed = True
                        break
        self._members = {}
        self._incl = {}

    # -- member lookup on the CURRENT header layout of the struct the base points to
    def resolve(self, sname, rel=None):
        """The struct a type name means in TU rel: a typedef alias visible there ( in an included header) wins over same-named structs of other TUs."""
        if rel:
            if rel not in self._incl:
                self._incl[rel] = self.idx.included(rel)
            name = sname
            for _ in range(6):
                hit = None
                for f in self._incl[rel]:
                    fi = self.idx.files.get(f)
                    if fi and name in fi.typedefs:
                        hit = C.split_type(fi.typedefs[name])
                        break
                if hit is None or hit[1]:
                    break
                name = hit[0]
            if name != sname:
                return self.idx.find_struct(name, rel)
        return self.idx.find_struct(sname, rel)

    def members(self, sname, rel=None):
        key = (sname, rel)
        if key not in self._members:
            d = self.resolve(sname, rel)
            lay = self.idx.layout(d)
            mem = []
            for m in lay.members:
                if m.is_pad or m.bits is not None or m.name == "__vptr":
                    continue
                mem.append(m)
            self._members[key] = (lay, mem)
        return self._members[key]

    def find_member(self, doc, off: int, acc_type: str, sname: str | None = None, rel: str | None = None):
        """(expr_suffix, member_name, reason, detail)"""
        sname = sname or doc.family
        lay, mem = self.members(sname, rel)
        want = self.types.canon(acc_type)
        vol = None          # a matching member that is volatile: a plain access through it would change codegen
        # a whole by-value struct member of exactly the access type (struct-typed union roles / overlays:
        # `(RdpSerialStateTri_guess*)((char*)x + 0x118)` -> `&x->serialState_guess`)
        for m in lay.aggregates:
            if m.offset == off and self.types.canon(m.type) == want:
                if not m.volatile:
                    return m.path, m.path, None, None
                vol = vol or m
        cover = [m for m in mem if m.offset <= off < m.offset + max(m.size, 1)]
        if not cover and vol is None:
            return None, None, "no_member", f"0x{off:X} is padding/unnamed in {sname}"
        seen = []
        for m in cover:
            dims = [x for x in m.dims if x is not None]
            if dims:
                es = m.elem_size or (m.size // max(1, nprod(dims)))
                et = self.types.canon(m.type)
                seen.append(f"{m.path}:{et}{''.join(f'[{x}]' for x in dims)}")
                rel_off = off - m.offset
                if es and rel_off % es == 0 and et == want:
                    if m.volatile:
                        vol = vol or m
                        continue
                    k = rel_off // es
                    idx = []
                    for dd in reversed(dims):
                        idx.append(k % dd)
                        k //= dd
                    return f"{m.path}{''.join(f'[{i}]' for i in reversed(idx))}", m.path, None, None
                continue
            mt = self.types.canon(m.type)
            seen.append(f"{m.path}:{mt}")
            if m.offset == off and mt == want:
                if m.volatile:
                    vol = vol or m
                    continue
                return m.path, m.path, None, None
        if vol is not None:
            return None, None, "volatile", f"member {vol.path} is volatile, the access is not"
        starts = [m for m in cover if m.offset == off or m.dims]
        if not starts:
            return None, None, "no_member", f"0x{off:X} is inside {', '.join(seen)}"
        return None, None, "type_differs", f"access {want} vs {', '.join(seen)}"

    def mode_of(self, r):
        gva = r.get("base_global_va")
        if gva and int(gva, 16) in self.globals and r.get("base_struct") not in self.family_names:
            return "s4b"
        if r.get("base_struct") in self.family_names and self.is_family_def(r["base_struct"], r["file"]):
            return "s4c" if r.get("base_inferred") else "s4a"
        return None

    def is_family_def(self, sname, rel) -> bool:
        """The struct  as the TU sees it is the layout's own definition (or derives from it), not a TU-local
        struct that happens to share an alias name (eqgfx Actor_guess copies)."""
        key = ("fam", sname, rel)
        if key not in self._members:
            doc = self.family_names[sname]
            d = self.resolve(sname, rel)
            ok = d is not None and d.file == doc.def_file
            seen = set()
            while d is not None and not ok and d.bases and d.uid not in seen:
                seen.add(d.uid)
                d = self.idx.find_struct(d.bases[0], rel)
                ok = d is not None and d.file == doc.def_file
            self._members[key] = ok
        return self._members[key]

    def collect(self):
        rows = [r for r in self.rows
                if r.get("form") in ("deref_cast", "ptr_add") and self.mode_of(r)
                and (self.files is None or r["file"] in self.files)]
        byfile = collections.defaultdict(list)
        macro_rows = collections.defaultdict(list)
        for r in rows:
            if r.get("via_macro"):
                macro_rows[(r["file"], r["via_macro"])].append(r)
            else:
                byfile[r["file"]].append(r)
        for rel in sorted(byfile):
            self.scan_file(rel, byfile[rel])
        self.scan_macros(macro_rows)
        if self.whole_function:
            self.whole_function_globals()
        return self.sites

    def whole_function_globals(self):
        """CSE rule (0x0043B010): within one function, a global's value is converted through its accessor for all of
        its census uses or for none (a function where some use is refused keeps every use raw)."""
        groups = collections.defaultdict(list)
        for st in self.sites:
            if st.mode.split(":")[-1] == "s4b" and st.func_va:
                groups[(st.file if not st.func.startswith("#define") else st.func, st.func_va, st.family)].append(st)
        for key, ss in groups.items():
            bad = [x for x in ss if x.status != "rewrite"]
            if bad and len(bad) < len(ss):
                for x in ss:
                    if x.status == "rewrite":
                        x.status, x.reason = "refused", "partial_function"
                        x.detail = f"{len(bad)} other use(s) of {x.family} in {x.func_va} stay raw ({bad[0].reason})"

    def scan_file(self, rel: str, rows: list):
        p = self.repo / rel
        try:
            text = read_raw(p)
        except OSError:
            for r in rows:
                s = self.site_of(r)
                s.reason, s.detail = "census_stale", "file missing"
                self.sites.append(s)
            return
        masked = C.mask(text)
        line_starts = [0] + [i + 1 for i, c in enumerate(text) if c == "\n"]
        used = set()
        file_sites = []
        for r in sorted(rows, key=lambda r: (r["line"], r["offset"] or 0)):
            s = self.site_of(r)
            file_sites.append(s)
            ln = r["line"]
            if ln - 1 >= len(line_starts):
                s.reason, s.detail = "census_stale", "line past end"
                continue
            lo = line_starts[ln - 1]
            hi = line_starts[ln] if ln < len(line_starts) else len(text)
            if r.get("dynamic"):
                s.reason, s.detail = "dynamic_offset", r["dynamic"]
                continue
            if r.get("note") and str(r["note"]).startswith("scaled"):
                s.reason, s.detail = "scaled_offset", r["note"]
                continue
            if s.mode == "s4c" and not self.cast_at_use_ok(rel):
                s.reason, s.detail = "base_not_typed", f"base {s.base_expr} inferred ({r.get('base_type')}); S4c off"
                continue
            found = self.locate(text, masked, lo, hi, r, used)
            if found is None:
                s.reason, s.detail = "census_stale", "access text not found on its census line"
                continue
            used.add(found["start"])
            s.start, s.end, s.old = found["start"], found["end"], text[found["start"]:found["end"]]
            self.judge(s, r, found, text, masked)
        # nested: an accepted site containing another accepted one is refused
        ok = sorted([s for s in file_sites if s.status == "rewrite"], key=lambda s: (s.start, -s.end))
        for a in ok:
            for b in ok:
                if a is not b and a.start <= b.start and b.end <= a.end and b.status == "rewrite":
                    a.status, a.reason, a.detail = "refused", "nested", f"contains line {b.line} site"
                    break
        self.sites.extend(file_sites)

    def cast_at_use_ok(self, rel):
        return self.cast_at_use and (self.cau_files is None or rel in self.cau_files)

    def site_of(self, r) -> Site:
        s = Site(r["file"], r["line"], r.get("func") or "", r.get("func_va"), r["form"], r.get("base_expr") or "",
                 r.get("offset") if r.get("offset") is not None else -1, r.get("type") or "", r.get("rw") or "",
                 r.get("base_struct") or (r.get("base_global_va") or ""))
        s.mode = self.mode_of(r) or "s4a"
        return s

    def locate(self, text, masked, lo, hi, r, used):
        want_base = norm_ws(r.get("base_expr") or "")
        if r["form"] == "deref_cast":
            for m in DEREF_HEAD.finditer(masked, lo, hi):
                if m.start() in used:
                    continue
                lp = m.end() - 1
                rp = C.match_close(masked, lp)
                if rp >= len(masked):
                    continue
                content = masked[lp + 1:rp]
                ss = split_sum(strip_parens(content))
                if not ss:
                    continue
                cast, base, off = ss
                if norm_ws(strip_parens(base)) != want_base:
                    continue
                if not self.offset_matches(r, off, cast):
                    continue
                return {"start": m.start(), "end": rp + 1, "ptype": " ".join(m.group(1).split()), "cast": cast,
                        "base_span": self.base_span(text, masked, lp + 1, rp, base), "off": off, "kind": "deref"}
            return None
        # ptr_add: `(char*)base + off`, look for an outer `(T*)( ... )`
        for m in CAST_HEAD.finditer(masked, lo, hi):
            if m.start() in used:
                continue
            ctext = " ".join(m.group(1).split())
            if C.norm_type(ctext).replace(" ", "") not in {x.replace(" ", "") for x in BYTE_CASTS}:
                continue
            k = m.end()
            tail = masked[k:hi]
            base_txt, off, end = None, None, None
            for mo in re.finditer(r"[ \t]*([+-])[ \t]*(0[xX][0-9a-fA-F]+|\d+)\b", tail):
                cand = tail[:mo.start()]
                if norm_ws(strip_parens(cand)) == want_base:
                    base_txt = cand
                    off = int(mo.group(2), 0) * (-1 if mo.group(1) == "-" else 1)
                    end = k + mo.end()
                    break
            if base_txt is None or off != r.get("offset"):
                continue
            return {"start": m.start(), "end": end, "ptype": None, "cast": ctext, "off": off, "kind": "addr",
                    "base_span": (k + len(base_txt) - len(base_txt.lstrip()), k + len(base_txt.rstrip()))}
        return None

    def offset_matches(self, r, off, cast):
        if r.get("offset") == off:
            return True
        return bool(r.get("note")) and cast is None      # scaled sites: matched, refused later

    def base_span(self, text, masked, a, b, base):
        """Position of the base expression inside the deref parens [a, b)."""
        seg = masked[a:b]
        i = seg.find(base.strip())
        if i < 0:
            i = 0
        return a + i, a + i + len(base.strip())

    def func_checks(self, r, s, text, masked):
        """Per-function refusals; returns (reason, detail) or None."""
        va = int(r["func_va"], 16) if r.get("func_va") else None
        stem = Path(r["file"]).stem
        if va is not None and va in self.skip_funcs:
            return "gate_rollback", f"{r.get('func_va')}"
        if (r["file"], r["line"]) in self.keep:
            return "keep_cast", "layout keep_cast_sites"
        if va in HEADER_SENSITIVE_VAS or any(h in stem for h in HEADER_SENSITIVE_FILES):
            return "header_sensitive", f"{r.get('func_va')}"
        if va is not None and va in self.name_sensitive:
            return "name_sensitive", f"{r.get('func_va')}"
        if s is not None and s.start >= 0:
            if func_is_naked(text, masked, s.start):
                return "naked", ""
            if in_asm(masked, s.start):
                return "asm", ""
        return None

    def base_ok(self, s, r, cast, base_txt):
        """Mode-specific base checks; returns (reason, detail) or None, and sets s.include for S4b."""
        bt = C.split_type(r.get("base_type") or "")
        if s.mode == "s4b":
            if re.match(r"^\(?\s*\*\s*\(", strip_parens(base_txt.strip())) or r.get("base_src") == "abs":
                return "abs_base", base_txt.strip()
            if top_level_ops(strip_parens(C.mask(base_txt))):
                return "base_arith", base_txt.strip()
            if cast is None:
                base_t = r.get("base_type") or ""
                if not (C.is_byte_ptr(base_t) and C.split_type(base_t)[0] != "void") and \
                        not (bt[1] == 0 and (bt[0] in INT_CASTS or self.types.canon(bt[0]) in
                                             {self.types.canon(x) for x in INT_CASTS})):
                    return "scaled_offset", f"global value typed {base_t!r} without a byte cast"
            return None
        if s.mode == "s4c":
            if not re.fullmatch(r"[A-Za-z_]\w*", strip_parens(base_txt.strip())):
                return "role_not_fixed", f"base {base_txt.strip()!r} is not a plain local"
            if top_level_ops(strip_parens(C.mask(base_txt))):
                return "base_arith", base_txt.strip()
            if cast is None and not (C.is_byte_ptr(r.get("base_type") or "") or bt[1] == 0):
                return "scaled_offset", f"base typed {r.get('base_type')!r} without a byte cast"
            return None
        if r.get("base_inferred"):
            return "base_not_typed", f"base {s.base_expr} inferred ({r.get('base_type')})"
        if bt[1] != 1 or bt[0] not in self.family_names:
            return "base_not_typed", f"base type {r.get('base_type')!r}"
        if top_level_ops(strip_parens(C.mask(base_txt))):
            return "base_arith", base_txt.strip()
        if cast is None:
            return "scaled_offset", "no byte/integer cast on the base"
        return None

    def new_base(self, s, r, base_txt):
        if s.mode == "s4b":
            doc, acc, inc = self.globals[int(r["base_global_va"], 16)]
            s.include = inc
            return acc, doc, doc.family
        doc = self.family_names[r["base_struct"]]
        if s.mode == "s4c":
            return f"(({r['base_struct']}*){strip_parens(base_txt.strip())})", doc, r["base_struct"]
        return postfix(base_txt), doc, r["base_struct"]

    def judge(self, s: Site, r, found, text, masked):
        def refuse(reason, detail=""):
            s.status, s.reason, s.detail = "refused", reason, detail
        fc = self.func_checks(r, s, text, masked)
        if fc:
            return refuse(*fc)
        cast = found["cast"]
        ptype = found["ptype"]
        if (ptype and re.search(r"\bvolatile\b", ptype)) or (cast and re.search(r"\bvolatile\b", cast)) \
                or re.search(r"\bvolatile\b", r.get("type") or ""):
            return refuse("volatile")
        if r.get("dynamic"):
            return refuse("dynamic_offset", r["dynamic"])
        base_txt = found.get("base_txt") or text[found["base_span"][0]:found["base_span"][1]]
        if re.search(r"\bvolatile\b", base_txt) or re.search(r"\bvolatile\b", r.get("base_type") or ""):
            return refuse("volatile", "volatile base")
        bo = self.base_ok(s, r, cast, base_txt)
        if bo:
            return refuse(*bo)
        if cast is not None:
            cn = C.norm_type(cast).replace(" ", "")
            if cn not in {x.replace(" ", "") for x in BYTE_CASTS} and C.norm_type(cast) not in INT_CASTS \
                    and self.types.canon(cast) not in {self.types.canon(x) for x in INT_CASTS}:
                return refuse("scaled_offset", f"base cast ({cast}) is not a byte or integer cast")
        nb, doc, sname = self.new_base(s, r, base_txt)
        if found["kind"] == "deref":
            acc = r.get("type") or C.fmt_type(C.split_type(ptype)[0], C.split_type(ptype)[1] - 1)
            expr, mname, why, det = self.find_member(doc, found["off"], acc, sname, r["file"])
            if expr is None:
                return refuse(why, det)
            s.member = mname
            s.new = f"{nb}->{expr}"
        else:
            # address form: needs an enclosing `(T*)( ... )` whose parens are exactly this sum
            a = found["start"]
            pre = masked[:a]
            mo = re.search(r"\([ \t]*([A-Za-z_][\w \t]*?(?:[ \t]*\*)+)[ \t]*\)[ \t\r\n]*\([ \t\r\n]*$", pre)
            if not mo:
                return refuse("addr_untyped", "char* sum used as is")
            lp = len(pre.rstrip()) - 1
            rp = C.match_close(masked, lp)
            if rp >= len(masked) or masked[found["end"]:rp].strip():
                return refuse("addr_untyped", "outer parens hold more than the sum")
            tptr = " ".join(mo.group(1).split())
            if re.search(r"\bvolatile\b", tptr):
                return refuse("volatile")
            tb, tp = C.split_type(tptr)
            expr, mname, why, det = self.find_member(doc, found["off"], C.fmt_type(tb, tp - 1), sname, r["file"])
            if expr is None:
                return refuse("addr_type_differs" if why == "type_differs" else why, det)
            s.start, s.end = mo.start(), rp + 1
            s.old = text[s.start:s.end]
            s.member = mname
            s.new = f"&{nb}->{expr}"
        s.status, s.reason = "rewrite", ""

    # -- macro definitions
    def find_define(self, rel, name):
        """(file rel, start, end of the #define line (without newline), params list or None)."""
        cands = [rel] + sorted(self.idx.included(rel) - {rel})
        rx = re.compile(r"^[ \t]*#[ \t]*define[ \t]+" + re.escape(name) + r"(\([^)\n]*\))?[ \t]+([^\n]*)$", re.M)
        for f in cands:
            p = self.repo / f
            if not p.is_file():
                continue
            t = read_raw(p)
            m = rx.search(t)
            if m:
                body = m.group(2).rstrip("\r")
                params = [x.strip() for x in m.group(1)[1:-1].split(",")] if m.group(1) else None
                return f, m.start(2), m.start(2) + len(body), params, body
        return None

    def scan_macros(self, groups):
        defs = {}
        use_rows = collections.defaultdict(list)
        for (rel, name), rows in sorted(groups.items()):
            d = self.find_define(rel, name)
            if d is None or not self.macros:
                if d is None and self.macro_uses:
                    use_rows[(rel, name)].extend(rows)
                    continue
                for r in rows:
                    s = self.site_of(r)
                    s.reason, s.detail = "via_macro", f"{name}: no single-line #define found" if d is None else name
                    s.mode = "macro"
                    self.sites.append(s)
                continue
            defs.setdefault((d[0], d[1]), (d, name, []))[2].extend(rows)
        for (frel, bstart), (d, name, rows) in defs.items():
            frel, a, b, params, body = d
            text = read_raw(self.repo / frel)
            masked = C.mask(text)
            sites_here = []
            for m in DEREF_HEAD.finditer(masked, a, b):
                lp = m.end() - 1
                rp = C.match_close(masked, lp)
                if rp > b:
                    continue
                ss = split_sum(strip_parens(masked[lp + 1:rp]))
                if not ss:
                    continue
                cast, base, off = ss
                ptype = " ".join(m.group(1).split())
                acc = C.fmt_type(C.split_type(ptype)[0], C.split_type(ptype)[1] - 1)
                mine = [r for r in rows if r.get("offset") == off and self.types.canon(r.get("type") or "") ==
                        self.types.canon(acc)]
                if not mine:
                    continue
                r = mine[0]
                s = self.site_of(r)
                s.mode = "macro:" + (self.mode_of(r) or "")
                s.file, s.line, s.uses = frel, C.LineIndex(text).line(m.start()), len(mine)
                s.func = f"#define {name}"
                s.start, s.end, s.old = m.start(), rp + 1, text[m.start():rp + 1]
                sites_here.append((s, mine))
                inner = masked[lp + 1:rp]
                if params and any(re.search(r"\b" + re.escape(pp) + r"\b", inner) for pp in params if pp):
                    s.reason, s.detail = "via_macro", f"{name}: the access depends on a macro parameter"
                    if self.macro_uses:
                        sites_here.pop()
                        for x in mine:
                            use_rows[(x["file"], name)].append(x)
                    continue
                bad = [self.func_checks(x, None, text, masked) for x in mine]
                bad = [x for x in bad if x]
                if bad:
                    s.reason, s.detail = bad[0][0], f"a use of {name}: {bad[0][1]}"
                    continue
                found = {"start": m.start(), "end": rp + 1, "ptype": ptype, "cast": cast, "off": off, "kind": "deref",
                         "base_span": self.base_span(text, masked, lp + 1, rp, base)}
                real_mode = self.mode_of(r)
                s.mode = real_mode
                if real_mode == "s4c":
                    s.reason, s.detail = "role_not_fixed", f"{name}: a local inside a macro body"
                    s.mode = "macro:s4c"
                    continue
                self.judge(s, r, found, text, masked)
                s.mode = "macro:" + real_mode
            covered = {id(r) for _, mine in sites_here for r in mine}
            covered |= {id(x) for v in use_rows.values() for x in v}
            for r in rows:
                if id(r) not in covered and self.macro_uses:
                    use_rows[(r["file"], name)].append(r)
                    continue
                if id(r) not in covered:
                    s = self.site_of(r)
                    s.mode = "macro"
                    s.reason, s.detail = "via_macro", f"{name}: access not found as a literal deref in the #define"
                    self.sites.append(s)
            self.sites.extend(s for s, _ in sites_here)
        for (rel, name), rows in sorted(use_rows.items()):
            self.sites.extend(self.scan_macro_uses(rel, name, rows))

    # -- macro use sites (parameter-driven macros: HWM_FIELD(int, HWM_EQMAIN, 0xEB8) -> pinstEQ_Main_guess->field)
    def macro_table(self, rel):
        key = ("macros", rel)
        if key not in self._members:
            import census as CE
            if rel not in self._incl:
                self._incl[rel] = self.idx.included(rel)
            tab = {}
            for f in [rel] + sorted(x for x in self._incl[rel] if x != rel):
                fi = self.idx.files.get(f)
                if fi:
                    for k, v in CE.parse_macros(fi).items():
                        if not k.startswith(CE._MACRO_SKIP_PREFIX):
                            tab.setdefault(k, v)
            self._members[key] = tab
        return self._members[key]

    def scan_macro_uses(self, rel, name, rows):
        """Rewrite whole macro invocations whose full expansion is exactly one raw deref matching a census row."""
        import census as CE
        out = []
        text = read_raw(self.repo / rel)
        masked = C.mask(text)
        tab = self.macro_table(rel)
        ent = tab.get(name)
        starts = [0] + [i + 1 for i, c in enumerate(text) if c == "\n"]
        byline = collections.defaultdict(list)
        for r in rows:
            byline[r["line"]].append(r)
        for ln, rs in sorted(byline.items()):
            left = list(rs)
            if ent is None or ln - 1 >= len(starts):
                continue
            lo = starts[ln - 1]
            hi = starts[ln] if ln < len(starts) else len(text)
            for m in re.finditer(r"\b" + re.escape(name) + r"\b", masked[lo:hi]):
                s0, e0 = lo + m.start(), lo + m.end()
                if ent[0] is not None:
                    k = e0
                    while k < len(masked) and masked[k] in " \t\r\n":
                        k += 1
                    if k >= len(masked) or masked[k] != "(":
                        continue
                    rp = C.match_close(masked, k)
                    if rp >= len(masked):
                        continue
                    e0 = rp + 1
                seg = masked[s0:e0]
                try:
                    ex_t, ex_o, _ = CE.MacroExpander(tab).expand(seg, list(range(s0, e0)), [None] * len(seg))
                except OverflowError:
                    continue
                # peel outer parens, the whole expansion must be one deref
                a, b = 0, len(ex_t)
                while True:
                    while a < b and ex_t[a] in " \t\r\n":
                        a += 1
                    while b > a and ex_t[b - 1] in " \t\r\n":
                        b -= 1
                    if a < b and ex_t[a] == "(" and C.match_close(ex_t, a) == b - 1:
                        a, b = a + 1, b - 1
                        continue
                    break
                md = DEREF_HEAD.match(ex_t, a)
                if not md:
                    continue
                lp = md.end() - 1
                rp2 = C.match_close(ex_t, lp)
                if rp2 != b - 1:
                    continue
                inner = ex_t[lp + 1:rp2]
                ss = split_sum(strip_parens(inner))
                if not ss:
                    continue
                cast, base, off = ss
                ptype = " ".join(md.group(1).split())
                acc = C.fmt_type(C.split_type(ptype)[0], C.split_type(ptype)[1] - 1)
                mine = [r for r in left if r.get("offset") == off and
                        self.types.canon(r.get("type") or "") == self.types.canon(acc)]
                if not mine:
                    continue
                r = mine[0]
                left.remove(r)
                st = self.site_of(r)
                st.start, st.end, st.old = s0, e0, text[s0:e0]
                st.detail = name
                # the base as written at the use site (argument text), when it maps back contiguously
                bfull = base.strip()
                core = strip_parens(bfull)
                bi = inner.find(bfull)
                base_orig = None
                if bi >= 0:
                    bi += bfull.find(core)
                    o = ex_o[lp + 1 + bi: lp + 1 + bi + len(core)]
                    if o and all(s0 <= x < e0 for x in o) and o == list(range(o[0], o[0] + len(o))):
                        base_orig = text[o[0]:o[-1] + 1]
                mode = self.mode_of(r)
                if base_orig is None and mode != "s4b":
                    st.reason, st.detail = "via_macro", f"{name}: the base is built inside the macro"
                    st.mode = "macro-use:" + (mode or "")
                    out.append(st)
                    continue
                found = {"start": s0, "end": e0, "ptype": ptype, "cast": cast, "off": off, "kind": "deref",
                         "base_span": (s0, s0), "base_txt": base_orig if base_orig is not None else base.strip()}
                self.judge(st, r, found, text, masked)
                st.mode = "macro-use:" + (mode or "")
                out.append(st)
            for r in left:
                st = self.site_of(r)
                st.mode = "macro-use"
                st.reason, st.detail = "via_macro", f"{name}: the expansion at the use site is not one raw deref"
                out.append(st)
        return out

    # -- apply
    def visible_define(self, rel, name) -> bool:
        if rel not in self._incl:
            self._incl[rel] = self.idx.included(rel)
        for f in self._incl[rel]:
            fi = self.idx.files.get(f)
            if fi and (name in fi.defines or name in fi.funcmacros):
                return True
        return False

    def apply(self) -> dict:
        byfile = collections.defaultdict(list)
        for s in self.sites:
            if s.status == "rewrite":
                byfile[s.file].append(s)
        changed = {}
        for rel, ss in byfile.items():
            p = self.repo / rel
            text = read_raw(p)
            for s in sorted(ss, key=lambda s: s.start, reverse=True):
                if text[s.start:s.end] != s.old:
                    raise SystemExit(f"propagate: {rel}:{s.line} text changed under the edit")
                text = text[:s.start] + s.new + text[s.end:]
            if rel not in self._incl:
                self._incl[rel] = self.idx.included(rel)
            needs = []
            for s in ss:
                if not s.mode.endswith("s4b") or not s.include:
                    continue
                for inc in s.include.split(";"):
                    inc = inc.strip()
                    if inc and "include/" + inc not in self._incl[rel] and inc not in needs:
                        needs.append(inc)
            for inc in needs:
                text = add_include(text, inc)
                self.log(f"  {rel}: + #include \"{inc}\"")
            write_raw(p, text)
            changed[rel] = len(ss)
        return changed


def nprod(dims):
    n = 1
    for x in dims:
        n *= x
    return n


def add_include(text: str, inc: str) -> str:
    """Insert `#include "inc"` after the last top-of-file `#include "..."` line (keeps the file's newline)."""
    nl = "\r\n" if "\r\n" in text else "\n"
    ms = list(re.finditer(r'^[ \t]*#[ \t]*include[ \t]+"[^"\n]+"[^\n]*\n', text, re.M))
    if not ms:
        return f'#include "{inc}"{nl}' + text
    # the first block of includes: stop at the first include separated by code
    last = ms[0]
    for m in ms[1:]:
        between = C.mask(text[last.end():m.start()])
        if re.search(r"[;{}]", between):
            break
        last = m
    return text[:last.end()] + f'#include "{inc}"{nl}' + text[last.end():]


def postfix(base: str) -> str:
    b = base.strip()
    inner = strip_parens(b)
    if POSTFIX.match(" ".join(inner.split())) or re.fullmatch(r"[A-Za-z_]\w*\s*\(.*\)(\s*->\s*\w+)*", inner, re.S):
        return inner if b.startswith("(") and POSTFIX.match(" ".join(inner.split())) else b
    return f"({inner})"


def read_raw(p: Path) -> str:
    with open(p, "r", encoding="latin-1", newline="") as fh:
        return fh.read()


def write_raw(p: Path, text: str):
    with open(p, "w", encoding="latin-1", newline="") as fh:
        fh.write(text)


# ------------------------------------------------------------------------------------------ reports

def table(sites) -> str:
    out = []
    for s in sorted(sites, key=lambda s: (s.file, s.line, s.start)):
        loc = f"{s.file}:{s.line}"
        if s.status == "rewrite":
            out.append(f"REWRITE  {loc:<44} 0x{s.offset:<5X} {s.type:<12} {' '.join(s.old.split())}  ->  {s.new}")
        else:
            out.append(f"REFUSE   {loc:<44} 0x{max(s.offset, 0):<5X} {s.type:<12} {s.reason}: {s.detail}")
    return "\n".join(out)


def md_report(sites, families) -> str:
    ok = [s for s in sites if s.status == "rewrite"]
    bad = [s for s in sites if s.status != "rewrite"]
    by = collections.Counter(s.reason for s in bad)
    out = [f"# S4a propagate report ({', '.join(families)})", "",
           f"{len(sites)} candidate sites: {len(ok)} rewritten, {len(bad)} refused.", "",
           "| reason | sites |", "|---|---|"]
    out += [f"| {k} | {v} |" for k, v in by.most_common()]
    out.append("")
    per_member = collections.Counter(s.member for s in ok)
    if per_member:
        out += ["## Rewritten by member", "", "| member | sites |", "|---|---|"]
        out += [f"| {k} | {v} |" for k, v in sorted(per_member.items())]
        out.append("")
    out += ["## Refused sites", "", "| file:line | function | offset | type | reason | detail |", "|---|---|---|---|---|---|"]
    for s in sorted(bad, key=lambda s: (s.reason, s.file, s.line)):
        det = s.detail.replace("|", "\\|")
        out.append(f"| {s.file}:{s.line} | {s.func} | 0x{max(s.offset, 0):X} | {s.type} | {s.reason} | {det} |")
    return "\n".join(out) + "\n"


def load_census(path) -> list:
    rows = []
    with open(path, encoding="utf-8") as fh:
        for ln in fh:
            ln = ln.strip()
            if ln:
                rows.append(json.loads(ln))
    return rows


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--repo", required=True)
    ap.add_argument("--layout", action="append", required=True)
    ap.add_argument("--census", required=True, help="census accesses.jsonl of the same tree state")
    ap.add_argument("--name-sensitive", action="append", default=[])
    ap.add_argument("--files", nargs="*")
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--report", help="all sites as JSON")
    ap.add_argument("--md", help="rejected-site report (markdown)")
    ap.add_argument("-q", "--quiet", action="store_true")
    ap.add_argument("--cast-at-use", action="store_true", help="S4c: ((Struct*)x)->field for census-typed locals")
    ap.add_argument("--cast-at-use-files", nargs="*", help="limit S4c to these TUs")
    ap.add_argument("--no-macros", action="store_true", help="refuse every macro-expanded site")
    ap.add_argument("--whole-function-globals", action="store_true",
                    help="S4b: convert a global's census uses in a function all together or not at all (partial_function)")
    ap.add_argument("--macro-uses", action="store_true",
                    help="rewrite whole invocations of parameter-driven macros whose expansion is one matching deref")
    ap.add_argument("--role", action="append", default=[], metavar="FAMILY=STRUCT",
                    help="S4c: treat a census untyped family (exact family key) as a pointer to STRUCT")
    ap.add_argument("--skip-funcs", nargs="*", default=[], help="function VAs whose sites are refused (gate_rollback)")
    ap.add_argument("--skip-funcs-file", help="file with function VAs (one per line, '#' comments)")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    docs = [load_layout(p) for p in a.layout]
    skip = list(a.skip_funcs)
    if a.skip_funcs_file:
        skip += [ln.split("#")[0].strip() for ln in Path(a.skip_funcs_file).read_text().splitlines()
                 if ln.split("#")[0].strip()]
    pr = Propagator(repo, docs, load_census(a.census), load_name_sensitive(a.name_sensitive),
                    [f.replace("\\", "/") for f in a.files] if a.files else None, cast_at_use=a.cast_at_use,
                    cast_at_use_files=[f.replace("\\", "/") for f in a.cast_at_use_files] if a.cast_at_use_files
                    else None, macros=not a.no_macros, skip_funcs=skip,
                    roles=dict(x.rsplit("=", 1) for x in a.role), macro_uses=a.macro_uses,
                    whole_function=a.whole_function_globals)
    sites = pr.collect()
    if not a.quiet:
        print(table(sites))
    n_ok = sum(1 for s in sites if s.status == "rewrite")
    reasons = collections.Counter(s.reason for s in sites if s.status != "rewrite")
    print(f"{len(sites)} sites: {n_ok} rewrite, {len(sites) - n_ok} refused "
          f"({', '.join(f'{k} {v}' for k, v in reasons.most_common())})")
    if a.report:
        Path(a.report).write_text(json.dumps([s.row() for s in sites], indent=1), encoding="utf-8")
    if a.md:
        Path(a.md).write_text(md_report(sites, [d.family for d in docs]), encoding="utf-8")
    if a.apply:
        ch = pr.apply()
        print(f"applied: {sum(ch.values())} site(s) in {len(ch)} file(s)")
        for k, v in sorted(ch.items()):
            print(f"  {k}: {v}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
