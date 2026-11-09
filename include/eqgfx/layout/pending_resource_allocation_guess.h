#ifndef EQGFX_PENDING_RESOURCE_ALLOCATION_GUESS_H
#define EQGFX_PENDING_RESOURCE_ALLOCATION_GUESS_H

#include "trilogy/eqaddr.h"

// Own a raw allocation until its client constructor completes.
struct PendingResourceAllocation_guess {
    void *memory;
    ~PendingResourceAllocation_guess() {
        if (memory != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(memory);
    }
};

#endif
