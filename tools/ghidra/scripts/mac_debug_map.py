r"""Read the Intel Mac client's debug map: which original source file each function came from.

    mac_debug_map.py

The Mach-O symbol table still has the STABS entries the linker kept for dsymutil: N_SO (source file),
N_OSO (object file) and N_FUN (function start). Writes build\ghidra\mac.debugmap.json, a list of
{address, symbol, file, object}. No Ghidra needed.
"""
import struct

import eqd_ghidra as eqd

MH_MAGIC = 0xFEEDFACE
LC_SYMTAB = 0x2
N_FUN, N_SO, N_OSO = 0x24, 0x64, 0x66


def read_stabs(data):
    magic, _, _, _, ncmds, _, _ = struct.unpack_from("<7I", data, 0)
    if magic != MH_MAGIC:
        raise SystemExit(f"not a 32-bit little-endian Mach-O (magic {magic:#x})")
    off = 28
    for _ in range(ncmds):
        cmd, size = struct.unpack_from("<2I", data, off)
        if cmd == LC_SYMTAB:
            symoff, nsyms, stroff, _ = struct.unpack_from("<4I", data, off + 8)
            break
        off += size
    else:
        raise SystemExit("no LC_SYMTAB")

    def name(strx):
        end = data.index(b"\0", stroff + strx)
        return data[stroff + strx:end].decode("latin-1")

    for i in range(nsyms):
        strx, n_type, _, _, value = struct.unpack_from("<IBBHI", data, symoff + 12 * i)
        if n_type in (N_FUN, N_SO, N_OSO):
            yield n_type, name(strx), value


def main():
    ws = eqd.Workspace()
    data = ws.binary("mac").read_bytes()
    out = []
    directory = source = obj = ""
    for n_type, name, value in read_stabs(data):
        if n_type == N_SO:
            # A pair per compile unit: directory (ends with "/"), then the file. An empty name ends it.
            if not name:
                directory = source = obj = ""
            elif name.endswith("/"):
                directory = name
            else:
                source = name if name.startswith("/") else directory + name
        elif n_type == N_OSO:
            obj = name
        elif name:                      # N_FUN with a name starts a function; the empty one ends it
            out.append({"address": value, "symbol": name.split(":")[0], "file": source, "object": obj})
    files = sorted({e["file"] for e in out})
    ws.write_report("mac.debugmap.json", {"functions": out, "files": files})
    print(f"{len(out)} functions in {len(files)} source files")


if __name__ == "__main__":
    main()
