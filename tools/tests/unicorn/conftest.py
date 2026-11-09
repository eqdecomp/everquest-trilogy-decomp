"""Harness self-tests never touch the shared test-DLL cache (<workspace>/build/unicorn_cache): each session
gets its own (EQD_UNICORN_CACHE)."""
import faulthandler

import pytest


@pytest.hookimpl(trylast=True)
def pytest_configure(config):
    # Unicorn's first mem_map raises (and handles) an access violation inside QEMU on Windows; pytest's
    # faulthandler would print a "Windows fatal exception" stack for it (as in eq-trilogy's conftest).
    faulthandler.disable()


@pytest.fixture(autouse=True, scope="session")
def _private_dll_cache(tmp_path_factory):
    mp = pytest.MonkeyPatch()
    mp.setenv("EQD_UNICORN_CACHE", str(tmp_path_factory.mktemp("dllcache")))
    yield
    mp.undo()
