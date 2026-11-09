/* bandwidth.c - a leaky bucket for the serial link: bytes drain at `rate` per second.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/bandwidth.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))

// FUNCTION: TRILOGY 0x004f7b30
// ORIGINAL FILE: Network/rdp/bandwidth.c (Mac: bandwidth_init, high)
// __cdecl. Empty, now, 3000 bytes/s.
void bandwidth_init_guess(RdpBandwidth_guess *bw)
{
    bw->bytes = 0;
    bw->time = rdp_timeGetTime_guess();
    bw->rate = 3000;
}

// FUNCTION: TRILOGY 0x004f7b50
// Accounts outgoing packet bytes against the serial link's bandwidth budget.
// ORIGINAL FILE: Network/rdp/bandwidth.c (address: between Mac-mapped neighbours)
// __cdecl. Drains the bucket up to now, then adds `bytes`. The drained amount is compared in 64 bits but
// subtracted as the 32-bit product rate * elapsed / 1000 (faithful).
void UpdateBandwidthQuota_guess(RdpBandwidth_guess *bw, u32 bytes)
{
    DWORD now;
    u32 elapsed;

    if (bw->bytes > 0)
    {
        now = rdp_timeGetTime_guess();
        elapsed = now - bw->time;
        if ((unsigned __int64)bw->rate * elapsed / 1000 >= bw->bytes)
            bw->bytes = 0;
        else
            bw->bytes -= bw->rate * elapsed / 1000;
        bw->time = now;
    }
    bw->bytes += bytes;
    bw->time = rdp_timeGetTime_guess();
}

// FUNCTION: TRILOGY 0x004f7bd0
// ORIGINAL FILE: Network/rdp/bandwidth.c (address: between Mac-mapped neighbours)
// __cdecl. Drains the bucket up to now and returns what is left.
u32 update_token_bucket_guess(RdpBandwidth_guess *bw)
{
    DWORD now;
    u32 elapsed;

    if (bw->bytes > 0)
    {
        now = rdp_timeGetTime_guess();
        elapsed = now - bw->time;
        if ((unsigned __int64)bw->rate * elapsed / 1000 >= bw->bytes)
            bw->bytes = 0;
        else
            bw->bytes -= bw->rate * elapsed / 1000;
        bw->time = now;
    }
    return bw->bytes;
}

// FUNCTION: TRILOGY 0x004f7c50
// ORIGINAL FILE: Network/rdp/bandwidth.c (Mac: bandwidth_get_time_empty, high)
// __cdecl. When the bucket will be empty (timeGetTime ms): last update + bytes * 1000 / rate (32-bit).
DWORD bandwidth_get_time_empty_guess(RdpBandwidth_guess *bw)
{
    return bw->bytes * 1000 / bw->rate + bw->time;
}
