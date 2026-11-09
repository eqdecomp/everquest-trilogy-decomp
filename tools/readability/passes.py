r"""Scripted-pass report (readability R1, review §1 rec. 5): what each mechanical pass WOULD change. Report
only: nothing is written to the repo. Each pass then lands as its own commit, gated by gate.py.

    cd eq-trilogy
    ..\.venv\Scripts\python.exe tools\readability\passes.py [--out DIR] [--check-renames]
    ..\.venv\Scripts\python.exe tools\readability\passes.py --apply spelling|calls|defines [--dry-run]

--apply spelling writes pass (a): each VA token that (a) lists becomes `0x` + upper-case hex in place; nothing
else in the file changes (same length, same bytes otherwise). Gate it with gate.py and a full compare --match.
--apply calls writes (c) and --apply defines writes (e), each only where the preprocessed source provably stays
the same token for token (see the comment above plan_calls: exact #define bodies, visibility through
unconditional includes, comments/strings untouched, CRLF kept); --apply defines repeats until nothing is left.
Their report rows: <out>/c_apply.tsv (status apply/hidden/none/nested/multiline) and e_apply.tsv.

(a) spelling   one spelling per VA: tokens written in another letter case than `0x` + upper-case hex
               (EQ_* macro arguments and hand `eqd_fn_/eqd_va_` externs); the match header changes once.
(b) csv        names/*.csv names for functions still FUN_ in source; with --check-renames each one is
               planned with rename.py (dry run) and its refusal, if any, is listed.
(c) calls      literal-VA call/global macros inside function bodies whose VA has a registry name (function
               defined in source, or a non-FUN queue/csv name); split by how many prototypes the VA is
               called with (one canonical macro per VA is a blind substitution only for single-prototype VAs).
(d) opcodes    `case 0x...:` labels in EQHandleMsg.cpp whose value is an OP_* of include/trilogy/opcodes.h,
               grouped by switch: a switch whose cases are mostly opcodes is a candidate.
(e) defines    VAs with 2+ #define names (registry define_names): the merge candidates.

Writes <out>/passes.md (summary) and one TSV per pass.
"""
from __future__ import annotations

import argparse
import collections
import re
import sys
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import registry as R  # noqa: E402

import ctoken as ct  # noqa: E402
import tu  # noqa: E402

CANON = re.compile(r"0x[0-9A-F]+")
HEXVA = re.compile(r"0[xX][0-9a-fA-F]+")
CALL_KINDS = ("CDECL_CPP", "STDCALL_CPP", "CDECL", "STDCALL", "THISCALL", "FASTCALL", "METHOD")


def canonical_spelling(tok: str) -> str:
    return "0x" + tok[2:].upper()


def pass_spelling(reg: R.Registry) -> list[tuple]:
    rows = []
    idents, header_fns, fm = spelling_scan(reg.files.values())
    for rel, text in sorted(reg.files.items()):
        for s, e, new, kind in spelling_edits(text, idents, header_fns, fm):
            rows.append((reg.client, rel, text.count("\n", 0, s) + 1, text[s:e], new, kind))
    return rows


LITERAL = re.compile(r'"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'')


VA_MACRO = re.compile(r"\bEQ_(CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY|FUNC)\s*\(")
VA_ARG = {"METHOD": 2, "FUNC": 2}                  # index of the VA argument (default 1)
OBJ_DEFINE = re.compile(r"^([ \t]*#[ \t]*define[ \t]+)([A-Za-z_]\w*)([ \t]+)(0[xX][0-9a-fA-F]+)([ \t]*(?://[^\r\n]*|/\*[^\r\n]*)?)\r?$",
                        re.M)
HAND_EXTERN = re.compile(r'^[ \t]*extern\b[^;\n]*\beqd_fn_(0[xX][0-9a-fA-F]+)[ \t]*\(([^)]*)\)[ \t]*;[ \t]*\r?\n', re.M)
EMPTY_IFDEF = re.compile(r"^#ifdef EQD_MATCH[ \t]*\r?\n#endif[ \t]*\r?\n", re.M)


def _code(text: str) -> str:
    """Comments and string/char literals blanked (same length, newlines kept)."""
    bt = R.blank_comments(text)
    return LITERAL.sub(lambda m: m.group(0)[0] + " " * (len(m.group(0)) - 2) + m.group(0)[-1], bt)


def _va_args(bt: str):
    """Yield (macro kind, VA argument as written, its offset) for every EQ_* macro use with a VA argument."""
    for m in VA_MACRO.finditer(bt):
        kind = m.group(1)
        args, end = R.macro_args(bt, m.end())
        k = VA_ARG.get(kind, 1)
        if len(args) <= k:
            continue
        depth, i, commas = 0, m.end(), 0           # walk to the k-th top-level comma, then to the token
        while i < end and commas < k:
            c = bt[i]
            if c in "([{":
                depth += 1
            elif c in ")]}":
                depth -= 1
            elif c == "," and depth == 0:
                commas += 1
            i += 1
        j = bt.find(args[k], i, end)
        if j < 0 or bt[i:j].strip():
            raise ValueError(f"VA argument {args[k]} not where expected at offset {m.start()}")
        yield kind, args[k], j


FN_DEFINE = re.compile(r"^[ \t]*#[ \t]*define[ \t]+([A-Za-z_]\w*)\(([^)]*)\)((?:[^\n]*\\\r?\n)*[^\n]*)", re.M)
# eqaddr.h's own definitions (EQ_CDECL(ret, va, params) -> EQ_FUNC(...)): never "a macro that forwards a VA"; the
# VA_MACRO scan already sees their uses, and learning them too gave every EQ_CDECL token twice (crash:
# 'overlapping VA tokens', report mode only, since all_code_files skips eqaddr.h but Registry.files has it).
VA_MACRO_NAME = re.compile(r"EQ_(?:CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY|FUNC)")


def _macro_calls(bt: str, names):
    """Yield (name, args, [arg offsets]) for every call NAME(...) of a function-like macro in `names`."""
    if not names:
        return
    rx = re.compile(r"\b(" + "|".join(sorted(map(re.escape, names), key=len, reverse=True)) + r")\s*\(")
    for m in rx.finditer(bt):
        line_start = bt.rfind("\n", 0, m.start()) + 1
        if re.match(r"[ \t]*#[ \t]*define[ \t]+" + re.escape(m.group(1)) + r"\(", bt[line_start:m.end()]):
            continue                                   # the definition itself
        args, end = R.macro_args(bt, m.end())
        offs, depth, i, start = [], 0, m.end(), m.end()
        while i < end - 1:
            c = bt[i]
            if c in "([{":
                depth += 1
            elif c in ")]}":
                depth -= 1
            elif c == "," and depth == 0:
                offs.append(start)
                start = i + 1
            i += 1
        offs.append(start)
        yield m.group(1), args, offs


def spelling_scan(texts) -> tuple[set, set, dict]:
    """Over all files: (identifiers used as a VA argument, so `#define NAME 0x...` is a VA too; canonical VAs the
    generated match header declares as functions: EQ_CDECL/STDCALL/THISCALL/FASTCALL/METHOD with a hex VA;
    function-like macros whose parameter ends up as a VA argument: {name: {argument index}})."""
    idents, header_fns = set(), set()
    codes = [_code(t) for t in texts]
    defs = []                                          # (name, [params], body)
    for bt in codes:
        for kind, arg, _ in _va_args(bt):
            if HEXVA.fullmatch(arg):
                if kind in CALL_KINDS:
                    header_fns.add(canonical_spelling(arg))
            elif re.fullmatch(r"[A-Za-z_]\w*", arg):
                idents.add(arg)
        for m in FN_DEFINE.finditer(bt):
            if VA_MACRO_NAME.fullmatch(m.group(1)):
                continue
            defs.append((m.group(1), [x.strip() for x in m.group(2).split(",")], m.group(3)))
    fmacros: dict = {}
    for _ in range(3):                                 # macros that pass a VA parameter on to another macro
        for name, params, body in defs:
            vas = {arg for _, arg, _ in _va_args(body)}
            for mname, args, _ in _macro_calls(body, set(fmacros)):
                vas |= {a for k, a in enumerate(args) if k in fmacros[mname]}
            for k, prm in enumerate(params):
                if prm in vas:
                    fmacros.setdefault(name, set()).add(k)
    return idents, header_fns, fmacros


def spelling_edits(text: str, va_idents=frozenset(), header_fns=frozenset(), fmacros=None) -> list[tuple[int, int, str, str]]:
    """[(start, end, replacement, kind)] for every VA token of (a) in `text` not yet spelled `0x` + upper-case
    hex: the VA argument of EQ_* macros (EQ_FUNC included), the hex part of eqd_fn_/eqd_va_ names, and the value
    of `#define NAME 0x...` when NAME is used as a VA argument somewhere (`va_idents`). Comments and strings are
    untouched. Kind 'extern' deletes a hand `extern ... eqd_fn_<VA>(params);` with a prototype other than
    `(void)` whose VA the match header declares too (`header_fns`): once both are spelled alike they clash
    (C2733, a second C linkage), and the header's declaration is all a cast call needs."""
    bt = _code(text)
    out = []
    for kind, arg, j in _va_args(bt):
        if HEXVA.fullmatch(arg) and not CANON.fullmatch(arg):
            out.append((j, j + len(arg), canonical_spelling(arg), kind))
    for name, args, offs in _macro_calls(bt, set(fmacros or ())):
        for k in fmacros[name]:
            if k < len(args) and HEXVA.fullmatch(args[k]) and not CANON.fullmatch(args[k]):
                j = bt.find(args[k], offs[k])
                out.append((j, j + len(args[k]), canonical_spelling(args[k]), "macro:" + name))
    for m in OBJ_DEFINE.finditer(bt):
        if m.group(2) in va_idents and not CANON.fullmatch(m.group(4)):
            out.append((m.start(4), m.end(4), canonical_spelling(m.group(4)), "define"))
    dropped = []
    for m in HAND_EXTERN.finditer(bt):
        if canonical_spelling(m.group(1)) in header_fns and m.group(2).strip() not in ("void", ""):
            dropped.append((m.start(), m.end()))
            out.append((m.start(), m.end(), "", "extern"))
    for m in R.EQD_SYM.finditer(bt):
        va = m.group(2)
        if not CANON.fullmatch(va) and not any(a <= m.start() < b for a, b in dropped):
            out.append((m.start(2), m.end(2), canonical_spelling(va), "sym"))
    out.sort()
    uniq = []                                          # one token found twice (same span, same new text): one edit
    for x in out:
        if not (uniq and uniq[-1][:3] == x[:3]):
            uniq.append(x)
    out = uniq
    for a, b in zip(out, out[1:]):
        if a[1] > b[0]:
            raise ValueError(f"overlapping VA tokens at {a[0]}/{b[0]}")
    return out


def all_code_files(repo: Path) -> list[Path]:
    """What compare.match_symbols scans: every .h/.c/.cpp under include/ and src/ (eqaddr.h aside)."""
    return sorted(p for d in ("include", "src") for p in (repo / d).rglob("*")
                  if p.suffix.lower() in (".h", ".c", ".cpp") and p.is_file() and p.name != "eqaddr.h")


# Files pass (a) leaves alone: VC6 compiles them differently when only a symbol name's spelling changes (its
# optimiser's order depends on the names), so a spelling edit is a codegen change there.
SPELLING_KEEP = {
    "src/EverQuest/EQ_MainDraw.cpp": "0043B010 (85 KB giant) schedules differently after any one VA token "
                                     "changes case (bisected: 0x006b341c alone moves code, +10 bytes)",
}


def apply_spelling(repo: Path, regs: dict | None = None, dry_run: bool = False,
                   keep=tuple(SPELLING_KEEP)) -> dict[str, int]:
    """Rewrite pass (a) in place over every code file of the repo. Returns {rel: edits}. Token edits keep the
    length; a dropped hand extern (see spelling_edits) removes its line, and an `#ifdef EQD_MATCH` / `#endif`
    pair left empty by it goes too."""
    repo = Path(repo)
    files = all_code_files(repo)
    texts = {p: p.read_bytes().decode("utf-8", errors="surrogateescape") for p in files}
    idents, header_fns, fm = spelling_scan(texts.values())
    done = {}
    for p in files:
        if p.relative_to(repo).as_posix() in keep:
            continue
        text = texts[p]
        edits = spelling_edits(text, idents, header_fns, fm)
        if not edits:
            continue
        done[p.relative_to(repo).as_posix()] = len(edits)
        if dry_run:
            continue
        parts, last = [], 0
        for s_, e_, new, kind in edits:
            assert kind == "extern" or text[s_:e_].upper() == new.upper()
            parts.append(text[last:s_])
            parts.append(new)
            last = e_
        parts.append(text[last:])
        out = "".join(parts)
        if any(e[3] == "extern" for e in edits):
            out = EMPTY_IFDEF.sub("", out)
        else:
            assert len(out) == len(text)
        p.write_bytes(out.encode("utf-8", errors="surrogateescape"))
    return done


def pass_csv(repo: Path, reg: R.Registry, check: bool) -> list[tuple]:
    rows = []
    if check:
        import rename as rn
    for va, e in sorted(reg.entries.items()):
        an = e.ann_name
        c = e.csv_best
        if not (c and an and R.is_fun(an)):
            continue
        cls_src = an.rsplit("::", 1)[0] if "::" in an else ""
        cls_csv = c.name.rsplit("::", 1)[0] if "::" in c.name else ""
        new = R.simple(c.name)              # rename.py keeps the class; a different class is a note
        verdict = "" if cls_src == cls_csv else f"class differs: source {cls_src or '(none)'}, csv {cls_csv or '(none)'}; "
        if check:
            try:
                rn.plan_rename(repo, reg.client, f"0x{va:08X}", new, reg=reg)
                verdict += "ok"
            except rn.Refused as ex:
                verdict += f"REFUSED: {ex}"
        rows.append((reg.client, f"{va:08X}", an, new, c.source[4:], c.extra.get("confidence") or "", verdict))
    return rows


def _bodies(text: str):
    """[(start, end)] of function bodies."""
    try:
        return [(f.body_start, f.body_end) for f in tu.find_functions(text)]
    except (ValueError, KeyError, IndexError):
        return []


def pass_calls(reg: R.Registry) -> tuple[list[tuple], dict]:
    protos = collections.defaultdict(set)
    uses = []
    for rel, text in sorted(reg.files.items()):
        bt = R.blank_comments(text)
        bodies = _bodies(text)
        starts = [b[0] for b in bodies]
        import bisect
        for m in R.EQ_MACRO.finditer(bt):
            args, end = R.macro_args(bt, m.end())
            va = R.macro_va(m.group(1), args)
            if not va:
                continue
            k = bisect.bisect_right(starts, m.start()) - 1
            in_body = k >= 0 and bodies[k][0] <= m.start() < bodies[k][1]
            v = int(va, 16)
            if m.group(1) in CALL_KINDS:
                a = args[1:] if m.group(1) == "METHOD" else args
                protos[v].add(re.sub(r"\s+", "", a[0] + (a[2] if len(a) > 2 else "")))
            if in_body:
                uses.append((rel, text.count("\n", 0, m.start()) + 1, m.group(1), v))
    rows = []
    for rel, line, kind, v in uses:
        e = reg.entries.get(v)
        name = e.canonical if e else ""
        named = bool(e) and not R.is_fun(name) and not name.startswith("DAT_")
        rows.append((reg.client, rel, line, kind, f"{v:08X}", name if named else "", len(protos.get(v, ())) or "",
                     "global" if kind in ("GLOBAL", "ARRAY") else "call"))
    return rows, protos


# --- (c) calls/globals and (e) defines: the apply side -------------------------------------------------------
#
# Both passes only ever write one macro name in place of another spelling of the SAME token sequence:
#   (c) `EQ_CDECL(int, 0x00401100, (int))(2)` -> `WidgetTotal(2)` where `#define WidgetTotal EQ_CDECL(int,
#       0x00401100, (int))`: the define's body is exactly the literal's tokens (VA letter case included: each
#       spelling is its own EQD_MATCH symbol; no added or dropped parentheses either);
#   (e) alias `#define B EQ_GLOBAL(int, 0x006E0020)` -> the kept name A with a token-identical body; B's
#       #define goes once nothing uses it any more.
# So the preprocessed translation unit, and the match header generated from the VA tokens (they stay in the
# kept #define), do not change. A name qualifies only when it is safe to expand anywhere: object-like, the
# same body wherever it is defined, never #undef'd, not inside #if/#ifdef (an include guard aside), and
# visible at the use: defined earlier in the same file or in a header reached through unconditional
# `#include "..."` lines before the use (resolved like CL /I<repo>/include: the including file's folder, then
# include/). Edits never touch comments or string literals and never span a line break, so CRLF/LF stay as
# they are; a deleted #define takes its own line ending with it.

PP_LINE = re.compile(r"^[ \t]*#[ \t]*([A-Za-z_]\w*)((?:[^\n]*\\\r?\n)*[^\n]*)", re.M)
DEF_HEAD = re.compile(r"[ \t]*([A-Za-z_]\w*)(\([^)]*\))?")
INCLUDE_Q = re.compile(r'[ \t]*"([^"\r\n]+)"')      # used with match(text, pos): no ^
INCLUDE_A = re.compile(r'[ \t]*<([^>\r\n]+)>')
WORD = re.compile(r"[A-Za-z_]\w*")
EQ_HEAD = re.compile(r"EQ_(CDECL_CPP|STDCALL_CPP|CDECL|STDCALL|THISCALL|FASTCALL|METHOD|GLOBAL|ARRAY)")
# Where a macro name may still be used outside src/ and include/: a name found there keeps its #define.
EXTERNAL_DIRS = ("hooks", "tests", "ghidra-types", "reference")
EXTERNAL_SUFFIXES = {".c", ".cpp", ".h", ".inl", ".py", ".json", ".def", ".ini", ".bat", ".txt", ".cmd"}


def _toks(s: str) -> tuple:
    """Significant tokens (continuation backslashes and whitespace dropped)."""
    s = re.sub(r"\\\r?\n", " ", s)
    return tuple(t.text for t in ct.significant(ct.tokenize(s)))


def _direct_key(body: tuple) -> tuple | None:
    """The tokens of the one EQ_* use a define body consists of, when that use has a literal hex VA; else None.
    Exact tokens only: a body inside parentheses, `(EQ_GLOBAL(int, 0x..))`, has no key (review #1: the
    rewrite would add or remove the pair, and EQ_METHOD's expansion is not parenthesised, so `&A` or
    `sizeof A` could change meaning; the preprocessed text would differ in any case)."""
    t = body
    if len(t) < 4 or not EQ_HEAD.fullmatch(t[0]) or t[1] != "(" or t[-1] != ")":
        return None
    depth = 0
    for i, x in enumerate(t[1:], 1):
        depth += x in ("(", "[", "{")
        depth -= x in (")", "]", "}")
        if depth == 0 and i < len(t) - 1:
            return None                                # EQ_X(...) followed by more: (EQ_X(...))[1], an offset
    args, _ = R.macro_args(" ".join(t[2:]), 0)
    return t if R.macro_va(t[0][3:], args) else None


def _key_va(key: tuple) -> int:
    args, _ = R.macro_args(" ".join(key[2:]), 0)
    return int(R.macro_va(key[0][3:], args), 16)


def _guard_closes_file(code: str, dirs: list) -> bool:
    """Do the first two directives `#ifndef X` / `#define X` form an include guard: X the same name, nothing
    after it on the #define line, and the #endif matching the #ifndef the last directive with only whitespace
    after it (review #9: `#ifndef X` / `#define X` / real defines / `#endif` / more code is no guard)."""
    if len(dirs) < 2 or dirs[0][0] != "ifndef" or dirs[1][0] != "define":
        return False
    g = WORD.search(code, dirs[0][3], dirs[0][2])
    h = DEF_HEAD.match(code, dirs[1][3])
    if not (g and h and g.group(0) == h.group(1) and not code[h.end():dirs[1][2]].strip()):
        return False
    depth = 0
    for i, (d, _, e, _) in enumerate(dirs):
        if d in ("if", "ifdef", "ifndef"):
            depth += 1
        elif d == "endif":
            depth -= 1
            if depth == 0:
                return i == len(dirs) - 1 and not code[e:].strip()
    return False


def _one_line(text: str, s: int, e: int, multi: list, mstarts: list) -> bool:
    """Is the directive text[s:e] one physical line that can be dropped whole: no continuation, no `//`
    comment ending in a backslash (C splices it onto the next line), and no /* */ comment that crosses one of
    its ends (`multi`: the comments that span a line break, sorted, `mstarts` their starts)."""
    import bisect
    line = text[s:e]
    if "\n" in line or line.rstrip("\r").endswith("\\"):
        return False
    # comments do not overlap and each of `multi` holds a newline, which the line does not: only the one
    # crossing s and the one crossing e can touch it, the last two starting before e
    k = bisect.bisect_left(mstarts, e)
    return not any(ce > s for _, ce in multi[max(0, k - 2):k])


@dataclass
class _Def:
    name: str
    params: str | None
    body: tuple
    start: int          # offset of the line
    name_at: int        # offset of the name
    end: int            # one past the line's newline
    top: bool           # not inside #if/#ifdef (the include guard aside)
    single: bool        # one physical line


class _Src:
    """One file: raw text (CRLF kept), comments blanked (`bc`), comments and literals blanked (`code`), its
    preprocessor lines, #defines, #undefs and quoted #includes."""

    def __init__(self, rel: str, text: str):
        self.rel, self.text = rel, text
        self.bc = R.blank_comments(text)
        self.code = _code(text)
        self.defs, self.undefs, self.includes, self.angles, self.pp = [], set(), [], [], []
        dirs = [(m.group(1), m.start(), m.end(), m.start(2)) for m in PP_LINE.finditer(self.code)]
        guard = 0 if _guard_closes_file(self.code, dirs) else None
        # comments that cross a line break (review #2): a #define line that opens, closes or sits inside one
        # is not "one physical line" (dropping it would leave ` more */` behind, or swallow a `*/`)
        multi = [(m.start(), m.end()) for m in R._COMMENT_OR_LIT.finditer(text)
                 if m.group(0)[:2] == "/*" and "\n" in m.group(0)]
        mstarts = [c[0] for c in multi]
        stack = []                                     # one bool per open #if: is it the include guard
        for i, (d, s, e, rest) in enumerate(dirs):
            top = not any(not g for g in stack)
            self.pp.append((s, e))
            if d in ("if", "ifdef", "ifndef"):
                stack.append(i == guard)
            elif d == "endif":
                if stack:
                    stack.pop()
            elif d in ("else", "elif"):
                if stack:
                    stack[-1] = False
            elif d == "define":
                h = DEF_HEAD.match(self.code, rest)
                if not h:
                    continue
                end = e + 1 if e < len(text) and text[e] == "\n" else e
                self.defs.append(_Def(h.group(1), h.group(2), _toks(self.bc[h.end():e]), s, h.start(1), end, top,
                                      _one_line(text, s, e, multi, mstarts)))
            elif d == "undef":
                w = WORD.search(self.code, rest, e)
                if w:
                    self.undefs.add(w.group(0))
            elif d == "include":
                m = INCLUDE_Q.match(text, rest)        # the raw text: `code` blanks the quoted path
                if m:
                    self.includes.append((s, m.group(1), top))
                else:
                    m = INCLUDE_A.match(text, rest)
                    if m:
                        self.angles.append((s, m.group(1)))
        self._pp_starts = [p[0] for p in self.pp]
        self._fn = None

    def pp_at(self, pos: int):
        """The (start, end) of the preprocessor line holding `pos`, or None."""
        import bisect
        k = bisect.bisect_right(self._pp_starts, pos) - 1
        return self.pp[k] if k >= 0 and self.pp[k][0] <= pos < self.pp[k][1] else None

    def bodies(self):
        if self._fn is None:
            self._fn = _bodies(self.text)
        return self._fn

    def line(self, pos: int) -> int:
        return self.text.count("\n", 0, pos) + 1


class _Tree:
    """Every file a #define can come from or a macro name can be used in: the code files (src/, include/) and
    the C sources of hooks/ and tests/ (read only: their definitions count for 'one body everywhere').
    `texts` ({rel: text}) stands in for the files on disk (the report's in-memory rounds)."""

    def __init__(self, repo: Path, files=None, texts: dict | None = None):
        self.repo = Path(repo)
        files = list(files) if files is not None else all_code_files(self.repo)
        self.code_rels = [p.relative_to(self.repo).as_posix() for p in files]
        extra = [p for d in ("hooks", "tests") if (self.repo / d).is_dir() for p in sorted((self.repo / d).rglob("*"))
                 if p.suffix.lower() in (".h", ".c", ".cpp") and p.is_file()]
        self.src = {}
        for p in files + [p for p in extra if p not in files]:
            rel = p.relative_to(self.repo).as_posix()
            text = texts.get(rel) if texts else None
            if text is None:
                text = p.read_bytes().decode("utf-8", errors="surrogateescape")
            self.src[rel] = _Src(rel, text)
        self.lower = {rel.lower(): rel for rel in self.src}
        self.forms = collections.defaultdict(set)      # name -> {(params, body, top)}
        self.undefs = set()
        for s in self.src.values():
            self.undefs |= s.undefs
            for d in s.defs:
                self.forms[d.name].add((d.params, d.body, d.top))
        self.hnames = {rel: {d.name for d in s.defs} for rel, s in self.src.items()}
        self._clos, self._heads, self._first, self._words, self._included, self._reach = {}, {}, {}, {}, None, {}
        self._base = None

    def stable(self, name: str) -> tuple | None:
        """The body of an object-like macro that means the same everywhere (see above), else None."""
        f = self.forms.get(name)
        if not f or len(f) != 1 or name in self.undefs:
            return None
        params, body, top = next(iter(f))
        return body if params is None and top else None

    def resolve(self, frm: str, inc: str) -> str | None:
        """The repo file `#include "inc"` in `frm` opens: its own folder, then the /I folders of the build that
        compiles it (include/; the hooks build adds hooks/decomp)."""
        import posixpath
        extra = ("hooks/decomp/" + inc,) if frm.startswith("hooks/") else ()
        for c in (posixpath.join(posixpath.dirname(frm), inc), "include/" + inc) + extra:
            r = self.lower.get(posixpath.normpath(c).lower())
            if r:
                return r
        return None

    def closure(self, rel: str) -> frozenset:
        """`rel` and every header it includes (unconditionally), transitively."""
        if rel in self._clos:
            return self._clos[rel]
        seen, todo = {rel}, [rel]
        while todo:
            r = todo.pop()
            if r in self._clos:
                seen |= self._clos[r]
                continue
            for _, inc, top in self.src[r].includes:
                t = self.resolve(r, inc) if top else None
                if t and t not in seen:
                    seen.add(t)
                    todo.append(t)
        self._clos[rel] = frozenset(seen)
        return self._clos[rel]

    def visible(self, rel: str, pos: int, name: str) -> bool:
        """Is `name` defined (by an unconditional #define) before offset `pos` of `rel` in every TU?"""
        import bisect
        s = self.src[rel]
        first = self._first.setdefault(rel, {})
        if not first:
            for d in s.defs:
                first.setdefault(d.name, d.start)
            first[""] = 0
        if first.get(name, len(s.text)) < pos:
            return True
        if rel not in self._heads:                      # [(include offset, headers reached up to it)]
            acc, out = frozenset(), []
            for off, inc, top in s.includes:
                t = self.resolve(rel, inc) if top else None
                if t:
                    acc = acc | self.closure(t)
                out.append((off, acc))
            self._heads[rel] = out
        hs = self._heads[rel]
        k = bisect.bisect_left([h[0] for h in hs], pos) - 1
        return k >= 0 and any(name in self.hnames[h] for h in hs[k][1])

    def local_ok(self, rel: str, a: str, keep: str, poss) -> bool:
        """May every occurrence of macro `a` in `rel` (offsets `poss`) be renamed to `keep`? Only in a .c/.cpp
        that no file includes (its macros reach no other TU), whose first occurrence of `a` is its own #define
        (so no use relies on a header's), where `keep` is not a word anywhere in the TU (the file and every
        header it includes, conditionally or not: no clash, no second definition) and no occurrence of `a`
        sits next to `#`/`##` (a parameter of that name would stringize differently)."""
        if not self.is_root(rel):
            return False
        s = self.src[rel]
        first = min(poss)
        if not any(d.name == a and d.name_at == first for d in s.defs):
            return False
        if keep in self.tu_words(rel):
            return False
        for p in poss:
            if s.code[:p].rstrip().endswith("#") or s.code[p + len(a):].lstrip().startswith("##"):
                return False
        return True

    def is_root(self, rel: str) -> bool:
        """A .c/.cpp that no file includes: a translation unit of its own, nothing else sees its text."""
        if self._included is None:
            self._included = {t for r, s in self.src.items() for _, inc, _ in s.includes
                              for t in [self.resolve(r, inc)] if t}
        return rel.lower().endswith((".c", ".cpp")) and rel not in self._included

    def reach(self, rel: str) -> set:
        """`rel` and every file it reaches through any quoted #include (conditional ones too)."""
        if rel not in self._reach:
            seen, todo = {rel}, [rel]
            while todo:
                r = todo.pop()
                for _, inc, _ in self.src[r].includes:
                    t = self.resolve(r, inc)
                    if t and t not in seen:
                        seen.add(t)
                        todo.append(t)
            self._reach[rel] = seen
        return self._reach[rel]

    def tu_words(self, rel: str) -> set:
        """Identifiers of `rel` and of every header it reaches through any quoted #include."""
        if rel not in self._words:
            self._words[rel] = set().union(*(set(WORD.findall(self.src[r].code)) for r in self.reach(rel)))
        return self._words[rel]

    def never_macro(self, rel: str, name: str) -> bool:
        """Is `name` certainly NOT a macro anywhere in `rel`? Only for a TU root (a header's text can follow a
        #define of the name in whatever TU includes it) that neither defines it nor reaches a file that does,
        and whose includes are all modelled (see `opaque`)."""
        return self.is_root(rel) and not any(name in self.hnames[r] or self.opaque(r) for r in self.reach(rel))

    def opaque(self, rel: str) -> bool:
        """Can `rel` pull in repo text the quoted-include graph does not follow (review #3)? An angle include
        that names a repo header (CL finds it through /I), or a quoted include that does not resolve while a
        repo file has its file name (MSVC also searches the folders of the files that included this one)."""
        if self._base is None:
            import posixpath
            self._base = {posixpath.basename(r).lower() for r in self.src}
        s = self.src[rel]
        if any(self.resolve(rel, inc) for _, inc in s.angles):
            return True
        return any(self.resolve(rel, inc) is None and inc.replace("\\", "/").rsplit("/", 1)[-1].lower() in self._base
                   for _, inc, _ in s.includes)

    def direct_defines(self) -> dict:
        """{EQ_* use tokens: {names}} over the stable defines whose body is that one literal-VA use."""
        out = collections.defaultdict(set)
        for name in self.forms:
            b = self.stable(name)
            k = _direct_key(b) if b else None
            if k:
                out[k].add(name)
        return out

    def uses(self) -> collections.Counter:
        c = collections.Counter()
        for rel in self.code_rels:
            c.update(WORD.findall(self.src[rel].code))
        return c


def _external_words(repo: Path) -> set:
    out = set()
    for p in sorted(repo.iterdir()):
        if p.is_file() and p.suffix.lower() in EXTERNAL_SUFFIXES | {".md"}:
            out.update(WORD.findall(p.read_text(encoding="utf-8", errors="replace")))
    for d in EXTERNAL_DIRS:
        if (repo / d).is_dir():
            for p in (repo / d).rglob("*"):
                if p.suffix.lower() in EXTERNAL_SUFFIXES and p.is_file() and "__pycache__" not in p.parts:
                    out.update(WORD.findall(p.read_text(encoding="utf-8", errors="replace")))
    return out


def _shadow_names(regs: dict) -> set:
    return {R.simple(s.name) for reg in regs.values() for e in reg.entries.values() for s in e.by("ann")}


HEX_STAMP = re.compile(r"_(?:0[xX])?(?=[0-9A-Fa-f]*\d)[0-9A-Fa-f]{6,8}$")    # PTR_GetThreadPriority_100400e8


def _registry_names(regs: dict, clients, va: int) -> set:
    """The names the registry gives VA `va` other than its #define names (those are what is being merged): the
    queue's (not FUN_), every names/*.csv row's, and a canonical name that comes from neither a #define nor a
    FUN_/DAT_ default. `clients`: the clients whose files define the names (eqmain and eqgfx share VAs)."""
    out = set()
    for c in clients or (regs or {}):
        e = (regs or {}).get(c) and regs[c].entries.get(va)
        if not e:
            continue
        out |= {s.name for s in e.by("csv")}
        if e.queue_name and not R.is_fun(e.queue_name):
            out.add(e.queue_name)
        if e.canonical_source not in ("define", "none", "ann"):
            out.add(e.canonical)
    return out


def _pick(names, uses: collections.Counter, shadows: set, headers: set = frozenset(),
          preferred: set = frozenset()) -> str:
    """The name a group keeps (review #4: by meaning first, not by use count): not a function's name (the
    macro would rewrite that definition), then a name the registry gives the VA (`preferred`: names/*.csv,
    the queue), then one without a hex stamp (`_100400e8`), then one defined in a header (`headers`: other TUs
    can see it), then the most used, then the first alphabetically."""
    return sorted(names, key=lambda n: (n in shadows, n not in preferred, bool(HEX_STAMP.search(n)), n not in headers,
                                        -uses[n], n))[0]


def _client_of(regs: dict) -> dict:
    return {rel: c for c, reg in (regs or {}).items() for rel in reg.files}


def plan_calls(repo: Path, regs: dict | None = None, tree: _Tree | None = None):
    """(c): [(client, file, line, macro, va, name, status, candidates)] for every literal-VA EQ_* use inside a
    function body, and {rel: [(start, end, name)]}. status: apply (an exact define is visible: the edit),
    hidden (exact defines exist, none visible here), none (no define has this exact text), nested (inside a
    use that is rewritten as a whole)."""
    repo = Path(repo)
    tree = tree or _Tree(repo)
    regs = regs if regs is not None else {c: R.build(repo, c) for c in R.CLIENT_IDS}
    table = tree.direct_defines()
    uses, shadows, owner = tree.uses(), _shadow_names(regs), _client_of(regs)
    in_headers = {d.name for rel in tree.code_rels if rel.lower().endswith(".h") for d in tree.src[rel].defs}
    rows, edits = [], collections.defaultdict(list)
    import bisect
    for rel in tree.code_rels:
        s = tree.src[rel]
        bodies = s.bodies()
        starts = [b[0] for b in bodies]
        last = -1
        for m in R.EQ_MACRO.finditer(s.code):
            args, end = R.macro_args(s.code, m.end())
            va = R.macro_va(m.group(1), args)
            if not va:
                continue
            k = bisect.bisect_right(starts, m.start()) - 1
            if not (k >= 0 and bodies[k][0] <= m.start() < bodies[k][1]) or s.pp_at(m.start()):
                continue
            names = table.get(_toks(s.bc[m.start():end]), set())
            vis = [n for n in names if tree.visible(rel, m.start(), n)]
            if m.start() < last:
                status, pick = "nested", ""
            elif vis and "\n" in s.text[m.start():end]:
                status, pick = "multiline", ""         # one name for 2+ lines would move every later __LINE__
            elif vis:
                pref = _registry_names(regs, [owner[rel]] if rel in owner else None, int(va, 16))
                status, pick = "apply", _pick(vis, uses, shadows, in_headers, pref)
                edits[rel].append((m.start(), end, pick))
                last = end
            else:
                status, pick = ("hidden" if names else "none"), ""
            rows.append((owner.get(rel, ""), rel, s.line(m.start()), m.group(1), f"{int(va, 16):08X}", pick, status,
                         " ".join(sorted(names))))
    return rows, dict(edits)


def plan_defines(repo: Path, regs: dict | None = None, tree: _Tree | None = None, external: set | None = None):
    """(e): [(va, alias, kept, uses, rewritten, blocked, verdict)] for every name of a group of 2+ stable defines
    with the same body (the same VA, type and prototype), and {rel: [(start, end, new)]} (new '' = drop the
    alias's #define line). A VA whose names differ in type/prototype is not mergeable: those rows say
    'differs'. An alias keeps its #define while any use could not be rewritten (the kept name not visible
    there, `#`/`##` next to it, a #if/#ifdef line, a macro parameter of the same name) or the name appears in
    hooks/, tests/, ghidra-types/, reference/ or a top-level file."""
    repo = Path(repo)
    tree = tree or _Tree(repo)
    regs = regs if regs is not None else {c: R.build(repo, c) for c in R.CLIENT_IDS}
    external = external if external is not None else _external_words(repo)
    uses, shadows, owner = tree.uses(), _shadow_names(regs), _client_of(regs)
    in_headers = {d.name for rel in tree.code_rels if rel.lower().endswith(".h") for d in tree.src[rel].defs}
    def_rels = collections.defaultdict(set)
    for rel, s_ in tree.src.items():
        for d in s_.defs:
            def_rels[d.name].add(rel)
    by_va = collections.defaultdict(lambda: collections.defaultdict(set))
    loose = collections.defaultdict(set)               # va -> names whose define is not stable
    for name, forms in tree.forms.items():
        b = tree.stable(name)
        k = _direct_key(b) if b else None
        if k:
            by_va[_key_va(k)][k].add(name)
        else:
            for params, body, _ in forms:
                k2 = _direct_key(body) if params is None else None
                if k2:
                    loose[_key_va(k2)].add(name)
    occ = collections.defaultdict(list)                # word -> [(rel, pos)] in code files
    introduced = collections.defaultdict(set)          # rel -> kept names a local rename already defines there
    rows, edits = [], collections.defaultdict(list)
    for va in sorted(set(by_va) | set(loose)):
        groups = by_va.get(va, {})
        if sum(len(n) for n in groups.values()) + len(loose.get(va, ())) < 2:
            continue
        for n in sorted(loose.get(va, ())):
            rows.append((f"{va:08X}", n, "", uses[n], 0, 0, "unstable: defined differently, #undef'd or under #if"))
        for key, names in sorted(groups.items(), key=lambda kv: sorted(kv[1])):
            if len(names) < 2:
                (n,) = names
                rows.append((f"{va:08X}", n, "", uses[n], 0, 0, "differs: its own type/prototype"))
                continue
            clients = {owner[r] for n in names for r in def_rels[n] if r in owner}
            keep = _pick(names, uses, shadows, in_headers, _registry_names(regs, clients, va))
            rows.append((f"{va:08X}", keep, keep, uses[keep], 0, 0, "kept"))
            for a in sorted(names - {keep}):
                if not occ:
                    for rel in tree.code_rels:
                        for w in WORD.finditer(tree.src[rel].code):
                            occ[w.group(0)].append((rel, w.start()))
                done, blocked, renamed, ident = [], 0, [], 0
                by_file = collections.defaultdict(list)
                for rel, pos in occ.get(a, ()):
                    by_file[rel].append(pos)
                for rel, poss in sorted(by_file.items()):
                    if keep not in introduced[rel] and tree.local_ok(rel, a, keep, poss):
                        introduced[rel].add(keep)
                        renamed.append(rel)
                        done += [(rel, p, p + len(a), keep) for p in poss]
                for rel, pos in occ.get(a, ()):
                    if rel in renamed:
                        continue
                    s = tree.src[rel]
                    pp = s.pp_at(pos)
                    if pp:
                        d = next((d for d in s.defs if d.start == pp[0]), None)
                        if d and d.name_at == pos:
                            continue                       # a definition of the alias itself
                        if d is None or pos < d.name_at or (d.params and a in WORD.findall(d.params)):
                            blocked += 1                   # #if/#ifdef/#undef line, or a parameter named so
                            continue
                    if not tree.visible(rel, pos, a):
                        if tree.never_macro(rel, a):
                            ident += 1                     # an identifier of that name (a function, say)
                            continue
                        blocked += 1                       # may or may not be the macro here: leave it all
                        continue
                    before, after = s.code[:pos].rstrip(), s.code[pos + len(a):].lstrip()
                    if before.endswith("#") or after.startswith("##") or not tree.visible(rel, pos, keep):
                        blocked += 1
                        continue
                    done.append((rel, pos, pos + len(a), keep))
                for rel, p0, p1, new in done:
                    edits[rel].append((p0, p1, new))
                defs = [(rel, d) for rel in tree.code_rels if rel not in renamed for d in tree.src[rel].defs
                        if d.name == a]
                pre = f"renamed in {len(renamed)} files; " if renamed else ""
                if not defs:
                    verdict = pre + "no other #define"
                elif blocked:
                    verdict = pre + f"define kept: {blocked} uses not rewritable"
                elif ident:
                    # review #3: the name is also a function/variable somewhere. Should a TU see the alias in a
                    # way the include graph misses, a use left behind would bind to that silently: keep the define
                    verdict = pre + f"define kept: also a non-macro identifier ({ident} places)"
                elif a in external:
                    verdict = "define kept: name used outside src/ and include/"
                elif not all(d.single for _, d in defs) or len(defs) + sum(
                        1 for r in renamed for d in tree.src[r].defs if d.name == a) != sum(
                        1 for s in tree.src.values() for d in s.defs if d.name == a):
                    verdict = pre + "define kept: multi-line or defined outside src/ and include/"
                else:
                    verdict = pre + "define dropped"
                    for rel, d in defs:
                        edits[rel].append((d.start, d.end, ""))
                rows.append((f"{va:08X}", a, keep, len(done) + blocked, len(done), blocked, verdict))
    return rows, dict(edits)


def apply_pass(repo: Path, which: str, regs: dict | None = None, dry_run: bool = False, rounds: int = 10) -> dict:
    """--apply calls|defines: plan, write, and plan again on the result until nothing is left (one round when
    dry_run): an (e) round can make a kept name visible where it was not (a .cpp-local rename) and so unlock
    more. Returns {rel: edits} summed over the rounds."""
    plan = {"calls": plan_calls, "defines": plan_defines}[which]
    total = collections.Counter()
    for _ in range(1 if dry_run else rounds):
        _, edits = plan(repo, regs)
        done = write_edits(repo, edits, dry_run)
        if not done:
            break
        total.update(done)
    return dict(total)


def _edited(rel: str, text: str, ed) -> str:
    """`text` with the edits [(start, end, new)] applied (see write_edits); raises ValueError on a bad edit."""
    parts, last, dropped, dropped_crlf = [], 0, 0, 0
    for s_, e_, new in sorted(ed):
        if s_ < last:
            raise ValueError(f"{rel}: overlapping edits at {s_}")
        span = text[s_:e_]
        if new:
            if "\n" in span or "\r" in span:
                raise ValueError(f"{rel}: edit at {s_} spans a line break: {span!r}")
        else:
            if not ((s_ == 0 or text[s_ - 1] == "\n") and (e_ == len(text) or text[e_ - 1] == "\n")):
                raise ValueError(f"{rel}: line drop at {s_} is not whole lines")
            dropped += span.count("\n")
            dropped_crlf += span.count("\r\n")
        parts.append(text[last:s_])
        parts.append(new)
        last = e_
    parts.append(text[last:])
    out = "".join(parts)
    if out.count("\n") != text.count("\n") - dropped or out.count("\r\n") != text.count("\r\n") - dropped_crlf:
        raise ValueError(f"{rel}: line endings would change")
    return out


RENAMED_PRE = re.compile(r"^renamed in (\d+) files; ")


def report_defines(repo: Path, regs: dict | None = None, rounds: int = 10):
    """(e) for the report, counted the way --apply defines writes it (review #5): round after round on an
    in-memory copy until nothing is left. -> (rows, {rel: edits} summed, rounds with edits). One row per name,
    from the last round it appears in; an alias's `rewritten` sums its rounds and `blocked` is what is still
    blocked at the end (a use a later round rewrites is not reported as blocked)."""
    repo = Path(repo)
    regs = regs if regs is not None else {c: R.build(repo, c) for c in R.CLIENT_IDS}
    external = _external_words(repo)
    texts, total, n = {}, collections.Counter(), 0
    final, rewritten, renamed = {}, collections.Counter(), collections.Counter()
    for _ in range(rounds):
        tree = _Tree(repo, texts=texts)
        rows, edits = plan_defines(repo, regs, tree, external)
        for r in rows:
            key = (r[0], r[1])
            if r[2] and r[1] != r[2]:
                rewritten[key] += r[4]
                m = RENAMED_PRE.match(r[6])
                if m:
                    renamed[key] += int(m.group(1))
            elif key in final and final[key][2]:
                # a merged name left alone on its key (its aliases gone) reads 'differs' in the next round:
                # keep it as the kept name it is, with its current use count
                old = final[key]
                r = old[:3] + (r[3],) + old[4:] if old[1] == old[2] else old
            final[key] = r
        edits = {rel: ed for rel, ed in edits.items() if ed}
        if not edits:
            break
        n += 1
        for rel, ed in edits.items():
            texts[rel] = _edited(rel, tree.src[rel].text, ed)
            total[rel] += len(ed)
    out = []
    for key, r in final.items():
        if r[2] and r[1] != r[2]:
            v = RENAMED_PRE.sub("", r[6])
            pre = f"renamed in {renamed[key]} files; " if renamed[key] else ""
            r = (r[0], r[1], r[2], rewritten[key] + r[5], rewritten[key], r[5], pre + v)
        out.append(r)
    out.sort(key=lambda r: r[0])                       # stable: a VA's rows keep their order
    return out, dict(total), n


def write_edits(repo: Path, edits: dict, dry_run: bool = False) -> dict[str, int]:
    """Apply {rel: [(start, end, new)]} to the files (raw bytes: CRLF/LF untouched). An edit stays within one line,
    except new == '' which drops whole lines. Returns {rel: edits}."""
    repo = Path(repo)
    done, outs = {}, []
    for rel, ed in sorted(edits.items()):                # check every file first: all or nothing is written
        if not ed:
            continue
        p = repo / rel
        out = _edited(rel, p.read_bytes().decode("utf-8", errors="surrogateescape"), ed)
        done[rel] = len(ed)
        outs.append((p, out))
    if not dry_run:
        for p, out in outs:
            p.write_bytes(out.encode("utf-8", errors="surrogateescape"))
    return done


def opcode_values(repo: Path) -> dict:
    p = repo / "include/trilogy/opcodes.h"
    out = {}
    if p.is_file():
        for m in re.finditer(r"\b(OP_\w+)\s*=\s*(0x[0-9A-Fa-f]+)", p.read_text(encoding="utf-8", errors="replace")):
            out.setdefault(int(m.group(2), 16), m.group(1))
    return out


def pass_opcodes(repo: Path, rel="src/EverQuest/EQHandleMsg.cpp") -> list[tuple]:
    ops = opcode_values(repo)
    p = repo / rel
    if not p.is_file():
        return []
    text = p.read_text(encoding="utf-8", errors="replace")
    S = ct.significant(ct.tokenize(text))
    M = ct.match_brackets(S)
    rows = []
    stack = []                         # (switch line, its expression, closing brace index)
    for i, t in enumerate(S):
        while stack and i > stack[-1][2]:
            stack.pop()
        if t.kind == "id" and t.text == "switch" and i + 1 < len(S) and S[i + 1].text == "(":
            rp = M.get(i + 1)
            if rp is not None and rp + 1 < len(S) and S[rp + 1].text == "{":
                expr = text[S[i + 2].start:S[rp].start] if rp > i + 2 else ""
                stack.append((text.count("\n", 0, t.start) + 1, re.sub(r"\s+", " ", expr), M.get(rp + 1, len(S))))
        elif t.kind == "id" and t.text == "case" and i + 1 < len(S) and S[i + 1].kind == "num" \
                and S[i + 1].text.lower().startswith("0x"):
            v = int(re.sub(r"[uUlL]+$", "", S[i + 1].text), 16)
            sw = stack[-1] if stack else (0, "", 0)
            rows.append((rel, text.count("\n", 0, t.start) + 1, f"0x{v:04X}", ops.get(v, ""), sw[0], sw[1]))
    return rows


def write_tsv(path: Path, header: list[str], rows):
    path.write_text("\t".join(header) + "\n" + "".join("\t".join(str(x) for x in r) + "\n" for r in rows),
                    encoding="utf-8")


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--repo", default=".")
    ap.add_argument("--out", help="folder for passes.md and the TSVs")
    ap.add_argument("--check-renames", action="store_true", help="(b): dry-run rename.py on each csv name")
    ap.add_argument("--apply", choices=["spelling", "calls", "defines"],
                    help="write a pass to the repo: (a) spelling, (c) calls, (e) defines")
    ap.add_argument("--dry-run", action="store_true", help="with --apply: count, write nothing")
    a = ap.parse_args(argv)
    repo = Path(a.repo).resolve()
    regs = {c: R.build(repo, c) for c in R.CLIENT_IDS}
    if a.apply == "spelling":
        changed = apply_spelling(repo, regs, a.dry_run)
        for rel, n in sorted(changed.items()):
            print(f"{n:6d}  {rel}")
        print(f"(a) spelling: {sum(changed.values())} VA tokens in {len(changed)} files"
              + (" (dry run, nothing written)" if a.dry_run else " rewritten"))
        return 0
    if a.apply in ("calls", "defines"):
        changed = apply_pass(repo, a.apply, regs, a.dry_run)
        for rel, n in sorted(changed.items()):
            print(f"{n:6d}  {rel}")
        tag = "(c) calls" if a.apply == "calls" else "(e) defines"
        print(f"{tag}: {sum(changed.values())} edits in {len(changed)} files"
              + (" (dry run of the first round, nothing written)" if a.dry_run else " written"))
        return 0
    sp, cs, calls, defs = [], [], [], []
    multi = {}
    for c, reg in regs.items():
        sp += pass_spelling(reg)
        cs += pass_csv(repo, reg, a.check_renames)
        rows, protos = pass_calls(reg)
        calls += rows
        multi[c] = sum(1 for v in protos.values() if len(v) > 1)
        defs += [(c, f"{v:08X}", d) for k, v, d in reg.conflicts() if k == "define_names"]
    ops = pass_opcodes(repo)
    md = ["# Scripted passes: what they would change", ""]
    by = collections.Counter(r[0] for r in sp)
    md.append(f"(a) spelling: {len(sp)} VA tokens to rewrite to `0x` + upper-case hex in "
              f"{len({(r[0], r[1]) for r in sp})} files ({', '.join(f'{k} {v}' for k, v in sorted(by.items()))}); "
              f"{len({(r[0], int(r[3], 16)) for r in sp})} VAs.")
    ok = sum(1 for r in cs if r[6].endswith("ok"))
    ref = sum(1 for r in cs if "REFUSED" in r[6])
    md.append(f"(b) csv: {len(cs)} FUN_ functions with a names/*.csv name ("
              + ", ".join(f"{k} {v}" for k, v in sorted(collections.Counter(r[0] for r in cs).items())) + ")"
              + (f"; rename.py would apply {ok}, refuse {ref}" if a.check_renames else "") + ".")
    named = [r for r in calls if r[5]]
    single = [r for r in named if r[7] == "global" or r[6] in (1, "")]
    md.append(f"(c) calls/globals in bodies: {len(calls)} literal-VA uses; {len(named)} to a VA with a registry "
              f"name; {len(single)} of those are globals or single-prototype calls (blind-substitutable); "
              f"VAs called with 2+ prototypes: " + ", ".join(f"{k} {v}" for k, v in multi.items()) + ".")
    op_hits = [r for r in ops if r[3]]
    sw = collections.defaultdict(lambda: [0, 0])
    for r in ops:
        sw[(r[4], r[5])][0] += 1
        sw[(r[4], r[5])][1] += bool(r[3])
    cand = {k: v for k, v in sw.items() if v[1] * 2 > v[0]}
    md.append(f"(d) opcodes: EQHandleMsg.cpp has {len(ops)} `case 0x` labels, {len(op_hits)} are OP_* values; "
              f"{len(cand)} of {len(sw)} switches are mostly opcodes ("
              + "; ".join(f"line {k[0]} `{k[1][:40]}` {v[1]}/{v[0]}" for k, v in sorted(cand.items())) + ").")
    md.append(f"(e) defines: {len(defs)} VAs with 2+ #define names ("
              + ", ".join(f"{k} {v}" for k, v in sorted(collections.Counter(r[0] for r in defs).items())) + ").")
    kept = [r for r in sp if r[1] in SPELLING_KEEP]
    if kept:
        md.append(f"(a) note: {len(kept)} of those tokens are in {', '.join(sorted({r[1] for r in kept}))}, which "
                  "--apply spelling leaves alone (SPELLING_KEEP).")
    tree = _Tree(repo)
    c_rows, _ = plan_calls(repo, regs, tree)
    e_rows, e_edits, e_rounds = report_defines(repo, regs)
    st = collections.Counter(r[6] for r in c_rows)
    md.append(f"(c) apply: {len(c_rows)} literal-VA uses in bodies; {st['apply']} rewritable to an exact visible "
              f"#define ({len({r[5] for r in c_rows if r[6] == 'apply'})} names, "
              f"{len({r[1] for r in c_rows if r[6] == 'apply'})} files; "
              + ", ".join(f"{k} {v}" for k, v in sorted(collections.Counter(r[0] for r in c_rows
                                                                          if r[6] == "apply").items()))
              + f"); {st['hidden']} have an exact #define that is not visible there; {st['none']} have none; "
              f"{st['nested']} nested; {st['multiline']} span 2+ lines (left as they are).")
    al = [r for r in e_rows if r[2] and r[1] != r[2]]
    ev = [RENAMED_PRE.sub("", r[6]) for r in al]
    md.append(f"(e) apply ({e_rounds} rounds to the fixed point, as --apply defines writes them): "
              f"{len({r[0] for r in e_rows})} VAs with 2+ define names; {len({r[0] for r in al})} have "
              f"token-identical aliases ({len(al)} aliases of {sum(1 for r in e_rows if r[6] == 'kept')} kept "
              f"names): {sum(r[4] for r in al)} uses rewritten, {sum(r[5] for r in al)} still blocked; "
              f"{sum(1 for r in al if RENAMED_PRE.match(r[6]))} aliases renamed .cpp-locally, "
              f"{sum(1 for v in ev if v == 'define dropped')} alias #defines dropped, "
              f"{sum(1 for v in ev if v.startswith('define kept'))} kept; "
              f"{sum(1 for r in e_rows if r[6].startswith('differs'))} names differ in type/prototype, "
              f"{sum(1 for r in e_rows if r[6].startswith('unstable'))} unstable "
              f"({sum(e_edits.values())} edits in {len(e_edits)} files).")
    print("\n".join(md))
    if a.out:
        out = Path(a.out)
        out.mkdir(parents=True, exist_ok=True)
        (out / "passes.md").write_text("\n\n".join(md) + "\n", encoding="utf-8")
        write_tsv(out / "a_spelling.tsv", ["client", "file", "line", "token", "canonical", "macro"], sp)
        write_tsv(out / "b_csv.tsv", ["client", "va", "source_name", "csv_name", "csv_source", "confidence",
                                      "rename_check"], cs)
        write_tsv(out / "c_calls.tsv", ["client", "file", "line", "macro", "va", "registry_name", "prototypes",
                                        "kind"], calls)
        write_tsv(out / "d_opcodes.tsv", ["file", "line", "value", "opcode", "switch_line", "switch_expr"], ops)
        write_tsv(out / "e_defines.tsv", ["client", "va", "names"], defs)
        write_tsv(out / "c_apply.tsv", ["client", "file", "line", "macro", "va", "name", "status", "candidates"],
                  c_rows)
        write_tsv(out / "e_apply.tsv", ["va", "name", "kept_name", "uses", "rewritten", "blocked", "verdict"],
                  e_rows)
    return 0


if __name__ == "__main__":
    sys.exit(main())
