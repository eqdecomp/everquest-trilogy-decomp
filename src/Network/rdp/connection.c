/* connection.c - one rdp connection: state, timeouts, accessors.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/connection.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))

/* 0x004F26D0 tx_create_guess, 0x004F5820 rx_create_guess: both __cdecl, one connection argument. */
#define tx_create_guess  EQ_CDECL(int, 0x004F26D0, (RdpConnection_guess *))
#define rx_create_guess  EQ_CDECL(int, 0x004F5820, (RdpConnection_guess *))

/* list.c / fast_malloc.c helpers, all __cdecl. */
#define rdp_list_add_tail_guess     EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))
#define rdp_list_remove_head_guess  EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))
#define rdp_fast_free_guess         EQ_CDECL(void, 0x004F4AC0, (void *))
#define get_min_packet_time_guess   EQ_CDECL(int, 0x004F7E50, (RdpList_guess *))

#define connhash_lock_perf_guess             EQ_CDECL(void, 0x004F51C0, (void *, void *))
#define tx_get_stall_time_perf_guess EQ_CDECL(unsigned int, 0x004F46C0, (void *))
#define rdp_unlock_perf_guess                EQ_CDECL(void, 0x004EF240, (void *))
#define rdp_get_serial_stall_time_perf_guess EQ_CDECL(unsigned int, 0x004F17E0, (void *))

/* connection_init calls these two cdecl initializers directly. */
#define tx_init_guess EQ_CDECL(void, 0x004F24D0, (RdpConnection_guess *, RdpState_guess *, RdpAddr_guess *))
#define rx_init_guess EQ_CDECL(void, 0x004F5700, (RdpConnection_guess *))

typedef void (WINAPI *InitializeCriticalSectionProc_guess)(LPCRITICAL_SECTION);
#define initializeCriticalSection_import_guess EQ_GLOBAL(InitializeCriticalSectionProc_guess, 0x005340E0)

// FUNCTION: TRILOGY 0x004f1800
// Initializes a peer connection's send and receive sessions.
// ORIGINAL FILE: Network/rdp/connection.c (from Mac debug map)
void connection_init_guess(RdpConnection_guess *conn, RdpState_guess *rdp, RdpAddr_guess *remote, u32 field_064)
{
    conn->rdp = rdp;
    tx_init_guess(conn, rdp, remote);
    rx_init_guess(conn);
    conn->timeout_armed = 0;
    conn->timeout_time = 0;
    conn->field_05C = 0;
    conn->field_060 = 0;
    *(u32 *)&conn->unknown000[0] = 0;
    *(u32 *)&conn->unknown000[4] = 0;
    *(u32 *)&conn->unknown000[8] = 0;
    initializeCriticalSection_import_guess((LPCRITICAL_SECTION)&conn->lock);
    conn->hash_link.key = &conn->remote;
    conn->field_034 = 0;
    conn->state = 0;
    conn->hash_link.data = conn;
    conn->self_028 = conn;
    conn->ptr_02C = &conn->field_030;
    conn->field_064 = field_064;
    conn->field_030 = 1;
}

// FUNCTION: TRILOGY 0x004f1870
// __cdecl. Creates the tx side; the rx side only when tx_create returned 0.
int initializeNetworkConnectionResources_guess(RdpConnection_guess *conn)
{
    int result;

    result = tx_create_guess(conn);
    if (result == 0)
        result = rx_create_guess(conn);
    return result;
}

// FUNCTION: TRILOGY 0x004f1890
// __cdecl. Frees the buffer list, the queued items, then deletes the connection's critical section.
void NetworkConnection_CleanupNetworkConnectionResources_guess(RdpConnection_guess *connection)
{
    EQ_CDECL(void, 0x004F2730, (RdpConnection_guess *))(connection);
    EQ_CDECL(void, 0x004F5870, (RdpConnection_guess *))(connection);
    DeleteCriticalSection((CRITICAL_SECTION *)connection->lock);
}

// FUNCTION: TRILOGY 0x004f18b0
// Chooses the next service deadline for packet handling, probes, and connection timers.
void NetworkConnection_getConnectionStatus_guess(RdpConnection_guess *connection, int *status)
{
    int deadline;
    u32 rto;

    // status[0]: 1 = no deadline; status[1]: earliest deadline (ms)
    *status = 1;
    status[1] = 0;
    connection->state = 0;
    if (connection->connected != 0 && connection->field_780 == 0)
    {
        EQ_CDECL(void, 0x004F39C0, (RdpConnection_guess *, int *))(connection, status);
        if (*status == 0)
        {
            connection->state = 2;
        }
    }
    else if (connection->connected != 0 && connection->field_780 != 0 && connection->delayed_ack != 0)
    {
        connection->state = 2;
        *status = 0;
        status[1] = connection->delayed_ack_time;
    }
    // field_064 bit 0 gates state 3
    if ((*(u8 *)&connection->field_064 & 1) != 0 && connection->field_764 != 0 &&
        connection->field_770 == 0 && connection->connected != 0 && connection->field_780 == 0)
    {
        // 10000 ms (10 s) after the last packet number was issued
        deadline = connection->tx_time + 10000;
        if ((*status != 0) || (deadline - status[1] < 0))
        {
            connection->state = 3;
            *status = 0;
            status[1] = deadline;
        }
    }
    // field_064 bit 1 gates state 4
    if ((*(u8 *)&connection->field_064 & 2) != 0 && connection->field_7DC < 3)
    {
        if (connection->field_7C4 != 0)
        {
            // Copy of rto: needed to match the original's codegen.
            u32 interval;
            rto = (u32)connection->rtt.mean + (u32)connection->rtt.deviation * 2;
            interval = (u32)connection->rtt.mean + (u32)connection->rtt.deviation * 2;
            // rto clamped to 50..0xFFFF ms
            deadline = connection->field_7C8 + (u16)
                ((rto > 50 ? rto : 50) < 65535 ?
                 (interval > 50 ? interval : 50) : 65535);
        }
        else
        {
            deadline = connection->field_7C8;
        }
        if ((*status != 0) || (deadline - status[1] < 0))
        {
            connection->state = 4;
            *status = 0;
            status[1] = deadline;
        }
    }
    if ((connection->timeout_armed != 0) &&
        ((*status != 0) || ((int)(connection->timeout_time - status[1]) < 0)))
    {
        connection->state = 1;
        *status = 0;
        status[1] = connection->timeout_time;
    }
    return;
}

// FUNCTION: TRILOGY 0x004F1A40
// Validates a peer packet's control fields and length before receive processing.
// @early-stop: 23 differences remain in logging/counter registers and fragment-check load scheduling.
// ORIGINAL FILE: Network/rdp/connection.c (from Mac debug map)
int connection_parse_and_validate_arrival
    (int connection, u16 *packet, u16 packetLen, u16 *packetInfo)
{
    u8 stream_byte;
    u16 word_value;
    int result;
    u16 *cursor;
    int ack_bytes;
    char sender[64];

    cursor = packet;
    word_value = EQ_STDCALL(u16, 0x00500380, (u16))(*cursor++);
    *packetInfo = word_value;
    if (!(((((short)(word_value & 0x2000) != 0) || ((short)(word_value & 0x4000) != 0)) &&
         ((short)(word_value & 0x200) == 0)) ||
        ((((short)(word_value & 0x800) != 0) && ((short)(word_value & 0x200) == 0)) ||
         ((short)(word_value & 0x100) != 0))))
    {
        word_value = EQ_STDCALL(u16, 0x00500380, (u16))(*cursor++);
        packetInfo[1] = word_value;
        result = EQ_CDECL(int, 0x004F5BA0, (int, u16))(connection, word_value);
        if (result == 0)
        {
            *(u16 **)(packetInfo + 2) = cursor;
            result = EQ_CDECL(int, 0x004F2990, (int, u16 *, int *))
                (connection, packetInfo, &ack_bytes);
            if (result == 0)
            do
            {
                cursor = (u16 *)((char *)cursor + ack_bytes);
                if ((short)(*packetInfo & 0x200) != 0)
                {
                    packetInfo[4] = *cursor++;
                    packetInfo[4] = EQ_STDCALL(u16, 0x00500380, (u16))(packetInfo[4]);
                    result = EQ_CDECL(int, 0x004F5C70, (int, u16 *))
                        (connection, packetInfo);
                    if (result != 0)
                        break;
                }
                packetInfo[7] = 1;
                packetInfo[5] = 0;
                packetInfo[6] = 0;
                if ((short)(*packetInfo & 0x800) != 0)
                {
                    packetInfo[5] = *cursor++;
                    packetInfo[5] = EQ_STDCALL(u16, 0x00500380, (u16))(packetInfo[5]);
                    packetInfo[6] = *cursor++;
                    packetInfo[6] = EQ_STDCALL(u16, 0x00500380, (u16))(packetInfo[6]);
                    packetInfo[7] = *cursor++;
                    packetInfo[7] = EQ_STDCALL(u16, 0x00500380, (u16))(packetInfo[7]);
                }
                if ((short)(*packetInfo & 0x1000) != 0)
                {
                    *(char *)(packetInfo + 8) = *(char *)cursor;
                    cursor = (u16 *)((char *)cursor + 1);
                    if ((short)(*packetInfo & 0x200) != 0)
                    {
                        stream_byte = *(u8 *)cursor;
                        cursor = (u16 *)((char *)cursor + 1);
                        *((u8 *)packetInfo + 0x11) = stream_byte;
                    }
                    result = EQ_CDECL(int, 0x004F5EF0, (int, u16 *))
                        (connection, packetInfo);
                    if (result != 0)
                        break;
                }
                packetInfo[9] = (u16)((char *)cursor - (char *)packet);
                if (packetLen < packetInfo[9])
                {
                    result = 2;
                    EQ_CDECL(void, 0x004F1230, (char *, RdpAddr_guess *))
                        (sender, (RdpAddr_guess *)(connection + 0x4a8));
                    EQ_CDECL(void, 0x004F66F0, (const char *, char *, u32, u32))
                        ((const char *)0x0057E884, sender, packetLen, packetInfo[9]);
                    ++g_rdp_stats_guess->c[33];
                    *(int *)(connection + 0xe4) = *(int *)(connection + 0xe4) + 1;
                }
                else
                {
                    packetInfo[10] = packetLen - packetInfo[9];
                    if ((short)(*packetInfo & 0x800) != 0)
                    {
                        result = EQ_CDECL(int, 0x004F5D70, (int, u16 *))
                            (connection, packetInfo);
                    }
                }
            } while (0);
        }
    }
    else
    {
        result = 2;
        EQ_CDECL(void, 0x004F1230, (char *, RdpAddr_guess *))
            (sender, (RdpAddr_guess *)(connection + 0x4a8));
        ++g_rdp_stats_guess->c[27];
        *(int *)(connection + 0xcc) = *(int *)(connection + 0xcc) + 1;
    }
    if (result == 2)
    {
        if (*(int *)(connection + 0x77c) != 0)
            EQ_CDECL(void, 0x004F3AC0, (int, int))(connection, 0x60000);
    }
    return result;
}

// FUNCTION: TRILOGY 0x004f1ce0
// Records an accepted packet's sequence, peer acknowledgments, and traffic totals.
// @early-stop: VC6 sign-extends the disconnect mask to 0xffff8000; original uses 0x8000 before the same 16-bit test.
void connection_record_arrival
    (int connection, u16 *packetInfo, int *processFlag)
{
    u16 payloadBytes;
    int sequenceResult;

    *processFlag = 0;
    EQ_CDECL(void, 0x004F59C0, (int))(connection);
    if (((short)(*packetInfo & 0x8000) != 0) && (*(int *)(connection + 0x77c) != 0))
    {
        EQ_CDECL(void, 0x004F3AC0, (int, int))(connection, 0x10000);
        return;
    }
    if (((short)(*packetInfo & 0x400) != 0) && (*(int *)(connection + 0x780) == 0))
    {
        EQ_CDECL(void, 0x004F3D40, (int))(connection);
    }
    EQ_CDECL(void, 0x004F59D0, (int, u16 *))(connection, packetInfo);
    EQ_CDECL(void, 0x004F2CD0, (int, u16 *))(connection, packetInfo);
    if ((short)(*packetInfo & 0x200) != 0)
    {
        sequenceResult = EQ_CDECL(int, 0x004F5F70, (int, u16))(connection, packetInfo[4]);
        *processFlag = sequenceResult;
        if (sequenceResult != 0)
        {
            g_rdp_stats_guess->c[12] += 1;
            payloadBytes = packetInfo[10];
            g_rdp_stats_guess->c[13] += payloadBytes;
            *(int *)(connection + 0x98) = *(int *)(connection + 0x98) + 1;
            *(u32 *)(connection + 0x9c) = *(u32 *)(connection + 0x9c) + (u32)packetInfo[10];
        }
        else
        {
            g_rdp_stats_guess->c[10] += 1;
            payloadBytes = packetInfo[10];
            g_rdp_stats_guess->c[11] += payloadBytes;
            *(int *)(connection + 0x90) = *(int *)(connection + 0x90) + 1;
            *(u32 *)(connection + 0x94) = *(u32 *)(connection + 0x94) + (u32)packetInfo[10];
        }
    }
    else if (packetInfo[10] > 0)
    {
        g_rdp_stats_guess->c[8] += 1;
        payloadBytes = packetInfo[10];
        g_rdp_stats_guess->c[9] += payloadBytes;
        *(int *)(connection + 0x88) = *(int *)(connection + 0x88) + 1;
        *(u32 *)(connection + 0x8c) = *(u32 *)(connection + 0x8c) + (u32)packetInfo[10];
    }
    payloadBytes = packetInfo[9];
    g_rdp_stats_guess->c[14] += payloadBytes;
    *(u32 *)(connection + 0xa0) = *(u32 *)(connection + 0xa0) + (u32)packetInfo[9];
    if ((short)(*packetInfo & 0x200) != 0)
    {
        EQ_CDECL(void, 0x004F3E10, (int))(connection);
    }
    return;
}

// FUNCTION: TRILOGY 0x004f1ef0
// Tracks packet type counts and recent probe details for a connection.
void NetworkConnection_pprocess_network_packet_stats_guess
               (int connection, char packetType, u8 packetCode, char trackPacket,
                u32 tmpByte, int fromAddr)
{
    int addressValue;
    DWORD DVar3;
    u32 codeIndex;
    u32 *recordEntry;
    u32 sampleValue;

    DVar3 = rdp_timeGetTime_guess();
    if (trackPacket != '\0')
    {
        if ((tmpByte & 0xff) >= *(u32 *)(connection + 0x7e0))
            return;
        if (*(int *)(connection + 0x7c0) == 0)
            return;
        recordEntry = (u32 *)((tmpByte & 0xff) * 0x10 +
                         *(int *)(connection + 0x7c0));
        if (((packetType == '\x03') && (packetCode == 3)) &&
            (sampleValue = (u32)*(u8 *)(recordEntry + 3),
             *(u32 *)(connection + 0x7d8) > sampleValue) &&
            (*(u32 *)(connection + 0x7d8) = sampleValue,
             *(u32 *)(connection + 0x7d4) > sampleValue))
        {
            *(u32 *)(connection + 0x7d4) = 1;
            *(int *)(connection + 0x7dc) = *(int *)(connection + 0x7dc) + 1;
        }
        recordEntry[1] = DVar3 - *recordEntry;
        addressValue = *(int *)(fromAddr + 4);
        *(char *)((int)recordEntry + 0xd) = packetType;
        recordEntry[2] = addressValue;
        *(u8 *)((int)recordEntry + 0xe) = packetCode;
        *(DWORD *)(connection + 0x7c8) = DVar3;
        *(u32 *)(connection + 0x7c4) = 0;
        return;
    }
    switch ((u8)packetType)
    {
    case '\x03':
        if (packetCode < 0x10)
        {
            codeIndex = (u32)packetCode;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x0057F698) + codeIndex * 8 + 0x288);
            recordEntry = (u32 *)(connection + 0x104 + codeIndex * 4);
            *recordEntry = *recordEntry + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[102];
            *(int *)(connection + 0x158) = *(int *)(connection + 0x158) + 1;
        }
        break;
    case '\x04':
        ++g_rdp_stats_guess->c[97];
        *(int *)(connection + 0x144) = *(int *)(connection + 0x144) + 1;
        break;
    default:
        return;
    case '\v':
        if (packetCode < 2)
        {
            codeIndex = (u32)packetCode;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x0057F698) + codeIndex * 8 + 0x310);
            recordEntry = (u32 *)(connection + 0x148 + codeIndex * 4);
            *recordEntry = *recordEntry + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[102];
            *(int *)(connection + 0x158) = *(int *)(connection + 0x158) + 1;
        }
        break;
    case '\f':
        if (packetCode < 2)
        {
            codeIndex = (u32)packetCode;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x0057F698) + codeIndex * 8 + 0x320);
            recordEntry = (u32 *)(connection + 0x150 + codeIndex * 4);
            *recordEntry = *recordEntry + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[102];
            *(int *)(connection + 0x158) = *(int *)(connection + 0x158) + 1;
        }
    }
    *(char *)(connection + 0x174) = packetType;
    *(u8 *)(connection + 0x175) = packetCode;
    *(int *)(connection + 0x17c) = *(int *)(connection + 0x17c) + 1;
    DVar3 = rdp_timeGetTime_guess();
    *(DWORD *)(connection + 0x178) = DVar3;
    *(u32 *)(connection + 0x180) = *(u32 *)(fromAddr + 4);
    if ((packetType == '\x03') && (packetCode == 3))
        EQ_CDECL(void, 0x004F3AC0, (int, int))(connection, 0x30000);
    return;
}

// FUNCTION: TRILOGY 0x004f2160
// Runs a connection's due packet handling, control acknowledgment, and probe work.
void NetworkConnection_process_connection_state_guess(int connObj, int currentTime, int *status)
{
    while (1)
    {
        switch (*(int *)(connObj + 0x38))
        {
        case 1:
            return;
        case 2:
            EQ_CDECL(void, 0x004F3E40, (int))(connObj);
            break;
        case 3:
            EQ_CDECL(void, 0x004F3330, (int))(connObj);
            break;
        case 4:
            EQ_CDECL(void, 0x004F33E0, (int))(connObj);
        }
        EQ_CDECL(void, 0x004F18B0, (int, int *))(connObj, status);
        if (*status != 0)
            break;
        if (currentTime - status[1] < 0)
            return;
    }
    return;
}

typedef void (WINAPI *ConnectionCloseCriticalSectionProc_guess)(LPCRITICAL_SECTION);

// FUNCTION: TRILOGY 0x004f21d0
// Ends a peer session and can give its reliable close message time to complete.
// ORIGINAL FILE: Network/rdp/connection.c (from Mac debug map)
// @early-stop: the original caches LeaveCriticalSection's IAT load in EBP for its second call (resultPending in EDI, here EBP)
int connection_close(int connection, int timeoutMs, u32 *resultOut, int doneEvent)
{
    int resultPending;
    int deferredEvent;
    int connObj;
    DWORD DVar4;
    int rdpState;
    u32 *deferredResult;

    connObj = connection;
    if (connection == 0)
        return 6;
    EQ_CDECL(void, 0x004F51C0, (void *, void *))(
        (void *)(*(int *)(connection + 0xc) + 0x50), (void *)(connection + 0x4a8));
    rdpState = *(int *)(connection + 0xc);
    EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x00534194)(
        (LPCRITICAL_SECTION)(rdpState + 0xb8));
    if (*(int *)(rdpState + 0xa4) != 0)
        connection = *(int *)(*(int *)(rdpState + 0xa4) + 8);
    else
        connection = 0;
    EQ_CDECL(void, 0x004F78E0, (int *, int))((int *)(rdpState + 0xa4), connObj);
    if ((connection != 0) &&
        (*(int *)(rdpState + 0xa4) == 0 || *(int *)(*(int *)(rdpState + 0xa4) + 8) == 0))
        EQ_CDECL(int, 0x004F53F0, (void *, int))((void *)(rdpState + 0xa0), 0);
    EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x00534190)(
        (LPCRITICAL_SECTION)(rdpState + 0xb8));
    EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x00534194)(
        (LPCRITICAL_SECTION)(rdpState + 0xe4));
    EQ_CDECL(void, 0x004F78E0, (int *, int))((int *)(rdpState + 0xd0), connObj);
    EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x00534190)(
        (LPCRITICAL_SECTION)(rdpState + 0xe4));
    resultPending = 1;
    if (((*(int *)(connObj + 0x77c) == 0) || (*(int *)(connObj + 0x780) == 0)) &&
        ((*(int *)(connObj + 0x774) == 0) ||
         (*(short *)(connObj + 0x778) != *(short *)(connObj + 0x4b8))))
    {
        if (*(int *)(connObj + 0x77c) == 0)
        {
            if (resultOut != (u32 *)0)
                *resultOut = 0;
            resultPending = 0;
        }
    }
    else
    {
        if (resultOut != (u32 *)0)
            *resultOut = 1;
        resultPending = 0;
    }
    if (timeoutMs == 0)
    {
        if (resultPending)
        {
            if (resultOut != (u32 *)0)
                *resultOut = 0;
            resultPending = 0;
        }
        EQ_CDECL(void, 0x004F1580, (int, int))(rdpState, connObj);
    }
    else
    {
        deferredEvent = 0;
        deferredResult = (u32 *)0;
        if (resultPending)
        {
            deferredResult = resultOut;
            deferredEvent = doneEvent;
        }
        EQ_CDECL(void, 0x004F5940, (int))(connObj);
        *(u32 *)(connObj + 0x54) = 1;
        DVar4 = EQ_CDECL(DWORD, 0x004F66D0, (void))();
        *(int *)(connObj + 0x7a0) = deferredEvent;
        *(DWORD *)(connObj + 0x58) = DVar4 + timeoutMs;
        *(u32 **)(connObj + 0x7a4) = deferredResult;
        EQ_CDECL(void, 0x004F3260, (int))(connObj);
        EQ_CDECL(void, 0x004EF130, (int, int))(connObj, 1);
    }
    if ((!resultPending) && (doneEvent != 0))
        EQ_CDECL(void, 0x004F78B0, (int))(doneEvent);
    EQ_CDECL(void, 0x004EF240, (int))(connObj);
    return 0;
}

// FUNCTION: TRILOGY 0x004f2370
// ORIGINAL FILE: Network/rdp/connection.c (address: between Mac-mapped neighbours)
// __cdecl. 1 when the timeout is armed and its deadline is at most 10 ms away (or past; signed difference).
int NetworkConnection_isConnectionTimedOut_guess(RdpConnection_guess *conn)
{
    int remaining = (int)(conn->timeout_time - rdp_timeGetTime_guess());

    if (conn->timeout_armed && remaining <= 10)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004f23a0
// ORIGINAL FILE: Network/rdp/connection.c (address: between Mac-mapped neighbours)
// __cdecl. Sets the dword at 0x620 (meaning unknown; unnamed in the queue: connection_set_field_620_guess).
void connection_set_field_620_guess(RdpConnection_guess *conn, u32 value)
{
    conn->field_620 = value;
}

// FUNCTION: TRILOGY 0x004f23b0
// ORIGINAL FILE: Network/rdp/connection.c (Mac: connection_connected, high)
// __cdecl.
int connection_connected_guess(RdpConnection_guess *connection)
{
    return connection->connected;
}

// FUNCTION: TRILOGY 0x004f23c0
// ORIGINAL FILE: Network/rdp/connection.c (Mac: connection_app_ptr, high)
// __cdecl. The application's handle for a connection is the connection itself.
void *connection_app_ptr_guess(RdpConnection_guess *conn)
{
    return conn;
}

// FUNCTION: TRILOGY 0x004f23d0
// ORIGINAL FILE: Network/rdp/connection.c (Mac: connection_get_remote_addr, high)
// __cdecl.
RdpAddr_guess *connection_get_remote_addr_guess(RdpConnection_guess *conn)
{
    return &conn->remote;
}

// FUNCTION: TRILOGY 0x004F23E0
// Collects connection traffic and delay metrics for status and performance displays.
// ORIGINAL FILE: Network/rdp/connection.c (from Mac debug map)
void connection_get_perf_stats(RdpConnection_guess *conn, unsigned int *stats)
{
    unsigned char *conn_bytes;
    unsigned char *stats_bytes;
    unsigned int value;

    conn_bytes = (unsigned char *)conn;
    stats_bytes = (unsigned char *)stats;
    connhash_lock_perf_guess((void *)(*(unsigned int *)(conn_bytes + 0xc) + 0x50),
                             (void *)(conn_bytes + 0x4a8));
    *(unsigned int *)(stats_bytes + 0) = *(unsigned int *)(conn_bytes + 0x294);
    *(unsigned int *)(stats_bytes + 8) = *(unsigned int *)(conn_bytes + 0x490);
    *(unsigned int *)(stats_bytes + 0xc) = *(unsigned int *)(conn_bytes + 0x494);
    *(unsigned short *)(stats_bytes + 0x10) = *(unsigned short *)(conn_bytes + 0x48c);
    *(unsigned int *)(stats_bytes + 0x14) = *(unsigned short *)(conn_bytes + 0x746);
    *(unsigned int *)(stats_bytes + 0x18) = *(unsigned short *)(conn_bytes + 0x748);
    *(unsigned int *)(stats_bytes + 0x1c) = *(unsigned short *)(conn_bytes + 0x79c);
    *(unsigned int *)(stats_bytes + 0x20) = *(int *)(conn_bytes + 0x60c) +
                                             *(int *)(conn_bytes + 0x5f4) +
                                             *(int *)(conn_bytes + 0x5dc);
    value = tx_get_stall_time_perf_guess(conn);
    *(unsigned int *)(stats_bytes + 0x24) = value;
    rdp_unlock_perf_guess(conn);
    if (*(short *)(conn_bytes + 0x4a8) == 0x45)
    {
        value = rdp_get_serial_stall_time_perf_guess((void *)(*(unsigned int *)(conn_bytes + 0xc)));
        *(unsigned int *)(stats_bytes + 0x24) = value;
    }
    return;
}

// FUNCTION: TRILOGY 0x004f2490
// ORIGINAL FILE: Network/rdp/connection.c (Mac: connection_get_disconnect_info, high)
// __cdecl. Fills `disconnectInfo` only when the caller's size is 12 (bytes 6..7 are left alone).
void connection_get_disconnect_info(RdpConnection_guess *conn, RdpDisconnectInfo_guess *disconnectInfo, int size)
{
    if (size == sizeof(RdpDisconnectInfo_guess))
    {
        disconnectInfo->code = conn->disconnect_code;
        disconnectInfo->reason2 = conn->disconnect_reason2;
        disconnectInfo->reason = conn->disconnect_reason;
        disconnectInfo->value = conn->disconnect_value;
    }
}

// FUNCTION: TRILOGY 0x004f46c0
// Measures how overdue a connection's oldest unacknowledged packet has become.
// @early-stop: list head and &conn->tx_sent swap ECX/EDX (VC6 loads the head into EDX; the original loads it into ECX).
// __cdecl. How far the oldest unacknowledged packet is past the retransmit timeout (rtt mean + 2 * deviation,
// clamped to 50..0xFFFF ms); 0 when nothing is waiting or it is not late yet.
/* fake match: interval repeats rto so VC6 keeps the repeated lower-bound compare of MIN(MAX(rto, 50), 0xFFFF). */
u32 tx_get_stall_time_guess(RdpConnection_guess *conn)
{
    u32 age;
    u32 rto;
    u32 interval;
    u16 timeout;

    age = 0;
    if (conn->tx_sent.head && conn->tx_sent.head->data)
    {
        age = rdp_timeGetTime_guess() - ((u32)(get_min_packet_time_guess(&conn->tx_sent)));
    }
    rto = conn->rtt.mean + conn->rtt.deviation * 2;
    interval = conn->rtt.mean + conn->rtt.deviation * 2;
    timeout = (u16)((rto > 50 ? rto : 50) < 0xFFFF ? (interval > 50 ? interval : 50) : 0xFFFF);
    if (age > timeout)
        return age - timeout;
    return 0;
}

// FUNCTION: TRILOGY 0x004f78e0
// Removes queued incoming messages associated with a connection being closed.
// __cdecl. Rotates the list once: every message whose sender is `sender` is freed, the others go back to the
// tail in order. Returns the number freed.
int RemoveMessagesFromConnection_guess(RdpList_guess *list, void *connObj)
{
    RdpMsgArrival_guess *msg;
    int removed = 0;
    int n = list->count;

    while (n--)
    {
        msg = (RdpMsgArrival_guess *)rdp_list_remove_head_guess(list);
        if (msg->sender == connObj)
        {
            removed++;
            rdp_fast_free_guess(msg);
        }
        else
            rdp_list_add_tail_guess(list, &msg->link);
    }
    return removed;
}
