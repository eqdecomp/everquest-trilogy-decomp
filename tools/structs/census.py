r"""Raw-offset access census for the STRUCT-LAYOUT phase (step S1).

    ..\.venv\Scripts\python.exe structs\census.py --repo TREE [--client trilogy|eqmain|eqgfx] --out DIR
        [--fields guess|all|none] [-v]

For every function body of the client's sources (src/..), finds memory accesses that go through a raw byte
offset (or a field of a struct the parser can lay out) and writes, per client, under DIR/<client>/:

  accesses.jsonl        one record per access (see below)
  families.tsv          one row per base family: struct name, global VA, or untyped base, with use counts
  by_base.tsv           per (family, offset): count, access types, read/write mix, functions
  globals_conflicts.tsv a global VA declared/used under 2+ types (or 2+ names)
  struct_coverage.tsv   per (struct, offset): defined member / placeholder / padding hit by raw accesses
  gapfill.tsv           the gap-fill candidates alone: raw accesses on a typed base whose struct has no field there
  census.md             headline numbers, top 40 families, what is not caught
and DIR/census.md (all clients run).

Access record (accesses.jsonl):
  client, file, line, func_va ("0x..." from `// FUNCTION: <CLIENT> 0x..`, null for helpers), func,
  form          deref_cast  *(T*)(base + off)         index  ((T*)base)[i] / p[0x..]
                global_cast *(T*)EQ_GLOBAL(..)        abs    *(T*)0x6E4DA8
                ptr_add     (char*)base + off          field  base->member on a laid-out struct (see --fields)
  base_expr     the base as written (casts stripped), base_type (declared/cast type text), base_src
                (param|local|global|this|member|nested|cast|abs|unknown), base_struct (struct the base points to:
                declared, inferred from assignments, or named by a cast type), base_inferred (bool),
                base_global_va ("0x..." when the base is a global), family (the grouping key),
  offset        int bytes (pointer-scaled when the base is a typed pointer used without a byte cast),
  dynamic       text of non-constant terms (clientBase + index*0x10 + 0x685f8 -> "index*0x10"), or null
  type, width   the access type text (pointee of the cast) and its size when known
  rw            r | w | rw | addr
  coverage      for typed bases: defined | unnamed | padding | beyond_end | hole | unknown_layout

Family key (the `family` field; `coarse_family` keeps the old, merged key as a rollup, and is the last column of
families.tsv):
  struct     the struct name (the definition the record's TU sees; `base_struct_uid` identifies it)
  G:0xVA     the global VARIABLE's own storage: `*(T*)0xVA`, an array global, or `&global` (`(char*)&g + 0xC`)
  G:0xVA->   what the global's VALUE points to: `*(T*)EQ_GLOBAL(T*,VA)`, `*(T*)g_name`, `*(T*)(g_name + off)`,
             and `*(T*)(*(u32*)0xVA + off)` (the value read from the variable, then an offset)
  ?<type>:<expr>[<scope>]   untyped base: declared type, base text, and the function's class (`Class::method`)
             or, for a free function, `file:function`; two unrelated `void *self` no longer merge
  <family>@0xN  the pointer stored at offset N of <family> (nested deref)

Lower bounds: regex/bracket heuristics on comment-masked text. See census.md "Not caught".
"""
from __future__ import annotations

import argparse
import collections
import dataclasses
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
for _p in (HERE.parent / "readability", HERE.parent / "unicorn", HERE.parent / "compare", HERE.parent / "permute"):
    if str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

import cparse as C  # noqa: E402

CLIENT_IDS = ("trilogy", "eqgfx", "eqmain")
MARKER = re.compile(r"^[ \t]*//[ \t]*FUNCTION:[ \t]*(\w+)[ \t]+(0[xX][0-9a-fA-F]+)", re.M)
STMT_KEYWORDS = {"return", "goto", "else", "case", "break", "continue", "delete", "new", "throw", "typedef", "do",
                 "if", "while", "for", "switch", "sizeof", "default", "__asm", "asm", "__try", "__except",
                 "__finally", "try", "catch"}
DEREF_CAST = re.compile(
    r"\*[ \t\r\n]*\([ \t\r\n]*((?:[A-Za-z_]\w*[ \t\r\n]+)*[A-Za-z_]\w*[ \t\r\n]*"
    r"(?:\*[ \t\r\n]*(?:(?:const|volatile)[ \t\r\n]*)*)+)\)[ \t\r\n]*")
INDEX_CLOSE = re.compile(r"\)[ \t\r\n]*\[[ \t\r\n]*(0[xX][0-9a-fA-F]+|\d+)[ \t\r\n]*\]")
IDENT_INDEX = re.compile(r"(?<![\w.>])([A-Za-z_]\w*(?:[ \t]*(?:->|\.)[ \t]*[A-Za-z_]\w*)*)[ \t]*\[[ \t]*(0[xX][0-9a-fA-F]+)[ \t]*\]")
OUTER_CAST_ADDR = re.compile(
    r"\*[ \t\r\n]*\([ \t\r\n]*((?:[A-Za-z_]\w*[ \t\r\n]+)*[A-Za-z_]\w*[ \t\r\n]*(?:\*[ \t\r\n]*)+)\)[ \t\r\n]*&[ \t\r\n]*$")
PTR_ADD = re.compile(r"\([ \t]*((?:unsigned[ \t]+|signed[ \t]+)?(?:char|u8|BYTE|byte|UCHAR|void|uint8_t)[ \t]*\*)[ \t]*\)[ \t]*")
EQ_MACRO_RX = re.compile(r"\bEQ_(GLOBAL|ARRAY)[ \t]*\(")
TAIL_ASSIGN = re.compile(r"[ \t\r\n]*(\+\+|--|[-+*/%&|^]=|<<=|>>=|=(?!=))")
HEXLIT = re.compile(r"^0[xX][0-9a-fA-F]+$")
GUESS_RX = re.compile(r"_guess$")


# --------------------------------------------------------------------------------------------- data

@dataclass
class Func:
    name: str
    qual: str | None
    va: str | None
    head: int
    brace: int
    end: int
    params: str
    marker_line: int = 0


@dataclass
class GlobalDef:
    name: str
    type: str
    va: int
    kind: str                   # global | array
    file: str
    line: int


TR = tuple      # (base, ptr, arr)


def tr_of(typestr: str, arr: int = 0) -> TR:
    b, p = C.split_type(typestr)
    return (b, p, arr)


def tr_str(t: TR | None) -> str:
    if t is None:
        return ""
    return C.fmt_type(t[0], t[1]) + ("[]" * t[2] if t[2] else "")


def match_arg_list(code: str, lp: int):
    """Args of the call whose '(' is at code[lp]; returns (args list, index of ')')."""
    rp = C.match_close(code, lp)
    inner = code[lp + 1:rp]
    return [a.strip() for _, a in C.split_top(inner, ",")], rp



# ---------------------------------------------------------------------------------------- macro expansion

_IDENT_RX = re.compile(r"[A-Za-z_]\w*")
_MACRO_SKIP_PREFIX = ("EQ_",)
_NUMERIC_BODY = re.compile(r"^\(*\s*(?:0[xX][0-9a-fA-F]+|\d+)[uUlL]*\s*\)*$")
MAX_MACRO_DEPTH = 24
MAX_MACRO_CHARS = 400000


class MacroLines:
    """LineIndex stand-in for expanded text: line() reports the USE-site line, via() the outermost macro name."""

    def __init__(self, base, prefix_len, body_org, body_via, body_len, old_end):
        self.base, self.prefix_len = base, prefix_len
        self.body_org, self.body_via, self.body_len = body_org, body_via, body_len
        self.shift = body_len - (old_end - prefix_len)
        self.suffix_start = prefix_len + body_len

    def _orig(self, pos):
        if pos < self.prefix_len:
            return pos
        if pos < self.suffix_start:
            return self.body_org[pos - self.prefix_len]
        return pos - self.shift

    def line(self, pos):
        return self.base.line(self._orig(pos))

    def via(self, pos):
        if self.prefix_len <= pos < self.suffix_start:
            return self.body_via[pos - self.prefix_len]
        return None


def parse_macros(fi: C.FileInfo) -> dict:
    """name -> (params | None, body) for every #define in the file (masked text, continuations joined)."""
    out: dict = {}
    mk = fi.masked
    for m in C._DEFINE_RX.finditer(mk):
        body = m.group(3).strip()
        end = m.end()
        while body.endswith("\\") and end < len(mk):
            nl = mk.find("\n", end)
            nl = len(mk) if nl < 0 else nl
            body = (body[:-1] + " " + mk[end:nl].strip()).strip() if end < nl else body[:-1].strip()
            end = nl + 1 if nl < len(mk) else nl
        params = None
        if m.group(2) is not None:
            params = [x.strip() for x in m.group(2)[1:-1].split(",") if x.strip()]
        out.setdefault(m.group(1), (params, body))
    return out


class MacroExpander:
    """Recursive, argument-substituting expansion of project #defines.

    Chars of the result carry (origin, via): origin is the position in the original text of the use site (macro-body
    chars) or of the argument text itself (argument chars); via is the outermost macro name the char came out of.
    """

    def __init__(self, macros: dict):
        self.macros = macros
        self.budget = 0

    def usable(self, name):
        ent = self.macros.get(name)
        if ent is None:
            return False
        params, body = ent
        if not body or "#" in body or _NUMERIC_BODY.match(body):
            return False
        return True

    def expand(self, text, org, via, disabled=frozenset(), depth=0):
        if depth > MAX_MACRO_DEPTH:
            return text, org, via
        out_t: list = []
        out_o: list = []
        out_v: list = []
        i, n = 0, len(text)
        for m in _IDENT_RX.finditer(text):
            if m.start() < i:
                continue
            name = m.group(0)
            if name in disabled or not self.usable(name):
                continue
            params, body = self.macros[name]
            s, e = m.start(), m.end()
            args = None
            if params is None:
                call_end = e
            else:
                k = e
                while k < n and text[k] in " \t\r\n":
                    k += 1
                if k >= n or text[k] != "(":
                    continue
                rp = C.match_close(text, k)
                if rp >= n:
                    continue
                pieces = []
                pos = k + 1
                for _sep, piece in C.split_top(text[k + 1:rp], ","):
                    pieces.append((pos, piece))
                    pos += len(piece) + 1
                variadic = bool(params) and params[-1] == "..."
                fixed = params[:-1] if variadic else params
                if variadic:
                    if len(pieces) < len(fixed):
                        continue
                    if len(pieces) > len(fixed):
                        a0 = pieces[len(fixed)][0]
                        pieces = pieces[:len(fixed)] + [(a0, text[a0:rp])]
                elif not (len(pieces) == len(params) or (not params and len(pieces) == 1 and not pieces[0][1].strip())):
                    continue
                pnames = fixed + (["__VA_ARGS__"] if variadic else [])
                args = {}
                for pn, (apos, atxt) in zip(pnames, pieces):
                    args[pn] = self.expand(atxt, org[apos:apos + len(atxt)], via[apos:apos + len(atxt)],
                                           disabled, depth + 1)
                call_end = rp + 1
            out_t.append(text[i:s]); out_o.extend(org[i:s]); out_v.extend(via[i:s])
            use_org = org[s]
            label = via[s] or name
            b_t: list = []
            b_o: list = []
            b_v: list = []
            last = 0
            for bm in _IDENT_RX.finditer(body):
                if args is not None and bm.group(0) in args:
                    b_t.append(body[last:bm.start()])
                    b_o.extend([use_org] * (bm.start() - last)); b_v.extend([label] * (bm.start() - last))
                    a_t, a_o, a_v = args[bm.group(0)]
                    b_t.append(a_t); b_o.extend(a_o); b_v.extend(a_v)
                    last = bm.end()
            b_t.append(body[last:])
            b_o.extend([use_org] * (len(body) - last)); b_v.extend([label] * (len(body) - last))
            bt = "".join(b_t)
            self.budget += len(bt)
            if self.budget > MAX_MACRO_CHARS:
                raise OverflowError("macro expansion budget exceeded")
            bt, b_o, b_v = self.expand(bt, b_o, b_v, disabled | {name}, depth + 1)
            out_t.append(bt); out_o.extend(b_o); out_v.extend(b_v)
            i = call_end
        out_t.append(text[i:]); out_o.extend(org[i:]); out_v.extend(via[i:])
        return "".join(out_t), out_o, out_v


# ---------------------------------------------------------------------------------- function discovery

def find_functions(fi: C.FileInfo) -> list[Func]:
    code = fi.code
    out: list[Func] = []
    markers = [(m.start(), m.group(1), m.group(2)) for m in MARKER.finditer(fi.text)]

    def is_func_head(h: str) -> bool:
        if "(" not in h or h.endswith("=") or h.endswith(","):
            return False
        if re.match(r"^(?:typedef|enum|struct\s+\w*\s*$|union\s+\w*\s*$|class\s+\w*\s*$)", h):
            return False
        if h.endswith(")"):
            return True
        if re.search(r"\)[ \t\r\n]*(?:const)?[ \t\r\n]*:[^{;]*$", h):
            return True
        return bool(re.search(r"\)[ \t\r\n]*const$", h))

    def scan(lo, hi):
        i, boundary = lo, lo
        while i < hi:
            c = code[i]
            if c == "{":
                j = C.match_close(code, i)
                head = code[boundary:i]
                hs = head.strip()
                if is_func_head(hs):
                    start = boundary + (len(head) - len(head.lstrip()))
                    out.append(Func("", None, None, start, i, j, hs))
                    i = j + 1
                    boundary = i
                    continue
                if re.search(r'(?:\bextern[ \t]*"[^"]*"|\bnamespace\b[^{;]*)$', hs):
                    scan(i + 1, j)
                    i = j + 1
                    boundary = i
                    continue
                i = j + 1
                k = code.find(";", i)
                if 0 <= k < i + 300 and "{" not in code[i:k]:
                    i = k + 1
                boundary = i
                continue
            if c == ";":
                boundary = i + 1
            elif c == "(":
                i = C.match_close(code, i)
            i += 1
    scan(0, len(code))
    prev_end = 0
    for f in out:
        # name/params from the head
        h = f.params
        rp = len(h) - 1
        while rp >= 0 and h[rp] != ")":
            rp -= 1
        # constructor initialiser lists: use the first group instead
        if re.search(r"\)[ \t\r\n]*:[^{;]*$", h):
            lp0 = h.find("(")
            lp = lp0
            rp = C.match_close(h, lp)
        else:
            lp = C.match_open(h, rp) if rp >= 0 else -1
        f.params = h[lp + 1:rp] if lp >= 0 else ""
        mn = re.search(r"([A-Za-z_~][\w:~]*(?:[ \t]*::[ \t]*~?[\w]+)*)[ \t]*$", h[:lp] if lp >= 0 else h)
        full = mn.group(1).replace(" ", "") if mn else "?"
        if "::" in full:
            f.qual, f.name = full.rsplit("::", 1)
        else:
            f.name = full
        # nearest annotation between the previous function's end and this one's body
        cand = [m for m in markers if prev_end <= m[0] < f.brace]
        if cand:
            f.va = cand[-1][2]
            f.marker_line = fi.lines.line(cand[-1][0])
        prev_end = f.end
    return out


# ------------------------------------------------------------------------------------------- scope

@dataclass
class Scope:
    vars: dict = field(default_factory=dict)     # name -> (TR, src)
    inferred: dict = field(default_factory=dict)  # var -> struct type text (if one candidate)
    this_type: str | None = None


def parse_decl_stmt(stmt: str):
    """A local/param declaration statement -> [(name, TR)] or []."""
    s = stmt.strip()
    s = re.sub(r"^(?:(?:case[ \t]+[^:]+|default|[A-Za-z_]\w*)[ \t]*:(?!:)[ \t\r\n]*)+", "", s)
    s = re.sub(r"^else\b", "", s).strip()
    if not s or "(" in s.split("=")[0] and "*" not in s.split("(")[0] and "[" not in s.split("(")[0]:
        pass
    m = re.match(r"^[A-Za-z_]\w*", s)
    if not m or m.group(0) in STMT_KEYWORDS:
        return []
    pieces = C.split_top(s, ",")
    first = pieces[0][1]
    first_noinit = C.split_top(first, "=")[0][1].strip()
    if "(" in first_noinit and not re.search(r"\(\s*(?:\w+\s+)*\*\s*\w+\s*\)\s*\(", first_noinit):
        return []
    if "->" in first_noinit or "." in first_noinit.replace("...", ""):
        return []
    fp = re.match(r"^(.*?)\(\s*(?:\w+\s+)*\*+\s*([A-Za-z_]\w*)\s*\)\s*\(", first_noinit, re.S)
    if fp:
        return [(fp.group(2), ("void", 1, 0))]
    mm = re.match(r"^(?P<ty>(?:[A-Za-z_]\w*[ \t\r\n]+)*[A-Za-z_]\w*)(?P<rest>(?:[ \t\r\n]*\*)*[ \t\r\n]*[A-Za-z_]\w*[ \t\r\n]*(?:\[[^\]]*\][ \t\r\n]*)*)$",
                  first_noinit, re.S)
    if not mm:
        return []
    ty = mm.group("ty")
    words = ty.split()
    # 'foo bar' with no star: the last word is the name; type = the earlier words
    rest = mm.group("rest")
    if "*" not in rest and not words[1:] and not re.match(r"^\s*[A-Za-z_]", rest):
        return []
    # re-split: the declarator is the last identifier in `first_noinit`
    nm = re.search(r"([A-Za-z_]\w*)[ \t\r\n]*((?:\[[^\]]*\][ \t\r\n]*)*)$", first_noinit)
    name = nm.group(1)
    head = first_noinit[:nm.start()]
    if not head.strip().rstrip("*").strip():
        return []
    if head.strip().split()[0] in STMT_KEYWORDS:
        return []
    base_text = head
    base, ptr0 = C.split_type(base_text)
    if not base:
        return []
    out = [(name, (base, ptr0, len(re.findall(r"\[", nm.group(2)))))]
    for _, p in pieces[1:]:
        p = C.split_top(p, "=")[0][1]
        m2 = re.match(r"^\s*((?:\*\s*)*)([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)$", p)
        if m2:
            out.append((m2.group(2), (base, m2.group(1).count("*"), len(re.findall(r"\[", m2.group(3))))))
    return out


def build_scope(fi: C.FileInfo, f: Func, file_globals: dict) -> Scope:
    sc = Scope()
    if f.qual:
        sc.this_type = f.qual
    for _, p in C.split_top(f.params, ","):
        p = p.strip()
        if not p or p == "void" or p == "...":
            continue
        for name, tr in parse_decl_stmt(p):
            sc.vars.setdefault(name, (tr, "param"))
    body = fi.code[f.brace + 1:f.end]
    for stmt in re.split(r"[;{}]", body):
        for name, tr in parse_decl_stmt(stmt):
            sc.vars.setdefault(name, (tr, "local"))
    return sc


# ------------------------------------------------------------------------------------------- typing

class Typer:
    def __init__(self, idx: C.Index, fi: C.FileInfo, scope: Scope, gl: dict, file_globals: dict):
        self.idx, self.fi, self.scope, self.gl, self.fg = idx, fi, scope, gl, file_globals

    # root identifier -> (TR, src, GlobalDef|None)
    def root(self, name: str):
        if name in self.scope.vars:
            tr, src = self.scope.vars[name]
            return tr, src, None
        if name == "this" and self.scope.this_type:
            return (C.split_type(self.scope.this_type)[0], 1, 0), "this", None
        g = self.gl.get(name)
        if g:
            return (tr_of(g.type, 1 if g.kind == "array" else 0)), "global", g
        if name in self.fg:
            return self.fg[name], "fileglobal", None
        return None, None, None

    def struct_of(self, tr: TR):
        if tr is None:
            return None
        return self.idx.find_struct(tr[0], self.fi.rel)

    def pointee_struct(self, tr: TR | None):
        """The struct a pointer (or array / by-value struct) TR designates."""
        if tr is None:
            return None
        if tr[1] == 1 or (tr[2] >= 1 and tr[1] == 0) or tr[1] == 0:
            return self.struct_of(tr) if (tr[1] <= 1) else None
        return None

    def macro_type(self, name: str, args: list) -> TR | None:
        if len(args) < 1:
            return None
        t = args[0]
        return tr_of(t, 1 if name == "ARRAY" else 0)

    def walk(self, e: str, collect: list | None = None):
        """Type of expression e (None if unknown). With collect: append field-step tuples
        (struct_def, member, step_offset, via ('->'|'.'|'root'), expr_before)."""
        e = C.strip_outer_parens(e)
        if not e:
            return None
        if e[0] == "(":
            close = C.match_close(e, 0)
            inner, rest = e[1:close], e[close + 1:].strip()
            if rest and C.looks_like_type(inner) and not rest.startswith(("->", ".", "[")):
                return tr_of(inner)
            if rest:
                t = self.walk(inner)
                return self._steps(t, rest, inner, collect)
            return self.walk(inner)
        if e[0] == "*":
            t = self.walk(e[1:])
            if t is None:
                return None
            if t[2] > 0:
                return (t[0], t[1], t[2] - 1)
            return (t[0], t[1] - 1, 0) if t[1] > 0 else None
        if e[0] == "&":
            t = self.walk(e[1:])
            return (t[0], t[1] + 1, t[2]) if t else None
        m = re.match(r"([A-Za-z_]\w*)", e)
        if not m:
            return None
        name = m.group(1)
        pos = m.end()
        t = None
        if name in ("EQ_GLOBAL", "EQ_ARRAY"):
            while pos < len(e) and e[pos] in " \t":
                pos += 1
            if pos < len(e) and e[pos] == "(":
                args, rp = match_arg_list(e, pos)
                t = self.macro_type(name[3:], args)
                pos = rp + 1
        else:
            t, _, _ = self.root(name)
        if t is None:
            return None
        return self._steps(t, e[pos:], name, collect)

    def _steps(self, t: TR | None, rest: str, before: str, collect):
        i, n = 0, len(rest)
        acc_off = 0
        expr = before
        while i < n and t is not None:
            c = rest[i]
            if c in " \t\r\n":
                i += 1
                continue
            if rest.startswith("->", i) or c == ".":
                arrow = rest.startswith("->", i)
                i += 2 if arrow else 1
                m = re.match(r"\s*([A-Za-z_]\w*)", rest[i:])
                if not m:
                    return None
                member = m.group(1)
                i += m.end()
                if arrow:
                    if not ((t[1] == 1 and t[2] == 0) or (t[1] == 0 and t[2] >= 1)):
                        return None
                    acc_off = 0
                else:
                    if t[1] != 0 or t[2] != 0:
                        return None
                d = self.idx.find_struct(t[0], self.fi.rel)
                if d is None:
                    return None
                lay = self.idx.layout(d)
                ent = lay.direct.get(member)
                if ent is None:
                    return None
                off, tname, dims = ent
                acc_off = acc_off + off
                if collect is not None:
                    collect.append((d, member, acc_off, "->" if arrow else ".", expr.strip(), tname, len(dims)))
                expr = expr + rest[i - m.end() - (2 if arrow else 1):i]
                b, p = C.split_type(tname)
                t = (b, p, len(dims))
                continue
            if c == "[":
                j = C.match_close(rest, i)
                if t[2] > 0:
                    t = (t[0], t[1], t[2] - 1)
                elif t[1] > 0:
                    t = (t[0], t[1] - 1, 0)
                else:
                    return None
                expr = expr + rest[i:j + 1]
                acc_off = acc_off  # element offsets are dynamic: keep
                i = j + 1
                continue
            return None
        return t


# ----------------------------------------------------------------------------------------- terms

def split_terms(content: str):
    """Top-level +/- terms -> [(sign, text)]; '->' and unary signs are not separators."""
    out, cur, depth, sign = [], [], 0, "+"
    prev_sig = ""
    i, n = 0, len(content)
    while i < n:
        c = content[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        if depth == 0 and c in "+-":
            if c == "-" and i + 1 < n and content[i + 1] == ">":
                cur.append("->")
                prev_sig = ">"
                i += 2
                continue
            unary = prev_sig == "" or prev_sig in "+-*/%&|^<>=!~,(:?"
            if c == "+" and i + 1 < n and content[i + 1] == "+" or c == "-" and i + 1 < n and content[i + 1] == "-":
                cur.append(c)
                i += 1
                continue
            if not unary:
                out.append((sign, "".join(cur).strip()))
                cur, sign = [], c
                prev_sig = ""
                i += 1
                continue
        cur.append(c)
        if not c.isspace():
            prev_sig = c
        i += 1
    out.append((sign, "".join(cur).strip()))
    return [(s, t) for s, t in out if t]


# ----------------------------------------------------------------------------------------- scanner

@dataclass
class BaseInfo:
    text: str
    family: str
    kind: str                   # struct | global | nested | untyped
    type: str                   # declared/cast type text
    src: str
    struct: C.StructDef | None = None
    inferred: bool = False
    gva: int | None = None
    gname: str | None = None
    tr: TR | None = None
    coarse: str = ""            # the old, merged family key (rollup column)
    outer_cast: str | None = None   # the outermost cast stripped from the base term (governs pointer arithmetic)


class FuncScanner:
    def __init__(self, cen: "ClientCensus", fi: C.FileInfo, f: Func, scope: Scope):
        self.cen, self.fi, self.f, self.scope = cen, fi, f, scope
        self.typer = Typer(cen.idx, fi, scope, cen.globals, cen.file_globals.get(fi.rel, {}))
        self.code = fi.code
        self.spans: list = []
        self.out: list = []

    # ---- helpers
    def unary_star(self, pos: int) -> bool:
        code = self.code
        k = pos - 1
        while k >= 0 and code[k] in " \t\r\n":
            k -= 1
        if k < 0:
            return True
        c = code[k]
        if c.isalnum() or c == "_":
            m = re.search(r"([A-Za-z_]\w*)$", code[:k + 1])
            return bool(m) and m.group(1) in ("return", "else", "case", "do", "sizeof")
        if c == ")":
            o = C.match_open(code, k)
            if o < 0:
                return False
            pre = code[:o].rstrip()
            m = re.search(r"([A-Za-z_]\w*)$", pre)
            if m and m.group(1) in ("if", "while", "for", "switch"):
                return True
            return C.looks_like_type(code[o + 1:k])
        if c == "]":
            return False
        return True

    def rw_after(self, end: int, start: int) -> str:
        code = self.code
        while True:                     # `(*(int*)p) = 1`: the access is the whole parenthesised lvalue (macro bodies)
            a = start - 1
            while a >= 0 and code[a] in " \t\r\n":
                a -= 1
            z = end
            while z < len(code) and code[z] in " \t\r\n":
                z += 1
            if a < 0 or code[a] != "(" or z >= len(code) or code[z] != ")" or C.match_close(code, a) != z:
                break
            b = a - 1
            while b >= 0 and code[b] in " \t\r\n":
                b -= 1
            if b >= 0 and (code[b].isalnum() or code[b] in "_)]"):
                break
            start, end = a, z + 1
        m = TAIL_ASSIGN.match(code, end)
        pre = code[max(0, start - 3):start].rstrip()
        addr = False
        if pre.endswith("&") and not pre.endswith("&&"):
            before = pre[:-1].rstrip()
            addr = not before or not (before[-1].isalnum() or before[-1] in "_)]")
        if addr:
            return "addr"
        if m:
            return "w" if m.group(1) == "=" else "rw"
        if pre.endswith("++") or pre.endswith("--"):
            return "rw"
        return "r"

    def split_off(self, content: str):
        """content of (base + off) -> (base_term or None, offset, dynamic, scale_cast_seen).
        Returns None when there is no literal offset at all."""
        terms = split_terms(content)
        lit = 0
        have_lit = False
        nonlit = []
        for sign, t in terms:
            v = self.cen.idx.eval_const(t, self.fi.rel)
            if v is not None:
                lit += v if sign == "+" else -v
                have_lit = True
            else:
                nonlit.append((sign, t))
        if not nonlit:
            return ("", lit, None, have_lit)
        pick = None
        for k, (sign, t) in enumerate(nonlit):
            top_star = any(sep in "*<" for sep, _ in C.split_top(t, "*<")[1:])
            if sign == "+" and not top_star:
                pick = k
                break
        if pick is None:
            pick = 0
        base = nonlit[pick][1]
        dyn = " + ".join(t for k, (s, t) in enumerate(nonlit) if k != pick) or None
        return (base, lit, dyn, have_lit)

    # ---- base analysis
    def make_base(self, term: str, depth=0) -> tuple[BaseInfo, bool, str | None]:
        """-> (BaseInfo, cast_stripped, last_cast_type)"""
        t = term.strip()
        cast = False
        last_cast = None
        outer_cast = None
        addr = False
        while True:
            t = t.strip()
            if t.startswith("(") and C.match_close(t, 0) == len(t) - 1:
                t = t[1:-1]
                continue
            if t.startswith("("):
                close = C.match_close(t, 0)
                inner, rest = t[1:close], t[close + 1:].strip()
                if rest and C.looks_like_type(inner):
                    cast = True
                    last_cast = inner
                    if outer_cast is None:
                        outer_cast = inner
                    t = rest
                    continue
            if t.startswith("&"):
                addr = True
                t = t[1:]
                continue
            break
        text = t
        tr = self.typer.walk(t)
        src = "unknown"
        gva = gname = None
        # nested deref-cast base: *(T*)(inner)
        nm = re.match(r"^\*\s*\(\s*([^()]*?\*)\s*\)\s*\(", t, re.S)
        if nm and C.match_close(t, nm.end() - 1) != len(t) - 1:
            nm = None
        nm_inner = t[nm.end():-1] if nm else None
        m_root = re.match(r"([A-Za-z_]\w*)", t)
        rootname = m_root.group(1) if m_root else None
        if t.startswith("*") and depth < 4:
            gb = self._global_value_base(t)
            if gb is not None:
                gb.outer_cast = outer_cast
                return gb, cast, last_cast
        if t.startswith("*") and nm and depth < 4:
            ibase, ioff, idyn, _ = self.split_off_safe(nm_inner)
            if ibase is not None and ibase != "":
                inner, _, _ = self.make_base(ibase, depth + 1)
                vtype = C.fmt_type(*(lambda b, p: (b, max(p - 1, 0)))(*C.split_type(nm.group(1))))
                fam = f"{inner.family}@{hx(ioff)}"
                bi = BaseInfo(t, fam, "nested", vtype, "nested", tr=tr_of(vtype),
                              coarse=f"{inner.coarse or inner.family}@{hx(ioff)}")
                sd = self.typer.pointee_struct(bi.tr) if bi.tr and bi.tr[1] == 1 else None
                if sd is not None:
                    bi.struct = sd
                    bi.family = bi.coarse = sd.name
                    bi.kind = "struct"
                bi.outer_cast = outer_cast
                return bi, cast, last_cast
        if rootname in ("EQ_GLOBAL", "EQ_ARRAY"):
            lp = t.find("(", len(rootname))
            args, rp = match_arg_list(t, lp)
            if len(args) >= 2 and rp == len(t) - 1:
                gva = self.cen.parse_va(args[1])
                src = "global"
        elif rootname:
            rt, rsrc, g = self.typer.root(rootname)
            src = rsrc or "unknown"
            if g is not None and re.fullmatch(r"[A-Za-z_]\w*", t):
                gva, gname = g.va, g.name
            elif rsrc and t != rootname:
                src = "member" if rsrc not in ("global",) else "global"
        decl = tr_str(tr) if tr else ""
        bi = BaseInfo(text, "", "untyped", decl, src, tr=tr, gva=gva, gname=gname, outer_cast=outer_cast)
        # struct designated by the type; `&x` of a pointer variable designates the variable, not the pointee (C7)
        sd = None
        if tr is not None and ((not addr and (tr[1] == 1 or tr[2] >= 1)) or (addr and tr[1] == 0)):
            sd = self.typer.struct_of(tr)
        if sd is None and last_cast:
            cb, cp = C.split_type(last_cast)
            if cp == 1:
                cs = self.cen.idx.find_struct(cb, self.fi.rel)
                if cs is not None and self.cen.idx.layout(cs).size > 1:
                    sd = cs
        if sd is None and rootname and rootname in self.scope.inferred and t == rootname and not addr:
            ist = self.scope.inferred[rootname]
            sd = self.cen.idx.find_struct(ist, self.fi.rel)
            bi.inferred = sd is not None
        if sd is not None:
            bi.struct = sd
            bi.kind = "struct"
            bi.family = bi.coarse = sd.name
        elif gva is not None:
            bi.kind = "global"
            # `G:VA` is the variable's own storage (an array, or the address taken with &); `G:VA->` is what the
            # variable's value points to (a deref of the global) (C2)
            storage = addr or rootname == "EQ_ARRAY" or (rootname in self.cen.globals
                                                         and self.cen.globals[rootname].kind == "array")
            bi.coarse = f"G:0x{gva:08X}"
            bi.family = bi.coarse + ("" if storage else "->")
        else:
            bi.kind = "untyped"
            ty = (tr_str(tr) or "?").replace(" ", "")
            root = text if len(text) <= 40 else text[:37] + "..."
            bi.coarse = f"?{ty}:{root}"
            # keyed by the function's class (C++ Class::method) or by the function itself, so two unrelated
            # `void *self` do not merge (C1)
            scope = self.f.qual or f"{self.fi.rel}:{self.f.name}"
            bi.family = f"{bi.coarse}[{scope}]"
        return bi, cast, last_cast

    def _global_value_base(self, t: str):
        """A base that is the value stored in a global, `*(T*)0xVA` / `*(T*)(0xVA)` / `*(T*)EQ_GLOBAL(..)` /
        `*(T*)g_name`: the memory it points to is the global's pointee level (`G:VA->`), or the struct T points to."""
        gm = re.match(r"^\*\s*\(\s*([^()]*?\*)\s*\)\s*", t, re.S)
        if not gm:
            return None
        x = C.strip_outer_parens(t[gm.end():].strip())
        gva = gname = None
        cv = self.cen.idx.eval_const(x, self.fi.rel) if not x.startswith("*") else None
        if cv is not None and cv >= 0x10000:
            gva = cv
        else:
            mm = EQ_MACRO_RX.match(x)
            if mm:
                args, rp = match_arg_list(x, mm.end() - 1)
                if rp == len(x) - 1 and len(args) >= 2 and mm.group(1) == "GLOBAL":
                    gva = self.cen.parse_va(args[1])
            elif self._is_global_ident(x) and self.cen.globals[x].kind != "array":
                gva, gname = self.cen.globals[x].va, x
        if gva is None:
            return None
        vtype = C.fmt_type(*(lambda b, p: (b, max(p - 1, 0)))(*C.split_type(gm.group(1))))
        tr = tr_of(vtype)
        coarse = f"G:0x{gva:08X}"
        bi = BaseInfo(t, coarse + "->", "global", vtype, "global", gva=gva, gname=gname, tr=tr, coarse=coarse)
        if tr[1] == 1:
            sd = self.typer.struct_of(tr)
            if sd is not None:
                bi.struct, bi.kind, bi.family, bi.coarse = sd, "struct", sd.name, sd.name
        return bi

    def split_off_safe(self, content: str):
        r = self.split_off(content)
        if r[0] == "" and not r[3]:
            return (None, 0, None, False)
        return r

    # ---- emit
    def emit(self, form, pos, end, tytxt, bi, off, dyn, rw, extra=None):
        base, ptr = C.split_type(tytxt) if tytxt else ("", 0)
        pointee = (base, ptr - 1) if ptr > 0 else (base, 0)
        ptxt = C.fmt_type(*pointee) if base else ""
        width = None
        if base:
            width = 4 if pointee[1] > 0 else self.cen.idx.size_of(base, self.fi.rel)
            if width == 0:
                width = None
        rec = {
            "client": self.cen.client, "file": self.fi.rel, "line": self.fi.lines.line(pos),
            "func_va": self.f.va and "0x" + self.f.va[2:].upper(), "func": (self.f.qual + "::" if self.f.qual else "") + self.f.name,
            "form": form, "base_expr": bi.text, "base_type": bi.type, "base_src": bi.src,
            "base_struct": bi.struct.name if bi.struct else None, "base_inferred": bi.inferred,
            "base_global_va": f"0x{bi.gva:08X}" if bi.gva is not None else None,
            "base_global_name": bi.gname, "family": bi.family, "family_kind": bi.kind,
            "coarse_family": bi.coarse or bi.family,
            "base_struct_uid": bi.struct.uid if bi.struct else None,
            "offset": off, "dynamic": dyn, "type": ptxt, "width": width, "rw": rw,
        }
        if extra:
            rec.update(extra)
        _vm = self.fi.lines.via(pos) if hasattr(self.fi.lines, "via") else None
        if _vm:
            rec["via_macro"] = _vm
        if bi.struct is not None and off is not None and form != "field":
            rec["coverage"] = self.cen.classify(bi.struct, off, width)
        self.out.append(rec)

    # ---- the scans
    def run(self, fields_mode: str):
        f, code = self.f, self.code
        lo, hi = f.brace + 1, f.end
        self.scan_deref(lo, hi)
        self.scan_index(lo, hi)
        self.scan_ident_index(lo, hi)
        self.scan_eq_array_index(lo, hi)
        self.scan_ptr_add(lo, hi)
        self.scan_fields(lo, hi, fields_mode)
        return self.out

    def in_span(self, pos):
        return any(a <= pos < b for a, b in self.spans)

    def scale_for(self, bi: BaseInfo, cast: bool, off: int):
        """Pointer arithmetic is scaled by the pointee size of the pointer type the sum is done in: the outermost
        cast of the base term (`(int *)raw + 3`), else the declared type of an uncast pointer variable. A byte
        cast, a non-pointer cast, a void* or an unknown size leave the offset as it is; a `T **` steps by 4 (C6)."""
        if cast:
            if not bi.outer_cast:
                return off, None
            cb, cp = C.split_type(bi.outer_cast)
        else:
            if bi.tr is None or bi.tr[2]:
                return off, None
            cb, cp = bi.tr[0], bi.tr[1]
        if cp < 1:
            return off, None
        sz = 4 if cp > 1 else self.cen.idx.size_of(C.fmt_type(cb, 0), self.fi.rel)
        if sz and sz != 1:
            return off * sz, f"scaled*{sz}"
        return off, None

    def scan_deref(self, lo, hi):
        code = self.code
        for m in DEREF_CAST.finditer(code, lo, hi):
            star = m.start()
            if not self.unary_star(star):
                continue
            k = m.end()
            if k >= hi:
                continue
            tytxt = " ".join(m.group(1).split())
            c = code[k]
            if c == "(":
                close = C.match_close(code, k)
                if close >= hi:
                    continue
                content = code[k + 1:close]
                end = close + 1
                base_term, off, dyn, have_lit = self.split_off(content)
                if base_term == "" and have_lit:
                    # absolute address
                    bi = BaseInfo(content.strip(), f"G:0x{off:08X}", "global", "", "abs", gva=off)
                    self.spans.append((k, end))
                    self.emit("abs", star, end, tytxt, bi, 0, None, self.rw_after(end, star))
                    continue
                if not have_lit:
                    # *(T*)(EQ_GLOBAL(..)) / *(T*)(ident): offset 0 only for globals
                    bt = C.strip_outer_parens(content)
                    if not (re.match(r"^\(?\s*EQ_(GLOBAL|ARRAY)\b", bt) or self._is_global_ident(bt)):
                        continue
                    bi, cast, lc = self.make_base(bt)
                    self.spans.append((k, end))
                    self.emit("global_cast", star, end, tytxt, bi, 0, None, self.rw_after(end, star))
                    continue
                bi, cast, lc = self.make_base(base_term)
                off, note = self.scale_for(bi, cast, off)
                self.spans.append((k, end))
                self.emit("deref_cast", star, end, tytxt, bi, off, dyn, self.rw_after(end, star),
                          {"note": note} if note else None)
            elif code[k:k + 3] in ("EQ_",):
                mm = EQ_MACRO_RX.match(code, k)
                if mm:
                    args, rp = match_arg_list(code, mm.end() - 1)
                    bi, cast, lc = self.make_base(code[k:rp + 1])
                    self.spans.append((k, rp + 1))
                    self.emit("global_cast", star, rp + 1, tytxt, bi, 0, None, self.rw_after(rp + 1, star))
            else:
                mi = re.match(r"[A-Za-z_]\w*", code[k:hi])
                if mi and self._is_global_ident(mi.group(0)):
                    e = k + mi.end()
                    bi, cast, lc = self.make_base(mi.group(0))
                    self.spans.append((k, e))
                    self.emit("global_cast", star, e, tytxt, bi, 0, None, self.rw_after(e, star))
                else:
                    ma = re.match(r"0[xX][0-9a-fA-F]{5,8}\b", code[k:hi])
                    if ma:
                        va = int(ma.group(0), 16)
                        e = k + ma.end()
                        bi = BaseInfo(ma.group(0), f"G:0x{va:08X}", "global", "", "abs", gva=va)
                        self.emit("abs", star, e, tytxt, bi, 0, None, self.rw_after(e, star))

    def _is_global_ident(self, s):
        s = s.strip()
        return bool(re.fullmatch(r"[A-Za-z_]\w*", s)) and s in self.cen.globals and s not in self.scope.vars

    def _index_record(self, form, pos, end, tytxt, basetxt, idx_lit, rw_pos):
        bi, cast, lc = self.make_base(basetxt)
        n = int(idx_lit, 0)
        pb, pp = C.split_type(tytxt)
        esz = 4 if pp > 1 else self.cen.idx.size_of(pb, self.fi.rel)
        if not esz:
            return
        self.emit(form, pos, end, tytxt, bi, n * esz, None, self.rw_after(end, rw_pos), {"index": n})

    def scan_index(self, lo, hi):
        code = self.code
        for m in INDEX_CLOSE.finditer(code, lo, hi):
            close = m.start()
            o = C.match_open(code, close)
            if o < 0:
                continue
            k = o - 1
            while k >= 0 and code[k] in " \t\r\n":
                k -= 1
            if k >= 0 and (code[k].isalnum() or code[k] in "_)]"):
                continue
            content = code[o + 1:close].strip()
            if not content.startswith("("):
                continue
            cc = C.match_close(content, 0)
            ctype, rest = content[1:cc], content[cc + 1:].strip()
            if not rest or "*" not in ctype or not C.looks_like_type(ctype):
                continue
            tytxt = " ".join(ctype.split())
            self._index_record("index", o, m.end(), tytxt, rest, m.group(1), o)

    def scan_ident_index(self, lo, hi):
        code = self.code
        for m in IDENT_INDEX.finditer(code, lo, hi):
            chain = m.group(1)
            root = re.match(r"[A-Za-z_]\w*", chain).group(0)
            if root in STMT_KEYWORDS:
                continue
            tr = self.typer.walk(chain)
            if tr is None or tr[1] < 1:
                continue
            if tr[2] > 0 and self.typer.root(root)[1] != "global":
                continue
            pb, pp = tr[0], tr[1]
            esz = 4 if pp > 1 else self.cen.idx.size_of(pb, self.fi.rel)
            if not esz:
                continue
            tytxt = C.fmt_type(pb, pp)
            bi, _, _ = self.make_base(chain)
            n = int(m.group(2), 0)
            oc = OUTER_CAST_ADDR.search(code, max(lo, m.start() - 80), m.start())
            if oc and oc.end() == m.start() and self.unary_star(oc.start()):
                # `*(float *)&self[0xE]`: the access has the outer cast's type at the element's address (C3)
                self.emit("deref_cast", oc.start(), m.end(), " ".join(oc.group(1).split()), bi, n * esz, None,
                          self.rw_after(m.end(), oc.start()), {"index": n, "note": "outer cast type"})
                continue
            self.emit("index", m.start(), m.end(), tytxt, bi, n * esz, None, self.rw_after(m.end(), m.start()),
                      {"index": n})

    def scan_eq_array_index(self, lo, hi):
        code = self.code
        for m in EQ_MACRO_RX.finditer(code, lo, hi):
            if m.group(1) != "ARRAY":
                continue
            args, rp = match_arg_list(code, m.end() - 1)
            mm = re.match(r"[ \t\r\n]*\[[ \t\r\n]*(0[xX][0-9a-fA-F]+|\d+)[ \t\r\n]*\]", code[rp + 1:rp + 40])
            if not mm or len(args) < 2:
                continue
            bi, _, _ = self.make_base(code[m.start():rp + 1])
            esz = self.cen.idx.size_of(args[0], self.fi.rel) if "*" not in args[0] else 4
            if not esz:
                continue
            n = int(mm.group(1), 0)
            end = rp + 1 + mm.end()
            self.emit("index", m.start(), end, args[0] + "*", bi, n * esz, None, self.rw_after(end, m.start()),
                      {"index": n})

    def scan_ptr_add(self, lo, hi):
        code = self.code
        for m in PTR_ADD.finditer(code, lo, hi):
            if self.in_span(m.start()):
                continue
            k = m.end()
            mm = re.match(r"(\(?[ \t]*[A-Za-z_]\w*(?:[ \t]*(?:->|\.)[ \t]*[A-Za-z_]\w*|[ \t]*\[[^\]\[]*\])*[ \t]*\)?)"
                          r"[ \t]*([+-])[ \t]*(0[xX][0-9a-fA-F]+|\d+)\b(?!\s*[*/(])", code[k:hi])
            if not mm:
                continue
            bt = mm.group(1).strip()
            if bt.startswith("(") and not bt.endswith(")"):
                continue
            off = int(mm.group(3), 0) * (1 if mm.group(2) == "+" else -1)
            bi, _, _ = self.make_base(bt)
            lc = " ".join(m.group(1).split())
            bi2 = bi
            if bi2.struct is None:
                pass
            self.emit("ptr_add", m.start(), k + mm.end(), lc, bi, off, None, "addr")

    def scan_fields(self, lo, hi, mode):
        if mode == "none":
            return
        code = self.code
        seen_end = lo
        for m in re.finditer(r"(?<![\w.>])[A-Za-z_]\w*", code[lo:hi]):
            s = lo + m.start()
            name = m.group(0)
            if name in STMT_KEYWORDS or (name not in self.scope.vars and name != "this"
                                         and name not in self.cen.globals and name not in ("EQ_GLOBAL", "EQ_ARRAY")
                                         and name not in self.cen.file_globals.get(self.fi.rel, {})):
                continue
            # a chain must follow
            e = lo + m.end()
            j = e
            while j < hi and code[j] in " \t":
                j += 1
            if not (code.startswith("->", j) or code.startswith(".", j) or code[j:j + 1] == "["
                    or name in ("EQ_GLOBAL", "EQ_ARRAY")):
                continue
            end = self._chain_end(s, hi)
            expr = code[s:end]
            if "->" not in expr and "." not in expr.replace("...", ""):
                continue
            collect: list = []
            self.typer.walk(expr, collect)
            for k, (d, member, off, via, before, tname, ndim) in enumerate(collect):
                if mode == "guess" and not (GUESS_RX.search(d.name) or d.file == self.fi.rel):
                    continue
                last = k == len(collect) - 1
                bi = BaseInfo(before, d.name, "struct", "", "member", struct=d)
                b, p = C.split_type(tname)
                rec_t = C.fmt_type(b, p)
                width = 4 if p else self.cen.idx.size_of(b, self.fi.rel)
                rw = self.rw_after(end, s) if last else "r"
                self.out.append({
                    "client": self.cen.client, "file": self.fi.rel, "line": self.fi.lines.line(s),
                    "func_va": self.f.va and "0x" + self.f.va[2:].upper(), "func": (self.f.qual + "::" if self.f.qual else "") + self.f.name,
                    "form": "field", "base_expr": before, "base_type": d.name + ("*" if via == "->" else ""),
                    "base_src": "member", "base_struct": d.name, "base_inferred": False, "base_global_va": None,
                    "base_global_name": None, "family": d.name, "family_kind": "struct", "offset": off,
                    "coarse_family": d.name, "base_struct_uid": d.uid,
                    "dynamic": None, "type": rec_t + ("[]" * ndim if ndim else ""), "width": width, "rw": rw,
                    "member": member,
                    "coverage": "defined",
                })
                _vm = self.fi.lines.via(s) if hasattr(self.fi.lines, "via") else None
                if _vm:
                    self.out[-1]["via_macro"] = _vm

    def _chain_end(self, s, hi):
        code = self.code
        m = re.match(r"[A-Za-z_]\w*", code[s:hi])
        i = s + m.end()
        if m.group(0) in ("EQ_GLOBAL", "EQ_ARRAY"):
            while i < hi and code[i] in " \t":
                i += 1
            if i < hi and code[i] == "(":
                i = C.match_close(code, i) + 1
        while i < hi:
            j = i
            while j < hi and code[j] in " \t":
                j += 1
            if code.startswith("->", j):
                m2 = re.match(r"[ \t]*[A-Za-z_]\w*", code[j + 2:hi])
                if not m2:
                    break
                i = j + 2 + m2.end()
            elif code[j:j + 1] == "." and code[j + 1:j + 2] != ".":
                m2 = re.match(r"[ \t]*[A-Za-z_]\w*", code[j + 1:hi])
                if not m2:
                    break
                i = j + 1 + m2.end()
            elif code[j:j + 1] == "[":
                i = C.match_close(code, j) + 1
            else:
                break
        return min(i, hi)


# ------------------------------------------------------------------------------------ client driver

class ClientCensus:
    def __init__(self, repo: Path, client: str):
        self.repo, self.client = repo, client
        self.idx = C.Index()
        self.globals: dict[str, GlobalDef] = {}
        self.file_globals: dict[str, dict] = {}
        self.src_files: list[str] = []
        self.records: list = []
        self.global_uses: list = []        # (va, type, kind, name, file, line)
        self.funcs_seen = 0
        self.struct_hits: dict = collections.defaultdict(lambda: collections.defaultdict(list))
        self.errors: list = []
        self.macro_cache: dict = {}

    def parse_va(self, tok: str):
        tok = tok.strip()
        try:
            return int(tok, 0)
        except ValueError:
            return None

    # coverage classification of a raw access
    def classify(self, d: C.StructDef, off: int, width):
        lay = self.idx.layout(d)
        if lay.size <= 0 or not lay.members:
            return "unknown_layout"
        if off < 0 or off >= lay.size:
            return "beyond_end"
        cover = [m for m in lay.members if m.offset <= off < m.offset + max(m.size, 1)]
        if not cover:
            return "hole"
        real = [m for m in cover if not m.is_pad and not m.unknown_name]
        if real:
            return "defined"
        if any(m.is_pad for m in cover):
            return "padding"
        return "unnamed"

    def load(self, verbose=False):
        import registry as R
        repo = self.repo
        src_paths = R.code_files(repo, self.client)
        src_rel = {p.relative_to(repo).as_posix() for p in src_paths if "/src/" in "/" + p.relative_to(repo).as_posix()}
        extra = sorted((repo / "include").rglob("*.h")) if (repo / "include").is_dir() else []
        todo = {}
        for p in list(src_paths) + extra:
            rel = p.relative_to(repo).as_posix()
            todo[rel] = p
        for rel, p in todo.items():
            text = p.read_text(encoding="utf-8", errors="replace")
            try:
                self.idx.add(C.parse_file(rel, text))
            except Exception as ex:               # keep going: one odd file must not stop the census
                self.errors.append(f"{rel}: parse failed: {ex!r}")
        self.idx.resolve_aliases()
        self.src_files = sorted(r for r in src_rel if r in self.idx.files and r.endswith((".c", ".cpp")))
        self.own_files = {p.relative_to(repo).as_posix() for p in src_paths}
        self._scan_globals()

    # global defines + uses
    def _scan_globals(self):
        for rel in sorted(self.own_files):
            fi = self.idx.files.get(rel)
            if not fi:
                continue
            for m in C._DEFINE_RX.finditer(fi.masked):
                if m.group(2) is not None:
                    continue
                body = m.group(3).strip()
                b = C.strip_outer_parens(body)
                mm = EQ_MACRO_RX.match(b)
                if not mm:
                    continue
                lp = mm.end() - 1
                args, rp = match_arg_list(b, lp)
                if rp != len(b) - 1 or len(args) < 2:
                    continue
                va = self.parse_va(args[1])
                if va is None:
                    continue
                kind = "array" if mm.group(1) == "ARRAY" else "global"
                self.globals.setdefault(m.group(1), GlobalDef(m.group(1), " ".join(args[0].split()), va, kind, rel,
                                                              fi.lines.line(m.start())))
        # file-level variables (static T x;) - best effort
        for rel in sorted(self.own_files):
            fi = self.idx.files.get(rel)
            if not fi:
                continue
            fg = {}
            code = fi.code
            depth = 0
            start = 0
            for i, c in enumerate(code):
                if c in "{(":
                    depth += 1
                elif c in "})":
                    depth -= 1
                    if depth < 0:
                        depth = 0
                    if depth == 0 and c == "}":
                        start = i + 1
                elif c == ";" and depth == 0:
                    stmt = code[start:i]
                    start = i + 1
                    if re.match(r"\s*(?:typedef|struct|union|enum|extern\s+\"C\"|#)", stmt) and "(" not in stmt \
                            and not re.match(r"\s*(?:static|extern)\b", stmt):
                        continue
                    s2 = re.sub(r"\b(?:static|extern)\b", " ", stmt)
                    if "(" in s2.split("=")[0] and not re.search(r"\(\s*(?:\w+\s+)*\*", s2):
                        continue
                    for name, tr in parse_decl_stmt(s2):
                        fg.setdefault(name, tr)
            self.file_globals[rel] = fg

    def count_global_uses(self):
        """-> {(va): {type: {'uses': n, 'names': set, 'sites': [..]}}}"""
        table: dict = collections.defaultdict(lambda: collections.defaultdict(lambda: {"uses": 0, "names": set(), "sites": []}))
        name_uses: collections.Counter = collections.Counter()
        gnames = set(self.globals)
        for rel in sorted(self.own_files):
            fi = self.idx.files.get(rel)
            if not fi:
                continue
            code = fi.code
            if gnames:
                for m in re.finditer(r"\b[A-Za-z_]\w*\b", code):
                    if m.group(0) in gnames:
                        name_uses[m.group(0)] += 1
            for m in EQ_MACRO_RX.finditer(code):
                args, rp = match_arg_list(code, m.end() - 1)
                if len(args) < 2:
                    continue
                va = self.parse_va(args[1])
                if va is None:
                    continue
                ty = " ".join(args[0].split())
                cell = table[va][ty]
                cell["uses"] += 1
                if len(cell["sites"]) < 3:
                    cell["sites"].append(f"{rel}:{fi.lines.line(m.start())}")
        for name, g in self.globals.items():
            cell = table[g.va][g.type]
            cell["names"].add(name)
            cell["uses"] += name_uses.get(name, 0)
            if len(cell["sites"]) < 3:
                cell["sites"].append(f"{g.file}:{g.line} ({name})")
        return table

    def infer_locals(self, fi: C.FileInfo, f: Func, sc: Scope, typer: Typer):
        body = fi.code[f.brace + 1:f.end]
        cand: dict = collections.defaultdict(set)
        for m in re.finditer(r"(?:^|[;{}])[ \t\r\n]*([A-Za-z_]\w*)[ \t]*=(?!=)[ \t]*([^;]+);", body):
            v = m.group(1)
            ent = sc.vars.get(v)
            if not ent:
                continue
            tr = ent[0]
            if tr[2] or (tr[1] == 1 and self.idx.find_struct(tr[0], fi.rel) is not None
                         and self.idx.layout(self.idx.find_struct(tr[0], fi.rel)).size > 1):
                continue
            rhs = m.group(2).strip()
            while True:
                mc = re.match(r"^\(([^()]*)\)\s*(.+)$", rhs, re.S)
                if mc and C.looks_like_type(mc.group(1)):
                    rhs = mc.group(2).strip()
                else:
                    break
            if not re.match(r"^[A-Za-z_*&]", rhs) or "(" in rhs.split("->")[0].split(".")[0] and not rhs.startswith("("):
                if not re.match(r"^[A-Za-z_]\w*(?:->|\.)", rhs):
                    continue
            t = typer.walk(rhs)
            if t and t[1] == 1 and t[2] == 0:
                d = self.idx.find_struct(t[0], fi.rel)
                if d is not None and self.idx.layout(d).size > 1:
                    cand[v].add(d.name)
                    continue
            cand[v].add("<other>")
        for v, s in cand.items():
            if len(s) == 1 and "<other>" not in s:
                vq = re.escape(v)
                if re.search(rf"(?<![\w.>]){vq}[ \t\r\n]*(?:[-+*/%&|^]=|<<=|>>=|\+\+|--)|(?:\+\+|--)[ \t\r\n]*{vq}\b",
                             body):
                    continue                  # `q += 0x10; q++`: the pointer no longer points at the struct's start (C8)
                sc.inferred[v] = next(iter(s))

    def file_macros(self, rel: str) -> dict:
        """Macros visible to a TU: its own #defines first, then those of every project header it includes."""
        if rel in self.macro_cache:
            return self.macro_cache[rel]
        tab: dict = {}
        order = [rel] + sorted(r for r in self.idx.included(rel) if r != rel)
        for r in order:
            fi = self.idx.files.get(r)
            if not fi:
                continue
            pm = self.macro_cache.get(("own", r))
            if pm is None:
                pm = self.macro_cache[("own", r)] = parse_macros(fi)
            for k, v in pm.items():
                if k.startswith(_MACRO_SKIP_PREFIX) or k in self.globals:
                    continue
                tab.setdefault(k, v)
        self.macro_cache[rel] = tab
        return tab

    def expand_func(self, fi: C.FileInfo, f: Func):
        """-> (fi', f') with the function body macro-expanded, or (fi, f) when nothing was expanded."""
        tab = self.file_macros(fi.rel)
        if not tab:
            return fi, f
        base = f.brace + 1
        body = fi.code[base:f.end]
        ex = MacroExpander(tab)
        if not any(ex.usable(m.group(0)) for m in _IDENT_RX.finditer(body)):
            return fi, f
        try:
            t, o, v = ex.expand(body, list(range(base, base + len(body))), [None] * len(body))
        except OverflowError:
            self.errors.append(f"{fi.rel}:{f.name}: macro expansion budget exceeded; scanned unexpanded")
            return fi, f
        if t == body:
            return fi, f
        new_code = fi.code[:base] + t + fi.code[f.end:]
        lines = MacroLines(fi.lines, base, o, v, len(t), f.end)
        return dataclasses.replace(fi, code=new_code, lines=lines), dataclasses.replace(f, end=base + len(t))

    def run(self, fields_mode="guess", verbose=False):
        for rel in self.src_files:
            fi = self.idx.files[rel]
            try:
                funcs = find_functions(fi)
            except Exception as ex:
                self.errors.append(f"{rel}: function scan failed: {ex!r}")
                continue
            fi0 = fi
            for f in funcs:
                self.funcs_seen += 1
                try:
                    fi, f = self.expand_func(fi0, f)
                    sc = build_scope(fi, f, self.file_globals.get(rel, {}))
                    typer = Typer(self.idx, fi, sc, self.globals, self.file_globals.get(rel, {}))
                    self.infer_locals(fi, f, sc, typer)
                    scn = FuncScanner(self, fi, f, sc)
                    self.records.extend(scn.run(fields_mode))
                except Exception as ex:
                    self.errors.append(f"{rel}:{f.name}: scan failed: {ex!r}")
        return self.records


# ----------------------------------------------------------------------------------------- reports

def hx(v):
    return f"0x{v:X}" if v >= 0 else f"-0x{-v:X}"


def tsv_write(path: Path, header: list, rows):
    with path.open("w", encoding="utf-8", newline="\n") as fh:
        fh.write("\t".join(header) + "\n")
        for r in rows:
            fh.write("\t".join(str(x).replace("\t", " ").replace("\n", " ") for x in r) + "\n")


def type_key(t: str) -> str:
    """Type text as a comparison key: whitespace and struct/class/enum keywords dropped, const/volatile kept."""
    t = re.sub(r"\b(?:struct|union|class|enum)\b", " ", t)
    t = re.sub(r"\s*\*\s*", " * ", t)
    return " ".join(t.split()).replace(" * ", "*").replace(" *", "*").replace("* ", "*")


def canon_type(idx, t: str) -> str:
    """Shape of a type for the 'strong' conflict test: ints fold to size, structs/classes by name, pointers by pointee."""
    base, ptr = C.split_type(t)
    ti = idx.type_info(base)
    if ti is None:
        shape = base
    elif ti[3] is not None:
        shape = ti[3].name
    elif ti[2] == "ptr":
        shape = "ptr"
    elif base == "void":
        shape = "void"
    else:
        shape = f"{ti[2]}{ti[0]}"
    return shape + "*" * ptr


def counter_str(c: collections.Counter, n=8):
    return "; ".join(f"{k} x{v}" for k, v in c.most_common(n))


def write_reports(cen: ClientCensus, out: Path) -> dict:
    out.mkdir(parents=True, exist_ok=True)
    recs = cen.records
    with (out / "accesses.jsonl").open("w", encoding="utf-8", newline="\n") as fh:
        for r in recs:
            fh.write(json.dumps(r, separators=(",", ":")) + "\n")
    raw = [r for r in recs if r["form"] != "field"]
    fld = [r for r in recs if r["form"] == "field"]

    # families
    fam: dict = collections.defaultdict(lambda: {"n": 0, "kind": "", "offs": set(), "funcs": set(), "types": collections.Counter(),
                                                  "files": set(), "w": 0, "ex": "", "coarse": ""})
    for r in raw:
        f = fam[r["family"]]
        f["coarse"] = f["coarse"] or r.get("coarse_family", r["family"])
        f["n"] += 1
        f["kind"] = r["family_kind"]
        f["offs"].add(r["offset"])
        f["funcs"].add((r["file"], r["func"]))
        f["types"][r["type"]] += 1
        f["files"].add(r["file"])
        f["w"] += r["rw"] in ("w", "rw")
        f["ex"] = f["ex"] or f'{r["file"]}:{r["line"]}'
    fam_rows = sorted(fam.items(), key=lambda kv: -kv[1]["n"])
    tsv_write(out / "families.tsv", ["family", "kind", "accesses", "distinct_offsets", "functions", "files", "writes",
                                     "top_types", "example", "coarse_family"],
              [(k, v["kind"], v["n"], len(v["offs"]), len(v["funcs"]), len(v["files"]), v["w"],
                counter_str(v["types"], 5), v["ex"], v["coarse"]) for k, v in fam_rows])

    # by_base
    cell: dict = collections.defaultdict(lambda: {"n": 0, "types": collections.Counter(), "rw": collections.Counter(),
                                                   "funcs": set(), "ex": ""})
    for r in raw:
        c = cell[(r["family"], r["offset"])]
        c["n"] += 1
        c["types"][r["type"]] += 1
        c["rw"][r["rw"]] += 1
        c["funcs"].add((r["file"], r["func"]))
        c["ex"] = c["ex"] or f'{r["file"]}:{r["line"]}'
    rows = []
    famorder = {k: i for i, (k, _) in enumerate(fam_rows)}
    for (fk, off), c in sorted(cell.items(), key=lambda kv: (famorder[kv[0][0]], kv[0][1])):
        rows.append((fk, fam[fk]["kind"], hx(off), c["n"], counter_str(c["types"], 6), counter_str(c["rw"], 4),
                     len(c["funcs"]), c["ex"]))
    tsv_write(out / "by_base.tsv", ["family", "kind", "offset", "count", "types", "rw", "functions", "example"], rows)

    # globals
    table = cen.count_global_uses()
    grows = []
    names_only = []
    nconf = nconf_types = nstrong = 0
    for va, types in sorted(table.items()):
        names = set()
        for t, c in types.items():
            names |= c["names"]
        keyed: dict = collections.defaultdict(lambda: {"uses": 0, "sites": []})
        for t, c in types.items():
            k = type_key(t)
            keyed[k]["uses"] += c["uses"]
            keyed[k]["sites"] += c["sites"]
        by_q = {C.norm_type(t) for t in types}
        reasons = []
        if len(by_q) >= 2:
            reasons.append("types")
        elif len(keyed) >= 2:
            reasons.append("qualifiers")
        if len(names) >= 2:
            reasons.append("names")
        if not reasons:
            continue
        if reasons == ["names"]:
            names_only.append((f"0x{va:08X}", len(names), ",".join(sorted(names)), sum(c["uses"] for c in types.values())))
            continue
        if "types" in reasons or "qualifiers" in reasons:
            nconf += 1
        if "types" in reasons:
            nconf_types += 1
        total = sum(c["uses"] for c in types.values())
        tstr = "; ".join(f"{t} x{c['uses']}" for t, c in sorted(keyed.items(), key=lambda kv: -kv[1]["uses"]))
        sites = " | ".join(f"{t}: {s[0]}" for t, c in sorted(keyed.items(), key=lambda kv: -kv[1]["uses"])[:6]
                           for s in [c["sites"]] if s)
        strong = len({canon_type(cen.idx, t) for t in types}) >= 2
        nstrong += strong
        grows.append((f"0x{va:08X}", ",".join(reasons), "yes" if strong else "no", len(keyed), total,
                      ",".join(sorted(names)), tstr, sites))
    grows.sort(key=lambda r: -r[4])
    tsv_write(out / "globals_conflicts.tsv", ["va", "reason", "strong", "n_types", "uses", "names", "types_with_uses", "sites"], grows)

    names_only.sort(key=lambda r: -r[3])
    tsv_write(out / "globals_names.tsv", ["va", "n_names", "names", "uses"], names_only)

    # struct coverage
    sc_rows, gap_rows = struct_coverage(cen, raw, fld)
    tsv_write(out / "struct_coverage.tsv",
              ["struct", "scope", "file", "line", "size", "pack", "exact", "offset", "status", "member", "member_type",
               "raw_accesses", "field_accesses", "access_types", "example", "inner_offsets", "notes"], sc_rows)
    tsv_write(out / "gapfill.tsv",
              ["struct", "file", "size", "offset", "status", "member_or_pad", "raw_accesses", "access_types", "example"],
              gap_rows)

    stats = {
        "client": cen.client, "files": len(cen.src_files), "functions": cen.funcs_seen,
        "raw_accesses": len(raw), "field_accesses": len(fld),
        "distinct_bases": len({r["family"] for r in raw}),
        "distinct_base_exprs": len({(r["file"], r["func"], r["base_expr"]) for r in raw}),
        "forms": dict(collections.Counter(r["form"] for r in raw)),
        "typed_struct_raw": sum(1 for r in raw if r["family_kind"] == "struct"),
        "global_raw": sum(1 for r in raw if r["family_kind"] == "global"),
        "untyped_raw": sum(1 for r in raw if r["family_kind"] in ("untyped", "nested")),
        "conflicting_globals": nconf, "conflicting_globals_strong": nstrong, "conflicting_globals_types": nconf_types,
        "gapfill_candidates": sum(r[6] for r in gap_rows),
        "gapfill_offsets": len(gap_rows),
        "top": [(k, v["kind"], v["n"], len(v["offs"])) for k, v in fam_rows[:40]],
        "errors": cen.errors[:20], "n_errors": len(cen.errors),
    }
    (out / "census.json").write_text(json.dumps(stats, indent=1), encoding="utf-8")
    write_md(cen, out, stats, grows)
    return stats


def struct_coverage(cen: ClientCensus, raw, fld):
    """Rows per (struct definition, offset). A struct name defined more than once is reported per definition: each
    record carries the uid of the definition its TU sees (C4). Accesses that start inside a member (not at its
    first byte) are listed in `inner_offsets` and mark the row SPLIT unless the member is an array; an access
    wider than what is left of its member marks it OVERRUN (C5)."""
    uid_def = {d.uid: d for d in cen.idx.all_defs()}

    def key_of(r):
        return (r["base_struct"], r.get("base_struct_uid"))
    by_struct: dict = collections.defaultdict(lambda: {"raw": collections.defaultdict(list), "fld": collections.Counter()})
    for r in raw:
        if r["base_struct"] and r.get("coverage") is not None:
            by_struct[key_of(r)]["raw"][r["offset"]].append(r)
    for r in fld:
        by_struct[key_of(r)]["fld"][(r["offset"], r.get("member"))] += 1
    rows, gaps = [], []
    for (sname, uid), info in sorted(by_struct.items(), key=lambda kv: (kv[0][0], kv[0][1] or 0)):
        d = uid_def.get(uid) or cen.idx.find_struct(sname)
        if d is None:
            continue
        lay = cen.idx.layout(d)
        scope = "file-local" if d.file.startswith("src/") else "header"
        head = [sname, scope, d.file, d.line, lay.size, lay.pack, "yes" if lay.exact else "no"]
        done = set()
        for m in sorted(lay.members, key=lambda m: m.offset):
            status = "padding" if m.is_pad else ("unnamed" if m.unknown_name else "defined")
            end = m.offset + max(m.size, 1)
            raws = [r for o, rs in info["raw"].items() if m.offset <= o < end for r in rs]
            for o in info["raw"]:
                if m.offset <= o < end:
                    done.add(o)
            fc = sum(c for (o, nme), c in info["fld"].items() if o == m.offset and nme == m.name)
            if not raws and not fc and not m.is_pad:
                continue
            if m.is_pad:
                # one row per accessed offset inside the padding
                continue
            at = [r for r in raws if r["offset"] == m.offset]
            inner = [r for r in raws if r["offset"] > m.offset]
            tys = collections.Counter(r["type"] for r in at)
            sub = collections.Counter(f'+{hx(r["offset"] - m.offset)} {r["type"]}' for r in inner)
            notes = []
            if inner and not m.dims:
                notes.append("SPLIT")
            if not m.dims and any(r["offset"] + (r.get("width") or 0) > end for r in raws):
                notes.append("OVERRUN")
            rows.append(head + [hx(m.offset), status, m.path, m.type, len(raws), fc, counter_str(tys, 5),
                                raws[0]["file"] + ":" + str(raws[0]["line"]) if raws else "",
                                counter_str(sub, 5), " ".join(notes)])
        # offsets not covered by a non-pad member
        for o, rs in sorted(info["raw"].items()):
            cov = rs[0].get("coverage")
            if cov in ("defined", "unnamed") and o in done:
                continue
            cov = cen.classify(d, o, rs[0].get("width"))
            if cov == "defined" or (cov == "unnamed"):
                continue
            if cov == "unknown_layout":
                rows.append(head + [hx(o), cov, "", "", len(rs), 0, counter_str(collections.Counter(r["type"] for r in rs), 5),
                                    f'{rs[0]["file"]}:{rs[0]["line"]}', "", ""])
                continue
            pads = [m for m in lay.members if m.is_pad and m.offset <= o < m.offset + m.size]
            label = pads[0].path if pads else ""
            tys = collections.Counter(r["type"] for r in rs)
            rows.append(head + [hx(o), cov, label, pads[0].type if pads else "", len(rs), 0, counter_str(tys, 5),
                                f'{rs[0]["file"]}:{rs[0]["line"]}', "", ""])
            gaps.append([sname, d.file, lay.size, hx(o), cov, label, len(rs), counter_str(tys, 5),
                         f'{rs[0]["file"]}:{rs[0]["line"]}'])
    gaps.sort(key=lambda r: -r[6])
    return rows, gaps


def write_md(cen: ClientCensus, out: Path, stats: dict, grows):
    L = []
    L.append(f"# Raw-offset census: {cen.client}\n")
    L.append(f"{stats['files']} source files, {stats['functions']} function bodies scanned.\n")
    L.append("## Headline\n")
    L.append(f"- raw accesses: **{stats['raw_accesses']}** (forms: {stats['forms']}); typed-field accesses recorded: {stats['field_accesses']}")
    L.append(f"- distinct bases (families): **{stats['distinct_bases']}**; base expressions (per function): {stats['distinct_base_exprs']}")
    L.append(f"- on a base resolved to a struct: {stats['typed_struct_raw']}; on a global: {stats['global_raw']}; untyped/nested: {stats['untyped_raw']}")
    L.append(f"- conflicting globals (2+ types or qualifiers): **{stats['conflicting_globals']}** (differ in type proper, ignoring const/volatile: {stats['conflicting_globals_types']}; strong = differ after folding signedness, int typedefs and pointer-to-struct spelling: {stats['conflicting_globals_strong']})")
    L.append(f"- gap-fill candidates: **{stats['gapfill_candidates']}** raw accesses at {stats['gapfill_offsets']} (struct, offset) pairs\n")
    L.append("## Top 40 families / bases by raw-access count\n")
    L.append("| # | family | kind | accesses | offsets |")
    L.append("|---|---|---|---|---|")
    for i, (k, kind, n, no) in enumerate(stats["top"], 1):
        L.append(f"| {i} | `{k}` | {kind} | {n} | {no} |")
    L.append("")
    if grows:
        L.append("## Conflicting globals (top 15 by uses)\n")
        L.append("| VA | reason | strong | uses | names | types |")
        L.append("|---|---|---|---|---|---|")
        for r in grows[:15]:
            L.append(f"| {r[0]} | {r[1]} | {r[2]} | {r[4]} | {r[5][:80]} | {r[6][:160]} |")
        L.append("")
    L.append("""## Family keys

`Struct` = a laid-out struct (the definition visible to the record's TU). `G:0xVA` = the global variable's own storage
(`*(T*)0xVA`, array globals, `&global`); `G:0xVA->` = what the variable's value points to (`*(T*)EQ_GLOBAL(..)`,
`*(T*)(g_name + off)`, `*(T*)(*(u32*)0xVA + off)`). `?type:expr[scope]` = untyped base, scoped by the function's class
(`Class::method`) or by `file:function` for free functions (the merged key without the scope is families.tsv
`coarse_family`). `family@0xN` = the pointer stored at offset N of family. struct_coverage.tsv has one block per struct
DEFINITION (a name defined twice is two blocks); `inner_offsets`/`notes` mark accesses that start inside a member
(SPLIT) or run past it (OVERRUN).

## Method and what is NOT caught

Comment- and string-masked text, bracket matching and regexes; no preprocessing, no real C parser. Lower bounds.

Caught: `*(T*)(base +/- const [+ dyn])`, `*(T*)EQ_GLOBAL(..)` / named global macro (offset 0), `*(T*)0xVA`,
`((T*)base)[N]`, `p[0xN]` on a pointer-typed variable, `EQ_ARRAY(T,va)[N]`, `(char*)base + const` pointer arithmetic
(form ptr_add, not an access by itself), and `a->m` / `a.m` steps on structs the parser can lay out (form field).
Writes/reads are decided from the token after the expression (`=`, `op=`, `++`) and a leading `&`.

Not caught:
- `__declspec(naked)` / `__asm` function bodies (no C accesses there; asm `[reg+off]` is not read).
- accesses through macros that hide the cast (a `#define FIELD(p) *(int*)((p)+0x10)` is seen once, at the #define
  only if it is inside a function; macro uses are not expanded).
- typed-cast accesses `((EQ_Character*)temp)->field` (typed, not raw) and `base->field` where the base type is
  unknown, shadowed, a typedef'd pointer, or comes from an unparsed header; locals typed by a typedef alias of a
  pointer are not followed.
- offsets built in variables (`off = 0x24; *(int*)(p + off)`), `p + i*4` without a literal, pointer-typed
  arithmetic through `int` offsets stored in structs, `memcpy`/`memset` sized by offsets.
- decimal-indexed `p[3]` on non-byte pointers (only hex indexes are taken for the identifier form).
- code outside function bodies; inline methods inside class bodies; K&R definitions.
- the base type of `int`/`char*` locals is inferred only from simple `local = <typed expr>;` assignments in the same
  function, and only when every assignment agrees (family comes from `base_inferred`); no flow or alias analysis.
- struct layouts: bitfields are approximate; a member of an unknown type is assumed 4 bytes (Layout.exact = no,
  noted in struct_coverage.tsv); `#if` branches inside a struct body are both read; C++ vptrs are not modelled.
- the same VA written as different EQ_GLOBAL spellings is merged (numeric compare), but a global reached only
  through a pointer-valued macro that is not a plain `#define NAME EQ_GLOBAL(T,VA)` is not in globals_conflicts.tsv.
""")
    if cen.errors:
        L.append(f"Parser errors ({stats['n_errors']}, first few): " + "; ".join(cen.errors[:5]))
    (out / "census.md").write_text("\n".join(L) + "\n", encoding="utf-8")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--repo", required=True)
    ap.add_argument("--client", choices=CLIENT_IDS)
    ap.add_argument("--out", required=True)
    ap.add_argument("--fields", choices=("guess", "all", "none"), default="guess",
                    help="which typed field accesses to record as form=field (default: _guess/file-local structs)")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    out = Path(a.out)
    out.mkdir(parents=True, exist_ok=True)
    allstats = []
    for client in ([a.client] if a.client else CLIENT_IDS):
        cen = ClientCensus(repo, client)
        cen.load(a.verbose)
        cen.run(a.fields, a.verbose)
        st = write_reports(cen, out / client)
        allstats.append(st)
        print(f"{client}: files={st['files']} funcs={st['functions']} raw={st['raw_accesses']} "
              f"fields={st['field_accesses']} bases={st['distinct_bases']} conflicts={st['conflicting_globals']} "
              f"gapfill={st['gapfill_candidates']} errors={st['n_errors']}")
    md = ["# Raw-offset census\n", f"Tree: `{repo}`\n",
          "| client | files | functions | raw accesses | distinct bases | conflicting globals | gap-fill |",
          "|---|---|---|---|---|---|---|"]
    for s in allstats:
        md.append(f"| {s['client']} | {s['files']} | {s['functions']} | {s['raw_accesses']} | {s['distinct_bases']} | "
                  f"{s['conflicting_globals']} | {s['gapfill_candidates']} |")
    md.append("\nPer-client detail (top 40 families, conflicts, method/not caught): `<client>/census.md`.\n")
    for s in allstats:
        md.append(f"## {s['client']} top 10\n")
        for i, (k, kind, n, no) in enumerate(s["top"][:10], 1):
            md.append(f"{i}. `{k}` ({kind}) {n} accesses, {no} offsets")
        md.append("")
    (out / "census.md").write_text("\n".join(md) + "\n", encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
