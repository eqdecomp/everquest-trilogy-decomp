// unit_1003EA1C.cpp - eqmain.dll, the address run 0x1003EA1C..0x1003EA30 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// KERNEL32 LeaveCriticalSection, called through its IAT slot.
typedef void (__stdcall *LeaveCriticalSection_fn)(void *lock);

// FUNCTION: EQMAIN 0x1003EA1C
// Releases a shared data lock; its game-specific purpose is unclear.
// __stdcall, no stack arguments (the callee pops 0 bytes). When the dword at 0x1013CE80 is 2, leaves the
// critical section at 0x1013CE68.
void __stdcall leaveCriticalSectionIfState2_guess(void)
{
    if (EQ_GLOBAL(int, 0x1013CE80) == 2)
    {
        EQ_GLOBAL(LeaveCriticalSection_fn, 0x10040148)(EQ_ARRAY(void, 0x1013CE68));
    }
    return;
}
