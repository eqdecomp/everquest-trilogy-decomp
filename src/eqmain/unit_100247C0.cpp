// unit_100247C0.cpp - eqmain.dll, the single function at 0x100247C0 (a queue unit: no Mac map covers eqmain.dll, so
// the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

// The device counter that the callback at 0x100247C0 bumps.
#define g_eqmain_device_count_guess EQ_GLOBAL(int, 0x101258FC)

// FUNCTION: EQMAIN 0x100247C0
// Enumeration callback: adds 1 to the counter at 0x101258FC on each call and returns 1 to continue.
// __stdcall, four arguments (RET 0x10), none read, returns 1. Adds 1 to the counter at 0x101258FC. Its address is taken
// by EnumerateDirectDraw_guess, so it looks like an enumeration callback (e.g. DirectDrawEnumerate's: guid, description, name, context).
int __stdcall IncrementCallbackCount_guess(void *guid, char *description, char *name, void *context)
{
    g_eqmain_device_count_guess = g_eqmain_device_count_guess + 1;
    return 1;
}
