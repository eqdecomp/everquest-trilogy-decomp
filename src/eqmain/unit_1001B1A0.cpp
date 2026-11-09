#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1001B1A0
// Invokes two front-end routines; their game-facing purpose is unclear.
void CallTwoRoutines_guess(void)
{
    EQ_STDCALL(void, 0x1001B1B0, (void))();
    EQ_STDCALL(void, 0x1001B1C0, (void))();
}
