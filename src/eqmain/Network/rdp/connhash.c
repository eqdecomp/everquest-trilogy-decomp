// eqmain: the same original object as trilogy's src/Network/rdp/connhash.c (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
/* connhash.c - the connection hash: connections keyed by their remote address, one lock per bucket.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/connhash.c (from Mac debug map: connhash_lock)
 *
 * The Mac map names only connhash_lock (0x1002E6E0); the functions from 0x1002E550 up to list.c
 * (0x004F5440) are placed here by address (the queue's graph labels say rdp/list/connection). The
 * semaphore helpers at the end may belong to another small object.
 * C. The rdp code was built for speed with intrinsics (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "eqmain/rdp.h"

#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x10027940: a one-byte `ret` in the client (library code), called with each bucket's list. */
#define rdp_list_release_stub_guess  EQ_CDECL(void, 0x10027940, (RdpList_guess *list))

/* compareHashKeys_guess by its client address (0x1002E570 here): the buckets
 * keep the same callback pointer as the original (the hook routes calls through it to the rewrite), so other
 * client code sees identical lists. */
#define compareHashKeys_va_guess  ((RdpListCompare_guess)0x1002E570)

/* 0x1003342E: _malloc, cdecl (size) -> void *. */
#define malloc_va_guess  EQ_CDECL(void *, 0x1003342E, (size_t size))

/* 0x1002DEA0 (InitRecord_guess): list.c's init, cdecl (list, link_offset, key_offset, compare). The list is this
 * image's RdpList_guess (0x18 bytes, include/eqmain/rdp.h; trilogy's header says 0x14), so it is passed as void *. */
#define rdp_list_init_keyed_guess  EQ_CDECL(void, 0x1002DEA0, (void *list, u32 link_offset, u32 key_offset, RdpListCompare_guess compare))

u16 calculateHashIndex_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr);

// FUNCTION: EQMAIN 0x1002e550
// Cloned from trilogy initializeCounter_guess 0x004F5000 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Zeroes a 16-bit counter and the dword at +4.
void initializeCounter_guess(void *p)
{
    *(u16 *)p = 0;
    *(u32 *)((u8 *)p + 4) = 0;
}

// FUNCTION: EQMAIN 0x1002e570
// Compares keys used to match entries in the RDP connection table.
// @early-stop: key register/compare lowering and several return-path instructions still differ.
// __cdecl. The hash lists' compare callback (RdpListCompare_guess; HashTable_initializeHashTable_guess passes it by
// address). The key is the u16 at +0 (the family). Keys that differ: 1 when b's key is lower, else -1. Equal keys:
// family 2 compares 8 bytes, 0x45 compares 4 (memcmp: 0, -1 or 1); any other family returns a itself (non-zero).
int compareHashKeys_guess(void *a, void *b)
{
    u16 key;

    key = *(u16 *)a;
    if (key == *(u16 *)b)
    {
        if (key != 2)
        {
            if (key == 0x45)
                return memcmp(a, b, 4);
            return (int)a;
        }
        return memcmp(a, b, 8);
    }
    return (*(u16 *)b < *(u16 *)a) ? 1 : -1;
}

// FUNCTION: EQMAIN 0x1002e5d0
// Prepares the RDP connection hash table to track and find connections.
// Cloned from trilogy HashTable_initializeHashTable_guess 0x004F50A0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. 2^(bits-1) buckets of 0x30 bytes (a lock at +0, the list at +0x18), zeroed, each list set up with
// compareHashKeys and its lock initialised. Returns 1 when malloc fails, else 0.
int HashTable_initializeHashTable_guess(RdpConnHash_guess *hash, int bits)
{
    u32 i;

    hash->size = (u16)(1 << (bits - 1));
    hash->buckets = (RdpHashBucket_guess *)malloc_va_guess(hash->size * 0x30);
    memset(hash->buckets, 0, hash->size * 0x30);
    if (hash->buckets)
    {
        // buckets are indexed in bytes: trilogy's RdpHashBucket_guess is 0x2C, this image's stride is 0x30
        for (i = 0; i < hash->size; i++)
        {
            rdp_list_init_keyed_guess((u8 *)hash->buckets + i * 0x30 + 0x18, 0x10, 0x478, compareHashKeys_va_guess);
            InitializeCriticalSection((CRITICAL_SECTION *)((u8 *)hash->buckets + i * 0x30));
        }
    }
    return hash->buckets == 0;
}

// FUNCTION: EQMAIN 0x1002e680
// Releases the RDP connection hash table when it is no longer needed.
// Cloned from trilogy cleanup_hash_table_guess 0x004F5160 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Releases every bucket (list release = the client's empty stub at 0x10027940, then the lock)
// and frees the buckets. The connections themselves are not touched.
void cleanup_hash_table_guess(RdpConnHash_guess *hash)
{
    u32 i;

    if (hash->buckets)
    {
        for (i = 0; i < hash->size; i++)
        {
            rdp_list_release_stub_guess(&hash->buckets[i].list);
            DeleteCriticalSection((CRITICAL_SECTION *)hash->buckets[i].lock);
        }
        free(hash->buckets);
        hash->buckets = 0;
    }
}

// FUNCTION: EQMAIN 0x1002e8b0
// Creates an RDP synchronization semaphore; what network activity it coordinates is unclear.
// Cloned from trilogy createSemaphore_guess 0x004F53C0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. An unnamed semaphore, count 0, max 0x7FFFFFFF. 0, or 3 when CreateSemaphore fails.
int createSemaphore_guess(HANDLE *sem)
{
    int rc = 0;

    *sem = CreateSemaphoreA(0, 0, 0x7fffffff, 0);
    if (*sem == 0)
        rc = 3;
    return rc;
}

// FUNCTION: EQMAIN 0x1002e8e0
// Waits for an RDP synchronization signal before the network caller continues.
// Cloned from trilogy wait_for_semaphore_with_timeout_guess 0x004F53F0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Waits up to `timeout` ms (not alertable); 1 when signalled, else 0.
int wait_for_semaphore_with_timeout_guess(HANDLE *sem, DWORD timeout)
{
    int rc = 0;

    if (WaitForSingleObjectEx(*sem, timeout, FALSE) == WAIT_OBJECT_0)
        rc = 1;
    return rc;
}

// FUNCTION: EQMAIN 0x1002e910
// Signals an RDP semaphore; the network task it wakes is unclear.
// Cloned from trilogy signalMessageProcessing_guess 0x004F5420 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Releases the semaphore once.
void signalMessageProcessing_guess(HANDLE *sem)
{
    ReleaseSemaphore(*sem, 1, 0);
}
