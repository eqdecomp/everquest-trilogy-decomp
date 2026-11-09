// EQ_Guide.cpp - old guide window behavior.

#include "trilogy/common.h"
#include "trilogy/eq_guide.h"
#include "trilogy/eqaddr.h"

/* fake match: stands in for the EQ_Overlay base sub-object (same shape as EQ_Main14OverlayBase_guess).
   The ctor keeps `self` in a local slot and runs the base ctor; that gives VC6's EH frame and the `this`
   slot the original has. The dtor must stay empty: the base dtor must not run on a normal return. */
class EQ_GuideEHLocal_guess
{
public:
    void* overlay;

    EQ_GuideEHLocal_guess(void* self) : overlay(self)
    {
        EQ_METHOD(0, void, 0x00450049, (void*), overlay)();
    }

    ~EQ_GuideEHLocal_guess() {}
};

// FUNCTION: TRILOGY 0x0043629a
// Initializes the guide UI overlay and clears its display state.
// @early-stop: `push 1` must precede `mov ebp,[SetRect]`; second SetRect `lea` is hoisted; EQ_THISCALL dummy EDX `xor edx,edx` before load_guess
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
u32* EQ_Guide::construct_guess(void)
{
    void* self = (void*)this;
    typedef int (__stdcall *SetRect_guess_t)(void*, int, int, int, int);
    EQ_GuideEHLocal_guess ehLocal(self);
    SetRect_guess_t SetRect = EQ_GLOBAL(SetRect_guess_t, 0x00534248);
    u32* obj = (u32*)self;
    u32* ptr;
    int remaining;

    obj[0x22] |= 0xffffffff;
    obj[0] = 0x00535464;
    obj[4] = 0;
    obj[0x24] = 1;
    obj[0x1c] = 0;
    obj[3] = 0;
    SetRect(obj + 9, 0, 0, 0x280, 0x1e0);
    SetRect(obj + 5, 0, 0, 0x280, 0x1e0);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)(obj + 0xd), (const char*)0x0053e950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)obj + 0x52, (const char*)0x00558a10);
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), self)(1, 0);
    obj[0x1e] = 0;
    obj[0x21] = 0;
    obj[0x27] = 0;
    obj[0x25] = 0;
    obj[0x23] = 0;
    obj[0x28] = 0;
    obj[0x29] = 0;
    obj[0x2a] = 0;
    obj[0x2b] = 0;
    obj[0x2c] = 0;
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)(obj + 0x2d), (const char*)0x0058fba4);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)obj + 0xd2, (const char*)0x0058fba4);
    obj[0x3c] = 1;
    obj[0x3d] = 1;
    *(u8*)((char*)obj + 0x116) = 0;
    obj[0x20] = 0;
    ptr = obj + 0x37a;
    remaining = 0xc;
    do
    {
        *(u8*)(ptr - 8) = 0;
        *(u8*)ptr = 0;
        *(u8*)(ptr + 8) = 0;
        *(u8*)(ptr + 0x10) = 0;
        ptr[0x18] = 0;
        ptr[0x19] = 0;
        *(u8*)(ptr + 0x1a) = 0;
        ptr = ptr + 0x62;
        remaining = remaining + -1;
    } while (remaining != 0);
    obj[0x370] |= 0xffffffff;
    obj[0x371] |= 0xffffffff;
    obj[0x36f] = 0;
    return obj;
}

// FUNCTION: TRILOGY 0x004363EA
// Releases the guide window's buttons and display overlay.
void __fastcall EQ_Guide_derivedDestructor_004363EA_guess(void* self)
{
    *(u32*)self = 0x00535464;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

typedef int (__stdcall *EQ_GuideSetRectFn_guess)(void*, int, int, int, int);

// FUNCTION: TRILOGY 0x004363f5
// Places the guide window's button hit areas for its current page (petition list, petition, prompt, records).
void __fastcall PlaceGuideButtons_guess(void* self, int unusedEdx)
{
    register EQ_GuideSetRectFn_guess setRect;
    int buttonCount;
    int buttonIndex;
    int rowIndex;
    int nextRow;
    int right;
    int left;
    int slotIndex;
    char* rect;
    char* slot;

    setRect = EQ_GLOBAL(EQ_GuideSetRectFn_guess, 0x00534248);
    *(u32*)((char*)self + 0x10) = 1;
    if (*(int*)((char*)self + 0x8c) == 0)
    {
        setRect((char*)self + 0x2028, 0x77, 0, 0xb3, 0x1e);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (0, (char*)self + 0x2028, -1, -1, 0, 0);
    }
    if (*(int*)((char*)self + 0x8c) == 1)
    {
        if (*(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba4) > 0x19)
        {
            *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba4) = 0x19;
        }
        buttonCount = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba4);
        if (buttonCount < 0xb)
        {
            right = 0x207;
        }
        else
        {
            right = ((buttonCount > 0x14) - 1 & 0x65) + 0x13d;
        }
        rowIndex = 0;
        buttonIndex = 0;
        if (buttonCount > 0)
        {
            left = right - 100;
            rect = (char*)self + 0x2028;
            do
            {
                if (rowIndex > 9)
                {
                    right = right + 0x65;
                    rowIndex = 0;
                    left = left + 0x65;
                }
                nextRow = rowIndex + 1;
                setRect(rect, left, rowIndex << 5, right, nextRow * 0x20);
                EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
                    (buttonIndex, rect, -1, -1, 0, 0);
                rowIndex = nextRow;
                buttonIndex = buttonIndex + 1;
                rect = rect + 0x10;
            } while (buttonIndex < *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba4));
        }
        setRect((char*)self + 0x2658, 0x77, 0x122, 0xb3, 0x140);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (99, (char*)self + 0x2658, -1, -1, 0, 0);
    }
    if (*(int*)((char*)self + 0x8c) == 2)
    {
        rect = (char*)self + 0x2028;
        buttonIndex = 0;
        left = 0;
        do
        {
            setRect(rect, 0x77, left, 0xdb, left + 0x19);
            EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
                (buttonIndex, rect, -1, -1, 0, 0);
            left = left + 0x19;
            buttonIndex = buttonIndex + 1;
            rect = rect + 0x10;
        } while (left < 300);
        setRect((char*)self + 0x2648, 0x13b, 0x7d, 0x19f, 0x96);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (0x62, (char*)self + 0x2648, -1, -1, 0, 0);
    }
    if (*(int*)((char*)self + 0x8c) == 3)
    {
        setRect((char*)self + 0x2028, 0x8b, 100, 199, 0x82);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (0, (char*)self + 0x2028, -1, -1, 0, 0);
        setRect((char*)self + 0x2038, 0x13f, 100, 0x17b, 0x82);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (1, (char*)self + 0x2038, -1, -1, 0, 0);
    }
    if (*(int*)((char*)self + 0x8c) == 4)
    {
        setRect((char*)self + 0x2028, 0x77, 0, 0xb3, 0x1e);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (0, (char*)self + 0x2028, -1, -1, 0, 0);
        setRect((char*)self + 0x2038, 0x77, 0x1e, 0xb3, 0x3c);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (1, (char*)self + 0x2038, -1, -1, 0, 0);
        setRect((char*)self + 0x2048, 0x77, 0x3c, 0xb3, 0x5a);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (2, (char*)self + 0x2048, -1, -1, 0, 0);
        setRect((char*)self + 0x2058, 0x77, 0x5a, 0xb3, 0x78);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (3, (char*)self + 0x2058, -1, -1, 0, 0);
        setRect((char*)self + 0x2068, 0x77, 0x78, 0xb3, 0x96);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (4, (char*)self + 0x2068, -1, -1, 0, 0);
        setRect((char*)self + 0x2078, 0x77, 0x96, 0xb3, 0xb4);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (5, (char*)self + 0x2078, -1, -1, 0, 0);
        setRect((char*)self + 0x2088, 0x77, 0xb4, 0xb3, 0xd2);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (6, (char*)self + 0x2088, -1, -1, 0, 0);
        *(int*)((char*)self + 0xdc0) = -1;
        if (*(int*)((char*)self + 0xdbc) > 0)
        {
            slotIndex = *(int*)((char*)self + 0xdbc) - 1;
            if (slotIndex >= 0)
            {
                slot = (char*)self + slotIndex * 0x188 + 0xe4c;
                do
                {
                    if (*(int*)slot != 0)
                    {
                        setRect((char*)self + 0x2098, 0x77, 0x10e, 0xb3, 300);
                        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
                            (7, (char*)self + 0x2098, -1, -1, 0, 0);
                        *(int*)((char*)self + 0xdc0) = slotIndex;
                        break;
                    }
                    slotIndex = slotIndex - 1;
                    slot = slot - 0x188;
                } while (slotIndex >= 0);
            }
        }
        setRect((char*)self + 0x20a8, 0x121, 0x10e, 0x15d, 300);
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
            (8, (char*)self + 0x20a8, -1, -1, 0, 0);
        *(int*)((char*)self + 0xdc4) = -1;
        slotIndex = *(int*)((char*)self + 0xdbc) + 1;
        if (slotIndex < 0xc)
        {
            slot = (char*)self + slotIndex * 0x188 + 0xe4c;
            do
            {
                if (*(int*)slot != 0)
                {
                    setRect((char*)self + 0x20b8, 0x1cb, 0x10e, 0x207, 300);
                    EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)
                        (9, (char*)self + 0x20b8, -1, -1, 0, 0);
                    *(int*)((char*)self + 0xdc4) = slotIndex;
                    break;
                }
                slotIndex = slotIndex + 1;
                slot = slot + 0x188;
            } while (slotIndex < 0xc);
        }
        setRect((char*)self + 0x20c8, 0xbd, 0, 0xd1, 0x1e);
        setRect((char*)self + 0x20d8, 0xbd, 0x1e, 0xd1, 0x3c);
        setRect((char*)self + 0x20e8, 0xbd, 0x3c, 0xd1, 0x5a);
        setRect((char*)self + 0x20f8, 0xbd, 0x5a, 0xd1, 0x78);
        setRect((char*)self + 0x2108, 0xbd, 0x78, 0xd1, 0x96);
        setRect((char*)self + 0x2118, 0xbd, 0x96, 0xd1, 0xb4);
        setRect((char*)self + 0x2128, 0xbd, 0xb4, 0xd1, 0xd2);
        setRect((char*)self + 0x2138, 0xbd, 0xc3, 0xd1, 0xe1);
        setRect((char*)self + 0x2148, 0xbd, 0xd2, 0xd1, 0xf0);
        setRect((char*)self + 0x2158, 0xbd, 0xe1, 0xd1, 0xff);
    }
    return;
}

// FUNCTION: TRILOGY 0x00436934
// Deactivates the guide window and removes its registered buttons.
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
void EQ_Guide::Deactivate_guess()
{
    EQ_METHOD(0, void, 0x00450463, (EQ_Guide*), this)();
    *(u32*)((char*)this + 0x70) &= 0;
    this->isActive_guess &= 0;
}

struct EQ_GuideButtonLayout_guess
{
    char unknown00[0x10];
    int active;
    char unknown14[0x6c];
    int mode_value;
    int unknown84;
    int previous_state;
    int state;
    char unknown90[0x0c];
    int sorting;
    int petitioner_found;
    int bug_logged;
    int feedback_logged;
    int guide_logged;
    int sorting_in_progress;
    char request_packet[0x64];
    union
    {
        char bytes[0x4a4];
        struct
        {
            char unknown118[4];
            int urgency;
            char unknown120[0x94];
            int unavailable_count;
              char unknown1b8[4];
              char submitted_text[0x400];
        } fields;
    } outgoing;
    char guide_text[0x400];
    char gm_text[0x3ff];
    u8 append_from_chat;
    int selected_soulmark;
    int next_soulmark;
    int current_soulmark;
    char soulmark_records[0x1170];
};

#define EQ_GUIDE_VCALL(object, byteOffset) \
    ((void (__fastcall *)(EQ_Guide*, int))(u32)(*(u32*)(*(u32*)(object) + (byteOffset))))(object, 0)

class EQ_GuideProcess_guess : public EQ_Guide
{
public:
    void ProcessButton_guess(short buttonState, short buttonId);
};

#define EQ_Guide EQ_GuideProcess_guess
// FUNCTION: TRILOGY 0x00437792
// Handles guide window clicks: open a petition, append chat text, set urgency, send petition actions.
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
void EQ_Guide::ProcessButton_guess(short buttonState, short buttonId)
{
    typedef EQ_GuideButtonLayout_guess GuideLayout;
    GuideLayout* guide;
    int* soulmarkValue;
    int value;
    size_t textLength;
    u32 opcode;
    u32 messageSize;
    int copyCount;
    struct GuideScratch_guess
    {
        char packet_player_name[0x20];
        char bytes[7260];
        char frame_tail[4];
    } scratch;

    guide = (GuideLayout*)this;
    if (guide->active == 0)
        return;
    if (buttonState != 0)
        return;
    if (guide->sorting != 0)
        return;

    value = guide->state;
    if (value == 0 && buttonId == 0)
    {
        EQ_GUIDE_VCALL(this, 8);
        guide->state = 1;
        goto refresh_and_return;
    }

    if (value == 1)
    {
        if (guide->sorting_in_progress == 0)
        {
            if (buttonId < 0x19
                && *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x4c + buttonId * 0x74) > -1)
            {
                guide->sorting = 1;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558FA0, 0x12);
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                    ((void**)0x006B3194, 0x218e,
                     (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x4c + buttonId * 0x74, 4, 1);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (scratch.bytes + 0x1a60, (const char*)0x00558F98,
                     (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x5c + buttonId * 0x74);
                EQ_METHOD(1, void, 0x004CA5C7, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))
                    (scratch.bytes + 0x1a60);
                return;
            }
            if (buttonId == 99)
            {
                EQ_GUIDE_VCALL(this, 8);
                guide->state = 0;
                goto refresh_and_return;
            }
        }
        else
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00558FC4, 0x12);
            return;
        }
    }

    if (value == 2 && buttonId == 0x62)
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x0d, 1, 0);

    if (value == 2)
    {
        switch (buttonId)
        {
        case 0:
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (scratch.bytes + 0x1260, (const char*)0x00558F88, guide->outgoing.fields.submitted_text);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                (scratch.bytes + 0x1260, 0x12);
            break;

        case 1:
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (scratch.bytes + 0x1260, (const char*)0x00558F7C, guide->gm_text);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                (scratch.bytes + 0x1260, 0x0f);
            break;

        case 2:
            if (EQ_GLOBAL(u8, 0x006B6404) == 0
                && EQ_CDECL(int, 0x004D7CA7, (void))() == 2)
            {
                if (EQ_CDECL(size_t, 0x005223D0, (const char*))
                    ((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8) != 0)
                {
                    textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(guide->guide_text);
                    if (textLength == 0)
                    {
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                            (guide->gm_text, (const char*)0x00558F68,
                             EQ_GLOBAL(void*, 0x006E4DA8),
                             (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8);
                    }
                    else
                    {
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                            (guide->gm_text, (const char*)0x00558F70,
                             guide->guide_text, EQ_GLOBAL(void*, 0x006E4DA8),
                             (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8);
                    }
                    guide->append_from_chat = 0;
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8) = 0;
                    EQ_GLOBAL(u8, 0x006B6404) = 1;
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x13ec) = 1;
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                        ((const char*)0x00558F44, 0x12);
                }
                else
                {
                    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                        ((const char*)0x00558EE8, 0x0d);
                }
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558EAC, 0x0d);
            }
            break;

        case 3:
            if (guide->outgoing.fields.urgency != 0)
            {
                value = guide->outgoing.fields.urgency - 1;
                guide->outgoing.fields.urgency = value;
                if (value < 0)
                    guide->outgoing.fields.urgency = 0;
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (scratch.bytes + 0x1a60, (const char*)0x00558BAC);
                if (guide->outgoing.fields.urgency == 1)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (scratch.bytes + 0x1a60, (const char*)0x00558BA4);
                if (guide->outgoing.fields.urgency == 2)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (scratch.bytes + 0x1a60, (const char*)0x00558B98);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (scratch.bytes + 0x1260, (const char*)0x00558E94, scratch.bytes + 0x1a60);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    (scratch.bytes + 0x1260, 0x12);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558E68, 0x0d);
            }
            break;

        case 4:
            if (guide->outgoing.fields.urgency < 2)
            {
                value = guide->outgoing.fields.urgency + 1;
                guide->outgoing.fields.urgency = value;
                if (value > 2)
                    guide->outgoing.fields.urgency = 2;
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                    (scratch.bytes + 0x1a60, (const char*)0x00558BAC);
                if (guide->outgoing.fields.urgency == 1)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (scratch.bytes + 0x1a60, (const char*)0x00558BA4);
                if (guide->outgoing.fields.urgency == 2)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                        (scratch.bytes + 0x1a60, (const char*)0x00558B98);
                EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                    (scratch.bytes + 0x1260, (const char*)0x00558E50, scratch.bytes + 0x1a60);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    (scratch.bytes + 0x1260, 0x12);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558E20, 0x0d);
            }
            break;

        case 5:
            if (guide->petitioner_found == 0)
            {
                guide->outgoing.fields.unavailable_count++;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558DE8, 0x12);
                guide->petitioner_found = 1;
            }
            else
            {
                if (guide->outgoing.fields.unavailable_count != 0)
                    guide->outgoing.fields.unavailable_count--;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558DB0, 0x12);
                guide->petitioner_found = 0;
            }
            break;

        case 6:
            if (guide->bug_logged == 0)
            {
                guide->bug_logged = 1;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558D8C, 0x12);
                textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(guide->guide_text);
                messageSize = (u32)textLength + 0x4a8;
                opcode = 0x21a1;
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                    ((void**)0x006B3194, opcode, guide->outgoing.bytes, messageSize, 1);
            }
            break;

        case 7:
            if (guide->feedback_logged == 0)
            {
                guide->feedback_logged = 1;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558D64, 0x12);
                textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(guide->guide_text);
                messageSize = (u32)textLength + 0x4a8;
                opcode = 0x21a2;
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                    ((void**)0x006B3194, opcode, guide->outgoing.bytes, messageSize, 1);
            }
            break;

        case 8:
            if (guide->guide_logged == 0)
            {
                guide->guide_logged = 1;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558D40, 0x12);
                textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(guide->guide_text);
                messageSize = (u32)textLength + 0x4a8;
                opcode = 0x21a3;
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                    ((void**)0x006B3194, opcode, guide->outgoing.bytes, messageSize, 1);
            }
            break;

        case 9:
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (guide->guide_text, guide->gm_text);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00558D34, 0x12);
            textLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(guide->guide_text);
            messageSize = (u32)textLength + 0x4a8;
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                ((void**)0x006B3194, 0x219e, guide->outgoing.bytes, messageSize, 1);
            EQ_GUIDE_VCALL(this, 8);
            guide->state = 1;
            EQ_GUIDE_VCALL(this, 4);
            break;

        case 10:
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00558D20, 0x12);
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                ((void**)0x006B3194, 0x219f, guide->outgoing.bytes, 4, 1);
            EQ_GUIDE_VCALL(this, 8);
            guide->state = 1;
            EQ_GUIDE_VCALL(this, 4);
            break;

        case 11:
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00558D08, 0x12);
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                ((void**)0x006B3194, 0x21a0, guide->outgoing.bytes, 4, 1);
            EQ_GUIDE_VCALL(this, 8);
            guide->state = 1;
            EQ_GUIDE_VCALL(this, 4);
            break;
        }
    }

    if (guide->state == 3)
    {
        if (buttonId == 0)
        {
            guide->mode_value = 0;
            guide->request_packet[0x62] = 1;
        }
        else if (buttonId == 1)
        {
            guide->request_packet[0x62] = 0;
            guide->mode_value = 0;
        }
        else
        {
            goto check_soulmarks;
        }
        EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
            ((void**)0x006B3194, 0x208c, guide->request_packet, 100, 1);
        EQ_GUIDE_VCALL(this, 8);
        if (guide->previous_state != -1)
        {
            guide->state = guide->previous_state;
            EQ_GUIDE_VCALL(this, 4);
            guide->previous_state = -1;
        }
    }

check_soulmarks:
    if (guide->state == 4)
    {
        if (buttonId == 5)
        {
            soulmarkValue = (int*)((char*)guide + 0xe4c) + guide->selected_soulmark * 0x62;
            if (*soulmarkValue == 3)
            {
                *soulmarkValue = 0;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                    ((const char*)0x00558C88, 0x0f);
                return;
            }
            *soulmarkValue = *soulmarkValue + 1;
        }
        else if (buttonId == 7)
        {
            value = guide->next_soulmark;
            if (value > -1)
            {
                guide->selected_soulmark = value;
                EQ_GUIDE_VCALL(this, 8);
                goto refresh_and_return;
            }
        }
        else if (buttonId == 8)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                ((const char*)0x00558CE0, 0x0f);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (scratch.packet_player_name, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
            soulmarkValue = (int*)scratch.bytes;
            value = 0;
            copyCount = 12;
            do
            {
                EQ_CDECL(void*, 0x00521B10, (void*, const void*, u32))
                    (soulmarkValue, (char*)guide + 0xdc8 + value, 0x188);
                soulmarkValue = (int*)((char*)soulmarkValue + 0x188);
                value += 0x188;
                copyCount--;
            }
            while (copyCount != 0);
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                ((void**)0x006B3194, 0x21d1, scratch.packet_player_name, 0x1280, 1);
            EQ_GUIDE_VCALL(this, 8);
            return;
        }
        else if (buttonId == 9)
        {
            value = guide->current_soulmark;
            if (value > -1)
            {
                guide->selected_soulmark = value;
                EQ_GUIDE_VCALL(this, 8);
                goto refresh_and_return;
            }
        }
    }
    return;

refresh_and_return:
    EQ_GUIDE_VCALL(this, 4);
    return;
}

#undef EQ_Guide
#undef EQ_GUIDE_VCALL

struct EQ_GuideRect_guess
{
    int left;
    int top;
    int right;
    int bottom;
};

class EQ_GuideDraw_guess
{
public:
    void drawBorders_guess();
};

// FUNCTION: TRILOGY 0x00437eca
// Draws border strips around guide window regions.
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
void EQ_GuideDraw_guess::drawBorders_guess()
{
    typedef int (__stdcall *CopyRectFunction_guess)(EQ_GuideRect_guess*, const EQ_GuideRect_guess*);
    typedef int (__stdcall *SetRectFunction_guess)(EQ_GuideRect_guess*, int, int, int, int);
    SetRectFunction_guess pcSetRect;
    EQ_GuideRect_guess edgeDestRect;
    EQ_GuideRect_guess textureRect;
    EQ_GuideRect_guess destRect;
    int rectWidth;
    int borderEntry;
    EQ_GuideDraw_guess* drawObj;
    int yBottom;

    if (*(int*)((char*)this + 0x90) != 0)
    {
        drawObj = this;
        {
            for (borderEntry = *(int*)((char*)this + 0x70); borderEntry != 0; borderEntry = *(int*)(borderEntry + 0x20))
            {
                pcSetRect = EQ_GLOBAL(SetRectFunction_guess, 0x00534248);
                EQ_GLOBAL(CopyRectFunction_guess, 0x00534240)
                    (&destRect, (const EQ_GuideRect_guess*)((char*)drawObj + *(short*)(borderEntry + 0x14) * 0x10 + 0x2028));
                yBottom = destRect.bottom - destRect.top;
                rectWidth = destRect.right - destRect.left + 1;
                pcSetRect(&textureRect, 0, 0, rectWidth, 1);
                if (EQ_GLOBAL(int, 0x0058FC6C) == 2 && EQ_GLOBAL(u16, 0x006B63E4) != 0x280)
                {
                    EQ_METHOD(3, void, 0x004500BE, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &destRect);
                }
                else
                {
                    EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*, int), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &destRect, 1);
                }
                pcSetRect(&textureRect, 0x27f, 0, 0x280, yBottom);
                if (EQ_GLOBAL(int, 0x0058FC6C) == 2 && EQ_GLOBAL(u16, 0x006B63E4) != 0x280)
                {
                    EQ_METHOD(3, void, 0x004500BE, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &destRect);
                }
                else
                {
                    EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*, int), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &destRect, 1);
                }
                pcSetRect(&textureRect, 0, 0, rectWidth, 1);
                pcSetRect(&edgeDestRect, destRect.left, destRect.bottom, 2000, 2000);
                if (EQ_GLOBAL(int, 0x0058FC6C) == 2 && EQ_GLOBAL(u16, 0x006B63E4) != 0x280)
                {
                    EQ_METHOD(3, void, 0x004500BE, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &edgeDestRect);
                }
                else
                {
                    EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*, int), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &edgeDestRect, 1);
                }
                pcSetRect(&textureRect, 0x27f, 0, 0x280, yBottom);
                pcSetRect(&edgeDestRect, destRect.right, destRect.top, 2000, 2000);
                if (EQ_GLOBAL(int, 0x0058FC6C) == 2 && EQ_GLOBAL(u16, 0x006B63E4) != 0x280)
                {
                    EQ_METHOD(3, void, 0x004500BE, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &edgeDestRect);
                }
                else
                {
                    EQ_METHOD(4, void, 0x00450125, (void*, void*, EQ_GuideRect_guess*, EQ_GuideRect_guess*, int), drawObj)(*(void**)(EQ_GLOBAL(char*, 0x006E4E04) + 4), &textureRect, &edgeDestRect, 1);
                }
            }
        }
    }
}

class EQ_Guide_guess
{
public:
    void Inquire(char* text);
};

// FUNCTION: TRILOGY 0x004380c0
// Sends an inquiry using the target player's name and PID.
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
void EQ_Guide_guess::Inquire(char* text)
{
    u32 length;
    char* word;
    char packet[128];
    char firstWord[32];
    int inquiring;

    inquiring = *(int*)((char*)this + 0x10);
    if (inquiring != 0 && (*(int*)((char*)this + 0x8C) == 2 || *(int*)((char*)this + 0x8C) == 3))
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00559068, 15);
        return;
    }
    if (*(int*)((char*)this + 0x10) != 0 && *(int*)((char*)this + 0x8C) == 4)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00559024, 15);
        return;
    }

    word = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(text, firstWord);
    firstWord[31] = 0;
    if (firstWord[0] != 0 && word[0] != 0)
    {
        length = EQ_CDECL(u32, 0x005223D0, (const char*))(word);
        if ((s32)length > 0x1F)
            word[0x1F] = 0;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))
            (packet, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
        packet[0x20] = 0;
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x40, firstWord);
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x60, word);
        EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
            (EQ_ARRAY(void*, 0x006B3194), 0x21D0, packet, 0x80, 1);
        return;
    }

    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00558FF4, 15);
    return;
}

class EQ_GuideAction_guess
{
public:
    void Praise_guess(char* text);
};

// FUNCTION: TRILOGY 0x0043819F
// Sends praise for a named player with the supplied message.
void EQ_GuideAction_guess::Praise_guess(char* text)
{
    char* remainingText;
    size_t messageLength;
    char* messageText;
    char packet[0x188];
    char firstWord[32];
    char secondWord[32];

    if ((*(int*)((char*)this + 0x10) == 0) || (*(int*)((char*)this + 0x8C) != 4))
    {
        remainingText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(text, firstWord);
        firstWord[31] = 0;
        messageText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(remainingText, secondWord);
        secondWord[31] = 0;
        if ((firstWord[0] != 0) && ((secondWord[0] != 0) && (*messageText != 0)))
        {
            messageLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(messageText);
            if (0xFF < (int)messageLength)
                messageText[0xFF] = 0;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet, firstWord);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x20, secondWord);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (packet + 0x40, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
            packet[0x60] = 0;
            *(u32*)(packet + 0x80) = 0;
            *(u32*)(packet + 0x84) = 1;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x88, messageText);
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                (EQ_ARRAY(void*, 0x006B3194), 0x21D2, packet, 0x188, 1);
            return;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005590AC, 0xF);
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005590E4, 0xF);
    }
}

// FUNCTION: TRILOGY 0x004382ae
// Sends a warning for a named player with the supplied message.
void __fastcall Warn_guess(void* guideAction, int unused_edx, char* action)
{
    char* remainingText;
    size_t messageLength;
    char* messageText;
    char firstWord[32];
    char secondWord[32];
    char packet[0x188];

    if ((*(int*)((char*)guideAction + 0x10) == 0) || (*(int*)((char*)guideAction + 0x8c) != 4))
    {
        remainingText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(action, firstWord);
        firstWord[31] = 0;
        messageText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(remainingText, secondWord);
        secondWord[31] = 0;
        if ((firstWord[0] != 0) && ((secondWord[0] != 0) && (*messageText != 0)))
        {
            messageLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(messageText);
            if (0xFF < (int)messageLength)
                messageText[0xFF] = 0;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet, firstWord);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x20, secondWord);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))
                (packet + 0x40, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
            packet[0x60] = 0;
            *(int*)(packet + 0x80) = 0;
            *(int*)(packet + 0x84) = 2;
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(packet + 0x88, messageText);
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                (EQ_ARRAY(void*, 0x006B3194), 0x21D2, packet, 0x188, 1);
            return;
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00559158, 0xF);
    }
    else
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x005590E4, 0xF);
    }
}

// FUNCTION: TRILOGY 0x004383bd
// Stores replacement text entered with /edit in the active guide window.
// ORIGINAL FILE: EverQuest/EQ_Guide.cpp (from Mac debug map)
void EQ_Guide::Edit(char* text)
{
    u32 length;
    u32 index;
    u32 offset;
    char* destination;

    if (this->isActive_guess != 0 && *(u32*)((char*)this + 0x8C) == 4)
    {
        if (*text == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0055918C, 15);
        }
        else
        {
            length = EQ_CDECL(u32, 0x005223D0, (const char*))(text);
            if ((s32)length > 0xFF)
                text[0xFF] = 0;
            index = *(u32*)((char*)this + 0xDBC);
            offset = index * 0x188 + 0xE50;
            destination = (char*)((u32)this + offset);
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(destination, text);
        }
    }
}
