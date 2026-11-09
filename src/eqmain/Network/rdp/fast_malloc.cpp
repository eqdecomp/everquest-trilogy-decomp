#include <windows.h>
#include <string.h>

#include "eqmain/rdp.h"
#include "trilogy/eqaddr.h"

// eqmain.dll's rdp pool globals (this image's addresses): the chunk list, the four pool locks 0x3C apart (each starts
// with its CRITICAL_SECTION), and the initial size.
#define g_pool_chunks_guess         EQ_ARRAY(RdpList_guess, 0x1013D820)
#define g_pool_locks_guess          EQ_ARRAY(u8, 0x1013D744)
#define g_pool_locks_end_guess      EQ_ARRAY(u8, 0x1013D834)
#define g_pool_init_size_guess      EQ_GLOBAL(u32, 0x1013C9E4)

// FUNCTION: EQMAIN 0x10028850
// Releases pooled memory and resets allocator state.
// __cdecl, no arguments (__stdcall is the same code; the test harness declares it __cdecl). Frees every block left on
// the chunk list, then deletes the four pool locks in address order through the import slot 0x100400F0, then clears
// the initial size. The value folded in before each delete is never read again.
void cleanupMemoryPool_guess(void)
{
    typedef void (__stdcall *DeleteCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    DeleteCriticalSectionFn_guess deleteCS;
    void *chunk;
    LPCRITICAL_SECTION lock;
    u32 folded = 0;

    chunk = EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))(g_pool_chunks_guess);
    while (chunk != 0)
    {
        EQ_CDECL(void, 0x10033568, (void *))(chunk);
        chunk = EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))(g_pool_chunks_guess);
    }
    deleteCS = EQ_GLOBAL(DeleteCriticalSectionFn_guess, 0x100400F0);
    lock = (LPCRITICAL_SECTION)g_pool_locks_guess;
    do
    {
        folded = (folded >> 1) + *(u32 *)((u8 *)lock - 0x1C);   // the dword 0x1C below each lock (+0x20 of the pool before)
        deleteCS(lock);
        lock = (LPCRITICAL_SECTION)((u8 *)lock + 0x3C);
    } while ((u8 *)lock < g_pool_locks_end_guess);
    g_pool_init_size_guess = 0;
}

// eqmain's own globals (this image's VAs, not trilogy's). g_rdp_pools_guess is the four RdpPool_guess size classes,
// 0x3C apart (iVar5 = class * 0x3C, so every access is base + iVar5 + offset); class 3's count and free list follow
// it, then the chunk list.
#define g_rdp_pools_guess           EQ_ARRAY(u8, 0x1013D720)
#define g_rdp_pool3_count_guess     EQ_GLOBAL(DWORD, 0x1013D7DC)
#define g_rdp_pool3_list_guess      EQ_GLOBAL(RdpList_guess, 0x1013D7E0)
#define g_rdp_pool_chunks_guess     EQ_GLOBAL(RdpList_guess, 0x1013D820)

// eqmain's four pool classes as RdpPool_guess (0x3C apart); class k's payload is the threshold dword at +0x04.
#define g_rdp_pool_class_guess      EQ_ARRAY(RdpPool_guess, 0x1013D720)

// FUNCTION: EQMAIN 0x100288B0
// Allocates storage for RDP network messages.
// Port of trilogy fast_malloc_guess 0x004F48C0 (BinDiff sibling 0.989) with eqmain's pools: class k's size threshold is
// the dword at 0x1013D724 + k * 0x3C, and the classes end at 0x1013D814.
// __cdecl. A block of at least `size` bytes: from the smallest class that fits, else malloc(size + 8). The 8-byte header
// holds the class index (4 = malloc'd) and the payload size (the request for malloc'd ones); returns the bytes after it,
// NULL when malloc fails.
void *fast_malloc_guess(u32 size)
{
    int idx;
    u32 *payload;
    u32 *blk;
    u32 *ret;

    idx = 0;
    payload = &g_rdp_pool_class_guess[0].payload;
    do
    {
        if (!(size <= *payload))
        {
        }
        else
        {
            blk = (u32 *)EQ_CDECL(void *, 0x10028920, (int))(idx);
            break;
        }
        payload += 15;
        idx++;
    } while (payload < &g_rdp_pool_class_guess[4].payload);
    /* running out of classes leaves blk unset, but that is idx 4, and idx 4 always takes this malloc */
    if (idx == 4)
        blk = (u32 *)EQ_CDECL(void *, 0x1003342E, (size_t))(size + 8);
    ret = blk;
    if (blk != 0)
    {
        blk[0] = idx;
        if (idx == 4)
        {
            blk[1] = size;
            return blk + 2;
        }
        blk[1] = *(u32 *)((u8 *)g_rdp_pools_guess + 0x4 + idx * 0x3C);
        ret = blk + 2;
    }
    return ret;
}

// FUNCTION: EQMAIN 0x10028920
// Gets one block of a size class for fast_malloc; an empty class is refilled from a bigger one or new memory
// Port of trilogy allocate_memory_from_pool_guess 0x004F4930 (BinDiff sibling 0.912) with eqmain's pools: each class
// keeps its block size at +0x00, its count at +0x08, its free list at +0x0C and its lock at +0x24.
// __cdecl. Takes a free block of class `poolIndex` (0..3) and returns it. When the class is empty, class 3 carves a new
// chunk into blocks of its own size onto its free list; any other class takes a block of the class above (its lock
// at +0x60 and count at +0x44 from this class), puts the second half on its own free list (the counts move by +2
// here and -1 there). A 0 from the class above is not checked: block_size itself goes on the list.
void *allocate_memory_from_pool_guess(int poolIndex)
{
    LPCRITICAL_SECTION lpCriticalSection;
    RdpList_guess *freeList;
    void *ptr;
    void *chunk;
    DWORD *countPtr;
    int poolOffset;
    DWORD chunkSize;
    DWORD blockOffset;
    DWORD acc;

    poolOffset = poolIndex * 0x3c;
    lpCriticalSection = (LPCRITICAL_SECTION)(g_rdp_pools_guess + poolOffset + 0x24);
    EnterCriticalSection(lpCriticalSection);
    freeList = (RdpList_guess *)(g_rdp_pools_guess + poolOffset + 0x0C);
    ptr = EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))(freeList);
    if (ptr == 0)
    {
        if (poolIndex + 1 == 4)
        {
            chunkSize = ((DWORD)(*(DWORD *)(g_rdp_pools_guess + poolOffset) % 0x248 != 0) +
                     *(DWORD *)(g_rdp_pools_guess + poolOffset) / 0x248) * 0x248;
            chunk = EQ_CDECL(void *, 0x1003342E, (DWORD))(chunkSize + 8);
            EQ_CDECL(void, 0x1002DF20, (RdpList_guess *, int))(&g_rdp_pool_chunks_guess, (int)chunk);
            blockOffset = 0;
            if (chunkSize != 0)
            {
                do
                {
                    EQ_CDECL(void, 0x1002DF20, (RdpList_guess *, int))(&g_rdp_pool3_list_guess, blockOffset + 8 + (int)chunk);
                    blockOffset = blockOffset + 0x248;
                    g_rdp_pool3_count_guess = g_rdp_pool3_count_guess + 1;
                } while (blockOffset < chunkSize);
            }
            acc = 0;
            countPtr = (DWORD *)(g_rdp_pools_guess + 0x08);
            do
            {
                acc = (acc >> 1) + *countPtr;
                countPtr = countPtr + 0xf;
            } while (countPtr < (DWORD *)(g_rdp_pools_guess + 0xF8));
            ptr = EQ_CDECL(void *, 0x1002E130, (RdpList_guess *))(freeList);
            LeaveCriticalSection(lpCriticalSection);
            return ptr;
        }
        ptr = allocate_memory_from_pool_guess(poolIndex + 1);
        EnterCriticalSection((LPCRITICAL_SECTION)(g_rdp_pools_guess + poolOffset + 0x60));
        *(DWORD *)(g_rdp_pools_guess + poolOffset + 0x44) = *(DWORD *)(g_rdp_pools_guess + poolOffset + 0x44) - 1;
        LeaveCriticalSection((LPCRITICAL_SECTION)(g_rdp_pools_guess + poolOffset + 0x60));
        *(DWORD *)(g_rdp_pools_guess + poolOffset + 0x08) = *(DWORD *)(g_rdp_pools_guess + poolOffset + 0x08) + 2;
        EQ_CDECL(void, 0x1002DED0, (RdpList_guess *, int))(freeList, *(int *)(g_rdp_pools_guess + poolOffset) + (int)ptr);
    }
    LeaveCriticalSection(lpCriticalSection);
    return ptr;
}

// FUNCTION: EQMAIN 0x10030050
// Writes queued connection data and tracks pending completion.
// @early-stop: item and queue registers stay reversed; the zero return is rematerialized in EAX.
// Port of trilogy writeFileWithOverlapped_guess 0x004F6D30 (BinDiff sibling 0.991) with eqmain's offsets, globals and
// callees: the queued item keeps its OVERLAPPED at +0x08 (hEvent +0x18), its byte count at +0x1C, its time at +0x20
// and its data at +0x24; the queue keeps its handle at +0x1C, its list at +0x38 and its queued bytes at +0x50.
// __cdecl. Starts an overlapped WriteFile of item `param_2` to the handle at writeQueueAddress+0x1C. Returns 0 when it
// completed (the item is closed and freed) or is pending (the item is queued with its time); 5 for error 0x6F8 or 8
// (closed and freed); 1 for any other error (the handle becomes INVALID_HANDLE_VALUE; closed and freed).
DWORD writeFileWithOverlapped_guess(int writeQueueAddress, DWORD item)
{
    typedef struct WriteItem_guess
    {
        BYTE unknown00[8];
        OVERLAPPED overlapped;
        DWORD byteCount;
        DWORD time;
        BYTE data[1];
    } WriteItem_guess;
    typedef BOOL (__stdcall *WriteFileFn_guess)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
    typedef BOOL (__stdcall *CloseHandleFn_guess)(HANDLE);
    typedef DWORD (__stdcall *GetLastErrorFn_guess)(void);
    WriteItem_guess *DVar1;
    DWORD bytesWritten;
    BOOL BVar2;
    DWORD DVar3;
    DWORD status;
    BYTE *queue = (BYTE *)writeQueueAddress;

    DVar1 = (WriteItem_guess *)item;
    BVar2 = EQ_GLOBAL(WriteFileFn_guess, 0x10040108)(
        *(HANDLE *)(queue + 0x1C),
        (LPCVOID)DVar1->data,
        DVar1->byteCount,
        &bytesWritten,
        &DVar1->overlapped);
    if (BVar2 != 0)
    {
        if (bytesWritten == DVar1->byteCount)
            *(int *)(queue + 0x80) = *(int *)(queue + 0x80) + 1;
        else
            *(int *)(queue + 0x8C) = *(int *)(queue + 0x8C) + 1;
        EQ_GLOBAL(CloseHandleFn_guess, 0x100400C0)(DVar1->overlapped.hEvent);
        EQ_CDECL(void, 0x10028A90, (void *))((void *)DVar1);
        return 0;
    }
    DVar3 = EQ_GLOBAL(GetLastErrorFn_guess, 0x10040094)();
    if (DVar3 == 0x3E5)
    {
        void *list;

        DVar3 = EQ_STDCALL(DWORD, 0x1002F9F0, (void))();
        DVar1->time = DVar3;
        list = (void *)(queue + 0x38);
        *(int *)((BYTE *)list + 0x18) += (int)DVar1->byteCount;
        EQ_CDECL(void, 0x1002DF20, (void *, void *))(list, (void *)DVar1);
        return 0;
    }
    if ((DVar3 == 0x6F8) || (DVar3 == 8))
    {
        *(int *)(queue + 0x90) = *(int *)(queue + 0x90) + 1;
        status = 5;
    }
    else
    {
        int errorCount = *(int *)(queue + 0x84);

        *(DWORD *)(queue + 0x88) = DVar3;
        errorCount++;
        *(DWORD *)(queue + 0x1C) = 0xFFFFFFFF;
        *(int *)(queue + 0x84) = errorCount;
        status = 1;
    }
    EQ_GLOBAL(CloseHandleFn_guess, 0x100400C0)(DVar1->overlapped.hEvent);
    EQ_CDECL(void, 0x10028A90, (void *))((void *)DVar1);
    return status;
}

// g_last_send_tick_guess: the send tick, stamped with timeGetTime() on every call (this image's address).
#define g_last_send_tick_guess      EQ_GLOBAL(u32, 0x1013C9EC)
#define g_send_packet_tag_0_guess   EQ_GLOBAL(DWORD, 0x10041F94)
#define g_send_packet_tag_1_guess   EQ_GLOBAL(DWORD, 0x10041F98)
#define g_send_packet_tag_2_guess   EQ_GLOBAL(DWORD, 0x10041F9C)
#define g_send_packet_tag_3_guess   EQ_GLOBAL(DWORD, 0x10041FA0)
#define g_send_packet_tag_4_guess   (*(WORD *)(EQ_ARRAY(DWORD, 0x10041FA0) + 1))

// FUNCTION: EQMAIN 0x10030140
// Queues one outgoing network packet: reaps finished writes, copies the segments behind a checksummed header
// __cdecl. Under the queue's critical section: stamps the send tick, reaps the queued writes that finished (each is
// closed and freed; ERROR_IO_INCOMPLETE stops the reap), then builds an item (OVERLAPPED at +0x08, byte count at +0x1C,
// data at +0x24: a 0x1A-byte header, the segments' bytes and a one's-complement checksum) and queues it through
// writeFileWithOverlapped_guess. Returns that result; 2 when the item can't be allocated and 3 when its event can't be
// created (the item is freed). Both leave the critical section.
// @early-stop: similarity 0.4384; queue registers and checksum layout still differ after 8 iterations without a gain.
DWORD SendPacketWithCRC_guess(LPCRITICAL_SECTION queue, int segments, DWORD segmentCount, int obj)
{
    typedef void (__stdcall *EnterCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    typedef void (__stdcall *LeaveCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    typedef BOOL (__stdcall *GetOverlappedResultFn_guess)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
    typedef DWORD (__stdcall *GetLastErrorFn_guess)(void);
    typedef BOOL (__stdcall *CloseHandleFn_guess)(HANDLE);
    typedef HANDLE (__stdcall *CreateEventAFn_guess)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR);
    LONG *pLVar1;
    int tmp;
    BOOL BVar3;
    int popped;
    int *item;
    HANDLE pvVar6;
    DWORD uVar8;
    WORD tmpWord;
    DWORD value;
    DWORD sum;
    WORD *wordPtr;
    DWORD *segmentPtr;
    DWORD *src;
    DWORD *dest;
    DWORD DVar13;
    DWORD byteCount;
    CloseHandleFn_guess closeHandle;

    EQ_GLOBAL(EnterCriticalSectionFn_guess, 0x1004014C)(queue);
    g_last_send_tick_guess = EQ_STDCALL(DWORD, 0x1002F9F0, (void))();
    pLVar1 = &queue[2].RecursionCount;
    tmp = EQ_CDECL(int, 0x1002E120, (LONG *))(pLVar1);
    if (tmp != 0)
    {
        closeHandle = EQ_GLOBAL(CloseHandleFn_guess, 0x100400C0);
        do
        {
            BVar3 = EQ_GLOBAL(GetOverlappedResultFn_guess, 0x10040104)(
                (HANDLE)queue[1].LockCount, (LPOVERLAPPED)(tmp + 8), &byteCount, 0);
            if (BVar3 != 0)
            {
                if (byteCount == *(DWORD *)(tmp + 0x1c))
                    queue[5].LockCount = queue[5].LockCount + 1;
                else
                    queue[5].DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)((int)queue[5].DebugInfo + 1);
                popped = EQ_CDECL(int, 0x1002E130, (LONG *))(pLVar1);
                pLVar1[6] = pLVar1[6] - *(int *)(popped + 0x1c);
                pvVar6 = *(HANDLE *)(tmp + 0x18);
            }
            else
            {
                DVar13 = EQ_GLOBAL(GetLastErrorFn_guess, 0x10040094)();
                if (DVar13 == 0x3e4)
                    break;
                queue[5].OwningThread = (HANDLE)((int)queue[5].OwningThread + 1);
                popped = EQ_CDECL(int, 0x1002E130, (LONG *))(pLVar1);
                pLVar1[6] = pLVar1[6] - *(int *)(popped + 0x1c);
                pvVar6 = *(HANDLE *)(tmp + 0x18);
            }
            closeHandle(pvVar6);
            EQ_CDECL(void, 0x10028A90, (void *))((void *)tmp);
            tmp = EQ_CDECL(int, 0x1002E120, (LONG *))(pLVar1);
        }
        while (tmp != 0);
    }
    DVar13 = 0;
    byteCount = DVar13;
    if (segmentCount != 0)
    {
        item = (int *)(segments + 4);
        value = segmentCount;
        do
        {
            tmp = *item;
            item = item + 2;
            DVar13 = DVar13 + tmp;
            value = value - 1;
        } while (value != 0);
        byteCount = DVar13;
    }
    item = EQ_CDECL(int *, 0x100288B0, (int *))((int *)(DVar13 + 0x3e));
    if (item == (int *)0x0)
    {
        uVar8 = 2;
        EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x10040148)(queue);
        return uVar8;
    }
    item[2] = 0;
    item[3] = 0;
    item[4] = 0;
    item[5] = 0;
    item[6] = 0;
    pvVar6 = EQ_GLOBAL(CreateEventAFn_guess, 0x1004010C)((LPSECURITY_ATTRIBUTES)0x0, 1, 0, (LPCSTR)0x0);
    item[6] = (int)pvVar6;
    if (pvVar6 == (HANDLE)0x0)
    {
        uVar8 = 3;
        EQ_CDECL(void, 0x10028A90, (void *))((void *)item);
        EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x10040148)(queue);
        return uVar8;
    }
    item[9] = g_send_packet_tag_0_guess;
    item[10] = g_send_packet_tag_1_guess;
    item[0xb] = g_send_packet_tag_2_guess;
    item[0xc] = g_send_packet_tag_3_guess;
    *(WORD *)(item + 0xd) = g_send_packet_tag_4_guess;
    wordPtr = (WORD *)((BYTE *)item + 0x36);
    tmp = 0x1a;
    *wordPtr = (WORD)queue[1].RecursionCount;
    *(WORD *)(item + 0xe) = *(WORD *)(obj + 2);
    *(WORD *)((BYTE *)item + 0x3a) = (WORD)DVar13;
    *(WORD *)(item + 0xf) = 0;
    if (segmentCount != 0)
    {
        segmentPtr = (DWORD *)(segments + 4);
        do
        {
            value = *segmentPtr;
            src = (DWORD *)segmentPtr[-1];
            dest = (DWORD *)((BYTE *)item + tmp + 0x24);
            memcpy(dest, src, value);
            tmp = tmp + *segmentPtr;
            segmentCount = segmentCount - 1;
            segmentPtr = segmentPtr + 2;
        } while (segmentCount != 0);
        DVar13 = byteCount;
    }
    value = DVar13 + 8;
    sum = 0;
    tmpWord = (WORD)value;
    if (1 < tmpWord)
    {
        segmentCount = (value & 0xffff) >> 1;
        do
        {
            value = value + 0xfffe;
            tmpWord = *wordPtr;
            wordPtr = wordPtr + 1;
            sum = sum + tmpWord;
            segmentCount = segmentCount - 1;
        } while (segmentCount != 0);
        tmpWord = (WORD)value;
    }
    if (tmpWord == 1)
    {
        sum = sum + *(BYTE *)wordPtr;
    }
    tmp = (sum >> 0x10) + (sum & 0xffff);
    *(WORD *)(item + 0xf) = (WORD)~(((DWORD)tmp >> 0x10) + tmp);
    item[7] = DVar13 + 0x1a;
    uVar8 = EQ_CDECL(DWORD, 0x10030050, (int, DWORD))((int)queue, (DWORD)item);
    EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x10040148)(queue);
    return uVar8;
}
