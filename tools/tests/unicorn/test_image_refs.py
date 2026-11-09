"""resolve_image_refs: `from_image` fixture values come from the original image, not the fixture."""
import sys
from pathlib import Path

import pytest

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "unicorn"))

from eqd_unicorn import HarnessError, Image, resolve_image_refs  # noqa: E402

IMG = Image.from_bytes("t.exe", 0x400000, bytes.fromhex("0000a0424c414444455200"))


def test_memory_and_callee_writes_resolve():
    fx = {"memory": [{"addr": "0x400004", "from_image": "0x400004", "image_size": 7},
                     {"addr": "0x400000", "f32": 80.0}],
          "callees": {"0x401000": {"writes": [{"arg": 0, "offset": 2, "from_image": "0x400002", "image_size": 2}]}},
          "cases": [{"memory": [{"addr": "@o", "from_image": 0x400004, "image_size": 3}]}]}
    out = resolve_image_refs(fx, IMG)
    assert out["memory"][0] == {"addr": "0x400004", "hex": "4c414444455200"}
    assert out["memory"][1] == {"addr": "0x400000", "f32": 80.0}
    assert out["callees"]["0x401000"]["writes"][0] == {"arg": 0, "offset": 2, "hex": "a042"}
    assert out["cases"][0]["memory"][0]["hex"] == "4c4144"
    assert "from_image" in fx["memory"][0]                  # the input is not modified


def test_string_values_resolve():
    ref = {"from_image": "0x400004", "image_size": 6, "as": "cstr"}
    fx = {"cases": [{"args": [ref, 1], "expect": {"ret": ref}}],
          "fuzz": {"args": [{"choices": [ref, "@o"]}]}}
    out = resolve_image_refs(fx, IMG)
    assert out["cases"][0]["args"] == ["LADDER", 1]
    assert out["cases"][0]["expect"]["ret"] == "LADDER"
    assert out["fuzz"]["args"][0]["choices"] == ["LADDER", "@o"]
    with pytest.raises(HarnessError):
        resolve_image_refs({"memory": [dict(ref, addr="0x400000")]}, IMG)
    with pytest.raises(HarnessError):
        resolve_image_refs({"memory": [{"addr": "0x400000", "from_image": "0x400004", "image_size": 2, "as": "x"}]},
                           IMG)


def test_no_refs_returns_same_object():
    fx = {"memory": [{"addr": "0x400000", "u32": 1}]}
    assert resolve_image_refs(fx, IMG) is fx


@pytest.mark.parametrize("bad", [{"from_image": "0x400000"}, {"from_image": "0x500000", "image_size": 4},
                                 {"from_image": "0x400ffe", "image_size": 8}])
def test_bad_refs_raise(bad):
    with pytest.raises(HarnessError):
        resolve_image_refs({"memory": [dict(bad, addr="0x400000")]}, IMG)
