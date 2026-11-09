# Contributing

Thank you for helping. This project lives or dies by where its material comes from, so the provenance
rules come first; a change that breaks one of them is rejected however good the code is.

## Provenance rules

1. **No original bytes.** Do not add game files, binaries, DLLs, memory dumps, disassembly listings,
   Ghidra/IDA/BinDiff projects or exports, or bulk copies of the client's code or data, in any form (hex,
   base64, arrays). Inline assembly is only for code the compiler cannot reproduce, and strings only where
   behaviour depends on them. Fixtures that need original data reference it by address (`"from_image": "0x..."`) and
   the harness reads it from the user's own binary; `tools/unicorn/README.md` explains how.
2. **No decompiler output.** Do not paste Hex-Rays, Ghidra or other decompiler output, from this client or
   any other build. Write the function yourself from the evidence; the decompiler is a reading aid.
3. **No leaked or unlicensed source.** Never use leaked Sony/Daybreak source. Do not paste code from GPL or
   unlicensed projects (EQEmu, ShowEQ, MacroQuest, EQClassic, TAKP and others): this repo is MIT. They are
   **evidence only**: facts such as offsets, opcode values, addresses and names may be used and are credited
   in `reference/MANIFEST.md`; their code may not.
4. **Third-party library code** linked into the client (zlib, libdes, ...) is reconstructed from the binary,
   marked as altered in the file, and listed with its licence in `THIRD_PARTY.md`.
5. **Names say where they come from.** A new name goes into `names/` with its source and a confidence, or
   carries the `_guess` suffix.
6. **No connection to live servers**, and no code meant to cheat, bypass protection or attack anyone.

By opening a pull request you confirm that your contribution follows these rules and that you have the right
to submit it under the repository's licence. The maintainers may reject or remove anything whose origin is
unclear.

## How a change is checked

You need the setup from `README.md` ("Quick start"): your own binaries and Visual C++ 6.0. Run
`tools\doctor.py` first.

- **A new or changed function**: annotate it (`// FUNCTION: <IMAGE> 0x...`), add or update its fixture in
  `tests/unicorn/`, and run it:
  `..\.venv\Scripts\python.exe -m pytest tests\unicorn\<path to the fixture or folder>`.
  Then the mutation check (`tools\unicorn\tools\mutate_check.py --va <VA>`) and the match check
  (`tools\compare\compare.py --match --va <VA>`). Say in the pull request what still differs, if anything.
  The full workflow and what "tested" means: `tools/docs/RULES.md`.
- **A rename, comment or other change that must not change code**: run the gate,
  `..\.venv\Scripts\python.exe tools\readability\gate.py --base main`; it compiles every touched file before
  and after and fails on any difference.
- **Before a pull request**: `tools\ci\check_tree.py .` (what CI runs), the fixtures of every file you
  touched, and `tools\compare\source_checks.py` (status markers, fake-match oddities, the readability
  ratchet; it also lists existing violations, so only new ones in your files matter).

CI runs only the checks that need neither the binaries nor VC6 (see `README.md`, "Continuous
integration"); the fixtures and compare must be run locally and their result stated in the pull request.

## Style
- Literal translation over cleanup; keep the original's types, statement order and code shape.
- Fake matches only in the allowed forms, always marked (`tools/docs/RULES.md` "Fake matches").
- One topic per pull request; small is better.
