"""permute/mutate.py: the passes added after the cpp_permuter bake-off (no game files, no VC6)."""
import random
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "permute"))

import ctoken as T  # noqa: E402
import mutate as M  # noqa: E402
from mutate import Body  # noqa: E402


def outs(fn, src, cpp=True, n=60):
    res = set()
    for s in range(n):
        r = fn(Body(src, (), cpp), random.Random(s))
        if r is not None:
            res.add(squash(r))
    return res


def squash(s):
    return " ".join(t.text for t in T.significant(T.tokenize(s)))


def test_registered():
    for n in ("chain_assign", "compound_assign", "negate_const", "bool_return", "reassociate", "cond_temp",
              "scope_block", "pow2_shift", "ref_local"):
        assert n in M.MUTATIONS
    assert not M.MUTATIONS["pow2_shift"][2] and not M.MUTATIONS["ref_local"][2]


def test_chain_assign_merge_and_split():
    # the EQ_TextIO_ClearTextBuffer_guess shape (the bake-off's first match)
    got = outs(M.m_chain_assign, "{ cur = buf; rd = buf + 4; wr = buf + 4; }")
    assert "{ cur = buf ; wr = rd = buf + 4 ; }" in got
    assert "{ cur = buf ; rd = wr = buf + 4 ; }" in got
    got = outs(M.m_chain_assign, "{ a = b = c + 1; }")
    assert got == {"{ b = c + 1 ; a = b ; }", "{ b = c + 1 ; a = c + 1 ; }"}
    # `a = v; b = a;` merges too
    assert "{ b = a = v ; }" in outs(M.m_chain_assign, "{ a = v; b = a; }")


def test_chain_assign_respects_effects():
    assert outs(M.m_chain_assign, "{ a = f(); b = f(); }") == set()          # calls: not the same value
    assert outs(M.m_chain_assign, "{ i = k; p[i] = k; }") == set()         # second target reads the first
    assert outs(M.m_chain_assign, "{ k = k + 1; j = k + 1; }") == set()    # first write changes v
    # writing b changes v: the split must read b back, not v
    assert outs(M.m_chain_assign, "{ a = k = k + 1; }") == {"{ k = k + 1 ; a = k ; }"}


def test_compound_assign():
    assert outs(M.m_compound_assign, "{ x = x + y * 2; }") == {"{ x += y * 2 ; }"}
    assert outs(M.m_compound_assign, "{ x = y | x; }") == {"{ x |= y ; }"}
    assert outs(M.m_compound_assign, "{ x -= a + b; }") == {"{ x = x - ( a + b ) ; }"}
    assert outs(M.m_compound_assign, "{ x = x - a + b; }") == set()        # not x - (a + b)
    assert outs(M.m_compound_assign, "{ x = y - x; }") == set()            # not commutative


def test_negate_const():
    assert outs(M.m_negate_const, "{ q = r - 8; }") == {"{ q = r + - 8 ; }"}
    assert outs(M.m_negate_const, "{ z += -4; }") == {"{ z -= 4 ; }"}
    assert outs(M.m_negate_const, "{ q = r - 8 * k; }") == set()           # 8 binds to * first


def test_pow2_shift_parenthesises():
    assert outs(M.m_pow2_shift, "{ x = a + y * 8; }") == {"{ x = a + ( y << 3 ) ; }"}
    assert outs(M.m_pow2_shift, "{ x = y >> 2; }") == {"{ x = ( y / 4 ) ; }"}
    assert outs(M.m_pow2_shift, "{ x = y * 6; }") == set()


def test_bool_return_both_ways():
    got = outs(M.m_bool_return, "{ return a < b; }")
    assert got == {"{ if ( a < b ) return 1 ; return 0 ; }", "{ if ( a < b ) return 1 ; else return 0 ; }"}
    assert outs(M.m_bool_return, "{ if (a < b) return 0; return 1; }") == {"{ return a >= b ; }"} or \
        outs(M.m_bool_return, "{ if (a < b) return 0; return 1; }") == {"{ return ! ( a < b ) ; }"}
    assert outs(M.m_bool_return, "{ if (p) return 1; else return 0; }") == {"{ return p ; }"}
    assert outs(M.m_bool_return, "{ return a + b; }") == set()


def test_reassociate():
    assert outs(M.m_reassociate, "{ s = a + b + c; }") == {"{ s = a + ( b + c ) ; }"}
    assert outs(M.m_reassociate, "{ s = a + (b + c); }") == {"{ s = a + b + c ; }"}
    assert outs(M.m_reassociate, "{ s = a + f() + c; }") == set()


def test_cond_temp():
    assert outs(M.m_cond_temp, "{ if (n > 3 && p->f) k = 1; }") == {"{ int cond = n > 3 && p -> f ; if ( cond ) k = 1 ; }"}
    # C: only where declarations may still go
    assert outs(M.m_cond_temp, "{ f(); if (n > 3) k = 1; }", cpp=False) == set()
    assert outs(M.m_cond_temp, "{ int k; if (n > 3) k = 1; }", cpp=False)


def test_ref_local():
    got = outs(M.m_ref_local, "{ int n = p->count; int j = 2; g(n, j); }")
    assert got == {"{ int & n = p -> count ; int j = 2 ; g ( n , j ) ; }",
                   "{ const int & n = p -> count ; int j = 2 ; g ( n , j ) ; }"}
    assert outs(M.m_ref_local, "{ const int& n = p->count; g(n); }") >= {"{ int n = p -> count ; g ( n ) ; }"}
    assert outs(M.m_ref_local, "{ int n = p->count; g(n); }", cpp=False) == set()


def test_scope_block_wrap_and_unwrap():
    got = outs(M.m_scope_block, "{ f(); int k = g(); h(k); x = 1; }")
    assert "{ f ( ) ; { int k = g ( ) ; h ( k ) ; } x = 1 ; }" in got
    got = outs(M.m_scope_block, "{ f(); { int j = 2; g(j); } }")
    assert "{ f ( ) ; int j = 2 ; g ( j ) ; }" in got
    # C: no unwrapping a block with declarations
    assert "{ f ( ) ; int j = 2 ; g ( j ) ; }" not in outs(M.m_scope_block, "{ f(); { int j = 2; g(j); } }",
                                                           cpp=False)


CONN = """{
    DWORD now;

    now = rdp_timeGetTime_guess();
    if (conn->timeout_armed && (int)(conn->timeout_time - now) <= 10)
        return 1;
    return 0;
}"""


def test_hoist_cond_operand_and_inline_call_reach_the_cpp_permuter_match():
    # NetworkConnection_isConnectionTimedOut_guess: cpp_permuter's match, in two of our steps
    h = outs(M.m_hoist_cond_operand, CONN, cpp=False)
    assert h == {"{ int tmp ; DWORD now ; now = rdp_timeGetTime_guess ( ) ; tmp = ( int ) ( conn -> timeout_time "
                 "- now ) ; if ( conn -> timeout_armed && tmp <= 10 ) return 1 ; return 0 ; }"}
    step = None
    for s in range(40):
        step = M.m_hoist_cond_operand(Body(CONN, (), False), random.Random(s))
        if step:
            break
    got = outs(M.m_inline_call, step, cpp=False)
    assert any("DWORD now" not in g and "( ( DWORD ) ( rdp_timeGetTime_guess ( ) ) )" in g for g in got)


def test_hoist_cond_operand_guards():
    assert outs(M.m_hoist_cond_operand, "{ if (p && (int)(p->x - 1) <= 10) f(); }") == set()   # null test first
    assert outs(M.m_hoist_cond_operand, "{ if (a && (int)(g() - 1) <= 10) f(); }") == set()    # a call
    assert outs(M.m_hoist_cond_operand, "{ if ((int)(q->x - 1) <= 10) f(); }") == set()        # not conditional


def test_inline_call_only_unconditional_single_use():
    assert outs(M.m_inline_call, "{ int x = g(); if (a && x) f(); }") == set()
    assert outs(M.m_inline_call, "{ int x = g(); f(x); h(x); }") == set()
    assert outs(M.m_inline_call, "{ int x = g(); f(x); }") == {"{ f ( ( ( int ) ( g ( ) ) ) ) ; }"}


def test_const_compare_named_bound():
    # CDisplay::GetUserDefinedColor_guess: original `cmp eax, 0x21; jg`, source `index >= NUM_USER_COLORS_GUESS`
    got = outs(M.m_const_compare, "{ if (index >= NUM_COLORS) index = 0; }")
    assert got == {"{ if ( index > ( NUM_COLORS - 1 ) ) index = 0 ; }"}
    assert outs(M.m_const_compare, "{ if (MAXV < y) f(); }") == {"{ if ( ( MAXV + 1 ) <= y ) f ( ) ; }"}
    assert outs(M.m_const_compare, "{ if (x < count) f(); }") == set()       # not a constant-looking name
