"""The emulated machine: images mapped at their VAs, a stack, a heap for fixture objects, a TEB, import stubs,
and the call interception that replaces every callee with a recorded, scripted stub.

One Machine runs both sides of a differential test (the original and the rewrite are both mapped). Between
runs, `reset()` restores every page that was written since the last reset, so each run starts from the same
snapshot.
"""
from __future__ import annotations

import math
import struct
from dataclasses import dataclass, field
from fractions import Fraction

from unicorn import (UC_ARCH_X86, UC_HOOK_BLOCK, UC_HOOK_CODE, UC_HOOK_INTR, UC_HOOK_MEM_UNMAPPED, UC_HOOK_MEM_WRITE,
                     UC_MODE_32, UC_PROT_ALL, Uc, UcError)
from unicorn import unicorn_const as UC
from unicorn import x86_const as R

from capstone import CS_ARCH_X86, CS_MODE_32, Cs
from capstone.x86_const import X86_OP_IMM as CS_OP_IMM

from .pe import PAGE, Image, page_up

_MD = Cs(CS_ARCH_X86, CS_MODE_32)
_MD.detail = True

# Fixed layout (all outside both images; the rewrite DLL is loaded at REWRITE_BASE, see difftest).
HEAP_BASE, HEAP_SIZE = 0x60000000, 0x00400000
STUB_BASE, STUB_SIZE = 0x7E000000, 0x00100000
INIT_STUB = STUB_BASE + STUB_SIZE - 0x3000      # fninit; fldcw
FPU_IN_STUB = INIT_STUB + 0x20                  # fld tbyte [FPU_IN_SLOT]   (a case's x87 inputs, `fpu`)
FPU_IN_SLOT = INIT_STUB + 0x110
FLD_STUB = STUB_BASE + STUB_SIZE - 0x2000       # fld qword [slot]; ret N   (float-returning callees)
SENTINEL = STUB_BASE + STUB_SIZE - 0x1000       # return address of the function under test
STACK_BASE, STACK_SIZE = 0x7F000000, 0x00100000
ESP0 = STACK_BASE + STACK_SIZE - 0x10000        # ESP at entry (points at SENTINEL)
TEB_BASE, GDT_BASE = 0x7FFD0000, 0x7FFE0000
FPU_CW = 0x027F                                 # Win32 default: 53-bit precision, all exceptions masked

# RDTSC (0F 31) is deterministic: each execution returns the next value of a 64-bit counter that starts at
# RDTSC_START and advances by RDTSC_STEP, reset at the start of every run (so the original and the rewrite see
# the same sequence). A fixture or case overrides both with "rdtsc" / "rdtsc_step".
RDTSC_START, RDTSC_STEP = 0x0000_0010_0000_0000, 0x1000

JUNK_ECX, JUNK_EDX = 0xC1C1C1C1, 0xD1D1D1D1     # volatile registers after a stubbed call

CALLEE_POPS = {"cdecl": False, "stdcall": True, "thiscall": True, "fastcall": True}

# Fault kinds. A CPU fault is behavior: when both sides fault with the same kind at the same data address, the
# runs agree (the pc necessarily differs). The other kinds (unscripted, timeout, harness, emulation) are
# harness/fixture problems and always fail.
CPU_FAULTS = ("read_unmapped", "write_unmapped", "fetch_unmapped", "div0", "invalid_insn", "int3", "interrupt")
# A call to a callee marked `noreturn` (exit, abort, a fatal-error handler) ends the run like a CPU fault: the
# runs agree when both sides make the same calls and writes and end in the same noreturn callee.
ENDINGS = CPU_FAULTS + ("noreturn",)
_UNMAPPED = {UC.UC_MEM_READ_UNMAPPED: "read_unmapped", UC.UC_MEM_WRITE_UNMAPPED: "write_unmapped",
             UC.UC_MEM_FETCH_UNMAPPED: "fetch_unmapped"}
_INTERRUPTS = {0: "div0", 3: "int3", 6: "invalid_insn"}
VARARGS = ("printf", "stack")


class HarnessError(RuntimeError):
    """A fixture or harness problem (not a behavior difference)."""


# --- argument types ---------------------------------------------------------------------------------------
# type -> (stack slots, mask of meaningful bits, signed). Sub-dword args only define their low bits; the
# caller leaves garbage above them, so recorded values and fuzzed inputs use only the meaningful bits.
SCALARS = {
    "int": (1, 32, True), "i32": (1, 32, True), "long": (1, 32, True),
    "u32": (1, 32, False), "uint": (1, 32, False), "dword": (1, 32, False), "ptr": (1, 32, False),
    "short": (1, 16, True), "i16": (1, 16, True), "u16": (1, 16, False), "ushort": (1, 16, False),
    "char": (1, 8, True), "i8": (1, 8, True), "u8": (1, 8, False), "uchar": (1, 8, False),
    "bool": (1, 8, False),
    "float": (1, 32, False), "double": (2, 64, False), "i64": (2, 64, True), "u64": (2, 64, False),
    "cstr": (1, 32, False), "ignore": (1, 32, False),
}


def arg_slots(t: str) -> int:
    if t.startswith("mem:"):
        return 1
    if t not in SCALARS:
        raise HarnessError(f"unknown argument type {t!r}")
    return SCALARS[t][0]


def sx(v: int, bits: int) -> int:
    v &= (1 << bits) - 1
    return v - (1 << bits) if v >> (bits - 1) else v


def f80_to_float(mant: int, exp_sign: int) -> float:
    sign = -1.0 if exp_sign & 0x8000 else 1.0
    e = exp_sign & 0x7FFF
    if e == 0 and mant == 0:
        return 0.0 * sign
    if e == 0x7FFF:
        return sign * float("inf") if (mant << 1) & ((1 << 64) - 1) == 0 else float("nan")
    # math.ldexp, not mant * 2.0 ** k: 2.0 ** k underflows to 0 for k < -1074 although mant * 2**k may still
    # be a (subnormal) double. Below the normal range, Fraction rounds once (ldexp(float(mant)) would twice).
    k = max(e, 1) - 16383 - 63
    if k + 63 < -1022:
        return sign * float(Fraction(mant) * Fraction(2) ** k)
    return sign * math.ldexp(mant, k)


def float_to_f80(v: float) -> bytes:
    """The 10 bytes of the x87 extended value equal to the double `v` (exact: every double is an f80)."""
    (bits,) = struct.unpack("<Q", struct.pack("<d", float(v)))
    return f64_bits_to_f80(bits)


def f64_bits_to_f80(bits: int) -> bytes:
    sign, e, f = bits >> 63, (bits >> 52) & 0x7FF, bits & ((1 << 52) - 1)
    if e == 0x7FF:                              # inf / NaN (payload and quiet bit kept)
        mant, ex = (1 << 63) | (f << 11), 0x7FFF
    elif e == 0 and f == 0:
        mant, ex = 0, 0
    elif e == 0:                                # subnormal double: normal in f80
        n = f.bit_length()
        mant, ex = f << (64 - n), 15308 + n
    else:
        mant, ex = (1 << 63) | (f << 11), e - 1023 + 16383
    return struct.pack("<QH", mant, (sign << 15) | ex)


def _gdt_entry(base: int, limit: int, access: int, flags: int) -> int:
    return (limit & 0xFFFF) | ((base & 0xFFFFFF) << 16) | (access << 40) | (((limit >> 16) & 0xF) << 48)         | (flags << 52) | ((base >> 24) << 56)


@dataclass
class Callee:
    """How to stand in for one callee. `ret` may be an int, a list (one per call, the last repeats) or a
    callable(call_index, Call) -> int."""
    key: object
    name: str = ""
    conv: str = "cdecl"
    args: list = field(default_factory=list)
    this: str = "u32"
    ret: object = 0
    ret_edx: object = None
    ret_float: object = None
    passthrough: bool = False
    probe: bool = False                          # VC6 _chkstk/__alloca_probe (see _intercept)
    writes: list = field(default_factory=list)   # [{"arg": i | "addr": va, "offset": n, "hex": "..", "call": k}]
    varargs: str | None = None                   # "printf": the last fixed arg is a printf format that
                                                 # types the rest; "stack": count from the caller's cleanup
    noreturn: bool = False                       # the call ends the run (exit, abort, fatal error)
    purge_ok: str | None = None                  # why the pop count may differ from the callee's `ret N`

    def pops(self) -> int:
        """Stack bytes the stub pops on return (the caller's args for stdcall/thiscall/fastcall)."""
        slots = self.slots()
        if self.conv == "fastcall":
            slots -= len([t for t in self.args if arg_slots(t) == 1][:2])
        return 4 * slots if CALLEE_POPS[self.conv] else 0

    def slots(self) -> int:
        return sum(arg_slots(t) for t in self.args)


@dataclass(frozen=True)
class Call:
    target: object          # original VA (int) or "dll!name" or an unaliased rewrite symbol name
    name: str
    this: object
    args: tuple

    def __str__(self):
        t = f"{self.target:#010x}" if isinstance(self.target, int) else str(self.target)
        n = f" {self.name}" if self.name else ""
        th = f" this={self.this}" if self.this is not None else ""
        return f"{t}{n}{th} args={list(self.args)}"


@dataclass
class RunResult:
    eax: int = 0
    edx: int = 0
    st0: float | None = None
    fpu_depth: int = 0
    esp_delta: int = 0
    clobbered: dict = field(default_factory=dict)     # callee-saved reg -> (before, after)
    writes: dict = field(default_factory=dict)        # va -> final byte
    trace: list = field(default_factory=list)
    ret_str: bytes | None = None
    error: str | None = None                          # human-readable, includes the pc
    fault: tuple | None = None                        # (kind, data address or None): what is compared


@dataclass
class _RunState:
    allowed: set
    callees: dict
    aliases: dict
    max_insns: int
    trace: list = field(default_factory=list)
    counts: dict = field(default_factory=dict)
    written: set = field(default_factory=set)
    passthrough: list = field(default_factory=list)  # [(return address, esp at the call)]
    cover: set | None = None                         # (va, size) of executed blocks starting in `allowed`
    last_block: tuple | None = None                  # (va, size) of the last block entered (fault hints)
    last_stub: tuple | None = None                   # (key, name, bytes popped) of the last stubbed call
    error: str | None = None
    fault: tuple | None = None


class Machine:
    def __init__(self, images: list[Image]):
        self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        self.images = images
        self.regions = []                               # (base, size, label)
        for img in images:
            self._map(img.base, len(img.data), img.name)
            self.uc.mem_write(img.base, bytes(img.data))
        self._map(STUB_BASE, STUB_SIZE, "stubs")
        self.uc.mem_write(STUB_BASE, b"\xCC" * STUB_SIZE)
        self._map(HEAP_BASE, HEAP_SIZE, "heap")
        self._map(STACK_BASE, STACK_SIZE, "stack")
        self._map(TEB_BASE, PAGE, "teb")
        self._map(GDT_BASE, PAGE, "gdt")

        # Import stubs: one address per distinct "dll!name", shared by every image, so the original and the
        # rewrite see the same callee for the same import.
        self.stub_names: dict = {}
        by_name: dict = {}
        for img in images:
            for slot, nm in sorted(img.imports.items()):
                if nm not in by_name:
                    va = STUB_BASE + 16 * len(by_name)
                    if va >= INIT_STUB:
                        raise HarnessError("too many imports for the stub region")
                    by_name[nm] = va
                    self.stub_names[va] = nm
                self.uc.mem_write(slot, struct.pack("<I", by_name[nm]))
        self.uc.mem_write(INIT_STUB, b"\xDB\xE3" + b"\xD9\x2D" + struct.pack("<I", INIT_STUB + 0x100) + b"\x90")
        self.uc.mem_write(INIT_STUB + 0x100, struct.pack("<H", FPU_CW))
        self.uc.mem_write(FPU_IN_STUB, b"\xDB\x2D" + struct.pack("<I", FPU_IN_SLOT))
        self.uc.mem_write(SENTINEL, b"\xF4")            # hlt (never executed: emu_start stops on it)

        # TEB through FS (GDT entry 5). fs:[0] = end of the SEH chain, fs:[0x18] = TEB itself.
        teb = bytearray(PAGE)
        struct.pack_into("<I", teb, 0x00, 0xFFFFFFFF)
        struct.pack_into("<I", teb, 0x04, STACK_BASE + STACK_SIZE)   # stack base (top)
        struct.pack_into("<I", teb, 0x08, STACK_BASE)                # stack limit
        struct.pack_into("<I", teb, 0x18, TEB_BASE)
        self.uc.mem_write(TEB_BASE, bytes(teb))
        # Loading FS from the GDT invalidates Unicorn's default SS, so DS/ES/SS get a flat 4 GB data
        # descriptor (entry 6) too.
        self.uc.mem_write(GDT_BASE + 5 * 8, struct.pack("<Q", _gdt_entry(TEB_BASE, PAGE - 1, 0x93, 0x4)))
        self.uc.mem_write(GDT_BASE + 6 * 8, struct.pack("<Q", _gdt_entry(0, 0xFFFFF, 0x93, 0xC)))
        self.uc.reg_write(R.UC_X86_REG_GDTR, (0, GDT_BASE, PAGE - 1, 0))
        for seg in (R.UC_X86_REG_SS, R.UC_X86_REG_DS, R.UC_X86_REG_ES):
            self.uc.reg_write(seg, 6 << 3)
        self.uc.reg_write(R.UC_X86_REG_FS, 5 << 3)

        self.pristine = {b: bytes(self.uc.mem_read(b, s)) for b, s, _ in self.regions}
        # CPU state is restored before every run too: after a CPU exception (e.g. #DE) Unicorn keeps it
        # pending, and the next run's exception would turn into a double fault.
        self.context = self.uc.context_save()
        self.dirty: set = set()
        self.heap_top = HEAP_BASE
        self.state: _RunState | None = None
        self.uc.hook_add(UC_HOOK_BLOCK, self._on_block)
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._on_write)
        self.uc.hook_add(UC_HOOK_MEM_UNMAPPED, self._on_unmapped)
        self.uc.hook_add(UC_HOOK_INTR, self._on_interrupt)
        self._rdtsc_next, self._rdtsc_step = RDTSC_START, RDTSC_STEP
        self._hook_rdtsc()

    def _hook_rdtsc(self):
        """Unicorn has no instruction hook for RDTSC. A code hook bounded to each `0F 31` in the images' code
        is only consulted for that address, so every other instruction runs at full speed; a byte pair that
        isn't an instruction start never fires."""
        for img in self.images:
            for base, size in (img.code_ranges or [(img.base, len(img.data))]):
                code, i = bytes(img.data[base - img.base:base - img.base + size]), 0
                while (i := code.find(b"\x0f1", i)) >= 0:
                    self.uc.hook_add(UC_HOOK_CODE, self._on_rdtsc, begin=base + i, end=base + i)
                    i += 1

    def _on_rdtsc(self, uc, va, size, _):
        if bytes(uc.mem_read(va, 2)) != b"\x0f1":
            return
        v = self._rdtsc_next
        self._rdtsc_next = (v + self._rdtsc_step) & 0xFFFFFFFFFFFFFFFF
        uc.reg_write(R.UC_X86_REG_EAX, v & 0xFFFFFFFF)
        uc.reg_write(R.UC_X86_REG_EDX, v >> 32)
        uc.reg_write(R.UC_X86_REG_EIP, va + 2)          # skip the instruction (the host clock never shows)

    # --- memory ------------------------------------------------------------------------------------------
    def _map(self, base, size, label):
        size = page_up(size)
        for b, s, lb in self.regions:
            if base < b + s and b < base + size:
                raise HarnessError(f"{label} [{base:#x}+{size:#x}] overlaps {lb}")
        self.uc.mem_map(base, size, UC_PROT_ALL)
        self.regions.append((base, size, label))

    def region_of(self, va):
        for b, s, lb in self.regions:
            if b <= va < b + s:
                return lb
        return None

    def poke(self, va: int, data: bytes):
        """Host-side write (fixture seeding, stub side effects). Tracked for reset()."""
        self.uc.mem_write(va, bytes(data))
        for p in range(va & ~(PAGE - 1), va + len(data), PAGE):
            self.dirty.add(p)

    def peek(self, va: int, n: int) -> bytes:
        return bytes(self.uc.mem_read(va, n))

    def u32(self, va: int) -> int:
        return struct.unpack("<I", self.peek(va, 4))[0]

    def cstr(self, va: int, limit: int = 4096) -> bytes | None:
        if va == 0:
            return None
        out = bytearray()
        try:
            while len(out) < limit:
                chunk = self.peek(va + len(out), 64)
                i = chunk.find(b"\0")
                if i >= 0:
                    return bytes(out + chunk[:i])
                out += chunk
        except UcError:
            return bytes(out) + b"<unmapped>"
        return bytes(out) + b"<...>"

    def alloc(self, size: int, align: int = 16) -> int:
        """Bump allocation in the fixture heap. reset(heap=True) frees everything."""
        va = (self.heap_top + align - 1) & ~(align - 1)
        if va + size > HEAP_BASE + HEAP_SIZE:
            raise HarnessError("fixture heap exhausted")
        self.heap_top = va + max(size, 1)
        return va

    def reset(self, heap: bool = True):
        for p in self.dirty:
            for b, s, _ in self.regions:
                if b <= p < b + s:
                    self.uc.mem_write(p, self.pristine[b][p - b:p - b + PAGE])
                    break
        self.dirty.clear()
        if heap:
            self.heap_top = HEAP_BASE

    # --- hooks -------------------------------------------------------------------------------------------
    def _on_write(self, uc, access, va, size, value, _):
        st = self.state
        for p in range(va & ~(PAGE - 1), va + size, PAGE):
            self.dirty.add(p)
        if st is not None:
            st.written.update(range(va, va + size))

    def _on_unmapped(self, uc, access, va, size, value, _):
        st = self.state
        if st is not None and st.error is None:
            pc = uc.reg_read(R.UC_X86_REG_EIP)
            kind = _UNMAPPED.get(access, "read_unmapped")
            st.error = f"{kind} at {va:#x} (size {size}, pc {pc:#x})"
            if kind == "fetch_unmapped":
                st.error += self._fetch_hint(va)
            st.fault = (kind, va)
        return False

    def _on_interrupt(self, uc, intno, _):
        st = self.state
        if st is not None and st.error is None:
            kind = _INTERRUPTS.get(intno, "interrupt")
            pc = uc.reg_read(R.UC_X86_REG_EIP)
            st.error = f"{kind} (interrupt {intno}, pc {pc:#x})"
            st.fault = (kind, intno if kind == "interrupt" else None)
        # A faulting instruction would restart after the hook: leave through the sentinel.
        uc.reg_write(R.UC_X86_REG_EIP, SENTINEL)
        uc.emu_stop()

    def _fail(self, msg, kind="harness", addr=None):
        st = self.state
        if st.error is None:
            st.error = msg
            st.fault = (kind, addr)
        self.uc.emu_stop()

    def _on_block(self, uc, va, size, _):
        st = self.state
        if st is None or st.error is not None:
            return
        st.last_block = (va, size)
        cover = st.cover
        if cover is not None and va in st.allowed:
            # Coverage (coverage.py): own-code blocks, also those reached on return from a passthrough. A set,
            # never a list: a sort loop runs millions of blocks (a list grew to 7 GB on DoTrackSort).
            cover.add((va, size))
        if va == SENTINEL or FLD_STUB <= va < FLD_STUB + 0x100:
            return
        is_stub = STUB_BASE <= va < INIT_STUB
        if st.passthrough:
            ret, esp_call = st.passthrough[-1]
            # Back at the return address: normally ESP is above the call, but VC6 __EH_prolog returns with
            # its EH frame pushed (ESP 12 below; until a later fix that passthrough never ended and nothing
            # after it was intercepted). A deeper ESP means the caller was re-entered: not this return.
            if va == ret and uc.reg_read(R.UC_X86_REG_ESP) > esp_call - 0x40:
                st.passthrough.pop()
                return
            if not is_stub:
                return
        elif va in st.allowed:
            return
        self._intercept(va, is_stub)

    def _fetch_hint(self, target) -> str:
        """Why execution reached unmapped code, from the last instruction that ran (the end of the last block):
        a `ret` popped a bad return address (the stack was unbalanced: a stub popped the wrong number of bytes,
        or a callee that never returns was stubbed as returning), or an indirect call/jump read its target from
        memory the fixture didn't set (a function pointer or vtable slot)."""
        st = self.state
        last = None
        if st.last_block is not None:
            va, size = st.last_block
            try:
                for insn in _MD.disasm(self.peek(va, size), va):
                    last = insn
            except UcError:
                last = None
        stub = ""
        if st.last_stub is not None:
            k, nm, pop = st.last_stub
            k = f"{k:#010x}" if isinstance(k, int) else k
            stub = f"; the last stubbed call was {k}{' ' + nm if nm else ''}, which popped {pop} bytes"
        if last is not None and last.mnemonic == "ret":
            return (f"; hint: `{(last.mnemonic + ' ' + last.op_str).strip()}` at {last.address:#x} returned to "
                    f"{target:#x}: the stack was unbalanced when the function returned. Usually a callee stub pops "
                    "the wrong number of bytes (its conv/args; a by-value object the caller builds on the stack "
                    "with `push ecx`/`mov ecx, esp` before the call is an arg too, and a thiscall callee that "
                    "ends in `ret N` pops it), or a callee that never returns (exit, abort, a fatal-error "
                    f"handler) is stubbed as returning: mark it \"noreturn\": true{stub}")
        if last is not None and last.mnemonic in ("call", "jmp") and last.operands \
                and last.operands[0].type != CS_OP_IMM:
            return (f"; hint: `{last.mnemonic} {last.op_str}` at {last.address:#x} took its target {target:#x} "
                    "from memory or a register the fixture did not set: a function pointer or vtable slot. Store "
                    "an \"@fn\" object's address there (memory / objects init) and key a callee \"@fn\", or "
                    "give the object its vtable")
        if target < 0x10000:
            return (f"; hint: execution reached {target:#x}: a bad return address (a stub that pops the wrong "
                    f"number of bytes, or a noreturn callee stubbed as returning) or a null function pointer{stub}")
        return ""

    def _read_arg(self, t, words):
        if t == "ignore":
            return None
        if t == "cstr":
            s = self.cstr(words[0])
            return None if s is None else s.decode("latin-1")
        if t.startswith("mem:"):
            n = int(t[4:], 0)
            if words[0] == 0:
                return None
            try:
                return self.peek(words[0], n).hex()
            except UcError:
                return f"<unmapped {words[0]:#x}>"
        # NaN != NaN, so a NaN argument would never compare equal in a trace: record NaNs by their bits.
        if t == "float":
            v = struct.unpack("<f", struct.pack("<I", words[0]))[0]
            return f"nan:{words[0]:08x}" if v != v else v
        if t == "double":
            v = struct.unpack("<d", struct.pack("<II", *words))[0]
            return f"nan:{words[1]:08x}{words[0]:08x}" if v != v else v
        slots, bits, signed = SCALARS[t]
        v = words[0] | (words[1] << 32 if slots == 2 else 0)
        v &= (1 << bits) - 1
        return sx(v, bits) if signed else v

    def _intercept(self, va, is_stub):
        st, uc = self.state, self.uc
        key = self.stub_names.get(va) if is_stub else st.aliases.get(va, va)
        spec = st.callees.get(key)
        if spec is None:
            where = self.region_of(va)
            k = f"{key:#010x}" if isinstance(key, int) else key
            return self._fail(f"call to unscripted target {k} (in {where}); add it to the fixture's callees",
                              "unscripted", key)
        esp = uc.reg_read(R.UC_X86_REG_ESP)
        ret = self.u32(esp)
        if spec.probe:
            # VC6 _chkstk (__alloca_probe): EAX = frame size; returns with ESP lowered by it below the
            # caller's ESP, ECX kept and EAX = the return address. A passthrough never sees it return (ESP
            # ends below the call) and a stub leaves the frame unreserved. Not traced: the frame sizes of
            # the two sides may differ, or only one side may need a probe.
            new_esp = (esp + 4 - uc.reg_read(R.UC_X86_REG_EAX)) & 0xFFFFFFFF
            if not STACK_BASE + PAGE <= new_esp <= esp:
                return self._fail(f"{spec.name or key}: probe of {uc.reg_read(R.UC_X86_REG_EAX):#x} bytes "
                                  "overflows the stack")
            uc.reg_write(R.UC_X86_REG_EAX, ret)
            uc.reg_write(R.UC_X86_REG_ESP, new_esp)
            uc.reg_write(R.UC_X86_REG_EIP, ret)
            return
        ecx, edx = uc.reg_read(R.UC_X86_REG_ECX), uc.reg_read(R.UC_X86_REG_EDX)
        # Gather argument words: fastcall's first two dword args come from ECX/EDX.
        reg_words = [ecx, edx] if spec.conv == "fastcall" else []
        sp = esp + 4
        vals, stack_slots = [], 0
        for t in spec.args:
            n = arg_slots(t)
            if n == 1 and reg_words:
                words = [reg_words.pop(0)]
            else:
                reg_words = [] if n > 1 else reg_words
                words = [self.u32(sp + 4 * i) for i in range(n)]
                sp += 4 * n
                stack_slots += n
            vals.append(self._read_arg(t, words))
        if spec.varargs:
            extra = self._varargs(spec, vals, ret, stack_slots)
            if extra is None:
                return
            for t in extra:
                n = arg_slots(t)
                vals.append(self._read_arg(t, [self.u32(sp + 4 * i) for i in range(n)]))
                sp += 4 * n
        this = self._read_arg(spec.this, [ecx]) if spec.conv == "thiscall" else None
        call = Call(key, spec.name, this, tuple(vals))
        st.trace.append(call)
        idx = st.counts.get(key, 0)
        st.counts[key] = idx + 1
        if spec.passthrough:
            if is_stub:
                return self._fail(f"{key}: an import cannot be passed through")
            st.passthrough.append((ret, esp))
            return
        for w in spec.writes:
            # "call": N (or [N, ...]): only on that call of this callee in the run (0 = the first)
            when = w.get("call")
            if when is not None and idx not in (when if isinstance(when, (list, tuple)) else (when,)):
                continue
            if "arg" in w:
                base = self.u32(esp + 4 + 4 * w["arg"])
            else:
                base = int(w["addr"], 0) if isinstance(w["addr"], str) else w["addr"]
            dst = base + int(w.get("offset", 0))
            data = bytes.fromhex(w["hex"])
            self.poke(dst, data)
            st.written.update(range(dst, dst + len(data)))
        if spec.noreturn:
            k = f"{key:#010x}" if isinstance(key, int) else key
            return self._fail(f"noreturn call to {k}{' ' + spec.name if spec.name else ''}", "noreturn", key)
        pop = 4 * stack_slots if CALLEE_POPS[spec.conv] else 0
        st.last_stub = (key, spec.name, pop)

        def pick(v):
            if callable(v):
                return v(idx, call)
            if isinstance(v, (list, tuple)):
                return v[min(idx, len(v) - 1)]
            return v
        uc.reg_write(R.UC_X86_REG_EAX, int(pick(spec.ret) or 0) & 0xFFFFFFFF)
        edx_v = pick(spec.ret_edx)
        uc.reg_write(R.UC_X86_REG_EDX, JUNK_EDX if edx_v is None else int(edx_v) & 0xFFFFFFFF)
        uc.reg_write(R.UC_X86_REG_ECX, JUNK_ECX)
        fv = pick(spec.ret_float)
        if fv is not None:
            slot = FLD_STUB + 0x80
            self.poke(slot, struct.pack("<d", float(fv)))
            self.poke(FLD_STUB, b"\xDD\x05" + struct.pack("<I", slot) + b"\xC2" + struct.pack("<H", pop))
            # The stub is rewritten per call, and Unicorn runs its cached translation of the old bytes: a
            # thiscall float callee after a cdecl one ran `ret 0` and left its args on the stack (seen on
            # 0x0041EE15's GetFloorHeight).
            uc.ctl_remove_cache(FLD_STUB, FLD_STUB + 0x10)
            uc.reg_write(R.UC_X86_REG_EIP, FLD_STUB)
            return
        uc.reg_write(R.UC_X86_REG_ESP, esp + 4 + pop)
        uc.reg_write(R.UC_X86_REG_EIP, ret)

    def _varargs(self, spec, vals, ret, fixed_slots):
        """The types of the variable arguments at this call site, or None after failing the run."""
        what = spec.name or spec.key
        if spec.conv != "cdecl":
            return self._fail(f"{what}: varargs callees are cdecl")
        if spec.varargs == "printf":
            if not spec.args or spec.args[-1] != "cstr":
                return self._fail(f"{what}: varargs 'printf' needs the format as the last fixed arg, typed cstr")
            fmt = vals[-1]
            return [] if fmt is None else printf_arg_types(fmt)
        if spec.varargs == "stack":
            n = caller_cleanup(self.peek(ret, 8))
            if n is None or n % 4 or n // 4 < fixed_slots:
                return self._fail(f"{what}: varargs 'stack': no `add esp, N`/`pop` after the call at {ret:#x} "
                                  "(cleanup deferred or merged?); use 'printf' or fixed args")
            return ["u32"] * (n // 4 - fixed_slots)
        return self._fail(f"{what}: varargs must be one of {VARARGS}")

    # --- running -----------------------------------------------------------------------------------------
    def run(self, entry: int, allowed: set, *, regs: dict, stack_words: list, callees: dict,
            aliases: dict, ret_type: str = "int", max_insns: int = 200000,
            keep_stack_below: bool = False, arg_bytes: int = 0, ignore_writes=(),
            cover: set | None = None, rdtsc: int | None = None, rdtsc_step: int | None = None,
            fpu: list | None = None) -> RunResult:
        """Call `entry` once. `regs` = initial EAX..EBP (ECX/EDX already set for this/fastcall args);
        `stack_words` = dwords at [ESP+4]... Returns what the caller can observe. `cover`: a set that collects
        (va, size) of every executed translation block starting in `allowed` (coverage.py). `rdtsc` /
        `rdtsc_step`: the first RDTSC value and the increment per execution (default RDTSC_START / RDTSC_STEP).
        `fpu`: the x87 stack at entry, ST0 first, as 10-byte extended values (float_to_f80); default empty."""
        uc = self.uc
        self._rdtsc_next = RDTSC_START if rdtsc is None else rdtsc
        self._rdtsc_step = RDTSC_STEP if rdtsc_step is None else rdtsc_step
        self.state = None
        uc.context_restore(self.context)
        uc.reg_write(R.UC_X86_REG_EIP, INIT_STUB)
        uc.emu_start(INIT_STUB, INIT_STUB + 8)
        if fpu:
            if len(fpu) > 8:
                raise HarnessError("fpu: the x87 stack holds at most 8 values")
            for v in reversed(fpu):             # pushed bottom first, so fpu[0] ends up in ST0
                if len(v) != 10:
                    raise HarnessError(f"fpu: expected a 10-byte extended value, got {v!r}")
                uc.mem_write(FPU_IN_SLOT, bytes(v))
                uc.emu_start(FPU_IN_STUB, FPU_IN_STUB + 6)
        self.poke(ESP0, struct.pack("<I", SENTINEL) + b"".join(struct.pack("<I", w & 0xFFFFFFFF)
                                                              for w in stack_words))
        names = {"eax": R.UC_X86_REG_EAX, "ebx": R.UC_X86_REG_EBX, "ecx": R.UC_X86_REG_ECX,
                 "edx": R.UC_X86_REG_EDX, "esi": R.UC_X86_REG_ESI, "edi": R.UC_X86_REG_EDI,
                 "ebp": R.UC_X86_REG_EBP}
        for k, r in names.items():
            uc.reg_write(r, regs[k] & 0xFFFFFFFF)
        uc.reg_write(R.UC_X86_REG_ESP, ESP0)
        uc.reg_write(R.UC_X86_REG_EFLAGS, 0x202)
        st = _RunState(allowed=allowed, callees=callees, aliases=aliases, max_insns=max_insns, cover=cover)
        self.state = st
        try:
            uc.emu_start(entry, SENTINEL, count=max_insns)
        except UcError as e:
            if st.error is None:
                pc = uc.reg_read(R.UC_X86_REG_EIP)
                if e.errno == UC.UC_ERR_INSN_INVALID:
                    st.error, st.fault = f"invalid_insn (pc {pc:#x})", ("invalid_insn", None)
                else:
                    st.error, st.fault = f"emulation error: {e} at pc {pc:#x}", ("emulation", e.errno)
        self.state = None
        res = RunResult(trace=st.trace)
        pc = uc.reg_read(R.UC_X86_REG_EIP)
        if st.error is None and pc != SENTINEL:
            st.error = f"did not return within {max_insns} instructions (pc {pc:#x})"
            st.fault = ("timeout", None)
        res.error, res.fault = st.error, st.fault
        res.eax = uc.reg_read(R.UC_X86_REG_EAX)
        res.edx = uc.reg_read(R.UC_X86_REG_EDX)
        res.esp_delta = uc.reg_read(R.UC_X86_REG_ESP) - ESP0
        for k in ("ebx", "esi", "edi", "ebp"):
            v = uc.reg_read(names[k])
            if v != regs[k] & 0xFFFFFFFF:
                res.clobbered[k] = (regs[k] & 0xFFFFFFFF, v)
        sw = uc.reg_read(R.UC_X86_REG_FPSW)
        top = (sw >> 11) & 7
        res.fpu_depth = (8 - top) % 8
        if res.fpu_depth:
            mant, ex = uc.reg_read(R.UC_X86_REG_FP0 + top)
            res.st0 = f80_to_float(mant, ex)
        if ret_type == "cstr" and res.error is None:
            res.ret_str = self.cstr(res.eax)
        # Observable writes: everything except the callee's own stack frame, the argument slots (a callee
        # may reuse them as scratch), the TEB, and explicitly ignored ranges.
        lo_args, hi_args = ESP0 + 4, ESP0 + 4 + arg_bytes
        for va in sorted(st.written):
            if STACK_BASE <= va < ESP0 and not keep_stack_below:
                continue
            if lo_args <= va < hi_args or TEB_BASE <= va < TEB_BASE + PAGE:
                continue
            if FLD_STUB <= va < FLD_STUB + 0x100:
                continue
            if any(a <= va < b for a, b in ignore_writes):
                continue
            if self.region_of(va) is None:      # the write hook also fires for a write that then faults
                continue
            res.writes[va] = self.peek(va, 1)[0]
        return res


# --- varargs -----------------------------------------------------------------------------------------------

_PRINTF_INT = {"": "int", "h": "short", "l": "int", "I32": "int", "I64": "i64"}
_PRINTF_UINT = {"": "u32", "h": "u16", "l": "u32", "I32": "u32", "I64": "u64"}


def printf_arg_types(fmt: str) -> list:
    """Argument types a VC6 printf-family format consumes, in order (a `*` width or precision is an int).
    Sub-dword conversions (%c, %hd) record only their meaningful bits, like sub-dword args."""
    out, i, n = [], 0, len(fmt)
    while i < n:
        if fmt[i] != "%":
            i += 1
            continue
        i += 1
        if i < n and fmt[i] == "%":
            i += 1
            continue
        while i < n and fmt[i] in "-+ #0":
            i += 1
        if i < n and fmt[i] == "*":
            out.append("int")
            i += 1
        while i < n and fmt[i].isdigit():
            i += 1
        if i < n and fmt[i] == ".":
            i += 1
            if i < n and fmt[i] == "*":
                out.append("int")
                i += 1
            while i < n and fmt[i].isdigit():
                i += 1
        size = ""
        for p in ("I64", "I32", "h", "l", "L", "w"):
            if fmt.startswith(p, i):
                size, i = p, i + len(p)
                break
        if i >= n:
            break
        c = fmt[i]
        i += 1
        if c in "di":
            out.append(_PRINTF_INT.get(size, "int"))
        elif c in "ouxX":
            out.append(_PRINTF_UINT.get(size, "u32"))
        elif c in "cC":
            out.append("u16" if c == "C" or size in ("l", "w") else "char")
        elif c in "eEfgG":
            out.append("double")
        elif c == "s" and size not in ("l", "w"):
            out.append("cstr")
        elif c in "sSZpn":
            out.append("ptr")           # wide strings, ANSI_STRING*, pointers, %n targets: the pointer value
        # anything else: VC6's _output prints the character literally and consumes no argument
    return out


def caller_cleanup(code: bytes) -> int | None:
    """Bytes a cdecl caller pops right after the call (`add esp, N`, or VC6's `pop ecx` once or twice), else
    None (cleanup deferred and merged with a later call's, or folded into the epilogue)."""
    if code[:2] == b"\x83\xC4":
        return code[2]
    if code[:2] == b"\x81\xC4":
        return struct.unpack("<I", code[2:6])[0]
    pops = 0
    while pops < 2 and code[pops] == 0x59:
        pops += 1
    return 4 * pops if pops else None
