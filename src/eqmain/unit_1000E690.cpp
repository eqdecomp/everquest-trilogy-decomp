// unit_1000E690.cpp - eqmain.dll, the address run 0x1000E690 (a queue unit: no Mac map covers eqmain.dll, so the
// original object and its name are unknown).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1000e690
// Runs an initialization sequence; its in-game purpose is unclear.
// __stdcall, no arguments. Calls GetFlagValues_guess (0x1000E6A0), then register_exit_guess (0x1000E6C0), which the
// original reaches with a tail JMP. Nothing is returned.
void __stdcall CallFunctionsInSequence_guess(void)
{
    EQ_STDCALL(void, 0x1000E6A0, (void))();
    EQ_STDCALL(void, 0x1000E6C0, (void))();
}
