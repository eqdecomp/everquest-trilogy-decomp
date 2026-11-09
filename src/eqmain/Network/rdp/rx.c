// eqmain: the same original object as trilogy's src/Network/rdp/rx.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* rx.c - the receiving side of a connection: sequence checks, fragment reassembly, in-order delivery
 * per channel.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/rx.c (from Mac debug map)
 *
 * The functions from 0x1002F170 to 0x1002F550 follow rx_validate_stream_arrival (the last one the Mac map
 * names, 0x1002F0F0) up to cypher.c and work on the same per-channel state; they are placed here by
 * address (the queue's graph labels say rdp/list/bitops).
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))
/* 0x100322AE: the import thunk for WSOCK32 htons. */
#define rdp_htons_guess        EQ_STDCALL(u16, 0x100322AE, (u16 n))
/* 0x10027940: an empty function (ret) the client calls on lists it is done with (a destructor that does nothing). */
#define rdp_no_op_guess        EQ_CDECL(void, 0x10027940, (void *p))
/* 0x1002DEA0: the intrusive list's init (cdecl): zeroes head, tail and count, sets link_offset, key_offset and the compare
 * function (eqmain/rdp.h's RdpList_guess). The name is a guess; the header's comment says what it does. */
#define rdp_list_init_guess  EQ_CDECL(void, 0x1002DEA0, (RdpList_guess *list, u32 link_offset, u32 key_offset, RdpListCompare_guess compare))
/* 0x1002DF70: the callee of 0x1002F3B0; takes a channel's queue (conn->chan_queue[channel]) and the message.
 * The name is a guess: its body is not read yet. */
#define rx_channel_queue_insert_guess  EQ_CDECL(void, 0x1002DF70, (RdpList_guess *list, RdpMsgArrival_guess *msg))
/* 0x10028A90: the rdp pool's free (fast_free; cdecl, one item; unit_10028A90.cpp). */
#define rdp_fast_free_guess  EQ_CDECL(void, 0x10028A90, (void *p))
/* 0x1002E120: the intrusive list's peek head (cdecl): the first item (head - link_offset), not removed. */
#define rdp_list_peek_head_guess  EQ_CDECL(void *, 0x1002E120, (RdpList_guess *list))
/* 0x1002E130: the intrusive list's remove head (cdecl): the item (link - link_offset), or 0 when the list is empty. */
#define rdp_list_remove_head_guess  EQ_CDECL(void *, 0x1002E130, (RdpList_guess *list))
/* 0x10033568: the CRT _free (cdecl, one pointer; library code, the name is the ANALYSIS one). */
#define crt_free_guess  EQ_CDECL(void, 0x10033568, (void *p))
/* 0x1002AA20: format_sockaddr_guess (cdecl): writes the text of an address into out (char[64] on the caller's stack). */
#define rdp_format_sockaddr_guess  EQ_CDECL(void, 0x1002AA20, (char *out, const RdpAddr_guess *addr))
/* 0x1002FA10: discard_log_append_guess (cdecl, printf-style: the format, then its arguments). */
#define rdp_discard_log_append_guess  EQ_CDECL(void, 0x1002FA10, (const char *format, ...))
/* 0x10030D50: bitarray_clear_guess (cdecl, one pointer; bitops.c, see eqmain/rdp.h): clears the 1024-bit window and its
 * count. The name is the header's; the macro is not, so the header's prototype stays as it is. */
#define rdp_bitarray_clear_guess  EQ_CDECL(void, 0x10030D50, (RdpBitArray_guess *ba))
/* 0x10030D70: copyBitfieldToBuffer_guess (cdecl; the prototype is in eqmain/rdp.h): copies the bit array from bit
 * `bit` into dst, count bytes. The name is the header's; the macro is not, so the header's prototype stays as it is. */
#define rdp_copyBitfieldToBuffer_guess  EQ_CDECL(void, 0x10030D70, (const unsigned char *bits, unsigned char *dst, unsigned int bit, unsigned int count))
/* 0x10030E30: bitarray_setbit_guess (cdecl; the prototype is in eqmain/rdp.h). The caller tests the returned AL. */
#define rdp_bitarray_setbit_guess  EQ_CDECL(unsigned char, 0x10030E30, (unsigned char *bits, unsigned int n))
/* 0x10030EB0: getbit_guess (cdecl; the prototype is in eqmain/rdp.h). The caller tests the returned AL. */
#define rdp_getbit_guess  EQ_CDECL(unsigned char, 0x10030EB0, (const unsigned char *bits, unsigned int n))
/* 0x1002E0C0: the intrusive list's find (cdecl; eqmain/rdp.h): compare(item + key_offset, key) from the head until it is >= 0. */
#define rdp_list_find_guess  EQ_CDECL(RdpMsgArrival_guess *, 0x1002E0C0, (RdpList_guess *list, const void *key))
/* 0x1002E170: the intrusive list's remove of the given item (cdecl). The name is a guess: its body is not read yet.
 * Its EAX is what the caller returns. */
#define rdp_list_remove_item_guess  EQ_CDECL(RdpMsgArrival_guess *, 0x1002E170, (RdpList_guess *list, RdpMsgArrival_guess *msg))
/* 0x100288B0: fast_malloc_guess (cdecl; eqmain/rdp.h's prototype): size bytes from the rdp pool. */
#define rdp_fast_malloc_guess  EQ_CDECL(void *, 0x100288B0, (u32 size))
/* 0x1002DD40: SetRecordWord12_guess (cdecl; the name is the bundle's): sets the u16 at +0xC of the record. */
#define rdp_SetRecordWord12_guess  EQ_CDECL(void, 0x1002DD40, (RdpMsgArrival_guess *msg, u16 word))
/* 0x1002DD60: the fragment processor (cdecl): its EAX is the accepted flag, tested in full. eqmain/rdp.h's prototype
 * for it is process_network_message_packet_guess; that name is not confirmed here. */
#define rdp_process_network_message_packet_guess  EQ_CDECL(int, 0x1002DD60, (RdpMsgArrival_guess *msg, void *sender, RdpPacketInfo_guess *info, const void *data))
/* 0x1002DE30: the fragment consistency check (cdecl, msg then info; the name is a sibling guess). Its result is what
 * rx_validate_fragment_arrival_guess returns, and logs when nonzero. */
#define rdp_validate_fragment_consistency_guess  EQ_CDECL(int, 0x1002DE30, (RdpMsgArrival_guess *msg, RdpPacketInfo_guess *info))

// FUNCTION: EQMAIN 0x1002E930
// Prepares the connection to receive server traffic.
// @early-stop: EQ_CDECL indirect-call cleanup stays immediate instead of folding into the next cdecl cleanup.
// Sibling: trilogy rx_init_guess 0x004F5700 (BinDiff 0.959): the same steps on eqmain's offsets (RdpConnection_guess
// in eqmain/rdp.h). __cdecl. Resets the receiving side: the stat block, the ack window, the message list (keyed by
// msgid), the 20 channel queues (keyed by channel_seq, 0x18 bytes apart), the pending message, the sequence fields and
// the ttl/heartbeat fields. Returns nothing.
void rx_init_guess(RdpConnection_guess *conn)
{
    RdpList_guess *q;
    int i;

    memset(conn->stat, 0, sizeof(conn->stat));
    conn->rx_base = 0;
    conn->rx_max = 0;
    rdp_bitarray_clear_guess(&conn->rx_ack_bits);
    conn->ack_count = 0;
    conn->rx_last_activity = rdp_timeGetTime_guess();
    conn->rx_last_arrival = rdp_timeGetTime_guess();
    rdp_list_init_guess(&conn->msg_list, 0, 0x12, (RdpListCompare_guess)0x10031040);
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
        rdp_list_init_guess(q, 0, 0x17, (RdpListCompare_guess)0x10031070);
        q++;
        i--;
    } while (i != 0);
    conn->ttl_log = 0;
    conn->field_7C4 = 0;
    conn->field_7C8 = rdp_timeGetTime_guess();
    conn->ttl_value = 1;
    conn->ttl_max = 0x1e;
    conn->field_7DC = 0;
    conn->ttl_index = 0;
    conn->field_498 = 0;
    conn->field_49C = 0;
    conn->field_17C = 0;
}

// FUNCTION: EQMAIN 0x1002ea70
// Frees buffered incoming messages and the connection's TTL bookkeeping.
// Cloned from trilogy NetworkConnection_FreeQueuedItems_guess 0x004F5870 (BinDiff sibling 1.000/0.993: the same
// original code, names/siblings/). __cdecl. Frees the pending message (clearing conn->pending), then every message
// on msg_list and on each of the 20 chan_queue lists (0x18 bytes apart here), calls the empty function on msg_list
// and on each chan_queue list, then frees the ttl_log (clearing conn->ttl_log). Returns nothing.
void NetworkConnection_FreeQueuedItems_guess(RdpConnection_guess *conn)
{
    void *item;
    RdpList_guess *list;
    RdpList_guess *q;
    int i;

    if (conn->pending != 0)
    {
        rdp_fast_free_guess(conn->pending);
        conn->pending = 0;
    }
    list = &conn->msg_list;
    item = rdp_list_remove_head_guess(list);
    while (item != 0)
    {
        rdp_fast_free_guess(item);
        item = rdp_list_remove_head_guess(list);
    }
    q = conn->chan_queue;
    i = 20;
    do
    {
        item = rdp_list_remove_head_guess(q);
        while (item != 0)
        {
            rdp_fast_free_guess(item);
            item = rdp_list_remove_head_guess(q);
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
        crt_free_guess(conn->ttl_log);
        conn->ttl_log = 0;
    }
}

// FUNCTION: EQMAIN 0x1002eb40
// Discards pending and queued incoming messages.
// Cloned from trilogy cleanup_connection_queues_guess 0x004F5940 (BinDiff sibling 1.000: the same original code,
// names/siblings/). __cdecl. Frees the pending message (clearing conn->pending), then every message on msg_list
// and on each of the 20 chan_queue lists (0x18 bytes apart here). Returns nothing.
void cleanup_connection_queues_guess(RdpConnection_guess *conn)
{
    void *item;
    RdpList_guess *q;
    int i;

    if (conn->pending != 0)
    {
        rdp_fast_free_guess(conn->pending);
        conn->pending = 0;
    }
    item = rdp_list_remove_head_guess(&conn->msg_list);
    while (item != 0)
    {
        rdp_fast_free_guess(item);
        item = rdp_list_remove_head_guess(&conn->msg_list);
    }
    q = conn->chan_queue;
    i = 20;
    do
    {
        item = rdp_list_remove_head_guess(q);
        while (item != 0)
        {
            rdp_fast_free_guess(item);
            item = rdp_list_remove_head_guess(q);
        }
        q++;
        i--;
    } while (i != 0);
}

// FUNCTION: EQMAIN 0x1002ebc0
// Marks the connection as recently active after network traffic.
// Cloned from trilogy NetworkConnection_update_connection_last_activity_time_guess 0x004F59C0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Stamps the connection as active now.
void NetworkConnection_update_connection_last_activity_time_guess(RdpConnection_guess *conn)
{
    conn->rx_last_activity = rdp_timeGetTime_guess();
}

// FUNCTION: EQMAIN 0x1002ebd0
// Tracks received packet numbers and bytes for connection receive statistics.
// Cloned from trilogy rx_record_seqnum_arrival_guess 0x004F59D0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
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
        conn->recv_mask |= (__int64)(1 << (-1 - diff));
        RDP_STAT_guess(0xB8)++;
        RDP_STAT_guess(0xC0) += info->data_len + info->overhead;
        RDP_CSTAT_guess(conn, 0xBC)++;
        RDP_CSTAT_guess(conn, 0xC0) += info->data_len + info->overhead;
    }
    else
    {
        RDP_STAT_guess(0xA8)++;
        RDP_STAT_guess(0xB0) += info->data_len + info->overhead;
        RDP_CSTAT_guess(conn, 0xB4)++;
        RDP_CSTAT_guess(conn, 0xB8) += info->data_len + info->overhead;
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

// FUNCTION: EQMAIN 0x1002eda0
// Rejects duplicate or out-of-window packets before their contents are accepted.
// Cloned from trilogy NetworkConnection_ValidatePacketSequenceNumber_guess 0x004F5BA0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
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
            RDP_CSTAT_guess(conn, 0xCC)++;
            return 1;
        }
    }
    if ((short)(diff + 0x40) < 0 || (short)(diff - 0x1000) > 0)
    {
        RDP_STAT_guess(0xE0)++;
        RDP_CSTAT_guess(conn, 0xC8)++;
        rc = 1;
    }
    return rc;
}

// FUNCTION: EQMAIN 0x1002ee70
// Blocks implausible message numbers and unexpected SYN starts.
// Cloned from trilogy rx_validate_msgid_arrival_guess 0x004F5C70 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
// __cdecl. Once the first packet has set the window (msgid_valid): a packet 0x78 or more ahead of rx_base, or a
// SYN one (0x2000) whose number isn't the first one, is logged and counted; returns 1 for that, else 0.
int rx_validate_msgid_arrival_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
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
                rdp_format_sockaddr_guess(addr, &conn->remote);
                if (0 != (0x2000 & info->flags))
                    syn = "SYN ";
                else
                    syn = "";
                temp = info->frag_seq;
                rdp_discard_log_append_guess("%s bad msgid %i=%u-%u-%u, %s%u\n", addr, (int)(short)(temp - conn->rx_base),
                                             (u32)temp, (u32)conn->rx_base, 0x78, syn, (u32)conn->first_msgid);
                RDP_STAT_guess(0xF0)++;
                RDP_CSTAT_guess(conn, 0xD0)++;
            }
        }
    }
    return rc;
}

// FUNCTION: EQMAIN 0x1002ef70
// Checks fragment bounds and validates consistency when a matching message is already in progress.
// Cloned from trilogy rx_validate_fragment_arrival_guess 0x004F5D70 (the same original code: BinDiff sibling).
// Offsets are this image's (msg_list 0x214, remote 0x478, the stat counters); the list find returns the message itself.
// __cdecl. 2 (logged, counted) when the fragment numbers are impossible (index not below count, count under 2, or
// neither the last fragment nor a full 0x200 bytes); else, for a message that exists, the consistency check's result,
// logged and counted when nonzero; 0 when no message matches.
int rx_validate_fragment_arrival_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    u16 index;
    u16 count;
    int rc;
    RdpMsgArrival_guess *msg;
    char addr[64];

    rc = 0;
    index = info->frag_index;
    count = info->frag_count;
    if (index < count && count >= 2 && (index + 1 == count || info->data_len == 0x200))
    {
        msg = rdp_list_find_guess(&conn->msg_list, &info->msgid);
        if (msg != 0)
        {
            rc = rdp_validate_fragment_consistency_guess(msg, info);
            if (rc != 0)
            {
                rdp_format_sockaddr_guess(addr, &conn->remote);
                rdp_discard_log_append_guess("%s bad fragment size fragid:%u part:%u/%u msgid:%u a_total:%u a_msgid:%u\n",
                                             addr, info->msgid, info->frag_index, info->frag_count, info->frag_seq,
                                             msg->frag_count, msg->first_frag_seq);
                RDP_STAT_guess(0x110)++;
                RDP_CSTAT_guess(conn, 0xE0)++;
            }
        }
    }
    else
    {
        rc = 2;
        rdp_format_sockaddr_guess(addr, &conn->remote);
        rdp_discard_log_append_guess("%s bad fragment: %u %u %u %u %u\n", addr, info->frag_index, info->frag_count, 0x200,
                                     info->data_len);
        RDP_STAT_guess(0xF8)++;
        RDP_CSTAT_guess(conn, 0xD4)++;
    }
    return rc;
}

// FUNCTION: EQMAIN 0x1002f0f0
// __cdecl. 2 (logged, counted) when the channel number is 20 or more, else 0.
int rx_validate_stream_arrival_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info)
{
    char addr[64];
    int rc;

    rc = 0;
    if (info->channel >= 0x14)
    {
        rc = 2;
        rdp_format_sockaddr_guess(addr, &conn->remote);
        rdp_discard_log_append_guess("%s bad stream id: %u >= %u\n", addr, info->channel, 0x14);
        RDP_STAT_guess(0x100)++;
        RDP_CSTAT_guess(conn, 0xD8)++;
    }
    return rc;
}

// FUNCTION: EQMAIN 0x1002f170
// Advances the receive acknowledgment window as server messages arrive.
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
        if (rdp_bitarray_setbit_guess(bits, run) != 0)
            prev = 1;
        run = 0;
        while (rdp_getbit_guess(bits, run) != 0)
        {
            run++;
        }
        if (run > (unsigned int)0)
        {
            /* 0x80 bytes: this image's window is 128 bytes (the trilogy build copies 0x100). */
            rdp_copyBitfieldToBuffer_guess(bits, bits, run, 0x80);
            conn->rx_base += run;
        }
    }
    else
        prev = 1;
    return prev;
}

// FUNCTION: EQMAIN 0x1002f2a0
// Reports received server messages in the next outgoing packet.
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
            rdp_copyBitfieldToBuffer_guess(conn->rx_ack_bits.bits, (u8 *)out,
                                           (u32)conn->ack_min - (u32)conn->rx_base, n);
            ret = n + 2;
        }
        conn->ack_count = 0;
    }
    return ret;
}

// FUNCTION: EQMAIN 0x1002f3b0
// Places a received message in its channel's delivery queue.
// __cdecl. Queues `msg` on its channel: the list at conn->chan_queue[msg->channel] (the channel byte at
// 0x16, no bounds check, as the original). Returns nothing.
void queueMessageOnChannel_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg)
{
    rx_channel_queue_insert_guess(&conn->chan_queue[msg->channel], msg);
}

// FUNCTION: EQMAIN 0x1002F3E0
// Delivers the next in-order received message for a channel when its turn arrives.
// @early-stop: the channel-order byte load/store use the opposite SIB base/index register order.
// __cdecl. The next message of `channel` if it is the one whose turn it is (its channel_seq is the channel's
// chan_order), removed from the channel's queue and the expectation moved past it; 0 otherwise. The channel is
// masked to a byte; there is no bounds check on chan_queue[] (channels 20 and up read past it, as the original).
int dequeueMessageBySequence_guess(RdpConnection_guess *conn, unsigned int channel)
{
    RdpList_guess *queue;
    int msg;
    char expected;

    channel &= 0xff;
    queue = &conn->chan_queue[channel];
    msg = (int)rdp_list_peek_head_guess(queue);
    if (msg != 0)
    {
        expected = conn->chan_order[channel];
        if (expected == *(char *)((char *)msg + 0x17))
        {
            conn->chan_order[channel] = expected + 1;
            msg = (int)rdp_list_remove_head_guess(queue);
        }
        else
            msg = 0;
    }
    return msg;
}

// FUNCTION: EQMAIN 0x1002f440
// Checks a message's sequence against its channel and advances the expected number.
// Cloned from trilogy validateAndUpdateMessageSequence_guess 0x004F6240 (the same original code: BinDiff sibling, names/siblings/).
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

// FUNCTION: EQMAIN 0x1002F470
// Adds an incoming fragment to its message and makes assembled messages available.
// __cdecl. Takes one fragment (info: its header, data: its payload) of a connection and returns the message record it
// goes into, or 0. With flag 0x800 in info->flags the record is looked up in conn->msg_list by msgid. When it is not
// there (or the flag is clear) a new record is made: frag_count * 0x200 + 0x34 bytes with the flag, data_len + 0x34
// without; its word at +0xC is set from msgid, also when the allocation failed (not checked, as in the original).
// The fragment is then processed (0x1002DD60). On failure a new record is queued on msg_list and 0 is returned. On
// success a new record is returned; a record found in msg_list is taken off it and that call's result is returned.
RdpMsgArrival_guess *rx_accept_fragment_guess(RdpConnection_guess *conn, RdpPacketInfo_guess *info, u32 *data)
{
    RdpMsgArrival_guess *msg;
    u32 size;
    s16 flags;
    int isNew;
    int ok;

    flags = info->flags;
    size = info->data_len;
    if ((flags & 0x800) != 0)
    {
        msg = rdp_list_find_guess(&conn->msg_list, &info->msgid);
        size = (u32)info->frag_count << 9;
    }
    else
        msg = 0;
    if (msg == 0)
    {
        msg = rdp_fast_malloc_guess(size + 0x34);
        rdp_SetRecordWord12_guess(msg, info->msgid);
        isNew = 1;
    }
    else
        isNew = 0;
    if (msg != 0)
    {
        ok = rdp_process_network_message_packet_guess(msg, conn, info, data);
        if (ok != 0)
        {
            if (!isNew)
                return rdp_list_remove_item_guess(&conn->msg_list, msg);
        }
        else
        {
            if (isNew)
                rx_channel_queue_insert_guess(&conn->msg_list, msg);
            return 0;
        }
    }
    return msg;
}

// FUNCTION: EQMAIN 0x1002f530
// Makes the connection's saved incoming message available for processing.
// Cloned from trilogy clearAndReturnPendingMessage_guess 0x004F6330 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Takes the pending message (NULL if none). The flag at 0x484 is left set.
RdpMsgArrival_guess *clearAndReturnPendingMessage_guess(RdpConnection_guess *conn)
{
    RdpMsgArrival_guess *msg;

    msg = conn->pending;
    conn->pending = 0;
    return msg;
}

// FUNCTION: EQMAIN 0x1002f550
// Schedules a received message as the connection's next pending item.
// Cloned from trilogy markMessageForPendingProcessing_guess 0x004F6350 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Parks `msg` as the connection's pending message.
void markMessageForPendingProcessing_guess(RdpConnection_guess *conn, RdpMsgArrival_guess *msg)
{
    conn->pending = msg;
    conn->pending_flag = 1;
    conn->pending_seq = msg->first_frag_seq;
}
