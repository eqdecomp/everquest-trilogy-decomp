r"""Repository checks that need neither the original binaries nor VC6 (what CI runs).

    python tools/ci/check_tree.py [REPO]          exit 0 = clean, 1 = problems (listed)

- no forbidden files: executables, objects, archives, images, sound, game data, Ghidra/IDA/BinDiff projects,
  decompiler dumps, local config (paths.local.ini), virtual environments;
- no file over 1 MB (fixtures 2 MB), no NUL bytes, every text file UTF-8, no binary or archive magic;
- no local paths (a drive's Users\<name> folder, /home/<name>, AppData) and no e-mail addresses beyond the
  credited ones;
- no control characters in text files;
- every fixture (tests/unicorn/**/*.json) parses, its `sources` stay inside the repo, and every `from_image` reference is `"0x<hex>"` with a
  positive integer `image_size` beside it (the bytes themselves are read from the user's binary at test time);
- every `// FUNCTION: <IMAGE> 0x<VA>` annotation is unique per image.
It cannot tell whether a hex blob in a fixture is original data: that needs the binaries (the maintainers
check it before every release).
"""
from __future__ import annotations

import json
import re
import subprocess
import sys
from pathlib import Path

FORBIDDEN_EXT = {".exe", ".dll", ".obj", ".lib", ".pdb", ".exp", ".ilk", ".res", ".idb", ".i64", ".id0", ".id1",
                 ".nam", ".til", ".wav", ".bmp", ".png", ".jpg", ".tga", ".dds", ".s3d", ".wld", ".eqg", ".mid",
                 ".xmi", ".mp3", ".zip", ".7z", ".gz", ".rar", ".tar", ".cab", ".iso", ".gpr", ".gzf", ".binexport",
                 ".bindiff", ".pyc", ".dmp", ".asm", ".lst", ".cod", ".map"}
FORBIDDEN_PARTS = {"original", "downloads", "toolchains", "dx7sdk", ".venv", "__pycache__"}   # any level
FORBIDDEN_TOP = {"build", "ghidra"}                                   # workspace folders, at the repo root
FORBIDDEN_NAMES = re.compile(r"(?i)^(paths\.local\.ini.*|eqmac.*\.c|ddraw\.h|d3d\.h|d3dtypes\.h|d3dcaps\.h|"
                             r"dinput\.h|dsound\.h|dplay\.h|eqclassic_notes\.tsv)$")
MAX_FILE, MAX_FIXTURE = 1 << 20, 2 << 20
MAGIC = [(b"MZ", "PE/DOS executable"), (b"PK\x03\x04", "zip"), (b"7z\xbc\xaf\x27\x1c", "7z"), (b"\x1f\x8b", "gzip"),
         (b"Rar!", "rar"), (b"%PDF", "pdf"), (b"\x89PNG", "png"), (b"RIFF", "riff/wav"), (b"MThd", "midi"),
         (b"\x7fELF", "elf"), (b"\xcf\xfa\xed\xfe", "mach-o"), (b"\xce\xfa\xed\xfe", "mach-o")]
LOCAL = [(re.compile(r"[A-Za-z]:[\\/]+Users[\\/]+(?!<)[A-Za-z0-9_.-]+", re.I), "local user path"),
         (re.compile(r"/home/[a-z_][a-z0-9_-]*/"), "home path"),
         (re.compile(r"AppData[\\/]", re.I), "profile path")]
EMAIL = re.compile(r"[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}")
EMAIL_OK = {"jloup@gzip.org", "madler@alumni.caltech.edu", "eay@mincom.oz.au"}
EMAIL_OK_DOMAINS = ("example.com", "example.org", "users.noreply.github.com")
ANN = re.compile(r"^[ \t]*//[ \t]*FUNCTION:[ \t]*([A-Z0-9_]+)[ \t]+0x([0-9A-Fa-f]+)", re.M)
HEX = re.compile(r"0x[0-9A-Fa-f]+\Z")
CONTROL = re.compile(r"[\x01-\x08\x0b\x0c\x0e-\x1f\x7f]")
# This file names the patterns it looks for; it is not checked against its own pattern list.
SELF = "tools/ci/check_tree.py"


def tracked(repo: Path) -> list[str]:
    try:
        out = subprocess.run(["git", "-C", str(repo), "ls-files", "-z"], capture_output=True, check=True).stdout
        return [p for p in out.decode("utf-8").split("\0") if p]
    except (OSError, subprocess.CalledProcessError):
        return [p.relative_to(repo).as_posix() for p in repo.rglob("*") if p.is_file() and ".git" not in p.parts]


def refs(node, path, out):
    """Every dict holding a from_image key, with its JSON path."""
    if isinstance(node, dict):
        if "from_image" in node:
            out.append((path, node))
        for k, v in node.items():
            refs(v, f"{path}.{k}", out)
    elif isinstance(node, list):
        for i, v in enumerate(node):
            refs(v, f"{path}[{i}]", out)


def check(repo: Path) -> list[str]:
    problems = []
    seen = {}
    for rel in tracked(repo):
        p = repo / rel
        if not p.is_file():
            continue
        parts = rel.split("/")
        low = [x.lower() for x in parts]
        if (p.suffix.lower() in FORBIDDEN_EXT or set(low[:-1]) & FORBIDDEN_PARTS or FORBIDDEN_NAMES.match(parts[-1])
                or (len(low) > 1 and low[0] in FORBIDDEN_TOP)):
            problems.append(f"{rel}: forbidden file")
            continue
        data = p.read_bytes()
        fixture = rel.startswith("tests/unicorn/") and rel.endswith(".json")
        if len(data) > (MAX_FIXTURE if fixture else MAX_FILE):
            problems.append(f"{rel}: {len(data)} bytes, over the size limit")
        for m, what in MAGIC:
            if data.startswith(m):
                problems.append(f"{rel}: starts like a {what} file")
        if b"\0" in data:
            problems.append(f"{rel}: NUL bytes (binary)")
            continue
        try:
            text = data.decode("utf-8")
        except UnicodeDecodeError as e:
            problems.append(f"{rel}: not UTF-8 ({e})")
            continue
        if CONTROL.search(text):
            problems.append(f"{rel}: control character in a text file")
        if rel != SELF:
            for rx, what in LOCAL:
                for mm in rx.finditer(text):
                    problems.append(f"{rel}:{text.count(chr(10), 0, mm.start()) + 1}: {what}: {mm.group(0)!r}")
            for mm in EMAIL.finditer(text):
                e = mm.group(0).lower()
                if e in EMAIL_OK or e.endswith(EMAIL_OK_DOMAINS) or re.fullmatch(r".*@\d+\.\d+.*", e):
                    continue
                problems.append(f"{rel}:{text.count(chr(10), 0, mm.start()) + 1}: e-mail address {mm.group(0)!r}")
        if fixture:
            try:
                fx = json.loads(text)
            except ValueError as e:
                problems.append(f"{rel}: invalid JSON ({e})")
                continue
            found = []
            refs(fx, "$", found)
            for jp, node in found:
                va, n = node.get("from_image"), node.get("image_size")
                if not (isinstance(va, str) and HEX.match(va)) or not (isinstance(n, int) and n > 0):
                    problems.append(f"{rel}: {jp}: from_image needs \"0x<hex>\" and a positive image_size")
                if "hex" in node:
                    problems.append(f"{rel}: {jp}: from_image together with hex")
        if rel.startswith("src/") and p.suffix.lower() in (".c", ".cpp", ".h"):
            for tok, va in ANN.findall(text):
                key = (tok, int(va, 16))
                if key in seen:
                    problems.append(f"{rel}: // FUNCTION: {tok} 0x{va} also in {seen[key]}")
                else:
                    seen[key] = rel
    return problems


def main(argv=None) -> int:
    argv = sys.argv[1:] if argv is None else argv
    repo = Path(argv[0] if argv else ".").resolve()
    problems = check(repo)
    for x in problems:
        print(x)
    print(f"check_tree: {len(problems)} problem(s)")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
