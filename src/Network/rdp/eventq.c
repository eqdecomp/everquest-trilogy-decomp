/* eventq.c - the serial link's outgoing queue.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/eventq.c (from Mac debug map)
 *
 * The Mac map names only 0x004F7180 here; 0x004F70D0..0x004F7120 (between usemaphore.c and it) work on the
 * same queue and are placed here too (the queue's graph labels say rdp).
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]); called directly, like the original. */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))

// FUNCTION: TRILOGY 0x004f70d0
// ORIGINAL FILE: Network/rdp/eventq.c (address: between usemaphore.c and eventq.c)
// __cdecl. The guarded value (see usemaphore_decrement_guess), read under the lock.
u32 GetSerialQueueValue_guess(RdpSerialQueue_guess *q)
{
    u32 value;

    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    value = q->value;
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    return value;
}

// FUNCTION: TRILOGY 0x004f70f0
// ORIGINAL FILE: Network/rdp/eventq.c (address: between usemaphore.c and eventq.c)
// __cdecl. 1 when fewer than two items are queued.
int is_connection_inactive_guess(RdpSerialQueue_guess *q)
{
    u32 count;

    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    count = q->list.count;
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    return count < 2;
}

// FUNCTION: TRILOGY 0x004f7120
// Estimates when the outbound serial data behind the oldest item will finish sending.
// ORIGINAL FILE: Network/rdp/eventq.c (address: between usemaphore.c and eventq.c)
// __cdecl. When the queue will have drained: now + the bytes queued behind the oldest item at 57600 baud
// (9 bits per byte). Faithful quirks: an empty list with queued bytes reads item 0 (NULL + 0x24), and
// timeGetTime is called twice (the first result unused).
DWORD calculateAdjustedClientTime_guess(RdpSerialQueue_guess *q)
{
    u32 bytes;
    RdpSerialItem_guess *item;

    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    bytes = q->queued_bytes;
    if (bytes > 0)
    {
        item = q->list.head ? (RdpSerialItem_guess *)q->list.head->data : 0;
        bytes -= item->size;
    }
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    rdp_timeGetTime_guess();
    return rdp_timeGetTime_guess() + bytes * 9000 / 57600;
}

// FUNCTION: TRILOGY 0x004f7180
// Measures how long the oldest outbound serial item has exceeded its expected delivery time.
// ORIGINAL FILE: Network/rdp/eventq.c (Mac: ascending_timeout_data_cmp, low; TAKP: calculate_connection_latency)
// __cdecl. How long (ms) the oldest queued item has been stalled: its age beyond twice the time its `size`
// bytes take on a 57600-baud serial line (9 bits per byte), or 0. Used by rdp_get_serial_stall_time.
// The Mac-chain name doesn't fit what it does.
DWORD GetSerialQueueStallTime_guess(RdpSerialQueue_guess *q)
{
    RdpSerialItem_guess *item;
    u32 limit;
    DWORD age;

    age = 0;
    EnterCriticalSection((CRITICAL_SECTION *)q->lock);
    if (q->list.head && (item = (RdpSerialItem_guess *)q->list.head->data) != 0)
    {
        limit = item->size * 9000 / 57600 * 2;
        age = rdp_timeGetTime_guess() - item->time;
        if (age > limit)
        {
            LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
            return age - limit;
        }
        age = 0;
    }
    LeaveCriticalSection((CRITICAL_SECTION *)q->lock);
    return age;
}
