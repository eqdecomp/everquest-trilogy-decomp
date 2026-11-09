r"""A decomp repo's Unicorn test setup, for the tools in this folder (make_fixture, mutate_check, the cache's
warm-up): its `tests/unicorn/conftest.py` is loaded as a module, so fixtures are found and run exactly as
pytest runs them (same Harness, fixture defaults, link sources, function starts).

This tools checkout's own `eqd_unicorn` is imported first, so it is the one the conftest uses (whatever
EQD_TOOLS says).
"""
from __future__ import annotations

import importlib.util
import inspect
import json
import os
import sys
from pathlib import Path

UNICORN = Path(__file__).resolve().parents[1]
if str(UNICORN) not in sys.path:
    sys.path.insert(0, str(UNICORN))

import eqd_unicorn  # noqa: E402,F401  (before the conftest: it must use this copy)
from eqd_unicorn import annotations as ann  # noqa: E402
from eqd_unicorn import clients  # noqa: E402
from eqd_unicorn import load_fixture  # noqa: E402
from eqd_unicorn.workspace import find_root  # noqa: E402


class RepoError(RuntimeError):
    pass


def find_repo(repo=None, client="trilogy") -> Path:
    """`repo`, else the cwd (or a parent) holding src/ and tests/unicorn/conftest.py, else the client's repo
    (<workspace>/eq-trilogy for trilogy and eqmain; EQD_REPO)."""
    if repo:
        return Path(repo).resolve()
    for d in (Path.cwd(), *Path.cwd().parents):
        if (d / "tests" / "unicorn" / "conftest.py").is_file() and (d / "src").is_dir():
            return d
    root = find_root()
    cand = clients.repo_dir(client, root) if root else None
    if cand and (cand / "tests" / "unicorn" / "conftest.py").is_file():
        return cand
    raise RepoError("decomp repo not found (run inside it or pass --repo)")


def va_of(v) -> int:
    return int(str(v), 16) if isinstance(v, str) else int(v)


class Repo:
    def __init__(self, repo=None, client="trilogy"):
        self.root = find_repo(repo, client)
        self.client = client
        path = self.root / "tests" / "unicorn" / "conftest.py"
        name = f"eqd_repo_conftest_{abs(hash(str(path)))}"
        spec = importlib.util.spec_from_file_location(name, path)
        self.conf = importlib.util.module_from_spec(spec)
        sys.modules[name] = self.conf
        spec.loader.exec_module(self.conf)
        self._harness = None

    # --- fixtures ---------------------------------------------------------------------------------------
    @property
    def tests_dir(self) -> Path:
        return self.root / "tests" / "unicorn"

    def fixtures(self) -> list[Path]:
        return list(self.conf.fixture_paths())

    def fixtures_for(self, va: int) -> list[Path]:
        out = []
        for p in self.fixtures():
            try:
                if va_of(json.loads(p.read_text(encoding="utf-8"))["original"]) == va:
                    out.append(p)
            except (ValueError, KeyError):
                continue
        return out

    def _conf_call(self, fn, *args):
        """A conftest function, with this repo's client when it takes one (older conftests are eqgame-only)."""
        f = getattr(self.conf, fn)
        try:
            takes = bool(inspect.signature(f).parameters)
        except (TypeError, ValueError):
            takes = False
        if takes:
            return f(*args, self.client)
        if self.client != "trilogy" and not clients.get(self.client).primary:
            raise RepoError(f"{self.root / 'tests/unicorn/conftest.py'}: {fn}() takes no client; it can't run "
                            f"{self.client} fixtures")
        return f(*args)

    def crt_defaults(self) -> dict:
        return self._conf_call("crt_defaults")

    def src_path(self, s) -> Path:
        return Path(s) if Path(s).is_absolute() else self.root / s

    def rel(self, p) -> str:
        p = Path(p).resolve()
        try:
            return p.relative_to(self.root).as_posix()
        except ValueError:
            return str(p)

    def build_sources(self, fx: dict, sources=None) -> list[Path]:
        """The test DLL's sources as conftest.run_fixture_with builds them: the fixture's (or `sources`), then
        the defaults' link sources."""
        srcs = [self.src_path(s) for s in (sources or fx["sources"])]
        srcs += [s for s in self._conf_call("link_sources") if s not in srcs]
        return srcs

    def annotations(self, paths=None) -> list:
        paths = paths or clients.sources(self.root, self.client)
        return ann.scan(paths, self.client.upper())

    # --- running ----------------------------------------------------------------------------------------
    def harness(self):
        if self._harness is None:
            try:
                self._harness = self._conf_call("make_harness")
            except BaseException as e:            # pytest.skip -> Skipped (an OutcomeException)
                if type(e).__name__ in ("Skipped", "SkipTest"):
                    raise RepoError(f"cannot run Unicorn tests here: {e}") from None
                raise
        return self._harness

    def difftest(self, path_or_fx, sources=None, harness=None, **overrides):
        fx = load_fixture(path_or_fx) if not isinstance(path_or_fx, dict) else dict(path_or_fx)
        fx.update(overrides)
        h = harness or self.harness()
        return h.difftest(fx, self.build_sources(fx, sources), base_dir=self.root)

    def run(self, path_or_fx, sources=None, harness=None, stop_on_failure=False, **overrides):
        return self.difftest(path_or_fx, sources, harness, **overrides).run(stop_on_failure=stop_on_failure)


def default_work_dir(tag: str, client: str = "trilogy") -> Path:
    """A private scratch folder: <EQD_UNICORN_BUILD or <build>/<client>/unicorn>/<tag>/<pid>."""
    env = os.environ.get("EQD_UNICORN_BUILD")
    base = Path(env) if env else (find_root() or Path.cwd()) / "build" / client / "unicorn"
    return base / tag / str(os.getpid())
