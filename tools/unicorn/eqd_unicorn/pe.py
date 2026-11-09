"""PE images for the harness: map a PE at its VA (or relocated to a chosen base), bind imports, read VC6 maps."""
from __future__ import annotations

import re
from dataclasses import dataclass, field
from pathlib import Path

import pefile

PAGE = 0x1000


def page_up(n: int) -> int:
    return (n + PAGE - 1) & ~(PAGE - 1)


@dataclass
class Image:
    """A flat, page-aligned memory image to map into the emulator."""
    name: str
    base: int
    data: bytearray
    preferred_base: int = 0
    # import slot VA -> "dll!name" (dll lower-case, name or "#ordinal")
    imports: dict = field(default_factory=dict)
    # VA -> symbol name, and the reverse (from a linker map, or given by hand)
    symbols: dict = field(default_factory=dict)
    by_name: dict = field(default_factory=dict)
    # [(va, size)] of code sections, for disassembly bounds
    code_ranges: list = field(default_factory=list)

    @property
    def end(self) -> int:
        return self.base + len(self.data)

    def contains(self, va: int) -> bool:
        return self.base <= va < self.end

    def read(self, va: int, n: int) -> bytes:
        o = va - self.base
        return bytes(self.data[o:o + n])

    def add_symbol(self, va: int, name: str):
        self.symbols.setdefault(va, name)
        self.by_name.setdefault(name, va)

    @classmethod
    def from_bytes(cls, name: str, base: int, code: bytes, size: int | None = None) -> "Image":
        """A raw blob (for self-tests): one RWX region at `base`."""
        data = bytearray(page_up(max(size or 0, len(code), 1)))
        data[:len(code)] = code
        return cls(name, base, data, base, code_ranges=[(base, len(code))])


def load_pe(path, base: int | None = None, name: str | None = None) -> Image:
    """Map a PE file. With `base` different from its preferred base, apply its base relocations."""
    path = Path(path)
    pe = pefile.PE(str(path), fast_load=False)
    preferred = pe.OPTIONAL_HEADER.ImageBase
    import_rvas = {}                                         # read before any relocation touches pe
    for entry in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
        dll = entry.dll.decode("ascii", "replace").lower()
        for imp in entry.imports:
            nm = imp.name.decode("ascii", "replace") if imp.name else f"#{imp.ordinal}"
            import_rvas[imp.address - preferred] = f"{dll}!{nm}"
    if base is None or base == preferred:
        base = preferred
        raw = pe.get_memory_mapped_image()
    else:
        if not hasattr(pe, "DIRECTORY_ENTRY_BASERELOC"):
            raise ValueError(f"{path.name} has no relocations; cannot load it at {base:#x}")
        raw = pe.get_memory_mapped_image(ImageBase=base)     # applies the base relocations
    size = page_up(pe.OPTIONAL_HEADER.SizeOfImage)
    data = bytearray(size)
    data[:min(len(raw), size)] = raw[:size]
    img = Image(name or path.name, base, data, preferred)
    img.imports = {base + rva: nm for rva, nm in import_rvas.items()}
    for s in pe.sections:
        if s.Characteristics & 0x20000000:          # IMAGE_SCN_MEM_EXECUTE
            img.code_ranges.append((base + s.VirtualAddress, s.Misc_VirtualSize))
    pe.close()
    return img


# --- VC6 linker maps -------------------------------------------------------------------------------------

_MAP_LINE = re.compile(r"^\s*[0-9a-fA-F]{4}:[0-9a-fA-F]{8}\s+(\S+)\s+([0-9a-fA-F]{8})\b")
_PREF = re.compile(r"Preferred load address is ([0-9a-fA-F]+)")


def read_map(map_path, img: Image):
    """Add the public and static symbols of a VC6 /MAP file to `img` (relocated to img.base)."""
    text = Path(map_path).read_text(errors="replace")
    m = _PREF.search(text)
    pref = int(m.group(1), 16) if m else img.preferred_base
    for line in text.splitlines():
        m = _MAP_LINE.match(line)
        if not m:
            continue
        va = int(m.group(2), 16)
        if va < pref:                       # absolute symbols, section 0000
            continue
        img.add_symbol(va - pref + img.base, m.group(1))


def undecorate(sym: str) -> str:
    """Rough MSVC undecoration, enough to match `Class::Method` / `function` against source annotations.

    ?Name@Class@NS@@... -> NS::Class::Name;  ??0Class@@ -> Class::Class;  ??1Class@@ -> Class::~Class;
    _name / _name@8 / @name@8 -> name.  Anything else is returned unchanged.
    """
    if sym.startswith("??0") or sym.startswith("??1"):
        parts = sym[3:].split("@@", 1)[0].split("@")
        cls = "::".join(reversed(parts))
        return f"{cls}::{'~' if sym[2] == '1' else ''}{parts[0]}"
    if sym.startswith("?") and not sym.startswith("??"):
        parts = sym[1:].split("@@", 1)[0].split("@")
        return "::".join(reversed(parts))
    if sym.startswith("_") or sym.startswith("@"):
        return re.sub(r"@\d+$", "", sym[1:])
    return sym
