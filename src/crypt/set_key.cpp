// set_key.cpp - lives in the crypt/set_key range, but the function works on the Columns high score table.
// ORIGINAL FILE: crypt/set_key.c (from Mac debug map)

#include "trilogy/common.h"
#include "trilogy/eq_columns.h"

// FUNCTION: TRILOGY 0x00433495
// Clears the ten saved Columns high score entries.
// ORIGINAL FILE: crypt/set_key.c (from Mac debug map)
// __thiscall (this in ECX, no stack args). Clears score, level and the first name byte of each of the
// 10 high score entries. Written as a __fastcall free function because eq_columns.h declares no member for it.
void __fastcall ClearHighScores_guess(EQ_Columns_guess* columns)
{
    u32* p = (u32*)&columns->scores_guess[0].level;
    int i = 10;

    do
    {
        p[-1] = 0;          // score
        p[0] = 0;           // level
        *(char*)(p + 1) = 0;    // name[0]
        p += 10;
        i--;
    } while (i != 0);
}
