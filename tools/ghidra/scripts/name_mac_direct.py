r"""Mac symbols -> Trilogy functions directly (no TAKP in between), for what the TAKP chain did not name.

    name_mac_direct.py gfx     eqgfx_dx7.dll   -> eq-trilogy\names\eqgfx_dx7.mac.csv
    name_mac_direct.py game    eqgame.exe      -> eq-trilogy\names\eqgame.mac.direct.csv (only addresses
                                                  eqgame.mac.csv does not already name)

Inputs (build\ghidra\, see name_chain.py): mac/<client>.features.json, mac.debugmap.json, and the BinDiff
files in build\bindiff. Evidence and grading are eqd_match.Matcher's; seeds are symbols both sides already
carry (DX7 exports, eqgame.mac.csv/manual names). gfx also joins TAKP eqgfx_dx8: a function list name that
is a Mac symbol, carried over by takp_gfx_vs_eqgfx.BinDiff. Where both routes name a function the same, it
is `high` at best; where they disagree the stronger one wins at `low`.

Mac is GCC, Trilogy is VC6: BinDiff alone never reaches `high` here ("weak" trust), and a pair without a
unique string or constant is dropped across a size ratio above SIZE_RATIO.
"""
import argparse
import csv
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

import eqd_ghidra as eqd
import eqd_match as em
from name_chain import FIELDS, is_new_ui, MAC_ROOT, SIZE_RATIO, mac_name, read_takp_list, resolve_list_key, source_file

GFX_FILE = re.compile(r"EQGfx_Dx8")
RANK = em.RANK
# Mac functions that exist twice in the GCC build or only because of it.
NOT_A_NAME = re.compile(r"(^|::)(operator |__|_GLOBAL|~?_?_?cxa)")


# ---- a small Itanium demangler: enough for `_ZN5CXStr6AssureEl15EStringEncoding` -----------------
BUILTIN = dict(v="void", w="wchar_t", b="bool", c="char", a="signed char", h="unsigned char", s="short",
               t="unsigned short", i="int", j="unsigned int", l="long", m="unsigned long", x="long long",
               y="unsigned long long", f="float", d="double", e="long double", z="...")
OPERATORS = {"aS": "operator=", "eq": "operator==", "ne": "operator!=", "pl": "operator+", "mi": "operator-",
             "ml": "operator*", "dv": "operator/", "ix": "operator[]", "cl": "operator()", "pp": "operator++",
             "mm": "operator--", "ps": "operator+", "ng": "operator-", "lt": "operator<", "gt": "operator>",
             "le": "operator<=", "ge": "operator>=", "pL": "operator+=", "mI": "operator-=", "nw": "operator new",
             "dl": "operator delete", "aN": "operator&=", "oR": "operator|=", "co": "operator~", "nt": "operator!"}


class Demangle:
    def __init__(self, s):
        self.s, self.i, self.subs = s, 0, []

    def peek(self):
        return self.s[self.i:self.i + 1]

    def number(self):
        j = self.i
        while self.s[j:j + 1].isdigit():
            j += 1
        n, self.i = int(self.s[self.i:j]), j
        return n

    def source_name(self):
        n = self.number()
        name, self.i = self.s[self.i:self.i + n], self.i + n
        return name

    def subst(self):
        self.i += 1                                 # S
        if self.peek() == "t":
            self.i += 1
            return "std"
        j = self.i
        while self.s[self.i:self.i + 1] not in ("_", ""):
            self.i += 1
        key = self.s[j:self.i]
        self.i += 1
        idx = 0 if key == "" else int(key, 36) + 1
        return self.subs[idx]

    def nested(self):
        self.i += 1                                 # N
        quals = ""
        while self.peek() in "rVK":
            quals += " const" if self.peek() == "K" else ""
            self.i += 1
        parts = []
        while self.peek() != "E":
            c = self.peek()
            if c == "S":
                parts.append(self.subst())
            elif c.isdigit():
                parts.append(self.source_name())
            elif c == "C":
                self.i += 2
                parts.append(parts[-1].split("::")[-1])
                self.subs.append("::".join(parts))
                continue
            elif c == "D":
                self.i += 2
                parts.append("~" + parts[-1].split("::")[-1])
                continue
            else:
                op = self.s[self.i:self.i + 2]
                if op not in OPERATORS:
                    raise ValueError(op)
                self.i += 2
                parts.append(OPERATORS[op])
                continue
            self.subs.append("::".join(parts))
        self.i += 1
        return "::".join(parts), quals

    def type(self):
        c = self.peek()
        if c in BUILTIN:
            self.i += 1
            return BUILTIN[c]
        if c in "PRK":
            self.i += 1
            inner = self.type()
            out = inner + {"P": "*", "R": "&", "K": " const"}[c]
            if c != "K":
                self.subs.append(out)
            return out
        if c == "S":
            return self.subst()
        if c.isdigit():
            t = self.source_name()
            self.subs.append(t)
            return t
        if c == "N":
            t, _ = self.nested()
            self.subs.append(t)
            return t
        raise ValueError(c)

    def run(self):
        if not self.s.startswith("_Z"):
            raise ValueError("not mangled")
        self.i = 2
        if self.peek() == "N":
            name, quals = self.nested()
        else:
            name, quals = self.source_name(), ""
        params = []
        while self.i < len(self.s):
            params.append(self.type())
        if params == ["void"]:
            params = []
        return f"{name}({', '.join(params)}){quals}"


def demangle(symbol):
    """`__ZN5CXStr7FreeRepEP7CStrRep` -> `CXStr::FreeRep(CStrRep*)`, or "" when the symbol is beyond this."""
    s = symbol[1:] if symbol.startswith("__Z") else symbol
    try:
        return Demangle(s).run()
    except (ValueError, IndexError, KeyError):
        return ""


class AnchorMatcher(em.Matcher):
    """Matcher plus a neighbourhood vote: an unmatched pair whose callees and callers are (mostly) the same
    already matched functions, on both sides, with at least two of them. GCC inlines differently from VC6,
    so single call gaps (propagate_calls) are rare; shared neighbourhoods are not."""

    MIN_SHARED = 2
    MIN_JACCARD = 0.6
    MAX_SIZE = 4

    def _neighbours(self, build):
        out = {a: set(f["calls"]) for a, f in build.funcs.items()}
        rev = defaultdict(set)
        for a, cs in out.items():
            for c in cs:
                rev[c].add(a)
        return {a: out[a] | rev[a] for a in out}

    def propagate_calls(self, min_conf="medium"):
        super().propagate_calls(min_conf)
        if not hasattr(self, "_na"):
            self._na, self._nb = self._neighbours(self.a), self._neighbours(self.b)
        anchors = {fa: fb for fa, fb in self.pairs.items() if RANK[self.conf[fa]] >= RANK[min_conf]}
        images = set(anchors.values())
        votes = defaultdict(set)
        for fa, fb in anchors.items():
            for na in self._na[fa]:
                if na in self.pairs or not self.a.game(na):
                    continue
                for nb in self._nb[fb]:
                    if nb in self.rpairs or not self.b.game(nb):
                        continue
                    votes[(na, nb)].add((fa, fb))
        for (na, nb), shared in votes.items():
            if len(shared) < self.MIN_SHARED:
                continue
            sa = {anchors[x] for x in self._na[na] if x in anchors}
            sb = self._nb[nb] & images
            if len(sa & sb) / max(1, len(sa | sb)) < self.MIN_JACCARD:
                continue
            x, y = self.a.funcs[na]["size"], self.b.funcs[nb]["size"]
            if max(x, y) > self.MAX_SIZE * max(1, min(x, y)):
                continue
            self.evidence[(na, nb)]["neighbours"] = len(shared)
            self.evidence[(na, nb)]["calls"] = len(shared)


# ---- features ---------------------------------------------------------------------------------------
def load_mac(ws, wanted_file):
    """Mac Build with every function outside the source files that can hold this client marked 'renderer'
    (a skipped class), so candidates are limited to the right half of the 8,000 symbols."""
    data = json.loads((ws.reports / "mac.features.json").read_text())
    dmap = {e["address"]: e for e in json.loads((ws.reports / "mac.debugmap.json").read_text())["functions"]}
    keep = 0
    for f in data["functions"]:
        e = dmap.get(f["address"])
        if f["class"] == "named" and e is not None and wanted_file(e["file"]) and mac_name(f["name"]):
            keep += 1
        else:
            f["class"] = "renderer"
    tmp = ws.build / "names" / "mac_direct" / "mac.features.filtered.json"
    tmp.parent.mkdir(parents=True, exist_ok=True)
    tmp.write_text(json.dumps(data))
    print(f"mac candidates: {keep}")
    return em.Build(tmp), dmap


def norm(name):
    return name.lstrip("_")


def write_csv(path, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, FIELDS, lineterminator="\n")
        w.writeheader()
        for r in sorted(rows, key=lambda r: r["address"]):
            w.writerow(r)
    print(f"wrote {path} ({len(rows)} rows)")


def carry(mac, dmap, ma):
    """(name, file, signature note) of Mac function ma."""
    f = mac.funcs[ma]
    name = mac_name(f["name"])
    e = dmap.get(ma, {})
    sig = demangle(e.get("symbol", "")) if e.get("symbol", "").startswith("__Z") else ""
    return name, source_file(e.get("file", "")), sig


def direct_match(mac, tri, seeds, bindiff_path):
    m = AnchorMatcher(mac, tri)
    m.add_seeds(seeds)
    m.add_strings()
    m.add_immediates()
    m.add_bindiff(bindiff_path)
    m.run("weak")
    return m


def size_ok(mac, tri, ma, ta, ev):
    a, b = mac.funcs[ma]["size"], tri.funcs[ta]["size"]
    return bool({"strings", "immediates", "seed"} & set(ev)) or max(a, b) <= SIZE_RATIO * max(1, min(a, b))


def finish(rows, tag, mac, tri, ws, out, extra):
    # One Mac name on several Trilogy functions: only the best-graded one keeps it.
    by_name = defaultdict(list)
    for r in rows.values():
        by_name[r["name"]].append(r)
    dropped = 0
    for rs in by_name.values():
        if len(rs) > 1:
            rs.sort(key=lambda r: -RANK[r["confidence"]])
            tied = RANK[rs[1]["confidence"]] == RANK[rs[0]["confidence"]]
            for r in rs[(0 if tied else 1):]:
                rows.pop(r["address"], None)
                dropped += 1
    write_csv(out, list(rows.values()))
    report = {"rows": len(rows), "by_confidence": dict(Counter(r["confidence"] for r in rows.values())),
              "duplicate_name_dropped": dropped, **extra}
    ws.write_report(f"mac_direct_{tag}.json", report)
    print(json.dumps(report, indent=2))


# ---- gfx --------------------------------------------------------------------------------------------
def run_gfx(ws):
    mac, dmap = load_mac(ws, lambda p: bool(GFX_FILE.search(p)))
    tri = em.Build(ws.reports / "eqgfx.features.json")
    takp = em.Build(ws.reports / "takp_gfx.features.json")
    mac_by = defaultdict(list)
    for a, f in mac.funcs.items():
        if mac.game(a):
            mac_by[norm(mac_name(f["name"]))].append(a)
    export_of = defaultdict(list)       # real names the DX7 binary already has (exports)
    for a, f in tri.funcs.items():
        if f["class"] == "named":
            export_of[norm(f["name"])].append(a)
    seeds = [(mac_by[k][0], v[0]) for k, v in export_of.items() if len(v) == 1 and len(mac_by.get(k, ())) == 1]
    print(f"seeds (same name on exports): {len(seeds)}")
    m = direct_match(mac, tri, seeds, ws.build / "bindiff" / "mac_vs_eqgfx.BinDiff")
    seeded = {t for _, t in seeds}

    cand = {}                           # trilogy address -> row
    stats = Counter()
    for ma, ta in m.pairs.items():
        if ta in seeded or tri.funcs[ta]["class"] == "named":
            continue
        ev = m.evidence[(ma, ta)]
        if not size_ok(mac, tri, ma, ta, ev):
            stats["direct: weak match across a size gap dropped"] += 1
            continue
        name, file, sig = carry(mac, dmap, ma)
        conf = m.conf[ma]
        note = f"Mac direct: {m.describe(ma, ta)}" + (f"; signature {sig}" if sig else "")
        cand[ta] = dict(zip(FIELDS, (f"{ta:08X}", name, conf, "mac", note, "", file)))
        cand[ta]["_ma"] = ma
        stats[f"direct {conf}"] += 1

    # Route 2: TAKP eqgfx_dx8 list names that are Mac symbols, carried by takp_gfx_vs_eqgfx.
    listed = read_takp_list(ws.path("reference", "takp_lists") / "eqgfx_dx8.dll_takp_function_list.txt")
    by_key = {k: v for k, v in mac_by.items()}
    by_stem = defaultdict(list)
    for k in by_key:
        by_stem[k[:-1]].append(k)
    lkey = {t: resolve_list_key(norm(n), by_key, by_stem) for t, n in listed.items()}
    t2 = AnchorMatcher(takp, tri)
    t2.add_seeds([(ta, e[0]) for k, e in export_of.items() if len(e) == 1
                  for ta in [next((t for t, n in listed.items() if norm(n) == k), None)]
                  if ta is not None and takp.game(ta)])
    t2.add_strings()
    t2.add_immediates()
    t2.add_bindiff(ws.build / "bindiff" / "takp_gfx_vs_eqgfx.BinDiff")
    t2.run("code")
    for ta_takp, tt in t2.pairs.items():
        k = lkey.get(ta_takp)
        if not k or len(by_key[k]) != 1 or tri.funcs[tt]["class"] == "named" or tt in seeded:
            continue
        ma = by_key[k][0]
        name, file, sig = carry(mac, dmap, ma)
        conf = t2.conf[ta_takp]
        note = (f"TAKP eqgfx_dx8 {ta_takp:08X} (list name is a Mac symbol): {t2.describe(ta_takp, tt)}"
                + (f"; signature {sig}" if sig else ""))
        old = cand.get(tt)
        if old is None:
            row = dict(zip(FIELDS, (f"{tt:08X}", name, conf, "mac", note, f"{ta_takp:08X}", file)))
            row["_ma"] = ma
            cand[tt] = row
            stats[f"chain only {conf}"] += 1
        elif old["name"] == name:
            old["confidence"] = "high" if max(RANK[conf], RANK[old["confidence"]]) >= RANK["medium"] else \
                old["confidence"]
            old["note"] += f" | agrees with {note}"
            old["from_address"] = f"{ta_takp:08X}"
            stats["both routes agree"] += 1
        else:
            keep = old if RANK[old["confidence"]] >= RANK[conf] else dict(
                zip(FIELDS, (f"{tt:08X}", name, conf, "mac", note, f"{ta_takp:08X}", file)))
            keep["confidence"] = "low"
            keep["note"] = f"routes disagree ({old['name']} vs {name}); " + keep["note"]
            cand[tt] = keep
            stats["routes disagree"] += 1
    for r in cand.values():
        r.pop("_ma", None)
    ge = tri
    unnamed = sum(1 for f in ge.funcs.values() if f["class"] == "unnamed")
    return cand, {"direct_pairs": len(m.pairs), "stats": dict(stats), "dx7_unnamed_at_export": unnamed,
                  "seeds": len(seeds)}, tri, mac


# ---- game -------------------------------------------------------------------------------------------
def run_game(ws, names_dir):
    mac, dmap = load_mac(ws, lambda p: "/EverQuest/" in p or "/Suite/" in p or "/Network" in p or True)
    tri = em.Build(ws.reports / "trilogy.features.json")
    have = {}                           # trilogy address -> name, from the lists Mac chain/manual already produced
    for fn in ("eqgame.mac.csv", "eqgame.manual.csv"):
        with (names_dir / fn).open(newline="", encoding="utf-8") as f:
            for r in csv.DictReader(f):
                have[int(r["address"], 16)] = r["name"]
    mac_by = defaultdict(list)
    for a, f in mac.funcs.items():
        if mac.game(a):
            mac_by[mac_name(f["name"])].append(a)
    seeds = [(mac_by[n][0], a) for a, n in have.items()
             if n in mac_by and len(mac_by[n]) == 1 and a in tri.funcs and tri.game(a)]
    seeds = [s for s in seeds if sum(1 for x in seeds if x[0] == s[0]) == 1]
    print(f"seeds (names already carried): {len(seeds)}")
    m = direct_match(mac, tri, seeds, ws.build / "bindiff" / "mac_vs_trilogy.BinDiff")
    seeded = {t for _, t in seeds}
    mac_csv = {a for a in have}
    cand, stats = {}, Counter()
    for ma, ta in m.pairs.items():
        if ta in seeded or ta in have:
            continue
        ev = m.evidence[(ma, ta)]
        if not size_ok(mac, tri, ma, ta, ev):
            stats["weak match across a size gap dropped"] += 1
            continue
        name, file, sig = carry(mac, dmap, ma)
        conf = m.conf[ma]
        note = f"Mac direct: {m.describe(ma, ta)}" + (f"; signature {sig}" if sig else "")
        if is_new_ui(name, file):
            conf = "low"
            note = "Post-Luclin UI name, Trilogy has the old UI: check before trusting. " + note
        cand[ta] = dict(zip(FIELDS, (f"{ta:08X}", name, conf, "mac", note, "", file)))
        stats[conf] += 1
    return cand, {"direct_pairs": len(m.pairs), "stats": dict(stats), "seeds": len(seeds),
                  "already_named_by_mac_or_manual": len(mac_csv)}, tri, mac


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("target", choices=["gfx", "game"])
    ap.add_argument("--names", help="output names directory (default eq-trilogy\\names)")
    args = ap.parse_args()
    ws = eqd.Workspace()
    names_dir = Path(args.names) if args.names else ws.root / "eq-trilogy" / "names"
    if args.target == "gfx":
        rows, extra, tri, mac = run_gfx(ws)
        out = names_dir / "eqgfx_dx7.mac.csv"
    else:
        rows, extra, tri, mac = run_game(ws, ws.root / "eq-trilogy" / "names")
        out = names_dir / "eqgame.mac.direct.csv"
    finish(rows, args.target, mac, tri, ws, out, extra)


if __name__ == "__main__":
    main()
