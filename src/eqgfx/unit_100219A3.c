// unit_100219A3.c - eqgfx_dx7.dll, the address run 0x100219A3..0x100219BD (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// Local layout guess: a holder whose first field points at an inner object
// that holds the file handle at +0x8; the handle holds its data at +0x0.
// No header defines these yet.
struct T3dFileHandle100219A3_guess {
    void *data; // 0x0
};

struct T3dFileInner100219A3_guess {
    char pad[8]; // 0x0
    struct T3dFileHandle100219A3_guess *handle; // 0x8
};

struct T3dFastFile100219A3_guess {
    struct T3dFileInner100219A3_guess *inner; // 0x0
};

// FUNCTION: EQGFX 0x100219a3
// __cdecl. Returns the file size, or -1 when the file or its data is missing.
int __cdecl t3dFastFileGetEOF(struct T3dFastFile100219A3_guess *fastFile)
{
    int eof = -1;
    struct T3dFileHandle100219A3_guess *handle;

    if (fastFile != 0)
    {
        handle = fastFile->inner->handle;
        if (handle->data != 0)
            eof = EQ_THISCALL(int, 0x10052A80, (void *))(handle);
    }
    return eof;
}
