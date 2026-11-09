r"""Names from the January 1999 beta client's debug strings, carried to Trilogy eqgame.exe (docs/PLAN.md 1.1c).

    name_beta.py               -> eq-trilogy\names\eqgame.beta.csv, build\ghidra\beta.names.json,
                                  build\names\beta\ (match table, not committed)
    name_beta.py --validate    also measures the matcher with the string evidence held out

Inputs, all under build\ (see README.md for the order): beta.features.json and trilogy.features.json from
export_features.py, build\bindiff\beta_vs_trilogy.BinDiff, the Trilogy name lists in eq-trilogy\names\, and
the original Trilogy eqgame.exe / eqgfx_dx7.dll bytes (to tell game strings from renderer strings). No
Ghidra needed.

1. Name beta functions from their own debug strings. Patterns that name the *enclosing* function:
       in()     "... in Foo()..."  / "(in Foo())"          in$    "... NULL in Foo\n"   (end of string)
       (f())    "(Foo()) ..."                               f:     "Foo: message"         (renderer style)
   Strings that name a *callee* ("t3dFoo() failed", "Foo failed", "* Foo error_code") are ignored. A name
   is "high" when all of a function's naming strings agree and no other function carries that name,
   "medium" when it wins a majority, or when several functions carry the name and this one has the most
   strings for it. Everything else is dropped.
2. Renderer code: the beta links the renderer statically (Trilogy has it in eqgfx_dx7.dll). Functions whose
   strings occur only in eqgfx_dx7.dll, or whose name is t3d*/s3d*, mark the renderer block; the block is
   the address span between the nearest game-string functions around them. It is kept out of the matching.
3. Match beta -> Trilogy with eqd_match (unique strings, constants, BinDiff, call position).
4. Direct route: the same patterns applied to Trilogy's own strings that also occur in the beta. When both
   routes name a function they must agree. Confidence is the weaker of "the beta function has this name"
   and "the Trilogy function is that beta function"; a direct string hit on its own counts as the name.
"""
import argparse
import csv
import json
import re
from collections import Counter, defaultdict

import eqd_ghidra as eqd
import eqd_match as em
from name_chain import FIELDS, write_csv

IDENT = r"([A-Za-z_][A-Za-z0-9_]{3,})"
PATTERNS = [
    ("in()", re.compile(r"\bin " + IDENT + r"\(\)")),
    ("(f())", re.compile(r"\(" + IDENT + r"\(\)\)")),
    ("in$", re.compile(r"\bin " + IDENT + r"\s*[!.]*\s*$")),
    ("f:", re.compile(r"^\s*" + IDENT + r" ?:\s+\S")),      # libxml writes "xmlNewDoc : malloc failed"
]
NOT_A_NAME = {"everquest"}
RENDERER_NAME = re.compile(r"^_?[ts]3d", re.I)
TRILOGY_SOURCES = ("eqclassic", "mac", "takp")
# libxml2 (statically linked in Trilogy eqgame.exe). Its messages also name grammar productions
# ("...", "...") and, in valid.c, xmlAddID/xmlAddRef print "xmlAddIDDecl:" /
# "xmlAddRefDecl:". In a function with xml* messages only xml*/html* names count, after these aliases.
LIBXML_NAME = re.compile(r"^(xml|html)[A-Z]")
LIBXML_ALIASES = {"xmlAddIDDecl": "xmlAddID", "xmlAddRefDecl": "xmlAddRef"}


def looks_like_function(name):
    """Mixed case with an inner capital or an underscore (CreateActor, loadSounds, Strn_cmp); rules out
    labels and prose ("Class:", "ERROR:", "Sec2:", "in bank", "USERDATA")."""
    if name.lower() in NOT_A_NAME or not re.search(r"[a-z]", name):
        return False
    return bool(re.search(r"[A-Z_]", name[1:]))       # a digit alone is not enough ("Sec2:" is a label)


def string_names(s):
    """(pattern, name) if the string names its enclosing function, else None."""
    for kind, p in PATTERNS:
        m = p.search(s)
        if m and looks_like_function(m.group(1)):
            return kind, m.group(1)
    return None


def name_functions(build, keep=None):
    """{address: (name, confidence, [(kind, string)])} from debug strings. keep(s) filters strings."""
    votes = {}
    for a, f in build.funcs.items():
        if f["class"] in ("library", "import_thunk", "eh_funclet"):
            continue
        hits = [(k, n, s) for s in f["strings"] if (keep is None or keep(s))
                for k, n in [string_names(s) or (None, None)] if n]
        if any(LIBXML_NAME.match(n) for _, n, _ in hits):
            hits = [(k, LIBXML_ALIASES.get(n, n), s) for k, n, s in hits if LIBXML_NAME.match(n)]
        if hits:
            votes[a] = hits
    best = {}
    for a, hits in votes.items():
        c = Counter(n.lower() for _, n, _ in hits)
        (top, n1), *rest = c.most_common()
        if rest and rest[0][1] == n1:
            continue                                    # tie: an inlined callee or two, can't tell
        spell = Counter(n for _, n, _ in hits if n.lower() == top).most_common(1)[0][0]
        best[a] = (spell, "high" if not rest else "medium", n1, hits)
    by_name = defaultdict(list)
    for a, (n, _, k, _) in best.items():
        by_name[n.lower()].append(a)
    out = {}
    for key, addrs in by_name.items():
        if len(addrs) == 1:
            a = addrs[0]
            out[a] = (best[a][0], best[a][1], best[a][3])
            continue
        # Several functions carry the name (inlining, shared helpers): the one with the most strings wins.
        addrs.sort(key=lambda a: -best[a][2])
        if best[addrs[0]][2] > best[addrs[1]][2]:
            a = addrs[0]
            out[a] = (best[a][0], "medium", best[a][3])
    return out


def cstring_in(blob):
    return lambda s: (s.encode("latin-1", "replace") + b"\0") in blob


def renderer_block(beta, in_gfx, in_game, names, game_pairs):
    """Beta functions that belong to the statically linked renderer, and the block's (first, last).

    Evidence per function: renderer (+1) = strings only eqgfx_dx7.dll has, or a t3d*/s3d* debug name;
    game (-1) = two or more strings only Trilogy eqgame.exe has, or a strong BinDiff pair with a Trilogy
    game function (game_pairs). The block is the address run with the highest total (maximum subarray).
    Functions inside it with game evidence of their own stay game code (shared helpers, odd strays)."""
    order = [a for a in sorted(beta.funcs) if beta.funcs[a]["class"] not in em.SKIP_CLASSES]
    ev = {}
    for a in order:
        f = beta.funcs[a]
        g = sum(in_gfx(s) and not in_game(s) for s in f["strings"])
        t = sum(in_game(s) and not in_gfx(s) for s in f["strings"])
        n = names.get(a)
        if (n and RENDERER_NAME.match(n[0])) or g > t:
            ev[a] = 1
        elif t >= 2 or a in game_pairs:
            ev[a] = -1
        elif t:
            ev[a] = 0                                   # one game string: kept as game, but weak
    best, cur, lo, span = 0, 0, 0, None
    for i, a in enumerate(order):
        if cur <= 0:
            cur, lo = 0, i
        cur += ev.get(a) or 0
        if cur > best:
            best, span = cur, (lo, i)
    if span is None:
        return set(), None
    block = {a for a in order[span[0]:span[1] + 1] if a not in ev or ev[a] > 0}
    block |= {a for a, e in ev.items() if e > 0}        # stray renderer functions outside the block
    return block, (order[span[0]], order[span[1]])


def read_trilogy_names(ws):
    out = defaultdict(list)
    for src in TRILOGY_SOURCES:
        p = ws.root / "eq-trilogy" / "names" / f"eqgame.{src}.csv"
        if p.exists():
            with p.open(newline="", encoding="utf-8") as f:
                for r in csv.DictReader(f):
                    out[int(r["address"], 16)].append(r)
    return out


def same_name(a, b):
    return a.rsplit("::", 1)[-1].lower() == b.rsplit("::", 1)[-1].lower()


def cap_if_chain_elsewhere(rows, chain):
    """The chain may already put a row's name on another Trilogy function (the code was split or merged
    since, or a debug message was copied from elsewhere): then the name is only a low guess here."""
    chain_at = defaultdict(set)
    for a, cs in chain.items():
        for c in cs:
            if em.RANK.get(c["confidence"], 0) >= em.RANK["medium"]:
                chain_at[c["name"].rsplit("::", 1)[-1].lower()].add(a)
    elsewhere = []
    for r in rows:
        t = int(r["address"], 16)
        other = sorted(chain_at.get(r["name"].lower(), set()) - {t})
        if other:
            r["confidence"] = "low"
            r["note"] = (f"CHAIN PUTS {r['name']} AT {', '.join(f'{a:08X}' for a in other)}: check before "
                         f"trusting. {r['note']}")
            elsewhere.append({"address": r["address"], "name": r["name"],
                              "chain_elsewhere": [f"{a:08X}" for a in other]})
    return elsewhere


def compare_with_chain(rows, chain, stats, disagree):
    """Free accuracy check on the chain: agree/disagree counts, disagreements listed."""
    for r in rows:
        t, name, conf = int(r["address"], 16), r["name"], r["confidence"]
        for c in chain.get(t, ()):
            ok = same_name(c["name"], name)
            stats[f"vs {c['source']} ({c['confidence']}): {'agree' if ok else 'disagree'}"] += 1
            if not ok:
                disagree.append({"address": f"{t:08X}", "name": name, "confidence": conf,
                                 "chain": c["name"], "chain_source": c["source"],
                                 "chain_confidence": c["confidence"]})


def libxml_version(exe_bytes):
    """libxml2's xmlParserVersion ("20306" = 2.3.6) sits right after the parser option names."""
    m = re.search(rb"load subset\0validate\0+(\d{5})\0", exe_bytes)
    if not m:
        return ""
    v = int(m.group(1))
    return f"{v // 10000}.{v // 100 % 100}.{v % 100}"


def strings_rows(tri, tnames_own, chain, beta_rows, exe_bytes):
    """eqgame.strings.csv: Trilogy functions named by their own debug strings (ones the beta lacks)."""
    ver = libxml_version(exe_bytes)
    taken = {int(r["address"], 16) for r in beta_rows}
    # Inside the libxml block only xml*/html* names are function names; the rest are grammar productions
    # ("..." is printed by xmlParseEntityValue).
    lib = [a for a, (n, _, _) in tnames_own.items() if LIBXML_NAME.match(n)]
    lo, hi = (min(lib), max(lib)) if lib else (0, -1)
    rows = []
    for a, (name, conf, hits) in sorted(tnames_own.items()):
        if a in taken or (lo <= a <= hi and not LIBXML_NAME.match(name)):
            continue
        note = (f"{len(hits)} debug string(s) name it ({', '.join(sorted({k for k, _, _ in hits}))}), "
                f"e.g. {hits[0][2].strip()[:60]!r}")
        if LIBXML_NAME.match(name):
            note = f"library: libxml2{' ' + ver if ver else ''}. {note}"
        rows.append(dict(zip(FIELDS, (f"{a:08X}", name, conf, "strings", note, "", ""))))
    elsewhere = cap_if_chain_elsewhere(rows, chain)
    stats, disagree = Counter(), []
    compare_with_chain(rows, chain, stats, disagree)
    return rows, {"libxml_version": ver, "libxml_named_span": [f"{lo:08X}", f"{hi:08X}"], "rows": len(rows),
                  "by_confidence": dict(Counter(r["confidence"] for r in rows)),
                  "library_rows": sum(r["note"].startswith("library:") for r in rows),
                  "how": dict(stats), "chain_has_name_elsewhere": elsewhere,
                  "disagreements_with_chain": disagree}


def build_matcher(beta, tri, ws, strings=True, trust="code"):
    m = em.Matcher(beta, tri)
    if strings:
        m.add_strings()
    m.add_immediates()
    m.add_bindiff(ws.build / "bindiff" / "beta_vs_trilogy.BinDiff")
    m.run(trust)
    return m


def validate(beta, tri, ws):
    """Matcher without string evidence, checked against the pairs a unique shared string gives."""
    truth = {fa: fb for (fa, fb), ev in build_matcher(beta, tri, ws).evidence.items() if "strings" in ev}
    out = {"truth_pairs_from_unique_strings": len(truth)}
    for trust in ("code", "weak"):
        m = build_matcher(beta, tri, ws, strings=False, trust=trust)
        res = defaultdict(Counter)
        for fa, fb in truth.items():
            got = m.pairs.get(fa)
            res[m.conf.get(fa, "none")]["right" if got == fb else "missed" if got is None else "wrong"] += 1
        out[f"bindiff_trust={trust}"] = {k: dict(v) for k, v in res.items()}
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--validate", action="store_true")
    args = ap.parse_args()
    ws = eqd.Workspace()
    beta = em.Build(ws.reports / "beta.features.json")
    tri = em.Build(ws.reports / "trilogy.features.json")
    trilogy_dir = ws.original / "trilogy"
    in_gfx = cstring_in((trilogy_dir / "eqgfx_dx7.dll").read_bytes())
    in_game = cstring_in((trilogy_dir / "eqgame.exe").read_bytes())

    # 1. + 2. Beta names and the renderer block.
    bnames = name_functions(beta)
    game_pairs = {fa for fa, fb, sim, conf, _ in em.read_bindiff(ws.build / "bindiff" / "beta_vs_trilogy.BinDiff")
                  if sim >= 0.9 and conf >= 0.9 and tri.game(fb) and fa in beta.funcs
                  and min(beta.funcs[fa]["instructions"], tri.funcs[fb]["instructions"]) >= em.TINY}
    renderer, span = renderer_block(beta, in_gfx, in_game, bnames, game_pairs)
    for a in renderer:
        beta.funcs[a]["class"] = "renderer"
    # Beta functions BinDiff pairs with Trilogy library code (FID) are the VC5 CRT: keep them out too.
    lib_tri = {a for a, f in tri.funcs.items() if f["class"] == "library"}
    crt = set()
    for fa, fb, sim, conf, _ in em.read_bindiff(ws.build / "bindiff" / "beta_vs_trilogy.BinDiff"):
        if fb in lib_tri and fa in beta.funcs and beta.funcs[fa]["class"] == "unnamed" and sim >= 0.5:
            beta.funcs[fa]["class"] = "library"
            crt.add(fa)

    # 3. Matching.
    m = build_matcher(beta, tri, ws)

    # 4. Direct route: Trilogy's own naming strings, restricted to strings the beta has too.
    beta_strings = {s for f in beta.funcs.values() for s in f["strings"]}
    tnames = name_functions(tri, keep=lambda s: s in beta_strings)
    tnames_own = name_functions(tri, keep=lambda s: s not in beta_strings)   # -> eqgame.strings.csv

    chain = read_trilogy_names(ws)
    unnamed = {a for a, f in tri.funcs.items() if f["class"] == "unnamed" and a not in chain}
    string_users = defaultdict(set)
    for a, f in beta.funcs.items():
        for s in f["strings"]:
            string_users[s].add(a)

    rows, stats, disagree = [], Counter(), []
    for t in sorted(set(tnames) | {m.pairs[b] for b in bnames if b in m.pairs}):
        via_b = m.rpairs.get(t)
        via = bnames.get(via_b) if via_b is not None else None
        direct = tnames.get(t)
        if via:
            vconf = min(via[1], m.conf[via_b], key=em.RANK.get)
            vnote = (f"beta {via_b:08X} is {via[0]} ({via[1]}: {len(via[2])} debug string(s), "
                     f"e.g. {via[2][0][2].strip()[:60]!r}); match: {m.describe(via_b, t)}")
        if direct:
            # The beta function that shares the Trilogy naming strings, if there is exactly one.
            users = {u for _, _, s in direct[2] for u in string_users.get(s, ())}
            dfrom = users.pop() if len(users) == 1 else None
            dnote = (f"Trilogy references the beta debug string(s) naming {direct[0]} ({direct[1]}: "
                     f"{len(direct[2])}, e.g. {direct[2][0][2].strip()[:60]!r})")
        if via and direct:
            if same_name(via[0], direct[0]):
                name = direct[0]
                conf = max(direct[1], vconf, key=em.RANK.get)
                src_b, note = via_b, f"{dnote}; also {vnote}"
                stats["both routes, agree"] += 1
            else:
                name, conf, src_b = direct[0], "low", dfrom
                note = f"ROUTES DISAGREE: {dnote}; but {vnote}"
                stats["both routes, disagree"] += 1
        elif direct:
            name, conf, src_b, note = direct[0], direct[1], dfrom, dnote
            if via_b is not None and dfrom is not None and via_b != dfrom:
                conf, note = "low", note + f"; but the matcher pairs it with beta {via_b:08X}"
                stats["direct only, matcher points elsewhere"] += 1
            else:
                stats["direct only"] += 1
        else:
            name, conf, src_b, note = via[0], vconf, via_b, vnote
            stats["via beta match only"] += 1
        r = dict(zip(FIELDS, (f"{t:08X}", name, conf, "beta", note,
                              f"{src_b:08X}" if src_b is not None else "", "")))
        rows.append((r, bool(direct)))

    # One name, one function: where a name lands on several Trilogy functions, the debug string in Trilogy
    # itself decides and the match-only rows are dropped (the code was split or the string moved).
    by_name = defaultdict(list)
    for r, direct in rows:
        by_name[r["name"].lower()].append((r, direct))
    rows, dropped = [], []
    for group in by_name.values():
        keep = [r for r, d in group if d] if len(group) > 1 else [group[0][0]]
        if len(keep) != 1:
            keep = []
        rows += keep
        dropped += [{"address": r["address"], "name": r["name"], "note": r["note"]}
                    for r, _ in group if r not in keep]
    rows.sort(key=lambda r: r["address"])
    elsewhere = cap_if_chain_elsewhere(rows, chain)
    compare_with_chain(rows, chain, stats, disagree)

    write_csv(ws.root / "eq-trilogy" / "names" / "eqgame.beta.csv", rows)
    srows, sreport = strings_rows(tri, tnames_own, chain, rows, (trilogy_dir / "eqgame.exe").read_bytes())
    for r in srows:
        r["previously_unnamed"] = int(r["address"], 16) in unnamed
    sreport["previously_unnamed"] = sum(r.pop("previously_unnamed") for r in srows)
    write_csv(ws.root / "eq-trilogy" / "names" / "eqgame.strings.csv", srows)

    # Full match table (not committed): every beta -> Trilogy pair, named or not.
    out_dir = ws.build / "names" / "beta"
    out_dir.mkdir(parents=True, exist_ok=True)
    with (out_dir / "beta_trilogy.match.csv").open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["beta", "trilogy", "confidence", "evidence", "beta_size", "trilogy_size", "beta_name",
                    "trilogy_name", "trilogy_unnamed"])
        for fa, fb in sorted(m.pairs.items()):
            cn = chain.get(fb)
            w.writerow([f"{fa:08X}", f"{fb:08X}", m.conf[fa], m.describe(fa, fb), beta.funcs[fa]["size"],
                        tri.funcs[fb]["size"], bnames[fa][0] if fa in bnames else "",
                        cn[0]["name"] if cn else "", int(fb in unnamed)])
    with (out_dir / "beta.debugnames.csv").open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["beta", "name", "confidence", "renderer", "patterns", "strings"])
        for a, (n, c, hits) in sorted(bnames.items()):
            w.writerow([f"{a:08X}", n, c, int(a in renderer), " ".join(sorted({k for k, _, _ in hits})),
                        " | ".join(s.strip() for _, _, s in hits)])

    named_rows = {int(r["address"], 16): r for r in rows}
    matched_unnamed = {fb for fb in m.pairs.values() if fb in unnamed}
    beta_game = [a for a in beta.funcs if beta.game(a)]
    report = {
        "beta_functions": len(beta.funcs),
        "beta_classes": dict(Counter(f["class"] for f in beta.funcs.values())),
        "beta_renderer_block": [f"{span[0]:08X}", f"{span[1]:08X}"] if span else None,
        "beta_renderer_functions": len(renderer),
        "beta_crt_by_bindiff_to_trilogy_fid": len(crt),
        "beta_game_functions": len(beta_game),
        "beta_debug_names": len(bnames),
        "beta_debug_names_by_confidence": dict(Counter(c for _, c, _ in bnames.values())),
        "beta_debug_names_in_renderer": sum(a in renderer for a in bnames),
        "beta_debug_names_by_pattern": dict(Counter(k for _, _, h in bnames.values() for k in {k for k, _, _ in h})),
        "matcher_pairs": dict(Counter(m.conf.values())),
        "trilogy_named": len(rows),
        "trilogy_named_by_confidence": dict(Counter(r["confidence"] for r in rows)),
        "trilogy_named_previously_unnamed": sum(a in unnamed for a in named_rows),
        "how": dict(stats),
        "trilogy_unnamed": len(unnamed),
        "trilogy_unnamed_matched_to_beta": len(matched_unnamed),
        "trilogy_unnamed_matched_by_confidence": dict(Counter(m.conf[m.rpairs[b]] for b in matched_unnamed)),
        "strings_csv": sreport,
        "dropped_same_name_elsewhere": dropped,
        "chain_has_name_elsewhere": elsewhere,
        "disagreements_with_chain": disagree,
    }
    if args.validate:
        report["validation"] = validate(beta, tri, ws)
    ws.write_report("beta.names.json", report)
    print(json.dumps({k: v for k, v in report.items()
                      if k not in ("strings_csv", "disagreements_with_chain",
                                   "dropped_same_name_elsewhere", "chain_has_name_elsewhere")}, indent=2))
    print(len(disagree), "disagreements with chain names;", len(dropped), "rows dropped (name taken);", len(elsewhere),
          "capped at low (chain has the name elsewhere)")
    print("strings:", json.dumps({k: v for k, v in sreport.items() if k != "chain_has_name_elsewhere"}))


if __name__ == "__main__":
    main()
