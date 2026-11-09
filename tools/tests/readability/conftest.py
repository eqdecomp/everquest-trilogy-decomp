"""A small synthetic decomp repo (trilogy + eqmain + eqgfx sources, queue, fixtures, names) for the readability
tools' tests. No game files."""
import json
import subprocess
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "readability"))

EQADDR = """\
#ifndef TRILOGY_EQADDR_H
#define TRILOGY_EQADDR_H
#ifdef EQD_MATCH
#define EQ_GLOBAL(type, va)                 (*(type *)&eqd_va_##va)
#define EQ_ARRAY(type, va)                  ((type *)&eqd_va_##va)
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)eqd_fn_##va)
#else
#define EQ_GLOBAL(type, va)                 (*(type *)(va))
#define EQ_ARRAY(type, va)                  ((type *)(va))
#define EQ_FUNC(ret, conv, va, params)      ((ret (conv *) params)(va))
#endif
#define EQ_CDECL(ret, va, params)           EQ_FUNC(ret, __cdecl, va, params)
#endif
"""

HEADER = """\
#include "trilogy/eqaddr.h"
struct Widget
{
    int count;
    int Total_guess(int extra);
};
#define g_widget_guess EQ_GLOBAL(Widget *, 0x006E0010)
"""

WIDGET = """\
// widget.cpp - test source
#include "trilogy/widget.h"

#define ALLOC_MEMORY EQ_CDECL(void *, 0x00401500, (unsigned int))
#define g_scale EQ_GLOBAL(int, 0x006E0020)
#define BUMP() (count_ref += 1)

// FUNCTION: TRILOGY 0x00401000
// Sums count and extra. Called by FUN_00401100.
int Widget::Total_guess(int extra)
{
    return this->count + extra * g_scale;
}

// FUNCTION: TRILOGY 0x00401100
int FUN_00401100(int param_1)
{
    int local_8;
    int count_ref;

    count_ref = param_1;
    BUMP();
    local_8 = g_widget_guess->Total_guess(param_1);
    if (local_8 < 0)
        goto LAB_00401130;
    local_8 += (int)ALLOC_MEMORY(4);
LAB_00401130:
    return local_8 + count_ref;
}
"""

OTHER = """\
// other.cpp - calls the widget by name and by VA
#include "trilogy/widget.h"

#define WidgetTotal EQ_CDECL(int, 0x00401100, (int))
#define g_scale_factor EQ_GLOBAL(int, 0x006e0020)

// FUNCTION: TRILOGY 0x00402000
int UseWidget_guess(Widget *w)
{
    return w->Total_guess(1) + WidgetTotal(2) + g_scale_factor;
}
"""

EQMAIN = """\
// unit_10001000.cpp - eqmain test source
#include "trilogy/eqaddr.h"

#define PTR_Thing_guess EQ_GLOBAL(int, 0x10040000)

// FUNCTION: EQMAIN 0x10001000
int DrawRows_guess(int *param_1)
{
    return param_1[1] + PTR_Thing_guess;
}
"""


def write(p: Path, text: str):
    p.parent.mkdir(parents=True, exist_ok=True)
    p.write_text(text, encoding="utf-8")


def make_repo(root: Path, git=False) -> Path:
    write(root / "include/trilogy/eqaddr.h", EQADDR)
    write(root / "include/trilogy/widget.h", HEADER)
    write(root / "src/EverQuest/widget.cpp", WIDGET)
    write(root / "src/EverQuest/other.cpp", OTHER)
    write(root / "src/eqmain/unit_10001000.cpp", EQMAIN)
    q = [{"address": "00401000", "name": "Widget::Total_guess", "state": "tested"},
         {"address": "00401100", "name": "FUN_00401100", "state": "tested"},
         {"address": "00402000", "name": "UseWidget_guess", "state": "tested"}]
    write(root / "queue/queue.jsonl", "".join(json.dumps(x) + "\n" for x in q))
    write(root / "queue/eqmain.jsonl", json.dumps({"address": "10001000", "name": "FUN_10001000",
                                                   "state": "tested"}) + "\n")
    write(root / "tests/unicorn/EverQuest/widget/FUN_00401100.json", json.dumps({
        "function": "FUN_00401100", "original": "0x00401100", "sources": ["src/EverQuest/widget.cpp"],
        "callees": {"0x00401500": {"name": "AllocMemory", "conv": "cdecl"}}}, indent=2) + "\n")
    write(root / "tests/unicorn/EverQuest/other/UseWidget.json", json.dumps({
        "function": "UseWidget_guess", "original": "0x00402000", "sources": ["src/EverQuest/other.cpp"],
        "callees": {"0x00401100": {"name": "FUN_00401100", "conv": "cdecl"}}}, indent=2) + "\n")
    write(root / "names/eqgame.manual.csv", "address,name,confidence,source,note,from_address,file\n"
          "00402000,UseWidget,low,manual,\"a note, with a comma\",,\n")
    write(root / "names/eqgame.mac.csv", "address,name,confidence,source,note,from_address,file\n"
          "00401100,Widget::Accumulate,medium,mac,chain,,\n")
    if git:
        for cmd in (["init", "-q"], ["config", "user.email", "t@example.com"], ["config", "user.name", "t"],
                    ["config", "core.autocrlf", "false"], ["add", "-A"], ["commit", "-qm", "base"]):
            subprocess.run(["git", *cmd], cwd=root, check=True, capture_output=True)
    return root


@pytest.fixture
def repo(tmp_path):
    return make_repo(tmp_path / "repo")


@pytest.fixture
def git_repo(tmp_path):
    return make_repo(tmp_path / "repo", git=True)
