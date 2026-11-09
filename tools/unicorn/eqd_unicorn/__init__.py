"""eqd_unicorn: differential testing of decomp rewrites against the original binary under Unicorn.

    from eqd_unicorn import Harness, load_fixture
    h = Harness(original_exe, build_dir, include_dirs=[repo / "include"], client="TRILOGY")
    report = h.difftest(load_fixture(path), sources=["src/EverQuest/EverQuest.cpp"], base_dir=repo).run()
    assert report.ok, report.summary()

See ../README.md.
"""
from . import clients
from .difftest import REWRITE_BASE, CaseReport, DiffTest, Harness, Report, apply_defaults, load_fixture, \
    resolve_image_refs
from .machine import Call, Callee, HarnessError, Machine, RunResult, printf_arg_types
from .pe import Image, load_pe, read_map, undecorate
from .vc6 import DEFAULT_CFLAGS, BuildError, build_dll, cflags_for, source_cflags
from .workspace import Workspace, find_root

__all__ = ["Harness", "DiffTest", "Report", "CaseReport", "load_fixture", "apply_defaults", "resolve_image_refs", "REWRITE_BASE",
           "Machine", "Call", "Callee", "RunResult", "HarnessError", "printf_arg_types", "Image", "load_pe",
           "read_map", "undecorate", "build_dll", "BuildError", "DEFAULT_CFLAGS", "cflags_for", "source_cflags",
           "Workspace", "find_root", "clients"]
