"""Find the workspace (binaries, build dir, VC6) from paths.local.ini, like the Ghidra scripts do."""
from __future__ import annotations

import configparser
import os
from pathlib import Path

from . import clients as _clients
from .vc6 import DEFAULT_VC6

# client -> binary relative to [workspace] original (clients.py is the source; kept for old callers)
BINARIES = {c.id: c.binary for c in _clients.CLIENTS.values() if c.binary}


def find_root(start=None) -> Path | None:
    """EQD_ROOT, or the first parent (of `start`, the cwd, or this file) holding paths.local.ini."""
    env = os.environ.get("EQD_ROOT")
    if env:
        return Path(env)
    for s in [Path(p).resolve() for p in (start, Path.cwd(), __file__) if p]:
        for d in (s, *s.parents):
            if (d / "paths.local.ini").is_file():
                return d
    return None


class Workspace:
    def __init__(self, start=None):
        self.root = find_root(start)
        if self.root is None:
            raise FileNotFoundError("paths.local.ini not found; set EQD_ROOT")
        self.cfg = configparser.ConfigParser()
        self.cfg.read(self.root / "paths.local.ini")
        ws = self.cfg["workspace"] if self.cfg.has_section("workspace") else {}
        self.original = self.root / ws.get("original", "original")
        self.build = self.root / ws.get("build", "build")

    def binary(self, client: str) -> Path:
        b = _clients.get(client).binary or BINARIES.get(client)
        if not b:
            raise KeyError(f"no binary registered for client {client!r} (eqd_unicorn/clients.py)")
        return self.original / b

    def vc6(self, client: str | None = None) -> Path:
        """The VC6 toolchain: the client's own ([toolchain] <clients.py toolchain>, e.g. vc6_sp5 for eqmain),
        else [toolchain] vc6, else the installed default."""
        key = _clients.get(client).toolchain if client else "vc6"
        for k in (key, "vc6"):
            if self.cfg.has_option("toolchain", k):
                return Path(self.cfg["toolchain"][k])
        return Path(DEFAULT_VC6)

    def repo(self, client: str) -> Path:
        """The client's decomp repo (EQD_REPO overrides it, e.g. for a worktree)."""
        return _clients.repo_dir(client, self.root)

    def unicorn_build_dir(self, client: str) -> Path:
        """<build>/<client>/unicorn, or EQD_UNICORN_BUILD (lets parallel runs use separate dirs)."""
        env = os.environ.get("EQD_UNICORN_BUILD")
        return Path(env) if env else self.build / client / "unicorn"
