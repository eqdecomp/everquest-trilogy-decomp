// eqmain: the same original object as trilogy's src/Network/rdp/bandwidth.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* bandwidth.c - a leaky bucket for the serial link: bytes drain at `rate` per second.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/bandwidth.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))

// FUNCTION: EQMAIN 0x10030c10
// Initializes per-connection network bandwidth tracking at 3000 bytes per second.
// Cloned from trilogy bandwidth_init_guess 0x004F7B30 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Empty, now, 3000 bytes/s.
void bandwidth_init_guess(RdpBandwidth_guess *bw)
{
    bw->bytes = 0;
    bw->time = rdp_timeGetTime_guess();
    bw->rate = 3000;
}

// FUNCTION: EQMAIN 0x10030c30
// Accounts newly transferred network data against the connection's bandwidth limit.
// ORIGINAL FILE: Network/rdp/bandwidth.c (inferred from the neighbours; no Mac map entry)
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

// FUNCTION: EQMAIN 0x10030cb0
// Reports the connection's outstanding bandwidth use after accounting for elapsed time.
// ORIGINAL FILE: Network/rdp/bandwidth.c (inferred from the neighbours; no Mac map entry)
// __cdecl. Drains the bucket up to now and returns what is left. The drained amount is compared in 64 bits
// but subtracted as the 32-bit product rate * elapsed / 1000 (faithful).
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

// FUNCTION: EQMAIN 0x10030d30
// Estimates when the connection's outstanding bandwidth use will clear.
// Cloned from trilogy bandwidth_get_time_empty_guess 0x004F7C50 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. When the bucket will be empty (timeGetTime ms): last update + bytes * 1000 / rate (32-bit).
DWORD bandwidth_get_time_empty_guess(RdpBandwidth_guess *bw)
{
    return bw->bytes * 1000 / bw->rate + bw->time;
}
