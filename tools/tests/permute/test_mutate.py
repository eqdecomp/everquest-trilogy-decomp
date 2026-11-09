"""permute/: tokenizer, statement parser, effects and the mutations (no game files, no VC6)."""
import random
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "permute"))

import ctoken as T  # noqa: E402
import mutate as M  # noqa: E402
from mutate import Body  # noqa: E402


def body(src, cpp=True, types=None, params=()):
    return Body(src, params, cpp, types)


def all_outputs(fn, src, cpp=True, types=None, n=40):
    """Every distinct result of a mutation over n seeds."""
    out = set()
    for s in range(n):
        r = fn(body(src, cpp, types), random.Random(s))
        if r is not None:
            out.add(r)
    return out


def squash(s):
    return " ".join(t.text for t in T.significant(T.tokenize(s)))


# --- tokenizer / parser -----------------------------------------------------------------------------------------

def test_tokenize_keeps_offsets_and_preprocessor_lines():
    src = '#include "a.h"\nint x = 0x1F; // c\nchar *s = "a;b";\n'
    toks = T.tokenize(src)
    assert "".join(t.text for t in toks) == src
    assert toks[0].kind == "pp"
    kinds = [t.kind for t in T.significant(toks)]
    assert "comment" not in kinds and "str" in kinds


def test_parser_statement_kinds():
    b = body("{ int i; if (a) x = 1; else { y++; } for (i = 0; i < 3; i++) f(i); while (k) k--; "
             "do { k++; } while (k < 3); switch (k) { case 1: break; default: return 2; } }")
    kinds = [s.kind for s in b.root.kids]
    assert kinds == ["decl", "if", "for", "while", "do", "switch"]
    assert b.root.kids[1].els is not None
    assert "i" in b.locals


def test_is_decl():
    for src, want in [("{ Foo* p = q; }", "decl"), ("{ unsigned int n; }", "decl"), ("{ a = b * c; }", "expr"),
                      ("{ p->x = 3; }", "expr"), ("{ CFoo::Bar x; }", "decl"), ("{ f(x); }", "expr")]:
        assert body(src).root.kids[0].kind == want, src


def test_effects_and_conflicts():
    b = body("{ int a; int b; a = p->x; b = a + 1; q->y = 2; g(); }")
    k = b.root.kids
    e = [b.stmt_eff(s) for s in k[2:]]
    assert T.conflicts(e[0], e[1])               # b = a + 1 reads a
    assert not T.conflicts(e[0], e[2])           # p->x vs q->y: different fields
    assert T.conflicts(e[0], e[3])               # a call may write p->x
    assert not T.conflicts(b.stmt_eff(k[0]), b.stmt_eff(k[1]))


def test_struct_fields_of_a_local_are_separate():
    b = body("{ Msg m; m.a = g_p->id; m.b = 5; send(&m); }")
    k = b.root.kids
    assert "m" in b.escaped
    assert not T.conflicts(b.stmt_eff(k[1]), b.stmt_eff(k[2]))


def test_cast_is_not_a_multiplication():
    S = T.significant(T.tokenize("(u32)*(s8*)&x & 2"))
    Mb = T.match_brackets(S)
    assert not T.ends_operand(S, Mb, 2)          # the ')' of (u32)


# --- mutations ----------------------------------------------------------------------------------------------------

def test_swap_stmts_only_independent():
    outs = all_outputs(M.m_swap_stmts, "{\n    a = 1;\n    b = a;\n    c = 2;\n}")
    assert outs == {"{\n    a = 1;\n    c = 2;\n    b = a;\n}"}


def test_swap_keeps_whitespace_in_place():
    out = M.m_swap_stmts(body("{\n    Msg x;\n    x.a = g->id;\n\n    x.b = 5;\n}"), random.Random(0))
    assert out == "{\n    Msg x;\n    x.b = 5;\n\n    x.a = g->id;\n}"


def test_swap_decls_respects_initializer_dependency():
    assert all_outputs(M.m_swap_decls, "{\n    int a = 1;\n    int b = a;\n}") == set()
    assert all_outputs(M.m_swap_decls, "{\n    int a = 1;\n    int b = 2;\n}") == {"{\n    int b = 2;\n    int a = 1;\n}"}


def test_commute_and_parenthesise():
    outs = {squash(o) for o in all_outputs(M.m_commute, "{ return a - b + c; }")}
    assert outs == {squash("{ return c + (a - b); }")}
    assert all_outputs(M.m_commute, "{ return f() + g(); }") == set()      # call order
    assert {squash(o) for o in all_outputs(M.m_commute, "{ return *(s8*)&x & 1; }")} == \
        {squash("{ return 1 & *(s8*)&x; }")}


def test_flip_relational():
    assert {squash(o) for o in all_outputs(M.m_flip_relational, "{ if (a < b + 1) f(); }")} == \
        {squash("{ if (b + 1 > a) f(); }")}


def test_const_compare():
    assert {squash(o) for o in all_outputs(M.m_const_compare, "{ if (x < 0xDD) f(); }")} == \
        {squash("{ if (x <= 0xDC) f(); }")}


def test_invert_if_wraps_a_dangling_else():
    out = squash(M.m_invert_if(body("{ if (a == 1) x(); else if (b) y(); }"), random.Random(0)))
    assert out == squash("{ if (a != 1) { if (b) y(); } else x(); }")


def test_negate_strips_a_bang():
    assert squash(M.m_invert_if(body("{ if (!(a && b)) x(); else y(); }"), random.Random(0))) == \
        squash("{ if (a && b) y(); else x(); }")


def test_for_to_while_and_back():
    src = "{\n    for (i = 0; i < n; i++)\n        f(i);\n}"
    out = M.m_for_to_while(body(src), random.Random(0))
    assert squash(out) == squash("{ i = 0; while (i < n) { f(i); i++; } }")
    back = M.m_while_to_for(body(out), random.Random(1))
    assert back is not None
    assert all_outputs(M.m_for_to_while, "{ for (i = 0; i < n; i++) { if (a) continue; f(i); } }") == set()


def test_incdec_forms():
    outs = {squash(o) for o in all_outputs(M.m_incdec, "{ i++; }")}
    assert outs == {squash(x) for x in ("{ ++i; }", "{ i += 1; }", "{ i = i + 1; }")}


def test_ternary_round_trip():
    out = M.m_ternary_to_if(body("{\n    x = c ? 1 : 2;\n}"), random.Random(0))
    assert squash(out) == squash("{ if (c) x = 1; else x = 2; }")
    assert squash(M.m_if_to_ternary(body(out), random.Random(0))) == squash("{ x = c ? 1 : 2; }")


def test_split_and_merge():
    out = M.m_split_and(body("{\n    if (a && b)\n        f();\n}"), random.Random(0))
    assert squash(out) == squash("{ if (a) { if (b) f(); } }")
    assert squash(M.m_merge_if(body(out), random.Random(0))) == squash("{ if (a && b) f(); }")
    assert all_outputs(M.m_split_and, "{ if (a && b) f(); else g(); }") == set()


def test_zero_compare():
    outs = {squash(o) for o in all_outputs(M.m_zero_compare, "{ if (p != 0 && !q) f(); }")}
    assert squash("{ if (p && !q) f(); }") in outs
    assert squash("{ if (p != 0 && q == 0) f(); }") in outs


def test_remove_temp_single_use_only():
    outs = all_outputs(M.m_remove_temp, "{\n    int t = a + b;\n    f(t);\n}")
    assert {squash(o) for o in outs} == {squash("{ f(((int)(a + b))); }")}
    assert all_outputs(M.m_remove_temp, "{\n    int t = a + b;\n    f(t);\n    g(t);\n}") == set()


def test_add_temp_uses_probed_type():
    types = {T.norm("p->next"): "struct Node *"}
    out = M.m_add_temp(body("{\n    f(p->next->v);\n}", types=types), random.Random(0))
    assert squash(out) == squash("{ struct Node *temp = p->next; f(temp->v); }")
    out = M.m_add_temp(body("{\n    int k;\n    f(p->next->v);\n}", cpp=False, types=types), random.Random(0))
    assert squash(out) == squash("{ struct Node *temp; int k; temp = p->next; f(temp->v); }")


def test_add_temp_not_out_of_a_short_circuit():
    types = {T.norm("p->v"): "int"}
    assert all_outputs(M.m_add_temp, "{\n    if (p && p->v) f();\n}", types=types) == set()


def test_cse_and_inline_var():
    types = {T.norm("p->a"): "int"}
    out = M.m_cse(body("{\n    x = p->a + 1;\n    y = p->a * 2;\n}", types=types), random.Random(0))
    assert squash(out) == squash("{ int temp = p->a; x = temp + 1; y = temp * 2; }")
    assert all_outputs(M.m_cse, "{\n    x = p->a;\n    p->a = 3;\n    y = p->a;\n}", types=types) == set()
    back = M.m_inline_var(body(out, types=types), random.Random(0))
    assert squash(back) == squash("{ x = p->a + 1; y = p->a * 2; }")


def test_scope_decl_in_and_out():
    src = "{\n    int t;\n    if (a)\n    {\n        t = f();\n        g(t);\n    }\n}"
    out = M.m_scope_decl(body(src), random.Random(0))
    assert squash(out) == squash("{ if (a) { int t; t = f(); g(t); } }")
    assert squash(M.m_scope_decl(body(out), random.Random(0))) == squash(src)
    loop = "{\n    int t = 0;\n    while (a)\n    {\n        t = t + 1;\n    }\n}"
    assert all(squash(o) != squash("{ while (a) { int t = 0; t = t + 1; } }")
               for o in all_outputs(M.m_scope_decl, loop))


def test_tail_return_with_known_constant():
    src = "{\n    void *d;\n    d = 0;\n    if (n > 0)\n    {\n        d = get();\n    }\n    return d;\n}"
    outs = {squash(o) for o in all_outputs(M.m_tail_return, src)}
    assert squash("{ void *d; d = 0; if (n > 0) { d = get(); return d; } return 0; }") in outs


def test_hoist_incdec():
    out = M.m_hoist_incdec(body("{\n    if (--h->n != 0)\n        f();\n}"), random.Random(0))
    assert squash(out) == squash("{ --h->n; if (h->n != 0) f(); }")
    assert squash(M.m_hoist_incdec(body(out), random.Random(0))) == squash("{ if (--h->n != 0) f(); }")


def test_mutate_on_unparseable_body_is_none():
    assert M.mutate("{ if ( }", random.Random(0)) is None


@pytest.mark.parametrize("name", sorted(M.MUTATIONS))
def test_every_mutation_keeps_the_body_parseable(name):
    src = ("{\n    int i;\n    int n = 0;\n    Foo* p = g_foo;\n    if (p != 0 && p->a > 3)\n    {\n"
           "        for (i = 0; i < 10; i++)\n            n = n + p->b[i];\n    }\n    else\n        n = -1;\n"
           "    while (n > 100)\n        n--;\n    x = n ? 1 : 2;\n    return n;\n}")
    types = {T.norm("p->a"): "int", T.norm("p->b"): "int *"}
    for s in range(20):
        r = M.MUTATIONS[name][1](body(src, types=types), random.Random(s))
        if r is not None:
            Body(r)
