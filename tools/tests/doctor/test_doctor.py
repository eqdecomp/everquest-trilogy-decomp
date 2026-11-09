"""doctor.py on a synthetic workspace (no game files, no VC6): what it reports as ok, warn and missing."""
import hashlib
import struct
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(TOOLS))
import doctor  # noqa: E402


def fake_c2(path: Path, build: int):
    """A file with a VS_FIXEDFILEINFO block saying version 12.0.<build>.0, which is all doctor reads."""
    path.parent.mkdir(parents=True, exist_ok=True)
    info = b"\xbd\x04\xef\xfe" + struct.pack("<IIII", 0x00010000, 12 << 16, build << 16, 0)
    path.write_bytes(b"MZ" + b"\0" * 62 + info)


def workspace(tmp_path, ini):
    (tmp_path / "paths.local.ini").write_text(ini, encoding="utf-8")
    return tmp_path


def by_item(res):
    return {(r["area"], r["item"]): r for r in res}


def test_empty_workspace_reports_missing_binaries_and_vc6(tmp_path, monkeypatch):
    ws = workspace(tmp_path, f"[workspace]\noriginal = original\n[toolchain]\nvc6 = {tmp_path / 'novc'}\n")
    monkeypatch.setenv("EQD_ROOT", str(ws))
    r = by_item(doctor.check())
    assert r[("workspace", "paths.local.ini")]["status"] == "ok"
    for b in ("trilogy/eqgame.exe", "trilogy/eqmain.dll", "trilogy/eqgfx_dx7.dll"):
        assert r[("binaries", b)]["status"] == "missing"
    assert r[("vc6", "vc6")]["status"] == "missing"
    assert r[("vc6", "vc6_sp5")]["status"] == "missing"     # falls back to [toolchain] vc6, which is missing
    assert doctor.main([]) == 1


def test_wrong_build_and_service_pack_are_warnings(tmp_path, monkeypatch):
    sp4, sp5 = tmp_path / "sp4" / "VC98", tmp_path / "sp5" / "VC98"
    for vc98, build in ((sp4, 8799), (sp5, 8168)):
        (vc98 / "Bin").mkdir(parents=True)
        (vc98 / "Bin" / "cl.exe").write_bytes(b"")
        fake_c2(vc98 / "Bin" / "c2.dll", build)
    ws = workspace(tmp_path, f"[workspace]\noriginal = orig\n[toolchain]\nvc6 = {sp4}\nvc6_sp5 = {sp5}\n")
    (ws / "orig" / "trilogy").mkdir(parents=True)
    (ws / "orig" / "trilogy" / "eqgame.exe").write_bytes(b"not the game")
    monkeypatch.setenv("EQD_ROOT", str(ws))
    r = by_item(doctor.check())
    assert r[("binaries", "trilogy/eqgame.exe")]["status"] == "warn"
    assert r[("vc6", "vc6")]["status"] == "ok" and "SP4" in r[("vc6", "vc6")]["detail"]
    assert r[("vc6", "vc6_sp5")]["status"] == "warn" and "RTM" in r[("vc6", "vc6_sp5")]["detail"]


def test_supported_binary_is_ok(tmp_path, monkeypatch):
    data = b"pretend eqmain"
    monkeypatch.setitem(doctor.SUPPORTED_SHA256, "eqmain.dll", hashlib.sha256(data).hexdigest())
    ws = workspace(tmp_path, "[workspace]\noriginal = original\n")
    (ws / "original" / "trilogy").mkdir(parents=True)
    (ws / "original" / "trilogy" / "eqmain.dll").write_bytes(data)
    monkeypatch.setenv("EQD_ROOT", str(ws))
    assert by_item(doctor.check())[("binaries", "trilogy/eqmain.dll")]["status"] == "ok"


def test_lock_pins_parse():
    pins = doctor.lock_pins(TOOLS / "requirements-lock.txt")
    assert {"unicorn", "capstone", "pefile", "pytest"} <= set(pins)
