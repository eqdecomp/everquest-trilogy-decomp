/* connhash.c - the connection hash: connections keyed by their remote address, one lock per bucket.
 * ORIGINAL FILE: Source/PC Source/Network/rdp/connhash.c (from Mac debug map: connhash_lock)
 *
 * The Mac map names only connhash_lock (0x004F51C0); the functions from 0x004F5000 up to list.c
 * (0x004F5440) are placed here by address (the queue's graph labels say rdp/list/connection). The
 * semaphore helpers at the end may belong to another small object.
 * C. The rdp code was built for speed with intrinsics (see bitops.c).
 */
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include "trilogy/rdp.h"

#include "trilogy/eqaddr.h"

// CFLAGS: /O2

/* 0x0051CF90: a one-byte `ret` in the client (library code), called with each bucket's list. */
#define rdp_list_release_stub_guess  EQ_CDECL(void, 0x0051CF90, (RdpList_guess *list))

/* compareHashKeys_guess by its client address: the buckets keep the same callback pointer as the original
 * (the hook routes calls through it to the rewrite), so other client code sees identical lists. */
#define compareHashKeys_va_guess  ((RdpListCompare_guess)0x004F5020)

u16 calculateHashIndex_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr);

// FUNCTION: TRILOGY 0x004f5000
// ORIGINAL FILE: Network/rdp/connhash.c (address; uncertain)
// __cdecl. Zeroes a 16-bit counter and the dword at +4.
void initializeCounter_guess(void *p)
{
    *(u16 *)p = 0;
    *(u32 *)((u8 *)p + 4) = 0;
}

// FUNCTION: TRILOGY 0x004f5020
// Orders peer addresses so the RDP connection table can match endpoints.
// ORIGINAL FILE: Network/rdp/connhash.c (address: before connhash_lock)
// __cdecl. List compare for the buckets: by family first (1 when a's is greater, else -1), then the
// family's address bytes (AF_INET 8, AF_IPX 14, family 0x45 4) as memcmp -1/0/1. Faithful bug: any other
// family returns `a` itself (an uninitialised result in the original that lives in a's slot).
// @early-stop: default return is emitted before the compare blocks, changing the final dispatch branch and shared epilogue.
int compareHashKeys_guess(RdpAddr_guess *a, RdpAddr_guess *b)
{
    u16 family = a->family;
    int result;

    if (b->family == family)
    {
        switch (family)
        {
        case 2:
            result = memcmp(a, b, 8);
            break;
        case 6:
            result = memcmp(a, b, 14);
            break;
        case 0x45:
            result = memcmp(a, b, 4);
            break;
        default:
            result = (int)a;
            break;
        }
    }
    else
        return b->family < family ? 1 : -1;
    return result;
}

// FUNCTION: TRILOGY 0x004f50a0
// Prepares the address index used to track RDP peer connections.
// ORIGINAL FILE: Network/rdp/connhash.c (address: before connhash_lock)
// __cdecl. 2^(bits-1) buckets, zeroed, each with an empty sorted list and an initialised lock. Returns 1
// when malloc fails, else 0.
int HashTable_initializeHashTable_guess(RdpConnHash_guess *hash, int bits)
{
    u32 i;

    hash->size = (u16)(1 << (bits - 1));
    hash->buckets = (RdpHashBucket_guess *)malloc(hash->size * sizeof(RdpHashBucket_guess));
    memset(hash->buckets, 0, hash->size * sizeof(RdpHashBucket_guess));
    if (hash->buckets)
    {
        for (i = 0; i < hash->size; i++)
        {
            list_init_guess(&hash->buckets[i].list);
            setLinkedListProperties_guess(&hash->buckets[i].list, 1, compareHashKeys_va_guess);
            InitializeCriticalSection((CRITICAL_SECTION *)hash->buckets[i].lock);
        }
    }
    return hash->buckets == 0;
}

// FUNCTION: TRILOGY 0x004f5160
// Releases the address index used to track RDP peer connections.
// ORIGINAL FILE: Network/rdp/connhash.c (address: before connhash_lock)
// __cdecl. Releases every bucket (list release = the client's empty stub at 0x0051CF90, then the lock)
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

// FUNCTION: TRILOGY 0x004f51c0
// Finds and locks the live RDP connection associated with a remote endpoint.
// ORIGINAL FILE: Network/rdp/connhash.c (Mac: connhash_lock, high)
// __cdecl. Looks up the connection for `addr`; if found, takes a reference (under the bucket lock) and
// then the connection's own lock. Returns it, or NULL.
RdpConnection_guess *connhash_lock_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr)
{
    u16 h;
    RdpList_guess *list;
    RdpLink_guess *link;
    RdpConnection_guess *conn;

    h = calculateHashIndex_guess(hash, addr);
    EnterCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
    list = &hash->buckets[h].list;
    link = list_find_link_by_key_guess(list, addr);
    conn = link ? (RdpConnection_guess *)link->data : 0;
    if (conn)
        conn->refcount++;
    LeaveCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
    if (conn)
        EnterCriticalSection((CRITICAL_SECTION *)conn->lock);
    return conn;
}

// FUNCTION: TRILOGY 0x004f5240
// Selects the address index bucket for an RDP peer connection.
// ORIGINAL FILE: Network/rdp/connhash.c (address: after connhash_lock)
// __cdecl. Bucket of an address: AF_INET mixes the address and port, AF_IPX sums the signed bytes 2..11
// plus the socket; other families hash to 0. 16-bit result, masked by size - 1.
// @early-stop: default `return 0` is xor ax,ax (u16 return); AF_INET port/low-word XORs load into regs, not xor ax,[mem]
u16 calculateHashIndex_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr)
{
    const char *b;

    switch (addr->family)
    {
    case 2:
        return (u16)((addr->addr >> 24) ^ (addr->addr >> 12) ^ addr->port ^ *(u16 *)&addr->addr)
               & (u16)(hash->size - 1);
    case 6:
        b = (const char *)addr;
        return (u16)((short)(b[11] + b[10] + b[9] + b[8] + b[7] + b[6] + b[5] + b[4] + b[3] + b[2]
                             + *(u16 *)(b + 12))
                     & (u16)(hash->size - 1));
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004f52d0
// ORIGINAL FILE: Network/rdp/connhash.c (address: after connhash_lock)
// __cdecl. Inserts `conn` into its bucket (sorted by address) with a reference count of 1.
void addConnectionToHashTable_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn)
{
    u16 h;

    h = calculateHashIndex_guess(hash, &conn->remote);
    EnterCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
    conn->refcount = 1;
    LinkedList_insertNodeIntoSortedLinkedList_guess(&hash->buckets[h].list, &conn->hash_link);
    LeaveCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
}

// FUNCTION: TRILOGY 0x004f5330
// Stops tracking a peer connection once it is no longer in use.
// ORIGINAL FILE: Network/rdp/connhash.c (address: after connhash_lock)
// __cdecl. Drops a reference; at zero, unlinks the connection from its bucket and returns it (NULL
// otherwise, or when it isn't in the bucket).
RdpConnection_guess *removeConnectionFromHashTable_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn)
{
    RdpConnection_guess *removed;
    u16 h;
    RdpList_guess *list;
    RdpLink_guess *link;

    removed = 0;
    h = calculateHashIndex_guess(hash, &conn->remote);
    EnterCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
    if (--conn->refcount == 0)
    {
        list = &hash->buckets[h].list;
        link = list_find_link_by_key_guess(list, &conn->remote);
        if (link)
            removed = (RdpConnection_guess *)removeNodeFromLinkedList_guess(list, link);
        else
            removed = 0;
    }
    LeaveCriticalSection((CRITICAL_SECTION *)hash->buckets[h].lock);
    return removed;
}

// FUNCTION: TRILOGY 0x004f53c0
// ORIGINAL FILE: Network/rdp/connhash.c (address; uncertain, may be its own object)
// __cdecl. An unnamed semaphore, count 0, max 0x7FFFFFFF. 0, or 3 when CreateSemaphore fails.
int createSemaphore_guess(HANDLE *sem)
{
    int rc = 0;

    *sem = CreateSemaphoreA(0, 0, 0x7fffffff, 0);
    if (*sem == 0)
        rc = 3;
    return rc;
}

// FUNCTION: TRILOGY 0x004f53f0
// ORIGINAL FILE: Network/rdp/connhash.c (address; uncertain)
// __cdecl. Waits up to `timeout` ms (not alertable); 1 when signalled, else 0.
int wait_for_event_with_timeout_guess(HANDLE *sem, DWORD timeout)
{
    int rc = 0;

    if (WaitForSingleObjectEx(*sem, timeout, FALSE) == WAIT_OBJECT_0)
        rc = 1;
    return rc;
}

// FUNCTION: TRILOGY 0x004f5420
// ORIGINAL FILE: Network/rdp/connhash.c (address; uncertain)
// __cdecl. Releases the semaphore once.
void signalMessageProcessing_guess(HANDLE *sem)
{
    ReleaseSemaphore(*sem, 1, 0);
}
