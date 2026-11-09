/* uevent.c - a thin wrapper over a Win32 auto-reset event.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/uevent.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "trilogy/rdp.h"

// CFLAGS: /O2

// FUNCTION: TRILOGY 0x004f7250
// ORIGINAL FILE: Network/rdp/uevent.c (Mac: uevent_destroy, high)
// __cdecl. Closes the event handle if there is one (the handle is left as is).
void uevent_destroy_guess(HANDLE *ev)
{
    if (*ev)
        CloseHandle(*ev);
}

// FUNCTION: TRILOGY 0x004f7880
// ORIGINAL FILE: Network/rdp/uevent.c (Mac: uevent_init, high)
// __cdecl.
void uevent_init_guess(HANDLE *ev)
{
    *ev = 0;
}

// FUNCTION: TRILOGY 0x004f7890
// ORIGINAL FILE: Network/rdp/uevent.c (Mac: uevent_create, high)
// __cdecl. An unnamed auto-reset event, not signalled. 0, or 3 when CreateEvent fails.
int uevent_create(HANDLE *ev)
{
    *ev = CreateEventA(0, FALSE, FALSE, 0);
    return *ev ? 0 : 3;
}

// FUNCTION: TRILOGY 0x004f78b0
// ORIGINAL FILE: Network/rdp/uevent.c (Mac: uevent_signal, high)
// __cdecl.
void uevent_signal_guess(HANDLE *ev)
{
    SetEvent(*ev);
}

// FUNCTION: TRILOGY 0x004f78c0
// ORIGINAL FILE: Network/rdp/uevent.c (Mac: uevent_wait, high)
// __cdecl. Waits forever (not alertable).
void uevent_wait_guess(HANDLE *ev)
{
    WaitForSingleObjectEx(*ev, INFINITE, FALSE);
}
