# ghidra/scripts

Headless PyGhidra scripts. Run them with the workspace venv from the workspace root; they find
`paths.local.ini` by walking up (or via `EQD_ROOT`). Projects go to `<workspace>\ghidra\`, reports to
`<workspace>\build\ghidra\`. Neither is ever committed.

PyGhidra comes with Ghidra, not PyPI:
```
.venv\Scripts\python -m pip install --no-index --find-links <ghidra>\Ghidra\Features\PyGhidra\pypkg\dist pyghidra
```

Rebuild everything from scratch, in this order:
```
make_project.py trilogy|takp|mac|beta --overwrite  import + auto-analysis        -> <client>.project.json
build_vc6_fid.py import LIBC|LIBCMT|LIBCP|LIBCPMT|MSVCRT [--toolchain vc6|vc6_rtm|vc6_sp5] --overwrite
                                               (one process each, parallel is fine; ~1 h when all run at once;
                                               toolchain defaults to vc6 = SP4, see Notes)
build_vc6_fid.py build [--toolchains vc6,vc6_rtm,vc6_sp5]   -> build\ghidra\vc6_x86.fidb (all variants by default)
apply_fid.py trilogy|takp|beta|eqmain|eqgfx    FID match + LIBRARY tag       -> <client>.fid.json
                                               (short helpers below FID's score threshold: fid_names.py below)
extract_eqclassic_notes.py                     -> eq-trilogy\names\eqclassic_notes.tsv (no Ghidra needed)
export_features.py trilogy|takp|mac|beta       -> <client>.BinExport + <client>.features.json (parallel is fine;
                                               run before apply_names.py so the diffs stay name-blind)
mac_debug_map.py                               -> mac.debugmap.json: original source file per Mac function
bindiff --output_dir=build\bindiff <a>.BinExport <b>.BinExport      for mac/takp, takp/trilogy, beta/trilogy
name_chain.py takp                             -> build\names\takp\eqgame.chain.csv, takp.chain.json
name_chain.py trilogy                          -> eq-trilogy\names\eqgame.{mac,takp}.csv, trilogy.chain.json
name_beta.py [--validate]                      -> eq-trilogy\names\eqgame.{beta,strings}.csv, beta.names.json, build\names\beta\
fid_names.py trilogy                           short CRT helpers FID can't score -> eq-trilogy\names\eqgame.fid.csv
annotation_names.py trilogy [--committed] [--write]   the rewrites' `// FUNCTION:` names that differ from the
                                               export -> rows in eq-trilogy\names\eqgame.manual.csv (no Ghidra;
                                               needs the export; then apply_names, export)
apply_names.py takp|trilogy                    applies the name lists        -> <client>.names.json
build_gdt.py trilogy|eqmain|eqgfx [--out FILE]   all nested include/<client> headers -> reports/<client>.gdt
                                               generates headers/<client>.h under reports; checks all source assertions
                                               --layouts DIR --overlays DIR --field-edits FILE adds canonical checks
                                               --canonical warn: canonical drift is a report warning, not a failure
                                               types filed under /<declaring header> (as the program's old types)
                                               no project needed -> <client>.gdt.json (validated=false on failure)
apply_types.py trilogy [--archive FILE]         loads the .gdt into the project (replace, idempotent) -> <client>.types.json
                                               never replaces another archive's type (kept_foreign)
apply_typed_symbols.py trilogy [--symbols CSV]  types globals + sets signatures from <repo>\ghidra-types\<client>.symbols.csv
                                               (never renames functions; idempotent) -> <client>.typed.json
recover_code.py trilogy [--dry-run] [--only send_social,0042A26F] [--all]
                                               code the analysis missed: send_social's second switch (computed jump
                                               with no range check: refs + override + body refit) and functions that
                                               exist only as .rdata/.data pointers (0042A26F; --all = every clean one;
                                               the rest listed with a reason) -> <client>.recover.json + .missed.tsv.
                                               Idempotent; --dry-run rolls back. Run after the analysis/names/types
                                               steps, before export_client.py (see Notes)
coverage_report.py                             -> build\coverage.md
delink.py trilogy [--va VA ...|--state S|--subsystem S|--match-status S] [--units]
                                               read-only: COFF objects from the delinker (below)
                                               -> build\trilogy\delink\<VA>.obj, units\<unit>.obj, delink.json
..\..\export\export_client.py trilogy          read-only JSON export for the queue / context packer
                                               -> build\export\trilogy\ (schema: export\README.md)
```

All three Trilogy clients resolve to `eq-trilogy` through the shared client registry.
`EQD_REPO` selects a source worktree; `[repos] <client>` in `paths.local.ini` can also route
each client. `EQD_CONFIG` selects an alternate INI without editing the workspace configuration.
`[workspace] ghidra_reports` or `EQD_REPORT_DIR` routes generated headers, default archives and
reports to a session directory. `[ghidra_types] <client>_archive` and `<client>_symbols` select
explicit paths (relative to the workspace, or absolute). Both application scripts accept
`--project-dir DIR` for a scratch copy. Archive layout validation must pass before applying it.

The name chain (`eqd_match.py`, `name_chain.py`) needs no Ghidra, only the exported files. Evidence for
a pair of functions: a unique shared string, unique shared constants, a BinDiff pair, or the same call
position in an already matched caller. Measured on Mac -> TAKP with half of the name anchors held out:
strings and constants were right every time, call position 97%, BinDiff alone 85%, and only 28% of the
held-out functions were found at all. Between GCC and VC6 the join by name does the real work.

Notes:
- The FID database holds one library variant per VC6 service pack (`[toolchain]` keys in paths.local.ini):
  `vc6` = SP4 (CRT 8797; eqgame.exe, eqgfx_dx7.dll), `vc6_rtm` = RTM (8168), `vc6_sp5` = SP5 (8047; TAKP's CRT).
  The CRT differs between service packs, so import the one the client was linked with (Rich header, see
  `eq-trilogy\reference\VERSIONS.md`). Library projects live in `ghidra\vc6libs\<toolchain>\<LIB>\`.
- Never run a script from a directory that has a `ghidra\` folder on `sys.path`; it shadows the Java
  package. `eqd_ghidra.start()` strips such entries.
- `make_project.py` raises the decompiler switch-analysis timeout to 900 s. With the default 60 s the
  clients' largest functions lose their switch targets (Trilogy: 10% of `.text` left undisassembled).
- One process per Ghidra project at a time (project lock).
- `recover_code.py`: the project keeps its FID/names/types; it only adds COMPUTED_JUMP references, a decompiler
  jump-table override, the table as a pointer array and the functions it creates (`FUN_<va>`, so run
  `apply_names.py` again for any name list that covers them), then re-export (`export_client.py`, meta +
  decompile). The `.missed.tsv` lists every pointer from `.rdata`/`.data` into
  uncovered `.text` with a verdict (`clean` = ret-terminated, >= 5 instructions, plausible first
  instruction; `review` = decodes by chance or too short); `clean` is a candidate, not a proof: check the
  reason before `--only <va>` or `--all`.

## Delinking (`delink.py`, ghidra-delinker-extension)

[ghidra-delinker-extension](https://github.com/boricj/ghidra-delinker-extension) v0.9.2 (prebuilt for 12.1.3)
rebuilds relocations from the Ghidra database and exports address
ranges as relocatable COFF objects. Install the release zip with File > Install Extensions (it lands in
`%APPDATA%\ghidra\ghidra_12.1.3_PUBLIC\Extensions\`). PyGhidra loads user extensions from there without
help: `Application.getApplicationLayout().getExtensionInstallationDirs()` lists that folder.

`delink.py` runs the extension's `Relocation table synthesizer` analyzer over the chosen functions (thunk
redirection off: a call to a thunk stays a call to the thunk), renames every symbol the objects define or
reference to `_eqd_fn_0x<VA>` (functions) / `_eqd_va_0x<VA>` (other targets), the names match mode gives
client addresses, and exports with the extension's COFF exporter. All of that happens in one transaction that
is rolled back: the project is never changed or saved. One object per function (`<VA>.obj`: the body range
at the entry plus the switch tables after it) and, with `--units`, one per objdiff unit
(`compare\objdiff_config.py`). Uses: target objects for objdiff and cpp_permuter, relocation ground truth
for `compare.py --match --target-obj` (`compare\README.md`), and later the relink of Phase 3.
Names use 8 upper-case hex digits; sources that write a VA token in lower case (`0x005316c0`) give a
differently spelt symbol in the match objects. `coff.py` compares them by value, but a real link would not
resolve them; normalise the tokens before a relink.
