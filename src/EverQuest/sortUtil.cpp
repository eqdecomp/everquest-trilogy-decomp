#include "trilogy/eqaddr.h"

// FUNCTION: TRILOGY 0x004e1d09
// ORIGINAL FILE: EverQuest/sortUtil.cpp (from Mac debug map)
// as original: the sort-type dispatch shares the secondary-key comparison and the swap block (goto into them).
void __cdecl doSort(short sortType)
{
    int* current = EQ_GLOBAL(int*, 0x006E4D70);
    int* next;
    for (;;)
    {
        if (current == 0 || (next = (int*)current[1]) == 0)
            return;
        const char* nextKey;
        int comparison;
        if (sortType == 0)
            goto secondary;
        if (sortType == 1)
        {
            nextKey = (const char*)(next + 7);
            if (EQ_CDECL(int, 0x00524240, (const char*, const char*))
                ((const char*)(current + 7), nextKey) == 0)
                goto secondary;
            comparison = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                ((const char*)(current + 7), nextKey);
        }
        else if (sortType == 2)
        {
            if (*(unsigned char*)((char*)current + 0x3a) ==
                *(unsigned char*)((char*)next + 0x3a))
                goto secondary;
            if (*(unsigned char*)((char*)current + 0x3a) >
                *(unsigned char*)((char*)next + 0x3a))
                goto swap;
            current = next;
            continue;
        }
        else if (sortType == 3)
        {
            nextKey = (const char*)next + 0x3b;
            if (EQ_CDECL(int, 0x00524240, (const char*, const char*))
                ((const char*)current + 0x3b, nextKey) == 0)
            {
secondary:
                comparison = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                    ((const char*)(current + 3), (const char*)(next + 3));
            }
            else
                comparison = EQ_CDECL(int, 0x00524240, (const char*, const char*))
                    ((const char*)current + 0x3b, nextKey);
        }
        else
            continue;
        if (comparison > 0)
        {
swap:
            int* after = (int*)next[1];
            int before = *current;
            if (after != 0)
                *after = (int)current;
            if (*current != 0)
                *(int**)(*current + 4) = next;
            next[1] = (int)current;
            *next = before;
            current[1] = (int)after;
            *current = (int)next;
            if (current == EQ_GLOBAL(int*, 0x006E4D70))
                EQ_GLOBAL(int*, 0x006E4D70) = next;
            current = EQ_GLOBAL(int*, 0x006E4D70);
        }
        else
            current = (int*)current[1];
    }
}
