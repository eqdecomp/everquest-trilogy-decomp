"""Cross-TU calls by VA: a call to a function defined in ANOTHER translation unit, keyed by that definition's
`// FUNCTION:` VA (gate.py, compare.py --match).

A C++ class whose constructor/destructor lives in one TU (EQ_Overlay::EQ_Overlay in EQ_Overlay.cpp, annotated at
0x00450049) is called from every derived constructor by its decorated symbol (`??0EQ_Overlay@@QAE@XZ`), where a
TU-local stand-in calls the same function by address (`EQ_METHOD(0, void, 0x00450049, ...)`, in a match build the
EQD_MATCH symbol `?m@?$eqd_tc0@$0EFAAEJ@...`). Both are the same call. Keying the symbol by the VA its definition
is annotated with makes them compare equal, so the real form (`EQ_Bag_guess::EQ_Bag_guess` on the layout class)
can replace the stand-in.

What may be keyed this way, and only that (`annotated_defs`): an external FUNCTION symbol defined in a compiled
object of the client, whose undecorated name carries exactly one `// FUNCTION:` VA in that object's source and is
defined exactly once in that object (a decorated name, so overloads that share an undecorated name are left out:
they keep their symbol key). The map is decorated name -> VA. A symbol that two TUs define at different VAs is
dropped (`merge`). Nothing else changes: a call to an unannotated function, to an overload, to a symbol no compiled
TU defines, or to a TU that does not build keeps its symbol key, and a resolved VA that is not the one the other
side calls compares as a difference.
"""
from __future__ import annotations

import collections
from pathlib import Path

import coff


def _undecorate():
    from eqd_unicorn import undecorate
    return undecorate


def annotated_defs(obj, anns, undecorate=None) -> dict:
    """{decorated name: VA} of the external function definitions in COFF object `obj` (a path or a parsed
    coff.Obj) that its source annotates: `anns` are that source's annotations (eqd_unicorn.annotations)."""
    undecorate = undecorate or _undecorate()
    o = coff.parse(obj) if not isinstance(obj, coff.Obj) else obj
    vas = collections.defaultdict(set)
    for a in anns:
        vas[a.name].add(a.va)
    defs = collections.defaultdict(set)
    for s in o.symbols:
        if s and s.section > 0 and (s.type & 0x20) and s.storage == 2:
            defs[undecorate(s.name)].add(s.name)
    out = {}
    for name, v in vas.items():
        d = defs.get(name, ())
        if len(v) == 1 and len(d) == 1:
            out[next(iter(d))] = next(iter(v))
    return out


def external_refs(obj) -> set:
    """Undefined external symbols of a COFF object that could name another TU's function: not EQD_MATCH symbols,
    not imports, not absolute CRT values."""
    o = coff.parse(obj) if not isinstance(obj, coff.Obj) else obj
    out = set()
    for s in o.symbols:
        if s and s.section == 0 and s.storage == 2 and not s.value and not coff.eqd_va(s.name) \
                and not s.name.startswith("__imp_") and s.name not in coff.ABSOLUTE_SYMBOLS:
            out.add(s.name)
    return out


def merge(maps) -> dict:
    """One {decorated: VA} map from several TUs' maps; a name with two different VAs is dropped (ambiguous)."""
    out, bad = {}, set()
    for m in maps:
        for k, v in m.items():
            if k in out and out[k] != v:
                bad.add(k)
            out.setdefault(k, v)
    for k in bad:
        out.pop(k, None)
    return out


def defining_sources(refs, ann_by_name: dict, undecorate=None) -> set:
    """The sources whose annotations name one of `refs` (decorated, undecorated here): the TUs to compile to learn
    the refs' VAs. `ann_by_name`: {annotated name: {source}} (`annotation_index`)."""
    undecorate = undecorate or _undecorate()
    out = set()
    for r in refs:
        out |= set(ann_by_name.get(undecorate(r), ()))
    return out


def annotation_index(anns) -> dict:
    """{annotated name: {source path}} from a list of annotations."""
    out = collections.defaultdict(set)
    for a in anns:
        out[a.name].add(a.path if isinstance(a.path, str) else str(a.path))
    return out


def scan_sources(paths, token: str) -> list:
    """Annotations of every readable source in `paths` (a file that fails to scan is skipped)."""
    from eqd_unicorn import annotations as ann
    out = []
    for p in paths:
        try:
            out += ann.scan([Path(p)], token)
        except (ValueError, OSError):
            continue
    return out
