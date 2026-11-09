#include "trilogy/common.h"
#include "trilogy/eqaddr.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/layout/eq_bag.h"

/* fake match: pos and y are loop temps that pin VC6's ecx/eax order; getProfile is a local pointer that VC6 spills (not ebx) */
// FUNCTION: TRILOGY 0x00416d0c
// Sets up the bag window overlay and reads its saved screen positions.
// ORIGINAL FILE: EverQuest/EQ_Bag.cpp (from Mac debug map)
// @early-stop: frame 0x54 vs 0x4c (EH this copy not shared with positions/x); SetRect and GetPrivateProfileStringA pointers lose ebx to hoisted 0xc2 and *NULL* constants
EQ_Bag_guess::EQ_Bag_guess(int unused)
{
    char key[0x20];
    char value[0x20];
    int index;
    int x;
    int y;
    u32* positions;
    u32* pos;

    *(short*)((char*)this + 0x94) = -1;
    *(u32*)((char*)this + 0x88) |= 0xffffffff;
    *(u32*)this = 0x00535264;
    *(u32*)((char*)this + 0x10) = 0;
    *(u32*)((char*)this + 0xa4) = 0;
    *(u32*)((char*)this + 0x98) = 0;
    *(u32*)((char*)this + 0x9c) = 0;
    *(u32*)((char*)this + 0xa0) = 0;
    *(u16*)((char*)this + 0x84) = 0;
    *(u16*)((char*)this + 0x86) = 0;
    *(u32*)((char*)this + 0xa8) = 0;
    *(u32*)((char*)this + 0xac) = 0;
    *(u32*)((char*)this + 0x160) = 0;
    *(u32*)((char*)this + 0x174) = 0;
    *(u32*)((char*)this + 0x70) = 0;

    typedef int (__stdcall *SetRect_guess_t)(void*, int, int, int, int);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((char*)this + 0x24, 0, 0, 0xc2, 0x8f);
    EQ_GLOBAL(SetRect_guess_t, 0x00534248)((char*)this + 0x14, 0, 0, 0xc2, 0x8f);

    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, (const char*)EQ_ARRAY(char, 0x0053E950));
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, (const char*)EQ_ARRAY(char, 0x0053EAF8));
    EQ_METHOD(2, void, 0x00450168, (void*, int, int), this)(1, 0);

    if (EQ_GLOBAL(int, 0x005918F0) == 0)
    {
        typedef u32 (__stdcall *GetPrivateProfileStringA_guess_t)
            (const char*, const char*, const char*, char*, u32, const char*);
        EQ_GLOBAL(int, 0x005918F0) = 1;
        GetPrivateProfileStringA_guess_t getProfile =
            EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C);

        index = 0;
        positions = EQ_ARRAY(u32, 0x00591848);
        do
        {
            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (key, (const char*)EQ_ARRAY(char, 0x0053EADC), index);
            getProfile((const char*)EQ_ARRAY(char, 0x0053EAE8), key,
                (const char*)EQ_ARRAY(char, 0x0053DB48), value, 0x20,
                (const char*)EQ_ARRAY(char, 0x0053DB50));
            if (value[0] == '*')
            {
                x = 0x118;
            }
            else
            {
                x = EQ_CDECL(int, 0x00522781, (const char*))(value);
            }

            EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                (key, (const char*)EQ_ARRAY(char, 0x0053EAD0), index);
            getProfile((const char*)EQ_ARRAY(char, 0x0053EAE8), key,
                (const char*)EQ_ARRAY(char, 0x0053DB48), value, 0x20,
                (const char*)EQ_ARRAY(char, 0x0053DB50));
            if (value[0] == '*')
            {
                y = 10;
            }
            else
            {
                y = EQ_CDECL(int, 0x00522781, (const char*))(value);
            }
            pos = positions;
            index += 1;
            pos[-1] = x;
            pos[0] = y;
            pos += 2;
            positions = pos;
        } while ((int)pos < (int)0x005918C8);
    }
}

class BagDtorEHMarker416EBA_guess
{
public:
    ~BagDtorEHMarker416EBA_guess() {}
};

class BagDtor416EBA_guess
{
public:
    ~BagDtor416EBA_guess();
};

/* fake match: EH marker orders state; volatile this preserves the behavior-neutral unwind slot. */
// FUNCTION: TRILOGY 0x00416eba
// Releases the window's buttons and base overlay.
BagDtor416EBA_guess::~BagDtor416EBA_guess()
{
    BagDtor416EBA_guess* volatile unwind_this = this;
    *(u32*)this = 0x00535264;
    {
        BagDtorEHMarker416EBA_guess eh_marker;
        void (__fastcall* delButtons)(void*) = EQ_THISCALL(void, 0x00450463, (void*));
        delButtons(this);
    }
    EQ_METHOD(0, void, 0x0045005D, (void*), this)();
}

/* fake match: selector is an int copy of hoveredId, so VC6 keeps one movsx eax for the whole chain (a short compare narrows to 16-bit cmp di) */
// FUNCTION: TRILOGY 0x00416ef2
// Shows container help for the hovered slot, close control, or combine action.
// ORIGINAL FILE: EverQuest/EQ_Bag.cpp (from Mac debug map)
// @early-stop: raw bytes differ only by the EQ_THISCALL stand-in's dummy EDX (xor edx,edx before the DisplayAutoHelpText call); match.sh normalises it.
void __fastcall AutoHelp(void* self)
{
    typedef long LONG_guess;
    typedef int BOOL_guess;
    struct POINT_guess
    {
        LONG_guess x;
        LONG_guess y;
    };
    struct RECT_guess
    {
        LONG_guess left;
        LONG_guess top;
        LONG_guess right;
        LONG_guess bottom;
    };

    POINT_guess pt;
    BOOL_guess BVar1;
    int cursorY;
    int element;
    short hoveredId;
    int selector;
    char buffer[128];
    typedef BOOL_guess (__stdcall *PtInRectProc_guess)(const RECT_guess*, POINT_guess);

    hoveredId = -1;
    if (*(int*)0x006b6864 == 0)
        return;
    if (*(short*)(*(u32*)0x006e4e44 + 0xec2) < 3)
        return;
    pt.x = (LONG_guess)*(short*)0x006b6414;
    buffer[0] = 0;
    cursorY = (int)*(short*)0x006b6416;
    if (*(int*)((char*)self + 0x10) == 0)
        return;
    for (element = *(int*)((char*)self + 0x70); element != 0; element = *(int*)(element + 0x20))
    {
        pt.y = cursorY;
        BVar1 = EQ_GLOBAL(PtInRectProc_guess, 0x0053423C)((RECT_guess*)(element + 4), pt);
        if (BVar1 != 0)
        {
            hoveredId = *(short*)(element + 0x14);
            break;
        }
    }
    *(short*)(*(u32*)0x006e4e44 + 0xebc) = hoveredId;
    if (hoveredId == -1)
        return;
    selector = hoveredId;
    if (selector >= 0)
    {
        if (selector > 9)
        {
            if (selector != 10)
            {
                if (selector == 0xb)
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))(buffer, EQ_ARRAY(char, 0x0053EB24));
                else
                {
                    *(short*)(*(u32*)0x006e4e44 + 0xebc) = -1;
                    return;
                }
            }
            else
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))(buffer, EQ_ARRAY(char, 0x0053EB14));
        }
        else
            EQ_CDECL(char*, 0x005219C0, (char*, const char*))(buffer, EQ_ARRAY(char, 0x0053EB00));
        EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, const char*, int), (void*)*(u32*)0x006e4e44)(pt.x, cursorY + 0x14, buffer, 10);
    }
    else
        *(short*)(*(u32*)0x006e4e44 + 0xebc) = -1;
}

// FUNCTION: TRILOGY 0x00416fdd
// Copies the container type name onto its item and builds the container window layout.
// @early-stop: zero in EDI (orig EBX), temp1 in EBX (orig EDI); clamps reload screen size as 16-bit; no height spill; first SetRect is call-through-memory
void __fastcall GetContainerTypeName_guess(void* self, int edx_unused, int positionIndex)
{
    typedef int (__stdcall *SetRectProc_guess)(void*, int, int, int, int);
    typedef void (__fastcall *VCallProc_guess)(void*, int);

    char* base = (char*)self;
    u8 containerType;
    short height;
    short temp1;
    short temp2;
    int soundId;
    int baseY;
    int item;
    int slotCount;
    void* lprc_00;
    void* lprc;
    void* obj;

    EQ_METHOD(0, void, 0x00450463, (void*), self)();
    *(u16*)(base + 0x94) = 0xffff;
    *(u8*)(base + 0x8c) = 0;
    *(int*)(base + 0x70) = 0;
    *(int*)(base + 0xa8) = 0;
    if (*(int*)(base + 0x174) != 0)
    {
        *(int*)(base + 0x88) = positionIndex;
        if (positionIndex < 0 || 0xf < positionIndex)
        {
            *(int*)(base + 0x88) = -1;
            temp2 = 0x118;
            temp1 = 10;
            if (self == (void*)*(u32*)0x006e4e58)
                temp2 = 400;
        }
        else
        {
            temp2 = *(short*)(0x00591844 + positionIndex * 8);
            temp1 = *(short*)(0x00591848 + positionIndex * 8);
        }
        obj = (void*)*(u32*)0x006018c0;
        if (obj != 0 && *(int*)((char*)obj + 0x10) != 0)
            ((VCallProc_guess)(*(u32**)obj)[2])(obj, 0);
        obj = (void*)*(u32*)0x00604984;
        if (obj != 0 && *(int*)((char*)obj + 0x10) != 0)
        {
            *(int*)((char*)obj + 0x1e8) = 1;
            obj = (void*)*(u32*)0x00604984;
            ((VCallProc_guess)(*(u32**)obj)[2])(obj, 0);
        }
        soundId = 0x91;
        containerType = *(u8*)(*(int*)(base + 0x174) + 0xd4);
        if (5 < containerType &&
            (containerType < 8 || containerType == 10 || containerType == 0xf ||
             (0x11 < containerType && (containerType < 0x15 || containerType == 0x16))))
        {
            soundId = 0x86;
        }
        obj = (void*)*(u32*)0x006e4dd8;
        if (obj != 0)
            EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), obj)(soundId, 0);
        containerType = *(u8*)(*(int*)(base + 0x174) + 0xd5);
        *(u16*)(base + 0x84) = 0x76;
        u16 rowCount = (u16)(containerType >> 1);
        if (*(u8*)(*(int*)(base + 0x174) + 0xd4) < 9)
            height = rowCount * 0x30 + 0x76;
        else
            height = rowCount * 0x30 + 0x94;
        *(short*)(base + 0x86) = height;
        if ((int)temp2 > *(int*)0x006b63fc - 0x77)
            temp2 = *(int*)0x006b63fc - 0x77;
        if (temp2 < 0)
            temp2 = 0;
        height = *(short*)(base + 0x86);
        if ((int)temp1 > *(int*)0x006b6400 - height - 1)
            temp1 = *(int*)0x006b6400 - height - 1;
        if (temp1 < 0)
            temp1 = 0;
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)((lprc = base + 0x24), (int)temp2, (int)temp1, temp2 + 0x76, (int)height + (int)temp1);
        SetRectProc_guess setRect = EQ_GLOBAL(SetRectProc_guess, 0x00534248);
        setRect(base + 0xb0, *(int*)lprc + 0xb, *(int*)(base + 0x28) + 0x58,
                *(int*)lprc + 0x39, *(int*)(base + 0x28) + 0x86);
        setRect(base + 0xc0, *(int*)lprc + 0x3c, *(int*)(base + 0x28) + 0x58,
                *(int*)lprc + 0x6a, *(int*)(base + 0x28) + 0x86);
        setRect(base + 0xd0, *(int*)lprc + 0xb, *(int*)(base + 0x28) + 0x88,
                *(int*)lprc + 0x39, *(int*)(base + 0x28) + 0xb6);
        setRect(base + 0xe0, *(int*)lprc + 0x3c, *(int*)(base + 0x28) + 0x88,
                *(int*)lprc + 0x6a, *(int*)(base + 0x28) + 0xb6);
        setRect(base + 0xf0, *(int*)lprc + 0xb, *(int*)(base + 0x28) + 0xb8,
                *(int*)lprc + 0x39, *(int*)(base + 0x28) + 0xe6);
        setRect(base + 0x100, *(int*)lprc + 0x3c, *(int*)(base + 0x28) + 0xb8,
                *(int*)lprc + 0x6a, *(int*)(base + 0x28) + 0xe6);
        setRect(base + 0x110, *(int*)lprc + 0xb, *(int*)(base + 0x28) + 0xe8,
                *(int*)lprc + 0x39, *(int*)(base + 0x28) + 0x116);
        setRect(base + 0x120, *(int*)lprc + 0x3c, *(int*)(base + 0x28) + 0xe8,
                *(int*)lprc + 0x6a, *(int*)(base + 0x28) + 0x116);
        setRect(base + 0x130, *(int*)lprc + 0xb, *(int*)(base + 0x28) + 0x118,
                *(int*)lprc + 0x39, *(int*)(base + 0x28) + 0x146);
        setRect(base + 0x140, *(int*)lprc + 0x3c, *(int*)(base + 0x28) + 0x118,
                *(int*)lprc + 0x6a, *(int*)(base + 0x28) + 0x146);
        item = 0;
        if (*(u8*)(*(int*)(base + 0x174) + 0xd5) != 0)
        {
            do
            {
                EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)(item, base + ((short)item + 0xb) * 0x10, -1, -1, 0, 0);
                item = item + 1;
            } while ((short)item < (short)(u16)*(u8*)(*(int*)(base + 0x174) + 0xd5));
        }
        slotCount = *(u8*)(*(int*)(base + 0x174) + 0xd5);
        if (*(u8*)(*(int*)(base + 0x174) + 0xd4) >= 9)
        {
            lprc_00 = base + 0x150;
            item = (slotCount * 0x30) / 2 + *(int*)(base + 0x28);
            setRect(lprc_00, *(int*)lprc + 0x24, item + 0x78, *(int*)lprc + 0x52, item + 0x89);
            baseY = (*(u8*)(*(int*)(base + 0x174) + 0xd5) * 0x30) / 2 + *(int*)(base + 0x28);
            setRect(base + 0x164, *(int*)lprc + 0x14, baseY + 0x5c, *(int*)lprc + 0x61, baseY + 0x6d);
        }
        else
        {
            lprc_00 = base + 0x150;
            baseY = (slotCount * 0x30) / 2 + *(int*)(base + 0x28);
            setRect(lprc_00, *(int*)lprc + 0x24, baseY + 0x5a, *(int*)lprc + 0x52, baseY + 0x6b);
        }
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)(10, lprc_00, -1, -1, 0, 0);
        if (*(u8*)(*(int*)(base + 0x174) + 0xd4) >= 9)
        {
            EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)(0xb, base + 0x164, -1, -1, 0, 0);
        }
        EQ_METHOD(6, void, 0x004503BE, (void*, int, void*, int, int, int, int), self)(0xc, lprc, -1, -1, 0, 0);
        *(int*)(base + 0x10) = 1;
        item = *(int*)(*(u32*)0x006e4e44 + 0x1010);
        if (item != 0 && item == *(int*)(base + 0x174))
        {
            switch (*(u8*)(*(int*)(base + 0x174) + 0xd4))
            {
            case 0:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ee08);
                break;
            case 1:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053edfc);
                break;
            case 2:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053edf4);
                break;
            case 3:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ede8);
                break;
            case 4:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eddc);
                break;
            case 5:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053edd0);
                break;
            case 6:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053edc4);
                break;
            case 7:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053edb8);
                break;
            default:
                break;
            case 9:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eda8);
                break;
            case 10:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed9c);
                break;
            case 0xb:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed94);
                break;
            case 0xc:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed8c);
                break;
            case 0xd:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed7c);
                break;
            case 0xe:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed70);
                break;
            case 0xf:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed68);
                break;
            case 0x10:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed5c);
                break;
            case 0x11:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed54);
                break;
            case 0x12:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed44);
                break;
            case 0x13:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed38);
                break;
            case 0x14:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed28);
                break;
            case 0x15:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed18);
                break;
            case 0x16:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed10);
                break;
            case 0x17:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ed04);
                break;
            case 0x18:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ecf0);
                break;
            case 0x19:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ece0);
                break;
            case 0x1a:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ecc8);
                break;
            case 0x1b:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ecb4);
                break;
            case 0x1e:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eca4);
                break;
            case 0x1f:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec94);
                break;
            case 0x20:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec84);
                break;
            case 0x21:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec78);
                break;
            case 0x22:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec64);
                break;
            case 0x23:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec54);
                break;
            case 0x24:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec44);
                break;
            case 0x26:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec34);
                break;
            case 0x27:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec24);
                break;
            case 0x28:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ec10);
                break;
            case 0x2d:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ebfc);
                break;
            case 0x2a:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ebe8);
                break;
            case 0x29:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ebd0);
                break;
            case 0x2b:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053ebb8);
                break;
            case 0x2c:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eba0);
                break;
            case 0x2e:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eb94);
                break;
            case 0x2f:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eb88);
                break;
            case 0x31:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eb7c);
                break;
            case 0x30:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eb6c);
                break;
            case 0x32:
                EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)item, (const char*)0x0053eb60);
                break;
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004176a0
// Saves the bag window position and closes the active container, sending a close message when needed.
// @early-stop: profile sprintf cleanups merge into add esp, 0x18; IAT loads are repeated; packet fields land 4 bytes below the original
/* fake match: pass the sound ID in EQ_THISCALL's ignored EDX slot to preserve the original's live EDX value */
void __fastcall SaveBagPositionAndClose_guess(void* self)
{
    typedef int (__stdcall *WritePrivateProfileStringAProc_guess)(const char*, const char*, const char*, const char*);
    typedef void (__fastcall *BagCallback_guess)(void*);

    struct MsgClickObjectAck_guess      /* OP_CraftingStation (0x20D7), 0x14 bytes */
    {
        u32     player_id;              /* 0x00 */
        u32     drop_id;                /* 0x04 */
        short   open;                   /* 0x08 */
        u8      unknown0x0A[10];        /* 0x0A */
    } msgAck;
    struct MsgClientError_guess         /* OP_ClientError (0x2147), 0x5C bytes */
    {
        int     unknown0x00;            /* 0x00 */
        int     unknown0x04;            /* 0x04 */
        int     reason;                 /* 0x08 */
        u8      unknown0x0C[80];        /* 0x0C */
    } msgError;
    char profileKey[32];
    char profileValue[32];
    void* object;
    void* openContainer;
    u8 containerType;
    u32 soundId;

    *(u32*)((char*)self + 0x98) = 0;
    *(u32*)((char*)self + 0x9c) = 0;
    *(u32*)((char*)self + 0xa0) = 0;
    if (*(int*)((char*)self + 0x88) != -1)
    {
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (profileKey, EQ_ARRAY(char, 0x0053EADC), *(int*)((char*)self + 0x88));
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (profileValue, EQ_ARRAY(char, 0x0053E16C),
                EQ_ARRAY(u32, 0x00591844)[*(int*)((char*)self + 0x88) * 2]);
        EQ_GLOBAL(WritePrivateProfileStringAProc_guess, 0x00534188)
            (EQ_ARRAY(char, 0x0053EAE8), profileKey, profileValue, EQ_ARRAY(char, 0x0053DB50));
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (profileKey, EQ_ARRAY(char, 0x0053EAD0), *(int*)((char*)self + 0x88));
        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
            (profileValue, EQ_ARRAY(char, 0x0053E16C),
                EQ_ARRAY(u32, 0x00591848)[*(int*)((char*)self + 0x88) * 2]);
        EQ_GLOBAL(WritePrivateProfileStringAProc_guess, 0x00534188)
            (EQ_ARRAY(char, 0x0053EAE8), profileKey, profileValue, EQ_ARRAY(char, 0x0053DB50));
    }
    if (*(int*)((char*)self + 0x174) != 0)
    {
        object = EQ_GLOBAL(void*, 0x006018C0);
        if (object != 0 && *(int*)((char*)object + 0x10) != 0)
            ((BagCallback_guess)(*(u32*)(*(u32*)object + 8)))(object);
        object = EQ_GLOBAL(void*, 0x00604984);
        if (object != 0 && *(int*)((char*)object + 0x10) != 0)
        {
            *(u32*)((char*)object + 0x1e8) = 1;
            object = EQ_GLOBAL(void*, 0x00604984);
            ((BagCallback_guess)(*(u32*)(*(u32*)object + 8)))(object);
        }
        soundId = 0x91;
        containerType = *(u8*)(*(char**)((char*)self + 0x174) + 0xd4);
        if (containerType >= 6 &&
            (containerType <= 7 || containerType == 10 || containerType == 0xf ||
                (containerType > 0x11 && (containerType <= 0x14 || containerType == 0x16))))
            soundId = 0x85;
        if (EQ_GLOBAL(void*, 0x006E4DD8) != 0)
            EQ_THISCALL(void, 0x004E34FA, (void*, int, u32, int))
                (EQ_GLOBAL(void*, 0x006E4DD8), soundId, soundId, 0);
        EQ_METHOD(0, void, 0x0043407E, (void*), *(void**)((char*)self + 0x174))();
        if (*(int*)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1010) != 0 &&
            *(int*)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1010) == *(int*)((char*)self + 0x174))
        {
            msgAck.player_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
            msgAck.drop_id = *(u32*)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1014);
            msgAck.open = 0;
            EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                (EQ_ARRAY(void*, 0x006B3194), 0x20d7, &msgAck, 0x14, 1);
            openContainer = *(void**)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1010);
            if (openContainer != 0)
            {
                EQ_METHOD(0, void, 0x0043405A, (void*), openContainer)();
                EQ_CDECL(void, 0x005218BB, (void*))(openContainer);
            }
            *(u32*)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1010) = 0;
            *(u32*)(EQ_GLOBAL(char*, 0x006E4E44) + 0x1014) = 0;
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 1)
            {
                msgError.reason = 8;
                msgError.unknown0x00 = -4;
                msgError.unknown0x04 = -4;
                EQ_CDECL(void, 0x004E1C45, (void**, u32, void*, u32, int))
                    (EQ_ARRAY(void*, 0x006B3194), 0x2147, &msgError, 0x5c, 1);
            }
            *(u32*)(EQ_GLOBAL(char*, 0x006E4E44) + 0xe5c) = 0;
        }
    }
    *(u32*)((char*)self + 0xa8) = 0;
    *(u32*)((char*)self + 0x160) = 0;
    pinstEQ_Main_guess->uiDirty_u32_guess = 1;
    EQ_METHOD(0, void, 0x004504BA, (void*), self)();
}

/* fake match: xLeft is read before the re-read of self->top (the Ghidra order is the reverse), so VC6 gives xLeft EAX and the top value ECX as the original has them */
// FUNCTION: TRILOGY 0x004178f4
// Moves the window within screen bounds, records its new position, and shifts its controls.
// @early-stop: frame is 1 slot, not 2 (VC6 spills oldY/dy into the newX slot [ebp+8]), so one push is missing and every later jump index is off by one; the then-branch's DAT_6b6400 - height takes ECX not EAX; the EQ_THISCALL dummy EDX xor is extra
void __fastcall EQ_Bag_updateBagPosition_guess(void* self, int edx, int newX, int newY)
{
    typedef int (__stdcall *SetRectProc_guess)(void*, int, int, int, int);
    typedef int (__stdcall *CopyRectProc_guess)(void*, const void*);

    int* rect = (int*)((char*)self + 0x24);
    int oldX;
    int oldY;
    int xLeft;
    int tmp;
    int dx;
    int dy;
    short elementId;
    int element;

    pinstEQ_Main_guess->uiDirty_guess = 1;
    oldX = rect[0];
    rect[0] = newX;
    oldY = *(int*)((char*)self + 0x28);
    *(int*)((char*)self + 0x28) = newY;
    if (EQ_GLOBAL(int, 0x0058FC6C) == 2 || EQ_GLOBAL(int, 0x006B63FC) == 0x280)
    {
        tmp = EQ_GLOBAL(int, 0x006B63FC) - *(short*)((char*)self + 0x84) - 1;
        if (newX > tmp)
            rect[0] = tmp;
        tmp = EQ_GLOBAL(int, 0x006B6400) - *(short*)((char*)self + 0x86) - 1;
    }
    else
    {
        tmp = 0x27f - *(short*)((char*)self + 0x84);
        if (newX > tmp)
            rect[0] = tmp;
        tmp = 0x1df - *(short*)((char*)self + 0x86);
    }
    if (newY > tmp)
        *(int*)((char*)self + 0x28) = tmp;
    if (rect[0] < 0)
        rect[0] = 0;
    if (*(int*)((char*)self + 0x28) < 0)
        *(int*)((char*)self + 0x28) = 0;
    xLeft = rect[0];
    tmp = *(int*)((char*)self + 0x28);
    dy = tmp - oldY;
    dx = xLeft - oldX;
    EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, xLeft, tmp,
        *(short*)((char*)self + 0x84) + xLeft,
        *(short*)((char*)self + 0x86) + tmp);
    if (*(int*)((char*)self + 0x88) != -1)
    {
        EQ_ARRAY(int, 0x00591844)[*(int*)((char*)self + 0x88) * 2] = rect[0];
        EQ_ARRAY(int, 0x00591848)[*(int*)((char*)self + 0x88) * 2] = *(int*)((char*)self + 0x28);
    }
    element = *(int*)((char*)self + 0x70);
    while (element != 0)
    {
        EQ_METHOD(2, void, 0x00419583, (void*, int, int), (void*)element)(dx, dy);
        elementId = *(short*)(element + 0x14);
        if (elementId < (short)(unsigned char)*(unsigned char*)((char*)*(int*)((char*)self + 0x174) + 0xd5))
        {
            rect = (int*)((char*)self + (elementId + 0xb) * 0x10);
            EQ_GLOBAL(CopyRectProc_guess, 0x00534240)(rect, (void*)(element + 4));
        }
        else
        {
            if (elementId == 10)
            {
                rect = (int*)((char*)self + 0x150);
                EQ_GLOBAL(CopyRectProc_guess, 0x00534240)(rect, (void*)(element + 4));
            }
            else if (elementId == 0xb)
            {
                rect = (int*)((char*)self + 0x164);
                EQ_GLOBAL(CopyRectProc_guess, 0x00534240)(rect, (void*)(element + 4));
            }
        }
        element = *(int*)(element + 0x20);
    }
}

// FUNCTION: TRILOGY 0x00417a5c
// Draws an open container window: frame, bag and slot item icons with number labels, blinking slot, bag name
// ORIGINAL FILE: EverQuest/EQ_Bag.cpp (from Mac debug map)
void __fastcall EQ_Bag_draw_guess(void* self, int edx)
{
    typedef int (__stdcall *SetRectProc_guess)(void*, int, int, int, int);

    int tmp;
    int temp;
    int value;
    size_t nameLen;
    char* namePtr;
    short textWidth;
    char text[64];
    int pos[4];
    int rect[4];
    int slotOffset;
    int index;

    tmp = EQ_CDECL(int, 0x004504AC, (void))();
    if (tmp != 0)
    {
        if (*(int*)((char*)self + 0xa8) != 0)
        {
            EQ_METHOD(2, void, 0x004178F4, (void*, int, int), self)(EQ_GLOBAL(int, 0x006EFC80), EQ_GLOBAL(int, 0x006EFC84));
        }
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x167, 0xe8, 0x1de, 0x140);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4), rect,
             (char*)self + 0x24, 0);
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x167, 0x141, 0x1de, 0x172);
        index = 0;
        if ((*(unsigned char*)(*(int*)((char*)self + 0x174) + 0xd5) & 0xfe) != 0)
        {
            do
            {
                tmp = (short)index * 0x30 + *(int*)((char*)self + 0x28);
                EQ_GLOBAL(SetRectProc_guess, 0x00534248)(pos,
                    *(int*)((char*)self + 0x24), tmp + 0x58,
                    *(int*)((char*)self + 0x2c), tmp + 0x88);
                EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4), rect, pos, 0);
                index = index + 1;
            } while ((short)index < (short)(unsigned short)
                (*(unsigned char*)(*(int*)((char*)self + 0x174) + 0xd5) >> 1));
        }
        *(unsigned short*)&index = (unsigned short)
            (*(unsigned char*)(*(int*)((char*)self + 0x174) + 0xd5) >> 1);
        if (*(unsigned char*)(*(int*)((char*)self + 0x174) + 0xd4) < 9)
        {
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x167, 0x173, 0x1de, 0x192);
            temp = (short)index * 0x30 + *(int*)((char*)self + 0x28);
            tmp = temp + 0x76;
            temp = temp + 0x58;
            value = *(int*)((char*)self + 0x2c);
        }
        else
        {
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x167, 0x193, 0x1de, 0x1b1);
            slotOffset = (short)index * 0x30;
            tmp = slotOffset + *(int*)((char*)self + 0x28);
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(pos,
                *(int*)((char*)self + 0x24), tmp + 0x58,
                *(int*)((char*)self + 0x2c), tmp + 0x75);
            EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4), rect, pos, 0);
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x167, 0x173, 0x1de, 0x192);
            temp = *(int*)((char*)self + 0x28) + slotOffset;
            tmp = temp + 0x94;
            temp = temp + 0x76;
            value = *(int*)((char*)self + 0x2c);
        }
        EQ_GLOBAL(SetRectProc_guess, 0x00534248)(pos,
            *(int*)((char*)self + 0x24), temp, value, tmp);
        EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4), rect, pos, 0);
        if (*(short*)((char*)self + 0x94) == 10)
        {
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37C0))((char*)self + 0x150, 0);
        }
        if (*(short*)((char*)self + 0x94) == 0xb)
        {
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect, 0x168, 0x1b1, 0x1b6, 0x1c3);
            EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4), rect,
                 (char*)self + 0x164, 0);
        }
        if (*(int*)(EQ_GLOBAL(u32, 0x006E4E44) + 0x1010) == 0 ||
            *(int*)(EQ_GLOBAL(u32, 0x006E4E44) + 0x1010) != *(int*)((char*)self + 0x174))
        {
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect,
                *(int*)((char*)self + 0x24) + 0x27,
                *(int*)((char*)self + 0x28) + 0x16, 0x280, 0x1e0);
            tmp = *(short*)(*(int*)((char*)self + 0x174) + 0x84);
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_ARRAY(void*, 0x006B32B4)[tmp])(rect, 0);
        }
        else
        {
            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect,
                *(int*)((char*)self + 0x24) + 0x27,
                *(int*)((char*)self + 0x28) + 0x16, 0x280, 0x1e0);
            tmp = *(short*)(*(int*)(EQ_GLOBAL(u32, 0x006E4E44) + 0x1010) + 0x84);
            value = EQ_ARRAY(int, 0x006B32B4)[tmp];
            if (value == 0)
                value = EQ_GLOBAL(int, 0x006B3E00);
            EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), (void*)value)(rect, 0);
        }
        tmp = *(int*)((char*)self + 0x174);
        index = 0;
        if (*(char*)(tmp + 0xd5) != '\0')
        {
            do
            {
                value = (short)index;
                slotOffset = value * 4 + 0xac;
                if (*(int*)(slotOffset + tmp) != 0)
                {
                    tmp = value * 0x10 + (int)self;
                    EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect,
                        *(int*)((char*)self + (value + 0xb) * 0x10) + 3,
                        *(int*)(tmp + 0xb4) + 3, 0x280, 0x1e0);
                    temp = *(int*)(slotOffset + *(int*)((char*)self + 0x174));
                    value = *(short*)(temp + 0x84);
                    if (value < 0x1f4 || 0xbba < value)
                    {
                        value = EQ_GLOBAL(int, 0x006B3E30);
                    }
                    else
                    {
                        value = EQ_ARRAY(int, 0x006B32B4)[value];
                    }
                    EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), (void*)value)(rect, 0);
                    value = EQ_CDECL(int, 0x004A1D83, (void*))
                        ((void*)*(int*)(slotOffset + *(int*)((char*)self + 0x174)));
                    if (value != 0)
                    {
                        EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect,
                            *(int*)(tmp + 0xb8) - 0x12,
                            *(int*)(tmp + 0xbc) - 0xe,
                            *(int*)(tmp + 0xb8), *(int*)(tmp + 0xbc));
                        EQ_METHOD(2, void, 0x004384B2, (void*, void*, int), EQ_GLOBAL(void*, 0x006B37FC))(rect, 0);
                        value = *(int*)(slotOffset + *(int*)((char*)self + 0x174));
                        EQ_CDECL(int, 0x005224FB, (char*, const char*, ...))
                            (text, EQ_ARRAY(char, 0x0053E16C),
                             ((int)(*(unsigned char*)(value + 0xda) - 1) /
                                (int)*(char*)(value + 0xbb)) + 1);
                        EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(text,
                             *(int*)(tmp + 0xb8) - 0xd,
                             *(int*)(tmp + 0xbc) - 9,
                             10, EQ_GLOBAL(int, 0x006EFDF4), 1);
                    }
                }
                tmp = *(int*)((char*)self + 0x174);
                index = index + 1;
            } while ((short)index < (short)(unsigned short)*(unsigned char*)(tmp + 0xd5));
        }
        if (pinstEQ_Main_guess->merchantOpen_guess != 0 &&
            *(int*)(EQ_GLOBAL(u32, 0x006E4E44) + 0x6c658) != 0)
        {
            tmp = *(int*)((char*)self + 0x174);
            index = 0;
            value = EQ_GLOBAL(int, 0x006E4E44);
            if (*(char*)(tmp + 0xd5) != '\0')
            {
                do
                {
                    if (**(int**)(value + 0x6c658) != 0)
                    {
                        temp = (short)index;
                        if (**(int**)(value + 0x6c658) == *(int*)(tmp + 0xac + temp * 4))
                        {
                            if (*(int*)((char*)self + 8) == 0)
                                *(unsigned char*)((char*)self + 0x8c) =
                                    *(unsigned char*)((char*)self + 0x8c) ^ 1;
                            if (*(char*)((char*)self + 0x8c) == '\0')
                            {
                                value = 0xe8;
                                tmp = 0xb9;
                            }
                            else
                            {
                                value = 0xb9;
                                tmp = 0x8a;
                            }
                            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(rect,
                                0x167, tmp, 0x196, value);
                            EQ_GLOBAL(SetRectProc_guess, 0x00534248)(pos,
                                *(int*)((char*)self + (temp + 0xb) * 0x10),
                                *(int*)((char*)self + temp * 0x10 + 0xb4), 0x280, 0x1e0);
                            EQ_METHOD(4, void, 0x00450125, (void*, void*, void*, void*, int), self)(*(void**)(EQ_GLOBAL(u32, 0x006E4E24) + 4),
                                 rect, pos, 0);
                            value = EQ_GLOBAL(int, 0x006E4E44);
                        }
                    }
                    tmp = *(int*)((char*)self + 0x174);
                    index = index + 1;
                } while ((short)index < (short)(unsigned short)*(unsigned char*)(tmp + 0xd5));
            }
        }
        textWidth = 0;
        nameLen = EQ_CDECL(size_t, 0x005223D0, (const char*))((char*)*(int*)((char*)self + 0x174));
        if (0 < (short)nameLen)
        {
            namePtr = (char*)*(int*)((char*)self + 0x174);
            tmp = (int)(short)nameLen;
            do
            {
                if ('\0' < *namePtr)
                {
                    textWidth = textWidth + *(short*)(EQ_GLOBAL(short*, 0x006EFDDC) + *namePtr * 2);
                }
                namePtr = namePtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))((const char*)*(int*)((char*)self + 0x174),
             (*(int*)((char*)self + 0x24) - (int)(textWidth / 2)) + 0x3b,
             *(int*)((char*)self + 0x28) + 7, 10, EQ_GLOBAL(int, 0x006EFDF8), 1);
    }
    return;
}
