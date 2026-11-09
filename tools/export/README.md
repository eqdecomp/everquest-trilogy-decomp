# export/

`export_client.py` exports a client's Ghidra project to JSON: the input for the work queue (`queue/`) and
the context packer (`context/`). Decomp repos see this folder as `tools\export\`.

```
.venv\Scripts\python.exe eq-trilogy\tools\export\export_client.py trilogy
    [--phase all|meta|decompile]   default all
    [--timeout 600]                decompiler seconds per function
    [--limit N]                    decompile at most N more functions, then stop
    [--retry-failed]               decompile timeouts/errors again
    [--restart]                    drop the pseudocode and start over
    [--out DIR]                    default <build>\export\<client>
```

- Opens the existing project (`<workspace>\ghidra\<client>\`) **read-only**: no import, no analysis,
  nothing saved. Build the project first (`ghidra\README.md`); re-export after any script that changes it
  (names, types, signatures). One Ghidra process per project at a time, so nothing else may have it open.
- Output goes to `<workspace>\build\export\<client>\`. It contains decompiler output and the client's bytes,
  so it is **never committed**; the script refuses to write inside a git repo.
- Trilogy, 3,241 functions: meta phase ~35 s, decompile phase ~5 min (slowest function 77 s), 48 MB (2026-10-01).
- **Resumable.** The meta phase is always rewritten in full. The decompile phase appends one line per
  function and flushes it; Ctrl-C or a crash loses at most the function in flight, and the next run carries
  on. When every function has a record, each pseudocode shard is compacted to one record per function in
  address order. A torn last line is cut off on the next run.
- **Fingerprint.** SHA-1 over every function's entry, size, full name and prototype. If it differs from the
  one the pseudocode was started with, the pseudocode starts over (decompiler output names callees and
  types, so it would be stale). `manifest.json` records it; `meta_fingerprint` != `fingerprint` means
  `functions.json` is older than the program (run `--phase meta`).
- Smoke test: `.venv\Scripts\python -m pytest eq-trilogy\tools\tests\export` (reads the export; no Ghidra).

## Conventions

- **Addresses** are strings of 8 upper-case hex digits without `0x` (`"004C8FCE"`), as in
  `eq-<client>\names\*.csv`. Every `address` / `to` / `from` / `function` / `site` field uses this form.
- Function order everywhere is ascending entry address. Functions are Ghidra's non-external functions
  (the same set `coverage_report.py` counts).
- A function's **shard** is `index // 100` of its position in that order, file name `NNNN.jsonl` (4 digits).
  The same file name is used under `asm\` and `pseudocode\`; `functions.json` gives it per function
  (`shard`). Shard size is in `manifest.json` (`shard_size`).
- `.json` files are one JSON object, UTF-8. `.jsonl` files are one JSON object per line, UTF-8, `\n`.
- Names are Ghidra's full names (`Class::Method_guess`, `USER32.DLL::SetRect` for imports).
- `null` means unknown / not applicable; lists are empty rather than `null`.
- `schema_version` (top level of every `.json`) is `1`. Additive changes keep the number; renames or
  removals bump it.

## Files

```
build\export\<client>\
  manifest.json
  functions.json
  callgraph.json
  strings.json
  globals.json
  asm\0000.jsonl ...
  pseudocode\0000.jsonl ...  pseudocode\_state.json
```

### manifest.json

| Key | Meaning |
|---|---|
| `schema_version`, `client`, `program` | `1`, `trilogy`, `eqgame.exe` |
| `binary_sha256` | SHA-256 of the imported file |
| `image_base` | e.g. `"00400000"` |
| `ghidra_version`, `project`, `project_file_version` | Where the export came from |
| `fingerprint` | Fingerprint of the program at the last run (see above) |
| `meta_fingerprint`, `meta_exported` | Fingerprint and time of the last meta phase |
| `shard_size` | `100` |
| `name_lists` | The `names\*.csv` files read |
| `library_span_estimate` | `{start, end, functions, not_fid_matched, not_fid_matched_unnamed}`: the statically linked CRT block as estimated by `coverage_report.py` (lower-case hex here, as in that report) |
| `counts` | `functions`, `classes` (`{class: n}`), `call_edges`, `indirect_calls`, `strings`, `globals`, `asm_shards` |
| `meta_seconds` | Duration of the meta phase |
| `decompile` | `complete` (bool: every function has a record), `statuses` (`{ok, timeout, error, pending: n}`), `failed` (`[{address, name, status, error}]`), `total_seconds`, `slowest` (`[{address, seconds}]`, top 10), `timeout` |

### functions.json

`{"schema_version": 1, "client": "trilogy", "functions": [ <record>, ... ]}`, records in address order.

| Key | Type | Meaning |
|---|---|---|
| `address` | hex | Entry point |
| `end` | hex | Highest address of the body (bodies can be non-contiguous; see `asm` `ranges`) |
| `size` | int | Bytes in the body |
| `instructions` | int | Instruction count |
| `name` | str | Full Ghidra name, with namespace (`EQ_Main::draw_guess`) |
| `short_name` | str | Without namespace |
| `namespace` | str/null | `null` = global |
| `name_source` | str | Ghidra symbol source: `DEFAULT` (`FUN_`), `ANALYSIS`, `IMPORTED` (our name lists, FID), `USER_DEFINED` (set by hand) |
| `is_default_name` | bool | Still `FUN_…` / `thunk_FUN_…` |
| `class` | str | Exactly one of `library`, `import_thunk`, `eh_funclet`, `named`, `unnamed`: the same classification and counts as `build\coverage.md` (`eqd_ghidra.function_class`) |
| `tags` | [str] | Sorted set: `library` (class library), `LIBRARY` (Ghidra tag), `fid` (Function ID match), `thunk`, `import_thunk`, `eh_funclet` (`Unwind@`/`Catch@` stub), `library_span` (inside the estimated CRT block, not tagged library, not an EH funclet: probably library code too), `noreturn`, `varargs` |
| `ghidra_tags` | [str] | Ghidra function tags as set in the project |
| `calling_convention` | str | Ghidra's name: `__cdecl`, `__stdcall`, `__thiscall`, `__fastcall`, or `unknown` (not yet set) |
| `signature` | str | Ghidra prototype, e.g. `undefined GetLanguageName()` |
| `signature_source` | str | `DEFAULT` (Ghidra guessed), `IMPORTED`/`USER_DEFINED` (set by `apply_typed_symbols.py` or by hand), `ANALYSIS` |
| `typed` | bool | `signature_source != DEFAULT` (the "typed" column of `coverage.md`) |
| `return_type` | str | |
| `parameters` | [obj] | `{name, type, storage}` (storage like `Stack[0x4]:4`, `ECX:4`) |
| `stack_purge` | int/null | Bytes the callee pops (`ret N`); `null` if unknown |
| `thunk_target` | str/null | Thunks only: target entry (hex) or external name |
| `name_list` | obj/null | Winning row of the `names\*.csv` lists for this address (priority as in `apply_names.py`: manual, fid, eqclassic, mac, strings, beta, takp): `{name, confidence, source, note, from_address, file}`, empty CSV cells as `null`. Confidence `certain`/`high`/`medium`/`low`/`guess`. `null` if no list names it (CRT names from Ghidra's own FID have no row) |
| `name_alternatives` | [obj] | Losing rows: `{name, confidence, source}` |
| `source_file` | str/null | Original source file from the Mac debug map, carried by the name chain (`file` column of the winning row, else of any row), e.g. `Source/PC Source/EverQuest/EverQuest.cpp` |
| `subsystem` | str/null | Grouping hint for the queue: `library` (class library or import_thunk, tag `library_span`, or a `library:` name row), else `source_file` without its `Source/PC Source/` / `Source/` / `GameLib/` prefix and extension (`EverQuest/EQ_Character`, `Network/rdp/rdp`, `zlib/inflate`), else `null` (about 60% of functions; the queue has to group those from the call graph) |
| `strings` | [hex] | Strings this function references (see strings.json), first-reference order |
| `globals` | [hex] | Non-code, non-string addresses it references (see globals.json), first-reference order |
| `callee_count`, `caller_count` | int | Lengths of the call-graph lists |
| `shard` | str | `NNNN.jsonl` under `asm\` and `pseudocode\` |

### callgraph.json

`{"schema_version": 1, "client": ..., "functions": {"<address>": {"callees": [...], "callers": [...], "address_taken": [...]}}}`,
one entry per function.

`callees`: every call site in the body in address order (`order` = index, 0-based):

| Key | Meaning |
|---|---|
| `site` | Address of the call (or tail-jump) instruction |
| `order` | Position among this function's callees |
| `kind` | `call` (direct call to a function in the program), `import` (to an imported DLL function, directly through the IAT or through an import thunk), `indirect` (register / memory / vtable call, target unknown), `tailcall` (unconditional `JMP` to another function's entry), `call_no_function` (call to an address with no function) |
| `to` | Called entry (hex), the import thunk's entry for `import` through a thunk; `null` for `indirect` and IAT calls |
| `name` | Callee's full name; for `import` the external name (`USER32.DLL::SetRect`); `null` for `indirect` |
| `operand` | `indirect` only: the operand text (`dword ptr [EAX + 0x10]`) |

`callers`: `{function, site, order, kind}` for every callee edge of another function whose `to` is this
function (`order` = the position of that site in the caller's `callees`). Sorted by caller, then site.

`address_taken`: non-flow references to the entry (function pointers, vtables, callback tables):
`{from, function, type}`; `function` is the function containing `from`, or `null` for data.

### strings.json

`{"schema_version": 1, "client": ..., "strings": [ {address, value, defined, type, block, xrefs}, ... ]}`

- Every string Ghidra defined in a data block, plus every printable NUL-terminated ASCII run (>= 4 chars)
  that code or data references but Ghidra left undefined (`defined: false`, `type: null`).
- `type`: Ghidra data type (`TerminatedCString`, `string`, `unicode` ...). `block`: `.rdata`, `.data`, ...
- `xrefs`: `[{from, function, type}]` sorted by `from`: every reference to the address
  (`type` = Ghidra reference type: `DATA`, `READ`, `WRITE`, `PARAM` ...; `function` `null` for data).
  Defined strings can have no xrefs (import names, unreferenced literals).

### globals.json

`{"schema_version": 1, "client": ..., "globals": [ <record>, ... ]}`: every address in a non-executable
block (`.rdata`, `.data`, `.rsrc`, `Headers`, `tdb`) that is referenced or has a symbol, except strings.
References into the middle of a struct or array are their own records (`data_start` gives the start of the
containing defined data).

| Key | Meaning |
|---|---|
| `address` | hex |
| `name` | Primary symbol (`g_pc_guess`, `DAT_006e4da8`, `PTR_…`, `switchdataD_…`) or `null` |
| `name_source` | Ghidra symbol source, or `null` |
| `block`, `writable`, `initialized` | Memory block and its flags (`initialized: false` = `.bss`-like) |
| `type`, `size` | Data type and length of the defined data containing the address; both `null` if undefined |
| `base_type`, `type_archive` | Type under pointers/arrays/typedefs, and the archive it came from (`trilogy` = our headers via `apply_types.py`) |
| `data_start` | Start of the containing defined data |
| `xrefs` | As in strings.json |

### asm\NNNN.jsonl

One line per function, shard order = address order:
`{"address", "name", "ranges": [[start, end], ...], "instructions": [[address, bytes, text, symbolic], ...]}`

- `bytes`: lower-case hex of the instruction bytes. `text`: Ghidra's plain disassembly (`MOV EAX,0x56b980`).
  `symbolic`: with labels and stack names (`MOV EAX,s_Common_Tongue_0056b980`).
- Data inside the body (switch tables) is not listed; instructions only.

### pseudocode\NNNN.jsonl

One line per function: `{"address", "name", "status", "error", "seconds", "signature", "c"}`

- `status`: `ok`, `timeout` or `error` (`error` has the decompiler's message). `c` and `signature` (the
  decompiler's prototype) are `null` unless `ok`. `c` includes the function's plate comment (name-list
  evidence, `ORIGINAL FILE:`).
- While a run is incomplete, a shard may hold several lines for one address (a retry): the **last** one wins.
  After a complete run there is exactly one line per function, in address order.
- `_state.json`: `{fingerprint, shard_size, started, updated, complete, statuses, failed, total_seconds, slowest}`.
