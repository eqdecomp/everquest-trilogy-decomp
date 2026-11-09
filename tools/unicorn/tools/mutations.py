r"""Semantics-changing source mutations of one function body (for mutate_check.py). Pure text, no compiler.

A small C tokenizer (comments, string/char literals and preprocessor lines are skipped) finds the body of the
function annotated `// FUNCTION: <CLIENT> 0x...` and proposes one-edit mutants:

  rel       relational flip: < <=, > >=, == != (off by one / inverted), < >=, > <=  (negated)
  const     integer literal +1 / -1 (not array sizes in declarations)
  arith     operator swap: + -, * /, % *, & |, ^ |, << >>, && ||, += -=, &= |=, <<= >>=
  swap      operands of a non-commutative operator (- / % < <= > >= << >>), when both are simple operands
  delete    an expression statement (call, assignment, ++/--) -> `;`, or a for loop's increment
  negate    if/while condition -> !(condition); `!x` -> `x`
  return    early return at the top of the body (after leading declarations)

Heuristic by design: a mutant that doesn't compile is reported as such and not counted.
"""
from __future__ import annotations

import re
from dataclasses import dataclass

_ANN = re.compile(r"^\s*//\s*FUNCTION:\s*(\w+)\s+(0x[0-9a-fA-F]+)", re.M)
_OPS = sorted(""">>= <<= -> ++ -- << >> <= >= == != && || += -= *= /= %= &= |= ^= ::
                 + - * / % & | ^ ! ~ < > = ? : ; , . ( ) [ ] { }""".split(), key=len, reverse=True)
_NUM = re.compile(r"0[xX][0-9a-fA-F]+[uUlL]*|\d+\.\d*(?:[eE][-+]?\d+)?[fFlL]?|\d+[eE][-+]?\d+[fFlL]?|\.\d+[fFlL]?|\d+[uUlL]*")
_ID = re.compile(r"[A-Za-z_]\w*")

KEYWORDS_NOT_OPERAND = {"return", "case", "else", "do", "sizeof", "new", "delete", "throw", "goto", "typedef"}
STMT_KEYWORDS = {"return", "break", "continue", "goto", "if", "while", "for", "do", "switch", "case", "default",
                 "else", "try", "catch", "throw", "__asm", "_asm"}
TYPE_WORDS = {"int", "char", "short", "long", "unsigned", "signed", "float", "double", "void", "const",
              "volatile", "struct", "class", "union", "enum", "bool", "static", "register", "auto", "__int64",
              "BOOL", "BYTE", "WORD", "DWORD", "UINT", "ULONG", "LONG", "HANDLE", "HWND", "LPSTR", "LPCSTR",
              "u8", "u16", "u32", "s8", "s16", "s32", "size_t"}


@dataclass
class Tok:
    kind: str       # id num str op
    text: str
    start: int
    end: int


@dataclass
class Mutant:
    kind: str
    start: int
    end: int
    replacement: str
    line: int
    desc: str

    def apply(self, text: str) -> str:
        return text[:self.start] + self.replacement + text[self.end:]


def tokenize(text: str, start: int = 0, end: int | None = None) -> list[Tok]:
    end = len(text) if end is None else end
    out, i = [], start
    line_start = True
    while i < end:
        c = text[i]
        if c == "\n":
            line_start = True
            i += 1
            continue
        if c in " \t\r\f\v":
            i += 1
            continue
        if line_start and c == "#":                       # preprocessor line (with continuations)
            while i < end and text[i] != "\n":
                i += 2 if text[i] == "\\" else 1
            continue
        line_start = False
        if text.startswith("//", i):
            j = text.find("\n", i)
            i = end if j < 0 else j
            continue
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            i = end if j < 0 else j + 2
            continue
        if c in "\"'":
            j = i + 1
            while j < end and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            out.append(Tok("str", text[i:j + 1], i, j + 1))
            i = j + 1
            continue
        m = _NUM.match(text, i) if (c.isdigit() or (c == "." and i + 1 < end and text[i + 1].isdigit())) else None
        if m:
            out.append(Tok("num", m.group(), i, m.end()))
            i = m.end()
            continue
        m = _ID.match(text, i)
        if m:
            out.append(Tok("id", m.group(), i, m.end()))
            i = m.end()
            continue
        for op in _OPS:
            if text.startswith(op, i):
                out.append(Tok("op", op, i, i + len(op)))
                i += len(op)
                break
        else:
            i += 1                                        # stray character (e.g. `@`, `$`)
    return out


def function_body(text: str, va: int, client: str | None = None) -> tuple[int, int, str]:
    """(index of the body's `{`, index just after its `}`, the definition head) for the function annotated
    with `va`. ValueError if absent or ambiguous."""
    hits = [m for m in _ANN.finditer(text) if int(m.group(2), 16) == va
            and (client is None or m.group(1).upper() == client.upper())]
    if len(hits) != 1:
        raise ValueError(f"{len(hits)} FUNCTION annotations for {va:#x}")
    nl = text.find("\n", hits[0].end())
    toks = tokenize(text, len(text) if nl < 0 else nl + 1)
    depth = 0
    for k, t in enumerate(toks):
        if t.text == "(":
            depth += 1
        elif t.text == ")":
            depth -= 1
        elif t.text == ";" and depth == 0:
            raise ValueError(f"{va:#x}: annotation is followed by a declaration, not a definition")
        elif t.text == "{" and depth == 0:
            open_ = t.start
            level = 0
            for u in toks[k:]:
                if u.text == "{":
                    level += 1
                elif u.text == "}":
                    level -= 1
                    if level == 0:
                        return open_, u.end, " ".join(x.text for x in toks[:k])
            raise ValueError(f"{va:#x}: unbalanced braces")
    raise ValueError(f"{va:#x}: no function body found")


def _line(text, pos):
    return text.count("\n", 0, pos) + 1


def _operand_end(t: Tok | None) -> bool:
    if t is None:
        return False
    if t.kind == "id":
        return t.text not in KEYWORDS_NOT_OPERAND
    return t.kind in ("num", "str") or t.text in (")", "]", "++", "--")


def _operand_start(t: Tok | None) -> bool:
    return t is not None and (t.kind in ("id", "num", "str") or t.text in ("(", "~", "!", "-", "+", "*", "&",
                                                                             "++", "--"))


def _match(toks, k, open_, close, step):
    """Index of the bracket matching toks[k] (step +1 forward from an opener, -1 backward from a closer)."""
    level = 0
    for j in range(k, len(toks) if step > 0 else -1, step):
        if toks[j].text == open_:
            level += 1
        elif toks[j].text == close:
            level -= 1
        if level == 0:
            return j
    return None


def _simple_left(toks, k):
    """Start index of a simple operand ending at toks[k]: id/num with member, index and call suffixes."""
    j = k
    while j >= 0:
        t = toks[j]
        if t.text in (")", "]"):
            o = _match(toks, j, t.text, "(" if t.text == ")" else "[", -1)
            if o is None:
                return None
            j = o - 1
            if j >= 0 and toks[j].kind == "id" and toks[j].text not in KEYWORDS_NOT_OPERAND | STMT_KEYWORDS:
                j -= 1                                     # call / index on a name
            elif t.text == "]":
                continue
            else:
                return j + 1                               # a parenthesised expression
        elif t.kind in ("id", "num"):
            j -= 1
        else:
            return None
        if j >= 0 and toks[j].text in (".", "->"):
            j -= 1
            continue
        return j + 1
    return 0


def _simple_right(toks, k):
    """End index (inclusive) of a simple operand starting at toks[k]."""
    j = k
    if j >= len(toks):
        return None
    t = toks[j]
    if t.text == "(":
        return _match(toks, j, "(", ")", 1)
    if t.kind not in ("id", "num"):
        return None
    while True:
        n = toks[j + 1] if j + 1 < len(toks) else None
        if n is None:
            return j
        if n.text in ("(", "["):
            c = _match(toks, j + 1, n.text, ")" if n.text == "(" else "]", 1)
            if c is None:
                return None
            j = c
        elif n.text in (".", "->") and j + 2 < len(toks) and toks[j + 2].kind == "id":
            j += 2
        else:
            return j


LEFT_BOUND = {"(", ",", "=", ";", "{", "}", "return", "&&", "||", "?", ":", "+=", "-=", "*=", "/=", "|=", "&=",
              "^=", "!"}
RIGHT_BOUND = {")", ",", ";", "&&", "||", "?", ":", "]"}

REL = {"<": ["<=", ">="], "<=": ["<", ">"], ">": [">=", "<="], ">=": [">", "<"], "==": ["!="], "!=": ["=="]}
ARITH = {"+": "-", "-": "+", "*": "/", "/": "*", "%": "*", "&": "|", "|": "&", "^": "|", "<<": ">>", ">>": "<<",
         "&&": "||", "||": "&&", "+=": "-=", "-=": "+=", "&=": "|=", "|=": "&=", "<<=": ">>=", ">>=": "<<="}
NONCOMM = {"-", "/", "%", "<", "<=", ">", ">=", "<<", ">>"}


def _is_decl_star(toks, k):
    """`T *name =|;|,|[` at a statement start: a declaration, not a multiplication / bitwise and."""
    pp = toks[k - 2] if k >= 2 else None
    n, nn = (toks[k + 1] if k + 1 < len(toks) else None), (toks[k + 2] if k + 2 < len(toks) else None)
    return ((pp is None or pp.text in (";", "{", "}")) and n is not None and n.kind == "id"
            and nn is not None and nn.text in ("=", ";", ",", "[", ")"))


def _is_declaration(stmt: list[Tok]) -> bool:
    if not stmt:
        return False
    a = stmt[0]
    if a.kind == "id" and a.text in TYPE_WORDS:
        return True
    b = stmt[1] if len(stmt) > 1 else None
    if a.kind == "id" and b is not None and b.kind == "id":
        return True                                        # Foo bar ...
    if a.kind == "id" and b is not None and b.text in ("*", "&") and len(stmt) > 3 and stmt[2].kind == "id" \
            and stmt[3].text in ("=", ";", ",", "["):
        return True                                        # Foo *bar = ...
    if a.kind == "id" and b is not None and b.text == "::":
        # Class::member(...) call vs Class::Type var: a declaration if an id follows the qualified name
        j = 2
        while j + 1 < len(stmt) and stmt[j].kind == "id" and stmt[j + 1].text == "::":
            j += 2
        return j + 1 < len(stmt) and stmt[j + 1].kind == "id"
    return False


def mutants(text: str, va: int, client: str | None = None) -> list[Mutant]:
    open_, close, head = function_body(text, va, client)
    toks = tokenize(text, open_, close)
    out: list[Mutant] = []

    def add(kind, start, end, rep, desc):
        out.append(Mutant(kind, start, end, rep, _line(text, start), desc))

    for k, t in enumerate(toks):
        prev = toks[k - 1] if k else None
        nxt = toks[k + 1] if k + 1 < len(toks) else None
        binary = t.kind == "op" and _operand_end(prev) and _operand_start(nxt)
        # relational
        if t.text in REL and binary:
            for rep in REL[t.text]:
                add("rel", t.start, t.end, rep, f"`{t.text}` -> `{rep}`")
        # arithmetic / logical swaps
        if t.text in ARITH and binary and not (t.text in ("*", "&") and _is_decl_star(toks, k)):
            add("arith", t.start, t.end, ARITH[t.text], f"`{t.text}` -> `{ARITH[t.text]}`")
        # operand swap
        if t.text in NONCOMM and binary and not (t.text == "*" and _is_decl_star(toks, k)):
            ls = _simple_left(toks, k - 1)
            re_ = _simple_right(toks, k + 1)
            if ls is not None and re_ is not None and ls > 0 and re_ + 1 < len(toks) \
                    and toks[ls - 1].text in LEFT_BOUND and toks[re_ + 1].text in RIGHT_BOUND:
                left = text[toks[ls].start:toks[k - 1].end]
                right = text[toks[k + 1].start:toks[re_].end]
                if left != right:
                    add("swap", toks[ls].start, toks[re_].end, f"{right} {t.text} {left}",
                        f"`{left} {t.text} {right}` -> `{right} {t.text} {left}`")
        # integer constants
        if t.kind == "num" and re.fullmatch(r"(0[xX][0-9a-fA-F]+|\d+)[uUlL]*", t.text):
            decl_size = (prev is not None and prev.text == "[" and nxt is not None and nxt.text == "]"
                         and k >= 3 and toks[k - 2].kind == "id" and toks[k - 3].kind == "id")
            if not decl_size:
                m = re.fullmatch(r"(0[xX][0-9a-fA-F]+|\d+)([uUlL]*)", t.text)
                digits = m.group(1)
                hexa = digits[1:2] in ("x", "X")
                v = int(digits, 16) if hexa else int(digits, 8) if len(digits) > 1 and digits[0] == "0" \
                    else int(digits)
                fmt = (lambda n: f"0x{n:X}{m.group(2)}") if hexa else (lambda n: f"{n}{m.group(2)}")
                add("const", t.start, t.end, fmt(v + 1), f"`{t.text}` -> `{fmt(v + 1)}`")
                if v > 0:
                    add("const", t.start, t.end, fmt(v - 1), f"`{t.text}` -> `{fmt(v - 1)}`")
        # negation of if/while conditions
        if t.kind == "id" and t.text in ("if", "while") and nxt is not None and nxt.text == "(":
            c = _match(toks, k + 1, "(", ")", 1)
            inner = toks[k + 2:c] if c is not None else []
            single_rel = (sum(1 for x in inner if x.text in REL) == 1
                          and not any(x.text in ("&&", "||", "?") for x in inner))
            if c is not None and c > k + 2 and not single_rel:    # `!(a == b)` would just repeat `rel`
                cond = text[toks[k + 2].start:toks[c - 1].end]
                add("negate", toks[k + 2].start, toks[c - 1].end, f"!({cond})", f"`{t.text} ({cond})` negated")
        if t.text == "!" and prev is not None and not _operand_end(prev):
            add("negate", t.start, t.end, "", "`!` removed")
        # for-loop increment
        if t.kind == "id" and t.text == "for" and nxt is not None and nxt.text == "(":
            c = _match(toks, k + 1, "(", ")", 1)
            semis = [j for j in range(k + 2, c or k) if toks[j].text == ";"]
            if c is not None and len(semis) == 2 and semis[1] + 1 < c:
                inc = text[toks[semis[1] + 1].start:toks[c - 1].end]
                add("delete", toks[semis[1] + 1].start, toks[c - 1].end, "", f"for increment `{inc}` deleted")

    # statements: delete expression statements; early return after the leading declarations
    for s in _statements(toks):
        if _is_declaration(s):
            continue
        a = s[0]
        if (a.kind == "id" and a.text not in STMT_KEYWORDS) or a.text in ("++", "--", "*", "("):
            if any(x.text in ("=", "(", "++", "--", "+=", "-=", "*=", "/=", "|=", "&=", "^=", "<<=", ">>=",
                              "%=") for x in s):
                body = text[a.start:s[-1].end]
                add("delete", a.start, s[-1].end, "", f"statement `{_short(body)}` deleted")
    first = _first_statement(toks)
    if first is not None:
        words = [w for w in head.split("(")[0].split() if w not in ("static", "inline", "__inline", "virtual")]
        void = bool(words) and words[0] == "void" and "*" not in words
        ctor = re.search(r"(\w+) :: ~?\1 \(", head) is not None
        rep = "return; " if void or ctor else "return 0; "
        add("return", first.start, first.start, rep, f"early `{rep.strip()}` before line {_line(text, first.start)}")
    seen, uniq = set(), []
    for m in out:
        if (m.start, m.end, m.replacement) not in seen:
            seen.add((m.start, m.end, m.replacement))
            uniq.append(m)
    return uniq


KIND_ORDER = ["rel", "const", "arith", "swap", "delete", "negate", "return"]


def select(ms: list[Mutant], n: int | None) -> list[Mutant]:
    """At most n mutants, spread over kinds (round robin) and, within a kind, over the body (evenly spaced
    picks first, so a long body isn't sampled only at the top). Result in source order."""
    if n is None or len(ms) <= n:
        return list(ms)
    by: dict = {}
    for m in ms:
        by.setdefault(m.kind, []).append(m)
    for k, lst in by.items():
        order, seen, step = [], set(), len(lst)
        while len(order) < len(lst):
            step = max(1, step // 2)
            for i in range(0, len(lst), step):
                if i not in seen:
                    seen.add(i)
                    order.append(lst[i])
        by[k] = order
    kinds = KIND_ORDER + [k for k in by if k not in KIND_ORDER]
    out = []
    while len(out) < n:
        for k in kinds:
            if by.get(k) and len(out) < n:
                out.append(by[k].pop(0))
    return sorted(out, key=lambda m: (m.start, m.kind))


def _short(s, n=60):
    s = " ".join(s.split())
    return s if len(s) <= n else s[:n - 3] + "..."


def _first_statement(toks: list[Tok]) -> Tok | None:
    """The first token of the body's first top-level statement that is not a declaration (C needs the
    declarations first), or None for an empty body."""
    k = 1
    while k < len(toks) - 1:
        if toks[k].text == ";":
            k += 1
            continue
        j, depth = k, 0
        while j < len(toks) - 1:                      # this statement's tokens, up to `;` at depth 0
            if toks[j].text in ("(", "{"):
                depth += 1
            elif toks[j].text in (")", "}"):
                depth -= 1
            if (toks[j].text == ";" and depth == 0) or depth < 0:
                break
            j += 1
        stmt = toks[k:j]
        if stmt and _is_declaration(stmt) and not any(t.text == "{" for t in stmt[:2]):
            k = j + 1
            continue
        return toks[k]
    return None


def _statements(toks: list[Tok]) -> list[list[Tok]]:
    """Statements ending in `;` at paren depth 0 inside the body (any brace depth), each as its tokens
    (from after the previous `;`, `{`, `}`, `else`, `:` label or control header)."""
    out, cur, depth = [], [], 0
    k = 1                                                  # skip the body's `{`
    while k < len(toks) - 1:
        t = toks[k]
        if depth == 0 and not cur and t.kind == "id" and t.text in ("if", "while", "for", "switch") \
                and k + 1 < len(toks) and toks[k + 1].text == "(":
            c = _match(toks, k + 1, "(", ")", 1)
            k = (c if c is not None else k) + 1            # skip the header; the controlled statement follows
            continue
        if depth == 0 and not cur and t.text in ("else", "do", "{", "}", ";"):
            k += 1
            continue
        if depth == 0 and not cur and t.kind == "id" and t.text in ("case", "default"):
            while k < len(toks) and toks[k].text != ":":
                k += 1
            k += 1
            continue
        if depth == 0 and not cur and t.kind == "id" and k + 1 < len(toks) and toks[k + 1].text == ":":
            k += 2                                         # label:
            continue
        if t.text == "(":
            depth += 1
        elif t.text == ")":
            depth -= 1
        cur.append(t)
        if t.text == ";" and depth == 0:
            out.append(cur[:-1] if len(cur) > 1 else cur)
            cur = []
        elif t.text in ("{", "}") and depth == 0:
            cur = []                                       # a nested block (or initialiser) starts/ends
        k += 1
    return [s for s in out if s and s[0].text != ";"]


# ---------------------------------------------------------------- scoping (giants: --lines, --max, region markers)

_REGION = re.compile(r"^[ \t]*//[ \t]*REGION[ \t]+(\w+)\b(.*)$")
_END_REGION = re.compile(r"^[ \t]*//[ \t]*END[ \t]+REGION\b")


def parse_line_ranges(specs) -> list[tuple[int, int]]:
    """['10-20', '35'] -> [(10, 20), (35, 35)] (1-based, inclusive). ValueError on junk."""
    out = []
    for s in specs or ():
        m = re.fullmatch(r"\s*(\d+)\s*(?:-\s*(\d+))?\s*", s)
        if not m:
            raise ValueError(f"bad line range {s!r} (want A-B or A)")
        a, b = int(m.group(1)), int(m.group(2) or m.group(1))
        if b < a:
            raise ValueError(f"bad line range {s!r}: end before start")
        out.append((a, b))
    return out


def in_lines(ms: list[Mutant], ranges) -> list[Mutant]:
    """Only the mutants whose line lies in one of the (A, B) ranges."""
    if not ranges:
        return list(ms)
    return [m for m in ms if any(a <= m.line <= b for a, b in ranges)]


def region_blocks(text: str) -> list[tuple[str, int, int]]:
    """The `// REGION Rnn 0xA-0xB: label` ... `// END REGION Rnn` blocks of a source (the skeleton's markers):
    [(id, first line, last line)], 1-based, marker lines included. A block ends at its END REGION line or at
    the next REGION marker (the last one at the end of the text)."""
    lines = text.split("\n")
    out, cur = [], None
    for i, ln in enumerate(lines, 1):
        m = _REGION.match(ln)
        if m:
            if cur is not None:
                out.append((cur[0], cur[1], i - 1))
            cur = (m.group(1), i)
        elif _END_REGION.match(ln) and cur is not None:
            out.append((cur[0], cur[1], i))
            cur = None
    if cur is not None:
        out.append((cur[0], cur[1], len(lines)))
    return out


def region_of_line(blocks, line: int):
    for rid, a, b in blocks:
        if a <= line <= b:
            return rid
    return None


def scaled_max(insns: int | None, base: int = 30, per_100: int = 1, cap: int = 250) -> int:
    """Default number of mutants for a function of `insns` original instructions: 30 + 1 per 100, at most 250
    (30 when the size is unknown)."""
    if not insns:
        return base
    return min(cap, base + per_100 * (insns // 100))


def select_per_region(ms: list[Mutant], blocks, per_region: int | None) -> tuple[list[Mutant], dict]:
    """Up to `per_region` mutants (spread over kinds and the block, as select()) in each region block.
    -> (mutants in source order, {region id: how many mutants it had}). Mutants outside every block are
    left out (they show up under None in the counts)."""
    by: dict = {}
    for m in ms:
        by.setdefault(region_of_line(blocks, m.line), []).append(m)
    chosen = []
    for rid, group in by.items():
        if rid is None:
            continue
        chosen.extend(select(group, per_region))
    return sorted(chosen, key=lambda m: (m.start, m.kind)), {k: len(v) for k, v in by.items()}
