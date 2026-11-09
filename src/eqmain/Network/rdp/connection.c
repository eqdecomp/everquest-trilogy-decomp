// eqmain: the same original object as trilogy's src/Network/rdp/connection.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* connection.c - one rdp connection: state, timeouts, accessors.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/connection.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x1002F9F0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x1002F9F0, (void))

/* 0x1002EA60 tx_create_guess, 0x1002EA60 rx_create_guess: both __cdecl, one connection argument. */
#define tx_create_guess  EQ_CDECL(int, 0x1002EA60, (RdpConnection_guess *))
#define rx_create_guess  EQ_CDECL(int, 0x1002EA60, (RdpConnection_guess *))

/* list.c / fast_malloc.c helpers, all __cdecl. */
#define rdp_list_add_tail_guess     EQ_CDECL(void, 0x1002DF20, (RdpList_guess *, RdpLink_guess *))
#define rdp_list_remove_head_guess  EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))
#define rdp_fast_free_guess         EQ_CDECL(void, 0x10028A90, (void *))
#define get_min_packet_time_guess   EQ_CDECL(int, 0x004F7E50, (RdpList_guess *))

#define connhash_lock_perf_guess             EQ_CDECL(void, 0x1002E6E0, (void *, void *))
#define tx_get_stall_time_perf_guess EQ_CDECL(unsigned int, 0x004F46C0, (void *))
#define rdp_unlock_perf_guess                EQ_CDECL(void, 0x10028BF0, (void *))
#define rdp_get_serial_stall_time_perf_guess EQ_CDECL(unsigned int, 0x004F17E0, (void *))

/* connection_init calls these two cdecl initializers directly. */
#define tx_init_guess EQ_CDECL(void, 0x1002B000, (RdpConnection_guess *, RdpState_guess *, RdpAddr_guess *))
#define rx_init_guess EQ_CDECL(void, 0x1002E930, (RdpConnection_guess *))

typedef void (WINAPI *InitializeCriticalSectionProc_guess)(LPCRITICAL_SECTION);
#define initializeCriticalSection_import_guess EQ_GLOBAL(InitializeCriticalSectionProc_guess, 0x100400A8)

typedef void (WINAPI *ConnectionCloseCriticalSectionProc_guess)(LPCRITICAL_SECTION);

// FUNCTION: EQMAIN 0x1000f3b0
// Adds 12 to the first dword of its argument; its network role is unclear.
// Name from trilogy 004F23D0 connection_get_remote_addr_guess (medium sibling); the code is not that function's:
// it loads the first dword at its argument and adds 0xc.
// __fastcall: the one pointer argument is in ECX, the callee pops nothing. Returns the value in EAX.
int __fastcall connection_get_remote_addr_guess(int *ptr)
{
    return *ptr + 0xc;
}

// FUNCTION: EQMAIN 0x1002d180
// Prepares a peer connection to exchange reliable packets.
// Name from trilogy connection_init_guess 0x004F1800 (BinDiff sibling, same code shape; offsets are this image's).
// __cdecl, no return value (no caller reads EAX). Sets up a new connection: its rdp, the tx and rx sides, the
// timeout and state fields, the critical section, and the pointers to itself and to field_030.
void connection_init_guess(RdpConnection_guess *conn, RdpState_guess *rdp, RdpAddr_guess *remoteAddress, u32 value)
{
    conn->rdp = rdp;
    tx_init_guess(conn, rdp, remoteAddress);
    rx_init_guess(conn);
    conn->timeout_armed = 0;
    conn->timeout_time = 0;
    conn->field_05C = 0;
    conn->field_060 = 0;
    *(u32 *)&conn->unknown000[0] = 0;
    *(u32 *)&conn->unknown000[4] = 0;
    *(u32 *)&conn->unknown000[8] = 0;
    initializeCriticalSection_import_guess((LPCRITICAL_SECTION)conn->lock);
    conn->field_034 = 0;
    conn->ptr_02C = &conn->field_030;
    conn->state = 0;
    conn->field_030 = 1;
    conn->self_028 = conn;
    conn->field_064 = value;
}

// FUNCTION: EQMAIN 0x1002d1f0
// Requests creation of a connection's send and receive resources.
// Cloned from trilogy initializeNetworkConnectionResources_guess 0x004F1870 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Creates the tx side; the rx side only when tx_create returned 0.
int initializeNetworkConnectionResources_guess(RdpConnection_guess *conn)
{
    int result;

    result = tx_create_guess(conn);
    if (result == 0)
        result = rx_create_guess(conn);
    return result;
}

// FUNCTION: EQMAIN 0x1002d210
// Releases the connection's packet resources and critical section.
// Cloned from trilogy NetworkConnection_CleanupNetworkConnectionResources_guess 0x004F1890 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Frees the buffer list, the queued items, then deletes the connection's critical section.
void NetworkConnection_CleanupNetworkConnectionResources_guess(RdpConnection_guess *conn)
{
    EQ_CDECL(void, 0x1002B210, (RdpConnection_guess *))(conn);
    EQ_CDECL(void, 0x1002EA70, (RdpConnection_guess *))(conn);
    DeleteCriticalSection((CRITICAL_SECTION *)conn->lock);
}

// FUNCTION: EQMAIN 0x1002d230
// Selects the next connection event and deadline for the network loop.
// Cloned from trilogy NetworkConnection_getConnectionStatus_guess 0x004F18B0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Sets *status to 1 (nothing due) or 0 with the due time in status[1], and the connection state at +0x30.
// Field offsets are this image's: RdpConnection_guess (include/eqmain/rdp.h) has them.
void NetworkConnection_getConnectionStatus_guess(RdpConnection_guess *conn, int *status)
{
    int deadline;
    u32 rttInterval;

    *status = 1;
    status[1] = 0;
    conn->state = 0;
    if (conn->connected != 0 && conn->field_780 == 0)
    {
        EQ_CDECL(void, 0x1002C4A0, (RdpConnection_guess *, int *))(conn, status);
        if (*status == 0)
        {
            conn->state = 2;
        }
    }
    else if (conn->connected != 0 && conn->field_780 != 0 && conn->delayed_ack != 0)
    {
        conn->state = 2;
        *status = 0;
        status[1] = conn->delayed_ack_time;
    }
    if ((*(u8 *)&conn->field_064 & 1) != 0 && conn->field_764 != 0 &&
        conn->field_770 == 0 && conn->connected != 0 && conn->field_780 == 0)
    {
        deadline = conn->tx_time + 10000;
        if ((*status != 0) || (deadline - status[1] < 0))
        {
            conn->state = 3;
            *status = 0;
            status[1] = deadline;
        }
    }
    if ((*(u8 *)&conn->field_064 & 2) != 0 && conn->field_7DC < 3)
    {
        if (conn->field_7C4 != 0)
        {
            /* fake match: a second interval local preserves VC6's repeated clamp
               comparison and ECX allocation; the arithmetic is combined by VC6. */
            u32 interval;
            rttInterval = (u32)conn->rtt.mean + (u32)conn->rtt.deviation * 2;
            interval = (u32)conn->rtt.mean + (u32)conn->rtt.deviation * 2;
            deadline = conn->field_7C8 + (u16)
                ((rttInterval > 50 ? rttInterval : 50) < 65535 ?
                 (interval > 50 ? interval : 50) : 65535);
        }
        else
        {
            deadline = conn->field_7C8;
        }
        if ((*status != 0) || (deadline - status[1] < 0))
        {
            conn->state = 4;
            *status = 0;
            status[1] = deadline;
        }
    }
    if ((conn->timeout_armed != 0) &&
        ((*status != 0) || ((int)(conn->timeout_time - status[1]) < 0)))
    {
        conn->state = 1;
        *status = 0;
        status[1] = conn->timeout_time;
    }
}

// FUNCTION: EQMAIN 0x1002d3c0
// Checks an arriving RDP packet's header and payload bounds before processing.
// Cloned from trilogy connection_parse_and_validate_arrival_guess 0x004F1A40 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Returns 0 for an accepted packet, else the failing check's result: 2 = rejected (counted; short packets are
// logged too). A result of 2 also disconnects when +0x6d4 is set. Field offsets and constants this image changed are
// substituted in the code; comments keep trilogy's.
// @early-stop: headerLen (original loads packet into EDX first) and the register picks in the two log/counter blocks still differ.
int connection_parse_and_validate_arrival_guess
    (int connection, u16 *packet, u16 packetLen, u16 *packetInfo)
{
    u8 extraByte;
    u8 *statCounter;
    u16 flags;
    u16 headerLen;
    int result;
    u16 *cursor;
    int ackLen;
    char addrText[64];

    cursor = packet;
    flags = EQ_STDCALL(u16, 0x100322BA, (u16))(*cursor++);
    *packetInfo = flags;
    do
    {
        if (((((short)(flags & 0x2000) != 0) || ((short)(flags & 0x4000) != 0)) &&
             ((short)(flags & 0x200) == 0)) ||
            ((((short)(flags & 0x800) != 0) && ((short)(flags & 0x200) == 0)) ||
             ((short)(flags & 0x100) != 0)))
        {
            result = 2;
            EQ_CDECL(void, 0x1002AA20, (char *, RdpAddr_guess *))
                (addrText, (RdpAddr_guess *)(connection + 0x478));
            statCounter = EQ_GLOBAL(u8 *, 0x100498B4) + 0xd8;
            *(unsigned __int64 *)statCounter += 1;
            *(int *)(connection + 0xc4) = *(int *)(connection + 0xc4) + 1;
            break;
        }
        flags = EQ_STDCALL(u16, 0x100322BA, (u16))(*cursor++);
        packetInfo[1] = flags;
        result = EQ_CDECL(int, 0x1002EDA0, (int, u16))(connection, flags);
        if (result == 0)
        {
            *(u16 **)(packetInfo + 2) = cursor;
            result = EQ_CDECL(int, 0x1002B470, (int, u16 *, int *))
                (connection, packetInfo, &ackLen);
            if (result == 0)
            {
                cursor = (u16 *)((char *)cursor + ackLen);
                if ((short)(*packetInfo & 0x200) != 0)
                {
                    packetInfo[4] = *cursor++;
                    packetInfo[4] = EQ_STDCALL(u16, 0x100322BA, (u16))(packetInfo[4]);
                    result = EQ_CDECL(int, 0x1002EE70, (int, u16 *))
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
                    packetInfo[5] = EQ_STDCALL(u16, 0x100322BA, (u16))(packetInfo[5]);
                    packetInfo[6] = *cursor++;
                    packetInfo[6] = EQ_STDCALL(u16, 0x100322BA, (u16))(packetInfo[6]);
                    packetInfo[7] = *cursor++;
                    packetInfo[7] = EQ_STDCALL(u16, 0x100322BA, (u16))(packetInfo[7]);
                }
                if ((short)(*packetInfo & 0x1000) != 0)
                {
                    *(char *)(packetInfo + 8) = *(char *)cursor;
                    cursor = (u16 *)((char *)cursor + 1);
                    if ((short)(*packetInfo & 0x200) != 0)
                    {
                        extraByte = *(u8 *)cursor;
                        cursor = (u16 *)((char *)cursor + 1);
                        *((u8 *)packetInfo + 0x11) = extraByte;
                    }
                    result = EQ_CDECL(int, 0x1002F0F0, (int, u16 *))
                        (connection, packetInfo);
                    if (result != 0)
                        break;
                }
                headerLen = (u16)((char *)cursor - (char *)packet);
                packetInfo[9] = headerLen;
                if (packetLen < headerLen)
                {
                    result = 2;
                    EQ_CDECL(void, 0x1002AA20, (char *, RdpAddr_guess *))
                        (addrText, (RdpAddr_guess *)(connection + 0x478));
                    EQ_CDECL(void, 0x1002FA10, (const char *, char *, int, int))
                        ((const char *)0x10048B9C, addrText, packetLen, packetInfo[9]);
                    statCounter = EQ_GLOBAL(u8 *, 0x100498B4) + 0x108;
                    *(unsigned __int64 *)statCounter += 1;
                    *(int *)(connection + 0xdc) = *(int *)(connection + 0xdc) + 1;
                }
                else
                {
                    packetInfo[10] = packetLen - headerLen;
                    if ((short)(*packetInfo & 0x800) != 0)
                    {
                        result = EQ_CDECL(int, 0x1002EF70, (int, u16 *))
                            (connection, packetInfo);
                    }
                }
            }
        }
    } while (0);
    if (result == 2 && *(int *)(connection + 0x6d4) != 0)
        EQ_CDECL(void, 0x1002C5B0, (int, int))(connection, 0x60000);
    return result;
}

// FUNCTION: EQMAIN 0x1002d660
// Applies packet fields to connection state and updates traffic counts.
// Ported from trilogy NetworkConnection_process_network_packet_data_guess 0x004F1CE0 (BinDiff sibling): the same code, with
// this image's connection offsets, stats global (0x100498B4) and callees.
void NetworkConnection_process_network_packet_data_guess
    (int connectionObject, u16 *packet, int *sequenceStatus)
{
    u16 packetLength;
    int sequenceAccepted;

    *sequenceStatus = 0;
    EQ_CDECL(void, 0x1002EBC0, (int))(connectionObject);
    if (((short)(*packet & 0x8000) != 0) && (*(int *)(connectionObject + 0x6d4) != 0))
    {
        EQ_CDECL(void, 0x1002C5B0, (int, int))(connectionObject, 0x10000);
        return;
    }
    if (((short)(*packet & 0x400) != 0) && (*(int *)(connectionObject + 0x6d8) == 0))
    {
        EQ_CDECL(void, 0x1002C830, (int))(connectionObject);
    }
    EQ_CDECL(void, 0x1002EBD0, (int, u16 *))(connectionObject, packet);
    EQ_CDECL(void, 0x1002B7B0, (int, u16 *))(connectionObject, packet);
    if ((short)(*packet & 0x200) != 0)
    {
        sequenceAccepted = EQ_CDECL(int, 0x1002F170, (int, u16))(connectionObject, packet[4]);
        *sequenceStatus = sequenceAccepted;
        if (sequenceAccepted != 0)
        {
            g_rdp_stats_guess->c[12] += 1;
            packetLength = packet[10];
            g_rdp_stats_guess->c[13] += packetLength;
            *(int *)(connectionObject + 0x90) = *(int *)(connectionObject + 0x90) + 1;
            *(u32 *)(connectionObject + 0x94) = *(u32 *)(connectionObject + 0x94) + (u32)packet[10];
        }
        else
        {
            g_rdp_stats_guess->c[10] += 1;
            packetLength = packet[10];
            g_rdp_stats_guess->c[11] += packetLength;
            *(int *)(connectionObject + 0x88) = *(int *)(connectionObject + 0x88) + 1;
            *(u32 *)(connectionObject + 0x8c) = *(u32 *)(connectionObject + 0x8c) + (u32)packet[10];
        }
    }
    else if (packet[10] > 0)
    {
        g_rdp_stats_guess->c[8] += 1;
        packetLength = packet[10];
        g_rdp_stats_guess->c[9] += packetLength;
        *(int *)(connectionObject + 0x80) = *(int *)(connectionObject + 0x80) + 1;
        *(u32 *)(connectionObject + 0x84) = *(u32 *)(connectionObject + 0x84) + (u32)packet[10];
    }
    packetLength = packet[9];
    g_rdp_stats_guess->c[14] += packetLength;
    *(u32 *)(connectionObject + 0x98) = *(u32 *)(connectionObject + 0x98) + (u32)packet[9];
    if ((short)(*packet & 0x200) != 0)
    {
        EQ_CDECL(void, 0x1002C900, (int))(connectionObject);
    }
    return;
}

// FUNCTION: EQMAIN 0x1002d870
// Tracks per-connection packet counts and timing, including a disconnect control subtype.
// Cloned from trilogy NetworkConnection_pprocess_network_packet_stats_guess 0x004F1EF0 (the same original code: BinDiff sibling, names/siblings/).
// Field offsets and constants this image changed are substituted in the code (clone.offset_map); comments keep trilogy's.
void NetworkConnection_pprocess_network_packet_stats_guess
               (RdpConnection_guess *connectionObject, char packetType, u8 packetSubtype, char recordingEnabled,
                u32 recordIndex, int obj)
{
    int sourceValue;
    DWORD DVar3;
    u32 subtypeIndex;
    u32 *ptr;
    u32 recordByte;

    DVar3 = rdp_timeGetTime_guess();
    if (recordingEnabled != '\0')
    {
        if ((recordIndex & 0xff) >= connectionObject->ttl_index)
            return;
        if (connectionObject->unk_0x718 == 0)
            return;
        ptr = (u32 *)((recordIndex & 0xff) * 0x10 +
                         connectionObject->unk_0x718);
        if (((packetType == '\x03') && (packetSubtype == 3)) &&
            (recordByte = (u32)*(u8 *)(ptr + 3),
             connectionObject->ttl_max > recordByte) &&
            (connectionObject->ttl_max = recordByte,
             connectionObject->ttl_value > recordByte))
        {
            connectionObject->ttl_value = 1;
            connectionObject->field_7DC = connectionObject->field_7DC + 1;
        }
        ptr[1] = DVar3 - *ptr;
        sourceValue = *(int *)(obj + 4);
        *(char *)((int)ptr + 0xd) = packetType;
        ptr[2] = sourceValue;
        *(u8 *)((int)ptr + 0xe) = packetSubtype;
        connectionObject->field_7C8 = DVar3;
        connectionObject->field_7C4 = 0;
        return;
    }
    switch ((u8)packetType)
    {
    case '\x03':
        if (packetSubtype < 0x10)
        {
            subtypeIndex = (u32)packetSubtype;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x100498B4) + subtypeIndex * 8 + 0x290);
            ptr = &connectionObject->stat[39 + subtypeIndex];
            *ptr = *ptr + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[103];
            connectionObject->stat[60] = connectionObject->stat[60] + 1;
        }
        break;
    case '\x04':
        ++g_rdp_stats_guess->c[98];
        connectionObject->stat[55] = connectionObject->stat[55] + 1;
        break;
    default:
        return;
    case '\v':
        if (packetSubtype < 2)
        {
            subtypeIndex = (u32)packetSubtype;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x100498B4) + subtypeIndex * 8 + 0x318);
            ptr = &connectionObject->stat[56 + subtypeIndex];
            *ptr = *ptr + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[103];
            connectionObject->stat[60] = connectionObject->stat[60] + 1;
        }
        break;
    case '\f':
        if (packetSubtype < 2)
        {
            subtypeIndex = (u32)packetSubtype;
            ++*(unsigned __int64 *)(EQ_GLOBAL(u8 *, 0x100498B4) + subtypeIndex * 8 + 0x328);
            ptr = &connectionObject->stat[58 + subtypeIndex];
            *ptr = *ptr + 1;
        }
        else
        {
            ++g_rdp_stats_guess->c[103];
            connectionObject->stat[60] = connectionObject->stat[60] + 1;
        }
    }
    connectionObject->disconnect_reason = packetType;
    connectionObject->disconnect_reason2 = packetSubtype;
    connectionObject->field_17C = connectionObject->field_17C + 1;
    DVar3 = rdp_timeGetTime_guess();
    connectionObject->field_178 = DVar3;
    connectionObject->disconnect_value = *(u32 *)(obj + 4);
    if ((packetType == '\x03') && (packetSubtype == 3))
        EQ_CDECL(void, 0x1002C5B0, (int, int))((int)connectionObject, 0x30000);
    return;
}

// FUNCTION: EQMAIN 0x1002dae0
// __cdecl, no return value. The state at +0x30 picks the handler: 1 returns, 2/3/4 run one handler, any other
// value runs none. Then the status query: a non-zero statusOut[0] stops, as does currentTime - statusOut[1] < 0; else loop.
/* Callees of 0x1002dae0 (this image's addresses, __cdecl). */
#define NetworkConnection_ProcessNetworkPackets_guess         EQ_CDECL(void, 0x1002C930, (int))
#define NetworkConnection_SendConnectionAcknowledgement_guess EQ_CDECL(void, 0x1002BE10, (int))
#define NetworkConnection_SendHeartbeat_guess                 EQ_CDECL(void, 0x1002BEC0, (int))
#define NetworkConnection_getConnectionStatus_guess           EQ_CDECL(void, 0x1002D230, (int, int *))

void NetworkConnection_process_connection_state_guess(int connection, int currentTime, int *statusOut)
{
    while (1)
    {
        switch (*(int *)(connection + 0x30))
        {
        case 1:
            return;
        case 2:
            NetworkConnection_ProcessNetworkPackets_guess(connection);
            break;
        case 3:
            NetworkConnection_SendConnectionAcknowledgement_guess(connection);
            break;
        case 4:
            NetworkConnection_SendHeartbeat_guess(connection);
        }
        NetworkConnection_getConnectionStatus_guess(connection, statusOut);
        if (*statusOut != 0)
            break;
        if (currentTime - statusOut[1] < 0)
            return;
    }
    return;
}

/* Callees of 0x1002db50 that have no macro above (this image's addresses, __cdecl unless noted). The two
 * critical-section slots are the KERNEL32 IAT entries the original calls through (__stdcall). */
#define enterCriticalSection_import_guess EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x1004014C)
#define leaveCriticalSection_import_guess EQ_GLOBAL(ConnectionCloseCriticalSectionProc_guess, 0x10040148)
#define rdp_queue_count_guess              EQ_CDECL(int, 0x1002E120, (int *))
#define rdp_queue_update_guess             EQ_CDECL(int, 0x10030FF0, (int *, int))
#define wait_for_semaphore_with_timeout_guess  EQ_CDECL(int, 0x1002E8E0, (u32 *, DWORD))
#define close_connection_and_cleanup_guess EQ_CDECL(void, 0x1002AE10, (int, int))
#define cleanup_connection_queues_guess    EQ_CDECL(void, 0x1002EB40, (int))
#define NetworkConnection_send_disconnect_message_guess EQ_CDECL(int, 0x1002BD40, (int))
#define rdp_resort_guess                   EQ_CDECL(void, 0x10028AE0, (int, int))
#define uevent_signal_guess                EQ_CDECL(void, 0x10030A00, (u32 *))

// FUNCTION: EQMAIN 0x1002db50
// Initiates peer shutdown immediately or schedules a delayed close.
// @early-stop: the final low-byte status test and deferred cdecl stack cleanup still differ.
// __cdecl. Returns 6 for a null connection, else 0. delayMs == 0 closes the connection now. Otherwise it
// stores now + delayMs at +0x50, sends the disconnect message and resorts the connection.
int NetworkConnection_disconnect_guess(int connection, int delayMs, u32 *statusOut, u32 *doneEvent)
{
    int rdpState;
    int queueCount;
    u32 *savedStatusOut;
    u32 *savedEvent;
    DWORD DVar4;
    /* fake match: read the low byte so VC6 keeps the 0/1 status flag in EBX. */
    union
    {
        int dword;
        unsigned char lowByte;
    } status;

    if (connection == 0)
        return 6;
    connhash_lock_perf_guess((void *)(*(int *)(connection + 0xc) + 0x50), (void *)(connection + 0x478));
    rdpState = *(int *)(connection + 0xc);
    enterCriticalSection_import_guess((LPCRITICAL_SECTION)(rdpState + 0xbc));
    queueCount = rdp_queue_count_guess((int *)(rdpState + 0xa4));
    queueCount = queueCount != 0;
    rdp_queue_update_guess((int *)(rdpState + 0xa4), connection);
    if (queueCount)
    {
        queueCount = rdp_queue_count_guess((int *)(rdpState + 0xa4));
        if (queueCount == 0)
            wait_for_semaphore_with_timeout_guess((u32 *)(rdpState + 0xa0), 0);
    }
    {
        ConnectionCloseCriticalSectionProc_guess leaveCriticalSection = leaveCriticalSection_import_guess;
        leaveCriticalSection((LPCRITICAL_SECTION)(rdpState + 0xbc));
        enterCriticalSection_import_guess((LPCRITICAL_SECTION)(rdpState + 0xec));
        rdp_queue_update_guess((int *)(rdpState + 0xd4), connection);
        leaveCriticalSection((LPCRITICAL_SECTION)(rdpState + 0xec));
        enterCriticalSection_import_guess((LPCRITICAL_SECTION)(rdpState + 0x120));
        *(int *)(rdpState + 0x138) = *(int *)(rdpState + 0x138) - 1;
        leaveCriticalSection((LPCRITICAL_SECTION)(rdpState + 0x120));
    }
    status.dword = 1;
    if (((*(int *)(connection + 0x6d4) == 0) || (*(int *)(connection + 0x6d8) == 0)) &&
        ((*(int *)(connection + 0x6cc) == 0 ||
          (*(short *)(connection + 0x6d0) != *(short *)(connection + 0x488)))))
    {
        /* 6d4 != 0 keeps status at 1 and writes nothing (the original jumps past the reset). */
        if (*(int *)(connection + 0x6d4) == 0)
        {
            if (statusOut != 0)
                *statusOut = 0;
            status.dword = 0;
        }
    }
    else
    {
        if (statusOut != 0)
            *statusOut = 1;
        status.dword = 0;
    }
    if (delayMs == 0)
    {
        if (status.dword)
        {
            if (statusOut != 0)
                *statusOut = 0;
            status.dword = 0;
        }
        close_connection_and_cleanup_guess(rdpState, connection);
    }
    else
    {
        savedEvent = 0;
        savedStatusOut = 0;
        if (status.dword)
        {
            savedEvent = doneEvent;
            savedStatusOut = statusOut;
        }
        cleanup_connection_queues_guess(connection);
        *(u32 *)(connection + 0x4c) = 1;
        DVar4 = rdp_timeGetTime_guess();
        *(u32 **)(connection + 0x6f8) = savedEvent;
        *(DWORD *)(connection + 0x50) = DVar4 + delayMs;
        *(u32 **)(connection + 0x6fc) = savedStatusOut;
        NetworkConnection_send_disconnect_message_guess(connection);
        rdp_resort_guess(connection, 1);
    }
    if (!status.lowByte && doneEvent != 0)
        uevent_signal_guess(doneEvent);
    rdp_unlock_perf_guess((void *)connection);
    return 0;
}

// FUNCTION: EQMAIN 0x1002dd00
// Checks whether the armed connection timeout is due.
// Cloned from trilogy NetworkConnection_isConnectionTimedOut_guess 0x004F2370 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. 1 when the timeout is armed and its deadline is at most 10 ms away (or past; signed difference).
int NetworkConnection_isConnectionTimedOut_guess(RdpConnection_guess *conn)
{
    int remaining = (int)(conn->timeout_time - rdp_timeGetTime_guess());

    if (conn->timeout_armed && remaining <= 10)
        return 1;
    return 0;
}
