/* rdp.c - the rdp instance: receive/send entry points, its connections, its serial link.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/rdp.c (from Mac debug map)
 *
 * Also the small OS helpers the queue labels rdp but whose object the Mac map doesn't name (each says where
 * it sits by address).
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <string.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x005003FE: the client's import thunk for WSOCK32 ioctlsocket (jmp [IAT]); called directly. */
#define rdp_ioctlsocket_guess  EQ_STDCALL(int, 0x005003FE, (u32 s, long cmd, u32 *argp))
/* 0x004F21D0: connection_close (not rewritten yet). */
#define connection_close EQ_CDECL(void, 0x004F21D0, (RdpState_guess *rdp, RdpConnection_guess *conn, u32 reason, HANDLE *event))
/* 0x00500380 / 0x00500386: the import thunks for WSOCK32 ntohs and inet_ntoa. */
#define rdp_ntohs_guess        EQ_STDCALL(u16, 0x00500380, (u16 n))
#define rdp_inet_ntoa_guess    EQ_STDCALL(char *, 0x00500386, (u32 addr))
/* 0x004F66D0: the client's import thunk for WINMM timeGetTime (jmp [IAT]). */
#define rdp_timeGetTime_guess  EQ_STDCALL(DWORD, 0x004F66D0, (void))
/* Callees of initialize_network_connection_state_guess, by address (list.c, connhash.c, uevent.c). */
#define rdp_init_network_buffer_guess EQ_CDECL(void, 0x004F67C0, (void *obj))
#define rdp_init_counter_guess        EQ_CDECL(void, 0x004F5000, (void *p))
#define rdp_uevent_init_guess         EQ_CDECL(void, 0x004F7880, (void **ev))
#define rdp_list_init_guess           EQ_CDECL(void, 0x004F5440, (RdpList_guess *list))

typedef void (__stdcall *rdp_critical_section_proc_guess)(LPCRITICAL_SECTION);
#define rdp_enter_critical_section_guess EQ_GLOBAL(rdp_critical_section_proc_guess, 0x00534194)
#define rdp_leave_critical_section_guess EQ_GLOBAL(rdp_critical_section_proc_guess, 0x00534190)

/* Address thunks for helpers implemented in other rdp translation units. */
#define removeConnectionFromHashTable_guess(hash, conn) \
    EQ_CDECL(RdpConnection_guess *, 0x004F5330, (RdpConnHash_guess *, RdpConnection_guess *))(hash, conn)
#define uevent_init_guess(ev) \
    EQ_CDECL(void, 0x004F7880, (HANDLE *))(ev)
#define uevent_create(ev) \
    EQ_CDECL(int, 0x004F7890, (HANDLE *))(ev)
#define uevent_wait_guess(ev) \
    EQ_CDECL(void, 0x004F78C0, (HANDLE *))(ev)
#define uevent_destroy_guess(ev) \
    EQ_CDECL(void, 0x004F7250, (void *))(ev)
#define is_connection_inactive_guess(q) \
    EQ_CDECL(int, 0x004F70F0, (RdpSerialQueue_guess *))(q)
#define calculateAdjustedClientTime_guess(q) \
    EQ_CDECL(DWORD, 0x004F7120, (RdpSerialQueue_guess *))(q)
#define GetSerialQueueStallTime_guess(q) \
    EQ_CDECL(DWORD, 0x004F7180, (RdpSerialQueue_guess *))(q)

// FUNCTION: TRILOGY 0x004ef130
// Reschedules a connection's timeout and wakes the network worker when it becomes next to expire.
/* fake match: the status scope preserves the original load/store scheduling. */
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
void rdp_resort(int connectionBase, int sendWakeup)
{
    struct AddressCopy { int words[4]; } address;
    int connection = connectionBase;
    int rdp = *(int *)(connection + 0xc);
    {
        struct StatusCopy { int low; int high; } status;
        LPCRITICAL_SECTION lock;
        struct StatusCopy *saved_status;

        EQ_CDECL(void, 0x004F18B0, (int, int *))(connection, (int *)&status);
        lock = (LPCRITICAL_SECTION)(rdp + 0x6c);
        rdp_enter_critical_section_guess(lock);
        saved_status = (struct StatusCopy *)(connection + 0x30);
        if (memcmp(saved_status, &status, sizeof(status)) != 0)
        {
            *saved_status = status;
            EQ_CDECL(void, 0x004F4F30, (int, int *))(rdp + 0x58, (int *)(connection + 0x24));
            if (connection == EQ_CDECL(int, 0x004F4FE0, (int *))((int *)(rdp + 0x58)) &&
                *(int *)(rdp + 0x84) == 0 && sendWakeup != 0)
            {
                SOCKET local_socket = *(SOCKET *)(rdp + 4);
                connectionBase = 0;
                if (local_socket != (SOCKET)-1)
                {
                    address = *(struct AddressCopy *)(rdp + 0x14);
                    address.words[1] = EQ_STDCALL(u32, 0x00500398, (u32))(0x7f000001);
                    EQ_STDCALL(int, 0x00500392, (SOCKET, char *, int, int, struct sockaddr *, int))(
                        *(SOCKET *)(rdp + 4), (char *)&connectionBase, 4, 0, (struct sockaddr *)&address, 0x10);
                }
                else
                {
                    SOCKET s = *(SOCKET *)(rdp + 0x10);
                    if (s != (SOCKET)-1)
                        EQ_STDCALL(int, 0x00500392, (SOCKET, char *, int, int, struct sockaddr *, int))(
                            s, (char *)&connectionBase, 4, 0, (struct sockaddr *)(rdp + 0x34), 0xe);
                }
                *(int *)(rdp + 0x84) = 1;
            }
        }
        rdp_leave_critical_section_guess(lock);
    }
}

// FUNCTION: TRILOGY 0x004ef240
// Releases the connection from tracking, then frees its resources if it was detached.
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
void rdp_unlock_guess(void *connection)
{
    int rdp;
    int removed;
    rdp_critical_section_proc_guess leaveCriticalSection;

    leaveCriticalSection = rdp_leave_critical_section_guess;
    rdp = (int)((RdpConnection_guess *)connection)->rdp;
    leaveCriticalSection((LPCRITICAL_SECTION)(&((RdpConnection_guess *)connection)->lock));
    removed = EQ_CDECL(int, 0x004F5330, (void *, void *))(&((RdpState_guess *)rdp)->hash, connection);
    if (removed != 0)
    {
        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(&((RdpState_guess *)rdp)->timeout_lock));
        // 0x24: the connection's heap element
        EQ_CDECL(void, 0x004F4DC0, (void *, void *))(&((RdpState_guess *)rdp)->timeout_heap,
                                                      (void *)(removed + 0x24));
        leaveCriticalSection((LPCRITICAL_SECTION)(&((RdpState_guess *)rdp)->timeout_lock));
        EQ_CDECL(void, 0x004F1890, (void *))((void *)removed);
        EQ_CDECL(void, 0x005230DD, (void *))((void *)removed);
    }
}

// FUNCTION: TRILOGY 0x004ef2a0
// Orders arrived messages per channel and queues those ready for processing.
void process_network_message_guess(int rdpInstance, int connection, int work)
{
    short flags = *(short *)(work + 0x14);
    int message;
    int queue_count;

    if ((short)(flags & 0x4000) != 0)
    {
        EQ_CDECL(void, 0x004F6350, (int, int))(connection, work);
    }
    else if ((short)(flags & 0x1000) != 0)
    {
        if ((short)(flags & 0x200) != 0)
        {
            message = work;
            /* Dequeue preserves the argument's upper bytes and replaces its sequence byte. */
            *(u8 *)&work = *(u8 *)(message + 0x1e);
            EQ_CDECL(void, 0x004F61B0, (int, int))(connection, message);
            while ((message = EQ_CDECL(int, 0x004F61E0, (int, int))(connection, work)) != 0)
            {
                if (*(int *)(message + 0x10) != 0 &&
                    *(int *)(*(int *)(message + 0x10) + 0x54) != 0)
                {
                    EQ_CDECL(void, 0x004F4AC0, (void *))((void *)message);
                }
                else
                {
                    EQ_CDECL(void, 0x004F4B60, (void *))((void *)message);
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                    if (*(int *)(rdpInstance + 0xa4) != 0)
                    {
                        queue_count = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
                    }
                    else
                    {
                        queue_count = 0;
                    }
                    *(int *)(message + 0x28) = 0;
                    EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)(rdpInstance + 0xa4), (void *)message);
                    if (queue_count == 0)
                    {
                        EQ_CDECL(void, 0x004F5420, (void *))((void *)(rdpInstance + 0xa0));
                    }
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                }
            }
        }
        else
        {
            if (EQ_CDECL(int, 0x004F6240, (int, int))(connection, work) != 0)
            {
                if (*(int *)(work + 0x10) != 0 &&
                    *(int *)(*(int *)(work + 0x10) + 0x54) != 0)
                {
                    EQ_CDECL(void, 0x004F4AC0, (void *))((void *)work);
                }
                else
                {
                    EQ_CDECL(void, 0x004F4B60, (void *))((void *)work);
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                    if (*(int *)(rdpInstance + 0xa4) != 0)
                    {
                        queue_count = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
                    }
                    else
                    {
                        queue_count = 0;
                    }
                    *(int *)(work + 0x28) = 0;
                    EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)(rdpInstance + 0xa4), (void *)work);
                    if (queue_count == 0)
                    {
                        EQ_CDECL(void, 0x004F5420, (void *))((void *)(rdpInstance + 0xa0));
                    }
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                }
            }
            else
            {
                EQ_CDECL(void, 0x004F4AC0, (void *))((void *)work);
            }
        }
    }
    else
    {
        if (*(int *)(work + 0x10) != 0 &&
            *(int *)(*(int *)(work + 0x10) + 0x54) != 0)
        {
            EQ_CDECL(void, 0x004F4AC0, (void *))((void *)work);
        }
        else
        {
            EQ_CDECL(void, 0x004F4B60, (void *))((void *)work);
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
            if (*(int *)(rdpInstance + 0xa4) != 0)
            {
                queue_count = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
            }
            else
            {
                queue_count = 0;
            }
            *(int *)(work + 0x28) = 0;
            EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)(rdpInstance + 0xa4), (void *)work);
            if (queue_count == 0)
            {
                EQ_CDECL(void, 0x004F5420, (void *))((void *)(rdpInstance + 0xa0));
            }
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
        }
    }
    if (*(u16 *)(connection + 0x484) != 0 &&
        *(u16 *)(connection + 0x184) == *(u16 *)(connection + 0x486) &&
        *(int *)(connection + 0x488) != 0)
    {
        message = EQ_CDECL(int, 0x004F6330, (int))(connection);
        if (*(int *)(message + 0x10) != 0 &&
            *(int *)(*(int *)(message + 0x10) + 0x54) != 0)
        {
            EQ_CDECL(void, 0x004F4AC0, (void *))((void *)message);
            return;
        }
        EQ_CDECL(void, 0x004F4B60, (void *))((void *)message);
        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
        if (*(int *)(rdpInstance + 0xa4) != 0)
        {
            queue_count = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
        }
        else
        {
            queue_count = 0;
        }
        *(int *)(message + 0x28) = 0;
        EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)(rdpInstance + 0xa4), (void *)message);
        if (queue_count == 0)
        {
            EQ_CDECL(void, 0x004F5420, (void *))((void *)(rdpInstance + 0xa0));
        }
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
    }
}

// FUNCTION: TRILOGY 0x004ef550
// Handles one received packet: decrypt, CRC check, then feed it to its connection or queue it as connectionless
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
u32 rdp_handle_data_recv(int rdpInstance, u16 *packet, u32 packetLen, u8 *fromAddr)
{
    u8 lastByte;
    short addrFamily;
    u16 hostWord;
    u32 packetCrc;
    u32 tmp;
    u32 computedCrc;
    char *ipText;
    u32 *arrival;
    u32 *puVar10;
    u32 *statCounter;
    int work;
    int aux;
    LPCRITICAL_SECTION p_Var14;
    u8 *fromByte;
    u16 *srcCursor;
    int *oldStatusCursor;
    u8 *refByte;
    u32 acc;
    int *newStatusCursor;
    int bVar21;
    int isEqual;
    u32 connection;
    LPCRITICAL_SECTION tmpSlot;
    int processFlag;
    u32 result;
    int parseResult;
    int status[4];
    u32 packetInfo[4];
    u32 payloadField;
    u8 addrText[64];
    u8 ipxText[16];
    u8 *rdpStats;

    result = 0;
    if ((int)packetLen < 2)
    {
        return 0;
    }
    if (((*(short *)fromAddr != 2) || (*(short *)(fromAddr + 2) != *(short *)(rdpInstance + 0x16))) ||
        (*(u32 *)(fromAddr + 4) != EQ_STDCALL(u32, 0x00500398, (u32))(0x7f000001)))
    {
        work = 0x10;
        bVar21 = 0;
        aux = 0;
        isEqual = 1;
        fromByte = fromAddr;
        refByte = (u8 *)(rdpInstance + 0x34);
        do
        {
            if (work == 0)
                break;
            work = work + -1;
            bVar21 = *fromByte < *refByte;
            isEqual = *fromByte == *refByte;
            fromByte = fromByte + 1;
            refByte = refByte + 1;
        } while (isEqual);
        if (!isEqual)
        {
            aux = (1 - (u32)bVar21) - (u32)(bVar21 != 0);
        }
        if (aux != 0)
        {
            acc = 0;
            connection = 0;
            processFlag = 1;
            *(u32 *)(rdpInstance + 0xfc) = *(int *)(rdpInstance + 0xfc) + packetLen + 0x1c;
            if (*(int *)(rdpInstance + 0x110) != 0)
            {
                if (((packetLen & 7) == 0) && (packetLen != 0))
                {
                    EQ_CDECL(void, 0x004F6680, (u16 *, int))(
                        packet, (int)(packetLen + ((int)packetLen >> 0x1f & 7U)) >> 3);
                    lastByte = *(u8 *)((int)packet + (packetLen - 1));
                    tmpSlot = (LPCRITICAL_SECTION)((u32)lastByte & 0xffffff0f);
                    if ((lastByte & 0xf) < 9)
                    {
                        packetLen = packetLen - (lastByte & 0xf);
                    }
                    else
                    {
                        packetLen = 0;
                    }
                }
                else
                {
                    addrFamily = *(short *)fromAddr;
                    if (addrFamily == 2)
                    {
                        hostWord = EQ_STDCALL(u16, 0x00500380, (u16))(*(u16 *)(fromAddr + 2));
                        tmp = (u32)hostWord;
                        ipText = EQ_STDCALL(char *, 0x00500386, (u32))(*(u32 *)(fromAddr + 4));
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_INET %s:%u", ipText, tmp);
                    }
                    else if (addrFamily == 6)
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_IPX %s:%s:%u", ipxText, status,
                            *(u16 *)(fromAddr + 0xc));
                    }
                    else if (addrFamily == 0x45)
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_COMPORT %u", (int)*(short *)(fromAddr + 2));
                    }
                    else
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "unknown address family (%u)", (u16)addrFamily);
                    }
                    EQ_CDECL(void, 0x004F66F0, (const char *, ...))(
                        "%s invalid size (%u)\n", addrText, packetLen);
                    rdpStats = (u8 *)EQ_GLOBAL(void *, 0x0057F698);
                    tmp = *(u32 *)(rdpStats + 0x140);
                    *(u32 *)(rdpStats + 0x140) = tmp + 1;
                    *(u32 *)(rdpStats + 0x144) = *(int *)(rdpStats + 0x144) + (u32)(0xfffffffe < tmp);
                }
            }
            if (((*(int *)(rdpInstance + 0x114) != 0) || (*(int *)(rdpInstance + 0x110) != 0)) &&
                ((5 < (int)packetLen) &&
                 ((hostWord = EQ_STDCALL(u16, 0x00500380, (u16))(*packet), hostWord != 0xffff))))
            {
                if (5 < packetLen)
                {
                    acc = packetLen - 4;
                    packetCrc = EQ_STDCALL(u32, 0x005003AA, (u32))(
                        *(u32 *)((int)packet + (packetLen - 4)));
                    computedCrc = EQ_CDECL(u32, 0x004F6380, (u32, u16 *, u32))(0, packet, acc);
                    if (packetCrc != computedCrc)
                    {
                        acc = 0;
                    }
                }
                packetLen = acc;
                if (acc == 0)
                {
                    addrFamily = *(short *)fromAddr;
                    if (addrFamily == 2)
                    {
                        hostWord = EQ_STDCALL(u16, 0x00500380, (u16))(*(u16 *)(fromAddr + 2));
                        acc = (u32)hostWord;
                        ipText = EQ_STDCALL(char *, 0x00500386, (u32))(*(u32 *)(fromAddr + 4));
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_INET %s:%u", ipText, acc);
                    }
                    else if (addrFamily == 6)
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_IPX %s:%s:%u", status, ipxText,
                            *(u16 *)(fromAddr + 0xc));
                    }
                    else if (addrFamily == 0x45)
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "AF_COMPORT %u", (int)*(short *)(fromAddr + 2));
                    }
                    else
                    {
                        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
                            (char *)addrText, "unknown address family (%u)", (u16)addrFamily);
                    }
                    EQ_CDECL(void, 0x004F66F0, (const char *, ...))(
                        "%s illegal message\n", addrText);
                    statCounter = (u32 *)((u8 *)EQ_GLOBAL(void *, 0x0057F698) + 0x148);
                    goto bumpStat;
                }
            }
            if ((int)packetLen < 2)
            {
                return result;
            }
            hostWord = EQ_STDCALL(u16, 0x00500380, (u16))(*packet);
            if (hostWord == 0xffff)
            {
                tmpSlot = (LPCRITICAL_SECTION)(packetLen - 2);
                arrival = (u32 *)EQ_CDECL(void *, 0x004F48C0, (u32))(packetLen + 0x3a);
                puVar10 = arrival;
                for (work = 0xf; work != 0; work = work + -1)
                {
                    *puVar10 = 0;
                    puVar10 = puVar10 + 1;
                }
                EQ_CDECL(void, 0x004F4B20, (void *, int))(arrival, 0);
                arrival[9] = packetLen - 2;
                *(u16 *)(arrival + 5) = 0xffff;
                puVar10 = (u32 *)EQ_CDECL(void *, 0x004F4C90, (void *))(arrival);
                srcCursor = packet + 1;
                for (acc = (u32)tmpSlot >> 2; acc != 0; acc = acc - 1)
                {
                    *puVar10 = *(u32 *)srcCursor;
                    srcCursor = srcCursor + 2;
                    puVar10 = puVar10 + 1;
                }
                for (acc = (u32)tmpSlot & 3; acc != 0; acc = acc - 1)
                {
                    *(char *)puVar10 = (char)*srcCursor;
                    srcCursor = (u16 *)((int)srcCursor + 1);
                    puVar10 = (u32 *)((int)puVar10 + 1);
                }
                arrival[0xb] = *(u32 *)fromAddr;
                arrival[0xc] = *(u32 *)(fromAddr + 4);
                arrival[0xd] = *(u32 *)(fromAddr + 8);
                arrival[0xe] = *(u32 *)(fromAddr + 0xc);
                if ((arrival[4] == 0) || (*(int *)(arrival[4] + 0x54) == 0))
                {
                    EQ_CDECL(void, 0x004F4B60, (void *))(arrival);
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                    if (*(int *)(rdpInstance + 0xa4) == 0)
                    {
                        work = 0;
                    }
                    else
                    {
                        work = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
                    }
                    arrival[10] = 0;
                    EQ_CDECL(void, 0x004F54C0, (void *, void *))(
                        (void *)(rdpInstance + 0xa4), arrival);
                    if (work == 0)
                    {
                        EQ_CDECL(void, 0x004F5420, (void *))(rdpInstance + 0xa0);
                    }
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                }
                else
                {
                    EQ_CDECL(void, 0x004F4AC0, (void *))(arrival);
                }
            }
            else
            {
                connection = (u32)EQ_CDECL(void *, 0x004F51C0, (void *, void *))(
                    (void *)(rdpInstance + 0x50), fromAddr);
                if (connection != 0)
                    goto haveConnection;
                if ((hostWord & 0x2000) == 0)
                {
                    statCounter = (u32 *)((u8 *)EQ_GLOBAL(void *, 0x0057F698) + 0x168);
                }
                else
                {
                    EQ_CDECL(void, 0x004F12E0, (int, u32 *, void *, int))(
                        rdpInstance, &connection, fromAddr, 0);
                    statCounter = (u32 *)((u8 *)EQ_GLOBAL(void *, 0x0057F698) + 0x158);
                }
                acc = *statCounter;
                *statCounter = acc + 1;
                statCounter[1] = statCounter[1] + (u32)(0xfffffffe < acc);
            }
            if (connection == 0)
            {
                return result;
            }
haveConnection:
            if (*(int *)(connection + 0x60) == 0)
            {
                work = EQ_CDECL(int, 0x004F1A40, (int, u16 *, u32, u32 *))(
                    connection, packet, packetLen, packetInfo);
                parseResult = work;
                rdp_timeGetTime_guess();
                rdpStats = (u8 *)EQ_GLOBAL(void *, 0x0057F698);
                if (work == 0)
                {
                    EQ_CDECL(void, 0x004F1CE0, (int, u32 *, int *))(
                        connection, packetInfo, &processFlag);
                    if (processFlag != 0)
                    {
                        *(u32 *)(rdpInstance + 0x100) = *(int *)(rdpInstance + 0x100) + packetLen + 0x1c;
                    }
                }
                else if ((packetInfo[0] & 0x2000) != 0)
                {
                    acc = *(u32 *)(rdpStats + 0x160);
                    *(u32 *)(rdpStats + 0x160) = acc + 1;
                    *(u32 *)(rdpStats + 0x164) = *(int *)(rdpStats + 0x164) + (u32)(0xfffffffe < acc);
                }
                p_Var14 = (LPCRITICAL_SECTION)connection;
                if ((*(int *)(connection + 0x77c) == 0) && (*(int *)(connection + 0x788) == 0))
                {
                    tmpSlot = (LPCRITICAL_SECTION)connection;
                    work = (int)EQ_CDECL(void *, 0x004F48C0, (u32))(0x3c);
                    EQ_CDECL(void, 0x004F4C20, (int, int))(work, (int)p_Var14);
                    if ((*(int *)(work + 0x10) == 0) || (*(int *)(*(int *)(work + 0x10) + 0x54) == 0))
                    {
                        EQ_CDECL(void, 0x004F4B60, (void *))(work);
                        rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                        if (*(int *)(rdpInstance + 0xa4) == 0)
                        {
                            aux = 0;
                        }
                        else
                        {
                            aux = *(int *)(*(int *)(rdpInstance + 0xa4) + 8);
                        }
                        *(u32 *)(work + 0x28) = 0;
                        EQ_CDECL(void, 0x004F54C0, (void *, void *))(
                            (void *)(rdpInstance + 0xa4), (void *)work);
                        if (aux == 0)
                        {
                            EQ_CDECL(void, 0x004F5420, (void *))(rdpInstance + 0xa0);
                        }
                        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdpInstance + 0xb8));
                        p_Var14 = tmpSlot;
                    }
                    else
                    {
                        EQ_CDECL(void, 0x004F4AC0, (void *))(work);
                    }
                    *(u32 *)((int)p_Var14 + 0x788) = 1;
                }
                acc = connection;
                work = *(int *)(connection + 0xc);
                EQ_CDECL(void, 0x004F18B0, (int, int *))(connection, status);
                tmpSlot = (LPCRITICAL_SECTION)(work + 0x6c);
                rdp_enter_critical_section_guess(tmpSlot);
                aux = 2;
                bVar21 = 1;
                oldStatusCursor = (int *)(acc + 0x30);
                newStatusCursor = status;
                do
                {
                    if (aux == 0)
                        break;
                    aux = aux + -1;
                    bVar21 = *oldStatusCursor == *newStatusCursor;
                    oldStatusCursor = oldStatusCursor + 1;
                    newStatusCursor = newStatusCursor + 1;
                } while (bVar21);
                if (!bVar21)
                {
                    work = work + 0x58;
                    *(int *)(acc + 0x30) = status[0];
                    *(int *)(acc + 0x34) = status[1];
                    EQ_CDECL(void, 0x004F4F30, (int, void *))(
                        work, (void *)(acc + 0x24));
                    EQ_CDECL(void, 0x004F4FE0, (int))(work);
                }
                rdp_leave_critical_section_guess(tmpSlot);
                rdpStats = (u8 *)EQ_GLOBAL(void *, 0x0057F698);
                if (*(int *)(connection + 0x77c) == 0)
                {
                    acc = *(u32 *)(rdpStats + 0xd0);
                    *(u32 *)(rdpStats + 0xd0) = acc + 1;
                    *(u32 *)(rdpStats + 0xd4) = *(int *)(rdpStats + 0xd4) + (u32)(0xfffffffe < acc);
                }
                else if (*(int *)(connection + 0x54) == 0)
                {
                    if ((((packetInfo[0] & 1) == 0) && (processFlag == 0)) && (parseResult == 0))
                    {
                        if (((((payloadField >> 16) != 0) || ((packetInfo[0] & 0x4000) != 0)) &&
                            (work = EQ_CDECL(int, 0x004F6270, (int, u32 *, int))(
                                connection, packetInfo, (payloadField & 0xffff) + (int)packet),
                             work != 0)))
                        {
                            EQ_CDECL(void, 0x004EF2A0, (int, int, int))(rdpInstance, connection, work);
                        }
                    }
                }
                else
                {
                    acc = *(u32 *)(rdpStats + 200);
                    *(u32 *)(rdpStats + 200) = acc + 1;
                    *(u32 *)(rdpStats + 0xcc) = *(int *)(rdpStats + 0xcc) + (u32)(0xfffffffe < acc);
                }
                acc = connection;
                work = *(int *)(connection + 0xc);
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connection + 0x3c));
                aux = (int)EQ_CDECL(void *, 0x004F5330, (void *, void *))(
                    (void *)(work + 0x50), (void *)acc);
                if (aux != 0)
                {
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(work + 0x6c));
                    EQ_CDECL(void, 0x004F4DC0, (void *, void *))(
                        (void *)(work + 0x58), (void *)(aux + 0x24));
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(work + 0x6c));
                    EQ_CDECL(void, 0x004F1890, (void *))(aux);
                    EQ_CDECL(void, 0x005230DD, (void *))(aux);
                }
            }
            return result;
        }
    }
    if (packetLen == 4)
    {
        return *(u32 *)packet;
    }
    statCounter = (u32 *)((u8 *)EQ_GLOBAL(void *, 0x0057F698) + 0x138);
bumpStat:
    acc = *statCounter;
    *statCounter = acc + 1;
    statCounter[1] = statCounter[1] + (u32)(0xfffffffe < acc);
    return result;
}

// FUNCTION: TRILOGY 0x004efd10
// Handles an ICMP error (unreachable, quench, time exceeded) about one of our UDP packets: tells its connection
// @early-stop: checksum folding, hash/stat setup, and status register layout still differ.
void process_icmp_packet_guess(int rdpInstance, u8 *ipPacket, int packetLen, int fromAddr)
{
    LPCRITICAL_SECTION p_Var1;
    int ownerRdp;
    u16 destPortNet;
    int tmp;
    u16 ip_header_len;
    u16 udp_header_len;
    u8 tmpByte;
    u32 checksumAcc;
    u32 source_address;
    u32 destination_address;
    int connection;
    u16 destPortHost;
    int headerBytes;
    u8 *udp_header;
    /* Stats use the first word before the status call fills the same storage. */
    union LocalStatsAndStatus
    {
        int status[2];
        struct
        {
            u8 packet_byte;
            u8 source_port_high;
            u16 translated_port;
        } packet;
        u32 packet_word;
    } scratch;
    struct LocalHashKey
    {
        u16 type;
        u16 port;
        u32 address;
        u32 reserved[2];
    } local_key;
    char ipText[20];

    if ((packetLen != 0xffffffff) &&
        (*(u32 *)(rdpInstance + 0xfc) = *(u32 *)(rdpInstance + 0xfc) + packetLen + 0x1c,
         packetLen >= 0x1c) &&
        ((ip_header_len = (*ipPacket & 0xf) * 4),
         packetLen >= (u32)(ip_header_len + 8)) &&
        ((tmpByte = ipPacket[ip_header_len]), tmpByte >= 3) &&
        ((tmpByte <= 4) || (tmpByte == 0xb)) &&
        (packetLen >= (u32)(ip_header_len + 0x1c)) &&
        ((udp_header_len = (ipPacket[ip_header_len + 8] & 0xf) * 4),
         (headerBytes = udp_header_len + ip_header_len),
         packetLen >= (u32)(headerBytes + 0x10)) &&
        (ipPacket[ip_header_len + 0x11] == 0x11) &&
        ((*(u16 *)(ipPacket + headerBytes + 8) == *(u16 *)(rdpInstance + 0x16)) ||
         (*(u16 *)(ipPacket + headerBytes + 8) == *(u16 *)(rdpInstance + 0x26))))
    {
        udp_header = ipPacket + headerBytes + 8;
        strcpy(ipText, rdp_inet_ntoa_guess(*(u32 *)(fromAddr + 4)));
        strcpy(ipText, rdp_inet_ntoa_guess(*(u32 *)(ipPacket + ip_header_len + 0x14)));
        strcpy(ipText, rdp_inet_ntoa_guess(*(u32 *)(ipPacket + ip_header_len + 0x18)));

        rdp_ntohs_guess(*(u16 *)udp_header);
        destPortHost = rdp_ntohs_guess(*(u16 *)(udp_header + 2));
        tmpByte = 0;
        if (*(u16 *)udp_header == *(u16 *)(rdpInstance + 0x26))
        {
            destination_address = *(u32 *)(ipPacket + ip_header_len + 0x18);
            source_address = *(u32 *)(ipPacket + ip_header_len + 0x14);
            checksumAcc = source_address >> 0x10;
            checksumAcc += (u32)*(u16 *)(udp_header + 4) * 2;
            checksumAcc += destination_address >> 0x10;
            checksumAcc += *(u16 *)(udp_header + 6);
            checksumAcc += source_address & 0xffff;
            checksumAcc += destination_address & 0xffff;
            checksumAcc += ipPacket[ip_header_len + 0x11];
            checksumAcc += *(u16 *)(udp_header + 2);
            checksumAcc += *(u16 *)udp_header;
            connection = ((int)checksumAcc >> 0x10) + (checksumAcc & 0xffff);
            checksumAcc = connection + (connection >> 0x10);
            connection = (checksumAcc & 0xff) + ((int)checksumAcc >> 8);
            tmpByte = (u8)~((connection >> 8) + connection);
            destPortNet = rdp_ntohs_guess(*(u16 *)(udp_header + 2));
            destPortNet = (u16)EQ_STDCALL(u16, 0x005003B0, (u16))(destPortNet & 0x7fff);
            *(u16 *)(udp_header + 2) = destPortNet;
        }
        local_key.reserved[0] = 0;
        local_key.reserved[1] = 0;
        local_key.type = 2;
        local_key.port = *(u16 *)(udp_header + 2);
        local_key.address = *(u32 *)(ipPacket + ip_header_len + 0x18);
        connection = EQ_CDECL(int, 0x004F51C0, (int, void *))(
            rdpInstance + 0x50, &local_key);
        if (connection != 0)
        {
            scratch.packet.packet_byte = ipPacket[ip_header_len + 1];
            scratch.packet.source_port_high = udp_header[1];
            scratch.packet.translated_port = destPortHost;
            EQ_CDECL(void, 0x004F1EF0, (int, int, u32, int, int, int))(
                connection, ipPacket[ip_header_len], scratch.packet_word,
                *(u16 *)udp_header == *(u16 *)(rdpInstance + 0x26),
                tmpByte, fromAddr);
            if ((*(int *)(connection + 0x77c) == 0) && (*(int *)(connection + 0x788) == 0))
            {
                tmp = EQ_CDECL(int, 0x004F48C0, (int))(0x3c);
                EQ_CDECL(void, 0x004F4C20, (int, int))(tmp, connection);
                if ((*(int *)(tmp + 0x10) == 0) ||
                    (*(int *)(*(int *)(tmp + 0x10) + 0x54) == 0))
                {
                    EQ_CDECL(void, 0x004F4B60, (int))(tmp);
                    ownerRdp = rdpInstance;
                    p_Var1 = (LPCRITICAL_SECTION)(ownerRdp + 0xb8);
                    rdp_enter_critical_section_guess(p_Var1);
                    headerBytes = ownerRdp + 0xa4;
                    if (*(int *)(ownerRdp + 0xa4) != 0)
                    {
                        rdpInstance = *(int *)(*(int *)(ownerRdp + 0xa4) + 8);
                    }
                    else
                    {
                        rdpInstance = 0;
                    }
                    *(u32 *)(tmp + 0x28) = 0;
                    EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)headerBytes, (void *)tmp);
                    if (rdpInstance == 0)
                    {
                        EQ_CDECL(void, 0x004F5420, (void *))((void *)(ownerRdp + 0xa0));
                    }
                    rdp_leave_critical_section_guess(p_Var1);
                }
                else
                {
                    EQ_CDECL(void, 0x004F4AC0, (void *))((void *)tmp);
                }
                *(u32 *)(connection + 0x788) = 1;
            }
            ownerRdp = *(int *)(connection + 0xc);
            EQ_CDECL(void, 0x004F18B0, (int, int *))(
                connection, scratch.status);
            p_Var1 = (LPCRITICAL_SECTION)(ownerRdp + 0x6c);
            rdp_enter_critical_section_guess(p_Var1);
            if (memcmp((void *)(connection + 0x30), scratch.status,
                       sizeof(scratch.status)) != 0)
            {
                *(int *)(connection + 0x30) = scratch.status[0];
                *(int *)(connection + 0x34) = scratch.status[1];
                ownerRdp = ownerRdp + 0x58;
                EQ_CDECL(void, 0x004F4F30, (int, int))(ownerRdp, connection + 0x24);
                EQ_CDECL(int, 0x004F4FE0, (int))(ownerRdp);
            }
            rdp_leave_critical_section_guess(p_Var1);
            ownerRdp = *(int *)(connection + 0xc);
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(connection + 0x3c));
            tmp = (int)removeConnectionFromHashTable_guess(
                (RdpConnHash_guess *)(ownerRdp + 0x50), (RdpConnection_guess *)connection);
            if (tmp != 0)
            {
                rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
                EQ_CDECL(void, 0x004F4DC0, (int, int))(ownerRdp + 0x58, tmp + 0x24);
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(ownerRdp + 0x6c));
                EQ_CDECL(void, 0x004F1890, (int))(tmp);
                EQ_CDECL(void, 0x005230DD, (void *))((void *)tmp);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004f0120
// Releases all connections, queued data, sockets, and synchronization state for the RDP session.
void cleanup_network_connection_guess(int *rdpInstance)
{
    int *timeoutHeap;
    int *bufferList;
    int entry;
    u32 connection;
    rdp_critical_section_proc_guess deleteCriticalSection;

    timeoutHeap = rdpInstance + 0x16;
    entry = EQ_CDECL(int, 0x004F4FA0, (int *))(timeoutHeap);
    if (entry != 0)
    {
        do
        {
            connection = EQ_CDECL(u32, 0x004F5330, (int *, int *))(rdpInstance + 0x14, (int *)entry);
            EQ_CDECL(void, 0x004F1890, (void *))((void *)connection);
            EQ_CDECL(void, 0x005230DD, (void *))((void *)connection);
            entry = EQ_CDECL(int, 0x004F4FA0, (int *))(timeoutHeap);
        } while (entry != 0);
    }
    entry = EQ_CDECL(int, 0x004F5500, (int *))(rdpInstance + 0x29);
    while (entry != 0)
    {
        EQ_CDECL(void, 0x004F4AC0, (void *))((void *)entry);
        entry = EQ_CDECL(int, 0x004F5500, (int *))(rdpInstance + 0x29);
    }
    bufferList = rdpInstance + 0x34;
    entry = EQ_CDECL(int, 0x004F5500, (int *))(bufferList);
    while (entry != 0)
    {
        EQ_CDECL(void, 0x004F4AC0, (void *))((void *)entry);
        entry = EQ_CDECL(int, 0x004F5500, (int *))(bufferList);
    }
    if (rdpInstance[1] != 0xffffffff)
    {
        EQ_STDCALL(int, 0x005003BC, (u32))(rdpInstance[1]);
    }
    rdpInstance[1] = -1;
    if (rdpInstance[2] != 0xffffffff)
    {
        EQ_STDCALL(int, 0x005003BC, (u32))(rdpInstance[2]);
    }
    rdpInstance[2] = -1;
    if (rdpInstance[3] != 0xffffffff)
    {
        EQ_STDCALL(int, 0x005003BC, (u32))(rdpInstance[3]);
    }
    rdpInstance[3] = -1;
    if (rdpInstance[4] != 0xffffffff)
    {
        EQ_STDCALL(int, 0x005003BC, (u32))(rdpInstance[4]);
    }
    rdpInstance[4] = -1;
    if (*rdpInstance != 0)
    {
        EQ_STDCALL(int, 0x005003B6, (void))();
        *rdpInstance = 0;
    }
    EQ_CDECL(void, 0x004F6940, (void *))(rdpInstance + 0x46);
    EQ_CDECL(void, 0x004F5160, (void *))(rdpInstance + 0x14);
    EQ_CDECL(void, 0x004F8030, (void *))(timeoutHeap);
    deleteCriticalSection = EQ_GLOBAL(rdp_critical_section_proc_guess, 0x005341C8);
    timeoutHeap += 5;
    deleteCriticalSection((LPCRITICAL_SECTION)timeoutHeap);
    EQ_CDECL(void, 0x004F7250, (void *))(rdpInstance + 0x28);
    EQ_CDECL(void, 0x0051CF90, (void *))(rdpInstance + 0x29);
    deleteCriticalSection((LPCRITICAL_SECTION)(rdpInstance + 0x2e));
    EQ_CDECL(void, 0x0051CF90, (void *))(bufferList);
    deleteCriticalSection((LPCRITICAL_SECTION)(rdpInstance + 0x39));
    if (rdpInstance[0x22] == 0)
    {
        EQ_CDECL(void, 0x004F7250, (void *))(rdpInstance + 0x24);
        EQ_CDECL(void, 0x005230DD, (void *))(rdpInstance);
    }
}

// FUNCTION: TRILOGY 0x004f0290
// Network thread of an rdp instance: polls sockets and serial link, receives packets, runs due connection timers
// @early-stop: Stack-slot offsets, the ESI/EDI pair (critical-section pointer and timeout) and the unbundled SEH filter/handler still differ.
void process_network_connections_guess(int *param_1)
{
    char local_6dc[16];
    char local_6cc[804];
    u32 local_3a8;
    u32 local_3a4;
    u32 local_3a0;
    u32 local_39c;
    int local_398[4];
    int local_388;
    int local_384;
    struct sockaddr local_380;
    char local_370[536];
    u32 local_158;
    int local_154;
    DWORD local_150;
    int local_14c;
    u32 local_148;
    int local_144;
    int local_140;
    struct timeval *timeout;
    int rdp;
    int local_134;
    struct timeval local_130;
    DWORD local_128;
    fd_set local_124;
    DWORD local_20;
    int local_6e0;
    /* fake match: the original body runs inside an SEH frame (trylevel 0 before the first load of param_1).
       Its filter and handler lie outside this function's bytes and are not in the bundle, so this filter
       continues the search and an exception takes the same path as before. */
    __try
    {
        rdp = (int)param_1;
        timeout = 0;
        local_128 = rdp_timeGetTime_guess();
        local_20 = rdp_timeGetTime_guess();
        local_134 = 1;
        if (1 < *(int *)(rdp + 0x04) + 1)
            local_134 = *(int *)(rdp + 0x04) + 1;
        if (*(int *)(rdp + 0x08) + 1 > local_134)
            local_134 = *(int *)(rdp + 0x08) + 1;
        if (*(int *)(rdp + 0x10) + 1 > local_134)
            local_134 = *(int *)(rdp + 0x10) + 1;

        do
        {
            if (*(int *)(rdp + 0x8c) == 0)
                break;
            local_124.fd_count = 0;
            if (*(SOCKET *)(rdp + 0x04) != 0xffffffff)
            {
                local_124.fd_array[0] = *(SOCKET *)(rdp + 0x04);
                local_124.fd_count = 1;
            }
            if (*(SOCKET *)(rdp + 0x10) != 0xffffffff && local_124.fd_count < 0x40)
            {
                local_124.fd_array[local_124.fd_count] = *(SOCKET *)(rdp + 0x10);
                local_124.fd_count = local_124.fd_count + 1;
            }
            if (*(SOCKET *)(rdp + 0x08) != 0xffffffff && local_124.fd_count < 0x40)
            {
                local_124.fd_array[local_124.fd_count] = *(SOCKET *)(rdp + 0x08);
                local_124.fd_count++;
            }
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));
            timeout = (struct timeval *)EQ_CDECL(void *, 0x004F7350, (void *, struct timeval *))((void *)(rdp + 0x58), &local_130);
            if (timeout != 0)
                local_14c = timeout->tv_usec / 1000 + timeout->tv_sec * 1000;
            else
                local_14c = -1;
            *(u32 *)(rdp + 0x84) = 0;
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));

            if (*(int *)(rdp + 0x134) != -1)
            {
                local_154 = EQ_CDECL(int, 0x004F70D0, (void *))((void *)(rdp + 0x118));
                local_154 = local_154 - rdp_timeGetTime_guess();
                if (local_154 > 0)
                    local_154 = local_154 * 1000;
                else
                    local_154 = 0;
                if (timeout == 0 || local_154 < timeout->tv_usec || timeout->tv_sec != 0)
                {
                    timeout = &local_130;
                    local_130.tv_sec = 0;
                    local_130.tv_usec = local_154;
                    local_14c = local_154 / 1000;
                }
            }

        if (*(int *)(rdp + 0x134) == -1 || *(int *)(rdp + 0x10) != -1 || *(int *)(rdp + 0x04) != -1)
        {
            if (timeout == 0 || timeout->tv_usec != 0 || timeout->tv_sec != 0)
            {
                if (*(int *)(rdp + 0x10) == -1 && *(int *)(rdp + 0x04) == -1)
                {
                    local_140 = 0;
                    EQ_CDECL(void, 0x004F66E0, (int))(100);
                }
                else
                {
                    local_140 = EQ_STDCALL(int, 0x005003CE, (int, fd_set *, fd_set *, fd_set *, struct timeval *))
                        (local_134, &local_124, 0, 0, timeout);
                }
            }
            else
            {
                local_158 = rdp_timeGetTime_guess() - local_128;
                if (local_158 < 4)
                {
                    local_14c = 4 - local_158;
                    EQ_CDECL(void, 0x004F66E0, (int))(local_14c);
                }
                local_140 = 2;
            }
        }
        else
            EQ_CDECL(void, 0x004F66E0, (int))(local_14c);

        local_20 = rdp_timeGetTime_guess();
        if (*(int *)(rdp + 0x134) != -1)
        {
            local_158 = EQ_CDECL(DWORD, 0x004F70D0, (void *))((void *)(rdp + 0x118));
            local_150 = rdp_timeGetTime_guess();
            if (local_150 != local_158 && -1 < (int)(local_150 - local_158))
            {
                do
                {
                    local_6e0 = EQ_CDECL(int, 0x004F6A30, (void *, void *, int, void *))
                        ((void *)(rdp + 0x118), local_6cc, 0x218, local_6dc);
                    EQ_CDECL(void, 0x004EF550, (int *, void *, int, void *))
                        ((int *)rdp, local_6cc, local_6e0, local_6dc);
                } while (local_6e0 != -1);
                local_150 = rdp_timeGetTime_guess();
                EQ_CDECL(void, 0x004F70B0, (void *, DWORD))((void *)(rdp + 0x118), local_150 + 100);
            }
        }

        if (local_140 > 0)
        {
            if (*(int *)(rdp + 0x04) != -1 &&
                EQ_STDCALL(int, 0x005003C8, (SOCKET, fd_set *))(*(SOCKET *)(rdp + 0x04), &local_124) != 0)
            {
                do
                {
                    local_388 = 0x10;
                    local_384 = EQ_STDCALL(int, 0x005003C2, (SOCKET, char *, int, int, struct sockaddr *, int *))
                        (*(SOCKET *)(rdp + 0x04), local_370, 0x218, 0, &local_380, &local_388);
                    local_398[3] = EQ_CDECL(int, 0x004EF550, (int *, void *, int, struct sockaddr *))
                        ((int *)rdp, local_370, local_384, &local_380);
                    if (local_398[3] != 0)
                        break;
                } while (local_384 != -1);
            }
            if (*(int *)(rdp + 0x10) != -1 &&
                EQ_STDCALL(int, 0x005003C8, (SOCKET, fd_set *))(*(SOCKET *)(rdp + 0x10), &local_124) != 0)
            {
                do
                {
                    memset(&local_380, 0, 16);
                    local_388 = 0x10;
                    local_384 = EQ_STDCALL(int, 0x005003C2, (SOCKET, char *, int, int, struct sockaddr *, int *))
                        (*(SOCKET *)(rdp + 0x10), local_370, 0x218, 0, &local_380, &local_388);
                    local_398[2] = EQ_CDECL(int, 0x004EF550, (int *, void *, int, struct sockaddr *))
                        ((int *)rdp, local_370, local_384, &local_380);
                    if (local_398[2] != 0)
                        break;
                } while (local_384 != -1);
            }
            if (*(int *)(rdp + 0x08) != -1 &&
                EQ_STDCALL(int, 0x005003C8, (SOCKET, fd_set *))(*(SOCKET *)(rdp + 0x08), &local_124) != 0)
            {
                do
                {
                    local_388 = 0x10;
                    memset(&local_380, 0, 16);
                    local_384 = EQ_STDCALL(int, 0x005003C2, (SOCKET, char *, int, int, struct sockaddr *, int *))
                        (*(SOCKET *)(rdp + 0x08), local_370, 0x218, 0, &local_380, &local_388);
                    EQ_CDECL(void, 0x004EFD10, (int *, void *, int, struct sockaddr *))
                        ((int *)rdp, local_370, local_384, &local_380);
                } while (local_384 != -1);
            }
        }
        else
        {
            *(unsigned __int64 *)((char *)EQ_GLOBAL(void *, 0x0057F698) + 0x1a0) += 1;
        }

        local_150 = rdp_timeGetTime_guess();
        local_148 = local_150 - *(u32 *)(rdp + 0x104);
        if (1000 < local_148)
        {
            *(u32 *)(rdp + 0x108) = (*(u32 *)(rdp + 0xfc) * 1000) / local_148;
            *(u32 *)(rdp + 0x10c) = (*(u32 *)(rdp + 0x100) * 1000) / local_148;
            *(u32 *)(rdp + 0xfc) = 0;
            *(u32 *)(rdp + 0x100) = 0;
            *(DWORD *)(rdp + 0x104) = local_150;
        }
        local_128 = rdp_timeGetTime_guess();
        while (1)
        {
            int conn;
            int owner;
            int *list;
            int head;
            int msg;
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));
            local_144 = EQ_CDECL(int, 0x004F4FE0, (void *))((void *)(rdp + 0x58));
            if (local_144 == 0 || *(int *)(local_144 + 0x30) != 0 ||
                (int)(local_128 - *(u32 *)(local_144 + 0x34)) < 0)
                local_144 = 0;
            else
            {
                local_3a8 = *(u32 *)(local_144 + 0x4a8);
                local_3a4 = *(u32 *)(local_144 + 0x4ac);
                local_3a0 = *(u32 *)(local_144 + 0x4b0);
                local_39c = *(u32 *)(local_144 + 0x4b4);
            }
            conn = local_144;
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));
            if (conn == 0)
                break;
            conn = EQ_CDECL(int, 0x004F51C0, (void *, void *))((void *)(rdp + 0x50), &local_3a8);
            local_144 = conn;
            if (conn != 0)
            {
                EQ_CDECL(void, 0x004F2160, (void *, DWORD, int *))((void *)conn, local_128, local_398);
                if (EQ_CDECL(int, 0x004F2370, (void *))((void *)conn) == 0)
                {
                    if (*(int *)(conn + 0x77c) == 0 && *(int *)(conn + 0x788) == 0)
                    {
                        msg = EQ_CDECL(int, 0x004F48C0, (int))(0x3c);
                        EQ_CDECL(void, 0x004F4C20, (void *, void *))((void *)msg, (void *)conn);
                        if (*(int *)(msg + 0x10) == 0 || *(int *)(*(int *)(msg + 0x10) + 0x54) == 0)
                        {
                            EQ_CDECL(void, 0x004F4B60, (void *))((void *)msg);
                            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0xb8));
                            list = (int *)(rdp + 0xa4);
                            head = *list;
                            if (head != 0)
                                head = *(int *)(head + 0x08);
                            *(u32 *)(msg + 0x28) = 0;
                            EQ_CDECL(void, 0x004F54C0, (void *, void *))((void *)list, (void *)msg);
                            if (head == 0)
                                EQ_CDECL(void, 0x004F5420, (void *))((void *)(rdp + 0xa0));
                            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0xb8));
                        }
                        else
                            EQ_CDECL(void, 0x004F4AC0, (void *))((void *)msg);
                        *(u32 *)(conn + 0x788) = 1;
                    }
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));
                    if (memcmp((char *)local_144 + 0x30, local_398, 8) != 0)
                    {
                        *(int *)(local_144 + 0x30) = local_398[0];
                        *(int *)(local_144 + 0x34) = local_398[1];
                        EQ_CDECL(void, 0x004F4F30, (void *, void *))((void *)(rdp + 0x58), (void *)(local_144 + 0x24));
                    }
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(rdp + 0x6c));
                    conn = local_144;
                }
                else
                    EQ_CDECL(void, 0x004F5330, (void *, void *))((void *)(rdp + 0x50), (void *)conn);
                owner = *(int *)(conn + 0x0c);
                rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(conn + 0x3c));
                conn = EQ_CDECL(int, 0x004F5330, (void *, void *))((void *)(owner + 0x50), (void *)conn);
                if (conn != 0)
                {
                    rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(owner + 0x6c));
                    EQ_CDECL(void, 0x004F4DC0, (void *, void *))((void *)(owner + 0x58), (void *)(conn + 0x24));
                    rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(owner + 0x6c));
                    EQ_CDECL(void, 0x004F1890, (void *))((void *)conn);
                    EQ_CDECL(void, 0x005230DD, (void *))((void *)conn);
                }
            }
        }
    } while (1);
        EQ_CDECL(void, 0x004F0120, (int *))((int *)rdp);
    }
    __except (EXCEPTION_CONTINUE_SEARCH)
    {
    }
}

#define RDP_NAKED __declspec(naked)

// FUNCTION: TRILOGY 0x004f0a4f
// Writes captured exception details to Vince_Exception.txt.
RDP_NAKED int __cdecl WriteExceptionReport_guess(void)
{
    __asm
    {
        mov edi, dword ptr [ebp - 14h]
        mov edx, dword ptr [edi]
        mov eax, dword ptr [edx]
        mov dword ptr [ebp - 4b0h], eax
        mov dword ptr [ebp - 7fch], edi
        push 57e660h
        push 57e814h
        lea ecx, [ebp - 7f4h]
        push ecx
        mov eax, 5224fbh
        call eax
        push 567f10h
        lea edx, [ebp - 7f4h]
        push edx
        mov eax, 5223bch
        call eax
        add esp, 14h
        mov esi, eax
        mov dword ptr [ebp - 7f8h], esi
        test esi, esi
        jz file_unavailable
        test edi, edi
        jz no_exception_data
        mov eax, dword ptr [edi]
        mov ecx, dword ptr [eax + 0ch]
        push ecx
        mov edx, dword ptr [eax]
        push edx
        push 57e85ch
        push 57e7f8h
        push esi
        mov eax, 524138h
        call eax
        add esp, 14h
        push esi
        mov eax, 521fbbh
        call eax
        add esp, 4
        mov eax, 1
        ret
    no_exception_data:
        push 57e7dch
        push esi
        mov eax, 524138h
        call eax
        add esp, 8
        push esi
        mov eax, 521fbbh
        call eax
        add esp, 4
    file_unavailable:
        mov eax, 1
        ret
    }
}

// FUNCTION: TRILOGY 0x004f0ae4
// Records the client's fatal worker-thread error, then exits that thread.
void __cdecl LogFatalThreadErrorAndExit_guess(void)
{
    typedef void (__stdcall *sleep_function)(DWORD);
    typedef void (__stdcall *exit_thread_function)(DWORD);
    char filePath[0x4a0];
    long currentTime;
    int errorCode;
    int *parentFrame;
    DWORD *savedStack;
    FILE *file;
    char *timeText;
    exit_thread_function exitThread;

    __asm
    {
        mov eax, dword ptr [ebp]
        mov parentFrame, eax
    }

    savedStack = *(DWORD **)((char *)parentFrame - 0x18);
    currentTime = EQ_CDECL(long, 0x005213E9, (long *))(0);
    EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
        filePath,
        (const char *)0x0057e814,
        (const char *)0x0057e660);
    file = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))(
        filePath,
        (const char *)0x00567f10);
    if (file != 0)
    {
        timeText = EQ_CDECL(char *, 0x005213D0, (long *))(&currentTime);
        errorCode = *(int *)((char *)parentFrame - 0x4b0);
        EQ_CDECL(int, 0x00524138, (FILE *, const char *, ...))(
            file,
            (const char *)0x0057e82c,
            errorCode,
            timeText);
        EQ_CDECL(int, 0x00521FBB, (FILE *))(file);
    }

    EQ_GLOBAL(u32, 0x006EFEC8) = 1;
    EQ_GLOBAL(sleep_function, 0x00534198)(200);
    exitThread = EQ_GLOBAL(exit_thread_function, 0x005340D4);
    __asm
    {
        mov eax, exitThread
        mov ecx, savedStack
        mov esp, ecx
        push 0
        call eax
    }
}

// FUNCTION: TRILOGY 0x004f0b90
// Prepares RDP state for network traffic by resetting connection tracking, queues, and wakeups.
// ORIGINAL FILE: Network/rdp/rdp.c (address: before rdp_create, its only caller)
// __cdecl. Resets the rdp instance: +4..+0x10 = -1, the scalars and the 0x2C bytes at +0x58 zeroed, the
// serial queue (+0x118), the hash header (+0x50), two uevents, two list + lock pairs, and +0x104 = now.
// RdpState_guess names few of these fields, hence the raw offsets.
void initialize_network_connection_state_guess(RdpState_guess *rdp)
{
    char *p = (char *)rdp;

    *(u32 *)(p + 0x04) = (u32)-1;
    *(u32 *)(p + 0x10) = (u32)-1;
    *(u32 *)(p + 0x08) = (u32)-1;
    *(u32 *)(p + 0x0c) = (u32)-1;
    *(u32 *)(p + 0x00) = 0;
    memset(p + 0x14, 0, 0x10);
    memset(p + 0x24, 0, 0x10);
    *(u32 *)(p + 0x44) = 0;
    *(u32 *)(p + 0x4c) = 0;
    *(u32 *)(p + 0x48) = 0;
    *(u32 *)(p + 0x100) = 0;
    *(u32 *)(p + 0xfc) = 0;
    *(u32 *)(p + 0x10c) = 0;
    rdp_init_network_buffer_guess(p + 0x118);
    rdp_init_counter_guess(p + 0x50);
    memset(p + 0x58, 0, 0x2c);
    *(u32 *)(p + 0x84) = 0;
    rdp_uevent_init_guess((void **)(p + 0x90));
    rdp_uevent_init_guess((void **)(p + 0xa0));
    rdp_list_init_guess((RdpList_guess *)(p + 0xa4));
    InitializeCriticalSection((CRITICAL_SECTION *)(p + 0xb8));
    rdp_list_init_guess((RdpList_guess *)(p + 0xd0));
    InitializeCriticalSection((CRITICAL_SECTION *)(p + 0xe4));
    *(u32 *)(p + 0x104) = rdp_timeGetTime_guess();
}

/* Callees used by rdp_create, recorded by their Trilogy addresses. */
#define rdp_create_malloc_guess EQ_CDECL(void *, 0x00522FA3, (size_t size))
#define rdp_create_state_init_guess EQ_CDECL(void, 0x004F0B90, (void *rdp))
#define rdp_create_wsastartup_guess EQ_STDCALL(int, 0x0050038C, (WORD version, LPWSADATA data))
#define rdp_create_queue_init_guess EQ_CDECL(int, 0x004F6810, (void *queue, u32 id))
#define rdp_create_semaphore_decrement_guess EQ_CDECL(void, 0x004F70B0, (void *queue, DWORD now))
#define rdp_create_hash_init_guess EQ_CDECL(int, 0x004F50A0, (void *hash, u32 size))
#define rdp_create_buffer_init_guess EQ_CDECL(int, 0x004F72E0, (void *buffer, int size))
#define rdp_create_semaphore_guess EQ_CDECL(int, 0x004F53C0, (HANDLE *semaphore))
#define rdp_create_getprotobyname_guess EQ_STDCALL(struct protoent *, 0x005003EC, (const char *name))
#define rdp_create_socket_guess EQ_STDCALL(SOCKET, 0x005003E6, (int af, int type, int protocol))
#define rdp_create_htons_guess EQ_STDCALL(u16, 0x005003B0, (u16 value))
#define rdp_create_bind_guess EQ_STDCALL(int, 0x005003E0, (SOCKET s, struct sockaddr *addr, int len))
#define rdp_create_get_last_error_guess EQ_STDCALL(int, 0x005003DA, (void))
#define rdp_create_setsockopt_guess EQ_STDCALL(int, 0x0050039E, (SOCKET s, int level, int option, const char *value, int len))
#define rdp_create_getsockname_guess EQ_STDCALL(int, 0x005003D4, (SOCKET s, struct sockaddr *addr, int *len))
#define rdp_create_nonblocking_guess EQ_CDECL(int, 0x004F7730, (SOCKET s))
#define rdp_create_net_strerror_guess EQ_CDECL(char *, 0x004F73C0, (int error))
#define rdp_create_getsockopt_guess EQ_STDCALL(int, 0x005003A4, (SOCKET s, int level, int option, char *value, int *len))
#define rdp_create_closesocket_guess EQ_STDCALL(int, 0x005003BC, (SOCKET s))
#define rdp_create_cleanup_guess EQ_CDECL(void, 0x004F0120, (void *rdp))
#define rdp_create_thread_guess EQ_CDECL(int, 0x004F7290, (void *thread, void *start, void *context))

// FUNCTION: TRILOGY 0x004f0c70
// @early-stop: Duplicate result test remains at shared cleanup; socket-address writes, flags register use, and socket-option block layout still differ.
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
int rdp_create(void **rdpOut, u_short port, int maxConnections, u32 createFlags)
{
    struct sockaddr *bindAddr;
    u32 serialQueueId;
    u16 netPort;
    u32 *rdpInstance;
    DWORD DVar5;
    struct protoent *protoEntry;
    SOCKET SVar7;
    int sockResult;
    int iVar9;
    int nameLen;
    int broadcastOn;
    int udpProto;
    struct sockaddr scratchAddr;
    int optLen;
    WSADATA wsaData;

    *rdpOut = 0;
    rdpInstance = (u32 *)rdp_create_malloc_guess(0x1ac);
    /* Every failure leaves this loop for the shared cleanup below. */
    for (;;)
    {
        if (rdpInstance == 0)
        {
            iVar9 = 2;
            break;
        }
        rdp_create_state_init_guess(rdpInstance);
        iVar9 = rdp_create_wsastartup_guess(0x101, &wsaData);
        if (iVar9 != 0)
        {
            iVar9 = 1;
            break;
        }
        *rdpInstance = 1;
        serialQueueId = EQ_GLOBAL(u32, 0x0057E760);
        (*(u16 *)&EQ_GLOBAL(u32, 0x0057E760))++;
        iVar9 = rdp_create_queue_init_guess(rdpInstance + 0x46, serialQueueId);
        if (iVar9 != 0)
            break;
        DVar5 = rdp_timeGetTime_guess();
        rdp_create_semaphore_decrement_guess(rdpInstance + 0x46, DVar5);
        iVar9 = rdp_create_hash_init_guess(rdpInstance + 0x14, maxConnections == 1 ? 1 : 12);
        if (iVar9 != 0 ||
            (iVar9 = rdp_create_buffer_init_guess(rdpInstance + 0x16, maxConnections * 2), iVar9 != 0) ||
            (iVar9 = rdp_create_semaphore_guess((HANDLE *)(rdpInstance + 0x28)), iVar9 != 0))
            break;
        protoEntry = rdp_create_getprotobyname_guess(&EQ_GLOBAL(char, 0x0057E880));
        if (protoEntry == 0)
            udpProto = 0x11;
        else
            udpProto = (int)protoEntry->p_proto;
        if ((createFlags & 2) != 0 || (createFlags & 1) != 0)
        {
            SVar7 = rdp_create_socket_guess(2, 2, udpProto);
            rdpInstance[1] = SVar7;
        }
        if ((createFlags & 2) != 0 && rdpInstance[1] == (u32)-1)
        {
            iVar9 = 10;
            break;
        }
        if ((createFlags & 4) != 0 || (createFlags & 1) != 0)
        {
            SVar7 = rdp_create_socket_guess(6, 2, 1000);
            rdpInstance[4] = SVar7;
        }
        if ((createFlags & 4) != 0 && rdpInstance[4] == (u32)-1)
        {
            iVar9 = 10;
            break;
        }
        if (rdpInstance[1] != (u32)-1)
        {
            bindAddr = (struct sockaddr *)(rdpInstance + 5);
            memset(bindAddr, 0, 16);
            bindAddr->sa_family = 2;
            netPort = rdp_create_htons_guess(port);
            *(u16 *)((char *)rdpInstance + 0x16) = netPort;
            iVar9 = rdp_create_bind_guess(rdpInstance[1], bindAddr, 0x10);
            if (iVar9 == -1)
            {
                iVar9 = rdp_create_get_last_error_guess();
                iVar9 = iVar9 == 0x2740 ? 11 : 1;
                break;
            }
            broadcastOn = 1;
            iVar9 = rdp_create_setsockopt_guess(rdpInstance[1], 0xffff, 0x20, (char *)&broadcastOn, 4);
            rdpInstance[0x13] = (u32)(iVar9 == 0);
            nameLen = 0x10;
            iVar9 = rdp_create_getsockname_guess(rdpInstance[1], bindAddr, &nameLen);
            if (iVar9 == -1)
            {
                iVar9 = 1;
                break;
            }
            iVar9 = rdp_create_nonblocking_guess(rdpInstance[1]);
            if (iVar9 != 0)
                break;
            protoEntry = rdp_create_getprotobyname_guess(&EQ_GLOBAL(char, 0x0057E878));
            if (protoEntry == 0)
                iVar9 = 1;
            else
                iVar9 = (int)protoEntry->p_proto;
            SVar7 = rdp_create_socket_guess(2, 3, iVar9);
            rdpInstance[2] = SVar7;
            if (SVar7 == (SOCKET)-1)
            {
                iVar9 = rdp_create_get_last_error_guess();
                rdp_create_net_strerror_guess(iVar9);
            }
            else
            {
                iVar9 = rdp_create_nonblocking_guess(SVar7);
                if (iVar9 != 0)
                    break;
                memset(&scratchAddr, 0, 16);
                scratchAddr.sa_family = 2;
                *(u16 *)&scratchAddr.sa_data[0] = 0;
                iVar9 = rdp_create_bind_guess(rdpInstance[2], &scratchAddr, 0x10);
                if (iVar9 == -1)
                {
                    iVar9 = 1;
                    break;
                }
                SVar7 = rdp_create_socket_guess(2, 2, udpProto);
                rdpInstance[3] = SVar7;
                if (SVar7 == (SOCKET)-1)
                {
                    iVar9 = 1;
                    break;
                }
                bindAddr = (struct sockaddr *)(rdpInstance + 9);
                memset(bindAddr, 0, 16);
                SVar7 = rdpInstance[3];
                bindAddr->sa_family = 2;
                iVar9 = rdp_create_bind_guess(SVar7, bindAddr, 0x10);
                if (iVar9 == -1)
                {
                    iVar9 = 1;
                    break;
                }
                SVar7 = rdpInstance[3];
                nameLen = 0x10;
                iVar9 = rdp_create_getsockname_guess(SVar7, bindAddr, &nameLen);
                if (iVar9 == -1)
                {
                    iVar9 = 1;
                    break;
                }
                iVar9 = rdp_create_nonblocking_guess(rdpInstance[3]);
                if (iVar9 != 0)
                    break;
                SVar7 = rdpInstance[3];
                optLen = 4;
                iVar9 = rdp_create_getsockopt_guess(SVar7, 0, 7, (char *)(rdpInstance + 0x11), &optLen);
                if (iVar9 != 0)
                {
                    iVar9 = rdp_create_get_last_error_guess();
                    if (iVar9 != 0x273a)
                    {
                        iVar9 = rdp_create_get_last_error_guess();
                        rdp_create_net_strerror_guess(iVar9);
                        iVar9 = 1;
                        break;
                    }
                    rdp_create_closesocket_guess(rdpInstance[3]);
                    rdpInstance[3] = (u32)-1;
                }
                if (rdpInstance[3] != (u32)-1 &&
                    (iVar9 = rdp_create_setsockopt_guess(rdpInstance[3], 0, 7,
                        (char *)(rdpInstance + 0x11), 4), iVar9 != 0))
                {
                    iVar9 = rdp_create_get_last_error_guess();
                    if (iVar9 != 0x273a)
                    {
                        iVar9 = rdp_create_get_last_error_guess();
                        rdp_create_net_strerror_guess(iVar9);
                        iVar9 = 1;
                        break;
                    }
                    rdp_create_closesocket_guess(rdpInstance[3]);
                    rdpInstance[3] = (u32)-1;
                }
            }
        }
        if (rdpInstance[4] != (u32)-1)
        {
            SVar7 = rdpInstance[4];
            memset(&scratchAddr, 0, 14);
            scratchAddr.sa_family = 6;
            *(u16 *)&scratchAddr.sa_data[10] = port;
            iVar9 = rdp_create_bind_guess(SVar7, &scratchAddr, 0xe);
            if (iVar9 == -1)
            {
                iVar9 = rdp_create_get_last_error_guess();
                iVar9 = iVar9 == 0x2740 ? 11 : 1;
                break;
            }
            iVar9 = 1;
            broadcastOn = 1;
            sockResult = rdp_create_setsockopt_guess(rdpInstance[4], 0xffff, 0x20, (char *)&broadcastOn, 4);
            rdpInstance[0x12] = (u32)(sockResult == 0);
            nameLen = 0xe;
            sockResult = rdp_create_getsockname_guess(rdpInstance[4],
                (struct sockaddr *)(rdpInstance + 0xd), &nameLen);
            if (sockResult == -1 || (iVar9 = rdp_create_nonblocking_guess(rdpInstance[4]), iVar9 != 0))
                break;
        }
        rdpInstance[0x45] = createFlags >> 0x1e & 1;
        rdpInstance[0x44] = createFlags >> 0x1f;
        rdpInstance[0x23] = 1;
        iVar9 = rdp_create_thread_guess(rdpInstance + 0x24,
            (void *)0x004F0290, rdpInstance);
        if (iVar9 == 0)
            break;
        rdpInstance[0x23] = 0;
        break;
    }
    if (iVar9 != 0)
    {
        rdpInstance[0x22] = 0;
        rdp_create_cleanup_guess(rdpInstance);
        return iVar9;
    }
    *rdpOut = rdpInstance;
    return iVar9;
}

/* Callees used by rdp_destroy, recorded by their Trilogy addresses. */
#define rdp_destroy_htonl_guess EQ_STDCALL(u32, 0x00500398, (u32 hostlong))
#define rdp_destroy_sendto_guess EQ_STDCALL(int, 0x00500392, (SOCKET s, char *buf, int len, int flags, char *to, int tolen))
#define rdp_destroy_wait_thread_guess EQ_CDECL(void, 0x004F71F0, (void *thread, DWORD *exit_code))
#define rdp_destroy_uevent_destroy_guess EQ_CDECL(void, 0x004F7250, (void *event))
#define rdp_destroy_free_guess EQ_CDECL(void, 0x005230DD, (void *ptr))

// FUNCTION: TRILOGY 0x004f1160
// Stops an active RDP instance, optionally waits for its worker, then frees its state.
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
void rdp_destroy(int networkBase, int freeInstance)
{
    int stateBase;
    int releaseInstance;
    int zero;
    SOCKET s;
    char *to;
    int tolen;

    if (*(int *)(networkBase + 0x8c) != 1)
    {
        return;
    }
    stateBase = networkBase;
    releaseInstance = freeInstance;
    zero = 0;
    if (releaseInstance != zero)
        *(int *)(stateBase + 0x88) = 1;
    else
        *(int *)(stateBase + 0x88) = zero;
    networkBase = 1;
    *(int *)(stateBase + 0x8c) = zero;
    if (*(SOCKET *)(stateBase + 4) != (SOCKET)-1)
    {
        struct sockaddr destinationAddress;
        destinationAddress = *(struct sockaddr *)(stateBase + 0x14);
        *(u32 *)((char *)&destinationAddress + 4) = rdp_destroy_htonl_guess(0x7f000001);
        s = *(SOCKET *)(stateBase + 4);
        to = (char *)&destinationAddress;
        tolen = 0x10;
        rdp_destroy_sendto_guess(s, (char *)&networkBase,
                                4, zero, to, tolen);
    }
    else
    {
        s = *(SOCKET *)(stateBase + 0x10);
        if (s != (SOCKET)-1)
        {
            to = (char *)(stateBase + 0x34);
            tolen = 0xe;
            rdp_destroy_sendto_guess(s, (char *)&networkBase,
                                    4, zero, to, tolen);
        }
    }
    if (releaseInstance != zero)
    {
        rdp_destroy_wait_thread_guess((void *)(stateBase + 0x90), (DWORD *)&networkBase);
        rdp_destroy_uevent_destroy_guess((void *)(stateBase + 0x90));
        rdp_destroy_free_guess((void *)stateBase);
    }
    return;
}

#undef rdp_destroy_htonl_guess
#undef rdp_destroy_sendto_guess
#undef rdp_destroy_wait_thread_guess
#undef rdp_destroy_uevent_destroy_guess
#undef rdp_destroy_free_guess

// FUNCTION: TRILOGY 0x004f1230
// Makes a peer endpoint readable for network diagnostics.
// ORIGINAL FILE: Network/rdp/rdp.c (Mac: format_sockaddr, medium)
// __cdecl. Writes "AF_INET a.b.c.d:port", "AF_COMPORT n", "AF_IPX %s:%s:%u" or "unknown address family (n)".
// Quirk kept: the two %s of the IPX text are never filled in (uninitialised locals).
void format_sockaddr(char *out, const RdpAddr_guess *addr)
{
    char ipx_a[16];
    char ipx_b[16];
    u32 port;

    switch (addr->family)
    {
    case 2:
        port = rdp_ntohs_guess(addr->port);
        sprintf(out, "AF_INET %s:%u", rdp_inet_ntoa_guess(addr->addr), port);
        return;
    case 6:
        sprintf(out, "AF_IPX %s:%s:%u", ipx_a, ipx_b, *(u16 *)&addr->rest[4]);
        return;
    case 0x45:
        sprintf(out, "AF_COMPORT %u", (int)(short)addr->port);
        return;
    default:
        sprintf(out, "unknown address family (%u)", addr->family);
        return;
    }
}

#define rdp_create_connection_malloc_guess \
    EQ_CDECL(void *, 0x00522FA3, (u32 size))
#define rdp_create_connection_free_guess \
    EQ_CDECL(void, 0x005230DD, (void *p))
#define rdp_connection_init_for_create_guess \
    EQ_CDECL(void, 0x004F1800, (RdpConnection_guess *conn, RdpState_guess *rdp, u32 arg3, u32 arg4))
#define rdp_initialize_connection_resources_guess \
    EQ_CDECL(int, 0x004F1870, (RdpConnection_guess *conn))
#define rdp_cleanup_connection_resources_guess \
    EQ_CDECL(void, 0x004F1890, (RdpConnection_guess *conn))
#define rdp_sprintf_for_create_guess \
    EQ_CDECL(int, 0x005224FB, (char *out, const char *format, ...))
#define rdp_heap_insert_connection_guess \
    EQ_CDECL(int, 0x004F4D10, (void *heap, void *elem))
#define rdp_add_connection_to_hash_guess \
    EQ_CDECL(void, 0x004F52D0, (RdpConnHash_guess *hash, RdpConnection_guess *conn))
#define rdp_lock_connection_remote_guess \
    EQ_CDECL(RdpConnection_guess *, 0x004F51C0, (RdpConnHash_guess *hash, RdpAddr_guess *addr))

// FUNCTION: TRILOGY 0x004f12e0
// @early-stop: rc stays in EDI (original spills it to the flags arg slot and reloads it on the init-fail path); malloc-fail path is tail-threaded.
// __cdecl. Allocates and initializes a connection, registering it with the RDP heap and hash on success.
int createNetworkConnection_guess(RdpState_guess *rdp, RdpConnection_guess **out, u32 arg3, u32 flags)
{
    RdpConnection_guess *conn;
    char ipxPart2Text[16];
    char ipxPart1Text[16];
    char addressDescription[64];
    int rc;
    RdpAddr_guess *addr;
    u16 family;
    u32 port;

    if ((flags & 0xfffffffe) != 0)
        return 6;

    conn = (RdpConnection_guess *)rdp_create_connection_malloc_guess(0x7e8);
    if (conn == 0)
    {
        rc = 2;
    }
    else
    {
        rdp_connection_init_for_create_guess(conn, rdp, arg3, flags);
        rc = rdp_initialize_connection_resources_guess(conn);
        if (rc == 0)
        {
            family = conn->remote.family;
            addr = &conn->remote;
            switch (family)
            {
            case 2:
                port = rdp_ntohs_guess(addr->port);
                rdp_sprintf_for_create_guess(addressDescription, "AF_INET %s:%u",
                                              rdp_inet_ntoa_guess(addr->addr), port);
                break;
            case 6:
                rdp_sprintf_for_create_guess(addressDescription, "AF_IPX %s:%s:%u", ipxPart1Text, ipxPart2Text,
                                             *(u16 *)&addr->rest[4]);
                break;
            case 0x45:
                rdp_sprintf_for_create_guess(addressDescription, "AF_COMPORT %u", (int)(short)addr->port);
                break;
            default:
                rdp_sprintf_for_create_guess(addressDescription, "unknown address family (%u)", family);
                break;
            }
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            rdp_heap_insert_connection_guess((char *)rdp + 0x58, (char *)conn + 0x24);
            rdp_add_connection_to_hash_guess(&rdp->hash, conn);
            rdp_leave_critical_section_guess((LPCRITICAL_SECTION)((char *)rdp + 0x6c));
            rdp_lock_connection_remote_guess(&conn->rdp->hash, addr);
            *out = conn;
            return rc;
        }
    }

    if (conn != 0)
    {
        rdp_cleanup_connection_resources_guess(conn);
        rdp_create_connection_free_guess(conn);
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004f1460
// Creates a client connection to the resolved peer address.
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
int rdp_connect(RdpState_guess *rdp, RdpConnection_guess **out, char *name, int port, u32 flags)
{
    int result;
    RdpU64_guess *counters;
    u16 networkPort;
    struct hostent *hostEntry;
    int createResult;
    int connection;
    u32 remoteAddress[4];
    rdp_critical_section_proc_guess leave;

    remoteAddress[1] = 0;
    remoteAddress[2] = 0;
    remoteAddress[0] = 2;
    remoteAddress[3] = 0;
    networkPort = EQ_STDCALL(u16, 0x005003B0, (u16))( (u16)port );
    ((u16 *)remoteAddress)[1] = networkPort;
    remoteAddress[1] = EQ_STDCALL(u32, 0x005003F8, (char *))(name);
    if (remoteAddress[1] == 0xffffffff)
    {
        hostEntry = EQ_STDCALL(struct hostent *, 0x005003F2, (char *))(name);
        if (hostEntry == 0)
            return 0xc;
        remoteAddress[1] = *(u32 *)hostEntry->h_addr_list[0];
    }
    counters = EQ_GLOBAL(RdpStats_guess *, 0x0057F698)->c;
    counters[0x2a]++;
    createResult = EQ_CDECL(int, 0x004F12E0, (RdpState_guess *, RdpConnection_guess **, u32, u32))(
        rdp, (RdpConnection_guess **)&port, (u32)remoteAddress, flags);
    connection = port;
    if (createResult == 0)
    {
        leave = rdp_leave_critical_section_guess;
        *(u32 *)((char *)port + 0x5c) = 1;
        result = *(int *)((char *)port + 0xc);
        leave((LPCRITICAL_SECTION)(port + 0x3c));
        connection = (int)EQ_CDECL(RdpConnection_guess *, 0x004F5330, (RdpConnHash_guess *, RdpConnection_guess *))(
            (RdpConnHash_guess *)(result + 0x50), (RdpConnection_guess *)connection);
        if (connection != 0)
        {
            rdp_enter_critical_section_guess((LPCRITICAL_SECTION)(result + 0x6c));
            EQ_CDECL(void *, 0x004F4DC0, (void *, void *))(
                (void *)(result + 0x58), (void *)(connection + 0x24));
            leave((LPCRITICAL_SECTION)(result + 0x6c));
            EQ_CDECL(void, 0x004F1890, (RdpConnection_guess *))((RdpConnection_guess *)connection);
            EQ_CDECL(void, 0x005230DD, (void *))((void *)connection);
        }
        *out = (RdpConnection_guess *)port;
    }
    return createResult;
}

// FUNCTION: TRILOGY 0x004f1580
// ORIGINAL FILE: Network/rdp/rdp.c (address: between Mac-mapped neighbours)
// __cdecl. Takes the connection out of the instance's hash table.
void close_connection_and_cleanup_guess(RdpState_guess *rdp, RdpConnection_guess *conn)
{
    removeConnectionFromHashTable_guess(&rdp->hash, conn);
}

// FUNCTION: TRILOGY 0x004f15a0
// Calls 0x004F21D0 with a new auto-reset event and waits until it is signalled; 1 if CreateEvent fails.
// ORIGINAL FILE: Network/rdp/rdp.c (Mac: connection_close_wait, high)
// __cdecl. Closes the connection and waits for the close to complete (an event the connection signals); returns
// 1 when the event could not be created.
int connection_close_wait(RdpState_guess *rdp, RdpConnection_guess *conn, u32 reason)
{
    HANDLE event;
    int rc;

    uevent_init_guess(&event);
    rc = uevent_create(&event);
    if (rc != 0)
    {
        rc = 1;
    }
    else
    {
        connection_close(rdp, conn, reason, &event);
        uevent_wait_guess(&event);
    }
    uevent_destroy_guess(&event);
    return rc;
}

#define rdp_receive_list_remove_head_guess(list) \
    EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(list)
#define rdp_receive_serial_recv_from_guess(queue, buffer, size, info) \
    EQ_CDECL(int, 0x004F6A30, (RdpSerialQueue_guess *, void *, u32, void *))(queue, buffer, size, info)
#define rdp_receive_handle_data_recv_guess(rdp, buffer, size, info) \
    EQ_CDECL(void, 0x004EF550, (RdpState_guess *, void *, int, void *))(rdp, buffer, size, info)
#define rdp_receive_usemaphore_decrement_guess(queue, time) \
    EQ_CDECL(void, 0x004F70B0, (RdpSerialQueue_guess *, DWORD))(queue, time)
#define rdp_receive_wait_for_event_guess(event, timeout) \
    EQ_CDECL(int, 0x004F53F0, (HANDLE *, int))(event, timeout)
// FUNCTION: TRILOGY 0x004f1600
// @early-stop: Initial queue-result allocation, critical-section load placement, and return-block order differ; similarity 0.8898.
// ORIGINAL FILE: Network/rdp/rdp.c (from Mac debug map)
// __cdecl.
int rdp_receive(RdpState_guess *rdp, int timeout)
{
    int result;
    DWORD deadline;
    int temp;
    u8 serialFrom[16];
    u8 serialPacket[0x218];
    u8 *base;
    u32 *src;
    rdp_critical_section_proc_guess critical_proc;
    rdp_critical_section_proc_guess leave_proc;

    base = (u8 *)rdp;
    result = 0;
    temp = timeout;
    if (rdp != 0)
    {
        critical_proc = rdp_enter_critical_section_guess;
        critical_proc((LPCRITICAL_SECTION)(base + 0xe4));
        result = (int)rdp_receive_list_remove_head_guess((RdpList_guess *)(base + 0xd0));
        rdp_leave_critical_section_guess((LPCRITICAL_SECTION)(base + 0xe4));
        if (result == 0)
        {
            if ((timeout != 0) && (timeout != -1))
            {
                deadline = rdp_timeGetTime_guess() + timeout;
            }
            do
            {
                do
                {
                    critical_proc = rdp_enter_critical_section_guess;
                    if (*(int *)(base + 0x134) != -1)
                    {
                        int received;
                        do
                        {
                            received = rdp_receive_serial_recv_from_guess(
                                (RdpSerialQueue_guess *)(base + 0x118),
                                serialPacket,
                                0x218,
                                serialFrom);
                            rdp_receive_handle_data_recv_guess(rdp, serialPacket, received, serialFrom);
                        } while (received != -1);
                        rdp_receive_usemaphore_decrement_guess(
                            (RdpSerialQueue_guess *)(base + 0x118),
                            rdp_timeGetTime_guess() + 100);
                    }
                    if (rdp_receive_wait_for_event_guess((HANDLE *)(base + 0xa0), temp) == 0)
                    {
                        return result;
                    }
                    critical_proc((LPCRITICAL_SECTION)(base + 0xb8));
                    critical_proc((LPCRITICAL_SECTION)(base + 0xe4));
                    src = (u32 *)(base + 0xa4);
                    *(RdpList_guess *)(base + 0xd0) = *(RdpList_guess *)src;
                    memset(src, 0, sizeof(RdpList_guess));
                    result = (int)rdp_receive_list_remove_head_guess((RdpList_guess *)(base + 0xd0));
                    leave_proc = rdp_leave_critical_section_guess;
                    leave_proc((LPCRITICAL_SECTION)(base + 0xe4));
                    leave_proc((LPCRITICAL_SECTION)(base + 0xb8));
                    if ((result != 0) || (timeout == 0))
                    {
                        return result;
                    }
                } while (timeout == -1);
                temp = (int)(deadline - rdp_timeGetTime_guess());
            } while (0 < temp);
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x004f1790
// ORIGINAL FILE: Network/rdp/rdp.c (Mac: rdp_get_input_rate, high)
// __cdecl.
u32 rdp_get_input_rate_guess(RdpState_guess *rdp)
{
    return rdp->input_rate;
}

// FUNCTION: TRILOGY 0x004f17a0
// ORIGINAL FILE: Network/rdp/rdp.c (address: between Mac-mapped neighbours)
// __cdecl. Whether the serial queue holds fewer than two items.
int CheckSerialQueueStatus_guess(RdpState_guess *rdp)
{
    return is_connection_inactive_guess(&rdp->serial);
}

// FUNCTION: TRILOGY 0x004f17c0
// ORIGINAL FILE: Network/rdp/rdp.c (address: between Mac-mapped neighbours)
// __cdecl. When the serial queue will have drained (timeGetTime ms).
DWORD getTimeFromEQClient_guess(RdpState_guess *rdp)
{
    return calculateAdjustedClientTime_guess(&rdp->serial);
}

// FUNCTION: TRILOGY 0x004f17e0
// ORIGINAL FILE: Network/rdp/rdp.c (Mac: rdp_get_serial_stall_time, high)
// __cdecl. How long the serial link has been stalled (ms), 0 if not.
DWORD rdp_get_serial_stall_time_guess(RdpState_guess *rdp)
{
    return GetSerialQueueStallTime_guess(&rdp->serial);
}

// FUNCTION: TRILOGY 0x004f44e0
// Initializes a connection's 0x5a0-byte buffer and timing fields; their purpose is unclear.
int TraceStart_guess(void *connectionObject)
{
    int result = 0;
    u8 *connection = (u8 *)connectionObject;
    RdpState_guess *rdp;
    DWORD now;
    void *buffer;

    if (connectionObject == 0)
        return 6;

    rdp = *(RdpState_guess **)(connection + 0x0c);
    if (rdp->socket == (u32)-1)
        return 8;

    EQ_CDECL(RdpConnection_guess *, 0x004F51C0,
             (RdpConnHash_guess *, RdpAddr_guess *))
        (&rdp->hash, (RdpAddr_guess *)(connection + 0x4a8));

    if ((connection[0x64] & 2) != 0 &&
        rdp_timeGetTime_guess() - *(u32 *)(connection + 0x7cc) < *(u32 *)(connection + 0x790))
    {
        result = 9;
    }
    else if (*(u32 *)(connection + 0x7b8) == (u32)-1)
    {
        result = 8;
    }
    else
    {
        if (*(u32 *)(connection + 0x7c0) != 0)
        {
            if (*(u32 *)(connection + 0x498) == 0)
            {
                buffer = EQ_CDECL(void *, 0x00522FA3, (u32))(*(u32 *)(connection + 0x7e0) << 4);
                *(void **)(connection + 0x498) = buffer;
                if (buffer == 0)
                {
                    result = 2;
                }
                else
                {
                    memcpy(buffer, *(void **)(connection + 0x7c0), *(u32 *)(connection + 0x7e0) << 4);
                    *(u32 *)(connection + 0x49c) = *(u32 *)(connection + 0x7e0);
                    *(u32 *)(connection + 0x4a0) = *(u32 *)(connection + 0x7d0);
                }
            }
            else
            {
                EQ_CDECL(void, 0x005230DD, (void *))(*(void **)(connection + 0x7c0));
                *(u32 *)(connection + 0x7c0) = 0;
            }
        }

        if (result == 0)
        {
            if (*(u32 *)(connection + 0x7c0) == 0)
            {
                buffer = EQ_CDECL(void *, 0x00522FA3, (u32))(0x5a0);
                *(void **)(connection + 0x7c0) = buffer;
                if (buffer == 0)
                {
                    result = 2;
                }
            }

            if (result == 0)
            {
                memset(*(void **)(connection + 0x7c0), 0, 0x5a0);
                *(u32 *)(connection + 0x7c4) = 0;
                now = rdp_timeGetTime_guess();
                *(DWORD *)(connection + 0x7cc) = now;
                *(DWORD *)(connection + 0x7c8) = now;
                EQ_CDECL(long, 0x005213E9, (long *))((long *)(connection + 0x7d0));
                *(u32 *)(connection + 0x7d4) = 1;
                *(u32 *)(connection + 0x7d8) = 0x1e;
                *(u32 *)(connection + 0x7dc) = 0;
                *(u32 *)(connection + 0x7e0) = 0;
                *(u32 *)(connection + 0x64) |= 2;
            }
        }
    }
    EQ_CDECL(void, 0x004EF240, (void *))(connectionObject);
    return result;
}

// FUNCTION: TRILOGY 0x004f66e0
// ORIGINAL FILE: unknown (between cypher.c and log.c; queue label rdp)
// __cdecl.
void sleep_milliseconds_guess(DWORD ms)
{
    Sleep(ms);
}

// FUNCTION: TRILOGY 0x004f71f0
// Collects a worker's final exit status during network teardown.
// ORIGINAL FILE: unknown (between eventq.c and uevent.c; queue label rdp)
// __cdecl. Waits for the thread to end and stores its exit code (-1 when there is no thread). Returns 0;
// GetExitCodeThread's result - 1 when that isn't 0 (failure: -1, after an unused GetLastError); -1 when
// the stored code is STILL_ACTIVE (0x103).
int waitForThreadAndGetExitCode_guess(HANDLE *thread, DWORD *code)
{
    DWORD exit_code;
    {
        u32 rc;
    
        if (0 == *thread)
        {
            rc = 0;
            *code = (DWORD)-1;
        }
        else
        {
            WaitForSingleObject(*thread, INFINITE);
            rc = GetExitCodeThread(*thread, &exit_code) - 1;
            *code = exit_code;
            if (0 == rc)
            {
            }
            else
            {
                GetLastError();
                return rc;
            }
        }
        if (*code == STILL_ACTIVE)
            return -1;
        return rc;
    }
}

// FUNCTION: TRILOGY 0x004f7290
// Starts a background network worker at high priority.
// ORIGINAL FILE: unknown (between uevent.c and the heap object; queue label rdp)
// __cdecl. Starts a thread that runs func(arg) (the client's thread stub 0x004F7270), at THREAD_PRIORITY_HIGHEST
// (2). thread[0] = handle, thread[1] = id, thread[2] = func, thread[3] = arg. Returns 1 when the thread could not
// be created.
int createThreadWithPriority_guess(u32 *thread, u32 func, u32 arg)
{
    HANDLE h;

    thread[2] = func;
    thread[3] = arg;
    h = (HANDLE)_beginthreadex(0, 0, (unsigned (__stdcall *)(void *))0x004F7270, thread, 0,
                               (unsigned *)&thread[1]);
    thread[0] = (u32)h;
    if (h != 0)
        SetThreadPriority(h, 2);
    return thread[0] == 0;
}

// FUNCTION: TRILOGY 0x004f7730
// ORIGINAL FILE: unknown (between ustrerror.c and the CRC sender; queue label rdp)
// __cdecl. FIONBIO on; 1 when ioctlsocket fails, else 0.
int set_socket_non_blocking_guess(u32 s)
{
    u32 on;

    on = 1;
    return rdp_ioctlsocket_guess(s, 0x8004667e, &on) != 0;
}
