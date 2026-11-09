/* ringbuf.c - a circular byte buffer (the serial link's receive buffer): allocate, fill from an overlapped
 * ReadFile, drain or peek.
 * ORIGINAL FILE: unknown. The Mac map names nothing after msg_outgoing.c (0x004F7EB0); these functions form
 * one object at 0x004F7FD0..0x004F8216 (the queue's graph labels say list/rdp/serial).
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x00534278: the client's IAT slot for WINMM timeGetTime (the test DLL does not link winmm). */
typedef DWORD (__stdcall *RdpTimeGetTime_t)(void);
#define rdp_timeGetTime_iat_guess  EQ_GLOBAL(RdpTimeGetTime_t, 0x00534278)

// FUNCTION: TRILOGY 0x004f7fd0
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
// __cdecl. All five fields zero.
void initializeNetworkBufferHeader_guess(RdpRing_guess *ring)
{
    memset(ring, 0, sizeof(*ring));
}

// FUNCTION: TRILOGY 0x004f7ff0
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
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

// FUNCTION: TRILOGY 0x004f8030
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
// __cdecl. Frees the buffer (if any) and clears the pointer.
void FreeRingBuffer_guess(RdpRing_guess *ring)
{
    if (ring->base != 0)
    {
        free(ring->base);
        ring->base = 0;
    }
}

// FUNCTION: TRILOGY 0x004f8050
// Collects incoming serial-link bytes in the receive buffer for later consumption.
// @early-stop: One extra byte-count initialization store; original reuses the incoming ring argument slot.
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
// __cdecl. Reads from the (overlapped) file into the free space until the buffer is full or a read comes
// back short. Quirks kept: when ReadFile fails with an error other than ERROR_IO_PENDING the byte count is
// whatever it was before (initially the ring pointer itself, the original reused its argument slot), the
// failed read is not retried via GetOverlappedResult, and timeGetTime / GetLastError are called and unused.
void read_network_data_into_buffer_guess(RdpRing_guess *ring, HANDLE file)
{
    u8 *wr;
    u32 room;
    OVERLAPPED ov;
    DWORD got;
    BOOL ok;

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
                break;
        }
        if (room == 0)
            break;
        memset(&ov, 0, sizeof(ov));
        ok = ReadFile(file, wr, room, &got, &ov);
        if (ok == 0 && GetLastError() == ERROR_IO_PENDING)
        {
            ok = GetOverlappedResult(file, &ov, &got, 1);
            if (ok == 0)
                got = 0;
        }
        if (ok != 0)
        {
            ring->wr = ring->wr + got;
            if (ring->wr == ring->base + ring->size)
                ring->wr = ring->base;
            ring->count += got;
        }
        else
        {
            rdp_timeGetTime_iat_guess();
            GetLastError();
        }
    } while (got >= room);
}

// FUNCTION: TRILOGY 0x004f8120
// Retire bytes from the serial link's pending receive data.
// @early-stop: the original saves dst's register (EDI) and spills it before the n test; ours loads it after.
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
// __cdecl. Consumes `n` bytes, copying them to `dst` unless that is NULL (then they are just dropped). There is
// no check that n bytes are stored: with an empty buffer this does not end.
/* fake match: avail is a temp copied into chunk before the clamp; without it VC6 merges them and swaps rd/wr. */
void read_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n)
{
    u32 chunk;
    u32 avail;

    while (n > 0)
    {
        if (ring->rd < ring->wr)
            avail = ring->wr - ring->rd;
        else
        {
            avail = (ring->base - ring->rd) + ring->size;
            if (ring->count == 0)
                avail = 0;
        }
        chunk = avail;
        if (n < chunk)
            chunk = n;
        if (dst != 0)
        {
            memcpy(dst, ring->rd, chunk);
            dst = (u8 *)dst + chunk;
        }
        n -= chunk;
        ring->rd += chunk;
        if (ring->rd == ring->base + ring->size)
            ring->rd = ring->base;
        ring->count -= chunk;
    }
}

// FUNCTION: TRILOGY 0x004f81b0
// Support serial receive parsing that examines pending data before advancing the stream.
// @early-stop: VC6 loads the working `out` pointer at the join and shrink-wraps EBP; the original homes both in the prologue.
// ORIGINAL FILE: unknown (ring buffer object after msg_outgoing.c)
// __cdecl. Copies the next `n` bytes to `dst` without consuming them (wrapping to the start of the buffer
// when the stored bytes before the end are fewer than n). No check that n bytes are stored.
void copy_data_from_circular_buffer_guess(RdpRing_guess *ring, void *dst, u32 n)
{
    u8 *src;
    u32 avail;
    u8 *out;

    src = ring->rd;
    if (src < ring->wr)
        avail = ring->wr - src;
    else
    {
        avail = (ring->size - (u32)src) + (u32)ring->base;
        if (ring->count == 0)
            avail = 0;
    }
    out = (u8 *)dst;
    if (avail < n)
    {
        memcpy(out, src, avail);
        src = ring->base;
        n -= avail;
        out += avail;
    }
    memcpy(out, src, n);
}
