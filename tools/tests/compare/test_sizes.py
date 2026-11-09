r"""compare.py's `size` escape hatch: the fixtures' `size` and --size reach compare_function's extent."""
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "compare"))
import compare  # noqa: E402


def _fixture(path: Path, **kw):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(kw), encoding="utf-8")


def test_fixture_sizes_by_client(tmp_path):
    t = tmp_path / "tests" / "unicorn"
    _fixture(t / "Sub" / "a.json", original="0x00438AD4", size="0x1000")
    _fixture(t / "Sub" / "b.json", original=0x401000)                       # no size
    _fixture(t / "negative" / "c.json", original="0x00402000", size=8)
    _fixture(t / "eqmain" / "d.json", original="0x00403000", size=16)
    _fixture(t / "crt_defaults.json", size=4)
    assert compare.fixture_sizes(tmp_path, "trilogy") == {0x438AD4: 0x1000}
    assert compare.fixture_sizes(tmp_path, "eqmain") == {0x403000: 16}
    compare.EXTRA_SIZES[0x500000] = 0x20
    try:
        assert compare.fixture_sizes(tmp_path, "trilogy") == {0x438AD4: 0x1000, 0x500000: 0x20}
    finally:
        compare.EXTRA_SIZES.clear()
    assert compare.fixture_sizes(tmp_path / "nowhere") == {}
