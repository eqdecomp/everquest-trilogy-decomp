// unit_10031040.cpp - eqmain.dll, the address run 0x10031040..0x10031064 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x10031040
// __cdecl, two pointers to shorts: a qsort-style comparator. Returns 1, 0 or -1 by the sign of the 16-bit difference.
int __cdecl CompareShorts_guess(short *first, short *second)
{
    short diff = *first - *second;

    if (diff > 0)
        diff = 1;
    else if (diff < 0)
        diff = -1;
    return diff;
}
