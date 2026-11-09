// EQ_CreateBS.cpp - character creation overlay destructor.

#include "trilogy/common.h"
#include "trilogy/eq_overlay.h"
#include "trilogy/eqaddr.h"
#include "trilogy/globals.h"
#include "trilogy/display.h"

class EQ_CreateBS : public EQ_Overlay
{
public:
    EQ_CreateBS();
};

class EQ_CreateBS3 : public EQ_Overlay
{
public:
    EQ_CreateBS3();
};

// FUNCTION: TRILOGY 0x004340a1
// Initializes the character creation dialog with the appropriate background for the display mode.
// ORIGINAL FILE: EverQuest/EQ_CreateBS.cpp (from Mac debug map)
EQ_CreateBS::EQ_CreateBS()
    : EQ_Overlay()
{
    typedef BOOL (__stdcall *SetRect_guess_t)(LPRECT, int, int, int, int);
    SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
    char *self_bytes = (char *)this;

    *(u32 *)self_bytes = 0x005353E4;
    *(u32 *)(self_bytes + 0x10) = 0;
    *(u32 *)(self_bytes + 0x70) = 0;

    setRect((LPRECT)(self_bytes + 0x24), 0, 0, 0x180, 0x1e0);
    setRect((LPRECT)(self_bytes + 0x14), 0, 0, 0x180, 0x1e0);

    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
        self_bytes + 0x34, EQ_ARRAY(char, 0x0053E950));

    if (g_cdisplay_guess->fastFileHandle6_guess != 0)
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            self_bytes + 0x52, EQ_ARRAY(char, 0x0053E074));
    else if (g_cdisplay_guess->fastFileHandle3_guess != 0)
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            self_bytes + 0x52, EQ_ARRAY(char, 0x0053E098));
    else
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            self_bytes + 0x52, EQ_ARRAY(char, 0x005588E0));

    EQ_METHOD(2, void, 0x00450168, (EQ_Overlay *, int, int), (EQ_Overlay *)this)(1, 0);
    EQ_METHOD(0, void, 0x00434166, (void *), this)();
}

// FUNCTION: TRILOGY 0x0043415b
// Delegates cleanup of the character creation dialog to the base overlay destructor.
void __fastcall EQ_CreateBS_derivedDestructor_0043415B_guess(void* self)
{
    *(u32*)self = 0x005353E4;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x00434166
// Registers row rects 92x27, 29px apart, as overlay elements 0x46..0x53; a global flag adds 0x54 as row 2.
// @early-stop: the nonzero path caches SetRect after the first row in EBX; the original caches it in EDI and keeps the loop counter in EBX.
void EQ_Overlay::RegisterOverlayElementsForCharacterCreation_guess(void)
{
    typedef BOOL (__stdcall *SetRect_guess_t)(LPRECT, int, int, int, int);
    typedef void (EQ_Overlay::*RegisterOverlayElement_guess_t)(short, void*, RECT*);
    union RegisterOverlayElementAddress_guess_t
    {
        void (__cdecl *function)(void);
        RegisterOverlayElement_guess_t method;
    };
    int rowYOrIndex;
    RegisterOverlayElementAddress_guess_t registerOverlayElement;
    struct { RECT rect; int y; } rows;

    registerOverlayElement.function = EQ_FUNC(void, __cdecl, 0x004502A1, (void));
    rowYOrIndex = 0;
    if (g_cdisplay_guess->fastFileHandle6_guess != 0)
    {
        EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&rows.rect, 1, rowYOrIndex, 0x5c, 0x1b);
        (this->*registerOverlayElement.method)(rowYOrIndex + 0x46, *(void**)((char*)this + 4), &rows.rect);
        SetRect_guess_t const setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
        rowYOrIndex = rowYOrIndex + 1;
        setRect(&rows.rect, rowYOrIndex, 0x1d, 0x5c, 0x38);
        (this->*registerOverlayElement.method)(0x54, *(void**)((char*)this + 4), &rows.rect);
        rows.y = 0x3a;
        do
        {
            setRect(&rows.rect, 1, rows.y, 0x5c, rows.y + 0x1b);
            (this->*registerOverlayElement.method)(rowYOrIndex + 0x46, *(void**)((char*)this + 4), &rows.rect);
            rows.y = rows.y + 0x1d;
            rowYOrIndex = rowYOrIndex + 1;
        } while ((short)rowYOrIndex < 0xe);
    }
    else
    {
        int index = 0;
        do
        {
            EQ_GLOBAL(SetRect_guess_t, 0x00534248)(&rows.rect, 1, rowYOrIndex, 0x5c, rowYOrIndex + 0x1b);
            (this->*registerOverlayElement.method)(index + 0x46, *(void**)((char*)this + 4), &rows.rect);
            index = index + 1;
            rowYOrIndex = rowYOrIndex + 0x1d;
        } while ((short)index < 0xe);
    }
    return;
}

class EQ_CreateBS2 : public EQ_Overlay
{
public:
    EQ_CreateBS2();
};

// FUNCTION: TRILOGY 0x00434230
// Initializes the godscreen.bmp overlay used during character creation.
// ORIGINAL FILE: EverQuest/EQ_CreateBS.cpp (from Mac debug map)
EQ_CreateBS2::EQ_CreateBS2()
{
    typedef BOOL (__stdcall *SetRect_guess_t)(LPRECT, int, int, int, int);
    SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);

    *(u32*)this = 0x005353F4;
    *(int*)((char*)this + 0x10) = 0;
    *(int*)((char*)this + 0x70) = 0;
    setRect((LPRECT)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((LPRECT)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, EQ_ARRAY(char, 0x005588F0));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(0, 0); // EDX slot is a dummy: load_guess reads only ECX and its stack args
}

// FUNCTION: TRILOGY 0x004342be
// Delegates cleanup of the godscreen.bmp overlay to the base overlay destructor.
void __fastcall DestroyCreateBS2_guess(void* self)
{
    *(u32*)self = 0x005353F4;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x004342c9
// Initializes the blue location background overlay with the appropriate display variant.
// ORIGINAL FILE: EverQuest/EQ_CreateBS.cpp (from Mac debug map)
EQ_CreateBS3::EQ_CreateBS3()
{
    typedef int (__stdcall *SetRect_guess_t)(void *, int, int, int, int);
    SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);

    *(u32 *)this = 0x00535400;
    *(u32 *)((char *)this + 0x10) = 0;
    *(u32 *)((char *)this + 0x70) = 0;
    setRect((char *)this + 0x24, 0, 0, 0x280, 0x1e0);
    setRect((char *)this + 0x14, 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
        (char *)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    if (g_cdisplay_guess->fastFileHandle3_guess != 0)
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            (char *)this + 0x52, EQ_ARRAY(char, 0x0053E068));
    else
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            (char *)this + 0x52, EQ_ARRAY(char, 0x00558900));
    EQ_METHOD(2, void, 0x00450168, (EQ_Overlay *, int, int), (EQ_Overlay *)this)(0, 0);
}

// FUNCTION: TRILOGY 0x0043436c
// Delegates cleanup of the blue location background overlay to the base overlay destructor.
void __fastcall EQ_CreateBS_derivedDestructor_0043436C_guess(void* self)
{
    *(u32*)self = 0x00535400;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_CreateBS4 : public EQ_Overlay
{
public:
    EQ_CreateBS4();
};

typedef BOOL (WINAPI *SetRect_guess_t)(LPRECT, int, int, int, int);

// FUNCTION: TRILOGY 0x00434377
// Initializes the gold location background overlay with the appropriate display variant.
// ORIGINAL FILE: EverQuest/EQ_CreateBS.cpp (from Mac debug map)
EQ_CreateBS4::EQ_CreateBS4()
{
    SetRect_guess_t setRect = (SetRect_guess_t)EQ_GLOBAL(void *, 0x00534248);

    *(u32 *)this = 0x0053540C;
    *((u32 *)this + 4) = 0;
    *((u32 *)this + 0x1C) = 0;
    setRect((LPRECT)((char *)this + 0x24), 0, 0, 0x280, 0x1E0);
    setRect((LPRECT)((char *)this + 0x14), 0, 0, 0x280, 0x1E0);
    EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
        (char *)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    if (g_cdisplay_guess->fastFileHandle3_guess != 0)
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            (char *)this + 0x52, EQ_ARRAY(char, 0x0053E050));
    else
        EQ_CDECL(char *, 0x005219C0, (char *, const char *))(
            (char *)this + 0x52, EQ_ARRAY(char, 0x0055890C));
    EQ_METHOD(2, void, 0x00450168, (void *, int, int), (void *)this)(0, 0);
}

// FUNCTION: TRILOGY 0x0043441a
// Delegates cleanup of the gold location background overlay to the base overlay destructor.
void __fastcall EQ_CreateBS_derivedDestructor_0043441A_guess(void* self)
{
    *(u32*)self = 0x0053540C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_CreateBS5 : public EQ_Overlay
{
public:
    EQ_CreateBS5();
};

// FUNCTION: TRILOGY 0x00434425
// Initializes the red location background overlay with the appropriate display variant.
// ORIGINAL FILE: EverQuest/EQ_CreateBS.cpp (from Mac debug map)
EQ_CreateBS5::EQ_CreateBS5()
{
    typedef int (__stdcall *SetRect_guess_t)(void*, int, int, int, int);

    SetRect_guess_t setRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
    ((u32*)this)[0] = 0x00535418;
    ((u32*)this)[4] = 0;
    ((u32*)this)[0x1c] = 0;
    setRect((char*)this + 0x24, 0, 0, 0x280, 0x1e0);
    setRect((char*)this + 0x14, 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    if (g_cdisplay_guess->fastFileHandle3_guess != 0)
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)this + 0x52, EQ_ARRAY(char, 0x0053E05C));
    else
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)this + 0x52, EQ_ARRAY(char, 0x00558918));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(0, 0);
}
