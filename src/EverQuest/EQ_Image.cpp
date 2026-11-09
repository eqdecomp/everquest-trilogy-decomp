// EQ_Image.cpp - EQ_Image, the client's image/overlay-rect object.
// Field layout: unrecovered (a dword at +0, a RECT at +4); all accesses stay raw offsets.

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/eq_columns.h"
#include "trilogy/eq_image.h"
#include "trilogy/display.h"
#include "trilogy/globals.h"

#pragma comment(lib, "user32.lib")     // CopyRect, SetRect

/* USER32 rect helpers without <windows.h>; the imports resolve to user32.dll on both sides. */
extern "C" __declspec(dllimport) int __stdcall CopyRect(RECT* dest, const RECT* src);
extern "C" __declspec(dllimport) int __stdcall SetRect(RECT* rect, int left, int top, int right, int bottom);

// FUNCTION: TRILOGY 0x00438423
// Draws the requested image by id, falling back to a default image when unavailable.
// ORIGINAL FILE: EverQuest/EQ_Image.cpp (from Mac debug map)
void DrawImage(u32 arg, int imageId)
{
    EQ_Image_guess* image;

    if (imageId < 0 || imageId >= 0xBBA)
        imageId = 0x1F4;
    image = EQ_ARRAY(EQ_Image_guess*, 0x006B32B4)[imageId];
    if (image == 0)
        imageId = 0x1F4;
    image = EQ_ARRAY(EQ_Image_guess*, 0x006B32B4)[imageId];
    EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), image)(arg, 1);
}

// FUNCTION: TRILOGY 0x00438458
// Initializes an image overlay from a source descriptor's image data and rectangle.
// ORIGINAL FILE: EverQuest/EQ_Image.cpp (inferred from Mac-mapped neighbors 00438423 and 00438475)
EQ_Image_guess::EQ_Image_guess(RECT* source)
{
    *(u32*)this = *(u32*)((char*)source + 0x10);
    CopyRect((RECT*)((char*)this + 4), source);
}

// FUNCTION: TRILOGY 0x00438475
// Like the two-argument draw, but passes the stored rect with its right edge minus 1; args 2 and 3 are unused.
// ORIGINAL FILE: EverQuest/EQ_Image.cpp (from Mac debug map)
void EQ_Image_guess::draw_guess(u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
    RECT rc;

    SetRect(&rc, *(s32*)((char*)this + 4), *(s32*)((char*)this + 8),
            *(s32*)((char*)this + 0xC) - 1, *(s32*)((char*)this + 0x10));
    /* display.h:242 DrawOverlay_guess(void* overlay, int arg2, int* pos, int unused); `this` ignored. */
    EQ_METHOD(4, void, 0x0040B492, (CDisplay*, void*, void*, u32, u32), (CDisplay*)g_display_guess)((void*)*(u32*)this, (void*)&rc, arg1, arg4);
}

// FUNCTION: TRILOGY 0x004384B2
// Draws the image: passes its handle (+0) and stored rect (+4) plus the caller's rect and flag to CDisplay.
// ORIGINAL FILE: EverQuest/EQ_Image.cpp (from Mac debug map)
void EQ_Image_guess::draw_guess(u32 arg1, u32 arg2)
{
    EQ_METHOD(4, void, 0x0040B492, (CDisplay*, void*, void*, u32, u32), (CDisplay*)g_display_guess)((void*)*(u32*)this, (void*)((char*)this + 4), arg1, arg2);
}

// FUNCTION: TRILOGY 0x004384ce
// Refreshes an image overlay's image data and rectangle from a source descriptor.
// ORIGINAL FILE: EverQuest/EQ_Image.cpp (inferred from Mac-mapped neighbors 00438423 and 00438475)
void EQ_Image_guess::UpdateImageRect_guess(RECT* source)
{
    *(u32*)this = *(u32*)((char*)source + 0x10);
    CopyRect((RECT*)((char*)this + 4), source);
}

typedef int (__stdcall *SetRectProc_guess)(RECT*, int, int, int, int);

#ifdef EQD_MATCH      /* not in the generated match header (0x006e4e1c is) */
extern char eqd_va_0x006E4E2C;
extern char eqd_va_0x006E4E3C;
#endif

class EQ_ImageProgressOwner_guess
{
public:
    void UpdateProgress_guess(u32 current, u32 maximum);
};

// FUNCTION: TRILOGY 0x0046a177
// Updates a progress bar for the active interface layout; the tracked in-game value is unclear.
void EQ_ImageProgressOwner_guess::UpdateProgress_guess(u32 current, u32 maximum)
{
    RECT rc;
    unsigned __int64 wideCurrent;
    unsigned __int64 wideMaximum;
    double progress;
    u32 width;
    int left;
    int top;
    int right;
    int bottom;
    SetRectProc_guess setRect;

    if (maximum > 0)
    {
        wideCurrent = current;
        wideMaximum = maximum;
        progress = (double)wideCurrent * EQ_GLOBAL(double, 0x00535588) / (double)wideMaximum;
    }
    else
    {
        progress = 0.0;
    }
    width = (u32)progress;
    if (width > 0x14a)
        width = 0x14a;

    if (EQ_GLOBAL(u32, 0x0055A128) != current || EQ_GLOBAL(u32, 0x0055A12C) != maximum)
        *(u32*)((char*)this + 0xeb8) = 1;
    EQ_GLOBAL(u32, 0x0055A128) = current;
    EQ_GLOBAL(u32, 0x0055A12C) = maximum;

    if (*(u8*)((char*)this + 0xfe0) == 6)
    {
        setRect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
        setRect(&rc, 0x223, 0x60, 0x264, 0x65);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B3610))((u32)&rc, 1);

        setRect(&rc, 0x222, 0x60, 0x264, 0x65);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E2C) + 4);
        setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0x222, 0x60, width / 5 + 0x223, 0x65);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B341C))((u32)&rc, 1);
        bottom = 0x65;
        right = 0x264;
        top = 0x62;
        left = 0x223;
        setRect(&rc, left, top, right, bottom);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4);
    }
    else if (g_game_state_guess == 2)
    {
        int x;
        int y;
        int barTop;
        int barLeft;
        int barRight;

        x = *(int*)((char*)this + 0x90184);
        *(u32*)((char*)this + 0x90194) = 0;
        *(u32*)((char*)this + 0x90194) = 0;
        y = *(int*)((char*)this + 0x90188);
        barTop = y + 0x14;
        barRight = x + 299;
        barLeft = y + 0x7d;
        setRect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
        setRect(&rc, x + 0x108, barLeft, barRight, y + 0x82);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4);
        setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0x91, 0x145, 0xd2, 0x14a);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B341C))((u32)&rc, 1);

        setRect(&rc, x + 0x108, barLeft, barRight, y + 0x82);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4);
        setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0x91, 0x14c, width / 5 + 0x91, 0x151);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B341C))((u32)&rc, 1);

        setRect(&rc, x + 0x109, barTop + 0x6b, barRight, barTop + 0x6b);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4);
    }
    else
    {
        setRect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
        setRect(&rc, 0x1b, 0xc3, 0x5c, 200);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B3610))((u32)&rc, 1);

        setRect(&rc, 0x1a, 0xc3, 0x5c, 200);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E3C) + 4);
        setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0x102, 0xa2, width / 5 + 0x103, 0xa7);
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B341C))((u32)&rc, 1);
        bottom = 200;
        right = 0x5d;
        top = 0xc5;
        left = 0x1b;
        setRect(&rc, left, top, right, bottom);
        *(u32*)EQ_GLOBAL(void*, 0x006B341C) = *(u32*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4);
    }

    setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0xd2, 0x124, width % 0x42 + 0xd2, 0x125);
    EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), (EQ_Image_guess*)EQ_GLOBAL(void*, 0x006B341C))((u32)&rc, 1);
}
