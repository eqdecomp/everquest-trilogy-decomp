/* serial.cpp - the serial link's receive path. */
#include <windows.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

/* This receive state is a CRITICAL_SECTION followed by the serial receive data. */
typedef struct SerialReceiveState_guess
{
    CRITICAL_SECTION lock;             /* 0x00 */
    DWORD unknown18;                    /* 0x18 */
    HANDLE file;                        /* 0x1C */
    DWORD sequence;                     /* 0x20 */
    RdpRing_guess ring;                 /* 0x24 */
    RdpList_guess pending;              /* 0x38 */
    DWORD queued_bytes;                 /* 0x4C */
    DWORD unknown50[4];                 /* 0x50 */
    DWORD oversized_packets;            /* 0x60 */
    DWORD checksum_errors;              /* 0x64 */
    DWORD sequence_errors;              /* 0x68 */
    DWORD bytes_received;                /* 0x6C */
    DWORD heartbeat_bytes;               /* 0x70 */
    DWORD partial_reads;                 /* 0x74 */
    DWORD full_reads;                    /* 0x78 */
    DWORD unknown7C;                     /* 0x7C */
    DWORD read_errors;                   /* 0x80 */
    DWORD unknown84[3];                  /* 0x84 */
    DWORD state;                         /* 0x90 */
} SerialReceiveState_guess;

typedef struct SerialReceiveLocals_guess
{
    DWORD local_230;                    /* 0x00 */
    u8 local_22c[20];                   /* 0x04 */
    u16 local_218[2];                   /* 0x18 */
    u32 local_214;                      /* 0x1C */
    u32 local_210[132];                 /* 0x20 */
} SerialReceiveLocals_guess;

typedef BOOL (__stdcall *SerialGetOverlappedResult_t)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
typedef DWORD (__stdcall *SerialGetLastError_t)(void);
typedef BOOL (__stdcall *SerialCloseHandle_t)(HANDLE);
typedef void (__stdcall *SerialCriticalSection_t)(LPCRITICAL_SECTION);

#define serial_GetOverlappedResult_guess EQ_GLOBAL(SerialGetOverlappedResult_t, 0x005340C0)
#define serial_GetLastError_guess       EQ_GLOBAL(SerialGetLastError_t, 0x005341AC)
#define serial_CloseHandle_guess        EQ_GLOBAL(SerialCloseHandle_t, 0x005341D4)
#define serial_EnterCriticalSection_guess EQ_GLOBAL(SerialCriticalSection_t, 0x00534194)
#define serial_LeaveCriticalSection_guess EQ_GLOBAL(SerialCriticalSection_t, 0x00534190)

// FUNCTION: TRILOGY 0x004f6a30
// Serial-link recvfrom: reaps done writes, syncs on the marker string, returns one checksummed frame's payload
// ORIGINAL FILE: Network/rdp/serial.c (from Mac debug map)
// @early-stop: EBP frame, heartbeat compare, checksum loop, and cleanup layout still differ.
u32 serial_recv_from(LPCRITICAL_SECTION port, u32 *buf, u32 bufLen, u32 *fromAddr)
{
    LONG *pLVar1;
    RdpRing_guess *rxRing;
    u16 remaining;
    BOOL BVar4;
    int tmp;
    DWORD DVar6;
    u32 len;
    int compareResult;
    u32 sum;
    u32 wordCount;
    u8 *gotPtr;
    u16 *sumPtr;
    u32 *src;
    u8 *magicPtr;
    BOOL below;
    BOOL equal;
    SerialReceiveLocals_guess locals;
    volatile u32 return_value; /* fake match: retain the original four-byte return slot */

    return_value = 0xffffffff;
    serial_EnterCriticalSection_guess(port);
    EQ_GLOBAL(DWORD, 0x006EFED4) = EQ_CDECL(DWORD, 0x004F66D0, (void))();
    pLVar1 = &port[2].RecursionCount;
    while ((*pLVar1 != 0 && (compareResult = *(int *)(*pLVar1 + 8), compareResult != 0)))
    {
        BVar4 = serial_GetOverlappedResult_guess((HANDLE)port[1].LockCount,
                                                 (LPOVERLAPPED)(compareResult + 0x10),
                                                 &locals.local_230, 0);
        if (BVar4 != 0)
        {
            if (locals.local_230 == *(DWORD *)(compareResult + 0x24))
                port[5].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)
                    ((DWORD)port[5].DebugInfo + 1);
            else
                port[4].SpinCount = port[4].SpinCount + 1;
            tmp = (int)EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(
                (RdpList_guess *)&port[2].RecursionCount);
            port[3].LockCount = port[3].LockCount - *(DWORD *)(tmp + 0x24);
            serial_CloseHandle_guess(*(HANDLE *)(compareResult + 0x20));
            EQ_CDECL(void, 0x004F4AC0, (void *))((void *)compareResult);
        }
        else
        {
            DVar6 = serial_GetLastError_guess();
            if (DVar6 == 0x3e4)
                break;
            port[5].RecursionCount = port[5].RecursionCount + 1;
            tmp = (int)EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(
                (RdpList_guess *)&port[2].RecursionCount);
            port[3].LockCount = port[3].LockCount - *(DWORD *)(tmp + 0x24);
            serial_CloseHandle_guess(*(HANDLE *)(compareResult + 0x20));
            EQ_CDECL(void, 0x004F4AC0, (void *))((void *)compareResult);
        }
    }
    rxRing = (RdpRing_guess *)&port[1].OwningThread;
    EQ_CDECL(void, 0x004F8050, (RdpRing_guess *, void *))(
        rxRing, (HANDLE)port[1].LockCount);

    for (;;)
    {
        switch ((u32)port[6].DebugInfo)
        {
        case 0:
            len = (u32)port[2].LockCount;
            while (len >= 0x12)
            {
                EQ_CDECL(void, 0x004F81B0, (RdpRing_guess *, void *, u32))(
                    rxRing, locals.local_22c, 0x12);
                tmp = 0x12;
                below = FALSE;
                compareResult = 0;
                equal = TRUE;
                gotPtr = locals.local_22c;
                magicPtr = (u8 *)"\n\rwho's yo daddy?";
                do
                {
                    if (tmp == 0)
                        break;
                    tmp = tmp + -1;
                    below = *gotPtr < *magicPtr;
                    equal = *gotPtr == *magicPtr;
                    gotPtr = gotPtr + 1;
                    magicPtr = magicPtr + 1;
                } while (equal);
                if (!equal)
                    compareResult = (1 - (u32)below) - (u32)(below != 0);
                if (compareResult == 0)
                {
                    EQ_CDECL(void, 0x004F8120, (RdpRing_guess *, void *, u32))(
                        rxRing, 0, 0x12);
                    port[6].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)1;
                    break;
                }
                EQ_CDECL(void, 0x004F8120, (RdpRing_guess *, void *, u32))(
                    rxRing, 0, 1);
                len = (u32)port[2].LockCount;
                port[4].LockSemaphore = (HANDLE)((DWORD)port[4].LockSemaphore + 1);
            }
            if (port[6].DebugInfo == 0)
            {
                serial_LeaveCriticalSection_guess(port);
                return return_value;
            }
            /* fall through */
        case 1:
                if ((u32)port[2].LockCount >= 8)
            {
                EQ_CDECL(void, 0x004F81B0, (RdpRing_guess *, void *, u32))(
                    rxRing, locals.local_218, 8);
                if ((0x218 < (u16)locals.local_214) ||
                    ((locals.local_218[1] != (u16)port[1].RecursionCount &&
                      (locals.local_218[1] != 0))))
                {
                    port[6].DebugInfo = 0;
                    if ((u16)locals.local_214 > 0x218)
                        port[4].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)
                            ((DWORD)port[4].DebugInfo + 1);
                    else
                        port[4].RecursionCount = port[4].RecursionCount + 1;
                    continue;
                }
                len = (locals.local_214 & 0xffff) + 8;
                if ((u32)port[2].LockCount >= len)
                {
                    EQ_CDECL(void, 0x004F81B0, (RdpRing_guess *, void *, u32))(
                        rxRing, locals.local_218, len);
                    len = locals.local_214 + 8;
                    sum = 0;
                    remaining = (u16)len;
                    sumPtr = locals.local_218;
                    if (1 < remaining)
                    {
                        wordCount = (u16)len >> 1;
                        do
                        {
                            len = len + 0xfffe;
                            remaining = *sumPtr;
                            sumPtr = sumPtr + 1;
                            sum = sum + remaining;
                            wordCount = wordCount - 1;
                        } while (wordCount != 0);
                        remaining = (u16)len;
                    }
                    if (remaining == 1)
                        sum = sum + (u8)*sumPtr;
                    compareResult = (sum >> 0x10) + (sum & 0xffff);
                    if ((short)((short)((u32)compareResult >> 0x10) + (short)compareResult) == -1)
                    {
                        port[4].OwningThread = (HANDLE)
                            ((DWORD)port[4].OwningThread +
                             (locals.local_214 & 0xffff) + 0x1a);
                        EQ_CDECL(void, 0x004F8120, (RdpRing_guess *, void *, u32))(
                            rxRing, 0, (locals.local_214 & 0xffff) + 8);
                        src = locals.local_210;
                        for (len = (locals.local_214 & 0xffff) >> 2;
                             len != 0; len = len - 1)
                        {
                            *buf = *src;
                            src = src + 1;
                            buf = buf + 1;
                        }
                        for (len = locals.local_214 & 3; len != 0; len = len - 1)
                        {
                            *(u8 *)buf = *(u8 *)src;
                            src = (u32 *)((u8 *)src + 1);
                            buf = (u32 *)((u8 *)buf + 1);
                        }
                        fromAddr[0] = 0;
                        fromAddr[1] = 0;
                        fromAddr[2] = 0;
                        fromAddr[3] = 0;
                        *(u16 *)fromAddr = 0x45;
                        *(u16 *)((u8 *)fromAddr + 2) = locals.local_218[0];
                        port[6].DebugInfo = 0;
                        return_value = locals.local_214 & 0xffff;
                        serial_LeaveCriticalSection_guess(port);
                        return return_value;
                    }
                    compareResult = port[4].LockCount;
                    port[6].DebugInfo = 0;
                    port[4].LockCount = compareResult + 1;
                    continue;
                }
            }
            serial_LeaveCriticalSection_guess(port);
            return return_value;
        default:
            continue;
        }
    }
}
