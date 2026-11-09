r"""Shared helpers for the headless PyGhidra scripts.

Run the scripts with the workspace venv:  .venv\Scripts\python.exe eq-trilogy\tools\ghidra\scripts\<script>.py
Projects live in <workspace>\ghidra\<client>\ and reports in <workspace>\build\ghidra\ (never in a repo).
"""
import configparser
import json
import os
import sys
from pathlib import Path

# client -> (binary relative to [workspace] original, program name inside the project)
CLIENTS = {
    "trilogy": ("trilogy/eqgame.exe", "eqgame.exe"),
    "takp": ("takp/eqgame.exe", "eqgame.exe"),
    "mac": ("mac/eqmac.exe", "eqmac.exe"),
    # January 1999 beta: a name source for Trilogy (its debug strings name functions), not a target.
    "beta": ("beta/eqgame.exe", "eqgame.exe"),
    # Trilogy front end (login, server and character select), VC6 SP5. Phase 1d after eqgame.exe.
    "eqmain": ("trilogy/eqmain.dll", "eqmain.dll"),
    # Trilogy renderer (1,065 named exports), done last for Trilogy.
    "eqgfx": ("trilogy/eqgfx_dx7.dll", "eqgfx_dx7.dll"),
    # TAKP's DX8 renderer: a name source for eqgfx (community function list + BinDiff), not a target.
    "takp_gfx": ("takp/eqgfx_dx8.dll", "eqgfx_dx8.dll"),
}

FID_BOOKMARK_CATEGORY = "Function ID Analyzer"
LIBRARY_TAG = "LIBRARY"


def find_root() -> Path:
    """Workspace root: EQD_ROOT, or the first parent holding paths.local.ini."""
    env = os.environ.get("EQD_ROOT")
    if env:
        return Path(env)
    for start in (Path.cwd(), Path(__file__).resolve()):
        for d in (start, *start.parents):
            if (d / "paths.local.ini").is_file():
                return d
    raise SystemExit("paths.local.ini not found; set EQD_ROOT")


class Workspace:
    def __init__(self):
        self.root = find_root()
        self.cfg = configparser.ConfigParser()
        self.cfg.read(os.environ.get("EQD_CONFIG", self.root / "paths.local.ini"))
        ws = self.cfg["workspace"]
        self.original = self.root / ws.get("original", "original")
        self.ghidra = self.root / ws.get("ghidra", "ghidra")
        self.build = self.root / ws.get("build", "build")
        self.reports = self.root / os.environ.get("EQD_REPORT_DIR", ws.get("ghidra_reports", str(self.build / "ghidra")))
        self.reports.mkdir(parents=True, exist_ok=True)

    def path(self, section, key) -> Path:
        return Path(self.cfg[section][key])

    def binary(self, client) -> Path:
        return self.original / CLIENTS[client][0]

    def program_path(self, client) -> str:
        return "/" + CLIENTS[client][1]

    def repo(self, client) -> Path:
        """The client's repo (eq-trilogy for all three Trilogy images); EQD_REPO wins."""
        if os.environ.get("EQD_REPO"):
            return registry().repo_dir(client, self.root)
        override = self.cfg.get("repos", client, fallback=None)
        return self.root / override if override else registry().repo_dir(client, self.root)

    def archive(self, client) -> Path:
        return self.root / self.cfg.get("ghidra_types", client + "_archive",
                                       fallback=str(self.reports / f"{client}.gdt"))

    def typed_symbols(self, client) -> Path:
        return self.root / self.cfg.get("ghidra_types", client + "_symbols",
                                       fallback=str(self.repo(client) / "ghidra-types" / f"{client}.symbols.csv"))

    def queue(self, client) -> Path:
        return self.repo(client) / registry().get(client).queue

    def write_report(self, name, data):
        out = self.reports / name
        out.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")
        print(f"wrote {out}")
        return out


def registry():
    """unicorn/eqd_unicorn/clients.py, the decomp-target registry (repo, queue, names stem per client)."""
    import importlib.util
    mod = sys.modules.get("eqd_clients")
    if mod is None:
        path = Path(__file__).resolve().parents[2] / "unicorn" / "eqd_unicorn" / "clients.py"
        spec = importlib.util.spec_from_file_location("eqd_clients", path)
        mod = importlib.util.module_from_spec(spec)
        sys.modules["eqd_clients"] = mod
        spec.loader.exec_module(mod)
    return mod


def names_stem(client) -> str:
    """names/<stem>.*.csv: the program name without extension (eqgame, eqmain, eqgfx_dx7)."""
    return CLIENTS[client][1].rsplit(".", 1)[0]


def start(ws: Workspace):
    """Start the JVM with the Ghidra from paths.local.ini."""
    import pyghidra
    # A folder named "ghidra" on sys.path (the workspace has one) shadows the Java package and makes
    # PyGhidra recurse forever on import.
    sys.path[:] = [p for p in sys.path if not (Path(p or ".") / "ghidra").is_dir()]
    pyghidra.start(install_dir=ws.path("toolchain", "ghidra"))
    return pyghidra


def open_project(ws: Workspace, client, create=False):
    import pyghidra
    d = ws.ghidra / client
    d.mkdir(parents=True, exist_ok=True)
    return pyghidra.open_project(str(d), client, create=create)


def is_default_name(name: str) -> bool:
    return name.startswith(("FUN_", "thunk_FUN_"))


def is_eh_funclet(name: str) -> bool:
    """Compiler-generated C++ exception handling stubs that Ghidra splits out as functions."""
    return name.startswith(("Unwind@", "Catch@", "Catch_All@"))


def fid_addresses(program) -> set:
    """Entry points that carry a Function ID bookmark."""
    out = set()
    for b in program.getBookmarkManager().getBookmarksIterator("Analysis"):
        if b.getCategory() == FID_BOOKMARK_CATEGORY:
            out.add(b.getAddress())
    return out


def function_class(f, fid_addrs) -> str:
    """One of: library, import_thunk, eh_funclet, unnamed, named."""
    name = f.getName()
    if f.getEntryPoint() in fid_addrs or any(t.getName() == LIBRARY_TAG for t in f.getTags()):
        return "library"
    if f.isThunk() and f.getThunkedFunction(True).isExternal():
        return "import_thunk"
    if is_eh_funclet(name):
        return "eh_funclet"
    return "unnamed" if is_default_name(name) else "named"


def function_stats(program) -> dict:
    """Counts used by every report. Each function lands in exactly one "classes" bucket."""
    from ghidra.program.model.address import AddressSet
    fid_addrs = fid_addresses(program)
    classes = dict.fromkeys(("library", "import_thunk", "eh_funclet", "named", "unnamed"), 0)
    by_source = {}
    total = fid = 0
    body = AddressSet()
    for f in program.getFunctionManager().getFunctions(True):
        total += 1
        body.add(f.getBody())
        src = str(f.getSymbol().getSource())
        by_source[src] = by_source.get(src, 0) + 1
        fid += f.getEntryPoint() in fid_addrs
        classes[function_class(f, fid_addrs)] += 1
    stats = {"functions": total, "fid_matched": fid, "classes": classes, "by_symbol_source": by_source}
    text = program.getMemory().getBlock(".text") or program.getMemory().getBlock("__text")
    if text is not None:
        ts = AddressSet(text.getStart(), text.getEnd())
        undefined = program.getListing().getUndefinedRanges(ts, False, None)
        stats["text"] = {
            "bytes": int(ts.getNumAddresses()),
            "in_functions": int(body.intersect(ts).getNumAddresses()),
            "undefined": int(undefined.getNumAddresses()),
        }
    return stats
