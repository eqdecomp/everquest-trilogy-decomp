#ifndef TRILOGY_EQGFX_SHARED_PTR_GUESS_H
#define TRILOGY_EQGFX_SHARED_PTR_GUESS_H

#include "eqaddr.h"

struct EqgfxRefCounted10021c32_guess {
    void *vtable;
    int referenceCount;
};

struct EqgfxSharedPtr10021c32_guess {
    struct EqgfxRefCounted10021c32_guess *value;

    ~EqgfxSharedPtr10021c32_guess() {
        if (value != 0)
            EQ_METHOD(0, void, 0x1002190D, (EqgfxRefCounted10021c32_guess *), value)();
    }
    EqgfxSharedPtr10021c32_guess *t3dAssignSharedPtr_guess(EqgfxSharedPtr10021c32_guess value);
};

#endif
