/* usemaphore.c - a counter guarded by a critical section.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/usemaphore.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "trilogy/rdp.h"

// CFLAGS: /O2

// FUNCTION: TRILOGY 0x004f70b0
// Updates shared state used while processing network connections.
// ORIGINAL FILE: Network/rdp/usemaphore.c (Mac: usemaphore_decrement, low; TAKP: updateNetworkTimestamp)
// __cdecl. Stores `value` (the dword after the critical section) under the lock. Despite the Mac-chain
// name nothing is decremented here.
void usemaphore_decrement_guess(CRITICAL_SECTION *cs, u32 value)
{
    EnterCriticalSection(cs);
    *(u32 *)(cs + 1) = value;
    LeaveCriticalSection(cs);
}
