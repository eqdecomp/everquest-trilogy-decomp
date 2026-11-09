r"""Check a workspace before the first test run: what is there, what is missing, what to do about it.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\doctor.py [--json]

Reports, without changing anything:
  python     the interpreter's version (3.13 is the tested one) and whether it runs from a virtual environment
  packages   unicorn, capstone, pefile, pytest importable, versions against requirements-lock.txt
  workspace  paths.local.ini found (EQD_ROOT, or the first parent folder holding it)
  binaries   each original image of the repo under [workspace] original, and whether its SHA-256 is the
             supported build (another build makes the fixtures skip)
  vc6        each [toolchain] folder the images need (vc6 = SP4, vc6_sp5 = SP5): bin\cl.exe present, and the
             service pack read from c2.dll's version (12.0.8799 = SP4, 12.0.8966 = SP5)

Exit code 0 when everything the fixtures need is there (warnings allowed), 1 otherwise. Nothing here needs
the binaries or VC6 to run: it only looks. Loads eqd_unicorn/clients.py by path, so it works before the
packages are installed.
"""
from __future__ import annotations

import argparse
import configparser
import hashlib
import importlib.util
import json
import os
import re
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
DEFAULT_VC6 = r"C:\Program Files (x86)\Microsoft Visual Studio\VC98"
PACKAGES = ("unicorn", "capstone", "pefile", "pytest")
# The supported original images (the decomp repo's reference/VERSIONS.md and tests/unicorn/conftest.py).
SUPPORTED_SHA256 = {
    "eqgame.exe": "ee124a75d46aa61197e9160598c567237573bd656a32865637765165274b3cfa",
    "eqmain.dll": "8048b5d3328aca87e2bc13b4a39b6d2bd0991d5d9b7b76abcd06b261f42a6daa",
    "eqgfx_dx7.dll": "19c904bcfaa6a03df7af5fab3c644935265f792ed49096b45861d690beab8b96",
}
C2_BUILDS = {8168: "RTM", 8799: "SP4", 8966: "SP5"}
NEEDED_BUILD = {"vc6": 8799, "vc6_sp5": 8966}

OK, WARN, MISSING = "ok", "warn", "missing"


def load_clients():
    spec = importlib.util.spec_from_file_location("eqd_clients_doctor", TOOLS / "unicorn" / "eqd_unicorn" / "clients.py")
    mod = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = mod                    # dataclasses look the module up while it loads
    spec.loader.exec_module(mod)
    return mod


def find_root(start: Path | None = None) -> Path | None:
    env = os.environ.get("EQD_ROOT")
    if env:
        return Path(env)
    for s in [p.resolve() for p in (start, Path.cwd(), TOOLS) if p]:
        for d in (s, *s.parents):
            if (d / "paths.local.ini").is_file():
                return d
    return None


def lock_pins(path: Path) -> dict:
    pins = {}
    if path.is_file():
        for line in path.read_text(encoding="utf-8").splitlines():
            m = re.match(r"\s*([A-Za-z0-9_.-]+)==([^\s;#]+)", line)
            if m:
                pins[m.group(1).lower()] = m.group(2)
    return pins


def c2_build(vc98: Path) -> int | None:
    """The build number of the VC6 back end (bin\\c2.dll's file version 12.0.<build>), None if unreadable."""
    c2 = vc98 / "Bin" / "c2.dll"
    if not c2.is_file():
        c2 = vc98 / "bin" / "c2.dll"
    if not c2.is_file():
        return None
    data = c2.read_bytes()
    i = data.find(b"\xbd\x04\xef\xfe")              # VS_FIXEDFILEINFO signature
    if i < 0:
        return None
    ms = int.from_bytes(data[i + 8:i + 12], "little")
    ls = int.from_bytes(data[i + 12:i + 16], "little")
    return (ls >> 16) if (ms >> 16) == 12 else None


def check(start: Path | None = None, repo_name: str = "eq-trilogy") -> list[dict]:
    res = []

    def add(area, item, status, detail, fix=""):
        res.append({"area": area, "item": item, "status": status, "detail": detail, "fix": fix})

    v = sys.version_info
    add("python", "version", OK if v[:2] == (3, 13) else WARN, sys.version.split()[0],
        "" if v[:2] == (3, 13) else "the tools are tested with Python 3.13")
    venv = sys.prefix != getattr(sys, "base_prefix", sys.prefix)
    add("python", "virtual environment", OK if venv else WARN, sys.prefix,
        "" if venv else r"py -3.13 -m venv .venv  (in the workspace), then use .venv\Scripts\python.exe")

    pins = lock_pins(TOOLS / "requirements-lock.txt")
    from importlib import metadata
    for pkg in PACKAGES:
        try:
            ver = metadata.version(pkg)
        except metadata.PackageNotFoundError:
            add("packages", pkg, MISSING, "not installed",
                r"pip install -r tools\requirements-lock.txt")
            continue
        want = pins.get(pkg)
        add("packages", pkg, OK if want in (None, ver) else WARN, ver,
            "" if want in (None, ver) else f"tested with {want}")

    root = find_root(start)
    if root is None:
        add("workspace", "paths.local.ini", MISSING, "not found above the current folder and EQD_ROOT is unset",
            r"copy tools\paths.example.ini ..\paths.local.ini  (into the folder that holds the repo)")
        return res
    add("workspace", "paths.local.ini", OK, str(root / "paths.local.ini"))
    cfg = configparser.ConfigParser()
    cfg.read(root / "paths.local.ini")
    ws = cfg["workspace"] if cfg.has_section("workspace") else {}
    original = root / ws.get("original", "original")

    clients = load_clients()
    images = [c for c in clients.of_repo(repo_name) if c.binary]
    for c in images:
        p = original / c.binary
        if not p.is_file():
            add("binaries", c.binary, MISSING, f"{p} not found",
                f"copy your own {Path(c.binary).name} into {p.parent}")
            continue
        got = hashlib.sha256(p.read_bytes()).hexdigest()
        want = SUPPORTED_SHA256.get(Path(c.binary).name.lower())
        if want and got != want:
            add("binaries", c.binary, WARN, f"SHA-256 {got[:16]}... is not the supported build",
                "fixtures skip with another build (EQD_ANY_BINARY=1 runs them anyway)")
        else:
            add("binaries", c.binary, OK, str(p))

    keys = sorted({c.toolchain for c in images} | {k for c in images for _, k in c.toolchain_dirs})
    for key in keys:
        if cfg.has_option("toolchain", key):
            vc98, how = Path(cfg["toolchain"][key]), f"[toolchain] {key}"
        elif cfg.has_option("toolchain", "vc6"):
            vc98, how = Path(cfg["toolchain"]["vc6"]), f"[toolchain] vc6 (no {key} entry)"
        else:
            vc98, how = Path(DEFAULT_VC6), "default folder"
        users = ", ".join(c.id for c in images if c.toolchain == key) or "per-folder sources"
        if not (vc98 / "Bin" / "cl.exe").is_file() and not (vc98 / "bin" / "cl.exe").is_file():
            add("vc6", key, MISSING, f"{vc98} ({how}): no Bin\\cl.exe; used by {users}",
                f"install Visual C++ 6.0 and set [toolchain] {key} in paths.local.ini to its VC98 folder")
            continue
        build = c2_build(vc98)
        need = NEEDED_BUILD.get(key)
        name = C2_BUILDS.get(build, f"build {build}") if build else "unknown service pack"
        if need and build != need:
            add("vc6", key, WARN, f"{vc98}: c2.dll is {name}; used by {users}",
                f"fixtures run, but byte-matching needs {C2_BUILDS[need]} (c2 12.0.{need})")
        else:
            add("vc6", key, OK, f"{vc98}: {name}; used by {users}")
    return res


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--json", action="store_true", help="print the results as JSON")
    ap.add_argument("--repo-name", default="eq-trilogy", help="the decomp repo whose images to check")
    a = ap.parse_args(argv)
    res = check(repo_name=a.repo_name)
    if a.json:
        print(json.dumps(res, indent=1))
    else:
        for r in res:
            print(f"{r['status'].upper():8} {r['area']:10} {r['item']:22} {r['detail']}")
            if r["fix"]:
                print(f"{'':8} {'':10} {'-> ' + r['fix']}")
    bad = [r for r in res if r["status"] == MISSING]
    if not a.json:
        print("\nready for the fixtures" if not bad else f"\n{len(bad)} missing: see the -> lines")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
