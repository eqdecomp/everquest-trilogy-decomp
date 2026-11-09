# References

Community projects and other material used as **evidence only**. None of it is copied into this repo:
facts (offsets, opcode values, addresses, names) may be used, code may not.

| Reference | What it gives us | Licence |
|---|---|---|
| **EQClassic** (Trilogy-era server source) | Packet structs, opcodes, wire protocol notes, and comments that name client addresses (the `eqclassic` names in `names/`, with our own notes) | Unclear: no licence file; GPLv2 headers in some files, "All Rights Reserved" in others |
| **ShowEQ 3.0.4** | Velious-era packet structs and opcodes (independent cross-check) | GPL v2 or later |
| **TAKP-Client-Underthehood** | Function name lists for the 2002 PC client (the `takp` names in `names/`, marked descriptive) | None stated |
| **TAKP server (EQMacEmu)** | Packet structs and opcodes, net-message field names | GPL |
| **EQEmu** | Later-client structs and opcodes, for comparison | GPL v3 |
| **MacroQuest (2002)** | In-memory layout naming hints (addresses don't apply) | None stated |
| Later EverQuest clients (Mac, 2002 PC) and the 1999 beta client | Function names and original source file names from their symbols and debug strings (`mac`, `beta` names in `names/`) | Proprietary; never committed |

Every name in `names/` records its source and a confidence; see `names/README.md`.
