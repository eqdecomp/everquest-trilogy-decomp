r"""Decomp targets ("clients"): one per original image, several images per decomp repo.

`-c/--client` everywhere names one image. The primary image of a repo keeps the repo's name (`trilogy` =
eqgame.exe in eq-trilogy); other images of the same repo get their own id (`eqmain` = eqmain.dll, later
`eqgfx` = eqgfx_dx7.dll). The id decides, for that image:

  annotation token   `// FUNCTION: <ID upper-case> 0x...`  (TRILOGY, EQMAIN)
  sources/fixtures   src/<prefix>/...  and  tests/unicorn/<prefix>/...   (prefix "" for the primary image)
  queue file         <repo>/<queue>           (queue/queue.jsonl, queue/eqmain.jsonl)
  names              <repo>/names/<names>.*.csv
  export, builds     <build>/export/<id>, <build>/<id>/..., <build>/loop/<id>.results.jsonl
  toolchain          paths.local.ini [toolchain] <toolchain>  (vc6 = SP4, vc6_sp5 = SP5); per source: a
                     `// TOOLCHAIN: <key>` line, else the image's folder rule (toolchain_dirs), see source_toolchain
  fixture defaults   <repo>/<crt_defaults>   (the image's own CRT addresses)

See docs/EQMAIN.md. Pure data and path helpers: no imports from the rest of the package, so tools that
must stay light (queue/queuelib.py, ghidra/scripts/eqd_ghidra.py) load this file directly.
"""
from __future__ import annotations

import os
import re
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


@dataclass(frozen=True)
class Client:
    id: str
    repo: str                       # decomp repo folder in the workspace
    binary: str | None              # relative to [workspace] original
    base: int                       # preferred image base (the PE header has the real one)
    toolchain: str = "vc6"          # [toolchain] key in paths.local.ini
    queue: str = "queue/queue.jsonl"
    prefix: str = ""                # sub-folder of src/ and tests/unicorn/ ("" = the repo's primary image)
    names: str = ""                 # names/<names>.*.csv
    crt_defaults: str = "tests/unicorn/crt_defaults.json"
    compiler: str = "Visual C++ 6.0 SP4"
    compiler_detail: str = ""       # for the context bundle
    title: str = ""
    hooks: bool = True              # the repo's hook DLL patches this image (hooks/)
    graph_labels: bool = True       # build_queue: namespace and call-graph subsystem rounds
    units: bool = False             # build_queue: unlabelled functions get address-run units (no Mac map)
    siblings: tuple = ()            # other images whose code this one shares (BinDiff pairs, clones)
    unit_max: int = 0               # build_queue: split address-run units longer than this many functions (0 = off)
    cflags: tuple = ()              # default options of its sources (src/<prefix>/...), before their CFLAGS lines
    toolchain_dirs: tuple = ()      # ((folder under src/<prefix>/, [toolchain] key), ...): sources built with another
                                    # toolchain than the image's (a `// TOOLCHAIN:` line in the file still wins)
    new_ext: str = ".cpp"           # extension of a NEW source file (eqgfx: the t3d renderer is C, so ".c")

    @property
    def token(self) -> str:
        return self.id.upper()

    @property
    def primary(self) -> bool:
        return not self.prefix

    def src_dir(self, repo) -> Path:
        return Path(repo) / "src" / self.prefix if self.prefix else Path(repo) / "src"

    def tests_dir(self, repo) -> Path:
        t = Path(repo) / "tests" / "unicorn"
        return t / self.prefix if self.prefix else t


CLIENTS = {
    "trilogy": Client(
        "trilogy", "eq-trilogy", "trilogy/eqgame.exe", 0x00400000, toolchain="vc6", names="eqgame",
        compiler="Visual C++ 6.0 SP4",
        compiler_detail="Visual C++ 6.0 SP4 (c2 12.0.8799, LIBCMT 8797), `/O1 /MT`; no Processor Pack",
        title="Trilogy"),
    # Front end (login, server and character select). Built with SP5 (c2 8966, libc 8047) plus 30 C objects
    # at 8799 (the rdp library eqgame.exe links too). /O2: its game code inlines strlen (524 `repne scasb`) and
    # has none of /O1's `push imm8; pop reg` (eqgame.exe: 826). No Mac map: subsystems come from eqgame siblings
    # (BinDiff pairs, names/siblings/) and address-run units. No hooks yet (the DLL is loaded at run time).
    "eqmain": Client(
        "eqmain", "eq-trilogy", "trilogy/eqmain.dll", 0x10000000, toolchain="vc6_sp5",
        queue="queue/eqmain.jsonl", prefix="eqmain", names="eqmain",
        crt_defaults="tests/unicorn/crt_defaults_eqmain.json",
        compiler="Visual C++ 6.0 SP5",
        compiler_detail="Visual C++ 6.0 SP5 (c2 12.0.8966, LIBC 8047), `/O2 /MT` (intrinsic strlen/strcpy/memcpy "
                        "inline: `repne scasb`, `rep movsd`); its rdp/C library objects are SP4 (8799) builds shared "
                        "with eqgame.exe; no Processor Pack",
        title="Trilogy front end (eqmain.dll)", hooks=False, graph_labels=False, units=True,
        siblings=("trilogy",), cflags=("/O2",),
        # The rdp objects are the 30 C objects at 8799 (SP4) of the Rich header. SP4 and SP5 give the same
        # instructions for all of our code (docs/EQMAIN.md "Per-file toolchain"); the object's @comp.id differs.
        toolchain_dirs=(("Network/rdp", "vc6"),)),
    # Renderer (1,065 exports named in the DLL: t3d*/s3d*). SP4 like eqgame.exe (Rich: c2 8799 + libc 8797), /O1
    # (to be confirmed by the pilot rewrites). No Mac map and no namespaces: subsystems come from siblings
    # (the packfs/ffcreate2 code eqgame.exe also links: BinDiff trilogy_vs_eqgfx), the zlib range (build_queue)
    # and address-run units. No hooks yet (the DLL is loaded at run time, like eqmain.dll).
    "eqgfx": Client(
        "eqgfx", "eq-trilogy", "trilogy/eqgfx_dx7.dll", 0x10000000, toolchain="vc6",
        queue="queue/eqgfx.jsonl", prefix="eqgfx", names="eqgfx_dx7",
        crt_defaults="tests/unicorn/crt_defaults_eqgfx.json",
        compiler="Visual C++ 6.0 SP4",
        compiler_detail="Visual C++ 6.0 SP4 (c2 12.0.8799) plus some SP5 objects, `/O1 /MT`",
        title="Trilogy renderer (eqgfx_dx7.dll)", hooks=False, graph_labels=False, units=True,
        siblings=("trilogy",), unit_max=40, new_ext=".c"),
}


# [toolchain] key -> what the prompt and the context bundle call it.
TOOLCHAIN_NAMES = {"vc6": "Visual C++ 6.0 SP4", "vc6_sp5": "Visual C++ 6.0 SP5", "vc6_rtm": "Visual C++ 6.0 RTM"}

# `// TOOLCHAIN: vc6` (or ` * TOOLCHAIN: ...` in a block comment, or `/* TOOLCHAIN: ... */`): the file is built
# with that [toolchain] of paths.local.ini instead of its image's (vc6.py, compare --match, permute, stackcmp).
TOOLCHAIN_LINE = re.compile(r"^[ \t]*(?://|/\*|\*)[ \t]*TOOLCHAIN:[ \t]*(\S+?)[ \t]*(?:\*/)?[ \t]*\r?$", re.M)


def image_of_source(path):
    """(client, folder parts below src/<prefix>/) of a source path (absolute or repo-relative) in another image's
    sub-folder (`.../src/eqmain/Network/rdp/x.c` -> eqmain, ('Network', 'rdp')), else (None, ())."""
    parts = [x for x in PurePosixPath(str(path).replace("\\", "/")).parts]
    low = [x.lower() for x in parts]
    for i, x in enumerate(low[:-2]):
        if x == "src":
            for c in CLIENTS.values():
                if c.prefix and low[i + 1] == c.prefix.lower():
                    return c, tuple(parts[i + 2:-1])
    return None, ()


def new_source(subsystem) -> str:
    """Repo-relative path of a NEW source file for a queue subsystem (`eqgfx/unit_10001000` ->
    src/eqgfx/unit_10001000.c): the subsystem's image decides the extension (Client.new_ext)."""
    rel = f"src/{subsystem or 'unassigned'}"
    c, _ = image_of_source(rel + ".x")
    return rel + (c.new_ext if c else ".cpp")


def source_toolchain(path, text=None):
    """The [toolchain] key a source is built with when it isn't its image's own, else None: the last
    `// TOOLCHAIN: <key>` line of `text` (the file's content; read from `path` when None and it exists), else
    its image's folder rule (Client.toolchain_dirs, by `path`; the file need not exist)."""
    if text is None:
        try:
            text = Path(path).read_text(encoding="utf-8", errors="replace")
        except OSError:
            text = ""
    keys = TOOLCHAIN_LINE.findall(text)
    if keys:
        return keys[-1]
    c, folders = image_of_source(path)
    if c is not None:
        low = "/".join(folders).lower() + "/"
        for d, key in c.toolchain_dirs:
            if low.startswith(d.strip("/").lower() + "/"):
                return key
    return None


def toolchain_name(key) -> str:
    return TOOLCHAIN_NAMES.get(key, f"the `{key}` toolchain")


def get(client: str) -> Client:
    """The registered client, or a single-image default for any other id (eq-<id>, queue/queue.jsonl)."""
    c = CLIENTS.get(client)
    if c is not None:
        return c
    return Client(client, f"eq-{client}", None, 0, names=client, title=client.title())


def of_repo(repo_name: str) -> list:
    """Clients living in one decomp repo (by its folder name), primary first."""
    out = [c for c in CLIENTS.values() if c.repo.lower() == str(repo_name).lower()]
    return sorted(out, key=lambda c: (not c.primary, c.id))


def repo_dir(client: str, root) -> Path:
    """The client's decomp repo: EQD_REPO (a worktree or copy standing in for it), else <root>/<repo>."""
    env = os.environ.get("EQD_REPO")
    return Path(env) if env else Path(root) / get(client).repo


def _parts(rel) -> tuple:
    p = PurePosixPath(str(rel).replace("\\", "/"))
    parts = [x for x in p.parts if x not in ("", ".")]
    if parts[:2] == ["tests", "unicorn"]:
        parts = parts[2:]
    elif parts[:1] == ["src"]:
        parts = parts[1:]
    return tuple(parts)


def client_of_path(rel, repo_clients=None, default: str = "trilogy") -> str:
    """Which image a repo-relative path belongs to: `src/eqmain/...`, `tests/unicorn/eqmain/...` or a
    subsystem `eqmain/...` -> eqmain; anything else -> the primary image (`default`)."""
    parts = _parts(rel)
    if len(parts) < 2:              # a file directly in src/ or tests/unicorn/ (crt_defaults.json)
        return default
    head = parts[0].lower()
    for c in (repo_clients if repo_clients is not None else CLIENTS.values()):
        if c.prefix and c.prefix.lower() == head:
            return c.id
    return default


def owns(client: str, rel) -> bool:
    """True if the repo-relative source/fixture path (or subsystem) belongs to `client`."""
    c = get(client)
    peers = of_repo(c.repo) or [c]
    primary = next((x.id for x in peers if x.primary), c.id)
    return client_of_path(rel, peers, default=primary) == c.id


def sources(repo, client: str, suffixes=(".c", ".cpp")) -> list:
    """The client's own sources under <repo>/src (another image's sub-folder is left out)."""
    repo = Path(repo)
    src = repo / "src"
    if not src.is_dir():
        return []
    out = []
    for p in sorted(src.rglob("*")):
        if p.suffix.lower() in suffixes and p.is_file() and owns(client, p.relative_to(repo).as_posix()):
            out.append(p)
    return out


def subsystem(client: str, sub: str) -> str:
    """A queue subsystem in the client's namespace: `Network/rdp/rdp` -> `eqmain/Network/rdp/rdp` (library,
    eh_funclet and already-prefixed names are kept)."""
    c = get(client)
    if not c.prefix or not sub or sub in ("library", "eh_funclet") \
            or sub.lower().startswith(c.prefix.lower() + "/"):
        return sub
    return f"{c.prefix}/{sub}"


def unassigned(sub) -> bool:
    """A subsystem that names no file yet (`unassigned`, `eqmain/unassigned`)."""
    return not sub or str(sub).rsplit("/", 1)[-1] == "unassigned"


def results_name(client: str) -> str:
    return f"{client}.results.jsonl"


def load_file(path=None):
    """This module loaded by file path (for tools that don't import the eqd_unicorn package)."""
    import importlib.util
    import sys
    path = Path(path or __file__)
    name = "eqd_clients"
    mod = sys.modules.get(name)
    if mod is not None and Path(getattr(mod, "__file__", "")).resolve() == path.resolve():
        return mod
    spec = importlib.util.spec_from_file_location(name, path)
    mod = importlib.util.module_from_spec(spec)
    sys.modules[name] = mod
    spec.loader.exec_module(mod)
    return mod
