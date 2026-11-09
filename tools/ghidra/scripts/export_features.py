r"""Export what the name chain needs from one client project.

    export_features.py <client> [--no-binexport]

Writes to build\ghidra\ (never committed):
    <client>.BinExport        input for BinDiff (namespaces prepended to function names)
    <client>.features.json    per function: name, class, size, referenced strings, callees in call-site
                              order, distinctive immediates

Export before applying a name list that another project's names are checked against: BinDiff also
matches on names, and the cross-check is only worth something while the two sides are independent.
"""
import argparse
import time

import eqd_ghidra as eqd

MIN_STRING = 4
MAX_STRING = 256
MIN_IMMEDIATE = 0x100      # smaller constants are too common to identify anything
MAX_IMMEDIATES = 64


def binexport(program, out):
    from com.google.security.binexport import BinExport2Builder
    from java.io import FileOutputStream
    builder = BinExport2Builder(program, program.getMemory())
    builder.setPrependNamespace(True)
    proto = builder.build()
    stream = FileOutputStream(str(out))
    try:
        proto.writeTo(stream)
    finally:
        stream.close()


class StringReader:
    """String at an address: Ghidra's defined string, else printable NUL-terminated bytes in a data
    block (the string analyzer leaves many literals undefined)."""

    def __init__(self, program):
        from jpype import JArray, JByte
        self.listing = program.getListing()
        self.mem = program.getMemory()
        self.buf = JArray(JByte)(MAX_STRING + 1)
        self.cache = {}

    def at(self, addr):
        key = addr.getOffset()
        if key not in self.cache:
            self.cache[key] = self._read(addr)
        return self.cache[key]

    def _read(self, addr):
        data = self.listing.getDataAt(addr)
        if data is not None and data.hasStringValue():
            return str(data.getValue())
        block = self.mem.getBlock(addr)
        if block is None or not block.isInitialized() or block.isExecute():
            return None
        if data is not None and data.isDefined() and not data.hasStringValue():
            return None
        try:
            n = self.mem.getBytes(addr, self.buf)
        except Exception:
            return None
        raw = bytes(b & 0xFF for b in self.buf[:n])
        end = raw.find(bytes(1))
        if end < MIN_STRING or not all(0x20 <= c < 0x7F or c in (9, 10, 13) for c in raw[:end]):
            return None
        return raw[:end].decode("ascii")


def features(program):
    from ghidra.program.model.scalar import Scalar
    listing = program.getListing()
    fm = program.getFunctionManager()
    mem = program.getMemory()
    reader = StringReader(program)
    space = program.getAddressFactory().getDefaultAddressSpace()

    def add_string(addr, strings, seen):
        s = reader.at(addr)
        if s is not None and len(s) >= MIN_STRING and s not in seen:
            seen.add(s)
            strings.append(s)
    fid_addrs = eqd.fid_addresses(program)
    out = []
    for f in fm.getFunctions(True):
        if f.isExternal():
            continue
        strings, callees, imms = [], [], []
        seen_s, seen_i = set(), set()
        n_ins = 0
        for ins in listing.getInstructions(f.getBody(), True):
            n_ins += 1
            is_call = ins.getFlowType().isCall()
            for ref in ins.getReferencesFrom():
                to = ref.getToAddress()
                if ref.getReferenceType().isCall() or (is_call and ref.getReferenceType().isFlow()):
                    callee = fm.getFunctionAt(to)
                    if callee is not None:
                        if callee.isThunk():
                            callee = callee.getThunkedFunction(True)
                        callees.append(callee.getName(True) if callee.isExternal()
                                       else int(callee.getEntryPoint().getOffset()))
                    continue
                if not ref.getReferenceType().isData():
                    continue
                if to.isMemoryAddress():
                    add_string(to, strings, seen_s)
            for i in range(ins.getNumOperands()):
                if ins.getOperandReferences(i):
                    continue
                for obj in ins.getOpObjects(i):
                    if not isinstance(obj, Scalar):
                        continue
                    v = int(obj.getUnsignedValue()) & 0xFFFFFFFF
                    if not MIN_IMMEDIATE <= v < 0xFFFFFF00:
                        continue
                    addr = space.getAddress(v)
                    if mem.contains(addr):
                        add_string(addr, strings, seen_s)
                    elif v not in seen_i and len(imms) < MAX_IMMEDIATES:
                        seen_i.add(v)
                        imms.append(v)
        out.append({
            "address": int(f.getEntryPoint().getOffset()),
            "name": f.getName(True),
            "class": eqd.function_class(f, fid_addrs),
            "thunk": bool(f.isThunk()),
            "size": int(f.getBody().getNumAddresses()),
            "instructions": n_ins,
            "strings": strings,
            "callees": callees,
            "immediates": imms,
        })
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    ap.add_argument("--no-binexport", action="store_true")
    args = ap.parse_args()
    ws = eqd.Workspace()
    pyghidra = eqd.start(ws)

    project = eqd.open_project(ws, args.client)
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            t0 = time.time()
            funcs = features(program)
            ws.write_report(f"{args.client}.features.json", {
                "client": args.client,
                "image_base": int(program.getImageBase().getOffset()),
                "functions": funcs,
            })
            print(f"{len(funcs)} functions, {sum(bool(f['strings']) for f in funcs)} reference strings "
                  f"({time.time() - t0:.0f} s)")
            if not args.no_binexport:
                t0 = time.time()
                out = ws.reports / f"{args.client}.BinExport"
                binexport(program, out)
                print(f"wrote {out} ({out.stat().st_size} bytes, {time.time() - t0:.0f} s)")
    finally:
        project.close()


if __name__ == "__main__":
    main()
