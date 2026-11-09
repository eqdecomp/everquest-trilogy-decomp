// SoundManager.cpp - eqmain.dll, the 24-byte function at 0x10027960..0x10027977 (just before unit_10027980.cpp).
// The Mac map does not cover eqmain.dll, so the original file is unknown (tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"

typedef int (__stdcall *DeleteObject_fn)(void *);

// FUNCTION: EQMAIN 0x10027960
// Cleans up a GDI resource for an interface element whose in-game purpose is unclear.
// __thiscall (this in ECX, no stack arguments; written as __fastcall with an EDX dummy). Name from the trilogy sibling
// (low confidence): the body does not copy anything. If the GDI handle at self[0] is not null, DeleteObject (imported
// through the pointer at 0x10040020) deletes it; the word at self+0x40 is then cleared. The handle is left as is.
void __fastcall DeleteGdiObjectAndClearWord_guess(unsigned int *self, int /* EDX dummy */)
{
    if (self[0] != 0)
    {
        EQ_GLOBAL(DeleteObject_fn, 0x10040020)((void *)self[0]);
    }
    *(unsigned short *)(self + 0x10) = 0;
}
