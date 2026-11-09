// eqmain: the same original object as trilogy's src/Network/rdp/uevent.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* uevent.c - a thin wrapper over a Win32 auto-reset event.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/uevent.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "eqmain/rdp.h"

// FUNCTION: EQMAIN 0x1002e890
// Closes an event handle during network cleanup; its specific game purpose is unclear.
// Cloned from trilogy uevent_destroy_guess 0x004F7250 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Closes the event handle if there is one (the handle is left as is).
void uevent_destroy_guess(HANDLE *ev)
{
    if (*ev)
        CloseHandle(*ev);
}

// FUNCTION: EQMAIN 0x10030500
// Initializes an empty event handle for network state; its specific game purpose is unclear.
// Cloned from trilogy uevent_init_guess 0x004F7880 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl.
void uevent_init_guess(HANDLE *ev)
{
    *ev = 0;
}

// FUNCTION: EQMAIN 0x10030a00
// Signals an event used by network disconnect; its specific game purpose is unclear.
// Cloned from trilogy uevent_signal_guess 0x004F78B0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl.
void uevent_signal_guess(HANDLE *ev)
{
    SetEvent(*ev);
}
