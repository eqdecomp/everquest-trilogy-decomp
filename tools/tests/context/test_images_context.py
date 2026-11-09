r"""Bundles for a second image (eqmain.dll): shared headers, the sibling example, the image note."""
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[2]
for _p in (TOOLS / "context", TOOLS / "unicorn"):
    sys.path.insert(0, str(_p))
import pack_context as pc  # noqa: E402
import similar  # noqa: E402


def test_headers_of_a_secondary_image_include_the_primary_ones(tmp_path):
    (tmp_path / "include" / "trilogy").mkdir(parents=True)
    (tmp_path / "include" / "eqmain").mkdir(parents=True)
    (tmp_path / "include" / "trilogy" / "rdp.h").write_text("struct RdpHeap_guess {\n    int count; // 0x0\n};\n")
    (tmp_path / "include" / "eqmain" / "login.h").write_text("struct Login_guess {\n    int state; // 0x0\n};\n")
    h = pc.Headers(tmp_path, "eqmain")
    assert {"RdpHeap_guess", "Login_guess"} <= set(h.defs)
    assert "Login_guess" not in pc.Headers(tmp_path, "trilogy").defs


def test_sibling_pairs_and_markdown(tmp_path):
    p = tmp_path / "names" / "siblings"
    p.mkdir(parents=True)
    (p / "eqmain_trilogy.csv").write_text(
        "address,sibling_address,sibling_name,sibling_subsystem,similarity,confidence,algorithm\n"
        "1002e530,004f4fe0,Heap_getTopHeapElement_guess,Network/rdp/heap,1.000,0.99,h\n")
    pairs = similar.sibling_pairs(tmp_path, "eqmain", "trilogy")
    assert pairs["1002E530"]["sibling_address"] == "004f4fe0"
    md = similar.markdown([{"address": "004F4FE0", "name": "Heap_getTopHeapElement_guess", "similarity": 0.9,
                            "source": "src/Network/rdp/heap.c", "code": "int f() {}", "listing": ["ret"],
                            "image": "trilogy", "sibling": "BinDiff 1.000"}])
    assert "(trilogy, similarity 0.90" in md and "**Sibling: the same original code in trilogy" in md
    assert similar.registered_siblings("eqmain") == ("trilogy",)
    assert similar.registered_siblings("trilogy") == ()           # eqgame bundles are unchanged


def test_compiler_line_and_repo():
    assert "SP5" in pc.compiler_of("eqmain") and "/O2" in pc.compiler_of("eqmain")
    assert pc.compiler_of("trilogy") == pc.COMPILERS["trilogy"]
