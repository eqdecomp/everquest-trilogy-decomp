// EQMisc.cpp - eqmain.dll, the address run 0x100239C0 (a queue unit: no Mac map covers eqmain.dll, so the
// original file is unknown; tools/docs/EQMAIN.md).

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "trilogy/eqaddr.h"

typedef INT_PTR (__stdcall *DialogBoxParamAFn_guess)(HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM);

#define eqmain_fn_10004ec0_guess        EQ_CDECL(void, 0x10004EC0, (DWORD *))                       // DrawQueueRecord_guess: __cdecl, one argument
#define eqmain_fn_10007ae0_guess        EQ_CDECL(int *, 0x10007AE0, (char *, int))                  // PackFsGetNamedImage_guess: __cdecl, returns int * (its first two ints are read)
#define eqmain_fn_100082c0_guess        EQ_CDECL(DWORD *, 0x100082C0, (int, COLORREF, HDC, int, LPCSTR, int, int))  // EQMain_DrawTextRun_guess: __cdecl, the caller pushes 7 arguments
#define eqmain_fn_1001ce60_guess        EQ_STDCALL(void, 0x1001CE60, (void))                        // EQMain_LoadLicenseText_guess: __stdcall, no arguments
#define eqmain_fn_100278a0_guess        EQ_CDECL(void, 0x100278A0, (char *))                        // FatalErrorExit_guess: __cdecl here (BUNDLE says __stdcall; the call site ADDs ESP,4)
#define eqmain_fn_10028340_guess        EQ_CDECL(void, 0x10028340, (DWORD *, int, int))             // list3_insert_guess: __cdecl, 3 arguments
#define eqmain_fn_1003342e_guess        EQ_CDECL(void *, 0x1003342E, (size_t))                      // _malloc: __cdecl, 18 bytes, a library wrapper
#define eqmain_fn_1003407f_guess        EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))  // fopen-like library wrapper (19 bytes)
#define eqmain_fn_10034169_guess        EQ_CDECL(size_t, 0x10034169, (const void *, size_t, size_t, FILE *))   // _fwrite
#define eqmain_fn_100343b9_guess        EQ_CDECL(int, 0x100343B9, (FILE *))                         // _fclose (49 bytes, a library wrapper)
#define g_eqmain_dialogboxparama_guess  EQ_GLOBAL(DialogBoxParamAFn_guess, 0x100401F8)              // PTR_DialogBoxParamA: USER32 IAT slot
#define g_eqmain_dat_10055a70_guess     EQ_GLOBAL(DWORD, 0x10055A70)                                // DAT_10055a70: undefined4 global, FatalErrorExit_guess's argument

// FUNCTION: EQMAIN 0x100018f0
// Adds an image and optional label as an entry in a front-end list.
// __cdecl, ten stack arguments (the code reads all ten; the last two only go on to EQMain_DrawTextRun_guess). Callee pops
// nothing. Returns a new 0x2C-byte record, or NULL when parent is NULL (after FatalErrorExit_guess is called with
// DAT_10055a70) or malloc fails. Otherwise it stores the two PackFsGetNamedImage_guess results, the EQMain_DrawTextRun_guess pointer
// (+8, so a NULL result becomes 8) when text is not NULL, then calls list3_insert_guess with parent + 0x1C and
// DrawQueueRecord_guess on the record.
DWORD *__cdecl CreateColumnCell_guess(int parent, int x, HDC y, char *imageName, char *altImageName,
                                      DWORD value, int textColor, LPCSTR text, int fontSizeId, int fontWeight)
{
    DWORD *record;
    DWORD *textWidth;
    int *image;
    int imageWidth;
    int imageHeight;

    textWidth = 0;
    if (parent == 0)
    {
        eqmain_fn_100278a0_guess((char *)g_eqmain_dat_10055a70_guess);
        return 0;
    }
    record = (DWORD *)eqmain_fn_1003342e_guess(0x2c);
    if (record != 0)
    {
        record[0] = 5;
        record[2] = parent;
        record[7] = value;
        image = eqmain_fn_10007ae0_guess(imageName, 0x10);
        record[9] = (DWORD)image;
        image = eqmain_fn_10007ae0_guess(altImageName, 0x10);
        record[10] = (DWORD)image;
        imageWidth = *(int *)record[9];
        imageHeight = ((int *)record[9])[1];
        if (text != 0)
        {
            textWidth = (DWORD *)eqmain_fn_100082c0_guess(parent, x + 8 + imageWidth, y, textColor, text, fontSizeId, fontWeight);
            textWidth = textWidth + 2;
        }
        record[3] = x;
        record[8] = (DWORD)textWidth;
        record[4] = (DWORD)y;
        record[5] = (int)textWidth + x + imageWidth;
        record[6] = (int)y + imageHeight;
        eqmain_fn_10028340_guess((DWORD *)(parent + 0x1c), 0, (int)record);
        eqmain_fn_10004ec0_guess(record);
    }
    return record;
}

// FUNCTION: EQMAIN 0x1001d5e0
// Handles the eqlicense.txt prompt and creates the file when it is missing.
// __cdecl, one argument (the HINSTANCE the dialogs are made from), returns bool; callee pops 0 bytes.
// Runs EQMain_LoadLicenseText_guess, then opens eqlicense.txt. If that fails, shows dialog 67 and returns false when it
// returns 0; otherwise it creates eqlicense.txt, writes its eleven strings (the new FILE * is not checked)
// and closes it. If the first open worked, that FILE * is closed. Then shows dialog 66 and returns whether
// that dialog returned non-zero.
bool __cdecl CheckLicenseFileAndPrompt_guess(HINSTANCE hInstance)
{
    FILE *f;
    INT_PTR r;
    DialogBoxParamAFn_guess dialog;

    eqmain_fn_1001ce60_guess();
    f = eqmain_fn_1003407f_guess((const char *)0x100482a4, (const char *)0x100480a4);
    dialog = g_eqmain_dialogboxparama_guess;
    if (f == 0)
    {
        r = dialog(hInstance, (LPCSTR)0x67, 0, (DLGPROC)0x1001d3c0, 0);
        if (r == 0)
            return false;
        f = eqmain_fn_1003407f_guess((const char *)0x100482a4, (const char *)0x10047f74);
        eqmain_fn_10034169_guess((const char *)0x100dc8d8, strlen((const char *)0x100dc8d8), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100dd490, strlen((const char *)0x100dd490), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d5f60, strlen((const char *)0x100d5f60), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d6b18, strlen((const char *)0x100d6b18), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d76d0, strlen((const char *)0x100d76d0), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d8288, strlen((const char *)0x100d8288), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d8e40, strlen((const char *)0x100d8e40), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100d99f8, strlen((const char *)0x100d99f8), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100da5b0, strlen((const char *)0x100da5b0), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100db168, strlen((const char *)0x100db168), 1, f);
        eqmain_fn_10034169_guess((const char *)0x100dbd20, strlen((const char *)0x100dbd20), 1, f);
        eqmain_fn_100343b9_guess(f);
    }
    else
    {
        eqmain_fn_100343b9_guess(f);
    }
    r = dialog(hInstance, (LPCSTR)0x66, 0, (DLGPROC)0x1001cfb0, 0);
    if (r != 0)
        return 1;
    return 0;
}

// FUNCTION: EQMAIN 0x100239c0
// Releases an allocated block; its specific front-end role is unclear.
// __cdecl, one argument, no return value. The sibling 004E5F2D is the destructor of flex_unit_guess
// (delete[] of its unit array); here it is the free-only tail: the pointer is passed to the client's
// _free (0x10033568, VC6 LIBCMT) when it is not null, and the caller pops the argument.
void __cdecl flex_unit_free_guess(void *p)
{
    if (p != 0)
        EQ_CDECL(void, 0x10033568, (void *))(p);
}
