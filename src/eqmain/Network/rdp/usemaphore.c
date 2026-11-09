// eqmain: the same original object as trilogy's src/Network/rdp/usemaphore.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* usemaphore.c - a counter guarded by a critical section.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/usemaphore.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "eqmain/rdp.h"

// FUNCTION: EQMAIN 0x100303d0
// Stores a caller-supplied value alongside a critical section; its game purpose is unclear.
// Cloned from trilogy usemaphore_decrement_guess 0x004F70B0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Stores `value` (the dword after the critical section) under the lock. Despite the Mac-chain
// name nothing is decremented here.
void usemaphore_decrement_guess(CRITICAL_SECTION *cs, u32 value)
{
    EnterCriticalSection(cs);
    *(u32 *)(cs + 1) = value;
    LeaveCriticalSection(cs);
}
