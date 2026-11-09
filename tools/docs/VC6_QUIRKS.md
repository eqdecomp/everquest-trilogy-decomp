# VC6 SP4 codegen quirks

Patterns in what VC6 SP4 (c2 12.00.8799, the eqgame.exe compiler) makes of a given C/C++ shape, for
matching near misses. One claim per entry, HoMM1 `docs/patterns/` style: the claim, a minimal repro, what
SP4 emits, and how to use it. **Every entry here was reproduced on SP4** with `compare/probe.py`; claims
from elsewhere that don't reproduce are listed at the end, not silently dropped.

Flags are the match-mode defaults (`/O1 /W3 /MT /Gy /GX`, `eqd_unicorn.vc6.DEFAULT_CFLAGS`) unless the
entry says /O2 (the rdp sources and a few others use `// CFLAGS: /O2`). Many patterns differ between /O1
and /O2, so check the source's flags first.

**Repros.** Each entry's snippet is in `docs/vc6_quirks/<file>` with its claim written as checks
(`// EXPECT:` lines, see `compare/probe.py`). Rerun them all after a toolchain or probe change:
```
..\.venv\Scripts\python.exe tools\compare\probe.py --check-quirks           PASS/FAIL per entry
..\.venv\Scripts\python.exe -m pytest tools\tests\compare\test_probe.py     the same, as tests
..\.venv\Scripts\python.exe tools\compare\probe.py a.c b.c -f f             try your own A vs B
```

**Selecting entries.** Every entry starts with `### <id>` and a `keywords:` line (the same id and keywords
head its repro file as `// QUIRK:` / `// KEYWORDS:`). A bundle packer can pick entries whose keywords
appear in a near miss's diff summary ("jump table", "block order", "regalloc", "EH", "stack slot", ...).

## Control flow and layout

### vc6q-switch-table-threshold
keywords: switch, jump table, dec chain, case count, /O1, /O2 · repro: `vc6_quirks/switch_table_threshold.c`

**Claim.** A dense switch gets a jump table from **8** cases at /O1 but from **4** at /O2. Below that VC6
tests with `sub eax, 0; je` / `dec eax; je` chains.

**SP4.** /O1 with cases 0..6: `sub eax, 0; je` + six `dec eax; je`; 0..7: `cmp eax, 7; ja; jmp [eax*4 +
table]`. /O2 with 0..2: chain; 0..3: `cmp eax, 3; ja; jmp [eax*4 + table]`.

**Use.** A `dec` chain in a /O1 original doesn't mean an if-chain: a switch with up to 7 cases compiles
that way. Conversely a jump table with 4–7 cases means /O2 (or more cases than you have).

### vc6q-switch-byte-index
keywords: switch, jump table, index table, sparse switch, binary search, /O2 · repro: `vc6_quirks/switch_byte_index.c`

**Claim.** Sparse cases share a dword table through a byte index table (`mov cl, byte ptr [eax + idx];
jmp [ecx*4 + tbl]`) while the span is small enough (cases 0,3,6,200: yes); past that (0,3,6,300) VC6
builds a compare tree (`cmp eax, K; jg; je ...`, split near the median).

**Use.** The byte table's values give the case → body mapping; the dword table has one entry per distinct
body plus default. A compare tree in the original with a wide span is still a switch.

### vc6q-switch-default-label
keywords: switch, jump table, default, dec chain, merged labels, /O2 · repro: `vc6_quirks/switch_default_label.c` · from gta2_re

**Claim.** `case 4: default:` counts as three cases plus a default, so /O2 makes a `dec` chain; writing
`case 4:` with its own (identical) body gives the 4-entry jump table.

**Use.** If the original has a table and you have a chain, look for a case label merged into `default`.

### vc6q-switch-one-case
keywords: switch, compare, dec, branch · repro: `vc6_quirks/switch_one_case.c` · from gta2_re

**Claim.** `switch (x) { case 1: ... }` loads x and tests `dec eax; je`; `if (x == 1)` gives
`cmp dword ptr [mem], 1; jne`.

**Use.** A lone `mov eax, x; dec eax; jcc` in the original is a one-case switch.

### vc6q-switch-case-order
keywords: switch, jump table, block order, case order, layout · repro: `vc6_quirks/switch_case_order.c` · from gta2_re

**Claim.** With a jump table, case bodies are laid out in source order (cases written 3,2,1,0 put the
`g(13)` block first). In a /O1 compare chain the layout is the same whatever the source order.

**Use.** Read the jump table targets in address order to get the original's case order. For short /O1
chains, case order is not a lever.

### vc6q-if-then-first
keywords: branch, block order, if else, jcc polarity, layout · repro: `vc6_quirks/if_then_first.c` · from gta2_re, HoMM1

**Claim.** The `then` block is laid out first (fall-through) and the condition jumps to `else`.
`if (a) g(); else h();` → `je else; jmp _g; else: jmp _h`; `if (!a) h(); else g();` → `jne; h; g`.

**Use.** If the original has your `else` block first, invert the condition and swap the blocks
(permuter: `invert_if`). Nesting decides which block is "then", so block order follows nesting.

### vc6q-saved-comparison
keywords: branch, block order, compound comparison, int, temporary, /O1 · repro: `vc6_quirks/saved_comparison.cpp` · from EQ

**Claim.** Saving a compound comparison in an `int` before an `if` can change branch polarity and
block order even when VC6 eliminates the temporary. At /O1, five equality tests joined by `||` differ
between `int hit = ...; if (hit)` and a direct `if (...)`.

**SP4.** Saved: the last compare uses `jne`, followed by the yes block then the no block. Direct: `je`,
with the no block first. Both have five compares and no materialised boolean.

**Use.** Keep a named compound test when inlining it changes an otherwise matching branch layout
(GetActorTag, 0x004B156C).

## Compares and booleans

### vc6q-setcc-compare
keywords: setcc, sete, setle, compare, bool, return · repro: `vc6_quirks/setcc_compare.c`

**Claim.** `return a == b;` (and `<`, `<=`, `!= 0`, `!x`) on ints is `xor eax, eax; cmp; setcc al` with the
`xor` first. `if (a <= b) return 1; return 0;` gives exactly the same code.

**Use.** A setcc in the original doesn't tell you whether the source returned the compare or branched.

### vc6q-bool-not-call
keywords: bool, setcc, sete, neg sbb, not, ternary · repro: `vc6_quirks/bool_not_call.cpp` · from gta2_re

**Claim.** On a `bool` call result, `!f()` gives `neg al; sbb eax, eax; inc eax`; `f() ? false : true`
gives `test al, al; sete al` at /O2 and the byte form `neg al; sbb al, al; inc al` at /O1. For an `int`
result both spellings give the dword `neg/sbb/inc`.

**Use.** `neg/sbb/inc` vs `test/sete` (or byte vs dword `sbb`) picks the spelling; check the return type
first.

### vc6q-literal-address-fcomp
keywords: float, compare, fcomp, NaN, unordered, EQ_GLOBAL, literal address, test DLL, operand order · repro: `vc6_quirks/literal_address_fcomp.c`

**Claim.** `if (v >= G)` with `G` a float global compiles differently in the two builds of a rewrite. Through
a symbol (the match build's EQD_MATCH expansion of `EQ_GLOBAL`, like the original) it is `fld v; fcomp [G];
fnstsw; sahf; jb fail`. Through a literal address (the test DLL's and hook DLL's `*(float *)va`) VC6 loads
the global first: `fld [va]; fcomp v; ...; ja fail`. Both agree on ordered values. On NaN, `jb` (CF=1) takes
the reject path and `ja` doesn't, so the test DLL accepts NaN where the original rejects it. Reading the
global through a local pointer (`const float *m = &G; if (v >= *m)`) gives `fld v; fcomp [G]; jb` in both
builds.

**Use.** When `match` says identical but `test` fails only on NaN (or -NaN) cases of a float compare
against an `EQ_GLOBAL`, the harness is testing the literal-address build. Read the global through a local
pointer (t3dSetRenderContextNoFogDistance, 0x10043192), then rerun match: it must still be identical.

## Evaluation order and scheduling

### vc6q-call-order
keywords: evaluation order, arguments, call order, operand order · repro: `vc6_quirks/call_arg_order.c` · from gta2_re

**Claim.** Arguments are evaluated right to left: `kk(g(), h())` calls `h` first. In `a() + b()` VC6
picks the call order itself; `h() + k()` and `k() + h()` compile identically.

**Use.** Don't swap operands of `+` to reorder calls; use a temporary (`int r = b(); r += a();`).

### vc6q-store-order
keywords: store order, scheduling, zero store, constant, field stores · repro: `vc6_quirks/store_order.c` · from HoMM1, gta2_re

**Claim.** Field stores mostly keep source order, but zero stores get pulled together: /O1 emits them as
`and dword ptr [..], 0` *first*; /O2 zeroes one register and stores it from the first zero store on.
`p->a = x; p->b = 0; p->c = y; p->d = 0;` → /O1: `and [+4],0; and [+0xc],0; mov [+0],x; mov [+8],y`.

**Use.** The original's store order reflects the source order of the non-constant stores only; reorder
those (permuter: `swap_stmts`), not the zero stores.

## Arithmetic

### vc6q-signed-div-pow2
keywords: arithmetic, modulo, division, power of two, idiv, /O1, /O2 · repro: `vc6_quirks/signed_div_pow2.c` · from HoMM1

**Claim.** Signed `% 8` / `/ 4`: /O1 keeps `push 8; cdq; pop ecx; idiv ecx`; /O2 uses
`and eax, 0x80000007; jns; dec; or eax, -8; inc` and `cdq; and edx, 3; add eax, edx; sar eax, 2`.
Unsigned operands give `and` / `shr` at both levels.

**Use.** An `idiv` by a constant power of two means /O1 (and a signed operand); the `0x80000007` form
means /O2. A `shr`/`and` where you have `idiv` means the operand is unsigned.

### vc6q-o1-idioms
keywords: /O1, /O2, constants, push pop, stack cleanup, call · repro: `vc6_quirks/o1_idioms.c`

**Claim.** /O1 size idioms: small constants via `push 9; pop eax` (/O2: `mov eax, 9`), one stack argument
cleaned with `pop ecx` (/O2: `add esp, 4`). Both use `or eax, 0xffffffff` for -1.

**Use.** Quick flag check: `push imm; pop reg` and `pop ecx` after calls mean /O1; `add esp, 4` means /O2.

### vc6q-fldz-return
keywords: x87, float, fldz, fld1, constant, return, ternary · repro: `vc6_quirks/float_zero_one.c` · from the DX7 review

**Claim.** A `return 0.0f;` / `return 1.0f;` *statement* loads the constant from .rdata
(`fld dword ptr [__real@...]`); the same value reached through `?:` or an assignment (`r = 0; ... return r;`,
`p->f = 0.0f`) gives `fldz` / `fld1`. No /O level or /Op /Oi /Ot changes it.

**Use.** An original with `fldz`/`fld1` where the rewrite loads a constant: route the value through a ternary
or a local instead of a separate `return 0.0f;`. And the reverse for a memory load.

### vc6q-short-push
keywords: short, parameter, push, upper bits, mov cx · repro: `vc6_quirks/short_push.cpp` · from EQ

**Claim.** Passing a memory `short` to a `short` parameter can load only the low word of a register
before pushing the full dword; the upper bits need not be extended.

**SP4.** Three short arguments: `mov cx, word ptr [eax + 4]; push ecx`, then more word loads and dword
pushes. Declaring the callee's parameters as `int` gives three `movsx` loads instead.

**Use.** Match the callee's parameter widths before trying to remove apparent missing extensions.

### vc6q-byte-temp-width
keywords: byte, temporary, widening, zeroing, regalloc, /O2 · repro: `vc6_quirks/byte_temp_width.cpp` · from EQ

**Claim.** At /O2, widening an `unsigned char` temporary to `unsigned int` can widen a register copy
and add zeroing before the byte load, even when the result is ultimately stored as a byte.

**SP4.** The decoder's byte temporary gives `mov al, [edx]; mov cl, al; and ecx, 0x80`; the widened
temporary gives `xor eax, eax; mov al, [edx]; mov ecx, eax; and ecx, 0x80`. Later allocation also changes.

**Use.** Inspect the whole listing when widening a byte to match a dword copy. The EQ observation
used `dl` / `edx` on SP5; the SP4 reduction uses `cl` / `ecx` and reproduces the added zeroing.

## Exception handling

### vc6q-eh-frame
keywords: EH, exception, frame, destructor, __EH_prolog, fs:[0], state, /GX · repro: `vc6_quirks/eh_frame.cpp` · from HoMM1, gta2_re

**Claim.** An EH frame is a source fact. Under /GX, a local with a destructor that's alive across a call
gets one, and a function without such an object never does. /O1: `mov eax, <handler>; call __EH_prolog`
(ebp frame; state `and [ebp-4], 0` after construction, `or [ebp-4], -1` before destruction). /O2:
written out (`push -1; push <handler>; mov eax, fs:[0]; ...`), state in an esp slot.

**Use.** Frame in the original and none in yours: you're missing a destructible local (or the object
has no destructor in your headers). The entry state counts the destructible objects.

### vc6q-new-ctor-eh
keywords: EH, new, constructor, throw(), definition order, frame, extern "C", linkage · repro: `vc6_quirks/new_ctor_eh.cpp` · from gta2_re, EQ

**Claim.** `new T(x)` gets an EH frame (to free the memory if the constructor throws) unless VC6 knows the
constructor can't throw: for example a `throw()` declaration, or an inline or **earlier non-inline
definition in the same TU** whose body it can prove non-throwing. Definition order alone is not enough.

**SP4.** An earlier constructor doing arithmetic or calling only `extern "C"` functions drops the
frame. An earlier constructor calling a declared C++ function keeps it, as does the later definition.

**Use.** A missing or extra frame around `new` can come from where the constructor's body sits in the
file. Keep definitions in original address order (RULES: one source per original object, ascending
addresses) before trying anything else.

### vc6q-raw-vtable-state
keywords: EH, vtable, vfptr, state, scheduling, constructor, destructor, /O1 · repro: `vc6_quirks/raw_vtable_state.cpp` · from EQ

**Claim.** At /O1 a destructor's raw vfptr store in a non-polymorphic stand-in can be scheduled after
the EH-state store and argument setup. A real base supplies the unwind lifetime but does not make the
raw write follow compiler-generated destructor scheduling.

**SP4.** With a real base in both variants, the raw destructor emits `and [ebp-4], 0; push esi; mov
[esi], table`; the polymorphic destructor emits `mov [esi], vftable; and [ebp-4], 0; push esi`.

**Use.** Reconstruct real bases/members for correct unwind states. For a remaining destructor
vfptr/state ordering mismatch, check whether the original class was polymorphic.

### vc6q-inline-vfptr-dtor
keywords: EH, destructor, vfptr, inline, out-of-line copy, unwind · repro: `vc6_quirks/inline_vfptr_dtor.cpp` · from EQ

**Claim.** An inline empty virtual destructor can leave no destructor call on the normal path while
EH cleanup uses an out-of-line copy that only restores the vfptr.

**SP4.** A local live across a C++ call keeps its EH frame. With the inline destructor there is no
normal-path destructor call; its emitted copy is `mov [ecx], vftable; ret`. Defining it out of line after
the caller keeps a normal-path call too.

**Use.** An unwind-only destructor call does not imply a missing normal-path call in the rewrite
(EQSoundBuffer, out-of-line destructor 0x004E24FC).

### vc6q-c-linkage-try
keywords: EH, try, catch, rethrow, extern "C", linkage, STL, realloc · repro: `vc6_quirks/c_linkage_try.cpp` · from EQ

**Claim.** VC6 drops `try { copy(...); } catch (...) { release(...); throw; }` when the try's only
call is declared `extern "C"`. A declared C++ copy function retains the catch frame.

**SP4.** C linkage: allocation and copy calls with no `__EH_prolog`. C++ linkage: `__EH_prolog` and
a state store before the copy.

**Use.** A missing STL-style insert/reallocation cleanup frame can come from callee linkage; use
C++ declarations (or `EQ_CDECL_CPP`) for C++ callees.

### vc6q-new-state-reset
keywords: EH, new, state, extern "C", linkage, join · repro: `vc6_quirks/new_state_reset.cpp` · from EQ

**Claim.** After a new-expression join, VC6 can omit the reset to state -1 when all later calls are
`extern "C"`, even though construction itself still needs an EH frame.

**SP4.** A declared constructor keeps `__EH_prolog` in both variants. C-only later calls omit
`or [ebp-4], -1`; making the last call C++ restores that reset before an intervening C call.

**Use.** For a missing reset, inspect the linkage of later callees, including delete/error helpers;
the declaration responsible may be several calls after the differing store.

### vc6q-c-linkage-nothrow
keywords: EH, state, extern "C", linkage, nothrow, cdecl, /GX, add esp, EQ_CDECL, EQ_CDECL_CPP · repro: `vc6_quirks/c_linkage_nothrow.cpp`

**Claim.** Under /GX VC6 treats an `extern "C"` function as non-throwing, also when it is called through a cast
of its address. In a function with an EH frame it emits no state stores around such a call, where a call to a
C++-linkage function gets them (`mov byte ptr [ebp - 4], 1` before, `and byte ptr [ebp - 4], 0` after, inside an
inner object's lifetime). It can also merge the argument cleanup of neighbouring calls into one `add esp`.

**SP4.** The same `use` with two destructible locals: through `extern "C" eqd_fn_0x00401000` 37 instructions,
through `eqd_cc<0x00401000>::f` (C++ linkage) 39: the two state stores around the second call.

**Use.** In a match build `EQ_CDECL`'s target is extern "C" (the generated `eqd_match_syms.h`). Where the
original C++ code keeps EH states or separate cleanups around a call to a client C++ function, write
`EQ_CDECL_CPP` / `EQ_STDCALL_CPP` (eqaddr.h; compare/README "C++ cdecl targets").

## Statics, inlining and frames

### vc6q-static-guard
keywords: static, guard, function static, local static, atexit, constructor · repro: `vc6_quirks/static_guard.cpp` · from HoMM1

**Claim.** A function-local static with a constructor or a non-constant initialiser gets a guard bit in a
compiler byte `?$S<n>@...`: `test byte ptr [guard], 1; jne; or byte ptr [guard], 1; <init>`, plus
`push $E<n>; call _atexit` when it has a destructor. A function's statics share one byte (bits 1, 2, ...
in declaration order).

**Use.** The bit order gives the declaration order of the statics. No atexit means no destructor.

### vc6q-inline-o1
keywords: inline, inline budget, out-of-line copy, COMDAT, /O1, /O2, Ob1 · repro: `vc6_quirks/inline_o1.cpp` · from HoMM1, gta2_re

**Claim.** /O1 and /O2 both imply /Ob1: only `inline` (or in-class) functions are expanded; a `static`
helper stays a call however small. At /O1 an inline is expanded only while its own expanded body stays
small: a leaf is expanded, but `mid` (whose body expands two inline leaves that each call `g`) is **not**;
the call stays and an out-of-line COMDAT copy is emitted in the same object. /O2 expands it. (No expansion-count budget showed up: 128 calls of an inline leaf all expanded.)

**Use.** An out-of-line copy of a small inline, called from a /O1 function, is expected, not a sign of
different source. Don't hand-expand it: keep the inline and its call.

### vc6q-asm-cleanup
keywords: inline, __asm, cleanup, optimisation, frame · repro: `vc6_quirks/asm_cleanup.cpp` · from EQ

**Claim.** An inline destructor containing `__asm { nop }` can change the enclosing function's frame
and argument addressing, including code before the cleanup. Its effect is not limited to the final nop.

**SP4.** A sum loop with that cleanup uses `push ebp; mov ebp, esp` and ebp-relative arguments.
An empty destructor gives esp-relative arguments with no frame. Neither version needs an EH frame.

**Use.** Removing an assembly cleanup marker can disturb code throughout a function. Recheck the
whole listing; this small repro establishes the frame effect, not all EnterZone allocation changes.

### vc6q-const-ref-inline
keywords: inline, const reference, by value, struct copy, movsd, frame · repro: `vc6_quirks/const_ref_inline.cpp` · from gta2_re

**Claim.** A struct passed by value to an inline helper is still copied to a frame temporary (`movsd` x4,
ebp frame) after expansion; through `const V&` the fields are read in place.

**Use.** A struct copy around an inlined helper means a by-value parameter; none means a reference.

### vc6q-block-scope-slots
keywords: stack, stack slot, frame size, block scope, switch, locals · repro: `vc6_quirks/block_scope_slots.cpp` · from HoMM1, gta2_re

**Claim.** Locals in disjoint blocks (braces around each switch arm) share a stack slot; at function scope
each gets its own (frame 0x10 vs 0x20 for two `char[16]`).

**Use.** Frame bigger than the original's: scope the temporaries. Two arms with distinct slots in the
original: hoist them to function scope.

### vc6q-decl-order-names
keywords: regalloc, register, stack slot, declaration order, rename, entropy, slot order · repro: `vc6_quirks/decl_order_names.c`

**Claim.** On SP4, the **names** and the **declaration order** of locals don't change the code, neither
registers nor stack slots (address-taken locals included). The order of the *uses* does change slot order.
Only initialised declarations "move" code, because moving them moves the initialiser (statement order).

**Measured on real near misses** (docs/PLAN.md 9.6, 2026-10-04): 15 functions from
`build\match\nearmiss.txt` (regalloc/slot diffs: PutPlayerOnFloor, GetStatus, GetVolume, CommandSlot,
TurnInfravisionEffectOn/Off, UpdateWeather, Heap_extractMinFromHeap, fast_malloc_init,
MarkColumnCellsAsActive, ClearTextBuffer, EQ_Character ctor, ReadHighScores, vlong divide, modinv), each
local renamed three ways (`zq_<n>`, `a0`, `Qrenamed_<reversed>`) and every adjacent declaration pair
swapped: 97 variants (84 renames, 13 swaps), **0 changed the code**.

**Use.** For regalloc/slot near misses, don't spend permuter budget on renames (no rename pass was added
to `permute/mutate.py`). The existing `swap_decls`/`move_decl` passes only help through initialisers;
statement order, use order, scopes and temporaries are the levers. After a cleanup or rename pass, still
rerun `compare.py --match` on the whole file: the TU-level effects below (definition order, include order)
are real.

## Checked, doesn't hold on SP4

Each was probed on SP4 with the flags given; the repro is in the named file or below.

- **Renaming a local moves slots/registers** (HoMM1 on VC4; isledecomp "entropy"). No: see
  `vc6q-decl-order-names`.
- **Declaration order picks registers or slots** (gta2_re: "declaration order of Fix16 locals picks which
  product goes first", "field-index locals: try every declaration order"). Not for plain declarations: with
  identical statements, reordering the declarations of `unsigned short`, `int` and address-taken `int`
  locals, and of small C++ objects with inline constructors, changed nothing at /O1 or /O2. Only reordering
  *initialised* declarations did, which is statement order. (gta2_re's cases are /O2 C++ with by-value
  `Fix16` temporaries; not reproduced in a small repro.)
- **`if (a <= b) return 1; return 0;` gives `setle` without the `xor`** (gta2_re). No: identical to
  `return a <= b;` for ints (`vc6q-setcc-compare`).
- **An expansion-count inline budget** (gta2_re "big functions run out of inline expansions"). Not
  reproduced with a leaf inline (128 expansions in one function, all inline) or a 4-deep single-call
  chain (64x). The /O1 nesting limit in `vc6q-inline-o1` is what showed up. gta2_re's cases are /O2 C++
  operator chains in large functions; recheck when a real EQ near miss shows it.
- **`!f()` vs `f() ? 0 : 1` differ** (gta2_re) holds only for `bool` results (`vc6q-bool-not-call`); for
  `int` results both give `neg/sbb/inc`.
- **Raw and compiler-generated constructor vfptr stores have different EH-state ordering** (from EQ).
  Not in the /O1 real-base reduction (`vc6_quirks/raw_vtable_state.cpp`): both constructors emit state 0,
  argument push, then vfptr store. The destructor difference does reproduce (`vc6q-raw-vtable-state`).

## From gta2_re

Source: CriminalRETeam/gta2_re (GTA2 matching decomp, VC6, mostly `/O2 /GX` C++), its `docs/`
(no LICENSE file at the repo root; knowledge only, no code
copied). Folded in so far, each reproduced on SP4: `matching_quirks.md` entries on switch tables and
default labels, one-case switches, case/block order, call order, store order, `!f()` vs ternary, EH frames
around `new` / ctor definition order, block-scope slot sharing, const-ref vs by-value inlines, inline
out-of-line copies (entries marked "from gta2_re" above; non-reproducing ones in the section before).

Not folded yet:
- `matching_quirks.md`: the many `Fix16`/`Ang16`-specific entries (GTA2's fixed-point class; the general
  lessons are covered above), global definition placement changing load width ("a 2-byte global defined in
  the same file loads as 32 bits", "even a global's name can change code"): worth a probe once EQ
  globals are real definitions rather than `eqd_va_` externs in match mode; "include order can change
  codegen"; tail-merging/cross-jumping entries; parameter-slot reuse; `T x = f()` vs `T x; x = f()`.
  Each needs a C++ repro before it goes in.
- `x87_handoff.md` and the x87 section ("assigning to an f32 local adds a rounding node"): probe once an
  EQ float near miss needs it (PutPlayerOnFloor, SetMinDistance are candidates).
- `match_attempts.md`, `inlines_96f.md`: per-function GTA2 logs, nothing generic beyond the above.
- `permuter.md`: candidate passes for the bake-off (docs/PLAN.md 9.1), not ported.
