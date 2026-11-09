"""compare/probe.py: snippet listings, A/B comparison and the docs/vc6_quirks claims (needs the workspace's VC6;
skipped without it)."""
import sys
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "unicorn"))
sys.path.insert(0, str(ROOT / "compare"))

import probe  # noqa: E402


def _vc6():
    cl = probe.find_vc6() / "Bin" / "CL.EXE"
    return cl.is_file()


needs_vc6 = pytest.mark.skipif(not _vc6(), reason="needs the workspace's VC6")


def test_undecorated():
    assert probe.undecorated("_f") == "f"
    assert probe.undecorated("_g@8") == "g"
    assert probe.undecorated("?m@C@@QAEHH@Z") == "C::m"
    assert probe.undecorated("??0R@@QAE@H@Z") == "R::R"
    assert probe.undecorated("??1D@@QAE@XZ") == "D::~D"


def test_parse_quirk(tmp_path):
    p = tmp_path / "q.c"
    p.write_text("// QUIRK: vc6q-x\n// KEYWORDS: switch, jump table\n// VARIANTS: A B\n// FLAGS@B: /O2\n"
                 "// EXPECT@A: f has ^ret$\n// EXPECT: f differs A B\n// CFLAGS: /O1\nint f(void){return 0;}\n")
    q = probe.parse_quirk(p)
    assert q.id == "vc6q-x" and q.keywords == ["switch", "jump table"] and q.variants == ["A", "B"]
    assert q.var_flags == {"B": ["/O2"]} and q.flags == []
    assert q.expects == [("A", "f", "has", "^ret$"), (None, "f", "differs", "A B")]


@needs_vc6
def test_listing_switch_table_and_relocs(tmp_path):
    src = tmp_path / "s.c"
    src.write_text("extern int g(int); extern int gx;\nint f(int x) { switch (x) {\n"
                   "case 0: return g(1); case 1: return g(7); case 2: return gx; case 3: return g(3);\n"
                   "default: return -1; } }\n")
    ls = probe.probe(src, "f", tmp_path, ["/O2"])
    text = ls.text()
    assert "jmp dword ptr [eax*4 + @" in text            # table address keyed to the table's own line
    assert "call _g" in text and "dword ptr [_gx]" in text
    assert text.count("dd @") == 4
    assert ls.lines[-1].startswith("dd @")                  # no trailing padding


@needs_vc6
def test_cli_compare_two_variants(tmp_path, capsys):
    src = tmp_path / "v.c"
    src.write_text("int f(int a) {\n#ifdef A\n return a % 8;\n#else\n return a & 7;\n#endif\n}\n")
    assert probe.main([str(src), "-f", "f", "-V", "A", "-V", "B", "--out", str(tmp_path / "o")]) == 1
    assert "differs" in capsys.readouterr().out
    assert probe.main([str(src), "-f", "f", "-V", "A", "-V", "A", "--out", str(tmp_path / "o")]) == 0


@needs_vc6
def test_quirk_checker_fails_on_a_false_claim(tmp_path):
    """Negative control: a planted wrong claim must fail."""
    d = tmp_path / "q"
    d.mkdir()
    (d / "bad.c").write_text("// QUIRK: bad\n// VARIANTS: A B\n// EXPECT: f has idiv\n// EXPECT: f differs A B\n"
                             "unsigned f(unsigned a) { return a % 8; }\n")
    lines = []
    assert probe.check_quirks(d, tmp_path / "o", log=lines.append) == 1
    assert lines[0].startswith("FAIL") and "has /idiv/" in "\n".join(lines) and "differs A B" in "\n".join(lines)


@needs_vc6
@pytest.mark.parametrize("path", probe.quirk_files(), ids=lambda p: p.stem)
def test_vc6_quirk_holds(path, tmp_path):
    q = probe.parse_quirk(path)
    assert probe.check_quirk(q, tmp_path) == []
