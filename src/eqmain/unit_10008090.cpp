// unit_10008090.cpp - eqmain.dll, the address run 0x10008090..0x10008C10 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "trilogy/eqaddr.h"

// File-local stand-in for the record (no header defines it yet): vtable at 0x0, then three dwords.
class EQMain_Record_guess
{
public:
    void *vtbl;         // 0x0
    int field_4;        // 0x4
    int field_8;        // 0x8
    int field_c;        // 0xc

    EQMain_Record_guess *Init_guess(int param_1, int param_2);
};

// FUNCTION: EQMAIN 0x10008090
// Initializes a generic front-end record; its specific role is unclear.
// __thiscall, two int arguments (callee pops 8). Stores 0 at +0x4, the first argument at +0x8, the second at
// +0xc, then the vtable address 0x10040df4 at +0x0. Returns this (EAX = ECX in the original).
EQMain_Record_guess *EQMain_Record_guess::Init_guess(int buffer, int value2)
{
    this->field_4 = 0;
    this->field_8 = buffer;
    this->field_c = value2;
    this->vtbl = (void *)0x10040df4;
    return this;
}

// FUNCTION: EQMAIN 0x100080B0
// Fits front-end text within the available display area.
// __cdecl with six stack arguments (the callee pops 0): the object, the available width and height, the text, a
// size id and a dword. The Ghidra signature shows four; the stack offsets after the vtable call show six. A null
// text returns 0. Else the sub-object at +0xc of the object writes a DC over the text argument (vtable slot 0x44,
// __stdcall). From the size id down, the font is selected (EQMain_SelectFontSize_guess, 0x10008230), the text is
// measured (GetTextExtentPoint32A, import slot 0x10040038) and, once it is narrower than the width and shorter than
// the height, the DC is released (vtable slot 0x68, __stdcall) and the size id is returned.
int __cdecl EQMain_FitFontSize_guess(int drawObject, int availableWidth, int availableHeight, LPCSTR text, int fontSizeId, DWORD fontWeight)
{
    typedef BOOL (__stdcall *GetTextExtentPoint32A_guess_t)(HDC, LPCSTR, int, LPSIZE);
    typedef void (__stdcall *SubVt_guess_t)(void *, void *);
    LPCSTR lpString;
    void *sub;
    SIZE sz;

    lpString = text;
    if (lpString == 0) {
        return 0;
    }
    sub = *(void **)(drawObject + 0xc);
    ((SubVt_guess_t)(*(void ***)sub)[0x44 / 4])(sub, &text);
    for (;;) {
        EQ_CDECL(int, 0x10008230, (HDC, int, DWORD))((HDC)text, fontSizeId, fontWeight);
        EQ_GLOBAL(GetTextExtentPoint32A_guess_t, 0x10040038)((HDC)text, lpString, strlen(lpString), &sz);
        if (availableWidth > sz.cx && availableHeight > sz.cy) {
            sub = *(void **)(drawObject + 0xc);
            ((SubVt_guess_t)(*(void ***)sub)[0x68 / 4])(sub, (HDC)text);
            return fontSizeId;
        }
        fontSizeId--;
    }
}

// The object FontCache_GetOrCreateByParams_guess is called on (g_eqmain_dd_result_guess).
#define g_font_mgr_guess  EQ_GLOBAL(void *, 0x1004CEC0)   // g_eqmain_dd_result_guess

// FUNCTION: EQMAIN 0x10008150
// Selects an Arial font variant for supported front-end languages.
// __cdecl: a dword id, an int and a dword. Ids 1, 2 and 4 get the "Arial" font from FontCache_GetOrCreateByParams_guess (__thiscall on
// g_font_mgr_guess; the int is its first argument, the dword its fifth) with charset 0; ids 8 and 0x10 do the same
// with charset 0x80 and 0x81 (Shift-JIS, Hangul). Any other id returns the dword as it is. The font call's result is
// returned unchanged (its one caller reads EAX).
int __cdecl EQMain_ArialFontForLanguage_guess(DWORD languageId, int fontHeight, DWORD fontWeight)
{
    int fontHandle;

    switch (languageId) {
    case 1:
    case 2:
    case 4:
        fontHandle = EQ_METHOD(14, int, 0x10027BD0, (void *, int, int, int, int, int, int, int, int, int, int, int, int, int, LPCSTR), g_font_mgr_guess)(fontHeight, 0, 0, 0, fontWeight, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
        return fontHandle;
    case 8:
        fontHandle = EQ_METHOD(14, int, 0x10027BD0, (void *, int, int, int, int, int, int, int, int, int, int, int, int, int, LPCSTR), g_font_mgr_guess)(fontHeight, 0, 0, 0, fontWeight, 0, 0, 0, 0x80, 0, 0, 0, 0, "Arial");
        return fontHandle;
    case 0x10:
        fontHandle = EQ_METHOD(14, int, 0x10027BD0, (void *, int, int, int, int, int, int, int, int, int, int, int, int, int, LPCSTR), g_font_mgr_guess)(fontHeight, 0, 0, 0, fontWeight, 0, 0, 0, 0x81, 0, 0, 0, 0, "Arial");
        return fontHandle;
    default:
        return fontWeight;
    }
}

// FUNCTION: EQMAIN 0x10008230
// Selects an Arial font for a front-end text size preset.
// __cdecl, an HDC, a size id and a dword. The id is mapped to a point size (-1 -> 12, 0 -> 14, 1 -> 25, 2 -> 20,
// 3 -> 16; any other id is kept as it is). The "Arial" font of that size comes from FontCache_GetOrCreateByParams_guess (__thiscall on
// g_font_mgr_guess; the dword is its fifth argument), is selected into the DC through the import slot at
// 0x10040028, and the mapped id is returned (no caller reads EAX).
int __cdecl EQMain_SelectFontSize_guess(HDC deviceContext, int fontSizeId, DWORD fontWeight)
{
    typedef HGDIOBJ (__stdcall *SelectObject_guess_t)(HDC, HGDIOBJ);
    HGDIOBJ h;

    switch (fontSizeId) {
    case 1:
        fontSizeId = 0x19;
        break;
    case -1:
        fontSizeId = 0xc;
        break;
    case 0:
        fontSizeId = 0xe;
        break;
    case 2:
        fontSizeId = 0x14;
        break;
    case 3:
        fontSizeId = 0x10;
        break;
    }
    h = EQ_METHOD(14, HGDIOBJ, 0x10027BD0, (void *, int, int, int, int, int, char, char, char, char, char, char, char, char, char *), g_font_mgr_guess)(fontSizeId, 0, 0, 0, fontWeight, 0, 0, 0, 0, 0, 0, 0, 0, EQ_ARRAY(char, 0x100463C4));
    EQ_GLOBAL(SelectObject_guess_t, 0x10040028)(deviceContext, h);
    return fontSizeId;
}

// FUNCTION: EQMAIN 0x100082C0
// Draws front-end text at its supplied screen position.
// __cdecl, seven dwords (the callee pops 0). Draws the NUL-terminated text param_5 at x = x, y = y (TA_LEFT,
// transparent) in colour textColor, with the font of EQMain_SelectFontSize_guess(hdc, fontSizeId, fontWeight). The DC comes from the surface at
// [drawObject + 0xc]: its vtable +0x44 (GetDC, __stdcall) writes the HDC over text's slot, and +0x68 (ReleaseDC, __stdcall)
// gives it back. Returns the width (the cx from GetTextExtentPoint32A); a NULL text returns 0 before any call. The vtable
// callees and the GDI imports pop their own arguments, so all seven dwords are read (the Ghidra five-argument signature
// stops at text).
int __cdecl EQMain_DrawTextRun_guess(int drawObject, int x, int y, COLORREF textColor, LPCSTR text,
                                     int fontSizeId, DWORD fontWeight)
{
    typedef int (__stdcall *GetDC_guess_t)(void *, HDC *);
    typedef int (__stdcall *ReleaseDC_guess_t)(void *, HDC);
    typedef COLORREF (__stdcall *SetTextColor_guess_t)(HDC, COLORREF);
    typedef int (__stdcall *SetBkMode_guess_t)(HDC, int);
    typedef UINT (__stdcall *SetTextAlign_guess_t)(HDC, UINT);
    typedef BOOL (__stdcall *GetTextExtentPoint32A_guess_t)(HDC, LPCSTR, int, LPSIZE);
    typedef BOOL (__stdcall *TextOutA_guess_t)(HDC, int, int, LPCSTR, int);
    void *surface;
    LPCSTR lpString = text;
    SIZE size;

    if (lpString == 0)
        return 0;
    surface = *(void **)((char *)drawObject + 0xc);
    ((GetDC_guess_t)(*(void ***)surface)[0x11])(surface, (HDC *)&text);
    EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)((HDC)text, textColor);
    EQ_GLOBAL(SetBkMode_guess_t, 0x10040014)((HDC)text, 1);
    EQ_GLOBAL(SetTextAlign_guess_t, 0x10040018)((HDC)text, 0);
    EQ_CDECL(int, 0x10008230, (HDC, int, DWORD))((HDC)text, fontSizeId, fontWeight);
    EQ_GLOBAL(GetTextExtentPoint32A_guess_t, 0x10040038)((HDC)text, lpString, strlen(lpString), &size);
    EQ_GLOBAL(TextOutA_guess_t, 0x1004002C)((HDC)text, x, y, lpString, strlen(lpString));
    surface = *(void **)((char *)drawObject + 0xc);
    ((ReleaseDC_guess_t)(*(void ***)surface)[0x1a])(surface, (HDC)text);
    return size.cx;
}

// FUNCTION: EQMAIN 0x10008390
// Draws front-end text centered on a screen position.
// __cdecl, seven dwords (the callee pops 0). Draws the NUL-terminated text param_5 centred on x = centerX (the width
// is the cx from GetTextExtentPoint32A) at y = y, in colour textColor, transparent, TA_LEFT, with the font of
// EQMain_SelectFontSize_guess(hdc, fontSizeId, fontWeight). The DC comes from the surface at [drawObject + 0xc]: its vtable +0x44 (GetDC,
// __stdcall) writes the HDC over text's slot, and +0x68 (ReleaseDC, __stdcall) gives it back. Returns the width;
// a NULL text returns 0 before any call. Both vtable callees pop their two arguments, which is why the arguments up
// to fontWeight are read (the five-argument Ghidra signature assumes they do not). Slots 17 and 26 of the DirectDraw
// surface layout are a guess from the callee shapes.
int __cdecl EQMain_DrawCenteredText_guess(int drawObject, int centerX, int y, COLORREF textColor, LPCSTR text,
                                          int fontSizeId, DWORD fontWeight)
{
    typedef int (__stdcall *GetDC_guess_t)(void *, HDC *);
    typedef int (__stdcall *ReleaseDC_guess_t)(void *, HDC);
    typedef COLORREF (__stdcall *SetTextColor_guess_t)(HDC, COLORREF);
    typedef int (__stdcall *SetBkMode_guess_t)(HDC, int);
    typedef UINT (__stdcall *SetTextAlign_guess_t)(HDC, UINT);
    typedef BOOL (__stdcall *GetTextExtentPoint32A_guess_t)(HDC, LPCSTR, int, LPSIZE);
    typedef BOOL (__stdcall *TextOutA_guess_t)(HDC, int, int, LPCSTR, int);
    void *surface;
    LPCSTR lpString;
    SIZE size;

    lpString = text;
    if (lpString == 0)
        return 0;
    surface = *(void **)((char *)drawObject + 0xc);
    ((GetDC_guess_t)(*(void ***)surface)[0x11])(surface, (HDC *)&text);
    EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)((HDC)text, textColor);
    EQ_GLOBAL(SetBkMode_guess_t, 0x10040014)((HDC)text, 1);
    EQ_GLOBAL(SetTextAlign_guess_t, 0x10040018)((HDC)text, 0);
    EQ_CDECL(int, 0x10008230, (HDC, int, DWORD))((HDC)text, fontSizeId, fontWeight);
    EQ_GLOBAL(GetTextExtentPoint32A_guess_t, 0x10040038)((HDC)text, lpString, strlen(lpString), &size);
    EQ_GLOBAL(TextOutA_guess_t, 0x1004002C)((HDC)text, centerX - size.cx / 2, y, lpString, strlen(lpString));
    surface = *(void **)((char *)drawObject + 0xc);
    ((ReleaseDC_guess_t)(*(void ***)surface)[0x1a])(surface, (HDC)text);
    return size.cx;
}

// g_eqmain_flag_cec4_guess (52 xrefs): the first record made without bit 2 (value 2) of its flags; 0 until one is.
#define g_default_record_guess  EQ_GLOBAL(DWORD *, 0x1004CEC4)
// g_eqmain_dlg_arg_guess (param_8 == 0), g_mode_neg_guess (param_8 < 0) and g_mode_pos_guess (param_8 > 0): stored in record [16].
#define g_mode_zero_guess  EQ_GLOBAL(DWORD, 0x1004CEC8)
#define g_mode_neg_guess   EQ_GLOBAL(DWORD, 0x1004CECC)
#define g_mode_pos_guess   EQ_GLOBAL(DWORD, 0x1004CED0)

// The 100-byte DDBLTFX-sized block that Blt takes: dwSize = 100 at 0, dwROP at 8; the rest is never written.
typedef struct
{
    DWORD dwSize;       // 0x0
    DWORD dwDDFX;       // 0x4 (left uninitialised, as in the original)
    DWORD dwROP;        // 0x8
    DWORD rest[22];     // 0xc
} EQMain_BltFx_guess;

// FUNCTION: EQMAIN 0x10008460
// Creates a drawable surface record attached to its owning front-end object.
// __cdecl, ten arguments (callee pops 0). Returns the new 0xa8-byte record, or 0 when owner is 0 (nothing is
// allocated) or the malloc fails. Record: [0] kind (8 if flags & 4, else 9 if & 2, else 1; the first two set
// mode = 0), [2] owner, [3..6] rect (x, y, x + width, y + height), [8] a malloc'd
// buffer of bufferSize bytes, [9] its size, [10] surface, or else a CreateSurfaceRecord_guess surface (width x height, flags
// 0x10), [11] 1 when surface was given, [12] a second CreateSurfaceRecord_guess surface, [14] timeGetTime(), [16] one of the
// g_mode_* by the sign of mode, [18] value, [19] flags, the rest 0. The fresh surface gets a SRCCOPY Blt
// of the rect from owner's surface (at +0xc) and PackFsHalveSurface16_guess when flags & 8. The record becomes
// g_default_record_guess when that is still 0 and flags & 2 is clear.
DWORD * __cdecl EQMain_CreateSurfaceRecord_guess(int owner, int x, int y, int width, int height,
                                                 size_t bufferSize, int surface, int mode, DWORD value,
                                                 unsigned int flags)
{
    typedef int (__stdcall *Blt_guess_t)(void *, RECT *, DWORD, RECT *, DWORD, EQMain_BltFx_guess *);
    typedef DWORD (__stdcall *timeGetTime_guess_t)(void);
    DWORD *record;
    DWORD *fieldPtr;
    BYTE *buffer;
    int *createdSurface;
    int *surfaceInterface;
    DWORD DVar4;
    int index;
    RECT rect;
    EQMain_BltFx_guess fx;

    if (owner == 0)
    {
        return 0;
    }
    record = (DWORD *)EQ_CDECL(void *, 0x1003342E, (size_t))(0xa8);
    if (record != 0)
    {
        if (flags & 4)
        {
            record[0] = 8;
            mode = 0;
        }
        else if (flags & 2)
        {
            record[0] = 9;
            mode = 0;
        }
        else
        {
            record[0] = 1;
        }
        record[2] = owner;
        record[3] = x;
        record[4] = y;
        record[5] = x + width;
        record[6] = y + height;
        record[7] = 0;
        buffer = (BYTE *)EQ_CDECL(void *, 0x1003342E, (size_t))(bufferSize);
        record[8] = (DWORD)buffer;
        *buffer = 0;
        record[9] = bufferSize;
        if (surface != 0)
        {
            record[10] = surface;
            record[11] = 1;
        }
        else
        {
            createdSurface = EQ_CDECL(int *, 0x10006D80, (int, int, int))(width, height, 0x10);
            record[10] = (DWORD)createdSurface;
            record[11] = 0;
            rect.left = 0;
            rect.top = 0;
            rect.right = width;
            rect.bottom = height;
            fx.dwSize = 100;
            fx.dwROP = 0xcc0020;
            surfaceInterface = (int *)((int *)record[10])[3];
            ((Blt_guess_t *)*surfaceInterface)[5](surfaceInterface, &rect, ((DWORD *)owner)[3], (RECT *)(record + 3), 0x1020000, &fx);
            if (flags & 8)
            {
                EQ_CDECL(void, 0x10007CF0, (int *))((int *)record[10]);
            }
        }
        createdSurface = EQ_CDECL(int *, 0x10006D80, (int, int, int))(width, height, 0x10);
        record[12] = (DWORD)createdSurface;
        record[13] = 0;
        DVar4 = EQ_GLOBAL(timeGetTime_guess_t, 0x10040244)();
        record[14] = DVar4;
        record[15] = 0;
        if (mode > 0)
        {
            record[16] = g_mode_pos_guess;
        }
        else if (mode < 0)
        {
            record[16] = g_mode_neg_guess;
        }
        else
        {
            record[16] = g_mode_zero_guess;
        }
        record[18] = value;
        record[19] = flags;
        record[17] = 0;
        record[20] = 0;
        fieldPtr = record + 21;
        for (index = 0x15; index != 0; index = index + -1)
        {
            *fieldPtr = 0;
            fieldPtr = fieldPtr + 1;
        }
        EQ_CDECL(void, 0x10028340, (DWORD *, int, DWORD *))((DWORD *)(owner + 0x1c), 0, record);
        EQ_CDECL(void, 0x10004EC0, (DWORD *))(record);
        if (g_default_record_guess == 0 && (record[19] & 2) == 0)
        {
            record[13] = 1;
            g_default_record_guess = record;
        }
    }
    return record;
}

// g_eqmain_ceb8_guess (460 xrefs): the first argument of BltFastSrcRect_guess in every draw call; its meaning is unknown.
#define g_eqmain_ceb8_guess  EQ_GLOBAL(int, 0x1004CEB8)

// FUNCTION: EQMAIN 0x10008660
// Draws the full frame around a front-end text box.
// @early-stop: remaining differences are register allocation and stack-slot scheduling around bitmap loads and vertical copies.
// __cdecl, four int arguments (callee pops 0), no return value. Loads three bitmaps by name with PackFsGetNamedImage_guess
// ("...", "...", "..."; record [0] and [1] are passed on as sizes, guessed width and
// height), draws the left cap at (x - 4, y - 2), then tileCount = (width - right width - left width)
// / tile width + 1 tiles (none when tileCount <= 0) at y - 2, then the right cap. Then height / 4 blits of
// 4 rows from (x - 1, y - 1) to (x - 1, y + 3 + 4 * k), and one row from
// (x - 1, y - 2) after them, all tileWidth = tile width * tileCount + right width + left width - 6 wide.
// Frees the three records with PackFsFreeEntry_guess.
void __cdecl EQMain_DrawTextFrame_guess(int x, int rightCap, int width, int height)
{
    int *leftCap;
    int *middleTile;
    int tileCount;
    int tileWidth;
    int frameY;
    int frameX;

    frameX = x + -4;
    frameY = rightCap + -2;
    leftCap = EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046514), 0x10);
    middleTile = EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046520), 0x10);
    rightCap = (int)EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046508), 0x10);
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))(
        g_eqmain_ceb8_guess, frameX, frameY, (int)leftCap, 0, 0, *leftCap, leftCap[1], 1);
    x = *leftCap + frameX;
    tileWidth = *middleTile;
    tileCount = ((width - *(int *)rightCap) - *leftCap) / tileWidth + 1;
    if (0 < tileCount) {
        width = tileCount;
        do {
            EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))(
                g_eqmain_ceb8_guess, x, frameY, (int)middleTile, 0, 0, tileWidth, middleTile[1], 0);
            tileWidth = *middleTile;
            x = x + tileWidth;
            width = width + -1;
        } while (width != 0);
    }
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))(
        g_eqmain_ceb8_guess, x, frameY, rightCap, 0, 0, *(int *)rightCap, ((int *)rightCap)[1], 1);
    x = frameY + 5;
    width = height / 4;
    frameY = frameY + 1;
    frameX = frameX + 3;
    tileWidth = *middleTile * tileCount + *(int *)rightCap + -6 + *leftCap;
    if (0 < width) {
        do {
            EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))(
                g_eqmain_ceb8_guess, frameX, x, g_eqmain_ceb8_guess, frameX, frameY, tileWidth, 4, 0);
            x = x + 4;
            width = width + -1;
        } while (width != 0);
    }
    frameY = frameY + -1;
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))(
        g_eqmain_ceb8_guess, frameX, x, g_eqmain_ceb8_guess, frameX, frameY, tileWidth, 1, 0);
    EQ_CDECL(void, 0x10007A80, (int *))(leftCap);
    EQ_CDECL(void, 0x10007A80, (int *))(middleTile);
    EQ_CDECL(void, 0x10007A80, (int *))((int *)rightCap);
}

// FUNCTION: EQMAIN 0x10008800
// Draws one horizontal frame row around front-end text.
// __cdecl, three int arguments (callee pops 0), no return value. Loads three bitmaps by name with PackFsGetNamedImage_guess
// ("...", "...", "..."; record [0] and [1] are passed on as sizes, guessed width and
// height), draws the left cap at (x - 4, y - 2), then q + 1 tiles where q = (width - right
// width - left width) / tile width (none when q < 0), then the right cap, and frees the records with
// PackFsFreeEntry_guess. y is reused as y - 2 and x as the tile counter, as the original's stack
// slots are (no frame: the function has no locals in memory).
void __cdecl EQMain_DrawTextFrameRow_guess(int x, int y, int width)
{
    int *leftCap;
    int *middleTile;
    int *rightCap;
    int tileCount;
    int tileWidth;
    int tileX;

    x = x + -4;
    y = y + -2;
    leftCap = EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046514), 0x10);
    middleTile = EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046520), 0x10);
    rightCap = EQ_CDECL(int *, 0x10007AE0, (char *, int))(EQ_ARRAY(char, 0x10046508), 0x10);
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
        (g_eqmain_ceb8_guess, x, y, (int)leftCap, 0, 0, *leftCap, leftCap[1], 1);
    tileWidth = *middleTile;
    tileX = x + *leftCap;
    tileCount = ((width - *rightCap) - *leftCap) / tileWidth;
    if (tileCount >= 0) {
        x = tileCount + 1;
        do {
            EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
                (g_eqmain_ceb8_guess, tileX, y, (int)middleTile, 0, 0, tileWidth, middleTile[1], 0);
            tileWidth = *middleTile;
            tileX = tileX + tileWidth;
            x = x + -1;
        } while (x != 0);
    }
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
        (g_eqmain_ceb8_guess, tileX, y, (int)rightCap, 0, 0, *rightCap, rightCap[1], 1);
    EQ_CDECL(void, 0x10007A80, (int *))(leftCap);
    EQ_CDECL(void, 0x10007A80, (int *))(middleTile);
    EQ_CDECL(void, 0x10007A80, (int *))(rightCap);
}

// FUNCTION: EQMAIN 0x100088f0
// Prepares an empty text buffer for front-end text entry.
// __cdecl, one int argument: a pointer to a record (no header defines it yet). When it is not null, a
// buffer of the size at +0x24 is malloc'd, its pointer is stored at +0x3c and its first byte is set to 0
// (an empty string). Nothing is checked: a failed malloc is written through, as in the original.
void __cdecl EQMain_AllocEmptyString_guess(int record)
{
    char *buffer;

    if (record != 0) {
        buffer = (char *)EQ_CDECL(void *, 0x1003342E, (size_t))(*(size_t *)(record + 0x24));
        *(char **)(record + 0x3c) = buffer;
        *buffer = 0;
    }
}

// FUNCTION: EQMAIN 0x10008970
// Inserts text at the current cursor in a front-end text field.
// __cdecl, two arguments: a record and a string, no return value (no caller reads EAX). The record has no header
// yet: the text pointer at +0x20, the capacity at +0x24 and the cursor at +0x1c (a guess: the text from the cursor
// on moves up by the string's length, the string is copied in and the cursor moves past it). A null record or
// string does nothing, and so does an empty string or a text that with its NUL would not stay below the capacity.
// Otherwise 0x10004ec0 is called with the record. Both lengths come from strlen: a buffer with no NUL is read past
// its end, as in the original.
// @early-stop: the shift loop's dec is emitted before the text-len sub (VC6 keeps statement order); 3 insns differ.
void __cdecl EQMain_InsertAtCursor_guess(int *record, char *insertText)
{
    int n;
    int len;

    if ((record != 0) && (insertText != 0)) {
        len = strlen(insertText);
        n = strlen((char *)record[8]);
        if ((len != 0) && (n + len + 1 < record[9])) {
            n = n + len + 1;
            if (n > record[7]) {
                do {
                    n = n - 1;
                    *(char *)(record[8] + 1 + n) = *(char *)((record[8] - len) + 1 + n);
                } while (n > record[7]);
            }
            n = 0;
            if (0 < len) {
                do {
                    *(char *)(record[8] + record[7]) = insertText[n];
                    record[7] = record[7] + 1;
                    n = n + 1;
                } while (n < len);
            }
            EQ_CDECL(void, 0x10004EC0, (int *))(record);
        }
    }
}

#include "trilogy/eqaddr.h"

// The two eqmain globals that may hold the entry being freed (VAs in the comments). FreeEntry clears each one
// that holds it, so neither is left pointing at freed memory.
#define g_cached_entry_a_guess  EQ_GLOBAL(void *, 0x1004CE6C)   // g_cached_entry_a_guess
#define g_cached_entry_b_guess  EQ_GLOBAL(void *, 0x1004CEC4)   // g_eqmain_flag_cec4_guess

// The WINMM timeGetTime import slot (VA in the name): the original calls timeGetTime through this pointer.
typedef DWORD (__stdcall *timeGetTime_guess_t)(void);
#define PTR_timeGetTime_guess     EQ_GLOBAL(timeGetTime_guess_t, 0x10040244)   // PTR_timeGetTime_10040244
#define eqmain_fn_10004ec0_guess  EQ_CDECL(void, 0x10004EC0, (void *))        // DrawQueueRecord_guess (one pointer argument)

// FUNCTION: EQMAIN 0x10008910
// Updates the cached front-end entry and redraws entries when it changes.
// __cdecl, one void * argument, no return value (no caller reads EAX). A null entry does nothing. When the cached-entry
// global b is empty, the entry becomes it and the function returns. Otherwise, when the entry is not that global's value
// and its byte at +0x4c has 0x2 clear, the old cached entry gets +0x34 = 0 and goes to 0x10004ec0; the entry gets
// +0x34 = 1 and the timeGetTime value at +0x38, becomes the cached entry, and goes to 0x10004ec0 too.
void __cdecl EQMain_SetCachedEntry_guess(void *entry)
{
    DWORD DVar1;

    if (entry != 0) {
        if (g_cached_entry_b_guess == 0) {
            g_cached_entry_b_guess = entry;
            return;
        }
        if (g_cached_entry_b_guess != entry && (*((unsigned char *)entry + 0x4c) & 2) == 0) {
            ((int *)g_cached_entry_b_guess)[0xd] = 0;
            eqmain_fn_10004ec0_guess(g_cached_entry_b_guess);
            ((int *)entry)[0xd] = 1;
            DVar1 = PTR_timeGetTime_guess();
            ((DWORD *)entry)[0xe] = DVar1;
            g_cached_entry_b_guess = entry;
            eqmain_fn_10004ec0_guess(entry);
        }
    }
}

// FUNCTION: EQMAIN 0x10008A00
// Removes a front-end entry from its owner and releases its associated resources.
// __cdecl, one void * argument, no return value (no caller reads EAX). A null entry does nothing. Otherwise it
// clears each cached-entry global that holds the entry, unlinks the entry from the list at its owner's +0x1c
// (owner at entry +0x8; the list nodes are three dwords: entry at node +0x4, next at +0x8), hands the entries
// at +0x30 and +0x28 (the latter only when +0x2c is 0) to 0x10007a80, frees the block at +0x20 (unchecked: a
// null is passed to _free) and the one at +0x3c (when set), then frees the entry itself.
void __cdecl EQMain_FreeEntry_guess(void *entry)
{
    int *node;

    if (entry != 0) {
        if (entry == g_cached_entry_a_guess) {
            g_cached_entry_a_guess = 0;
        }
        if (g_cached_entry_b_guess == entry) {
            g_cached_entry_b_guess = 0;
        }
        for (node = *(int **)(*(int *)((char *)entry + 0x8) + 0x1c); node != 0;
             node = (int *)node[2]) {
            if (entry == (void *)node[1]) {
                EQ_CDECL(int, 0x10028430, (int *, int *))((int *)(*(int *)((char *)entry + 0x8) + 0x1c), node);
                break;
            }
        }
        EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((char *)entry + 0x30));
        if (*(int *)((char *)entry + 0x2c) == 0) {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((char *)entry + 0x28));
        }
        EQ_CDECL(void, 0x10033568, (void *))(*(void **)((char *)entry + 0x20));
        if (*(void **)((char *)entry + 0x3c) != 0) {
            EQ_CDECL(void, 0x10033568, (void *))(*(void **)((char *)entry + 0x3c));
        }
        EQ_CDECL(void, 0x10033568, (void *))(entry);
    }
    return;
}

// FUNCTION: EQMAIN 0x10008AA0
// Clears a front-end text field and returns its cursor to the start.
// __cdecl, one dword * argument (the record), no return value (no caller reads EAX). Clears the byte that the
// record's +0x20 pointer holds, zeroes the dword at +0x1c, then passes the record to 0x10004ec0.
void __cdecl EQMain_ResetRecord_guess(int *record)
{
    *(char *)record[8] = 0;
    record[7] = 0;
    EQ_CDECL(void, 0x10004EC0, (int *))(record);
}

// FUNCTION: EQMAIN 0x10008AC0
// Sets the display color for a coded front-end text segment.
// __cdecl, an HDC and a char code (read signed, MOVSX). Sets the text color for the letter codes b, g, k, o, r, w,
// x and y (k sets black and falls through to the end); any other code sets nothing. No return value (no caller
// reads EAX). SetTextColor is called through its import slot at 0x1004003c.
void __cdecl EQMain_SetTextColorFromCode_guess(HDC deviceContext, char colorCode)
{
    typedef COLORREF (__stdcall *SetTextColor_guess_t)(HDC, COLORREF);

    switch (colorCode) {
    case 'w':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0xf3f2f2);
        return;
    case 'y':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0x3cd9f5);
        return;
    case 'o':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0x22650);
        return;
    case 'r':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0x1818f0);
        return;
    case 'g':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0x6410);
        return;
    case 'b':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0xc81000);
        return;
    case 'x':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0x626308);
        return;
    case 'k':
        EQ_GLOBAL(SetTextColor_guess_t, 0x1004003C)(deviceContext, 0);
        break;
    }
    return;
}

// FUNCTION: EQMAIN 0x10008bb0
// Maps known front-end text colors back to their formatting letters.
// __cdecl, one uint argument. Returns a character in AL (the callers read AL only): a letter for each
// of a few known codes, a space for everything else. The upper bits of EAX keep the argument.
char __cdecl EQMain_CodeToChar_guess(unsigned int colorValue)
{
    if (colorValue <= 0x3cd9f5) {
        if (colorValue != 0x3cd9f5) {
            if (colorValue <= 0x22650) {
                if (colorValue != 0x22650) {
                    if (colorValue != 0) {
                        if (colorValue == 0x6410)
                            return 'g';
                    }
                    else {
                        return 'k';
                    }
                }
                else {
                    return 'o';
                }
            }
            else if (colorValue == 0x1818f0) {
                return 'r';
            }
        }
        else {
            return 'y';
        }
    }
    else {
        switch (colorValue) {
        case 0x626308:
            return 'x';
        case 0xc81000:
            return 'b';
        case 0xf3f2f2:
            return 'w';
        }
    }
    return ' ';
}

// eqmain.dll's own globals and functions, by VA (eqaddr.h: each call is a direct `call va`)
#define g_row_kind_guess                EQ_GLOBAL(unsigned int, 0x1004CEC8)
#define g_parent_guess                EQ_GLOBAL(unsigned int, 0x1004CEB8)
#define eqmain_malloc_guess             EQ_CDECL(void *, 0x1003342E, (unsigned int))
#define EQMain_ClearRecord3_guess       EQ_CDECL(void, 0x10028330, (unsigned int *))
#define eqmain_fn_10006d80_guess       EQ_CDECL(int *, 0x10006D80, (int, int, int))
#define EQMain_FUN_10006c20_guess       EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, int))

// FUNCTION: EQMAIN 0x10008c10
// Creates a drawable text block within a front-end screen region.
// __cdecl, nine arguments. Returns a new 0x44-byte record, or NULL when malloc fails (returned as it is).
// The record: +0 value; a rect from +4 x, +8 y to +0xc x + width, +0x10 y + height;
// +0x14 fieldValue; +0x1c cleared by ClearRecord3_guess; +0x38 mode; +0x3c 0xe when mode equals
// g_eqmain_dlg_arg_guess, else left as malloc returned it (bug kept); +0x2c (bottom - top) / +0x3c; +0x30 the
// CreateSurfaceRecord_guess(width, height, 0x10) result, also passed to BltFastSrcRect_guess; +0x34 optionValue; +0x40 extraValue.
unsigned int * __cdecl EQMain_CreateRecord_guess(unsigned int value, int x, int y, int width, int height,
                                        unsigned int fieldValue, unsigned int optionValue, int mode, unsigned int extraValue)
{
    unsigned int *record;
    int *buffer;

    record = (unsigned int *)eqmain_malloc_guess(0x44);
    if (record != 0) {
        record[0] = value;
        record[1] = x;
        record[2] = y;
        record[3] = x + width;
        record[4] = y + height;
        record[5] = fieldValue;
        record[6] = 0;
        EQMain_ClearRecord3_guess(record + 7);
        record[0xe] = mode;
        if (mode == g_row_kind_guess) {
            record[0xf] = 0xe;
        }
        record[10] = 0;
        record[0xb] = (int)(record[4] - record[2]) / (int)record[0xf];
        buffer = eqmain_fn_10006d80_guess(width, height, 0x10);
        record[0xc] = (unsigned int)buffer;
        EQMain_FUN_10006c20_guess((int)buffer, 0, 0, g_parent_guess, x, y, width, height, 0);
        record[0xd] = optionValue;
        record[0x10] = extraValue;
    }
    return record;
}
