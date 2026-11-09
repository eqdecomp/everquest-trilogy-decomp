r"""Name registry: one table per client (trilogy, eqgfx, eqmain), VA -> canonical name, built from every place a
name lives, plus the conflicts between those places.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\readability\registry.py [--client trilogy|eqgfx|eqmain|all]
        [--out DIR] [--json F] [-v]

Where names live (each is a "sighting" of a VA):
  ann       `// FUNCTION: <CLIENT> 0x...` + the definition after it (the name the compiler sees)
  fixture   tests/unicorn/<prefix>/**/*.json  "function" (with "original" = the VA)
  queue     the client's queue file, `name`
  csv:<src> names/<names>.<src>.csv rows (confidence other than `certain` -> `_guess` suffix, as apply_names.py)
  define    `#define NAME EQ_CDECL/STDCALL/THISCALL/FASTCALL/METHOD/GLOBAL/ARRAY(..., 0xVA, ...)` in src/ and
            include/ (macro names: call/global helpers, not function names)
Also every VA token of an EQ_* macro or a hand `eqd_fn_/eqd_va_` extern, as spelled (letter case).

Canonical name, functions: the source definition, else the queue's (if not FUN_), else the best names/*.csv
row (apply_names.py PRIORITY), else the fixture's, else FUN_<va>. Globals: the most used `#define` name.

Conflicts (`--out DIR` writes <client>.tsv + <client>.conflicts.tsv; stdout gets the counts):
  define_names    a VA whose #defines carry 2+ different macro names (across files)
  spelling        a VA written in more than one letter case (each spelling is its own EQD_MATCH symbol)
  queue_fun       a function named in source but still FUN_ in the queue
  queue_differs   queue name and source name differ (both not FUN_)
  fixture_differs a fixture's "function" differs from the source name
  csv_unapplied   a names/*.csv name for a function that is still FUN_ in source
  ann_dup         one VA annotated twice in one client with different names
  name_dup        one function name (qualified) defined for 2+ VAs
  macro_shadows   a #define name equal to a function's simple name (the macro rewrites the definition)

Importable: build(repo, client) -> Registry.
"""
from __future__ import annotations

import argparse
import collections
import csv
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
for _p in (HERE.parent / "unicorn", HERE.parent / "compare", HERE.parent / "permute"):
    if str(_p) not in sys.path:
        sys.path.insert(0, str(_p))

from eqd_unicorn import annotations as ann  # noqa: E402
from eqd_unicorn import clients  # noqa: E402

CLIENT_IDS = ("trilogy", "eqgfx", "eqmain")
CSV_PRIORITY = ["manual", "fid", "eqclassic", "mac", "strings", "beta", "takp", "sibling", "trilogy"]
FN_MACROS = ("CDECL_CPP", "STDCALL_CPP", "CDECL", "STDCALL", "THISCALL", "FASTCALL", "METHOD")
EQ_MACRO = re.compile(r"\bEQ_(CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY)\s*\(")
EQD_SYM = re.compile(r"\beqd_(fn|va)_(0[xX][0-9a-fA-F]+)\b")
DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)(\([^)]*\))?[ \t]*(.*)$")
HEXVA = re.compile(r"0[xX][0-9a-fA-F]+")
FUN_RX = re.compile(r"^(?:.*::)?FUN_[0-9a-fA-F]{8}$")
DIRECT = re.compile(r"\(*\s*EQ_(?:CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY)\s*\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)\s*\)*")
_COMMENT_OR_LIT = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'', re.S)


def blank_comments(text: str) -> str:
    """Comments -> spaces (string literals kept; newlines kept so offsets and line numbers stay put)."""
    return _COMMENT_OR_LIT.sub(
        lambda m: m.group(0) if m.group(0)[0] in "\"'" else re.sub(r"[^\n]", " ", m.group(0)), text)


def macro_args(text: str, start: int) -> tuple[list[str], int]:
    """Top-level comma-separated arguments of the macro call whose '(' is at text[start - 1], and the offset
    one past its ')' (len(text) when unbalanced)."""
    depth, cur, args, i = 0, [], [], start
    while i < len(text):
        c = text[i]
        if c in "([{":
            depth += 1
        elif c in ")]}":
            if depth == 0:
                args.append("".join(cur).strip())
                return args, i + 1
            depth -= 1
        elif c == "," and depth == 0:
            args.append("".join(cur).strip())
            cur = []
            i += 1
            continue
        cur.append(c)
        i += 1
    return args, len(text)


def macro_va(kind: str, args: list[str]) -> str | None:
    """The VA token (as written) of an EQ_<kind>(...) call's arguments, or None (a macro parameter)."""
    if kind == "METHOD":                    # EQ_METHOD(n, ret, va, params, obj)
        args = args[1:]
    if len(args) < 2 or not HEXVA.fullmatch(args[1]):
        return None
    return args[1]


def is_fun(name: str | None) -> bool:
    return bool(name) and bool(FUN_RX.match(name))


def simple(name: str) -> str:
    return name.split("::")[-1]


@dataclass
class Sighting:
    source: str                 # ann | fixture | queue | csv:<src> | define
    name: str
    path: str = ""              # repo-relative
    line: int = 0
    kind: str = "fn"            # fn | global
    extra: dict = field(default_factory=dict)


@dataclass
class Entry:
    va: int
    kind: str = "fn"
    sightings: list = field(default_factory=list)
    spellings: collections.Counter = field(default_factory=collections.Counter)

    def by(self, source: str) -> list:
        return [s for s in self.sightings if s.source == source or (source == "csv" and s.source.startswith("csv:"))]

    @property
    def ann_name(self):
        a = self.by("ann")
        return a[0].name if a else None

    @property
    def queue_name(self):
        q = self.by("queue")
        return q[0].name if q else None

    @property
    def csv_best(self):
        rows = self.by("csv")
        if not rows:
            return None
        rank = {s: i for i, s in enumerate(CSV_PRIORITY)}
        return min(rows, key=lambda s: rank.get(s.source[4:], len(rank)))

    @property
    def define_names(self) -> collections.Counter:
        return collections.Counter(s.name for s in self.by("define"))

    @property
    def canonical(self) -> str:
        if self.kind == "global" and not self.by("ann"):
            d = self.define_names
            if d:
                return sorted(d.items(), key=lambda kv: (-kv[1], kv[0]))[0][0]
            return f"DAT_{self.va:08X}"
        if self.ann_name:
            return self.ann_name
        q = self.queue_name
        if q and not is_fun(q):
            return q
        c = self.csv_best
        if c:
            return c.name
        f = self.by("fixture")
        if f:
            return f[0].name
        return f"FUN_{self.va:08x}"

    @property
    def canonical_source(self) -> str:
        if self.kind == "global" and not self.by("ann"):
            return "define" if self.define_names else "none"
        if self.ann_name:
            return "ann"
        q = self.queue_name
        if q and not is_fun(q):
            return "queue"
        if self.csv_best:
            return self.csv_best.source
        if self.by("fixture"):
            return "fixture"
        return "none"


@dataclass
class Registry:
    client: str
    entries: dict = field(default_factory=dict)          # va -> Entry
    files: dict = field(default_factory=dict)            # rel -> text (sources scanned)

    def entry(self, va: int) -> Entry:
        if va not in self.entries:
            self.entries[va] = Entry(va)
        return self.entries[va]

    def name(self, va: int) -> str | None:
        e = self.entries.get(va)
        return e.canonical if e else None

    def lookup(self, name: str) -> list[int]:
        """VAs whose canonical name, source name or a define name is `name` (qualified or simple)."""
        out = []
        for va, e in self.entries.items():
            names = {e.canonical} | {s.name for s in e.sightings}
            if name in names or any(simple(n) == name for n in names if "::" in n):
                out.append(va)
        return sorted(out)

    def conflicts(self) -> list[tuple]:
        """[(kind, va or '', detail)]"""
        out = []
        fn_by_name = collections.defaultdict(set)
        for va, e in sorted(self.entries.items()):
            d = e.define_names
            if len(d) > 1:
                out.append(("define_names", va, " | ".join(f"{n} x{c}" for n, c in sorted(d.items()))))
            if len(e.spellings) > 1:
                out.append(("spelling", va, " | ".join(f"{s} x{c}" for s, c in sorted(e.spellings.items()))))
            an, qn = e.ann_name, e.queue_name
            if an and qn:
                if is_fun(qn) and not is_fun(an):
                    out.append(("queue_fun", va, f"source {an} / queue {qn}"))
                elif not is_fun(qn) and qn != an:
                    out.append(("queue_differs", va, f"source {an} / queue {qn}"))
            for f in e.by("fixture"):
                if an and f.name != an:
                    out.append(("fixture_differs", va, f"source {an} / fixture {f.name} ({f.path})"))
            c = e.csv_best
            if c and (an is None or is_fun(an)) and (an is not None or (qn and is_fun(qn))):
                out.append(("csv_unapplied", va, f"{an or qn} -> {c.name} ({c.source}, {c.extra.get('confidence')})"))
            anns = {s.name for s in e.by("ann")}
            if len(anns) > 1:
                out.append(("ann_dup", va, " | ".join(sorted(anns))))
            for n in anns:
                fn_by_name[n].add(va)
        for n, vas in sorted(fn_by_name.items()):
            if len(vas) > 1 and not is_fun(n):
                out.append(("name_dup", "", f"{n}: " + " ".join(f"{v:08X}" for v in sorted(vas))))
        simple_fns = collections.defaultdict(set)
        for va, e in self.entries.items():
            for s in e.by("ann"):
                simple_fns[simple(s.name)].add(va)
        for va, e in sorted(self.entries.items()):
            for s in e.by("define"):
                if s.name in simple_fns:
                    out.append(("macro_shadows", va, f"#define {s.name} ({s.path}:{s.line}) = the name of function "
                                + " ".join(f"{v:08X}" for v in sorted(simple_fns[s.name]))))
        return out

    def rows(self):
        for va, e in sorted(self.entries.items()):
            yield {"va": f"{va:08X}", "kind": e.kind, "name": e.canonical, "from": e.canonical_source,
                   "source_name": e.ann_name or "", "queue_name": e.queue_name or "",
                   "csv_name": (e.csv_best.name if e.csv_best else ""),
                   "defines": " ".join(sorted(e.define_names)),
                   "spellings": " ".join(sorted(e.spellings))}


# --- scanning -----------------------------------------------------------------------------------------------

def include_owner(rel: str) -> str:
    """Client of a header under include/: include/<prefix>/ -> that client, anything else -> trilogy."""
    parts = rel.split("/")
    if len(parts) > 2 and parts[0] == "include":
        for c in clients.CLIENTS.values():
            if c.prefix and c.prefix.lower() == parts[1].lower():
                return c.id
    return "trilogy"


def code_files(repo: Path, client: str) -> list[Path]:
    """The client's .c/.cpp/.h under src/ and its headers under include/."""
    repo = Path(repo)
    out = clients.sources(repo, client, suffixes=(".c", ".cpp", ".h"))
    inc = repo / "include"
    if inc.is_dir():
        out += [p for p in sorted(inc.rglob("*.h")) if include_owner(p.relative_to(repo).as_posix()) == client]
    return out


def scan_macros(text: str):
    """Yield (kind, va_token, start, end, macro_offset) for every EQ_* macro use with a hex VA (comments
    blanked), and ('sym', token, start, end, start) for every eqd_fn_/eqd_va_ name."""
    bt = blank_comments(text)
    for m in EQ_MACRO.finditer(bt):
        args, end = macro_args(bt, m.end())
        va = macro_va(m.group(1), args)
        if va:
            yield m.group(1), va, m.start(), end, m.start()
    for m in EQD_SYM.finditer(bt):
        yield "sym", m.group(2), m.start(2), m.end(2), m.start()


def scan_defines(text: str):
    """Yield (name, macro kind, va token, line, direct) for `#define NAME ... EQ_<kind>(..., 0xVA, ...)` lines
    whose body uses one VA (possibly wrapped: casts, parentheses, an offset, a function-like macro). `direct`:
    an object-like macro whose whole body is the one EQ_* use, i.e. another name for the VA itself."""
    lines = text.split("\n")
    i = 0
    while i < len(lines):
        ln = lines[i]
        start = i
        while ln.rstrip("\r").endswith("\\") and i + 1 < len(lines):
            i += 1
            ln = ln.rstrip("\r")[:-1] + " " + lines[i]
        m = DEFINE.match(ln)
        if m:
            body = blank_comments(m.group(3))
            uses = [(u.group(1), macro_va(u.group(1), macro_args(body, u.end())[0])) for u in EQ_MACRO.finditer(body)]
            uses = [(k, v) for k, v in uses if v]
            if len({int(v, 16) for _, v in uses}) == 1:
                direct = m.group(2) is None and DIRECT.fullmatch(body.strip()) is not None
                yield m.group(1), uses[0][0], uses[0][1], start + 1, direct
        i += 1


def build(repo, client: str) -> Registry:
    repo = Path(repo).resolve()
    c = clients.get(client)
    reg = Registry(client)
    files = code_files(repo, client)
    token = c.token
    for p in files:
        rel = p.relative_to(repo).as_posix()
        text = p.read_text(encoding="utf-8", errors="replace")
        reg.files[rel] = text
        for kind, va, *_ in scan_macros(text):
            e = reg.entry(int(va, 16))
            e.spellings[va] += 1
        for name, kind, va, line, direct in scan_defines(text):
            e = reg.entry(int(va, 16))
            k = "fn" if kind in FN_MACROS else "global"
            e.sightings.append(Sighting("define" if direct else "define_use", name, rel, line, k, {"macro": kind}))
        if p.suffix.lower() in (".c", ".cpp"):
            try:
                anns = ann.scan([p], token)
            except ValueError:
                anns = []
            for a in anns:
                e = reg.entry(a.va)
                e.sightings.append(Sighting("ann", a.name, rel, a.line))
    # kinds: a VA with an annotation or a function macro/define is a function, else a global
    fn_vas = set()
    for p in files:
        for kind, va, *_ in scan_macros(reg.files[p.relative_to(repo).as_posix()]):
            if kind in FN_MACROS:
                fn_vas.add(int(va, 16))
    for va, e in reg.entries.items():
        if e.by("ann") or va in fn_vas or any(s.kind == "fn" for s in e.by("define")):
            e.kind = "fn"
        else:
            e.kind = "global"
    # fixtures
    tdir = c.tests_dir(repo)
    if tdir.is_dir():
        for f in sorted(tdir.rglob("*.json")):
            rel = f.relative_to(repo).as_posix()
            if not clients.owns(client, rel):
                continue
            try:
                d = json.loads(f.read_text(encoding="utf-8"))
            except (ValueError, OSError):
                continue
            if not isinstance(d, dict) or "function" not in d or "original" not in d:
                continue
            try:
                va = int(str(d["original"]), 16)
            except ValueError:
                continue
            reg.entry(va).sightings.append(Sighting("fixture", d["function"], rel))
    # queue
    q = repo / c.queue
    if q.is_file():
        for n, line in enumerate(q.read_text(encoding="utf-8").splitlines(), 1):
            if not line.strip():
                continue
            d = json.loads(line)
            e = reg.entry(int(d["address"], 16))
            e.sightings.append(Sighting("queue", d.get("name") or "", c.queue, n, extra={"state": d.get("state")}))
            e.kind = "fn"
    # names csv
    nd = repo / "names"
    for f in sorted(nd.glob(f"{c.names}.*.csv")) if nd.is_dir() else []:
        rel = f.relative_to(repo).as_posix()
        with f.open(newline="", encoding="utf-8") as fh:
            for n, row in enumerate(csv.DictReader(fh), 2):
                try:
                    va = int(row["address"], 16)
                except (KeyError, ValueError, TypeError):
                    continue
                name = row["name"]
                if row.get("confidence") != "certain" and not name.endswith("_guess"):
                    name += "_guess"
                reg.entry(va).sightings.append(Sighting(f"csv:{row.get('source') or f.stem.split('.')[-1]}", name, rel,
                                                        n, extra={"confidence": row.get("confidence")}))
    return reg


def summary(regs: dict) -> str:
    lines = []
    tot = collections.Counter()
    for client, reg in regs.items():
        cnt = collections.Counter(k for k, *_ in reg.conflicts())
        tot.update(cnt)
        fns = sum(1 for e in reg.entries.values() if e.kind == "fn")
        lines.append(f"{client}: {len(reg.entries)} VAs ({fns} functions); conflicts: "
                     + ", ".join(f"{k} {v}" for k, v in sorted(cnt.items())))
    if len(regs) > 1:
        lines.append("all: " + ", ".join(f"{k} {v}" for k, v in sorted(tot.items())))
    return "\n".join(lines)


def write_tables(reg: Registry, out: Path):
    out.mkdir(parents=True, exist_ok=True)
    rows = list(reg.rows())
    cols = list(rows[0]) if rows else ["va", "name"]
    (out / f"{reg.client}.tsv").write_text(
        "\t".join(cols) + "\n" + "".join("\t".join(r[c] for c in cols) + "\n" for r in rows), encoding="utf-8")
    (out / f"{reg.client}.conflicts.tsv").write_text(
        "kind\tva\tdetail\n" + "".join(f"{k}\t{v if v == '' else f'{v:08X}'}\t{d}\n" for k, v, d in reg.conflicts()),
        encoding="utf-8")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", default=".")
    ap.add_argument("--client", default="all", choices=CLIENT_IDS + ("all",))
    ap.add_argument("--out", help="write <client>.tsv and <client>.conflicts.tsv here")
    ap.add_argument("--json", help="write {client: {va: canonical}} here")
    ap.add_argument("-v", "--verbose", action="store_true", help="print every conflict")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    regs = {c: build(repo, c) for c in (CLIENT_IDS if a.client == "all" else (a.client,))}
    print(summary(regs))
    if a.verbose:
        for client, reg in regs.items():
            for k, v, d in reg.conflicts():
                print(f"{client}\t{k}\t{v if v == '' else f'{v:08X}'}\t{d}")
    if a.out:
        for reg in regs.values():
            write_tables(reg, Path(a.out))
    if a.json:
        Path(a.json).write_text(json.dumps({c: {f"{va:08X}": e.canonical for va, e in sorted(r.entries.items())}
                                            for c, r in regs.items()}, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
