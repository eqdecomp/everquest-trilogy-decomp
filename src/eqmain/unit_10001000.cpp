// unit_10001000.cpp - eqmain.dll, the address run 0x10001000..0x100017B0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include <string.h>

#include "trilogy/eqaddr.h"

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
#define g_eqmain_null_msg_guess     EQ_GLOBAL(char *, 0x10055A68)   // g_eqmain_null_msg_guess: the message FatalErrorExit_guess gets for a NULL object
#define g_eqmain_ref_a_guess        EQ_GLOBAL(void *, 0x1004CD7C)   // g_eqmain_cur_c_guess: an object pointer, cleared when that object is destroyed
#define g_cached_entry_a_guess        EQ_GLOBAL(void *, 0x1004CE6C)   // g_cached_entry_a_guess: the same, a second slot
#define eqmain_fn_100278a0_guess    EQ_CDECL(void, 0x100278A0, (char *))
#define eqmain_fn_10007a80_guess    EQ_CDECL(void, 0x10007A80, (int *))
#define eqmain_fn_10028430_guess    EQ_CDECL(int, 0x10028430, (int *, int *))
#define eqmain_free_guess           EQ_CDECL(void, 0x10033568, (void *))   // _free (the CRT's, linked into eqmain.dll)
#define g_eqmain_str_count_guess    EQ_GLOBAL(unsigned int, 0x1004CD74)   // g_eqmain_str_count_guess: the entry count, read again after each copy
#define g_eqmain_str_table_guess    EQ_ARRAY(char, 0x1004CCD0)            // g_eqmain_str_table_guess: the table of 0x80-byte entries
#define eqmain_fn_100250a0_guess    EQ_CDECL(void, 0x100250A0, (void *))
#define eqmain_fn_10025120_guess    EQ_STDCALL(unsigned int, 0x10025120, (void))
#define eqmain_fn_10025150_guess    EQ_STDCALL(unsigned char *, 0x10025150, (void))
#define eqmain_fn_10021110_guess    EQ_CDECL(char *, 0x10021110, (unsigned char *))
#define eqmain_strncpy_guess        EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))   // _strncpy (the CRT's, linked into eqmain.dll)
#define eqmain_fn_10006c20_guess    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))   // BltFastSrcRect_guess (nine cdecl arguments; the result is ignored)
#define g_eqmain_ceb8_guess          EQ_GLOBAL(int, 0x1004CEB8)            // g_eqmain_ceb8_guess: the first argument of BltFastSrcRect_guess (460 xrefs in the image)
#define g_eqmain_obj_64_guess       EQ_GLOBAL(int *, 0x1004CD64)          // g_eqmain_obj_64_guess: an object pointer, passed to PackFsFreeEntry_guess when set, then cleared
#define g_eqmain_obj_68_guess       EQ_GLOBAL(int *, 0x1004CD68)          // g_eqmain_obj_68_guess: the same, a second slot
#define g_eqmain_obj_6c_guess       EQ_GLOBAL(int *, 0x1004CD6C)          // g_eqmain_obj_6c_guess: the same, a third slot
#define g_eqmain_obj_70_guess       EQ_GLOBAL(int *, 0x1004CD70)          // g_eqmain_obj_70_guess: the same, a fourth slot (passed to BltFastSrcRect_guess first)
#define g_eqmain_flag_78_guess      EQ_GLOBAL(unsigned char, 0x1004CD78)  // g_eqmain_flag_78_guess: a byte flag, cleared here
#define PTR_GetTextExtentPoint32A_guess EQ_GLOBAL(GetTextExtentPoint32A_guess_t, 0x10040038)   // GDI32 import slot
#define eqmain_fn_10008230_guess    EQ_CDECL(int, 0x10008230, (int, int, unsigned int))   // EQMain_SelectFontSize_guess (cdecl: the HDC, a size id, a dword)
#define g_eqmain_size_58_guess      EQ_GLOBAL(int, 0x1004CD58)            // g_eqmain_size_58_guess: the SIZE GetTextExtentPoint32A writes (cx here, cy at 0x1004cd5c)
#define g_eqmain_ptr_60_guess       EQ_GLOBAL(char *, 0x1004CD60)         // g_eqmain_ptr_60_guess: a pointer to the string table (set to g_eqmain_str_table_guess)
#define g_eqmain_val_54_guess       EQ_GLOBAL(unsigned int, 0x1004CD54)   // g_eqmain_int_54_guess: set to 0x198
#define g_eqmain_val_50_guess       EQ_GLOBAL(unsigned int, 0x1004CD50)   // g_eqmain_int_50_guess: set to 0
#define eqmain_fn_10007ae0_guess    EQ_CDECL(int *, 0x10007AE0, (char *, int))   // PackFsGetNamedImage_guess (cdecl: a name, 0x10; returns an object or NULL)
#define eqmain_fn_100210f0_guess    EQ_CDECL(char *, 0x100210F0, (char *))   // Identity_guess (cdecl: one pointer in, a pointer out)
#define eqmain_fn_10006d80_guess    EQ_CDECL(int *, 0x10006D80, (int, int, int))   // CreateSurfaceRecord_guess (cdecl: three ints; returns a surface or NULL)
#define eqmain_fn_100011a0_guess    EQ_STDCALL(void, 0x100011A0, (void))   // eqmain_release_objects_guess (no arguments; eqmain_release_objects_guess)
#define eqmain_fn_10001000_guess    EQ_CDECL(unsigned int, 0x10001000, (unsigned char *))   // eqmain_load_banner_guess (cdecl: one byte pointer in; the result's low byte is tested)
#define eqmain_fn_10001290_guess    EQ_STDCALL(void, 0x10001290, (void))   // eqmain_measure_string_guess (no arguments; eqmain_measure_string_guess)
#define g_eqmain_null_owner_msg_guess EQ_GLOBAL(char *, 0x10055A64)   // g_eqmain_null_owner_msg_guess: the message FatalErrorExit_guess gets when param_1 (the owner) is NULL
#define eqmain_malloc_guess         EQ_CDECL(void *, 0x1003342E, (unsigned int))   // _malloc (the CRT's, linked into eqmain.dll)
#define eqmain_fn_10028340_guess    EQ_CDECL(void, 0x10028340, (unsigned int *, int, int))   // list3_insert_guess (cdecl: the owner's list head at +0x1c, 0, the new object)
#define eqmain_fn_10004ec0_guess    EQ_CDECL(void, 0x10004EC0, (unsigned int *))   // DrawQueueRecord_guess (cdecl: the new object; no result is read)
#define eqmain_fn_10001560_guess    EQ_CDECL(unsigned int *, 0x10001560, (int, int, int, unsigned char, char *, char *, char *))   // eqmain_create_widget_guess (eqmain_create_widget_guess: seven cdecl arguments, returns the new object)
#define g_eqmain_sbutton1_guess     EQ_ARRAY(char *, 0x10046098)   // PTR_s_sbutton1.bmp_10046098: the first name of each 12-byte entry (index param_5 * 3)
#define g_eqmain_sbutton2_guess     EQ_ARRAY(char *, 0x1004609C)   // PTR_s_sbutton2.bmp_1004609c: the second name of each entry
#define g_eqmain_sbutton3_guess     EQ_ARRAY(char *, 0x100460A0)   // PTR_s_sbutton3.bmp_100460a0: the third name of each entry
#define g_eqmain_int_50_guess       EQ_GLOBAL(int, 0x1004CD50)            // g_eqmain_int_50_guess: the word g_eqmain_val_50_guess names, read as a signed int
#define g_eqmain_int_54_guess       EQ_GLOBAL(int, 0x1004CD54)            // g_eqmain_int_54_guess: the word g_eqmain_val_54_guess names, read as a signed int
#define g_eqmain_size_5c_guess      EQ_GLOBAL(int, 0x1004CD5C)            // g_eqmain_size_5c_guess: the cy GetTextExtentPoint32A writes (cx at g_eqmain_size_58_guess)
#define PTR_SetTextColor_guess      EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)   // GDI32 import slot
#define PTR_SetBkMode_guess         EQ_GLOBAL(SetBkMode_guess_t, 0x10040014)      // GDI32 import slot
#define PTR_DrawTextA_guess         EQ_GLOBAL(DrawTextA_guess_t, 0x100401C8)      // USER32 import slot

// The surface at g_eqmain_obj_64_guess +0xc: vtable slot 0x44 is GetDC, 0x68 is ReleaseDC (__stdcall, the surface first).
typedef void (__stdcall *SurfaceGetDC_guess_t)(int *surface, int *hdc);
typedef void (__stdcall *SurfaceReleaseDC_guess_t)(int *surface, int hdc);
typedef int (__stdcall *GetTextExtentPoint32A_guess_t)(int hdc, const char *text, int count, int *size);
typedef unsigned int (__stdcall *SetTextColor_guess_t)(int hdc, unsigned int color);
typedef int (__stdcall *SetBkMode_guess_t)(int hdc, int mode);
typedef int (__stdcall *DrawTextA_guess_t)(int hdc, const char *text, int count, void *rect, unsigned int format);
typedef int (__stdcall *SetTextAlign_guess_t)(int hdc, unsigned int align);
typedef unsigned int (__stdcall *GetLastError_guess_t)(void);
#define PTR_SetTextAlign_guess      EQ_GLOBAL(SetTextAlign_guess_t, 0x10040018)   // GDI32 import slot
#define g_pfn_getlasterror_10040094_guess      EQ_GLOBAL(GetLastError_guess_t, 0x10040094)   // KERNEL32 import slot
#define g_eqmain_msg_6c_guess       EQ_GLOBAL(char *, 0x10055A6C)   // g_eqmain_msg_6c_guess: the message FatalErrorExit_guess gets when GetTextExtentPoint32A fails
#define eqmain_fn_100278a0_err_guess EQ_CDECL(void, 0x100278A0, (char *, unsigned int))   // FatalErrorExit_guess as the failure path calls it: the message, GetLastError()
#define eqmain_fn_100082c0_guess    EQ_CDECL(void, 0x100082C0, (int *, int, int, int, char *, int, int))   // EQMain_DrawTextRun_guess (cdecl: seven words; the result is ignored)

// FUNCTION: EQMAIN 0x10001000
// Loads and displays the chat or server selection banner with its matching overlay.
// __cdecl, one argument (a bitmap name). Returns a bool (callers read AL only). If g_eqmain_obj_64_guess is already set, returns 1.
// Else g_eqmain_obj_64_guess = PackFsGetNamedImage_guess(name, 0x10); NULL returns 0. The name must be "..." (overlay
// "...") or "..." (overlay "..."), else returns 0. Identity_guess maps the
// overlay name, and PackFsGetNamedImage_guess makes g_eqmain_obj_68_guess from it. g_eqmain_obj_6c_guess (CreateSurfaceRecord_guess: 0x182, 0x13, 0x10) and
// g_eqmain_obj_70_guess (0x280, 0x31, 0x10) are surfaces; a NULL one calls eqmain_release_objects_guess and returns 0. Then BltFastSrcRect_guess draws
// with g_eqmain_ceb8_guess, and the function returns 1. The error returns leave g_eqmain_obj_64_guess set, so the next call returns 1.
bool eqmain_load_banner_guess(unsigned char *bannerPath)
{
    if (g_eqmain_obj_64_guess == 0)
    {
        g_eqmain_obj_64_guess = eqmain_fn_10007ae0_guess((char *)bannerPath, 0x10);
        if (g_eqmain_obj_64_guess == 0)
            return 0;
        if (strcmp(EQ_ARRAY(char, 0x10046088), (char *)bannerPath) == 0)
        {
            g_eqmain_obj_68_guess = eqmain_fn_10007ae0_guess(
                eqmain_fn_100210f0_guess(EQ_ARRAY(char, 0x10046070)), 0x10);
        }
        else if (strcmp(EQ_ARRAY(char, 0x10046064), (char *)bannerPath) == 0)
        {
            g_eqmain_obj_68_guess = eqmain_fn_10007ae0_guess(
                eqmain_fn_100210f0_guess(EQ_ARRAY(char, 0x10046050)), 0x10);
        }
        else
            return 0;
        g_eqmain_obj_6c_guess = eqmain_fn_10006d80_guess(0x182, 0x13, 0x10);
        if (g_eqmain_obj_6c_guess == 0)
        {
            eqmain_fn_100011a0_guess();
            return 0;
        }
        eqmain_fn_10006c20_guess((int)g_eqmain_obj_6c_guess, 0, 0, (int)g_eqmain_obj_64_guess,
                                 0x16, 0x14, 0x182, 0x13, 0);
        g_eqmain_obj_70_guess = eqmain_fn_10006d80_guess(0x280, 0x31, 0x10);
        if (g_eqmain_obj_70_guess == 0)
        {
            eqmain_fn_100011a0_guess();
            return 0;
        }
        eqmain_fn_10006c20_guess((int)g_eqmain_obj_70_guess, 0, 0, g_eqmain_ceb8_guess, 0, 0, 0x280, 0x31, 0);
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)g_eqmain_obj_64_guess, 0, 0, 0x280, 0x31, 0);
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)g_eqmain_obj_68_guess, 0, 0, 1, 1, 0);
    }
    return 1;
}

// FUNCTION: EQMAIN 0x100011A0
// Releases cached front-end images and resets the scrolling text state.
// __stdcall (the bundle's header; with no arguments cdecl gives the same code), returns nothing (no caller reads
// EAX). If g_eqmain_obj_70_guess is set, BltFastSrcRect_guess is called first (arguments g_eqmain_ceb8_guess, 0, 0, g_eqmain_obj_70_guess, 0, 0,
// 0x280, 0x31, 0), then PackFsFreeEntry_guess gets g_eqmain_obj_70_guess read again (the call may change it). Each other slot that
// is set goes to PackFsFreeEntry_guess too, in the order g_eqmain_obj_6c_guess, g_eqmain_obj_64_guess, g_eqmain_obj_68_guess. Then the four slots,
// the count g_eqmain_str_count_guess and the flag g_eqmain_flag_78_guess are cleared.
void eqmain_release_objects_guess(void)
{
    if (g_eqmain_obj_70_guess != 0)
    {
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)g_eqmain_obj_70_guess, 0, 0, 0x280, 0x31, 0);
        eqmain_fn_10007a80_guess(g_eqmain_obj_70_guess);
    }
    if (g_eqmain_obj_6c_guess != 0)
    {
        eqmain_fn_10007a80_guess(g_eqmain_obj_6c_guess);
    }
    if (g_eqmain_obj_64_guess != 0)
    {
        eqmain_fn_10007a80_guess(g_eqmain_obj_64_guess);
    }
    if (g_eqmain_obj_68_guess != 0)
    {
        eqmain_fn_10007a80_guess(g_eqmain_obj_68_guess);
    }
    g_eqmain_obj_64_guess = 0;
    g_eqmain_obj_68_guess = 0;
    g_eqmain_obj_70_guess = 0;
    g_eqmain_obj_6c_guess = 0;
    g_eqmain_str_count_guess = 0;
    g_eqmain_flag_78_guess = 0;
    return;
}

// FUNCTION: EQMAIN 0x10001230
// Loads decoded interface strings from a packed data block into the front-end table.
// __cdecl, one stack argument (the caller pops it), returns nothing (no caller reads EAX). Hands param_1 to
// SetStateAndDword_guess (it is kept in g_eqmain_value_b914_guess, the base of a packed blob), reads the entry count from the blob
// (NextDword_guess), then for each entry copies up to 0x80 bytes of the string DecodeToBuffer_guess makes of NextString_guess's
// pointer into the table at g_eqmain_str_table_guess. g_eqmain_str_count_guess is read again after every copy: entry 1 lies over it
// (its byte 36), so the copy can change the loop bound.
void eqmain_load_string_table_guess(void *param_1)
{
    unsigned int entryIndex;
    char *_Dest;

    eqmain_fn_100250a0_guess(param_1);
    g_eqmain_str_count_guess = eqmain_fn_10025120_guess();
    entryIndex = 0;
    if (g_eqmain_str_count_guess > 0)
    {
        _Dest = g_eqmain_str_table_guess;
        do
        {
            eqmain_strncpy_guess(_Dest, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 0x80);
            entryIndex = entryIndex + 1;
            _Dest = _Dest + 0x80;
        } while (entryIndex < g_eqmain_str_count_guess);
    }
}

// FUNCTION: EQMAIN 0x10001290
// Measures the front-end message and initializes its scrolling display.
// __stdcall, no arguments (the callee pops nothing), returns nothing (no caller reads EAX). Gets the DC of the surface
// at g_eqmain_obj_64_guess +0xc (vtable slot 0x44, GetDC), sets it up with EQMain_SelectFontSize_guess(dc, 2, 600), measures the string at
// g_eqmain_str_table_guess into g_eqmain_size_58_guess (GetTextExtentPoint32A) and gives the DC back (slot 0x68, ReleaseDC). Then
// g_eqmain_flag_78_guess = 1, g_eqmain_ptr_60_guess = &g_eqmain_str_table_guess, g_eqmain_int_54_guess = 0x198 and g_eqmain_int_50_guess = 0.
void __stdcall eqmain_measure_string_guess(void)
{
    int hdc;

    (*(SurfaceGetDC_guess_t *)(**(int **)((int)g_eqmain_obj_64_guess + 0xc) + 0x44))(*(int **)((int)g_eqmain_obj_64_guess + 0xc), &hdc);
    eqmain_fn_10008230_guess(hdc, 2, 600);
    PTR_GetTextExtentPoint32A_guess(hdc, g_eqmain_str_table_guess, strlen(g_eqmain_str_table_guess), &g_eqmain_size_58_guess);
    (*(SurfaceReleaseDC_guess_t *)(**(int **)((int)g_eqmain_obj_64_guess + 0xc) + 0x68))(*(int **)((int)g_eqmain_obj_64_guess + 0xc), hdc);
    g_eqmain_flag_78_guess = 1;
    g_eqmain_ptr_60_guess = g_eqmain_str_table_guess;
    g_eqmain_val_54_guess = 0x198;
    g_eqmain_val_50_guess = 0;
}

// FUNCTION: EQMAIN 0x10001320
// Prepares the selected front-end banner and its message for display.
// __cdecl, one stack argument (the caller pops it), returns nothing (no caller reads EAX). When the entry count
// g_eqmain_str_count_guess is 0, eqmain_release_objects_guess runs and that is all. Otherwise eqmain_load_banner_guess is given bannerPath, and when the low
// byte of its result is set, eqmain_measure_string_guess measures the string table.
void eqmain_update_text_guess(unsigned char *bannerPath)
{
    unsigned int loadSucceeded;

    if (g_eqmain_str_count_guess != 0)
    {
        loadSucceeded = eqmain_fn_10001000_guess(bannerPath);
        if ((char)loadSucceeded != '\0')
        {
            eqmain_fn_10001290_guess();
        }
    }
    else
    {
        eqmain_fn_100011a0_guess();
    }
}

// FUNCTION: EQMAIN 0x10001350
// Draws and advances the scrolling front-end message across the banner.
// __stdcall, no arguments, returns nothing (no caller reads EAX). Does nothing while g_eqmain_flag_78_guess is 0. Otherwise BltFastSrcRect_guess
// runs with g_eqmain_obj_6c_guess, and the text at g_eqmain_ptr_60_guess is drawn by DrawTextA on a DC of the surface at g_eqmain_ceb8_guess +0x0c, in
// the rect (g_eqmain_int_54_guess, 0x14, g_eqmain_int_54_guess + g_eqmain_int_50_guess capped at 0x198, g_eqmain_size_5c_guess + 0x14). Then g_eqmain_int_54_guess drops by
// one. Under 0x16, when the first character's width fits in 0x17 - x, x goes back to 0x16 and the text moves on: eqmain_measure_string_guess
// measures the table when the string has ended, else the rest of the string is measured into g_eqmain_size_58_guess. The DC is released
// and BltFastSrcRect_guess runs again with g_eqmain_obj_64_guess.
void __stdcall eqmain_draw_scroll_text_guess(void)
{
    unsigned char hdc[4];                               // undefined1 local_1c[4] in the decompiler: the HDC GetDC writes
    struct { int cx; int cy; } size;                    // SIZE of the first character
    struct { int left; int top; int right; int bottom; } rect;   // RECT of the text
    int *surface;
    GetTextExtentPoint32A_guess_t measure;

    if (g_eqmain_flag_78_guess != '\0')
    {
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x16, 0x14, (int)g_eqmain_obj_6c_guess, 0, 0, 0x182, 0x13, 0);
        if (*g_eqmain_ptr_60_guess == '\0')
        {
            eqmain_fn_10001290_guess();
        }
        rect.left = g_eqmain_int_54_guess;
        rect.right = g_eqmain_int_54_guess + g_eqmain_int_50_guess;
        if (rect.right > 0x198)
        {
            rect.right = 0x198;
        }
        rect.top = 0x14;
        rect.bottom = g_eqmain_size_5c_guess + 0x14;
        surface = *(int **)(g_eqmain_ceb8_guess + 0xc);
        (*(SurfaceGetDC_guess_t *)(*surface + 0x44))(surface, (int *)hdc);
        PTR_SetTextColor_guess(*(int *)hdc, 0xc81000);
        PTR_SetBkMode_guess(*(int *)hdc, 1);
        eqmain_fn_10008230_guess(*(int *)hdc, 2, 600);
        PTR_DrawTextA_guess(*(int *)hdc, g_eqmain_ptr_60_guess, strlen(g_eqmain_ptr_60_guess), &rect, 0x20);
        g_eqmain_int_54_guess = g_eqmain_int_54_guess - 1;
        if (g_eqmain_int_50_guess < g_eqmain_size_58_guess)
        {
            g_eqmain_int_50_guess = g_eqmain_int_50_guess + 1;
        }
        if (g_eqmain_int_54_guess < 0x16)
        {
            measure = PTR_GetTextExtentPoint32A_guess;
            measure(*(int *)hdc, g_eqmain_ptr_60_guess, 1, (int *)&size);
            if (0x17 - g_eqmain_int_54_guess >= size.cx)
            {
                g_eqmain_int_54_guess = 0x16;
                g_eqmain_ptr_60_guess = g_eqmain_ptr_60_guess + 1;
                if (*g_eqmain_ptr_60_guess == '\0')
                {
                    eqmain_fn_10001290_guess();
                }
                else
                {
                    measure(*(int *)hdc, g_eqmain_ptr_60_guess, strlen(g_eqmain_ptr_60_guess), &g_eqmain_size_58_guess);
                }
            }
        }
        surface = *(int **)(g_eqmain_ceb8_guess + 0xc);
        (*(SurfaceReleaseDC_guess_t *)(*surface + 0x68))(surface, *(int *)hdc);
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)g_eqmain_obj_64_guess, 0, 0, 0x16, 0x31, 0);
    }
    return;
}

// FUNCTION: EQMAIN 0x10001520
// Builds a front-end button from the indexed three-image button set.
// __cdecl, five stack arguments (the caller pops them), returns what eqmain_create_widget_guess returns (the callers read EAX). The
// three names of entry buttonSetIndex (12-byte entries: g_eqmain_sbutton1_guess, g_eqmain_sbutton2_guess and g_eqmain_sbutton3_guess at the entry's +0, +4 and
// +8) go to eqmain_create_widget_guess after object to flag.
unsigned int *eqmain_create_button_guess(int owner, int x, int y, unsigned char flags, int buttonSetIndex)
{
    return eqmain_fn_10001560_guess(owner, x, y, flags,
                                    g_eqmain_sbutton1_guess[buttonSetIndex * 3],
                                    g_eqmain_sbutton2_guess[buttonSetIndex * 3],
                                    g_eqmain_sbutton3_guess[buttonSetIndex * 3]);
}

// FUNCTION: EQMAIN 0x10001560
// Builds a UI element of three named images, aligned on the 640x480 screen by flag bits, in its owner's list
// __cdecl, seven stack arguments (the caller pops them), returns the new object (the callers read EAX). NULL owner
// (the owner): FatalErrorExit_guess is given the message at g_eqmain_null_owner_msg_guess and NULL comes back. Otherwise _malloc makes a 0x2c
// byte object (NULL when that fails: nothing else happens). +0x08 the owner; +0x20, +0x24 and +0x28 the objects
// PackFsGetNamedImage_guess returns for the names name1, name2 and name3 (width at +0, height at +4). The rectangle (+0x0c x,
// +0x10 y, +0x14 right, +0x18 bottom) comes from the flags in flags on a 640x480 screen (width and height are the
// first object's): 1 centre x (0x140 - width / 2),
// 2 centre y (0xf0 - height / 2), 4 x + 10, 8 right edge (x 0 is 0x280; x - 10 - width), 0x10 bottom edge (y 0 is
// 0x1e0; y - 10 - height). Then list3_insert_guess adds it to the owner's list (owner + 0x1C) and DrawQueueRecord_guess runs on it.
unsigned int *eqmain_create_widget_guess(int owner, int param_2, int y, unsigned char flags, char *name1, char *name2, char *name3)
{
    int width;
    int height;
    unsigned int *obj;
    int *loaded;

    if (owner == 0)
    {
        eqmain_fn_100278a0_guess(g_eqmain_null_owner_msg_guess);
        return (unsigned int *)0x0;
    }
    obj = (unsigned int *)eqmain_malloc_guess(0x2c);
    if (obj != (unsigned int *)0x0)
    {
        obj[0] = 0;
        obj[2] = owner;
        obj[7] = 0;
        loaded = eqmain_fn_10007ae0_guess(name1, 0x10);
        obj[8] = (unsigned int)loaded;
        loaded = eqmain_fn_10007ae0_guess(name2, 0x10);
        obj[9] = (unsigned int)loaded;
        loaded = eqmain_fn_10007ae0_guess(name3, 0x10);
        obj[10] = (unsigned int)loaded;
        width = *(int *)obj[8];
        height = ((int *)obj[8])[1];
        if ((flags & 1) != 0)
        {
            param_2 = 0x140 - width / 2;
        }
        if ((flags & 2) != 0)
        {
            y = 0xf0 - height / 2;
        }
        if ((flags & 0x10) != 0)
        {
            if (y == 0)
            {
                y = 0x1e0;
            }
            y = y + (-10 - height);
        }
        if ((flags & 4) != 0)
        {
            param_2 = param_2 + 10;
        }
        if ((flags & 8) != 0)
        {
            if (param_2 == 0)
            {
                param_2 = 0x280;
            }
            param_2 = param_2 + (-10 - width);
        }
        obj[4] = y;
        obj[3] = param_2;
        obj[5] = width + param_2;
        obj[6] = height + y;
        eqmain_fn_10028340_guess((unsigned int *)(owner + 0x1c), 0, (int)obj);
        eqmain_fn_10004ec0_guess(obj);
    }
    return obj;
}

// FUNCTION: EQMAIN 0x10001680
// Creates and registers a rectangular front-end control with its owner.
// __cdecl, five stack arguments (the caller pops them), returns the new object (the caller reads EAX). NULL owner
// (the owner): FatalErrorExit_guess is given the message at g_eqmain_null_owner_msg_guess and NULL comes back. Otherwise _malloc makes a 0x2c
// byte object (NULL when that fails: nothing else happens). +0x08 the owner; +0x0c x (x), +0x10 y (y),
// +0x14 x + width and +0x18 y + height (the right and bottom edges); +0x00, +0x1c, +0x20, +0x24 and +0x28 are 0.
// Then list3_insert_guess adds it to the owner's list (owner + 0x1C) and DrawQueueRecord_guess runs on it.
unsigned int *eqmain_create_rect_widget_guess(int owner, int x, int y, int width, int height)
{
    unsigned int *obj;

    if (owner == 0)
    {
        eqmain_fn_100278a0_guess(g_eqmain_null_owner_msg_guess);
        return (unsigned int *)0x0;
    }
    obj = (unsigned int *)eqmain_malloc_guess(0x2c);
    if (obj != (unsigned int *)0x0)
    {
        obj[3] = x;
        obj[2] = owner;
        obj[4] = y;
        obj[5] = x + width;
        obj[0] = 0;
        obj[7] = 0;
        obj[8] = 0;
        obj[9] = 0;
        obj[10] = 0;
        obj[6] = y + height;
        eqmain_fn_10028340_guess((unsigned int *)(owner + 0x1c), 0, (int)obj);
        eqmain_fn_10004ec0_guess(obj);
    }
    return obj;
}

// FUNCTION: EQMAIN 0x10001700
// Removes a front-end control from its owner's list and frees its storage.
// __cdecl, one stack argument (the caller pops it), returns nothing (no caller reads EAX). Destroys the object
// object. NULL: FatalErrorExit_guess is given the message at g_eqmain_null_msg_guess and nothing else happens. Otherwise
// g_eqmain_cur_c_guess and g_cached_entry_a_guess are cleared when they hold the object; its sub-objects at +0x20, +0x24 and +0x28
// go through PackFsFreeEntry_guess when set; then, when the list at owner+0x1C holds it (owner = object +0x8; list node:
// item at +4, next at +8), the node is unlinked by list3_unlink_free_guess; the object is freed with _free.
void eqmain_object_destroy_guess(void *object)
{
    int *node;

    if (object == (void *)0x0)
    {
        eqmain_fn_100278a0_guess(g_eqmain_null_msg_guess);
        return;
    }
    if (g_eqmain_ref_a_guess == object)
    {
        g_eqmain_ref_a_guess = (void *)0x0;
    }
    if (object == g_cached_entry_a_guess)
    {
        g_cached_entry_a_guess = (void *)0x0;
    }
    if (*(int **)((int)object + 0x20) != (int *)0x0)
    {
        eqmain_fn_10007a80_guess(*(int **)((int)object + 0x20));
    }
    if (*(int **)((int)object + 0x24) != (int *)0x0)
    {
        eqmain_fn_10007a80_guess(*(int **)((int)object + 0x24));
    }
    if (*(int **)((int)object + 0x28) != (int *)0x0)
    {
        eqmain_fn_10007a80_guess(*(int **)((int)object + 0x28));
    }
    node = *(int **)(*(int *)((int)object + 8) + 0x1c);
    while (node != (int *)0x0)
    {
        if ((int)object == node[1])
        {
            eqmain_fn_10028430_guess((int *)(*(int *)((int)object + 8) + 0x1c), node);
            break;
        }
        node = (int *)node[2];
    }
    eqmain_free_guess(object);
    return;
}

// FUNCTION: EQMAIN 0x100017B0
// Draws a centered label on a front-end control and its optional image layers.
// __cdecl, five stack arguments (the caller pops them). A NULL text returns. Otherwise the surface at object+0x20
// (+0xC) gives its DC through slot 0x44 (written over the text argument), SetTextAlign(hdc, 0) and EQMain_SelectFontSize_guess(hdc,
// option, setting) run, and GetTextExtentPoint32A measures the text (a failure gives GetLastError to FatalErrorExit_guess
// and returns). Otherwise slot 0x68 releases the DC, and the text is drawn centred on the surface's two words (+0,
// +4) by EQMain_DrawTextRun_guess for the surface, then for the sub-objects at +0x24 and +0x28 when set. DrawQueueRecord_guess takes object.
void eqmain_draw_text_guess(void *object, int value, char *textString, int option, int fontWeight)
{
    char *lpString;
    int *ptr;
    int size[2];
    int x;
    int y;

    lpString = textString;
    if (textString != (char *)0x0)
    {
        ptr = *(int **)(*(int *)((int)object + 0x20) + 0xc);
        (*(SurfaceGetDC_guess_t *)(*ptr + 0x44))(ptr, (int *)&textString);
        PTR_SetTextAlign_guess((int)textString, 0);
        eqmain_fn_10008230_guess((int)textString, option, (unsigned int)fontWeight);
        if (!PTR_GetTextExtentPoint32A_guess((int)textString, lpString, (int)strlen(lpString), size))
        {
            eqmain_fn_100278a0_err_guess(g_eqmain_msg_6c_guess, g_pfn_getlasterror_10040094_guess());
            return;
        }
        ptr = *(int **)(*(int *)((int)object + 0x20) + 0xc);
        (*(SurfaceReleaseDC_guess_t *)(*ptr + 0x68))(ptr, (int)textString);
        ptr = *(int **)((int)object + 0x20);
        x = ptr[0] / 2 - size[0] / 2;
        y = ptr[1] / 2 - size[1] / 2;
        if (ptr != (int *)0x0)
        {
            eqmain_fn_100082c0_guess(ptr, x, y, value, lpString, option, fontWeight);
        }
        if (*(int **)((int)object + 0x24) != (int *)0x0)
        {
            eqmain_fn_100082c0_guess(*(int **)((int)object + 0x24), x, y, value, lpString, option, fontWeight);
        }
        if (*(int **)((int)object + 0x28) != (int *)0x0)
        {
            eqmain_fn_100082c0_guess(*(int **)((int)object + 0x28), x, y, value, lpString, option, fontWeight);
        }
        eqmain_fn_10004ec0_guess((unsigned int *)object);
    }
    return;
}
