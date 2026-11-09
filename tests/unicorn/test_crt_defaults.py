"""crt_defaults.json: every default alias must name the same CRT function in the test DLL as the client has at
the aliased VA (both are VC6 SP4 LIBCMT), and the fixture defaults must reach the fixtures."""
import re

import pytest
from capstone import CS_ARCH_X86, CS_MODE_32, Cs

from conftest import HERE, crt_defaults, fixture_paths, link_sources

# One call per default so the test DLL links all of them (C++: the ctype functions are calls, not macros).
SOURCE = r"""
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern "C" __declspec(dllexport) int crt_defaults_user(char* a, const char* b, int n, float f)
{
    int r = (int)f + abs(n);
    r += strlen(b);
    strcpy(a, b);
    strcat(a, b);
    r += strcmp(a, b) + strncmp(a, b, n) + _strcmpi(a, b) + _stricmp(a, b) + _strnicmp(a, b, n);
    strncpy(a, b, n);
    memcpy(a, b, n);
    memset(a, n, n);
    r += sprintf(a, "%d", n) + _snprintf(a, n, "%s", b);
    r += atoi(b) + atol(b);
    r += isdigit(n) + isspace(n) + isalpha(n) + isprint(n) + toupper(n);
    FILE* file = fopen(b, "rb");
    r += fread(a, 1, n, file);
    r += fwrite(a, 1, n, file);
    r += fclose(file);
    r += (int)time(0);
    char big[0x1100];               // > 4 KB of locals: __chkstk
    big[n & 0xFF] = (char)r;
    return r + big[n & 0x7F];
}
"""
MAX_INSNS = 48
_ADDR = re.compile(r"0x[0-9a-f]{4,}")


def shape(read, va, call_depth=0):
    """The instruction sequence from `va` to the first ret or jmp (memcpy has a jump table inline), with
    addresses (>= 0x1000) masked. Include one level of direct-call shapes so thin wrappers that differ only
    in the helper they call (e.g. fread/fwrite) remain distinguishable across image bases."""
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    out = []
    callees = []
    for insn in md.disasm(read(va, 0x200), va):
        out.append(f"{insn.mnemonic} {_ADDR.sub('X', insn.op_str)}")
        if call_depth and insn.mnemonic == "call":
            target = _ADDR.search(insn.op_str)
            if target:
                callees.append(shape(read, int(target.group(), 16), call_depth - 1))
        if insn.mnemonic in ("ret", "jmp") or len(out) >= MAX_INSNS:
            break
    return out + [("callee", callee) for callee in callees]


def alias_shape(read, symbol, va):
    """Compare one level into fread/fwrite because their entry-point wrappers are otherwise identical."""
    return shape(read, va, call_depth=1 if symbol in ("_fread", "_fwrite") else 0)


@pytest.fixture(scope="module")
def crt_dll(harness, tmp_path_factory):
    src = tmp_path_factory.mktemp("crt") / "crt_defaults_user.cpp"
    src.write_text(SOURCE, encoding="latin-1")
    return harness.rewrite_image("crt_defaults_check", [src] + link_sources())


@pytest.mark.parametrize("symbol, va", sorted(crt_defaults()["rewrite_aliases"].items()))
def test_alias_is_the_same_crt_function(harness, crt_dll, symbol, va):
    sva = crt_dll.by_name.get(symbol)
    assert sva is not None, f"{symbol} is not in the test DLL"
    want = alias_shape(harness.original.read, symbol, int(va, 16))
    got = alias_shape(crt_dll.read, symbol, sva)
    assert got == want, f"{symbol} differs from the client's code at {va}"


def test_fread_and_fwrite_aliases_are_not_swapped(harness, crt_dll):
    """Negative: the same comparator used by positive alias checks rejects swapped read/write helpers."""
    for symbol, other_va in (("_fread", 0x0052359E), ("_fwrite", 0x00522038)):
        sva = crt_dll.by_name[symbol]
        assert alias_shape(crt_dll.read, symbol, sva) != alias_shape(harness.original.read, symbol, other_va)


def test_the_check_tells_the_functions_apart(harness):
    """Negative: aliasing a symbol to another default's VA must fail the check above."""
    aliases = crt_defaults()["rewrite_aliases"]
    vas = sorted({int(v, 16) for v in aliases.values()})
    # Use each symbol's positive-check comparator against every incorrect target.
    shapes = {(symbol, va): alias_shape(harness.original.read, symbol, va)
              for symbol in aliases for va in vas}
    same = [(symbol, f"{other:#x}") for symbol, value in aliases.items()
            for other in vas if other != int(value, 16)
            and shapes[symbol, int(value, 16)] == shapes[symbol, other]]
    assert not same


def test_every_default_alias_has_a_default_callee():
    d = crt_defaults()
    callees = {int(k, 16) for k in d["callees"]}
    assert {int(v, 16) for v in d["rewrite_aliases"].values()} <= callees


def test_fixtures_live_in_their_source_files_subsystem():
    """Layout: tests/unicorn/<subsystem>/<Function>.json, <subsystem> = the rewrite's src/<subsystem>.cpp
    (the queue's subsystem, the Mac source path without extension)."""
    import json
    bad = []
    for p in fixture_paths():
        src = json.loads(p.read_text(encoding="utf-8"))["sources"][0]
        want = src.removeprefix("src/").rsplit(".", 1)[0]
        got = p.parent.relative_to(HERE).as_posix()
        if got != want:
            bad.append(f"{p.relative_to(HERE).as_posix()}: sources[0] is {src}, so it belongs in {want}/")
    assert not bad, "\n".join(bad)


def test_defaults_reach_the_fixtures_and_a_fixture_overrides_them(harness):
    """conftest passes crt_defaults.json to the Harness: every fixture gets the default CRT callees, and a
    fixture's own entry with the same key replaces the default (MakeNPCCorpseName: strncpy's destination is
    a stack buffer, so it records `ignore` instead of the pointer)."""
    from conftest import REPO, load_fixture
    fx = load_fixture(HERE / "EverQuest" / "EQMisc" / "MakeNPCCorpseName.json")
    assert "rewrite_aliases" not in fx and "0x005223d0" not in fx["callees"]
    t = harness.difftest(fx, fx["sources"], base_dir=REPO)
    defaults = {int(k, 16) for k in crt_defaults()["callees"]}
    assert defaults <= set(t.callees)
    assert t.callees[0x5231D0].args[0] == "ignore"
    assert t.callees[0x5224FB].varargs == "printf"


def test_without_defaults_a_crt_call_is_unscripted(run_fixture):
    """Negative: `"defaults": false` drops the table, and is_number's isdigit calls are then unscripted."""
    rep = run_fixture(HERE / "EverQuest" / "EQMisc" / "is_number.json", defaults=False)
    assert not rep.ok and "unscripted" in rep.summary()


# --- eqmain.dll (VC6 SP5 LIBC; tests/unicorn/eqmain/ fixtures get crt_defaults_eqmain.json) -------------------

@pytest.fixture(scope="module")
def eqmain_crt(tmp_path_factory):
    from conftest import make_harness
    h = make_harness("eqmain")
    src = tmp_path_factory.mktemp("crt_eqmain") / "crt_defaults_user.cpp"
    src.write_text(SOURCE.replace("int r = (int)f", "int r = isupper(n) + tolower(n) + (strchr(b, n) != 0)"
                                  " + (strstr(a, b) != 0) + (int)f"), encoding="latin-1")
    return h, h.rewrite_image("crt_defaults_check_eqmain", [src] + link_sources("eqmain"))


@pytest.mark.parametrize("symbol, va", sorted(crt_defaults("eqmain").get("rewrite_aliases", {}).items()))
def test_eqmain_alias_is_the_same_crt_function(eqmain_crt, symbol, va):
    h, dll = eqmain_crt
    sva = dll.by_name.get(symbol)
    assert sva is not None, f"{symbol} is not in the test DLL"
    assert alias_shape(dll.read, symbol, sva) == alias_shape(h.original.read, symbol, int(va, 16)), \
        f"{symbol} differs from eqmain.dll's code at {va}"


def test_eqmain_every_default_alias_has_a_default_callee():
    d = crt_defaults("eqmain")
    assert {int(v, 16) for v in d["rewrite_aliases"].values()} <= {int(k, 16) for k in d["callees"]}


# --- eqgfx_dx7.dll (VC6 SP4 LIBCMT; tests/unicorn/eqgfx/ fixtures get crt_defaults_eqgfx.json) ----------------

@pytest.fixture(scope="module")
def eqgfx_crt(tmp_path_factory):
    from conftest import make_harness
    h = make_harness("eqgfx")
    src = tmp_path_factory.mktemp("crt_eqgfx") / "crt_defaults_user.cpp"
    src.write_text(SOURCE.replace("int r = (int)f", "int r = tolower(n) + (strchr(b, n) != 0) + (strrchr(b, n) != 0)"
                                  " + (strstr(a, b) != 0) + memcmp(a, b, n) + (int)f") + r"""
extern "C" {
void _EH_prolog();
// Keep the compiler helper linked for the alias identity check.
__declspec(dllexport) void (*eqgfx_crt_eh_helper)(void) = _EH_prolog;
}
""", encoding="latin-1")
    return h, h.rewrite_image("crt_defaults_check_eqgfx", [src] + link_sources("eqgfx"))


@pytest.mark.parametrize("symbol, va", sorted(crt_defaults("eqgfx").get("rewrite_aliases", {}).items()))
def test_eqgfx_alias_is_the_same_crt_function(eqgfx_crt, symbol, va):
    h, dll = eqgfx_crt
    sva = dll.by_name.get(symbol)
    assert sva is not None, f"{symbol} is not in the test DLL"
    assert alias_shape(dll.read, symbol, sva) == alias_shape(h.original.read, symbol, int(va, 16)), \
        f"{symbol} differs from eqgfx_dx7.dll's code at {va}"


def test_eqgfx_every_default_alias_has_a_default_callee():
    d = crt_defaults("eqgfx")
    assert {int(v, 16) for v in d["rewrite_aliases"].values()} <= {int(k, 16) for k in d["callees"]}
