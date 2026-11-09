// unit_10030FF0.cpp - eqmain.dll, the address run 0x10030FF0..0x1003103F (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x10030FF0
// Removes matching entries from a client bookkeeping queue; the queue's game purpose is unclear.
// __cdecl, two stack args: a record pointer and a key. Rotates the record once: its count at +0x10 is read first,
// then that many nodes are popped from the head (0x1002E130). A node whose dword at +0x8 is the key is freed
// (0x10028A90) and counted; the others go back to the tail (0x1002DF20). Returns the number freed. The record
// layout is the one unit_1002DE90.cpp infers (head +0x0, tail +0x4, base +0x8, count +0x10).
int __cdecl record_remove_matching_guess(int *record, int key)
{
    int item;
    int removed = 0;
    int n = record[4];

    while (n--)
    {
        item = EQ_CDECL(int, 0x1002E130, (int *))(record);
        if (*(int *)(item + 8) == key)
        {
            removed++;
            EQ_CDECL(void, 0x10028A90, (int))(item);
        }
        else
        {
            EQ_CDECL(void, 0x1002DF20, (int *, int))(record, item);
        }
    }
    return removed;
}
