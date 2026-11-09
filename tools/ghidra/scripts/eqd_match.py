r"""Function matching between two client builds, for the name chain (docs/PLAN.md 4.2).

Works on the files export_features.py and BinDiff leave in build\; no Ghidra needed.

A Matcher collects evidence that function a in build A is function b in build B:

    seed         given by the caller (e.g. both sides carry the same symbol name)
    strings      a string literal that exactly one function references on each side
    immediates   the same for distinctive constants (weaker: needs two)
    bindiff      a BinDiff pair, with its similarity and confidence
    calls        a and b are called at the same place by an already matched pair of callers

and turns it into one-to-one pairs with a confidence of high, medium or low.
"""
import json
import sqlite3
from collections import Counter, defaultdict

TINY = 8                    # instructions; below this, code similarity says little
SMALL_BYTES = 40            # below this, a BinDiff pair with nothing else to back it is at most medium
# "renderer": set by name_beta.py for the beta's statically linked renderer; no export uses it.
SKIP_CLASSES = ("library", "import_thunk", "eh_funclet", "renderer")
# BinDiff steps that pair functions by position or by a bare count, not by their code.
WEAK_ALGORITHMS = ("address sequence", "call sequence matching", "loop count matching", "instruction count")
RANK = {"low": 1, "medium": 2, "high": 3}


class Build:
    def __init__(self, path):
        data = json.loads(open(path, encoding="utf-8").read())
        self.client = data["client"]
        self.funcs = {f["address"]: f for f in data["functions"]}
        for f in self.funcs.values():
            # Internal callees, first occurrence order. Thunks were already resolved by the export.
            seen, order = set(), []
            for c in f["callees"]:
                if isinstance(c, int) and c in self.funcs and c != f["address"] and c not in seen:
                    seen.add(c)
                    order.append(c)
            f["calls"] = order

    def game(self, addr):
        f = self.funcs.get(addr)
        return f is not None and f["class"] not in SKIP_CLASSES

    def users(self, key):
        """feature value -> set of game functions that use it"""
        out = defaultdict(set)
        for f in self.funcs.values():
            if f["class"] not in SKIP_CLASSES:
                for v in f[key]:
                    out[v].add(f["address"])
        return out


def library_span(build, window=50, density=0.4):
    """(first, last) address of the statically linked library block: from where library (FID) functions
    become dense to the last one. Same estimate as coverage_report.py, on an export. None if none."""
    funcs = [f for _, f in sorted(build.funcs.items()) if f["class"] != "eh_funclet"]
    flags = [f["class"] == "library" for f in funcs]
    start = next((i for i in range(len(funcs)) if flags[i] and sum(flags[i:i + window]) >= density * window),
                 None)
    if start is None:
        return None
    end = max(i for i, x in enumerate(flags) if x)
    return funcs[start]["address"], funcs[end]["address"]


def read_bindiff(path):
    """[(address1, address2, similarity, confidence, algorithm)]"""
    con = sqlite3.connect(f"file:{path}?mode=ro", uri=True)
    try:
        return con.execute(
            "select f.address1, f.address2, f.similarity, f.confidence, a.name "
            "from function f join functionalgorithm a on a.id = f.algorithm").fetchall()
    finally:
        con.close()


def unique_feature_votes(a, b, key):
    """{(fa, fb): [values]} for feature values used by exactly one game function on each side."""
    ua, ub = a.users(key), b.users(key)
    votes = defaultdict(list)
    for v, fa in ua.items():
        fb = ub.get(v)
        if fb and len(fa) == 1 and len(fb) == 1:
            votes[(next(iter(fa)), next(iter(fb)))].append(v)
    return votes


class Matcher:
    def __init__(self, a: Build, b: Build):
        self.a, self.b = a, b
        self.evidence = defaultdict(dict)      # (fa, fb) -> {kind: detail}
        self.pairs = {}                        # fa -> fb, accepted
        self.rpairs = {}                       # fb -> fa
        self.conf = {}                         # fa -> high/medium/low
        self.fixed = {}                        # fa -> fb, known pairs that only serve as call anchors

    # ---- evidence -------------------------------------------------------------------------------
    def add_seeds(self, pairs, confidence="high"):
        for fa, fb in pairs:
            self.evidence[(fa, fb)]["seed"] = 1
            self._accept(fa, fb, confidence)

    def add_fixed(self, pairs):
        """Pairs outside the matching (library code with the same name) that anchor call positions."""
        self.fixed.update(pairs)

    def add_strings(self):
        for pair, vals in unique_feature_votes(self.a, self.b, "strings").items():
            self.evidence[pair]["strings"] = len(vals)

    def add_immediates(self):
        for pair, vals in unique_feature_votes(self.a, self.b, "immediates").items():
            if len(vals) >= 2:
                self.evidence[pair]["immediates"] = len(vals)

    def add_bindiff(self, path, kind="bindiff"):
        for fa, fb, sim, conf, alg in read_bindiff(path):
            if self.a.game(fa) and self.b.game(fb):
                weak = alg.replace("function: ", "").startswith(WEAK_ALGORITHMS)
                self.evidence[(fa, fb)][kind] = (round(sim, 3), round(conf, 3), weak)

    # ---- scoring --------------------------------------------------------------------------------
    def grade(self, fa, fb, bindiff_trust):
        """Confidence of a pair from its evidence, or None. bindiff_trust is "code" when both builds
        come from the same compiler (similarity means something) and "weak" when they don't."""
        ev = self.evidence.get((fa, fb), {})
        tiny = min(self.a.funcs[fa]["instructions"], self.b.funcs[fb]["instructions"]) < TINY
        strings = ev.get("strings", 0)
        calls = ev.get("calls", 0)
        imms = ev.get("immediates", 0)
        sim, conf, weak = ev.get("bindiff", (0.0, 0.0, True))
        if bindiff_trust == "code":
            bd = 0 if weak and sim < 0.9 else 3 if sim >= 0.9 and conf >= 0.9 else 2 if sim >= 0.6 else \
                1 if sim >= 0.35 and conf >= 0.5 else 0
            if tiny:
                bd = min(bd, 1)
            # Short leaf functions of the same shape (a range test on a byte field) diff as identical
            # whatever field or constants they use: with no string, constant, callee or matched caller to
            # back the pair, BinDiff alone is not enough for high (Trilogy 00436285 is not IsMage).
            fa_, fb_ = self.a.funcs[fa], self.b.funcs[fb]
            if (min(fa_["size"], fb_["size"]) < SMALL_BYTES and not (strings or calls or imms)
                    and not fa_["callees"] and not fb_["callees"]):
                bd = min(bd, 2)
        else:
            bd = 1 if sim >= 0.6 and conf >= 0.6 and not weak and not tiny else 0
        kinds = (strings > 0) + (calls > 0) + (bd > 0) + (imms > 0)
        if "seed" in ev or bd == 3 or (kinds >= 2 and (strings or calls >= 2 or bd >= 2)):
            return "high"
        if strings or calls >= 2 or bd == 2 or kinds >= 2:
            return "medium"
        if calls or bd or imms:
            return "low"
        return None

    def _accept(self, fa, fb, confidence):
        self.pairs[fa] = fb
        self.rpairs[fb] = fa
        self.conf[fa] = confidence

    def resolve(self, bindiff_trust):
        """Accept the pairs that are each other's best candidate; returns how many were added or upgraded."""
        best_a, best_b = defaultdict(list), defaultdict(list)
        for (fa, fb) in self.evidence:
            g = self.grade(fa, fb, bindiff_trust)
            if g:
                best_a[fa].append((RANK[g], fb))
                best_b[fb].append((RANK[g], fa))
        changed = 0
        for fa, cands in best_a.items():
            cands.sort(reverse=True)
            rank, fb = cands[0]
            if len(cands) > 1 and cands[1][0] == rank:
                continue                        # two equally good candidates: leave it
            back = sorted(best_b[fb], reverse=True)
            if back[0][1] != fa or (len(back) > 1 and back[1][0] == rank):
                continue
            conf = next(k for k, v in RANK.items() if v == rank)
            old_b, old_conf = self.pairs.get(fa), self.conf.get(fa)
            if old_b == fb and RANK[old_conf] >= rank:
                continue
            if old_b is not None and old_b != fb:
                if RANK[old_conf] >= rank or "seed" in self.evidence[(fa, old_b)]:
                    continue
                del self.rpairs[old_b]
            prev_a = self.rpairs.get(fb)
            if prev_a is not None and prev_a != fa:
                if RANK[self.conf[prev_a]] >= rank or "seed" in self.evidence[(prev_a, fb)]:
                    continue
                del self.pairs[prev_a], self.conf[prev_a]
            self._accept(fa, fb, conf)
            changed += 1
        return changed

    def propagate_calls(self, min_conf="medium"):
        """Callees that sit in the same gap between matched callees of a matched caller pair."""
        votes = Counter()
        for fa, fb in self.pairs.items():
            if RANK[self.conf[fa]] < RANK[min_conf]:
                continue
            ca, cb = self.a.funcs[fa]["calls"], self.b.funcs[fb]["calls"]
            if not ca or not cb:
                continue
            # Anchors: callees already matched to each other, in the same relative order on both sides.
            pos_b = {c: i for i, c in enumerate(cb)}
            anchors, last = [], -1
            for i, c in enumerate(ca):
                j = pos_b.get(self.pairs.get(c, self.fixed.get(c)))
                if j is not None and j > last:
                    anchors.append((i, j))
                    last = j
            bounds = [(-1, -1)] + anchors + [(len(ca), len(cb))]
            for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
                ga = [c for c in ca[i0 + 1:i1] if self.a.game(c)]
                gb = [c for c in cb[j0 + 1:j1] if self.b.game(c)]
                if len(ga) == 1 and len(gb) == 1 and i1 - i0 == 2 and j1 - j0 == 2:
                    votes[(ga[0], gb[0])] += 1
        for pair, n in votes.items():
            self.evidence[pair]["calls"] = n

    def run(self, bindiff_trust, rounds=12):
        self.resolve(bindiff_trust)
        for _ in range(rounds):
            self.propagate_calls()
            if not self.resolve(bindiff_trust):
                break
        return self.pairs

    def describe(self, fa, fb):
        ev = self.evidence.get((fa, fb), {})
        parts = []
        if "seed" in ev:
            parts.append("same symbol name")
        if "bindiff" in ev:
            sim, conf, _ = ev["bindiff"]
            parts.append(f"BinDiff {sim:.2f}/{conf:.2f}")
        if "strings" in ev:
            parts.append(f"{ev['strings']} unique string(s)")
        if "calls" in ev:
            parts.append(f"call position in {ev['calls']} matched caller(s)")
        if "immediates" in ev:
            parts.append(f"{ev['immediates']} unique constants")
        return ", ".join(parts)
