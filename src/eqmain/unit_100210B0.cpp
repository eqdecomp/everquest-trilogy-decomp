// unit_100210B0.cpp - eqmain.dll, the address run 0x100210B0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x100210B0
// Calls two argument-less helpers in turn; the second is a tail jump in the original.
void __stdcall CallTwoHelpers_guess(void)
{
    EQ_STDCALL(void, 0x100210C0, (void))();
    EQ_STDCALL(void, 0x100210D0, (void))();
}
