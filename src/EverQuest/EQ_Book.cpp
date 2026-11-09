// EQ_Book.cpp - EQ_Book (the spell book / book window overlay).
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Book.cpp (from Mac debug map)

#include <string.h>
#include "trilogy/common.h"
#include "trilogy/layout/eq_book.h"
#include "trilogy/eqaddr.h"
#include "trilogy/globals.h"
#include "trilogy/layout/eq_main.h"

// FUNCTION: TRILOGY 0x00418a30
// Opens a book or note at the beginning and prepares its navigation and close controls.
// ORIGINAL FILE: EverQuest/EQ_Book.cpp (from Mac debug map)
// @early-stop: single-button branch takes EBP/-1 register (shared with the 3-button branch) where the original uses EDI and push -1; vtable call has an extra xor edx; lea placement differs
void EQ_Book_guess::Activate_guess(void)
{
    char* book = (char*)this;
    typedef void (__fastcall *EQ_BookMainCallback_guess)(char*, int);
    typedef int (__stdcall *SetRect_guess_t)(void*, int, int, int, int);
    EQ_BookMainCallback_guess mainCallback;
    SetRect_guess_t setRect;

    EQ_METHOD(0, void, 0x00450463, (char*), book)();
    *(unsigned short*)(book + 0x84) = 0xffff;
    *(int*)(book + 0x70) = 0;
    if (*(int*)(book + 0x37ac) != 0)
    {
        if (g_eqmain_guess != 0)
        {
            pinstEQ_Main_guess->showEquipment_guess = 0;
            mainCallback = (EQ_BookMainCallback_guess)(*(int*)(*(int*)g_eqmain_guess + 4));
            mainCallback(g_eqmain_guess, 0);
            pinstEQ_Main_guess->uiDirty_guess = 1;
        }
        *(unsigned short*)(book + 0x80) = 0;
        *(unsigned short*)(book + 0x82) = 0;
        EQ_METHOD(0, void, 0x00419010, (char*), book)();
        if (*(char*)(*(int*)(book + 0x37ac) + 0xac) != 0)
        {
            setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
            setRect(book + 0x377c, 0x80, 0x115, 0x9f, 0x133);
            setRect(book + 0x378c, 0x1de, 0x115, 0x1fd, 0x133);
            setRect(book + 0x379c, 0x128, 0x117, 0x156, 0x128);
            EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), book)(0, book + 0x377c, -1, -1, 0, 0);
            EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), book)(1, book + 0x378c, -1, -1, 0, 0);
            EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), book)(2, book + 0x379c, -1, -1, 0, 0);
        }
        else
        {
            setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
            setRect(book + 0x377c, 0x128, 0x117, 0x156, 0x128);
            EQ_METHOD(6, void, 0x004503BE, (char*, int, void*, int, int, int, int), book)(0, book + 0x377c, -1, -1, 0, 0);
        }
        *(int*)(book + 0x10) = 1;
    }
    return;
}

// FUNCTION: TRILOGY 0x00418b71
// Clears the book window's displayed text and active state.
// ORIGINAL FILE: EverQuest/EQ_Book.cpp (from Mac debug map)
// __thiscall (no EQ_Book class in the headers, so a __fastcall free function on raw bytes). Clears the
// dwords at +0x88/+0x8C, the byte +0xAD of the object at +0x37AC when there is one, empties the text at
// +0x9C (0x0058FBA4 is an empty string), clears +0x98, sets +0xEB8 of the main UI object and finishes
// with EQ_Overlay::Deactivate (0x004504BA) on this.
void __fastcall EQ_Book_Deactivate_guess(char* book)
{
    *(int*)(book + 0x88) = 0;
    *(int*)(book + 0x8C) = 0;
    if (*(char**)(book + 0x37AC) != 0)
        *(*(char**)(book + 0x37AC) + 0xAD) = 0;
    strcpy(book + 0x9C, EQ_ARRAY(char, 0x0058FBA4));
    *(int*)(book + 0x98) = 0;
    pinstEQ_Main_guess->uiDirty_guess = 1;
    EQ_METHOD(0, void, 0x004504BA, (char*), book)();
}

struct EQ_Book_RECT_guess
{
    long left;
    long top;
    long right;
    long bottom;
};

typedef int (__stdcall *EQ_Book_SetRect_guess_t)(EQ_Book_RECT_guess*, int, int, int, int);

struct EQ_Book_POINT_guess
{
    long x;
    long y;
};

typedef int (__stdcall *EQ_Book_PtInRect_guess_t)(EQ_Book_RECT_guess*, EQ_Book_POINT_guess);

// FUNCTION: TRILOGY 0x00418bc5
// Shows hover help for turning book pages and closing a book or note.
// ORIGINAL FILE: EverQuest/EQ_Book.cpp (from Mac debug map)
void __fastcall EQ_Book_AutoHelp_guess(char* book)
{
    EQ_Book_POINT_guess pt;
    int hit;
    short button;
    int item;
    char help[128];

    button = -1;
    if (EQ_GLOBAL(int, 0x006B6864) == 0)
        return;
    if (pinstEQ_Main_guess->hoverTicks_guess < 3)
        return;
    pt.x = EQ_GLOBAL(short, 0x006B6414);
    help[0] = 0;
    pt.y = EQ_GLOBAL(short, 0x006B6416);
    if (*(int*)(book + 0x10) == 0)
        return;
    for (item = *(int*)(book + 0x70); item != 0; item = *(int*)(item + 0x20))
    {
        hit = EQ_GLOBAL(EQ_Book_PtInRect_guess_t, 0x0053423C)
            ((EQ_Book_RECT_guess*)(item + 4), pt);
        if (hit != 0)
        {
            button = *(short*)(item + 0x14);
            break;
        }
    }
    pinstEQ_Main_guess->hoverId_guess = button;
    if (button == -1)
        return;
    if (*(char*)(*(int*)(book + 0x37AC) + 0xAC) != '\0')
    {
        switch (button)
        {
        case 0:
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(help, EQ_ARRAY(char, 0x0053F168));
            break;
        case 1:
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(help, EQ_ARRAY(char, 0x0053F178));
            break;
        case 2:
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(help, EQ_ARRAY(char, 0x0053F184));
            break;
        default:
            pinstEQ_Main_guess->hoverId_guess = -1;
            return;
        }
    }
    else if (button == 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(help, EQ_ARRAY(char, 0x0053F15C));
    }
    EQ_METHOD(4, void, 0x0046D4DA, (void*, long, long, const char*, int), EQ_GLOBAL(void*, 0x006E4E44))(pt.x, pt.y + 0x14, help, 10);
    return;
}

class EQ_Book_DrawVtable_guess
{
public:
    virtual void unknown0();
    virtual void unknown1();
    virtual void draw();
};

// FUNCTION: TRILOGY 0x00418ccd
// Draws the book or note background, pressed controls and reading content.
// ORIGINAL FILE: EverQuest/EQ_Book.cpp (from Mac debug map)
// @early-stop: SetRect stays memory-indirect (a live setRect local matches 0.939 but shifts the fixture-pinned frame), '1' sits in EDI, some leas/mov ecx hoisted 1-3 slots.
void EQ_Book_guess::draw_guess(void)
{
    char* book = (char*)this;
    short page;
    int active;
    EQ_Book_RECT_guess destinationRect;
    EQ_Book_RECT_guess sourceRect;

    active = EQ_CDECL(int, 0x004504AC, (void))();
    if (active == 0)
        return;
    if (EQ_GLOBAL(int, 0x0058FC6C) != 1)
    {
        ((EQ_Book_DrawVtable_guess*)book)->draw();
        return;
    }
    if (*(*(char**)(book + 0x37AC) + 0xAC) != 0)
    {
        EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 0, 0, 400, 0x140);
        EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&destinationRect, 0x77, 0, 0x280, 0x1E0);
        EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, EQ_Book_RECT_guess*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E08) + 4)), &sourceRect, &destinationRect, 1);
        page = ((EQ_Book_guess*)book)->pressedControlId_guess;
        switch (page)
        {
        case 0:
            EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 0, 0x154, 0x20, 0x173);
            EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, char*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E08) + 4)), &sourceRect, book + 0x377C, 1);
            break;
        case 1:
            EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 0x20, 0x154, 0x41, 0x173);
            EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, char*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E08) + 4)), &sourceRect, book + 0x378C, 1);
            break;
        case 2:
            EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 1, 0x141, 0x30, 0x153);
            EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, char*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E08) + 4)), &sourceRect, book + 0x379C, 1);
            break;
        }
        EQ_METHOD(0, void, 0x00418E97, (char*), book)();
    }
    else
    {
        EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 0, 0, 400, 0x140);
        EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&destinationRect, 0x77, 0, 0x280, 0x1E0);
        EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, EQ_Book_RECT_guess*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E0C) + 4)), &sourceRect, &destinationRect, 1);
        page = ((EQ_Book_guess*)book)->pressedControlId_guess;
        if (page == 0)
        {
            EQ_GLOBAL(EQ_Book_SetRect_guess_t, 0x00534248)(&sourceRect, 1, 0x141, 0x30, 0x153);
            EQ_METHOD(4, void, 0x00450125, (char*, void*, EQ_Book_RECT_guess*, char*, int), book)((void*)(*(u32*)(EQ_GLOBAL(u32, 0x006E4E08) + 4)), &sourceRect, book + 0x377C, 1);
        }
        EQ_METHOD(0, void, 0x00418E4A, (char*), book)();
    }
}

// FUNCTION: TRILOGY 0x00418e4a
// Draws up to 14 word-wrapped text lines (0x3C-byte records at +0x89C) in one column for the single-page layout.
// __thiscall (no EQ_Book class in the headers, so a __fastcall free function on raw bytes). Clamps the short at +0x82
// to 14 and calls EQ_TextO::WriteTextHD_guess once per page on the object that the pointer at 0x006E4E68 names: the
// page record at +0x89C with a stride of 0x3C, y from 0x46 in steps of 0x0F.
void __fastcall EQ_Book_DrawBookPages_guess(char* book)
{
    short pageCount;
    int y;
    char* page;
    int i;

    pageCount = ((EQ_Book_guess*)book)->textLineCount;
    if (pageCount > 0x0E)
        pageCount = 0x0E;
    if (0 < pageCount)
    {
        y = 0x46;
        page = book + 0x89C;
        i = pageCount;
        do
        {
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(page, 200, y, 0,
                EQ_GLOBAL(int, 0x006EFDF8), 1);
            y = y + 0x0F;
            page = page + 0x3C;
            i = i - 1;
        } while (i != 0);
    }
    return;
}
