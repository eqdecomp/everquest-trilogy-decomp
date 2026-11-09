// eqmain: the same original object as trilogy's src/Network/rdp/serial.cpp (cloned function). Address
// macros below are this image's own: any trilogy address not yet replaced is wrong in this file.
/* serial.cpp - the serial link's receive path: finish the overlapped reads of the pending list, refill the receive
 * ring, then take one packet out of it (state 0 drops heartbeat text in front of the packet first).
 * ORIGINAL FILE: unknown (the Mac map names nothing after msg_outgoing.c; see ringbuf.c).
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* eqmain's receive state: param_1 is a CRITICAL_SECTION followed by the serial receive data. The offsets are the
 * sibling's (trilogy serial.cpp) except from queued_bytes on, which are 4 further: eqmain's list is 0x18 bytes. */
typedef struct SerialReceiveState_guess
{
    CRITICAL_SECTION lock;              /* 0x00 */
    DWORD unknown18;                    /* 0x18 */
    HANDLE file;                        /* 0x1C */
    DWORD sequence;                     /* 0x20 */
    RdpRing_guess ring;                 /* 0x24 */
    RdpList_guess pending;              /* 0x38 */
    DWORD queued_bytes;                 /* 0x50 */
    DWORD unknown54[4];                 /* 0x54 */
    DWORD oversized_packets;            /* 0x64 */
    DWORD checksum_errors;              /* 0x68 */
    DWORD sequence_errors;              /* 0x6C */
    DWORD bytes_received;               /* 0x70 */
    DWORD heartbeat_bytes;              /* 0x74 */
    DWORD partial_reads;                /* 0x78 */
    DWORD full_reads;                   /* 0x7C */
    DWORD unknown80;                    /* 0x80 */
    DWORD read_errors;                  /* 0x84 */
    DWORD unknown88[3];                 /* 0x88 */
    DWORD state;                        /* 0x94 */
} SerialReceiveState_guess;
C_ASSERT(offsetof(SerialReceiveState_guess, pending) == 0x38);
C_ASSERT(offsetof(SerialReceiveState_guess, queued_bytes) == 0x50);
C_ASSERT(offsetof(SerialReceiveState_guess, state) == 0x94);
C_ASSERT(sizeof(SerialReceiveState_guess) == 0x98);

/* The frame's locals after return_value (the first local, at the bottom of the frame). */
typedef struct SerialReceiveLocals_guess
{
    DWORD local_230;                    /* 0x00 */
    u8 local_22c[20];                   /* 0x04 */
    u16 local_218[2];                   /* 0x18 */
    u32 local_214;                      /* 0x1C */
    u32 local_210[132];                 /* 0x20 */
} SerialReceiveLocals_guess;

/* 0x1004014C, 0x10040148, 0x10040104, 0x10040094, 0x100400C0: the client's IAT slots for EnterCriticalSection,
 * LeaveCriticalSection, GetOverlappedResult, GetLastError and CloseHandle. */
typedef void (__stdcall *SerialCriticalSection_t)(LPCRITICAL_SECTION);
typedef BOOL (__stdcall *SerialGetOverlappedResult_t)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
typedef DWORD (__stdcall *SerialGetLastError_t)(void);
typedef BOOL (__stdcall *SerialCloseHandle_t)(HANDLE);

#define serial_EnterCriticalSection_guess EQ_GLOBAL(SerialCriticalSection_t, 0x1004014C)
#define serial_LeaveCriticalSection_guess EQ_GLOBAL(SerialCriticalSection_t, 0x10040148)
#define serial_GetOverlappedResult_guess  EQ_GLOBAL(SerialGetOverlappedResult_t, 0x10040104)
#define serial_GetLastError_guess         EQ_GLOBAL(SerialGetLastError_t, 0x10040094)
#define serial_CloseHandle_guess          EQ_GLOBAL(SerialCloseHandle_t, 0x100400C0)

// FUNCTION: EQMAIN 0x1002FD50
// Delivers the next incoming network packet payload to the client.
// Cloned from trilogy serial_recv_from_guess 0x004F6A30 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Finishes the overlapped reads of the pending list, refills the ring, then: state 0 drops heartbeat text
// until the "..." string is found; state 1 takes one packet (8-byte header, checksum over header and
// payload) out of the ring and copies its payload to buffer. Returns the payload length, or 0xffffffff when no
// packet is ready; a bad packet is dropped and counted.
u32 serial_recv_from_guess(LPCRITICAL_SECTION queue, u32 *buffer, u32 bufferCapacity, u32 *sourceInfo)
{
    RdpList_guess *pLVar1;
    HANDLE *recvRing;
    u16 *sumPtr;
    u16 remaining;
    BOOL BVar5;
    int node;
    int tmp;
    DWORD DVar7;
    u32 byteCount;
    u32 sum;
    u32 wordsLeft;
    HANDLE hObject;
    SerialReceiveLocals_guess locals;
    u32 return_value;
    SerialReceiveState_guess *state;

    state = (SerialReceiveState_guess *)queue;
    return_value = 0xffffffff;
    serial_EnterCriticalSection_guess(queue);
    EQ_GLOBAL(DWORD, 0x1013C9EC) = EQ_STDCALL(DWORD, 0x1002F9F0, (void))();
    pLVar1 = &state->pending;
    node = EQ_CDECL(int, 0x1002E120, (RdpList_guess *))(pLVar1);
    while (node != 0)
    {
        BVar5 = serial_GetOverlappedResult_guess(state->file, (LPOVERLAPPED)(node + 8), &locals.local_230, 0);
        if (BVar5 == 0)
        {
            DVar7 = serial_GetLastError_guess();
            if (DVar7 == 0x3e4)
                break;
            state->read_errors = state->read_errors + 1;
            tmp = EQ_CDECL(int, 0x1002E130, (RdpList_guess *))(pLVar1);
            state->queued_bytes = state->queued_bytes - *(DWORD *)(tmp + 0x1c);
            hObject = *(HANDLE *)(node + 0x18);
        }
        else
        {
            if (locals.local_230 == *(DWORD *)(node + 0x1c))
                state->full_reads = state->full_reads + 1;
            else
                state->partial_reads = state->partial_reads + 1;
            tmp = EQ_CDECL(int, 0x1002E130, (RdpList_guess *))(pLVar1);
            state->queued_bytes = state->queued_bytes - *(DWORD *)(tmp + 0x1c);
            hObject = *(HANDLE *)(node + 0x18);
        }
        serial_CloseHandle_guess(hObject);
        EQ_CDECL(void, 0x10028A90, (void *))((void *)node);
        node = EQ_CDECL(int, 0x1002E120, (RdpList_guess *))(pLVar1);
    }
    recvRing = (HANDLE *)&state->ring;
    EQ_CDECL(void, 0x100310F0, (RdpRing_guess *, void *))((RdpRing_guess *)recvRing, state->file);
    for (;;)
    {
        switch (state->state)
        {
        case 0:
            byteCount = state->ring.count;
            while (0x11 < byteCount)
            {
                EQ_CDECL(void, 0x10031250, (RdpRing_guess *, void *, u32))(&state->ring, locals.local_22c, 0x12);
                tmp = memcmp(locals.local_22c, "\n\rwho's yo daddy?", 0x12);
                if (tmp == 0)
                {
                    EQ_CDECL(void, 0x100311C0, (RdpRing_guess *, void *, u32))(&state->ring, 0, 0x12);
                    state->state = 1;
                    break;
                }
                EQ_CDECL(void, 0x100311C0, (RdpRing_guess *, void *, u32))(&state->ring, 0, 1);
                byteCount = state->ring.count;
                state->heartbeat_bytes = state->heartbeat_bytes + 1;
            }
            if (state->state == 0)
                goto done;
            /* fall through */
        case 1:
            if (7 < state->ring.count)
            {
                EQ_CDECL(void, 0x10031250, (RdpRing_guess *, void *, u32))(&state->ring, locals.local_218, 8);
                if ((0x218 < (u16)locals.local_214) ||
                    ((locals.local_218[1] != (u16)state->sequence && (locals.local_218[1] != 0))))
                {
                    state->state = 0;
                    if ((u16)locals.local_214 < 0x219)
                        state->sequence_errors = state->sequence_errors + 1;
                    else
                        state->oversized_packets = state->oversized_packets + 1;
                    continue;
                }
                byteCount = (locals.local_214 & 0xffff) + 8;
                if (byteCount <= state->ring.count)
                {
                    EQ_CDECL(void, 0x10031250, (RdpRing_guess *, void *, u32))(&state->ring, locals.local_218, byteCount);
                    byteCount = locals.local_214 + 8;
                    sum = 0;
                    remaining = (u16)byteCount;
                    sumPtr = locals.local_218;
                    if (1 < remaining)
                    {
                        wordsLeft = (byteCount & 0xffff) >> 1;
                        do
                        {
                            byteCount = byteCount + 0xfffe;
                            remaining = *sumPtr;
                            sumPtr = sumPtr + 1;
                            sum = sum + remaining;
                            wordsLeft = wordsLeft - 1;
                        } while (wordsLeft != 0);
                        remaining = (u16)byteCount;
                    }
                    if (remaining == 1)
                        sum = sum + (u8)*sumPtr;
                    node = (sum >> 0x10) + (sum & 0xffff);
                    if ((short)((short)((u32)node >> 0x10) + (short)node) == -1)
                    {
                        state->bytes_received = state->bytes_received + (locals.local_214 & 0xffff) + 0x1a;
                        EQ_CDECL(void, 0x100311C0, (RdpRing_guess *, void *, u32))(&state->ring, 0, (locals.local_214 & 0xffff) + 8);
                        return_value = locals.local_214 & 0xffff;
                        memcpy(buffer, locals.local_210, return_value);
                        sourceInfo[0] = 0;
                        sourceInfo[1] = 0;
                        sourceInfo[2] = 0;
                        sourceInfo[3] = 0;
                        *(u16 *)sourceInfo = 0x45;
                        *(u16 *)((u8 *)sourceInfo + 2) = locals.local_218[0];
                        state->state = 0;
                        serial_LeaveCriticalSection_guess(queue);
                        return return_value;
                    }
                    tmp = state->checksum_errors;
                    state->state = 0;
                    state->checksum_errors = tmp + 1;
                    continue;
                }
            }
done:
            serial_LeaveCriticalSection_guess(queue);
            return return_value;
        }
    }
}
