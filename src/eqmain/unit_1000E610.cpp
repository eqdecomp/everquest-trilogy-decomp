// unit_1000E610.cpp - eqmain.dll, the address run 0x1000E610..0x1000E620 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// eqmain.dll's own globals (this image's addresses, not eqgame.exe's).
#define g_eqmain_source_7e58_guess  EQ_GLOBAL(unsigned int, 0x10047E58)
#define g_eqmain_copy_cf08_guess    EQ_GLOBAL(unsigned int, 0x1004CF08)

// FUNCTION: EQMAIN 0x1000e610
// Thunk to a routine with unknown game purpose.
// __stdcall, no arguments. A bare thunk (one JMP) to CopyGlobalValue_guess at 0x1000E620, which copies the dword at
// 0x10047E58 to the one at 0x1004CF08.
void __stdcall thunk_FUN_1000e620(void)
{
    EQ_CDECL(void, 0x1000E620, (void))();
}

// FUNCTION: EQMAIN 0x1000E620
// __cdecl, void. Copies the dword at 0x10047E58 to the dword at 0x1004CF08. The meanings of both globals are unknown.
void CopyGlobalValue_guess(void)
{
    g_eqmain_copy_cf08_guess = g_eqmain_source_7e58_guess;
}
