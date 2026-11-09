r"""Light C/C++ declaration parser shared by the struct-layout tools (census.py, unify_diff.py, layout_check.py).

Not a compiler front end: a bracket-matching, regex-driven reader that is good enough for this code base's
`typedef struct X_guess { ... } X_guess;` style. What it does:

  * masks comments and string/char literals (same length, newlines kept) so offsets and line numbers hold
  * finds struct/union definitions (nested, anonymous, typedef'd, forward typedefs), enums, simple typedefs,
    object-like numeric #defines, `#pragma pack` state per definition, #include "..." lists
  * computes MSVC layouts: natural alignment capped by the pack in effect (default /Zp8), arrays, nested
    structs/unions, bitfields (approximate: same-size neighbours share a unit), unknown types flagged
    (assumed 4 bytes so later offsets stay usable, Layout.exact = False)
  * a tiny expression typer (identifier chains with ->, ., [], casts, EQ_GLOBAL/EQ_ARRAY)

Importable only; no CLI.
"""
from __future__ import annotations

import bisect
import re
from dataclasses import dataclass, field
from pathlib import Path

DEFAULT_PACK = 8

# --------------------------------------------------------------------------------------------- text masks

_MASK_RX = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)


def mask(text: str) -> str:
    """Comments -> spaces, string/char literal contents -> spaces (quotes kept). Same length, newlines kept."""
    def rep(m):
        s = m.group(0)
        if s[0] in "\"'":
            return s[0] + re.sub(r"[^\n]", " ", s[1:-1]) + s[-1] if len(s) >= 2 else s
        return re.sub(r"[^\n]", " ", s)
    return _MASK_RX.sub(rep, text)


def blank_preproc(masked: str) -> str:
    """Preprocessor lines (with backslash continuations) -> spaces."""
    out = []
    cont = False
    for ln in masked.split("\n"):
        if cont or ln.lstrip().startswith("#"):
            cont = ln.rstrip("\r").rstrip().endswith("\\")
            out.append(re.sub(r"[^\r]", " ", ln))
        else:
            out.append(ln)
    return "\n".join(out)


def match_close(s: str, i: int) -> int:
    """Index of the bracket closing the one at s[i] (len(s) when unbalanced). Handles ( [ {."""
    pairs = {"(": ")", "[": "]", "{": "}"}
    stack = []
    n = len(s)
    j = i
    while j < n:
        c = s[j]
        if c in pairs:
            stack.append(pairs[c])
        elif c in ")]}":
            if stack and stack[-1] == c:
                stack.pop()
                if not stack:
                    return j
            elif stack:
                pass
        j += 1
    return n


def match_open(s: str, i: int) -> int:
    """Index of the bracket opening the one closing at s[i] (-1 when unbalanced)."""
    pairs = {")": "(", "]": "[", "}": "{"}
    stack = []
    j = i
    while j >= 0:
        c = s[j]
        if c in pairs:
            stack.append(pairs[c])
        elif c in "([{":
            if stack and stack[-1] == c:
                stack.pop()
                if not stack:
                    return j
        j -= 1
    return -1


def split_top(s: str, seps: str) -> list[tuple[str, str]]:
    """Split on top-level single-char separators. Returns [(sep_before, piece)] (first sep is '')."""
    out, cur, depth, sep = [], [], 0, ""
    for c in s:
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if depth == 0 and c in seps:
            out.append((sep, "".join(cur)))
            cur, sep = [], c
            continue
        cur.append(c)
    out.append((sep, "".join(cur)))
    return out


class LineIndex:
    def __init__(self, text: str):
        self.starts = [0]
        for m in re.finditer(r"\n", text):
            self.starts.append(m.end())

    def line(self, pos: int) -> int:
        return bisect.bisect_right(self.starts, pos)


# ------------------------------------------------------------------------------------------------ types

# canonical builtin spelling -> (size, kind); kind: int | float | ptr
BUILTIN = {
    "char": (1, "int"), "signed char": (1, "int"), "unsigned char": (1, "int"), "bool": (1, "int"),
    "short": (2, "int"), "short int": (2, "int"), "unsigned short": (2, "int"), "unsigned short int": (2, "int"),
    "signed short": (2, "int"), "wchar_t": (2, "int"),
    "int": (4, "int"), "unsigned": (4, "int"), "unsigned int": (4, "int"), "signed": (4, "int"),
    "signed int": (4, "int"), "long": (4, "int"), "long int": (4, "int"), "unsigned long": (4, "int"),
    "unsigned long int": (4, "int"), "signed long": (4, "int"),
    "float": (4, "float"), "double": (8, "float"), "long double": (8, "float"),
    "__int64": (8, "int"), "unsigned __int64": (8, "int"), "long long": (8, "int"),
    "unsigned long long": (8, "int"), "signed __int64": (8, "int"),
    "u8": (1, "int"), "s8": (1, "int"), "BYTE": (1, "int"), "UCHAR": (1, "int"), "CHAR": (1, "int"),
    "BOOLEAN": (1, "int"), "byte": (1, "int"), "uint8_t": (1, "int"), "int8_t": (1, "int"),
    "u16": (2, "int"), "s16": (2, "int"), "WORD": (2, "int"), "USHORT": (2, "int"), "SHORT": (2, "int"),
    "uint16_t": (2, "int"), "int16_t": (2, "int"), "WCHAR": (2, "int"),
    "u32": (4, "int"), "s32": (4, "int"), "DWORD": (4, "int"), "UINT": (4, "int"), "INT": (4, "int"),
    "LONG": (4, "int"), "ULONG": (4, "int"), "BOOL": (4, "int"), "size_t": (4, "int"), "uint32_t": (4, "int"),
    "int32_t": (4, "int"), "HRESULT": (4, "int"), "COLORREF": (4, "int"), "FLOAT": (4, "float"),
    "u64": (8, "int"), "s64": (8, "int"), "LONGLONG": (8, "int"), "ULONGLONG": (8, "int"),
    "DWORDLONG": (8, "int"), "uint64_t": (8, "int"), "int64_t": (8, "int"), "DOUBLE": (8, "float"),
}
# Windows SDK structs the headers use by value: (size, align)
WIN_STRUCTS = {"RECT": (16, 4), "POINT": (8, 4), "SIZE": (8, 4), "GUID": (16, 4), "FILETIME": (8, 4),
               "SYSTEMTIME": (16, 2), "LARGE_INTEGER": (8, 8), "ULARGE_INTEGER": (8, 8), "RGBQUAD": (4, 1),
               "CRITICAL_SECTION": (24, 4), "OVERLAPPED": (20, 4), "RECTL": (16, 4), "POINTL": (8, 4),
               "SIZEL": (8, 4), "LOGFONT": (60, 4), "LOGFONTA": (60, 4), "BITMAPINFOHEADER": (40, 4),
               "PALETTEENTRY": (4, 1), "POINTS": (4, 2), "va_list": (4, 4), "CLSID": (16, 4), "IID": (16, 4)}
_PTR_NAME = re.compile(r"^(?:LP[A-Z0-9_]+|P[A-Z][A-Z0-9_]+|H[A-Z0-9]+|PVOID|LPVOID|FARPROC|WPARAM|LPARAM)$")
_QUAL = re.compile(r"\b(?:const|volatile|register|static|extern|inline|__inline|__forceinline|struct|union|enum|class|"
                   r"__cdecl|__stdcall|__fastcall|__thiscall|WINAPI|CALLBACK|far|near)\b")
_DECLSPEC = re.compile(r"__declspec\s*\([^)]*\)")


def split_type(t: str) -> tuple[str, int]:
    """'unsigned char *const *' -> ('unsigned char', 2). Qualifiers/struct keywords dropped."""
    t = _DECLSPEC.sub(" ", t)
    ptr = t.count("*")
    t = _QUAL.sub(" ", t.replace("*", " "))
    base = " ".join(t.split())
    return base, ptr


def fmt_type(base: str, ptr: int) -> str:
    return base + ("*" * ptr if ptr else "")


def norm_type(t: str) -> str:
    return fmt_type(*split_type(t))


def has_qual(t: str, q: str) -> bool:
    return re.search(r"\b" + q + r"\b", t) is not None


def is_byte_ptr(t: str) -> bool:
    base, ptr = split_type(t)
    return ptr == 1 and base in ("char", "unsigned char", "signed char", "u8", "s8", "BYTE", "UCHAR", "byte",
                                 "CHAR", "void", "uint8_t", "int8_t")


# ------------------------------------------------------------------------------------- parsed structures

@dataclass
class RawMember:
    name: str | None            # None = anonymous inline struct/union
    type: str                   # base type text (no declarator stars), e.g. 'unsigned char' / 'EQPlayer *'
    ptr: int = 0                # pointer levels of this declarator
    dims: list = field(default_factory=list)       # array dimension expression strings
    bits: str | None = None
    inline: "StructDef | None" = None
    funcptr: bool = False
    line: int = 0


@dataclass
class StructDef:
    tag: str | None
    kind: str                   # struct | union
    file: str                   # repo-relative path
    line: int
    pack: int
    raw: list = field(default_factory=list)
    aliases: list = field(default_factory=list)   # typedef names
    parent: "StructDef | None" = None
    conditional: bool = False   # body contained #if/#else
    end_line: int = 0
    fi: "FileInfo | None" = None
    uid: int = 0
    bases: list = field(default_factory=list)
    has_virtual: bool = False
    kw: str = "struct"      # keyword as written: struct | union | class

    @property
    def name(self) -> str:
        if self.tag and self.aliases and self.tag not in self.aliases:
            return self.aliases[0] if self.tag.startswith("_") else self.tag
        return self.tag or (self.aliases[0] if self.aliases else f"<anon@{self.file}:{self.line}>")

    @property
    def names(self) -> list:
        return [n for n in ([self.tag] if self.tag else []) + self.aliases if n]


@dataclass
class Member:
    name: str
    path: str                   # dotted path from the struct (flattened nested members)
    type: str                   # as written (with stars)
    offset: int
    size: int
    align: int
    dims: list = field(default_factory=list)    # evaluated ints (None where unknown)
    bits: int | None = None
    bit_offset: int | None = None
    kind: str = "int"           # int | float | ptr | struct | unknown
    elem_size: int = 0
    unknown_name: bool = False  # name looks like a placeholder
    is_pad: bool = False        # placeholder byte array (a gap that hides fields)
    guessed: bool = False       # size assumed (unknown type)
    line: int = 0
    volatile: bool = False      # declared volatile (or inside a volatile by-value struct member)


def _moved(m: "Member", delta: int, path: str, volatile: bool) -> "Member":
    """A copy of member m `delta` bytes further on, under `path`."""
    return Member(m.name, path, m.type, m.offset + delta, m.size, m.align, m.dims, m.bits, m.bit_offset, m.kind,
                  m.elem_size, m.unknown_name, m.is_pad, m.guessed, m.line, volatile)


@dataclass
class Layout:
    name: str
    kind: str
    size: int
    align: int
    pack: int
    members: list
    direct: dict                # name -> (offset, type text, dims) for direct (and anonymous-flattened) members
    exact: bool = True
    issues: list = field(default_factory=list)
    file: str = ""
    line: int = 0
    # by-value struct/union members that have a name (`serial`, `u.sub`), with their dotted path and type: the
    # whole-aggregate view, which `members` (scalars only) flattens away. Nested ones included.
    aggregates: list = field(default_factory=list)


def looks_unknown(name: str) -> bool:
    """Placeholder-style name: unknown0x10, unused_04, pad8, field34, reserved, gap..."""
    n = name.lstrip("_")
    if re.match(r"(?i)^(unknown|unk|unused|pad|padding|reserved|filler|gap|dummy|undef)", n):
        return True
    return re.match(r"(?i)^field_?(0x)?[0-9a-f]+$", n) is not None


# ---------------------------------------------------------------------------------------------- file info

_PACK_RX = re.compile(r"^[ \t]*#[ \t]*pragma[ \t]+pack[ \t]*\(([^)\n]*)\)", re.M)
_DEFINE_RX = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)(\([^)\n]*\))?[ \t]*([^\n]*)", re.M)
_INCLUDE_RX = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"([^"\n]+)"', re.M)
_CPP_COND = re.compile(r"^[ \t]*#[ \t]*(?:if|ifdef|ifndef|elif|else|endif)\b", re.M)


@dataclass
class FileInfo:
    rel: str
    text: str
    masked: str                 # comments/strings masked
    code: str                   # + preprocessor lines blanked
    lines: LineIndex
    pack_events: list = field(default_factory=list)       # [(pos, pack_after)] precomputed pack state
    defines: dict = field(default_factory=dict)           # object-like: name -> body
    funcmacros: dict = field(default_factory=dict)
    includes: list = field(default_factory=list)
    structs: list = field(default_factory=list)
    typedefs: dict = field(default_factory=dict)          # alias -> type text (non-struct)
    struct_aliases: dict = field(default_factory=dict)    # alias -> struct tag (typedef struct Tag Alias;)
    enums: set = field(default_factory=set)

    def pack_at(self, pos: int) -> int:
        cur = DEFAULT_PACK
        for p, v in self.pack_events:
            if p > pos:
                break
            cur = v
        return cur


def _pack_events(masked: str) -> list:
    stack, cur, ev = [], DEFAULT_PACK, []
    for m in _PACK_RX.finditer(masked):
        args = [a.strip() for a in m.group(1).split(",") if a.strip()]
        if not args:
            cur = DEFAULT_PACK
        elif args[0] == "push":
            stack.append(cur)
            if len(args) > 1:
                try:
                    cur = int(args[-1], 0)
                except ValueError:
                    pass
        elif args[0] == "pop":
            cur = stack.pop() if stack else DEFAULT_PACK
        else:
            try:
                cur = int(args[0], 0)
            except ValueError:
                pass
        ev.append((m.start(), cur))
    return ev


def _blank(masked: str, a: int, b: int) -> str:
    return masked[:a] + re.sub(r"[^\n]", " ", masked[a:b]) + masked[b:]


_STRUCT_KW = re.compile(r"\b(struct|union|class)\b[ \t\r\n]*([A-Za-z_]\w*)?[ \t\r\n]*(:[^{;]*)?(?=\{)")
_ENUM_KW = re.compile(r"\benum\b[ \t\r\n]*([A-Za-z_]\w*)?[ \t\r\n]*(?=\{)")
_FWD_TYPEDEF = re.compile(r"\btypedef\s+(?:struct|union)\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*;")
_SIMPLE_TYPEDEF = re.compile(r"\btypedef\s+([^;{}()]+?)\s*([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*;")
_FP_TYPEDEF = re.compile(r"\btypedef\s+[^;{}]*?\(\s*(?:\w+\s+)*\*\s*([A-Za-z_]\w*)\s*\)\s*\(")


def parse_file(rel: str, text: str) -> FileInfo:
    mk = mask(text)
    code = blank_preproc(mk)
    fi = FileInfo(rel=rel, text=text, masked=mk, code=code, lines=LineIndex(text))
    fi.pack_events = _pack_events(mk)
    for m in _DEFINE_RX.finditer(mk):
        body = m.group(3).strip()
        # continuation lines
        end = m.end()
        while body.endswith("\\") and end < len(mk):
            nl = mk.find("\n", end)
            nl = len(mk) if nl < 0 else nl
            body = body[:-1] + " " + mk[end:nl].strip() if end < nl else body[:-1]
            end = nl + 1 if nl < len(mk) else nl
            body = body.strip()
        if m.group(2) is None:
            fi.defines.setdefault(m.group(1), body)
        else:
            fi.funcmacros.setdefault(m.group(1), body)
    # the masked text blanks the quoted path: take the path from the source, the directive from the masked text
    # (so an #include inside a comment is not one)
    fi.includes = [m.group(1) for m in _INCLUDE_RX.finditer(text) if "include" in mk[m.start():m.end()]]
    for m in _FWD_TYPEDEF.finditer(code):
        fi.struct_aliases[m.group(2)] = m.group(1)
    for m in _FP_TYPEDEF.finditer(code):
        fi.typedefs[m.group(1)] = "void *"
    for m in _SIMPLE_TYPEDEF.finditer(code):
        t, name = m.group(1), m.group(2)
        if "(" in t or re.match(r"\s*(?:struct|union|enum)\s*$", t):
            continue
        if m.group(3):
            continue
        fi.typedefs.setdefault(name, t.strip())
    _parse_enums(fi)
    _parse_structs(fi)
    return fi


def _declarators(after: str) -> list[tuple[str, int]]:
    """'Alias, *PAlias' -> [('Alias', 0), ('PAlias', 1)]."""
    out = []
    for _, piece in split_top(after.strip(), ","):
        m = re.match(r"\s*((?:\*\s*)*)([A-Za-z_]\w*)", piece)
        if m:
            out.append((m.group(2), m.group(1).count("*")))
    return out


def _parse_enums(fi: FileInfo):
    code = fi.code
    for m in _ENUM_KW.finditer(code):
        b = m.end()
        e = match_close(code, b)
        semi = code.find(";", e)
        if m.group(1):
            fi.enums.add(m.group(1))
        pre = code[max(0, m.start() - 12):m.start()]
        if re.search(r"typedef\s*$", pre) and semi > 0:
            for n, p in _declarators(code[e + 1:semi]):
                if p == 0:
                    fi.enums.add(n)


def _parse_structs(fi: FileInfo):
    code = fi.code
    taken = []          # body ranges consumed by an outer definition
    for m in _STRUCT_KW.finditer(code):
        if any(a <= m.start() < b for a, b in taken):
            continue
        b = m.end()
        e = match_close(code, b)
        if e >= len(code):
            continue
        taken.append((b, e))
        if re.search(r"template\s*<[^<>]*(?:<[^<>]*>[^<>]*)*>\s*$", code[max(0, m.start() - 200):m.start()]):
            continue                    # class template: no fixed layout
        pre = code[max(0, m.start() - 16):m.start()]
        is_typedef = re.search(r"\btypedef\s*$", pre) is not None
        semi = code.find(";", e)
        trailing = code[e + 1:semi] if semi > 0 else ""
        d = _build_def(fi, "union" if m.group(1) == "union" else "struct", m.group(2), b, e, None)
        d.kw = m.group(1)
        if m.group(3):
            for part in m.group(3)[1:].split(","):
                words = re.sub(r"\b(?:public|private|protected|virtual)\b", " ", part).split()
                if words:
                    d.bases.append(words[-1])
        if is_typedef:
            for n, p in _declarators(trailing):
                if p == 0:
                    d.aliases.append(n)
                else:
                    fi.typedefs.setdefault(n, fmt_type(d.name, p))
        fi.structs.append(d)


def _build_def(fi: FileInfo, kind: str, tag, lb: int, rb: int, parent) -> StructDef:
    d = StructDef(tag=tag, kind=kind, file=fi.rel, line=fi.lines.line(lb), pack=fi.pack_at(lb), parent=parent,
                  end_line=fi.lines.line(rb), fi=fi)
    d.conditional = bool(_CPP_COND.search(fi.masked[lb:rb]))
    d.has_virtual = re.search(r"\bvirtual\b", fi.code[lb:rb]) is not None
    d.raw = _parse_members(fi, lb + 1, rb, d)
    return d


_NESTED_KW = re.compile(r"\s*(struct|union)\b[ \t\r\n]*([A-Za-z_]\w*)?[ \t\r\n]*\{")


def _parse_members(fi: FileInfo, a: int, b: int, owner: StructDef) -> list:
    code = fi.code
    out = []
    i = a
    while i < b:
        while i < b and code[i] in " \t\r\n;":
            i += 1
        if i >= b:
            break
        acc = re.match(r"(?:public|private|protected)\s*:(?!:)", code[i:b])
        if acc:
            i += acc.end()
            continue
        nm = _NESTED_KW.match(code, i)
        if nm and nm.end() <= b:
            lb = nm.end() - 1
            rb = match_close(code, lb)
            inner = _build_def(fi, nm.group(1), nm.group(2), lb, rb, owner)
            inner.pack = owner.pack
            semi = code.find(";", rb)
            trailing = code[rb + 1:semi] if 0 < semi <= b else ""
            decls = _decl_list(trailing, "") if trailing.strip() else []
            if nm.group(2):
                fi.structs.append(inner) if inner not in fi.structs else None
            if not decls:
                out.append(RawMember(name=None, type=f"{inner.kind} {inner.tag or ''}".strip(), inline=inner,
                                     line=inner.line))
            else:
                for nme, ptr, dims, bits, fp in decls:
                    out.append(RawMember(name=nme, type=f"{inner.kind} {inner.tag or '<anon>'}", ptr=ptr,
                                         dims=dims, bits=bits, inline=inner if ptr == 0 else None,
                                         line=inner.line))
            i = (semi if semi > 0 else rb) + 1
            continue
        # plain declaration up to ';' at depth 0
        j = i
        depth = 0
        while j < b:
            c = code[j]
            if c in "([{":
                depth += 1
            elif c in ")]}":
                depth -= 1
            elif c == ";" and depth == 0:
                break
            if c == "}" and depth == 0 and "(" in code[i:j]:
                j += 1          # an inline method body ended the statement
                break
            j += 1
        stmt = code[i:j].strip()
        stmt = re.sub(r"^(?:(?:public|private|protected)\s*:\s*)+", "", stmt)
        if "{" in stmt:
            stmt = ""
        line = fi.lines.line(i)
        i = j + 1
        if not stmt:
            continue
        for nme, ptr, dims, bits, fp, ty in _decl_pieces(stmt):
            out.append(RawMember(name=nme, type=ty, ptr=ptr, dims=dims, bits=bits, funcptr=fp, line=line))
    return out


def _decl_pieces(stmt: str):
    """Declaration statement -> [(name, ptr, dims, bits, funcptr, base type text)]."""
    stmt = re.sub(r"\b(?:typedef|static|extern)\b", " ", stmt).strip()
    if not stmt:
        return []
    # function pointer member: ret (conv *name)(...)
    fp = re.match(r"^(.*?)\(\s*(?:[A-Za-z_]\w*\s+)*\*+\s*([A-Za-z_]\w*)\s*(\[[^\]]*\])?\s*\)\s*\(", stmt, re.S)
    if fp:
        return [(fp.group(2), 1, [fp.group(3)[1:-1]] if fp.group(3) else [], None, True, "void")]
    if "(" in stmt and not stmt.startswith("("):
        # prototype / macro use inside a struct: ignore
        if re.match(r"^[\w\s\*]+\([^)]*\)\s*(const)?$", stmt):
            return []
    pieces = split_top(stmt, ",")
    first = pieces[0][1]
    m = re.match(r"^(?P<ty>.*?)(?P<stars>[\s*]*?)(?P<name>[A-Za-z_]\w*)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)"
                 r"(?::\s*(?P<bits>[^,]+?))?\s*$", first, re.S)
    if not m or not m.group("ty").strip():
        return []
    ty = m.group("ty").strip()
    # type text may end with stars that belong to the first declarator
    tstars = ty.count("*")
    base_ty = ty.replace("*", " ").strip()
    out = []
    ptr0 = tstars + m.group("stars").count("*")
    out.append((m.group("name"), ptr0, _dims(m.group("dims")), (m.group("bits") or "").strip() or None, False,
                base_ty))
    for _, p in pieces[1:]:
        m2 = re.match(r"^\s*(?P<stars>(?:\*\s*)*)(?P<name>[A-Za-z_]\w*)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)"
                      r"(?::\s*(?P<bits>.+?))?\s*(?:=.*)?$", p, re.S)
        if m2:
            out.append((m2.group("name"), m2.group("stars").count("*"), _dims(m2.group("dims")),
                        (m2.group("bits") or "").strip() or None, False, base_ty))
    return out


def _dims(s: str) -> list:
    return [x.strip() for x in re.findall(r"\[([^\]]*)\]", s or "")]


def _decl_list(s: str, base: str):
    out = []
    for _, p in split_top(s.strip(), ","):
        m = re.match(r"^\s*(?P<stars>(?:\*\s*)*)(?P<name>[A-Za-z_]\w*)\s*(?P<dims>(?:\[[^\]]*\]\s*)*)"
                     r"(?::\s*(?P<bits>.+?))?\s*$", p, re.S)
        if m:
            out.append((m.group("name"), m.group("stars").count("*"), _dims(m.group("dims")),
                        (m.group("bits") or "").strip() or None, False))
    return out


# --------------------------------------------------------------------------------------------- the index

_CONST_OK = re.compile(r"^[0-9a-fA-FxX+\-*/()<>|&~%\s]*$")


def align_up(v: int, a: int) -> int:
    return (v + a - 1) // a * a if a > 0 else v


class Index:
    """All parsed files of one scope (a client, or a whole tree) and the layout engine."""

    def __init__(self):
        self.files: dict[str, FileInfo] = {}
        self.by_name: dict[str, list] = {}
        self.defines: dict[str, str] = {}
        self.typedefs: dict[str, str] = {}
        self.enums: set = set()
        self._layouts: dict[int, Layout] = {}
        self._busy: set = set()
        self._uid = 0
        self._inc_cache: dict[str, set] = {}
        self._dirs: list = []

    # -- building
    def add(self, fi: FileInfo):
        self.files[fi.rel] = fi
        for d in fi.structs:
            self._uid += 1
            d.uid = self._uid
            self._reg(d)
        for k, v in fi.defines.items():
            self.defines.setdefault(k, v)
        for k, v in fi.typedefs.items():
            self.typedefs.setdefault(k, v)
        self.enums |= fi.enums
        for alias, tag in fi.struct_aliases.items():
            for d in self.by_name.get(tag, []):
                if alias not in d.aliases:
                    d.aliases.append(alias)
                    self.by_name.setdefault(alias, []).append(d)

    def _reg(self, d: StructDef):
        for n in d.names:
            self.by_name.setdefault(n, []).append(d)
        # tag-less nested definitions get their uid too
        for rm in d.raw:
            if rm.inline is not None and not rm.inline.uid:
                self._uid += 1
                rm.inline.uid = self._uid
                self._reg_inline(rm.inline)

    def _reg_inline(self, d):
        for rm in d.raw:
            if rm.inline is not None and not rm.inline.uid:
                self._uid += 1
                rm.inline.uid = self._uid
                self._reg_inline(rm.inline)

    def resolve_aliases(self):
        """Second pass after all files are added: `typedef struct Tag Alias;` may precede the definition.
        A plain typedef of a struct name (`typedef deflate_state DeflateState_guess;`, no pointer or array), and a
        typedef of such a typedef, is an alias of that struct too (to a fixed point; the struct is the one the
        typedef's own file sees)."""
        for fi in self.files.values():
            for alias, tag in fi.struct_aliases.items():
                for d in self.by_name.get(tag, []):
                    if alias not in d.aliases:
                        d.aliases.append(alias)
                        self.by_name.setdefault(alias, []).append(d)
        changed = True
        while changed:
            changed = False
            for fi in self.files.values():
                for alias, t in fi.typedefs.items():
                    base, ptr = split_type(t)
                    if ptr or not base or base == alias or base not in self.by_name or re.search(r"[\[(]", t):
                        continue
                    d = self.find_struct(base, fi.rel)
                    if d is None or alias in d.names:
                        continue
                    d.aliases.append(alias)
                    self.by_name.setdefault(alias, []).append(d)
                    changed = True

    def all_defs(self):
        for fi in self.files.values():
            yield from fi.structs

    # -- includes
    def included(self, rel: str) -> set:
        if rel in self._inc_cache:
            return self._inc_cache[rel]
        seen = {rel}
        stack = [rel]
        while stack:
            cur = stack.pop()
            fi = self.files.get(cur)
            if not fi:
                continue
            for inc in fi.includes:
                for cand in self._inc_candidates(cur, inc):
                    if cand in self.files and cand not in seen:
                        seen.add(cand)
                        stack.append(cand)
        self._inc_cache[rel] = seen
        return seen

    def _inc_candidates(self, cur: str, inc: str):
        d = cur.rsplit("/", 1)[0] if "/" in cur else ""
        yield (d + "/" + inc) if d else inc
        yield "include/" + inc
        for r in self.files:
            if r.endswith("/" + inc) or r == inc:
                yield r

    # -- lookup
    def find_struct(self, name: str, from_file: str | None = None) -> StructDef | None:
        base, ptr = split_type(name)
        cands = self.by_name.get(base) or []
        if not cands:
            return None
        if len(cands) == 1:
            return cands[0]
        if from_file:
            same = [c for c in cands if c.file == from_file]
            if same:
                return same[0]
            inc = self.included(from_file)
            vis = [c for c in cands if c.file in inc]
            if vis:
                return vis[0]
        return cands[0]

    # -- constants
    def eval_const(self, expr: str, fi_rel: str | None = None, _depth=0):
        if _depth > 8 or expr is None:
            return None
        e = expr.strip()
        if not e:
            return None
        fi = self.files.get(fi_rel) if fi_rel else None

        def sub_ident(m):
            n = m.group(0)
            body = None
            if fi and n in fi.defines:
                body = fi.defines[n]
            elif n in self.defines:
                body = self.defines[n]
            if body is None:
                raise KeyError(n)
            v = self.eval_const(body, fi_rel, _depth + 1)
            if v is None:
                raise KeyError(n)
            return str(v)

        def sub_sizeof(m):
            ti = self.type_info(m.group(1), fi_rel)
            if not ti or ti[0] is None:
                raise KeyError("sizeof")
            return str(ti[0])
        try:
            e = re.sub(r"sizeof\s*\(\s*([^()]+?)\s*\)", sub_sizeof, e)
            e = re.sub(r"\(\s*(?:int|unsigned|unsigned int|long|size_t|DWORD|u32)\s*\)", "", e)
            e = re.sub(r"(?<![\w.])(0[xX][0-9a-fA-F]+|\d+)[uUlL]+\b", r"\1", e)
            e = re.sub(r"\b(?!0[xX])[A-Za-z_]\w*\b", sub_ident, e)
            if not _CONST_OK.match(e) or "**" in e or len(e) > 200:
                return None
            e = e.replace("/", "//")
            return int(eval(e, {"__builtins__": {}}, {}))      # noqa: S307 - only digits/operators pass _CONST_OK
        except (KeyError, SyntaxError, ZeroDivisionError, ValueError, TypeError, RecursionError, MemoryError,
                OverflowError):
            return None

    # -- type info: (size, align_natural, kind, StructDef|None) ; size None = unknown
    def type_info(self, typestr: str, fi_rel: str | None = None, _depth=0):
        base, ptr = split_type(typestr)
        if ptr:
            return (4, 4, "ptr", None)
        if _depth > 10 or not base:
            return None
        if base in BUILTIN:
            s, k = BUILTIN[base]
            return (s, s, k, None)
        if base == "void":
            return (0, 1, "int", None)
        d = self.find_struct(base, fi_rel)
        if d is not None:
            lay = self.layout(d)
            return (lay.size, lay.align, "struct", d)
        fi = self.files.get(fi_rel) if fi_rel else None
        if fi and base in fi.typedefs:
            return self.type_info(fi.typedefs[base], fi_rel, _depth + 1)
        if base in self.typedefs:
            return self.type_info(self.typedefs[base], fi_rel, _depth + 1)
        if base in self.enums or (fi and base in fi.enums):
            return (4, 4, "int", None)
        if base in WIN_STRUCTS:
            return (WIN_STRUCTS[base][0], WIN_STRUCTS[base][1], "struct", None)
        if _PTR_NAME.match(base):
            return (4, 4, "ptr", None)
        return None

    def size_of(self, typestr: str, fi_rel=None):
        ti = self.type_info(typestr, fi_rel)
        return ti[0] if ti else None

    # -- layout
    def layout(self, d: StructDef) -> Layout:
        if d.uid in self._layouts:
            return self._layouts[d.uid]
        if d.uid in self._busy:
            return Layout(d.name, d.kind, 0, 1, d.pack, [], {}, False, ["recursive struct"], d.file, d.line)
        self._busy.add(d.uid)
        try:
            lay = self._layout(d)
        finally:
            self._busy.discard(d.uid)
        self._layouts[d.uid] = lay
        return lay

    def _layout(self, d: StructDef) -> Layout:
        pack = d.pack
        is_union = d.kind == "union"
        members: list[Member] = []
        direct: dict = {}
        aggregates: list[Member] = []
        issues: list = []
        exact = True
        off = 0
        maxalign = 1
        unit = None            # [offset, size, bits_used] current bitfield unit
        fi = d.file
        if d.conditional:
            issues.append("body has #if/#else: both branches read")
        has_vptr = False
        for bn in d.bases:
            bd = self.find_struct(bn, fi)
            if bd is None or bd is d:
                exact = False
                issues.append(f"unknown base class {bn}")
                continue
            bl = self.layout(bd)
            if not bl.exact:
                exact = False
                issues.append(f"base {bn} not exact")
            ba = max(1, min(bl.align, pack))
            if not is_union:
                bo = align_up(off, ba)
                for bm in bl.members:
                    members.append(Member(bm.name, bm.path, bm.type, bo + bm.offset, bm.size, bm.align, bm.dims,
                                          bm.bits, bm.bit_offset, bm.kind, bm.elem_size, bm.unknown_name, bm.is_pad,
                                          bm.guessed, bm.line, bm.volatile))
                for am in bl.aggregates:
                    aggregates.append(_moved(am, bo, am.path, am.volatile))
                for k, (so, st, sd_) in bl.direct.items():
                    direct.setdefault(k, (bo + so, st, sd_))
                off = bo + bl.size
                maxalign = max(maxalign, ba)
                has_vptr = has_vptr or any(m.path == "__vptr" for m in bl.members)
        if d.has_virtual and not has_vptr and not is_union:
            members.append(Member("__vptr", "__vptr", "void *", off, 4, 4, [], None, None, "ptr", 4, False, False,
                                  False, d.line))
            off += 4
            maxalign = max(maxalign, min(4, pack))

        def place(size, al):
            nonlocal off
            al = max(1, min(al, pack))
            if is_union:
                o = 0
                off = max(off, size)
            else:
                o = align_up(off, al)
                off = o + size
            return o, al

        for rm in d.raw:
            dims = [self.eval_const(x, fi) if x else None for x in rm.dims]
            if rm.bits is not None and rm.name is not None:
                ti = self.type_info(rm.type, fi)
                width = self.eval_const(rm.bits, fi) or 1
                sz = ti[0] if ti and ti[0] else 4
                if ti is None:
                    exact = False
                    issues.append(f"unknown bitfield type {rm.type}")
                if is_union:
                    o, al = place(sz, sz)
                    unit = None
                else:
                    if unit and unit[1] == sz and unit[2] + width <= sz * 8 and width > 0:
                        pass
                    else:
                        a = max(1, min(sz, pack))
                        o0 = align_up(off, a)
                        unit = [o0, sz, 0]
                        off = o0 + sz
                        maxalign = max(maxalign, a)
                    o = unit[0]
                    bit_off = unit[2]
                    unit[2] += width
                    m = Member(rm.name, rm.name, rm.type, o, sz, min(sz, pack), [], width, bit_off, "int", sz,
                               looks_unknown(rm.name), False, False, rm.line, has_qual(rm.type, "volatile"))
                    members.append(m)
                    direct[rm.name] = (o, rm.type, [])
                    continue
                maxalign = max(maxalign, al)
                members.append(Member(rm.name, rm.name, rm.type, o, sz, al, [], width, 0, "int", sz,
                                      looks_unknown(rm.name), False, False, rm.line, has_qual(rm.type, "volatile")))
                direct[rm.name] = (o, rm.type, [])
                continue
            unit = None
            # element size/align
            guessed = False
            kind = "int"
            sub = None
            if rm.inline is not None and rm.ptr == 0:
                sub = self.layout(rm.inline)
                es, ea, kind = sub.size, sub.align, "struct"
                if not sub.exact:
                    exact = False
                    issues.append(f"inline {rm.inline.kind} {rm.name or ''} not exact")
            elif rm.ptr or rm.funcptr:
                es, ea, kind = 4, 4, "ptr"
            else:
                ti = self.type_info(rm.type, fi)
                if ti is None or ti[0] is None:
                    es, ea, kind = 4, 4, "unknown"
                    guessed = True
                    exact = False
                    issues.append(f"unknown type '{norm_type(rm.type)}' for member {rm.name} (assumed 4 bytes)")
                else:
                    es, ea, kind, sd = ti
                    if sd is not None:
                        sub = self.layout(sd)
                        if not sub.exact:
                            exact = False
                            issues.append(f"member {rm.name}: struct {sd.name} not exact")
            count = 1
            for x in dims:
                if x is None:
                    exact = False
                    issues.append(f"unevaluated array size in {rm.name}")
                    x = 1
                count *= max(x, 0)
            size = es * count
            o, al = place(size, ea)
            maxalign = max(maxalign, al)
            tname = fmt_type(split_type(rm.type)[0], rm.ptr) if rm.inline is None else rm.type
            vol = has_qual(rm.type, "volatile")
            if rm.name is None and sub is not None:
                # anonymous inline struct/union: promote its members
                for sm in sub.members:
                    members.append(_moved(sm, o, sm.path, sm.volatile or vol))
                for am in sub.aggregates:
                    aggregates.append(_moved(am, o, am.path, am.volatile or vol))
                for k, (so, st, sd_) in sub.direct.items():
                    direct[k] = (o + so, st, sd_)
                continue
            name = rm.name
            is_arr = bool(dims)
            if sub is not None and not is_arr and rm.ptr == 0:
                # expand by-value struct members under a dotted path
                for sm in sub.members:
                    mm = _moved(sm, o, f"{name}.{sm.path}", sm.volatile or vol)
                    mm.line = sm.line or rm.line
                    members.append(mm)
                aggregates.append(Member(name, name, tname, o, size, al, [], None, None, "struct", es,
                                         looks_unknown(name), False, False, rm.line, vol))
                for am in sub.aggregates:
                    aggregates.append(_moved(am, o, f"{name}.{am.path}", am.volatile or vol))
                if not sub.members:
                    members.append(Member(name, name, tname, o, size, al, dims, None, None, "struct", es,
                                          looks_unknown(name), False, False, rm.line, vol))
            else:
                unk = looks_unknown(name)
                byte_arr = is_arr and es == 1 and rm.ptr == 0 and kind == "int"
                members.append(Member(name, name, tname, o, size, al, dims, None, None, kind, es, unk,
                                      unk and byte_arr, guessed, rm.line, vol))
            direct[name] = (o, tname, dims)
        align = maxalign
        size = align_up(off, align)
        if size == 0:
            size = 1                    # empty class/struct (C++): one byte
        return Layout(d.name, d.kind, size, align, pack, members, direct, exact, issues, d.file, d.line, aggregates)


# ------------------------------------------------------------------------------------- expression typing

_CAST_BODY = re.compile(r"^\s*(?:(?:const|volatile|unsigned|signed|struct|union|enum)\s+)*[A-Za-z_]\w*"
                        r"(?:\s+[A-Za-z_]\w*)*\s*(?:\*\s*(?:(?:const|volatile)\s*)*)*$")
KEYWORDS_NOT_TYPES = {"return", "sizeof", "if", "while", "for", "switch", "case", "else", "do", "goto", "defined"}


def looks_like_type(s: str) -> bool:
    s = s.strip()
    if not s or not _CAST_BODY.match(s):
        return False
    first = re.match(r"[A-Za-z_]\w*", s.lstrip("*").strip()).group(0)
    if first in KEYWORDS_NOT_TYPES:
        return False
    if "*" in s:
        return True
    # bare word: a type only when known-ish builtin or ALLCAPS/typedef-like with qualifier words
    words = s.split()
    return all(w in ("unsigned", "signed", "int", "char", "short", "long", "float", "double", "const", "volatile",
                     "struct", "union", "enum", "void", "u8", "u16", "u32", "s8", "s16", "s32", "BYTE", "WORD",
                     "DWORD", "BOOL", "UINT") or w in BUILTIN for w in words)


def strip_outer_parens(e: str) -> str:
    e = e.strip()
    while e.startswith("(") and match_close(e, 0) == len(e) - 1:
        e = e[1:-1].strip()
    return e


# ---------------------------------------------------------------------------------------------- loading

def load_index(repo, subdirs=("src", "include"), suffixes=(".c", ".cpp", ".h", ".hpp")) -> Index:
    """Parse every C/C++ file under repo/<subdirs> into one Index (rel paths use forward slashes)."""
    repo = Path(repo)
    idx = Index()
    for sd in subdirs:
        base = repo / sd
        if not base.is_dir():
            continue
        for p in sorted(base.rglob("*")):
            if p.is_file() and p.suffix.lower() in suffixes:
                rel = p.relative_to(repo).as_posix()
                idx.add(parse_file(rel, p.read_text(encoding="utf-8", errors="replace")))
    idx.resolve_aliases()
    return idx
