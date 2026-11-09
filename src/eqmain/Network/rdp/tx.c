// eqmain: the same original object as trilogy's src/Network/rdp/tx.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* tx.c - the sending side of a connection: outgoing queues, acknowledgements, retransmission.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/tx.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <math.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))
#define setLinkedListProperties_by_addr_guess EQ_CDECL(void, 0x004F5460, (RdpList_guess *list, int sorted, RdpListCompare_guess compare))
#define rdp_list_remove_head_guess EQ_CDECL(void *, 0x1002E130, (RdpList_guess *list))
#define rdp_fast_free_guess EQ_CDECL(void, 0x10028A90, (void *p))
#define rdp_uevent_signal_guess EQ_CDECL(void, 0x10030A00, (void **ev))
#define eqmain_srand_guess EQ_CDECL(void, 0x100360B2, (unsigned long))
#define eqmain_rand_guess EQ_CDECL(int, 0x100360BF, (void))
#define rdp_ntohs_guess EQ_STDCALL(u16, 0x100322BA, (u16 n))
#define rdp_htons_guess EQ_STDCALL(u16, 0x100322AE, (u16 n))
#define bitarray_clear_guess EQ_CDECL(void, 0x10030D50, (void *bits))
#define rdp_list_init_keyed_guess EQ_CDECL(void, 0x1002DEA0, (void *list, u32 link_offset, u32 key_offset, RdpListCompare_guess compare))
#define bandwidth_init_guess EQ_CDECL(void, 0x10030C10, (void *bw))
#define rdp_packet_rate_init_guess EQ_CDECL(void, 0x10030A10, (void *, int, int))
/* 0x1002EB40: the connection's queue cleanup (cdecl, one argument). */
#define cleanup_connection_queues_guess EQ_CDECL(void, 0x1002EB40, (int conn))
/* 0x1002F2A0: NetworkConnection_pPrepareAcknowledgementHeader_guess (cdecl): fills the header, returns its length. */
#define rdp_prepare_ack_header_guess EQ_CDECL(int, 0x1002F2A0, (RdpConnection_guess *conn, void *data, u16 *flags))
/* 0x10030140: SendPacketWithCRC_guess, the serial variant (cdecl; its first argument is the serial queue's lock). */
#define rdp_send_packet_serial_guess EQ_CDECL(int, 0x10030140, (void *lock, void *packet, int count, RdpAddr_guess *addr))
/* 0x1002F8D0: SendPacketWithCRC_guess, the socket variant (cdecl, six arguments). */
#define rdp_send_packet_socket_guess EQ_CDECL(int, 0x1002F8D0, (u32 sock, void *packet, int count, RdpAddr_guess *addr, u32 a5, u32 a6))
/* 0x10030C30: UpdateBandwidthQuota_guess (cdecl). */
#define rdp_bandwidth_update_guess EQ_CDECL(void, 0x10030C30, (RdpBandwidth_guess *bw, u32 bytes))
/* 0x1002C5B0: NetworkConnection_Disconnect_guess (cdecl; the sibling defined below). */
#define rdp_connection_disconnect_guess EQ_CDECL(void, 0x1002C5B0, (RdpConnection_guess *conn, u32 reason))
/* 0x100498B4: PTR_DAT_100498b4, the rdp statistics block (64-bit counters from +0x190 up). */
#define rdp_stats_guess EQ_GLOBAL(u8 *, 0x100498B4)
/* 0x1002CD70: NetworkConnection_SendPacket_guess (cdecl; its int result is unused by the callers here). */
#define rdp_connection_send_packet_guess EQ_CDECL(int, 0x1002CD70, (int conn, void *data, int size, u16 flags))
/* 0x10030CB0: update_token_bucket_guess (cdecl, one argument: the bucket). */
#define rdp_update_token_bucket_guess EQ_CDECL(u32, 0x10030CB0, (u32 *bucket))
/* 0x1002AFC0: check_connection_status_guess (cdecl, one argument). */
#define rdp_check_connection_status_guess EQ_CDECL(int, 0x1002AFC0, (u32 handle))
/* 0x1002E120: GetRecordDelta_guess (cdecl): the queue's count, *q - q[2] (name guessed). */
#define rdp_list_count_guess EQ_CDECL(int, 0x1002E120, (void *list))
/* 0x1002DF20: record_add_tail_guess (cdecl, two arguments): the queue's add of a packet (name guessed). */
#define rdp_list_add_tail_guess EQ_CDECL(void, 0x1002DF20, (void *list, void *link))

// FUNCTION: EQMAIN 0x1002b000
// Sets up the connection's outgoing packet state for its remote peer.
// @early-stop: the cdecl argument cleanup is split into add esp,0x38 and add esp,0xc instead of one add esp,0x44.
// Cloned from trilogy tx_init_guess 0x004F24D0 (BinDiff sibling 0.980, names/siblings/; the struct offsets here are
// this image's: remote address at 0x478, ttl address at 0x700, queues at 0x518/0x534/0x550, close pointers at 0x6F8).
// __cdecl, void. Seeds the rand state from timeGetTime and copies the address at remoteAddress to +0x478. Clears the ttl
// address at +0x700; for family 2 copies the address there and sets 0x8000 in the port at +0x702 (ntohs/htons).
// Then sets +0x474 by family (2: protocolContext+4, 6: protocolContext+0x10, 0x45: -1), resets the rand-derived fields, the three
// queues, the bandwidth and the packet rate state, and stores protocolContext+0x44 at +0x714.
void tx_init_guess(int connection, int protocolContext, short *remoteAddress)
{
    short addressFamily;
    u16 port;
    DWORD DVar3;
    int initialSequence;
    void *list;

    DVar3 = rdp_timeGetTime_guess();
    eqmain_srand_guess(DVar3);
    *(RdpAddr_guess *)(connection + 0x478) = *(RdpAddr_guess *)remoteAddress;
    memset((void *)(connection + 0x700), 0, sizeof(RdpAddr_guess));
    addressFamily = *remoteAddress;
    switch ((u16)addressFamily)
    {
    case 2:
        *(u32 *)(connection + 0x474) = *(u32 *)(protocolContext + 4);
        *(RdpAddr_guess *)(connection + 0x700) = *(RdpAddr_guess *)remoteAddress;
        port = rdp_ntohs_guess(*(u16 *)(connection + 0x702));
        port = rdp_htons_guess(port | 0x8000);
        *(u16 *)(connection + 0x702) = port;
        break;
    case 6:
        *(u32 *)(connection + 0x474) = *(u32 *)(protocolContext + 0x10);
        break;
    case 0x45:
        *(u32 *)(connection + 0x474) = 0xffffffff;
        break;
    }
    *(u32 *)(connection + 0x710) = *(u32 *)(protocolContext + 0xc);
    initialSequence = eqmain_rand_guess();
    *(short *)(connection + 0x48c) = (short)initialSequence;
    *(short *)(connection + 0x488) = (short)initialSequence - 1;
    *(u16 *)(connection + 0x48a) = 0;
    *(u16 *)(connection + 0x48e) = 0;
    DVar3 = rdp_timeGetTime_guess();
    *(DWORD *)(connection + 0x490) = DVar3;
    bitarray_clear_guess((void *)(connection + 0x494));
    list = (void *)(connection + 0x518);
    rdp_list_init_keyed_guess(list, 0, 0, 0);
    *(u32 *)((char *)list + 0x18) = 0;
    list = (void *)(connection + 0x534);
    rdp_list_init_keyed_guess(list, 0, 0, 0);
    *(u32 *)((char *)list + 0x18) = 0;
    list = (void *)(connection + 0x550);
    rdp_list_init_keyed_guess(list, 0, 0, 0);
    *(u32 *)((char *)list + 0x18) = 0;
    bandwidth_init_guess((void *)(connection + 0x56c));
    *(u32 *)(connection + 0x57c) = 8000;
    memset((void *)(connection + 0x6a8), 0, 20);
    DVar3 = rdp_timeGetTime_guess();
    *(DWORD *)(connection + 0x580) = DVar3;
    *(u32 *)(connection + 0x584) = 0;
    rdp_packet_rate_init_guess((void *)(connection + 0x588), 500, 1);
    *(u32 *)(connection + 0x6bc) = 0;
    *(u32 *)(connection + 0x6c0) = 0;
    *(u16 *)(connection + 0x6c4) = *(u16 *)(connection + 0x48c);
    *(u32 *)(connection + 0x6c8) = 0;
    *(u32 *)(connection + 0x6cc) = 0;
    *(u16 *)(connection + 0x6d0) = 0;
    *(u32 *)(connection + 0x6d4) = 1;
    *(u32 *)(connection + 0x6d8) = 0;
    *(u32 *)(connection + 0x6dc) = 0;
    *(u32 *)(connection + 0x6e0) = 0;
    *(u32 *)(connection + 0x6e4) = 10000;
    *(u32 *)(connection + 0x6e8) = 5000;
    *(u32 *)(connection + 0x6ec) = 0;
    *(u32 *)(connection + 0x6f0) = 0;
    *(u16 *)(connection + 0x6f4) = 0;
    *(u32 *)(connection + 0x6f8) = 0;
    *(u32 *)(connection + 0x6fc) = 0;
    *(u32 *)(connection + 0x714) = *(u32 *)(protocolContext + 0x44);
    return;
}

// FUNCTION: EQMAIN 0x1002b210
// Releases all queued outgoing packets from the connection.
// Cloned from trilogy NetworkConnection_FreeBuffers_guess 0x004F2730 (BinDiff sibling, names/siblings/; the
// struct offsets here are this image's: item size +0x20, queues at 0x550/0x534/0x518, close pointers at 0x6F8/0x6FC).
// __cdecl. Frees every item of the hold, wait and sent queues (taking each item's size off the queue's byte
// count), clears the close result pointer (storing 0 through it) and signals the close event.
void NetworkConnection_FreeBuffers_guess(RdpConnection_guess *conn)
{
    RdpList_guess *list;
    void *item;

    list = &conn->tx_hold;
    while (1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)((char *)(((char *)item + 0x20)));
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_wait;
    while (1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)((char *)item + 0x20);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_sent;
    while (1)
    {
        unsigned long *temp;
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        temp = (u32 *)((char *)item + 0x20);
        *(u32 *)(list + 1) -= *temp;
        rdp_fast_free_guess(item);
    }
    if (conn->close_result != 0)
    {
        *conn->close_result = 0;
        conn->close_result = 0;
    }
    if (conn->close_event != 0)
    {
        rdp_uevent_signal_guess(conn->close_event);
        conn->close_event = 0;
    }
}

// FUNCTION: EQMAIN 0x1002b2d0
// Accounts for one acknowledged packet, sampling its round-trip time and completing a close if due.
// Cloned from trilogy NetworkConnection_phandle_packet_acknowledgment_guess 0x004F27F0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
void NetworkConnection_phandle_packet_acknowledgment_guess(RdpConnection_guess *conn, u32 sequence)
{
    char *item;
    int elapsed;
    unsigned __int64 *stat;

    item = (char *)EQ_CDECL(void *, 0x10030ED0, (RdpList_guess *, u32))(&conn->tx_sent, sequence);
    RDP_STAT_guess(0x88)++;
    RDP_CSTAT_guess(conn, 0xA4)++;
    if (item != 0)
    {
        elapsed = rdp_timeGetTime_guess() - *(int *)(item + 0x8);
        RDP_STAT_guess(0x1a0)++;
        RDP_CSTAT_guess(conn, 0xf8)++;
        if (*(u16 *)(item + 0x1c) == 1)
        {
            stat = &RDP_STAT_guess(0x198);
            ++*stat;
            RDP_CSTAT_guess(conn, 0xF4)++;
            EQ_CDECL(void, 0x10030A80, (RdpRttStats_guess *, int, u16))(
                &conn->rtt, elapsed, *(u16 *)(item + 0x1c));
            conn->last_rtt = (u16)elapsed;
        }
        rdp_fast_free_guess(item);
    }
    if ((conn->field_764 != 0) && (conn->field_768 == 0))
    {
        conn->field_768 = 1;
        stat = &RDP_STAT_guess(0x178);
        ++*stat;
        conn->field_78C = 120000;
        conn->field_790 = 30000;
    }
    if (conn->field_770 != 0)
    {
        if ((conn->field_774 == 0) && ((u16)sequence == conn->field_778))
            conn->field_774 = 1;
    }
    if ((conn->field_770 != 0) && (conn->tx_acked == conn->field_778))
    {
        if (conn->close_result != 0)
        {
            *conn->close_result = 1;
            conn->close_result = 0;
        }
        if (conn->close_event != 0)
        {
            rdp_uevent_signal_guess(conn->close_event);
            conn->close_event = 0;
        }
    }
    return;
}

// FUNCTION: EQMAIN 0x1002b470
// Checks incoming ACK flags and sequence range before accepting an acknowledgment.
// Cloned from trilogy tx_validate_ack_arrival_guess 0x004F2990 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code; comments keep trilogy's.
int tx_validate_ack_arrival_guess(u8 *arg1, u16 *arg2, int *ackLenOut)
{
    u16 pktFlags;
    u16 logFlags;
    u16 *ackData;
    int connection;
    char bitSet;
    u16 ackSeq;
    short temp;
    u8 *synText;
    u32 bitIndex;
    u8 *ptr;
    u16 *ackMask;
    char *fragText;
    u8 maskNibble;
    short ackedSeq;
    char *msgIdText;
    char *multiText;
    u32 value;
    u32 maskLen;
    u32 bitCounter;
    u8 addrText[64];

    connection = (int)arg1;
    pktFlags = *arg2;
    ackData = *(u16 **)(arg2 + 2);
    *ackLenOut = 0;
    ptr = rdp_stats_guess;
    if (((pktFlags & 4) != 0) && ((pktFlags & 8) != 0))
    {
        EQ_CDECL(void, 0x1002AA20, (char *, void *))((char *)addrText, (void *)(connection + 0x478));
        logFlags = *arg2;
        multiText = (char *)EQ_ARRAY(char, 0x10048B78);
        if ((logFlags & 0x100) == 0)
            multiText = (char *)EQ_ARRAY(char, 0x1004CE40);
        arg1 = (u8 *)EQ_ARRAY(char, 0x10048B70);
        if ((logFlags & 1) == 0)
            arg1 = (u8 *)EQ_ARRAY(char, 0x1004CE40);
        fragText = (char *)EQ_ARRAY(char, 0x10048B68);
        if ((logFlags & 0x800) == 0)
            fragText = (char *)EQ_ARRAY(char, 0x1004CE40);
        msgIdText = (char *)EQ_ARRAY(char, 0x10048B60);
        if ((logFlags & 0x200) == 0)
            msgIdText = (char *)EQ_ARRAY(char, 0x1004CE40);
        ptr = (u8 *)EQ_ARRAY(char, 0x10048B58);
        if ((logFlags & 0x4000) == 0)
            ptr = (u8 *)EQ_ARRAY(char, 0x1004CE40);
        synText = (u8 *)EQ_ARRAY(char, 0x10048B50);
        if ((logFlags & 0x2000) == 0)
            synText = (u8 *)EQ_ARRAY(char, 0x1004CE40);
        EQ_CDECL(void, 0x1002FA10, (const char *, ...))(
            (const char *)EQ_ARRAY(char, 0x10048B20), addrText, pktFlags, synText, ptr, msgIdText, fragText,
            arg1, multiText);
        ptr = rdp_stats_guess;
        value = *(u32 *)(ptr + 0x118);
        *(u32 *)(ptr + 0x118) = value + 1;
        *(int *)(ptr + 0x11c) = *(int *)(ptr + 0x11c) + (u32)(0xfffffffe < value);
        *(int *)(connection + 0xe4) = *(int *)(connection + 0xe4) + 1;
        return 2;
    }
    if ((*arg2 & 0xc) == 0)
    {
        if ((pktFlags & 0xf0) != 0)
        {
            ptr = rdp_stats_guess;
            value = *(u32 *)(ptr + 0x130);
            *(u32 *)(ptr + 0x130) = value + 1;
            *(int *)(ptr + 0x134) = *(int *)(ptr + 0x134) + (u32)(0xfffffffe < value);
            *(int *)(connection + 0xec) = *(int *)(connection + 0xec) + 1;
            return 2;
        }
    }
    else
    {
        ackMask = ackData + 1;
        ackSeq = rdp_ntohs_guess(*ackData);
        value = (u32)ackSeq;
        maskNibble = (u8)pktFlags >> 4;
        maskLen = (u32)maskNibble;
        *ackLenOut = 2;
        if (maskNibble != 0)
        {
            arg2 = (u16 *)(maskLen * 8);
            *ackLenOut = maskLen + 2;
            bitCounter = (int)arg2 - 8;
            temp = (short)arg2;
            bitIndex = bitCounter & 0xffff;
            if ((u32)(int)temp <= bitIndex)
                goto mask_empty;
            do
            {
                bitSet = EQ_CDECL(char, 0x10030EB0, (u16 *, u32))(ackMask, bitIndex);
                if (bitSet != '\0')
                    arg2 = (u16 *)bitCounter;
                bitCounter = bitCounter + 1;
                bitIndex = bitCounter & 0xffff;
            } while (bitIndex < (u32)(int)temp);
            if ((short)arg2 == temp)
                goto mask_empty;
            value = (int)arg2 + 1U + value;
            goto check_ack_range;
        }
        goto check_ack_range;
mask_empty:
        EQ_CDECL(void, 0x1002AA20, (char *, void *))((char *)addrText,
            (void *)(connection + 0x478));
        EQ_CDECL(void, 0x1002FA10, (const char *, ...))(
            (const char *)EQ_ARRAY(char, 0x10048AF8), addrText,
            *(u8 *)((maskLen - 1) + (int)ackMask));
        ptr = rdp_stats_guess;
        value = *(u32 *)(ptr + 0x120);
        *(u32 *)(ptr + 0x120) = value + 1;
        *(int *)(ptr + 0x124) = *(int *)(ptr + 0x124) + (u32)(0xfffffffe < value);
        *(int *)(connection + 0xe8) = *(int *)(connection + 0xe8) + 1;
        return 2;
check_ack_range:
        ackedSeq = (short)value;
        temp = ackedSeq - *(short *)(connection + 0x48c);
        if ((-1 < temp) || (temp < -0x1000))
        {
            EQ_CDECL(void, 0x1002AA20, (char *, void *))((char *)addrText,
                (void *)(connection + 0x478));
            EQ_CDECL(void, 0x1002FA10, (const char *, ...))(
                (const char *)EQ_ARRAY(char, 0x10048AD0), addrText,
                (int)(short)(ackedSeq - *(short *)(connection + 0x48c)), value & 0xffff,
                *(u16 *)(connection + 0x48c));
            ptr = rdp_stats_guess;
            if ((short)(ackedSeq - *(short *)(connection + 0x48c)) < 0)
            {
                value = *(u32 *)(ptr + 0x128);
                *(u32 *)(ptr + 0x128) = value + 1;
                *(int *)(ptr + 300) = *(int *)(ptr + 300) + (u32)(0xfffffffe < value);
                *(int *)(connection + 0xf0) = *(int *)(connection + 0xf0) + 1;
                return 1;
            }
            value = *(u32 *)(ptr + 0xa0);
            *(u32 *)(ptr + 0xa0) = value + 1;
            *(int *)(ptr + 0xa4) = *(int *)(ptr + 0xa4) + (u32)(0xfffffffe < value);
            *(int *)(connection + 0xb0) = *(int *)(connection + 0xb0) + 1;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: EQMAIN 0x1002b7b0
// Applies incoming ACK ranges to the send window and retires packets they acknowledge.
// Cloned from trilogy tx_record_ack_arrival_guess 0x004F2CD0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code; comments keep trilogy's.
void tx_record_ack_arrival_guess(int connection, u16 *pktHeader)
{
    u32 ackFieldSize;
    char bitSet;
    u16 ackSeq;
    int tmp;
    int *hold;
    short bitSkip;
    u16 bitPos;
    short ackSpan;
    u16 value;
    int maskBits;
    u16 sequence;
    u32 bitIndex;
    u16 mask_offset;
    int count;
    short ackedCount;
    u16 *ackCursor;
    char traceText[252];

    value = *pktHeader;
    ackCursor = *(u16 **)(pktHeader + 2);
    mask_offset = 0;
    bitPos = 0;
    ackedCount = 0;
    if ((value & 4) != 0)
    {
        ackSeq = rdp_ntohs_guess(*ackCursor++);
        EQ_CDECL(int, 0x10034117, (char *, const char *, ...))(
            traceText, (const char *)EQ_ARRAY(char, 0x10048B90), ackSeq);
        if (0 < (short)(ackSeq - *(short *)(connection + 0x488)))
        {
            tmp = connection + 0x494;
            do
            {
                bitSet = EQ_CDECL(char, 0x10030EB0, (u16 *, u32))((u16 *)tmp, (u32)bitPos);
                if (bitSet != '\0')
                {
                    EQ_CDECL(void, 0x1002B2D0, (int, u16))(
                        connection, (u16)(*(short *)(connection + 0x488) + bitPos) + 1);
                    ackedCount = ackedCount + 1;
                }
                bitPos = bitPos + 1;
                ackSpan = ackSeq - *(short *)(connection + 0x488);
            } while (0 < (short)(ackSpan - bitPos));
            if (bitPos > 0)
            {
                EQ_CDECL(void, 0x10030D70, (void *, void *, int, int))(
                    (void *)tmp, (void *)tmp, bitPos, 0x80);
                *(u16 *)(connection + 0x488) = ackSeq;
                hold = (int *)(connection + 0x550);
                tmp = rdp_list_count_guess(hold);
                while (tmp != 0 && (tmp = rdp_list_count_guess(hold),
                       (short)((*(short *)(tmp + 0x12) - *(short *)(connection + 0x488)) - 0x78) < 0))
                {
                    tmp = EQ_CDECL(int, 0x1002E130, (int *))(hold);
                    if (tmp != 0)
                        *(int *)(connection + 0x568) -= *(int *)(tmp + 0x20);
                    EQ_CDECL(void, 0x1002BAE0, (int, int))(connection, tmp);
                    tmp = rdp_list_count_guess(hold);
                }
            }
        }
        mask_offset = ackSeq - *(short *)(connection + 0x488);
    }
    else if ((value & 8) != 0)
    {
        ackSeq = rdp_ntohs_guess(*ackCursor++);
        sequence = ackSeq - *(short *)(connection + 0x488);
        mask_offset = sequence;
        if ((0 < (short)sequence) &&
            (bitSet = EQ_CDECL(char, 0x10030E70, (u8 *, u32))((u8 *)(connection + 0x494), mask_offset - 1), bitSet != '\0'))
        {
            EQ_CDECL(void, 0x1002B2D0, (int, u16))(connection, ackSeq);
            ackedCount = 1;
        }
        EQ_CDECL(int, 0x10034117, (char *, const char *, ...))(
            traceText, (const char *)EQ_ARRAY(char, 0x10048B80), ackSeq);
    }
    maskBits = (u8)(value >> 4) & 0xf;
    ackFieldSize = maskBits + 2;
    maskBits <<= 3;
    if ((*pktHeader & 0xc) != 0)
    {
        if (pktHeader[10] == 0)
        {
            g_rdp_stats_guess->c[15] += 1;
            *(int *)(connection + 0x9c) = *(int *)(connection + 0x9c) + 1;
        }
        else
        {
            g_rdp_stats_guess->c[16] += 1;
            *(int *)(connection + 0xa0) = *(int *)(connection + 0xa0) + 1;
        }
    }
    bitSkip = 0;
    if ((short)mask_offset < 0)
    {
        bitSkip = mask_offset;
        mask_offset = 0;
        bitSkip = -bitSkip;
        maskBits = (short)(maskBits - bitSkip);
    }
    count = (short)maskBits;
    value = 0;
    if (0 < count)
    {
        bitIndex = 0;
        do
        {
            bitSet = EQ_CDECL(char, 0x10030EB0, (u16 *, u32))(ackCursor, (bitSkip & 0xffff) + bitIndex);
            if ((bitSet != '\0') &&
                (bitSet = EQ_CDECL(char, 0x10030E70, (u8 *, u32))((u8 *)(connection + 0x494), mask_offset + bitIndex), bitSet != '\0'))
            {
                sequence = *(short *)(connection + 0x488) + value;
                sequence += 1;
                sequence += mask_offset;
                EQ_CDECL(void, 0x1002B2D0, (int, u16))(connection, sequence);
                ackedCount = ackedCount + 1;
            }
            value = value + 1;
            bitIndex = (u32)value;
        } while ((int)bitIndex < count);
    }
    if (ackedCount == 0)
    {
        g_rdp_stats_guess->c[18] += 1;
        g_rdp_stats_guess->c[19] += (int)(u16)ackFieldSize;
        *(int *)(connection + 0xa8) = *(int *)(connection + 0xa8) + 1;
        *(u32 *)(connection + 0xac) = *(int *)(connection + 0xac) + (u16)ackFieldSize;
    }
    return;
}

// FUNCTION: EQMAIN 0x1002bae0
// Routes outgoing packets to transmission or queues them until the connection can send.
// Cloned from trilogy tx_enqueue_outgoing_guess 0x004F3000 (BinDiff sibling; this image's offsets and callees substituted).
// @early-stop: VC6 loads the flag byte (mov cl, byte ptr [esi+0x11]) ahead of the stats pointer at both sites; the original tests it in memory, so the temps after it differ.
void tx_enqueue_outgoing_guess(int connection, int message)
{
    u8 *statsBase;
    DWORD DVar3;
    u32 bucketLevel;
    int headMsg;
    int *txList;
    u16 msgIdBit;
    int underLimit;

    msgIdBit = *(u16 *)(message + 0x10) & 0x200;
    if ((msgIdBit != 0) && (*(int *)(connection + 0x6bc) == 0))
    {
        *(u16 *)(message + 0x10) = *(u16 *)(message + 0x10) | 0x2000;
        *(u32 *)(connection + 0x6bc) = 1;
        DVar3 = rdp_timeGetTime_guess();
        *(short *)(message + 0x1c) = *(short *)(message + 0x1c) + 1;
        *(DWORD *)(message + 0xc) = DVar3;
        *(DWORD *)(message + 0x8) = DVar3;
        rdp_connection_send_packet_guess(connection, (void *)(message + 0x28), *(int *)(message + 0x20),
            *(u16 *)(message + 0x10));
        statsBase = rdp_stats_guess;
        if ((*(u8 *)(message + 0x11) & 2) != 0)
        {
            *((unsigned __int64 *)(statsBase + 0x10)) += 1;
            statsBase = rdp_stats_guess;
            *((unsigned __int64 *)(statsBase + 0x18)) += *(u32 *)(message + 0x20);
            *(int *)(connection + 0x68) = *(int *)(connection + 0x68) + 1;
            txList = (int *)(connection + 0x518);
            *(int *)(connection + 0x6c) = *(int *)(connection + 0x6c) + *(int *)(message + 0x20);
        }
        else
        {
            *((unsigned __int64 *)rdp_stats_guess) += 1;
            *((unsigned __int64 *)(rdp_stats_guess + 8)) += *(u32 *)(message + 0x20);
            *(int *)(connection + 0x60) = *(int *)(connection + 0x60) + 1;
            *(int *)(connection + 0x64) = *(int *)(connection + 0x64) + *(int *)(message + 0x20);
            rdp_fast_free_guess((void *)message);
            return;
        }
        txList[6] = txList[6] + *(int *)(message + 0x20);
        rdp_list_add_tail_guess(txList, (void *)message);
        return;
    }
    if (((*(u16 *)(message + 0x10) & 0x200) != 0) &&
        ((short)((*(short *)(message + 0x12) - *(short *)(connection + 0x488)) + -0x78) >= 0))
    {
        txList = (int *)(connection + 0x550);
        txList[6] = txList[6] + *(int *)(message + 0x20);
        rdp_list_add_tail_guess(txList, (void *)message);
        return;
    }
    if (*(int *)(connection + 0x6c0) != 0)
    {
        bucketLevel = rdp_update_token_bucket_guess((u32 *)(connection + 0x56c));
        underLimit = bucketLevel < (*(u32 *)(connection + 0x574) >> 3);
        if (*(short *)(connection + 0x478) == 0x45)
        {
            if ((underLimit) && (headMsg = rdp_check_connection_status_guess(*(u32 *)(connection + 0xc)), headMsg != 0))
            {
send_now:
                txList = (int *)(connection + 0x534);
                headMsg = rdp_list_count_guess(txList);
                if (headMsg != 0)
                {
                    txList[6] = txList[6] + *(int *)(message + 0x20);
                    rdp_list_add_tail_guess(txList, (void *)message);
                    headMsg = (int)rdp_list_remove_head_guess((RdpList_guess *)txList);
                    if (headMsg != 0)
                    {
                        txList[6] = txList[6] - *(int *)(headMsg + 0x20);
                    }
                    message = headMsg;
                }
                DVar3 = rdp_timeGetTime_guess();
                *(short *)(message + 0x1c) = *(short *)(message + 0x1c) + 1;
                *(DWORD *)(message + 0xc) = DVar3;
                *(DWORD *)(message + 0x8) = DVar3;
                rdp_connection_send_packet_guess(connection, (void *)(message + 0x28), *(int *)(message + 0x20),
                    *(u16 *)(message + 0x10));
                statsBase = rdp_stats_guess;
                if ((*(u8 *)(message + 0x11) & 2) == 0)
                {
                    *((unsigned __int64 *)rdp_stats_guess) += 1;
                    *((unsigned __int64 *)(rdp_stats_guess + 8)) += *(u32 *)(message + 0x20);
                    *(int *)(connection + 0x60) = *(int *)(connection + 0x60) + 1;
                    *(int *)(connection + 0x64) = *(int *)(connection + 0x64) + *(int *)(message + 0x20);
                    rdp_fast_free_guess((void *)message);
                    return;
                }
                *((unsigned __int64 *)(statsBase + 0x10)) += 1;
                statsBase = rdp_stats_guess;
                *((unsigned __int64 *)(statsBase + 0x18)) += *(u32 *)(message + 0x20);
                *(int *)(connection + 0x68) = *(int *)(connection + 0x68) + 1;
                txList = (int *)(connection + 0x518);
                *(int *)(connection + 0x6c) = *(int *)(connection + 0x6c) + *(int *)(message + 0x20);
                goto add_to_list; // as original: this branch jumps to the one shared list-add tail, not a copy
            }
        }
        else if (underLimit) goto send_now; // as original: the status-not-0x45 branch jumps into the one shared list block
    }
    txList = (int *)(connection + 0x534);
add_to_list:
    txList[6] = txList[6] + *(int *)(message + 0x20);
    rdp_list_add_tail_guess(txList, (void *)message);
    return;
}

// FUNCTION: EQMAIN 0x1002bd40
// Starts connection shutdown by queuing a FIN message with its sequence number.
// Cloned from trilogy NetworkConnection_send_disconnect_message_guess 0x004F3260 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
int NetworkConnection_send_disconnect_message_guess(int connectionObject)
{
    int message;
    DWORD DVar2;
    int result = 0;
    u16 sequence;

    if (((*(int *)(connectionObject + 0x6c8) == 0) && (*(int *)(connectionObject + 0x6d4) != 0)) &&
        (*(int *)(connectionObject + 0x6d8) == 0))
    {
        message = EQ_CDECL(int, 0x100288B0, (int))(0x2a);
        if (message == 0)
            result = 2;
        else
        {
            EQ_CDECL(void, 0x10030E30, (void *, u32))(
                (void *)(connectionObject + 0x494),
                (u16)(*(short *)(connectionObject + 0x48c) - *(short *)(connectionObject + 0x488) - 1));
            /* The zero-argument timer thunk has the same stack contract as cdecl. */
            DVar2 = EQ_CDECL(DWORD, 0x1002F9F0, (void))();
            *(DWORD *)(connectionObject + 0x490) = DVar2;
            sequence = *(u16 *)(connectionObject + 0x48c);
            *(u16 *)(connectionObject + 0x48c) = sequence + 1;
            *(u16 *)(connectionObject + 0x6d0) = sequence;
            *(u16 *)(message + 0x10) = 0x4200;
            *(u16 *)(message + 0x12) = *(u16 *)(connectionObject + 0x6d0);
            EQ_CDECL(void, 0x10030F30, (void *))((void *)message);
            EQ_CDECL(void, 0x1002BAE0, (int, int))(connectionObject, message);
            *(int *)(connectionObject + 0x6c8) = 1;
        }
    }
    return result;
}

// FUNCTION: EQMAIN 0x1002be10
// Sends a sequenced SYS control message; its specific purpose is unclear.
// Cloned from trilogy NetworkConnection_SendConnectionAcknowledgement_guess 0x004F3330 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
int NetworkConnection_SendConnectionAcknowledgement_guess(int connectionObject)
{
    short sequence;
    int message;
    DWORD DVar3;
    int result = 0;

    if (((*(int *)(connectionObject + 0x6c8) == 0) && (*(int *)(connectionObject + 0x6d4) != 0)) &&
        (*(int *)(connectionObject + 0x6d8) == 0))
    {
        message = EQ_CDECL(int, 0x100288B0, (int))(0x2a);
        if (message == 0)
            result = 2;
        else
        {
            *(u16 *)(message + 0x10) = 0x201;
            EQ_CDECL(void, 0x10030E30, (void *, u32))(
                (void *)(connectionObject + 0x494),
                (u16)(*(short *)(connectionObject + 0x48c) - *(short *)(connectionObject + 0x488) - 1));
            /* The zero-argument timer thunk has the same stack contract as cdecl. */
            DVar3 = EQ_CDECL(DWORD, 0x1002F9F0, (void))();
            *(DWORD *)(connectionObject + 0x490) = DVar3;
            sequence = *(short *)(connectionObject + 0x48c);
            *(short *)(connectionObject + 0x48c) = sequence + 1;
            *(short *)(message + 0x12) = sequence;
            EQ_CDECL(void, 0x10030F30, (void *))((void *)message);
            EQ_CDECL(void, 0x1002BAE0, (int, int))(connectionObject, message);
        }
    }
    return result;
}

// FUNCTION: EQMAIN 0x1002bec0
// Sends a one-byte peer probe and records its send time.
// Cloned from trilogy NetworkConnection_SendHeartbeat_guess 0x004F33E0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
int NetworkConnection_SendHeartbeat_guess(int connectionArg)
{
    int connection = connectionArg;
    DWORD DVar3;
    int result;
    int socketError;
    DWORD *pDVar7;
    u32 socketOptionValue;
    BYTE heartbeat;

    pDVar7 = (DWORD *)(*(int *)(connectionArg + 0x718) + *(int *)(connectionArg + 0x738) * 0x10);
    DVar3 = rdp_timeGetTime_guess();
    *pDVar7 = DVar3;
    *(BYTE *)(pDVar7 + 3) = *(BYTE *)(connection + 0x72c);
    socketOptionValue = *(BYTE *)(pDVar7 + 3);
    heartbeat = *(BYTE *)(connection + 0x738);
    result = EQ_STDCALL(int, 0x1003229C, (int, int, int, char *, int))(
        *(int *)(connection + 0x710), 0, 7, (char *)&socketOptionValue, 4);
    if (result != 0)
    {
        socketError = EQ_STDCALL(int, 0x100322E4, (void))();
        EQ_CDECL(void, 0x10030660, (int))(socketError);
        *(int *)(connection + 0x734) = 3;
        return result;
    }
    result = EQ_STDCALL(int, 0x10032290, (int, char *, int, int, void *, int))(
        *(int *)(connection + 0x710), (char *)&heartbeat, 1, 0,
        (void *)(connection + 0x700), 0x10);
    if (result == 1)
    {
        if (++*(u32 *)(connection + 0x72c) > *(u32 *)(connection + 0x730))
        {
            *(int *)(connection + 0x72c) = 1;
            *(int *)(connection + 0x734) = *(int *)(connection + 0x734) + 1;
        }
        *(int *)(connection + 0x71c) = 1;
        *(int *)(connection + 0x738) = *(int *)(connection + 0x738) + 1;
        DVar3 = rdp_timeGetTime_guess();
        *(DWORD *)(connection + 0x720) = DVar3;
    }
    else
    {
        result = EQ_STDCALL(int, 0x100322E4, (void))();
        EQ_CDECL(void, 0x10030660, (int))(result);
        memset(pDVar7, 0, 16);
    }
    result = EQ_STDCALL(int, 0x1003229C, (int, int, int, char *, int))(
        *(int *)(connection + 0x710), 0, 7, (char *)(connection + 0x714), 4);
    if (result != 0)
    {
        socketError = EQ_STDCALL(int, 0x100322E4, (void))();
        EQ_CDECL(void, 0x10030660, (int))(socketError);
        *(int *)(connection + 0x734) = 3;
    }
    return result;
}

// FUNCTION: EQMAIN 0x1002c010
// Cloned from trilogy connection_send_guess 0x004F3530 (the same original code: BinDiff sibling, names/siblings/).
int connection_send_guess(int connection, u32 payloadData, u32 payloadBytes, u32 value1, u32 value2)
{
    u32 payloadEntry[2];

    payloadEntry[0] = payloadData;
    payloadEntry[1] = payloadBytes;
    return EQ_CDECL(int, 0x1002C050, (int, u32 *, int, u32, u32))(
        connection, payloadEntry, 1, value1, value2);
}

// FUNCTION: EQMAIN 0x1002c050
// Queues a gather list on a connection: one unsequenced packet (max 512 B) or sequenced 512-byte fragments
// Cloned from trilogy connection_sendv_guess 0x004F3570 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Early returns (6, 0x13) take no lock; the unlock paths return `result`, which the original keeps in a
// stack slot and reads back after rdp_unlock (VC6 spill). Bugs kept.
int connection_sendv_guess(int arg1, u32 *iov, int arg3, u32 chanId, u32 arg5)
{
    char *chanCounter;
    short assignedSeq;
    int segLen;
    int connection;
    int segsLeft;
    int *outMsg;
    DWORD DVar6;
    u32 chunkLen;
    u32 fragLeft;
    u32 length;
    u32 *srcPtr;
    u16 fragCount;
    u16 temp;
    u32 *dstPtr;
    int result;
    int iovIndex;
    u32 fragLen;
    int *iovLenPtr;

    connection = arg1;
    length = 0;
    result = 0;
    if (arg1 == 0)
    {
        return 6;
    }
    if (arg3 != 0)
    {
        outMsg = (int *)iov + 1;
        segsLeft = arg3;
        do
        {
            segLen = *outMsg;
            outMsg = outMsg + 2;
            length = length + segLen;
            segsLeft = segsLeft + -1;
        } while (segsLeft != 0);
        if (0xc800 < length)
        {
            return 0x13;
        }
    }
    if (((arg5 & 1) == 0) && (0x200 < length))
    {
        return 0x13;
    }
    if ((chanId & 0xffffff00) != 0)
    {
        return 6;
    }
    if ((arg5 & 0xfffffffe) != 0)
    {
        return 6;
    }
    EQ_CDECL(int, 0x1002E6E0, (short *, short *))
        ((short *)(*(int *)(connection + 0xc) + 0x50), (short *)(connection + 0x478));
    if (*(int *)(connection + 0x6d4) == 0)
    {
        result = 0xe;
        EQ_CDECL(void, 0x10028BF0, (int))(connection);
        return result;
    }
    if (*(int *)(connection + 0x6d8) != 0)
    {
        result = 0x12;
        EQ_CDECL(void, 0x10028BF0, (int))(connection);
        return result;
    }
    if (*(int *)(connection + 0x6c8) != 0)
    {
        result = 0x11;
        EQ_CDECL(void, 0x10028BF0, (int))(connection);
        return result;
    }
    if (0x3ff < ((u16)(*(short *)(connection + 0x48c) - *(short *)(connection + 0x488)) - 1 & 0xffff) + 1)
    {
        result = 0x10;
        EQ_CDECL(void, 0x10028BF0, (int))(connection);
        return result;
    }
    if (*(u32 *)(connection + 0x57c) <
        (u32)(*(int *)(connection + 0x568) + *(int *)(connection + 0x54c) + *(int *)(connection + 0x530)))
    {
        result = 0xf;
        EQ_CDECL(void, 0x10028BF0, (int))(connection);
        return result;
    }
    if ((arg5 & 1) == 0)
    {
        temp = 0;
        if (chanId != 0)
        {
            temp = 0x1000;
        }
        outMsg = EQ_CDECL(int *, 0x100288B0, (u32))(length + 0x2c);
        *(u16 *)(outMsg + 4) = temp;
        *(char *)((int)outMsg + 0x1a) = (char)chanId;
        EQ_CDECL(void, 0x10030F30, (int))((int)outMsg);
        if (arg3 != 0)
        {
            chanId = arg3;
            do
            {
                length = iov[1];
                srcPtr = (u32 *)*iov;
                dstPtr = (u32 *)(outMsg[8] + 0x28 + (int)outMsg);
                memcpy(dstPtr, srcPtr, length);
                iov = iov + 2;
                chanId = chanId - 1;
                outMsg[8] = outMsg[8] + length;
            } while (chanId != 0);
        }
        EQ_CDECL(void, 0x1002BAE0, (int, int))(arg1, (int)outMsg);
    }
    else
    {
        arg1 = 0x200;
        fragCount = (u16)(length + 0x1ff >> 9);
        if (1 < fragCount)
        {
            arg1 = 0xa00;
        }
        iovIndex = 0;
        arg3 = 0;
        arg5 = 0;
        if (fragCount != 0)
        {
            do
            {
                fragLen = length;
                if (0x1ff < length)
                {
                    fragLen = 0x200;
                }
                outMsg = EQ_CDECL(int *, 0x100288B0, (u32))(fragLen + 0x32);
                if (outMsg == (int *)0x0)
                {
                    result = 2;
                    EQ_CDECL(void, 0x10028BF0, (int))(connection);
                    return result;
                }
                arg1 = arg1 & 0xefff;
                if ((chanId != 0) && ((short)arg5 == 0))
                {
                    arg1 = arg1 | 0x1000;
                }
                *(u16 *)(outMsg + 4) = (u16)arg1;
                EQ_CDECL(void, 0x10030E30, (int, u32))(connection + 0x494,
                    (u16)(*(short *)(connection + 0x48c) - *(short *)(connection + 0x488)) - 1 & 0xffff);
                DVar6 = rdp_timeGetTime_guess();
                *(DWORD *)(connection + 0x490) = DVar6;
                assignedSeq = *(short *)(connection + 0x48c);
                *(short *)(connection + 0x48c) = assignedSeq + 1;
                *(short *)((int)outMsg + 0x12) = assignedSeq;
                temp = *(u16 *)(connection + 0x48e);
                *(short *)((int)outMsg + 0x16) = (short)arg5;
                *(u16 *)(outMsg + 5) = temp;
                *(u16 *)(outMsg + 6) = fragCount;
                *(char *)((int)outMsg + 0x1a) = (char)chanId;
                *(u8 *)((int)outMsg + 0x1b) = *(u8 *)(connection + 0x6a8 + chanId);
                EQ_CDECL(void, 0x10030F30, (int))((int)outMsg);
                if (fragLen != 0)
                {
                    iovLenPtr = (int *)(iov + iovIndex * 2 + 1);
                    fragLeft = fragLen;
                    do
                    {
                        chunkLen = *iovLenPtr - arg3;
                        if (fragLeft <= (u32)(*iovLenPtr - arg3))
                        {
                            chunkLen = fragLeft;
                        }
                        srcPtr = (u32 *)(iovLenPtr[-1] + arg3);
                        dstPtr = (u32 *)(outMsg[8] + 0x28 + (int)outMsg);
                        memcpy(dstPtr, srcPtr, chunkLen);
                        outMsg[8] = outMsg[8] + chunkLen;
                        arg3 = arg3 + chunkLen;
                        if (arg3 == *iovLenPtr)
                        {
                            arg3 = 0;
                            iovIndex = iovIndex + 1;
                            iovLenPtr = iovLenPtr + 2;
                        }
                        fragLeft = fragLeft - chunkLen;
                    } while (fragLeft != 0);
                }
                if ((arg1 & 0x1000U) != 0)
                {
                    chanCounter = (char *)(connection + 0x6a8 + chanId);
                    *chanCounter = *chanCounter + '\x01';
                }
                EQ_CDECL(void, 0x1002BAE0, (int, int))(connection, (int)outMsg);
                length = length - fragLen;
                arg5 = arg5 + 1;
            } while ((u16)arg5 < fragCount);
        }
        if (1 < fragCount)
        {
            *(short *)(connection + 0x48e) = *(short *)(connection + 0x48e) + 1;
        }
    }
    EQ_CDECL(void, 0x10028AE0, (int, int))(connection, 1);
    EQ_CDECL(void, 0x10028BF0, (int))(connection);
    return result;
}

// FUNCTION: EQMAIN 0x1002c4a0
// Reports whether outgoing work is idle and selects the next timeout deadline.
// BinDiff sibling of trilogy NetworkConnection_UpdateConnectionTimeout_guess 0x004F39C0 (0.906). Here the three
// list fronts are GetRecordDelta_guess calls on tx_wait 0x534, tx_sent 0x518 and tx_hold 0x550 (rdp.h); trilogy inlines them.
// __cdecl. status[0] = 1 and status[1] = 0 when both fronts are empty and 0x6EC is clear; otherwise 0 and the
// bandwidth time (EQ client time when the word at 0x478 is 0x45), raised to the deadline below when later. Bugs kept.
void NetworkConnection_UpdateConnectionTimeout_guess(int connection, u32 *status)
{
    int temp;
    int sentHead;
    int bwTime;
    u32 altTime;
    u32 rawTimeout;
    u32 value;

    temp = EQ_CDECL(int, 0x1002E120, (int *))((int *)(connection + 0x534));
    sentHead = EQ_CDECL(int, 0x1002E120, (int *))((int *)(connection + 0x518));
    EQ_CDECL(int, 0x1002E120, (int *))((int *)(connection + 0x550));
    if (temp == 0 && sentHead == 0 && *(int *)(connection + 0x6ec) == 0)
    {
        status[1] = sentHead;
        *status = 1;
        return;
    }
    *status = 0;
    bwTime = EQ_CDECL(int, 0x10030D30, (int *))((int *)(connection + 0x56c));
    status[1] = bwTime;
    if (*(short *)(connection + 0x478) == 0x45)
    {
        altTime = EQ_CDECL(u32, 0x1002AFE0, (int))(*(int *)(connection + 0xc));
        status[1] = altTime;
    }
    if (temp != 0 && *(int *)(connection + 0x6c0) != 0)
        return;
    temp = *(int *)(connection + 0x6f0);
    if (sentHead != 0)
    {
        /* fake match: the expanded min/max clamp repeats its lower-bound comparison. */
        rawTimeout = (u32)*(u16 *)(connection + 0x69e) + (u32)*(u16 *)(connection + 0x6a0) * 2;
        value = (u32)*(u16 *)(connection + 0x69e) + (u32)*(u16 *)(connection + 0x6a0) * 2;
        sentHead = *(int *)(sentHead + 0xc) + (u16)
            ((rawTimeout > 50 ? rawTimeout : 50) < 65535 ?
             (value > 50 ? value : 50) : 65535);
        if (*(int *)(connection + 0x6ec) == 0 || temp - sentHead > 0)
            temp = sentHead;
    }
    if (temp - (int)status[1] > 0)
        status[1] = temp;
    return;
}

// FUNCTION: EQMAIN 0x1002c5b0
// Ends the connection, records its disconnect reason, and releases its queued packets.
// BinDiff sibling of trilogy NetworkConnection_Disconnect_guess 0x004F3AC0 (1.000/0.977). Here the three queue
// fronts are PopRecordDelta_guess calls on tx_hold 0x550, tx_wait 0x534 and tx_sent 0x518 (rdp.h). __cdecl, void. Frees
// every item of the three queues (item+0x20 comes off its queue's byte count at +0x18), runs the queue cleanup,
// counts a stat unless +0x6C0 is set, stores the reason at +0x6DC and counts a second stat by it (a 0x30000 reason
// takes its counter from the byte at +0x16C, index at +0x16D), then clears the close result and the close event.
void NetworkConnection_Disconnect_guess(RdpConnection_guess *conn, u32 disconnectCode)
{
    RdpList_guess *list;
    int item;
    DWORD DVar5;

    list = &conn->tx_hold;
    while (1)
    {
        item = (int)rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(item + 0x20);
        rdp_fast_free_guess((void *)item);
    }
    list = &conn->tx_wait;
    while (1)
    {
        item = (int)rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(item + 0x20);
        rdp_fast_free_guess((void *)item);
    }
    list = &conn->tx_sent;
    while (1)
    {
        item = (int)rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(item + 0x20);
        rdp_fast_free_guess((void *)item);
    }
    cleanup_connection_queues_guess((int)conn);
    if (conn->field_768 == 0)
    {
        RDP_STAT_guess(0x190)++;
    }
    conn->connected = 0;
    conn->field_780 = 1;
    conn->disconnect_code = disconnectCode;
    if (disconnectCode == 0x60000)
    {
        RDP_STAT_guess(0x1b0)++;
    }
    else
    {
        if (conn->field_17C > 0 && (DVar5 = rdp_timeGetTime_guess(), (int)(DVar5 - conn->field_178) < 10000))
        {
            conn->disconnect_code = 0x30000;
        }
        switch (disconnectCode)
        {
        case 0x30000:
            switch (conn->disconnect_reason)
            {
            case 3:
                if (conn->disconnect_reason2 < 0x10)
                    ++*(RdpU64_guess *)(rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x1d0);
                else
                    RDP_STAT_guess(0x278)++;
                break;
            case 4:
                RDP_STAT_guess(0x250)++;
                break;
            case 0xb:
                if (conn->disconnect_reason2 < 2)
                    ++*(RdpU64_guess *)(rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x258);
                else
                    RDP_STAT_guess(0x278)++;
                break;
            case 0xc:
                if (conn->disconnect_reason2 < 2)
                    ++*(RdpU64_guess *)(rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x268);
                else
                    RDP_STAT_guess(0x278)++;
                break;
            }
            break;
        case 0x10000:
            RDP_STAT_guess(0x1b8)++;
            break;
        case 0x40000:
            RDP_STAT_guess(0x1c8)++;
            break;
        case 0x50000:
            RDP_STAT_guess(0x1c0)++;
            break;
        }
    }
    if (conn->close_result != 0)
    {
        *conn->close_result = 0;
        conn->close_result = 0;
    }
    if (conn->close_event != 0)
    {
        rdp_uevent_signal_guess(conn->close_event);
        conn->close_event = 0;
    }
    conn->field_788 = 0;
}

// FUNCTION: EQMAIN 0x1002c830
// Completes a pending close (close result 1, close event signalled) and frees the queued outgoing packets.
// Cloned from trilogy NetworkConnection_handle_disconnect_request_guess 0x004F3D40 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Clears the close result (storing 1 through it) and signals and clears the close event, then frees every
// item of the hold, wait and sent queues (taking each item's size, item+0x20, off its queue's byte count), and
// sets the flag at +0x6D8.
void NetworkConnection_handle_disconnect_request_guess(RdpConnection_guess *connectionObject)
{
    RdpConnection_guess *conn = connectionObject;
    RdpList_guess *list;
    void *item;

    if (conn->close_result != 0)
    {
        *conn->close_result = 1;
        conn->close_result = 0;
    }
    if (conn->close_event != 0)
    {
        rdp_uevent_signal_guess(conn->close_event);
        conn->close_event = 0;
    }
    list = &conn->tx_hold;
    while (1 == 1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)((char *)list + 0x18) -= *(u32 *)((char *)item + 0x20);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_wait;
    while (1 == 1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)((char *)list + 0x18) -= *(u32 *)((char *)item + 0x20);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_sent;
    while (1 == 1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)((char *)list + 0x18) -= *(u32 *)((char *)item + 0x20);
        rdp_fast_free_guess(item);
    }
    conn->field_780 = 1;
}

// FUNCTION: EQMAIN 0x1002c900
// Defers the connection's acknowledgement response when none is already pending.
// Cloned from trilogy tx_set_delayed_ack_guess 0x004F3E10 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Schedules an acknowledgement 50 ms from now, unless one is already pending.
void tx_set_delayed_ack_guess(RdpConnection_guess *conn)
{
    if (conn->delayed_ack == 0)
    {
        conn->delayed_ack = 1;
        conn->delayed_ack_time = rdp_timeGetTime_guess() + 50;
    }
}

// FUNCTION: EQMAIN 0x1002cd70
// Transmits connection traffic to the remote endpoint, including pending acknowledgements.
// Cloned from trilogy NetworkConnection_SendPacket_guess 0x004F4280 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Sends one packet to the connection's remote address: through the serial queue when the family is 0x45, else
// through the socket. Returns the send result: 1 disconnects (reason 0x70000), 5 charges the quota by rate / 10, 0 charges
// it by the header and payload length and advances tx_serial.
int NetworkConnection_SendPacket_guess(RdpConnection_guess *connectionObject, int param_2, int param_3, int value)
{
    u16 networkOrderValue;
    int ackHeaderSize;
    int sendResult;
    struct SendPacketHeader_guess
    {
        u16 flags;
        u16 serial;
        u8 data[20];
    } local_header;
    struct SendPacketBuffer_guess
    {
        u16 *data;
        int size;
        int param_2;
        int param_3;
    } local_packet;

    local_header.flags = value;
    if (connectionObject->connected == 0)
    {
        local_header.flags |= 0x8000;
    }
    else if (connectionObject->timeout_armed != 0)
    {
        local_header.flags |= 0x400;
    }
    networkOrderValue = rdp_htons_guess(connectionObject->tx_serial);
    local_header.serial = networkOrderValue;
    ackHeaderSize = rdp_prepare_ack_header_guess(connectionObject, local_header.data, &local_header.flags);
    value = ackHeaderSize + 4;
    connectionObject->delayed_ack = 0;
    if (ackHeaderSize != 0)
    {
        if (param_2 != 0)
        {
            RDP_STAT_guess(0x38)++;
            connectionObject->stat[7]++;
        }
        else
        {
            RDP_STAT_guess(0x30)++;
            connectionObject->stat[6]++;
        }
    }
    local_packet.data = &local_header.flags;
    local_packet.param_2 = param_2;
    local_packet.size = value;
    local_packet.param_3 = param_3;
    networkOrderValue = rdp_htons_guess(local_header.flags);
    local_header.flags = networkOrderValue;
    if (connectionObject->remote.family == 0x45)
    {
        sendResult = rdp_send_packet_serial_guess(&connectionObject->rdp->serial, &local_packet, 2, &connectionObject->remote);
    }
    else
    {
        sendResult = rdp_send_packet_socket_guess(connectionObject->field_4A4, &local_packet, 2, &connectionObject->remote,
                                             connectionObject->rdp->unk_0x118, connectionObject->rdp->unk_0x11C);
        rdp_timeGetTime_guess();
    }
    RDP_STAT_guess(0x340)++;
    if (sendResult != 0)
    {
        if (RDP_STAT_guess(0x358) != 0)
            RDP_STAT_guess(0x350)++;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x358) = 1;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x35c) = 0;
        RDP_STAT_guess(0x348)++;
    }
    else
    {
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x358) = 0;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x35c) = 0;
    }
    switch (sendResult)
    {
        case 1:
            rdp_connection_disconnect_guess(connectionObject, 0x70000);
            break;
        case 5:
            rdp_bandwidth_update_guess(&connectionObject->bandwidth, connectionObject->bandwidth.rate / 10);
            break;
        case 0:
            rdp_bandwidth_update_guess(&connectionObject->bandwidth, value + 0x1c + param_3);
            connectionObject->tx_serial++;
            break;
    }
    return sendResult;
}

// FUNCTION: EQMAIN 0x10030a10
// Seeds the connection's round-trip statistics with one initial timing sample.
// @early-stop: VC6 moves EAX to ESI after the sum store and uses ECX for the trailing zero stores.
// Cloned from trilogy initializePacketRateController_guess 0x004F7930 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Clears the estimator and seeds it with one sample of weight 1; first is capped at 0xFFFF.
void initializePacketRateController_guess(RdpRttStats_guess *rtt, u32 first)
{
    volatile u32 *sum = &rtt->sum;
    u32 sq;
    {
        memset(rtt, 0, sizeof(*rtt));
        if (!(0xffff <= first))
        {
        }
        else
            first = 0xffff;
        rtt->samples[0].value = (u16)first;
        rtt->samples[0].weight = 1;
        rtt->weight_sum = 1;
        sq = 0xffff & first;
        rtt->mean = (u16)first;
        // Preserve the original's sum write before calculating the square.
        *sum = sq;
        sq *= sq;
    }
    rtt->sumsq_hi = 0;
    rtt->sumsq_lo = sq;
    rtt->deviation = 0;
}

// FUNCTION: EQMAIN 0x10030a80
// Updates rolling packet timing statistics from a new elapsed-time sample.
// @early-stop: VC6 /O2 keeps reversing the commutative sample-product operand loads; four load instructions differ.
// Cloned from trilogy update_network_statistics_guess 0x004F79A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Takes the sample at rtt->next out of the sums, stores value (capped at 0xFFFF) with weight 0x3FF / interval
// (at least 1), puts it back, then sets the mean and the deviation (0 unless the total weight is above 1).
void update_network_statistics_guess(RdpRttStats_guess *rtt, u32 elapsed, u32 interval)
{
    RdpRttSample_guess *sample;
    u32 index;
    u32 contribution;
    int weight;
    __int64 variance;

    index = rtt->next;
    sample = &rtt->samples[index];
    contribution = (u32)sample->weight * (u32)sample->value;
    rtt->sum -= contribution;
    *(unsigned __int64 *)&rtt->sumsq_lo -= (u32)sample->value * contribution;
    rtt->weight_sum = (u16)(rtt->weight_sum - sample->weight);

    if (elapsed >= 0xffff)
        elapsed = 0xffff;
    sample->value = (u16)elapsed;

    weight = 0x3ff / (int)(interval & 0xffff);
    if (weight < 1)
        weight = 1;
    rtt->samples[rtt->next].weight = (u16)weight;

    index = rtt->next;
    sample = &rtt->samples[index];
    contribution = (u32)sample->weight * (u32)sample->value;
    rtt->sum += contribution;
    *(unsigned __int64 *)&rtt->sumsq_lo += (u32)sample->value * contribution;
    rtt->weight_sum = (u16)(rtt->weight_sum + sample->weight);
    rtt->mean = (u16)(rtt->sum / rtt->weight_sum);

    if (1 < rtt->weight_sum)
    {
        variance = *(__int64 *)&rtt->sumsq_lo - (u32)rtt->mean * rtt->sum;
        rtt->deviation = (u16)(long)sqrt((double)variance / (double)((int)rtt->weight_sum - 1));
        rtt->next = (index + 1) & 0x3f;
        return;
    }
    rtt->deviation = 0;
    rtt->next = (index + 1) & 0x3f;
    return;
}
