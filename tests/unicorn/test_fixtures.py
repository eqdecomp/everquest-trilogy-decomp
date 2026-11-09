"""Every fixture under tests/unicorn/<subsystem>/*.json: the rewrite must match the original."""
import pytest

from conftest import HERE, fixture_paths


@pytest.mark.parametrize("path", fixture_paths(), ids=lambda p: str(p.relative_to(HERE)).replace("\\", "/"))
def test_fixture(run_fixture, path):
    report = run_fixture(path)
    assert report.ok, report.summary()
