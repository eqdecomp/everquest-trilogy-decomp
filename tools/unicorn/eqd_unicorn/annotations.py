"""Read `// FUNCTION: <CLIENT> 0x...` annotations from decomp sources.

The annotation is followed (after more `//` comment lines) by the function definition. The qualified name
is the last `Name` or `Class::Name` before the first `(` on the definition line, e.g.
    char* CEverQuest::GetLanguageName(short languageId)  ->  CEverQuest::GetLanguageName
with the file's one-identifier `#define`s in force there applied, as the compiler sees it (the giants'
`#define CEverQuest CEverQuest_HandleWorldMessage_type_guess` file-local class trick).
"""
from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path

_ANN = re.compile(r"^\s*//\s*FUNCTION:\s*(\w+)\s+(0x[0-9a-fA-F]+)")
_NAME = re.compile(r"([A-Za-z_~][\w]*(?:::[A-Za-z_~][\w]*)*)\s*\(")
# Comments between the annotation and the definition are skipped whole, `/* */` blocks included, and
# preprocessor lines: a word before a parenthesis in one ("the creation routine (0x0040D980)") was taken as the
# name (seen in another decomp project, where it was worked around by setting `rewrite` by hand).
_BLOCK = re.compile(r"/\*.*?\*/", re.S)
_LINE = re.compile(r"//[^\n]*")
_DEFINE = re.compile(r"^\s*#\s*define\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*(?://.*)?$")
_UNDEF = re.compile(r"^\s*#\s*undef\s+([A-Za-z_]\w*)")


@dataclass(frozen=True)
class Annotation:
    client: str
    va: int
    name: str
    path: str
    line: int


def scan(paths, client: str | None = None) -> list[Annotation]:
    out = []
    for p in paths:
        lines = Path(p).read_text(encoding="utf-8", errors="replace").splitlines()
        defines = {}
        i = 0
        while i < len(lines):
            if d := _DEFINE.match(lines[i]):
                defines[d.group(1)] = d.group(2)
            elif u := _UNDEF.match(lines[i]):
                defines.pop(u.group(1), None)
            m = _ANN.match(lines[i])
            if not m or (client and m.group(1).upper() != client.upper()):
                i += 1
                continue
            code = _LINE.sub("", _BLOCK.sub(" ", "\n".join(lines[i + 1:i + 200])))
            head = " ".join([l for l in code.splitlines() if l.strip() and not l.lstrip().startswith("#")][:3])
            n = _NAME.search(head)
            if not n:
                raise ValueError(f"{p}:{i + 2}: no function name after FUNCTION annotation")
            name = "::".join(defines.get(part, part) for part in n.group(1).split("::"))
            out.append(Annotation(m.group(1).upper(), int(m.group(2), 16), name, str(p), i + 1))
            i += 1
    return out
