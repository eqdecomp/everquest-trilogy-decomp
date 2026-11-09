// unit_1001B1E0.cpp - eqmain.dll, the address run 0x1001B1E0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1001B1E0
// Runs the two no-argument helpers at 0x1001B1F0 and 0x1001B200 in turn. __stdcall, no arguments.
void __stdcall FUN_1001b1e0(void)
{
    EQ_STDCALL(void, 0x1001B1F0, (void))();
    EQ_STDCALL(void, 0x1001B200, (void))();
}
