// BoxDraw.cpp - eqmain.dll, Suite/BoxDraw (the chain tables). No Mac map covers eqmain.dll, so the original source file is unknown.

#include "trilogy/eqaddr.h"
#include <string.h>

// FUNCTION: EQMAIN 0x10008CE0
// Releases a front-end data table and its owned resources.
// __cdecl, one pointer argument: the table (the callee pops 0). No return value (no caller reads EAX). Frees a table of
// two-level chains: PackFsFreeEntry_guess first, on the table's +0x30 pointer. While the outer list at +0x1c has a node,
// list3_unlink_free_guess unlinks it and returns the entry it held; the entry's sub-list (head at +0xc) is emptied the same way, each
// sub-entry's block (+0x8) freed and then the sub-entry, and the outer entry is freed once its sub-list is empty.
// The table itself is freed last.
void __cdecl chain_table_free_guess(void *block)
{
    int *node;
    void *_Memory;
    void *_Memory_00;

    EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((char *)block + 0x30));
    node = *(int **)((char *)block + 0x1c);
    while (node != 0)
    {
        _Memory = EQ_CDECL(void *, 0x10028430, (int *, int *))((int *)((char *)block + 0x1c), node);
        node = *(int **)((char *)_Memory + 0xc);
        while (node != 0)
        {
            _Memory_00 = EQ_CDECL(void *, 0x10028430, (int *, int *))((int *)((char *)_Memory + 0xc), node);
            EQ_CDECL(void, 0x10033568, (void *))(*(void **)((char *)_Memory_00 + 8));
            EQ_CDECL(void, 0x10033568, (void *))(_Memory_00);
            node = *(int **)((char *)_Memory + 0xc);
        }
        EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        node = *(int **)((char *)block + 0x1c);
    }
    EQ_CDECL(void, 0x10033568, (void *))(block);
    return;
}

// FUNCTION: EQMAIN 0x10018AE0
// Loads named entries from a front-end data record.
// @early-stop: /O2 adds a first-entry bounds check and precomputes the second destination in EDI.
// __cdecl, one argument (the callee pops 0). No return value (no caller reads EAX). Clears the counter g_eqmain_count_guess,
// passes the argument to SetStateAndDword_guess (it stores it as the read base), then fills the 0x58-byte entries from
// DAT_10052DC0 while the entry is below DAT_10055020: a 12-byte name from NextString_guess, a dword from NextDword_guess at
// entry-8, a 0x40-byte string from NextString_guess at entry+0xc, and the counter. An empty name (strlen 0) returns before
// the dword and the string are read.
void __cdecl chain_iter_next_bucket_guess(unsigned int data)
{
    char *_Dest;

    EQ_GLOBAL(int, 0x10055038) = 0;
    EQ_CDECL(void, 0x100250A0, (unsigned int))(data);
    for (_Dest = EQ_ARRAY(char, 0x10052DC0); (int)_Dest < 0x10055020; _Dest = _Dest + 0x58)
    {
        EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))(_Dest, EQ_STDCALL(char *, 0x10025150, (void))(), 0xc);
        if (strlen(_Dest) == 0)
            return;
        *(unsigned int *)(_Dest - 8) = EQ_STDCALL(unsigned int, 0x10025120, (void))();
        EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))(_Dest + 0xc, EQ_STDCALL(char *, 0x10025150, (void))(), 0x40);
        EQ_GLOBAL(int, 0x10055038) = EQ_GLOBAL(int, 0x10055038) + 1;
    }
}
