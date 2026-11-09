# permute/ – random source mutations for near-miss functions (VC6 SP4)

A decomp-permuter for MSVC 6: for one annotated function whose match-mode build is *almost* the original
(`compare.py --match` status `diff`, e.g. the near misses in `build\match\nearmiss.txt`), it tries many
small, semantics-preserving rewrites of the function body, compiles each one exactly like `compare.py
--match` and keeps what brings the code closer to the original's bytes. The winner is checked with the
function's Unicorn fixtures and written as a patch; **the decomp repo is never written to**.

```
cd eq-trilogy
..\.venv\Scripts\python.exe tools\permute\permute.py --snapshot --va 0x004A3911
..\.venv\Scripts\python.exe tools\permute\permute.py --snapshot --nearmiss ..\build\match\nearmiss.txt --budget 600
..\.venv\Scripts\python.exe tools\permute\permute.py --snapshot --match-json ..\build\match\match.json --status diff
```
`--nearmiss` also reads a `compare.py --nearmiss` file.

Options: `--budget` seconds per function (default 600; stops early at score 0, or 1, see below), `--iters`,
`--jobs` parallel compiles (default CPUs - 1; each worker compiles in its own folder), `--seed`, `--safe`
(no value-changing mutations), `--no-verify`, `--no-full-check` (skip step 5), `--full-tries` (default 6),
`--out` (default `<build>\match\permute`), `--work` (default
`<build>\permute\work`), `--unicorn-build` (default `$EQD_UNICORN_BUILD` or `<build>\permute_ub`).
`--snapshot [dir]` first copies `include/ src/ names/ hooks/ tests/unicorn/` of the repo **as committed**
(`git archive HEAD`, `--snapshot-rev <rev>`, or `worktree` for the working tree) to `<build>\permute\snap`
and works on that, so other people's edits in progress don't get in the way. Patches are against that
snapshot (its `SNAPSHOT.txt` names the commit).

Output per function: `<out>\<VA>.diff` (a unified diff `a/src/... b/src/...`, `git apply`-able, with a `#`
header line: score, status and ratio before/after, the whole-source check, fixture result, the mutations
kept), a line in `<out>\summary.tsv` (with the ratios and what the whole-source check said of each candidate
it tried), and in `<work>\<VA>\`: `result.json` and `best_asm.diff` (the remaining instruction diff, if any).
No diff is written when no candidate passes the whole-source check.

## How

1. **Unit.** `tu.py` cuts the source down to the target plus what it needs: everything at file scope
   (includes, declarations, classes, globals, `// CFLAGS:` lines) and the definitions the target names,
   transitively. Every other definition is dropped with its annotation. The target must compile to the same
   instructions in the unit as in the full source (else the full source is used).
2. **Score** (`worker_score`). The unit is compiled by `compare.MatchContext.compile` (SP4, the source's
   CFLAGS, `/DEQD_MATCH`, the generated `eqd_match_syms.h` force-included) and compared by
   `MatchContext.compare_obj`, the same code as `compare.py --match`. Score (`score_of`), ordered like the
   landing check (status, then compare ratio): **0** byte-identical (outside relocation fields), **1**
   instruction-identical but bytes differ (in practice `EQ_THISCALL`'s dummy `xor edx, edx`, which no source
   change removes; the search stops there too), else **2 + 1000 per 0.0001 of `1 - ratio` + the differing
   instruction count** (`ndiff`, a tie-break between equal ratios); none when it doesn't build. A lower score
   therefore never means a lower ratio. (The score used to be 10 per differing instruction plus 1000 when the
   count differs. `ndiff` compares labelled tokens position by position, so one extra instruction renumbers
   every later branch label, and the +1000 made any same-count candidate beat any other: e.g. 0x10010AB0
   went from 53/51 instructions, ratio 0.9808, to 53/53 with register swaps, score 1100 -> 60 but ratio 0.9057,
   and was dropped at landing, like 14 others.)
3. **Search.** Hill climbing: a candidate is a best-scoring one (sometimes one within 0.005 of ratio) with
   1–4 more random mutations; a better one replaces the pool, an equal one joins it (drift across plateaus),
   already seen bodies are skipped. Compiles run in a process pool (`cl.exe` runs fine in parallel with
   separate output folders; each worker loads the client image once).
4. **Minimise.** Each mutation on the winner's path is reverted as a whole (all its hunks, located by
   context; skipped if a later mutation touched them) and the reversion kept when the score holds.
5. **Whole-source check** (`FullCheck`), the landing check: the whole source with the new body is compiled at
   its own path (a copy of `src/`) and compared like `compare.py --match` does (every annotated function,
   cross-TU call keys); the target must score better than with the unmodified source and no other function
   may get a worse status (identical < normalised < diff/structural) or a lower ratio. The search scores the
   cut-down unit with calls keyed by name, so this also catches unit-vs-source differences. When the
   minimised winner fails, the raw winner and then the earlier bests (latest first) are tried, up to
   `--full-tries`; the first that passes is the result.
6. **Verify.** The function's fixtures (every `tests/unicorn/**/*.json` whose `original` is the VA) run with
   pytest in a copy of the repo with the patched source, through the repo's own `tests/unicorn/conftest.py`
   with `EQD_TOOLS` = this tools tree and `EQD_UNICORN_BUILD` = `--unicorn-build`. The unmodified source
   runs first; when it fails too, the result says so. No fixture: `verified` is `None`.

## Mutations (`mutate.py`)

Token-level (`ctoken.py`: tokenizer with offsets, bracket matching, a statement parser for blocks / if-else
/ loops / switch / labels / declarations, operand finding with C precedence, and a rough read/write model:
locals and their fields, other names, memory through pointers by field name, calls). Edits are text splices,
so the rest of the source keeps its layout.

| mutation | does | why VC6 cares |
|---|---|---|
| `swap_stmts`, `move_stmt` | reorder independent statements (no read/write overlap, calls order kept) | scheduling, store order |
| `swap_decls`, `move_decl` | reorder local declarations (before first use; C: within the declarations) | stack slots, register choice |
| `scope_decl` | move a declaration into the innermost block holding all its uses, or out (never into a loop) | callee-saved push/pop placement, regalloc |
| `split_decl_init` | `T x = e;` <-> `T x; x = e;` | |
| `commute`, `flip_relational` | `a+b` -> `b+a` (+ * & \| ^ == !=), `a<b` -> `b>a` (operands without conflicting side effects) | operand order, x87 load order |
| `invert_if`, `invert_if_noelse` | `if (c) A else B` -> `if (!c) B else A` (dangling else wrapped) | block order, jcc polarity |
| `else_after_jump` | `if (c) {..return;} else S` <-> `if (c) {..return;} S` | |
| `tail_return` | `if (c) S; return v;` -> `if (c) { S; return v; } return v;` (or `return K` when v still holds the literal K) | early-exit shape, register lifetimes |
| `split_and`, `merge_if` | `if (a && b) S` <-> `if (a) { if (b) S }` (no else) | |
| `zero_compare` | in conditions `x != 0` <-> `x`, `x == 0` <-> `!x` | |
| `ternary_to_if`, `if_to_ternary` | `x = c ? a : b;` <-> `if (c) x = a; else x = b;` (and returns) | |
| `for_to_while`, `while_to_for`, `loop_rotate` | loop forms (no `continue` in moved bodies); `while (c) B` -> `if (c) do B while (c);` | loop layout |
| `incdec`, `hoist_incdec` | `i++` / `++i` / `i += 1` / `i = i + 1`; `if (--x != 0)` <-> `--x; if (x != 0)` | |
| `add_temp`, `cse`, `remove_temp`, `inline_var` | hoist a subexpression into a typed temporary (one or every use), or inline a temporary (cast to its type) | redundant loads, regalloc |
| `register` | add/remove `register` (not used by the search: `register` pinning is banned, docs/RULES.md) | |
| `chain_assign` | `a = v; b = v;` / `a = v; b = a;` <-> `b = a = v;` | which register holds v, store order |
| `compound_assign` | `x = x op y` <-> `x op= y` | |
| `negate_const` | `x - 4` <-> `x + -4` (and `-=`/`+=`) | `sub` vs `add -N` |
| `bool_return` | `return a < b;` <-> `if (a < b) return 1; [else] return 0;` | setcc vs branches |
| `reassociate` | `a + b + c` <-> `a + (b + c)` (+ * & \| ^, no side effects) | operand order |
| `cond_temp` | `if (c)` -> `int t = c; if (t)` (C: only where declarations may go) | condition into a register first |
| `scope_block` | wrap a declaration and its uses in `{ }`, or take a nested block apart | stack slot sharing |
| `pow2_shift` (**unsafe**) | `x * 2^n` <-> `x << n`, `x / 2^n` <-> `x >> n`, `x % 2^n` -> `x & (2^n-1)` | signed/float differ |
| `ref_local` (**unsafe**, C++) | `T x = lv;` <-> `const T& x = lv;` | a reference rereads its object |
| `const_compare` (**unsafe**) | `x < C` <-> `x <= C-1` (integer literal, or an all-caps name: `x >= N` -> `x > (N - 1)`; wrong for float x) | compare constants |
| `change_type` (**unsafe**) | another integer type for a local (width/signedness) | movsx/movzx, widths |
| `cast` (**unsafe**) | drop an integer cast or add one to an operand | |

Temporaries need the expression's type: `probe_types` asks the compiler once per function, inserting
`{ struct eqd_probe_s eqd_pN = (expr); }` in front of each statement; cl names the type in C2440 (C++) or,
with a pointer probe, C4047/C4133 (C: integer and pointer types only). Struct copies, arrays and function
types are never made into temporaries.

**Semantics.** The safe mutations check side effects, evaluation order (calls, `&&`/`||`/`?:` short
circuits, `++`/`--`, assignments inside expressions), aliasing (a call may change any non-local or
address-taken local; `p->f` may be any `f`), loops and dangling elses, conservatively. The model is
heuristic (no types: a C++ object with a constructor is treated like a POD, implicit `this->` members are
names); the unsafe ones change values by design. Hence the fixture check, and the patches are proposals for
review, not commits. `--safe` leaves the unsafe ones out.

## Limitations / ideas

- Only the function body changes. Signatures, return types, struct layouts, header declarations, CFLAGS,
  and other functions are never touched (e.g. `ExpansionFlag*_guess`'s `movsx` vs `mov al` looks like a
  type outside the body).
- Pointer-walking loops (induction-variable rewrites like `for (p = t; p < t + 20; p++)`), switch
  rewrites, goto-based shapes and macro-level changes are not generated.
- Plateau search only: no crossover, no weights learned from what helped. The score is compare's ratio (a
  `difflib` alignment of normalised instructions, rounded to 4 places); a finer one (e.g. per-byte or
  regalloc-aware distance) might guide better.
- `minimise` reverts whole mutations, but a later mutation's preconditions were checked with the earlier
  one in place; the fixture run is the safety net.
- Fixtures cover what they cover: a passing run is evidence, not proof.

Tests: `tests\permute\` (tokenizer, parser, effects, every mutation, unit extraction, the search and
minimiser with a fake scorer, the score's order and the whole-source verdict; the type probe against VC6 when
the workspace has it).

## Bake-off against cpp_permuter (`bakeoff.py`)

```
..\.venv\Scripts\python.exe tools\permute\bakeoff.py --repo SNAP --va 0x004F2370 ... --compiles 200 -j 4 ^
    --tools ours,cpp,cppx --out OUT
..\.venv\Scripts\python.exe tools\permute\bakeoff.py verify --repo SNAP --run OUT --va 0x004F2370 --unicorn-build UB
```
Runs permute.py and cpp_permuter (random `cpp`, exhaustive depth 1 `cppx`; `--cpp-args` for more) on the
same unit, the same `compare.py --match` compile line and the same score (`compare_obj` + `score_of`;
cpp_permuter through `--score-cmd "bakeoff.py score {obj} {src}"`, a named-pipe score server), with the
same number of compiles each (`--compiles`). `--without a,b` drops mutations (the baseline), `--tag` names
the row. Rows go to `OUT\bakeoff.tsv`. `verify` checks every score-0 candidate on a private copy
(`compare.py --match` status + the Unicorn fixtures, unmodified and patched) and writes patches with that
evidence to `OUT\..\matches\`. Results: docs/PLAN.md 9.1. Set `PYTHONHASHSEED=0` until
`compare.write_match_header` sorts deterministically (its workers otherwise race on the header).
