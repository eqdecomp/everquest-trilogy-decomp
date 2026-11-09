// eqmain: the same original object as trilogy's src/Network/rdp/eventq.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* eventq.c - the serial link's outgoing queue.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/eventq.c (from Mac debug map)
 *
 * The Mac map names only 0x004F7180 here; 0x100303F0..0x10030440 (between usemaphore.c and it) work on the
 * same queue and are placed here too (the queue's graph labels say rdp).
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]); called directly, like the original. */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))

// FUNCTION: EQMAIN 0x100303f0
// Returns the serial queue's stored value; what that value represents is unclear.
// Cloned from trilogy getDebugInfoFromCriticalSection_guess 0x004F70D0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. The guarded value (see usemaphore_decrement_guess), read under the lock.
u32 getDebugInfoFromCriticalSection_guess(RdpSerialQueue_guess *q)
{
    u32 value;

    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    value = q->value;
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    return value;
}

// FUNCTION: EQMAIN 0x10030410
// Checks whether the connection has fewer than two items in its serial queue.
// Cloned from trilogy is_connection_inactive_guess 0x004F70F0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. 1 when fewer than two items are queued.
int is_connection_inactive_guess(RdpSerialQueue_guess *q)
{
    u32 count;

    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    count = q->list.count;
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    return count < 2;
}
