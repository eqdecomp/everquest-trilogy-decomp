r"""Source checks for a decomp repo's src/: status markers, unmarked fake-match oddities, readability ratchet.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\compare\source_checks.py [--floor F] [--queue F] [-v] [--json F]
        [--update | --raise-floor]

(a) **Status markers** (RULES.md "Source status markers"): a comment line starting with `@name` must use an
    approved name (`@early-stop`, `@identity-TODO`, `@dead-code`, `@equivalent`), be followed by `: <reason>`,
    sit in the comment block in front of a `// FUNCTION:`-annotated function or inside its body, and appear
    once per function. `fake match:` comments need a reason too. With `--queue`, `@early-stop`/`@equivalent`
    on a function whose recorded match status is `identical` is stale.
(b) **Oddities** that look like fake-match code (RULES.md "Fake matches"): empty `do {} while (0)` barriers,
    locals that are never used again, `goto`, `__asm`, and `register` (banned: no marker covers it). Each needs a `fake match: <what and why>`
    comment (goto/`__asm` may say `as original: <why>` instead: the original's own shape) on its line, in the
    comment lines directly above it, or in the comment block in front of the function (covers the function).
(c) **Ratchet**: per unit (an annotated function by VA, an unannotated function `path:Name`, the file scope
    `path`) the counts of C-style/C++ casts, `reinterpret_cast`, `goto`, raw address arithmetic
    (`*(T*)(p + 0x10)`, `(T*)0x006E4DE0`), `extern`, `union`, `__asm`, `register`, `volatile`, empty
    barriers, unused locals and unmarked oddities, against the floor file (default `<repo>\tools-ratchet.json`).
    A count above its floor is a violation; so is any unmarked oddity in a unit the floor doesn't know yet
    (new functions). Counts that drop are reported; `--update` writes the lower floors and adds new units
    (refused while there are violations); `--raise-floor` accepts every current count (the explicit, reviewed
    step) and also creates the file.

Exit 0 = no violations, 1 = violations, 2 = setup problem. Importable: `check_repo(repo, floor_path, queue)`.
"""
from __future__ import annotations

import argparse
import bisect
import collections
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

_PERMUTE = Path(__file__).resolve().parent.parent / "permute"
if str(_PERMUTE) not in sys.path:
    sys.path.insert(0, str(_PERMUTE))
import ctoken as ct  # noqa: E402
import tu  # noqa: E402

MARKERS = {"early-stop": "body complete, remaining diff bounded and explained",
           "identity-TODO": "equivalence not proven yet; say what would prove it",
           "dead-code": "no references, still rebuilt",
           "equivalent": "non-identical: why the remaining diff is harmless"}
MATCH_MARKERS = {"early-stop", "equivalent"}     # stale once the function is byte-identical
METRICS = ["cast", "reinterpret_cast", "goto", "raw_addr", "extern", "union", "asm", "register", "volatile",
           "barrier", "unused_local", "unmarked"]
ODDITIES = {"barrier", "register", "unused_local", "goto", "asm"}
SHAPE_OK = {"goto", "asm"}                       # may be covered by `as original:` too
BANNED = {"register"}                            # register pinning: never covered by a marker
FLOOR_NAME = "tools-ratchet.json"
SCHEMA = 1

_ANN = re.compile(r"^\s*//\s*FUNCTION:\s*(\w+)\s+(0x[0-9a-fA-F]+)", re.M)
_MARK = re.compile(r"^\s*(?://+|/\*+|\*+)?\s*@([A-Za-z][\w-]*)(.*)$")
_FAKE = re.compile(r"fake match\s*:(.*)", re.I)
_ASORIG = re.compile(r"as original\s*:(.*)", re.I)
# words in comments that identify a routine or state its contract (readability: R5 must never lose one)
_KEYWORDS = {"zlib": re.compile(r"(?i)zlib"), "__cdecl": re.compile(r"__cdecl"), "__thiscall": re.compile(r"__thiscall"),
             "__fastcall": re.compile(r"__fastcall"), "__stdcall": re.compile(r"__stdcall"),
             "EAX": re.compile(r"\bEAX\b"), "Returns": re.compile(r"\bReturns\b"), "returns": re.compile(r"\breturns\b")}
_CONT_STOP = re.compile(r"^\s*//\s*(?:FUNCTION:|[A-Z][A-Z ]+:)")
_CPP_CASTS = {"static_cast", "const_cast", "dynamic_cast"}


@dataclass
class Finding:
    kind: str           # marker | oddity | ratchet
    path: str
    line: int
    unit: str
    msg: str

    def __str__(self):
        return f"{self.path}:{self.line}: [{self.unit}] {self.msg}"


@dataclass
class FileResult:
    path: str
    units: dict = field(default_factory=dict)        # unit key -> Counter
    lines: dict = field(default_factory=dict)        # unit key -> first line
    markers: list = field(default_factory=list)      # (line, unit, name, text)
    errors: list = field(default_factory=list)       # Finding (marker problems)
    oddities: list = field(default_factory=list)     # (Finding, covered)
    notes: dict = field(default_factory=dict)        # unit key -> Counter of `fake match` / `as original` notes


# --- scanning ------------------------------------------------------------------------------------------------

class _Lines:
    def __init__(self, text):
        self.starts = [0] + [m.end() for m in re.finditer(r"\n", text)]
        self.text = text

    def of(self, off):
        return bisect.bisect_right(self.starts, off)

    def get(self, n):
        a = self.starts[n - 1]
        b = self.starts[n] if n < len(self.starts) else len(self.text)
        return self.text[a:b].rstrip("\r\n")


def _is_cast(S, M, i, typedef_names=frozenset()):
    if not ct.is_cast_group(S, M, i, typedef_names):
        return False
    prev = S[i - 1] if i else None
    if prev is not None and ((prev.kind == "id" and prev.text not in ("return", "case", "else", "do"))
                             or prev.text == "]" or (prev.text == ")" and not _is_cast(S, M, M[i - 1], typedef_names))):
        return False                                  # a call, a macro, `if (x) y`, `sizeof (T)`; `(T)(U)x` is
    inner = S[i + 1:M[i]]
    ptr = False
    for t in inner:
        if t.text in ("*", "&"):
            ptr = True
        elif ptr and t.text not in ("const", "volatile"):
            return False                              # `(a * b)` is arithmetic
    return True


def _num(t):
    try:
        return int(re.sub(r"[uUlL]+$", "", t.text), 0)
    except ValueError:
        return None


def _raw_addr(S, M, i):
    """S[i] starts a pointer cast: is its operand a client address or `(base + constant)`?"""
    if not any(t.text == "*" for t in S[i + 1:M[i]]):
        return False
    j = M[i] + 1
    if j >= len(S):
        return False
    if S[j].kind == "num":
        v = _num(S[j])
        return v is not None and v >= 0x10000
    if S[j].text != "(":
        return False
    k, end = j + 1, M[j]
    while k < end:
        t = S[k]
        if t.kind == "op" and t.text in "([{":
            k = M[k] + 1
            continue
        if t.text in ("+", "-") and k + 1 < end and S[k + 1].kind == "num":
            return True
        k += 1
    return False


_DTOR = re.compile(r"~\s*([A-Za-z_]\w*)\s*\(")


def _unused_locals(S, M, a, b, raii=frozenset()):
    """[(token index, name)] of locals declared in S[a:b] (a body without braces) that are never named again.
    A non-pointer object of a type with a destructor (`raii`: a scoped lock or guard) does its work in its
    constructor and destructor, so it isn't unused."""
    try:
        stmts = ct.Parser(S, M).seq(a, b)
    except (ct.ParseError, KeyError, IndexError, ValueError):
        return []
    names = collections.Counter(S[k].text for k in range(a, b) if S[k].kind == "id")
    out = []
    for root in stmts:
        for s in root.walk():
            if s.kind != "decl" or S[s.a].text in ("static", "typedef", "extern"):
                continue
            pd = ct.parse_decl(S, s.a, s.b - 1, M)
            if not pd:
                continue
            base = {S[k].text for k in range(*pd[0])}
            for d in pd[1]:
                if names[d.name] == 1 and not (base & raii and not d.pointer and not d.array):
                    out.append((d.name_idx, d.name))
    return out


def _comment_reason(comments, lo_line, hi_line, rx):
    return any(rx.search(text) for (l0, l1, text) in comments if l1 >= lo_line and l0 <= hi_line)


_INCLUDE_DTORS: frozenset = frozenset()     # class names with a destructor in <repo>/include (check_repo)


def scan_file(path: Path, rel: str, client: str | None = None) -> FileResult:
    text = path.read_text(encoding="utf-8", errors="replace")
    raii = frozenset(_DTOR.findall(text)) | _INCLUDE_DTORS
    L = _Lines(text)
    toks = ct.tokenize(text)
    S = ct.significant(toks)
    res = FileResult(rel)
    try:
        M = ct.match_brackets(S)
        funcs = tu.find_functions(text)
    except ValueError as e:
        res.errors.append(Finding("marker", rel, 1, rel, f"cannot parse: {e}"))
        return res
    typedef_names = ct.typedef_names(S, M)
    comments = [(L.of(t.start), L.of(t.end - 1), t.text) for t in toks if t.kind == "comment"]
    code_lines = {L.of(t.start) for t in S}

    # units: (key, lead offset, start, end, annotated)
    units = []
    seen = collections.Counter()
    for f in funcs:
        m = None
        for m_ in _ANN.finditer(text, f.lead, f.start):
            if client is None or m_.group(1).upper() == client.upper():
                m = m_
        if m:
            key = f"{int(m.group(2), 16):08X}"
        else:
            seen[f.name] += 1
            key = f"{rel}:{f.name}" + (f"#{seen[f.name]}" if seen[f.name] > 1 else "")
        units.append((key, f.lead, f.start, f.body_end, bool(m), f))
    starts = [u[2] for u in units]

    def unit_at(off, with_lead=False):
        k = bisect.bisect_right(starts, off) - 1
        for j in (k, k + 1) if with_lead else (k,):
            if 0 <= j < len(units):
                key, lead, st, end, ann, _ = units[j]
                if (lead if with_lead else st) <= off < end:
                    return units[j]
        return None

    counts = collections.defaultdict(collections.Counter)
    res.lines[rel] = 1
    counts[rel]                                          # the file scope is a unit too
    for u in units:
        res.lines[u[0]] = L.of(u[2])
        counts[u[0]]                                     # every unit is listed, even with no counts

    def add(off, metric, n=1):
        u = unit_at(off)
        counts[u[0] if u else rel][metric] += n

    # (a) status markers and fake-match comments
    fn_markers = collections.defaultdict(set)
    for (l0, l1, ctext) in comments:
        off = None
        for n, ln in enumerate(ctext.splitlines()):
            mm = _MARK.match(ln)
            fm = _FAKE.search(ln)
            line = l0 + n
            if fm and len(fm.group(1).strip(" */")) < 3:
                res.errors.append(Finding("marker", rel, line, rel, "`fake match:` without a reason"))
            if not mm:
                continue
            name, rest = mm.group(1), mm.group(2)
            if off is None:
                off = next(t.start for t in toks if t.kind == "comment" and L.of(t.start) == l0)
            u = unit_at(off, with_lead=True)
            ukey = u[0] if u else rel
            if name not in MARKERS:
                alt = next((k for k in MARKERS if k.lower() == name.lower()), None)
                res.errors.append(Finding("marker", rel, line, ukey, f"unknown marker @{name}" + (
                    f" (write @{alt})" if alt else f" (approved: {', '.join('@' + k for k in MARKERS)})")))
                continue
            reason = rest.strip().rstrip("*/").strip()
            if not reason.startswith(":") or len(reason[1:].strip()) < 3:
                res.errors.append(Finding("marker", rel, line, ukey, f"@{name} needs `: <reason>` ({MARKERS[name]})"))
            if not u or not u[4]:
                res.errors.append(Finding("marker", rel, line, ukey,
                                          f"@{name} is not attached to a // FUNCTION: annotated function"))
                continue
            if name in fn_markers[ukey]:
                res.errors.append(Finding("marker", rel, line, ukey, f"@{name} twice for one function"))
            fn_markers[ukey].add(name)
            res.markers.append((line, ukey, name, reason[1:].strip()))

    # (a2) fake-match / as-original notes per unit, for readability's "no marker may disappear" check: each
    # comment line naming one counts once (`fake match`, `fake match lines`, `as original`), and the plain `//`
    # lines that continue a `// fake match:` note count as its lines
    notes = collections.defaultdict(collections.Counter)
    prev_fake_line = None
    for t in toks:
        if t.kind != "comment":
            continue
        l0 = L.of(t.start)
        u = unit_at(t.start, with_lead=True)
        ukey = u[0] if u else rel
        for n, ln in enumerate(t.text.splitlines() or [t.text]):
            line = l0 + n
            if re.search(r"fake match", ln, re.I):
                notes[ukey]["fake match"] += 1
                notes[ukey]["fake match lines"] += 1
                prev_fake_line = line
            elif prev_fake_line == line - 1 and ln.strip().startswith("//") and not _MARK.match(ln) \
                    and not _CONT_STOP.match(ln):
                notes[ukey]["fake match lines"] += 1
                prev_fake_line = line
            if _ASORIG.search(ln):
                notes[ukey]["as original"] += 1
            for kw, rx in _KEYWORDS.items():                  # identification / contract words in comments
                n_kw = len(rx.findall(ln))
                if n_kw:
                    notes[ukey]["kw:" + kw] += n_kw
    res.notes = {k: v for k, v in notes.items() if v}

    # (b)+(c) tokens
    odd = []                                             # (offset, kind, msg)
    for i, t in enumerate(S):
        x = t.text
        if t.kind == "op" and x == "(" and _is_cast(S, M, i, typedef_names):
            add(t.start, "cast")
            if _raw_addr(S, M, i):
                add(t.start, "raw_addr")
            continue
        if t.kind != "id":
            continue
        if x == "reinterpret_cast":
            add(t.start, "reinterpret_cast")
        elif x in _CPP_CASTS:
            add(t.start, "cast")
        elif x == "goto":
            add(t.start, "goto")
            odd.append((t.start, "goto", f"goto {S[i + 1].text if i + 1 < len(S) else ''}"))
        elif x in ("__asm", "_asm", "asm"):
            add(t.start, "asm")
            odd.append((t.start, "asm", "__asm block"))
        elif x == "register":
            add(t.start, "register")
            odd.append((t.start, "register", "register keyword (banned)"))
        elif x in ("extern", "union", "volatile"):
            add(t.start, x)
        elif x == "do" and i + 6 < len(S) and [s.text for s in S[i + 1:i + 7]] == ["{", "}", "while", "(", "0", ")"]:
            add(t.start, "barrier")
            odd.append((t.start, "barrier", "empty do {} while (0) barrier"))
    for key, lead, st, end, ann, f in units:
        bi = next((k for k, s in enumerate(S) if s.start == f.body_start), None)
        if bi is None:
            continue
        for k, name in _unused_locals(S, M, bi + 1, M[bi], raii):
            add(S[k].start, "unused_local")
            odd.append((S[k].start, "unused_local", f"local `{name}` is never used"))

    for off, kind, msg in sorted(odd):
        line = L.of(off)
        u = unit_at(off)
        ukey = u[0] if u else rel
        rx = () if kind in BANNED else (_FAKE, _ASORIG) if kind in SHAPE_OK else (_FAKE,)
        above = line - 1                                  # contiguous comment-only lines directly above
        while above >= 1 and above not in code_lines and L.get(above).strip() and \
                any(l0 <= above <= l1 for l0, l1, _ in comments):
            above -= 1
        covered = any(_comment_reason(comments, above + 1, line, r) for r in rx)
        if not covered and u:
            lead_lo, lead_hi = L.of(u[1]), L.of(u[2]) - 1
            covered = any(_comment_reason(comments, lead_lo, lead_hi, r) for r in rx)
        if not covered:
            counts[ukey]["unmarked"] += 1
        res.oddities.append((Finding("oddity", rel, line, ukey, msg), covered))
    res.units = {k: collections.Counter({m: v for m, v in c.items() if v}) for k, c in counts.items()}
    return res


# --- repo + floor --------------------------------------------------------------------------------------------

def sources(repo: Path):
    return sorted(p for p in (repo / "src").rglob("*") if p.suffix.lower() in (".c", ".cpp", ".h") and p.is_file())


def load_floor(path: Path | None):
    if not path or not path.is_file():
        return None
    d = json.loads(path.read_text(encoding="utf-8"))
    if d.get("schema_version") != SCHEMA:
        raise ValueError(f"{path}: schema_version {d.get('schema_version')!r}, expected {SCHEMA}")
    return d.get("units", {})


def write_floor(path: Path, units: dict, where: dict):
    """One unit per line (stable diffs): {"schema_version", "about", "metrics", "units": {key: {metric: n}}}."""
    rows = []
    for key in sorted(units, key=lambda k: (where.get(k, ""), k)):
        c = {m: units[key][m] for m in METRICS if units[key].get(m)}
        rows.append(f"  {json.dumps(key)}: {json.dumps(c)}")      # clean units too: {} is a floor of 0
    about = ("Readability ratchet floors for tools/compare/source_checks.py: counts per unit (function VA, "
             "path:Name, or file scope = path) may only go down. Lower with --update; raising needs --raise-floor.")
    text = ("{\n" f'  "schema_version": {SCHEMA},\n  "about": {json.dumps(about)},\n'
            f'  "metrics": {json.dumps(METRICS)},\n  "units": {{\n' + ",\n".join("  " + r for r in rows)
            + "\n  }\n}\n")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8", newline="\n")


def queue_match(queue: Path | None) -> dict:
    if not queue or not Path(queue).is_file():
        return {}
    out = {}
    for ln in Path(queue).read_text(encoding="utf-8").splitlines():
        if ln.strip():
            e = json.loads(ln)
            out[e["address"]] = (e.get("match") or {}).get("status")
    return out


@dataclass
class Report:
    files: list
    units: dict                  # key -> Counter (current)
    where: dict                  # key -> path
    floor: dict | None
    violations: list = field(default_factory=list)   # Finding
    lowered: list = field(default_factory=list)      # (key, metric, old, new)
    new_units: list = field(default_factory=list)
    gone: list = field(default_factory=list)

    @property
    def marker_errors(self):
        return [v for v in self.violations if v.kind == "marker"]

    def totals(self):
        t = collections.Counter()
        for c in self.units.values():
            t.update(c)
        return t

    def summary_lines(self) -> list[str]:
        t = self.totals()
        n_mark = sum(len(f.markers) for f in self.files)
        n_odd = sum(len(f.oddities) for f in self.files)
        kinds = collections.Counter(v.kind for v in self.violations)
        out = [f"source checks: {len(self.files)} files, {len(self.units)} units; markers {n_mark}; oddities {n_odd} "
               f"({t['unmarked']} unmarked); " + ", ".join(f"{m} {t[m]}" for m in METRICS if t[m] and m != "unmarked")]
        if self.floor is None:
            out.append("  ratchet: no floor file (create one with --raise-floor)")
        else:
            out.append(f"  ratchet: {kinds['ratchet']} raised, {len(self.lowered)} lowered (--update records them), "
                       f"{len(self.new_units)} new units, {len(self.gone)} gone")
        out.append(f"  violations: {len(self.violations)}" + (
            " (" + ", ".join(f"{k} {v}" for k, v in sorted(kinds.items())) + ")" if self.violations else ""))
        return out


def _why(f):
    return "not allowed, marked or not (RULES.md Fake matches)" if "(banned)" in f.msg else "no `fake match:` comment"


def check_repo(repo: Path, floor_path: Path | None = None, queue: Path | None = None, client: str | None = None,
               files=None) -> Report:
    repo = Path(repo).resolve()
    global _INCLUDE_DTORS
    _INCLUDE_DTORS = frozenset(n for h in (repo / "include").rglob("*.h")
                               for n in _DTOR.findall(h.read_text(encoding="utf-8", errors="replace")))
    results = []
    for p in (files or sources(repo)):
        p = Path(p).resolve()
        rel = p.relative_to(repo).as_posix() if p.is_relative_to(repo) else p.as_posix()
        results.append(scan_file(p, rel, client))
    units, where = {}, {}
    for fr in results:
        for k, c in fr.units.items():
            if k in units:                               # the same VA annotated twice: add up
                units[k] = units[k] + c
            else:
                units[k] = c
            where.setdefault(k, fr.path)
    floor = load_floor(floor_path)
    rep = Report(results, units, where, floor)
    for fr in results:
        rep.violations += fr.errors
    qm = queue_match(queue)
    for fr in results:
        for line, ukey, name, _ in fr.markers:
            if name in MATCH_MARKERS and qm.get(ukey) == "identical":
                rep.violations.append(Finding("marker", fr.path, line, ukey,
                                              f"@{name} is stale: the queue records this function as identical"))
    odd_by_unit = collections.defaultdict(list)
    for fr in results:
        for f, covered in fr.oddities:
            if not covered:
                odd_by_unit[f.unit].append(f)
    for key, c in units.items():
        line = next((fr.lines.get(key, 1) for fr in results if fr.path == where[key]), 1)
        old = None if floor is None else floor.get(key)
        if old is None:
            if floor is not None:
                rep.new_units.append(key)
            for f in odd_by_unit[key]:
                rep.violations.append(Finding("oddity", f.path, f.line, key,
                                              f"{f.msg}: {_why(f)} (new code)"))
            continue
        for m in METRICS:
            o, n = old.get(m, 0), c.get(m, 0)
            if n > o:
                rep.violations.append(Finding("ratchet", where[key], line, key, f"{m} {o} -> {n} (above its floor)"))
                if m == "unmarked":
                    for f in odd_by_unit[key]:
                        rep.violations.append(Finding("oddity", f.path, f.line, key,
                                                      f"{f.msg}: {_why(f)}"))
            elif n < o:
                rep.lowered.append((key, m, o, n))
    if floor is not None:
        rep.gone = sorted(k for k in floor if k not in units)
    return rep


def updated_floor(rep: Report, raise_floor=False) -> dict:
    if raise_floor or rep.floor is None:
        return {k: dict(c) for k, c in rep.units.items()}
    out = {}
    for k, c in rep.units.items():
        old = rep.floor.get(k)
        out[k] = dict(c) if old is None else {m: min(old.get(m, 0), c.get(m, 0)) for m in METRICS}
    return out


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("files", nargs="*", help="sources (default every src\\**\\*.c/.cpp/.h)")
    ap.add_argument("--repo", default=".")
    ap.add_argument("--client", help="only annotations of this client (default any)")
    ap.add_argument("--floor", help=f"floor file (default <repo>\\{FLOOR_NAME})")
    ap.add_argument("--queue", help="queue.jsonl for stale-marker checks (default <repo>\\queue\\queue.jsonl)")
    ap.add_argument("--json", help="write the full report as JSON")
    ap.add_argument("-v", "--verbose", action="store_true", help="also list every oddity and lowered count")
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--update", action="store_true", help="write lowered floors + new units (no violations only)")
    g.add_argument("--raise-floor", action="store_true", help="accept every current count as the floor")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    if not (repo / "src").is_dir():
        print(f"source_checks: {repo}\\src not found", file=sys.stderr)
        return 2
    floor_path = Path(a.floor) if a.floor else repo / FLOOR_NAME
    queue = Path(a.queue) if a.queue else repo / "queue" / "queue.jsonl"
    try:
        rep = check_repo(repo, floor_path, queue, a.client, a.files or None)
    except ValueError as e:
        print(f"source_checks: {e}", file=sys.stderr)
        return 2
    for v in rep.violations:
        print(f"  VIOLATION {v}")
    if a.verbose:
        for fr in rep.files:
            for f, covered in fr.oddities:
                print(f"  oddity {f}{'' if not covered else '  (marked)'}")
        for k, m, o, n in rep.lowered:
            print(f"  lowered {k} ({rep.where[k]}): {m} {o} -> {n}")
    print("\n".join(rep.summary_lines()))
    if a.json:
        Path(a.json).parent.mkdir(parents=True, exist_ok=True)
        Path(a.json).write_text(json.dumps({
            "totals": dict(rep.totals()),
            "files": {fr.path: dict(sum((rep.units[k] for k in fr.units), collections.Counter())) for fr in rep.files},
            "markers": [{"path": fr.path, "line": ln, "unit": u, "marker": n, "reason": r}
                        for fr in rep.files for ln, u, n, r in fr.markers],
            "oddities": [dict(f.__dict__, covered=c) for fr in rep.files for f, c in fr.oddities],
            "violations": [v.__dict__ for v in rep.violations],
            "lowered": rep.lowered, "new_units": rep.new_units, "gone": rep.gone}, indent=1), encoding="utf-8")
    if a.raise_floor or a.update:
        if a.update and rep.violations:
            print(f"source_checks: --update refused: {len(rep.violations)} violations (fix them, or accept the "
                  f"counts with --raise-floor after review)", file=sys.stderr)
            return 1
        write_floor(floor_path, updated_floor(rep, a.raise_floor), rep.where)
        print(f"  floor written: {floor_path}")
        return 0 if a.raise_floor else int(bool(rep.violations))
    return 1 if rep.violations else 0


if __name__ == "__main__":
    sys.exit(main())
