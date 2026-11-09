# eqgfx_dx7.dll: the renderer as a third client id (`-c eqgfx`)

Same model as eqmain (`docs/EQMAIN.md`): one client id, registered in `unicorn/eqd_unicorn/clients.py`, living in
eq-trilogy. Sources `src/eqgfx/`, fixtures `tests/unicorn/eqgfx/`, annotation `// FUNCTION: EQGFX 0x...`,
names `names/eqgfx_dx7.*.csv` + `names/siblings/eqgfx_trilogy.csv`, export `build/export/eqgfx`.

## The image (measured)

- 745,472 bytes, base 0x10000000, `.text` 479,232 B. Ghidra project `ghidra/eqgfx`: 2,169 functions (326 FID/library,
  3 import thunks, 83 EH funclets; 908 named: ~836 are the DLL's own PE exports `t3d*`/`s3d*`, 72 from the name lists).
- Toolchain `vc6` (SP4). Rich header: 76 C objects at 8799 (SP4 cl), 12 C + 3 C++ at 8966 (SP5), 4 C++ at 8799; the CRT
  is SP4 LIBCMT (8797). The renderer is mostly **C** (the t3d library). Default flags are right: four pilot
  rewrites compile byte-identical with `/O1 /W3 /MT /Gy /GX` (push imm8/pop reg 0.96 per KB like eqgame.exe; strlen is
  a call, not inlined). Not measured yet: whether any object needs `// TOOLCHAIN: vc6_sp5` or another `// CFLAGS:`.
- CRT block 0x10067942-0x10075A66 (`library_span`). No `__ftol`: float to int is inline `fistp` (check `/QIfist`-like
  behaviour when a rewrite casts). `crt_defaults_eqgfx.json` holds the CRT callees/aliases (strcpy = the 7-byte
  entry before strcat, `__chkstk` = `__alloca_probe` 0x100691D0); `test_crt_defaults.py` checks every alias.
- Shared with eqgame.exe (BinDiff `trilogy_vs_eqgfx`, 1647 pairs): the ffcreate2 packfs/verify code
  (0x10051E80-0x10054A70, 57 pairs at 1.000), red-black tree helpers (0x10019C6A-0x1001A43D, 0x1001A0F7 clone is
  only structural: the template's eqgame code differs in register use), and **zlib 1.1.2**
  (0x10054AC0-0x10059A7F, inflateInit2_ to inflate_fast; `THIRD_PARTY_RANGES["eqgfx"]` -> state `library`). Everything else of the renderer is its own.
  Tiny functions BinDiff pairs by "address sequence" are accidents: filter pairs by size.
- Rewrites reach the image's imports as `user32.dll!GetDC`-style scripted callees (works: t3dRestoreSystemColors)
  and indirect calls through object memory (t3dSendMessageToActor). The VC6 SP4 headers are DX5-era
  (`DDRAW.H`/`D3D.H` have no `IDirectDraw7`/`IDirect3DDevice7`): D3D7/DDraw7 calls need hand-written vtable slots.

## Rebuild order (Ghidra side)

```
set EQD_ROOT=<workspace>   (EQD_REPO=<wt> when working in a worktree)
ghidra\scripts\apply_fid.py eqgfx --tag-only        # without vc6_x86.fidb: tags the analysis' own FID matches
ghidra\scripts\export_features.py trilogy ; ... eqgfx      # BinExports (7 + 2 min)
bindiff --primary=trilogy.BinExport --secondary=eqgfx.BinExport --output_dir=build\bindiff   # 3 s
ghidra\scripts\apply_names.py eqgfx                 # names\eqgfx_dx7.{trilogy,strings}.csv
export\export_client.py eqgfx                       # 2 min
```
`names/eqgfx_dx7.strings.csv` (8 rows) came from `Name: ...` debug strings of unnamed functions; extend it the same way.
No object boundaries exist in the t3d code (no padding between functions, one 558-function address run), so units are
cut by count (about 40 functions per unit, at the biggest .data gap near the even cut). Rewrites keep their file.

## Not done / later

- FID database for SP4 CRT (`ghidra\vc6libs` + `build_vc6_fid.py`, hours): only 40 CRT-span functions lack a FID match
  and they are already `library_span`; low value.
- Names from TAKP `eqgfx_dx8` (2,740 entries, `takp_lists\eqgfx_dx8.dll_takp_function_list.txt`) and Mac `t3d*`
  symbols: the Mac BinDiff (`mac_vs_eqgfx`) has only 8 pairs >= 0.9. Needs a TAKP eqgfx_dx8 Ghidra project, its
  BinExport and a BinDiff against eqgfx; then `name_chain.py`-style carry-over.
- Hooks (`hooks=False`): eqgfx_dx7.dll stays loaded and prefers 0x10000000 like eqmain.dll (docs/EQMAIN.md "Hooks").
- `recover_code.py eqgfx`: 12,028 bytes of `.text` are not in functions (2.5%).
- Clones: 62 eqgfx functions have a same-shape eqgame.exe sibling (about 50 packfs/verify functions) whose rewrite
  can be carried over.
- Naming: some rewrites add `_guess` to export names (certain names); `annotation_names.py` ignores the suffix, so
  the Ghidra name is untouched, but the source reads wrongly.

