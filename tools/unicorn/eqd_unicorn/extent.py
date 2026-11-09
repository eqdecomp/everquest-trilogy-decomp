"""Which instructions belong to the function under test.

The harness treats any basic block that starts outside this set as a call (or tail jump) to another function
and intercepts it. The set comes from a recursive descent with capstone:
  - conditional and direct jumps are followed, calls are not (execution continues after them);
  - `ret`, `int3`, `hlt`, indirect `jmp reg` and `jmp [mem]` (an import thunk / tail call) end a path;
  - a direct jump to, or fall-through onto, a known function start (`stops`) is a tail call, not a branch;
  - an explicit `size` limits the function to [entry, entry + size) instead;
  - VC6 switch tables `jmp [reg*4 + table]` are followed. How many entries the table has:
      1. the immediate of the last `cmp` that is *immediately followed by an unsigned upper-bound jump*
         (ja/jbe/jae/jb): N+1 entries. Any other `cmp` (also `cmp reg,reg`) and any `call` forgets the
         bound, so a stale one from an earlier switch or loop is never used (`cmp eax,-1; je` is no bound);
      2. a byte index table (`mov/movzx r8,[reg+tbl]`, VC6's form for sparse switches), only if it is loaded
         after that bound, its destination is the jump's index register and `tbl` lies inside the image
         (`in_image`): max byte + 1 entries. A struct byte `[esi+0x8FBAC]` has a small disp but is no table;
      3. no bound at all (VC6 omits the range check after `default: __assume(0)`): read entries while they
         point into [entry, table) (with `size`: [entry, entry + size)) and are not other functions' starts
         (at most 1024);
      4. none of these: ExtentError, give `size` (with `size` such a table just yields no cases).
    (CRT memcpy's two tables are the known exception: bounded only by an `and` on another path, they need
    `size`.)
    Pass a list as `switches` to get one `(jmp_va, table, count, how)` per decoded switch jump
    (`how` is "cmp", "bytetable" or "scan").
"""
from __future__ import annotations

import capstone
from capstone import x86_const as X

_MD = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
_MD.detail = True
_UPPER = {"ja", "jbe", "jae", "jb"}
_REG32 = {"al": "eax", "ah": "eax", "ax": "eax", "bl": "ebx", "bh": "ebx", "bx": "ebx", "cl": "ecx",
          "ch": "ecx", "cx": "ecx", "dl": "edx", "dh": "edx", "dx": "edx", "si": "esi", "di": "edi",
          "bp": "ebp", "sp": "esp"}
_NONE = (None, None, None)        # (pending cmp imm, bound, byte table (reg32, va))


class ExtentError(RuntimeError):
    pass


def _reg32(r):
    n = _MD.reg_name(r)
    return _REG32.get(n, n)


def function_extent(read, entry: int, stops=frozenset(), size: int | None = None,
                    max_insns: int = 200000, in_image=None, switches=None) -> set:
    """Return the set of instruction addresses of the function at `entry`.

    `read(va, n) -> bytes` reads the image. `stops` = known function starts other than `entry`.
    `in_image(va) -> bool` says whether an address is inside the image (default: >= 0x10000); pass the PE's
    `contains`. `switches`, if a list, receives `(jmp_va, table, count, how)` for every switch jump.
    """
    lo, hi = (entry, entry + size) if size else (None, None)
    in_image = in_image or (lambda a: a >= 0x10000)
    seen: set = set()
    work = [(entry, _NONE)]
    while work:
        va, (pend, bound, btab) = work.pop()
        while True:
            if va in seen or (va != entry and va in stops):
                break
            if lo is not None and not (lo <= va < hi):
                break
            insn = next(_MD.disasm(read(va, 16), va, 1), None)
            if insn is None:
                break
            seen.add(va)
            if len(seen) > max_insns:
                raise ExtentError(f"function at {entry:#x} exceeds {max_insns} instructions")
            m, ops = insn.mnemonic, insn.operands
            if m == "cmp":                      # any compare starts a new bound (cmp reg,reg -> none)
                imm = len(ops) == 2 and ops[1].type == X.X86_OP_IMM
                pend, bound, btab = (ops[1].imm & 0xFFFFFFFF) if imm else None, None, None
            elif m in ("mov", "movzx") and len(ops) == 2 and ops[0].type == X.X86_OP_REG \
                    and ops[1].type == X.X86_OP_MEM and ops[1].size == 1 and ops[1].mem.scale == 1 \
                    and in_image(ops[1].mem.disp & 0xFFFFFFFF) and bound is not None:
                btab = (_reg32(ops[0].reg), ops[1].mem.disp & 0xFFFFFFFF)
            elif m == "call":
                pend = bound = btab = None
            if m in ("ret", "retf", "int3", "hlt", "ud2", "iretd"):
                break
            if m == "jmp":
                op = ops[0]
                if op.type == X.X86_OP_IMM:
                    va = op.imm & 0xFFFFFFFF
                    continue
                if op.type == X.X86_OP_MEM and op.mem.index != 0 and op.mem.scale == 4 and op.mem.base == 0:
                    table = op.mem.disp & 0xFFFFFFFF
                    idx = _reg32(op.mem.index)
                    if btab is not None and btab[0] == idx and bound is not None and bound < 0x1000:
                        count, how = max(read(btab[1], bound + 1)) + 1, "bytetable"
                    elif bound is not None and bound < 0x1000:
                        count, how = bound + 1, "cmp"
                    else:
                        count = 0
                        while count < 1024:
                            t = int.from_bytes(read(table + 4 * count, 4), "little")
                            if not (entry <= t < (hi if hi is not None else table)) or t in stops:
                                break
                            count += 1
                        how = "scan"
                        if not count and size is None:
                            raise ExtentError(f"switch table at {table:#x} ({va:#x}) has no bound; give `size`")
                    if switches is not None:
                        switches.append((va, table, count, how))
                    for i in range(count):
                        work.append((int.from_bytes(read(table + 4 * i, 4), "little"), _NONE))
                break
                # jmp reg / jmp [mem]: tail call or thunk, ends the path
            if insn.group(capstone.CS_GRP_JUMP):          # jcc, loop, jecxz
                if pend is not None and m in _UPPER:
                    bound = pend
                pend = None
                if ops[0].type == X.X86_OP_IMM:
                    work.append((ops[0].imm & 0xFFFFFFFF, (pend, bound, btab)))
            va += insn.size
    if not seen:
        raise ExtentError(f"nothing decodes at {entry:#x}")
    return seen


def stack_purge(read, entry: int, stops=frozenset(), in_image=None) -> int | None:
    """Bytes a function pops on return (its `ret N`), from its code: the one N every `ret` in its extent
    agrees on. None when that can't be known: no `ret` reached (a thunk `jmp [IAT]`, a function that never
    returns), `ret`s that disagree, or code that doesn't decode. A lone `jmp` (an ILT-style thunk) is followed
    once. Used to check a fixture's callee stubs (difftest `_check_purges`): a stub that pops a different
    number of bytes than the real callee unbalances the caller's stack."""
    try:
        insn = next(_MD.disasm(read(entry, 16), entry, 1), None)
        if insn is not None and insn.mnemonic == "jmp" and insn.operands[0].type == X.X86_OP_IMM:
            entry = insn.operands[0].imm & 0xFFFFFFFF
        ext = function_extent(read, entry, frozenset(stops) - {entry}, in_image=in_image, max_insns=20000)
    except Exception:           # noqa: BLE001 (ExtentError, unreadable or undecodable code: unknown)
        return None
    pops = set()
    for va in ext:
        i = next(_MD.disasm(read(va, 16), va, 1), None)
        if i is not None and i.mnemonic == "ret":
            pops.add(i.operands[0].imm & 0xFFFF if i.operands else 0)
    return pops.pop() if len(pops) == 1 else None
