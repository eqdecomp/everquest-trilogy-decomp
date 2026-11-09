# More than one image per repo: eqmain.dll (and eqgfx_dx7.dll)

eqgame.exe's pipeline is the default: every default below is what it is for `-c trilogy`.

## Model: one client id per image

`-c/--client` already threads through every tool (export, build dirs, harness). A second image
of a repo is simply another client id, registered in **`unicorn/eqd_unicorn/clients.py`** with the repo it
lives in. Nothing is hardcoded to eqmain: `eqgfx` (eqgfx_dx7.dll, base 0x10000000, SP4 plus some SP5 objects)
is registered the same way (`docs/EQGFX.md`).

| | `trilogy` (eqgame.exe, primary) | `eqmain` (eqmain.dll) |
|---|---|---|
| repo | eq-trilogy | eq-trilogy |
| annotation | `// FUNCTION: TRILOGY 0x...` | `// FUNCTION: EQMAIN 0x...` (token = id upper-cased) |
| sources / fixtures | `src/<sub>.cpp`, `tests/unicorn/<sub>/` | `src/eqmain/<sub>.cpp`, `tests/unicorn/eqmain/<sub>/` |
| names | `names/eqgame.*.csv` | `names/eqmain.*.csv`; `names/siblings/eqmain_trilogy.csv` |
| export, builds | `build/export/trilogy`, `build/trilogy/...` | `build/export/eqmain`, `build/eqmain/...` |
| match | `build/match/match.json` | `build/match/eqmain.json` |
| toolchain | `[toolchain] vc6` (SP4) | `[toolchain] vc6_sp5` (SP5); `src/eqmain/Network/rdp/`: `vc6` (SP4) |
| default flags | `/O1` | `/O2` (by path, `vc6.image_cflags`) |
| fixture defaults | `tests/unicorn/crt_defaults.json` | `tests/unicorn/crt_defaults_eqmain.json` |
| hooks | `hooks/` (eqdecomp.dll) | none yet (`hooks=False`) |

`EQD_REPO=<dir>` makes a worktree or copy stand in for the repo of every client (names, the annotation
scans).

## Flags (measured)

eqmain.dll's Rich header says SP5 (c2 8966, LIBC 8047) plus 30 C objects at 8799 (the rdp library eqgame.exe
links too). Its game code inlines `strlen`/`strcpy`/`memcpy` (524 `repne scasb`, 738 `rep movs`; eqgame.exe 62)
and has none of /Os's `push imm8; pop reg` (eqgame.exe 826): **/O2**. Pilot: the first rewrites compile
byte-identical with SP5 /O2.

### Per-file toolchain

The Rich header's C objects split 30 at 8799 (SP4: the rdp library) and 15 at 8966 (SP5); every C++ object is
SP5. A source picks another `[toolchain]` of paths.local.ini with a line `// TOOLCHAIN: <key>` (` * TOOLCHAIN:`
in a block comment, `/* TOOLCHAIN: <key> */`); without one, its image's folder rule applies
(`clients.py toolchain_dirs`: eqmain's `Network/rdp` is `vc6`), else the image's own toolchain. Resolution:
`clients.source_toolchain` (key) and `vc6.source_vc6` (folder; `EQD_TOOLCHAIN_<KEY>` overrides the ini). Honoured
by the harness build (`vc6.build_dll`: one cl run per (toolchain, flags) group; the test DLL is linked with the
image's toolchain; the build cache keys every toolchain used), `compare --match` (and so objdiff's base objects), `permute` (candidates compile `like` their repo source: its image's flags and
folder rule, its TOOLCHAIN line kept in the cut-down unit), `stackcmp`, and the context bundle
("Visual C++ 6.0 SP4 (this file's toolchain ...)"). eqgame.exe's defaults are unchanged (no rule, no lines).

Measured 2026-10-09: SP4 and SP5 give byte-identical code for every one of the 1596 eqgame.exe functions
compare --match builds (two full runs, one with `// TOOLCHAIN: vc6_sp5` added to every source) and for the
eqmain rdp functions; the objects differ only in their `@comp.id` (0x000A225F = C 8799 vs 0x000A2306 = C 8966),
which is what the Rich header counts. So the switch decides the object's compiler id (needed for an exact
rebuild, and the reason rdp defaults to SP4), not instruction matching: no function matches with one and not
the other (C1.DLL is the same file in both; C2.DLL, the back end, differs).

## Harness

The original is mapped at its preferred base (0x10000000; nothing is relocated, so its `.reloc` is irrelevant),
the test DLL at 0x30000000, the fixture heap at 0x60000000: no overlap. `tests/unicorn/conftest.py` keeps one
Harness per image and picks it from the fixture's folder (`tests/unicorn/eqmain/...` -> eqmain): binary,
toolchain, function starts (`build/export/eqmain` + `names/eqmain.*.csv`) and fixture defaults. `make_harness`,
`crt_defaults`, `link_sources`, `function_starts` take the client (default trilogy); `eqd_repo.Repo` passes it.
Match mode lays the rewrite object out at 0x40000000 (`coff.ALT_OBJ_BASE`) and the non-match compare loads the
test DLL at 0x30000000 whenever the original covers 0x10000000 (stackcmp links its DLL there too). At run
time eqmain.dll and eqgfx_dx7.dll both prefer 0x10000000, so one of them is relocated: see "Hooks (design)".

## Repo layout

- Headers: `include/trilogy/` is shared (the same game structs); eqmain-only address macros go in the source file
  or `include/eqmain/`. The context bundle reads `include/eqmain/` and `include/trilogy/`.
- The images' sources never mix: the eqgame hook build skips `src/eqmain/` (`hooks/eqdecomp.py OTHER_IMAGES`),
  `compare.py`'s default sources and `clients.sources()` take only the client's own files.

## Subsystems

There is no Mac map for eqmain.dll, so subsystems come from (in order): **siblings** (BinDiff `trilogy_vs_eqmain` pairs >= 0.8 of game functions; the eqgame function's subsystem, prefixed:
`eqmain/Network/rdp/rdp`, `eqmain/ffcreate2/ff/packfs`), address neighbours between equal sibling labels,
rewrites' files, then **address-run units** `eqmain/unit_<first VA>` (consecutive unlabelled functions, split
at a labelled function or a data gap of 5+ non-literal `.data` items). Namespace and
call-graph rounds are off for eqmain (`graph_labels=False`): with ~170 anchors they would drag front-end code
into rdp. 2026-10-09: 660 todo, 155 sibling, 43 address, 467 unit labels in 30 units.

## rdp structs (eqmain links another rdp version)

The same rdp code, other layouts: eqmain's lists are intrusive (`RdpList_guess` 0x18 bytes: head, tail,
link_offset, key_offset, count, compare; a link is only next/prev, and the list functions take and return
items), its bit windows are 1024 bits, so `RdpConnection_guess`'s fields move (connected 0x77C -> 0x6D4,
remote 0x4A8 -> 0x478, ...), `RdpState_guess`'s serial queue is at 0x140 and the global counters have one more
entry. `include/eqmain/rdp.h` holds eqmain's own layouts under the same struct and field names as
`include/trilogy/rdp.h` (each field comment gives the eqmain offset and the eqgame one), so code written with
struct fields compiles unchanged. Every `src/eqmain/Network/rdp/` source includes it instead of
`trilogy/rdp.h` (it `#error`s if both are included).

## Other tools per image

- `permute/permute.py -c eqmain --nearmiss|--match-json ...`: the image's sources, annotations, CRT aliases and
  defaults (`<build>/permute/eqmain/work`, `<build>/match/eqmain/permute`); candidates compile `like` their
  repo source (image flags /O2, toolchain folder rule; a `// TOOLCHAIN:` line stays in the cut-down unit).
  Checked on 3 `diff` functions: start scores 70/10/70 = compare --match's 7/1/7 differing instructions.
- `compare/stackcmp.py --client eqmain --nearmiss F`: client toolchain per source, the image's CRT defaults and
  reccmp target id `EQMAIN`, DLL linked at 0x30000000. Checked on the same 3: reccmp pairs all of them.
- `compare/objdiff_config.py --client eqmain write|report|check`: the image's sources; objdiff.json
  goes to `<build>/eqmain/objdiff_project/` (`--project`), its build command carries `--client eqmain`. Target
  objects: `ghidra/scripts/delink.py eqmain --units --no-functions` (its --units repo is now the client's).
  objdiff-cli 3.8.2 hangs on two of eqmain's units (`eqmain/unit_10008090`: FUN_10008150; `eqmain/unit_10018B60`):
  pass `--skip-unit` for both; report/check stop after `--timeout` (900 s) instead of hanging. Last run:
  63 units, 5 of 3885 objdiff functions matched; check: 5 of 6 byte-identical at 100%.

## Hooks (design, not built)

eqmain.dll is loaded by eqgame.exe for the front end and freed again; eqgfx_dx7.dll stays loaded and also
prefers 0x10000000, so eqmain.dll usually lands elsewhere. Plan:
1. **One hook DLL, one table per image.** eqdecomp.dll (already injected) builds `src/eqmain/` too (each source
   with `vc6.source_vc6`'s toolchain: hooks/eqdecomp.py `compile_one` uses the default one today; it skips
   src/eqmain/ for now) and gets a second table: eqmain RVAs + fingerprints.
2. **Install on load.** eqdecomp.dll hooks eqgame's `LoadLibraryA` IAT slot; after a load whose module is
   eqmain.dll it checks the fingerprints at `base + RVA` and patches the jmps there (and again on every later
   load: the image is fresh each time); nothing to undo on FreeLibrary.
3. **Rebased addresses.** Rewrites keep writing preferred VAs (0x10000000-based; match mode and the harness
   need them as they are). For the hook build only, `eqaddr.h` gets an `EQD_IMAGE_DELTA` variant:
   `EQ_GLOBAL/EQ_ARRAY/EQ_FUNC(.., va, ..)` use `(va) + eqd_image_delta`, a variable the installer sets to
   (actual base - 0x10000000) before enabling the table; eqmain sources are compiled with it, eqgame's without.
   Calls between rewrites stay direct (both live in the hook DLL).
4. eqgfx_dx7.dll (later) is the same with its own table and delta; whichever of the two the loader relocates
   just gets a non-zero delta.
Not done now: it needs the LoadLibrary hook in hooks/decomp/eqdecomp.c, the second table in hooks/eqdecomp.py
and a launcher test with the game, none of which is simple to verify offline.

