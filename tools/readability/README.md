# readability/

Tools for the readability work (names, labels, fields, globals, constants, comments): apply names safely and
prove the code did not change.

Run them from the repo folder with `..\.venv\Scripts\python.exe`.

| tool | what it does |
|---|---|
| `registry.py` | One table per client (trilogy, eqgfx, eqmain): VA → canonical name. Built from the `// FUNCTION:` definitions, fixtures' `"function"`, `names/*.csv` and `#define`s, plus each VA's spellings. Reports conflicts: `define_names`, `spelling`, `fixture_differs`, `csv_unapplied`, `ann_dup`, `name_dup`, `macro_shadows`. `--out DIR` writes `<client>.tsv` and `<client>.conflicts.tsv`. |
| `rename.py` | Renames a function, a VA global or a local/label everywhere it lives. Covers definitions, uses, headers, `#define` bodies, call macros derived from the old name, fixtures (`function`, `rewrite`, callers' `callees[VA].name`) and `names/<names>.manual.csv`. Uses C tokens, so string literals are never touched; comments get only code-like names. `--dry-run` lists every edit; `--map` applies a JSON list; `--journal` records what was done for the gate. |
| `gate.py` | The gate: every TU the edit touches compiles to the same code as on the base commit. It uses one shared match header and compares the objects function by function. It also checks that both builds compile, that no new C4005 appears, that source_checks counts per unit don't rise and markers/comment words don't fall (below), and it lints the fixtures. A retyped prototype (decorated name changed: `?f@@YAXH@Z` -> `?f@@YAXJ@Z`) is journaled (`protolink.py`, `--protos`/`--journal-out`) so callers compare as the same target; when anything is journaled or a definition is added/removed, a link check resolves the symbols over the changed TUs and every TU of that client naming them (LNK2005 duplicates, LNK2001 unresolved, C/C++ linkage mismatches). No fixtures are run, no mutation, no original binary. |
| `passes.py` | Report only: what the scripted passes (a) spelling, (b) csv names, (c) address calls/globals → names, (d) HWM opcodes and (e) duplicate defines would change. |

## Typical use

```
cd eq-trilogy
..\.venv\Scripts\python.exe tools\readability\registry.py --out ..\build\readability\registry
..\.venv\Scripts\python.exe tools\readability\rename.py -c eqmain 0x10009080 WrapTextIntoLines_guess --dry-run
..\.venv\Scripts\python.exe tools\readability\rename.py -c eqmain --map proposals.json --journal renames.jsonl
..\.venv\Scripts\python.exe tools\readability\gate.py --base main --renames renames.jsonl
```

The gate's default `--base HEAD` compares the working tree with its own commit; on a branch, pass
`--base main`. The before builds are cached under `<build>\readability\before\<base>\<header>`, so a second
run only compiles the after side: about 5 s per TU on 2 workers. Exit codes: 0 pass, 1 fail, 2 setup problem.

**Name-sensitive functions.** VC6's stack-frame layout can depend on the *names* of locals in some functions
(seen in rdp.c `process_network_connections_guess`: renaming even one local changes the codegen). The gate
catches it; keep such a function's locals as they are.

## Rules the tools enforce

- **One match header for both builds.** It is generated from the VA tokens of *both* trees. When the header
  differed between the builds, a function's code moved on its own (00428686 ndiff 37/47/77). A VA written
  in another letter case compiles to the same code and passes.
- **Symbols are keyed by what they are, not how they are spelled:**
  - `eqd_fn_/eqd_va_` by VA;
  - the TU's functions by their annotated VA;
  - statics and strings by section and offset;
  - other symbols by name, after the renames (from the TUs' annotations on both sides plus `--renames`).
- **Litter is ignored.** `.check/` output, `__pycache__` and build files are not "changed files".
- **A marker or identification word that disappears fails.** Per unit (source_checks notes): `@early-stop`-style markers,
  `fake match` comments (`/* fake match */` blocks and each line of a `// fake match:` note), `as original` notes, and the
  words zlib, `__cdecl`, `__thiscall`, `__fastcall`, `__stdcall`, `EAX`, `Returns` in comments. A dropped one hides an oddity
  or a lost fact.
- **A TU that differs only under the shared header gets a second look.** The union header can move one function's code on its
  own (1005FCB3, 00428686). When the only difference is under the union header, the TU is rebuilt with each tree's own
  generated header; if both sides then compile to the same code it passes, reported as `header-sensitive, own-header PASS`.
  Any difference under the own headers still fails.
- **source_checks compares counts per unit, not label text.** A renamed goto label or unused local keeps
  the same count. An unannotated unit is matched by name after the renames.
- **rename.py refuses (exit 2, nothing written):**
  - a new name that is a `#define` name anywhere: a macro named like a function rewrites its definition;
  - an existing identifier in the touched files (`--allow-existing` overrides this one);
  - an ambiguous simple name;
  - a new C4005 clash in a file;
  - a local that a `#define` body names (e.g. `roll` in `CCI_PICK_CITY`): it would compile but point at
    nothing.
  - a local named like a file-scope name of the file or its headers (types, globals, prototypes, enum
    constants).

## Pilot check

The pilot TUs were EQ_Character.cpp, unit_10008D60.cpp and unit_1000650E.c. Nine renames were applied:
- 3 functions: a member declared in `eq_character.h`, a `_guess` function, and a `FUN_` one whose
  `EQGFX_FUN_*_GUESS` call macro followed;
- 1 VA global (`PTR_SetTextColor_guess`, defined in two eqmain files);
- 3 goto labels;
- 2 locals: Ghidra's `param_1` and an unused local.

The gate built 7 TUs (the header pulled in EQSwitch, EQ_Character_MagicSystem and EQ_PC) and found 273
functions unchanged: PASS. The reverse map also passed.

Negative controls on the same files:
- a changed constant: FAIL, naming 10009080;
- a redefined macro: FAIL, C4005;
- one more cast: FAIL, source_checks `cast 21 -> 22`;
- a VA in lower case: PASS.

A tenth rename was refused: `roll` in FUN_0042a26f. The gate had first shown it breaking the build
(`C2065`) because `CCI_*` macros use that local.

## Known limits

- **Function renames replace the simple identifier in every file of the client.** A struct field or local
  with the same spelling would be renamed too. The gate catches any code change this causes, and the rename
  is refused if another function has that simple name.
- **The `_GUESS` suffix of derived macros follows the new name.** The round trip `EQGFX_FUN_X_GUESS` ↔
  `EQGFX_NAME_GUESS` is symmetric. Macros not derived from the old name are listed and kept; merging them is
  pass (e) (`--unify-defines` on a global).
- **Renaming a local to a plain word (`rows`) updates code, not prose.** Renaming it back leaves comments
  that already said "rows" alone. A comment that said `param_1` keeps "rows".
- **Ghidra is not updated here.** Run `ghidra/scripts/apply_names.py <client>` after a wave: rename.py
  writes `names/<names>.manual.csv` rows (`_guess` → `low`, else `certain`).
