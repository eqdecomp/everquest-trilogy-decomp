/* tx.c - the sending side of a connection: outgoing queues, acknowledgements, retransmission.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/tx.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <math.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))
#define setLinkedListProperties_by_addr_guess EQ_CDECL(void, 0x004F5460, (RdpList_guess *list, int sorted, RdpListCompare_guess compare))
#define rdp_list_remove_head_guess EQ_CDECL(void *, 0x004F5500, (RdpList_guess *list))
#define rdp_fast_free_guess EQ_CDECL(void, 0x004F4AC0, (void *p))
#define rdp_uevent_signal_guess EQ_CDECL(void, 0x004F78B0, (void **ev))

// FUNCTION: TRILOGY 0x004f24d0
// Initializes a connection's send session, including its peer address, sequence tracking, and timing.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
// @early-stop: Argument cleanup remains split into add esp,0x14 and add esp,0xc instead of one add esp,0x20.
void tx_init_guess(int connection, int rdpState, short *remoteAddress)
{
    short addressFamily;
    u16 markedPort;
    DWORD DVar3;
    int initialSequence;

    DVar3 = rdp_timeGetTime_guess();
    EQ_CDECL(void, 0x00522EA0, (unsigned int))(DVar3);
    *(RdpAddr_guess *)(connection + 0x4a8) = *(RdpAddr_guess *)remoteAddress;
    memset((void *)(connection + 0x7a8), 0, sizeof(RdpAddr_guess));
    addressFamily = *remoteAddress;
    switch ((u16)addressFamily)
    {
    case 2:
        *(u32 *)(connection + 0x4a4) = *(u32 *)(rdpState + 4);
        *(RdpAddr_guess *)(connection + 0x7a8) = *(RdpAddr_guess *)remoteAddress;
        markedPort = EQ_STDCALL(u16, 0x00500380, (u16))( *(u16 *)(connection + 0x7aa));
        markedPort = EQ_STDCALL(u16, 0x005003B0, (u16))(markedPort | 0x8000);
        *(u16 *)(connection + 0x7aa) = markedPort;
        break;
    case 6:
        *(u32 *)(connection + 0x4a4) = *(u32 *)(rdpState + 0x10);
        break;
    case 0x45:
        *(u32 *)(connection + 0x4a4) = 0xffffffff;
        break;
    }
    *(u32 *)(connection + 0x7b8) = *(u32 *)(rdpState + 0xc);
    initialSequence = EQ_CDECL(int, 0x00522EAD, (void))();
    *(short *)(connection + 0x4bc) = (short)initialSequence;
    *(short *)(connection + 0x4b8) = (short)initialSequence - 1;
    *(u16 *)(connection + 0x4ba) = 0;
    *(u16 *)(connection + 0x4be) = 0;
    DVar3 = rdp_timeGetTime_guess();
    *(DWORD *)(connection + 0x4c0) = DVar3;
    EQ_CDECL(void, 0x004F7C70, (void *))( (void *)(connection + 0x4c4));
    EQ_CDECL(void, 0x004F5440, (void *))( (void *)(connection + 0x5c8));
    EQ_CDECL(void, 0x004F5440, (void *))( (void *)(connection + 0x5e0));
    EQ_CDECL(void, 0x004F5440, (void *))( (void *)(connection + 0x5f8));
    EQ_CDECL(void, 0x004F7B30, (void *))( (void *)(connection + 0x610));
    *(u32 *)(connection + 0x620) = 8000;
    memset((void *)(connection + 0x750), 0, 20);
    DVar3 = rdp_timeGetTime_guess();
    *(DWORD *)(connection + 0x624) = DVar3;
    *(u32 *)(connection + 0x628) = 0;
    EQ_CDECL(void, 0x004F7930, (void *, int, int))( (void *)(connection + 0x630), 500, 1);
    *(u32 *)(connection + 0x764) = 0;
    *(u32 *)(connection + 0x768) = 0;
    *(u16 *)(connection + 0x76c) = *(u16 *)(connection + 0x4bc);
    *(u32 *)(connection + 0x770) = 0;
    *(u32 *)(connection + 0x774) = 0;
    *(u16 *)(connection + 0x778) = 0;
    *(u32 *)(connection + 0x77c) = 1;
    *(u32 *)(connection + 0x780) = 0;
    *(u32 *)(connection + 0x784) = 0;
    *(u32 *)(connection + 0x788) = 0;
    *(u32 *)(connection + 0x78c) = 10000;
    *(u32 *)(connection + 0x790) = 5000;
    *(u32 *)(connection + 0x794) = 0;
    *(u32 *)(connection + 0x798) = 0;
    *(u16 *)(connection + 0x79c) = 0;
    *(u32 *)(connection + 0x7a0) = 0;
    *(u32 *)(connection + 0x7a4) = 0;
    *(u32 *)(connection + 0x7bc) = *(u32 *)(rdpState + 0x44);
    return;
}

// FUNCTION: TRILOGY 0x004f26d0
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
// __cdecl. Empties the sent, wait and hold queues (byte counts 0, unsorted lists). Returns 0.
int tx_create_guess(RdpConnection_guess *conn)
{
    RdpList_guess *list;

    list = &conn->tx_sent;
    *(u32 *)(list + 1) = 0;
    setLinkedListProperties_by_addr_guess(list, 0, 0);
    list = &conn->tx_wait;
    *(u32 *)(list + 1) = 0;
    setLinkedListProperties_by_addr_guess(list, 0, 0);
    list = &conn->tx_hold;
    *(u32 *)(list + 1) = 0;
    setLinkedListProperties_by_addr_guess(list, 0, 0);
    return 0;
}

// FUNCTION: TRILOGY 0x004f2730
// Clears pending send queues and completes any wait on connection close.
// ORIGINAL FILE: Network/rdp/tx.c (address: after tx_create)
// __cdecl. Frees every item of the hold, wait and sent queues (taking each item's size, item+0x28, off the
// queue's byte count), clears the close result pointer (storing 0 through it) and signals the close event.
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
        *(u32 *)(list + 1) -= *(u32 *)((char *)(((char *)item + 0x28)));
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_wait;
    while (1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)((char *)item + 0x28);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_sent;
    while (1)
    {
        unsigned long *temp;
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        temp = (u32 *)((char *)item + 0x28);
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

// FUNCTION: TRILOGY 0x004f27f0
// Applies an acknowledged packet to the connection's send state and updates its round-trip estimate.
void NetworkConnection_phandle_packet_acknowledgment_guess(RdpConnection_guess *connection, u32 sequence)
{
    char *item;
    int elapsed;
    unsigned __int64 *stat;

    item = (char *)EQ_CDECL(void *, 0x004F7DF0, (RdpList_guess *, u32))(&connection->tx_sent, sequence);
    RDP_STAT_guess(0x88)++;
    RDP_CSTAT_guess(connection, 0xAC)++;
    if (item != 0)
    {
        elapsed = rdp_timeGetTime_guess() - *(int *)(item + 0x10);
        RDP_STAT_guess(0x198)++;
        RDP_CSTAT_guess(connection, 0x100)++;
        if (*(u16 *)(item + 0x24) == 1)
        {
            stat = &RDP_STAT_guess(0x190);
            ++*stat;
            RDP_CSTAT_guess(connection, 0xFC)++;
            EQ_CDECL(void, 0x004F79A0, (RdpRttStats_guess *, int, u16))(
                &connection->rtt, elapsed, *(u16 *)(item + 0x24));
            connection->last_rtt = (u16)elapsed;
        }
        rdp_fast_free_guess(item);
    }
    if ((connection->field_764 != 0) && (connection->field_768 == 0))
    {
        connection->field_768 = 1;
        stat = &RDP_STAT_guess(0x170);
        ++*stat;
        connection->field_78C = 120000;
        connection->field_790 = 30000;
    }
    if (connection->field_770 != 0)
    {
        if ((connection->field_774 == 0) && ((u16)sequence == connection->field_778))
            connection->field_774 = 1;
    }
    if ((connection->field_770 != 0) && (connection->tx_acked == connection->field_778))
    {
        if (connection->close_result != 0)
        {
            *connection->close_result = 1;
            connection->close_result = 0;
        }
        if (connection->close_event != 0)
        {
            rdp_uevent_signal_guess(connection->close_event);
            connection->close_event = 0;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004f2990
// Rejects acknowledgments with invalid flags, empty masks, or sequence numbers outside the send window.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
// @early-stop: Entry register choice, flag-string registers, ACK loop, counter addressing and return scheduling still differ.
int tx_validate_ack_arrival(u8 *connection, u16 *ackHeader, int *ackBytes)
{
    u8 maskBytes;
    u32 maskBits, lastBit;
    int bit;
    u16 flags;
    short logFlags;
    u16 *ackData;
    int connectionAddress;
    short delta;
    u8 *flag2000Text;
    u8 *statsOrFlagText;
    u16 *ackMask;
    char *fragmentText;
    short ackSequence;
    char *messageIdText;
    char *multiText;
    u32 highestAck;
    u8 addressText[64];

    flags = *ackHeader;
    ackData = *(u16 **)(ackHeader + 2);
    *ackBytes = 0;
    statsOrFlagText = EQ_GLOBAL(u8 *, 0x0057F698);
    if (((flags & 4) != 0) && ((flags & 8) != 0))
    {
        connectionAddress = (int)connection;
        EQ_CDECL(void, 0x004F1230, (char *, void *))( (char *)addressText, (void *)(connectionAddress + 0x4a8));
        logFlags = *ackHeader;
        multiText = (logFlags & 0x100) != 0 ?
            (char *)EQ_ARRAY(char, 0x0057E950) : (char *)EQ_ARRAY(char, 0x0058FBA4);
        connection = (u8 *)EQ_ARRAY(char, 0x0057E948);
        if ((logFlags & 1) == 0)
            connection = (u8 *)EQ_ARRAY(char, 0x0058FBA4);
        fragmentText = (logFlags & 0x800) != 0 ?
            (char *)EQ_ARRAY(char, 0x0057E940) : (char *)EQ_ARRAY(char, 0x0058FBA4);
        messageIdText = (logFlags & 0x200) != 0 ?
            (char *)EQ_ARRAY(char, 0x0057E938) : (char *)EQ_ARRAY(char, 0x0058FBA4);
        statsOrFlagText = (logFlags & 0x4000) != 0 ?
            (u8 *)EQ_ARRAY(char, 0x0057E930) : (u8 *)EQ_ARRAY(char, 0x0058FBA4);
        flag2000Text = (logFlags & 0x2000) != 0 ?
            (u8 *)EQ_ARRAY(char, 0x0057E928) : (u8 *)EQ_ARRAY(char, 0x0058FBA4);
        EQ_CDECL(void, 0x004F66F0, (const char *, ...))(
            (const char *)EQ_ARRAY(char, 0x0057E8F8), addressText, flags, flag2000Text, statsOrFlagText, messageIdText, fragmentText,
            connection, multiText);
        ++g_rdp_stats_guess->c[35];
        *(int *)(connectionAddress + 0xec) = *(int *)(connectionAddress + 0xec) + 1;
        return 2;
    }
    if ((*ackHeader & 0xc) != 0)
    {
        ackMask = ackData + 1;
        highestAck = EQ_STDCALL(int, 0x00500380, (u16))(*ackData);
        maskBytes = (u8)(flags >> 4) & 15;
        *ackBytes = 2;
        if (maskBytes != 0)
        {
            maskBits = maskBytes * 8;
            lastBit = maskBits;
            *ackBytes = (u16)maskBytes + 2;
            for (bit = maskBits - 8; (int)(u16)bit < (short)maskBits; ++bit)
            {
                if (EQ_CDECL(char, 0x004F7DD0, (u16 *, u32))(ackMask, (u16)bit))
                    lastBit = bit;
            }
            if ((short)lastBit == (short)maskBits)
            {
                EQ_CDECL(void, 0x004F1230, (char *, void *))((char *)addressText,
                    (void *)(connection + 0x4a8));
                EQ_CDECL(void, 0x004F66F0, (const char *, ...))(
                    (const char *)EQ_ARRAY(char, 0x0057E8D0), addressText,
                    *(u8 *)(((u16)maskBits / 8 - 1) + (int)ackMask));
                statsOrFlagText = EQ_GLOBAL(u8 *, 0x0057F698) + 0x120;
                ++*(unsigned __int64 *)statsOrFlagText;
                *(int *)(connection + 0xf0) = *(int *)(connection + 0xf0) + 1;
                return 2;
            }
            highestAck += lastBit + 1;
        }
        ackSequence = (short)highestAck;
        delta = ackSequence - *(short *)(connection + 0x4bc);
        if ((-1 < delta) || (delta < -0x1000))
        {
            EQ_CDECL(void, 0x004F1230, (char *, void *))( (char *)addressText,
                (void *)(connection + 0x4a8));
            EQ_CDECL(void, 0x004F66F0, (const char *, ...))(
                (const char *)EQ_ARRAY(char, 0x0057E8A8), addressText,
                (int)(short)(ackSequence - *(short *)(connection + 0x4bc)), highestAck & 0xffff,
                *(u16 *)(connection + 0x4bc));

            if ((short)(ackSequence - *(short *)(connection + 0x4bc)) >= 0)
            {
                statsOrFlagText = EQ_GLOBAL(u8 *, 0x0057F698) + 0xa0;
                ++*(unsigned __int64 *)statsOrFlagText;
                *(int *)(connection + 0xb8) = *(int *)(connection + 0xb8) + 1;
                return 1;
            }
            statsOrFlagText = EQ_GLOBAL(u8 *, 0x0057F698) + 0x128;
            ++*(unsigned __int64 *)statsOrFlagText;
            *(int *)(connection + 0xf8) = *(int *)(connection + 0xf8) + 1;
            return 1;
        }
    }
    else if ((flags & 0xf0) != 0)
    {
        statsOrFlagText = EQ_GLOBAL(u8 *, 0x0057F698) + 0x130;
        ++*(unsigned __int64 *)statsOrFlagText;
        *(int *)(connection + 0xf4) = *(int *)(connection + 0xf4) + 1;
        return 2;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004f2cd0
// Applies peer acknowledgments to outstanding sends and advances the acknowledged sequence window.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
void tx_record_ack_arrival(int connection, u16 *packet)
{
    u16 ackSize;
    char bitValue;
    u16 sequence;
    int ptr;
    short skipBits;
    u16 shiftBits;
    short ackSpan;
    u16 tmp;
    int maskBits;
    u16 seqTemp;
    u32 bitIndex;
    u16 mask_offset;
    int count;
    int *hold;
    short ackedCount;
    u16 *ackPtr;
    char debugText[252];

    ackPtr = *(u16 **)(packet + 2);
    mask_offset = 0;
    tmp = *packet;
    shiftBits = 0;
    ackedCount = 0;
    if ((tmp & 4) != 0)
    {
        sequence = EQ_STDCALL(u16, 0x00500380, (u16))(*ackPtr++);
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
            debugText, (const char *)EQ_ARRAY(char, 0x0057E968), sequence);
        if (0 < (short)(sequence - *(short *)(connection + 0x4b8)))
        {
            ptr = connection + 0x4c4;
            do
            {
                bitValue = EQ_CDECL(char, 0x004F7DD0, (u16 *, u32))((u16 *)ptr, (u32)shiftBits);
                if (bitValue != '\0')
                {
                    EQ_CDECL(void, 0x004F27F0, (int, u16))(
                        connection, (u16)(*(short *)(connection + 0x4b8) + shiftBits) + 1);
                    ackedCount = ackedCount + 1;
                }
                shiftBits = shiftBits + 1;
                ackSpan = sequence - *(short *)(connection + 0x4b8);
            } while (0 < (short)(ackSpan - shiftBits));
            if (shiftBits > 0)
            {
                EQ_CDECL(void, 0x004F7C90, (void *, void *, int, int))(
                    (void *)ptr, (void *)ptr, shiftBits, 0x100);
                *(u16 *)(connection + 0x4b8) = sequence;
                hold = (int *)(connection + 0x5f8);
                while ((ptr = *hold, ptr != 0 && *(int *)(ptr + 8) != 0))
                {
                    if (ptr != 0)
                        ptr = *(int *)(ptr + 8);
                    else
                        ptr = 0;
                    if (0 <= (short)((*(short *)(ptr + 0x1a) - *(short *)(connection + 0x4b8)) - 0x78))
                        break;
                    ptr = EQ_CDECL(void *, 0x004F5500, (void *))(hold);
                    if (ptr != 0)
                        hold[5] -= *(int *)(ptr + 0x28);
                    EQ_CDECL(void, 0x004F3000, (int, int))(connection, ptr);
                }
            }
        }
        mask_offset = sequence - *(short *)(connection + 0x4b8);
    }
    else if ((tmp & 8) != 0)
    {
        sequence = EQ_STDCALL(u16, 0x00500380, (u16))(*ackPtr++);
        seqTemp = sequence - *(short *)(connection + 0x4b8);
        mask_offset = seqTemp;
        if ((0 < (short)seqTemp) &&
            (bitValue = EQ_CDECL(char, 0x004F7D90, (u8 *, u32))((u8 *)(connection + 0x4c4), mask_offset - 1), bitValue != '\0'))
        {
            EQ_CDECL(void, 0x004F27F0, (int, u16))(connection, sequence);
            ackedCount = 1;
        }
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
            debugText, (const char *)EQ_ARRAY(char, 0x0057E958), sequence);
    }
    maskBits = (u8)(tmp >> 4) & 0xf;
    ackSize = maskBits + 2;
    maskBits <<= 3;
    if ((*packet & 0xc) != 0)
    {
        if (packet[10] == 0)
        {
            g_rdp_stats_guess->c[15] += 1;
            *(int *)(connection + 0xa4) = *(int *)(connection + 0xa4) + 1;
        }
        else
        {
            g_rdp_stats_guess->c[16] += 1;
            *(int *)(connection + 0xa8) = *(int *)(connection + 0xa8) + 1;
        }
    }
    skipBits = 0;
    if ((short)mask_offset < 0)
    {
        skipBits = mask_offset;
        mask_offset = 0;
        skipBits = -skipBits;
        maskBits = (short)(maskBits - skipBits);
    }
    count = (short)maskBits;
    tmp = 0;
    if (0 < count)
    {
        bitIndex = 0;
        do
        {
            bitValue = EQ_CDECL(char, 0x004F7DD0, (u16 *, u32))(ackPtr, (skipBits & 0xffff) + bitIndex);
            if ((bitValue != '\0') &&
                (bitValue = EQ_CDECL(char, 0x004F7D90, (u8 *, u32))((u8 *)(connection + 0x4c4), mask_offset + bitIndex), bitValue != '\0'))
            {
                seqTemp = *(short *)(connection + 0x4b8) + tmp;
                seqTemp += 1;
                seqTemp += mask_offset;
                EQ_CDECL(void, 0x004F27F0, (int, u16))(
                    connection, seqTemp);
                ackedCount = ackedCount + 1;
            }
            tmp = tmp + 1;
            bitIndex = (u32)tmp;
        } while ((int)bitIndex < count);
    }
    if (ackedCount == 0)
    {
        g_rdp_stats_guess->c[18] += 1;
        g_rdp_stats_guess->c[19] += (int)(u16)ackSize;
        *(int *)(connection + 0xb0) = *(int *)(connection + 0xb0) + 1;
        *(u32 *)(connection + 0xb4) = *(int *)(connection + 0xb4) + ackSize;
    }
    return;
}

/* fake match: volatile packet flags preserve the post-send reads; shared goto tails preserve VC6's block order. */
// FUNCTION: TRILOGY 0x004f3000
// Schedules outgoing connection traffic for delivery, deferring it when bandwidth is unavailable.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
// @early-stop: stat and queue-path register allocation differs; the rewrite has three fewer instructions.
void tx_enqueue_outgoing(int connection, int outgoingMessage)
{
    u8 *stats;
    DWORD DVar3;
    u32 bucketBytes;
    int targetQueue;
    int messageQueue;
    int dequeuedMessage;
    int belowBandwidthThreshold;

    if ((*(u16 *)(outgoingMessage + 0x18) & 0x200) != 0)
    {
        if (*(int *)(connection + 0x764) == 0)
        {
            *(u16 *)(outgoingMessage + 0x18) = *(u16 *)(outgoingMessage + 0x18) | 0x2000;
            *(u32 *)(connection + 0x764) = 1;
            DVar3 = rdp_timeGetTime_guess();
            *(short *)(outgoingMessage + 0x24) = *(short *)(outgoingMessage + 0x24) + 1;
            *(DWORD *)(outgoingMessage + 0x14) = DVar3;
            *(DWORD *)(outgoingMessage + 0x10) = DVar3;
            EQ_CDECL(void, 0x004F4280, (int, void *, u32, u16))(
                connection, (void *)(outgoingMessage + 0x30), *(u32 *)(outgoingMessage + 0x28),
                *(u16 *)(outgoingMessage + 0x18));
            stats = EQ_GLOBAL(u8 *, 0x0057F698);
            if ((*(volatile u8 *)(outgoingMessage + 0x19) & 2) != 0)
            {
                RDP_STAT_guess(0x10)++;
                RDP_STAT_guess(0x18) += *(u32 *)(outgoingMessage + 0x28);
                *(int *)(connection + 0x70) = *(int *)(connection + 0x70) + 1;
                targetQueue = connection + 0x5c8;
                *(int *)(connection + 0x74) = *(int *)(connection + 0x74) + *(int *)(outgoingMessage + 0x28);
                goto enqueueMessage;
            }
            goto freeMessage;
        }
    }
    if ((*(u16 *)(outgoingMessage + 0x18) & 0x200) != 0 &&
        ((short)((*(short *)(outgoingMessage + 0x1a) - *(short *)(connection + 0x4b8)) + -0x78) >= 0))
    {
        targetQueue = connection + 0x5f8;
        goto enqueueMessage;
    }
    if (*(int *)(connection + 0x768) != 0)
    {
        bucketBytes = EQ_CDECL(u32, 0x004F7BD0, (void *))((void *)(connection + 0x610));
        belowBandwidthThreshold = bucketBytes < (*(u32 *)(connection + 0x618) >> 3);
        if (*(short *)(connection + 0x4a8) == 0x45)
        {
            if ((belowBandwidthThreshold) &&
                (targetQueue = EQ_CDECL(int, 0x004F17A0, (u32))(*(u32 *)(connection + 0xc)), targetQueue != 0))
            {
sendQueuedMessage:
                if ((*(int *)(connection + 0x5e0) != 0) &&
                    (*(int *)(*(int *)(connection + 0x5e0) + 8) != 0))
                {
                    messageQueue = connection + 0x5e0;
                    *(int *)(messageQueue + 0x14) = *(int *)(outgoingMessage + 0x28) + *(int *)(messageQueue + 0x14);
                    EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)messageQueue, (void *)outgoingMessage);
                    dequeuedMessage = (int)rdp_list_remove_head_guess((RdpList_guess *)messageQueue);
                    if (dequeuedMessage != 0)
                    {
                        *(int *)(messageQueue + 0x14) = *(int *)(messageQueue + 0x14) - *(int *)(dequeuedMessage + 0x28);
                    }
                    outgoingMessage = dequeuedMessage;
                }
                DVar3 = rdp_timeGetTime_guess();
                *(short *)(outgoingMessage + 0x24) = *(short *)(outgoingMessage + 0x24) + 1;
                *(DWORD *)(outgoingMessage + 0x14) = DVar3;
                *(DWORD *)(outgoingMessage + 0x10) = DVar3;
                EQ_CDECL(void, 0x004F4280, (int, void *, u32, u16))(
                    connection, (void *)(outgoingMessage + 0x30), *(u32 *)(outgoingMessage + 0x28),
                    *(u16 *)(outgoingMessage + 0x18));
                stats = EQ_GLOBAL(u8 *, 0x0057F698);
                if ((*(volatile u8 *)(outgoingMessage + 0x19) & 2) != 0)
                {
                    RDP_STAT_guess(0x10)++;
                    RDP_STAT_guess(0x18) += *(u32 *)(outgoingMessage + 0x28);
                    *(int *)(connection + 0x70) = *(int *)(connection + 0x70) + 1;
                    targetQueue = connection + 0x5c8;
                    *(int *)(connection + 0x74) = *(int *)(connection + 0x74) + *(int *)(outgoingMessage + 0x28);
                    goto enqueueMessage;
                }
freeMessage:
                *((unsigned __int64 *)stats) += 1;
                stats = EQ_GLOBAL(u8 *, 0x0057F698);
                *((unsigned __int64 *)(stats + 8)) += *(u32 *)(outgoingMessage + 0x28);
                *(int *)(connection + 0x68) = *(int *)(connection + 0x68) + 1;
                *(int *)(connection + 0x6c) = *(int *)(connection + 0x6c) + *(int *)(outgoingMessage + 0x28);
                rdp_fast_free_guess((void *)outgoingMessage);
                return;
            }
        }
        else if (belowBandwidthThreshold)
            goto sendQueuedMessage;
    }
    targetQueue = connection + 0x5e0;
enqueueMessage:
    *(int *)(targetQueue + 0x14) = *(int *)(targetQueue + 0x14) + *(int *)(outgoingMessage + 0x28);
    EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)targetQueue, (void *)outgoingMessage);
    return;
}

// FUNCTION: TRILOGY 0x004f3260
// Starts an orderly close by sending a reliable final packet.
int NetworkConnection_send_disconnect_message_guess(RdpConnection_guess *conn)
{
    int msg;
    DWORD now;
    int result = 0;
    u16 sequence;

    if (conn->field_770 == 0 && conn->connected != 0 && conn->field_780 == 0)
    {
        /* 0x32 = RdpMsgOutgoing_guess header (0x30) + the 2-byte seq */
        msg = EQ_CDECL(int, 0x004F48C0, (int))(0x32);
        if (msg == 0)
            result = 2;
        else
        {
            EQ_CDECL(void, 0x004F7D50, (void *, u32))(
                (void *)&conn->tx_sent_bits,
                (u16)(conn->tx_next - conn->tx_acked - 1));
            /* The zero-argument timer thunk has the same stack contract as cdecl. */
            now = EQ_CDECL(DWORD, 0x004F66D0, (void))();
            conn->tx_time = now;
            sequence = conn->tx_next;
            conn->tx_next = sequence + 1;
            conn->field_778 = sequence;
            /* 0x4000 = fin, 0x200 = seq present */
            *(short *)(msg + 0x18) = 0x4200;
            *(short *)(msg + 0x1a) = conn->field_778;
            EQ_CDECL(void, 0x004F7EB0, (void *))((void *)msg);
            EQ_CDECL(void, 0x004F3000, (RdpConnection_guess *, void *))(conn, (void *)msg);
            conn->field_770 = 1;
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x004f3330
// Queues a sequence-numbered connection control packet; its specific control meaning is unclear.
int NetworkConnection_SendConnectionAcknowledgement_guess(RdpConnection_guess *conn)
{
    short sequence;
    int msg;
    DWORD now;
    int result = 0;

    if (conn->field_770 == 0 && conn->connected != 0 && conn->field_780 == 0)
    {
        /* 0x32 = RdpMsgOutgoing_guess header (0x30) + the 2-byte seq */
        msg = EQ_CDECL(int, 0x004F48C0, (int))(0x32);
        if (msg == 0)
            result = 2;
        else
        {
            /* 0x200 = seq present; 0x1 not identified */
            *(u16 *)(msg + 0x18) = 0x201;
            EQ_CDECL(void, 0x004F7D50, (void *, u32))(
                (void *)&conn->tx_sent_bits,
                (u16)(conn->tx_next - conn->tx_acked - 1));
            /* The zero-argument timer thunk has the same stack contract as cdecl. */
            now = EQ_CDECL(DWORD, 0x004F66D0, (void))();
            conn->tx_time = now;
            sequence = conn->tx_next;
            conn->tx_next = sequence + 1;
            *(short *)(msg + 0x1a) = sequence;
            EQ_CDECL(void, 0x004F7EB0, (void *))((void *)msg);
            EQ_CDECL(void, 0x004F3000, (RdpConnection_guess *, void *))(conn, (void *)msg);
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x004f33e0
// Sends a 1-byte probe to the peer with a stepped IP TTL (traceroute-style); logs its send time and TTL.
int NetworkConnection_SendHeartbeat_guess(int connection)
{
    int connectionBase = connection;
    DWORD DVar3;
    int socketResult;
    int socketError;
    DWORD *pDVar7;
    u32 socketOptionValue;
    BYTE heartbeat;

    pDVar7 = (DWORD *)(*(int *)(connection + 0x7c0) + *(int *)(connection + 0x7e0) * 0x10);
    DVar3 = rdp_timeGetTime_guess();
    *pDVar7 = DVar3;
    *(BYTE *)(pDVar7 + 3) = *(BYTE *)(connectionBase + 0x7d4);
    socketOptionValue = *(BYTE *)(pDVar7 + 3);
    heartbeat = *(BYTE *)(connectionBase + 0x7e0);
    socketResult = EQ_STDCALL(int, 0x0050039E, (int, int, int, char *, int))(
        *(int *)(connectionBase + 0x7b8), 0, 7, (char *)&socketOptionValue, 4);
    if (socketResult != 0)
    {
        socketError = EQ_STDCALL(int, 0x005003DA, (void))();
        EQ_CDECL(void, 0x004F73C0, (int))(socketError);
        *(int *)(connectionBase + 0x7dc) = 3;
        return socketResult;
    }
    socketResult = EQ_STDCALL(int, 0x00500392, (int, char *, int, int, void *, int))(
        *(int *)(connectionBase + 0x7b8), (char *)&heartbeat, 1, 0,
        (void *)(connectionBase + 0x7a8), 0x10);
    if (socketResult == 1)
    {
        if (++*(u32 *)(connectionBase + 0x7d4) > *(u32 *)(connectionBase + 0x7d8))
        {
            *(int *)(connectionBase + 0x7d4) = 1;
            *(int *)(connectionBase + 0x7dc) = *(int *)(connectionBase + 0x7dc) + 1;
        }
        *(int *)(connectionBase + 0x7c4) = 1;
        *(int *)(connectionBase + 0x7e0) = *(int *)(connectionBase + 0x7e0) + 1;
        DVar3 = rdp_timeGetTime_guess();
        *(DWORD *)(connectionBase + 0x7c8) = DVar3;
    }
    else
    {
        socketResult = EQ_STDCALL(int, 0x005003DA, (void))();
        EQ_CDECL(void, 0x004F73C0, (int))(socketResult);
        memset(pDVar7, 0, 16);
    }
    socketResult = EQ_STDCALL(int, 0x0050039E, (int, int, int, char *, int))(
        *(int *)(connectionBase + 0x7b8), 0, 7, (char *)(connectionBase + 0x7bc), 4);
    if (socketResult != 0)
    {
        socketError = EQ_STDCALL(int, 0x005003DA, (void))();
        EQ_CDECL(void, 0x004F73C0, (int))(socketError);
        *(int *)(connectionBase + 0x7dc) = 3;
    }
    return socketResult;
}

// FUNCTION: TRILOGY 0x004f3530
// A one-buffer send wrapper; packet handling is passed to the vector-send path.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
int connection_send(int conn, u32 packet, u32 length, u32 channel, u32 flags)
{
    // {buffer, length} pair: a one-chunk list for connection_sendv
    u32 chunk[2];

    chunk[0] = packet;
    chunk[1] = length;
    // flags: bit 0 = reliable; other bits must be 0
    return EQ_CDECL(int, 0x004F3570, (int, u32 *, int, u32, u32))(
        conn, chunk, 1, channel, flags);
}

// FUNCTION: TRILOGY 0x004f3570
// Queues a buffer list on a channel: flag 1 splits it into 512-byte sequenced fragments, else one plain packet.
// @early-stop: Return-status cleanup scheduling, fragment header loads, and unfragmented length slot still differ.
// ORIGINAL FILE: Network/rdp/tx.c (from Mac debug map)
int connection_sendv(int connection, u32 *chunk, u32 chunkCount, u32 channelId, u32 flags)
{
    /* fake match: volatile result reads retain the original stack status across unlock. */
    int result = 0;
    char *channelSeq;
    short sequence;
    u16 msgFlags;
    u16 fragIndex;
    int *lengthPtr;
    DWORD DVar7;
    u32 copyLen;
    int message;
    u32 fragLeft;
    u32 totalSize;
    u32 *src;
    u16 fragCount;
    u16 tmp;
    u32 *dest;
    int chunkIndex;
    u32 fragLen;
    int *chunkLenPtr;

    totalSize = 0;
    if (connection == 0)
        return 6;
    if (chunkCount > 0)
    {
        lengthPtr = (int *)chunk + 1;
        message = chunkCount;
        do
        {
            totalSize += *lengthPtr;
            lengthPtr += 2;
            message = message + -1;
        } while (message != 0);
    }
    if (totalSize <= 0xc800 && ((flags & 1) != 0 || totalSize <= 0x200))
    {
        if ((channelId & 0xffffff00) != 0)
            return 6;
        if ((flags & 0xfffffffe) != 0)
            return 6;
        EQ_CDECL(void, 0x004F51C0, (int, int))(
            *(int *)(connection + 0xc) + 0x50, connection + 0x4a8);
        if (*(int *)(connection + 0x77c) == 0)
        {
            result = 0xd;
            EQ_CDECL(void, 0x004EF240, (int))(connection);
            return *(volatile int *)&result;
        }
        if (*(int *)(connection + 0x780) != 0)
        {
            result = 0x11;
            EQ_CDECL(void, 0x004EF240, (int))(connection);
            return *(volatile int *)&result;
        }
        if (*(int *)(connection + 0x770) != 0)
        {
            result = 0x10;
            EQ_CDECL(void, 0x004EF240, (int))(connection);
            return *(volatile int *)&result;
        }
        if (((u16)(*(short *)(connection + 0x4bc) - *(short *)(connection + 0x4b8) - 1)) + 1 >= 0x800)
        {
            result = 0xf;
            EQ_CDECL(void, 0x004EF240, (int))(connection);
            return *(volatile int *)&result;
        }
        if ((u32)(*(int *)(connection + 0x60c) + *(int *)(connection + 0x5f4) + *(int *)(connection + 0x5dc)) >
            *(u32 *)(connection + 0x620))
        {
            result = 0xe;
            EQ_CDECL(void, 0x004EF240, (int))(connection);
            return *(volatile int *)&result;
        }
        if ((flags & 1) != 0)
        {
            u32 remaining;
            msgFlags = 0x200;
            fragCount = (u16)(totalSize + 0x1ff >> 9);
            remaining = totalSize;
            if (1 < fragCount)
                msgFlags = 0xa00;
            chunkIndex = 0;
            chunkCount = 0;
            fragIndex = 0;
            if (fragCount > 0)
            {
                do
                {
                    if (remaining < 0x200)
                        fragLen = remaining;
                    else
                        fragLen = 0x200;
                    message = EQ_CDECL(int, 0x004F48C0, (u32))(fragLen + 0x3a);
                    if (message != 0)
                    {
                        msgFlags = msgFlags & 0xefff;
                        if ((channelId != 0) && (fragIndex == 0))
                            msgFlags = msgFlags | 0x1000;
                        *(u16 *)(message + 0x18) = msgFlags;
                        EQ_CDECL(void, 0x004F7D50, (int, int))(
                            connection + 0x4c4,
                            (u16)(*(short *)(connection + 0x4bc) - *(short *)(connection + 0x4b8) - 1));
                        /* With no arguments, the import thunk has the same cdecl/stdcall ABI. */
                        DVar7 = EQ_CDECL(DWORD, 0x004F66D0, (void))();
                        *(DWORD *)(connection + 0x4c0) = DVar7;
                        sequence = *(short *)(connection + 0x4bc);
                        *(short *)(connection + 0x4bc) = sequence + 1;
                        *(short *)(message + 0x1a) = sequence;
                        *(u16 *)(message + 0x1e) = fragIndex;
                        tmp = *(u16 *)(connection + 0x4be);
                        *(u16 *)(message + 0x1c) = tmp;
                        *(u16 *)(message + 0x20) = fragCount;
                        *(char *)(message + 0x22) = (char)channelId;
                        *(u8 *)(message + 0x23) = *(u8 *)(connection + 0x750 + channelId);
                        EQ_CDECL(void, 0x004F7EB0, (int))(message);
                        fragLeft = fragLen;
                        if (fragLeft > 0)
                        {
                            chunkLenPtr = (int *)chunk + chunkIndex * 2 + 1;
                            do
                            {
                                copyLen = *chunkLenPtr - chunkCount;
                                if (copyLen >= fragLeft)
                                    copyLen = fragLeft;
                                src = (u32 *)(chunkLenPtr[-1] + chunkCount);
                                dest = (u32 *)(*(int *)(message + 0x28) + 0x30 + message);
                                memcpy(dest, src, copyLen);
                                *(u32 *)(message + 0x28) = *(int *)(message + 0x28) + copyLen;
                                chunkCount = chunkCount + copyLen;
                                if (chunkCount == *chunkLenPtr)
                                {
                                    chunkCount = 0;
                                    chunkIndex = chunkIndex + 1;
                                    chunkLenPtr = chunkLenPtr + 2;
                                }
                                fragLeft = fragLeft - copyLen;
                            } while (fragLeft != 0);
                        }
                        if ((msgFlags & 0x1000) != 0)
                        {
                            channelSeq = (char *)(connection + 0x750 + channelId);
                            *channelSeq = *channelSeq + '\x01';
                        }
                        EQ_CDECL(void, 0x004F3000, (int, int))(connection, message);
                        remaining = remaining - fragLen;
                        fragIndex = fragIndex + 1;
                    }
                    else
                    {
                        result = 2;
                        EQ_CDECL(void, 0x004EF240, (int))(connection);
                        return *(volatile int *)&result;
                    }
                } while (fragIndex < fragCount);
            }
            if (1 < fragCount)
                *(short *)(connection + 0x4be) = *(short *)(connection + 0x4be) + 1;
        }
        else
        {
            u32 length;
            tmp = 0;
            if (channelId != 0)
                tmp = 0x1000;
            message = EQ_CDECL(int, 0x004F48C0, (u32))(totalSize + 0x34);
            *(u16 *)(message + 0x18) = tmp;
            *(char *)(message + 0x22) = (char)channelId;
            EQ_CDECL(void, 0x004F7EB0, (int))(message);
            if (chunkCount > 0)
            {
                u32 *vector = chunk;
                channelId = chunkCount;
                do
                {
                    length = vector[1];
                    src = (u32 *)*vector;
                    dest = (u32 *)(*(int *)(message + 0x28) + 0x30 + message);
                    memcpy(dest, src, length);
                    vector = vector + 2;
                    *(u32 *)(message + 0x28) = *(int *)(message + 0x28) + length;
                    channelId = channelId + -1;
                } while (channelId != 0);
            }
            EQ_CDECL(void, 0x004F3000, (int, int))(connection, message);
        }
        EQ_CDECL(void, 0x004EF130, (int, int))(connection, 1);
        EQ_CDECL(void, 0x004EF240, (int))(connection);
        return *(volatile int *)&result;
    }
    return 0x12;
}

// FUNCTION: TRILOGY 0x004f39c0
// Fills status: [0]=1 if idle, else [1]=next time the connection needs servicing (send, resend or timer).
void NetworkConnection_UpdateConnectionTimeout_guess(int connection, u32 *status)
{
    u32 sendTime;
    u32 rawInterval;
    int entry;
    int tmp;

    if (*(int *)(connection + 0x5e0) != 0)
        tmp = *(int *)(*(int *)(connection + 0x5e0) + 8);
    else
        tmp = 0;
    if (*(int *)(connection + 0x5c8) != 0)
        entry = *(int *)(*(int *)(connection + 0x5c8) + 8);
    else
        entry = 0;
    if (((tmp == 0) && (entry == 0)) && (*(int *)(connection + 0x794) == 0))
    {
        status[1] = 0;
        *status = 1;
        return;
    }
    *status = 0;
    sendTime = EQ_CDECL(u32, 0x004F7C50, (int))(connection + 0x610);
    status[1] = sendTime;
    if (*(short *)(connection + 0x4a8) == 0x45)
    {
        sendTime = EQ_CDECL(u32, 0x004F17C0, (u32))(*(u32 *)(connection + 0xc));
        status[1] = sendTime;
    }
    if ((tmp != 0) && (*(int *)(connection + 0x768) != 0))
        return;
    tmp = *(int *)(connection + 0x798);
    if (entry != 0)
    {
        /* fake match: the second interval keeps VC6's repeated lower-bound
           comparison, as in the original expanded min/max clamp. */
        u32 interval;
        rawInterval = (u32)*(u16 *)(connection + 0x746) + (u32)*(u16 *)(connection + 0x748) * 2;
        interval = (u32)*(u16 *)(connection + 0x746) + (u32)*(u16 *)(connection + 0x748) * 2;
        entry = *(int *)(entry + 0x14) + (u16)
            ((rawInterval > 50 ? rawInterval : 50) < 65535 ?
             (interval > 50 ? interval : 50) : 65535);
        if ((*(int *)(connection + 0x794) == 0) || (tmp - entry > 0))
            tmp = entry;
    }
    if (tmp - (int)status[1] > 0)
        status[1] = tmp;
    return;
}

// FUNCTION: TRILOGY 0x004f3ac0
// Ends a connection and records its disconnect reason.
void NetworkConnection_Disconnect_guess(RdpConnection_guess *conn, u32 disconnectCode)
{
    RdpList_guess *list;
    int queuedPacket;
    DWORD DVar5;

    list = &conn->tx_hold;
    while (1)
    {
        queuedPacket = (int)rdp_list_remove_head_guess(list);
        if (queuedPacket == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(queuedPacket + 0x28);
        rdp_fast_free_guess((void *)queuedPacket);
    }
    list = &conn->tx_wait;
    while (1)
    {
        queuedPacket = (int)rdp_list_remove_head_guess(list);
        if (queuedPacket == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(queuedPacket + 0x28);
        rdp_fast_free_guess((void *)queuedPacket);
    }
    list = &conn->tx_sent;
    while (1)
    {
        queuedPacket = (int)rdp_list_remove_head_guess(list);
        if (queuedPacket == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)(queuedPacket + 0x28);
        rdp_fast_free_guess((void *)queuedPacket);
    }
    EQ_CDECL(void, 0x004F5940, (RdpConnection_guess *))(conn);
    if (conn->field_768 == 0)
    {
        RDP_STAT_guess(0x188)++;
    }
    conn->connected = 0;
    conn->field_780 = 1;
    conn->disconnect_code = disconnectCode;
    if (disconnectCode == 0x60000)
    {
        RDP_STAT_guess(0x1a8)++;
    }
    else
    {
        if ((conn->field_17C > 0) &&
            (DVar5 = rdp_timeGetTime_guess(), (int)(DVar5 - conn->field_178) < 10000))
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
                        ++*(RdpU64_guess *)((u8 *)g_rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x1c8);
                    else
                        RDP_STAT_guess(0x270)++;
                    break;
                case 4:
                    RDP_STAT_guess(0x248)++;
                    break;
                case 0xb:
                    if (conn->disconnect_reason2 < 2)
                        ++*(RdpU64_guess *)((u8 *)g_rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x250);
                    else
                        RDP_STAT_guess(0x270)++;
                    break;
                case 0xc:
                    if (conn->disconnect_reason2 < 2)
                        ++*(RdpU64_guess *)((u8 *)g_rdp_stats_guess + (u32)conn->disconnect_reason2 * 8 + 0x260);
                    else
                        RDP_STAT_guess(0x270)++;
                    break;
                }
                break;
            case 0x10000:
                RDP_STAT_guess(0x1b0)++;
                break;
            case 0x40000:
                RDP_STAT_guess(0x1c0)++;
                break;
            case 0x50000:
                RDP_STAT_guess(0x1b8)++;
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

// FUNCTION: TRILOGY 0x004f3d40
// Completes a pending close and discards queued outgoing packets.
void NetworkConnection_handle_disconnect_request_guess(int connection)
{
    RdpConnection_guess *conn = (RdpConnection_guess *)connection;
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
        *(u32 *)(list + 1) -= *(u32 *)((char *)item + 0x28);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_wait;
    while (1 == 1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)((char *)item + 0x28);
        rdp_fast_free_guess(item);
    }
    list = &conn->tx_sent;
    while (1 == 1)
    {
        item = rdp_list_remove_head_guess(list);
        if (item == 0)
            break;
        *(u32 *)(list + 1) -= *(u32 *)((char *)item + 0x28);
        rdp_fast_free_guess(item);
    }
    conn->field_780 = 1;
}

// FUNCTION: TRILOGY 0x004f3e10
// ORIGINAL FILE: Network/rdp/tx.c (Mac: tx_set_delayed_ack, high)
// __cdecl. Schedules an acknowledgement 50 ms from now, unless one is already pending.
void tx_set_delayed_ack_guess(RdpConnection_guess *conn)
{
    if (conn->delayed_ack == 0)
    {
        conn->delayed_ack = 1;
        conn->delayed_ack_time = rdp_timeGetTime_guess() + 50;
    }
}

// FUNCTION: TRILOGY 0x004f4280
// Sends game traffic on the connection and piggybacks pending acknowledgements.
int NetworkConnection_SendPacket_guess(RdpConnection_guess *connection, int param_2, int param_3, int headerSize)
{
    u16 networkWord;
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

    local_header.flags = headerSize;
    if (connection->connected == 0)
    {
        local_header.flags |= 0x8000;
    }
    else if (connection->timeout_armed != 0)
    {
        local_header.flags |= 0x400;
    }
    networkWord = EQ_STDCALL(u16, 0x005003B0, (u16))(connection->tx_serial);
    local_header.serial = networkWord;
    ackHeaderSize = EQ_CDECL(int, 0x004F60A0, (RdpConnection_guess *, void *, u16 *))
        (connection, local_header.data, &local_header.flags);
    headerSize = ackHeaderSize + 4;
    connection->delayed_ack = 0;
    if (ackHeaderSize != 0)
    {
        if (param_2 != 0)
        {
            RDP_STAT_guess(0x38)++;
            connection->stat[7]++;
        }
        else
        {
            RDP_STAT_guess(0x30)++;
            connection->stat[6]++;
        }
    }
    local_packet.data = &local_header.flags;
    local_packet.param_2 = param_2;
    local_packet.size = headerSize;
    local_packet.param_3 = param_3;
    networkWord = EQ_STDCALL(u16, 0x005003B0, (u16))(local_header.flags);
    local_header.flags = networkWord;
    if (connection->remote.family == 0x45)
    {
        sendResult = EQ_CDECL(int, 0x004F6E20, (void *, void *, int, RdpAddr_guess *))
            (&connection->rdp->serial, &local_packet, 2, &connection->remote);
    }
    else
    {
        sendResult = EQ_CDECL(int, 0x004F7760, (u32, void *, int, RdpAddr_guess *, u32, u32))
            (connection->field_4A4, &local_packet, 2, &connection->remote,
             *(u32 *)((u8 *)connection->rdp + 0x110), *(u32 *)((u8 *)connection->rdp + 0x114));
        rdp_timeGetTime_guess();
    }
    RDP_STAT_guess(0x338)++;
    if (sendResult != 0)
    {
        if (RDP_STAT_guess(0x350) != 0)
            RDP_STAT_guess(0x348)++;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x350) = 1;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x354) = 0;
        RDP_STAT_guess(0x340)++;
    }
    else
    {
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x350) = 0;
        *(u32 *)((u8 *)g_rdp_stats_guess + 0x354) = 0;
    }
    switch (sendResult)
    {
        case 1:
            EQ_CDECL(void, 0x004F3AC0, (RdpConnection_guess *, u32))(connection, 0x70000);
            break;
        case 5:
            EQ_CDECL(void, 0x004F7B50, (RdpBandwidth_guess *, u32))
                (&connection->bandwidth, connection->bandwidth.rate / 10);
            break;
        case 0:
            EQ_CDECL(void, 0x004F7B50, (RdpBandwidth_guess *, u32))
                (&connection->bandwidth, headerSize + 0x1c + param_3);
            connection->tx_serial++;
            break;
    }
    return sendResult;
}

// FUNCTION: TRILOGY 0x004f7930
// __cdecl. Clears the estimator and seeds it with one sample of weight 1; first is capped at 0xFFFF.
void InitializeRttStats_guess(RdpRttStats_guess *rtt, u32 first)
{
    struct RdpRttSample_guess *temp2;
    u32 sq;
    {
        u32 v;
    
        memset(rtt, 0, sizeof(*rtt));
        if (!(0xffff <= first))
            {
        }
        else
            first = 0xffff;
        temp2 = rtt->samples;
        rtt->samples[0].value = (u16)first;
        ((struct RdpRttSample_guess *)(temp2))[0].weight = 1;
        rtt->weight_sum = 1;
        v = 0xffff & first;
        rtt->mean = (u16)first;
        sq = v;
        rtt->sum = v;
        sq *= v;
    }
    rtt->sumsq_hi = 0;
    rtt->sumsq_lo = sq;
    rtt->deviation = 0;
}

// FUNCTION: TRILOGY 0x004f79a0
// Tracks a weighted average and deviation for recent packet timing samples.
// @early-stop: only four sample-word loads for commutative products use the opposite registers.
/* fake match: view adjacent sumsq words as one 64-bit counter for VC6 carry chains. */
void update_network_statistics_guess(RdpRttStats_guess *rtt, u32 elapsed, u32 interval)
{
    RdpRttSample_guess *sample;
    u32 index;
    u32 contribution;
    u32 square_contribution;
    int weight;
    union
    {
        unsigned __int64 unsigned_value;
        __int64 signed_value;
    } variance;

    index = rtt->next;
    sample = &rtt->samples[index];
    contribution = (u32)sample->weight * sample->value;
    rtt->sum -= contribution;
    square_contribution = sample->value * contribution;
    *(unsigned __int64 *)&rtt->sumsq_lo -= square_contribution;
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
    contribution = (u32)sample->weight * sample->value;
    rtt->sum += contribution;
    square_contribution = sample->value * contribution;
    *(unsigned __int64 *)&rtt->sumsq_lo += square_contribution;
    rtt->weight_sum = (u16)(rtt->weight_sum + sample->weight);
    rtt->mean = (u16)(rtt->sum / rtt->weight_sum);

    if (1 < rtt->weight_sum)
    {
        variance.unsigned_value = *(unsigned __int64 *)&rtt->sumsq_lo;
        variance.unsigned_value -= (u32)rtt->mean * rtt->sum;
        rtt->deviation = (u16)(long)sqrt((double)variance.signed_value / (double)((int)rtt->weight_sum - 1));
        rtt->next = (index + 1) & 0x3f;
        return;
    }
    rtt->deviation = 0;
    rtt->next = (index + 1) & 0x3f;
    return;
}
