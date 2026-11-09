r"""Naming coverage for the client projects.

    coverage_report.py [client ...]        default: trilogy takp mac

Writes build\ghidra\<client>.coverage.json and build\coverage.md. Every function is counted in exactly
one class:

    library        Function ID match (tagged LIBRARY)
    import_thunk   thunk to an imported DLL function
    eh_funclet     compiler-generated Unwind@/Catch@ stub
    named          any other non-default name (symbols, name lists)
    unnamed        still FUN_xxxxxxxx

"Typed" counts game functions (named + unnamed) whose signature is no longer Ghidra's default
(set from eq-<client>\ghidra-types\<client>.symbols.csv by apply_typed_symbols.py, or by hand), and
globals whose data type comes from the client's own type archive (apply_types.py).
"""
import argparse
import time

import eqd_ghidra as eqd

WINDOW = 50        # functions looked at when locating the statically linked CRT
DENSITY = 0.4      # share of them that must be FID matches


def library_span(program):
    """Estimate the statically linked library block: from the first point where FID matches become
    dense to the last FID match. Returns (start, end, unmatched functions inside) or None."""
    fid_addrs = eqd.fid_addresses(program)
    funcs = [f for f in program.getFunctionManager().getFunctions(True)
             if not eqd.is_eh_funclet(f.getName())]
    flags = [f.getEntryPoint() in fid_addrs for f in funcs]
    start = next((i for i in range(len(funcs))
                  if flags[i] and sum(flags[i:i + WINDOW]) >= DENSITY * WINDOW), None)
    if start is None:
        return None
    end = max(i for i, x in enumerate(flags) if x)
    inside = [f for f, x in zip(funcs[start:end + 1], flags[start:end + 1]) if not x]
    return {
        "start": str(funcs[start].getEntryPoint()),
        "end": str(funcs[end].getEntryPoint()),
        "functions": end - start + 1,
        "not_fid_matched": len(inside),
        "not_fid_matched_unnamed": sum(eqd.is_default_name(f.getName()) for f in inside),
    }


def typed_stats(program, client):
    """Functions with a non-default signature, and globals typed with the client's archive types."""
    from ghidra.program.model.data import Array, Pointer, TypeDef
    from ghidra.program.model.symbol import SourceType
    fid_addrs = eqd.fid_addresses(program)
    game = typed = 0
    for f in program.getFunctionManager().getFunctions(True):
        if eqd.function_class(f, fid_addrs) in ("named", "unnamed"):
            game += 1
            typed += f.getSignatureSource() != SourceType.DEFAULT
    def ours(dt):
        src = dt.getSourceArchive() if dt is not None else None
        return src is not None and src.getName() == client

    globals_typed = 0
    for d in program.getListing().getDefinedData(True):
        dt = d.getDataType()
        while isinstance(dt, (Array, Pointer, TypeDef)) and not ours(dt):
            dt = dt.getDataType()
        globals_typed += ours(dt)
    return {"game_functions": game, "functions_typed": typed, "globals_typed": globals_typed}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("clients", nargs="*", default=["trilogy", "takp", "mac"])
    args = ap.parse_args()
    ws = eqd.Workspace()
    pyghidra = eqd.start(ws)

    rows = []
    for client in args.clients:
        project = eqd.open_project(ws, client)
        try:
            with pyghidra.program_context(project, ws.program_path(client)) as program:
                stats = eqd.function_stats(program)
                stats["client"] = client
                stats["library_span_estimate"] = library_span(program)
                stats["typed"] = typed_stats(program, client)
        finally:
            project.close()
        ws.write_report(f"{client}.coverage.json", stats)
        rows.append(stats)

    def pct(n, d):
        return f"{100.0 * n / d:.1f}%" if d else "-"

    lines = [f"# Naming coverage ({time.strftime('%Y-%m-%d')})", "",
             "| Client | Functions | Library (FID) | Import thunks | EH funclets | Named | Unnamed | "
             ".text in functions | .text undefined | Typed (functions / globals) |",
             "|---|---|---|---|---|---|---|---|---|---|"]
    for s in rows:
        c, n, t = s["classes"], s["functions"], s.get("text")
        lines.append(
            f"| {s['client']} | {n} | {c['library']} ({pct(c['library'], n)}) | {c['import_thunk']} | "
            f"{c['eh_funclet']} | {c['named']} ({pct(c['named'], n)}) | {c['unnamed']} "
            f"({pct(c['unnamed'], n)}) | {pct(t['in_functions'], t['bytes']) if t else '-'} | "
            f"{pct(t['undefined'], t['bytes']) if t else '-'} | "
            f"{s['typed']['functions_typed']} ({pct(s['typed']['functions_typed'], s['typed']['game_functions'])}) "
            f"/ {s['typed']['globals_typed']} |")
    lines.append("")
    for s in rows:
        span = s["library_span_estimate"]
        if span:
            lines.append(
                f"- **{s['client']}**: FID matches are dense from `{span['start']}` to `{span['end']}` "
                f"({span['functions']} functions). {span['not_fid_matched']} functions in that span have no "
                f"FID match ({span['not_fid_matched_unnamed']} unnamed); most are probably library code too "
                f"(estimate, not tagged).")
    out = ws.build / "coverage.md"
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("\n".join(lines))
    print(f"wrote {out}")


if __name__ == "__main__":
    main()
