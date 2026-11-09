// EQ_Columns.cpp - eqmain.dll, the EverQuest subsystem's EQ_Columns functions (this image's VAs, not trilogy's).

#include <windows.h>

#include "trilogy/eqaddr.h"

// eqmain.dll's globals, functions and string literals used by EQ_Columns_MarkColumnCellsAsActive_guess (this image's VAs).
#define g_eqmain_ceb8_guess             EQ_GLOBAL(int, 0x1004CEB8)                                  // g_eqmain_ceb8_guess: the first argument of CreateColumnCell_guess (460 xrefs)
#define eqmain_fn_10025940_guess(object) EQ_METHOD(1, char *, 0x10025940, (void *, int), (object))        // IdxLookup_guess::Get_guess: __thiscall (this in ECX, one stack argument: the index; the middle int is the EDX dummy argument), returns the item's text
#define eqmain_fn_100018f0_guess        EQ_CDECL(DWORD *, 0x100018F0, (int, int, HDC, char *, char *, DWORD, int, LPCSTR))  // CreateColumnCell_guess: __cdecl, 8 arguments (the checkbox control)
#define eqmain_str_chekbox1_guess       EQ_ARRAY(char, 0x100461D8)                                  // s_chekbox1.bmp_100461d8: "chekbox1.bmp" (its address is the argument)
#define eqmain_str_chekbox2_guess       EQ_ARRAY(char, 0x100461E8)                                  // s_chekbox2.bmp_100461e8: "chekbox2.bmp"

// FUNCTION: EQMAIN 0x100119a0
// @early-stop: the required fastcall address macro sets dummy EDX, and its one declared stack argument cannot reproduce the original's two ignored stack words.
// __cdecl, seven stack arguments (callee pops 0 bytes); returns nothing (the callers never read EAX). IdxLookup_guess::Get_guess on the
// object at labelEntry with index 0 is called first and its result dropped; then, for each of the object's items (the int at +8),
// IdxLookup_guess::Get_guess gives the item's text and CreateColumnCell_guess makes a checkbox control from it (chekbox1.bmp, chekbox2.bmp, the flags
// mask & (1 << index), id 0x21a3c, the text) on the parent at ptr, which is stored in toggleControls[index]. The next x is
// x + columnSpacing; when x + 2 * columnSpacing is above 0x280 the next x goes back to initialX and ptr moves on by rowStride bytes.
// Name note: this image's function is a cdecl free function (no `this`), named after the trilogy class it sits with.
// The original pushes two more zero arguments after the index for the item call and leaves them for the ADD ESP,0x28 after
// the control call; that dead stack is not reproduced.
void __cdecl EQ_Columns_MarkColumnCellsAsActive_guess(DWORD *toggleControls, int initialX, HDC ptr, void *labelEntry, int columnSpacing, int rowStride, unsigned int mask)
{
    int itemIndex;
    int currentX;
    unsigned int activeBit;
    DWORD *control;
    char *pCVar1;

    eqmain_fn_10025940_guess(labelEntry)( 0);
    itemIndex = 0;
    activeBit = 1;
    currentX = initialX;
    if (0 < *(int *)((int)labelEntry + 8)) {
        DWORD *pOutput = toggleControls;
        do {
            pCVar1 = eqmain_fn_10025940_guess(labelEntry)( itemIndex);
            control = eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, currentX, ptr, eqmain_str_chekbox1_guess,
                                              eqmain_str_chekbox2_guess, activeBit & mask, 0x21a3c, pCVar1);
            activeBit = activeBit * 2;
            *pOutput = (DWORD)control;
            currentX += columnSpacing;
            if (0x280 < currentX + columnSpacing) {
                ptr = (HDC)((int)ptr + rowStride);
                currentX = initialX;
            }
            itemIndex = itemIndex + 1;
            pOutput = pOutput + 1;
        } while (itemIndex < *(int *)((int)labelEntry + 8));
    }
    return;
}
