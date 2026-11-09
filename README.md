# eq-trilogy

> **Legal warning:** This is an unofficial, unaffiliated reverse-engineering project. It contains no game
> files, and you need your own legally obtained copy of the client to use it. EverQuest is a trademark of its
> owners. For research and preservation only: do not use it with official servers or to break any agreement
> you are bound by.

> **AI notice:** Most of the code and documentation was written with AI tools (Claude, Codex and DeepSeek).
> Every function is checked against the original by the tests and compare tools, not taken on trust, but
> names, comments and notes can still be wrong.

A decompilation of the EverQuest Trilogy client (2001): `eqgame.exe`, `eqmain.dll` and `eqgfx_dx7.dll`. The
goal is readable C and C++ that behaves exactly like the original and, with Visual C++ 6, compiles to the
same machine code.

## Status

| Image | Identical functions | Identical code | Similarity |
|---|---:|---:|---:|
| `eqgame.exe` | 69.2% | 22.4% | 78.7% |
| `eqmain.dll` | 67.6% | 31.5% | 78.0% |
| `eqgfx_dx7.dll` | 55.0% | 17.8% | 67.0% |
| **Total** | **64.0%** | **22.5%** | **75.7%** |

Identical code is the share of instructions in byte-identical functions, so it is the strict figure: most
small functions match, most large ones do not yet. [PROGRESS.md](PROGRESS.md) has the counts and definitions.

This repository holds 4,064 reconstructed functions, 3,965 of them with a differential test.
It does not build a playable game.

Known gaps:

- 34 source files still contain small `__asm` blocks (about 1 KB of transcribed instructions in all,
  including `__ftol` calls) where no C form has been found yet.
- Two functions keep placeholder names (`FUN_00438a29`, `FUN_1001b1e0`), and many names end in `_guess`:
  those are inferred, not taken from symbols.
- Not included: the D3DX units of `eqgfx_dx7.dll`, fixtures that contain client text, and the hooks DLL
  (only its CRT support file is here).

## How it is checked

Each rewritten function carries its original address (`// FUNCTION: TRILOGY 0x...`) and is checked against
**your own copy** of the client:

- **Fixtures** (`tests/unicorn/`) run the original and the VC6-built rewrite side by side in an emulator on
  the same inputs. Return values, memory writes and calls must agree.
- **compare** (`tools/compare/`) checks the rewrite's instructions and bytes against the original's.

## Terms

- **Fixture**: a JSON test case in `tests/unicorn/` that runs one function, original and rewrite, on the
  same inputs and compares the results.
- **Identical function**: compiles to exactly the original's bytes (addresses aside).
- **Identical code / Similarity**: see Status above; similarity is an instruction-weighted score, so
  100% would mean every function is identical.
- **`_guess`** on a name: inferred from behaviour or context, not from a symbol. `FUN_<address>`: no name yet.
- **`@early-stop`**: the function behaves like the original but its machine code still differs; the
  comment says how.
- **`@equivalent`**: differs only in a way that cannot change behaviour (block order, for example).

## Legal

- Not affiliated with or endorsed by Daybreak Game Company or Sony Online Entertainment. EverQuest is a
  trademark of its owners.
- **This repository contains no game files or assets:** no executables, models, textures, sounds, music,
  archives or data files. Bring your own legally obtained binaries; `reference/VERSIONS.md` lists the
  supported builds by SHA-256.
- It does contain material derived from the client, so that the code can be read and tested: reconstructed
  source, much of which compiles to the original machine code; a small amount of inline assembly transcribed
  from the client where the compiler cannot reproduce it; about 500 of the client's text strings, kept where
  behaviour depends on them; and addresses, structure layouts, packet opcodes, and symbol and source file
  names. Other original data is not stored: tests refer to it by address and read it from your binaries.
- Microsoft's D3DX library, statically linked into `eqgfx_dx7.dll`, is left out, as is the code around it.
- For research, preservation and interoperability. Do not use it to connect to official servers.
- MIT for this project's own contributions; that grants nothing in EverQuest or other third-party material.
  See [LICENSE](LICENSE) and [THIRD_PARTY.md](THIRD_PARTY.md).
- **Rights holders:** see [SECURITY.md](SECURITY.md) to reach us privately. We respond and comply quickly.

## Quick start (Windows)

You need Python 3.13, git, your own client binaries, and Visual C++ 6.0: SP4 for `eqgame.exe` and
`eqgfx_dx7.dll`, SP5 for `eqmain.dll` ([details](reference/VERSIONS.md)).

```
mkdir C:\eqws & cd /d C:\eqws
git clone <this repository> eq-trilogy
copy eq-trilogy\tools\paths.example.ini paths.local.ini
py -3.13 -m venv .venv
.venv\Scripts\python -m pip install -r eq-trilogy\tools\requirements-lock.txt
```

Keep the folder name `eq-trilogy`. Copy your binaries to `C:\eqws\original\trilogy\`, set `[toolchain]` in
`paths.local.ini`, then:

```
cd eq-trilogy
..\.venv\Scripts\python tools\doctor.py
..\.venv\Scripts\python -m pytest tests\unicorn\eqmain\Network\rdp\tx\tx_init.json -v
```

`doctor.py` lists anything missing. Without the binaries or VC6, fixtures skip rather than fail, so a fresh
clone alone tests nothing. See [tools/README.md](tools/README.md) for the full tool set.

## Layout

```
src/         reconstructed code (src/eqmain/ and src/eqgfx/ for the DLLs)
include/     struct and class definitions, global addresses
tests/       differential fixtures
names/       address-to-name lists, with source and confidence
hooks/       one compiler helper that the test DLLs link
reference/   supported binaries, compiler, references
tools/       test harness, compare, permuter, readability and Ghidra scripts
```

## What's next

- **Large functions.** Most small functions match; most large ones are close but not identical, which is
  why identical code (22.5%) trails identical functions (64.0%). About 540 functions carry `@early-stop`.
- **`eqgfx_dx7.dll`** is the furthest behind of the three.
- **The `__asm` leftovers** in 34 files, to be replaced with plain C.
- **Names**: confirming `_guess` names and naming the last `FUN_` functions.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md), provenance rules first. Security issues: [SECURITY.md](SECURITY.md).
