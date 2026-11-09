# context/

`pack_context.py` writes one markdown **context bundle** per function: everything needed to
rewrite it. Input: `build\export\<client>\` (see `export\README.md`) and `eq-<client>\include\<client>\*.h`.
Output: `build\context\<client>\<ADDR>_<name>.md`, outside every repo. Bundles contain decompiler output and
client bytes: **never committed**.

```
.venv\Scripts\python eq-trilogy\tools\context\pack_context.py trilogy 004C8FCE [ADDR ...]   (0x prefix ok)
    --subsystem S      every function with that subsystem (or under S/): the map-derived one, else the
                       queue's inferred one; --limit N caps it
    --export-dir DIR   --out DIR
    --similar K        the K most similar matched functions (default 5, 0 = none); --similar-cache DIR
    --quirks M         VC6_QUIRKS entries: nearmiss (default: queue match diff/structural), all, off
    --stack-hints F    stackcmp hints JSON (default <build>\match\stackcmp.json when present; off = none)
```
Smoke test: `.venv\Scripts\python -m pytest eq-trilogy\tools\tests\context`.

## Bundle sections

1. **Header**: address range, name, name evidence (confidence/source, note, rejected alternatives, decompiler
   plate comment), class/tags, subsystem, Mac source file, size, calling convention, signatures (Ghidra and
   decompiler), queue state, **ECX evidence** and **EAX evidence** (below).
2. **Rules (excerpt)**: `// FUNCTION: TRILOGY 0x...` / `// ORIGINAL FILE:` annotation (pre-filled for this
   function), VC6 constraints, `_guess` naming, struct/`EQD_FIXES` rules, escalation. Text lives in `RULES`
   in the script; keep it in step with `docs\RULES.md`.
3. **Ghidra pseudocode** (first 400 lines). Over the limit the bundle says plainly **TRUNCATED** above the
   listing, and names where the rest is: with `build_bundle(full_dir=...)` the complete text is `FULL_PSEUDO.c` next to `BUNDLE.md`; without `full_dir` the
   export shard.
4. **Assembly** with bytes and symbolic operands (first 700 instructions; the same TRUNCATED note, and the
   complete listing is `FULL_ASM.txt` next to `BUNDLE.md`). A reference to a
   GetProcAddress slot is annotated `-> t3dFoo (GetProcAddress slot)`. FULL_* files are Ghidra's whole
   listing; `build_bundle` removes the previous function's before writing (group mode reuses one copy).
   **Code outside the Ghidra listing**: instructions of the function's extent (`function_extent`, what the
   harness runs and the coverage gate counts) that Ghidra's body lacks, disassembled from the binary with
   capstone. Mostly case blocks of a switch Ghidra could not decode (eqmain `1001C030`: 27 of its 34 states,
   202 instructions, were missing, so 7 runs translated only the listed states and failed the gate at 53%)
   and tails after no-return calls. Over the asm cap the rest goes to `EXTRA_ASM.txt`.
   **Switch tables**: which switches exist and each table's entry count come from
   `eqd_unicorn.extent.function_extent(..., switches=[...])` (control flow through the binary's bytes: also
   jumps Ghidra never decoded, and tables with no bound check, which are read while their entries point into
   the function). The code before each `JMP` is read only for the annotations (bias, bound, default, byte
   index table); if extent can't run (missing binary, an extent without `switches`, CRT `memcpy`'s
   unbounded tables) the listing's own `JMP dword ptr [R*0x4 + T]` are decoded from the code's bound check
   as before. Each is decoded as target -> case values, including VC6's
   byte index tables (`MOVZX R,byte ptr [I + B]`, or `XOR R,R; MOV R8,byte ptr [I + B]`, when that is the
   last write to `R` before the jump). The bound is the `CMP idx,N` + `JA default` next to the jump
   (`CMP idx,reg` resolves `MOV reg,N` / `PUSH N; POP reg`; `CMP [local],N; MOV idx,[local]` is followed);
   a `CMP` only counts when a `JA` follows it and nothing rewrites the index before the jump (except
   reloading it or redoing the biasing instruction). A bias added before the check
   (`ADD/SUB/LEA/DEC/INC`) is undone, so the values are the source's `case` labels (Ghidra's pseudocode
   often shows the biased index instead: `case 0` for `case 100`). Table bytes come from the binary
   (`original\<client>\...`, checked against the export's sha256), since the export has none. Jumps
   without a bound check (the CRT's hand-written `memcpy`/`memmove` tables) say so instead.
   Before extent supplied the list: 121 of 155 decoded, the 34 others CRT `memcpy`/`memmove` plus one game
   function (`004CC92F`, `MOVSX EAX,word ptr [EBP + 0x8]` with no check: now its 81 entries).
5. **Callers** (grouped, with call sites, up to 40, and per caller how many sites load ECX first) and
   address-taken references.
6. **Callees**: distinct targets with name, signature, convention, size, name confidence/source, queue
   state, and a 3-statement body one-liner from their pseudocode; imports and indirect calls listed.
   An indirect `call dword ptr [slot]` through a GetProcAddress slot is named:
   `= **GetProcAddress "t3dFloatFastSine"** (loader 004DDE87 Init_EQGfx_Dx8_guess), eqgfx.h ...`.
7. **Strings** (value) and **Globals** (name, type, size, block, writable, xref count). The export has no
   data bytes, so globals have no values.
8. **Known structs**: header definitions chosen by name from the signature, namespace/class, decompiler
   pointer types and callee classes (max 6). Definitions over 120 lines are cut to the header comment plus
   the field lines whose `0xNN` offset comment matches a `[reg+0xNN]` displacement in the asm. The list of
   displacements is printed so a reader can spot offsets the headers lack.
9. **Similar matched functions** (`similar.py`): the 5 tested functions with match status
   `identical`/`normalised` whose original code is most like this one, each with its rewrite (annotation to
   end of definition, first 60 lines) and its original normalised listing (first 60 instructions).
10. **VC6 matching notes** (only when there is something to say): the queue's `match` (and `match_best`
   when better), the stackcmp frame line, up to 4 `docs\VC6_QUIRKS.md` entries in short form, and an x87
   note (below).
11. **Prior attempts**: last 3 from `eq-<client>\queue\queue.jsonl` (`attempts`), omitted when none.

## Derived facts

- **GetProcAddress slots** (eqgfx calls): computed at pack time from the export, no cache. Every function
  that reads the `GetProcAddress` IAT entry is scanned for `PUSH <string>` ... `CALL <GetProcAddress>`
  (through the IAT or a register loaded from it) ... `MOV [slot],EAX`; the name is the full string from
  `strings.json` (Ghidra's labels are truncated). Trilogy: 250 slots, 246 from the eqgfx loader `004DDE87`
  (`Init_EQGfx_Dx8_guess`; `GetDataAddresses`, `t3d*`, `s3d*`), plus `AWEManager`, `dll_main` (eqmain.dll)
  and the CRT's `MessageBoxA`/`GetLastActivePopup`. The `#define name EQ_GLOBAL(type, 0xADDR)` macros in
  `include\<client>\eqgfx.h` are cross-checked (all 19 agree); a disagreement shows as "eqgfx.h names it ..." in the callee line.
- **EAX evidence** (returns a value vs void): for each direct call site, up to 12 instructions after the
  call (one unconditional `JMP` followed) decide whether the caller reads EAX/AX/AL before overwriting it,
  or uses ST0 (float return). Plus a heuristic on the function: the last EAX write before each `RET`
  (fresh value, call result, arithmetic, or scratch pointer). Verdicts: `returns a value ...` (some site
  reads it),
  `likely void`, `inconclusive`. make_fixture.py prefills `ret` from it.
- **ECX evidence** (thiscall vs static): for each direct call site, the up to 16 instructions before the
  call are walked back to the last write to ECX. A `CALL`/`RET` ends the walk; at a `JMP` the call's block
  is only reached by jumps, so the walk continues from every jump to it (2 hops, all must agree). The
  last write decides: `POP ECX` is argument cleanup, and a load followed by `PUSH ECX` is an argument
  (`LEA ECX,[EBP + -0x1a]; PUSH ECX`), neither counts as `this`. Plus a linear scan of the function
  itself: does it read ECX before writing it (`PUSH ECX` is ignored: VC6 uses it to reserve a stack
  slot). Summary: "ECX set before the call at k/n direct call sites" -> `__thiscall` (all, and the
  function reads ECX), "a `__thiscall` method that ignores `this`" (all, the function doesn't read ECX
  and ends in `RET n`), static/`__cdecl`/`__stdcall` (none) or mixed. Heuristic: ECX loaded far before
  the call is missed, and the linear scan ignores branches.

## VC6 matching notes

- **Quirks** (`--quirks`, default `nearmiss`: functions whose queue `match` is `diff`/`structural`; `all`
  also covers todo functions, from the listing alone; `off`). Entries are read from `docs\VC6_QUIRKS.md`
  (`### vc6q-*`, `keywords:`, `**Claim.**`, `**Use.**`) and scored by: the last 3 attempts' recorded `diff`
  summaries against `TEXT_TRIGGERS` ("regalloc"/"EBP frame"/"stack" -> decl order, block-scope slots;
  "jump table"/"switch"; "block order" -> if-then-first; "EH"/`fs:[`; "store order"; "call order"; setcc;
  idiv; inline; static guard: weight 3 for the main entry), the entry's own multi-word keywords in that text
  (1), and cheap features of the original listing (`FEATURE_QUIRKS`: jump table, byte index table, `dec; jz`
  chain, `__EH_prolog`/`fs:[0]`, `operator new`, `idiv` by a power of two, `and r, 0x8000000k`, static guard
  byte `test/or [g], bit` or `_atexit`, setcc, `neg; sbb`). Top 4, ties in doc order, each with its reasons,
  claim and "use" cut at ~320 characters. A diff summary that calls the miss an artefact
  (`__except_list`, "bytes identical": the `vlong`/`monty` ones) gets that note instead of quirks. x87 code
  gets a one-line note: VC6_QUIRKS has no SP4-checked x87 entry yet. On the 239 diff/structural functions
  (2026-10-04): 89 get quirks (64 from their diff summary), 16 the artefact note, the rest nothing.
- **Stack frame** (`--stack-hints`): `compare\stackcmp.py --json` output (schema 1, `compare\README.md`),
  matched by `va`, used only when `status` is `ok`: stack `same`/`differs`/`no_stack_operands`, the `sub esp`
  sizes, "partial" when structural blocks weren't paired, and the first 3 `hints`. Shown for any function in
  the file (they are all near misses). A missing default file is silent; an unusable given file is a warning.
- Tests: `tests\context\test_quirks.py` (synthetic doc, listings, queue entries and hints JSON; the two
  bundle tests need the export).

## Similar examples: `similar.py`

```
.venv\Scripts\python eq-trilogy\tools\context\similar.py trilogy 00419867 [ADDR ...] [-k 5] [--cache DIR]
```
Offline, no compiler: each function is the original side of `compare.py`'s normalised listing
(`compare.tokenise` over the recursive-descent extent; calls `F:<va>`, imports `I:dll!name`, branches
`@<index>`), registers and numbers abstracted (`mov R, dword ptr [R + N]`, keys kept). A function's
features are its instruction 1- to 3-grams plus its call/import keys; similarity is their Jaccard index
(exact; ~460 candidates, so no MinHash needed). Candidates: the queue's tested/verified functions with match
`identical`/`normalised` and a `// FUNCTION:` rewrite in `src\`, minus `queue\benchmark_<client>.json` and the
target. Listings are cached in `<build>\similar\<client>\listings.json` (keyed by the binary's sha256; first
build ~1 s). Tests: `tests\context\test_similar.py`.

## Caveats
- Struct selection is by name, not by type inference: displacements are not tied to a specific base
  register's struct. A function using `[ecx+0x40]` on an unknown class gets the offset list, no struct.
- The queue is read directly (`state`, `attempts`); a missing or odd queue just drops those parts.
- A bundle is a snapshot: re-pack after the export or the queue changes.
- Size: small functions 3-10 KB; large ones are bounded by the truncation limits (EQPlayer ctor: 64 KB).
