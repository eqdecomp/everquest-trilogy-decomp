/* fast_malloc.c - the rdp layer's pool allocator: four size classes (0x48, 0x90, 0x120, 0x248 byte blocks)
 * carved from chunks that are only freed at shutdown; bigger requests go to malloc. A class that runs dry
 * splits a block of the next larger class; the largest class grows by one block.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/fast_malloc.c (from Mac debug map)
 *
 * C. The rdp code was built for speed (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x006F1320: the four size classes. 0x006F1400: the chunk list (every malloc'd chunk, freed at cleanup).
 * 0x006EFECC: the size the pool was initialised with. */
#define g_rdp_pools_guess       EQ_ARRAY(RdpPool_guess, 0x006F1320)
#define g_rdp_pool_chunks_guess EQ_GLOBAL(RdpList_guess, 0x006F1400)
#define g_rdp_pool_size_guess   EQ_GLOBAL(u32, 0x006EFECC)
#define g_rdp_pool_locks_guess  EQ_ARRAY(RdpPoolLock_guess, 0x006F1340)

// FUNCTION: TRILOGY 0x004f4740
// Prepare the RDP memory pools for network allocations at client startup.
// ORIGINAL FILE: Network/rdp/fast_malloc.c (Mac: fast_malloc_init, high)
// __cdecl. Sets up the four classes (largest 0x248, each next half of it rounded down to 8) and fills the
// largest one with ceil(size / 0x248) blocks from one chunk (16-byte chunk header).
void fast_malloc_init(u32 size)
{
    RdpPool_guess *pool;
    u32 block;
    u32 total;
    u32 *stat;
    u32 n;
    u32 bytes;
    u8 *chunk;
    u8 *p;

    g_rdp_pool_size_guess = size;
    EQ_CDECL(void, 0x004F5440, (RdpList_guess *))(&g_rdp_pool_chunks_guess);
    EQ_CDECL(void, 0x004F5460, (RdpList_guess *, int, RdpListCompare_guess))(
        &g_rdp_pool_chunks_guess, 0, 0);
    block = 0x248;
    pool = &g_rdp_pools_guess[4];
    do
    {
        pool--;
        EQ_CDECL(void, 0x004F5440, (RdpList_guess *))(&pool->list);
        EQ_CDECL(void, 0x004F5460, (RdpList_guess *, int, RdpListCompare_guess))(
            &pool->list, 0, 0);
        InitializeCriticalSection((CRITICAL_SECTION *)pool->lock);
        pool->block_size = block;
        pool->payload = (block - 8) & 0xfffffff8;
        block = (block >> 1) & 0x7ffffff8;
    } while (&pool->list != &g_rdp_pools_guess[0].list);
    bytes = ((size % 0x248 != 0) + size / 0x248) * 0x248;
    chunk = (u8 *)malloc(bytes + 0x10);
    *(void **)(chunk + 8) = chunk;
    EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))(
        &g_rdp_pool_chunks_guess, (RdpLink_guess *)chunk);
    p = chunk + 0x10;
    if (bytes > 0)
    {
        n = (bytes + 0x247) / 0x248;
        do
        {
            *(void **)(p + 8) = p;
            EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))(
                &g_rdp_pools_guess[3].list, (RdpLink_guess *)p);
            g_rdp_pools_guess[3].count++;
            p += 0x248;
            n--;
        } while (n != 0);
    }
    total = 0;
    stat = &g_rdp_pools_guess[0].count;
    do
    {
        total = (total >> 1) + *stat;
        stat += 14;
    } while (stat < (u32 *)((u8 *)g_rdp_pools_guess + 0xe8));
}

/* fast_malloc.c: a class's lock as cleanupMemoryPool walks them, 0x38 apart. The lock is the last 0x18 bytes of
 * RdpPool_guess, so from a lock on come the lock and then the next class's header; the count of class k is the
 * after_count of the element before its lock (class 0's is at 0x006F1328). */
typedef struct RdpPoolLock_guess
{
    CRITICAL_SECTION lock;          /* 0x00  RdpPool_guess.lock of this class */
    u32 after_block_size;           /* 0x18  the next class's block_size */
    u32 after_payload;              /* 0x1C  the next class's payload */
    u32 after_count;                /* 0x20  the next class's count */
    RdpList_guess after_list;       /* 0x24  the next class's list */
} RdpPoolLock_guess;
C_ASSERT(sizeof(RdpPoolLock_guess) == 0x38);
C_ASSERT(offsetof(RdpPoolLock_guess, after_count) == 0x20);

// FUNCTION: TRILOGY 0x004f4860
// ORIGINAL FILE: Network/rdp/fast_malloc.c (address: between Mac-mapped neighbours)
// __cdecl. Frees every chunk, deletes the four locks, forgets the initial size.
void cleanupMemoryPool_guess(void)
{
    void *chunk;
    RdpPoolLock_guess *lock;
    u32 total; /* fake match: the original also folds the four class counts into a running total while it
                  deletes the locks (edi in its asm) and never reads it. No effect; kept because the original
                  computes it. */

    total = 0;
    chunk = EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(&g_rdp_pool_chunks_guess);
    while (chunk != 0)
    {
        free(chunk);
        chunk = EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(&g_rdp_pool_chunks_guess);
    }
    lock = g_rdp_pool_locks_guess;
    do
    {
        /* lock is a class's lock; the class's count is in the element before it (0x6F1340 - 0x38 + 0x20) */
        total = (total >> 1) + lock[-1].after_count;
        DeleteCriticalSection(&lock->lock);
        lock++;
    } while (lock < EQ_ARRAY(RdpPoolLock_guess, 0x006F1420));
    g_rdp_pool_size_guess = 0;
}

// FUNCTION: TRILOGY 0x004f48c0
// Provides buffers for RDP network message handling.
// ORIGINAL FILE: Network/rdp/fast_malloc.c (Mac: fast_malloc, high)
// __cdecl. A block of at least `size` bytes: from the smallest class that fits, else malloc(size + 8). The
// 8-byte header holds the class index (4 = malloc'd) and the payload size (the request for malloc'd ones);
// returns the bytes after it, NULL when malloc fails.
void *fast_malloc(u32 size)
{
    int idx;
    u32 *payload;
    u32 *blk;
    u32 *ret;

    idx = 0;
    payload = &g_rdp_pools_guess[0].payload;
    do
    {
        if (!(size <= *payload))
        {
        }
        else
        {
            blk = (u32 *)allocate_memory_from_pool_guess(idx);
            break;
        }
        payload += 14;
        idx++;
    } while (payload < &g_rdp_pools_guess[4].payload);
    /* running out of classes leaves blk unset, but that is idx 4, and idx 4 always takes this malloc */
    if (idx == 4)
        blk = (u32 *)malloc(size + 8);
    ret = blk;
    if (blk != 0)
    {
        blk[0] = idx;
        if (idx == 4)
        {
            blk[1] = size;
            return blk + 2;
        }
        blk[1] = g_rdp_pools_guess[idx].payload;
        ret = blk + 2;
    }
    return ret;
}

// FUNCTION: TRILOGY 0x004f4930
// Supply a block from a selected RDP memory class, replenishing that class when exhausted.
// ORIGINAL FILE: Network/rdp/fast_malloc.c (address: between Mac-mapped neighbours)
// __cdecl. Takes a free block of class `idx`. When the class is empty: the largest class allocates a new
// chunk of one block; the others split a block of the next larger class (the second half goes on this
// class's free list, the counters move by +2 here and -1 there).
void *allocate_memory_from_pool_guess(int idx)
{
    RdpPool_guess *pool;
    void *blk;
    u32 bytes;
    u32 n;
    u8 *chunk;
    u8 *p;
    u32 total;
    u32 *stat;

    pool = &g_rdp_pools_guess[idx];
    EnterCriticalSection((CRITICAL_SECTION *)pool->lock);
    blk = EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(&pool->list);
    if (blk == 0)
    {
        if (idx + 1 == 4)
        {
            bytes = ((pool->block_size % 0x248 != 0) + pool->block_size / 0x248) * 0x248;
            chunk = (u8 *)malloc(bytes + 0x10);
            *(void **)(chunk + 8) = chunk;
            EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))(
                &g_rdp_pool_chunks_guess, (RdpLink_guess *)chunk);
            p = chunk + 0x10;
            if (bytes > 0)
            {
                n = (bytes + 0x247) / 0x248;
                do
                {
                    *(void **)(p + 8) = p;
                    EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))(
                        &g_rdp_pools_guess[3].list, (RdpLink_guess *)p);
                    g_rdp_pools_guess[3].count++;
                    p += 0x248;
                    n--;
                } while (n != 0);
            }
            total = 0;
            stat = &g_rdp_pools_guess[0].count;
            do
            {
                total = (total >> 1) + *stat;
                stat += 14;
            } while (stat < (u32 *)((u8 *)g_rdp_pools_guess + 0xe8));
            blk = EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(&pool->list);
        }
        else
        {
            blk = allocate_memory_from_pool_guess(idx + 1);   // self-recursion: by name (the harness runs it natively)
            EnterCriticalSection((CRITICAL_SECTION *)g_rdp_pools_guess[idx + 1].lock);
            g_rdp_pools_guess[idx + 1].count--;
            LeaveCriticalSection((CRITICAL_SECTION *)g_rdp_pools_guess[idx + 1].lock);
            pool->count += 2;
            p = (u8 *)blk + pool->block_size;
            *(void **)(p + 8) = p;
            EQ_CDECL(void, 0x004F5480, (RdpList_guess *, RdpLink_guess *))(
                &pool->list, (RdpLink_guess *)p);
        }
    }
    LeaveCriticalSection((CRITICAL_SECTION *)pool->lock);
    return blk;
}

// FUNCTION: TRILOGY 0x004f4ac0
// Releases buffers after RDP network message handling.
// ORIGINAL FILE: Network/rdp/fast_malloc.c (Mac: fast_free, high)
// __cdecl. Frees a block of fast_malloc: class 4 goes to free, the others onto their class's free list
// (head). The counter is not touched.
/* fake match: the lock is indexed as its own 0x38-stride array, so idx*0x38 is computed once. */
void fast_free_guess(void *allocation)
{
    u32 *blk;
    int idx;

    blk = (u32 *)allocation - 2;
    idx = blk[0];
    if (idx == 4)
    {
        free(blk);
        return;
    }
    ((u32 **)blk)[2] = blk;
    EnterCriticalSection(&g_rdp_pool_locks_guess[idx].lock);
    EQ_CDECL(void, 0x004F5480, (RdpList_guess *, RdpLink_guess *))(
        &g_rdp_pools_guess[idx].list, (RdpLink_guess *)blk);
    LeaveCriticalSection(&g_rdp_pool_locks_guess[idx].lock);
}

// FUNCTION: TRILOGY 0x004f6940
// Wait for outstanding writes and release their buffers and the output context's resources during cleanup.
// @early-stop: base/list/item registers are swapped and the original's CloseHandle/free tails remain duplicated.
/* fake match: access the queue byte count through its head pointer to match the original +0x14 field access. */
void cleanup_io_completion_queue_guess(LPCRITICAL_SECTION value)
{
    typedef void (__stdcall *EnterCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    typedef BOOL (__stdcall *GetOverlappedResultFn_guess)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
    typedef DWORD (__stdcall *GetLastErrorFn_guess)(void);
    typedef BOOL (__stdcall *CloseHandleFn_guess)(HANDLE);
    typedef void (__stdcall *LeaveCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    typedef void (__stdcall *DeleteCriticalSectionFn_guess)(LPCRITICAL_SECTION);
    LONG *pLVar1;
    BYTE *mem;
    LPCRITICAL_SECTION lpCriticalSection;
    BOOL BVar3;
    int removedEntry;
    DWORD DVar5;
    CloseHandleFn_guess closeHandle;

    lpCriticalSection = value;
    EQ_GLOBAL(EnterCriticalSectionFn_guess, 0x00534194)(lpCriticalSection);
    EQ_GLOBAL(u32, 0x006EFED4) =
        EQ_CDECL(DWORD, 0x004F66D0, (void))();
    closeHandle = EQ_GLOBAL(CloseHandleFn_guess, 0x005341D4);
    pLVar1 = &lpCriticalSection[2].RecursionCount;
    while (*pLVar1 != 0)
    {
        mem = *(BYTE **)(*pLVar1 + 8);
        if (mem == 0)
            break;
        BVar3 = EQ_GLOBAL(GetOverlappedResultFn_guess, 0x005340C0)(
            *(HANDLE *)((BYTE *)lpCriticalSection + 0x1C),
            (LPOVERLAPPED)(mem + 0x10),
            (LPDWORD)&value,
            1);
        if (BVar3 != 0)
        {
            if (value == *(LPCRITICAL_SECTION *)(mem + 0x24))
                *(LONG *)((BYTE *)lpCriticalSection + 0x78) =
                    *(LONG *)((BYTE *)lpCriticalSection + 0x78) + 1;
            else
                *(LONG *)((BYTE *)lpCriticalSection + 0x74) =
                    *(LONG *)((BYTE *)lpCriticalSection + 0x74) + 1;
            removedEntry = (int)EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(
                (RdpList_guess *)pLVar1);
            pLVar1[5] = pLVar1[5] -
                *(int *)(removedEntry + 0x24);
            closeHandle(*(HANDLE *)(mem + 0x20));
            EQ_CDECL(void, 0x004F4AC0, (void *))(mem);
            continue;
        }
        else
        {
            DVar5 = EQ_GLOBAL(GetLastErrorFn_guess, 0x005341AC)();
            if (DVar5 == 0x3E4)
                break;
            lpCriticalSection[5].RecursionCount = lpCriticalSection[5].RecursionCount + 1;
            removedEntry = (int)EQ_CDECL(void *, 0x004F5500, (RdpList_guess *))(
                (RdpList_guess *)pLVar1);
            *(LONG *)((BYTE *)lpCriticalSection + 0x4C) =
                *(LONG *)((BYTE *)lpCriticalSection + 0x4C) -
                *(int *)(removedEntry + 0x24);
            closeHandle(*(HANDLE *)(mem + 0x20));
            EQ_CDECL(void, 0x004F4AC0, (void *))(mem);
            continue;
        }
    }
    EQ_CDECL(void, 0x004F8030, (void *))((BYTE *)lpCriticalSection + 0x24);
    EQ_CDECL(void, 0x0051CF90, (void *))(pLVar1);
    EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x00534190)(lpCriticalSection);
    EQ_GLOBAL(DeleteCriticalSectionFn_guess, 0x005341C8)(lpCriticalSection);
    return;
}

// FUNCTION: TRILOGY 0x004F6D30
// Submit a prepared output buffer and track any write that remains pending.
// @early-stop: original keeps a zero in EBX across WriteFile that VC6 folds away here; item and state registers are swapped
DWORD writeFileWithOverlapped_guess(int senderContext, DWORD value)
{
    typedef BOOL (__stdcall *WriteFileFn_guess)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
    typedef BOOL (__stdcall *CloseHandleFn_guess)(HANDLE);
    typedef DWORD (__stdcall *GetLastErrorFn_guess)(void);
    DWORD DVar1;
    BOOL BVar2;
    DWORD DVar3;
    DWORD result;
    RdpList_guess *list;

    DVar1 = value;
    BVar2 = EQ_GLOBAL(WriteFileFn_guess, 0x005340BC)(
        *(HANDLE *)(senderContext + 0x1C),
        (LPCVOID)(DVar1 + 0x2C),
        *(DWORD *)(DVar1 + 0x24),
        &value,
        (LPOVERLAPPED)(DVar1 + 0x10));
    if (BVar2 != 0)
    {
        if (value == *(DWORD *)(DVar1 + 0x24))
            ++*(int *)(senderContext + 0x7C);
        else
            ++*(int *)(senderContext + 0x88);
        EQ_GLOBAL(CloseHandleFn_guess, 0x005341D4)(*(HANDLE *)(DVar1 + 0x20));
        EQ_CDECL(void, 0x004F4AC0, (void *))((void *)DVar1);
        return 0;
    }
    DVar3 = EQ_GLOBAL(GetLastErrorFn_guess, 0x005341AC)();
    if (DVar3 == 0x3E5)
    {
        *(DWORD *)(DVar1 + 8) = DVar1;
        DVar3 = EQ_CDECL(DWORD, 0x004F66D0, (void))();
        *(DWORD *)(DVar1 + 0x28) = DVar3;
        list = (RdpList_guess *)(senderContext + 0x38);
        *(int *)(list + 1) += *(int *)(DVar1 + 0x24);
        EQ_CDECL(void, 0x004F54C0, (RdpList_guess *, RdpLink_guess *))(
            list, (RdpLink_guess *)DVar1);
        return 0;
    }
    if ((DVar3 == 0x6F8) || (DVar3 == 8))
    {
        ++*(int *)(senderContext + 0x8C);
        result = 5;
    }
    else
    {
        *(DWORD *)(senderContext + 0x84) = DVar3;
        *(DWORD *)(senderContext + 0x1C) = 0xFFFFFFFF;
        ++*(int *)(senderContext + 0x80);
        result = 1;
    }
    EQ_GLOBAL(CloseHandleFn_guess, 0x005341D4)(*(HANDLE *)(DVar1 + 0x20));
    EQ_CDECL(void, 0x004F4AC0, (void *))((void *)DVar1);
    return result;
}

typedef struct RdpSender_guess
{
    CRITICAL_SECTION lock;      /* 0x00 */
    u8 unknown18[4];            /* 0x18 */
    HANDLE handle;              /* 0x1C  the socket's file handle */
    u16 id;                     /* 0x20 */
    u8 unknown22[0x16];         /* 0x22 */
    RdpList_guess pending;      /* 0x38  writes in flight */
    int pending_bytes;          /* 0x4C */
    u8 unknown50[0x24];         /* 0x50 */
    int short_writes;           /* 0x74 */
    int full_writes;            /* 0x78 */
    u8 unknown7c[4];            /* 0x7C */
    int errors;                 /* 0x80 */
} RdpSender_guess;

typedef struct RdpSendBuf_guess
{
    RdpLink_guess link;         /* 0x00 */
    OVERLAPPED overlapped;      /* 0x10 */
    u32 length;                 /* 0x24 */
    DWORD queued_time;          /* 0x28 */
    u8 signature[18];           /* 0x2C */
    u16 id;                     /* 0x3E */
    u16 port;                   /* 0x40 */
    u16 payload_length;         /* 0x42 */
    u16 checksum;               /* 0x44 */
    u8 payload[1];              /* 0x46 */
} RdpSendBuf_guess;

#define RDP_SEND_HEADER_guess 0x1A

// FUNCTION: TRILOGY 0x004F6E20
// Reaps finished overlapped writes, then wraps the segments in a checksummed datagram and writes it.
int SendOverlappedPacketWithCRC_guess(RdpSender_guess *sender, void *segments, u32 segmentCount, RdpAddr_guess *dest)
{
    RdpList_guess *pending;
    RdpSendBuf_guess *item;
    RdpSendBuf_guess *buf;
    RdpSendBuf_guess *removed;
    u32 *segment;
    u32 remaining;
    u32 total;
    int offset;
    int result;
    u32 segmentSize;
    u32 words;
    u32 sum;
    u16 *word;
    u32 n;

    EnterCriticalSection(&sender->lock);
    EQ_GLOBAL(DWORD, 0x006EFED4) = EQ_CDECL(DWORD, 0x004F66D0, (void))();
    pending = &sender->pending;
    while (pending->head != 0 && (item = (RdpSendBuf_guess *)pending->head->data) != 0)
    {
        if (!GetOverlappedResult(sender->handle, &item->overlapped, &total, 0))
        {
            if (GetLastError() == 0x3E4)
                break;
            ++sender->errors;
            removed = EQ_CDECL(RdpSendBuf_guess *, 0x004F5500, (RdpList_guess *))(pending);
            sender->pending_bytes -= removed->length;
            CloseHandle(item->overlapped.hEvent);
            EQ_CDECL(void, 0x004F4AC0, (void *))(item);
        }
        else
        {
            if (total == item->length)
                ++sender->full_writes;
            else
                ++sender->short_writes;
            removed = EQ_CDECL(RdpSendBuf_guess *, 0x004F5500, (RdpList_guess *))(pending);
            sender->pending_bytes -= removed->length;
            CloseHandle(item->overlapped.hEvent);
            EQ_CDECL(void, 0x004F4AC0, (void *))(item);
        }
    }
    total = 0;
    if (segmentCount != 0)
    {
        segment = (u32 *)((u8 *)segments + 4);
        remaining = segmentCount;
        do
        {
            total += *segment;
            segment += 2;
            --remaining;
        } while (remaining != 0);
    }
    buf = EQ_CDECL(RdpSendBuf_guess *, 0x004F48C0, (u32))(total + 0x46);
    if (buf == 0)
    {
        LeaveCriticalSection(&sender->lock);
        return 2;
    }
    buf->overlapped.Internal = 0;
    buf->overlapped.InternalHigh = 0;
    buf->overlapped.Offset = 0;
    buf->overlapped.OffsetHigh = 0;
    buf->overlapped.hEvent = 0;
    buf->overlapped.hEvent = CreateEventA(0, 1, 0, 0);
    if (buf->overlapped.hEvent == 0)
    {
        EQ_CDECL(void, 0x004F4AC0, (void *))(buf);
        LeaveCriticalSection(&sender->lock);
        return 3;
    }
    memcpy(buf->signature, "\n\rwho's yo daddy?", 18);
    buf->id = sender->id;
    buf->port = dest->port;
    buf->payload_length = (u16)total;
    buf->checksum = 0;
    offset = RDP_SEND_HEADER_guess;
    if (segmentCount != 0)
    {
        segment = (u32 *)((u8 *)segments + 4);
        do
        {
            segmentSize = *segment;
            memcpy(buf->signature + offset, (void *)segment[-1], segmentSize);
            offset += *segment;
            --segmentCount;
            segment += 2;
        } while (segmentCount != 0);
    }
    n = total + 8;
    sum = 0;
    word = &buf->id;
    if ((u16)n > 1)
    {
        words = (n & 0xFFFF) >> 1;
        do
        {
            n += 0xFFFE;
            sum += *word++;
            --words;
        } while (words != 0);
    }
    if ((u16)n == 1)
        sum += *(u8 *)word;
    sum = (sum >> 16) + (sum & 0xFFFF);
    buf->checksum = (u16)~((sum >> 16) + sum);
    buf->length = total + RDP_SEND_HEADER_guess;
    result = EQ_CDECL(int, 0x004F6D30, (RdpSender_guess *, RdpSendBuf_guess *))(sender, buf);
    LeaveCriticalSection(&sender->lock);
    return result;
}
