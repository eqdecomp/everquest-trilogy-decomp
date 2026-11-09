// unit_10030440.cpp - eqmain.dll, the address run 0x10030440..0x10030497 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// One 0x18-byte element of the lock array, as the decompile types LPCRITICAL_SECTION: the code reads only
// recursion_count (0x08) of elements 2 and 3, at 0x38 and 0x50 of the object. The other fields are unknown
// beyond the CRITICAL_SECTION layout.
struct CritSlot_guess
{
    unsigned int debug_info;        // 0x00
    int lock_count;                 // 0x04
    int recursion_count;            // 0x08
    unsigned int owner_thread;      // 0x0C
    unsigned int lock_semaphore;    // 0x10
    unsigned int spin_count;        // 0x14
};

// EnterCriticalSection and LeaveCriticalSection are kernel32 imports, called through their IAT slots.
typedef void (__stdcall *CriticalSectionCall_fn)(CritSlot_guess *);

// FUNCTION: EQMAIN 0x10030440
// Adjusts client timing to account for data still queued for transmission.
// __cdecl, one stack argument (the caller pops). Under the lock: the count at 0x50, less the dword at 0x1c of
// the item GetRecordDelta_guess returns for 0x38 (only when the count is non-zero). Unlocks, then returns the second
// timeGetTime() plus the count * 9000 / 57600 (9 bits per byte at 57600 baud, as calculateAdjustedClientTime_guess).
// The first timeGetTime() result is unused.
unsigned int __cdecl calculateAdjustedClientTime_eqmain_guess(CritSlot_guess *lock)
{
    int item;
    unsigned int now;
    unsigned int bytes;

    EQ_GLOBAL(CriticalSectionCall_fn, 0x1004014C)(lock);
    bytes = lock[3].recursion_count;
    if (bytes > 0)
    {
        item = EQ_CDECL(int, 0x1002E120, (int *))(&lock[2].recursion_count);
        bytes = bytes - *(int *)(item + 0x1c);
    }
    EQ_GLOBAL(CriticalSectionCall_fn, 0x10040148)(lock);
    EQ_STDCALL(unsigned int, 0x1002F9F0, (void))();
    now = EQ_STDCALL(unsigned int, 0x1002F9F0, (void))();
    return now + (unsigned int)(bytes * 9000) / 0xe100;
}
