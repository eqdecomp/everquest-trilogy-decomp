"""Unicorn differential tests for eqgame.exe and eqmain.dll (see tools/unicorn/README.md, tools/docs/EQMAIN.md).

Run from the repo root:  ..\\.venv\\Scripts\\python.exe -m pytest tests/unicorn
Needs the workspace (paths.local.ini, original\\trilogy\\eqgame.exe) and VC6; skips otherwise.
Each fixture runs against its own image: tests/unicorn/eqmain/... against eqmain.dll (client `eqmain`, VC6 SP5,
crt_defaults_eqmain.json), everything else against eqgame.exe (client `trilogy`, VC6 SP4, crt_defaults.json).
Test DLLs are built into <workspace>\\build\\<client>\\unicorn\\ (never committed).
"""
import csv
import faulthandler
import hashlib
import json
import os
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
# Merged under every fixture: the client's CRT callees (passthrough) and the test DLL's CRT aliases.
CRT_DEFAULTS = HERE / "crt_defaults.json"
PRIMARY = "trilogy"
# EQD_TOOLS lets you run against another copy of tools/ instead of this one.
TOOLS = Path(os.environ.get("EQD_TOOLS", REPO / "tools"))
sys.path.insert(0, str(TOOLS / "unicorn"))

from eqd_unicorn import Harness, Workspace, load_fixture  # noqa: E402

try:                                    # the image registry (tools/unicorn/eqd_unicorn/clients.py)
    from eqd_unicorn import clients as _clients  # noqa: E402
except ImportError:                     # older tools: this fallback table
    _clients = None
_FALLBACK = {"trilogy": ("eqgame", "trilogy/eqgame.exe", "vc6", "", "crt_defaults.json"),
             "eqmain": ("eqmain", "trilogy/eqmain.dll", "vc6_sp5", "eqmain", "crt_defaults_eqmain.json")}


def _info(client):
    """(names stem, binary under original/, toolchain key, fixture sub-folder, defaults file name)."""
    if _clients is not None and client in _clients.CLIENTS:
        c = _clients.get(client)
        return c.names, c.binary, c.toolchain, c.prefix, Path(c.crt_defaults).name
    return _FALLBACK[client]


def clients():
    """The images this repo tests, the primary (trilogy) first."""
    if _clients is not None:
        return [c.id for c in _clients.of_repo("eq-trilogy")]
    return list(_FALLBACK)


def client_of(path) -> str:
    """The image a fixture (or source) belongs to, by its folder: tests/unicorn/eqmain/... -> eqmain."""
    p = Path(path).resolve()
    for root in (HERE, REPO / "src"):
        try:
            rel = p.relative_to(root).parts
            break
        except ValueError:
            continue
    else:
        return PRIMARY
    if len(rel) < 2:
        return PRIMARY
    for c in clients():
        prefix = _info(c)[3]
        if prefix and rel[0].lower() == prefix.lower():
            return c
    return PRIMARY


@pytest.hookimpl(trylast=True)
def pytest_configure(config):
    # Unicorn's first mem_map raises (and handles) an access violation inside QEMU on Windows; pytest's
    # faulthandler prints a scary "Windows fatal exception" stack for every Uc() otherwise.
    faulthandler.disable()


def function_starts(ws=None, client=PRIMARY):
    """Known function entries = tail-call boundaries for the original's extent: every function in the
    Ghidra export (<build>/export/<client>/functions.json, if present) plus every entry in
    names/<stem>.*.csv (eqgame.*.csv; eqmain.*.csv for eqmain)."""
    starts = set()
    if ws is not None:
        exp = ws.build / "export" / client / "functions.json"
        if exp.is_file():
            starts.update(int(f["address"], 16) for f in json.loads(exp.read_text(encoding="utf-8"))["functions"])
    for f in (REPO / "names").glob(f"{_info(client)[0]}.*.csv"):
        with open(f, newline="", encoding="utf-8") as fh:
            for row in csv.DictReader(fh):
                try:
                    starts.add(int(row["address"], 16))
                except (KeyError, ValueError):
                    pass
    return starts


def fixture_paths():
    """All fixtures: tests/unicorn/<subsystem>/*.json, where <subsystem> is the queue's subsystem (the Mac
    source path, e.g. EverQuest/Display = src/EverQuest/Display.cpp). Not the negative/ folders, and not the
    files directly in tests/unicorn/ (crt_defaults*.json). eqmain's fixtures are under tests/unicorn/eqmain/."""
    return sorted(p for p in HERE.rglob("*.json") if "negative" not in p.parts and p.parent != HERE)


def crt_defaults(client=PRIMARY):
    """The image's fixture defaults: crt_defaults.json (eqgame.exe), crt_defaults_eqmain.json (eqmain.dll)."""
    p = CRT_DEFAULTS if client == PRIMARY else HERE / _info(client)[4]
    return json.loads(p.read_text(encoding="utf-8")) if p.is_file() else {}


_HARNESS = {}

# The supported original images (reference/VERSIONS.md). Another build gives misleading failures, so a mismatch
# skips; EQD_ANY_BINARY=1 runs anyway.
SHA256 = {"eqgame.exe": "ee124a75d46aa61197e9160598c567237573bd656a32865637765165274b3cfa",
          "eqmain.dll": "8048b5d3328aca87e2bc13b4a39b6d2bd0991d5d9b7b76abcd06b261f42a6daa",
          "eqgfx_dx7.dll": "19c904bcfaa6a03df7af5fab3c644935265f792ed49096b45861d690beab8b96"}


def _check_binary(exe: Path):
    want = SHA256.get(exe.name.lower())
    if want is None or os.environ.get("EQD_ANY_BINARY") == "1":
        return
    got = hashlib.sha256(exe.read_bytes()).hexdigest()
    if got != want:
        pytest.skip(f"{exe}: SHA-256 {got} is not the supported build ({want}, reference/VERSIONS.md); "
                    "set EQD_ANY_BINARY=1 to run anyway")


def _vc6(ws, client):
    try:
        return ws.vc6(client)
    except TypeError:                   # older tools: one toolchain in Workspace
        key = _info(client)[2]
        return Path(ws.cfg["toolchain"][key]) if ws.cfg.has_option("toolchain", key) else ws.vc6()


def make_harness(client=PRIMARY):
    """The session's Harness for one image (created once; skips the test when the workspace, the binary or
    VC6 is missing)."""
    if client in _HARNESS:
        return _HARNESS[client]
    try:
        ws = Workspace(REPO)
    except FileNotFoundError as e:
        pytest.skip(str(e))
    exe = ws.original / _info(client)[1]
    if not exe.is_file():
        pytest.skip(f"{exe} not found")
    _check_binary(exe)
    vc6 = _vc6(ws, client)
    if not (vc6 / "Bin" / "CL.EXE").is_file():
        pytest.skip(f"VC6 not found at {vc6}")
    _HARNESS[client] = Harness(exe, ws.unicorn_build_dir(client), include_dirs=[REPO / "include"],
                               client=client.upper(), function_starts=function_starts(ws, client), vc6=vc6,
                               fixture_defaults=crt_defaults(client))
    return _HARNESS[client]


def link_sources(client=PRIMARY):
    """Linked into every test DLL besides the fixture's sources (the defaults' "link"): the image's own
    versions of compiler helpers, e.g. eqgame.exe's __ftol."""
    return [REPO / p for p in crt_defaults(client).get("link", [])]


def run_fixture_with(harness, path, sources=None, name=None, rewrite_symbol=None, defines=(), client=None,
                     **overrides):
    """`harness`: the trilogy session harness; a fixture of another image (tests/unicorn/eqmain/...) runs on
    that image's own harness instead (or pass `client`)."""
    client = client or client_of(path)
    if client != PRIMARY or harness is None:
        harness = make_harness(client)
    fx = load_fixture(path)
    fx.update(overrides)
    srcs = [Path(s) if Path(s).is_absolute() else REPO / s for s in (sources or fx["sources"])]
    srcs += [s for s in link_sources(client) if s not in srcs]
    t = harness.difftest(fx, srcs, name=name, rewrite_symbol=rewrite_symbol, base_dir=REPO, defines=defines)
    return t.run()


@pytest.fixture(scope="session")
def harness():
    return make_harness()


@pytest.fixture(scope="session")
def run_fixture(harness):
    """run_fixture(path, sources=None, name=None, rewrite_symbol=None, defines=(), **overrides) -> Report.

    Builds the fixture's `sources` (or the given ones) into a test DLL and runs the differential test.
    """
    def run(path, *a, **kw):
        return run_fixture_with(harness, path, *a, **kw)
    return run


# --- Subdirectory runs ------------------------------------------------------------------------------------
# test_fixtures.py (directly in tests/unicorn/) parametrizes over every fixture, so `pytest tests/unicorn` and
# `-k <Function>` find them all. A run of a subdirectory or a single fixture file (`pytest
# tests/unicorn/EverQuest/EQMisc`, `.../EQMisc/X.json`) never collects test_fixtures.py, so there the JSON
# fixtures are collected directly, as items with the same name (test_fixture[<path>]). Never both.

def fixture_id(path) -> str:
    return str(Path(path).resolve().relative_to(HERE)).replace("\\", "/")


def _is_fixture(path: Path) -> bool:
    try:
        rel = path.resolve().relative_to(HERE)
    except ValueError:
        return False
    return path.suffix.lower() == ".json" and len(rel.parts) > 1 and "negative" not in rel.parts


def _suite_collected(config) -> bool:
    """True when the session's args include test_fixtures.py (tests/unicorn/ or a folder above it)."""
    suite = HERE / "test_fixtures.py"
    for a in config.args:
        p = Path(config.invocation_params.dir, a.split("::", 1)[0]).resolve()
        if p == suite or suite.is_relative_to(p):
            return True
    return False


class FixtureItem(pytest.Item):
    def runtest(self):
        report = run_fixture_with(None, self.path)
        assert report.ok, report.summary()

    def reportinfo(self):
        return self.path, None, self.name


class FixtureFile(pytest.File):
    def collect(self):
        yield FixtureItem.from_parent(self, name=f"test_fixture[{fixture_id(self.path)}]")


def pytest_collect_file(file_path, parent):
    if _is_fixture(file_path) and not _suite_collected(parent.config):
        return FixtureFile.from_parent(parent, path=file_path)
    return None
