/* msg_arrival.c - an incoming message reassembled from its fragments.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/msg_arrival.c (from Mac debug map)
 *
 * The Mac map names only the four getters at 0x004F4C80..; the functions from 0x004F4B20 sit between
 * fast_malloc.c (fast_free 0x004F4AC0) and those getters, so they belong to this object too (the queue's
 * graph labels say rdp/list/rx).
 * C. The rdp code was built for speed with intrinsics (see bitops.c).
 */
#include <string.h>

#include "trilogy/rdp.h"

// CFLAGS: /O2

// FUNCTION: TRILOGY 0x004f4b20
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
// __cdecl. Prepares a message as a list link keyed by its msgid; no sender, no fragments yet.
void initializeMessageHeader_guess(RdpMsgArrival_guess *msg, u16 msgid)
{
    msg->link.data = msg;
    msg->link.key = &msg->msgid;
    msg->frags_received = 0;
    msg->msgid = msgid;
    msg->sender = 0;
}

// FUNCTION: TRILOGY 0x004f4b50
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
// __cdecl. Makes the link key point at the message's channel_seq (what the channel queue's compare reads).
void initializeChannelSequenceKey_guess(RdpMsgArrival_guess *msg)
{
    msg->link.key = &msg->channel_seq;
}

// FUNCTION: TRILOGY 0x004f4b60
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
// __cdecl. Clears the link key.
void clearMessageContext_guess(RdpMsgArrival_guess *msg)
{
    msg->link.key = 0;
}

// FUNCTION: TRILOGY 0x004f4b70
// Reassembles an incoming network message as its fragments arrive.
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
// __cdecl. Adds one fragment: fragment 0 also sets the message's header fields and sender. Copies the
// payload to its slot, sets the size when the last fragment (by index) arrives, and returns whether all
// fragments are in (frags_received == frag_count). No bounds checks.
int process_network_message_packet_guess(RdpMsgArrival_guess *msg, void *sender, RdpPacketInfo_guess *info,
                                         const void *data)
{
    if (info->frag_index == 0)
    {
        msg->flags = info->flags;
        msg->seq = info->seq;
        msg->first_frag_seq = info->frag_seq;
        msg->frag_count = info->frag_count;
        msg->channel = info->channel;
        msg->channel_seq = info->channel_seq;
        msg->sender = sender;
    }
    msg->frags_received++;
    memcpy(msg->data + info->frag_index * RDP_FRAGMENT_SIZE_guess, data, info->data_len);
    if (info->frag_index + 1 == info->frag_count)
        msg->size = info->frag_index * RDP_FRAGMENT_SIZE_guess + info->data_len;
    return info->frag_count == msg->frags_received;
}

// FUNCTION: TRILOGY 0x004f4c20
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
// __cdecl. Zeroes the 0x3C-byte header and makes the message its own link data, with `sender`.
void initialize_message_node_guess(RdpMsgArrival_guess *msg, void *sender)
{
    memset(msg, 0, 0x3c);
    msg->link.key = 0;
    msg->link.data = msg;
    msg->sender = sender;
}

// FUNCTION: TRILOGY 0x004f4c40
// Confirms an arriving fragment belongs to the same incoming message assembly.
// ORIGINAL FILE: Network/rdp/msg_arrival.c (address: between Mac-mapped neighbours)
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

// FUNCTION: TRILOGY 0x004f4c80
// ORIGINAL FILE: Network/rdp/msg_arrival.c (Mac: msg_arrival_get_size, high)
// __cdecl.
u32 msg_arrival_get_size_guess(RdpMsgArrival_guess *msg)
{
    return msg->size;
}

// FUNCTION: TRILOGY 0x004f4c90
// ORIGINAL FILE: Network/rdp/msg_arrival.c (Mac: msg_arrival_get_data, high)
// __cdecl. The payload, or NULL while the size is 0.
void *msg_arrival_get_data(RdpMsgArrival_guess *msg)
{
    if (msg->size)
        return msg->data;
    return 0;
}

// FUNCTION: TRILOGY 0x004f4cb0
// ORIGINAL FILE: Network/rdp/msg_arrival.c (Mac: msg_arrival_get_sender, high)
// __cdecl.
void *msg_arrival_get_sender_guess(RdpMsgArrival_guess *msg)
{
    return msg->sender;
}

// FUNCTION: TRILOGY 0x004f4cc0
// ORIGINAL FILE: Network/rdp/msg_arrival.c (Mac: msg_arrival_has_fin, high)
// __cdecl. The fin flag (0x4000) of fragment 0.
int msg_arrival_has_fin_guess(RdpMsgArrival_guess *msg)
{
    return (msg->flags >> 14) & 1;
}
