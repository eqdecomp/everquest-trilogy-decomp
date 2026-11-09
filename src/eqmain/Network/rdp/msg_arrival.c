// eqmain: the same original object as trilogy's src/Network/rdp/msg_arrival.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
// CFLAGS: /O2
/* msg_arrival.c - an incoming message reassembled from its fragments.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/msg_arrival.c (from Mac debug map)
 *
 * The Mac map names only the four getters at 0x004F4C80..; the functions from 0x004F4B20 sit between
 * fast_malloc.c (fast_free 0x10028A90) and those getters, so they belong to this object too (the queue's
 * graph labels say rdp/list/rx).
 * C. The rdp code was built for speed with intrinsics (see bitops.c).
 */
#include <string.h>

#include "eqmain/rdp.h"

// FUNCTION: EQMAIN 0x1002de30
// Checks incoming message fragments against the first fragment's count and sequence.
// Cloned from trilogy ValidateFragmentConsistency_guess 0x004F4C40 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. 2 when a fragment doesn't fit a message that already has fragment 0 (other fragment count, or
// its sequence number isn't first_frag_seq + index, 16-bit), else 0.
int ValidateFragmentConsistency_guess(RdpMsgArrival_guess *msg, RdpPacketInfo_guess *info)
{
    int result;

    result = 0;
    if (msg->sender)
    {
        if (msg->frag_count != info->frag_count
            || (u16)(msg->first_frag_seq + info->frag_index) != info->frag_seq)
            result = 2;
    }
    return result;
}

// FUNCTION: EQMAIN 0x1002de70
// Supplies received message data to the network layer when available.
// Cloned from trilogy msg_arrival_get_data_guess 0x004F4C90 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. The payload, or NULL while the size is 0.
void *msg_arrival_get_data_guess(RdpMsgArrival_guess *msg)
{
    if (msg->size)
        return msg->data;
    return 0;
}
