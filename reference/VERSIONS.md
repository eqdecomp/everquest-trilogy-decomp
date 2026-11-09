# Supported binaries and compiler

Your own copies go in `<workspace>/original/trilogy/`, never in this repo. The test harness checks these
hashes and skips (with a message) on any other build; `tools/doctor.py` reports them.

## Binaries

| File | Size | Link time (UTC) | SHA-256 | Notes |
|---|---|---|---|---|
| `eqgame.exe` | 1,642,496 | 2001-08-21 23:07 | `ee124a75d46aa61197e9160598c567237573bd656a32865637765165274b3cfa` | The game. VC6 (Rich: Utc12 8797/8799/8966) |
| `eqmain.dll` | 344,064 | 2001-08-20 23:30 | `8048b5d3328aca87e2bc13b4a39b6d2bd0991d5d9b7b76abcd06b261f42a6daa` | Front end (login, character select) |
| `eqgfx_dx7.dll` | 745,472 | 2001-07-28 01:14 | `19c904bcfaa6a03df7af5fab3c644935265f792ed49096b45861d690beab8b96` | Renderer, 1,065 exports |

Other material used as evidence (names, layouts, protocol facts) is listed, without copies or hashes, in
`MANIFEST.md`.

## Compiler

Rich-header compiler objects (prodid 10 = Utc12 C, 11 = Utc12 C++; count of objects):

| Binary | 8168 (RTM) | 8797 | 8799 | 8966 (SP5) | 9044 (Processor Pack) |
|---|---|---|---|---|---|
| `eqgame.exe` | C++ 2 | C 175, C++ 14 | C 3, C++ 94 | C 62, C++ 3 | none |
| `eqgfx_dx7.dll` | C++ 1 | C 147, C++ 12 | C 76, C++ 4 | C 12, C++ 3 | none |
| `eqmain.dll` | – | – | C 30 | C 15, C++ 45 | none (also 132 C + 13 C++ objects at 8047) |

- Builds are the version of `c2.dll`, the back end that writes the object. Checked by compiling with each
  service pack: SP4's `c2.dll` (12.0.8799) writes 8799; SP5's (12.0.8966) writes 8966.
- The CRT in the libraries carries its own build: `LIBC.LIB`/`LIBCMT.LIB` objects are 8168 in RTM, **8797 in
  SP4**, 8047 in SP5 (plus Masm 7299 in all three).
- So **`eqgame.exe` and `eqgfx_dx7.dll` are SP4 builds**: game code 8799, CRT from SP4's libc (8797).
  `eqgame.exe`'s 65 objects at 8966 (62 of them C) were compiled with SP5 elsewhere, most likely a prebuilt C
  library (unconfirmed).
- **`eqmain.dll` is an SP5 build**: game code 8966, CRT 8047 (SP5 libc), plus 30 C objects at 8799.
- No 9044 objects anywhere: the Processor Pack was not used.

### Two service packs side by side

Matching needs both: SP4 for `eqgame.exe` and `eqgfx_dx7.dll`, SP5 for `eqmain.dll`. A service pack
replaces the compiler in place, so keep two copies of the `VC98` folder: install Visual C++ 6.0, copy its
`VC98` folder, apply SP4 to one copy and SP5 to the other (or install twice). Point `paths.local.ini` at them:
```
[toolchain]
vc6     = <folder>\vs6sp4\VC98
vc6_sp5 = <folder>\vs6sp5\VC98
```
`tools/doctor.py` reads each folder's `c2.dll` version and says which service pack it is. A plain
install without service packs runs the fixtures; only the byte-match numbers depend on the exact build.
Visual C++ 6.0 is Microsoft software and is not distributed here.
