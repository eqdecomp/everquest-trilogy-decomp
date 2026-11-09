# EverQuest Trilogy client decompilation: plan

This is the public, trimmed version of the project's plan: the targets, the evidence behind the approach,
the phases and the key designs. Section numbers are kept so references from the tools (e.g. "PLAN.md 4.2")
still point at the right place.

---

## 1. Targets and sources

### 1.1 Target binaries: Trilogy

| File | Size | Link timestamp (UTC) | Notes |
|---|---|---|---|
| `eqgame.exe` | 1,642,496 | 2001-08-21 23:07 | **Primary target.** Velious-era live build. `.text` 0x401000–0x533580 (~1.2 MB) |
| `eqmain.dll` | 344,064 | 2001-08-20 23:30 | Front end: login, server select, char select. Base 0x10000000 |
| `eqgfx_dx7.dll` | 745,472 | 2001-07-28 01:14 | Renderer (DX7). Base 0x10000000 |

Full SHA-256 hashes are in `reference/VERSIONS.md`; the test harness refuses other builds.

- **Compiler:** the Rich header says Visual C++ 6.0. It is **VC6 SP4** for `eqgame.exe` and `eqgfx_dx7.dll`
  (game code 8799 = SP4 `c2.dll`, CRT 8797 = SP4 libc, plus some SP5-built objects, probably a prebuilt C
  library) and **SP5** for `eqmain.dll`. No Processor Pack. Details: `reference/VERSIONS.md`.

### 1.1c Beta client: extra name source, not a target

- A January 1999 beta `eqgame.exe` (VC5, no symbols). The renderer is linked in statically.
- Its debug messages name functions that the Trilogy files don't name. In practice it gave only one new
  Trilogy name (the name chain, 4.2, already had the rest) plus ~105 nameless matches for unnamed Trilogy
  functions. `ghidra/scripts/name_beta.py` carries its names across.

### 1.2 Reference material

Facts (names, packet layouts, opcodes) were taken from community projects; their code was not copied. The
list, with versions and licences, is in `reference/MANIFEST.md`. In order of usefulness for Trilogy:

1. **EQClassic** (a server for the Trilogy era): its comments cite client addresses that land exactly on
   functions in this `eqgame.exe` build, plus packet structs and opcodes.
2. **The Intel Mac client** (`eqmac.exe`): it kept its symbol table (~8,000 mangled C++ names) and a debug map
   that ties each function to its original source file. The best naming source, and the origin of the source
   tree layout.
3. **TAKP PC client binaries** (VC6, ~16 months after Trilogy) and the TAKP function list: the bridge between
   the Mac names and Trilogy.
4. **ShowEQ 3.0.4** (Velious era): an independent cross-check of packet structs.
5. **Early MacroQuest**: naming hints only (different build).
6. **TAKP server / EQMacEmu**: game logic and packet semantics for a later client.

---

## 2. Approach

1. **Names come from a chain of binaries** (4.2), with EQClassic's addresses as ground truth.
2. **The source tree follows the original layout** recovered from the Mac debug map, with an
   `// ORIGINAL FILE:` note per function where known.
3. **Unicorn is the primary gate** (4.4): there is no server, so in-game checks are limited to what runs
   offline (boot to the login screen, front-end UI, file loading).
4. **Hooking without trampolines** (4.3): whole functions are replaced with a 5-byte `JMP rel32`, so the patcher
   has no dependencies and the rewrites build with VC6 from day one. The same sources feed byte-matching.
5. **Headless PyGhidra export is the backbone** for context bundles and the work queue, so everything is
   scripted and reproducible.
6. **Scope:** `eqgame.exe` first, then `eqmain.dll`, then `eqgfx_dx7.dll` (4.5).

---

## 3. Phases

### Phase 0: environment and baseline
Repos, hashes of the supported binaries, a Python venv with unicorn/capstone/pefile/pytest, and an offline
smoke test: the client reaches its login screen when started through `hooks/` (`eqlaunch.exe` plus a small
compatibility shim for DirectInput cooperative levels that modern Windows rejects).

### Phase 1a: Ghidra baseline and naming
- Ghidra projects for Trilogy, TAKP and the Mac client (`ghidra/scripts/make_project.py`).
- A custom VC6 FID database (`build_vc6_fid.py`, `apply_fid.py`, `fid_names.py`) tags the statically linked
  CRT; libxml2 2.3.6 and zlib are tagged by address range.
- The name chain (4.2): `export_features.py`, `mac_debug_map.py`, BinDiff, `name_chain.py`, `apply_names.py`.
  Every imported name carries its source and a confidence; low confidence gets `_guess`. A seeded spot-check
  of 40 names found 39 matches right.
- Types: `include/trilogy/*.h` → `build_gdt.py` → `ghidra-types/trilogy.gdt` → `apply_types.py`, plus typed
  globals and signatures via `apply_typed_symbols.py`. Every struct size is `C_ASSERT`ed; where a reference
  and the binary disagree, the binary wins and the header says so.

### Phase 1b: tooling
1. `tools/export/`: headless PyGhidra export of functions, xrefs, call graph, pseudocode, asm, strings and
   globals to JSON.
2. A work queue (not published): functions leaf-first by subsystem.
3. `tools/context/`: the context packer (pseudocode, asm, callers/callees, known structs, prior attempts →
   one markdown bundle per function).
4. `tools/unicorn/` and `tests/unicorn/`: the differential test harness (4.4).
5. `hooks/`: VC6 hook DLL, injector, per-function switchboard (`hooks.ini`) and a bisect script (4.3).
6. A packet capture/replay tool: on hold until a server exists.

### Phase 1c: vertical slice
About 60 functions taken end to end through rewrite → Unicorn → hooked smoke test, then the tooling was fixed
until that loop was routine. Conventions that came out of it: `include/trilogy/eqaddr.h` (`EQ_GLOBAL`,
`EQ_ARRAY`, `EQ_CDECL`, `EQ_STDCALL`, `EQ_THISCALL`, always `(type, va)`), VC6 SP4 `/O1 /MT`, a default CRT
alias table for fixtures, and the test layout `tests/unicorn/<source path>/<Function>.json`.

### Phase 1d: scale out
Work the queue subsystem by subsystem: utilities and containers → net/packet decode → character and inventory
data → world/spawn → UI windows → rest. The pipeline handles several images per repo (`-c eqmain`,
`docs/EQMAIN.md`; `docs/EQGFX.md`). Large functions follow the giants routine (8).

### Phase 1e: renderer `eqgfx_dx7.dll`
See 4.5. Pure logic first (math library, file/WLD parsing, dictionaries, memory pools); DirectDraw/D3D7
device code and the software/MMX rasterizer paths last.

### Phase 3: matching
VC6 with the exact service pack; match function by function. The functional source is already VC6-shaped.
- **Match mode:** `EQD_MATCH` in `eqaddr.h` turns every client address into an external symbol
  (`eqd_fn_<va>` / `eqd_va_<va>`), so objects get `call rel32` / absolute relocations like the client;
  `compare.py --match` compares them byte for byte with relocation fields masked and the queue records the
  status per function.
- Still missing for whole-exe linking: an address-assigning link (every rewrite and data object at its
  original VA), the original object order and section alignment, `.rdata` literal pooling as in the client,
  the CRT/third-party objects linked as in 2001, real `__thiscall` classes instead of `EQ_THISCALL`, and the
  import table / resources. Until then match status is per function.
- reccmp checks (9.4): `stackcmp` on near misses is in place (`compare/stackcmp.py`); `vtable` and `datacmp`
  wait for real C++ classes and data objects; a full `reccmp` run waits for a linked `eqgame.exe`.

---

## 4. Key designs

### 4.1 Repos and workspace
- `tools/` holds the shared tooling (export, context packer, Unicorn harness, name chain, rules).
- Binaries, Ghidra projects and decompiler output live **outside** the repos, in the workspace folder the
  README describes. Repos find them through a gitignored `paths.local.ini` or `EQD_ROOT`.

### 4.2 Name chain
1. **`eqmac.exe` → TAKP `eqgame.exe`** (GCC vs VC6, so a harder diff): BinDiff, then our own string/constant
   anchor matcher (per-function signatures from string literals, distinctive immediates, callee count and
   size; anchor on unique string sets, then propagate through the call graph). Cross-checked against the
   TAKP list: where a Mac symbol and a TAKP name agree, confidence is high; where they disagree, the Mac name
   wins.
2. **TAKP `eqgame.exe` → Trilogy `eqgame.exe`** (both VC6): BinDiff. Good on game logic and networking, weak
   on UI (the old UI was replaced in Luclin).
3. **EQClassic's addresses are ground truth** for Trilogy and override the chain.
4. Every imported name records its source (`mac`, `takp`, `eqclassic`, `strings`, `beta`, `manual`) and a
   confidence in `names/`. Low confidence → `_guess`; a human name is never overwritten.
5. The renderer uses the same chain (`eqmac.exe` `t3d*` → TAKP `eqgfx_dx8.dll` → Trilogy `eqgfx_dx7.dll`),
   anchored by the Trilogy DLL's own 1,065 export names.

### 4.3 Hook DLL
- A launcher starts `eqgame.exe` suspended, injects `eqdecomp.dll`, then resumes. No original file is
  modified.
- At `DllMain` it checks the exe and each original's first bytes against a fingerprint, then writes
  `E9 rel32` at each enabled original address → the replacement.
- `__thiscall` stays exact (replacements are resolved from the link map, no pointer-to-member casts).
- If a smoke test breaks, `bisect_hooks.py` halves the enabled set until the culprit remains.

### 4.4 Unicorn differential test
- Load the original image's sections at their VAs and the VC6-built rewrite DLL at its base.
- Per case: seed the memory the function touches, run the original and the rewrite from identical snapshots,
  intercept every call leaving the function (record target and arguments, return a scripted value), and
  compare the return value, ESP delta, callee-saved registers, x87 depth, memory writes and the call trace.
- Fixtures are JSON under `tests/unicorn/<source path>/`, plus fuzz seeds. Values that would copy original
  data are stored as `from_image` references and read from the user's binary at test time.

### 4.5 Renderer (`eqgfx_dx7.dll`): how much work
| | `eqgame.exe` | `eqmain.dll` | `eqgfx_dx7.dll` |
|---|---|---|---|
| `.text` size | 1,254 KB | 248 KB | 480 KB |
| Free names | EQClassic notes | none | 1,065 exports |

- About 35–40% of `eqgame.exe`'s effort.
- Easier: very good names; much self-contained math and parsing that Unicorn tests well.
- Harder: heavy x87 code, DirectDraw/D3D7 COM calls that need stubbing, software/MMX render paths; visual
  correctness can only be smoke-tested in game.

---

## 5. Workflow
Queue → context bundle → rewrite → Unicorn → hook switchboard → state update. Every attempt and its diff is
recorded in the queue entry so the next attempt sees it.
Parallel work takes functions from different subsystems, and changes to `include/` go through one owner at a
time. Working rules: `docs/RULES.md`; tools by the problem they solve: `docs/tool-index.md`; VC6 codegen
patterns: `docs/VC6_QUIRKS.md`.

---

## 6. What is never committed
Original binaries (any client); Ghidra project files (they embed the binary's bytes); BinExport/BinDiff
databases; decompiler dumps and exports; third-party SDKs and toolchains. Reference projects are listed in
`reference/MANIFEST.md`, not vendored. Fixtures hold no original bytes (4.4).

---

## 8. Giants (functions of 4 KB and more)

- **One function in the source**, worked in **regions** (0.5–2 KB: a switch case group, an init block). Regions
  are a work and test unit only; the code stays one function in its original object, in address order.
- **Completeness is measured, not claimed.**

### 8.1 Tooling (Phase A)
- **A1** extent fix: switch-table bounds and byte tables recovered reliably (`unicorn/eqd_unicorn/extent.py`).
- **A2** **coverage gate**: original-side block coverage per fixture; the check unions a function's fixtures,
  prints uncovered spans and fails below 90% of blocks unless the fixture lists `coverage_ok` spans with
  reasons; an instruction-ratio check (rewrite/original < 0.7 with >= 100 instructions needs a reason)
  (`unicorn/eqd_unicorn/coverage.py`).
- **A3** the single-function runner passes the full listing; the context packer takes extent's switch list.
- **A4** **region kit**: slice the full listing by address range, coverage per range, `mutate_check --lines A-B`
  with a mutant budget scaled to size, and a skeleton generator (function frame, every switch with its case
  labels, `// REGION` markers) (`context/giant_kit.py`, `unicorn/tools/mutate_check.py`).
- **A5** recover code Ghidra's analysis missed (undecoded switch tails, functions never created)
  (`ghidra/scripts/recover_code.py`).

### 8.2 The routine (Phase B)
- **Default: one pass** for any giant whose full listing fits one pass comfortably (up to ~20 KB): the whole
  function and its fixture; done when the cases pass, coverage >= 90% (or `coverage_ok` reasons), the ratio
  check and the mutants pass.
- **Region giants** (too big for one pass):
  1. **Skeleton**: signature, frame/locals, top-level control flow, every case label, empty
     `// REGION Rnn 0xA-0xB: <label>` bodies, the shared fixture setup, and the whole spine (every statement
     on the path into each region, so each region's fixture can pass on its own).
  2. **Regions**: one region at a time; a change stays between its markers and owns its own fixture
     file (`<Giant>_Rnn.json`). Done = its cases pass, its range's coverage >= 90%, its range's mutants are
     caught.
  3. **Close**: all fixtures, whole coverage, ratio check, mutants over the whole body, one match pass.

---

## 9. Tooling adopted from other decomp projects

### 9.1 VC6 matching knowledge and a source permuter
VC6 codegen patterns (switch layout, argument evaluation order vs register allocation, inline budget, branch
order from nesting, EH state setup, stack-slot reuse) collected in `docs/VC6_QUIRKS.md`, each checked on SP4.
`permute/` is our VC6 source permuter; passes ported after a bake-off with gta2_re's cpp_permuter (MIT).

### 9.2 Delinking (COFF export from Ghidra)
Export address ranges of the original as relocatable COFF objects: target objects per function for diffing,
ground truth for relocations, and a route to Phase 3's address-assigning link (relink from delinked objects
plus rewritten ones).

### 9.3 objdiff
Diff viewer and progress reports over the delinked targets and our objects (`objdiff.json`).

### 9.4 reccmp
The LEGO Island toolset (reads VC6 PDB 2.0). `stackcmp` compares stack frames of near misses and feeds hints
to the permuter and the context bundles (`compare/stackcmp.py`, `compare/reccmp_map.py` maps our
`// FUNCTION:` annotations, no second set of comments). `vtable`, `datacmp` and a full run are Phase 3 items.

### 9.5 Context bundles
Similar already-matched examples in each context bundle (n-gram similarity over normalised listings).

### 9.6 Practices
- **Names and declaration order change code in old MSVC** (operand order and register ties follow the
  compiler's internal symbol order). After any rename pass, rerun `compare.py --match` on the whole file;
  for near misses that differ only in register choice or slot order, try renaming locals and reordering
  declarations before touching logic (both are permuter passes).
- Fake matches only after a real attempt, marked in the source; readability ratchets (casts, gotos, raw
  addresses) checked by `readability/gate.py`; every judge is negative-controlled with a planted bad case.
