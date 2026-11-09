// EQ_Character.cpp - eqmain.dll, the address run 0x1001DBC0 (no Mac map covers eqmain.dll, so the original
// file is unknown).

#include <windows.h>

#include "trilogy/eqaddr.h"

#define eqmain_fn_1001db40_guess        EQ_STDCALL(void, 0x1001DB40, (void))                        // LoadSelServerOverlay_guess: __stdcall, no arguments
#define eqmain_fn_1001e350_guess        EQ_CDECL(LPCSTR, 0x1001E350, (DWORD))                       // FlagClassValue_guess: __cdecl, returns the string (undefined4 in Ghidra)
#define eqmain_fn_1001e2f0_guess        EQ_CDECL(int, 0x1001E2F0, (DWORD))                          // FlagClassConst_guess: __cdecl, returns int
#define eqmain_fn_100082c0_guess        EQ_CDECL(DWORD *, 0x100082C0, (int, COLORREF, HDC, int, LPCSTR, int, int))  // EQMain_DrawTextRun_guess: __cdecl, the caller pushes 7 arguments
#define eqmain_fn_100017b0_guess        EQ_CDECL(void, 0x100017B0, (HDC, int, LPCSTR, int, int))    // eqmain_draw_text_guess: __cdecl, the caller pushes 5 arguments
#define g_eqmain_ceb8_guess     EQ_GLOBAL(int, 0x1004CEB8)                                  // g_eqmain_ceb8_guess: undefined4 global, first argument of EQMain_DrawTextRun_guess
#define g_eqmain_dat_10055a00_guess     EQ_GLOBAL(LPCSTR, 0x10055A00)                               // DAT_10055a00: undefined4 global, the string when param_2 != 0
#define g_eqmain_dat_100f1160_guess     EQ_GLOBAL(DWORD, 0x100F1160)                                // DAT_100f1160: undefined4 global, argument of FlagClassValue_guess and FlagClassConst_guess
#define g_eqmain_dat_100f10d0_guess     EQ_GLOBAL(HDC, 0x100F10D0)                                  // DAT_100f10d0: undefined4 global, first argument of eqmain_draw_text_guess

// FUNCTION: EQMAIN 0x1001dbc0
// Displays a supplied label with optional accompanying text.
// __cdecl free function, no `this` (the queue name EQ_Character::ClassMin_guess is a low-confidence sibling
// guess; the sibling's table lookup is a different function). Calls LoadSelServerOverlay_guess, then: flag != 0 passes
// DAT_10055a00 to EQMain_DrawTextRun_guess with 0xf3f2f2; flag == 0 does nothing more while DAT_100f1160 is 0, else passes
// FlagClassConst_guess's int and FlagClassValue_guess's string to EQMain_DrawTextRun_guess (both called with DAT_100f1160). Last, eqmain_draw_text_guess
// gets text. The trailing 0, 0 of each call are real stack arguments (the caller pushes 7 and 5). VC6 pushes
// arguments right to left, so FlagClassValue_guess runs before FlagClassConst_guess, as the asm does. Returns nothing.
void __cdecl EQ_Character_ClassMin_guess(LPCSTR text, char flag)
{
    eqmain_fn_1001db40_guess();
    if (flag != 0)
    {
        eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x74, (HDC)0x16a, 0xf3f2f2, g_eqmain_dat_10055a00_guess, 0, 0);
    }
    else
    {
        if (g_eqmain_dat_100f1160_guess == 0)
            goto drawLabel;
        eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x74, (HDC)0x16a,
                                 eqmain_fn_1001e2f0_guess(g_eqmain_dat_100f1160_guess),
                                 eqmain_fn_1001e350_guess(g_eqmain_dat_100f1160_guess), 0, 0);
    }
drawLabel:
    eqmain_fn_100017b0_guess(g_eqmain_dat_100f10d0_guess, 0xe8c9cc, text, 0, 0);
}
