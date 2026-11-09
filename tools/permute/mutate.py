"""Source mutations for the permuter: small, (mostly) semantics-preserving rewrites of one function body
that are known to move VC6's code generation (statement and declaration order, operand order, branch
polarity, loop form, temporaries, local types, ...).

Every mutation takes a parsed `Body` and a `random.Random` and returns the new body text, or None when it
found nothing to change. Mutations are text splices on token offsets; the rest of the body is left as is.
Semantics: each mutation checks what it can cheaply (side effects, evaluation order, `continue` in loops,
dangling else, ...). Some are value-changing by nature and flagged `UNSAFE` (local type changes, integer
constant comparisons, casts); every winning candidate is verified with the function's Unicorn fixtures
anyway (permute.py).
"""
from __future__ import annotations

import random
import re

from ctoken import (ASSIGN_OPS, INT_TYPES, KEYWORDS, PREC, Effects, Parser, ParseError, Stmt, conflicts,
                    effects, ends_operand, escaped_locals, is_cast_group, is_decl, is_operand_end, link_parents, match_brackets, norm, operand_back,
                    operand_fwd, parse_decl, significant, tokenize)

JUMPS = ("return", "break", "continue", "goto")


class Local:
    def __init__(self, decl, stmt=None, base=None):
        self.decl, self.stmt, self.base = decl, stmt, base
        self.array, self.pointer = decl.array, decl.pointer


class Body:
    """A function body `{ ... }` parsed into statements, with its locals and parameters."""

    def __init__(self, text: str, params=(), cpp=True, types=None):
        self.text = text
        self.toks = tokenize(text)
        self.S = significant(self.toks)
        self.M = match_brackets(self.S)
        if not self.S or self.S[0].text != "{" or self.M.get(0) != len(self.S) - 1:
            raise ParseError("body is not one { } block")
        self.root = Parser(self.S, self.M).stmt(0, len(self.S))
        link_parents(self.root)
        self.cpp = cpp
        self.types = types or {}
        self.locals: dict = {p.name: Local(p) for p in params}
        self.decl_of: dict = {}
        for st in self.root.walk():
            if st.kind == "decl":
                d = parse_decl(self.S, st.a, st.b - 1, self.M)
                if d:
                    for dc in d[1]:
                        self.locals[dc.name] = Local(dc, st, d[0])
                    self.decl_of[id(st)] = d
        self.escaped = escaped_locals(self.S, 0, len(self.S), self.locals, self.M)
        self.blocks = [s for s in self.root.walk() if s.kind == "block"]

    # --- text helpers ---
    def span(self, a, b) -> str:
        return self.text[self.S[a].start:self.S[b - 1].end] if b > a else ""

    def stext(self, st: Stmt) -> str:
        return self.span(st.a, st.b)

    def cpos(self, a, b):
        return self.S[a].start, self.S[b - 1].end

    def indent(self, i) -> str:
        ls = self.text.rfind("\n", 0, self.S[i].start) + 1
        m = re.match(r"[ \t]*", self.text[ls:])
        return m.group()

    def edit(self, edits) -> str:
        """Apply (start, end, replacement) character edits (non-overlapping)."""
        out = self.text
        for s, e, r in sorted(edits, key=lambda x: x[0], reverse=True):
            out = out[:s] + r + out[e:]
        return out

    def eff(self, a, b) -> Effects:
        return effects(self.S, self.M, a, b, self.locals)

    def stmt_eff(self, st: Stmt) -> Effects:
        if st.kind == "decl":
            d = self.decl_of.get(id(st))
            e = Effects()
            if not d:
                e.opaque = True
                return e
            for dc in d[1]:
                e.writes.add(f"L:{dc.name}")
                if dc.init:
                    sub = self.eff(*dc.init)
                    e.reads |= sub.reads
                    e.writes |= sub.writes
                    e.call |= sub.call
                    e.side |= sub.side
                    e.opaque |= sub.opaque
            return e
        if st.kind in ("expr", "return"):
            return self.eff(st.a, st.b - 1)
        e = Effects()
        e.opaque = True
        return e

    def chunks(self, blk: Stmt):
        """The block's statements as text chunks (each with the whitespace/comments before it)."""
        pos = self.S[blk.a].end
        out = []
        for k in blk.kids:
            end = self.S[k.b - 1].end
            out.append(self.text[pos:end])
            pos = end
        return out, pos

    def rebuild(self, blk: Stmt, chunks) -> str:
        _, pos = self.chunks(blk)
        return self.text[:self.S[blk.a].end] + "".join(chunks) + self.text[pos:]

    def reorder(self, blk: Stmt, order) -> str:
        """The block's statements in a new order; the whitespace between them stays where it was (comments
        travel with the statement after them)."""
        ch, _ = self.chunks(blk)
        ws = [re.match(r"\s*", c).group() for c in ch]
        rest = [c[len(w):] for c, w in zip(ch, ws)]
        return self.rebuild(blk, [ws[k] + rest[o] for k, o in enumerate(order)])

    def uses(self, name, a, b) -> int:
        return sum(1 for j in range(a, b) if self.S[j].kind == "id" and self.S[j].text == name
                   and not (j > 0 and self.S[j - 1].text in (".", "->", "::")))

    def fresh(self, base="temp") -> str:
        names = {t.text for t in self.S if t.kind == "id"}
        n = base
        k = 2
        while n in names:
            n = f"{base}{k}"
            k += 1
        return n


# --- binary operators ------------------------------------------------------------------------------------------

def _is_binary(S, k, lo, M=None) -> bool:
    return k - 1 >= lo and (ends_operand(S, M, k - 1) if M is not None else is_operand_end(S[k - 1]))


def _unary_back(S, M, la, lo):
    """Extend an operand start over prefix operators and casts in front of it (`-x`, `*(T*)&x`)."""
    if la is None:
        return None
    while la - 1 >= lo:
        q = S[la - 1]
        if q.kind == "op" and q.text in ("!", "~", "-", "+", "*", "&", "++", "--") and \
                not (la - 2 >= lo and ends_operand(S, M, la - 2)):
            la -= 1
            continue
        if q.kind == "op" and q.text == ")" and M[la - 1] >= lo and is_cast_group(S, M, M[la - 1]):
            la = M[la - 1]
            continue
        break
    return la


def bin_operands(S, M, k, lo, hi):
    """(la, lb, ra, rb) token spans of the operands of the binary operator S[k] within S[lo:hi], or None."""
    op = S[k].text
    p = PREC.get(op)
    if p is None or not _is_binary(S, k, lo, M):
        return None
    la = _unary_back(S, M, operand_back(S, M, k - 1, lo), lo)
    if la is None:
        return None
    while la - 1 >= lo:                          # absorb tighter (or equal, left-assoc) operators on the left
        q = S[la - 1]
        if q.kind == "op" and q.text in PREC and q.text not in ("?", ":", ",") and PREC[q.text] >= p \
                and _is_binary(S, la - 1, lo, M):
            nla = _unary_back(S, M, operand_back(S, M, la - 2, lo), lo)
            if nla is None:
                return None
            la = nla
            continue
        break
    if la - 1 >= lo:
        q = S[la - 1]
        ok = (q.kind == "op" and q.text in ("(", "[", ",", ";", "{", "}", "?", ":")) or \
             (q.kind == "id" and q.text in ("return", "case")) or \
             (q.kind == "op" and q.text in PREC and _is_binary(S, la - 1, lo, M) and PREC[q.text] < p)
        if q.text == "(" and la - 2 >= lo and ends_operand(S, M, la - 2):
            ok = True                            # a call's argument list
        if not ok:
            return None
    rb = operand_fwd(S, M, k + 1, hi)
    if rb is None:
        return None
    while rb < hi:
        q = S[rb]
        if q.kind == "op" and q.text in PREC and q.text not in ("?", ":", ",") and PREC[q.text] > p:
            nrb = operand_fwd(S, M, rb + 1, hi)
            if nrb is None:
                return None
            rb = nrb
            continue
        break
    if rb < hi:
        q = S[rb]
        if not (q.text in (")", "]", ",", ";", "}", ":", "?") or (q.text in PREC and PREC[q.text] <= p)):
            return None
    return la, k, k + 1, rb


def expr_spans(B: Body):
    """Token spans holding expressions: expression/return statements, initializers, conditions, for parts."""
    out = []
    for st in B.root.walk():
        if st.kind in ("expr", "return"):
            out.append((st.a, st.b - 1, st))
        elif st.kind == "decl":
            d = B.decl_of.get(id(st))
            for dc in (d[1] if d else []):
                if dc.init:
                    out.append((dc.init[0], dc.init[1], st))
        if st.cond:
            out.append((st.cond[0], st.cond[1], st))
        if st.kind == "for":
            out.append((st.init[0], st.init[1], st))
            out.append((st.step[0], st.step[1], st))
    return out


def _ops_in(B, ops):
    res = []
    for lo, hi, st in expr_spans(B):
        for k in range(lo, hi):
            t = B.S[k]
            if t.kind == "op" and t.text in ops:
                r = bin_operands(B.S, B.M, k, lo, hi)
                if r:
                    res.append((r, st))
    return res


COMMUTATIVE = ("+", "*", "&", "|", "^", "==", "!=")
FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}


def _swap_operands(B, r, newop=None):
    la, k, ra, rb = r
    a_txt, b_txt = B.span(la, k), B.span(ra, rb)
    mid = B.text[B.S[k - 1].end:B.S[ra].start]
    if newop:
        mid = mid.replace(B.S[k].text, newop, 1)
    if _top_ops(B.S, B.M, la, k):                 # `a - b + c` -> `c + (a - b)`
        a_txt = f"({a_txt})"
    s, e = B.S[la].start, B.S[rb - 1].end
    return B.edit([(s, e, b_txt + mid + a_txt)])


def m_commute(B: Body, rng):
    """a OP b -> b OP a for + * & | ^ == != (when evaluating either side can't affect the other)."""
    c = [(r, st) for r, st in _ops_in(B, COMMUTATIVE)
         if not conflicts(B.eff(r[0], r[1]), B.eff(r[2], r[3]), B.escaped)]
    if not c:
        return None
    r, _ = rng.choice(c)
    return _swap_operands(B, r)


def m_flip_relational(B: Body, rng):
    """a < b -> b > a (and <=, >=, >)."""
    c = [(r, st) for r, st in _ops_in(B, FLIP)
         if not conflicts(B.eff(r[0], r[1]), B.eff(r[2], r[3]), B.escaped)]
    if not c:
        return None
    r, _ = rng.choice(c)
    return _swap_operands(B, r, FLIP[B.S[r[1]].text])


_INT_LIT = re.compile(r"^(0[xX][0-9a-fA-F]+|[1-9]\d*|0)([uUlL]*)$")


def _int_lit(t):
    if t.kind != "num":
        return None
    m = _INT_LIT.match(t.text)
    if not m:
        return None
    return int(m.group(1), 0), m.group(1).lower().startswith("0x"), m.group(2), m.group(1)


def _fmt_lit(v, hexa, suffix, orig):
    if hexa:
        digits = orig[2:]
        s = f"{v:X}" if any(ch.isupper() for ch in digits) else f"{v:x}"
        s = s.rjust(len(digits), "0") if len(digits) > len(s) else s
        return orig[:2] + s + suffix
    return f"{v}{suffix}"


_NAMED_CONST = re.compile(r"^[A-Z][A-Z0-9_]*$")


def m_const_compare(B: Body, rng):
    """UNSAFE for float operands: x < C <-> x <= C-1, x > C <-> x >= C+1 (C an integer literal, or an
    all-caps name taken as a constant: x >= N -> x > (N - 1))."""
    c = []
    for r, st in _ops_in(B, FLIP):
        la, k, ra, rb = r
        for side, (a, b) in (("r", (ra, rb)), ("l", (la, k))):
            if b - a == 1 and (_int_lit(B.S[a]) or _NAMED_CONST.match(B.S[a].text) and B.S[a].kind == "id"):
                c.append((r, side, a))
    if not c:
        return None
    (la, k, ra, rb), side, a = rng.choice(c)
    op = B.S[k].text
    if not _int_lit(B.S[a]):                     # a named bound (macro / enumerator): N -> (N - 1)
        nop, dv = {"<": ("<=", "-"), "<=": ("<", "+"), ">": (">=", "+"), ">=": (">", "-")}[FLIP[op] if side == "l" else op]
        if side == "l":
            nop = FLIP[nop]
        return B.edit([(B.S[k].start, B.S[k].end, nop), (B.S[a].start, B.S[a].end, f"({B.S[a].text} {dv} 1)")])
    v, hexa, suffix, orig = _int_lit(B.S[a])
    # normalise to "x OP C" (constant on the right)
    if side == "l":
        op = FLIP[op]
    table = {"<": ("<=", -1), "<=": ("<", +1), ">": (">=", +1), ">=": (">", -1)}
    nop, dv = table[op]
    nv = v + dv
    if nv < 0 or nv > 0xFFFFFFFF:
        return None
    if side == "l":
        nop = FLIP[nop]
    return B.edit([(B.S[k].start, B.S[k].end, nop), (B.S[a].start, B.S[a].end, _fmt_lit(nv, hexa, suffix, orig))])


# --- conditions ------------------------------------------------------------------------------------------------

def _top_ops(S, M, a, b):
    out, j = [], a
    while j < b:
        t = S[j]
        if t.kind == "op" and t.text in "([{":
            j = M[j] + 1
            continue
        if t.kind == "op" and t.text in PREC and (t.text not in ("*", "&", "-", "+") or _is_binary(S, j, a, M)):
            out.append(j)
        j += 1
    return out


def negate(B: Body, a, b) -> str:
    S, M = B.S, B.M
    txt = B.span(a, b)
    if S[a].text == "!" and b - a >= 2:
        if S[a + 1].text == "(" and M[a + 1] == b - 1:
            return B.span(a + 2, b - 1)
        if operand_fwd(S, M, a + 1, b) == b:
            return B.span(a + 1, b)
    ops = _top_ops(S, M, a, b)
    if len(ops) == 1 and S[ops[0]].text in ("==", "!="):
        k = ops[0]
        new = "!=" if S[k].text == "==" else "=="
        return txt[:S[k].start - S[a].start] + new + txt[S[k].end - S[a].start:]
    if operand_fwd(S, M, a, b) == b:
        return "!" + txt
    return "!(" + txt + ")"


def _wrap(B: Body, st: Stmt, ind: str) -> str:
    t = B.stext(st)
    return t if st.kind == "block" else "{\n" + ind + "    " + t + "\n" + ind + "}"


def m_invert_if(B: Body, rng):
    """if (c) A else B -> if (!c) B else A."""
    c = [s for s in B.root.walk() if s.kind == "if" and s.els is not None]
    if not c:
        return None
    st = rng.choice(c)
    ind = B.indent(st.a)
    new_then = B.stext(st.els)
    if st.els.kind not in ("block",) and st.els.kind == "if":
        new_then = _wrap(B, st.els, ind)
    new_else = B.stext(st.then)
    cs, ce = B.cpos(*st.cond)
    return B.edit([(cs, ce, negate(B, *st.cond)), (*B.cpos(st.then.a, st.then.b), new_then),
                   (*B.cpos(st.els.a, st.els.b), new_else)])


def m_invert_if_noelse(B: Body, rng):
    """if (c) A -> if (!c) {} else A."""
    c = [s for s in B.root.walk() if s.kind == "if" and s.els is None]
    if not c:
        return None
    st = rng.choice(c)
    ind = B.indent(st.a)
    cs, ce = B.cpos(*st.cond)
    then = B.stext(st.then)
    ts, te = B.cpos(st.then.a, st.then.b)
    return B.edit([(cs, ce, negate(B, *st.cond)), (ts, te, "{\n" + ind + "}\n" + ind + "else\n" + ind
                                                   + ("" if st.then.kind == "block" else "    ") + then)])


def _ends_in_jump(st: Stmt) -> bool:
    if st.kind in JUMPS:
        return True
    if st.kind == "block" and st.kids:
        return _ends_in_jump(st.kids[-1])
    return False


def m_else_after_jump(B: Body, rng):
    """if (c) { ...; return; } else S  <->  if (c) { ...; return; } S."""
    c = []
    for blk in B.blocks:
        for i, st in enumerate(blk.kids):
            if st.kind == "if" and _ends_in_jump(st.then):
                if st.els is not None and st.els.kind != "if":
                    c.append(("drop", blk, i, st))
                elif st.els is None and i + 1 < len(blk.kids):
                    rest = blk.kids[i + 1:]
                    if all(r.kind not in ("label", "pp") for r in rest) and (B.cpp or all(r.kind != "decl" for r in rest)):
                        c.append(("add", blk, i, st))
    if not c:
        return None
    how, blk, i, st = rng.choice(c)
    ind = B.indent(st.a)
    if how == "drop":
        then_end = B.S[st.then.b - 1].end
        els = st.els
        if els.kind == "block" and not any(k.kind == "decl" for k in els.kids) and els.kids:
            inner = B.span(els.kids[0].a, els.kids[-1].b)
            inner = re.sub(r"\n" + re.escape(ind) + "    ", "\n" + ind, inner)
        else:
            inner = B.stext(els)
        return B.edit([(then_end, B.S[els.b - 1].end, "\n" + ind + inner)])
    rest = blk.kids[i + 1:]
    s, e = B.S[st.b - 1].end, B.S[rest[-1].b - 1].end
    body = B.text[B.S[rest[0].a].start:e]
    if len(rest) == 1 and rest[0].kind not in ("decl",):
        return B.edit([(s, e, "\n" + ind + "else\n" + ind + "    " + body.replace("\n", "\n    "))])
    body = body.replace("\n", "\n    ")
    return B.edit([(s, e, "\n" + ind + "else\n" + ind + "{\n" + ind + "    " + body + "\n" + ind + "}")])


def m_split_and(B: Body, rng):
    """if (a && b) S  ->  if (a) { if (b) S }   (no else)."""
    c = []
    for st in B.root.walk():
        if st.kind == "if" and st.els is None:
            ops = [k for k in _top_ops(B.S, B.M, *st.cond) if B.S[k].text in ("&&", "||", "?", ",")]
            if ops and B.S[ops[0]].text == "&&" and all(B.S[k].text == "&&" for k in ops):
                c.append((st, ops))
    if not c:
        return None
    st, ops = rng.choice(c)
    k = rng.choice(ops)
    a, b = st.cond
    ind = B.indent(st.a)
    first, second = B.span(a, k), B.span(k + 1, b)
    then = B.stext(st.then)
    s, e = B.S[st.a].start, B.S[st.b - 1].end
    inner_ind = ind + "    "
    then = then.replace("\n", "\n    ")
    new = (f"if ({first})\n{ind}{{\n{inner_ind}if ({second})\n{inner_ind}"
           + ("" if st.then.kind == "block" else "    ") + then + f"\n{ind}}}")
    return B.edit([(s, e, new)])


def m_merge_if(B: Body, rng):
    """if (a) { if (b) S }  ->  if (a && b) S   (neither has an else)."""
    c = []
    for st in B.root.walk():
        if st.kind == "if" and st.els is None:
            inner = st.then
            if inner.kind == "block" and len(inner.kids) == 1:
                inner = inner.kids[0]
            if inner.kind == "if" and inner.els is None:
                c.append((st, inner))
    if not c:
        return None
    st, inner = rng.choice(c)

    def part(a, b):
        ops = [B.S[k].text for k in _top_ops(B.S, B.M, a, b)]
        t = B.span(a, b)
        return f"({t})" if any(o in ("||", "?", ",", "=") or o in ASSIGN_OPS for o in ops) else t
    ind = B.indent(st.a)
    then = B.stext(inner.then)
    then = re.sub(r"\n" + re.escape(ind) + "        ", "\n" + ind + "    ", then)
    if inner.then.kind != "block":
        then = "    " + then
    new = f"if ({part(*st.cond)} && {part(*inner.cond)})\n{ind}{then}"
    return B.edit([(B.S[st.a].start, B.S[st.b - 1].end, new)])


def _cond_spans(B):
    for st in B.root.walk():
        if st.kind in ("if", "while", "do", "for") and st.cond and st.cond[1] > st.cond[0]:
            yield st.cond


def m_zero_compare(B: Body, rng):
    """In a condition: `x != 0` <-> `x`, `x == 0` <-> `!x`."""
    S, M = B.S, B.M
    c = []
    for a, b in _cond_spans(B):
        bounds = {a} | {k + 1 for k in _top_ops(S, M, a, b) if S[k].text in ("&&", "||")}
        ends = {b} | {k for k in _top_ops(S, M, a, b) if S[k].text in ("&&", "||")}
        for s in bounds:
            e = next((x for x in sorted(ends) if x > s), b)
            # x == 0 / x != 0
            if e - s >= 3 and S[e - 1].text == "0" and S[e - 2].text in ("==", "!="):
                if operand_fwd(S, M, s, e) == e - 2:
                    c.append(("drop", s, e))
            elif S[s].text == "!" and operand_fwd(S, M, s + 1, e) == e:
                c.append(("bang", s, e))
            elif operand_fwd(S, M, s, e) == e and S[s].text != "!":
                c.append(("add", s, e))
    if not c:
        return None
    how, s, e = rng.choice(c)
    if how == "drop":
        txt = B.span(s, e - 2)
        return B.edit([(S[s].start, S[e - 1].end, txt if S[e - 2].text == "!=" else "!" + txt)])
    if how == "bang":
        return B.edit([(S[s].start, S[e - 1].end, B.span(s + 1, e) + " == 0")])
    return B.edit([(S[s].start, S[e - 1].end, B.span(s, e) + (" != 0" if rng.random() < 0.7 else ""))]) \
        if True else None


# --- statements ------------------------------------------------------------------------------------------------

def _swappable(B: Body, s1: Stmt, s2: Stmt) -> bool:
    """Can s1 (first) and s2 (right after it) trade places?"""
    if s1.kind not in ("expr", "decl") or s2.kind not in ("expr", "decl"):
        return False
    if not B.cpp and s1.kind != s2.kind:
        return False
    return not conflicts(B.stmt_eff(s1), B.stmt_eff(s2), B.escaped)


def m_swap_stmts(B: Body, rng, decls=False):
    c = []
    for blk in B.blocks:
        for i in range(len(blk.kids) - 1):
            s1, s2 = blk.kids[i], blk.kids[i + 1]
            both = s1.kind == "decl" and s2.kind == "decl"
            if both == decls and _swappable(B, s1, s2):
                c.append((blk, i))
    if not c:
        return None
    blk, i = rng.choice(c)
    order = list(range(len(blk.kids)))
    order[i], order[i + 1] = i + 1, i
    return B.reorder(blk, order)


def m_swap_decls(B: Body, rng):
    """Swap two adjacent declarations (stack slot / register order)."""
    return m_swap_stmts(B, rng, decls=True)


def m_move_stmt(B: Body, rng):
    """Move a statement up or down past two or more independent neighbours."""
    c = []
    for blk in B.blocks:
        kids, n = blk.kids, len(blk.kids)
        for i in range(n):
            j = i
            while j + 1 < n and _swappable(B, kids[i], kids[j + 1]):
                j += 1
                if j - i >= 2:
                    c.append((blk, i, j))
            j = i
            while j - 1 >= 0 and _swappable(B, kids[j - 1], kids[i]):
                j -= 1
                if i - j >= 2:
                    c.append((blk, i, j))
    if not c:
        return None
    blk, i, j = rng.choice(c)
    order = list(range(len(blk.kids)))
    order.insert(j, order.pop(i))
    return B.reorder(blk, order)


def m_move_decl(B: Body, rng):
    """Move a declaration without initializer anywhere before its first use (C: among the declarations)."""
    c = []
    for blk in B.blocks:
        kids = blk.kids
        ndecl = next((k for k, o in enumerate(kids) if o.kind != "decl"), len(kids))
        for i, st in enumerate(kids):
            d = B.decl_of.get(id(st))
            if not d or any(dc.init for dc in d[1]):
                continue
            names = [dc.name for dc in d[1]]
            first = next((k for k, o in enumerate(kids) if o is not st and
                          any(B.uses(nm, o.a, o.b) for nm in names)), len(kids))
            hi = first if B.cpp else min(first, ndecl)
            c += [(blk, i, j) for j in range(0, hi) if j != i]
    if not c:
        return None
    blk, i, j = rng.choice(c)
    order = list(range(len(blk.kids)))
    order.insert(j, order.pop(i))
    return B.reorder(blk, order)


def m_split_decl_init(B: Body, rng):
    """`T x = e;` <-> `T x; x = e;` (one declarator)."""
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i, st in enumerate(kids):
            d = B.decl_of.get(id(st))
            if not d or len(d[1]) != 1:
                continue
            dc = d[1][0]
            if dc.init and not dc.array and B.S[dc.init[0]].text != "{":
                nxt = kids[i + 1] if i + 1 < len(kids) else None
                if B.cpp or nxt is None or nxt.kind != "decl":
                    c.append(("split", st, dc, None))
            elif not dc.init and i + 1 < len(kids):
                n = kids[i + 1]
                if n.kind == "expr" and B.S[n.a].text == dc.name and B.S[n.a + 1].text == "=" \
                        and B.uses(dc.name, n.a + 2, n.b) == 0:
                    if B.cpp or i + 2 >= len(kids) or True:
                        c.append(("merge", st, dc, n))
    if not c:
        return None
    how, st, dc, n = rng.choice(c)
    ind = B.indent(st.a)
    if how == "split":
        a, b = dc.init
        eq_start = B.S[a - 1].start
        init_txt = B.span(a, b)
        s_end = B.S[st.b - 1].end
        head = B.text[B.S[st.a].start:eq_start].rstrip()
        return B.edit([(B.S[st.a].start, s_end, f"{head};\n{ind}{dc.name} = {init_txt};")])
    rhs = B.span(n.a + 2, n.b - 1)
    head = B.span(st.a, st.b - 1)
    return B.edit([(B.S[st.a].start, B.S[n.b - 1].end, f"{head} = {rhs};")])


def m_register(B: Body, rng):
    """Add or remove `register` on a local declaration."""
    c = []
    for st in B.root.walk():
        d = B.decl_of.get(id(st))
        if not d:
            continue
        if B.S[st.a].text == "register":
            c.append(("drop", st))
        elif B.S[st.a].text not in ("static", "const", "struct", "class", "union", "enum") and \
                not any(dc.name in B.escaped or dc.array for dc in d[1]):
            c.append(("add", st))
    if not c:
        return None
    how, st = rng.choice(c)
    if how == "drop":
        return B.edit([(B.S[st.a].start, B.S[st.a + 1].start, "")])
    return B.edit([(B.S[st.a].start, B.S[st.a].start, "register ")])


def m_change_type(B: Body, rng):
    """UNSAFE: change the integer type of a local (width or signedness)."""
    c = []
    for st in B.root.walk():
        d = B.decl_of.get(id(st))
        if not d:
            continue
        base = " ".join(t.text for t in B.S[d[0][0]:d[0][1]])
        prefix = ""
        if base.startswith("register "):
            prefix, base = "register ", base[len("register "):]
        if base in INT_TYPES and all(not dc.pointer and not dc.array and dc.name not in B.escaped
                                     for dc in d[1]):
            c.append((st, d, base, prefix))
    if not c:
        return None
    st, d, base, prefix = rng.choice(c)
    new = rng.choice([t for t in INT_TYPES if t != base and t not in ("BYTE", "WORD", "DWORD")])
    s, e = B.cpos(*d[0])
    return B.edit([(s, e, prefix + new)])


def _lvalue_ok(B, a, b):
    if b <= a:
        return False
    e = B.eff(a, b)
    return not e.side and not e.call and operand_fwd(B.S, B.M, a, b) == b


def m_incdec(B: Body, rng):
    """`i++;` <-> `++i;` <-> `i += 1;` <-> `i = i + 1;` (statements and for steps)."""
    S = B.S
    spans = []
    for st in B.root.walk():
        if st.kind == "expr":
            spans.append((st.a, st.b - 1))
        if st.kind == "for" and st.step[1] > st.step[0]:
            spans.append(st.step)
    c = []
    for a, b in spans:
        form = None
        if b - a >= 2 and S[b - 1].text in ("++", "--") and _lvalue_ok(B, a, b - 1):
            form, lv, sign = "post", (a, b - 1), S[b - 1].text[0]
        elif b - a >= 2 and S[a].text in ("++", "--") and _lvalue_ok(B, a + 1, b):
            form, lv, sign = "pre", (a + 1, b), S[a].text[0]
        else:
            ops = _top_ops(S, B.M, a, b)
            if len(ops) == 1 and S[ops[0]].text in ("+=", "-=") and ops[0] == b - 2 and S[b - 1].text == "1" \
                    and _lvalue_ok(B, a, ops[0]):
                form, lv, sign = "comp", (a, ops[0]), S[ops[0]].text[0]
            elif len(ops) == 2 and S[ops[0]].text == "=" and S[ops[1]].text in ("+", "-") and ops[1] == b - 2 \
                    and S[b - 1].text == "1" and _lvalue_ok(B, a, ops[0]) \
                    and norm(B.span(a, ops[0])) == norm(B.span(ops[0] + 1, ops[1])):
                form, lv, sign = "full", (a, ops[0]), S[ops[1]].text
        if form:
            c.append((a, b, form, lv, sign))
    if not c:
        return None
    a, b, form, lv, sign = rng.choice(c)
    x = B.span(*lv)
    forms = {"post": f"{x}{sign}{sign}", "pre": f"{sign}{sign}{x}", "comp": f"{x} {sign}= 1",
             "full": f"{x} = {x} {sign} 1"}
    new = rng.choice([v for k, v in forms.items() if k != form])
    return B.edit([(S[a].start, S[b - 1].end, new)])


# --- loops -----------------------------------------------------------------------------------------------------

def _has_continue(B, st: Stmt) -> bool:
    return any(s.kind == "continue" for s in st.walk())


def m_for_to_while(B: Body, rng):
    """for (i; c; s) B  ->  i; while (c) { B s; }   (no `continue` in B)."""
    c = [s for s in B.root.walk() if s.kind == "for" and not _has_continue(B, s.body)]
    if not c:
        return None
    st = rng.choice(c)
    ind = B.indent(st.a)
    init, cond, step = B.span(*st.init), B.span(*st.cond), B.span(*st.step)
    cond = cond or "1"
    body = st.body
    if body.kind == "block":
        inner = B.text[B.S[body.a].end:B.S[body.b - 1].start].rstrip()
        if step:
            inner += f"\n{ind}    {step};"
        loop = f"while ({cond})\n{ind}{{{inner}\n{ind}}}"
    else:
        bt = B.stext(body)
        loop = f"while ({cond})\n{ind}{{\n{ind}    {bt}" + (f"\n{ind}    {step};" if step else "") + f"\n{ind}}}"
    parent_block = st.parent is not None and st.parent.kind == "block"
    decl_init = st.init[1] > st.init[0] and B.S[st.init[0]].kind == "id" and \
        __import__("ctoken").is_decl(B.S, st.init[0], st.init[1])
    if not init:
        new = loop
    elif parent_block and not decl_init:
        new = f"{init};\n{ind}{loop}"
    else:
        loop = loop.replace("\n", "\n    ")
        new = f"{{\n{ind}    {init};\n{ind}    {loop}\n{ind}}}"
    return B.edit([(B.S[st.a].start, B.S[st.b - 1].end, new)])


def m_while_to_for(B: Body, rng):
    """while (c) B -> for (; c; ) B,  for (; c; ) B -> while (c) B,  i; while (c) { B s; } -> for (i; c; s) B."""
    c = []
    for st in B.root.walk():
        if st.kind == "while":
            c.append(("simple", st, None))
            p = st.parent
            if p is not None and p.kind == "block":
                i = next(k for k, x in enumerate(p.kids) if x is st)
                if i > 0 and p.kids[i - 1].kind == "expr" and st.body.kind == "block" and st.body.kids \
                        and st.body.kids[-1].kind == "expr" and not _has_continue(B, st.body):
                    c.append(("full", st, p.kids[i - 1]))
        elif st.kind == "for" and st.init[1] == st.init[0] and st.step[1] == st.step[0]:
            c.append(("back", st, None))
    if not c:
        return None
    how, st, prev = rng.choice(c)
    if how == "simple":
        s, e = B.S[st.a].start, B.S[st.cond[1]].end
        return B.edit([(s, e, f"for (; {B.span(*st.cond)}; )")])
    if how == "back":
        cond = B.span(*st.cond) or "1"
        return B.edit([(B.S[st.a].start, B.S[st.step[1]].end, f"while ({cond})")])
    last = st.body.kids[-1]
    init, step = B.span(prev.a, prev.b - 1), B.span(last.a, last.b - 1)
    ind = B.indent(st.a)
    edits = [(B.S[prev.a].start, B.S[st.a].start, ""),
             (B.S[st.a].start, B.S[st.cond[1]].end, f"for ({init}; {B.span(*st.cond)}; {step})")]
    prev_end = B.S[st.body.kids[-2].b - 1].end if len(st.body.kids) > 1 else B.S[st.body.a].end
    edits.append((prev_end, B.S[last.b - 1].end, ""))
    return B.edit(edits)


def m_loop_rotate(B: Body, rng):
    """while (c) B -> if (c) { do B while (c); }   (c without side effects)."""
    c = [s for s in B.root.walk() if s.kind == "while" and not B.eff(*s.cond).side]
    if not c:
        return None
    st = rng.choice(c)
    ind = B.indent(st.a)
    cond = B.span(*st.cond)
    body = B.stext(st.body).replace("\n", "\n    ")
    if st.body.kind != "block":
        body = "{\n" + ind + "        " + body + "\n" + ind + "    }"
    new = f"if ({cond})\n{ind}{{\n{ind}    do\n{ind}    {body}\n{ind}    while ({cond});\n{ind}}}"
    return B.edit([(B.S[st.a].start, B.S[st.b - 1].end, new)])


# --- ternaries -------------------------------------------------------------------------------------------------

def _ternary(B, a, b):
    """(q, c) indices of the top-level ? and : of S[a:b], or None."""
    ops = _top_ops(B.S, B.M, a, b)
    qs = [k for k in ops if B.S[k].text == "?"]
    if len(qs) != 1:
        return None
    depth, q = 0, qs[0]
    j = q + 1
    while j < b:
        t = B.S[j]
        if t.kind == "op" and t.text in "([{":
            j = B.M[j] + 1
            continue
        if t.text == "?":
            depth += 1
        elif t.text == ":":
            if depth == 0:
                return q, j
            depth -= 1
        j += 1
    return None


def m_ternary_to_if(B: Body, rng):
    """x = c ? a : b;  ->  if (c) x = a; else x = b;   and the same for return."""
    S = B.S
    c = []
    for st in B.root.walk():
        if st.kind == "expr":
            ops = _top_ops(S, B.M, st.a, st.b - 1)
            if ops and S[ops[0]].text == "=" and _lvalue_ok(B, st.a, ops[0]):
                t = _ternary(B, ops[0] + 1, st.b - 1)
                if t:
                    c.append((st, "x", ops[0], t))
        elif st.kind == "return":
            t = _ternary(B, st.a + 1, st.b - 1)
            if t:
                c.append((st, "r", st.a, t))
    if not c:
        return None
    st, how, k, (q, col) = rng.choice(c)
    ind = B.indent(st.a)
    cond, x, y = B.span(k + 1, q), B.span(q + 1, col), B.span(col + 1, st.b - 1)
    if how == "x":
        lv = B.span(st.a, k)
        new = f"if ({cond})\n{ind}    {lv} = {x};\n{ind}else\n{ind}    {lv} = {y};"
    else:
        new = f"if ({cond})\n{ind}    return {x};\n{ind}else\n{ind}    return {y};"
    if st.parent is not None and st.parent.kind != "block":
        new = "{\n" + ind + "    " + new.replace("\n", "\n    ") + "\n" + ind + "}"
    return B.edit([(S[st.a].start, S[st.b - 1].end, new)])


def _single(st):
    if st.kind == "block" and len(st.kids) == 1:
        return st.kids[0]
    return st


def m_if_to_ternary(B: Body, rng):
    """if (c) x = a; else x = b;  ->  x = c ? a : b;   (and returns)."""
    S = B.S
    c = []
    for st in B.root.walk():
        if st.kind != "if" or st.els is None:
            continue
        t, e = _single(st.then), _single(st.els)
        if t.kind == "return" and e.kind == "return" and t.b - t.a > 2 and e.b - e.a > 2:
            c.append((st, "r", t, e))
        elif t.kind == "expr" and e.kind == "expr":
            o1, o2 = _top_ops(S, B.M, t.a, t.b - 1), _top_ops(S, B.M, e.a, e.b - 1)
            if o1 and o2 and S[o1[0]].text == "=" and S[o2[0]].text == "=" and \
                    norm(B.span(t.a, o1[0])) == norm(B.span(e.a, o2[0])) and _lvalue_ok(B, t.a, o1[0]):
                c.append((st, "x", (t, o1[0]), (e, o2[0])))
    if not c:
        return None
    st, how, t, e = rng.choice(c)

    def par(txt):
        sub = significant(tokenize(txt))
        return txt if len(sub) == 1 else f"({txt})"
    cond = B.span(*st.cond)
    if how == "r":
        new = f"return {par(cond)} ? {par(B.span(t.a + 1, t.b - 1))} : {par(B.span(e.a + 1, e.b - 1))};"
    else:
        (ts, k1), (es, k2) = t, e
        new = (f"{B.span(ts.a, k1)} = {par(cond)} ? {par(B.span(k1 + 1, ts.b - 1))} : "
               f"{par(B.span(k2 + 1, es.b - 1))};")
    return B.edit([(S[st.a].start, S[st.b - 1].end, new)])


# --- temporaries -----------------------------------------------------------------------------------------------

def temp_candidates(B: Body, typed=True):
    """Subexpressions that could move into a temporary: (statement, block, index, a, b). With `typed`, only
    those with a known type in B.types."""
    S, M = B.S, B.M
    out = []
    for blk in B.blocks:
        for idx, st in enumerate(blk.kids):
            if st.kind in ("expr", "return"):
                spans = [(st.a + (1 if st.kind == "return" else 0), st.b - 1)]
            elif st.kind == "if":
                spans = [st.cond]
            elif st.kind == "decl" and B.cpp:
                d = B.decl_of.get(id(st))
                spans = [dc.init for dc in (d[1] if d else []) if dc.init]
            else:
                continue
            for lo, hi in spans:
                for j in range(lo, hi):
                    t = S[j]
                    if not (t.kind == "id" and (t.text not in KEYWORDS or t.text == "this") or
                            (t.kind == "op" and t.text == "(" and not (j > lo and ends_operand(S, M, j - 1)))):
                        continue
                    if j > lo and S[j - 1].text in (".", "->", "::"):
                        continue
                    # primary, then each postfix step is a candidate end
                    e = M[j] + 1 if t.text == "(" else j + 1
                    ends = [e]
                    while e < hi:
                        x = S[e]
                        if x.text in ("[", "(") and x.kind == "op":
                            e = M[e] + 1
                        elif x.text in (".", "->") and e + 1 < hi and S[e + 1].kind == "id":
                            e += 2
                        else:
                            break
                        ends.append(e)
                    for e in ends:
                        if e - j == 1 and (S[j].text in B.locals or S[j].kind != "id"):
                            continue                  # a plain local or literal
                        if e < hi and S[e].text in ("(",):
                            continue                  # a function designator
                        if typed and norm(B.span(j, e)) not in B.types:
                            continue
                        out.append((st, blk, idx, j, e, lo, hi))
    return out


def _call_parens(B: Body, lo, hi) -> list:
    """Indices of the `(` of every call in S[lo:hi] (casts excluded)."""
    S, M = B.S, B.M
    out = []
    for p in range(lo + 1, hi):
        if S[p].kind == "op" and S[p].text == "(" and \
                (S[p - 1].kind == "id" and S[p - 1].text not in KEYWORDS or S[p - 1].text in (")", "]")) and \
                not (S[p - 1].text == ")" and is_cast_group(S, M, M[p - 1])):
            out.append(p)
    return out


def _hoist_ok(B: Body, st, j, e, lo, hi) -> bool:
    S = B.S
    nxt = S[e].text if e < hi else None
    prv = S[j - 1].text if j > lo else None
    if nxt in ASSIGN_OPS or nxt in ("++", "--") or prv in ("++", "--"):
        return False
    if prv == "&" and not (j - 2 >= lo and ends_operand(S, B.M, j - 2)):
        return False
    if prv == "sizeof":
        return False
    # nothing conditional before it
    if any(S[k].text in ("&&", "||", "?") for k in range(lo, j)):
        return False
    ex = B.eff(j, e)
    rest = Effects()
    for a, b in ((lo, j), (e, hi)):
        r = B.eff(a, b)
        rest.reads |= r.reads
        rest.writes |= r.writes
        rest.call |= r.call
        rest.side |= r.side
        rest.opaque |= r.opaque
    if ex.opaque or rest.opaque:
        return False
    if ex.side:
        # a call/assignment hoisted out: the rest must not read memory or have effects of its own,
        # other than one top-level assignment
        ops = [S[k].text for k in _top_ops(S, B.M, lo, hi) if S[k].text in ASSIGN_OPS]
        other_calls = [p for p in _call_parens(B, lo, hi) if not j <= p < e and not (p < j and B.M[p] >= e)]
        if other_calls or any(S[k].text in ("++", "--") for k in range(lo, hi) if not j <= k < e) or len(ops) > 1:
            return False
        rest_reads = B.eff(lo, j).reads | B.eff(e, hi).reads
        if any(x[0] in "NA" or x[2:].split(".")[0] in B.escaped for x in rest_reads if x != "CALL"):
            return False
        return True
    # a pure expression hoisted: the rest must not change what it reads before it is evaluated. Calls whose
    # argument list holds it run after it anyway; any other call might run first.
    mem = any(x[0] in "NA" or x == "CALL" or x[2:].split(".")[0] in B.escaped for x in ex.reads)
    if rest.call and mem and any(not (p < j and B.M[p] >= e) for p in _call_parens(B, lo, hi)
                                 if not j <= p < e):
        return False
    ops = [k for k in range(lo, hi) if not j <= k < e and S[k].text in ("++", "--")]
    if ops:
        return False
    assigns = [k for k in _top_ops(S, B.M, lo, hi) if S[k].text in ASSIGN_OPS]
    inner_assigns = [k for k in range(lo, hi) if S[k].text in ASSIGN_OPS and k not in assigns
                     and not j <= k < e]
    if inner_assigns or len(assigns) > 1:
        return False
    return True


def m_add_temp(B: Body, rng):
    """Hoist a subexpression into a new local: `f(p->a->b)` -> `T temp = p->a; f(temp->b)`."""
    c = [x for x in temp_candidates(B) if _hoist_ok(B, x[0], x[3], x[4], x[5], x[6])]
    if not c:
        return None
    st, blk, idx, j, e, lo, hi = rng.choice(c)
    typ = B.types[norm(B.span(j, e))]
    name = B.fresh()
    ind = B.indent(st.a)
    expr = B.span(j, e)
    S = B.S
    edits = [(S[j].start, S[e - 1].end, name)]
    if B.cpp:
        edits.append((S[st.a].start, S[st.a].start, f"{_decl(typ, name)} = {expr};\n{ind}"))
    else:
        first = blk.kids[0] if blk.kids else None
        if any(k.kind == "decl" for k in blk.kids[idx:]):
            return None
        ins = B.S[blk.a].end
        bind = B.indent(first.a) if first else ind + "    "
        edits.append((ins, ins, f"\n{bind}{_decl(typ, name)};"))
        edits.append((S[st.a].start, S[st.a].start, f"{name} = {expr};\n{ind}"))
    return B.edit(edits)


def _decl(typ: str, name: str) -> str:
    typ = typ.strip()
    if typ.endswith("*"):
        return f"{typ}{name}" if typ.endswith(" *") else f"{typ} {name}"
    return f"{typ} {name}"


def m_remove_temp(B: Body, rng):
    """`T t = e; use(t);` -> `use((T)(e));`  when t is used exactly once, in the very next statement."""
    S = B.S
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i in range(len(kids) - 1):
            st, use = kids[i], kids[i + 1]
            if use.kind not in ("expr", "return", "if", "decl"):
                continue
            name = init = decl_st = dc = base = None
            d = B.decl_of.get(id(st))
            if d and len(d[1]) == 1 and d[1][0].init and not d[1][0].array:
                dc, base = d[1][0], d[0]
                if S[dc.init[0]].text == "{":
                    continue
                name, init, decl_st = dc.name, dc.init, st
            elif st.kind == "expr" and S[st.a].kind == "id" and S[st.a + 1].text == "=" and S[st.a].text in B.locals:
                name, init = S[st.a].text, (st.a + 2, st.b - 1)
                loc = B.locals[name]
                dd = B.decl_of.get(id(loc.stmt)) if loc.stmt is not None else None
                if not dd or len(dd[1]) != 1 or loc.decl.init or loc.array:
                    continue
                dc, base, decl_st = loc.decl, dd[0], loc.stmt
            if name is None or name in B.escaped:
                continue
            typ = B.span(*base)
            if re.search(r"b(static|extern)b", typ):
                continue
            typ = re.sub(r"b(register|const|volatile)\s+", "", typ)
            stars = "".join(S[k].text for k in range(dc.a, dc.name_idx) if S[k].text in ("*", "&"))
            typ = typ + (" " + stars if stars else "")
            # exactly one use in the whole body besides the definition (and the bare declaration)
            total = B.uses(name, 0, len(S))
            own = B.uses(name, st.a, st.b) + (B.uses(name, decl_st.a, decl_st.b) if decl_st is not st else 0)
            ue = use.cond if use.kind == "if" else (use.a, use.b - 1)
            if total - own != 1 or B.uses(name, *ue) != 1:
                continue
            k = next(q for q in range(ue[0], ue[1]) if S[q].kind == "id" and S[q].text == name and
                     not (q > 0 and S[q - 1].text in (".", "->", "::")))
            if k + 1 < len(S) and (S[k + 1].text in ASSIGN_OPS or S[k + 1].text in ("++", "--")):
                continue
            if S[k - 1].text in ("&", "++", "--") and not ends_operand(S, B.M, k - 2):
                continue
            ie, rest = B.eff(*init), B.eff(*ue)
            if ie.opaque or rest.opaque:
                continue
            if any(S[q].text in ("++", "--") for q in range(ue[0], ue[1])):
                continue
            if len([q for q in range(ue[0], ue[1]) if S[q].text in ASSIGN_OPS]) > 1:
                continue
            mem = any(r[0] in "NA" or r == "CALL" or r[2:].split(".")[0] in B.escaped for r in ie.reads)
            # calls whose argument list holds the use run after it anyway; any other call might run first
            other_calls = [p for p in _call_parens(B, ue[0], ue[1]) if not (p < k and B.M[p] > k)]
            if other_calls and (mem or ie.side):
                continue
            if ie.side:
                if any(S[q].text in ("&&", "||", "?") for q in range(ue[0], k)):
                    continue
                others = B.eff(ue[0], k).reads | B.eff(k + 1, ue[1]).reads
                if any(r[0] in "NA" or r[2:].split(".")[0] in B.escaped for r in others if r != "CALL"):
                    continue
            c.append((st, k, init, typ, decl_st))
    if not c:
        return None
    st, k, init, typ, decl_st = rng.choice(c)
    expr = B.span(*init)
    edits = [(S[k].start, S[k].end, f"(({typ})({expr}))")]
    edits.append((B.S[st.a - 1].end, S[st.b - 1].end, ""))
    if decl_st is not st:
        edits.append((B.S[decl_st.a - 1].end, S[decl_st.b - 1].end, ""))
    return B.edit(edits)


def _decl_count(B: Body, name: str) -> int:
    n = sum(1 for d in B.decl_of.values() for dc in d[1] if dc.name == name)
    return n + (1 if name in B.locals and B.locals[name].stmt is None else 0)


def _use_positions(B: Body, name: str, a: int, b: int) -> list:
    S = B.S
    return [k for k in range(a, b) if S[k].kind == "id" and S[k].text == name
            and not (k > 0 and S[k - 1].text in (".", "->", "::"))]


def _is_written(B: Body, k: int) -> bool:
    S = B.S
    nxt = S[k + 1].text if k + 1 < len(S) else None
    if nxt in ASSIGN_OPS or nxt in ("++", "--"):
        return True
    return k > 0 and S[k - 1].text in ("&", "++", "--") and not ends_operand(S, B.M, k - 2)


def _stmt_at(B: Body, blk: Stmt, k: int) -> Stmt | None:
    for st in blk.kids:
        if st.a <= k < st.b:
            return st
    return None


def _loop_extent(B: Body, blk: Stmt, k: int, hi: int) -> int:
    """Extend `hi` to the end of every loop between the token k and the block blk."""
    st = None
    for s in B.root.walk():
        if s.a <= k < s.b and s.kind != "block" and (st is None or s.b - s.a < st.b - st.a):
            st = s
    while st is not None and st is not blk:
        if st.kind in ("for", "while", "do"):
            hi = max(hi, st.b)
        st = st.parent
    return hi


def _loop_between(outer: Stmt, inner: Stmt) -> bool:
    s = inner.parent
    while s is not None and s is not outer:
        if s.kind in ("for", "while", "do"):
            return True
        s = s.parent
    return False


def _drop_stmt(B: Body, st: Stmt):
    return (B.S[st.a - 1].end, B.S[st.b - 1].end, "")


def _strip_tag(t: str) -> str:
    return re.sub(r"\b(class|struct|union)\s+", "", t).replace(" ", "")


def _ctype(B: Body, d, dc) -> str:
    typ = re.sub(r"\b(register|const|volatile|static)\s+", "", B.span(*d[0]))
    stars = "".join(B.S[k].text for k in range(dc.a, dc.name_idx) if B.S[k].text in ("*", "&"))
    return typ + (" " + stars if stars else "")


def m_scope_decl(B: Body, rng):
    """Move a declaration into the innermost block holding all its uses, or out to an enclosing block
    (register allocation and push/pop placement follow scopes)."""
    S = B.S
    c = []
    for blk in B.blocks:
        for idx, st in enumerate(blk.kids):
            d = B.decl_of.get(id(st))
            if not d or S[st.a].text == "static":
                continue
            names = [dc.name for dc in d[1]]
            if any(_decl_count(B, n) != 1 for n in names):
                continue
            uses = [k for n in names for k in _use_positions(B, n, 0, len(S)) if not st.a <= k < st.b]
            if not uses or any(not (st.b <= k < blk.b) for k in uses):
                continue
            inits = [dc.init for dc in d[1] if dc.init]
            ie = Effects()
            for i in inits:
                e = B.eff(*i)
                ie.reads |= e.reads
                ie.side |= e.side or e.call
                ie.opaque |= e.opaque
            if ie.side or ie.opaque:
                continue
            lo, hi = min(uses), max(uses)
            for b2 in B.blocks:
                if b2 is blk or not (blk.a < b2.a and b2.b <= blk.b and b2.a < lo and hi < b2.b - 1):
                    continue
                if _loop_between(blk, b2):
                    continue
                if inits and conflicts(B.eff(st.b, b2.a), Effects(reads=ie.reads), B.escaped):
                    continue
                c.append(("in", st, b2))
            # outward: no initializer, or a literal one, to the top of an enclosing block without a loop between
            if blk is not B.root and all(dc.init is None or (dc.init[1] - dc.init[0] == 1 and
                                                              S[dc.init[0]].kind in ("num", "chr")) for dc in d[1]):
                outer = blk.parent
                while outer is not None:
                    if outer.kind == "block":
                        if inits and _loop_between(outer, blk):
                            break
                        c.append(("out", st, outer))
                    outer = outer.parent
    if not c:
        return None
    how, st, b2 = rng.choice(c)
    text = B.stext(st)
    ind = B.indent(b2.kids[0].a) if b2.kids else B.indent(b2.a) + "    "
    ins = S[b2.a].end
    return B.edit([_drop_stmt(B, st), (ins, ins, "\n" + ind + text)])


def m_inline_var(B: Body, rng):
    """`T x = e; ... x ... x` -> uses of `e` (pure, nothing in between changes what it reads)."""
    S = B.S
    c = []
    for blk in B.blocks:
        for idx, st in enumerate(blk.kids):
            d = B.decl_of.get(id(st))
            if not d or len(d[1]) != 1 or S[st.a].text == "static":
                continue
            dc = d[1][0]
            if not dc.init or dc.array or dc.name in B.escaped or S[dc.init[0]].text == "{" \
                    or _decl_count(B, dc.name) != 1:
                continue
            ie = B.eff(*dc.init)
            if ie.side or ie.opaque or ie.call:
                continue
            uses = [k for k in _use_positions(B, dc.name, 0, len(S)) if not st.a <= k < st.b]
            if not uses or any(not (st.b <= k < blk.b) or _is_written(B, k) for k in uses):
                continue
            hi = max(uses) + 1
            for k in uses:
                hi = _loop_extent(B, blk, k, hi)
            if conflicts(B.eff(st.b, hi), Effects(reads=ie.reads), B.escaped):
                continue
            c.append((st, d, dc, uses))
    if not c:
        return None
    st, d, dc, uses = rng.choice(c)
    expr = B.span(*dc.init)
    typ = _ctype(B, d, dc)
    et = B.types.get(norm(expr))
    single = operand_fwd(S, B.M, dc.init[0], dc.init[1]) == dc.init[1]
    if et is not None and _strip_tag(et) == _strip_tag(typ):
        rep = expr if single else f"({expr})"
    else:
        rep = f"(({typ})({expr}))"
    edits = [(S[k].start, S[k].end, rep) for k in uses] + [_drop_stmt(B, st)]
    return B.edit(edits)


def m_cse(B: Body, rng):
    """A subexpression used more than once -> one temporary: `T temp = p->a; ... temp ... temp`."""
    S = B.S
    groups: dict = {}
    for x in temp_candidates(B):
        st, blk, idx, j, e, lo, hi = x
        groups.setdefault((id(blk), norm(B.span(j, e))), []).append(x)
    c = []
    for (bid, key), xs in groups.items():
        xs = sorted(xs, key=lambda x: x[3])
        # non-overlapping occurrences
        occ, last_end = [], -1
        for x in xs:
            if x[3] >= last_end:
                occ.append(x)
                last_end = x[4]
        if len(occ) < 2:
            continue
        st0, blk, idx = occ[0][0], occ[0][1], occ[0][2]
        if not _hoist_ok(B, st0, occ[0][3], occ[0][4], occ[0][5], occ[0][6]):
            continue
        ex = B.eff(occ[0][3], occ[0][4])
        if ex.side or ex.opaque:
            continue
        if any(_is_written(B, x[4] - 1) or (x[3] > 0 and S[x[3] - 1].text in ("&", "++", "--")
                                            and not ends_operand(S, B.M, x[3] - 2)) for x in occ):
            continue
        hi = occ[-1][4]
        for x in occ:
            hi = _loop_extent(B, blk, x[3], hi)
        # nothing between the first and the last use may change what it reads (the uses' own reads are fine)
        if conflicts(B.eff(occ[0][4], hi), Effects(reads=ex.reads), B.escaped):
            continue
        if not B.cpp and any(k.kind == "decl" for k in blk.kids[idx:]):
            continue
        c.append((key, occ))
    if not c:
        return None
    key, occ = rng.choice(c)
    st0, blk, idx = occ[0][0], occ[0][1], occ[0][2]
    typ = B.types[key]
    name = B.fresh()
    ind = B.indent(st0.a)
    expr = B.span(occ[0][3], occ[0][4])
    edits = [(S[x[3]].start, S[x[4] - 1].end, name) for x in occ]
    if B.cpp:
        edits.append((S[st0.a].start, S[st0.a].start, f"{_decl(typ, name)} = {expr};\n{ind}"))
    else:
        first = blk.kids[0]
        ins = S[blk.a].end
        edits.append((ins, ins, f"\n{B.indent(first.a)}{_decl(typ, name)};"))
        edits.append((S[st0.a].start, S[st0.a].start, f"{name} = {expr};\n{ind}"))
    return B.edit(edits)


def m_tail_return(B: Body, rng):
    """`if (c) S; return v;` -> `if (c) { S; return v; } return v;` (or `return K;` when v still holds the
    literal K on the fall-through path)."""
    S = B.S
    c = []
    for blk in B.blocks:
        kids = blk.kids
        if len(kids) < 2 or kids[-1].kind != "return" or kids[-2].kind != "if" or kids[-2].els is not None:
            continue
        ret, iff = kids[-1], kids[-2]
        if _ends_in_jump(iff.then):
            continue
        const = None
        if ret.b - ret.a == 3 and S[ret.a + 1].kind == "id" and S[ret.a + 1].text in B.locals \
                and S[ret.a + 1].text not in B.escaped:
            v = S[ret.a + 1].text
            for i in range(len(kids) - 3, -1, -1):
                k = kids[i]
                lit = None
                d = B.decl_of.get(id(k))
                if d and len(d[1]) == 1 and d[1][0].name == v and d[1][0].init:
                    lit = d[1][0].init
                elif k.kind == "expr" and S[k.a].text == v and S[k.a + 1].text == "=":
                    lit = (k.a + 2, k.b - 1)
                if lit:
                    a, b = lit
                    if (b - a == 1 and S[a].kind in ("num", "chr")) or \
                            (b - a == 2 and S[a].text == "-" and S[a + 1].kind == "num"):
                        if not any(B.uses(v, o.a, o.b) for o in kids[i + 1:len(kids) - 2]) and \
                                not B.uses(v, *iff.cond):
                            const = B.span(a, b)
                    break
                if B.uses(v, k.a, k.b):
                    break
        c.append((iff, ret, const))
    if not c:
        return None
    iff, ret, const = rng.choice(c)
    rtext = B.stext(ret)
    th = iff.then
    edits = []
    if th.kind == "block":
        ind = B.indent(th.kids[-1].a) if th.kids else B.indent(th.a) + "    "
        pos = B.S[th.kids[-1].b - 1].end if th.kids else B.S[th.a].end
        edits.append((pos, pos, "\n" + ind + rtext))
    else:
        ind = B.indent(iff.a)
        edits.append((*B.cpos(th.a, th.b), "{\n" + ind + "    " + B.stext(th) + "\n" + ind + "    " + rtext
                      + "\n" + ind + "}"))
    if const is not None and rng.random() < 0.7:
        edits.append((*B.cpos(ret.a, ret.b), f"return {const};"))
    return B.edit(edits)


def m_hoist_incdec(B: Body, rng):
    """`if (--x != 0)` <-> `--x; if (x != 0)` (the first thing the condition evaluates)."""
    S = B.S
    c = []
    for blk in B.blocks:
        for idx, st in enumerate(blk.kids):
            if st.kind == "if":
                lo, hi = st.cond
            elif st.kind in ("expr", "return"):
                lo, hi = st.a + (st.kind == "return"), st.b - 1
            else:
                continue
            for k in range(lo, hi):
                if S[k].text in ("&&", "||", "?"):
                    break
                if S[k].text in ("++", "--") and not (k - 1 >= lo and ends_operand(S, B.M, k - 1)):
                    e = operand_fwd(S, B.M, k + 1, hi)
                    if e and _lvalue_ok(B, k + 1, e) and not (st.kind == "expr" and k == lo and e == hi):
                        c.append(("out", st, k, e))
                    break
            # reverse: `--x;` followed by this statement starting with x
            if idx > 0 and st.kind in ("if", "expr", "return"):
                p = blk.kids[idx - 1]
                if p.kind == "expr" and S[p.a].text in ("++", "--") and _lvalue_ok(B, p.a + 1, p.b - 1):
                    lv = norm(B.span(p.a + 1, p.b - 1))
                    n = p.b - 1 - (p.a + 1)
                    if lo + n <= hi and norm(B.span(lo, lo + n)) == lv and \
                            (lo + n == hi or S[lo + n].text not in (".", "->", "[", "(", "++", "--")) and \
                            (lo + n >= hi or S[lo + n].text not in ASSIGN_OPS):
                        c.append(("in", st, p, lo + n))
    if not c:
        return None
    how, st, x, y = rng.choice(c)
    if how == "out":
        k, e = x, y
        ind = B.indent(st.a)
        return B.edit([(S[st.a].start, S[st.a].start, f"{S[k].text}{B.span(k + 1, e)};\n{ind}"),
                       (S[k].start, S[k + 1].start, "")])
    p, end = x, y
    lo = st.cond[0] if st.kind == "if" else st.a + (st.kind == "return")
    return B.edit([_drop_stmt(B, p), (S[lo].start, S[lo].start, S[p.a].text)])


# --- casts -----------------------------------------------------------------------------------------------------

def m_cast(B: Body, rng):
    """UNSAFE: drop an integer cast `(T)x`, or add `(int)` / `(unsigned int)` to an operand of an arithmetic,
    bitwise or comparison operator."""
    S, M = B.S, B.M
    c = []
    for j in range(len(S) - 1):
        if S[j].text == "(" and S[j].kind == "op" and not (j > 0 and ends_operand(S, M, j - 1)):
            k = M[j]
            inner = " ".join(t.text for t in S[j + 1:k])
            if inner in INT_TYPES and k + 1 < len(S) and operand_fwd(S, M, k + 1, len(S)):
                c.append(("drop", j, k))
    ops = _ops_in(B, ("+", "-", "*", "&", "|", "^", "<", ">", "<=", ">=", "==", "!=", ">>", "<<", "/", "%"))
    for (la, k, ra, rb), _ in ops:
        c.append(("add", la, k))
        c.append(("add", ra, rb))
    if not c:
        return None
    how, a, b = rng.choice(c)
    if how == "drop":
        return B.edit([(S[a].start, S[b].end + (1 if B.text[S[b].end:S[b].end + 1] == " " else 0), "")])
    typ = rng.choice(["int", "unsigned int", "short", "unsigned short", "char", "unsigned char"])
    txt = B.span(a, b)
    if operand_fwd(S, M, a, b) != b:
        txt = f"({txt})"
    return B.edit([(S[a].start, S[b - 1].end, f"({typ}){txt}")])


# --- passes from the bake-off with cpp_permuter (docs/PLAN.md 9.1) ------------------------------
# Same ideas as cpp_permuter's chain_assign / compound_assign / negate_const / bool_return / reassociate /
# scope_block passes and gta2_re's hand strategies (cond_temp, pow2_shift, ref_local); our own code.

def _assign_parts(B: Body, st: Stmt):
    """An expression statement `L = R;` -> (L span, R span) when L is a plain lvalue, else None."""
    if st.kind != "expr":
        return None
    a, b = st.a, st.b - 1
    ops = _top_ops(B.S, B.M, a, b)
    if not ops or B.S[ops[0]].text != "=" or not _lvalue_ok(B, a, ops[0]):
        return None
    return (a, ops[0]), (ops[0] + 1, b)


def _pure(B: Body, a, b) -> bool:
    e = B.eff(a, b)
    return b > a and not e.call and not e.side and not e.opaque


def _writes_conflict(B: Body, lv, *spans) -> bool:
    """Does the assignment whose target is the lvalue span `lv` (S[lv[1]] is its `=`) conflict with
    reading the spans?"""
    w = B.eff(lv[0], lv[1] + 1)
    return any(conflicts(w, B.eff(*s), B.escaped) for s in spans)


def m_chain_assign(B: Body, rng):
    """`a = v; b = v;` / `a = v; b = a;` <-> `b = a = v;` (or `a = b = v;`): which register holds v, store
    order."""
    S = B.S
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i, st in enumerate(kids):
            p = _assign_parts(B, st)
            if not p:
                continue
            (la, lb), (ra, rb) = p
            inner = _top_ops(S, B.M, ra, rb)
            if inner and S[inner[0]].text == "=" and len([k for k in inner if S[k].text in ASSIGN_OPS]) == 1 \
                    and _lvalue_ok(B, ra, inner[0]) and _pure(B, inner[0] + 1, rb):
                c.append(("split", st, (la, lb), (ra, inner[0]), (inner[0] + 1, rb)))
            if i + 1 >= len(kids):
                continue
            q = _assign_parts(B, kids[i + 1])
            if not q or not _pure(B, ra, rb):
                continue
            (la2, lb2), (ra2, rb2) = q
            same_v = norm(B.span(ra, rb)) == norm(B.span(ra2, rb2))
            reads_a = norm(B.span(ra2, rb2)) == norm(B.span(la, lb))
            if (same_v or reads_a) and norm(B.span(la, lb)) != norm(B.span(la2, lb2)) \
                    and not _writes_conflict(B, (la, lb), (ra, rb), (la2, lb2)):
                c.append(("merge", st, kids[i + 1], (la, lb), (la2, lb2), (ra, rb)))
    if not c:
        return None
    x = rng.choice(c)
    if x[0] == "split":
        _, st, a_, b_, v = x
        A, Bv, V = B.span(*a_), B.span(*b_), B.span(*v)
        ind = B.indent(st.a)
        new = f"{Bv} = {V};\n{ind}{A} = {Bv};" if rng.random() < 0.5 else f"{Bv} = {V};\n{ind}{A} = {V};"
        if _writes_conflict(B, b_, v, a_):
            new = f"{Bv} = {V};\n{ind}{A} = {Bv};"
        return B.edit([(*B.cpos(st.a, st.b), new)])
    _, s1, s2, a_, b_, v = x
    A, Bv, V = B.span(*a_), B.span(*b_), B.span(*v)
    new = f"{Bv} = {A} = {V};" if rng.random() < 0.6 else f"{A} = {Bv} = {V};"
    return B.edit([(B.S[s1.a].start, B.S[s2.b - 1].end, new)])


_COMPOUND = ("+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>")


def m_compound_assign(B: Body, rng):
    """`x = x op y` <-> `x op= y` (also `x = y op x` -> `x op= y` for commutative op)."""
    S, M = B.S, B.M
    c = []
    for st in B.root.walk():
        if st.kind != "expr":
            continue
        a, b = st.a, st.b - 1
        ops = _top_ops(S, M, a, b)
        if not ops or not _lvalue_ok(B, a, ops[0]):
            continue
        k = ops[0]
        x = norm(B.span(a, k))
        t = S[k].text
        if t == "=":
            rest = ops[1:]
            if rest and S[rest[0]].text in _COMPOUND and S[rest[0]].text in PREC \
                    and all(PREC.get(S[r].text, 99) > PREC[S[rest[0]].text] for r in rest[1:]):
                j = rest[0]
                if norm(B.span(k + 1, j)) == x:
                    c.append((a, b, k, S[j].text, (j + 1, b)))
                elif S[j].text in COMMUTATIVE and len(rest) == 1 and norm(B.span(j + 1, b)) == x:
                    c.append((a, b, k, S[j].text, (k + 1, j)))
        elif t.endswith("=") and t[:-1] in _COMPOUND and len(ops) >= 1:
            c.append((a, b, k, None, (k + 1, b)))
    if not c:
        return None
    a, b, k, op, y = rng.choice(c)
    X, Y = B.span(a, k), B.span(*y)
    if op is not None:
        return B.edit([(S[a].start, S[b - 1].end, f"{X} {op}= {Y}")])
    op = S[k].text[:-1]
    if _top_ops(S, M, *y):
        Y = f"({Y})"
    return B.edit([(S[a].start, S[b - 1].end, f"{X} = {X} {op} {Y}")])


def m_negate_const(B: Body, rng):
    """`x - 4` <-> `x + -4`, `x -= 4` <-> `x += -4` (integer literals): sub vs add of a negative."""
    S = B.S
    c = []
    for lo, hi, st in expr_spans(B):
        for k in range(lo, hi - 1):
            t = S[k]
            if t.kind != "op" or t.text not in ("-", "+", "-=", "+="):
                continue
            if t.text in ("-", "+") and not _is_binary(S, k, lo, B.M):
                continue
            if t.text in ("-", "-=") and _int_lit(S[k + 1]) and (k + 2 >= hi or S[k + 2].text not in ("(", "[", ".")):
                if k + 2 < hi and S[k + 2].text in PREC and PREC[S[k + 2].text] > PREC["+"]:
                    continue
                c.append((k, k + 1, "+" if t.text == "-" else "+=", "-" + S[k + 1].text))
            elif t.text in ("+", "+=") and S[k + 1].text == "-" and k + 2 < hi and _int_lit(S[k + 2]):
                if k + 3 < hi and S[k + 3].text in PREC and PREC[S[k + 3].text] > PREC["+"]:
                    continue
                c.append((k, k + 2, "-" if t.text == "+" else "-=", S[k + 2].text))
    if not c:
        return None
    k, last, op, lit = rng.choice(c)
    return B.edit([(S[k].start, S[last].end, f"{op} {lit}")])


def m_pow2_shift(B: Body, rng):
    """UNSAFE (signed / float operands): `x * 2^n` <-> `x << n`, `x / 2^n` <-> `x >> n`, `x % 2^n` -> `x & (2^n-1)`."""
    S = B.S
    c = []
    for (la, k, ra, rb), _ in _ops_in(B, ("*", "/", "%", "<<", ">>")):
        if rb - ra != 1:
            continue
        lit = _int_lit(S[ra])
        if not lit:
            continue
        v = lit[0]
        op = S[k].text
        if op in ("*", "/", "%") and v >= 2 and v & (v - 1) == 0:
            n = v.bit_length() - 1
            new = {"*": f"<< {n}", "/": f">> {n}", "%": f"& {hex(v - 1)}"}[op]
            c.append((la, k, ra, new))
        elif op in ("<<", ">>") and 1 <= v <= 30:
            c.append((la, k, ra, f"{'*' if op == '<<' else '/'} {1 << v}"))
    if not c:
        return None
    la, k, ra, new = rng.choice(c)
    return B.edit([(S[la].start, S[ra].end, f"({B.span(la, k)} {new})")])


def m_bool_return(B: Body, rng):
    """`return c;` (c a comparison or logical expression) <-> `if (c) return 1; return 0;` (with or without
    else): setcc vs branches."""
    S, M = B.S, B.M
    rel = ("==", "!=", "<", ">", "<=", ">=", "&&", "||")
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i, st in enumerate(kids):
            if st.kind == "return" and st.b - st.a > 2:
                ops = _top_ops(S, M, st.a + 1, st.b - 1)
                if (ops and all(S[j].text in rel or PREC.get(S[j].text, 0) > PREC["<"] for j in ops)
                        and any(S[j].text in rel for j in ops)) or S[st.a + 1].text == "!":
                    c.append(("split", st, None))
            if st.kind == "if" and st.then.kind in ("return", "block"):
                th = st.then.kids[0] if st.then.kind == "block" and len(st.then.kids) == 1 else st.then
                if th.kind != "return" or th.b - th.a != 3:
                    continue
                tv = S[th.a + 1].text
                if tv not in ("1", "0", "true", "false", "TRUE", "FALSE"):
                    continue
                if st.els is not None:
                    el = st.els.kids[0] if st.els.kind == "block" and len(st.els.kids) == 1 else st.els
                    end = st
                elif i + 1 < len(kids):
                    el = kids[i + 1]
                    end = el
                else:
                    continue
                if el.kind == "return" and el.b - el.a == 3 and S[el.a + 1].text in ("1", "0", "true", "false",
                                                                                      "TRUE", "FALSE") \
                        and (tv in ("1", "true", "TRUE")) != (S[el.a + 1].text in ("1", "true", "TRUE")):
                    c.append(("merge", st, (end, tv in ("1", "true", "TRUE"))))
    if not c:
        return None
    how, st, x = rng.choice(c)
    ind = B.indent(st.a)
    if how == "split":
        cond = B.span(st.a + 1, st.b - 1)
        if rng.random() < 0.5:
            new = f"if ({cond})\n{ind}    return 1;\n{ind}return 0;"
        else:
            new = f"if ({cond})\n{ind}    return 1;\n{ind}else\n{ind}    return 0;"
        return B.edit([(*B.cpos(st.a, st.b), new)])
    end, pos = x
    cond = B.span(*st.cond)
    if not pos:
        cond = negate(B, *st.cond)
    return B.edit([(B.S[st.a].start, B.S[end.b - 1].end, f"return {cond};")])


def m_reassociate(B: Body, rng):
    """`a + b + c` <-> `a + (b + c)` for + * & | ^ (operands without side effects)."""
    S, M = B.S, B.M
    c = []
    for (la, k, ra, rb), _ in _ops_in(B, ("+", "*", "&", "|", "^")):
        op = S[k].text
        if not _pure(B, la, rb):
            continue
        left = [j for j in _top_ops(S, M, la, k) if S[j].text == op]
        if left and len(_top_ops(S, M, la, k)) == len(left):
            j = left[-1]
            c.append(("right", la, j, k, rb))
        if S[ra].text == "(" and M[ra] == rb - 1:
            inner = _top_ops(S, M, ra + 1, rb - 1)
            if inner and all(S[j].text == op for j in inner):
                c.append(("flat", ra, rb, None, None))
    if not c:
        return None
    how, a, j, k, rb = rng.choice(c)
    if how == "right":
        return B.edit([(S[j + 1].start, S[j + 1].start, "("), (S[rb - 1].end, S[rb - 1].end, ")")])
    return B.edit([(S[a].start, S[a].end, ""), (S[j - 1].start, S[j - 1].end, "")])


def m_cond_temp(B: Body, rng):
    """`if (c) S` -> `int t = c; if (t) S` (C++, or C where only declarations come before the if): the
    condition computed into a register first."""
    c = []
    for blk in B.blocks:
        for i, st in enumerate(blk.kids):
            if st.kind == "if" and st.cond[1] - st.cond[0] >= 3:
                if B.cpp or all(k.kind == "decl" for k in blk.kids[:i]):
                    c.append(st)
    if not c:
        return None
    st = rng.choice(c)
    name = B.fresh("cond")
    ind = B.indent(st.a)
    cs, ce = B.cpos(*st.cond)
    return B.edit([(B.S[st.a].start, B.S[st.a].start, f"int {name} = {B.span(*st.cond)};\n{ind}"),
                   (cs, ce, name)])


def m_ref_local(B: Body, rng):
    """UNSAFE (C++): `T x = lvalue;` <-> `const T& x = lvalue;` (a reference rereads its object)."""
    S = B.S
    if not B.cpp:
        return None
    c = []
    for st in B.root.walk():
        d = B.decl_of.get(id(st))
        if not d or len(d[1]) != 1:
            continue
        dc = d[1][0]
        if dc.array or not dc.init:
            continue
        if S[dc.name_idx - 1].text == "&":
            c.append(("drop", st, dc))
        elif not dc.pointer and _lvalue_ok(B, *dc.init) and dc.name not in B.escaped \
                and any(S[j].kind == "id" for j in range(*dc.init)):
            c.append(("add", st, dc))
    if not c:
        return None
    how, st, dc = rng.choice(c)
    if how == "drop":
        j = dc.name_idx - 1
        s = S[j].start
        edits = [(s, S[j].end, "")]
        if S[st.a].text == "const" and rng.random() < 0.8:
            edits.append((S[st.a].start, S[st.a + 1].start, ""))
        return B.edit(edits)
    s = S[dc.name_idx].start
    edits = [(s, s, "& ")]
    if S[st.a].text != "const" and rng.random() < 0.7:
        edits.append((S[st.a].start, S[st.a].start, "const "))
    return B.edit(edits)


def m_scope_block(B: Body, rng):
    """Wrap a declaration and the statements up to its last use in their own `{ }` (VC6 reuses a block's
    stack slots), or take apart a nested plain block (C++, or when it holds no declarations)."""
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i, st in enumerate(kids):
            d = B.decl_of.get(id(st))
            if d and blk is not B.root or (d and i + 1 < len(kids)):
                names = [dc.name for dc in d[1]]
                last = i
                for j in range(i + 1, len(kids)):
                    if any(B.uses(n, kids[j].a, kids[j].b) for n in names):
                        last = j
                if last == len(kids) - 1 and i == 0:
                    continue
                if any(k.kind in ("label", "pp") for k in kids[i:last + 1]):
                    continue
                if not B.cpp and any(k.kind == "decl" for k in kids[last + 1:]):
                    continue
                c.append(("wrap", blk, i, last))
            if st.kind == "block" and st.kids and (B.cpp or not any(k.kind == "decl" for k in st.kids)):
                if not any(k.kind == "label" for k in st.kids):
                    c.append(("unwrap", blk, i, st))
    if not c:
        return None
    how, blk, i, x = rng.choice(c)
    if how == "wrap":
        kids = blk.kids
        ind = B.indent(kids[i].a)
        s, e = B.S[kids[i].a].start, B.S[kids[x].b - 1].end
        body = B.text[s:e].replace("\n", "\n    ")
        return B.edit([(s, e, "{\n" + ind + "    " + body + "\n" + ind + "}")])
    st = x
    ind = B.indent(st.a)
    inner = B.span(st.kids[0].a, st.kids[-1].b)
    inner = re.sub(r"\n" + re.escape(ind) + "    ", "\n" + ind, inner)
    return B.edit([(*B.cpos(st.a, st.b), inner)])


def _lead_cast_type(B: Body, a, b):
    """`(T)x` / `(T)(e)` spanning all of S[a:b] -> "T" for an integer or pointer cast, else None."""
    S, M = B.S, B.M
    if S[a].text == "(" and is_cast_group(S, M, a) and operand_fwd(S, M, M[a] + 1, b) == b:
        t = B.span(a + 1, M[a])
        return t if t in INT_TYPES or t.endswith("*") or re.fullmatch(r"[A-Z][A-Z0-9_]*", t) else None
    return None


def m_hoist_cond_operand(B: Body, rng):
    """`if (a && x - y <= 10)` -> `T t = x - y; if (a && t <= 10)`: a pure operand of a comparison on the
    conditional side of && / || computed before the if (cpp_permuter reached this as inline_local +
    temp_for_expr). Not when it dereferences a pointer the condition tests first (`p && p->x`)."""
    S, M = B.S, B.M
    c = []
    for blk in B.blocks:
        for idx, st in enumerate(blk.kids):
            if st.kind != "if":
                continue
            lo, hi = st.cond
            for (la, k, ra, rb), _ in _ops_in(B, ("<", ">", "<=", ">=", "==", "!=")):
                if not lo <= k < hi:
                    continue
                for a, b in ((la, k), (ra, rb)):
                    if b - a < 2 or not any(S[q].text in ("&&", "||") for q in _top_ops(S, M, lo, a)):
                        continue
                    if any(S[q].text == "?" for q in range(lo, a)):
                        continue
                    e = B.eff(a, b)
                    if e.call or e.side or e.opaque or B.eff(lo, a).side:
                        continue
                    typ = _lead_cast_type(B, a, b) or B.types.get(norm(B.span(a, b)))
                    if not typ:
                        continue
                    bases = {S[q].text for q in range(a, b - 1) if S[q].kind == "id" and S[q + 1].text in ("->", "[")}
                    bare = {S[q].text for q in range(lo, a) if S[q].kind == "id" and
                            (q + 1 >= a or S[q + 1].text not in ("->", ".", "[", "("))}
                    if bases & bare:
                        continue
                    c.append((st, blk, idx, a, b, typ))
    if not c:
        return None
    st, blk, idx, a, b, typ = rng.choice(c)
    name = B.fresh("tmp")
    ind = B.indent(st.a)
    expr = B.span(a, b)
    edits = [(S[a].start, S[b - 1].end, name)]
    if B.cpp:
        edits.append((S[st.a].start, S[st.a].start, f"{_decl(typ, name)} = {expr};\n{ind}"))
    else:
        if any(k.kind == "decl" for k in blk.kids[idx:]):
            return None
        first = blk.kids[0]
        ins = S[blk.a].end
        edits.append((ins, ins, f"\n{B.indent(first.a)}{_decl(typ, name)};"))
        edits.append((S[st.a].start, S[st.a].start, f"{name} = {expr};\n{ind}"))
    return B.edit(edits)


def m_inline_call(B: Body, rng):
    """UNSAFE (the call may now run after reads it used to precede): `T x = f(..);` (or `x = f(..);` with a
    plain `T x;`) whose only use is in the next statement, evaluated unconditionally -> `((T)(f(..)))` there."""
    S, M = B.S, B.M
    c = []
    for blk in B.blocks:
        kids = blk.kids
        for i in range(len(kids) - 1):
            st, nx = kids[i], kids[i + 1]
            d = B.decl_of.get(id(st))
            decl_st = None
            if d and len(d[1]) == 1 and d[1][0].init and not d[1][0].array:
                dc, init = d[1][0], d[1][0].init
                decl_st = st
            else:
                p = _assign_parts(B, st)
                if not p or p[0][1] - p[0][0] != 1:
                    continue
                name = S[p[0][0]].text
                loc = B.locals.get(name)
                if not loc or loc.stmt is None or loc.decl.init or loc.decl.array:
                    continue
                d2 = B.decl_of.get(id(loc.stmt))
                if not d2 or len(d2[1]) != 1:
                    continue
                dc, init, d = loc.decl, p[1], d2
                decl_st = loc.stmt
            name = dc.name
            if name in B.escaped or _decl_count(B, name) != 1 or not B.eff(*init).call:
                continue
            uses = [k for k in _use_positions(B, name, 0, len(S)) if not st.a <= k < st.b
                    and not decl_st.a <= k < decl_st.b]
            if len(uses) != 1 or not nx.a <= uses[0] < nx.b or _is_written(B, uses[0]):
                continue
            if nx.kind not in ("expr", "return", "decl", "if"):
                continue
            k = uses[0]
            if any(S[q].text in ("&&", "||", "?") for q in range(nx.a, k)) or nx.kind == "if" and k >= nx.cond[1]:
                continue
            c.append((st, decl_st, d, dc, init, k))
    if not c:
        return None
    st, decl_st, d, dc, init, k = rng.choice(c)
    rep = f"(({_ctype(B, d, dc)})({B.span(*init)}))"
    edits = [(S[k].start, S[k].end, rep), _drop_stmt(B, st)]
    if decl_st is not st:
        edits.append(_drop_stmt(B, decl_st))
    return B.edit(edits)


# --- registry --------------------------------------------------------------------------------------------------

MUTATIONS = {
    # name: (weight, function, safe)
    "swap_stmts": (12, m_swap_stmts, True),
    "swap_decls": (12, m_swap_decls, True),
    "move_stmt": (5, m_move_stmt, True),
    "move_decl": (5, m_move_decl, True),
    "commute": (10, m_commute, True),
    "flip_relational": (5, m_flip_relational, True),
    "invert_if": (8, m_invert_if, True),
    "invert_if_noelse": (3, m_invert_if_noelse, True),
    "else_after_jump": (4, m_else_after_jump, True),
    "split_and": (4, m_split_and, True),
    "merge_if": (4, m_merge_if, True),
    "zero_compare": (4, m_zero_compare, True),
    "split_decl_init": (5, m_split_decl_init, True),
    "register": (3, m_register, True),
    "incdec": (4, m_incdec, True),
    "for_to_while": (4, m_for_to_while, True),
    "while_to_for": (4, m_while_to_for, True),
    "loop_rotate": (2, m_loop_rotate, True),
    "ternary_to_if": (4, m_ternary_to_if, True),
    "if_to_ternary": (4, m_if_to_ternary, True),
    "add_temp": (8, m_add_temp, True),
    "remove_temp": (6, m_remove_temp, True),
    "scope_decl": (6, m_scope_decl, True),
    "inline_var": (5, m_inline_var, True),
    "cse": (5, m_cse, True),
    "tail_return": (4, m_tail_return, True),
    "hoist_incdec": (4, m_hoist_incdec, True),
    "chain_assign": (5, m_chain_assign, True),
    "compound_assign": (4, m_compound_assign, True),
    "negate_const": (3, m_negate_const, True),
    "bool_return": (3, m_bool_return, True),
    "reassociate": (3, m_reassociate, True),
    "cond_temp": (3, m_cond_temp, True),
    "scope_block": (4, m_scope_block, True),
    "const_compare": (3, m_const_compare, False),
    "hoist_cond_operand": (4, m_hoist_cond_operand, True),
    "inline_call": (3, m_inline_call, False),
    "pow2_shift": (2, m_pow2_shift, False),
    "ref_local": (2, m_ref_local, False),
    "change_type": (3, m_change_type, False),
    "cast": (2, m_cast, False),
}


def mutate(text: str, rng: random.Random, params=(), cpp=True, types=None, allow_unsafe=True,
           only=None, tries=30):
    """One random applicable mutation of a body: (new text, mutation name) or None."""
    try:
        B = Body(text, params, cpp, types)
    except (ParseError, ValueError, KeyError, IndexError):
        return None
    names = [n for n, (w, f, safe) in MUTATIONS.items() if (allow_unsafe or safe) and (only is None or n in only)]
    weights = [MUTATIONS[n][0] for n in names]
    for _ in range(tries):
        n = rng.choices(names, weights)[0]
        try:
            out = MUTATIONS[n][1](B, rng)
        except (ParseError, ValueError, KeyError, IndexError, StopIteration):
            out = None
        if out is not None and out != text:
            return out, n
    return None
