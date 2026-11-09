"""Typedef hints for cast recognition (no preprocessing or compilation)."""
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "permute"))

import ctoken as ct  # noqa: E402


def tokens(src):
    S = ct.significant(ct.tokenize(src))
    return S, ct.match_brackets(S)


def test_collect_typedef_names_without_members_bounds_or_parameters():
    S, M = tokens("""
        typedef struct tag { int member; } record, *record_ptr;
        typedef int values[bound];
        typedef record alias;
        typedef int (__stdcall *callback)(int param, record *other);
        int (*variable)(int);
        // typedef int comment_name;
        #define DECL typedef int macro_name;
    """)
    assert ct.typedef_names(S, M) == {"record", "record_ptr", "values", "alias", "callback"}


@pytest.mark.parametrize("cc", ["", "__cdecl", "__stdcall", "__fastcall", "__thiscall"])
def test_function_pointer_typedef_names(cc):
    S, M = tokens(f"typedef int ({cc} *callback)(int param);")
    assert ct.typedef_names(S, M) == {"callback"}


@pytest.mark.parametrize("operand", ["(value)(arg)", "value", "*ptr", "&value", "+value", "-value"])
def test_typedef_hint_recognizes_casts_independent_of_spelling(operand):
    for name in ("lower_guess", "Upper_guess"):
        S, M = tokens(f"({name}){operand}")
        assert ct.is_cast_group(S, M, 0, {name})


@pytest.mark.parametrize("name, expected", [("lower_guess", False), ("Upper_guess", True), ("other_t", True),
                                           ("u32", True), ("x", False)])
def test_callers_without_typedef_hints_keep_the_heuristic(name, expected):
    S, M = tokens(f"({name})(value)")
    assert ct.is_cast_group(S, M, 0) is expected


def test_unrelated_typedef_does_not_turn_a_call_into_a_cast():
    S, M = tokens("(x)(y)")
    assert not ct.is_cast_group(S, M, 0, {"other_type"})
