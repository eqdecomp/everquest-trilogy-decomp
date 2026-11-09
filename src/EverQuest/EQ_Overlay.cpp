// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EQ_Overlay.cpp - EQ_Overlay, the old UI's overlay list.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Overlay.cpp (from Mac debug map)
// Field layout: unrecovered (displacements are what the code reads); all accesses stay raw offsets.

#include "trilogy/common.h"
#include "trilogy/eq_columns.h"
#include "trilogy/display.h"
#include "trilogy/eqgfx.h"
#define InitializeOverlayElement_guess InitializeOverlayElement_guess_header_decl
#include "trilogy/eq_overlay.h"
#undef InitializeOverlayElement_guess
#include "trilogy/eq_image.h"
#include "trilogy/eqaddr.h"
#include "trilogy/globals.h"
#include "trilogy/character.h"
#include "trilogy/item.h"
#include "trilogy/player.h"
#include "trilogy/everquest.h"
#include "trilogy/layout/eq_requestor.h"
#include "trilogy/layout/eq_charcreate.h"
#include "trilogy/eq_textio.h"
#include "trilogy/layout/eq_bag.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef EQTRACE_guess
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
void __cdecl EQTRACE_guess(const char* format, ...);
#undef send_message_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);

#pragma comment(lib, "user32.lib")     // SetRect

/* USER32 SetRect without <windows.h>; the import resolves to user32.dll!SetRect on both sides. */
extern "C" __declspec(dllimport) int __stdcall SetRect(RECT* rect, int left, int top, int right, int bottom);

typedef void (__cdecl *t3dDeferLine_guess_t)(float* vertices, u32 color);
typedef int (__stdcall *tSetRect_guess_t)(RECT* rect, int left, int top, int right, int bottom);
// 0x00534248: USER32 SetRect import address (IAT slot)
#define g_setrect_guess EQ_GLOBAL(tSetRect_guess_t, 0x00534248)
// 0x006EFE2C: CEverQuest as void* (pinstCEverQuest_guess is typed)
#define g_everquest_guess EQ_GLOBAL(void *, 0x006EFE2C)
// 0x006E4E68: the EQ_TextO instance that WriteTextHD_guess is called on
#define g_eqtexto_guess EQ_GLOBAL(void *, 0x006E4E68)
// 0x006EFE00: font handle passed to WriteTextHD_guess
#define g_font_guess EQ_GLOBAL(int, 0x006EFE00)
// 0x006E11E4: key bindings 1, u32 per entry: scan code + modifier bits
#define g_keyBindings1_guess EQ_ARRAY(unsigned int, 0x006E11E4)
// 0x006E15E4: key bindings 2, same layout
#define g_keyBindings2_guess EQ_ARRAY(unsigned int, 0x006E15E4)

/* EH marker for EQ_Overlay_derivedConstructor_004344D3_guess: an empty destructible local gives the function the
 * original's EH frame. File scope on purpose: a local class's inline dtor is called out of line, which the original never does. */
class EQOverlayCtorEHMarker_004344D3_guess
{
public:
    ~EQOverlayCtorEHMarker_004344D3_guess() {}
};

#ifdef EQD_MATCH
extern char eqd_va_0x006E4E40;
#endif

// FUNCTION: TRILOGY 0x00402E9C
// Destroys a 3D overlay instance used by the interface.
void __stdcall DestroyT3DOverlay_guess(u32* overlayInstance)
{
    if (overlayInstance != 0)
    {
        t3dOverlayDestroy((void*)*overlayInstance);
        EQ_METHOD(0, void, 0x0040101D, (void*), overlayInstance)();
        operator delete(overlayInstance);
    }
}

// file-local stand-in for the derived overlay that EQ_Overlay_derivedDestructor_004189F8_guess destroys (vtable 0x535278, set by hand)
class EQ_Overlay_derivedConstructor_0041893B_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_0041893B_guess(int unused);
};

// FUNCTION: TRILOGY 0x0041893B
// Loads the bag artwork and prepares its overlay for display.
EQ_Overlay_derivedConstructor_0041893B_guess::EQ_Overlay_derivedConstructor_0041893B_guess(int unused)
{
    typedef void (EQ_Overlay::*tOverlayLoad_guess_t)(int, int);
    union tOverlayLoad_guess_address_t
    {
        void (__cdecl *function)(int, int);
        tOverlayLoad_guess_t method;
    } load;
    tSetRect_guess_t setRect = g_setrect_guess;

    *(short*)((char*)this + 0x84) = -1;
    *(u32*)this = 0x00535278;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x90) = 0;
    *(u32*)((char*)this + 0x88) = 0;
    *(u32*)((char*)this + 0x8c) = 0;
    *(u32*)((char*)this + 0x94) = 0;
    *(u32*)((char*)this + 0x98) = 0;
    *(u32*)((char*)this + 0x37ac) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)EQ_ARRAY(char, 0x0053EAF8));
    load.function = EQ_FUNC(void, __cdecl, 0x00450168, (int, int));
    (this->*load.method)(1, 0);
}

// FUNCTION: TRILOGY 0x004189f8
// Derived overlay destructor (vtable 0x00535278): deletes its buttons, then runs the base cleanup.
/* fake match: an EH marker and volatile saved-this slot reproduce VC6's unwind frame. */
void __fastcall EQ_Overlay_derivedDestructor_004189F8_guess(u32* overlayInstance)
{
    union OverlayPointer_guess
    {
        u32* raw;
        EQ_Overlay_derivedConstructor_0041893B_guess* object;
    } overlay = { overlayInstance };
    EQ_Overlay_derivedConstructor_0041893B_guess* volatile unwind_this = overlay.object;

    *overlay.raw = 0x00535278;
    {
        EQOverlayCtorEHMarker_004344D3_guess eh_marker;
        void (__fastcall *delButtons)(void*) = EQ_FUNC(void, __fastcall, 0x00450463, (void*));
        delButtons(overlay.object);
    }
    overlay.object->~EQ_Overlay_derivedConstructor_0041893B_guess();
}

// FUNCTION: TRILOGY 0x00419572
// Clears overlay element state and sets two initialization fields to -1.
OverlayElement_guess* __fastcall InitializeOverlayElement_guess(OverlayElement_guess* element)
{
    *(u32*)((char*)element + 0x20) &= 0;
    *(short*)((char*)element + 0x18) = -1;
    *(short*)((char*)element + 0x16) = -1;
    return element;
}

// FUNCTION: TRILOGY 0x004195a9
// Displays the button artwork for its current state.
void __fastcall drawButtonImage_guess(OverlayElement_guess* element)
{
    short imageId;
    EQ_Image_guess* image;

    if (*(short*)element != 0)
        imageId = ((short*)element)[0xc];
    else
        imageId = ((short*)element)[0xb];

    if (imageId >= 0)
    {
        image = EQ_ARRAY(EQ_Image_guess*, 0x006B32B4)[imageId];
        if (image != 0)
            EQ_METHOD(2, void, 0x004384B2, (EQ_Image_guess*, u32, int), image)((u32)((char*)element + 4), 1);
    }
}

// FUNCTION: TRILOGY 0x004195DA
// A click in a button's hit area toggles it or switches it on.
// __thiscall. When `point` is inside the element's rect (+0x4), toggles the state word (+0x0), or sets it
// to 1 when the element is not a toggle (+0x1A != 0), redraws it and returns 1; else 0.
int __fastcall handleMouseClickInButtonRect_guess(OverlayElement_guess* element, int unusedEdx, POINT* point)
{
    if (PtInRect((RECT*)((char*)element + 4), *point))
    {
        if (*(short*)((char*)element + 0x1a) == 0)
            *(u16*)element ^= 1;
        else if (*(short*)element == 0)
            *(u16*)element = 1;
        EQ_METHOD(0, void, 0x004195A9, (OverlayElement_guess*), element)();
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0041961E
// Sets an overlay button's state flag (picks image +0x18 or +0x16) and optionally redraws it.
void __fastcall OverlayElement_SetOverlayElementVisibility_guess(OverlayElement_guess* element, int unusedEdx, int visible, int redraw)
{
    *(u16*)element = (u16)(visible != 0);
    if (redraw != 0)
        drawButtonImage_guess(element);
}

// FUNCTION: TRILOGY 0x0041BA72
// Checks whether a character meets an item's race, class, deity, and level restrictions.
// @early-stop: VC6 keeps the early flag check separate from the word counter's zeroing.
// __thiscall on the character (race u16 +0x34, class u8 +0x36, level u8 +0x38, deity s16 +0x1038). Item:
// +0x81 nonzero -> always usable; else class mask +0xD0, deity mask +0x120 (0 = any, bit deity-200),
// deity-only +0xCC (201..216 must equal ours), race mask +0xD4 (iksar 128 -> bit 12, vah shir 130 -> 13),
// required level +0xC4 (51..127 above our level zeroes the count). Usable when all three masks pass.
int __fastcall can_use_item_guess(void* self, int unusedEdx, void* item)
{
    int race;
    unsigned short count;
    short itemDeity;
    u8 reqLevel;

    race = *(u16*)((char*)self + 0x34);
    if (*(char*)((char*)item + 0x81) != 0)
        return 1;
    count = 0;
    if (*(u32*)((char*)item + 0xd0) & (1 << (*(u8*)((char*)self + 0x36) - 1)))
        count = 1;
    if (*(u32*)((char*)item + 0x120) == 0
        || (*(u32*)((char*)item + 0x120) & (1 << (*(short*)((char*)self + 0x1038) - 200))))
        count++;
    itemDeity = *(short*)((char*)item + 0xcc);
    if (itemDeity >= 201 && itemDeity <= 216 && *(short*)((char*)self + 0x1038) != itemDeity)
        return 0;
    if (race == 0x80)
        race = 13;
    else if (race == 0x82)
        race = 14;
    if (*(u32*)((char*)item + 0xd4) & (1 << (race - 1)))
        count++;
    reqLevel = *(u8*)((char*)item + 0xc4);
    if (reqLevel > 50 && reqLevel < 128 && reqLevel > *(u8*)((char*)self + 0x38))
        count = 0;
    return count == 3;
}

// EXTERNAL: TRILOGY 0x005353A8 ??_7EQ_CharacterCreationOverlay_guess@@6B@
class EQ_CharacterCreationOverlay_guess : public EQ_VirtualOverlay_guess
{
public:
    EQ_CharacterCreationOverlay_guess();
    ~EQ_CharacterCreationOverlay_guess();
    void InitializeCharacterCreationButtons_guess(void);    /* 0x004292CE */
    void InitializeCharacterCreationRectangles_guess(void);    /* 0x00429571 */
};

// FUNCTION: TRILOGY 0x00428686
// Sets up the character creation screen: ccreate background bitmap, element rects and deity names.
// @early-stop: remaining call-convention, register-allocation, and stack-slot differences persisted after eight trials.
EQ_CharacterCreationOverlay_guess::EQ_CharacterCreationOverlay_guess()
{
    tSetRect_guess_t setRect;
    char* const self = (char*)this;
    RECT* rect;
    int top;
    int count;

    setRect =g_setrect_guess;
    *(short*)(self + 0x1bae) = -1;
    *(short*)(self + 0x1bb0) = -1;
    *(short*)(self + 0x1bb2) = -1;
    *(u8*)(self + 0x1bc0) = 0;
    *(u8*)(self + 0x1bd4) = 0;
    *(u8*)(self + 0x1bd5) = 0;
    *(u8*)(self + 0x1bd6) = 0;
    *(u8*)(self + 0x1bd7) = 0;
    *(u8*)(self + 0x1bd8) = 0;
    *(u8*)(self + 0x1bd9) = 0;
    *(u8*)(self + 0x1bda) = 0;
    *(u8*)(self + 0x1bdb) = 0;
    *(u8*)(self + 0x1bdc) = 0;
    *(u32*)(self + 0x664) = 0;
    *(u32*)(self + 0x10) = 1;
    *(u32*)(self + 0x0c) = 0;
    *(u32*)(self + 0x1be8) = 1;
    *(u16*)(self + 0x1bd0) = 1;
    *(u16*)(self + 0x1bb8) = 1;
    *(u32*)(self + 0x1bec) = 0;
    *(u8*)(self + 0x19a2) = 0;
    *(u16*)(self + 0x1ba8) = 0;
    *(u16*)(self + 0x1baa) = 0;
    *(u16*)(self + 0x1bac) = 0;
    *(u32*)(self + 0x1bc8) = 0;
    *(u32*)(self + 0x1bbc) = 0;
    *(u16*)(self + 0x1bd2) = 0;
    *(u16*)(self + 0x19a4) = 0x38;
    *(u16*)(self + 0x19a6) = 0x32;
    *(u16*)(self + 0x19a8) = 0x32;
    *(u32*)(self + 0x1be0) = 0;
    *(u32*)(self + 0x1be4) = 0;
    *(u16*)(self + 0x1bf0) = 0;
    setRect((RECT*)(self + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((RECT*)(self + 0x14), 0, 0, 0x280, 0x1e0);
    *(u32*)(self + 0x70) = 0;
    strcpy(self + 0x34, EQ_ARRAY(char, 0x0053E950));
    if (*(int*)(g_displayManager_guess + 0x94) != 0)
        strcpy(self + 0x52, EQ_ARRAY(char, 0x0053E088));
    else if (*(int*)(g_displayManager_guess + 0x88) != 0)
        strcpy(self + 0x52, EQ_ARRAY(char, 0x0053E0AC));
    else
        strcpy(self + 0x52, EQ_ARRAY(char, 0x005426D8));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(0, 0);
    EQ_METHOD(0, void, 0x00429571, (void*), this)();
    setRect((RECT*)(self + 0x80), 0x181, 0x0d, 0x1dc, 0x28);
    setRect((RECT*)(self + 0x90), 0x181, 0x2e, 0x1dc, 0x49);
    setRect((RECT*)(self + 0xa0), 0x181, 0x4f, 0x1dc, 0x6a);
    setRect((RECT*)(self + 0xb0), 0x181, 0x6f, 0x1dc, 0x8a);
    setRect((RECT*)(self + 0xc0), 0x181, 0x8f, 0x1dc, 0xaa);
    setRect((RECT*)(self + 0xd0), 0x181, 0xb0, 0x1dc, 0xcb);
    setRect((RECT*)(self + 0xe0), 0x181, 0xd1, 0x1dc, 0xec);
    setRect((RECT*)(self + 0xf0), 0x181, 0xf2, 0x1dc, 0x10d);
    setRect((RECT*)(self + 0x100), 0x181, 0x113, 0x1dc, 0x12e);
    setRect((RECT*)(self + 0x110), 0x181, 0x133, 0x1dc, 0x14e);
    setRect((RECT*)(self + 0x120), 0x181, 0x153, 0x1dc, 0x16e);
    setRect((RECT*)(self + 0x130), 0x181, 0x174, 0x1dc, 0x18f);
    setRect((RECT*)(self + 0x140), 0x181, 0x195, 0x1dc, 0x1b0);
    setRect((RECT*)(self + 0x150), 0x181, 0x1b6, 0x1dc, 0x1d1);
    setRect((RECT*)(self + 0x160), 0x228, 0x58, 0x234, 0x65);
    setRect((RECT*)(self + 0x170), 0x261, 0x58, 0x26d, 0x65);
    setRect((RECT*)(self + 0x180), 0x228, 0x81, 0x234, 0x8e);
    setRect((RECT*)(self + 0x190), 0x261, 0x81, 0x26d, 0x8e);
    setRect((RECT*)(self + 0x1a0), 0x228, 0xaa, 0x234, 0xb7);
    setRect((RECT*)(self + 0x1b0), 0x261, 0xaa, 0x26d, 0xb7);
    setRect((RECT*)(self + 0x1c0), 0x228, 0xd3, 0x234, 0xe0);
    setRect((RECT*)(self + 0x1d0), 0x261, 0xd3, 0x26d, 0xe0);
    setRect((RECT*)(self + 0x1e0), 0x228, 0xfc, 0x234, 0x109);
    setRect((RECT*)(self + 0x1f0), 0x261, 0xfc, 0x26d, 0x109);
    setRect((RECT*)(self + 0x200), 0x228, 0x125, 0x234, 0x132);
    setRect((RECT*)(self + 0x210), 0x261, 0x125, 0x26d, 0x132);
    setRect((RECT*)(self + 0x220), 0x228, 0x14e, 0x234, 0x15b);
    setRect((RECT*)(self + 0x230), 0x261, 0x14e, 0x26d, 0x15b);
    setRect((RECT*)(self + 0x240), 0x231, 0x1b6, 0x26c, 0x1d1);
    setRect((RECT*)(self + 0x250), 0x1e8, 0x1b6, 0x223, 0x1d1);
    setRect((RECT*)(self + 0x260), 0x1f7, 0x196, 0x25b, 0x1a7);
    setRect((RECT*)(self + 0x270), 0x0e, 0x0e, 0x69, 0x29);
    setRect((RECT*)(self + 0x280), 0x0e, 0x2f, 0x69, 0x4a);
    setRect((RECT*)(self + 0x290), 0x0e, 0x50, 0x69, 0x6b);
    setRect((RECT*)(self + 0x2a0), 0x0e, 0x70, 0x69, 0x8b);
    setRect((RECT*)(self + 0x2b0), 0x0e, 0x90, 0x69, 0xab);
    setRect((RECT*)(self + 0x2c0), 0x0e, 0xb1, 0x69, 0xcc);
    setRect((RECT*)(self + 0x2d0), 0x0e, 0xd2, 0x69, 0xed);
    setRect((RECT*)(self + 0x2e0), 0x0e, 0xf3, 0x69, 0x10e);
    setRect((RECT*)(self + 0x2f0), 0x0e, 0x114, 0x69, 0x12f);
    if (*(int*)(g_displayManager_guess + 0x94) != 0)
    {
        setRect((RECT*)(self + 0x80), 0x181, 0x0d, 0x1dc, 0x28);
        setRect((RECT*)(self + 0x390), 0x181, 0x2a, 0x1dc, 0x45);
        rect = (RECT*)(self + 0x90);
        top = 0x47;
        count = 0x0d;
        do
        {
            setRect(rect, 0x181, top, 0x1dc, top + 0x1b);
            rect = rect + 1;
            top = top + 0x1d;
            count = count - 1;
        } while (count != 0);
        setRect((RECT*)(self + 0x260), 0x1f7, 0x19e, 0x25b, 0x1af);
        rect = (RECT*)(self + 0x270);
        top = 0x0d;
        count = 9;
        do
        {
            setRect(rect, 0x0e, top, 0x69, top + 0x1b);
            rect = rect + 1;
            top = top + 0x1d;
            count = count - 1;
        } while (count != 0);
        setRect((RECT*)(self + 0x360), 0x0e, 0x112, 0x69, 0x12d);
        setRect((RECT*)(self + 0x300), 0x0e, 0x12f, 0x69, 0x14a);
        setRect((RECT*)(self + 0x310), 0x0e, 0x14c, 0x69, 0x167);
        setRect((RECT*)(self + 0x380), 0x0e, 0x169, 0x69, 0x184);
        setRect((RECT*)(self + 0x320), 0x0e, 0x186, 0x69, 0x1a1);
        setRect((RECT*)(self + 0x330), 0x54, 0x1b5, 0x60, 0x1c2);
        setRect((RECT*)(self + 0x340), 0x54, 0x1c5, 0x60, 0x1d2);
        setRect((RECT*)(self + 0x370), 0x1d6, 0xf6, 0x223, 0x114);
        setRect((RECT*)(self + 0x600), 0x13a, 0x14, 0x176, 0x30);
        setRect((RECT*)(self + 0x610), 0x13a, *(int*)(self + 0x60c) + 1, 0x176, *(int*)(self + 0x60c) + 0x1d);
        setRect((RECT*)(self + 0x620), 0x13a, *(int*)(self + 0x61c) + 1, 0x176, *(int*)(self + 0x61c) + 0x1d);
        setRect((RECT*)(self + 0x630), 0x13a, *(int*)(self + 0x62c) + 1, 0x176, *(int*)(self + 0x62c) + 0x1d);
        setRect((RECT*)(self + 0x640), 0xb4, 0x37, 0xd4, 0x57);
        setRect((RECT*)(self + 0x650), 0x118, 0x37, 0x138, 0x57);
    }
    else if (*(int*)(g_displayManager_guess + 0x88) != 0)
    {
        setRect((RECT*)(self + 0x360), 0x0e, 0x134, 0x69, 0x14f);
        setRect((RECT*)(self + 0x300), 0x0e, 0x154, 0x69, 0x16f);
        setRect((RECT*)(self + 0x310), 0x0e, 0x175, 0x69, 0x190);
        setRect((RECT*)(self + 0x320), 0x0e, 0x196, 0x69, 0x1b1);
        setRect((RECT*)(self + 0x330), 0x53, 0x1b6, 0x5f, 0x1c3);
        setRect((RECT*)(self + 0x340), 0x53, 0x1c6, 0x5f, 0x1d3);
        setRect((RECT*)(self + 0x370), 0x1d6, 0xf6, 0x223, 0x114);
    }
    else
    {
        setRect((RECT*)(self + 0x300), 0x0e, 0x134, 0x69, 0x14f);
        setRect((RECT*)(self + 0x310), 0x0e, 0x154, 0x69, 0x16f);
        setRect((RECT*)(self + 0x320), 0x0e, 0x175, 0x69, 0x190);
        setRect((RECT*)(self + 0x330), 0x59, 0x1a0, 0x65, 0x1ad);
        setRect((RECT*)(self + 0x340), 0x59, 0x1b9, 0x65, 0x1c6);
    }
    setRect((RECT*)(self + 0x350), 0xd8, 0x14, 0x113, 0x2f);
    rect = (RECT*)(self + 0x3a0);
    top = 0x1d;
    count = 0x11;
    do
    {
        setRect(rect, 0x20, top, 0xe6, top + 0x15);
        rect = rect + 1;
        top = top + 0x19;
        count = count - 1;
    } while (count != 0);
    setRect((RECT*)(self + 0x4b0), 0x1b3, 0x185, 0x1bd, 0x18f);
    setRect((RECT*)(self + 0x4c0), 0x162, 0x186, 0x173, 0x190);
    setRect((RECT*)(self + 0x4d0), 0x1f5, 0x186, 0x206, 0x190);
    setRect((RECT*)(self + 0x4e0), 0xb4, 0x14, 0xd4, 0x34);
    setRect((RECT*)(self + 0x4f0), 0x118, 0x14, 0x138, 0x34);
    setRect((RECT*)(self + 0x500), 0x1c8, 0x173, 0x1d2, 0x184);
    setRect((RECT*)(self + 0x510), 0x1c8, 0x1b9, 0x1d2, 0x1ca);
    setRect((RECT*)(self + 0x520), 0x32, 0x9e, 0x76, 0xbc);
    setRect((RECT*)(self + 0x530), 0x94, 0x91, 0xda, 0xb0);
    setRect((RECT*)(self + 0x540), 0xbc, 0x2d, 0xf8, 0x4b);
    setRect((RECT*)(self + 0x550), 0xe6, 0x72, 0x14a, 0x90);
    setRect((RECT*)(self + 0x560), 0x111, 0x91, 0x16a, 0xb0);
    setRect((RECT*)(self + 0x570), 0x128, 0x52, 0x16b, 0x70);
    setRect((RECT*)(self + 0x580), 0x118, 0x11c, 0x154, 0x13b);
    setRect((RECT*)(self + 0x590), 200, 0xe1, 0x109, 0xff);
    setRect((RECT*)(self + 0x5a0), 0x16c, 0x96, 0x1bd, 0xb4);
    setRect((RECT*)(self + 0x5b0), 0x1c2, 0x84, 0x21a, 0xa2);
    setRect((RECT*)(self + 0x5c0), 0x209, 0x74, 0x261, 0x92);
    setRect((RECT*)(self + 0x5d0), 0x203, 0xc0, 0x250, 0xde);
    setRect((RECT*)(self + 0x5e0), 0x1e8, 0x1b6, 0x224, 0x1d2);
    setRect((RECT*)(self + 0x5f0), 0x231, 0x1b6, 0x26d, 0x1d2);
    strcpy(self + 0x19aa, EQ_ARRAY(char, 0x005426CC));
    strcpy(self + 0x19c8, EQ_ARRAY(char, 0x005426B8));
    strcpy(self + 0x19e6, EQ_ARRAY(char, 0x005426A4));
    strcpy(self + 0x1a04, EQ_ARRAY(char, 0x00542694));
    strcpy(self + 0x1a22, EQ_ARRAY(char, 0x00542680));
    strcpy(self + 0x1a40, EQ_ARRAY(char, 0x00542670));
    strcpy(self + 0x1a5e, EQ_ARRAY(char, 0x0054265C));
    strcpy(self + 0x1a7c, EQ_ARRAY(char, 0x0054264C));
    strcpy(self + 0x1a9a, EQ_ARRAY(char, 0x00542638));
    strcpy(self + 0x1ab8, EQ_ARRAY(char, 0x00542628));
    strcpy(self + 0x1ad6, EQ_ARRAY(char, 0x00542618));
    strcpy(self + 0x1af4, EQ_ARRAY(char, 0x00542604));
    strcpy(self + 0x1b12, EQ_ARRAY(char, 0x005425F0));
    strcpy(self + 0x1b30, EQ_ARRAY(char, 0x005425E0));
    strcpy(self + 0x1b4e, EQ_ARRAY(char, 0x005425CC));
    strcpy(self + 0x1b6c, EQ_ARRAY(char, 0x005425BC));
    strcpy(self + 0x1b8a, EQ_ARRAY(char, 0x005425B0));
    EQ_METHOD(0, void, 0x004292CE, (void*), this)();
}

// FUNCTION: TRILOGY 0x00429292
// Closes the character creation overlay after clearing its button list.
EQ_CharacterCreationOverlay_guess::~EQ_CharacterCreationOverlay_guess()
{
    isActive_guess = 0;
    DelButtons();
}

// FUNCTION: TRILOGY 0x004292ce
// Shows the buttons for the current character creation step.
// __fastcall (this in ECX). Calls DelButtons, then adds the buttons for the step in the short at this+0x1BA8
// (cases 0..3); any other value adds none. Cases 0 and 2 read g_displayManager_guess.
void EQ_CharacterCreationOverlay_guess::InitializeCharacterCreationButtons_guess(void)
{
    int self = (int)this;
    int i;
    int slot;
    short state;

    EQ_THISCALL(void, 0x00450463, (int))(self);
    state = *(short*)(self + 0x1ba8);
    *(u32*)(self + 0x10) = 1;
    *(u32*)(self + 0x70) = 0;
    switch (state)
    {
    case 0:
        slot = self + 0x80;
        i = 0;
        do
        {
            EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(i, slot, -1, -1, 0, 0);
            slot = slot + 0x10;
            i = i + 1;
        } while ((short)i < 0x2e);
        if (g_cdisplay_guess->fastFileHandle3_guess != 0 || g_cdisplay_guess->fastFileHandle6_guess != 0)
        {
            EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x2e, self + 0x360, -1, -1, 0, 0);
            if (g_cdisplay_guess->fastFileHandle6_guess != 0)
            {
                EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x30, self + 0x380, -1, -1, 0, 0);
                EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x31, self + 0x390, -1, -1, 0, 0);
            }
        }
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x46, self + 0x4e0, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x47, self + 0x4f0, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x58, self + 0x600, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x59, self + 0x610, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x5a, self + 0x620, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x5b, self + 0x630, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x5c, self + 0x640, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x5d, self + 0x650, -1, -1, 0, 0);
        break;
    case 1:
        i = 0x32;
        slot = self + 0x3a0;
        do
        {
            EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(i, slot, -1, -1, 0, 0);
            slot = slot + 0x10;
            i = i + 1;
        } while ((short)i < 0x43);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x1c, self + 0x240, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x1d, self + 0x250, -1, -1, 0, 0);
        *(u32*)(self + 0x1bcc) = 0;
        return;
    case 2:
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x1c, self + 0x240, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x1d, self + 0x250, -1, -1, 0, 0);
        i = 0x48;
        slot = self + 0x500;
        do
        {
            EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(i, slot, -1, -1, 0, 0);
            slot = slot + 0x10;
            i = i + 1;
        } while ((short)i <= 0x55);
        if (g_cdisplay_guess->fastFileHandle3_guess == 0)
            return;
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x2f, self + 0x370, -1, -1, 0, 0);
        break;
    case 3:
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x56, self + 0x5e0, -1, -1, 0, 0);
        EQ_METHOD(6, void, 0x004503BE, (int, int, int, int, int, int, int), self)(0x57, self + 0x5f0, -1, -1, 0, 0);
        break;
    }
}

/* 0x006E4E74  object whose dword +4 is an element list, used like self+4 (Ghidra: CCH_Ovl74). File-local name:
 * the Ghidra one is not in globals.h. */
#define g_second_overlay_guess EQ_GLOBAL(char *, 0x006E4E74)

/* One element: SetRect into `rect`, then RegisterOverlayElement (0x004502A1) with that rectangle on `list`. */
#define REGISTER_RECT_guess(id, list, left, top, right, bottom) \
    setRect(&rect, left, top, right, bottom); \
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(id, list, &rect)
#define REGISTER_ON_SELF_guess(id, left, top, right, bottom) \
    REGISTER_RECT_guess(id, *(void**)((char*)self + 4), left, top, right, bottom)
#define REGISTER_ON_LIST_guess(id, left, top, right, bottom) \
    REGISTER_RECT_guess(id, *(void**)(g_second_overlay_guess + 4), left, top, right, bottom)

// FUNCTION: TRILOGY 0x00429571
// Registers the character creation images' source rects (SetRect, 0x004502A1) in one of two layouts.
// @early-stop: entry register order and repeated SetRect/register argument scheduling still differ.
// __fastcall (this in ECX). Registers the character creation element rectangles (SetRect, then 0x004502A1) in one of
// two layouts, chosen by the dword at g_displayManager_guess+0x94. Set: a column on the list at CCH_Ovl74+4 (skipped when
// that object or its +4 is null), then the narrower set on self+4. Clear: the wider set on self+4, plus element 0x45
// on the CCH_Ovl74 list when the dword at g_displayManager_guess+0x88 is set and that list exists.
/* fake match: a single-inheritance member-pointer union models ECX plus stack arguments without writing EDX. */
void EQ_CharacterCreationOverlay_guess::InitializeCharacterCreationRectangles_guess(void)
{
    void* self = (void*)this;
    struct OverlayElementRegisterer_guess
    {
        void Register(short imageIndex, void* list, RECT* source) {}
    };
    typedef void (OverlayElementRegisterer_guess::*tRegisterOverlayElement_guess_t)
        (short, void*, RECT*);
    union RegisterOverlayElementCall_guess_t
    {
        void (__cdecl *function)(short, void*, RECT*);
        tRegisterOverlayElement_guess_t method;
    };
    RECT rect;
    int y;
    int i = 0;
    tSetRect_guess_t setRect = g_setrect_guess;

#undef REGISTER_RECT_guess
#define REGISTER_RECT_guess(id, list, left, top, right, bottom) \
    setRect(&rect, left, top, right, bottom); \
    { RegisterOverlayElementCall_guess_t registerOverlayElement; \
      registerOverlayElement.function = EQ_FUNC(void, __cdecl, 0x004502A1, (short, void*, RECT*)); \
      (((OverlayElementRegisterer_guess*)self)->*registerOverlayElement.method)(id, list, &rect); }

    if (g_cdisplay_guess->fastFileHandle6_guess != 0)
    {
        if (g_second_overlay_guess != 0 && *(int *)(g_second_overlay_guess + 4) != 0)
        {
            y = 0;
            do
            {
                REGISTER_ON_LIST_guess(i + 0x1e, 0x12b, y, 0x186, y + 0x1b);
                i = i + 1;
                y = y + 0x1d;
            } while ((short)i < 9);
            REGISTER_ON_LIST_guess(0x45, 0x12b, 0x105, 0x186, 0x120);
            REGISTER_ON_LIST_guess(0x27, 0x12b, 0x122, 0x186, 0x13d);
            REGISTER_ON_LIST_guess(0x28, 0x12b, 0x13f, 0x186, 0x15a);
            REGISTER_ON_LIST_guess(0x3a, 0x12b, 0x15c, 0x186, 0x177);
            REGISTER_ON_LIST_guess(0x29, 0x12b, 0x179, 0x186, 0x194);
            REGISTER_ON_LIST_guess(0x2b, 0x186, 0, 0x1e1, 0x1b);
            REGISTER_ON_LIST_guess(0x3b, 0x186, 0x1d, 0x1e1, 0x38);
            i = 1;
            y = 0x3a;
            do
            {
                REGISTER_ON_LIST_guess(i + 0x2b, 0x186, y, 0x1e1, y + 0x1b);
                i = i + 1;
                y = y + 0x1d;
            } while ((short)i < 0xd);
            REGISTER_ON_LIST_guess(0x2a, 0x186, 0x196, 0x1e1, 0x1b1);
        }
        REGISTER_ON_SELF_guess(0x38, 0x76, 0x15, 0x83, 0x23);
        REGISTER_ON_SELF_guess(0x39, 0x85, 0x14, 0x94, 0x23);
        REGISTER_ON_SELF_guess(0x3c, 0x76, 0x140, 0xb2, 0x15c);
        REGISTER_ON_SELF_guess(0x3d, 0x76, 0x123, 0xb2, 0x13f);
        REGISTER_ON_SELF_guess(0x3e, 0x75, 0x30, 0x94, 0x4f);
        REGISTER_ON_SELF_guess(0x3f, 0x95, 0x30, 0xb4, 0x4f);
        REGISTER_ON_SELF_guess(0x40, 0x75, 0x50, 0x94, 0x6f);
        REGISTER_ON_SELF_guess(0x41, 0x95, 0x50, 0xb4, 0x6f);
        REGISTER_ON_SELF_guess(0x42, 0x76, 0xaa, 0xb2, 0xc6);
        REGISTER_ON_SELF_guess(0x43, 0x76, 199, 0xb2, 0xe3);
        REGISTER_ON_SELF_guess(0x44, 0x76, 0xe4, 0xb2, 0x100);
        REGISTER_ON_SELF_guess(0x55, 0x87, 0x10c, 0x99, 0x117);
        REGISTER_ON_SELF_guess(0x56, 0x75, 0x10c, 0x87, 0x117);
        REGISTER_ON_SELF_guess(0x57, 0x75, 0x117, 0x80, 0x123);
        REGISTER_ON_SELF_guess(0x5a, 0, 0, 0x280, 0x1e0);
        REGISTER_ON_SELF_guess(0x5b, 0x75, 0x50, 0x95, 0x70);
        REGISTER_ON_SELF_guess(0x5c, 0x95, 0x50, 0xb5, 0x70);
        REGISTER_ON_SELF_guess(0x5d, 0x75, 0x30, 0x95, 0x50);
        REGISTER_ON_SELF_guess(0x5e, 0x95, 0x30, 0xb5, 0x50);
        REGISTER_ON_SELF_guess(0x5f, 0x76, 0x70, 0xb2, 0x8c);
        REGISTER_ON_SELF_guess(0x60, 0x8b, 0x15d, 0x95, 0x16f);
        REGISTER_ON_SELF_guess(0x61, 0x95, 0x15d, 0x9f, 0x16f);
        REGISTER_ON_SELF_guess(0x62, 0xa5, 0x14, 0xb5, 0x24);
        REGISTER_ON_SELF_guess(99, 0x95, 0x14, 0xa5, 0x24);
        REGISTER_ON_SELF_guess(10, 0x76, 0xaa, 0xb2, 0xc6);
        REGISTER_ON_SELF_guess(0xb, 0x76, 0x16f, 0xb2, 0x18b);
        REGISTER_ON_SELF_guess(1, 0xb3, 0x70, 0xef, 0x8c);
        REGISTER_ON_SELF_guess(2, 0xb3, 0xaa, 0xef, 0xc6);
        REGISTER_ON_SELF_guess(3, 0xb3, 0x8d, 0xef, 0xa9);
        REGISTER_ON_SELF_guess(4, 0xb3, 199, 0xef, 0xe3);
        REGISTER_ON_SELF_guess(5, 0xb3, 0x11e, 0xef, 0x13a);
        REGISTER_ON_SELF_guess(6, 0xb3, 0x13b, 0xef, 0x157);
        REGISTER_ON_SELF_guess(7, 0xb3, 0x101, 0xef, 0x11d);
    }
    else
    {
        y = 0x14;
        do
        {
            REGISTER_ON_SELF_guess(i + 0x1e, 0x76, y, 0xd1, y + 0x1b);
            y = y + 0x1d;
            i = i + 1;
        } while ((short)i < 0xd);
        i = 0;
        if (g_cdisplay_guess->fastFileHandle3_guess != 0 && g_second_overlay_guess != 0 && *(int *)(g_second_overlay_guess + 4) != 0)
        {
            REGISTER_ON_LIST_guess(0x45, 0x5e, 0xa5, 0xb9, 0xc0);
        }
        y = 0x14;
        do
        {
            REGISTER_ON_SELF_guess(i + 0x2b, 0x119, y, 0x174, y + 0x1b);
            y = y + 0x1d;
            i = i + 1;
        } while ((short)i < 0xd);
        REGISTER_ON_SELF_guess(0x38, 0xd3, 0x15, 0xe0, 0x23);
        REGISTER_ON_SELF_guess(0x39, 0xe2, 0x14, 0xf1, 0x23);
        REGISTER_ON_SELF_guess(0x3a, 0xf2, 0x14, 0x101, 0x23);
        REGISTER_ON_SELF_guess(0x3b, 0x102, 0x14, 0x111, 0x23);
        REGISTER_ON_SELF_guess(0x3c, 0xd3, 0x140, 0x10f, 0x15c);
        REGISTER_ON_SELF_guess(0x3d, 0xd3, 0x123, 0x10f, 0x13f);
        REGISTER_ON_SELF_guess(0x3e, 0xd2, 0x30, 0xf1, 0x4f);
        REGISTER_ON_SELF_guess(0x3f, 0xf2, 0x30, 0x111, 0x4f);
        REGISTER_ON_SELF_guess(0x40, 0xd2, 0x50, 0xf1, 0x6f);
        REGISTER_ON_SELF_guess(0x41, 0xf2, 0x50, 0x111, 0x6f);
        REGISTER_ON_SELF_guess(0x42, 0xd3, 0xaa, 0x10f, 0xc6);
        REGISTER_ON_SELF_guess(0x43, 0xd3, 199, 0x10f, 0xe3);
        REGISTER_ON_SELF_guess(0x44, 0xd3, 0xe4, 0x10f, 0x100);
        REGISTER_ON_SELF_guess(0x55, 0xe4, 0x10c, 0xf6, 0x117);
        REGISTER_ON_SELF_guess(0x56, 0xd2, 0x10c, 0xe4, 0x117);
        REGISTER_ON_SELF_guess(0x57, 0xd2, 0x117, 0xdd, 0x123);
        REGISTER_ON_SELF_guess(0x5a, 0, 0, 0x280, 0x1e0);
        REGISTER_ON_SELF_guess(0x5b, 0xd2, 0x50, 0xf2, 0x70);
        REGISTER_ON_SELF_guess(0x5c, 0xf2, 0x50, 0x112, 0x70);
        REGISTER_ON_SELF_guess(0x5d, 0xd2, 0x30, 0xf2, 0x50);
        REGISTER_ON_SELF_guess(0x5e, 0xf2, 0x30, 0x112, 0x50);
        REGISTER_ON_SELF_guess(0x5f, 0xd3, 0x70, 0x10f, 0x8c);
        REGISTER_ON_SELF_guess(0x60, 0xe8, 0x15d, 0xf2, 0x16f);
        REGISTER_ON_SELF_guess(0x61, 0xf2, 0x15d, 0xfe, 0x16f);
        REGISTER_ON_SELF_guess(0x62, 0x102, 0x14, 0x112, 0x24);
        REGISTER_ON_SELF_guess(99, 0xf2, 0x14, 0x102, 0x24);
        REGISTER_ON_SELF_guess(10, 0xd3, 0xaa, 0x10f, 0xc6);
        REGISTER_ON_SELF_guess(0xb, 0xd3, 0x16f, 0x10f, 0x18b);
    }

#undef REGISTER_RECT_guess
}

#undef REGISTER_ON_LIST_guess
#undef REGISTER_ON_SELF_guess
#undef REGISTER_RECT_guess
#undef g_second_overlay_guess

// FUNCTION: TRILOGY 0x004301e9
// Registers image source rects 0x13..0x1B on the character select overlay's bitmap (+4).
void __fastcall RegisterCharacterSelectionOverlayElements_guess(void* self)
{
    RECT rect;

    g_setrect_guess(&rect, 0x30, 0x30, 0x133, 0x53);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x14, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x30, 0x5f, 0x133, 0x82);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x15, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x30, 0x8e, 0x133, 0xb1);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x16, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x30, 0xbd, 0x133, 0xe0);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x17, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x30, 0xec, 0x133, 0x10f);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x18, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x165, 0x1b, 0x1c0, 0x36);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x19, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x165, 0x38, 0x1a0, 0x53);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x1a, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x165, 0x55, 0x1a0, 0x70);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x1b, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x164, 0x72, 0x16c, 0x82);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x1c, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x164, 0x82, 0x16c, 0x92);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x1d, *(void**)((char*)self + 4), &rect);
    g_setrect_guess(&rect, 0x164, 0x92, 0x16c, 0xa2);
    EQ_METHOD(3, void, 0x004502A1, (void*, short, void*, RECT*), self)(0x13, *(void**)((char*)self + 4), &rect);
}

// Stand-in for the header's EQ_Columns_guess, which has no base class and no constructor. Deriving from
// EQ_Overlay gives the constructor the base-object EH frame the original has (base ctor, state 0, vtable store).
class EQ_Columns : public EQ_Overlay
{
public:
    EQ_Columns();
};

// FUNCTION: TRILOGY 0x0043208F
// Initializes an overlay's play area, controls, and score display.
EQ_Columns::EQ_Columns()
{
    tSetRect_guess_t set_rect;
    RECT* lprc;

    *(u32*)this = 0x005353d8;
    EQ_METHOD(0, void, 0x004334AF, (void*), this)();
    set_rect = g_setrect_guess;
    *(u32*)((char*)this + 0x80) |= 0xffffffff;
    lprc = (RECT*)((char*)this + 0x90);
    *(u32*)((char*)this + 0x9a0) = 0x801;
    *(u32*)((char*)this + 0x9a4) = 0x806;
    *(u32*)((char*)this + 0x9a8) = 0x849;
    *(u32*)((char*)this + 0x9ac) = 0x803;
    *(u32*)((char*)this + 0x9b0) = 0x80b;
    *(u32*)((char*)this + 0x9b4) = 0x843;
    *(u32*)((char*)this + 0x9b8) = 0x7e2;
    *(u32*)((char*)this + 0x9bc) = 0x824;
    *(u32*)((char*)this + 0x9c0) = 0x7e6;
    *(u32*)((char*)this + 0x9c4) = 0x7e3;
    *(u32*)((char*)this + 0x9c8) = 0x7e8;
    *(u32*)((char*)this + 0x9cc) = 0x82a;
    *(u32*)((char*)this + 0x9e8) = 0x28;
    *(u32*)((char*)this + 0x9ec) = 0x28;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x88) = 1;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    *(u8*)((char*)this + 0x34) = 0;
    *(u8*)((char*)this + 0x52) = 0;
    *(u32*)((char*)this + 0x78) = 0;
    *(u32*)((char*)this + 0x8c) = 0;
    *(u32*)((char*)this + 0x7c) = 0;
    *(u32*)((char*)this + 0x04) = 0;
    set_rect(lprc, 0x7d, 0x0a, 0x217, 0x203);
    EQ_METHOD(6, void, 0x004503BE, (void*, short, RECT*, int, int, int, int), this)(0, lprc, -1, -1, 0, 0);
    set_rect((RECT*)((char*)this + 0xb0), *(int*)((char*)this + 0x98), *(int*)((char*)this + 0x94),
        *(int*)((char*)this + 0x98) + 0x38, *(int*)((char*)this + 0x94) + 0x18);
    EQ_METHOD(6, void, 0x004503BE, (void*, short, RECT*, int, int, int, int), this)(2, (RECT*)((char*)this + 0xb0), -1, -1, 0, 0);
    set_rect((RECT*)((char*)this + 0xa0), *(int*)((char*)this + 0x98), *(int*)((char*)this + 0x9c) + -0x18,
        *(int*)((char*)this + 0x98) + 0x38, *(int*)((char*)this + 0x9c));
    EQ_METHOD(6, void, 0x004503BE, (void*, short, RECT*, int, int, int, int), this)(1, (RECT*)((char*)this + 0xa0), -1, -1, 0, 0);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    set_rect((RECT*)((char*)this + 0x24), lprc->left, *(int*)((char*)this + 0x94),
        *(int*)((char*)this + 0xa8), *(int*)((char*)this + 0xac));
    *(u32*)((char*)this + 0x84) = 0;
    EQ_METHOD(0, void, 0x00432264, (void*), this)();
    *(u32*)((char*)this + 0x84) = 0;
}

// FUNCTION: TRILOGY 0x00432259
// Closes the puzzle game overlay after play ends.
void __fastcall EQ_Overlay_derivedDestructor_00432259_guess(void* self)
{
    *(u32*)self = 0x005353D8;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x004344c8
// Closes a character creation overlay; its screen-specific role is unclear.
void __fastcall EQ_Overlay_derivedDestructor_004344C8_guess(void* self)
{
    *(u32*)self = 0x00535418;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Overlay_derivedConstructor_004344D3_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_004344D3_guess();
};

// FUNCTION: TRILOGY 0x004344d3
// Loads the bug report artwork for an overlay in character creation.
EQ_Overlay_derivedConstructor_004344D3_guess::EQ_Overlay_derivedConstructor_004344D3_guess()
{
    tSetRect_guess_t set_rect = g_setrect_guess;
    *(u32*)this = 0x00535424;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)0x0053E950);
    strcpy((char*)this + 0x52, (const char*)0x00558924);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(0, 0);
}

// FUNCTION: TRILOGY 0x00434561
// Closes the character creation bug report overlay.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (inferred from mapped address neighbors)
void __fastcall EQ_Overlay_derivedDestructor_00434561_guess(void* self)
{
    *(u32*)self = 0x00535424;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// file-local stand-in for the derived overlay that EQ_Overlay_derivedConstructor_00434607_guess::EQ_Overlay_derivedConstructor_00434607_guess constructs (vtable 0x535430, set by hand)
class EQ_Overlay_derivedConstructor_00434607_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_00434607_guess();
};

// FUNCTION: TRILOGY 0x00434607
// Loads DragItem01 artwork and defines images for moving items.
EQ_Overlay_derivedConstructor_00434607_guess::EQ_Overlay_derivedConstructor_00434607_guess()
{
    typedef int (__stdcall *CopyRect_guess_t)(RECT* dst, RECT* src);

    tSetRect_guess_t setRect = g_setrect_guess;

    *(u32*)this = 0x00535430;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x005589D0));
    CopyRect_guess_t copyRect = EQ_GLOBAL(CopyRect_guess_t, 0x00534240);
    copyRect((RECT*)((char*)this + 0xa4), (RECT*)((char*)this + 0x24));
    copyRect((RECT*)((char*)this + 0x94), (RECT*)((char*)this + 0x24));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
    EQ_METHOD(0, void, 0x004346B9, (void*), this)();
}

// FUNCTION: TRILOGY 0x00435ff5
// Closes an overlay whose game-specific screen purpose is unclear.
void __fastcall EQ_Overlay_derivedDestructor_00435FF5_guess(void* self)
{
    *(u32*)self = 0x0053544C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// file-local stand-in for the derived overlay that EQ_Overlay_derivedDestructor_0043608E_guess destroys (vtable 0x535458, set by hand)
class EQ_Overlay_derivedConstructor_00436000_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_00436000_guess();
};

// FUNCTION: TRILOGY 0x00436000
// Loads DragItem04 artwork for the item dragging overlay.
// @early-stop: mov edi,[SetRect] is emitted before mov ebp,0x1e0 (original: after); EQ_THISCALL's dummy-EDX xor before load_guess stays (its 0 mutant is equivalent); match normalised 1.0
EQ_Overlay_derivedConstructor_00436000_guess::EQ_Overlay_derivedConstructor_00436000_guess()
{
    tSetRect_guess_t setRect = g_setrect_guess;

    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)this = 0x00535458;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x280, 0x1e0);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)EQ_ARRAY(char, 0x0053E0D4));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
}

// FUNCTION: TRILOGY 0x0043608E
// Derived overlay destructor (vtable 0x00535458): runs the base EQ_Overlay cleanup.
void __fastcall EQ_Overlay_derivedDestructor_0043608E_guess(void* self)
{
    *(u32*)self = 0x00535458;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x0043ABDD
// Displays an active overlay.
void __fastcall DrawOverlayIfActive_guess(void* self)
{
    if (EQ_METHOD(0, int, 0x004504AC, (void*), self)() != 0)
    {
        ((EQ_Overlay*)self)->draw_guess(1);
        *(u32*)((char*)self + 0x80) = 1;
    }
}

// FUNCTION: TRILOGY 0x0045002a
// Clears dwords 0x0, 0x4, 0x14..0x1C and sets 0x8, 0xC, 0x10 to 1.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (inferred from mapped address neighbors)
void EQ_Overlay::InitializeOverlayProperties_guess()
{
    *(u32*)((char*)this + 0x0) = 0;
    *(u32*)((char*)this + 0x4) = 0;
    *(u32*)((char*)this + 0x8) = 1;
    *(u32*)((char*)this + 0xC) = 1;
    *(u32*)((char*)this + 0x14) = 0;
    *(u32*)((char*)this + 0x10) = 1;
    *(u32*)((char*)this + 0x18) = 0;
    *(u32*)((char*)this + 0x1C) = 0;
}

// FUNCTION: TRILOGY 0x00450049
// Initializes state shared by old UI overlays.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Overlay::EQ_Overlay()
{
    *(u32*)this = 0x0053548C;
    *(u32*)((char*)this + 0x74) = 0;
    *(u32*)((char*)this + 0x78) = 0;
    *(u32*)((char*)this + 0x8) = 0;
}

// FUNCTION: TRILOGY 0x0045005d
// Releases an overlay's buttons and closes its display overlay.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Overlay::~EQ_Overlay()
{
    *(u32*)this = 0x0053548C;
    EQ_METHOD(0, void, 0x00450463, (EQ_Overlay*), this)();
    if (*(u32*)((char*)this + 0x7C) != 0)
    {
        CDisplay* display;

        display = g_cdisplay_guess;
        EQ_METHOD(1, void, 0x0040B173, (CDisplay*, void*), display)(*(void**)((char*)this + 0x4));
    }
}

// FUNCTION: TRILOGY 0x00450081
// Draws an active UI overlay and its registered button images.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::draw_guess(int mode)
{
    int active;
    OverlayElement_guess* element;
    CDisplay* display;

    active = EQ_METHOD(0, int, 0x004504AC, (void*), this)();
    if (active != 0)
    {
        display = g_cdisplay_guess;
        EQ_METHOD(4, void, 0x0040B492, (CDisplay*, void*, int, int*, int), display)(*(void**)((char*)this + 0x4),
             (int)((char*)this + 0x14), (int*)((char*)this + 0x24), mode);

        element = *(OverlayElement_guess**)((char*)this + 0x70);
        while (element != 0)
        {
            EQ_METHOD(0, void, 0x004195A9, (void*), element)();
            element = *(OverlayElement_guess**)((char*)element + 0x20);
        }
    }
}

// FUNCTION: TRILOGY 0x004500be
// Applies an active overlay texture when the player actor permits it.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::drawExplicit(void* overlay, int arg, int* pos)
{
    EQPlayer* player;
    EQActorInfo_guess* actor;
    int active;

    active = EQ_METHOD(0, int, 0x004504AC, (void*), this)();
    if (active != 0)
    {
        player = EQ_GLOBAL(EQPlayer*, 0x006E4DA0);
        if (player == 0 || ((actor = player->actor_info) == 0) || actor->unknown0x189 == 0)
            t3dOverlayApply(overlay, pos[0], pos[1], arg);
    }
}

// FUNCTION: TRILOGY 0x004500fd
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (address-inferred from neighboring functions)
// The third stack dword is consumed by RET 0xC but is not read by the body.
void EQ_Overlay::UpdatePartyVitals_guess(int arg, int* pos, int unused)
{
    int active;
    CDisplay* display;

    active = EQ_METHOD(0, int, 0x004504AC, (void*), this)();
    if (active != 0)
    {
        display = g_cdisplay_guess;
        EQ_METHOD(4, void, 0x0040B492, (CDisplay*, void*, int, int*, int), display)(*(void**)((char*)this + 0x4), arg, pos, 1);
    }
}

// FUNCTION: TRILOGY 0x00450125
// Draws an active overlay within a caller-supplied screen region.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::drawPartial_guess(void* ptr, int arg, int* pos, int unused)
{
    int active;
    CDisplay* display;

    active = EQ_METHOD(0, int, 0x004504AC, (void*), this)();
    if (active != 0)
    {
        display = g_cdisplay_guess;
        EQ_METHOD(4, void, 0x0040B492, (CDisplay*, void*, int, int*, int), display)(ptr, arg, pos, unused);
    }
}

// FUNCTION: TRILOGY 0x0045014c
// Finds a registered overlay element by its ID.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (inferred from mapped address neighbors)
OverlayElement_guess* EQ_Overlay::FindOverlayElementById_guess(short id)
{
    OverlayElement_guess* el;

    el = *(OverlayElement_guess**)((char*)this + 0x70);
    while (1)
    {
        if (el == 0)
            return 0;
        if (*(s16*)((char*)el + 0x14) == id)
            return el;
        el = *(OverlayElement_guess**)((char*)el + 0x20);
    }
}

// FUNCTION: TRILOGY 0x00450168
// Loads or adopts the bitmap used by an old UI overlay.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::load_guess(char* overlayName, int overlayHandle)
{
    if (overlayHandle != 0)
    {
        *(u32*)((char*)this + 0x7c) = 0;
        *(u32*)((char*)this + 0x4) = overlayHandle;
    }
    else
    {
        *(u32*)((char*)this + 0x4) = EQ_METHOD(3, u32, 0x0040B188, (CDisplay*, void*, void*, char*), g_cdisplay_guess)
            ((char*)this + 0x34, (char*)this + 0x52, overlayName);
        *(u32*)((char*)this + 0x7c) = 1;
    }
}

// FUNCTION: TRILOGY 0x004501a4
// Lets a button click through only when no higher-priority element covers it.
// __thiscall on EQ_Overlay. Returns 0 when an element in the list (+0x70, linked by +0x20) whose priority
// (+0x1C) is above `target`'s takes the click at `point`; else 1.
int __fastcall EQ_Overlay_CheckButtonPriorityConflict_guess(void* self, int unusedEdx,
                                                            OverlayElement_guess* target, POINT* point)
{
    OverlayElement_guess* el;

    el = *(OverlayElement_guess**)((char*)self + 0x70);
    while (el != 0)
    {
        if (*(short*)((char*)el + 0x1c) > *(short*)((char*)target + 0x1c)
            && EQ_METHOD(1, int, 0x004195DA, (OverlayElement_guess*, POINT*), el)(point) != 0)
            return 0;
        el = *(OverlayElement_guess**)((char*)el + 0x20);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004501d9
// Routes a mouse release to the first eligible overlay button under the pointer.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
int __fastcall EQ_Overlay_MouseUp_guess(EQ_Overlay* self, int unusedEdx, int arg, short x, short y)
{
    typedef int (__stdcall *tPtInRect_MouseUp_guess_t)(const RECT*, POINT);
    struct MouseUpCallbackDispatch_guess
    {
        virtual void call(int callbackArg, short buttonId) = 0;
    };
    typedef void (__cdecl *tWavePlayAddress_MouseUp_guess_t)(int, int);
    typedef void (EQ_Overlay::*tWavePlayMethod_MouseUp_guess_t)(int, int);
    union tWavePlayCall_MouseUp_guess_t
    {
        tWavePlayAddress_MouseUp_guess_t address;
        tWavePlayMethod_MouseUp_guess_t method;
    } wavePlayCall;
    typedef int (__cdecl *tIsActiveAddress_MouseUp_guess_t)(void*);
    typedef int (EQ_Overlay::*tIsActiveMethod_MouseUp_guess_t)(void);
    union tIsActiveCall_MouseUp_guess_t
    {
        tIsActiveAddress_MouseUp_guess_t address;
        tIsActiveMethod_MouseUp_guess_t method;
    } isActiveCall;
    typedef int (__cdecl *tHandleAddress_MouseUp_guess_t)(POINT*);
    typedef int (EQ_Overlay::*tHandleMethod_MouseUp_guess_t)(POINT*);
    union tHandleCall_MouseUp_guess_t
    {
        tHandleAddress_MouseUp_guess_t address;
        tHandleMethod_MouseUp_guess_t method;
    } handleCall;
    POINT point;
    int active;
    int handled;
    OverlayElement_guess* element;

    isActiveCall.address = EQ_FUNC(int, __cdecl, 0x004504AC, (void*));
    active = ((self)->*isActiveCall.method)();
    if (active != 0)
    {
        point.x = x;
        point.y = y;
        if (EQ_GLOBAL(tPtInRect_MouseUp_guess_t, 0x0053423C)
                ((RECT*)&self->clipRect_guess, point) != 0
            || self->unk_0x78 != 0)
        {
            handleCall.address = EQ_FUNC(int, __cdecl, 0x004195DA, (POINT*));
            element = self->elements_guess;
            while (element != 0)
            {
                handled = (((EQ_Overlay*)element)->*handleCall.method)(&point);
                if (handled != 0
                    && (*(short*)((char*)element + 0x1c) == -1
                        || EQ_METHOD(2, int, 0x004501A4, (void*, OverlayElement_guess*, POINT*), self)(element, &point) != 0))
                {
                    ((MouseUpCallbackDispatch_guess*)self)->call(
                        arg, *(short*)((char*)element + 0x14));
                    if (500 < (u32)(*(int*)((char*)g_display_guess + 0xbc)
                                    - EQ_GLOBAL(int, 0x005921F4)))
                    {
                        if (g_sound_manager_ptr_guess != 0)
                        {
                            wavePlayCall.address = EQ_FUNC(void, __cdecl, 0x004E34FA,
                                                           (int, int));
                            (((EQ_Overlay*)g_sound_manager_ptr_guess)
                                ->*wavePlayCall.method)(0x8e, 0);
                        }
                        EQ_GLOBAL(int, 0x005921F4) =
                            *(int*)((char*)g_display_guess + 0xbc);
                    }
                    return 1;
                }
                element = *(OverlayElement_guess**)((char*)element + 0x20);
            }
        }
    }
    return 0;
}

// The 0x14-byte image AddImage allocates: EQ_Image's constructor (0x00438458, EQ_Image.cpp), only declared
// here on a type of the image's size.
struct EQ_ImageStorage_guess
{
    char bytes[0x14];

    static void* operator new(unsigned int size)
    {
        return ::operator new(size);
    }

    // EXTERNAL: TRILOGY 0x00438458 EQ_ImageStorage_guess::EQ_ImageStorage_guess
    EQ_ImageStorage_guess(RECT* rect);
};

// FUNCTION: TRILOGY 0x004502a1
// Registers or updates an old UI image's source rectangle.
void __fastcall AddImage(void* self, int unusedEdx,
                                                             short imageIndex, int bottom, RECT* srcRect)
{
    typedef int (__stdcall *CopyRect_guess_t)(RECT*, const RECT*);
    char rect[0x14];
    EQ_Image_guess* image;
    EQ_Image_guess** images;

    EQ_GLOBAL(CopyRect_guess_t, 0x00534240)((RECT*)rect, srcRect);
    *(int*)(rect + 0x10) = bottom;
    images = EQ_ARRAY(EQ_Image_guess*, 0x006B32B4);
    image = images[imageIndex];
    if (image != 0)
    {
        EQ_METHOD(1, void, 0x004384CE, (EQ_Image_guess*, RECT*), image)((RECT*)rect);
    }
    else
    {
        image = (EQ_Image_guess*)new EQ_ImageStorage_guess((RECT*)rect);
        images[imageIndex] = image;
    }
}

// The button element: new runs its constructor InitializeOverlayElement_guess (0x00419572) under the new-expression's
// cleanup. Declared here, so the frame and EH state are VC6's own.
class OverlayElementNew_guess
{
public:
    // EXTERNAL: TRILOGY 0x00419572 OverlayElementNew_guess::OverlayElementNew_guess
    OverlayElementNew_guess();
    char storage_guess[0x24];
};

// FUNCTION: TRILOGY 0x00450316
// Like AddButton, but the +0x1c word is the last argument instead of -1.
// @early-stop: no zero register (the original keeps 0 in EDI), so the null, visible and zero-store forms differ; self is EDI not EBX; raw is EAX not ECX; no state -1 store.
void EQ_Overlay::add_overlay_element_guess(short id, RECT* rect, short left, short top, short visible, short buttonId, short imageId)
{
    char* element;
    char* cur;
    char* next;

    element = (char*)new OverlayElementNew_guess;

    EQ_METHOD(1, void, 0x00419598, (void*, RECT*), element)(rect);
    *(short*)(element + 0x1c) = imageId;
    *(short*)(element + 0x14) = id;
    *(short*)(element + 0x16) = left;
    *(short*)(element + 0x18) = top;
    EQ_METHOD(2, void, 0x0041961E, (void*, int, int), element)(visible != 0, 0);
    *(short*)(element + 0x1a) = buttonId;

    cur = *(char**)((char*)this + 0x70);
    if (cur == 0)
    {
        *(char**)((char*)this + 0x70) = element;
    }
    else
    {
        do
        {
            next = *(char**)(cur + 0x20);
            if (next != 0)
                cur = next;
        } while (next != 0);
        *(char**)(cur + 0x20) = element;
        *(char**)(element + 0x20) = 0;
    }
}

// FUNCTION: TRILOGY 0x004503be
// Adds a clickable hit region to an overlay, with its ID and visibility.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Overlay_AddButton_guess(EQ_Overlay* self, int unusedEdx,
                                           short id, RECT* rect,
                                           short left, short top,
                                           short visible, short buttonId)
{
    char* element;
    char* cur;
    char* next;

    element = (char*)new OverlayElementNew_guess;

    EQ_METHOD(1, void, 0x00419598, (void*, RECT*), element)(rect);
    *(short*)(element + 0x1c) = -1;
    *(short*)(element + 0x14) = id;
    *(short*)(element + 0x16) = left;
    *(short*)(element + 0x18) = top;
    if (visible != 0)
        EQ_METHOD(2, void, 0x0041961E, (void*, short, short), element)(1, 0);
    else
        EQ_METHOD(2, void, 0x0041961E, (void*, short, short), element)(0, 0);
    *(short*)(element + 0x1a) = buttonId;

    cur = *(char**)((char*)self + 0x70);
    if (cur == 0)
    {
        *(char**)((char*)self + 0x70) = element;
    }
    else
    {
        do
        {
            next = *(char**)(cur + 0x20);
            if (next != 0)
                cur = next;
        } while (next != 0);
        *(char**)(cur + 0x20) = element;
        *(char**)(element + 0x20) = 0;
    }
}

// FUNCTION: TRILOGY 0x00450463
// Removes all registered buttons from an overlay.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::DelButtons()
{
    char* cur;
    char* saved;

    cur = *(char**)((char*)this + 0x70);
    if (cur != 0)
    {
        if (*(char**)(cur + 0x20) != 0)
        {
            do
            {
                saved = *(char**)(cur + 0x20);
                if (cur != 0)
                {
                    EQ_METHOD(0, void, 0x0040101D, (void*), cur)();
                    operator delete(cur);
                }
                if (*(char**)(saved + 0x20) == 0)
                {
                    cur = saved;
                    break;
                }
                cur = saved;
            } while (1);
        }
        if (cur != 0)
        {
            EQ_METHOD(0, void, 0x0040101D, (void*), cur)();
            operator delete(cur);
        }
    }
    *(u32*)((char*)this + 0x70) &= 0;
}

// FUNCTION: TRILOGY 0x004504b0
// Sets two overlay state flags; their specific game meaning is unclear.
void __fastcall OverlayActivate_guess(EQ_Overlay* overlay)
{
    overlay->isActive_guess = 1;
    overlay->redrawAll_guess = 1;
}

// FUNCTION: TRILOGY 0x004504ba
// Clears one overlay state flag; its specific game meaning is unclear.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::Deactivate_guess()
{
    *(u32*)((char*)this + 0x10) &= 0;
}

// FUNCTION: TRILOGY 0x004504bf
// Calculates a 40-by-40 source rectangle for an overlay image index.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (inferred from mapped address neighbors)
void EQ_Overlay::CalculateOverlayElementRect_guess(short id, RECT* rect)
{
    int top;
    short left;

    int temp = ((id / 12) * 40);
    left = (int)(short)temp;
    top = (int)(short)((id % 12) * 40);
    SetRect(rect, left, top, left + 40, 40 + top);
}

// FUNCTION: TRILOGY 0x004504ff
// Reads an integer UI setting from eqclient.ini, using the default for missing or zero values.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (inferred from mapped address neighbors)
int EQ_Overlay::GetConfigIntValueWithDefault_guess(const char* section, const char* key, int def)
{
    char buf[32];
    const char* ini = EQ_ARRAY(char, 0x0053DB50);
    int v = def;

    GetPrivateProfileStringA(section, key, EQ_ARRAY(char, 0x0053DB48), buf, 32, ini);
    if (buf[0] == '*')
    {
        sprintf(buf, EQ_ARRAY(char, 0x0053E16C), v);
        WritePrivateProfileStringA(section, key, buf, ini);
    }
    else
    {
        v = atoi(buf);
        if (v == 0)
            v = def;
    }
    return v;
}

// FUNCTION: TRILOGY 0x0045056d
// Saves an old UI window value to eqclient.ini.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Overlay::WritePrivateProfileWindowValue(const char* section, const char* key, int value)
{
    char buf[32];

    sprintf(buf, EQ_ARRAY(char, 0x0053E16C), value);
    WritePrivateProfileStringA(section, key, buf, EQ_ARRAY(char, 0x0053DB50));
}

// FUNCTION: TRILOGY 0x004505a0
// Draws the key bound to one key-binding entry (first or second table) as text, e.g. 'Shift A' or NOKEY.
// @early-stop: frame/register allocation and NOKEY call setup still differ from the original.
// __cdecl, void. Draws one key-binding label through the text object at 0x006E4E68: NOKEY when the
// binding is 0xFFFFFFFF, else the key's name or character with its Shift/Ctrl/Alt prefix.
// slot is a label slot (the set byte at base+0xFE0 times 400 is added), bindingIndex indexes the binding
// table (0x006E11E4, or 0x006E15E4 when secondTable is not 0). The original reuses the parameter slots:
// byte 3 of slot is the Shift flag, byte 3 of bindingIndex the Alt flag, secondTable the key character.
void renderKeyBindingText_guess(int slot, int bindingIndex, int secondTable)
{
    int keyChar;
    size_t textLen;
    unsigned int binding;
    unsigned char ctrlDown;
    int scanCode;
    int textY;
    char* text;
    char label[128];
    int textX;

    slot = slot + pinstEQ_Main_guess->screenMode_guess * 400;
    // per-slot position: x at 0x6C90C + slot * 16, y at 0x6C910 + slot * 16
    textX = *(int*)(g_eqmain_guess + slot * 0x10 + 0x6C90C) + 8;
    if (secondTable == 0)
    {
        binding = g_keyBindings1_guess[bindingIndex];
        if (binding != 0xFFFFFFFF)
        {
            scanCode = binding & 0xFF;
            textY = *(int*)(g_eqmain_guess + (slot + 0x6C91) * 0x10) - 1;
        }
        else
        {
            textY = *(int*)(g_eqmain_guess + (slot + 0x6C91) * 0x10) - 1;
            text = EQ_ARRAY(char, 0x0055A130);
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(text, textX, textY, 7, g_font_guess, 1);
            return;
        }
    }
    else
    {
        binding = g_keyBindings2_guess[bindingIndex];
        if (binding != 0xFFFFFFFF)
        {
            scanCode = binding & 0xFF;
            textY = *(int*)(g_eqmain_guess + (slot + 0x6C91) * 0x10) + 0xD;
        }
        else
        {
            textY = *(int*)(g_eqmain_guess + (slot + 0x6C91) * 0x10) + 0xD;
            text = EQ_ARRAY(char, 0x0055A130);
            EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(text, textX, textY, 7, g_font_guess, 1);
            return;
        }
    }
    // 0x10000000 Shift, 0x20000000 Ctrl, 0x40000000 Alt
    *((unsigned char*)&slot + 3) = (unsigned char)((binding >> 28) & 1);
    ctrlDown = (unsigned char)((binding >> 29) & 1);
    *((unsigned char*)&bindingIndex + 3) = (unsigned char)((binding >> 30) & 1);
    if (scanCode == 0)
    {
        return;
    }
    keyChar = EQ_CDECL(int, 0x004C1C23, (int))(scanCode);
    label[0] = '\0';
    if (*((unsigned char*)&slot + 3) != 0)
    {
        if (ctrlDown != 0)
        {
            if (*((unsigned char*)&bindingIndex + 3) != 0)
            {
                strcat(label, EQ_ARRAY(char, 0x0055A2E0));
            }
            else
            {
                strcat(label, EQ_ARRAY(char, 0x0055A2D8));
            }
        }
        else if (*((unsigned char*)&bindingIndex + 3) != 0)
        {
            strcat(label, EQ_ARRAY(char, 0x0055A2D0));
        }
        else
        {
            strcat(label, EQ_ARRAY(char, 0x0055A2C0));
        }
    }
    else if (ctrlDown != 0)
    {
        if (*((unsigned char*)&bindingIndex + 3) != 0)
        {
            strcat(label, EQ_ARRAY(char, 0x0055A2C8));
        }
        else
        {
            strcat(label, EQ_ARRAY(char, 0x0055A2B8));
        }
    }
    else if (*((unsigned char*)&bindingIndex + 3) != 0)
    {
        strcat(label, EQ_ARRAY(char, 0x0055A2B0));
    }
    // no printable character, or space (0x39): name the key instead
    if (keyChar == 0 || scanCode == 0x39)
    {
        if (scanCode <= 0x4C)
        {
            if (scanCode != 0x4C)
            {
                if (scanCode <= 0x3E)
                {
                    if (scanCode != 0x3E)
                    {
                        if (scanCode <= 0x37)
                        {
                            if (scanCode != 0x37)
                            {
                                switch (scanCode)
                                {
                                case 0x0E:
                                    strcat(label, EQ_ARRAY(char, 0x0055A280));
                                    break;
                                case 0x0F:
                                    strcat(label, EQ_ARRAY(char, 0x0055A28C));
                                    break;
                                case 0x1D:
                                    strcat(label, EQ_ARRAY(char, 0x0055A290));
                                    break;
                                case 0x29:
                                    strcat(label, EQ_ARRAY(char, 0x0055A298));
                                    break;
                                case 0x2A:
                                    strcat(label, EQ_ARRAY(char, 0x0055A2A0));
                                    break;
                                case 0x36:
                                    strcat(label, EQ_ARRAY(char, 0x0055A2A8));
                                    break;
                                }
                            }
                            else
                            {
                                strcat(label, EQ_ARRAY(char, 0x0055A278));
                            }
                        }
                        else
                        {
                            switch (scanCode)
                            {
                            case 0x38:
                                strcat(label, EQ_ARRAY(char, 0x0055A250));
                                break;
                            case 0x39:
                                strcat(label, EQ_ARRAY(char, 0x0055A258));
                                break;
                            case 0x3A:
                                strcat(label, EQ_ARRAY(char, 0x0055A260));
                                break;
                            case 0x3B:
                                strcat(label, EQ_ARRAY(char, 0x0055A26C));
                                break;
                            case 0x3C:
                                strcat(label, EQ_ARRAY(char, 0x0055A270));
                                break;
                            case 0x3D:
                                strcat(label, EQ_ARRAY(char, 0x0055A274));
                                break;
                            }
                        }
                    }
                    else
                    {
                        strcat(label, EQ_ARRAY(char, 0x0055A24C));
                    }
                }
                else
                {
                    switch (scanCode)
                    {
                    case 0x3F:
                        strcat(label, EQ_ARRAY(char, 0x0055A248));
                        break;
                    case 0x40:
                        strcat(label, EQ_ARRAY(char, 0x0055A244));
                        break;
                    case 0x41:
                        strcat(label, EQ_ARRAY(char, 0x0055A240));
                        break;
                    case 0x42:
                        strcat(label, EQ_ARRAY(char, 0x0055A23C));
                        break;
                    case 0x43:
                        strcat(label, EQ_ARRAY(char, 0x0055A238));
                        break;
                    case 0x44:
                        strcat(label, EQ_ARRAY(char, 0x0055A234));
                        break;
                    case 0x45:
                        strcat(label, EQ_ARRAY(char, 0x0055A22C));
                        break;
                    case 0x4B:
                        strcat(label, EQ_ARRAY(char, 0x0055A224));
                        break;
                    case 0x47:
                        strcat(label, EQ_ARRAY(char, 0x0055A21C));
                        break;
                    case 0x48:
                        strcat(label, EQ_ARRAY(char, 0x0055A214));
                        break;
                    case 0x49:
                        strcat(label, EQ_ARRAY(char, 0x0055A20C));
                        break;
                    case 0x4A:
                        strcat(label, EQ_ARRAY(char, 0x0055A204));
                        break;
                    }
                }
            }
            else
            {
                strcat(label, EQ_ARRAY(char, 0x0055A1FC));
            }
        }
        else if (scanCode <= 0x9C)
        {
            if (scanCode != 0x9C)
            {
                if (scanCode <= 0x51)
                {
                    if (scanCode != 0x51)
                    {
                        switch (scanCode)
                        {
                        case 0x4D:
                            strcat(label, EQ_ARRAY(char, 0x0055A1DC));
                            break;
                        case 0x4E:
                            strcat(label, EQ_ARRAY(char, 0x0055A1E4));
                            break;
                        case 0x4F:
                            strcat(label, EQ_ARRAY(char, 0x0055A1EC));
                            break;
                        case 0x50:
                            strcat(label, EQ_ARRAY(char, 0x0055A1F4));
                            break;
                        }
                    }
                    else
                    {
                        strcat(label, EQ_ARRAY(char, 0x0055A1D4));
                    }
                }
                else
                {
                    switch (scanCode)
                    {
                    case 0x52:
                        strcat(label, EQ_ARRAY(char, 0x0055A1BC));
                        break;
                    case 0x53:
                        strcat(label, EQ_ARRAY(char, 0x0055A1C4));
                        break;
                    case 0x57:
                        strcat(label, EQ_ARRAY(char, 0x0055A1CC));
                        break;
                    case 0x58:
                        strcat(label, EQ_ARRAY(char, 0x0055A1D0));
                        break;
                    }
                }
            }
            else
            {
                strcat(label, EQ_ARRAY(char, 0x0055A1B0));
            }
        }
        else if (scanCode <= 0xCD)
        {
            if (scanCode != 0xCD)
            {
                if (scanCode <= 0xC7)
                {
                    if (scanCode != 0xC7)
                    {
                        switch (scanCode)
                        {
                        case 0x9D:
                            strcat(label, EQ_ARRAY(char, 0x0055A190));
                            break;
                        case 0xB5:
                            strcat(label, EQ_ARRAY(char, 0x0055A198));
                            break;
                        case 0xB8:
                            strcat(label, EQ_ARRAY(char, 0x0055A1A0));
                            break;
                        case 0xC5:
                            strcat(label, EQ_ARRAY(char, 0x0055A1A8));
                            break;
                        }
                    }
                    else
                    {
                        strcat(label, EQ_ARRAY(char, 0x0055A188));
                    }
                }
                else
                {
                    switch (scanCode)
                    {
                    case 0xC8:
                        strcat(label, EQ_ARRAY(char, 0x0055A174));
                        break;
                    case 0xC9:
                        strcat(label, EQ_ARRAY(char, 0x0055A178));
                        break;
                    case 0xCB:
                        strcat(label, EQ_ARRAY(char, 0x0055A180));
                        break;
                    }
                }
            }
            else
            {
                strcat(label, EQ_ARRAY(char, 0x0055A16C));
            }
        }
        else
        {
            switch (scanCode)
            {
            case 0xCF:
                strcat(label, EQ_ARRAY(char, 0x0055A138));
                break;
            case 0xD0:
                strcat(label, EQ_ARRAY(char, 0x0055A13C));
                break;
            case 0xD1:
                strcat(label, EQ_ARRAY(char, 0x0055A144));
                break;
            case 0xD2:
                strcat(label, EQ_ARRAY(char, 0x0055A14C));
                break;
            case 0xD3:
                strcat(label, EQ_ARRAY(char, 0x0055A154));
                break;
            case 0xDB:
                strcat(label, EQ_ARRAY(char, 0x0055A15C));
                break;
            case 0xDC:
                strcat(label, EQ_ARRAY(char, 0x0055A164));
                break;
            }
        }
    }
    else
    {
        // new NUL one past the end, then the key character overwrites the old NUL
        textLen = strlen(label);
        label[textLen + 1] = '\0';
        textLen = strlen(label);
        label[textLen] = (char)keyChar;
    }
drawLabel:
    text = label;
    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(text, textX, textY, 7, g_font_guess, 1);
}

class EQ_Main14_guess : public EQ_Overlay
{
public:
    EQ_Main14_guess();
};

// FUNCTION: TRILOGY 0x00450aa2
// Initializes the old UI note window using note.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main14_guess::EQ_Main14_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x00535498;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A2EC));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450b34
// Runs the base overlay cleanup for this derived old UI window.
void __fastcall EQ_Overlay_derivedDestructor_00450B34_guess(void* self)
{
    *(u32*)self = 0x00535498;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main15_guess : public EQ_Overlay
{
public:
    EQ_Main15_guess();
};

// FUNCTION: TRILOGY 0x00450b3f
// Initializes the old UI book window using book.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main15_guess::EQ_Main15_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354A4;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A2F8));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

class EQ_Main16 : public EQ_Overlay
{
public:
    EQ_Main16();
};

// FUNCTION: TRILOGY 0x00450bdc
// Initializes the overlay panel represented by bbblank1.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main16::EQ_Main16()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354B0;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)0x0053E950);
    strcpy((char*)this + 0x52, (const char*)0x0055A304);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450bd1
// Destructor thunk for overlay vtable 0x005354A4; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450BD1_guess(void* self)
{
    *(u32*)self = 0x005354A4;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x00450c6e
// Destructor thunk for overlay vtable 0x005354B0; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450C6E_guess(void* self)
{
    *(u32*)self = 0x005354B0;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main17_guess : public EQ_Overlay
{
public:
    EQ_Main17_guess();
};

// FUNCTION: TRILOGY 0x00450c79
// Initializes the overlay panel represented by bbcomp1.bmp.
EQ_Main17_guess::EQ_Main17_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354BC;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)0x0055A314);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450d0b
// Destructor thunk for overlay vtable 0x005354BC; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450D0B_guess(void* self)
{
    *(u32*)self = 0x005354BC;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main18_guess : public EQ_Overlay
{
public:
    EQ_Main18_guess();
};

// FUNCTION: TRILOGY 0x00450d16
// Initializes the character select overlay represented by outchsel.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main18_guess::EQ_Main18_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354C8;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x640, 0x4b0);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A320));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450da8
// Destructor thunk for overlay vtable 0x005354C8; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450DA8_guess(void* self)
{
    *(u32*)self = 0x005354C8;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main9 : public EQ_Overlay
{
public:
    EQ_Main9();
};

// FUNCTION: TRILOGY 0x00450db3
// Initializes the overlay panel represented by main9.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main9::EQ_Main9()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354D4;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A330));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450e45
// Destructor thunk for overlay vtable 0x005354D4; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450E45_guess(void* self)
{
    *(u32*)self = 0x005354D4;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main10_guess : public EQ_Overlay
{
public:
    EQ_Main10_guess();
};

// FUNCTION: TRILOGY 0x00450e50
// Initializes the options overlay represented by optionscr1.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main10_guess::EQ_Main10_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354E0;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0053E028));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450ee2
// Destructor thunk for overlay vtable 0x005354E0; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450EE2_guess(void* self)
{
    *(u32*)self = 0x005354E0;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main11_guess : public EQ_Overlay
{
public:
    EQ_Main11_guess();
};

// FUNCTION: TRILOGY 0x00450eed
// Initializes the spell gem overlay represented by spelgems.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main11_guess::EQ_Main11_guess()
{
    tSetRect_guess_t set_rect;

    set_rect = g_setrect_guess;
    *(u32*)this = 0x005354EC;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)0x0053E950);
    strcpy((char*)this + 0x52, (const char*)0x0055A33C);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00450f7f
// Destructor thunk for overlay vtable 0x005354EC; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00450F7F_guess(void* self)
{
    *(u32*)self = 0x005354EC;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main12_guess : public EQ_Overlay
{
public:
    EQ_Main12_guess();
};

// FUNCTION: TRILOGY 0x00450f8a
// Initializes the spell icon overlay represented by spelicon.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main12_guess::EQ_Main12_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x005354F8;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A34C));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x0045101c
// Destructor thunk for overlay vtable 0x005354F8; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_0045101C_guess(void* self)
{
    *(u32*)self = 0x005354F8;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x00451027
// Calls overlay method 0x00450168 with (1, 0); its own two arguments are ignored.
void __fastcall EQ_Overlay_loadOverlay_guess(void* self, int unusedEdx, int unused1, int unused2)
{
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), self)(1, 0);
}

class EQ_Main13_guess : public EQ_Overlay
{
public:
    EQ_Main13_guess();
};

// FUNCTION: TRILOGY 0x00451033
// Initializes the overlay panel represented by bugrpt.bmp.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main13_guess::EQ_Main13_guess()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x00535504;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)0x00558924);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x004510c5
// Destructor thunk for overlay vtable 0x00535504; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_004510C5_guess(void* self)
{
    *(u32*)self = 0x00535504;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main8 : public EQ_Overlay
{
public:
    EQ_Main8();
};

// FUNCTION: TRILOGY 0x004510d0
// Initializes the overlay panel represented by main8.bmp.
EQ_Main8::EQ_Main8()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x00535510;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)0x0055A35C);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00451162
// Destructor thunk for overlay vtable 0x00535510; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00451162_guess(void* self)
{
    *(u32*)self = 0x00535510;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main7 : public EQ_Overlay
{
public:
    EQ_Main7();
};

// FUNCTION: TRILOGY 0x0045116d
// Initializes the overlay panel represented by main7.bmp.
EQ_Main7::EQ_Main7()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x0053551C;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)0x0055A368);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x004511ff
// Destructor thunk for overlay vtable 0x0053551C; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_004511FF_guess(void* self)
{
    *(u32*)self = 0x0053551C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main6 : public EQ_Overlay
{
public:
    EQ_Main6();
};

// FUNCTION: TRILOGY 0x0045120a
// Initializes the overlay panel represented by main6.bmp.
EQ_Main6::EQ_Main6()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x00535528;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, (const char*)0x0055A374);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x0045129c
// Destructor thunk for overlay vtable 0x00535528; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_0045129C_guess(void* self)
{
    *(u32*)self = 0x00535528;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main5 : public EQ_Overlay
{
public:
    EQ_Main5();
};

// FUNCTION: TRILOGY 0x004512a7
// Initializes the overlay panel represented by main5.bmp.
EQ_Main5::EQ_Main5()
{
    tSetRect_guess_t setRect;

    setRect = g_setrect_guess;
    *(u32*)this = 0x00535534;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)0x0053E950);
    strcpy((char*)this + 0x52, (const char*)0x0055A380);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00451339
// Destructor thunk for overlay vtable 0x00535534; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00451339_guess(void* self)
{
    *(u32*)self = 0x00535534;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Main4_guess : public EQ_Overlay
{
public:
    EQ_Main4_guess();
};

// FUNCTION: TRILOGY 0x00451344
// Initializes a character overlay with an icon selected for the player's class.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_Main4_guess::EQ_Main4_guess()
{
    tSetRect_guess_t setRect;
    u8 cls;

    setRect = g_setrect_guess;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    *(u32*)this = 0x00535540;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x1e0, 0xfa);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x1e0, 0xfa);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    // The class tests are nested `!=` conjunctions with one charicon per else: VC6 then lays out the original's
    // single compare chain with the arms after it, last first (a switch would become a jump table).
    if (EQ_GLOBAL(void*, 0x006E4DA8) != 0 &&
        (cls = *((u8*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36)) != 1 && cls != 8)
    {
        if (cls != 0xe && cls != 0xb && cls != 5 && cls != 3)
        {
            if (cls != 6 && cls != 2)
            {
                if (cls != 0xd && cls != 4 && cls != 0xc)
                {
                    if (cls == 7 || cls == 9 || cls == 0xa)
                    {
                        strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A3CC));
                    }
                    else
                    {
                        strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A38C));
                    }
                }
                else
                {
                    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A3BC));
                }
            }
            else
            {
                strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A3AC));
            }
        }
        else
        {
            strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A39C));
        }
    }
    else
    {
        strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0055A38C));
    }
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
    EQ_METHOD(0, void, 0x0045144A, (void*), this)();
}

// FUNCTION: TRILOGY 0x0045143f
// Destructor thunk for overlay vtable 0x00535540; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_0045143F_guess(void* self)
{
    *(u32*)self = 0x00535540;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x0045144a
// Registers image frames 400+ cut from a sheet in a grid layout that depends on the player's class (1..14).
// @early-stop: loop register allocation, index updates, and EBX offset hoisting still differ.
// __fastcall (`self` in ECX, EDX unused), no stack arguments. Re-reads the profile pointer at 0x006E4DA8 for each
// class test (byte +0x36). For the matching class it lays out grids of RECTs with SetRect (pointer at 0x00534248)
// and registers each one through 0x004502A1 (index, self+4, &rect).
void __fastcall EQ_Overlay_registerOverlayElementsByCharacterClass_0045144A_guess(EQ_Overlay* self, int edx_unused)
{
    volatile int idx; /* fake match: keep the zero-based index as a stack local, as in the original */
    int x;
    RECT rect;

    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 1)
    {
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0, x + 0x46, 0xa2);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x46;
            idx++;
        } while (x < 0x276);
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0xa2, x + 0x46, 0x144);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x199, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x46;
            idx++;
        } while (x < 0x276);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 0xe)
    {
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0, x + 0x47, 0x5d);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x47;
        } while (x < 0x238);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 0xb)
    {
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0x5d, x + 0x47, 0xcb);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x47;
            idx++;
        } while (x < 0x27f);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 5)
    {
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0xcb, x + 0x47, 0x143);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x47;
            idx++;
        } while (x < 0x27f);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 3)
    {
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0x143, x + 0x47, 0x1bb);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x47;
            idx++;
        } while (x < 0x27f);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 8)
    {
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0x143, x + 0x46, 0x1c1);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x46;
        } while (x < 0x1ea);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 6)
    {
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0, x + 0x46, 0x72);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x46;
        } while (x < 0x276);
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0x72, x + 0x46, 0xe4);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x199, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x46;
        } while (x < 0x118);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 2)
    {
        idx = 0;
        x = 0x120;
        do
        {
            SetRect(&rect, x, 0x72, x + 0x38, 0xef);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x46;
            idx++;
        } while (x < 0x27e);
        idx = 0;
        x = 8;
        do
        {
            SetRect(&rect, x, 0xef, x + 0x38, 0x16c);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x195, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x46;
            idx++;
        } while (x < 0x238);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 0xd)
    {
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0, x + 0x39, 0x70);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x39;
        } while (x < 0x273);
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0x70, x + 0x39, 0xe0);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x19b, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x39;
        } while (x < 0x11d);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 4)
    {
        idx = 0;
        x = 0x12c;
        do
        {
            SetRect(&rect, x, 0x70, x + 0x3c, 0xf4);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x3c;
            idx++;
        } while (x < 0x258);
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0xe0, x + 0x3c, 0x164);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x195, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x3c;
            idx++;
        } while (x < 0x12c);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 0xc)
    {
        idx = 0;
        x = 0x12c;
        do
        {
            SetRect(&rect, x, 0xf4, x + 0x34, 0x164);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x34;
            idx++;
        } while (x < 0x264);
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0x164, x + 0x34, 0x1d4);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x196, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x34;
            idx++;
        } while (x < 0x9c);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 7)
    {
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0, x + 0x3b, 0x96);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x3b;
            idx++;
        } while (x < 0x24e);
        idx = 0;
        x = 0;
        do
        {
            SetRect(&rect, x, 0x96, x + 0x3b, 0x12c);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x19a, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x3b;
            idx++;
        } while (x < 0xec);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 9)
    {
        idx = 0;
        x = 0xec;
        do
        {
            SetRect(&rect, x, 0x96, x + 0x3c, 0x139);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            x += 0x3c;
            idx++;
        } while (x < 0x254);
        x = 0;
        idx = 0;
        do
        {
            SetRect(&rect, x, 0x12c, x + 0x3c, 0x1cf);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x196, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x3c;
        } while (x < 0x78);
    }
    if (*((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36) == 0xa)
    {
        idx = 0;
        x = 0xe8;
        do
        {
            SetRect(&rect, x, 0x139, x + 0x44, 0x1b8);
            EQ_THISCALL(void, 0x004502A1, (void*, int, short, void*, RECT*))
                (self, 0, idx + 0x190, (void*)self->displayOverlay_int_guess, &rect);
            idx++;
            x += 0x44;
        } while (x < 0x280);
    }
}

class EQ_Overlay_derivedConstructor_00451ABA_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_00451ABA_guess();
};

// FUNCTION: TRILOGY 0x00451aba
// Cloned from EQ_Main9::EQ_Main9 0x00450DB3 (same original code).
EQ_Overlay_derivedConstructor_00451ABA_guess::EQ_Overlay_derivedConstructor_00451ABA_guess()
{
    tSetRect_guess_t set_rect = g_setrect_guess;
    *(u32*)this = 0x0053554C;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, (const char*)0x0053E950);
    strcpy((char*)this + 0x52, (const char*)0x0053E038);
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00451b4c
// Destructor thunk for overlay vtable 0x0053554C; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00451B4C_guess(void* self)
{
    *(u32*)self = 0x0053554C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

class EQ_Overlay_derivedConstructor_00451B57_guess : public EQ_Overlay
{
public:
    EQ_Overlay_derivedConstructor_00451B57_guess();
};

// FUNCTION: TRILOGY 0x00451b57
// Initializes the overlay panel represented by main2.bmp.
EQ_Overlay_derivedConstructor_00451B57_guess::EQ_Overlay_derivedConstructor_00451B57_guess()
{
    tSetRect_guess_t set_rect;

    set_rect = g_setrect_guess;
    *(u32*)this = 0x00535558;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    set_rect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    set_rect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e0);
    strcpy((char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy((char*)this + 0x52, EQ_ARRAY(char, 0x0053E044));
    EQ_METHOD(2, void, 0x00451027, (void*, int, int), this)(0, 0);
    *(u32*)((char*)this + 0x78) = 0;
}

// FUNCTION: TRILOGY 0x00451be9
// Destructor thunk for overlay vtable 0x00535558; runs shared overlay teardown.
void __fastcall EQ_Overlay_derivedDestructor_00451BE9_guess(void* self)
{
    *(u32*)self = 0x00535558;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// 0x20-byte overlay slot; its destructor is the empty function at 0x0040101D
class EQ_OverlaySlot_guess
{
public:
    EQ_OverlaySlot_guess()
    {
        EQ_METHOD(0, void, 0x0045002A, (void*), this)();
    }

    // EXTERNAL: TRILOGY 0x0040101D EQ_OverlaySlot_guess::~EQ_OverlaySlot_guess
    ~EQ_OverlaySlot_guess() throw();

    char pad[0x20];
};

// file-local stand-in for the overlay derived class (vtable 0x535564) that owns 18 slots at +0x90044 and
// 18 six-slot arrays at +0x90284 (stride 0xC0)
// EXTERNAL: TRILOGY 0x00535564 ??_7EQ_OverlaySlots_guess@@6B@
class EQ_OverlaySlots_guess : public EQ_VirtualOverlay_guess
{
public:
    EQ_OverlaySlots_guess();
    ~EQ_OverlaySlots_guess();

    char pad[0x90044 - 0x80];
    EQ_OverlaySlot_guess slot0;
    EQ_OverlaySlot_guess slot1;
    EQ_OverlaySlot_guess slot2;
    EQ_OverlaySlot_guess slot3;
    EQ_OverlaySlot_guess slot4;
    EQ_OverlaySlot_guess slot5;
    EQ_OverlaySlot_guess slot6;
    EQ_OverlaySlot_guess slot7;
    EQ_OverlaySlot_guess slot8;
    EQ_OverlaySlot_guess slot9;
    EQ_OverlaySlot_guess slot10;
    EQ_OverlaySlot_guess slot11;
    EQ_OverlaySlot_guess slot12;
    EQ_OverlaySlot_guess slot13;
    EQ_OverlaySlot_guess slot14;
    EQ_OverlaySlot_guess slot15;
    EQ_OverlaySlot_guess slot16;
    EQ_OverlaySlot_guess slot17;
    EQ_OverlaySlot_guess row0[6];
    EQ_OverlaySlot_guess row1[6];
    EQ_OverlaySlot_guess row2[6];
    EQ_OverlaySlot_guess row3[6];
    EQ_OverlaySlot_guess row4[6];
    EQ_OverlaySlot_guess row5[6];
    EQ_OverlaySlot_guess row6[6];
    EQ_OverlaySlot_guess row7[6];
    EQ_OverlaySlot_guess row8[6];
    EQ_OverlaySlot_guess row9[6];
    EQ_OverlaySlot_guess row10[6];
    EQ_OverlaySlot_guess row11[6];
    EQ_OverlaySlot_guess row12[6];
    EQ_OverlaySlot_guess row13[6];
    EQ_OverlaySlot_guess row14[6];
    EQ_OverlaySlot_guess row15[6];
    EQ_OverlaySlot_guess row16[6];
    EQ_OverlaySlot_guess row17[6];
};

// Raw field access for the constructor below: layout unrecovered, so each access is a dword index (OVD, OVW,
// OVB: units of 4 bytes from this) or a byte offset (OVDB, OVWB, OVBB).
#define OVD(i)    (((u32*)this)[i])
#define OVW(i)    (*(u16*)((u32*)this + (i)))
#define OVB(i)    (*(u8*)((u32*)this + (i)))
#define OVDB(off) (*(u32*)((char*)this + (off)))
#define OVWB(off) (*(u16*)((char*)this + (off)))
#define OVBB(off) (*(u8*)((char*)this + (off)))
#define OVRECT(i, l, t, r, b) setRect((RECT*)((u32*)this + (i)), l, t, r, b)
#define OVCLEAR(dst) strcpy((char*)(dst), EQ_ARRAY(char, 0x0058FBA4))
#define OVGET(sec, key, def) \
    EQ_METHOD(3, int, 0x004504FF, (void*, const char*, const char*, int), \
        this)(sec, key, def)

// one 0x18-byte window slot: x, y, w, h, then two flags
#define OVSLOT(b, x, y, w, h, f) \
    OVD((b) + 4) = 1; OVD(b) = x; OVD((b) + 1) = y; OVD((b) + 4) = 1; \
    OVD((b) + 2) = w; OVD((b) + 3) = h; OVD((b) + 4) = 1; OVD((b) + 5) = f
#define OVCFG(b, sec, ky, kx, kh, kw, dy, dx, dh, dw) \
    y = OVGET(sec, ky, dy); \
    x = OVGET(sec, kx, dx); \
    OVD((b) + 4) = 1; OVD(b) = x; OVD((b) + 1) = y; \
    h = OVGET(sec, kh, dh); \
    w = OVGET(sec, kw, dw); \
    OVD((b) + 4) = 1; OVD((b) + 2) = w; OVD((b) + 3) = h
#define OVFLAGS(b, v) OVD((b) + 4) = 1; OVD((b) + 5) = v
#define OVCOLOR(i, sec, kr, kg, kb, kt, dt) \
    red = (u8)OVGET(sec, kr, 1); \
    green = (u8)OVGET(sec, kg, 1); \
    blue = (u8)OVGET(sec, kb, 1); \
    trans = (u8)OVGET(sec, kt, dt); \
    OVD(i) = ((((trans << 8) | red) << 8 | green) << 8) | blue

#define OVS_640    EQ_ARRAY(char, 0x0055A944)
#define OVS_800    EQ_ARRAY(char, 0x0055A7D8)
#define OVS_1024   EQ_ARRAY(char, 0x0055A7CC)
#define OVS_1280   EQ_ARRAY(char, 0x0055A7C0)
#define OVS_1600   EQ_ARRAY(char, 0x0055A7B4)
#define OVS_COLORS EQ_ARRAY(char, 0x0055A6B4)

#define OVCFG_PLAYER(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A938), EQ_ARRAY(char, 0x0055A92C), \
          EQ_ARRAY(char, 0x0055A920), EQ_ARRAY(char, 0x0055A914), dy, dx, dh, dw)
#define OVCFG_PARTY(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A908), EQ_ARRAY(char, 0x0055A8FC), \
          EQ_ARRAY(char, 0x0055A8F0), EQ_ARRAY(char, 0x0055A8E4), dy, dx, dh, dw)
#define OVCFG_TARGET(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A8D8), EQ_ARRAY(char, 0x0055A8CC), \
          EQ_ARRAY(char, 0x0055A8C0), EQ_ARRAY(char, 0x0055A8B4), dy, dx, dh, dw)
#define OVCFG_HOTBUTTON(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A8A4), EQ_ARRAY(char, 0x0055A894), \
          EQ_ARRAY(char, 0x0055A884), EQ_ARRAY(char, 0x0055A874), dy, dx, dh, dw)
#define OVCFG_SPELLS(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A868), EQ_ARRAY(char, 0x0055A85C), \
          EQ_ARRAY(char, 0x0055A850), EQ_ARRAY(char, 0x0055A844), dy, dx, dh, dw)
#define OVCFG_CHAT(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A838), EQ_ARRAY(char, 0x0055A82C), \
          EQ_ARRAY(char, 0x0055A820), EQ_ARRAY(char, 0x0055A814), dy, dx, dh, dw)
#define OVCFG_AFFECT(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A808), EQ_ARRAY(char, 0x0055A7FC), \
          EQ_ARRAY(char, 0x0055A7F0), EQ_ARRAY(char, 0x0055A7E4), dy, dx, dh, dw)
#define OVCFG_MAINMENU(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A7A4), EQ_ARRAY(char, 0x0055A794), \
          EQ_ARRAY(char, 0x0055A784), EQ_ARRAY(char, 0x0055A774), dy, dx, dh, dw)
#define OVCFG_ITEMDATA(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A764), EQ_ARRAY(char, 0x0055A754), \
          EQ_ARRAY(char, 0x0055A744), EQ_ARRAY(char, 0x0055A734), dy, dx, dh, dw)
#define OVCFG_INVENTORY(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A724), EQ_ARRAY(char, 0x0055A714), \
          EQ_ARRAY(char, 0x0055A704), EQ_ARRAY(char, 0x0055A6F4), dy, dx, dh, dw)
#define OVCFG_TRACK(b, sec, dy, dx, dh, dw) \
    OVCFG(b, sec, EQ_ARRAY(char, 0x0055A6E8), EQ_ARRAY(char, 0x0055A6DC), \
          EQ_ARRAY(char, 0x0055A6D0), EQ_ARRAY(char, 0x0055A6C4), dy, dx, dh, dw)

// the seven per-resolution windows of one resolution block; k selects the block (b = base + 8 * k)
#define OVFLAGS_7(p, pa, t, h, s, c, a) \
    OVFLAGS(p, 1); OVFLAGS(pa, 1); OVFLAGS(t, 1); OVFLAGS(h, 1); \
    OVFLAGS(s, 1); OVFLAGS(c, 1); OVFLAGS(a, 1)

// FUNCTION: TRILOGY 0x00451bf4
// Constructor for the old-UI main-screen layout object; OVD/OVB/OVWB are field accessors on this object.
// Phases: zero the state fields and tables; set the element rectangles through the SetRect import; build each of
// the five resolution layouts (640, 800, 1024, 1280, 1600) with OVCFG window rectangles (OVGET with defaults),
// mark the seven windows of each block with OVFLAGS_7; then the main menu, item data, inventory and track
// windows, and finally the window colours with OVCOLOR.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
EQ_OverlaySlots_guess::EQ_OverlaySlots_guess()
{
    tSetRect_guess_t setRect;
    int i;
    u32* p;
    char* s;
    u16* q;
    u8* bp;
    RECT* r;
    int count;
    int row;
    int x, y, w, h;
    u8 red, green, blue, trans;
    u8 cls;
    RECT* r2;

    OVD(0x3b5) = 0xffffffff;
    OVB(0x3f8) = 0;
    OVD(0x20) = 0;
    p = &OVD(0x42);
    for (i = 8; i != 0; i--)
        *p++ = 0;
    OVD(0x4a) = 0;
    OVD(0x4b) = 0;
    OVD(0x4c) = 0;
    OVD(0x4d) = 0;
    OVD(0x50c) = 0;
    OVD(0x50d) = 0;
    p = &OVD(0x36e);
    for (i = 0x28; i != 0; i--)
        *p++ = 0;
    OVD(0x36c) = 0xffffffff;
    OVD(0x36d) = 0;
    OVD(0x396) = 0;
    OVD(0x21) = 0;
    OVD(0x23) = 0;
    OVD(0x24) = 0;
    OVD(0x25) = 0;
    OVD(0x1b242) = 0;
    OVD(0x397) = 0;
    OVD(0x26) = 0;
    OVD(0x27) = 0;
    OVD(0x28) = 0;
    OVD(0x29) = 0;
    OVD(0x3f7) = 0;
    OVD(0x2b) = 0;
    OVD(0x2c) = 0;
    OVWB(0x102) = 0;
    OVW(0x41) = 0;
    OVB(0x40) = 0;
    OVBB(0x101) = 0;
    OVD(0x2a) = 0;
    OVD(0x22) = 0;
    OVBB(0xeda) = 0;
    p = &OVD(0x4e);
    for (i = 0x14; i != 0; i--)
        *p++ = 0;
    OVBB(0xfe) = 0;
    OVW(0x3f) = 0;
    OVCLEAR(&OVD(0x62));
    OVCLEAR((char*)this + 0x1a6);
    OVW(0x3e) = 0;
    OVWB(0xfa) = 0;
    OVWB(0xf2) = 0;
    OVW(0x3d) = 0;
    OVWB(0xdaa) = 0xb;
    OVW(0x36b) = 3;
    OVBB(0xdae) = 0;
    OVBB(0xf6) = 0;
    p = (u32*)((char*)this + 0x6c8b6);
    for (i = 0xf; i != 0; i--)
        *p++ = 0xffffffff;
    p = &OVD(0x1a17f);
    i = 1000;
    do
    {
        p[-1] = 0;
        *p = 0xbf800000;
        p[1] = 0xffffffff;
        p[2] = 0;
        p = p + 4;
        i = i - 1;
    } while (i != 0);
    EQ_GLOBAL(u32, 0x006B676C) = 0;
    EQ_GLOBAL(u32, 0x006B6764) = EQ_CDECL(u32, 0x004A461E, (void))();
    OVD(0x404) = 0;
    OVD(0x405) = 0;
    OVWB(0xda6) = 0;
    OVW(0x36a) = 0xffff;
    count = 0x16;
    OVD(0x1b165) = 0xffffffff;
    OVD(0x1b166) = 0xffffffff;
    OVD(0x1b167) = 0xffffffff;
    OVD(0x1b168) = 0xffffffff;
    s = (char*)&OVD(0x409);
    OVD(0x407) = 0;
    OVD(0x408) = 0;
    q = (u16*)&OVD(0x4b9);
    do
    {
        OVCLEAR(s);
        *q = 0xffff;
        s = s + 0x20;
        q = q + 1;
        count = count - 1;
    } while (count != 0);
    OVCLEAR(&OVD(0x4c4));
    OVCLEAR(&OVD(0x504));
    OVCLEAR(&OVD(0x23efe));
    OVCLEAR(&OVD(0x23f06));
    OVCLEAR(&OVD(0x23f0e));
    OVCLEAR((char*)this + 0x8fc3d);
    OVBB(0x8fc41) = 0;
    OVBB(0x8fc42) = 0;
    OVCLEAR((char*)this + 0x8fc43);
    OVB(0x403) = 0;
    OVD(0x1b136) = 0;
    OVD(0x1b138) = 0;
    OVD(0x1b137) = 0;
    p = &OVD(0x398);
    *(float*)&OVD(0x406) = 0.0f;
    OVBB(0xfe1) = 0;
    OVBB(0xfe2) = 0;
    OVW(0x3a8) = 0xffff;
    i = 8;
    do
    {
        p[8] = 0;
        *p = 4;
        p = p + 1;
        i = i - 1;
    } while (i != 0);
    OVB(0x3b4) = 0;
    OVD(0x3a9) = 1;
    OVD(0x3aa) = 1;
    OVD(0x3ab) = 1;
    OVD(0x3ac) = 1;
    OVWB(0xebe) = 0xffff;
    OVW(0x3b0) = 0xffff;
    OVWB(0xec2) = 0;
    OVW(0x3af) = 0xffff;
    OVD(0x1a179) = 0;
    OVD(0x1b15f) = 0xffffffff;
    OVD(0x1b160) = 0xffffffff;
    OVW(0x1b161) = 0xffff;
    OVD(4) = 0;
    OVD(0x1c) = 0;
    OVW(0x17c61) = 0xffff;
    OVD(0x3ae) = 1;
    OVB(0x3f8) = 0;
    OVD(0x1b23f) = 1;
    OVD(0x23eea) = 0;
    OVBB(0x100d) = 0;
    OVBB(0x6841d) = 0;
    OVB(0x2d) = 0xff;
    OVBB(0xb5) = 0;
    OVCLEAR((char*)this + 0xb6);
    s = (char*)&OVD(0x17c30);
    OVD(0x1b155) = 0;
    OVWB(0x685ca) = 0xffff;
    OVD(0x1b157) = 0;
    OVW(0x1b158) = 0;
    OVD(0x1b159) = 0;
    OVD(0x23ee4) = 0;
    OVD(0x1a175) = 0;
    OVD(0x1a176) = 0;
    OVW(0x1b15a) = 0;
    OVD(0x23ee9) = 0;
    OVD(0x1a177) = 0;
    OVD(0x1a178) = 0;
    OVD(0x1a17a) = 0;
    OVD(0x1a17d) = 0;
    OVD(0x17c5d) = 0;
    OVD(0x17c60) = 0;
    OVW(0x1a172) = 0;
    OVW(0x1a173) = 0;
    OVWB(0x685ce) = 0xffff;
    OVW(0x1a174) = 0;
    OVWB(0x685d2) = 0xffff;
    p = &OVD(0x17c56);
    OVD(0x17c2f) = 0;
    count = 5;
    do
    {
        *p = 0;
        OVCLEAR(s);
        p = p + 1;
        s = s + 0x1e;
        count = count - 1;
    } while (count != 0);
    OVD(0x17c5c) = 0;
    OVD(0x17c5e) = 0;
    OVD(0x17c5f) = 0;
    OVW(0x402) = 0xffff;
    OVWB(0x100a) = 0;
    OVD(0x1b240) = 0;
    OVW(0x23eed) = 0;
    OVBB(0x8fbad) = 0;
    OVD(0x23eec) = 0;
    OVWB(0x8fbb6) = 0xffff;
    OVW(0x1b15b) = 0xffff;
    OVWB(0x6c56a) = 0xffff;
    OVW(0x1b15c) = 0xffff;
    OVWB(0x6c56e) = 0xffff;
    OVDB(0x6c572) = 0xffffffff;
    OVDB(0x6c576) = 0xffffffff;
    OVWB(0x6c57a) = 0xffff;
    OVD(0x17c63) = 0;
    OVD(0x17c64) = 0;
    OVB(0x17c65) = 0xff;
    OVBB(0x5f195) = 0xff;
    OVB(0x1a107) = 0;
    OVBB(0x8fbd2) = 0;
    bp = (u8*)this + 0x5f2cb;
    row = 10;
    do
    {
        count = 0xc;
        do
        {
            OVCLEAR(bp - 0x135);
            *bp = 0;
            s = (char*)(bp - 300);
            i = 5;
            do
            {
                OVCLEAR(s);
                s = s + 0x3c;
                i = i - 1;
            } while (i != 0);
            bp = bp + 0x136;
            count = count - 1;
        } while (count != 0);
        row = row - 1;
    } while (row != 0);
    p = &OVD(0x17c0e);
    for (i = 0x19; i != 0; i--)
        *p++ = 0;
    OVCLEAR((char*)this + 0x682e6);
    OVBB(0x6841b) = 0;
    s = (char*)this + 0x682ef;
    row = 5;
    do
    {
        OVCLEAR(s);
        s = s + 0x3c;
        row = row - 1;
    } while (row != 0);
    p = &OVD(0x1a108);
    i = 10;
    do
    {
        count = 10;
        do
        {
            *((u8*)p - 2) = 0;
            *(short*)p = -1;
            p = p + 1;
            count = count - 1;
        } while (count != 0);
        i = i - 1;
    } while (i != 0);
    p = &OVD(0x1b143);
    i = 8;
    do
    {
        p[-8] = 0;
        *p = 0;
        p = p + 1;
        i = i - 1;
    } while (i != 0);
    p = &OVD(0x1b14b);
    i = 4;
    do
    {
        p[4] = 0;
        *p = 0;
        p = p + 1;
        i = i - 1;
    } while (i != 0);
    OVD(0x1b13a) = 0;
    OVD(0x1a17b) = 0;
    OVD(0x1a17c) = 0;
    OVD(0x17c5b) = 0;
    OVWB(0x1006) = 0xffff;
    OVW(0x401) = 0xffff;
    OVW(0x400) = 0;
    OVWB(0x1002) = 0;
    OVD(0x1b162) = 0;
    OVBB(0xfe2) = 0;
    OVW(0x1b164) = 0xffff;
    OVW(0x3fa) = 0;
    OVD(0x3f9) = 0;
    OVD(0x1b23d) = 1;
    OVD(0x23ee3) = 0;
    OVD(0x1b196) = 0;
    OVB(0x1b241) = 0;
    OVW(0x1b197) = 0xffff;
    OVBB(0x6c592) = 0;
    p = &OVD(0x1b178);
    for (i = 0x1e; i != 0; i--)
        *p++ = 0;
    q = (u16*)((char*)this + 0x6c7ee);
    i = 100;
    do
    {
        q[-200] = 0xffff;
        *q = 0xffff;
        q[-100] = 0xffff;
        q = q + 1;
        i = i - 1;
    } while (i != 0);
    OVD(0x3fb) = 0xffffffff;
    OVD(0x3fc) = 0xffffffff;
    OVD(0x3fd) = 0xffffffff;
    OVD(0x3fe) = 0xffffffff;
    OVD(0x3ff) = 0xffffffff;
    p = &OVD(0x1b143);
    i = 8;
    do
    {
        p[-8] = 0;
        *p = 0;
        p = p + 1;
        i = i - 1;
    } while (i != 0);
    setRect = g_setrect_guess;
    OVD(0x1a16e) = 0;
    OVD(0x1a16f) = 0;
    OVB(0x1b169) = 0;
    OVB(0x23eeb) = 0;
    OVD(0x23ee5) = 0;
    OVD(0x23ee6) = 0;
    OVD(0x23ee7) = 0;
    OVD(0x23ee8) = 0;
    OVRECT(9, 0, 0, 0x640, 0x4b0);
    OVRECT(5, 0, 0, 0x280, 0x1e0);
    strcpy(
        (char*)&OVD(0xd), EQ_ARRAY(char, 0x0053E950));
    strcpy(
        (char*)this + 0x52, EQ_ARRAY(char, 0x0055A950));
    EQ_METHOD(2, void, 0x0046B067, (void*, int, int), this)(0, 0);
    OVD(0x1e) = 0;
    p = &OVD(0x23eef);
    i = 6;
    do
    {
        *(short*)((char*)p - 2) = -1;
        *(u16*)p = 0;
        p = p + 1;
        i = i - 1;
    } while (i != 0);
    r = (RECT*)&OVD(0x1b243);
    row = 0xf;
    do
    {
        count = 400;
        do
        {
            setRect(r, -1, -1, -1, -1);
            r = r + 1;
            count = count - 1;
        } while (count != 0);
        row = row - 1;
    } while (row != 0);
    r = (RECT*)&OVD(0x1b5b7);
    row = 4;
    do
    {
        setRect(r - 1, 0xc, 0x10, 0x50, 0x21);
        setRect(r, 0xc, 0x25, 0x50, 0x36);
        setRect(r + 0xf, 0x37, 0x135, 0x44, 0x140);
        if (EQ_GLOBAL(void*, 0x006E4DA8) == 0 ||
            (cls = *((u8*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36)) == 1 || cls == 7 || cls == 9)
        {
            setRect(r + 1, 0xc, 0x41, 0x50, 0x100);
        }
        else
        {
            setRect(r + 1, 0xc, 0x41, 0x50, 0xcd);
            setRect(r + 2, 0xc, 0xce, 0x50, 0x112);
        }
        setRect(r + 3, 0xc, 0x11e, 0x50, 0x12f);
        setRect(r - 0x15, 0x10, 0x146, 0x3e, 0x174);
        setRect(r - 0x14, 0x41, 0x146, 0x6f, 0x174);
        setRect(r - 0x13, 0x10, 0x177, 0x3e, 0x1a5);
        setRect(r - 0x12, 0x41, 0x177, 0x6f, 0x1a5);
        setRect(r - 0x11, 0x10, 0x1a8, 0x3e, 0x1d6);
        setRect(r - 0x10, 0x41, 0x1a8, 0x6f, 0x1d6);
        setRect(r - 0xb, 0x24, 0x136, 0x36, 0x140);
        setRect(r - 10, 0x46, 0x136, 0x58, 0x140);
        y = 0x10;
        r2 = r - 9;
        count = 8;
        do
        {
            setRect(r2, 0x58, y, 0x76, y + 0x16);
            r2 = r2 + 1;
            y = y + 0x26;
            count = count - 1;
        } while (count != 0);
        setRect(r + 0x1f, 0x215, 0xb, 0x272, 0x4f);
        setRect(r + 0x20, 0x215, 0x68, 0x273, 0xdf);
        setRect(r + 4, 0x212, 0x53, 0x276, 100);
        setRect(r + 5, 0x212, 0xec, 0x277, 0xf9);
        setRect(r + 6, 0x212, 0xfd, 0x276, 0x10e);
        setRect(r + 7, 0x212, 0x112, 0x276, 0x123);
        setRect(r + 8, 0x212, 0x127, 0x276, 0x138);
        setRect(r + 0x10, 0x20e, 0x141, 0x280, 0x1e0);
        setRect(r + 0x11, 0x70, 0x147, 0x208, 0x1e0);
        setRect(r + 0x21, 0x215, 0x68, 0x272, 0x82);
        setRect(r + 0x22, 0x215, 0x83, 0x272, 0x98);
        setRect(r + 0x23, 0x215, 0x99, 0x272, 0xae);
        setRect(r + 0x24, 0x215, 0xaf, 0x272, 0xc4);
        setRect(r + 0x25, 0x215, 0xc5, 0x272, 0xdf);
        setRect(r - 0x76, 8, 0x30, 0x6d, 0x41);
        setRect(r - 0x75, 8, 0x56, 0x6d, 0x67);
        setRect(r - 0x74, 8, 0x7c, 0x6d, 0x8d);
        setRect(r - 0x73, 8, 0xa2, 0x6d, 0xb3);
        setRect(r - 0x72, 8, 200, 0x6d, 0xd9);
        setRect(r - 0x6f, 0x24, 0x1be, 0x52, 0x1cf);
        setRect(r - 0x6e, 8, 0x1be, 0x36, 0x1cf);
        setRect(r - 0x6d, 0x3e, 0x1be, 0x6c, 0x1cf);
        setRect(r - 0x70, 0, 0, 0x77, 0x1e0);
        r = r + 400;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1b5db, 0x212, 0x14b, 0x276, 0x15c);
    OVRECT(0x1b5df, 0x212, 0x162, 0x276, 0x173);
    OVRECT(0x1b5e3, 0x212, 0x179, 0x276, 0x18a);
    OVRECT(0x1b5e7, 0x212, 400, 0x276, 0x1a1);
    OVRECT(0x1b5eb, 0x212, 0x1a7, 0x276, 0x1b8);
    OVRECT(0x1b5ef, 0x212, 0x1be, 0x276, 0x1cf);
    OVRECT(0x1bc43, 0x216, 0x149, 0x23e, 0x171);
    OVRECT(0x1bc47, 0x247, 0x149, 0x26f, 0x171);
    OVRECT(0x1bc4b, 0x216, 0x17a, 0x23e, 0x1a2);
    OVRECT(0x1bc4f, 0x247, 0x17a, 0x26f, 0x1a2);
    OVRECT(0x1bc53, 0x216, 0x1ab, 0x23e, 0x1d2);
    OVRECT(0x1bc57, 0x247, 0x1ab, 0x26f, 0x1d2);
    OVRECT(0x1c29b, 0x216, 0x149, 0x23e, 0x171);
    OVRECT(0x1c29f, 0x247, 0x149, 0x26f, 0x171);
    OVRECT(0x1c2a3, 0x216, 0x17a, 0x23e, 0x1a2);
    OVRECT(0x1c2a7, 0x247, 0x17a, 0x26f, 0x1a2);
    OVRECT(0x1c2ab, 0x216, 0x1ab, 0x23e, 0x1d2);
    OVRECT(0x1c2af, 0x247, 0x1ab, 0x26f, 0x1d2);
    r = (RECT*)&OVD(0x1c5ff);
    y = 0x14b;
    row = 6;
    do
    {
        setRect(r - 6, 0x212, y, 0x240, y + 0x11);
        setRect(r, 0x246, y, 0x274, y + 0x11);
        y = y + 0x17;
        r = r + 1;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1c617, 0xd1, 0x21, 0x10f, 0x159);
    OVRECT(0x1c61b, 0x19b, 0x149, 0x1ad, 0x15b);
    OVRECT(0x1c61f, 0x75, 0x15e, 0x207, 0x170);
    OVRECT(0x1c623, 0x75, 0x172, 0x207, 0x184);
    OVRECT(0x1c627, 0x75, 0x186, 0x207, 0x198);
    OVRECT(0x1c62b, 0x75, 0x19a, 0x207, 0x1ac);
    OVRECT(0x1c62f, 0x75, 0x1ae, 0x207, 0x1c0);
    OVRECT(0x1c633, 0x75, 0x1c4, 0xa3, 0x1d5);
    OVRECT(0x1c637, 0x1d9, 0x1c4, 0x207, 0x1d5);
    OVRECT(0x1cb47, 0x11e, 0xc, 0x146, 0x34);
    OVRECT(0x1cb4b, 0x1b1, 0xc, 0x1d9, 0x34);
    OVRECT(0x1cb4f, 0x180, 0xc, 0x1a8, 0x34);
    OVRECT(0x1cb53, 0x1e2, 0xc, 0x20a, 0x34);
    OVRECT(0x1cb57, 0x14f, 0xc, 0x177, 0x34);
    OVRECT(0x1cb7f, 0xed, 0x4d, 0x115, 0x75);
    OVRECT(0x1cb67, 0x11e, 0x4d, 0x146, 0x75);
    OVRECT(0x1cb5f, 0x14f, 0x4d, 0x177, 0x75);
    OVRECT(0x1cb73, 0x180, 0x4d, 0x1a8, 0x75);
    OVRECT(0x1cb6b, 0x1b1, 0x4d, 0x1d9, 0x75);
    OVRECT(0x1cb83, 0x1e2, 0x4d, 0x20a, 0x75);
    OVRECT(0x1cb5b, 0xed, 0x8e, 0x115, 0xb6);
    OVRECT(0x1cb87, 0x11e, 0x8e, 0x146, 0xb6);
    OVRECT(0x1cb63, 0x14f, 0x8e, 0x177, 0xb6);
    OVRECT(0x1cb93, 0x180, 0x8e, 0x1a8, 0xb6);
    OVRECT(0x1cb8b, 0x1b1, 0x8e, 0x1d9, 0xb6);
    OVRECT(0x1cb8f, 0x1e2, 0x8e, 0x20a, 0xb6);
    OVRECT(0x1cb77, 0x14f, 0xcf, 0x177, 0xf7);
    OVRECT(0x1cb7b, 0x180, 0xcf, 0x1a8, 0xf7);
    OVRECT(0x1cb6f, 0x1b1, 0xcf, 0x1d9, 0xf7);
    OVRECT(0x1cb97, 0x1e2, 0xcf, 0x20a, 0xf7);
    r = (RECT*)&OVD(0x1cbab);
    y = 0x101;
    row = 4;
    do
    {
        setRect(r - 4, 0x218, y, 0x240, y + 0x28);
        setRect(r, 0x249, y, 0x271, y + 0x28);
        y = y + 0x31;
        r = r + 1;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1cbbb, 10, 0xc, 0x37, 0x1d);
    OVRECT(0x1cbbf, 0x3e, 0xc, 0x6b, 0x1d);
    OVRECT(0x1cbc3, 0x211, 0x1c, 0x277, 0xe3);
    OVRECT(0x1cbc7, 0x21e, 0xe8, 0x26b, 0xf9);
    OVRECT(0x1cbcb, 0x22d, 0x1c3, 0x25b, 0x1d4);
    OVRECT(0x1cbdf, 0x212, 8, 0x276, 0x19);
    OVRECT(0x1cefb, 0x70, 0x147, 0x208, 0x1e0);
    OVRECT(0x1cbe3, 0x61, 0x23, 0x6c, 0x35);
    OVRECT(0x1cbe7, 0x61, 0x1c1, 0x6c, 0x1d3);
    OVRECT(0x1cbeb, 0x61, 0x35, 0x6b, 0x40);
    OVRECT(0x1cfcf, 0x10, 0x1b2, 0x36, 0x1d2);
    OVRECT(0x1cfd3, 0x41, 0x1b2, 0x67, 0x1d2);
    r = (RECT*)&OVD(0x1cf93);
    y = 0x25;
    row = 8;
    do
    {
        setRect(r - 1, 0xb, y, 0x39, y + 0x2e);
        setRect(r, 0x3c, y, 0x6a, y + 0x2e);
        y = y + 0x31;
        r = r + 2;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1cfd7, 0x1c, 0x1b, 0x5a, 0x31);
    OVRECT(0x1cfdb, 0x1c, 0x19b, 0x5a, 0x1b1);
    OVRECT(0x1cff3, 0xc, 0x23, 0x3a, 0x51);
    OVRECT(0x1cff7, 0x3d, 0x23, 0x6b, 0x51);
    OVRECT(0x1cffb, 0xc, 0x54, 0x3a, 0x82);
    OVRECT(0x1cfff, 0x3d, 0x54, 0x6b, 0x82);
    OVRECT(0x1d003, 0xc, 0x85, 0x3a, 0xb3);
    OVRECT(0x1d007, 0x3d, 0x85, 0x6b, 0xb3);
    OVRECT(0x1d00b, 0xc, 0xb6, 0x3a, 0xe4);
    OVRECT(0x1d00f, 0x3d, 0xb6, 0x6b, 0xe4);
    OVRECT(0x1d12f, 10, 0xeb, 0x6e, 0x13e);
    OVRECT(0x1d04f, 0x2d, 0x164, 0x54, 0x18b);
    OVRECT(0x1d053, 0x24, 0x1ae, 0x5f, 0x1c9);
    OVRECT(0x1d01b, 0x76, 0x23, 0xa4, 0x51);
    OVRECT(0x1d01f, 0xa7, 0x23, 0xd5, 0x51);
    OVRECT(0x1d023, 0x76, 0x54, 0xa4, 0x82);
    OVRECT(0x1d027, 0xa7, 0x54, 0xd5, 0x82);
    OVRECT(0x1d02b, 0x76, 0x85, 0xa4, 0xb3);
    OVRECT(0x1d02f, 0xa7, 0x85, 0xd5, 0xb3);
    OVRECT(0x1d033, 0x76, 0xb6, 0xa4, 0xe4);
    OVRECT(0x1d037, 0xa7, 0xb6, 0xd5, 0xe4);
    OVRECT(0x1d03b, 0xc0, 0xf4, 0xcf, 0x103);
    OVRECT(0x1d03f, 0xc0, 0x105, 0xcf, 0x114);
    OVRECT(0x1d043, 0xc0, 0x115, 0xcf, 0x125);
    OVRECT(0x1d047, 0xc0, 0x126, 0xcf, 0x135);
    OVRECT(0x1d04b, 0x73, 0xeb, 0xd7, 0x13e);
    OVRECT(0x1cdeb, 0xc, 0x5a, 0x3a, 0x88);
    OVRECT(0x1cdef, 0xc, 0x8b, 0x3a, 0xb9);
    OVRECT(0x1cdf3, 0xc, 0xbc, 0x3a, 0xea);
    OVRECT(0x1cdf7, 0xc, 0xed, 0x3a, 0x11b);
    OVRECT(0x1cdfb, 0x3d, 0x5a, 0x6b, 0x88);
    OVRECT(0x1cdff, 0x3d, 0x8b, 0x6b, 0xb9);
    OVRECT(0x1ce03, 0x3d, 0xbc, 0x6b, 0xea);
    OVRECT(0x1ce07, 0x3d, 0xed, 0x6b, 0x11b);
    OVRECT(0x1ce13, 7, 0x142, 0x67, 0x154);
    OVRECT(0x1ce17, 7, 0x165, 0x67, 0x177);
    OVRECT(0x1ce1b, 7, 0x189, 0x67, 0x19b);
    OVRECT(0x1ce1f, 7, 0x1ad, 0x67, 0x1bf);
    OVRECT(0x1ce23, 0, 0, 0x77, 0x1e0);
    OVRECT(0x1ccdb, 0xe0, 0x104, 0x20d, 0x13f);
    OVRECT(0x1cbcf, 0xed, 0x10b, 0x12f, 0x139);
    OVRECT(0x1cbd3, 0x135, 0x10b, 0x177, 0x139);
    OVRECT(0x1cbd7, 0x17d, 0x10b, 0x1bf, 0x139);
    OVRECT(0x1cbdb, 0x1c5, 0x10b, 0x207, 0x139);
    OVRECT(0x1d163, 0x10, 0x2e, 0x58, 0x67);
    OVRECT(0x1d167, 0x10, 0x75, 0x58, 0xae);
    OVRECT(0x1d16b, 0x10, 0xbe, 0x58, 0xf7);
    OVRECT(0x1d16f, 0x10, 0x107, 0x58, 0x140);
    OVRECT(0x1d173, 0x10, 0x14e, 0x58, 0x187);
    OVRECT(0x1d177, 0x10, 0x192, 0x58, 0x1cb);
    OVRECT(0x1d117, 8, 0x21, 0x70, 0x1d5);
    OVRECT(0x1d11b, 0, 0, 0x77, 0x1e0);
    OVRECT(0x1d11f, 0x111, 0, 0x20d, 0x40);
    OVRECT(0x1d123, 0xe0, 0x41, 0x20d, 0x80);
    OVRECT(0x1d127, 0xe0, 0x82, 0x20d, 0xc1);
    OVRECT(0x1d12b, 0x142, 0xc3, 0x20d, 0x102);
    OVRECT(0x1d113, 0x207, 0, 0x280, 0x1e0);
    OVRECT(0x1d10b, 0, 0, 0x77, 0x1e0);
    OVRECT(0x1d10f, 0, 0, 0xf0, 0x1e0);
    OVRECT(0x1d4f3, 0xc, 0x10, 0x50, 0x21);
    OVRECT(0x1d4f7, 0xc, 0x25, 0x50, 0x36);
    OVRECT(0x1d533, 0x37, 0x135, 0x44, 0x140);
    if (EQ_GLOBAL(void*, 0x006E4DA8) == 0 ||
        (cls = *((u8*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x36)) == 1 || cls == 7 || cls == 9)
    {
        OVRECT(0x1d4fb, 0xc, 0x41, 0x50, 0x100);
    }
    else
    {
        OVRECT(0x1d4fb, 0xc, 0x41, 0x50, 0xcd);
        OVRECT(0x1d4ff, 0xc, 0xce, 0x50, 0x112);
    }
    OVRECT(0x1d503, 0xc, 0x11e, 0x50, 0x12f);
    OVRECT(0x1d4a3, 0x10, 0x146, 0x3e, 0x174);
    OVRECT(0x1d4a7, 0x41, 0x146, 0x6f, 0x174);
    OVRECT(0x1d4ab, 0x10, 0x177, 0x3e, 0x1a5);
    OVRECT(0x1d4af, 0x41, 0x177, 0x6f, 0x1a5);
    OVRECT(0x1d4b3, 0x10, 0x1a8, 0x3e, 0x1d6);
    OVRECT(0x1d4b7, 0x41, 0x1a8, 0x6f, 0x1d6);
    OVRECT(0x1d4cb, 0x24, 0x136, 0x36, 0x140);
    OVRECT(0x1d4cf, 0x46, 0x136, 0x58, 0x140);
    r = (RECT*)&OVD(0x1d4d3);
    y = 0x10;
    row = 8;
    do
    {
        setRect(r, 0x58, y, 0x76, y + 0x16);
        y = y + 0x26;
        r = r + 1;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1d573, 0x215, 0xb, 0x272, 0x4f);
    OVRECT(0x1d577, 0x215, 0x68, 0x273, 0xdf);
    OVRECT(0x1d507, 0x212, 0x53, 0x276, 100);
    OVRECT(0x1d50b, 0x212, 0xec, 0x277, 0xf9);
    OVRECT(0x1d537, 0x20e, 0x141, 0x280, 0x1e0);
    OVRECT(0x1d263, 0x22d, 0x1c3, 0x25b, 0x1d4);
    OVRECT(0x1d53b, 0x70, 0x147, 0x208, 0x1e0);
    OVRECT(0x1d53f, 0x70, 0x147, 0x208, 0x1e0);
    OVRECT(0x1d257, 0x80, 0x115, 0x9f, 0x133);
    OVRECT(0x1d25b, 0x1de, 0x115, 0x1fd, 0x133);
    OVRECT(0x1d25f, 0x121, 0x117, 0x15c, 0x12f);
    r = (RECT*)&OVD(0x1d1eb);
    y = 0x101;
    row = 4;
    do
    {
        setRect(r - 4, 0x218, y, 0x240, y + 0x28);
        setRect(r, 0x249, y, 0x271, y + 0x28);
        y = y + 0x31;
        r = r + 1;
        row = row - 1;
    } while (row != 0);
    OVRECT(0x1d22f, 0xba, 0x2e, 0xe3, 0x56);
    OVRECT(0x1d233, 0x103, 0x28, 300, 0x50);
    OVRECT(0x1d237, 0x103, 0x6c, 300, 0x94);
    OVRECT(0x1d23b, 0xba, 0xa6, 0xe3, 0xce);
    OVRECT(0x1d23f, 0x103, 0xb0, 300, 0xd8);
    OVRECT(0x1d243, 0x157, 0x28, 0x180, 0x50);
    OVRECT(0x1d247, 0x1a3, 0x46, 0x1cc, 0x6e);
    OVRECT(0x1d24b, 0x157, 0x6c, 0x180, 0x94);
    OVRECT(0x1d24f, 0x1a3, 0x90, 0x1cd, 0xb8);
    OVRECT(0x1d253, 0x157, 0xb0, 0x180, 0xd8);
    OVRECT(0x1d57b, 0x21c, 0x6c, 0x26c, 0x82);
    OVRECT(0x1d57f, 0x21c, 0x83, 0x26c, 0x98);
    OVRECT(0x1d583, 0x21c, 0x99, 0x26c, 0xad);
    OVRECT(0x1d587, 0x21c, 0xae, 0x26c, 0xc4);
    OVRECT(0x1d58b, 0x21c, 0xc5, 0x26c, 0xda);
    OVRECT(0x1d773, 0x77, 0, 0x280, 0x1e0);
    OVRECT(0x1d777, 0xb4, 0x11d, 200, 0x131);
    OVRECT(0x1d77b, 0x170, 0x11d, 400, 0x131);
    OVRECT(0x1d77f, 0x20e, 0xfc, 0x280, 0x1e0);
    OVRECT(0x1d787, 0x77, 0, 0x82, 0x1e);
    OVRECT(0x1d78b, 200, 0, 0xa0, 0x1e);
    OVRECT(0x1d78f, 0x140, 0, 0x15e, 0x1e);
    OVRECT(0x1d793, 0x152, 0, 400, 0x1e);
    OVRECT(0x1d797, 0xab, 0x26, 0xb4, 0x28);
    OVRECT(0x1d79b, 0xa6, 0x28, 200, 0x50);
    OVRECT(0x1d79f, 0xa6, 0x27, 200, 0x50);
    OVRECT(0x1d7c3, 0, 0, 0x77, 0x1e0);
    OVRECT(0x1d7c7, 0x207, 0, 0x280, 0x1e0);
    OVRECT(0x1d7cb, 8, 0xe, 0x6d, 0x1b);
    OVRECT(0x1d7cf, 0xe, 0x25, 0x57, 0x5e);
    OVRECT(0x1d7d3, 0xe, 0x65, 0x57, 0x9e);
    OVRECT(0x1d7d7, 0xe, 0xa5, 0x57, 0xde);
    OVRECT(0x1d7db, 0xe, 0xe5, 0x57, 0x11e);
    OVRECT(0x1d7df, 0xe, 0x125, 0x57, 0x15e);
    OVRECT(0x1d7e3, 8, 0x19d, 0x6c, 0x1ae);
    OVRECT(0x1d7e7, 0x26, 0x1bb, 0x54, 0x1cc);
    OVRECT(0x1d7eb, 0x5f, 0x21, 0x69, 0x32);
    OVRECT(0x1d7ef, 0x5f, 0x154, 0x69, 0x165);
    OVRECT(0x1d7f3, 0x210, 0xe, 0x275, 0x1b);
    OVRECT(0x1d7f7, 0x222, 0x60, 0x264, 0x65);
    OVRECT(0x1d7fb, 0x215, 0x8e, 0x25e, 199);
    OVRECT(0x1d7ff, 0x215, 0xce, 0x25e, 0x107);
    OVRECT(0x1d803, 0x215, 0x10e, 0x25e, 0x147);
    OVRECT(0x1d807, 0x215, 0x14e, 0x25e, 0x187);
    OVRECT(0x1d80b, 0x215, 0x18e, 0x25e, 0x1c7);
    OVRECT(0x1d80f, 0x267, 0x8a, 0x271, 0x9b);
    OVRECT(0x1d813, 0x267, 0x1bd, 0x271, 0x1ce);
    OVRECT(0x1d817, 0x5f, 0x32, 0x69, 0x3d);
    OVRECT(0x1d81b, 0x267, 0x9b, 0x271, 0xa6);
    OVRECT(0x1d81f, 0x6e, 0x140, 0x20a, 0x1e0);
    OVRECT(0x1e443, 0x16, 0x1b8, 0x44, 0x1c9);
    OVRECT(0x1e447, 0x4a, 0x1b8, 0x78, 0x1c9);
    OVRECT(0x1e44b, 0x1fd, 0x1b8, 0x22b, 0x1c9);
    OVRECT(0x1e44f, 0x23b, 0x1b8, 0x269, 0x1c9);
    OVRECT(0x1ea83, 0x66, 0x26, 0xca, 0x37);
    OVRECT(0x1ea87, 0xd1, 0x26, 0x135, 0x37);
    OVRECT(0x1ea8b, 0x13c, 0x26, 0x1a0, 0x37);
    OVRECT(0x1ea8f, 0x1a6, 0x26, 0x20a, 0x37);
    OVRECT(0x1ea93, 0x250, 0x3f, 0x25a, 0x50);
    OVRECT(0x1ea97, 0x250, 0x179, 0x25a, 0x18a);
    OVRECT(0x1ea9b, 0x250, 0x51, 0x25a, 0x5c);
    OVRECT(0x1ea9f, 0x24, 0x1a2, 0x5f, 0x1bd);
    OVRECT(0x1eaa3, 0x66, 0x1a2, 0xa1, 0x1bd);
    OVRECT(0x1eaa7, 0xc1, 0x19c, 0x125, 0x1ad);
    OVRECT(0x1eaab, 0xc1, 0x1b4, 0x125, 0x1c5);
    OVRECT(0x1eaaf, 300, 0x19c, 400, 0x1ad);
    OVRECT(0x1eab3, 300, 0x1b4, 400, 0x1c5);
    OVRECT(0x1eab7, 0x197, 0x19c, 0x1fb, 0x1ad);
    OVRECT(0x1eabb, 0x197, 0x1b4, 0x1fb, 0x1c5);
    OVRECT(0x1eabf, 0x21f, 0x1a2, 0x25a, 0x1bd);
    r = (RECT*)&OVD(0x1eac3);
    row = 0x12;
    y = 0x55;
    do
    {
        setRect(r, 0x46, y, 0x226, y + 0xf);
        r = r + 1;
        row = row - 1;
        y = y + 0xf;
    } while (row != 0);
    OVRECT(0x1f0c3, 0x24, 0x1a2, 0x5f, 0x1bd);
    OVRECT(0x1f0c7, 0x66, 0x1a2, 0xa1, 0x1bd);
    OVRECT(0x1f0cb, 0x21f, 0x1a2, 0x25a, 0x1bd);
    OVRECT(0x1f703, 0x24, 0x1a2, 0x5f, 0x1bd);
    OVRECT(0x1f707, 0x66, 0x1a2, 0xa1, 0x1bd);
    OVRECT(0x1f70b, 0x125, 0x197, 0x189, 0x1a8);
    OVRECT(0x1f70f, 0x21f, 0x1a2, 0x25a, 0x1bd);
    OVRECT(0x1f713, 0x46, 0x78, 0x226, 0x91);
    OVRECT(0x1f717, 0x46, 0x96, 0x226, 0x145);
    OVRECT(0x1fd43, 0x23b, 0x1b8, 0x269, 0x1c9);
    OVSLOT(0x240a1, 0x214, 8, 0x5f, 0x46, 1);
    OVSLOT(0x240d1, 0x20a, 0x6c, 0x73, 0x6e, 1);
    OVSLOT(0x24101, 0x20a, 0xda, 0x73, 0x22, 1);
    OVSLOT(0x24161, 7, 0x135, 0x6b, 0xa4, 1);
    OVSLOT(0x24131, 0x46, 10, 0x32, 0xf5, 1);
    OVSLOT(0x24191, 0x78, 0x14a, 0x192, 0x8f, 1);
    OVSLOT(0x24251, 0x1e0, 2, 100, 0x154, 1);
    OVSLOT(0x24371, 0x78, 0x14a, 0x192, 0x8f, 0);
    OVSLOT(0x243a1, 0x20a, 0xe6, 0x78, 200, 0);
    OVSLOT(0x24281, 0, 0, 0x280, 0x1e0, 0);
    OVSLOT(0x243d1, 0, 0, 0x78, 0x1e0, 0);

    OVCFG_PLAYER(0x240a9, OVS_640, 2, 0x208, 0x46, 0x5f);
    OVCFG_PARTY(0x240d9, OVS_640, 0x14f, 0x20a, 0x6e, 0x73);
    OVCFG_TARGET(0x24109, OVS_640, 0x4c, 0x1fe, 0x22, 0x73);
    OVCFG_HOTBUTTON(0x24169, OVS_640, 0x139, 2, 0xec, 0x5c);
    OVCFG_SPELLS(0x24139, OVS_640, 0x1b, 2, 0xd8, 0x28);
    OVCFG_CHAT(0x24199, OVS_640, 0x146, 0x72, 0x95, 0x192);
    OVCFG_AFFECT(0x24259, OVS_640, 0x74, 0x1fc, 0xda, 0x80);
    OVFLAGS_7(0x240a9, 0x240d9, 0x24109, 0x24169, 0x24139, 0x24199, 0x24259);

    OVCFG_PLAYER(0x240b1, OVS_800, 8, 0x2b3, 0x46, 0x5f);
    OVCFG_PARTY(0x240e1, OVS_800, 0x6c, 0x2a9, 0x6e, 0x73);
    OVCFG_TARGET(0x24111, OVS_800, 0xda, 0x2a9, 0x22, 0x73);
    OVCFG_HOTBUTTON(0x24171, OVS_800, 0x1ab, 6, 0xec, 0x5c);
    OVCFG_SPELLS(0x24141, OVS_800, 0x1b, 2, 0xd8, 0x28);
    OVCFG_CHAT(0x241a1, OVS_800, 0x19c, 0x96, 0x95, 0x1f6);
    OVCFG_AFFECT(0x24261, OVS_800, 0x17a, 0x29e, 0xda, 0x80);
    OVFLAGS_7(0x240b1, 0x240e1, 0x24111, 0x24171, 0x24141, 0x241a1, 0x24261);

    OVCFG_PLAYER(0x240b9, OVS_1024, 8, 0x393, 0x46, 0x5f);
    OVCFG_PARTY(0x240e9, OVS_1024, 0x6c, 0x389, 0x6e, 0x73);
    OVCFG_TARGET(0x24119, OVS_1024, 0xda, 0x389, 0x22, 0x73);
    OVCFG_HOTBUTTON(0x24179, OVS_1024, 0x253, 6, 0xec, 0x5c);
    OVCFG_SPELLS(0x24149, OVS_1024, 0x1b, 2, 0xd8, 0x28);
    OVCFG_CHAT(0x241a9, OVS_1024, 0x24a, 0xbe, 0x95, 0x27c);
    OVCFG_AFFECT(0x24269, OVS_1024, 0x223, 0x37c, 0xda, 0x80);
    OVFLAGS_7(0x240b9, 0x240e9, 0x24119, 0x24179, 0x24149, 0x241a9, 0x24269);

    OVCFG_PLAYER(0x240c1, OVS_1280, 8, 0x393, 0x46, 0x5f);
    OVCFG_PARTY(0x240f1, OVS_1280, 0x6c, 0x389, 0x6e, 0x73);
    OVCFG_TARGET(0x24121, OVS_1280, 0xda, 0x389, 0x22, 0x73);
    OVCFG_HOTBUTTON(0x24181, OVS_1280, 0x253, 6, 0xec, 0x5c);
    OVCFG_SPELLS(0x24151, OVS_1280, 0x1b, 2, 0xd8, 0x28);
    OVCFG_CHAT(0x241b1, OVS_1280, 0x24a, 0xbe, 0x95, 0x27c);
    OVCFG_AFFECT(0x24271, OVS_1280, 0x223, 0x37c, 0xda, 0x80);
    OVFLAGS_7(0x240c1, 0x240f1, 0x24121, 0x24181, 0x24151, 0x241b1, 0x24271);

    OVCFG_PLAYER(0x240c9, OVS_1600, 8, 0x393, 0x46, 0x5f);
    OVCFG_PARTY(0x240f9, OVS_1600, 0x6c, 0x389, 0x6e, 0x73);
    OVCFG_TARGET(0x24129, OVS_1600, 0xda, 0x389, 0x22, 0x73);
    OVCFG_HOTBUTTON(0x24189, OVS_1600, 0x253, 6, 0xec, 0x5c);
    OVCFG_SPELLS(0x24159, OVS_1600, 0x1b, 2, 0xd8, 0x28);
    OVCFG_CHAT(0x241b9, OVS_1600, 0x24a, 0xbe, 0x95, 0x27c);
    OVCFG_AFFECT(0x24279, OVS_1600, 0x223, 0x37c, 0xda, 0x80);
    OVFLAGS_7(0x240c9, 0x240f9, 0x24129, 0x24189, 0x24159, 0x241b9, 0x24279);

    OVCFG_MAINMENU(0x243a9, OVS_640, 0x109, 0x204, 200, 0x78);
    OVCFG_MAINMENU(0x243b1, OVS_800, 0x181, 0x2a3, 200, 0x78);
    OVCFG_MAINMENU(0x243b9, OVS_1024, 0x226, 900, 200, 0x78);
    OVCFG_MAINMENU(0x243c1, OVS_1280, 0x32a, 0x483, 200, 0x78);
    OVCFG_MAINMENU(0x243c9, OVS_1600, 0x3d9, 0x5c3, 200, 0x78);
    OVFLAGS(0x243a9, 1);
    OVFLAGS(0x243b1, 1);
    OVFLAGS(0x243b9, 1);
    OVFLAGS(0x243c1, 1);
    OVFLAGS(0x243c9, 1);

    OVCFG_ITEMDATA(0x24379, OVS_640, 0xa5, 0x78, 0x96, 400);
    OVCFG_ITEMDATA(0x24381, OVS_800, 0xe1, 200, 0x96, 400);
    OVCFG_ITEMDATA(0x24389, OVS_1024, 0x135, 0x138, 0x96, 400);
    OVCFG_ITEMDATA(0x24391, OVS_1280, 0x1b5, 0x1b8, 0x96, 400);
    OVCFG_ITEMDATA(0x24399, OVS_1600, 0x20d, 600, 0x96, 400);
    OVFLAGS(0x24379, 0);
    OVFLAGS(0x24381, 0);
    OVFLAGS(0x24389, 0);
    OVFLAGS(0x24391, 0);
    OVFLAGS(0x24399, 0);

    OVCFG_INVENTORY(0x24289, OVS_640, 10, 0x46, 400, 0x154);
    OVCFG_INVENTORY(0x24291, OVS_800, 0x14, 0x96, 400, 0x154);
    OVCFG_INVENTORY(0x24299, OVS_1024, 0x1e, 0x106, 400, 0x154);
    OVCFG_INVENTORY(0x242a1, OVS_1280, 0x28, 0x186, 400, 0x154);
    OVCFG_INVENTORY(0x242a9, OVS_1600, 0x32, 0x226, 400, 0x154);
    OVFLAGS(0x24289, 0);
    OVFLAGS(0x24291, 0);
    OVFLAGS(0x24299, 0);
    OVFLAGS(0x242a1, 0);
    OVFLAGS(0x242a9, 0);

    OVCFG_TRACK(0x243d9, OVS_640, 0, 0x8c, 0x144, 0x78);
    OVCFG_TRACK(0x243e1, OVS_800, 10, 0x8c, 0x144, 0x78);
    OVCFG_TRACK(0x243e9, OVS_1024, 10, 0x8c, 0x144, 0x78);
    OVCFG_TRACK(0x243f1, OVS_1280, 10, 0x8c, 0x144, 0x78);
    OVCFG_TRACK(0x243f9, OVS_1600, 10, 0x8c, 0x144, 0x78);
    OVFLAGS(0x243d9, 0);
    OVFLAGS(0x243e1, 0);
    OVFLAGS(0x243e9, 0);
    OVFLAGS(0x243f1, 0);
    OVFLAGS(0x243f9, 0);

    OVCOLOR(0x24401, OVS_COLORS, EQ_ARRAY(char, 0x0055A6A4), EQ_ARRAY(char, 0x0055A694),
            EQ_ARRAY(char, 0x0055A684), EQ_ARRAY(char, 0x0055A674), 2);
    OVCOLOR(0x24402, OVS_COLORS, EQ_ARRAY(char, 0x0055A668), EQ_ARRAY(char, 0x0055A658),
            EQ_ARRAY(char, 0x0055A648), EQ_ARRAY(char, 0x0055A638), 2);
    OVCOLOR(0x24403, OVS_COLORS, EQ_ARRAY(char, 0x0055A628), EQ_ARRAY(char, 0x0055A618),
            EQ_ARRAY(char, 0x0055A608), EQ_ARRAY(char, 0x0055A5F8), 2);
    OVCOLOR(0x24405, OVS_COLORS, EQ_ARRAY(char, 0x0055A5E8), EQ_ARRAY(char, 0x0055A5D4),
            EQ_ARRAY(char, 0x0055A5C0), EQ_ARRAY(char, 0x0055A5AC), 2);
    OVCOLOR(0x24404, OVS_COLORS, EQ_ARRAY(char, 0x0055A59C), EQ_ARRAY(char, 0x0055A58C),
            EQ_ARRAY(char, 0x0055A57C), EQ_ARRAY(char, 0x0055A56C), 2);
    OVCOLOR(0x24406, OVS_COLORS, EQ_ARRAY(char, 0x0055A560), EQ_ARRAY(char, 0x0055A550),
            EQ_ARRAY(char, 0x0055A544), EQ_ARRAY(char, 0x0055A534), 2);
    OVCOLOR(0x2440a, OVS_COLORS, EQ_ARRAY(char, 0x0055A524), EQ_ARRAY(char, 0x0055A514),
            EQ_ARRAY(char, 0x0055A504), EQ_ARRAY(char, 0x0055A4F4), 2);
    OVCOLOR(0x24410, OVS_COLORS, EQ_ARRAY(char, 0x0055A4E4), EQ_ARRAY(char, 0x0055A4D0),
            EQ_ARRAY(char, 0x0055A4C0), EQ_ARRAY(char, 0x0055A4AC), 3);
    OVCOLOR(0x24411, OVS_COLORS, EQ_ARRAY(char, 0x0055A49C), EQ_ARRAY(char, 0x0055A488),
            EQ_ARRAY(char, 0x0055A478), EQ_ARRAY(char, 0x0055A464), 3);
    OVCOLOR(0x2440b, OVS_COLORS, EQ_ARRAY(char, 0x0055A454), EQ_ARRAY(char, 0x0055A440),
            EQ_ARRAY(char, 0x0055A42C), EQ_ARRAY(char, 0x0055A418), 2);
    OVCOLOR(0x2440c, OVS_COLORS, EQ_ARRAY(char, 0x0055A40C), EQ_ARRAY(char, 0x0055A3FC),
            EQ_ARRAY(char, 0x0055A3EC), EQ_ARRAY(char, 0x0055A3DC), 2);
}

// FUNCTION: TRILOGY 0x00456584
// Destructor for the vtable 0x00535564 class: resets the vtable, then runs teardown helper 0x00450463.
EQ_OverlaySlots_guess::~EQ_OverlaySlots_guess()
{
    DelButtons();
}

// FUNCTION: TRILOGY 0x004571eb
// Sends a trade message, then clears the active trade panes and item lists.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_cancelTrade_guess(EQ_Main* self)
{
    typedef void (__fastcall *tNoArgMethod_guess)(void*);
    int* count;
    int update_message[0x17];
    char* object;
    int* entry;
    int remainingFields;

    if (EQ_GLOBAL(char*, 0x006E4D88) != 0)
    {
        u32 trade_message[2];

        trade_message[0] = *(u32*)(EQ_GLOBAL(char*, 0x006E4D88) + 0x80);
        trade_message[1] = g_local_player_guess->spawn_id;
        send_message_guess(
            g_world_connection_guess, 0x20DB, trade_message, 8, 1);
    }

    object = EQ_GLOBAL(char*, 0x006018C0);
    if (object != 0 && *(int*)(object + 0x10) != 0)
        (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);

    object = EQ_GLOBAL(char*, 0x00604984);
    if (object != 0 && *(int*)(object + 0x10) != 0)
    {
        *(int*)(object + 0x1E8) = 1;
        object = EQ_GLOBAL(char*, 0x00604984);
        (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);
    }

    object = EQ_GLOBAL(char*, 0x006E4E5C);
    if (*(int*)(object + 0x10) != 0)
    {
        *(u8*)(*(u32*)(object + 0x174) + 0xD6) = 0;
        (*(tNoArgMethod_guess*)(*(u32*)EQ_GLOBAL(char*, 0x006E4E5C) + 8))(
            EQ_GLOBAL(char*, 0x006E4E5C));
        object = EQ_GLOBAL(char*, 0x006E4E5C);
        *(u32*)(object + 0x174) = 0;
    }

    object = EQ_GLOBAL(char*, 0x006E4E58);
    if (*(int*)(object + 0x10) != 0)
    {
        *(u8*)(*(u32*)(object + 0x174) + 0xD6) = 0;
        (*(tNoArgMethod_guess*)(*(u32*)EQ_GLOBAL(char*, 0x006E4E58) + 8))(
            EQ_GLOBAL(char*, 0x006E4E58));
        object = EQ_GLOBAL(char*, 0x006E4E58);
        *(u32*)(object + 0x174) = 0;
    }

    entry = (int*)self + 0x1B13B;
    {
        int remainingEntries = 8;

        do
        {
            if (entry[8] != 0)
            {
                operator delete((void*)entry[8]);
                entry[8] = 0;
            }
            if (*entry != 0)
            {
                operator delete((void*)*entry);
                *entry = 0;
            }
            entry = entry + 1;
            remainingEntries = remainingEntries - 1;
        } while (remainingEntries != 0);
    }

    entry = (int*)self + 0x1B14F;
    remainingFields = 4;
    do
    {
        entry[-4] = 0;
        *entry = 0;
        entry = entry + 1;
        remainingFields = remainingFields - 1;
    } while (remainingFields != 0);

    EQ_GLOBAL(char*, 0x006E4D88) = 0;
    ((int*)self)[0x1A179] = 0;
    ((int*)self)[0x1B13A] = 0;
    ((int*)self)[0x1A17B] = 0;
    ((int*)self)[0x1A17C] = 0;
    (*(tNoArgMethod_guess*)(*(u32*)self + 8))(self);
    self->screenMode_u8_guess = self->prevScreenMode_u8_guess;
    (*(tNoArgMethod_guess*)(*(u32*)self + 4))(self);

    object = g_eqmain_guess;
    *(int*)(object + 0xE5C) = *(int*)(object + 0xE5C) - 1;
    count = (int*)(g_eqmain_guess + 0xE5C);
    if (*count != 0)
    {
        update_message[0] = -4;
        update_message[1] = *count;
        update_message[2] = 3;
        send_message_guess(
            g_world_connection_guess, 0x2147, update_message, 0x5C, 1);
    }

    pinstEQ_Main_guess->pendingServerReplies_guess = 0;
    pinstEQ_Main_guess->tradeSessionActive_guess = 0;
    if (EQ_GLOBAL(int, 0x00590170) == 2)
    {
        g_game_state_guess = 2;
        if (g_eqmain_guess != 0)
            pinstEQ_Main_guess->uiDirty_guess = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_displayManager_guess)(0);
    }
}

// FUNCTION: TRILOGY 0x004573d2
// Closes both trade panes, clears exchange state, and saves the character.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_doTrade_guess(EQ_Main* self)
{
    typedef void (__fastcall *tNoArgMethod_guess)(void*);
    int update_message[0x17];
    char* object;
    int* entry;
    int loopCount;
    int* count;

    object = EQ_GLOBAL(char*, 0x006E4E58);
    (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);
    object = EQ_GLOBAL(char*, 0x006E4E58);
    *(u32*)(object + 0x174) = 0;

    object = EQ_GLOBAL(char*, 0x006E4E5C);
    (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);
    object = EQ_GLOBAL(char*, 0x006E4E5C);
    *(u32*)(object + 0x174) = 0;

    entry = (int*)self + 0x1B13B;
    loopCount = 8;
    do
    {
        if (entry[8] != 0)
        {
            operator delete((void*)entry[8]);
            entry[8] = 0;
        }
        if (*entry != 0)
        {
            operator delete((void*)*entry);
            *entry = 0;
        }
        entry = entry + 1;
        loopCount = loopCount - 1;
    } while (loopCount != 0);

    entry = (int*)self + 0x1B14F;
    loopCount = 4;
    do
    {
        entry[-4] = 0;
        *entry = 0;
        entry = entry + 1;
        loopCount = loopCount - 1;
    } while (loopCount != 0);

    EQ_GLOBAL(int, 0x006B7120) = 9;
    EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
    EQ_GLOBAL(char*, 0x006E4D88) = 0;
    ((int*)self)[0x1A179] = 0;
    ((int*)self)[0x1B13A] = 0;
    ((int*)self)[0x1A17B] = 0;
    ((int*)self)[0x1A17C] = 0;

    object = EQ_GLOBAL(char*, 0x006E4E58);
    if (object != 0 && *(int*)(object + 0x10) != 0 && *(int*)(object + 0x98) != 0)
        (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);

    object = EQ_GLOBAL(char*, 0x006E4E5C);
    if (object != 0 && *(int*)(object + 0x10) != 0 && *(int*)(object + 0x98) != 0)
        (*(tNoArgMethod_guess*)(*(u32*)object + 8))(object);

    (*(tNoArgMethod_guess*)(*(u32*)self + 8))(self);
    self->screenMode_u8_guess = self->prevScreenMode_u8_guess;
    (*(tNoArgMethod_guess*)(*(u32*)self + 4))(self);

    object = g_eqmain_guess;
    *(int*)(object + 0xE5C) = *(int*)(object + 0xE5C) - 1;
    count = (int*)(g_eqmain_guess + 0xE5C);
    if (*count != 0)
    {
        update_message[0] = -4;
        update_message[1] = *count;
        update_message[2] = 4;
        send_message_guess(
            g_world_connection_guess, 0x2147, update_message, 0x5C, 1);
    }

    object = g_eqmain_guess;
    *(int*)(object + 0xE5C) = 0;
    if (EQ_GLOBAL(int, 0x00590170) == 2)
    {
        g_game_state_guess = 2;
        object = g_eqmain_guess;
        if (object != 0)
            *(int*)(object + 0xEB8) = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_displayManager_guess)(0);
    }
}

// FUNCTION: TRILOGY 0x0045755e
// Calculates the displayed combat rating using the primary-hand skill or a default skill.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: g_pc temp in EAX (orig ECX); arg and to_hit registers swapped (orig arg ESI, to_hit EDI); one shared compute_tohit call (orig: one per path)
int __fastcall EQ_Main_CalcOffense_guess(void* self, int unusedEdx)
{
    EQ_Equipment* item;
    u8 skill;
    u32 skill_arg;
    s16 to_hit;
    s16 offense;

    item = (EQ_Equipment*)g_pc_guess->inventory_items[0xd];
    if (item != 0 && item->item.type == '\0' &&
        EQ_METHOD(0, int, 0x0043624B, (EQ_Equipment*), item)() != 0)
    {
        skill = EQ_METHOD(1, u8, 0x004B422A, (EQPlayer*, u8), g_local_player_guess)(item->item_type);
        skill_arg = ((u32)item & 0xffff0000) | (u32)skill;
        to_hit = EQ_METHOD(1, s16, 0x0041A0C5, (CharacterProfile*, u32), g_pc_guess)(skill_arg);
    }
    else
    {
        to_hit = EQ_METHOD(1, s16, 0x0041A0C5, (CharacterProfile*, u32), g_pc_guess)(0x1c);
        skill_arg = 0x1c;
    }

    offense = EQ_METHOD(1, s16, 0x0041A32C, (CharacterProfile*, u32), g_pc_guess)(skill_arg);
    return (((int)offense + (int)to_hit) * 1000) / 0x2e8;
}

// FUNCTION: TRILOGY 0x004575e4
// Combines two character values into one scaled rating for the character display.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
int __fastcall EQ_Main_CalcDefense_guess(void* self, int unusedEdx)
{
    s32 ac;
    s16 defense;

    ac = EQ_METHOD(1, s32, 0x00419D0F, (CharacterProfile*, int), g_pc_guess)(0);
    defense = EQ_METHOD(0, s16, 0x0041A26B, (CharacterProfile*), g_pc_guess)();
    return (((int)defense + (int)(s16)ac) * 1000) / 0x34f;
}

// FUNCTION: TRILOGY 0x00457617
// Updates a timed skill progress indicator, checks improvement at completion, and sends its completion packet.
void __fastcall drawMemVial(EQ_Main* self)
{
    int currentTick;
    char skillResult;
    short tickInterval;
    struct FUN_00457617_message_guess
    {
        int local_10;
        int local_c;
        u32 local_8;
    } local;

    if (self->memorizingSpell_guess != -1)
    {
        if (self->memorizeMeditate_guess == 0)
        {
            skillResult = EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)(0x1f);
            if (skillResult != 0)
            {
                self->memorizeMeditate_guess = 1;
                pinstEQ_Main_guess->uiDirty_guess = 1;
            }
        }
        tickInterval = (((u16)*(u8*)(EQ_ARRAY(char*, 0x005FE9A0)[self->memorizingSpell_guess] + 0x234
            + g_pc_guess->class_)
            - (u16)g_pc_guess->level) + 5) * 100;
        if (tickInterval < 100)
        {
            tickInterval = 100;
        }
        currentTick = *(int*)((char*)g_displayManager_guess + 0xbc);
        if ((u32)(currentTick - EQ_GLOBAL(int, 0x005921F8)) > (u32)(int)tickInterval)
        {
            EQ_GLOBAL(int, 0x005921F8) = currentTick;
            if (self->memorizeMeditate_guess != 0)
            {
                self->memorizeVialRemaining_guess = self->memorizeVialRemaining_guess
                    + (-2 - g_pc_guess->skills[31] / 10);
            }
            else
            {
                (self->memorizeVialRemaining_guess)--;
            }
        }
        if (self->memorizeVialRemaining_guess < 0)
        {
            self->memorizeVialRemaining_guess = 0;
        }
        if (self->memorizeVialRemaining_guess == 0)
        {
            if (self->memorizeMeditate_guess != 0)
            {
                EQ_METHOD(2, void, 0x004501A1, (void*, int, float), g_pc_guess)(0x1f, 1.0f);
            }
            (pinstEQ_Main_guess->pendingWorldRequests_guess)++;
            local.local_8 = 1;
            local.local_10 = self->memorizeGem_guess;
            local.local_c = self->memorizingSpell_guess;
            send_message_guess(
                g_world_connection_guess, 0x2182, &local, 0xc, 1);
            EQ_GLOBAL(int, 0x005921F8) = 0;
            self->memorizingSpell_guess = -1;
            self->memorizeGem_guess = -1;
            self->memorizeVialRemaining_guess = 0;
            self->unk_0x6C54E = 0;
        }
        else
        {
            g_setrect_guess(
                (RECT*)((char*)EQ_GLOBAL(char*, 0x006B3814) + 4), 0xc, 0x143,
                self->memorizeVialRemaining_guess + 0xc, 0x14d);
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(char*, 0x006B3814))(
                (char*)self + self->screenMode_guess * 0x1900 + 0x6e0dc, 1);
        }
    }
}

// FUNCTION: TRILOGY 0x004577c6
// Tracks skill-learning progress and notifies the server when the timer expires.
void __fastcall drawLearnVial(void* self)
{
    short tickInterval;
    struct FUN_004577c6_message_guess
    {
        int local_10;
        int local_c;
        u32 local_8;
    } local;

    if (*(short*)((char*)self + 0x1006) != -1)
    {
        tickInterval = (((u16)*(u8*)(EQ_ARRAY(char*, 0x005FE9A0)
            [*(short*)((char*)self + 0x1006)] + 0x234
            + g_pc_guess->class_)
            - (u16)g_pc_guess->level) + 5) * 100;
        if (tickInterval < 100)
            tickInterval = 100;
        if ((u32)(*(int*)(g_displayManager_guess + 0xbc)
            - EQ_GLOBAL(int, 0x005921FC)) > (u32)(int)tickInterval)
        {
            EQ_GLOBAL(int, 0x005921FC) = *(int*)(g_displayManager_guess + 0xbc);
            *(short*)((char*)self + 0x1000) = *(short*)((char*)self + 0x1000) - 1;
        }
        if (*(short*)((char*)self + 0x1000) == 0)
        {
            pinstEQ_Main_guess->pendingWorldRequests_guess = pinstEQ_Main_guess->pendingWorldRequests_guess + 1;
            local.local_8 = 0;
            local.local_10 = (int)*(short*)((char*)self + 0x6c550);
            local.local_c = (int)*(short*)((char*)self + 0x1006);
            send_message_guess(g_world_connection_guess, 0x2182, &local.local_10, 0xc, 1);
            *(u16*)((char*)self + 0x1000) = 0;
            *(short*)((char*)self + 0x1006) = -1;
            *(u16*)((char*)self + 0x6c54c) = 0;
            EQ_GLOBAL(int, 0x005921FC) = 0;
            *(short*)((char*)self + 0x6c550) = -1;
        }
        else
        {
            g_setrect_guess(
                (RECT*)(EQ_GLOBAL(char*, 0x006B3814) + 4),
                0xc,
                0x143,
                *(short*)((char*)self + 0x1000) + 0xc,
                0x14d);
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(char*, 0x006B3814))(
                (char*)self + *(char*)((char*)self + 0xfe0) * 0x1900 + 0x6e0ec,
                1);
        }
    }
    return;
}

#define MERC_TELL_guess(fmt) \
    sprintf(buf, fmt, \
        EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(\
            (char*)(EQ_GLOBAL(int, 0x006E4DB8) + 1))); \
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(buf, 0x111)

// FUNCTION: TRILOGY 0x004578F4
// Merchant window Buy/Sell button: checks lore, free space and worth, then buys or sells the selected item
// @early-stop: off/j stack slots swapped; g_pc+off loads use the other base register; IsActive store jumps into the material-branch sell copy
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// Merchant buy/sell button: the held item is this[0x6c658] (pointer to item pointer), its slot this[0x6c65c]
// (< 30 buys from the merchant, >= 100 sells slot - 100). Refusals are merchant "tells you" chat lines.
/* fake match: do/while(0)+break makes the IsActive store one shared join, and SellItem is called from both branches (VC6 merges the tails) */
void __fastcall EQ_Main_Btn_Merc_Buy_Sell_guess(EQ_Main* self)
{
    int found;
    int off;
    int j;
    int cnt;
    short count;
    short slot;
    int item;
    char buf[512];

    if (self->merchantOpen_guess == 0)
        return;
    if (self->merchantReady_guess == 0)
    {
        if (EQ_GLOBAL(int, 0x006E4DB8) == 0)
            return;
        MERC_TELL_guess(EQ_ARRAY(char, 0x0055ABB0));
    }
    else
    {
        if (*(int*)((char*)self + 0x6c658) == 0)
            return;
        if (*self->selectedItemSlotPtr_guess == 0)
            return;
        if (self->selectedItemSlot_guess < 0x1e)
        {
            found = 0;
            if (EQ_CDECL(int, 0x004A28AD, (void*, short, void*))(
                    EQ_GLOBAL(void*, 0x006E4DA8),
                    *(short*)(*self->selectedItemSlotPtr_guess + 0x82),
                    (void*)*self->selectedItemSlotPtr_guess) != 0)
            {
                MERC_TELL_guess(EQ_ARRAY(char, 0x0055AB1C));
            }
            else
            {
                off = 0x138;
                cnt = 8;
                do
                {
                    item = *(int*)(EQ_GLOBAL(char*, 0x006E4DA8) + off);
                    if (item == 0)
                    {
                        found = 1;
                    }
                    else if (*(char*)(item + 0x81) == 1)
                    {
                        for (j = 0; (short)j < (short)*(u8*)(item + 0xd5); j++)
                        {
                            if (*(int*)(item + 0xac + (short)j * 4) == 0 ||
                                (EQ_CDECL(int, 0x004A1D83, (int))(*(int*)(item + 0xac + (short)j * 4)) != 0 &&
                                 EQ_CDECL(int, 0x004A1D83, (int))(*self->selectedItemSlotPtr_guess) != 0 &&
                                 *(short*)(*(int*)(item + 0xac + (short)j * 4) + 0x82) ==
                                     *(short*)(*self->selectedItemSlotPtr_guess + 0x82)))
                            {
                                found = 1;
                            }
                        }
                    }
                    else
                    {
                        if (EQ_CDECL(int, 0x004A1D83, (int))(item) != 0 &&
                            EQ_CDECL(int, 0x004A1D83, (int))(*self->selectedItemSlotPtr_guess) != 0 &&
                            *(short*)(*(int*)(EQ_GLOBAL(char*, 0x006E4DA8) + off) + 0x82) ==
                                *(short*)(*self->selectedItemSlotPtr_guess + 0x82))
                        {
                            found = 1;
                        }
                    }
                    off = off + 4;
                    cnt = cnt - 1;
                } while (cnt != 0);
                if ((short)found == 0)
                {
                    MERC_TELL_guess(EQ_ARRAY(char, 0x0055AAD8));
                }
                else
                {
                    if (EQ_CDECL(int, 0x004A1D83, (int))(*self->selectedItemSlotPtr_guess) != 0)
                    {
                        EQ_METHOD(2, void, 0x00486CB6, (void*, void*, int), EQ_GLOBAL(void*, 0x006018C0))(
                            (void*)*(int*)((char*)self + 0x6c658),
                            (int)self->selectedItemSlot_guess);
                        return;
                    }
                    EQ_METHOD(1, void, 0x00463A0E, (void*, int), self)(1);
                    return;
                }
            }
        }
        else
        {
            if (EQ_CDECL(int, 0x004A1D83, (int))(*self->selectedItemSlotPtr_guess) != 0)
                count = (short)((int)(*(u8*)(*self->selectedItemSlotPtr_guess + 0xda) - 1) /
                    (int)*(char*)(*self->selectedItemSlotPtr_guess + 0xbb)) + 1;
            else
                count = 1;
            item = *self->selectedItemSlotPtr_guess;
            if (*(char*)(item + 0x81) == 0 && *(char*)(item + 0xc2) == 0x27)
            {
                MERC_TELL_guess(EQ_ARRAY(char, 0x0055AA98));
            }
            else
            {
                if (*(int*)(item + 0x8c) != 0 &&
                    EQ_METHOD(2, int, 0x004385A7, (void*, float, int), (void*)item)(
                        pinstEQ_Main_guess->merchantRate_guess, (int)count) != 0)
                {
                    if (EQ_CDECL(int, 0x004A1D83, (int))(*self->selectedItemSlotPtr_guess) != 0)
                    {
                        EQ_METHOD(2, void, 0x00486CB6, (void*, void*, int), EQ_GLOBAL(void*, 0x006018C0))(
                            (void*)*(int*)((char*)self + 0x6c658),
                            self->selectedItemSlot_guess - 100);
                        return;
                    }
                    slot = self->selectedItemSlot_guess;
                    if (slot < 100)
                        return;
                    item = *self->selectedItemSlotPtr_guess;
                    if (*(char*)(item + 0x81) == 1)
                    {
                        void* obj;
                        do
                        {
                            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0 &&
                                item == *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174))
                            {
                                if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) != 0)
                                    *(u8*)(*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) + 0xd6) = 0;
                                (*(void (__fastcall**)(void*))(**(int**)&EQ_GLOBAL(void*, 0x006E4E58) + 8))(
                                    EQ_GLOBAL(void*, 0x006E4E58));
                                obj = EQ_GLOBAL(void*, 0x006E4E58);
                            }
                            else if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0 &&
                                item == *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174))
                            {
                                if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) != 0)
                                    *(u8*)(*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) + 0xd6) = 0;
                                (*(void (__fastcall**)(void*))(**(int**)&EQ_GLOBAL(void*, 0x006E4E5C) + 8))(
                                    EQ_GLOBAL(void*, 0x006E4E5C));
                                obj = EQ_GLOBAL(void*, 0x006E4E5C);
                            }
                            else
                                break;
                            *(int*)((char*)obj + 0x174) = 0;
                        } while (0);
                        EQ_METHOD(2, void, 0x00463B5D, (void*, void*, short), self)((void*)*(int*)((char*)self + 0x6c658),
                            (short)(self->selectedItemSlot_guess - 100));
                        return;
                    }
                    else
                    {
                        if (slot - 100 < 0x16)
                        {
                            EQ_CDECL(void, 0x004A1E2A, (int, short, int, int))(
                                (int)EQ_GLOBAL(void*, 0x006E4DA8), slot - 100, 0,
                                *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + slot * 4 - 0xb0));
                        }
                        EQ_METHOD(2, void, 0x00463B5D, (void*, void*, short), self)((void*)*(int*)((char*)self + 0x6c658),
                            (short)(self->selectedItemSlot_guess - 100));
                        return;
                    }
                }
                MERC_TELL_guess(EQ_ARRAY(char, 0x0055AA54));
            }
        }
    }
}
#undef MERC_TELL_guess

/* File-local layouts for ProcessButtonFullView: EQ_Main's window records (0x20 bytes each from +0x90044), its hot buttons
 * (+0x6841E, ten per page), the track list (+0x685F8) and the messages the button handler sends. */
/* MainWindowState_guess and HotButtonSlot_guess are EQ_Main types now (trilogy/layout/eq_main.h). */

struct TrackSlot_guess
{
    EQPlayer* player;
    float distance;
    int value8;
    int valueC;
};

struct CoinMove_guess
{
    int fromSlot;
    int toSlot;
    int fromCoin;
    int toCoin;
    int amount;
};

struct ItemMove_guess
{
    u32 spawnId;
    int npcId;
    u8 detail[10];
    short itemNumber;
    short slot;
    u16 type;
    int cost;
};

struct HouseCoins_guess
{
    u32 spawnId;
    int npcId;
    int coin;
    int amount;
};

struct TradeAccept_guess
{
    u32 traderId;
    u8 accept;
};

struct TradeCancel_guess
{
    u32 traderId;
    u32 spawnId;
};

struct DestroyCoins_guess
{
    int fromSlot;
    int toSlot;
    int amount;
};

struct LootRequest_guess
{
    u32 corpseId;
    u32 spawnId;
    short slot;
    int rightClick;
};

struct CastRequest_guess
{
    int gem;
    int spell;
    int mode;
};

/* Vtable slots of EQ_Main and the windows it drives: +4 shows, +8 hides, +0x10 opens a bag window for a slot. */
class UiWindow_00457CD6_guess
{
public:
    virtual void Slot00();
    virtual void Activate();
    virtual void Deactivate();
    virtual void Slot0C();
    virtual void Open(int slot);
};

#define MAIN_INT(off)           (*(int*)(self + (off)))
#define MAIN_SHORT(off)         (*(short*)(self + (off)))
#define MAIN_BYTE(off)          (*(char*)(self + (off)))
#define MAIN_WINDOW(index)      (((MainWindowState_guess*)(self + 0x90044))[index])
#define MAIN_MODE               MAIN_BYTE(0xfe0)
#define LIST_PAGE               MAIN_BYTE(0x6c592)
#define HOTBUTTON_PAGE          MAIN_BYTE(0x6841d)
#define SPELLSET_PAGE           MAIN_BYTE(0x6841c)
#define HOTBUTTON(id)           (((HotButtonSlot_guess*)(self + 0x6841e))[HOTBUTTON_PAGE * 10 + (id) - 200])
#define LOOT_OPEN               MAIN_INT(0x685dc)
#define MERCHANT_OPEN           MAIN_INT(0x685e0)
#define TRADE_OPEN              MAIN_INT(0x685e4)
#define BANK_OPEN               MAIN_INT(0x685e8)
#define TRADE_ACCEPTED          MAIN_INT(0x685ec)
#define TRADE_OTHER_ACCEPTED    MAIN_INT(0x685f0)
#define HOUSE_BANK              MAIN_BYTE(0x101)
#define HOUSE_ITEMS             ((EQ_Item**)(self + 0x108))
#define HOUSE_COINS             ((s32*)(self + 0x128))
#define LIST_ITEMS              ((EQ_Item**)(self + 0x6c5e0))
#define TRADE_THEIR_ITEMS       ((EQ_Item**)(self + 0x6c4ec))
#define TRADE_MY_ITEMS          ((EQ_Item**)(self + 0x6c50c))
#define TRADE_COINS             ((int*)(self + 0x6c52c))
#define ABILITY_SLOTS           ((short*)(self + 0xff4))
#define COMBAT_SLOTS            ((short*)(self + 0xfec))
#define SKILL_LIST              ((short*)(self + 0x6c65e))
#define GEM_STATE               ((int*)(self + 0xe60))
#define TRACK_SLOTS             ((TrackSlot_guess*)(self + 0x685f8))
#define SPELL_SET(index)        (self + 0x5f196 + ((index) % 12 + (index) / 12 * 12) * 0x136)
#define UI_WINDOW(window)       ((UiWindow_00457CD6_guess*)(window))
#define WINDOW_INT(window, off) (*(int*)((window) + (off)))
#define WINDOW_ACTIVE(window)   WINDOW_INT(window, 0x10)
#define WINDOW_ITEM(window)     (*(EQ_Item**)((window) + 0x174))
#define WINDOW_SLOT(window)     (*(short*)((window) + 0x92))
#define BOOK_ITEM               (*(EQ_Item**)(g_bookWindow + 0x37ac))
#define BAG_CONTENTS(item)      ((EQ_Item**)((char*)(item) + 0xac))
#define BOOK_FILE(item)         ((char*)(item) + 0xaf)
#define ITEM_CLASS(item)        (*((u8*)(item) + 0xc2))
#define BAG_SLOT_COUNT(item)    (*((u8*)(item) + 0xd5))
#define BAG_OPEN(item)          (*((char*)(item) + 0xd6))
#define ITEM_DETAIL(item)       ((char*)(item) + 0xd8)
#define CURSOR_HAS_ITEM         (*(u8*)(g_displayManager_guess + 0x3c))
#define CURSOR_COIN             (g_cdisplay_guess->moneyOnCursor_guess)
#define HOTBUTTON_HELD          (*(u8*)(g_displayManager_guess + 0x3e))
#define DISPLAY_TIME            (*(int*)(g_displayManager_guess + 0xbc))
#define NPC_ID                  (*(short*)(EQ_GLOBAL(char*, 0x006E4DCC) + 0x62))
#define g_screenWidth           EQ_GLOBAL(int, 0x006B63FC)
#define g_bagWindow1            EQ_GLOBAL(char*, 0x006E4E58)
#define g_bagWindow2            EQ_GLOBAL(char*, 0x006E4E5C)
#define g_bookWindow            EQ_GLOBAL(char*, 0x006E4E60)
#define g_messageBox            EQ_GLOBAL(char*, 0x006E4E20)
#define g_itemDisplay           EQ_GLOBAL(char*, 0x006E4D84)
#define g_moneySlider           EQ_GLOBAL(char*, 0x00604984)
#define g_quantitySlider        EQ_GLOBAL(char*, 0x006018C0)
#define g_trader                EQ_GLOBAL(EQPlayer*, 0x006E4D88)
#define g_merchant              EQ_GLOBAL(EQPlayer*, 0x006E4DB8)
#define g_corpse                EQ_GLOBAL(EQPlayer*, 0x006E4DC0)
#define g_target                EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define g_soundManager          EQ_GLOBAL(void*, 0x006E4DD8)
#define g_trackMode             EQ_GLOBAL(short, 0x006B6772)
#define g_trackPlayer           EQ_GLOBAL(EQPlayer*, 0x006B6780)
#define g_lootNoDropMode        EQ_GLOBAL(int, 0x006B6F04)
#define g_groupNames            EQ_ARRAY(char, 0x005FE8BD)
#define g_groupMembers          EQ_ARRAY(EQPlayer*, 0x005FE954)
#define g_emptyString           EQ_ARRAY(char, 0x0058FBA4)
#define COMBINE                 EQ_CDECL(int, 0x004A1D83, (EQ_Item*))
#define CAN_DROP                EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))
#define SWAP_COMBINE            EQ_CDECL(int, 0x004A1C7C, (EQ_Item**, EQ_Item**, int, int))
#define AUTO_INVENTORY          EQ_CDECL(void, 0x004A2351, (CharacterProfile*, EQ_Item**, int))
#define HANDLE_MATERIAL         EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, short, EQ_Item*, EQ_Item*))
#define UPDATE_LIGHT            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))
#define CAN_EQUIP_IN_SLOT       EQ_CDECL(int, 0x004A2180, (CharacterProfile*, u32, short, EQ_Item*))
#define EXECUTE_CMD             EQ_CDECL(void, 0x004D7D6A, (int, int, int))
#define SEND_MESSAGE(opcode, data, size) send_message_guess(g_world_connection_guess, (opcode), (data), (size), 1)
#define IS_ACTIVE(window)       EQ_METHOD(0, int, 0x004504AC, (char*), (window))()
#define WRITE_WINDOW_VALUE      EQ_METHOD(3, void, 0x0045056D, (char*, const char*, const char*, int), self)
#define SAVE_CUSTOM             EQ_METHOD(0, void, 0x004567F8, (char*), self)
#define RIGHT_CLICK_ITEM        EQ_METHOD(3, void, 0x0046AB71, (char*, EQ_Item*, EQ_Item*, short), self)
#define DSP_CHAT                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)
#define TRIM_NAME               EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)
#define STRIP_NAME              EQ_METHOD(1, char*, 0x004D0436, (void*, char*), g_everquest_guess)
#define MOVE_ITEM               EQ_METHOD(4, void, 0x00463651, (char*, int, int, int, int), g_eqmain_guess)
#define MONEY_SLIDER            EQ_METHOD(3, void, 0x0048629A, (char*, s32*, int, int), g_moneySlider)
#define QUANTITY_SLIDER         EQ_METHOD(2, void, 0x00486CB6, (char*, EQ_Item**, int), g_quantitySlider)
#define WAVE_PLAY               EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_soundManager)
#define CHANGE_VIDEO_MODE       EQ_METHOD(1, void, 0x0040E58B, (void*, int), EQ_GLOBAL(void*, 0x006E4DE0))
#define UPDATE_EQUIPMENT        EQ_METHOD(2, void, 0x0040B521, (void*, EQPlayer*, short), EQ_GLOBAL(void*, 0x006E4DE0))
#define USE_ABILITY             EQ_METHOD(2, void, 0x0041D790, (CharacterProfile*, char, EQPlayer*), g_pc_guess)
#define CAN_USE_ITEM            EQ_METHOD(1, int, 0x0041BA72, (CharacterProfile*, EQ_Item*), g_pc_guess)
#define CAST_SPELL              EQ_METHOD(4, char, 0x0041F381, (CharacterProfile*, u8, short, void*, int), g_pc_guess)
#define STOP_SPELL_CAST         EQ_METHOD(1, void, 0x0042798F, (CharacterProfile*, int), g_pc_guess)
#define SELL_PRICE(item)        EQ_METHOD(2, char*, 0x004385D5, (EQ_Item*, float, int), (item))(pinstEQ_Main_guess->merchantRate_guess, 1)
#define BUY_PRICE(item)         EQ_METHOD(2, char*, 0x00438701, (EQ_Item*, float, int), (item))(pinstEQ_Main_guess->merchantRate_guess, 1)
#define RENT_PRICE(item)        EQ_METHOD(0, char*, 0x0043882D, (EQ_Item*), (item))()
#define CLOSE_BAG(item)         EQ_METHOD(0, char, 0x0043407E, (EQ_Item*), (item))()

// FUNCTION: TRILOGY 0x00457CD6
// Performs an old-UI button press: saves window positions, moves cursor coins/items, casts gems, picks targets
// EQ_Main's button handler (mouseButton 0 = left, 1 = right): window toggles, inventory, coins, bank, trade,
// merchant, loot, track and hot buttons. Called by EQ_Main::ProcessButton (0x0045CB52).
void __fastcall ProcessButtonFullView(char* self, int unusedEdx, short mouseButton, short buttonId)
{
    char text[128];
    char itemName[32];
    char section[16];
    CoinMove_guess move;
    ItemMove_guess itemMove;
    HouseCoins_guess houseCoins;
    CastRequest_guess cast;
    LootRequest_guess loot;
    TradeAccept_guess accept;
    TradeCancel_guess cancel;
    DestroyCoins_guess destroy;
    const char* sectionName;
    char* bookFile;
    char* merchantName;
    char* price;
    char* rent;
    EQ_Item* item;
    EQ_Item* held;
    EQ_Item** slotPtr;
    EQ_Item** contents;
    TrackSlot_guess* track;
    int* gemState;
    EQPlayer* member;
    u32 now;
    int slot;
    int count;
    int canLoot;
    short index;
    short freeSlot;
    short bagSlot;
    short spell;
    short heldValue;
    short oldValue;
    char oldType;
    char hotButtonType;
    char setIndex;

    if (buttonId >= 0x168 && buttonId <= 0x171 && mouseButton == 0)
    {
        if (MAIN_INT(0x6c8f8) == 0)
            goto inventoryWindow;
        if (g_screenWidth == 800)
            sectionName = OVS_800;
        else if (g_screenWidth == 0x400)
            sectionName = OVS_1024;
        else if (g_screenWidth == 0x500)
            sectionName = OVS_1280;
        else if (g_screenWidth == 0x640)
            sectionName = OVS_1600;
        else
            sectionName = OVS_640;
        strcpy(section, sectionName);
        if (EQ_GLOBAL(int, 0x006E2094) != 0)
        {
            MAIN_WINDOW(0).active = 1;
            MAIN_WINDOW(0).moving = 0;
            MAIN_WINDOW(1).active = 1;
            MAIN_WINDOW(1).moving = 0;
            MAIN_WINDOW(2).active = 1;
            MAIN_WINDOW(2).moving = 0;
            MAIN_WINDOW(3).active = 1;
            MAIN_WINDOW(3).moving = 0;
            MAIN_WINDOW(4).active = 1;
            MAIN_WINDOW(4).moving = 0;
            MAIN_WINDOW(9).active = 1;
            MAIN_WINDOW(9).moving = 0;
            MAIN_WINDOW(9).active = 1;
            MAIN_WINDOW(9).sizing = 0;
            MAIN_WINDOW(16).active = 1;
            MAIN_WINDOW(16).moving = 0;
            MAIN_WINDOW(10).active = 1;
            MAIN_WINDOW(10).moving = 0;
            MAIN_WINDOW(17).active = 1;
            MAIN_WINDOW(17).moving = 0;
        }
        else
        {
            switch (buttonId)
            {
            case 0x168:
                MAIN_WINDOW(0).active = 0;
                if (MAIN_WINDOW(0).visible != 0)
                {
                    MAIN_WINDOW(0).active = 0;
                    MAIN_WINDOW(0).active = 1;
                    if (MAIN_WINDOW(0).moving != 0)
                    {
                        MAIN_WINDOW(0).moving = 0;
                        MAIN_WINDOW(0).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A92C), MAIN_WINDOW(0).x);
                        MAIN_WINDOW(0).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A938), MAIN_WINDOW(0).y);
                    }
                    else
                        MAIN_WINDOW(0).moving = 1;
                }
                break;
            case 0x169:
                MAIN_WINDOW(1).active = 0;
                if (MAIN_WINDOW(1).visible != 0)
                {
                    MAIN_WINDOW(1).active = 0;
                    MAIN_WINDOW(1).active = 1;
                    if (MAIN_WINDOW(1).moving != 0)
                    {
                        MAIN_WINDOW(1).moving = 0;
                        MAIN_WINDOW(1).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A8FC), MAIN_WINDOW(1).x);
                        MAIN_WINDOW(1).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A908), MAIN_WINDOW(1).y);
                    }
                    else
                        MAIN_WINDOW(1).moving = 1;
                }
                break;
            case 0x16a:
                MAIN_WINDOW(2).active = 0;
                if (MAIN_WINDOW(2).visible != 0)
                {
                    MAIN_WINDOW(2).active = 0;
                    MAIN_WINDOW(2).active = 1;
                    if (MAIN_WINDOW(2).moving != 0)
                    {
                        MAIN_WINDOW(2).moving = 0;
                        MAIN_WINDOW(2).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A8CC), MAIN_WINDOW(2).x);
                        MAIN_WINDOW(2).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A8D8), MAIN_WINDOW(2).y);
                    }
                    else
                        MAIN_WINDOW(2).moving = 1;
                }
                break;
            case 0x16b:
                MAIN_WINDOW(3).active = 0;
                if (MAIN_WINDOW(3).visible != 0)
                {
                    MAIN_WINDOW(3).active = 0;
                    MAIN_WINDOW(3).active = 1;
                    if (MAIN_WINDOW(3).moving != 0)
                    {
                        MAIN_WINDOW(3).moving = 0;
                        MAIN_WINDOW(3).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A85C), MAIN_WINDOW(3).x);
                        MAIN_WINDOW(3).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A868), MAIN_WINDOW(3).y);
                    }
                    else
                        MAIN_WINDOW(3).moving = 1;
                }
                break;
            case 0x16c:
                MAIN_WINDOW(4).active = 0;
                if (MAIN_WINDOW(4).visible != 0)
                {
                    MAIN_WINDOW(4).active = 0;
                    MAIN_WINDOW(4).active = 1;
                    if (MAIN_WINDOW(4).moving != 0)
                    {
                        MAIN_WINDOW(4).moving = 0;
                        MAIN_WINDOW(4).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A894), MAIN_WINDOW(4).x);
                        MAIN_WINDOW(4).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A8A4), MAIN_WINDOW(4).y);
                    }
                    else
                        MAIN_WINDOW(4).moving = 1;
                }
                break;
            case 0x16d:
                MAIN_WINDOW(9).active = 0;
                if (MAIN_WINDOW(9).visible != 0)
                {
                    MAIN_WINDOW(9).active = 0;
                    MAIN_WINDOW(9).active = 1;
                    if (MAIN_WINDOW(9).moving != 0)
                    {
                        MAIN_WINDOW(9).moving = 0;
                        MAIN_WINDOW(9).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A7FC), MAIN_WINDOW(9).x);
                        MAIN_WINDOW(9).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A808), MAIN_WINDOW(9).y);
                    }
                    else
                        MAIN_WINDOW(9).moving = 1;
                }
                break;
            case 0x16e:
                MAIN_WINDOW(9).active = 0;
                if (MAIN_WINDOW(9).visible != 0)
                {
                    MAIN_WINDOW(9).active = 0;
                    MAIN_WINDOW(9).active = 1;
                    if (MAIN_WINDOW(9).sizing != 0)
                    {
                        MAIN_WINDOW(9).sizing = 0;
                        MAIN_WINDOW(9).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A7E4), MAIN_WINDOW(9).width);
                        MAIN_WINDOW(9).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A7F0), MAIN_WINDOW(9).height);
                    }
                    else
                        MAIN_WINDOW(9).sizing = 1;
                }
                break;
            case 0x16f:
                MAIN_WINDOW(16).active = 0;
                if (MAIN_WINDOW(16).visible != 0)
                {
                    MAIN_WINDOW(16).active = 0;
                    MAIN_WINDOW(16).active = 1;
                    if (MAIN_WINDOW(16).moving != 0)
                    {
                        MAIN_WINDOW(16).moving = 0;
                        MAIN_WINDOW(16).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A794), MAIN_WINDOW(16).x);
                        MAIN_WINDOW(16).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A7A4), MAIN_WINDOW(16).y);
                    }
                    else
                        MAIN_WINDOW(16).moving = 1;
                }
                break;
            case 0x170:
                MAIN_WINDOW(10).active = 0;
                if (MAIN_WINDOW(10).visible != 0)
                {
                    MAIN_WINDOW(10).active = 0;
                    MAIN_WINDOW(10).active = 1;
                    if (MAIN_WINDOW(10).moving != 0)
                    {
                        MAIN_WINDOW(10).moving = 0;
                        MAIN_WINDOW(10).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A714), MAIN_WINDOW(10).x);
                        MAIN_WINDOW(10).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A724), MAIN_WINDOW(10).y);
                    }
                    else
                        MAIN_WINDOW(10).moving = 1;
                }
                break;
            case 0x171:
                MAIN_WINDOW(17).active = 0;
                if (MAIN_WINDOW(17).visible != 0)
                {
                    MAIN_WINDOW(17).active = 0;
                    MAIN_WINDOW(17).active = 1;
                    if (MAIN_WINDOW(17).moving != 0)
                    {
                        MAIN_WINDOW(17).moving = 0;
                        MAIN_WINDOW(17).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A6DC), MAIN_WINDOW(17).x);
                        MAIN_WINDOW(17).active = 0;
                        WRITE_WINDOW_VALUE(section, EQ_ARRAY(char, 0x0055A6E8), MAIN_WINDOW(17).y);
                    }
                    else
                        MAIN_WINDOW(17).moving = 1;
                }
                break;
            }
        }
    }

    if (MAIN_INT(0x6c8f8) != 0 && EQ_GLOBAL(int, 0x006B649C) == 0)
    {
        if (mouseButton == 0)
        {
            MAIN_SHORT(0x5f188) = buttonId;
            MAIN_SHORT(0x5f18a) = -1;
            MAIN_WINDOW(10).active = 0;
            if (MAIN_WINDOW(10).visible != 0 && buttonId == 0x2a && MERCHANT_OPEN == 0 && LOOT_OPEN == 0 &&
                BANK_OPEN == 0)
            {
                if (MAIN_INT(0x8fb90) != 0)
                    MAIN_INT(0x5f174) = 1;
                return;
            }
            MAIN_WINDOW(17).active = 0;
            if (MAIN_WINDOW(17).visible != 0 && buttonId == 0x6c)
                MAIN_INT(0x5f180) = 1;
            return;
        }
        if (mouseButton != 1)
            return;
        if (EQ_GLOBAL(int, 0x006B6498) == 0)
            return;
        MAIN_WINDOW(10).active = 0;
        if (MAIN_WINDOW(10).visible != 0)
        {
            if (LOOT_OPEN != 0 && buttonId >= 0x113 && buttonId <= 0x122)
            {
                item = LIST_ITEMS[buttonId - 0x113 + LIST_PAGE * 2];
                goto inspectItem;
            }
            if (MERCHANT_OPEN != 0 && buttonId >= 0x104 && buttonId <= 0x10f)
            {
                item = LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2];
                goto inspectItem;
            }
            if (BANK_OPEN != 0 && buttonId >= 0xaa && buttonId <= 0xb1)
            {
                if (HOUSE_BANK != 0)
                {
                    item = HOUSE_ITEMS[buttonId - 0xaa];
                    goto inspectItem;
                }
                item = g_pc_guess->bank_items[buttonId - 0xaa];
                goto inspectItem;
            }
            if (buttonId >= 1 && buttonId <= 0x1d)
            {
                if (MAIN_BYTE(0x100c) != 0)
                {
                    if (buttonId >= 0x16)
                        return;
                    *(short*)(g_itemDisplay + 0x96) = buttonId;
                    goto showItemDisplay;
                }
                item = g_pc_guess->inventory_items[buttonId];
                goto inspectItem;
            }
            if (TRADE_OPEN != 0 && g_trader != 0)
            {
                if (buttonId >= 300 && buttonId <= 0x133)
                {
                    item = TRADE_THEIR_ITEMS[buttonId - 300];
                    goto inspectItem;
                }
                if (buttonId >= 0x136 && buttonId <= 0x13d)
                {
                    item = TRADE_MY_ITEMS[buttonId - 0x136];
                    goto inspectItem;
                }
            }
        }
        MAIN_WINDOW(3).active = 0;
        if (MAIN_WINDOW(3).visible != 0 && buttonId >= 0xd4 && buttonId <= 0xdb)
        {
            spell = g_pc_guess->spell_memorized[buttonId - 0xd4];
            goto inspectSpell;
        }
        MAIN_WINDOW(4).active = 0;
        if (MAIN_WINDOW(4).visible == 0)
            return;
        if (buttonId < 200)
            return;
        if (buttonId > 0xd1)
            return;
        if (HOTBUTTON(buttonId).type == 5)
        {
            item = g_pc_guess->inventory_items[HOTBUTTON(buttonId).value];
            goto inspectItem;
        }
        if (HOTBUTTON(buttonId).type != 7)
            return;
        spell = g_pc_guess->spell_memorized[HOTBUTTON(buttonId).value];
    inspectSpell:
        if (spell == -1)
            return;
        *(short*)(g_itemDisplay + 0x94) = spell;
        goto showItemDisplay;
    inspectItem:
        if (item == 0)
            return;
        *(EQ_Item**)(g_itemDisplay + 0x90) = item;
    showItemDisplay:
        UI_WINDOW(g_itemDisplay)->Activate();
        return;
    }

inventoryWindow:
    MAIN_WINDOW(10).active = 0;
    if (MAIN_WINDOW(10).visible == 0)
        goto trackWindow;
    if (MAIN_INT(0x94) != 0)
        return;
    if (TRADE_OPEN != 0 && TRADE_ACCEPTED != 0 && TRADE_OTHER_ACCEPTED != 0)
        return;
    if (TRADE_OPEN != 0 && TRADE_ACCEPTED != 0 && buttonId != 0x144)
    {
        DSP_CHAT(EQ_ARRAY(char, 0x0055AE0C), 0xf);
        return;
    }
    if (TRADE_OPEN != 0 && MAIN_INT(0x6c908) == 0)
        return;
    if (MERCHANT_OPEN != 0 && MAIN_INT(0xa0) == 0)
        return;
    if (LOOT_OPEN != 0 && MAIN_INT(0xa4) == 0)
        return;
    if (EQ_GLOBAL(int, 0x006B649C) == 0)
    {
        if (mouseButton != 0 || (buttonId != 0x27 && buttonId != 0x38))
        {
            if (mouseButton != 1)
                goto inventoryLeftClick;
            if (buttonId != 0x20)
                goto inventoryRightClick;
        }
        if (WINDOW_ACTIVE(g_bookWindow) != 0 && MAIN_INT(0x8fba4) == 0)
            return;
        MAIN_INT(0x8fba4) ^= 1;
        if (MAIN_INT(0x8fba4) != 0)
        {
            MAIN_INT(0x8fb94) = 1;
            MAIN_INT(0x8fb9c) = 1;
            MAIN_INT(0x8fba0) = 1;
            MAIN_INT(0x8fb98) = 1;
        }
        else
        {
            MAIN_INT(0x8fb94) = 0;
            MAIN_INT(0x8fb9c) = 0;
            MAIN_INT(0x8fba0) = 0;
            MAIN_INT(0x8fb98) = 0;
        }
        UI_WINDOW(self)->Deactivate();
        MAIN_INT(0x8fba8) = 1;
        MAIN_MODE = 4;
        goto activateAndReturn;
    }
    if (mouseButton != 1)
        goto inventoryLeftClick;

inventoryRightClick:
    if (EQ_GLOBAL(int, 0x006B6498) != 0)
        goto tradeWindow;
    if (BANK_OPEN != 0 && buttonId >= 0xaa && buttonId <= 0xb1)
    {
        if (HOUSE_BANK != 0)
        {
            item = HOUSE_ITEMS[buttonId - 0xaa];
            if (item == 0)
                return;
            if (item->type != 1)
                return;
            if (BAG_OPEN(item) == 1)
            {
                if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                {
                    BAG_OPEN(item) = 0;
                    UI_WINDOW(g_bagWindow1)->Deactivate();
                    pinstEQ_Bag1_guess->item_guess = 0;
                    return;
                }
                if (IS_ACTIVE(g_bagWindow2) == 0)
                    return;
                if (pinstEQ_Bag2_guess->item_guess != item)
                    return;
                BAG_OPEN(item) = 0;
                UI_WINDOW(g_bagWindow2)->Deactivate();
                pinstEQ_Bag2_guess->item_guess = 0;
                return;
            }
            if (IS_ACTIVE(g_bagWindow1) == 0)
            {
                BAG_OPEN(item) = 1;
                pinstEQ_Bag1_guess->item_guess = item;
                UI_WINDOW(g_bagWindow1)->Open(buttonId - 0xa2);
                pinstEQ_Bag1_guess->slot_guess = buttonId - 0xaa;
                pinstEQ_Bag1_guess->bankContainer_guess = 1;
                return;
            }
            if (IS_ACTIVE(g_bagWindow2) != 0)
                return;
            BAG_OPEN(item) = 1;
            pinstEQ_Bag2_guess->item_guess = item;
            UI_WINDOW(g_bagWindow2)->Open(buttonId - 0xa2);
            pinstEQ_Bag2_guess->slot_guess = buttonId - 0xaa;
            pinstEQ_Bag2_guess->bankContainer_guess = 1;
            return;
        }
        item = g_pc_guess->bank_items[buttonId - 0xaa];
        if (item == 0)
            return;
        if (item->type != 1)
            return;
        if (BAG_OPEN(item) == 1)
        {
            if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
            {
                BAG_OPEN(item) = 0;
                UI_WINDOW(g_bagWindow1)->Deactivate();
                pinstEQ_Bag1_guess->item_guess = 0;
                return;
            }
            if (IS_ACTIVE(g_bagWindow2) == 0)
                return;
            if (pinstEQ_Bag2_guess->item_guess != item)
                return;
            BAG_OPEN(item) = 0;
            UI_WINDOW(g_bagWindow2)->Deactivate();
            pinstEQ_Bag2_guess->item_guess = 0;
            return;
        }
        if (IS_ACTIVE(g_bagWindow1) == 0)
        {
            BAG_OPEN(item) = 1;
            pinstEQ_Bag1_guess->item_guess = item;
            UI_WINDOW(g_bagWindow1)->Open(buttonId - 0xa2);
            pinstEQ_Bag1_guess->slot_guess = buttonId - 0xaa;
            pinstEQ_Bag1_guess->bankContainer_guess = 1;
            return;
        }
        if (IS_ACTIVE(g_bagWindow2) != 0)
            return;
        BAG_OPEN(item) = 1;
        pinstEQ_Bag2_guess->item_guess = item;
        UI_WINDOW(g_bagWindow2)->Open(buttonId - 0xa2);
        pinstEQ_Bag2_guess->slot_guess = buttonId - 0xaa;
        pinstEQ_Bag2_guess->bankContainer_guess = 1;
        return;
    }
    if (buttonId >= 0x16 && buttonId <= 0x1d && MAIN_BYTE(0x100c) == 0)
    {
        slot = buttonId;
        item = g_pc_guess->inventory_items[slot];
        if (item != 0 && item->type == 1)
        {
            if (BAG_OPEN(item) == 1)
            {
                if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                {
                    BAG_OPEN(item) = 0;
                    UI_WINDOW(g_bagWindow1)->Deactivate();
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (IS_ACTIVE(g_bagWindow2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                {
                    BAG_OPEN(item) = 0;
                    UI_WINDOW(g_bagWindow2)->Deactivate();
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            else if (IS_ACTIVE(g_bagWindow1) == 0)
            {
                BAG_OPEN(item) = 1;
                pinstEQ_Bag1_guess->item_guess = item;
                UI_WINDOW(g_bagWindow1)->Open(slot - 0x16);
                pinstEQ_Bag1_guess->slot_guess = buttonId;
            }
            else if (IS_ACTIVE(g_bagWindow2) == 0)
            {
                BAG_OPEN(item) = 1;
                pinstEQ_Bag2_guess->item_guess = item;
                UI_WINDOW(g_bagWindow2)->Open(slot - 0x16);
                pinstEQ_Bag2_guess->slot_guess = buttonId;
            }
        }
        item = g_pc_guess->inventory_items[slot];
        if (item != 0 && item->type == 2)
        {
            if (WINDOW_ACTIVE(g_bookWindow) == 0)
            {
                BOOK_ITEM = item;
                bookFile = BOOK_FILE(BOOK_ITEM);
                SEND_MESSAGE(0x20ce, bookFile, strlen(bookFile) + 1);
                goto inventoryLeftClick;
            }
            if (IS_ACTIVE(g_bookWindow) != 0 && BOOK_ITEM == item)
                UI_WINDOW(g_bookWindow)->Deactivate();
        }
    }
    goto tradeWindow;

inventoryLeftClick:
    if (mouseButton != 0)
        goto tradeWindow;
    if (buttonId == 0x20)
    {
        if (CURSOR_COIN == 0)
        {
            if (g_pc_guess->inventory_items[0] != 0 && g_soundManager != 0)
                WAVE_PLAY(0x92, 0);
            AUTO_INVENTORY(g_pc_guess, g_pc_guess->inventory_items, 0);
            if (g_pc_guess->inventory_items[0] == 0)
                return;
        }
        else
        {
            move.fromSlot = 0;
            move.toSlot = 1;
            if (g_pc_guess->platinum_cursor > 0)
            {
                move.fromCoin = 3;
                move.amount = g_pc_guess->platinum_cursor;
                g_pc_guess->platinum += g_pc_guess->platinum_cursor;
            }
            else if (g_pc_guess->gold_cursor > 0)
            {
                move.fromCoin = 2;
                move.amount = g_pc_guess->gold_cursor;
                g_pc_guess->gold += g_pc_guess->gold_cursor;
            }
            else if (g_pc_guess->silver_cursor > 0)
            {
                move.fromCoin = 1;
                move.amount = g_pc_guess->silver_cursor;
                g_pc_guess->silver += g_pc_guess->silver_cursor;
            }
            else if (g_pc_guess->copper_cursor > 0)
            {
                move.fromCoin = 0;
                move.amount = g_pc_guess->copper_cursor;
                g_pc_guess->copper += g_pc_guess->copper_cursor;
            }
            move.toCoin = move.fromCoin;
            if (move.amount > 0)
            {
                SEND_MESSAGE(0x212d, &move, 0x14);
                g_pc_guess->platinum_cursor = 0;
                g_pc_guess->gold_cursor = 0;
                g_pc_guess->silver_cursor = 0;
                g_pc_guess->copper_cursor = 0;
                CURSOR_COIN = 0;
            }
        }
    }

    switch (buttonId)
    {
    case 0x1e:
        UI_WINDOW(self)->Deactivate();
        MAIN_MODE = 4;
        if (MAIN_INT(0x8fb90) == 0)
            MAIN_INT(0x8fb90) = 1;
        UI_WINDOW(self)->Activate();
        break;
    case 0x1f:
        UI_WINDOW(self)->Deactivate();
        MAIN_MODE = 4;
        if (MAIN_INT(0x8fb90) != 0)
            MAIN_INT(0x8fb90) = 0;
        UI_WINDOW(self)->Activate();
        break;
    case 0x21:
        if (EQ_GLOBAL(int, 0x006B68AC) == 0)
        {
            if (g_pc_guess->inventory_items[0] != 0)
            {
                sprintf(g_messageBox + 0x1b9, EQ_ARRAY(char, 0x0055ADD4), g_pc_guess->inventory_items[0]);
            }
            else
            {
                if (CURSOR_COIN == 0)
                    break;
                strcpy(g_messageBox + 0x1b9, EQ_ARRAY(char, 0x0055AD8C));
            }
            *(u8*)(g_messageBox + 0x17c) = 5;
            UI_WINDOW(g_messageBox)->Activate();
            pinstEQ_Requestor_guess->timeout_guess = DISPLAY_TIME + 10000;
            break;
        }
        if (g_pc_guess->inventory_items[0] != 0)
        {
            destroy.fromSlot = 0;
            destroy.toSlot = -1;
            destroy.amount = 0;
            SEND_MESSAGE(0x212c, &destroy, 0xc);
            CURSOR_HAS_ITEM = 0;
            operator delete(g_pc_guess->inventory_items[0]);
            g_pc_guess->inventory_items[0] = 0;
            break;
        }
        if (CURSOR_COIN == 0)
            break;
        move.toSlot = -1;
        move.fromSlot = 0;
        if (g_pc_guess->platinum_cursor > 0)
        {
            move.fromCoin = 3;
            move.amount = g_pc_guess->platinum_cursor;
        }
        else if (g_pc_guess->gold_cursor > 0)
        {
            move.fromCoin = 2;
            move.amount = g_pc_guess->gold_cursor;
        }
        else if (g_pc_guess->silver_cursor > 0)
        {
            move.fromCoin = 1;
            move.amount = g_pc_guess->silver_cursor;
        }
        else if (g_pc_guess->copper_cursor > 0)
        {
            move.fromCoin = 0;
            move.amount = g_pc_guess->copper_cursor;
        }
        if (move.amount <= 0)
            break;
        move.toCoin = move.fromCoin;
        SEND_MESSAGE(0x212d, &move, 0x14);
        CURSOR_COIN = 0;
        g_pc_guess->platinum_cursor = 0;
        g_pc_guess->gold_cursor = 0;
        g_pc_guess->silver_cursor = 0;
        g_pc_guess->copper_cursor = 0;
        break;
    case 0x22:
        if (MAIN_BYTE(0x100c) != 0)
        {
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                g_game_state_guess = 2;
                CHANGE_VIDEO_MODE(0);
                MAIN_BYTE(0xfe1) = 0;
            }
            UI_WINDOW(self)->Deactivate();
            MAIN_MODE = MAIN_BYTE(0xfe1);
            goto activateAndReturn;
        }
        if (BANK_OPEN != 0)
        {
            if (HOUSE_BANK != 0)
            {
                if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
                    EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
                EQ_METHOD(0, void, 0x0047D6E2, (char*), self)();
                return;
            }
            EQ_METHOD(1, void, 0x004C53D8, (void*, int), g_everquest_guess)(1);
            return;
        }
        if (MERCHANT_OPEN != 0 && g_merchant != 0)
        {
            EQ_METHOD(0, void, 0x00464AB6, (char*), self)();
            return;
        }
        if (LOOT_OPEN != 0 && g_corpse != 0)
        {
            EQ_METHOD(0, void, 0x00464C8D, (char*), self)();
            return;
        }
        if (TRADE_OPEN != 0)
        {
            DSP_CHAT(EQ_ARRAY(char, 0x0055AD64), 0xf);
            return;
        }
        UI_WINDOW(self)->Deactivate();
        MAIN_SHORT(0x5f186) = -1;
        MAIN_MODE = MAIN_BYTE(0xfe2);
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f18a) = -1;
        UI_WINDOW(self)->Activate();
        if (EQ_GLOBAL(int, 0x00590170) != 2)
            return;
        g_game_state_guess = 2;
        CHANGE_VIDEO_MODE(0);
        return;
    case 0x23:
        if (CURSOR_COIN != 0)
        {
            move.fromSlot = 0;
            move.toSlot = 1;
            if (BANK_OPEN == 0 || HOUSE_BANK != 0)
                goto coinsToInventory;
            if (CURSOR_COIN == 1)
                goto platinumToInventory;
            if (CURSOR_COIN == 2)
            {
                if (g_pc_guess->gold_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->gold_cursor;
                g_pc_guess->platinum += g_pc_guess->gold_cursor / 10;
                g_pc_guess->gold_cursor = g_pc_guess->gold_cursor % 10;
                move.fromCoin = 2;
                move.toCoin = 3;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 3)
            {
                if (g_pc_guess->silver_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->silver_cursor;
                g_pc_guess->platinum += g_pc_guess->silver_cursor / 100;
                g_pc_guess->silver_cursor = g_pc_guess->silver_cursor % 100;
                move.fromCoin = 1;
                move.toCoin = 3;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 4)
            {
                if (g_pc_guess->copper_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->copper_cursor;
                g_pc_guess->platinum += g_pc_guess->copper_cursor / 1000;
                g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 1000;
                move.fromCoin = 0;
                move.toCoin = 3;
                goto sendCoinMove;
            }
            goto resetCursorCoin;
        }
        if (g_pc_guess->platinum > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
            MONEY_SLIDER(&g_pc_guess->platinum, 1, 1);
        break;
    case 0x24:
        if (CURSOR_COIN != 0)
        {
            move.fromSlot = 0;
            move.toSlot = 1;
            if (BANK_OPEN == 0 || HOUSE_BANK != 0)
                goto coinsToInventory;
            if (CURSOR_COIN == 1)
            {
                if (g_pc_guess->platinum_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->gold += g_pc_guess->platinum_cursor * 10;
                move.amount = g_pc_guess->platinum_cursor;
                g_pc_guess->platinum_cursor = 0;
                move.fromCoin = 3;
                move.toCoin = 2;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 2)
                goto goldToInventory;
            if (CURSOR_COIN == 3)
            {
                if (g_pc_guess->silver_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->silver_cursor;
                g_pc_guess->gold += g_pc_guess->silver_cursor / 10;
                g_pc_guess->silver_cursor = g_pc_guess->silver_cursor % 10;
                move.fromCoin = 1;
                move.toCoin = 2;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 4)
            {
                if (g_pc_guess->copper_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->copper_cursor;
                g_pc_guess->gold += g_pc_guess->copper_cursor / 100;
                g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 100;
                move.fromCoin = 0;
                move.toCoin = 2;
                goto sendCoinMove;
            }
            goto resetCursorCoin;
        }
        if (g_pc_guess->gold > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
            MONEY_SLIDER(&g_pc_guess->gold, 2, 1);
        break;
    case 0x25:
        if (CURSOR_COIN != 0)
        {
            move.fromSlot = 0;
            move.toSlot = 1;
            if (BANK_OPEN == 0 || HOUSE_BANK != 0)
                goto coinsToInventory;
            if (CURSOR_COIN == 1)
            {
                if (g_pc_guess->platinum_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->silver += g_pc_guess->platinum_cursor * 100;
                move.amount = g_pc_guess->platinum_cursor;
                g_pc_guess->platinum_cursor = 0;
                move.fromCoin = 3;
                move.toCoin = 1;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 2)
            {
                if (g_pc_guess->gold_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->silver += g_pc_guess->gold_cursor * 10;
                move.amount = g_pc_guess->gold_cursor;
                g_pc_guess->gold_cursor = 0;
                move.fromCoin = 2;
                move.toCoin = 1;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 3)
                goto silverToInventory;
            if (CURSOR_COIN == 4)
            {
                if (g_pc_guess->copper_cursor <= 0)
                    goto resetCursorCoin;
                move.amount = g_pc_guess->copper_cursor;
                g_pc_guess->silver += g_pc_guess->copper_cursor / 10;
                g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 10;
                move.fromCoin = 0;
                move.toCoin = 1;
                goto sendCoinMove;
            }
            goto resetCursorCoin;
        }
        if (g_pc_guess->silver > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
            MONEY_SLIDER(&g_pc_guess->silver, 3, 1);
        break;
    case 0x26:
        if (CURSOR_COIN != 0)
        {
            move.fromSlot = 0;
            move.toSlot = 1;
            if (BANK_OPEN == 0 || HOUSE_BANK != 0)
                goto coinsToInventory;
            if (CURSOR_COIN == 1)
            {
                if (g_pc_guess->platinum_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->copper += g_pc_guess->platinum_cursor * 1000;
                move.amount = g_pc_guess->platinum_cursor;
                g_pc_guess->platinum_cursor = 0;
                move.fromCoin = 3;
                move.toCoin = 0;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 2)
            {
                if (g_pc_guess->gold_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->copper += g_pc_guess->gold_cursor * 100;
                move.amount = g_pc_guess->gold_cursor;
                g_pc_guess->gold_cursor = 0;
                move.fromCoin = 2;
                move.toCoin = 0;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 3)
            {
                if (g_pc_guess->silver_cursor <= 0)
                    goto resetCursorCoin;
                g_pc_guess->copper += g_pc_guess->silver_cursor * 10;
                move.amount = g_pc_guess->silver_cursor;
                g_pc_guess->silver_cursor = 0;
                move.toCoin = 0;
                move.fromCoin = 1;
                goto sendCoinMove;
            }
            if (CURSOR_COIN == 4)
                goto copperToInventory;
            goto resetCursorCoin;
        }
        if (g_pc_guess->copper > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
            MONEY_SLIDER(&g_pc_guess->copper, 4, 1);
        break;
    case 0x28:
        if (MERCHANT_OPEN == 0 && LOOT_OPEN == 0 && BANK_OPEN == 0 && MAIN_INT(0x8fb90) != 0 &&
            MAIN_SHORT(0x685c8) > 0)
        {
            MAIN_SHORT(0x685c8) = MAIN_SHORT(0x685c8) - 1;
            EQ_METHOD(1, void, 0x00463CBE, (char*, int), self)(-1);
        }
        break;
    case 0x29:
        if (MERCHANT_OPEN == 0 && LOOT_OPEN == 0 && BANK_OPEN == 0 && MAIN_INT(0x8fb90) != 0 &&
            MAIN_SHORT(0x685c8) < MAIN_INT(0x685bc) - 6)
        {
            MAIN_SHORT(0x685c8) = MAIN_SHORT(0x685c8) + 1;
            EQ_METHOD(1, void, 0x00463CBE, (char*, int), self)(1);
        }
        break;
    case 0x66:
        if (CURSOR_COIN == 0)
            break;
        move.fromSlot = 0;
        move.toSlot = 1;
        goto coinsToInventory;
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
    case 0xaf:
    case 0xb0:
    case 0xb1:
        if (CURSOR_COIN != 0)
            break;
        buttonId -= 0xaa;
        if (HOUSE_BANK != 0)
        {
            if (g_pc_guess->inventory_items[0] != 0)
                goto dropIntoHouseBank;
            slotPtr = &HOUSE_ITEMS[buttonId];
            if (*slotPtr == 0)
                return;
            if (COMBINE(*slotPtr) != 0)
            {
                *(int*)(g_quantitySlider + 0x1dc) = 1;
                *(short*)(g_quantitySlider + 0x1e0) = buttonId;
                QUANTITY_SLIDER(slotPtr, buttonId);
                return;
            }
            itemMove.spawnId = g_local_player_guess->spawn_id;
            itemMove.npcId = NPC_ID;
            itemMove.itemNumber = -1;
            itemMove.slot = buttonId;
            itemMove.cost = 0;
            SEND_MESSAGE(0x2112, &itemMove, 0x1c);
            g_pc_guess->inventory_items[0] = *slotPtr;
            *slotPtr = 0;
            item = g_pc_guess->inventory_items[0];
            if (item->type == 1 && CLOSE_BAG(item) != 0)
            {
                if (item == pinstEQ_Bag1_guess->item_guess)
                {
                    UI_WINDOW(g_bagWindow1)->Deactivate();
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (item == pinstEQ_Bag2_guess->item_guess)
                {
                    UI_WINDOW(g_bagWindow2)->Deactivate();
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
                else
                    EQTRACE_guess(EQ_ARRAY(char, 0x0055AD30));
            }
            goto setCursorItemFlag;
        }
        if (g_pc_guess->inventory_items[0] == 0)
        {
            slot = buttonId;
            if (g_pc_guess->bank_items[slot] == 0)
                return;
            if (COMBINE(g_pc_guess->bank_items[slot]) != 0)
            {
                QUANTITY_SLIDER(&g_pc_guess->bank_items[slot], slot + 2000);
                return;
            }
            item = g_pc_guess->bank_items[slot];
            if (item->type == 1)
            {
                if (WINDOW_ACTIVE(g_bagWindow1) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                {
                    BAG_OPEN(item) = 0;
                    UI_WINDOW(g_bagWindow1)->Deactivate();
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (WINDOW_ACTIVE(g_bagWindow2) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                {
                    BAG_OPEN(item) = 0;
                    UI_WINDOW(g_bagWindow2)->Deactivate();
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            MOVE_ITEM(slot + 2000, 0, 1, 1);
            goto setCursorItemFlag;
        }
        slot = buttonId;
        item = g_pc_guess->bank_items[slot];
        if (item != 0 && item->type == 1)
        {
            if (WINDOW_ACTIVE(g_bagWindow1) != 0 && item == pinstEQ_Bag1_guess->item_guess)
            {
                BAG_OPEN(item) = 0;
                UI_WINDOW(g_bagWindow1)->Deactivate();
                pinstEQ_Bag1_guess->item_guess = 0;
            }
            else if (WINDOW_ACTIVE(g_bagWindow2) != 0 && item == pinstEQ_Bag2_guess->item_guess)
            {
                BAG_OPEN(item) = 0;
                UI_WINDOW(g_bagWindow2)->Deactivate();
                pinstEQ_Bag2_guess->item_guess = 0;
            }
        }
        MOVE_ITEM(0, slot + 2000, 1, 1);
        goto setCursorItemFlag;
    case 0xb4:
        if (CURSOR_COIN == 0)
        {
            if (HOUSE_BANK != 0)
            {
                if (HOUSE_COINS[0] <= 0)
                    break;
                *(int*)(g_moneySlider + 0x1dc) = 1;
                MONEY_SLIDER(&HOUSE_COINS[0], 1, 7);
                break;
            }
            if (g_pc_guess->platinum_bank <= 0)
                break;
            MONEY_SLIDER(&g_pc_guess->platinum_bank, 1, 2);
            break;
        }
        if (HOUSE_BANK != 0)
        {
            if (CURSOR_COIN != 1)
                break;
            if (g_pc_guess->platinum_cursor <= 0)
                break;
            HOUSE_COINS[0] += g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
            CURSOR_COIN = 0;
            houseCoins.spawnId = g_local_player_guess->spawn_id;
            houseCoins.npcId = NPC_ID;
            houseCoins.coin = 0;
            houseCoins.amount = HOUSE_COINS[0];
            SEND_MESSAGE(0x2111, &houseCoins, 0x10);
            break;
        }
        move.fromSlot = 0;
        move.toSlot = 2;
        if (CURSOR_COIN == 1)
        {
            if (g_pc_guess->platinum_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->platinum_bank += g_pc_guess->platinum_cursor;
            move.amount = g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
            move.fromCoin = 3;
            move.toCoin = 3;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 2)
        {
            if (g_pc_guess->gold_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->gold_cursor;
            g_pc_guess->platinum_bank += g_pc_guess->gold_cursor / 10;
            g_pc_guess->gold_cursor = g_pc_guess->gold_cursor % 10;
            move.fromCoin = 2;
            move.toCoin = 3;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 3)
        {
            if (g_pc_guess->silver_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->silver_cursor;
            g_pc_guess->platinum_bank += g_pc_guess->silver_cursor / 100;
            g_pc_guess->silver_cursor = g_pc_guess->silver_cursor % 100;
            move.fromCoin = 1;
            move.toCoin = 3;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 4)
        {
            if (g_pc_guess->copper_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->copper_cursor;
            g_pc_guess->platinum_bank += g_pc_guess->copper_cursor / 1000;
            g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 1000;
            move.fromCoin = 0;
            move.toCoin = 3;
            goto sendCoinMove;
        }
        goto resetCursorCoin;
    case 0xb5:
        if (CURSOR_COIN == 0)
        {
            if (HOUSE_BANK != 0)
            {
                if (HOUSE_COINS[1] <= 0)
                    break;
                *(int*)(g_moneySlider + 0x1dc) = 1;
                MONEY_SLIDER(&HOUSE_COINS[1], 2, 7);
                break;
            }
            if (g_pc_guess->gold_bank <= 0)
                break;
            MONEY_SLIDER(&g_pc_guess->gold_bank, 2, 2);
            break;
        }
        if (HOUSE_BANK != 0)
        {
            if (CURSOR_COIN != 2)
                break;
            if (g_pc_guess->gold_cursor <= 0)
                break;
            HOUSE_COINS[1] += g_pc_guess->gold_cursor;
            g_pc_guess->gold_cursor = 0;
            CURSOR_COIN = 0;
            houseCoins.spawnId = g_local_player_guess->spawn_id;
            houseCoins.npcId = NPC_ID;
            houseCoins.coin = 1;
            houseCoins.amount = HOUSE_COINS[1];
            SEND_MESSAGE(0x2111, &houseCoins, 0x10);
            break;
        }
        move.fromSlot = 0;
        move.toSlot = 2;
        if (CURSOR_COIN == 1)
        {
            if (g_pc_guess->platinum_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->gold_bank += g_pc_guess->platinum_cursor * 10;
            move.amount = g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
            move.fromCoin = 3;
            move.toCoin = 2;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 2)
        {
            if (g_pc_guess->gold_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->gold_bank += g_pc_guess->gold_cursor;
            move.amount = g_pc_guess->gold_cursor;
            g_pc_guess->gold_cursor = 0;
            move.fromCoin = 2;
            move.toCoin = 2;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 3)
        {
            if (g_pc_guess->silver_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->silver_cursor;
            g_pc_guess->gold_bank += g_pc_guess->silver_cursor / 10;
            g_pc_guess->silver_cursor = g_pc_guess->silver_cursor % 10;
            move.fromCoin = 1;
            move.toCoin = 2;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 4)
        {
            if (g_pc_guess->copper_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->copper_cursor;
            g_pc_guess->gold_bank += g_pc_guess->copper_cursor / 100;
            g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 100;
            move.fromCoin = 0;
            move.toCoin = 2;
            goto sendCoinMove;
        }
        goto resetCursorCoin;
    case 0xb6:
        if (CURSOR_COIN == 0)
        {
            if (HOUSE_BANK != 0)
            {
                if (HOUSE_COINS[2] <= 0)
                    break;
                *(int*)(g_moneySlider + 0x1dc) = 1;
                MONEY_SLIDER(&HOUSE_COINS[2], 3, 7);
                break;
            }
            if (g_pc_guess->silver_bank <= 0)
                break;
            MONEY_SLIDER(&g_pc_guess->silver_bank, 3, 2);
            break;
        }
        if (HOUSE_BANK != 0)
        {
            if (CURSOR_COIN != 3)
                break;
            if (g_pc_guess->silver_cursor <= 0)
                break;
            HOUSE_COINS[2] += g_pc_guess->silver_cursor;
            g_pc_guess->silver_cursor = 0;
            CURSOR_COIN = 0;
            houseCoins.spawnId = g_local_player_guess->spawn_id;
            houseCoins.npcId = NPC_ID;
            houseCoins.coin = 2;
            houseCoins.amount = HOUSE_COINS[2];
            SEND_MESSAGE(0x2111, &houseCoins, 0x10);
            break;
        }
        move.fromSlot = 0;
        move.toSlot = 2;
        if (CURSOR_COIN == 1)
        {
            if (g_pc_guess->platinum_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->silver_bank += g_pc_guess->platinum_cursor * 100;
            move.amount = g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
            move.fromCoin = 3;
            move.toCoin = 1;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 2)
        {
            if (g_pc_guess->gold_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->silver_bank += g_pc_guess->gold_cursor * 10;
            move.amount = g_pc_guess->gold_cursor;
            g_pc_guess->gold_cursor = 0;
            move.fromCoin = 2;
            move.toCoin = 1;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 3)
        {
            if (g_pc_guess->silver_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->silver_bank += g_pc_guess->silver_cursor;
            move.amount = g_pc_guess->silver_cursor;
            g_pc_guess->silver_cursor = 0;
            move.fromCoin = 1;
            move.toCoin = 1;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 4)
        {
            if (g_pc_guess->copper_cursor <= 0)
                goto resetCursorCoin;
            move.amount = g_pc_guess->copper_cursor;
            g_pc_guess->silver_bank += g_pc_guess->copper_cursor / 10;
            g_pc_guess->copper_cursor = g_pc_guess->copper_cursor % 10;
            move.fromCoin = 0;
            move.toCoin = 1;
            goto sendCoinMove;
        }
        goto resetCursorCoin;
    case 0xb7:
        if (CURSOR_COIN == 0)
        {
            if (HOUSE_BANK != 0)
            {
                if (HOUSE_COINS[3] <= 0)
                    break;
                *(int*)(g_moneySlider + 0x1dc) = 1;
                MONEY_SLIDER(&HOUSE_COINS[3], 4, 7);
                break;
            }
            if (g_pc_guess->copper_bank <= 0)
                break;
            MONEY_SLIDER(&g_pc_guess->copper_bank, 4, 2);
            break;
        }
        if (HOUSE_BANK != 0)
        {
            if (CURSOR_COIN != 4)
                break;
            if (g_pc_guess->copper_cursor <= 0)
                break;
            HOUSE_COINS[3] += g_pc_guess->copper_cursor;
            g_pc_guess->copper_cursor = 0;
            CURSOR_COIN = 0;
            houseCoins.spawnId = g_local_player_guess->spawn_id;
            houseCoins.npcId = NPC_ID;
            houseCoins.coin = 3;
            houseCoins.amount = HOUSE_COINS[3];
            SEND_MESSAGE(0x2111, &houseCoins, 0x10);
            break;
        }
        move.fromSlot = 0;
        move.toSlot = 2;
        if (CURSOR_COIN == 1)
        {
            if (g_pc_guess->platinum_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->copper_bank += g_pc_guess->platinum_cursor * 1000;
            move.amount = g_pc_guess->platinum_cursor;
            g_pc_guess->platinum_cursor = 0;
            move.fromCoin = 3;
            move.toCoin = 0;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 2)
        {
            if (g_pc_guess->gold_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->copper_bank += g_pc_guess->gold_cursor * 100;
            move.amount = g_pc_guess->gold_cursor;
            g_pc_guess->gold_cursor = 0;
            move.fromCoin = 2;
            move.toCoin = 0;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 3)
        {
            if (g_pc_guess->silver_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->copper_bank += g_pc_guess->silver_cursor * 10;
            move.amount = g_pc_guess->silver_cursor;
            g_pc_guess->silver_cursor = 0;
            move.toCoin = 0;
            move.fromCoin = 1;
            goto sendCoinMove;
        }
        if (CURSOR_COIN == 4)
        {
            if (g_pc_guess->copper_cursor <= 0)
                goto resetCursorCoin;
            g_pc_guess->copper_bank += g_pc_guess->copper_cursor;
            move.amount = g_pc_guess->copper_cursor;
            g_pc_guess->copper_cursor = 0;
            move.fromCoin = 0;
            move.toCoin = 0;
            goto sendCoinMove;
        }
        goto resetCursorCoin;
    case 0xb8:
        if (HOUSE_BANK != 0)
            break;
        if (CURSOR_COIN == 0)
        {
            if (g_pc_guess->inventory_items[0] != 0)
            {
                freeSlot = -1;
                index = 0;
                slotPtr = g_pc_guess->bank_items;
                do
                {
                    if (freeSlot < 0 && *slotPtr == 0)
                        freeSlot = index;
                    index++;
                    slotPtr++;
                } while (index < 8);
                if (freeSlot >= 0)
                    MOVE_ITEM(0, freeSlot + 2000, 0, 1);
            }
        }
        else
        {
            move.fromSlot = 0;
            move.toSlot = 2;
            if (CURSOR_COIN == 1)
            {
                if (g_pc_guess->platinum_cursor > 0)
                {
                    g_pc_guess->platinum_bank += g_pc_guess->platinum_cursor;
                    move.amount = g_pc_guess->platinum_cursor;
                    g_pc_guess->platinum_cursor = 0;
                    move.fromCoin = 3;
                    move.toCoin = 3;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                }
            }
            else if (CURSOR_COIN == 2)
            {
                if (g_pc_guess->gold_cursor > 0)
                {
                    g_pc_guess->gold_bank += g_pc_guess->gold_cursor;
                    move.amount = g_pc_guess->gold_cursor;
                    g_pc_guess->gold_cursor = 0;
                    move.fromCoin = 2;
                    move.toCoin = 2;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                }
            }
            else if (CURSOR_COIN == 3)
            {
                if (g_pc_guess->silver_cursor > 0)
                {
                    g_pc_guess->silver_bank += g_pc_guess->silver_cursor;
                    move.amount = g_pc_guess->silver_cursor;
                    g_pc_guess->silver_cursor = 0;
                    move.fromCoin = 1;
                    move.toCoin = 1;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                }
            }
            else if (CURSOR_COIN == 4 && g_pc_guess->copper_cursor > 0)
            {
                g_pc_guess->copper_bank += g_pc_guess->copper_cursor;
                move.amount = g_pc_guess->copper_cursor;
                g_pc_guess->copper_cursor = 0;
                move.fromCoin = 0;
                move.toCoin = 0;
                SEND_MESSAGE(0x212d, &move, 0x14);
            }
            CURSOR_COIN = 0;
            if (g_pc_guess->platinum_cursor > 0)
                CURSOR_COIN = 1;
            else if (g_pc_guess->gold_cursor > 0)
                CURSOR_COIN = 2;
            else if (g_pc_guess->silver_cursor > 0)
                CURSOR_COIN = 3;
            else if (g_pc_guess->copper_cursor > 0)
                CURSOR_COIN = 4;
        }
        if (g_pc_guess->inventory_items[0] != 0)
            CURSOR_HAS_ITEM = 1;
        else
            CURSOR_HAS_ITEM = 0;
        break;
    case 0x110:
    case 0x123:
        if (MERCHANT_OPEN != 0 || LOOT_OPEN != 0)
        {
            LIST_PAGE -= 1;
            if (LIST_PAGE < 0)
                LIST_PAGE = 0;
        }
        break;
    case 0x111:
    case 0x124:
        if (MERCHANT_OPEN != 0 || LOOT_OPEN != 0)
        {
            LIST_PAGE += 1;
            if (MERCHANT_OPEN != 0)
            {
                if (LIST_PAGE > 9)
                    LIST_PAGE = 9;
            }
            else if (LOOT_OPEN != 0 && LIST_PAGE > 7)
                LIST_PAGE = 7;
        }
        break;
    case 0x112:
        EQ_METHOD(0, void, 0x004578F4, (char*), self)();
        break;
    case 0x143:
        if (TRADE_OPEN != 0 && g_trader != 0 && TRADE_ACCEPTED == 0)
        {
            TRADE_ACCEPTED = 1;
            accept.accept = 1;
            accept.traderId = g_trader->spawn_id;
            SEND_MESSAGE(0x20da, &accept, 8);
        }
        break;
    case 0x144:
        if (TRADE_OPEN == 0)
            goto merchantWindow;
        if (g_trader != 0 && TRADE_OTHER_ACCEPTED + TRADE_ACCEPTED < 2)
        {
            pinstEQ_Main_guess->tradeSessionActive_guess = 0;
            cancel.traderId = g_trader->spawn_id;
            cancel.spawnId = g_local_player_guess->spawn_id;
            SEND_MESSAGE(0x20db, &cancel, 8);
        }
        break;
    }
    goto tradeWindow;

coinsToInventory:
    if (CURSOR_COIN == 1)
        goto platinumToInventory;
    if (CURSOR_COIN == 2)
        goto goldToInventory;
    if (CURSOR_COIN == 3)
        goto silverToInventory;
    if (CURSOR_COIN != 4)
        goto resetCursorCoin;
copperToInventory:
    if (g_pc_guess->copper_cursor <= 0)
        goto resetCursorCoin;
    g_pc_guess->copper += g_pc_guess->copper_cursor;
    move.amount = g_pc_guess->copper_cursor;
    g_pc_guess->copper_cursor = 0;
    move.fromCoin = 0;
    move.toCoin = 0;
    goto sendCoinMove;
platinumToInventory:
    if (g_pc_guess->platinum_cursor <= 0)
        goto resetCursorCoin;
    g_pc_guess->platinum += g_pc_guess->platinum_cursor;
    move.amount = g_pc_guess->platinum_cursor;
    g_pc_guess->platinum_cursor = 0;
    move.fromCoin = 3;
    move.toCoin = 3;
    goto sendCoinMove;
goldToInventory:
    if (g_pc_guess->gold_cursor <= 0)
        goto resetCursorCoin;
    g_pc_guess->gold += g_pc_guess->gold_cursor;
    move.amount = g_pc_guess->gold_cursor;
    g_pc_guess->gold_cursor = 0;
    move.fromCoin = 2;
    move.toCoin = 2;
    goto sendCoinMove;
silverToInventory:
    if (g_pc_guess->silver_cursor <= 0)
        goto resetCursorCoin;
    g_pc_guess->silver += g_pc_guess->silver_cursor;
    move.amount = g_pc_guess->silver_cursor;
    g_pc_guess->silver_cursor = 0;
    move.fromCoin = 1;
    move.toCoin = 1;
sendCoinMove:
    SEND_MESSAGE(0x212d, &move, 0x14);
resetCursorCoin:
    CURSOR_COIN = 0;
    if (g_pc_guess->platinum_cursor > 0)
        CURSOR_COIN = 1;
    else if (g_pc_guess->gold_cursor > 0)
        CURSOR_COIN = 2;
    else if (g_pc_guess->silver_cursor > 0)
        CURSOR_COIN = 3;
    else if (g_pc_guess->copper_cursor > 0)
        CURSOR_COIN = 4;
    goto tradeWindow;

dropIntoHouseBank:
    if (CAN_DROP(g_pc_guess->inventory_items[0], 1) == 0)
        return;
    slotPtr = &HOUSE_ITEMS[buttonId];
    if (SWAP_COMBINE(g_pc_guess->inventory_items, slotPtr, 0, buttonId + 8000) != 0)
    {
        itemMove.spawnId = g_local_player_guess->spawn_id;
        itemMove.npcId = NPC_ID;
        itemMove.slot = buttonId;
        item = *slotPtr;
        itemMove.cost = 0;
        if (item == 0)
            itemMove.itemNumber = -1;
        else
        {
            itemMove.itemNumber = item->item_nr;
            itemMove.type = item->type;
            itemMove.cost = item->cost;
            if (itemMove.type == 0)
                memcpy(itemMove.detail, ITEM_DETAIL(item), 10);
        }
        SEND_MESSAGE(0x2112, &itemMove, 0x1c);
        goto setCursorItemFlag;
    }
    itemMove.spawnId = g_local_player_guess->spawn_id;
    itemMove.npcId = NPC_ID;
    itemMove.itemNumber = g_pc_guess->inventory_items[0]->item_nr;
    itemMove.slot = buttonId;
    if (*slotPtr == 0)
    {
        itemMove.cost = g_pc_guess->inventory_items[0]->cost;
        itemMove.type = g_pc_guess->inventory_items[0]->type;
        if (itemMove.type == 0)
            memcpy(itemMove.detail, ITEM_DETAIL(g_pc_guess->inventory_items[0]), 10);
        SEND_MESSAGE(0x2112, &itemMove, 0x1c);
        if (itemMove.type == 1)
        {
            index = 0;
            contents = BAG_CONTENTS(g_pc_guess->inventory_items[0]);
            bagSlot = buttonId * 10 + 30;
            do
            {
                itemMove.cost = 0;
                itemMove.slot = index + bagSlot;
                if (*contents == 0)
                    itemMove.itemNumber = -1;
                else
                {
                    itemMove.itemNumber = (*contents)->item_nr;
                    itemMove.type = (*contents)->type;
                    itemMove.cost = (*contents)->cost;
                    if (itemMove.type == 0)
                        memcpy(itemMove.detail, ITEM_DETAIL(*contents), 10);
                }
                SEND_MESSAGE(0x2112, &itemMove, 0x1c);
                index++;
                contents++;
            } while (index < 8);
        }
        *slotPtr = g_pc_guess->inventory_items[0];
        g_pc_guess->inventory_items[0] = 0;
    }
    else
    {
        itemMove.type = g_pc_guess->inventory_items[0]->type;
        itemMove.cost = g_pc_guess->inventory_items[0]->cost;
        if (itemMove.type == 0)
            memcpy(itemMove.detail, ITEM_DETAIL(g_pc_guess->inventory_items[0]), 10);
        SEND_MESSAGE(0x2112, &itemMove, 0x1c);
        if (itemMove.type == 1)
        {
            index = 0;
            contents = BAG_CONTENTS(g_pc_guess->inventory_items[0]);
            bagSlot = buttonId * 10 + 30;
            do
            {
                itemMove.cost = 0;
                itemMove.slot = index + bagSlot;
                if (*contents == 0)
                    itemMove.itemNumber = -1;
                else
                {
                    itemMove.itemNumber = (*contents)->item_nr;
                    itemMove.type = (*contents)->type;
                    itemMove.cost = (*contents)->cost;
                    if (itemMove.type == 0)
                        memcpy(itemMove.detail, ITEM_DETAIL(*contents), 10);
                }
                SEND_MESSAGE(0x2112, &itemMove, 0x1c);
                index++;
                contents++;
            } while (index < 8);
        }
        held = g_pc_guess->inventory_items[0];
        g_pc_guess->inventory_items[0] = *slotPtr;
        *slotPtr = held;
    }
setCursorItemFlag:
    if (g_pc_guess->inventory_items[0] != 0)
    {
        CURSOR_HAS_ITEM = 1;
        return;
    }
    CURSOR_HAS_ITEM = 0;
    return;

tradeWindow:
    if (TRADE_OPEN == 0)
        goto merchantWindow;
    if (g_trader == 0)
        goto merchantWindow;
    if (mouseButton == 1)
    {
        if (EQ_GLOBAL(int, 0x006B6498) != 0)
            goto merchantWindow;
        if (buttonId >= 300 && buttonId <= 0x133)
        {
            item = TRADE_THEIR_ITEMS[buttonId - 300];
            if (item != 0 && item->type == 1)
            {
                if (BAG_OPEN(item) == 1)
                {
                    if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow1)->Deactivate();
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (IS_ACTIVE(g_bagWindow2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow2)->Deactivate();
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                }
                else if (IS_ACTIVE(g_bagWindow1) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag1_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow1)->Open(-1);
                    pinstEQ_Bag1_guess->slot_guess = buttonId - 300;
                    pinstEQ_Bag1_guess->theirTradeContainer_guess = 1;
                }
                else if (IS_ACTIVE(g_bagWindow2) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag2_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow2)->Open(-1);
                    pinstEQ_Bag2_guess->slot_guess = buttonId - 300;
                    pinstEQ_Bag2_guess->theirTradeContainer_guess = 1;
                }
                if (EQ_GLOBAL(int, 0x006B6498) != 0)
                    goto merchantWindow;
            }
        }
        if (buttonId >= 0x136 && buttonId <= 0x13d)
        {
            item = TRADE_MY_ITEMS[buttonId - 0x136];
            if (item != 0 && item->type == 1)
            {
                if (BAG_OPEN(item) == 1)
                {
                    if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow1)->Deactivate();
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (IS_ACTIVE(g_bagWindow2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow2)->Deactivate();
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                }
                else if (IS_ACTIVE(g_bagWindow1) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag1_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow1)->Open(-1);
                    pinstEQ_Bag1_guess->slot_guess = buttonId - 0x136;
                    pinstEQ_Bag1_guess->myTradeContainer_guess = 1;
                }
                else if (IS_ACTIVE(g_bagWindow2) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag2_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow2)->Open(-1);
                    pinstEQ_Bag2_guess->slot_guess = buttonId - 0x136;
                    pinstEQ_Bag2_guess->myTradeContainer_guess = 1;
                }
            }
        }
        goto merchantWindow;
    }
    if (mouseButton != 0)
        goto merchantWindow;
    if (buttonId >= 0x13e && buttonId <= 0x142)
    {
        TRADE_ACCEPTED = 0;
        TRADE_OTHER_ACCEPTED = 0;
        if (CURSOR_COIN != 0)
        {
            move.fromSlot = 0;
            move.toSlot = 3;
            if (g_pc_guess->platinum_cursor > 0)
            {
                move.fromCoin = 3;
                move.toCoin = 3;
                move.amount = g_pc_guess->platinum_cursor;
                SEND_MESSAGE(0x212d, &move, 0x14);
                TRADE_COINS[0] += g_pc_guess->platinum_cursor;
                g_pc_guess->platinum_cursor = 0;
                CURSOR_COIN = 0;
            }
            if (g_pc_guess->gold_cursor > 0)
            {
                move.fromCoin = 2;
                move.toCoin = 2;
                move.amount = g_pc_guess->gold_cursor;
                SEND_MESSAGE(0x212d, &move, 0x14);
                TRADE_COINS[1] += g_pc_guess->gold_cursor;
                g_pc_guess->gold_cursor = 0;
                CURSOR_COIN = 0;
            }
            if (g_pc_guess->silver_cursor > 0)
            {
                move.fromCoin = 1;
                move.toCoin = 1;
                move.amount = g_pc_guess->silver_cursor;
                SEND_MESSAGE(0x212d, &move, 0x14);
                TRADE_COINS[2] += g_pc_guess->silver_cursor;
                g_pc_guess->silver_cursor = 0;
                CURSOR_COIN = 0;
            }
            if (g_pc_guess->copper_cursor > 0)
            {
                move.fromCoin = 0;
                move.toCoin = 0;
                move.amount = g_pc_guess->copper_cursor;
                SEND_MESSAGE(0x212d, &move, 0x14);
                TRADE_COINS[3] += g_pc_guess->copper_cursor;
                g_pc_guess->copper_cursor = 0;
                CURSOR_COIN = 0;
            }
        }
        else if (CURSOR_HAS_ITEM == 0)
        {
            move.fromSlot = 3;
            move.toSlot = 0;
            switch (buttonId)
            {
            case 0x13e:
                if (TRADE_COINS[0] > 0)
                {
                    move.amount = TRADE_COINS[0];
                    move.fromCoin = 3;
                    move.toCoin = 3;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                    g_pc_guess->platinum_cursor += TRADE_COINS[0];
                    TRADE_COINS[0] = 0;
                    CURSOR_COIN = 1;
                }
                break;
            case 0x13f:
                if (TRADE_COINS[1] > 0)
                {
                    move.amount = TRADE_COINS[1];
                    move.fromCoin = 2;
                    move.toCoin = 2;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                    g_pc_guess->gold_cursor += TRADE_COINS[1];
                    TRADE_COINS[1] = 0;
                    CURSOR_COIN = 2;
                }
                break;
            case 0x140:
                if (TRADE_COINS[2] > 0)
                {
                    move.amount = TRADE_COINS[2];
                    move.fromCoin = 1;
                    move.toCoin = 1;
                    SEND_MESSAGE(0x212d, &move, 0x14);
                    g_pc_guess->silver_cursor += TRADE_COINS[2];
                    TRADE_COINS[2] = 0;
                    CURSOR_COIN = 3;
                }
                break;
            case 0x141:
                if (TRADE_COINS[3] > 0)
                {
                    move.fromCoin = 0;
                    move.toCoin = 0;
                    move.amount = TRADE_COINS[3];
                    SEND_MESSAGE(0x212d, &move, 0x14);
                    g_pc_guess->copper_cursor += TRADE_COINS[3];
                    TRADE_COINS[3] = 0;
                    CURSOR_COIN = 4;
                }
                break;
            }
        }
    }
    if (buttonId >= 0x136 && buttonId <= 0x13d && CURSOR_COIN == 0)
    {
        TRADE_ACCEPTED = 0;
        TRADE_OTHER_ACCEPTED = 0;
        if (g_pc_guess->inventory_items[0] == 0)
        {
            if (TRADE_MY_ITEMS[buttonId - 0x136] != 0)
                goto tradeBusy;
            TRADE_ACCEPTED = 0;
            TRADE_OTHER_ACCEPTED = 0;
            MOVE_ITEM(buttonId + 0xa82, 0, 1, 1);
        }
        else
        {
            if ((g_trader == 0 || g_trader->type != 1) && CAN_DROP(g_pc_guess->inventory_items[0], 1) == 0)
                goto merchantWindow;
            if (TRADE_MY_ITEMS[buttonId - 0x136] != 0)
            {
            tradeBusy:
                DSP_CHAT(EQ_ARRAY(char, 0x0055AD64), 0xf);
                goto merchantWindow;
            }
            TRADE_ACCEPTED = 0;
            TRADE_OTHER_ACCEPTED = 0;
            MOVE_ITEM(0, buttonId + 0xa82, 1, 1);
        }
        if (g_pc_guess->inventory_items[0] == 0)
            CURSOR_HAS_ITEM = 0;
        else
            CURSOR_HAS_ITEM = 1;
    }

merchantWindow:
    if (MERCHANT_OPEN == 0)
        goto notMerchant;
    if (mouseButton != 0)
        goto rightClickInventoryItem;
    if (buttonId > 0x1d)
        goto equipSlots;
    if (EQ_GLOBAL(int, 0x006B649C) != 0)
        goto hotButtonFromInventory;
    if (g_pc_guess->inventory_items[buttonId] == 0)
        goto hotButtonFromInventory;
    MAIN_INT(0x6c658) = (int)&g_pc_guess->inventory_items[buttonId];
    MAIN_SHORT(0x6c65c) = buttonId + 100;
    strcpy(itemName, (char*)g_pc_guess->inventory_items[buttonId]);
    item = g_pc_guess->inventory_items[buttonId];
    if (item->type == 0)
    {
        if (COMBINE(item) != 0)
        {
            price = SELL_PRICE(g_pc_guess->inventory_items[buttonId]);
            merchantName = TRIM_NAME(g_merchant->name);
            sprintf(text, EQ_ARRAY(char, 0x0053F134), merchantName, price, itemName);
        }
        else
        {
            price = SELL_PRICE(g_pc_guess->inventory_items[buttonId]);
            merchantName = TRIM_NAME(g_merchant->name);
            sprintf(text, EQ_ARRAY(char, 0x0053EE14), merchantName, price, itemName);
        }
        DSP_CHAT(text, 0x113);
        goto hotButtonFromInventory;
    }
    if (item->type == 2)
    {
        price = SELL_PRICE(item);
        merchantName = TRIM_NAME(g_merchant->name);
        sprintf(text, EQ_ARRAY(char, 0x0053EE14), merchantName, price, itemName);
        DSP_CHAT(text, 0x113);
        goto hotButtonFromInventory;
    }
    if (item->type != 1)
        goto hotButtonFromInventory;
    index = 0;
    if (BAG_SLOT_COUNT(item) != 0)
    {
        do
        {
            slotPtr = &BAG_CONTENTS(item)[index];
            if (*slotPtr != 0)
            {
                strcpy(itemName, (char*)*slotPtr);
                if (COMBINE(*slotPtr) != 0)
                {
                    price = SELL_PRICE(*slotPtr);
                    merchantName = TRIM_NAME(g_merchant->name);
                    sprintf(text, EQ_ARRAY(char, 0x0053F134), merchantName, price, itemName);
                }
                else
                {
                    price = SELL_PRICE(*slotPtr);
                    merchantName = TRIM_NAME(g_merchant->name);
                    sprintf(text, EQ_ARRAY(char, 0x0053EE14), merchantName, price, itemName);
                }
                DSP_CHAT(text, 0x113);
            }
            index++;
        } while (index < (short)BAG_SLOT_COUNT(item));
    }
    held = g_pc_guess->inventory_items[buttonId];
    price = SELL_PRICE(held);
    merchantName = TRIM_NAME(g_merchant->name);
    sprintf(text, EQ_ARRAY(char, 0x0053EE14), merchantName, price, held);
    DSP_CHAT(text, 0x113);

notMerchant:
    if (mouseButton != 0)
        goto rightClickInventoryItem;
hotButtonFromInventory:
    if (buttonId > 0x1d)
        goto equipSlots;
    if (EQ_GLOBAL(int, 0x006B649C) == 0)
        goto equipSlots;
    if (MAIN_BYTE(0x100c) != 0)
        goto equipSlots;
    MAIN_BYTE(0x685ae) = 5;
    heldValue = buttonId;
holdHotButton:
    MAIN_SHORT(0x685b0) = heldValue;
startHotButtonDrag:
    HOTBUTTON_HELD = 1;
    return;

rightClickInventoryItem:
    if (mouseButton == 1 && buttonId <= 0x1d && MAIN_BYTE(0x100c) == 0 && LOOT_OPEN == 0 && MERCHANT_OPEN == 0)
    {
        item = g_pc_guess->inventory_items[buttonId];
        if (item != 0 && item->type == 0 && item != 0)
            RIGHT_CLICK_ITEM(item, item, buttonId);
    }

equipSlots:
    if (MERCHANT_OPEN != 0)
        goto merchantBuyInfo;
    if (mouseButton == 0 && buttonId <= 0x1d && MAIN_BYTE(0x100c) == 0)
    {
        if (CURSOR_COIN != 0)
            return;
        if (g_pc_guess->inventory_items[0] == 0)
        {
            slot = buttonId;
            if (g_pc_guess->inventory_items[slot] != 0)
            {
                if (COMBINE(g_pc_guess->inventory_items[slot]) != 0)
                {
                    QUANTITY_SLIDER(&g_pc_guess->inventory_items[slot], slot);
                }
                else
                {
                    item = g_pc_guess->inventory_items[slot];
                    if (item->type == 1)
                    {
                        if (WINDOW_ACTIVE(g_bagWindow1) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                        {
                            BAG_OPEN(item) = 0;
                            UI_WINDOW(g_bagWindow1)->Deactivate();
                            pinstEQ_Bag1_guess->item_guess = 0;
                        }
                        else if (WINDOW_ACTIVE(g_bagWindow2) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                        {
                            BAG_OPEN(item) = 0;
                            UI_WINDOW(g_bagWindow2)->Deactivate();
                            pinstEQ_Bag2_guess->item_guess = 0;
                        }
                    }
                    HANDLE_MATERIAL(g_pc_guess, buttonId, g_pc_guess->inventory_items[0],
                        g_pc_guess->inventory_items[slot]);
                    if (g_soundManager != 0)
                        WAVE_PLAY(0x92, 0);
                    MOVE_ITEM(slot, 0, 1, 1);
                    UPDATE_EQUIPMENT(g_local_player_guess, buttonId);
                    UPDATE_LIGHT(g_pc_guess);
                    if (g_pc_guess->inventory_items[0] == 0)
                        CURSOR_HAS_ITEM = 0;
                    else
                        CURSOR_HAS_ITEM = 1;
                }
            }
        }
        else
        {
            held = g_pc_guess->inventory_items[0];
            if (CAN_EQUIP_IN_SLOT(g_pc_guess, held->equipable_slots, buttonId, held) != 0)
            {
                if (buttonId < 0x16 && CAN_USE_ITEM(g_pc_guess->inventory_items[0]) == 0)
                {
                    DSP_CHAT(EQ_ARRAY(char, 0x0055ACF8), 0xd);
                }
                else
                {
                    slot = buttonId;
                    item = g_pc_guess->inventory_items[slot];
                    if (item != 0 && item->type == 1)
                    {
                        if (WINDOW_ACTIVE(g_bagWindow1) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                        {
                            BAG_OPEN(item) = 0;
                            UI_WINDOW(g_bagWindow1)->Deactivate();
                            pinstEQ_Bag1_guess->item_guess = 0;
                        }
                        else if (WINDOW_ACTIVE(g_bagWindow2) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                        {
                            BAG_OPEN(item) = 0;
                            UI_WINDOW(g_bagWindow2)->Deactivate();
                            pinstEQ_Bag2_guess->item_guess = 0;
                        }
                    }
                    HANDLE_MATERIAL(g_pc_guess, buttonId, g_pc_guess->inventory_items[0],
                        g_pc_guess->inventory_items[slot]);
                    if (g_soundManager != 0)
                        WAVE_PLAY(0x92, 0);
                    MOVE_ITEM(0, slot, 1, 1);
                    UPDATE_EQUIPMENT(g_local_player_guess, buttonId);
                    UPDATE_LIGHT(g_pc_guess);
                }
            }
            if (g_pc_guess->inventory_items[0] == 0)
                CURSOR_HAS_ITEM = 0;
            else
                CURSOR_HAS_ITEM = 1;
        }
    }
    if (MERCHANT_OPEN == 0)
        goto lootWindow;

merchantBuyInfo:
    if (EQ_GLOBAL(int, 0x006B649C) == 0 && mouseButton == 0 && buttonId >= 0x104 && buttonId <= 0x10f)
    {
        slotPtr = &LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2];
        if (*slotPtr != 0)
        {
            MAIN_INT(0x6c658) = (int)slotPtr;
            MAIN_SHORT(0x6c65c) = buttonId + LIST_PAGE * 2 - 0x104;
            strcpy(itemName, (char*)*slotPtr);
            item = LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2];
            if (item->type == 0)
            {
                if (ITEM_CLASS(item) == 0x27)
                {
                    price = BUY_PRICE(item);
                    rent = RENT_PRICE(LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2]);
                    merchantName = TRIM_NAME(g_merchant->name);
                    sprintf(text, EQ_ARRAY(char, 0x0055ACAC), merchantName, price, itemName, rent);
                }
                else if (COMBINE(item) != 0)
                {
                    price = BUY_PRICE(LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2]);
                    merchantName = TRIM_NAME(g_merchant->name);
                    sprintf(text, EQ_ARRAY(char, 0x0055AC84), merchantName, price, itemName);
                }
                else
                {
                    price = BUY_PRICE(LIST_ITEMS[buttonId - 0x104 + LIST_PAGE * 2]);
                    merchantName = TRIM_NAME(g_merchant->name);
                    sprintf(text, EQ_ARRAY(char, 0x0055AC58), merchantName, price, itemName);
                }
            }
            else
            {
                price = BUY_PRICE(item);
                merchantName = TRIM_NAME(g_merchant->name);
                sprintf(text, EQ_ARRAY(char, 0x0055AC58), merchantName, price, itemName);
            }
            DSP_CHAT(text, 0x113);
        }
    }

lootWindow:
    if (LOOT_OPEN != 0 && MAIN_INT(0xa4) != 0)
    {
        if (CURSOR_COIN != 0)
            return;
        if (g_pc_guess->inventory_items[0] != 0)
            return;
        if (buttonId >= 0x113 && buttonId <= 0x122 && (mouseButton == 0 || mouseButton == 1) &&
            EQ_GLOBAL(int, 0x006B6498) == 0)
        {
            index = buttonId + LIST_PAGE * 2 - 0x113;
            slotPtr = &LIST_ITEMS[index];
            if (*slotPtr != 0)
            {
                canLoot = 1;
                if (g_lootNoDropMode > 2 || g_lootNoDropMode < 0)
                    g_lootNoDropMode = 0;
                loot.corpseId = g_corpse->spawn_id;
                loot.spawnId = g_local_player_guess->spawn_id;
                loot.slot = *(short*)(self + 0x6c8b6 + index * 2);
                loot.rightClick = 0;
                if (mouseButton == 1)
                    loot.rightClick = 1;
                if ((*slotPtr)->nodrop == 0 && g_lootNoDropMode != 2 &&
                    (g_lootNoDropMode == 0 || CAN_USE_ITEM(*slotPtr) == 0))
                    canLoot = 0;
                if (_strcmpi(g_local_player_guess->name, STRIP_NAME(g_corpse->name)) == 0)
                    canLoot = 1;
                if (canLoot != 0)
                {
                    MAIN_INT(0xa4) = 0;
                    MAIN_INT(0x94) += 1;
                    SEND_MESSAGE(0x20a0, &loot, 0x10);
                }
                else
                {
                    EQ_GLOBAL(LootRequest_guess, 0x005921A8) = loot;
                    strcpy(g_messageBox + 0x1b9, EQ_ARRAY(char, 0x0055AC28));
                    *(u8*)(g_messageBox + 0x17c) = 0x16;
                    UI_WINDOW(g_messageBox)->Activate();
                    pinstEQ_Requestor_guess->timeout_guess = DISPLAY_TIME + 20000;
                }
            }
        }
    }

trackWindow:
    MAIN_WINDOW(17).active = 0;
    if (MAIN_WINDOW(17).visible != 0 && mouseButton == 0 && MAIN_INT(0x685f4) != 0)
    {
        if (buttonId == 0x50)
        {
            if (TRACK_SLOTS[MAIN_INT(0x6c4d8)].player != 0)
            {
                g_trackMode = 0x35;
                g_trackPlayer = TRACK_SLOTS[MAIN_INT(0x6c4d8)].player;
                now = EqGetTime();
                EQ_GLOBAL(int, 0x006B677C) = 0;
                EQ_GLOBAL(u32, 0x006B6774) = now;
                EQ_GLOBAL(u32, 0x006B6778) = now + 360000;
                sprintf(text, EQ_ARRAY(char, 0x0055AC10), TRIM_NAME(g_trackPlayer->name));
                DSP_CHAT(text, 0x10e);
            }
            track = TRACK_SLOTS;
            count = 1000;
            do
            {
                track->player = 0;
                track->distance = -1.0f;
                track->value8 = -1;
                track->valueC = 0;
                track++;
                count--;
            } while (count != 0);
        }
        else if (buttonId == 0x51)
        {
            if (g_trackMode == 0x35)
                g_trackMode = -1;
            track = TRACK_SLOTS;
            count = 1000;
            do
            {
                track->player = 0;
                track->distance = -1.0f;
                track->value8 = -1;
                track->valueC = 0;
                track++;
                count--;
            } while (count != 0);
        }
        else
        {
            if (buttonId > 0x51 && buttonId <= 0x65)
                MAIN_INT(0x6c4d8) = MAIN_INT(0x6c340 + buttonId * 4);
            goto modeButtons;
        }
        UI_WINDOW(self)->Deactivate();
        MAIN_MODE = 0;
        UI_WINDOW(self)->Activate();
    }

modeButtons:
    if (MAIN_MODE == 0)
    {
        if (mouseButton != 0)
            goto hotButtonWindow;
        if (EQ_GLOBAL(int, 0x006B649C) != 0 && buttonId >= 0xe6 && buttonId <= 0xeb)
        {
            MAIN_BYTE(0x685ae) = 6;
            heldValue = buttonId - 0xe6;
            goto holdHotButton;
        }
        switch (buttonId)
        {
        case 0xe3:
        showMode1:
            UI_WINDOW(self)->Deactivate();
            MAIN_MODE = 1;
            goto resetSelection;
        case 0xe4:
        showMode2:
            UI_WINDOW(self)->Deactivate();
            MAIN_MODE = 2;
            goto resetSelection;
        case 0xe5:
        showMode3:
            UI_WINDOW(self)->Deactivate();
            MAIN_MODE = 3;
            goto resetSelection;
        case 0xe6:
            if (g_offlineMode_guess == 0)
                EQ_METHOD(1, void, 0x004CA5C7, (void*, char*), g_everquest_guess)(g_emptyString);
            break;
        case 0xe7:
            if (MAIN_BYTE(0xb5) != 0)
                EQ_METHOD(0, void, 0x004CB362, (void*), g_everquest_guess)();
            else
                EQ_METHOD(0, void, 0x004CAF5E, (void*), g_everquest_guess)();
            break;
        case 0xe8:
            EQ_METHOD(0, void, 0x004CB4FA, (void*), g_everquest_guess)();
            break;
        case 0xe9:
            EQ_METHOD(0, void, 0x004CB72D, (void*), g_everquest_guess)();
            break;
        case 0xea:
            EQ_METHOD(0, void, 0x004CB6E6, (void*), g_everquest_guess)();
            break;
        case 0xeb:
            EQ_GLOBAL(char, 0x006B6405) ^= 1;
            break;
        }
        goto hotButtonWindow;
    }
    if (MAIN_MODE == 1)
    {
        if (mouseButton == 0)
        {
            if (EQ_GLOBAL(int, 0x006B649C) != 0 && buttonId >= 0xf0 && buttonId <= 0xf5)
            {
                MAIN_BYTE(0x685ae) = 3;
                MAIN_SHORT(0x685b0) = buttonId - 0xf0;
                goto startHotButtonDrag;
            }
            if (buttonId == 0xe3)
                goto showMode2;
            if (buttonId == 0xe4)
                goto showMode0;
            if (buttonId == 0xe5)
                goto showMode3;
            if (buttonId > 0xef)
            {
                if (buttonId <= 0xf5)
                {
                    if (ABILITY_SLOTS[buttonId - 0xf0] == -1)
                    {
                        UI_WINDOW(self)->Deactivate();
                        MAIN_INT(0x685d8) = 1;
                        MAIN_SHORT(0x6c568) = buttonId - 0xf0;
                        MAIN_MODE = 1;
                        goto activateAndReturn;
                    }
                    USE_ABILITY(*(char*)&ABILITY_SLOTS[buttonId - 0xf0], g_target);
                    return;
                }
                if (buttonId == 0x163)
                {
                    MAIN_INT(0x685d8) = 0;
                    goto redrawMode1;
                }
            }
            if (MAIN_INT(0x685d8) == 0 || buttonId < 0x145 || buttonId > 0x162)
                goto hotButtonWindow;
            MAIN_INT(0x685d8) = 0;
            ABILITY_SLOTS[MAIN_SHORT(0x6c568)] = SKILL_LIST[buttonId - 0x145];
            SAVE_CUSTOM();
        redrawMode1:
            UI_WINDOW(self)->Deactivate();
            MAIN_MODE = 1;
            goto activateAndReturn;
        }
        if (mouseButton != 1 || buttonId < 0xf0 || buttonId > 0xf5)
            goto hotButtonWindow;
        UI_WINDOW(self)->Deactivate();
        MAIN_INT(0x685d8) = 1;
        MAIN_MODE = 1;
        MAIN_SHORT(0x6c568) = buttonId - 0xf0;
        UI_WINDOW(self)->Activate();
        goto hotButtonWindow;
    }
    if (MAIN_MODE == 2)
    {
        if (mouseButton == 0)
        {
            if (EQ_GLOBAL(int, 0x006B649C) != 0)
            {
                if (buttonId == 0xf6 || buttonId == 0xf7)
                {
                    MAIN_BYTE(0x685ae) = 1;
                    heldValue = buttonId - 0xf6;
                    goto holdHotButton;
                }
                if (buttonId >= 0xf8 && buttonId <= 0xfb)
                {
                    MAIN_BYTE(0x685ae) = 2;
                    heldValue = buttonId - 0xf8;
                    goto holdHotButton;
                }
            }
            slot = buttonId;
            if (slot <= 0xf6)
            {
                if (slot == 0xf6)
                    EXECUTE_CMD(8, 1, 0);
                else if (slot == 0xe3)
                    goto showMode1;
                else if (slot == 0xe4)
                    goto showMode0;
                else if (slot == 0xe5)
                    goto showMode3;
            }
            else if (slot == 0xf7)
            {
                if (pinstEQ_Main_guess->primaryAttackReady_guess != 0)
                {
                    pinstEQ_Main_guess->primaryAttackReady_guess = 0;
                    EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, int, EQPlayer*), g_local_player_guess)(
                        0xb, 0, g_target);
                }
            }
            else if (slot <= 0xfb)
            {
                if (COMBAT_SLOTS[slot - 0xf8] == -1)
                {
                    UI_WINDOW(self)->Deactivate();
                    MAIN_MODE = 2;
                    MAIN_SHORT(0x6c568) = buttonId - 0xf8;
                    MAIN_INT(0x685d4) = 1;
                    UI_WINDOW(self)->Activate();
                }
                else
                    USE_ABILITY(*(char*)&COMBAT_SLOTS[slot - 0xf8], g_target);
            }
            else if (slot == 0x163)
            {
                UI_WINDOW(self)->Deactivate();
                MAIN_INT(0x685d4) = 0;
                goto redrawMode2;
            }
            if (MAIN_INT(0x685d4) == 0 || buttonId < 0x145 || buttonId > 0x162)
                goto hotButtonWindow;
            MAIN_INT(0x685d4) = 0;
            COMBAT_SLOTS[MAIN_SHORT(0x6c568)] = SKILL_LIST[slot - 0x145];
            SAVE_CUSTOM();
            UI_WINDOW(self)->Deactivate();
            goto redrawMode2;
        }
        if (mouseButton != 1 || buttonId < 0xf8 || buttonId > 0xfb)
            goto hotButtonWindow;
        UI_WINDOW(self)->Deactivate();
        MAIN_INT(0x685d4) = 1;
        MAIN_SHORT(0x6c568) = buttonId - 0xf8;
    redrawMode2:
        MAIN_MODE = 2;
        goto activateAndReturn;
    }
    if (MAIN_MODE != 3)
        goto hotButtonWindow;
    if (mouseButton != 0)
        goto pickSpellSet;
    if (EQ_GLOBAL(int, 0x006B649C) != 0 && buttonId >= 0x39 && buttonId <= 0x44)
    {
        MAIN_BYTE(0x685ae) = 4;
        heldValue = buttonId + SPELLSET_PAGE * 12 - 0x39;
        goto holdHotButton;
    }
    switch (buttonId)
    {
    case 0x39:
    case 0x3a:
    case 0x3b:
    case 0x3c:
    case 0x3d:
    case 0x3e:
    case 0x3f:
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
        if (EQ_GLOBAL(int, 0x0053F230) == -1)
            EQ_GLOBAL(int, 0x0053F230) = (buttonId + SPELLSET_PAGE * 12) * 8 - 0x1c8;
        break;
    case 0x45:
        MAIN_BYTE(0x5f195) = 5;
        EQ_GLOBAL(int, 0x006EFDC8) = 0;
        EQ_GLOBAL(int, 0x006EFDCC) = 0;
        strcpy(self + 0x682e6, g_emptyString);
        break;
    case 0x46:
        MAIN_BYTE(0x6841b) += 1;
        if (MAIN_BYTE(0x6841b) > 0x13)
            MAIN_BYTE(0x6841b) = 0;
        break;
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
        MAIN_BYTE(0x5f195) = (char)buttonId - 0x47;
        EQ_GLOBAL(int, 0x006EFDC8) = 0;
        EQ_GLOBAL(int, 0x006EFDCC) = 0;
        strcpy(self + 0x682ef + MAIN_BYTE(0x5f195) * 0x3c, g_emptyString);
        break;
    case 0x4c:
        MAIN_INT(0x5f190) = 0;
        MAIN_BYTE(0x5f195) = -1;
        MAIN_INT(0x5f18c) = 0;
        UI_WINDOW(self)->Deactivate();
        MAIN_MODE = 3;
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f186) = -1;
        MAIN_SHORT(0x5f18a) = -1;
        goto spellSetChanged;
    case 0x4d:
        setIndex = MAIN_BYTE(0x5f194);
        memcpy(SPELL_SET(setIndex), self + 0x682e6, 0x136);
        MAIN_INT(0x5f190) = 0;
        MAIN_INT(0x5f18c) = 0;
        MAIN_BYTE(0x5f195) = -1;
        SAVE_CUSTOM();
        UI_WINDOW(self)->Deactivate();
        MAIN_SHORT(0x5f186) = -1;
        MAIN_MODE = 3;
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f18a) = -1;
    spellSetChanged:
        g_chatInputInactive_guess = 1;
        UI_WINDOW(self)->Activate();
        break;
    case 0x4e:
        SPELLSET_PAGE -= 1;
        if (SPELLSET_PAGE >= 0)
            break;
        SPELLSET_PAGE = 9;
        goto pickSpellSet;
    case 0x4f:
        SPELLSET_PAGE += 1;
        if (SPELLSET_PAGE > 9)
            SPELLSET_PAGE = 0;
        break;
    case 0xe3:
        MAIN_INT(0x5f190) = 0;
        MAIN_BYTE(0x5f195) = -1;
        MAIN_INT(0x5f18c) = 0;
        goto showMode1;
    case 0xe4:
        MAIN_INT(0x5f190) = 0;
        MAIN_BYTE(0x5f195) = -1;
        MAIN_INT(0x5f18c) = 0;
        goto showMode2;
    case 0xe5:
        MAIN_INT(0x5f190) = 0;
        MAIN_BYTE(0x5f195) = -1;
        MAIN_INT(0x5f18c) = 0;
        goto showMode0;
    }
    goto hotButtonWindow;

pickSpellSet:
    if (mouseButton != 1 || buttonId < 0x39 || buttonId > 0x44)
        goto hotButtonWindow;
    g_chatInputInactive_guess = 0;
    *((char*)g_everquest_guess + 0x13ec) = 1;
    setIndex = SPELLSET_PAGE * 12 + (char)buttonId - 0x39;
    MAIN_BYTE(0x5f194) = setIndex;
    memcpy(self + 0x682e6, SPELL_SET(setIndex), 0x136);
    MAIN_BYTE(0x5f195) = -1;
    UI_WINDOW(self)->Deactivate();
    MAIN_MODE = 3;
    MAIN_SHORT(0x5f188) = -1;
    MAIN_SHORT(0x5f186) = -1;
    MAIN_SHORT(0x5f18a) = -1;
    MAIN_INT(0x5f18c) = 1;
    goto activateAndReturn;

showMode0:
    UI_WINDOW(self)->Deactivate();
    MAIN_MODE = 0;
resetSelection:
    MAIN_SHORT(0x5f188) = -1;
    MAIN_SHORT(0x5f186) = -1;
    MAIN_SHORT(0x5f18a) = -1;
activateAndReturn:
    UI_WINDOW(self)->Activate();
    return;

hotButtonWindow:
    MAIN_WINDOW(4).active = 0;
    if (MAIN_WINDOW(4).visible == 0)
        goto spellsWindow;
    if (mouseButton == 1)
    {
        if (buttonId < 200)
            goto spellsWindow;
        if (buttonId > 0xd1)
            goto hotButtonAction;
        if (EQ_GLOBAL(int, 0x006B6498) == 0 && HOTBUTTON(buttonId).type == 5)
        {
            item = g_pc_guess->inventory_items[HOTBUTTON(buttonId).value];
            if (item != 0 && item->type == 1)
            {
                if (BAG_OPEN(item) == 1)
                {
                    if (IS_ACTIVE(g_bagWindow1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow1)->Deactivate();
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (IS_ACTIVE(g_bagWindow2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                    {
                        BAG_OPEN(item) = 0;
                        UI_WINDOW(g_bagWindow2)->Deactivate();
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                }
                else if (IS_ACTIVE(g_bagWindow1) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag1_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow1)->Open(HOTBUTTON(buttonId).value - 0x16);
                    pinstEQ_Bag1_guess->slot_guess = HOTBUTTON(buttonId).value;
                }
                else if (IS_ACTIVE(g_bagWindow2) == 0)
                {
                    BAG_OPEN(item) = 1;
                    pinstEQ_Bag2_guess->item_guess = item;
                    UI_WINDOW(g_bagWindow2)->Open(HOTBUTTON(buttonId).value - 0x16);
                    pinstEQ_Bag2_guess->slot_guess = HOTBUTTON(buttonId).value;
                }
            }
        }
        if (buttonId <= 0xd1 && HOTBUTTON(buttonId).type == 5)
        {
            item = g_pc_guess->inventory_items[HOTBUTTON(buttonId).value];
            if (item != 0 && item->type == 0 && item != 0)
            {
                RIGHT_CLICK_ITEM(item, item, HOTBUTTON(buttonId).value);
                goto hotButtonLeftClick;
            }
        }
        goto hotButtonAction;
    }
hotButtonLeftClick:
    if (mouseButton == 0)
    {
        if (buttonId < 200)
            goto spellsWindow;
        if (buttonId > 0xd1)
            goto useHotButton;
        if (EQ_GLOBAL(int, 0x006B649C) != 0)
        {
            if (HOTBUTTON(buttonId).type == 0)
                return;
            MAIN_BYTE(0x685ae) = HOTBUTTON(buttonId).type;
            MAIN_SHORT(0x685b0) = HOTBUTTON(buttonId).value;
            HOTBUTTON_HELD = 1;
            HOTBUTTON(buttonId).type = 0;
            HOTBUTTON(buttonId).value = 0;
            return;
        }
        if (HOTBUTTON_HELD == 0)
            goto useHotButton;
        oldValue = HOTBUTTON(buttonId).value;
        oldType = HOTBUTTON(buttonId).type;
        HOTBUTTON(buttonId).type = MAIN_BYTE(0x685ae);
        HOTBUTTON(buttonId).value = MAIN_SHORT(0x685b0);
        MAIN_BYTE(0x685ae) = oldType;
        MAIN_SHORT(0x685b0) = oldValue;
        if (oldType == 0)
            HOTBUTTON_HELD = 0;
        goto saveAndReturn;
    }
    if (mouseButton != 1)
        goto afterHotButtonCast;
hotButtonAction:
    if (buttonId < 200 || buttonId > 0xd1)
        goto spellsWindow;
    slot = buttonId;
    hotButtonType = HOTBUTTON(slot).type;
    if (hotButtonType == 2)
    {
        UI_WINDOW(self)->Deactivate();
        MAIN_INT(0x685d4) = 1;
        heldValue = *(char*)&HOTBUTTON(slot).value;
        MAIN_SHORT(0x5f186) = -1;
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f18a) = -1;
        MAIN_SHORT(0x6c568) = heldValue;
        MAIN_MODE = 2;
        UI_WINDOW(self)->Activate();
    }
    else if (hotButtonType == 3)
    {
        UI_WINDOW(self)->Deactivate();
        MAIN_INT(0x685d8) = 1;
        heldValue = *(char*)&HOTBUTTON(slot).value;
        MAIN_SHORT(0x5f186) = -1;
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f18a) = -1;
        MAIN_SHORT(0x6c568) = heldValue;
        MAIN_MODE = 1;
        UI_WINDOW(self)->Activate();
    }
    else if (hotButtonType == 4)
    {
        UI_WINDOW(self)->Deactivate();
        g_chatInputInactive_guess = 0;
        setIndex = *(char*)&HOTBUTTON(slot).value;
        MAIN_BYTE(0x5f194) = setIndex;
        memcpy(self + 0x682e6, SPELL_SET(setIndex), 0x136);
        MAIN_BYTE(0x5f195) = -1;
        UI_WINDOW(self)->Deactivate();
        MAIN_SHORT(0x5f186) = -1;
        MAIN_MODE = 3;
        MAIN_SHORT(0x5f188) = -1;
        MAIN_SHORT(0x5f18a) = -1;
        MAIN_INT(0x5f18c) = 1;
        *((char*)g_everquest_guess + 0x13ec) = 1;
        UI_WINDOW(self)->Activate();
    }
    else
    {
        if (hotButtonType != 7)
            goto spellsWindow;
        if (EQ_GLOBAL(int, 0x006B6498) != 0)
            goto spellsWindow;
        if (g_pc_guess->spell_memorized[HOTBUTTON(slot).value] == -1)
            goto spellsWindow;
        if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
            goto spellsWindow;
        pinstEQ_Main_guess->pendingWorldRequests_guess = 1;
        cast.gem = HOTBUTTON(slot).value;
        cast.spell = g_pc_guess->spell_memorized[HOTBUTTON(slot).value];
        cast.mode = 2;
        SEND_MESSAGE(0x2182, &cast, 0xc);
    afterHotButtonCast:
        if (mouseButton != 0)
            goto spellsWindow;
    useHotButton:
        if (buttonId < 200 || buttonId > 0xd1)
            goto spellsWindow;
        if (HOTBUTTON_HELD != 0)
        {
            HOTBUTTON(buttonId).type = MAIN_BYTE(0x685ae);
            HOTBUTTON(buttonId).value = MAIN_SHORT(0x685b0);
            HOTBUTTON_HELD = 0;
            MAIN_BYTE(0x685ae) = 0;
            MAIN_SHORT(0x685b0) = -1;
        saveAndReturn:
            SAVE_CUSTOM();
            return;
        }
        EQ_METHOD(2, void, 0x0047E1E3, (char*, short, int), self)(buttonId, 1);
        return;
    }

spellsWindow:
    MAIN_WINDOW(3).active = 0;
    if (MAIN_WINDOW(3).visible != 0)
    {
        if (mouseButton == 1)
        {
            if (EQ_GLOBAL(int, 0x006B6498) != 0)
                return;
            if (buttonId < 0xd4)
                return;
            if (buttonId > 0xdb)
                return;
            if (g_pc_guess->spell_memorized[buttonId - 0xd4] == -1)
                return;
            if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                return;
            pinstEQ_Main_guess->pendingWorldRequests_guess = 1;
            cast.gem = buttonId - 0xd4;
            cast.spell = g_pc_guess->spell_memorized[buttonId - 0xd4];
            cast.mode = 2;
            SEND_MESSAGE(0x2182, &cast, 0xc);
            return;
        }
        if (mouseButton != 0)
            return;
        if (buttonId >= 0xd4 && buttonId <= 0xdb)
        {
            if (EQ_GLOBAL(int, 0x006B649C) != 0)
            {
                MAIN_BYTE(0x685ae) = 7;
                MAIN_SHORT(0x685b0) = buttonId - 0xd4;
                goto startHotButtonDrag;
            }
            if (GEM_STATE[buttonId - 0xd4] == 0 &&
                g_local_player_guess->actor_info->unknown0x248 == -1)
            {
                spell = g_pc_guess->spell_memorized[buttonId - 0xd4];
                if (spell == -1)
                    return;
                if (MAIN_SHORT(0xfe8) != 0)
                    return;
                if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                    return;
                if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                    return;
                if (CAST_SPELL((u8)(buttonId + 0x2c), spell, 0, -1) == 0)
                    return;
                gemState = GEM_STATE;
                count = 8;
                do
                {
                    if (*gemState == 0)
                        *gemState = 1;
                    gemState[8] = 0;
                    gemState++;
                    count--;
                } while (count != 0);
                return;
            }
            if (EQ_METHOD(0, int, 0x0046A72F, (char*), self)() != 0)
            {
                STOP_SPELL_CAST(0);
                return;
            }
            DSP_CHAT(EQ_ARRAY(char, 0x0055ABF0), 0x108);
            return;
        }
    }
    else if (mouseButton != 0)
        return;
    if (buttonId == 0xd2)
    {
        MAIN_WINDOW(4).active = 0;
        if (MAIN_WINDOW(4).visible != 0)
        {
            HOTBUTTON_PAGE -= 1;
            if (HOTBUTTON_PAGE < 0)
                HOTBUTTON_PAGE = 9;
        }
    }
    else if (buttonId == 0xd3)
    {
        MAIN_WINDOW(4).active = 0;
        if (MAIN_WINDOW(4).visible != 0)
        {
            HOTBUTTON_PAGE += 1;
            if (HOTBUTTON_PAGE > 9)
                HOTBUTTON_PAGE = 0;
        }
    }
    else if (buttonId == 0xfc)
    {
        if (g_local_player_guess != 0)
        {
            MAIN_WINDOW(0).active = 0;
            if (MAIN_WINDOW(0).visible != 0)
                g_target = g_local_player_guess;
        }
    }
    else if (buttonId > 0xfd && buttonId <= 0x102)
    {
        MAIN_WINDOW(1).active = 0;
        if (MAIN_WINDOW(1).visible != 0 && g_groupNames[(buttonId - 0xfe) * 0x1e] != 0)
        {
            member = g_groupMembers[buttonId - 0xfe];
            if (member != 0)
                g_target = member;
            MAIN_BYTE(0xb4) = (char)buttonId + 2;
        }
    }
}
#undef MAIN_INT
#undef MAIN_SHORT
#undef MAIN_BYTE
#undef MAIN_WINDOW
#undef MAIN_MODE
#undef LIST_PAGE
#undef HOTBUTTON_PAGE
#undef SPELLSET_PAGE
#undef HOTBUTTON
#undef LOOT_OPEN
#undef MERCHANT_OPEN
#undef TRADE_OPEN
#undef BANK_OPEN
#undef TRADE_ACCEPTED
#undef TRADE_OTHER_ACCEPTED
#undef HOUSE_BANK
#undef HOUSE_ITEMS
#undef HOUSE_COINS
#undef LIST_ITEMS
#undef TRADE_THEIR_ITEMS
#undef TRADE_MY_ITEMS
#undef TRADE_COINS
#undef ABILITY_SLOTS
#undef COMBAT_SLOTS
#undef SKILL_LIST
#undef GEM_STATE
#undef TRACK_SLOTS
#undef SPELL_SET
#undef UI_WINDOW
#undef WINDOW_INT
#undef WINDOW_ACTIVE
#undef WINDOW_ITEM
#undef WINDOW_SLOT
#undef BOOK_ITEM
#undef BAG_CONTENTS
#undef BOOK_FILE
#undef ITEM_CLASS
#undef BAG_SLOT_COUNT
#undef BAG_OPEN
#undef ITEM_DETAIL
#undef CURSOR_HAS_ITEM
#undef CURSOR_COIN
#undef HOTBUTTON_HELD
#undef DISPLAY_TIME
#undef NPC_ID
#undef g_screenWidth
#undef g_bagWindow1
#undef g_bagWindow2
#undef g_bookWindow
#undef g_messageBox
#undef g_itemDisplay
#undef g_moneySlider
#undef g_quantitySlider
#undef g_trader
#undef g_merchant
#undef g_corpse
#undef g_target
#undef g_soundManager
#undef g_trackMode
#undef g_trackPlayer
#undef g_lootNoDropMode
#undef g_groupNames
#undef g_groupMembers
#undef g_emptyString
#undef strcpy
#undef sprintf
#undef memcpy
#undef strlen
#undef _strcmpi
#undef operator delete
#undef COMBINE
#undef CAN_DROP
#undef SWAP_COMBINE
#undef AUTO_INVENTORY
#undef HANDLE_MATERIAL
#undef UPDATE_LIGHT
#undef CAN_EQUIP_IN_SLOT
#undef EXECUTE_CMD
#undef SEND_MESSAGE
#undef IS_ACTIVE
#undef WRITE_WINDOW_VALUE
#undef SAVE_CUSTOM
#undef RIGHT_CLICK_ITEM
#undef DSP_CHAT
#undef TRIM_NAME
#undef STRIP_NAME
#undef MOVE_ITEM
#undef MONEY_SLIDER
#undef QUANTITY_SLIDER
#undef WAVE_PLAY
#undef CHANGE_VIDEO_MODE
#undef UPDATE_EQUIPMENT
#undef USE_ABILITY
#undef CAN_USE_ITEM
#undef CAST_SPELL
#undef STOP_SPELL_CAST
#undef SELL_PRICE
#undef BUY_PRICE
#undef RENT_PRICE
#undef CLOSE_BAG

typedef void (__fastcall *tProcessButtonVirtual0_guess)(void* object);
typedef void (__fastcall *tProcessButtonVirtual1_guess)(void* object, int unused, int argument);
typedef int (__stdcall *tWriteProfileString_guess)(const char* section, const char* key, const char* value, const char* file);
typedef int (__stdcall *tCopyRect_guess)(RECT* destination, const RECT* source);

struct CoinMove_pb_guess
{
    int fromSlot;
    int toSlot;
    int fromType;
    int toType;
    int amount;
};

struct BankItemMsg_guess
{
    u32 spawnId;
    int bankerId;
    u8 data[10];
    short itemNr;
    short slot;
    u16 itemType;
    int cost;
};

struct LootItemMsg_guess
{
    u32 corpseId;
    u32 playerId;
    u16 slot;
    u16 unused;
    int rightClick;
};

struct BoardPost_guess
{
    u32 spawnId;
    char date[16];
    char author[32];
    u16 language;
    char subject[32];
    short board;
    char body[1024];
};

struct SpawnAppearanceMsg_guess
{
    u32 spawnId;
    u16 type;
    u16 unused;
    int value;
};

struct BoardRequestMsg_guess
{
    u32 spawnId;
    char board;
};

struct BoardMessageMsg_guess
{
    u32 spawnId;
    u32 messageId;
    char board;
};

struct BoardDeleteMsg_guess
{
    u32 spawnId;
    u32 messageId;
    int board;
};

struct TradeAcceptMsg_guess
{
    u32 targetId;
    char accept;
};

#ifdef EQD_MATCH
extern char eqd_va_0x005921A8;
#endif

#define PB_CHAR(off) (*(char*)(self + (off)))
#define PB_SHORT(off) (*(short*)(self + (off)))
#define PB_INT(off) (*(int*)(self + (off)))
#define PB_MODE PB_CHAR(0xfe0)
#define PB_PREV_MODE PB_CHAR(0xfe1)
#define PB_PRESSED PB_SHORT(0x5f188)
#define PB_DRAG_TYPE PB_CHAR(0x685ae)
#define PB_DRAG_VALUE PB_SHORT(0x685b0)
#define PB_HOT_PAGE PB_CHAR(0x6841d)
#define PB_HOT_TYPE(index) PB_CHAR(0x680fe + (index) * 4)
#define PB_HOT_VALUE(index) PB_SHORT(0x68100 + (index) * 4)
#define PB_OPTIONS_PAGE PB_CHAR(0x8fbac)
#define PB_EDIT_LAYOUT PB_INT(0x6c8f8)
#define PB_LOOT_OPEN PB_INT(0x685dc)
#define PB_MERCHANT_OPEN PB_INT(0x685e0)
#define PB_TRADE_OPEN PB_INT(0x685e4)
#define PB_BANK_OPEN PB_INT(0x685e8)
#define PB_HOUSE_BANK PB_CHAR(0x101)
#define PB_LIST_PAGE PB_CHAR(0x6c592)
#define PB_CURSOR_SPELL PB_SHORT(0xfe8)
#define PB_BOOK_PAGE PB_SHORT(0x6c560)
#define PB_MEMORIZING PB_SHORT(0x1004)
#define PB_SCRIBING PB_SHORT(0x1006)
#define PB_VIDEO_MODE EQ_GLOBAL(int, 0x0058FC6C)
#define PB_VIDEO_RESTORE EQ_GLOBAL(int, 0x00590170)
#define PB_HOTKEY_EDIT EQ_GLOBAL(int, 0x006B649C)
#define PB_INSPECT EQ_GLOBAL(int, 0x006B6498)
#define PB_DISPLAY EQ_GLOBAL(char*, 0x006E4DE0)
#define PB_MAIN EQ_GLOBAL(char*, 0x006E4E44)
#define PB_EVERQUEST EQ_GLOBAL(void*, 0x006EFE2C)
#define PB_ITEM_DISPLAY EQ_GLOBAL(char*, 0x006E4D84)
#define PB_BAG1 EQ_GLOBAL(char*, 0x006E4E58)
#define PB_BAG2 EQ_GLOBAL(char*, 0x006E4E5C)
#define PB_BOOK EQ_GLOBAL(char*, 0x006E4E60)
#define PB_CONFIRM EQ_GLOBAL(char*, 0x006E4E20)
#define PB_TARGET EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define PB_MERCHANT EQ_GLOBAL(EQPlayer*, 0x006E4DB8)
#define PB_CORPSE EQ_GLOBAL(EQPlayer*, 0x006E4DC0)
#define PB_TRAINER EQ_GLOBAL(EQPlayer*, 0x006E4DC4)
#define PB_TRADE_TARGET EQ_GLOBAL(EQPlayer*, 0x006E4D88)
#define PB_BANKER_ID (*(short*)(EQ_GLOBAL(char*, 0x006E4DCC) + 0x62))
#define PB_SOUND EQ_GLOBAL(void*, 0x006E4DD8)
#define PB_MUSIC EQ_GLOBAL(void*, 0x006ECDF4)
#define PB_SOUND_OFF EQ_GLOBAL(int, 0x006EFE1C)
#define PB_COIN_SLIDER EQ_GLOBAL(char*, 0x00604984)
#define PB_QUANTITY_SLIDER EQ_GLOBAL(char*, 0x006018C0)
#define PB_BOARD_LIST EQ_GLOBAL(char*, 0x006E4D70)
#define PB_VIRTUAL(object, offset) (*(tProcessButtonVirtual0_guess*)(*(char**)(object) + (offset)))(object)
#define PB_VIRTUAL_ARG(object, offset, argument) \
    (*(tProcessButtonVirtual1_guess*)(*(char**)(object) + (offset)))(object, 0, argument)
#define PB_SEND(opcode, data, size) \
    send_message_guess(EQ_ARRAY(void*, 0x006B3194), (opcode), (data), (size), 1)
#define PB_DSP_CHAT(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), PB_EVERQUEST)((text), (color))
#define PB_PRINT_CHAT(text) EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), PB_EVERQUEST)(text)
#define PB_TRIM_NAME(name) EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), PB_EVERQUEST)(name)
#define PB_CHANGE_VIDEO_MODE() EQ_METHOD(1, void, 0x0040E58B, (char*, int), PB_DISPLAY)(0)
#define PB_SAVE_CUSTOM() EQ_METHOD(0, void, 0x004567F8, (char*), self)()
#define PB_SAVE_OPTIONS() EQ_METHOD(0, void, 0x004CF31A, (void*), PB_EVERQUEST)()
#define PB_SAVE_OPTIONS2() EQ_METHOD(0, void, 0x004CF3E1, (void*), PB_EVERQUEST)()
#define PB_IS_ACTIVE(window) EQ_METHOD(0, int, 0x004504AC, (char*), window)()
#define PB_COMBINE(stack) EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(stack)
#define PB_MOVE_ITEM(from, to, a, b) \
    EQ_METHOD(4, int, 0x00463651, (char*, int, int, int, int), PB_MAIN)((from), (to), (a), (b))
#define PB_WAVE_PLAY() EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), PB_SOUND)(0x92, 0)
#define PB_UPDATE_LIGHT() EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess)
#define PB_QUANTITY_ACTIVATE(slotPointer, value) \
    EQ_METHOD(2, void, 0x00486CB6, (char*, EQ_Item**, int), PB_QUANTITY_SLIDER)((slotPointer), (value))
#define PB_COIN_ACTIVATE(coins, type, source) \
    EQ_METHOD(3, void, 0x0048629A, (char*, s32*, int, int), PB_COIN_SLIDER)((coins), (type), (source))
#define PB_BAG_OPEN(bagItem) (*(char*)((char*)(bagItem) + 0xd6))
#define PB_BAG_ITEM(bag) (*(EQ_Item**)((bag) + 0x174))
#define PB_SPELL_TABLE EQ_ARRAY(char*, 0x005FE9A0)
#define PB_CLASS_LEVEL(spellData) (*(u8*)((spellData) + g_pc_guess->class_ + 0x234))
#define PB_FLOAT_GAMMA EQ_GLOBAL(float, 0x006B685C)
#define PB_FLOAT_SCALE EQ_GLOBAL(float, 0x006B6EF0)
#define PB_CURRENT_VIDEO EQ_GLOBAL(int, 0x006E20C0)

// FUNCTION: TRILOGY 0x0045cb52
// __fastcall on EQ_Main; handles one click, with mouseButton and button stored at this+0x5f184/0x5f186.
// Video mode 2 forwards to 0x00457cd6; layout-edit and hot-key edit state (this+0x6c8f8, 0x006b649c) is checked.
// Screens 0-3 and 5 first handle hot-button and spell-gem clicks, then a switch on the button id (options pages
// via this+0x8fbac, spellbook flips, save options).
// At label screens the byte at this+0xfe0 selects a block per screen: social, inventory with bank, trade, loot
// and merchant windows, spellbook memorize/scribe, boards; blocks send packets with send_message_guess.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_ProcessButton_guess(char* self, int unused, short mouseButton, short button)
{
    struct CoinMove_pb_guess coinMove;
    struct BankItemMsg_guess bankMsg;
    struct LootItemMsg_guess lootMsg;
    struct BoardPost_guess post;
    struct SpawnAppearanceMsg_guess appearance;
    struct BoardRequestMsg_guess boardRequest;
    struct BoardMessageMsg_guess boardMessage;
    struct BoardDeleteMsg_guess boardDelete;
    struct TradeAcceptMsg_guess tradeAccept;
    u32 pairPacket[2];
    int gemPacket[3];
    int destroyPacket[3];
    int houseCoins[4];
    u32 trainerPacket[23];
    char nameCopy[32];
    char message[128];
    char videoModeText[16];
    EQ_Item* item;
    EQ_Item** slot;
    char* entry;
    char* spellData;
    char* text;
    int index;
    int slotOffset;
    int allowed;
    int oldVideoMode;
    short spellSlot;
    short spellId;
    short found;
    short counter;
    short freeSlot;
    short thumb;
    short oldValue;
    char oldType;
    char optionsPage;
    char mode;
    u8 needed;

    PB_SHORT(0xec2) = 0;
    PB_INT(0xeb8) = 1;
    PB_SHORT(0x5f184) = mouseButton;
    PB_SHORT(0x5f186) = button;
    if (PB_VIDEO_MODE == 2)
    {
        EQ_METHOD(2, void, 0x00457CD6, (char*, short, short), self)(mouseButton, button);
        return;
    }
    if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0 && mouseButton == 0)
    {
        PB_PRESSED = button;
        PB_SHORT(0x5f18a) = -1;
        if (button == 0x6c)
        {
            PB_INT(0x5f180) = 1;
            return;
        }
    }
    if (PB_MODE > 3 && PB_MODE != 5)
        goto screens;
    if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
    {
        if (mouseButton == 0)
            goto setPressed;
        if (PB_INSPECT == 0)
            return;
        if (mouseButton != 1)
            return;
        if (button >= 200 && button <= 0xd1)
        {
            oldType = PB_HOT_TYPE(button + PB_HOT_PAGE * 10);
            if (oldType == 5)
            {
                item = g_pc_guess->inventory_items[PB_HOT_VALUE(button + PB_HOT_PAGE * 10)];
                if (item != 0)
                {
                    *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                    PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                }
            }
            else if (oldType == 7)
            {
                spellId = g_pc_guess->spell_memorized[PB_HOT_VALUE(button + PB_HOT_PAGE * 10)];
                if (spellId != -1)
                {
                    *(short*)(PB_ITEM_DISPLAY + 0x94) = spellId;
                    PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                }
            }
        }
        if (button >= 0xd4 && button <= 0xdb)
        {
            spellId = g_pc_guess->spell_memorized[button - 0xd4];
            if (spellId != -1)
            {
                *(short*)(PB_ITEM_DISPLAY + 0x94) = spellId;
                PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
            }
        }
        if (PB_MODE == 5 && PB_INSPECT != 0)
        {
            if (button >= 0x16 && button <= 0x1d)
            {
                item = g_pc_guess->inventory_items[button];
                if (item != 0)
                {
                    *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                    PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                }
            }
            if (PB_INSPECT != 0 && button >= 0x2b && button <= 0x34)
            {
                spellSlot = (short)(PB_BOOK_PAGE * 10 + button - 0x2b);
                PB_SHORT(0xfea) = spellSlot;
                if (spellSlot <= 0xfa)
                {
                    spellId = g_pc_guess->spell_book[spellSlot];
                    if (spellId != -1)
                    {
                        *(short*)(PB_ITEM_DISPLAY + 0x94) = spellId;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
            }
        }
        PB_PRESSED = button;
        goto clearRelease;
    }
    if (mouseButton == 1)
    {
        if (button >= 200 && button <= 0xd1)
        {
            EQ_METHOD(1, void, 0x0047DD6C, (char*, short), self)(button);
            goto afterHotButtonRightClick;
        }
rightClickGem:
        if (PB_INSPECT == 0 && button >= 0xd4 && button <= 0xdb &&
            g_pc_guess->spell_memorized[button - 0xd4] != -1 && pinstEQ_Main_guess->pendingWorldRequests_guess == 0)
        {
            pinstEQ_Main_guess->pendingWorldRequests_guess = 1;
            gemPacket[0] = button - 0xd4;
            gemPacket[1] = g_pc_guess->spell_memorized[button - 0xd4];
            gemPacket[2] = 2;
            PB_SEND(0x2182, gemPacket, 0xc);
            goto afterGem;
        }
        goto optionsRightClick;
    }
afterHotButtonRightClick:
    if (mouseButton != 0)
    {
        if (mouseButton == 1)
            goto rightClickGem;
        goto afterGem;
    }
    if (PB_HOTKEY_EDIT != 0)
    {
        switch (button)
        {
        case 0xdc:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 6;
            goto dragArmed;
        case 0xdd:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 7;
            goto dragArmed;
        case 0xde:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 9;
            goto dragArmed;
        case 0xdf:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 10;
            goto dragArmed;
        case 0xe0:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 8;
            goto dragArmed;
        case 0xe1:
            PB_DRAG_TYPE = 6;
            PB_DRAG_VALUE = 11;
            goto dragArmed;
        }
    }
    if (button < 200 || button > 0xd1)
        goto hotkeyGemCheck;
    if (PB_HOTKEY_EDIT != 0)
    {
        oldType = PB_HOT_TYPE(button + PB_HOT_PAGE * 10);
        if (oldType == 0)
            return;
        PB_DRAG_TYPE = oldType;
        PB_DRAG_VALUE = PB_HOT_VALUE(button + PB_HOT_PAGE * 10);
        g_cdisplay_guess->dragArmed_guess = 1;
        PB_HOT_TYPE(button + PB_HOT_PAGE * 10) = 0;
        PB_HOT_VALUE(button + PB_HOT_PAGE * 10) = 0;
        return;
    }
    if (g_cdisplay_guess->dragArmed_guess != 0)
    {
        oldValue = PB_HOT_VALUE(button + PB_HOT_PAGE * 10);
        oldType = PB_HOT_TYPE(button + PB_HOT_PAGE * 10);
        PB_HOT_TYPE(button + PB_HOT_PAGE * 10) = PB_DRAG_TYPE;
        PB_HOT_VALUE(button + PB_HOT_PAGE * 10) = PB_DRAG_VALUE;
        PB_DRAG_TYPE = oldType;
        PB_DRAG_VALUE = oldValue;
        if (PB_DRAG_TYPE == 0)
            g_cdisplay_guess->dragArmed_guess = 0;
        PB_SAVE_CUSTOM();
        return;
    }
    goto buttonSwitch;
afterGem:
    if (mouseButton != 0)
        goto afterFilters;
hotkeyGemCheck:
    if (PB_HOTKEY_EDIT != 0 && button >= 0xd4 && button <= 0xdb)
        goto dragSpellGem;
buttonSwitch:
    switch (button)
    {
    case 200:
    case 201:
    case 202:
    case 203:
    case 204:
    case 205:
    case 206:
    case 207:
    case 208:
    case 209:
        if (g_cdisplay_guess->dragArmed_guess == 0)
        {
            EQ_METHOD(2, void, 0x0047E1E3, (char*, short, int), self)(button, 1);
        }
        else
        {
            PB_HOT_TYPE(button + PB_HOT_PAGE * 10) = PB_DRAG_TYPE;
            PB_HOT_VALUE(button + PB_HOT_PAGE * 10) = PB_DRAG_VALUE;
            g_cdisplay_guess->dragArmed_guess = 0;
            PB_DRAG_TYPE = 0;
            PB_DRAG_VALUE = -1;
            PB_SAVE_CUSTOM();
        }
        break;
    case 0xd2:
        if (--PB_HOT_PAGE < 0)
            PB_HOT_PAGE = 9;
        break;
    case 0xd3:
        if (++PB_HOT_PAGE > 9)
            PB_HOT_PAGE = 0;
        break;
    case 0xd4:
    case 0xd5:
    case 0xd6:
    case 0xd7:
    case 0xd8:
    case 0xd9:
    case 0xda:
    case 0xdb:
        if (PB_INT(0xb10 + button * 4) != 0 ||
            g_local_player_guess->actor_info->unknown0x248 != -1)
        {
            if (EQ_METHOD(0, int, 0x0046A72F, (char*), self)() == 0)
                PB_DSP_CHAT(EQ_ARRAY(char, 0x0055ABF0), 0x108);
            else
                EQ_METHOD(1, void, 0x0042798F, (CharacterProfile*, int), g_pc_guess)(0);
            break;
        }
        spellId = g_pc_guess->spell_memorized[button - 0xd4];
        if (spellId == -1 || PB_CURSOR_SPELL != 0 || pinstEQ_Main_guess->pendingWorldRequests_guess != 0 ||
            pinstEQ_Main_guess->pendingServerReplies_guess != 0)
            break;
        if (EQ_METHOD(4, char, 0x0041F381, (CharacterProfile*, char, short, int, int), g_pc_guess)(
                (char)(button + 0x2c), spellId, 0, -1) == 0)
            break;
        for (index = 0; index < 8; index++)
        {
            if (PB_INT(0xe60 + index * 4) == 0)
                PB_INT(0xe60 + index * 4) = 1;
            PB_INT(0xe80 + index * 4) = 0;
        }
        break;
    case 0xdc:
        if (PB_VIDEO_MODE == 2)
        {
            PB_VIDEO_RESTORE = 2;
            PB_VIDEO_MODE = 1;
            PB_CHANGE_VIDEO_MODE();
        }
        PB_PREV_MODE = PB_MODE;
        PB_VIRTUAL(self, 8);
        PB_MODE = 0xc;
        goto activateReturn;
    case 0xdd:
        if (pinstEQ_Main_guess->screenMode_guess >= 4)
        {
            PB_DSP_CHAT(EQ_ARRAY(char, 0x0055B330), 0x111);
            return;
        }
        PB_VIRTUAL(self, 8);
optionsMain:
        PB_OPTIONS_PAGE = 1;
        goto activateReturn;
    case 0xde:
        if (PB_MODE == 5)
        {
            PB_INT(0x6c554) = 0;
            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_control_player_guess)(100);
            if (PB_VIDEO_MODE == 2 && (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
            {
                PB_VIDEO_RESTORE = 2;
                PB_VIDEO_MODE = 1;
                PB_CHANGE_VIDEO_MODE();
            }
            PB_VIRTUAL(self, 8);
            PB_CURSOR_SPELL = 0;
            if (g_pc_guess->inventory_items[0] == 0)
                g_cdisplay_guess->itemOnCursor_guess = 0;
            PB_INT(0xfe4) = 0;
            PB_CHAR(0xfe2) = 0;
            PB_MEMORIZING = -1;
            PB_SCRIBING = -1;
            PB_MODE = 4;
            PB_SHORT(0x5f186) = -1;
            PB_PRESSED = -1;
            PB_SHORT(0x5f18a) = -1;
            PB_INT(0xeb8) = 1;
        }
        else
        {
            PB_VIRTUAL(self, 8);
            PB_CHAR(0xfe2) = PB_MODE;
            PB_MODE = 4;
            PB_SHORT(0x5f186) = -1;
            PB_PRESSED = -1;
            PB_SHORT(0x5f18a) = -1;
            PB_INT(0x8fba8) = 1;
        }
        goto activateReturn;
    case 0xdf:
spellbookFlip:
        PB_CHAR(0xfe2) = PB_MODE;
        PB_INT(0x6c554) = *(int*)(PB_DISPLAY + 0xbc);
        return;
    case 0xe0:
        EQ_METHOD(0, void, 0x0040F47F, (char*), PB_DISPLAY)();
        break;
    case 0xe1:
        EQ_GLOBAL(u8, 0x006B658C) ^= 1;
        break;
    case 0xfc:
        if (g_local_player_guess != 0)
            PB_TARGET = g_local_player_guess;
        break;
    case 0xfe:
    case 0xff:
    case 0x100:
    case 0x101:
    case 0x102:
        index = button - 0xfe;
        if (EQ_ARRAY(char, 0x005FE8BD)[index * 0x1e] != 0)
        {
            if (EQ_ARRAY(EQPlayer*, 0x005FE954)[index] != 0)
                PB_TARGET = EQ_ARRAY(EQPlayer*, 0x005FE954)[index];
            PB_CHAR(0xb4) = (char)button + 2;
        }
        break;
    }
    optionsPage = PB_OPTIONS_PAGE;
    if (optionsPage == 0)
        goto trackWindow;
    switch (button)
    {
    case 0x67:
        PB_VIRTUAL(self, 8);
        PB_OPTIONS_PAGE = 2;
        goto activateReturn;
    case 0x68:
        PB_VIRTUAL(self, 8);
        PB_OPTIONS_PAGE = 3;
        goto activateReturn;
    case 0x69:
        PB_VIRTUAL(self, 8);
        PB_OPTIONS_PAGE = 4;
        goto activateReturn;
    case 0x6a:
        PB_VIRTUAL(self, 8);
        PB_OPTIONS_PAGE = 5;
        goto activateReturn;
    case 0x6b:
        PB_VIRTUAL(self, 8);
        PB_OPTIONS_PAGE = 6;
        goto activateReturn;
    case 0x6e:
        if (optionsPage == 1)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 0;
            PB_VIRTUAL(self, 4);
            PB_SAVE_OPTIONS();
            if (PB_VIDEO_RESTORE != 2)
                return;
            if (PB_MODE == 5)
                return;
            goto restoreVideo;
        }
        if (optionsPage == 7)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 3;
            goto optionsActivateSave;
        }
        if (optionsPage == 8)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 6;
            PB_VIRTUAL(self, 4);
            PB_SAVE_OPTIONS2();
            return;
        }
        goto optionsPageDown;
    case 0x6f:
        if (optionsPage == 3)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 1;
            goto optionsActivateSave;
        }
optionsPageDown:
        PB_VIRTUAL(self, 8);
        if (PB_CHAR(0x100d) == 0)
            goto optionsMain;
        oldType = PB_CHAR(0x100d) - 1;
        goto optionsPageStore;
    case 0x70:
        if (optionsPage == 3)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 7;
            goto optionsActivateSave;
        }
        if (optionsPage == 6)
        {
            PB_VIRTUAL(self, 8);
            PB_OPTIONS_PAGE = 8;
optionsActivateSave:
            PB_VIRTUAL(self, 4);
            PB_SAVE_OPTIONS();
            return;
        }
        PB_VIRTUAL(self, 8);
        if (*(u8*)(self + 0x100d) >= 10)
            goto activateReturn;
        oldType = PB_CHAR(0x100d) + 1;
optionsPageStore:
        PB_CHAR(0x100d) = oldType;
        PB_OPTIONS_PAGE = optionsPage;
        goto activateReturn;
    case 0x72:
        if (optionsPage != 8)
            goto filterToggle;
        if (++EQ_GLOBAL(int, 0x006B6EE4) >= 4)
            EQ_GLOBAL(int, 0x006B6EE4) = 0;
        goto filtersSaved;
    case 0x73:
        if (optionsPage != 8)
            goto filterToggle;
        if (++EQ_GLOBAL(int, 0x006B6EE8) >= 2)
            EQ_GLOBAL(int, 0x006B6EE8) = 0;
        goto filtersSaved;
    case 0x74:
        if (optionsPage != 8)
            goto filterToggle;
        if (++EQ_GLOBAL(int, 0x006B6EEC) >= 4)
            EQ_GLOBAL(int, 0x006B6EEC) = 0;
filtersSaved:
        PB_SAVE_OPTIONS2();
        goto sendFilters;
    case 0x75:
        if (optionsPage != 8)
            goto filterToggle;
        EQ_GLOBAL(int, 0x006B6F00) ^= 1;
        PB_DSP_CHAT(EQ_GLOBAL(int, 0x006B6F00) != 0 ? EQ_ARRAY(char, 0x0055B168) : EQ_ARRAY(char, 0x0055B118), 0x111);
saveOptions2Track:
        PB_SAVE_OPTIONS2();
        goto trackWindow;
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
filterToggle:
        if (optionsPage == 6)
        {
            EQ_ARRAY(u32, 0x006B67BC)[button] ^= 1;
            PB_SAVE_OPTIONS();
        }
sendFilters:
        EQ_METHOD(0, void, 0x004CF829, (void*), PB_EVERQUEST)();
afterFilters:
        if (mouseButton == 1)
            goto optionsRightClick;
        goto afterOptionsMouse;
    case 0x7d:
    case 0x7e:
    case 0x7f:
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
    case 0x84:
    case 0x85:
        if (optionsPage == 5)
        {
            PB_CHAR(0x8fbad) = PB_CHAR(0x100d) * 9 + (char)button - 0x7c;
            PB_INT(0x8fbb0) = 0;
        }
        goto trackWindow;
    case 0x86:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B68AC) != 0)
        {
            EQ_GLOBAL(int, 0x006B68AC) = 0;
            text = EQ_ARRAY(char, 0x0055B314);
        }
        else
        {
            EQ_GLOBAL(int, 0x006B68AC) = 1;
            text = EQ_ARRAY(char, 0x0055B2F8);
        }
optionsChat:
        PB_DSP_CHAT(text, 0xf);
        goto saveOptions;
    case 0x87:
        if (optionsPage != 2)
            goto trackWindow;
        if (++EQ_GLOBAL(int, 0x006B6868) > 2)
            EQ_GLOBAL(int, 0x006B6868) = 0;
        goto saveOptions;
    case 0x88:
        if (optionsPage != 2)
            goto trackWindow;
        EQ_GLOBAL(int, 0x006B6860) = EQ_GLOBAL(int, 0x006B6860) == 0;
        g_local_player_guess->anon = EQ_GLOBAL(int, 0x006B6860);
        PB_DSP_CHAT(g_local_player_guess->anon != 0 ? EQ_ARRAY(char, 0x0055B2E0) : EQ_ARRAY(char, 0x0055B2C0), 0x111);
        appearance.type = 0x15;
        appearance.spawnId = g_local_player_guess->spawn_id;
        appearance.value = g_local_player_guess->anon;
        PB_SEND(0x20f5, &appearance, 0xc);
        g_pc_guess->anon = (u8)g_local_player_guess->anon;
        EQ_GLOBAL(int, 0x006B7120) = 5;
        EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), PB_EVERQUEST)(0, 1);
        goto saveOptions;
    case 0x89:
        if (optionsPage != 2)
            goto trackWindow;
        EQ_GLOBAL(int, 0x006B6864) ^= 1;
        goto saveOptions;
    case 0x8a:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6894) > 0)
            EQ_GLOBAL(int, 0x006B6894)--;
soundVolume:
        if (PB_SOUND_OFF == 0 && PB_SOUND != 0)
        {
            EQ_GLOBAL(int, 0x0057931C) = EQ_GLOBAL(int, 0x006B6894);
            EQ_METHOD(1, void, 0x004E3346, (void*, int), PB_SOUND)(EQ_GLOBAL(int, 0x006B6894));
        }
        goto saveOptions;
    case 0x8b:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6894) < 0x14)
            EQ_GLOBAL(int, 0x006B6894)++;
        goto soundVolume;
    case 0x8d:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B688C) > 0)
            EQ_GLOBAL(int, 0x006B688C)--;
musicVolume:
        if (PB_SOUND_OFF == 0 && PB_MUSIC != 0)
        {
            EQ_GLOBAL(int, 0x00579318) = (int)(EQ_GLOBAL(int, 0x006B688C) * EQ_GLOBAL(float, 0x00535578));
            EQ_METHOD(2, int, 0x004E5EE6, (void*, int, int), PB_MUSIC)(0, EQ_GLOBAL(int, 0x00579318));
        }
        goto saveOptions;
    case 0x8e:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B688C) < 0x14)
            EQ_GLOBAL(int, 0x006B688C)++;
        goto musicVolume;
    case 0x90:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6890) > 0)
            EQ_GLOBAL(int, 0x006B6890)--;
effectsVolume:
        if (PB_SOUND_OFF == 0 && PB_SOUND != 0)
        {
            EQ_GLOBAL(int, 0x00579314) = EQ_GLOBAL(int, 0x006B6890) * 5;
            EQ_METHOD(1, void, 0x004E36EE, (void*, int), PB_SOUND)(EQ_GLOBAL(int, 0x00579314));
        }
        goto saveOptions;
    case 0x91:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6890) < 0x14)
            EQ_GLOBAL(int, 0x006B6890)++;
        goto effectsVolume;
    case 0x93:
        if (optionsPage != 2)
            goto trackWindow;
        EQ_GLOBAL(int, 0x006B686C) ^= 1;
        goto saveOptions;
    case 0x94:
        if (optionsPage != 2)
            goto trackWindow;
        *(int*)&g_pc_guess->autosplit = *(int*)&g_pc_guess->autosplit == 0;
        PB_DSP_CHAT(*(int*)&g_pc_guess->autosplit != 0 ? EQ_ARRAY(char, 0x0055B288) : EQ_ARRAY(char, 0x0055B258), 0x111);
        EQ_METHOD(0, void, 0x004CFF5E, (void*), PB_EVERQUEST)();
        goto saveOptions;
    case 0x95:
        if (optionsPage != 2)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6EFC) == -1)
            text = EQ_ARRAY(char, 0x0055B254);
        else if (EQ_GLOBAL(int, 0x006B6EFC) == 0)
            text = EQ_ARRAY(char, 0x0055B250);
        else
            text = EQ_ARRAY(char, 0x0055B248);
        EQ_CDECL(void, 0x004A887E, (EQPlayer*, char*))(g_local_player_guess, text);
        goto trackWindow;
    case 0x96:
        if (optionsPage == 3)
        {
            PB_FLOAT_GAMMA = PB_FLOAT_GAMMA - 1.0f;
            if (PB_FLOAT_GAMMA < 0.0f)
                PB_FLOAT_GAMMA = 0.0f;
gammaApply:
            EQ_METHOD(1, void, 0x0040AF4F, (char*, float), PB_DISPLAY)(PB_FLOAT_GAMMA * 0.25f + 0.5f);
        }
        goto saveOptions;
    case 0x97:
        if (optionsPage == 3)
        {
            PB_FLOAT_GAMMA = PB_FLOAT_GAMMA + 1.0f;
            if (PB_FLOAT_GAMMA > 8.0f)
                PB_FLOAT_GAMMA = 8.0f;
            goto gammaApply;
        }
        goto saveOptions;
    case 0x99:
        if (optionsPage == 3)
        {
            if (++EQ_GLOBAL(int, 0x006B6870) > 2)
                EQ_GLOBAL(int, 0x006B6870) = 0;
            switch (EQ_GLOBAL(int, 0x006B6870))
            {
            case 0:
                g_cdisplay_guess->skyLayerMode_guess = 0;
                break;
            case 1:
                g_cdisplay_guess->skyLayerMode_guess = EQ_GLOBAL(u8, 0x006B6726);
                EQ_METHOD(1, void, 0x004124BE, (char*, int), PB_DISPLAY)(0);
                break;
            case 2:
                g_cdisplay_guess->skyLayerMode_guess = EQ_GLOBAL(u8, 0x006B6726);
                EQ_METHOD(1, void, 0x004124BE, (char*, int), PB_DISPLAY)(1);
                break;
            }
        }
        goto saveOptions;
    case 0x9a:
        if (optionsPage == 3)
        {
            if (++EQ_GLOBAL(int, 0x006B6874) > 3)
                EQ_GLOBAL(int, 0x006B6874) = 0;
            EQ_METHOD(1, void, 0x0040AF61, (char*, int), PB_DISPLAY)(EQ_GLOBAL(int, 0x006B6874));
        }
        goto saveOptions;
    case 0x9d:
        if (optionsPage != 3)
            goto trackWindow;
        if ((EQ_GLOBAL(int, 0x006B6884) ^= 1) != 0)
            EQ_METHOD(0, void, 0x00412194, (char*), PB_DISPLAY)();
        else
            EQ_METHOD(0, void, 0x004121CA, (char*), PB_DISPLAY)();
        goto saveOptions;
    case 0x9e:
        if (optionsPage != 3)
            goto trackWindow;
        if ((EQ_GLOBAL(int, 0x006B6888) ^= 1) != 0)
            EQ_METHOD(0, void, 0x004121FD, (char*), PB_DISPLAY)();
        else
            EQ_METHOD(0, void, 0x00412233, (char*), PB_DISPLAY)();
        goto saveOptions;
    case 0x9f:
        if (optionsPage != 3)
            goto trackWindow;
        oldVideoMode = PB_CURRENT_VIDEO++;
        PB_INT(0xeb8) = 1;
        if (PB_CURRENT_VIDEO == 1)
        {
            if (EQ_METHOD(1, int, 0x0040E4E4, (char*, int), PB_DISPLAY)(1) == 0)
                PB_CURRENT_VIDEO++;
            else if (oldVideoMode != 1)
            {
                if (EQ_GLOBAL(int, 0x006B6880) == 0)
                    PB_VIDEO_MODE = 2;
                PB_CHANGE_VIDEO_MODE();
            }
        }
        if (PB_CURRENT_VIDEO == 2)
        {
            if (EQ_METHOD(1, int, 0x0040E4E4, (char*, int), PB_DISPLAY)(2) == 0)
                PB_CURRENT_VIDEO++;
            else if (oldVideoMode != 2)
            {
                if (EQ_GLOBAL(int, 0x006B6880) == 0)
                    PB_VIDEO_MODE = 2;
                PB_CHANGE_VIDEO_MODE();
            }
        }
        if (g_pc_guess != 0 && EQ_METHOD(0, int, 0x004197B2, (CharacterProfile*), g_pc_guess)() != 0)
        {
            if (PB_CURRENT_VIDEO == 3)
            {
                if (EQ_METHOD(1, int, 0x0040E4E4, (char*, int), PB_DISPLAY)(3) == 0)
                    PB_CURRENT_VIDEO++;
                else if (oldVideoMode != 3)
                {
                    if (EQ_GLOBAL(int, 0x006B6880) == 0)
                        PB_VIDEO_MODE = 2;
                    PB_CHANGE_VIDEO_MODE();
                }
            }
            if (PB_CURRENT_VIDEO == 4)
            {
                if (EQ_METHOD(1, int, 0x0040E4E4, (char*, int), PB_DISPLAY)(4) == 0)
                    EQ_GLOBAL(int, 0x006B687C)++;
                else if (oldVideoMode != 4)
                {
                    if (EQ_GLOBAL(int, 0x006B6880) == 0)
                        PB_VIDEO_MODE = 2;
                    PB_CHANGE_VIDEO_MODE();
                }
            }
            if (PB_CURRENT_VIDEO > 4)
                PB_CURRENT_VIDEO = 0;
        }
        else if (PB_CURRENT_VIDEO > 2)
            PB_CURRENT_VIDEO = 0;
        if (PB_CURRENT_VIDEO == 0 &&
            (oldVideoMode == 1 || oldVideoMode == 2 || oldVideoMode == 3 || oldVideoMode == 4))
            PB_CHANGE_VIDEO_MODE();
        sprintf(videoModeText, EQ_ARRAY(char, 0x0053E16C), PB_CURRENT_VIDEO);
        EQ_GLOBAL(tWriteProfileString_guess, 0x00534188)(EQ_ARRAY(char, 0x0053DB3C), EQ_ARRAY(char, 0x0055B23C),
            videoModeText, EQ_ARRAY(char, 0x0053DB50));
        goto saveOptions;
    case 0xa0:
        if (optionsPage != 3)
            goto trackWindow;
        EQ_GLOBAL(int, 0x006B6880) = EQ_GLOBAL(int, 0x006B6880) == 0;
        PB_CHANGE_VIDEO_MODE();
        goto saveOptions;
    case 0xa1:
        if (optionsPage != 3)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B6878) != 0)
        {
            EQ_GLOBAL(int, 0x006B6878) = 0;
            EQ_CDECL(void, 0x004AC585, (int))(0);
        }
        else
        {
            EQ_GLOBAL(int, 0x006B6878) = 1;
            EQ_CDECL(void, 0x004AC585, (int))(1);
        }
        goto saveOptions;
    case 0xa2:
        if (optionsPage != 3)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B68B0) != 0)
        {
            EQ_GLOBAL(int, 0x006B68B0) = 0;
            text = EQ_ARRAY(char, 0x0055B1FC);
        }
        else
        {
            EQ_GLOBAL(int, 0x006B68B0) = 1;
            text = EQ_ARRAY(char, 0x0055B1BC);
        }
        goto optionsChat;
    case 0xa3:
        if (optionsPage != 7)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B68B8) > 0)
            EQ_GLOBAL(int, 0x006B68B8)--;
        PB_FLOAT_SCALE = EQ_GLOBAL(int, 0x006B68B8) * 0.2f;
        if (PB_FLOAT_SCALE < 0.1f)
            PB_FLOAT_SCALE = 0.1f;
        PB_SAVE_OPTIONS();
        goto saveOptions2Track;
    case 0xa4:
        if (optionsPage != 7)
            goto trackWindow;
        if (EQ_GLOBAL(int, 0x006B68B8) < 0x14)
            EQ_GLOBAL(int, 0x006B68B8)++;
        PB_FLOAT_SCALE = EQ_GLOBAL(int, 0x006B68B8) * 0.2f;
        if (PB_FLOAT_SCALE > 4.0f)
            PB_FLOAT_SCALE = 4.0f;
        PB_SAVE_OPTIONS();
        goto saveOptions2Track;
    case 0xa5:
        if (optionsPage == 4)
        {
            if (EQ_GLOBAL(int, 0x006B689C) > 1)
                EQ_GLOBAL(int, 0x006B689C)--;
detailLevel:
            EQ_GLOBAL(short, 0x006B641C) = (short)EQ_GLOBAL(int, 0x006B689C);
        }
        goto saveOptions;
    case 0xa6:
        if (optionsPage == 4)
        {
            if (EQ_GLOBAL(int, 0x006B689C) < 8)
                EQ_GLOBAL(int, 0x006B689C)++;
            goto detailLevel;
        }
        goto saveOptions;
    case 0xa7:
        if (optionsPage == 4)
        {
            EQ_GLOBAL(int, 0x006B68A0) ^= 1;
            EQ_GLOBAL(u8, 0x006B6458) = (u8)EQ_GLOBAL(int, 0x006B68A0);
        }
        goto saveOptions;
    case 0xa8:
        if (optionsPage == 4)
            EQ_GLOBAL(int, 0x006B68A4) = EQ_GLOBAL(int, 0x006B68A4) == 0;
        goto saveOptions;
    case 0xa9:
        if (optionsPage != 4)
            goto trackWindow;
        EQ_GLOBAL(int, 0x006B68A8) = EQ_GLOBAL(int, 0x006B68A8) == 0;
        EQ_GLOBAL(u8, 0x006B6409) = (u8)EQ_GLOBAL(int, 0x006B68A8);
saveOptions:
        PB_SAVE_OPTIONS();
        goto trackWindow;
    }
trackWindow:
    if (PB_INT(0x685f4) != 0)
    {
        if (button == 0x50)
        {
            if (PB_INT(0x685f8 + PB_INT(0x6c4d8) * 0x10) != 0)
            {
                EQ_GLOBAL(short, 0x006B6772) = 0x35;
                EQ_GLOBAL(int, 0x006B6780) = PB_INT(0x685f8 + PB_INT(0x6c4d8) * 0x10);
                index = EQ_CDECL(int, 0x004A461E, (void))();
                EQ_GLOBAL(int, 0x006B677C) = 0;
                EQ_GLOBAL(int, 0x006B6774) = index;
                EQ_GLOBAL(int, 0x006B6778) = index + 360000;
                sprintf(message, EQ_ARRAY(char, 0x0055AC10), PB_TRIM_NAME((char*)EQ_GLOBAL(int, 0x006B6780) + 1));
                PB_DSP_CHAT(message, 0x10e);
            }
            for (index = 0; index < 1000; index++)
            {
                PB_INT(0x685f8 + index * 0x10) = 0;
                *(float*)(self + 0x685fc + index * 0x10) = -1.0f;
                PB_INT(0x68600 + index * 0x10) = -1;
                PB_INT(0x68604 + index * 0x10) = 0;
            }
        }
        else if (button == 0x51)
        {
            if (EQ_GLOBAL(short, 0x006B6772) == 0x35)
                EQ_GLOBAL(short, 0x006B6772) = -1;
            for (index = 0; index < 1000; index++)
            {
                PB_INT(0x685f8 + index * 0x10) = 0;
                *(float*)(self + 0x685fc + index * 0x10) = -1.0f;
                PB_INT(0x68600 + index * 0x10) = -1;
                PB_INT(0x68604 + index * 0x10) = 0;
            }
        }
        else
        {
            if (button > 0x51 && button <= 0x65)
                PB_INT(0x6c4d8) = PB_INT(0x6c340 + button * 4);
            goto screens;
        }
        PB_VIRTUAL(self, 8);
        PB_MODE = 0;
        PB_VIRTUAL(self, 4);
    }
screens:
    mode = PB_MODE;
    if (mode == 0)
        goto mainScreen;
    if (mode == 1)
    {
        if (mouseButton != 0)
        {
            if (mouseButton != 1)
                return;
            if (button < 0xf0 || button > 0xf5)
                return;
            goto socialEdit;
        }
        if (PB_HOTKEY_EDIT != 0 && button >= 0xf0 && button <= 0xf5)
        {
            PB_DRAG_TYPE = 3;
            button -= 0xf0;
            goto setDragValue;
        }
        if (button == 0xe3)
            goto caseE4;
        if (button == 0xe4)
            goto closeToMain;
        if (button == 0xe5)
            goto caseE5;
        if (button <= 0xef)
            goto socialAssign;
        if (button <= 0xf5)
        {
            if (*(short*)(self + button * 2 + 0xe14) == -1)
            {
socialEdit:
                PB_VIRTUAL(self, 8);
                PB_INT(0x685d8) = 1;
                PB_SHORT(0x6c568) = button - 0xf0;
                PB_MODE = 1;
                goto activateReturn;
            }
            EQ_METHOD(2, void, 0x0041D790, (CharacterProfile*, char, EQPlayer*), g_pc_guess)(
                *(char*)(self + button * 2 + 0xe14), PB_TARGET);
            return;
        }
        if (button != 0x163)
            goto socialAssign;
        PB_INT(0x685d8) = 0;
socialRefresh:
        PB_VIRTUAL(self, 8);
        PB_MODE = 1;
activateRestore:
        PB_VIRTUAL(self, 4);
        if (PB_VIDEO_RESTORE != 2)
            return;
        PB_VIDEO_MODE = 2;
        PB_CHANGE_VIDEO_MODE();
        PB_PREV_MODE = 0;
        return;
socialAssign:
        if (PB_INT(0x685d8) == 0)
            return;
        if (button < 0x145 || button > 0x162)
            return;
        PB_INT(0x685d8) = 0;
        *(short*)(self + PB_SHORT(0x6c568) * 2 + 0xff4) = *(short*)(self + button * 2 + 0x6c3d4);
        PB_SAVE_CUSTOM();
        goto socialRefresh;
    }
    if (mode == 2)
    {
        if (mouseButton != 0)
        {
            if (mouseButton != 1)
                return;
            if (button < 0xf8 || button > 0xfb)
                return;
            PB_VIRTUAL(self, 8);
            PB_INT(0x685d4) = 1;
            PB_SHORT(0x6c568) = button - 0xf8;
            PB_MODE = 2;
            goto activateReturn;
        }
        if (PB_HOTKEY_EDIT != 0)
        {
            if (button == 0xf6 || button == 0xf7)
            {
                PB_DRAG_TYPE = 1;
                button -= 0xf6;
                goto setDragValue;
            }
            if (button >= 0xf8 && button <= 0xfb)
            {
                PB_DRAG_TYPE = 2;
                button -= 0xf8;
                goto setDragValue;
            }
        }
        if (button <= 0xf6)
        {
            if (button == 0xf6)
                EQ_CDECL(void, 0x004D7D6A, (int, int, int))(8, 1, 0);
            else if (button == 0xe3)
                goto caseE3;
            else if (button == 0xe4)
                goto closeToMain;
            else if (button == 0xe5)
                goto caseE5;
        }
        else if (button == 0xf7)
        {
            if (pinstEQ_Main_guess->primaryAttackReady_guess != 0)
            {
                pinstEQ_Main_guess->primaryAttackReady_guess = 0;
                EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, int, EQPlayer*), g_local_player_guess)(0xb, 0, PB_TARGET);
            }
        }
        else if (button <= 0xfb)
        {
            if (*(short*)(self + button * 2 + 0xdfc) == -1)
            {
                PB_VIRTUAL(self, 8);
                PB_INT(0x685d4) = 1;
                PB_SHORT(0x6c568) = button - 0xf8;
                PB_MODE = 2;
                PB_VIRTUAL(self, 4);
            }
            else
            {
                EQ_METHOD(2, void, 0x0041D790, (CharacterProfile*, char, EQPlayer*), g_pc_guess)(
                    *(char*)(self + button * 2 + 0xdfc), PB_TARGET);
            }
        }
        else if (button == 0x163)
        {
            PB_VIRTUAL(self, 8);
            PB_INT(0x685d4) = 0;
            goto combatRefresh;
        }
        if (PB_INT(0x685d4) == 0)
            return;
        if (button < 0x145 || button > 0x162)
            return;
        PB_INT(0x685d4) = 0;
        *(short*)(self + PB_SHORT(0x6c568) * 2 + 0xfec) = *(short*)(self + button * 2 + 0x6c3d4);
        PB_SAVE_CUSTOM();
        PB_VIRTUAL(self, 8);
combatRefresh:
        PB_MODE = 2;
        goto activateRestore;
    }
    if (mode == 3)
    {
        if (mouseButton == 0)
        {
            if (PB_HOTKEY_EDIT != 0 && button >= 0x39 && button <= 0x44)
            {
                PB_DRAG_TYPE = 4;
                button = (short)(button + PB_CHAR(0x6841c) * 12 - 0x39);
                goto setDragValue;
            }
            if (button > 0x4d)
            {
                if (button == 0x4e)
                {
                    if (--PB_CHAR(0x6841c) >= 0)
                        return;
                    PB_CHAR(0x6841c) = 9;
                    goto macroRightClick;
                }
                if (button == 0x4f)
                {
                    if (++PB_CHAR(0x6841c) > 9)
                        PB_CHAR(0x6841c) = 0;
                    return;
                }
                if (button == 0xe3)
                {
                    PB_INT(0x5f190) = 0;
                    PB_CHAR(0x5f195) = -1;
                    PB_INT(0x5f18c) = 0;
                    goto caseE3;
                }
                if (button == 0xe4)
                {
                    PB_INT(0x5f190) = 0;
                    PB_CHAR(0x5f195) = -1;
                    PB_INT(0x5f18c) = 0;
                    goto caseE4;
                }
                if (button != 0xe5)
                    return;
                PB_INT(0x5f190) = 0;
                PB_CHAR(0x5f195) = -1;
                PB_INT(0x5f18c) = 0;
                PB_VIRTUAL(self, 8);
                PB_MODE = 0;
                goto macroReset;
            }
            if (button == 0x4d)
            {
                index = PB_CHAR(0x5f194);
                memcpy(self + 0x5f196 + (index / 12 * 12 + index % 12) * 0x136, self + 0x682e6, 0x136);
                PB_INT(0x5f190) = 0;
                PB_INT(0x5f18c) = 0;
                PB_CHAR(0x5f195) = -1;
                PB_SAVE_CUSTOM();
                goto macroClose;
            }
            if (button < 0x39)
                return;
            if (button <= 0x44)
            {
                if (EQ_GLOBAL(int, 0x0053F230) != -1)
                    return;
                EQ_GLOBAL(int, 0x0053F230) = (button + PB_CHAR(0x6841c) * 12) * 8 - 0x1c8;
                return;
            }
            if (button == 0x45)
            {
                PB_CHAR(0x5f195) = 5;
                EQ_GLOBAL(int, 0x006EFDC8) = 0;
                EQ_GLOBAL(int, 0x006EFDCC) = 0;
                strcpy(self + 0x682e6, EQ_ARRAY(char, 0x0058FBA4));
                return;
            }
            if (button == 0x46)
            {
                if (++PB_CHAR(0x6841b) > 0x13)
                    PB_CHAR(0x6841b) = 0;
                return;
            }
            if (button <= 0x4b)
            {
                PB_CHAR(0x5f195) = (char)button - 0x47;
                EQ_GLOBAL(int, 0x006EFDC8) = 0;
                EQ_GLOBAL(int, 0x006EFDCC) = 0;
                strcpy(self + PB_CHAR(0x5f195) * 0x3c + 0x682ef, EQ_ARRAY(char, 0x0058FBA4));
                return;
            }
            if (button != 0x4c)
                return;
            PB_INT(0x5f190) = 0;
            PB_CHAR(0x5f195) = -1;
            PB_INT(0x5f18c) = 0;
macroClose:
            PB_VIRTUAL(self, 8);
            PB_MODE = 3;
macroReset:
            PB_SHORT(0x5f186) = -1;
            PB_PRESSED = -1;
            PB_SHORT(0x5f18a) = -1;
            goto activateRestore;
        }
macroRightClick:
        if (mouseButton != 1)
            return;
        if (button < 0x39 || button > 0x44)
            return;
        EQ_GLOBAL(u8, 0x006B6404) = 0;
        pinstCEverQuest_guess->chatInputChanged_guess = 1;
        oldType = PB_CHAR(0x6841c) * 12 + (char)button - 0x39;
        PB_CHAR(0x5f194) = oldType;
        index = oldType;
        memcpy(self + 0x682e6, self + 0x5f196 + (index / 12 * 12 + index % 12) * 0x136, 0x136);
        PB_CHAR(0x5f195) = -1;
        PB_VIRTUAL(self, 8);
        PB_MODE = 3;
        PB_SHORT(0x5f186) = -1;
        PB_PRESSED = -1;
        PB_SHORT(0x5f18a) = -1;
        PB_INT(0x5f18c) = 1;
        goto activateReturn;
    }
    if (mode == 4)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
        {
            if (PB_INSPECT != 0 && mouseButton == 1)
            {
                if (PB_LOOT_OPEN != 0 && button >= 0x113 && button <= 0x122)
                {
                    item = *(EQ_Item**)(self + (button + PB_LIST_PAGE * 2) * 4 + 0x6c194);
                    if (item != 0)
                    {
                        *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
                if (PB_MERCHANT_OPEN != 0 && button >= 0x104 && button <= 0x10f)
                {
                    item = *(EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4);
                    if (item != 0)
                    {
                        *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
                if (PB_BANK_OPEN != 0 && button >= 0xaa && button <= 0xb1)
                {
                    if (PB_HOUSE_BANK != 0)
                        item = *(EQ_Item**)(self + button * 4 - 0x1a0);
                    else
                        item = g_pc_guess->bank_items[button - 0xaa];
                    if (item != 0)
                    {
                        *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
                if (button >= 1 && button <= 0x1d)
                {
                    if (PB_CHAR(0x100c) != 0)
                    {
                        if (button >= 0x16)
                            return;
                        *(short*)(PB_ITEM_DISPLAY + 0x96) = button;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                        return;
                    }
                    item = g_pc_guess->inventory_items[button];
                    if (item != 0)
                    {
                        *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
                if (PB_TRADE_OPEN == 0)
                    return;
                if (PB_TRADE_TARGET == 0)
                    return;
                if (button >= 0x12c && button <= 0x133)
                {
                    item = *(EQ_Item**)(self + button * 4 + 0x6c03c);
                    if (item != 0)
                    {
                        *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                        PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                    }
                }
                if (button < 0x136 || button > 0x13d)
                    return;
                item = *(EQ_Item**)(self + button * 4 + 0x6c034);
                if (item == 0)
                    return;
                *(EQ_Item**)(PB_ITEM_DISPLAY + 0x90) = item;
                PB_VIRTUAL(PB_ITEM_DISPLAY, 4);
                return;
            }
            if (mouseButton != 0)
                return;
            PB_PRESSED = button;
            PB_SHORT(0x5f18a) = -1;
            if (button != 0x2a)
                return;
            if (PB_MERCHANT_OPEN != 0)
                return;
            if (PB_LOOT_OPEN != 0)
                return;
            if (PB_BANK_OPEN != 0)
                return;
            if (PB_INT(0x8fb90) == 0)
                return;
setLayoutFlag:
            PB_INT(0x5f174) = 1;
            return;
        }
        if (PB_INT(0x94) != 0)
            return;
        if (PB_TRADE_OPEN != 0)
        {
            if (PB_INT(0x685ec) != 0 && PB_INT(0x685f0) != 0)
                return;
            if (PB_TRADE_OPEN != 0)
            {
                if (PB_INT(0x685ec) != 0 && button != 0x144)
                {
                    PB_DSP_CHAT(EQ_ARRAY(char, 0x0055AE0C), 0xf);
                    return;
                }
                if (PB_TRADE_OPEN != 0 && PB_INT(0x6c908) == 0)
                    return;
            }
        }
        if (PB_MERCHANT_OPEN != 0 && PB_INT(0xa0) == 0)
            return;
        if (PB_LOOT_OPEN != 0 && PB_INT(0xa4) == 0)
            return;
        if (PB_HOTKEY_EDIT == 0)
        {
            if (mouseButton != 0 || button != 0x27)
            {
                if (mouseButton != 1)
                    goto inventoryLeftClick;
                if (button != 0x38 && button != 0x20)
                    goto inventoryRightClick;
            }
            if (*(int*)(PB_BOOK + 0x10) != 0 && PB_INT(0x8fba4) == 0)
                return;
            if ((PB_INT(0x8fba4) ^= 1) != 0)
            {
                PB_INT(0x8fb94) = 1;
                PB_INT(0x8fb9c) = 1;
                PB_INT(0x8fba0) = 1;
                PB_INT(0x8fb98) = 1;
            }
            else
            {
                PB_INT(0x8fb94) = 0;
                PB_INT(0x8fb9c) = 0;
                PB_INT(0x8fba0) = 0;
                PB_INT(0x8fb98) = 0;
            }
            PB_VIRTUAL(self, 8);
            PB_INT(0x8fba8) = 1;
            PB_MODE = 4;
            goto activateReturn;
        }
        if (mouseButton != 1)
            goto inventoryLeftClick;
inventoryRightClick:
        if (PB_INSPECT != 0)
            goto tradeWindow;
        if (PB_BANK_OPEN != 0 && button >= 0xaa && button <= 0xb1)
        {
            index = button;
            if (PB_HOUSE_BANK != 0)
                item = *(EQ_Item**)(self + index * 4 - 0x1a0);
            else
                item = g_pc_guess->bank_items[index - 0xaa];
            if (item == 0 || item->type != 1)
                goto inventoryBagRightClick;
            if (PB_BAG_OPEN(item) == 1)
            {
                if (PB_IS_ACTIVE(PB_BAG1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG1, 8);
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (PB_IS_ACTIVE(PB_BAG2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG2, 8);
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            else if (PB_IS_ACTIVE(PB_BAG1) == 0)
            {
                PB_BAG_OPEN(item) = 1;
                pinstEQ_Bag1_guess->item_guess = item;
                PB_VIRTUAL_ARG(PB_BAG1, 0x10, index - 0xa2);
                pinstEQ_Bag1_guess->slot_guess = button - 0xaa;
                pinstEQ_Bag1_guess->bankContainer_guess = 1;
            }
            else if (PB_IS_ACTIVE(PB_BAG2) == 0)
            {
                PB_BAG_OPEN(item) = 1;
                pinstEQ_Bag2_guess->item_guess = item;
                PB_VIRTUAL_ARG(PB_BAG2, 0x10, index - 0xa2);
                pinstEQ_Bag2_guess->slot_guess = button - 0xaa;
                pinstEQ_Bag2_guess->bankContainer_guess = 1;
            }
            if (PB_INSPECT != 0)
                goto tradeWindow;
        }
inventoryBagRightClick:
        if (button >= 0x16 && button <= 0x1d && PB_CHAR(0x100c) == 0)
        {
            index = button;
            slotOffset = index * 4 + 0xe0;
            item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
            if (item != 0 && item->type == 1)
            {
                if (PB_BAG_OPEN(item) == 1)
                {
                    if (PB_IS_ACTIVE(PB_BAG1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                    {
                        PB_BAG_OPEN(item) = 0;
                        PB_VIRTUAL(PB_BAG1, 8);
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (PB_IS_ACTIVE(PB_BAG2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                    {
                        PB_BAG_OPEN(item) = 0;
                        PB_VIRTUAL(PB_BAG2, 8);
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                }
                else if (PB_IS_ACTIVE(PB_BAG1) == 0)
                {
                    PB_BAG_OPEN(item) = 1;
                    pinstEQ_Bag1_guess->item_guess = item;
                    PB_VIRTUAL_ARG(PB_BAG1, 0x10, index - 0x16);
                    pinstEQ_Bag1_guess->slot_guess = button;
                }
                else if (PB_IS_ACTIVE(PB_BAG2) == 0)
                {
                    PB_BAG_OPEN(item) = 1;
                    pinstEQ_Bag2_guess->item_guess = item;
                    PB_VIRTUAL_ARG(PB_BAG2, 0x10, index - 0x16);
                    pinstEQ_Bag2_guess->slot_guess = button;
                }
            }
            item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
            if (item != 0 && item->type == 2)
            {
                if (*(int*)(PB_BOOK + 0x10) == 0)
                {
                    *(EQ_Item**)(PB_BOOK + 0x37ac) = item;
                    text = *(char**)(PB_BOOK + 0x37ac) + 0xaf;
                    PB_SEND(0x20ce, text, strlen(text) + 1);
                    goto inventoryLeftClick;
                }
                if (PB_IS_ACTIVE(PB_BOOK) != 0 && *(EQ_Item**)(PB_BOOK + 0x37ac) == item)
                    PB_VIRTUAL(PB_BOOK, 8);
            }
        }
        goto tradeWindow;
inventoryLeftClick:
        if (mouseButton != 0)
            goto tradeWindow;
        if (button == 0x20)
        {
            if (*(char*)(PB_DISPLAY + 0x3d) == 0)
            {
                if (g_pc_guess->inventory_items[0] != 0 && PB_SOUND != 0)
                    PB_WAVE_PLAY();
                EQ_CDECL(void, 0x004A2351, (CharacterProfile*, EQ_Item**, int))(
                    g_pc_guess, g_pc_guess->inventory_items, 0);
                if (g_pc_guess->inventory_items[0] == 0)
                    return;
            }
            else
            {
                coinMove.fromSlot = 0;
                coinMove.toSlot = 1;
                if (g_pc_guess->platinum_cursor > 0)
                {
                    coinMove.fromType = 3;
                    coinMove.amount = g_pc_guess->platinum_cursor;
                    g_pc_guess->platinum += g_pc_guess->platinum_cursor;
                }
                else if (g_pc_guess->gold_cursor > 0)
                {
                    coinMove.fromType = 2;
                    coinMove.amount = g_pc_guess->gold_cursor;
                    g_pc_guess->gold += g_pc_guess->gold_cursor;
                }
                else if (g_pc_guess->silver_cursor > 0)
                {
                    coinMove.fromType = 1;
                    coinMove.amount = g_pc_guess->silver_cursor;
                    g_pc_guess->silver += g_pc_guess->silver_cursor;
                }
                else if (g_pc_guess->copper_cursor > 0)
                {
                    coinMove.fromType = 0;
                    coinMove.amount = g_pc_guess->copper_cursor;
                    g_pc_guess->copper += g_pc_guess->copper_cursor;
                }
                coinMove.toType = coinMove.fromType;
                if (coinMove.amount > 0)
                {
                    PB_SEND(0x212d, &coinMove, 0x14);
                    g_pc_guess->platinum_cursor = 0;
                    g_pc_guess->gold_cursor = 0;
                    g_pc_guess->silver_cursor = 0;
                    g_pc_guess->copper_cursor = 0;
                    *(char*)(PB_DISPLAY + 0x3d) = 0;
                }
            }
        }
        if (button > 0x66)
        {
            if (button > 0x110)
            {
                switch (button)
                {
                case 0x111:
                case 0x124:
                    goto listPageUp;
                case 0x112:
                    EQ_METHOD(0, void, 0x004578F4, (char*), self)();
                    goto tradeWindow;
                case 0x123:
                    goto listPageDown;
                case 0x143:
                    if (PB_TRADE_OPEN == 0)
                        goto merchantWindow;
                    if (PB_TRADE_TARGET == 0 || PB_INT(0x685ec) != 0)
                        goto tradeWindow;
                    PB_INT(0x685ec) = 1;
                    tradeAccept.accept = 1;
                    tradeAccept.targetId = PB_TRADE_TARGET->spawn_id;
                    PB_SEND(0x20da, &tradeAccept, 8);
                    goto tradeWindow;
                case 0x144:
                    if (PB_TRADE_OPEN == 0)
                        goto merchantWindow;
                    if (PB_TRADE_TARGET == 0 || PB_INT(0x685f0) + PB_INT(0x685ec) >= 2)
                        goto tradeWindow;
                    pinstEQ_Main_guess->tradeSessionActive_guess = 0;
                    pairPacket[0] = PB_TRADE_TARGET->spawn_id;
                    pairPacket[1] = g_local_player_guess->spawn_id;
                    PB_SEND(0x20db, pairPacket, 8);
                    goto tradeWindow;
                }
                goto tradeWindow;
            }
            if (button == 0x110)
            {
listPageDown:
                if (PB_MERCHANT_OPEN != 0 || PB_LOOT_OPEN != 0)
                {
                    if (--PB_LIST_PAGE < 0)
                        PB_LIST_PAGE = 0;
                }
                goto tradeWindow;
            }
            if (button < 0xaa)
                goto tradeWindow;
            if (button <= 0xb1)
                goto bankSlot;
            switch (button)
            {
            case 0xb4:
                if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                {
                    if (PB_HOUSE_BANK != 0)
                    {
                        if (*(char*)(PB_DISPLAY + 0x3d) != 1 || g_pc_guess->platinum_cursor <= 0)
                            goto tradeWindow;
                        PB_INT(0x128) += g_pc_guess->platinum_cursor;
                        g_pc_guess->platinum_cursor = 0;
                        *(char*)(PB_DISPLAY + 0x3d) = 0;
                        houseCoins[0] = g_local_player_guess->spawn_id;
                        houseCoins[1] = PB_BANKER_ID;
                        houseCoins[2] = 0;
                        houseCoins[3] = PB_INT(0x128);
                        PB_SEND(0x2111, houseCoins, 0x10);
                        goto tradeWindow;
                    }
                    coinMove.fromSlot = 0;
                    coinMove.toSlot = 2;
                    switch (*(char*)(PB_DISPLAY + 0x3d))
                    {
                    case 1:
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            g_pc_guess->platinum_bank += g_pc_guess->platinum_cursor;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            coinMove.fromType = 3;
                            coinMove.toType = 3;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 2:
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->gold_cursor;
                            g_pc_guess->platinum_bank += g_pc_guess->gold_cursor / 10;
                            g_pc_guess->gold_cursor %= 10;
                            coinMove.fromType = 2;
                            coinMove.toType = 3;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 3:
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->silver_cursor;
                            g_pc_guess->platinum_bank += g_pc_guess->silver_cursor / 100;
                            g_pc_guess->silver_cursor %= 100;
                            coinMove.fromType = 1;
                            coinMove.toType = 3;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 4:
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->copper_cursor;
                            g_pc_guess->platinum_bank += g_pc_guess->copper_cursor / 1000;
                            g_pc_guess->copper_cursor %= 1000;
                            coinMove.fromType = 0;
                            coinMove.toType = 3;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    }
                    goto cursorCoinFlag;
                }
                if (PB_HOUSE_BANK != 0)
                {
                    if (PB_INT(0x128) > 0)
                    {
                        *(int*)(PB_COIN_SLIDER + 0x1dc) = 1;
                        PB_COIN_ACTIVATE((s32*)(self +0x128), 1, 7);
                    }
                }
                else if (g_pc_guess->platinum_bank > 0)
                    PB_COIN_ACTIVATE(&g_pc_guess->platinum_bank, 1, 2);
                goto tradeWindow;
            case 0xb5:
                if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                {
                    if (PB_HOUSE_BANK != 0)
                    {
                        if (*(char*)(PB_DISPLAY + 0x3d) != 2 || g_pc_guess->gold_cursor <= 0)
                            goto tradeWindow;
                        PB_INT(0x12c) += g_pc_guess->gold_cursor;
                        g_pc_guess->gold_cursor = 0;
                        *(char*)(PB_DISPLAY + 0x3d) = 0;
                        houseCoins[0] = g_local_player_guess->spawn_id;
                        houseCoins[1] = PB_BANKER_ID;
                        houseCoins[2] = 1;
                        houseCoins[3] = PB_INT(0x12c);
                        PB_SEND(0x2111, houseCoins, 0x10);
                        goto tradeWindow;
                    }
                    coinMove.fromSlot = 0;
                    coinMove.toSlot = 2;
                    switch (*(char*)(PB_DISPLAY + 0x3d))
                    {
                    case 1:
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            g_pc_guess->gold_bank += g_pc_guess->platinum_cursor * 10;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            coinMove.fromType = 3;
                            coinMove.toType = 2;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 2:
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            g_pc_guess->gold_bank += g_pc_guess->gold_cursor;
                            coinMove.amount = g_pc_guess->gold_cursor;
                            g_pc_guess->gold_cursor = 0;
                            coinMove.fromType = 2;
                            coinMove.toType = 2;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 3:
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->silver_cursor;
                            g_pc_guess->gold_bank += g_pc_guess->silver_cursor / 10;
                            g_pc_guess->silver_cursor %= 10;
                            coinMove.fromType = 1;
                            coinMove.toType = 2;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 4:
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->copper_cursor;
                            g_pc_guess->gold_bank += g_pc_guess->copper_cursor / 100;
                            g_pc_guess->copper_cursor %= 100;
                            coinMove.fromType = 0;
                            coinMove.toType = 2;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    }
                    goto cursorCoinFlag;
                }
                if (PB_HOUSE_BANK != 0)
                {
                    if (PB_INT(0x12c) > 0)
                    {
                        *(int*)(PB_COIN_SLIDER + 0x1dc) = 1;
                        PB_COIN_ACTIVATE((s32*)(self +0x12c), 2, 7);
                    }
                }
                else if (g_pc_guess->gold_bank > 0)
                    PB_COIN_ACTIVATE(&g_pc_guess->gold_bank, 2, 2);
                goto tradeWindow;
            case 0xb6:
                if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                {
                    if (PB_HOUSE_BANK != 0)
                    {
                        if (*(char*)(PB_DISPLAY + 0x3d) != 3 || g_pc_guess->silver_cursor <= 0)
                            goto tradeWindow;
                        PB_INT(0x130) += g_pc_guess->silver_cursor;
                        g_pc_guess->silver_cursor = 0;
                        *(char*)(PB_DISPLAY + 0x3d) = 0;
                        houseCoins[0] = g_local_player_guess->spawn_id;
                        houseCoins[1] = PB_BANKER_ID;
                        houseCoins[2] = 2;
                        houseCoins[3] = PB_INT(0x130);
                        PB_SEND(0x2111, houseCoins, 0x10);
                        goto tradeWindow;
                    }
                    coinMove.fromSlot = 0;
                    coinMove.toSlot = 2;
                    switch (*(char*)(PB_DISPLAY + 0x3d))
                    {
                    case 1:
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            g_pc_guess->silver_bank += g_pc_guess->platinum_cursor * 100;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            coinMove.fromType = 3;
                            coinMove.toType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 2:
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            g_pc_guess->silver_bank += g_pc_guess->gold_cursor * 10;
                            coinMove.amount = g_pc_guess->gold_cursor;
                            g_pc_guess->gold_cursor = 0;
                            coinMove.fromType = 2;
                            coinMove.toType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 3:
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            g_pc_guess->silver_bank += g_pc_guess->silver_cursor;
                            coinMove.amount = g_pc_guess->silver_cursor;
                            g_pc_guess->silver_cursor = 0;
                            coinMove.fromType = 1;
                            coinMove.toType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 4:
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            coinMove.amount = g_pc_guess->copper_cursor;
                            g_pc_guess->silver_bank += g_pc_guess->copper_cursor / 10;
                            g_pc_guess->copper_cursor %= 10;
                            coinMove.fromType = 0;
                            coinMove.toType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    }
                    goto cursorCoinFlag;
                }
                if (PB_HOUSE_BANK != 0)
                {
                    if (PB_INT(0x130) > 0)
                    {
                        *(int*)(PB_COIN_SLIDER + 0x1dc) = 1;
                        PB_COIN_ACTIVATE((s32*)(self +0x130), 3, 7);
                    }
                }
                else if (g_pc_guess->silver_bank > 0)
                    PB_COIN_ACTIVATE(&g_pc_guess->silver_bank, 3, 2);
                goto tradeWindow;
            case 0xb7:
                if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                {
                    if (PB_HOUSE_BANK != 0)
                    {
                        if (*(char*)(PB_DISPLAY + 0x3d) != 4 || g_pc_guess->copper_cursor <= 0)
                            goto tradeWindow;
                        PB_INT(0x134) += g_pc_guess->copper_cursor;
                        g_pc_guess->copper_cursor = 0;
                        *(char*)(PB_DISPLAY + 0x3d) = 0;
                        houseCoins[0] = g_local_player_guess->spawn_id;
                        houseCoins[1] = PB_BANKER_ID;
                        houseCoins[2] = 3;
                        houseCoins[3] = PB_INT(0x134);
                        PB_SEND(0x2111, houseCoins, 0x10);
                        goto tradeWindow;
                    }
                    coinMove.fromSlot = 0;
                    coinMove.toSlot = 2;
                    switch (*(char*)(PB_DISPLAY + 0x3d))
                    {
                    case 1:
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            g_pc_guess->copper_bank += g_pc_guess->platinum_cursor * 1000;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            coinMove.fromType = 3;
                            coinMove.toType = 0;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 2:
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            g_pc_guess->copper_bank += g_pc_guess->gold_cursor * 100;
                            coinMove.amount = g_pc_guess->gold_cursor;
                            g_pc_guess->gold_cursor = 0;
                            coinMove.fromType = 2;
                            coinMove.toType = 0;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 3:
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            g_pc_guess->copper_bank += g_pc_guess->silver_cursor * 10;
                            coinMove.amount = g_pc_guess->silver_cursor;
                            g_pc_guess->silver_cursor = 0;
                            coinMove.toType = 0;
                            coinMove.fromType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 4:
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            g_pc_guess->copper_bank += g_pc_guess->copper_cursor;
                            coinMove.amount = g_pc_guess->copper_cursor;
                            g_pc_guess->copper_cursor = 0;
                            coinMove.fromType = 0;
                            coinMove.toType = 0;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    }
                    goto cursorCoinFlag;
                }
                if (PB_HOUSE_BANK != 0)
                {
                    if (PB_INT(0x134) > 0)
                    {
                        *(int*)(PB_COIN_SLIDER + 0x1dc) = 1;
                        PB_COIN_ACTIVATE((s32*)(self +0x134), 4, 7);
                    }
                }
                else if (g_pc_guess->copper_bank > 0)
                    PB_COIN_ACTIVATE(&g_pc_guess->copper_bank, 4, 2);
                goto tradeWindow;
            case 0xb8:
                if (PB_HOUSE_BANK != 0)
                    goto tradeWindow;
                if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                {
                    coinMove.fromSlot = 0;
                    coinMove.toSlot = 2;
                    switch (*(char*)(PB_DISPLAY + 0x3d))
                    {
                    case 1:
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            g_pc_guess->platinum_bank += g_pc_guess->platinum_cursor;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            coinMove.fromType = 3;
                            coinMove.toType = 3;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 2:
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            g_pc_guess->gold_bank += g_pc_guess->gold_cursor;
                            coinMove.amount = g_pc_guess->gold_cursor;
                            g_pc_guess->gold_cursor = 0;
                            coinMove.fromType = 2;
                            coinMove.toType = 2;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 3:
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            g_pc_guess->silver_bank += g_pc_guess->silver_cursor;
                            coinMove.amount = g_pc_guess->silver_cursor;
                            g_pc_guess->silver_cursor = 0;
                            coinMove.fromType = 1;
                            coinMove.toType = 1;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    case 4:
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            g_pc_guess->copper_bank += g_pc_guess->copper_cursor;
                            coinMove.amount = g_pc_guess->copper_cursor;
                            g_pc_guess->copper_cursor = 0;
                            coinMove.fromType = 0;
                            coinMove.toType = 0;
                            PB_SEND(0x212d, &coinMove, 0x14);
                        }
                        break;
                    }
                    *(char*)(PB_DISPLAY + 0x3d) = 0;
                    if (g_pc_guess->platinum_cursor > 0)
                        *(char*)(PB_DISPLAY + 0x3d) = 1;
                    else if (g_pc_guess->gold_cursor > 0)
                        *(char*)(PB_DISPLAY + 0x3d) = 2;
                    else if (g_pc_guess->silver_cursor > 0)
                        *(char*)(PB_DISPLAY + 0x3d) = 3;
                    else if (g_pc_guess->copper_cursor > 0)
                        *(char*)(PB_DISPLAY + 0x3d) = 4;
                }
                else
                {
                    if (g_pc_guess->inventory_items[0] == 0)
                    {
                        g_cdisplay_guess->itemOnCursor_guess = 0;
                        goto tradeWindow;
                    }
                    freeSlot = -1;
                    for (counter = 0; counter < 8; counter++)
                    {
                        if (freeSlot < 0 && g_pc_guess->bank_items[counter] == 0)
                            freeSlot = counter;
                    }
                    if (freeSlot >= 0)
                        PB_MOVE_ITEM(0, freeSlot + 2000, 0, 1);
                }
                if (g_pc_guess->inventory_items[0] == 0)
                    g_cdisplay_guess->itemOnCursor_guess = 0;
                else
                    g_cdisplay_guess->itemOnCursor_guess = 1;
                goto tradeWindow;
            }
            goto tradeWindow;
        }
        if (button == 0x66)
        {
            if (*(char*)(PB_DISPLAY + 0x3d) == 0)
                goto tradeWindow;
            coinMove.fromSlot = 0;
            coinMove.toSlot = 1;
            goto coinToInventory;
        }
        switch (button)
        {
        case 0x1e:
            PB_VIRTUAL(self, 8);
            PB_MODE = 4;
            if (PB_INT(0x8fb90) == 0)
                PB_INT(0x8fb90) = 1;
            PB_VIRTUAL(self, 4);
            break;
        case 0x1f:
            PB_VIRTUAL(self, 8);
            PB_MODE = 4;
            if (PB_INT(0x8fb90) != 0)
                PB_INT(0x8fb90) = 0;
            PB_VIRTUAL(self, 4);
            break;
        case 0x21:
            if (EQ_GLOBAL(int, 0x006B68AC) == 0)
            {
                if (g_pc_guess->inventory_items[0] != 0)
                {
                    sprintf(PB_CONFIRM + 0x1b9, EQ_ARRAY(char, 0x0055ADD4), g_pc_guess->inventory_items[0]);
                }
                else
                {
                    if (*(char*)(PB_DISPLAY + 0x3d) == 0)
                        break;
                    strcpy(PB_CONFIRM + 0x1b9, EQ_ARRAY(char, 0x0055AD8C));
                }
                pinstEQ_Requestor_guess->requestAction_guess = 5;
                PB_VIRTUAL(PB_CONFIRM, 4);
                pinstEQ_Requestor_guess->timeout_guess = *(int*)(PB_DISPLAY + 0xbc) + 10000;
            }
            else if (g_pc_guess->inventory_items[0] != 0)
            {
                destroyPacket[0] = 0;
                destroyPacket[1] = -1;
                destroyPacket[2] = 0;
                PB_SEND(0x212c, destroyPacket, 0xc);
                g_cdisplay_guess->itemOnCursor_guess = 0;
                operator delete(g_pc_guess->inventory_items[0]);
                g_pc_guess->inventory_items[0] = 0;
            }
            else if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            {
                coinMove.toSlot = -1;
                coinMove.fromSlot = 0;
                if (g_pc_guess->platinum_cursor > 0)
                {
                    coinMove.fromType = 3;
                    coinMove.amount = g_pc_guess->platinum_cursor;
                }
                else if (g_pc_guess->gold_cursor > 0)
                {
                    coinMove.fromType = 2;
                    coinMove.amount = g_pc_guess->gold_cursor;
                }
                else if (g_pc_guess->silver_cursor > 0)
                {
                    coinMove.fromType = 1;
                    coinMove.amount = g_pc_guess->silver_cursor;
                }
                else if (g_pc_guess->copper_cursor > 0)
                {
                    coinMove.fromType = 0;
                    coinMove.amount = g_pc_guess->copper_cursor;
                }
                if (coinMove.amount > 0)
                {
                    coinMove.toType = coinMove.fromType;
                    PB_SEND(0x212d, &coinMove, 0x14);
                    *(char*)(PB_DISPLAY + 0x3d) = 0;
                    g_pc_guess->platinum_cursor = 0;
                    g_pc_guess->gold_cursor = 0;
                    g_pc_guess->silver_cursor = 0;
                    g_pc_guess->copper_cursor = 0;
                }
            }
            break;
        case 0x22:
            if (PB_CHAR(0x100c) != 0)
            {
                if (PB_VIDEO_RESTORE == 2)
                {
                    PB_VIDEO_MODE = 2;
                    PB_CHANGE_VIDEO_MODE();
                    PB_PREV_MODE = 0;
                }
                PB_VIRTUAL(self, 8);
                PB_MODE = PB_PREV_MODE;
                goto activateReturn;
            }
            if (PB_BANK_OPEN != 0)
            {
                if (PB_HOUSE_BANK != 0)
                {
                    if (PB_SOUND_OFF == 0 && PB_MUSIC != 0)
                        EQ_METHOD(1, void, 0x004E4EE6, (void*, int), PB_MUSIC)(0x15);
                    EQ_METHOD(0, void, 0x0047D6E2, (char*), self)();
                    return;
                }
                EQ_METHOD(1, void, 0x004C53D8, (void*, int), PB_EVERQUEST)(1);
                return;
            }
            if (PB_MERCHANT_OPEN != 0 && PB_MERCHANT != 0)
            {
                EQ_METHOD(0, void, 0x00464AB6, (char*), self)();
                return;
            }
            if (PB_LOOT_OPEN != 0 && PB_CORPSE != 0)
            {
                EQ_METHOD(0, void, 0x00464C8D, (char*), self)();
                return;
            }
            if (PB_TRADE_OPEN == 0)
                goto closeToPrevious;
            PB_DSP_CHAT(EQ_ARRAY(char, 0x0055AD64), 0xf);
            return;
        case 0x23:
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            {
                coinMove.fromSlot = 0;
                coinMove.toSlot = 1;
                if (PB_BANK_OPEN == 0 || PB_HOUSE_BANK != 0)
                    goto coinToInventory;
                switch (*(char*)(PB_DISPLAY + 0x3d))
                {
                case 1:
                    goto platinumToInventory;
                case 2:
                    if (g_pc_guess->gold_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->gold_cursor;
                        g_pc_guess->platinum += g_pc_guess->gold_cursor / 10;
                        g_pc_guess->gold_cursor %= 10;
                        coinMove.fromType = 2;
                        coinMove.toType = 3;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 3:
                    if (g_pc_guess->silver_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->silver_cursor;
                        g_pc_guess->platinum += g_pc_guess->silver_cursor / 100;
                        g_pc_guess->silver_cursor %= 100;
                        coinMove.fromType = 1;
                        coinMove.toType = 3;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 4:
                    if (g_pc_guess->copper_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->copper_cursor;
                        g_pc_guess->platinum += g_pc_guess->copper_cursor / 1000;
                        g_pc_guess->copper_cursor %= 1000;
                        coinMove.fromType = 0;
                        coinMove.toType = 3;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                }
                goto cursorCoinFlag;
            }
            if (g_pc_guess->platinum > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
                PB_COIN_ACTIVATE(&g_pc_guess->platinum, 1, 1);
            break;
        case 0x24:
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            {
                coinMove.fromSlot = 0;
                coinMove.toSlot = 1;
                if (PB_BANK_OPEN == 0 || PB_HOUSE_BANK != 0)
                    goto coinToInventory;
                switch (*(char*)(PB_DISPLAY + 0x3d))
                {
                case 1:
                    if (g_pc_guess->platinum_cursor > 0)
                    {
                        g_pc_guess->gold += g_pc_guess->platinum_cursor * 10;
                        coinMove.amount = g_pc_guess->platinum_cursor;
                        g_pc_guess->platinum_cursor = 0;
                        coinMove.fromType = 3;
                        coinMove.toType = 2;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 2:
                    goto goldToInventory;
                case 3:
                    if (g_pc_guess->silver_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->silver_cursor;
                        g_pc_guess->gold += g_pc_guess->silver_cursor / 10;
                        g_pc_guess->silver_cursor %= 10;
                        coinMove.fromType = 1;
                        coinMove.toType = 2;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 4:
                    if (g_pc_guess->copper_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->copper_cursor;
                        g_pc_guess->gold += g_pc_guess->copper_cursor / 100;
                        g_pc_guess->copper_cursor %= 100;
                        coinMove.fromType = 0;
                        coinMove.toType = 2;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                }
                goto cursorCoinFlag;
            }
            if (g_pc_guess->gold > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
                PB_COIN_ACTIVATE(&g_pc_guess->gold, 2, 1);
            break;
        case 0x25:
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            {
                coinMove.fromSlot = 0;
                coinMove.toSlot = 1;
                if (PB_BANK_OPEN == 0 || PB_HOUSE_BANK != 0)
                    goto coinToInventory;
                switch (*(char*)(PB_DISPLAY + 0x3d))
                {
                case 1:
                    if (g_pc_guess->platinum_cursor > 0)
                    {
                        g_pc_guess->silver += g_pc_guess->platinum_cursor * 100;
                        coinMove.amount = g_pc_guess->platinum_cursor;
                        g_pc_guess->platinum_cursor = 0;
                        coinMove.fromType = 3;
                        coinMove.toType = 1;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 2:
                    if (g_pc_guess->gold_cursor > 0)
                    {
                        g_pc_guess->silver += g_pc_guess->gold_cursor * 10;
                        coinMove.amount = g_pc_guess->gold_cursor;
                        g_pc_guess->gold_cursor = 0;
                        coinMove.fromType = 2;
                        coinMove.toType = 1;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 3:
                    goto silverToInventory;
                case 4:
                    if (g_pc_guess->copper_cursor > 0)
                    {
                        coinMove.amount = g_pc_guess->copper_cursor;
                        g_pc_guess->silver += g_pc_guess->copper_cursor / 10;
                        g_pc_guess->copper_cursor %= 10;
                        coinMove.fromType = 0;
                        coinMove.toType = 1;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                }
                goto cursorCoinFlag;
            }
            if (g_pc_guess->silver > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
                PB_COIN_ACTIVATE(&g_pc_guess->silver, 3, 1);
            break;
        case 0x26:
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            {
                coinMove.fromSlot = 0;
                coinMove.toSlot = 1;
                if (PB_BANK_OPEN == 0 || PB_HOUSE_BANK != 0)
                    goto coinToInventory;
                switch (*(char*)(PB_DISPLAY + 0x3d))
                {
                case 1:
                    if (g_pc_guess->platinum_cursor > 0)
                    {
                        g_pc_guess->copper += g_pc_guess->platinum_cursor * 1000;
                        coinMove.amount = g_pc_guess->platinum_cursor;
                        g_pc_guess->platinum_cursor = 0;
                        coinMove.fromType = 3;
                        coinMove.toType = 0;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 2:
                    if (g_pc_guess->gold_cursor > 0)
                    {
                        g_pc_guess->copper += g_pc_guess->gold_cursor * 100;
                        coinMove.amount = g_pc_guess->gold_cursor;
                        g_pc_guess->gold_cursor = 0;
                        coinMove.fromType = 2;
                        coinMove.toType = 0;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                case 3:
                    if (g_pc_guess->silver_cursor > 0)
                    {
                        g_pc_guess->copper += g_pc_guess->silver_cursor * 10;
                        coinMove.amount = g_pc_guess->silver_cursor;
                        g_pc_guess->silver_cursor = 0;
                        coinMove.toType = 0;
                        coinMove.fromType = 1;
                        PB_SEND(0x212d, &coinMove, 0x14);
                    }
                    break;
                default:
                    goto copperToInventory;
                }
                goto cursorCoinFlag;
            }
            if (g_pc_guess->copper > 0 && pinstEQ_Main_guess->merchantOpen_guess == 0)
                PB_COIN_ACTIVATE(&g_pc_guess->copper, 4, 1);
            break;
        case 0x28:
            if (PB_MERCHANT_OPEN == 0 && PB_LOOT_OPEN == 0 && PB_BANK_OPEN == 0 && PB_INT(0x8fb90) != 0 &&
                PB_SHORT(0x685c8) > 0)
            {
                PB_SHORT(0x685c8)--;
                EQ_METHOD(1, void, 0x00463CBE, (char*, int), self)(-1);
            }
            break;
        case 0x29:
            if (PB_MERCHANT_OPEN == 0 && PB_LOOT_OPEN == 0 && PB_BANK_OPEN == 0 && PB_INT(0x8fb90) != 0 &&
                PB_SHORT(0x685c8) < PB_INT(0x685bc) - 6)
            {
                PB_SHORT(0x685c8)++;
                EQ_METHOD(1, void, 0x00463CBE, (char*, int), self)(1);
            }
            break;
        }
        goto tradeWindow;
coinToInventory:
        switch (*(char*)(PB_DISPLAY + 0x3d))
        {
        case 1:
platinumToInventory:
            if (g_pc_guess->platinum_cursor > 0)
            {
                g_pc_guess->platinum += g_pc_guess->platinum_cursor;
                coinMove.amount = g_pc_guess->platinum_cursor;
                g_pc_guess->platinum_cursor = 0;
                coinMove.fromType = 3;
                coinMove.toType = 3;
                PB_SEND(0x212d, &coinMove, 0x14);
            }
            break;
        case 2:
goldToInventory:
            if (g_pc_guess->gold_cursor > 0)
            {
                g_pc_guess->gold += g_pc_guess->gold_cursor;
                coinMove.amount = g_pc_guess->gold_cursor;
                g_pc_guess->gold_cursor = 0;
                coinMove.fromType = 2;
                coinMove.toType = 2;
                PB_SEND(0x212d, &coinMove, 0x14);
            }
            break;
        case 3:
silverToInventory:
            if (g_pc_guess->silver_cursor > 0)
            {
                g_pc_guess->silver += g_pc_guess->silver_cursor;
                coinMove.amount = g_pc_guess->silver_cursor;
                g_pc_guess->silver_cursor = 0;
                coinMove.fromType = 1;
                coinMove.toType = 1;
                PB_SEND(0x212d, &coinMove, 0x14);
            }
            break;
        default:
copperToInventory:
            if (*(char*)(PB_DISPLAY + 0x3d) == 4 && g_pc_guess->copper_cursor > 0)
            {
                g_pc_guess->copper += g_pc_guess->copper_cursor;
                coinMove.amount = g_pc_guess->copper_cursor;
                g_pc_guess->copper_cursor = 0;
                coinMove.fromType = 0;
                coinMove.toType = 0;
                PB_SEND(0x212d, &coinMove, 0x14);
            }
            break;
        }
cursorCoinFlag:
        *(char*)(PB_DISPLAY + 0x3d) = 0;
        if (g_pc_guess->platinum_cursor > 0)
            *(char*)(PB_DISPLAY + 0x3d) = 1;
        else if (g_pc_guess->gold_cursor > 0)
            *(char*)(PB_DISPLAY + 0x3d) = 2;
        else if (g_pc_guess->silver_cursor > 0)
            *(char*)(PB_DISPLAY + 0x3d) = 3;
        else if (g_pc_guess->copper_cursor > 0)
            *(char*)(PB_DISPLAY + 0x3d) = 4;
        goto tradeWindow;
listPageUp:
        if (PB_MERCHANT_OPEN != 0 || PB_LOOT_OPEN != 0)
        {
            PB_LIST_PAGE++;
            if (PB_MERCHANT_OPEN != 0)
            {
                if (PB_LIST_PAGE > 9)
                    PB_LIST_PAGE = 9;
            }
            else if (PB_LOOT_OPEN != 0 && PB_LIST_PAGE > 7)
                PB_LIST_PAGE = 7;
        }
        goto tradeWindow;
bankSlot:
        if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            goto tradeWindow;
        button -= 0xaa;
        if (PB_HOUSE_BANK != 0)
        {
            if (g_pc_guess->inventory_items[0] == 0)
            {
                slot = (EQ_Item**)(self + button * 4 + 0x108);
                if (*slot == 0)
                    return;
                if (PB_COMBINE(*slot) != 0)
                {
                    *(int*)(PB_QUANTITY_SLIDER + 0x1dc) = 1;
                    *(short*)(PB_QUANTITY_SLIDER + 0x1e0) = button;
                    PB_QUANTITY_ACTIVATE(slot, button);
                    return;
                }
                bankMsg.spawnId = g_local_player_guess->spawn_id;
                bankMsg.bankerId = PB_BANKER_ID;
                bankMsg.itemNr = -1;
                bankMsg.slot = button;
                bankMsg.cost = 0;
                PB_SEND(0x2112, &bankMsg, 0x1c);
                g_pc_guess->inventory_items[0] = *slot;
                *slot = 0;
                item = g_pc_guess->inventory_items[0];
                if (item->type == 1 && EQ_METHOD(0, char, 0x0043407E, (EQ_Item*), item)() != 0)
                {
                    if (item == pinstEQ_Bag1_guess->item_guess)
                    {
                        PB_VIRTUAL(PB_BAG1, 8);
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (item == pinstEQ_Bag2_guess->item_guess)
                    {
                        PB_VIRTUAL(PB_BAG2, 8);
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                    else
                        EQTRACE_guess(EQ_ARRAY(char, 0x0055AD30));
                }
                goto cursorItemFlag;
            }
            if (EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(g_pc_guess->inventory_items[0], 1) == 0)
                return;
            slot = (EQ_Item**)(self + button * 4 + 0x108);
            if (EQ_CDECL(int, 0x004A1C7C, (EQ_Item**, EQ_Item**, int, short))(
                    g_pc_guess->inventory_items, slot, 0, button + 8000) == 0)
            {
                bankMsg.spawnId = g_local_player_guess->spawn_id;
                bankMsg.bankerId = PB_BANKER_ID;
                bankMsg.itemNr = g_pc_guess->inventory_items[0]->item_nr;
                bankMsg.slot = button;
                if (*slot == 0)
                {
                    bankMsg.cost = g_pc_guess->inventory_items[0]->cost;
                    bankMsg.itemType = g_pc_guess->inventory_items[0]->type;
                    if (bankMsg.itemType == 0)
                        memcpy(bankMsg.data, (char*)g_pc_guess->inventory_items[0] + 0xd8, 10);
                    PB_SEND(0x2112, &bankMsg, 0x1c);
                    if (bankMsg.itemType == 1)
                    {
                        slotOffset = (button * 5 + 0xf) * 2;
                        for (counter = 0; counter < 8; counter++)
                        {
                            item = ((EQ_Item**)((char*)g_pc_guess->inventory_items[0] + 0xac))[counter];
                            bankMsg.cost = 0;
                            bankMsg.slot = slotOffset + counter;
                            if (item == 0)
                                bankMsg.itemNr = -1;
                            else
                            {
                                bankMsg.itemNr = item->item_nr;
                                bankMsg.itemType = item->type;
                                bankMsg.cost = item->cost;
                                if (bankMsg.itemType == 0)
                                    memcpy(bankMsg.data, (char*)item + 0xd8, 10);
                            }
                            PB_SEND(0x2112, &bankMsg, 0x1c);
                        }
                    }
                    *slot = g_pc_guess->inventory_items[0];
                    g_pc_guess->inventory_items[0] = 0;
                }
                else
                {
                    bankMsg.itemType = g_pc_guess->inventory_items[0]->type;
                    bankMsg.cost = g_pc_guess->inventory_items[0]->cost;
                    if (bankMsg.itemType == 0)
                        memcpy(bankMsg.data, (char*)g_pc_guess->inventory_items[0] + 0xd8, 10);
                    PB_SEND(0x2112, &bankMsg, 0x1c);
                    if (bankMsg.itemType == 1)
                    {
                        slotOffset = (button * 5 + 0xf) * 2;
                        for (counter = 0; counter < 8; counter++)
                        {
                            item = ((EQ_Item**)((char*)g_pc_guess->inventory_items[0] + 0xac))[counter];
                            bankMsg.cost = 0;
                            bankMsg.slot = slotOffset + counter;
                            if (item == 0)
                                bankMsg.itemNr = -1;
                            else
                            {
                                bankMsg.itemNr = item->item_nr;
                                bankMsg.itemType = item->type;
                                bankMsg.cost = item->cost;
                                if (bankMsg.itemType == 0)
                                    memcpy(bankMsg.data, (char*)item + 0xd8, 10);
                            }
                            PB_SEND(0x2112, &bankMsg, 0x1c);
                        }
                    }
                    item = g_pc_guess->inventory_items[0];
                    g_pc_guess->inventory_items[0] = *slot;
                    *slot = item;
                }
            }
            else
            {
                bankMsg.spawnId = g_local_player_guess->spawn_id;
                bankMsg.bankerId = PB_BANKER_ID;
                bankMsg.slot = button;
                bankMsg.cost = 0;
                if (*slot == 0)
                    bankMsg.itemNr = -1;
                else
                {
                    bankMsg.itemNr = (*slot)->item_nr;
                    bankMsg.itemType = (*slot)->type;
                    bankMsg.cost = (*slot)->cost;
                    if (bankMsg.itemType == 0)
                        memcpy(bankMsg.data, (char*)*slot + 0xd8, 10);
                }
                PB_SEND(0x2112, &bankMsg, 0x1c);
            }
            goto cursorItemFlag;
        }
        if (g_pc_guess->inventory_items[0] == 0)
        {
            index = button;
            if (g_pc_guess->bank_items[index] == 0)
                return;
            if (PB_COMBINE(g_pc_guess->bank_items[index]) != 0)
            {
                PB_QUANTITY_ACTIVATE(&g_pc_guess->bank_items[index], index + 2000);
                return;
            }
            item = g_pc_guess->bank_items[index];
            if (item->type == 1)
            {
                if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG1, 8);
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG2, 8);
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            PB_MOVE_ITEM(index + 2000, 0, 1, 1);
            goto cursorItemFlag;
        }
        item = g_pc_guess->bank_items[button];
        if (item != 0 && item->type == 1)
        {
            if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
            {
                PB_BAG_OPEN(item) = 0;
                PB_VIRTUAL(PB_BAG1, 8);
                pinstEQ_Bag1_guess->item_guess = 0;
            }
            else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
            {
                PB_BAG_OPEN(item) = 0;
                PB_VIRTUAL(PB_BAG2, 8);
                pinstEQ_Bag2_guess->item_guess = 0;
            }
        }
        PB_MOVE_ITEM(0, button + 2000, 1, 1);
cursorItemFlag:
        if (g_pc_guess->inventory_items[0] == 0)
            goto clearCursorItem;
        g_cdisplay_guess->itemOnCursor_guess = 1;
        return;
tradeWindow:
        if (PB_TRADE_OPEN != 0 && PB_TRADE_TARGET != 0)
        {
            if (mouseButton == 1)
            {
                if (PB_INSPECT != 0)
                    goto merchantWindow;
                if (button >= 0x12c && button <= 0x133)
                {
                    item = *(EQ_Item**)(self + button * 4 + 0x6c03c);
                    if (item != 0 && item->type == 1)
                    {
                        if (PB_BAG_OPEN(item) == 1)
                        {
                            if (PB_IS_ACTIVE(PB_BAG1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG1, 8);
                                pinstEQ_Bag1_guess->item_guess = 0;
                            }
                            else if (PB_IS_ACTIVE(PB_BAG2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG2, 8);
                                pinstEQ_Bag2_guess->item_guess = 0;
                            }
                        }
                        else if (PB_IS_ACTIVE(PB_BAG1) == 0)
                        {
                            PB_BAG_OPEN(item) = 1;
                            pinstEQ_Bag1_guess->item_guess = item;
                            PB_VIRTUAL_ARG(PB_BAG1, 0x10, -1);
                            pinstEQ_Bag1_guess->slot_guess = button - 300;
                            pinstEQ_Bag1_guess->theirTradeContainer_guess = 1;
                        }
                        else if (PB_IS_ACTIVE(PB_BAG2) == 0)
                        {
                            PB_BAG_OPEN(item) = 1;
                            pinstEQ_Bag2_guess->item_guess = item;
                            PB_VIRTUAL_ARG(PB_BAG2, 0x10, -1);
                            pinstEQ_Bag2_guess->slot_guess = button - 300;
                            pinstEQ_Bag2_guess->theirTradeContainer_guess = 1;
                        }
                        if (PB_INSPECT != 0)
                            goto merchantWindow;
                    }
                }
                if (button >= 0x136 && button <= 0x13d)
                {
                    item = *(EQ_Item**)(self + button * 4 + 0x6c034);
                    if (item != 0 && item->type == 1)
                    {
                        if (PB_BAG_OPEN(item) == 1)
                        {
                            if (PB_IS_ACTIVE(PB_BAG1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG1, 8);
                                pinstEQ_Bag1_guess->item_guess = 0;
                            }
                            else if (PB_IS_ACTIVE(PB_BAG2) != 0 && pinstEQ_Bag2_guess->item_guess == item)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG2, 8);
                                pinstEQ_Bag2_guess->item_guess = 0;
                            }
                        }
                        else if (PB_IS_ACTIVE(PB_BAG1) == 0)
                        {
                            PB_BAG_OPEN(item) = 1;
                            pinstEQ_Bag1_guess->item_guess = item;
                            PB_VIRTUAL_ARG(PB_BAG1, 0x10, -1);
                            pinstEQ_Bag1_guess->slot_guess = button - 0x136;
                            pinstEQ_Bag1_guess->myTradeContainer_guess = 1;
                        }
                        else if (PB_IS_ACTIVE(PB_BAG2) == 0)
                        {
                            PB_BAG_OPEN(item) = 1;
                            pinstEQ_Bag2_guess->item_guess = item;
                            PB_VIRTUAL_ARG(PB_BAG2, 0x10, -1);
                            pinstEQ_Bag2_guess->slot_guess = button - 0x136;
                            pinstEQ_Bag2_guess->myTradeContainer_guess = 1;
                        }
                    }
                }
            }
            else if (mouseButton == 0)
            {
                if (button >= 0x13e && button <= 0x142)
                {
                    PB_INT(0x685ec) = 0;
                    PB_INT(0x685f0) = 0;
                    if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                    {
                        coinMove.fromSlot = 0;
                        coinMove.toSlot = 3;
                        if (g_pc_guess->platinum_cursor > 0)
                        {
                            coinMove.fromType = 3;
                            coinMove.toType = 3;
                            coinMove.amount = g_pc_guess->platinum_cursor;
                            PB_SEND(0x212d, &coinMove, 0x14);
                            PB_INT(0x6c52c) += g_pc_guess->platinum_cursor;
                            g_pc_guess->platinum_cursor = 0;
                            *(char*)(PB_DISPLAY + 0x3d) = 0;
                        }
                        if (g_pc_guess->gold_cursor > 0)
                        {
                            coinMove.fromType = 2;
                            coinMove.toType = 2;
                            coinMove.amount = g_pc_guess->gold_cursor;
                            PB_SEND(0x212d, &coinMove, 0x14);
                            PB_INT(0x6c530) += g_pc_guess->gold_cursor;
                            g_pc_guess->gold_cursor = 0;
                            *(char*)(PB_DISPLAY + 0x3d) = 0;
                        }
                        if (g_pc_guess->silver_cursor > 0)
                        {
                            coinMove.fromType = 1;
                            coinMove.toType = 1;
                            coinMove.amount = g_pc_guess->silver_cursor;
                            PB_SEND(0x212d, &coinMove, 0x14);
                            PB_INT(0x6c534) += g_pc_guess->silver_cursor;
                            g_pc_guess->silver_cursor = 0;
                            *(char*)(PB_DISPLAY + 0x3d) = 0;
                        }
                        if (g_pc_guess->copper_cursor > 0)
                        {
                            coinMove.fromType = 0;
                            coinMove.toType = 0;
                            coinMove.amount = g_pc_guess->copper_cursor;
                            PB_SEND(0x212d, &coinMove, 0x14);
                            PB_INT(0x6c538) += g_pc_guess->copper_cursor;
                            g_pc_guess->copper_cursor = 0;
                            *(char*)(PB_DISPLAY + 0x3d) = 0;
                        }
                    }
                    else if (g_cdisplay_guess->itemOnCursor_guess == 0 && button >= 0x13e && button <= 0x141)
                        PB_DSP_CHAT(EQ_ARRAY(char, 0x0055AD64), 0xf);
                }
                if (button >= 0x136 && button <= 0x13d && *(char*)(PB_DISPLAY + 0x3d) == 0)
                {
                    PB_INT(0x685ec) = 0;
                    PB_INT(0x685f0) = 0;
                    if (g_pc_guess->inventory_items[0] == 0)
                    {
                        if (*(int*)(self + button * 4 + 0x6c034) != 0)
                            goto tradeBusy;
                        PB_INT(0x685ec) = 0;
                        PB_INT(0x685f0) = 0;
                        PB_MOVE_ITEM(button + 0xa82, 0, 1, 1);
                        if (g_pc_guess->inventory_items[0] == 0)
                            g_cdisplay_guess->itemOnCursor_guess = 0;
                        else
                            g_cdisplay_guess->itemOnCursor_guess = 1;
                    }
                    else if ((PB_TRADE_TARGET != 0 && PB_TRADE_TARGET->type == 1) ||
                        EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(g_pc_guess->inventory_items[0], 1) != 0)
                    {
                        if (*(int*)(self + button * 4 + 0x6c034) != 0)
                        {
tradeBusy:
                            PB_DSP_CHAT(EQ_ARRAY(char, 0x0055AD64), 0xf);
                        }
                        else
                        {
                            PB_INT(0x685ec) = 0;
                            PB_INT(0x685f0) = 0;
                            PB_MOVE_ITEM(0, button + 0xa82, 1, 1);
                            if (g_pc_guess->inventory_items[0] == 0)
                                g_cdisplay_guess->itemOnCursor_guess = 0;
                            else
                                g_cdisplay_guess->itemOnCursor_guess = 1;
                        }
                    }
                }
            }
        }
merchantWindow:
        if (PB_MERCHANT_OPEN != 0)
        {
            if (mouseButton != 0)
                goto inventoryContextClick;
            if (button <= 0x1d)
            {
                if (PB_HOTKEY_EDIT != 0)
                    goto inventoryDragCheck;
                slotOffset = button * 4 + 0xe0;
                if (*(EQ_Item**)((char*)g_pc_guess + slotOffset) == 0)
                    goto inventoryDragCheck;
                *(EQ_Item***)(self + 0x6c658) = (EQ_Item**)((char*)g_pc_guess + slotOffset);
                PB_SHORT(0x6c65c) = button + 100;
                strcpy(nameCopy, (*(EQ_Item**)((char*)g_pc_guess + slotOffset))->name);
                item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
                if (item->type == 0)
                {
                    allowed = PB_COMBINE(item);
                    sprintf(message, allowed != 0 ? EQ_ARRAY(char, 0x0053F134) : EQ_ARRAY(char, 0x0053EE14),
                        PB_TRIM_NAME(PB_MERCHANT->name),
                        EQ_METHOD(2, char*, 0x004385D5, (EQ_Item*, float, int), *(EQ_Item**)((char*)g_pc_guess + slotOffset))(
                            pinstEQ_Main_guess->merchantRate_guess, 1),
                        nameCopy);
                    PB_DSP_CHAT(message, 0x113);
                }
                else if (item->type == 2)
                {
                    sprintf(message, EQ_ARRAY(char, 0x0053EE14), PB_TRIM_NAME(PB_MERCHANT->name),
                        EQ_METHOD(2, char*, 0x004385D5, (EQ_Item*, float, int), item)(pinstEQ_Main_guess->merchantRate_guess, 1),
                        nameCopy);
                    PB_DSP_CHAT(message, 0x113);
                }
                else if (item->type == 1)
                {
                    for (counter = 0; counter < *(u8*)((char*)item + 0xd5); counter++)
                    {
                        slot = (EQ_Item**)((char*)item + counter * 4 + 0xac);
                        if (*slot != 0)
                        {
                            strcpy(nameCopy, (*slot)->name);
                            allowed = PB_COMBINE(*slot);
                            sprintf(message, allowed != 0 ? EQ_ARRAY(char, 0x0053F134) : EQ_ARRAY(char, 0x0053EE14),
                                PB_TRIM_NAME(PB_MERCHANT->name),
                                EQ_METHOD(2, char*, 0x004385D5, (EQ_Item*, float, int), *slot)(pinstEQ_Main_guess->merchantRate_guess, 1),
                                nameCopy);
                            PB_DSP_CHAT(message, 0x113);
                        }
                    }
                    item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
                    sprintf(message, EQ_ARRAY(char, 0x0053EE14), PB_TRIM_NAME(PB_MERCHANT->name),
                        EQ_METHOD(2, char*, 0x004385D5, (EQ_Item*, float, int), item)(pinstEQ_Main_guess->merchantRate_guess, 1),
                        item->name);
                    PB_DSP_CHAT(message, 0x113);
                    if (mouseButton != 0)
                        goto inventoryContextClick;
                }
            }
            goto inventoryDragCheck;
        }
        if (mouseButton != 0)
            goto inventoryContextClick;
inventoryDragCheck:
        if (button <= 0x1d && PB_HOTKEY_EDIT != 0 && PB_CHAR(0x100c) == 0)
        {
            PB_DRAG_TYPE = 5;
            goto setDragValue;
        }
        goto inventoryEquip;
inventoryContextClick:
        if (mouseButton == 1 && button <= 0x1d && PB_CHAR(0x100c) == 0 && PB_LOOT_OPEN == 0 &&
            PB_MERCHANT_OPEN == 0)
        {
            item = g_pc_guess->inventory_items[button];
            if (item != 0 && item->type == 0 && item != 0)
                EQ_METHOD(3, void, 0x0046AB71, (char*, EQ_Item*, EQ_Item*, short), self)(item, item, button);
        }
inventoryEquip:
        if (PB_MERCHANT_OPEN != 0)
            goto merchantBuyPrice;
        if (mouseButton == 0 && button <= 0x1d && PB_CHAR(0x100c) == 0)
        {
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                return;
            if (g_pc_guess->inventory_items[0] == 0)
            {
                index = button;
                slotOffset = index * 4 + 0xe0;
                if (*(EQ_Item**)((char*)g_pc_guess + slotOffset) == 0)
                    goto lootWindow;
                if (PB_COMBINE(*(EQ_Item**)((char*)g_pc_guess + slotOffset)) != 0)
                {
                    PB_QUANTITY_ACTIVATE((EQ_Item**)((char*)g_pc_guess + slotOffset), index);
                    goto lootWindow;
                }
                item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
                if (item->type == 1)
                {
                    if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                    {
                        PB_BAG_OPEN(item) = 0;
                        PB_VIRTUAL(PB_BAG1, 8);
                        pinstEQ_Bag1_guess->item_guess = 0;
                    }
                    else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                    {
                        PB_BAG_OPEN(item) = 0;
                        PB_VIRTUAL(PB_BAG2, 8);
                        pinstEQ_Bag2_guess->item_guess = 0;
                    }
                }
                EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, short, EQ_Item*, EQ_Item*))(
                    g_pc_guess, button, g_pc_guess->inventory_items[0], *(EQ_Item**)((char*)g_pc_guess + slotOffset));
                if (PB_SOUND != 0)
                    PB_WAVE_PLAY();
                PB_MOVE_ITEM(index, 0, 1, 1);
                EQ_METHOD(2, void, 0x0040B521, (char*, EQPlayer*, short), PB_DISPLAY)(g_local_player_guess, button);
                PB_UPDATE_LIGHT();
            }
            else
            {
                item = g_pc_guess->inventory_items[0];
                if (EQ_CDECL(int, 0x004A2180, (CharacterProfile*, u32, short, EQ_Item*))(
                        g_pc_guess, item->equipable_slots, button, item) != 0)
                {
                    if (button < 0x16 &&
                        EQ_METHOD(1, int, 0x0041BA72, (CharacterProfile*, EQ_Item*), g_pc_guess)(
                            g_pc_guess->inventory_items[0]) == 0)
                    {
                        PB_DSP_CHAT(EQ_ARRAY(char, 0x0055ACF8), 0xd);
                    }
                    else
                    {
                        index = button;
                        slotOffset = index * 4 + 0xe0;
                        item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
                        if (item != 0 && item->type == 1)
                        {
                            if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG1, 8);
                                pinstEQ_Bag1_guess->item_guess = 0;
                            }
                            else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                            {
                                PB_BAG_OPEN(item) = 0;
                                PB_VIRTUAL(PB_BAG2, 8);
                                pinstEQ_Bag2_guess->item_guess = 0;
                            }
                        }
                        EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, short, EQ_Item*, EQ_Item*))(
                            g_pc_guess, button, g_pc_guess->inventory_items[0], *(EQ_Item**)((char*)g_pc_guess + slotOffset));
                        if (PB_SOUND != 0)
                            PB_WAVE_PLAY();
                        PB_MOVE_ITEM(0, index, 1, 1);
                        EQ_METHOD(2, void, 0x0040B521, (char*, EQPlayer*, short), PB_DISPLAY)(g_local_player_guess, button);
                        PB_UPDATE_LIGHT();
                    }
                }
            }
            if (g_pc_guess->inventory_items[0] == 0)
                g_cdisplay_guess->itemOnCursor_guess = 0;
            else
                g_cdisplay_guess->itemOnCursor_guess = 1;
        }
lootWindow:
        if (PB_MERCHANT_OPEN == 0)
            goto lootSlot;
merchantBuyPrice:
        if (PB_HOTKEY_EDIT == 0 && mouseButton == 0 && button >= 0x104 && button <= 0x10f)
        {
            slot = (EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4);
            if (*slot != 0)
            {
                *(EQ_Item***)(self + 0x6c658) = slot;
                PB_SHORT(0x6c65c) = button + PB_LIST_PAGE * 2 - 0x104;
                strcpy(nameCopy, (*(EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4))->name);
                item = *(EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4);
                if (item->type == 0)
                {
                    if (((EQ_Equipment*)item)->item_type == 0x27)
                    {
                        text = EQ_METHOD(2, char*, 0x00438701, (EQ_Item*, float, int), item)(pinstEQ_Main_guess->merchantRate_guess, 1);
                        sprintf(message, EQ_ARRAY(char, 0x0055ACAC), PB_TRIM_NAME(PB_MERCHANT->name), text, nameCopy,
                            EQ_METHOD(0, char*, 0x0043882D, (EQ_Item*),
                                *(EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4))());
                    }
                    else
                    {
                        allowed = PB_COMBINE(item);
                        text = EQ_METHOD(2, char*, 0x00438701, (EQ_Item*, float, int),
                            *(EQ_Item**)(self + (button + PB_LIST_PAGE * 2 + 0x1b074) * 4))(pinstEQ_Main_guess->merchantRate_guess, 1);
                        sprintf(message, allowed != 0 ? EQ_ARRAY(char, 0x0055AC84) : EQ_ARRAY(char, 0x0055AC58),
                            PB_TRIM_NAME(PB_MERCHANT->name), text, nameCopy);
                    }
                }
                else
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055AC58), PB_TRIM_NAME(PB_MERCHANT->name),
                        EQ_METHOD(2, char*, 0x00438701, (EQ_Item*, float, int), item)(pinstEQ_Main_guess->merchantRate_guess, 1),
                        nameCopy);
                }
                PB_DSP_CHAT(message, 0x113);
            }
        }
lootSlot:
        if (PB_LOOT_OPEN == 0)
            return;
        if (PB_INT(0xa4) == 0)
            return;
        if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            return;
        if (g_pc_guess->inventory_items[0] != 0)
            return;
        if (button < 0x113 || button > 0x122)
            return;
        if (mouseButton != 0 && mouseButton != 1)
            return;
        if (PB_INSPECT != 0)
            return;
        index = (short)(button - 0x113 + PB_LIST_PAGE * 2);
        slot = (EQ_Item**)(self + index * 4 + 0x6c5e0);
        if (*slot == 0)
            return;
        allowed = 1;
        if (EQ_GLOBAL(int, 0x006B6F04) > 2 || EQ_GLOBAL(int, 0x006B6F04) < 0)
            EQ_GLOBAL(int, 0x006B6F04) = 0;
        lootMsg.corpseId = PB_CORPSE->spawn_id;
        lootMsg.playerId = g_local_player_guess->spawn_id;
        lootMsg.slot = *(u16*)(self + index * 2 + 0x6c8b6);
        lootMsg.rightClick = mouseButton == 1;
        if ((*slot)->nodrop == 0 && EQ_GLOBAL(int, 0x006B6F04) != 2 &&
            (EQ_GLOBAL(int, 0x006B6F04) == 0 ||
             EQ_METHOD(1, int, 0x0041BA72, (CharacterProfile*, EQ_Item*), g_pc_guess)(*slot) == 0))
            allowed = 0;
        if (_strcmpi(g_local_player_guess->name,
                EQ_METHOD(1, char*, 0x004D0436, (void*, char*), PB_EVERQUEST)(PB_CORPSE->name)) == 0)
            allowed = 1;
        if (allowed != 0)
        {
            PB_INT(0xa4) = 0;
            PB_INT(0x94)++;
            PB_SEND(0x20a0, &lootMsg, 0x10);
            return;
        }
        *(struct LootItemMsg_guess*)EQ_ARRAY(char, 0x005921A8) = lootMsg;
        strcpy(PB_CONFIRM + 0x1b9, EQ_ARRAY(char, 0x0055AC28));
        pinstEQ_Requestor_guess->requestAction_guess = 0x16;
        PB_VIRTUAL(PB_CONFIRM, 4);
        pinstEQ_Requestor_guess->timeout_guess = *(int*)(PB_DISPLAY + 0xbc) + 20000;
        return;
    }
    if (mode == 5)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
            return;
        item = g_pc_guess->inventory_items[0];
        if (item != 0 && item->type == 0 && EQ_METHOD(0, int, 0x00436270, (EQ_Item*), item)() != 0)
            PB_CURSOR_SPELL = 1;
        if (mouseButton == 0)
        {
            switch (button)
            {
            case 0x35:
                if (PB_INT(0x6c564) == 0 && PB_BOOK_PAGE > 0)
                {
                    PB_INT(0x6c564) = *(int*)(PB_DISPLAY + 0xbc);
                    PB_BOOK_PAGE--;
                    PB_INT(0x6c55c) = 0;
                    EQ_METHOD(1, void, 0x0046E4AA, (char*, int), self)(PB_BOOK_PAGE);
                    goto spellbookMouse;
                }
                break;
            case 0x36:
                if (PB_INT(0x6c564) == 0 && PB_BOOK_PAGE < 0x18)
                {
                    PB_INT(0x6c564) = *(int*)(PB_DISPLAY + 0xbc);
                    PB_BOOK_PAGE++;
                    PB_INT(0x6c55c) = 1;
                    EQ_METHOD(1, void, 0x0046E4AA, (char*, int), self)(PB_BOOK_PAGE);
                }
                break;
            case 0x37:
                if (EQ_METHOD(1, char, 0x0041A3E9, (CharacterProfile*, int), g_pc_guess)(0x1f) != 0)
                    PB_INT(0xfe4) ^= 1;
                else
                    PB_INT(0xfe4) = 0;
                break;
            case 0x38:
                goto spellbookFlip;
            }
            goto spellbookLeftClick;
        }
spellbookMouse:
        if (mouseButton != 1)
        {
            if (mouseButton != 0)
                return;
            goto spellbookLeftClick;
        }
        if (button >= 0x16 && button <= 0x1d)
        {
            if (PB_SCRIBING != -1)
                goto rightAbortScribe;
            if (PB_MEMORIZING != -1)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055B0DC));
                PB_MEMORIZING = -1;
                goto abortMemorizeChat;
            }
            index = button;
            item = g_pc_guess->inventory_items[index];
            if (item != 0 && item->type == 2 && PB_INSPECT == 0)
            {
                if (*(int*)(PB_BOOK + 0x10) == 0)
                {
                    *(EQ_Item**)(PB_BOOK + 0x37ac) = item;
                    text = *(char**)(PB_BOOK + 0x37ac) + 0xaf;
                    PB_SEND(0x20ce, text, strlen(text) + 1);
                }
                else if (PB_IS_ACTIVE(PB_BOOK) != 0 && *(EQ_Item**)(PB_BOOK + 0x37ac) == item)
                    PB_VIRTUAL(PB_BOOK, 8);
            }
            item = g_pc_guess->inventory_items[index];
            if (item == 0)
                return;
            if (item->type != 1)
                return;
            if (PB_INSPECT != 0)
                return;
            if (PB_BAG_OPEN(item) == 1)
            {
                if (PB_IS_ACTIVE(PB_BAG1) != 0 && pinstEQ_Bag1_guess->item_guess == item)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG1, 8);
                    pinstEQ_Bag1_guess->item_guess = 0;
                    return;
                }
                if (PB_IS_ACTIVE(PB_BAG2) == 0)
                    return;
                if (pinstEQ_Bag2_guess->item_guess != item)
                    return;
                PB_BAG_OPEN(item) = 0;
                PB_VIRTUAL(PB_BAG2, 8);
                pinstEQ_Bag2_guess->item_guess = 0;
                return;
            }
            if (PB_IS_ACTIVE(PB_BAG1) == 0)
            {
                PB_BAG_OPEN(item) = 1;
                pinstEQ_Bag1_guess->item_guess = item;
                PB_VIRTUAL_ARG(PB_BAG1, 0x10, index - 0x16);
                pinstEQ_Bag1_guess->slot_guess = button;
                return;
            }
            if (PB_IS_ACTIVE(PB_BAG2) != 0)
                return;
            PB_BAG_OPEN(item) = 1;
            pinstEQ_Bag2_guess->item_guess = item;
            PB_VIRTUAL_ARG(PB_BAG2, 0x10, index - 0x16);
            pinstEQ_Bag2_guess->slot_guess = button;
            return;
        }
        if (button < 0x2b || button > 0x34)
            return;
        if (PB_SCRIBING != -1)
            goto rightAbortScribe;
        if (PB_MEMORIZING != -1)
        {
            sprintf(message, EQ_ARRAY(char, 0x0055B0DC));
            PB_MEMORIZING = -1;
            PB_SHORT(0x6c54e) = 0;
            goto abortChat;
        }
        index = PB_INT(0xed4);
        if (index == -1)
        {
            PB_INT(0xed4) = PB_BOOK_PAGE * 10 + button - 0x2b;
            PB_DSP_CHAT(EQ_ARRAY(char, 0x0055B078), 0xf);
            return;
        }
        if (index < 0)
            return;
        pairPacket[0] = index;
        pairPacket[1] = PB_BOOK_PAGE * 10 + button - 0x2b;
        PB_SEND(0x21ce, pairPacket, 8);
        PB_INT(0xed4) = -2;
        return;
spellbookLeftClick:
        if (PB_HOTKEY_EDIT != 0)
        {
            if (button >= 0xd4 && button <= 0xdb)
                goto dragSpellGem;
            if (button < 0x16)
                goto spellbookSlots;
            if (button <= 0x1d)
            {
                PB_DRAG_TYPE = 5;
                goto setDragValue;
            }
        }
        if (button < 0x16 || button > 0x1d)
            goto spellbookSlots;
        PB_INT(0xed4) = -1;
        if (PB_SCRIBING != -1)
            goto abortScribe;
        if (PB_MEMORIZING != -1)
            goto abortMemorize;
        if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            return;
        if (PB_CURSOR_SPELL != 0)
        {
            PB_CURSOR_SPELL = 0;
            g_cdisplay_guess->itemOnCursor_guess = 0;
        }
        if (g_pc_guess->inventory_items[0] == 0)
        {
            index = button;
            slotOffset = index * 4 + 0xe0;
            if (*(EQ_Item**)((char*)g_pc_guess + slotOffset) == 0)
                goto spellbookSlots;
            if (PB_COMBINE(*(EQ_Item**)((char*)g_pc_guess + slotOffset)) != 0)
            {
                PB_QUANTITY_ACTIVATE((EQ_Item**)((char*)g_pc_guess + slotOffset), index);
                goto spellbookSlots;
            }
            item = *(EQ_Item**)((char*)g_pc_guess + slotOffset);
            if (item->type == 1)
            {
                if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG1, 8);
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG2, 8);
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            if (PB_SOUND != 0)
                PB_WAVE_PLAY();
            PB_MOVE_ITEM(index, 0, 1, 1);
        }
        else
        {
            index = button;
            item = g_pc_guess->inventory_items[index];
            if (item != 0 && item->type == 1)
            {
                if (*(int*)(PB_BAG1 + 0x10) != 0 && item == pinstEQ_Bag1_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG1, 8);
                    pinstEQ_Bag1_guess->item_guess = 0;
                }
                else if (*(int*)(PB_BAG2 + 0x10) != 0 && item == pinstEQ_Bag2_guess->item_guess)
                {
                    PB_BAG_OPEN(item) = 0;
                    PB_VIRTUAL(PB_BAG2, 8);
                    pinstEQ_Bag2_guess->item_guess = 0;
                }
            }
            if (PB_SOUND != 0)
                PB_WAVE_PLAY();
            PB_MOVE_ITEM(0, index, 1, 1);
        }
        PB_UPDATE_LIGHT();
        if (g_pc_guess->inventory_items[0] == 0)
            g_cdisplay_guess->itemOnCursor_guess = 0;
        else
            g_cdisplay_guess->itemOnCursor_guess = 1;
spellbookSlots:
        if (button >= 0x2b && button <= 0x34)
        {
            PB_INT(0xed4) = -1;
            if (PB_SCRIBING != -1)
                goto abortScribe;
            if (PB_MEMORIZING != -1)
                goto abortMemorize;
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                return;
            spellSlot = (short)(PB_BOOK_PAGE * 10 + button - 0x2b);
            PB_SHORT(0xfea) = spellSlot;
            if (PB_CURSOR_SPELL == 1 && (item = g_pc_guess->inventory_items[0]) != 0 && spellSlot <= 0xfa)
            {
                if (g_pc_guess->spell_book[spellSlot] != -1)
                {
                    PB_PRINT_CHAT(EQ_ARRAY(char, 0x0055AFA0));
                }
                else if (EQ_METHOD(1, int, 0x00419956, (CharacterProfile*, short), g_pc_guess)(
                             *(short*)((char*)item + 0xce)) != 0)
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055AFC8), g_pc_guess->inventory_items[0]);
                    PB_DSP_CHAT(message, 0x108);
                }
                else
                {
                    spellData = PB_SPELL_TABLE[*(short*)((char*)item + 0xce)];
                    needed = PB_CLASS_LEVEL(spellData);
                    if (needed > g_pc_guess->level)
                    {
                        if (needed == 0xff)
                            goto neverScribe;
                        sprintf(message, EQ_ARRAY(char, 0x0055AFF8), needed, spellData);
                        PB_DSP_CHAT(message, 0x108);
                    }
                    else if (needed == 0xff)
                    {
neverScribe:
                        sprintf(message, EQ_ARRAY(char, 0x0055B038), g_pc_guess->inventory_items[0]);
                        PB_DSP_CHAT(message, 0x108);
                    }
                    else if (pinstEQ_Main_guess->pendingWorldRequests_guess == 0 && pinstEQ_Main_guess->scribingSpell_guess == -1 &&
                             pinstEQ_Main_guess->memorizingSpell_guess == -1)
                    {
                        sprintf(message, EQ_ARRAY(char, 0x0055B05C), spellData);
                        PB_DSP_CHAT(message, 0x108);
                        pinstEQ_Main_guess->scribingSpell_guess = *(short*)((char*)item + 0xce);
                        pinstEQ_Main_guess->unk_0x6C54C = 0;
                        pinstEQ_Main_guess->scribeBookSlot_guess = PB_SHORT(0xfea);
                        pinstEQ_Main_guess->scribeVialRemaining_guess = 0x5c;
                        pinstEQ_Main_guess->uiDirty_guess = 1;
                    }
                }
            }
            else if (PB_CURSOR_SPELL == 0 && spellSlot <= 0xfa)
            {
                spellId = g_pc_guess->spell_book[spellSlot];
                if (spellId != -1)
                {
                    found = -1;
                    for (counter = 0; counter < 8; counter++)
                    {
                        if (g_pc_guess->spell_memorized[counter] == spellId)
                            found = counter;
                    }
                    if (found != -1)
                        PB_PRINT_CHAT(EQ_ARRAY(char, 0x0055AF68));
                    else
                    {
                        PB_CURSOR_SPELL = spellId + 1000;
                        g_cdisplay_guess->itemOnCursor_guess = 1;
                    }
                }
            }
        }
        if (button >= 200 && button <= 0xd1)
        {
            if (PB_SCRIBING != -1)
                goto abortScribe;
            if (PB_MEMORIZING != -1)
                goto abortMemorize;
            if (*(char*)(PB_DISPLAY + 0x3d) != 0)
                return;
            if (PB_HOT_TYPE(button + PB_HOT_PAGE * 10) == 7 && PB_CURSOR_SPELL >= 1000 &&
                g_pc_guess->spell_memorized[PB_HOT_VALUE(button + PB_HOT_PAGE * 10)] == -1)
            {
                spellData = PB_SPELL_TABLE[PB_CURSOR_SPELL - 1000];
                if ((int)g_pc_guess->level - (int)PB_CLASS_LEVEL(spellData) < 0)
                    PB_PRINT_CHAT(EQ_ARRAY(char, 0x0055AF14));
                else if (pinstEQ_Main_guess->pendingWorldRequests_guess == 0 && pinstEQ_Main_guess->scribingSpell_guess == -1 &&
                         pinstEQ_Main_guess->memorizingSpell_guess == -1)
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055AF4C), spellData);
                    PB_DSP_CHAT(message, 0x108);
                    pinstEQ_Main_guess->memorizingSpell_guess = PB_CURSOR_SPELL - 1000;
                    pinstEQ_Main_guess->unk_0x6C54E = 0;
                    pinstEQ_Main_guess->memorizeGem_guess = PB_HOT_VALUE(button + PB_HOT_PAGE * 10);
                    pinstEQ_Main_guess->memorizeVialRemaining_guess = 0x5c;
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                    PB_CURSOR_SPELL = 0;
                }
            }
        }
        if (button < 0xd4 || button > 0xdb)
            return;
        if (PB_SCRIBING != -1)
            goto abortScribe;
        if (PB_MEMORIZING != -1)
            goto abortMemorize;
        if (*(char*)(PB_DISPLAY + 0x3d) != 0)
            return;
        if (PB_CURSOR_SPELL < 1000)
            return;
        if (g_pc_guess->spell_memorized[button - 0xd4] != -1)
            return;
        spellData = PB_SPELL_TABLE[PB_CURSOR_SPELL - 1000];
        if ((int)g_pc_guess->level - (int)PB_CLASS_LEVEL(spellData) < 0)
        {
            PB_PRINT_CHAT(EQ_ARRAY(char, 0x0055AF14));
            return;
        }
        if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
            return;
        if (pinstEQ_Main_guess->scribingSpell_guess != -1)
            return;
        if (pinstEQ_Main_guess->memorizingSpell_guess != -1)
            return;
        sprintf(message, EQ_ARRAY(char, 0x0055AF4C), spellData);
        PB_DSP_CHAT(message, 0x108);
        pinstEQ_Main_guess->memorizingSpell_guess = PB_CURSOR_SPELL - 1000;
        pinstEQ_Main_guess->unk_0x6C54E = 0;
        pinstEQ_Main_guess->memorizeGem_guess = button - 0xd4;
        pinstEQ_Main_guess->memorizeVialRemaining_guess = 0x5c;
        pinstEQ_Main_guess->uiDirty_guess = 1;
        PB_CURSOR_SPELL = 0;
        return;
abortScribe:
        sprintf(message, EQ_ARRAY(char, 0x0055B0FC));
        PB_SCRIBING = -1;
        goto abortScribeChat;
rightAbortScribe:
        sprintf(message, EQ_ARRAY(char, 0x0055B0FC));
        PB_SCRIBING = -1;
abortScribeChat:
        PB_SHORT(0x6c54c) = 0;
        PB_DSP_CHAT(message, 0x108);
        return;
abortMemorize:
        sprintf(message, EQ_ARRAY(char, 0x0055B0DC));
        PB_MEMORIZING = -1;
abortMemorizeChat:
        PB_SHORT(0x6c54e) = 0;
abortChat:
        PB_DSP_CHAT(message, 0x108);
        PB_CURSOR_SPELL = 0;
clearCursorItem:
        g_cdisplay_guess->itemOnCursor_guess = 0;
        return;
    }
    if (mode == 6)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
        {
            if (mouseButton != 0)
                return;
            PB_PRESSED = button;
            PB_SHORT(0x5f18a) = -1;
            if (button == 0x15)
                PB_INT(0x5f178) = 1;
            if (button != 0x16)
                return;
            PB_INT(0x5f17c) = 1;
            return;
        }
        if (mouseButton != 0)
            return;
        if (button > 10)
        {
            if (button == 0xb)
            {
                if (PB_SHORT(0x685cc) >= PB_INT(0x685c0) - 5)
                    return;
                PB_SHORT(0x685cc)++;
                EQ_METHOD(2, void, 0x00463DC1, (char*, int, short), self)(1, 0);
                return;
            }
            if (button == 0x13)
            {
                if (PB_SHORT(0x685d0) <= 0)
                    return;
                PB_SHORT(0x685d0)--;
                EQ_METHOD(2, void, 0x00463DC1, (char*, int, short), self)(-1, 1);
                return;
            }
            if (button != 0x14)
                return;
            if (PB_SHORT(0x685d0) >= PB_INT(0x685c4) - 5)
                return;
            PB_SHORT(0x685d0)++;
            EQ_METHOD(2, void, 0x00463DC1, (char*, int, short), self)(1, 1);
            return;
        }
        if (button == 10)
        {
            if (PB_SHORT(0x685cc) <= 0)
                return;
            PB_SHORT(0x685cc)--;
            EQ_METHOD(2, void, 0x00463DC1, (char*, int, short), self)(-1, 0);
            return;
        }
        if (button < 3)
            return;
        if (button <= 7)
        {
            spellSlot = PB_SHORT(0x685cc) + button - 3;
            PB_SHORT(0x1008) = spellSlot;
            spellId = *(short*)(self + spellSlot * 2 + 0x6c7ee);
            if (spellId < 100)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055AE90), PB_TRIM_NAME(PB_TRAINER->name),
                    g_skill_records_guess[spellId]);
            }
            else
            {
                sprintf(message, EQ_ARRAY(char, 0x0055AEBC), PB_TRIM_NAME(PB_TRAINER->name),
                    EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), PB_EVERQUEST)(spellId - 100));
            }
            PB_DSP_CHAT(message, 0xf);
            return;
        }
        if (button == 8)
        {
            if (PB_SHORT(0x1008) >= 0)
            {
                EQ_METHOD(0, void, 0x00464468, (char*), self)();
                return;
            }
            PB_PRINT_CHAT(EQ_ARRAY(char, 0x0055AEF4));
            return;
        }
        if (button != 9)
            return;
        if (PB_SOUND_OFF == 0 && PB_MUSIC != 0)
            EQ_METHOD(1, void, 0x004E4EE6, (void*, int), PB_MUSIC)(0x15);
        pairPacket[0] = PB_TRAINER->spawn_id;
        pairPacket[1] = g_local_player_guess->spawn_id;
        PB_SEND(0x209d, pairPacket, 8);
        if (--pinstEQ_Main_guess->pendingServerReplies_guess != 0)
        {
            trainerPacket[0] = 0xfffffffc;
            trainerPacket[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
            trainerPacket[2] = 2;
            PB_SEND(0x2147, trainerPacket, 0x5c);
        }
        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
        PB_TRAINER = 0;
        PB_VIRTUAL(self, 8);
        PB_MODE = PB_PREV_MODE;
        PB_VIRTUAL(self, 4);
        PB_TARGET = 0;
        if (PB_VIDEO_RESTORE != 2)
            return;
        goto restoreVideo;
    }
    if (mode == 8)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
            goto releasePressed;
        if (mouseButton != 0)
            return;
        switch (button)
        {
        case 0:
            if (PB_INT(0xa8) != 0)
                return;
            PB_CHAR(0x8fc41) ^= 1;
            return;
        case 1:
            if (PB_INT(0xa8) != 0)
                return;
            PB_CHAR(0x8fc42) ^= 1;
            return;
        case 3:
            EQ_METHOD(0, void, 0x0046360C, (char*), self)();
        case 2:
closeToPrevious:
            PB_VIRTUAL(self, 8);
            PB_MODE = PB_CHAR(0xfe2);
            PB_SHORT(0x5f186) = -1;
            PB_PRESSED = -1;
            PB_SHORT(0x5f18a) = -1;
            PB_VIRTUAL(self, 4);
            if (PB_VIDEO_RESTORE != 2)
                return;
            goto restoreVideo;
        }
        return;
    }
    if (mode == 9)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
        {
            if (mouseButton != 0)
                return;
            PB_PRESSED = button;
            PB_SHORT(0x5f18a) = -1;
            if (button != 6)
                return;
            goto setLayoutFlag;
        }
        if (PB_CHAR(0x100) != 0)
            return;
        if (mouseButton == 0)
        {
            if (button >= 0x10 && PB_INT(0xf8 + button * 4) != 0)
            {
                PB_SHORT(0xfc) = button - 0x10;
                PB_CHAR(0xdae) = 0;
                if (strcmp(
                        *(char**)(self + PB_SHORT(0xfc) * 4 + 0x138) + 0x1c, g_pc_guess->name) == 0)
                {
                    PB_CHAR(0xdae) = 1;
                    goto boardRead;
                }
            }
        }
        else
        {
boardRead:
            if (mouseButton == 1)
            {
                if (button < 0x10)
                    return;
                if (PB_INT(0xf8 + button * 4) == 0)
                    return;
                PB_SHORT(0xfc) = button - 0x10;
                PB_CHAR(0xdae) = 0;
                if (strcmp(
                        *(char**)(self + PB_SHORT(0xfc) * 4 + 0x138) + 0x1c, g_pc_guess->name) == 0)
                    PB_CHAR(0xdae) = 1;
                PB_CHAR(0x100) = 1;
                boardMessage.board = PB_CHAR(0xdaa) - 9;
                boardMessage.messageId = *(u32*)(*(char**)(self + PB_SHORT(0xfc) * 4 + 0x138) + 8);
                boardMessage.spawnId = g_local_player_guess->spawn_id;
                PB_SEND(0x2109, &boardMessage, 0xc);
            }
            if (mouseButton != 0)
                return;
        }
        if (button < 8)
        {
            if (button == 7)
            {
                if (PB_SHORT(0xdaa) == 9 && g_local_player_guess->gm != 0)
                    return;
                strcpy(self + 0x188, EQ_ARRAY(char, 0x0055AE7C));
                strcpy(self + 0x1a6, EQ_ARRAY(char, 0x0055AE68));
                PB_VIRTUAL(self, 8);
                PB_MODE = 0xb;
                PB_CHAR(0xf6) = 0;
                goto activateReturn;
            }
            if (button < 0)
                return;
            if (button <= 3)
            {
                PB_SHORT(0xdac) = button;
                EQ_CDECL(void, 0x004E1D09, (short))(button);
                PB_SHORT(0xf2) = 0;
                PB_SHORT(0xfc) = 0;
                entry = PB_BOARD_LIST;
            }
            else if (button == 4)
            {
                if (PB_SHORT(0xf2) > 0)
                    PB_SHORT(0xf2)--;
                entry = PB_BOARD_LIST;
                for (index = PB_SHORT(0xf2); index > 0; index--)
                    entry = *(char**)(entry + 4);
            }
            else if (button == 5)
            {
                if (PB_SHORT(0xf2) < PB_SHORT(0xf4) - 1)
                    PB_SHORT(0xf2)++;
                entry = PB_BOARD_LIST;
                for (index = PB_SHORT(0xf2); index > 0; index--)
                    entry = *(char**)(entry + 4);
            }
            else
                return;
            for (index = 0; index < 20; index++)
            {
                *(char**)(self + 0x138 + index * 4) = 0;
                if (entry != 0)
                {
                    *(char**)(self + 0x138 + index * 4) = entry;
                    entry = *(char**)(entry + 4);
                }
            }
            if (PB_SHORT(0xf4) <= 1)
                thumb = 0x51;
            else
            {
                thumb = (short)(PB_SHORT(0xf2) * 0x11c / (PB_SHORT(0xf4) - 1) + 0x51);
                if (thumb > 0x16d)
                    thumb = 0x16d;
            }
            (*(RECT**)(self + 0x6c5d0))->top = thumb;
            (*(RECT**)(self + 0x6c5d0))->bottom = thumb + 0xb;
            EQ_GLOBAL(tCopyRect_guess, 0x00534240)((RECT*)(self + PB_MODE * 0x1900 + 0x6c96c), *(RECT**)(self + 0x6c5d0));
            return;
        }
        if (button == 8)
        {
            if (PB_CHAR(0xdae) == 0)
                return;
            goto boardDeleteMessage;
        }
        if (button > 0xe)
        {
            if (button != 0xf)
                return;
            goto closeBoard;
        }
        while (PB_BOARD_LIST != 0)
        {
            entry = PB_BOARD_LIST;
            EQ_METHOD(0, void, 0x004906B1, (char*), entry)();
            operator delete(entry);
        }
        for (index = 0; index < 20; index++)
            *(int*)(self + 0x138 + index * 4) = 0;
        PB_SHORT(0xf2) = 0;
        PB_CHAR(0xdae) = 0;
        PB_SHORT(0xf4) = 0;
        PB_SHORT(0xfc) = 0;
        PB_CHAR(0x100) = 1;
        PB_SHORT(0xdaa) = button;
        oldType = (char)button;
        goto boardRequestList;
    }
    if (mode == 10)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
            goto releasePressed;
        if (PB_CHAR(0x100) != 0)
            return;
        if (mouseButton != 0)
            return;
        switch (button)
        {
        case 0:
            if (PB_SHORT(0xdaa) == 9 && g_local_player_guess->gm == 0)
                return;
            PB_VIRTUAL(self, 8);
            PB_MODE = 0xb;
            PB_CHAR(0xf6) = 1;
            PB_CHAR(0xfe) = 0;
            strcpy(self + 0x188, *(char**)(self + PB_SHORT(0xfc) * 4 + 0x138) + 0x3b);
            strcpy(self + 0x1a6, EQ_ARRAY(char, 0x0055AE68));
            goto activateReturn;
        case 1:
            if (PB_CHAR(0xdae) == 0)
                return;
            goto boardDeleteMessage;
        case 2:
boardList:
            PB_VIRTUAL(self, 8);
            PB_MODE = 9;
            goto activateReturn;
        }
        return;
    }
    if (mode == 11)
    {
        if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
            goto releasePressed;
        if (PB_CHAR(0x100) != 0)
            return;
        if (mouseButton != 0)
            return;
        switch (button)
        {
        case 0:
            strcpy(post.body, self + 0x1a6);
            strcpy(post.subject, self + 0x188);
            post.spawnId = g_local_player_guess->spawn_id;
            strcpy(post.author, g_pc_guess->name);
            post.board = PB_SHORT(0xdaa) - 9;
            sprintf(message, EQ_ARRAY(char, 0x0055AE50), *(short*)(EQ_GLOBAL(u8*, 0x006E4D54) + 4),
                EQ_GLOBAL(u8*, 0x006E4D54)[3], EQ_GLOBAL(u8*, 0x006E4D54)[2], EQ_GLOBAL(u8*, 0x006E4D54)[0],
                EQ_GLOBAL(u8*, 0x006E4D54)[1]);
            strcpy(post.date, message + 2);
            post.language = *(u8*)(self + PB_SHORT(0xf8) * 2 + 0x6c65e);
            PB_SEND(0x210c, &post, strlen(post.body) + 0x59);
            while (PB_BOARD_LIST != 0)
            {
                entry = PB_BOARD_LIST;
                EQ_METHOD(0, void, 0x004906B1, (char*), entry)();
                operator delete(entry);
            }
            goto boardClear;
        case 1:
            if (PB_CHAR(0xdae) == 0)
                return;
            goto boardDeleteMessage;
        case 2:
            if (++PB_SHORT(0xf8) > PB_SHORT(0xfa))
                PB_SHORT(0xf8) = 0;
            return;
        case 3:
            goto boardList;
        case 4:
            if (PB_CHAR(0xf6) != 0)
                return;
            PB_CHAR(0xfe) = 1;
            return;
        case 5:
            PB_CHAR(0xfe) = 0;
            return;
        }
        return;
    }
    if (mode != 0xc)
        return;
    if (PB_EDIT_LAYOUT != 0 && PB_HOTKEY_EDIT == 0)
        goto releasePressed;
    if (mouseButton != 0)
        return;
    if (button != 0)
        return;
closeBoard:
    PB_VIRTUAL(self, 8);
    PB_MODE = PB_PREV_MODE;
    PB_VIRTUAL(self, 4);
    if (PB_VIDEO_RESTORE != 2)
        return;
    PB_VIDEO_MODE = 2;
    if (PB_MAIN != 0)
        pinstEQ_Main_guess->uiDirty_guess = 1;
    PB_CHANGE_VIDEO_MODE();
    return;
boardDeleteMessage:
    boardDelete.board = PB_SHORT(0xdaa) - 9;
    boardDelete.messageId = *(u32*)(*(char**)(self + PB_SHORT(0xfc) * 4 + 0x138) + 8);
    boardDelete.spawnId = g_local_player_guess->spawn_id;
    PB_SEND(0x210d, &boardDelete, 0xc);
    while (PB_BOARD_LIST != 0)
    {
        entry = PB_BOARD_LIST;
        EQ_METHOD(0, void, 0x004906B1, (char*), entry)();
        operator delete(entry);
    }
boardClear:
    for (index = 0; index < 20; index++)
        *(int*)(self + 0x138 + index * 4) = 0;
    PB_SHORT(0xf2) = 0;
    PB_CHAR(0xdae) = 0;
    PB_SHORT(0xf4) = 0;
    PB_SHORT(0xfc) = 0;
    PB_CHAR(0x100) = 1;
    oldType = PB_CHAR(0xdaa);
boardRequestList:
    boardRequest.board = oldType - 9;
    boardRequest.spawnId = g_local_player_guess->spawn_id;
    PB_SEND(0x2108, &boardRequest, 8);
    return;
releasePressed:
    if (mouseButton != 0)
        return;
setPressed:
    PB_PRESSED = button;
clearRelease:
    PB_SHORT(0x5f18a) = -1;
    return;
mainScreen:
    if (mouseButton != 0)
        return;
    if (PB_HOTKEY_EDIT != 0 && button >= 0xe6 && button <= 0xeb)
    {
        PB_DRAG_TYPE = 6;
        button -= 0xe6;
        goto setDragValue;
    }
    switch (button)
    {
    case 0xe3:
caseE3:
        PB_VIRTUAL(self, 8);
        PB_MODE = 1;
        goto resetPressed;
    case 0xe4:
caseE4:
        PB_VIRTUAL(self, 8);
        PB_MODE = 2;
        goto resetPressed;
    case 0xe5:
caseE5:
        PB_VIRTUAL(self, 8);
        PB_MODE = 3;
resetPressed:
        PB_SHORT(0x5f186) = -1;
        PB_PRESSED = -1;
        PB_SHORT(0x5f18a) = -1;
activateReturn:
        PB_VIRTUAL(self, 4);
        break;
    case 0xe6:
        if (EQ_GLOBAL(char, 0x006B63F8) == 0)
            EQ_METHOD(1, void, 0x004CA5C7, (void*, char*), PB_EVERQUEST)(EQ_ARRAY(char, 0x0058FBA4));
        break;
    case 0xe7:
        if (PB_CHAR(0xb5) != 0)
            EQ_METHOD(0, void, 0x004CB362, (void*), PB_EVERQUEST)();
        else
            EQ_METHOD(0, void, 0x004CAF5E, (void*), PB_EVERQUEST)();
        break;
    case 0xe8:
        EQ_METHOD(0, void, 0x004CB4FA, (void*), PB_EVERQUEST)();
        break;
    case 0xe9:
        EQ_METHOD(0, void, 0x004CB72D, (void*), PB_EVERQUEST)();
        break;
    case 0xea:
        EQ_METHOD(0, void, 0x004CB6E6, (void*), PB_EVERQUEST)();
        break;
    case 0xeb:
        EQ_GLOBAL(u8, 0x006B6405) ^= 1;
        break;
    }
    return;
closeToMain:
    PB_VIRTUAL(self, 8);
    PB_MODE = 0;
    goto resetPressed;
dragSpellGem:
    PB_DRAG_TYPE = 7;
    button -= 0xd4;
setDragValue:
    PB_DRAG_VALUE = button;
dragArmed:
    g_cdisplay_guess->dragArmed_guess = 1;
    return;
restoreVideo:
    PB_VIDEO_MODE = 2;
    PB_CHANGE_VIDEO_MODE();
    return;
optionsRightClick:
    if (PB_OPTIONS_PAGE == 5 && button >= 0x7d && button <= 0x85)
    {
        PB_CHAR(0x8fbad) = PB_CHAR(0x100d) * 9 + (char)button - 0x7c;
        PB_INT(0x8fbb0) = 1;
afterOptionsMouse:
        if (mouseButton != 0)
            goto screens;
        goto trackWindow;
    }
    goto screens;
}

#undef PB_CHAR
#undef PB_SHORT
#undef PB_INT
#undef PB_MODE
#undef PB_PREV_MODE
#undef PB_PRESSED
#undef PB_DRAG_TYPE
#undef PB_DRAG_VALUE
#undef PB_HOT_PAGE
#undef PB_HOT_TYPE
#undef PB_HOT_VALUE
#undef PB_OPTIONS_PAGE
#undef PB_EDIT_LAYOUT
#undef PB_LOOT_OPEN
#undef PB_MERCHANT_OPEN
#undef PB_TRADE_OPEN
#undef PB_BANK_OPEN
#undef PB_HOUSE_BANK
#undef PB_LIST_PAGE
#undef PB_CURSOR_SPELL
#undef PB_BOOK_PAGE
#undef PB_MEMORIZING
#undef PB_SCRIBING
#undef PB_VIDEO_MODE
#undef PB_VIDEO_RESTORE
#undef PB_HOTKEY_EDIT
#undef PB_INSPECT
#undef PB_DISPLAY
#undef PB_MAIN
#undef PB_EVERQUEST
#undef PB_ITEM_DISPLAY
#undef PB_BAG1
#undef PB_BAG2
#undef PB_BOOK
#undef PB_CONFIRM
#undef PB_TARGET
#undef PB_MERCHANT
#undef PB_CORPSE
#undef PB_TRAINER
#undef PB_TRADE_TARGET
#undef PB_BANKER_ID
#undef PB_SOUND
#undef PB_MUSIC
#undef PB_SOUND_OFF
#undef PB_COIN_SLIDER
#undef PB_QUANTITY_SLIDER
#undef PB_BOARD_LIST
#undef PB_VIRTUAL
#undef PB_VIRTUAL_ARG
#undef PB_SEND
#undef PB_DSP_CHAT
#undef PB_PRINT_CHAT
#undef sprintf
#undef strcpy
#undef strlen
#undef memcpy
#undef PB_TRIM_NAME
#undef PB_CHANGE_VIDEO_MODE
#undef PB_SAVE_CUSTOM
#undef PB_SAVE_OPTIONS
#undef PB_SAVE_OPTIONS2
#undef PB_IS_ACTIVE
#undef PB_COMBINE
#undef PB_MOVE_ITEM
#undef PB_WAVE_PLAY
#undef PB_UPDATE_LIGHT
#undef PB_QUANTITY_ACTIVATE
#undef PB_COIN_ACTIVATE
#undef PB_BAG_OPEN
#undef PB_BAG_ITEM
#undef PB_SPELL_TABLE
#undef PB_CLASS_LEVEL
#undef PB_FLOAT_GAMMA
#undef PB_FLOAT_SCALE
#undef PB_CURRENT_VIDEO

// FUNCTION: TRILOGY 0x0046360c
// Sends prepared form data, including the character name, to the server.
void __fastcall EQ_Main_SubmitBugOrFeedback_guess(void* self, int unused)
{
    char* data = (char*)self + 0x8fbf8;

    strcpy(data, EQ_GLOBAL(char*, 0x006E4DA8));
    if (*(int*)((char*)self + 0xa8) == 0)
        send_message_guess(
            g_world_connection_guess, 0x20b3, data, 1099, 1);
    else
        send_message_guess(
            g_world_connection_guess, 0x213c, data, 1099, 1);
}

// FUNCTION: TRILOGY 0x00463651
// Moves an item from one slot id to another (inventory, bags, other containers), telling the server (0x212C)
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: 116 structural differences remain, mainly in range joins and exit branch layout.
// Never reads `this`: works on the EQ_Main instance in HWM_EQMAIN. __fastcall with a dummy EDX keeps
// the four stack arguments and the RET 0x10.
int __fastcall EQ_Main_MoveItem_guess(void* self, int unused, int fromSlotId, int param_2, int showMessage, int tryCombine)
{
    char* fromBag;
    char** fromSlot;
    char* toBag;
    int payload[3];
    char* obj;
    char* profile;
    char* eqMain;
    char** toSlot;
    char* item;

    toSlot = 0;
    fromSlot = 0;
    fromBag = 0;
    toBag = 0;
    obj = EQ_GLOBAL(char*, 0x006E4E20);
    if (obj != 0 && *(int*)(obj + 0x10) != 0 && (*(u8*)(obj + 0x17C) == 5 || *(u8*)(obj + 0x17C) == 0x15))
        ((void (__fastcall*)(void*))(*(void***)obj)[2])(obj);
    profile = EQ_GLOBAL(char*, 0x006E4DA8);
    eqMain = g_eqmain_guess;
    if (fromSlotId >= 0)
    {
        if (fromSlotId < 0x1e)
        {
            fromSlot = (char**)(profile + 0xe0) + fromSlotId;
        }
        else if (fromSlotId <= 0x14a)
        {
            item = ((char**)(profile + 0xe0))[(fromSlotId - 0x1e) / 10];
            if (item == 0)
                return 0;
            if (*(u8*)(item + 0x81) != 1)
                return 0;
            fromBag = item;
            fromSlot = (char**)(fromBag + 0xac + ((fromSlotId - 0x1e) % 10) * 4);
        }
    }
    if (fromSlotId < 0 || fromSlotId > 0x14a)
    {
        if (fromSlotId >= 2000 && fromSlotId < 0x7ee)
        {
            fromSlot = (char**)(profile + fromSlotId * 4 - 0xffc);
        }
        else if (fromSlotId >= 0x7ee && fromSlotId <= 0x91a)
        {
            item = ((char**)(profile + 0xf44))[(fromSlotId - 0x7ee) / 10];
            if (item == 0)
                return 0;
            if (*(u8*)(item + 0x81) != 1)
                return 0;
            fromBag = item;
            fromSlot = (char**)(fromBag + 0xac + ((fromSlotId - 0x7ee) % 10) * 4);
        }
        else if (fromSlotId >= 3000 && fromSlotId < 0xbd6)
        {
            fromSlot = (char**)(eqMain + 0x6962c + fromSlotId * 4);
        }
        else if (fromSlotId >= 0xbd6 && fromSlotId <= 0xd02)
        {
            item = ((char**)(eqMain + 0x6c50c))[(fromSlotId - 0xbd6) / 10];
            if (item == 0)
                return 0;
            if (*(u8*)(item + 0x81) != 1)
                return 0;
            fromBag = item;
            fromSlot = (char**)(fromBag + 0xac + ((fromSlotId - 0xbd6) % 10) * 4);
        }
        else if (fromSlotId >= 4000 && fromSlotId <= 0x10ea)
        {
            fromBag = *(char**)(eqMain + 0x1010);
            fromSlot = (char**)(fromBag + 0xac + ((fromSlotId - 4000) % 10) * 4);
        }
    }
    if (param_2 < 0x1e)
    {
        toSlot = (char**)(profile + 0xe0) + param_2;
    }
    else if (param_2 <= 0x14a)
    {
        item = ((char**)(profile + 0xe0))[(param_2 - 0x1e) / 10];
        if (item == 0)
            return 0;
        if (*(u8*)(item + 0x81) != 1)
            return 0;
        toBag = item;
        toSlot = (char**)(toBag + 0xac + ((param_2 - 0x1e) % 10) * 4);
    }
    else if (param_2 >= 2000 && param_2 < 0x7ee)
    {
        toSlot = (char**)(profile + param_2 * 4 - 0xffc);
    }
    else if (param_2 >= 0x7ee && param_2 <= 0x91a)
    {
        item = ((char**)(profile + 0xf44))[(param_2 - 0x7ee) / 10];
        if (item == 0)
            return 0;
        if (*(u8*)(item + 0x81) != 1)
            return 0;
        toBag = item;
        toSlot = (char**)(toBag + 0xac + ((param_2 - 0x7ee) % 10) * 4);
    }
    else if (param_2 >= 3000 && param_2 < 0xbd6)
    {
        toSlot = (char**)(eqMain + 0x6962c + param_2 * 4);
    }
    else if (param_2 >= 0xbd6 && param_2 <= 0xd02)
    {
        item = ((char**)(eqMain + 0x6c50c))[(param_2 - 0xbd6) / 10];
        if (item == 0)
            return 0;
        if (*(u8*)(item + 0x81) != 1)
            return 0;
        toBag = item;
        toSlot = (char**)(toBag + 0xac + ((param_2 - 0xbd6) % 10) * 4);
    }
    else if (param_2 >= 4000 && param_2 <= 0x10ea)
    {
        toBag = *(char**)(eqMain + 0x1010);
        toSlot = (char**)(toBag + 0xac + ((param_2 - 4000) % 10) * 4);
    }

    if (fromSlot == 0)
        return 0;
    if (toSlot == 0)
        return 0;
    item = *fromSlot;
    if (item == 0 && *toSlot == 0)
        return 0;
    if (fromBag == 0)
    {
        fromBag = toBag;
    }
    else
    {
        if (toBag != 0)
            return 0;
        item = *toSlot;
    }
    if (fromBag != 0 && item != 0 && EQ_CDECL(int, 0x004A25FF, (void*, void*, int))(item, fromBag, showMessage) == 0)
        return 0;
    if (tryCombine != 0 && EQ_CDECL(int, 0x004A1C7C, (void*, void*, int, int))(fromSlot, toSlot, fromSlotId, param_2) != 0)
        return 1;
    if (*fromSlot != 0 && *toSlot != 0 && EQ_CDECL(int, 0x004A1D83, (void*))(*fromSlot) != 0 &&
        *(short*)(*fromSlot + 0x82) == *(short*)(*toSlot + 0x82))
        return 0;
    item = *fromSlot;
    *fromSlot = *toSlot;
    *toSlot = item;
    payload[2] = 0;
    payload[0] = fromSlotId;
    payload[1] = param_2;
    send_message_guess(
        g_world_connection_guess, 0x212c, payload, 0xc, 1);
    return 1;
}

// FUNCTION: TRILOGY 0x00463A0E
// Requests a merchant purchase for the selected item and quantity, or reports when it cannot be afforded.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// Buy request for `count` of the item at this[0x6c658]. When the player can't afford it (or there is no
// target) a chat line says so; otherwise the buy message (opcode 0x2027) is sent.
void __fastcall EQ_Main_BuyItem_guess(EQ_Main* self, int unused, int count)
{
    struct BuyItemMessage_guess
    {
        u32 npcId;          // 0x00  target spawn id
        u32 playerId;       // 0x04  local player spawn id
        short slot;         // 0x08  this[0x6c65c]
        int quantity;       // 0x0C
        u32 unwritten;      // 0x10  not set here; the message is 0x14 bytes
    } msg;
    char message[128];
    int quantity;
    int value;
    char* price;

    // count's slot holds the total on person from the TotalOnPerson call on; quantity keeps the count
    quantity = count;
    if (quantity >= 1)
    {
        count = EQ_METHOD(0, int, 0x00423DEB, (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
        value = EQ_METHOD(2, int, 0x00438550, (void*, float, int), *self->selectedItemSlotPtr_charpp_guess)(pinstEQ_Main_guess->merchantRate_guess, quantity);
        if (count < value || EQ_GLOBAL(char*, 0x006E4DB8) == 0)
        {
            if (quantity < 2)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055B3B8),
                     EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(EQ_GLOBAL(char*, 0x006E4DB8) + 1),
                     *self->selectedItemSlotPtr_charpp_guess);
            }
            else
            {
                price = EQ_METHOD(2, char*, 0x00438701, (void*, float, int), *self->selectedItemSlotPtr_charpp_guess)(pinstEQ_Main_guess->merchantRate_guess, quantity);
                sprintf(message, EQ_ARRAY(char, 0x0055B36C),
                     EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(EQ_GLOBAL(char*, 0x006E4DB8) + 1),
                     price, quantity, *self->selectedItemSlotPtr_charpp_guess);
            }
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
        }
        else
        {
            self->merchantReady_guess = 0;
            self->uiDirty_guess = 1;
            msg.npcId = *(u32*)(EQ_GLOBAL(char*, 0x006E4DB8) + 0x80);
            msg.playerId = g_local_player_guess->spawn_id;
            ++self->pendingWorldRequests_guess;
            msg.quantity = quantity;
            msg.slot = (short)*(char*)((char*)self + 0x6c65c);
            send_message_guess(g_world_connection_guess, 0x2027, &msg, 0x14, 1);
        }
    }
}

// FUNCTION: TRILOGY 0x00463b5d
// Requests a merchant sale for the selected item after checking its contents and sale eligibility.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: loop guard is `test ecx, ecx` (orig `cmp ecx, edx`) and the slot-array `lea` sits above its `jle` (orig below)
// Sell request for the item at *itemPointer (opcode 0x2028). When the item can't be sold, or another request is
// still pending, a chat line says so and 0 is returned; otherwise the sell message is sent and 1 is returned.
int __fastcall EQ_Main_SellItem_guess(void* self, int unused, int* itemPointer, short itemSlot)
{
    struct SellItemMessage_guess
    {
        u32 npcId;          // 0x00  target spawn id
        u32 playerId;       // 0x04  local player spawn id
        short slot;         // 0x08
        int quantity;       // 0x0C
        u32 unwritten;      // 0x10  not set here; the message is 0x14 bytes
    } msg;
    char message[512];
    int item;
    int index;
    int count;
    int* itemValue;
    int quantity;
    int result;

    item = *itemPointer;
    result = EQ_CDECL(int, 0x004A0A4C, (int, int))(item, 1);
    if (result == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0055B47C),
            EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(EQ_GLOBAL(char*, 0x006E4DB8) + 1));
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
        return 0;
    }

    index = 0;
    if (*(int*)((char*)self + 0xa0) != 0 && *(int*)((char*)self + 0x94) == 0)
    {
        if (*(char*)(item + 0x81) == '\x01')
        {
            count = *(unsigned char*)(item + 0xd5);
            itemValue = (int*)(item + 0xac);
            while (index < count)
            {
                if (*itemValue != 0)
                {
                    sprintf(message, EQ_ARRAY(char, 0x0055B428),
                        EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(EQ_GLOBAL(char*, 0x006E4DB8) + 1));
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
                    return 0;
                }
                index = index + 1;
                itemValue = itemValue + 1;
            }
        }

        *(int*)((char*)self + 0xa0) = 0;
        *(int*)((char*)self + 0x94) = 1;
        *(int*)((char*)self + 0xeb8) = 1;
        result = EQ_CDECL(int, 0x004A1D83, (int))(item);
        if (result != 0)
            quantity = ((int)(unsigned char)*(char*)(item + 0xda) - 1) /
                (int)*(char*)(item + 0xbb) + 1;
        else
            quantity = 0;

        msg.npcId = *(u32*)(EQ_GLOBAL(char*, 0x006E4DB8) + 0x80);
        msg.playerId = g_local_player_guess->spawn_id;
        msg.quantity = quantity;
        msg.slot = itemSlot;
        send_message_guess(g_world_connection_guess, 0x2028, &msg, 0x14, 1);
        *itemPointer = item;
        return 1;
    }

    sprintf(message, EQ_ARRAY(char, 0x0055B3E4),
        EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(EQ_GLOBAL(char*, 0x006E4DB8) + 1));
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
    return 0;
}

// FUNCTION: TRILOGY 0x00463cbe
// Repositions the current list scrollbar for the active interface layout.
// @early-stop: then arm loads its values before the zero store (original: store first); both arms compute the denominator as load + sub in place (original: mov eax,[mem]; lea reg,[eax-6])
/* fake match: rowOffset takes its +0x14 in its own statement before the test (VC6 keeps it in ecx for the max and min compares); 0x15e and 0x21 are relative to it */
void EQ_Main::EQ_Main_UpdateScrollbarPosition_guess(int unused)
{
    EQ_Main* self = (EQ_Main *)this;
    int count;
    int rowOffset;
    int numerator;
    int position;
    char* copySource;
    int panel;
    int sourceOffset;

    if (g_game_state_guess == 2)
    {
        *(u32*)((char *)self + 0x90194) = 0;
        rowOffset = *(int*)((char *)self + 0x90188);
        count = self->skillListCount_guess;
        rowOffset += 0x14;
        if (count - 6 <= 0)
            return;

        numerator = (int)self->skillListTop_guess * 0x140;
        position = (int)(s16)(numerator / (count - 6));
        position = position + rowOffset + 0x21;
        *(int*)(self->scrollThumbRect_bytes_guess + 4) = position;
        if (*(int*)(self->scrollThumbRect_bytes_guess + 4) > rowOffset + 0x15e)
            *(int*)(self->scrollThumbRect_bytes_guess + 4) = rowOffset + 0x15e;
        if (*(int*)(self->scrollThumbRect_bytes_guess + 4) < rowOffset + 0x21)
            *(int*)(self->scrollThumbRect_bytes_guess + 4) = rowOffset + 0x21;
    }
    else
    {
        count = self->skillListCount_guess;
        if (count - 6 <= 0)
            return;

        numerator = (int)self->skillListTop_guess * 0x180;
        position = (int)(s16)(numerator / (count - 6));
        position = (int)((u32)position + 0x35u);
        *(int*)(self->scrollThumbRect_bytes_guess + 4) = position;
        if (*(int*)(self->scrollThumbRect_bytes_guess + 4) > 0x1b5)
            *(int*)(self->scrollThumbRect_bytes_guess + 4) = 0x1b5;
        if (*(int*)(self->scrollThumbRect_bytes_guess + 4) < 0x35)
            *(int*)(self->scrollThumbRect_bytes_guess + 4) = 0x35;
    }

    *(int*)(self->scrollThumbRect_bytes_guess + 0xc) = (int)((u32)*(int*)(self->scrollThumbRect_bytes_guess + 4) + 0xbu);
    copySource = self->scrollThumbRect_bytes_guess;
    panel = (int)*(signed char*)((char *)self + 0xfe0);
    sourceOffset = panel * 0x1900 + 0x6cbac;
    CopyRect((RECT*)((char*)self + sourceOffset), (RECT*)copySource);
    self->uiDirty_u32_guess = 1;
}

// FUNCTION: TRILOGY 0x00463dc1
// Repositions the selected one of two content-pane scrollbars from its stored offset.
void EQ_Main::EQ_Main_UpdateScrollbarPosition_guess(int unused, short mode)
{
    EQ_Main* self;
    int height;
    int denominator;
    int numerator;
    int position;
    char* thumb;
    char* copySource;
    int panel;
    int sourceOffset;

    self = (EQ_Main *)this;
    if (mode == 0)
    {
        height = self->trainSkillCount_guess;
        denominator = (int)((u32)height - 5u);
        if (denominator < 1)
            return;

        numerator = (int)self->trainSkillTop_guess * 0x116;
        position = (int)(s16)(numerator / denominator);
        position = (int)((u32)position + 0x32u);
        *(int*)(self->trainThumbRect_bytes_guess + 4) = position;
        if (*(int*)(self->trainThumbRect_bytes_guess + 4) > 0x148)
            *(int*)(self->trainThumbRect_bytes_guess + 4) = 0x148;
        if (*(int*)(self->trainThumbRect_bytes_guess + 4) < 0x32)
            *(int*)(self->trainThumbRect_bytes_guess + 4) = 0x32;
        thumb = self->trainThumbRect_bytes_guess;
        *(int*)(thumb + 0xc) = (int)((u32)*(int*)(thumb + 4) + 0xbu);
        copySource = self->trainThumbRect_bytes_guess;
        panel = (int)*(signed char*)((char *)self + 0xfe0);
        sourceOffset = panel * 0x1900 + 0x6ca5c;
    }
    else
    {
        height = self->skillWindowCount_guess;
        denominator = (int)((u32)height - 5u);
        if (denominator < 1)
            return;

        numerator = (int)self->skillWindowTop_guess * 0x116;
        position = (int)(s16)(numerator / denominator);
        position = (int)((u32)position + 0x9bu);
        *(int*)(self->skillWindowThumbRect_bytes_guess + 4) = position;
        if (*(int*)(self->skillWindowThumbRect_bytes_guess + 4) > 0x1b1)
            *(int*)(self->skillWindowThumbRect_bytes_guess + 4) = 0x1b1;
        if (*(int*)(self->skillWindowThumbRect_bytes_guess + 4) < 0x9b)
            *(int*)(self->skillWindowThumbRect_bytes_guess + 4) = 0x9b;
        thumb = self->skillWindowThumbRect_bytes_guess;
        *(int*)(thumb + 0xc) = (int)((u32)*(int*)(thumb + 4) + 0xbu);
        copySource = self->skillWindowThumbRect_bytes_guess;
        panel = (int)*(signed char*)((char *)self + 0xfe0);
        sourceOffset = panel * 0x1900 + 0x6ca6c;
    }

    CopyRect((RECT*)((char*)self + sourceOffset), (RECT*)copySource);
    self->uiDirty_u32_guess = 1;
}

// FUNCTION: TRILOGY 0x00463ed9
// Drags the grabbed old-UI window (move or resize) to the mouse within the screen; saves it per resolution.
// @early-stop: selector dispatch and coordinate clamping still differ in branch order and register use
void __fastcall EQ_Main_HandleMousePositionForUIElements_guess(EQ_Main* selfPointer, int edxDummy, short param_2, short param_3)
{
    int tmp;
    int* dragWindow;
    int* resWindow;
    int resIndex;
    int* base;
    class EQ_MainOverlayCallbacks_guess
    {
    public:
        virtual void unused0() = 0;
        virtual void callback4() = 0;
        virtual void callback8() = 0;
    };

    base = (int*)selfPointer;
    resIndex = 4;
    if (EQ_GLOBAL(int, 0x006B63FC) == 800)
        resIndex = 2;
    else if (EQ_GLOBAL(int, 0x006B63FC) == 0x400)
        resIndex = 3;
    else if (EQ_GLOBAL(int, 0x006B63FC) != 0x500)
        resIndex = ((EQ_GLOBAL(int, 0x006B63FC) != 0x640) - 1 & 4) + 1;

    do
    {
        dragWindow = base + 0x24011;
        dragWindow[4] = 0;
        if (dragWindow[6] != 0)
        {
            resWindow = base + resIndex * 8 + 0x240a1;
            break;
        }

        dragWindow = base + 0x24019;
        base[0x2401d] = 0;
        if (base[0x2401f] != 0)
        {
            resWindow = base + resIndex * 8 + 0x240d1;
            break;
        }

        dragWindow = base + 0x24021;
        base[0x24025] = 0;
        if (base[0x24027] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24101;
            break;
        }

        dragWindow = base + 0x24029;
        base[0x2402d] = 0;
        if (base[0x2402f] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24131;
            break;
        }

        dragWindow = base + 0x24031;
        base[0x24035] = 0;
        if (base[0x24037] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24161;
            break;
        }

        dragWindow = base + 0x24059;
        base[0x2405d] = 0;
        if (base[0x2405f] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24251;
            break;
        }
        base[0x2405d] = 0;
        if (base[0x24060] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24251;
            break;
        }

        dragWindow = base + 0x24091;
        base[0x24095] = 0;
        if (base[0x24097] != 0)
        {
            resWindow = base + resIndex * 8 + 0x243a1;
            break;
        }

        dragWindow = base + 0x24061;
        base[0x24065] = 0;
        if (base[0x24067] != 0)
        {
            resWindow = base + resIndex * 8 + 0x24281;
            break;
        }

        dragWindow = base + 0x24099;
        base[0x2409d] = 0;
        if (base[0x2409f] != 0)
        {
            resWindow = base + resIndex * 8 + 0x243d1;
            break;
        }
        return;
    }
    while (0);

    if (dragWindow != 0)
    {
        dragWindow[4] = 0;
        if (dragWindow[7] != 0)
        {
            tmp = EQ_GLOBAL(int, 0x006B63FC) - 5;
            if ((int)param_2 >= tmp)
                param_2 = (short)tmp;
            tmp = EQ_GLOBAL(int, 0x006B6400) - 5;
            if ((int)param_3 >= tmp)
                param_3 = (short)tmp;
            dragWindow[4] = 0;
            if ((int)param_2 < *dragWindow + 0x2c)
            {
                dragWindow[4] = 0;
                param_2 = (short)(*dragWindow + 0x2c);
            }
            dragWindow[4] = 0;
            if ((int)param_3 < dragWindow[1] + 0x2c)
            {
                dragWindow[4] = 0;
                param_3 = (short)(dragWindow[1] + 0x2c);
            }
            dragWindow[4] = 1;
            dragWindow[2] = ((int)param_2 - *dragWindow) + 4;
            dragWindow[3] = ((int)param_3 - dragWindow[1]) + 4;
            resIndex = dragWindow[1];
            dragWindow[4] = 0;
            tmp = *dragWindow;
            resWindow[4] = 1;
            resWindow[2] = (param_2 - tmp) + 4;
            resWindow[3] = (param_3 - resIndex) + 4;
        }
        else
        {
            dragWindow[4] = 0;
            if ((int)param_2 >= EQ_GLOBAL(int, 0x006B63FC) - dragWindow[2])
            {
                dragWindow[4] = 0;
                param_2 = (short)(EQ_GLOBAL(int, 0x006B63FC) - dragWindow[2] - 1);
            }
            dragWindow[4] = 0;
            if ((int)param_3 >= EQ_GLOBAL(int, 0x006B6400) - dragWindow[3])
            {
                dragWindow[4] = 0;
                param_3 = (short)(EQ_GLOBAL(int, 0x006B6400) - dragWindow[3] - 1);
            }
            if (param_2 < 3)
                param_2 = 3;
            if (param_3 < 4)
                param_3 = 4;
            dragWindow[4] = 1;
            *dragWindow = param_2 - 2;
            dragWindow[1] = param_3 - 3;
            resWindow[4] = 1;
            *resWindow = param_2 - 2;
            resWindow[1] = param_3 - 3;
        }
        ((EQ_MainOverlayCallbacks_guess*)selfPointer)->callback8();
        ((EQ_MainOverlayCallbacks_guess*)selfPointer)->callback4();
    }
}

// FUNCTION: TRILOGY 0x0046418e
// Old-UI mouse move: drags windows, then a grabbed scroll thumb (the +0x6c4e0 list or the skill list).
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __thiscall in the binary (ECX = this, callee pops 8). eq_overlay.h does not declare it, so it is a
// __fastcall free function with the dummy EDX argument, like the other EQ_Main methods in this file.
// @early-stop: EBX/EDI are swapped (the parameter/short sits in EBX, the int copy in EDI), so most operands differ; mode 2 reuses the block 2 condition in EDX instead of re-reading the field, which merges the imul into one tail; fimul by the int field comes before fmul.
void __fastcall EQ_Main_MouseMove_guess(EQ_Main* selfPointer, int edxDummy, int param_2, int param_3)
{
    char* self;
    int scrollValue;
    int temp;
    int thumbTop;
    int trackLength;
    short sVar4;

    self = (char*)selfPointer;
    EQ_THISCALL(void, 0x00463ED9, (EQ_Main*, int, int, int))(selfPointer, edxDummy, param_2, param_3);
    if (*(int *)(self + 0x5f180) != 0)
    {
        if (g_game_state_guess == 2)
        {
            temp = *(int *)(self + 0x90268);
            *(int *)(self + 0x90274) = 0;
            temp += 5;
            if ((int)(short)param_3 > temp + 0xf0)
                param_3 = temp + 0xf0;
            if ((int)(short)param_3 < temp)
                param_3 = temp;
            sVar4 = (short)param_3;
            scrollValue = (int)((float)((int)sVar4 - temp) * EQ_GLOBAL(float, 0x00535580) * (float)*(int *)(self + 0x6c4dc));
        }
        else
        {
            if (0x14f < (short)param_3)
                param_3 = 0x14f;
            if ((short)param_3 < 0x21)
                param_3 = 0x21;
            sVar4 = (short)param_3;
            scrollValue = (int)((float)((int)sVar4 - 0x21) * EQ_GLOBAL(float, 0x0053557C) * (float)*(int *)(self + 0x6c4dc));
        }
        *(int *)(self + 0x6c4e0) = scrollValue;
        *(int *)(*(int *)(self + 0x6c5dc) + 4) = (int)sVar4;
        *(int *)(*(int *)(self + 0x6c5dc) + 0xc) = sVar4 + 0xb;
        CopyRect((LPRECT)(self + *(char *)(self + 0xfe0) * 0x1900 + 0x6cfcc), *(RECT **)(self + 0x6c5dc));
        *(int *)(self + 0xeb8) = 1;
    }
    sVar4 = (short)param_3;
    if (*(int *)(self + 0x5f174) != 0 && (temp = *(int *)(self + 0x685bc) - 6, 0 < temp))
    {
        if (g_game_state_guess == 2)
        {
            temp = *(int *)(self + 0x90188);
            *(int *)(self + 0x90194) = 0;
            temp += 0x14;
            if ((int)sVar4 > temp + 0x15e)
                sVar4 = temp + 0x15e;
            if ((int)sVar4 < temp + 0x21)
                sVar4 = temp + 0x21;
            thumbTop = (int)sVar4;
            trackLength = 0x140;
            temp = (*(int *)(self + 0x685bc) - 6) * (thumbTop - 0x21);
        }
        else
        {
            if (0x1b5 < sVar4)
                sVar4 = 0x1b5;
            if (sVar4 < 0x35)
                sVar4 = 0x35;
            thumbTop = (int)sVar4;
            trackLength = 0x180;
            temp = (thumbTop - 0x35) * temp;
        }
        *(short *)(self + 0x685c8) = (short)(temp / trackLength);
        *(int *)(*(int *)(self + 0x6c5d0) + 4) = thumbTop;
        *(int *)(*(int *)(self + 0x6c5d0) + 0xc) = thumbTop + 0xb;
        CopyRect((LPRECT)(self + *(char *)(self + 0xfe0) * 0x1900 + 0x6cbac), *(RECT **)(self + 0x6c5d0));
        *(int *)(self + 0xeb8) = 1;
    }
}

// FUNCTION: TRILOGY 0x00464352
// Scroll-thumb drag of an unidentified 20-row list (nodes at 0x006E4D70): mouse y picks the top node.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall bbMouseMove(EQ_Main* selfPointer, int edxDummy, int unused, short mouseY)
{
    short entryCount;
    short topIndex;
    int node;
    int iVar4;
    int* rowPtr;
    EQ_Main* self;
    int panel;
    int sourceOffset;
    typedef BOOL (__stdcall *CopyRect_guess_t)(LPRECT, const RECT*);

    self = (EQ_Main *)selfPointer;
    if ((self->skillListThumbDrag_guess != 0) && (self->boardRequestPending_guess == '\0'))
    {
        if (0x16d < mouseY)
            mouseY = 0x16d;
        if (mouseY < 0x51)
            mouseY = 0x51;
        entryCount = self->boardMessageCount_guess;
        if (entryCount <= 1)
            self->boardScrollTop_guess = 0;
        else
        {
            topIndex = (short)(((mouseY + -0x51) * (entryCount + -1)) / 0x11c);
            self->boardScrollTop_guess = topIndex;
            if (topIndex < 0)
                self->boardScrollTop_guess = 0;
            if ((int)self->boardScrollTop_guess > entryCount + -1)
                self->boardScrollTop_guess = entryCount + -1;
        }
        self->boardSelected_guess = 0;
        node = EQ_GLOBAL(int, 0x006E4D70);
        if (0 < self->boardScrollTop_guess)
        {
            iVar4 = (int)self->boardScrollTop_guess;
            do
            {
                node = *(int*)(node + 4);
                iVar4 = iVar4 + -1;
            } while (iVar4 != 0);
        }
        rowPtr = (int*)((char*)self + 0x138);
        iVar4 = 0x14;
        do
        {
            *rowPtr = 0;
            if (node != 0)
            {
                *rowPtr = node;
                node = *(int*)(node + 4);
            }
            rowPtr = rowPtr + 1;
            iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
        *(int*)(self->scrollThumbRect_int_guess + 4) = (int)mouseY;
        *(int*)(self->scrollThumbRect_int_guess + 0xc) = mouseY + 0xb;
        panel = (int)*(signed char*)((char *)self + 0xfe0);
        sourceOffset = panel * 0x1900 + 0x6c96c;
        EQ_GLOBAL(CopyRect_guess_t, 0x00534240)((RECT*)((char*)self + sourceOffset),
            (RECT*)self->scrollThumbRect_int_guess);
        self->uiDirty_u32_guess = 1;
    }
}

// FUNCTION: TRILOGY 0x00464468
// Trains the selected skill or language with a trainer, charging the fee and updating the character.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: VC6 keeps kind in EBX, so the constant 1 is not hoisted to entry (frame 0x94 vs 0x98; EBX/EDI choice, lea offsets and the entry block follow from that)
#define DOLEARN_SELECTED(self) (*(short*)((self) + *(short*)((self) + 0x1008) * 2 + 0x6c7ee))
void EQ_Main::doLearn(void)
{
    struct DoLearnPacket_guess
    {
        int spawnId;
        int kind;
        int skill;
    };
    int cost;
    int mustLearn;
    int kind;
    DoLearnPacket_guess packet;
    char message[128];
    char* self;
    short skill;
    short count;
    short i;
    short j;
    int k;
    int* fill;

    self = (char*)this;
    mustLearn = 0;
    skill = DOLEARN_SELECTED(self);
    if (skill > 99)
    {
        if (EQ_CDECL(int, 0x004A0F1C, (void*, int, int, int))(
                g_pc_guess, *(u8*)(self + skill + 0x5f038), 1, skill - 100) == 0)
            return;
        kind = 1;
        skill = DOLEARN_SELECTED(self) - 100;
    }
    else
    {
        if (EQ_CDECL(int, 0x004A0F1C, (void*, int, int, int))(
                g_pc_guess, *(u8*)(self + skill + 0x5f038), 0, skill) == 0)
            return;
        skill = DOLEARN_SELECTED(self);
        kind = 0;
        if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)(*(u8*)&DOLEARN_SELECTED(self)) == 0)
            mustLearn = 1;
    }
    cost = EQ_CDECL(int, 0x004A09A4, (void*, int, float, int))(
        g_pc_guess, kind, *(float*)(self + 0x1018), (int)skill);
    if (cost > g_pc_guess->copper + (g_pc_guess->silver + (g_pc_guess->gold + g_pc_guess->platinum * 10) * 10) * 10)
    {
        sprintf(
            message, EQ_ARRAY(char, 0x0055B58C),
            EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
            g_skill_records_guess[DOLEARN_SELECTED(self)]);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
        return;
    }
    if (cost != 0)
        EQ_CDECL(void, 0x004A406D, (void*, int))(g_pc_guess, cost);
    packet.skill = (int)skill;
    packet.spawnId = *(int*)(EQ_GLOBAL(char*, 0x006E4DC4) + 0x80);
    packet.kind = kind;
    send_message_guess(
        g_world_connection_guess, 0x2140, &packet, 0xc, 1);
    if (mustLearn != 0)
    {
        g_pc_guess->skills[DOLEARN_SELECTED(self)] =
            *(u8*)(g_skill_records_guess[DOLEARN_SELECTED(self)] + 0x28 + g_pc_guess->class_);
        g_pc_guess->training_points--;
        sprintf(
            message, EQ_ARRAY(char, 0x0055B558),
            EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
            g_skill_records_guess[DOLEARN_SELECTED(self)]);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
        *(u32*)(self + 0xeb8) = 1;
        fill = (int*)(self + 0x6c726);
        for (k = 0x32; k != 0; k--)
            *fill++ = -1;
        count = 0;
        i = 0;
        do
        {
            if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)((u8)i) != 0)
                *(short*)(self + count++ * 2 + 0x6c726) = i;
            i++;
        } while (i < 100);
        j = 0;
        k = 0;
        do
        {
            if (*(u8*)((char*)g_pc_guess + k + 0x7e) > 0)
                *(short*)(self + count++ * 2 + 0x6c726) = j + 100;
            j++;
            k++;
        } while (j < 0x20);
        *(int*)(self + 0x685c4) = (int)(char)count;
    }
    else
    {
        skill = DOLEARN_SELECTED(self);
        if (skill > 99)
        {
            if (*((char*)g_pc_guess + skill + 0x1a) == 0)
            {
                sprintf(
                    message, EQ_ARRAY(char, 0x0055B520),
                    EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
                    EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), pinstCEverQuest_guess)((short)(skill - 100)));
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
            }
            else
            {
                sprintf(
                    message, EQ_ARRAY(char, 0x0055B4E8),
                    EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
                    EQ_METHOD(1, char*, 0x004C8FCE, (void*, short), pinstCEverQuest_guess)((short)(skill - 100)));
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
            }
            g_pc_guess->training_points--;
            *((char*)g_pc_guess + DOLEARN_SELECTED(self) + 0x1a) += 1;
        }
        else
        {
            g_pc_guess->training_points--;
            g_pc_guess->skills[DOLEARN_SELECTED(self)] += 1;
            sprintf(
                message, EQ_ARRAY(char, 0x0055B4B0),
                EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), pinstCEverQuest_guess)(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
                g_skill_records_guess[DOLEARN_SELECTED(self)]);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), pinstCEverQuest_guess)(message, 0x111);
        }
        *(u32*)(self + 0xeb8) = 1;
    }
    (*(void (__fastcall **)(void*, int))(*(int*)self + 4))(self, 0);
}
#undef DOLEARN_SELECTED

// FUNCTION: TRILOGY 0x0046486c
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Main::MouseMoveGm(int unused, short y)
{
    char* self;
    int total;
    int delta;
    int numerator;
    int position;
    char* thumb;
    char* copySource;
    int panel;
    int sourceOffset;

    self = (char*)this;
    if (*(int*)(self + 0x5f178) == 0)
        return;
    total = *(int*)(self + 0x685c0);
    if (total - 5 <= 0)
        return;

    if (y > 0x148)
        y = 0x148;
    if (y < 0x32)
        y = 0x32;
    delta = (int)y - 0x32;
    numerator = delta * (total - 5);
    position = numerator / 0x116;
    thumb = *(char**)(self + 0x6c5d4);
    *(s16*)(self + 0x685cc) = (s16)position;
    *(int*)(thumb + 4) = (int)y;
    thumb = *(char**)(self + 0x6c5d4);
    *(int*)(thumb + 0xc) = (int)((u32)(int)y + 0xbu);
    copySource = *(char**)(self + 0x6c5d4);
    panel = (int)*(signed char*)(self + 0xfe0);
    sourceOffset = panel * 0x1900 + 0x6ca5c;
    CopyRect((RECT*)(self + sourceOffset), (RECT*)copySource);
    *(u32*)(self + 0xeb8) = 1;
}

// FUNCTION: TRILOGY 0x004648f9
void EQ_Main::MouseMoveMe_ModelScrollPosition_guess(int unused, short y)
{
    EQ_Main* self;
    int height;
    int delta;
    int numerator;
    int position;
    char* thumb;
    char* copySource;
    int panel;
    int sourceOffset;

    self = (EQ_Main *)this;
    if (self->skillWindowThumbDrag_guess == 0)
        return;
    height = self->skillWindowCount_guess;
    if (height - 5 <= 0)
        return;

    if (y > 0x1b1)
        y = 0x1b1;
    if (y < 0x9b)
        y = 0x9b;
    delta = (int)y - 0x9b;
    numerator = (int)((u32)delta * (u32)(height - 5));
    position = numerator / 0x116;
    thumb = self->skillWindowThumbRect_bytes_guess;
    self->skillWindowTop_guess = (s16)position;
    *(int*)(thumb + 4) = (int)y;
    thumb = self->skillWindowThumbRect_bytes_guess;
    *(int*)(thumb + 0xc) = (int)((u32)(int)y + 0xbu);
    copySource = self->skillWindowThumbRect_bytes_guess;
    panel = (int)*(signed char*)((char *)self + 0xfe0);
    sourceOffset = panel * 0x1900 + 0x6ca6c;
    CopyRect((RECT*)((char*)self + sourceOffset), (RECT*)copySource);
  self->uiDirty_u32_guess = 1;
}

// FUNCTION: TRILOGY 0x0046498f
// Opens merchant trading on an accepted response; otherwise reports that the merchant is busy.
void __fastcall HandleMerchantResponse_guess(EQ_Main* self, int unused, char param_2)
{
    typedef void (__fastcall *tOverlayMethod_guess_t)(void*);     // one arg: ECX only, the vtable calls are thiscall
    char message[128];
    int* slot;
    int remaining;
    int vtable;

    if (param_2 == '\x01')
    {
        if (g_game_state_guess == 2 &&
            (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            pinstEQ_Main_guess->uiDirty_u32_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }

        slot = (int*)((char*)self + 0x6C5E0);
        remaining = 0x1E;
        do
        {
            if (*slot != 0)
            {
                operator delete( (void*)*slot );
                *slot = 0;
            }
            slot = slot + 1;
            remaining = remaining - 1;
        } while (remaining != 0);

        vtable = (int)self->vftable_guess;
        self->merchantEndSent_guess = 0;
        self->merchantReady_u32_guess = 1;
        ((tOverlayMethod_guess_t)(*(u32*)(vtable + 8)))(self);
        self->listPage_guess = 0;
        self->prevScreenMode_u8_guess = self->screenMode_u8_guess;
        self->renderDollActive_guess = 1;
        self->screenMode_u8_guess = 4;
        self->merchantOpen_guess = 1;
        vtable = (int)self->vftable_guess;
        ((tOverlayMethod_guess_t)(*(u32*)(vtable + 4)))(self);
        if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(int, 0x006ECDF4) != 0)
        {
            EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x12);
        }
    }
    else
    {
        sprintf(message, EQ_ARRAY(char, 0x0055B5EC),
             EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), g_everquest_guess)(
                  EQ_GLOBAL(char*, 0x006E4DB8) + 1));
        EQ_GLOBAL(void*, 0x006E4DB8) = 0;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
    }
    return;
}

// FUNCTION: TRILOGY 0x00464ab6
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_ClearItems_guess(void* self, int unused)
{
    u32 message[2];

    *(u32*)((char*)self + 0xa0) = 0;
    if (EQ_GLOBAL(char*, 0x006E4DB8) != 0 && *(int*)((char*)self + 0x8c) == 0)
    {
        *(int*)((char*)self + 0x8c) = 1;
        message[0] = *(u32*)(EQ_GLOBAL(char*, 0x006E4DB8) + 0x80);
        message[1] = g_local_player_guess->spawn_id;
        send_message_guess(
            g_world_connection_guess, 0x2037, message, 8, 1);
    }
}

// FUNCTION: TRILOGY 0x00464b15
// Ends an active interaction, saves the character, and clears its player and item state.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_ReleaseMerchant_guess(void* self, int unused)
{
    typedef void (__fastcall *tOverlayMethod_guess_t)(void*);     // one arg: ECX only, the overlay vtable slots take no stack arguments
    int message[0x17];
    int* count;
    int vtable;
    int active;
    char* sub;
    char* object;

    if (*(int*)((char*)self + 0x685E0) != 0)
    {
        if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(int, 0x006ECDF4) != 0)
        {
            EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
        }
        EQ_GLOBAL(int, 0x006B7120) = 10;
        EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
        *(u32*)((char*)self + 0xA0) = 0;

        active = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))();
        if (active != 0)
        {
            sub = *(char**)(EQ_GLOBAL(char*, 0x006E4E58) + 0x174);
            if (sub != 0)
                *(u8*)(sub + 0xD6) = 0;
            object = EQ_GLOBAL(char*, 0x006E4E58);
            ((tOverlayMethod_guess_t)(*(u32*)(*(u32*)object + 8)))(object);
            *(int*)(EQ_GLOBAL(char*, 0x006E4E58) + 0x174) = 0;
        }
        active = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))();
        if (active != 0)
        {
            sub = *(char**)(EQ_GLOBAL(char*, 0x006E4E58) + 0x174);
            if (sub != 0)
                *(u8*)(sub + 0xD6) = 0;
            object = EQ_GLOBAL(char*, 0x006E4E5C);
            ((tOverlayMethod_guess_t)(*(u32*)(*(u32*)object + 8)))(object);
            *(int*)(EQ_GLOBAL(char*, 0x006E4E5C) + 0x174) = 0;
        }

        *(int*)((char*)self + 0x8C) = 1;
        EQ_GLOBAL(int, 0x006E4DB8) = 0;
        vtable = *(int*)self;
        ((tOverlayMethod_guess_t)(*(u32*)(vtable + 8)))(self);
        *(u8*)((char*)self + 0xFE0) = *(u8*)((char*)self + 0xFE1);
        vtable = *(int*)self;
        *(u32*)((char*)self + 0x685E0) = 0;
        ((tOverlayMethod_guess_t)(*(u32*)(vtable + 4)))(self);

        EQ_GLOBAL(int, 0x006E4DAC) = 0;
        pinstEQ_Main_guess->pendingServerReplies_guess = pinstEQ_Main_guess->pendingServerReplies_guess - 1;
        count = (int*)(g_eqmain_guess + 0xE5C);
        if (*count != 0)
        {
            message[0] = -4;
            message[1] = *count;
            message[2] = 5;
            send_message_guess(
                g_world_connection_guess, 0x2147, message, 0x5C, 1);
        }

        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
        if (EQ_GLOBAL(int, 0x00590170) == 2 && g_game_state_guess != 2)
        {
            g_game_state_guess = 2;
            *(int*)((char*)self + 0xEB8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
    }
}

// FUNCTION: TRILOGY 0x00464c8d
// Clears a pending target interaction and sends its target ID to the server.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_ClearLoot_guess(EQ_Main* self, int unused)
{
    self->lootReady_guess = 0;
    if (EQ_GLOBAL(char*, 0x006E4DC0) != 0 && self->lootEndSent_guess == 0)
    {
        self->lootEndSent_guess = 1;
        send_message_guess(
            g_world_connection_guess, 0x204F,
            (void*)(EQ_GLOBAL(char*, 0x006E4DC0) + 0x80), 4, 1);
    }
}

// FUNCTION: TRILOGY 0x00464cd1
// Finalizes the corpse loot session, saves the character, and updates the server.
// Ends looting / leaves the loot state (EQ_Main, __thiscall ECX = self, no stack args). Does nothing unless
// self+0x685dc is set. The second overlay block reads the 0x174 item of 0x006E4E58 but releases 0x006E4E5C (as the original).
void __fastcall EQ_Main_EndLoot_guess(EQ_Main* self)
{
    typedef void (__fastcall *tOverlayVFunc_guess_t)(void*);     // one arg: ECX only, the vtable calls are thiscall
    u32 packet[23];         // 0x5C bytes sent with opcode 0x2147; only packet[0..2] are set here
    int* count;
    int* item;

    if (pinstEQ_Main_guess->lootOpen_guess != 0)
    {
        if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
        {
            EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
        }
        EQ_GLOBAL(char*, 0x006E4DAC) = 0;
        EQ_GLOBAL(char*, 0x006E4DC0) = 0;
        self->lootEndSent_guess = 1;
        EQ_GLOBAL(int, 0x006B7120) = 0xb;
        EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
        self->lootReady_guess = 0;
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0)
        {
            item = *(int**)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174);
            if (item != 0)
            {
                *((char*)item + 0xd6) = 0;
            }
            ((tOverlayVFunc_guess_t)(*(u32**)EQ_GLOBAL(void*, 0x006E4E58))[2])(EQ_GLOBAL(void*, 0x006E4E58));
            *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) = 0;
        }
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0)
        {
            item = *(int**)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174);
            if (item != 0)
            {
                *((char*)item + 0xd6) = 0;
            }
            ((tOverlayVFunc_guess_t)(*(u32**)EQ_GLOBAL(void*, 0x006E4E5C))[2])(EQ_GLOBAL(void*, 0x006E4E5C));
            *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174) = 0;
        }
        EQ_METHOD(1, void, 0x004B45CB, (void*, int), EQ_GLOBAL(void*, 0x006E4DA0))(0x64);
        ((tOverlayVFunc_guess_t)(*(u32**)self)[2])(self);
        self->screenMode_guess = self->prevScreenMode_guess;
        self->lootOpen_guess = 0;
        ((tOverlayVFunc_guess_t)(*(u32**)self)[1])(self);
        pinstEQ_Main_guess->pendingServerReplies_guess -= 1;
        count = (int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xe5c);
        if (*count != 0)
        {
            packet[0] = 0xfffffffc;
            packet[1] = *count;
            packet[2] = 6;
            send_message_guess(
                g_world_connection_guess, 0x2147, packet, 0x5c, 1);
        }
        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
        if (EQ_GLOBAL(int, 0x00590170) == 2 && g_game_state_guess != 2)
        {
            g_game_state_guess = 2;
            self->uiDirty_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
    }
}

// FUNCTION: TRILOGY 0x00464e5b
// Applies a corpse loot reply, credits any coins, and opens or closes the loot interface.
// Loot reply (EQ_Main). lootResult 0: someone else is looting the corpse; 2: no looting now; 1 or 3: the coins
// were taken. lootResult is a char (Ghidra: char param_2), only its low byte is compared. slotCount is the 30-slot
// loop counter; VC6 gives it the dead lootResult stack slot, as the original does.
void __fastcall EQ_Main_LootResult_guess(EQ_Main* self, int unused, char lootResult, int platinum, int gold, int silver, int copper)
{
    typedef void (__fastcall *tOverlayVFunc_guess_t)(void*);     // one arg: ECX only, the vtable calls are thiscall
    u32 packet[23];         // 0x5C bytes sent with opcode 0x2147; only packet[0..2] are set here
    char coins[128];
    char message[256];
    u16* slotFlags;
    int* slotPtr;
    char oldStance;
    int slotCount;

    if (lootResult == 2)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B6A0), 0x111);
        EQ_GLOBAL(char*, 0x006E4DC0) = 0;
        *(u32*)((char*)self + 0xa4) = 0;
        pinstEQ_Main_guess->pendingServerReplies_guess -= 1;
        if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
        {
            packet[0] = 0xfffffffc;
            packet[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
            packet[2] = 7;
            send_message_guess(
                g_world_connection_guess, 0x2147, packet, 0x5c, 1);
        }
        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
    }
    else if (lootResult == 1 || lootResult == 3)
    {
        self->lootEndSent_guess = 0;
        *(u32*)((char*)self + 0xa4) = 1;
        if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
        {
            EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x16);
        }
        if (g_game_state_guess == 2 &&
            (g_pcloudFlagA_guess == 0 || EQ_GLOBAL(int, 0x006B63FC) == 0x280))
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            self->uiDirty_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x998) += platinum;
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x99c) += gold;
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x9a0) += silver;
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x9a4) += copper;
        if (0 < platinum + gold + silver + copper)
        {
            if (g_sound_manager_ptr_guess != 0)
            {
                EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x8a, 0);
            }
            coins[0] = 0;
            EQ_CDECL(void, 0x004A8610, (int, int, int, int, char*))(platinum, gold, silver, copper, coins);
            if (g_pc_guess->autosplit_int_guess != 0)
            {
                sprintf(message, EQ_ARRAY(char, 0x0055B658), coins);
            }
            else
            {
                sprintf(message, EQ_ARRAY(char, 0x0055B638), coins);
            }
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x11d);
        }
        slotFlags = &self->slotFlags_guess[0];
        slotPtr = (int*)((char*)self + 0x6c5e0);
        slotCount = 0x1e;
        do
        {
            if (*slotPtr != 0)
            {
                operator delete((void*)*slotPtr);
                *slotPtr = 0;
            }
            *slotFlags = 0xffff;
            slotPtr = slotPtr + 1;
            slotFlags = slotFlags + 1;
            slotCount--;
        } while (slotCount != 0);
        self->listPage_guess = 0;
        EQ_METHOD(1, void, 0x004B45CB, (void*, int), EQ_GLOBAL(void*, 0x006E4DA0))(0x69);
        ((tOverlayVFunc_guess_t)(*(u32**)self)[2])(self);     // vtable slot 2, ECX = self
        oldStance = self->screenMode_guess;
        self->showEquipment_guess = 0;
        self->screenMode_guess = 4;
        self->renderDollActive_guess = 1;
        self->lootOpen_guess = 1;
        self->prevScreenMode_guess = oldStance;
        ((tOverlayVFunc_guess_t)(*(u32**)self)[1])(self);     // vtable slot 1, ECX = self
    }
    else if (lootResult == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B678), 0x111);
        EQ_GLOBAL(char*, 0x006E4DC0) = 0;
        pinstEQ_Main_guess->pendingServerReplies_guess -= 1;
        if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
        {
            packet[0] = 0xfffffffc;
            packet[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
            packet[2] = 7;
            send_message_guess(
                g_world_connection_guess, 0x2147, packet, 0x5c, 1);
        }
        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
        *(u32*)((char*)self + 0xa4) = 0;
    }
}

#define CHARICON_IMG_guess(i)   EQ_ARRAY(void*, 0x006B38F4)[i]
#define CHARICON_DRAW_guess(img) \
    EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), (img))(&rect, 1)

// FUNCTION: TRILOGY 0x00465128
// Animates the class icon; for casters, the toggle that opens the spellbook or closes it and stands up.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: thiscall address wrappers still set dummy EDX; the sentinel stores remain separate immediate ORs.
// __thiscall (ECX = self, no stack args). Draws the class' animated icon (image table at 0x006B38F4) from the
// time since self+0x6c588, then the sit/stand icon (timer at self+0x6c554) and finishes a stand/sit toggle.
void EQ_Main::doCharIconAnim_guess(void)
{
    void* self = (void*)this;
    typedef void (__fastcall *tOverlayVFunc_guess_t)(void*);     // one arg: ECX only, the vtable calls are thiscall
    RECT rect;
    u32 elapsed;
    u32 delta;
    u8 cls;
    char oldStance;

    *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_Main_guess->overlaySprite_int_guess;
    SetRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 0xb, 0x52, 0x51, 0x101);
    EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_GLOBAL(void*, 0x006B341C))((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4), 1);
    if (*(int*)((char*)self + 0x6c588) == 0)
    {
        elapsed = *(int*)((char*)g_display_guess + 0xbc);
        *(int*)((char*)self + 0x6c58c) = 0;
        *(u16*)((char*)self + 0x6c55a) = 0xffff;
        *(u16*)((char*)self + 0x6c558) = 0;
        *(int*)((char*)self + 0x6c588) = elapsed;
        return;
    }
    elapsed = *(int*)((char*)g_display_guess + 0xbc) - *(int*)((char*)self + 0x6c588);
    cls = g_pc_guess->class_;
    if (cls == 1)
    {
        SetRect(&rect, 0xb, 0x61, 0x51, 0x103);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(16));
        }
        else
        {
            if (elapsed > 0x5db)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(15));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x1963)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 0xe)
    {
        SetRect(&rect, 10, 0x5b, 0x51, 0xb8);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(7));
        }
        else
        {
            if (elapsed > 0x4af)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else if (elapsed < 700)
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            else if (elapsed < 0x4b0)
            {
                elapsed = 0xc - elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x1837)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 0xb)
    {
        SetRect(&rect, 10, 0x5a, 0x51, 200);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(8));
        }
        else
        {
            if (elapsed > 0x257)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(7));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed + 1));
            }
            if (elapsed > 0x15df)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 5)
    {
        SetRect(&rect, 10, 0x5a, 0x51, 0xd2);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(8));
        }
        else
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess((elapsed / 100) % 6));
            if (elapsed > 0x15df)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 3)
    {
        SetRect(&rect, 10, 0x5a, 0x51, 0xd2);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(8));
        }
        else
        {
            if (elapsed > 0x31f)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x16a7)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 8)
    {
        SetRect(&rect, 0xb, 0x52, 0x51, 0xd1);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(6));
        }
        else
        {
            if (elapsed > 0x257)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x15df)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 6)
    {
        SetRect(&rect, 0xb, 0x5c, 0x51, 0xce);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(12));
        }
        else
        {
            if (elapsed > 0x897)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else if (elapsed < 0x44c)
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            else if (elapsed < 0x898)
            {
                elapsed = 10 - (elapsed - 0x44c) / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x1c1f)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 2)
    {
        SetRect(&rect, 0x13, 0x5b, 0x51, 0xd7);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(12));
        }
        else
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess((elapsed / 100) % 12));
            if (elapsed > 0x4af)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 0xd)
    {
        SetRect(&rect, 0x12, 0x62, 0x4c, 0xd2);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(15));
        }
        else
        {
            if (elapsed > 0x44b)
            {
                switch (((elapsed - 0x44c) / 100) & 3)
                {
                case 0:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
                    break;
                case 1:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(12));
                    break;
                case 2:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(13));
                    break;
                case 3:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(14));
                    break;
                }
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed + 1));
            }
            if (elapsed > 0x17d3)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 4)
    {
        SetRect(&rect, 0x11, 0x5a, 0x4d, 0xde);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(9));
        }
        else
        {
            if (elapsed > 0x383)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x170b)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 0xc)
    {
        SetRect(&rect, 0x16, 0x5c, 0x4a, 0xca);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(8));
        }
        else
        {
            if (elapsed > 0x1f3)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(((elapsed - 500) / 100) % 3));
            }
            else
            {
                elapsed = elapsed / 100;
                switch (elapsed)
                {
                case 0:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(3));
                    break;
                case 1:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(4));
                    break;
                case 2:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(5));
                    break;
                case 3:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(6));
                    break;
                case 4:
                    CHARICON_DRAW_guess(CHARICON_IMG_guess(7));
                    break;
                default:
                    break;
                }
            }
            if (elapsed > 0x157b)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 7)
    {
        SetRect(&rect, 0x13, 0x66, 0x4e, 0xfe);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(13));
        }
        else
        {
            if (elapsed > 0x513)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x189b)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 9)
    {
        SetRect(&rect, 0x11, 0x5a, 0x4d, 0xfd);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(7));
        }
        else
        {
            if (elapsed > 0x2bb)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x1643)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }
    else if (cls == 10)
    {
        SetRect(&rect, 0xc, 0x52, 0x52, 0xd0);
        if (*(short*)((char*)self + 0x5f188) == 3)
        {
            CHARICON_DRAW_guess(CHARICON_IMG_guess(5));
        }
        else
        {
            if (elapsed > 0x1f3)
            {
                CHARICON_DRAW_guess(CHARICON_IMG_guess(0));
            }
            else
            {
                elapsed = elapsed / 100;
                CHARICON_DRAW_guess(CHARICON_IMG_guess(elapsed));
            }
            if (elapsed > 0x157b)
                *(int*)((char*)self + 0x6c588) = *(int*)((char*)g_display_guess + 0xbc);
        }
    }

    cls = g_pc_guess->class_;
    if (cls == 1 || cls == 7 || cls == 9)
        return;
    if (*(int*)((char*)self + 0x6c554) == 0)
    {
        if (pinstEQ_Main_guess->screenMode_guess == 5)
        {
            SetRect(&rect, 0x10, 0xd6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(53));
        }
        else
        {
            SetRect(&rect, 0x10, 0xd3, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(50));
        }
        return;
    }
    delta = *(int*)((char*)g_display_guess + 0xbc) - *(int*)((char*)self + 0x6c554);
    if (delta < 100)
    {
        if (pinstEQ_Main_guess->screenMode_guess == 5)
        {
            SetRect(&rect, 0x10, 0xc6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(52));
        }
        else
        {
            SetRect(&rect, 0x10, 0xc6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(51));
        }
        return;
    }
    if (delta < 200)
    {
        if (pinstEQ_Main_guess->screenMode_guess == 5)
        {
            SetRect(&rect, 0x10, 0xc6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(51));
        }
        else
        {
            SetRect(&rect, 0x10, 0xc6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(52));
        }
        return;
    }
    if (delta < 300)
    {
        if (pinstEQ_Main_guess->screenMode_guess == 5)
        {
            SetRect(&rect, 0x10, 0xd3, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(50));
        }
        else
        {
            SetRect(&rect, 0x10, 0xd6, 0x15, 0xdc);
            CHARICON_DRAW_guess(CHARICON_IMG_guess(53));
        }
        return;
    }
    *(int*)((char*)self + 0x6c554) = 0;
    if (pinstEQ_Main_guess->screenMode_guess == 5)
    {
        SetRect(&rect, 0x10, 0xd3, 0x15, 0xdc);
        CHARICON_DRAW_guess(CHARICON_IMG_guess(50));
        EQ_METHOD(1, void, 0x004B45CB, (void*, int), EQ_GLOBAL(void*, 0x006E4DA0))(0x64);
        ((tOverlayVFunc_guess_t)(*(u32**)self)[2])(self);     // vtable slot 2, ECX = self
        *(u16*)((char*)self + 0xfe8) = 0;
        if (*(int*)((char*)g_pc_guess + 0xe0) == 0)
            g_cdisplay_guess->itemOnCursor_guess = 0;
        oldStance = *(char*)((char*)self + 0xfe0);
        *(char*)((char*)self + 0xfe0) = 0;
        *(char*)((char*)self + 0xfe2) = oldStance;
        *(u16*)((char*)self + 0x1004) = 0xffff;
        *(u16*)((char*)self + 0x1006) = 0xffff;
        *(u16*)((char*)self + 0x5f186) = 0xffff;
        *(u16*)((char*)self + 0x5f188) = 0xffff;
        *(u16*)((char*)self + 0x5f18a) = 0xffff;
        *(int*)((char*)self + 0xfe4) = 0;
        *(int*)((char*)self + 0xeb8) = 1;
        ((tOverlayVFunc_guess_t)(*(u32**)self)[1])(self);     // vtable slot 1, ECX = self
        if (EQ_GLOBAL(int, 0x00590170) != 2)
            return;
        g_game_state_guess = 2;
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            pinstEQ_Main_guess->uiDirty_guess = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
    }
    else
    {
        SetRect(&rect, 0x10, 0xd6, 0x15, 0xdc);
        CHARICON_DRAW_guess(CHARICON_IMG_guess(53));
        EQ_METHOD(0, void, 0x00465A60, (void*), self)();
    }
}

#undef CHARICON_DRAW_guess
#undef CHARICON_IMG_guess

// FUNCTION: TRILOGY 0x00465a60
// Opens the spellbook view (5): sits unless stance 'n'/'f', saves the old view, sets +0xfe4 if skill 31 known.
/* fake match: a local abstract interface keeps VC6 from tail-jumping the final virtual slot call. */
void EQ_Main::ActivateSpellbook(void)
{
    int* self = (int*)this;
    class tOverlayCall_guess_t
    {
    public:
        virtual void First_guess() = 0;
        virtual void Second_guess() = 0;
        virtual void Third_guess() = 0;
    };
    char standState;
    char hasSkill;

    standState = *(char*)((char*)EQ_GLOBAL(void*, 0x006E4D8C) + 0xa1);
    if (standState != 'n' && standState != 'f')
        EQ_METHOD(0, void, 0x004CB6E6, (void*), g_everquest_guess)();

    ((tOverlayCall_guess_t*)self)->Third_guess();
    *(short*)((char*)self + 0x5f186) = -1;
    *(short*)((char*)self + 0x5f188) = -1;
    *(short*)((char*)self + 0x5f18a) = -1;
    *(char*)((char*)self + 0xfe2) = *(char*)((char*)self + 0xfe0);
    *(char*)((char*)self + 0xfe0) = 5;
    hasSkill = EQ_METHOD(1, char, 0x0041A3E9, (void*, u8), EQ_GLOBAL(void*, 0x006E4DA8))(0x1f);
    if (hasSkill != '\0')
        self[0x3f9] = 1;
    self[0x3ae] = 1;
    ((tOverlayCall_guess_t*)self)->Second_guess();
}

// Helpers local to EQ_Main_drawSkills_guess: draw the image kept in a global on the slot rect (drawRect),
// or SetRect a source rect into sourceRect and draw it through the overlay at +4 of an overlay global.
#define SPELLBOOK_DRAW(image_va) \
    EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), \
        EQ_GLOBAL(void*, image_va))(&drawRect, 1)
#define SPELLBOOK_PARTIAL(overlay_va, left, top, right, bottom) \
    pSetRect(&sourceRect, left, top, right, bottom); \
    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), \
        self)(*(void**)(EQ_GLOBAL(char*, overlay_va) + 4), &sourceRect, &drawRect, 1)

// FUNCTION: TRILOGY 0x00465ade
// Draws six skill/language rows of the skills page: icon, a rank image by tens, the number when above 100.
// __fastcall (self in ECX, no stack arguments), no return value. Six slots from the scroll position
// (self+0x685c8, shadowed at self+0x685ca: a change sets the redraw flag at self+0xeb8), each inside the count at
// self+0x685bc. Same two passes as EQ_Main_DrawSkillList_guess: pass 1 draws a slot when the flag at
// self+0xc is set or one of its marks (self+0x8462c, self+0x8a3ec) is set. Pass 2 skips a slot with no flag and no
// marks, draws its element, clears the mark at self+0x8a3ec, and ends the slot at kind -1 (kind = the entry at
// self+0x6c65e). Otherwise it draws the switch image for the kind (a global per case, or a rect set into sourceRect
// and drawn through the overlay at +4 of one of three globals with drawPartial; none for the default), then the
// image chosen by the skill byte (kind < 100) or name byte (kind >= 100) of g_pc. A skill above 100 that
// HasSkill reports also prints its value beside the slot.
// The case bodies are in the original's layout order (which is not numeric); each draws on its own and VC6 merges
// the identical tails.
void __fastcall EQ_Main_drawSkills_guess(EQ_Main* self)
{
    u8 skillRank;
    short kind;
    int cellIndex;
    int slot;
    tSetRect_guess_t pSetRect;
    char rankText[64];
    RECT sourceRect;
    RECT drawRect;
    int row;

    if (self->skillListTopDrawn_guess != self->skillListTop_guess)
    {
        self->skillListTopDrawn_guess = self->skillListTop_guess;
        self->uiDirty_guess = 1;
    }
    slot = 0;
    row = 6;
    do
    {
        if ((((self->redrawAll_guess != 0) ||
              (cellIndex = self->screenMode_guess * 400 + slot,
               *(int*)((char*)self + 0x8462c + cellIndex * 4) != 0)) ||
             (*(int*)((char*)self + 0x8a3ec + cellIndex * 4) != 0)) &&
            (self->skillListTop_guess + slot < self->skillListCount_guess))
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
                (char*)self + (self->screenMode_guess * 400 + slot) * 0x10 + 0x6e18c, 1);
        }
        slot = slot + 1;
        row = row + -1;
    } while (row != 0);
    row = 0;
    pSetRect = g_setrect_guess;
    do
    {
        if ((((self->redrawAll_guess == 0) &&
              (slot = self->screenMode_guess * 400 + (int)(short)row,
               *(int*)((char*)self + 0x8462c + slot * 4) == 0)) &&
             (*(int*)((char*)self + 0x8a3ec + slot * 4) == 0)) ||
            (self->skillListCount_guess <= (int)self->skillListTop_guess + (int)(short)row))
            goto nextSlot;
        EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
            (char*)self + (self->screenMode_guess * 400 + (int)(short)row) * 0x10 + 0x6e18c, 1);
        *(int*)((char*)self + 0x8a3ec + (self->screenMode_guess * 400 + (int)(short)row) * 4) = 0;
        if (*(short*)((char*)self + 0x6c65e + ((int)self->skillListTop_guess + (int)(short)row) * 2) == -1)
            goto nextSlot;
        cellIndex = (int)(short)row + self->screenMode_guess * 400;
        slot = *(int*)((char*)self + (cellIndex + 0x6e19) * 0x10);
        cellIndex = *(int*)((char*)self + cellIndex * 0x10 + 0x6e18c);
        pSetRect(&drawRect, cellIndex + 7, slot + 6, cellIndex + 200, slot + 200);
        switch (*(short*)((char*)self + 0x6c65e + ((int)self->skillListTop_guess + (int)(short)row) * 2))
        {
        case 0x64: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0x88, 0x1f7, 0x9e); break;
        case 0x65: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0x88, 0x235, 0x9e); break;
        case 0x66: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0x9e, 0x1f7, 0xb4); break;
        case 0x67: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0x9e, 0x235, 0xb4); break;
        case 0x68: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0xb4, 0x1f7, 0xca); break;
        case 0x69: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0xb4, 0x235, 0xca); break;
        case 0x6a: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0xca, 0x1f7, 0xe0); break;
        case 0x6b: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0xca, 0x235, 0xe0); break;
        case 0x6c: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0xe0, 0x1f7, 0xf6); break;
        case 0x6d: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0xe0, 0x235, 0xf6); break;
        case 0x6e: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0x122, 0x273, 0x138); break;
        case 0x6f: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0x122, 0x1f7, 0x138); break;
        case 0x70: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0x10c, 0x273, 0x122); break;
        case 0x71: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0xf6, 0x1f7, 0x10c); break;
        case 0x72: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0xf6, 0x235, 0x10c); break;
        case 0x73: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1ba, 0x10c, 0x1f7, 0x122); break;
        case 0x74: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0x10c, 0x235, 0x122); break;
        case 0x75: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0x88, 0x273, 0x9e); break;
        case 0x76: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0x9e, 0x273, 0xb4); break;
        case 0x77: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0xb4, 0x273, 0xca); break;
        case 0x78: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0xca, 0x273, 0xe0); break;
        case 0x79: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0xe0, 0x273, 0xf6); break;
        case 0x7a: SPELLBOOK_PARTIAL(0x006E4E1C, 0x236, 0xf6, 0x273, 0x10c); break;
        case 0x7b: SPELLBOOK_PARTIAL(0x006E4E1C, 0x1f8, 0x122, 0x235, 0x138); break;
        case 0x3a: SPELLBOOK_PARTIAL(0x006E4DE8, 0x218, 0xdc, 0x255, 0xf2); break;
        case 0x39: SPELLBOOK_PARTIAL(0x006E4DE8, 0x1d9, 0xdc, 0x216, 0xf2); break;
        case 0x38: SPELLBOOK_PARTIAL(0x006E4DE8, 0x218, 0xc6, 0x255, 0xdc); break;
        case 0x3b: SPELLBOOK_PARTIAL(0x006E4DE8, 0x1d9, 0xc6, 0x216, 0xdc); break;
        case 0x49: SPELLBOOK_PARTIAL(0x006E4DE8, 0x204, 0x1c7, 0x241, 0x1dd); break;
        case 0x37: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23b, 0xb0, 0x278, 0xc6); break;
        case 0x0: SPELLBOOK_DRAW(0x006B3750); break;
        case 0x1: SPELLBOOK_DRAW(0x006B3748); break;
        case 0x2: SPELLBOOK_DRAW(0x006B375C); break;
        case 0x3: SPELLBOOK_DRAW(0x006B374C); break;
        case 0x4: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0x1a2, 0x1b7, 0x1b8); break;
        case 0x5: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0x1b8, 0x1b7, 0x1ce); break;
        case 0x6: SPELLBOOK_DRAW(0x006B379C); break;
        case 0x7: SPELLBOOK_DRAW(0x006B385C); break;
        case 0x8: SPELLBOOK_DRAW(0x006B378C); break;
        case 0x9: SPELLBOOK_DRAW(0x006B3768); break;
        case 0xa: SPELLBOOK_DRAW(0x006B3718); break;
        case 0xb: SPELLBOOK_DRAW(0x006B3774); break;
        case 0xc: SPELLBOOK_DRAW(0x006B3860); break;
        case 0xd: SPELLBOOK_DRAW(0x006B3858); break;
        case 0xe: SPELLBOOK_DRAW(0x006B3854); break;
        case 0xf: SPELLBOOK_DRAW(0x006B3744); break;
        case 0x10: SPELLBOOK_DRAW(0x006B3738); break;
        case 0x11: SPELLBOOK_DRAW(0x006B3850); break;
        case 0x12: SPELLBOOK_DRAW(0x006B384C); break;
        case 0x13: SPELLBOOK_DRAW(0x006B3724); break;
        case 0x14: SPELLBOOK_DRAW(0x006B3728); break;
        case 0x15:
            if (EQ_GLOBAL(char*, 0x006E4DA8) != 0 &&
                *(short*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x34) == 0x80 &&
                *(char*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x36) == 7)
                SPELLBOOK_DRAW(0x006B6194);
            else
                SPELLBOOK_DRAW(0x006B37AC);
            break;
        case 0x16: SPELLBOOK_DRAW(0x006B3730); break;
        case 0x17: SPELLBOOK_DRAW(0x006B37A8); break;
        case 0x18: SPELLBOOK_DRAW(0x006B37A4); break;
        case 0x19: SPELLBOOK_DRAW(0x006B3794); break;
        case 0x1a: SPELLBOOK_DRAW(0x006B376C); break;
        case 0x1b: SPELLBOOK_DRAW(0x006B3770); break;
        case 0x1c: SPELLBOOK_DRAW(0x006B37A0); break;
        case 0x1d: SPELLBOOK_DRAW(0x006B3788); break;
        case 0x1e: SPELLBOOK_DRAW(0x006B371C); break;
        case 0x1f: SPELLBOOK_DRAW(0x006B3764); break;
        case 0x20: SPELLBOOK_DRAW(0x006B377C); break;
        case 0x21: SPELLBOOK_DRAW(0x006B3740); break;
        case 0x22: SPELLBOOK_DRAW(0x006B3720); break;
        case 0x23: SPELLBOOK_DRAW(0x006B372C); break;
        case 0x24: SPELLBOOK_DRAW(0x006B3760); break;
        case 0x25: SPELLBOOK_DRAW(0x006B373C); break;
        case 0x26: SPELLBOOK_DRAW(0x006B3734); break;
        case 0x27: SPELLBOOK_DRAW(0x006B3778); break;
        case 0x28: SPELLBOOK_DRAW(0x006B3780); break;
        case 0x29: SPELLBOOK_DRAW(0x006B3754); break;
        case 0x2a: SPELLBOOK_DRAW(0x006B3790); break;
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f: SPELLBOOK_DRAW(0x006B3758); break;
        case 0x30: SPELLBOOK_DRAW(0x006B3784); break;
        case 0x31: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0x9a, 0x1b7, 0xb0); break;
        case 0x32: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0xb0, 0x1b7, 0xc6); break;
        case 0x33: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0xc6, 0x1b7, 0xdc); break;
        case 0x34: SPELLBOOK_PARTIAL(0x006E4E3C, 0x17a, 0xdc, 0x1b7, 0xf2); break;
        case 0x35: SPELLBOOK_DRAW(0x006B3798); break;
        case 0x36: SPELLBOOK_PARTIAL(0x006E4E3C, 0xf0, 0x1b7, 0x12d, 0x1cd); break;
        case 0x41: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x16, 0x279, 0x2c); break;
        case 0x40: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x2c, 0x279, 0x42); break;
        case 0x3f: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x42, 0x279, 0x58); break;
        case 0x3e: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x58, 0x279, 0x6e); break;
        case 0x45: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x6e, 0x279, 0x84); break;
        case 0x3d: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x84, 0x279, 0x9a); break;
        case 0x3c: SPELLBOOK_PARTIAL(0x006E4DE8, 0x23c, 0x9a, 0x279, 0xb0); break;
        case 0x44: SPELLBOOK_PARTIAL(0x006E4DE8, 0x204, 0x185, 0x241, 0x19b); break;
        case 0x46: SPELLBOOK_PARTIAL(0x006E4DE8, 0x204, 0x19b, 0x241, 0x1b1); break;
        case 0x48: SPELLBOOK_PARTIAL(0x006E4DE8, 0x204, 0x1b1, 0x241, 0x1c7); break;
        case 0x43: SPELLBOOK_PARTIAL(0x006E4DE8, 0x242, 0x185, 0x27f, 0x19b); break;
        case 0x42: SPELLBOOK_PARTIAL(0x006E4DE8, 0x242, 0x19b, 0x27f, 0x1b1); break;
        case 0x47: SPELLBOOK_PARTIAL(0x006E4DE8, 0x242, 0x1b1, 0x27f, 0x1c7); break;
        default: break;
        }
        cellIndex = (int)(short)row + self->screenMode_guess * 400;
        slot = *(int*)((char*)self + (cellIndex + 0x6e19) * 0x10);
        cellIndex = *(int*)((char*)self + cellIndex * 0x10 + 0x6e18c);
        pSetRect(&drawRect, cellIndex + 7, slot + 0x1f, cellIndex + 200, slot + 200);
        kind = *(short*)((char*)self + 0x6c65e + ((int)self->skillListTop_guess + (int)(short)row) * 2);
        if (kind > 99)
        {
            skillRank = *(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + kind + 0x1a);
            if (skillRank <= 10)
                SPELLBOOK_DRAW(0x006B36E8);
            else if (skillRank <= 20)
                SPELLBOOK_DRAW(0x006B36EC);
            else if (skillRank <= 30)
                SPELLBOOK_DRAW(0x006B36F0);
            else if (skillRank <= 40)
                SPELLBOOK_DRAW(0x006B36F4);
            else if (skillRank <= 50)
                SPELLBOOK_DRAW(0x006B36F8);
            else if (skillRank <= 60)
                SPELLBOOK_DRAW(0x006B36FC);
            else if (skillRank <= 70)
                SPELLBOOK_DRAW(0x006B3700);
            else if (skillRank <= 80)
                SPELLBOOK_DRAW(0x006B3704);
            else if (skillRank <= 90)
                SPELLBOOK_DRAW(0x006B3708);
            else if (skillRank <= 99)
                SPELLBOOK_DRAW(0x006B370C);
            else if (skillRank >= 100)
                SPELLBOOK_DRAW(0x006B3710);
        }
        else
        {
            skillRank = *(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + kind + 0x9c8);
            if (skillRank <= 10)
                SPELLBOOK_DRAW(0x006B36E8);
            else if (skillRank <= 20)
                SPELLBOOK_DRAW(0x006B36EC);
            else if (skillRank <= 30)
                SPELLBOOK_DRAW(0x006B36F0);
            else if (skillRank <= 40)
                SPELLBOOK_DRAW(0x006B36F4);
            else if (skillRank <= 50)
                SPELLBOOK_DRAW(0x006B36F8);
            else if (skillRank <= 60)
                SPELLBOOK_DRAW(0x006B36FC);
            else if (skillRank <= 70)
                SPELLBOOK_DRAW(0x006B3700);
            else if (skillRank <= 80)
                SPELLBOOK_DRAW(0x006B3704);
            else if (skillRank <= 90)
                SPELLBOOK_DRAW(0x006B3708);
            else if (skillRank <= 100)
                SPELLBOOK_DRAW(0x006B370C);
            else if (skillRank >= 101)
                SPELLBOOK_DRAW(0x006B3710);
            slot = (int)self->skillListTop_guess + (int)(short)row;
            if (*(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + *(short*)((char*)self + 0x6c65e + slot * 2) + 0x9c8) > 100 &&
                EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), EQ_GLOBAL(void*, 0x006E4DA8))(*(u8*)((char*)self + 0x6c65e + slot * 2)) != 0)
            {
                sprintf(
                    rankText, EQ_ARRAY(char, 0x0053E16C),
                    *(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + *(short*)((char*)self + 0x6c65e +
                        ((int)self->skillListTop_guess + (int)(short)row) * 2) + 0x9c8));
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(rankText, drawRect.left + 0x18, drawRect.top - 7, 10,
                    EQ_GLOBAL(int, 0x006EFDF8), 1);
            }
        }
nextSlot:
        row = row + 1;
        if (5 < (short)row)
            return;
    } while (true);
}
#undef SPELLBOOK_DRAW
#undef SPELLBOOK_PARTIAL

#define SKILLS_DRAW(image_va) \
    EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), \
        EQ_GLOBAL(void*, image_va))(&drawRect, 1)
#define SKILLS_PARTIAL(overlay_va, left, top, right, bottom) \
    pSetRect(&secondaryCoinRect, left, top, right, bottom); \
    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), \
        self)(*(void**)(EQ_GLOBAL(char*, overlay_va) + 4), &secondaryCoinRect, &drawRect, 1)
#define SKILLS_TEXT(text, x, y, color) \
    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), \
        EQ_GLOBAL(void*, 0x006E4E68))(text, x, y, color, EQ_GLOBAL(int, 0x006EFDFC), 1)
#define SKILLS_COIN_PARTIAL(top, bottom) \
    pSetRect(&drawRect, 0x144, top, 0x163, bottom); \
    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), \
        self)(*(void**)(EQ_GLOBAL(char*, 0x006E4E2C) + 4), &drawRect, &secondaryCoinRect, 1)

// FUNCTION: TRILOGY 0x004667F6
// Draws the trainer window: five skill/language rows, each with its icon and its training price in coins
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (self in ECX, no stack arguments), no return value. The skills page of the old UI (GM/trainer
// window): five slots from the scroll position (self+0x685cc, shadowed at self+0x685ce: a change sets the redraw
// flag at self+0xeb8), each inside the count at self+0x685c0. Same two passes as EQ_Main_drawSkills_guess
// (flag at self+0xc, marks at self+0x84018 / self+0x89dd8, the selected slot at self+0x1008 which also animates
// through the counter at self+0x100a); the kind is the entry at self+0x6c7ee. After the switch image for the kind
// each slot prints its cost line beside it: kind < 100 is a skill of g_pc ("Never Learn" at 0xff, "LEVEL: %d" when
// g_pc's level is below the class requirement, "No Charge" when it has no skill to train), kind >= 100 a language
// style entry costed with flag 1. A cost is split into platinum/gold/silver/copper digits: the first non-zero coin is
// drawn as its own image, then at most ONE of the following coins is drawn through drawPartial (bug kept).
void __fastcall EQ_Main_drawSkillsGm_guess(EQ_Main* self)
{
    char lineText[128];
    int platDigit;
    int goldDigit;
    int silverDigit;
    int temp;
    int price;
    int local_28;
    RECT secondaryCoinRect;
    RECT drawRect;
    u8 requiredLevel;
    short kind;
    int cellIndex;
    int tmp;
    int iFlag;
    float fCost;
    u8* kindPtr;
    tSetRect_guess_t pSetRect;

    if (self->trainSkillTopDrawn_guess != self->trainSkillTop_guess)
    {
        self->trainSkillTopDrawn_guess = self->trainSkillTop_guess;
        self->uiDirty_guess = 1;
    }
    tmp = 0;
    price = 5;
    do
    {
        if (((((self->redrawAll_guess != 0) ||
               (cellIndex = self->screenMode_guess * 400 + tmp,
                *(int*)((char*)self + 0x84018 + cellIndex * 4) != 0)) ||
              (*(int*)((char*)self + 0x89dd8 + cellIndex * 4) != 0)) ||
             ((int)self->trainSelectedRow_guess == self->trainSkillTop_guess + tmp)) &&
            (self->trainSkillTop_guess + tmp < self->trainSkillCount_guess))
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
                (char*)self + (self->screenMode_guess * 400 + tmp) * 0x10 + 0x6c93c, 1);
        }
        tmp = tmp + 1;
        price = price + -1;
    } while (price != 0);
    local_28 = 0;
    pSetRect = g_setrect_guess;
    do
    {
        if ((((self->redrawAll_guess == 0) &&
              (tmp = self->screenMode_guess * 400 + (int)(short)local_28,
               *(int*)((char*)self + 0x84018 + tmp * 4) == 0)) &&
             (*(int*)((char*)self + 0x89dd8 + tmp * 4) == 0)) &&
            ((int)self->trainSelectedRow_guess !=
             (int)self->trainSkillTop_guess + (int)(short)local_28))
            goto nextSlot;
        tmp = (int)(short)local_28 + (int)self->trainSkillTop_guess;
        if (self->trainSkillCount_guess <= tmp)
            goto nextSlot;
        if (self->trainSelectedRow_guess == tmp)
        {
            if (self->trainBlinkTick_guess < 0xb)
            {
                EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38B8))(
                    (char*)self + (self->screenMode_guess * 400 + (int)(short)local_28) * 0x10 + 0x6c93c, 1);
            }
            else if (self->trainBlinkTick_guess < 0x15)
            {
                EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38BC))(
                    (char*)self + (self->screenMode_guess * 400 + (int)(short)local_28) * 0x10 + 0x6c93c, 1);
            }
            self->trainBlinkTick_guess = self->trainBlinkTick_guess + 1;
            if (0x14 < self->trainBlinkTick_guess)
            {
                self->trainBlinkTick_guess = 0;
            }
        }
        else
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
                (char*)self + (self->screenMode_guess * 400 + (int)(short)local_28) * 0x10 + 0x6c93c, 1);
        }
        *(int*)((char*)self + 0x89dd8 + (self->screenMode_guess * 400 + (int)(short)local_28) * 4) = 0;
        if (*(short*)((char*)self + 0x6c7ee + ((int)self->trainSkillTop_guess + (int)(short)local_28) * 2) == -1)
            goto nextSlot;
        tmp = (int)(short)local_28 + self->screenMode_guess * 400;
        pSetRect(&drawRect, *(int*)((char*)self + tmp * 0x10 + 0x6c93c) + 7,
                 *(int*)((char*)self + (tmp + 0x6c94) * 0x10) + 6, 0x280, 0x1e0);
        switch (*(short*)((char*)self + 0x6c7ee + ((int)self->trainSkillTop_guess + (int)(short)local_28) * 2))
        {
        case 0x64: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0x88, 0x1f7, 0x9e); break;
        case 0x65: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0x88, 0x235, 0x9e); break;
        case 0x66: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0x9e, 0x1f7, 0xb4); break;
        case 0x67: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0x9e, 0x235, 0xb4); break;
        case 0x68: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0xb4, 0x1f7, 0xca); break;
        case 0x69: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0xb4, 0x235, 0xca); break;
        case 0x6a: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0xca, 0x1f7, 0xe0); break;
        case 0x6b: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0xca, 0x235, 0xe0); break;
        case 0x6c: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0xe0, 0x1f7, 0xf6); break;
        case 0x6d: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0xe0, 0x235, 0xf6); break;
        case 0x6e: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0x122, 0x273, 0x138); break;
        case 0x6f: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0x122, 0x1f7, 0x138); break;
        case 0x70: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0x10c, 0x273, 0x122); break;
        case 0x71: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0xf6, 0x1f7, 0x10c); break;
        case 0x72: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0xf6, 0x236, 0x10c); break;
        case 0x73: SKILLS_PARTIAL(0x006E4E1C, 0x1ba, 0x10c, 0x1f7, 0x122); break;
        case 0x74: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0x10c, 0x235, 0x122); break;
        case 0x75: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0x88, 0x273, 0x9e); break;
        case 0x76: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0x9e, 0x273, 0xb4); break;
        case 0x77: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0xb4, 0x273, 0xca); break;
        case 0x78: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0xca, 0x273, 0xe0); break;
        case 0x79: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0xe0, 0x273, 0xf6); break;
        case 0x7a: SKILLS_PARTIAL(0x006E4E1C, 0x236, 0xf6, 0x273, 0x10c); break;
        case 0x7b: SKILLS_PARTIAL(0x006E4E1C, 0x1f8, 0x122, 0x235, 0x138); break;
        case 0x3a: SKILLS_PARTIAL(0x006E4DE8, 0x218, 0xdc, 0x255, 0xf2); break;
        case 0x39: SKILLS_PARTIAL(0x006E4DE8, 0x1d9, 0xdc, 0x216, 0xf2); break;
        case 0x38: SKILLS_PARTIAL(0x006E4DE8, 0x218, 0xc6, 0x255, 0xdc); break;
        case 0x3b: SKILLS_PARTIAL(0x006E4DE8, 0x1d9, 0xc6, 0x216, 0xdc); break;
        case 0x49: SKILLS_PARTIAL(0x006E4DE8, 0x204, 0x1c7, 0x241, 0x1dd); break;
        case 0x37: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0xb0, 0x279, 0xc6); break;
        case 0x0: SKILLS_DRAW(0x006B3750); break;
        case 0x1: SKILLS_DRAW(0x006B3748); break;
        case 0x2: SKILLS_DRAW(0x006B375C); break;
        case 0x3: SKILLS_DRAW(0x006B374C); break;
        case 0x4: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0x1a2, 0x1b7, 0x1b8); break;
        case 0x5: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0x1b8, 0x1b7, 0x1ce); break;
        case 0x6: SKILLS_DRAW(0x006B379C); break;
        case 0x7: SKILLS_DRAW(0x006B385C); break;
        case 0x8: SKILLS_DRAW(0x006B378C); break;
        case 0x9: SKILLS_DRAW(0x006B3768); break;
        case 0xa: SKILLS_DRAW(0x006B3718); break;
        case 0xb: SKILLS_DRAW(0x006B3774); break;
        case 0xc: SKILLS_DRAW(0x006B3860); break;
        case 0xd: SKILLS_DRAW(0x006B3858); break;
        case 0xe: SKILLS_DRAW(0x006B3854); break;
        case 0xf: SKILLS_DRAW(0x006B3744); break;
        case 0x10: SKILLS_DRAW(0x006B3738); break;
        case 0x11: SKILLS_DRAW(0x006B3850); break;
        case 0x12: SKILLS_DRAW(0x006B384C); break;
        case 0x13: SKILLS_DRAW(0x006B3724); break;
        case 0x14: SKILLS_DRAW(0x006B3728); break;
        case 0x15:
            if (EQ_GLOBAL(char*, 0x006E4DA8) != 0 &&
                *(short*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x34) == 0x80 &&
                *(char*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x36) == 7)
                SKILLS_DRAW(0x006B6194);
            else
                SKILLS_DRAW(0x006B37AC);
            break;
        case 0x16: SKILLS_DRAW(0x006B3730); break;
        case 0x17: SKILLS_DRAW(0x006B37A8); break;
        case 0x18: SKILLS_DRAW(0x006B37A4); break;
        case 0x19: SKILLS_DRAW(0x006B3794); break;
        case 0x1a: SKILLS_DRAW(0x006B376C); break;
        case 0x1b: SKILLS_DRAW(0x006B3770); break;
        case 0x1c: SKILLS_DRAW(0x006B37A0); break;
        case 0x1d: SKILLS_DRAW(0x006B3788); break;
        case 0x1e: SKILLS_DRAW(0x006B371C); break;
        case 0x1f: SKILLS_DRAW(0x006B3764); break;
        case 0x20: SKILLS_DRAW(0x006B377C); break;
        case 0x21: SKILLS_DRAW(0x006B3740); break;
        case 0x22: SKILLS_DRAW(0x006B3720); break;
        case 0x23: SKILLS_DRAW(0x006B372C); break;
        case 0x24: SKILLS_DRAW(0x006B3760); break;
        case 0x25: SKILLS_DRAW(0x006B373C); break;
        case 0x26: SKILLS_DRAW(0x006B3734); break;
        case 0x27: SKILLS_DRAW(0x006B3778); break;
        case 0x28: SKILLS_DRAW(0x006B3780); break;
        case 0x29: SKILLS_DRAW(0x006B3754); break;
        case 0x2a: SKILLS_DRAW(0x006B3790); break;
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f: SKILLS_DRAW(0x006B3758); break;
        case 0x30: SKILLS_DRAW(0x006B3784); break;
        case 0x31: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0x9a, 0x1b7, 0xb0); break;
        case 0x32: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0xb0, 0x1b7, 0xc6); break;
        case 0x33: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0xc6, 0x1b7, 0xdc); break;
        case 0x34: SKILLS_PARTIAL(0x006E4E3C, 0x17a, 0xdc, 0x1b7, 0xf2); break;
        case 0x35: SKILLS_DRAW(0x006B3798); break;
        case 0x36: SKILLS_PARTIAL(0x006E4E3C, 0xf0, 0x1b7, 0x12d, 0x1cd); break;
        case 0x41: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x16, 0x279, 0x2c); break;
        case 0x40: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x2c, 0x279, 0x42); break;
        case 0x3f: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x42, 0x279, 0x58); break;
        case 0x3e: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x58, 0x279, 0x6e); break;
        case 0x45: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x6e, 0x279, 0x84); break;
        case 0x3d: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x84, 0x279, 0x9a); break;
        case 0x3c: SKILLS_PARTIAL(0x006E4DE8, 0x23c, 0x9a, 0x279, 0xb0); break;
        case 0x44: SKILLS_PARTIAL(0x006E4DE8, 0x204, 0x185, 0x241, 0x19b); break;
        case 0x46: SKILLS_PARTIAL(0x006E4DE8, 0x204, 0x19b, 0x241, 0x1b1); break;
        case 0x48: SKILLS_PARTIAL(0x006E4DE8, 0x204, 0x1b1, 0x241, 0x1c7); break;
        case 0x43: SKILLS_PARTIAL(0x006E4DE8, 0x242, 0x185, 0x27f, 0x19b); break;
        case 0x42: SKILLS_PARTIAL(0x006E4DE8, 0x242, 0x19b, 0x27f, 0x1b1); break;
        case 0x47: SKILLS_PARTIAL(0x006E4DE8, 0x242, 0x1b1, 0x27f, 0x1c7); break;
        default: break;
        }
        tmp = (int)(short)local_28 + self->screenMode_guess * 400;
        pSetRect(&drawRect, *(int*)((char*)self + tmp * 0x10 + 0x6c93c) + 7,
                 *(int*)((char*)self + (tmp + 0x6c94) * 0x10) + 0x1e, 0x280, 0x1e0);
        tmp = (int)(short)local_28 + self->screenMode_guess * 400;
        pSetRect(&secondaryCoinRect, *(int*)((char*)self + tmp * 0x10 + 0x6c93c) + 0x25,
                 *(int*)((char*)self + (tmp + 0x6c94) * 0x10) + 0x1e, 0x280, 0x1e0);
        tmp = (int)self->trainSkillTop_guess + (int)(short)local_28;
        kindPtr = (u8*)((char*)self + 0x6c7ee + tmp * 2);
        kind = *(short*)((char*)self + 0x6c7ee + tmp * 2);
        if (kind < 100)
        {
            if (*(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + kind + 0x9c8) == 0xff)
            {
                SKILLS_TEXT(EQ_ARRAY(char, 0x0055B6D8), drawRect.left + 1, drawRect.top + 5, 7);
                goto nextSlot;
            }
            requiredLevel = *(u8*)(EQ_ARRAY(int, 0x006E33A4)[kind] + 0x28 + g_pc_guess->class_);
            if (requiredLevel > g_pc_guess->level)
            {
                sprintf(lineText, EQ_ARRAY(char, 0x00558B44), requiredLevel);
                SKILLS_TEXT(lineText, drawRect.left + 5, drawRect.top + 5, 7);
                goto nextSlot;
            }
            if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), EQ_GLOBAL(void*, 0x006E4DA8))(*kindPtr) == 0)
                goto noCharge;
            fCost = self->merchantRate_guess;
            tmp = (int)*(short*)((char*)self + 0x6c7ee +
                                   ((int)self->trainSkillTop_guess + (int)(short)local_28) * 2);
            iFlag = 0;
        }
        else
        {
            fCost = self->merchantRate_guess;
            tmp = kind + -100;
            iFlag = 1;
        }
        price = EQ_CDECL(int, 0x004A09A4, (void*, int, float, int))(
            EQ_GLOBAL(void*, 0x006E4DA8), iFlag, fCost, tmp);
        if (price < 1)
        {
noCharge:
            SKILLS_TEXT(EQ_ARRAY(char, 0x0055B6CC), drawRect.left + 5, drawRect.top + 5, 7);
            goto nextSlot;
        }
        platDigit = price / 1000;
        price = price % 1000;
        goldDigit = price / 100;
        price = price % 100;
        silverDigit = price / 10;
        temp = price % 10;
        if (platDigit != 0)
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38C0))(&drawRect, 1);
            sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), platDigit);
            SKILLS_TEXT(lineText, drawRect.left + 10, drawRect.top + 5, 0);
            goto nextGold;
        }
        if (goldDigit != 0)
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38C4))(&drawRect, 1);
            sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), goldDigit);
            SKILLS_TEXT(lineText, drawRect.left + 10, drawRect.top + 5, 0);
            goto nextSilver;
        }
        if (silverDigit != 0)
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38C8))(&drawRect, 1);
            sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), silverDigit);
            SKILLS_TEXT(lineText, drawRect.left + 10, drawRect.top + 5, 0);
            goto nextCopper;
        }
        if (temp == 0)
            goto nextSlot;
        EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B38CC))(&drawRect, 1);
        sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), temp);
        SKILLS_TEXT(lineText, drawRect.left + 10, drawRect.top + 5, 0);
        temp = 0;
nextGold:
        if (goldDigit == 0)
            goto nextSilver;
        SKILLS_COIN_PARTIAL(0x19e, 0x1b4);
        sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), goldDigit);
        SKILLS_TEXT(lineText, secondaryCoinRect.left + 10, secondaryCoinRect.top + 5, 0);
        goto nextSlot;
nextSilver:
        if (silverDigit == 0)
            goto nextCopper;
        SKILLS_COIN_PARTIAL(0x1b4, 0x1ca);
        sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), silverDigit);
        SKILLS_TEXT(lineText, secondaryCoinRect.left + 10, secondaryCoinRect.top + 5, 0);
        goto nextSlot;
nextCopper:
        if (temp == 0)
            goto nextSlot;
        SKILLS_COIN_PARTIAL(0x1ca, 0x1e0);
        sprintf(lineText, EQ_ARRAY(char, 0x0053E16C), temp);
        SKILLS_TEXT(lineText, secondaryCoinRect.left + 10, secondaryCoinRect.top + 5, 0);
nextSlot:
        local_28 = local_28 + 1;
        if (4 < (short)local_28)
            return;
    } while (true);
}
#undef SKILLS_DRAW
#undef SKILLS_PARTIAL
#undef SKILLS_TEXT
#undef SKILLS_COIN_PARTIAL

// FUNCTION: TRILOGY 0x0046776C
// Draws a skill list page: five scrolled rows, each a skill icon plus a rank image for its value in steps of 10
// @early-stop: literal-1 and dummy-EDX call setup, plus the shared partial-call branch, still differ.
// __fastcall (self in ECX, no stack arguments), no return value. Five slots from the scroll position
// (self+0x685d0), each inside the count at self+0x685c4. Pass 1 draws a slot when the flag at self+0xc is set or
// one of its marks (self+0x84044, self+0x89e04) is set. Pass 2 skips a slot with no flag and no marks, draws its
// element, clears the mark at self+0x89e04, and ends the slot at kind -1 (kind = the entry at self+0x6c726).
// Otherwise it draws the switch image for the kind (a global per case, a rect for drawPartial, none for the
// default), then the image chosen by the skill byte (kind < 100) or name byte (kind >= 100) of g_pc.
#pragma optimize("y", off)
/* fake match: retain the original EBP frame under /O1's frame-pointer omission. */
void __fastcall EQ_Main_DrawSkillList_guess(EQ_Main* self)
{
    u8 skillValue;
    short entryId;
    int entryIndex;
    int tmp;
    tSetRect_guess_t pSetRect;
    RECT local_28;
    RECT local_18;
    int row;

    if (self->skillWindowTopDrawn_guess != self->skillWindowTop_guess)
    {
        self->skillWindowTopDrawn_guess = self->skillWindowTop_guess;
        self->uiDirty_guess = 1;
    }
    tmp = 0;
    row = 5;
    do
    {
        if ((((self->redrawAll_guess != 0) ||
              (entryIndex = self->screenMode_guess * 400 + tmp,
               *(int*)((char*)self + 0x84044 + entryIndex * 4) != 0)) ||
             (*(int*)((char*)self + 0x89e04 + entryIndex * 4) != 0)) &&
            (self->skillWindowTop_guess + tmp < self->skillWindowCount_guess))
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
                (char*)self + (self->screenMode_guess * 400 + tmp) * 0x10 + 0x6c9ec, 1);
        }
        tmp = tmp + 1;
        row = row + -1;
    } while (row != 0);
    row = 0;
    pSetRect = g_setrect_guess;
    do
    {
        if ((((self->redrawAll_guess != 0) ||
              (tmp = self->screenMode_guess * 400 + (int)(short)row,
               *(int*)((char*)self + 0x84044 + tmp * 4) != 0)) ||
             (*(int*)((char*)self + 0x89e04 + tmp * 4) != 0)) &&
            ((int)self->skillWindowTop_guess + (int)(short)row <
             self->skillWindowCount_guess))
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36D4))(
                (char*)self + (self->screenMode_guess * 400 + (int)(short)row) * 0x10 + 0x6c9ec, 1);
            *(int*)((char*)self + 0x89e04 + (self->screenMode_guess * 400 + (int)(short)row) * 4) = 0;
            if (*(short*)((char*)self + 0x6c726 +
                          ((int)self->skillWindowTop_guess + (int)(short)row) * 2) != -1)
            {
                tmp = (int)(short)row + self->screenMode_guess * 400;
                pSetRect(&local_18, *(int*)((char*)self + tmp * 0x10 + 0x6c9ec) + 7,
                         *(int*)((char*)self + (tmp + 0x6c9f) * 0x10) + 6, 0x280, 0x1e0);
                tmp = 0;
                switch (*(short*)((char*)self + 0x6c726 +
                                  ((int)self->skillWindowTop_guess + (int)(short)row) * 2))
                {
                case 0x0: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3750))(&local_18, 1); break;
                case 0x1: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3748))(&local_18, 1); break;
                case 0x2: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B375C))(&local_18, 1); break;
                case 0x3: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B374C))(&local_18, 1); break;
                case 0x4: pSetRect(&local_28, 0x17a, 0x1a2, 0x1b7, 0x1b8); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x5: pSetRect(&local_28, 0x17a, 0x1b8, 0x1b7, 0x1ce); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x6: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B379C))(&local_18, 1); break;
                case 0x7: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B385C))(&local_18, 1); break;
                case 0x8: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B378C))(&local_18, 1); break;
                case 0x9: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3768))(&local_18, 1); break;
                case 0xa: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3718))(&local_18, 1); break;
                case 0xb: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3774))(&local_18, 1); break;
                case 0xc: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3860))(&local_18, 1); break;
                case 0xd: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3858))(&local_18, 1); break;
                case 0xe: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3854))(&local_18, 1); break;
                case 0xf: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3744))(&local_18, 1); break;
                case 0x10: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3738))(&local_18, 1); break;
                case 0x11: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3850))(&local_18, 1); break;
                case 0x12: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B384C))(&local_18, 1); break;
                case 0x13: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3724))(&local_18, 1); break;
                case 0x14: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3728))(&local_18, 1); break;
                case 0x15:
                    if (EQ_GLOBAL(char*, 0x006E4DA8) != 0 &&
                        *(short*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x34) == 0x80 &&
                        *(char*)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x36) == 7)
                        EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B6194))(&local_18, 1);
                    else
                        EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37AC))(&local_18, 1);
                    break;
                case 0x16: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3730))(&local_18, 1); break;
                case 0x17: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37A8))(&local_18, 1); break;
                case 0x18: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37A4))(&local_18, 1); break;
                case 0x19: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3794))(&local_18, 1); break;
                case 0x1a: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B376C))(&local_18, 1); break;
                case 0x1b: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3770))(&local_18, 1); break;
                case 0x1c: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37A0))(&local_18, 1); break;
                case 0x1d: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3788))(&local_18, 1); break;
                case 0x1e: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B371C))(&local_18, 1); break;
                case 0x1f: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3764))(&local_18, 1); break;
                case 0x20: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B377C))(&local_18, 1); break;
                case 0x21: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3740))(&local_18, 1); break;
                case 0x22: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3720))(&local_18, 1); break;
                case 0x23: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B372C))(&local_18, 1); break;
                case 0x24: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3760))(&local_18, 1); break;
                case 0x25: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B373C))(&local_18, 1); break;
                case 0x26: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3734))(&local_18, 1); break;
                case 0x27: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3778))(&local_18, 1); break;
                case 0x28: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3780))(&local_18, 1); break;
                case 0x29: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3754))(&local_18, 1); break;
                case 0x2a: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3790))(&local_18, 1); break;
                case 0x2b:
                case 0x2c:
                case 0x2d:
                case 0x2e:
                case 0x2f: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3758))(&local_18, 1); break;
                case 0x30: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3784))(&local_18, 1); break;
                case 0x31: pSetRect(&local_28, 0x17a, 0x9a, 0x1b7, 0xb0); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x32: pSetRect(&local_28, 0x17a, 0xb0, 0x1b7, 0xc6); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x33: pSetRect(&local_28, 0x17a, 0xc6, 0x1b7, 0xdc); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x34: pSetRect(&local_28, 0x17a, 0xdc, 0x1b7, 0xf2); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x35: EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3798))(&local_18, 1); break;
                case 0x36: pSetRect(&local_28, 0xf0, 0x1b7, 0x12d, 0x1cd); tmp = EQ_GLOBAL(int, 0x006E4E3C); break;
                case 0x37: pSetRect(&local_28, 0x23c, 0xb0, 0x279, 0xc6); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x38: pSetRect(&local_28, 0x218, 0xc6, 0x255, 0xdc); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x39: pSetRect(&local_28, 0x1d9, 0xdc, 0x216, 0xf2); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3a: pSetRect(&local_28, 0x218, 0xdc, 0x255, 0xf2); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3b: pSetRect(&local_28, 0x1d9, 0xc6, 0x216, 0xdc); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3c: pSetRect(&local_28, 0x23c, 0x9a, 0x279, 0xb0); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3d: pSetRect(&local_28, 0x23c, 0x84, 0x279, 0x9a); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3e: pSetRect(&local_28, 0x23c, 0x58, 0x279, 0x6e); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x3f: pSetRect(&local_28, 0x23c, 0x42, 0x279, 0x58); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x40: pSetRect(&local_28, 0x23c, 0x2c, 0x279, 0x42); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x41: pSetRect(&local_28, 0x23c, 0x16, 0x279, 0x2c); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x42: pSetRect(&local_28, 0x242, 0x19b, 0x27f, 0x1b1); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x43: pSetRect(&local_28, 0x242, 0x185, 0x27f, 0x19b); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x44: pSetRect(&local_28, 0x204, 0x185, 0x241, 0x19b); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x45: pSetRect(&local_28, 0x23c, 0x6e, 0x279, 0x84); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x46: pSetRect(&local_28, 0x204, 0x19b, 0x241, 0x1b1); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x47: pSetRect(&local_28, 0x242, 0x1b1, 0x27f, 0x1c7); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x48: pSetRect(&local_28, 0x204, 0x1b1, 0x241, 0x1c7); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                case 0x49: pSetRect(&local_28, 0x204, 0x1c7, 0x241, 0x1dd); tmp = EQ_GLOBAL(int, 0x006E4DE8); break;
                default: break;
                case 0x64: pSetRect(&local_28, 0x1ba, 0x88, 0x1f7, 0x9e); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x65: pSetRect(&local_28, 0x1f8, 0x88, 0x235, 0x9e); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x66: pSetRect(&local_28, 0x1ba, 0x9e, 0x1f7, 0xb4); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x67: pSetRect(&local_28, 0x1f8, 0x9e, 0x235, 0xb4); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x68: pSetRect(&local_28, 0x1ba, 0xb4, 0x1f7, 0xca); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x69: pSetRect(&local_28, 0x1f8, 0xb4, 0x235, 0xca); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6a: pSetRect(&local_28, 0x1ba, 0xca, 0x1f7, 0xe0); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6b: pSetRect(&local_28, 0x1f8, 0xca, 0x235, 0xe0); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6c: pSetRect(&local_28, 0x1ba, 0xe0, 0x1f7, 0xf6); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6d: pSetRect(&local_28, 0x1f8, 0xe0, 0x235, 0xf6); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6e: pSetRect(&local_28, 0x236, 0x122, 0x273, 0x138); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x6f: pSetRect(&local_28, 0x1ba, 0x122, 0x1f7, 0x138); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x70: pSetRect(&local_28, 0x236, 0x10c, 0x273, 0x122); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x71: pSetRect(&local_28, 0x1ba, 0xf6, 0x1f7, 0x10c); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x72: pSetRect(&local_28, 0x1f8, 0xf6, 0x235, 0x10c); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x73: pSetRect(&local_28, 0x1ba, 0x10c, 0x1f7, 0x122); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x74: pSetRect(&local_28, 0x1f8, 0x10c, 0x235, 0x122); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x75: pSetRect(&local_28, 0x236, 0x88, 0x273, 0x9e); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x76: pSetRect(&local_28, 0x236, 0x9e, 0x273, 0xb4); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x77: pSetRect(&local_28, 0x236, 0xb4, 0x273, 0xca); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x78: pSetRect(&local_28, 0x236, 0xca, 0x273, 0xe0); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x79: pSetRect(&local_28, 0x236, 0xe0, 0x273, 0xf6); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x7a: pSetRect(&local_28, 0x236, 0xf6, 0x273, 0x10c); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                case 0x7b: pSetRect(&local_28, 0x1f8, 0x122, 0x235, 0x138); tmp = EQ_GLOBAL(int, 0x006E4E1C); break;
                }
                if (tmp != 0)
                    EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(tmp + 4), &local_28, &local_18, 1);
                tmp = (int)(short)row + self->screenMode_guess * 400;
                pSetRect(&local_18, *(int*)((char*)self + tmp * 0x10 + 0x6c9ec) + 7,
                         *(int*)((char*)self + (tmp + 0x6c9f) * 0x10) + 0x1f, 0x280, 0x1e0);
                entryId = *(short*)((char*)self + 0x6c726 +
                                  ((int)self->skillWindowTop_guess + (int)(short)row) * 2);
                if (entryId > 99)
                {
                    skillValue = *(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + entryId + 0x1a);
                    if (skillValue <= 10) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36E8))(&local_18, 1);
                    else if (skillValue <= 20) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36EC))(&local_18, 1);
                    else if (skillValue <= 30) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F0))(&local_18, 1);
                    else if (skillValue <= 40) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F4))(&local_18, 1);
                    else if (skillValue <= 50) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F8))(&local_18, 1);
                    else if (skillValue <= 60) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36FC))(&local_18, 1);
                    else if (skillValue <= 70) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3700))(&local_18, 1);
                    else if (skillValue <= 80) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3704))(&local_18, 1);
                    else if (skillValue <= 90) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3708))(&local_18, 1);
                    else if (skillValue <= 99) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B370C))(&local_18, 1);
                    else if (skillValue >= 100) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3710))(&local_18, 1);
                }
                else
                {
                    skillValue = *(u8*)(EQ_GLOBAL(char*, 0x006E4DA8) + entryId + 0x9c8);
                    if (skillValue <= 10) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36E8))(&local_18, 1);
                    else if (skillValue <= 20) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36EC))(&local_18, 1);
                    else if (skillValue <= 30) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F0))(&local_18, 1);
                    else if (skillValue <= 40) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F4))(&local_18, 1);
                    else if (skillValue <= 50) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36F8))(&local_18, 1);
                    else if (skillValue <= 60) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B36FC))(&local_18, 1);
                    else if (skillValue <= 70) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3700))(&local_18, 1);
                    else if (skillValue <= 80) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3704))(&local_18, 1);
                    else if (skillValue <= 90) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3708))(&local_18, 1);
                    else if (skillValue <= 100) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B370C))(&local_18, 1);
                    else if (skillValue >= 101) EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B3710))(&local_18, 1);
                }
            }
        }
        row = row + 1;
    } while ((short)row < 5);
}
#pragma optimize("y", on)

/* drawPolys_guess helpers: the colour word at self+off split into the four DoPoly bytes (alpha masked to 3 bits;
   only the low byte of each dword slot is written, as in the original), the fill and the outline calls. */
#define DRAWPOLYS_COLOR(off) \
    *(u8*)&local_18 = (u8)((*(u32*)((char*)self + (off)) >> 0x18) & 7); \
    *(u8*)&tempWord2 = (u8)(*(u32*)((char*)self + (off)) >> 0x10); \
    *(u8*)&tempWord1 = (u8)(*(u32*)((char*)self + (off)) >> 8); \
    *(u8*)&local_14 = *(u8*)((char*)self + (off));
#define DRAWPOLYS_FILL(rect) \
    if (EQ_GLOBAL(int, 0x006B65A0) == 0) \
        EQ_METHOD(6, void, 0x004081F2, (void*, u32, u32, u32, u32, int, void*), EQ_GLOBAL(void*, 0x006E4DE0))(tempWord2, tempWord1, local_14, local_18, 0, &rect);
#define DRAWPOLYS_OUTLINE(rect) \
    EQ_METHOD(1, void, 0x0046AF87, (void*, void*), self)(&rect);
#define DRAWPOLYS_PANEL(off) \
    EQ_METHOD(1, void, 0x0046AF87, (void*, void*), self)(\
        (char*)self + *(char*)((char*)self + 0xfe0) * 0x1900 + (off));

// FUNCTION: TRILOGY 0x0046835A
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (self in ECX, no stack arguments), no return value. Eleven overlay panels, each a rect at
// self+X (left, top, width, height) with a colour word at self+0x91xxx: clears the panel's flag word (+0x..d4
// style), and when its enable word is set fills the rect through DoPoly (unless DAT_006b65a0 is set) and outlines
// it plus the panel's rect(s) at self+0xfe0*0x1900+off. The class-gated panel (+0x900a4) is skipped for class
// 1/7/9 of g_pc. The panel at +0x90064 also draws four rows and four horizontal t3dDeferLine strokes.
void EQ_Main::drawPolys_guess(void)
{
    EQ_Main* self = this;
    u8 charClass;
    tSetRect_guess_t pSetRect;
    int tmp;
    u32 tempWord1;
    u32 tempWord2;
    u32 local_14;
    u32 local_18;
    int scratch;
    LONG* ptr;
    float vertices[6];
    RECT rect90044;
    RECT rect90244;
    RECT rect900c4;
    RECT rect90184;
    RECT rect90084;
    RECT rect90264;
    RECT rect90224;
    RECT rect90064;
    RECT rect900a4;
    RECT rect90164;
    RECT rect900e4;
    RECT rows[4];

    pSetRect = g_setrect_guess;
    *(int*)((char*)this + 0x900d4) = 0;
    if (*(int*)((char*)this + 0x900d8) != 0)
    {
        *(int*)((char*)this + 0x900d4) = 0;
        tmp = *(int*)((char*)this + 0x900c8);
        *(int*)((char*)this + 0x900d4) = 0;
        *(int*)((char*)this + 0x900d4) = 0;
        *(int*)((char*)this + 0x900d4) = 0;
        DRAWPOLYS_COLOR(0x91014)
        pSetRect(&rect900c4, *(int*)((char*)this + 0x900c4), tmp,
                 *(int*)((char*)this + 0x900cc) + *(int*)((char*)this + 0x900c4),
                 *(int*)((char*)this + 0x900d0) + tmp);
        DRAWPOLYS_FILL(rect900c4)
        DRAWPOLYS_OUTLINE(rect900c4)
        DRAWPOLYS_PANEL(0x6dfcc)
    }
    charClass = g_pc_guess->class_;
    if (charClass != 1 && charClass != 7 && charClass != 9 &&
        (*(int*)((char*)this + 0x900b4) = 0, *(int*)((char*)this + 0x900b8) != 0))
    {
        *(int*)((char*)this + 0x900b4) = 0;
        tmp = *(int*)((char*)this + 0x900a8);
        *(int*)((char*)this + 0x900b4) = 0;
        *(int*)((char*)this + 0x900b4) = 0;
        *(int*)((char*)this + 0x900b4) = 0;
        DRAWPOLYS_COLOR(0x91010)
        pSetRect(&rect900a4, *(int*)((char*)this + 0x900a4), tmp,
                 *(int*)((char*)this + 0x900ac) + *(int*)((char*)this + 0x900a4),
                 *(int*)((char*)this + 0x900b0) + tmp);
        DRAWPOLYS_FILL(rect900a4)
        DRAWPOLYS_OUTLINE(rect900a4)
        DRAWPOLYS_PANEL(0x6dfbc)
    }
    *(int*)((char*)this + 0x90054) = 0;
    if (*(int*)((char*)this + 0x90058) != 0)
    {
        *(int*)((char*)this + 0x90054) = 0;
        tmp = *(int*)((char*)this + 0x90048);
        *(int*)((char*)this + 0x90054) = 0;
        *(int*)((char*)this + 0x90054) = 0;
        *(int*)((char*)this + 0x90054) = 0;
        DRAWPOLYS_COLOR(0x91004)
        pSetRect(&rect90044, *(int*)((char*)this + 0x90044), tmp,
                 *(int*)((char*)this + 0x9004c) + *(int*)((char*)this + 0x90044),
                 *(int*)((char*)this + 0x90050) + tmp);
        DRAWPOLYS_FILL(rect90044)
        DRAWPOLYS_OUTLINE(rect90044)
        DRAWPOLYS_PANEL(0x6df8c)
    }
    *(int*)((char*)this + 0x90074) = 0;
    if (*(int*)((char*)this + 0x90078) != 0)
    {
        scratch = *(int*)((char*)this + 0x90064);
        *(int*)((char*)this + 0x90074) = 0;
        tmp = *(int*)((char*)this + 0x90068);
        *(int*)((char*)this + 0x90074) = 0;
        *(int*)((char*)this + 0x90074) = 0;
        *(int*)((char*)this + 0x90074) = 0;
        ptr = (LONG*)(*(int*)((char*)this + 0x9006c) + scratch);
        DRAWPOLYS_COLOR(0x91008)
        pSetRect(&rect90064, scratch, tmp, (int)ptr, *(int*)((char*)this + 0x90070) + tmp);
        DRAWPOLYS_FILL(rect90064)
        pSetRect(&rows[0], scratch, tmp + 0x16, (int)ptr, 0x84);
        pSetRect(&rows[1], scratch, tmp + 0x2c, (int)ptr, 0x99);
        pSetRect(&rows[2], scratch, tmp + 0x42, (int)ptr, 0xaf);
        pSetRect(&rows[3], scratch, tmp + 0x58, (int)ptr, 0xc5);
        DRAWPOLYS_OUTLINE(rect90064)
        DRAWPOLYS_PANEL(0x6df9c)
        *((u8*)&scratch + 3) = 0xff;
        *((u8*)&scratch + 2) = 0;
        *((u8*)&scratch + 1) = 0;
        *(u8*)&scratch = 0;
        ptr = &rows[0].top;
        tmp = 4;
        do
        {
            vertices[0] = (float)ptr[-1];
            vertices[4] = (float)*ptr;
            vertices[1] = vertices[4];
            vertices[3] = (float)ptr[1];
            EQ_GLOBAL(t3dDeferLine_guess_t, 0x006E51B8)(vertices, (u32)scratch);
            ptr = ptr + 4;
            tmp = tmp + -1;
        } while (tmp != 0);
    }
    *(int*)((char*)this + 0x90094) = 0;
    if (*(int*)((char*)this + 0x90098) != 0)
    {
        *(int*)((char*)this + 0x90094) = 0;
        tmp = *(int*)((char*)this + 0x90088);
        *(int*)((char*)this + 0x90094) = 0;
        *(int*)((char*)this + 0x90094) = 0;
        *(int*)((char*)this + 0x90094) = 0;
        DRAWPOLYS_COLOR(0x9100c)
        pSetRect(&rect90084, *(int*)((char*)this + 0x90084), tmp,
                 *(int*)((char*)this + 0x9008c) + *(int*)((char*)this + 0x90084),
                 *(int*)((char*)this + 0x90090) + tmp);
        DRAWPOLYS_FILL(rect90084)
        DRAWPOLYS_OUTLINE(rect90084)
        DRAWPOLYS_PANEL(0x6dfac)
    }
    *(int*)((char*)this + 0x90174) = 0;
    if (*(int*)((char*)this + 0x90178) != 0)
    {
        *(int*)((char*)this + 0x90174) = 0;
        tmp = *(int*)((char*)this + 0x90168);
        *(int*)((char*)this + 0x90174) = 0;
        *(int*)((char*)this + 0x90174) = 0;
        *(int*)((char*)this + 0x90174) = 0;
        DRAWPOLYS_COLOR(0x91028)
        pSetRect(&rect90164, *(int*)((char*)this + 0x90164), tmp,
                 *(int*)((char*)this + 0x9016c) + *(int*)((char*)this + 0x90164),
                 *(int*)((char*)this + 0x90170) + tmp);
        DRAWPOLYS_FILL(rect90164)
        DRAWPOLYS_OUTLINE(rect90164)
        DRAWPOLYS_PANEL(0x6dfdc)
        DRAWPOLYS_PANEL(0x6dfec)
    }
    *(int*)((char*)this + 0x900f4) = 0;
    if (*(int*)((char*)this + 0x900f8) != 0)
    {
        *(int*)((char*)this + 0x900f4) = 0;
        tmp = *(int*)((char*)this + 0x900e8);
        *(int*)((char*)this + 0x900f4) = 0;
        *(int*)((char*)this + 0x900f4) = 0;
        *(int*)((char*)this + 0x900f4) = 0;
        DRAWPOLYS_COLOR(0x91018)
        pSetRect(&rect900e4, *(int*)((char*)this + 0x900e4), tmp,
                 *(int*)((char*)this + 0x900ec) + *(int*)((char*)this + 0x900e4),
                 *(int*)((char*)this + 0x900f0) + tmp);
        DRAWPOLYS_FILL(rect900e4)
    }
    *(int*)((char*)this + 0x90234) = 0;
    if (*(int*)((char*)this + 0x90238) != 0)
    {
        *(int*)((char*)this + 0x90234) = 0;
        tmp = *(int*)((char*)this + 0x90228);
        *(int*)((char*)this + 0x90234) = 0;
        *(int*)((char*)this + 0x90234) = 0;
        *(int*)((char*)this + 0x90234) = 0;
        DRAWPOLYS_COLOR(0x91040)
        pSetRect(&rect90224, *(int*)((char*)this + 0x90224), tmp,
                 *(int*)((char*)this + 0x9022c) + *(int*)((char*)this + 0x90224),
                 *(int*)((char*)this + 0x90230) + tmp);
        DRAWPOLYS_FILL(rect90224)
        DRAWPOLYS_OUTLINE(rect90224)
    }
    *(int*)((char*)this + 0x90254) = 0;
    if (*(int*)((char*)this + 0x90258) != 0)
    {
        *(int*)((char*)this + 0x90254) = 0;
        tmp = *(int*)((char*)this + 0x90248);
        *(int*)((char*)this + 0x90254) = 0;
        *(int*)((char*)this + 0x90254) = 0;
        *(int*)((char*)this + 0x90254) = 0;
        DRAWPOLYS_COLOR(0x91044)
        pSetRect(&rect90244, *(int*)((char*)this + 0x90244), tmp,
                 *(int*)((char*)this + 0x9024c) + *(int*)((char*)this + 0x90244),
                 *(int*)((char*)this + 0x90250) + tmp);
        DRAWPOLYS_FILL(rect90244)
        DRAWPOLYS_OUTLINE(rect90244)
        DRAWPOLYS_PANEL(0x6dffc)
    }
    *(int*)((char*)this + 0x90194) = 0;
    if (*(int*)((char*)this + 0x90198) != 0)
    {
        *(int*)((char*)this + 0x90194) = 0;
        tmp = *(int*)((char*)this + 0x90188);
        *(int*)((char*)this + 0x90194) = 0;
        *(int*)((char*)this + 0x90194) = 0;
        *(int*)((char*)this + 0x90194) = 0;
        DRAWPOLYS_COLOR(0x9102c)
        pSetRect(&rect90184, *(int*)((char*)this + 0x90184), tmp,
                 *(int*)((char*)this + 0x9018c) + *(int*)((char*)this + 0x90184),
                 *(int*)((char*)this + 0x90190) + tmp);
        DRAWPOLYS_FILL(rect90184)
        DRAWPOLYS_OUTLINE(rect90184)
        DRAWPOLYS_PANEL(0x6e00c)
        DRAWPOLYS_PANEL(0x6cb0c)
        DRAWPOLYS_PANEL(0x6cb3c)
        DRAWPOLYS_PANEL(0x6cb4c)
        DRAWPOLYS_PANEL(0x6cb5c)
        DRAWPOLYS_PANEL(0x6cb6c)
    }
    *(int*)((char*)this + 0x90274) = 0;
    if (*(int*)((char*)this + 0x90278) != 0)
    {
        *(int*)((char*)this + 0x90274) = 0;
        tmp = *(int*)((char*)this + 0x90268);
        *(int*)((char*)this + 0x90274) = 0;
        *(int*)((char*)this + 0x90274) = 0;
        *(int*)((char*)this + 0x90274) = 0;
        DRAWPOLYS_COLOR(0x91030)
        pSetRect(&rect90264, *(int*)((char*)this + 0x90264), tmp,
                 *(int*)((char*)this + 0x9026c) + *(int*)((char*)this + 0x90264),
                 *(int*)((char*)this + 0x90270) + tmp);
        DRAWPOLYS_FILL(rect90264)
        DRAWPOLYS_OUTLINE(rect90264)
        DRAWPOLYS_PANEL(0x6e01c)
    }
}

#undef DRAWPOLYS_COLOR
#undef DRAWPOLYS_FILL
#undef DRAWPOLYS_OUTLINE
#undef DRAWPOLYS_PANEL

#define VIT_I(off)          (*(int*)((char*)self + (off)))
#define VIT_S(off)          (*(short*)((char*)self + (off)))
#define VIT_SLOT            (*(char*)((char*)self + 0xfe0))
#define VIT_SLOT_I(off)     (*(int*)((char*)self + VIT_SLOT * 0x640 + (off)))
#define VIT_STATE           EQ_GLOBAL(int, 0x0058FC6C)
#define VIT_PC              EQ_GLOBAL(void*, 0x006E4DA8)
#define VIT_TARGET          EQ_GLOBAL(char*, 0x006E4DAC)
#define VIT_CUR_HP()        EQ_METHOD(1, int, 0x004199C0, (void*, int), VIT_PC)(0)
#define VIT_MAX_HP()        EQ_METHOD(1, int, 0x00419A2F, (void*, int), VIT_PC)(0)
#define VIT_CUR_MANA()      EQ_METHOD(0, int, 0x00419BA3, (void*), VIT_PC)()
#define VIT_MAX_MANA()      EQ_METHOD(0, int, 0x00419BCA, (void*), VIT_PC)()
#define VIT_OV_DE8          (pinstEQ_TextIO_guess->displayOverlay_int_guess)
#define VIT_OV_E1C          (*(int*)(EQ_GLOBAL(char*, 0x006E4E1C) + 4))
#define VIT_OV_E44          (pinstEQ_Main_guess->overlaySprite_int_guess)
#define VIT_OV_E40          (*(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4))
#define VIT_DRAW_PARTIAL(ov, a, b) \
    EQ_METHOD(4, void, 0x00450125, (void*, int, void*, void*, int), self)((ov), (a), (b), 1)
#define VIT_PARTY_VITALS(a, b) \
    EQ_METHOD(3, void, 0x004500FD, (void*, void*, void*, int), self)((a), (b), 1)
#define VIT_DRAW(img, rect) \
    EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), (img))((rect), 1)
#define VIT_STRCPY(dst, src) strcpy((dst), (src))
#define VIT_STRCMP(a, b)    strcmp((a), (b))

// FUNCTION: TRILOGY 0x00468df1
// Animates and redraws the vitals bars: player HP/mana/fatigue, party HP bars and names, target HP bar.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (self in ECX, no stack arguments), no return value. Does nothing without a profile. Refreshes the
// vitals bars: the five party names (self+0x5f0c0, 30 bytes each, from 0x005FE8BD) and pointers (self+0x5f158, from
// 0x005FE954) mark a slot's redraw flag (self+0x8a1c4) and reset its bar (self+0x6c572) when they change; the
// target (self+0x5f0bc) resets the target bar (self+0x6c570). Then the player's HP/mana/fatigue bars (self+0x6c56a/c/e,
// 0..0x42) step towards their values, the party bars (0..0x42) and the target bar (0..0x62) likewise; a changed bar
// sets its redraw flag. Draws each flagged bar, either through the overlay (game state 2, rects from self+0x90044..)
// or through the images at 0x006B3610/0x006B341C, then the party names and the target bar.
void EQ_Main::drawVials(void)
{
    void* self = (void*)this;
    bool partyChanged;
    short tmp;
    short tmp2;
    short tmp3;
    short hpBarShown;
    int work;
    int temp;
    int srcTop;
    int srcBottom;
    int maxMana;
    int textColor;
    int memberIndex;
    char* partyName;
    int* partyMember;
    char* pcVar10;
    int* memberEntry;
    short* partyBar;
    unsigned short* partyBarCursor;
    tSetRect_guess_t pSetRect;
    RECT rectA;
    RECT rectB;
    char nameBuf[60];

    partyChanged = false;
    if (VIT_PC == 0)
        return;
    if (VIT_I(8) == 0)
    {
        VIT_SLOT_I(0x8a154) = 0;
        VIT_SLOT_I(0x8a1bc) = 0;
        work = 0;
        do
        {
            temp = VIT_SLOT * 400 + work;
            work = work + 1;
            VIT_I(0x8a1c4 + temp * 4) = 0;
        } while (work < 5);
    }
    memberIndex = 0;
    partyMember = EQ_ARRAY(int, 0x005FE954);
    partyName = EQ_ARRAY(char, 0x005FE8BD);
    memberEntry = (int*)((char*)self + 0x5f158);
    partyBarCursor = (unsigned short*)((char*)self + 0x6c572);
    pcVar10 = (char*)self + 0x5f0c0;
    do
    {
        if (VIT_STRCMP(pcVar10, partyName) != 0 && VIT_I(8) == 0)
        {
            *partyBarCursor = 0xffff;
            partyChanged = true;
            VIT_I(0x8a1c4 + (VIT_SLOT * 400 + memberIndex) * 4) = 1;
            VIT_STRCPY(pcVar10, partyName);
        }
        if (*memberEntry != *partyMember && VIT_I(8) == 0)
        {
            *partyBarCursor = 0xffff;
            partyChanged = true;
            VIT_I(0x8a1c4 + (VIT_SLOT * 400 + memberIndex) * 4) = 1;
            *memberEntry = *partyMember;
        }
        memberIndex = memberIndex + 1;
        partyName = partyName + 0x1e;
        pcVar10 = pcVar10 + 0x1e;
        partyBarCursor = partyBarCursor + 1;
        partyMember = partyMember + 1;
        memberEntry = memberEntry + 1;
    } while ((int)partyName < (int)EQ_ARRAY(char, 0x005FE953));
    work = 0;
    do
    {
        if (partyChanged)
            VIT_I(0x8a1c4 + (VIT_SLOT * 400 + work) * 4) = 1;
        work = work + 1;
    } while (work < 5);
    if (VIT_TARGET != *(char**)((char*)self + 0x5f0bc) && VIT_I(8) == 0)
    {
        *(char**)((char*)self + 0x5f0bc) = VIT_TARGET;
        VIT_SLOT_I(0x8a154) = 1;
        VIT_S(0x6c570) = -1;
    }
    tmp = VIT_CUR_HP();
    tmp2 = VIT_CUR_MANA();
    tmp3 = (short)*(char*)((char*)VIT_PC + 0x1046);
    if (tmp < 1 && (tmp = tmp + 0xb, tmp < 0))
        tmp = 0;
    if (tmp2 < 0)
        tmp2 = 0;
    if (tmp3 < 0)
        tmp3 = 0;
    work = VIT_MAX_HP();
    if (work < 1)
        tmp = 0;
    else
    {
        work = VIT_CUR_HP();
        if (work < 1)
            work = 0xb;
        else
            work = VIT_MAX_HP();
        tmp = (short)((tmp * 0x42) / work);
    }
    maxMana = VIT_MAX_MANA();
    if ((short)maxMana < 1)
        tmp2 = 0;
    else
    {
        maxMana = VIT_MAX_MANA();
        tmp2 = (short)((tmp2 * 0x42) / (int)(short)maxMana);
    }
    if (*(char*)((char*)VIT_PC + 0x1046) < 1)
        tmp3 = 0;
    else
        tmp3 = (short)((tmp3 * 0x42) / 100);
    hpBarShown = 0x42;
    tmp3 = 0x42 - tmp3;
    if (tmp < 0)
        tmp = 0;
    if (tmp2 < 0)
        tmp2 = 0;
    if (tmp3 < 0)
        tmp3 = 0;
    if (0x42 < tmp)
        tmp = hpBarShown;
    if (0x42 < tmp2)
        tmp2 = hpBarShown;
    if (0x42 < tmp3)
        tmp3 = hpBarShown;
    hpBarShown = VIT_S(0x6c56a);
    if (hpBarShown == -1)
    {
        if (VIT_I(8) == 0)
        {
            VIT_S(0x6c56a) = tmp;
hpBarChanged:
            VIT_SLOT_I(0x8a1bc) = 1;
        }
    }
    else if (hpBarShown < tmp)
    {
        if (VIT_I(8) == 0)
        {
            VIT_SLOT_I(0x8a1bc) = 1;
            if (7 < (int)tmp - (int)VIT_S(0x6c56a))
                VIT_S(0x6c56a) = VIT_S(0x6c56a) + 3;
            VIT_S(0x6c56a) = VIT_S(0x6c56a) + 1;
        }
    }
    else if (tmp < hpBarShown && VIT_I(8) == 0)
    {
        if (7 < (int)hpBarShown - (int)tmp)
            VIT_S(0x6c56a) = hpBarShown - 3;
        VIT_S(0x6c56a) = VIT_S(0x6c56a) - 1;
        goto hpBarChanged;
    }
    tmp = VIT_S(0x6c56c);
    if (tmp == -1)
    {
        if (VIT_I(8) != 0)
            goto stepFatigueBar;
        VIT_S(0x6c56c) = tmp2;
manaBarChanged:
        VIT_SLOT_I(0x8a1bc) = 1;
    }
    else if (tmp < tmp2)
    {
        if (VIT_I(8) == 0)
        {
            tmp = tmp + 1;
storeManaBar:
            VIT_S(0x6c56c) = tmp;
            goto manaBarChanged;
        }
    }
    else if (tmp2 < tmp && VIT_I(8) == 0)
    {
        tmp = tmp - 1;
        goto storeManaBar;
    }
stepFatigueBar:
    tmp = VIT_S(0x6c56e);
    if (tmp == -1)
    {
        if (VIT_I(8) != 0)
            goto drawPlayerBars;
        VIT_S(0x6c56e) = tmp3;
fatigueBarChanged:
        VIT_SLOT_I(0x8a1bc) = 1;
    }
    else if (tmp < tmp3)
    {
        if (VIT_I(8) == 0)
        {
            tmp = tmp + 1;
storeFatigueBar:
            VIT_S(0x6c56e) = tmp;
            goto fatigueBarChanged;
        }
    }
    else if (tmp3 < tmp && VIT_I(8) == 0)
    {
        tmp = tmp - 1;
        goto storeFatigueBar;
    }
drawPlayerBars:
    pSetRect = g_setrect_guess;
    work = VIT_SLOT * 0x640 + (int)self;
    if (((*(int*)(work + 0x843fc) != 0) || (VIT_I(0xc) != 0)) || (*(int*)(work + 0x8a1bc) != 0))
    {
        if (VIT_STATE != 2)
        {
            if (VIT_CUR_HP() < 1)
            {
                pSetRect(&rectA, 0x1da, 0x103, 0x230, 0x108);
                pSetRect(&rectB, 0x219, 0x2c, 0x26f, 0x32);
                VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectA, &rectB);
            }
            else
            {
                pSetRect(&rectA, 0x21c, 0x2a, 0x26c, 0x4a);
                pSetRect(&rectB, 0x21c, 0x2a, 0x26c, 0x4a);
                VIT_PARTY_VITALS(&rectA, &rectB);
            }
        }
        if (VIT_STATE == 2)
        {
            VIT_I(0x90054) = 0;
            if (VIT_I(0x90058) != 0)
            {
                VIT_I(0x90054) = 0;
                VIT_I(0x90054) = 0;
                pSetRect(&rectA, VIT_I(0x90044) + 0xf, VIT_I(0x90048) + 0x24,
                         VIT_I(0x90044) + 0x51, VIT_I(0x90048) + 0x29);
                pSetRect(&rectB, 0x91, 0x145, 0xd2, 0x14b);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
                if (VIT_CUR_HP() < 1)
                {
                    work = VIT_S(0x6c56a) + 0xd3;
                    if (0x113 < work)
                        work = 0x113;
                    srcBottom = 0x12f;
                    srcTop = 0x129;
                    temp = 0xd2;
                }
                else
                {
                    work = VIT_S(0x6c56a) + 0x92;
                    if (0xd2 < work)
                        work = 0xd2;
                    srcBottom = 0x128;
                    srcTop = 0x122;
                    temp = 0x91;
                }
                pSetRect(&rectB, temp, srcTop, work, srcBottom);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
            }
        }
        else
        {
            pSetRect(&rectA, 0x223, 0x2c, 0x265, 0x31);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B3610), &rectA);
            *EQ_GLOBAL(int*, 0x006B341C) = VIT_OV_E44;
            if (VIT_CUR_HP() < 1)
            {
                pSetRect(&rectB, 0x1e2, 0x103, VIT_S(0x6c56a) + 0x1e3, 0x108);
                VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectB, &rectA);
            }
            else
            {
                pSetRect((RECT*)(EQ_GLOBAL(int*, 0x006B341C) + 1), 0x222, 0x2c, VIT_S(0x6c56a) + 0x223, 0x31);
                VIT_DRAW(EQ_GLOBAL(void*, 0x006B341C), &rectA);
            }
        }
        if (VIT_STATE == 2)
        {
            VIT_I(0x90054) = 0;
            if (VIT_I(0x90058) != 0)
            {
                VIT_I(0x90054) = 0;
                VIT_I(0x90054) = 0;
                pSetRect(&rectA, VIT_I(0x90044) + 0xf, VIT_I(0x90048) + 0x2f,
                         VIT_I(0x90044) + 0x51, VIT_I(0x90048) + 0x34);
                pSetRect(&rectB, 0x91, 0x145, 0xd2, 0x14b);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
                work = VIT_S(0x6c56c) + 0xd3;
                if (0x113 < work)
                    work = 0x113;
                pSetRect(&rectB, 0xd2, 0x122, work, 0x128);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
            }
        }
        else
        {
            pSetRect(&rectA, 0x223, 0x37, 0x265, 0x3c);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B3610), &rectA);
            pSetRect((RECT*)(EQ_GLOBAL(int*, 0x006B341C) + 1), 0x222, 0x37, VIT_S(0x6c56c) + 0x223, 0x3c);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B341C), &rectA);
        }
        if (VIT_STATE == 2)
        {
            VIT_I(0x90054) = 0;
            if (VIT_I(0x90058) != 0)
            {
                VIT_I(0x90054) = 0;
                VIT_I(0x90054) = 0;
                pSetRect(&rectA, VIT_I(0x90044) + 0xf, VIT_I(0x90048) + 0x3a,
                         VIT_I(0x90044) + 0x51, VIT_I(0x90048) + 0x3f);
                pSetRect(&rectB, 0x91, 0x145, 0xd2, 0x14b);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
                work = VIT_S(0x6c56e) + 0x92;
                if (0xd2 < work)
                    work = 0xd2;
                pSetRect(&rectB, 0x91, 0x129, work, 0x12f);
                VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
            }
        }
        else
        {
            pSetRect(&rectA, 0x223, 0x42, 0x265, 0x47);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B3610), &rectA);
            pSetRect((RECT*)(EQ_GLOBAL(int*, 0x006B341C) + 1), 0x222, 0x42, VIT_S(0x6c56e) + 0x223, 0x47);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B341C), &rectA);
        }
    }
    if (g_attacking_guess != 0 || g_rangedAttacking_guess != 0)
    {
        if (VIT_I(8) == 0)
            *(char*)((char*)self + 0xed0) = *(char*)((char*)self + 0xed0) ^ 1;
        VIT_I(0x90054) = 0;
        if (VIT_I(0x90058) != 0)
        {
            if (*(char*)((char*)self + 0xed0) == 0)
            {
                temp = 0xbc;
                work = 0x5e;
            }
            else
            {
                temp = 0x5e;
                work = 0;
            }
            pSetRect(&rectA, work, 0x188, temp, 0x1cd);
            VIT_I(0x90054) = 0;
            VIT_I(0x90054) = 0;
            pSetRect(&rectB, VIT_I(0x90044) + 1, VIT_I(0x90048) + 1,
                     EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
            VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectA, &rectB);
        }
    }
    memberIndex = 0;
    memberEntry = EQ_ARRAY(int, 0x005FE954);
    partyBar = (short*)((char*)self + 0x6c572);
    do
    {
        if (*memberEntry != 0)
        {
            work = *(int*)(*memberEntry + 0x8c);
            if ((work > 0 && partyBar[5] <= 0) || (work <= 0 && partyBar[5] > 0))
            {
                *partyBar = -1;
                partyBar[5] = *(short*)(*memberEntry + 0x8c);
            }
            work = *memberEntry;
            tmp = *(short*)(work + 0x8c);
            if (tmp < 1 && (tmp = tmp + 0xb, tmp < 0))
                tmp = 0;
            temp = *(int*)(work + 0x88);
            if (temp < 1)
                tmp = 0;
            else
            {
                if (*(int*)(work + 0x8c) < 1)
                    temp = 0xb;
                tmp = (short)((tmp * 0x42) / temp);
                if (tmp < 0)
                    tmp = 0;
                if (0x42 < tmp)
                    tmp = 0x42;
            }
            tmp2 = *partyBar;
            if (tmp2 == -1)
            {
                if (VIT_I(8) == 0)
                {
                    *partyBar = tmp;
memberBarChanged:
                    VIT_I(0x8a1c4 + (VIT_SLOT * 400 + memberIndex) * 4) = 1;
                }
            }
            else if (tmp2 < tmp)
            {
                if (VIT_I(8) == 0)
                {
                    tmp2 = tmp2 + 1;
storeMemberBar:
                    *partyBar = tmp2;
                    goto memberBarChanged;
                }
            }
            else if (tmp < tmp2 && VIT_I(8) == 0)
            {
                tmp2 = tmp2 - 1;
                goto storeMemberBar;
            }
        }
        memberIndex = memberIndex + 1;
        memberEntry = memberEntry + 1;
        partyBar = partyBar + 1;
    } while ((int)memberEntry < (int)g_name_buffer_guess);
    memberIndex = 0;
    do
    {
        if (((VIT_I(0xc) != 0) ||
             (work = VIT_SLOT * 400 + memberIndex, VIT_I(0x84404 + work * 4) != 0)) ||
            (VIT_I(0x8a1c4 + work * 4) != 0))
        {
            if (VIT_STATE == 1)
            {
                work = (VIT_SLOT * 400 + memberIndex) * 0x10 + 0x6d8ec + (int)self;
                VIT_PARTY_VITALS((void*)work, (void*)work);
            }
            if (EQ_ARRAY(char, 0x005FE8BD)[memberIndex * 0x1e] != 0)
            {
                if (EQ_ARRAY(int, 0x005FE954)[memberIndex] == 0)
                {
                    if (VIT_STATE == 2)
                    {
                        VIT_I(0x90074) = 0;
                        if (VIT_I(0x90078) == 0)
                            goto nextMemberBar;
                        VIT_I(0x90074) = 0;
                        VIT_I(0x90074) = 0;
                        work = VIT_I(0x90068) + 0xd + memberIndex * 0x16;
                        pSetRect(&rectB, VIT_I(0x90064) + 0x19, work, VIT_I(0x90064) + 0x5b, work + 5);
                        pSetRect(&rectA, 0x91, 0x130, 0xd2, 0x136);
                        VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectA, &rectB);
                        goto nextMemberBar;
                    }
                    switch (memberIndex)
                    {
                    case 4:
                        pSetRect(&rectA, 0x1ed, 0x165, 0x24b, 0x17f);
                        pSetRect(&rectB, 0x215, 199, 0x272, 0xe1);
                        break;
                    case 3:
                        pSetRect(&rectA, 0x1ed, 0x14f, 0x24b, 0x165);
                        pSetRect(&rectB, 0x215, 0xaf, 0x272, 199);
                        break;
                    case 2:
                        pSetRect(&rectA, 0x1ed, 0x139, 0x24b, 0x14f);
                        pSetRect(&rectB, 0x215, 0x99, 0x272, 0xaf);
                        break;
                    case 1:
                        pSetRect(&rectA, 0x1ed, 0x123, 0x24b, 0x139);
                        pSetRect(&rectB, 0x215, 0x83, 0x272, 0x99);
                        break;
                    case 0:
                        pSetRect(&rectA, 0x1ed, 0x108, 0x24b, 0x123);
                        pSetRect(&rectB, 0x215, 0x68, 0x272, 0x83);
                        break;
                    }
                    VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectA, &rectB);
                }
                else if (VIT_STATE == 2)
                {
                    VIT_I(0x90074) = 0;
                    if (VIT_I(0x90078) == 0)
                        goto nextMemberBar;
                    VIT_I(0x90074) = 0;
                    VIT_I(0x90074) = 0;
                    work = VIT_I(0x90068) + 0xd + memberIndex * 0x16;
                    pSetRect(&rectA, VIT_I(0x90064) + 0x19, work, VIT_I(0x90064) + 0x5b, work + 5);
                    pSetRect(&rectB, 0x91, 0x145, 0xd2, 0x14b);
                    VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
                    work = (int)*(short*)((char*)self + 0x6c572 + memberIndex * 2);
                    if (*(int*)(EQ_ARRAY(int, 0x005FE954)[memberIndex] + 0x8c) < 1)
                    {
                        work = work + 0xd3;
                        if (0x113 < work)
                            work = 0x113;
                        srcBottom = 0x12f;
                        srcTop = 0x129;
                        temp = 0xd2;
                    }
                    else
                    {
                        work = work + 0x92;
                        if (0xd2 < work)
                            work = 0xd2;
                        srcBottom = 0x128;
                        srcTop = 0x122;
                        temp = 0x91;
                    }
                    pSetRect(&rectB, temp, srcTop, work, srcBottom);
                    VIT_DRAW_PARTIAL(VIT_OV_E1C, &rectB, &rectA);
                }
                else
                {
                    switch (memberIndex)
                    {
                    case 4:
                        pSetRect(&rectA, 0x18e, 0x165, 0x1ec, 0x17f);
                        pSetRect(&rectB, 0x215, 0xc5, 0x272, 0xe1);
                        break;
                    case 3:
                        pSetRect(&rectA, 0x18e, 0x14f, 0x1ec, 0x165);
                        pSetRect(&rectB, 0x215, 0xaf, 0x272, 0xc5);
                        break;
                    case 2:
                        pSetRect(&rectA, 0x18e, 0x139, 0x1ec, 0x14f);
                        pSetRect(&rectB, 0x215, 0x99, 0x272, 0xaf);
                        break;
                    case 1:
                        pSetRect(&rectA, 0x18e, 0x123, 0x1ec, 0x139);
                        pSetRect(&rectB, 0x215, 0x83, 0x272, 0x99);
                        break;
                    case 0:
                        pSetRect(&rectA, 0x18e, 0x108, 0x1ec, 0x123);
                        pSetRect(&rectB, 0x215, 0x68, 0x272, 0x83);
                        break;
                    }
                    VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectA, &rectB);
                    if (*(int*)(EQ_ARRAY(int, 0x005FE954)[memberIndex] + 0x8c) < 1)
                    {
                        pSetRect(&rectA, 0x1da, 0x103, 0x230, 0x108);
                        pSetRect(&rectB, 0x21a, memberIndex * 0x16 + 0x79, 0x26f, memberIndex * 0x16 + 0x7e);
                        VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectA, &rectB);
                    }
                    pSetRect(&rectA, 0x223, memberIndex * 0x16 + 0x79, 0x265, memberIndex * 0x16 + 0x7e);
                    VIT_DRAW(EQ_GLOBAL(void*, 0x006B3610), &rectA);
                    *EQ_GLOBAL(int*, 0x006B341C) = VIT_OV_DE8;
                    if (0 < *(int*)(EQ_ARRAY(int, 0x005FE954)[memberIndex] + 0x8c))
                    {
                        pSetRect((RECT*)(EQ_GLOBAL(int*, 0x006B341C) + 1), 0x19d, 0x119,
                                 *(short*)((char*)self + 0x6c572 + memberIndex * 2) + 0x19e, 0x11e);
                        VIT_DRAW(EQ_GLOBAL(void*, 0x006B341C), &rectA);
                        goto nextMemberBar;
                    }
                    pSetRect(&rectB, 0x1e3, 0x103,
                             *(short*)((char*)self + 0x6c572 + memberIndex * 2) + 0x1e3, 0x108);
                    VIT_DRAW_PARTIAL(VIT_OV_DE8, &rectB, &rectA);
                }
            }
        }
nextMemberBar:
        memberIndex = memberIndex + 1;
    } while (memberIndex < 5);
    VIT_I(0x90074) = 0;
    if (VIT_I(0x90078) != 0 || VIT_STATE != 2)
    {
        memberIndex = 0;
        do
        {
            if ((((VIT_I(0xc) != 0) ||
                  (work = VIT_SLOT * 400 + memberIndex, VIT_I(0x84404 + work * 4) != 0)) ||
                 (VIT_I(0x8a1c4 + work * 4) != 0)) &&
                EQ_ARRAY(char, 0x005FE8BD)[memberIndex * 0x1e] != 0)
            {
                work = VIT_I(0x90064);
                VIT_I(0x90074) = 0;
                temp = VIT_I(0x90068);
                VIT_I(0x90074) = 0;
                switch (memberIndex)
                {
                case 4:
                    VIT_STRCPY(nameBuf, EQ_ARRAY(char, 0x005FE935));
                    pSetRect(&rectA, work + 0x12, temp + 0x58,
                             EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
                    if (*(char*)((char*)self + 0xb4) == 4)
                        goto useColor10;
                    pcVar10 = EQ_ARRAY(char, 0x005FE935);
                    goto pickNameColor;
                case 3:
                    VIT_STRCPY(nameBuf, EQ_ARRAY(char, 0x005FE917));
                    pSetRect(&rectA, work + 0x12, temp + 0x42,
                             EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
                    if (*(char*)((char*)self + 0xb4) == 3)
                        goto useColor10;
                    pcVar10 = EQ_ARRAY(char, 0x005FE917);
                    goto pickNameColor;
                case 2:
                    VIT_STRCPY(nameBuf, EQ_ARRAY(char, 0x005FE8F9));
                    pSetRect(&rectA, work + 0x12, temp + 0x2c,
                             EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
                    if (*(char*)((char*)self + 0xb4) == 2)
                        goto useColor10;
                    pcVar10 = EQ_ARRAY(char, 0x005FE8F9);
                    goto pickNameColor;
                case 1:
                    VIT_STRCPY(nameBuf, EQ_ARRAY(char, 0x005FE8DB));
                    pSetRect(&rectA, work + 0x12, temp + 0x16,
                             EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
                    if (*(char*)((char*)self + 0xb4) == 1)
                        goto useColor10;
                    pcVar10 = EQ_ARRAY(char, 0x005FE8DB);
                    goto pickNameColor;
                case 0:
                    VIT_STRCPY(nameBuf, EQ_ARRAY(char, 0x005FE8BD));
                    pSetRect(&rectA, work + 0x12, temp,
                             EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
                    if (*(char*)((char*)self + 0xb4) != 0)
                    {
                        pcVar10 = EQ_ARRAY(char, 0x005FE8BD);
                        goto pickNameColor;
                    }
useColor10:
                    textColor = 10;
                    goto writeName;
                default:
                    goto nextMemberName;
                }
pickNameColor:
                work = VIT_STRCMP(g_name_buffer_guess, pcVar10);
                if (work == 0)
                    textColor = 0xf;
                else
                    textColor = 7;
writeName:
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(nameBuf, rectA.left, rectA.top, textColor,
                    EQ_GLOBAL(int, 0x006EFDFC), 1);
            }
nextMemberName:
            memberIndex = memberIndex + 1;
        } while (memberIndex < 5);
    }
    if (VIT_STATE != 1)
        VIT_SLOT_I(0x84394) = 1;
    if (VIT_TARGET != 0)
    {
        tmp = *(short*)(VIT_TARGET + 0x8c);
        if (tmp < 0)
            tmp = 0;
        work = *(int*)(VIT_TARGET + 0x88);
        if (work < tmp)
            tmp = (short)work;
        if (work < 1)
            tmp = 0;
        else
            tmp = (short)((tmp * 0x62) / work);
        if (VIT_S(0x6c570) == -1)
        {
            if (1 < *(u8*)(VIT_TARGET + 0x98))
                tmp = 0;
            if (VIT_I(8) != 0)
                goto drawTargetBar;
            VIT_S(0x6c570) = tmp;
        }
        else
        {
            if (1 < *(u8*)(VIT_TARGET + 0x98))
            {
                VIT_S(0x6c570) = 0;
                tmp = 0;
            }
            tmp2 = VIT_S(0x6c570);
            if (tmp2 < tmp)
            {
                if (VIT_I(8) != 0)
                    goto drawTargetBar;
                tmp2 = tmp2 + 1;
            }
            else
            {
                if (tmp2 <= tmp || VIT_I(8) != 0)
                    goto drawTargetBar;
                tmp2 = tmp2 - 1;
            }
            VIT_S(0x6c570) = tmp2;
        }
        VIT_SLOT_I(0x8a154) = 1;
    }
drawTargetBar:
    if (((VIT_I(0xc) != 0) ||
         (work = VIT_SLOT * 0x640 + (int)self, *(int*)(work + 0x84394) != 0)) ||
        (*(int*)(work + 0x8a154) != 0))
    {
        if (VIT_STATE == 1)
        {
            work = VIT_SLOT * 0x1900 + 0x6d72c + (int)self;
            VIT_DRAW_PARTIAL(VIT_OV_E44, (void*)work, (void*)work);
        }
        if (VIT_TARGET != 0 && (VIT_I(0x90094) = 0, VIT_I(0x90098) != 0 || VIT_STATE != 2))
        {
            VIT_I(0x90094) = 0;
            VIT_I(0x90094) = 0;
            pSetRect(&rectA, VIT_I(0x90084) + 10, VIT_I(0x90088) + 0x14,
                     VIT_I(0x90084) + 0x6c, VIT_I(0x90088) + 0x1e);
            *EQ_GLOBAL(int*, 0x006B341C) = VIT_OV_E40;
            pSetRect((RECT*)(EQ_GLOBAL(int*, 0x006B341C) + 1), 0, 0x1b0, (int)VIT_S(0x6c570), 0x1ba);
            VIT_DRAW(EQ_GLOBAL(void*, 0x006B341C), &rectA);
        }
    }
}

#undef VIT_I
#undef VIT_S
#undef VIT_SLOT
#undef VIT_SLOT_I
#undef VIT_STATE
#undef VIT_PC
#undef VIT_TARGET
#undef VIT_CUR_HP
#undef VIT_MAX_HP
#undef VIT_CUR_MANA
#undef VIT_MAX_MANA
#undef VIT_OV_DE8
#undef VIT_OV_E1C
#undef VIT_OV_E44
#undef VIT_OV_E40
#undef VIT_DRAW_PARTIAL
#undef VIT_PARTY_VITALS
#undef VIT_DRAW
#undef VIT_STRCPY
#undef VIT_STRCMP

// FUNCTION: TRILOGY 0x0046a760
// Reorders the track list by the ranger's sort mode: original order, by key up/down, or NPCs by level.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: outer and inner loop counters still use different registers and instruction order.
// __thiscall (self in ECX; `unused` is the EDX dummy), no return value. Sorts the track table of 1000
// 16-byte entries at self+0x685f8 in place: [0] object pointer, [1] float key, [2] index, [3] spare.
// Mode 0 (the default) follows each entry's index [2]; the mode is DAT_006b675c when the profile's class
// (+0x36) is 4 (ranger). Modes 1/3 move the lowest/highest key >= -0.0001 (the constant at 0x00535590) to
// the front of the rest of the table; modes 2/4 do the same with the byte at +0x9d of the object (2 the
// highest, 4 the lowest). Each pass fixes the entry at local_c - 1 and moves on to the next.
void __fastcall DoTrackSort(void* self, int unused)
{
    struct TrackEntry_guess
    {
        char* object;
        float key;
        int index;
        int spare;
    };

    TrackEntry_guess* otherEntry;
    int nextObject;
    int entryIndex;
    TrackEntry_guess* curEntry;
    TrackEntry_guess* candidate;
    int local_14;
    int local_10;
    int local_c;
    TrackEntry_guess* nextEntry;
    int trackCount;

    local_10 = 0;
    if (EQ_GLOBAL(void*, 0x006E4DA8) != 0 && g_pc_guess->class_ == 4)
        local_10 = EQ_GLOBAL(int, 0x006B675C);
    const float minimumKey = EQ_GLOBAL(float, 0x00535590);
    curEntry = (TrackEntry_guess*)((char*)self + 0x685f8);
    trackCount = 1000;
    local_c = 1;
    for (; local_c <= trackCount; local_c++)
    {
        bool swapWithNeighbour = false;
        if (local_10 == 0)
        {
            entryIndex = curEntry->index;
            while (entryIndex != local_c + -1 && entryIndex != -1)
            {
                otherEntry = (TrackEntry_guess*)((char*)self + entryIndex * 0x10 + 0x685f8);
                *(TrackEntry_guess*)((char*)self + 0x6c478) = *otherEntry;
                otherEntry = (TrackEntry_guess*)((char*)self + curEntry->index * 0x10 + 0x685f8);
                *otherEntry = *curEntry;
                *curEntry = *(TrackEntry_guess*)((char*)self + 0x6c478);
                entryIndex = curEntry->index;
            }
        }
        else if (local_10 == 1)
        {
            if (local_c < trackCount)
            {
                local_14 = trackCount - local_c;
                otherEntry = curEntry;
                do
                {
                    candidate = otherEntry + 1;
                    // Negated relations preserve the original x87 branch behavior for unordered keys.
                    if (!(minimumKey > otherEntry[1].key) && !(otherEntry[1].key >= curEntry->key))
                    {
                        *(TrackEntry_guess*)((char*)self + 0x6c478) = *curEntry;
                        *curEntry = *candidate;
                        *candidate = *(TrackEntry_guess*)((char*)self + 0x6c478);
                    }
                    local_14 = local_14 + -1;
                    otherEntry = candidate;
                } while (local_14 != 0);
            }
        }
        else if (local_10 == 3)
        {
            if (local_c < trackCount)
            {
                local_14 = trackCount - local_c;
                otherEntry = curEntry;
                do
                {
                    candidate = otherEntry + 1;
                    if (!(minimumKey > otherEntry[1].key) && otherEntry[1].key > curEntry->key)
                    {
                        *(TrackEntry_guess*)((char*)self + 0x6c478) = *curEntry;
                        *curEntry = *candidate;
                        *candidate = *(TrackEntry_guess*)((char*)self + 0x6c478);
                    }
                    local_14 = local_14 + -1;
                    otherEntry = candidate;
                } while (local_14 != 0);
            }
        }
        else
        {
            entryIndex = local_c;
            if (local_10 == 2)
            {
                if (curEntry->object != 0 && (otherEntry = curEntry, *(char*)(curEntry->object + 0x98) == 1))
                {
                    for (; entryIndex < trackCount; entryIndex = entryIndex + 1)
                    {
                        nextEntry = otherEntry + 1;
                        nextObject = (int)nextEntry->object;
                        if (nextObject != 0)
                        {
                            if (*(char*)(nextObject + 0x98) != 1)
                            {
                                swapWithNeighbour = true;
                                break;
                            }
                            if (*(u8*)(curEntry->object + 0x9d) < *(u8*)(nextObject + 0x9d))
                            {
                                *(TrackEntry_guess*)((char*)self + 0x6c478) = *curEntry;
                                *curEntry = *nextEntry;
                                *nextEntry = *(TrackEntry_guess*)((char*)self + 0x6c478);
                            }
                        }
                        otherEntry = nextEntry;
                    }
                }
            }
            else if (local_10 == 4 && curEntry->object != 0 &&
                (otherEntry = curEntry, *(char*)(curEntry->object + 0x98) == 1))
            {
                for (; entryIndex < trackCount; entryIndex = entryIndex + 1)
                {
                    nextEntry = otherEntry + 1;
                    nextObject = (int)nextEntry->object;
                    if (nextObject != 0)
                    {
                        if (*(char*)(nextObject + 0x98) != 1)
                        {
                            swapWithNeighbour = true;
                            break;
                        }
                        if (*(u8*)(nextObject + 0x9d) < *(u8*)(curEntry->object + 0x9d))
                        {
                            *(TrackEntry_guess*)((char*)self + 0x6c478) = *curEntry;
                            *curEntry = *nextEntry;
                            *nextEntry = *(TrackEntry_guess*)((char*)self + 0x6c478);
                        }
                    }
                    otherEntry = nextEntry;
                }
            }
        }
        if (swapWithNeighbour)
        {
            *(TrackEntry_guess*)((char*)self + 0x6c478) = *curEntry;
            otherEntry = (TrackEntry_guess*)((char*)self + entryIndex * 0x10 + 0x685f8);
            *curEntry = *otherEntry;
            *otherEntry = *(TrackEntry_guess*)((char*)self + 0x6c478);
        }
        curEntry = curEntry + 1;
    }
}

// FUNCTION: TRILOGY 0x0046aa06
// Classifies each tracked spawn by level against the player and marks which ones the track list shows.
void __fastcall DoTrackFilter(void* self)
{
    int remaining;
    u32* state;

    state = (u32*)((char*)self + 0x68604);
    remaining = 1000;
    do
    {
        int bVar1;
        int trueFlag;
        int filterMatch;
        u32 level;
        int levelDelta;
        u32 targetLevel;
        EQPlayer* player;

        filterMatch = 0;
        if (state[-3] != 0)
        {
            *state = 0;
            level = 1;
            if (g_pc_guess != 0)
                level = (u32)g_pc_guess->level;
            targetLevel = (u32)*(u8*)(state[-3] + 0x9d);
            levelDelta = (int)(targetLevel - level);
            if ((targetLevel < (level * 3) / 4) && (levelDelta < -3))
                *state = 0x100;
            else if (levelDelta < 0)
                *state = 0x80;
            else if (targetLevel == level)
                *state = 0x40;
            else
                *state = (((2 < levelDelta) - 1) & 0x10) + 0x10;

            if ((g_pc_guess != 0) && (g_pc_guess->class_ == 4) &&
                (*(char*)(state[-3] + 0x98) == 1) &&
                ((((EQ_GLOBAL(u8, 0x006B6760) & 1) != 0) && ((*state & 0x100) != 0)) ||
                 (((EQ_GLOBAL(u8, 0x006B6760) & 2) != 0) && ((*state & 0x80) != 0)) ||
                 (((EQ_GLOBAL(u8, 0x006B6760) & 4) != 0) && ((*state & 0x40) != 0)) ||
                 (((EQ_GLOBAL(u8, 0x006B6760) & 8) != 0) && ((*state & 0x20) != 0)) ||
                 (((EQ_GLOBAL(u8, 0x006B6760) & 0x10) != 0) && ((*state & 0x10) != 0))))
                filterMatch = 1;

            trueFlag = 1;
            bVar1 = filterMatch;
            if ((*(char*)(state[-3] + 0x98) == 0) &&
                ((*state = *state | 2), bVar1 = trueFlag, EQ_GLOBAL(u32, 0x006B6758) != 0))
            {
                if (EQ_GLOBAL(u32, 0x006B6758) == 1)
                    bVar1 = 0;
                else if (EQ_GLOBAL(u32, 0x006B6758) != 2)
                    bVar1 = filterMatch;
                else if (EQ_GLOBAL(u8, 0x005FE968) != 0)
                {
                    u32* filterPlayer;
                    char* filterEnabled;

                    filterPlayer = g_death_state_guess;
                    filterEnabled = EQ_ARRAY(char, 0x005FE8BD);
                    do
                    {
                        if (((*filterEnabled != 0) && (*filterPlayer != 0)) &&
                            ((bVar1 = filterMatch), (u32)state[-3] == *filterPlayer))
                            break;
                        filterEnabled += 0x1e;
                        filterPlayer++;
                        bVar1 = trueFlag;
                    } while ((int)filterEnabled < 0x005FE953);
                }
            }

            if ((*(int*)(state[-3] + 0x84) != 0) &&
                ((player = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int), (EQPlayer*)state[-3])(*(int*)(state[-3] + 0x84))) != 0) &&
                (player->type != 1))
                *state = *state | 4;
            if (bVar1)
                *state = *state | 1;
        }
        state += 4;
        remaining--;
    } while (remaining != 0);
}

// FUNCTION: TRILOGY 0x0046AB71
// @early-stop: alcohol path keeps the constant 1 in EBX (VC6 CSE), so the item pointer spills (frame 0x144, not 0x140); eat/drink send+join tails merge
// Callers set ECX, but the body never reads it; it pops its three args (RET 0xC).
void __stdcall RightClickItem_guess(int itemArg, int castArg, int slotArg)
{
    struct RightClickItem_msg12_guess
    {
        int slot;
        int unknown0x04;
        int unknown0x08;
    } consumeItemMsg;
    struct RightClickItem_msg16_guess
    {
        int slot;
        int unknown0x04;
        int unknown0x08;
        int type;               // 1 = food, 2 = drink
    } foodDrinkMsg;
    int item;
    char itemType;                      // also holds charges, then effect_type
    int slot;
    char message[256];
    char itemName[36];

    item = itemArg;
    strcpy(itemName, (char*)item);
    itemType = *(char*)(item + 0xc2);
    if (itemType == '*')
    {
        EQ_METHOD(1, void, 0x004DBD8B, (void*, u32), g_everquest_guess)(*(u32*)(item + 0xa0));
        return;
    }
    if (itemType == '\x0e')
    {
        if (g_pc_guess->hunger >= 6000)         // 6000 = full
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B870), 0x111);
            return;
        }
        if ((itemName[0] == 'a') || (itemName[0] == 'A'))
        {
            sprintf(message, EQ_ARRAY(char, 0x0055B830), item);
        }
        else
        {
            sprintf(message, EQ_ARRAY(char, 0x0055B850), item);
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
        g_pc_guess->hunger += (*(int*)(item + 0xe8) * 100) / 2;
        if (6000 < g_pc_guess->hunger)
            g_pc_guess->hunger = 6000;
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B818), 0x111);
        slot = (int)(short)slotArg;
        foodDrinkMsg.unknown0x08 = -1;
        foodDrinkMsg.slot = slot;
        foodDrinkMsg.unknown0x04 = 999;
        foodDrinkMsg.type = 1;
        send_message_guess(g_world_connection_guess, 0x2156, &foodDrinkMsg, 0x10, 1);   // OP_ConsumeFoodDrink
        itemType = *(char*)(item + 0xda);
        if (itemType == '\x01')
        {
            operator delete(g_pc_guess->inventory_items[slot]);
            g_pc_guess->inventory_items[slot] = 0;
            return;
        }
        *(char*)(item + 0xda) = itemType - 1;
        return;
    }
    else if (itemType == '\x0f')
    {
        if (g_pc_guess->thirst >= 6000)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B7DC), 0x111);
            return;
        }
        if ((itemName[0] == 'a') || (itemName[0] == 'A'))
        {
            sprintf(message, EQ_ARRAY(char, 0x0055B7A0), item);
        }
        else
        {
            sprintf(message, EQ_ARRAY(char, 0x0055B7C0), item);
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
        g_pc_guess->thirst += (*(int*)(item + 0xe8) * 100) / 2;
        if (6000 < g_pc_guess->thirst)
            g_pc_guess->thirst = 6000;
        if (g_sound_manager_ptr_guess != 0)
            EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x95, 0);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B784), 0x111);
        slot = (int)(short)slotArg;
        foodDrinkMsg.unknown0x08 = -1;
        foodDrinkMsg.slot = slot;
        foodDrinkMsg.unknown0x04 = 999;
        foodDrinkMsg.type = 2;
        send_message_guess(g_world_connection_guess, 0x2156, &foodDrinkMsg, 0x10, 1);
        itemType = *(char*)(item + 0xda);
        if (itemType == '\x01')
        {
            operator delete(g_pc_guess->inventory_items[slot]);
            g_pc_guess->inventory_items[slot] = 0;
            return;
        }
        *(char*)(item + 0xda) = itemType - 1;
        return;
    }
    else if (itemType == '&')
    {
        if (g_pc_guess->intoxication >= 200)    // 200 = max intoxication
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B73C), 0x111);
            return;
        }
        sprintf(message, EQ_ARRAY(char, 0x0055B710), item);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(message, 0x111);
        itemArg = 0x28;                         // itemArg doubles as the intoxication gain
        if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)(0x42) != '\0')
        {
            EQ_METHOD(2, void, 0x004501A1, (void*, int, float), g_pc_guess)(0x42, 1.0f);
            slot = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 100);       // slot doubles as the roll
            if ((slot <= (int)g_pc_guess->skills[0x42]) && (slot < 0x60))
                itemArg = (slot * 0x28) / (int)g_pc_guess->skills[0x42];
        }
        g_pc_guess->intoxication += (short)itemArg;
        if (200 < g_pc_guess->intoxication)
            g_pc_guess->intoxication = 200;
        slot = (int)(short)slotArg;
        consumeItemMsg.unknown0x04 = -1;
        consumeItemMsg.unknown0x08 = -1;
        consumeItemMsg.slot = slot;
        send_message_guess(g_world_connection_guess, 0x2146, &consumeItemMsg, 0xc, 1);   // OP_ConsumeItem
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B784), 0x111);
        itemType = *(char*)(item + 0xda);
        if (itemType == '\x01')
        {
            operator delete(g_pc_guess->inventory_items[slot]);
            g_pc_guess->inventory_items[slot] = 0;
            return;
        }
        *(char*)(item + 0xda) = itemType - 1;
        return;
    }
    else
    {
        if (*(u8*)(item + 0xd8) < 2)
            return;
        itemType = *(char*)(item + 0xdb);
        if (itemType == '\0')
            return;
        if ((g_pc_guess->class_ == 8) &&
            ((itemType == '\x02') || (itemType == '\x04') || (itemType == '\x01')) &&
            (g_local_player_guess->actor_info->unknown0x248 != -1))
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055B6E4), 0x108);
            return;
        }
        if (itemType == '\x02')
            return;
        if (g_local_player_guess->actor_info->unknown0x248 != -1)
            return;
        EQ_METHOD(4, void, 0x0041F381, (void*, int, int, int*, int), g_pc_guess)(10, 0, &castArg, slotArg);
        if (*(char*)(item + 0xc2) != 0x15)
            return;
        if (g_sound_manager_ptr_guess == 0)
            return;
        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x95, 0);
    }
}

// FUNCTION: TRILOGY 0x0046af87
// Draws a black outline around an on-screen rectangle.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void EQ_Main::drawRectangle(int* rect)
{
    int left;
    int top;
    int right;
    int bottom;
    float vertices[6];
    t3dDeferLine_guess_t drawLine;

    top = rect[1];
    bottom = rect[3];
    if (top != bottom)
    {
        right = rect[2];
        left = rect[0];
        if (right != left && top >= 0 && left >= 0 && bottom >= 0 && right >= 0)
        {
            drawLine = EQ_GLOBAL(t3dDeferLine_guess_t, 0x006E51B8);
            vertices[0] = (float)left;
            vertices[1] = (float)top;
            vertices[3] = (float)right;
            vertices[4] = (float)top;
            drawLine(vertices, 0xff000000u);

            vertices[0] = (float)rect[2];
            vertices[1] = (float)rect[1];
            vertices[3] = (float)rect[2];
            vertices[4] = (float)rect[3];
            drawLine(vertices, 0xff000000u);

            vertices[0] = (float)rect[0];
            vertices[1] = (float)rect[3];
            vertices[3] = (float)rect[2];
            vertices[4] = (float)rect[3];
            drawLine(vertices, 0xff000000u);

            vertices[0] = (float)rect[0];
            vertices[1] = (float)rect[1];
            vertices[3] = (float)rect[0];
            vertices[4] = (float)rect[3];
            drawLine(vertices, 0xff000000u);
        }
    }
}

// FUNCTION: TRILOGY 0x0046b067
// Calls 0x00450168(1, 0) on the object, then its fourth virtual method (EQ_OverlaySlots_guess ctor).
void __fastcall FUN_0046b067_guess(int* obj, int unusedEdx,
                                  int unusedArg1, int unusedArg2)
{
    class OverlayCallbackVtable_guess
    {
    public:
        virtual void unused0_guess(void) {}
        virtual void unused1_guess(void) {}
        virtual void unused2_guess(void) {}
        virtual void invoke_guess(void) {}
    };

    EQ_METHOD(2, void, 0x00450168, (int*, int, int), obj)(1, 0);
    ((OverlayCallbackVtable_guess*)obj)->invoke_guess();
    return;
}

/* Element lists: objects whose dword +4 is the list handed to RegisterOverlayElement (0x004502A1). */
#define OVL_LIST_4E14_guess ((char*)EQ_GLOBAL(int, 0x006E4E14))
#define OVL_LIST_4E18_guess ((char*)EQ_GLOBAL(int, 0x006E4E18))
#define OVL_LIST_4E24_guess ((char*)EQ_GLOBAL(int, 0x006E4E24))
#define OVL_LIST_4E28_guess ((char*)EQ_GLOBAL(int, 0x006E4E28))
#define OVL_LIST_4E2C_guess ((char*)EQ_GLOBAL(int, 0x006E4E2C))
#define OVL_LIST_4E30_guess ((char*)EQ_GLOBAL(int, 0x006E4E30))
#define OVL_LIST_4E34_guess ((char*)EQ_GLOBAL(int, 0x006E4E34))
#define OVL_LIST_4E3C_guess ((char*)EQ_GLOBAL(int, 0x006E4E3C))
#define OVL_LIST_4E40_guess ((char*)EQ_GLOBAL(int, 0x006E4E40))
#define OVL_LIST_4E48_guess ((char*)EQ_GLOBAL(int, 0x006E4E48))
#define OVL_LIST_4E4C_guess ((char*)EQ_GLOBAL(int, 0x006E4E4C))
#define OVL_LIST_4E50_guess ((char*)EQ_GLOBAL(int, 0x006E4E50))
#define OVL_LIST_4E54_guess ((char*)EQ_GLOBAL(int, 0x006E4E54))

/* SetRect into `rect`, then RegisterOverlayElement(id, list->+4, &rect) on `self`. */
#define OVL_REGISTER_guess(id, list, left, top, right, bottom) \
    setRect(&rect, left, top, right, bottom); \
    EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(id, *(void**)((list) + 4), &rect)
#define OVL_REGISTER_SELF_guess(id, left, top, right, bottom) \
    setRect(&rect, left, top, right, bottom); \
    EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(id, *(void**)((char*)self + 4), &rect)
#define OVL_CALC_RECT_guess(id) \
    EQ_METHOD(2, void, 0x004504BF, (void*, int, RECT*), self)(id, &rect)

// FUNCTION: TRILOGY 0x0046b07e
// Sets up the rectangle of every old-UI element id on its overlay list, then clears two 15x400 tables
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (this in ECX). Registers every old-UI overlay element rectangle (SetRect via the pointer at 0x00534248,
// then 0x004502A1 with the list at +4 of one of the 0x006E4Exx objects, or self+4), then zeroes two 15 x 400 dword
// tables at self+0x8400c and self+0x8400c+0x5dc0.
void __fastcall DefineImages_guess(void* self)
{
    tSetRect_guess_t setRect;
    int tmp;
    int acc;
    int local_18;
    int local_14;
    RECT rect;
    unsigned int* tablePtr;

    setRect = g_setrect_guess;
    tmp = 0;
    OVL_REGISTER_SELF_guess(0x5a, 0, 0, 0x28, 0x28);
    do
    {
        OVL_CALC_RECT_guess(tmp);
        EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(tmp + 500, *(void**)(OVL_LIST_4E48_guess + 4), &rect);
        tmp = tmp + 1;
    } while ((short)tmp < 0xc0);
    tmp = 0;
    do
    {
        OVL_CALC_RECT_guess(tmp);
        EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(tmp + 0x2b4, *(void**)(OVL_LIST_4E4C_guess + 4), &rect);
        tmp = tmp + 1;
    } while ((short)tmp < 0xc0);
    tmp = 0;
    do
    {
        OVL_CALC_RECT_guess(tmp);
        EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(tmp + 0x374, *(void**)(OVL_LIST_4E50_guess + 4), &rect);
        tmp = tmp + 1;
    } while ((short)tmp < 0xc0);
    tmp = 0;
    do
    {
        OVL_CALC_RECT_guess(tmp);
        EQ_METHOD(3, void, 0x004502A1, (void*, int, void*, RECT*), self)(tmp + 0x434, *(void**)(OVL_LIST_4E54_guess + 4), &rect);
        tmp = tmp + 1;
    } while ((short)tmp < 0xc0);
    acc = 0;
    tmp = 0x1f;
    do
    {
        OVL_REGISTER_guess(acc + 0x7d1, OVL_LIST_4E18_guess, tmp, 0, tmp + 0x1f, 0x17);
        acc = acc + 1;
        tmp = tmp + 0x1f;
    } while ((short)acc < 4);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x7d5, OVL_LIST_4E18_guess, acc, 0xb8, acc + 0x1f, 0xcf);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x7e0, OVL_LIST_4E18_guess, acc, 0xcf, acc + 0x1f, 0xe6);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x7eb, OVL_LIST_4E18_guess, acc, 0xe6, acc + 0x1f, 0xfd);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x7f6, OVL_LIST_4E18_guess, acc, 0xfd, acc + 0x1f, 0x114);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x801, OVL_LIST_4E18_guess, acc, 0x114, acc + 0x1f, 299);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x80c, OVL_LIST_4E18_guess, acc, 299, acc + 0x1f, 0x142);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x817, OVL_LIST_4E18_guess, acc, 0x142, acc + 0x1f, 0x159);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x822, OVL_LIST_4E18_guess, acc, 0x159, acc + 0x1f, 0x170);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x82d, OVL_LIST_4E18_guess, acc, 0x170, acc + 0x1f, 0x187);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x838, OVL_LIST_4E18_guess, acc, 0x187, acc + 0x1f, 0x19e);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x843, OVL_LIST_4E18_guess, acc, 0x19e, acc + 0x1f, 0x1b5);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x84e, OVL_LIST_4E18_guess, acc, 0x1b5, acc + 0x1f, 0x1cc);
        tmp = tmp + 1;
        acc = acc + 0x1f;
    } while ((short)tmp < 0xb);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x9c4, OVL_LIST_4E14_guess, acc, 0, acc + 0x28, 0x28);
        tmp = tmp + 1;
        acc = acc + 0x28;
    } while ((short)tmp < 5);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x9c9, OVL_LIST_4E14_guess, acc, 0x28, acc + 0x28, 0x50);
        tmp = tmp + 1;
        acc = acc + 0x28;
    } while ((short)tmp < 5);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x9ce, OVL_LIST_4E14_guess, acc, 0x50, acc + 0x28, 0x78);
        tmp = tmp + 1;
        acc = acc + 0x28;
    } while ((short)tmp < 5);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x9d3, OVL_LIST_4E14_guess, acc, 0x78, acc + 0x28, 0xa0);
        tmp = tmp + 1;
        acc = acc + 0x28;
    } while ((short)tmp < 4);
    tmp = 0;
    acc = 0;
    do
    {
        OVL_REGISTER_guess(tmp + 0x9d7, OVL_LIST_4E14_guess, acc, 0xa0, acc + 0x28, 200);
        tmp = tmp + 1;
        acc = acc + 0x28;
    } while ((short)tmp < 4);
    OVL_REGISTER_guess(2000, OVL_LIST_4E18_guess, 0xba, 0, 0xd9, 0x17);
    OVL_REGISTER_guess(0xd1, OVL_LIST_4E40_guess, 0x134, 0x1c0, 0x199, 0x1d2);
    OVL_REGISTER_guess(0xd2, OVL_LIST_4E40_guess, 0x19a, 0x1c0, 0x1ff, 0x1d2);
    OVL_REGISTER_guess(0xd3, OVL_LIST_4E40_guess, 0x209, 10, 0x26d, 0x1b);
    OVL_REGISTER_guess(0xd4, OVL_LIST_4E40_guess, 1, 0x1ba, 0x66, 0x1cc);
    OVL_REGISTER_guess(0xd5, OVL_LIST_4E3C_guess, 0x158, 0x191, 0x19b, 0x196);
    OVL_REGISTER_guess(0xd6, OVL_LIST_4E3C_guess, 0x159, 0x74, 0x188, 0x86);
    OVL_REGISTER_guess(0xd7, OVL_LIST_4E3C_guess, 0x158, 0x191, 0x19b, 0x196);
    OVL_REGISTER_guess(0xd8, OVL_LIST_4E3C_guess, 0x159, 0x132, 0x1be, 0x144);
    OVL_REGISTER_guess(0xd9, OVL_LIST_4E3C_guess, 0x159, 0x145, 0x1be, 0x157);
    OVL_REGISTER_guess(0xda, OVL_LIST_4E40_guess, 0, 0, 0x71, 0x9e);
    OVL_REGISTER_guess(0xdb, OVL_LIST_4E40_guess, 0, 0x9f, 0x71, 0x13d);
    OVL_REGISTER_SELF_guess(0xdc, 0x211, 0x142, 0x280, 0x1e0);
    OVL_REGISTER_guess(0xdd, OVL_LIST_4E40_guess, 0xe1, 0x56, 0x146, 0x68);
    OVL_REGISTER_SELF_guess(0xde, 0x212, 0xfd, 0x277, 0x10f);
    OVL_REGISTER_SELF_guess(0xdf, 0x212, 0x127, 0x277, 0x139);
    OVL_REGISTER_guess(0xe0, OVL_LIST_4E3C_guess, 0x159, 0x158, 0x1bd, 0x16a);
    OVL_REGISTER_guess(0xe1, OVL_LIST_4E40_guess, 0x126, 0x7d, 0x131, 0x8f);
    OVL_REGISTER_guess(0xe2, OVL_LIST_4E40_guess, 0x131, 0x7d, 0x13c, 0x8f);
    OVL_REGISTER_guess(0xe3, OVL_LIST_4E40_guess, 0x130, 0x69, 0x13a, 0x75);
    OVL_REGISTER_SELF_guess(0xe4, 0x212, 0x112, 0x277, 0x124);
    OVL_REGISTER_guess(0xe5, OVL_LIST_4E40_guess, 0x209, 0x1d, 0x26e, 0x2f);
    OVL_REGISTER_guess(0xe6, OVL_LIST_4E40_guess, 0, 0, 0x6f, 0x96);
    OVL_REGISTER_guess(0xe7, OVL_LIST_4E40_guess, 0x70, 0, 0xdf, 0x9c);
    OVL_REGISTER_guess(0xe8, OVL_LIST_4E40_guess, 0x73, 0x13, 0xb8, 0x25);
    OVL_REGISTER_guess(0xe9, OVL_LIST_4E40_guess, 0x73, 0, 0xb8, 0x12);
    OVL_REGISTER_guess(0xea, OVL_LIST_4E40_guess, 0x73, 0x26, 0xb8, 0x38);
    OVL_REGISTER_guess(0xeb, OVL_LIST_4E40_guess, 0x73, 0x39, 0xb8, 0x4b);
    OVL_REGISTER_guess(0xec, OVL_LIST_4E40_guess, 0xe1, 10, 0x126, 0x1c);
    OVL_REGISTER_guess(0xed, OVL_LIST_4E40_guess, 0xe1, 0x1d, 0x146, 0x2f);
    OVL_REGISTER_guess(0xee, OVL_LIST_4E40_guess, 0xe1, 0x30, 0x146, 0x42);
    OVL_REGISTER_guess(0xef, OVL_LIST_4E40_guess, 0xe1, 0x43, 0x146, 0x55);
    OVL_REGISTER_guess(0xf0, OVL_LIST_4E40_guess, 0xe1, 0x7c, 0x110, 0x8e);
    OVL_REGISTER_guess(0xf1, OVL_LIST_4E3C_guess, 0x159, 0x16b, 0x1be, 0x17d);
    OVL_REGISTER_guess(0xf2, OVL_LIST_4E40_guess, 0x14d, 0x1d, 0x1b2, 0x2f);
    OVL_REGISTER_guess(0xf3, OVL_LIST_4E40_guess, 0x14d, 0x30, 0x1b2, 0x42);
    OVL_REGISTER_guess(0xf4, OVL_LIST_4E40_guess, 0x14d, 0x43, 0x1b2, 0x55);
    OVL_REGISTER_guess(0xf5, OVL_LIST_4E40_guess, 0x14d, 0x56, 0x1b2, 0x68);
    OVL_REGISTER_guess(0xf6, OVL_LIST_4E40_guess, 0x14d, 0x69, 0x1b2, 0x7b);
    OVL_REGISTER_guess(0xf7, OVL_LIST_4E40_guess, 0x14d, 0x7c, 0x1b2, 0x8e);
    OVL_REGISTER_guess(0xf8, OVL_LIST_4E40_guess, 8, 0xa7, 0x31, 0xd0);
    OVL_REGISTER_guess(0xf9, OVL_LIST_4E40_guess, 0x39, 0xa7, 0x61, 0xd0);
    OVL_REGISTER_guess(0xfa, OVL_LIST_4E40_guess, 8, 0xd8, 0x31, 0x101);
    OVL_REGISTER_guess(0xfb, OVL_LIST_4E40_guess, 0x39, 0xd8, 0x61, 0x101);
    OVL_REGISTER_guess(0xfc, OVL_LIST_4E40_guess, 8, 0x109, 0x30, 0x132);
    OVL_REGISTER_guess(0xfd, OVL_LIST_4E40_guess, 0x39, 0x109, 0x61, 0x132);
    OVL_REGISTER_guess(0xfe, OVL_LIST_4E40_guess, 0x1b8, 0x1d, 0x1e1, 0x46);
    OVL_REGISTER_guess(0xff, OVL_LIST_4E40_guess, 0x1e0, 0x1d, 0x209, 0x46);
    OVL_REGISTER_guess(0x100, OVL_LIST_4E40_guess, 0x1b8, 0x45, 0x1e1, 0x6e);
    OVL_REGISTER_guess(0x101, OVL_LIST_4E40_guess, 0x1e0, 0x45, 0x209, 0x6e);
    OVL_REGISTER_guess(0x102, OVL_LIST_4E40_guess, 0x1b8, 0x6d, 0x1e1, 0x96);
    OVL_REGISTER_guess(0x103, OVL_LIST_4E40_guess, 0x1e0, 0x6d, 0x209, 0x96);
    OVL_REGISTER_guess(0x104, OVL_LIST_4E40_guess, 0xf1, 0x9f, 0x133, 0xe1);
    OVL_REGISTER_guess(0x105, OVL_LIST_4E3C_guess, 0, 0, 0x77, 0x1e0);
    OVL_REGISTER_guess(0x106, OVL_LIST_4E3C_guess, 0x77, 0, 0xf0, 0x1e0);
    OVL_REGISTER_guess(0x107, OVL_LIST_4E3C_guess, 0xf0, 0, 0x158, 0x1b7);
    OVL_REGISTER_guess(0x108, OVL_LIST_4E3C_guess, 0x159, 0x39, 0x1a3, 0x73);
    OVL_REGISTER_guess(0x109, OVL_LIST_4E3C_guess, 0x159, 0, 0x189, 0x12);
    OVL_REGISTER_guess(0x10a, OVL_LIST_4E3C_guess, 0x189, 0, 0x1b9, 0x12);
    OVL_REGISTER_guess(0x10b, OVL_LIST_4E3C_guess, 0x159, 0x13, 0x1a5, 0x25);
    OVL_REGISTER_guess(0x10c, OVL_LIST_4E3C_guess, 0x159, 0x26, 0x1a5, 0x38);
    OVL_REGISTER_guess(0x10d, OVL_LIST_4E3C_guess, 0x1b8, 0x58, 0x1f5, 0x6d);
    OVL_REGISTER_guess(0x10e, OVL_LIST_4E3C_guess, 0x1b8, 0x42, 0x1f5, 0x57);
    OVL_REGISTER_guess(0x10f, OVL_LIST_4E3C_guess, 0x1b8, 0x9a, 0x1f5, 0xaf);
    OVL_REGISTER_guess(0x110, OVL_LIST_4E3C_guess, 0x1b8, 0x84, 0x1f5, 0x99);
    OVL_REGISTER_guess(0x111, OVL_LIST_4E3C_guess, 0x1b8, 0xdc, 0x1f5, 0xf1);
    OVL_REGISTER_guess(0x112, OVL_LIST_4E3C_guess, 0x1b8, 0xb0, 0x1f5, 0xc5);
    OVL_REGISTER_guess(0x113, OVL_LIST_4E3C_guess, 0x1b8, 0xb0, 0x1f5, 0xc5);
    OVL_REGISTER_guess(0x114, OVL_LIST_4E3C_guess, 0x1b8, 0x2c, 0x1f5, 0x41);
    OVL_REGISTER_guess(0x115, OVL_LIST_4E3C_guess, 0x1b8, 0x6e, 0x1f5, 0x83);
    OVL_REGISTER_guess(0x116, OVL_LIST_4E3C_guess, 0x1b8, 0x16, 0x1f5, 0x2b);
    OVL_REGISTER_guess(0x117, OVL_LIST_4E3C_guess, 0x1b8, 0, 0x1f5, 0x15);
    OVL_REGISTER_guess(0x118, OVL_LIST_4E40_guess, 0x66, 0x163, 0x85, 0x1c7);
    OVL_REGISTER_guess(0x119, OVL_LIST_4E3C_guess, 0x1f6, 0, 0x233, 0x15);
    OVL_REGISTER_guess(0x11a, OVL_LIST_4E3C_guess, 0x1f6, 0x16, 0x233, 0x2b);
    OVL_REGISTER_guess(0x11b, OVL_LIST_4E3C_guess, 0x1f6, 0x2c, 0x233, 0x41);
    OVL_REGISTER_guess(0x11c, OVL_LIST_4E3C_guess, 0x1f6, 0x42, 0x233, 0x57);
    OVL_REGISTER_guess(0x11d, OVL_LIST_4E3C_guess, 0x1f6, 0x58, 0x233, 0x6d);
    OVL_REGISTER_guess(0x11e, OVL_LIST_4E3C_guess, 0x1f6, 0x6e, 0x233, 0x83);
    OVL_REGISTER_guess(0x11f, OVL_LIST_4E3C_guess, 0x1f6, 0x84, 0x233, 0x99);
    OVL_REGISTER_guess(0x120, OVL_LIST_4E3C_guess, 0x1f6, 0x9a, 0x233, 0xaf);
    OVL_REGISTER_guess(0x121, OVL_LIST_4E3C_guess, 0x1f6, 0xb0, 0x233, 0xc5);
    OVL_REGISTER_guess(0x122, OVL_LIST_4E3C_guess, 0x1f6, 0xc6, 0x233, 0xdb);
    OVL_REGISTER_guess(0x123, OVL_LIST_4E3C_guess, 0x1f6, 0xdc, 0x233, 0xf1);
    OVL_REGISTER_guess(0x124, OVL_LIST_4E3C_guess, 0x1f6, 0xf2, 0x233, 0x107);
    OVL_REGISTER_guess(0x125, OVL_LIST_4E3C_guess, 0x1f6, 0x108, 0x233, 0x11d);
    OVL_REGISTER_guess(0x126, OVL_LIST_4E3C_guess, 0x1f6, 0x11e, 0x233, 0x133);
    OVL_REGISTER_guess(0x127, OVL_LIST_4E3C_guess, 0x1f6, 0x134, 0x233, 0x149);
    OVL_REGISTER_guess(0x128, OVL_LIST_4E3C_guess, 0x1f6, 0x14a, 0x233, 0x15f);
    OVL_REGISTER_guess(0x129, OVL_LIST_4E3C_guess, 0x1f6, 0x160, 0x233, 0x175);
    OVL_REGISTER_guess(0x12a, OVL_LIST_4E3C_guess, 0x1f6, 0x176, 0x233, 0x18b);
    OVL_REGISTER_guess(299, OVL_LIST_4E3C_guess, 0x1f6, 0x18c, 0x233, 0x1a1);
    OVL_REGISTER_guess(300, OVL_LIST_4E3C_guess, 0x1f6, 0x1a2, 0x233, 0x1b7);
    OVL_REGISTER_guess(0x12d, OVL_LIST_4E3C_guess, 0x1f6, 0x1b8, 0x233, 0x1cd);
    OVL_REGISTER_guess(0x12e, OVL_LIST_4E3C_guess, 0x234, 0, 0x271, 0x15);
    OVL_REGISTER_guess(0x12f, OVL_LIST_4E3C_guess, 0x234, 0x16, 0x271, 0x2b);
    OVL_REGISTER_guess(0x130, OVL_LIST_4E3C_guess, 0x234, 0x2c, 0x271, 0x41);
    OVL_REGISTER_guess(0x131, OVL_LIST_4E3C_guess, 0x234, 0x42, 0x271, 0x57);
    OVL_REGISTER_guess(0x132, OVL_LIST_4E3C_guess, 0x234, 0x58, 0x271, 0x6d);
    OVL_REGISTER_guess(0x133, OVL_LIST_4E3C_guess, 0x234, 0x6e, 0x271, 0x83);
    OVL_REGISTER_guess(0x134, OVL_LIST_4E3C_guess, 0x234, 0x84, 0x271, 0x99);
    OVL_REGISTER_guess(0x135, OVL_LIST_4E3C_guess, 0x234, 0x9a, 0x271, 0xaf);
    OVL_REGISTER_guess(0x136, OVL_LIST_4E3C_guess, 0x234, 0xb0, 0x271, 0xc5);
    OVL_REGISTER_guess(0x137, OVL_LIST_4E3C_guess, 0x234, 0xc6, 0x271, 0xdb);
    OVL_REGISTER_guess(0x138, OVL_LIST_4E3C_guess, 0x234, 0xdc, 0x271, 0xf1);
    OVL_REGISTER_guess(0x139, OVL_LIST_4E3C_guess, 0x234, 0xf2, 0x271, 0x107);
    OVL_REGISTER_guess(0x13a, OVL_LIST_4E3C_guess, 0x234, 0x108, 0x271, 0x11d);
    OVL_REGISTER_guess(0x13b, OVL_LIST_4E3C_guess, 0x234, 0x11e, 0x271, 0x133);
    OVL_REGISTER_guess(0x13c, OVL_LIST_4E3C_guess, 0x234, 0x134, 0x271, 0x149);
    OVL_REGISTER_guess(0x13d, OVL_LIST_4E3C_guess, 0x234, 0x14a, 0x271, 0x15f);
    OVL_REGISTER_guess(0x13e, OVL_LIST_4E3C_guess, 0x234, 0x160, 0x271, 0x175);
    OVL_REGISTER_guess(3000, OVL_LIST_4E3C_guess, 0x12d, 0x1b8, 0x16a, 0x1cd);
    OVL_REGISTER_guess(0x166, OVL_LIST_4E3C_guess, 0x234, 0x176, 0x271, 0x18b);
    OVL_REGISTER_guess(0x167, OVL_LIST_4E3C_guess, 0x234, 0x18c, 0x271, 0x1a1);
    OVL_REGISTER_guess(0x168, OVL_LIST_4E3C_guess, 0x234, 0x1a2, 0x271, 0x1b7);
    OVL_REGISTER_guess(0x169, OVL_LIST_4E3C_guess, 0x234, 0x1b8, 0x271, 0x1cd);
    OVL_REGISTER_guess(0x16a, OVL_LIST_4E3C_guess, 0x1b8, 0x1a2, 0x1f5, 0x1b7);
    OVL_REGISTER_guess(0x16b, OVL_LIST_4E3C_guess, 0x1b8, 0x1b8, 0x1f5, 0x1cd);
    OVL_REGISTER_guess(0x16c, OVL_LIST_4E34_guess, 0x217, 0x39, 0x256, 0x50);
    OVL_REGISTER_guess(0x16d, OVL_LIST_4E34_guess, 0x217, 0x21, 0x256, 0x38);
    OVL_REGISTER_guess(0x17d, OVL_LIST_4E40_guess, 0x15e, 0x9a, 0x171, 0xa6);
    OVL_REGISTER_guess(0x17e, OVL_LIST_4E40_guess, 0x14c, 0x9a, 0x15f, 0xa6);
    OVL_REGISTER_guess(0x17f, OVL_LIST_4E2C_guess, 400, 0, 0x207, 0x1e0);
    OVL_REGISTER_guess(0x180, OVL_LIST_4E2C_guess, 0x207, 0, 0x280, 0x1e0);
    OVL_REGISTER_guess(0x181, OVL_LIST_4E2C_guess, 0x126, 0x104, 0x16f, 0x13e);
    OVL_REGISTER_guess(0x182, OVL_LIST_4E2C_guess, 0x126, 0x13f, 0x16f, 0x179);
    OVL_REGISTER_guess(0x183, OVL_LIST_4E2C_guess, 0x125, 0x188, 0x145, 0x19e);
    OVL_REGISTER_guess(0x184, OVL_LIST_4E2C_guess, 0x125, 0x19e, 0x145, 0x1b4);
    OVL_REGISTER_guess(0x185, OVL_LIST_4E2C_guess, 0x125, 0x1b4, 0x145, 0x1ca);
    OVL_REGISTER_guess(0x186, OVL_LIST_4E2C_guess, 0x125, 0x1ca, 0x145, 0x1e0);
    OVL_REGISTER_guess(0x187, OVL_LIST_4E2C_guess, 0x33, 0x103, 0x98, 0x115);
    OVL_REGISTER_guess(0x188, OVL_LIST_4E30_guess, 400, 0, 0x280, 0x1e0);
    OVL_REGISTER_guess(0x189, OVL_LIST_4E30_guess, 0x168, 0x15d, 400, 0x185);
    OVL_REGISTER_guess(0x18a, OVL_LIST_4E30_guess, 0x154, 0x140, 400, 0x15c);
    OVL_REGISTER_guess(0x18b, OVL_LIST_4E40_guess, 0, 0x13e, 0x79, 0x177);
    OVL_REGISTER_guess(0x18c, OVL_LIST_4E40_guess, 0, 0x177, 0x79, 0x1b0);
    OVL_REGISTER_guess(0x18d, OVL_LIST_4E34_guess, 0, 0, 0x19f, 0xa0);
    OVL_REGISTER_guess(0x18e, OVL_LIST_4E40_guess, 0x15e, 0x90, 0x171, 0x9a);
    OVL_REGISTER_guess(399, OVL_LIST_4E40_guess, 0x14c, 0x90, 0x15f, 0x9a);
    local_18 = 0;
    local_14 = 0x58;
    do
    {
        OVL_REGISTER_guess(local_18 + 0x145, OVL_LIST_4E40_guess, 0x223, local_14, 0x24a, local_14 + 0x27);
        local_18 = local_18 + 1;
        local_14 = local_14 + 0x28;
    } while ((short)local_18 < 6);
    local_18 = 0;
    local_14 = 0x58;
    do
    {
        OVL_REGISTER_guess(local_18 + 0x14b, OVL_LIST_4E40_guess, 0x24b, local_14, 0x273, local_14 + 0x27);
        local_18 = local_18 + 1;
        local_14 = local_14 + 0x28;
    } while ((short)local_18 < 6);
    OVL_REGISTER_guess(0x151, OVL_LIST_4E3C_guess, 0x160, 0x8a, 0x1a0, 0x92);
    OVL_REGISTER_guess(0x152, OVL_LIST_4E3C_guess, 0x159, 0x95, 0x16c, 0xa4);
    OVL_REGISTER_guess(0x153, OVL_LIST_4E40_guess, 0x18e, 0x8f, 0x1ac, 0xa6);
    OVL_REGISTER_guess(0x154, OVL_LIST_4E34_guess, 0x19f, 0, 0x216, 0x1e0);
    OVL_REGISTER_guess(0x155, OVL_LIST_4E34_guess, 0x217, 0, 0x23d, 0x20);
    OVL_REGISTER_guess(0x156, OVL_LIST_4E34_guess, 0x23e, 0, 0x264, 0x20);
    OVL_REGISTER_guess(0x157, OVL_LIST_4E30_guess, 0, 0, 400, 0x140);
    OVL_REGISTER_guess(0x158, OVL_LIST_4E30_guess, 0xd, 0x143, 0x68, 0x14d);
    OVL_REGISTER_guess(0x159, OVL_LIST_4E30_guess, 0x74, 0x140, 0xaf, 0x158);
    OVL_REGISTER_guess(0x15a, OVL_LIST_4E30_guess, 0xb0, 0x140, 0xcf, 0x160);
    OVL_REGISTER_guess(0x15b, OVL_LIST_4E30_guess, 0xd0, 0x140, 0xef, 0x160);
    OVL_REGISTER_guess(0x15c, OVL_LIST_4E2C_guess, 0, 0, 0x179, 0xfe);
    OVL_REGISTER_guess(0x15d, OVL_LIST_4E2C_guess, 0, 0x116, 0x124, 0x1dd);
    OVL_REGISTER_guess(0x15e, OVL_LIST_4E28_guess, 0, 0, 199, 0x11a);
    OVL_REGISTER_guess(0x15f, OVL_LIST_4E28_guess, 200, 0, 0x13f, 0x11c);
    OVL_REGISTER_guess(0x160, OVL_LIST_4E28_guess, 0x140, 0, 0x1ca, 0x11a);
    OVL_REGISTER_guess(0x161, OVL_LIST_4E28_guess, 0x1cb, 0, 0x280, 0x118);
    OVL_REGISTER_guess(0x162, OVL_LIST_4E28_guess, 0, 0x128, 0x12e, 0x1da);
    OVL_REGISTER_guess(0x163, OVL_LIST_4E28_guess, 0x130, 0x129, 0x260, 0x1de);
    OVL_REGISTER_guess(0x164, OVL_LIST_4E24_guess, 7, 0xfc, 0x79, 0x1e0);
    tmp = 0xe1;
    OVL_REGISTER_guess(0x165, OVL_LIST_4E40_guess, 0xe1, 0x69, 0x10f, 0x7a);
    local_18 = 0x65;
    do
    {
        if ((short)local_18 != 0x69)
        {
            OVL_REGISTER_guess(local_18 + -1, OVL_LIST_4E40_guess, 0xe3, tmp, 0x10b, tmp + 0x28);
            OVL_REGISTER_guess(local_18, OVL_LIST_4E40_guess, 0x10b, tmp, 0x133, tmp + 0x28);
        }
        local_18 = local_18 + 2;
        tmp = tmp + 0x28;
    } while ((short)local_18 < 0x71);
    local_18 = 0x71;
    tmp = 0xa8;
    do
    {
        if ((short)local_18 != 0x7b)
        {
            OVL_REGISTER_guess(local_18 + -1, OVL_LIST_4E40_guess, 0x133, tmp, 0x15b, tmp + 0x28);
            OVL_REGISTER_guess(local_18, OVL_LIST_4E40_guess, 0x15b, tmp, 0x183, tmp + 0x28);
        }
        local_18 = local_18 + 2;
        tmp = tmp + 0x28;
    } while ((short)local_18 < 0x7f);
    local_18 = 0x7f;
    tmp = 0xa8;
    do
    {
        acc = tmp + 0x28;
        OVL_REGISTER_guess(local_18 + -1, OVL_LIST_4E40_guess, 0x183, tmp, 0x1ab, acc);
        OVL_REGISTER_guess(local_18, OVL_LIST_4E40_guess, 0x1ab, tmp, 0x1d3, acc);
        local_18 = local_18 + 2;
        tmp = acc;
    } while ((short)local_18 < 0x8d);
    local_18 = 0x8d;
    tmp = 0xa8;
    do
    {
        acc = tmp + 0x28;
        OVL_REGISTER_guess(local_18 + -1, OVL_LIST_4E40_guess, 0x1d3, tmp, 0x1fb, acc);
        OVL_REGISTER_guess(local_18, OVL_LIST_4E40_guess, 0x1fb, tmp, 0x223, acc);
        local_18 = local_18 + 2;
        tmp = acc;
    } while ((short)local_18 < 0x9b);
    tablePtr = (unsigned int*)((char*)self + 0x8400c);
    tmp = 0xf;
    do
    {
        acc = 400;
        do
        {
            tablePtr[6000] = 0;
            *tablePtr = 0;
            tablePtr = tablePtr + 1;
            acc = acc + -1;
        } while (acc != 0);
        tmp = tmp + -1;
    } while (tmp != 0);
}

// FUNCTION: TRILOGY 0x0046d4da
// Displays text in a framed on-screen popup at the requested position.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: ESI/EDI swapped for screenWidth/screenHeight (VC6 gives ESI to screenHeight in every statement and declaration order tried); call-arg and store scheduling in SetRect-2, SetRect-1 setup and WriteText differs
/* fake match: screenHeight copies local_c so VC6 reloads its register from [ebp-8] after the LUT load; without the copy it spills to a new stack slot */
void __fastcall DisplayAutoHelpText(void* unusedThis, int unusedEdx,
                                         short posX, short param_2,
                                         char* buffer, int textColor)
{
    int screenWidth;
    size_t textLen;
    char* charPtr;
    int tmp;
    int screenHeight;
    RECT destRect;
    int boxLeft;
    int local_18;
    int local_14;
    int local_10;
    int local_c;
    int segmentCount;
    tSetRect_guess_t setRect;

    screenWidth = EQ_GLOBAL(int, 0x006B63FC);
    local_10 = screenWidth;
    local_c = EQ_GLOBAL(int, 0x006B6400);
    screenHeight = local_c;
    short textWidth = 0;
    textLen = strlen(buffer);
    if (0 < (short)textLen)
    {
        tmp = (int)(short)textLen;
        charPtr = buffer;
        do
        {
            if ('\0' < *charPtr)
            {
                textWidth = textWidth + EQ_GLOBAL(short*, 0x006EFDE4)[*charPtr * 2];
            }
            charPtr = charPtr + 1;
            tmp = tmp + -1;
        } while (tmp != 0);
    }
    segmentCount = (int)textWidth / 8;
    if ((short)segmentCount < 0)
    {
        segmentCount = 0;
    }
    if (posX + 0x10 + (short)segmentCount * 8 > screenWidth + -10)
    {
        posX = (short)screenWidth + (short)segmentCount * -8 + -0x1a;
    }
    if (posX < 0)
    {
        posX = 0;
    }
    if (param_2 + 0x10 > screenHeight + -10)
    {
        param_2 = (short)screenHeight + -0x1a;
    }
    setRect = g_setrect_guess;
    *(int*)EQ_GLOBAL(void*, 0x006B341C) =
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E40) + 4);
    setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4),
            0xc6, 0, 0xce, 0x10);
    local_18 = (int)param_2;
    boxLeft = (int)posX;
    setRect(&destRect, boxLeft, local_18, local_10, local_c);
    EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_GLOBAL(void*, 0x006B341C))(&destRect, 1);
    setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4),
            0xc6, 0x10, 0xce, 0x20);
    if (0 < (short)segmentCount)
    {
        tmp = boxLeft + 8;
        local_14 = (int)(short)segmentCount;
        do
        {
            setRect(&destRect, tmp, local_18, local_10, local_c);
            EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_GLOBAL(void*, 0x006B341C))(&destRect, 1);
            tmp = tmp + 8;
            local_14 = local_14 + -1;
        } while (local_14 != 0);
    }
    setRect((RECT*)((char*)EQ_GLOBAL(void*, 0x006B341C) + 4),
            0xc6, 0x20, 0xce, 0x30);
    screenWidth = local_18;
    setRect(&destRect,
            (int)(short)(posX + 8 + (short)segmentCount * 8),
            local_18, local_10, local_c);
    EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), EQ_GLOBAL(void*, 0x006B341C))(&destRect, 1);
    EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(buffer, boxLeft + 5,
         screenWidth + -1, textColor, g_font_guess, 1);
    return;
}

// FUNCTION: TRILOGY 0x0046d69c
// Shows the text (if non-null) as a popup near overlay entry page*400 + base + index.
void __fastcall EQ_Main_DisplayPopupItemName_guess(EQ_Main* state, int unusedEdx,
                                                   char* param_2, int sectionOffset,
                                                   int relativeOffset)
{
    int popupX;
    int popupY;
    int pageBaseIndex;
    int entryIndex;

    if (param_2 != 0)
    {
        pageBaseIndex = state->screenMode_guess * 400 + sectionOffset;
        entryIndex = pageBaseIndex + relativeOffset;
        popupX = *(int*)((char*)state + entryIndex * 0x10 + 0x6c914) + -5;
        popupY = *(int*)((char*)state + (entryIndex + 0x6c91) * 0x10);
        if (popupX > *(int*)((char*)state + pageBaseIndex * 0x10 + 0x6c914) + -5)
        {
            popupY = popupY + 0x17;
        }
        EQ_THISCALL(void, 0x0046D4DA,
                    (void*, int, short, short, char*, int))
            (state, popupX, (short)popupX, (short)popupY, param_2, 10);
    }
    return;
}

// FUNCTION: TRILOGY 0x0046d6fe
// Draws labels: popup name lists in mode 4, else '%d) %s' names of the 8 spell gems; then overlay labels
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (`this` in ECX, EDX unused), no stack arguments, no return value (callers never read EAX).
// Mode 4 draws the popup names (or the alt-name table while a flag is set); other modes draw the spell
// auto-help lines. Then both overlay lists are drawn while the mouse is over an active overlay.
// @early-stop: the zero constant lives in EBX, not EDX (probably because each EQ_THISCALL writes EDX as its dummy argument), so the zero compares, the mode-4 A/B block layout (the original re-tests A and reads B from memory) and the last loop's registers differ.
/* fake match: the gotos give the original's block order, acc carries rectPtr[-1] + 0x23 so ECX stays busy across the idiv, and tmp/acc fold +4/+5 into their loads so VC6 adds early as the original does. */
void __fastcall EQ_Main_ShowAltPopupNames_guess(void* eqMain, int unusedEdx)
{
    char* self;
    int tmp;
    int acc;
    int slot;
    int local_c;
    int local_10;
    int local_14;
    int* rectPtr;
    int local_18;
    int* entryPtr;
    short spellId;
    RECT hitRect;
    char lineBuf[128];
    POINT pt;

    self = (char*)eqMain;
    if (EQ_GLOBAL(int, 0x006B70F0) == 0)
    {
        return;
    }
    if (pinstEQ_Main_guess->isActive_guess == 0)
    {
        return;
    }
    if (*(char*)(self + 0x8fbac) != 0)
    {
        return;
    }
    if (*(char*)(self + 0xfe0) == 4)
    {
        if (*(int*)(self + 0x685e0) == 0)
        {
            if (*(int*)(self + 0x685dc) == 0 && *(int*)(self + 0x685e4) == 0 && *(int*)(self + 0x685e8) == 0)
            {
                goto otherModes;
            }
            tmp = local_18;
            if (*(int*)(self + 0x685dc) != 0)
            {
                local_10 = 0x113;
                goto loadTableOffset;
            }
        }
        else
        {
            local_10 = 0x104;
loadTableOffset:
            tmp = (int)*(char*)(self + 0x6c592) << 1;
        }
        if (*(int*)(self + 0x685e0) == 0 && *(int*)(self + 0x685dc) == 0)
        {
            if (*(int*)(self + 0x685e8) != 0)
            {
                tmp = 0;
                acc = 0xf44;
                do
                {
                    EQ_METHOD(3, void, 0x0046D69C, (void*, char*, int, int), self)(*(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + acc), 0xaa, tmp);
                    acc = acc + 4;
                    tmp = tmp + 1;
                } while (acc < 0xf64);
            }
            else if (*(int*)(self + 0x685e4) != 0)
            {
                slot = 0;
                SetRect(&hitRect, *(int*)(self + 0x7406c), *(int*)(self + 0x74070),
                        *(int*)(self + 0x740e4), *(int*)(self + 0x74138));
                pt.x = EQ_GLOBAL(short, 0x006B6414);
                pt.y = EQ_GLOBAL(short, 0x006B6416);
                if (PtInRect(&hitRect, pt) != 0)
                {
                    slot = 1;
                }
                if (EQ_GLOBAL(char*, 0x006E4D88) != 0 && *(char*)(EQ_GLOBAL(char*, 0x006E4D88) + 0x98) == 1)
                {
                    slot = 1;
                }
                tmp = 0;
                entryPtr = (int*)(self + 0x6c4ec);
                do
                {
                    if (slot != 0)
                    {
                        EQ_METHOD(3, void, 0x0046D69C, (void*, char*, int, int), self)((char*)entryPtr[8], 0x136, tmp);
                    }
                    else
                    {
                        EQ_METHOD(3, void, 0x0046D69C, (void*, char*, int, int), self)((char*)*entryPtr, 300, tmp);
                    }
                    tmp = tmp + 1;
                    entryPtr = entryPtr + 1;
                } while (tmp < 8);
            }
        }
        else
        {
            acc = 0;
            entryPtr = (int*)(self + 0x6c5e0 + tmp * 4);
            do
            {
                EQ_METHOD(3, void, 0x0046D69C, (void*, char*, int, int), self)((char*)*entryPtr, local_10, acc);
                acc = acc + 1;
                entryPtr = entryPtr + 1;
            } while (acc < 0xc);
        }
        if (g_game_state_guess != 2)
        {
            *(int*)(self + 0xeb8) = 1;
        }
    }
    else
    {
otherModes:
        if (*(char*)(self + 0xfe0) <= 3)
        {
            local_c = 0;
            slot = 0x952;
            do
            {
                spellId = *(short*)(EQ_GLOBAL(char*, 0x006E4DA8) + slot);
                if (spellId > 0 && spellId < 3000)
                {
                    acc = *(char*)(self + 0xfe0) * 400 + local_c;
                    tmp = *(int*)(self + (acc + 0x6d65) * 0x10) + 4;
                    acc = *(int*)(self + acc * 0x10 + 0x6d654) + 5;
                    if (EQ_ARRAY(char*, 0x005FE9A0)[spellId] != 0)
                    {
                        sprintf(lineBuf, (const char*)0x0055B8A8, local_c + 1, EQ_ARRAY(char*, 0x005FE9A0)[spellId]);
                        EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, char*, int), self)(acc, tmp, lineBuf, 10);
                    }
                }
                slot = slot + 2;
                local_c = local_c + 1;
            } while (slot < 0x962);
        }
    }
    local_c = 0;
    do
    {
        tmp = EQ_GLOBAL(int, 0x006E4E58);
        if (local_c != 0)
        {
            tmp = EQ_GLOBAL(int, 0x006E4E5C);
        }
        if (tmp != 0 &&
            (acc = EQ_METHOD(0, int, 0x004504AC, (void*), (void*)tmp)(), acc != 0) &&
            (pt.x = EQ_GLOBAL(short, 0x006B6414), pt.y = EQ_GLOBAL(short, 0x006B6416),
             PtInRect((RECT*)(tmp + 0x24), pt) != 0) &&
            ((acc = *(int*)(tmp + 0x174)) != 0 && (slot = 0, *(unsigned char*)(acc + 0xd5) > 0)))
        {
            rectPtr = (int*)(tmp + 0xb4);
            local_10 = 0xac;
            do
            {
                local_18 = *(int*)(local_10 + acc);
                if (local_18 != 0)
                {
                    local_14 = *rectPtr;
                    acc = rectPtr[-1] + 0x23;
                    if (0 < slot % 2)
                    {
                        local_14 = local_14 + 0x17;
                    }
                    EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, char*, int), self)(acc, local_14, (char*)local_18, 10);
                }
                acc = *(int*)(tmp + 0x174);
                slot = slot + 1;
                local_10 = local_10 + 4;
                rectPtr = rectPtr + 4;
            } while (slot < (int)(unsigned int)*(unsigned char*)(acc + 0xd5));
        }
        local_c = local_c + 1;
    } while (local_c < 2);
    return;
}

#define AUTOHELP_STR(va) EQ_ARRAY(char, va)
#ifdef EQD_MATCH
/* The match build declares eqd_va_* only for EQ_ARRAY/EQ_GLOBAL written out in the source: the text table
   goes through AUTOHELP_STR, so declare its addresses here. */
#define AUTOHELP_EXT(va) extern "C" char eqd_va_##va;
AUTOHELP_EXT(0x0055C110) AUTOHELP_EXT(0x0055C134) AUTOHELP_EXT(0x0055C150) AUTOHELP_EXT(0x0055C09C)
AUTOHELP_EXT(0x0055C050) AUTOHELP_EXT(0x0055C084) AUTOHELP_EXT(0x0055C068) AUTOHELP_EXT(0x0055C104)
AUTOHELP_EXT(0x0055C0F4) AUTOHELP_EXT(0x0055C0DC) AUTOHELP_EXT(0x0055C0CC) AUTOHELP_EXT(0x0055C0B8)
AUTOHELP_EXT(0x0055C01C) AUTOHELP_EXT(0x0055C004) AUTOHELP_EXT(0x0055BF90) AUTOHELP_EXT(0x0055BFEC)
AUTOHELP_EXT(0x0055BFC0) AUTOHELP_EXT(0x0055BFA8) AUTOHELP_EXT(0x0055BF48) AUTOHELP_EXT(0x0055BF78)
AUTOHELP_EXT(0x0055BF5C) AUTOHELP_EXT(0x0055BF30) AUTOHELP_EXT(0x0055BE78) AUTOHELP_EXT(0x0055BE90)
AUTOHELP_EXT(0x0055BEB4) AUTOHELP_EXT(0x005FE968) AUTOHELP_EXT(0x0055BEE0) AUTOHELP_EXT(0x0055BECC)
AUTOHELP_EXT(0x0055BEF4) AUTOHELP_EXT(0x0055BF18) AUTOHELP_EXT(0x0055BF0C) AUTOHELP_EXT(0x0055BF24)
AUTOHELP_EXT(0x0055BF28) AUTOHELP_EXT(0x0055BE40) AUTOHELP_EXT(0x0055BE08) AUTOHELP_EXT(0x0055BE24)
AUTOHELP_EXT(0x0055BDA8) AUTOHELP_EXT(0x0055BDE0) AUTOHELP_EXT(0x0055BDF4) AUTOHELP_EXT(0x0055BD94)
AUTOHELP_EXT(0x005456EC) AUTOHELP_EXT(0x0055BD84) AUTOHELP_EXT(0x0055BD78) AUTOHELP_EXT(0x0055BD70)
AUTOHELP_EXT(0x0055BD68) AUTOHELP_EXT(0x0055BD5C) AUTOHELP_EXT(0x0055BD54) AUTOHELP_EXT(0x0055BD48)
AUTOHELP_EXT(0x0055BD40) AUTOHELP_EXT(0x0055BD38) AUTOHELP_EXT(0x0055BD2C) AUTOHELP_EXT(0x0055BD20)
AUTOHELP_EXT(0x0055BD10) AUTOHELP_EXT(0x0055BD08) AUTOHELP_EXT(0x0055BCF8) AUTOHELP_EXT(0x0055BCE4)
AUTOHELP_EXT(0x0055BCD4) AUTOHELP_EXT(0x0055BCC4) AUTOHELP_EXT(0x0055BCBC) AUTOHELP_EXT(0x0055BCB4)
AUTOHELP_EXT(0x0055BCAC) AUTOHELP_EXT(0x0055BCA4) AUTOHELP_EXT(0x0055BC9C) AUTOHELP_EXT(0x0055BC84)
AUTOHELP_EXT(0x0055BC30) AUTOHELP_EXT(0x0055BC14) AUTOHELP_EXT(0x0055BC08) AUTOHELP_EXT(0x0055BBF4)
AUTOHELP_EXT(0x0055BBE4) AUTOHELP_EXT(0x0055BBD0) AUTOHELP_EXT(0x0055BBBC) AUTOHELP_EXT(0x0055BB98)
AUTOHELP_EXT(0x0055BB38) AUTOHELP_EXT(0x0055BB50) AUTOHELP_EXT(0x0055B8B0) AUTOHELP_EXT(0x0055B94C)
AUTOHELP_EXT(0x0055BB64) AUTOHELP_EXT(0x0055BB80) AUTOHELP_EXT(0x0055BB24) AUTOHELP_EXT(0x0055BAC8)
AUTOHELP_EXT(0x0055BAE0) AUTOHELP_EXT(0x0055BAF4) AUTOHELP_EXT(0x0055BB04) AUTOHELP_EXT(0x0055BB14)
AUTOHELP_EXT(0x0055B9AC) AUTOHELP_EXT(0x0055B9C8) AUTOHELP_EXT(0x0055B9E0) AUTOHELP_EXT(0x0055B9F8)
AUTOHELP_EXT(0x0055BA10) AUTOHELP_EXT(0x0055BAB0) AUTOHELP_EXT(0x0055BA7C) AUTOHELP_EXT(0x0055BA64)
AUTOHELP_EXT(0x0055BA50) AUTOHELP_EXT(0x0055BA3C) AUTOHELP_EXT(0x0055BA28) AUTOHELP_EXT(0x0055BA90)
AUTOHELP_EXT(0x0055BAA0) AUTOHELP_EXT(0x0055B99C) AUTOHELP_EXT(0x0055B988) AUTOHELP_EXT(0x0055B978)
AUTOHELP_EXT(0x0055B96C) AUTOHELP_EXT(0x0055B958) AUTOHELP_EXT(0x0055B904) AUTOHELP_EXT(0x0055B920)
AUTOHELP_EXT(0x0055B938) AUTOHELP_EXT(0x0055B8E4) AUTOHELP_EXT(0x0055B8BC) AUTOHELP_EXT(0x0055B8D0)
AUTOHELP_EXT(0x0053E678)
#endif

// FUNCTION: TRILOGY 0x0046da45
// Shows the hover help line for the old-UI button under the mouse (menus, spell gems, HotButtons, tracking).
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// __fastcall (`this` in ECX, EDX unused), no stack arguments, no return value (the caller never reads EAX).
// Finds the control under the mouse (self+0x70 list, id in hoverId), picks the auto-help text for the current
// screen mode (self+0xfe0) into a 128-byte buffer and draws it below the cursor.
/* fake match: retained shared-label gotos reproduce the original dispatch and common-exit layout. */
void __fastcall EQ_Main_AutoHelp_guess(char* self)
{
    char helpText[128];
    int spellId;
    int controlId;
    short gemSpellId;
    short hoverId;
    char screenMode;
    POINT pt;

    hoverId = -1;
    EQ_METHOD(0, void, 0x0046D6FE, (void*), self)();
    if (EQ_GLOBAL(int, 0x006B6864) == 0)
    {
        return;
    }
    if (*(short*)(self + 0xec2) < 3)
    {
        return;
    }
    pt.x = EQ_GLOBAL(short, 0x006B6414);
    helpText[0] = 0;
    pt.y = EQ_GLOBAL(short, 0x006B6416);
    if (pinstEQ_Main_guess->isActive_guess == 0)
    {
        return;
    }
    for (controlId = *(int*)(self + 0x70); controlId != 0; controlId = *(int*)(controlId + 0x20))
    {
        if (PtInRect((RECT*)(controlId + 4), pt) != 0)
        {
            hoverId = *(short*)(controlId + 0x14);
            break;
        }
    }
    *(short*)(self + 0xebc) = hoverId;
    if (hoverId == -1)
    {
        return;
    }
    screenMode = *(char*)(self + 0xfe0);
    if (screenMode <= 3)
    {
        if (*(int*)(self + 0x685f4) != 0)
        {
            controlId = (int)hoverId;
            if (controlId != 0x50)
            {
                if (controlId != 0x51)
                {
                    if (controlId > 0x51 && controlId <= 0x65)
                    {
                        strcpy(helpText, AUTOHELP_STR(0x0055C150));
                    }
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055C134));
                }
            }
            else
            {
                strcpy(helpText, AUTOHELP_STR(0x0055C110));
            }
        }
        if (*(char*)(self + 0x8fbac) != 0)
        {
            return;
        }
        controlId = (int)hoverId;
        switch (controlId)
        {
        case 0xdc:
            strcpy(helpText, AUTOHELP_STR(0x0055C104));
            break;
        case 0xdd:
            strcpy(helpText, AUTOHELP_STR(0x0055C0F4));
            break;
        case 0xde:
            strcpy(helpText, AUTOHELP_STR(0x0055C0DC));
            break;
        case 0xdf:
            strcpy(helpText, AUTOHELP_STR(0x0055C0CC));
            break;
        case 0xe0:
            strcpy(helpText, AUTOHELP_STR(0x0055C0B8));
            break;
        case 200:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xcd:
        case 0xce:
        case 0xcf:
        case 0xd0:
        case 0xd1:
            strcpy(helpText, AUTOHELP_STR(0x0055C09C));
            break;
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xd7:
        case 0xd8:
        case 0xd9:
        case 0xda:
        case 0xdb:
            gemSpellId = *(short*)((char*)g_pc_guess + controlId * 2 + 0x7aa);
            if (gemSpellId == -1)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055C084));
            }
            else
            {
                spellId = (int)gemSpellId;
                if (spellId > 0 && spellId < 3000 && EQ_ARRAY(char*, 0x005FE9A0)[spellId] != 0)
                {
                    sprintf(helpText, AUTOHELP_STR(0x0053E678), EQ_ARRAY(char*, 0x005FE9A0)[spellId]);
                    goto afterButtonBar;
                }
                strcpy(helpText, AUTOHELP_STR(0x0055C068));
            }
            break;
        case 0xd2:
        case 0xd3:
            strcpy(helpText, AUTOHELP_STR(0x0055C050));
            break;
        default:
            goto afterButtonBar;
        }
afterButtonBar:
        if (*(int*)(self + 0x685d8) != 0 || *(int*)(self + 0x685d4) != 0)
        {
            if (hoverId >= 0x145 && hoverId <= 0x162)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055C01C));
            }
            if (hoverId == 0x163)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055C004));
            }
        }
        switch (controlId)
        {
        case 0xe3:
            if (*(char*)(self + 0xfe0) == 1)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BF90));
            }
            else
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BFEC));
            }
            break;
        case 0xe1:
            strcpy(helpText, AUTOHELP_STR(0x0055BFC0));
            break;
        case 0xde:
            strcpy(helpText, AUTOHELP_STR(0x0055BFA8));
            break;
        case 0xe4:
            if (*(char*)(self + 0xfe0) == 0 || *(char*)(self + 0xfe0) == 3)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BF90));
            }
            else
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BF48));
            }
            break;
        case 0xfe:
        case 0xff:
        case 0x100:
        case 0x101:
        case 0x102:
            strcpy(helpText, AUTOHELP_STR(0x0055BF78));
            break;
        case 0xfc:
            strcpy(helpText, AUTOHELP_STR(0x0055BF5C));
            break;
        case 0xe5:
            if (*(char*)(self + 0xfe0) == 3)
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BF48));
            }
            else
            {
                strcpy(helpText, AUTOHELP_STR(0x0055BF30));
            }
            break;
        default:
            goto perModeHelp;
        }
perModeHelp:
        screenMode = *(char*)(self + 0xfe0);
        if (screenMode == 0)
        {
            switch (controlId)
            {
            case 0xe6:
                strcpy(helpText, AUTOHELP_STR(0x0055BE78));
                break;
            case 0xe7:
                if (*(char*)(self + 0xb5) != 0)
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BEB4));
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BE90));
                }
                break;
            case 0xe8:
                if (strcmp((const char*)g_pc_guess, AUTOHELP_STR(0x005FE968)) == 0)
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BEE0));
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BECC));
                }
                break;
            case 0xe9:
                strcpy(helpText, AUTOHELP_STR(0x0055BEF4));
                break;
            case 0xea:
                if (*(char*)((char*)g_control_player_guess + 0xa1) == 0x6e)
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BF18));
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BF0C));
                }
                break;
            case 0xeb:
                if (EQ_GLOBAL(char, 0x006B6405) != 0)
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BF28));
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BF24));
                }
                break;
            default:
                goto drawHelp;
            }
        }
        else if (screenMode == 1)
        {
            if (controlId < 0xf0 || controlId > 0xf5)
                goto drawHelp;
            strcpy(helpText, AUTOHELP_STR(0x0055BE40));
        }
        else if (screenMode == 2)
        {
            switch (controlId)
            {
            case 0xf6:
                strcpy(helpText, AUTOHELP_STR(0x0055BE08));
                break;
            case 0xf7:
                strcpy(helpText, AUTOHELP_STR(0x0055BE24));
                break;
            case 0xf8:
            case 0xf9:
            case 0xfa:
            case 0xfb:
                strcpy(helpText, AUTOHELP_STR(0x0055BE40));
                break;
            default:
                goto drawHelp;
            }
        }
        else
        {
            if (screenMode != 3)
                goto drawHelp;
            switch (controlId)
            {
            case 0x39:
            case 0x3a:
            case 0x3b:
            case 0x3c:
            case 0x3d:
            case 0x3e:
            case 0x3f:
            case 0x40:
            case 0x41:
            case 0x42:
            case 0x43:
            case 0x44:
                strcpy(helpText, AUTOHELP_STR(0x0055BDA8));
                break;
            case 0x45:
                strcpy(helpText, AUTOHELP_STR(0x0055BDE0));
                break;
            case 0x46:
                strcpy(helpText, AUTOHELP_STR(0x0055BDF4));
                break;
            case 0x47:
            case 0x48:
            case 0x49:
            case 0x4a:
            case 0x4b:
                strcpy(helpText, AUTOHELP_STR(0x0055BD94));
                break;
            case 0x4c:
                strcpy(helpText, AUTOHELP_STR(0x005456EC));
                break;
            case 0x4d:
                strcpy(helpText, AUTOHELP_STR(0x0055BD84));
                break;
            default:
                goto drawHelp;
            }
        }
    }
    else if (screenMode == 4)
    {
        if (*(char*)(self + 0x100c) == 0)
        {
            switch ((int)hoverId)
            {
            case 1:
                strcpy(helpText, AUTOHELP_STR(0x0055BD78));
                break;
            case 2:
                strcpy(helpText, AUTOHELP_STR(0x0055BD70));
                break;
            case 3:
                strcpy(helpText, AUTOHELP_STR(0x0055BD68));
                break;
            case 4:
                strcpy(helpText, AUTOHELP_STR(0x0055BD5C));
                break;
            case 5:
                strcpy(helpText, AUTOHELP_STR(0x0055BD54));
                break;
            case 6:
                strcpy(helpText, AUTOHELP_STR(0x0055BD48));
                break;
            case 7:
                strcpy(helpText, AUTOHELP_STR(0x0055BD40));
                break;
            case 8:
                strcpy(helpText, AUTOHELP_STR(0x0055BD38));
                break;
            case 9:
                strcpy(helpText, AUTOHELP_STR(0x0055BD2C));
                break;
            case 10:
                strcpy(helpText, AUTOHELP_STR(0x0055BD20));
                break;
            case 0xb:
                strcpy(helpText, AUTOHELP_STR(0x0055BD10));
                break;
            case 0xc:
                strcpy(helpText, AUTOHELP_STR(0x0055BD08));
                break;
            case 0xd:
                strcpy(helpText, AUTOHELP_STR(0x0055BCF8));
                break;
            case 0xe:
                strcpy(helpText, AUTOHELP_STR(0x0055BCE4));
                break;
            case 0xf:
                strcpy(helpText, AUTOHELP_STR(0x0055BCD4));
                break;
            case 0x10:
                strcpy(helpText, AUTOHELP_STR(0x0055BCC4));
                break;
            case 0x11:
                strcpy(helpText, AUTOHELP_STR(0x0055BCBC));
                break;
            case 0x12:
                strcpy(helpText, AUTOHELP_STR(0x0055BCB4));
                break;
            case 0x13:
                strcpy(helpText, AUTOHELP_STR(0x0055BCAC));
                break;
            case 0x14:
                strcpy(helpText, AUTOHELP_STR(0x0055BCA4));
                break;
            case 0x15:
                strcpy(helpText, AUTOHELP_STR(0x0055BC9C));
                break;
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1a:
            case 0x1b:
            case 0x1c:
            case 0x1d:
                strcpy(helpText, AUTOHELP_STR(0x0055BC84));
                break;
            default:
                goto afterMode4Ids;
            case 0x20:
                strcpy(helpText, AUTOHELP_STR(0x0055BC30));
                break;
            case 0x21:
                strcpy(helpText, AUTOHELP_STR(0x0055BC14));
                break;
            case 0x22:
                strcpy(helpText, AUTOHELP_STR(0x0055BC08));
                break;
            case 0x23:
                strcpy(helpText, AUTOHELP_STR(0x0055BBF4));
                break;
            case 0x24:
                strcpy(helpText, AUTOHELP_STR(0x0055BBE4));
                break;
            case 0x25:
                strcpy(helpText, AUTOHELP_STR(0x0055BBD0));
                break;
            case 0x26:
                strcpy(helpText, AUTOHELP_STR(0x0055BBBC));
                break;
            case 0x27:
                strcpy(helpText, AUTOHELP_STR(0x0055BB98));
            }
        }
afterMode4Ids:
        if (*(int*)(self + 0x685e0) != 0)
        {
            switch ((int)hoverId)
            {
            case 0x103:
                strcpy(helpText, AUTOHELP_STR(0x0055BB38));
                break;
            case 0x104:
            case 0x105:
            case 0x106:
            case 0x107:
            case 0x108:
            case 0x109:
            case 0x10a:
            case 0x10b:
            case 0x10c:
            case 0x10d:
            case 0x10e:
            case 0x10f:
                strcpy(helpText, AUTOHELP_STR(0x0055BB50));
                break;
            case 0x110:
                strcpy(helpText, AUTOHELP_STR(0x0055B8B0));
                break;
            case 0x111:
                strcpy(helpText, AUTOHELP_STR(0x0055B94C));
                break;
            case 0x112:
                if (*(short*)(self + 0x6c65c) < 0x1e || *(int**)(self + 0x6c658) == 0 ||
                    **(int**)(self + 0x6c658) == 0)
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BB64));
                }
                else
                {
                    strcpy(helpText, AUTOHELP_STR(0x0055BB80));
                }
                break;
            default:
                goto drawHelp;
            }
        }
        else if (*(int*)(self + 0x685dc) != 0)
        {
            switch ((int)hoverId)
            {
            case 0x113:
            case 0x114:
            case 0x115:
            case 0x116:
            case 0x117:
            case 0x118:
            case 0x119:
            case 0x11a:
            case 0x11b:
            case 0x11c:
            case 0x11d:
            case 0x11e:
            case 0x11f:
            case 0x120:
            case 0x121:
            case 0x122:
                strcpy(helpText, AUTOHELP_STR(0x0055BB24));
                break;
            case 0x123:
                strcpy(helpText, AUTOHELP_STR(0x0055B8B0));
                break;
            case 0x124:
                strcpy(helpText, AUTOHELP_STR(0x0055B94C));
                break;
            default:
                goto drawHelp;
            }
        }
        else if (*(int*)(self + 0x685e8) != 0)
        {
            switch ((int)hoverId)
            {
            case 0xaa:
            case 0xab:
            case 0xac:
            case 0xad:
            case 0xae:
            case 0xaf:
            case 0xb0:
            case 0xb1:
                strcpy(helpText, AUTOHELP_STR(0x0055BAC8));
                break;
            case 0xb4:
                strcpy(helpText, AUTOHELP_STR(0x0055BAE0));
                break;
            case 0xb5:
                strcpy(helpText, AUTOHELP_STR(0x0055BAF4));
                break;
            case 0xb6:
                strcpy(helpText, AUTOHELP_STR(0x0055BB04));
                break;
            case 0xb7:
                strcpy(helpText, AUTOHELP_STR(0x0055BB14));
                break;
            default:
                goto drawHelp;
            }
        }
        else
        {
            if (*(int*)(self + 0x685e4) != 0)
            {
                switch ((int)hoverId)
                {
                case 300:
                case 0x12d:
                case 0x12e:
                case 0x12f:
                case 0x130:
                case 0x131:
                case 0x132:
                case 0x133:
                    strcpy(helpText, AUTOHELP_STR(0x0055BAB0));
                    break;
                case 0x144:
                    strcpy(helpText, AUTOHELP_STR(0x0055BAA0));
                    break;
                case 0x143:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA90));
                    break;
                default:
                    goto drawHelp;
                case 0x136:
                case 0x137:
                case 0x138:
                case 0x139:
                case 0x13a:
                case 0x13b:
                case 0x13c:
                case 0x13d:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA7C));
                    break;
                case 0x13e:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA64));
                    break;
                case 0x13f:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA50));
                    break;
                case 0x140:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA3C));
                    break;
                case 0x141:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA28));
                    break;
                }
            }
            else
            {
                switch ((int)hoverId)
                {
                case 0x1e:
                    strcpy(helpText, AUTOHELP_STR(0x0055B9AC));
                    break;
                case 0x1f:
                    strcpy(helpText, AUTOHELP_STR(0x0055B9C8));
                    break;
                case 0x28:
                    strcpy(helpText, AUTOHELP_STR(0x0055B9E0));
                    break;
                case 0x29:
                    strcpy(helpText, AUTOHELP_STR(0x0055B9F8));
                    break;
                case 0x2a:
                    strcpy(helpText, AUTOHELP_STR(0x0055BA10));
                    break;
                default:
                    goto drawHelp;
                }
            }
        }
    }
    else if (screenMode == 5)
    {
        if (*(char*)(self + 0x8fbac) != 0)
        {
            return;
        }
        controlId = (int)hoverId;
        switch (controlId)
        {
        case 0xdc:
            strcpy(helpText, AUTOHELP_STR(0x0055C104));
            break;
        case 0xdd:
            strcpy(helpText, AUTOHELP_STR(0x0055C0F4));
            break;
        case 0xde:
            strcpy(helpText, AUTOHELP_STR(0x0055C0DC));
            break;
        case 0xdf:
            strcpy(helpText, AUTOHELP_STR(0x0055B99C));
            break;
        case 0xe0:
            strcpy(helpText, AUTOHELP_STR(0x0055C0B8));
            break;
        case 200:
        case 0xc9:
        case 0xca:
        case 0xcb:
        case 0xcc:
        case 0xcd:
        case 0xce:
        case 0xcf:
        case 0xd0:
        case 0xd1:
            strcpy(helpText, AUTOHELP_STR(0x0055C09C));
            break;
        case 0xd4:
        case 0xd5:
        case 0xd6:
        case 0xd7:
        case 0xd8:
        case 0xd9:
        case 0xda:
        case 0xdb:
            strcpy(helpText, AUTOHELP_STR(0x0055C084));
            break;
        case 0xd2:
        case 0xd3:
            strcpy(helpText, AUTOHELP_STR(0x0055C050));
            break;
        default:
            goto afterButtonBar5;
        }
afterButtonBar5:
        switch (controlId)
        {
        case 0x38:
            strcpy(helpText, AUTOHELP_STR(0x0055BC08));
            break;
        case 0x16:
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
        case 0x1d:
            strcpy(helpText, AUTOHELP_STR(0x0055BC84));
            break;
        case 0x2b:
        case 0x2c:
        case 0x2d:
        case 0x2e:
        case 0x2f:
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
            strcpy(helpText, AUTOHELP_STR(0x0055B958));
            break;
        case 0x35:
            strcpy(helpText, AUTOHELP_STR(0x0055B96C));
            break;
        case 0x36:
            strcpy(helpText, AUTOHELP_STR(0x0055B978));
            break;
        case 0x37:
            strcpy(helpText, AUTOHELP_STR(0x0055B988));
            break;
        case 0xde:
            strcpy(helpText, AUTOHELP_STR(0x0055BF5C));
            break;
        case 0xe1:
            strcpy(helpText, AUTOHELP_STR(0x0055BFC0));
            break;
        case 0xe2:
            strcpy(helpText, AUTOHELP_STR(0x0055BFA8));
            break;
        case 0xfe:
        case 0xff:
        case 0x100:
        case 0x101:
        case 0x102:
            strcpy(helpText, AUTOHELP_STR(0x0055BF78));
            break;
        default:
            goto drawHelp;
        }
    }
    else
    {
        if (screenMode != 6)
            goto drawHelp;
        switch ((int)hoverId)
        {
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            strcpy(helpText, AUTOHELP_STR(0x0055B904));
            break;
        case 8:
            strcpy(helpText, AUTOHELP_STR(0x0055B920));
            break;
        case 9:
            strcpy(helpText, AUTOHELP_STR(0x0055B938));
            break;
        case 10:
            strcpy(helpText, AUTOHELP_STR(0x0055B8B0));
            break;
        case 0xb:
            strcpy(helpText, AUTOHELP_STR(0x0055B94C));
            break;
        case 0xe:
        case 0xf:
        case 0x10:
        case 0x11:
        case 0x12:
            strcpy(helpText, AUTOHELP_STR(0x0055B8E4));
            break;
        case 0x13:
            strcpy(helpText, AUTOHELP_STR(0x0055B8B0));
            break;
        case 0x14:
            strcpy(helpText, AUTOHELP_STR(0x0055B94C));
            break;
        case 0x15:
        case 0x16:
            strcpy(helpText, AUTOHELP_STR(0x0055B8BC));
            break;
        case 0x18:
            strcpy(helpText, AUTOHELP_STR(0x0055B8D0));
            break;
        default:
            goto drawHelp;
        }
    }
drawHelp:
    EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, char*, int), self)(pt.x, pt.y + 0x14, helpText, 10);
}

// FUNCTION: TRILOGY 0x0046e4aa
// Repositions ten interface controls to match a selected page layout.
// __fastcall (`this` in ECX, EDX unused), one stack argument, no return value (the caller never reads EAX).
// Lays out the ten spellbook hot button rectangles for layout (layoutSelector % 3): each is set into the
// per-screen-mode rect array (self + 0x6cbbc + mode * 0x1900, 0x10 apart) and into the rect the pointer at
// self + 0x6c5a8.. refers to (the last one through the pointer at self + 0x6c5cc).
void __fastcall EQ_Main_DoHotButton_configureSpellbookHotButtonRects_guess(void* eqMain, int unusedEdx, int layoutSelector)
{
    char* self = (char*)eqMain;
    tSetRect_guess_t setRect;
    int layoutIndex;

    layoutIndex = layoutSelector % 3;
    switch (layoutIndex)
    {
    case 2:
        setRect = g_setrect_guess;
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbbc + self), 0xb3, 0x33, 0xdc, 0x5c);
        setRect(*(RECT**)(self + 0x6c5a8), 0xb3, 0x33, 0xdc, 0x5c);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbcc + self), 0x10a, 0x2f, 0x134, 0x58);
        setRect(*(RECT**)(self + 0x6c5ac), 0x10a, 0x2f, 0x134, 0x58);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbdc + self), 0xdf, 0x70, 0x108, 0x99);
        setRect(*(RECT**)(self + 0x6c5b0), 0xdf, 0x70, 0x108, 0x99);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbec + self), 0xb3, 0xa9, 0xdc, 0xd2);
        setRect(*(RECT**)(self + 0x6c5b4), 0xb3, 0xa9, 0xdc, 0xd2);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbfc + self), 0x10b, 0xaa, 0x134, 0xd3);
        setRect(*(RECT**)(self + 0x6c5b8), 0x10b, 0xaa, 0x134, 0xd3);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc0c + self), 0x17c, 0x29, 0x1a5, 0x52);
        setRect(*(RECT**)(self + 0x6c5bc), 0x17c, 0x29, 0x1a5, 0x52);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc1c + self), 0x14e, 0x6b, 0x177, 0x94);
        setRect(*(RECT**)(self + 0x6c5c0), 0x14e, 0x6b, 0x177, 0x94);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc2c + self), 0x1a7, 0x6b, 0x1d0, 0x94);
        setRect(*(RECT**)(self + 0x6c5c4), 0x1a7, 0x6b, 0x1d0, 0x94);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc3c + self), 0x15c, 0xae, 0x185, 0xd7);
        setRect(*(RECT**)(self + 0x6c5c8), 0x15c, 0xae, 0x185, 0xd7);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc4c + self), 0x197, 0xae, 0x1c0, 0xd7);
        setRect(*(RECT**)(self + 0x6c5cc), 0x197, 0xae, 0x1c0, 0xd7);
        break;
    case 1:
        setRect = g_setrect_guess;
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbbc + self), 0xad, 0x2d, 0xd6, 0x55);
        setRect(*(RECT**)(self + 0x6c5a8), 0xad, 0x2d, 0xd6, 0x55);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbcc + self), 0x10b, 0x2d, 0x134, 0x55);
        setRect(*(RECT**)(self + 0x6c5ac), 0x10b, 0x2d, 0x134, 0x55);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbdc + self), 0xdc, 0x72, 0x105, 0x9a);
        setRect(*(RECT**)(self + 0x6c5b0), 0xdc, 0x72, 0x105, 0x9a);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbec + self), 0xac, 0xab, 0xd5, 0xd3);
        setRect(*(RECT**)(self + 0x6c5b4), 0xac, 0xab, 0xd5, 0xd3);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbfc + self), 0x10b, 0xab, 0x134, 0xd3);
        setRect(*(RECT**)(self + 0x6c5b8), 0x10b, 0xab, 0x134, 0xd3);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc0c + self), 0x14c, 0x2d, 0x175, 0x55);
        setRect(*(RECT**)(self + 0x6c5bc), 0x14c, 0x2d, 0x175, 0x55);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc1c + self), 0x1a4, 0x2d, 0x1cd, 0x55);
        setRect(*(RECT**)(self + 0x6c5c0), 0x1a4, 0x2d, 0x1cd, 0x55);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc2c + self), 0x177, 0x71, 0x1a0, 0x99);
        setRect(*(RECT**)(self + 0x6c5c4), 0x177, 0x71, 0x1a0, 0x99);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc3c + self), 0x14b, 0xaa, 0x174, 0xd2);
        setRect(*(RECT**)(self + 0x6c5c8), 0x14b, 0xaa, 0x174, 0xd2);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc4c + self), 0x1a4, 0xaa, 0x1cd, 0xd2);
        setRect(*(RECT**)(self + 0x6c5cc), 0x1a4, 0xaa, 0x1cd, 0xd2);
        break;
    case 0:
        setRect = g_setrect_guess;
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbbc + self), 0xba, 0x2e, 0xe3, 0x56);
        setRect(*(RECT**)(self + 0x6c5a8), 0xba, 0x2e, 0xe3, 0x56);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbcc + self), 0x103, 0x28, 0x12c, 0x50);
        setRect(*(RECT**)(self + 0x6c5ac), 0x103, 0x28, 0x12c, 0x50);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbdc + self), 0x103, 0x6c, 0x12c, 0x94);
        setRect(*(RECT**)(self + 0x6c5b0), 0x103, 0x6c, 0x12c, 0x94);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbec + self), 0xba, 0xa6, 0xe3, 0xce);
        setRect(*(RECT**)(self + 0x6c5b4), 0xba, 0xa6, 0xe3, 0xce);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cbfc + self), 0x103, 0xb0, 0x12c, 0xd8);
        setRect(*(RECT**)(self + 0x6c5b8), 0x103, 0xb0, 0x12c, 0xd8);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc0c + self), 0x157, 0x28, 0x180, 0x50);
        setRect(*(RECT**)(self + 0x6c5bc), 0x157, 0x28, 0x180, 0x50);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc1c + self), 0x1a3, 0x46, 0x1cc, 0x6e);
        setRect(*(RECT**)(self + 0x6c5c0), 0x1a3, 0x46, 0x1cc, 0x6e);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc2c + self), 0x157, 0x6c, 0x180, 0x94);
        setRect(*(RECT**)(self + 0x6c5c4), 0x157, 0x6c, 0x180, 0x94);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc3c + self), 0x1a3, 0x90, 0x1cd, 0xb8);
        setRect(*(RECT**)(self + 0x6c5c8), 0x1a3, 0x90, 0x1cd, 0xb8);
        setRect((RECT*)(*(char*)(self + 0xfe0) * 0x1900 + 0x6cc4c + self), 0x157, 0xb0, 0x180, 0xd8);
        setRect(*(RECT**)(self + 0x6c5cc), 0x157, 0xb0, 0x180, 0xd8);
        break;
    }
}

// One 0x20-byte layout entry of the EQ_Main window positions (self + 0x90044 ... 0x90264, with a template row per
// screen mode at self + 0x90284 ... 0x90f44 + mode * 0x20). Every accessor VC6 expands inline: the getters clear
// `dirty` first, the setters set it.
struct OverlayPos_guess
{
    int x;      // 0x00
    int y;      // 0x04
    int w;      // 0x08
    int h;      // 0x0c
    int dirty;  // 0x10
    int f14;    // 0x14
    int f18;    // 0x18
    int f1c;    // 0x1c

    int GetX() { dirty = 0; return x; }
    int GetY() { dirty = 0; return y; }
    int GetW() { dirty = 0; return w; }
    int GetH() { dirty = 0; return h; }
    int GetF14() { dirty = 0; return f14; }
    int GetF18() { dirty = 0; return f18; }
    int GetF1C() { dirty = 0; return f1c; }
    void SetPos(int px, int py) { dirty = 1; x = px; y = py; }
    void SetY(int py) { dirty = 1; y = py; }
    void SetW(int pw) { dirty = 1; w = pw; }
    void SetH(int ph) { dirty = 1; h = ph; }
    void SetF14(int v) { dirty = 1; f14 = v; }
    void SetF18(int v) { dirty = 1; f18 = v; }
    void SetF1C(int v) { dirty = 1; f1c = v; }
};

#define OVL_POS(off)    ((OverlayPos_guess*)(self + (off)))
#define OVL_ROW(off)    ((OverlayPos_guess*)(self + mode * 0x20 + (off)))
// the current screen layout's rect array: self + off + (*(char*)(self + 0xfe0)) * 0x1900
#define OVL_RECT(off)   ((RECT*)(self + (off) + *(char*)(self + 0xfe0) * 0x1900))
#define OVL_IDXRECT(idx) ((RECT*)(self + 0x6c90c + *(char*)(self + 0xfe0) * 0x1900 + (idx) * 0x10))
#define OVL_SETRECT(off, l, t, r, b)  SetRect(OVL_RECT(off), l, t, r, b)
#define OVL_SETIDX(idx, l, t, r, b)   SetRect(OVL_IDXRECT(idx), l, t, r, b)
#define OVL_SETABS(off, l, t, r, b)   SetRect((RECT*)(self + (off)), l, t, r, b)
#define OVL_SETABSIDX(base, idx, l, t, r, b)  SetRect((RECT*)(self + (base) + (idx) * 0x10), l, t, r, b)
#define OVL_ADD(id, rect) \
    EQ_METHOD(6, void, 0x004503BE, (char*, short, RECT*, int, int, int, int), self)(id, rect, -1, -1, 0, 0)
#define OVL_BTN(id, off)     OVL_ADD(id, OVL_RECT(off))
#define OVL_BTNIDX(id, idx)  OVL_ADD(id, OVL_IDXRECT(idx))
#define OVL_BTNABS(id, off)  OVL_ADD(id, (RECT*)(self + (off)))
#define OVL_VCALL(obj, slot) \
    ((void (__fastcall *)(void*))*(int*)(*(int*)(obj) + (slot)))((void*)(obj))
#define OVL_HASSKILL(skill) \
    EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), EQ_GLOBAL(void*, 0x006E4DA8))(skill)
#define OVL_PC               EQ_GLOBAL(char*, 0x006E4DA8)
// copy the template row to the entry, then (when the entry's f18 was set) mark it set again
#define OVL_SYNC18(dst, src) \
    if (OVL_POS(dst)->GetF18() != 0) \
    { \
        *OVL_POS(dst) = *OVL_ROW(src); \
        OVL_POS(dst)->SetF18(1); \
    } \
    else \
    { \
        *OVL_POS(dst) = *OVL_ROW(src); \
    }
// an entry's f14 flag and its start position: either parked off screen (2000, 2000) or left alone
#define OVL_PARK(dst, flag) \
    if (flag) \
    { \
        OVL_POS(dst)->SetF14(0); \
        OVL_POS(dst)->SetPos(2000, 2000); \
    } \
    else \
    { \
        OVL_POS(dst)->SetF14(1); \
    }
// the hotbar-style skill list: walk the overlay element list at self + 0x70 for the element with id `id`
#define OVL_FIND_ELEMENT(dst, id) \
    *(int*)(self + (dst)) = 0; \
    if (*(int*)(self + 0x70) != 0) \
    { \
        for (local_10 = *(int*)(self + 0x70); local_10 != 0; local_10 = *(int*)(local_10 + 0x20)) \
        { \
            if (*(short*)(local_10 + 0x14) == (id)) \
                *(int*)(self + (dst)) = local_10 + 4; \
        } \
    }

// FUNCTION: TRILOGY 0x0046eba7
// Rebuilds the old UI main screen for its current view: window layout, button hit areas and skill lists.
// __fastcall (`this` in ECX), no stack arguments, no return value. Re-lays out the EQ_Main window for the current
// screen mode (self + 0xfe0): picks the resolution row (mode 0..5) from the game state / screen width, syncs the
// layout entries from their per-resolution template rows, then rebuilds the button rect list for the screen.
void __fastcall FUN_0046eba7_guess(void* thisObj)
{
    char* self = (char*)thisObj;
    int mode = 0;
    int iVar4;
    int iVar6;
    int tmp;
    int iVar8;
    int local_10;
    short idx2;
    short idx;
    short local_14;
    short counter2;
    short gridRow;
    short local_c;
    char hasSkill;

    *(int*)(self + 0x6c8f4) = 0;
    if (g_game_state_guess == 2)
    {
        if (EQ_GLOBAL(int, 0x006B63FC) == 800)
            mode = 2;
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x400)
            mode = 3;
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x500)
            mode = 4;
        else if (EQ_GLOBAL(int, 0x006B63FC) == 0x640)
            mode = 5;
        else
            mode = 1;
    }
    else
    {
        mode = 0;
    }
    if (mode != 0)
    {
        OVL_SYNC18(0x90044, 0x90284);
        OVL_SYNC18(0x90064, 0x90344);
        OVL_SYNC18(0x90084, 0x90404);
        OVL_SYNC18(0x900a4, 0x904c4);
        if (OVL_POS(0x900a4)->GetH() != 0xd8)
        {
            OVL_POS(0x900a4)->SetH(0xd8);
            OVL_ROW(0x904c4)->SetH(0xd8);
        }
        if (OVL_POS(0x900a4)->GetW() != 0x28)
        {
            OVL_POS(0x900a4)->SetW(0x28);
            OVL_ROW(0x904c4)->SetW(0x28);
        }
        OVL_SYNC18(0x900c4, 0x90584);
        if (OVL_POS(0x900c4)->GetH() != 0xec)
        {
            OVL_POS(0x900c4)->SetH(0xec);
            OVL_ROW(0x90584)->SetH(0xec);
        }
        if (OVL_POS(0x900c4)->GetW() != 0x5c)
        {
            OVL_POS(0x900c4)->SetW(0x5c);
            OVL_ROW(0x90584)->SetW(0x5c);
        }
        if (EQ_GLOBAL(int, 0x006B6400) - 0xed < OVL_POS(0x900c4)->GetY())
        {
            OVL_POS(0x900c4)->SetY(EQ_GLOBAL(int, 0x006B6400) - 0xed);
            OVL_ROW(0x90584)->SetY(EQ_GLOBAL(int, 0x006B6400) - 0xed);
        }
        if (OVL_POS(0x90164)->GetF18() != 0 || OVL_POS(0x90164)->GetF1C() != 0)
        {
            if (OVL_POS(0x90164)->GetF1C() != 0)
            {
                *OVL_POS(0x90164) = *OVL_ROW(0x90944);
                OVL_POS(0x90164)->SetF1C(1);
            }
            else
            {
                *OVL_POS(0x90164) = *OVL_ROW(0x90944);
                OVL_POS(0x90164)->SetF18(1);
            }
        }
        else
        {
            *OVL_POS(0x90164) = *OVL_ROW(0x90944);
        }
        OVL_SYNC18(0x90244, 0x90e84);
        OVL_SYNC18(0x90184, 0x90a04);
        OVL_SYNC18(0x90264, 0x90f44);
    }
    else
    {
        *OVL_POS(0x90044) = *OVL_POS(0x90284);
        *OVL_POS(0x90064) = *OVL_POS(0x90344);
        *OVL_POS(0x90084) = *OVL_POS(0x90404);
        *OVL_POS(0x900a4) = *OVL_POS(0x904c4);
        *OVL_POS(0x900c4) = *OVL_POS(0x90584);
        *OVL_POS(0x90164) = *OVL_POS(0x90944);
        *OVL_POS(0x90244) = *OVL_POS(0x90e84);
        *OVL_POS(0x90184) = *OVL_POS(0x90a04);
        *OVL_POS(0x90264) = *OVL_POS(0x90f44);
    }
    *OVL_POS(0x900e4) = *OVL_ROW(0x90644);
    *OVL_POS(0x90224) = *OVL_ROW(0x90dc4);
    if (mode != 0)
    {
        OVL_PARK(0x90044, EQ_GLOBAL(int, 0x006E2098));
        OVL_PARK(0x90064, EQ_GLOBAL(int, 0x006E209C));
        OVL_PARK(0x90084, EQ_GLOBAL(int, 0x006E20A0));
        OVL_PARK(0x900a4, EQ_GLOBAL(int, 0x006E20A4));
        OVL_PARK(0x900c4, EQ_GLOBAL(int, 0x006E20AC));
        OVL_PARK(0x90164, EQ_GLOBAL(int, 0x006E20A8));
        OVL_PARK(0x900e4, EQ_GLOBAL(int, 0x006E20B0));
        OVL_PARK(0x90244, EQ_GLOBAL(int, 0x006E20B4));
        if (*(int*)(self + 0x685f4) != 0)
            OVL_POS(0x90264)->SetF14(1);
        else
            OVL_POS(0x90264)->SetF14(0);
    }
    OVL_VCALL(EQ_GLOBAL(int*, 0x006E4DE8), 8);
    OVL_VCALL(EQ_GLOBAL(int*, 0x006E4DE8), 4);
    EQ_GLOBAL(int, 0x006B676C) = 0;
    EQ_GLOBAL(int, 0x006B6764) =
        EQ_CDECL(int, 0x004A461E, (void))();
    if (*(char*)(self + 0xfe0) == 4)
        *(int*)(self + 0x8fba8) = 1;
    else
        *(int*)(self + 0x8fba8) = 0;
    EQ_GLOBAL(int, 0x006EFDD4) = 0;
    if (EQ_GLOBAL(int*, 0x006018C0) != 0 && EQ_GLOBAL(int*, 0x006018C0)[4] != 0)
        OVL_VCALL(EQ_GLOBAL(int*, 0x006018C0), 8);
    if (EQ_GLOBAL(int*, 0x00604984) != 0 && EQ_GLOBAL(int*, 0x00604984)[4] != 0)
    {
        EQ_GLOBAL(int*, 0x00604984)[0x7a] = 1;
        OVL_VCALL(EQ_GLOBAL(int*, 0x00604984), 8);
    }
    if (g_game_state_guess != 2)
    {
        if (EQ_GLOBAL(char*, 0x006E4E58) != 0 && *(int*)(EQ_GLOBAL(char*, 0x006E4E58) + 0x10) != 0)
            EQ_METHOD(2, void, 0x004178F4, (char*, int, int), EQ_GLOBAL(char*, 0x006E4E58))(*(int*)(EQ_GLOBAL(char*, 0x006E4E58) + 0x24),
                 *(int*)(EQ_GLOBAL(char*, 0x006E4E58) + 0x28));
        if (EQ_GLOBAL(char*, 0x006E4E5C) != 0 && *(int*)(EQ_GLOBAL(char*, 0x006E4E5C) + 0x10) != 0)
            EQ_METHOD(2, void, 0x004178F4, (char*, int, int), EQ_GLOBAL(char*, 0x006E4E5C))(*(int*)(EQ_GLOBAL(char*, 0x006E4E5C) + 0x24),
                 *(int*)(EQ_GLOBAL(char*, 0x006E4E5C) + 0x28));
    }
    if (*(u8*)(self + 0x100c) != 0 &&
        EQ_GLOBAL(void*, 0x006E4DAC) ==
        EQ_GLOBAL(void*, 0x006E4D8C))
    {
        local_14 = (short)(int)strlen(g_eqmain_guess + 0xeda);
        for (counter2 = 0; counter2 < local_14; counter2++)
        {
            if (*(char*)(g_eqmain_guess + 0xeda + counter2) > 0)
                EQ_GLOBAL(int, 0x006EFDD4) = EQ_GLOBAL(int, 0x006EFDD4) +
                    EQ_GLOBAL(int*, 0x006EFDDC)
                        [*(char*)(g_eqmain_guess + 0xeda + counter2)];
        }
    }
    *(int*)(self + 0x1430) = 0;
    *(int*)(self + 0x1434) = 0;
    *(short*)(self + 0xed8) = 0;
    OVCLEAR(self + 0x8fbf8);
    OVCLEAR(self + 0x8fc18);
    OVCLEAR(self + 0x8fc38);
    OVCLEAR(self + 0x8fc3d);
    *(char*)(self + 0x8fc41) = 0;
    *(char*)(self + 0x8fc42) = 0;
    OVCLEAR(self + 0x8fc43);
    *(int*)(self + 0x5f174) = 0;
    *(int*)(self + 0x5f178) = 0;
    *(int*)(self + 0x5f17c) = 0;
    *(int*)(self + 0x5f180) = 0;
    if (*(int*)(self + 0x685f4) == 0)
    {
        *(int*)(self + 0x6c4e0) = 0;
        *(int*)(self + 0x6c4dc) = 0;
    }
    *(short*)(self + 0x685c8) = 0;
    *(int*)(self + 0x6c658) = 0;
    *(short*)(self + 0x6c65c) = -1;
    *(char*)(self + 0x8fbad) = 0;
    *(int*)(self + 0x8fbb0) = 0;
    *(short*)(self + 0x8fbb4) = 0;
    *(short*)(self + 0x5f186) = -1;
    *(int*)(self + 0x685b4) = 0;
    *(short*)(self + 0x5f188) = -1;
    *(short*)(self + 0x5f18a) = -1;
    *(int*)(self + 0x6c554) = 0;
    *(short*)(self + 0xebc) = -1;
    *(short*)(self + 0xec2) = 0;
    *(int*)(self + 0xeb8) = 1;
    *(int*)(self + 0x10) = 1;
    EQ_METHOD(0, void, 0x00450463, (void*), self)();
    *(int*)(self + 0x70) = 0;
    *(char*)(self + 0x8fbb8) = -1;
    if (g_game_state_guess == 2)
    {
        iVar8 = OVL_POS(0x90044)->GetX();
        iVar4 = OVL_POS(0x90044)->GetY();
        OVL_SETRECT(0x6df8c, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x168, 0x6df8c);
        iVar8 = OVL_POS(0x90064)->GetX();
        iVar4 = OVL_POS(0x90064)->GetY();
        OVL_SETRECT(0x6df9c, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x169, 0x6df9c);
        iVar8 = OVL_POS(0x90084)->GetX();
        iVar4 = OVL_POS(0x90084)->GetY();
        OVL_SETRECT(0x6dfac, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x16a, 0x6dfac);
        iVar8 = OVL_POS(0x900a4)->GetX();
        iVar4 = OVL_POS(0x900a4)->GetY();
        OVL_SETRECT(0x6dfbc, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x16b, 0x6dfbc);
        iVar8 = OVL_POS(0x900c4)->GetX();
        iVar4 = OVL_POS(0x900c4)->GetY();
        OVL_SETRECT(0x6dfcc, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x16c, 0x6dfcc);
        iVar8 = OVL_POS(0x90164)->GetX();
        iVar4 = OVL_POS(0x90164)->GetY();
        OVL_SETRECT(0x6dfdc, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x16d, 0x6dfdc);
        iVar6 = OVL_POS(0x90164)->GetW();
        tmp = OVL_POS(0x90164)->GetH();
        OVL_SETRECT(0x6dfec, iVar8 - 8 + iVar6, iVar4 - 8 + tmp, iVar8 + iVar6, iVar4 + tmp);
        OVL_BTN(0x16e, 0x6dfec);
        iVar8 = OVL_POS(0x90244)->GetX();
        iVar4 = OVL_POS(0x90244)->GetY();
        OVL_SETRECT(0x6dffc, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x16f, 0x6dffc);
        iVar8 = OVL_POS(0x90264)->GetX();
        iVar4 = OVL_POS(0x90264)->GetY();
        OVL_SETRECT(0x6e01c, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
        OVL_BTN(0x171, 0x6e01c);
        if (*(char*)(self + 0xfe0) == 4 && g_pcloudFlagA_guess != 0)
        {
            OVL_POS(0x90184)->SetF14(1);
            iVar8 = OVL_POS(0x90184)->GetX();
            iVar4 = OVL_POS(0x90184)->GetY();
            OVL_SETRECT(0x6e00c, iVar8, iVar4, iVar8 + 8, iVar4 + 8);
            OVL_BTN(0x170, 0x6e00c);
        }
        else
        {
            OVL_POS(0x90184)->SetF14(0);
        }
    }
    EQ_METHOD(0, void, 0x004155FE, (void*), EQ_GLOBAL(void*, 0x006E4DEC))();
    if (*(char*)(self + 0xfe0) < 4 || *(char*)(self + 0xfe0) == 5 ||
        (g_game_state_guess == 2 && *(char*)(self + 0xfe0) == 4))
    {
        if (g_game_state_guess == 2)
        {
            iVar8 = OVL_POS(0x90244)->GetX();
            iVar4 = OVL_POS(0x90244)->GetY();
            OVL_SETRECT(0x6d73c, iVar8 + 10, iVar4 + 4, iVar8 + 0x6e, iVar4 + 0x18);
            OVL_SETRECT(0x6d74c, iVar8 + 10, iVar4 + 0x18, iVar8 + 0x6e, iVar4 + 0x2c);
            OVL_SETRECT(0x6d75c, iVar8 + 10, iVar4 + 0x2c, iVar8 + 0x6e, iVar4 + 0x40);
            OVL_SETRECT(0x6d7dc, iVar8, iVar4 + 0x40, 0x280, 0x1e0);
        }
        else
        {
            OVL_SETRECT(0x6d73c, 0x212, 0xfd, 0x276, 0x10e);
            OVL_SETRECT(0x6d74c, 0x212, 0x112, 0x276, 0x123);
            OVL_SETRECT(0x6d75c, 0x212, 0x127, 0x276, 0x138);
            OVL_SETRECT(0x6d7dc, 0x20e, 0x141, 0x280, 0x1e0);
        }
        iVar8 = OVL_POS(0x900c4)->GetX();
        iVar4 = OVL_POS(0x900c4)->GetY();
        if (g_game_state_guess == 2)
        {
            OVL_SETRECT(0x6d58c, iVar8 + 4, iVar4 + 0x10, iVar8 + 0x2c, iVar4 + 0x38);
            OVL_SETRECT(0x6d59c, iVar8 + 0x30, iVar4 + 0x10, iVar8 + 0x58, iVar4 + 0x38);
            OVL_SETRECT(0x6d5ac, iVar8 + 4, iVar4 + 0x3c, iVar8 + 0x2c, iVar4 + 100);
            OVL_SETRECT(0x6d5bc, iVar8 + 0x30, iVar4 + 0x3c, iVar8 + 0x58, iVar4 + 100);
            OVL_SETRECT(0x6d5cc, iVar8 + 4, iVar4 + 0x68, iVar8 + 0x2c, iVar4 + 0x90);
            OVL_SETRECT(0x6d5dc, iVar8 + 0x30, iVar4 + 0x68, iVar8 + 0x58, iVar4 + 0x90);
            OVL_SETRECT(0x6d5ec, iVar8 + 4, iVar4 + 0x94, iVar8 + 0x2c, iVar4 + 0xbc);
            OVL_SETRECT(0x6d5fc, iVar8 + 0x30, iVar4 + 0x94, iVar8 + 0x58, iVar4 + 0xbc);
            OVL_SETRECT(0x6d60c, iVar8 + 4, iVar4 + 0xc0, iVar8 + 0x2c, iVar4 + 0xec);
            OVL_SETRECT(0x6d61c, iVar8 + 0x30, iVar4 + 0xc0, iVar8 + 0x58, iVar4 + 0xec);
            OVL_SETRECT(0x6d62c, iVar8 + 0x16, iVar4 + 1, iVar8 + 0x26, iVar4 + 0xb);
            OVL_SETRECT(0x6d63c, iVar8 + 0x36, iVar4 + 1, iVar8 + 0x46, iVar4 + 0xb);
        }
        else
        {
            OVL_SETRECT(0x6d58c, iVar8 + 8, iVar4 + 0x10, iVar8 + 0x31, iVar4 + 0x39);
            OVL_SETRECT(0x6d59c, iVar8 + 0x3a, iVar4 + 0x10, iVar8 + 99, iVar4 + 0x39);
            OVL_SETRECT(0x6d5ac, iVar8 + 8, iVar4 + 0x42, iVar8 + 0x31, iVar4 + 0x6b);
            OVL_SETRECT(0x6d5bc, iVar8 + 0x3a, iVar4 + 0x42, iVar8 + 99, iVar4 + 0x6b);
            OVL_SETRECT(0x6d5cc, iVar8 + 8, iVar4 + 0x74, iVar8 + 0x31, iVar4 + 0x9d);
            OVL_SETRECT(0x6d5dc, iVar8 + 0x3a, iVar4 + 0x74, iVar8 + 99, iVar4 + 0x9d);
            OVL_SETRECT(0x6d5ec, -0x578, -10, -0x582, -10);
            OVL_SETRECT(0x6d5fc, -0x578, -10, -0x582, -10);
            OVL_SETRECT(0x6d60c, -0x578, -10, -0x582, -10);
            OVL_SETRECT(0x6d61c, -0x578, -10, -0x582, -10);
            OVL_SETRECT(0x6d62c, iVar8 + 0x1e, iVar4 + 1, iVar8 + 0x36, iVar4 + 0xb);
            OVL_SETRECT(0x6d63c, iVar8 + 0x3e, iVar4 + 1, iVar8 + 0x56, iVar4 + 0xb);
        }
        iVar8 = OVL_POS(0x900a4)->GetX();
        iVar4 = OVL_POS(0x900a4)->GetY();
        if (g_game_state_guess == 2)
        {
            for (local_14 = 0; local_14 < 8; local_14++)
            {
                OVL_SETIDX(local_14 + 0xd4, iVar8 + 8, iVar4 + 6 + local_14 * 0x1a, iVar8 + 0x26,
                           iVar4 + 0x1a + local_14 * 0x1a);
            }
        }
        else
        {
            for (local_14 = 0; local_14 < 8; local_14++)
            {
                OVL_SETIDX(local_14 + 0xd4, iVar8 + 0x12, iVar4 + 6 + local_14 * 0x26, iVar8 + 0x30,
                           iVar4 + 0x1e + local_14 * 0x26);
            }
        }
        iVar8 = OVL_POS(0x90044)->GetX();
        iVar4 = OVL_POS(0x90044)->GetY();
        iVar6 = OVL_POS(0x90044)->GetW();
        tmp = OVL_POS(0x90044)->GetH();
        OVL_SETRECT(0x6d8cc, iVar8, iVar4, iVar8 + iVar6, iVar4 + tmp);
        iVar8 = OVL_POS(0x90064)->GetX();
        iVar4 = OVL_POS(0x90064)->GetY();
        iVar6 = OVL_POS(0x90064)->GetW();
        OVL_SETRECT(0x6d8dc, iVar8, iVar4, iVar8 + iVar6, iVar4 + OVL_POS(0x90064)->GetH());
        OVL_SETRECT(0x6d8ec, iVar8 + 10, iVar4, iVar8 - 0x14 + iVar6, iVar4 + 0x15);
        OVL_SETRECT(0x6d8fc, iVar8 + 10, iVar4 + 0x16, iVar8 - 0x14 + iVar6, iVar4 + 0x2b);
        OVL_SETRECT(0x6d90c, iVar8 + 10, iVar4 + 0x2c, iVar8 - 0x14 + iVar6, iVar4 + 0x41);
        OVL_SETRECT(0x6d91c, iVar8 + 10, iVar4 + 0x42, iVar8 - 0x14 + iVar6, iVar4 + 0x57);
        OVL_SETRECT(0x6d92c, iVar8 + 10, iVar4 + 0x58, iVar8 - 0x14 + iVar6, iVar4 + 0x6e);
        if (g_game_state_guess != 2)
        {
            if (OVL_PC == 0 || *(char*)(OVL_PC + 0x36) == 1 || *(char*)(OVL_PC + 0x36) == 7 ||
                *(char*)(OVL_PC + 0x36) == 9)
            {
                OVL_SETRECT(0x6d6ec, 0xc, 0x41, 0x50, 0x100);
            }
            else
            {
                OVL_SETRECT(0x6d6ec, 0xc, 0x41, 0x50, 0xcd);
                OVL_SETRECT(0x6d6fc, 0xc, 0xce, 0x50, 0x112);
            }
        }
    }
    switch (*(char*)(self + 0xfe0))
    {
    case 0:
        if (g_game_state_guess == 2)
        {
            iVar8 = OVL_POS(0x90244)->GetX();
            iVar4 = OVL_POS(0x90244)->GetY();
            OVL_SETRECT(0x6d76c, iVar8 + 10, iVar4 + 0x4b, iVar8 + 0x6e, iVar4 + 0x5f);
            OVL_SETRECT(0x6d77c, iVar8 + 10, iVar4 + 0x5f, iVar8 + 0x6e, iVar4 + 0x73);
            OVL_SETRECT(0x6d78c, iVar8 + 10, iVar4 + 0x73, iVar8 + 0x6e, iVar4 + 0x87);
            OVL_SETRECT(0x6d79c, iVar8 + 10, iVar4 + 0x87, iVar8 + 0x6e, iVar4 + 0x9b);
            OVL_SETRECT(0x6d7ac, iVar8 + 10, iVar4 + 0x9b, iVar8 + 0x6e, iVar4 + 0xaf);
            OVL_SETRECT(0x6d7bc, iVar8 + 10, iVar4 + 0xaf, iVar8 + 0x6e, iVar4 + 0xc3);
        }
        else
        {
            OVL_SETRECT(0x6d76c, 0x212, 0x14b, 0x276, 0x15c);
            OVL_SETRECT(0x6d77c, 0x212, 0x162, 0x276, 0x173);
            OVL_SETRECT(0x6d78c, 0x212, 0x179, 0x276, 0x18a);
            OVL_SETRECT(0x6d79c, 0x212, 400, 0x276, 0x1a1);
            OVL_SETRECT(0x6d7ac, 0x212, 0x1a7, 0x276, 0x1b8);
            OVL_SETRECT(0x6d7bc, 0x212, 0x1be, 0x276, 0x1cf);
        }
        OVL_BTN(0xe6, 0x6d76c);
        OVL_BTN(0xe7, 0x6d77c);
        OVL_BTN(0xe8, 0x6d78c);
        OVL_BTN(0xe9, 0x6d79c);
        OVL_BTN(0xeb, 0x6d7bc);
        OVL_BTN(0xea, 0x6d7ac);
        break;
    case 1:
        if (g_game_state_guess == 2)
        {
            iVar8 = OVL_POS(0x90244)->GetX();
            iVar4 = OVL_POS(0x90244)->GetY();
            OVL_SETRECT(0x6d80c, iVar8 + 0xf, iVar4 + 0x45, iVar8 + 0x37, iVar4 + 0x6d);
            OVL_SETRECT(0x6d81c, iVar8 + 0x41, iVar4 + 0x45, iVar8 + 0x73, iVar4 + 0x6d);
            OVL_SETRECT(0x6d82c, iVar8 + 0xf, iVar4 + 0x72, iVar8 + 0x37, iVar4 + 0x9a);
            OVL_SETRECT(0x6d83c, iVar8 + 0x41, iVar4 + 0x72, iVar8 + 0x73, iVar4 + 0x9a);
            OVL_SETRECT(0x6d84c, iVar8 + 0xf, iVar4 + 0x9f, iVar8 + 0x37, iVar4 + 199);
            OVL_SETRECT(0x6d85c, iVar8 + 0x41, iVar4 + 0x9f, iVar8 + 0x73, iVar4 + 199);
        }
        else
        {
            OVL_SETRECT(0x6d80c, 0x216, 0x149, 0x23e, 0x171);
            OVL_SETRECT(0x6d81c, 0x247, 0x149, 0x26f, 0x171);
            OVL_SETRECT(0x6d82c, 0x216, 0x17a, 0x23e, 0x1a2);
            OVL_SETRECT(0x6d83c, 0x247, 0x17a, 0x26f, 0x1a2);
            OVL_SETRECT(0x6d84c, 0x216, 0x1ab, 0x23e, 0x1d2);
            OVL_SETRECT(0x6d85c, 0x247, 0x1ab, 0x26f, 0x1d2);
        }
        OVL_BTN(0xf0, 0x6d80c);
        OVL_BTN(0xf1, 0x6d81c);
        OVL_BTN(0xf2, 0x6d82c);
        OVL_BTN(0xf3, 0x6d83c);
        OVL_BTN(0xf4, 0x6d84c);
        OVL_BTN(0xf5, 0x6d85c);
        *(int*)(self + 0x685bc) = 0;
        for (local_14 = 0; local_14 < 100; local_14++)
            *(short*)(self + 0x6c65e + local_14 * 2) = -1;
        idx = 0;
        for (counter2 = 0; counter2 < 100; counter2++)
        {
            hasSkill = OVL_HASSKILL(counter2);
            if (hasSkill != 0 && *(char*)(EQ_ARRAY(int, 0x006E33A4)[counter2] + 0x25) != 0)
            {
                *(short*)(self + 0x6c65e + idx * 2) = counter2;
                idx++;
            }
        }
        for (counter2 = 0; counter2 < 0x19; counter2++)
        {
            if (*(u8*)(OVL_PC + 0xa2c + counter2) != 0xff)
            {
                if (counter2 == 5 || counter2 == 7 || counter2 == 0xb)
                {
                    *(short*)(self + 0x6c65e + idx * 2) = counter2 + 100;
                    idx++;
                }
                else if (counter2 == 0xe && 8 < g_pc_guess->level)
                {
                    *(short*)(self + 0x6c65e + idx * 2) = 0x72;
                    idx++;
                }
            }
        }
        if (g_pc_guess->innateSkills_guess[2] != 0xff)
            g_pc_guess->innateSkills_guess[2] = 0xff;
        *(int*)(self + 0x685bc) = (char)idx;
        if (*(int*)(self + 0x685d8) != 0)
        {
            iVar8 = EQ_GLOBAL(int, 0x006B63FC) / 2;
            iVar4 = iVar8 + 0x85;
            iVar6 = (EQ_GLOBAL(int, 0x006B6400) - 0x1e0) / 2;
            tmp = iVar6 + 0xfe;
            for (idx = 0; idx < *(int*)(self + 0x685bc); idx++)
            {
                gridRow = idx / 6;
                local_c = idx % 6;
                if (g_game_state_guess == 2)
                {
                    OVL_SETIDX(idx + 0x145, iVar4 + local_c * -0x42, tmp + gridRow * -0x42,
                               iVar4 + local_c * -0x42 + 0x41, tmp + gridRow * -0x42 + 0x41);
                }
                else
                {
                    OVL_SETIDX(idx + 0x145, local_c * -0x42 + 0x1c5, gridRow * -0x42 + 0xfe,
                               local_c * -0x42 + 0x206, gridRow * -0x42 + 0x13f);
                }
                OVL_BTNIDX(idx + 0x145, idx + 0x145);
            }
            if (*(int*)(self + 0x685bc) != 0)
            {
                if (g_game_state_guess == 2)
                {
                    OVL_SETRECT(0x6df3c, iVar8 + 0x6d + local_c * -0x42, iVar6 + 0xe6 + gridRow * -0x42,
                                iVar4 + local_c * -0x42, tmp + gridRow * -0x42);
                }
                else
                {
                    OVL_SETRECT(0x6df3c, local_c * -0x42 + 0x1ad, gridRow * -0x42 + 0xe6,
                                local_c * -0x42 + 0x1c5, gridRow * -0x42 + 0xfe);
                }
                OVL_BTN(0x163, 0x6df3c);
            }
        }
        break;
    case 2:
        if (g_game_state_guess == 2)
        {
            iVar8 = OVL_POS(0x90244)->GetX();
            iVar4 = OVL_POS(0x90244)->GetY();
            OVL_SETRECT(0x6d86c, iVar8 + 0xf, iVar4 + 0x45, iVar8 + 0x37, iVar4 + 0x6d);
            OVL_SETRECT(0x6d87c, iVar8 + 0x41, iVar4 + 0x45, iVar8 + 0x73, iVar4 + 0x6d);
            OVL_SETRECT(0x6d88c, iVar8 + 0xf, iVar4 + 0x72, iVar8 + 0x37, iVar4 + 0x9a);
            OVL_SETRECT(0x6d89c, iVar8 + 0x41, iVar4 + 0x72, iVar8 + 0x73, iVar4 + 0x9a);
            OVL_SETRECT(0x6d8ac, iVar8 + 0xf, iVar4 + 0x9f, iVar8 + 0x37, iVar4 + 199);
            OVL_SETRECT(0x6d8bc, iVar8 + 0x41, iVar4 + 0x9f, iVar8 + 0x73, iVar4 + 199);
        }
        else
        {
            OVL_SETABS(0x70a6c, 0x216, 0x149, 0x23e, 0x171);
            OVL_SETABS(0x70a7c, 0x247, 0x149, 0x26f, 0x171);
            OVL_SETABS(0x70a8c, 0x216, 0x17a, 0x23e, 0x1a2);
            OVL_SETABS(0x70a9c, 0x247, 0x17a, 0x26f, 0x1a2);
            OVL_SETABS(0x70aac, 0x216, 0x1ab, 0x23e, 0x1d2);
            OVL_SETABS(0x70abc, 0x247, 0x1ab, 0x26f, 0x1d2);
        }
        OVL_BTN(0xf6, 0x6d86c);
        OVL_BTN(0xf7, 0x6d87c);
        OVL_BTN(0xf8, 0x6d88c);
        OVL_BTN(0xf9, 0x6d89c);
        OVL_BTN(0xfa, 0x6d8ac);
        OVL_BTN(0xfb, 0x6d8bc);
        *(int*)(self + 0x685b8) = 0;
        for (idx = 0; idx < 100; idx++)
            *(short*)(self + 0x6c65e + idx * 2) = -1;
        idx = 0;
        for (counter2 = 0; counter2 < 100; counter2++)
        {
            hasSkill = OVL_HASSKILL(counter2);
            if (hasSkill != 0 && *(char*)(EQ_ARRAY(int, 0x006E33A4)[counter2] + 0x25) != 0 &&
                *(char*)(EQ_ARRAY(int, 0x006E33A4)[counter2] + 0x26) != 0)
            {
                *(short*)(self + 0x6c65e + idx * 2) = counter2;
                idx++;
            }
        }
        for (idx2 = 0; idx2 < 0x19; idx2++)
        {
            if (*(u8*)(OVL_PC + 0xa2c + idx2) != 0xff)
            {
                if (idx2 == 5 || idx2 == 7 || idx2 == 0xb)
                {
                    *(short*)(self + 0x6c65e + idx * 2) = idx2 + 100;
                    idx++;
                }
                else if (idx2 == 0xe && 8 < g_pc_guess->level)
                {
                    *(short*)(self + 0x6c65e + idx * 2) = 0x72;
                    idx++;
                }
            }
        }
        *(int*)(self + 0x685b8) = (char)idx;
        if (*(int*)(self + 0x685d4) != 0)
        {
            iVar8 = EQ_GLOBAL(int, 0x006B63FC) / 2;
            iVar4 = iVar8 + 0x85;
            iVar6 = (EQ_GLOBAL(int, 0x006B6400) - 0x1e0) / 2;
            tmp = iVar6 + 0xfe;
            for (idx = 0; idx < *(int*)(self + 0x685b8); idx++)
            {
                gridRow = idx / 6;
                local_c = idx % 6;
                if (g_game_state_guess == 2)
                {
                    OVL_SETIDX(idx + 0x145, iVar4 + local_c * -0x42, tmp + gridRow * -0x42,
                               iVar4 + local_c * -0x42 + 0x41, tmp + gridRow * -0x42 + 0x41);
                }
                else
                {
                    OVL_SETIDX(idx + 0x145, local_c * -0x42 + 0x1c5, gridRow * -0x42 + 0xfe,
                               local_c * -0x42 + 0x206, gridRow * -0x42 + 0x13f);
                }
                OVL_BTNIDX(idx + 0x145, idx + 0x145);
            }
            if (*(int*)(self + 0x685b8) != 0)
            {
                if (g_game_state_guess == 2)
                {
                    OVL_SETRECT(0x6df3c, iVar8 + 0x6d + local_c * -0x42, iVar6 + 0xe6 + gridRow * -0x42,
                                iVar4 + local_c * -0x42, tmp + gridRow * -0x42);
                }
                else
                {
                    OVL_SETRECT(0x6df3c, local_c * -0x42 + 0x1ad, gridRow * -0x42 + 0xe6,
                                local_c * -0x42 + 0x1c5, gridRow * -0x42 + 0xfe);
                }
                OVL_BTN(0x163, 0x6df3c);
            }
        }
        break;
    case 3:
        if (g_game_state_guess == 2)
        {
            iVar8 = OVL_POS(0x90244)->GetX();
            iVar4 = OVL_POS(0x90244)->GetY();
            for (local_14 = 0; local_14 < 6; local_14++)
            {
                OVL_SETABSIDX(0x7140c, local_14 + 0x39, iVar8 + 0xc, iVar4 + 0x50 + local_14 * 0x14, iVar8 + 0x3a,
                              iVar4 + 0x61 + local_14 * 0x14);
                OVL_SETABSIDX(0x7140c, local_14 + 0x3f, iVar8 + 0x40, iVar4 + 0x50 + local_14 * 0x14, iVar8 + 0x6e,
                              iVar4 + 0x61 + local_14 * 0x14);
            }
            OVL_SETRECT(0x6cdec, iVar8 + 0x20, iVar4 + 0x42, iVar8 + 0x30, iVar4 + 0x4c);
            OVL_SETRECT(0x6cdfc, iVar8 + 0x48, iVar4 + 0x42, iVar8 + 0x58, iVar4 + 0x4c);
            iVar8 = EQ_GLOBAL(int, 0x006B63FC) / 2;
            iVar4 = EQ_GLOBAL(int, 0x006B6400) / 2;
            OVL_SETABS(0x7185c, iVar8 - 0x6e, iVar4 - 0x46, iVar8 - 0x30, iVar4 - 0x37);
            OVL_SETABS(0x7186c, iVar8 + 0x5c, iVar4 - 0x46, iVar8 + 0x6e, iVar4 - 0x37);
            OVL_SETABS(0x7187c, iVar8 - 0xca, iVar4 - 0x32, iVar8 + 200, iVar4 - 0x20);
            OVL_SETABS(0x7188c, iVar8 - 0xca, iVar4 - 0x1e, iVar8 + 200, iVar4 - 0xc);
            OVL_SETABS(0x7189c, iVar8 - 0xca, iVar4 - 10, iVar8 + 200, iVar4 + 8);
            OVL_SETABS(0x718ac, iVar8 - 0xca, iVar4 + 10, iVar8 + 200, iVar4 + 0x1c);
            OVL_SETABS(0x718bc, iVar8 - 0xca, iVar4 + 0x1e, iVar8 + 200, iVar4 + 0x30);
            OVL_SETABS(0x718cc, iVar8 - 0xca, iVar4 + 0x34, iVar8 - 0x9c, iVar4 + 0x45);
            OVL_SETABS(0x718dc, iVar8 + 0x9a, iVar4 + 0x34, iVar8 + 200, iVar4 + 0x45);
        }
        else
        {
            for (local_14 = 0; local_14 < 6; local_14++)
            {
                OVL_SETABSIDX(0x7140c, local_14 + 0x39, 0x212, local_14 * 0x17 + 0x14b, 0x240,
                              local_14 * 0x17 + 0x15c);
                OVL_SETABSIDX(0x7140c, local_14 + 0x3f, 0x246, local_14 * 0x17 + 0x14b, 0x274,
                              local_14 * 0x17 + 0x15c);
            }
            OVL_SETABS(0x7185c, 0xd1, 0x14a, 0x10f, 0x159);
            OVL_SETABS(0x7186c, 0x19b, 0x149, 0x1ad, 0x15b);
            OVL_SETABS(0x7187c, 0x75, 0x15e, 0x207, 0x170);
            OVL_SETABS(0x7188c, 0x75, 0x172, 0x207, 0x184);
            OVL_SETABS(0x7189c, 0x75, 0x186, 0x207, 0x198);
            OVL_SETABS(0x718ac, 0x75, 0x19a, 0x207, 0x1ac);
            OVL_SETABS(0x718bc, 0x75, 0x1ae, 0x207, 0x1c0);
            OVL_SETABS(0x718cc, 0x75, 0x1c4, 0xa3, 0x1d5);
            OVL_SETABS(0x718dc, 0x1d9, 0x1c4, 0x207, 0x1d5);
            OVL_SETRECT(0x6cdec, 0x226, 0x13d, 0x236, 0x147);
            OVL_SETRECT(0x6cdfc, 0x24e, 0x13d, 0x25e, 0x147);
        }
        for (idx = 0; idx < 6; idx++)
        {
            OVL_BTNIDX(idx + 0x39, idx + 0x39);
            OVL_BTNIDX(idx + 0x3f, idx + 0x3f);
        }
        OVL_BTN(0x4e, 0x6cdec);
        OVL_BTN(0x4f, 0x6cdfc);
        if (*(int*)(self + 0x5f18c) != 0)
        {
            OVL_BTN(0x45, 0x6cd5c);
            OVL_BTN(0x46, 0x6cd6c);
            OVL_BTN(0x47, 0x6cd7c);
            OVL_BTN(0x48, 0x6cd8c);
            OVL_BTN(0x49, 0x6cd9c);
            OVL_BTN(0x4a, 0x6cdac);
            OVL_BTN(0x4b, 0x6cdbc);
            OVL_BTN(0x4c, 0x6cdcc);
            OVL_BTN(0x4d, 0x6cddc);
        }
        break;
    case 4:
        if (g_game_state_guess == 2 && g_pcloudFlagA_guess != 0)
        {
            iVar8 = OVL_POS(0x90184)->GetX();
            iVar4 = OVL_POS(0x90184)->GetY();
            OVL_POS(0x90184)->GetW();
            OVL_POS(0x90184)->GetH();
            OVL_SETABS(0x72d1c, iVar8 + 0x28, iVar4, iVar8 + 0x50, iVar4 + 0x28);
            OVL_SETABS(0x72d5c, iVar8 + 0x50, iVar4, iVar8 + 0x78, iVar4 + 0x28);
            OVL_SETABS(0x72d3c, iVar8 + 0x78, iVar4, iVar8 + 0xa0, iVar4 + 0x28);
            OVL_SETABS(0x72d2c, iVar8 + 0xa0, iVar4, iVar8 + 200, iVar4 + 0x28);
            OVL_SETABS(0x72d4c, iVar8 + 200, iVar4, iVar8 + 0xf0, iVar4 + 0x28);
            OVL_SETABS(0x72dfc, iVar8, iVar4 + 0x28, iVar8 + 0x28, iVar4 + 0x50);
            OVL_SETABS(0x72d9c, iVar8 + 0x28, iVar4 + 0x28, iVar8 + 0x50, iVar4 + 0x50);
            OVL_SETABS(0x72d7c, iVar8 + 0x50, iVar4 + 0x28, iVar8 + 0x78, iVar4 + 0x50);
            OVL_SETABS(0x72dcc, iVar8 + 0x78, iVar4 + 0x28, iVar8 + 0xa0, iVar4 + 0x50);
            OVL_SETABS(0x72dac, iVar8 + 0xa0, iVar4 + 0x28, iVar8 + 200, iVar4 + 0x50);
            OVL_SETABS(0x72e0c, iVar8 + 200, iVar4 + 0x28, iVar8 + 0xf0, iVar4 + 0x50);
            OVL_SETABS(0x72d6c, iVar8, iVar4 + 0x50, iVar8 + 0x28, iVar4 + 0x78);
            OVL_SETABS(0x72e1c, iVar8 + 0x28, iVar4 + 0x50, iVar8 + 0x50, iVar4 + 0x78);
            OVL_SETABS(0x72d8c, iVar8 + 0x50, iVar4 + 0x50, iVar8 + 0x78, iVar4 + 0x78);
            OVL_SETABS(0x72e4c, iVar8 + 0x78, iVar4 + 0x50, iVar8 + 0xa0, iVar4 + 0x78);
            OVL_SETABS(0x72e2c, iVar8 + 0xa0, iVar4 + 0x50, iVar8 + 200, iVar4 + 0x78);
            OVL_SETABS(0x72e3c, iVar8 + 200, iVar4 + 0x50, iVar8 + 0xf0, iVar4 + 0x78);
            OVL_SETABS(0x72ddc, iVar8 + 0x50, iVar4 + 0x78, iVar8 + 0x78, iVar4 + 0xa0);
            OVL_SETABS(0x72dec, iVar8 + 0x78, iVar4 + 0x78, iVar8 + 0xa0, iVar4 + 0xa0);
            OVL_SETABS(0x72dbc, iVar8 + 0xa0, iVar4 + 0x78, iVar8 + 200, iVar4 + 0xa0);
            OVL_SETABS(0x72e5c, iVar8 + 200, iVar4 + 0x78, iVar8 + 0xf0, iVar4 + 0xa0);
            OVL_BTN(1, 0x6c91c);
            OVL_BTN(2, 0x6c92c);
            OVL_BTN(3, 0x6c93c);
            OVL_BTN(4, 0x6c94c);
            OVL_BTN(5, 0x6c95c);
            OVL_BTN(0xf, 0x6c9fc);
            OVL_BTN(9, 0x6c99c);
            OVL_BTN(7, 0x6c97c);
            OVL_BTN(0xc, 0x6c9cc);
            OVL_BTN(10, 0x6c9ac);
            OVL_BTN(0x10, 0x6ca0c);
            OVL_BTN(6, 0x6c96c);
            OVL_BTN(0x11, 0x6ca1c);
            OVL_BTN(8, 0x6c98c);
            OVL_BTN(0x14, 0x6ca4c);
            OVL_BTN(0x12, 0x6ca2c);
            OVL_BTN(0x13, 0x6ca3c);
            OVL_BTN(0xd, 0x6c9dc);
            OVL_BTN(0xe, 0x6c9ec);
            OVL_BTN(0xb, 0x6c9bc);
            OVL_BTN(0x15, 0x6ca5c);
            for (local_14 = 0; local_14 < 4; local_14++)
            {
                OVL_SETABSIDX(0x72d0c, local_14 + 0x16, iVar8 + 0xa0, iVar4 + 0xaa + local_14 * 0x28, iVar8 + 200,
                              iVar4 + 0xd2 + local_14 * 0x28);
                OVL_SETABSIDX(0x72d0c, local_14 + 0x1a, iVar8 + 200, iVar4 + 0xaa + local_14 * 0x28, iVar8 + 0xf0,
                              iVar4 + 0xd2 + local_14 * 0x28);
            }
            OVL_SETABS(0x72f3c, iVar8 + 0x50, iVar4 + 0xaa, iVar8 + 0x96, iVar4 + 0xd2);
            OVL_SETABS(0x72f4c, iVar8 + 0x50, iVar4 + 0xd2, iVar8 + 0x96, iVar4 + 0xfa);
            OVL_SETABS(0x72f5c, iVar8 + 0x50, iVar4 + 0xfa, iVar8 + 0x96, iVar4 + 0x122);
            OVL_SETABS(0x72f6c, iVar8 + 0x50, iVar4 + 0x122, iVar8 + 0x96, iVar4 + 0x14a);
            OVL_SETABS(0x72f0c, iVar8 + 3, iVar4 + 0xa0, iVar8 + 0x4d, iVar4 + 300);
            OVL_SETABS(0x72f1c, iVar8 + 0xfa, iVar4 + 0x17a, iVar8 + 0x140, iVar4 + 0x18e);
            OVL_SETABS(0x72f2c, iVar8 + 10, iVar4 + 0x17a, iVar8 + 0x50, iVar4 + 0x18e);
            OVL_SETABS(0x72eec, iVar8 + 0xf0, iVar4, iVar8 + 0x122, iVar4 + 0x14);
            OVL_SETABS(0x72efc, iVar8 + 0x122, iVar4, iVar8 + 0x154, iVar4 + 0x14);
            OVL_SETABS(0x7458c, iVar8 + 0x100, iVar4 + 0x1a, iVar8 + 0x148, iVar4 + 0x53);
            OVL_SETABS(0x7459c, iVar8 + 0x100, iVar4 + 0x53, iVar8 + 0x148, iVar4 + 0x8c);
            OVL_SETABS(0x745ac, iVar8 + 0x100, iVar4 + 0x8c, iVar8 + 0x148, iVar4 + 0xc5);
            OVL_SETABS(0x745bc, iVar8 + 0x100, iVar4 + 0xc5, iVar8 + 0x148, iVar4 + 0xfe);
            OVL_SETABS(0x745cc, iVar8 + 0x100, iVar4 + 0xfe, iVar8 + 0x148, iVar4 + 0x137);
            OVL_SETABS(0x745dc, iVar8 + 0x100, iVar4 + 0x137, iVar8 + 0x148, iVar4 + 0x170);
            OVL_SETABS(0x73bec, 0x70, 0x147, 0x208, 0x1e0);
            OVL_SETABS(0x72f8c, 0x61, 0x23, 0x6c, 0x35);
            OVL_SETABS(0x72f9c, 0x61, 0x1c1, 0x6c, 0x1d3);
            OVL_SETABS(0x72fac, 0x61, 0x35, 0x6b, 0x40);
            OVL_SETABS(0x73f3c, 0x10, 0x1b2, 0x36, 0x1d2);
            OVL_SETABS(0x73f4c, 0x41, 0x1b2, 0x67, 0x1d2);
            for (local_14 = 0; local_14 <= 7; local_14++)
            {
                OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x113, 0xb, local_14 * 0x31 + 0x25, 0x39,
                              local_14 * 0x31 + 0x53);
                OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x114, 0x3c, local_14 * 0x31 + 0x25, 0x6a,
                              local_14 * 0x31 + 0x53);
            }
            OVL_SETABS(0x73f5c, 0x1c, 0x1b, 0x5a, 0x31);
            OVL_SETABS(0x73f6c, 0x1c, 0x19b, 0x5a, 0x1b1);
            OVL_SETABS(0x73fcc, 0xc, 0x23, 0x3a, 0x51);
            OVL_SETABS(0x73fdc, 0x3d, 0x23, 0x6b, 0x51);
            OVL_SETABS(0x73fec, 0xc, 0x54, 0x3a, 0x82);
            OVL_SETABS(0x73ffc, 0x3d, 0x54, 0x6b, 0x82);
            OVL_SETABS(0x7400c, 0xc, 0x85, 0x3a, 0xb3);
            OVL_SETABS(0x7401c, 0x3d, 0x85, 0x6b, 0xb3);
            OVL_SETABS(0x7402c, 0xc, 0xb6, 0x3a, 0xe4);
            OVL_SETABS(0x7403c, 0x3d, 0xb6, 0x6b, 0xe4);
            OVL_SETABS(0x744bc, 10, 0xeb, 0x6e, 0x13e);
            OVL_SETABS(0x7413c, 0x2d, 0x164, 0x54, 0x18b);
            OVL_SETABS(0x7414c, 0x24, 0x1ae, 0x5f, 0x1c9);
            OVL_SETABS(0x7406c, 0x76, 0x23, 0xa4, 0x51);
            OVL_SETABS(0x7407c, 0xa7, 0x23, 0xd5, 0x51);
            OVL_SETABS(0x7408c, 0x76, 0x54, 0xa4, 0x82);
            OVL_SETABS(0x7409c, 0xa7, 0x54, 0xd5, 0x82);
            OVL_SETABS(0x740ac, 0x76, 0x85, 0xa4, 0xb3);
            OVL_SETABS(0x740bc, 0xa7, 0x85, 0xd5, 0xb3);
            OVL_SETABS(0x740cc, 0x76, 0xb6, 0xa4, 0xe4);
            OVL_SETABS(0x740dc, 0xa7, 0xb6, 0xd5, 0xe4);
            OVL_SETABS(0x740ec, 0xc0, 0xf4, 0xcf, 0x103);
            OVL_SETABS(0x740fc, 0xc0, 0x105, 0xcf, 0x114);
            OVL_SETABS(0x7410c, 0xc0, 0x115, 0xcf, 0x125);
            OVL_SETABS(0x7411c, 0xc0, 0x126, 0xcf, 0x135);
            OVL_SETABS(0x7412c, 0x73, 0xeb, 0xd7, 0x13e);
            OVL_SETABS(0x737ac, 0xc, 0x5a, 0x3a, 0x88);
            OVL_SETABS(0x737bc, 0xc, 0x8b, 0x3a, 0xb9);
            OVL_SETABS(0x737cc, 0xc, 0xbc, 0x3a, 0xea);
            OVL_SETABS(0x737dc, 0xc, 0xed, 0x3a, 0x11b);
            OVL_SETABS(0x737ec, 0x3d, 0x5a, 0x6b, 0x88);
            OVL_SETABS(0x737fc, 0x3d, 0x8b, 0x6b, 0xb9);
            OVL_SETABS(0x7380c, 0x3d, 0xbc, 0x6b, 0xea);
            OVL_SETABS(0x7381c, 0x3d, 0xed, 0x6b, 0x11b);
            OVL_SETABS(0x7384c, 7, 0x142, 0x67, 0x154);
            OVL_SETABS(0x7385c, 7, 0x165, 0x67, 0x177);
            OVL_SETABS(0x7386c, 7, 0x189, 0x67, 0x19b);
            OVL_SETABS(0x7387c, 7, 0x1ad, 0x67, 0x1bf);
            OVL_SETABS(0x7388c, 0, 0, 0x77, 0x1e0);
            OVL_SETABS(0x7447c, 0x111, 0, 0x20d, 0x40);
            OVL_SETABS(0x7448c, 0xe0, 0x41, 0x20d, 0x80);
            OVL_SETABS(0x7449c, 0xe0, 0x82, 0x20d, 0xc1);
            OVL_SETABS(0x744ac, 0x142, 0xc3, 0x20d, 0x102);
            OVL_SETABS(0x7442c, 0, 0, 0x77, 0x1e0);
            OVL_SETABS(0x7443c, 0, 0, 0xf0, 0x1e0);
            if (*(u8*)(self + 0x100c) == 0)
            {
                OVL_BTN(0x23, 0x6cb3c);
                OVL_BTN(0x24, 0x6cb4c);
                OVL_BTN(0x25, 0x6cb5c);
                OVL_BTN(0x26, 0x6cb6c);
            }
            if (*(int*)(self + 0x685e8) != 0)
            {
                for (local_14 = 0; local_14 < 8; local_14++)
                    OVL_BTNIDX(local_14 + 0xaa, local_14 + 0xaa);
                for (local_14 = 0; local_14 < 4; local_14++)
                    OVL_BTNIDX(local_14 + 0xb4, local_14 + 0xb4);
                OVL_BTN(0xb8, 0x6d48c);
            }
            if (*(u8*)(self + 0x100c) == 0)
            {
                for (local_14 = 0; local_14 < 8; local_14++)
                    OVL_BTNIDX(local_14 + 0x16, local_14 + 0x16);
            }
            else
            {
                OVL_SETRECT(0x6df5c, iVar8 + 0xa0, iVar4 + 0xaa, iVar8 + 0xf0, iVar4 + 0xb9);
                OVL_SETRECT(0x6df6c, iVar8 + 0xa0, iVar4 + 0xbe, iVar8 + 0xf0, iVar4 + 0x186);
            }
            if (*(int*)(self + 0x685e0) == 0 && *(int*)(self + 0x685dc) == 0 && *(int*)(self + 0x685e8) == 0)
            {
                OVL_BTN(0x1e, 0x6caec);
                OVL_BTN(0x1f, 0x6cafc);
            }
            if (*(u8*)(self + 0x100c) == 0)
            {
                EQ_METHOD(7, void, 0x00450316, (char*, short, RECT*, int, int, int, int, int), self)(0x20, OVL_RECT(0x6cb0c), -1, -1, 0, 0, 1);
                OVL_BTN(0x21, 0x6cb1c);
            }
            OVL_BTN(0x22, 0x6cb2c);
            if (*(int*)(self + 0x685e0) == 0)
            {
                if (*(int*)(self + 0x685dc) == 0)
                {
                    if (*(int*)(self + 0x685e8) == 0 && *(int*)(self + 0x8fb90) != 0)
                    {
                        iVar8 = OVL_POS(0x90184)->GetX();
                        iVar4 = OVL_POS(0x90184)->GetY();
                        OVL_SETABS(0x72f8c, iVar8 + 0x14a, iVar4 + 0x23, iVar8 + 0x154, iVar4 + 0x35);
                        OVL_BTN(0x28, 0x6cb8c);
                        OVL_SETABS(0x72f9c, iVar8 + 0x14a, iVar4 + 0x17c, iVar8 + 0x154, iVar4 + 0x197);
                        OVL_BTN(0x29, 0x6cb9c);
                        OVL_SETABS(0x72fac, iVar8 + 0x14a, iVar4 + 0x35, iVar8 + 0x154, iVar4 + 0x40);
                        OVL_BTN(0x2a, 0x6cbac);
                    }
                }
                else
                {
                    *EQ_GLOBAL(int*, 0x006B3804) =
                        *(int*)(EQ_GLOBAL(char*, 0x006E4E34) + 4);
                    SetRect((RECT*)(EQ_GLOBAL(int*, 0x006B3804) + 1), 0x19f, 0, 0x216, 0x1e0);
                    OVL_SETABS(0x73f3c, 0x10, 0x1b2, 0x36, 0x1d2);
                    OVL_BTN(0x123, 0x6db3c);
                    OVL_SETABS(0x73f4c, 0x41, 0x1b2, 0x67, 0x1d2);
                    OVL_BTN(0x124, 0x6db4c);
                    OVL_SETABS(0x73f5c, 0x1c, 0x1b, 0x5a, 0x31);
                    OVL_SETABS(0x73f6c, 0x1c, 0x19b, 0x5a, 0x1b1);
                    for (local_14 = 0; local_14 <= 7; local_14++)
                    {
                        OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x113, 0xb, local_14 * 0x31 + 0x25, 0x39,
                                      local_14 * 0x31 + 0x53);
                        OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x114, 0x3c, local_14 * 0x31 + 0x25, 0x6a,
                                      local_14 * 0x31 + 0x53);
                    }
                    for (local_14 = 0; local_14 < 0x10; local_14++)
                        OVL_BTNIDX(local_14 + 0x113, local_14 + 0x113);
                }
            }
            else
            {
                *(int*)(self + 0xac) = 0;
                *(int*)(self + 0xb0) = 0;
                *EQ_GLOBAL(int*, 0x006B3804) =
                    *(int*)(EQ_GLOBAL(char*, 0x006E4E24) + 4);
                SetRect((RECT*)(EQ_GLOBAL(int*, 0x006B3804) + 1), 0xf0, 0, 0x167, 0x1e0);
                OVL_SETABS(0x73e0c, 0xf, 0x156, 0x35, 0x176);
                OVL_BTN(0x110, 0x6da0c);
                OVL_SETABS(0x73e1c, 0x40, 0x156, 0x66, 0x176);
                OVL_BTN(0x111, 0x6da1c);
                for (local_14 = 0; local_14 < 6; local_14++)
                {
                    OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x104, 0xb, local_14 * 0x31 + 0x2d, 0x39,
                                  local_14 * 0x31 + 0x5b);
                    OVL_BTNIDX(local_14 * 2 + 0x104, local_14 * 2 + 0x104);
                    OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x105, 0x3c, local_14 * 0x31 + 0x2d, 0x6a,
                                  local_14 * 0x31 + 0x5b);
                    OVL_BTNIDX(local_14 * 2 + 0x105, local_14 * 2 + 0x105);
                }
                OVL_SETRECT(0x6d93c, 0x25, 399, 0x53, 0x1bd);
                OVL_SETRECT(0x6da2c, 0x16, 0x1c2, 99, 0x1d3);
                OVL_BTN(0x112, 0x6da2c);
                OVL_SETABS(0x73f5c, 0x1c, 0x2d, 0x5a, 0x43);
                OVL_SETABS(0x73f6c, 0x1c, 0x13b, 0x5a, 0x151);
            }
            if (*(int*)(self + 0x685e4) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006E4D88) == 0 || *(char*)(EQ_GLOBAL(char*, 0x006E4D88) + 0x98) != 1)
                {
                    OVL_SETABS(0x73fcc, 0xc, 0x23, 0x3a, 0x51);
                    OVL_SETABS(0x73fdc, 0x3d, 0x23, 0x6b, 0x51);
                    OVL_SETABS(0x73fec, 0xc, 0x54, 0x3a, 0x82);
                    OVL_SETABS(0x73ffc, 0x3d, 0x54, 0x6b, 0x82);
                    OVL_SETABS(0x7400c, 0xc, 0x85, 0x3a, 0xb3);
                    OVL_SETABS(0x7401c, 0x3d, 0x85, 0x6b, 0xb3);
                    OVL_SETABS(0x7402c, 0xc, 0xb6, 0x3a, 0xe4);
                    OVL_SETABS(0x7403c, 0x3d, 0xb6, 0x6b, 0xe4);
                    OVL_SETABS(0x744bc, 10, 0xeb, 0x6e, 0x13e);
                    OVL_SETABS(0x7413c, 0x2d, 0x164, 0x54, 0x18b);
                    OVL_SETABS(0x7414c, 0x24, 0x1ae, 0x5f, 0x1c9);
                    OVL_SETABS(0x7406c, 0x76, 0x23, 0xa4, 0x51);
                    OVL_SETABS(0x7407c, 0xa7, 0x23, 0xd5, 0x51);
                    OVL_SETABS(0x7408c, 0x76, 0x54, 0xa4, 0x82);
                    OVL_SETABS(0x7409c, 0xa7, 0x54, 0xd5, 0x82);
                    OVL_SETABS(0x740ac, 0x76, 0x85, 0xa4, 0xb3);
                    OVL_SETABS(0x740bc, 0xa7, 0x85, 0xd5, 0xb3);
                    OVL_SETABS(0x740cc, 0x76, 0xb6, 0xa4, 0xe4);
                    OVL_SETABS(0x740dc, 0xa7, 0xb6, 0xd5, 0xe4);
                    OVL_SETABS(0x740ec, 0xc0, 0xf4, 0xcf, 0x103);
                    OVL_SETABS(0x740fc, 0xc0, 0x105, 0xcf, 0x114);
                    OVL_SETABS(0x7410c, 0xc0, 0x115, 0xcf, 0x125);
                    OVL_SETABS(0x7411c, 0xc0, 0x126, 0xcf, 0x135);
                    OVL_SETABS(0x7412c, 0x73, 0xeb, 0xd7, 0x13e);
                    for (local_14 = 300; local_14 < 0x145; local_14++)
                        OVL_BTNIDX(local_14, local_14);
                    OVL_BTN(0x17b, 0x6e0bc);
                }
                else
                {
                    OVL_SETABS(0x7413c, 0x26, 0x175, 0x4d, 0x19c);
                    OVL_BTNABS(0x143, 0x7413c);
                    OVL_SETABS(0x7414c, 0x1c, 0x1b2, 0x58, 0x1cd);
                    OVL_BTNABS(0x144, 0x7414c);
                    OVL_SETABS(0x7406c, 0x24, 0x27, 0x52, 0x55);
                    OVL_BTNABS(0x136, 0x7406c);
                    OVL_SETABS(0x7407c, 0x24, 0x5d, 0x52, 0x8b);
                    OVL_BTNABS(0x137, 0x7407c);
                    OVL_SETABS(0x7408c, 0x24, 0x93, 0x52, 0xc1);
                    OVL_BTNABS(0x138, 0x7408c);
                    OVL_SETABS(0x7409c, 0x24, 0xc9, 0x52, 0xf7);
                    OVL_BTNABS(0x139, 0x7409c);
                    OVL_SETABS(0x740ec, 9, 0x106, 0x6d, 0x11f);
                    OVL_BTNABS(0x13e, 0x740ec);
                    OVL_SETABS(0x740fc, 9, 0x120, 0x6d, 0x130);
                    OVL_BTNABS(0x13f, 0x740fc);
                    OVL_SETABS(0x7410c, 9, 0x131, 0x6d, 0x140);
                    OVL_BTNABS(0x140, 0x7410c);
                    OVL_SETABS(0x7411c, 9, 0x141, 0x6d, 0x15a);
                    OVL_BTNABS(0x141, 0x7411c);
                    OVL_SETABS(0x7412c, 9, 0x106, 0x6d, 0x15a);
                    OVL_BTNABS(0x142, 0x7412c);
                }
            }
        }
        else
        {
            OVL_SETABS(0x72d1c, 0x11e, 0xc, 0x146, 0x34);
            OVL_SETABS(0x72d2c, 0x1b1, 0xc, 0x1d9, 0x34);
            OVL_SETABS(0x72d3c, 0x180, 0xc, 0x1a8, 0x34);
            OVL_SETABS(0x72d4c, 0x1e2, 0xc, 0x20a, 0x34);
            OVL_SETABS(0x72d5c, 0x14f, 0xc, 0x177, 0x34);
            OVL_SETABS(0x72dfc, 0xed, 0x4d, 0x115, 0x75);
            OVL_SETABS(0x72d9c, 0x11e, 0x4d, 0x146, 0x75);
            OVL_SETABS(0x72d7c, 0x14f, 0x4d, 0x177, 0x75);
            OVL_SETABS(0x72dcc, 0x180, 0x4d, 0x1a8, 0x75);
            OVL_SETABS(0x72dac, 0x1b1, 0x4d, 0x1d9, 0x75);
            OVL_SETABS(0x72e0c, 0x1e2, 0x4d, 0x20a, 0x75);
            OVL_SETABS(0x72d6c, 0xed, 0x8e, 0x115, 0xb6);
            OVL_SETABS(0x72e1c, 0x11e, 0x8e, 0x146, 0xb6);
            OVL_SETABS(0x72d8c, 0x14f, 0x8e, 0x177, 0xb6);
            OVL_SETABS(0x72e4c, 0x180, 0x8e, 0x1a8, 0xb6);
            OVL_SETABS(0x72e2c, 0x1b1, 0x8e, 0x1d9, 0xb6);
            OVL_SETABS(0x72e3c, 0x1e2, 0x8e, 0x20a, 0xb6);
            OVL_SETABS(0x72ddc, 0x14f, 0xcf, 0x177, 0xf7);
            OVL_SETABS(0x72dec, 0x180, 0xcf, 0x1a8, 0xf7);
            OVL_SETABS(0x72dbc, 0x1b1, 0xcf, 0x1d9, 0xf7);
            OVL_SETABS(0x72e5c, 0x1e2, 0xcf, 0x20a, 0xf7);
            for (local_14 = 0; local_14 < 4; local_14++)
            {
                OVL_SETABSIDX(0x72d0c, local_14 + 0x16, 0x218, local_14 * 0x31 + 0x101, 0x240,
                              local_14 * 0x31 + 0x129);
                OVL_SETABSIDX(0x72d0c, local_14 + 0x1a, 0x249, local_14 * 0x31 + 0x101, 0x271,
                              local_14 * 0x31 + 0x129);
            }
            OVL_SETABS(0x72eec, 10, 0xc, 0x37, 0x1d);
            OVL_SETABS(0x72efc, 0x3e, 0xc, 0x6b, 0x1d);
            OVL_SETABS(0x72f0c, 0x211, 0x1c, 0x277, 0xe3);
            OVL_SETABS(0x72f1c, 0x21e, 0xe8, 0x26b, 0xf9);
            OVL_SETABS(0x72f2c, 0x22d, 0x1c3, 0x25b, 0x1d4);
            OVL_SETABS(0x72f7c, 0x212, 8, 0x276, 0x19);
            OVL_SETABS(0x73bec, 0x70, 0x147, 0x208, 0x1e0);
            OVL_SETABS(0x72f8c, 0x61, 0x23, 0x6c, 0x35);
            OVL_SETABS(0x72f9c, 0x61, 0x1c1, 0x6c, 0x1d3);
            OVL_SETABS(0x72fac, 0x61, 0x35, 0x6b, 0x40);
            OVL_SETABS(0x73f3c, 0x10, 0x1b2, 0x36, 0x1d2);
            OVL_SETABS(0x73f4c, 0x41, 0x1b2, 0x67, 0x1d2);
            for (local_14 = 0; local_14 <= 7; local_14++)
            {
                OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x113, 0xb, local_14 * 0x31 + 0x25, 0x39,
                              local_14 * 0x31 + 0x53);
                OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x114, 0x3c, local_14 * 0x31 + 0x25, 0x6a,
                              local_14 * 0x31 + 0x53);
            }
            OVL_SETABS(0x73f5c, 0x1c, 0x1b, 0x5a, 0x31);
            OVL_SETABS(0x73f6c, 0x1c, 0x19b, 0x5a, 0x1b1);
            OVL_SETABS(0x73fcc, 0xc, 0x23, 0x3a, 0x51);
            OVL_SETABS(0x73fdc, 0x3d, 0x23, 0x6b, 0x51);
            OVL_SETABS(0x73fec, 0xc, 0x54, 0x3a, 0x82);
            OVL_SETABS(0x73ffc, 0x3d, 0x54, 0x6b, 0x82);
            OVL_SETABS(0x7400c, 0xc, 0x85, 0x3a, 0xb3);
            OVL_SETABS(0x7401c, 0x3d, 0x85, 0x6b, 0xb3);
            OVL_SETABS(0x7402c, 0xc, 0xb6, 0x3a, 0xe4);
            OVL_SETABS(0x7403c, 0x3d, 0xb6, 0x6b, 0xe4);
            OVL_SETABS(0x744bc, 10, 0xeb, 0x6e, 0x13e);
            OVL_SETABS(0x7413c, 0x2d, 0x164, 0x54, 0x18b);
            OVL_SETABS(0x7414c, 0x24, 0x1ae, 0x5f, 0x1c9);
            OVL_SETABS(0x7406c, 0x76, 0x23, 0xa4, 0x51);
            OVL_SETABS(0x7407c, 0xa7, 0x23, 0xd5, 0x51);
            OVL_SETABS(0x7408c, 0x76, 0x54, 0xa4, 0x82);
            OVL_SETABS(0x7409c, 0xa7, 0x54, 0xd5, 0x82);
            OVL_SETABS(0x740ac, 0x76, 0x85, 0xa4, 0xb3);
            OVL_SETABS(0x740bc, 0xa7, 0x85, 0xd5, 0xb3);
            OVL_SETABS(0x740cc, 0x76, 0xb6, 0xa4, 0xe4);
            OVL_SETABS(0x740dc, 0xa7, 0xb6, 0xd5, 0xe4);
            OVL_SETABS(0x740ec, 0xc0, 0xf4, 0xcf, 0x103);
            OVL_SETABS(0x740fc, 0xc0, 0x105, 0xcf, 0x114);
            OVL_SETABS(0x7410c, 0xc0, 0x115, 0xcf, 0x125);
            OVL_SETABS(0x7411c, 0xc0, 0x126, 0xcf, 0x135);
            OVL_SETABS(0x7412c, 0x73, 0xeb, 0xd7, 0x13e);
            OVL_SETABS(0x737ac, 0xc, 0x5a, 0x3a, 0x88);
            OVL_SETABS(0x737bc, 0xc, 0x8b, 0x3a, 0xb9);
            OVL_SETABS(0x737cc, 0xc, 0xbc, 0x3a, 0xea);
            OVL_SETABS(0x737dc, 0xc, 0xed, 0x3a, 0x11b);
            OVL_SETABS(0x737ec, 0x3d, 0x5a, 0x6b, 0x88);
            OVL_SETABS(0x737fc, 0x3d, 0x8b, 0x6b, 0xb9);
            OVL_SETABS(0x7380c, 0x3d, 0xbc, 0x6b, 0xea);
            OVL_SETABS(0x7381c, 0x3d, 0xed, 0x6b, 0x11b);
            OVL_SETABS(0x7384c, 7, 0x142, 0x67, 0x154);
            OVL_SETABS(0x7385c, 7, 0x165, 0x67, 0x177);
            OVL_SETABS(0x7386c, 7, 0x189, 0x67, 0x19b);
            OVL_SETABS(0x7387c, 7, 0x1ad, 0x67, 0x1bf);
            OVL_SETABS(0x7388c, 0, 0, 0x77, 0x1e0);
            OVL_SETABS(0x7336c, 0xe0, 0x104, 0x20d, 0x13f);
            OVL_SETABS(0x72f3c, 0xed, 0x10b, 0x12f, 0x139);
            OVL_SETABS(0x72f4c, 0x135, 0x10b, 0x177, 0x139);
            OVL_SETABS(0x72f5c, 0x17d, 0x10b, 0x1bf, 0x139);
            OVL_SETABS(0x72f6c, 0x1c5, 0x10b, 0x207, 0x139);
            OVL_SETABS(0x7458c, 0x10, 0x2e, 0x58, 0x67);
            OVL_SETABS(0x7459c, 0x10, 0x75, 0x58, 0xae);
            OVL_SETABS(0x745ac, 0x10, 0xbe, 0x58, 0xf7);
            OVL_SETABS(0x745bc, 0x10, 0x107, 0x58, 0x140);
            OVL_SETABS(0x745cc, 0x10, 0x14e, 0x58, 0x187);
            OVL_SETABS(0x745dc, 0x10, 0x192, 0x58, 0x1cb);
            OVL_SETABS(0x7445c, 8, 0x21, 0x70, 0x1d5);
            OVL_SETABS(0x7446c, 0, 0, 0x77, 0x1e0);
            OVL_SETABS(0x7447c, 0x111, 0, 0x20d, 0x40);
            OVL_SETABS(0x7448c, 0xe0, 0x41, 0x20d, 0x80);
            OVL_SETABS(0x7449c, 0xe0, 0x82, 0x20d, 0xc1);
            OVL_SETABS(0x744ac, 0x142, 0xc3, 0x20d, 0x102);
            OVL_SETABS(0x7444c, 0x207, 0, 0x280, 0x1e0);
            OVL_SETABS(0x7442c, 0, 0, 0x77, 0x1e0);
            OVL_SETABS(0x7443c, 0, 0, 0xf0, 0x1e0);
            if (*(int*)(self + 0x8fba4) != 0)
            {
                OVL_BTN(1, 0x6c91c);
                OVL_BTN(2, 0x6c92c);
                OVL_BTN(3, 0x6c93c);
                OVL_BTN(4, 0x6c94c);
                OVL_BTN(5, 0x6c95c);
                OVL_BTN(0xf, 0x6c9fc);
                OVL_BTN(9, 0x6c99c);
                OVL_BTN(7, 0x6c97c);
                OVL_BTN(0xc, 0x6c9cc);
                OVL_BTN(10, 0x6c9ac);
                OVL_BTN(0x10, 0x6ca0c);
                OVL_BTN(6, 0x6c96c);
                OVL_BTN(0x11, 0x6ca1c);
                OVL_BTN(8, 0x6c98c);
                OVL_BTN(0x14, 0x6ca4c);
                OVL_BTN(0x12, 0x6ca2c);
                OVL_BTN(0x13, 0x6ca3c);
                OVL_BTN(0xd, 0x6c9dc);
                OVL_BTN(0xe, 0x6c9ec);
                OVL_BTN(0xb, 0x6c9bc);
                OVL_BTN(0x15, 0x6ca5c);
                if (*(u8*)(self + 0x100c) == 0)
                {
                    OVL_BTN(0x23, 0x6cb3c);
                    OVL_BTN(0x24, 0x6cb4c);
                    OVL_BTN(0x25, 0x6cb5c);
                    OVL_BTN(0x26, 0x6cb6c);
                    OVL_BTN(0x66, 0x6cf6c);
                }
            }
            if (*(int*)(self + 0x685e8) != 0)
            {
                for (local_14 = 0; local_14 < 8; local_14++)
                    OVL_BTNIDX(local_14 + 0xaa, local_14 + 0xaa);
                for (local_14 = 0; local_14 < 4; local_14++)
                    OVL_BTNIDX(local_14 + 0xb4, local_14 + 0xb4);
                OVL_BTN(0xb8, 0x6d48c);
            }
            if (*(u8*)(self + 0x100c) == 0)
            {
                for (local_14 = 0; local_14 <= 3; local_14++)
                {
                    OVL_SETIDX(local_14 + 0x16, 0x218, local_14 * 0x31 + 0x101, 0x240, local_14 * 0x31 + 0x129);
                    OVL_SETIDX(local_14 + 0x1a, 0x249, local_14 * 0x31 + 0x101, 0x271, local_14 * 0x31 + 0x129);
                }
                for (local_14 = 0; local_14 < 8; local_14++)
                    OVL_BTNIDX(local_14 + 0x16, local_14 + 0x16);
            }
            else
            {
                OVL_SETRECT(0x6df5c, 0x211, 0xc, 0x276, 0x17);
                OVL_SETRECT(0x6df6c, 0x211, 0xe7, 0x276, 0x1bd);
            }
            if (*(int*)(self + 0x685e0) == 0 && *(int*)(self + 0x685dc) == 0 && *(int*)(self + 0x685e8) == 0)
            {
                OVL_BTN(0x1e, 0x6caec);
                OVL_BTN(0x1f, 0x6cafc);
            }
            if (*(u8*)(self + 0x100c) == 0)
            {
                EQ_METHOD(7, void, 0x00450316, (char*, short, RECT*, int, int, int, int, int), self)(0x20, OVL_RECT(0x6cb0c), -1, -1, 0, 0, 1);
                OVL_BTN(0x21, 0x6cb1c);
            }
            OVL_BTN(0x22, 0x6cb2c);
            if (*(u8*)(self + 0x100c) == 0)
                OVL_BTN(0x27, 0x6cb7c);
            if (*(int*)(self + 0x685e0) == 0)
            {
                if (*(int*)(self + 0x685dc) == 0)
                {
                    if (*(int*)(self + 0x685e8) == 0 && *(int*)(self + 0x8fb90) != 0)
                    {
                        OVL_SETABS(0x72f8c, 0x61, 0x23, 0x6c, 0x35);
                        OVL_BTN(0x28, 0x6cb8c);
                        OVL_SETABS(0x72f9c, 0x61, 0x1c1, 0x6c, 0x1d3);
                        OVL_BTN(0x29, 0x6cb9c);
                        OVL_SETABS(0x72fac, 0x61, 0x35, 0x6b, 0x40);
                        OVL_BTN(0x2a, 0x6cbac);
                    }
                }
                else
                {
                    *EQ_GLOBAL(int*, 0x006B3804) =
                        *(int*)(EQ_GLOBAL(char*, 0x006E4E34) + 4);
                    SetRect((RECT*)(EQ_GLOBAL(int*, 0x006B3804) + 1), 0x19f, 0, 0x216, 0x1e0);
                    OVL_SETABS(0x73f3c, 0x10, 0x1b2, 0x36, 0x1d2);
                    OVL_BTN(0x123, 0x6db3c);
                    OVL_SETABS(0x73f4c, 0x41, 0x1b2, 0x67, 0x1d2);
                    OVL_BTN(0x124, 0x6db4c);
                    OVL_SETABS(0x73f5c, 0x1c, 0x1b, 0x5a, 0x31);
                    OVL_SETABS(0x73f6c, 0x1c, 0x19b, 0x5a, 0x1b1);
                    for (local_14 = 0; local_14 <= 7; local_14++)
                    {
                        OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x113, 0xb, local_14 * 0x31 + 0x25, 0x39,
                                      local_14 * 0x31 + 0x53);
                        OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x114, 0x3c, local_14 * 0x31 + 0x25, 0x6a,
                                      local_14 * 0x31 + 0x53);
                    }
                    for (local_14 = 0; local_14 < 0x10; local_14++)
                        OVL_BTNIDX(local_14 + 0x113, local_14 + 0x113);
                }
            }
            else
            {
                *(int*)(self + 0xac) = 0;
                *(int*)(self + 0xb0) = 0;
                *EQ_GLOBAL(int*, 0x006B3804) =
                    *(int*)(EQ_GLOBAL(char*, 0x006E4E24) + 4);
                SetRect((RECT*)(EQ_GLOBAL(int*, 0x006B3804) + 1), 0xf0, 0, 0x167, 0x1e0);
                OVL_SETABS(0x73e0c, 0xf, 0x156, 0x35, 0x176);
                OVL_BTN(0x110, 0x6da0c);
                OVL_SETABS(0x73e1c, 0x40, 0x156, 0x66, 0x176);
                OVL_BTN(0x111, 0x6da1c);
                for (local_14 = 0; local_14 < 6; local_14++)
                {
                    OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x104, 0xb, local_14 * 0x31 + 0x2d, 0x39,
                                  local_14 * 0x31 + 0x5b);
                    OVL_BTNIDX(local_14 * 2 + 0x104, local_14 * 2 + 0x104);
                    OVL_SETABSIDX(0x72d0c, local_14 * 2 + 0x105, 0x3c, local_14 * 0x31 + 0x2d, 0x6a,
                                  local_14 * 0x31 + 0x5b);
                    OVL_BTNIDX(local_14 * 2 + 0x105, local_14 * 2 + 0x105);
                }
                OVL_SETRECT(0x6d93c, 0x25, 399, 0x53, 0x1bd);
                OVL_SETRECT(0x6da2c, 0x16, 0x1c2, 99, 0x1d3);
                OVL_BTN(0x112, 0x6da2c);
                OVL_SETABS(0x73f5c, 0x1c, 0x2d, 0x5a, 0x43);
                OVL_SETABS(0x73f6c, 0x1c, 0x13b, 0x5a, 0x151);
            }
            if (*(int*)(self + 0x685e4) != 0)
            {
                if (EQ_GLOBAL(char*, 0x006E4D88) == 0 || *(char*)(EQ_GLOBAL(char*, 0x006E4D88) + 0x98) != 1)
                {
                    OVL_SETABS(0x73fcc, 0xc, 0x23, 0x3a, 0x51);
                    OVL_SETABS(0x73fdc, 0x3d, 0x23, 0x6b, 0x51);
                    OVL_SETABS(0x73fec, 0xc, 0x54, 0x3a, 0x82);
                    OVL_SETABS(0x73ffc, 0x3d, 0x54, 0x6b, 0x82);
                    OVL_SETABS(0x7400c, 0xc, 0x85, 0x3a, 0xb3);
                    OVL_SETABS(0x7401c, 0x3d, 0x85, 0x6b, 0xb3);
                    OVL_SETABS(0x7402c, 0xc, 0xb6, 0x3a, 0xe4);
                    OVL_SETABS(0x7403c, 0x3d, 0xb6, 0x6b, 0xe4);
                    OVL_SETABS(0x744bc, 10, 0xeb, 0x6e, 0x13e);
                    OVL_SETABS(0x7413c, 0x2d, 0x164, 0x54, 0x18b);
                    OVL_SETABS(0x7414c, 0x24, 0x1ae, 0x5f, 0x1c9);
                    OVL_SETABS(0x7406c, 0x76, 0x23, 0xa4, 0x51);
                    OVL_SETABS(0x7407c, 0xa7, 0x23, 0xd5, 0x51);
                    OVL_SETABS(0x7408c, 0x76, 0x54, 0xa4, 0x82);
                    OVL_SETABS(0x7409c, 0xa7, 0x54, 0xd5, 0x82);
                    OVL_SETABS(0x740ac, 0x76, 0x85, 0xa4, 0xb3);
                    OVL_SETABS(0x740bc, 0xa7, 0x85, 0xd5, 0xb3);
                    OVL_SETABS(0x740cc, 0x76, 0xb6, 0xa4, 0xe4);
                    OVL_SETABS(0x740dc, 0xa7, 0xb6, 0xd5, 0xe4);
                    OVL_SETABS(0x740ec, 0xc0, 0xf4, 0xcf, 0x103);
                    OVL_SETABS(0x740fc, 0xc0, 0x105, 0xcf, 0x114);
                    OVL_SETABS(0x7410c, 0xc0, 0x115, 0xcf, 0x125);
                    OVL_SETABS(0x7411c, 0xc0, 0x126, 0xcf, 0x135);
                    OVL_SETABS(0x7412c, 0x73, 0xeb, 0xd7, 0x13e);
                    for (local_14 = 300; local_14 < 0x145; local_14++)
                        OVL_BTNIDX(local_14, local_14);
                    OVL_BTN(0x17b, 0x6e0bc);
                }
                else
                {
                    OVL_SETABS(0x7413c, 0x26, 0x175, 0x4d, 0x19c);
                    OVL_BTNABS(0x143, 0x7413c);
                    OVL_SETABS(0x7414c, 0x1c, 0x1b2, 0x58, 0x1cd);
                    OVL_BTNABS(0x144, 0x7414c);
                    OVL_SETABS(0x7406c, 0x24, 0x27, 0x52, 0x55);
                    OVL_BTNABS(0x136, 0x7406c);
                    OVL_SETABS(0x7407c, 0x24, 0x5d, 0x52, 0x8b);
                    OVL_BTNABS(0x137, 0x7407c);
                    OVL_SETABS(0x7408c, 0x24, 0x93, 0x52, 0xc1);
                    OVL_BTNABS(0x138, 0x7408c);
                    OVL_SETABS(0x7409c, 0x24, 0xc9, 0x52, 0xf7);
                    OVL_BTNABS(0x139, 0x7409c);
                    OVL_SETABS(0x740ec, 9, 0x106, 0x6d, 0x11f);
                    OVL_BTNABS(0x13e, 0x740ec);
                    OVL_SETABS(0x740fc, 9, 0x120, 0x6d, 0x130);
                    OVL_BTNABS(0x13f, 0x740fc);
                    OVL_SETABS(0x7410c, 9, 0x131, 0x6d, 0x140);
                    OVL_BTNABS(0x140, 0x7410c);
                    OVL_SETABS(0x7411c, 9, 0x141, 0x6d, 0x15a);
                    OVL_BTNABS(0x141, 0x7411c);
                    OVL_SETABS(0x7412c, 9, 0x106, 0x6d, 0x15a);
                    OVL_BTNABS(0x142, 0x7412c);
                }
            }
        }
        for (local_14 = 0; local_14 < 100; local_14++)
            *(short*)(self + 0x6c65e + local_14 * 2) = -1;
        idx = 0;
        for (local_14 = 0; local_14 < 100; local_14++)
        {
            hasSkill = OVL_HASSKILL(local_14);
            if (hasSkill != 0)
            {
                *(short*)(self + 0x6c65e + idx * 2) = local_14;
                idx++;
            }
        }
        for (idx2 = 0; idx2 < 0x20; idx2++)
        {
            if (*(char*)(OVL_PC + 0x7e + idx2) != 0)
            {
                *(short*)(self + 0x6c65e + idx * 2) = idx2 + 100;
                idx++;
            }
        }
        *(int*)(self + 0x685bc) = (char)idx;
        OVL_FIND_ELEMENT(0x6c5d0, 0x2a);
        break;
    case 5:
        if (*(char*)(self + 0x8fbac) == 0)
        {
            OVL_BTN(0xdc, 0x6d6cc);
            OVL_BTN(0xdd, 0x6d6dc);
            OVL_BTN(0xde, 0x6d6ec);
            if (*(char*)(OVL_PC + 0x36) != 1 && *(char*)(OVL_PC + 0x36) != 7 && *(char*)(OVL_PC + 0x36) != 9)
                OVL_BTN(0xdf, 0x6d6fc);
            OVL_BTN(0xe0, 0x6d70c);
            OVL_BTN(200, 0x6d58c);
            OVL_BTN(0xc9, 0x6d59c);
            OVL_BTN(0xca, 0x6d5ac);
            OVL_BTN(0xcb, 0x6d5bc);
            OVL_BTN(0xcc, 0x6d5cc);
            OVL_BTN(0xcd, 0x6d5dc);
            if (g_game_state_guess == 2)
            {
                OVL_BTN(0xce, 0x6d5ec);
                OVL_BTN(0xcf, 0x6d5fc);
                OVL_BTN(0xd0, 0x6d60c);
                OVL_BTN(0xd1, 0x6d61c);
            }
            OVL_BTN(0xd2, 0x6d62c);
            OVL_BTN(0xd3, 0x6d63c);
            for (idx = 0; idx < 8; idx++)
                OVL_BTNIDX(idx + 0xd4, idx + 0xd4);
        }
        OVL_BTN(0xfc, 0x6d8cc);
        OVL_BTN(0xe1, 0x6d71c);
        OVL_BTN(0xe2, 0x6d72c);
        OVL_BTN(0x38, 0x6cc8c);
        OVL_BTN(0xfe, 0x6d8ec);
        OVL_BTN(0xff, 0x6d8fc);
        OVL_BTN(0x100, 0x6d90c);
        OVL_BTN(0x101, 0x6d91c);
        OVL_BTN(0x102, 0x6d92c);
        OVL_BTN(0x35, 0x6cc5c);
        OVL_BTN(0x36, 0x6cc6c);
        OVL_BTN(0x37, 0x6cc7c);
        for (idx = 0; idx < 8; idx++)
            OVL_BTNIDX(idx + 0x16, idx + 0x16);
        OVL_BTN(0x2b, 0x6cbbc);
        OVL_BTN(0x2c, 0x6cbcc);
        OVL_BTN(0x2d, 0x6cbdc);
        OVL_BTN(0x2e, 0x6cbec);
        OVL_BTN(0x2f, 0x6cbfc);
        OVL_BTN(0x30, 0x6cc0c);
        OVL_BTN(0x31, 0x6cc1c);
        OVL_BTN(0x32, 0x6cc2c);
        OVL_BTN(0x33, 0x6cc3c);
        OVL_BTN(0x34, 0x6cc4c);
        for (idx = 0; idx < 10; idx++)
        {
            *(int*)(self + 0x6c5a8 + idx * 4) = 0;
            if (*(int*)(self + 0x70) != 0)
            {
                for (local_10 = *(int*)(self + 0x70); local_10 != 0; local_10 = *(int*)(local_10 + 0x20))
                {
                    if (*(short*)(local_10 + 0x14) == idx + 0x2b)
                        *(int*)(self + 0x6c5a8 + idx * 4) = local_10 + 4;
                }
            }
        }
        EQ_METHOD(1, void, 0x0046E4AA, (char*, int), self)(*(short*)(self + 0x6c560));
        break;
    case 6:
        OVL_SETABS(0x7336c, 0xe0, 0x104, 0x20d, 0x13f);
        OVL_SETABS(0x72f3c, 0xed, 0x10b, 0x12f, 0x139);
        OVL_SETABS(0x72f4c, 0x135, 0x10b, 0x177, 0x139);
        OVL_SETABS(0x72f5c, 0x17d, 0x10b, 0x1bf, 0x139);
        OVL_SETABS(0x72f6c, 0x1c5, 0x10b, 0x207, 0x139);
        OVL_BTN(3, 0x6c93c);
        OVL_BTN(4, 0x6c94c);
        OVL_BTN(5, 0x6c95c);
        OVL_BTN(6, 0x6c96c);
        OVL_BTN(7, 0x6c97c);
        OVL_BTN(8, 0x6c98c);
        OVL_BTN(9, 0x6c99c);
        OVL_BTN(10, 0x6c9ac);
        OVL_BTN(0xb, 0x6c9bc);
        OVL_BTN(0xe, 0x6c9ec);
        OVL_BTN(0xf, 0x6c9fc);
        OVL_BTN(0x10, 0x6ca0c);
        OVL_BTN(0x11, 0x6ca1c);
        OVL_BTN(0x12, 0x6ca2c);
        OVL_BTN(0x13, 0x6ca3c);
        OVL_BTN(0x14, 0x6ca4c);
        OVL_BTN(0x15, 0x6ca5c);
        OVL_BTN(0x16, 0x6ca6c);
        for (local_14 = 0; local_14 < 100; local_14++)
            *(short*)(self + 0x6c726 + local_14 * 2) = -1;
        idx = 0;
        for (local_14 = 0; local_14 < 100; local_14++)
        {
            hasSkill = OVL_HASSKILL(local_14);
            if (hasSkill != 0)
            {
                *(short*)(self + 0x6c726 + idx * 2) = local_14;
                idx++;
            }
        }
        for (idx2 = 0; idx2 < 0x20; idx2++)
        {
            if (*(char*)(OVL_PC + 0x7e + idx2) != 0)
            {
                *(short*)(self + 0x6c726 + idx * 2) = idx2 + 100;
                idx++;
            }
        }
        *(int*)(self + 0x685c4) = (char)idx;
        *(int*)(self + 0x6c5d4) = 0;
        *(int*)(self + 0x6c5d8) = 0;
        if (*(int*)(self + 0x70) != 0)
        {
            for (local_10 = *(int*)(self + 0x70); local_10 != 0; local_10 = *(int*)(local_10 + 0x20))
            {
                if (*(short*)(local_10 + 0x14) == 0x15)
                    *(int*)(self + 0x6c5d4) = local_10 + 4;
                else if (*(short*)(local_10 + 0x14) == 0x16)
                    *(int*)(self + 0x6c5d8) = local_10 + 4;
            }
        }
        break;
    case 8:
        OVL_BTN(0, 0x6c90c);
        OVL_BTN(1, 0x6c91c);
        OVL_BTN(2, 0x6c92c);
        OVL_BTN(3, 0x6c93c);
        break;
    case 9:
        for (local_14 = 0; local_14 < 0x22; local_14++)
            OVL_BTNIDX(local_14, local_14);
        OVL_FIND_ELEMENT(0x6c5d0, 6);
        break;
    case 10:
        OVL_BTN(0, 0x6c90c);
        OVL_BTN(1, 0x6c91c);
        OVL_BTN(2, 0x6c92c);
        break;
    case 0xb:
        OVL_BTN(0, 0x6c90c);
        OVL_BTN(1, 0x6c91c);
        OVL_BTN(2, 0x6c92c);
        OVL_BTN(3, 0x6c93c);
        OVL_BTN(4, 0x6c94c);
        OVL_BTN(5, 0x6c95c);
        for (local_14 = 0; local_14 < 100; local_14++)
            *(short*)(self + 0x6c65e + local_14 * 2) = 0;
        idx2 = 0;
        for (idx = 0; idx < 0x20; idx++)
        {
            if (*(char*)(OVL_PC + 0x7e + idx) != 0)
            {
                *(short*)(self + 0x6c65e + idx2 * 2) = idx;
                idx2++;
            }
        }
        *(short*)(self + 0xfa) = idx2;
        break;
    case 0xc:
        OVL_BTN(0, 0x6c90c);
        EQ_METHOD(0, void, 0x0047BAAC, (char*), self)();
    }
    if (*(char*)(self + 0xfe0) < 4 || (g_game_state_guess == 2 && *(char*)(self + 0xfe0) == 4))
    {
        if (*(char*)(self + 0x8fbac) == 0 && *(int*)(self + 0x685f4) == 0)
        {
            if (g_game_state_guess != 2)
            {
                OVL_BTN(0xdc, 0x6d6cc);
                OVL_BTN(0xdd, 0x6d6dc);
            }
            if (OVL_PC != 0 && *(char*)(OVL_PC + 0x36) != 1 && *(char*)(OVL_PC + 0x36) != 7 &&
                *(char*)(OVL_PC + 0x36) != 9)
            {
                if (g_game_state_guess == 1)
                    OVL_BTN(0xdf, 0x6d6fc);
                if (OVL_POS(0x900a4)->GetF14() != 0)
                {
                    for (idx = 0; idx < 8; idx++)
                        OVL_BTNIDX(idx + 0xd4, idx + 0xd4);
                }
            }
            if (g_game_state_guess == 1)
            {
                OVL_BTN(0xde, 0x6d6ec);
                OVL_BTN(0xe0, 0x6d70c);
            }
            if (OVL_POS(0x900c4)->GetF14() != 0)
            {
                OVL_BTN(200, 0x6d58c);
                OVL_BTN(0xc9, 0x6d59c);
                OVL_BTN(0xca, 0x6d5ac);
                OVL_BTN(0xcb, 0x6d5bc);
                OVL_BTN(0xcc, 0x6d5cc);
                OVL_BTN(0xcd, 0x6d5dc);
                if (g_game_state_guess == 2)
                {
                    OVL_BTN(0xce, 0x6d5ec);
                    OVL_BTN(0xcf, 0x6d5fc);
                    OVL_BTN(0xd0, 0x6d60c);
                    OVL_BTN(0xd1, 0x6d61c);
                }
            }
        }
        OVL_BTN(0xfc, 0x6d8cc);
        if (*(char*)(self + 0x8fbac) == 0 && *(int*)(self + 0x685f4) == 0)
        {
            OVL_BTN(0xd2, 0x6d62c);
            OVL_BTN(0xd3, 0x6d63c);
        }
        if (g_game_state_guess == 1)
        {
            OVL_BTN(0xe1, 0x6d71c);
            OVL_BTN(0xe2, 0x6d72c);
        }
        OVL_BTN(0xe3, 0x6d73c);
        OVL_BTN(0xe4, 0x6d74c);
        OVL_BTN(0xe5, 0x6d75c);
        OVL_BTN(0xfe, 0x6d8ec);
        OVL_BTN(0xff, 0x6d8fc);
        OVL_BTN(0x100, 0x6d90c);
        OVL_BTN(0x101, 0x6d91c);
        OVL_BTN(0x102, 0x6d92c);
        if (*(int*)(self + 0x685f4) == 0)
        {
            if (g_game_state_guess != 2)
            {
                OVL_SETRECT(0x6cf7c, 8, 0x30, 0x6d, 0x41);
                OVL_SETRECT(0x6cf8c, 8, 0x56, 0x6d, 0x67);
                OVL_SETRECT(0x6cf9c, 8, 0x7c, 0x6d, 0x8d);
                OVL_SETRECT(0x6cfac, 8, 0xa2, 0x6d, 0xb3);
                OVL_SETRECT(0x6cfbc, 8, 200, 0x6d, 0xd9);
                OVL_SETRECT(0x6cfec, 0x24, 0x1be, 0x52, 0x1cf);
                OVL_SETRECT(0x6cffc, 8, 0x1be, 0x36, 0x1cf);
                OVL_SETRECT(0x6d00c, 0x3e, 0x1be, 0x6c, 0x1cf);
                switch (*(char*)(self + 0x8fbac))
                {
                case 1:
                    for (idx = 0x67; idx < 0x6c; idx++)
                        OVL_BTNIDX(idx, idx);
                    OVL_BTN(0x6e, 0x6cfec);
                    break;
                case 2:
                    OVL_SETRECT(0x6d16c, 0x2c, 0x39, 0x49, 0x46);
                    OVL_SETRECT(0x6d17c, 8, 99, 0x6e, 0x70);
                    OVL_SETRECT(0x6d18c, 0x2c, 0x8d, 0x49, 0x9a);
                    OVL_SETRECT(0x6d19c, 0x2c, 0xb7, 0x49, 0xc4);
                    OVL_SETRECT(0x6d1ac, 9, 0xe1, 0x1a, 0xee);
                    OVL_SETRECT(0x6d1bc, 0x5c, 0xe1, 0x6d, 0xee);
                    OVL_SETRECT(0x6d1cc, 0x35, 0xe2, 0x3f, 0xee);
                    OVL_SETRECT(0x6d1dc, 9, 0x10b, 0x1a, 0x118);
                    OVL_SETRECT(0x6d1ec, 0x5c, 0x10b, 0x6d, 0x118);
                    OVL_SETRECT(0x6d1fc, 0x35, 0x10c, 0x3f, 0x118);
                    OVL_SETRECT(0x6d20c, 9, 0x135, 0x1a, 0x142);
                    OVL_SETRECT(0x6d21c, 0x5c, 0x135, 0x6d, 0x142);
                    OVL_SETRECT(0x6d22c, 0x35, 0x136, 0x3f, 0x142);
                    OVL_SETRECT(0x6d23c, 0x2c, 0x15f, 0x49, 0x16c);
                    OVL_SETRECT(0x6d24c, 0x2c, 0x184, 0x49, 0x191);
                    OVL_SETRECT(0x6d25c, 8, 0x1a9, 0x6e, 0x1b6);
                    for (idx = 0x86; idx < 0x96; idx++)
                        OVL_BTNIDX(idx, idx);
                    OVL_BTN(0x6e, 0x6cfec);
                    break;
                case 3:
                    OVL_SETRECT(0x6d26c, 9, 0x55, 0x1a, 0x5f);
                    OVL_SETRECT(0x6d27c, 0x5c, 0x55, 0x6d, 0x5f);
                    OVL_SETRECT(0x6d28c, 0x35, 0x54, 0x3f, 0x5f);
                    OVL_SETRECT(0x6d29c, 8, 0x7d, 0x6e, 0x89);
                    OVL_SETRECT(0x6d2ac, 0x2b, 0xa7, 0x48, 0xb3);
                    for (idx = 0x96; idx < 0x9b; idx++)
                        OVL_BTNIDX(idx, idx);
                    OVL_SETRECT(0x6d2dc, 0x2b, 0xd1, 0x49, 0xde);
                    OVL_BTN(0x9d, 0x6d2dc);
                    OVL_SETRECT(0x6d2ec, 0x2b, 0xfb, 0x49, 0x108);
                    OVL_BTN(0x9e, 0x6d2ec);
                    OVL_SETRECT(0x6d2fc, 8, 0x125, 0x6e, 0x132);
                    OVL_BTN(0x9f, 0x6d2fc);
                    OVL_SETRECT(0x6d30c, 0x2b, 0x14f, 0x49, 0x15c);
                    OVL_BTN(0xa0, 0x6d30c);
                    OVL_SETRECT(0x6d31c, 0x2b, 0x179, 0x49, 0x186);
                    OVL_BTN(0xa1, 0x6d31c);
                    OVL_SETRECT(0x6d32c, 0x2b, 0x1a3, 0x49, 0x1b0);
                    OVL_BTN(0xa2, 0x6d32c);
                    OVL_BTN(0x6f, 0x6cffc);
                    OVL_BTN(0x70, 0x6d00c);
                    break;
                case 4:
                    OVL_SETRECT(0x6d35c, 9, 0x55, 0x1a, 0x5f);
                    OVL_BTN(0xa5, 0x6d35c);
                    OVL_SETRECT(0x6d36c, 0x5c, 0x55, 0x6d, 0x5f);
                    OVL_BTN(0xa6, 0x6d36c);
                    OVL_SETRECT(0x6d37c, 0x2b, 0x7f, 0x48, 0x8b);
                    OVL_BTN(0xa7, 0x6d37c);
                    OVL_SETRECT(0x6d38c, 0x2b, 0xa7, 0x48, 0xb3);
                    OVL_BTN(0xa8, 0x6d38c);
                    OVL_SETRECT(0x6d39c, 0x2b, 0xcf, 0x48, 0xdb);
                    OVL_BTN(0xa9, 0x6d39c);
                    OVL_BTN(0x6e, 0x6cfec);
                    break;
                case 5:
                    for (idx = 0; idx < 9; idx++)
                        OVL_SETIDX(idx + 0x7d, 8, idx * 0x2a + 0x39, 0x6e, idx * 0x2a + 0x57);
                    if (*(char*)(self + 0x100d) == '\n')
                    {
                        for (idx = 0x7d; idx < 0x84; idx++)
                            OVL_BTNIDX(idx, idx);
                    }
                    else
                    {
                        for (idx = 0x7d; idx < 0x86; idx++)
                            OVL_BTNIDX(idx, idx);
                    }
                    if (*(char*)(self + 0x100d) == '\n')
                    {
                        OVL_BTN(0x6e, 0x6cfec);
                    }
                    else
                    {
                        OVL_BTN(0x6f, 0x6cffc);
                        OVL_BTN(0x70, 0x6d00c);
                    }
                    break;
                case 6:
                    for (counter2 = 0; counter2 < 10; counter2++)
                        OVL_SETIDX(counter2 + 0x72, 0x2b, counter2 * 0x2a + 0x2f, 0x49, counter2 * 0x2a + 0x3c);
                    for (idx = 0x72; idx < 0x7c; idx++)
                        OVL_BTNIDX(idx, idx);
                    OVL_BTN(0x6f, 0x6cffc);
                    OVL_BTN(0x70, 0x6d00c);
                    break;
                case 7:
                    OVL_SETRECT(0x6d33c, 9, 0x39, 0x1a, 0x46);
                    OVL_BTN(0xa3, 0x6d33c);
                    OVL_SETRECT(0x6d34c, 0x5c, 0x39, 0x6d, 0x46);
                    OVL_BTN(0xa4, 0x6d34c);
                    OVL_BTN(0x6e, 0x6cfec);
                    break;
                case 8:
                    for (counter2 = 0; counter2 < 4; counter2++)
                        OVL_SETIDX(counter2 + 0x72, 0x2b, counter2 * 0x2a + 0x2f, 0x49, counter2 * 0x2a + 0x3c);
                    for (idx = 0x72; idx < 0x76; idx++)
                        OVL_BTNIDX(idx, idx);
                    OVL_BTN(0x6e, 0x6cfec);
                }
            }
        }
        else
        {
            if (g_game_state_guess == 2)
            {
                iVar8 = OVL_POS(0x90264)->GetX();
                iVar4 = OVL_POS(0x90264)->GetY();
                OVL_SETRECT(0x6ce0c, iVar8 + 0x26, iVar4 + 0xfa, iVar8 + 0x4d, iVar4 + 0x121);
                OVL_BTN(0x50, 0x6ce0c);
                OVL_SETRECT(0x6ce1c, iVar8 + 0x1d, iVar4 + 0x126, iVar8 + 0x58, iVar4 + 0x141);
                OVL_BTN(0x51, 0x6ce1c);
                OVL_SETRECT(0x6cfcc, iVar8 + 0x6e, iVar4 + 5, iVar8 + 0x78, iVar4 + 0xf);
                OVL_BTN(0x6c, 0x6cfcc);
                for (idx = 0; idx < 0x14; idx++)
                {
                    OVL_SETIDX(idx + 0x52, iVar8 + 10, iVar4 + 4 + idx * 0xc, iVar8 + 100,
                               iVar4 + 0x10 + idx * 0xc);
                    OVL_BTNIDX(idx + 0x52, idx + 0x52);
                }
            }
            else
            {
                OVL_SETRECT(0x6ce0c, 0x26, 0x175, 0x4d, 0x19c);
                OVL_BTN(0x50, 0x6ce0c);
                OVL_SETRECT(0x6ce1c, 0x1d, 0x1b2, 0x58, 0x1cd);
                OVL_BTN(0x51, 0x6ce1c);
                OVL_SETRECT(0x6cfcc, 100, 0x23, 0x6e, 0x2d);
                OVL_BTN(0x6c, 0x6cfcc);
                for (idx = 0; idx < 0x14; idx++)
                {
                    OVL_SETIDX(idx + 0x52, 0xf, idx * 0xf + 0x23, 100, idx * 0xf + 0x32);
                    OVL_BTNIDX(idx + 0x52, idx + 0x52);
                }
            }
            *(int*)(self + 0x6c5dc) = 0;
            if (*(int*)(self + 0x70) != 0)
            {
                for (local_10 = *(int*)(self + 0x70); local_10 != 0; local_10 = *(int*)(local_10 + 0x20))
                {
                    if (*(short*)(local_10 + 0x14) == 0x6c)
                    {
                        *(int*)(self + 0x6c5dc) = local_10 + 4;
                        return;
                    }
                }
            }
        }
    }
}

#undef OVL_POS
#undef OVL_ROW
#undef OVL_RECT
#undef OVL_IDXRECT
#undef OVL_SETRECT
#undef OVL_SETIDX
#undef OVL_SETABS
#undef OVL_SETABSIDX
#undef OVL_ADD
#undef OVL_BTN
#undef OVL_BTNIDX
#undef OVL_BTNABS
#undef OVL_VCALL
#undef OVL_HASSKILL
#undef OVL_PC
#undef OVL_SYNC18
#undef OVL_PARK
#undef OVL_FIND_ELEMENT

// FUNCTION: TRILOGY 0x0047b88b
// Closes open gameplay windows and clears transient interface state.
void __fastcall ResetOverlayState_guess(void* self)
{
    typedef void (__fastcall *tOverlayNoArgMethod_guess)(void*);
    void* window;

    if (pinstEQ_Main_guess->screenMode_guess == 8)
    {
        g_chatInputInactive_guess = 1;
        EQ_METHOD(1, void, 0x004CF7A1, (void*, int), g_everquest_guess)(0);
    }
    *(char*)((char*)self + 0x8FBAC) = 0;
    *(int*)((char*)self + 0xED4) = -1;
    *(int*)((char*)self + 0x88) = 0;
    *(int*)((char*)self + 0xFE4) = 0;
    *(int*)((char*)self + 0x10) = 0;
    *(int*)((char*)self + 0x8FBA8) = 0;
    *(int*)((char*)self + 0x5F18C) = 0;
    *(char*)((char*)self + 0x100C) = 0;
    *(int*)((char*)self + 0x84) = 0;
    *(int*)((char*)self + 0x90274) = 0;
    if (*(int*)((char*)self + 0x9027C) == 0)
        *(int*)((char*)self + 0x685F4) = 0;
    *(short*)((char*)self + 0x1006) = -1;
    *(short*)((char*)self + 0x1004) = -1;
    *(short*)((char*)self + 0x6C54E) = 0;
    *(short*)((char*)self + 0x6C54C) = 0;
    *(int*)((char*)self + 0x685D4) = 0;
    *(int*)((char*)self + 0x685D8) = 0;
    *(short*)((char*)self + 0x6C568) = -1;
    if (g_pc_guess->inventory_items[0] != NULL)
        g_cdisplay_guess->itemOnCursor_guess = 1;
    else
        g_cdisplay_guess->itemOnCursor_guess = 0;

    window = EQ_GLOBAL(void*, 0x006018C0);
    if (window != NULL && *(int*)((char*)window + 0x10) != 0)
        ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);

    window = EQ_GLOBAL(void*, 0x00604984);
    if (window != NULL && *(int*)((char*)window + 0x10) != 0)
    {
        *(int*)((char*)window + 0x1E8) = 1;
        window = EQ_GLOBAL(void*, 0x00604984);
        ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);
    }

    window = EQ_GLOBAL(void*, 0x006E4DE8);
    if (*(char*)((char*)window + 0x1F1) != 0)
        *(char*)((char*)window + 0x1F1) = 0;
    window = EQ_GLOBAL(void*, 0x006E4DE8);
    ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);
    window = EQ_GLOBAL(void*, 0x006E4DE8);
    ((tOverlayNoArgMethod_guess)(*(void***)window)[1])(window);

    window = EQ_GLOBAL(void*, 0x006E4E20);
    if (window != NULL && *(int*)((char*)window + 0x10) != 0 &&
        *(char*)((char*)window + 0x17C) != 1)
        ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);

    window = EQ_GLOBAL(void*, 0x006E4E60);
    if (*(int*)((char*)window + 0x10) != 0)
    {
        *(char*)(*(u32*)((char*)window + 0x37AC) + 0xAD) = 0;
        window = EQ_GLOBAL(void*, 0x006E4E60);
        ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);
        window = EQ_GLOBAL(void*, 0x006E4E60);
        *(u32*)((char*)window + 0x37AC) = 0;
    }

    window = EQ_GLOBAL(void*, 0x006E4D84);
    if (*(int*)((char*)window + 0x10) != 0)
    {
        *(int*)((char*)window + 0x9C) = 0;
        window = EQ_GLOBAL(void*, 0x006E4D84);
        ((tOverlayNoArgMethod_guess)(*(void***)window)[2])(window);
        pinstEQ_TextIO_guess->redrawPass_guess = 1;
    }
}

// FUNCTION: TRILOGY 0x0047ba1f
// Checks whether current interactions or open windows block an action.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: bool state uses AL operations; the original keeps its state in EAX.
bool EQ_Main::CanTransact(void)
{
    bool canTransact = true;

    if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
        canTransact = 0;
    if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
        canTransact = 0;
    if (EQ_GLOBAL(int, 0x006E4DB8) != 0 ||
        EQ_GLOBAL(int, 0x006E4DBC) != 0 ||
        EQ_GLOBAL(int, 0x006E4DC0) != 0 ||
        EQ_GLOBAL(int, 0x006E4DC4) != 0)
        canTransact = 0;
    if (EQ_GLOBAL(u8, 0x006B7111) != 0)
        canTransact = 0;

    if (EQ_GLOBAL(void*, 0x006E4D8C) != 0)
    {
        char* actor = *(char**)((char*)EQ_GLOBAL(void*, 0x006E4D8C) + 0x70);
        if (*(int*)(actor + 0x68) != 0)
            canTransact = 0;
        if (*(int*)(actor + 0x18c) != 0)
            canTransact = 0;
    }

    if (*(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x10) != 0 ||
        *(int*)((char*)*(void**)0x006018c0 + 0x10) != 0 ||
        *(int*)((char*)*(void**)0x006e4d84 + 0x10) != 0)
        canTransact = 0;

    return canTransact;
}

// FUNCTION: TRILOGY 0x0047baac
// Loads the manual into its window and reports when everquest_manual.txt is missing.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_loadManual_guess(int* manualWindow)
{
    char fileName[128];
    FILE* file;
    char* text;
    char* main;
    int length;
    int pos;
    int next;
    char c;

    strcpy(fileName, (const char*)0x0055c1b0);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (const char*)0x0055c1ac);
    if (file == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)((const char*)0x0055c184, 15);
        (*(void (__fastcall **)(int*))(*manualWindow + 8))(manualWindow);
        *((char*)manualWindow + 0xfe0) = *((char*)manualWindow + 0xfe1);
        (*(void (__fastcall **)(int*))(*manualWindow + 4))(manualWindow);
        if (EQ_GLOBAL(int, 0x00590170) == 2)
        {
            g_game_state_guess = 2;
            main = (char*)EQ_GLOBAL(void*, 0x006E4E44);
            if (main != 0)
                *(int*)(main + 0xeb8) = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
    }
    else
    {
        EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
        manualWindow[0x50c] = 0;
        text = (char*)(manualWindow + 0xc08e);
        EQ_CDECL(size_t, 0x00522038, (char*, size_t, size_t, FILE*))(text, 0x2edff, 1, file);
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        *((char*)manualWindow + 0x5f037) = 0;
        length = strlen(text);
        if (length != 0)
        {
            do
            {
                for (pos = 0; pos < length; pos++)
                {
                    c = text[pos];
                    if (c < ' ' && c != -0x6e)
                    {
                        strncpy((char*)(manualWindow + manualWindow[0x50c] * 0x20 + 0x50e), text, pos);
                        *((char*)manualWindow + manualWindow[0x50c] * 0x80 + pos + 0x1438) = 0;
                        manualWindow[0x50c]++;
                        for (next = pos + 1; next < length; next++)
                        {
                            c = text[next];
                            if (c >= ' ' || c == -0x6e)
                                break;
                        }
                        strcpy(text, text + next);
                        length = strlen(text);
                        break;
                    }
                }
            } while (length != 0);
        }
        manualWindow[0x50d] = 0;
    }
}

#ifdef EQD_MATCH
/* The match build only declares eqd_va_* for addresses written out in EQ_GLOBAL(); these come through macros or
   differ in case from the extern at the top of the file. */
extern char eqd_va_0x006E4E40;
extern char eqd_va_0x006E4E3C;
extern char eqd_va_0x006E4DE8;
#endif

// The slot's image rectangle: 16 bytes (x, y, right, bottom) per slot at +0x6d58c, 400 slots per set (+0xfe0).
#define OVL_ELEM_guess(i)       (self + (*(char*)(self + 0xfe0) * 400 + (i)) * 0x10 + 0x6d58c)
#define OVL_ELEM_X_guess        (*(int*)OVL_ELEM_guess(slotIndex))
#define OVL_ELEM_Y_guess        (*(int*)(OVL_ELEM_guess(slotIndex) + 4))
#define OVL_ELEM_R_guess        (*(int*)(OVL_ELEM_guess(slotIndex) + 8))
#define OVL_ELEM_B_guess        (*(int*)(OVL_ELEM_guess(slotIndex) + 0xc))
#define OVL_SUB_guess           (*(short*)(self + (slotIndex + (*(char*)(self + 0x6841d) + 0x29b4) * 10) * 4))
#define OVL_NAME_guess          (self + (OVL_SUB_guess % 12 + (OVL_SUB_guess / 12) * 12) * 0x136 + 0x5f196)
#define OVL_SELECTED_guess      ((int)*(short*)(self + 0x5f188) == slotIndex + 200)
#define OVL_DRAW_guess(image, rect) \
    EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), (image))((rect), 1)
#define OVL_TEXT_guess(str, x, y, a4, color) \
    EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))((str), (x), (y), (a4), (color), 1)
#define OVL_PCITEM_guess        (*(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + 0xe0 + OVL_SUB_guess * 4))
#define OVL_RECT5_guess(obj, l, t, r, b) \
    *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, obj) + 4); \
    left = (l); top = (t); right = (r); bottom = (b); \
    break

// FUNCTION: TRILOGY 0x0047bc1f
// Draws one user-defined HotButton slot: equipped item, skill, named entry, inventory item or menu button.
// Draws one overlay slot (kind at +0x6841e, sub-index at +0x68420 per page entry): picks the image or rect for
// kinds 1-6, then draws it, plus the item icon, stack count or label text where the kind has one.
void __fastcall DrawHotButton_guess(char* self, int unusedEdx, int slotIndex)
{
    char text[128];
    size_t len;
    RECT rect;
    char kind;
    short sub;
    short v;
    short width;
    void* image;
    char* item;
    char* p;
    int count;
    int left;
    int top;
    int right;
    int bottom;
    int page;

    page = *(char*)(self + 0x6841d);
    kind = *(char*)(self + (slotIndex + page * 10) * 4 + 0x6841e);
    if (kind == 1)
    {
        sub = *(short*)(self + (slotIndex + (page * 5 + 0xd084) * 2) * 4);
        switch (sub)
        {
        case 0:
            if (OVL_SELECTED_guess || pinstEQ_Main_guess->primaryAttackReady_guess == 0)
                image = EQ_GLOBAL(void*, 0x006B36AC);
            else
                image = EQ_GLOBAL(void*, 0x006B3694);
            OVL_DRAW_guess(image, OVL_ELEM_guess(slotIndex));
            item = *(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x114);
            if (item == 0)
                return;
            EQ_CDECL(void, 0x00438423, (void*, int))(OVL_ELEM_guess(slotIndex), *(short*)(item + 0x84));
            return;
        case 1:
            if (OVL_SELECTED_guess || pinstEQ_Main_guess->primaryAttackReady_guess == 0)
                image = EQ_GLOBAL(void*, 0x006B36B0);
            else
                image = EQ_GLOBAL(void*, 0x006B3698);
            OVL_DRAW_guess(image, OVL_ELEM_guess(slotIndex));
            item = *(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x10c);
            if (item == 0)
                return;
            EQ_CDECL(void, 0x00438423, (void*, int))(OVL_ELEM_guess(slotIndex), *(short*)(item + 0x84));
            if (EQ_CDECL(int, 0x004A1D83, (char*))(*(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x10c)) == 0)
                return;
            SetRect(&rect, OVL_ELEM_X_guess + 0x15, OVL_ELEM_Y_guess + 0x1a, OVL_ELEM_R_guess, OVL_ELEM_B_guess);
            OVL_DRAW_guess(EQ_GLOBAL(void*, 0x006B37FC), &rect);
            item = *(char**)(EQ_GLOBAL(char*, 0x006E4DA8) + 0x10c);
            sprintf(text, (const char*)0x0053e16c,
                ((int)*(unsigned char*)(item + 0xda) - 1) / *(char*)(item + 0xbb) + 1);
            OVL_TEXT_guess(text, OVL_ELEM_X_guess + 0x1a, OVL_ELEM_Y_guess + 0x1f, 10, EQ_GLOBAL(int, 0x006EFDF4));
            return;
        default:
            return;
        }
    }
    else if (kind == 2)
    {
        sub = *(short*)(self + (slotIndex + (page * 5 + 0xd084) * 2) * 4);
        switch (sub)
        {
        case 0:
            v = *(short*)(self + 0xfec);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B36B4);
                else
                    image = EQ_GLOBAL(void*, 0x006B369C);
                goto drawElem;
            }
            break;
        case 1:
            v = *(short*)(self + 0xfee);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B36B8);
                else
                    image = EQ_GLOBAL(void*, 0x006B36A0);
                goto drawElem;
            }
            break;
        case 2:
            v = *(short*)(self + 0xff0);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B36BC);
                else
                    image = EQ_GLOBAL(void*, 0x006B36A4);
                goto drawElem;
            }
            break;
        case 3:
            v = *(short*)(self + 0xff2);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B36C0);
                else
                    image = EQ_GLOBAL(void*, 0x006B36A8);
                goto drawElem;
            }
            break;
        default:
            return;
        }
        if (v > 100)
        {
            if (v == 0x6f)
                goto spell6f;
            if (v == 0x6b)
                goto spell6b;
            if (v == 0x69)
                goto spell69;
            return;
        }
        goto lookup;
    }
    else if (kind == 3)
    {
        sub = *(short*)(self + (slotIndex + (page * 5 + 0xd084) * 2) * 4);
        switch (sub)
        {
        case 0:
            v = *(short*)(self + 0xff4);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37E0);
                else
                    image = EQ_GLOBAL(void*, 0x006B37C8);
                goto drawElem;
            }
            break;
        case 1:
            v = *(short*)(self + 0xff6);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37E4);
                else
                    image = EQ_GLOBAL(void*, 0x006B37CC);
                goto drawElem;
            }
            break;
        case 2:
            v = *(short*)(self + 0xff8);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37E8);
                else
                    image = EQ_GLOBAL(void*, 0x006B37D0);
                goto drawElem;
            }
            break;
        case 3:
            v = *(short*)(self + 0xffa);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37EC);
                else
                    image = EQ_GLOBAL(void*, 0x006B37D4);
                goto drawElem;
            }
            break;
        case 4:
            v = *(short*)(self + 0xffc);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37F0);
                else
                    image = EQ_GLOBAL(void*, 0x006B37D8);
                goto drawElem;
            }
            break;
        case 5:
            v = *(short*)(self + 0xffe);
            if (v == -1)
            {
                if (OVL_SELECTED_guess)
                    image = EQ_GLOBAL(void*, 0x006B37F4);
                else
                    image = EQ_GLOBAL(void*, 0x006B37DC);
                goto drawElem;
            }
            break;
        default:
            return;
        }
        if (v > 100)
        {
            if (v == 0x6f)
                goto spell6f;
            if (v == 0x6b)
                goto spell6b;
            if (v == 0x69)
                goto spell69;
            if (v != 0x72)
                return;
            if (OVL_SELECTED_guess
                || *(unsigned*)(g_displayManager_guess + 0xbc)
                    < *(unsigned*)(*(char**)(EQ_GLOBAL(char*, 0x006E4D8C) + 0x70) + 0xf8))
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), 0x24b, 0x30, 0x273, 0x58);
            }
            else
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), 0x223, 0x30, 0x24b, 0x58);
            }
            OVL_DRAW_guess(EQ_GLOBAL(void*, 0x006B341C), OVL_ELEM_guess(slotIndex));
            OVL_TEXT_guess((const char*)0x005589e0, OVL_ELEM_X_guess + 2, OVL_ELEM_Y_guess + 8, 0,
                EQ_GLOBAL(int, 0x006EFDF8));
            OVL_TEXT_guess((const char*)0x00540678, OVL_ELEM_X_guess + 6, OVL_ELEM_Y_guess + 0x12, 0,
                EQ_GLOBAL(int, 0x006EFDF8));
            return;
        }
        goto lookup;
    }
    else if (kind == 4)
    {
        *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
        if (OVL_SELECTED_guess)
            SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), 0x24b, 0x30, 0x273, 0x58);
        else
            SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), 0x223, 0x30, 0x24b, 0x58);
        OVL_DRAW_guess(EQ_GLOBAL(void*, 0x006B341C), OVL_ELEM_guess(slotIndex));
        len = strlen(OVL_NAME_guess);
        width = 0;
        if ((short)len > 0)
        {
            p = OVL_NAME_guess;
            count = (short)len;
            do
            {
                if (*p > 0)
                    width = width + *(short*)(EQ_GLOBAL(char*, 0x006EFDDC) + *p * 4);
                p++;
                count--;
            } while (count != 0);
        }
        width = width >> 1;
        if (width > 0x11)
            width = 0x11;
        OVL_TEXT_guess(OVL_NAME_guess, OVL_ELEM_X_guess - width + 0x14, OVL_ELEM_Y_guess + 0xf,
            (int)(((unsigned)(OVL_SUB_guess % 12) & 0xffff0000) | (unsigned short)(short)*(char*)(OVL_NAME_guess + 0x135)),
            EQ_GLOBAL(int, 0x006EFDF8));
        return;
    }
    else if (kind == 5)
    {
        sub = *(short*)(self + (slotIndex + (page * 5 + 0xd084) * 2) * 4);
        switch (sub)
        {
        case 1: OVL_RECT5_guess(0x006E4DE8, 0x117, 9, 0x146, 0x38);
        case 2: OVL_RECT5_guess(0x006E4DE8, 0x1aa, 9, 0x1d9, 0x38);
        case 3: OVL_RECT5_guess(0x006E4DE8, 0x179, 9, 0x1a8, 0x38);
        case 4: OVL_RECT5_guess(0x006E4DE8, 0x1db, 9, 0x20a, 0x38);
        case 5: OVL_RECT5_guess(0x006E4DE8, 0x148, 9, 0x177, 0x38);
        case 6: OVL_RECT5_guess(0x006E4DE8, 0x117, 0x8d, 0x146, 0xbc);
        case 7: OVL_RECT5_guess(0x006E4DE8, 0x179, 0x4b, 0x1a8, 0x7a);
        case 8: OVL_RECT5_guess(0x006E4DE8, 0x179, 0x8d, 0x1a8, 0xbc);
        case 9: OVL_RECT5_guess(0x006E4DE8, 0x148, 0x4b, 0x177, 0x7a);
        case 10: OVL_RECT5_guess(0x006E4DE8, 0x1db, 0x4b, 0x209, 0x7a);
        case 11: OVL_RECT5_guess(0x006E4DE8, 0x179, 0xcf, 0x1a8, 0xfe);
        case 12: OVL_RECT5_guess(0x006E4DE8, 0x1aa, 0x4b, 0x1d9, 0x7a);
        case 13: OVL_RECT5_guess(0x006E4DE8, 0x117, 0xcf, 0x146, 0xfe);
        case 14: OVL_RECT5_guess(0x006E4DE8, 0x148, 0xcf, 0x177, 0xfe);
        case 15: OVL_RECT5_guess(0x006E4DE8, 0x117, 0x4b, 0x146, 0x7a);
        case 16: OVL_RECT5_guess(0x006E4DE8, 0x20c, 0x4b, 0x23b, 0x7a);
        case 17: OVL_RECT5_guess(0x006E4DE8, 0x148, 0x8d, 0x177, 0xbc);
        case 18: OVL_RECT5_guess(0x006E4DE8, 0x1db, 0x8d, 0x20a, 0xbc);
        case 19: OVL_RECT5_guess(0x006E4DE8, 0x20c, 0x8d, 0x23b, 0xbc);
        case 20: OVL_RECT5_guess(0x006E4DE8, 0x1aa, 0x8d, 0x1d9, 0xbc);
        case 21: OVL_RECT5_guess(0x006E4DE8, 0x1aa, 0xcf, 0x1d9, 0xfe);
        case 22: OVL_RECT5_guess(0x006E4E3C, 0x85, 0xfe, 0xb4, 0x12d);
        case 23: OVL_RECT5_guess(0x006E4E3C, 0x85, 0x12f, 0xb4, 0x15e);
        case 24: OVL_RECT5_guess(0x006E4E3C, 0x85, 0x160, 0xb4, 0x18f);
        case 25: OVL_RECT5_guess(0x006E4E3C, 0x85, 0x191, 0xb4, 0x1c0);
        case 26: OVL_RECT5_guess(0x006E4E3C, 0xb6, 0xfe, 0xe4, 0x12d);
        case 27: OVL_RECT5_guess(0x006E4E3C, 0xb6, 0x12f, 0xe5, 0x15e);
        case 28: OVL_RECT5_guess(0x006E4E3C, 0xb6, 0x160, 0xe5, 0x18f);
        case 29: OVL_RECT5_guess(0x006E4E3C, 0xb6, 0x191, 0xe5, 0x1c0);
        default:
            goto items;
        }
        SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), left, top, right, bottom);
        SetRect(&rect, OVL_ELEM_X_guess - 3, OVL_ELEM_Y_guess - 3, OVL_ELEM_R_guess, OVL_ELEM_B_guess);
        OVL_DRAW_guess(EQ_GLOBAL(void*, 0x006B341C), &rect);
items:
        if (OVL_SUB_guess == -1)
            return;
        item = OVL_PCITEM_guess;
        if (item == 0)
            return;
        EQ_CDECL(void, 0x00438423, (void*, int))(OVL_ELEM_guess(slotIndex), *(short*)(item + 0x84));
        if (EQ_CDECL(int, 0x004A1D83, (char*))(OVL_PCITEM_guess) == 0)
            return;
        SetRect(&rect, OVL_ELEM_X_guess + 0x19, OVL_ELEM_Y_guess + 0x1d, OVL_ELEM_R_guess, OVL_ELEM_B_guess);
        OVL_DRAW_guess(EQ_GLOBAL(void*, 0x006B37FC), &rect);
        item = OVL_PCITEM_guess;
        sprintf(text, (const char*)0x0053e16c,
            ((int)*(unsigned char*)(item + 0xda) - 1) / *(char*)(item + 0xbb) + 1);
        OVL_TEXT_guess(text, OVL_ELEM_X_guess + 0x1e, OVL_ELEM_Y_guess + 0x22, 10, EQ_GLOBAL(int, 0x006EFDF4));
        return;
    }
    else if (kind == 6)
    {
        sub = *(short*)(self + (slotIndex + (page * 5 + 0xd084) * 2) * 4);
        switch (sub)
        {
        case 0:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
            bottom = 0xa0;
            top = 0x78;
            if (OVL_SELECTED_guess) { left = 0xa1; right = 0xc9; }
            else { left = 0x79; right = 0xa1; }
            break;
        case 1:
            if (*(char*)(self + 0xb5) != 0)
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                bottom = 0x1b8;
                top = 400;
                if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
                else { left = 0x93; right = 0xbb; }
            }
            else
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
                bottom = 0x78;
                top = 0x50;
                if (OVL_SELECTED_guess) { left = 0x45; right = 0x6d; }
                else { left = 0x1d; right = 0x45; }
            }
            break;
        case 2:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
            bottom = 0xf0;
            top = 200;
            if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
            else { left = 0x93; right = 0xbb; }
            break;
        case 3:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
            bottom = 0x118;
            top = 0xf0;
            if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
            else { left = 0x93; right = 0xbb; }
            break;
        case 4:
            if (*(char*)(EQ_GLOBAL(char*, 0x006E4DA0) + 0xa1) == 'n')
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                bottom = 200;
                top = 0xa0;
                if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
                else { left = 0x93; right = 0xbb; }
            }
            else
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                bottom = 0x168;
                top = 0x140;
                if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
                else { left = 0x93; right = 0xbb; }
            }
            break;
        case 5:
            if (EQ_GLOBAL(char, 0x006B6405) != 0)
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                bottom = 0x78;
                top = 0x50;
                if (OVL_SELECTED_guess) { left = 0xa1; right = 0xc9; }
                else { left = 0x79; right = 0xa1; }
            }
            else
            {
                *(int*)EQ_GLOBAL(void*, 0x006B341C) = *(int*)(EQ_GLOBAL(char*, 0x006E4E40) + 4);
                bottom = 0x1e0;
                top = 0x1b8;
                if (OVL_SELECTED_guess) { left = 0xbb; right = 0xe3; }
                else { left = 0x93; right = 0xbb; }
            }
            break;
        case 6:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0x50;
            top = 0x28;
            if (OVL_SELECTED_guess) { left = 0x95; right = 0xbd; }
            else { left = 0x6d; right = 0x95; }
            break;
        case 7:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0x78;
            top = 0x50;
            if (OVL_SELECTED_guess) { left = 0x95; right = 0xbd; }
            else { left = 0x6d; right = 0x95; }
            break;
        case 8:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0x28;
            top = 0;
            if (OVL_SELECTED_guess) { left = 0xe5; right = 0x10d; }
            else { left = 0xbd; right = 0xe5; }
            break;
        case 9:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0x50;
            top = 0x28;
            if (OVL_SELECTED_guess) { left = 0xe5; right = 0x10d; }
            else { left = 0xbd; right = 0xe5; }
            break;
        case 10:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0x78;
            top = 0x50;
            if (OVL_SELECTED_guess) { left = 0xe5; right = 0x10d; }
            else { left = 0xbd; right = 0xe5; }
            break;
        case 11:
            *(int*)EQ_GLOBAL(void*, 0x006B341C) = pinstEQ_TextIO_guess->displayOverlay_int_guess;
            bottom = 0xa0;
            top = 0x78;
            if (!OVL_SELECTED_guess && EQ_GLOBAL(char, 0x006B658C) == 0) { left = 0xbd; right = 0xe5; }
            else { left = 0xe5; right = 0x10d; }
            break;
        default:
            return;
        }
        SetRect((RECT*)(EQ_GLOBAL(char*, 0x006B341C) + 4), left, top, right, bottom);
        image = EQ_GLOBAL(void*, 0x006B341C);
        goto drawElem;
    }
    return;

lookup:
    item = g_skill_records_guess[v];
    if (*(char*)(item + 0x26) == 2)
    {
        if (OVL_SELECTED_guess || pinstEQ_Main_guess->combatSkillReady_guess == 0)
            image = EQ_ARRAY(void*, 0x006B32B4)[*(short*)(item + 0x8c)];
        else
            image = EQ_ARRAY(void*, 0x006B32B4)[*(short*)(item + 0x8a)];
    }
    else
    {
        if (OVL_SELECTED_guess || EQ_ARRAY(char, 0x006E117C)[v] == 0)
            image = EQ_ARRAY(void*, 0x006B32B4)[*(short*)(item + 0x8c)];
        else
            image = EQ_ARRAY(void*, 0x006B32B4)[*(short*)(item + 0x8a)];
    }
    goto drawElem;

spell6f:
    if (OVL_SELECTED_guess || pinstEQ_Main_guess->combatSkillReady_guess == 0)
        image = EQ_GLOBAL(void*, 0x006B3480);
    else
        image = EQ_GLOBAL(void*, 0x006B347C);
    goto drawElem;

spell6b:
    if (OVL_SELECTED_guess
        || *(unsigned*)(g_displayManager_guess + 0xbc)
            < *(unsigned*)(*(char**)(EQ_GLOBAL(char*, 0x006E4D8C) + 0x70) + 0xf8))
        image = EQ_GLOBAL(void*, 0x006B34E8);
    else
        image = EQ_GLOBAL(void*, 0x006B34E4);
    goto drawElem;

spell69:
    if (OVL_SELECTED_guess
        || *(unsigned*)(g_displayManager_guess + 0xbc)
            < *(unsigned*)(*(char**)(EQ_GLOBAL(char*, 0x006E4D8C) + 0x70) + 0xf8))
        image = EQ_GLOBAL(void*, 0x006B34F0);
    else
        image = EQ_GLOBAL(void*, 0x006B34EC);

drawElem:
    OVL_DRAW_guess(image, OVL_ELEM_guess(slotIndex));
}

#undef OVL_ELEM_guess
#undef OVL_ELEM_X_guess
#undef OVL_ELEM_Y_guess
#undef OVL_ELEM_R_guess
#undef OVL_ELEM_B_guess
#undef OVL_SUB_guess
#undef OVL_NAME_guess
#undef OVL_SELECTED_guess
#undef OVL_DRAW_guess
#undef OVL_TEXT_guess
#undef OVL_PCITEM_guess
#undef OVL_RECT5_guess

// FUNCTION: TRILOGY 0x0047d4e5
// Word-wraps the text at +0x1a6 to 400 width units and draws each line; stores the line count at +0xff.
void __fastcall HandleMessageBoardMessage_guess(char* boardEntry)
{
    char out[2048];
    char text[2048];
    int lines;
    int width;
    short space;
    short pos;
    short len;
    int i;

    width = 0;
    boardEntry[0xff] = 0;
    lines = 0;
    strcpy(text, boardEntry + 0x1a6);
    len = (short)strlen(text);
    pos = 0;
    if (len > 0)
    {
        do
        {
            if (len < 1)
                break;
            if (width > 400)
            {
                width = 0;
                space = 0;
                for (i = pos; i > pos - 10; i--)
                {
                    if (space == 0 && text[i] == ' ')
                        space = (short)(i + 1);
                }
                if (space == 0)
                    space = pos;
                strcpy(out, (const char*)0x0058fba4);
                strncpy(out, text, space);
                out[space] = 0;
                strcpy(text, text + space);
                width = 0;
                pos = 0;
                len = (short)strlen(text);
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(out, 0x4b, ((short)lines + 10) * 15,
                     (boardEntry[0xfe] == 0 && boardEntry[0xfe0] != '\n') ? 4 : 0,
                     EQ_GLOBAL(int, 0x006EFDF8), 1);
                lines++;
            }
            if (text[pos] > 0)
                width += EQ_GLOBAL(int*, 0x006EFDDC)[text[pos]];
            pos++;
        } while (pos < len);
    }
    if (strlen(text) != 0)
    {
        if (boardEntry[0xfe0] == '\n')
        {
            strcpy(out, text);
        }
        else
        {
            strcpy(out, text);
            strcat(out, (const char*)0x005459a4);
        }
        EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), g_eqtexto_guess)(out, 0x4b, ((short)lines + 10) * 15,
             (boardEntry[0xfe] == 0 && boardEntry[0xfe0] != '\n') ? 4 : 0,
             EQ_GLOBAL(int, 0x006EFDF8), 1);
        lines++;
    }
    boardEntry[0xff] = (char)lines;
}

// FUNCTION: TRILOGY 0x0047d6e2
// Frees 8 stored buffers, sends opcode 0x2113 (1 byte), clears flag +0x685E8, restores the prior view mode.
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
void __fastcall EQ_Main_ClearHouseBank_guess(EQ_Main* state, int unused)
{
    int i;
    int remainingEntries;
    int* entry;
    void (__fastcall **vtable)(void*);

    for (i = 0; i < 4; i++)
        ((int*)state)[0x4a + i] = 0;
    entry = (int*)state + 0x42;
    remainingEntries = 8;
    do
    {
        if (*entry != 0)
        {
            operator delete( (void*)*entry );
            *entry = 0;
        }
        entry = entry + 1;
        remainingEntries = remainingEntries - 1;
    } while (remainingEntries != 0);

    send_message_guess(
        g_world_connection_guess,
        0x2113,
        (void*)(EQ_GLOBAL(u32, 0x006E4DCC) + 0xC),
        1,
        1);
    EQ_GLOBAL(int, 0x006E4DBC) = 0;
    EQ_GLOBAL(int, 0x006E4DCC) = 0;

    vtable = (void (__fastcall **)(void*))*(int*)state;
    vtable[2](state);
    state->screenMode_u8_guess = state->prevScreenMode_u8_guess;
    state->bankOpen_guess = 0;
    state->houseBank_u8_guess = 0;
    vtable = (void (__fastcall **)(void*))*(int*)state;
    vtable[1](state);

    if (EQ_GLOBAL(int, 0x00590170) == 2)
    {
        g_game_state_guess = 2;
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            pinstEQ_Main_guess->uiDirty_guess = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
    }
}

#define EQ_MAIN_ESCAPE_VCALL(obj, slot) \
    (((void (__fastcall *)(void*))(*(int**)(obj))[slot])((void*)(obj)))

// FUNCTION: TRILOGY 0x0047d796
// Cancel/back-out step: closes the top window or mode, aborts spell scribe/memorize; refuses during a trade
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: post-switch tail and case 5 blocks are placed after case 5/6 (original: after case 0 and 5); F-loop +4 store order; dummy-EDX xor from EQ_THISCALL
void EQ_Main::ProcessEscape(void)
{
    int* eqMain = (int*)this;
    char stateByte;
    int* ptr;
    int remaining;
    char chatText[128];
    struct EQ_Main_ProcessEscape_big_guess
    {
        int local_68;
        int local_64;
        int local_60;
        int pad[20];
    } big;
    struct EQ_Main_ProcessEscape_message_guess
    {
        int local_c;
        u32 local_8;
    } message;

    if (eqMain[0x25] != 0)
    {
        return;
    }
    if (eqMain[0x1a17a] != 0)
    {
        if (*(u8*)((char*)eqMain + 0x101) != 0)
        {
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
            {
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
            }
            EQ_METHOD(0, void, 0x0047D6E2, (void*), eqMain)();
            return;
        }
        EQ_METHOD(1, void, 0x004C53D8, (void*, int), g_everquest_guess)(1);
        return;
    }
    if (eqMain[0x1a178] != 0 && EQ_GLOBAL(int, 0x006E4DB8) != 0)
    {
        EQ_METHOD(0, void, 0x00464AB6, (void*), eqMain)();
        return;
    }
    if (eqMain[0x1a177] != 0 && EQ_GLOBAL(int, 0x006E4DC0) != 0)
    {
        EQ_METHOD(0, void, 0x00464C8D, (void*), eqMain)();
        return;
    }
    if (eqMain[0x1a179] != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(EQ_ARRAY(char, 0x0055AD64), 0xF);
        return;
    }
    if (eqMain[0x1a17d] != 0)
    {
        if (EQ_GLOBAL(short, 0x006B6772) == 0x35)
        {
            EQ_GLOBAL(short, 0x006B6772) = -1;
        }
        ptr = eqMain + 0x1a17f;
        remaining = 1000;
        do
        {
            ptr[-1] = 0;
            *ptr = 0xBF800000;
            ptr[1] = -1;
            ptr[2] = 0;
            ptr = ptr + 4;
            remaining = remaining - 1;
        } while (remaining != 0);
        EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
        *(u8*)((char*)eqMain + 0xFE0) = 0;
        EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
        return;
    }
    else
    {
        ptr = EQ_GLOBAL(int*, 0x006018C0);
        if (ptr != 0 && ptr[4] != 0)
        {
            EQ_MAIN_ESCAPE_VCALL(ptr, 2);
            return;
        }
        if (EQ_GLOBAL(int*, 0x00604984)[4] != 0)
        {
            EQ_GLOBAL(int*, 0x00604984)[0x7A] = 1;
            ptr = EQ_GLOBAL(int*, 0x00604984);
            EQ_MAIN_ESCAPE_VCALL(ptr, 2);
            return;
        }
        if (EQ_GLOBAL(int*, 0x006E4E60)[4] != 0)
        {
            *(u8*)(EQ_GLOBAL(int*, 0x006E4E60)[0xDEB] + 0xAD) = 0;
            EQ_MAIN_ESCAPE_VCALL(EQ_GLOBAL(int*, 0x006E4E60), 2);
            EQ_GLOBAL(int*, 0x006E4E60)[0xDEB] = 0;
            return;
        }
        if (EQ_GLOBAL(int*, 0x006E4E5C)[4] != 0)
        {
            *(u8*)(EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] + 0xD6) = 0;
            EQ_MAIN_ESCAPE_VCALL(EQ_GLOBAL(int*, 0x006E4E5C), 2);
            EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] = 0;
            return;
        }
        if (EQ_GLOBAL(int*, 0x006E4E58)[4] != 0)
        {
            *(u8*)(EQ_GLOBAL(int*, 0x006E4E58)[0x5D] + 0xD6) = 0;
            EQ_MAIN_ESCAPE_VCALL(EQ_GLOBAL(int*, 0x006E4E58), 2);
            EQ_GLOBAL(int*, 0x006E4E58)[0x5D] = 0;
            return;
        }
        switch (*(char*)((char*)eqMain + 0xFE0))
        {
        case 0:
            stateByte = *(char*)((char*)eqMain + 0x8FBAC);
            if (stateByte == 0)
            {
                return;
            }
            break;
        case 1:
            stateByte = *(char*)((char*)eqMain + 0x8FBAC);
            if (stateByte == 0)
            {
                ptr = eqMain + 0x1a176;
                if (*ptr != 0)
                {
                    *ptr = 0;
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                    return;
                }
                EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                *(u8*)((char*)eqMain + 0xFE0) = 0;
                EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                return;
            }
            break;
        case 2:
            stateByte = *(char*)((char*)eqMain + 0x8FBAC);
            if (stateByte == 0)
            {
                if (eqMain[0x1a175] != 0)
                {
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                    eqMain[0x1a175] = 0;
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                    return;
                }
                EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                *(u8*)((char*)eqMain + 0xFE0) = 0;
                EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                return;
            }
            break;
        case 3:
            stateByte = *(char*)((char*)eqMain + 0x8FBAC);
            if (stateByte == 0)
            {
                ptr = eqMain + 0x17c63;
                if (*ptr != 0)
                {
                    *ptr = 0;
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                    EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                    return;
                }
                EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                *(u8*)((char*)eqMain + 0xFE0) = 0;
                EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                return;
            }
            break;
        case 4:
            if (eqMain[0x23ee9] != 0)
            {
                eqMain[0x23ee9] = 0;
                eqMain[0x23ee5] = 0;
                eqMain[0x23ee7] = 0;
                eqMain[0x23ee8] = 0;
                eqMain[0x23ee6] = 0;
                EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
                EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
                return;
            }
            EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
            *(short*)((char*)eqMain + 0x5F186) = -1;
            *(short*)(eqMain + 0x17c62) = -1;
            *(short*)((char*)eqMain + 0x5F18A) = -1;
            *(u8*)((char*)eqMain + 0xFE0) = *(u8*)((char*)eqMain + 0xFE2);
            EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
            if (EQ_GLOBAL(int, 0x00590170) != 2)
            {
                return;
            }
            g_game_state_guess = 2;
            if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
            {
                EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
            }
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            return;
        case 5:
            stateByte = *(char*)((char*)eqMain + 0x8FBAC);
            if (stateByte != 0)
            {
                break;
            }
            else
            {
                if (*(short*)((char*)eqMain + 0x1006) != -1)
                {
                    sprintf(
                        chatText, EQ_ARRAY(char, 0x0055B0FC));
                    *(short*)((char*)eqMain + 0x1006) = -1;
                    *(short*)(eqMain + 0x1b153) = 0;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatText, 0x108);
                    return;
                }
                if (*(short*)((char*)eqMain + 0x1004) != -1)
                {
                    sprintf(
                        chatText, EQ_ARRAY(char, 0x0055B0DC));
                    *(short*)((char*)eqMain + 0x1004) = -1;
                    *(short*)((char*)eqMain + 0x6C54E) = 0;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(chatText, 0x108);
                    *(short*)(eqMain + 0x3FA) = 0;
                    *(u8*)(g_displayManager_guess + 0x3C) = 0;
                    return;
                }
                eqMain[0x1b155] = *(int*)(g_displayManager_guess + 0xBC);
                return;
            }
        case 6:
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
            {
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
            }
            message.local_c = *(int*)(EQ_GLOBAL(char*, 0x006E4DC4) + 0x80);
            message.local_8 = g_local_player_guess->spawn_id;
            send_message_guess(
                g_world_connection_guess, 0x209D, &message, 8, 1);
            EQ_MAIN_ESCAPE_VCALL(EQ_GLOBAL(int*, 0x006E4E44), 2);
            pinstEQ_Main_guess->screenMode_u8_guess = pinstEQ_Main_guess->prevScreenMode_u8_guess;
            EQ_MAIN_ESCAPE_VCALL(EQ_GLOBAL(int*, 0x006E4E44), 1);
            EQ_GLOBAL(int, 0x006B7120) = 0x14;
            EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), g_everquest_guess)(0, 1);
            EQ_GLOBAL(int*, 0x006E4E44)[0x397] = EQ_GLOBAL(int*, 0x006E4E44)[0x397] - 1;
            if (EQ_GLOBAL(int*, 0x006E4E44)[0x397] != 0)
            {
                big.local_68 = -4;
                big.local_64 = EQ_GLOBAL(int*, 0x006E4E44)[0x397];
                big.local_60 = 2;
                send_message_guess(
                    g_world_connection_guess, 0x2147, &big, 0x5C, 1);
            }
            EQ_GLOBAL(int*, 0x006E4E44)[0x397] = 0;
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                g_game_state_guess = 2;
                if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
                {
                    EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                }
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            }
            EQ_GLOBAL(int, 0x006E4DC4) = 0;
            return;
        case 10:
        case 11:
            if (*(char*)((char*)eqMain + 0x100) != 0)
            {
                return;
            }
            EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
            *(u8*)((char*)eqMain + 0xFE0) = 9;
            EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
            return;
        case 9:
        case 12:
            if (*(char*)((char*)eqMain + 0x100) != 0)
            {
                return;
            }
            EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
            *(u8*)((char*)eqMain + 0xFE0) = *(u8*)((char*)eqMain + 0xFE1);
            EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
            if (EQ_GLOBAL(int, 0x00590170) != 2)
            {
                return;
            }
            g_game_state_guess = 2;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            return;
        default:
            return;
        }
        if (stateByte != 1)
        {
            EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
            if (*(char*)((char*)eqMain + 0x100D) == 0)
            {
                *(u8*)((char*)eqMain + 0x8FBAC) = 1;
            }
            else
            {
                *(char*)((char*)eqMain + 0x100D) = *(char*)((char*)eqMain + 0x100D) - 1;
            }
        }
        else
        {
            if (EQ_GLOBAL(int, 0x00590170) == 2 && *(char*)((char*)eqMain + 0xFE0) != 5)
            {
                g_game_state_guess = 2;
                if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
                {
                    EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
                }
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            }
            EQ_MAIN_ESCAPE_VCALL(eqMain, 2);
            *(u8*)((char*)eqMain + 0x8FBAC) = 0;
            EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
            EQ_METHOD(0, void, 0x004CF31A, (void*), g_everquest_guess)();
            return;
        }
    }
    EQ_MAIN_ESCAPE_VCALL(eqMain, 1);
}
#undef EQ_MAIN_ESCAPE_VCALL

// FUNCTION: TRILOGY 0x0047dd6c
// Hot button alt action: toggles a bag, right-clicks gear, opens a skill/edit page, or sends gem op 0x2182
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: dummy-EDX xor and EBP vtable reg (EQ_THISCALL); -1 stores as or word ptr (original keeps -1 in EBX); redundant pEVar2 test dropped
/* fake match: unused (the caller's EDX, which IsActive ignores) is the dummy for the first IsActive calls; 0 costs an xor and the ECX load stays below the branch */
void __fastcall EQ_Main_doHotButtonRightClick_guess(EQ_Main* eqMain, int unused, short param_2)
{
    char buttonType;
    EQ_Item* pEVar2;
    u16 itemSlot;
    int iVar4;
    int* overlayPtr;
    int buttonIndex;
    void (__fastcall **vtable)(void*, int);
    void (__fastcall **vtable4)(void*, int, int);
    struct EQ_Main_doHotButtonRightClick_message_guess
    {
        int iStack_c;
        int iStack_8;
        u32 uStack_4;
    } message;

    buttonIndex = (int)param_2;
    if (*(char*)((char*)eqMain + (buttonIndex + eqMain->hotPage_guess * 10) * 4 + 0x680FE) == 5)
    {
        if (EQ_GLOBAL(int, 0x006B6498) == 0 &&
            (pEVar2 = ((EQ_Item**)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0xE0))[(short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess * 5 + 0xD020) * 2]]) != 0 &&
            pEVar2->type == 1)
        {
            if (pEVar2[1].lore[7] == 1)
            {
                iVar4 = EQ_THISCALL(int, 0x004504AC, (void*, int))(EQ_GLOBAL(void*, 0x006E4E58), unused);
                if (iVar4 != 0 && EQ_GLOBAL(int*, 0x006E4E58)[0x5D] == (int)pEVar2)
                {
                    pEVar2[1].lore[7] = 0;
                    vtable = (void (__fastcall **)(void*, int))*EQ_GLOBAL(int*, 0x006E4E58);
                    vtable[2](EQ_GLOBAL(void*, 0x006E4E58), 0);
                    EQ_GLOBAL(int*, 0x006E4E58)[0x5D] = 0;
                }
                else
                {
                    iVar4 = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))();
                    if (iVar4 != 0 && EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] == (int)pEVar2)
                    {
                        pEVar2[1].lore[7] = 0;
                        vtable = (void (__fastcall **)(void*, int))*EQ_GLOBAL(int*, 0x006E4E5C);
                        vtable[2](EQ_GLOBAL(void*, 0x006E4E5C), 0);
                        EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] = 0;
                    }
                }
            }
            else
            {
                iVar4 = EQ_THISCALL(int, 0x004504AC, (void*, int))(EQ_GLOBAL(void*, 0x006E4E58), unused);
                if (iVar4 == 0)
                {
                    pEVar2[1].lore[7] = 1;
                    EQ_GLOBAL(int*, 0x006E4E58)[0x5D] = (int)pEVar2;
                    vtable4 = (void (__fastcall **)(void*, int, int))*EQ_GLOBAL(int*, 0x006E4E58);
                    vtable4[4](EQ_GLOBAL(void*, 0x006E4E58), 0,
                        (short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10] + -0x16);
                    itemSlot = (u16)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
                    overlayPtr = EQ_GLOBAL(int*, 0x006E4E58);
                }
                else
                {
                    iVar4 = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))();
                    if (iVar4 != 0)
                    {
                        /* fake match: this goto skips the shared store, as the original's failed-check jump does */
                        goto checkSlotItem;
                    }
                    pEVar2[1].lore[7] = 1;
                    EQ_GLOBAL(int*, 0x006E4E5C)[0x5D] = (int)pEVar2;
                    vtable4 = (void (__fastcall **)(void*, int, int))*EQ_GLOBAL(int*, 0x006E4E5C);
                    vtable4[4](EQ_GLOBAL(void*, 0x006E4E5C), 0,
                        (short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10] + -0x16);
                    itemSlot = (u16)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
                    overlayPtr = EQ_GLOBAL(int*, 0x006E4E5C);
                }
                *(u16*)((char*)overlayPtr + 0x92) = itemSlot;
            }
        }
checkSlotItem:
        pEVar2 = ((EQ_Item**)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0xE0))[(short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10]];
        if (pEVar2 != 0 && pEVar2->type == 0 && pEVar2 != 0)
        {
            EQ_METHOD(3, void, 0x0046AB71, (void*, EQ_Item*, EQ_Item*, short), eqMain)(pEVar2, pEVar2, (short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10]);
        }
    }
    buttonType = *(char*)((char*)eqMain + (buttonIndex + eqMain->hotPage_guess * 10) * 4 + 0x680FE);
    if (buttonType == 2)
    {
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        vtable = (void (__fastcall **)(void*, int))*(int*)eqMain;
        vtable[2](eqMain, 0);
        eqMain->combatSkillPickerOpen_guess = 1;
        *(short*)((int *)eqMain + 0x1B15A) = (short)(char)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
        eqMain->clickedButton_u16_guess = 0xFFFF;
        *(short*)((int *)eqMain + 0x17C62) = -1;
        eqMain->releasedButton_u16_guess = 0xFFFF;
        *(u8*)((int *)eqMain + 0x3F8) = 2;
        ((void (__fastcall **)(void*, int))*(int*)eqMain)[1](eqMain, 0);
    }
    else if (buttonType == 3)
    {
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        vtable = (void (__fastcall **)(void*, int))*(int*)eqMain;
        vtable[2](eqMain, 0);
        eqMain->abilityPickerOpen_guess = 1;
        *(short*)((int *)eqMain + 0x1B15A) = (short)(char)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
        eqMain->clickedButton_u16_guess = 0xFFFF;
        *(short*)((int *)eqMain + 0x17C62) = -1;
        eqMain->releasedButton_u16_guess = 0xFFFF;
        *(u8*)((int *)eqMain + 0x3F8) = 1;
        ((void (__fastcall **)(void*, int))*(int*)eqMain)[1](eqMain, 0);
    }
    else if (buttonType == 4)
    {
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        vtable = (void (__fastcall **)(void*, int))*(int*)eqMain;
        vtable[2](eqMain, 0);
        EQ_GLOBAL(u8, 0x006B6404) = 0;
        *(u8*)((char*)g_everquest_guess + 0x13EC) = 1;
        buttonType = (char)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
        *(char*)((int *)eqMain + 0x17C65) = buttonType;
        memcpy(
            (char*)eqMain + 0x682E6,
            ((int)buttonType % 0xC + ((int)buttonType / 0xC) * 0xC) * 0x136 + 0x5F196 + (char*)eqMain,
            0x136);
        *(u8*)((char*)eqMain + 0x5F195) = 0xFF;
        vtable = (void (__fastcall **)(void*, int))*(int*)eqMain;
        vtable[2](eqMain, 0);
        *(u8*)((int *)eqMain + 0x3F8) = 3;
        eqMain->clickedButton_u16_guess = 0xFFFF;
        *(short*)((int *)eqMain + 0x17C62) = -1;
        eqMain->releasedButton_u16_guess = 0xFFFF;
        eqMain->socialEditorOpen_guess = 1;
        ((void (__fastcall **)(void*, int))*(int*)eqMain)[1](eqMain, 0);
    }
    else if (buttonType == 7)
    {
        if (EQ_GLOBAL(int, 0x006B6498) != 0)
        {
            return;
        }
        if (((short*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x952))[(short)((int*)eqMain)[buttonIndex + (*(char*)((char*)eqMain + 0x6841D) * 5 + 0xD020) * 2]] == -1)
        {
            return;
        }
        if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
        {
            return;
        }
        pinstEQ_Main_guess->pendingWorldRequests_guess = 1;
        message.iStack_c = (int)(short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10];
        message.iStack_8 = (int)((short*)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0x952))[(short)((int*)eqMain)[buttonIndex + (eqMain->hotPage_guess + 0x29A0) * 10]];
        message.uStack_4 = 2;
        send_message_guess(
            g_world_connection_guess, 0x2182, &message, 0xC, 1);
        return;
    }
}

typedef void (__fastcall *tOverlayNoArgMethod_guess)(void* object);

// FUNCTION: TRILOGY 0x0047E1E3
// Presses a hot button: runs its command, skill, menu entry or spell gem, or picks up/equips its item slot
// ORIGINAL FILE: EverQuest/EQ_Overlay.cpp (from Mac debug map)
// @early-stop: VC6 keeps 2 (not -1) in EBP, so the -1 stores and 2-compares differ; command test is not a switch (it brings an EBP frame); 2nd equip tail loads SI.
void __fastcall EQ_Main_doHotButton_guess(void* self, int edxDummy, int hotButton, int clickType)
{
    short slotIndex;
    EQ_Item* item;
    char action;
    int page;
    int buttonIndex;
    int inventoryIndex;
    int slotValue;
    char screenState;
    int spellCastBusy;
    char* mainObject;

    if ((EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != 0) &&
        (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->stand_state == 'f'))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
            EQ_ARRAY(char, 0x0055C1C8), 0x111);
        return;
    }

    {
        void* overlay = EQ_GLOBAL(void*, 0x006018C0);
        if ((overlay != 0) && (*(int*)((char*)overlay + 0x10) != 0))
        {
            ((tOverlayNoArgMethod_guess)(*(u32**)overlay)[2])(overlay);
        }
    }
    {
        void* overlay = EQ_GLOBAL(void*, 0x00604984);
        if ((overlay != 0) && (*(int*)((char*)overlay + 0x10) != 0))
        {
            *(int*)((char*)overlay + 0x1E8) = 1;
            overlay = EQ_GLOBAL(void*, 0x00604984);
            ((tOverlayNoArgMethod_guess)(*(u32**)overlay)[2])(overlay);
        }
    }

    mainObject = (char*)EQ_GLOBAL(void*, 0x006E4E44);
    spellCastBusy = *(int*)(mainObject + 0x94);
    if (((spellCastBusy != 0) ||
         (((screenState = *(char*)(mainObject + 0xFE0)),
           (screenState > 3) && (screenState != 5))))
        && ((g_game_state_guess != 2) ||
            ((*(char*)(mainObject + 0xFE0) != 4) ||
             (*(int*)(mainObject + 0x685DC) != 0) ||
             (*(int*)(mainObject + 0x685E4) != 0) ||
             (*(int*)(mainObject + 0x685E0) != 0) ||
             (((screenState = 4), *(int*)(mainObject + 0x685E8) != 0)))))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), g_everquest_guess)(
            EQ_ARRAY(char, 0x0055C1C8), 0x111);
        return;
    }

    page = (int)*(char*)((char*)self + 0x6841D);
    buttonIndex = (int)(short)hotButton;
    action = *(char*)((char*)self + (buttonIndex + page * 10) * 4 + 0x680FE);

    if (action == 1)
    {
        int command = (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2];
        if (command == 0)
        {
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(8, 1, 0);
            return;
        }
        if (command != 1)
        {
            return;
        }
        if (*(int*)(mainObject + 0xEA4) == 0)
        {
            return;
        }
        *(int*)(mainObject + 0xEA4) = 0;
        EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, int, EQPlayer*),
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C))(0xB, 0, EQ_GLOBAL(EQPlayer*, 0x006E4DAC));
        return;
    }

    if (((action == 2) &&
         (*(short*)((char*)self + (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2] * 2 + 0xFEC) != -1)) ||
        ((action == 3) &&
         (*(short*)((char*)self + (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2] * 2 + 0xFF4) != -1)))
    {
        int actionOffset;

        if (action == 2)
        {
            actionOffset = (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2] * 2 + 0xFEC;
        }
        else
        {
            actionOffset = (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2] * 2 + 0xFF4;
        }
        EQ_METHOD(2, void, 0x0041D790, (CharacterProfile*, int, EQPlayer*),
            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(
                ((int)(*(char*)((char*)self + 0x6841D) >> 7) << 8) | (u8)*((char*)self + actionOffset),
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC));
        return;
    }

    if (action == 4)
    {
        if (EQ_GLOBAL(int, 0x0053F230) != -1)
        {
            return;
        }
        EQ_GLOBAL(int, 0x0053F230) = (int)(short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2] << 3;
        return;
    }

    if (action == 5)
    {
        if (clickType != 1)
        {
            EQ_METHOD(1, void, 0x0047DD6C, (void*, int), self)(hotButton);
            return;
        }
        if (*(char*)((char*)g_display_guess + 0x3D) != 0)
        {
            return;
        }

        item = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0];
        if (item == 0)
        {
            inventoryIndex = (short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2];
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[inventoryIndex] == 0)
            {
                return;
            }
            slotValue = EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[inventoryIndex]);
            if (slotValue != 0)
            {
                inventoryIndex = buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10;
                EQ_METHOD(2, void, 0x00486CB6, (void*, EQ_Item**, int),
                    EQ_GLOBAL(void*, 0x006018C0))(
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items +
                            (short)((int*)self)[inventoryIndex],
                        (int)(short)((int*)self)[inventoryIndex]);
                return;
            }

            inventoryIndex = buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10;
            item = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[
                (short)((int*)self)[inventoryIndex]];
            if (item->type == 1)
            {
                void* firstOverlay = EQ_GLOBAL(void*, 0x006E4E58);
                if ((*(int*)((char*)firstOverlay + 0x10) == 0) ||
                    (item != *(EQ_Item**)((char*)firstOverlay + 0x174)))
                {
                    void* secondOverlay = EQ_GLOBAL(void*, 0x006E4E5C);
                    if ((*(int*)((char*)secondOverlay + 0x10) != 0) &&
                        (item == *(EQ_Item**)((char*)secondOverlay + 0x174)))
                    {
                        *((char*)item + 0xD6) = 0;
                        secondOverlay = EQ_GLOBAL(void*, 0x006E4E5C);
                        ((tOverlayNoArgMethod_guess)(*(u32**)secondOverlay)[2])(secondOverlay);
                        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174) = 0;
                    }
                }
                else
                {
                    *((char*)item + 0xD6) = 0;
                    firstOverlay = EQ_GLOBAL(void*, 0x006E4E58);
                    ((tOverlayNoArgMethod_guess)(*(u32**)firstOverlay)[2])(firstOverlay);
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) = 0;
                }
            }

            EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, short, EQ_Item*, EQ_Item*))(
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8),
                    (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10],
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0],
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[
                        (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10]]);
            EQ_METHOD(4, void, 0x00463651, (void*, int, int, int, int),
                EQ_GLOBAL(void*, 0x006E4E44))(
                    (int)(short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10], 0, 1, 1);
            EQ_METHOD(2, void, 0x0040B521, (void*, EQPlayer*, short),
                g_display_guess)(
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                    (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10]);
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        }
        else if (EQ_CDECL(int, 0x004A2180,
                    (CharacterProfile*, u32, short, EQ_Item*))(
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), item->equipable_slots,
                        (short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2], item) != 0)
        {
            inventoryIndex = buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10;
            if (((short)((int*)self)[inventoryIndex] < 0x16) &&
                (EQ_METHOD(1, int, 0x0041BA72, (CharacterProfile*, EQ_Item*),
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0]) == 0))
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                    g_everquest_guess)(
                        EQ_ARRAY(char, 0x0055C1F0), 0xD);
            }
            else
            {
                item = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[
                    (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10]];
                if ((item != 0) && (item->type == 1))
                {
                    void* firstOverlay = EQ_GLOBAL(void*, 0x006E4E58);
                    if ((*(int*)((char*)firstOverlay + 0x10) == 0) ||
                        (item != *(EQ_Item**)((char*)firstOverlay + 0x174)))
                    {
                        void* secondOverlay = EQ_GLOBAL(void*, 0x006E4E5C);
                        if ((*(int*)((char*)secondOverlay + 0x10) != 0) &&
                            (item == *(EQ_Item**)((char*)secondOverlay + 0x174)))
                        {
                            *((char*)item + 0xD6) = 0;
                            secondOverlay = EQ_GLOBAL(void*, 0x006E4E5C);
                            ((tOverlayNoArgMethod_guess)(*(u32**)secondOverlay)[2])(secondOverlay);
                            *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174) = 0;
                        }
                    }
                    else
                    {
                        *((char*)item + 0xD6) = 0;
                        firstOverlay = EQ_GLOBAL(void*, 0x006E4E58);
                        ((tOverlayNoArgMethod_guess)(*(u32**)firstOverlay)[2])(firstOverlay);
                        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) = 0;
                    }
                }

                EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, short, EQ_Item*, EQ_Item*))(
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8),
                        (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10],
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0],
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[
                            (short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10]]);
                EQ_METHOD(4, void, 0x00463651, (void*, int, int, int, int),
                    EQ_GLOBAL(void*, 0x006E4E44))(
                        0, (int)(short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10], 1, 1);
                slotValue = ((u32)self & 0xFFFF0000) |
                    (u16)(short)((int*)self)[buttonIndex + (*(char*)((char*)self + 0x6841D) + 0x29A0) * 10];
                EQ_METHOD(2, void, 0x0040B521, (void*, EQPlayer*, int),
                    g_display_guess)(
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C), slotValue);
                EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
            }
        }

        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] != 0)
        {
            *(u8*)((char*)g_display_guess + 0x3C) = 1;
            return;
        }
        *(u8*)((char*)g_display_guess + 0x3C) = 0;
        return;
    }

    if (action != 6)
    {
        if (action != 7)
        {
            return;
        }

        slotIndex = (short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2];
        if ((((int*)self)[(int)slotIndex + 0x398] == 0) &&
            (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unknown0x248 == -1))
        {
            u32 memorizedSpell = (u16)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->spell_memorized[slotIndex];
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->spell_memorized[slotIndex] == -1)
            {
                return;
            }
            if (*(short*)((char*)self + 0xFE8) != 0)
            {
                return;
            }
            if (spellCastBusy != 0)
            {
                return;
            }
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
            {
                return;
            }
            memorizedSpell |= (u32)(slotIndex >> 15) << 16;
            slotValue = (memorizedSpell & 0xFFFFFF00) |
                (u8)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2];
            if (EQ_METHOD(4, char, 0x0041F381,
                (CharacterProfile*, int, u32, void*, int),
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(
                    slotValue, memorizedSpell, 0, -1) == 0)
            {
                return;
            }
            slotValue = 8;
            {
                int* spellButton = (int*)self + 0x398;
                do
                {
                    if (*spellButton == 0)
                    {
                        *spellButton = 1;
                    }
                    spellButton[8] = 0;
                    spellButton = spellButton + 1;
                    slotValue = slotValue - 1;
                } while (slotValue != 0);
            }
            return;
        }

        slotValue = EQ_METHOD(0, int, 0x0046A72F, (void*), self)();
        if (slotValue != 0)
        {
            EQ_METHOD(1, void, 0x0042798F, (CharacterProfile*, int),
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(0);
            return;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
            g_everquest_guess)(
                EQ_ARRAY(char, 0x0055ABF0), 0x108);
        return;
    }

    switch ((short)((int*)self)[buttonIndex + (page * 5 + 0xD020) * 2])
    {
    case 0:
        if (EQ_GLOBAL(char, 0x006B63F8) == 0)
        {
            EQ_METHOD(1, void, 0x004CA5C7, (void*, char*),
                g_everquest_guess)(EQ_ARRAY(char, 0x0058FBA4));
        }
        break;
    case 1:
        if (*(char*)((char*)self + 0xB5) == 0)
        {
            EQ_METHOD(0, void, 0x004CAF5E, (void*), g_everquest_guess)();
        }
        else
        {
            EQ_METHOD(0, void, 0x004CB362, (void*), g_everquest_guess)();
        }
        break;
    case 2:
        EQ_METHOD(0, void, 0x004CB4FA, (void*), g_everquest_guess)();
        break;
    case 3:
        EQ_METHOD(0, void, 0x004CB72D, (void*), g_everquest_guess)();
        break;
    case 4:
        EQ_METHOD(0, void, 0x004CB6E6, (void*), g_everquest_guess)();
        break;
    case 5:
        EQ_GLOBAL(u8, 0x006B6405) = EQ_GLOBAL(u8, 0x006B6405) ^ 1;
        break;
    case 6:
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int),
                g_display_guess)(0);
        }
        *(char*)((char*)self + 0xFE1) = (char)((int*)self)[0x3F8];
        ((tOverlayNoArgMethod_guess)(*(u32**)self)[2])(self);
        *(u8*)((char*)self + 0xFE0) = 0xC;
        ((tOverlayNoArgMethod_guess)(*(u32**)self)[1])(self);
        break;
    case 7:
        if (screenState >= 4)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*),
                g_everquest_guess)(
                    EQ_ARRAY(char, 0x0055B330));
            break;
        }
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int),
                g_display_guess)(0);
        }
        ((tOverlayNoArgMethod_guess)(*(u32**)self)[2])(self);
        *(u8*)((char*)self + 0x8FBAC) = 1;
        ((tOverlayNoArgMethod_guess)(*(u32**)self)[1])(self);
        break;
    case 8:
        EQ_METHOD(0, void, 0x0040F47F, (void*), g_display_guess)();
        break;
    case 9:
        if (*(char*)((char*)self + 0xFE0) == 5)
        {
            *(int*)((char*)self + 0x6C554) = 0;
            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int),
                EQ_GLOBAL(EQPlayer*, 0x006E4DA0))(100);
            if ((g_game_state_guess == 2) &&
                ((g_pcloudFlagA_guess == 0) ||
                 (EQ_GLOBAL(int, 0x006B63FC) == 0x280)))
            {
                EQ_GLOBAL(int, 0x00590170) = 2;
                g_game_state_guess = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int),
                    g_display_guess)(0);
            }
            ((tOverlayNoArgMethod_guess)(*(u32**)self)[2])(self);
            *(u16*)((char*)self + 0xFE8) = 0;
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0] == 0)
            {
                *(u8*)((char*)g_display_guess + 0x3C) = 0;
            }
            *(int*)((char*)self + 0xFE4) = 0;
            *(u8*)((char*)self + 0xFE2) = 0;
            *(short*)((char*)self + 0x1004) = -1;
            *(short*)((char*)self + 0x1006) = -1;
            *(u8*)((char*)self + 0xFE0) = 4;
            *(short*)((char*)self + 0x5F186) = -1;
            *(short*)((char*)self + 0x5F188) = -1;
            *(short*)((char*)self + 0x5F18A) = -1;
            *(int*)((char*)self + 0xEB8) = 1;
        }
        else
        {
            if ((g_game_state_guess == 2) &&
                ((g_pcloudFlagA_guess == 0) ||
                 (EQ_GLOBAL(int, 0x006B63FC) == 0x280)))
            {
                EQ_GLOBAL(int, 0x00590170) = 2;
                g_game_state_guess = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int),
                    g_display_guess)(0);
            }
            ((tOverlayNoArgMethod_guess)(*(u32**)self)[2])(self);
            slotValue = *(int*)((char*)self + 0xFE0);
            *(u8*)((char*)self + 0xFE0) = 4;
            *(char*)((char*)self + 0xFE2) = (char)slotValue;
            *(short*)((char*)self + 0x5F186) = -1;
            *(short*)((char*)self + 0x5F188) = -1;
            *(short*)((char*)self + 0x5F18A) = -1;
            *(int*)((char*)self + 0x8FBA8) = 1;
        }
        ((tOverlayNoArgMethod_guess)(*(u32**)self)[1])(self);
        break;
    case 10:
        *(char*)((char*)self + 0xFE2) = (char)((int*)self)[0x3F8];
        if (g_game_state_guess == 2)
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int),
                g_display_guess)(0);
            EQ_METHOD(0, void, 0x00465A60, (void*), self)();
        }
        else
        {
            *(int*)((char*)self + 0x6C554) =
                *(int*)((char*)g_display_guess + 0xBC);
        }
        break;
    case 11:
        EQ_GLOBAL(u8, 0x006B658C) = EQ_GLOBAL(u8, 0x006B658C) ^ 1;
        break;
    }
}

class EQ_OptionsOverlay_guess : public EQ_Overlay
{
public:
    EQ_OptionsOverlay_guess();
};

// FUNCTION: TRILOGY 0x00481320
// Initializes the options overlay and loads its screen artwork.
EQ_OptionsOverlay_guess::EQ_OptionsOverlay_guess()
{
    *(float*)((char*)this + 0x168) = 1.0f;
    *(u32*)this = 0x00535628;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x170) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    *(u32*)((char*)this + 0x0c) = 0;
    *(u32*)((char*)this + 0x164) = 0;
    *(u32*)((char*)this + 0x16c) = 1;
    *(u8*)((char*)this + 0x17d) = 0;
    *(u8*)((char*)this + 0x19b) = 0;
    *(u8*)((char*)this + 0x5d7) = 0;
    *(u8*)((char*)this + 0x174) = 0;
    *(u16*)((char*)this + 0x176) = 0;
    strcpy(
        (char*)this + 0x1b9, EQ_ARRAY(char, 0x0058FBA4));
    strcpy(
        (char*)this + 0x5b9, EQ_ARRAY(char, 0x0058FBA4));
    tSetRect_guess_t setRect = (tSetRect_guess_t)EQ_GLOBAL(void*, 0x00534248);
    *(u8*)((char*)this + 0x17c) = 0;
    *(u32*)((char*)this + 0x178) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x300);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x400, 0x300);
    strcpy(
        (char*)this + 0x34, EQ_ARRAY(char, 0x0053E950));
    strcpy(
        (char*)this + 0x52, EQ_ARRAY(char, 0x0053E028));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E1C) + 4));
    *(u8*)((char*)this + 0x174) |= 0xff;
    *(short*)((char*)this + 0xa4) = -1;
    *(u32*)((char*)this + 0x78) = 0;
    *(u32*)((char*)this + 0xa0) = 0;
}

class EQ_TextIO_InitializeTextIOOverlay_guess : public EQ_Overlay
{
public:
    EQ_TextIO_InitializeTextIOOverlay_guess();
};

// FUNCTION: TRILOGY 0x0048d188
// Initializes the text overlay and loads its text window artwork.
EQ_TextIO_InitializeTextIOOverlay_guess::EQ_TextIO_InitializeTextIOOverlay_guess()
{
    tSetRect_guess_t setRect;

    setRect = (tSetRect_guess_t)EQ_GLOBAL(void*, 0x00534248);
    *(u32*)this = 0x0053566C;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0x70) = 0;
    setRect((RECT*)((char*)this + 0x24), 0, 0, 0x400, 0x1e);
    setRect((RECT*)((char*)this + 0x14), 0, 0, 0x280, 0x1e);
    strcpy(
        (char*)this + 0x34, (const char*)0x0053E950);
    strcpy(
        (char*)this + 0x52, (const char*)0x0055D72C);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);
}

// FUNCTION: TRILOGY 0x0048d20f
// Runs the overlay base cleanup during destruction.
void __fastcall EQ_Overlay_derivedDestructor_0048D20F_guess(void* self)
{
    *(u32*)self = 0x0053566C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

/* Overlay control object at 0x006E4DD0; the slot at +0x58 writes an 8-byte result. */
class OverlayControlSlot58_guess
{
public:
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0C();
    virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1C();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2C();
    virtual void s30(); virtual void s34(); virtual void s38(); virtual void s3C();
    virtual void s40(); virtual void s44(); virtual void s48(); virtual void s4C();
    virtual void s50(); virtual void s54();
    virtual void ReadState_guess(void* result);                     /* +0x58 */
};

// FUNCTION: TRILOGY 0x004caf5e
// Invites the targeted player to the group when the party permits it.
void __fastcall Invite(void* self, int unusedEdx)
{
    EQPlayer* pEVar2;
    short memberIndex;
    char inviteText[120];
    char invitePacket[0x5b];

    if (EQ_GLOBAL(char, 0x005FE968) != '\0' &&
        strcmp(g_name_buffer_guess, EQ_GLOBAL(char*, 0x006E4DA8)) != 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CCF0));
        return;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CCB0));
        return;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CC84));
        return;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CC68));
        return;
    }
    if (EQ_GLOBAL(int, 0x00596330) == 4 &&
        EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))
            (EQ_GLOBAL(EQPlayer*, 0x006E4D8C), EQ_GLOBAL(EQPlayer*, 0x006E4DAC), 0) != 0)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056C3E8));
        return;
    }
    pEVar2 = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    for (memberIndex = 0; memberIndex < 5; memberIndex++)
    {
        if (EQ_ARRAY(char, 0x005FE8BD)[memberIndex * 0x1e] != '\0')
        {
            if (strcmp(pEVar2->name, EQ_ARRAY(char, 0x005FE8BD) + memberIndex * 0x1e) == 0)
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CC40));
                return;
            }
            pEVar2 = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
        }
    }
    for (memberIndex = 0; memberIndex < 5; memberIndex++)
    {
        if (EQ_ARRAY(char, 0x005FE8BD)[memberIndex * 0x1e] == '\0')
        {
            /* the original's signed index test (-1 < memberIndex); always true here */
            if (memberIndex >= 0)
            {
                strcpy(invitePacket, pEVar2->name);
                strcpy(invitePacket + 0x1e, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
                strcpy(invitePacket + 0x3c, EQ_ARRAY(char, 0x0058FBA4));
                *((u8*)(invitePacket + 0x5a)) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pvp;
                send_message_guess((void**)0x006B3194, 0x2040, invitePacket, 0x5b, 1);
                if (strcmp(invitePacket + 0x3c, EQ_ARRAY(char, 0x0058FBA4)) == 0)
                {
                    sprintf(inviteText, EQ_ARRAY(char, 0x0056CC1C),
                         EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
                }
                else
                {
                    sprintf(inviteText, EQ_ARRAY(char, 0x0056CC00),
                         EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name, invitePacket + 0x3c);
                }
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(inviteText);
                return;
            }
            break;
        }
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056CBB8));
}

/* Vtable slots called through the EQ_Main object (0x006E4E44) and the dialog object (0x006E4E20). */
class EQ_Main_vtable_guess
{
public:
    virtual void s00(); virtual void s04(); virtual void s08();
};

/* Stand-in for EQMoneyList (0x70 bytes): its constructor 0x004AD938 and destructor 0x004AD9B0 are the client's, only
 * declared here, so `new`/`delete` call them directly and get the original's EH frame. */
class EQMoneyList_guess
{
public:
    // EXTERNAL: TRILOGY 0x004AD938 EQMoneyList_guess::EQMoneyList_guess
    EQMoneyList_guess();
    // EXTERNAL: TRILOGY 0x004AD9B0 EQMoneyList_guess::~EQMoneyList_guess
    ~EQMoneyList_guess();
    char data[0x70];
};

#define OVL_AT(type, base, off)  (*(type*)((char*)(base) + (off)))
#define OVL_MOUSEUP(obj, mx, my) \
    EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), (obj))(0, (mx), (my))
#define OVL_SEND(op, data, size) \
    send_message_guess(EQ_ARRAY(void*, 0x006B3194), (op), (data), (size), 1)

// FUNCTION: TRILOGY 0x004cba05
// Left-click release: windows first; else trade cursor item/coins, drop them, or pick up ground item/coins
// @early-stop: frame size, call setup, and local/register allocation still differ across the handler.
// Left mouse button up (CEverQuest method, ECX = self, two stack args, RET 8): offers the click to each overlay
// window, then drops/combines the cursor item, trades with the clicked player, or picks up a ground item/money.
/* fake match: keep the existing label joins that reproduce the tested click-handler control flow. */
void __fastcall LMouseUp(void* self, int unusedEdx, int x, int y)
{
    EQPlayer* pPlayer;
    EQ_Item* item;
    int slot;
    int itemCount;
    int i;
    int off;
    char* ml;
    EQMoneyList_guess* mlObj;
    char* pDisplay;
    char* pCharacter;
    char* name;
    char type;
    int actor;
    short cursorRight;
    short cursorBottom;
    char* ia;
    int msgItem[2];
    int msgMoney[2];
    int msgTrade[2];
    int msgCoin[5];
    char buf[0x80];

    slot = -1;
    if (g_char_create_guess != 0)
    {
        pinstEQ_CharCreateS_guess->statStep = 1;
        OVL_AT(int, g_char_create_guess, 0x1BBC) = 0;
        pinstEQ_CharCreateS_guess->hoverOnly = 0;
        pinstEQ_CharCreateS_guess->hoverButton = -1;
        pinstEQ_CharCreateS_guess->alignmentDragging_guess = 0;
        if (OVL_MOUSEUP(g_char_create_guess, x, y) != 0)
        {
            return;
        }
    }
    if (g_char_select_guess != 0)
    {
        OVL_AT(int, g_char_select_guess, 0x88) = 0;
        OVL_AT(short, g_char_select_guess, 0x84) = -1;
        OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DF0), 0xA0) = 0;
        OVL_AT(short, EQ_GLOBAL(char*, 0x006E4DF0), 0xA4) = -1;
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4DF0), x, y) != 0)
        {
            return;
        }
        OVL_MOUSEUP(g_char_select_guess, x, y);
        return;
    }
    if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() == 0)
    {
        return;
    }
    if (g_eqmain_guess == 0)
    {
        return;
    }
    if (g_game_state_guess == 3)
    {
        if (EQ_GLOBAL(char, 0x006B645A) == 0)
        {
            return;
        }
        goto clearAndReturn;
    }
    pinstEQ_Requestor_guess->captureButtonResult_guess = 0;
    pinstEQ_Requestor_guess->buttonResult_guess = -1;
    pinstEQ_Main_guess->editLayout_guess = 0;
    pinstEQ_Main_guess->releasedButton_guess = pinstEQ_Main_guess->pressedButton_guess;
    pinstEQ_Main_guess->pressedButton_guess = -1;
    pinstEQ_Main_guess->clickedButton_guess = -1;
    pinstEQ_Main_guess->skillListThumbDrag_guess = 0;
    pinstEQ_Main_guess->trainSkillThumbDrag_guess = 0;
    pinstEQ_Main_guess->skillWindowThumbDrag_guess = 0;
    pinstEQ_Main_guess->trackThumbDrag_guess = 0;
    pinstEQ_Bag2_guess->mouseDown_guess = 0;
    pinstEQ_Bag1_guess->mouseDown_guess = 0;
    pinstEQ_Bag1_guess->pressedControl_guess = -1;
    pinstEQ_Bag2_guess->pressedControl_guess = -1;
    OVL_AT(int, g_eqBook_guess, 0x90) = 0;
    OVL_AT(short, g_eqBook_guess, 0x84) = -1;
    OVL_AT(int, EQ_GLOBAL(char*, 0x00604984), 0x1C8) = 0;
    OVL_AT(short, EQ_GLOBAL(char*, 0x00604984), 0x1CC) = -1;
    OVL_AT(int, EQ_GLOBAL(char*, 0x00604984), 0x1C4) = 0;
    OVL_AT(int, EQ_GLOBAL(char*, 0x006018C0), 0x1C8) = 0;
    OVL_AT(short, EQ_GLOBAL(char*, 0x006018C0), 0x1CC) = -1;
    OVL_AT(int, EQ_GLOBAL(char*, 0x006018C0), 0x1C4) = 0;
    pinstEQ_TextIO_guess->pressedControlId_guess = -1;
    pinstEQ_TextIO_guess->scrollThumbDragging_guess = 0;
    OVL_AT(short, g_chatWindow_guess, 0x1E6) = 0;
    OVL_AT(short, g_chatWindow_guess, 0x1E4) = -1;
    pinstEQ_TextIO_guess->mouseDispatchActive_guess = 0;
    if (EQ_GLOBAL(char, 0x006B645A) != 0)
    {
clearAndReturn:
        EQ_GLOBAL(char, 0x006B6408) = 0;
        return;
    }
    if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
    {
        return;
    }
    if (pinstEQ_Main_guess->screenMode_guess >= 8)
    {
        OVL_MOUSEUP(g_eqmain_guess, x, y);
        return;
    }
    if (g_game_state_guess == 1)
    {
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4D84), 0x10) != 0)
        {
            return;
        }
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E20), x, y) != 0)
        {
            return;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006018C0), 0x10) != 0)
        {
            OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006018C0), x, y);
            return;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x00604984), 0x10) != 0)
        {
            OVL_MOUSEUP(EQ_GLOBAL(char*, 0x00604984), x, y);
            return;
        }
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E5C), x, y) != 0)
        {
            return;
        }
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E58), x, y) != 0)
        {
            return;
        }
        if ((g_eqBook_guess != 0) && (OVL_AT(int, g_eqBook_guess, 0x10) != 0) &&
            (OVL_MOUSEUP(g_eqBook_guess, x, y) != 0))
        {
            return;
        }
        if ((EQ_GLOBAL(char*, 0x006E4DF4) != 0) && (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DF4), 0x10) != 0) &&
            (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4DF4), x, y) != 0))
        {
            return;
        }
        if ((EQ_GLOBAL(char*, 0x006E4DF8) != 0) && (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DF8), 0x10) != 0) &&
            (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4DF8), x, y) != 0))
        {
            return;
        }
        if ((pinstEQ_Main_guess->socialEditorOpen_guess == 0) &&
            (OVL_MOUSEUP(g_chatWindow_guess, x, y) != 0))
        {
            return;
        }
        if (OVL_MOUSEUP(g_eqmain_guess, x, y) != 0)
        {
            return;
        }
        g_cdisplay_guess->dragArmed_guess = 0;
        if (pinstEQ_Main_guess->screenMode_guess == 5)
        {
            return;
        }
    }
    else
    {
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E20), x, y) != 0)
        {
            return;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4D84), 0x10) != 0)
        {
            return;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006018C0), 0x10) != 0)
        {
            OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006018C0), x, y);
            return;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x00604984), 0x10) != 0)
        {
            OVL_MOUSEUP(EQ_GLOBAL(char*, 0x00604984), x, y);
            return;
        }
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E5C), x, y) != 0)
        {
            return;
        }
        if (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4E58), x, y) != 0)
        {
            return;
        }
        if ((EQ_GLOBAL(char*, 0x006E4DF4) != 0) && (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DF4), 0x10) != 0) &&
            (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4DF4), x, y) != 0))
        {
            return;
        }
        if ((EQ_GLOBAL(char*, 0x006E4DF8) != 0) && (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DF8), 0x10) != 0) &&
            (OVL_MOUSEUP(EQ_GLOBAL(char*, 0x006E4DF8), x, y) != 0))
        {
            return;
        }
        if ((pinstEQ_Main_guess->socialEditorOpen_guess == 0) &&
            (OVL_MOUSEUP(g_chatWindow_guess, x, y) != 0))
        {
            return;
        }
        if (OVL_MOUSEUP(g_eqmain_guess, x, y) != 0)
        {
            return;
        }
        g_cdisplay_guess->dragArmed_guess = 0;
    }
    pPlayer = EQ_METHOD(2, EQPlayer*, 0x004DD010, (void*, int, int), g_everquest_guess)(x, y);
    if ((pPlayer == 0) || ((pPlayer->type != 0) && (pPlayer->type != 1)))
    {
        goto noPlayerClicked;
    }
    item = g_pc_guess->inventory_items[0];
    if ((item == 0) && (OVL_AT(char, g_displayManager_guess, 0x3D) == 0))
    {
        goto fallbackMouseUp;
    }
    if (pinstEQ_Main_guess->tradeOpen_guess == 0)
    {
        goto startTrade;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D88) == 0)
    {
        return;
    }
    if (pPlayer != EQ_GLOBAL(EQPlayer*, 0x006E4D88))
    {
        return;
    }
    if (pinstEQ_Main_guess->tradeAccepted_guess != 0)
    {
        return;
    }
    type = EQ_GLOBAL(EQPlayer*, 0x006E4D88)->type;
    itemCount = type == 1 ? 4 : 8;
    if (item != 0)
    {
        if ((type != 1) &&
            (EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(item, 1) == 0))
        {
            return;
        }
        if (EQ_CDECL(int, 0x004A1D83, (EQ_Item*))(g_pc_guess->inventory_items[0]) != 0)
        {
            i = 0;
            if (0 < itemCount)
            {
                off = 0x6C50C;
                do
                {
                    if (g_pc_guess->inventory_items[0] != 0)
                    {
                        EQ_Item** slotPtr = (EQ_Item**)(off + EQ_GLOBAL(int, 0x006E4E44));
                        if ((*slotPtr != 0) &&
                            (g_pc_guess->inventory_items[0]->item_nr == (*slotPtr)->item_nr) &&
                            (EQ_CDECL(int, 0x004A1C7C, (void*, void*, int, int))
                                 (EQ_GLOBAL(char*, 0x006E4DA8) + 0xE0, slotPtr, 0, i + 3000) != 0))
                        {
                            slot = i;
                        }
                    }
                    off = off + 4;
                    i = i + 1;
                } while (i < itemCount);
            }
        }
        if (g_pc_guess->inventory_items[0] != 0)
        {
            i = 0;
            if (0 < itemCount)
            {
                off = 0x6C50C;
                do
                {
                    if ((g_pc_guess->inventory_items[0] != 0) &&
                        (OVL_AT(int, g_eqmain_guess, off) == 0))
                    {
                        EQ_METHOD(4, void, 0x00463651, (void*, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(0, i + 3000, 0, 1);
                        slot = i;
                    }
                    i = i + 1;
                    off = off + 4;
                } while (i < itemCount);
            }
        }
        if (g_pc_guess->inventory_items[0] == 0)
        {
            g_cdisplay_guess->itemOnCursor_guess = 0;
        }
        else
        {
            g_cdisplay_guess->itemOnCursor_guess = 1;
        }
        if ((short)slot == -1)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(EQ_ARRAY(char, 0x0056D154), 10);
            return;
        }
    }
    if (OVL_AT(char, g_displayManager_guess, 0x3D) != 0)
    {
        pinstEQ_Main_guess->tradeAccepted_guess = 0;
        pinstEQ_Main_guess->tradeOtherAccepted_guess = 0;
        msgCoin[1] = 3;
        msgCoin[0] = 0;
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9B8) > 0)
        {
            msgCoin[2] = 3;
            msgCoin[3] = 3;
            msgCoin[4] = OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9B8);
            OVL_SEND(0x212D, msgCoin, 0x14);
            pinstEQ_Main_guess->tradeCoinsPlatinum_guess += OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9B8);
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9B8) = 0;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9BC) > 0)
        {
            msgCoin[2] = 2;
            msgCoin[3] = 2;
            msgCoin[4] = OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9BC);
            OVL_SEND(0x212D, msgCoin, 0x14);
            pinstEQ_Main_guess->tradeCoinsGold_guess += OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9BC);
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9BC) = 0;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C0) > 0)
        {
            msgCoin[2] = 1;
            msgCoin[3] = 1;
            msgCoin[4] = OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C0);
            OVL_SEND(0x212D, msgCoin, 0x14);
            pinstEQ_Main_guess->tradeCoinsSilver_guess += OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C0);
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C0) = 0;
        }
        if (OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C4) > 0)
        {
            msgCoin[2] = 0;
            msgCoin[3] = 0;
            msgCoin[4] = OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C4);
            OVL_SEND(0x212D, msgCoin, 0x14);
            pinstEQ_Main_guess->tradeCoinsCopper_guess += OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C4);
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C4) = 0;
        }
        OVL_AT(char, g_displayManager_guess, 0x3D) = 0;
    }
fallbackMouseUp:
    EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(0, x, y);
    return;
startTrade:
    if (pinstEQ_Main_guess->screenMode_guess >= 5)
    {
        goto fallbackMouseUp;
    }
    if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
    {
        goto fallbackMouseUp;
    }
    if (OVL_AT(int, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info, 0x18C) != 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_GLOBAL(int, 0x006E4D88) != 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(pPlayer, EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 1.0f)
        > pPlayer->height_offset + EQ_GLOBAL(float, 0x00534498))
    {
        name = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(pPlayer->name);
        sprintf(buf, EQ_ARRAY(char, 0x0056D12C), name);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), g_everquest_guess)(buf);
        goto fallbackMouseUp;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id == pPlayer->spawn_id)
    {
        goto fallbackMouseUp;
    }
    if (g_eqmain_guess == 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->unk_0x68 != 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_GLOBAL(int, 0x006B6590) != 0)
    {
        goto fallbackMouseUp;
    }
    item = g_pc_guess->inventory_items[0];
    if (item != 0)
    {
        if (pPlayer->type == 1)
        {
            msgTrade[0] = pPlayer->spawn_id;
        }
        else
        {
            if (EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(item, 1) == 0)
            {
                return;
            }
            msgTrade[0] = pPlayer->spawn_id;
        }
        msgTrade[1] = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
        OVL_SEND(0x20D1, msgTrade, 8);
    }
    if (OVL_AT(char, g_displayManager_guess, 0x3D) != 0)
    {
        msgTrade[0] = pPlayer->spawn_id;
        msgTrade[1] = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
        OVL_SEND(0x20D1, msgTrade, 8);
    }
    goto fallbackMouseUp;
noPlayerClicked:
    if (g_eqmain_guess == 0)
    {
        goto skipGroundAction;
    }
    if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
    {
        goto skipGroundAction;
    }
    if (OVL_AT(int, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info, 0x18C) != 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_GLOBAL(int, 0x006E4D88) != 0)
    {
        goto fallbackMouseUp;
    }
    if (pinstEQ_Main_guess->tradeOpen_guess != 0)
    {
        goto fallbackMouseUp;
    }
    pCharacter = EQ_GLOBAL(char*, 0x006E4DA8);
    cursorBottom = EQ_GLOBAL(short, 0x006B63E6);
    cursorRight = EQ_GLOBAL(short, 0x006B63E4);
    item = OVL_AT(EQ_Item*, pCharacter, 0xE0);
    if ((item != 0) && ((short)x <= cursorRight) && ((short)y <= cursorBottom) &&
        ((short)x >= EQ_GLOBAL(short, 0x006B63E0)) && ((short)y >= EQ_GLOBAL(short, 0x006B63E2)))
    {
        if ((item->type == 0) &&
            ((OVL_AT(char, item, 0xC2) == 0x0C) || (OVL_AT(char, item, 0xC2) == 0x21) || (OVL_AT(char, item, 0xC2) == 0x27)))
        {
            goto skipDropItemType;
        }
        if (EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(item, 1) == 0)
        {
            return;
        }
        if (EQ_GLOBAL(int, 0x006B6EFC) == 1)
        {
            EQ_METHOD(1, void, 0x004CB7B8, (void*, int), g_everquest_guess)(1);
            return;
        }
        if (EQ_GLOBAL(int, 0x006B6EFC) == -1)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(EQ_ARRAY(char, 0x0056D0D0));
            return;
        }
        pinstEQ_Requestor_guess->requestAction_guess = 0x15;
        ((EQ_Main_vtable_guess*)EQ_GLOBAL(void*, 0x006E4E20))->s04();
        pinstEQ_Requestor_guess->timeout_guess = OVL_AT(int, g_displayManager_guess, 0xBC) + 20000;
        sprintf(EQ_GLOBAL(char*, 0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0056D098),
             g_pc_guess->inventory_items[0]);
        return;
    }
    pDisplay = g_displayManager_guess;
    type = OVL_AT(char, pDisplay, 0x3D);
    if (type != 0)
    {
        if (((short)x <= cursorRight) && ((short)y <= cursorBottom) &&
            ((short)x >= EQ_GLOBAL(short, 0x006B63E0)) && ((short)y >= EQ_GLOBAL(short, 0x006B63E2)))
        {
            mlObj = new EQMoneyList_guess;
            ml = (char*)mlObj;
            OVL_AT(float, ml, 0x38) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y;
            OVL_AT(float, ml, 0x3C) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x;
            OVL_AT(float, ml, 0x40) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z;
            OVL_AT(int, ml, 0x8) = 0;
            OVL_AT(int, ml, 0xC) = 0;
            OVL_AT(int, ml, 0x10) = 0;
            OVL_AT(int, ml, 0x14) = 0;
            pDisplay = g_displayManager_guess;
            type = OVL_AT(char, pDisplay, 0x3D);
            pCharacter = EQ_GLOBAL(char*, 0x006E4DA8);
            if ((type == 1) && (OVL_AT(int, pCharacter, 0x9B8) > 0))
            {
                OVL_AT(int, ml, 0x8) = OVL_AT(int, pCharacter, 0x9B8);
            }
            else if ((type == 2) && (OVL_AT(int, pCharacter, 0x9BC) > 0))
            {
                OVL_AT(int, ml, 0xC) = OVL_AT(int, pCharacter, 0x9BC);
            }
            else if ((type == 3) && (OVL_AT(int, pCharacter, 0x9C0) > 0))
            {
                OVL_AT(int, ml, 0x10) = OVL_AT(int, pCharacter, 0x9C0);
            }
            else if ((type == 4) && (OVL_AT(int, pCharacter, 0x9C4) > 0))
            {
                OVL_AT(int, ml, 0x14) = OVL_AT(int, pCharacter, 0x9C4);
            }
            OVL_AT(int, ml, 0x30) = OVL_AT(int, ml, 0x8) + OVL_AT(int, ml, 0xC) + OVL_AT(int, ml, 0x10) + OVL_AT(int, ml, 0x14);
            if (OVL_AT(int, ml, 0x8) <= 0)
            {
                if (OVL_AT(int, ml, 0xC) > 0)
                {
                    name = EQ_ARRAY(char, 0x0056D088);
                }
                else if (OVL_AT(int, ml, 0x10) > 0)
                {
                    name = EQ_ARRAY(char, 0x0056D078);
                }
                else if (OVL_AT(int, ml, 0x14) > 0)
                {
                    name = EQ_ARRAY(char, 0x0056D068);
                }
                else
                {
                    name = EQ_ARRAY(char, 0x0056D058);
                }
            }
            else
            {
                name = EQ_ARRAY(char, 0x0056D058);
            }
            strcpy(ml + 0x50, name);
            OVL_AT(int, ml, 0x34) = EQ_METHOD(9, int, 0x004112DF, (void*, char*, float, float, float, float, float, float, int, int), g_display_guess)(ml + 0x50, OVL_AT(float, ml, 0x38), OVL_AT(float, ml, 0x3C),
                 OVL_AT(float, ml, 0x40), OVL_AT(float, ml, 0x44), OVL_AT(float, ml, 0x48), 0.0f, 1, 1);
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9B8) = 0;
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9BC) = 0;
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C0) = 0;
            OVL_AT(int, EQ_GLOBAL(char*, 0x006E4DA8), 0x9C4) = 0;
            if ((EQ_GLOBAL(char, 0x006B63F8) == 0) && (DAT_006b6494 == 0))
            {
                EQTRACE_guess(EQ_ARRAY(char, 0x0056D034), OVL_AT(int, ml, 0x8));
                OVL_SEND(0x2007, ml, 0x70);
            }
            if (ml != 0)
            {
                delete mlObj;
            }
            OVL_AT(char, g_displayManager_guess, 0x3D) = 0;
            return;
        }
        if (type != 0)
        {
            goto fallbackMouseUp;
        }
    }
    if (item != 0)
    {
        goto fallbackMouseUp;
    }
    actor = EQ_METHOD(3, int, 0x0041125D, (void*, int, int, int), g_display_guess)((int)(short)x, (int)(short)y, 0);
    ia = EQ_METHOD(1, char*, 0x004A0413, (void*, int), EQ_GLOBAL(void*, 0x006E4D5C))(actor);
    if ((ia != 0) &&
        (EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
              EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z, OVL_AT(float, ia, 0x28), OVL_AT(float, ia, 0x2C),
              OVL_AT(float, ia, 0x30), 1.0f) < EQ_GLOBAL(float, 0x00534498)))
    {
        msgItem[0] = OVL_AT(int, ia, 0xC);
        msgItem[1] = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
        EQTRACE_guess(EQ_ARRAY(char, 0x0056CFFC), (int)OVL_AT(short, ia, 0x8), OVL_AT(int, ia, 0xC));
        pinstEQ_Main_guess->pendingWorldRequests_guess = pinstEQ_Main_guess->pendingWorldRequests_guess + 1;
        pinstEQ_Main_guess->pendingServerReplies_guess = pinstEQ_Main_guess->pendingServerReplies_guess + 1;
        if ((EQ_GLOBAL(char, 0x006B63F8) == 0) && (DAT_006b6494 == 0))
        {
            OVL_SEND(0x2036, msgItem, 8);
        }
        if ((g_game_state_guess == 2) && ((g_pcloudFlagA_guess == 0) || (EQ_GLOBAL(int, 0x006B63FC) == 0x280)))
        {
            EQ_GLOBAL(int, 0x00590170) = 2;
            g_game_state_guess = 1;
            if (g_eqmain_guess != 0)
            {
                pinstEQ_Main_guess->uiDirty_guess = 1;
            }
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
        ((EQ_Main_vtable_guess*)EQ_GLOBAL(void*, 0x006E4E44))->s08();
        pinstEQ_Main_guess->screenMode_guess = 4;
        ((EQ_Main_vtable_guess*)EQ_GLOBAL(void*, 0x006E4E44))->s04();
        return;
    }
    ia = EQ_METHOD(1, char*, 0x004ADA2D, (void*, int), EQ_GLOBAL(void*, 0x006E4D60))(actor);
    if (ia == 0)
    {
        goto fallbackMouseUp;
    }
    if (EQ_METHOD(7, float, 0x004129F3, (void*, float, float, float, float, float, float, float), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
             EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z, OVL_AT(float, ia, 0x38), OVL_AT(float, ia, 0x3C),
             OVL_AT(float, ia, 0x40), 1.0f) >= EQ_GLOBAL(float, 0x00534498))
    {
        goto fallbackMouseUp;
    }
    msgMoney[0] = OVL_AT(int, ia, 0x18);
    msgMoney[1] = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
    pinstEQ_Main_guess->pendingWorldRequests_guess = pinstEQ_Main_guess->pendingWorldRequests_guess + 1;
    if ((EQ_GLOBAL(char, 0x006B63F8) == 0) && (DAT_006b6494 == 0))
    {
        OVL_SEND(0x2008, msgMoney, 8);
    }
    return;
skipGroundAction:
    EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(0, x, y);
    return;
skipDropItemType:
    EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(0, x, y);
}

#undef OVL_SEND
#undef OVL_MOUSEUP
#undef OVL_AT

// FUNCTION: TRILOGY 0x004dcbad
// @early-stop: VC6 puts param_2 in EDI where the original has screenX, so call push order and tail merges differ; the == 2 and else arms merge (original keeps two copies).
// The original never reads ECX (it loads its own objects), so `self` is unused. Two stack args, RET 8.
void __fastcall EQ_Overlay_MouseUpDispatch_guess(void* self, int unusedEdx, int screenX, int param_2)
{
    if (EQ_GLOBAL(int, 0x006E4E6C) != 0)
    {
        return;
    }
    if (EQ_GLOBAL(int, 0x006E4E70) != 0)
    {
        return;
    }
    if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() == 0)
    {
        return;
    }
    EQ_GLOBAL(char, 0x006B6408) = 0;
    if (g_game_state_guess == 3)
    {
        if (EQ_GLOBAL(char, 0x006B645A) != 0)
        {
            EQ_GLOBAL(char, 0x006B645A) = 0;
        }
        EQ_CDECL(void, 0x004EB687, (void))();
        return;
    }
    if (g_eqmain_guess == 0)
    {
        return;
    }
    pinstEQ_TextIO_guess->mouseDispatchActive_guess = 0;
    pinstEQ_TextIO_guess->pressedControlId_guess = -1;
    *(short*)((char*)g_chatWindow_guess + 0x1E4) = -1;
    *(short*)((char*)g_chatWindow_guess + 0x1E6) = 0;
    pinstEQ_Bag1_guess->mouseDown_guess = 0;
    pinstEQ_Bag2_guess->mouseDown_guess = 0;
    pinstEQ_Main_guess->editLayout_guess = 0;
    pinstEQ_Main_guess->releasedButton_guess = pinstEQ_Main_guess->pressedButton_guess;
    pinstEQ_Main_guess->pressedButton_guess = -1;
    pinstEQ_Main_guess->clickedButton_guess = -1;
    g_cdisplay_guess->dragArmed_guess = 0;
    if (EQ_GLOBAL(char, 0x006B645A) != 0)
    {
        EQ_GLOBAL(char, 0x006B645A) = 0;
        if ((EQ_GLOBAL(char*, 0x006E4D84) != 0) &&
            ((*(short*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x94) = -1), EQ_GLOBAL(char*, 0x006E4D84) != 0) &&
            (*(int*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x10) != 0))
        {
            *(int*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x9C) = 0;
            ((void (__fastcall *)(void*))(*(u32*)(*(u32*)EQ_GLOBAL(char*, 0x006E4D84) + 0x08)))(EQ_GLOBAL(char*, 0x006E4D84));
            pinstEQ_TextIO_guess->redrawPass_guess = 1;
        }
        if (500 < (u32)(*(int*)((char*)g_displayManager_guess + 0xBC) - EQ_GLOBAL(int, 0x006B64A0)))
        {
            EQ_CDECL(void, 0x004EB687, (void))();
            return;
        }
        if (pinstEQ_Main_guess->screenMode_guess >= 5)
        {
            return;
        }
        EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(1, screenX, param_2);
    }
    else
    {
        if ((EQ_GLOBAL(char*, 0x006E4D84) != 0) &&
            ((*(short*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x94) = -1), EQ_GLOBAL(char*, 0x006E4D84) != 0) &&
            (*(int*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x10) != 0))
        {
            *(int*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x9C) = 0;
            ((void (__fastcall *)(void*))(*(u32*)(*(u32*)EQ_GLOBAL(char*, 0x006E4D84) + 0x08)))(EQ_GLOBAL(char*, 0x006E4D84));
            pinstEQ_TextIO_guess->redrawPass_guess = 1;
            return;
        }
        if (*(int*)(EQ_GLOBAL(char*, 0x006018C0) + 0x10) != 0)
        {
            return;
        }
        if (*(int*)(EQ_GLOBAL(char*, 0x00604984) + 0x10) != 0)
        {
            return;
        }
        if (pinstEQ_Main_guess->screenMode_guess >= 8)
        {
            EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(1, screenX, param_2);
            return;
        }
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0 &&
            EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E5C))(1, screenX, param_2) != 0)
        {
            return;
        }
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0 &&
            EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E58))(1, screenX, param_2) != 0)
        {
            return;
        }
        // Both arms make the same calls in the same order; the original compiled them as two copies.
        if (g_game_state_guess == 2)
        {
            if (*(int*)((char*)g_chatWindow_guess + 0x10) != 0 &&
                EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(1, screenX, param_2) != 0)
            {
                return;
            }
            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E44))() != 0 &&
                EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(1, screenX, param_2) != 0)
            {
                return;
            }
            if (pinstEQ_Main_guess->screenMode_guess >= 5)
            {
                return;
            }
            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DEC))() == 0)
            {
                return;
            }
            EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(1, screenX, param_2);
            return;
        }
        else
        {
            if (*(int*)((char*)g_chatWindow_guess + 0x10) != 0 &&
                EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(1, screenX, param_2) != 0)
            {
                return;
            }
            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E44))() != 0 &&
                EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(1, screenX, param_2) != 0)
            {
                return;
            }
            if (pinstEQ_Main_guess->screenMode_guess >= 5)
            {
                return;
            }
            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DEC))() == 0)
            {
                return;
            }
            EQ_METHOD(3, void, 0x00415EFB, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(1, screenX, param_2);
        }
    }
}

// FUNCTION: TRILOGY 0x004e3297
// Updates playback volume and sound capacity from the current settings.
void __fastcall ApplySoundSettings_guess(void* self, int unusedEdx)
{
    if ((EQ_GLOBAL(void*, 0x006E4DD0) != 0) && (EQ_GLOBAL(void*, 0x006ECDF4) != 0))
    {
        EQ_GLOBAL(int, 0x00579314) = EQ_GLOBAL(int, 0x006B6890) * 5;
        EQ_THISCALL(void, 0x004E36EE, (void*, int, int))
            (g_sound_manager_ptr_guess, unusedEdx, EQ_GLOBAL(int, 0x00579314));
        if ((EQ_GLOBAL(int, 0x006B688C) != 0) ||
            ((EQ_GLOBAL(int, 0x006ECE28) != 0) || (EQ_GLOBAL(int, 0x00579318) != 0)))
        {
            EQ_GLOBAL(int, 0x00579318) = (int)(EQ_GLOBAL(int, 0x006B688C) * EQ_GLOBAL(double, 0x005357B0));
            EQ_METHOD(2, int, 0x004E5EE6, (void*, int, int), EQ_GLOBAL(void*, 0x006ECDF4))(0, EQ_GLOBAL(int, 0x00579318));
        }
        if (EQ_GLOBAL(int, 0x006B6894) < 1)
        {
            EQ_GLOBAL(int, 0x006B6894) = 1;
        }
        EQ_GLOBAL(int, 0x0057931C) = EQ_GLOBAL(int, 0x006B6894);
        EQ_METHOD(2, void, 0x004E3354, (void*, int, int), self)(EQ_GLOBAL(int, 0x006B6894), 0);
        {
            u8 result[8];
            EQ_GLOBAL(OverlayControlSlot58_guess*, 0x006E4DD0)->ReadState_guess(result);
            *(u8*)self = result[5];
        }
    }
}

// FUNCTION: TRILOGY 0x004e3346
// Changes the sound playback limit with status reporting enabled.
void __fastcall SetSoundBufferCountWithStatus_guess(void* self, int unusedEdx, int bufferCount)
{
    EQ_METHOD(2, void, 0x004E3354, (void*, int, int), self)(bufferCount, 1);
}

// FUNCTION: TRILOGY 0x004e34f5
// Thunk that forwards to the audio settings routine.
void __fastcall thunk_FUN_004e3297(void* ptr)
{
    EQ_METHOD(0, void, 0x004E3297, (void*), ptr)();
}

/* The sound control object (0x006E4DD0) with the unnamed slots 0x00..0x4C as placeholders; only +0x50 is called here. */
class SoundControlSlot50_guess
{
public:
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0C();
    virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1C();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2C();
    virtual void s30(); virtual void s34(); virtual void s38(); virtual void s3C();
    virtual void s40(); virtual void s44(); virtual void s48(); virtual void s4C();
    virtual void SetVolume_guess(float volume);                      /* +0x50 */
};

// FUNCTION: TRILOGY 0x004E36EE
// __thiscall that ignores `this`. Forwards int * 0.01f to slot 0x50 of the sound control object.
void __fastcall SetSoundVolumePercent_guess(void* self, int unusedEdx, int percent)
{
    EQ_GLOBAL(SoundControlSlot50_guess*, 0x006E4DD0)->SetVolume_guess((float)percent * 0.01f);
}

/* MSS32.DLL import slot (IAT), as in soundmanager.h. */
typedef void (__stdcall *AIL_set_XMIDI_master_volume_t)(void* sequence, long volume);
#define AIL_set_XMIDI_master_volume  EQ_GLOBAL(AIL_set_XMIDI_master_volume_t, 0x00534348)

// FUNCTION: TRILOGY 0x004E5EE6
// Sets the active music sequence's master volume.
// __thiscall; the Miles sequence is at this+0x60, the stored volume at this+0x8. The first argument is unused.
// Returns 1 when a sequence exists (and the volume was set), else 0.
int __fastcall SetXMIDIMasterVolume_guess(void* self, int unusedEdx, int unused, int volume)
{
    void* sequence = *(void**)((char*)self + 0x60);

    if (sequence != 0)
    {
        *(int*)((char*)self + 8) = volume;
        AIL_set_XMIDI_master_volume(sequence, volume);
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004EB687
// Updates the center coordinates for the active screen area.
// Centre of the (clip) rectangle: half of the width/height pair at 0x006B63FC/0x006B6400, or of the sums of the
// shorts 0x006B63E0..E6 when 0x0058FC6C == 1. Stored as ints at 0x006EFA00/04 and 0x006EFC80/84 and as shorts at
// 0x006B6414/16.
void UpdateScreenCenter_guess()
{
    int x;
    int y;

    if (g_game_state_guess == 1)
    {
        x = (EQ_GLOBAL(short, 0x006B63E0) + EQ_GLOBAL(short, 0x006B63E4)) / 2;
        EQ_GLOBAL(int, 0x006EFA00) = x;
        EQ_GLOBAL(int, 0x006EFC80) = x;
        y = EQ_GLOBAL(short, 0x006B63E2) + EQ_GLOBAL(short, 0x006B63E6);
    }
    else
    {
        x = EQ_GLOBAL(int, 0x006B63FC) / 2;
        y = EQ_GLOBAL(int, 0x006B6400);
        EQ_GLOBAL(int, 0x006EFA00) = x;
        EQ_GLOBAL(int, 0x006EFC80) = x;
    }
    y = y / 2;
    EQ_GLOBAL(short, 0x006B6414) = (short)x;
    EQ_GLOBAL(int, 0x006EFA04) = y;
    EQ_GLOBAL(int, 0x006EFC84) = y;
    EQ_GLOBAL(short, 0x006B6416) = (short)y;
}

class FUN_004ee050_guess
{
public:
    bool Process_guess(char* windowName, int unused_arg);
};

// FUNCTION: TRILOGY 0x004ee050
// Processes a named window entry and refreshes active records; its exact purpose is unclear.
// @early-stop: indirect calls still add EDX/address setup; strcpy cleanup and address evaluation order differ.
/* fake match: pass the live index in ignored EDX slots so VC6 can keep it in EBX. */
bool FUN_004ee050_guess::Process_guess(char* windowName, int unused_arg)
{
    int resultCode;
    int windowIndex;
    int* windowSlot;
    char nameBuffer[260];
    int queryOutput = false;

    strcpy(nameBuffer, windowName);
    if (((int*)this)[0x2d4] == 0)
    {
        return false;
    }
    resultCode = ((int (__fastcall *)(int*, int*, int*, int*, int))(*(u32*)(*(u32*)((int*)this)[0x2d4] + 0x0c)))((int*)*(u32*)((int*)this)[0x2d4], &queryOutput, (int*)((int*)this)[0x2d4], &queryOutput, 0);
    if (resultCode != 0)
    {
        ((int (__fastcall *)(int*, int*, int*, int))(*(u32*)(*(u32*)((int*)this)[0x2d4] + 0x14)))((int*)*(u32*)((int*)this)[0x2d4], &queryOutput, (int*)((int*)this)[0x2d4], 0);
    }
    resultCode = ((int (__fastcall *)(int*, char*, int*, char*, int))(*(u32*)(*(u32*)((int*)this)[0x2d4] + 0x10)))((int*)*(u32*)((int*)this)[0x2d4], nameBuffer, (int*)((int*)this)[0x2d4], nameBuffer, 0);
    if (resultCode == 0)
    {
        ((int (__fastcall *)(int*, int, const char*))(*(u32*)(*(u32*)(int*)this + 0x4c)))((int*)this, 0, (const char*)0x00579398);
    }
    if (resultCode != -0x7ffbfffd)
    {
        if (resultCode != -0x7ffbfffc)
        {
            if (resultCode != -0x7ffbfffb)
            {
                if (resultCode == 0)
                {
                    windowSlot = (int*)this + 3;
                    if (((int*)this)[3] != 0)
                    {
                        windowIndex = 0;
                        do
                        {
                            if (*(int*)(*windowSlot + 4) != 0)
                            {
                                EQ_THISCALL(int, 0x004EE419, (int*, int, int, int))((int*)this, windowIndex, *(int*)(*windowSlot + 0x54), windowIndex);
                                EQ_THISCALL(void, 0x0040101D, (int*, int))((int*)*windowSlot, windowIndex);
                            }
                            windowIndex = windowIndex + 1;
                            windowSlot = windowSlot + 1;
                        } while (windowIndex < 0x2d0);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

class FUN_004ee11b_guess
{
public:
    int LookupAndSave_guess(u32 param_2);
};

// FUNCTION: TRILOGY 0x004ee11b
// Looks up the supplied key and conditionally saves its record; its in-game purpose is unclear.
// Flag at 0xbc8 clear: returns 1. Table at 0xb50 null: returns 0. Otherwise looks the name up, fetches
// the 0x38-byte record, and saves it to the store at 0xbc4 when the store's query sets bit 2.
// Returns -1 if the lookup left the index at -1, else 1.
// @early-stop: query key load and record address are scheduled before their preceding pushes; return mask uses AL.
int FUN_004ee11b_guess::LookupAndSave_guess(u32 param_2)
{
    int* store;
    int* table;
    u32* vtable;
    char buffer[260];
    char record[56];
    int index;

    if (((char*)this)[0xbc8] == 0)
    {
        return 1;
    }
    else if (((int*)this)[0x2d4] != 0)
    {
        index = -1;
        strcpy(buffer, (char*)param_2);
        table = (int*)((int*)this)[0x2d4];
        vtable = *(u32* volatile*)table;
        ((int (__stdcall *)(int*, char*, int*))(*(u32*)((char*)vtable + 0x2c)))(table, buffer, &index);
        table = (int*)((int*)this)[0x2d4];
        vtable = *(u32* volatile*)table;
        ((int (__stdcall *)(int*, int, char*))(vtable[0x30 / 4]))(table, index, record);
        store = (int*)((int*)this)[0x2f1];
        if (store != (int*)0)
        {
            vtable = *(u32* volatile*)store;
            ((int (__stdcall *)(int*, const char*, int, u32*))(vtable[0x14 / 4]))(store, (const char*)0x005359B0, 1, &param_2);
            if ((param_2 & 2) != 0)
            {
                table = (int*)((int*)this)[0x2f1];
                vtable = *(u32* volatile*)table;
                ((int (__stdcall *)(int*, const char*, u32, int, int, char*, int))(vtable[0x10 / 4]))(table, (const char*)0x005359B0, 0x80000001, 0, 0, record, 0x38);
            }
        }
        return index == -1 ? -1 : 1;
    }
    return 0;
}

class FUN_004ee2fd_guess
{
public:
    int IsEntryActive_guess(int index);
};

// FUNCTION: TRILOGY 0x004ee2fd
// Checks whether an indexed slot is active; the table's game role is unclear.
// __thiscall, one stack argument. The table at +0xc holds 0x2d0 pointers. Returns 1 when entry `index`
// (0..0x2cf) is non-null and the dword at its +4 is nonzero, else 0.
int FUN_004ee2fd_guess::IsEntryActive_guess(int index)
{
    int* entry;
    bool active;

    if (index >= 0 && index < 0x2d0)
    {
        entry = *(int**)((char*)this + 0xc + index * 4);
        if (entry != 0)
        {
            active = entry[1] != 0;
            return active;
        }
    }
    return 0;
}
