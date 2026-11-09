r"""Similar already-matched functions, for the examples in a context bundle (no network, no compiler).

    .venv\Scripts\python eq-trilogy\tools\context\similar.py trilogy 004C8FCE [ADDR ...] [-k 5] [--cache DIR]

Each function is the original side of `compare.py`'s normalised listing (`compare.tokenise` over the
recursive-descent extent: calls keyed `F:<va>`, imports `I:dll!name`, branches `@<index>`). Registers and
numbers are abstracted (`mov R, dword ptr [R + N]`), and a function becomes the set of its instruction
1- to 3-grams plus its call/import keys. Similarity is the Jaccard index of the sets. Candidates are the queue's
tested/verified functions whose match status is `identical` or `normalised` and whose rewrite is found in the
decomp repo's `src\` (by `// FUNCTION:` annotation), minus the frozen benchmark set
(`queue\benchmark_<client>.json`) and the target itself.

Listings are cached in `<build>\similar\<client>\listings.json` (or --cache / EQD_SIMILAR_CACHE), keyed by the
binary's sha256; the cache holds client code in normalised form, so it stays under build\ like the bundles.

A secondary image (eqmain) also draws candidates from its registered siblings (`siblings` in
unicorn\eqd_unicorn\clients.py: eqgame.exe's tested functions), and a function whose BinDiff sibling
(names\siblings\<client>_<sibling>.csv) is rewritten gets that rewrite first, marked as the same original code.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
TOOLS = HERE.parent
for _p in (TOOLS / "unicorn", TOOLS / "compare", TOOLS / "permute"):
    if str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

GOOD_MATCH = {"identical", "normalised"}
TESTED = {"tested", "verified"}
NGRAM = 3
MAX_SOURCE_LINES = 60
MAX_LISTING_LINES = 60

_REG = re.compile(r"\b(?:e?[abcd]x|[abcd][lh]|e?[sd]il?|e?[sb]pl?|st\(\d\)|[cdefgs]s)\b")
_NUM = re.compile(r"(?<![\w:])(?:0x[0-9a-f]+|\d+)\b")
_LABEL = re.compile(r"@\d+")
_KEY = re.compile(r"\b(?:F|I):\S+")


# --- similarity (pure: tested with synthetic listings) ---------------------------------------------------------

def abstract(tok: str) -> str:
    """`mov eax, dword ptr [ecx + 0x40]` -> `mov R, dword ptr [R + N]`; F:/I: keys kept, labels -> @."""
    keys = _KEY.findall(tok)
    t = _KEY.sub("K", tok)
    t = _NUM.sub("N", _REG.sub("R", _LABEL.sub("@", t)))
    for k in keys:
        t = t.replace("K", k, 1)
    return t


def features(listing: list[str], n: int = NGRAM) -> frozenset:
    """Instruction 1..n-grams of the abstracted listing, plus its call/import keys."""
    a = [abstract(t) for t in listing]
    out = set()
    for m in range(1, n + 1):
        out.update("|".join(a[i:i + m]) for i in range(len(a) - m + 1))
    for t in listing:
        out.update("key " + k for k in _KEY.findall(t))
    return frozenset(out)


def jaccard(a: frozenset, b: frozenset) -> float:
    if not a or not b:
        return 0.0
    return len(a & b) / len(a | b)


def rank(target: list[str], candidates: dict, k: int = 5) -> list[tuple[str, float]]:
    """[(va, similarity)] of the k candidates (va -> listing) closest to the target listing."""
    ft = features(target)
    scored = [(va, round(jaccard(ft, features(lst)), 4)) for va, lst in candidates.items()]
    scored.sort(key=lambda t: (-t[1], t[0]))
    return scored[:k]


# --- the decomp repo: candidate set and rewrite sources ------------------------------------------------------

def benchmark_set(client: str) -> set:
    p = TOOLS / "queue" / f"benchmark_{client}.json"
    if not p.exists():
        return set()
    d = json.loads(p.read_text(encoding="utf-8"))
    return {f["address"].upper() for f in d.get("functions", [])}


def candidates_from_queue(entries, exclude=()) -> list[str]:
    ex = {x.upper() for x in exclude}
    return [e["address"] for e in entries
            if e.get("state") in TESTED and (e.get("match") or {}).get("status") in GOOD_MATCH
            and e["address"] not in ex and not e.get("removed")]


def sibling_pairs(repo: Path, client: str, sibling: str) -> dict:
    """{client ADDR: row} from names/siblings/<client>_<sibling>.csv (queue/siblings.py), or {}."""
    import csv
    p = Path(repo) / "names" / "siblings" / f"{client}_{sibling}.csv"
    if not p.is_file():
        return {}
    with open(p, newline="", encoding="utf-8") as fh:
        return {r["address"].upper(): r for r in csv.DictReader(fh)}


def registered_siblings(client: str) -> tuple:
    try:
        from eqd_unicorn import clients
        return tuple(clients.get(client).siblings)
    except Exception:           # noqa: BLE001 (an older tools tree)
        return ()


def rewrite_sources(repo: Path, client: str) -> dict:
    """{ADDR: (relative path, text from the annotation to the end of the definition)} for every annotated
    function in repo/src."""
    from eqd_unicorn import annotations as ann
    import tu
    out = {}
    srcs = sorted(p for p in (Path(repo) / "src").rglob("*") if p.suffix.lower() in (".c", ".cpp") and p.is_file())
    for src in srcs:
        anns = ann.scan([src], client.upper())
        if not anns:
            continue
        text = src.read_text(encoding="utf-8", errors="replace")
        defs = tu.find_functions(text)
        starts = [0]
        for line in text.splitlines(keepends=True):
            starts.append(starts[-1] + len(line))
        for a in anns:
            pos = starts[min(a.line, len(starts) - 1)]
            f = next((d for d in defs if d.start >= pos), None)
            if f is None:
                continue
            begin = starts[a.line - 1]
            out[f"{a.va:08X}"] = (src.relative_to(repo).as_posix(), text[begin:f.body_end])
    return out


# --- original listings (compare.py's normalisation), cached --------------------------------------------------

class Listings:
    """Original-side normalised listings, computed with compare.tokenise and cached on disk."""

    def __init__(self, client: str, repo: Path, cache_dir=None):
        import compare
        from eqd_unicorn import Workspace, load_pe
        self.compare = compare
        self.ws = Workspace(repo)
        binary = self.ws.binary(client)
        self.side = compare.Side(load_pe(binary))
        self.starts = compare.function_starts(self.ws, client, Path(repo))
        self.side.funcs = {s: f"F:{s:x}" for s in self.starts}
        cache_dir = Path(cache_dir or os.environ.get("EQD_SIMILAR_CACHE") or self.ws.build / "similar" / client)
        self.path = cache_dir / "listings.json"
        self.key = hashlib.sha256(binary.read_bytes()).hexdigest()
        self.data, self.dirty = {}, False
        if self.path.exists():
            try:
                d = json.loads(self.path.read_text(encoding="utf-8"))
                if d.get("key") == self.key:
                    self.data = d["listings"]
            except (ValueError, KeyError, OSError):     # OSError: another worker is replacing it
                pass

    def get(self, addr: str) -> list[str]:
        addr = addr.upper()
        if addr not in self.data:
            from eqd_unicorn.extent import function_extent
            va = int(addr, 16)
            try:
                ext = function_extent(self.side.img.read, va, frozenset(self.starts - {va}),
                                      in_image=self.side.img.contains)
                self.data[addr] = self.compare._tokens(self.compare.tokenise(self.side, va, ext))
            except Exception:           # noqa: BLE001 (undecodable extent: no listing)
                self.data[addr] = []
            self.dirty = True
        return self.data[addr]

    def save(self):
        if self.dirty:
            self.path.parent.mkdir(parents=True, exist_ok=True)
            # A cache shared by parallel workers: own temp name, and skip the save when another worker
            # holds the file (Windows refuses the replace); a later run fills it in.
            tmp = self.path.with_suffix(f".{os.getpid()}.tmp")
            tmp.write_text(json.dumps({"key": self.key, "listings": self.data}), encoding="utf-8")
            try:
                tmp.replace(self.path)
            except PermissionError:
                tmp.unlink(missing_ok=True)
            self.dirty = False


class Finder:
    """Top-k similar matched functions with their rewrite source and original listing.

    `siblings`: other images of the repo whose matched rewrites are candidates too (None = the client's
    registered siblings; () = none). Their entries come from the same `entries` list (pack_context reads every
    queue file of the repo) or are loaded from their own queue file."""

    def __init__(self, client: str, repo: Path, entries, cache_dir=None, exclude=(), listings=None, siblings=None):
        self.repo = Path(repo)
        self.client = client
        self.sources = rewrite_sources(self.repo, client)
        self.listings = listings or Listings(client, self.repo, cache_dir)
        names = {e["address"]: e.get("name", "") for e in entries}
        self.names = names
        excl = set(exclude) | benchmark_set(client)
        self.cands = [a for a in candidates_from_queue(entries, excl) if a in self.sources]
        self.lists_of = {a: self.listings for a in self.cands}
        self.image_of = {}
        self.pairs = {}                 # client ADDR -> (sibling, sibling ADDR, BinDiff similarity)
        self._feat = None
        for sib in (registered_siblings(client) if siblings is None else siblings):
            try:
                self._add_sibling(sib, entries, exclude)
            except Exception as e:      # noqa: BLE001 (no binary/export for it: no candidates from it)
                print(f"similar: no candidates from {sib}: {type(e).__name__}: {e}", file=sys.stderr)

    def _add_sibling(self, sib, entries, exclude):
        srcs = rewrite_sources(self.repo, sib)
        ents = [e for e in entries if e["address"] in srcs]
        if not ents:                    # its queue file wasn't in `entries`
            try:
                from eqd_unicorn import clients
                qf = self.repo / clients.get(sib).queue
                ents = [json.loads(l) for l in qf.read_text(encoding="utf-8").splitlines() if l.strip()]
            except (OSError, ValueError, ImportError):
                ents = []
        lst = Listings(sib, self.repo, self.listings.ws.build / "similar" / sib)
        for e in ents:
            self.names.setdefault(e["address"], e.get("name", ""))
        for a in candidates_from_queue(ents, set(exclude) | benchmark_set(sib)):
            if a in srcs and a not in self.sources:
                self.sources[a] = srcs[a]
                self.lists_of[a] = lst
                self.image_of[a] = sib
                self.cands.append(a)
        for a, r in sibling_pairs(self.repo, self.client, sib).items():
            sa = r["sibling_address"].upper()
            if sa in srcs:
                self.pairs[a] = (sib, sa, r.get("similarity"))
                if sa not in self.sources:
                    self.sources[sa] = srcs[sa]
                    self.lists_of[sa] = lst
                    self.image_of[sa] = sib

    def _save(self):
        for lst in {id(x): x for x in [self.listings, *self.lists_of.values()]}.values():
            lst.save()

    def top(self, addr: str, k: int = 5, min_sim: float = 0.0) -> list[dict]:
        """review: entries below min_sim (1044 of 1165 eqgfx examples were ~0.16) are noise; a sibling (the
        same original code in another image) is kept whatever its score."""
        addr = addr.upper()
        if self._feat is None:
            self._feat = {a: features(self.lists_of[a].get(a)) for a in self.cands}
            self._save()
        ft = features(self.listings.get(addr))
        self._save()
        sc = sorted(((a, round(jaccard(ft, f), 4)) for a, f in self._feat.items() if a != addr),
                    key=lambda t: (-t[1], t[0]))
        sc = [t for t in sc if t[1] >= min_sim][:k]
        out = [self._row(a, s) for a, s in sc]
        pair = self.pairs.get(addr)
        if pair:                        # the same original code in a sibling image: first, whatever its score
            sib, sa, bsim = pair
            out = [r for r in out if r["address"] != sa][:max(k - 1, 0)]
            row = self._row(sa, round(jaccard(ft, features(self.lists_of[sa].get(sa))), 4))
            row["sibling"] = f"BinDiff {bsim}"
            out.insert(0, row)
        return out

    def _row(self, a, s):
        return {"address": a, "name": self.names.get(a, ""), "similarity": s, "source": self.sources[a][0],
                "code": self.sources[a][1], "listing": self.lists_of[a].get(a), "image": self.image_of.get(a)}


def markdown(examples: list[dict]) -> str:
    """The bundle section."""
    out = ["## Similar matched functions\n",
           "Already-rewritten functions whose original code looks most like this one (instruction 1- to 3-grams "
           "and call targets, Jaccard), each byte- or instruction-identical when compiled. Use them for "
           "idioms (types, statement order, macros) that reproduce VC6's code; they are not this function.\n"]
    if not examples:
        out.append("None found.\n")
    for i, x in enumerate(examples, 1):
        where = f"{x['image']}, " if x.get("image") else ""
        out.append(f"### {i}. `{x['name']}` @ 0x{x['address']}  ({where}similarity {x['similarity']:.2f}, "
                   f"`{x['source']}`)\n")
        if x.get("sibling"):
            out.append(f"**Sibling: the same original code in {x.get('image') or 'another image'} ({x['sibling']}).** "
                       "Port this rewrite: keep its code, change only what the two listings show differs (this "
                       "image's addresses, globals and callees, with their own address macros), and annotate it "
                       "for this image.\n")
        src = x["code"].splitlines()
        cut = len(src) > MAX_SOURCE_LINES
        out.append("```cpp\n" + "\n".join(src[:MAX_SOURCE_LINES]) + ("\n// ... (cut)" if cut else "") + "\n```")
        lst = x["listing"]
        cut = len(lst) > MAX_LISTING_LINES
        out.append("Original (normalised):\n```\n" + "\n".join(lst[:MAX_LISTING_LINES]) +
                   (f"\n... ({len(lst)} instructions)" if cut else "") + "\n```\n")
    return "\n".join(out) + "\n"


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("client")
    ap.add_argument("addresses", nargs="+")
    ap.add_argument("-k", type=int, default=5)
    ap.add_argument("--cache", help="listing cache dir (default <build>\\similar\\<client>)")
    ap.add_argument("--repo", help="decomp repo (default the client's, e.g. eq-trilogy; EQD_REPO)")
    ap.add_argument("--queue", help="queue file (default the client's queue file in the repo)")
    a = ap.parse_args(argv)
    from eqd_unicorn import Workspace
    ws = Workspace()
    from eqd_unicorn import clients
    repo = Path(a.repo) if a.repo else ws.repo(a.client)
    qf = Path(a.queue) if a.queue else repo / clients.get(a.client).queue
    entries = [json.loads(l) for l in qf.read_text(encoding="utf-8").splitlines() if l.strip()]
    f = Finder(a.client, repo, entries, a.cache)
    print(f"{len(f.cands)} candidates", file=sys.stderr)
    for ad in a.addresses:
        ad = ad.upper().removeprefix("0X").zfill(8)
        for x in f.top(ad, a.k):
            print(f"{ad}  {x['similarity']:.3f}  {x['address']}  {x['name']}  ({x['source']})")
    return 0


if __name__ == "__main__":
    sys.exit(main())
