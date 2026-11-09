r"""Region kit for the giants (functions >= 4 KB; docs/PLAN.md 8.1 A4 / 8.2).

    cd <workspace>
    .venv\Scripts\python eq-trilogy\tools\context\giant_kit.py regions  trilogy 0042C654
    .venv\Scripts\python eq-trilogy\tools\context\giant_kit.py slice    trilogy 0042C654
    .venv\Scripts\python eq-trilogy\tools\context\giant_kit.py skeleton trilogy 0042C654
    .venv\Scripts\python eq-trilogy\tools\context\giant_kit.py all      trilogy 0042C654 [--out DIR]

Options: --out DIR (default build\giants\kit\<client>\<ADDR>), --cards DIR (default build\giants\cards),
--no-card (derive the regions), --export DIR, --size N (limit the extent), --max-region N (default 4096: a
larger card region or gap is split), --target-region N (default 2048: size of the pieces).

Outputs in <out>:
  regions.json   what the other outputs are built from: regions (id, start, end, label, edges in/out to other
                 regions, entry/exit instructions), the switch list, warnings
  Rnn.asm        the full listing of the region (every instruction of the function's extent is in exactly one
                 region file; names as the export has them; branch targets labelled; switch cases listed)
  Rnn.pseudo.c   best-effort slice of Ghidra's pseudocode (the export has no per-line addresses: chunks are
                 placed by the strings, globals, callees and LAB_/case labels they mention; the header says so)
  INDEX.md       region table, edges, switches, callees / globals / strings per region
  refs.json      the callees, globals and strings per region in full
  skeleton.cpp   C++ skeleton: FUNCTION annotation, signature, every switch with all its case labels and
                 `// REGION Rnn 0xA-0xB: label` ... `// END REGION Rnn` marker pairs in address order

Regions come from the card (build\giants\cards\<ADDR>.md "Regions" section: `ID START-END label` entries; the
smaller of overlapping ranges wins; split points named in the card ("split at", "p0 442134") and the size
limit split big regions and gaps). Without a card the whole extent is cut into pieces at basic-block starts
(switch-case targets preferred, fewest jumps across the cut).

The extent and its switch list come from eqd_unicorn.extent.function_extent(..., switches=[...]) (the fixed
version); case values come from the jump tables read out of the binary (pack_context.switch_tables: bias,
byte index tables map values to targets).
"""
from __future__ import annotations

import argparse
import bisect
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "unicorn"))

import capstone  # noqa: E402
from capstone import x86_const as X  # noqa: E402

import pack_context as pc  # noqa: E402  (no Java needed)

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True

MAX_REGION = 4096
TARGET_REGION = 2048
TINY_GAP_INSNS = 8
RET_LIKE = {"ret", "retf", "retn", "int3", "hlt", "ud2", "iretd", "iret"}
NOT_TOKENS = {"word", "dword", "byte", "float", "double", "qword", "fword", "tbyte", "default"}
SKIP_PREFIX = ("LAB_", "caseD_", "switchdata", "switchD_", "joined_", "thunk_")
CARD_STOP = re.compile(r"^\s*(Fixture|Scripted|Repetition|Risks|Observable|Ready)\b")
ID_RE = r"[A-Z]{1,2}\d{1,3}"
RANGE_RE = re.compile(rf"(?<![\w.])({ID_RE})\s+(?:0x)?([0-9A-Fa-f]{{6}})\s*-\s*~?(?:0x)?([0-9A-Fa-f]{{6}})\b")
BARE_RANGE_RE = re.compile(r"(?<![0-9A-Za-z])(?:0x)?([0-9A-Fa-f]{6})\s*-\s*~?(?:0x)?([0-9A-Fa-f]{6})\b")
ANCHOR_RES = (re.compile(r"\bp(\d+)\s+(?:0x)?([0-9A-F]{6})\b"),)
LIST_RES = (re.compile(r"(?:split at|chunks? at|chunks? in)\s+((?:(?:0x)?[0-9A-F]{6}(?:\s*,\s*|\s+and\s+)?)+)"),)


def h8(v: int) -> str:
    return f"{v:08X}"


def sx(v: int) -> str:
    """Case value text."""
    return str(v) if -10 < v < 10 else (f"0x{v:X}" if v >= 0 else f"-0x{-v:X}")


# ------------------------------------------------------------------ cards

def parse_card(text: str):
    """-> (regions, anchors). regions = [{id, start, end, label}]; anchors = {addr: label} extra split points."""
    lines = text.splitlines()
    sec, on = [], False
    for ln in lines:
        if not on and re.match(r"\s*Regions\b", ln):
            on = True
        elif on and CARD_STOP.match(ln):
            break
        if on:
            sec.append(ln)
    regions, anchors, covered = [], {}, []
    n_unnamed = 0
    for ln in sec:
        ms = list(RANGE_RE.finditer(ln))
        spans = []
        for k, m in enumerate(ms):
            tail_end = ms[k + 1].start() if k + 1 < len(ms) else len(ln)
            tail = ln[m.end():tail_end]
            regions.append({"id": m.group(1), "start": int(m.group(2), 16), "end": int(m.group(3), 16),
                            "label": _label(tail)})
            spans.append((m.start(), m.end()))
            _anchors(tail, anchors)
        # ranges without an id ("tail 44FDF9-44FE24", "branch A 4736D0-475E3A")
        for m in BARE_RANGE_RE.finditer(ln):
            if any(a <= m.start() < b for a, b in spans):
                continue
            n_unnamed += 1
            head = re.sub(r"[\s:;,(]+$", "", ln[:m.start()])
            words = head.split()[-3:]
            tail = ln[m.end():]
            nxt = BARE_RANGE_RE.search(tail)
            tail = tail[:nxt.start()] if nxt else tail
            lab = " ".join(words) or "unnamed"
            regions.append({"id": f"u{n_unnamed}", "start": int(m.group(1), 16), "end": int(m.group(2), 16),
                            "label": (lab + " " + _label(tail)).strip()})
    for r in regions:
        if r["end"] <= r["start"]:
            r["end"] = r["start"]
    return [r for r in regions if r["end"] > r["start"]], anchors


def _label(tail: str) -> str:
    t = re.sub(r"\s+", " ", tail).strip()
    t = re.sub(r"^~?[\d.]+\s*[KB]\b\s*", "", t)
    t = re.sub(r"^\(?\s*[\d.]+\s*K\s*\)?\s*", "", t)
    return t.strip(" ;,-")[:90]


def _anchors(tail: str, out: dict):
    for rx in ANCHOR_RES:
        for m in rx.finditer(tail):
            out.setdefault(int(m.group(2), 16), f"p{m.group(1)}")
    for rx in LIST_RES:
        for m in rx.finditer(tail):
            for a in re.findall(r"(?:0x)?([0-9A-F]{6})", m.group(1)):
                out.setdefault(int(a, 16), "")


# ------------------------------------------------------------------ the function

class Insn:
    __slots__ = ("addr", "size", "raw", "mnem", "text", "named", "exported", "succ", "call", "indirect", "ret")

    @property
    def end(self):
        return self.addr + self.size


def ghidra_text(i) -> str:
    """Approximate the export's text (`MOV EAX,dword ptr [EDI + 0x1ba8]`) for an instruction it lacks."""
    ops = i.op_str
    if (i.mnemonic.startswith("j") or i.mnemonic in ("call", "loop")) and i.operands \
            and i.operands[0].type == X.X86_OP_IMM:
        return f"{i.mnemonic.upper()} 0x{i.operands[0].imm & 0xFFFFFFFF:08x}"
    ops = re.sub(r"\b([a-z]{2,3})\b", lambda m: m.group(1).upper() if _is_reg(m.group(1)) else m.group(1), ops)
    ops = re.sub(r"\*(\d)\b", lambda m: f"*0x{int(m.group(1)):x}", ops)
    ops = re.sub(r"(?<![0-9a-fA-Fx])(\d+)(?![0-9a-fA-Fx])", lambda m: f"0x{int(m.group(1)):x}", ops)
    ops = ops.replace(", ", ",")
    return f"{i.mnemonic.upper()} {ops}".strip()


_REGS = set("eax ebx ecx edx esi edi esp ebp ax bx cx dx si di sp bp al ah bl bh cl ch dl dh cs ds es fs gs ss "
            "st mm0 mm1 mm2 mm3 mm4 mm5 mm6 mm7 xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7".split())


def _is_reg(s):
    return s in _REGS


class Giant:
    def __init__(self, client: str, va: int, export_dir=None, size=None, card=None, no_card=False,
                 max_region=MAX_REGION, target_region=TARGET_REGION, cards_dir=None):
        from eqd_unicorn.extent import function_extent
        from eqd_unicorn.pe import load_pe
        self.client, self.va = client, va
        self.addr = h8(va)
        try:
            self.ex = pc.Export(client, export_dir)
        except SystemExit as e:
            raise RuntimeError(str(e))
        if self.addr not in self.ex.functions:
            raise RuntimeError(f"{self.addr} is not a function in the {client} export")
        self.f = self.ex.functions[self.addr]
        self.warnings: list[str] = []
        self.img = load_pe(self.ex.ws.binary(client))
        starts = {int(a, 16) for a in self.ex.functions}
        self.raw_switches: list = []
        try:
            ext = function_extent(self.img.read, va, frozenset(starts - {va}), size=size,
                                  switches=self.raw_switches, in_image=self.img.contains)
        except TypeError as e:
            raise RuntimeError("eqd_unicorn.extent.function_extent lacks the `switches`/`in_image` arguments "
                               f"(A1's extent fix is required): {e}")
        self.extent = ext
        self.name = self.f["name"]
        self._decode()
        n_exp = self.f.get("instructions")
        if n_exp is not None and n_exp != len(self.order):
            self.warnings.append(f"extent has {len(self.order)} instructions, the export {n_exp}")
        self.switches = self._switches()
        self._flow()
        self._regions(card, no_card, max_region, target_region, cards_dir)
        self._edges()

    # ---- decode

    def _decode(self):
        ex_ins = {}
        rec = self.ex.asm(self.addr)
        if rec:
            for a, _b, t, n in rec["instructions"]:
                ex_ins[int(a, 16)] = (t, n)
        else:
            self.warnings.append("no asm record in the export; names come from the function table only")
        funcs = self.ex.functions
        self.insns: dict[int, Insn] = {}
        for a in sorted(self.extent):
            i = next(_MD.disasm(self.img.read(a, 16), a, 1), None)
            if i is None:
                raise RuntimeError(f"cannot decode {a:#x}")
            o = Insn()
            o.addr, o.size, o.raw, o.mnem = a, i.size, bytes(i.bytes), i.mnemonic
            o.call, o.indirect, o.ret, o.succ = None, False, False, []
            imm_t = None
            if i.operands and i.operands[0].type == X.X86_OP_IMM and (
                    i.mnemonic in ("call", "jmp") or i.mnemonic.startswith("j") or i.mnemonic.startswith("loop")):
                imm_t = i.operands[0].imm & 0xFFFFFFFF
            if a in ex_ins:
                o.text, o.named, o.exported = ex_ins[a][0], ex_ins[a][1], True
            else:
                o.text, o.exported = ghidra_text(i), False
                o.named = o.text
                if imm_t is not None:
                    f = funcs.get(h8(imm_t))
                    if f and i.mnemonic in ("call", "jmp"):
                        o.named = f"{i.mnemonic.upper()} {f.get('short_name') or f['name']}"
            if i.mnemonic == "call":
                if imm_t is not None:
                    o.call = imm_t
                else:
                    o.indirect = True
            o.ret = i.mnemonic in RET_LIKE
            o.succ = [(imm_t, i.mnemonic)] if (imm_t is not None and i.mnemonic != "call") else []
            self.insns[a] = o
        self.order = sorted(self.insns)
        self.index = {a: k for k, a in enumerate(self.order)}

    # ---- switches

    def _switches(self):
        ins = [[h8(a), o.raw.hex(), o.text, o.named] for a, o in sorted(self.insns.items())]
        try:
            pcs = {int(s["at"], 16): s for s in pc.switch_tables(self.ex, {"address": self.addr, "instructions": ins})}
        except Exception as e:    # PE bytes missing etc.
            pcs = {}
            self.warnings.append(f"pack_context.switch_tables failed: {e}")
        out = []
        for jmp, table, count, how in sorted(set(self.raw_switches)):
            sw = {"jmp": jmp, "table": table, "count": count, "how": how, "bias": 0, "bound": None,
                  "default": None, "index_table": None, "cases": [], "source": "extent", "reg": None, "note": ""}
            sw["reg"] = self._index_reg(jmp)
            ent = [int.from_bytes(self.img.read(table + 4 * k, 4), "little") for k in range(count)]
            sw["table_targets"] = ent
            p = pcs.get(jmp)
            good = False
            if p and not p["error"] and p["cases"] and int(p["table"], 16) == table:
                tg = {t for _v, t, _s in p["cases"] if t is not None}
                if tg == set(ent) and (p["index_table"] is not None) == (how == "bytetable"):
                    sw.update(source="pack_context", bias=p["bias"], bound=p["bound"],
                              cases=[(v, t) for v, t, _s in p["cases"]])
                    if p["index_table"]:
                        sw["index_table"] = int(p["index_table"], 16)
                    if p["default"]:
                        sw["default"] = int(p["default"], 16)
                    good = True
            if not good:
                self._own_cases(sw)
            out.append(sw)
        return out

    def _index_reg(self, jmp):
        o = self.insns[jmp]
        m = re.search(r"\[(\w+)\*0x4", o.text)
        return m.group(1) if m else None

    def _own_cases(self, sw):
        """Fallback when pack_context can't decode the switch: values = table indices (bias 0)."""
        jmp, table, how = sw["jmp"], sw["table"], sw["how"]
        ent = sw["table_targets"]
        k = self.index[jmp]
        bt = bound = None
        if how == "bytetable":
            for q in range(k - 1, max(-1, k - 16), -1):
                o = self.insns[self.order[q]]
                m = re.search(r"MOV[ZS]?X? \w+,byte ptr \[\w+ \+ 0x([0-9a-f]+)\]", o.text)
                if m and self.img.contains(int(m.group(1), 16)):
                    bt = int(m.group(1), 16)
                    break
            for q in range(k - 1, max(-1, k - 20), -1):
                m = re.match(r"CMP \S+,(0x[0-9a-f]+|\d+)$", self.insns[self.order[q]].text)
                if m:
                    bound = int(m.group(1), 0)
                    break
        if bt is not None and bound is not None:
            bs = self.img.read(bt, bound + 1)
            sw["cases"] = [(i, ent[b]) for i, b in enumerate(bs) if b < len(ent)]
            sw["index_table"], sw["bound"] = bt, bound
            sw["note"] = "own decode: bias unknown (0 assumed)"
        else:
            sw["cases"] = [(i, t) for i, t in enumerate(ent)]
            sw["note"] = "own decode: bias 0 assumed" + ("; byte index table not found" if how == "bytetable" else "")
        sw["source"] = "own"
        self.warnings.append(f"switch {sw['jmp']:#x}: {sw['note']}")

    # ---- control flow

    def _flow(self):
        case_t = defaultdict(list)           # target -> [(jmp, value)]
        self.sw_by_jmp = {s["jmp"]: s for s in self.switches}
        for s in self.switches:
            for v, t in s["cases"]:
                case_t[t].append((s["jmp"], v))
        self.case_targets = case_t
        self.jump_edges = []                 # (src_idx, dst_idx) for jmp/jcc/switch
        self.edge_list = []                  # (src, dst or None, kind)
        leaders = {self.va}
        for a in self.order:
            o = self.insns[a]
            nxt = o.end if o.end in self.insns else None
            if o.mnem == "jmp" and a in self.sw_by_jmp:
                for v, t in self.sw_by_jmp[a]["cases"]:
                    self._edge(a, t, "switch")
                seen = {t for _v, t in self.sw_by_jmp[a]["cases"]}
                d = self.sw_by_jmp[a]["default"]
                if d is not None and d not in seen:
                    self._edge(a, d, "default")
                leaders.update(seen)
                leaders.add(o.end)
            elif o.mnem == "jmp":
                for t, _k in o.succ:
                    self._edge(a, t, "jmp")
                    leaders.add(t)
                leaders.add(o.end)
            elif o.ret:
                self.edge_list.append((a, None, "ret"))
                leaders.add(o.end)
            elif o.mnem.startswith("j") or o.mnem.startswith("loop"):
                for t, _k in o.succ:
                    self._edge(a, t, "jcc")
                    leaders.add(t)
                self._edge(a, o.end, "fall")
                leaders.add(o.end)
            else:
                self._edge(a, o.end, "fall")
        self.leaders = sorted(x for x in leaders if x in self.insns)
        for s, d, k in self.edge_list:
            if k in ("jmp", "jcc", "switch", "default") and d in self.index and s in self.index:
                self.jump_edges.append((self.index[s], self.index[d]))

    def _edge(self, s, d, kind):
        self.edge_list.append((s, d, kind))

    # ---- regions

    def _regions(self, card, no_card, max_region, target, cards_dir):
        n = len(self.order)
        cards, anchors = [], {}
        self.source, self.card_path = "derived", None
        if not no_card:
            p = Path(card) if card else Path(cards_dir or (self.ex.ws.build / "giants" / "cards")) / f"{self.addr}.md"
            if p.is_file():
                cards, anchors = parse_card(p.read_text(encoding="utf-8", errors="replace"))
                self.card_path = str(p)
                self.source = "card" if cards else "derived"
                if not cards:
                    self.warnings.append(f"{p.name} has no parsable region ranges; regions derived")
            else:
                self.warnings.append(f"no card at {p}; regions derived")
        # owner = the smallest card range containing the instruction
        owner = [None] * n
        best = [None] * n
        for ci, c in enumerate(cards):
            lo = bisect.bisect_left(self.order, c["start"])
            hi = bisect.bisect_left(self.order, c["end"])
            span = c["end"] - c["start"]
            for k in range(lo, hi):
                if best[k] is None or span < best[k]:
                    best[k], owner[k] = span, ci
        for ci, c in enumerate(cards):
            if ci not in owner and bisect.bisect_left(self.order, c["start"]) == bisect.bisect_left(self.order, c["end"]):
                self.warnings.append(f"card region {c['id']} {c['start']:X}-{c['end']:X} holds no extent instruction "
                                     "(outside the function?)")
        # runs of equal owner
        segs = []
        k = 0
        while k < n:
            j = k
            while j < n and owner[j] == owner[k]:
                j += 1
            segs.append([k, j, owner[k]])
            k = j
        # tiny gap runs join the previous (else next) segment
        i = 0
        while i < len(segs):
            s = segs[i]
            if s[2] is None and s[1] - s[0] < TINY_GAP_INSNS and len(segs) > 1:
                tgt = segs[i - 1] if i > 0 else segs[i + 1]
                if i > 0:
                    tgt[1] = s[1]
                else:
                    tgt[0] = s[0]
                del segs[i]
                if i > 0 and i < len(segs) and segs[i][2] == segs[i - 1][2]:
                    segs[i - 1][1] = segs[i][1]
                    del segs[i]
                continue
            i += 1
        # split big segments (card split points, then the size limit)
        pieces = []
        prev_card = ""
        cross = self._cross()
        for k0, k1, ci in segs:
            c = cards[ci] if ci is not None else None
            base = (c["id"], c["label"]) if c else ("", "gap" if cards else "derived")
            cut = [k0]
            for a, lab in sorted(anchors.items()):
                q = bisect.bisect_left(self.order, a)
                if k0 < q < k1 and (self.order[q] - self.order[k0]) > 0:
                    cut.append(q)
            cut = sorted(set(cut)) + [k1]
            seg_pieces = []
            for u, v in zip(cut, cut[1:]):
                tag = anchors.get(self.order[u], "")
                sub = self._size_split(u, v, max_region, target, cross, force=(c is None and not cards))
                for j, (uu, vv) in enumerate(sub):
                    seg_pieces.append((uu, vv, tag, j + 1, len(sub)))
            for uu, vv, tag, j, nsub in seg_pieces:
                lab = base[1]
                if len(seg_pieces) > 1:
                    lab += f" ({tag})" if tag else ""
                    if nsub > 1 or not tag:
                        lab += f" part {j}" if nsub > 1 else ""
                if c is None and prev_card:
                    lab = lab.replace("gap", f"gap after {prev_card}", 1)
                pieces.append({"u": uu, "v": vv, "card": base[0], "label": lab or "card region", "derived": c is None})
            prev_card = base[0] or prev_card
        pieces.sort(key=lambda p: p["u"])
        w = 2 if len(pieces) < 100 else 3
        self.regions = []
        self.region_of = {}
        for r, p in enumerate(pieces, 1):
            rid = f"R{r:0{w}d}"
            a0, a1 = self.order[p["u"]], self.insns[self.order[p["v"] - 1]].end
            reg = {"id": rid, "card_id": p["card"], "start": a0, "end": a1, "label": p["label"],
                   "insns": p["v"] - p["u"], "bytes": sum(self.insns[a].size for a in self.order[p["u"]:p["v"]]),
                   "derived": p["derived"], "u": p["u"], "v": p["v"]}
            self.regions.append(reg)
            for a in self.order[p["u"]:p["v"]]:
                self.region_of[a] = rid
        self.by_id = {r["id"]: r for r in self.regions}

    def _cross(self):
        n = len(self.order)
        d = [0] * (n + 2)
        # filled lazily: _flow ran before _regions, so jump_edges exist
        for s, t in self.jump_edges:
            lo, hi = (s, t) if s < t else (t, s)
            if hi > lo:
                d[lo + 1] += 1
                d[hi + 1] -= 1
        out, acc = [0] * (n + 1), 0
        for k in range(n + 1):
            acc += d[k]
            out[k] = acc
        return out

    def _size_split(self, u, v, max_region, target, cross, force=False):
        """Split instruction indices [u, v) into pieces of about `target` bytes at basic-block starts."""
        size = self.insns[self.order[v - 1]].end - self.order[u]
        if size <= max_region and not (force and size > target * 1.4):
            return [(u, v)]
        cands = [self.index[a] for a in self.leaders if u < self.index[a] < v]
        casets = {self.index[t] for t in self.case_targets if t in self.index}
        out, cur = [], u
        while True:
            rest = self.insns[self.order[v - 1]].end - self.order[cur]
            if rest <= target * 1.4:
                out.append((cur, v))
                break
            lo_a, hi_a, t_a = self.order[cur] + target * 0.6, self.order[cur] + target * 1.4, self.order[cur] + target
            win = [c for c in cands if c > cur and lo_a <= self.order[c] <= hi_a]
            if not win:
                win = [c for c in cands if c > cur and self.order[c] >= lo_a][:1] or [v]
                if win == [v]:
                    out.append((cur, v))
                    break
            b = min(win, key=lambda c: (cross[c] - (3 if c in casets else 0), abs(self.order[c] - t_a)))
            out.append((cur, b))
            cur = b
        return out

    # ---- edges between regions

    def _edges(self):
        out_e, in_e = defaultdict(dict), defaultdict(dict)
        self.exits = Counter()
        for s, d, kind in self.edge_list:
            rs = self.region_of[s]
            if d is None:
                self.exits[rs] += 1
                continue
            rd = self.region_of.get(d, "EXT")
            if rd == rs:
                continue
            for table, key, who in ((out_e, rs, rd), (in_e, rd, rs)):
                e = table[key].setdefault(who, {"n": 0, "kinds": set(), "first": [s, d]})
                e["n"] += 1
                e["kinds"].add(kind)
        for r in self.regions:
            r["out"] = _edge_list(out_e[r["id"]], "to")
            r["in"] = _edge_list(in_e[r["id"]], "from")
            r["exits_function"] = self.exits[r["id"]]
            r["entry_points"] = sorted({d for (s, d, k) in self.edge_list
                                        if d is not None and self.region_of.get(d) == r["id"]
                                        and self.region_of.get(s) != r["id"]} | ({self.va} if r["start"] <= self.va < r["end"] else set()))
            r["exit_points"] = sorted({s for (s, d, k) in self.edge_list
                                       if self.region_of.get(s) == r["id"] and (d is None or self.region_of.get(d) != r["id"])
                                       and k != "ret"})

    # ---- references

    def refs(self):
        """Per region: callees, globals, strings."""
        funcs = self.ex.functions
        strings, globs = self.ex.strings, self.ex.globals
        lo, hi = self.order[0], self.insns[self.order[-1]].end
        res = {r["id"]: {"callees": Counter(), "globals": Counter(), "strings": Counter()} for r in self.regions}
        for a in self.order:
            o = self.insns[a]
            rr = res[self.region_of[a]]
            if o.call is not None:
                f = funcs.get(h8(o.call))
                rr["callees"][f"{f['name']} @{h8(o.call)}" if f else f"@{h8(o.call)}"] += 1
            elif o.indirect:
                rr["callees"][f"indirect {o.named[5:].strip()}"] += 1
            for t in re.findall(r"0x([0-9a-fA-F]{6,8})\b", o.text):
                v = int(t, 16)
                k = h8(v)
                if o.call == v or lo <= v < hi:
                    continue
                if k in strings:
                    rr["strings"][f"{k} {strings[k]['value'][:60]!r}"] += 1
                elif k in globs:
                    rr["globals"][f"{k} {globs[k]['name']}"] += 1
        return res

    # ---- pseudocode

    def pseudo_record(self):
        return self.ex.pseudo(self.addr)


def _edge_list(d, key):
    return [{key: k, "n": v["n"], "kinds": sorted(v["kinds"]), "first": [f"0x{v['first'][0]:08X}", f"0x{v['first'][1]:08X}"]}
            for k, v in sorted(d.items())]


# ------------------------------------------------------------------ outputs

def regions_json(g: Giant) -> dict:
    return {
        "client": g.client, "va": f"0x{g.va:08X}", "name": g.name, "extent_insns": len(g.order),
        "extent_bytes": sum(o.size for o in g.insns.values()), "export_size": g.f.get("size"),
        "source": g.source, "card": g.card_path, "warnings": g.warnings,
        "regions": [{
            "id": r["id"], "card_id": r["card_id"], "start": f"0x{r['start']:08X}", "end": f"0x{r['end']:08X}",
            "label": r["label"], "insns": r["insns"], "bytes": r["bytes"], "derived": r["derived"],
            "in": r["in"], "out": r["out"], "exits_function": r["exits_function"],
            "entry_points": [f"0x{a:08X}" for a in r["entry_points"]],
            "exit_points": [f"0x{a:08X}" for a in r["exit_points"]],
        } for r in g.regions],
        "switches": [switch_json(g, s) for s in g.switches],
    }


def switch_json(g, s):
    groups = switch_groups(s)
    return {"jmp": f"0x{s['jmp']:08X}", "region": g.region_of[s["jmp"]], "table": f"0x{s['table']:08X}",
            "count": s["count"], "how": s["how"], "bias": s["bias"], "bound": s["bound"], "reg": s["reg"],
            "index_table": f"0x{s['index_table']:08X}" if s["index_table"] else None,
            "default": f"0x{s['default']:08X}" if s["default"] is not None else None,
            "source": s["source"], "note": s["note"],
            "cases": [{"values": vs, "target": f"0x{t:08X}", "region": g.region_of.get(t, "EXT")} for t, vs in groups]}


def switch_groups(s):
    """[(target, [values])] in order of first value."""
    d: dict = {}
    for v, t in sorted(s["cases"]):
        d.setdefault(t, []).append(v)
    return sorted(d.items(), key=lambda kv: kv[1][0])


def listing(g: Giant, r) -> str:
    lines = [f"; {g.addr} {g.name}: region {r['id']} 0x{r['start']:08X}-0x{r['end']:08X}  {r['label']}",
             f"; {r['insns']} instructions, {r['bytes']} bytes; text as the export names it; a trailing '; ...' is the address form where a name replaced it"
             + ("; '*' = not in the export's listing" if any(not g.insns[a].exported for a in g.order[r["u"]:r["v"]]) else "")]
    targets = {t for _s, t, k in ((s, d, k) for s, d, k in g.edge_list if d is not None) if k != "fall"}
    prev_end = None
    for a in g.order[r["u"]:r["v"]]:
        o = g.insns[a]
        if prev_end is not None and a != prev_end:
            lines.append(f"; ... {a - prev_end} bytes not in the extent (0x{prev_end:08X}-0x{a:08X}) ...")
        prev_end = o.end
        if a in targets:
            tag = "".join(f" case {sx(v)} of switch {h8(j)}" for j, v in g.case_targets.get(a, [])[:4])
            lines.append(f"LAB_{a:08x}:" + (f"   ;{tag}" if tag else ""))
        mark = " " if o.exported else "*"
        txt = o.named + (f"    ; {o.text}" if o.named != o.text else "")
        lines.append(f"{h8(a)}{mark} {o.raw.hex():<16} {txt}")
        sw = g.sw_by_jmp.get(a)
        if sw:
            lines.append(f"; switch @{h8(a)}: table 0x{sw['table']:08X}, {sw['count']} entries ({sw['how']}), bias {sw['bias']}"
                         + (f", index table 0x{sw['index_table']:08X}" if sw["index_table"] else "")
                         + (f", default 0x{sw['default']:08X}" if sw["default"] is not None else ""))
            for t, vs in switch_groups(sw):
                lines.append(f";   case {','.join(sx(v) for v in vs)} -> 0x{t:08X} ({g.region_of.get(t, 'EXT')})")
    return "\n".join(lines) + "\n"


_STMT_END = re.compile(r"[;{}:]\s*$")
_TOK = re.compile(r"[A-Za-z_][A-Za-z0-9_]{3,}")
_TOK_ASM = re.compile(r"[A-Za-z_][A-Za-z0-9_:~\-]{3,}")
_ADDR_SUFFIX = re.compile(r"_([0-9a-fA-F]{8})$")


def norm_token(t: str) -> str:
    """Ghidra spells the same data label differently in the listing (`s_A_-_B_0054543c`) and the C
    (`s_A___B_0054543c`): key labels that end in an address by that address."""
    m = _ADDR_SUFFIX.search(t)
    return "@" + m.group(1).lower() if m else t


def pseudo_chunks(c: str):
    """-> (header_lines, [(first_line_no, [lines])]) : declarations before the first statement, then statements."""
    lines = c.replace("\r\n", "\n").replace("\r", "\n").split("\n")
    start = next((i for i, ln in enumerate(lines) if ln.strip() == "{"), 0)
    body_start = start + 1
    # declarations: leading lines of the body up to the first blank line after at least one decl
    k = body_start
    while k < len(lines) and lines[k].strip() and re.match(r"\s+[A-Za-z_][\w\s\*\[\]]*;\s*$", lines[k]) \
            and "(" not in lines[k] and "=" not in lines[k]:
        k += 1
    while k < len(lines) and not lines[k].strip():
        k += 1
    header = lines[:k]
    chunks, cur, first = [], [], None
    for i in range(k, len(lines)):
        ln = lines[i]
        if not ln.strip() and not cur:
            continue
        if first is None:
            first = i + 1
        cur.append(ln)
        if _STMT_END.search(ln) or re.match(r"\s*(else|do)\s*$", ln):
            chunks.append((first, cur))
            cur, first = [], None
    if cur:
        chunks.append((first, cur))
    return header, chunks


def pseudo_slices(g: Giant, rec):
    """-> {region id: [(first_line, [lines])]}, stats, header lines."""
    if not rec or not rec.get("c"):
        return None, {}, []
    header, chunks = pseudo_chunks(rec["c"])
    tokmap: dict = defaultdict(Counter)
    for a in g.order:
        o = g.insns[a]
        rid = g.region_of[a]
        for t in _TOK_ASM.findall(o.named.split(" ", 1)[1] if " " in o.named else ""):
            if t in NOT_TOKENS or t.startswith(SKIP_PREFIX):
                continue
            tokmap[norm_token(t)][rid] += 1
    label_addr: dict = {}
    for a in g.order:
        label_addr[f"LAB_{a:08x}"] = a
    case_lab = {}
    for s in g.switches:
        for v, t in s["cases"]:
            case_lab[(f"switchD_{s['jmp']:08x}_caseD_{v & 0xFFFFFFFF:x}")] = t
        if s["default"] is not None:
            case_lab[f"switchD_{s['jmp']:08x}_default"] = s["default"]
    assign, strong = [], []
    for first, ls in chunks:
        text = "\n".join(ls)
        votes = Counter()
        m = re.match(r"\s*(\w+):", ls[0])
        direct = None
        if m:
            nm = m.group(1)
            ad = label_addr.get(nm) or case_lab.get(nm)
            if ad is None:
                mm = re.fullmatch(r"switchD_([0-9a-f]{8})_caseD_([0-9a-f]+)", nm)
                if mm:       # a case label of a switch the table decoder named differently
                    ad = case_lab.get(f"switchD_{mm.group(1)}_caseD_{int(mm.group(2), 16):x}")
            if ad is not None and ad in g.region_of:
                direct = g.region_of[ad]
        if direct:
            votes[direct] += 100
        for t in _TOK.findall(text):
            regs = tokmap.get(norm_token(t))
            if not regs:
                continue
            tot = sum(regs.values())
            for rid, cnt in regs.items():
                votes[rid] += (1.0 if len(regs) == 1 else 0.25) * cnt / tot
        if votes:
            assign.append(votes.most_common(1)[0][0])
            strong.append(True)
        else:
            assign.append(None)
            strong.append(False)
    n = len(chunks)
    for i in range(n):
        if assign[i] is not None:
            continue
        head = chunks[i][1][0].strip()
        look_ahead = bool(re.match(r"(case\b|default\s*:|LAB_\w+:|switchD_\w+:)", head))
        order = (range(i + 1, n), range(i - 1, -1, -1)) if look_ahead else (range(i - 1, -1, -1), range(i + 1, n))
        for rng in order:
            j = next((j for j in rng if strong[j]), None)
            if j is not None:
                assign[i] = assign[j]
                break
        else:
            assign[i] = g.regions[0]["id"]
    out = defaultdict(list)
    for (first, ls), rid in zip(chunks, assign):
        out[rid].append((first, ls))
    stats = {"chunks": n, "anchored": sum(strong), "by_region": {}}
    for rid in out:
        idx = [k for k in range(n) if assign[k] == rid]
        stats["by_region"][rid] = {"chunks": len(idx), "anchored": sum(strong[k] for k in idx)}
    return out, stats, header


def pseudo_file(g: Giant, r, slices, stats, header, total_lines) -> str:
    st = stats["by_region"].get(r["id"], {"chunks": 0, "anchored": 0})
    out = [f"/* {g.addr} {g.name}: region {r['id']} 0x{r['start']:08X}-0x{r['end']:08X}  {r['label']}",
           " * APPROXIMATE SLICE: the export's pseudocode has no per-line addresses. Statements are placed in the",
           " * region whose instructions mention the same strings/globals/callees (or the LAB_/case label they",
           f" * carry); the rest inherits from a neighbour. {st['anchored']} of {st['chunks']} statements here are",
           f" * anchored by a reference; whole function: {stats['anchored']} of {stats['chunks']}. Check against {r['id']}.asm.",
           " * Locals of the whole function follow (Ghidra's), then this region's statements with their line",
           " * numbers in the full pseudocode. */", ""]
    out.extend(header[:150])
    if len(header) > 150:
        out.append(f"  /* ... {len(header) - 150} more declaration lines in the full pseudocode ... */")
    out.append("")
    last = None
    for first, ls in slices.get(r["id"], []):
        if last is not None and first != last:
            out.append(f"  /* ... pseudocode lines {last}-{first - 1} belong to other regions ... */")
        elif last is None and first > len(header) + 2:
            out.append(f"  /* ... pseudocode lines 1-{first - 1} elsewhere ... */")
        out.append(f"/*L{first}*/ " + ls[0])
        out.extend(ls[1:])
        last = first + len(ls)
    if not slices.get(r["id"]):
        out.append("  /* no pseudocode statement was placed in this region */")
    return "\n".join(out) + "\n"


def index_md(g: Giant, refs) -> str:
    L = [f"# {g.addr} {g.name} - region kit index", "",
         f"{len(g.order)} instructions, {sum(o.size for o in g.insns.values())} bytes (extent); regions from "
         f"{'the card ' + Path(g.card_path).name if g.card_path else 'the extent (derived)'}; {len(g.regions)} regions, "
         f"{len(g.switches)} switches.", ""]
    if g.warnings:
        L += ["## Warnings", ""] + [f"- {w}" for w in g.warnings] + [""]
    L += ["## Regions", "", "| id | card | range | bytes | insns | label | in | out |", "|---|---|---|---|---|---|---|---|"]
    for r in g.regions:
        i = ", ".join(f"{e['from']}x{e['n']}" for e in r["in"]) or "-"
        o = ", ".join(f"{e['to']}x{e['n']}" for e in r["out"]) or "-"
        if r["exits_function"]:
            o += f", ret x{r['exits_function']}"
        L.append(f"| {r['id']} | {r['card_id']} | 0x{r['start']:08X}-0x{r['end']:08X} | {r['bytes']} | {r['insns']} | "
                 f"{r['label'].replace('|', '/')} | {i} | {o} |")
    L += ["", "## Switches", ""]
    if not g.switches:
        L.append("none")
    for s in g.switches:
        sj = switch_json(g, s)
        by_reg = Counter(c["region"] for c in sj["cases"])
        L.append(f"- jmp 0x{s['jmp']:08X} ({sj['region']}), table 0x{s['table']:08X}, {s['count']} entries ({s['how']})"
                 f", {len(sj['cases'])} distinct targets, case values {sx(min(v for v, _ in s['cases']))}.."
                 f"{sx(max(v for v, _ in s['cases']))}, bias {s['bias']}, default {sj['default']}, source {s['source']}; "
                 f"targets per region: {', '.join(f'{k}:{v}' for k, v in sorted(by_reg.items()))}")
    L += ["", "## Per region: callees, globals, strings", ""]
    for r in g.regions:
        rr = refs[r["id"]]
        L.append(f"### {r['id']} 0x{r['start']:08X}-0x{r['end']:08X} {r['label']}")
        for key in ("callees", "globals", "strings"):
            items = rr[key].most_common()
            show = ", ".join(f"{k} x{n}" if n > 1 else k for k, n in items[:25])
            L.append(f"- {key} ({len(items)}): {show}" + (f", +{len(items) - 25} more (refs.json)" if len(items) > 25 else "")
                     if items else f"- {key}: none")
        L.append("")
    return "\n".join(L)


# ------------------------------------------------------------------ skeleton

def skeleton(g: Giant) -> str:
    rec = g.pseudo_record() or {}
    sig = (rec.get("signature") or g.f.get("signature") or "void FUN()").strip().rstrip(";")
    frame = next((o.text for a in g.order[:14] for o in [g.insns[a]] if o.text.startswith("SUB ESP,")), None)
    L = [f"// SKELETON for {g.name} (0x{g.va:08X}), generated by context/giant_kit.py - a starting point to",
         "// complete, NOT compilable-guaranteed code. Signature, locals and the switch index expressions are",
         "// placeholders from the export; fix them against the listing (Rnn.asm) before building.",
         "// How it is laid out: REGION blocks are in address order. Each switch sits in the region that holds its jump;",
         "// its case labels are all there (values read from the jump tables, byte-index tables resolved), with the",
         "// target address and the region where that case body lives. Write each region's code between its markers;",
         "// move case bodies under their labels when you restructure, keeping the REGION/END REGION comments.",
         f"// {len(g.order)} instructions, {sum(o.size for o in g.insns.values())} bytes, {len(g.regions)} regions, "
         f"{len(g.switches)} switches" + (f"; {frame}" if frame else ""), "",
         f"// FUNCTION: {g.client.upper()} 0x{g.va:08x}", sig, "{"]
    sws = defaultdict(list)
    for s in g.switches:
        sws[g.region_of[s["jmp"]]].append(s)
    for r in g.regions:
        L.append(f"    // REGION {r['id']} 0x{r['start']:08X}-0x{r['end']:08X}: {_one_line(r['card_id'], r['label'])}")
        for s in sorted(sws[r["id"]], key=lambda s: s["jmp"]):
            L.extend(_switch_lines(g, s))
        L.append("    /* TODO */")
        L.append(f"    // END REGION {r['id']}")
        L.append("")
    L.append("}")
    return "\n".join(L) + "\n"


def _one_line(card_id, label):
    return (f"{card_id} {label}" if card_id and not label.startswith(card_id) else label).replace("\n", " ")


def _switch_lines(g, s):
    j = s["jmp"]
    L = [f"    // SWITCH jmp=0x{j:08X} table=0x{s['table']:08X} count={s['count']} how={s['how']} bias={s['bias']}"
         + (f" index_table=0x{s['index_table']:08X}" if s["index_table"] else "")
         + (f" bound={s['bound']}" if s["bound"] is not None else "") + f" source={s['source']}",
         f"    int sw_{j:08X} = 0; /* TODO: index expression, register {(s['reg'] or '?').upper()}"
         + (f", case value = index {'+' if -s['bias'] >= 0 else '-'} {abs(s['bias']):#x}" if s["bias"] else "") + " */",
         f"    switch (sw_{j:08X}) {{"]
    for t, vs in switch_groups(s):
        labs = " ".join(f"case {sx(v)}:" for v in vs)
        if len(labs) > 70:
            parts, cur = [], []
            for v in vs:
                cur.append(f"case {sx(v)}:")
                if len(" ".join(cur)) > 60:
                    parts.append(" ".join(cur))
                    cur = []
            if cur:
                parts.append(" ".join(cur))
            for p in parts[:-1]:
                L.append(f"    {p}")
            labs = parts[-1]
        L.append(f"    {labs} // -> 0x{t:08X} ({g.region_of.get(t, 'EXT')})")
        L.append("        /* TODO */ break;")
    if s["default"] is not None:
        L.append(f"    default: // -> 0x{s['default']:08X} ({g.region_of.get(s['default'], 'EXT')})")
        L.append("        break;")
    L.append("    }")
    return L


# ------------------------------------------------------------------ checks

def check(g: Giant, out: Path) -> dict:
    """Every extent instruction in exactly one R*.asm; skeleton case labels = jump table targets."""
    addrs = Counter()
    for r in g.regions:
        p = out / f"{r['id']}.asm"
        if p.is_file():
            for ln in p.read_text(encoding="utf-8").splitlines():
                if re.match(r"[0-9A-F]{8}[ *] ", ln):
                    addrs[int(ln[:8], 16)] += 1
    ext = set(g.order)
    res = {"asm_files": sum((out / f"{r['id']}.asm").is_file() for r in g.regions),
           "asm_missing": len(ext - set(addrs)), "asm_extra": len(set(addrs) - ext),
           "asm_duplicated": sum(1 for v in addrs.values() if v > 1)}
    sk = out / "skeleton.cpp"
    if sk.is_file():
        res.update(check_skeleton(g, sk.read_text(encoding="utf-8")))
    return res


def check_skeleton(g: Giant, text: str) -> dict:
    """Parse the skeleton's switches back and compare with the tables read from the image."""
    blocks = re.split(r"(?=    // SWITCH )", text)[1:]
    bad, n = [], 0
    seen = set()
    for b in blocks:
        m = re.match(r"    // SWITCH jmp=0x([0-9A-F]+) table=0x([0-9A-F]+) count=(\d+) how=(\w+) bias=(-?\d+)"
                     r"(?: index_table=0x([0-9A-F]+))?", b)
        jmp, table, count, how = int(m.group(1), 16), int(m.group(2), 16), int(m.group(3)), m.group(4)
        bias = int(m.group(5))
        seen.add(jmp)
        body = b.split("switch (", 1)[1].split("\n    }", 1)[0]
        got, pend = {}, []
        for ln in body.splitlines()[1:]:
            labs = re.findall(r"case (-?(?:0x[0-9A-Fa-f]+|\d+)):", ln)
            pend += [int(x, 0) for x in labs]
            t = re.search(r"// -> 0x([0-9A-F]{8})", ln)
            if t and ln.strip().startswith("default"):
                got["default"] = int(t.group(1), 16)
                pend = []
            elif t:
                for v in pend:
                    got[v] = int(t.group(1), 16)
                pend = []
        ent = [int.from_bytes(g.img.read(table + 4 * k, 4), "little") for k in range(count)]
        if how == "bytetable":
            ibt = int(m.group(6), 16)
            bs = g.img.read(ibt, max(g.sw_by_jmp[jmp]["bound"] or 0, 0) + 1)
            want = {i - bias: ent[bv] for i, bv in enumerate(bs)}
        else:
            want = {i - bias: t for i, t in enumerate(ent)}
        got.pop("default", None)
        n += len(want)
        if got != want:
            bad.append(f"{jmp:#x}: {len(got)} labels vs {len(want)} table cases")
    missing = [f"{s['jmp']:#x}" for s in g.switches if s["jmp"] not in seen]
    return {"skeleton_switches": len(blocks), "skeleton_cases_checked": n, "skeleton_mismatch": bad,
            "skeleton_missing_switches": missing, "skeleton_regions": len(re.findall(r"^    // REGION R", text, re.M)),
            "skeleton_end_regions": len(re.findall(r"^    // END REGION R", text, re.M))}


# ------------------------------------------------------------------ main

def run(client, va, cmd, out=None, **kw):
    cards = kw.pop("cards", None)
    g = Giant(client, va, cards_dir=cards, **kw)
    out = Path(out) if out else g.ex.ws.build / "giants" / "kit" / client / g.addr
    out.mkdir(parents=True, exist_ok=True)
    (out / "regions.json").write_text(json.dumps(regions_json(g), indent=1), encoding="utf-8")
    files = ["regions.json"]
    if cmd in ("slice", "all"):
        refs = g.refs()
        for r in g.regions:
            (out / f"{r['id']}.asm").write_text(listing(g, r), encoding="utf-8")
            files.append(f"{r['id']}.asm")
        rec = g.pseudo_record()
        slices, stats, header = pseudo_slices(g, rec)
        for r in g.regions:
            if slices is None:
                txt = f"/* {g.addr} {r['id']}: no pseudocode in the export */\n"
            else:
                txt = pseudo_file(g, r, slices, stats, header, 0)
            (out / f"{r['id']}.pseudo.c").write_text(txt, encoding="utf-8")
            files.append(f"{r['id']}.pseudo.c")
        (out / "INDEX.md").write_text(index_md(g, refs), encoding="utf-8")
        (out / "refs.json").write_text(json.dumps({k: {kk: dict(vv) for kk, vv in v.items()} for k, v in refs.items()},
                                                  indent=1), encoding="utf-8")
        files += ["INDEX.md", "refs.json"]
    if cmd in ("skeleton", "all"):
        (out / "skeleton.cpp").write_text(skeleton(g), encoding="utf-8")
        files.append("skeleton.cpp")
    chk = check(g, out) if cmd == "all" else {}
    summary = {"client": client, "va": g.addr, "name": g.name, "cmd": cmd, "out": str(out), "source": g.source,
               "extent_insns": len(g.order), "regions": len(g.regions), "switches": len(g.switches),
               "warnings": g.warnings, "files": len(files), "check": chk}
    return g, summary


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["regions", "slice", "skeleton", "all"])
    ap.add_argument("client")
    ap.add_argument("va")
    ap.add_argument("--out")
    ap.add_argument("--cards")
    ap.add_argument("--card", help="one card file")
    ap.add_argument("--no-card", action="store_true")
    ap.add_argument("--export")
    ap.add_argument("--size", type=lambda s: int(s, 0))
    ap.add_argument("--max-region", type=int, default=MAX_REGION)
    ap.add_argument("--target-region", type=int, default=TARGET_REGION)
    a = ap.parse_args(argv)
    try:
        _g, s = run(a.client, int(a.va, 16), a.cmd, a.out, cards=a.cards, card=a.card, no_card=a.no_card,
                    export_dir=a.export, size=a.size, max_region=a.max_region, target_region=a.target_region)
    except RuntimeError as e:
        print(f"giant_kit: {e}", file=sys.stderr)
        return 2
    print(json.dumps(s, indent=1))
    return 0


if __name__ == "__main__":
    sys.exit(main())
