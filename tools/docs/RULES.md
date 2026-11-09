# Rules for the decomp repo

How code is written and checked here. `CONTRIBUTING.md` (repo root) has the provenance rules and the
steps for a change; `docs/tool-index.md` lists which tool does what; `docs/PLAN.md` is the roadmap.

*Earned by:* lines name the failure behind a rule, so a hard-won rule can be told from an arbitrary one.

"Faithful functional" = the same behavior in every detail, **bugs included**, written so the original
compiler can build it and functions can be matched without rewriting.

## Ground rules
- **Never commit** original game files, Ghidra/BinDiff projects, decompiler output, exports or context
  bundles. They contain the client's bytes or derived listings. They live in the workspace, outside the
  repo; `.gitignore` is only a second line of defense.
- **Never connect to Daybreak's live servers.** Local servers only.
- Never use leaked Sony/Daybreak source.
- Community projects (EQClassic, EQEmu, ShowEQ, MacroQuest, TAKP lists) are **references only**. Use them to
  confirm struct layouts, offsets, opcodes, names and behavior. Never paste their code into the repo; they are
  GPL or unlicensed, and this repo is MIT. `reference/MANIFEST.md` lists them.
- Where a reference disagrees with the binary, **the binary wins**. Note the discrepancy.
- Preserve original behavior exactly. Any fix must be clearly marked, `#ifdef EQD_FIXES`, off by default.
- If stuck, **do not** modify the compiler, tests, harness or comparison tools to force a pass. Flag the
  function.

## Per-function workflow
1. Pick a function (leaf-first, within one subsystem) and gather its evidence: assembly, callers and
   callees, known structs (`tools/context/` builds a bundle from a local Ghidra export).
2. Write the C++ in `src/<subsystem>/`, annotated with the original address (and original file when known):
   ```cpp
   // FUNCTION: TRILOGY 0x0041a2c0
   // ORIGINAL FILE: EverQuest/Player.cpp (from Mac debug map)
   int EQPlayer::GetHealthPercent_guess() const { ... }
   ```
3. Add or update its fixture in `tests/unicorn/` and run it.
4. Run the mutation check and the bounded match pass (below).

## A function counts as tested when
- its fixture exists (`"original"` = its VA) and passes;
- a mutation check ran on it (`tools\unicorn\tools\mutate_check.py --va <VA>`: deliberate wrong edits fail
  the fixture; survivors are either equivalent or fixed by adding cases);
- its match status is recorded (identical, or what still differs).
Anything short of that is "rewritten", not "tested".

## Conventions
- Struct/class changes go in `include/` only, with offsets in comments (`// 0x40`) and compile-time size
  checks (`C_ASSERT`). If your guess conflicts with an existing field, stop and flag it; don't overwrite.
- Match the original calling conventions (`__thiscall` for methods with `this` in ECX; `__cdecl`/`__stdcall`
  as in the binary).
- **Code must compile with the client's original compiler** (Visual C++ 6.0, see `reference/VERSIONS.md`). No
  language features it lacks, no STL where the original didn't use it. Keep code shape close to the
  original: branch order, early returns, loop forms.
- Descriptive names; `_guess` suffix until confirmed. Never overwrite a confirmed name. Every imported name
  records its source (`eqclassic`, `mac`, `takp`, `manual`, ...) and a confidence in `names/`.
- Skip library code (CRT, DirectX, zlib, Miles, Smacker). Tag it in Ghidra (FID) instead.
- **Call other functions by address, never by symbol** (`EQ_CDECL`/`EQ_THISCALL`/`EQ_STDCALL` defines or
  inline macros), and never add a second source to a fixture to make it link. A function's own fixture can
  pass while every other fixture compiling that file alone fails to link.
- **C++ callers**: call a client address with `EQ_CDECL_CPP`/`EQ_STDCALL_CPP` when the original keeps its EH
  state stores (`mov byte ptr [ebp-4],n`) around the call; VC6 treats an `extern "C"` callee (`EQ_CDECL`) as
  non-throwing and drops them. To call a client constructor, destructor or method directly (e.g. `new Foo`),
  declare it without a body and add `// EXTERNAL: TRILOGY 0x<va> Foo::Foo` above the declaration (local
  classes move to file scope first); match mode, the hook DLL and the fixtures resolve it to that address.
- **Paths may hold spaces** (`src/Win32 Goodies/`): never split git or map output on whitespace.

## Keeping byte-matching possible
- **Source layout follows the original objects.** One source file per original object file (Mac map object
  names, address neighbours); inside a file, functions in ascending address order. A run of addresses that
  belongs to another object gets its own file.
- **Literal translation over cleanup.** Keep the original's types (signed/unsigned, `char` vs `int` widths),
  statement order, redundant loads and temporaries, and return patterns (`int rc = 0; ... return rc;`) when the
  code shape suggests them. Don't refactor.
- **Bounded match pass.** After a function passes its fixture, check it with `compare.py --match`. If it's
  `diff` with 5 or fewer instructions differing, spend at most 2 compile iterations chasing identical output.
  If it's `structural` or a larger diff, don't iterate: record it and move on (the permuter handles those).
  Either way, record what still differs (e.g. "regalloc ESI/EDI swap", "block order", "jump table vs
  compare tree", "EBP frame"). A fixed-address call compiled as `mov reg,VA; call reg` instead of `call VA`
  is a test-build artefact (the `eqaddr.h` macros), not a difference.
- **Per-object compiler options** (e.g. the rdp library's `/Ot`, `/GX` for C++ EH) go in the source file's
  flags line (see `unicorn/README.md`), never as a global change.
- **Known VC6 SP4 codegen patterns** (switch tables, block order, EH frames, inlining, slots, ...), each
  with a rerunnable repro: `docs/VC6_QUIRKS.md`; test a new idea with `compare/probe.py` before rewriting.
- **Duplicates**: the same code often exists several times (per-type copies, inline expansions); match one
  copy, then try its source on the others, each checked on its own.

## Fake matches
A fake match is code shaped only to steer the compiler's output, not to say what the original does. A check
blocks it instead of review catching it.
- Only after a real search (the bounded match pass, the permuter), and each one marked
  `/* fake match: <what is forced and why> */` on its line, in the comment lines directly above it, or in
  the comment block in front of the function (then it covers the whole function).
- **Allowed forms:** an empty `do {} while (0)` barrier; a local that only shapes the frame or register
  choice; a reused variable; permuter output cleaned up into one of these.
- **Banned:** `register` pinning (flagged even when marked), gotos standing in for a `switch` or a loop,
  code that only matches when compiled outside its own file (another translation unit, flags or includes).
- A `goto` or `__asm` that reproduces the original's own shape (a shared tail, an x87 sequence VC6 won't
  emit) is not a fake match. Say so instead: `// as original: <why>`.
- `tools\compare\source_checks.py` flags unmarked barriers, unused locals, `goto`, `__asm` and any
  `register`.
  *Earned by:* (elsewhere) dcb_decomp's and HoMM1's fake-match policies.

## Source status markers
A marker next to the code says "this was already chased". Put it in the comment block after `// FUNCTION:`,
once per function, each with a reason after the colon. These four are the only ones (`source_checks.py`
rejects others):
- `// @early-stop: <what still differs>`: body complete, the match was chased within the bound and stopped;
  the remaining diff is bounded and explained.
- `// @identity-TODO: <what would prove it>`: behavioral equivalence isn't proven yet (e.g. a path no
  fixture reaches).
- `// @dead-code: <evidence>`: nothing references it in the client; still rebuilt.
- `// @equivalent: <why it is harmless>`: the equivalence note for a function that stays non-identical
  (regalloc swap, block order: why the remaining diff can't change behavior).
`@early-stop`/`@equivalent` on a function that is now byte-identical is stale: remove it.
*Earned by:* (elsewhere) HoMM1 and isledecomp keep chase status next to the code.

## Readability ratchet
- `source_checks.py` counts per function (and per file scope) casts, `reinterpret_cast`, gotos, raw address
  arithmetic (`*(T*)(p + 0x10)`, `(T*)0x006E4DE0`), externs, unions, `__asm`, `register`, `volatile`,
  barriers, unused locals and unmarked oddities, against the repo's `tools-ratchet.json`.
- Counts of existing code may only go down; `--update` records drops. A new function's counts are free
  (literal translation needs casts), but its unmarked oddities are not.
- Raising a floor is an explicit, reviewed step: `source_checks.py --raise-floor`, committed on its own with
  the reason in the message.
  *Earned by:* (elsewhere) HoMM1's per-file floors against cruft creeping in.
- Renames and comment-only edits go through `tools\readability\gate.py`: every touched file must compile to
  the same code as before.

## Escalation
- A function that keeps failing after a few attempts gets a one-line note on what blocks it and is left for
  later; never bend the harness or the fixture to make it pass.
