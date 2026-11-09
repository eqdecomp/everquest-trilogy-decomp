"""mutations.py (mutate_check's mutant generator) and make_fixture's prototype parsing: pure text, no compiler."""
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn" / "tools"))

import make_fixture as mf  # noqa: E402
import mutations as mu  # noqa: E402

SRC = r'''
#include "x.h"
// a comment with if (a < b) and 42 in it
// FUNCTION: TRILOGY 0x00401000
// ORIGINAL FILE: Foo.cpp
int Foo::Bar_guess(int a, int b, char *name)
{
    int i;
    int total = 0;
    static const char *s = "x < 3; y++";   /* if (q == 7) */

    if (a < b)
        total += a - b;
    for (i = 0; i <= 10; i++)
        total = total * 2;
    while (name[i] != 0 && i < 0x20)
        i++;
    Helper(total, !a);
    return total;
}

// FUNCTION: TRILOGY 0x00402000
void plain_c(int *p)
{
    int k;
    k = 3;
    *p = k >> 1;
}
'''


def kinds(ms):
    return {m.kind for m in ms}


def test_body_and_head():
    o, c, head = mu.function_body(SRC, 0x00401000)
    assert SRC[o] == "{" and SRC[c - 1] == "}" and "return total;" in SRC[o:c]
    assert head == "int Foo :: Bar_guess ( int a , int b , char * name )"
    with pytest.raises(ValueError):
        mu.function_body(SRC, 0x00409999)


def test_mutants_stay_inside_the_body_and_skip_comments_and_strings():
    o, c, _ = mu.function_body(SRC, 0x00401000)
    ms = mu.mutants(SRC, 0x00401000)
    assert kinds(ms) == {"rel", "const", "arith", "swap", "delete", "negate", "return"}
    for m in ms:
        assert o <= m.start <= m.end <= c
        assert "x < 3" in m.apply(SRC)                         # the string literal is never touched
    descs = {m.desc for m in ms}
    assert "`<` -> `<=`" in descs and "`<=` -> `<` " not in descs
    assert "`a < b` -> `b < a`" in descs and "`a - b` -> `b - a`" in descs
    assert "`0x20` -> `0x21`" in descs and "`10` -> `11`" in descs and "`10` -> `9`" in descs
    assert "`+=` -> `-=`" in descs and "`&&` -> `||`" in descs and "`*` -> `/`" in descs
    assert "for increment `i++` deleted" in descs and "`!` removed" in descs
    assert any(d.startswith("statement `Helper(total, !a)`") for d in descs)
    assert any(d.startswith("`while (name[i] != 0 && i < 0x20)` negated") for d in descs)
    # single comparisons aren't negated (that would repeat the rel flip); declarations aren't deleted
    assert not any(d.startswith("`if (a < b)` negated") for d in descs)
    assert not any("deleted" in d and "int total" in d for d in descs)
    early = [m for m in ms if m.kind == "return"]
    assert len(early) == 1 and early[0].replacement == "return 0; "
    assert early[0].apply(SRC).index("return 0; if (a < b)") > SRC.index("static const char")


def test_c_function_void_return_after_declarations():
    ms = mu.mutants(SRC, 0x00402000)
    early = [m for m in ms if m.kind == "return"]
    assert early and early[0].replacement == "return; " and "int k;\n    return; k = 3;" in early[0].apply(SRC)
    assert "`*p = k >> 1` -> " not in {m.desc for m in ms}
    assert {"`>>` -> `<<`", "`k >> 1` -> `1 >> k`"} <= {m.desc for m in ms}
    # `int *p` / `*p = ...` are not multiplications
    assert not any(m.desc.startswith("`*` ->") for m in ms)


def test_select_spreads_over_kinds():
    ms = mu.mutants(SRC, 0x00401000)
    sel = mu.select(ms, 7)
    assert len(sel) == 7 and kinds(sel) == kinds(ms)
    assert sel == sorted(sel, key=lambda m: (m.start, m.kind))
    assert mu.select(ms, None) == ms


@pytest.mark.parametrize("t,want", [("int", "int"), ("unsigned char", "u8"), ("u16", "u16"), ("short", "short"),
                                    ("const char *", "cstr"), ("EQPlayer *", "ptr"), ("float", "float"),
                                    ("unsigned long", "u32"), ("bool", "bool"), ("__int64", "i64"),
                                    ("char*", "cstr"), ("int &", "ptr"), ("SomeEnum", "int")])
def test_ctype(t, want):
    assert mf.ctype(t) == want


def test_prototype_parsing():
    p = mf.Prototype("int CDisplay :: IsInvisible_guess ( EQPlayer * viewer , EQPlayer * target )",
                     "CDisplay::IsInvisible_guess")
    assert p.ret == "int" and p.params == [("viewer", "EQPlayer *"), ("target", "EQPlayer *")]
    p = mf.Prototype("static void __stdcall Foo :: Bar ( void )", "Foo::Bar")
    assert p.ret == "void" and p.params == [] and p.conv_keyword == "stdcall"
    p = mf.Prototype("Foo :: ~ Foo ( )", "Foo::~Foo")
    assert p.ret == "void" and p.params == []
    p = mf.Prototype("char * f ( unsigned int , char buf [ 64 ] )", "f")
    assert p.ret == "cstr" and p.params == [("", "unsigned int"), ("buf", "char *")]
    assert mf.OUT_NAMES.search("buf")             # make() turns a non-const char * output buffer into ptr
