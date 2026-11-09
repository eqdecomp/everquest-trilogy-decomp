r"""The name chain: Mac symbols -> TAKP eqgame.exe -> Trilogy eqgame.exe (docs/PLAN.md 4.2).

    name_chain.py takp       Mac symbols + the TAKP function list -> build\names\takp\eqgame.chain.csv
    name_chain.py trilogy    TAKP names -> eq-trilogy\names\eqgame.mac.csv and eqgame.takp.csv

Inputs, all under build\ (see README.md for the order): <client>.features.json from export_features.py,
mac.debugmap.json from mac_debug_map.py, and build\bindiff\<a>_vs_<b>.BinDiff. No Ghidra needed.

Name sources, per row:
    mac    the name is a symbol of the Intel Mac client
    takp   the name only occurs in the community TAKP function list (descriptive, not an original name)
Confidence is how sure we are that this address is that function: high, medium or low. None of them is
"certain", so apply_names.py adds the _guess suffix to all of them.
"""
import argparse
import csv
import json
import re
from collections import Counter, defaultdict

import eqd_ghidra as eqd
import eqd_match as em

FIELDS = ["address", "name", "confidence", "source", "note", "from_address", "file"]
MAC_ROOT = "MacCode_XCode/"
SIZE_RATIO = 3          # a weak Mac match may not replace a list name across a bigger size difference
# Compiler-generated or loader symbols: fine for matching, useless as names.
MAC_NOT_A_NAME = re.compile(r"^(___?tcf_\d+|__static_initialization_and_destruction_\d+|__GLOBAL__|dyld_|"
                            r"FUN_|thunk_|_?_?stub|___)")
# Luclin replaced the UI. These classes don't exist in Trilogy, whatever the code similarity says.
NEW_UI = re.compile(r"^(CXWnd|CXStr|CXRect|CXPoint|CXSize|CSidl|CXML|CXMLSOM|CParam|CTextureAnimation|"
                    r"CTAFrameDraw|C\w*Wnd\w*|C\w*Template\w*|CUITexture|STable|XML\w*|UIElement)\b")


def mac_name(raw):
    """Ghidra's name for a Mac symbol -> the name we carry over, or None."""
    if MAC_NOT_A_NAME.match(raw.rsplit("::", 1)[-1]) or MAC_NOT_A_NAME.match(raw):
        return None
    if "::" not in raw and raw.startswith("_"):
        raw = raw[1:]                       # C symbols carry Mach-O's leading underscore
    return raw or None


def list_key(name):
    """Mac "Class::Method" as the TAKP list writes it."""
    return name.replace("::", "_")


def clean_list_name(name):
    return re.sub(r"[^A-Za-z0-9_~<>,*&=+\-!.\[\]]", "_", name).strip("_") or None


def source_file(path):
    return path.split(MAC_ROOT, 1)[-1] if path else ""


def is_new_ui(name, file):
    return "/Suite/" in file or bool(NEW_UI.match(name))


def resolve_list_key(ln, by_key, by_stem):
    """The Mac key a TAKP list name stands for: as written, modulo a leading underscore, or cut short by
    one character (the list has "rdp_resor" for Mac "_rdp_resort"). None if there is no single one."""
    if ln in by_key:
        return ln
    for k in (ln.lstrip("_"), "_" + ln):
        if k in by_key:
            return k
    cands = by_stem.get(ln, ()) if len(ln) >= 6 else ()
    return cands[0] if len(cands) == 1 else None


def read_takp_list(path):
    out = {}
    for line in path.read_text(encoding="latin-1").splitlines():
        parts = line.split(None, 1)
        if len(parts) == 2:
            out[int(parts[0], 16)] = parts[1].strip()
    return out


def write_csv(path, rows):
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, FIELDS, lineterminator="\n")
        w.writeheader()
        for r in sorted(rows, key=lambda r: r["address"]):
            w.writerow(r)
    print(f"wrote {path} ({len(rows)} rows)")


def takp_csv(ws):
    return ws.build / "names" / "takp" / "eqgame.chain.csv"


# ---- Mac -> TAKP ----------------------------------------------------------------------------------
def chain_takp(ws):
    mac = em.Build(ws.reports / "mac.features.json")
    takp = em.Build(ws.reports / "takp.features.json")
    dmap = {e["address"]: e for e in json.loads((ws.reports / "mac.debugmap.json").read_text())["functions"]}
    listed = read_takp_list(ws.path("reference", "takp_lists") / "eqgame.exe_takp_function_list.txt")

    names = {}                              # mac address -> carried name
    by_key = defaultdict(list)              # list spelling -> mac addresses (debug-map functions first)
    for a, f in mac.funcs.items():
        n = mac_name(f["name"]) if f["class"] == "named" else None
        if n:
            names[a] = n
            by_key[list_key(n)].append(a)
    for key, addrs in by_key.items():
        real = [a for a in addrs if a in dmap]
        by_key[key] = real or addrs

    by_stem = defaultdict(list)
    for key in by_key:
        by_stem[key[:-1]].append(key)
    lkey = {t: resolve_list_key(n, by_key, by_stem) for t, n in listed.items()}
    list_count = Counter(k for k in lkey.values() if k)
    seeds = [(by_key[k][0], t) for t, k in lkey.items()
             if k and list_count[k] == 1 and len(by_key[k]) == 1 and takp.game(t)]
    # TAKP's statically linked CRT. The Mac client links its C library dynamically, so no Mac symbol
    # names code in there (TAKP list "CXWnd_OnProcessFrame" is VC6's __EH_prolog).
    crt = em.library_span(takp)

    m = em.Matcher(mac, takp)
    m.add_seeds(seeds)
    m.add_strings()
    m.add_immediates()
    m.add_bindiff(ws.build / "bindiff" / "mac_vs_takp.BinDiff")
    m.run("weak")

    def file_of(addrs):
        files = {source_file(dmap[a]["file"]) for a in addrs if a in dmap}
        return files.pop() if len(files) == 1 else ""

    rows, stats = [], Counter()
    seeded = {t for _, t in seeds}
    for t, f in takp.funcs.items():
        if not takp.game(t):
            continue
        ln = listed.get(t)
        lk = lkey.get(t)
        spelled = f" (list spells it {ln})" if lk and lk != ln else ""
        ma = m.rpairs.get(t)
        mconf = m.conf.get(ma)
        mname = names.get(ma) if ma is not None else None
        how = m.describe(ma, t) if ma is not None else ""
        row = None
        if crt and crt[0] <= t <= crt[1]:
            if ln and not lk and clean_list_name(ln):
                row = (clean_list_name(ln), "low", "takp", "TAKP list only; descriptive. Inside the CRT span, "
                       "where no Mac symbol applies", "", "")
                stats["CRT span: list name only"] += 1
            elif lk or mname:
                stats["CRT span: Mac name dropped"] += 1
        elif lk:
            cands = by_key[lk]
            name = names[cands[0]]
            if t in seeded:
                row = (name, "high", "mac", f"TAKP list name is a Mac symbol{spelled}, one function on each side", cands[0],
                       file_of(cands))
                stats["list name = Mac symbol, unique"] += 1
            elif ma in cands:
                row = (name, "high", "mac", f"TAKP list name is a Mac symbol{spelled}; overload picked by {how}", ma,
                       file_of([ma]))
                stats["list name = Mac symbol, overload resolved"] += 1
            elif mname and mconf in ("high", "medium") and mname != name:
                row = (name, "medium", "mac", f"TAKP list name is a Mac symbol{spelled}, but {how} points at {mname}", "",
                       file_of(cands))
                stats["list name = Mac symbol, matcher disagrees"] += 1
            else:
                row = (name, "high", "mac", f"TAKP list name is a Mac symbol{spelled} ({len(cands)} candidates there)", "",
                       file_of(cands))
                stats["list name = Mac symbol, overload open"] += 1
        elif (mname and ln and mconf != "high" and clean_list_name(ln)
              and not {"strings", "immediates"} & set(m.evidence[(ma, t)])
              and max(f["size"], mac.funcs[ma]["size"]) > SIZE_RATIO * min(f["size"], mac.funcs[ma]["size"])):
            # A weak match (BinDiff and call position only) that would replace the list name, between
            # functions of very different size. Unique strings or constants still win (zlib's
            # _tr_flush_block is 4x bigger in the GCC build).
            row = (clean_list_name(ln), "low", "takp", f"TAKP list only; descriptive. Rejected Mac {mname} ({how}): "
                   f"{f['size']} vs {mac.funcs[ma]['size']} bytes", "", "")
            stats["list name kept, weak Mac match of different size rejected"] += 1
        elif mname:
            note = how + (f"; TAKP list calls it {ln}" if ln else "")
            row = (mname, mconf, "mac", note, ma, file_of([ma]))
            stats[f"Mac match ({mconf})" + (", replaces a descriptive list name" if ln else ", not in the list")] += 1
        elif ln and clean_list_name(ln):
            row = (clean_list_name(ln), "low", "takp", "TAKP list only; descriptive, no Mac symbol of this name",
                   "", "")
            stats["list name only"] += 1
        if row:
            rows.append(dict(zip(FIELDS, (f"{t:08X}", row[0], row[1], row[2], row[3],
                                          f"{row[4]:08X}" if row[4] != "" else "", row[5]))))
    write_csv(takp_csv(ws), rows)
    game = sum(takp.game(t) for t in takp.funcs)
    report = {
        "takp_game_functions": game,
        "named": len(rows),
        "by_confidence": dict(Counter(r["confidence"] for r in rows)),
        "by_source": dict(Counter(r["source"] for r in rows)),
        "with_source_file": sum(bool(r["file"]) for r in rows),
        "how": dict(stats),
        "list_entries": len(listed),
        "list_names_resolved_modulo_underscore_or_truncation": sum(1 for t, k in lkey.items() if k and k != listed[t]),
        "crt_span": [f"{crt[0]:08X}", f"{crt[1]:08X}"] if crt else None,
        "list_entries_not_a_function_entry": sum(t not in takp.funcs for t in listed),
        "list_entries_on_library_or_thunk": sum(t in takp.funcs and not takp.game(t) for t in listed),
        "matcher_pairs": dict(Counter(m.conf.values())),
    }
    ws.write_report("takp.chain.json", report)
    print(json.dumps(report, indent=2))


# ---- TAKP -> Trilogy ------------------------------------------------------------------------------
def chain_trilogy(ws):
    takp = em.Build(ws.reports / "takp.features.json")
    tri = em.Build(ws.reports / "trilogy.features.json")
    with takp_csv(ws).open(newline="", encoding="utf-8") as f:
        takp_names = {int(r["address"], 16): r for r in csv.DictReader(f)}

    m = em.Matcher(takp, tri)
    # Library code and imports with the same name on both sides anchor call positions.
    for cls in ("library",):
        na, nb = defaultdict(list), defaultdict(list)
        for side, idx in ((takp, na), (tri, nb)):
            for a, f in side.funcs.items():
                if f["class"] == cls:
                    idx[f["name"]].append(a)
        m.add_fixed({v[0]: nb[k][0] for k, v in na.items() if len(v) == 1 and len(nb.get(k, ())) == 1})
    m.add_strings()
    m.add_immediates()
    m.add_bindiff(ws.build / "bindiff" / "takp_vs_trilogy.BinDiff")
    m.run("code")

    rows = {"mac": [], "takp": []}
    named = {}                              # trilogy address -> confidence
    stats = Counter()
    for t, r in m.pairs.items():
        src = takp_names.get(t)
        if src is None:
            stats["matched to an unnamed TAKP function"] += 1
            continue
        conf = min(m.conf[t], src["confidence"], key=em.RANK.get)
        note = f"TAKP {t:08X}: {m.describe(t, r)}. TAKP name: {src['note']}"
        if is_new_ui(src["name"], src["file"]):
            conf = "low"
            note = "Post-Luclin UI name, Trilogy has the old UI: check before trusting. " + note
            stats["new-UI names capped at low"] += 1
        rows[src["source"]].append(dict(zip(FIELDS, (f"{r:08X}", src["name"], conf, src["source"], note,
                                                     f"{t:08X}", src["file"]))))
        named[r] = conf

    names_dir = ws.root / "eq-trilogy" / "names"
    write_csv(names_dir / "eqgame.mac.csv", rows["mac"])
    write_csv(names_dir / "eqgame.takp.csv", rows["takp"])

    game = [f for a, f in tri.funcs.items() if tri.game(a)]
    largest = sorted(game, key=lambda f: -f["size"])[:50]
    report = {
        "trilogy_game_functions": len(game),
        "matched_to_takp": dict(Counter(m.conf.values())),
        "named": len(named),
        "by_confidence": dict(Counter(named.values())),
        "by_source": {k: len(v) for k, v in rows.items()},
        "with_source_file": sum(bool(r["file"]) for v in rows.values() for r in v),
        "notes": dict(stats),
        "largest_50_unnamed": sum(f["address"] not in named for f in largest),
        "largest_50": [{"address": f"{f['address']:08X}", "size": f["size"],
                        "confidence": named.get(f["address"], "")} for f in largest],
    }
    ws.write_report("trilogy.chain.json", report)
    print(json.dumps({k: v for k, v in report.items() if k != "largest_50"}, indent=2))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("target", choices=["takp", "trilogy"])
    args = ap.parse_args()
    ws = eqd.Workspace()
    (chain_takp if args.target == "takp" else chain_trilogy)(ws)


if __name__ == "__main__":
    main()
