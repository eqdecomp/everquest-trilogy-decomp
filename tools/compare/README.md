# compare/ – is a rewrite instruction-identical to the original?

`compare.py` builds the rewrites with the test DLL's flags (`eqd_unicorn.vc6.build_dll`: `DEFAULT_CFLAGS` plus
each source's `// CFLAGS:` lines, see `unicorn/README.md`) and compares every `// FUNCTION: <CLIENT> 0x...`
function instruction by instruction with the original. It says nothing about behavior (that's the Unicorn
test); it measures how close the code shape is, for flag choices and later matching (PLAN Phase 3).

```
cd eq-trilogy
..\.venv\Scripts\python.exe tools\compare\compare.py                       every src\**\*.c / *.cpp
..\.venv\Scripts\python.exe tools\compare\compare.py src\Network\rdp\heap.c -v     + unified diffs
..\.venv\Scripts\python.exe tools\compare\compare.py --va 0x4F4D10 --show          both normalised listings
..\.venv\Scripts\python.exe tools\compare\compare.py --json ..\build\trilogy\compare.json
```
Options: `--repo` (default `.`), `--client trilogy`, `--out` (DLL folder, default
`<build>\<client>\compare\`), `--aliases` (fixture defaults with `rewrite_aliases`, default the repo's
`tests\unicorn\crt_defaults.json`). Needs the workspace (`paths.local.ini`, the original exe, VC6, and the
Ghidra export for function starts and library names).

## How

1. **Build.** All given sources in one DLL (cross-file calls resolve, as in the hook DLL); if that fails, one
   DLL per source, and a source that doesn't build reports `error` for its functions.
2. **Entry.** The annotation's name undecorated against the map, in that source's own object; overloads are
   told apart by a `/FAs` listing (first body below the annotation), as `hooks\eqdecomp.py` does.
3. **Extent.** Recursive descent on both sides (`eqd_unicorn.extent`, as the harness), listed in address
   order.
4. **Normalise** every instruction to `mnemonic operands`, then compare the two lists:
   - branches inside the function become `@<index>` (the target's index among the instructions left after
     folding, step 5, so a folded `xor edx, edx` doesn't renumber the labels after it); calls and tail jumps become `F:<original VA>`: the
     original's targets by VA; the rewrite's by what the symbol is: another annotated rewrite → its
     original VA, a `rewrite_aliases` CRT symbol → its client VA, other library code (`_malloc`,
     `??2@YAPAXI@Z`) → the client function of the same name in the export, else `F:<symbol>`;
   - imports (`[IAT]`) become `I:dll!name`; addresses of the function's own code (switch tables) `CODE`;
   - the rewrite's relocated addresses of its **own data** (string literals, float constants) match the
     original's address at the same place when the contents agree (same C string, or the same bytes for
     the operand size). Each match counts as `data`;
   - unrelocated values in the rewrite (fixed client addresses via `EQ_GLOBAL`, `EQ_CDECL`, ...) are keyed
     exactly like the original's.
5. **Fold** (both sides): `mov r32, <function>` whose register is only read by later `call r32` / `jmp r32`
   becomes `call <function>` / `jmp <function>`, the mov dropped. VC6 compiles a call through a constant
   function pointer (`EQ_CDECL`/`EQ_STDCALL`/`EQ_THISCALL`) that way, where the original has `call rel32`.
   An `xor edx, edx` whose EDX nothing reads before the next transfer of control, when that is a folded
   call, is dropped too (the dummy EDX argument of `EQ_THISCALL`). Each counts as a fold.

## Verdicts
- `identical`: the same instructions after address keying, nothing folded or matched by content.
- `identical_normalised`: identical after folds and/or data matches (counts shown).
- `differs`: the first differing instruction, `-v` the unified diff. `ratio` is difflib's similarity with
  branch labels ignored (one extra instruction would renumber every later label).
- `error`: build failure, no or ambiguous symbol, undecodable extent.

The folds hide one known, mechanical difference of the fixed-address call macros; register allocation,
block order, frame pointers and instruction choice are never normalised. A real `call rel32` to a fixed VA
from a relocatable DLL needs a different call mechanism (PLAN Phase 3).

## Match mode (`--match`): byte-matching measurement

```
..\.venv\Scripts\python.exe tools\compare\compare.py --match --json ..\build\match\match.json ^
    --nearmiss ..\build\match\nearmiss.txt
```
Each source is compiled on its own to a COFF object (SP4, `cflags_for`: defaults + its `// CFLAGS:`) with
`/DEQD_MATCH`: `include/trilogy/eqaddr.h` then expands `EQ_CDECL/EQ_STDCALL/EQ_THISCALL` to the external
function `eqd_fn_<va>` and `EQ_GLOBAL/EQ_ARRAY` to the external object `eqd_va_<va>` (`<va>` = the VA token
as written, e.g. `eqd_fn_0x004A3D01`), so calls compile to `call rel32` and globals to absolute operands,
each with a relocation, as in the client. The tool scans `include/` and `src/` for the macro uses and
force-includes the generated declarations (`<out>\eqd_match_syms.h`, `/FI`). Objects are never linked into
a DLL; `coff.py` lays one out flat, applies its relocations with every `eqd_*` symbol at its own client VA
(other externals in a scratch area keyed like the DLL mode: rewrites, CRT by name, `__imp_` imports; a call to
another TU's function by the VA its definition is annotated with, see below), and
the same normalisation runs. On top, **bytes**: the original's bytes against the object's with the
relocation fields (and rel32 of calls leaving the function) masked at the same offsets.

Absolute symbols resolve to their value, unrelocated: `IMAGE_SYM_ABSOLUTE` ones and LIBC's `__except_list`
(= 0, so the C++ EH prologue's `fs:__except_list` reads `fs:[0]` like the original; `coff.ABSOLUTE_SYMBOLS`).

**Cross-TU calls** (`xtu.py`, `MatchContext.cross_tu`): a call by symbol to a function another source defines,
e.g. the base constructor `??0EQ_Overlay@@QAE@XZ` (EQ_Overlay.cpp, `// FUNCTION: TRILOGY 0x00450049`) called
from `EQ_Bag_guess::EQ_Bag_guess`, is keyed `F:450049` like the original's `call 0x00450049`. The map is
decorated name -> VA, taken from the built objects: a function defined once under a name annotated with one VA
in its source. Sources not given on the command line are built (into `--out`) when an annotation names a
reference. An overload that shares an annotated undecorated name, an unannotated definition and a symbol no
built source defines keep `F:<symbol>` (a difference); a definition annotated at another VA differs as that VA.
With the map, the older undecorated-name rule (`by_name`) no longer applies to externals; permute's scoring
(no map) keeps it.

**Declared-only client functions** (`// EXTERNAL:` markers, `unicorn/eqd_unicorn/externs.py`): a source can
declare a real class whose constructor (or any method or free function) it calls but does not define, and say
where the client has it:
```
class GroundItemNode_guess
{
public:
    // EXTERNAL: TRILOGY 0x004A02E5 GroundItemNode_guess::GroundItemNode_guess
    GroundItemNode_guess(void);
    ...
};
... node = new GroundItemNode_guess;        // call ??0GroundItemNode_guess@@QAE@XZ, keyed F:4a02e5
```
`MatchContext.externals` maps the object's undefined externals by those markers (decorated name -> VA) and the
call is keyed by the VA, like the original's direct call to the constructor; plain C++ construction then
replaces a wrapper (a local class whose inline constructor calls `EQ_METHOD`, which VC6 emits out of line: one
call level more than the original). Markers count in the source and in the headers under `include/` (the
source's own win); the name is the undecorated `Class::Class` / `Class::~Class` / `Class::Method` / `function`
(one-identifier `#define`s of the file applied, as for `// FUNCTION:`), or the decorated symbol when the object
references several overloads of it. Disagreeing markers and ambiguous overloads are logged and left unmapped (the
call keeps `F:<symbol>`); a definition another TU annotates (cross-TU map) wins over a marker. Permute's scoring
gets the same keying (it goes through `compare_obj`). The VA token also counts for `eqd_match_syms.h` (above),
so moving a call from `EQ_METHOD(..., 0x004A02E5, ...)` to a marker with the same token leaves the generated
header, and every other TU's code, unchanged. DLL builds and the fixture harness resolve the same markers
(`unicorn/README.md`, "Declared-only client functions"); the reccmp view (`reccmp_map.py`) blanks them.

**Vftables** (`// EXTERNAL: <CLIENT> 0x<va> ??_7Class@@6B@`): a polymorphic original's constructor and destructor store
the vfptr ahead of the body's EH state, which only a real virtual class gives. VC6 defines the `??_7` table as a COMDAT
in every object that stores it, so unlike a function's marker this one applies to a defined symbol: `compare_obj`
passes the marked vftables of the object to `coff.link` (`client_vas`), which resolves references to the client's
address like an `eqd_va_` symbol (a wrong VA is an ordinary difference). The name is the decorated symbol. The VA does
not go into `eqd_match_syms.h`: it is data, and a new vftable must not change that shared header. DLL builds store the
same address (`externs.patch_vftable_refs`, `unicorn/README.md`); the object's own table stays and is never used, but
it has to link, so declare its slots pure (`virtual void slot0_guess() = 0;`, slots go to `__purecall`) or give them
markers. A pure class can be constructed only through a derived constructor, not with `new`.

The generated `eqd_match_syms.h` declarations are shared across clients. New `EQ_ARRAY` / `EQ_GLOBAL`
tokens, or address tokens newly discovered through `EXTERNAL` markers (including calls previously hidden
inside parameterized macros), can change unrelated VC6 register allocation. After changing that declaration
set, compare every client, even when only one client's source changed.

**C++ cdecl targets** (`EQ_CDECL_CPP` / `EQ_STDCALL_CPP`, `include/trilogy/eqaddr.h`): `EQ_CDECL`'s match symbol
`eqd_fn_<va>` is declared `extern "C"` in the generated header, and VC6 /GX treats an extern "C" function as
non-throwing, also through the cast: in C++ code with an EH frame it leaves out the state stores around the call
and may merge the argument cleanup of neighbouring calls (VC6_QUIRKS `vc6q-c-linkage-nothrow`). The `_CPP` forms
call the static member `f` of `eqd_cc<va>` instead, a C++ function that is only declared; its decorated name
(`?f@?$eqd_cc@$0BAAFMCNF@@@SAXXZ` for 0x1005C2D5) carries the VA and `coff.eqd_va` maps it like `eqd_fn_<va>`.
Outside a C++ match build they are `EQ_CDECL` / `EQ_STDCALL` (DLL builds call the constant address, which VC6
treats as possibly throwing anyway). Opt-in per call site: use it where the original calls a C++ function from
C++ code and keeps EH states or separate `add esp` around the call. The VA counts for the generated header as
an `EQ_CDECL` would (see above).

When checking calls with short parameters, declare the fixture callee's corresponding `args` as `"short"`.
VC6 can load only a register's low word before pushing the whole register; the upper bits of that argument
slot are unspecified. Reading it as an `int` can produce a fixture mismatch unrelated to the short value.

Status per function (`match_status`, written to the queue's `match` field):
`identical` = same bytes outside relocation fields; `normalised` = same instructions only (in practice the
dummy `xor edx, edx` of `EQ_THISCALL`, see PLAN Phase 3); `diff` = same instruction count, `ndiff`
differ (1-5 = near miss, listed by `--nearmiss` for a permuter); `structural` = different count;
`error` = the object doesn't build or the symbol isn't found. Denser scores in the JSON for every non-identical
function: `prefix` (how many leading normalised instructions agree) and `first_diff_at` (byte offset of
the first differing original instruction from the entry); `record-match` uses them as tie-breakers for the
queue's `match_best`.

`compare.MatchContext` holds what match mode loads once (original image, function starts, names, header);
`permute/permute.py` scores its candidates through it (`compile` + `compare_obj`).

Self-tests on hand-assembled code (and one VC6 object for `coff.link`): `tests\compare\`.

## Source checks (`source_checks.py`): markers, fake-match oddities, readability ratchet

```
..\.venv\Scripts\python.exe tools\compare\source_checks.py [files] [-v] [--json F] [--floor F] [--queue F]
    [--update | --raise-floor]
```
Rules: RULES.md "Fake matches", "Source status markers", "Readability ratchet". Over `src\` (no compiler, about
1 s for eq-trilogy), using the permuter's tokenizer and parser (`permute\ctoken.py`, `tu.py`):
- **markers**: a comment line starting `@name` must be `@early-stop`, `@identity-TODO`, `@dead-code` or
  `@equivalent`, followed by `: <reason>`, attached to a `// FUNCTION:` function (its leading comment block or
  body), once per function; `fake match:` needs a reason; with the queue (default `queue\queue.jsonl`),
  `@early-stop`/`@equivalent` on a function recorded as `identical` is stale.
- **oddities**: empty `do {} while (0)`, unused locals (declared, never named again), `goto`, `__asm`
  (all need a `fake match:` comment; goto/`__asm` may say `as original:`) and `register` (banned). A comment
  covers an oddity on its line, from the comment lines directly above, or from the function's leading block.
- **ratchet**: per unit (annotated function = its VA, other function = `path:Name`, file scope = `path`)
  `cast` (C-style and `static_cast`/`const_cast`/`dynamic_cast`), `reinterpret_cast`, `goto`, `raw_addr`
  (a pointer cast of a constant >= 0x10000 or of `(x + constant)`), `extern`, `union`, `asm`, `register`,
  `volatile`, `barrier`, `unused_local`, `unmarked` against the floor file, default `<repo>\tools-ratchet.json`
  (`schema_version`, `metrics`, `units: {key: {metric: n}}`, one unit per line; clean units are `{}`).

Violations: marker errors; a count above its floor; an unmarked oddity in a unit the floor doesn't have yet.
New units' other counts and lowered counts are reported only. `--update` writes lowered floors and new units
and refuses while there are violations; `--raise-floor` writes the current counts (creates the file; the
explicit, reviewed step). Exit 0 clean, 1 violations, 2 setup. Install the first floor in a decomp repo with
`--raise-floor` and commit `tools-ratchet.json` on its own.

Tests: `tests\compare\test_source_checks.py`; negative controls for match mode (planted wrong immediate, slot,
encoding, data address and call target must not come out `identical`): `tests\compare\test_compare_negative.py`.

## probe.py: what does VC6 make of this snippet?

```
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c -f f                 listing of f (SP4, match-mode flags)
..\.venv\Scripts\python.exe tools\compare\probe.py a.c b.c -f f                same / differs + unified diff
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c -f f -V A -V B       one file built with /DA and /DB
..\.venv\Scripts\python.exe tools\compare\probe.py snip.c --flags /O2          an /O level replaces /O1
..\.venv\Scripts\python.exe tools\compare\probe.py --check-quirks              rerun docs/vc6_quirks (VC6_QUIRKS.md)
```
Compiles with `DEFAULT_CFLAGS` + the snippet's `// CFLAGS:` lines + `--flags` (no `EQD_MATCH` header: give
the snippet its own `extern` declarations), parses the COFF object (`coff.py`) and lists the function's
`/Gy` section: relocations by symbol name, branches as `@<index>`, switch tables as `dd`/`db`. Exit 1 when
two builds differ. Objects go to a temporary folder unless `--out`. Quirk files state their claim as
`// EXPECT:` checks (format in the module docstring). Tests: `tests\compare\test_probe.py`.

## stackcmp (`stackcmp.py`): stack-frame hints for near misses, via reccmp

```
..\.venv\Scripts\python.exe tools\compare\stackcmp.py --nearmiss ..\build\match\nearmiss.txt ^
    --json ..\build\match\stackcmp.json
..\.venv\Scripts\python.exe tools\compare\stackcmp.py --nearmiss ..\build\match\nearmiss.txt --va 0x004196D4 -v
```
Options: `--repo` (default `.`), `--client trilogy`, `--out` (build folder, default `<build>\<client>\stackcmp\`),
`--va` (one function of the list), `-v` (hints per function, reccmp's own log). Needs reccmp in the venv
(`isledecomp/reccmp` 0.1.7, installed offline), VC6, the original exe and the Ghidra export (original sizes).

1. **Build.** The list's sources are compiled as in match mode (`cflags_for` + `/DEQD_MATCH` +
   `eqd_match_syms.h`) **plus `/Zi`**, and linked into one DLL with `/DEBUG /PDBTYPE:CON /INCREMENTAL:NO`; a
   generated `eqd_stubs.c` defines every `eqd_fn_<va>` / `eqd_va_<va>`. A rewrite calling a rewrite outside
   the list links with `/FORCE:UNRESOLVED`. Each source is also compiled without `/Zi`:
   `build.zi_neutral` says whether the code sections (bytes + relocations, compiler labels `$L123` as one name)
   are identical. They were for all 8 near-miss sources (2026-10-04): `/Zi` doesn't change VC6 SP4 code.
2. **Map.** reccmp finds each function through our own annotations (`reccmp_map.py`, below) and the PDB's
   line numbers. A generated `stackcmp-data.csv` (reccmp data-sources) adds the original's size per near miss
   (Ghidra export) and puts the DLL's `eqd_fn_`/`eqd_va_` stubs and CRT copies (`crt_defaults.json`
   `rewrite_aliases`, `library_names`) at their client addresses by linker symbol, so calls and globals
   name the same entity on both sides.
3. **Compare.** reccmp's function diff (`Compare.compare_address`). As in `reccmp-stackcmp`, the stack operands
   of aligned instructions (equal blocks and same-length replace blocks) are paired; the rewrite's `ebp`
   slots are named from the PDB (`S_BPREL32`). Our operand parser also handles indexed operands:
   reccmp-stackcmp's regex reads `[ebp + eax*4 - 0x10]` as `ebp+0xea` (reccmp 0.1.7).

**PDB 2.0.** reccmp reads VC6 PDBs: its bundled `cvdump.exe` (14.00) dumps a VC6 `LINK /DEBUG` PDB
("Microsoft C/C++ program database 2.00") with publics, `S_GPROC32`/`S_BPREL32`, types, lines and section
contributions, and `reccmp.compare.Compare` matches and diffs with it. Use `/PDBTYPE:CON`: VC6's default
`SEPT` leaves the types in the compiler's `vc60.pdb` (it still works while that file sits next to it).
gta2_re does the same with `/Z7` objects and `link /debug`, plus CSV data sources for its names.

### Hints JSON (`--json`), schema 1
```
{"schema": 1, "tool": "stackcmp", "client": "trilogy", "generated": "...", "reccmp_version": "0.1.7",
 "nearmiss": "<list>", "build": {"dll", "pdb", "zi_neutral": {source: bool}, "errors": {source: text}},
 "functions": [ {
   "va": "0x004196D4", "name", "source", "ndiff",          from the near-miss list
   "status": "ok" | "no_match" | "error", "error": "...",   (no_match: reccmp found no rewrite for the VA)
   "rewrite_va": "0x10001D1C", "reccmp_accuracy": 0.87,    reccmp's own similarity (0-1)
   "stack": "same" | "differs" | "no_stack_operands",
   "complete": bool,                                        false: structural blocks were not paired
   "frame": {"orig": 32, "rewrite": 32},                    N of `sub esp, N` in the prologue, or null
   "ebp": [{"orig": -20, "rewrite": -16, "kind": "same" | "moved" | "conflict", "symbol": "i", "type": "T_INT4"}],
   "ebp_unseen": [{"rewrite": -8, "symbol", "type"}],      rewrite locals the diff never touched
   "esp": [{"orig_addr", "orig", "rewrite_addr", "rewrite", "orig_slot": "esp+0xc", "rewrite_slot": "esp+0x10"}],
   "esp_deltas": {"+0x4": 3},                               rewrite minus original, per aligned esp operand
   "operand_mismatch": [{"orig_addr", "orig", "rewrite_addr", "rewrite"}],   stack operand on one side only
   "structural": [{"orig_addr", "rewrite_addr", "orig": [...], "rewrite": [...]}],
   "hints": ["frame differs at ebp-0x14: the original's ebp-0x14 is the rewrite's ebp-0x10 (`count`)", ...]
 } ] }
```
Offsets are signed decimals relative to the register; addresses are hex strings as reccmp prints them.
`hints` are the one-line strings for a prompt or the permuter log; the other fields are for tools. Kinds:
`moved` = one original slot maps to one other rewrite slot (reorder the locals); `conflict` = a slot maps
to several, or several to one (a variable split or merged). `esp` offsets move with every push, so they
are reported per instruction and as a histogram, never as a variable map. Consumers (permuter feed,
`context/pack_context.py`) read `functions[*]` by `va` and should only use `hints` when `status` is `ok`.

**First run** (2026-10-04, `build\match\nearmiss.txt`, 31 functions, ~5 s): all 31 mapped (rewrite symbol
checked against the map), 24 stack `same`, 6 `no_stack_operands`, 1 `differs`
(`EQ_Character::EQ_Character`: the rewrite keeps the loop counter `i` at `ebp-4`, the original compares a
pointer in a register). The 14 `vlong`/`monty` near misses are 100% in reccmp: their one "difference" is
`fs:[0]` vs `fs:[__except_list]`, an artefact of match mode (`__except_list` is LIBC's absolute symbol 0)
that a linked DLL doesn't have. The other misses are register allocation or instruction choice, not
frame layout.

## reccmp mapping (`reccmp_map.py`)

Our `// FUNCTION: TRILOGY 0x...` line is reccmp's marker syntax already (target id `TRILOGY`), but reccmp
reads a comment line right after a marker as the function's *name* (its form for templates and library
code), so our `// ORIGINAL FILE: ...` lines would become names: 676 of the 733 markers. Instead of a second
set of comments, reccmp gets a rewritten view: each marker block moves down to sit directly above the
definition, the comment lines it jumped over go blank, the line count stays, so the PDB's line numbers
still point at the definitions. With that all 733 parse as line-based markers.

- `reccmp_map.code_files(paths)`: reccmp `TextFile`s (real path, rewritten text) for `reccmp.compare.Compare`
  (stackcmp uses this).
- `reccmp_map.py --mirror <dir>`: rewritten copies of `src\**` plus `reccmp-project.yml` (target `TRILOGY`,
  `eqgame.exe`, its SHA-256, `source-root: src`) and `reccmp-user.yml` (the original from `paths.local.ini`)
  for the reccmp CLI tools. Add a `reccmp-build.yml` (`project: .`, `targets: TRILOGY: path: <dll/exe>,
  pdb: <pdb>`) next to them; reccmp matches PDB source paths to the mirror by file name and path suffix.
  Smoke test 2026-10-04 on stackcmp's DLL: `reccmp-stackcmp --target TRILOGY 0x4E6447` and `reccmp-reccmp
  --target TRILOGY` both run (set `PYTHONIOENCODING=utf-8`; its check marks don't fit cp1252).

Self-tests (hand-made diffs, the marker rewrite, and reccmp on two VC6 `/Zi` DLLs when VC6 and reccmp are
present): `tests\compare\test_stackcmp.py`.

### Against the delinked original (`--target-obj`)

```
..\.venv\Scripts\python.exe tools\compare\compare.py --match --target-obj ..\build\trilogy\delink ^
    --json ..\build\trilogy\target_check.json
```
`ghidra\scripts\delink.py` writes one relocatable object per original function (`<VA>.obj`, symbols
`_eqd_fn_0x<VA>` / `_eqd_va_0x<VA>`). With `--target-obj` (that folder, or one `.obj`) every function is also
checked against its own: the object is linked at its VA (`coff.delinked_placement`) and laid over the exe,
then `target` in the JSON says
- `bytes_ok`: the delinked object reproduces the original's bytes (its relocations point at the right places);
- `status`: the match status against it;
- `missing` / `extra`: relocation fields (offsets from the entry, inside the instructions) that only the
  rewrite / only the delinked object has. `missing` = Ghidra has no reference there; `extra` = a client
  address the rewrite carries as a plain number (not relinkable), or a number Ghidra took for an address;
- `value_differs`: both relocate the field, to different client addresses;
- `ok`: all of the above clean and `status` identical.
The summary line counts pass/fail over the byte-identical functions, the set where the delinked object and the
match build must agree.

## objdiff (`objdiff_config.py`)

Needs a queue file and the `objdiff.json` it generates; neither is published.

[objdiff](https://github.com/encounter/objdiff) v3.8.2 (objdiff-cli, `[toolchain] objdiff_cli`) diffs a target object
(the delinked original) against a base object (our match build) function by function, with relocations shown
by symbol. `objdiff_config.py write` (run in the decomp repo) generates `objdiff.json`: one unit per source
file (its annotated functions plus the queued functions of the queue subsystem of the same name), and one per
queue subsystem without a source (target only). Target = `<build>\<client>\delink\units\<unit>.obj`
(`delink.py <client> --units`), base = `<build>\<client>\objdiff\<obj>.obj` (match-mode compile), and
`symbol_mappings` pair `_eqd_fn_0x<VA>` with the rewrite's decorated name. objdiff rebuilds a base through
`objdiff_config.py build <obj>`. Other commands: `build --all`, `check` (objdiff's 100% against
`compare --match`'s `identical`), `report` (progress snapshot), `units` (list the plan).

## How to look at a near miss

1. Find it: `..\build\match\nearmiss.txt` (VA, differing instructions, first difference), or the queue's
   `match` field (`status` diff, `ndiff`).
2. Text view, both normalised listings side by side:
   `..\.venv\Scripts\python.exe tools\compare\compare.py --match src\<file>.cpp --va 0x<VA> --show -v`.
   `-v` prints the unified diff of the normalised tokens; the first `-`/`+` pair is usually the cause.
3. objdiff view: the objdiff 3.8.2 GUI (`objdiff-windows-x86_64.exe`), open the repo folder as the
   project (it reads `objdiff.json`), pick the unit (= the source file) and the function. Left is the original
   (delinked), right is ours; differing instructions, arguments and relocations are coloured, and it rebuilds
   the right side when the source changes. One-shot text output without the GUI:
   `objdiff-cli diff -p . -u <unit> _eqd_fn_0x<VA> -o - --format json-pretty`.
4. Read the difference against the known causes before touching logic: register choice or slot order
   (try renaming locals / reordering declarations, docs/PLAN.md 9.6), block order (branch nesting),
   an `xor edx, edx` (`EQ_THISCALL`, a known difference until real `__thiscall` classes, objdiff shows it
   too), a string or float literal (compare.py matches them by contents, objdiff by symbol: objdiff may flag
   one that compare.py accepts).
5. If 1-5 instructions differ, hand it to the permuter (`permute\README.md`) rather than hand-tuning.

Self-tests on hand-assembled code (and one VC6 object for `coff.link`): `tests\compare\` (`test_target_obj.py`: hand-made
COFF objects for `--target-obj` and absolute symbols; `test_objdiff_config.py`: the unit plan and config).
