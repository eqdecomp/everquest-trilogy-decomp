// eqmain: the same original object as trilogy's src/Network/rdp/ringbuf.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* ringbuf.c - a circular byte buffer (the serial link's receive buffer): allocate, fill from an overlapped
 * ReadFile, drain or peek.
 * ORIGINAL FILE: unknown. The Mac map names nothing after msg_outgoing.c (0x10030F30); these functions form
 * one object at 0x10031090..0x004F8216 (the queue's graph labels say list/rdp/serial).
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x10040244: the client's IAT slot for WINMM timeGetTime (the test DLL does not link winmm). */
typedef DWORD (__stdcall *RdpTimeGetTime_t)(void);
#define rdp_timeGetTime_iat_guess  EQ_GLOBAL(RdpTimeGetTime_t, 0x10040244)

/* 0x10040120, 0x10040104, 0x10040094: the client's IAT slots for ReadFile, GetOverlappedResult and GetLastError. */
typedef BOOL (__stdcall *RdpReadFile_t)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef BOOL (__stdcall *RdpGetOverlappedResult_t)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
typedef DWORD (__stdcall *RdpGetLastError_t)(void);
#define rdp_ReadFile_iat_guess             EQ_GLOBAL(RdpReadFile_t, 0x10040120)
#define rdp_GetOverlappedResult_iat_guess  EQ_GLOBAL(RdpGetOverlappedResult_t, 0x10040104)
#define rdp_GetLastError_iat_guess         EQ_GLOBAL(RdpGetLastError_t, 0x10040094)

// FUNCTION: EQMAIN 0x1002e240
// Release the ring buffer's allocated storage.
// Cloned from trilogy freeRingBufferMemory_guess 0x004F8030 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Frees the buffer (if any) and clears the pointer.
void freeRingBufferMemory_guess(RdpRing_guess *ring)
{
    if (ring->base != 0)
    {
        free(ring->base);
        ring->base = 0;
    }
}

// FUNCTION: EQMAIN 0x10031090
// Prepare an empty byte ring for incoming data.
// Cloned from trilogy initializeNetworkBufferHeader_guess 0x004F7FD0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. All five fields zero.
void initializeNetworkBufferHeader_guess(RdpRing_guess *ring)
{
    memset(ring, 0, sizeof(*ring));
}

// FUNCTION: EQMAIN 0x100310b0
// Provide storage for bytes queued in the ring buffer.
// Cloned from trilogy allocate_memory_buffer_guess 0x004F7FF0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. malloc(size) as the empty buffer; 2 when malloc fails (base is then NULL, size untouched), else 0.
int allocate_memory_buffer_guess(RdpRing_guess *ring, u32 size)
{
    u8 *mem;
    int rc = 0;

    mem = (u8 *)malloc(size);
    ring->base = mem;
    if (mem == 0)
        rc = 2;
    else
    {
        ring->wr = mem;
        ring->rd = mem;
        ring->size = size;
    }
    return rc;
}

// FUNCTION: EQMAIN 0x100310F0
// Read incoming data into the ring buffer for later use.
// @early-stop: 26 instruction differences remain in argument storage, GetLastError pointer handling, and block layout.
// Cloned from trilogy read_network_data_into_buffer_guess 0x004F8050 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Reads from the (overlapped) file into the free space until the buffer is full or a read comes back short.
// Quirks kept: after a ReadFile failure other than ERROR_IO_PENDING `got` keeps its old value (initially the ring
// pointer, as in the original's reused argument slot), the failed read is not retried, and timeGetTime and
// GetLastError are called and their results ignored.
void read_network_data_into_buffer_guess(RdpRing_guess *ring, void *file)
{
    u8 *wr;
    u32 room;
    OVERLAPPED ov;
    DWORD got;
    int done;

    got = (DWORD)ring;
    do
    {
        wr = ring->wr;
        if (wr < ring->rd)
            room = ring->rd - wr;
        else
        {
            room = (ring->base - wr) + ring->size;
            if (ring->count == ring->size)
                return;
        }
        if (room == 0)
            return;
        memset(&ov, 0, sizeof(ov));
        done = 1;
        if (rdp_ReadFile_iat_guess(file, wr, room, &got, &ov) == 0)
        {
            done = 0;
            if (rdp_GetLastError_iat_guess() == ERROR_IO_PENDING)
            {
                if (rdp_GetOverlappedResult_iat_guess(file, &ov, &got, 1) != 0)
                    done = 1;
                else
                    got = 0;
            }
            if (done == 0)
            {
                rdp_timeGetTime_iat_guess();
                rdp_GetLastError_iat_guess();
            }
        }
        if (done)
        {
            ring->wr = ring->wr + got;
            if (ring->wr == ring->base + ring->size)
                ring->wr = ring->base;
            ring->count += got;
        }
    } while (got >= room);
}

// FUNCTION: EQMAIN 0x100311C0
// Consume pending input bytes, copying them out when a caller needs the data.
// @early-stop: VC6 delays saving EDI and omits the original parameter home-store; the loop body matches.
// Cloned from trilogy read_data_from_circular_buffer_guess 0x004F8120 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Consumes `n` bytes, copying them to `dst` unless that is NULL (then they are just dropped). There is
// no check that n bytes are stored: with an empty buffer this does not end.
void read_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n)
{
    u32 chunk;
    u32 amount;
    u8 *out;
    u8 *rd;

    out = (u8 *)dst;
    while (n > 0)
    {
        rd = ring->rd;
        chunk = (u32)ring->wr;
        if (rd < ring->wr)
        {
            chunk -= (u32)rd;
        }
        else
        {
            chunk = (ring->base - rd) + ring->size;
            if (ring->count == 0)
                chunk = 0;
        }
        amount = chunk;
        if (n < chunk)
            amount = n;
        if (out != 0)
        {
            memcpy(out, ring->rd, amount);
            dst = (u8 *)dst + amount;
            out = (u8 *)dst;
        }
        n -= amount;
        ring->rd += amount;
        if (ring->rd == ring->base + ring->size)
            ring->rd = ring->base;
        ring->count -= amount;
    }
}

// FUNCTION: EQMAIN 0x10031250
// Peek at pending input bytes so a caller can inspect them before consuming.
// @early-stop: 9 instruction differences remain in saved-register order and destination-pointer allocation; the avail == n mutation is equivalent because the second copy is zero-length.
// Cloned from trilogy copy_data_from_circular_buffer_guess 0x004F81B0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Copies the next `n` bytes to `dst` without consuming them (wrapping to the start of the buffer
// when the stored bytes before the end are fewer than n). No check that n bytes are stored.
void copy_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n)
{
    u8 *src;
    void *out;
    u32 avail;

    out = dst;
    src = ring->rd;
    if (src < ring->wr)
        avail = ring->wr - src;
    else
    {
        avail = (ring->size - (u32)src) + (u32)ring->base;
        if (ring->count == 0)
            avail = 0;
    }
    if (avail < n)
    {
        out = (u8 *)memcpy(out, src, avail) + avail;
        src = ring->base;
        n -= avail;
    }
    memcpy(out, src, n);
}
