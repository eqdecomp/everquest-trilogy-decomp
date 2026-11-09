# tools

The tooling of this decompilation, vendored into the repo as `tools/` (it is developed as a separate
project and copied in for each release). It contains no game code, game data, or decompiler output.

## Contents
- `docs/PLAN.md`: project roadmap, evidence and decisions.
- `docs/RULES.md`: how code is written and checked.
- `docs/tool-index.md`: which tool solves which need, with the command.
- `docs/VC6_QUIRKS.md`: known Visual C++ 6 code-generation patterns, each with a repro; `docs/EQ_NOTES.md`:
  facts about the client's own code; `docs/EQMAIN.md`, `docs/EQGFX.md`: the two DLLs as extra images.
- `doctor.py`: checks a workspace (Python, packages, `paths.local.ini`, binaries and their hashes, VC6
  service packs) and says what is missing.
- `paths.example.ini`: template for the workspace `paths.local.ini`.
- `requirements.txt`: the Python dependencies; `requirements-lock.txt`: the same, pinned with their
  dependencies as tested (Python 3.13).
- `unicorn/`: the differential test harness (original function vs VC6-built rewrite under Unicorn), see
  `unicorn/README.md`.
- `compare/`: instruction/byte comparison of rewrites with the original (`compare.py`, match mode),
  `source_checks.py` (status markers, fake-match oddities, readability ratchet), `progress.py`
  (`PROGRESS.md`), and reccmp glue (`stackcmp.py`, `reccmp_map.py`), see `compare/README.md`.
- `permute/`: VC6 permuter, random semantics-preserving source mutations for near-miss functions, scored
  by `compare/` match mode and checked with the fixtures, see `permute/README.md`.
- `readability/`: name registry and conflicts (`registry.py`), safe rename of functions/globals/locals
  everywhere a name lives (`rename.py`), the gate "compiles to the same code as before" (`gate.py`), and the
  scripted-pass report (`passes.py`), see `readability/README.md`.
- `structs/`: struct-layout census, layout JSON -> C header generator (bring your own JSON; ours is
  not published) and checks.
- `context/`: one function's evidence (pseudocode, assembly, callers/callees, structs) in one bundle, from a
  local Ghidra export; similar-function examples; the kit for very large functions.
- `ghidra/`: headless PyGhidra scripts (projects, FID, name chain, types, coverage); `export/`: read-only
  JSON export of a Ghidra project for `context/` and `compare/`. Both need your own Ghidra projects.
- `ci/check_tree.py`: the repository checks CI runs (no binaries needed).
- `tests/`: the tools' own tests. Many need a workspace with the binaries and VC6 and skip without them.

## Setup
See the repo `README.md` ("Quick start"). In short, from the workspace folder:
```
py -3.13 -m venv .venv
.venv\Scripts\python -m pip install -r eq-trilogy\tools\requirements-lock.txt
.venv\Scripts\python eq-trilogy\tools\doctor.py
```

## License
MIT. See `LICENSE`.
