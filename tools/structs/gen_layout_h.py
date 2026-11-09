r"""Layout JSON -> C header (STRUCT-LAYOUT phase S3 of the struct-layout plan; schema LAYOUT_SCHEMA.md).

    ..\.venv\Scripts\python.exe structs\gen_layout_h.py LAYOUT.json [...] --repo TREE [--extend] [--dry-run]
        [--header REL] [--allow-padding-ref NAME ...]

Generate mode (default): writes the JSON's `header` (repo-relative) as a complete, generated header:
  offset-sorted fields, `unknown0xNNN` byte padding for every gap, anonymous unions for fields with `union` roles,
  `#pragma pack(push, N)` only when pack != 8, `C_ASSERT(sizeof)` when `size` is given, one `C_ASSERT(offsetof)`
  per field, `typedef Family Alias;` per alias. An existing file is only overwritten when it carries the
  generated marker (headers are generated, never hand-edited, and a hand-written one is never clobbered).
  Line endings follow the target directory (majority of its *.h files; LF when there are none).

Extend mode (--extend): the family's struct already exists in `header` (e.g. player.h EQActorInfo_guess). Only
  top-level one-line byte-array padding members (`u8 NAME[N];` whose name looks like a placeholder:
  unknown0x04C, unknown04C, pad_0x10, reserved...) are touched: each new field replaces the bytes it covers and
  the rest of the padding is re-emitted as pieces named in that padding's own spelling (unknown0x04C ->
  unknown0x050, unknown04C -> unknown050; a name without its offset in it takes the struct's first such
  spelling, else unknown0xNNN), in the header's own column style (offset digits and decimal/hex array counts as the
  header writes them). Existing fields, comments and C_ASSERTs stay byte for byte;
  one `C_ASSERT(offsetof)` per new field is appended to the struct's assert block. The file keeps its own line
  endings. JSON fields that already exist (same name and offset, by-value struct members included) are skipped,
  except that `union` roles missing from an existing member wrap that member's line (unchanged, indented) in an
  anonymous union; a role wider than the member takes its bytes from the padding that follows. A JSON field
  overlapping an existing non-padding member is an error.
  Padding that the sources reference by name is rewritten in the same run when the reference is exact:
    x->unknown0x0A12[0x33]        -> x->airSupply_guess       (constant index into a new u8 field / array)
    x->unknown0x0A12[0x05]        -> x->unknown0x0A12[0x05]   (still in the piece that keeps the name)
    *(s16*)x->unknown0x1038       -> x->deityId_guess         (deref cast equal to the new field's type)
  A reference counts only when it can be this struct's: the file sees the header (its include closure), and when
  another struct the file sees has a member of the same name, the base expression types to this struct (or does
  not type at all). An address use of a piece that keeps its name and start (`(u32*)x->unknown0x1198`) is left
  (logged). Any other reference is refused unless --allow-padding-ref NAME.

Both modes re-parse the result with cparse and check every field offset and (when known) the struct size before
writing; any mismatch is an error and nothing is written.

Field record extras used here (besides LAYOUT_SCHEMA.md): `comment` (short text for the member's comment; default
none), union member `width` (default: the width of its type when it is a builtin/pointer, else the field's) and
`count` (default: none, a scalar). A union member may be wider than the primary field (`u8 autosplit` + `int
autosplit_int_guess`): the anonymous union's slot is the widest member, and padding resumes after it.
`count` may be a list for multi-dimensional arrays ([6, 400] -> `[6][400]`).
Struct-typed members: a union role (or overlay, or field) may name a struct: one of the layout's `types`
(its width is that struct's size; an explicit `width` must agree; a types entry may use the ones before it), or a
struct of another header (then give `width`: verify checks it against the cparse size). Fields and union / overlay
members take `qualifiers` (["volatile"], ["const"]) or spell them in `type` ("volatile u8"); they are emitted in
front of the type. verify walks every member path through a struct-typed member (x->serialState_guess.file) and
checks its offset. An `overlay` is a named sub-struct view over this field AND the following ones it covers
(`union { struct { ...fields... }; View view; }`); a union role covers only padding after its field.
Extend mode: `types` the header (or what it includes) does not define yet are inserted right above the family's
definition and its leading comment (a type that uses, by value, a struct the header defines only later is
refused); a union role on an existing by-value struct member wraps that member's line. `prelude` and `overlay`
are not applied in extend mode.
Document extras: `types` (sub-structs emitted before the family: [{name, size, fields}]), `methods` (declaration
lines for a class), `includes` ("x.h" or "<x.h>"), `forward` (struct tags to forward-declare), `globals[].accessor`
(emits `#define <accessor> EQ_GLOBAL(<Family> *, <va>)`), `prelude` (raw lines after the includes).
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))

import cparse as C  # noqa: E402

GEN_MARKER = "generated by structs/gen_layout_h.py"
TYPE_COL = 8            # `u8      name` : type padded to 8 columns (player.h style)
DECL_COL = 20           # `name;` padded to 20 columns before the offset comment


class LayoutError(Exception):
    pass


def hexint(v) -> int | None:
    if v is None:
        return None
    if isinstance(v, int):
        return v
    s = str(v).strip()
    return int(s, 16) if s.lower().startswith("0x") else int(s, 0)


def nelem(count) -> int:
    if not count:
        return 1
    if isinstance(count, list):
        n = 1
        for x in count:
            n *= int(x)
        return n
    return int(count)


QUALIFIERS = ("const", "volatile")


def split_quals(t: str, quals=None, where: str = "") -> tuple[str, str]:
    """('volatile u8', None) / ('u8', ['volatile']) -> ('u8', 'volatile'): leading cv-qualifiers of a spelled type
    plus a `qualifiers` list, in C order (const before volatile)."""
    q = set(quals or [])
    bad = q - set(QUALIFIERS)
    if bad:
        raise LayoutError(f"{where}: qualifiers {sorted(bad)} (only const / volatile)")
    t = t.strip()
    while True:
        m = re.match(r"(const|volatile)\s+(.*)$", t)
        if not m:
            break
        q.add(m.group(1))
        t = m.group(2).strip()
    return t, " ".join(x for x in QUALIFIERS if x in q)


def struct_name(t: str) -> str | None:
    """The struct a by-value type spelling names (`struct X` / `X`), or None for a pointer / function type."""
    if "*" in t or "(" in t or "[" in t:
        return None
    t = re.sub(r"^\s*(?:struct|union)\s+", "", t).strip()
    return t if re.fullmatch(r"[A-Za-z_]\w*", t) else None


@dataclass
class UMember:
    name: str
    type: str
    width: int
    count: object = None
    quals: str = ""                                 # "volatile" / "const" / "const volatile"

    @property
    def size(self):
        return self.width * nelem(self.count)

    @property
    def ctype(self) -> str:
        """The declared type text: qualifiers + type."""
        return f"{self.quals} {self.type}" if self.quals else self.type


@dataclass
class Field:
    offset: int
    name: str
    type: str
    width: int
    count: object = None
    comment: str = ""
    union: list = field(default_factory=list)       # [UMember]
    overlay: list = field(default_factory=list)     # [UMember] views over this and the following fields
    quals: str = ""

    @property
    def ctype(self) -> str:
        return f"{self.quals} {self.type}" if self.quals else self.type

    def copy(self, **kw) -> "Field":
        d = dict(offset=self.offset, name=self.name, type=self.type, width=self.width, count=self.count,
                 comment=self.comment, union=self.union, overlay=self.overlay, quals=self.quals)
        d.update(kw)
        return Field(**d)

    @property
    def own_size(self) -> int:
        return self.width * nelem(self.count)

    @property
    def size(self) -> int:
        s = self.own_size
        for u in self.union:
            s = max(s, u.size)
        return s

    @property
    def end(self) -> int:
        return self.offset + self.size


@dataclass
class SubType:
    name: str
    size: int | None
    fields: list


@dataclass
class LayoutDoc:
    family: str
    client: str
    kind: str
    header: str
    size: int | None
    pack: int
    base: str | None
    base_size: int | None
    vtable: bool
    aliases: list
    includes: list
    fields: list
    keep_cast_sites: list
    raw: dict
    path: str = ""
    types: list = field(default_factory=list)
    methods: list = field(default_factory=list)
    forward: list = field(default_factory=list)
    accessors: list = field(default_factory=list)     # [(name, va)]
    prelude: list = field(default_factory=list)       # raw lines after the includes


def load_layout(path) -> LayoutDoc:
    p = Path(path)
    d = json.loads(p.read_text(encoding="utf-8"))
    return parse_layout(d, str(p))


def type_width(t: str) -> int | None:
    """Element width of a plain type spelling (pointers 4, cparse builtins), else None."""
    base, ptr = C.split_type(t)
    if ptr:
        return 4
    b = C.BUILTIN.get(base)
    if b:
        return b[0]
    return C.WIN_STRUCTS.get(base, (None,))[0]


def _member_width(u: dict, tsizes: dict, where: str, default):
    """Element width of a union / overlay member: a struct of the layout's `types` has its size (an explicit
    `width` must agree); else the explicit width, else a builtin/pointer width, else `default`."""
    t, _ = split_quals(u["type"], u.get("qualifiers"), where)
    sn = struct_name(t)
    w = u.get("width")
    if sn in tsizes:
        ts = tsizes[sn]
        if ts is None:
            raise LayoutError(f"{where}: {u['name']} has type {sn}, a `types` struct without a size")
        if w is not None and int(w) != ts:
            raise LayoutError(f"{where}: {u['name']} width {w} != sizeof({sn}) 0x{ts:X}")
        return ts
    return w or type_width(t) or default


def parse_fields(lst, path, tsizes: dict | None = None) -> list:
    """tsizes: {struct name: size} of the layout's `types` defined so far (struct-typed union members)."""
    tsizes = tsizes or {}
    fields = []
    for f in lst or []:
        w = f.get("width")
        if w is None:
            raise LayoutError(f"{path}: field {f.get('name')} has no width")
        where = f"{path}: {f.get('name')}"
        ftype, fq = split_quals(f["type"], f.get("qualifiers"), where)
        sn = struct_name(ftype)
        if sn in tsizes and tsizes[sn] is not None and int(w) != tsizes[sn]:
            raise LayoutError(f"{where}: width {w} != sizeof({sn}) 0x{tsizes[sn]:X}")
        un = []
        for u in f.get("union") or []:
            uw = _member_width(u, tsizes, where, w)
            ut, uq = split_quals(u["type"], u.get("qualifiers"), where)
            un.append(UMember(u["name"], ut, int(uw), u.get("count"), uq))
        ov = []
        for u in f.get("overlay") or []:
            uw = _member_width(u, tsizes, where, None)
            if not uw:
                raise LayoutError(f"{path}: overlay {u['name']} on {f['name']} needs a width")
            ut, uq = split_quals(u["type"], u.get("qualifiers"), where)
            ov.append(UMember(u["name"], ut, int(uw), u.get("count"), uq))
        fields.append(Field(hexint(f["offset"]), f["name"], ftype, int(w), f.get("count"),
                            f.get("comment") or "", un, ov, fq))
    fields.sort(key=lambda x: x.offset)
    for a, b in zip(fields, fields[1:]):
        if b.offset < a.end:
            raise LayoutError(f"{path}: fields {a.name} (0x{a.offset:X}..0x{a.end:X}) and {b.name} (0x{b.offset:X}) "
                              f"overlap; overlapping roles go in `union`")
    names = [f.name for f in fields] + [u.name for f in fields for u in f.union]
    dup = {n for n in names if names.count(n) > 1}
    if dup:
        raise LayoutError(f"{path}: duplicate member names {sorted(dup)}")
    return fields


def parse_layout(d: dict, path: str = "") -> LayoutDoc:
    types, tsizes = [], {}
    for t in d.get("types") or []:          # in order: a type may use the ones before it
        st = SubType(t["name"], hexint(t.get("size")), parse_fields(t.get("fields"), f"{path}:{t['name']}", tsizes))
        if st.size is not None and st.fields and max(f.end for f in st.fields) > st.size:
            raise LayoutError(f"{path}:{st.name}: fields end at 0x{max(f.end for f in st.fields):X}, past size "
                              f"0x{st.size:X}")
        types.append(st)
        tsizes[st.name] = st.size
    fields = parse_fields(d.get("fields"), path, tsizes)
    size = hexint(d.get("size"))
    if size is not None and fields and fields[-1].end > size:
        raise LayoutError(f"{path}: field {fields[-1].name} ends at 0x{fields[-1].end:X}, past size 0x{size:X}")
    acc = [(g["accessor"], g["va"]) for g in d.get("globals") or [] if g.get("accessor")]
    return LayoutDoc(family=d["family"], client=d.get("client", ""), kind=d.get("kind", "struct"),
                     header=d.get("header") or "", size=size, pack=int(d.get("pack") or 8), base=d.get("base"),
                     base_size=hexint(d.get("base_size")), vtable=bool(d.get("vtable")),
                     aliases=list(d.get("aliases") or []), includes=list(d.get("includes") or ["common.h"]),
                     fields=fields, keep_cast_sites=list(d.get("keep_cast_sites") or []), raw=d, path=path,
                     types=types, methods=list(d.get("methods") or []), forward=list(d.get("forward") or []),
                     accessors=acc, prelude=list(d.get("prelude") or []))


# ------------------------------------------------------------------------------------------ formatting

@dataclass
class Fmt:
    nd: int = 3             # offset hex digits
    dec: bool = False       # array counts in decimal (character.h style) instead of 0x for >= 10


def odigits(limit: int) -> int:
    return max(3, len(f"{limit:X}"))


def ohex(v: int, nd: int) -> str:
    return f"0x{v:0{nd}X}"


def count_txt(n: int, dec: bool = False) -> str:
    return str(n) if (n < 10 or dec) else f"0x{n:X}"


def dims_txt(count, dec=False) -> str:
    if not count:
        return ""
    if isinstance(count, list):
        return "".join(f"[{count_txt(int(x), dec)}]" for x in count)
    return f"[{count_txt(int(count), dec)}]"


def decl_line(indent: str, typ: str, decl: str, off_txt: str, star: bool, comment: str) -> str:
    """`    u8      unknown0x004[0x44]; /* 0x004  */` / `    f32     floor_z;            /* 0x048* floor height */`"""
    t = typ + " " if len(typ) >= TYPE_COL else typ.ljust(TYPE_COL)
    d = decl + " " if len(decl) >= DECL_COL else decl.ljust(DECL_COL)
    tail = f"/* {off_txt}{'*' if star else ' '} {comment} */" if comment else f"/* {off_txt}{'*' if star else ' '} */"
    return f"{indent}{t}{d}{tail}"


def pad_lines(indent: str, start: int, n: int, fm, name_of=None) -> list:
    """name_of(offset) -> padding name (extend mode: the split padding's own spelling); default unknown0xNNN."""
    if isinstance(fm, int):
        fm = Fmt(fm)
    if n <= 0:
        return []
    name = name_of(start) if name_of else f"unknown{ohex(start, fm.nd)}"
    return [decl_line(indent, "u8", f"{name}[{count_txt(n, fm.dec)}];", ohex(start, fm.nd), False, "")]


def field_lines(indent: str, f: Field, fm) -> list:
    if isinstance(fm, int):
        fm = Fmt(fm)
    ot = ohex(f.offset, fm.nd)
    if not f.union:
        return [decl_line(indent, f.ctype, f"{f.name}{dims_txt(f.count, fm.dec)};", ot, True, f.comment)]
    inner = indent + "    "
    out = [f"{indent}union", f"{indent}{{"]
    out.append(decl_line(inner, f.ctype, f"{f.name}{dims_txt(f.count, fm.dec)};", ot, True, f.comment))
    for u in f.union:
        out.append(decl_line(inner, u.ctype, f"{u.name}{dims_txt(u.count, fm.dec)};", ot, True, ""))
    out.append(f"{indent}}};")
    return out


def assert_lines(family: str, fields: list, nd: int) -> list:
    return [f"C_ASSERT(offsetof({family}, {f.name}) == {ohex(f.offset, nd)});" for f in fields]


def dir_newline(directory: Path) -> str:
    crlf = lf = 0
    if directory.is_dir():
        for p in directory.glob("*.h"):
            b = p.read_bytes()
            if b"\n" not in b:
                continue
            if b"\r\n" in b:
                crlf += 1
            else:
                lf += 1
    return "\r\n" if crlf > lf else "\n"


# ------------------------------------------------------------------------------------------ verification

def _index_for(rel: str, text: str, repo: Path | None) -> C.Index:
    idx = C.Index()
    if repo is not None:
        hdir = (repo / rel).parent
        dirs = {hdir, hdir.parent} if hdir.name == "layout" else {hdir}
        for dd in dirs:
            if dd.is_dir():
                for p in sorted(dd.glob("*.h")):
                    r = p.relative_to(repo).as_posix()
                    if r != rel:
                        idx.add(C.parse_file(r, p.read_text(encoding="utf-8", errors="replace")))
    idx.add(C.parse_file(rel, text))
    idx.resolve_aliases()
    return idx


def nested_paths(prefix: str, off: int, typ: str, tmap: dict, depth: int = 0):
    """(dotted path, absolute offset) of every named member inside a by-value member of a `types` struct (and of
    the union / overlay roles there, recursively)."""
    st = tmap.get(struct_name(typ) or "")
    if st is None or depth > 8:
        return
    for g in st.fields:
        for nm, t, cnt in [(g.name, g.type, g.count)] + [(u.name, u.type, u.count) for u in g.union + g.overlay]:
            p = f"{prefix}.{nm}"
            yield p, off + g.offset
            if not cnt:
                yield from nested_paths(p, off + g.offset, t, tmap, depth + 1)


def check_nested(lay: C.Layout, doc: LayoutDoc, rel: str):
    """Struct-typed members (fields, union roles, overlays) of a `types` struct: every member path through them
    lands where the layout says (x->serialState_guess.file at 0x118 + 0x1C), and the cparse size of a struct
    role / overlay equals its width (a struct from another header included)."""
    tmap = {st.name: st for st in doc.types}
    have = {m.path: m.offset for m in lay.members}
    have.update({m.path: m.offset for m in lay.aggregates})
    agg = {m.path: m for m in lay.aggregates}
    for f in doc.fields:
        for nm, t, cnt, w in [(f.name, f.type, f.count, f.width)] + \
                [(u.name, u.type, u.count, u.width) for u in f.union + f.overlay]:
            if cnt:
                continue
            if nm in agg and struct_name(t) and agg[nm].size != w:
                raise LayoutError(f"{rel}: {doc.family}.{nm} ({t}) is 0x{agg[nm].size:X} bytes, the layout says "
                                  f"width 0x{w:X}")
            for p, o in nested_paths(nm, f.offset, t, tmap):
                if have.get(p) != o:
                    got = have.get(p)
                    raise LayoutError(f"{rel}: {doc.family}.{p} " + ("missing after generation" if got is None else
                                      f"lands at 0x{got:X}, expected 0x{o:X}"))


def verify(text: str, rel: str, doc: LayoutDoc, repo: Path | None, expect_size: int | None) -> C.Layout:
    """Lay the struct out with cparse (siblings of the header give the typedefs) and check every field offset."""
    idx = _index_for(rel, text, repo)
    for st in doc.types:
        cand = [d for d in idx.by_name.get(st.name, []) if d.file == rel] or \
            [d for d in idx.by_name.get(st.name, []) if d.parent is None and d.file in idx.included(rel)]
        if not cand:
            raise LayoutError(f"{rel}: type {st.name} not found after generation")
        sl = idx.layout(cand[0])
        for f in st.fields:
            got = sl.direct.get(f.name)
            if got is None or got[0] != f.offset:
                raise LayoutError(f"{rel}: {st.name}.{f.name} not at 0x{f.offset:X}")
        if st.size is not None and sl.size != st.size:
            raise LayoutError(f"{rel}: sizeof({st.name}) = 0x{sl.size:X}, expected 0x{st.size:X}")
    cands = [d for d in idx.by_name.get(doc.family, []) if d.file == rel]
    if not cands:
        raise LayoutError(f"{rel}: struct {doc.family} not found after generation")
    lay = idx.layout(cands[0])
    for f in doc.fields:
        for name in [f.name] + [u.name for u in f.union]:
            got = lay.direct.get(name)
            if got is None:
                raise LayoutError(f"{rel}: {doc.family}.{name} missing after generation")
            if got[0] != f.offset:
                raise LayoutError(f"{rel}: {doc.family}.{name} lands at 0x{got[0]:X}, expected 0x{f.offset:X} "
                                  f"(alignment under pack {cands[0].pack}?)")
    check_nested(lay, doc, rel)
    if expect_size is not None and lay.exact and lay.size != expect_size:
        raise LayoutError(f"{rel}: sizeof({doc.family}) = 0x{lay.size:X}, expected 0x{expect_size:X}")
    if expect_size is not None and not lay.exact:
        raise LayoutError(f"{rel}: layout of {doc.family} not exact: {lay.issues[:3]}")
    return lay


# ------------------------------------------------------------------------------------------ generate mode

def type_align(t: str, width: int, pack: int, local_align: dict) -> int:
    base, ptr = C.split_type(t)
    if ptr:
        a = 4
    elif base in local_align:
        a = local_align[base]
    elif base in C.WIN_STRUCTS:
        a = C.WIN_STRUCTS[base][1]
    elif base in C.BUILTIN:
        a = C.BUILTIN[base][0]
    else:
        # an unknown type (a struct from another header, a typedef): its width up to 4; wider ones align 4 (a
        # struct of dwords). cparse checks the real placement afterwards.
        a = width if width in (1, 2, 4) else 4
    return max(1, min(a, pack))


def natural_align(f: Field, pack: int, local_align: dict | None = None) -> int:
    la = local_align or {}
    a = type_align(f.type, f.width, pack, la)
    for u in f.union:
        a = max(a, type_align(u.type, u.width, pack, la))
    return a


def struct_body(fields, start, size, pack, fm, ind, where, local_align, is_union=False) -> tuple[list, int]:
    body = []
    cur = start
    maxa = 1
    fields = list(fields)
    i = 0
    while i < len(fields):
        f = fields[i]
        if f.overlay and not is_union:
            # an overlay view spans this field and the following ones it covers: union { struct {...}; views; }
            end = max(f.offset + u.size for u in f.overlay)
            j = i
            group = []
            while j < len(fields) and fields[j].offset < end:
                group.append(fields[j])
                end = max(end, fields[j].end)
                j += 1
            if f.offset % 4 and pack >= 4:
                raise LayoutError(f"{where}: overlay on {f.name} at 0x{f.offset:X} is not 4-aligned")
            body += pad_lines(ind, cur, f.offset - cur, fm)
            plain = [g.copy(overlay=[]) for g in group]
            inner, a = struct_body(plain, f.offset, max(g.end for g in group), pack, fm, ind + "        ", where,
                                   local_align)
            # the inner struct's padding is named from absolute offsets already; drop its leading pad (none)
            body += [f"{ind}union", f"{ind}{{", f"{ind}    struct", f"{ind}    {{"] + inner + [f"{ind}    }};"]
            for u in f.overlay:
                body.append(decl_line(ind + "    ", u.ctype, f"{u.name}{dims_txt(u.count, fm.dec)};",
                                      ohex(f.offset, fm.nd), True, "view"))
            body.append(f"{ind}}};")
            maxa = max(maxa, a, 4)
            cur = max(max(g.end for g in group), max(f.offset + u.size for u in f.overlay))
            i = j
            continue
        i += 1
        if f.offset < start:
            raise LayoutError(f"{where}: field {f.name} at 0x{f.offset:X} lies inside the base/vtable")
        a = natural_align(f, pack, local_align)
        maxa = max(maxa, a)
        if not is_union:
            if f.offset % a:
                raise LayoutError(f"{where}: field {f.name} at 0x{f.offset:X} is misaligned for its width under "
                                  f"pack {pack} (use pack 1 only with census evidence)")
            body += pad_lines(ind, cur, f.offset - cur, fm)
        body += field_lines(ind, f, fm)
        cur = f.end
    if size is not None and not is_union:
        body += pad_lines(ind, cur, size - cur, fm)
    return body, maxa


def subtype_lines(doc: LayoutDoc, st: SubType, local_align: dict) -> list:
    """`typedef struct T {...} T;` + its C_ASSERTs + a blank line, for one `types` entry (local_align gets its
    alignment, for the structs that use it by value)."""
    ind = "    "
    sfm = Fmt(odigits(st.size or (st.fields[-1].end if st.fields else 1)))
    body, a = struct_body(st.fields, 0, st.size, doc.pack, sfm, ind, f"{doc.path}:{st.name}", local_align)
    local_align[st.name] = a
    out = [f"typedef struct {st.name}", "{"] + body
    sz = f"/* size {ohex(st.size, sfm.nd)} */" if st.size is not None else "/* size unknown */"
    out.append(f"}} {st.name};".ljust(TYPE_COL + DECL_COL + 4) + sz)
    if st.size is not None:
        out.append(f"C_ASSERT(sizeof({st.name}) == {ohex(st.size, sfm.nd)});")
    out += assert_lines(st.name, st.fields, sfm.nd)
    out.append("")
    return out


def generate(doc: LayoutDoc, newline: str = "\n", source_name: str = "") -> str:
    fam = doc.family
    if doc.kind not in ("struct", "class", "union"):
        raise LayoutError(f"{doc.path}: kind {doc.kind!r}")
    start = 0
    if doc.base:
        if doc.base_size is None:
            raise LayoutError(f"{doc.path}: base {doc.base} needs base_size")
        start = doc.base_size
    if doc.vtable and not doc.base:
        if doc.fields and doc.fields[0].offset < 4:
            raise LayoutError(f"{doc.path}: vtable at 0 but a field starts at 0x{doc.fields[0].offset:X}")
        start = 4
    limit = doc.size if doc.size is not None else (doc.fields[-1].end if doc.fields else start)
    fm = Fmt(odigits(limit))
    ind = "    "
    local_align: dict = {}
    sub_out = []
    for st in doc.types:
        sub_out += subtype_lines(doc, st, local_align)
    body = []
    if doc.vtable and not doc.base:
        body.append(decl_line(ind, "void**", "vftable_guess;", ohex(0, fm.nd), True, "virtual function table"))
    b2, _ = struct_body(doc.fields, start, doc.size, doc.pack, fm, ind, doc.path, local_align, doc.kind == "union")
    body += b2
    guard = re.sub(r"[^A-Za-z0-9]", "_", doc.header or fam).upper().strip("_")
    guard = re.sub(r"^INCLUDE_", "", guard)
    src = source_name or (Path(doc.path).name if doc.path else "layout JSON")
    out = ["/*", f" * {Path(doc.header).name if doc.header else fam} - {fam}: {GEN_MARKER} from {src}.",
           " * Do not edit by hand: change the layout JSON and regenerate.", " */",
           f"#ifndef {guard}", f"#define {guard}", ""]
    for inc in doc.includes:
        out.append(f"#include {inc}" if inc.startswith("<") else f'#include "{inc}"')
    out.append("")
    if doc.prelude:
        out += list(doc.prelude) + [""]
    for fw in doc.forward:
        out.append(f"{fw};" if " " in fw else f"struct {fw};")
    if doc.forward:
        out.append("")
    if doc.pack != 8:
        out.append(f"#pragma pack(push, {doc.pack})")
        out.append("")
    out += sub_out
    if doc.kind == "class":
        out.append(f"class {fam}" + (f" : public {doc.base}" if doc.base else ""))
        out.append("{")
        out.append("public:")
        out += body
        if doc.methods:
            out.append("")
            out += [ind + m.strip() for m in doc.methods]
        sz = f"/* size {ohex(doc.size, fm.nd)} */" if doc.size is not None else "/* size unknown */"
        out.append("};".ljust(TYPE_COL + DECL_COL + 4) + sz)
    else:
        kw = "union" if doc.kind == "union" else "struct"
        if doc.base:
            out.append(f"{kw} {fam} : public {doc.base}")
            out.append("{")
            out += body
            out.append("};")
        else:
            out.append(f"typedef {kw} {fam}")
            out.append("{")
            out += body
            sz = f"/* size {ohex(doc.size, fm.nd)} */" if doc.size is not None else "/* size unknown */"
            out.append(f"}} {fam};".ljust(TYPE_COL + DECL_COL + 4) + sz)
    if doc.pack != 8:
        out.append("")
        out.append("#pragma pack(pop)")
    out.append("")
    if doc.size is not None:
        out.append(f"C_ASSERT(sizeof({fam}) == {ohex(doc.size, fm.nd)});")
    out += assert_lines(fam, doc.fields, fm.nd)
    if doc.aliases:
        out.append("")
        for a in doc.aliases:
            out.append(f"typedef {fam} {a};")
    if doc.accessors:
        out.append("")
        for name, va in doc.accessors:
            out.append(f"#define {name} EQ_GLOBAL({'class ' if doc.kind == 'class' else 'struct '}{fam} *, {va})")
    out += ["", f"#endif /* {guard} */", ""]
    return newline.join(out)


# ------------------------------------------------------------------------------------------ extend mode

_PAD_LINE = re.compile(r"^(?P<ind>[ \t]*)(?:u8|unsigned char|BYTE|char)[ \t]+(?P<name>[A-Za-z_]\w*)"
                       r"[ \t]*\[[ \t]*(?P<n>0[xX][0-9a-fA-F]+|\d+)[ \t]*\][ \t]*;[^\r\n]*$")


def pad_style(name: str, offset: int):
    """Spelling of a padding name that carries its own offset in hex: unknown0x04C -> ('unknown0x', 3, upper),
    unknown04c -> ('unknown', 3, lower). None when the name has no such offset (pad, reserved, gap...)."""
    for i in range(1, len(name)):
        pre, hx = name[:i], name[i:]
        if re.fullmatch(r"[0-9A-Fa-f]+", hx) and re.fullmatch(r"[A-Za-z_]\w*", pre) and int(hx, 16) == offset:
            return pre, len(hx), hx != hx.upper()
    return None


def style_name(style, offset: int) -> str:
    pre, nd, lower = style
    return f"{pre}{offset:0{nd}{'x' if lower else 'X'}}"
_MEMBER_LINE = re.compile(r"^(?P<ind>[ \t]*)(?P<body>[A-Za-z_][^;{}()]*;)(?P<tail>[ \t]*(?:/\*.*\*/)?[ \t]*)$")


def find_struct_span(text: str, rel: str, family: str):
    fi = C.parse_file(rel, text)
    for d in fi.structs:
        if family in d.names and d.parent is None:
            return fi, d
    raise LayoutError(f"{rel}: no top-level struct {family}")


def _src_files(repo: Path):
    for sd in ("src", "include"):
        base = repo / sd
        if not base.is_dir():
            continue
        for p in sorted(base.rglob("*")):
            if p.is_file() and p.suffix.lower() in (".c", ".cpp", ".h", ".hpp"):
                yield p


class _RefScope:
    """Which `->name` / `.name` references can be the family's padding (batch 2: rdp.h RdpState_guess
    unknown0x000 / unknown0x114 matched trilogy EverQuest.cpp / EQ_Slider.cpp members of unrelated structs).
      client: the referencing file must see the header (it is the header, or the header is in its include closure);
      struct: when another struct the file sees has a top-level member of that name, the base expression must type
              to the family (census typing of the enclosing function); an untyped base still counts (conservative)."""

    def __init__(self, repo: Path, header_rel: str, family: str, idx: C.Index | None = None):
        self.repo, self.header, self.family = repo, header_rel, family
        self._idx = idx
        self._fam = False
        self._others: dict = {}
        self._funcs: dict = {}

    @property
    def idx(self) -> C.Index:              # parsed on the first reference only
        if self._idx is None:
            self._idx = C.load_index(self.repo)
        return self._idx

    @property
    def fam(self):
        if self._fam is False:
            f = [d for d in self.idx.by_name.get(self.family, []) if d.file == self.header and d.parent is None]
            self._fam = f[0] if f else None
        return self._fam

    def sees_header(self, rel: str) -> bool:
        return rel == self.header or self.header in self.idx.included(rel)

    def others(self, rel: str, name: str) -> list:
        key = (rel, name)
        if key not in self._others:
            vis = self.idx.included(rel)
            self._others[key] = [d for d in self.idx.all_defs() if d.file in vis and d is not self.fam
                                 and name in self.idx.layout(d).direct]
        return self._others[key]

    def base_struct(self, rel: str, line: int, expr: str):
        """The struct the base expression `expr` (before `->` / `.`) designates, or None when it does not type."""
        import census as CE              # late import: census imports cparse only, but is heavy
        fi = self.idx.files.get(rel)
        if fi is None:
            return None
        if rel not in self._funcs:
            try:
                self._funcs[rel] = CE.find_functions(fi)
            except Exception:            # noqa: BLE001 - an odd file only loses the typing
                self._funcs[rel] = []
        for f in self._funcs[rel]:
            if fi.lines.line(f.brace) <= line <= fi.lines.line(f.end):
                try:
                    t = CE.Typer(self.idx, fi, CE.build_scope(fi, f, {}), {}, {}).walk(expr)
                except Exception:        # noqa: BLE001
                    return None
                return self.idx.find_struct(t[0], rel) if t is not None and t[1] + t[2] <= 1 else None
        return None

    def counts(self, rel: str, line: int, name: str, expr: str) -> bool:
        if not self.sees_header(rel):
            return False
        if rel == self.header or not self.others(rel, name) or self.fam is None:
            return True
        d = self.base_struct(rel, line, expr)
        return d is None or d is self.fam or (d.uid == self.fam.uid)


def padding_refs(repo: Path | None, names: set, exclude_rel: str, family: str | None = None,
                 scope: _RefScope | None = None) -> dict:
    """name -> [(rel, pos, line)] of member references (`->name` / `.name`) in src/ and include/ (comments masked).
    With `family`, only references that can be that struct's (see _RefScope)."""
    out: dict = {}
    if repo is None or not names:
        return out
    if family is not None and scope is None:
        scope = _RefScope(repo, exclude_rel, family)
    rx = re.compile(r"\b(" + "|".join(sorted(map(re.escape, names))) + r")\b")
    for p in _src_files(repo):
        rel = p.relative_to(repo).as_posix()
        text = read_text_raw(p, "latin-1")
        if not rx.search(text):
            continue
        if scope is not None and not scope.sees_header(rel):
            continue
        mk = C.mask(text)
        li = C.LineIndex(text)
        for m in rx.finditer(mk):
            if rel == exclude_rel:
                ln = text[text.rfind("\n", 0, m.start()) + 1:text.find("\n", m.start())]
                if _PAD_LINE.match(ln.rstrip("\r")) or "C_ASSERT" in ln:
                    continue
            prev = mk[max(0, m.start() - 3):m.start()]
            if not re.search(r"(->|\.)\s*$", prev):
                continue
            line = li.line(m.start())
            if scope is not None:
                base = re.sub(r"\s*(->|\.)\s*$", "", mk[_base_start(mk, m.start()):m.start()])
                if not scope.counts(rel, line, m.group(1), " ".join(base.split())):
                    continue
            out.setdefault(m.group(1), []).append((rel, m.start(), line))
    return out


class _Canon:
    def __init__(self, idx):
        import propagate as P          # late import: propagate imports this module
        self.t = P.Types(idx)

    def __call__(self, t):
        return self.t.canon(t)


def _base_start(mk: str, pos: int) -> int:
    """Start of the postfix base expression that ends right before `->name` at pos (pos = start of name)."""
    k = pos
    while k > 0 and mk[k - 1] in " \t":
        k -= 1
    if mk[k - 2:k] == "->":
        k -= 2
    elif mk[k - 1:k] == ".":
        k -= 1
    # now walk back over a postfix chain: ident / ) ... ( / ] ... [ / -> / .
    while True:
        while k > 0 and mk[k - 1] in " \t":
            k -= 1
        if k == 0:
            return 0
        c = mk[k - 1]
        if c in ")]":
            o = C.match_open(mk, k - 1)
            if o < 0:
                return k
            k = o
            # a call: ident before (
            m = re.search(r"[A-Za-z_]\w*\s*$", mk[:k])
            if c == ")" and m:
                k = m.start()
        elif re.match(r"\w", c):
            m = re.search(r"[A-Za-z_]\w*$", mk[:k])
            k = m.start() if m else k - 1
        else:
            return k
        kk = k
        while kk > 0 and mk[kk - 1] in " \t":
            kk -= 1
        if mk[kk - 2:kk] == "->":
            k = kk - 2
        elif mk[kk - 1:kk] == "." and not (kk >= 2 and mk[kk - 2].isdigit()):
            k = kk - 1
        else:
            return k


def plan_ref_rewrites(repo, refs: dict, pads: dict, newmap, canon, allow=()):
    """refs: name -> [(rel, pos, line)]; pads: name -> (offset, size, keeps_name_size or None);
    newmap(off) -> (member_name, member_type, elem_index or None, is_pad_piece, piece_size) covering off.
    Returns (edits {rel: [(a, b, new)]}, notes, errors)."""
    edits: dict = {}
    notes, errs = [], []
    cache = {}

    class _Err(list):
        pass
    errors = _Err()
    for name, occ in refs.items():
        poff, psize, kept = pads[name]
        for rel, pos, line in occ:
            if rel not in cache:
                t = read_text_raw(repo / rel, "latin-1")
                cache[rel] = (t, C.mask(t))
            text, mk = cache[rel]
            loc = f"{rel}:{line}"
            after = mk[pos + len(name):]
            mi = re.match(r"\s*\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\]", after)
            if mi:
                k = int(mi.group(1), 0)
                off = poff + k
                mem, mtype, eidx, is_piece, piece_size = newmap(off)
                if mem is None:
                    errors.append(f"{loc}: {name}[{mi.group(1)}] lands on no member")
                    continue
                if is_piece:
                    if mem == name and eidx == k:
                        continue
                    new = f"{mem}[0x{eidx:X}]" if eidx >= 10 else f"{mem}[{eidx}]"
                else:
                    if canon(mtype) != canon("u8"):
                        errors.append(f"{loc}: {name}[{mi.group(1)}] (u8) vs {mem} {mtype}")
                        continue
                    new = f"{mem}[{eidx}]" if eidx is not None else mem
                edits.setdefault(rel, []).append((pos, pos + len(name) + mi.end(), new, loc))
                continue
            # *(T*)base->name
            bs = _base_start(mk, pos)
            md = re.search(r"\*\s*\(\s*([A-Za-z_][\w \t]*?(?:\s*\*)+)\s*\)\s*$", mk[:bs])
            nxt = re.match(r"\s*([^\s])", after)
            nxt_c = nxt.group(1) if nxt else ""
            if md and nxt_c not in ("[", "+", "-") and not mk[:md.start()].rstrip().endswith((")",)):
                tb, tp = C.split_type(md.group(1))
                acc = C.fmt_type(tb, tp - 1)
                mem, mtype, eidx, is_piece, _ = newmap(poff)
                if mem is not None and not is_piece and eidx is None and canon(mtype) == canon(acc):
                    edits.setdefault(rel, []).append((md.start(), pos + len(name), text[bs:pos] + mem, loc))
                    continue
                errors.append(f"{loc}: *({md.group(1).strip()}){name} does not map to a new member of that type")
                continue
            if kept:
                notes.append(f"{loc}: address use of {name} left (piece keeps name and start, {kept} bytes)")
                continue
            errors.append(f"{loc}: reference to {name} is not a constant index or typed deref")
        for e in errors:
            (notes if name in allow else errs).append(e + (" (left: --allow-padding-ref)" if name in allow else ""))
        errors.clear()
    return edits, notes, errs


def extend(text: str, rel: str, doc: LayoutDoc, repo: Path | None = None, allow_refs=(), log=None,
           src_edits_out: dict | None = None):
    """Return (new_text, added_fields). Raises LayoutError. Source edits for padding references go to
    src_edits_out {rel: [(a, b, new, loc)]} (applied by the caller together with the header)."""
    log = log if log is not None else []
    idx = _index_for(rel, text, repo)
    d = [x for x in idx.by_name.get(doc.family, []) if x.file == rel and x.parent is None]
    if not d:
        raise LayoutError(f"{rel}: no top-level struct {doc.family}")
    d = d[0]
    lay = idx.layout(d)
    old_size = lay.size if lay.exact else None
    # top-level members only: the flattened sub-members of a by-value struct member (`RdpConnHash_guess hash` ->
    # hash.count ...) belong to that other struct's definition, never to this one's lines
    top = [m for m in lay.members if "." not in m.path]
    fm = Fmt()
    style0 = None                       # the struct's first padding spelling that carries its offset
    for m in top:
        if m.is_pad:
            st = pad_style(m.name, m.offset)
            if st:
                style0 = st
                fm.nd = st[1]
                break
    lines = text.split("\n")            # keeps \r at line ends when CRLF
    pad_counts = [re.search(r"\[\s*(0[xX][0-9a-fA-F]+|\d+)\s*\]", lines[m.line - 1]) for m in top
                  if m.is_pad and m.size >= 10 and 0 < m.line <= len(lines)]
    pad_counts = [x.group(1) for x in pad_counts if x]
    fm.dec = bool(pad_counts) and all(not c.lower().startswith("0x") for c in pad_counts)
    nl = "\r\n" if "\r\n" in text else "\n"

    existing = {m.name: m for m in top}
    pads = [m for m in top if m.is_pad]
    new_fields, wraps = [], []
    for f in doc.fields:
        ex = existing.get(f.name)
        if ex is None and f.name in lay.direct:
            # an existing by-value struct (or other non-flat) member: lay.direct has it, the flat list only its parts
            off = lay.direct[f.name][0]
            if off != f.offset:
                raise LayoutError(f"{rel}: {doc.family}.{f.name} exists at 0x{off:X}, JSON says 0x{f.offset:X}")
            missing = [u for u in f.union if u.name not in lay.direct]
            if missing:
                # a by-value struct member (`RdpSerialQueue_guess serial;`): its one declaration line is wrapped
                agg = [m for m in lay.aggregates if m.path == f.name]
                if not agg or not agg[0].line:
                    raise LayoutError(f"{rel}: {doc.family}.{f.name} is a by-value member whose declaration line "
                                      f"is not known; adding union roles to it is not supported")
                wraps.append((f, agg[0], missing))
            continue
        if ex is not None:
            if ex.offset != f.offset:
                raise LayoutError(f"{rel}: {doc.family}.{f.name} exists at 0x{ex.offset:X}, JSON says 0x{f.offset:X}")
            missing = [u for u in f.union if u.name not in lay.direct]
            if missing:
                wraps.append((f, ex, missing))
            continue
        new_fields.append(f)
    plan: dict = {}           # pad member name -> [(offset, end, lines or None)]
    added = []

    def host_of(a, b):
        h = [p for p in pads if p.offset <= a and b <= p.offset + p.size]
        return h[0] if h else None
    for f in new_fields:
        if f.overlay:
            raise LayoutError(f"{rel}: {doc.family}.{f.name} has an `overlay` view, which extend mode does not "
                              f"install (generate the header, or use a union role over padding)")
        host = host_of(f.offset, f.end)
        if host is None:
            clash = [m.name for m in lay.members if not m.is_pad and m.offset < f.end and f.offset < m.offset + m.size]
            raise LayoutError(f"{rel}: new field {f.name} (0x{f.offset:X}..0x{f.end:X}) is not inside one padding "
                              f"member" + (f"; overlaps {clash}" if clash else ""))
        plan.setdefault(host.name, []).append((f.offset, f.end, f))
        added.append(f)
    wrap_edits = []
    for f, ex, missing in wraps:
        span_end = ex.offset + max([ex.size] + [u.size for u in missing])
        if span_end > ex.offset + ex.size:
            host = host_of(ex.offset + ex.size, span_end)
            if host is None or host.offset != ex.offset + ex.size:
                raise LayoutError(f"{rel}: union role of {f.name} needs 0x{span_end - ex.offset - ex.size:X} bytes "
                                  f"after it, but no padding starts at 0x{ex.offset + ex.size:X}")
            plan.setdefault(host.name, []).append((ex.offset + ex.size, span_end, None))
        wrap_edits.append((ex, missing))
    vis = idx.included(rel)
    new_types = [st for st in doc.types
                 if not [x for x in idx.by_name.get(st.name, []) if x.parent is None and x.file in vis]]
    if not plan and not wrap_edits and not new_types:
        return text, []
    # padding pieces after the split: name -> (offset, size)
    pad_by_name = {p.name: p for p in pads}
    default_style = style0 or ("unknown0x", fm.nd, False)
    namer = {p.name: (lambda o, st=(pad_style(p.name, p.offset) or default_style): style_name(st, o)) for p in pads}
    pieces = {}               # new pad name -> (offset, size)
    for p in pads:
        if p.name not in plan:
            continue
        cur = p.offset
        for a, b, _ in sorted(plan[p.name], key=lambda x: x[0]):
            if a > cur:
                pieces[namer[p.name](cur)] = (cur, a - cur)
            cur = b
        if p.offset + p.size > cur:
            pieces[namer[p.name](cur)] = (cur, p.offset + p.size - cur)
    # references to split padding in the sources (only those that can be this struct's, see _RefScope)
    scope = _RefScope(repo, rel, doc.family) if repo is not None else None
    refs = padding_refs(repo, set(plan), rel, doc.family, scope)
    if refs:
        canon = _Canon(_index_for(rel, text, repo) if repo is None else scope.idx)

        def newmap(off):
            for f in added:
                if f.offset <= off < f.offset + f.own_size:
                    if f.count:
                        es = f.width
                        n = nelem(f.count)
                        if isinstance(f.count, list) and len(f.count) > 1:
                            return None, None, None, False, 0
                        return f.name, f.type, (off - f.offset) // es if (off - f.offset) % es == 0 else None, False, n
                    return (f.name, f.type, None, False, 0) if off == f.offset else (None, None, None, False, 0)
            for nm, (po, ps) in pieces.items():
                if po <= off < po + ps:
                    return nm, "u8", off - po, True, ps
            return None, None, None, False, 0
        pinfo = {}
        for nm in refs:
            p = pad_by_name[nm]
            kept = pieces.get(nm)
            pinfo[nm] = (p.offset, p.size, kept[1] if kept and kept[0] == p.offset else None)
        edits, notes, errors = plan_ref_rewrites(repo, refs, pinfo, newmap, canon, allow_refs)
        if errors:
            raise LayoutError(f"{rel}: padding referenced by name cannot be split: " + "; ".join(errors[:8]) +
                              (" ..." if len(errors) > 8 else "") + " (rewrite those sites first, leave the field "
                              "out, or --allow-padding-ref NAME)")
        log.extend(notes)
        if src_edits_out is not None:
            for r, ee in edits.items():
                src_edits_out.setdefault(r, []).extend(ee)
        for r, ee in edits.items():
            for a, b, new, loc in ee:
                log.append(f"  ref {loc}: -> {new}")
    edits_l = []
    for pname, fl in plan.items():
        p = pad_by_name[pname]
        li = p.line - 1
        raw = lines[li]
        cr = "\r" if raw.endswith("\r") else ""
        m = _PAD_LINE.match(raw.rstrip("\r"))
        if not m or m.group("name") != pname:
            raise LayoutError(f"{rel}:{p.line}: padding {pname} is not a one-line `u8 {pname}[N];` declaration")
        ind = m.group("ind")
        pc = re.search(r"/\*\s*0x[0-9A-Fa-f]+\s+(.*?)\s*\*/", raw)
        pad_comment = pc.group(1) if pc and pc.group(1) else ""
        cur = p.offset
        new = []
        for a, b, f in sorted(fl, key=lambda x: x[0]):
            new += pad_lines(ind, cur, a - cur, fm, namer[pname])
            if f is not None:
                if pad_comment and a == p.offset and not f.comment:
                    f = f.copy(comment=pad_comment)
                    pad_comment = ""
                new += field_lines(ind, f, fm)
            cur = b
        new += pad_lines(ind, cur, p.offset + p.size - cur, fm, namer[pname])
        edits_l.append((li, [x + cr for x in new]))
    for ex, missing in wrap_edits:
        li = ex.line - 1
        raw = lines[li]
        cr = "\r" if raw.endswith("\r") else ""
        m = _MEMBER_LINE.match(raw.rstrip("\r"))
        if not m or not re.search(r"\b" + re.escape(ex.name) + r"\b", m.group("body")):
            raise LayoutError(f"{rel}:{ex.line}: member {ex.name} is not a one-line declaration")
        ind = m.group("ind")
        ot = ohex(ex.offset, fm.nd)
        new = [f"{ind}union", f"{ind}{{", "    " + raw.rstrip("\r")]
        for u in missing:
            new.append(decl_line(ind + "    ", u.ctype, f"{u.name}{dims_txt(u.count, fm.dec)};", ot, True, ""))
        new.append(f"{ind}}};")
        edits_l.append((li, [x + cr for x in new]))
    for li, repl in sorted(edits_l, reverse=True):
        lines[li:li + 1] = repl
    text2 = "\n".join(lines)
    fi2, d2 = find_struct_span(text2, rel, doc.family)
    lines = text2.split("\n")
    end_li = d2.end_line - 1
    rx = re.compile(r"^\s*C_ASSERT\s*\(\s*(?:offsetof|sizeof)\s*\(\s*" + re.escape(doc.family) + r"\b")
    last = None
    i = end_li + 1
    while i < len(lines):
        s = lines[i].rstrip("\r")
        if rx.match(s):
            last = i
        elif s.strip() and last is not None:
            break
        elif s.strip() and not s.lstrip().startswith(("C_ASSERT", "}")):
            break
        i += 1
    at = (last if last is not None else end_li) + 1
    cr = "\r" if nl == "\r\n" else ""
    added = sorted(added, key=lambda x: x.offset)
    lines[at:at] = [x + cr for x in assert_lines(doc.family, added, fm.nd)]
    text3 = "\n".join(lines)
    if new_types:
        text3 = insert_types(text3, rel, doc, new_types, cr)
    verify(text3, rel, doc, repo, old_size)
    log.insert(0, f"{rel}: {doc.family}: +{len(added)} field(s) in {len(plan)} padding range(s), "
                  f"{len(wrap_edits)} union wrap(s)" +
                  (f", type(s) {', '.join(st.name for st in new_types)} added" if new_types else ""))
    return text3, added


def insert_types(text: str, rel: str, doc: LayoutDoc, new_types: list, cr: str) -> str:
    """Extend mode: the layout's `types` the header does not define yet go right before the family's definition
    (above the comment block that heads it). A type that uses, by value, a struct the header defines only later
    is an error (the C compiler needs the complete type first)."""
    fi, d = find_struct_span(text, rel, doc.family)
    lines = text.split("\n")
    mk = C.mask(text)
    lstart = sum(len(x) + 1 for x in lines[:d.line - 1])
    brace = mk.find("{", lstart)
    m = re.search(r"(?:\btypedef\s+)?\b(?:struct|union|class)\b(?:\s+[A-Za-z_]\w*)?\s*$", mk[:brace])
    at = mk.count("\n", 0, m.start()) if m else d.line - 1     # the line of `typedef struct X`
    while at > 0:
        s = lines[at - 1].rstrip("\r").strip()
        if s.startswith(("/*", "*", "//")) or s.endswith("*/"):
            at -= 1
            continue
        break
    defined_at = {}
    for x in fi.structs:
        if x.parent is None:
            for n in x.names:
                defined_at.setdefault(n, x.line)
    own = {st.name for st in new_types}
    for st in new_types:
        for g in st.fields:
            for t, cnt in [(g.type, g.count)] + [(u.type, u.count) for u in g.union + g.overlay]:
                sn = struct_name(t)
                if sn and sn not in own and defined_at.get(sn, 0) > at:
                    raise LayoutError(f"{rel}: type {st.name} uses {sn} by value, but {rel} defines {sn} at line "
                                      f"{defined_at[sn]}, after {doc.family}: move it up first")
    local_align: dict = {}
    block = []
    for st in new_types:
        block += subtype_lines(doc, st, local_align)
    lines[at:at] = [x + cr for x in block]
    return "\n".join(lines)


# ------------------------------------------------------------------------------------------ CLI

def read_text_raw(p: Path, enc: str = "utf-8") -> str:
    with open(p, "r", encoding=enc, newline="") as fh:
        return fh.read()


def write_text_raw(p: Path, text: str, enc: str = "utf-8"):
    p.parent.mkdir(parents=True, exist_ok=True)
    with open(p, "w", encoding=enc, newline="") as fh:
        fh.write(text)


def apply_src_edits(repo: Path, edits: dict):
    for rel, ee in edits.items():
        p = repo / rel
        t = read_text_raw(p, "latin-1")
        last = None
        for a, b, new, loc in sorted(ee, key=lambda x: x[0], reverse=True):
            if last is not None and b > last:
                raise LayoutError(f"{loc}: overlapping source edits")
            t = t[:a] + new + t[b:]
            last = a
        write_text_raw(p, t, "latin-1")


def run_one(path: str, repo: Path, do_extend: bool, header: str | None, dry: bool, allow_refs, log) -> int:
    doc = load_layout(path)
    rel = header or doc.header
    if not rel:
        raise LayoutError(f"{path}: no header given")
    target = repo / rel
    if do_extend:
        if not target.is_file():
            raise LayoutError(f"{rel}: --extend needs an existing header")
        text = read_text_raw(target)
        src_edits: dict = {}
        new, added = extend(text, rel, doc, repo, allow_refs, log, src_edits)
        for f in added:
            log.append(f"  + 0x{f.offset:X} {f.type} {f.name}" + "".join(f" | {u.type} {u.name}" for u in f.union))
        if new != text and not dry:
            write_text_raw(target, new)
            apply_src_edits(repo, src_edits)
        return len(added)
    if target.is_file() and GEN_MARKER not in read_text_raw(target):
        raise LayoutError(f"{rel}: exists and is not generated; use --extend or pick another header path")
    nl = dir_newline(target.parent)
    if not any(target.parent.glob("*.h")) if target.parent.is_dir() else True:
        nl = dir_newline(target.parent.parent) if target.parent.name == "layout" else nl
    if target.is_file():
        old = read_text_raw(target)
        nl = "\r\n" if "\r\n" in old else "\n"
    text = generate(doc, nl, Path(path).name)
    verify(text, rel, doc, repo, doc.size)
    log.append(f"{rel}: {doc.family} generated, {len(doc.fields)} field(s), newline {'CRLF' if nl == chr(13) + chr(10) else 'LF'}")
    if not dry:
        write_text_raw(target, text)
    return len(doc.fields)


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("layouts", nargs="+")
    ap.add_argument("--repo", required=True)
    ap.add_argument("--extend", action="store_true", help="add fields to an existing struct in place")
    ap.add_argument("--header", help="override the JSON's header path (repo-relative)")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--allow-padding-ref", action="append", default=[])
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    rc = 0
    for p in a.layouts:
        log: list = []
        try:
            run_one(p, repo, a.extend, a.header, a.dry_run, a.allow_padding_ref, log)
        except LayoutError as ex:
            print(f"ERROR {p}: {ex}", file=sys.stderr)
            rc = 1
        for line in log:
            print(line)
    return rc


if __name__ == "__main__":
    sys.exit(main())
