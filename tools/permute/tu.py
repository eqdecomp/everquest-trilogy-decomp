"""Cut one annotated function out of its source into a smaller translation unit for the permuter.

The unit keeps everything at file scope (includes, declarations, classes, globals, `// CFLAGS:` lines) and
the definitions of the functions the target refers to (transitively); every other function definition, with
the comments in front of it (its annotation), is dropped. permute.py checks that the target compiles to the
same code in the unit as in the full source and falls back to the full source otherwise.
"""
from __future__ import annotations

import re
from dataclasses import dataclass

from ctoken import KEYWORDS, match_brackets, params_of, significant, tokenize

_CFLAGS_LINE = re.compile(r"^\s*(?://|/\*|\*)\s*(?:CFLAGS|TOOLCHAIN):.*$", re.M)   # kept in every unit


@dataclass
class FuncDef:
    name: str               # qualified name as written (Class::Name)
    lead: int               # char offset where the stuff in front of the definition starts (prev item end)
    start: int              # first char of the definition's head
    body_start: int         # the '{'
    body_end: int           # one past the '}'
    params: list            # ctoken.Declarator
    words: set              # identifiers used in the body

    @property
    def simple(self):
        return self.name.split("::")[-1]


def find_functions(text: str) -> list[FuncDef]:
    toks = tokenize(text)
    S = significant(toks)
    M = match_brackets(S)
    out = []
    i, item = 0, 0
    prev_end = 0
    while i < len(S):
        t = S[i]
        if t.kind == "pp":
            i += 1
            item = i
            prev_end = t.end
            continue
        if t.kind == "op" and t.text == ";":
            i += 1
            item = i
            prev_end = t.end
            continue
        if t.kind == "op" and t.text in "([":
            i = M[i] + 1
            continue
        if t.kind == "op" and t.text == "{":
            j = M[i]
            f = _as_function(text, S, M, item, i)
            if f:
                out.append(FuncDef(f[0], prev_end, S[item].start, t.start, S[j].end, f[1],
                                   {x.text for x in S[i:j] if x.kind == "id"}))
                i = j + 1
                item = i
                prev_end = S[j].end
                continue
            i = j + 1                              # class/struct/enum/extern "C"/initializer: keep going
            if item < len(S) and S[item].kind == "id" and S[item].text in ("extern", "namespace") \
                    and S[i - 1].text == "}":
                item = i
                prev_end = S[j].end
            continue
        i += 1
    return out


def _as_function(text, S, M, a, brace):
    """(qualified name, params) when S[a:brace] is a function definition head."""
    if a >= brace:
        return None
    if S[a].kind == "id" and S[a].text in ("struct", "class", "union", "enum", "namespace", "typedef", "extern") \
            and not (S[a].text == "extern" and S[a + 1].kind == "id"):
        if S[a].text in ("struct", "class", "union", "enum") and not any(S[k].text == "(" for k in range(a, brace)):
            return None
        if S[a].text in ("namespace", "typedef"):
            return None
        if S[a].text == "extern":
            return None
    lp = None
    k = a
    while k < brace:
        x = S[k]
        if x.kind == "op" and x.text == "=":
            return None
        if x.kind == "op" and x.text == "(":
            lp = k
            break
        k += 1
    if lp is None:
        return None
    n = lp - 1
    if n < a or S[n].kind != "id" or (S[n].text in KEYWORDS and S[n].text != "operator"):
        return None
    parts = [S[n].text]
    if n - 1 >= a and S[n - 1].text == "~":
        parts[-1] = "~" + parts[-1]
        n -= 1
    while n - 2 >= a and S[n - 1].text == "::" and S[n - 2].kind == "id":
        n -= 2
        parts.insert(0, S[n].text)
    return "::".join(parts), params_of(S, lp, M[lp], M)


def find_target(text: str, ann_line: int) -> FuncDef | None:
    """The first definition whose head starts after the annotation line (1-based)."""
    pos = 0
    for _ in range(ann_line):
        nl = text.find("\n", pos)
        pos = len(text) if nl < 0 else nl + 1
    for f in find_functions(text):
        if f.start >= pos:
            return f
    return None


@dataclass
class Unit:
    prefix: str             # TU text before the target's body
    body: str               # the target's body `{ ... }`
    suffix: str             # TU text after it
    full: bool              # True: the unit is the whole source

    def text(self, body=None) -> str:
        return self.prefix + (self.body if body is None else body) + self.suffix


def extract(text: str, target: FuncDef, defs: list[FuncDef] | None = None) -> Unit:
    """The target's unit: file scope + the definitions the target needs (by name, transitively)."""
    defs = defs if defs is not None else find_functions(text)
    target = next(f for f in defs if f.body_start == target.body_start)
    by_simple: dict = {}
    for f in defs:
        by_simple.setdefault(f.simple, []).append(f)
    keep = {id(target)}
    todo = [target]
    while todo:
        f = todo.pop()
        for w in f.words:
            for g in by_simple.get(w, []):
                if id(g) not in keep:
                    keep.add(id(g))
                    todo.append(g)
    drops = sorted((f.lead, f.body_end) for f in defs if id(f) not in keep)
    out, pos, n, bs = [], 0, 0, None
    for s, e in drops + [(len(text), len(text))]:
        if pos <= target.body_start < s:
            bs = n + target.body_start - pos
        out.append(text[pos:s])
        n += s - pos
        if s < len(text):
            out.append("\n")
            n += 1
        pos = e
    joined = "".join(out)
    be = bs + (target.body_end - target.body_start)
    assert joined[bs:be] == text[target.body_start:target.body_end]
    pre, body, suf = joined[:bs], joined[bs:be], joined[be:]
    lost = [m.group().strip() for m in _CFLAGS_LINE.finditer(text) if m.group() not in pre + suf]
    if lost:
        pre = "\n".join(lost) + "\n" + pre
    return Unit(pre, body, suf, full=False)


def whole(text: str, target: FuncDef) -> Unit:
    return Unit(text[:target.body_start], text[target.body_start:target.body_end], text[target.body_end:], True)
