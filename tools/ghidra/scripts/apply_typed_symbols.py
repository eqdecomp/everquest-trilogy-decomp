r"""Type globals and function signatures from a committed list.

    apply_typed_symbols.py <client> [--project-dir DIR]

Reads the configured CSV or <client repo>\ghidra-types\<client>.symbols.csv
(--symbols overrides it; columns: address, kind, name, type, note):
  kind global    type is a data type ("EQPlayer *", "NetSpawn[400]"); the data at the address is
                 set to it and, if name is given, the label gets that name (source IMPORTED; a
                 hand-set label is never replaced). Existing defined data is kept, except a
                 bare pointer/void * that a typed pointer replaces.
  kind function  type is "<return> <convention> (<type> <name>, ...)". The function's parameters and
                 return type are set; its name is left alone (names come from apply_names.py), so
                 name must be empty. For __thiscall with a typed "this" first parameter the storage
                 is spelled out (this in ECX, the rest on the stack), because Ghidra would otherwise
                 type "this" from the namespace. A signature someone set by hand (USER_DEFINED) is
                 never replaced.
Types come from the program (run apply_types.py first) or are built-in (void, int, char, ...).
Running it twice changes nothing the second time. Report: build\ghidra\<client>.typed.json.
"""
import argparse
import csv
import re

import eqd_ghidra as eqd

SIG = re.compile(r"^\s*(?P<ret>.+?)\s*(?P<cc>__\w+)\s*\((?P<params>.*)\)\s*$")
TYPE = re.compile(r"^\s*(?P<base>[A-Za-z_]\w*)\s*(?P<ptr>[\s*]*)(?P<dims>(\[\d+\])*)\s*$")
PARAM = re.compile(r"^(?P<type>.*?[\s*])(?P<name>[A-Za-z_]\w*)$")


def read_list(path):
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


class Types:
    def __init__(self, program):
        from ghidra.program.model.data import BuiltInDataTypeManager
        self.dtm = program.getDataTypeManager()
        self.builtin = BuiltInDataTypeManager.getDataTypeManager()

    def base(self, name):
        from java.util import ArrayList
        found = ArrayList()
        self.dtm.findDataTypes(name, found)
        found = [dt for dt in found if dt.getName() == name]
        if len(found) == 1:
            return found[0]
        if len(found) > 1:
            raise ValueError(f"type {name} is ambiguous in the program ({len(found)} copies)")
        dt = self.builtin.getDataType("/" + name)
        if dt is None:
            raise ValueError(f"unknown type {name}")
        return dt

    def parse(self, text):
        from ghidra.program.model.data import ArrayDataType, PointerDataType
        m = TYPE.match(text)
        if not m:
            raise ValueError(f"cannot parse type {text!r}")
        dt = self.base(m.group("base"))
        for _ in range(m.group("ptr").count("*")):
            dt = PointerDataType(dt, self.dtm)
        for n in reversed(re.findall(r"\[(\d+)\]", m.group("dims"))):
            dt = ArrayDataType(dt, int(n), dt.getLength(), self.dtm)
        return dt


def parse_signature(types, text):
    m = SIG.match(text)
    if not m:
        raise ValueError(f"cannot parse signature {text!r}")
    ret = types.parse(m.group("ret"))
    params = []
    body = m.group("params").strip()
    if body and body != "void":
        for p in body.split(","):
            pm = PARAM.match(p.strip())
            if not pm:
                raise ValueError(f"cannot parse parameter {p!r}")
            params.append((pm.group("name"), types.parse(pm.group("type"))))
    return ret, m.group("cc"), params


def signature_key(f):
    """What we set, as comparable text."""
    return (str(f.getCallingConventionName()), str(f.getReturnType().getPathName()),
            tuple((str(p.getName()), str(p.getDataType().getPathName()), str(p.getVariableStorage()))
                  for p in f.getParameters()), bool(f.hasCustomVariableStorage()))


def apply_function(program, f, ret, cc, params):
    from ghidra.program.model.listing import Function, ParameterImpl, ReturnParameterImpl, VariableStorage
    from ghidra.program.model.symbol import SourceType
    from ghidra.program.model.data import VoidDataType
    from java.util import ArrayList
    custom = cc == "__thiscall" and params and params[0][0] == "this"
    if custom:
        reg = program.getRegister
        plist, offset = [], 4
        for i, (name, dt) in enumerate(params):
            if i == 0:
                storage = VariableStorage(program, [reg("ECX")])
            else:
                storage = VariableStorage(program, offset, dt.getLength())
                offset += (dt.getLength() + 3) // 4 * 4
            plist.append(ParameterImpl(name, dt, storage, program, SourceType.IMPORTED))
        if isinstance(ret, VoidDataType):
            rstore = VariableStorage.VOID_STORAGE
        else:
            rstore = VariableStorage(program, [reg("EAX")])
        f.updateFunction(cc, ReturnParameterImpl(ret, rstore, program), ArrayList(plist),
                         Function.FunctionUpdateType.CUSTOM_STORAGE, True, SourceType.IMPORTED)
    else:
        plist = [ParameterImpl(name, dt, program, SourceType.IMPORTED) for name, dt in params]
        f.setCustomVariableStorage(False)
        f.updateFunction(cc, ReturnParameterImpl(ret, program), ArrayList(plist),
                         Function.FunctionUpdateType.DYNAMIC_STORAGE_FORMAL_PARAMS, True, SourceType.IMPORTED)
    f.setSignatureSource(SourceType.IMPORTED)


def is_untyped_pointer(dt):
    """pointer, void * or undefined * (what analysis creates), but no typed pointer."""
    from ghidra.program.model.data import Pointer, TypeDef, Undefined, VoidDataType
    while isinstance(dt, TypeDef):
        dt = dt.getBaseDataType()
    if not isinstance(dt, Pointer):
        return False
    target = dt.getDataType()
    return target is None or isinstance(target, (VoidDataType, Undefined))


def apply_global(program, addr, dt, name, report, row):
    from ghidra.program.model.data import DataUtilities, Pointer
    from ghidra.program.model.symbol import SourceType
    changed = False
    data = program.getListing().getDefinedDataAt(addr)
    if data is None or not data.getDataType().isEquivalent(dt):
        mode = DataUtilities.ClearDataMode.CLEAR_ALL_UNDEFINED_CONFLICT_DATA
        if data is not None and is_untyped_pointer(data.getDataType()) and isinstance(dt, Pointer):
            # Analysis (or an older list) left a bare pointer/void * here: a typed pointer
            # of the same size replaces it. Other defined data is never cleared.
            mode = DataUtilities.ClearDataMode.CLEAR_SINGLE_DATA
        DataUtilities.createData(program, addr, dt, -1, False, mode)
        changed = True
    if name:
        st = program.getSymbolTable()
        sym = st.getPrimarySymbol(addr)
        if sym is not None and sym.getSource() == SourceType.USER_DEFINED and sym.getName() != name:
            report["problems"].append(f"{row['address']}: kept hand-set label {sym.getName()} (list says {name})")
        elif sym is None or sym.getName() != name:
            label = st.getGlobalSymbol(name, addr) or st.createLabel(addr, name, SourceType.IMPORTED)
            label.setPrimary()
            changed = True
    return changed


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("client", choices=sorted(eqd.CLIENTS))
    ap.add_argument("--project-dir", help="project folder to use instead of <workspace>\\ghidra\\<client>")
    ap.add_argument("--symbols", help="typed-symbol CSV (default from workspace config/repository)")
    args = ap.parse_args()
    ws = eqd.Workspace()
    from pathlib import Path
    lst = Path(args.symbols).resolve() if args.symbols else ws.typed_symbols(args.client)
    rows = read_list(lst)

    pyghidra = eqd.start(ws)
    from ghidra.program.model.symbol import SourceType
    from ghidra.util.task import TaskMonitor

    project = (pyghidra.open_project(args.project_dir, args.client) if args.project_dir
               else eqd.open_project(ws, args.client))
    report = {"client": args.client, "list": str(lst),
              "globals": {"applied": 0, "unchanged": 0}, "functions": {"applied": 0, "unchanged": 0},
              "problems": []}
    try:
        with pyghidra.program_context(project, ws.program_path(args.client)) as program:
            types = Types(program)
            space = program.getAddressFactory().getDefaultAddressSpace()
            fm = program.getFunctionManager()
            with pyghidra.transaction(program, "Apply typed symbols"):
                for row in rows:
                    addr = space.getAddress(int(row["address"], 16))
                    kind = row["kind"].strip()
                    try:
                        if kind == "global":
                            dt = types.parse(row["type"])
                            changed = apply_global(program, addr, dt, row["name"].strip(), report, row)
                            report["globals"]["applied" if changed else "unchanged"] += 1
                        elif kind == "function":
                            if row["name"].strip():
                                raise ValueError("function rows must not carry a name (apply_names.py names them)")
                            f = fm.getFunctionAt(addr)
                            if f is None:
                                raise ValueError("no function entry here")
                            ret, cc, params = parse_signature(types, row["type"])
                            if f.getSignatureSource() == SourceType.USER_DEFINED:
                                report["problems"].append(f"{row['address']}: kept hand-set signature {f.getSignature()}")
                                continue
                            before = signature_key(f)
                            apply_function(program, f, ret, cc, params)
                            report["functions"]["applied" if signature_key(f) != before else "unchanged"] += 1
                        else:
                            raise ValueError(f"unknown kind {kind!r}")
                    except Exception as e:      # bad type, conflicting data, ...
                        report["problems"].append(f"{row['address']}: {e}")
            program.save("Apply typed symbols", TaskMonitor.DUMMY)
    finally:
        project.close()
    ws.write_report(f"{args.client}.typed.json", report)
    print(report["globals"], report["functions"], len(report["problems"]), "problems")
    for p in report["problems"]:
        print("  ", p)
    if report["problems"]:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
