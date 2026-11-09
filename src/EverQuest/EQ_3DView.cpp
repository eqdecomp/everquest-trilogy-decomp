// EQ_3DView.cpp - EQ_Overlay-derived view destructor.
// Field layout: unrecovered; the vtable pointer at +0 is written raw.

#include "trilogy/common.h"
#include "trilogy/character.h"
#include "trilogy/eq_overlay.h"

class EQ_3DViewOverlayBase_guess
{
public:
    // EXTERNAL: TRILOGY 0x00450049 EQ_3DViewOverlayBase_guess::EQ_3DViewOverlayBase_guess
    EQ_3DViewOverlayBase_guess();
    // EXTERNAL: TRILOGY 0x0045005D EQ_3DViewOverlayBase_guess::~EQ_3DViewOverlayBase_guess
    ~EQ_3DViewOverlayBase_guess();
};

class EQ_3DView : public EQ_3DViewOverlayBase_guess
{
public:
    EQ_3DView();
    ~EQ_3DView();
    // EXTERNAL: TRILOGY 0x00450463 EQ_3DView::DestroyButtons_guess
    void DestroyButtons_guess();
    void UpdateRects(void);
    void DisplaySpells_guess(void);
    void HandleBuffClick_guess(short param_2, int param_3);
    bool MouseDown(int param_1, int param_2, int param_3);
};

// FUNCTION: TRILOGY 0x0040317d
// Routes incoming 3D-view mouse events to the appropriate overlay button handler.
// @early-stop: the original keeps 0 in EBX (xor ebx,ebx before the switch compare; push ebx; mov byte [6b648a],bl), this build uses immediates; EQ_THISCALL adds xor edx,edx per call
void __fastcall DispatchMouseEvent_guess(void* this_guess, int edx_unused, int* event)
{
    EQ_GLOBAL(int, 0x006EFC80) = (int)(*((float*)((char*)event + 0x4)));
    EQ_GLOBAL(int, 0x006EFC84) = (int)(*((float*)((char*)event + 0x8)));
    switch (*event)
    {
    case 0:
        EQ_METHOD(3, bool, 0x00415EE1, (EQ_3DView*, int, int, int), EQ_GLOBAL(EQ_3DView*, 0x006E4DEC))(0,
            (int)(*((float*)((char*)event + 0x4))), (int)(*((float*)((char*)event + 0x8))));
        break;
    case 3:
        EQ_METHOD(3, bool, 0x00415EE1, (EQ_3DView*, int, int, int), EQ_GLOBAL(EQ_3DView*, 0x006E4DEC))(2,
            (int)(*((float*)((char*)event + 0x4))), (int)(*((float*)((char*)event + 0x8))));
        break;
    case 4:
        EQ_GLOBAL(char, 0x006B648A) = 1;
        EQ_METHOD(3, bool, 0x00415EE1, (EQ_3DView*, int, int, int), EQ_GLOBAL(EQ_3DView*, 0x006E4DEC))(1,
            (int)(*((float*)((char*)event + 0x4))), (int)(*((float*)((char*)event + 0x8))));
        break;
    case 5:
        EQ_GLOBAL(char, 0x006B648A) = 0;
        // fall through
    case 1:
        EQ_METHOD(3, void, 0x00415EFB, (EQ_3DView*, int, int, int), EQ_GLOBAL(EQ_3DView*, 0x006E4DEC))(0,
            (int)(*((float*)((char*)event + 0x4))), (int)(*((float*)((char*)event + 0x8))));
        return;
    case 7:
        EQ_METHOD(3, bool, 0x00415EE1, (EQ_3DView*, int, int, int), EQ_GLOBAL(EQ_3DView*, 0x006E4DEC))(3,
            (int)(*((float*)((char*)event + 0x4))), (int)(*((float*)((char*)event + 0x8))));
        break;
    }
}

// FUNCTION: TRILOGY 0x0041551e
// Sets up the 3D view overlay and loads its graphics assets.
// ORIGINAL FILE: EverQuest/EQ_3DView.cpp (from Mac debug map)
EQ_3DView::EQ_3DView()
{
    int remainingCount;
    unsigned int* clearPtr;

    *(unsigned int*)this = 0x00535258;
    *(unsigned int*)((char*)this + 0xc0) = 0;
    clearPtr = (unsigned int*)((char*)this + 0x80);
    for (remainingCount = 0x10; remainingCount != 0; remainingCount = remainingCount + -1)
    {
        *clearPtr = 0;
        clearPtr = clearPtr + 1;
    }
    *(short*)((char*)this + 0xdc) = -1;
    *(short*)((char*)this + 0xde) = -1;
    *(unsigned int*)((char*)this + 0x10) = 1;
    *(unsigned int*)((char*)this + 0x70) = 0;
    *(unsigned int*)((char*)this + 0xc8) = 0;
    *(unsigned int*)((char*)this + 0x78) = 0;
    *(unsigned int*)((char*)this + 0xe0) = 0;
    *(unsigned int*)((char*)this + 0x464) = 0;
    *(unsigned int*)((char*)this + 0x0c) = 0;
    EQ_METHOD(0, void, 0x004155FE, (EQ_3DView*), this)();
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x34, (const char*)0x0053e950);
    EQ_CDECL(char*, 0x005219C0, (char*, const char*))
        ((char*)this + 0x52, (const char*)0x0053e944);
    EQ_METHOD(2, void, 0x00450168, (EQ_3DView*, int, int), this)(0, 0);
}

// FUNCTION: TRILOGY 0x004155C6
// Releases the view's overlay buttons and display overlay.
EQ_3DView::~EQ_3DView()
{
    *(u32*)this = 0x00535258;
    DestroyButtons_guess();
}

// FUNCTION: TRILOGY 0x004155FE
// Builds the 3D view's clickable regions for the current layout.
// ORIGINAL FILE: EverQuest/EQ_3DView.cpp (from Mac debug map)
// @early-stop: frame size and register/loop layout still differ from the original.
void EQ_3DView::UpdateRects(void)
{
    typedef BOOL (__stdcall *SetRectFunction_guess)(LPRECT, int, int, int, int);
    int top;
    int value;
    LPRECT lprc;
    LPRECT temp;
    LPRECT entry;

    EQ_GLOBAL(SetRectFunction_guess, 0x00534248)((LPRECT)((char*)this + 0x24),
        (int)EQ_GLOBAL(short, 0x006B63E0), (int)EQ_GLOBAL(short, 0x006B63E2),
        (int)EQ_GLOBAL(short, 0x006B63E4), (int)EQ_GLOBAL(short, 0x006B63E6));
    EQ_GLOBAL(SetRectFunction_guess, 0x00534248)((LPRECT)((char*)this + 0x14),
        (int)EQ_GLOBAL(short, 0x006B63E0), (int)EQ_GLOBAL(short, 0x006B63E2),
        (int)EQ_GLOBAL(short, 0x006B63E4), (int)EQ_GLOBAL(short, 0x006B63E6));
    EQ_METHOD(0, void, 0x00450463, (EQ_3DView*), this)();
    top = 0;
    *(int*)((char*)this + 0x70) = 0;
    value = EQ_GLOBAL(int, 0x006E4E44);
    if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
    {
        int viewHeight;
        int cellsPerColumn;
        int count;
        int cellTop;
        int lastSlotId;
        int column;
        int columnLeft;
        LPRECT columnRects;
        int* columnFlags;
        int columnFirstSlot;
        int row;
        LPRECT slotId;

        *(int*)(value + 0x90174) = 0;
        top = EQ_GLOBAL(int, 0x006E4E44);
        value = *(int*)(value + 0x90164);
        *(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
        top = *(int*)(top + 0x90168);
        cellTop = *(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x9016C);
        *(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
        viewHeight = EQ_GLOBAL(int, 0x006E4E44);
        *(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
        viewHeight = *(int*)(viewHeight + 0x90170);
        cellsPerColumn = (viewHeight - 4) / 0x28;
        count = (cellTop - 4) / 0x28;
        lastSlotId = 0;
        column = 0;
        if (0 < count)
        {
            columnFlags = (int*)((char*)this + 0x80);
            columnFirstSlot = 0;
            columnRects = (LPRECT)((char*)this + 0x164);
            columnLeft = cellTop - 0x2A + value;
            do
            {
                row = 0;
                if (0 < cellsPerColumn)
                {
                    slotId = (LPRECT)columnFirstSlot;
                    cellTop = top + 2;
                    entry = columnRects;
                    temp = (LPRECT)columnFlags;
                    do
                    {
                        lastSlotId = (int)slotId;
                        // slots past 0xF are skipped; the outer check below exits
                        if ((0xF >= lastSlotId) && (temp->left != 0) && (columnLeft > value) &&
                            (cellTop < viewHeight - 0x28 + top))
                        {
                            EQ_GLOBAL(SetRectFunction_guess, 0x00534248)(
                                entry, columnLeft, cellTop,
                                columnLeft + 0x28, cellTop + 0x28);
                            EQ_METHOD(6, void, 0x004503BE, (EQ_3DView*, int, LPRECT, int, int, int, int), this)((int)slotId, entry,
                                    0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
                        }
                        row = row + 1;
                        temp = (LPRECT)((char*)temp + 4);
                        entry = entry + 1;
                        slotId = (LPRECT)((int)slotId + 1);
                        cellTop = cellTop + 0x28;
                    } while (row < cellsPerColumn);
                }
                if (0xF < lastSlotId)
                    break;
                column = column + 1;
                columnFirstSlot = columnFirstSlot + cellsPerColumn;
                columnRects = columnRects + cellsPerColumn;
                columnLeft = columnLeft - 0x28;
                columnFlags = columnFlags + cellsPerColumn;
            } while (column < count);
        }
        count = count * cellsPerColumn;
        if (count < 0x10)
        {
            value = 0x10 - count;
            lprc = (LPRECT)((char*)this + count * 0x10 + 0x164);
            do
            {
                EQ_GLOBAL(SetRectFunction_guess, 0x00534248)(lprc, -1, -1, -1, -1);
                lprc = lprc + 1;
                value = value - 1;
            } while (value != 0);
        }
    }
    else
    {
        int* flagPtr;
        lprc = (LPRECT)((char*)this + 0x164);
        entry = (LPRECT)0;
        flagPtr = (int*)((char*)this + 0x80);
        temp = (LPRECT)0;
        do
        {
            if (*flagPtr != 0)
            {
                EQ_GLOBAL(SetRectFunction_guess, 0x00534248)(
                    lprc, 0x1DF, (int)temp, 0x207, (int)temp + 0x27);
                EQ_METHOD(6, void, 0x004503BE, (EQ_3DView*, int, LPRECT, int, int, int, int), this)((int)entry, lprc,
                        0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
            }
            temp = (LPRECT)((int)temp + 0x28);
            entry = (LPRECT)((char*)entry + 1);
            flagPtr = flagPtr + 1;
            lprc = lprc + 1;
        } while ((int)temp < 0x140);
        lprc = (LPRECT)((char*)this + 0x1E4);
        entry = (LPRECT)0;
        flagPtr = (int*)((char*)this + 0xA0);
        do
        {
            if (*flagPtr != 0)
            {
                EQ_GLOBAL(SetRectFunction_guess, 0x00534248)(
                    lprc, 0x1B7, top, 0x1DF, top + 0x27);
                EQ_METHOD(6, void, 0x004503BE, (EQ_3DView*, int, LPRECT, int, int, int, int), this)((int)entry + 8, lprc,
                        0xFFFFFFFF, 0xFFFFFFFF, 0, 0);
            }
            entry = (LPRECT)((char*)entry + 1);
            flagPtr = flagPtr + 1;
            lprc = lprc + 1;
            top = top + 0x28;
        } while (top < 0x140);
    }
    EQ_GLOBAL(SetRectFunction_guess, 0x00534248)(
        (LPRECT)((char*)this + 0xCC), 0x77, 0, 0xF0, 0x38);
    return;
}

// FUNCTION: TRILOGY 0x004158dc
// Draws the player's buff icons (blinking when nearly out) plus two meters, likely breath and casting.
// @early-stop: trueFlag (1) is folded into immediates instead of held in EDX, so the loop counters and pointers land in other registers (EDX/ECX, ECX/EAX, EBX/EBP); overlay record pointer not cached (add eax,0x90164); xor edx,edx dummy per EQ_THISCALL
// ORIGINAL FILE: EverQuest/EQ_3DView.cpp (from Mac debug map)
/* fake match: the three gotos (secondBar, drawBar, drawIcon) keep the original's block layout; a goto-free if/else tail makes VC6 use an EBP frame and spill bVar3 */
void EQ_3DView::DisplaySpells_guess(void)
{
    short shownFill;
    int viewLeft;
    BOOL bVar3;
    BOOL trueFlag;
    int obj;
    u8 maxValue;
    char result;
    short barWidth;
    u32 now;
    int tmp;
    int* ptr;
    int spell;
    u32 temp;
    u32* flagPtr;
    RECT drawRect;

    typedef BOOL (__stdcall *CopyRectFunction_guess)(LPRECT, const RECT*);
    typedef BOOL (__stdcall *SetRectFunction_guess)(LPRECT, int, int, int, int);
    CopyRectFunction_guess copyRectFn;
    SetRectFunction_guess setRectFn;

    maxValue = EQ_THISCALL(u8, 0x0041BA4E, (CharacterProfile*, int))(
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), 0);
    now = EQ_CDECL(u32, 0x004A461E, (void))();
    bVar3 = 0;
    if ((EQ_GLOBAL(u8, 0x00591828) & 1) == 0)
    {
        EQ_GLOBAL(u32, 0x00591814) = (u32)EQ_GLOBAL(u8, 0x006B658C);
        EQ_GLOBAL(u8, 0x00591828) = EQ_GLOBAL(u8, 0x00591828) | 1;
    }
    tmp = EQ_THISCALL(int, 0x004504AC, (EQ_3DView*, int))(this, 0);
    if (tmp == 0)
        return;
    if (EQ_GLOBAL(char, 0x006B63F8) != '\0')
        return;
    if (EQ_GLOBAL(int, 0x006B6494) != 0)
        return;

    trueFlag = 1;
    if (500 < (u32)(*(int*)((char*)EQ_GLOBAL(void*, 0x006E4DE0) + 0xBC)
        - EQ_GLOBAL(int, 0x00591840)))
    {
        *(int*)((char*)this + 0xC0) = *(int*)((char*)this + 0xC0) ^ trueFlag;
        EQ_GLOBAL(int, 0x00591840) = *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DE0) + 0xBC);
    }
    temp = (u32)EQ_GLOBAL(u8, 0x006B658C);
    if (EQ_GLOBAL(u32, 0x00591814) != temp)
    {
        bVar3 = trueFlag;
        EQ_GLOBAL(u32, 0x00591814) = temp;
        if (EQ_GLOBAL(u8, 0x006B658C) == 0)
        {
            flagPtr = (u32*)((char*)this + 0x80);
            for (tmp = 0x10; tmp != 0; tmp = tmp - 1)
            {
                *flagPtr = 0;
                flagPtr = flagPtr + 1;
            }
        }
    }
    if (((EQ_GLOBAL(int, 0x006E4E44) != 0) && (EQ_GLOBAL(u8, 0x006B658C) != 0)) &&
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != (CharacterProfile*)0))
    {
        ptr = (int*)((char*)this + 0x80);
        tmp = 0;
        do
        {
            if (((u8*)&EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->buffs[0].visible)[tmp] != '\0')
            {
                if ((EQ_ARRAY(int, 0x005FE9A0)[*(short*)((char*)&EQ_GLOBAL(
                    CharacterProfile*, 0x006E4DA8)->buffs[0].spell_id + tmp)] != 0) &&
                    (*ptr == 0))
                {
                    *ptr = trueFlag;
                    bVar3 = trueFlag;
                }
            }
            else
            {
                if (*ptr != 0)
                {
                    *ptr = 0;
                    bVar3 = trueFlag;
                }
            }
            tmp = tmp + 10;
            ptr = ptr + 1;
        } while (tmp < 0x96);
    }
    tmp = EQ_GLOBAL(int, 0x006E4E44);
    if ((bVar3) || ((EQ_GLOBAL(int, 0x0058FC6C) == 2 &&
        (*(u32*)(tmp + 0x90174) = 0, *(int*)(tmp + 0x90178) != 0))))
    {
        EQ_THISCALL(void, 0x004155FE, (EQ_3DView*, int))(this, 0);
    }
    if ((((pinstEQ_Main_guess->screenMode_guess < '\x04') ||
        ((EQ_GLOBAL(int, 0x0058FC6C) == 2 &&
        (pinstEQ_Main_guess->screenMode_guess == '\x04')))) &&
        (EQ_GLOBAL(int, 0x006E4E44) != 0)) && (EQ_GLOBAL(u8, 0x006B658C) != 0))
    {
        tmp = 0;
        ptr = (int*)((char*)this + 0x164);
        do
        {
            if ((((u8*)&EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->buffs[0].visible)[tmp] != '\0') &&
                (spell = EQ_ARRAY(int, 0x005FE9A0)[*(short*)((char*)&EQ_GLOBAL(
                    CharacterProfile*, 0x006E4DA8)->buffs[0].spell_id + tmp)], spell != 0) &&
                ((*(char*)(spell + 0x21F) == '\0') ||
                ((barWidth = *(short*)((char*)&EQ_GLOBAL(CharacterProfile*,
                    0x006E4DA8)->buffs[0].duration + tmp), 3 < barWidth || barWidth < 0) ||
                (*(int*)((char*)this + 0xC0) != 0) ||
                (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->class_ == '\b'))))
            {
                if (EQ_GLOBAL(int, 0x0058FC6C) == 2)
                {
                    *(u32*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
                    spell = EQ_GLOBAL(int, 0x006E4E44);
                    if (*(int*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90178) != 0)
                    {
                        *(u32*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
                        obj = EQ_GLOBAL(int, 0x006E4E44);
                        viewLeft = *(int*)(spell + 0x90164);
                        *(u32*)(EQ_GLOBAL(int, 0x006E4E44) + 0x90174) = 0;
                        if ((*(int*)(obj + 0x90168) < ptr[1]) && (viewLeft < *ptr))
                        {
                            spell = EQ_ARRAY(int, 0x005FE9A0)[*(short*)((char*)&EQ_GLOBAL(
                                CharacterProfile*, 0x006E4DA8)->buffs[0].spell_id + tmp)];
                            goto drawIcon;
                        }
                    }
                }
                else
                {
drawIcon:
                    EQ_CDECL(void, 0x00438423, (int*, int))(
                        ptr, (int)*(short*)(spell + 0x1FA));
                    EQ_THISCALL(void, 0x004384B2, (void*, int, int*, int))(
                        EQ_GLOBAL(void*, 0x006B59FC), 0, ptr, 1);
                }
            }
            tmp = tmp + 10;
            ptr = ptr + 4;
        } while (tmp < 0x96);
    }
    if (EQ_GLOBAL(u8, 0x0053E940) == 0xFF)
    {
        EQ_GLOBAL(u8, 0x0053E940) = maxValue;
    }
    if (EQ_GLOBAL(u8, 0x0053E940) != maxValue)
    {
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->airSupply_guess =
            (u8)((int)((int)(char)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->airSupply_guess *
            (u32)maxValue) / (int)(u32)EQ_GLOBAL(u8, 0x0053E940));
        EQ_GLOBAL(u8, 0x0053E940) = maxValue;
    }
    result = EQ_THISCALL(char, 0x00425817, (CharacterProfile*, int))(
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), 0);
    copyRectFn = EQ_GLOBAL(CopyRectFunction_guess, 0x00534240);
    setRectFn = EQ_GLOBAL(SetRectFunction_guess, 0x00534248);
    if ((result != '\0') || ((int)(char)EQ_GLOBAL(
        CharacterProfile*, 0x006E4DA8)->airSupply_guess >= (int)(u32)maxValue))
    {
        *(short*)((char*)this + 0xDC) = -1;
        goto secondBar;
    }
    barWidth = (short)(((char)EQ_GLOBAL(CharacterProfile*,
        0x006E4DA8)->airSupply_guess * 0x42) / (int)EQ_THISCALL(u8, 0x0041BA4E,
        (CharacterProfile*, int))(EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), 0));
    if (barWidth < 0)
    {
        barWidth = 0;
    }
    if (0x42 < barWidth)
    {
        barWidth = 0x42;
    }
    shownFill = *(short*)((char*)this + 0xDC);
    if (shownFill == -1)
    {
        *(short*)((char*)this + 0xDC) = barWidth;
    }
    else
    {
        if (shownFill < barWidth)
        {
            barWidth = 1;
        }
        else
        {
            if (shownFill <= barWidth)
                goto drawBar;
            barWidth = -1;
        }
        *(short*)((char*)this + 0xDC) = shownFill + barWidth;
    }
drawBar:
    copyRectFn(&drawRect, (RECT*)((char*)this + 0xCC));
    EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
        EQ_GLOBAL(void*, 0x006B38E0), 0, &drawRect, 1);
    setRectFn(&drawRect, 0x93, 0x14, 0xD4, 0x19);
    EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
        EQ_GLOBAL(void*, 0x006B3610), 0, &drawRect, 1);
    *EQ_GLOBAL(u32*, 0x006B341C) = *(u32*)(EQ_GLOBAL(u32, 0x006E4E40) + 4);
    setRectFn((LPRECT)(EQ_GLOBAL(u32*, 0x006B341C) + 1), 0x1B, 0x152,
        *(short*)((char*)this + 0xDC) + 0x1C, 0x157);
    EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
        EQ_GLOBAL(void*, 0x006B341C), 0, &drawRect, 1);
secondBar:
    if ((EQ_GLOBAL(void*, 0x006E4D8C) == (void*)0) ||
        ((barWidth = *(short*)((char*)*(int*)((char*)EQ_GLOBAL(void*, 0x006E4D8C) + 0x70) +
        0x248), barWidth == -1)))
    {
        tmp = 0;
    }
    else
    {
        tmp = EQ_ARRAY(int, 0x005FE9A0)[barWidth];
    }
    if (((EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != (CharacterProfile*)0) &&
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->class_ != '\b')) &&
        (EQ_GLOBAL(void*, 0x006E4D8C) != (void*)0))
    {
        if (((tmp == 0) || (temp = *(u32*)((char*)*(int*)((char*)EQ_GLOBAL(
            void*, 0x006E4D8C) + 0x70) + 0xC8), now > temp)) ||
            (*(u32*)(tmp + 0x1B0) <= 0))
        {
            *(short*)((char*)this + 0xDE) = -1;
        }
        else
        {
            barWidth = (short)((temp * 0x42 + now * -0x42) /
                *(u32*)(tmp + 0x1B0));
            if (barWidth < 0)
            {
                barWidth = 0;
            }
            if (0x42 < barWidth)
            {
                barWidth = 0x42;
            }
            *(short*)((char*)this + 0xDE) = barWidth;
            copyRectFn(&drawRect, (RECT*)((char*)this + 0xCC));
            EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
                EQ_GLOBAL(void*, 0x006B38E4), 0, &drawRect, 1);
            setRectFn(&drawRect, 0x93, 0x14, 0x280, 0x1E0);
            EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
                EQ_GLOBAL(void*, 0x006B3610), 0, &drawRect, 1);
            *EQ_GLOBAL(u32*, 0x006B341C) = *(u32*)(EQ_GLOBAL(u32, 0x006E4E40) + 4);
            setRectFn((LPRECT)(EQ_GLOBAL(u32*, 0x006B341C) + 1), 0x1B, 0x18B,
                *(short*)((char*)this + 0xDE) + 0x1C, 400);
            EQ_THISCALL(void, 0x004384B2, (void*, int, RECT*, int))(
                EQ_GLOBAL(void*, 0x006B341C), 0, &drawRect, 1);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00415DE5
// Selects a visible buff's spell for display or routes its slot to a buff action.
void EQ_3DView::HandleBuffClick_guess(short action, int buffSlot)
{
    int* valuePtr;
    short spellId;
    int statePtr;

    spellId = (short)buffSlot;
    *(short*)((char*)this + 0xc4) = spellId;
    *(short*)((char*)this + 0xc6) = action;
    statePtr = EQ_GLOBAL(int, 0x006E4E44);
    if ((((statePtr != 0) && (EQ_GLOBAL(char, 0x006B658C) != '\0')) &&
        (*(char*)(statePtr + 0xfe0) != '\x05')) &&
        ((EQ_GLOBAL(int, 0x0058FC6C) != 2 ||
        (valuePtr = (int*)(statePtr + 0x90178), *(int*)(statePtr + 0x90174) = 0,
            *valuePtr != 0))))
    {
        if (action == 1)
        {
            CharacterProfile* profile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
            if ((((profile->buffs[spellId].visible != '\0') &&
                (spellId = profile->buffs[spellId].spell_id, -1 < spellId)) &&
                (spellId < 3000)) &&
                (EQ_ARRAY(int, 0x005FE9A0)[spellId] != 0))
            {
                *(short*)((char*)EQ_GLOBAL(void*, 0x006E4D84) + 0x94) = spellId + 10000;
                typedef void (__fastcall *DisplayCallback_t)(void*, int);
                void* display = EQ_GLOBAL(void*, 0x006E4D84);
                DisplayCallback_t callback = *(DisplayCallback_t*)(*(char**)display + 4);
                callback(display, 0);
            }
        }
        else if (((action == 0) &&
            (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->buffs[spellId].visible != '\0')) &&
            ((spellId = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->buffs[spellId].spell_id,
            -1 < spellId && ((spellId < 3000 &&
            (EQ_ARRAY(int, 0x005FE9A0)[spellId] != 0))))))
        {
            EQ_METHOD(1, void, 0x00480954, (CharacterProfile*, int), EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(buffSlot);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00415ee1
// Thunk that forwards a 3D-view mouse action to its overlay handler.
// ORIGINAL FILE: EverQuest/EQ_3DView.cpp (from Mac debug map)
bool EQ_3DView::MouseDown(int arg, int mouseX, int mouseY)
{
    int result;

    result = EQ_METHOD(3, int, 0x004501D9, (EQ_3DView*, int, int, int), this)(arg, mouseX, mouseY);
    return result != 0;
}
