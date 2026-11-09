// EQ_DragItem.cpp - EQ_DragItem, the client's drag-item object.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_DragItem.cpp (from Mac debug map)
// Field layout: unrecovered (rectangles at +0x24 and +0x84); all accesses stay raw offsets.

#include "trilogy/common.h"
#include "trilogy/eq_columns.h"
#include "trilogy/eq_dragitem.h"
#include "trilogy/eq_image.h"
#include "trilogy/character.h"
#include "trilogy/eqaddr.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/display.h"
#include "trilogy/eq_textio.h"
#include "trilogy/layout/eq_bag.h"

#pragma comment(lib, "user32.lib")     // SetRect

/* USER32 SetRect without <windows.h>; the import resolves to user32.dll on both sides. */
extern "C" __declspec(dllimport) int __stdcall SetRect(RECT* rect, int left, int top, int right, int bottom);

// FUNCTION: TRILOGY 0x004301b1
// Overlay-derived destructor (caller frees it as oldCharacterSelect): runs 0x00450463, then ~EQ_Overlay.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
void __fastcall EQ_DragItem_dtor_guess(EQ_DragItem *self, int)
{
    try
    {
        *(DWORD *)self = 0x005353c4;
        EQ_METHOD(0, void, 0x00450463, (EQ_DragItem *), self)();
        EQ_METHOD(0, void, 0x0045005D, (EQ_DragItem *), self)();
    }
    catch (...)
    {
        throw;
    }
    return;
}

// FUNCTION: TRILOGY 0x004346b9
// Defines the source image regions used by the drag-item display.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
// @early-stop: SetRect slot re-read at each call (original loads it once into EDI), so 600 and 0x26f take EDI/EBX; thiscall dummy-EDX xors and lea/ecx order differ.
/* fake match: setRect is a reference to the SetRect slot, not a local pointer; a local spills to a stack slot here (frame 0x14), a reference takes no storage. */
void EQ_DragItem::DefineImages_guess(void)
{
    typedef int (__stdcall *SetRectFunction_guess)(RECT *, int, int, int, int);
    RECT imageBounds;
    SetRectFunction_guess &setRect = EQ_GLOBAL(SetRectFunction_guess, 0x00534248);

    setRect(&imageBounds, 600, 0x118, 0x263, 300);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xC8, this->displayOverlay_guess, &imageBounds);
    setRect(&imageBounds, 600, 0x140, 0x26F, 0x154);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xC9, this->displayOverlay_guess, &imageBounds);
    setRect(&imageBounds, 600, 0x168, 0x26F, 0x17C);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xCA, this->displayOverlay_guess, &imageBounds);
    setRect(&imageBounds, 600, 400, 0x26F, 0x1A4);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xCB, this->displayOverlay_guess, &imageBounds);
    setRect(&imageBounds, 600, 0x1B8, 0x26F, 0x1CC);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xCC, this->displayOverlay_guess, &imageBounds);
    setRect(&imageBounds, 0x230, 0x1B8, 0x247, 0x1CC);
    EQ_METHOD(3, void, 0x004502A1, (EQ_DragItem *, int, void *, RECT *), this)(0xCD, this->displayOverlay_guess, &imageBounds);
    return;
}

// FUNCTION: TRILOGY 0x00434806
// Keeps the drag-item display on-screen and updates its two rectangles.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
void EQ_DragItem::CalcRedraw2()
{
    int x;
    int y;
    int maxY;

    x = g_dragItemX_guess;
    y = g_dragItemY_guess;
    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (g_game_state_guess == 2)
    {
        if (x > (g_screenWidth - 12))
            x = g_screenWidth + -12;
        maxY = g_screenHeight_guess - 21;
    }
    else
    {
        if (0x274 < x)
            x = 0x274;
        maxY = 0x1CB;
    }
    if (y > maxY)
        y = maxY;
    class EQ_DragItem *const temp = this;
    SetRect((RECT*)((char*)temp + 0x24), x, y, x + 12, y + 21);
    SetRect((RECT*)((char*)temp + 0x84), x, y, x + 12, y + 21);
}

// FUNCTION: TRILOGY 0x0043488c
// Renders the drag-item image at its current screen position.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
void EQ_DragItem::draw2_guess()
{
    EQ_Image_guess* image;

    image = EQ_GLOBAL(EQ_Image_guess*, 0x006B35D4);
    EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, u32), image)((u32)((char*)this + 0x84), 1);
}

// FUNCTION: TRILOGY 0x004348a1
// Sizes and places the dragged item/coin rectangle, clamped to the screen, and flags the rects it overlaps
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
// @early-stop: EBP frame and register allocation still differ
void __fastcall EQ_DragItem_CalcRedraw_guess(void* self, int)
{
    typedef BOOL (WINAPI *CopyRectProc_guess)(LPRECT, const RECT*);
    typedef BOOL (WINAPI *SetRectProc_guess)(LPRECT, int, int, int, int);
    typedef BOOL (WINAPI *PtInRectProc_guess)(const RECT*, POINT);
    PtInRectProc_guess pt_in_rect;
    CopyRectProc_guess copy_rect;
    RECT* lprc;
    POINT point;
    POINT point2;
    POINT point3;
    char loot_result;
    int value;
    int width;
    int height;
    int max_height;
    int result;
    int overlapFlagOffset;
    short groupIndex;
    int slot_offset;
    int slot_rect_offset;

    if (EQ_GLOBAL(char*, 0x006E4E44) != 0)
    {
        groupIndex = (short)pinstEQ_Main_guess->screenMode_guess;
    }
    copy_rect = EQ_GLOBAL(CopyRectProc_guess, 0x00534240);
    copy_rect((LPRECT)((char*)self + 0xA4), (RECT*)((char*)self + 0x94));
    copy_rect((LPRECT)((char*)self + 0x94), (RECT*)((char*)self + 0x24));
    width = g_dragItemX_guess;
    height = g_dragItemY_guess;
    if (((EQ_GLOBAL(char*, 0x006E4E44) != 0) &&
         (pinstEQ_Main_guess->cursorSpell_guess <= 1)) &&
        (g_cdisplay_guess->itemOnCursor_guess == '\x01') &&
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] == (EQ_Item*)0))
    {
        g_cdisplay_guess->itemOnCursor_guess = 0;
    }
    if (g_cdisplay_guess->itemOnCursor_guess == '\0')
    {
        *(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) = 0;
        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->platinum_cursor > 0)
        {
            *(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) = 1;
        }
        else if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->gold_cursor > 0)
        {
            *(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) = 2;
        }
        else if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->silver_cursor > 0)
        {
            *(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) = 3;
        }
        else if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->copper_cursor > 0)
        {
            *(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) = 4;
        }
    }
    if ((EQ_GLOBAL(int, 0x006B676C) == 0) &&
        ((value = EQ_CDECL(u32, 0x004A461E, (void))(),
          2000 < (u32)(value - EQ_GLOBAL(u32, 0x006B6764)))))
    {
        EQ_GLOBAL(u32, 0x006B6764) = EQ_CDECL(u32, 0x004A461E, (void))();
        EQ_GLOBAL(int, 0x006B676C) = EQ_GLOBAL(int, 0x006B676C) + 1;
    }
    if (EQ_GLOBAL(int, 0x006B676C) >= 4)
    {
        if (EQ_GLOBAL(char*, 0x006E4E44) != 0)
        {
            slot_offset = (short)groupIndex * 0x640 + 0x8400C;
            slot_rect_offset = 400;
            do
            {
                *(u32*)(slot_offset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 0;
                slot_offset = slot_offset + 4;
                slot_rect_offset = slot_rect_offset - 1;
            } while (slot_rect_offset != 0);
        }
        return;
    }
    {
        if ((g_cdisplay_guess->itemOnCursor_guess != '\0') ||
            (*(char*)(EQ_GLOBAL(char*, 0x006E4DE0) + 0x3D) != '\0'))
        {
            g_cdisplay_guess->dragArmed_guess = 0;
        }
        if (width < 0)
        {
            width = 0;
        }
        if (height < 0)
        {
            height = 0;
        }
        if (g_game_state_guess == 2)
        {
            if (g_screenWidth - 0xC < width)
            {
                width = g_screenWidth - 0xC;
            }
            max_height = g_screenHeight_guess - 0x15;
        }
        else
        {
            if (0x274 < width)
            {
                width = 0x274;
            }
            max_height = 0x1CB;
        }
        if (max_height < height)
        {
            height = max_height;
        }
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)((LPRECT)((char*)self + 0x24), width, height, width + 0xC, height + 0x15);
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)((LPRECT)((char*)self + 0x84), width, height, width + 0xC, height + 0x15);
        value = 0x29;
        overlapFlagOffset = 0x29;
        if (g_cdisplay_guess->itemOnCursor_guess == '\x01')
        {
            if ((EQ_GLOBAL(char*, 0x006E4E44) != 0) &&
                (pinstEQ_Main_guess->cursorSpell_guess < 2))
            {
                if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0]->type == '\x01')
                {
                    loot_result = EQ_CDECL(char, 0x00434096, (void))();
                    if ((loot_result != '\0') &&
                        ((pinstEQ_Bag1_guess->moving_guess != 0) ||
                         (pinstEQ_Bag2_guess->moving_guess != 0)))
                    {
                        value = 0x104;
                        overlapFlagOffset = 0x96;
                        pinstEQ_Bag1_guess->containerDragConflict_guess = 1;
                        pinstEQ_Bag2_guess->containerDragConflict_guess = 1;
                    }
                }
                else
                {
                    result = EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0]);
                    if (result != 0)
                    {
                        value = 0x33;
                        overlapFlagOffset = value;
                    }
                }
            }
        }
        else if (g_cdisplay_guess->dragArmed_guess != '\0')
        {
            value = 0x32;
            overlapFlagOffset = value;
        }
        if (width < 0)
        {
            width = 0;
        }
        if (height < 0)
        {
            height = 0;
        }
        if (g_game_state_guess == 2)
        {
            max_height = g_screenHeight_guess;
            if (g_screenWidth - value < width)
            {
                width = g_screenWidth - value;
            }
        }
        else
        {
            if (0x280 - value < width)
            {
                width = 0x280 - value;
            }
            max_height = 0x1E0;
        }
        if (max_height - overlapFlagOffset < height)
        {
            height = max_height - overlapFlagOffset;
        }
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)((LPRECT)((char*)self + 0x24), width, height, value + width, overlapFlagOffset + height);
        if ((g_game_state_guess != 2) && (EQ_GLOBAL(char*, 0x006E4E44) != 0))
        {
            slot_rect_offset = (short)groupIndex * 0x1900;
            overlapFlagOffset = (short)groupIndex * 0x640 + 0x8400C;
            groupIndex = 400;
            pt_in_rect = EQ_GLOBAL(PtInRectProc_guess, 0x0053423C);
            do
            {
                *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 0;
                if (*(int*)(slot_rect_offset + 0x6C910 + (int)EQ_GLOBAL(char*, 0x006E4E44)) != -1)
                {
                    lprc = (RECT*)((char*)self + 0xA4);
                    if (pt_in_rect((RECT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44)), *(POINT*)((char*)self + 0xA4)))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point.y = *(LONG*)((char*)self + 0xA8);
                    point.x = *(LONG*)((char*)self + 0xAC);
                    if (pt_in_rect((RECT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44)), point))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point2.x = lprc->left;
                    point2.y = *(LONG*)((char*)self + 0xB0);
                    if (pt_in_rect((RECT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44)), point2))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    if (pt_in_rect((RECT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44)), *(POINT*)((char*)self + 0xAC)))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point2.x = (lprc->left + *(int*)((char*)self + 0xAC)) / 2;
                    point2.y = (*(int*)((char*)self + 0xA8) + *(int*)((char*)self + 0xB0)) / 2;
                    if (pt_in_rect((RECT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44)), point2))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    if (pt_in_rect(lprc, *(POINT*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44))))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point.x = *(LONG*)(slot_rect_offset + 0x6C914 + (int)EQ_GLOBAL(char*, 0x006E4E44));
                    point.y = *(LONG*)(slot_rect_offset + 0x6C910 + (int)EQ_GLOBAL(char*, 0x006E4E44));
                    if (pt_in_rect(lprc, point))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point2.x = *(LONG*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44));
                    point2.y = *(LONG*)(slot_rect_offset + 0x6C918 + (int)EQ_GLOBAL(char*, 0x006E4E44));
                    if (pt_in_rect(lprc, point2))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    if (pt_in_rect(lprc, *(POINT*)(slot_rect_offset + 0x6C914 + (int)EQ_GLOBAL(char*, 0x006E4E44))))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                    point3.x = (*(int*)(slot_rect_offset + 0x6C914 + (int)EQ_GLOBAL(char*, 0x006E4E44)) +
                                *(int*)(slot_rect_offset + 0x6C90C + (int)EQ_GLOBAL(char*, 0x006E4E44))) / 2;
                    point3.y = (*(int*)(slot_rect_offset + 0x6C910 + (int)EQ_GLOBAL(char*, 0x006E4E44)) +
                                *(int*)(slot_rect_offset + 0x6C918 + (int)EQ_GLOBAL(char*, 0x006E4E44))) / 2;
                    if (pt_in_rect(lprc, point3))
                    {
                        *(u32*)(overlapFlagOffset + (int)EQ_GLOBAL(char*, 0x006E4E44)) = 1;
                    }
                }
                overlapFlagOffset = overlapFlagOffset + 4;
                slot_rect_offset = slot_rect_offset + 0x10;
                groupIndex = groupIndex - 1;
            } while (groupIndex != 0);
        }
    }
}

#define DI_P4E44            EQ_GLOBAL(char *, 0x006E4E44)
#define DI_P4DE0            EQ_GLOBAL(char *, 0x006E4DE0)
#define DI_PC               EQ_GLOBAL(CharacterProfile *, 0x006E4DA8)
#define DI_SLOT             (pinstEQ_Main_guess->dragValue_guess)
#define DI_SPRITE           EQ_GLOBAL(int *, 0x006B341C)
#define DI_RES_4E40         (*(int *)(EQ_GLOBAL(char *, 0x006E4E40) + 4))
#define DI_RES_4DE8         (pinstEQ_TextIO_guess->displayOverlay_int_guess)
#define DI_RES_4E3C         (*(int *)(EQ_GLOBAL(char *, 0x006E4E3C) + 4))
#define DI_SELF_RECT        ((void *)((char *)self + 0x24))
#define DI_SELF_Y           (*(int *)((char *)self + 0x28))
#define DI_ICON(idx)        EQ_ARRAY(void *, 0x006B32B4)[(idx)]
#define DI_STACK(item)      ((int)*(unsigned char *)((char *)(item) + 0xDA) - 1) / \
                            (int)*(signed char *)((char *)(item) + 0xBB) + 1
#define DI_DRAW(image, rect) \
    EQ_METHOD(4, void, 0x00438475, (void *, void *, int, int, int), (image))((rect), 1, 0xB4, 1)
#define DI_TEXT(text, x, y, size, color) \
    EQ_METHOD(6, void, 0x0048D21A, (void *, const char *, int, int, short, int, int), EQ_GLOBAL(void *, 0x006E4E68))( (text), (x), (y), (size), (color), 1)
#define DI_PLACE(res, l, t, r, b) \
    { \
        *DI_SPRITE = (res); \
        SetRect((RECT *)(DI_SPRITE + 1), (l), (t), (r), (b)); \
    }
#define DI_PLACE_DRAW(res, l, t, r, b) \
    { \
        DI_PLACE(res, l, t, r, b) \
        DI_DRAW(DI_SPRITE, DI_SELF_RECT); \
    }

// FUNCTION: TRILOGY 0x00434de2
// Draws what the cursor is dragging (this = EQ_DragItem): item, coins, or a button/gem.
// Draws the base image first unless 0x006b645a is set, then returns early if the merchant window is open.
// Byte 0x3c of the object at 0x006e4de0: the cursor item icon (inventory_items[0]) with a stack count, or a spell.
// Byte 0x3d: the cursor coins by type 1-4, drawing the coin image and the amount from the profile.
// Byte 0x3e: a dragged button by dragType_guess 1-7, each with its own image and text.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
void EQ_DragItem::draw_guess(void)
{
    void * self = (void *)this;
    short width;
    int *destRect;
    unsigned int len;
    RECT rect;
    char text[64];
    int *dragRect;
    char *strPtr;
    char ch;
    short value;

    if (EQ_GLOBAL(char, 0x006B645A) == 0)
        EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess *, u32, u32), EQ_GLOBAL(EQ_Image_guess *, 0x006B35D4))((u32)((char *)self + 0x84), 1);
    EQ_GLOBAL(char, 0x006B658D) = 0;
    if (DI_P4E44 != 0 && pinstEQ_Main_guess->merchantOpen_guess != 0)
        return;
    if (DI_P4DE0[0x3C] == 1)
    {
        if (pinstEQ_Main_guess->cursorSpell_guess <= 1)
        {
            if (DI_PC->inventory_items[0] == 0)
                return;
            dragRect = (int *)((char *)self + 0x24);
            if (DI_PC->inventory_items[0]->icon_nr >= 500 && DI_PC->inventory_items[0]->icon_nr <= 3002)
                DI_DRAW(DI_ICON(DI_PC->inventory_items[0]->icon_nr), dragRect);
            else
                DI_DRAW(EQ_GLOBAL(void *, 0x006B3E30), dragRect);
            if (DI_P4E44 == 0)
                return;
            if (EQ_CDECL(int, 0x004A1D83, (void *))(DI_PC->inventory_items[0]) == 0)
                return;
            SetRect(&rect, *dragRect + 0x15, DI_SELF_Y + 0x1A, *(int *)((char *)self + 0x2C),
                    *(int *)((char *)self + 0x30));
            EQ_METHOD(2, void, 0x004384B2, (void *, void *, int), EQ_GLOBAL(void *, 0x006B37FC))(&rect, 1);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_STACK(DI_PC->inventory_items[0]));
            DI_TEXT(text, *dragRect + 0x1A, DI_SELF_Y + 0x1F, 10, EQ_GLOBAL(int, 0x006EFDF4));
            return;
        }
        if (pinstEQ_Main_guess->cursorSpell_guess < 1000)
            return;
        DI_DRAW(EQ_GLOBAL(void *, 0x006B51F4), DI_SELF_RECT);
        return;
    }
    if (DI_P4DE0[0x3D] != 0)
    {
        switch (DI_P4DE0[0x3D])
        {
        case 1:
            dragRect = (int *)((char *)self + 0x24);
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3CC4), dragRect);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_PC->platinum_cursor);
            break;
        case 2:
            dragRect = (int *)((char *)self + 0x24);
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3CC8), dragRect);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_PC->gold_cursor);
            break;
        case 3:
            dragRect = (int *)((char *)self + 0x24);
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3CCC), dragRect);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_PC->silver_cursor);
            break;
        case 4:
            dragRect = (int *)((char *)self + 0x24);
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3CD0), dragRect);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_PC->copper_cursor);
            break;
        default:
            return;
        }
        DI_TEXT(text, *dragRect + 10, DI_SELF_Y + 10, 0, EQ_GLOBAL(int, 0x006EFDFC));
        return;
    }
    if (DI_P4DE0[0x3E] == 0)
        return;
    if (pinstEQ_Main_guess->dragType_guess == 1)
    {
        switch (DI_SLOT)
        {
        case 0:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3694), DI_SELF_RECT);
            if (DI_PC->inventory_items[13] == 0)
                return;
            DI_DRAW(DI_ICON(DI_PC->inventory_items[13]->icon_nr), DI_SELF_RECT);
            return;
        case 1:
            destRect = (int *)((char *)self + 0x24);
            DI_DRAW(EQ_GLOBAL(void *, 0x006B3698), destRect);
            if (DI_PC->inventory_items[11] == 0)
                return;
            DI_DRAW(DI_ICON(DI_PC->inventory_items[11]->icon_nr), destRect);
            if (EQ_CDECL(int, 0x004A1D83, (void *))(DI_PC->inventory_items[11]) == 0)
                return;
            SetRect(&rect, *destRect + 0x15, DI_SELF_Y + 0x1A, *(int *)((char *)self + 0x2C),
                    *(int *)((char *)self + 0x30));
            DI_DRAW(EQ_GLOBAL(void *, 0x006B37FC), &rect);
            EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                (text, (const char *)0x0053E16C, DI_STACK(DI_PC->inventory_items[11]));
            DI_TEXT(text, *destRect + 0x1A, DI_SELF_Y + 0x1F, 10, EQ_GLOBAL(int, 0x006EFDF4));
            return;
        default:
            return;
        }
    }
    else if (pinstEQ_Main_guess->dragType_guess == 2)
    {
        switch (DI_SLOT)
        {
        case 0:
            value = pinstEQ_Main_guess->combatSlots_guess[0];
            if (value == -1)
            {
                DI_PLACE_DRAW(DI_RES_4E40, 8, 0xD8, 0x30, 0x100)
                return;
            }
            break;
        case 1:
            value = pinstEQ_Main_guess->combatSlots_guess[1];
            if (value == -1)
            {
                DI_PLACE_DRAW(DI_RES_4E40, 0x39, 0xD8, 0x61, 0x100)
                return;
            }
            break;
        case 2:
            value = pinstEQ_Main_guess->combatSlots_guess[2];
            if (value == -1)
            {
                DI_PLACE_DRAW(DI_RES_4E40, 8, 0x109, 0x30, 0x131)
                return;
            }
            break;
        case 3:
            value = pinstEQ_Main_guess->combatSlots_guess[3];
            if (value == -1)
            {
                DI_PLACE_DRAW(DI_RES_4E40, 0x39, 0x109, 0x61, 0x131)
                return;
            }
            break;
        default:
            return;
        }
        if (value <= 100)
        {
            DI_DRAW(DI_ICON(*(short *)(EQ_ARRAY(char *, 0x006E33A4)[value] + 0x8A)), DI_SELF_RECT);
            return;
        }
        switch (value)
        {
        case 0x69:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B34EC), DI_SELF_RECT);
            return;
        case 0x6B:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B34E4), DI_SELF_RECT);
            return;
        case 0x6F:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B347C), DI_SELF_RECT);
            return;
        default:
            return;
        }
    }
    else if (pinstEQ_Main_guess->dragType_guess == 3)
    {
        switch (DI_SLOT)
        {
        case 0:
            value = pinstEQ_Main_guess->abilitySlots_guess[0];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37C8), DI_SELF_RECT);
                return;
            }
            break;
        case 1:
            value = pinstEQ_Main_guess->abilitySlots_guess[1];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37CC), DI_SELF_RECT);
                return;
            }
            break;
        case 2:
            value = pinstEQ_Main_guess->abilitySlots_guess[2];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37D0), DI_SELF_RECT);
                return;
            }
            break;
        case 3:
            value = pinstEQ_Main_guess->abilitySlots_guess[3];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37D4), DI_SELF_RECT);
                return;
            }
            break;
        case 4:
            value = pinstEQ_Main_guess->abilitySlots_guess[4];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37D8), DI_SELF_RECT);
                return;
            }
            break;
        case 5:
            value = pinstEQ_Main_guess->abilitySlots_guess[5];
            if (value == -1)
            {
                DI_DRAW(EQ_GLOBAL(void *, 0x006B37DC), DI_SELF_RECT);
                return;
            }
            break;
        default:
            return;
        }
        if (value <= 100)
        {
            DI_DRAW(DI_ICON(*(short *)(EQ_ARRAY(char *, 0x006E33A4)[value] + 0x8A)), DI_SELF_RECT);
            return;
        }
        switch (value)
        {
        case 0x69:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B34EC), DI_SELF_RECT);
            return;
        case 0x6B:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B34E4), DI_SELF_RECT);
            return;
        case 0x6F:
            DI_DRAW(EQ_GLOBAL(void *, 0x006B347C), DI_SELF_RECT);
            return;
        case 0x72:
            DI_PLACE(DI_RES_4E40, 0x223, 0x30, 0x24B, 0x58)
            dragRect = (int *)((char *)self + 0x24);
            DI_DRAW(DI_SPRITE, dragRect);
            DI_TEXT((const char *)0x005589E0, *dragRect + 2, DI_SELF_Y + 8, 0, EQ_GLOBAL(int, 0x006EFDF8));
            DI_TEXT((const char *)0x00540678, *dragRect + 6, DI_SELF_Y + 0x12, 0, EQ_GLOBAL(int, 0x006EFDF8));
            return;
        default:
            return;
        }
    }
    else if (pinstEQ_Main_guess->dragType_guess == 4)
    {
        DI_PLACE(DI_RES_4E40, 0x223, 0x30, 0x24B, 0x58)
        destRect = (int *)((char *)self + 0x24);
        DI_DRAW(DI_SPRITE, destRect);
        len = EQ_CDECL(unsigned int, 0x005223D0, (const char *))
            (DI_P4E44 + (DI_SLOT % 12 + DI_SLOT / 12 * 12) * 0x136 + 0x5F196);
        width = 0;
        if ((short)len > 0)
        {
            strPtr = DI_P4E44 + (DI_SLOT % 12 + DI_SLOT / 12 * 12) * 0x136 + 0x5F196;
            len = (short)len;
            do
            {
                ch = *strPtr;
                if (ch > 0)
                    width += *(short *)(EQ_GLOBAL(char *, 0x006EFDDC) + ch * 4);
                strPtr++;
                len = len - 1;
            } while (len != 0);
        }
        width = width >> 1;
        if (width > 0x11)
            width = 0x11;
        DI_TEXT(DI_P4E44 + (DI_SLOT % 12 + DI_SLOT / 12 * 12) * 0x136 + 0x5F196,
                *destRect - width + 0x14, DI_SELF_Y + 0xF,
                (short)*(char *)(DI_P4E44 + (DI_SLOT % 12 + DI_SLOT / 12 * 12) * 0x136 + 0x5F2CB),
                EQ_GLOBAL(int, 0x006EFDF8));
        return;
    }
    else if (pinstEQ_Main_guess->dragType_guess == 5)
    {
        switch (DI_SLOT)
        {
        case 1:  DI_PLACE_DRAW(DI_RES_4DE8, 0x117, 9, 0x146, 0x38) break;
        case 2:  DI_PLACE_DRAW(DI_RES_4DE8, 0x1AA, 9, 0x1D9, 0x38) break;
        case 3:  DI_PLACE_DRAW(DI_RES_4DE8, 0x179, 9, 0x1A8, 0x38) break;
        case 4:  DI_PLACE_DRAW(DI_RES_4DE8, 0x1DB, 9, 0x20A, 0x38) break;
        case 5:  DI_PLACE_DRAW(DI_RES_4DE8, 0x148, 9, 0x177, 0x38) break;
        case 6:  DI_PLACE_DRAW(DI_RES_4DE8, 0x117, 0x8D, 0x146, 0xBC) break;
        case 7:  DI_PLACE_DRAW(DI_RES_4DE8, 0x179, 0x4B, 0x1A8, 0x7A) break;
        case 8:  DI_PLACE_DRAW(DI_RES_4DE8, 0x179, 0x8D, 0x1A8, 0xBC) break;
        case 9:  DI_PLACE_DRAW(DI_RES_4DE8, 0x148, 0x4B, 0x177, 0x7A) break;
        case 10: DI_PLACE_DRAW(DI_RES_4DE8, 0x1DB, 0x4B, 0x209, 0x7A) break;
        case 11: DI_PLACE_DRAW(DI_RES_4DE8, 0x179, 0xCF, 0x1A8, 0xFE) break;
        case 12: DI_PLACE_DRAW(DI_RES_4DE8, 0x1AA, 0x4B, 0x1D9, 0x7A) break;
        case 13: DI_PLACE_DRAW(DI_RES_4DE8, 0x117, 0xCF, 0x146, 0xFE) break;
        case 14: DI_PLACE_DRAW(DI_RES_4DE8, 0x148, 0xCF, 0x177, 0xFE) break;
        case 15: DI_PLACE_DRAW(DI_RES_4DE8, 0x117, 0x4B, 0x146, 0x7A) break;
        case 16: DI_PLACE_DRAW(DI_RES_4DE8, 0x20C, 0x4B, 0x23B, 0x7A) break;
        case 17: DI_PLACE_DRAW(DI_RES_4DE8, 0x148, 0x8D, 0x177, 0xBC) break;
        case 18: DI_PLACE_DRAW(DI_RES_4DE8, 0x1DB, 0x8D, 0x20A, 0xBC) break;
        case 19: DI_PLACE_DRAW(DI_RES_4DE8, 0x20C, 0x8D, 0x23B, 0xBC) break;
        case 20: DI_PLACE_DRAW(DI_RES_4DE8, 0x1AA, 0x8D, 0x1D9, 0xBC) break;
        case 21: DI_PLACE_DRAW(DI_RES_4DE8, 0x1AA, 0xCF, 0x1D9, 0xFE) break;
        case 22: DI_PLACE_DRAW(DI_RES_4E3C, 0x85, 0xFE, 0xB4, 0x12D) break;
        case 23: DI_PLACE_DRAW(DI_RES_4E3C, 0x85, 0x12F, 0xB4, 0x15E) break;
        case 24: DI_PLACE_DRAW(DI_RES_4E3C, 0x85, 0x160, 0xB4, 0x18F) break;
        case 25: DI_PLACE_DRAW(DI_RES_4E3C, 0x85, 0x191, 0xB4, 0x1C0) break;
        case 26: DI_PLACE_DRAW(DI_RES_4E3C, 0xB6, 0xFE, 0xE4, 0x12D) break;
        case 27: DI_PLACE_DRAW(DI_RES_4E3C, 0xB6, 0x12F, 0xE5, 0x15E) break;
        case 28: DI_PLACE_DRAW(DI_RES_4E3C, 0xB6, 0x160, 0xE5, 0x18F) break;
        case 29: DI_PLACE_DRAW(DI_RES_4E3C, 0xB6, 0x191, 0xE5, 0x1C0) break;
        default:
            break;
        }
        if (DI_PC->inventory_items[DI_SLOT] == 0)
            return;
        SetRect(&rect, *(int *)((char *)self + 0x24) + 3, DI_SELF_Y + 3, *(int *)((char *)self + 0x2C),
                *(int *)((char *)self + 0x30));
        DI_DRAW(DI_ICON(DI_PC->inventory_items[DI_SLOT]->icon_nr), &rect);
        if (EQ_CDECL(int, 0x004A1D83, (void *))(DI_PC->inventory_items[DI_SLOT]) == 0)
            return;
        SetRect(&rect, *(int *)((char *)self + 0x24) + 0x1C, DI_SELF_Y + 0x20, *(int *)((char *)self + 0x2C),
                *(int *)((char *)self + 0x30));
        DI_DRAW(EQ_GLOBAL(void *, 0x006B37FC), &rect);
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
            (text, (const char *)0x0053E16C, DI_STACK(DI_PC->inventory_items[DI_SLOT]));
        DI_TEXT(text, *(int *)((char *)self + 0x24) + 0x21, DI_SELF_Y + 0x25, 10, EQ_GLOBAL(int, 0x006EFDF4));
    }
    else if (pinstEQ_Main_guess->dragType_guess == 6)
    {
        switch (DI_SLOT)
        {
        case 0:
            DI_PLACE(DI_RES_4E40, 0x79, 0x78, 0xA1, 0xA0)
            break;
        case 1:
            if (pinstEQ_Main_guess->groupInvitePending_guess != 0)
                DI_PLACE(DI_RES_4E40, 0x93, 0x190, 0xBB, 0x1B8)
            else
                DI_PLACE(DI_RES_4DE8, 0x1D, 0x50, 0x45, 0x78)
            break;
        case 2:
            DI_PLACE(DI_RES_4E40, 0x93, 0xC8, 0xBB, 0xF0)
            break;
        case 3:
            DI_PLACE(DI_RES_4E40, 0x93, 0xF0, 0xBB, 0x118)
            break;
        case 4:
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 0x6E)
                DI_PLACE(DI_RES_4E40, 0x93, 0xA0, 0xBB, 0xC8)
            else
                DI_PLACE(DI_RES_4E40, 0x93, 0x140, 0xBB, 0x168)
            break;
        case 5:
            if (EQ_GLOBAL(char, 0x006B6405) != 0)
                DI_PLACE(DI_RES_4E40, 0x79, 0x50, 0xA1, 0x78)
            else
                DI_PLACE(DI_RES_4E40, 0x93, 0x1B8, 0xBB, 0x1E0)
            break;
        case 6:
            DI_PLACE(DI_RES_4DE8, 0x6D, 0x28, 0x95, 0x50)
            break;
        case 7:
            DI_PLACE(DI_RES_4DE8, 0x6D, 0x50, 0x95, 0x78)
            break;
        case 8:
            DI_PLACE(DI_RES_4DE8, 0xBD, 0, 0xE5, 0x28)
            break;
        case 9:
            DI_PLACE(DI_RES_4DE8, 0xBD, 0x28, 0xE5, 0x50)
            break;
        case 10:
            DI_PLACE(DI_RES_4DE8, 0xBD, 0x50, 0xE5, 0x78)
            break;
        case 11:
            DI_PLACE(DI_RES_4DE8, 0xBD, 0x78, 0xE5, 0xA0)
            break;
        default:
            return;
        }
        DI_DRAW(DI_SPRITE, DI_SELF_RECT);
        return;
    }
    else if (pinstEQ_Main_guess->dragType_guess == 7)
    {
        *DI_SPRITE = pinstEQ_Main_guess->overlaySprite_int_guess;
        SetRect((RECT *)(DI_SPRITE + 1), 0x51, 0x2D, 0x78, 0x54);
        destRect = (int *)((char *)self + 0x24);
        DI_DRAW(DI_SPRITE, destRect);
        if (DI_PC->spell_memorized[DI_SLOT] == -1)
            return;
        SetRect(&rect, *destRect + 7, DI_SELF_Y + 9, 0x280, 0x1E0);
        EQ_CDECL(void, 0x00438423, (RECT *, int))
            (&rect, (int)*(short *)(EQ_ARRAY(char *, 0x005FE9A0)[DI_PC->spell_memorized[DI_SLOT]] + 0x1FC));
    }
}

#undef DI_PLACE_DRAW
#undef DI_PLACE
#undef DI_TEXT
#undef DI_DRAW
#undef DI_STACK
#undef DI_ICON
#undef DI_SELF_Y
#undef DI_SELF_RECT
#undef DI_RES_4E3C
#undef DI_RES_4DE8
#undef DI_RES_4E40
#undef DI_SPRITE
#undef DI_SLOT
#undef DI_PC
#undef DI_P4DE0
#undef DI_P4E44

class EQ_DragItem2 : public EQ_Overlay
{
public:
    EQ_DragItem2();
};

// FUNCTION: TRILOGY 0x00435ece
// Configures a drag-item display with the DragItem02.bmp artwork.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
// @early-stop: mov edi,[SetRect] is emitted before mov ebp,0x1e0 (original: after); dummy-EDX xors stay; normalised 1.0
EQ_DragItem2::EQ_DragItem2()
{
    typedef int (__stdcall *SetRectProc_guess)(RECT*, int, int, int, int);
    SetRectProc_guess set_rect;

    set_rect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)this = 0x00535440;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, (const char*)0x0053E950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, (const char*)0x005589E8);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
}

// FUNCTION: TRILOGY 0x00435f5c
// Releases the drag-item display overlay's resources.
void __fastcall EQ_DragItem_derivedDestructor_00435F5C_guess(void* self)
{
    *(u32*)self = 0x00535440;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_DragItem3 : public EQ_Overlay
{
public:
    EQ_DragItem3();
};

typedef int (__stdcall *EQ_DragItem3_SetRectFn_guess)(RECT*, int, int, int, int);

// FUNCTION: TRILOGY 0x00435f67
// Configures a drag-item display with the DragItem03.bmp artwork.
// ORIGINAL FILE: EverQuest/EQ_DragItem.cpp (from Mac debug map)
EQ_DragItem3::EQ_DragItem3()
{
    EQ_DragItem3_SetRectFn_guess setRect;

    setRect = EQ_GLOBAL(EQ_DragItem3_SetRectFn_guess, 0x00534248);
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)this = 0x0053544C;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1E0);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1E0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, (const char*)0x0053E950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, (const char*)0x005589F8);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
}
