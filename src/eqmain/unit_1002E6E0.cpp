// unit_1002E6E0.cpp - eqmain.dll, the address run 0x1002E6E0..0x1002E800 (3 functions; this is the first). The
// original is the rdp connection hash lookup: the Mac map names it connhash_lock. eqmain's build inlines the bucket
// hash and lays the structs out differently from rdp.h (buckets 0x30 bytes, refcount +0x18, lock +0x34), so the
// offsets below are raw.

#include "trilogy/rdp.h"
#include "trilogy/eqaddr.h"

// kernel32 CRITICAL_SECTION calls, through the import slots (call dword ptr [va], stdcall), as in unit_10028A90.cpp.
typedef void (__stdcall *EnterCriticalSectionFn_guess)(void *cs);
typedef void (__stdcall *LeaveCriticalSectionFn_guess)(void *cs);
#define pEnterCriticalSection_guess  EQ_GLOBAL(EnterCriticalSectionFn_guess, 0x1004014C)
#define pLeaveCriticalSection_guess  EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x10040148)

// The bucket list's search (0x1002E0C0, unit_1002DE90.cpp: FindRecordNode_guess): __cdecl, (list, dword).
#define FindRecordNode_guess  EQ_CDECL(int, 0x1002E0C0, (int *, int))

// The bucket list's removal for a connection whose last reference is dropped (0x1002E170, unnamed in the bundle):
// __cdecl, (list, conn). Called by connhash_release_guess.
#define removeConnFromBucket_guess  EQ_CDECL(int, 0x1002E170, (int *, int))

// The bucket list's insertion (0x1002DF70, record_insert_sorted_guess in the bundle, unnamed): __cdecl, (list, conn). Called by
// connhash_add_guess, whose result is not used.
#define insertConnToBucket_guess  EQ_CDECL(void, 0x1002DF70, (int *, int))

// FUNCTION: EQMAIN 0x1002E6E0
// Finds the network connection for a remote endpoint so game traffic can use it.
// ORIGINAL FILE: Source/PC Source/Network/rdp/connhash.c (from Mac debug map: connhash_lock)
// __cdecl. Looks up the connection for `addr` in its bucket; if found, takes a reference (under the bucket lock)
// and then the connection's own lock (+0x34). Returns it, or NULL. The bucket is index * 0x30 from hash->buckets:
// its lock at +0x0, its list at +0x18.
RdpConnection_guess *connhash_lock_guess(RdpConnHash_guess *hash, RdpAddr_guess *addr)
{
    u16 h;
    int offset;
    RdpConnection_guess *conn;
    EnterCriticalSectionFn_guess enter;

    switch (addr->family)
    {
    default:
        h = 0;
        break;
    case 2:
        h = ((addr->addr >> 24) ^ (addr->addr >> 12) ^ addr->port ^ *(u16 *)&addr->addr) & (hash->size - 1U);
        break;
    }
    offset = h * 0x30;
    enter = pEnterCriticalSection_guess;
    enter((char *)hash->buckets + offset);
    conn = (RdpConnection_guess *)FindRecordNode_guess((int *)((char *)hash->buckets + offset + 0x18), (int)addr);
    if (conn != 0)
    {
        *(int *)((char *)conn + 0x18) = *(int *)((char *)conn + 0x18) + 1;
    }
    pLeaveCriticalSection_guess((char *)hash->buckets + offset);
    if (conn != 0)
    {
        enter((char *)conn + 0x34);
    }
    return conn;
}

// FUNCTION: EQMAIN 0x1002E770
// Registers a new remote connection for use by game traffic.
// __cdecl. Inserts `conn` into its bucket with a reference count of 1, under the bucket's lock. The bucket is chosen as in
// connhash_lock_guess, from the connection's remote address (family +0x478, port +0x47a, addr +0x47c); the list insert is
// insertConnToBucket_guess. No Mac name in the bundle: it sits between connhash_lock and connhash_release in the run.
void connhash_add_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn)
{
    u16 h;
    int offset;

    switch (*(u16 *)((char *)conn + 0x478))
    {
    default:
        h = 0;
        break;
    case 2:
        h = ((*(u32 *)((char *)conn + 0x47c) >> 24) ^ (*(u32 *)((char *)conn + 0x47c) >> 12) ^
             *(u16 *)((char *)conn + 0x47a) ^ *(u16 *)((char *)conn + 0x47c)) & (u16)(hash->size - 1);
        break;
    }
    offset = h * 0x30;
    pEnterCriticalSection_guess((char *)hash->buckets + offset);
    *(int *)((char *)conn + 0x18) = 1;
    insertConnToBucket_guess((int *)((char *)hash->buckets + offset + 0x18), (int)conn);
    pLeaveCriticalSection_guess((char *)hash->buckets + offset);
}

// FUNCTION: EQMAIN 0x1002E800
// Allows game networking to stop tracking a remote connection when it is no longer in use.
// __cdecl. Drops a reference on `conn` (refcount +0x18) under its bucket's lock. At zero the connection is taken out of
// the bucket list (removeConnFromBucket_guess) and that result is returned; otherwise NULL. The bucket is chosen as in
// connhash_lock_guess, from the connection's remote address (family +0x478, port +0x47a, addr +0x47c).
RdpConnection_guess *connhash_release_guess(RdpConnHash_guess *hash, RdpConnection_guess *conn)
{
    u16 h;
    int offset;
    int refcount;
    RdpConnection_guess *removed;

    removed = 0;
    switch (*(u16 *)((char *)conn + 0x478))
    {
    case 2:
        h = ((*(u32 *)((char *)conn + 0x47c) >> 24) ^ (*(u32 *)((char *)conn + 0x47c) >> 12) ^
             *(u16 *)((char *)conn + 0x47a) ^ *(u16 *)((char *)conn + 0x47c)) & (hash->size - 1);
        break;
    default:
        h = 0;
        break;
    }
    offset = h * 0x30;
    pEnterCriticalSection_guess((char *)hash->buckets + offset);
    refcount = *(int *)((char *)conn + 0x18) - 1;
    *(int *)((char *)conn + 0x18) = refcount;
    if (refcount == 0)
    {
        removed = (RdpConnection_guess *)removeConnFromBucket_guess((int *)((char *)hash->buckets + offset + 0x18), (int)conn);
    }
    pLeaveCriticalSection_guess((char *)hash->buckets + offset);
    return removed;
}
