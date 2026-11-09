# unicorn/ – differential test harness

Runs one function of the original binary and its rewrite side by side under Unicorn, from identical
snapshots, and compares everything a caller can observe (PLAN §4.4). Decomp repos see this folder as
`tools\unicorn\`; their fixtures and tests live in `tests\unicorn\<subsystem path>\`, where the subsystem
path is the queue's subsystem = the rewrite's source file without `src\` and the extension (fixtures for
`src\EverQuest\EQMisc.cpp` live in `tests\unicorn\EverQuest\EQMisc\`).

A failing harness test means the rewrite is wrong or the fixture is incomplete. **Never change the harness,
its comparisons, or a fixture's expectations to make a rewrite pass** (RULES.md). If a function can't be
tested faithfully, flag it.

## Running

```
cd eq-trilogy
..\.venv\Scripts\python.exe -m pytest tests/unicorn            # every fixture + negative tests
..\.venv\Scripts\python.exe -m pytest tests/unicorn -k GetLanguageName -v
..\.venv\Scripts\python.exe -m pytest eq-trilogy\tools\tests\unicorn  (from the workspace: harness self-tests,
                                                                       hand-assembled code, no game files)
```
Set `EQD_TOOLS=<path to a tools folder>` to run a decomp repo's tests against a working copy of this repo
instead of the pinned `tools\` submodule. The tests skip when `paths.local.ini`, the original binary or VC6
is missing. Test DLLs come from the shared [test-DLL cache](#test-dll-cache) (`EQD_UNICORN_CACHE`);
`EQD_UNICORN_BUILD=<dir>` moves the per-process build folders (and, with the cache off, the DLLs) out of
`<build>\<client>\unicorn\`.

**Several images** (`docs/EQMAIN.md`): a fixture under `tests\unicorn\eqmain\` runs against eqmain.dll (client
`eqmain`, `original\trilogy\eqmain.dll` at 0x10000000, VC6 SP5 = `[toolchain] vc6_sp5`, its own fixture defaults
`crt_defaults_eqmain.json`, rewrites annotated `// FUNCTION: EQMAIN 0x...` under `src\eqmain\`); everything else
against eqgame.exe as before. The decomp repo's conftest keeps one Harness per image (`make_harness(client)`);
`eqd_unicorn\clients.py` is the registry (`Workspace.binary/vc6/repo(client)`).

Fixture and wave tools (details in [Fixture tools](#fixture-tools)):
```
..\.venv\Scripts\python.exe tools\unicorn\tools\make_fixture.py trilogy 0x00410C80 [--run]   fixture skeleton
..\.venv\Scripts\python.exe tools\unicorn\tools\mutate_check.py tests\unicorn\...\X.json      mutation check
..\.venv\Scripts\python.exe tools\unicorn\tools\unicorn_cache.py --stats | --prune | --clear | --warm
```

## How a test runs

1. **Images.** The original (`original\<client>\eqgame.exe`) is mapped at its VAs. The rewrite is built with
   VC6 (`vc6.py`: `cl /O1 /MT /Gy /GX` plus the source's own [`CFLAGS`](#per-source-compiler-flags), `link /DLL /NOENTRY /FIXED:NO /MAP /BASE:0x10000000`) from the fixture's
   `sources` into a DLL taken from the [test-DLL cache](#test-dll-cache) (built only when no entry has the
   same sources, headers and options), then loaded at `0x30000000` with its base relocations applied. Its `.map` gives the
   symbols. Neither DllMain nor the CRT start-up runs (the link names `_fltused`, the DLL start-up object
   and kernel32/user32 so float code, stdio and Win32 calls still link; see `vc6.py`).
2. **Entry points.** Original: the fixture's `original` VA. Rewrite: the function annotated
   `// FUNCTION: <CLIENT> 0x...` with that VA in the sources (undecorated names are matched against the
   map), or `rewrite` / `rewrite_symbol` explicitly.
3. **Extent.** Recursive descent with capstone decides which instructions belong to each function
   (`extent.py`; follows branches and VC6 switch tables, stops at `ret`, indirect jumps, and known function
   starts). Known starts come from the client's export (`build\export\<client>\functions.json`) and
   `names\*.csv`. An explicit `size` overrides it for the original.
4. **Interception.** Any basic block that starts outside the function is a call (or tail jump) to
   something else. It is never executed: the harness records `(target, this, args)`, applies scripted side
   effects, sets EAX/EDX (ECX/EDX otherwise get fixed junk), pops the stack per the callee's convention and
   returns. Targets are keyed the same on both sides:
   - original callees by VA; the rewrite reaches unported callees through their fixed VA, so same key;
   - imports (`call [IAT]`) by `"dll!name"`: every image's IAT points at one shared stub per import;
   - rewrite-internal calls to another annotated rewrite by that rewrite's original VA (automatic);
     other DLL symbols by their decorated name, or by `rewrite_aliases` (e.g. the DLL's CRT `_strlen` →
     the client's CRT VA). The client's CRT callees and aliases come from a default table (see
     [Fixture defaults](#fixture-defaults)), so fixtures rarely list them.
   A target with no script is an error (both sides fail with "call to unscripted target ...").
   `passthrough: true` runs a callee natively on each side instead (each side its own copy) and still
   records the call; imports can't be passed through.
   `probe: true` marks VC6's `_chkstk`/`__alloca_probe` (frames over 4 KB): the harness lowers ESP by EAX
   as the real probe does, without recording a call (the two sides' frame sizes may differ).
   `noreturn: true` marks a callee that never returns (`exit`, `abort`, `_amsg_exit`, `__purecall`, a
   fatal-error handler): the call is recorded and ends that side's run (see `error` below). VC6 emits no
   stack cleanup after such a call, so a stub that returns runs on into code that was never meant to run, and
   an ESP-framed original then "returns" to a garbage address.
   **Stub purge check.** For every stubbed callee keyed by an original VA, the harness reads the callee's own
   `ret N` from the original's code (`extent.stack_purge`) and compares it with what the stub pops (its `conv`
   and stack `args`). A mismatch leaves the caller's ESP off by the difference after every call; it is listed
   as `warning:` lines above the failing cases (`Report.warnings`). A deliberate mismatch says why with
   `"purge_ok": "<why>"` on that callee; passthrough, probe and noreturn callees aren't checked.
   **Varargs callees** (cdecl only) record a different number of args per call: `"varargs": "printf"` takes
   the last fixed arg (typed `cstr`) as a VC6 printf format and records the args it consumes, typed by their
   conversions (`%d` int, `%u`/`%x` u32, `%hd` short, `%I64d` i64, `%f`/`%g` double, `%s` cstr, `%c` char,
   `%p`/`%n`/`%S` ptr, `*` int); `"varargs": "stack"` counts the slots the caller pops right after the call
   (`add esp, N`, or one or two `pop ecx`) and records the extra ones as u32. "stack" fails when the cleanup
   isn't right after the call (deferred, or merged with a later call's) and records pointers as values, so
   prefer "printf" for format functions. With "printf" an extra arg the format doesn't use matches (neither
   the callee nor the caller can tell); a missing one reads what lies above it on the stack, a trace
   difference.
5. **Per case.** Every touched page is restored, objects are allocated in a fixture heap (`0x60000000`),
   memory and objects are filled from a per-case RNG (`random.Random("case:<seed>:<i>")`, the same stream for
   both sides), and registers start with the same random junk (a case's `regs` override them). Fuzzed cases can also set declared memory
   fields (`fuzz.memory`, see below) after the fixture's `objects` and `memory`. Sub-dword args get random garbage above
   their meaningful bits, like a real `push eax`. ESP points at a sentinel return address; FPU is `fninit`
   with control word `0x27F`, then the case's `fpu` values (if any) are loaded onto the x87 stack; FS points
   at a TEB (`fs:[0]` = -1, `fs:[0x18]` = TEB).
6. **Compare.** Any difference fails the case, labelled by kind:
   - `return`: by the fixture's `ret` type. Integers compare only their meaningful bits (`char`/`bool` =
     AL, `short` = AX, `i64` = EDX:EAX); `float`/`double` compare ST0 and require x87 depth 1; `cstr`
     compares the string EAX points at (the rewrite's literals live at different addresses); `void`
     compares nothing.
   - `fpu`: x87 stack not empty after a non-float return, or not depth 1 after a float one. A helper that
     pops an input it never got (no `fpu` in the case) ends at depth 7 on both sides.
   - `esp`: ESP after return (4 + bytes the callee popped). Both sides must agree **and** match the
     fixture's `conv` + `args`.
   - `preserved`: EBX/ESI/EDI/EBP changed.
   - `writes`: the set of bytes written and their final values, excluding the callee's own stack frame,
     its argument slots (callees may reuse them as scratch), the TEB, and `ignore_writes` ranges.
     Rewrite writes into its own DLL (statics) therefore fail unless ignored.
   - `trace`: the ordered list of intercepted calls with their recorded args.
   - `error`: the runs end differently. A **CPU fault** (`read_unmapped`, `write_unmapped`,
     `fetch_unmapped` with the faulting data/target address; `div0`, `invalid_insn`, `int3`, other
     `interrupt` N) is behavior: when both sides fault with the same kind and address, the case matches
     (the pc necessarily differs), provided the writes and calls before the fault agree (reported as
     `writes`/`trace`) and the case has no `expect.ret`. A different kind or address, or only one side
     faulting, fails. Both sides ending in the same `noreturn` callee is a match on the same terms. Harness
     problems (unscripted callee, no return within `max_insns`, emulation errors) always fail, even when both
     sides hit the same one. A `fetch_unmapped` error carries a `; hint:` from the last instruction that ran:
     a `ret` (the stack was unbalanced: a stub's pop count or a noreturn callee; it names the last stubbed
     call and what it popped) or an indirect `call`/`jmp` (a function pointer or vtable slot the fixture
     didn't set).
   - `expect`: a case's `expect.ret` disagrees with what the **original** returned (fixture check).
   - `coverage`: **every** case ends in the same CPU fault at the same pc on both sides. The rewrite runs
     from the test DLL, so a shared pc is in the original image: a passed-through callee faulted and
     nothing after that call ran on either side. Give the callee what it reads, or script it; a fixture
     where the fault is the point says so with `"fault_ok": "<why>"`. Not applied to mutation runs.

## Fixture format (JSON)

```json
{
  "function": "CEverQuest::GetLanguageName",          // label only
  "original": "0x004c8fce",
  "sources": ["src/EverQuest/EverQuest.cpp"],         // relative to the decomp repo
  "rewrite": "CEverQuest::GetLanguageName",           // optional, default: from the FUNCTION annotation
  "conv": "thiscall",                                 // cdecl | stdcall | thiscall | fastcall
  "this": "@everquest",                               // thiscall only; value, "0x..", or "@object[+off]"
  "args": [{"name": "languageId", "type": "short"}],
  "ret": "cstr",
  "objects": {"everquest": {"size": "0x100", "fill": "random",
                            "init": [{"offset": "0x10", "u32": "@other"}]}},
  "memory": [{"addr": "0x006efe2c", "u32": "@everquest"}, {"addr": "0x7a0000", "size": 64, "fill": "random"},
             {"addr": "@everquest", "offset": "0x1c", "u8": 3}],
  "callees": {
    "0x004d02fd": {"name": "itoa_guess", "conv": "cdecl", "args": ["int", "ptr"], "ret": "@buf"},
    "kernel32.dll!GetTickCount": {"conv": "stdcall", "ret": [100, 200]},
    "0x0052a000": {"name": "strlen", "args": ["cstr"], "passthrough": true},
    "0x005224fb": {"name": "sprintf", "args": ["ptr", "cstr"], "varargs": "printf", "passthrough": true},
    "0x00522dad": {"name": "_exit", "args": ["int"], "noreturn": true},   // the call ends the run
    "0x004fb6e0": {"name": "updateRef", "conv": "thiscall", "args": ["ignore"]},  // pops a by-value arg (ret 4)
    "0x00522920": null                                // drops a default entry (see Fixture defaults)
  },
  "rewrite_aliases": {"_strlen": "0x0052a000"},
  "ignore_writes": [["0x30003000", "0x30003010"]],
  "size": "0xd6",                                     // optional: original extent instead of descent
  "max_insns": 200000,
  "fault_ok": "null this is the point",               // optional: allow every case to fault in a callee
  "coverage_ok": {"004DE640-004DE6A0": "needs a D3D device lost mid-call"},   // optional, see Coverage
  "ratio_ok": "the original inlines 3 helpers the rewrite calls",            // optional, see Coverage
  "fpu": [1.5],                                       // optional: x87 stack at entry, ST0 first (see below)
  "cases": [{"args": [0], "expect": {"ret": "..."}},
            {"args": ["@everquest"], "this": "0x0", "memory": [...], "objects": {...}, "regs": {"esi": 5}}],
  "fuzz": {"seed": 5017550, "count": 300, "args": [{"range": [-4, 30]}],
           "memory": [{"addr": "@everquest", "offset": "0x1c", "type": "u8", "range": [0, 40]},
                      {"addr": "@everquest", "offset": "0x40", "type": "float", "count": 3, "p": 0.5},
                      {"addr": "0x7a0000", "size": 64}]},
  "defaults": true                                    // optional: false = no fixture defaults
}
```
(JSON has no comments; they are here for explanation only.)

- **RDTSC** (`0F 31`) is deterministic, never the host clock: every run (each case, each side) starts a 64-bit
  counter at `0x0000001000000000` that advances by `0x1000` per executed RDTSC, so the original and the rewrite
  read the same sequence. A fixture or a case sets `"rdtsc": "0x0001c449333acc24"` (first value) and/or
  `"rdtsc_step": 16` (increment; the case's key wins over the fixture's). Implemented in `machine.py`
  (`_hook_rdtsc`): Unicorn has no RDTSC instruction hook, so a code hook bounded to each `0F 31` found in the
  images' code sets EDX:EAX and skips the instruction; all other instructions run unhooked (no measurable
  cost). RDTSCP and CPUID are not handled.
- **Types** (args, callee args, `ret`): `int i32 long u32 uint dword ptr short i16 u16 ushort char i8 u8 uchar
  bool float double i64 u64 cstr ignore mem:N void` (`void` for `ret` only). Callee args record
  `cstr` as the string, `mem:N` as N bytes at the pointer, `ignore` as nothing; a NaN `float`/`double`
  as `"nan:<bits>"` (so equal NaNs match). A `cstr` case arg given as plain text is allocated in the
  fixture heap (text starting with `@` or `0x` is a reference instead; fuzzed text never starts so).
- **Values:** integers, `"0x..."` strings, or `"@object"` / `"@object+0x10"`.
- **x87 inputs** (`fpu`, in a case or as the fixture's default): for helpers that take their input in ST0
  (`__ftol`-style conversions, `FUN_1004ec36`), which no `args` entry can express (a `float`/`double` arg is
  a memory-stack argument). A list, **ST0 first**, at most 8 entries, each loaded with `fld tbyte`, so the
  value is exact: a number or `"nan"`/`"inf"`/`"-inf"` (as a double), `{"f32": x}`, `{"f64": x}` (`x` a number
  or `"0x..."` raw double bits), or `{"f80": "0x..."}` (raw 80 bits: sign and exponent in the top 16, then the
  64-bit mantissa with its explicit integer bit; for values no double holds). A bare `"0x..."` is refused
  (ambiguous). x87 names in `regs` (`st0`, `fp0`, `fpsw`, ...) are an error, not silently ignored: `regs` only
  sets EAX..EBP. The function is expected to pop what it consumes (an `int`/`i64` return checks depth 0).
- **`i64`/`u64` returns** compare EDX:EAX; `expect.ret` is the 64-bit value (negative = signed), e.g.
  `{"fpu": [-2.5], "expect": {"ret": -2}}`.
- **Memory entries:** `hex`, `u32`, `u16`, `u8`, `f32`, `f64`, `cstr`, or `fill: random|zero` with `size`;
  an optional `offset` is added to `addr`.
- **Values from the original image:** `{"from_image": "0xVA", "image_size": N}` in place of `hex` (in a
  memory/`init` entry or a callee write) is replaced by the N bytes of the user's original image at VA when the
  fixture is loaded (`resolve_image_refs`). Published fixtures use it for original `.data`/`.rdata` content
  (game text, constants), so they store the address, not the bytes. With `"as": "cstr"` it stands for a string
  value instead (a case argument, a fuzz choice, `expect.ret`): the N bytes as a latin-1 string.
- **Callee returns:** `ret` (int, `@object`, or a list: one per call, the last repeats; a Python
  `callable(index, call)` also works when the fixture is built in Python), `ret_edx`, `ret_float` (pushed on
  the x87 stack), `writes: [{"arg": 1, "offset": 0, "hex": ".."} | {"addr": "0x..", "hex": ".."}]`. A write
  with `"call": N` (or a list of N) happens only on that call of the callee within the run (0 = the first),
  e.g. a `_read` stub that fills a header field with a different value on each read.
- **Callee keys:** a VA, `"dll!name"`, a rewrite symbol, or `"@object[+off]"`: a code address the fixture
  makes itself, for calls through function pointers (store `"@fn"` in the pointer global or a vtable slot
  via `memory`/`init`, key the callee `"@fn"`). The eqgfx_dx7.dll entry points (`call [0x006E5xxx]`, filled
  by GetProcAddress at start-up) are tested this way.
- **Stack balance (callee stubs).** A stub pops exactly what its `conv` + `args` say (stdcall/thiscall: 4
  bytes per stack slot; cdecl: nothing), and it must pop what the real callee's `ret N` pops, or the caller's
  stack is off after the call. The harness checks this for callees keyed by VA and prints a `warning:` line
  when they disagree; fix the stub, don't work around it. The usual causes:
  - **By-value objects** (smart pointers, CXStr-like handles, small structs): VC6 builds the argument in
    place on the stack right before the call, with no plain `push` of a value: `push ecx` (reserves a slot) /
    `mov eax, esp` / `mov [eax], ecx`, or `mov ecx, esp` + `call <copy ctor>`, or a slot left over by an
    earlier call's partial `add esp`. That slot is a stack arg of the callee (type `ignore` or `u32`), and a
    thiscall callee that ends in `ret 4` pops it. The rewrite passes the object by value too.
  - **Callees that never return** (`exit`, `abort`, `__purecall`, a fatal-error/verify handler that calls
    exit): mark them `"noreturn": true`. Don't fake it with a different `conv`.
  - Symptom of either: the ORIGINAL ends in `fetch_unmapped` at a tiny address (0x0, 0x2, 0x5: whatever
    value sat where the return address should be) with a `hint:` naming a `ret`. Adding a callee for that
    address never helps.
- **Local classes:** methods of a class local to a function in the rewrite (`??1Cleanup@?1??Func@@...`,
  e.g. EH cleanup helpers the original inlines) run as part of the rewrite's body, untraced; so do the
  decorated names listed in `"rewrite_inline": [...]` (other helpers the original inlines).
- **Declared-only client functions** (`// EXTERNAL: <CLIENT> 0x<va> <Class::Name>` on a declaration the source
  calls but does not define, `eqd_unicorn/externs.py`): the test DLL links a generated `mov eax, va; jmp eax`
  for the symbol (the same stub object as EQ_METHOD's members, `thiscall_stubs.py`), and a call to it is a
  call to the client function at `va`: script the callee under that VA, as for the original. A marker counts in
  the source itself and in every header under the include directories (`include/`); `name` is the undecorated
  `Class::Class` / `Class::Method` / `function` (the file's one-identifier `#define`s applied), or the decorated
  symbol when the object references several overloads of the name. A symbol some object of the build defines
  is never stubbed.
- **Client vftables** (`// EXTERNAL: <CLIENT> 0x<va> ??_7Class@@6B@`, decorated name): VC6 defines the table in the
  object, so it is not stubbed: `externs.patch_vftable_refs` writes `va` into every reference and voids the
  relocation, so a constructor or destructor stores the client's address, as the original does. The hook DLL's
  build (`hooks/eqdecomp.py`) makes the same call. Details in `compare/README.md`, "Vftables".
- **Per-case callees:** a case's own `callees` replace the fixture's entries with the same key (e.g. a
  different return value for one case); `null` removes one for that case.
- **Fuzz:** `count` extra cases from `random.Random("fuzz:<seed>")`. Per-arg `range` or `choices`, else
  by type (half small values -8..64, edge values, uniform). Pointers need `choices`. A `float`/`double`
  `range` with a JSON float bound (`[-0.5, 1e6]`; `1e6` and `2.0` are floats in JSON) draws uniformly;
  integer bounds keep drawing integers. `fuzz.fpu`: one generator per x87 input (ST0 first, same keys as an
  arg's, typed `double`), drawn from its own stream (`random.Random("fuzz-fpu:<seed>")`), so adding it leaves
  the fuzzed args unchanged.
- **Memory fuzz** (`fuzz.memory`): fields of objects or globals that each fuzzed case sets, for functions
  that read their input through pointers (random object bytes alone rarely hit `type == 3`). Each field has
  `addr` (VA or `@object[+off]`), optional `offset`, and either `type` (a scalar: `int u32 short u16 char u8
  bool float double i64 u64 ptr ...`) with `range`/`choices` or else the type's generator (as for args;
  `ptr` needs `choices`; an object reference needs a 4-byte type), `count` consecutive values `stride` bytes
  apart (default: the type's size); or `size` N for N random bytes. `p` (default 1) is the chance that a
  case sets the field at all; otherwise it keeps its seeded value. Memory fields draw from their own stream
  (`random.Random("fuzz-memory:<seed>")`), so adding them leaves the fuzzed args unchanged. Fixed `cases`
  aren't affected.

## Coverage

Every complete run (`DiffTest.run()` without `stop_on_failure`) measures which of the **original's**
instructions the fixture executed: `Machine.run(cover=set)` records each translation block that starts in the
original's extent (also the code reached on return from a passthrough or a stubbed callee) as `(va, size)` in
a set, and `coverage.py` maps them onto the extent's instructions and basic blocks (leaders: entry, direct
jump targets, the instruction after a jump/ret, gaps; a call does not end a block). The result is
`Report.coverage` and, for a fixture loaded from a file, `<unicorn build>/coverage/<VA>/<fixture stem>.json`:
`insns` / `blocks` `[hit, total]`, `rewrite_insns` (the rewrite's extent in the test DLL), `uncovered` (runs of
unhit blocks, `["start", "end", insns]`, end-exclusive) and `map` (every block `[start, end, insns, insns
hit]`, for unions). Cost: +0.3% on the Display folder (190 fixtures), nothing measurable on DoTrackSort
(a 40M-instruction sort, ~100 s); `EQD_COVERAGE=0` turns it off.

The maintainers' coverage gate (not published) unions the VA's fixtures and fails when
- fewer than **90% of the original's basic blocks** ran. Blocks starting inside a fixture's
  `"coverage_ok": {"<start>-<end>": "<why>"}` span (hex, end-exclusive, as printed) count as covered: for code
  no input can reach (dead code, a path the harness can't set up), with the reason;
- the rewrite has **< 0.7x the original's instructions** (original >= 100 instructions), unless a fixture
  says `"ratio_ok": "<why>"` (the rewrite is really that much smaller).

## Fixture defaults

A decomp repo can pass one fixture fragment that is merged under every fixture:
`Harness(..., fixture_defaults=d)`. eq-trilogy's is `tests/unicorn/crt_defaults.json`: the client's VC6
LIBCMT string/ctype/`sprintf` functions as passthrough `callees` keyed by their VA, and `rewrite_aliases`
from the test DLL's own CRT symbols to them (`test_crt_defaults.py` checks each alias against the client's
code). Merge rules (`apply_defaults`):
- dict-valued keys (`callees`, `rewrite_aliases`) merge per entry; a fixture entry with the same key replaces
  the default **as a whole** (VA keys compare as numbers: `"0x5223D0"` = `"0x005223d0"`), and `null` removes
  it. Other keys: the fixture's value wins. Keys starting with `_` are comments.
- `"defaults": false` in a fixture drops the whole table.
- A default alias whose symbol the test DLL doesn't link is skipped; a fixture's own alias must exist.

Default callees record pointer args as values (`ptr`): override the entry with `ignore`, `cstr` or `mem:N`
when the pointer is into a stack frame or into either image's data (e.g. `strncpy` into a local buffer).

## Python API

```python
from eqd_unicorn import Harness, load_fixture
h = Harness(original_exe, build_dir, include_dirs=[repo / "include"], client="TRILOGY", function_starts=starts,
            fixture_defaults=json.loads((repo / "tests/unicorn/crt_defaults.json").read_text()))
report = h.difftest(load_fixture(path), sources, base_dir=repo, defines=["X"]).run()
report.ok, report.kinds(), report.summary(), report.cases[i].original / .rewrite   # RunResult
report.cases[i].original.error, report.cases[i].original.fault     # message (with pc); (kind, address)
apply_defaults(fixture, defaults); printf_arg_types("%d %s") == ["int", "cstr"]
Harness(..., cache_dir=None | path | False)       # test-DLL cache: default location, a folder, or off
DiffTest.run(stop_on_failure=True)                # stop after the first failing case (mutation checks)
report.coverage                                   # original-side coverage of a complete run (see Coverage)
```
`Machine` and `DiffTest` can also be driven directly with raw code blobs (`Image.from_bytes`), as the
self-tests do.

## Test-DLL cache

`vc6.build_dll` keeps every test DLL in a content-addressed cache (`eqd_unicorn/buildcache.py`), shared by all
pytest processes and worktrees on the machine. Most fixtures of a subsystem build the same sources
(EQMisc.cpp: 168 fixtures), so a full run builds one DLL per distinct source set (35 for 542 fixtures) instead
of one per fixture, and an unchanged source set is never rebuilt by the next run.

- **Key.** Recipe: the cache format and the build code (`buildcache.py` + `vc6.py`, line endings ignored),
  the toolchain (path, size and mtime of every file under its `Bin`, `Lib`, `Include` dirs), each source's
  cl options (defaults + its `// CFLAGS:` lines), every source's absolute path and content (the link
  sources from `crt_defaults.json` are sources too), include dirs, defines, libs. Plus **headers**: every
  file the preprocessor opened (`cl /E` with the same options; VC6 has no `/showIncludes`), outside the
  toolchain, with its content hash, stored with the entry and re-hashed on every lookup. Changing a header
  that a source includes, directly or not, is a miss; changing one it doesn't include is still a hit. The
  test DLL's `name` is not part of the key.
- **Atomic.** A miss builds in its own folder under the build dir (`<name>.<pid>.<random>`, removed after),
  copies the DLL + map into `<cache>\tmp\<pid>-<random>\`, then renames it to
  `<cache>\<rr>\<recipe>\<headers key>\`. A finished entry is never modified; when two processes build the same
  thing at once, the second rename fails and that process uses the first one's entry. An entry is stored only
  if the sources and headers still hash the same after the build.
- **Where.** `EQD_UNICORN_CACHE=<dir>`, default `<workspace>\build\unicorn_cache` (paths.local.ini);
  `EQD_UNICORN_CACHE=off` disables it (then `<build dir>\<name>.dll` + stamp as before). Parallel runs share one cache
  by default; they keep their own `EQD_UNICORN_BUILD` for the build folders.
- **Size.** ~0.1–0.5 MB per entry. A header edit that every source includes adds one entry per source set
  (a few MB). `tools\unicorn_cache.py --prune [--max-mb 500] [--days 14]` drops entries unused for N days, then
  least recently used ones (a hit touches its entry); `--clear` drops all; `--stats`. Run them while no tests
  run. `--warm` builds every fixture's DLL without running anything (e.g. after a wave lands).
- **Not detected:** a *new* header that shadows an existing one earlier in the include search path (nothing
  tracked changes): `--clear` after such a move. A build whose inputs changed while it ran is used once and
  left as `<build dir>\<name>.<tag>.dll`, not cached.

Measured 2026-10-02 (eq-trilogy, 591 tests incl. `hooks/tests`): before, every fixture linked its own DLL
(~7.5 min a run); building all 35 distinct DLLs from scratch (`--warm`) takes 17 s; a full run on a warm
cache 130 s on an idle machine. Two
harness speedups landed with it (an undecorated-symbol index for `_find_symbol`, callee scripts without
`@object` values shared instead of copied per run): -26% on a sample of three large fixtures. What remains
is emulation (Unicorn + the per-block/per-write hooks).

## Fixture tools

Scripts in `tools\` (here `unicorn\tools\`), run from the decomp repo. They load the repo's
`tests\unicorn\conftest.py` as a module (`eqd_repo.py`), so fixtures build and run exactly as under pytest.

**`make_fixture.py <client> <VA> [--source F] [--out F] [--force] [--no-build] [--run] [--stdout]`** writes a
fixture skeleton for a rewritten function to `tests\unicorn\<subsystem path>\<Name>.json`:
`function`/`original`/`sources` from the annotation (plus the files the test DLL can't link without, found by
linking and looking up each unresolved external among the other sources), `conv`/`args`/`ret` from the
rewrite's prototype (`Class::` methods are thiscall unless declared `static`; checked against the export's
stack purge), `this` and pointer args as random-filled `objects` sized by the headers' `C_ASSERT(sizeof(T) == N)`,
`callees` for every call of the original except the CRT defaults (prototypes from the `EQ_*` macros, else the
callee's rewrite, else the export: `ret N` + ECX at the call site → thiscall/stdcall, cdecl arg count from the
caller's `add esp`; library callees get their `rewrite_aliases`; imports through a thunk are keyed by the
thunk's VA), three starter cases, a fuzz block, and a `_todo` list (unknown sizes, export-guessed callees,
imports' args, indirect calls, referenced globals). `--run` runs it once. Checked on the 542 existing
fixtures: conv agrees for 541, arg types for 508 (the rest: `char *` buffers vs strings), ret for 491
(`bool`/`u8` vs `char`).

**`mutate_check.py <fixture.json> | --va VA [--source F] [--max 30] [--kinds rel,const,...] [--all-cases]
[--list] [--json F] [-v]`** runs every fixture of that VA against one-edit mutants of the rewrite's body
(`mutations.py`): `rel` (`<`↔`<=`, `==`↔`!=`, negated comparisons), `const` (±1, not declaration array
sizes), `arith` (`+ -`, `* /`, `& |`, `<< >>`, `&& ||`, compound assignments), `swap` (operands of `- / % < <= >
>= << >>`), `delete` (expression statements, for-loop increments), `negate` (if/while conditions, `!x`),
`return` (early return after the declarations). The sources are copied into a private temp folder (layout
kept, original folders appended to the include path) and the fixtures are pointed at the copy, so the repo's
files are never edited and other runs never see a mutant; the folder is removed at the end, also on errors
and Ctrl-C. Mutants build outside the shared cache. The unmutated copy must pass first; each mutant stops at its first failing case.
Result per mutant: `caught`, `SURVIVED` (add a case that tells them apart, or say why it is equivalent) or
`no-build` (not counted). Exit 0 = nothing survived, 1 = survivors, 2 = setup problem, 130 = interrupted.
Only the annotated function's own body is mutated (not inline helpers or macros it uses). ~1 s per mutant for
a typical function.

## Limitations

- **One function at a time, callees stubbed.** Behavior that depends on what a callee does to memory must
  be scripted (`writes`) or passed through. Recursion runs natively and isn't traced.
- **Memory is compared by value, not by type.** A rewrite that stores a pointer to its *own* string literal
  where the original stores a pointer into `.data` differs; `cstr` handles this for return values only.
  Same for callee args: use `cstr`/`mem:N`/`ignore` for pointers into either image's data or the stack.
- Uninitialised `.data`/`.bss` is the file's content, not the runtime state; seed what the function reads.
- Functions that throw C++ exceptions, use SEH unwinding, `alloca`/`_chkstk` beyond the stack, or call
  through vtables of objects the fixture didn't build need more fixture work (vtables: point an object's
  vptr at a fixture table whose slots hold `"@fn"` objects, and key the callees `"@fn"`).
- The stub purge check knows only callees keyed by an original VA whose code settles `ret N`; imports
  (`"dll!name"`) and `"@fn"` callees are not checked (stdcall imports pop 4 bytes per arg).
- Extent heuristics: switch tables need the VC6 `cmp/ja` (or byte-index) pattern, else give `size`.
- The test DLL links each fixture's sources on their own: references to functions that aren't rewritten
  yet must go through fixed addresses (as the hook DLL will), not undefined externals.
- Test-DLL flags: `/O1` with VC6 SP4, the client's compiler (`eq-trilogy/reference/VERSIONS.md`). Measured on the
  64 Phase 1c rewrites: `/O1` reproduces 37 instruction for instruction and is the closest for 61; `/O2` only 4.
  The game code calls the CRT string functions (no `/Oi`), which `/O1` gives without pragmas.


## Per-source compiler flags

Most game code was built with `/O1 /GX` (VC6 SP4). Code that was built otherwise says so in its source, once,
with a comment line anywhere in the file (`vc6.py` `cflags_for`; the hook build `eq-trilogy/hooks/eqdecomp.py`
reads the same lines, so both DLLs get the same code):
```c
// CFLAGS: /O2                  C/C++ line comment
 * CFLAGS: /O2                  inside a /* ... */ header block
/* CFLAGS: /Ot /Oy- */
```
- An image's own defaults come first (`vc6.image_cflags`, by path: `src\eqmain\...` gets eqmain's `/O2`), then
  the file's lines.
- The options are **added after** `DEFAULT_CFLAGS` (`/nologo /c /O1 /W3 /MT /Gy /GX`), so cl's last-one-wins
  applies: `/Ot` turns /O1's `/Os` into favor-speed, `/GX-` turns EH off, `/Oy-` keeps the frame pointer.
- An optimization level (`/O1 /O2 /Ox /Od`) **replaces** the default `/O1` (no D4025 warning).
- Several `CFLAGS` lines accumulate. Only code-generation options are allowed (`/O..`, `/G..`, `/Zp<n>`,
  `/Op`); anything else (`/MD`, `/D`, `/I`, ...) is a build error: all objects of one DLL must agree on those.
- Flags apply to the whole file. Within a file, `#pragma optimize` / `#pragma intrinsic` still work for single
  functions, but prefer a separate file with its own `CFLAGS` when a whole object was built differently.
- Measured 2026-10-02 (VC6 SP4): `/GX` is neutral for functions without EH state (all 347 functions of the
  24 sources then in eq-trilogy compile to identical object code with and without it), so it is a default;
  the client has 167 `__EH_prolog` functions (55 of its 87 game functions that call `operator new` have one:
  `new T(...)` of a class with a constructor gets an EH frame under `/GX`). The rdp library (`src/Network/rdp/*.c`) was built for speed:
  `// CFLAGS: /O2` in place of `#pragma optimize("tgy", on)` + `#pragma intrinsic(...)` gives identical
  code for all 76 of its functions.
