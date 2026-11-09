"""Original-side code coverage of a fixture run (the coverage gate, docs/PLAN.md 8.1 A2).

The machine records every translation block that starts inside the original's extent (`Machine.run(cover=)`:
a set of (va, size), so a hot loop costs one set insert, never a growing list). Here they are mapped onto the
function's static instructions and basic blocks:
  - instructions: those of the extent (`allowed`) inside an executed translation block;
  - basic blocks: leaders = the entry, direct jump targets, the instruction after a jump or ret, and any
    instruction not contiguous with the previous one of the extent. A call does not end a block (a block whose
    call never returns counts as hit, with fewer instructions hit). A switch case that is fallen into from the
    case above is not a leader (no table decoding here), so it merges with that case's block.
Spans are end-exclusive: "004DE640-004DFDF1" holds the instructions at 004DE640 <= va < 004DFDF1.

The per-fixture record (`<unicorn build>/coverage/<VA>/<fixture stem>.json`, see README "Coverage"):
  {"va", "fixture", "insns": [hit, total], "blocks": [hit, total], "rewrite_insns",
   "uncovered": [["004DE640", "004DFDF1", insns], ...],   # runs of consecutive unhit blocks
   "map": [[start, end, insns, insns hit], ...]}            # every basic block, for unions over fixtures
"""
from __future__ import annotations

import json
from bisect import bisect_left
from pathlib import Path

import capstone

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_ENDS = ("ret", "retf", "iretd", "int3", "hlt", "ud2")
_CACHE: dict = {}
_CACHE_MAX = 32


class Static:
    """The extent's instructions (address order) and basic blocks."""

    def __init__(self, read, allowed):
        addrs = sorted(allowed)
        self.addrs = addrs
        self.sizes = []
        self.block_of = []                  # instruction index -> block index
        self.blocks = []                    # [start, end, first insn index, insn count]
        if not addrs:
            return
        lo, hi = addrs[0], addrs[-1] + 16
        code = read(lo, hi - lo)
        lead = {addrs[0]}
        prev_end = None
        for va in addrs:
            o = va - lo
            ins = next(_MD.disasm_lite(code[o:o + 16], va, 1), None)
            size, m, ops = (ins[1], ins[2], ins[3]) if ins else (1, "", "")
            self.sizes.append(size)
            if va != prev_end:
                lead.add(va)
            prev_end = va + size
            if m.startswith("j") or m.startswith("loop") or m in _ENDS:
                lead.add(va + size)
                if m not in _ENDS and ops.startswith("0x"):     # direct jump / jcc / loop target
                    try:
                        lead.add(int(ops, 16))
                    except ValueError:
                        pass
        for i, va in enumerate(addrs):
            if va in lead or not self.blocks:
                self.blocks.append([va, va, i, 0])
            b = self.blocks[-1]
            b[1] = va + self.sizes[i]
            b[3] += 1
            self.block_of.append(len(self.blocks) - 1)


def static_of(image, allowed) -> Static:
    """Cached per (image, extent): one VA's fixtures in a pytest session share it."""
    key = (image.name, len(allowed), min(allowed), max(allowed)) if allowed else (image.name, 0, 0, 0)
    s = _CACHE.get(key)
    if s is None:
        if len(_CACHE) >= _CACHE_MAX:
            _CACHE.pop(next(iter(_CACHE)))
        s = _CACHE[key] = Static(image.read, allowed)
    return s


def measure(static: Static, hits) -> list:
    """Executed translation blocks -> the block map [[start, end, insns, insns hit], ...]."""
    addrs = static.addrs
    hit = bytearray(len(addrs))
    n = len(addrs)
    for va, size in hits:
        i = bisect_left(addrs, va)
        end = va + size
        while i < n and addrs[i] < end:
            hit[i] = 1
            i += 1
    out = []
    for start, end, first, cnt in static.blocks:
        out.append([start, end, cnt, sum(hit[first:first + cnt])])
    return out


def spans(blocks) -> list:
    """Runs of consecutive unhit blocks (address order) -> [[start, end, insns], ...]."""
    out = []
    prev_hit = True
    for start, end, cnt, h in blocks:
        if h:
            prev_hit = True
            continue
        if not prev_hit:
            out[-1][1] = end
            out[-1][2] += cnt
        else:
            out.append([start, end, cnt])
        prev_hit = False
    return out


def record(va: int, fixture: str, blocks: list, rewrite_insns: int) -> dict:
    ih = sum(b[3] for b in blocks)
    it = sum(b[2] for b in blocks)
    bh = sum(1 for b in blocks if b[3])
    return {"va": f"0x{va:08X}", "fixture": fixture, "insns": [ih, it], "blocks": [bh, len(blocks)],
            "rewrite_insns": rewrite_insns,
            "uncovered": [[f"{a:08X}", f"{b:08X}", n] for a, b, n in spans(blocks)],
            "map": blocks}


def write(root, rec: dict, stem: str) -> Path | None:
    d = Path(root) / rec["va"][2:]
    try:
        d.mkdir(parents=True, exist_ok=True)
        p = d / f"{stem}.json"
        p.write_text(json.dumps(rec, separators=(",", ":")), encoding="utf-8")
        return p
    except OSError:
        return None
