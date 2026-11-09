// unit_1002FAE0.cpp - eqmain.dll, the address run 0x1002FAE0..0x1002FC60 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown). This file holds 0x1002FAE0 only.

#include <string.h>
#include <windows.h>

#include "trilogy/eqaddr.h"

// The callees, by address (eqmain.dll's own functions: InitRecord_guess is the unit_1002DE90.cpp function).
#define InitRecord_guess                    EQ_CDECL(void, 0x1002DEA0, (int *, int, int, int))
#define initializeNetworkBufferHeader_guess EQ_CDECL(void, 0x10031090, (void *))
#define networkQueuePeek_guess              EQ_CDECL(int, 0x1002E120, (LONG *))
#define networkQueuePop_guess               EQ_CDECL(int, 0x1002E130, (LONG *))
#define eqmain_release_msg_guess              EQ_CDECL(void, 0x10028A90, (int))
#define freeRingBufferMemory_guess       EQ_CDECL(void, 0x1002E240, (void *))
#define rdp_list_release_stub_guess                        EQ_CDECL(void, 0x10027940, (LONG *))
#define timeGetTimeThunk_guess              EQ_STDCALL(DWORD, 0x1002F9F0, (void))
#define networkLastTick_guess               EQ_GLOBAL(DWORD, 0x1013C9EC)

// FUNCTION: EQMAIN 0x1002FAE0
// Resets the object: handle +0x1C = -1, queue record +0x38, ring header +0x24, zeroes 0x40 bytes at +0x54.
// __cdecl, one stack arg: a pointer to an object of at least 0x98 bytes. Sets +0x1C to -1 and +0x94 to 0, initializes
// the record at +0x38 (InitRecord_guess with 0, 0, 0), sets +0x50 to 0, initializes the ring header at +0x24 and zeroes
// the 0x40 bytes at +0x54. Returns nothing. The trilogy sibling initializeNetworkBuffer_guess 0x004F67C0
// (src/Network/rdp/list.c) resets the same shape with its tail fields one dword lower (+0x90, +0x4C, memset at +0x50).
void __cdecl initializeNetworkBuffer_guess(int buffer)
{
    char *p;
    int *rec;

    p = (char *)buffer;
    *(int *)(p + 0x1c) = -1;
    *(int *)(p + 0x94) = 0;
    rec = (int *)(p + 0x38);
    InitRecord_guess(rec, 0, 0, 0);
    rec[6] = 0;
    initializeNetworkBufferHeader_guess(p + 0x24);
    memset(p + 0x54, 0, 0x40);
    return;
}

// The ring allocator at 0x100310B0 (called by drainNetworkBuffer_guess below, not in the define block above).
#define allocate_memory_buffer_guess        EQ_CDECL(int, 0x100310B0, (void *, size_t))

// FUNCTION: EQMAIN 0x1002FB30
// Inits the section, allocates 8000 bytes at +0x24, then drains the queue and tears down as destroy does.
// @early-stop: VC6 /O2 merges branch handle loads and adjacent cdecl cleanup; remaining differences are those shapes and the resulting branch offsets.
// __cdecl, two stack args: the object (as in destroyNetworkBuffer_guess) and a DWORD whose low word is stored at +0x20.
// Initializes the section, stores that word and allocates the 8000-byte buffer at +0x24; when the allocation fails it
// returns 0 with the section still initialized. Otherwise it enters the section, stamps g_last_send_tick_guess and drains the queue
// as destroyNetworkBuffer_guess does (the DWORD is GetOverlappedResult's bytes-transferred slot, so its value is
// overwritten), then frees the list at +0x24, calls rdp_list_release_stub_guess, leaves and deletes the section. Returns the buffer
// pointer (0 when the allocation failed). The name is a guess.
int __cdecl drainNetworkBuffer_guess(LPCRITICAL_SECTION state, DWORD bytesTransferred)
{
    LONG *queue;
    int buffer;
    char *entry;
    BOOL ok;
    int taken;
    DWORD err;
    HANDLE hObject;

    InitializeCriticalSection(state);
    *(WORD *)&state[1].RecursionCount = (WORD)bytesTransferred;
    buffer = allocate_memory_buffer_guess(&state[1].OwningThread, 8000);
    if (buffer != 0)
    {
        EnterCriticalSection(state);
        networkLastTick_guess = timeGetTimeThunk_guess();
        queue = &state[2].RecursionCount;
        entry = (char *)networkQueuePeek_guess(queue);
        while (entry != 0)
        {
            ok = GetOverlappedResult((HANDLE)state[1].LockCount, (LPOVERLAPPED)(entry + 8),
                                     (LPDWORD)&bytesTransferred, 1);
            if (ok)
            {
                if (bytesTransferred == *(DWORD *)(entry + 0x1c))
                {
                    state[5].LockCount = state[5].LockCount + 1;
                }
                else
                {
                    state[5].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)((int)&(state[5].DebugInfo)->Type + 1);
                }
                taken = networkQueuePop_guess(queue);
                queue[6] = queue[6] - *(int *)(taken + 0x1c);
                hObject = *(HANDLE *)(entry + 0x18);
            }
            else
            {
                err = GetLastError();
                if (err == 0x3e4)
                    break;
                state[5].OwningThread = (HANDLE)((int)state[5].OwningThread + 1);
                taken = networkQueuePop_guess(queue);
                queue[6] = queue[6] - *(int *)(taken + 0x1c);
                hObject = *(HANDLE *)(entry + 0x18);
            }
            CloseHandle(hObject);
            eqmain_release_msg_guess((int)entry);
            entry = (char *)networkQueuePeek_guess(queue);
        }
        freeRingBufferMemory_guess(&state[1].OwningThread);
        rdp_list_release_stub_guess(queue);
        LeaveCriticalSection(state);
        DeleteCriticalSection(state);
    }
    return buffer;
}

// FUNCTION: EQMAIN 0x1002FC60
// Drains the overlapped I/O queue at +0x38, frees the ring at +0x24 and deletes the critical section.
// __cdecl, one stack arg: the object (its CRITICAL_SECTION at +0x00, the file handle at +0x1C, the pending queue record
// at +0x38, the pending byte count at +0x50). Enters the section, stores the tick count in g_last_send_tick_guess, then drains the
// queue: each entry waits on GetOverlappedResult (bWait 1). A failure other than ERROR_IO_INCOMPLETE (0x3E4, which ends
// the drain) bumps +0x84; a success bumps +0x7C when the transfer equals the entry's size at +0x1C, else +0x78. Each entry
// taken off the queue gives its size back from +0x50, has its event closed and is freed. Then it frees the list at +0x24,
// calls rdp_list_release_stub_guess, leaves and deletes the section. Returns nothing. buffer is also GetOverlappedResult's
// bytes-transferred out parameter (&buffer), so that call writes the stack slot the argument lives in.
// @early-stop: the merged CloseHandle tail pins both branches' results to EAX (orig EDX/ECX), and the free's add esp merges into the peek's.
void __cdecl destroyNetworkBuffer_guess(LPCRITICAL_SECTION buffer)
{
    LPCRITICAL_SECTION lpCriticalSection;
    int entry;
    LONG *queue;
    HANDLE hObject;

    lpCriticalSection = buffer;
    EnterCriticalSection(lpCriticalSection);
    networkLastTick_guess = timeGetTimeThunk_guess();
    queue = &lpCriticalSection[2].RecursionCount;
    entry = networkQueuePeek_guess(queue);
    while (entry != 0)
    {
        if (GetOverlappedResult((HANDLE)lpCriticalSection[1].LockCount, (LPOVERLAPPED)(entry + 8),
                                (LPDWORD)&buffer, 1))
        {
            if (buffer == *(LPCRITICAL_SECTION *)(entry + 0x1c))
            {
                lpCriticalSection[5].LockCount = lpCriticalSection[5].LockCount + 1;
            }
            else
            {
                lpCriticalSection[5].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)((int)&(lpCriticalSection[5].DebugInfo)->Type + 1);
            }
            queue[6] -= *(int *)(networkQueuePop_guess(queue) + 0x1c);
            hObject = *(HANDLE *)(entry + 0x18);
        }
        else
        {
            if (GetLastError() == 0x3e4)
                break;
            lpCriticalSection[5].OwningThread = (HANDLE)((int)lpCriticalSection[5].OwningThread + 1);
            queue[6] -= *(int *)(networkQueuePop_guess(queue) + 0x1c);
            hObject = *(HANDLE *)(entry + 0x18);
        }
        CloseHandle(hObject);
        eqmain_release_msg_guess(entry);
        entry = networkQueuePeek_guess(queue);
    }
    freeRingBufferMemory_guess(&lpCriticalSection[1].OwningThread);
    rdp_list_release_stub_guess(queue);
    LeaveCriticalSection(lpCriticalSection);
    DeleteCriticalSection(lpCriticalSection);
    return;
}
