// unit_10030510.cpp - eqmain.dll, the function at 0x10030510 (a queue unit: no Mac map covers eqmain.dll, so the
// original object and its name are unknown).

#include "trilogy/eqaddr.h"

// A thread start record: the callback at 0x08 and its argument at 0x0c. The other fields are unknown.
struct ThreadStart_guess
{
    unsigned int unknown0;          // 0x00
    unsigned int unknown4;          // 0x04
    void (__cdecl *fn)(void *);     // 0x08
    void *arg;                      // 0x0C
};

// FUNCTION: EQMAIN 0x10030510
// __stdcall thread entry (address taken by createThreadWithPriority_guess): calls start->fn(start->arg), returns 0.
unsigned int __stdcall InvokeThreadCallback_guess(ThreadStart_guess *start)
{
    start->fn(start->arg);
    return 0;
}
