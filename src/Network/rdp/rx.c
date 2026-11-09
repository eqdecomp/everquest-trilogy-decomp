/* rx.c - the receiving side of a connection: sequence checks, fragment reassembly, in-order delivery
 * per channel.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/rx.c (from Mac debug map)
 *
 * The functions from 0x004F5F70 to 0x004F6350 follow rx_validate_stream_arrival (the last one the Mac map
 * names, 0x004F5EF0) up to cypher.c and work on the same per-channel state; they are placed here by
 * address (the queue's graph labels say rdp/list/bitops).
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))
/* 0x005003B0: the import thunk for WSOCK32 htons. */
#define rdp_htons_guess        EQ_STDCALL(u16, 0x005003B0, (u16 n))
/* 0x0051CF90: an empty function (ret) the client calls on lists it is done with (a destructor that does nothing). */
#define rdp_no_op_guess        EQ_CDECL(void, 0x0051CF90, (void *p))

// FUNCTION: TRILOGY 0x004f5700
// Prepare a connection to receive a fresh stream of game messages.
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_init, low)
// __cdecl. Resets everything the receiving side keeps: counters, the ack window, the message lists, the
// channel queues and the heartbeat log fields.
void rx_init_guess(RdpConnection_guess *conn)
{
    RdpList_guess *q;
    int i;

    memset(conn->stat, 0, sizeof(conn->stat));
    conn->rx_base = 0;
    conn->rx_max = 0;
    bitarray_clear_guess(&conn->rx_ack_bits);
    conn->ack_count = 0;
    /* The timer thunk takes no arguments, so cdecl and stdcall have the same
     * ABI here. The cdecl view preserves the original's deferred stack cleanup. */
    conn->rx_last_activity = ((DWORD (__cdecl *)(void))rdp_timeGetTime_guess)();
    conn->rx_last_arrival = ((DWORD (__cdecl *)(void))rdp_timeGetTime_guess)();
    list_init_guess(&conn->msg_list);
    conn->recv_mask = (RdpU64_guess)-1;
    conn->chan_base_seq = 0;
    conn->msgid_valid = 0;
    conn->pending_flag = 0;
    conn->pending = 0;
    conn->recv_seq = 0xffff;
    memset(conn->chan_order, 0, sizeof(conn->chan_order));
    memset(conn->chan_seq, 0, sizeof(conn->chan_seq));
    q = conn->chan_queue;
    i = 20;
    do
    {
        list_init_guess(q);
        q++;
        --i;
    } while (i != 0);
    conn->ttl_log = 0;
    conn->field_7C4 = 0;
    conn->field_7C8 = ((DWORD (__cdecl *)(void))rdp_timeGetTime_guess)();
    conn->ttl_value = 1;
    conn->ttl_max = 30;
    conn->field_7DC = 0;
    conn->ttl_index = 0;
    conn->field_498 = 0;
    conn->field_49C = 0;
    conn->field_17C = 0;
}

// FUNCTION: TRILOGY 0x004f5820
// Prepares a connection's incoming messages and channel queues for ordered delivery.
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_create, low)
// __cdecl. Makes the message list and the 20 channel queues sorted lists (by the client's compare functions
// at 0x004F7F80 and 0x004F7FB0). Returns 0.
int rx_create_guess(RdpConnection_guess *conn)
{
    RdpList_guess *q;
    int i;

    setLinkedListProperties_guess(&conn->msg_list, 1, (RdpListCompare_guess)0x004f7f80);
    q = conn->chan_queue;
    i = 20;
    do
    {
        setLinkedListProperties_guess(q, 1, (RdpListCompare_guess)0x004f7fb0);
        q++;
        i--;
    } while (i != 0);
    return 0;
}

// FUNCTION: TRILOGY 0x004f5870
// Release buffered incoming game messages and heartbeat history during connection cleanup.
// ORIGINAL FILE: Network/rdp/rx.c (address: between Mac-mapped neighbours)
// __cdecl. Frees the pending message, every message being reassembled, every queued message of every channel
// and the heartbeat log.
void NetworkConnection_FreeQueuedItems_guess(RdpConnection_guess *conn)
{
    void *item;
    RdpList_guess *list;
    RdpList_guess *q;
    int i;

    if (conn->pending != 0)
    {
        fast_free_guess(conn->pending);
        conn->pending = 0;
    }
    list = &conn->msg_list;
    item = list_remove_head_guess(list);
    while (item != 0)
    {
        fast_free_guess(item);
        item = list_remove_head_guess(list);
    }
    q = conn->chan_queue;
    i = 20;
    do
    {
        item = list_remove_head_guess(q);
        while (item != 0)
        {
            fast_free_guess(item);
            item = list_remove_head_guess(q);
        }
        q++;
        i--;
    } while (i != 0);
    rdp_no_op_guess(list);
    q = conn->chan_queue;
    i = 20;
    do
    {
        rdp_no_op_guess(q);
        q++;
        i--;
    } while (i != 0);
    if (conn->ttl_log != 0)
    {
        free(conn->ttl_log);
        conn->ttl_log = 0;
    }
}

// FUNCTION: TRILOGY 0x004f5940
// ORIGINAL FILE: Network/rdp/rx.c (address: between Mac-mapped neighbours)
// __cdecl. As NetworkConnection_FreeQueuedItems_guess, without the empty calls and the heartbeat log.
void cleanup_connection_queues_guess(RdpConnection_guess *conn)
{
    void *item;
    RdpList_guess *q;
    int i;

    if (conn->pending != 0)
    {
        fast_free_guess(conn->pending);
        conn->pending = 0;
    }
    item = list_remove_head_guess(&conn->msg_list);
    while (item != 0)
    {
        fast_free_guess(item);
        item = list_remove_head_guess(&conn->msg_list);
    }
    q = conn->chan_queue;
    i = 20;
    do
    {
        item = list_remove_head_guess(q);
        while (item != 0)
        {
            fast_free_guess(item);
            item = list_remove_head_guess(q);
        }
        q++;
        i--;
    } while (i != 0);
}

// FUNCTION: TRILOGY 0x004f59c0
// ORIGINAL FILE: Network/rdp/rx.c (address: between Mac-mapped neighbours)
// __cdecl. Stamps the connection as active now.
void NetworkConnection_update_connection_last_activity_time_guess(RdpConnection_guess *conn)
{
    conn->rx_last_activity = rdp_timeGetTime_guess();
}

// FUNCTION: TRILOGY 0x004f59d0
// Track recent packet arrivals and traffic totals, keeping channel sequences within range.
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_record_seqnum_arrival, low)
// __cdecl. Notes the arrival of packet info->seq: a packet behind the newest in-order one (16-bit signed
// difference) sets its bit in the 64-packet mask and counts as late; a newer one shifts the mask (all clear when
// it is more than 64 ahead) and becomes recv_seq. Counts packets and bytes (data_len + overhead) in the global and
// the connection counters. A packet more than 16000 ahead of chan_base_seq resets the channel sequence numbers.
void rx_record_seqnum_arrival_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    u16 diff;
    short i;
    u16 *p;

    diff = info->seq - conn->recv_seq;
    if ((short)diff < 0)
    {
        conn->recv_mask |= (__int64)(1 << (-(char)diff - 1U));
        RDP_STAT_guess(0xB8)++;
        RDP_STAT_guess(0xC0) += info->data_len + info->overhead;
        RDP_CSTAT_guess(conn, 0xC4)++;
        RDP_CSTAT_guess(conn, 0xC8) += info->data_len + info->overhead;
    }
    else
    {
        RDP_STAT_guess(0xA8)++;
        RDP_STAT_guess(0xB0) += info->data_len + info->overhead;
        RDP_CSTAT_guess(conn, 0xBC)++;
        RDP_CSTAT_guess(conn, 0xC0) += info->data_len + info->overhead;
        if (diff > 0x40)
            conn->recv_mask = 0;
        else
        {
            conn->recv_mask = (conn->recv_mask << 1) | 1;
            diff--;
            conn->recv_mask = conn->recv_mask << (short)diff;
        }
        conn->recv_seq = info->seq;
    }
    if ((short)(info->seq - conn->chan_base_seq) > 16000)
    {
        p = conn->chan_seq;
        conn->chan_base_seq = info->seq - 1;
        i = 20;
        do
        {
            *p = conn->chan_base_seq;
            p++;
            i--;
        } while (i != 0);
    }
}

// FUNCTION: TRILOGY 0x004f5ba0
// Filters duplicate or out-of-window packets before they enter the receive stream.
// @early-stop: Counter increments still use different registers and immediates; final returns share an epilogue.
// ORIGINAL FILE: Network/rdp/rx.c (address: between Mac-mapped neighbours)
// __cdecl. 1 when packet `seq` should be dropped: a duplicate (the newest packet again, or one whose bit is
// set in the mask) or one outside the window (more than 64 behind or more than 0x1000 ahead); else 0. Counts
// the drop.
int NetworkConnection_ValidatePacketSequenceNumber_guess(RdpConnection_guess *conn, short seq)
{
    short diff;
    int rc = 0;
    RdpU64_guess mask_bit = 1;

    diff = seq - conn->recv_seq;
    if (diff <= (short)rc)
    {
        if (diff == 0 || (diff >= -0x40 && ((mask_bit << (-1 - diff)) & conn->recv_mask) != 0))
        {
            RDP_STAT_guess(0xE8)++;
            RDP_CSTAT_guess(conn, 0xD4)++;
            return 1;
        }
    }
    if ((short)(diff + 0x40) < 0 || (short)(diff - 0x1000) > 0)
    {
        RDP_STAT_guess(0xE0)++;
        RDP_CSTAT_guess(conn, 0xD0)++;
        rc = 1;
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004f5c70
// Rejects message fragments whose message IDs fall outside the connection's receive window.
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_validate_msgid_arrival, medium)
// __cdecl. Once the first packet has set the window (msgid_valid): a packet 0x78 or more ahead of rx_base, or a
// SYN one (0x2000) whose number isn't the first one, is logged and counted; returns 1 for that, else 0.
int rx_validate_msgid_arrival(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    const char *syn;
    int rc;
    {
        char addr[64];
    
        rc = 0;
        if (conn->msgid_valid != 0)
        {
            if ((short)(info->frag_seq - conn->rx_base - 0x78) >= 0
                || (0 != (0x2000 & info->flags) && info->frag_seq != conn->first_msgid))
            {
                unsigned short temp;
                rc = 1;
                format_sockaddr(addr, &conn->remote);
                if (0 != (0x2000 & info->flags))
                    syn = "SYN ";
                else
                    syn = "";
                temp = info->frag_seq;
                discard_log_append("%s bad msgid %i=%u-%u-%u, %s%u\n", addr, (int)(short)(temp - conn->rx_base),
                                         (u32)temp, (u32)conn->rx_base, 0x78, syn, (u32)conn->first_msgid);
                RDP_STAT_guess(0xF0)++;
                RDP_CSTAT_guess(conn, 0xD8)++;
            }
        }
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004f5d70
// Checks that incoming fragments fit a valid message before reassembly.
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_validate_fragment_arrival, high)
// __cdecl. 2 (logged, counted) when the fragment numbers are impossible (index not below a count above 1, or
// neither the last fragment nor a full 0x200 bytes); 0 or 2 from ValidateFragmentConsistency when the message
// already exists (the mismatch is logged and counted too).
int rx_validate_fragment_arrival(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    u16 index;
    u16 count;
    int rc;
    RdpLink_guess *link;
    RdpMsgArrival_guess *msg;
    char addr[64];

    rc = 0;
    index = info->frag_index;
    count = info->frag_count;
    if (index < count && count >= 2 && (index + 1 == count || info->data_len == 0x200))
    {
        link = list_find_link_by_key_guess(&conn->msg_list, &info->msgid);
        if (link != 0 && (msg = (RdpMsgArrival_guess *)link->data) != 0)
        {
            rc = ValidateFragmentConsistency_guess(msg, info);
            if (rc != 0)
            {
                format_sockaddr(addr, &conn->remote);
                discard_log_append("%s bad fragment size fragid:%u part:%u/%u msgid:%u a_total:%u a_msgid:%u\n",
                                         addr, info->msgid, info->frag_index, info->frag_count, info->frag_seq,
                                         msg->frag_count, msg->first_frag_seq);
                RDP_STAT_guess(0x110)++;
                RDP_CSTAT_guess(conn, 0xE8)++;
            }
        }
    }
    else
    {
        rc = 2;
        format_sockaddr(addr, &conn->remote);
        discard_log_append("%s bad fragment: %u %u %u %u %u\n", addr, info->frag_index, info->frag_count, 0x200,
                                 info->data_len);
        RDP_STAT_guess(0xF8)++;
        RDP_CSTAT_guess(conn, 0xDC)++;
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004f5ef0
// ORIGINAL FILE: Network/rdp/rx.c (Mac: rx_validate_stream_arrival, high)
// __cdecl. 2 (logged, counted) when the channel number is 20 or more, else 0.
int rx_validate_stream_arrival(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    char addr[64];
    int rc;

    rc = 0;
    if (info->channel >= 0x14)
    {
        rc = 2;
        format_sockaddr(addr, &conn->remote);
        discard_log_append("%s bad stream id: %u >= %u\n", addr, info->channel, 0x14);
        RDP_STAT_guess(0x100)++;
        RDP_CSTAT_guess(conn, 0xE0)++;
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004f5f70
// Track received packet sequences for reliable delivery, detecting repeats and advancing past filled gaps.
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Records the arrival of packet `seq` in the ack window (the first packet starts the window); the run
// of consecutive arrivals at the front moves rx_base forward. 1 when the packet had already arrived (or lies at
// or behind rx_base).
int NetworkConnection_process_sequence_number_guess(RdpConnection_guess *conn, u16 seq)
{
    u8 *bits;
    int prev = 0;
    u16 run;

    conn->rx_last_arrival = rdp_timeGetTime_guess();
    if (conn->msgid_valid == 0)
    {
        conn->msgid_valid = 1;
        conn->first_msgid = seq;
        conn->rx_base = seq - 1;
        conn->rx_max = seq - 1;
    }
    if (conn->ack_count == 0)
    {
        conn->ack_max = seq;
        conn->ack_min = seq;
    }
    else if (seq < conn->ack_min)
        conn->ack_min = seq;
    else if (seq > conn->ack_max)
        conn->ack_max = seq;
    conn->ack_count++;
    if ((short)(conn->rx_base - seq) < 0)
    {
        if (0 < (short)(seq - conn->rx_max))
            conn->rx_max = seq;
        /* The original masks the bit index to 16 bits after subtracting: required when seq wraps. */
        run = seq - conn->rx_base - 1;
        bits = conn->rx_ack_bits.bits;
        if (bitarray_setbit(bits, run) != 0)
            prev = 1;
        run = 0;
        while (getbit_guess(bits, run) != 0)
            run++;
        if (run > (unsigned int)0)
        {
            copyBitfieldToBuffer_guess(bits, bits, run, 0x100);
            conn->rx_base += run;
        }
    }
    else
        prev = 1;
    return prev;
}

// FUNCTION: TRILOGY 0x004f60a0
// Tell the remote peer which incoming packets have arrived through acknowledgements in outgoing traffic.
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Builds the acknowledgement part of an outgoing packet header into `out` (a number, then a bit mask
// of up to 15 bytes) and sets the ack flag bits in *flags (0x04: everything up to rx_base, 0x08: from ack_min on
// with a mask, the mask length in 0x00F0). Returns the bytes written (0 when nothing needs acknowledging) and
// clears ack_count.
int NetworkConnection_pPrepareAcknowledgementHeader_guess(RdpConnection_guess *conn, u16 *out, u16 *flags)
{
    short t;
    short delta;
    int ret;
    u16 n;

    ret = 0;
    if (conn->ack_count > (unsigned int)0)
    {
        if ((short)conn->ack_min - conn->rx_base > 8)
        {
            *out++ = rdp_htons_guess(conn->ack_min);
            *flags |= 8;
            ret = 2;
        }
        else
        {
            *out++ = rdp_htons_guess(conn->rx_base);
            *flags |= 4;
            t = conn->rx_base;
            delta = t - conn->ack_max;
            ret = 2;
            conn->ack_min = t;
            if (0 < delta)
                conn->ack_max = t;
        }
        n = (int)((u16)(conn->ack_max - conn->ack_min) + 7) >> 3;
        if (n >= 0xf)
            n = 0xf;
        if (n > (unsigned int)0)
        {
            *flags |= (u16)(n << 4);
            copyBitfieldToBuffer_guess(conn->rx_ack_bits.bits, (u8 *)out,
                                      (u32)conn->ack_min - (u32)conn->rx_base, n);
            ret = n + 2;
        }
        conn->ack_count = 0;
    }
    return ret;
}

// FUNCTION: TRILOGY 0x004f61b0
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Queues a message on its channel's sorted queue (keyed by its channel_seq).
void add_message_to_sorted_queue_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg)
{
    initializeChannelSequenceKey_guess(msg);
    LinkedList_insertNodeIntoSortedLinkedList_guess(&conn->chan_queue[msg->channel], &msg->link);
}

// FUNCTION: TRILOGY 0x004f61e0
// Delivers a channel's next queued message when its delivery turn arrives.
// @early-stop: list in EDX not EDI, msg in EDI not EAX, byte uses AL not DL, tail merges second null check
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. The next message of `channel` if it is the one whose turn it is (its channel_seq matches),
// removed from the channel's queue; else NULL.
/* fake match: unsigned channel plus explicit mask and separate head temp steer ECX/EAX allocation */
RdpMsgArrival_guess *dequeue_message_by_sequence_guess(RdpConnection_guess *conn, unsigned int channel)
{
    RdpMsgArrival_guess *msg;
    RdpList_guess *list;
    RdpLink_guess *head;

    channel &= 0xff;
    list = &conn->chan_queue[channel];
            head = conn->chan_queue[channel].head;
            if (head != 0)
            {
                msg = (RdpMsgArrival_guess *)list->head->data;
                if (msg != 0 && conn->chan_order[channel] == msg->channel_seq)
                {
                    conn->chan_order[channel]++;
                    return (RdpMsgArrival_guess *)list_remove_head_guess(list);
                }
            }
    return 0;
}

// FUNCTION: TRILOGY 0x004f6240
// Keeps each channel's accepted message sequence from moving backward.
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Accepts a message whose seq is not behind its channel's expected seq (16-bit signed
// difference) and moves the expectation past it; 1 if accepted, else 0.
int validateAndUpdateMessageSequence_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg)
{
    u16 *expected;
    int ok;

    ok = 1;
    expected = &conn->chan_seq[msg->channel];
    if ((short)(msg->seq - *expected) >= 0)
        *expected = msg->seq + 1;
    else
        ok = 0;
    return ok;
}

// FUNCTION: TRILOGY 0x004f6270
// Assembles received fragments into complete network messages for client processing.
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Adds a fragment (`data` = its payload) to its message: a fragmented packet (0x800) finds the
// message by msgid in the connection's list, else a new message is allocated (fragments * 0x200 payload bytes
// for a fragmented one, data_len otherwise). Returns the message once all fragments are in (taken out of the list
// when it was there), else NULL; also NULL when the allocation fails.
RdpMsgArrival_guess *create_or_retrieve_network_message_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info,
                                                              const void *data)
{
    int isnew;
    u32 size;
    RdpLink_guess *link;
    RdpMsgArrival_guess *msg;

    size = info->data_len;
    if ((info->flags & 0x800) != 0)
    {
        link = list_find_link_by_key_guess(&conn->msg_list, &info->msgid);
        if (link == 0)
            msg = 0;
        else
            msg = (RdpMsgArrival_guess *)link->data;
        size = (u32)info->frag_count << 9;
        if (msg != 0)
        {
            isnew = 0;
            goto have_message;
        }
    }
    msg = (RdpMsgArrival_guess *)fast_malloc(size + 0x3c);
    initializeMessageHeader_guess(msg, info->msgid);
    isnew = 1;
have_message:
    if (msg != 0)
    {
        if (process_network_message_packet_guess(msg, conn, info, data) == 0)
        {
            if (isnew)
                LinkedList_insertNodeIntoSortedLinkedList_guess(&conn->msg_list, &msg->link);
            return 0;
        }
        if (!isnew)
            msg = (RdpMsgArrival_guess *)removeNodeFromLinkedList_guess(&conn->msg_list, &msg->link);
    }
    return msg;
}

// FUNCTION: TRILOGY 0x004f6330
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Takes the pending message (NULL if none). The flag at 0x484 is left set.
RdpMsgArrival_guess *clearAndReturnPendingMessage_guess(RdpConnection_guess *conn)
{
    RdpMsgArrival_guess *msg;

    msg = conn->pending;
    conn->pending = 0;
    return msg;
}

// FUNCTION: TRILOGY 0x004f6350
// ORIGINAL FILE: Network/rdp/rx.c (address: after rx_validate_stream_arrival)
// __cdecl. Parks `msg` as the connection's pending message.
void markMessageForPendingProcessing_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg)
{
    conn->pending = msg;
    conn->pending_flag = 1;
    conn->pending_seq = msg->first_frag_seq;
}
