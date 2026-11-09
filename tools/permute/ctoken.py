"""A small C/C++ tokenizer and statement parser for the permuter (no preprocessing, no types).

Enough of C to find function definitions in a translation unit, parse a function body into statements
(blocks, if/else, loops, switch, labels, declarations, expression statements) and say roughly what a
statement reads and writes. Every token keeps its character offsets, so mutations are text splices that
leave the rest of the source (comments, layout) alone.
"""
from __future__ import annotations

import re
from dataclasses import dataclass, field

_TOKEN = re.compile(r"""
    (?P<ws>\s+)
  | (?P<comment>//[^\n]*|/\*.*?\*/)
  | (?P<str>L?"(?:\\.|[^"\\\n])*")
  | (?P<chr>L?'(?:\\.|[^'\\\n])*')
  | (?P<num>(?:0[xX][0-9a-fA-F]+|\d+\.?\d*(?:[eE][+-]?\d+)?|\.\d+(?:[eE][+-]?\d+)?)[uUlLfF]*)
  | (?P<id>[A-Za-z_]\w*)
  | (?P<op>>>=|<<=|\.\.\.|->\*|->|\+\+|--|<<|>>|<=|>=|==|!=|&&|\|\||\+=|-=|\*=|/=|%=|&=|\|=|\^=|::|\#\#|[-+*/%&|^~!<>=?:;,.()\[\]{}\#\\])
""", re.S | re.X)

KEYWORDS = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "class", "delete", "new",
    "operator", "private", "protected", "public", "this", "throw", "try", "catch", "virtual", "inline",
    "friend", "template", "bool", "true", "false", "__inline", "__forceinline", "__int8", "__int16",
    "__int32", "__int64", "__cdecl", "__stdcall", "__fastcall", "__thiscall", "__declspec", "namespace",
    "using", "typename", "mutable", "explicit", "wchar_t",
}
TYPE_WORDS = {"void", "char", "short", "int", "long", "float", "double", "signed", "unsigned", "bool", "const",
              "volatile", "register", "static", "struct", "union", "enum", "class", "__int8", "__int16",
              "__int32", "__int64", "wchar_t", "auto"}
# Integer local types the type-change mutation moves between (and the client's typedefs from common.h).
INT_TYPES = ["char", "signed char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "long",
             "unsigned long", "s8", "u8", "s16", "u16", "s32", "u32", "BYTE", "WORD", "DWORD"]
ASSIGN_OPS = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>="}
# Binary operator precedence (higher binds tighter); unary/postfix are above all of these.
PREC = {"*": 13, "/": 13, "%": 13, "+": 12, "-": 12, "<<": 11, ">>": 11, "<": 10, ">": 10, "<=": 10, ">=": 10,
        "==": 9, "!=": 9, "&": 8, "^": 7, "|": 6, "&&": 5, "||": 4, "?": 3, ":": 3}
for _o in ASSIGN_OPS:
    PREC[_o] = 2
PREC[","] = 1


@dataclass
class Tok:
    kind: str           # ws comment pp str chr num id op
    text: str
    start: int
    end: int

    def __repr__(self):
        return f"Tok({self.kind},{self.text!r})"


def tokenize(text: str) -> list[Tok]:
    out, i, n = [], 0, len(text)
    line_start = True
    while i < n:
        if line_start:
            m = re.compile(r"[ \t]*#").match(text, i)
            if m:                                           # a preprocessor line (with continuations)
                j = i
                while True:
                    k = text.find("\n", j)
                    if k < 0:
                        k = n
                        break
                    if text[k - 1] == "\\" or (text[k - 1] == "\r" and text[k - 2] == "\\"):
                        j = k + 1
                        continue
                    break
                s = m.end() - 1
                if s > i:
                    out.append(Tok("ws", text[i:s], i, s))
                out.append(Tok("pp", text[s:k], s, k))
                i = k
                line_start = False
                continue
        m = _TOKEN.match(text, i)
        if not m:
            out.append(Tok("op", text[i], i, i + 1))       # stray character: keep it as an operator
            i += 1
            line_start = False
            continue
        kind = m.lastgroup
        out.append(Tok(kind, m.group(), i, m.end()))
        if kind in ("ws", "comment"):
            if "\n" in m.group():
                line_start = True
        else:
            line_start = False
        i = m.end()
    return out


def significant(toks: list[Tok]) -> list[Tok]:
    return [t for t in toks if t.kind not in ("ws", "comment")]


def match_brackets(S: list[Tok]) -> dict:
    """Index of every bracket's partner (both directions). Unbalanced brackets raise ValueError."""
    pairs = {")": "(", "]": "[", "}": "{"}
    stack, m = [], {}
    for i, t in enumerate(S):
        if t.kind != "op":
            continue
        if t.text in "([{":
            stack.append(i)
        elif t.text in pairs:
            if not stack or S[stack[-1]].text != pairs[t.text]:
                raise ValueError(f"unbalanced {t.text!r} at offset {t.start}")
            j = stack.pop()
            m[i], m[j] = j, i
    if stack:
        raise ValueError(f"unclosed {S[stack[-1]].text!r} at offset {S[stack[-1]].start}")
    return m


def is_operand_end(t: Tok | None) -> bool:
    """Could `t` end an operand (so a following `*`, `&`, `-`, `+` is binary)?"""
    if t is None:
        return False
    if t.kind in ("num", "str", "chr"):
        return True
    if t.kind == "id":
        return t.text not in KEYWORDS or t.text in ("this", "true", "false")
    return t.text in (")", "]", "++", "--")


def ends_operand(S, M, k) -> bool:
    """Does S[k] end an operand? Like is_operand_end, but the `)` of a cast `(T)` doesn't."""
    if k < 0 or k >= len(S):
        return False
    if S[k].kind == "op" and S[k].text == ")" and k in M and is_cast_group(S, M, M[k]):
        return False
    return is_operand_end(S[k])


# --- statements ----------------------------------------------------------------------------------------------

@dataclass
class Stmt:
    kind: str           # block if while for do switch label decl expr return break continue goto empty pp try
    a: int              # first significant token
    b: int              # one past the last
    kids: list = field(default_factory=list)     # block: statements
    cond: tuple | None = None                    # (a, b) token span of the condition / switch value
    then: "Stmt | None" = None
    els: "Stmt | None" = None
    body: "Stmt | None" = None
    init: tuple | None = None                    # for: spans of the three header parts
    step: tuple | None = None
    parent: "Stmt | None" = None

    def walk(self):
        yield self
        for c in self.children():
            yield from c.walk()

    def children(self):
        out = list(self.kids)
        out += [x for x in (self.then, self.els, self.body) if x is not None]
        return out


class ParseError(ValueError):
    pass


class Parser:
    def __init__(self, S: list[Tok], M: dict):
        self.S, self.M = S, M

    def t(self, i):
        return self.S[i].text if 0 <= i < len(self.S) else None

    def paren(self, i):
        if self.t(i) != "(":
            raise ParseError(f"expected '(' at token {i} ({self.t(i)!r})")
        return i, self.M[i]

    def to_semicolon(self, i, end):
        j = i
        while j < end:
            x = self.S[j]
            if x.kind == "op" and x.text in "([{":
                j = self.M[j] + 1
                continue
            if x.text == ";":
                return j
            if x.kind == "pp" or (x.kind == "op" and x.text == "}"):
                break
            j += 1
        raise ParseError(f"no ';' after token {i}")

    def stmt(self, i, end) -> Stmt:
        S = self.S
        t = S[i]
        x = t.text
        if t.kind == "pp":
            return Stmt("pp", i, i + 1)
        if x == "{" and t.kind == "op":
            j = self.M[i]
            return Stmt("block", i, j + 1, kids=self.seq(i + 1, j))
        if x == ";":
            return Stmt("empty", i, i + 1)
        if t.kind == "id":
            if x == "if":
                lp, rp = self.paren(i + 1)
                th = self.stmt(rp + 1, end)
                e, b = None, th.b
                if b < end and self.t(b) == "else":
                    e = self.stmt(b + 1, end)
                    b = e.b
                return Stmt("if", i, b, cond=(lp + 1, rp), then=th, els=e)
            if x in ("while", "switch"):
                lp, rp = self.paren(i + 1)
                body = self.stmt(rp + 1, end)
                return Stmt(x, i, body.b, cond=(lp + 1, rp), body=body)
            if x == "for":
                lp, rp = self.paren(i + 1)
                semis, j = [], lp + 1
                while j < rp:
                    if S[j].kind == "op" and S[j].text in "([{":
                        j = self.M[j] + 1
                        continue
                    if S[j].text == ";":
                        semis.append(j)
                    j += 1
                if len(semis) != 2:
                    raise ParseError("for header without two ';'")
                body = self.stmt(rp + 1, end)
                return Stmt("for", i, body.b, init=(lp + 1, semis[0]), cond=(semis[0] + 1, semis[1]),
                            step=(semis[1] + 1, rp), body=body)
            if x == "do":
                body = self.stmt(i + 1, end)
                if self.t(body.b) != "while":
                    raise ParseError("do without while")
                lp, rp = self.paren(body.b + 1)
                if self.t(rp + 1) != ";":
                    raise ParseError("do-while without ';'")
                return Stmt("do", i, rp + 2, cond=(lp + 1, rp), body=body)
            if x == "try":
                body = self.stmt(i + 1, end)
                b = body.b
                while self.t(b) == "catch":
                    lp, rp = self.paren(b + 1)
                    b = self.stmt(rp + 1, end).b
                return Stmt("try", i, b)
            if x in ("case", "default"):
                j = i + 1
                depth = 0
                while j < end:
                    if S[j].text == "?":
                        depth += 1
                    elif S[j].text == ":":
                        if depth == 0:
                            return Stmt("label", i, j + 1)
                        depth -= 1
                    j += 1
                raise ParseError("case without ':'")
            if x not in KEYWORDS and self.t(i + 1) == ":":
                return Stmt("label", i, i + 2)
            if x in ("return", "break", "continue", "goto"):
                j = self.to_semicolon(i, end)
                return Stmt(x, i, j + 1)
        j = self.to_semicolon(i, end)
        return Stmt("decl" if is_decl(S, i, j) else "expr", i, j + 1)

    def seq(self, i, end) -> list[Stmt]:
        out = []
        while i < end:
            s = self.stmt(i, end)
            out.append(s)
            i = s.b
        return out


def link_parents(root: Stmt):
    for s in root.walk():
        for c in s.children():
            c.parent = s


def is_decl(S, a, b) -> bool:
    """Is the statement S[a:b] (without ';') a declaration? Heuristic: starts with a type word, or with
    `Name Name`, `Name * Name`, `Name :: ...` followed by a declarator."""
    if a >= b:
        return False
    t0 = S[a]
    if t0.kind != "id":
        return False
    if t0.text in TYPE_WORDS:
        return True
    if t0.text in KEYWORDS:
        return False
    j = a + 1
    while j + 1 < b and S[j].text == "::" and S[j + 1].kind == "id":
        j += 2
    k = j
    while k < b and S[k].text in ("*", "&"):
        k += 1
    if k < b and S[k].kind == "id" and S[k].text not in KEYWORDS:
        nxt = S[k + 1].text if k + 1 < len(S) else ";"
        if k == j:                                   # Name Name
            return True
        return nxt in ("=", ";", "[", ",", ")")      # Name * Name =
    return False


@dataclass
class Declarator:
    name: str
    name_idx: int       # significant index of the name
    a: int              # declarator span (from after the base type), incl. initializer
    b: int
    pointer: bool
    array: bool
    init: tuple | None  # span of the initializer expression


def parse_decl(S, a, b, M) -> tuple[tuple, list[Declarator]] | None:
    """A declaration S[a:b] (b = index of ';' or end) -> ((base type span), declarators). None if odd
    (function declarations, initializer lists of structs are fine)."""
    parts, j, cur = [], a, a
    while j < b:
        if S[j].kind == "op" and S[j].text in "([{":
            j = M[j] + 1
            continue
        if S[j].text == ",":
            parts.append((cur, j))
            cur = j + 1
        j += 1
    parts.append((cur, b))
    first_a, first_b = parts[0]
    # name of the first declarator: the last identifier before '=' / '[' / end at depth 0
    eq = next((k for k in range(first_a, first_b) if S[k].text == "="), first_b)
    k = eq - 1
    while k >= first_a and S[k].text == "]":
        k = M[k] - 1
    if k < first_a or S[k].kind != "id" or S[k].text in KEYWORDS:
        return None
    if S[k - 1].text == "(" if k - 1 >= first_a else False:
        return None                                   # function pointer declarators: leave alone
    base_end = k
    while base_end - 1 >= first_a and S[base_end - 1].text in ("*", "&"):
        base_end -= 1
    if base_end <= first_a:
        return None
    base = (first_a, base_end)
    decls = []
    for n, (pa, pb) in enumerate(parts):
        da = base_end if n == 0 else pa
        eq = next((q for q in range(da, pb) if S[q].text == "="), pb)
        q = eq - 1
        arr = False
        while q >= da and S[q].text == "]":
            arr = True
            q = M[q] - 1
        if q < da or S[q].kind != "id":
            return None
        ptr = any(S[r].text in ("*", "&") for r in range(da, q))
        decls.append(Declarator(S[q].text, q, da, pb, ptr, arr, (eq + 1, pb) if eq < pb else None))
    return base, decls


def split_top(S, a, b, M, sep) -> list[tuple]:
    """Split S[a:b] at top-level tokens equal to `sep`."""
    out, cur, j = [], a, a
    while j < b:
        if S[j].kind == "op" and S[j].text in "([{":
            j = M[j] + 1
            continue
        if S[j].text == sep:
            out.append((cur, j))
            cur = j + 1
        j += 1
    out.append((cur, b))
    return out


def params_of(S, lp, rp, M) -> list[Declarator]:
    """Parameter declarators of a function header `( ... )`."""
    out = []
    for pa, pb in split_top(S, lp + 1, rp, M, ","):
        if pb - pa == 1 and S[pa].text == "void" or pa >= pb or S[pa].text == "...":
            continue
        d = parse_decl(S, pa, pb, M)
        if d:
            out += d[1]
    return out


# --- operands ------------------------------------------------------------------------------------------------

def operand_back(S, M, j, lo) -> int | None:
    """Start index of the postfix expression ending at S[j] (identifiers, literals, member chains, calls,
    subscripts, parenthesised groups). None if S[j] can't end one."""
    while True:
        t = S[j]
        if t.text in (")", "]") and t.kind == "op":
            k = M[j]
            if k - 1 >= lo and (S[k - 1].kind == "id" and S[k - 1].text not in KEYWORDS
                                or S[k - 1].text in (")", "]")):
                j = k - 1
                continue
            if k - 1 >= lo and S[k - 1].kind == "id" and S[k - 1].text == "sizeof":
                return k - 1
            return k
        if t.kind in ("id", "num", "str", "chr"):
            if t.kind == "id" and t.text in KEYWORDS and t.text not in ("this", "true", "false"):
                return None
            if j - 2 >= lo and S[j - 1].text in (".", "->", "::"):
                j -= 2
                continue
            return j
        return None


def typedef_names(S, M) -> set[str]:
    """Collect ordinary and function-pointer typedef names from significant tokens.

    This is a file-wide hint, not C scope resolution or preprocessing. Skip balanced type bodies,
    array bounds and parameter lists so their identifiers do not become typedef names.
    """
    names = set()
    pointer_prefix = {"*", "&", "const", "volatile", "__cdecl", "__stdcall", "__fastcall", "__thiscall"}
    for i, t in enumerate(S):
        if t.kind != "id" or t.text != "typedef":
            continue
        end = i + 1
        while end < len(S) and S[end].text != ";":
            if S[end].kind == "op" and S[end].text in "([{":
                end = M[end] + 1
            else:
                end += 1
        if end == len(S):
            continue
        for a, b in split_top(S, i + 1, end, M, ","):
            k = b - 1
            while k >= a and S[k].text in (")", "]", "}"):
                lp = M[k]
                inner = S[lp + 1:k]
                if S[k].text == ")" and len(inner) >= 2 and inner[-1].kind == "id" \
                        and inner[-1].text not in KEYWORDS and any(x.text == "*" for x in inner[:-1]) \
                        and all(x.text in pointer_prefix for x in inner[:-1]):
                    names.add(inner[-1].text)             # `(cc *name)` before the parameter list
                    break
                k = lp - 1
            else:
                if k >= a and S[k].kind == "id" and S[k].text not in KEYWORDS:
                    names.add(S[k].text)
    return names


def is_cast_group(S, M, i, typedef_names=frozenset()) -> bool:
    """S[i] starts a C cast followed by an operand; optional typedef names supplement the heuristic."""
    if S[i].text != "(":
        return False
    j = M[i]
    inner = S[i + 1:j]
    if not inner or inner[0].kind != "id":
        return False
    if any(t.kind != "id" and t.text not in ("*", "&", "::") for t in inner):
        return False
    if inner[-1].kind == "id" and len(inner) == 1 and inner[0].text not in TYPE_WORDS:
        # `(x)` alone: use declared typedefs when available, otherwise the spelling heuristic.
        nxt = S[j + 1] if j + 1 < len(S) else None
        if not nxt:
            return False
        if inner[0].text in typedef_names:
            return nxt.kind in ("id", "num", "chr", "str") or nxt.text in ("(", "*", "&", "-", "!", "~",
                                                                                       "+", "++", "--")
        if inner[0].text in INT_TYPES:
            return nxt.kind in ("id", "num", "chr") or nxt.text in ("(", "*", "&", "-", "!", "~", "++", "--")
        return (nxt.kind in ("id", "num") or nxt.text == "(") and \
            (inner[0].text[0].isupper() or inner[0].text.endswith("_t"))
    if all(t.kind == "id" for t in inner) and not all(t.text in TYPE_WORDS or t.text in INT_TYPES for t in inner):
        return False                             # `(a b)` that isn't a type
    nxt = S[j + 1] if j + 1 < len(S) else None
    return bool(nxt) and (nxt.kind in ("id", "num", "chr", "str") or nxt.text in ("(", "*", "&", "-", "!", "~",
                                                                                   "+", "++", "--"))


def operand_fwd(S, M, i, hi) -> int | None:
    """End (exclusive) of the unary expression starting at S[i]: prefix operators and casts, then a primary
    with its postfix chain."""
    j = i
    while j < hi:
        t = S[j]
        if t.kind == "op" and t.text in ("!", "~", "-", "+", "*", "&", "++", "--"):
            j += 1
            continue
        if t.kind == "op" and t.text == "(" and is_cast_group(S, M, j):
            j = M[j] + 1
            continue
        if t.kind == "id" and t.text == "sizeof":
            j += 1
            continue
        break
    if j >= hi:
        return None
    t = S[j]
    if t.kind == "op" and t.text == "(":
        j = M[j] + 1
    elif t.kind in ("id", "num", "str", "chr"):
        if t.kind == "id" and t.text in KEYWORDS and t.text not in ("this", "true", "false"):
            return None
        j += 1
        while j + 1 < hi and S[j].text == "::" and S[j + 1].kind == "id":
            j += 2
    else:
        return None
    while j < hi:
        t = S[j]
        if t.text in ("[", "(") and t.kind == "op":
            j = M[j] + 1
        elif t.text in (".", "->") and j + 1 < hi and S[j + 1].kind == "id":
            j += 2
        elif t.text in ("++", "--"):
            j += 1
        else:
            break
    return j


# --- effects -------------------------------------------------------------------------------------------------

CALL = "CALL"


@dataclass
class Effects:
    reads: set = field(default_factory=set)
    writes: set = field(default_factory=set)
    call: bool = False
    side: bool = False          # assignment, ++/--, call
    opaque: bool = False


def _loc_conflict(x, y, escaped) -> bool:
    if x == CALL or y == CALL:
        o = y if x == CALL else x
        return not (o.startswith("L:") and o[2:].split(".")[0] not in escaped)
    kx, vx = x[0], x[2:]
    ky, vy = y[0], y[2:]
    if kx == "L" and ky == "L":
        return vx == vy or vx.startswith(vy + ".") or vy.startswith(vx + ".")
    if kx == "L" or ky == "L":
        lv, other = (vx, y) if kx == "L" else (vy, x)
        return lv.split(".")[0] in escaped and other[0] == "A" and other[2:] == "*"
    if kx == "N" and ky == "N":
        return vx == vy
    if kx == "A" and ky == "A":
        return vx == "*" or vy == "*" or vx == vy
    n, a = (vx, vy) if kx == "N" else (vy, vx)
    return a == "*" or a == n


def conflicts(e1: Effects, e2: Effects, escaped=frozenset()) -> bool:
    if e1.opaque or e2.opaque:
        return True
    for w in e1.writes:
        for o in e2.reads | e2.writes:
            if _loc_conflict(w, o, escaped):
                return True
    for w in e2.writes:
        for o in e1.reads:
            if _loc_conflict(w, o, escaped):
                return True
    return False


def effects(S, M, a, b, locals_: dict) -> Effects:
    """What S[a:b] reads and writes. Locations: `L:name[.field...]` (a local or parameter), `N:name` (any other
    plain name: a global or a member through `this`), `A:field` (a member through a pointer), `A:*` (any
    other memory through a pointer). A call reads and writes CALL (all non-local memory)."""
    e = Effects()
    j = a
    while j < b:
        t = S[j]
        if t.kind == "pp":
            e.opaque = True
            j += 1
            continue
        if t.kind == "op" and t.text in ("++", "--") or (t.kind == "op" and t.text in ASSIGN_OPS):
            e.side = True
        if t.kind == "op" and t.text == "(" and j > a and (S[j - 1].kind == "id" and S[j - 1].text not in KEYWORDS
                                                             or S[j - 1].text in (")", "]")):
            if not (S[j - 1].text == ")" and is_cast_group(S, M, M[j - 1])):
                e.call = e.side = True
        if t.kind == "id" and t.text not in KEYWORDS and not (j > a and S[j - 1].text in (".", "->", "::")) \
                and not (j + 1 < b and S[j + 1].text == "::") and not (j + 1 < len(S) and S[j + 1].text == "("):
            end = j + 1
            loc = (f"L:{t.text}" if t.text in locals_ else f"N:{t.text}")
            info = locals_.get(t.text)
            deref = False
            while end < b:
                x = S[end]
                if x.text == "." and end + 1 < b and S[end + 1].kind == "id":
                    if loc[0] == "L" or loc[0] == "N":
                        loc = loc if loc[0] == "N" else f"{loc}.{S[end + 1].text}"
                    else:
                        loc = f"A:{S[end + 1].text}"
                    end += 2
                elif x.text == "->" and end + 1 < b and S[end + 1].kind == "id":
                    e.reads.add(loc)
                    loc = f"A:{S[end + 1].text}"
                    deref = True
                    end += 2
                elif x.text == "[" and x.kind == "op":
                    sub = effects(S, M, end + 1, M[end], locals_)
                    _merge(e, sub)
                    if loc.startswith("L:") and info is not None and info.array:
                        loc = "L:" + loc[2:].split(".")[0]
                    else:
                        e.reads.add(loc)
                        loc = "A:*"
                        deref = True
                    end = M[end] + 1
                else:
                    break
            # unary * in front (not a multiplication)
            if j > a and S[j - 1].text == "*" and not (j - 2 >= a and ends_operand(S, M, j - 2)):
                e.reads.add(loc)
                loc = "A:*"
                deref = True
            if j > a and S[j - 1].text == "&" and not (j - 2 >= a and ends_operand(S, M, j - 2)) \
                    and not deref:
                j = end                               # address taken: not a read of the value
                continue
            nxt = S[end].text if end < b else None
            prv = S[j - 1].text if j > a else None
            if nxt in ASSIGN_OPS:
                e.writes.add(loc)
                if nxt != "=":
                    e.reads.add(loc)
            elif nxt in ("++", "--") or prv in ("++", "--"):
                e.writes.add(loc)
                e.reads.add(loc)
            else:
                e.reads.add(loc)
            j = end
            continue
        if t.kind == "op" and t.text == "*" and not (j > a and ends_operand(S, M, j - 1)) \
                and j + 1 < b and S[j + 1].text == "(":
            # *(expr) = ... / *(T*)p: memory through a pointer expression
            k = M[j + 1] + 1
            if is_cast_group(S, M, j + 1):
                k2 = operand_fwd(S, M, k, b)
                k = k2 if k2 else k
            nxt = S[k].text if k < b else None
            if nxt in ASSIGN_OPS or nxt in ("++", "--"):
                e.writes.add("A:*")
            e.reads.add("A:*")
        j += 1
    if e.call:
        e.reads.add(CALL)
        e.writes.add(CALL)
    return e


def _merge(e: Effects, o: Effects):
    e.reads |= o.reads
    e.writes |= o.writes
    e.call |= o.call
    e.side |= o.side
    e.opaque |= o.opaque


def escaped_locals(S, a, b, locals_, M=None) -> set:
    """Locals whose address is taken anywhere in S[a:b] (`&x`, `&x.f`, arrays passed by name)."""
    out = set()
    for j in range(a, b):
        t = S[j]
        if t.kind == "id" and t.text in locals_:
            if j > a and S[j - 1].text == "&" and not (j - 2 >= a and (ends_operand(S, M, j - 2) if M else is_operand_end(S[j - 2]))):
                out.add(t.text)
            elif locals_[t.text].array and not (j + 1 < b and S[j + 1].text == "["):
                out.add(t.text)
    return out


def norm(text: str) -> str:
    """Whitespace-insensitive form of an expression (for the type table)."""
    return " ".join(t.text for t in significant(tokenize(text)))
