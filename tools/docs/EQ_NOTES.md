# EQ code notes

Facts about EverQuest's own code (Trilogy client unless an entry says otherwise) that someone rewriting a
nearby function would otherwise rediscover. Compiler behaviour goes in `VC6_QUIRKS.md` instead, and only
with a repro.

**Format.** One entry per fact: `### <name> (0xADDR)`, then a `match:` line, then one to three lines of
text. `match:` is a Python regex (case-insensitive) searched in a function's context bundle (pseudocode,
assembly, callers/callees, globals), so a tool can show the matching entries next to the function: keep
the text short and make the regex specific: an address in the form the bundle prints it (`0x58fb58`,
`FUN_0040152f`) beats a word.

**Adding.** Check a fact against the assembly before it goes in here. Each entry says how it was checked.

### Small-block allocator (0x00401366)
match: 0x0?0401366|FUN_00401366|0x58fb5[89c]
`void *FUN_00401366(unsigned size)`: sizes above 0x80 go to operator_new (0x005218C6); otherwise it pops
the free list at `0x0058FB58 + ((size - 1) >> 3) * 4` (16 lists of 8-byte classes) and returns the node,
or refills via FUN_0040152F. It RETURNS the pointer in EAX on all three paths: callers use it. (Checked:
listing 00401366-0040139E. The first rewrite typed it `void` and its fixture `"ret": "void"`; it passed
every case because nothing compared EAX.)

### Free-list push, 8-byte class (0x0040139F)
match: 0x0?040139f|FUN_0040139f|0x58fb5c
Pushes a block on the 8-byte list head at 0x0058FB5C (`pool_free_lists_guess[1]`). Callee pops its one
argument but never reads ECX (a `__thiscall` that ignores `this`, or `__stdcall`). (Checked: BoxDraw.cpp
rewrite, tested.)

### rdp intrusive list helpers (0x004F5440-0x004F5500)
match: 0x0?04f54[4-9a-f]0|0x0?04f5500|list_(init|add_head|add_tail|remove_head)_guess|setLinkedListProperties_guess|fast_malloc
cdecl helpers the rdp code (connection.c, fast_malloc.c, heap.c) calls by address, never by name:
`list_init_guess` 0x004F5440 `(RdpList_guess *)`, `setLinkedListProperties_guess` 0x004F5460
`(RdpList_guess *, int, compare)`, `list_add_head_guess` 0x004F5480 and `list_add_tail_guess` 0x004F54C0
`(RdpList_guess *, RdpLink_guess *)`, `list_remove_head_guess` 0x004F5500 `(RdpList_guess *)` -> `void *`.
(Checked: Ghidra export names, existing EQ_CDECL uses in src/Network/rdp.)
