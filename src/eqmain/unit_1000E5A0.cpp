// unit_1000E5A0.cpp - eqmain.dll, the single function at 0x1000E5A0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1000E5A0
// __cdecl, two arguments; the first is unused and the second goes to the client's _free (0x10033568).
void __cdecl FreeBlock_guess(int unused, void *block)
{
    EQ_CDECL(void, 0x10033568, (void *))(block);
}
