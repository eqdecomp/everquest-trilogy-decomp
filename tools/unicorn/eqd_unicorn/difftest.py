"""Differential tests: run the original and the rewrite on identical snapshots and compare what a caller
can observe. See ../README.md for the fixture format.
"""
from __future__ import annotations

import copy
import json
import os
import random
import re
import struct
from dataclasses import dataclass, field
from pathlib import Path

from . import annotations as ann
from . import coverage as cov
from . import externs, thiscall_stubs
from .extent import function_extent, stack_purge
from .machine import (CALLEE_POPS, CPU_FAULTS, ENDINGS, ESP0, SCALARS, VARARGS, Call, Callee, HarnessError,
                      Machine, RunResult, arg_slots, f64_bits_to_f80, float_to_f80, sx)
from .pe import Image, load_pe, read_map, undecorate
from .vc6 import build_dll

REWRITE_BASE = 0x30000000      # the test DLL is linked at 0x10000000 and relocated here
CONVS = ("cdecl", "stdcall", "thiscall", "fastcall")
INT_RETS = {"int": 32, "i32": 32, "u32": 32, "uint": 32, "ptr": 32, "long": 32, "dword": 32,
            "short": 16, "i16": 16, "u16": 16, "ushort": 16, "char": 8, "i8": 8, "u8": 8, "uchar": 8,
            "bool": 8}
MAX_REPORTED_CASES = 8
LOCAL_CLASS = re.compile(r"@\?\d+\?\?")        # MSVC scope of a class local to a function


def _int(v) -> int:
    if isinstance(v, bool):
        return int(v)
    if isinstance(v, int):
        return v
    if isinstance(v, str):
        return int(v, 0)
    raise HarnessError(f"expected an integer, got {v!r}")


_X87_REG = re.compile(r"(?i)(st|fp|mm)\(?[0-7]\)?|fpsw|fpcw|fptag|x87")


def _f80(v) -> bytes:
    """One `fpu` entry as the 10 bytes `fld tbyte` loads: a number or "nan"/"inf"/"-inf" (as a double),
    {"f32": x}, {"f64": x} (a number, or "0x..." raw bits), or {"f80": "0x..."} (raw 80 bits: sign+exponent in
    the top 16, the 64-bit mantissa with its explicit integer bit below)."""
    if isinstance(v, bool):
        raise HarnessError(f"fpu: expected a number, got {v!r}")
    if isinstance(v, (int, float)):
        return float_to_f80(v)
    if isinstance(v, str):
        if v.lower().startswith("0x"):
            raise HarnessError(f"fpu: {v!r} is ambiguous: write {{\"f64\": \"{v}\"}} (double bits) or "
                               f"{{\"f80\": \"...\"}} (80-bit bits)")
        try:
            return float_to_f80(float(v))
        except ValueError:
            raise HarnessError(f"fpu: expected a number, got {v!r}") from None
    if isinstance(v, dict) and len(v) == 1:
        (k, x), = v.items()
        raw = isinstance(x, str) and x.lower().startswith("0x")
        if k == "f80" and raw:
            b = int(x, 16)
            if b >> 80:
                raise HarnessError(f"fpu: f80 value {x} has more than 80 bits")
            return struct.pack("<QH", b & ((1 << 64) - 1), b >> 64)
        if k == "f64":
            return f64_bits_to_f80(int(x, 16)) if raw else float_to_f80(float(x))
        if k == "f32":
            f = struct.unpack("<f", struct.pack("<I", int(x, 16)))[0] if raw else \
                struct.unpack("<f", struct.pack("<f", float(x)))[0]
            return float_to_f80(f)
    raise HarnessError(f"fpu: expected a number, \"nan\"/\"inf\", or one of {{f32|f64|f80: ...}}, got {v!r}")


def load_fixture(path) -> dict:
    fx = json.loads(Path(path).read_text(encoding="utf-8"))
    fx.setdefault("_path", str(path))
    return fx


def callee_key(k):
    """Callee keys: a VA ("0x..." or int) becomes an int, so "0x005223D0" and "0x5223d0" are one key."""
    if isinstance(k, int) or (isinstance(k, str) and k.lower().startswith("0x")):
        return _int(k)
    return k


def apply_defaults(fixture: dict, defaults: dict | None) -> dict:
    """Merge a client's default fixture fragment (e.g. its CRT `callees` + `rewrite_aliases`) under a fixture.

    Dict-valued keys merge per entry; a fixture entry replaces the default with the same key as a whole, and
    `null` removes it. Other keys: the fixture's value wins. Keys starting with "_" are comments. A fixture
    with `"defaults": false` gets none."""
    if not defaults or fixture.get("defaults", True) is False:
        return fixture
    fx = dict(fixture)
    for k, dv in defaults.items():
        if k.startswith("_"):
            continue
        if isinstance(dv, dict):
            norm = callee_key if k == "callees" else (lambda x: x)
            merged = {norm(a): b for a, b in dv.items()}
            for a, b in (fixture.get(k) or {}).items():
                merged[norm(a)] = b
            fx[k] = {a: b for a, b in merged.items() if b is not None}
        elif k not in fx:
            fx[k] = copy.deepcopy(dv)
    return fx


IMAGE_REF = "from_image"


def resolve_image_refs(fixture, image: Image):
    """Fixture values taken from the user's own original image instead of being stored in the fixture:
    `{"from_image": "0xVA", "image_size": N, ...}` anywhere in a fixture (a `memory`/`init` entry, a callee's
    `writes`) becomes `{"hex": <the N bytes of the original image at VA>, ...}`. Published fixtures use this for
    original `.data`/`.rdata` content (game text, constants), so they hold addresses, not the image's bytes.
    With `"as": "cstr"` the reference is a string value instead (a case argument, a fuzz choice, an expected
    return): `{"from_image": "0xVA", "image_size": N, "as": "cstr"}` becomes the N bytes as a latin-1 string.
    Returns a new fixture; one without references is returned as is."""
    def has(v):
        if isinstance(v, dict):
            return IMAGE_REF in v or any(has(x) for x in v.values())
        if isinstance(v, list):
            return any(has(x) for x in v)
        return False

    def res(v):
        if isinstance(v, dict):
            if IMAGE_REF in v:
                va, n = _int(v[IMAGE_REF]), _int(v.get("image_size", 0))
                if n <= 0:
                    raise HarnessError(f"{IMAGE_REF} needs a positive image_size: {v}")
                if not (image.contains(va) and image.contains(va + n - 1)):
                    raise HarnessError(f"{IMAGE_REF} {va:#x}+{n} is outside {image.name}")
                data = image.read(va, n)
                if v.get("as") == "cstr":
                    if set(v) - {IMAGE_REF, "image_size", "as"}:
                        raise HarnessError(f"a {IMAGE_REF} string has no other keys: {v}")
                    return data.decode("latin-1")
                if "as" in v:
                    raise HarnessError(f"{IMAGE_REF}: `as` must be \"cstr\": {v}")
                out = {k: x for k, x in v.items() if k not in (IMAGE_REF, "image_size")}
                out["hex"] = data.hex()
                return out
            return {k: res(x) for k, x in v.items()}
        if isinstance(v, list):
            return [res(x) for x in v]
        return v
    return res(fixture) if has(fixture) else fixture


def _seed_key(seed) -> str:
    """The fixture's fuzz seed, salted by EQD_FUZZ_SALT when set (held-out reseed check: the same fixture with
    other random cases; a rewrite that only passes its own seed's cases fails under another salt)."""
    salt = os.environ.get("EQD_FUZZ_SALT")
    return f"{seed}~{salt}" if salt else f"{seed}"


# Memory sizes of the scalar types a fuzzed memory field can have.
MEM_SIZES = {"int": 4, "i32": 4, "long": 4, "u32": 4, "uint": 4, "dword": 4, "ptr": 4, "float": 4,
             "short": 2, "i16": 2, "u16": 2, "ushort": 2, "char": 1, "i8": 1, "u8": 1, "uchar": 1, "bool": 1,
             "double": 8, "i64": 8, "u64": 8}


# --- results -----------------------------------------------------------------------------------------------

@dataclass
class CaseReport:
    index: int
    label: str
    diffs: list
    original: RunResult
    rewrite: RunResult

    @property
    def ok(self) -> bool:
        return not self.diffs


@dataclass
class Report:
    function: str
    cases: list = field(default_factory=list)
    coverage: dict | None = None        # original-side coverage of a complete run (coverage.record)
    warnings: list = field(default_factory=list)    # fixture problems found statically (callee stub purges)

    @property
    def ok(self) -> bool:
        return bool(self.cases) and all(c.ok for c in self.cases)

    @property
    def failures(self) -> list:
        return [c for c in self.cases if not c.ok]

    def kinds(self) -> set:
        """The kinds of difference seen ("return", "esp", "writes", "trace", ...)."""
        return {d.split(":", 1)[0] for c in self.failures for d in c.diffs}

    def summary(self) -> str:
        bad = self.failures
        lines = [f"{self.function}: {len(self.cases) - len(bad)}/{len(self.cases)} cases match"]
        if bad:
            lines += [f"  warning: {w}" for w in self.warnings]
        for c in bad[:MAX_REPORTED_CASES]:
            lines.append(f"  case {c.index} [{c.label}]")
            lines += [f"    {d}" for d in c.diffs]
        if len(bad) > MAX_REPORTED_CASES:
            lines.append(f"  ... {len(bad) - MAX_REPORTED_CASES} more failing cases")
        return "\n".join(lines)


# --- the test ----------------------------------------------------------------------------------------------

@dataclass
class Side:
    name: str
    entry: int
    allowed: set
    aliases: dict


class DiffTest:
    """One function: original entry + rewrite entry + fixture. Use `Harness.difftest()` to build one."""

    def __init__(self, machine: Machine, fixture: dict, original: Side, rewrite: Side, original_image=None,
                 coverage_dir=None, purge_of=None):
        """`original_image`: measure the original side's coverage (Report.coverage); `coverage_dir`: also write
        it to <coverage_dir>/<VA>/<fixture stem>.json (fixtures loaded from a file only). `purge_of(va)`: the
        bytes the original's function at `va` pops (`ret N`), or None if unknown: callee stubs that pop a
        different number are reported (Report.warnings, shown with the failures)."""
        self.m = machine
        self.purge_of = purge_of
        self.original_image = original_image
        self.coverage_dir = coverage_dir
        self.fx = fixture
        self.sides = (original, rewrite)
        self.conv = fixture.get("conv", "cdecl")
        if self.conv not in CONVS:
            raise HarnessError(f"conv must be one of {CONVS}")
        self.args = [a if isinstance(a, dict) else {"type": a} for a in fixture.get("args", [])]
        for a in self.args:
            arg_slots(a["type"])
        self.ret = fixture.get("ret", "int")
        self.callees = self._callees(fixture.get("callees", {}))
        self._static_callees = {k for k, c in self.callees.items() if not _needs_binding(k, c)}
        self.objects = fixture.get("objects", {})
        self.arg_bytes = 4 * sum(arg_slots(a["type"]) for a in self.args)
        if self.conv == "fastcall":
            regs = [a for a in self.args if arg_slots(a["type"]) == 1][:2]
            self.arg_bytes -= 4 * len(regs)
        self.expected_esp = 4 + (self.arg_bytes if CALLEE_POPS[self.conv] else 0)
        self.ignore_writes = [(_int(a), _int(b)) for a, b in fixture.get("ignore_writes", [])]
        self.max_insns = int(fixture.get("max_insns", 200000))
        self.label = fixture.get("function", f"{original.entry:#x}")
        self.warnings = self._check_purges(self.callees)

    @staticmethod
    def _callees(spec: dict) -> dict:
        out = {}
        for k, v in spec.items():
            if v is None:           # a fixture's `null` removes a default entry (see apply_defaults)
                continue
            key = callee_key(k)
            v = dict(v)
            if "ret_cstr" in v:
                raise HarnessError("ret_cstr is set up per case; use a callable ret or an object address")
            out[key] = Callee(key=key, name=v.get("name", ""), conv=v.get("conv", "cdecl"),
                              args=list(v.get("args", [])), this=v.get("this", "u32"), ret=v.get("ret", 0),
                              ret_edx=v.get("ret_edx"), ret_float=v.get("ret_float"),
                              passthrough=bool(v.get("passthrough", False)),
                              probe=bool(v.get("probe", False)), writes=list(v.get("writes", [])),
                              varargs=v.get("varargs"), noreturn=bool(v.get("noreturn", False)),
                              purge_ok=v.get("purge_ok"))
            if out[key].conv not in CONVS:
                raise HarnessError(f"callee {k}: conv must be one of {CONVS}")
            if out[key].varargs is not None and out[key].varargs not in VARARGS:
                raise HarnessError(f"callee {k}: varargs must be one of {VARARGS}")
            for t in out[key].args:
                arg_slots(t)
        return out

    def _check_purges(self, callees: dict, where: str = "") -> list:
        """Callee stubs keyed by an original VA whose pop count disagrees with that function's own `ret N`.
        Such a stub unbalances the caller's stack by the difference after every call; an ESP-framed original
        then returns to a garbage address (`fetch_unmapped` at a small address such as 0x0 or 0x5), and one
        with an EBP frame hides it. Skipped: passthrough and probe callees (the real code runs), noreturn ones,
        callees with `purge_ok`, and functions whose `ret N` the code doesn't settle."""
        if self.purge_of is None:
            return []
        out = []
        for key, c in callees.items():
            if not isinstance(key, int) or c.passthrough or c.probe or c.noreturn or c.purge_ok:
                continue
            n = self.purge_of(key)
            pop = c.pops()
            if n is None or n == pop:
                continue
            nm = f" {c.name}" if c.name else ""
            if n == 0:
                fix = "make it cdecl (the caller pops), or give it no stack args"
            elif c.conv == "cdecl":
                fix = (f"the callee pops its args: make it stdcall (thiscall if the caller loads ECX) with stack args "
                       f"adding up to {n} bytes")
            elif n > pop and n % 4 == 0:
                fix = (f"declare {(n - pop) // 4} more stack arg(s) (a by-value object the caller builds in place on "
                       "the stack, `push ecx`/`mov ecx, esp` before the call, is an arg too)")
            else:
                fix = f"declare stack args that add up to {n} bytes"
            out.append(f"{where}callee {key:#010x}{nm} is stubbed {c.conv} with {len(c.args)} arg(s), popping "
                       f"{pop} bytes, but its code returns with `ret {n}`: {fix}. A wrong pop count leaves the "
                       "caller's stack off after every call. If it never returns (exit, abort, a fatal-error "
                       "handler), mark it \"noreturn\": true; if the mismatch is deliberate, say why in "
                       "\"purge_ok\"")
        return out

    # --- case generation ---------------------------------------------------------------------------------
    def cases(self) -> list:
        """Fixed cases from the fixture, then `fuzz.count` generated ones (deterministic per seed)."""
        out = [(f"case {i}" + (f" {c['name']}" if "name" in c else ""), c)
               for i, c in enumerate(self.fx.get("cases", []))]
        fz = self.fx.get("fuzz")
        if fz:
            rng = random.Random(f"fuzz:{_seed_key(fz.get('seed', 0))}")
            # Memory fields draw from their own stream, so declaring them leaves the fuzzed args unchanged.
            mrng = random.Random(f"fuzz-memory:{_seed_key(fz.get('seed', 0))}")
            gens = fz.get("args") or [{} for _ in self.args]
            fields = fz.get("memory", [])
            for f in fields:
                self._check_field(f)
            # x87 inputs (ST0 first) likewise: their own stream.
            frng = random.Random(f"fuzz-fpu:{_seed_key(fz.get('seed', 0))}")
            fgens = fz.get("fpu") or []
            for i in range(int(fz.get("count", 0))):
                vals = [self._fuzz_value(rng, a["type"], g) for a, g in zip(self.args, gens)]
                case = {"args": vals}
                mem = [w for f in fields for w in self._fuzz_field(mrng, f)]
                if mem:
                    case["memory"] = mem
                if fgens:
                    case["fpu"] = [self._fuzz_value(frng, "double", g) for g in fgens]
                out.append((f"fuzz {i}", case))
        return out

    @staticmethod
    def _check_field(f):
        if "addr" not in f:
            raise HarnessError(f"fuzz memory entry needs `addr`: {f}")
        if "type" in f:
            if f["type"] not in MEM_SIZES:
                raise HarnessError(f"fuzz memory type must be one of {sorted(MEM_SIZES)}: {f}")
            if f["type"] == "ptr" and "choices" not in f:
                raise HarnessError(f"a fuzzed ptr field needs `choices`: {f}")
        elif "size" not in f:
            raise HarnessError(f"fuzz memory entry needs `type` (a scalar) or `size` (random bytes): {f}")

    def _fuzz_field(self, rng, f) -> list:
        """Memory entries for one declared field: `count` consecutive values of `type` (by `range`/`choices`/
        the type's generator), or `size` random bytes. With probability 1-`p` the field keeps its seeded
        value (the fixture's `objects`/`memory`)."""
        if rng.random() >= float(f.get("p", 1.0)):
            return []
        base = _int(f.get("offset", 0))
        if "type" not in f:
            return [{"addr": f["addr"], "offset": base, "hex": rng.randbytes(_int(f["size"])).hex()}]
        t, out = f["type"], []
        size = MEM_SIZES[t]
        stride = _int(f.get("stride", size))
        for k in range(int(f.get("count", 1))):
            v = self._fuzz_value(rng, t, f)
            w = {"addr": f["addr"], "offset": base + k * stride}
            if t == "float":
                w["f32"] = v
            elif t == "double":
                w["f64"] = v
            elif isinstance(v, str) and v.startswith("@"):
                if size != 4:
                    raise HarnessError(f"an object reference needs a 4-byte field: {f}")
                w["u32"] = v
            else:
                w["hex"] = (_int(v) & ((1 << (8 * size)) - 1)).to_bytes(size, "little").hex()
            out.append(w)
        return out

    @staticmethod
    def _fuzz_value(rng, t, gen):
        if "choices" in gen:
            return rng.choice(gen["choices"])
        if "range" in gen:
            if t in ("float", "double") and any(isinstance(x, float) for x in gen["range"]):
                lo, hi = (float(x) for x in gen["range"])
                return rng.uniform(lo, hi)
            lo, hi = (_int(x) for x in gen["range"])
            return rng.randint(lo, hi)
        if t in ("float", "double"):
            return rng.choice([0.0, -0.0, 1.0, -1.0, 0.5, 1e-3, 1e6, rng.uniform(-1e4, 1e4)])
        if t == "cstr":
            n = rng.choice([0, 1, 3, 8, 31, 64])
            s = "".join(chr(rng.randint(0x20, 0x7E)) for _ in range(n))
            # A case arg starting with "@" or "0x" means an object / an address (_arg_words), so a generated
            # text must not: re-draw its first character.
            while s.startswith("@") or s[:2].lower() == "0x":
                s = chr(rng.randint(0x20, 0x7E)) + s[1:]
            return s
        if t not in SCALARS or t in ("ptr", "ignore"):
            raise HarnessError(f"cannot fuzz a {t!r} argument without `choices` or `range`")
        bits, signed = SCALARS[t][1], SCALARS[t][2]
        lo, hi = (-(1 << (bits - 1)), (1 << (bits - 1)) - 1) if signed else (0, (1 << bits) - 1)
        r = rng.random()
        if r < 0.5:
            return rng.randint(max(lo, -8), min(hi, 64))           # small values: most branches live here
        if r < 0.7:
            return rng.choice([lo, hi, 0, 1, -1 if signed else hi - 1, lo + 1, hi - 1])
        return rng.randint(lo, hi)

    # --- seeding -------------------------------------------------------------------------------------------
    def _seed(self, case: dict, rng: random.Random) -> dict:
        """Write the case's memory state; return the symbol table for "@name" references."""
        m = self.m
        syms = {}
        objs = copy.deepcopy(self.objects)
        objs.update(case.get("objects", {}))
        for name, o in objs.items():
            size = _int(o.get("size", 0x100))
            va = m.alloc(size)
            syms[name] = va
            fill = o.get("fill", "random")
            data = bytearray(rng.randbytes(size) if fill == "random" else bytes(size))
            m.poke(va, bytes(data))
        for name, o in objs.items():
            for f in o.get("init", []):
                self._write_value(syms[name] + _int(f.get("offset", 0)), f, syms, rng)
        for w in list(self.fx.get("memory", [])) + list(case.get("memory", [])):
            self._write_value(self._resolve(w["addr"], syms) + _int(w.get("offset", 0)), w, syms, rng)
        return syms

    def _write_value(self, va, w, syms, rng):
        if "hex" in w:
            data = bytes.fromhex(w["hex"])
        elif "u32" in w:
            data = struct.pack("<I", self._resolve(w["u32"], syms) & 0xFFFFFFFF)
        elif "u16" in w:
            data = struct.pack("<H", _int(w["u16"]) & 0xFFFF)
        elif "u8" in w:
            data = bytes([_int(w["u8"]) & 0xFF])
        elif "f32" in w:
            data = struct.pack("<f", float(w["f32"]))
        elif "f64" in w:
            data = struct.pack("<d", float(w["f64"]))
        elif "cstr" in w:
            data = w["cstr"].encode("latin-1") + b"\0"
        elif w.get("fill") == "random":
            data = rng.randbytes(_int(w["size"]))
        elif w.get("fill") == "zero":
            data = bytes(_int(w["size"]))
        else:
            raise HarnessError(f"memory entry needs hex/u32/u16/u8/f32/f64/cstr/fill: {w}")
        self.m.poke(va, data)

    def _resolve(self, v, syms) -> int:
        if isinstance(v, str) and v.startswith("@"):
            name, _, off = v[1:].partition("+")
            if name not in syms:
                raise HarnessError(f"unknown object {name!r}")
            return syms[name] + (_int(off) if off else 0)
        return _int(v)

    def _bind_callees(self, syms, case=None) -> dict:
        """Resolve "0x.." / "@object" values in callee scripts against this case's objects. A case's own
        `callees` replace the fixture's entries with the same key (e.g. a different return value); `null`
        removes one."""
        def res(v):
            if isinstance(v, str):
                return self._resolve(v, syms)
            if isinstance(v, (list, tuple)):
                return [res(x) for x in v]
            return v
        out = {}
        own = (case or {}).get("callees", {})
        specs = dict(self.callees)
        specs.update(self._callees(own))
        for k in [k for k, v in own.items() if v is None]:     # `null`: no script for this case
            specs.pop(callee_key(k), None)
        for k, c in specs.items():
            if k in self._static_callees and c is self.callees.get(k):     # not replaced by the case
                out[k] = c                  # nothing to resolve: shared, the machine never modifies a Callee
                continue
            c = copy.copy(c)
            c.ret, c.ret_edx = res(c.ret), res(c.ret_edx)
            c.writes = [dict(w, addr=res(w["addr"])) if "addr" in w else w for w in c.writes]
            # An "@object[+off]" key is a fixture-made code address: a function pointer the fixture
            # stores in a global or a vtable (e.g. the eqgfx_dx7.dll pointers filled by GetProcAddress).
            if isinstance(k, str) and k.startswith("@"):
                k = c.key = self._resolve(k, syms)
            out[k] = c
        return out

    def _arg_words(self, t, v, syms, rng) -> list:
        if t == "cstr" and isinstance(v, str) and not v.startswith("@") and not v.lower().startswith("0x"):
            data = v.encode("latin-1") + b"\0"
            va = self.m.alloc(len(data))
            self.m.poke(va, data)
            return [va]
        if t == "float":
            return [struct.unpack("<I", struct.pack("<f", float(v)))[0]]
        if t == "double":
            return list(struct.unpack("<II", struct.pack("<d", float(v))))
        if t in ("i64", "u64"):
            v = _int(v) & (1 << 64) - 1
            return [v & 0xFFFFFFFF, v >> 32]
        v = self._resolve(v, syms)
        bits = SCALARS.get(t, (1, 32, False))[1]
        if bits < 32:
            # The caller only defines the low bits; fill the rest with garbage the rewrite must ignore.
            v = (v & ((1 << bits) - 1)) | (rng.getrandbits(32) & ~((1 << bits) - 1) & 0xFFFFFFFF)
        return [v & 0xFFFFFFFF]

    # --- running -------------------------------------------------------------------------------------------
    def run(self, stop_on_failure: bool = False, only=None, case_cover: dict | None = None) -> Report:
        """Every case on both sides. `stop_on_failure`: stop after the first failing case (mutation checks).
        `only`: run just these case indices (mutation checks: the cases that reach the mutated code; indices,
        and so the fuzz stream, stay those of the whole fixture). `case_cover`: filled with {case index: set of
        original blocks that case ran}."""
        report = Report(self.label, warnings=list(self.warnings))
        seed = _seed_key(self.fx.get("fuzz", {}).get("seed", 0))
        cover = set() if self.original_image is not None and not stop_on_failure and only is None else None
        for i, (label, case) in enumerate(self.cases()):
            if only is not None and i not in only:
                continue
            ccover = set() if case_cover is not None else None
            results = []
            rdtsc = case.get("rdtsc", self.fx.get("rdtsc"))              # deterministic RDTSC (machine.py)
            rdtsc_step = case.get("rdtsc_step", self.fx.get("rdtsc_step"))
            fpu = [_f80(v) for v in case.get("fpu", self.fx.get("fpu")) or []]
            bad = [k for k in case.get("regs", {}) if _X87_REG.fullmatch(str(k))]
            if bad:
                raise HarnessError(f"{label}: regs {bad}: x87 registers are not `regs`; put the x87 inputs in the "
                                   f"case's `fpu` list (ST0 first)")
            for side in self.sides:
                self.m.reset()
                rng = random.Random(f"case:{seed}:{i}")       # same stream for both sides
                syms = self._seed(case, rng)
                words, reg_args = [], []
                for a, v in zip(self.args, case.get("args", [])):
                    w = self._arg_words(a["type"], v, syms, rng)
                    if self.conv == "fastcall" and len(w) == 1 and len(reg_args) < 2:
                        reg_args.append(w[0])
                    else:
                        words += w
                if len(case.get("args", [])) != len(self.args):
                    raise HarnessError(f"{label}: expected {len(self.args)} args")
                regs = {k: rng.getrandbits(32) for k in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp")}
                if self.conv == "thiscall":
                    this = case.get("this", self.fx.get("this"))
                    if this is None:
                        raise HarnessError("thiscall needs `this` (in the fixture or the case)")
                    regs["ecx"] = self._resolve(this, syms)
                if reg_args:
                    regs["ecx"] = reg_args[0]
                if len(reg_args) > 1:
                    regs["edx"] = reg_args[1]
                for k, v in case.get("regs", {}).items():
                    regs[k] = self._resolve(v, syms)
                results.append(self.m.run(
                    side.entry, side.allowed, regs=regs, stack_words=words, callees=self._bind_callees(syms, case),
                    aliases=side.aliases, ret_type=self.ret, max_insns=self.max_insns,
                    arg_bytes=self.arg_bytes, ignore_writes=self.ignore_writes,
                    cover=(ccover if ccover is not None else cover) if side is self.sides[0] else None,
                    rdtsc=None if rdtsc is None else _int(rdtsc),
                    rdtsc_step=None if rdtsc_step is None else _int(rdtsc_step), fpu=fpu))
            orig, rew = results
            if ccover is not None:
                case_cover[i] = ccover
                if cover is not None:
                    cover |= ccover
            if case.get("callees"):
                own = {k: v for k, v in self._callees(case["callees"]).items()}
                for w in self._check_purges(own, f"{label}: "):
                    if w not in report.warnings:
                        report.warnings.append(w)
            report.cases.append(CaseReport(i, label, self.compare(orig, rew, case), orig, rew))
            if stop_on_failure and not report.cases[-1].ok:
                break
        self.m.reset()
        if cover is not None and report.cases:
            try:
                self._coverage(report, cover)
            except Exception as e:      # noqa: BLE001 (a measurement problem never fails the test itself;
                report.coverage = {"error": f"{type(e).__name__}: {e}"}     # the gate sees no record)
        if not stop_on_failure and report.ok and "fault_ok" not in self.fx:
            pc = _shared_fault_pc(report.cases)
            if pc is not None:
                o = report.cases[-1].original
                report.cases[-1].diffs.append(
                    f"coverage: every case ends in the same fault in shared code ({o.error}; a passed-through callee, or a "
                    f"jump to the same bad address), so "
                    f"the code after that call never runs on either side and is untested. Give the callee the "
                    f"memory it reads, or script it (ret/writes) instead of passthrough; if the fault is the "
                    f"point, add `\"fault_ok\": \"<why>\"` to the fixture")
        return report

    def _coverage(self, report: Report, cover: set) -> None:
        o, r = self.sides
        static = cov.static_of(self.original_image, o.allowed)
        path = self.fx.get("_path")
        report.coverage = cov.record(o.entry, Path(path).as_posix() if path else "", cov.measure(static, cover),
                                     len(r.allowed))
        if self.coverage_dir is not None and path:
            cov.write(self.coverage_dir, report.coverage, Path(path).stem)

    def compare(self, o: RunResult, r: RunResult, case: dict) -> list:
        d = []
        if o.error or r.error:
            if o.error and r.error and o.fault == r.fault and o.fault and o.fault[0] in ENDINGS:
                # The same CPU fault (kind + data address; the pc necessarily differs): the rewrite crashes
                # where the original does; or both end in the same noreturn callee (exit, a fatal error).
                # What happened before must still agree.
                if o.writes != r.writes:
                    d.append("writes: before the fault/noreturn call: " + _write_diff(o.writes, r.writes))
                if o.trace != r.trace:
                    d.append("trace: before the fault/noreturn call: " + _trace_diff(o.trace, r.trace))
                if (case.get("expect") or {}).get("ret") is not None:
                    d.append(f"expect: the ORIGINAL did not return ({o.error}); the fixture expects a return")
            elif not (o.error and r.error):
                d.append(f"error: original={o.error!r} rewrite={r.error!r}")
            elif o.fault != r.fault:
                d.append(f"error: original {_fault(o.fault)} vs rewrite {_fault(r.fault)} "
                         f"(original: {o.error}; rewrite: {r.error})")
            else:
                d.append(f"error: both sides: {o.error}")
            return d
        t = self.ret
        if t == "void":
            pass
        elif t == "cstr":
            if o.ret_str != r.ret_str:
                d.append(f"return: original={o.ret_str!r} rewrite={r.ret_str!r}")
        elif t in ("float", "double"):
            if o.fpu_depth != 1 or r.fpu_depth != 1:
                d.append(f"fpu: x87 stack depth original={o.fpu_depth} rewrite={r.fpu_depth} (expected 1)")
            elif not (o.st0 == r.st0 or (o.st0 != o.st0 and r.st0 != r.st0)):
                d.append(f"return: ST0 original={o.st0!r} rewrite={r.st0!r}")
        elif t in ("i64", "u64"):
            if (o.eax, o.edx) != (r.eax, r.edx):
                d.append(f"return: EDX:EAX original={o.edx:08x}:{o.eax:08x} rewrite={r.edx:08x}:{r.eax:08x}")
        elif t in INT_RETS:
            bits = INT_RETS[t]
            mask = (1 << bits) - 1
            if (o.eax & mask) != (r.eax & mask):
                d.append(f"return: EAX[{bits}] original={o.eax & mask:#x} rewrite={r.eax & mask:#x}")
        else:
            raise HarnessError(f"unknown return type {t!r}")
        if t not in ("float", "double") and (o.fpu_depth or r.fpu_depth):
            d.append(f"fpu: x87 stack left non-empty original={o.fpu_depth} rewrite={r.fpu_depth}")
        if o.esp_delta != r.esp_delta:
            d.append(f"esp: ESP delta original={o.esp_delta} rewrite={r.esp_delta} "
                     f"({self.conv} with {self.arg_bytes} arg bytes expects {self.expected_esp})")
        elif o.esp_delta != self.expected_esp:
            d.append(f"esp: both sides return with ESP delta {o.esp_delta}, but {self.conv} with "
                     f"{self.arg_bytes} arg bytes expects {self.expected_esp}: check the fixture's conv/args")
        if o.clobbered or r.clobbered:
            d.append(f"preserved: callee-saved registers changed: original={o.clobbered} rewrite={r.clobbered}")
        if o.writes != r.writes:
            d.append("writes: " + _write_diff(o.writes, r.writes))
        if o.trace != r.trace:
            d.append("trace: " + _trace_diff(o.trace, r.trace))
        exp = case.get("expect")
        if exp and "ret" in exp:
            got = o.ret_str.decode("latin-1") if t == "cstr" and o.ret_str is not None else \
                (o.st0 if t in ("float", "double") else o.eax)
            want = exp["ret"]
            if t in INT_RETS:
                bits = INT_RETS[t]
                got = sx(got, bits) if isinstance(want, int) and want < 0 else got & ((1 << bits) - 1)
                want = _int(want)
            elif t in ("i64", "u64"):              # EDX:EAX, not EAX alone
                got, want = (o.edx << 32) | o.eax, _int(want)
                if want < 0:
                    got = sx(got, 64)
            if got != want:
                d.append(f"expect: the ORIGINAL returned {got!r}, the fixture expects {want!r}")
        return d


def _write_diff(a: dict, b: dict) -> str:
    keys = sorted(set(a) | set(b))
    bad = [k for k in keys if a.get(k) != b.get(k)]
    shown = ", ".join(f"{k:#x}: {_b(a.get(k))} vs {_b(b.get(k))}" for k in bad[:8])
    more = f" (+{len(bad) - 8} more)" if len(bad) > 8 else ""
    return f"{len(bad)} byte(s) differ (original vs rewrite; '--' = not written): {shown}{more}"


_PC = re.compile(r"\bpc (0x[0-9a-fA-F]+)")


def _shared_fault_pc(cases):
    """The pc when every case faults at the same pc on both sides, else None. The rewrite runs from the test
    DLL, so a pc both sides share is in the original image: a passed-through callee faulted, and nothing after
    that call ran (seen on 0x00450C79's ctor: a store after loadOverlay)."""
    pcs = set()
    for c in cases:
        o, r = c.original, c.rewrite
        if not (o.fault and o.fault == r.fault and o.fault[0] in CPU_FAULTS):
            return None
        a, b = _PC.search(o.error or ""), _PC.search(r.error or "")
        if not (a and b and a.group(1) == b.group(1)):
            return None
        pcs.add(a.group(1))
    return pcs.pop() if len(pcs) == 1 else None


def _fault(f):
    if f is None:
        return "no fault"
    kind, addr = f
    return kind if addr is None else (f"{kind} at {addr:#x}" if isinstance(addr, int) else f"{kind} {addr}")


def _b(v):
    return "--" if v is None else f"{v:02x}"


def _trace_diff(a: list, b: list) -> str:
    for i, (x, y) in enumerate(zip(a, b)):
        if x != y:
            return f"call #{i} original: {x} | rewrite: {y}"
    longer, side = (a, "original") if len(a) > len(b) else (b, "rewrite")
    return f"{side} makes {len(longer) - min(len(a), len(b))} extra call(s), first: {longer[min(len(a), len(b))]}"


# --- harness: images + build + fixture -> DiffTest ----------------------------------------------------------

class Harness:
    """Per-client entry point. Holds the original image; builds and loads rewrite DLLs on demand."""

    def __init__(self, original_exe, build_dir, include_dirs=(), client="TRILOGY", function_starts=(),
                 defines=(), vc6=None, fixture_defaults=None, cache_dir=None):
        """`fixture_defaults`: a fixture fragment merged under every fixture (apply_defaults), e.g. the
        client's CRT `callees` (passthrough) and `rewrite_aliases` (test DLL CRT symbol -> client CRT VA).
        `cache_dir`: the test-DLL cache (vc6.build_dll; None = the default, False = none)."""
        self.cache_dir = cache_dir
        self.original = load_pe(original_exe)
        self.fixture_defaults = fixture_defaults or {}
        self.vc6 = vc6
        self.build_dir = Path(build_dir)
        self.include_dirs = [Path(d) for d in include_dirs]
        self.client = client
        self.function_starts = frozenset(function_starts)
        self.defines = list(defines)

    def rewrite_image(self, name, sources, defines=()):
        dll, mapf = build_dll(name, sources, self.build_dir, include_dirs=self.include_dirs,
                              defines=self.defines + list(defines), vc6=self.vc6, cache_dir=self.cache_dir)
        img = load_pe(dll, base=REWRITE_BASE, name=f"{name}.dll")
        read_map(mapf, img)
        return img

    def difftest(self, fixture: dict, sources, name: str | None = None, rewrite_symbol: str | None = None,
                 base_dir=None, defines=()) -> DiffTest:
        """`sources`: the C++ files to build into the test DLL (relative to `base_dir`); `defines`: extra -D."""
        own_aliases = set(fixture.get("rewrite_aliases") or {})
        fixture = resolve_image_refs(fixture, self.original)
        fixture = apply_defaults(fixture, self.fixture_defaults)
        base_dir = Path(base_dir or ".")
        srcs = [Path(s) if Path(s).is_absolute() else base_dir / s for s in sources]
        va = _int(fixture["original"])
        name = name or f"t{va:08x}"
        rimg = self.rewrite_image(name, srcs, defines)
        anns = ann.scan(srcs, self.client)

        # Rewrite entry: an explicit symbol, else the function annotated with the original's address.
        sym = rewrite_symbol or fixture.get("rewrite")
        if sym is None:
            hits = [a.name for a in anns if a.va == va]
            if len(hits) != 1:
                raise HarnessError(f"{len(hits)} FUNCTION annotations for {va:#x} in {sources}")
            sym = hits[0]
        entry = _find_symbol(rimg, sym)

        # Rewrite-side calls into other annotated rewrites count as calls to their original addresses;
        # fixture aliases cover the rest (e.g. the DLL's own CRT -> the client's CRT).
        aliases = {}
        for a in anns:
            if a.va != va:
                try:
                    aliases[_find_symbol(rimg, a.name)] = a.va
                except HarnessError:
                    pass
        for s, tgt in fixture.get("rewrite_aliases", {}).items():
            if tgt is None:
                continue
            try:
                sva = _find_symbol(rimg, s)
            except HarnessError:
                if s in own_aliases:
                    raise
                continue                    # a default alias for a CRT function this DLL doesn't link
            aliases[sva] = _int(tgt) if str(tgt).lower().startswith("0x") else tgt
        ext_marks = externs.scan(srcs) + externs.header_markers(self.include_dirs)
        for sva, s in rimg.symbols.items():                 # unaliased DLL symbols: keyed by name
            if sva not in aliases and sva != entry:
                # EQ_METHOD's generated `jmp va` stubs (thiscall_stubs.py) are calls to the client method,
                # and so are the stubs of functions declared under an `// EXTERNAL:` marker (externs.py)
                tc = thiscall_stubs.stub_va(s)
                if tc is None and externs.marked(s, ext_marks):
                    tc = externs.stub_target(rimg.read, sva)
                aliases[sva] = s if tc is None else tc

        size = fixture.get("size")
        o_allowed = function_extent(self.original.read, va, self.function_starts - {va},
                                    size=_int(size) if size else None, in_image=self.original.contains)
        r_stops = frozenset(rimg.symbols) - {entry}
        r_allowed = function_extent(rimg.read, entry, r_stops, in_image=rimg.contains)
        # Members of classes local to a function (`??1Cleanup@?1??Func@@...`: EH cleanup helpers the
        # original inlines) are part of the rewrite's body, not callees.
        # ... and so are the fixture's `rewrite_inline` symbols (e.g. a file-scope cleanup class).
        inline = set(fixture.get("rewrite_inline", []))
        for sva, s in rimg.symbols.items():
            if sva != entry and (LOCAL_CLASS.search(s) or s in inline):
                r_allowed |= function_extent(rimg.read, sva, r_stops - {sva}, in_image=rimg.contains)
                aliases.pop(sva, None)
        m = Machine([self.original, rimg])
        # Coverage of the original side (coverage.py) is always measured; EQD_COVERAGE=0 turns it off.
        on = os.environ.get("EQD_COVERAGE", "1") != "0"
        return DiffTest(m, fixture,
                        Side("original", va, o_allowed, {}),
                        Side("rewrite", entry, r_allowed, aliases),
                        original_image=self.original if on else None,
                        coverage_dir=self.build_dir / "coverage" if on else None,
                        purge_of=self.purge_of)

    def purge_of(self, va: int) -> int | None:
        """Bytes the original's function at `va` pops on return (`ret N`, extent.stack_purge), cached; None
        outside the original image or when its code doesn't settle it."""
        cache = self.__dict__.setdefault("_purges", {})
        if va not in cache:
            cache[va] = (stack_purge(self.original.read, va, self.function_starts, self.original.contains)
                         if self.original.contains(va) else None)
        return cache[va]


def _needs_binding(key, c) -> bool:
    """True if a callee script has "@object" / "0x.." strings that _bind_callees resolves per case."""
    def has_str(v):
        return isinstance(v, str) or (isinstance(v, (list, tuple)) and any(has_str(x) for x in v))
    return (isinstance(key, str) and key.startswith("@")) or has_str(c.ret) or has_str(c.ret_edx)         or any("addr" in w for w in c.writes)


def _find_symbol(img: Image, sym: str) -> int:
    if sym in img.by_name:
        return img.by_name[sym]
    # Undecorated names, indexed once per image (rebuilt if symbols were added since).
    idx = img.__dict__.get("_undecorated")
    if idx is None or idx[0] != len(img.symbols):
        d: dict = {}
        for va, s in img.symbols.items():
            d.setdefault(undecorate(s), []).append(va)
        idx = img.__dict__["_undecorated"] = (len(img.symbols), d)
    hits = list(idx[1].get(sym, ()))
    if len(hits) == 1:
        return hits[0]
    if not hits:
        raise HarnessError(f"symbol {sym!r} not found in {img.name}")
    raise HarnessError(f"symbol {sym!r} is ambiguous in {img.name} (overloads?); use the decorated name")
