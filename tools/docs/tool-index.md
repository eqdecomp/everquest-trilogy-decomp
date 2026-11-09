# Tool index: need -> tool -> command

Check here before writing a one-off script: most recurring needs already have a tool. Commands run from
the repo folder (`cd eq-trilogy`) unless they say "workspace" (run from the folder that holds the repo).
`PY` = `..\.venv\Scripts\python.exe` (from the workspace: `.venv\Scripts\python.exe`). Details are in each
folder's README.

## Set up and check

| Need | Tool | Command |
|---|---|---|
| What is missing (Python, packages, `paths.local.ini`, binaries + hashes, VC6 service packs) | `doctor.py` | `PY tools\doctor.py` |
| Another image of the repo (eqmain.dll, eqgfx_dx7.dll): client id, paths, toolchain | `unicorn/eqd_unicorn/clients.py` | `-c eqmain` / `-c eqgfx` on every tool; `docs/EQMAIN.md` |
| Repository checks CI runs (forbidden files, local paths, fixture JSON, annotations) | `ci/check_tree.py` | `PY tools\ci\check_tree.py .` |
| Progress table | `compare/progress.py` | `PY tools\compare\progress.py --compare trilogy=F.json ... --out PROGRESS.md` |

## Rewrite and test a function

| Need | Tool | Command |
|---|---|---|
| Everything known about a function in one bundle (needs a local Ghidra export) | `context/pack_context.py` | workspace: `PY eq-trilogy\tools\context\pack_context.py trilogy ADDR [ADDR ...]` |
| A fixture skeleton for a rewritten function | `unicorn/tools/make_fixture.py` | `PY tools\unicorn\tools\make_fixture.py trilogy 0x00410C80 [--run]` |
| Run fixtures (original vs rewrite under Unicorn) | `unicorn/` harness via pytest | `PY -m pytest tests/unicorn/<subsystem path> [-k Name -v]` |
| Prove the fixture catches wrong code (mutation check) | `unicorn/tools/mutate_check.py` | `PY tools\unicorn\tools\mutate_check.py --va 0x00410C80` (or a fixture path) |
| Test-DLL cache stats, prune, warm-up | `unicorn/tools/unicorn_cache.py` | `PY tools\unicorn\tools\unicorn_cache.py --stats \| --prune \| --clear \| --warm` |
| Per-file compiler options | `// CFLAGS:` line in the source | see `unicorn/README.md` "Per-source compiler flags" |
| Per-file toolchain (SP4/SP5) | `// TOOLCHAIN: vc6` line in the source, or the image's folder rule (`clients.py toolchain_dirs`) | see `docs/EQMAIN.md` "Per-file toolchain" |

## Match the original's code

| Need | Tool | Command |
|---|---|---|
| Is a rewrite instruction-identical (DLL build)? | `compare/compare.py` | `PY tools\compare\compare.py [src\...\X.cpp] [-v] [--va VA --show]` |
| Byte-match status for every rewrite, near-miss list | `compare/compare.py --match` | `PY tools\compare\compare.py --match --json ..\build\match\match.json --nearmiss ..\build\match\nearmiss.txt` |
| Search source variants for a near miss | `permute/permute.py` | `PY tools\permute\permute.py --snapshot --va 0x004A3911` (or `--nearmiss F`, `--match-json F --status diff`) |
| Status markers, unmarked fake-match oddities, readability ratchet | `compare/source_checks.py` | `PY tools\compare\source_checks.py [-v] [--update \| --raise-floor]` |
| Rename a function/global/local everywhere; prove nothing compiled differently | `readability/rename.py`, `readability/gate.py` | `PY tools\readability\rename.py -c eqmain 0x10009080 NewName_guess --dry-run`; `PY tools\readability\gate.py --base main` |

## Giants (functions >= 4 KB; `docs/PLAN.md` 8)

| Need | Tool | Command |
|---|---|---|
| Regions of a giant, with edges between them and the switch list | `context/giant_kit.py regions` | workspace: `PY eq-trilogy\tools\context\giant_kit.py regions trilogy 0042C654 [--out DIR] [--no-card]` |
| Per-region listing (`Rnn.asm`), approximate pseudocode slice (`Rnn.pseudo.c`), `INDEX.md` | `context/giant_kit.py slice` | `... giant_kit.py slice trilogy 0042C654` |
| C++ skeleton: `// FUNCTION`, signature, every switch with all case labels, `// REGION` markers | `context/giant_kit.py skeleton` | `... giant_kit.py skeleton trilogy 0042C654` (`all` = the three, plus a coverage/case-label check) |
| Mutants only in some lines / spread over the `// REGION` blocks | `unicorn/tools/mutate_check.py` | `--lines 120-340` (repeatable); `--region-markers [--region R05] [--max-per-region 3]` |
| Decode switches Ghidra missed, create functions it never made (vtable/table-only) | `ghidra/scripts/recover_code.py` | workspace: `PY eq-trilogy\tools\ghidra\scripts\recover_code.py trilogy [--dry-run] [--all]` |

## Hooks

| Need | Tool | Command |
|---|---|---|
| Hook DLL, hooks.ini, launcher; find a bad hook | not published | `hooks\build.bat`; `PY hooks\bisect_hooks.py`; `..\build\trilogy\hooks\eqlaunch.exe` |

## Ghidra project, names and types (workspace; order in `ghidra/README.md`)

| Need | Tool | Command |
|---|---|---|
| Create a client project (import + analysis) | `make_project.py` | `PY eq-trilogy\tools\ghidra\scripts\make_project.py trilogy --overwrite` |
| VC6 library signatures (FID) and applying them | `build_vc6_fid.py`, `apply_fid.py`, `fid_names.py` | `build_vc6_fid.py import LIB` / `build`; `apply_fid.py trilogy`; `fid_names.py trilogy` |
| BinExport/features, BinDiff pairs | `export_features.py` + `bindiff` | `export_features.py trilogy`; `bindiff --output_dir=build\bindiff A.BinExport B.BinExport` |
| Original source file per Mac function | `mac_debug_map.py` | `mac_debug_map.py` |
| Carry names Mac -> TAKP -> Trilogy; beta and string names | `name_chain.py`, `name_beta.py`, `eqd_match.py` | `name_chain.py takp`, `name_chain.py trilogy`; `name_beta.py [--validate]` |
| EQClassic source-comment names (the list it writes quotes EQClassic: never commit it) | `extract_eqclassic_notes.py` | `extract_eqclassic_notes.py` |
| Names from the rewrites' `// FUNCTION:` annotations | `annotation_names.py` | `annotation_names.py trilogy --committed --write` |
| Apply all name lists to the project | `apply_names.py` | `apply_names.py trilogy` |
| Headers -> Ghidra types; typed globals and signatures | `build_gdt.py`, `apply_types.py`, `apply_typed_symbols.py` | `build_gdt.py trilogy`; `apply_types.py trilogy`; `apply_typed_symbols.py trilogy` |
| How much is named/typed | `coverage_report.py` | `coverage_report.py` -> `build\coverage.md` |
| JSON export for the context packer and compare | `export/export_client.py` | `PY eq-trilogy\tools\export\export_client.py trilogy [--phase meta]` |

## Tool self-tests

`PY -m pytest tools\tests\<compare|context|doctor|export|ghidra|permute|readability|structs|unicorn>`. Many need
the binaries, VC6 or a Ghidra export and skip without them; the unicorn self-tests are slow, run only the
folder you changed.
