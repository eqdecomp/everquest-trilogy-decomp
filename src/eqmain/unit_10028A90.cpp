// unit_10028A90.cpp - eqmain.dll, the address run 0x10028A90..0x10028ADD (a pool unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// The pool records as this function uses them: 0x3c-byte records from 0x1013D72C, each with its critical
// section at +0x18 (so the lock of record i is at 0x1013D744 + i * 0x3c). The rest of a record is unknown.
#define g_pool_records_guess    EQ_ARRAY(char, 0x1013D72C)
#define g_pool_locks_guess      EQ_ARRAY(char, 0x1013D744)

// kernel32 CRITICAL_SECTION calls, through the import slots (call dword ptr [va], stdcall).
typedef void (__stdcall *EnterCriticalSectionFn_guess)(void *cs);
typedef void (__stdcall *LeaveCriticalSectionFn_guess)(void *cs);
#define pEnterCriticalSection_guess  EQ_GLOBAL(EnterCriticalSectionFn_guess, 0x1004014C)
#define pLeaveCriticalSection_guess  EQ_GLOBAL(LeaveCriticalSectionFn_guess, 0x10040148)

// FUNCTION: EQMAIN 0x10028a90
// Releases a client memory block after use.
// __cdecl. Frees a block whose pool index is the dword 8 bytes before it. Index 4 means the block came
// straight from the heap (its base is at -8): _free(base). Any other index selects pool record index
// (0x3c bytes each): record_add_head_guess (0x1002DED0) gets the record and the base, under the record's critical
// section. No checks: a bad index walks off the record table.
void __cdecl free_pool_block_guess(int block)
{
    int record_off;

    if (*(int *)(block - 8) == 4)
    {
        EQ_CDECL(void, 0x10033568, (void *))((void *)(block - 8));
        return;
    }
    record_off = *(int *)(block - 8) * 0x3c;
    pEnterCriticalSection_guess(g_pool_locks_guess + record_off);
    EQ_CDECL(void, 0x1002DED0, (int *, int))((int *)(g_pool_records_guess + record_off), block - 8);
    pLeaveCriticalSection_guess(g_pool_locks_guess + record_off);
    return;
}
