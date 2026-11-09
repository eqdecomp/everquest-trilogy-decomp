// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EverQuest.cpp - CEverQuest methods and the free functions of the EverQuest/EverQuest subsystem.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <time.h>

#include "trilogy/common.h"
#include "trilogy/layout/eq_sound_manager.h"
#include "trilogy/layout/effect_data_row.h"
#include "trilogy/character.h"
#include "trilogy/player.h"
#include "trilogy/net_structs.h"
#include "trilogy/display.h"
#define GetTPCoordsFromIndex GetTPCoordsFromIndex_guess_header
#include "trilogy/everquest.h"
#undef GetTPCoordsFromIndex
#include "trilogy/globals.h"
#include "trilogy/zone.h"
#include "trilogy/sound.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/layout/eq_requestor.h"
#include "trilogy/layout/eq_charcreate.h"
#include "trilogy/eq_textio.h"
#include "trilogy/layout/eq_bag.h"
#include "trilogy/layout/eq_effect.h"
#include "trilogy/spell.h"

// The debug log (0x004C1703), defined below.
int __cdecl DbgLog_guess(const char* formatString, ...);
#undef send_message_guess
#undef EQTRACE_guess
// EXTERNAL: TRILOGY 0x004E1C45 send_message_guess
void __cdecl send_message_guess(void** connection, u32 opcode, void* data, u32 size, int reliable);
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
void __cdecl EQTRACE_guess(const char* format, ...);

// *g_reverb_on_guess: reverb switch in the sound manager (unconfirmed).
// 1 = left and right sound channels are reversed.
#define g_reverb_on_guess           EQ_GLOBAL(char *, 0x006E4DD8)
#define g_channels_reversed_guess   EQ_GLOBAL(int, 0x006B6F08)

// FUNCTION: TRILOGY 0x00405b18
// Initializes the game client display and its default session settings.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall with one stack argument (callee pops 4), returns `this`. Resets the option and display state, fills the three
// 74-entry flag tables at +0x2cf9, +0x2d43 and +0x2d8d, then sets up the display: 0x0048d5d9, t3dInitializeLibrary,
// the two reference lists in a LocalAlloc block and the default cameras.
CEverQuest* CEverQuest::InitDisplay_guess(int arg)
{
    typedef HLOCAL (__stdcall *LocalAlloc_guess_t)(UINT uFlags, UINT uBytes);
    typedef void (__cdecl *InitReferenceList_guess_t)(void* list, int count);
    typedef void (__cdecl *InitializeLibrary_guess_t)(void);

    int i;
    HLOCAL refBlock;
    InitReferenceList_guess_t initReferenceList;

    *(int*)((char*)this + 0xbc) = 0;
    *((char*)this + 0xb8) = 0;
    *(int*)((char*)this + 0x14) = 0;
    EQ_CDECL(void, 0x0040BCBB, (void))();
    for (i = 0; i < 0x4a; i++)
    {
        if ((i <= 0x23
                && !(i < 0x22 && i != 0 && (i <= 0x10 || (i > 0x12 && (i <= 0x14 || i > 0x20)))))
            || (i > 0x23 && i >= 0x25 && (i <= 0x26 || (i > 0x2f && i <= 0x49))))
            *((char*)this + 0x2cf9 + i) = 1;
        else
            *((char*)this + 0x2cf9 + i) = 0;
        if (i < 2 || (i > 9 && (i <= 0x26 || i > 0x2c)))
            *((char*)this + 0x2d43 + i) = 0;
        else
            *((char*)this + 0x2d43 + i) = 1;
        if (i < 0x11 || (i > 0x12 && i != 0x20))
            *((char*)this + 0x2d8d + i) = 0;
        else
            *((char*)this + 0x2d8d + i) = 1;
    }
    EQ_METHOD(0, void, 0x0048D5D9, (CEverQuest *), this)();
    *((char*)this + 0x11) = 1;
    EQ_GLOBAL(u8, 0x006E11E1) = 1;
    *(int*)((char*)this + 0x124) = 0;
    *(int*)((char*)this + 0x128) = 0;
    *(int*)((char*)this + 0x12c) = 0;
    *(int*)((char*)this + 0x80) = 0;
    *(int*)((char*)this + 0x84) = 0;
    *(int*)((char*)this + 0x88) = 0;
    *(int*)((char*)this + 0x8c) = 0;
    *(int*)((char*)this + 0x90) = 0;
    *(int*)((char*)this + 0x94) = 0;
    EQ_GLOBAL(u8, 0x0058FBB8) = 0;
    *(int*)((char*)this + 0x2c94) = 0;
    *(int*)((char*)this + 0x2c98) = 0;
    *((char*)this + 0x2cf8) = 0;
    *((char*)this + 0x3c) = 0;
    *((char*)this + 0x3d) = 0;
    *((char*)this + 0x3e) = 0;
    *(int*)((char*)this + 0x2cac) = 0;
    *(int*)((char*)this + 0x287c) = arg;
    *(int*)((char*)this + 0xc) = 0;
    DbgLog_guess(EQ_ARRAY(char, 0x0053D7B4));
    EQ_GLOBAL(InitializeLibrary_guess_t, 0x006E5320)();
    DbgLog_guess(EQ_ARRAY(char, 0x0053D79C));
    *(float*)((char*)this + 0x2880) = 110.0f;
    *(float*)((char*)this + 0x2884) = 1.25f;
    *(float*)((char*)this + 0x18) = 600.0f;
    *(float*)((char*)this + 0x288c) = 1.0f;
    *(int*)((char*)this + 0x2888) = 0;
    EQ_GLOBAL(u32, 0x00590160) = 0;
    *(int*)((char*)this + 0x2c90) = 0;
    *(int*)this = 0;
    this->gameState_guess = 0;
    *(int*)((char*)this + 8) = 0;
    *(int*)((char*)this + 0x2cb4) = 0;
    *(int*)((char*)this + 0x2844) = 0;
    memset((char*)this + 0x130, 0, 10000);
    *((char*)this + 0x2cce) = 0xff;
    *((char*)this + 0x2ccc) = 0xff;
    *(short*)((char*)this + 0x2840) = 0;
    *(int*)((char*)this + 0x2848) = 0;
    *(int*)((char*)this + 0x284c) = 0;
    *(int*)((char*)this + 0x2850) = 0;
    *(int*)((char*)this + 0x2854) = 0;
    *(int*)((char*)this + 0x2858) = 0;
    *(int*)((char*)this + 0x285c) = 0;
    *(int*)((char*)this + 0x2860) = 0;
    *(int*)((char*)this + 0x2dd8) = 0;
    *(int*)((char*)this + 0x2ddc) = 0;
    *(int*)((char*)this + 0x2de0) = 0;
    *(int*)((char*)this + 0x120) = 0;
    *(int*)((char*)this + 0x2de4) = 0;
    *(float*)((char*)this + 0x2870) = 4.0f;
    *(float*)((char*)this + 0x2874) = 2.0f;
    *(float*)((char*)this + 0x2878) = 3.0f;
    *((char*)this + 0x2ccd) = 0;
    *((char*)this + 0x2ccf) = 0;
    refBlock = EQ_GLOBAL(LocalAlloc_guess_t, 0x00534180)(0x40, 0x10);
    initReferenceList = EQ_GLOBAL(InitReferenceList_guess_t, 0x006E5324);
    ((HLOCAL*)this)[0xb2e] = refBlock;
    initReferenceList(refBlock, 0x14);
    initReferenceList((char*)this + 0x40, 0x32);
    initReferenceList((int*)this + 0xb2f, 0x32);
    EQ_METHOD(0, void, 0x00408D5B, (CEverQuest *), this)();
    *((char*)this + 0x12) = 1;
    *((char*)this + 0x10) = 1;
    EQ_GLOBAL(float, 0x006B6858) = 0.0f;
    EQ_GLOBAL(u8, 0x006E11E0) = 0;
    EQ_GLOBAL(u8, 0x006B6405) = 0;
    EQ_GLOBAL(u8, 0x006B6407) = 0;
    *((char*)this + 0x2dee) = 100;
    *((char*)this + 0x2ded) = 0;
    *((char*)this + 0x2dec) = 0;
    return this;
}

// An interface seen through its vtable: slots 1 (+0x04) and 2 (+0x08), no arguments.
class EQBagVtbl_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
};

// The __thiscall methods the bag handler calls, with the EDX dummy folded in (see eqaddr.h).
#define BagDspChat_guess(text, color) \
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), pinstCEverQuest_guess)((text), (color))
#define BagPrintChat_guess(text) \
    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)((text))
#define BagTrimName_guess(name) \
    EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))((name))
#define BagValueSSell_guess(item, rate, count) \
    EQ_METHOD(2, char*, 0x004385D5, (void*, float, int), (item))((rate), (count))
#define BagWavePlay_guess(id, flag) \
    EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), EQ_GLOBAL(void*, 0x006E4DD8))((id), (flag))
#define BagMoveItem_guess(a1, a2, a3, a4) \
    EQ_METHOD(4, void, 0x00463651, (void*, int, int, int, int), g_eqmain_guess)((a1), (a2), (a3), (a4))
#define BagActivate_guess(slot, value) \
    EQ_METHOD(2, void, 0x00486CB6, (void*, int, int), EQ_GLOBAL(void*, 0x006018C0))((slot), (value))
#define BagUseItem_guess(uniqueId) \
    EQ_METHOD(1, void, 0x004DBD8B, (CEverQuest*, u32), pinstCEverQuest_guess)((uniqueId))

// FUNCTION: TRILOGY 0x0041800a
// Open-bag click handler: read notes, apply poison, gated tradeskill combine, move items, merchant price quotes
// ORIGINAL FILE: unknown (subsystem from the call graph; slot 0 of the bag window's vtable at 0x00535264)
// @early-stop: rewrite is 5 instructions longer; register allocation and dummy EDX setup still differ in the item-use and move tails.
// __thiscall on the bag window (EQ_Bag), two short stack arguments (callee pops 8), no result: `mouseButton` is the
// mouse button (0 = left, 1 = right) and `controlId` the control under the cursor (0..9 the bag's slots, 10 the
// close button, 11 the combine button, 12 the title bar). +0x174 is the container item, +0xAC cleared on
// entry, +0xA4 the "hot slot" mode, +0xA8 the drag flag, +0x90 / +0x8E / +0x94 the last button, control and
// the highlighted control. A right click reads a note (opcode 0x20CE) or uses an item from a merchant window;
// a left click closes, drags, combines (opcode 0x2105) or moves items between the bag and the cursor.
void EQ_Bag_guess::HandleMouse_guess(short mouseButton, short controlId)
{
    char* self = (char*)this;
    int item;
    int isActive;
    size_t noteLen;
    short invSlot;
    char comboType;
    EQ_Item** invPtr;
    short comboSlot;
    short* packetSlot;
    EQ_Item** contentPtr;
    int slotOfs;
    char itemType;
    int slotParam;
    short slotBase;
    int heldBag;
    char itemName[128];
    char priceText[128];
    MsgCombine combineMsg;
    EQ_Item* bag;
    int itemCount;
    int slotIndex;

    *(int*)(self + 0xac) = 0;
    if ((pinstEQ_Main_guess->screenMode_guess == 5 && pinstEQ_Main_guess->scribingSpell_guess != -1)
        || (pinstEQ_Main_guess->screenMode_guess == 5 && pinstEQ_Main_guess->memorizingSpell_guess != -1))
        return;
    *(short*)(self + 0x90) = mouseButton;
    *(short*)(self + 0x8e) = controlId;
    if (*(int*)(self + 0xa4) != 0)
    {
        if (*(int*)(self + 0xa8) == 0)
            *(short*)(self + 0x94) = controlId;
        if (mouseButton != 1)
            return;
        if (EQ_GLOBAL(int, 0x006B6498) == 0)
            return;
        if (controlId < 0)
            return;
        if (controlId >= (short)(u16)*(u8*)(*(int*)(self + 0x174) + 0xd5))
            return;
        item = *(int*)(*(int*)(self + 0x174) + 0xac + controlId * 4);
        if (item == 0)
            return;
        *(int*)(EQ_GLOBAL(char*, 0x006E4D84) + 0x90) = item;
        ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4D84))->vfunc1();
        return;
    }
    *(short*)(self + 0x94) = -1;
    if (mouseButton == 1)
    {
        if (pinstEQ_Main_guess->merchantOpen_guess == 0 && g_game_state_guess == 1
            && controlId < (short)(u16)*(u8*)(*(int*)(self + 0x174) + 0xd5)
            && (item = *(int*)(*(int*)(self + 0x174) + 0xac + controlId * 4)) != 0)
        {
            if (*(char*)(item + 0x81) == 2)
            {
                if (*(int*)(g_eqBook_guess + 0x10) != 0)
                {
                    isActive = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E60))();
                    if (isActive != 0 && *(int*)(g_eqBook_guess + 0x37ac) == item)
                        ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E60))->vfunc2();
                }
                else
                {
                    *(int*)(g_eqBook_guess + 0x37ac) = item;
                    item = *(int*)(g_eqBook_guess + 0x37ac);
                    noteLen = strlen((char*)(item + 0xaf));
                    send_message_guess(g_world_connection_guess, 0x20ce, (char*)(item + 0xaf), noteLen + 1, 1);
                }
            }
            else if (*(char*)(item + 0x81) == 0 && pinstEQ_Main_guess->inspectActive_guess == 0
                     && pinstEQ_Main_guess->lootOpen_guess == 0 && *(char*)(item + 0xc2) == '*')
            {
                invSlot = 0;
                do
                {
                    if (g_pc_guess->inventory_items[invSlot] == *(EQ_Item**)(self + 0x174))
                    {
                        if (invSlot != -1)
                        {
                            BagUseItem_guess(*(u32*)(item + 0xa0));
                            /* fake match: successful item use joins the shared left-click path. */
                            goto leftClick;
                        }
                        break;
                    }
                    invSlot = invSlot + 1;
                } while (invSlot < 0x1e);
            }
        }
    }
    else
    {
leftClick:
        if (mouseButton == 0)
        {
            if (*(int*)(self + 0xa8) != 0)
            {
                *(int*)(self + 0xa8) = 0;
                return;
            }
            if (controlId == 10)
            {
                ((EQBagVtbl_guess*)self)->vfunc2();
                if (EQ_GLOBAL(void*, 0x006018C0) == 0)
                    return;
                if (*(int*)(EQ_GLOBAL(char*, 0x006018C0) + 0x10) == 0)
                    return;
                ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006018C0))->vfunc2();
                return;
            }
            if (controlId == 12)
            {
                if (g_pc_guess->inventory_items[0] != 0)
                    return;
                if ((int)EQ_GLOBAL(short, 0x006B6416) >= *(int*)(self + 0x28) + 0x50)
                    return;
                *(int*)(self + 0xa8) = 1;
                return;
            }
            if (*(int*)(self + 0x9c) != 0)
                return;
            if (*(int*)(self + 0x98) != 0)
                return;
            if (controlId == 11)
            {
                if (*(int*)(self + 0xa0) != 0)
                    return;
                bag = *(EQ_Item**)(self + 0x174);
                comboType = *((char*)bag + 0xd4);
                if ((comboType == 0x18 && g_pc_guess->class_ != 0xc)
                    || (comboType == 0x19 && g_pc_guess->class_ != 0xd)
                    || (comboType == 0x1a && g_pc_guess->class_ != 0xb)
                    || (comboType == 0x1b && g_pc_guess->class_ != 0xe))
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F110), 0xf);
                    return;
                }
                if (comboType == 0x1f && g_pc_guess->race != 5)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F0EC), 0xf);
                    return;
                }
                if (comboType == 0x20 && g_pc_guess->race != 6)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F0C8), 0xf);
                    return;
                }
                if (comboType == 0x21 && g_pc_guess->race != 10)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F0A8), 0xf);
                    return;
                }
                if (comboType == 0x22 && g_pc_guess->race != 8)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F084), 0xf);
                    return;
                }
                if (comboType == 0x23 && g_pc_guess->race != 0xc)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F064), 0xf);
                    return;
                }
                if (comboType == 0x24 && g_pc_guess->race != 2)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F040), 0xf);
                    return;
                }
                if (comboType == 0x26 && g_pc_guess->race != 0x80)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F020), 0xf);
                    return;
                }
                if ((comboType == 0x27 && g_pc_guess->race != 1) || (comboType == 0x28 && g_pc_guess->race != 1))
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053F000), 0xf);
                    return;
                }
                if (comboType == 0x2d && g_pc_guess->race != 0x80)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EFD8), 0xf);
                    return;
                }
                if (comboType == 0x2a && g_pc_guess->race != 3)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EFAC), 0xf);
                    return;
                }
                if (comboType == 0x29 && g_pc_guess->race != 0xb)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EF80), 0xf);
                    return;
                }
                if (comboType == 0x2b && g_pc_guess->race != 4)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EF54), 0xf);
                    return;
                }
                if (comboType == 0x2c && g_pc_guess->race != 4)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EF28), 0xf);
                    return;
                }
                invPtr = g_pc_guess->inventory_items;
                if (g_pc_guess->inventory_items[0] != 0 || *(char*)(g_displayManager_guess + 0x3d) != 0)
                {
                    BagDspChat_guess(EQ_ARRAY(char,0x0053EE40), 0xf);
                    return;
                }
                combineMsg.container_id = bag->item_nr;
                combineMsg.world_object_type = *((char*)bag + 0xd4);
                *(s16*)&combineMsg.unknown0x01[1] = 0;
                combineMsg.container_slot = 0;
                if (pinstEQ_Main_guess->worldContainerItem_guess == 0 || pinstEQ_Main_guess->worldContainerItem_guess != bag)
                {
                    comboSlot = 0;
                    do
                    {
                        if (*invPtr == bag)
                            combineMsg.container_slot = comboSlot;
                        comboSlot = comboSlot + 1;
                        invPtr = invPtr + 1;
                    } while (comboSlot < 0x1e);
                }
                else
                {
                    combineMsg.container_slot = 1000;
                }
                itemCount = 0;
                slotIndex = 0;
                packetSlot = combineMsg.item_in_slot;
                contentPtr = (EQ_Item**)((char*)bag + 0xac);
                do
                {
                    *packetSlot = -1;
                    if ((short)slotIndex < (short)(u16)*(u8*)((char*)bag + 0xd5) && *contentPtr != 0)
                    {
                        itemCount = itemCount + 1;
                        *packetSlot = (*contentPtr)->item_nr;
                    }
                    slotIndex = slotIndex + 1;
                    contentPtr = contentPtr + 1;
                    packetSlot = packetSlot + 1;
                } while ((short)slotIndex < 10);
                combineMsg.unknown0x1A = -1;
                if (combineMsg.container_slot != 0)
                {
                    if ((short)itemCount != 0)
                    {
                        pinstEQ_Main_guess->pendingWorldRequests_guess = pinstEQ_Main_guess->pendingWorldRequests_guess + 1;
                        send_message_guess(g_world_connection_guess, 0x2105, &combineMsg, 0x20, 1);
                    }
                    else
                    {
                        BagPrintChat_guess(EQ_ARRAY(char, 0x0053EEE0));
                    }
                }
            }
        }
    }
    if (pinstEQ_Main_guess->merchantOpen_guess != 0 && mouseButton == 0
        && controlId < (short)(u16)*(u8*)(*(int*)(self + 0x174) + 0xd5)
        && *(int*)((slotOfs = controlId * 4 + 0xac) + *(int*)(self + 0x174)) != 0)
    {
        pinstEQ_Main_guess->uiDirty_guess = 1;
        *(int*)(g_eqmain_guess + 0x6c658) = slotOfs + *(int*)(self + 0x174);
        pinstEQ_Main_guess->selectedItemSlot_guess = controlId + (*(short*)(self + 0x92) + 0xd) * 10;
        strcpy(itemName, *(const char**)(slotOfs + *(int*)(self + 0x174)));
        itemType = *(char*)(*(int*)(slotOfs + *(int*)(self + 0x174)) + 0x81);
        if (itemType == 0)
        {
            if (EQ_CDECL(int, 0x004A1D83, (int))(*(int*)(slotOfs + *(int*)(self + 0x174))) != 0)
            {
                sprintf(priceText, EQ_ARRAY(char, 0x0053F134),
                     BagTrimName_guess((char*)(EQ_GLOBAL(int, 0x006E4DB8) + 1)),
                     BagValueSSell_guess((void*)*(int*)(slotOfs + *(int*)(self + 0x174)),
                                         pinstEQ_Main_guess->merchantRate_guess, 1),
                     itemName);
            }
            else
            {
                sprintf(priceText, EQ_ARRAY(char, 0x0053EE14),
                     BagTrimName_guess((char*)(EQ_GLOBAL(int, 0x006E4DB8) + 1)),
                     BagValueSSell_guess((void*)*(int*)(slotOfs + *(int*)(self + 0x174)),
                                         pinstEQ_Main_guess->merchantRate_guess, 1),
                     itemName);
            }
            BagPrintChat_guess(priceText);
        }
        else if (itemType == 2)
        {
            sprintf(priceText, EQ_ARRAY(char, 0x0053EE14),
                 BagTrimName_guess((char*)(EQ_GLOBAL(int, 0x006E4DB8) + 1)),
                 BagValueSSell_guess((void*)*(int*)(slotOfs + *(int*)(self + 0x174)),
                                     pinstEQ_Main_guess->merchantRate_guess, 1),
                 itemName);
            BagPrintChat_guess(priceText);
        }
    }
    if (pinstEQ_Main_guess->merchantOpen_guess != 0)
        return;
    if (mouseButton != 0)
        return;
    if (controlId < (short)(u16)*(u8*)(*(int*)(self + 0x174) + 0xd5))
    {
        if (*(int*)(self + 0x98) != 0 && pinstEQ_Main_guess->tradeOpen_guess != 0 && EQ_GLOBAL(int, 0x006E4D88) != 0
            && g_pc_guess->inventory_items[0] != 0 && *(char*)(EQ_GLOBAL(int, 0x006E4D88) + 0x98) != 1
            && EQ_CDECL(int, 0x004A0A4C, (EQ_Item*, int))(g_pc_guess->inventory_items[0], 1) == 0)
            return;
        if (pinstEQ_Bag1_guess->moving_guess != 0)
            return;
        if (pinstEQ_Bag2_guess->moving_guess != 0)
            return;
        if (*(char*)(g_displayManager_guess + 0x3d) != 0)
            return;
        if (pinstEQ_Main_guess->cursorSpell_guess != 0)
        {
            pinstEQ_Main_guess->cursorSpell_guess = 0;
            g_cdisplay_guess->itemOnCursor_guess = 0;
            pinstEQ_Main_guess->uiDirty_guess = 1;
        }
        slotBase = (*(short*)(self + 0x92) + 3) * 10;
        if (*(int*)(self + 0xa0) != 0)
            slotBase = slotBase + 2000;
        heldBag = *(int*)(g_eqmain_guess + 0x1010);
        if (heldBag != 0 && heldBag == *(int*)(self + 0x174))
            slotBase = 4000;
        if (g_pc_guess->inventory_items[0] == 0)
        {
            if (heldBag == 0 || heldBag != *(int*)(self + 0x174))
            {
                slotParam = (int)controlId;
                slotOfs = slotParam * 4 + 0xac;
                if (EQ_CDECL(int, 0x004A1D83, (int))(*(int*)(slotOfs + *(int*)(self + 0x174))) != 0)
                {
                    BagActivate_guess(slotOfs + *(int*)(self + 0x174), slotBase + slotParam);
                    return;
                }
                if (g_sound_manager_ptr_guess != 0)
                    BagWavePlay_guess(0x92, 0);
                BagMoveItem_guess(slotBase + slotParam, 0, 1, 1);
            }
            else
            {
                if (g_sound_manager_ptr_guess != 0)
                    BagWavePlay_guess(0x92, 0);
                BagMoveItem_guess((int)slotBase + (int)controlId, 0, 1, 1);
            }
        }
        else
        {
            if (EQ_CDECL(int, 0x004A25FF, (EQ_Item*, int, int))(g_pc_guess->inventory_items[0], *(int*)(self + 0x174), 1) == 0)
                return;
            BagMoveItem_guess(0, (int)slotBase + (int)controlId, 1,
                (*(int*)(g_eqmain_guess + 0x1010) == 0 || *(int*)(g_eqmain_guess + 0x1010) != *(int*)(self + 0x174)) ? 1 : 0);
            if (g_sound_manager_ptr_guess != 0)
                BagWavePlay_guess(0x92, 0);
        }
        if (g_pc_guess->inventory_items[0] == 0)
            g_cdisplay_guess->itemOnCursor_guess = 0;
        else
            g_cdisplay_guess->itemOnCursor_guess = 1;
    }
    else
        return;
}

// FUNCTION: TRILOGY 0x004307fc
// Counts character slots that contain a created character.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name EQ_CharSelS_CountAvailableCharacters, low)
// __thiscall (callers load ECX from 0x006E4E6C), `this` unused, no arguments. Counts the eight character
// select slots (30-byte names at CEverQuest + 0x22400) whose name is not "<none>".
short EQ_CharSelS::CountAvailableCharacters_guess(void)
{
    short count = 0;
    s16 i;

    for (i = 0; i < 8; i++)
    {
        if (strcmp((char*)pinstCEverQuest_guess + 0x22400 + i * 30, "<none>") != 0)
            count++;
    }
    return count;
}

// FUNCTION: TRILOGY 0x00430831
// Hover help for the button under the mouse on the character select and character creation screens
// __fastcall: `charSel` (the character select screen, the object at 0x006E4E6C) comes in ECX; no stack arguments.
// Shows the auto-help for the button under the mouse (x at 0x006B6414, y at 0x006B6416) through
// EQ_Main::DisplayAutoHelpText at y + 0x14. Does nothing unless 0x006B6864 is set and the select screen's +0xA4
// short is at least 3. While the character creation screen (0x006E4E70) is up (+0x10 != 0) its button list (+0x70)
// is searched, else the select screen's (+0x70). A button node has its RECT at +4, id (short) at +0x14 and next at
// +0x20. The first hit decides the text; the list is not searched further.
/* fake match: the 8/9/10 chain is nested with != so VC6 lays Enter and Delete out after Quit; else-if and switch lower differently. */
void __fastcall EQ_CharSelS_ShowAutoHelp_guess(char* charSel)
{
    typedef BOOL (__stdcall *PtInRectProc_guess)(const RECT*, POINT);
    POINT pt;
    char buf[128];
    char* node;
    char* create;
    short buttonId;
    int buttonIndex;

    if (EQ_GLOBAL(int, 0x006B6864) == 0)
        return;
    if (*(short*)(charSel + 0xA4) < 3)
        return;
    pt.x = EQ_GLOBAL(short, 0x006B6414);
    buf[0] = 0;
    create = g_char_create_guess;
    pt.y = EQ_GLOBAL(short, 0x006B6416);
    if (create != 0 && *(int*)(create + 0x10) != 0)
    {
        for (node = *(char**)(create + 0x70); node != 0; node = *(char**)(node + 0x20))
        {
            if (EQ_GLOBAL(PtInRectProc_guess, 0x0053423C)((const RECT*)(node + 4), pt))
            {
                buttonId = *(short*)(node + 0x14);
                if (buttonId < 0)
                    return;
                switch (buttonId)
                {
                case 0x00:
                    strcpy(buf, EQ_ARRAY(char, 0x005458E0));   // Bard class
                    break;
                case 0x01:
                    strcpy(buf, EQ_ARRAY(char, 0x005458D0));   // Cleric class
                    break;
                case 0x02:
                    strcpy(buf, EQ_ARRAY(char, 0x005458C4));   // Druid class
                    break;
                case 0x03:
                    strcpy(buf, EQ_ARRAY(char, 0x005458B4));   // Enchanter class
                    break;
                case 0x04:
                    strcpy(buf, EQ_ARRAY(char, 0x005458A4));   // Magician class
                    break;
                case 0x05:
                    strcpy(buf, EQ_ARRAY(char, 0x00545898));   // Monk class
                    break;
                case 0x06:
                    strcpy(buf, EQ_ARRAY(char, 0x00545884));   // Necromancer class
                    break;
                case 0x07:
                    strcpy(buf, EQ_ARRAY(char, 0x00545874));   // Paladin class
                    break;
                case 0x08:
                    strcpy(buf, EQ_ARRAY(char, 0x00545864));   // Ranger class
                    break;
                case 0x09:
                    strcpy(buf, EQ_ARRAY(char, 0x00545858));   // Rogue class
                    break;
                case 0x0A:
                    strcpy(buf, EQ_ARRAY(char, 0x00545844));   // Shadowknight class
                    break;
                case 0x0B:
                    strcpy(buf, EQ_ARRAY(char, 0x00545834));   // Shaman class
                    break;
                case 0x0C:
                    strcpy(buf, EQ_ARRAY(char, 0x00545824));   // Warrior class
                    break;
                case 0x0D:
                    strcpy(buf, EQ_ARRAY(char, 0x00545814));   // Wizard class
                    break;
                case 0x0E:
                    strcpy(buf, EQ_ARRAY(char, 0x00545800));   // Decrease strength
                    break;
                case 0x0F:
                    strcpy(buf, EQ_ARRAY(char, 0x005457EC));   // Increase strength
                    break;
                case 0x10:
                    strcpy(buf, EQ_ARRAY(char, 0x005457D8));   // Decrease stamina
                    break;
                case 0x11:
                    strcpy(buf, EQ_ARRAY(char, 0x005457C4));   // Increase stamina
                    break;
                case 0x12:
                    strcpy(buf, EQ_ARRAY(char, 0x005457B0));   // Decrease agility
                    break;
                case 0x13:
                    strcpy(buf, EQ_ARRAY(char, 0x0054579C));   // Increase agility
                    break;
                case 0x14:
                    strcpy(buf, EQ_ARRAY(char, 0x00545788));   // Decrease dexterity
                    break;
                case 0x15:
                    strcpy(buf, EQ_ARRAY(char, 0x00545774));   // Increase dexterity
                    break;
                case 0x16:
                    strcpy(buf, EQ_ARRAY(char, 0x00545764));   // Decrease wisdom
                    break;
                case 0x17:
                    strcpy(buf, EQ_ARRAY(char, 0x00545754));   // Increase wisdom
                    break;
                case 0x18:
                    strcpy(buf, EQ_ARRAY(char, 0x0054573C));   // Decrease intelligence
                    break;
                case 0x19:
                    strcpy(buf, EQ_ARRAY(char, 0x00545724));   // Increase intelligence
                    break;
                case 0x1A:
                    strcpy(buf, EQ_ARRAY(char, 0x00545710));   // Decrease charisma
                    break;
                case 0x1B:
                    strcpy(buf, EQ_ARRAY(char, 0x005456FC));   // Increase charisma
                    break;
                case 0x1C:
                    strcpy(buf, EQ_ARRAY(char, 0x005456F4));   // Proceed
                    break;
                case 0x1D:
                    strcpy(buf, EQ_ARRAY(char, 0x005456EC));   // Go back
                    break;
                default:
                    break;
                case 0x1F:
                    strcpy(buf, EQ_ARRAY(char, 0x005456DC));   // Barbarian race
                    break;
                case 0x20:
                    strcpy(buf, EQ_ARRAY(char, 0x005456CC));   // Dark Elf race
                    break;
                case 0x21:
                    strcpy(buf, EQ_ARRAY(char, 0x005456C0));   // Dwarf race
                    break;
                case 0x22:
                    strcpy(buf, EQ_ARRAY(char, 0x005456B0));   // Erudite race
                    break;
                case 0x23:
                    strcpy(buf, EQ_ARRAY(char, 0x005456A4));   // Gnome race
                    break;
                case 0x24:
                    strcpy(buf, EQ_ARRAY(char, 0x00545694));   // Half Elf race
                    break;
                case 0x25:
                    strcpy(buf, EQ_ARRAY(char, 0x00545684));   // Halfling race
                    break;
                case 0x26:
                    strcpy(buf, EQ_ARRAY(char, 0x00545674));   // High Elf race
                    break;
                case 0x27:
                    strcpy(buf, EQ_ARRAY(char, 0x00545668));   // Human race
                    break;
                case 0x28:
                    strcpy(buf, EQ_ARRAY(char, 0x0054565C));   // Ogre race
                    break;
                case 0x29:
                    strcpy(buf, EQ_ARRAY(char, 0x00545650));   // Troll race
                    break;
                case 0x2A:
                    strcpy(buf, EQ_ARRAY(char, 0x00545640));   // Wood Elf race
                    break;
                case 0x2B:
                    strcpy(buf, EQ_ARRAY(char, 0x00545634));   // Male sex
                    break;
                case 0x2C:
                    strcpy(buf, EQ_ARRAY(char, 0x00545628));   // Female sex
                    break;
                case 0x2D:
                    if (*(char*)(g_char_create_guess + 0x19A2) != 0)
                        strcpy(buf, EQ_ARRAY(char, 0x0054561C));   // Zoom out
                    else
                        strcpy(buf, EQ_ARRAY(char, 0x00545614));   // Zoom in
                    break;
                case 0x2E:
                    strcpy(buf, EQ_ARRAY(char, 0x00545608));   // Iksar race
                    break;
                case 0x32:
                case 0x33:
                case 0x34:
                case 0x35:
                case 0x36:
                case 0x37:
                case 0x38:
                case 0x39:
                case 0x3A:
                case 0x3B:
                case 0x3C:
                case 0x3D:
                case 0x3E:
                case 0x3F:
                case 0x40:
                case 0x41:
                case 0x42:
                    strcpy(buf, EQ_ARRAY(char, 0x005455F4));   // A deity of Norrath
                    break;
                case 0x43:
                    strcpy(buf, EQ_ARRAY(char, 0x005455DC));   // Alignment slider bar
                    break;
                case 0x44:
                    strcpy(buf, EQ_ARRAY(char, 0x005455D0));   // More evil
                    break;
                case 0x45:
                    strcpy(buf, EQ_ARRAY(char, 0x005455C4));   // More good
                    break;
                case 0x46:
                case 0x47:
                    if (*(char*)(g_char_create_guess + 0x19A2) == 0)
                        return;
                    strcpy(buf, EQ_ARRAY(char, 0x005455B8));   // Toggle face
                    break;
                case 0x48:
                case 0x49:
                    strcpy(buf, EQ_ARRAY(char, 0x005455AC));   // Toggle city
                    break;
                case 0x4A:
                case 0x4B:
                case 0x4C:
                case 0x4D:
                case 0x4E:
                case 0x4F:
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                    if (EQ_METHOD(4, bool, 0x0041C957, (void*, short, short, short, short), *(void**)(g_char_create_guess + 0x660))(buttonId - 0x4A, *(short*)(g_char_create_guess + 0x1BD2),
                         pinstEQ_CharCreateS_guess->deity, pinstEQ_CharCreateS_guess->class_))
                        strcpy(buf, EQ_ARRAY(char, 0x00545598));   // Valid starting city
                    else
                        strcpy(buf, EQ_ARRAY(char, 0x00545580));   // Invalid starting city
                    break;
                }
                if (g_eqmain_guess == 0)
                    return;
                EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, char*, int), g_eqmain_guess)(pt.x, pt.y + 0x14, buf, 10);
                return;
            }
        }
    }
    else
    {
        for (node = *(char**)(charSel + 0x70); node != 0; node = *(char**)(node + 0x20))
        {
            if (EQ_GLOBAL(PtInRectProc_guess, 0x0053423C)((const RECT*)(node + 4), pt))
            {
                buttonId = *(short*)(node + 0x14);
                if (buttonId < 0)
                    return;
                buttonIndex = buttonId;
                if (buttonIndex >= 0)
                {
                    if (buttonIndex > 7)
                    {
                        if (buttonIndex != 8)
                        {
                            if (buttonIndex != 9)
                            {
                                if (buttonIndex == 10)
                                    strcpy(buf, EQ_ARRAY(char, 0x00545570));   // Quit EverQuest
                            }
                            else
                                strcpy(buf, EQ_ARRAY(char, 0x00545550));   // Delete the selected character
                        }
                        else
                            strcpy(buf, EQ_ARRAY(char, 0x00545540));   // Enter Norrath
                    }
                    else if (buttonId == *(short*)(charSel + 0x80))
                        sprintf(buf, EQ_ARRAY(char, 0x00545520),
                            (char*)pinstCEverQuest_guess + 0x22400 + *(short*)(charSel + 0x80) * 30);   // Currently chosen character: %s
                    else if (strcmp((char*)pinstCEverQuest_guess + 0x22400 + buttonIndex * 30, EQ_ARRAY(char, 0x005454F0)) == 0)
                        strcpy(buf, EQ_ARRAY(char, 0x00545508));   // Create a new character
                    else
                        sprintf(buf, EQ_ARRAY(char, 0x005454F8),
                            (char*)pinstCEverQuest_guess + 0x22400 + buttonIndex * 30);   // Character: %s
                }
                if (g_eqmain_guess == 0)
                    return;
                EQ_METHOD(4, void, 0x0046D4DA, (void*, int, int, char*, int), g_eqmain_guess)(pt.x, pt.y + 0x14, buf, 10);
                return;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00431568
// Removes the deleted character's saved options file.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The ECX character-select object is unused. Build the per-character option filename from the login name
// and pass it to the original file-removal helper.
void CEverQuest::RemoveCharacterOptionFile(char* filePrefix)
{
    char filename[128];
    char serverName[128];
    char* loginName;
    size_t count;

    strcpy(filename, filePrefix);
    strcat(filename, (char*)0x005459A4);
    loginName = (char*)0x006EE3F4;
    if (_strnicmp(loginName, (char*)0x0054599C, 7) == 0)
    {
        strncpy(serverName, (char*)0x006EE3FC, 3);
        serverName[3] = 0;
        if (serverName[2] == '.')
            serverName[2] = 0;
    }
    else
    {
        count = EQ_CDECL(size_t, 0x00523560, (const char*, const char*))(loginName, (char*)0x00545998);
        strncpy(serverName, loginName, count);
        serverName[count] = 0;
    }
    strcat(filename, serverName);
    strcat(filename, (char*)0x00545990);
    EQ_CDECL(void, 0x00523548, (char*))(filename);
}

// FUNCTION: TRILOGY 0x00433330
// Sets inventory item movement state or enables fast dropping.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name EQ_Colums_SetColumnState, low)
// __thiscall on the object at 0x006E4DF8 (ExecuteCmd). 1000 sets the flag at +0x9DC; anything else clears it
// and stores the value at +0x9D8.
void EQ_Columns_guess::SetColumnState_guess(int state)
{
    if (state == 1000)
    {
        *(s32*)((char*)this + 0x9DC) = 1;
    }
    else
    {
        *(s32*)((char*)this + 0x9DC) = 0;
        *(s32*)((char*)this + 0x9D8) = state;
    }
}

// FUNCTION: TRILOGY 0x0043abd2
// Derived EQ_Overlay destructor: sets the vtable pointer, then tail-calls ~EQ_Overlay (0x0045005D).
void __fastcall DestroyOverlay_guess(void* self)
{
    *(u32*)self = 0x0053547C;
    EQ_METHOD(0, void, 0x0045005D, (void*), self)();
}

// FUNCTION: TRILOGY 0x004567f8
// Saves the player's custom command settings and related character configuration.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// CEverQuest::saveCustom_guess. this in ECX, no stack arguments, no return value. When the three global gates
// permit it, builds "<profile name><login prefix>.opt" (the part after the profile name is the first 3 characters
// of the world name for "eqworld*" logins, else the login name up to its first delimiter), opens it and writes:
// a 12-byte "optn" header (version 2), the 10x10 4-byte table at this+0x6841e, 10x12 custom records of 0x136
// bytes at this+0x5f196, six words at this+0xff4, four words at this+0xfec and 0x100 bytes at this+0xeda.
// everquest.h has no declaration for it, so it is a free __fastcall taking this in ECX.
void __fastcall CEverQuest_saveCustom_guess(void* self)
{
    char fileName[128];
    char serverName[128];
    size_t count;
    int row;
    FILE* file;
    char* p;
    int n;

    if (*(u8*)0x006b7111 == 0 &&
        *(u32*)0x006b63e8 != 0 &&
        *(u8*)0x006b7110 == 0)
    {
        strcpy(fileName, (char*)g_pc_guess);
        strcat(fileName, (char*)0x005459a4);
        if (_strnicmp((char*)0x006ee3f4, (char*)0x0054599c, 7) == 0)
        {
            strncpy(serverName, (char*)0x006ee3fc, 3);
            serverName[3] = 0;
            if (serverName[2] == '.')
                serverName[2] = 0;
        }
        else
        {
            count = EQ_CDECL(size_t, 0x00523560, (const char*, const char*))
                ((char*)0x006ee3f4, (char*)0x00545998);
            strncpy(serverName, (char*)0x006ee3f4, count);
            serverName[count] = 0;
        }
        strcat(fileName, serverName);
        strcat(fileName, (char*)0x00545990);
        EQ_CDECL(void, 0x00523548, (char*))(fileName);
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x005588dc);
        if (file == 0)
        {
            EQTRACE_guess((char*)0x0055a964);
        }
        else
        {
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
            strcpy((char*)0x005921c0, (char*)0x0055a95c);
            *(u32*)0x005921c8 = 2;
            EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                ((void*)0x005921c0, 1, 0xc, file);
            for (row = 0; row < 10; row++)
            {
                for (n = 0; n < 10; n++)
                {
                    EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                        (((u32 (*)[10])((char*)self + 0x6841e))[row] + n, 4, 1, file);
                }
            }
            for (count = 0; count < 10; count++)
            {
                for (row = 0; row < 12; row++)
                {
                    EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                        (((char (*)[12][0x136])((char*)self + 0x5f196))[count][row], 0x136, 1, file);
                }
            }
            p = (char*)self + 0xff4;
            n = 6;
            do
            {
                EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))(p, 2, 1, file);
                p += 2;
                n--;
            } while (n != 0);
            p = (char*)self + 0xfec;
            n = 4;
            do
            {
                EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))(p, 2, 1, file);
                p += 2;
                n--;
            } while (n != 0);
            EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                ((char*)self + 0xeda, 0x100, 1, file);
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        }
    }
}

// FUNCTION: TRILOGY 0x00456a14
// Loads the per-character .opt custom file, drops skill slots the character can't use, then saves it back
// CEverQuest::loadCustom_guess. this in ECX, no stack arguments, no return value. Resets the custom block with
// initCustom_guess, then reads "<profile name><login prefix>.opt" (falling back to "<profile name>.opt"): after the
// 12-byte "optn" header (version >= 2: the 10x10 table of 4-byte entries at this+0x6841e and 10x12 custom records of
// 0x136 bytes at this+0x5f196; older files: a 6x6 sub-table with a row stride of 10 entries and 12 records) it reads
// six words at this+0xff4, four words at this+0xfec and 0x100 bytes at this+0xeda. Skill ids that the character
// lacks (or that are not allowed) are set to -1, and table entries of kind 2/3 pointing at a -1 slot are cleared.
// everquest.h has no declaration for it, so it is a free __fastcall taking this in ECX.
void __fastcall CEverQuest_loadCustom_guess(EQ_Main* self)
{
    char fileName[128];
    char serverName[128];
    size_t count;
    FILE* file;
    char* tmp;
    char* counter;
    int remaining;
    int loopsLeft;
    char* p;
    short* sp;
    short* skillBase;
    int i;
    int j;

    EQ_METHOD(0, void, 0x00456DFE, (void*), self)();
    strcpy(fileName, (char*)g_pc_guess);
    strcat(fileName, (char*)0x005459a4);
    if (_strnicmp((char*)0x006ee3f4, (char*)0x0054599c, 7) == 0)
    {
        strncpy(serverName, (char*)0x006ee3fc, 3);
        serverName[3] = 0;
        if (serverName[2] == '.')
            serverName[2] = 0;
    }
    else
    {
        count = EQ_CDECL(size_t, 0x00523560, (const char*, const char*))
            ((char*)0x006ee3f4, (char*)0x00545998);
        strncpy(serverName, (char*)0x006ee3f4, count);
        serverName[count] = 0;
    }
    strcat(fileName, serverName);
    strcat(fileName, (char*)0x00545990);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x0053c2bc);
    if (file == 0)
    {
        strcpy(fileName, (char*)g_pc_guess);
        strcat(fileName, (char*)0x00545990);
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x0053c2bc);
        if (file == 0)
            goto done;
    }
    EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
    EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))((void*)0x005921c0, 1, 0xc, file);
    if (strcmp((char*)0x005921c0, (char*)0x0055a95c) == 0 && *(int*)0x005921c8 >= 2)
    {
        remaining = 10;
        p = (char*)self + 0x6841e;
        tmp = p;
        do
        {
            counter = (char*)10;
            do
            {
                EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(p, 4, 1, file);
                p += 4;
                counter = (char*)((int)counter - 1);
            } while (counter != 0);
            remaining--;
        } while (remaining != 0);
        p = (char*)self + 0x5f196;
        counter = (char*)10;
        do
        {
            remaining = 12;
            do
            {
                EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(p, 0x136, 1, file);
                p += 0x136;
                remaining--;
            } while (remaining != 0);
            counter = (char*)((int)counter - 1);
        } while (counter != 0);
    }
    else
    {
        EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
        loopsLeft = 6;
        tmp = (char*)self + 0x6841e;
        counter = tmp;
        do
        {
            p = counter;
            remaining = 6;
            do
            {
                EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(p, 4, 1, file);
                p += 4;
                remaining--;
            } while (remaining != 0);
            counter += 0x28;
            loopsLeft--;
        } while (loopsLeft != 0);
        p = (char*)self + 0x5f196;
        loopsLeft = 12;
        do
        {
            EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(p, 0x136, 1, file);
            p += 0x136;
            loopsLeft--;
        } while (loopsLeft != 0);
    }
    loopsLeft = 6;
    skillBase = &self->abilitySlots_guess[0];
    sp = skillBase;
    do
    {
        EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(sp, 2, 1, file);
        sp++;
        loopsLeft--;
    } while (loopsLeft != 0);
    loopsLeft = 4;
    sp = &self->combatSlots_guess[0];
    do
    {
        EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(sp, 2, 1, file);
        sp++;
        loopsLeft--;
    } while (loopsLeft != 0);
    EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))((char*)self + 0xeda, 0x100, 1, file);
    EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
    i = 6;
    sp = skillBase;
    do
    {
        if (*sp > -1)
        {
            if (*sp < 100)
            {
                if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)((u8)*sp) == 0)
                    *sp = -1;
            }
            else
            {
                if (g_pc_guess->skills[*sp] != 0)
                    *sp = -1;
                switch (*sp)
                {
                case 0x69:
                case 0x6b:
                case 0x6f:
                    break;
                case 0x72:
                    if (g_pc_guess->level >= 9)
                        break;
                default:
                    *sp = -1;
                }
            }
        }
        sp++;
        i--;
    } while (i != 0);
    i = 4;
    sp = &self->combatSlots_guess[0];
    do
    {
        if (*sp > -1)
        {
            if (*sp < 100)
            {
                if (EQ_METHOD(1, u8, 0x0041A3E9, (void*, u8), g_pc_guess)((u8)*sp) == 0)
                    *sp = -1;
            }
            else
            {
                if (g_pc_guess->skills[*sp] != 0)
                    *sp = -1;
                switch (*sp)
                {
                case 0x69:
                case 0x6b:
                case 0x6f:
                    break;
                default:
                    *sp = -1;
                }
            }
        }
        sp++;
        i--;
    } while (i != 0);
    i = 0x1a108;
    loopsLeft = 10;
    p = tmp;
    do
    {
        tmp = (char*)10;
        j = 0;
        do
        {
            if (*p == 2 &&
                *(short*)((char*)self + 0xfec + *(short*)((char*)self + (i + j) * 4) * 2) == -1)
            {
                *p = 0;
                *(short*)((char*)self + (i + j) * 4) = 0;
            }
            if (*p == 3 &&
                *(short*)((char*)self + 0xff4 + *(short*)((char*)self + (i + j) * 4) * 2) == -1)
            {
                *p = 0;
                *(short*)((char*)self + (i + j) * 4) = 0;
            }
            j++;
            p += 4;
            tmp = (char*)((int)tmp - 1);
        } while (tmp != 0);
        i += 10;
        loopsLeft--;
    } while (loopsLeft != 0);
done:
    EQ_METHOD(0, void, 0x004567F8, (void*), self)();
}

// FUNCTION: TRILOGY 0x00456dfe
// Resets the 120 custom social buttons (10 pages x 12) and gives page 1 its default commands (/afk, /anon, /con...)
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: flagPtr is lea edi (original: lea eax, mov edi,eax); the or-word store precedes the byte store (original: after).
// CEverQuest::initCustom_guess. this in ECX, no stack arguments, no return value. Clears the 120 custom-command
// records at this+0x5f196 (310 bytes each: a 9-byte name, five 60-byte strings, a flag byte at +0x135) and the
// 10x10 table of 4-byte entries at this+0x68420, sets five dwords at +0xfec..+0xffc to -1 and clears the byte at
// +0xeda. Then, for each of the first 12 records whose name is empty, it copies the default name and first string
// and clears the flag. everquest.h has no declaration for it, so it is a free __fastcall taking this in ECX.
void __fastcall CEverQuest_initCustom_guess(void* self)
{
    int groupsLeft;
    int recordsLeft;
    int stringsLeft;
    char* stringPtr;
    char* flagPtr;
    u16* tableEntry;
    int i;
    int rowsLeft;

    groupsLeft = 10;
    flagPtr = (char*)self + 0x5f2cb;
    do
    {
        recordsLeft = 0xc;
        do
        {
            strcpy(flagPtr - 0x135, EQ_ARRAY(char, 0x0058FBA4));
            *flagPtr = 0;
            stringPtr = flagPtr - 300;
            stringsLeft = 5;
            do
            {
                strcpy(stringPtr, EQ_ARRAY(char, 0x0058FBA4));
                stringPtr = stringPtr + 0x3c;
                stringsLeft = stringsLeft + -1;
            } while (stringsLeft != 0);
            flagPtr = flagPtr + 0x136;
            recordsLeft = recordsLeft + -1;
        } while (recordsLeft != 0);
        groupsLeft = groupsLeft + -1;
    } while (groupsLeft != 0);
    tableEntry = (u16*)((char*)self + 0x68420);
    rowsLeft = 10;
    do
    {
        i = 10;
        do
        {
            *(u8*)(tableEntry - 1) = 0;
            *tableEntry = 0xffff;
            tableEntry = tableEntry + 2;
            i = i + -1;
        } while (i != 0);
        rowsLeft = rowsLeft + -1;
    } while (rowsLeft != 0);
    for (i = 0; i < 2; i++)
        ((u32*)((char*)self + 0xfec))[i] = 0xffffffff;
    for (i = 0; i < 3; i++)
        ((u32*)((char*)self + 0xff4))[i] = 0xffffffff;
    *((char*)self + 0xeda) = 0;
    if (*((char*)self + 0x5f196) == '\0')
    {
        strcpy((char*)self + 0x5f196, EQ_ARRAY(char, 0x0055AA50));
        strcpy((char*)self + 0x5f19f, EQ_ARRAY(char, 0x0055AA48));
        *((char*)self + 0x5f2cb) = 0;
    }
    if (*((char*)self + 0x5f2cc) == '\0')
    {
        strcpy((char*)self + 0x5f2cc, EQ_ARRAY(char, 0x0055AA40));
        strcpy((char*)self + 0x5f2d5, EQ_ARRAY(char, 0x0055AA38));
        *((char*)self + 0x5f401) = 0;
    }
    if (*((char*)self + 0x5f402) == '\0')
    {
        strcpy((char*)self + 0x5f402, EQ_ARRAY(char, 0x0055AA30));
        strcpy((char*)self + 0x5f40b, EQ_ARRAY(char, 0x0055AA24));
        *((char*)self + 0x5f537) = 0;
    }
    if (*((char*)self + 0x5f538) == '\0')
    {
        strcpy((char*)self + 0x5f538, EQ_ARRAY(char, 0x0055AA20));
        strcpy((char*)self + 0x5f541, EQ_ARRAY(char, 0x0055AA18));
        *((char*)self + 0x5f66d) = 0;
    }
    if (*((char*)self + 0x5f66e) == '\0')
    {
        strcpy((char*)self + 0x5f66e, EQ_ARRAY(char, 0x0055AA0C));
        strcpy((char*)self + 0x5f677, EQ_ARRAY(char, 0x0055AA04));
        *((char*)self + 0x5f7a3) = 0;
    }
    if (*((char*)self + 0x5f7a4) == '\0')
    {
        strcpy((char*)self + 0x5f7a4, EQ_ARRAY(char, 0x0055A9FC));
        strcpy((char*)self + 0x5f7ad, EQ_ARRAY(char, 0x0055A9F4));
        *((char*)self + 0x5f8d9) = 0;
    }
    if (*((char*)self + 0x5f8da) == '\0')
    {
        strcpy((char*)self + 0x5f8da, EQ_ARRAY(char, 0x0055A9EC));
        strcpy((char*)self + 0x5f8e3, EQ_ARRAY(char, 0x0055A9E0));
        *((char*)self + 0x5fa0f) = 0;
    }
    if (*((char*)self + 0x5fa10) == '\0')
    {
        strcpy((char*)self + 0x5fa10, EQ_ARRAY(char, 0x0055A9D8));
        strcpy((char*)self + 0x5fa19, EQ_ARRAY(char, 0x0055A9D0));
        *((char*)self + 0x5fb45) = 0;
    }
    if (*((char*)self + 0x5fb46) == '\0')
    {
        strcpy((char*)self + 0x5fb46, EQ_ARRAY(char, 0x0055A9C8));
        strcpy((char*)self + 0x5fb4f, EQ_ARRAY(char, 0x0055A9C0));
        *((char*)self + 0x5fc7b) = 0;
    }
    if (*((char*)self + 0x5fc7c) == '\0')
    {
        strcpy((char*)self + 0x5fc7c, EQ_ARRAY(char, 0x0055A9B8));
        strcpy((char*)self + 0x5fc85, EQ_ARRAY(char, 0x0055A9B0));
        *((char*)self + 0x5fdb1) = 0;
    }
    if (*((char*)self + 0x5fdb2) == '\0')
    {
        strcpy((char*)self + 0x5fdb2, EQ_ARRAY(char, 0x0055A9A8));
        strcpy((char*)self + 0x5fdbb, EQ_ARRAY(char, 0x0055A99C));
        *((char*)self + 0x5fee7) = 0;
    }
    if (*((char*)self + 0x5fee8) == '\0')
    {
        strcpy((char*)self + 0x5fee8, EQ_ARRAY(char, 0x0055A994));
        strcpy((char*)self + 0x5fef1, EQ_ARRAY(char, 0x0055A98C));
        *((char*)self + 0x6001d) = 0;
    }
}

// FUNCTION: TRILOGY 0x0048d5d9
// Fills the 74 short 20-byte text slots at +0x2DF0 with built-in defaults; what the slots label is not known
// CEverQuest::loadDefaultStrings_guess (name is a guess; no header declares it). this in ECX, no stack arguments,
// no return value. For i = 0..0x49 it copies a fixed string global into the 20-byte field at this+0x2df0+20*i with
// strcpy. Every i has its own case, so the default (which would clear the field's first byte) is never reached.
// Called by CEverQuest::loadOptions_guess (0x00405b18). A free __fastcall taking this in ECX.
void __fastcall CEverQuest_loadDefaultStrings_guess(void* self)
{
    int i;

    for (i = 0; i < 0x4a; i++)
    {
        switch (i)
        {
        case 0x0:
            strcpy((char*)self + 0x2df0, EQ_ARRAY(char, 0x0058FBA4));
            break;
        case 0x1:
            strcpy((char*)self + 0x2e04, EQ_ARRAY(char, 0x0055D8F8));
            break;
        case 0x2:
            strcpy((char*)self + 0x2e18, EQ_ARRAY(char, 0x0055D8F4));
            break;
        case 0x3:
            strcpy((char*)self + 0x2e2c, EQ_ARRAY(char, 0x0055D8F0));
            break;
        case 0x4:
            strcpy((char*)self + 0x2e40, EQ_ARRAY(char, 0x0055D8EC));
            break;
        case 0x5:
            strcpy((char*)self + 0x2e54, EQ_ARRAY(char, 0x0055D8E8));
            break;
        case 0x6:
            strcpy((char*)self + 0x2e68, EQ_ARRAY(char, 0x0055D8E4));
            break;
        case 0x7:
            strcpy((char*)self + 0x2e7c, EQ_ARRAY(char, 0x0055D8E0));
            break;
        case 0x8:
            strcpy((char*)self + 0x2e90, EQ_ARRAY(char, 0x0055D8DC));
            break;
        case 0x9:
            strcpy((char*)self + 0x2ea4, EQ_ARRAY(char, 0x0055D8D8));
            break;
        case 0xa:
            strcpy((char*)self + 0x2eb8, EQ_ARRAY(char, 0x0055D8D4));
            break;
        case 0xb:
            strcpy((char*)self + 0x2ecc, EQ_ARRAY(char, 0x0055D8D0));
            break;
        case 0xc:
            strcpy((char*)self + 0x2ee0, EQ_ARRAY(char, 0x0055D8CC));
            break;
        case 0xd:
            strcpy((char*)self + 0x2ef4, EQ_ARRAY(char, 0x0055D8C8));
            break;
        case 0xe:
            strcpy((char*)self + 0x2f08, EQ_ARRAY(char, 0x0055D8C4));
            break;
        case 0xf:
            strcpy((char*)self + 0x2f1c, EQ_ARRAY(char, 0x0055D8C0));
            break;
        case 0x10:
            strcpy((char*)self + 0x2f30, EQ_ARRAY(char, 0x0055D8BC));
            break;
        case 0x11:
            strcpy((char*)self + 0x2f44, EQ_ARRAY(char, 0x0055D8B8));
            break;
        case 0x12:
            strcpy((char*)self + 0x2f58, EQ_ARRAY(char, 0x0055D8B4));
            break;
        case 0x13:
            strcpy((char*)self + 0x2f6c, EQ_ARRAY(char, 0x0055D8B0));
            break;
        case 0x14:
            strcpy((char*)self + 0x2f80, EQ_ARRAY(char, 0x0055D8AC));
            break;
        case 0x15:
            strcpy((char*)self + 0x2f94, EQ_ARRAY(char, 0x0055D8A8));
            break;
        case 0x16:
            strcpy((char*)self + 0x2fa8, EQ_ARRAY(char, 0x0055D8A4));
            break;
        case 0x17:
            strcpy((char*)self + 0x2fbc, EQ_ARRAY(char, 0x0055D8A0));
            break;
        case 0x18:
            strcpy((char*)self + 0x2fd0, EQ_ARRAY(char, 0x0055D89C));
            break;
        case 0x19:
            strcpy((char*)self + 0x2fe4, EQ_ARRAY(char, 0x0055D898));
            break;
        case 0x1a:
            strcpy((char*)self + 0x2ff8, EQ_ARRAY(char, 0x0055D894));
            break;
        case 0x48:
            strcpy((char*)self + 0x3390, EQ_ARRAY(char, 0x0055D890));
            break;
        case 0x49:
            strcpy((char*)self + 0x33a4, EQ_ARRAY(char, 0x0055D88C));
            break;
        case 0x20:
            strcpy((char*)self + 0x3070, EQ_ARRAY(char, 0x0055D888));
            break;
        case 0x21:
            strcpy((char*)self + 0x3084, EQ_ARRAY(char, 0x0055D884));
            break;
        case 0x22:
            strcpy((char*)self + 0x3098, EQ_ARRAY(char, 0x0055D880));
            break;
        case 0x23:
            strcpy((char*)self + 0x30ac, EQ_ARRAY(char, 0x0055D87C));
            break;
        case 0x24:
            strcpy((char*)self + 0x30c0, EQ_ARRAY(char, 0x0055D878));
            break;
        case 0x25:
            strcpy((char*)self + 0x30d4, EQ_ARRAY(char, 0x0055D874));
            break;
        case 0x26:
            strcpy((char*)self + 0x30e8, EQ_ARRAY(char, 0x0055D870));
            break;
        case 0x47:
            strcpy((char*)self + 0x337c, EQ_ARRAY(char, 0x0055D86C));
            break;
        case 0x1b:
            strcpy((char*)self + 0x300c, EQ_ARRAY(char, 0x0055D868));
            break;
        case 0x1c:
            strcpy((char*)self + 0x3020, EQ_ARRAY(char, 0x0055D864));
            break;
        case 0x1d:
            strcpy((char*)self + 0x3034, EQ_ARRAY(char, 0x0055D860));
            break;
        case 0x1e:
            strcpy((char*)self + 0x3048, EQ_ARRAY(char, 0x0055D85C));
            break;
        case 0x1f:
            strcpy((char*)self + 0x305c, EQ_ARRAY(char, 0x0055D858));
            break;
        case 0x30:
            strcpy((char*)self + 0x31b0, EQ_ARRAY(char, 0x0055D854));
            break;
        case 0x31:
            strcpy((char*)self + 0x31c4, EQ_ARRAY(char, 0x0055D850));
            break;
        case 0x32:
            strcpy((char*)self + 0x31d8, EQ_ARRAY(char, 0x0055D84C));
            break;
        case 0x33:
            strcpy((char*)self + 0x31ec, EQ_ARRAY(char, 0x0055D848));
            break;
        case 0x34:
            strcpy((char*)self + 0x3200, EQ_ARRAY(char, 0x0055D844));
            break;
        case 0x35:
            strcpy((char*)self + 0x3214, EQ_ARRAY(char, 0x0055D840));
            break;
        case 0x36:
            strcpy((char*)self + 0x3228, EQ_ARRAY(char, 0x0055D83C));
            break;
        case 0x37:
            strcpy((char*)self + 0x323c, EQ_ARRAY(char, 0x0055D838));
            break;
        case 0x38:
            strcpy((char*)self + 0x3250, EQ_ARRAY(char, 0x0055D834));
            break;
        case 0x39:
            strcpy((char*)self + 0x3264, EQ_ARRAY(char, 0x0055D830));
            break;
        case 0x3a:
            strcpy((char*)self + 0x3278, EQ_ARRAY(char, 0x0055D82C));
            break;
        case 0x3b:
            strcpy((char*)self + 0x328c, EQ_ARRAY(char, 0x0055D828));
            break;
        case 0x3c:
            strcpy((char*)self + 0x32a0, EQ_ARRAY(char, 0x0055D824));
            break;
        case 0x3d:
            strcpy((char*)self + 0x32b4, EQ_ARRAY(char, 0x0055D820));
            break;
        case 0x3e:
            strcpy((char*)self + 0x32c8, EQ_ARRAY(char, 0x0055D81C));
            break;
        case 0x3f:
            strcpy((char*)self + 0x32dc, EQ_ARRAY(char, 0x0055D818));
            break;
        case 0x40:
            strcpy((char*)self + 0x32f0, EQ_ARRAY(char, 0x0055D814));
            break;
        case 0x41:
            strcpy((char*)self + 0x3304, EQ_ARRAY(char, 0x0055D810));
            break;
        case 0x42:
            strcpy((char*)self + 0x3318, EQ_ARRAY(char, 0x0055D80C));
            break;
        case 0x43:
            strcpy((char*)self + 0x332c, EQ_ARRAY(char, 0x0055D808));
            break;
        case 0x44:
            strcpy((char*)self + 0x3340, EQ_ARRAY(char, 0x0055D804));
            break;
        case 0x45:
            strcpy((char*)self + 0x3354, EQ_ARRAY(char, 0x0055D800));
            break;
        case 0x46:
            strcpy((char*)self + 0x3368, EQ_ARRAY(char, 0x0055D7FC));
            break;
        case 0x27:
            strcpy((char*)self + 0x30fc, EQ_ARRAY(char, 0x0055D7F8));
            break;
        case 0x28:
            strcpy((char*)self + 0x3110, EQ_ARRAY(char, 0x0055D7F4));
            break;
        case 0x29:
            strcpy((char*)self + 0x3124, EQ_ARRAY(char, 0x0055D7F0));
            break;
        case 0x2a:
            strcpy((char*)self + 0x3138, EQ_ARRAY(char, 0x0055D7EC));
            break;
        case 0x2b:
            strcpy((char*)self + 0x314c, EQ_ARRAY(char, 0x0055D7E8));
            break;
        case 0x2c:
            strcpy((char*)self + 0x3160, EQ_ARRAY(char, 0x0055D7E4));
            break;
        case 0x2d:
            strcpy((char*)self + 0x3174, EQ_ARRAY(char, 0x0055D7E0));
            break;
        case 0x2e:
            strcpy((char*)self + 0x3188, EQ_ARRAY(char, 0x0055D7DC));
            break;
        case 0x2f:
            strcpy((char*)self + 0x319c, EQ_ARRAY(char, 0x0055D7D8));
            break;
        default:
            *(char*)((int*)self + i * 5 + 0xb7c) = 0;
            break;
        }
    }
}

// FUNCTION: TRILOGY 0x0048eb3a
// Gets the duration of a player's requested animation, including its fallback animation.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The ECX display argument at the call sites is unused. The routine uses the player's animation
// set, falls back when the requested track is absent, then multiplies the track interval by its frames.
int __fastcall GetAnimationDuration_guess(void* display, int unusedEdx, EQPlayer* player, int anim)
{
    char* table;
    char state = 0;
    u32 tableAddress;
    u32 slotAddress;
    u32 trackAddress;
    int track;
    int animationId;
    int interval;
    int frames;

    if (player == 0)
        return 0;

    table = *(char**)(*(char**)((char*)player + 0x70) + 0x178);
    if (table == 0)
        return 0;

    tableAddress = (u32)table;
    slotAddress = tableAddress + 0x30 + (u32)anim * 4;
    track = *(int*)slotAddress;
    if (track != 0)
        track = *(int*)((u32)track + 4);

    if (track == 0)
    {
        animationId = EQ_METHOD(3, int, 0x0048EBF8, (void*, int, EQPlayer*, char*), display)(anim, player, &state);
        slotAddress = tableAddress + 0x30 + (u32)animationId * 4;
        trackAddress = *(u32*)slotAddress;
        if (trackAddress == 0)
            return 0;

        trackAddress += (u32)(u8)state * 0x244 + 4;
        track = *(int*)trackAddress;
        if (track == 0)
            return 0;
    }

    interval = ((int (__cdecl *)(int))EQ_GLOBAL(void*, 0x006E52F8))(track);
    frames = ((int (__cdecl *)(int))EQ_GLOBAL(void*, 0x006E5304))(track);
    return (s32)((u32)interval * (u32)frames);
}

// The `new` of ReceiveEquipmentItem_guess. Its constructor is the client's item constructor, only declared here, so
// VC6 keeps the EH frame around the `new` (it can't prove the constructor doesn't throw).
class ItemMessageEquipment_guess : public EQ_Equipment
{
public:
    static void* operator new(size_t size)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))((u32)size);
    }
    static void operator delete(void* value)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(value);
    }
    // EXTERNAL: TRILOGY 0x00436099 ItemMessageEquipment_guess::ItemMessageEquipment_guess
    ItemMessageEquipment_guess(void);
};

// FUNCTION: TRILOGY 0x00496213
// Puts an item sent by the world server into its inventory, bag or bank slot; a bad item is deleted and reported
// @early-stop: EH temp of `new` sits under the EH record (ebp-0x10), the original has it under message (ebp-0x6c); ctor call target; dummy EDX of the thiscall helpers; bank-bag 2000 folded into 0x7EE; redundant parent != NULL test folded
// @equivalent: none of the six mutation survivors can change behavior:the EDX dummy of the thiscall vfunc calls, slot - 0x1E and slot + 0x1E agree mod 10 on 0x1E..0x154, and the final else arms of the bag chains are unreachable.
// __cdecl, no result (the caller, HandleWorldMessage 0x00496743, ignores EAX). Copies the 0x124-byte equipment
// message into a new item and files it in the profile slot named by its equip_slot: 0..29 worn/general,
// 30..340 inside a general bag, 2000..2008 bank, 2030.. inside a bank bag. A rejected item is freed and reported
// with opcode 0x2147 (0x5C bytes: -2, -2, error code 0x12..0x18, the rest is stack garbage). The errors are
// 0x12 bogus item (item type 13 or item number < 1000), 0x13 bad charges on a stackable, 0x14 / 0x15 general bag
// missing or not a container, 0x16 / 0x17 the same for a bank bag, 0x18 slot in no range. 0x15 and 0x17 can't
// happen: the bag was already tested for 0. A filed item then runs CheckDupLoreItems and UpdateLight, and
// raises a flag of the display object when the profile's first slot is filled.
void ReceiveEquipmentItem_guess(EQ_Equipment* itemMsg)
{
    s32 message[0x17];
    EQ_Equipment* item = new ItemMessageEquipment_guess;
    void* window;
    s16 slot;
    int bagIndex;
    int bagSlot;
    EQ_Item* parent;

    memcpy(item, itemMsg, 0x124);
    if (item->item_type == 0x0D || item->item.item_nr < 1000)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x0055E314), 0x0D);
        EQ_CDECL(void, 0x005218BB, (void*))(item);
        message[0] = -2;
        message[1] = -2;
        message[2] = 0x12;
        send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
    }
    else
    {
        char text[0x200];

        if (pinstCEverQuest_guess->gameState_guess == 5 &&
            EQ_ARRAY(int, 0x006B32B4)[item->item.icon_nr] == 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E2DC), (int)item->item.item_nr);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(text, 0x0D);
            item->item.icon_nr = 500;
        }

        if (item->click_effect_type == 0 ||
            (EQ_GLOBAL(EQ_Equipment*, 0x005DFBBC) = item,
             EQ_CDECL(int, 0x004A1D83, (EQ_Equipment*))(item)) == 0 ||
            item->charges > 0)
        {
            window = EQ_GLOBAL(void*, 0x006018C0);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
                ((void (__fastcall*)(void*, int))(*(u32**)window)[2])(window, 0);

            window = EQ_GLOBAL(void*, 0x00604984);
            if (window != NULL && *(int*)((char*)window + 0x10) != 0)
            {
                *(int*)((char*)window + 0x1E8) = 1;
                window = EQ_GLOBAL(void*, 0x00604984);
                ((void (__fastcall*)(void*, int))(*(u32**)window)[2])(window, 0);
            }

            slot = item->item.equip_slot;
            if (slot < 0x1E)
            {
                g_pc_guess->inventory_items[slot] = (EQ_Item*)item;
                if (item->item.equip_slot > 0)
                {
                    if (item->item.equip_slot < 0x15)
                        EQ_METHOD(0, void, 0x004D56B0, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
                    if (g_sound_manager_ptr_guess != NULL)
                        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x92, 0);
                }
            }
            else if (slot <= 0x154)
            {
                bagIndex = (slot - 0x1E) / 10;
                bagSlot = (slot - 0x1E) % 10;
                if (bagIndex == 0x1E)
                    bagIndex = 0;
                parent = g_pc_guess->inventory_items[bagIndex];
                if (parent == NULL || parent->type != 1)
                {
                    EQ_CDECL(void, 0x005218BB, (void*))(item);
                    message[0] = -2;
                    message[1] = -2;
                    message[2] = 0x14;
                    send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
                    return;
                }
                else if (parent != NULL)
                {
                    ((EQ_Container*)parent)->contents[bagSlot] = (EQ_Item*)item;
                    if (g_sound_manager_ptr_guess != NULL)
                        EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x92, 0);
                }
                else
                {
                    EQ_CDECL(void, 0x005218BB, (void*))(item);
                    message[0] = -2;
                    message[1] = -2;
                    message[2] = 0x15;
                    send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
                    return;
                }
            }
            else if (slot >= 2000 && slot < 0x7D9)
            {
                g_pc_guess->bank_items[slot - 2000] = (EQ_Item*)item;
            }
            else if (slot >= 0x7EE)
            {
                bagIndex = (slot - 2000 - 0x1E) / 10;
                parent = g_pc_guess->bank_items[bagIndex];
                if (parent == NULL || parent->type != 1)
                {
                    EQ_CDECL(void, 0x005218BB, (void*))(item);
                    message[0] = -2;
                    message[1] = -2;
                    message[2] = 0x16;
                    send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
                    return;
                }
                else if (parent != NULL)
                {
                    bagSlot = (slot - 2000 - 0x1E) % 10;
                    ((EQ_Container*)parent)->contents[bagSlot] = (EQ_Item*)item;
                }
                else
                {
                    EQ_CDECL(void, 0x005218BB, (void*))(item);
                    message[0] = -2;
                    message[1] = -2;
                    message[2] = 0x17;
                    send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
                    return;
                }
            }
            else
            {
                EQ_CDECL(void, 0x005218BB, (void*))(item);
                message[0] = -2;
                message[1] = -2;
                message[2] = 0x18;
                send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
                return;
            }
            EQ_CDECL(void, 0x004A2816, (CharacterProfile*))(g_pc_guess);
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
            if (g_pc_guess->inventory_items[0] != NULL)
                *(u8*)(g_displayManager_guess + 0x3C) = 1;
            return;
        }
        else
        {
            sprintf(text, EQ_ARRAY(char, 0x0055E704), (int)item->item.item_nr);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(text, 0x0D);
            EQ_CDECL(void, 0x005218BB, (void*))(item);
            message[0] = -2;
            message[1] = -2;
            message[2] = 0x13;
            send_message_guess(g_world_connection_guess, 0x2147, message, 0x5C, 1);
        }
    }
}

// FUNCTION: TRILOGY 0x00496598
// Processes incoming duel challenges, updates duel status, and sends the appropriate response.
// @early-stop: fixed-address thiscall adapters still zero dummy EDX; VC6 puts the shared sprintf/chat tail after the challenge path.
void HandleDuelChallenge_guess(void* duelMessage)
{
    char chatText[512];
    struct DuelResponse_guess
    {
        int target_spawn_id;
        u32 local_spawn_id;
        u8 response_state;
    } response;
    EQPlayer* player;

    player = EQPlayer_GetPlayerFromID_guess(
        g_player_top_guess, *(int*)((char*)duelMessage + 4));
    if (player == 0)
        return;

    if (EQ_METHOD(1, int, 0x004CFF2D, (CEverQuest*, char*), pinstCEverQuest_guess)((char*)player + 1) != 0)
    {
        response.target_spawn_id = *(int*)((char*)player + 0x80);
        response.local_spawn_id = g_local_player_guess->spawn_id;
        response.response_state = 0;
    }
    else
    {
        if (EQ_GLOBAL(char, 0x006B64D0) != 0)
        {
            if (EQ_GLOBAL(int, 0x006B64C8) == *(int*)((char*)player + 0x80))
                return;
            response.target_spawn_id = *(int*)((char*)player + 0x80);
            response.local_spawn_id = g_local_player_guess->spawn_id;
            response.response_state = 1;
        }
        else
        {
            if (EQ_GLOBAL(int, 0x006B64C8) == *(int*)((char*)player + 0x80))
            {
                EQ_GLOBAL(int, 0x006B64CC) = *(int*)((char*)player + 0x80);
                EQ_GLOBAL(char, 0x006B64D0) = 1;
                sprintf(
                    chatText, EQ_ARRAY(char, 0x0055EAC0), (char*)player + 1);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(chatText, 0x0F);
                return;
            }
            else
            {
                if (EQ_GLOBAL(int, 0x006B64C8) == 0
                    && EQ_GLOBAL(int, 0x006B64CC) == 0)
                {
                    EQ_GLOBAL(int, 0x006B64CC) = *(int*)((char*)player + 0x80);
                    sprintf(
                        chatText, EQ_ARRAY(char, 0x0055EA94), (char*)player + 1);
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(chatText, 0x0F);
                    return;
                }
                else if (EQ_GLOBAL(int, 0x006B64C8) == 0
                    && EQ_GLOBAL(int, 0x006B64CC) == *(int*)((char*)player + 0x80))
                {
                    return;
                }
                else
                {
                    response.target_spawn_id = *(int*)((char*)player + 0x80);
                    response.local_spawn_id = g_local_player_guess->spawn_id;
                    response.response_state = 2;
                }
            }
        }
    }

    send_message_guess(g_world_connection_guess, 0x20D0, &response.target_spawn_id, 0x0C, 1);
}

// FUNCTION: TRILOGY 0x004a07bb
// Identifies whether a skill is one of the tradeskills.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name EQ_PC_AtSkillLimit, low, does not fit)
// __cdecl. 1 for the trade skills: 57 (Tinkering), 59..61 (Alchemy, Baking, Tailoring), 63..65 (Blacksmithing,
// Fletching, Brewing), 68..69 (Jewelry Making, Pottery). Only caller: EQ_PC_BitchCanTrain.
int IsTradeskill_guess(int skill)
{
    switch (skill)
    {
    case 57:
    case 59: case 60: case 61:
    case 63: case 64: case 65:
    case 68: case 69:
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004a0f1c
// Decides whether a trainer can teach a character a skill or language and reports refusals.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name EQ_PC_BitchCanTrain, low)
// __cdecl. The trainer's answer to a tell asking the character `pc` to teach it `skill`, or the language `skill`
// when isLanguage is set (skill -1 is an unknown skill). Returns 1 when the teaching may go ahead. Every refusal
// is printed with print_chat and returns 0; advanced trade skills are refused through token_string_to_client.
// trainerLimit is the trainer's cap: a skill or language value at or above it gets the "more ... than I" refusal.
// @early-stop: the shared sprintf/print tail is kept by the -1 path and the skill arm, not the language arm and end block.
int EQ_PC_BitchCanTrain_guess(CharacterProfile* pc, int trainerLimit, int isLanguage, int skill)
{
    char buf[256];
    unsigned char* skillValuePtr;
    unsigned char trainingValue;
    char skillAtCap;
    int skillLimit;

    if (skill == -1)
    {
        sprintf(buf, EQ_ARRAY(char, 0x0056222C),
             EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
        return 0;
    }
    else if (isLanguage != 0)
    {
        if (EQ_CDECL(int, 0x004A08D7, (void*, int))(pc, skill) == 0)
        {
            sprintf(buf, EQ_ARRAY(char, 0x005621B4),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        if (pc->training_points <= 0)
        {
            sprintf(buf, EQ_ARRAY(char, 0x00562154),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        trainingValue = *((unsigned char*)pc + 0x7e + skill);
        if (trainingValue == 100)
        {
            sprintf(buf, EQ_ARRAY(char, 0x00562104),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
                 EQ_METHOD(1, char*, 0x004C8FCE, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(skill));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        if (trainingValue >= trainerLimit)
        {
            sprintf(buf, EQ_ARRAY(char, 0x005620B4),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
    }
    else
    {
        if (EQ_CDECL(int, 0x004A07BB, (int))(skill) != 0)
        {
            trainingValue = *((unsigned char*)pc + 0x9c8 + skill);
            if (trainingValue != 0xfe && trainingValue > 0x14)
            {
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                    (pc, EQ_ARRAY(char, 0x00562080), 0xd);
                return 0;
            }
        }
        if (pc->training_points <= 0)
        {
            sprintf(buf, EQ_ARRAY(char, 0x00562154),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        skillValuePtr = (unsigned char*)((char*)pc + 0x9c8 + skill);
        if (*skillValuePtr == 0xff)
        {
            sprintf(buf, EQ_ARRAY(char, 0x00562040),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        if (*(unsigned char*)(g_skill_records_guess[skill] + 0x28 + *((unsigned char*)pc + 0x36))
            > *((unsigned char*)pc + 0x38))
        {
            sprintf(buf, EQ_ARRAY(char, 0x00561FDC),
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
            return 0;
        }
        skillAtCap = EQ_METHOD(1, char, 0x00480F3B, (void*, int), pc)(skill);
        if (skillAtCap == 0 || *skillValuePtr == 0xfe)
        {
            trainingValue = *skillValuePtr;
            skillLimit = EQ_CDECL(int, 0x004A0C61, (void*, int))(pc, skill);
            if ((int)trainingValue < skillLimit || trainingValue == 0xfe)
            {
                if (trainingValue >= trainerLimit && trainingValue != 0xfe)
                {
                    sprintf(buf, EQ_ARRAY(char, 0x00561F3C),
                         EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1));
                    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
                    return 0;
                }
                return 1;
            }
        }
        sprintf(buf, EQ_ARRAY(char, 0x00561F8C),
             EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_GLOBAL(char*, 0x006E4DC4) + 1),
             g_skill_records_guess[skill]);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(buf);
        return 0;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004a30b5
// Extracts the next delimited field from a configuration line.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name copyStringUntilDelimiter, low)
// __cdecl. Copies the next field of `text`, from the parse position at 0x005FE79C, into `field`: up to a
// NUL, a newline or the delimiter character at 0x00561184 (which end the field and are consumed) or the
// end of the text. Returns strlen(field). The position is compared with strlen(text) only after each
// character, so a position already past the end keeps copying until a terminator turns up.
int copyStringUntilDelimiter_guess(char* text, char* field)
{
    int state = 0;
    char* p = field;

    do
    {
        *p = text[g_parse_pos_guess];
        if (*p == '\0' || *p == '\n' || *p == g_parse_delimiter_guess)
        {
            *p = '\0';
            state = 1;
        }
        p++;
        g_parse_pos_guess++;
        if (g_parse_pos_guess >= strlen(text))
        {
            *p = '\0';
            state = 2;
        }
    } while (state == 0);
    return strlen(field);
}

// FUNCTION: TRILOGY 0x004a3113
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name parseIntFromString, low)
// __cdecl. atoi() of the next field (copyStringUntilDelimiter).
int parseIntFromString_guess(char* text)
{
    char field[256];

    copyStringUntilDelimiter_guess(text, field);
    return atoi(field);
}

// FUNCTION: TRILOGY 0x004a313c
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name parseFloatFromString, low)
// __cdecl. atof() of the next field, returned in ST0 as atof left it.
double parseFloatFromString_guess(char* text)
{
    char field[256];

    copyStringUntilDelimiter_guess(text, field);
    return atof(field);
}

// FUNCTION: TRILOGY 0x004a3165
// Reads the next line from the teleport index file.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name readLineFromFile, low)
// __cdecl. Resets the parse position (0x005FE79C) and reads one line into `line` without its newline.
// Returns the length, or -1 at end of file (EOF before a newline, also when some characters were read).
// The character is compared as a char, so a 0xFF byte in the file counts as end of file.
int readLineFromFile_guess(FILE* file, char* line)
{
    int n = 0;
    int result;
    char c;

    g_parse_pos_guess = 0;
    while (1)
    {
        c = fgetc(file);
        if (c == -1)
        {
            result = -1;
            break;
        }
        if (c == '\n')
        {
            result = 1;
            break;
        }
        line[n] = c;
        n++;
    }
    line[n] = '\0';
    if (result < 0)
        return -1;
    return n;
}

// FUNCTION: TRILOGY 0x004a31cf
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl. Load the teleport index file and publish parsed rows in the table at 0x006E3538.
void msg_teleport_index(char* filePrefix)
{
    char fileName[256];
    char line[2048];
    FILE* file;
    TeleportEntry_guess* entry;
    unsigned int count = 0;

    strcpy(fileName, filePrefix);
    strcat(fileName, (char*)0x005623b0);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x0055c1ac);
    if (file == NULL)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))((char*)0x00562398, fileName);
        Sleep(1000);
        return;
    }

    if (readLineFromFile_guess(file, line) < 1)
    {
        EQ_CDECL(int, 0x005229E8, (const char*))((char*)0x00562384);
        Sleep(5000);
        EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        EQ_CDECL(void, 0x00522DAD, (int))(0);
    }

    entry = g_teleport_table_guess;
    for (;;)
    {
        if (readLineFromFile_guess(file, line) < 1)
            break;

        entry->index = (short)parseIntFromString_guess(line);
        entry->y = (float)parseFloatFromString_guess(line);
        entry->x = (float)parseFloatFromString_guess(line);
        entry->z = (float)parseFloatFromString_guess(line);
        entry->heading = (float)parseFloatFromString_guess(line);
        entry->zone_id = parseIntFromString_guess(line);
        count++;
        entry++;
    }

    EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
    g_teleport_count_guess = (int)count;
    EQ_CDECL(int, 0x005229E8, (const char*, ...))((char*)0x0056236c, (int)count);
}

// FUNCTION: TRILOGY 0x004a4a80
// Validates a new surname and submits it for server confirmation.
void SubmitSurname_guess(int unusedArg, char* input)
{
    char ch;
    size_t length;
    int i;
    char* charPtr;
    char lastname[20];

    if (g_pc_guess->last_name[0] != '\0')
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x005626B4, 0xF);
        return;
    }
    if (g_pc_guess->level < 0x14)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x00562664, 0xF);
        return;
    }
    if (*input == '\0')
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x0056263C, 0xF);
        return;
    }
    length = strlen(input);
    if ((int)length > 0x13)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x00562608, 0xF);
        return;
    }
    EQ_METHOD(3, char*, 0x004DBCE1, (CEverQuest*, char*, char*, int), pinstCEverQuest_guess)(input, lastname, 0x14);
    lastname[19] = '\0';
    length = strlen(lastname);
    for (i = 0; i < (int)length; i++)
    {
        ch = lastname[i];
        if (((ch < 'a') || ('z' < ch)) &&
            ((ch < 'A') || ('Z' < ch)))
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)((const char*)0x005625C0, 0xF);
            return;
        }
    }
    i = 0;
    if (0 < (int)length)
    {
        do
        {
            charPtr = lastname + i;
            ch = (char)EQ_CDECL(int, 0x005225BC, (int))((int)lastname[i]);
            i = i + 1;
            *charPtr = ch;
        } while (i < (int)length);
    }
    lastname[0] = (char)EQ_CDECL(int, 0x005238FD, (int))((int)lastname[0]);
    EQ_METHOD(1, void, 0x004D55B4, (CEverQuest*, char*), pinstCEverQuest_guess)(lastname);
}

// FUNCTION: TRILOGY 0x004a4b76
// Submits a player report with recent chat lines and identifies the reported player.
// __cdecl. The /report handler. An empty name prints the usage line. Otherwise the name is copied into the packet
// (cut to 31 characters). While the report state at 0x006B63E8 is 1, the 10 chat lines in the ring at 0x006D6B2C are
// copied into the packet, the packet goes out (opcode 0x21BD, reliable), the echo line is printed and the state
// becomes 2. Any other state prints the once-per-zone line.
void report_guess(int unused, char* name)
{
    struct ReportPacket_guess
    {
        char name[32];          // 0x00
        char lines[10][512];    // 0x20
        char tail[32];          // 0x1420, never written: the packet sends whatever the stack holds there
    } packet;
    char message[512];
    char* chat_line;
    char** ring;
    char* slot;

    if (name[0] == '\0')
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562764), 0xF);
        return;
    }
    name[0x1F] = '\0';
    strcpy(packet.name, name);
    if (EQ_GLOBAL(int, 0x006B63E8) == 1)
    {
        EQ_GLOBAL(int, 0x006B63E8) = 2;
        slot = packet.lines[0];
        ring = EQ_ARRAY(char*, 0x006D6B2C);
        do
        {
            chat_line = *ring;
            *slot = '\0';
            if ((chat_line != NULL) &&
                (strcmp(chat_line, EQ_ARRAY(char, 0x0055D6D4)) != 0))
            {
                strcpy(slot, *ring);
                slot[0x1FF] = '\0';
            }
            ring = ring + 1;
            slot = slot + 0x200;
        } while ((int)ring < 0x006D6B54);
        send_message_guess
            (g_world_connection_guess, 0x21BD, &packet, 0x1440, 1);
        sprintf(message, EQ_ARRAY(char, 0x0056272C), name);
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(message, 0xF);
        return;
    }
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005626E4), 0xF);
    return;
}

// FUNCTION: TRILOGY 0x004a4d37
// Requests a character rename and can add the old name to the invalid-name list.
// __cdecl. The /name handler: "/name <old> <new> [b]". An empty argument prints the usage line and the 'b' hint.
// Otherwise the first three words are copied into a 94-byte packet (old name, the local player's name, new name, a
// u16 flag that is 1 when the third word starts with 'b', and a zero u16) that goes out as opcode 0x20CB (reliable).
// The new name must be at least 4 characters. Success prints the confirmation and, with the flag, the
// invalid-list line; anything else prints the valid-name line.
void cmd_name_guess(int unused, char* args)
{
    struct NamePacket_guess
    {
        char old_name[30];      // 0x00
        char player_name[30];   // 0x1E
        char new_name[30];      // 0x3C
        u16 flag;               // 0x5A
        u16 zero;               // 0x5C
    } packet;
    char message[128];
    char old_word[40];
    char new_word[40];
    char flag_word[40];
    char* rest;

    if (args[0] == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005628E4));
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x00562898));
        return;
    }
    else
    {
        rest = EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(args, old_word);
        rest = EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(rest, new_word);
        EQ_METHOD(2, char*, 0x004DBC84, (CEverQuest*, char*, char*), pinstCEverQuest_guess)(rest, flag_word);
        strcpy(packet.old_name, old_word);
        strcpy(packet.player_name, g_local_player_guess->name);
        strcpy(packet.new_name, new_word);
        packet.zero = 0;
        packet.flag = 0;
        if (flag_word[0] == 'b')
            packet.flag = 1;
        if ((packet.new_name[0] != '\0') &&
            (strlen(packet.new_name) >= 4))
        {
            send_message_guess
                (g_world_connection_guess, 0x20CB, &packet, 0x5E, 1);
            sprintf(message, EQ_ARRAY(char, 0x00562874), old_word, new_word);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
            if (packet.flag == 0)
                return;
            sprintf(message, EQ_ARRAY(char, 0x00562818), packet.old_name);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(message, 0xD);
            return;
        }
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)(EQ_ARRAY(char, 0x005627EC));
    }
}

// FUNCTION: TRILOGY 0x004b45cb
// Changes a spawn's stance and animation, reporting the change to the server for the local player.
// @early-stop: byte param is loaded into dl at entry (original: cl), which pushes the zero to ecx (original: edx) and reorders the zero stores; EQ_THISCALL's dummy `xor edx,edx` before each call; the 110 case loads ECX before fld1 (in original)
/* fake match: the 'o' stance is the final else so VC6 lays its call out last and it falls into the shared call tail, as in the original */
void __fastcall EQPlayer_ChangeStance_guess(EQPlayer* self, int edx, u8 stance)
{
    u8 oldState;
    u32 charInfo;
    char fits;
    int tmp;
    struct
    {
        u32 local_10;
        u16 local_c;
        u32 local_8;
    } local_packet;

    if (self->stand_state == stance)
    {
        return;
    }
    if ((self->actor_info != 0) && (stance != 0x73))
    {
        self->actor_info->unk_0x18A = 0;
    }
    charInfo = (u32)self->char_info_guess;
    if (self == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        EQ_GLOBAL(u32, 0x006B6474) = 0;
        if (EQ_GLOBAL(u32, 0x006B6590) != 0)
        {
            EQ_GLOBAL(u32, 0x006B6590) = 0;
            EQ_GLOBAL(u32, 0x006E4E8C) = 0;
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00568A3C));
        }
        if (EQ_GLOBAL(u32, 0x006B6478) != 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x00542478));
            EQ_GLOBAL(u32, 0x006B6478) = 0;
            EQ_GLOBAL(u32, 0x006B647C) = 0;
        }
        if (self->stand_state == 'n')
        {
            EQ_METHOD(0, void, 0x004DBE26, (CEverQuest*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))();
        }
        tmp = EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)();
        if (((tmp != 0) && (self->stand_state == 'o')) && (stance == 100))
        {
            self->stand_state = 'd';
            fits = EQ_METHOD(4, char, 0x004AE535, (void*, EQPlayer*, float, float, float), g_display_guess)(self,
                    self->y, self->x, self->z);
            self->stand_state = 'o';
            if (fits == '\0')
            {
                return;
            }
        }
    }
    if (charInfo != 0)
    {
        local_packet.local_10 = self->spawn_id;
        local_packet.local_8 = (u32)stance;
        local_packet.local_c = 0xe;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x20f5, &local_packet, 0xc, 1);
        *(u8*)(charInfo + 0x994) = stance;
    }
    switch (stance)
    {
    case 100:
        oldState = self->stand_state;
        if (oldState != 'o')
        {
            if ((oldState == 'i') || (self->actor_info->animationId_guess == 0x24))
            {
                EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x24, 1, 1, 1.0f, 1);
            }
            else if (oldState != 'f')
            {
                EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x21, 1, 1, 1.0f, 1);
            }
        }
        else
        {
            EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x18, 1, 1, 1.0f, 1);
        }
        break;
    case 105:
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x24, 1, 0, 1.0f, 1);
        self->speed = 0.0f;
        self->delta_heading = 0.0f;
        break;
    case 110:
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x21, 1, 0, 1.0f, 1);
        self->speed = 0.0f;
        self->delta_heading = 0.0f;
        self->actor_info->unk_0x70 =
            g_cdisplay_guess->time_ms_guess;
        break;
    case 111:
        tmp = self->actor_info->animationId_guess;
        if ((tmp == 0x18) || (tmp == 0x16))
            break;
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x18, 1, 0, 1.0f, 1);
        break;
    case 115:
        if (self->actor_info->animationId_guess != 0x10)
        {
            EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(self, 0x10, 1, 0, 1.0f, 1);
        }
        self->speed = 0.0f;
        self->delta_heading = 0.0f;
        break;
    }
    self->stand_state = stance;
    EQ_METHOD(1, void, 0x00407FF1, (void*, EQPlayer*), g_display_guess)(self);
    return;
}

// FUNCTION: TRILOGY 0x004bd259
// Sets up a positional sound source for audio placed in the game world.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// EQSoundEmitter_guess constructor (__thiscall, returns `this`; loadSounds runs it on operator new(0x5C)).
// Links the emitter in as the new list head (0x006E4D80), numbers it, then copies the file record or, with
// none, sets the defaults.
EQSoundEmitter_guess::EQSoundEmitter_guess(const EQSoundEmitter_guess* record)
{
    if (g_sound_emitter_head_guess == NULL)
    {
        g_sound_emitter_head_guess = this;
        prev_guess = NULL;
        next_guess = NULL;
    }
    else
    {
        g_sound_emitter_head_guess->next_guess = this;
        prev_guess = g_sound_emitter_head_guess;
        next_guess = NULL;
        g_sound_emitter_head_guess = this;
    }
    id_guess = g_sound_emitter_next_id_guess++;
    unknown0x08 = 0;
    if (record != NULL)
    {
        y = record->y;
        x = record->x;
        z = record->z;
        radius = record->radius;
        unknown0x20 = record->unknown0x20;
        unknown0x24 = record->unknown0x24;
        unknown0x28 = record->unknown0x28;
        unknown0x2C = 0;
        sound_id_guess[0] = record->sound_id_guess[0];
        sound_id_guess[1] = record->sound_id_guess[1];
        unknown0x38 = record->unknown0x38;
        unknown0x39 = record->unknown0x39;
        unknown0x3A = record->unknown0x3A;
        unknown0x3C = record->unknown0x3C;
        unknown0x40 = record->unknown0x40;
        unknown0x44[0] = record->unknown0x44[0];
        unknown0x44[1] = record->unknown0x44[1];
        unknown0x44[2] = record->unknown0x44[2];
        unknown0x44[3] = record->unknown0x44[3];
        unknown0x54 = -1;
        unknown0x58 = -1;
    }
    else
    {
        sound_id_guess[0] = -1;
        sound_id_guess[1] = -1;
        y = 0.0f;
        unknown0x54 = -1;
        unknown0x58 = -1;
        radius = 100.0f;
        x = 0.0f;
        unknown0x20 = 0;
        unknown0x24 = 0;
        z = 0.0f;
        unknown0x28 = 0;
        unknown0x2C = 0;
        unknown0x38 = 0;
        unknown0x39 = 0;
        unknown0x3A = 0;
        unknown0x3C = 0;
        unknown0x40 = 0;
        unknown0x44[0] = 3000;
        unknown0x44[1] = 3000;
        unknown0x44[2] = 3000;
        unknown0x44[3] = 3000;
    }
}

// FUNCTION: TRILOGY 0x004bd36f
// Removes a positional sound source from the world's active emitters.
// The Mac map attributes this address to EQBBoard.cpp, but its Trilogy callers and list head identify the
// sound-emitter list: 0x004BD259 inserts into 0x006E4D80 and StopWorldDisplay removes from that same head.
EQSoundEmitter_guess::~EQSoundEmitter_guess()
{
    if (next_guess != NULL && prev_guess != NULL)
    {
        next_guess->prev_guess = prev_guess;
        prev_guess->next_guess = next_guess;
    }
    else if (next_guess == NULL && prev_guess != NULL)
    {
        prev_guess->next_guess = NULL;
        g_sound_emitter_head_guess = prev_guess;
    }
    else if (prev_guess == NULL && next_guess != NULL)
    {
        next_guess->prev_guess = NULL;
    }
    else
    {
        prev_guess = NULL;
        next_guess = NULL;
        g_sound_emitter_head_guess = NULL;
    }
}

/* 0x00534278: the client's IAT slot for WINMM timeGetTime (the test DLL does not link winmm). */
typedef DWORD (__stdcall *EQTimeGetTime_t)(void);
#define eq_timeGetTime_iat_guess  EQ_GLOBAL(EQTimeGetTime_t, 0x00534278)

// Layout of the object initialised by EQTimedPair_guess::EQTimedPair_guess (no header definition exists yet).
struct EQTimedPair_guess
{
    EQTimedPair_guess();

    unsigned char a0;           // 0x00
    unsigned char a1;           // 0x01
    unsigned char a2;           // 0x02
    unsigned char a3;           // 0x03
    unsigned short a4;          // 0x04
    unsigned char b0;           // 0x06
    unsigned char b1;           // 0x07
    unsigned char b2;           // 0x08
    unsigned char b3;           // 0x09
    unsigned short b4;          // 0x0A
    unsigned long tickMs;       // 0x0C
    long timeSec;               // 0x10
};

// FUNCTION: TRILOGY 0x004be706
// Initializes a timing record; its specific gameplay purpose is unclear.
// __thiscall constructor (caller CEverQuest ctor). Two identical 6-byte records (8,0,1,1,0x0C64), then the
// wall-clock seconds at +0x10 and timeGetTime at +0x0C. Returns this.
EQTimedPair_guess::EQTimedPair_guess()
{
    a0 = 8;
    a1 = 0;
    a2 = 1;
    a3 = 1;
    a4 = 0xc64;
    b0 = 8;
    b1 = 0;
    b2 = 1;
    b3 = 1;
    b4 = 0xc64;
    timeSec = time(0);
    tickMs = eq_timeGetTime_iat_guess();
}

// FUNCTION: TRILOGY 0x004c15cd
// Keeps a timestamped record of the chat shown during play.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl variadic logger (everquest.h bazaarlog_guess). Formats the message, prefixes the ctime stamp (trailing
// newline cut) and appends "[stamp] message" to "eqlog_<server>_<character or prelogin>.txt".
int __cdecl AppendChatLog_guess(const char* format, ...)
{
    char output[2048];
    char message[2048];
    char fileName[256];
    char timeText[32];
    time_t now;
    char server[4];
    char* last;
    FILE* file;
    va_list args;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    now = time(0);
    strcpy(timeText, ctime(&now));
    last = &timeText[strlen(timeText) - 1];
    if (*last == '\n')
        *last = 0;

    sprintf(output, "[%s] %s", timeText, message);
    strcpy(fileName, "eqlog_");
    strncpy(server, (char*)0x006ee3fc, 3);
    server[3] = 0;
    if (server[2] == '.')
        server[2] = 0;
    strcat(fileName, server);
    strcat(fileName, (char*)0x005459a4);
    if (g_pc_guess != 0 && *(char*)g_pc_guess != 0)
        strcat(fileName, (char*)g_pc_guess);
    else
        strcat(fileName, "prelogin");
    strcat(fileName, ".txt");
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x00564e48);
    if (file != 0)
    {
        fprintf(file, (char*)0x0056a308, output);
        fclose(file);
    }

    return 0;
}

// FUNCTION: TRILOGY 0x004c1703
// Records timestamped diagnostic messages in dbg.txt.
// __cdecl variadic debug logger (globals.h DbgLog_guess). Formats the message, prefixes the ctime stamp
// (trailing newline cut) and a wrapping 16-bit sequence (0x006E4E92), and appends it to "dbg.txt".
int __cdecl DbgLog_guess(const char* formatString, ...)
{
    char output[2048];
    char message[2048];
    char timeText[32];
    time_t now;
    char* last;
    FILE* file;
    va_list args;
    unsigned int sequence;

    va_start(args, formatString);
    vsprintf(message, formatString, args);
    va_end(args);

    now = time(0);
    strcpy(timeText, ctime(&now));
    last = &timeText[strlen(timeText) - 1];
    if (*last == '\n')
        *last = 0;

    sequence = (unsigned short)EQ_GLOBAL(unsigned short, 0x006E4E92);
    EQ_GLOBAL(unsigned short, 0x006E4E92)++;
    sprintf(output, "[%s]%05u:%s", timeText, sequence, message);
    file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))("dbg.txt", (char*)0x00564e48);
    if (file != 0)
    {
        fprintf(file, (char*)0x0056a308, output);
        fclose(file);
    }

    return 0;
}

// FUNCTION: TRILOGY 0x004c17bc
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl variadic printf-style chat logger. The sequence is a wrapping 16-bit counter.
int __cdecl printf_guess(const char* format, ...)
{
    char message[2048];
    char output[2048];
    va_list args;
    unsigned int sequence;

    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    sequence = (unsigned short)EQ_GLOBAL(unsigned short, 0x006E4E94);
    EQ_GLOBAL(unsigned short, 0x006E4E94)++;
    sprintf(output, "%05u:%s", sequence, message);
    if (pinstCEverQuest_guess != 0)
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), pinstCEverQuest_guess)(output);

    return 0;
}

// FUNCTION: TRILOGY 0x004c1823
// Updates the lookup table for teleport destinations received from the server.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl message handler (HandleWorldMessage passes three arguments; only the data is used). The data is
// a short count (1..127 accepted) and, from +4, that many 0x18-byte TeleportEntry_guess records, copied into
// the teleport table (0x006E3538). Other counts leave table and count alone.
void msg_teleport_table_guess(int unused1, int unused2, short* data)
{
    short n = *data;
    int count;
    TeleportEntry_guess* src;
    TeleportEntry_guess* dst;

    if (n > 0 && n < 0x80)
    {
        count = n;
        g_teleport_count_guess = count;
        if (count > 0)
        {
            dst = g_teleport_table_guess;
            src = (TeleportEntry_guess*)(data + 2);
            do
            {
                *dst = *src;
                src++;
                dst++;
            } while (--count != 0);
        }
    }
}

// FUNCTION: TRILOGY 0x004c1862
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl. Looks the teleport index up in the table at 0x006E3538; returns its table index or -1.
int GetTPCoordsFromIndex(int index, float* y, float* x, float* z, float* heading, int* zoneId)
{
    int count;
    int i;

    count = g_teleport_count_guess;
    for (i = 0; i < count; i++)
    {
        if (index == g_teleport_table_guess[i].index)
        {
            // Each field repeats every six 32-bit slots in the teleport table.
            *y = (&g_teleport_table_guess[0].y)[i * 6];
            *x = (&g_teleport_table_guess[0].x)[i * 6];
            *z = (&g_teleport_table_guess[0].z)[i * 6];
            *heading = (&g_teleport_table_guess[0].heading)[i * 6];
            *zoneId = (&g_teleport_table_guess[0].zone_id)[i * 6];
            return i;
        }
    }
    *y = 0.0f;
    *heading = 0.0f;
    *z = 0.0f;
    *x = 0.0f;
    *zoneId = -1;
    return -1;
}

// FUNCTION: TRILOGY 0x004c18ed
// Applies release behavior for gameplay controls bound to the key that was lifted.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void ProcessKeyUp(unsigned int keyCode)
{
    unsigned int keyWithModifiers;
    u32* keyBindings;
    int commandId;

    commandId = 0;
    if (EQ_GLOBAL(int, 0x006E4E6C) == 0)
    {
        g_updateInterval_guess = 0;
        g_nextUpdateTime_guess = 0;
        EQ_GLOBAL(u32, 0x006E1B28) = 0;
        EQ_GLOBAL(u32, 0x006E1B2C) = 0;
        keyWithModifiers = keyCode;
        if (EQ_GLOBAL(int, 0x006B70E8) != 0)
            keyWithModifiers |= 0x10000000;
        if (EQ_GLOBAL(int, 0x006B70EC) != 0)
            keyWithModifiers |= 0x20000000;
        if (EQ_GLOBAL(int, 0x006B70F0) != 0)
            keyWithModifiers |= 0x40000000;
        if ((char)keyWithModifiers == 0x0E &&
            (EQ_METHOD(0, int, 0x004D7CA7, (CEverQuest*), pinstCEverQuest_guess)() == 2 ||
             EQ_GLOBAL(int, 0x006E1D1C) != 0))
        {
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0xCE, 0, 0);
        }
        if ((char)EQ_GLOBAL(u32, 0x006E11F8) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E15F8) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(5, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E11FC) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E15FC) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(6, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E11F4) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E15F4) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(4, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E11F0) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E15F0) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(3, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E122C) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E162C) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x12, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E1230) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E1630) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x13, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E1374) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E1774) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(100, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E1378) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E1778) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x65, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E1370) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E1770) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(99, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E136C) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E176C) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x62, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E137C) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E177C) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x66, 0, 0);
        if ((char)EQ_GLOBAL(u32, 0x006E1380) == (char)keyWithModifiers ||
            (char)EQ_GLOBAL(u32, 0x006E1780) == (char)keyWithModifiers)
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x67, 0, 0);
        EQ_METHOD(0, int, 0x004D7CA7, (CEverQuest*), pinstCEverQuest_guess)();
        keyBindings = EQ_ARRAY(u32, 0x006E15E4);
        keyCode = 0x6A;
        do
        {
            if (keyWithModifiers == keyBindings[-0x100] || keyWithModifiers == *keyBindings)
                EQ_CDECL(void, 0x004D7D6A, (int, int, int))(commandId, 0, 0);
            commandId++;
            keyBindings++;
            keyCode--;
        } while (keyCode != 0);
    }
    return;
}

// FUNCTION: TRILOGY 0x004c1b48
// Blocks binding Escape, Enter, slash, and Windows keys to gameplay commands.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::IsValidKeyMap, high)
// __thiscall, `this` unused. Scan codes that can't be bound: Esc (0x01), Enter (0x1C), / (0x35) and the
// Windows keys (0xDB, 0xDC).
int CEverQuest::IsValidKeyMap(int scanCode)
{
    int valid = 1;

    if (scanCode == 0x01 || scanCode == 0x1C || scanCode == 0x35 || (scanCode > 0xDA && scanCode <= 0xDC))
        valid = 0;
    return valid;
}

// FUNCTION: TRILOGY 0x004c1b72
// Gets the character typed by a key using the active keyboard map.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl. DirectInput scan code -> the character it types (0 for keys that type nothing): the shifted map
// while shift is down, else the custom map in key-map mode 2, else the default map. Unlike
// GetKeyCharacter, Backspace (0x0E) and Space (0x39) count.
int ScanCodeToChar_guess(int scanCode)
{
    int character = 0;
    if (scanCode > 0x100)
        return character;
    switch (scanCode)
    {
    case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09:
    case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x0E: case 0x10: case 0x11: case 0x12:
    case 0x13: case 0x14: case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A:
    case 0x1B: case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x24:
    case 0x25: case 0x26: case 0x27: case 0x28: case 0x29: case 0x2B: case 0x2C: case 0x2D:
    case 0x2E: case 0x2F: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:
    case 0x39: case 0x70: case 0x73: case 0x79: case 0x7B: case 0x7D: case 0x7E: case 0x8D:
    case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0xB3:
    case 0xB5:
        if (g_shift_down_guess != 0)
            character = g_keymap_shifted_guess[scanCode];
        else if (g_keymap_mode_guess == 2)
            character = g_keymap_custom_guess[scanCode];
        else
            character = g_keymap_normal_guess[scanCode];
        break;
    }
    return character;
}

// FUNCTION: TRILOGY 0x004c1c23
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name, low)
// __cdecl. Scan code -> the character shown for it in key-binding text (0x0057D2D0), 0 for keys without one.
int GetKeyCharacter_guess(int scanCode)
{
    int character = 0;
    if (scanCode > 0x100)
        return character;
    switch (scanCode)
    {
    case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07: case 0x08: case 0x09:
    case 0x0A: case 0x0B: case 0x0C: case 0x0D: case 0x10: case 0x11: case 0x12: case 0x13:
    case 0x14: case 0x15: case 0x16: case 0x17: case 0x18: case 0x19: case 0x1A: case 0x1B:
    case 0x1E: case 0x1F: case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25:
    case 0x26: case 0x27: case 0x28: case 0x29: case 0x2B: case 0x2C: case 0x2D: case 0x2E:
    case 0x2F: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x70:
    case 0x73: case 0x79: case 0x7B: case 0x7D: case 0x7E: case 0x8D: case 0x90: case 0x91:
    case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0xB3: case 0xB5:
        character = g_keymap_display_guess[scanCode];
        break;
    }
    return character;
}

// FUNCTION: TRILOGY 0x004c1caa
// Stores a gameplay key binding in the client settings.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl. Writes one key binding to the KeyMaps section of .\eqclient.ini. The entry name is "KEYMAP_" plus table
// entry keymapIndex (plus the 0x0056A370 suffix when alternateBinding == 1). The value is the SHIFT+/CONTROL+/ALT+ words,
// then the key's name from the switch (keys with no display character, and the space key) or the display character
// from GetKeyCharacter_guess; NOKEY for keyBinding == -1.
void writeKeyMapToIni(int keymapIndex, int keyBinding, int alternateBinding)
{
    typedef BOOL (WINAPI *WritePrivateProfileStringA_guess_t)(LPCSTR, LPCSTR, LPCSTR, LPCSTR);
    char keyName[256];
    char valueText[256];
    int keyChar;
    size_t len;

    strcpy(keyName, EQ_ARRAY(char, 0x0056A374));
    strcat(keyName, EQ_ARRAY(char*, 0x0056999C)[keymapIndex]);
    if (alternateBinding == 1)
        strcat(keyName, EQ_ARRAY(char, 0x0056A370));
    valueText[0] = '\0';
    if (keyBinding == -1)
    {
        strcpy(valueText, EQ_ARRAY(char, 0x0055A130));
    }
    else
    {
        if (keyBinding & 0x10000000)
            strcat(valueText, EQ_ARRAY(char, 0x0056A368));
        if (keyBinding & 0x20000000)
            strcat(valueText, EQ_ARRAY(char, 0x0056A35C));
        if (keyBinding & 0x40000000)
            strcat(valueText, EQ_ARRAY(char, 0x0056A354));
        keyBinding &= 0xff;
        keyChar = EQ_CDECL(int, 0x004C1C23, (int))(keyBinding);
        if (keyChar == 0 || keyBinding == 0x39)
        {
            switch (keyBinding)
            {
            case 0x0e: strcat(valueText, EQ_ARRAY(char, 0x0055A280)); break;
            case 0x0f: strcat(valueText, EQ_ARRAY(char, 0x0055A28C)); break;
            case 0x1d: strcat(valueText, EQ_ARRAY(char, 0x00569F30)); break;
            case 0x29: strcat(valueText, EQ_ARRAY(char, 0x0055A298)); break;
            case 0x2a: strcat(valueText, EQ_ARRAY(char, 0x0055A2A0)); break;
            case 0x36: strcat(valueText, EQ_ARRAY(char, 0x0055A2A8)); break;
            case 0x37: strcat(valueText, EQ_ARRAY(char, 0x00569F5C)); break;
            case 0x38: strcat(valueText, EQ_ARRAY(char, 0x0055A250)); break;
            case 0x39: strcat(valueText, EQ_ARRAY(char, 0x0055A258)); break;
            case 0x3a: strcat(valueText, EQ_ARRAY(char, 0x0055A260)); break;
            case 0x3b: strcat(valueText, EQ_ARRAY(char, 0x0055A26C)); break;
            case 0x3c: strcat(valueText, EQ_ARRAY(char, 0x0055A270)); break;
            case 0x3d: strcat(valueText, EQ_ARRAY(char, 0x0055A274)); break;
            case 0x3e: strcat(valueText, EQ_ARRAY(char, 0x0055A24C)); break;
            case 0x3f: strcat(valueText, EQ_ARRAY(char, 0x0055A248)); break;
            case 0x40: strcat(valueText, EQ_ARRAY(char, 0x0055A244)); break;
            case 0x41: strcat(valueText, EQ_ARRAY(char, 0x0055A240)); break;
            case 0x42: strcat(valueText, EQ_ARRAY(char, 0x0055A23C)); break;
            case 0x43: strcat(valueText, EQ_ARRAY(char, 0x0055A238)); break;
            case 0x44: strcat(valueText, EQ_ARRAY(char, 0x0055A234)); break;
            case 0x45: strcat(valueText, EQ_ARRAY(char, 0x0055A22C)); break;
            case 0x4b: strcat(valueText, EQ_ARRAY(char, 0x00569F8C)); break;
            case 0x47: strcat(valueText, EQ_ARRAY(char, 0x00569F74)); break;
            case 0x48: strcat(valueText, EQ_ARRAY(char, 0x00569F6C)); break;
            case 0x49: strcat(valueText, EQ_ARRAY(char, 0x00569F64)); break;
            case 0x4a: strcat(valueText, EQ_ARRAY(char, 0x00569F4C)); break;
            case 0x4c: strcat(valueText, EQ_ARRAY(char, 0x00569F84)); break;
            case 0x4d: strcat(valueText, EQ_ARRAY(char, 0x00569F7C)); break;
            case 0x4e: strcat(valueText, EQ_ARRAY(char, 0x00569F54)); break;
            case 0x4f: strcat(valueText, EQ_ARRAY(char, 0x00569FA4)); break;
            case 0x50: strcat(valueText, EQ_ARRAY(char, 0x00569F9C)); break;
            case 0x51: strcat(valueText, EQ_ARRAY(char, 0x00569F94)); break;
            case 0x52: strcat(valueText, EQ_ARRAY(char, 0x00569FAC)); break;
            case 0x53: strcat(valueText, EQ_ARRAY(char, 0x00569F44)); break;
            case 0x57: strcat(valueText, EQ_ARRAY(char, 0x0055A1CC)); break;
            case 0x58: strcat(valueText, EQ_ARRAY(char, 0x0055A1D0)); break;
            case 0x9c: strcat(valueText, EQ_ARRAY(char, 0x0055A1B0)); break;
            case 0x9d: strcat(valueText, EQ_ARRAY(char, 0x00569F24)); break;
            case 0xb5: strcat(valueText, EQ_ARRAY(char, 0x00569F3C)); break;
            case 0xb8: strcat(valueText, EQ_ARRAY(char, 0x0055A1A0)); break;
            case 0xc5: strcat(valueText, EQ_ARRAY(char, 0x0055A1A8)); break;
            case 0xc7: strcat(valueText, EQ_ARRAY(char, 0x0055A188)); break;
            case 0xc8: strcat(valueText, EQ_ARRAY(char, 0x0055A174)); break;
            case 0xc9: strcat(valueText, EQ_ARRAY(char, 0x0055A178)); break;
            case 0xcb: strcat(valueText, EQ_ARRAY(char, 0x0055A180)); break;
            case 0xcd: strcat(valueText, EQ_ARRAY(char, 0x0055A16C)); break;
            case 0xcf: strcat(valueText, EQ_ARRAY(char, 0x0055A138)); break;
            case 0xd0: strcat(valueText, EQ_ARRAY(char, 0x0055A13C)); break;
            case 0xd1: strcat(valueText, EQ_ARRAY(char, 0x0055A144)); break;
            case 0xd2: strcat(valueText, EQ_ARRAY(char, 0x0055A14C)); break;
            case 0xd3: strcat(valueText, EQ_ARRAY(char, 0x0055A154)); break;
            case 0xdb: strcat(valueText, EQ_ARRAY(char, 0x0055A15C)); break;
            case 0xdc: strcat(valueText, EQ_ARRAY(char, 0x0055A164)); break;
            }
        }
        else
        {
            len = strlen(valueText);
            valueText[len + 1] = '\0';
            len = strlen(valueText);
            valueText[len] = (char)keyChar;
        }
    }
    EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188)(EQ_ARRAY(char, 0x0056A34C), keyName, valueText, EQ_ARRAY(char, 0x0053DB50));
}

// FUNCTION: TRILOGY 0x004c2109
// Restores a gameplay key binding from the client settings.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: VC6 rotates both table-scan loops (element test peeled, iniValue[0] hoisted to CL, strcmpi lea hoisted); the original keeps each test at the loop head.
u32 readKeyMapFromIni(int keymapIndex, int suffixFlag)
{
    int tmp;
    int cmpResult;
    char* afterPrefix;
    size_t valueLen;
    char* charEntry;
    u32 keyCode;
    char** nameEntry;
    CHAR iniKey[256];
    CHAR iniValue[256];
    typedef DWORD (WINAPI *GetPrivateProfileStringA_guess_t)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR);

    keyCode = 0;
    strcpy(iniKey, EQ_ARRAY(char, 0x0056A374));
    strcat(iniKey, EQ_ARRAY(char*, 0x0056999C)[keymapIndex]);
    if (suffixFlag == 1)
    {
        strcat(iniKey, EQ_ARRAY(char, 0x0056A370));
    }
    iniValue[0] = '\0';
    EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C)
        (EQ_ARRAY(char, 0x0056A34C), iniKey, EQ_ARRAY(char, 0x0056A37C), iniValue, 0x100,
         EQ_ARRAY(char, 0x0053DB50));
    tmp = strcmp(iniValue, EQ_ARRAY(char, 0x0056A37C));
    if (tmp == 0)
    {
        keyCode = 0xFFFFFFFE;
    }
    else
    {
        tmp = 3;
        do
        {
            if (_strnicmp(iniValue, EQ_ARRAY(char, 0x0056A368), 6) == 0)
            {
                keyCode = keyCode | 0x10000000;
                afterPrefix = iniValue + 6;
            }
            else if (_strnicmp(iniValue, EQ_ARRAY(char, 0x0056A35C), 8) == 0)
            {
                keyCode = keyCode | 0x20000000;
                afterPrefix = iniValue + 8;
            }
            else if (_strnicmp(iniValue, EQ_ARRAY(char, 0x0056A354), 4) == 0)
            {
                keyCode = keyCode | 0x40000000;
                afterPrefix = iniValue + 4;
            }
            else
            {
                continue;
            }
            strcpy(iniValue, afterPrefix);
        } while (--tmp != 0);
        valueLen = strlen(iniValue);
        if (valueLen == 1)
        {
            tmp = 0;
            charEntry = EQ_ARRAY(char, 0x00569B60);
            do
            {
                if (*charEntry == iniValue[0])
                {
                    keyCode = keyCode + *(int*)(EQ_ARRAY(char, 0x00569B64) + tmp * 8);
                    break;
                }
                charEntry = charEntry + 8;
                tmp = tmp + 1;
            } while ((int)charEntry < 0x00569CD8);
            if (tmp == 0x2f)
            {
                keyCode = 0xFFFFFFFF;
            }
        }
        else
        {
            tmp = 0;
            nameEntry = EQ_ARRAY(char*, 0x00569CDC);
            do
            {
                cmpResult = _strcmpi(*nameEntry, iniValue);
                if (cmpResult == 0)
                {
                    if (EQ_ARRAY(int, 0x00569CD8)[tmp * 2] == -1)
                    {
                        keyCode = 0xFFFFFFFF;
                    }
                    else
                    {
                        keyCode = keyCode + EQ_ARRAY(int, 0x00569CD8)[tmp * 2];
                    }
                    break;
                }
                nameEntry = nameEntry + 2;
                tmp = tmp + 1;
            } while ((int)nameEntry < 0x00569E8C);
            if (tmp == 0x36)
            {
                keyCode = 0xFFFFFFFF;
            }
        }
    }
    return keyCode;
}

// FUNCTION: TRILOGY 0x004c22ba
// Key-press handler: browsing characters at character select, capturing a new keymap, or running bound game commands
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void ProcessKeyDown_guess(int scanCode)
{
    short selectedIndex;
    short selectedClass;
    int scanCodeCharacter;
    int command;
    int index;
    int uiState;
    int itemId;
    u32 keyValue;
    u32 packedClass;
    u32 cameraLook;
    u32 cameraRace;
    u32* binding;
    bool alternateBinding;
    char isEvil;
    char* characterSelect;
    char* characterCreate;
    char* optionState;
    char* clientState;
    char* itemName;
    char itemNameBuffer[16];
    EQPlayer* localPlayer;

    characterSelect = (char*)EQ_GLOBAL(void*, 0x006E4E6C);
    if (characterSelect != 0)
    {
        if (scanCode == 1)
        {
            characterCreate = (char*)EQ_GLOBAL(void*, 0x006E4E70);
            if (characterCreate == 0 || *(int*)(characterCreate + 0x664) != 0)
            {
                *(int*)(characterSelect + 0x94) = 1;
            }
            else
            {
                *(short*)(characterCreate + 0x1BF0) = 0;
                *(int*)(characterCreate + 0x1BE4) = 1;
                *(int*)(characterCreate + 0x0C) = 1;
                EQ_METHOD(1, void, 0x0040E4C6, (void*, int), g_display_guess)(0x5A);
            }
        }
        else if (scanCode == 0x1C || scanCode == 0x9C)
        {
            if (pinstCEverQuest_guess->gameState_guess != 2)
            {
                *(int*)(characterSelect + 0x90) = 1;
            }
        }
        else if ((scanCode == 200 || scanCode == 0xD0) &&
                 pinstCEverQuest_guess->gameState_guess != 2)
        {
            if (scanCode == 0xD0)
            {
                *(short*)(characterSelect + 0x80) = *(short*)(characterSelect + 0x80) + 1;
                itemId = strcmp((char*)EQ_GLOBAL(void*, 0x006EFE2C) + *(short*)(characterSelect + 0x80) * 0x1E + 0x22400,
                     EQ_ARRAY(char, 0x005454F0));
                if (itemId == 0)
                {
                    *(short*)(characterSelect + 0x80) = 0;
                }
            }
            else
            {
                *(short*)(characterSelect + 0x80) = *(short*)(characterSelect + 0x80) - 1;
                if (*(short*)(characterSelect + 0x80) < 0)
                {
                    *(short*)(characterSelect + 0x80) = 7;
                }
            }
            selectedIndex = *(short*)(characterSelect + 0x80);
            do
            {
                itemId = strcmp((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex * 0x1E + 0x22400,
                     EQ_ARRAY(char, 0x005454F0));
                if (itemId != 0)
                    break;
                *(short*)(characterSelect + 0x80) = *(short*)(characterSelect + 0x80) - 1;
                selectedIndex = *(short*)(characterSelect + 0x80);
            } while (selectedIndex >= 0);
            if (*(short*)(characterSelect + 0x80) >= 0)
            {
                selectedIndex = *(short*)(characterSelect + 0x80);
                localPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
                localPlayer->appearance[5] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x2288A);
                localPlayer->appearance[1] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22862);
                localPlayer->appearance[4] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22880);
                localPlayer->appearance[0] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22858);
                localPlayer->appearance[2] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x2286C);
                selectedClass = *(short*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex * 2 + 0x22540);
                localPlayer->appearance[3] = *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22876);
                packedClass = ((u32)(short)(selectedIndex >> 15) << 16) | (u16)(selectedClass - 1);
                cameraLook = (packedClass & 0xFFFFFF00) |
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x2261C);
                cameraRace = ((u32)localPlayer & 0xFFFFFF00) |
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22536);
                EQ_METHOD(4, void, 0x00430381, (void*, u32, u32, int, u32), characterSelect)
                    (cameraLook, packedClass, 0, cameraRace);
                index = 0;
                do
                {
                    localPlayer->equipment[index] = '\0';
                    localPlayer->equip_colors[index] = 0;
                    index++;
                } while (index < 9);
                index = 0;
                do
                {
                    selectedIndex = *(short*)(characterSelect + 0x80);
                    if (index == 0)
                    {
                        EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, int, int),
                            g_display_guess)
                            (localPlayer,
                             *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex * 9 + 0x22630),
                             localPlayer->equipment[0], 0,
                             *(u32*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + (selectedIndex * 9 + 0x89A3) * 4),
                             0);
                    }
                    else
                    {
                        EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, int),
                            g_display_guess)
                            (localPlayer, index,
                             *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + index + selectedIndex * 9 + 0x22630),
                             localPlayer->equipment[index],
                             *(u32*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + (index + (selectedIndex + 0xF4B) * 9) * 4));
                    }
                    index++;
                } while (index < 9);
                EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int),
                    g_display_guess)
                    (localPlayer,
                     *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) +
                         *(short*)(characterSelect + 0x80) + 0x22626),
                     localPlayer->face);
                itemId = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) +
                    *(short*)(characterSelect + 0x80) * 4 + 0x22808);
                if (itemId < 1)
                {
                    itemName = EQ_ARRAY(char, 0x0053D860);
                }
                else
                {
                    sprintf(itemNameBuffer, EQ_ARRAY(char, 0x0053D7D4), itemId);
                    itemName = itemNameBuffer;
                }
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*),
                    g_display_guess)(localPlayer, 7, itemName);
                itemId = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) +
                    *(short*)(characterSelect + 0x80) * 4 + 0x22830);
                if (itemId < 1)
                {
                    itemName = EQ_ARRAY(char, 0x0053D860);
                }
                else
                {
                    sprintf(itemNameBuffer, EQ_ARRAY(char, 0x0053D7D4), itemId);
                    itemName = itemNameBuffer;
                }
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*),
                    g_display_guess)(localPlayer, 8, itemName);
                selectedIndex = *(short*)(characterSelect + 0x80);
                isEvil = EQ_METHOD(3, char, 0x00431509, (void*, int, int, int), characterSelect)
                    (*(short*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex * 2 + 0x22540),
                     *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex + 0x22536),
                     *(short*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + selectedIndex * 2 + 0x227F4));
                localPlayer->y = 0.0f;
                if (isEvil == '\0')
                {
                    localPlayer->x = 0.0f;
                    localPlayer->z = 7.0f;
                }
                else
                {
                    localPlayer->x = -315.0f;
                    localPlayer->z = 6.0f;
                }
                EQ_METHOD(1, void, 0x00410ABF, (void*, EQPlayer*),
                    g_display_guess)(localPlayer);
            }
        }
        scanCodeCharacter = EQ_CDECL(int, 0x004C1B72, (int))(scanCode);
        if (pinstCEverQuest_guess->gameState_guess != 2)
            return;
        characterCreate = (char*)EQ_GLOBAL(void*, 0x006E4E70);
        if (characterCreate == 0)
            return;
        if (*(short*)(characterCreate + 0x1BA8) != 0)
            return;
        EQ_CDECL(void, 0x004EA641, (int))(scanCodeCharacter);
        return;
    }
    optionState = (char*)EQ_GLOBAL(void*, 0x006E4E44);
    if (*(u8*)(optionState + 0x8FBAD) != 0)
    {
        uiState = EQ_METHOD(1, int, 0x004C1B48, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(scanCode);
        if (uiState == 0 && scanCode != 1)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))
                (EQ_ARRAY(char, 0x0056A3F4), 0x0F);
            return;
        }
        if (scanCode == 0x1D || scanCode == 0x2A || scanCode == 0x36 ||
            scanCode == 0x38 || scanCode == 0x9D || scanCode == 0xB8)
        {
            scanCode = -1;
        }
        if (scanCode == -1)
            return;
        if (scanCode == 1)
        {
            alternateBinding = *(int*)(optionState + 0x8FBB0) != 0;
            if (alternateBinding)
            {
                EQ_ARRAY(u32, 0x006E15E4)[*(u8*)(optionState + 0x8FBAD)] = 0xFFFFFFFF;
            }
            else
            {
                EQ_ARRAY(u32, 0x006E11E4)[*(u8*)(optionState + 0x8FBAD)] = 0xFFFFFFFF;
            }
            scanCode = -1;
        }
        else
        {
            if (EQ_GLOBAL(int, 0x006B70EC) != 0)
                scanCode |= 0x20000000;
            if (EQ_GLOBAL(int, 0x006B70E8) != 0)
                scanCode |= 0x10000000;
            if (EQ_GLOBAL(int, 0x006B70F0) != 0)
                scanCode |= 0x40000000;
            alternateBinding = *(int*)(optionState + 0x8FBB0) != 0;
            if (alternateBinding)
            {
                EQ_ARRAY(u32, 0x006E15E4)[*(u8*)(optionState + 0x8FBAD)] = scanCode;
            }
            else
            {
                EQ_ARRAY(u32, 0x006E11E4)[*(u8*)(optionState + 0x8FBAD)] = scanCode;
            }
        }
        EQ_CDECL(void, 0x004C1CAA, (u8, int, int))
            (*(u8*)(optionState + 0x8FBAD), scanCode, alternateBinding);
        *(u8*)(optionState + 0x8FBAD) = 0;
        return;
    }
    scanCodeCharacter = EQ_CDECL(int, 0x004C1B72, (int))(scanCode);
    if (scanCode == 1)
    {
        command = 0xC9;
        goto executeSpecialCommand;
    }
    if (scanCode == 0x0E)
    {
        uiState = EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
        if (uiState == 2)
        {
            EQ_GLOBAL(int, 0x006B65B8) = g_cdisplay_guess->time_ms_guess;
            command = 0xCE;
            goto executeSpecialCommand;
        }
        goto afterSpecialScanCode;
    }
    if (scanCode == 0x1C)
    {
        scanCodeCharacter = 0x0D;
        uiState = EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
        if (uiState == 0 ||
            EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))() == 2)
        {
            command = 0xCF;
            goto executeSpecialCommand;
        }
        goto afterSpecialScanCode;
    }
    if (scanCode == 0x22)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm == '\0' ||
            EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))() != 0 ||
            EQ_GLOBAL(u8, 0x006B6404) == '\0')
        {
            goto afterSpecialScanCode;
        }
        if (EQ_GLOBAL(int, 0x006B70E8) != 0)
            goto afterSpecialScanCode;
        if (EQ_GLOBAL(int, 0x006B70F0) != 0)
            goto afterShift2;
        if (EQ_GLOBAL(int, 0x006B70EC) == 0)
        {
            scanCodeCharacter = -1;
            clientState = (char*)EQ_GLOBAL(void*, 0x006E4DF4);
            if (*(int*)(clientState + 0x10) == 0)
            {
                *(int*)(clientState + 0x8C) = 0;
                __asm
                {
                    mov ecx, clientState
                    mov eax, [ecx]
                    call dword ptr [eax + 4]
                }
            }
            else if (*(int*)(clientState + 0x8C) == 0)
            {
                __asm
                {
                    mov ecx, clientState
                    mov eax, [ecx]
                    call dword ptr [eax + 8]
                }
            }
            else
            {
                if (*(int*)(clientState + 0x8C) == 4)
                    itemName = EQ_ARRAY(char, 0x0056A3BC);
                else
                    itemName = EQ_ARRAY(char, 0x0056A384);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                    EQ_GLOBAL(void*, 0x006EFE2C))(itemName, 0x0D);
            }
            goto afterSpecialScanCode;
        }
        goto afterShift2;
    }
    if (scanCode != 0x35)
        goto afterSpecialScanCode;
    if (EQ_GLOBAL(u8, 0x006B6404) == '\0')
        goto afterSpecialScanCode;
    if (EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))() != 0)
        goto afterSpecialScanCode;
    command = 200;
    goto executeSpecialCommand;

executeSpecialCommand:
    scanCodeCharacter = -1;
    EQ_CDECL(void, 0x004D7D6A, (int, int, int))(command, 1, 0);
    goto afterSpecialScanCode;

afterSpecialScanCode:
    if (EQ_GLOBAL(int, 0x006B70E8) != 0)
        scanCode |= 0x10000000;
    if (EQ_GLOBAL(int, 0x006B70EC) == 0)
        goto afterShift2;
    scanCode |= 0x20000000;
afterShift2:
    if (EQ_GLOBAL(int, 0x006B70F0) != 0)
        scanCode |= 0x40000000;
    uiState = EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
    if (uiState != 0 && scanCodeCharacter != 0)
        goto finishMovementKey;
    keyValue = (u32)scanCode & 0xD00000FF;
    if (keyValue == EQ_GLOBAL(u32, 0x006E11F8) || keyValue == EQ_GLOBAL(u32, 0x006E15F8))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(5, 1, 0);
    if (keyValue == EQ_GLOBAL(u32, 0x006E11FC) || keyValue == EQ_GLOBAL(u32, 0x006E15FC))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(6, 1, 0);
    if (keyValue == EQ_GLOBAL(u32, 0x006E11F4) || keyValue == EQ_GLOBAL(u32, 0x006E15F4))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(4, 1, 0);
    if (keyValue == EQ_GLOBAL(u32, 0x006E11F0) || keyValue == EQ_GLOBAL(u32, 0x006E15F0))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(3, 1, 0);
    keyValue = (u32)scanCode & 0xB00000FF;
    if (keyValue == EQ_GLOBAL(u32, 0x006E122C) || keyValue == EQ_GLOBAL(u32, 0x006E162C))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x12, 1, 0);
    if (keyValue == EQ_GLOBAL(u32, 0x006E1230) || keyValue == EQ_GLOBAL(u32, 0x006E1630))
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x13, 1, 0);
    EQ_GLOBAL(int, 0x006E1B28) = 0;
    EQ_GLOBAL(int, 0x006E1B2C) = 0;
    if (scanCode == EQ_GLOBAL(int, 0x006E1328) || scanCode == EQ_GLOBAL(int, 0x006E1728))
    {
        EQ_GLOBAL(int, 0x006E1B28) = 1;
    }
    else if (scanCode == EQ_GLOBAL(int, 0x006E132C) || scanCode == EQ_GLOBAL(int, 0x006E172C))
    {
        EQ_GLOBAL(int, 0x006E1B2C) = 1;
    }
    else
    {
        index = 0;
        binding = EQ_ARRAY(u32, 0x006E15E4);
        do
        {
            if ((u32)scanCode == binding[-0x100] || (u32)scanCode == *binding)
            {
                EQ_CDECL(void, 0x004D7D6A, (int, int, int))(index, 1, 0);
                scanCodeCharacter = -1;
            }
            binding++;
            index++;
        } while ((int)binding < 0x006E1789);
    }
finishMovementKey:
    if (scanCodeCharacter != -1 &&
        pinstCEverQuest_guess->gameState_guess == 5 &&
        ((EQ_GLOBAL(void*, 0x006E4DE8) != 0 &&
          *((char*)EQ_GLOBAL(void*, 0x006E4DE8) + 0x1F1) != '\0') ||
         (EQ_GLOBAL(void*, 0x006E4E44) != 0 &&
          (pinstEQ_Main_guess->keyCaptureActive_guess != 0 ||
           pinstEQ_Main_guess->socialEditorOpen_guess != 0 ||
           pinstEQ_Main_guess->screenMode_guess > 7)) ||
         EQ_METHOD(0, int, 0x004D7CA7, (void*), EQ_GLOBAL(void*, 0x006EFE2C))() != 0))
    {
        EQ_CDECL(void, 0x004E9DFB, (int))(scanCodeCharacter);
    }
    return;
}

// FUNCTION: TRILOGY 0x004C2C1A
// Turns mouse presses, releases and moves into game clicks each frame; a locked UI only gets a warning.
// __cdecl, no arguments. Per-frame mouse poll for game states 1, 2 and 5 (5 not while feigned). Button
// edges and moves go to the CEverQuest at 0x006EFE2C. Each handler's argument high word is the high word
// of EAX left by the last call or 32-bit load, so the handlers that return a value are declared int.
void PollMouseInput_guess(void)
{
    int tmp;
    u32 now;
    u16 eaxHigh;

    tmp = pinstCEverQuest_guess->gameState_guess;
    if (((tmp == 2) || (tmp == 1)) || (tmp == 5))
    {
        if ((EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == NULL) || (tmp != 5) ||
            (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->stand_state != 0x66))
        {
            if (EQ_GLOBAL(u8, 0x006E20F4) == 0)
            {
                now = EQ_CDECL(u32, 0x004A461E, (void))();
                if (EQ_GLOBAL(u32, 0x006E4E6C) == 0)
                {
                    tmp = EQ_METHOD(1, int, 0x004199C0, (void*, int), EQ_GLOBAL(void*, 0x006E4DA8))(0);
                    if (tmp < 1)
                        return;
                }
                EQ_CDECL(void, 0x004EB36E, (void))();
                EQ_GLOBAL(u16, 0x006B6418) = EQ_GLOBAL(u16, 0x006EFC88);
                EQ_GLOBAL(u16, 0x006B6414) = (u16)EQ_GLOBAL(int, 0x006EFC80);
                EQ_GLOBAL(u16, 0x006B6416) = (u16)EQ_GLOBAL(int, 0x006EFC84);
                EQ_GLOBAL(u16, 0x006B641A) = EQ_GLOBAL(u16, 0x006EFC8C);
                if ((EQ_GLOBAL(int, 0x006B6480) != EQ_GLOBAL(int, 0x006EFC80)) ||
                    ((tmp = EQ_GLOBAL(int, 0x006EFC80)), (EQ_GLOBAL(int, 0x006B6484) != EQ_GLOBAL(int, 0x006EFC84))))
                {
                    EQ_GLOBAL(int, 0x006B676C) = 0;
                    tmp = EQ_CDECL(int, 0x004A461E, (void))();
                    EQ_GLOBAL(int, 0x006B6764) = tmp;
                }
                eaxHigh = (u16)((u32)tmp >> 16);
                if (EQ_GLOBAL(u8, 0x006B648B) == 0)
                {
                    if (EQ_GLOBAL(u8, 0x006B6488) != 0)
                    {
                        EQ_GLOBAL(u8, 0x006B6408) = 0;
                        if (EQ_GLOBAL(int, 0x006B649C) == 0)
                        {
                            tmp = EQ_METHOD(2, int, 0x004CBA05, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                                 (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC80)),
                                 (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC84)));
                        }
                        else
                        {
                            if (EQ_GLOBAL(void*, 0x006E4E20) != NULL)
                            {
                                pinstEQ_Requestor_guess->captureButtonResult_guess = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0xA4) = 0xFFFF;
                            }
                            if (EQ_GLOBAL(void*, 0x006E4E44) != NULL)
                            {
                                pinstEQ_Main_guess->editLayout_guess = 0;
                                pinstEQ_Main_guess->releasedButton_u16_guess =
                                    pinstEQ_Main_guess->pressedButton_u16_guess;
                                pinstEQ_Main_guess->pressedButton_u16_guess = 0xFFFF;
                                pinstEQ_Main_guess->clickedButton_u16_guess = 0xFFFF;
                                pinstEQ_Main_guess->skillListThumbDrag_guess = 0;
                                pinstEQ_Main_guess->trainSkillThumbDrag_guess = 0;
                                pinstEQ_Main_guess->skillWindowThumbDrag_guess = 0;
                                pinstEQ_Main_guess->trackThumbDrag_guess = 0;
                            }
                            if (EQ_GLOBAL(void*, 0x006E4E5C) != NULL)
                            {
                                pinstEQ_Bag2_guess->mouseDown_guess = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x94) = 0xFFFF;
                            }
                            if (EQ_GLOBAL(void*, 0x006E4E58) != NULL)
                            {
                                pinstEQ_Bag1_guess->mouseDown_guess = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x94) = 0xFFFF;
                            }
                            if (EQ_GLOBAL(void*, 0x006E4E60) != NULL)
                            {
                                *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E60) + 0x90) = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4E60) + 0x84) = 0xFFFF;
                            }
                            if (EQ_GLOBAL(void*, 0x00604984) != NULL)
                            {
                                *(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x1C8) = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x1CC) = 0xFFFF;
                                *(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x1C4) = 0;
                            }
                            if (EQ_GLOBAL(void*, 0x006018C0) != NULL)
                            {
                                *(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x1C8) = 0;
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x1CC) = 0xFFFF;
                                *(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x1C4) = 0;
                            }
                            tmp = (int)EQ_GLOBAL(void*, 0x006E4DE8);
                            if (EQ_GLOBAL(void*, 0x006E4DE8) != NULL)
                            {
                                *(u16*)((char*)EQ_GLOBAL(void*, 0x006E4DE8) + 0x1E2) = 0xFFFF;
                                pinstEQ_TextIO_guess->scrollThumbDragging_guess = 0;
                                pinstEQ_TextIO_guess->unk_0x1E6 = 0;
                                pinstEQ_TextIO_guess->unk_0x1E4 = 0xFFFF;
                                tmp = (int)EQ_GLOBAL(void*, 0x006E4DE8);
                                pinstEQ_TextIO_guess->mouseDispatchActive_guess = 0;
                            }
                        }
                        eaxHigh = (u16)((u32)tmp >> 16);
                        EQ_GLOBAL(int, 0x006B649C) = 0;
                        EQ_GLOBAL(int, 0x006B64A4) = 0;
                    }
                }
                else if (EQ_GLOBAL(u8, 0x006B6488) == 0)
                {
                    EQ_GLOBAL(int, 0x006B64A4) = now;
                    tmp = EQ_METHOD(2, int, 0x004DC615, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                         (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC80)),
                         (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC84)));
                    eaxHigh = (u16)((u32)tmp >> 16);
                }
                EQ_GLOBAL(u8, 0x006B6488) = EQ_GLOBAL(u8, 0x006B648B);
                if (EQ_GLOBAL(u8, 0x006B648A) == 0)
                {
                    if (EQ_GLOBAL(u8, 0x006B6489) != 0)
                    {
                        if ((EQ_GLOBAL(u8, 0x006B645A) != 0) || (EQ_GLOBAL(int, 0x006B6498) == 0))
                        {
                            tmp = EQ_METHOD(2, int, 0x004DCBAD, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                                 (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC80)),
                                 (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC84)));
                            eaxHigh = (u16)((u32)tmp >> 16);
                        }
                        EQ_GLOBAL(int, 0x006B6498) = 0;
                        EQ_GLOBAL(int, 0x006B64A0) = 0;
                    }
                }
                else if (EQ_GLOBAL(u8, 0x006B6489) == 0)
                {
                    EQ_GLOBAL(int, 0x006B64A0) = now;
                    tmp = EQ_METHOD(2, int, 0x004DC9E6, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                         (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC80)),
                         (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC84)));
                    eaxHigh = (u16)((u32)tmp >> 16);
                }
                EQ_GLOBAL(u8, 0x006B6489) = EQ_GLOBAL(u8, 0x006B648A);
                EQ_METHOD(2, void, 0x004DC521, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                     (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC80)),
                     (int)(((u32)eaxHigh << 16) | (u16)EQ_GLOBAL(int, 0x006EFC84)));
            }
            else if ((EQ_GLOBAL(u8, 0x006B648B) != 0) || (EQ_GLOBAL(u8, 0x006B648A) != 0))
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x00562588), 0x111);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004c2f7c
// Clears pending input before the client continues processing.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl. Under the critical section at 0x006E3388, zeroes the two queue indices at 0x006018E0/E4 and the
// first dword of each of the 1024 twelve-byte entries at 0x006018EC.
void ClearInputQueue_guess(void)
{
    u32* entry;

    EnterCriticalSection(g_input_queue_lock_guess);
    entry = g_input_queue_guess;
    g_input_queue_head_guess = 0;
    g_input_queue_tail_guess = 0;
    do
    {
        *entry = 0;
        entry += 3;
    } while ((int)entry < 0x006048EC);
    LeaveCriticalSection(g_input_queue_lock_guess);
}

// FUNCTION: TRILOGY 0x004c2fb1
// @early-stop: EBP frame, register allocation, import-load timing and EndDialog tail merging still differ.
// __thiscall dialog procedure. Initializes the hardware dialog and saves accepted field values.
int __fastcall HardwareDialogProc_guess(char* dialogConfig, int unusedEdx, HWND dialogWindow, int windowMessage, short notificationCode, int unusedParameter)
{
    typedef HWND (WINAPI *GetDlgItemProc_guess)(HWND, int);
    typedef int (WINAPI *GetWindowTextAProc_guess)(HWND, LPSTR, int);
    typedef BOOL (WINAPI *SetWindowTextAProc_guess)(HWND, LPCSTR);
    typedef LRESULT (WINAPI *SendMessageAProc_guess)(HWND, UINT, WPARAM, LPARAM);
    typedef HWND (WINAPI *SetFocusProc_guess)(HWND);
    typedef BOOL (WINAPI *EndDialogProc_guess)(HWND, INT_PTR);
    typedef int (__cdecl *GetCPUSpeedProc_guess)(void);
    typedef LPCSTR (__cdecl *GetCPUNameProc_guess)(void);
    GetDlgItemProc_guess get_dlg_item;
    GetWindowTextAProc_guess get_window_text;
    SetWindowTextAProc_guess set_window_text;
    SendMessageAProc_guess send_message;
    HWND pHVar1;
    LPCSTR pCVar3;
    INT_PTR nResult;
    int itemIndex;
    char cpuSpeedText[12];

    switch (windowMessage)
    {
    case 0x110:
        get_dlg_item = EQ_GLOBAL(GetDlgItemProc_guess, 0x00534254);
        (set_window_text = EQ_GLOBAL(SetWindowTextAProc_guess, 0x00534258))
            (get_dlg_item(dialogWindow, 0x3ed), dialogConfig + 4);
        set_window_text(get_dlg_item(dialogWindow, 0x3ee), dialogConfig + 0x84);
        set_window_text(get_dlg_item(dialogWindow, 0x3ef), dialogConfig + 0x104);
        set_window_text(get_dlg_item(dialogWindow, 0x3ea), dialogConfig + 0x184);
        // The CPU query takes no arguments; the pending buffer and radix belong to itoa.
        pCVar3 = EQ_CDECL(char*, 0x0053162B, (int, char*, int))
            (EQ_GLOBAL(GetCPUSpeedProc_guess, 0x006E52A4)(), cpuSpeedText, 10);
        set_window_text(get_dlg_item(dialogWindow, 0x3f1), pCVar3);
        set_window_text(get_dlg_item(dialogWindow, 0x3f2),
            EQ_GLOBAL(GetCPUNameProc_guess, 0x006E52A0)());
        pHVar1 = get_dlg_item(dialogWindow, 0x3ed);
        EQ_GLOBAL(SetFocusProc_guess, 0x00534228)(pHVar1);
        pHVar1 = get_dlg_item(dialogWindow, 0x3e9);
        send_message = EQ_GLOBAL(SendMessageAProc_guess, 0x00534244);
        itemIndex = 0;
        if (0 < EQ_GLOBAL(int, 0x0058FBB4))
        {
            dialogWindow = (HWND)EQ_ARRAY(char, 0x005901B8);
            do
            {
                send_message(pHVar1, 0x143, 0, (LPARAM)dialogWindow);
                dialogWindow = (HWND)((int)dialogWindow + 0x15c);
                itemIndex = itemIndex + 1;
            } while (itemIndex < EQ_GLOBAL(int, 0x0058FBB4));
        }
        send_message(pHVar1, 0x14e, 0, 0);
        break;
    case 0x111:
        switch ((unsigned short)notificationCode)
        {
        case 1:
            get_dlg_item = EQ_GLOBAL(GetDlgItemProc_guess, 0x00534254);
            get_window_text = EQ_GLOBAL(GetWindowTextAProc_guess, 0x00534224);
            get_window_text(get_dlg_item(dialogWindow, 0x3ed), dialogConfig + 4, 0x80);
            get_window_text(get_dlg_item(dialogWindow, 0x3ee), dialogConfig + 0x84, 0x80);
            get_window_text(get_dlg_item(dialogWindow, 0x3ef), dialogConfig + 0x104, 0x80);
            get_window_text(get_dlg_item(dialogWindow, 0x3ea), dialogConfig + 0x184, 0x80);
            *(int*)(dialogConfig + 0x204) = 9000;
            pHVar1 = get_dlg_item(dialogWindow, 0x3e9);
            EQ_GLOBAL(int, 0x00590180) = (int)EQ_GLOBAL(SendMessageAProc_guess, 0x00534244)(pHVar1, 0x147, 0, 0);
            nResult = 1;
            EQ_GLOBAL(EndDialogProc_guess, 0x0053425C)(dialogWindow, nResult);
            break;
        case 2:
            nResult = 0;
            EQ_GLOBAL(EndDialogProc_guess, 0x0053425C)(dialogWindow, nResult);
            break;
        }
        break;
    }
    return 0;
}

// The 8-byte FilePath that PackFS::addFileSystem (0x004F8D80) takes by value.
class SoundPackPath_guess
{
public:
    int     data;
    int     unknown0x04;

    SoundPackPath_guess(const char* name)
    {
        EQ_METHOD(1, void, 0x004C479D, (void*, const char*), this)(name);
    }
};

// Heap objects the constructor allocates with the client's operator new (0x005218C6).
template<unsigned SIZE> class EverQuestAllocation_guess
{
public:
    char storage[SIZE];
};

// The sound engine returned by GetAudioInterface (0x004ECB4D).
class AudioInterface_guess
{
public:
    virtual void Unknown0_guess(void) = 0;
    virtual bool Initialize_guess(u32* window, u32 flags) = 0;
};

struct CEverQuest_ctor_type_guess : public CEverQuest   // the constructor's stand-in has the class layout
{
    CEverQuest_ctor_type_guess* CEverQuest_guess(u32 window);
};

typedef DWORD (WINAPI *GetProfileStringProc_guess)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR);
typedef BOOL (WINAPI *WriteProfileStringProc_guess)(LPCSTR, LPCSTR, LPCSTR, LPCSTR);
typedef char EverQuestChatLine_guess[0x100];
typedef char EverQuestName32_guess[0x20];
typedef char EverQuestBlock_guess[0x800];

#define WRITE_PROFILE_STRING        EQ_GLOBAL(WriteProfileStringProc_guess, 0x00534188)
#define EQCLIENT_INI                EQ_ARRAY(const char, 0x0053DB50)
#define INI_DEFAULTS                EQ_ARRAY(const char, 0x0053DB3C)
#define INI_MISSING                 EQ_ARRAY(const char, 0x0053DB48)
#define INI_TRUE                    EQ_ARRAY(const char, 0x00563F84)
#define INI_FALSE                   EQ_ARRAY(const char, 0x0053DAFC)
#define INI_ZERO                    EQ_ARRAY(const char, 0x0053D860)
#define INI_ONE                     EQ_ARRAY(const char, 0x005644D4)
#define INI_NORMAL                  EQ_ARRAY(const char, 0x00558BAC)
#define KEY_INSPECT_OTHERS          EQ_ARRAY(const char, 0x00564AD0)
#define KEY_LOG                     EQ_ARRAY(const char, 0x0056A87C)
#define KEY_ATTACK_ON_ASSIST        EQ_ARRAY(const char, 0x0056578C)
#define KEY_SHOW_INSPECT_MESSAGE    EQ_ARRAY(const char, 0x0056A850)
#define KEY_LOCK_WINDOWS            EQ_ARRAY(const char, 0x0056A828)
#define KEY_INV_WIN_LABELS          EQ_ARRAY(const char, 0x00565890)
#define KEY_HIDE_PLAYER_WIN         EQ_ARRAY(const char, 0x0056A804)
#define KEY_HIDE_PARTY_WIN          EQ_ARRAY(const char, 0x0056A7F4)
#define KEY_HIDE_TARGET_WIN         EQ_ARRAY(const char, 0x0056A7E4)
#define KEY_HIDE_SPELLS_WIN         EQ_ARRAY(const char, 0x0056A7D4)
#define KEY_HIDE_BUFF_WIN           EQ_ARRAY(const char, 0x0056A7C8)
#define KEY_HIDE_HOTBOX_WIN         EQ_ARRAY(const char, 0x0056A7B8)
#define KEY_HIDE_CHAT_WIN           EQ_ARRAY(const char, 0x0056A7AC)
#define KEY_HIDE_MAIN_MENU_WIN      EQ_ARRAY(const char, 0x0056A79C)
#define KEY_CHAT_LINE_SPACING       EQ_ARRAY(const char, 0x0056A78C)
#define KEY_SHOW_DYNAMIC_LIGHTS     EQ_ARRAY(const char, 0x00565820)
#define KEY_CHAT_FONT_SIZE          EQ_ARRAY(const char, 0x00565860)
#define KEY_VIDEO_MODE              EQ_ARRAY(const char, 0x0055B23C)
#define KEY_MOUSE_POINTER_SPEED_MOD EQ_ARRAY(const char, 0x005643C8)
#define KEY_SHOW_SPELL_EFFECTS      EQ_ARRAY(const char, 0x005644D8)
#define KEY_SERVER_FILTER           EQ_ARRAY(const char, 0x0056610C)
#define KEY_TRACK_PLAYERS           EQ_ARRAY(const char, 0x00563F74)
#define KEY_TRACK_SORT_TYPE         EQ_ARRAY(const char, 0x0056406C)
#define KEY_TRACK_FILTER_TYPE       EQ_ARRAY(const char, 0x0056A77C)
#define EMPTY_STRING                EQ_ARRAY(const char, 0x0058FBA4)
#define PACKFS_INSTANCE             EQ_ARRAY(char, 0x006ECCF4)
#define g_new_zone_guess            EQ_GLOBAL(MsgNewZone, 0x006B65DC)

#define CEverQuest CEverQuest_ctor_type_guess
class SkillData_guess : public EverQuestAllocation_guess<0x114>
{
public:
    // EXTERNAL: TRILOGY 0x004825B5 SkillData_guess::SkillData_guess
    SkillData_guess(int skill);
};

class SpellData_guess : public EverQuestAllocation_guess<0x260>
{
public:
    // EXTERNAL: TRILOGY 0x004876A8 SpellData_guess::SpellData_guess
    SpellData_guess(int spell);
};

class DisplayAllocation_guess : public EverQuestAllocation_guess<0x33B8>
{
public:
    // EXTERNAL: TRILOGY 0x00405B18 DisplayAllocation_guess::DisplayAllocation_guess
    DisplayAllocation_guess(u32 window);
};

class TimedPairAllocation_guess : public EverQuestAllocation_guess<0x14>
{
public:
    // EXTERNAL: TRILOGY 0x004BE706 TimedPairAllocation_guess::TimedPairAllocation_guess
    TimedPairAllocation_guess(void);
};

class Direct3DRMAllocation_guess : public EverQuestAllocation_guess<0xC>
{
public:
    // EXTERNAL: TRILOGY 0x004E3F12 Direct3DRMAllocation_guess::Direct3DRMAllocation_guess
    Direct3DRMAllocation_guess(void);
};

class SoundTableAllocation_guess : public EverQuestAllocation_guess<0x104C>
{
public:
    // EXTERNAL: TRILOGY 0x004E313F SoundTableAllocation_guess::SoundTableAllocation_guess
    SoundTableAllocation_guess(void);
};

class SoundManagerAllocation_guess : public EverQuestAllocation_guess<0x374>
{
public:
    // EXTERNAL: TRILOGY 0x004E1E81 SoundManagerAllocation_guess::SoundManagerAllocation_guess
    SoundManagerAllocation_guess(void);
};

class MidiAllocation_guess : public EverQuestAllocation_guess<0x6C>
{
public:
    // EXTERNAL: TRILOGY 0x004E4AE4 MidiAllocation_guess::MidiAllocation_guess
    MidiAllocation_guess(void);
};

class MidiSequenceAllocation_guess : public EverQuestAllocation_guess<0x58>
{
public:
    // EXTERNAL: TRILOGY 0x004E4482 MidiSequenceAllocation_guess::MidiSequenceAllocation_guess
    MidiSequenceAllocation_guess(void);
};

// FUNCTION: TRILOGY 0x004C3176
// Constructor for CEverQuest; the argument is the window handle.
// Phases: allocate and clear the 0x7ef4-byte buffer at 0x006e20cc, clear the 25 petition entries and globals,
// read each default setting from the client ini file (a missing key is written back),
// clear the fixed global tables, then create 100 SkillData and 3000 SpellData objects.
// Finally creates the display manager (a failure sets *this = 1), the timed pair, and the sound system, midi
// objects and sound packs; 0x006efe1c is set when audio is unavailable.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
CEverQuest* CEverQuest::CEverQuest_guess(u32 window)
{
    GetProfileStringProc_guess getProfileString;
    EverQuestEntry_guess* entry;
    void* sequence;
    int value;
    int index;
    char buffer[32];

    EQ_GLOBAL(int, 0x006E20D0) = 0x7EF4;
    EQ_GLOBAL(char*, 0x006E20CC) = (char*)EQ_CDECL(void*, 0x00522FA3, (size_t))(0x7EF4);
    if (EQ_GLOBAL(char*, 0x006E20CC) == 0)
    {
        DbgLog_guess(EQ_ARRAY(const char, 0x0056A884));
        EQ_GLOBAL(char*, 0x006E20D8) = 0;
        EQ_GLOBAL(char*, 0x006E20E0) = 0;
        EQ_GLOBAL(char*, 0x006E20DC) = 0;
        EQ_GLOBAL(char*, 0x006E20D4) = 0;
    }
    else
    {
        EQ_GLOBAL(char*, 0x006E20D8) = EQ_GLOBAL(char*, 0x006E20CC) + 4;
        EQ_GLOBAL(char*, 0x006E20DC) = EQ_GLOBAL(char*, 0x006E20D8);
        EQ_GLOBAL(char*, 0x006E20E0) = EQ_GLOBAL(char*, 0x006E20CC);
        memset(
            EQ_GLOBAL(char*, 0x006E20CC), 0, EQ_GLOBAL(int, 0x006E20D0));
        EQ_GLOBAL(char*, 0x006E20D4) = EQ_GLOBAL(char*, 0x006E20CC) + EQ_GLOBAL(int, 0x006E20D0);
    }
    EQ_GLOBAL(void*, 0x006E4D8C) = 0;
    EQ_GLOBAL(void*, 0x006E4DA8) = 0;
    this->petitionKind_guess = 0;
    this->petitionCount_guess = 0;
    entry = &this->petitions_guess[0];
    for (index = 0; index < 25; index++)
    {
        entry[index].id = -1;
        entry[index].rank_guess = 0;
        entry[index].flag_guess = 0;
        entry[index].sequence_guess = 0;
        entry[index].petitionerName_guess[0] = 0;
        entry[index].text0x30_guess[0] = 0;
        entry[index].text0x54_guess[0] = 0;
    }
    EQ_GLOBAL(int, 0x006B6480) = -1;
    EQ_GLOBAL(int, 0x006B6484) = -1;
    EQ_GLOBAL(int, 0x006E2058) = 0;
    g_pcloudFlagA_guess = 0;
    EQ_GLOBAL(int, 0x006E2064) = 0;
    EQ_GLOBAL(int, 0x006E2068) = 0;
    EQ_GLOBAL(char, 0x006B6488) = 0;
    EQ_GLOBAL(char, 0x006B6489) = 0;
    EQ_GLOBAL(char, 0x006B648B) = 0;
    EQ_GLOBAL(char, 0x006B648A) = 0;
    EQ_GLOBAL(int, 0x006E20EC) = 0;
    EQ_GLOBAL(int, 0x006E20F0) = 0;
    EQ_GLOBAL(char, 0x006E20F4) = 0;
    strcpy(
        EQ_ARRAY(char, 0x006B7128), EQ_ARRAY(const char, 0x0056A880));
    EQ_GLOBAL(short, 0x006B7528) = 1;
    EQ_GLOBAL(char, 0x006B752A) = 0;

    getProfileString = EQ_GLOBAL(GetProfileStringProc_guess, 0x0053418C);
    getProfileString(INI_DEFAULTS, KEY_INSPECT_OTHERS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_INSPECT_OTHERS, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20E4) = value;

    getProfileString(INI_DEFAULTS, KEY_LOG, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_LOG, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
    {
        value = 1;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(const char, 0x00564D64));
    }
    g_bazaar_log_on_guess = value;

    getProfileString(INI_DEFAULTS, KEY_ATTACK_ON_ASSIST, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_ATTACK_ON_ASSIST, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
    {
        value = 0;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(const char, 0x0056A864));
    }
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E206C) = value;

    getProfileString(INI_DEFAULTS, KEY_SHOW_INSPECT_MESSAGE, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_SHOW_INSPECT_MESSAGE, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
    {
        value = 0;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(const char, 0x0056A834));
    }
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E2070) = value;

    getProfileString(INI_DEFAULTS, KEY_LOCK_WINDOWS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_LOCK_WINDOWS, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
    {
        value = 1;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
            EQ_ARRAY(const char, 0x0056A814));
    }
    EQ_GLOBAL(int, 0x006E2094) = value;

    getProfileString(INI_DEFAULTS, KEY_INV_WIN_LABELS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_INV_WIN_LABELS, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E2074) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_PLAYER_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_PLAYER_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E2098) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_PARTY_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_PARTY_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E209C) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_TARGET_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_TARGET_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20A0) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_SPELLS_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_SPELLS_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20A4) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_BUFF_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_BUFF_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20A8) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_HOTBOX_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_HOTBOX_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20AC) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_CHAT_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_CHAT_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20B0) = value;

    getProfileString(INI_DEFAULTS, KEY_HIDE_MAIN_MENU_WIN, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_HIDE_MAIN_MENU_WIN, INI_FALSE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20B4) = value;

    getProfileString(INI_DEFAULTS, KEY_CHAT_LINE_SPACING, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        EQ_GLOBAL(int, 0x006B650C) = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_CHAT_LINE_SPACING, INI_ZERO, EQCLIENT_INI);
    }
    else
        EQ_GLOBAL(int, 0x006B650C) = atoi(buffer);

    getProfileString(INI_DEFAULTS, KEY_SHOW_DYNAMIC_LIGHTS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_SHOW_DYNAMIC_LIGHTS, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
        value = 0;
    else
        value = 1;
    EQ_GLOBAL(int, 0x006E20B8) = value;

    getProfileString(INI_DEFAULTS, KEY_CHAT_FONT_SIZE, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        EQ_GLOBAL(int, 0x006E20BC) = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_CHAT_FONT_SIZE, INI_ONE, EQCLIENT_INI);
    }
    else
        EQ_GLOBAL(int, 0x006E20BC) = atoi(buffer);

    getProfileString(INI_DEFAULTS, KEY_VIDEO_MODE, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*'
        || (EQ_GLOBAL(int, 0x006E20C0) = atoi(buffer)) < 0
        || EQ_GLOBAL(int, 0x006E20C0) > 4)
    {
        EQ_GLOBAL(int, 0x006E20C0) = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_VIDEO_MODE, INI_ZERO, EQCLIENT_INI);
    }

    getProfileString(INI_DEFAULTS, KEY_MOUSE_POINTER_SPEED_MOD, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*'
        || (EQ_GLOBAL(int, 0x006B6420) = atoi(buffer)) < 0
        || EQ_GLOBAL(int, 0x006B6420) > 10)
    {
        EQ_GLOBAL(int, 0x006B6420) = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_MOUSE_POINTER_SPEED_MOD, INI_ZERO, EQCLIENT_INI);
    }

    getProfileString(INI_DEFAULTS, KEY_SHOW_SPELL_EFFECTS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*'
        || (EQ_GLOBAL(int, 0x006E20E8) = atoi(buffer)) < 0
        || EQ_GLOBAL(int, 0x006E20E8) > 1)
    {
        EQ_GLOBAL(int, 0x006E20E8) = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_SHOW_SPELL_EFFECTS, INI_ONE, EQCLIENT_INI);
    }

    getProfileString(INI_DEFAULTS, KEY_SERVER_FILTER, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*'
        || (EQ_GLOBAL(int, 0x006B6424) = atoi(buffer)) < 0
        || EQ_GLOBAL(int, 0x006B6424) > 1)
    {
        EQ_GLOBAL(int, 0x006B6424) = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_SERVER_FILTER, INI_ZERO, EQCLIENT_INI);
    }

    getProfileString(INI_DEFAULTS, KEY_TRACK_PLAYERS, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 1;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_TRACK_PLAYERS, INI_TRUE, EQCLIENT_INI);
    }
    else if (buffer[0] != 'T' && buffer[0] != 't' && buffer[0] != '1')
    {
        if (buffer[0] != 'G' && buffer[0] != 'g' && buffer[0] != '2')
            value = 0;
        else
            value = 2;
    }
    else
        value = 1;
    EQ_GLOBAL(int, 0x006B6758) = value;

    getProfileString(INI_DEFAULTS, KEY_TRACK_SORT_TYPE, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        value = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_TRACK_SORT_TYPE, INI_NORMAL, EQCLIENT_INI);
    }
    else if (buffer[0] == 'N' || buffer[0] == 'n' || buffer[0] == '0')
        value = 0;
    else if (buffer[0] == 'D' || buffer[0] == 'd' || buffer[0] == '1')
        value = 1;
    else if (buffer[0] == 'C' || buffer[0] == 'c' || buffer[0] == '2')
        value = 2;
    else if (buffer[0] == 'R' || buffer[0] == 'r')
    {
        if (buffer[1] == 'D' || buffer[1] == 'd')
            value = 3;
        else if (buffer[1] == 'C' || buffer[1] == 'c')
            value = 4;
    }
    else if (buffer[0] == '3')
        value = 3;
    else if (buffer[0] == '4')
        value = 4;
    else
        value = 0;
    EQ_GLOBAL(int, 0x006B675C) = value;

    getProfileString(INI_DEFAULTS, KEY_TRACK_FILTER_TYPE, INI_MISSING, buffer, 0x20, EQCLIENT_INI);
    if (buffer[0] == '*')
    {
        EQ_GLOBAL(int, 0x006B6760) = 0;
        WRITE_PROFILE_STRING(INI_DEFAULTS, KEY_TRACK_FILTER_TYPE, INI_ZERO, EQCLIENT_INI);
    }
    else
        EQ_GLOBAL(int, 0x006B6760) = atoi(buffer);

    EQ_GLOBAL(int, 0x006E20C4) = 0;
    EQ_GLOBAL(float, 0x006E20C8) = 250.0f;
    EQ_GLOBAL(int, 0x006B63EC) = 0;
    EQ_GLOBAL(int, 0x006B7108) = 0;
    EQ_GLOBAL(int, 0x006B648C) = -1;
    EQ_GLOBAL(char, 0x006E1E0C) = 0;
    EQ_GLOBAL(char, 0x006E1E2C) = 0;
    EQ_GLOBAL(char, 0x006E1E4C) = 0;
    EQ_GLOBAL(char, 0x006E1E6C) = 0;
    EQ_GLOBAL(char, 0x006E1E8C) = 0;
    EQ_GLOBAL(char, 0x006E1EAC) = 0;
    EQ_GLOBAL(char, 0x006E1ECC) = 0;
    EQ_GLOBAL(char, 0x006E1EEC) = 0;
    EQ_GLOBAL(char, 0x006E1F0C) = 0;
    EQ_GLOBAL(char, 0x006E1F2C) = 0;
    g_tell_index_guess = 0;
    EQ_GLOBAL(int, 0x006B63F4) = 0;
    EQ_GLOBAL(int, 0x006B6490) = 0;
    DAT_006b6494 = 0;
    EQ_GLOBAL(char, 0x006B63F8) = 0;
    EQ_GLOBAL(int, 0x006B70E4) = 0;
    EQ_GLOBAL(char, 0x006B6770) = 0;
    EQ_GLOBAL(short, 0x006B6772) = -1;
    EQ_GLOBAL(int, 0x006B6774) = 0;
    EQ_GLOBAL(int, 0x006B6778) = 0;
    EQ_GLOBAL(int, 0x006B677C) = 0;
    EQ_GLOBAL(int, 0x006B6780) = 0;
    EQ_GLOBAL(short, 0x006B6784) = 0;
    EQ_GLOBAL(int, 0x006B6788) = 0;
    strcpy(EQ_ARRAY(char, 0x006B678C), EMPTY_STRING);
    EQ_GLOBAL(int, 0x006B67AC) = 0;
    EQ_GLOBAL(short, 0x006B67B0) = -1;
    EQ_GLOBAL(char, 0x006B6409) = 1;
    EQ_GLOBAL(char, 0x006B6408) = 0;
    EQ_GLOBAL(char, 0x006B64D8) = 0;
    EQ_GLOBAL(char, 0x006B645A) = 0;
    EQ_GLOBAL(int, 0x006B676C) = 0;
    EQ_GLOBAL(char, 0x006B6464) = 0;
    strcpy(EQ_ARRAY(char, 0x006B64A8), EMPTY_STRING);
    EQ_GLOBAL(char, 0x006B6465) = 0;
    EQ_GLOBAL(int, 0x006B6764) = 0;
    EQ_GLOBAL(char, 0x006B65D8) = 0;
    EQ_GLOBAL(short, 0x006B65B6) = 0;
    g_chatInputInactive_guess = 1;
    EQ_GLOBAL(int, 0x006B65BC) = 0;
    EQ_GLOBAL(int, 0x006B65C0) = 0;
    EQ_GLOBAL(int, 0x006B65C4) = 0;
    EQ_GLOBAL(int, 0x006B65C8) = 0;
    EQ_GLOBAL(int, 0x006B65CC) = 0;
    EQ_GLOBAL(int, 0x006B65D0) = 0;
    EQ_GLOBAL(int, 0x006B65D4) = 0;
    EQ_GLOBAL(int, 0x006B65A0) = 0;
    EQ_GLOBAL(int, 0x006B7124) = 0;
    EQ_GLOBAL(int, 0x006B6594) = 0;
    EQ_GLOBAL(int, 0x006B6590) = 0;
    EQ_GLOBAL(int, 0x006E4E8C) = 0;
    EQ_GLOBAL(int, 0x006B65A8) = 0;
    EQ_GLOBAL(int, 0x006B65AC) = 0;
    EQ_GLOBAL(int, 0x006B65B0) = 0;
    EQ_GLOBAL(char, 0x006B7111) = 0;
    EQ_GLOBAL(int, 0x006B6754) = 0;
    EQ_GLOBAL(short, 0x006B6752) = 0;
    EQ_GLOBAL(char, 0x006B7110) = 0;
    EQ_GLOBAL(int, 0x006B70E8) = 0;
    EQ_GLOBAL(int, 0x006B70EC) = 0;
    EQ_GLOBAL(int, 0x006B70F4) = 0;
    EQ_GLOBAL(int, 0x006B70F8) = 0;
    g_attacking_guess = 0;
    g_rangedAttacking_guess = 0;
    EQ_GLOBAL(char, 0x006B6405) = 0;
    EQ_GLOBAL(short, 0x006B70FC) = 0;
    EQ_GLOBAL(short, 0x006B70FE) = 0;
    EQ_GLOBAL(short, 0x006B7100) = 0;
    EQ_GLOBAL(short, 0x006B7102) = 0;
    EQ_GLOBAL(short, 0x006B7104) = 0;
    EQ_GLOBAL(short, 0x006B7106) = 0;
    EQ_GLOBAL(short, 0x006B7116) = 0;
    EQ_GLOBAL(short, 0x006B7118) = 0;
    EQ_GLOBAL(int, 0x006B64DC) = 0;
    EQ_GLOBAL(int, 0x006B64E0) = 0;
    EQ_GLOBAL(char, 0x006B710E) = 0;
    EQ_GLOBAL(char, 0x006B710F) = 0;
    EQ_GLOBAL(char, 0x006B6854) = 0;
    EQ_GLOBAL(int, 0x006B64E4) = 0x96;
    EQ_GLOBAL(int, 0x006B64E8) = 0;
    EQ_GLOBAL(int, 0x006B6474) = 0;
    EQ_CDECL(void, 0x004E878C, (void))();
    EQ_GLOBAL(int, 0x006B70F0) = 0;
    EQ_GLOBAL(int, 0x006B70EC) = 0;
    EQ_GLOBAL(int, 0x006B64F0) = 0x26;
    EQ_GLOBAL(int, 0x006B64F4) = 0x26;
    EQ_GLOBAL(int, 0x006B70E8) = 0;
    EQ_GLOBAL(int, 0x006B647C) = 0;
    EQ_GLOBAL(int, 0x006B6478) = 0;
    EQ_GLOBAL(int, 0x006E2054) = 0;
    EQ_GLOBAL(int, 0x006B64EC) = 0x4B;
    EQ_GLOBAL(int, 0x006B64F8) = 0x52;
    EQ_GLOBAL(int, 0x006B64FC) = 0x37;
    EQ_GLOBAL(int, 0x006B6500) = 0xA4;
    EQ_GLOBAL(int, 0x006B6768) = 0;
    EQ_GLOBAL(int, 0x006B6510) = -1;
    EQ_GLOBAL(int, 0x006B6514) = 0;
    if (EQ_GLOBAL(float*, 0x006E4DDC) != 0)
    {
        EQ_GLOBAL(float*, 0x006E4DDC)[2] = 0.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[0] = 1.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[1] = 0.0f;
        ((int*)EQ_GLOBAL(float*, 0x006E4DDC))[4] = -1;
    }
    for (index = 0; index < 0x24; index++)
        EQ_ARRAY(int, 0x006B67B4)[index] = 0;
    for (index = 0; index < 4; index++)
        EQ_ARRAY(int, 0x006B6844)[index] = 0;
    strcpy((char*)this + 0xBA8, EMPTY_STRING);
    EQ_GLOBAL(int, 0x006B6468) = 0;
    EQ_GLOBAL(int, 0x006B646C) = 0;
    EQ_GLOBAL(int, 0x006B6470) = 0;
    EQ_GLOBAL(int, 0x006B6508) = 0xB;
    EQ_GLOBAL(int, 0x006B6498) = 0;
    EQ_GLOBAL(int, 0x006B64A0) = 0;
    EQ_GLOBAL(int, 0x006B649C) = 0;
    EQ_GLOBAL(int, 0x006B64A4) = 0;
    this->nameBufferIndex_guess = 0;
    EQ_GLOBAL(float, 0x006B711C) = 0.0f;
    EQ_GLOBAL(int, 0x006B64C8) = 0;
    EQ_GLOBAL(int, 0x006B64CC) = 0;
    EQ_GLOBAL(int, 0x006B64D4) = 0;
    EQ_GLOBAL(char, 0x006B64D0) = 0;
    g_vehicle_found_guess = 0;
    g_vehicle_pending_guess = 0;
    EQ_GLOBAL(char, 0x006E1DE6) = 0;
    EQ_GLOBAL(char, 0x006B640A) = 0;
    EQ_GLOBAL(char, 0x006E1DE7) = 0;
    EQ_GLOBAL(char, 0x006E1DE8) = 0;
    EQ_GLOBAL(int, 0x006B70E0) = 0;
    for (index = 0; index < 0xF; index++)
        EQ_ARRAY(int, 0x006B6FC0)[index] = -1;
    for (index = 0; index < 0x2D; index++)
        EQ_ARRAY(int, 0x006B6FFC)[index] = -1;
    for (index = 0; index < 4; index++)
    {
        EQ_ARRAY(int, 0x006B70B0)[index] = 0;
        EQ_ARRAY(int, 0x006B70C0)[index] = 0;
        EQ_ARRAY(int, 0x006B70D0)[index] = 0;
    }
    EQ_GLOBAL(short, 0x006E0FE8) = -1;
    for (index = 0; index < 20; index++)
        EQ_ARRAY(EverQuestBlock_guess, 0x006D6FE8)[index][0] = 0;
    EQ_GLOBAL(int, 0x006D6FDC) = 0;
    for (index = 0; index < 500; index++)
    {
        sprintf(
            EQ_ARRAY(EverQuestChatLine_guess, 0x006B772B)[index], EQ_ARRAY(const char, 0x0055D6D4));
        EQ_ARRAY(short, 0x006D6BF4)[index] = 0;
    }
    for (index = 0; index < 25; index++)
    {
        EQ_ARRAY(char*, 0x006D6B2C)[index] = EQ_ARRAY(EverQuestChatLine_guess, 0x006B772B)[index];
        EQ_ARRAY(short*, 0x006D6B90)[index] = &EQ_ARRAY(short, 0x006D6BF4)[index];
    }
    EQ_GLOBAL(short, 0x006E0FEA) = 0;
    for (index = 0; index < 0x100; index++)
        EQ_ARRAY(int, 0x006E19E4)[index] = 0;
    for (index = 0; index < 100; index++)
    {
        EQ_ARRAY(char, 0x006E117C)[index] = 1;
        EQ_ARRAY(int, 0x006E0FEC)[index] = 0;
    }
    EQ_GLOBAL(int, 0x006B6860) = 0;
    EQ_GLOBAL(int, 0x006B688C) = 10;
    EQ_GLOBAL(int, 0x006B6890) = 10;
    EQ_GLOBAL(int, 0x006B6864) = 0;
    EQ_GLOBAL(float, 0x006B685C) = 1.0f;
    EQ_GLOBAL(int, 0x006B6868) = 2;
    EQ_GLOBAL(int, 0x006B6870) = 1;
    EQ_GLOBAL(int, 0x006B6874) = 3;
    EQ_GLOBAL(int, 0x006B689C) = 4;
    EQ_GLOBAL(int, 0x006B68A0) = 0;
    EQ_GLOBAL(int, 0x006B68A4) = 0;
    EQ_GLOBAL(int, 0x006B68A8) = 1;
    EQ_GLOBAL(int, 0x006B68AC) = 0;
    EQ_GLOBAL(int, 0x006B68B0) = 0;
    EQ_GLOBAL(int, 0x006B68B4) = 0;
    EQ_GLOBAL(int, 0x006B68B8) = 0;
    for (index = 0; index < 6; index++)
        EQ_ARRAY(int, 0x006B6984)[index] = 1;
    EQ_GLOBAL(int, 0x006B699C) = 1;
    EQ_GLOBAL(int, 0x006B69A0) = 0;
    EQ_GLOBAL(int, 0x006B69A4) = 1;
    EQ_GLOBAL(int, 0x006B69A8) = 1;
    for (index = 0; index < 30; index++)
        EQ_ARRAY(EverQuestName32_guess, 0x006E24D4)[index][0] = 0;
    for (index = 0; index < 30; index++)
        EQ_ARRAY(EverQuestName32_guess, 0x006E2114)[index][0] = 0;
    EQ_GLOBAL(int, 0x006B686C) = 1;
    EQ_GLOBAL(int, 0x006B6884) = 0;
    EQ_GLOBAL(int, 0x006B6888) = 0;
    EQ_GLOBAL(int, 0x006B687C) = 0;
    EQ_GLOBAL(int, 0x006B6880) = 0;
    EQ_GLOBAL(int, 0x006B6898) = 0;
    EQ_GLOBAL(int, 0x006B6878) = 1;
    this->gameState_guess = 6;
    if (EQ_GLOBAL(char, 0x006B6750) == 0)
    {
        g_new_zone_guess.fog_red[0] = 0xC0;
        g_new_zone_guess.fog_green[0] = 0xC0;
        g_new_zone_guess.fog_blue[0] = 0xC0;
        g_new_zone_guess.fog_min_clip[0] = 150.0f;
        g_new_zone_guess.fog_max_clip[0] = 390.0f;
        g_new_zone_guess.min_clip = 100.0f;
        g_new_zone_guess.max_clip = 1560.0f;
        *(float*)&g_new_zone_guess.unknown0x114[0] = 0.4f;
        g_new_zone_guess.unknown0x114[4] = 1;
        g_new_zone_guess.zone_type = 1;
        g_new_zone_guess.sky = 1;
        strcpy(g_new_zone_guess.zone_long_name, EMPTY_STRING);
        strcpy(g_new_zone_guess.zone_short_name, EMPTY_STRING);
        for (index = 1; index < 6; index++)
            strcpy(
                g_new_zone_guess.zone_long_name + index * 30, EMPTY_STRING);
        g_new_zone_guess.safe_z = 9.0f;
        *(float*)&g_new_zone_guess.unknown0x164[0] = 40000.0f;
        *(float*)&g_new_zone_guess.unknown0x14F[5] = 1.0f;
        g_new_zone_guess.underworld = -40000.0f;
        g_new_zone_guess.safe_y = 0.0f;
        g_new_zone_guess.safe_x = 0.0f;
    }
    *(short*)((char*)&g_new_zone_guess.gravity + 1) = 4;
    *(short*)((char*)&g_new_zone_guess.gravity + 3) = 1;
    *(short*)&g_new_zone_guess.unknown0x14F[1] = 0;
    EQ_GLOBAL(char, 0x006B645C) = 1;
    EQ_GLOBAL(char, 0x006B645D) = 0;
    EQ_GLOBAL(char, 0x006B658E) = 0;
    EQ_GLOBAL(char, 0x006B658D) = 0;
    EQ_GLOBAL(char, 0x006B658C) = 1;
    EQ_GLOBAL(char, 0x006B6428) = 8;
    EQ_GLOBAL(short, 0x006B642A) = 0;
    EQ_GLOBAL(char, 0x006B642C) = 0;
    for (index = 0; index < 10; index++)
    {
        EQ_ARRAY(int, 0x006B6538)[index] = 0;
        EQ_ARRAY(int, 0x006B6560)[index] = 0;
    }
    EQ_GLOBAL(int, 0x006B6588) = 0;
    EQ_GLOBAL(int, 0x006B652C) = 0;
    EQ_GLOBAL(int, 0x006B6534) = 0;
    EQ_GLOBAL(int, 0x006B6530) = 0;
    EQ_GLOBAL(int, 0x006B6450) = 0;
    EQ_GLOBAL(int, 0x006B6454) = 0;
    EQ_GLOBAL(int, 0x006B65A4) = 0;
    *(int*)this = 0;
    this->inCharacterCreation_guess = 0;
    this->disconnected_guess = 0;
    *(u32*)((char*)this + 0x22894) = window;
    EQ_GLOBAL(u32, 0x006B63D8) = window;
    g_attacking_guess = 0;
    g_rangedAttacking_guess = 0;
    g_chatInputInactive_guess = 0;
    EQ_GLOBAL(char, 0x006E1E08) = 1;
    EQ_GLOBAL(char, 0x006B6406) = 1;
    g_game_state_guess = 1;
    EQ_GLOBAL(int, 0x00590170) = 1;
    *((char*)this + 0x13EC) = 0;
    for (index = 0; index < 0xBBA; index++)
        EQ_ARRAY(int, 0x006B32B4)[index] = 0;
    EQ_GLOBAL(short, 0x006B63E0) = 0x77;
    EQ_GLOBAL(short, 0x006B63E2) = 0;
    EQ_GLOBAL(short, 0x006B63E4) = 0x207;
    EQ_GLOBAL(short, 0x006B63E6) = 0x140;
    EQ_GLOBAL(int, 0x00590158) = 1;
    g_activeView_guess = 0;
    EQ_GLOBAL(int, 0x00590168) = 0;
    strcpy((char*)this + 0xBA8, EMPTY_STRING);
    for (index = 0; index < 5; index++)
        EQ_ARRAY(int, 0x00601880)[index] = 0;
    for (index = 0; index < 5; index++)
    {
        if (index != 4)
            EQ_ARRAY(int, 0x006B619C)[index] = 0;
    }
    for (index = 0; index < 10; index++)
    {
        strcpy(
            (char*)this + 0x22400 + index * 0x1E, EQ_ARRAY(const char, 0x005454F0));
        ((int*)((char*)this + 0x22808))[index] = 0;
        ((int*)((char*)this + 0x22830))[index] = 0;
    }
    for (index = 0; index < 100; index++)
        EQ_ARRAY(void*, 0x006E33A4)[index] = new SkillData_guess(index);
    for (index = 0; index < 3000; index++)
        EQ_ARRAY(void*, 0x005FE9A0)[index] = new SpellData_guess(index);
    EQ_GLOBAL(void*, 0x006E4DA8) = 0;
    g_displayManager_guess = (char*)new DisplayAllocation_guess(window);
    DbgLog_guess(EQ_ARRAY(const char, 0x0056A768));
    if (g_displayManager_guess == 0 || g_cdisplay_guess->palette_failed_guess == 1)
    {
        DbgLog_guess(EQ_ARRAY(const char, 0x0056A714));
        *(int*)this = 1;
    }
    EQ_METHOD(0, void, 0x0040101D, (void*), this)();
    EQ_GLOBAL(void*, 0x006E4D54) = new TimedPairAllocation_guess;
    if (EQ_METHOD(0, bool, 0x00409FBA, (void*), g_displayManager_guess)() == 0)
    {
        DbgLog_guess(EQ_ARRAY(const char, 0x0056A6D8));
        *(int*)this = 1;
    }
    DbgLog_guess(EQ_ARRAY(const char, 0x0056A6C0));
    EQTRACE_guess(EQ_ARRAY(const char, 0x0056A6A0));
    EQ_GLOBAL(AudioInterface_guess*, 0x006E4DD0) = EQ_CDECL(AudioInterface_guess*, 0x004ECB4D, (void))();
    if (EQ_GLOBAL(AudioInterface_guess*, 0x006E4DD0) == 0)
    {
        DbgLog_guess(EQ_ARRAY(const char, 0x0056A660));
        EQ_GLOBAL(int, 0x006EFE1C) = 1;
    }
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0)
    {
        if (!EQ_GLOBAL(AudioInterface_guess*, 0x006E4DD0)->Initialize_guess(&window, EQ_GLOBAL(u32, 0x006EFE20)))
        {
            DbgLog_guess(EQ_ARRAY(const char, 0x0056A644));
            EQ_GLOBAL(int, 0x006EFE1C) = 1;
        }
        else
        {
            EQTRACE_guess(EQ_ARRAY(const char, 0x0056A62C));
            EQ_GLOBAL(void*, 0x006E4DD4) = new Direct3DRMAllocation_guess;
            g_sound_manager_ptr_guess = new SoundTableAllocation_guess;
            g_environment_ptr_guess = new SoundManagerAllocation_guess;
            EQTRACE_guess(EQ_ARRAY(const char, 0x0056A60C));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A600))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A5E8));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A5DC))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A5C4));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A5B8))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A5A0));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A594))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A57C));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A570))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A558));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A54C))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A534));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A528))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A510));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A504))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A4EC));
            if (EQ_METHOD(1, char, 0x004F8D80, (void*, SoundPackPath_guess), PACKFS_INSTANCE)(
                    SoundPackPath_guess(EQ_ARRAY(const char, 0x0056A4E0))) == 0)
                EQTRACE_guess(EQ_ARRAY(const char, 0x0056A4C8));
            EQTRACE_guess(EQ_ARRAY(const char, 0x0056A4B8));
            EQTRACE_guess(EQ_ARRAY(const char, 0x0056A49C));
            EQ_GLOBAL(void*, 0x006ECDF4) = new MidiAllocation_guess;
            sequence = new MidiSequenceAllocation_guess;
            EQ_GLOBAL(void*, 0x006ECDF8) = sequence;
            if (sequence != 0)
                EQ_METHOD(1, int, 0x004E4401, (void*, const char*), sequence)(EQ_ARRAY(const char, 0x0056A494));
            sequence = new MidiSequenceAllocation_guess;
            EQ_GLOBAL(void*, 0x006ECDFC) = sequence;
            if (sequence != 0)
            {
                EQ_METHOD(1, int, 0x004E4401, (void*, const char*), sequence)(
                    EQ_GLOBAL(int, 0x006ECE28) == 0x132 ? EQ_ARRAY(const char, 0x0056A488)
                                                        : EQ_ARRAY(const char, 0x0056A47C));
                EQ_METHOD(0, bool, 0x004E43E3, (void*), EQ_GLOBAL(void*, 0x006ECDFC))();
            }
            EQTRACE_guess(EQ_ARRAY(const char, 0x0056A464));
        }
    }
    DbgLog_guess(EQ_ARRAY(const char, 0x0056A450));
    EQ_METHOD(0, void, 0x004CF8C3, (void*), this)();
    return this;
}
#undef CEverQuest
#undef g_new_zone_guess
#undef PACKFS_INSTANCE
#undef EMPTY_STRING

// FUNCTION: TRILOGY 0x004C49EE
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The destructor ignores this; the entry point uses ECX/EDX as fastcall registers.
void __fastcall CEverQuest_dtor_guess(void* unusedThis, int unusedEdx)
{
    typedef void (__stdcall *AilDlsCloseProc_guess)(int, int);
    typedef void (__stdcall *AilShutdownProc_guess)(void);
    typedef void (__cdecl *T3dFastFileFinishProc_guess)(void*);
    typedef void (__fastcall *ComReleaseProc_guess)(void*, int, int);
    AilDlsCloseProc_guess ailDlsClose;
    AilShutdownProc_guess ailShutdown;
    T3dFastFileFinishProc_guess fastFileFinish;
    ComReleaseProc_guess* comVTable;
    char localFilePaths[92];
    char* ptr;
    char* manager;
    int* slot;
    int value;

    if (EQ_GLOBAL(int, 0x006EFE1C) == 0)
    {
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A9A4));
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A990));
        ptr = EQ_GLOBAL(char*, 0x006ECDFC);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x004E44D3, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006ECDFC) = 0;
        }
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A97C));
        ptr = EQ_GLOBAL(char*, 0x006ECDF8);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x004E44D3, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006ECDF8) = 0;
        }
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A968));
        ptr = EQ_GLOBAL(char*, 0x006ECE00);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x004E44D3, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006ECE00) = 0;
        }
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A954));
        ptr = EQ_GLOBAL(char*, 0x006ECDF4);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x004E4E26, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006ECDF4) = 0;
        }
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A940));
        ptr = EQ_GLOBAL(char*, 0x006ECDC0);
        if (ptr != 0)
        {
            ailDlsClose = EQ_GLOBAL(AilDlsCloseProc_guess, 0x005342FC);
            ailDlsClose((int)ptr, 2);
        }
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A92C));
        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A924));

        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A600));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A5DC));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A5B8));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A594));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A570));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A54C));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A528));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A504));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));
        EQ_THISCALL(void, 0x004C479D, (void*, int, const char*))
            (localFilePaths + 64, 0, EQ_ARRAY(char, 0x0056A4E0));
        EQ_THISCALL(void, 0x004F9320, (void*, int, int, int))
            (EQ_ARRAY(char, 0x006ECCF4), 0, *(int*)(localFilePaths + 64), *(int*)(localFilePaths + 68));

        EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A91C));
        ptr = EQ_GLOBAL(char*, 0x006E4DDC);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x0040101D, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006E4DDC) = 0;
        }
        ptr = g_reverb_on_guess;
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x0040101D, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006E4DD8) = 0;
        }
        ptr = EQ_GLOBAL(char*, 0x006E4DD4);
        if (ptr != 0)
        {
            if (ptr != 0)
            {
                EQ_THISCALL(void, 0x004E3F8A, (void*, int))(ptr, 0);
                EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            }
            EQ_GLOBAL(int, 0x006E4DD4) = 0;
        }
        ailShutdown = EQ_GLOBAL(AilShutdownProc_guess, 0x00534304);
        ailShutdown();
        ptr = EQ_GLOBAL(char*, 0x006E4DD0);
        if (ptr != 0)
        {
            comVTable = *(ComReleaseProc_guess**)ptr;
            comVTable[0](ptr, 0, 1);
            EQ_GLOBAL(int, 0x006E4DD0) = 0;
        }
    }

    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A914));
    fastFileFinish = EQ_GLOBAL(T3dFastFileFinishProc_guess, 0x006E5240);
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x80);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        *(int*)(g_displayManager_guess + 0x80) = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A90C));
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x84);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        g_cdisplay_guess->fastFileHandle2_guess = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A904));
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x88);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        g_cdisplay_guess->fastFileHandle3_guess = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8FC));
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x8C);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        g_cdisplay_guess->fastFileHandle4_guess = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8F4));
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x90);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        g_cdisplay_guess->fastFileHandle5_guess = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8EC));
    manager = g_displayManager_guess;
    value = *(int*)(manager + 0x94);
    if (value != 0)
    {
        fastFileFinish((void*)value);
        g_cdisplay_guess->fastFileHandle6_guess = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8E4));
    ptr = EQ_GLOBAL(char*, 0x006E4DA8);
    if (ptr != 0)
    {
        if (ptr != 0)
        {
            EQ_THISCALL(void, 0x0047EF27, (void*, int))(ptr, 0);
            EQ_CDECL(void, 0x005218BB, (void*))(ptr);
        }
        EQ_GLOBAL(int, 0x006E4DA8) = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8DC));
    slot = EQ_ARRAY(int, 0x006B32B4);
    do
    {
        if (*slot != 0)
        {
            ptr = (char*)*slot;
            EQ_THISCALL(void, 0x0040101D, (void*, int))(ptr, 0);
            EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            *slot = 0;
        }
        slot++;
    } while ((int)slot < 0x006B619C);
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8D4));
    slot = EQ_ARRAY(int, 0x006E33A4);
    do
    {
        if (*slot != 0)
        {
            ptr = (char*)*slot;
            EQ_THISCALL(void, 0x0040101D, (void*, int))(ptr, 0);
            EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            *slot = 0;
        }
        slot++;
    } while ((int)slot < 0x006E3534);
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8CC));
    slot = EQ_ARRAY(int, 0x005FE9A0);
    do
    {
        if (*slot != 0)
        {
            ptr = (char*)*slot;
            EQ_THISCALL(void, 0x0040101D, (void*, int))(ptr, 0);
            EQ_CDECL(void, 0x005218BB, (void*))(ptr);
            *slot = 0;
        }
        slot++;
    } while ((int)slot < 0x00601880);
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8C4));
    ptr = EQ_GLOBAL(char*, 0x006E4D54);
    manager = g_displayManager_guess;
    if (manager != 0)
    {
        if (manager != 0)
        {
            EQ_THISCALL(void, 0x0040601F, (void*, int))(manager, 0);
            EQ_CDECL(void, 0x005218BB, (void*))(manager);
        }
    }
    EQ_GLOBAL(int, 0x006E4DE0) = 0;
    if (ptr != 0)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(ptr);
        EQ_GLOBAL(int, 0x006E4D54) = 0;
    }
    EQ_CDECL(void, 0x004C1703, (const char*))(EQ_ARRAY(char, 0x0056A8BC));
    ptr = EQ_GLOBAL(char*, 0x006E20CC);
    EQ_GLOBAL(int, 0x006E4D58) = 0;
    EQ_GLOBAL(int, 0x006E4D5C) = 0;
    EQ_GLOBAL(int, 0x006E4D60) = 0;
    EQ_GLOBAL(int, 0x006E4D64) = 0;
    EQ_GLOBAL(int, 0x006E4D68) = 0;
    EQ_GLOBAL(int, 0x006E4D6C) = 0;
    EQ_GLOBAL(int, 0x006E4D70) = 0;
    EQ_GLOBAL(int, 0x006E4D74) = 0;
    EQ_GLOBAL(int, 0x006E4D78) = 0;
    EQ_GLOBAL(int, 0x006E4D7C) = 0;
    EQ_GLOBAL(int, 0x006E4D80) = 0;
    if (ptr != 0)
    {
        EQ_CDECL(void, 0x005230DD, (void*))(ptr);
        EQ_GLOBAL(int, 0x006E20CC) = 0;
        EQ_GLOBAL(int, 0x006E20D8) = 0;
        EQ_GLOBAL(int, 0x006E20DC) = 0;
        EQ_GLOBAL(int, 0x006E20D4) = 0;
        EQ_GLOBAL(int, 0x006E20E0) = 0;
    }
    return;
}

struct CEverQuest_ProcessMBox_type_guess
{
    u32 ProcessMBox_guess(void);
};

#define CEverQuest CEverQuest_ProcessMBox_type_guess
// FUNCTION: TRILOGY 0x004C4EE7
// Polls the network mailbox and routes received world messages to the game client.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
u32 CEverQuest::ProcessMBox_guess(void)
{
    u32 result;
    void* message;
    void* connection;
    int* application;
    u16 messageSize;
    u16* messageData;
    int hasFin;
    void* worldConnection;

    result = 1;
    if (EQ_GLOBAL(char, 0x006B63F8) != 0)
        return 1;
    for (message = EQ_CDECL(void*, 0x004F1600, (void*, int))(EQ_GLOBAL(void*, 0x006ECCA8), 0);
         message != 0;
         message = EQ_CDECL(void*, 0x004F1600, (void*, int))(EQ_GLOBAL(void*, 0x006ECCA8), 0))
    {
        connection = EQ_CDECL(void*, 0x004F4CB0, (void*))(message);
        if (connection != 0)
        {
            application = EQ_CDECL(int*, 0x004F23C0, (void*))(connection);
            messageSize = (u16)EQ_CDECL(u32, 0x004F4C80, (void*))(message);
            if (*application == 0)
                EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))(connection, 0, 0, 0);

            if (messageSize < 2)
            {
                hasFin = EQ_CDECL(int, 0x004F4CC0, (void*))(message);
                if (hasFin != 0 || EQ_CDECL(int, 0x004F23B0, (void*))(connection) == 0)
                {
                    DbgLog_guess((const char*)0x0056A9B8,
                        EQ_CDECL(int, 0x004F4CC0, (void*))(message), EQ_CDECL(int, 0x004F23B0, (void*))(connection));
                    worldConnection = EQ_GLOBAL(void*, 0x006B3194);
                    if (worldConnection != 0)
                    {
                        EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))(worldConnection, 10000, 0, 0);
                        EQ_GLOBAL(void*, 0x006B3194) = 0;
                    }
                }
            }
            else
            {
                messageData = (u16*)EQ_CDECL(void*, 0x004F4C90, (void*))(message);
                EnterCriticalSection((LPCRITICAL_SECTION)0x006048E8);
                result = (u8)EQ_METHOD(4, u8, 0x00496743, (CEverQuest*, void*, u32, void*, u32), this)
                    (connection, (u32)messageData[0], messageData + 1, (u16)(messageSize - 2));
                LeaveCriticalSection((LPCRITICAL_SECTION)0x006048E8);
            }
        }
        EQ_CDECL(void, 0x004F4AC0, (void*))(message);
        if (result == 0)
            break;
    }
    return result;
}
#undef CEverQuest

// FUNCTION: TRILOGY 0x004c5026
// Spends one charge of the player's item with the given unique id; using the last charge destroys the item.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, `this` unused (written as __fastcall with unused ECX/EDX). Uses one charge of the item with unique_id
// uniqueId: the first match in inventory slots 1..29, then slot 0, then g_eqmain_guess's 8 slots (when +0x685E4 is
// set), then the 8 bank slots (when +0x685E8 is set). A match is a type 0 item or a type 1 bag's type 0 contents.
// A charge byte (0xDA) of 1 deletes the item and clears its slot; otherwise the byte is decremented.
void __fastcall UseCharge(void* unusedThis, int unusedEdx, u32 uniqueId)
{
    int slotIndex;
    EQ_Item** slotPtr;
    EQ_Item* pEVar3;
    EQ_Item** pEVar6;
    int bagItem;
    int slot;
    char chargeCount;
    char* ptr;
    char* pCVar2;

    if (g_pc_guess == 0)
        return;
    if (g_eqmain_guess != 0)
        pinstEQ_Main_guess->uiDirty_guess = 1;
    slotPtr = &g_pc_guess->inventory_items[1];
    slotIndex = 1;
    do
    {
        pEVar3 = *slotPtr;
        if (pEVar3 != 0)
        {
            if (pEVar3->type == 0 && pEVar3->unique_id == uniqueId)
            {
                slot = slotIndex * 4 + 0xe0;
                goto foundItemSlot;
            }
            if (pEVar3 != 0 && pEVar3->type == 1)
            {
                for (slot = 0, pEVar6 = (EQ_Item**)((char*)pEVar3 + 0xac);
                     slot < (int)*(u8*)((char*)pEVar3 + 0xd5); slot++, pEVar6++)
                {
                    bagItem = (int)*pEVar6;
                    if (bagItem != 0 && *(char*)(bagItem + 0x81) == 0 && *(u32*)(bagItem + 0xa0) == uniqueId)
                        goto foundInBag;
                }
            }
        }
        slotIndex++;
        slotPtr++;
    } while (slotIndex <= 29);

    pEVar3 = g_pc_guess->inventory_items[0];
    slot = 0;
    if (pEVar3 != 0)
    {
        if (pEVar3->type == 0 && pEVar3->unique_id == uniqueId)
        {
            chargeCount = *(char*)((char*)pEVar3 + 0xda);
            if (chargeCount == 1)
            {
                EQ_CDECL(void, 0x005218BB, (void*))((void*)pEVar3);
                g_pc_guess->inventory_items[0] = 0;
                return;
            }
            goto decrementCharge;
        }
        if (pEVar3 != 0 && pEVar3->type == 1)
        {
            for (pEVar6 = (EQ_Item**)((char*)pEVar3 + 0xac);
                 slot < (int)*(u8*)((char*)pEVar3 + 0xd5); slot++, pEVar6++)
            {
                bagItem = (int)*pEVar6;
                if (bagItem != 0 && *(char*)(bagItem + 0x81) == 0 && *(u32*)(bagItem + 0xa0) == uniqueId)
                    goto foundInBag;
            }
        }
    }

    if (g_eqmain_guess == 0)
        return;
    if (pinstEQ_Main_guess->tradeOpen_guess != 0)
    {
        slotIndex = 0;
        slotPtr = (EQ_Item**)(g_eqmain_guess + 0x6c50c);
        do
        {
            pEVar3 = *slotPtr;
            if (pEVar3 != 0)
            {
                if (pEVar3->type == 0 && pEVar3->unique_id == uniqueId)
                {
                    slot = slotIndex * 4 + 0x6c50c;
                    pEVar3 = *(EQ_Item**)(g_eqmain_guess + slot);
                    chargeCount = *(char*)((char*)pEVar3 + 0xda);
                    if (chargeCount != 1)
                        goto decrementCharge;
                    EQ_CDECL(void, 0x005218BB, (void*))((void*)pEVar3);
                    pCVar2 = g_eqmain_guess;
                    goto clearSlot;
                }
                if (pEVar3 != 0 && pEVar3->type == 1)
                {
                    for (slot = 0, pEVar6 = (EQ_Item**)((char*)pEVar3 + 0xac);
                         slot < (int)*(u8*)((char*)pEVar3 + 0xd5); slot++, pEVar6++)
                    {
                        bagItem = (int)*pEVar6;
                        if (bagItem != 0 && *(char*)(bagItem + 0x81) == 0 && *(u32*)(bagItem + 0xa0) == uniqueId)
                            goto foundInBag;
                    }
                }
            }
            slotIndex++;
            slotPtr++;
        } while (slotIndex < 8);
    }

    if (pinstEQ_Main_guess->bankOpen_guess == 0)
        return;
    slotIndex = 0;
    slotPtr = g_pc_guess->bank_items;
    do
    {
        pEVar3 = *slotPtr;
        if (pEVar3 != 0)
        {
            if (pEVar3->type == 0 && pEVar3->unique_id == uniqueId)
            {
                slot = slotIndex * 4 + 0xf44;
foundItemSlot:
                pEVar3 = *(EQ_Item**)((char*)g_pc_guess + slot);
                chargeCount = *(char*)((char*)pEVar3 + 0xda);
                if (chargeCount == 1)
                {
                    EQ_CDECL(void, 0x005218BB, (void*))((void*)pEVar3);
                    pCVar2 = (char*)g_pc_guess;
clearSlot:
                    ptr = pCVar2 + slot;
                    *(int*)ptr = 0;
                    return;
                }
                goto decrementCharge;
            }
            if (pEVar3 != 0 && pEVar3->type == 1)
            {
                for (slot = 0, pEVar6 = (EQ_Item**)((char*)pEVar3 + 0xac);
                     slot < (int)*(u8*)((char*)pEVar3 + 0xd5); slot++, pEVar6++)
                {
                    bagItem = (int)*pEVar6;
                    if (bagItem != 0 && *(char*)(bagItem + 0x81) == 0 && *(u32*)(bagItem + 0xa0) == uniqueId)
                    {
                        ptr = (char*)pEVar3 + 0xac + slot * 4;
                        goto useSlotItem;
                    }
                }
            }
        }
        slotIndex++;
        slotPtr++;
    } while (slotIndex < 8);
    return;

foundInBag:
    ptr = (char*)pEVar3 + 0xac + slot * 4;
useSlotItem:
    pEVar3 = *(EQ_Item**)ptr;
    chargeCount = *(char*)((char*)pEVar3 + 0xda);
    if (chargeCount == 1)
    {
        EQ_CDECL(void, 0x005218BB, (void*))((void*)pEVar3);
        *(int*)ptr = 0;
        return;
    }
decrementCharge:
    *(char*)((char*)pEVar3 + 0xda) = chargeCount - 1;
}

// FUNCTION: TRILOGY 0x004c53d8
// Ends the bank interaction and closes the bank-related windows.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall that ignores `this`, one stack argument (`ret 4`): `announceFarewell` prints the banker's farewell. Leaves bank
// mode: music, the UI object at 0x006E4E44 (vtable slots 2 and 1, +0xE5C cleared after an 0x2147 message), SavePC,
// the two windows at 0x006E4E5C / 0x006E4E58 (their +0x174 item lost), and a video mode change when 0x00590170 is 2.
void __fastcall LeaveBankMode_guess_impl(void* unusedThis, int unusedEdx, int announceFarewell)
{
    char farewellText[256];
    int messagePayload[23];

    if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(int, 0x006ECDF4) != 0)
        EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
    ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc2();
    pinstEQ_Main_guess->screenMode_guess = pinstEQ_Main_guess->prevScreenMode_guess;
    pinstEQ_Main_guess->bankOpen_guess = 0;
    ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc1();
    EQ_GLOBAL(int, 0x006B7120) = 1;
    EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(0, 1);
    (pinstEQ_Main_guess->pendingServerReplies_guess)--;
    if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
    {
        messagePayload[0] = -4;
        messagePayload[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
        messagePayload[2] = 1;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2147, messagePayload, 0x5c, 1);
    }
    pinstEQ_Main_guess->pendingServerReplies_guess = 0;
    if (announceFarewell != 0)
    {
        sprintf(farewellText, EQ_ARRAY(char, 0x0056A9F0), BagTrimName_guess(EQ_GLOBAL(char*, 0x006E4DBC) + 1));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(farewellText);
    }
    EQ_GLOBAL(int, 0x006E4DBC) = 0;
    if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x10) != 0)
    {
        *(char*)(*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174) + 0xd6) = 0;
        ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E5C))->vfunc2();
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E5C) + 0x174) = 0;
    }
    if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x10) != 0)
    {
        *(char*)(*(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) + 0xd6) = 0;
        ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E58))->vfunc2();
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E58) + 0x174) = 0;
    }
    if (EQ_GLOBAL(int, 0x00590170) == 2)
    {
        g_game_state_guess = 2;
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            pinstEQ_Main_guess->uiDirty_guess = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
    }
}

#define REPORT_DEATH_LOCAL_CHAT_COLOR 0x115
#define REPORT_DEATH_OTHER_CHAT_COLOR 0x116

// FUNCTION: TRILOGY 0x004c555f
// Handles a death report: ends the dead spawn's duel, trade, merchant, bank or loot session and prints the death line.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void __fastcall CEverQuest_ReportDeath_guess(void* self, int unusedEdx, MsgDeath* deathMessage)
{
    struct ReportDeathHit_guess
    {
        u32 spawnId;
        u32 killerId;
        u8 skill;
        u8 padding;
        s16 spellId;
        int damage;
    } hitInfo;
    char duelMessage[256];
    char deathText[512];
    int rentPacket[3];
    u32 deathTargetPacket[2];
    u32 duelPacket[2];
    char deathType[8];
    char keepRentables;
    char showDeathText;
    EQPlayer* deadPlayer;
    EQPlayer* killerPlayer;
    char* strippedName;
    const char* format;
    const char* genderDeathType;
    int* slotEnabled;
    EQPlayer** slotPlayer;
    int slotCount;
    short slotIndex;
    int arrayIndex;

    keepRentables = 0;
    showDeathText = 1;
    deadPlayer = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(
        (int)deathMessage->spawn_id);
    if (deadPlayer == 0)
    {
        EQTRACE_guess(
            EQ_ARRAY(char, 0x0056AA10), deathMessage->spawn_id);
        return;
    }

    if (deadPlayer == g_local_player_guess && deadPlayer->stand_state == 'f')
        keepRentables = 1;

    if (deadPlayer->spawn_id == EQ_GLOBAL(u32, 0x006B64C8))
    {
        if (deathMessage->killer_id == g_local_player_guess->spawn_id
            && EQ_GLOBAL(u8, 0x006B64D0) != 0)
        {
            sprintf(
                duelMessage, (const char*)EQ_ARRAY(char, 0x0056AC10),
                g_local_player_guess->name, deadPlayer->name);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(duelMessage, 0x10D);
            duelPacket[0] = deadPlayer->spawn_id;
            duelPacket[1] = g_local_player_guess->spawn_id;
            send_message_guess(
                &EQ_GLOBAL(void*, 0x006B3194), 0x215E, duelPacket, 8, 1);
        }
        else
        {
            sprintf(
                duelMessage, (const char*)EQ_ARRAY(char, 0x0056ABC8),
                deadPlayer->name, deadPlayer->name);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(duelMessage, 0x10D);
        }
        EQ_GLOBAL(u8, 0x006B64D0) = 0;
        EQ_GLOBAL(u32, 0x006B64C8) = 0;
        EQ_GLOBAL(u32, 0x006B64CC) = 0;
    }

    if (EQ_GLOBAL(void*, 0x006E4E44) != 0
        && pinstEQ_Main_guess->tradeOpen_guess != 0
        && EQ_GLOBAL(EQPlayer*, 0x006E4D88) != 0
        && EQ_GLOBAL(EQPlayer*, 0x006E4D88) == deadPlayer)
    {
        EQ_METHOD(0, void, 0x004571EB, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
    }
    if (deadPlayer == EQ_GLOBAL(EQPlayer*, 0x006E4DB8)
        && EQ_GLOBAL(void*, 0x006E4E44) != 0
        && pinstEQ_Main_guess->merchantOpen_guess != 0)
    {
        EQ_METHOD(0, void, 0x00464AB6, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
        EQ_METHOD(0, void, 0x00464B15, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
    }
    if (deadPlayer == EQ_GLOBAL(EQPlayer*, 0x006E4DBC))
        EQ_METHOD(1, void, 0x004C53D8, (void*, int), self)(0);
    if (deadPlayer == EQ_GLOBAL(EQPlayer*, 0x006E4DC0)
        && EQ_GLOBAL(void*, 0x006E4E44) != 0
        && pinstEQ_Main_guess->lootOpen_guess != 0)
    {
        EQ_METHOD(0, void, 0x00464C8D, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
        EQ_METHOD(0, void, 0x00464CD1, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
    }

    if (deadPlayer == EQ_GLOBAL(EQPlayer*, 0x006E4DC4))
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4DC4) = 0;
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0
            && pinstEQ_Main_guess->screenMode_guess == 6)
        {
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(0x15);
            deathTargetPacket[0] = EQ_GLOBAL(EQPlayer*, 0x006E4DC4)->spawn_id;
            deathTargetPacket[1] = g_local_player_guess->spawn_id;
            send_message_guess(
                &EQ_GLOBAL(void*, 0x006B3194), 0x209D, deathTargetPacket, 8, 1);
            ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc2();
            pinstEQ_Main_guess->screenMode_guess =
                pinstEQ_Main_guess->prevScreenMode_guess;
            ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc1();
            EQ_GLOBAL(int, 0x006B7120) = 0x14;
            EQ_METHOD(2, void, 0x004CE8C0, (void*, int, int), self)(0, 1);
            (pinstEQ_Main_guess->pendingServerReplies_guess)--;
            if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
            {
                rentPacket[0] = -4;
                rentPacket[1] = pinstEQ_Main_guess->pendingServerReplies_guess;
                rentPacket[2] = 2;
                send_message_guess(
                    &EQ_GLOBAL(void*, 0x006B3194), 0x2147, rentPacket, 0x5C, 1);
            }
            pinstEQ_Main_guess->pendingServerReplies_guess = 0;
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                g_game_state_guess = 2;
                if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
            }
        }
    }

    if (EQ_GLOBAL(short, 0x006B6772) != -1
        && EQ_GLOBAL(EQPlayer*, 0x006B6780) == deadPlayer)
    {
        EQ_GLOBAL(short, 0x006B6772) = -1;
        EQ_GLOBAL(EQPlayer*, 0x006B6780) = 0;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), self)(
            (const char*)EQ_ARRAY(char, 0x0055F920));
    }

    if (deadPlayer == EQ_GLOBAL(EQPlayer*, 0x006E4DAC))
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = 0;
        EQ_GLOBAL(u8, 0x006B710C) = 0;
        EQ_GLOBAL(u8, 0x006B710D) = 0;
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            pinstEQ_Main_guess->uiDirty_guess = 1;
    }

    if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
    {
        if (deadPlayer == pinstEQ_Main_guess->inspectSpawn_guess)
        {
            if (EQ_GLOBAL(int, 0x00590170) == 2)
            {
                g_game_state_guess = 2;
                EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
                pinstEQ_Main_guess->prevScreenMode_guess = 0;
            }
            ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc2();
            pinstEQ_Main_guess->screenMode_guess =
                pinstEQ_Main_guess->prevScreenMode_guess;
            ((EQBagVtbl_guess*)EQ_GLOBAL(void*, 0x006E4E44))->vfunc1();
        }

        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
        {
            slotPlayer = (EQPlayer**)0x005FE954;
            slotEnabled = (int*)0x005FE8BD;
            slotCount = 5;
            if (EQ_GLOBAL(char, 0x005FE968) != 0)
            {
                do
                {
                    if (*(char*)slotEnabled != 0 && *slotPlayer == deadPlayer)
                    {
                        (*slotPlayer)->cur_hp = -11;
                        pinstEQ_Main_guess->uiDirty_guess = 1;
                    }
                    slotEnabled = (int*)((char*)slotEnabled + 0x1E);
                    slotPlayer++;
                    slotCount--;
                } while (slotCount != 0);
            }

            slotIndex = 0;
            do
            {
                if (*(EQPlayer**)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x685F8 + slotIndex * 16)
                    == deadPlayer)
                {
                    arrayIndex = (int)slotIndex;
                    *(EQPlayer**)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x685F8 + arrayIndex * 16) = 0;
                    *(float*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x685FC + arrayIndex * 16) =
                        EQ_GLOBAL(float, 0x005345BC);
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x68600 + arrayIndex * 16) = -1;
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x68604 + arrayIndex * 16) = 0;
                    break;
                }
                slotIndex++;
            } while (slotIndex < 1000);
        }
    }

    killerPlayer = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(
        (int)deathMessage->killer_id);
    if (killerPlayer != g_local_player_guess
        && deathMessage->spawn_id != g_local_player_guess->spawn_id
        && EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float),
            g_display_guess)(g_local_player_guess, deadPlayer, 0.0f)
            > EQ_GLOBAL(float, 0x00535190))
    {
        showDeathText = 0;
    }

    hitInfo.killerId = deathMessage->killer_id;
    hitInfo.spawnId = deathMessage->spawn_id;
    hitInfo.skill = deathMessage->skill;
    hitInfo.spellId = deathMessage->spell_id;
    hitInfo.damage = (int)deathMessage->damage;
    EQ_METHOD(2, void, 0x004C5E50, (void*, void*, int), self)(&hitInfo, showDeathText);

    if (deadPlayer == g_local_player_guess)
    {
        EQ_METHOD(2, void, 0x004C70EC, (void*, MsgDeath*, char), self)(deathMessage, keepRentables);
        if (deathMessage->skill == 0xFA)
        {
            strcpy(deathText,
                (const char*)EQ_ARRAY(char, 0x0056AB0C));
        }
        else if (deathMessage->skill == 0xFB)
        {
            strcpy(deathText,
                (const char*)EQ_ARRAY(char, 0x0056AB28));
        }
        else if (deathMessage->skill == 0xFC)
        {
            strcpy(deathText,
                (const char*)EQ_ARRAY(char, 0x0056AB58));
        }
        else if (deathMessage->killer_id != 0)
        {
            if (killerPlayer == 0)
            {
                sprintf(
                    deathText, (const char*)EQ_ARRAY(char, 0x0056AB78));
            }
            else
            {
                strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(killerPlayer->name);
                sprintf(
                    deathText, (const char*)EQ_ARRAY(char, 0x0056ABA0), strippedName);
            }
        }
        else
        {
            strcpy(deathText,
                (const char*)EQ_ARRAY(char, 0x0056ABBC));
        }
    }
    else if (deathMessage->skill == 0xFA)
    {
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AA60), strippedName);
    }
    else if (deathMessage->skill == 0xFB)
    {
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AA78), strippedName);
    }
    else if (deathMessage->skill == 0xFC)
    {
        if (deadPlayer->gender == 0)
            genderDeathType = (const char*)EQ_ARRAY(char, 0x005681DC);
        else if (deadPlayer->gender == 1)
            genderDeathType = (const char*)EQ_ARRAY(char, 0x0056AAC0);
        else
            genderDeathType = (const char*)EQ_ARRAY(char, 0x00568224);
        strcpy(deathType, genderDeathType);
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AAA4), strippedName, deathType);
    }
    else if (deathMessage->killer_id == 0)
    {
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AB00), strippedName);
    }
    else if (deathMessage->killer_id == g_pc_guess->player->spawn_id)
    {
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AAEC), strippedName);
    }
    else if (killerPlayer != 0)
    {
        if (killerPlayer == deadPlayer && deadPlayer->type == 1)
        {
            showDeathText = 0;
        }
        else
        {
            strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(killerPlayer->name);
            format = (const char*)EQ_ARRAY(char, 0x0055EFE0);
            {
                char* deadName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
                sprintf(
                    deathText, format, deadName, strippedName);
            }
        }
    }
    else
    {
        strippedName = EQ_METHOD(1, char*, 0x004D0436, (void*, char*), self)(deadPlayer->name);
        sprintf(
            deathText, (const char*)EQ_ARRAY(char, 0x0056AAC4), strippedName);
    }

    EQ_METHOD(1, void, 0x004B3E99, (EQPlayer*, MsgDeath*), deadPlayer)(deathMessage);
    if (deadPlayer == g_local_player_guess)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(
            deathText, REPORT_DEATH_LOCAL_CHAT_COLOR);
    }
    else if (showDeathText != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(
            deathText, REPORT_DEATH_OTHER_CHAT_COLOR);
    }
}

// FUNCTION: TRILOGY 0x004c5c4d
// __thiscall. Copies the attack name selected by `attackType`; when requested, appends "s" or "es".
void __fastcall FormatAttackName_guess(void* unusedThis, int unusedEdx, char* actionText, short attackType, char plural)
{
    size_t nameLength;

    switch (attackType)
    {
    case 0x2b:
    case 0xb1:
        strcpy(actionText, (char*)0x0056ac74);
        break;
    case 0x24:
    case 0x16:
    case 0x18:
    case 0x22:
    case 0x25:
    case 0x26:
    case 0x2a:
    case 0x3d:
    case 0x4a:
    case 0x53:
    case 0x54:
    case 0x57:
    case 0x5b:
    case 0x66:
    case 0x71:
    case 0x78:
    case 0x94:
    case 0xab:
    case 0xb0:
    case 0xc2:
        strcpy(actionText, (char*)0x0056ac6c);
        break;
    case 0x0e:
    case 0x17:
    case 0x2f:
    case 0x3f:
    case 0x31:
    case 0x32:
    case 0x4c:
    case 0x6f:
    case 0x77:
    case 0x9e:
    case 0xb8:
    case 0xc4:
        strcpy(actionText, (char*)0x0056ac64);
        break;
    case 0x6b:
    case 0x87:
    case 0xa9:
    case 0xbf:
        strcpy(actionText, (char*)0x0056ac5c);
        break;
    case 0x4f:
    case 0x6d:
    case 0x81:
    case 0x86:
    case 0xac:
        strcpy(actionText, (char*)0x0056ac54);
        break;
    case 0x55:
    case 0x69:
    case 0xae:
        strcpy(actionText, (char*)0x0056ac4c);
        break;
    default:
        if ((attackType <= 0xc) || (attackType == 0x80))
            strcpy(actionText, (char*)0x0056ac44);
        else
            strcpy(actionText, (char*)0x0056ac7c);
        break;
    }
    if (plural != '\0')
    {
        nameLength = strlen(actionText);
        if (actionText[nameLength - 1] == 'h')
            strcat(actionText, (char*)0x0056ac40);
        else
            strcat(actionText, (char*)0x0056ac3c);
    }
}

class CEverQuest_ReportSuccessfulHit_guess
{
public:
    void ReportSuccessfulHit(u32* hitData, char doChat);
};

#define CEverQuest CEverQuest_ReportSuccessfulHit_guess
// FUNCTION: TRILOGY 0x004c5e50
// Combat feedback for one hit message (victim, attacker, hit type, spell, damage); doChat gates the chat lines.
// Resolves the victim and attacker players and names, truncating long names, and decides whether the local
// player is the victim or attacker.
// Builds the chat text by hit type (below 0x27, below 0xe7, 0xe7, above) and prints it with 0x004d055e, picks hit
// sounds via 0x00410d13 and, for damage above zero, the blood effect and the victim's hit animation.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest::ReportSuccessfulHit(u32* hitMessage, char doChat)
{
    struct HitMessage_guess
    {
        u32 victimId;
        u32 attackerId;
        u8 hitType;
        u8 unknown09;
        short spellId;
        int damage;
        u32 unknown10;
        u32 unknown14;
        u32 unknown18;
        u32 unknown1C;
        u32 unknown20;
    };
    struct DamageEvent_guess
    {
        u32 victimId;
        u32 attackerId;
        u32 unknown08;
        u32 unknown0C;
        u32 unknown10;
        u32 unknown14;
        u32 unknown18;
        u32 unknown1C;
        u32 unknown20;
    };
    HitMessage_guess* message = (HitMessage_guess*)hitMessage;
    EQPlayer* victim;
    EQPlayer* attacker = 0;
    char victimName[256];
    char attackerName[256];
    char outputText[256];
    char spellText[256];
    char actionText[256];
    char suffixText[512];
    char damageText[256];
    char eventText[256];
    char* formatText = 0;
    char* sourceText = 0;
    char* attackVerb = 0;
    char* targetPronoun = 0;
    char* damageSuffix = 0;
    char* outputName;
    char* effectName;
    char* finalText;
    void* bloodTarget = 0;
    u32 animationId = 0xffffffff;
    unsigned short reportedDamage = 0;
    unsigned char hitType = message->hitType;
    short spellId = message->spellId;
    int damage = message->damage;
    int soundId;
    int index;
    int status;
    int targetFound;
    int selectedStatus;
    int color = 0;
    bool victimIsLocal = false;
    bool attackerIsVictim = false;
    bool attackerIsLocal = false;
    bool plural = false;
    bool shouldSendChat = false;
    bool specialLine = false;
    bool localHit = false;
    bool groupMember = false;
    u32* spellTable = EQ_ARRAY(u32, 0x005FE9A0);
    u32* targetList = g_death_state_guess;
    DamageEvent_guess damageEvent;
    u32 socialData[193];

    outputText[0] = '\0';
    spellText[0] = '\0';
    actionText[0] = '\0';
    suffixText[0] = '\0';
    damageText[0] = '\0';
    eventText[0] = '\0';
    victimName[0] = '\0';
    attackerName[0] = '\0';
    finalText = (char*)0xffffffff;
    bloodTarget = 0;
    victim = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))((int)message->victimId);
    if (victim == 0)
    {
        EQTRACE_guess(EQ_ARRAY(char, 0x0056B214), message->victimId);
        return;
    }
    if (strlen(victim->name) > 0x1f)
    {
        EQTRACE_guess(EQ_ARRAY(char, 0x0056B1F8));
        return;
    }
    if (message->attackerId != 0)
    {
        attacker = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))((int)message->attackerId);
        if (attacker == 0)
        {
            EQTRACE_guess(EQ_ARRAY(char, 0x0056B1B0), message->attackerId);
            return;
        }
        if (strlen(attacker->name) > 0x1f)
        {
            EQTRACE_guess(EQ_ARRAY(char, 0x0056B190));
            return;
        }
        if (hitType < 0xe8)
        {
            if (hitType == 0xe7)
            {
                if (spellId > 2999 || spellId < 0)
                {
                    EQTRACE_guess(EQ_ARRAY(char, 0x0056B15C));
                    return;
                }
                formatText = (char*)spellTable[spellId];
                if (formatText != 0 && formatText[0x21f] == '\0')
                    strcpy((char*)attacker->actor_info + 0x1bc, victim->name);
            }
            else
                strcpy((char*)attacker->actor_info + 0x1bc, victim->name);
        }
    }

    victimIsLocal = hitType < 0xe8 && g_local_player_guess->spawn_id == message->attackerId;
    if (victimIsLocal)
    {
        color = 0x109;
        strcpy(victimName, EQ_ARRAY(char, 0x0056B18C));
        if (message->attackerId != message->victimId)
        {
            sourceText = EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), this)(victim->name);
            strcpy(attackerName, sourceText);
        }
        else
        {
            strcpy(attackerName, EQ_ARRAY(char, 0x0056B180));
            color = 0x10a;
        }
        attackerIsVictim = message->attackerId == message->victimId;
        localHit = true;
    }
    else if (g_local_player_guess->spawn_id == message->victimId)
    {
        if (attacker != 0)
        {
            sourceText = EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), this)(attacker->name);
            strcpy(victimName, sourceText);
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0 && attacker->body_type < 0x41)
                EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = attacker;
        }
        strcpy(attackerName, EQ_ARRAY(char, 0x0056B14C));
        localHit = false;
        attackerIsVictim = true;
        color = 0x10a;
    }
    else
    {
        color = 0x117;
        if (hitType < 0xe7 && message->attackerId == message->victimId)
        {
            if (attacker != 0)
            {
                sourceText = EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), this)(attacker->name);
                strcpy(victimName, sourceText);
                if (attacker->gender == 0)
                    targetPronoun = (char*)EQ_ARRAY(char, 0x0056B134);
                else if (attacker->gender == 1)
                    targetPronoun = (char*)EQ_ARRAY(char, 0x0056B13C);
                else
                    targetPronoun = (char*)EQ_ARRAY(char, 0x0056B144);
                strcpy(attackerName, targetPronoun);
            }
        }
        else
        {
            if (attacker != 0)
            {
                sourceText = EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), this)(attacker->name);
                strcpy(victimName, sourceText);
            }
            sourceText = EQ_METHOD(1, char*, 0x004D0436, (void*, const char*), this)(victim->name);
            strcpy(attackerName, sourceText);
            attackerIsVictim = false;
            localHit = false;
        }
    }

    effectName = victimName[0] != '\0' ? victimName : attackerName;
    if (hitType >= 0xe8 && g_local_player_guess->spawn_id == message->victimId)
        effectName = attackerName;

    if (hitType < 0x27)
    {
        if (hitType == 7)
        {
            if (!localHit && damage > 0)
                strcpy(actionText, EQ_ARRAY(char, 0x0056AF54));
            else
                strcpy(actionText, EQ_ARRAY(char, 0x0056AC7C));
        }
        else if (hitType < 0x0b)
        {
            if (hitType == 0 || hitType == 2)
            {
                if (!localHit && damage > 0)
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B110);
                    strcpy(actionText, attackVerb);
                    specialLine = true;
                }
                else
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B108);
                    strcpy(actionText, attackVerb);
                }
                if (localHit && damage > 0)
                    bloodTarget = (void*)1;
            }
            else if (hitType == 1 || hitType == 3)
            {
                if (!localHit && damage > 0)
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B120);
                    strcpy(actionText, attackVerb);
                    specialLine = true;
                }
                else
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B118);
                    strcpy(actionText, attackVerb);
                }
                if (localHit && damage > 0)
                    bloodTarget = (void*)1;
            }
            else if (hitType == 8)
            {
                if (!localHit && damage > 0)
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B150);
                    strcpy(actionText, attackVerb);
                    specialLine = true;
                }
                else
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B128);
                    strcpy(actionText, attackVerb);
                }
                if (localHit && damage > 0)
                    bloodTarget = (void*)1;
            }
            else if (hitType == 10)
            {
                if (!localHit && damage > 0)
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B100);
                    strcpy(actionText, attackVerb);
                    specialLine = true;
                }
                else
                {
                    attackVerb = (char*)EQ_ARRAY(char, 0x0056B0F8);
                    strcpy(actionText, attackVerb);
                }
                if (localHit && damage > 0)
                    bloodTarget = (void*)1;
            }
            else
                goto make_attack_text;
        }
        else if (hitType == 0x15 || hitType == 0x17)
        {
            if (!localHit && damage > 0)
                strcpy(actionText, EQ_ARRAY(char, 0x0056AF64));
            else
                strcpy(actionText, EQ_ARRAY(char, 0x0056AF5C));
        }
        else if (hitType == 0x1a || hitType == 0x1e || hitType == 0x26)
        {
kick_verb:
            if (!localHit && damage > 0)
                strcpy(actionText, EQ_ARRAY(char, 0x0056AF6C));
            else
                strcpy(actionText, EQ_ARRAY(char, 0x00563A80));
        }
        else if (hitType == 0x24)
        {
            if (localHit)
                goto make_attack_text;
            if (damage > 0)
            {
                attackVerb = (char*)EQ_ARRAY(char, 0x0056B0F0);
                strcpy(actionText, attackVerb);
                specialLine = true;
            }
            else
            {
                attackVerb = (char*)EQ_ARRAY(char, 0x0056B0E8);
                strcpy(actionText, attackVerb);
            }
        }
        else
        {
make_attack_text:
            plural = !localHit && damage > 0;
            EQ_METHOD(3, void, 0x004C5C4D, (void*, char*, short, char), this)(actionText,
                attacker != 0 ? (short)attacker->race : 0, (char)plural);
        }
    }
    else
    {
        if (hitType < 0xf9)
        {
            if (hitType == 0xf8)
            {
                animationId = 0x0c;
                if (!attackerIsVictim && EQ_GLOBAL(int, 0x006B6F00) != 0)
                    suffixText[0] = '\0';
                else if (attackerIsVictim)
                {
                    sprintf(suffixText,
                        EQ_ARRAY(char, 0x0056AF44), effectName);
                }
                else
                {
                    sprintf(suffixText,
                        EQ_ARRAY(char, 0x0056AF34), effectName);
                }
                goto complete_message;
            }
            else if (hitType == 0x33)
            {
                if (!localHit && damage > 0)
                    strcpy(actionText, EQ_ARRAY(char, 0x0056AF54));
                else
                    strcpy(actionText, EQ_ARRAY(char, 0x0056AC7C));
                goto complete_message;
            }
            else if (hitType == 0x34)
            {
                if (!localHit && damage > 0)
                    strcpy(actionText, EQ_ARRAY(char, 0x0056AF64));
                else
                    strcpy(actionText, EQ_ARRAY(char, 0x0056AF5C));
                goto complete_message;
            }
            else if (hitType == 0x47)
                goto kick_verb; // 0x47 shares 0x1a/0x1e/0x26's verb pair (kick), not 0x34's
            else if (hitType == 0xe7)
            {
                if (spellId > 2999)
                    spellId = 2999;
                formatText = (char*)spellTable[spellId];
                color = 0x108;
                if (formatText == 0)
                {
                    EQTRACE_guess(EQ_ARRAY(char, 0x0056B05C));
                    return;
                }
                if (attackerIsVictim)
                {
                    if (damage < 1)
                        strcpy(suffixText, (char*)formatText + 0xe0);
                    else
                    {
                        attackVerb = damage < 2 ? EQ_ARRAY(char, 0x0056B00C) : EQ_ARRAY(char, 0x0056B034);
                        sprintf(suffixText, attackVerb,
                            (char*)formatText + 0xe0, damage);
                    }
                }
                else
                {
                    if (strlen((char*)formatText + 0x120) != 0)
                        sprintf(suffixText,
                            EQ_ARRAY(char, 0x0055DAC8), effectName, (char*)formatText + 0x120);
                    else
                    {
                        EQTRACE_guess(EQ_ARRAY(char, 0x0056AFDC), spellId, formatText);
                        suffixText[0] = '\0';
                    }
                }
                if ((formatText[0x22e] == 0x14 || formatText[0x22e] == 0x0d) && localHit && attackerIsVictim)
                {
                    actionText[0] = '\0';
                    suffixText[0] = '\0';
                    if (damage < 1)
                    {
                        strcpy(
                            (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8, actionText);
                        EQ_METHOD(3, void, 0x004CC92F, (void*, int, char*, char*),
                            EQ_GLOBAL(void*, 0x006EFE2C))(0,
                            EQ_ARRAY(char, 0x0058FBA4), EQ_ARRAY(char, 0x0058FBA4));
                        EQ_CDECL(void, 0x004A7F9B, (EQPlayer*, const char*))(attacker,
                            EQ_ARRAY(char, 0x0056AF74));
                        strcpy(actionText,
                            (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8);
                        goto complete_message;
                    }
                    sprintf(damageText,
                        EQ_ARRAY(char, 0x0056AFB8), victimName);
                    strcpy((char*)(socialData + 1), damageText);
                    reportedDamage = 1;
                    socialData[0] = g_local_player_guess->spawn_id;
                    send_message_guess(&EQ_GLOBAL(void*, 0x006B3194),
                        0x21d3, socialData, 0x204, 1);
                    if (EQ_GLOBAL(int, 0x006B6EE4) != 3)
                    {
                        sprintf(damageText,
                            EQ_ARRAY(char, 0x0056AF94));
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                            EQ_GLOBAL(void*, 0x006EFE2C))(damageText, 0x108);
                    }
                }
                else
                    actionText[0] = '\0';
                goto complete_message;
            }
            else if (hitType != 0xf6 && hitType != 0xf7)
            {
                plural = !localHit && damage > 0;
                EQ_METHOD(3, void, 0x004C5C4D, (void*, char*, short, char), this)(actionText,
                    attacker != 0 ? (short)attacker->race : 0, (char)plural);
                goto complete_message;
            }
            if (!attackerIsVictim && EQ_GLOBAL(int, 0x006B6F00) != 0)
            {
                suffixText[0] = '\0';
                goto complete_message;
            }
            animationId = 0x0c;
            if (hitType == 0xf6)
                sprintf(suffixText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056B09C) : EQ_ARRAY(char, 0x0056B08C), effectName);
            else
                sprintf(suffixText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056B0CC) : EQ_ARRAY(char, 0x0056B0B0), effectName);
            goto complete_message;
        }
        else if (hitType == 0xf9 || hitType == 0xfa)
        {
            if (!attackerIsVictim && EQ_GLOBAL(int, 0x006B6F00) != 0)
                suffixText[0] = '\0';
            else if (hitType == 0xf9)
            {
                animationId = 0x0c;
                sprintf(suffixText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056AE0C) : EQ_ARRAY(char, 0x0056ADF0), effectName);
            }
            else
            {
                animationId = 0x0f;
                sprintf(suffixText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056AE38) : EQ_ARRAY(char, 0x0056AE28), effectName);
            }
            goto complete_message;
        }
        else if (hitType == 0xfb)
        {
            if (attackerIsVictim && *(char*)((char*)victim->actor_info + 0x98) != '\0')
                g_pc_guess->airSupply_guess = '\0';
            animationId = 0x0f;
            sprintf(suffixText,
                attackerIsVictim ? EQ_ARRAY(char, 0x0056AE58) : EQ_ARRAY(char, 0x0056AE48),
                effectName[0] != '\0' ? effectName : attackerName);
            goto complete_message;
        }
        else if (hitType == 0xfc)
        {
            animationId = 0x0e;
            sprintf(suffixText,
                attackerIsVictim ? EQ_ARRAY(char, 0x0056AE88) : EQ_ARRAY(char, 0x0056AE6C), effectName);
            EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                victim->y, victim->x, victim->z, 0x73);
            goto complete_message;
        }
        else if (hitType == 0xfd)
        {
            animationId = 0x0e;
            sprintf(suffixText,
                attackerIsVictim ? EQ_ARRAY(char, 0x0056AF28) : EQ_ARRAY(char, 0x0056AF20), effectName);
            if (spellId == 0x8c || spellId == 0x91 || spellId == 0x92 || spellId == 0x96 || spellId == 0x97 || spellId == 0x98)
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                    victim->y, victim->x, victim->z, 0x73);
            if (spellId == 0x73 || spellId == 0x74)
                strcat(suffixText, EQ_ARRAY(char, 0x0056AEC4));
            else if (spellId == 0x78 || spellId == 0x7d)
                strcat(suffixText, EQ_ARRAY(char, 0x0056AEDC));
            else if (spellId == 0x82)
                strcat(suffixText, EQ_ARRAY(char, 0x0056AEF4));
            else if (spellId == 0x87)
                strcat(suffixText, EQ_ARRAY(char, 0x0056AF0C));
            else if (spellId == 0x8c || spellId == 0x91 || spellId == 0x92 || spellId == 0x96 || spellId == 0x97 || spellId == 0x98)
                strcat(suffixText, EQ_ARRAY(char, 0x0056AEA4));
            else
                strcat(suffixText, EQ_ARRAY(char, 0x0056AEB0));
            goto complete_message;
        }
        else
        {
            plural = !localHit && damage > 0;
            EQ_METHOD(3, void, 0x004C5C4D, (void*, char*, short, char), this)(actionText,
                attacker != 0 ? (short)attacker->race : 0, (char)plural);
            goto complete_message;
        }
    }

complete_message:
    outputName = victimName;
    if (strlen(outputName) == 0)
        strcpy(outputName, EQ_ARRAY(char, 0x0056ADDC));

    if (hitType < 0xe7)
    {
        reportedDamage = (unsigned short)damage;
        if (damage < 1)
        {
            if (damage == -6)
            {
                attackVerb = 0;
                sprintf(damageText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056AD20) : EQ_ARRAY(char, 0x0056ACFC),
                    localHit || message->attackerId == message->victimId ? attackerName : victimName);
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                    victim->y, victim->x, victim->z, 0x84);
            }
            else if (damage == -5)
            {
                attackVerb = 0;
                sprintf(damageText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056AD58) : EQ_ARRAY(char, 0x0056AD44),
                    localHit || message->attackerId == message->victimId ? attackerName : victimName);
            }
            else if (damage == -4)
            {
                attackVerb = 0;
                sprintf(damageText,
                    attackerIsVictim ? EQ_ARRAY(char, 0x0056AD78) : EQ_ARRAY(char, 0x0056AD6C),
                    localHit || message->attackerId == message->victimId ? attackerName : victimName);
            }
            else if (damage == -3)
                attackVerb = attackerIsVictim ? EQ_ARRAY(char, 0x0056AD90) : EQ_ARRAY(char, 0x0056AD84);
            else if (damage == -2)
                attackVerb = attackerIsVictim ? EQ_ARRAY(char, 0x0056ADA8) : EQ_ARRAY(char, 0x0056AD9C);
            else if (damage == -1)
                attackVerb = attackerIsVictim ? EQ_ARRAY(char, 0x0056ADC0) : EQ_ARRAY(char, 0x0056ADB4);
            else
            {
                if (localHit)
                    sprintf(damageText,
                        EQ_ARRAY(char, 0x0056ADD4));
                else
                    strcpy(damageText,
                        EQ_ARRAY(char, 0x0056ADCC));
                goto choose_animation;
            }
            if (attackVerb != 0)
                sprintf(damageText, attackVerb,
                    localHit || message->attackerId == message->victimId ? attackerName : victimName);
            if (attackVerb != 0)
            {
                soundId = EQ_METHOD(2, int, 0x004C707D, (CEverQuest*, EQPlayer*, EQPlayer*), this)(attacker, victim);
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                    victim->y, victim->x, victim->z, soundId);
            }
choose_animation:
            if (attackerIsVictim && EQ_GLOBAL(int, 0x006B69A8) == 0)
                doChat = '\0';
            status = localHit ? EQ_GLOBAL(int, 0x006B699C) : EQ_GLOBAL(int, 0x006B69A0);
            if ((localHit || !attackerIsVictim) && status == 0)
                doChat = '\0';
            finalText = actionText;
            if (localHit)
                sprintf(suffixText,
                    EQ_ARRAY(char, 0x0056ACE0), outputName, actionText, attackerName, damageText);
            else
                sprintf(suffixText,
                    EQ_ARRAY(char, 0x0056ACC4), outputName, actionText, attackerName, damageText);
        }
        else
        {
            status = *(int*)((char*)victim->actor_info + 0x1ec);
            if (status > 0x1f && status < 0x5a)
            {
                if (attackerIsVictim)
                    soundId = victim->cur_hp / damage;
                else
                    soundId = (victim->level * 0x1e) / damage;
                if (soundId >= 0x0a)
                    soundId = 0x93;
                else
                    soundId = 0x94;
                EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
                    victim->y, victim->x, victim->z, soundId);
            }
            memset(&damageEvent, 0, 0x24);
            damageEvent.victimId = message->victimId;
            damageEvent.attackerId = message->attackerId;
            damageEvent.unknown1C = message->hitType;
            damageEvent.unknown10 = message->unknown10;
            damageEvent.unknown14 = message->unknown14;
            damageEvent.unknown20 = (u16)message->spellId;
            EQ_METHOD(1, void, 0x004B3C9D, (void*, void*), victim)(&damageEvent);
            if (!attackerIsVictim && !localHit && EQ_GLOBAL(int, 0x006B69A4) == 0)
                goto finish_without_damage_text;
            if (damage == 1)
                attackVerb = (char*)EQ_ARRAY(char, 0x0056AC8C);
            else
                attackVerb = (char*)EQ_ARRAY(char, 0x0056ACA8);
            sprintf(damageText, attackVerb, damage);
            sprintf(suffixText,
                EQ_ARRAY(char, 0x0056AC80), outputName, actionText, attackerName, damageText);
            if (attacker != 0 && victim == attacker && victim->type == 1)
                doChat = '\0';
            reportedDamage = 1;
        }
        if (doChat != '\0' && suffixText[0] != '\0')
        {
            suffixText[0] = (char)EQ_CDECL(int, 0x005238FD, (int))(suffixText[0]);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(suffixText, color);
        }
    }
    else if (hitType == 0xe7)
    {
        if (attacker == 0)
            attacker = victim;
        if (formatText != 0)
        {
            if (message->attackerId != 0 && formatText[0x22e] != '\0')
            {
                if (strlen(actionText) != 0 && doChat != '\0' && attacker != 0)
                {
                    if (attacker->type == 0)
                    {
                        if (attacker->class_ == 8)
                        {
                            selectedStatus = EQ_GLOBAL(int, 0x006B6EEC);
                            if (attacker == g_local_player_guess)
                                groupMember = selectedStatus == 3;
                            else
                            {
                                targetFound = 0;
                                for (index = 0; index < 5; ++index)
                                {
                                    if ((EQPlayer*)targetList[index] == attacker)
                                    {
                                        targetFound = 1;
                                        break;
                                    }
                                }
                                groupMember = !targetFound && EQ_GLOBAL(int, 0x006B6EEC) == 0;
                            }
                        }
                        else
                        {
                            selectedStatus = EQ_GLOBAL(int, 0x006B6EE4);
                            if (attacker == g_local_player_guess)
                                groupMember = selectedStatus == 3;
                            else
                            {
                                targetFound = 0;
                                for (index = 0; index < 5; ++index)
                                {
                                    if ((EQPlayer*)targetList[index] == attacker)
                                    {
                                        targetFound = 1;
                                        break;
                                    }
                                }
                                groupMember = !targetFound && EQ_GLOBAL(int, 0x006B6EE4) == 0;
                            }
                        }
                    }
                    else
                        groupMember = EQ_GLOBAL(int, 0x006B6EE8) == 0;
                    if (groupMember)
                    {
                        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(actionText, color);
                    }
                }
                if (damage > 0 && formatText[0x21f] == '\0')
                    reportedDamage = (unsigned short)damage;
            }
            else
                goto spell_finished;
        }
        else
        {
spell_finished:
            reportedDamage = (unsigned short)damage;
        }
        if (doChat == '\0' || attacker == 0)
            goto finish_without_damage_text;
        if (attacker->type == 0)
        {
            if (attacker->class_ != 8)
            {
                selectedStatus = EQ_GLOBAL(int, 0x006B6EE4);
                if (attacker == g_local_player_guess)
                {
                    if (selectedStatus == 3)
                        goto finish_without_damage_text;
                }
                else
                {
                    targetFound = 0;
                    for (index = 0; index < 5; ++index)
                    {
                        if ((EQPlayer*)targetList[index] == attacker)
                        {
                            targetFound = 1;
                            break;
                        }
                    }
                    if (targetFound)
                        goto spell_message;
                    groupMember = selectedStatus == 0;
                    goto spell_group_check;
                }
            }
            else
            {
                selectedStatus = EQ_GLOBAL(int, 0x006B6EEC);
                if (attacker == g_local_player_guess)
                {
                    if (selectedStatus == 3)
                        goto finish_without_damage_text;
                }
                else
                {
                    targetFound = 0;
                    for (index = 0; index < 5; ++index)
                    {
                        if ((EQPlayer*)targetList[index] == attacker)
                            break;
                    }
                    if (index < 5)
                    {
                        targetFound = 1;
                        if (selectedStatus != 0)
                            groupMember = selectedStatus == 2;
                        else
                            groupMember = false;
                    }
                    else
                        groupMember = selectedStatus == 0;
                    if (selectedStatus != 0 && !targetFound)
                        groupMember = selectedStatus == 2;
                    if (!groupMember)
                        goto finish_without_damage_text;
                    goto spell_message;
                }
            }
        }
        else
        {
            groupMember = EQ_GLOBAL(int, 0x006B6EE8) == 0;
            if (!groupMember)
                goto finish_without_damage_text;
        }
spell_message:
        if (suffixText[0] != '\0')
        {
            suffixText[0] = (char)EQ_CDECL(int, 0x005238FD, (int))(suffixText[0]);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(suffixText, color);
        }
        goto finish_without_damage_text;
spell_group_check:
        if (selectedStatus != 0)
            goto finish_without_damage_text;
        if (suffixText[0] != '\0')
        {
            suffixText[0] = (char)EQ_CDECL(int, 0x005238FD, (int))(suffixText[0]);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(suffixText, color);
        }
    }
    else
    {
        reportedDamage = (unsigned short)damage;
        if (reportedDamage == 0)
            finalText = (char*)0xffffffff;
        else
        {
            if (doChat != '\0' && suffixText[0] != '\0')
            {
                suffixText[0] = (char)EQ_CDECL(int, 0x005238FD, (int))(suffixText[0]);
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(suffixText, color);
            }
        }
    }

finish_without_damage_text:
    if (victim->type > 1)
        return;
    if ((short)reportedDamage > 0)
    {
        if (victim == g_local_player_guess)
        {
            pinstEQ_Main_guess->playerHitFlag_guess = 1;
            pinstEQ_Main_guess->uiDirty_guess = 1;
            pinstEQ_Main_guess->memorizeMeditate_guess = 0;
            if (pinstEQ_Main_guess->scribingSpell_guess != -1)
            {
                sprintf(eventText,
                    EQ_ARRAY(char, 0x0055B0FC));
                pinstEQ_Main_guess->scribingSpell_guess = -1;
                pinstEQ_Main_guess->unk_0x6C54C = 0;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                    EQ_GLOBAL(void*, 0x006EFE2C))(eventText, 0x108);
            }
            if (pinstEQ_Main_guess->memorizingSpell_guess != -1)
            {
                sprintf(eventText,
                    EQ_ARRAY(char, 0x0055B0DC));
                pinstEQ_Main_guess->memorizingSpell_guess = -1;
                pinstEQ_Main_guess->unk_0x6C54E = 0;
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int),
                    EQ_GLOBAL(void*, 0x006EFE2C))(eventText, 0x108);
                pinstEQ_Main_guess->cursorSpell_guess = 0;
                *((char*)g_display_guess + 0x3c) = 0;
            }
            if (g_local_player_guess->stand_state == 'n' || g_local_player_guess->stand_state == 'i')
                EQ_METHOD(1, void, 0x004B45CB, (void*, int), g_local_player_guess)(100);
        }
        if (animationId == 0xffffffff)
            animationId = 0x0d;
        soundId = EQ_CDECL(int, 0x004A46F6, (int, int))(1, 4);
        if (soundId == 1)
            soundId = (int)(((u32)victim->actor_info & 0xffff0000) |
                (u16)victim->actor_info->unk_0x1EC_short_guess);
        else if (soundId == 2)
            soundId = (int)(((u32)victim->actor_info & 0xffff0000) |
                (u16)victim->actor_info->unk_0x1F0_short_guess);
        else if (soundId == 3)
            soundId = (int)(((u32)victim->actor_info & 0xffff0000) |
                (u16)victim->actor_info->unk_0x1F4_short_guess);
        else if (soundId == 4)
            soundId = (int)(((u32)victim->actor_info & 0xffff0000) |
                (u16)victim->actor_info->unk_0x1F8_short_guess);
        else
            goto after_hit_sound;
        EQ_METHOD(4, void, 0x00410D13, (void*, float, float, float, int), g_display_guess)(
            victim->y, victim->x, victim->z, soundId);
    }
after_hit_sound:
    if (bloodTarget == (void*)1)
    {
        bloodTarget = victim->actor_info->bld_dag;
        if (bloodTarget != 0)
            EQ_METHOD(1, void, 0x00412333, (void*, void*), g_display_guess)(bloodTarget);
    }
    if (animationId != 0xffffffff)
    {
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int),
            g_display_guess)(victim, (int)animationId, 1, 0, 1.0f, 1);
    }
}
#undef CEverQuest

// FUNCTION: TRILOGY 0x004c707d
// Selects the sound effect ID played for a riposte between two characters.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address; TAKP name
// CEverQuest_GetSoundEffectForRiposte, low)
// __thiscall (ReportSuccessfulHit), `this` unused. Sound id for a riposte between two spawns: 0x78 when both
// actors' +0x234 is 2, else 0x84; when the defender's actor +0x238 is 3, random_range(1, 2) == 1 makes it
// 0x7D.
int CEverQuest::GetSoundEffectForRiposte_guess(EQPlayer* attacker, EQPlayer* defender)
{
    int sound = 0x84;

    if (*(s32*)((char*)attacker->actor_info + 0x234) == 2 && *(s32*)((char*)defender->actor_info + 0x234) == 2)
        sound = 0x78;
    if (*(s32*)((char*)defender->actor_info + 0x238) == 3 && random_range_guess(1, 2) == 1)
        sound = 0x7D;
    return sound;
}

// FUNCTION: TRILOGY 0x004c70cb
// Refreshes the vial display in the game UI.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::refreshVial, low)
// __thiscall, no arguments, `this` unused. Invalidates the cached vial value and sets the redraw flag of
// the UI object at 0x006E4E44.
void CEverQuest::refreshVial_guess(void)
{
    if (g_eqmain_guess != NULL)
    {
        pinstEQ_Main_guess->hpBarShown_guess = -1;
        *(s32*)(g_eqmain_guess + 0xEB8) = 1;
    }
}

// FUNCTION: TRILOGY 0x004c70ec
// Sets the local character's death state and clears no-rent items when they are not kept.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address; TAKP name
// CEverQuest_ReportDeath_ClearOutRentables, low)
// __thiscall (ReportDeath), `this` and the first argument unused. Clears the two flags at 0x006B710C/D,
// copies the profile's level to the local spawn and recomputes its max hp; then, when the UI object, the
// local spawn (a player, type 0) and the profile exist: life state 4, clears the five dwords at 0x005FE954,
// sets 0x00590154 and, unless `keepRentables`, clears the rentable items (ClearOutRentables).
/* fake match: g_activeView_guess = 1 is stored before the zero loop (disjoint memory, same stores), which makes VC6 keep the constant 1 in ECX as the original does. */
void CEverQuest::ReportDeath_ClearOutRentables_guess(int unused, char keepRentables)
{
    int i;

    g_attacking_guess = 0;
    g_rangedAttacking_guess = 0;
    g_local_player_guess->level = g_pc_guess->level;
    g_local_player_guess->max_hp = EQ_Character_Max_HP_guess(g_pc_guess, 0);
    if (g_eqmain_guess != NULL && g_local_player_guess != NULL && g_pc_guess != NULL
        && g_local_player_guess->type == 0)
    {
        g_pc_guess->unknown0x0076 = 4;
        g_activeView_guess = 1;
        for (i = 0; i < 5; i++)
            g_death_state_guess[i] = 0;
        if (keepRentables == 0)
            ClearOutRentables(g_pc_guess, 1);
    }
}

// The class is a file-local stand-in: the name isn't in everquest.h and the headers aren't changed.
class CEverQuest_SaveAndSendDeath_guess
{
public:
    void PrepareLocalPCForRepop(void);
};

// FUNCTION: TRILOGY 0x004c717d
// Saves character settings and begins the death and repop update.
// __thiscall, no arguments, no return value. `this` goes on unchanged to saveOptions_guess. Then, when the UI
// object at 0x006E4E44 exists, saveCustom_guess runs with it as `this`. Last, SendPlayerDeath_guess runs on the
// profile at 0x006E4DA8 (no null check) with the dword at 0x00568308 as its one argument.
void CEverQuest_SaveAndSendDeath_guess::PrepareLocalPCForRepop(void)
{
    EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
    if (g_eqmain_guess != NULL)
        EQ_METHOD(0, void, 0x004567F8, (void*), g_eqmain_guess)();
    EQ_METHOD(1, void, 0x0048048E, (void*, u32), g_pc_guess)(EQ_GLOBAL(u32, 0x00568308));
}

class CEverQuest_SendPlayerProfileItems_guess
{
public:
    void SavePC_guess(int refit);
};

// FUNCTION: TRILOGY 0x004c71a3
// Sends the character profile to the world server, first refreshed from the live player's position, gear and items.
// on its dummy slot (xor edx,edx), and the rewrite loads ECX after the argument. The string pushes are relocations.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address; EQClassic OP_PlayerSave, guess)
// __thiscall, one argument (`refit`, nonzero: RefitNewbieEQ first and opcode 0x2154 instead of 0x2155).
// Saves the options, copies the local player's position and equipment into the profile, drops an
// unusable vehicle name, derives the two weapon models from the held items' idfile, then rebuilds the
// item-number (-1 = empty; only item_nr >= 1000) and 10-byte property tables of the inventory, its
// bags, the cursor bag, the bank and the bank bags from the live item pointers, and sends the 0x1FA4
// byte profile.
void CEverQuest_SendPlayerProfileItems_guess::SavePC_guess(int refit)
{
    char buf[1024];
    EQ_Item* item;
    int i;
    int j;

    EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
    if (g_eqmain_guess != NULL)
        EQ_METHOD(0, void, 0x004567F8, (void*), g_eqmain_guess)();
    if (EQ_GLOBAL(u8, 0x006E1DE5) == 0)
    {
        if (g_local_player_guess->type == 0)
        {
            g_pc_guess->y = g_local_player_guess->y;
            g_pc_guess->x = g_local_player_guess->x;
            g_pc_guess->z = g_local_player_guess->z;
            g_pc_guess->heading = g_local_player_guess->heading;
        }
        if (strlen(g_pc_guess->boat) > 0)
        {
            char* shipName = EQ_METHOD(1, char*, 0x004D024E, (void*, char*), this)(g_pc_guess->boat);
            if (EQ_METHOD(1, short, 0x004CE8B9, (void*, char*), this)(shipName) >= 0)
            {
                EQPlayer* vehicle = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, char*), EQ_GLOBAL(void*, 0x006E4D58))(
                     EQ_METHOD(1, char*, 0x004D024E, (void*, char*), this)(g_pc_guess->boat));
                if (vehicle != NULL)
                {
                    EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(vehicle);
                    g_pc_guess->y = g_local_player_guess->y;
                    g_pc_guess->x = g_local_player_guess->x;
                    g_pc_guess->z = g_local_player_guess->z;
                    g_pc_guess->heading = g_local_player_guess->heading;
                    EQ_CDECL(void, 0x004E1109, (EQPlayer*))(vehicle);
                }
                else
                {
                    EQTRACE_guess("Unable to find EQPlayer for vehicle called %s (in SavePC())\n",
                        g_pc_guess->boat);
                    g_pc_guess->boat[0] = '\0';
                }
            }
            else
            {
                EQTRACE_guess("No ticket found for the %s\n", g_pc_guess->boat);
                g_pc_guess->boat[0] = '\0';
            }
        }
    }
    if (g_pc_guess != NULL && EQ_GLOBAL(u8, 0x006B7111) != 0)
        ClearOutRentables(g_pc_guess, 1);
    g_pc_guess->primary_model = 0;
    g_pc_guess->secondary_model = 0;
    if (g_pc_guess->inventory_items[13] != NULL)
    {
        strcpy(buf, g_pc_guess->inventory_items[13]->idfile);
        strcpy(buf, buf + 2);
        if (is_number(buf) != 0)
            g_pc_guess->primary_model = atoi(buf);
    }
    if (g_pc_guess->inventory_items[14] != NULL)
    {
        strncpy(buf, g_pc_guess->inventory_items[14]->idfile, 0xf);
        strcpy(buf, buf + 2);
        if (is_number(buf) != 0)
            g_pc_guess->secondary_model = atoi(buf);
    }

    // inventory
    for (i = 0; i <= 29; i++)
    {
        g_pc_guess->inventory[i] = -1;
        g_pc_guess->inv_props[i].unknown0x00 = 0;
        g_pc_guess->inv_props[i].charges = 0;
    }
    for (i = 0; i <= 29; i++)
    {
        if (g_pc_guess->inventory_items[i] != NULL && g_pc_guess->inventory_items[i]->item_nr >= 1000)
        {
            g_pc_guess->inventory[i] = g_pc_guess->inventory_items[i]->item_nr;
            if (g_pc_guess->inventory_items[i]->type == 0)
                memcpy(&g_pc_guess->inv_props[i], (char*)g_pc_guess->inventory_items[i] + 0xd8, 10);
        }
    }

    // bags (8 bags x 10) and the cursor bag
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 10; j++)
        {
            g_pc_guess->bag_inventory[i * 10 + j] = -1;
            g_pc_guess->bag_props[i * 10 + j].unknown0x00 = 0;
            g_pc_guess->bag_props[i * 10 + j].charges = 0;
        }
    }
    item = g_pc_guess->inventory_items[0];
    if (item != NULL && item->type == 1)
    {
        for (i = 0; i < *((u8*)item + 0xd5); i++)
        {
            EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
            if (content != NULL && content->item_nr >= 1000)
            {
                g_pc_guess->cursor_bag_inventory[i] = content->item_nr;
                content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content->type == 0)
                    memcpy(&g_pc_guess->cursor_bag_props[i], (char*)content + 0xd8, 10);
            }
        }
    }
    for (j = 22; j <= 29; j++)
    {
        item = g_pc_guess->inventory_items[j];
        if (item != NULL && item->type == 1)
        {
            for (i = 0; i < *((u8*)item + 0xd5); i++)
            {
                EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content != NULL && content->item_nr >= 1000)
                {
                    g_pc_guess->bag_inventory[(j - 22) * 10 + i] = content->item_nr;
                    content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                    if (content->type == 0)
                        memcpy(&g_pc_guess->bag_props[(j - 22) * 10 + i], (char*)content + 0xd8, 10);
                }
            }
        }
    }

    // bank and bank bags
    for (i = 0; i < 8; i++)
    {
        g_pc_guess->bank_inventory[i] = -1;
        g_pc_guess->bank_props[i].unknown0x00 = 0;
        g_pc_guess->bank_props[i].charges = 0;
        for (j = 0; j < 10; j++)
        {
            g_pc_guess->bank_bag_inventory[i * 10 + j] = -1;
            g_pc_guess->bank_bag_props[i * 10 + j].unknown0x00 = 0;
            g_pc_guess->bank_bag_props[i * 10 + j].charges = 0;
        }
    }
    for (i = 0; i < 8; i++)
    {
        if (g_pc_guess->bank_items[i] != NULL && g_pc_guess->bank_items[i]->item_nr >= 1000)
        {
            g_pc_guess->bank_inventory[i] = g_pc_guess->bank_items[i]->item_nr;
            if (g_pc_guess->bank_items[i]->type == 0)
                memcpy(&g_pc_guess->bank_props[i], (char*)g_pc_guess->bank_items[i] + 0xd8, 10);
        }
    }
    for (j = 0; j < 8; j++)
    {
        item = g_pc_guess->bank_items[j];
        if (item != NULL && item->type == 1)
        {
            for (i = 0; i < *((u8*)item + 0xd5); i++)
            {
                EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content != NULL && content->item_nr >= 1000)
                {
                    g_pc_guess->bank_bag_inventory[j * 10 + i] = content->item_nr;
                    content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                    if (content->type == 0)
                        memcpy(&g_pc_guess->bank_bag_props[j * 10 + i], (char*)content + 0xd8, 10);
                }
            }
        }
    }

    // equipment materials and colours of the local player
    for (i = 0; i < 9; i++)
    {
        if (g_local_player_guess != NULL)
        {
            g_pc_guess->equipment[i] = g_local_player_guess->equipment[i];
            *(u32*)((char*)g_pc_guess + 0x50 + i * 4) = g_local_player_guess->equip_colors[i];
        }
    }
    if (refit != 0)
    {
        EQ_METHOD(0, void, 0x004806CB, (void*), g_pc_guess)();
        send_message_guess(g_world_connection_guess, 0x2154, g_pc_guess, 0x1fa4, 1);
    }
    else
    {
        send_message_guess(g_world_connection_guess, 0x2155, g_pc_guess, 0x1fa4, 1);
    }
    EQ_GLOBAL(u8, 0x006E11E0) = 0;
    EQ_GLOBAL(int, 0x006B63E8) = 0;
}

// The 0x80-byte sprite effect DoSpriteEffect creates; its constructor is the client's.
class SpriteEffect_guess
{
public:
    // EXTERNAL: TRILOGY 0x00490C8E SpriteEffect_guess::SpriteEffect_guess
    SpriteEffect_guess(int unknown, char* name, short id, float* origin, void* destination, void* target,
        char* offsets, float scale, short value300, short value318, float value330, float value378);
    char data_guess[0x80];
};

// FUNCTION: TRILOGY 0x004c784d
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __cdecl. Builds the effect(s) selected by the data row and returns the first surviving effect.
void* DoSpriteEffect(int effectType, char* effectData, void* effectSource, void* effectDestination,
    float* effectPosition, void* effectTarget)
{
    typedef float (__cdecl *SpriteEffectTrig_guess)(float);
    int count;
    int index;
    int tableOffset;
    char* row;
    char* string;
    void* firstEffect;
    EQEffect_guess* effect;
    void* checkedSource;
    float origin[3];
    float step;
    float heading;
    float distance;
    int value;

    count = 0;
    firstEffect = 0;
    checkedSource = effectSource;
    if (effectType == 2)
        checkedSource = effectDestination;
    if (checkedSource == 0 && (checkedSource = effectSource, checkedSource == 0))
    {
        return 0;
    }

    tableOffset = effectType * 0x3a0;
    row = effectData + tableOffset;
    index = 0;
    string = row + 0xa0;
    effectData = string;
    do
    {
        if (strlen(effectData) <= 0)
            break;
        count++;
        index++;
        effectData += 0x20;
    } while (index < 0xc);
    effectData = row - tableOffset;

    value = ((EffectDataRow_guess*)row)->spritePlacementMode_guess;
    switch (value)
    {
    case 0:
    {
        if (effectTarget == 0)
            return 0;
        index = 0;
        if ((short)count > 0)
        {
            do
            {
                origin[0] = *(float*)((char*)effectTarget + 0xc);
                origin[1] = *(float*)((char*)effectTarget + 0x10);
                origin[2] = *(float*)((char*)effectTarget + 0x14);
                effect = (EQEffect_guess*)new SpriteEffect_guess(0, effectData + (effectType * 0x1d + 5 + index) * 0x20,
                    *(unsigned short*)(row + 0x360 + index * 2), origin, 0, effectTarget,
                    effectData + tableOffset + (index + 0xe4) * 3, *(float*)(row + 0x2d0 + index * 4),
                    *(unsigned short*)(row + 0x300 + index * 2), *(unsigned short*)(row + 0x318 + index * 2),
                    *(float*)(row + 0x330 + index * 4), *(float*)(row + 0x378 + index * 4));
                if (index == 0)
                    firstEffect = effect;
                if (*(int*)((char*)effect + 0x60) == 0)
                {
                    if (index == 0)
                        firstEffect = 0;
                    if (effect != 0)
                    {
                        EQ_METHOD(0, void, 0x00490EF4, (void*), (void*)effect)();
                        EQ_CDECL(void, 0x005218BB, (void*))(effect);
                    }
                }
                else
                {
                    effect->heading = *(float*)((char*)effectTarget + 0x18);
                    effect->pitch = *(float*)((char*)effectTarget + 0x1c);
                    effect->roll = *(float*)((char*)effectTarget + 0x38);
                    effect->speed_guess = *(float*)((char*)effectTarget + 0x20);
                    effect->velocityY_guess = *(float*)((char*)effectTarget + 0x24);
                    effect->velocityX_guess = *(float*)((char*)effectTarget + 0x28);
                    effect->velocityZ_guess = *(float*)((char*)effectTarget + 0x2c);
                    value = effect->headingOffset;
                    heading = EQ_METHOD(1, float, 0x00408156, (void*, float), g_display_guess)(
                            (float)value + effect->heading);
                    distance = EQ_GLOBAL(SpriteEffectTrig_guess, 0x006E524C)(heading);
                    *(float*)((char*)effect) = distance * effect->scale
                        + *(float*)((char*)effect);
                    distance = EQ_GLOBAL(SpriteEffectTrig_guess, 0x006E5250)(heading);
                    effect->x = distance * effect->scale
                        + effect->x;
                    value = effect->rollOffset;
                    heading = EQ_METHOD(1, float, 0x00408156, (void*, float), g_display_guess)(
                            (float)value + effect->roll
                                + EQ_GLOBAL(float, 0x005350F8));
                    distance = EQ_GLOBAL(SpriteEffectTrig_guess, 0x006E5250)(heading);
                    effect->z = distance * effect->scale
                        + effect->z;
                }
                index++;
            } while ((short)count > index);
        }
        break;
    }
    case 1:
    {
        if (effectPosition != 0)
        {
            origin[0] = effectPosition[0];
            origin[1] = effectPosition[1];
            origin[2] = effectPosition[2];
        }
        else if (effectTarget != 0)
        {
            origin[0] = *(float*)((char*)effectTarget + 0xc);
            origin[1] = *(float*)((char*)effectTarget + 0x10);
            origin[2] = *(float*)((char*)effectTarget + 0x14);
        }
        else
        {
            checkedSource = effectDestination;
            if (checkedSource == 0)
                checkedSource = effectSource;
            if (checkedSource == 0)
            {
                origin[0] = 0.0f;
                origin[2] = 8.0f;
            }
            else
            {
                origin[0] = *(float*)((char*)checkedSource + 0x34);
                origin[1] = *(float*)((char*)checkedSource + 0x38);
                origin[2] = *(float*)((char*)checkedSource + 0x3c);
            }
        }
        effect = (EQEffect_guess*)new SpriteEffect_guess(0, row + 0xa0,
            ((EffectDataRow_guess*)row)->unk_0x360[0], origin, 0, effectTarget,
            row + 0x2ac, ((EffectDataRow_guess*)row)->spriteScale_guess[0],
            ((EffectDataRow_guess*)row)->unk_0x300[0], ((EffectDataRow_guess*)row)->unk_0x318[0],
            ((EffectDataRow_guess*)row)->unk_0x330[0], ((EffectDataRow_guess*)row)->unk_0x378[0]);
        firstEffect = effect;
        if (*(int*)((char*)effect + 0x60) == 0)
        {
            if (effect != 0)
            {
                EQ_METHOD(0, void, 0x00490EF4, (void*), (void*)effect)();
                EQ_CDECL(void, 0x005218BB, (void*))(effect);
            }
            firstEffect = 0;
        }
        break;
    }
    case 2:
    {
        value = (short)count;
        step = EQ_GLOBAL(float, 0x00535108) / (float)value;
        if (effectDestination != 0)
        {
            origin[0] = *(float*)((char*)effectDestination + 0x34);
            origin[1] = *(float*)((char*)effectDestination + 0x38);
            origin[2] = *(float*)((char*)effectDestination + 0x3c);
            index = 0;
            if ((short)count > 0)
            {
                do
                {
                    effect = (EQEffect_guess*)new SpriteEffect_guess(0, effectData + (effectType * 0x1d + 5 + index) * 0x20,
                        *(unsigned short*)(row + 0x360 + index * 2), origin, effectDestination, effectTarget,
                        effectData + tableOffset + (index + 0xe4) * 3, *(float*)(row + 0x2d0 + index * 4),
                        *(unsigned short*)(row + 0x300 + index * 2), *(unsigned short*)(row + 0x318 + index * 2),
                        *(float*)(row + 0x330 + index * 4), *(float*)(row + 0x378 + index * 4));
                    if (index == 0)
                        firstEffect = effect;
                    if (*(int*)((char*)effect + 0x60) == 0)
                    {
                        if (index == 0)
                            firstEffect = 0;
                        if (effect != 0)
                        {
                            EQ_METHOD(0, void, 0x00490EF4, (void*), (void*)effect)();
                            EQ_CDECL(void, 0x005218BB, (void*))(effect);
                        }
                    }
                    else
                    {
                        effect->heading = (float)index * step;
                    }
                    index++;
                } while (index < value);
            }
        }
        break;
    }
    default:
        break;
    }

    return firstEffect;
}

// FUNCTION: TRILOGY 0x004c7cdb
// Chooses the model attachment point for placing a character effect.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: GetDagName, low)
// __cdecl. Writes the actor's attachment-point DAG name for `point` into `out`: 1 head, 2 right / 3 left
// hand point, 4 / 5 right / left back (BO_R / BO_L), anything else the chest (CH). Returns 1.
int GetDagName_guess(char* actorTag, char* out, int point)
{
    switch (point)
    {
    case 0:
        sprintf(out, "%sCH_DAG", actorTag);
        break;
    case 1:
        sprintf(out, "%sHEAD_POINT_DAG", actorTag);
        break;
    case 2:
        sprintf(out, "%sR_POINT_DAG", actorTag);
        break;
    case 3:
        sprintf(out, "%sL_POINT_DAG", actorTag);
        break;
    case 4:
        sprintf(out, "%sBO_R_DAG", actorTag);
        break;
    case 5:
        sprintf(out, "%sBO_L_DAG", actorTag);
        break;
    default:
        sprintf(out, "%sCH_DAG", actorTag);
        break;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004c7d3f
// Creates one particle cloud from an effect definition and applies its properties and attachment.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: NULL-path cleanup is one add esp,0x10 (original pops per call); sectionIndex is cached in EDI across the blit (original reloads it)
int MakePCloudInstance(int effectData, int sectionIndex, int i, int particle,
    int location, int cloudAttachment)
{
    int value;
    int vectorOffset;
    float propertyValue;
    int fieldIndex;

    if (EQ_ARRAY(int, 0x006B3198)[particle] == 0)
    {
        EQTRACE_guess
            ((const char *)0x0056B2DC, particle);
        DbgLog_guess((const char *)0x0056B2DC, particle);
        return 0;
    }
    particle = ((int (__cdecl *)(int, int, int, int, int))EQ_GLOBAL(void *, 0x006E5198))
        (*(int *)((char *)g_display_guess + 4),
        EQ_ARRAY(int, 0x006B3198)[particle], location, 0, 0);
    if (particle != 0)
    {
        fieldIndex = ((int (__cdecl *)(int, int))EQ_GLOBAL(void *, 0x006E52E0))
            (*(int *)((char *)g_display_guess + 4),
            (sectionIndex * 0x1d + i) * 0x20 + 8 + effectData);
        if (fieldIndex != 0)
        {
            ((void (__cdecl *)(int, int, int))EQ_GLOBAL(void *, 0x006E53F4))
                (*(int *)((char *)g_display_guess + 4), particle, fieldIndex);
        }
        fieldIndex = sectionIndex * 0xe8 + i;
        propertyValue = *(float *)(effectData + 0x234 + fieldIndex * 4);
        if (propertyValue != EQ_GLOBAL(float, 0x0053510C))
        {
            ((void (__cdecl *)(int, float))EQ_GLOBAL(void *, 0x006E5404))(particle, propertyValue);
        }
        value = sectionIndex * 0x3a0 + (i * 3 + 0x90) * 4;
        if ((*(float *)(value + effectData) != EQ_GLOBAL(float, 0x0053510C)) ||
            ((vectorOffset = sectionIndex * 0x3a0 + i * 0xc,
            *(float *)(vectorOffset + 0x244 + effectData) != EQ_GLOBAL(float, 0x0053510C)) ||
            (*(float *)(vectorOffset + effectData + 0x248) != EQ_GLOBAL(float, 0x0053510C))))
        {
            ((void (__cdecl *)(int, int))EQ_GLOBAL(void *, 0x006E5410))
                (particle, value + effectData);
        }
        propertyValue = *(float *)(effectData + 0x264 + fieldIndex * 4);
        if (propertyValue != EQ_GLOBAL(float, 0x0053510C))
        {
            ((void (__cdecl *)(int, float))EQ_GLOBAL(void *, 0x006E5414))(particle, propertyValue);
        }
        propertyValue = *(float *)(effectData + 0x270 + fieldIndex * 4);
        if (propertyValue != EQ_GLOBAL(float, 0x0053510C))
        {
            ((void (__cdecl *)(int, float))EQ_GLOBAL(void *, 0x006E5408))(particle, propertyValue);
        }
        value = *(int *)(effectData + 0x27c + fieldIndex * 4);
        if (value != 0)
        {
            ((void (__cdecl *)(int, int))EQ_GLOBAL(void *, 0x006E540C))(particle, value);
        }
        propertyValue = *(float *)(effectData + 0x288 + fieldIndex * 4);
        if (propertyValue != EQ_GLOBAL(float, 0x0053510C))
        {
            ((void (__cdecl *)(int, float))EQ_GLOBAL(void *, 0x006E5420))(particle, propertyValue);
        }
        value = *(int *)(effectData + 0x294 + fieldIndex * 4);
        if (value != 0)
        {
            ((void (__cdecl *)(int, int))EQ_GLOBAL(void *, 0x006E5418))(particle, value);
        }
        propertyValue = *(float *)(effectData + 0x2a0 + fieldIndex * 4);
        if (propertyValue != EQ_GLOBAL(float, 0x0053510C))
        {
            ((void (__cdecl *)(int, float))EQ_GLOBAL(void *, 0x006E541C))(particle, propertyValue);
        }
        if ((*(char *)(effectData + 0x22a + fieldIndex * 4) != 0) ||
            ((*(char *)(effectData + 0x229 + fieldIndex * 4) != 0) ||
            (*(char *)(effectData + 0x228 + fieldIndex * 4) != 0)))
        {
            ((void (__cdecl *)(int, int))EQ_GLOBAL(void *, 0x006E53F8))
                (particle, effectData + 0x228 + fieldIndex * 4);
        }
        if (cloudAttachment != 0)
        {
            ((void (__cdecl *)(int, int, int))EQ_GLOBAL(void *, 0x006E5400))
                (*(int *)((char *)g_display_guess + 4), particle, cloudAttachment);
        }
    }
    return particle;
}

// FUNCTION: TRILOGY 0x004c7f71
// Spawns one effect section's particle clouds (fewer below level 39) on the source/target spawn or at a point.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: the floor branch reloads z after the FCOMP (the original loads it before, once), and each EQ_THISCALL adds a dummy xor edx
int DoParticleEffect_guess_004c7f71(int sectionIndex, char* effectData, EQPlayer* sourceSpawn, EQPlayer* targetSpawn,
    float* unusedArg, float* position, void* obj, int cloudArg)
{
    EQPlayer* selectedSpawn = 0;
    int i = 0;
    int k;
    int base;
    int effectOffset;
    int* effectSlot;
    int result = 0;
    short level = 1;
    /* z is copied as a dword (MOV, as the original does), not as a float: a float copy goes through the FPU */
    float location[3];
    char actorTag[0x20];
    char dagName[0x80];

    location[0] = 0.0f;
    location[1] = 0.0f;
    location[2] = 0.0f;
    if (sourceSpawn != 0 && sourceSpawn->type == 0)
        level = sourceSpawn->level;

    base = sectionIndex * 0xe8;
    for (; i < 3; i++)
    {
        effectSlot = (int*)(effectData + (base + i + 0x25) * 4);
        if (*effectSlot == -1)
            continue;

        if (level < 0x27)
        {
            switch (i)
            {
            case 0:
                if (level < 0x0a && sectionIndex == 1)
                    continue;
                break;
            case 1:
                if (level < 0x18)
                    continue;
                break;
            case 2:
                continue;
            }
        }

        effectOffset = sectionIndex * 0x3a0;
        if (_strcmpi(effectData + effectOffset + 0x68, EQ_ARRAY(char, 0x0056B314)) == 0)
            selectedSpawn = sourceSpawn;
        else if (_strcmpi(effectData + effectOffset + 0x68, EQ_ARRAY(char, 0x00565A90)) == 0)
            selectedSpawn = targetSpawn;

        if (selectedSpawn == 0)
        {
            selectedSpawn = sourceSpawn;
            if (selectedSpawn == 0)
                return 0;
        }

        switch (*effectSlot)
        {
        case 5:
            if (position != 0)
            {
                location[0] = position[0];
                location[1] = position[1];
                location[2] = position[2];
            }
            else
            {
                if (obj != 0)
                {
                    location[0] = *(float*)((char*)obj + 0x0c);
                    location[1] = *(float*)((char*)obj + 0x10);
                    *(int*)&location[2] = *(int*)((char*)obj + 0x14);
                }
                else
                {
                    if (selectedSpawn == 0)
                        continue;
                    location[0] = selectedSpawn->y;
                    location[1] = selectedSpawn->x;
                    *(int*)&location[2] = *(int*)&selectedSpawn->z;
                }

                if (!(*(float*)(effectData + effectOffset + 0x248 + i * 0x0c)
                    >= EQ_GLOBAL(float, 0x0053510C)))
                    location[2] += EQ_GLOBAL(float, 0x00535124);
                else
                    location[2] = EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(location[0], location[1], location[2]);
            }

            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 6, (int)location, cloudArg);
            if (obj != 0)
                continue;

            EQ_METHOD(1, void, 0x004B156C, (void*, char*), selectedSpawn)(actorTag);
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 6, (int)location, cloudArg);
            if (*(int*)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250) == 0)
                continue;

            EQ_CDECL(int, 0x004C7CDB, (char*, char*, int))
                (actorTag, dagName, *(int*)(effectData + (base + i) * 4 + 0x88));
            {
                void* dag;
                dag = ((void* (__cdecl *)(void*, const char*))EQ_GLOBAL(void*, 0x006E52C8))
                    (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), dagName);
                if (dag == 0)
                    dag = ((void* (__cdecl *)(void*, int))EQ_GLOBAL(void*, 0x006E52C4))
                        (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), 0);
                if (result != 0 && dag != 0 && position == 0)
                    ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))(result, (int)dag);
            }
            continue;

        case 3:
            if (position == 0)
            {
                if (obj != 0)
                {
                    location[0] = *(float*)((char*)obj + 0x0c);
                    location[1] = *(float*)((char*)obj + 0x10);
                    *(int*)&location[2] = *(int*)((char*)obj + 0x14);
                }
                else
                {
                    if (selectedSpawn == 0)
                        continue;
                    location[0] = selectedSpawn->y;
                    location[1] = selectedSpawn->x;
                    *(int*)&location[2] = *(int*)&selectedSpawn->z;
                }

                if (!(*(float*)(effectData + effectOffset + 0x248 + i * 0x0c)
                    >= EQ_GLOBAL(float, 0x0053510C)))
                    location[2] += EQ_GLOBAL(float, 0x00535124);
                else
                    location[2] = EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(location[0], location[1], location[2]);
            }
            else
            {
                location[0] = position[0];
                location[1] = position[1];
                location[2] = position[2];
            }
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 3, (int)location, cloudArg);
            continue;

        case 4:
            if (position == 0)
            {
                if (obj != 0)
                {
                    location[0] = *(float*)((char*)obj + 0x0c);
                    location[1] = *(float*)((char*)obj + 0x10);
                    *(int*)&location[2] = *(int*)((char*)obj + 0x14);
                }
                else
                {
                    if (selectedSpawn == 0)
                        continue;
                    location[0] = selectedSpawn->y;
                    location[1] = selectedSpawn->x;
                    *(int*)&location[2] = *(int*)&selectedSpawn->z;
                }

                if (!(*(float*)(effectData + effectOffset + 0x248 + i * 0x0c)
                    >= EQ_GLOBAL(float, 0x0053510C)))
                    location[2] += EQ_GLOBAL(float, 0x00535124);
                else
                    location[2] = EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(location[0], location[1], location[2]);
            }
            else
            {
                location[0] = position[0];
                location[1] = position[1];
                location[2] = position[2];
            }
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 4, (int)location, cloudArg);
            continue;

        case 2:
            if (position != 0)
            {
                location[0] = position[0];
                location[1] = position[1];
                location[2] = position[2];
            }
            else if (obj != 0)
            {
                location[0] = *(float*)((char*)obj + 0x0c);
                location[1] = *(float*)((char*)obj + 0x10);
                *(int*)&location[2] = *(int*)((char*)obj + 0x14);
            }
            else if (selectedSpawn != 0)
            {
                location[0] = selectedSpawn->y;
                location[1] = selectedSpawn->x;
                *(int*)&location[2] = *(int*)&selectedSpawn->z;
            }
            else
            {
                location[0] = 0.0f;
                location[1] = 0.0f;
                location[2] = 0.0f;
            }
            EQ_METHOD(1, void, 0x004B156C, (void*, char*), selectedSpawn)(actorTag);
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 1, (int)location, cloudArg);
            if (*(int*)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250) == 0)
                continue;
            EQ_CDECL(int, 0x004C7CDB, (char*, char*, int))
                (actorTag, dagName, *(int*)(effectData + (base + i) * 4 + 0x88));
            {
                void* dag;
                dag = ((void* (__cdecl *)(void*, const char*))EQ_GLOBAL(void*, 0x006E52C8))
                    (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), dagName);
                if (dag == 0)
                    dag = ((void* (__cdecl *)(void*, int))EQ_GLOBAL(void*, 0x006E52C4))
                        (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), 0);
                if (result != 0 && dag != 0 && position == 0)
                    ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))(result, (int)dag);
            }
            continue;

        case 1:
            if (position != 0)
            {
                location[0] = position[0];
                location[1] = position[1];
                location[2] = position[2];
            }
            else
            {
                location[0] = 0.0f;
                location[1] = 0.0f;
                location[2] = 0.0f;
            }
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 5, (int)location, cloudArg);
            if (result == 0)
                continue;

            if (obj != 0)
            {
                int* spawnClouds = *(int**)((char*)obj + 0x3c);
                if (spawnClouds != 0 && spawnClouds[0] != 0)
                {
                    ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))
                        (result, spawnClouds[0]);
                    for (k = 0x2c8; k < 0x2d4; k += 4)
                    {
                        int* cloud = (int*)(*(int*)((char*)obj + 0x3c) + k);
                        if (*cloud == 0)
                            *cloud = result;
                    }
                    continue;
                }
            }

            if (position != 0 || selectedSpawn == 0)
                continue;
            EQ_METHOD(1, void, 0x004B156C, (void*, char*), selectedSpawn)(actorTag);
            if (*(int*)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250) == 0)
                continue;
            EQ_CDECL(int, 0x004C7CDB, (char*, char*, int))
                (actorTag, dagName, *(int*)(effectData + (base + i) * 4 + 0x88));
            {
                void* dag;
                dag = ((void* (__cdecl *)(void*, const char*))EQ_GLOBAL(void*, 0x006E52C8))
                    (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), dagName);
                if (dag == 0)
                    dag = ((void* (__cdecl *)(void*, int))EQ_GLOBAL(void*, 0x006E52C4))
                        (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), 0);
                if (dag != 0)
                    ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))(result, (int)dag);
                else
                    EQ_CDECL(int, 0x004C17BC, (const char*, ...))
                        (EQ_ARRAY(char, 0x0056B2FC), 0);
            }
            continue;

        case 0:
            EQ_METHOD(1, void, 0x004B156C, (void*, char*), selectedSpawn)(actorTag);
            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 0, (int)location, cloudArg);
            if (*(int*)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250) != 0 && result != 0)
            {
                sprintf(dagName, EQ_ARRAY(char, 0x0053E3A8), actorTag);
                {
                    void* dag;
                    dag = ((void* (__cdecl *)(void*, const char*))EQ_GLOBAL(void*, 0x006E52C8))
                        (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), dagName);
                    if (dag == 0)
                        dag = ((void* (__cdecl *)(void*, int))EQ_GLOBAL(void*, 0x006E52C4))
                            (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), 0);
                    if (dag != 0)
                        ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))(result, (int)dag);
                }
            }

            result = EQ_CDECL(int, 0x004C7D3F, (int, int, int, int, int, int))
                ((int)effectData, sectionIndex, i, 0, (int)location, cloudArg);
            if (*(int*)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250) != 0 && result != 0)
            {
                sprintf(dagName, EQ_ARRAY(char, 0x0053E3B8), actorTag);
                {
                    void* dag;
                    dag = ((void* (__cdecl *)(void*, const char*))EQ_GLOBAL(void*, 0x006E52C8))
                        (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), dagName);
                    if (dag == 0)
                        dag = ((void* (__cdecl *)(void*, int))EQ_GLOBAL(void*, 0x006E52C4))
                            (*(void**)((char*)*(void**)((char*)selectedSpawn + 0x70) + 0x250), 1);
                    if (dag != 0)
                        ((void (__cdecl *)(int, int))EQ_GLOBAL(void*, 0x006E5160))(result, (int)dag);
                }
            }
            continue;
        }
    }

    return result;
}

// The audio engine object (0xBD8 bytes); its constructor 0x004ECC9F is ConstructEngine_guess below, called
// through this declaration.
struct FUN_004ecc9f_object_guess
{
    unsigned char unknown[0xbd8];

    // EXTERNAL: TRILOGY 0x004ECC9F FUN_004ecc9f_object_guess::FUN_004ecc9f_object_guess
    FUN_004ecc9f_object_guess();
};

// FUNCTION: TRILOGY 0x004ecb4d
// Creates the client's audio engine interface.
void *CreateAudioInterface_guess(void)
{
    return new FUN_004ecc9f_object_guess;
}

// FUNCTION: TRILOGY 0x004ecc9f
// Initializes the audio engine and starts its processing thread.
unsigned int * __fastcall ConstructEngine_guess(unsigned int *engine)
{
    HANDLE hThread;
    int remaining;
    unsigned int *fieldPtr;

    engine[0] = (unsigned int)EQ_ARRAY(void *, 0x00535898);
    EQ_GLOBAL(FILE *, 0x006EFEA8) = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x0053DBA8));
    if (EQ_GLOBAL(FILE *, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (FILE *, const char *, ...))
            (EQ_GLOBAL(FILE *, 0x006EFEA8), EQ_ARRAY(char, 0x0057E368));
        EQ_CDECL(int, 0x00521FBB, (FILE *))(EQ_GLOBAL(FILE *, 0x006EFEA8));
    }

    engine[0x2F5] = 0;
    *((unsigned char *)engine + 0xBC8) = 0;
    *((unsigned char *)engine + 0xBC9) = 0;
    *((unsigned char *)engine + 0xBCA) = 0;
    engine[0x2F3] = 8;
    engine[0x2D6] = 0;
    engine[0x2D7] = 0;
    engine[0x2D8] = 0;
    engine[0x2F1] = 0;
    engine[0x2D5] = 0;
    engine[0x2D4] = 0;
    fieldPtr = engine + 3;
    for (remaining = 0x2D0; remaining != 0; remaining = remaining - 1)
    {
        *fieldPtr = 0;
        fieldPtr = fieldPtr + 1;
    }
    engine[0x2D3] = 0;
    engine[2] = 0;
    EQ_GLOBAL(int, 0x006EFE48) = 0;
    hThread = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE,
        LPVOID, DWORD, LPDWORD))EQ_GLOBAL(void *, 0x005341D0))
        (0, 0, (LPTHREAD_START_ROUTINE)0x004EE325, 0, 0, (LPDWORD)(engine + 1));
    engine[2] = (unsigned int)hThread;
    ((BOOL (WINAPI *)(HANDLE, int))EQ_GLOBAL(void *, 0x005340DC))(hThread, -1);
    EQ_GLOBAL(FILE *, 0x006EFEA8) = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    if (EQ_GLOBAL(FILE *, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (FILE *, const char *, ...))
            (EQ_GLOBAL(FILE *, 0x006EFEA8), EQ_ARRAY(char, 0x0057E350));
        EQ_CDECL(int, 0x00521FBB, (FILE *))(EQ_GLOBAL(FILE *, 0x006EFEA8));
    }
    return engine;
}

// FUNCTION: TRILOGY 0x004c85f4
// Plays the sound and launches the visual effect for a spell effect event.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
int __cdecl DoSpellEffect(int effectType, int effectContext, int effectSource, int effectDestination,
    u32 effectPosition, u32 effectTarget, u32 cloudArg)
{
    int soundSource;
    int soundId;
    int canShowEffect;
    u32 spriteEffect;

    spriteEffect = 0;
    if ((((effectSource != 0) && (effectContext != 0)) &&
        (*(int *)((char *)effectContext + 600) != 0)) &&
        ((effectType >= 0 && (effectType <= 2))))
    {
        if ((effectType != 1) &&
            (soundId = *(int *)((char *)(*(int *)((char *)effectContext + 600)) +
                effectType * 0x3a0 + 0x224), soundId != -1))
        {
            soundSource = effectSource;
            if (effectType == 2)
                soundSource = effectDestination;
            if (soundSource != 0)
            {
                EQ_METHOD(5, void, 0x0040E3EA, (void *, int, int, float, float, float), g_display_guess)(0xff, soundId,
                     *(float *)((char *)soundSource + 0x34),
                     *(float *)((char *)soundSource + 0x38),
                     *(float *)((char *)soundSource + 0x3c));
            }
        }
        if (((EQ_GLOBAL(int, 0x006E20E8) != 0) || (effectType == 1)) ||
            (canShowEffect = EQ_METHOD(0, int, 0x004875F6, (void *), (void *)effectContext)(), canShowEffect == 1))
        {
            if (*(int *)((char *)(*(int *)((char *)effectContext + 600)) +
                effectType * 0x3a0 + 0x220) != -1)
            {
                spriteEffect = EQ_CDECL(int, 0x004C784D,
                    (int, int, int, int, u32, u32))
                    (effectType, *(int *)((char *)effectContext + 600), effectSource,
                     effectDestination, effectPosition, effectTarget);
            }
            EQ_CDECL(int, 0x004C7F71,
                (int, int, int, int, u32, u32, u32, int))
                (effectType, *(int *)((char *)effectContext + 600), effectSource,
                 effectDestination, spriteEffect, effectPosition, effectTarget, cloudArg);
        }
        return 1;
    }
    return 0;
}

// File-local declarations for CEverQuest methods absent from the public header.
class CEverQuest_guess
{
public:
    void StartCasting(int param_1);
    void Sit_guess(void);
};

// FUNCTION: TRILOGY 0x004c86f8
// Shows a spawn beginning to cast: animation, filtered chat line, spell effects; spell -1 = fishing hook splash.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: spell-id error block is laid out after the success path (original: inline), and the allow-list loops keep entry in ECX/EDX (original: EAX)
void CEverQuest_guess::StartCasting(int castPayload)
{
    float hookX;
    short spellId;
    EQ_Spell_guess* spell;
    char envType;
    EQPlayer* pEVar6;
    int value;
    int animDuration;
    int* entry;
    float temp;
    char chatText[128];
    float pos[3];
    float fDist;
    float hookZ;
    float hookY;

    pEVar6 = EQ_METHOD(1, EQPlayer*, 0x004B1416, (void*, int), EQ_GLOBAL(void*, 0x006E4D58))(*(int*)castPayload);
    if (pEVar6 == 0)
    {
        EQTRACE_guess
            ("caster is NULL (in Startcasting)!\n");
        return;
    }
    spellId = *(short*)((char*)castPayload + 4);
    if (spellId == -1)
    {
        typedef float (__cdecl *FloatFn_guess)(float);

        fDist = *(u32*)((char*)castPayload + 8);
        temp = EQ_GLOBAL(FloatFn_guess, 0x006E524C)(pEVar6->heading);
        hookY = temp * fDist + pEVar6->y;
        temp = EQ_GLOBAL(FloatFn_guess, 0x006E5250)(pEVar6->heading);
        hookX = temp * fDist + pEVar6->x;
        temp = EQ_METHOD(3, float, 0x00414245, (void*, float, float, float), g_display_guess)(
             hookY, hookX, pEVar6->z);
        hookZ = temp + 3.0f;
        EQTRACE_guess
            ("hook xyz = %f, %f, %f\n", hookY,
             hookX, hookZ);
        envType = EQ_METHOD(4, char, 0x004147BB, (void*, float, float, float, int), g_display_guess)(
             hookY, hookX, hookZ, 0);
        if (envType == '\0')
            return;
        if (envType == '\x03')
            return;
        if (envType == '\a')
            return;
        temp = EQ_METHOD(3, float, 0x00414705, (void*, float, float, float), g_display_guess)(
             hookY, hookX, hookZ);
        hookZ = temp;
        if (temp == -1.0e27f)
            return;
        pos[0] = hookY;
        pos[1] = hookX;
        pos[2] = temp;
        EQ_CDECL(int, 0x004C7F71,
            (int, void*, EQPlayer*, int, int, float*, int, int))
            (2, (void*)0x0062E2D8, pEVar6, 0, 0, pos, 0, 500);
        EQ_METHOD(5, void, 0x0040E3EA, (void*, int, int, float, float, float), g_display_guess)(0xFF, 99,
             hookY, hookX, hookZ);
        return;
    }
    if (((spellId <= 0) || (spellId >= 3000)) ||
        (spell = (EQ_Spell_guess*)EQ_ARRAY(int, 0x005FE9A0)[spellId], spell == 0))
    {
        EQTRACE_guess
            ("the_spell is NULL (Startcasting) for spell nbr %d\n", (int)spellId);
        return;
    }
    value = EQ_METHOD(0, int, 0x004875F6, (void*), (void*)spell)();
    if (value != 0)
    {
        *(int*)((char*)pEVar6->actor_info + 0x1AC) =
            *(int*)((char*)g_display_guess + 0xBC) + 1000;
        *(int*)((char*)pEVar6->actor_info + 0x1B0) = 0x4E2;
        pEVar6->actor_info->unk_0x1B4 =
            *(short*)((char*)castPayload + 4);
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(pEVar6,
             spell->spellAnimation_guess, 1, 0, 1.0f, 1);
        return;
    }
    EQ_GLOBAL(int, 0x006D6FE0) = 1;
    animDuration = EQ_METHOD(2, int, 0x0048EB3A, (void*, EQPlayer*, int), g_display_guess)(pEVar6,
         spell->spellAnimation_guess);
    EQ_GLOBAL(int, 0x006D6FE0) = 0;
    value = *(int*)((char*)castPayload + 8) + animDuration;
    if (pEVar6->class_ != '\b')
    {
        if (pEVar6 == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            sprintf(chatText, "You begin casting %s.", (char*)spell);
        }
        else
        {
            sprintf(chatText, "%s begins to cast a spell.",
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), (void*)this)(pEVar6->name));
        }
    }
    else
    {
        if (pEVar6 == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            strcpy(chatText, (char*)spell + 0x60);
        }
        else if (spell->castOnOtherMessage_guess == '\0')
        {
            chatText[0] = 0;
        }
        else
        {
            sprintf(chatText, "%s%s",
                 EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), (void*)this)(pEVar6->name),
                 (char*)spell + 0xA0);
        }
    }
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != '\0')
    {
        sprintf(chatText, "%s (%s)", chatText, (char*)spell);
    }
    if (pEVar6->type != '\0')
    {
        if (EQ_GLOBAL(int, 0x006B6EE8) == 0)
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(chatText, 0x120);
    }
    else if (pEVar6->class_ == '\b')
    {
        if (pEVar6 == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            if (EQ_GLOBAL(int, 0x006B6EEC) != 3)
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(chatText, 0x108);
        }
        else
        {
            for (entry = EQ_ARRAY(int, 0x005FE954);
                 (int)entry < 0x005FE968 && pEVar6 != (EQPlayer*)*entry;
                 entry = entry + 1)
            {
            }
            if (EQ_GLOBAL(int, 0x006B6EEC) == 0 ||
                ((int)entry < 0x005FE968 && EQ_GLOBAL(int, 0x006B6EEC) == 2))
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(chatText, 0x120);
        }
    }
    else
    {
        if (pEVar6 == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            if (EQ_GLOBAL(int, 0x006B6EE4) != 3)
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(chatText, 0x108);
        }
        else
        {
            for (entry = EQ_ARRAY(int, 0x005FE954);
                 (int)entry < 0x005FE968 && pEVar6 != (EQPlayer*)*entry;
                 entry = entry + 1)
            {
            }
            if (EQ_GLOBAL(int, 0x006B6EE4) == 0 ||
                ((int)entry < 0x005FE968 && EQ_GLOBAL(int, 0x006B6EE4) == 2))
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(chatText, 0x120);
        }
    }
    EQ_CDECL(int, 0x004C85F4,
        (int, int, int, int, u32, u32, u32))
        (0, (int)spell, (int)pEVar6, (int)pEVar6, 0, 0, value);
}

// FUNCTION: TRILOGY 0x004C8ABA
// __cdecl, returns int. Consider line for `target` as seen from `me`: the faction text (faction is zeroed for
// players, type 0), the deity text when unworthy == 4, and the level text for the level difference. Prints
// "<name> -- <faction text> <level text>" through CEverQuest::dsp_chat in the colour the level text picked.
// Returns 0 without printing when there is no target.
int ConsiderMessage_guess(CharacterProfile* me, EQPlayer* target, MsgConsider* info)
{
    char chatLine[512];
    char factionText[256];
    char levelText[256];
    char genderText[16];
    int levelDiff;
    int tmp;
    int value;
    int color;
    short shortColor;
    char* phrase;

    levelDiff = (u8)target->level - (u8)me->level;
    value = info->faction;
    if (target == 0)
        return 0;
    if (target->gender == 0)
        phrase = EQ_ARRAY(char, 0x0056B7A8);
    else if (target->gender == 1)
        phrase = EQ_ARRAY(char, 0x0056B7AC);
    else
        phrase = EQ_ARRAY(char, 0x00568220);
    strcpy(genderText, phrase);
    if (target->type == 0)
        value = 0;
    if (value >= 0x44c)
        phrase = EQ_ARRAY(char, 0x0056B790);
    else if (value >= 0x2ee)
        phrase = EQ_ARRAY(char, 0x0056B778);
    else if (value >= 0x1f4)
        phrase = EQ_ARRAY(char, 0x0056B760);
    else if (value >= 0x64)
        phrase = EQ_ARRAY(char, 0x0056B74C);
    else if (value >= 0)
        phrase = EQ_ARRAY(char, 0x0056B730);
    else if (value >= -0x64)
        phrase = EQ_ARRAY(char, 0x0056B6B8);
    else if (value >= -0x1f4)
        phrase = EQ_ARRAY(char, 0x0056B6D8);
    else if (value >= -0x2ee)
        phrase = EQ_ARRAY(char, 0x0056B6F4);
    else
        phrase = EQ_ARRAY(char, 0x0056B710);
    strcpy(factionText, phrase);
    if (info->unworthy == 4)
    {
        if (EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))(target, EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 0) == 0)
        {
            phrase = EQ_ARRAY(char, 0x0056B790);
            sprintf(factionText, phrase);
        }
        else
        {
            value = EQ_CDECL(int, 0x004B2198, (int))((int)(short)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->deity);
            tmp = EQ_CDECL(int, 0x004B2198, (int))((int)(short)target->deity);
            if (value == 2)
            {
                if (tmp == 1)
                    phrase = EQ_ARRAY(char, 0x0056B694);
                else
                    phrase = EQ_ARRAY(char, 0x0056B670);
                strcpy(factionText, phrase);
            }
            else if (((value == 1) || (value == 3)) && (tmp == 2))
                strcpy(factionText, EQ_ARRAY(char, 0x0056B6B8));
            else
                sprintf(factionText, EQ_ARRAY(char, 0x0056B6F4));
        }
    }
    value = info->unworthy;
    if (value == 1)
    {
        if (levelDiff < -4)
            tmp = 2;
        else if (levelDiff <= 0)
            tmp = 0;
        else
            tmp = (levelDiff >= 5) ? 13 : 0;
    }
    else if (value == 2)
    {
        if (levelDiff < -8)
            tmp = 2;
        else if (levelDiff <= 0)
            tmp = 0;
        else
            tmp = (levelDiff >= 9) ? 13 : 0;
    }
    else if (value == 4)
    {
        if (levelDiff < -5)
            tmp = 2;
        else if (levelDiff <= 0)
            tmp = 0;
        else
            tmp = (levelDiff >= 6) ? 13 : 0;
    }
    else
    {
        tmp = 4;
        if (((u8)me->level * 3) / 4 <= (u8)target->level || levelDiff >= -3)
        {
            if (levelDiff == 0)
                tmp = 0;
            else if (levelDiff > 0)
                tmp = (levelDiff > 2) ? 13 : 15;
        }
        else
            tmp = 2;
    }
    color = tmp;
    if (value != 0)
    {
        if ((u8)target->level < 6)
            color = 2;
        else if ((u8)me->level < 6)
            color = 13;
    }
    shortColor = (short)color;
    if (value == 0)
    {
        if ((u8)me->level < 15)
        {
            if (shortColor == 0)
            {
                if ((u8)me->level < 8)
                    phrase = EQ_ARRAY(char, 0x0056B580);
                else
                    phrase = EQ_ARRAY(char, 0x0056B55C);
                strcpy(levelText, phrase);
            }
            else if (shortColor == 2)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B4C0));
            else if (shortColor == 4)
            {
                if (levelDiff < -3 || (u8)me->level < 7)
                    phrase = EQ_ARRAY(char, 0x0056B59C);
                else
                    phrase = EQ_ARRAY(char, 0x0056B5C8);
                strcpy(levelText, phrase);
            }
            else if (shortColor == 13)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B644));
            else if (shortColor == 15)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B540));
        }
        else if ((u8)me->level < 25)
        {
            if (shortColor == 0)
                sprintf(levelText, EQ_ARRAY(char, 0x0056B430), genderText);
            else if (shortColor == 2)
            {
                if (levelDiff < -5)
                    phrase = EQ_ARRAY(char, 0x0056B4C0);
                else
                    phrase = EQ_ARRAY(char, 0x0056B484);
                strcpy(levelText, phrase);
            }
            else if (shortColor == 4)
            {
                if (levelDiff > -3)
                    phrase = EQ_ARRAY(char, 0x0056B518);
                else if (levelDiff > -5)
                    phrase = EQ_ARRAY(char, 0x0056B4E8);
                else
                    phrase = EQ_ARRAY(char, 0x0056B484);
                strcpy(levelText, phrase);
            }
            else if (shortColor == 13)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B644));
            else if (shortColor == 15)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B540));
        }
        else
        {
            if (shortColor == 0)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B540));
            else if (shortColor == 2)
            {
                if (levelDiff < -10)
                    phrase = EQ_ARRAY(char, 0x0056B40C);
                else
                    phrase = EQ_ARRAY(char, 0x0056B3C8);
                strcpy(levelText, phrase);
            }
            else if (shortColor == 4)
                sprintf(levelText, EQ_ARRAY(char, 0x0056B430), genderText);
            else if (shortColor == 13)
                strcpy(levelText, EQ_ARRAY(char, 0x0056B644));
            else if (shortColor == 15)
                sprintf(levelText, EQ_ARRAY(char, 0x0056B454), genderText);
        }
    }
    else
    {
        if (shortColor == 0)
            sprintf(levelText, EQ_ARRAY(char, 0x0056B5F0), genderText);
        else if (shortColor == 2)
            sprintf(levelText, EQ_ARRAY(char, 0x0056B61C), genderText);
        else if (shortColor == 13)
            strcpy(levelText, EQ_ARRAY(char, 0x0056B644));
    }
    sprintf(chatLine, EQ_ARRAY(char, 0x0056B534),
         EQ_METHOD(1, char*, 0x004D0436, (void*, char*), (void*)EQ_GLOBAL(void*, 0x006EFE2C))((char*)target + 1),
         factionText, levelText);
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)EQ_GLOBAL(void*, 0x006EFE2C))(chatLine, color);
    return 1;
}

class CEverQuest_Consider_guess_class : public CEverQuest
{
public:
    void Consider_guess(EQPlayer* target, MsgConsider* info);
};

// FUNCTION: TRILOGY 0x004C8EBE
// Handles /consider replies for missing, dead, player, and NPC targets.
// __thiscall (`ret 8`), two stack arguments: the /consider target (null -> "Consider who?") and the OP_Consider
// message. Type 0 and 1 spawns (player, NPC) go to ConsiderMessage_guess; any other type (corpses) gets
// "It is futile to consider the dead..." through CEverQuest::dsp_chat. `this` is passed on in ECX.
void CEverQuest_Consider_guess_class::Consider_guess(EQPlayer* target, MsgConsider* info)
{
    if (target == 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(EQ_ARRAY(char, 0x0056B7B0), 0x111);
    }
    else if (target->type >= 2)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), (void*)this)(EQ_ARRAY(char, 0x005637C4), 0x111);
    }
    else
    {
        EQ_CDECL(int, 0x004C8ABA, (CharacterProfile*, EQPlayer*, MsgConsider*))
            (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), target, info);
    }
}

// FUNCTION: TRILOGY 0x004c8f02
// Maps an item type to its instrument category label.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall (callers load ECX from pinstCEverQuest), `this` unused. Item types 23..26 -> instrument name,
// anything else "".
char* CEverQuest::GetInstrumentDesc_guess(short itemType)
{
    switch (itemType)
    {
    case 23: return "Wind Instrument";
    case 24: return "Stringed Instrument";
    case 25: return "Brass Instrument";
    case 26: return "Percussion Instrument";
    }
    return "";
}

// FUNCTION: TRILOGY 0x004c8f39
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::GetBodyTypeDesc, medium)
// __thiscall (`ret 4`), `this` unused. The id is a short (movsx).
char* CEverQuest::GetBodyTypeDesc(short bodyType)
{
    switch (bodyType)
    {
    case 1:  return "Humanoid";
    case 2:  return "Lycanthrope";
    case 3:  return "Undead";
    case 4:  return "Giant";
    case 5:  return "Construct";
    case 6:  return "Extraplanar";
    case 7:  return "Magical";
    case 21: return "Animal";
    case 22: return "Insect";
    case 23: return "Monster";
    case 24: return "Elemental";
    case 25: return "Plant";
    case 26: return "Dragon";
    }
    return "UNKNOWN BODYTYPE";
}

// FUNCTION: TRILOGY 0x004c8fce
// Maps a language ID to its in-game language name.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::GetLangDesc, BinDiff low)
// NAME: GetLanguageName (eqclassic, certain)
// __thiscall (callers load ECX from pinstCEverQuest 0x006EFE2C; `ret 4`). `this` is unused.
// The id is a short: the original sign-extends the low word (movsx), so 0xFFFF is -1, not 65535.
// Ids 0..23 go through a jump table; everything else is "an unknown tongue". The cases are listed in the
// order of the original's case blocks (VC6 emits them in source order), not in id order.
char* CEverQuest::GetLanguageName(short languageId)
{
    switch (languageId)
    {
    case 0:  return "Common Tongue";
    case 1:  return "Barbarian";
    case 2:  return "Erudian";
    case 3:  return "Elvish";
    case 4:  return "Dark Elvish";
    case 5:  return "Dwarvish";
    case 6:  return "Troll";
    case 7:  return "Ogre";
    case 8:  return "Gnomish";
    case 9:  return "Halfling";
    case 13: return "Froglok";
    case 14: return "Goblin";
    case 15: return "Gnoll";
    case 16: return "Combine Tongue";
    case 17: return "Elder Teir'Dal";
    case 18: return "Lizardman";
    case 19: return "Orcish";
    case 20: return "Faerie";
    case 10: return "Thieves Cant";
    case 21: return "Dragon";
    case 11: return "Old Erudian";
    case 12: return "Elder Elvish";
    case 22: return "Elder Dragon";
    case 23: return "Dark Speech";
    }
    return "an unknown tongue";
}

// FUNCTION: TRILOGY 0x004c9105
// Race id -> race name for player and creature races 1..213; any other id gives "UNKNOWN RACE".
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (TAKP: CEverQuest_GetRaceDesc2, low)
// NAME: GetRaceName (eqclassic: races.h "found in sub_4C9105")
// __thiscall (CEverQuest::Who loads ECX with its `this`; `ret 4`), `this` unused. The id is a short
// (movsx); 1..213 go through a jump table, 142, 143, 179 and 180 have no case. Cases are in the order of
// the original's case blocks: 67 (Highpass Citizen) sits where 14 would be and 14 (Were Wolf) where 67
// would be.
char* CEverQuest::GetRaceName(short race)
{
    char* name;

    switch (race)
    {
    case 1:
        name = "Human";
        break;
    case 2:
        name = "Barbarian";
        break;
    case 3:
        name = "Erudite";
        break;
    case 4:
        name = "Wood Elf";
        break;
    case 5:
        name = "High Elf";
        break;
    case 6:
        name = "Dark Elf";
        break;
    case 7:
        name = "Half Elf";
        break;
    case 8:
        name = "Dwarf";
        break;
    case 9:
        name = "Troll";
        break;
    case 10:
        name = "Ogre";
        break;
    case 11:
        name = "Halfling";
        break;
    case 12:
        name = "Gnome";
        break;
    case 13:
        name = "Aviak";
        break;
    case 67:
        name = "Highpass Citizen";
        break;
    case 15:
        name = "Brownie";
        break;
    case 16:
        name = "Centaur";
        break;
    case 17:
        name = "Golem";
        break;
    case 18:
        name = "Giant / Cyclops";
        break;
    case 19:
        name = "Trakenon";
        break;
    case 20:
        name = "Venril Sathor";
        break;
    case 21:
        name = "Evil Eye";
        break;
    case 22:
        name = "Beetle";
        break;
    case 23:
        name = "Kerra";
        break;
    case 24:
        name = "Fish";
        break;
    case 25:
        name = "Fairy";
        break;
    case 26:
        name = "Froglok";
        break;
    case 27:
        name = "Froglok Ghoul";
        break;
    case 28:
        name = "Fungusman";
        break;
    case 29:
        name = "Gargoyle";
        break;
    case 30:
        name = "Gasbag";
        break;
    case 31:
        name = "Gelatinous Cube";
        break;
    case 32:
        name = "Ghost";
        break;
    case 33:
        name = "Ghoul";
        break;
    case 34:
        name = "Giant Bat";
        break;
    case 35:
        name = "Giant Eel";
        break;
    case 36:
        name = "Giant Rat";
        break;
    case 37:
        name = "Giant Snake";
        break;
    case 38:
        name = "Giant Spider";
        break;
    case 39:
        name = "Gnoll";
        break;
    case 40:
        name = "Goblin";
        break;
    case 41:
        name = "Gorilla";
        break;
    case 42:
        name = "Wolf";
        break;
    case 43:
        name = "Bear";
        break;
    case 44:
        name = "Freeport Guards";
        break;
    case 45:
        name = "Demi Lich";
        break;
    case 46:
        name = "Imp";
        break;
    case 47:
        name = "Griffin";
        break;
    case 48:
        name = "Kobold";
        break;
    case 49:
        name = "Lava Dragon";
        break;
    case 50:
        name = "Lion";
        break;
    case 51:
        name = "Lizard Man";
        break;
    case 52:
        name = "Mimic";
        break;
    case 53:
        name = "Minotaur";
        break;
    case 54:
        name = "Orc";
        break;
    case 55:
        name = "Human Beggar";
        break;
    case 56:
        name = "Pixie";
        break;
    case 57:
        name = "Dracnid";
        break;
    case 58:
        name = "Solusek Ro";
        break;
    case 59:
        name = "Bloodgills";
        break;
    case 60:
        name = "Skeleton";
        break;
    case 61:
        name = "Shark";
        break;
    case 62:
        name = "Tunare";
        break;
    case 63:
        name = "Tiger";
        break;
    case 64:
        name = "Treant";
        break;
    case 65:
        name = "Vampire";
        break;
    case 66:
        name = "Rallos Zek";
        break;
    case 14:
        name = "Were Wolf";
        break;
    case 68:
        name = "Tentacle";
        break;
    case 69:
        name = "Will 'O Wisp";
        break;
    case 70:
        name = "Zombie";
        break;
    case 71:
        name = "Qeynos Citizen";
        break;
    case 72:
        name = "Ship";
        break;
    case 73:
        name = "Launch";
        break;
    case 74:
        name = "Piranha";
        break;
    case 75:
        name = "Elemental";
        break;
    case 141:
        name = "Boat";
        break;
    case 76:
        name = "Puma";
        break;
    case 77:
        name = "Neriak Citizen";
        break;
    case 78:
        name = "Erudite Citizen";
        break;
    case 79:
        name = "Bixie";
        break;
    case 80:
        name = "Reanimated Hand";
        break;
    case 81:
        name = "Rivervale Citizen";
        break;
    case 82:
        name = "Scarecrow";
        break;
    case 83:
        name = "Skunk";
        break;
    case 84:
        name = "Snake Elemental";
        break;
    case 85:
        name = "Spectre";
        break;
    case 86:
        name = "Sphinx";
        break;
    case 87:
        name = "Armadillo";
        break;
    case 88:
        name = "Clockwork Gnome";
        break;
    case 89:
        name = "Drake";
        break;
    case 90:
        name = "Halas Citizen";
        break;
    case 91:
        name = "Alligator";
        break;
    case 92:
        name = "Grobb Citizen";
        break;
    case 93:
        name = "Oggok Citizen";
        break;
    case 94:
        name = "Kaladim Citizen";
        break;
    case 95:
        name = "Cazic Thule";
        break;
    case 96:
        name = "Cockatrice";
        break;
    case 97:
        name = "Daisy Man";
        break;
    case 98:
        name = "Elf Vampire";
        break;
    case 99:
        name = "Denizen";
        break;
    case 100:
        name = "Dervish";
        break;
    case 101:
        name = "Efreeti";
        break;
    case 102:
        name = "Froglok Tadpole";
        break;
    case 103:
        name = "Kedge";
        break;
    case 104:
        name = "Leech";
        break;
    case 105:
        name = "Swordfish";
        break;
    case 106:
        name = "Felguard";
        break;
    case 107:
        name = "Mammoth";
        break;
    case 108:
        name = "Eye of Zomm";
        break;
    case 109:
        name = "Wasp";
        break;
    case 110:
        name = "Mermaid";
        break;
    case 111:
        name = "Harpie";
        break;
    case 112:
        name = "Fayguard";
        break;
    case 113:
        name = "Drixie";
        break;
    case 114:
        name = "Ghost Ship";
        break;
    case 115:
        name = "Clam";
        break;
    case 116:
        name = "Sea Horse";
        break;
    case 117:
        name = "Ghost Dwarf";
        break;
    case 118:
        name = "Erudite Ghost";
        break;
    case 119:
        name = "Sabertooth Cat";
        break;
    case 120:
        name = "Wolf Elemental";
        break;
    case 121:
        name = "Gorgon";
        break;
    case 122:
        name = "Dragon Skeleton";
        break;
    case 123:
        name = "Innoruuk";
        break;
    case 124:
        name = "Unicorn";
        break;
    case 125:
        name = "Pegasus";
        break;
    case 126:
        name = "Djinn";
        break;
    case 127:
        name = "Invisible Man";
        break;
    case 128:
        name = "Iksar";
        break;
    case 129:
        name = "Scorpion";
        break;
    case 131:
        name = "Sarnak";
        break;
    case 132:
        name = "Draglock";
        break;
    case 133:
        name = "Lycanthrope";
        break;
    case 134:
        name = "Mosquito";
        break;
    case 135:
        name = "Rhino";
        break;
    case 136:
        name = "Xalgoz";
        break;
    case 137:
        name = "Kunark Goblin";
        break;
    case 138:
        name = "Yeti";
        break;
    case 139:
        name = "Iksar Citizen";
        break;
    case 140:
        name = "Forest Giant";
        break;
    case 144:
        name = "Burynai";
        break;
    case 145:
        name = "Goo";
        break;
    case 146:
        name = "Spectral Sarnak";
        break;
    case 147:
        name = "Spectral Iksar";
        break;
    case 148:
        name = "Kunark Fish";
        break;
    case 149:
        name = "Iksar Scorpion";
        break;
    case 150:
        name = "Erollisi";
        break;
    case 151:
        name = "Tribunal";
        break;
    case 152:
        name = "Bertoxxulous";
        break;
    case 153:
        name = "Bristlebane";
        break;
    case 154:
        name = "Fay Drake";
        break;
    case 155:
        name = "Sarnak Skeleton";
        break;
    case 156:
        name = "Ratman";
        break;
    case 157:
        name = "Wyvern";
        break;
    case 158:
        name = "Wurm";
        break;
    case 159:
        name = "Devourer";
        break;
    case 160:
        name = "Iksar Golem";
        break;
    case 161:
        name = "Iksar Skeleton";
        break;
    case 162:
        name = "Man Eating Plant";
        break;
    case 163:
        name = "Raptor";
        break;
    case 164:
        name = "Sarnak Golem";
        break;
    case 165:
        name = "Water Dragon";
        break;
    case 166:
        name = "Iksar Hand";
        break;
    case 167:
        name = "Succulent";
        break;
    case 168:
        name = "Flying Monkey";
        break;
    case 169:
        name = "Brontotherium";
        break;
    case 170:
        name = "Snow Dervish";
        break;
    case 171:
        name = "Dire Wolf";
        break;
    case 172:
        name = "Manticore";
        break;
    case 173:
        name = "Totem";
        break;
    case 174:
        name = "Cold Spectre";
        break;
    case 175:
        name = "Enchanted Armor";
        break;
    case 176:
        name = "Snow Bunny";
        break;
    case 177:
        name = "Walrus";
        break;
    case 178:
        name = "Rock-gem Men";
        break;
    case 181:
        name = "Yak Man";
        break;
    case 182:
        name = "Faun";
        break;
    case 183:
        name = "Coldain";
        break;
    case 184:
        name = "Velious Dragons";
        break;
    case 185:
        name = "Hag";
        break;
    case 186:
        name = "Hippogriff";
        break;
    case 187:
        name = "Siren";
        break;
    case 188:
        name = "Frost Giant";
        break;
    case 189:
        name = "Storm Giant";
        break;
    case 190:
        name = "Ottermen";
        break;
    case 191:
        name = "Walrus Man";
        break;
    case 192:
        name = "Clockwork Dragon";
        break;
    case 193:
        name = "Abhorent";
        break;
    case 194:
        name = "Sea Turtle";
        break;
    case 195:
        name = "Black and White Dragons";
        break;
    case 196:
        name = "Ghost Dragon";
        break;
    case 197:
        name = "Ronnie Test";
        break;
    case 198:
        name = "Prismatic Dragon";
        break;
    case 199:
        name = "ShikNar";
        break;
    case 200:
        name = "Rockhopper";
        break;
    case 201:
        name = "Underbulk";
        break;
    case 202:
        name = "Grimling";
        break;
    case 130:
        name = "Vah Shir";
        break;
    case 203:
        name = "Vacuum Worm";
        break;
    case 204:
        name = "Evan Test";
        break;
    case 205:
        name = "Kahli Shah";
        break;
    case 206:
        name = "Owlbear";
        break;
    case 207:
        name = "Rhino Beetle";
        break;
    case 208:
        name = "Vampyre";
        break;
    case 209:
        name = "Earth Elemental";
        break;
    case 210:
        name = "Air Elemental";
        break;
    case 211:
        name = "Water Elemental";
        break;
    case 212:
        name = "Fire Elemental";
        break;
    case 213:
        name = "Wetfang Minnow";
        break;
    default:
        name = "UNKNOWN RACE";
        break;
    }
    return name;
}

// FUNCTION: TRILOGY 0x004c9c6d
// Maps a deity ID to its in-game name.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall, `this` unused. Deity id (201..216, 396) -> name; anything else is "Agnostic" too. Cases in
// the order of the original's case blocks.
char* CEverQuest::GetDeityDesc_guess(short deity)
{
    char* name;

    switch (deity)
    {
    case 201: name = "Bertoxxulous"; break;
    case 205: name = "Bristlebane"; break;
    case 203: name = "Cazic-Thule"; break;
    case 206: name = "Innoruuk"; break;
    case 207: name = "Karana"; break;
    case 204: name = "Erollisi Marr"; break;
    case 208: name = "Mithaniel Marr"; break;
    case 212: name = "Rodcet Nife"; break;
    case 209: name = "Prexus"; break;
    case 210: name = "Quellious"; break;
    case 213: name = "Solusek Ro"; break;
    case 202: name = "Brell Serilis"; break;
    case 214: name = "The Tribunal"; break;
    case 215: name = "Tunare"; break;
    case 216: name = "Veeshan"; break;
    case 211: name = "Rallos Zek"; break;
    case 396: name = "Agnostic"; break;
    default:  name = "Agnostic"; break;
    }
    return name;
}

// FUNCTION: TRILOGY 0x004c9d4e
// Maps a class ID to its class, guildmaster, banker, or merchant label.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall, `this` unused. Class id 1..32 -> name (17..31 are the guildmasters, 32 merchant), else
// "UNKNOWN CLASS". Cases in the order of the original's case blocks.
char* CEverQuest::GetClassDesc_guess(short classId)
{
    char* name;

    switch (classId)
    {
    case 1:  name = "Warrior"; break;
    case 2:  name = "Cleric"; break;
    case 3:  name = "Paladin"; break;
    case 4:  name = "Ranger"; break;
    case 5:  name = "Shadow Knight"; break;
    case 6:  name = "Druid"; break;
    case 7:  name = "Monk"; break;
    case 8:  name = "Bard"; break;
    case 9:  name = "Rogue"; break;
    case 10: name = "Shaman"; break;
    case 11: name = "Necromancer"; break;
    case 12: name = "Wizard"; break;
    case 13: name = "Magician"; break;
    case 14: name = "Enchanter"; break;
    case 15: name = "Beastlord"; break;
    case 32: name = "Merchant"; break;
    case 16: name = "Banker"; break;
    case 17: name = "Warrior Guildmaster"; break;
    case 18: name = "Cleric Guildmaster"; break;
    case 19: name = "Paladin Guildmaster"; break;
    case 21: name = "Shadowknight Guildmaster"; break;
    case 20: name = "Ranger Guildmaster"; break;
    case 22: name = "Druid Guildmaster"; break;
    case 23: name = "Monk Guildmaster"; break;
    case 24: name = "Bard Guildmaster"; break;
    case 25: name = "Rogue Guildmaster"; break;
    case 26: name = "Shaman Guildmaster"; break;
    case 27: name = "Necromancer Guildmaster"; break;
    case 28: name = "Wizard Guildmaster"; break;
    case 29: name = "Magician Guildmaster"; break;
    case 30: name = "Enchanter Guildmaster"; break;
    case 31: name = "Beastlord Guildmaster"; break;
    default: name = "UNKNOWN CLASS"; break;
    }
    return name;
}

struct GuildInviteMessage_guess
{
    char invitee[30];           /* 0x00  the player being invited */
    char inviter[30];           /* 0x1E  the character's own name */
    short guild_id;             /* 0x3C  only the low 16 bits are written */
    int officer;                /* 0x40  0 = member invite, 1 = officer invite */
};
C_ASSERT(sizeof(GuildInviteMessage_guess) == 0x44);

class CEverQuest_GuildInvite_guess
{
public:
    void GuildInvite(char* param_1, char* param_2);
};

// FUNCTION: TRILOGY 0x004c9ef6
// Invites a player to the guild, optionally granting officer rank.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_GuildInvite_guess::GuildInvite(char* inviteeName, char* officerOption)
{
    char inviteText[128];
    GuildInviteMessage_guess inviteMessage;
    char firstCharacter;
    EQPlayer* player;

    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id == -1)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C584), 0x111);
        return;
    }
    if ((u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank < 1)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C524), 0x111);
        return;
    }
    firstCharacter = *inviteeName;
    if (firstCharacter == '\0' || firstCharacter == 'o' || firstCharacter == 'l')
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == 0 ||
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type != 0 ||
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C374), 0x111);
            return;
        }
        if (EQ_GLOBAL(int, 0x00596330) == 4 &&
            EQ_CDECL(int, 0x004B21FC, (EQPlayer*, EQPlayer*, void*))
                (EQ_GLOBAL(EQPlayer*, 0x006E4D8C), EQ_GLOBAL(EQPlayer*, 0x006E4DAC), 0) != 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(EQ_ARRAY(char, 0x0056C3E8));
            return;
        }
        strcpy(inviteMessage.inviter, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        strcpy(inviteMessage.invitee, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
        inviteMessage.guild_id = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
        inviteMessage.officer = 0;
        sprintf(inviteText, EQ_ARRAY(char, 0x0056C458), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
        if (*inviteeName == 'o')
        {
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank == 2)
            {
                inviteMessage.officer = 1;
                sprintf(inviteText, EQ_ARRAY(char, 0x0056C420), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C3B0), 0x111);
                return;
            }
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(inviteText, 0x111);
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2117, &inviteMessage, 0x44, 1);
        return;
    }
    else
    {
        if (_strcmpi((char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), inviteeName) == 0)
        {
            return;
        }
        strcpy(inviteMessage.inviter, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        strcpy(inviteMessage.invitee, inviteeName);
        inviteMessage.guild_id = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
        inviteMessage.officer = 0;
        player = EQ_METHOD(1, EQPlayer*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(inviteMessage.invitee);
        if (player == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C4D0), 0x111);
            return;
        }
        if (player->type != 0 && player->gm == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C490), 0x111);
            return;
        }
        sprintf(inviteText, EQ_ARRAY(char, 0x0056C458), inviteeName);
        if (*officerOption == 'o')
        {
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank == 2)
            {
                inviteMessage.officer = 1;
                sprintf(inviteText, EQ_ARRAY(char, 0x0056C420), inviteeName);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C3B0), 0x111);
                return;
            }
        }
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(inviteText, 0x111);
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2117, &inviteMessage, 0x44, 1);
        return;
    }
}

struct GuildRemoveMessage_guess
{
    char removee[30];           /* 0x00  the player being removed */
    char remover[30];           /* 0x1E  the character's own name */
    short guild_id;             /* 0x3C  only the low 16 bits are written */
    u32 rank;                   /* 0x40  the character's guild rank, zero-extended from a byte */
};
C_ASSERT(sizeof(GuildRemoveMessage_guess) == 0x44);

class CEverQuest_GuildRemove_guess
{
public:
    void GuildRemove_guess(char* param_1);
};

// FUNCTION: TRILOGY 0x004ca148
// Requests removal of a named or targeted guild member, or yourself.
void CEverQuest_GuildRemove_guess::GuildRemove_guess(char* removeeName)
{
    char statusText[128];
    GuildRemoveMessage_guess removeMessage;

    if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id == -1)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C584), 0x111);
        return;
    }
    if ((*removeeName == '\0' && EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)) ||
        strcmp(removeeName, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)) == 0)
    {
        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank == 2)
        {
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C698), 0x111);
            return;
        }
        strcpy(removeMessage.remover, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        strcpy(removeMessage.removee, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        removeMessage.guild_id = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
        removeMessage.rank = (u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2119, &removeMessage, 0x44, 1);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C66C), 0x111);
        return;
    }
    if ((u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank < 1)
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C610), 0x111);
        return;
    }
    if (*removeeName == '\0')
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0 &&
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 0)
        {
            strcpy(removeMessage.remover, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
            strcpy(removeMessage.removee, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            removeMessage.guild_id = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
            removeMessage.rank = (u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank;
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2119, &removeMessage, 0x44, 1);
            sprintf(statusText, EQ_ARRAY(char, 0x0056C5A8), EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(statusText, 0x111);
        }
        else
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(EQ_ARRAY(char, 0x0056C5D4), 0x111);
    }
    else
    {
        strcpy(removeMessage.remover, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        strcpy(removeMessage.removee, removeeName);
        removeMessage.guild_id = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
        removeMessage.rank = (u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2119, &removeMessage, 0x44, 1);
        sprintf(statusText, EQ_ARRAY(char, 0x0056C5A8), removeeName);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(statusText, 0x111);
    }
}

struct GuildWarMessage_guess
{
    char requester[30];
    char profile_name[30];
    int guild_id;
    int days;
    int reliable;
};

class CEverQuest_GuildWar_guess
{
public:
    void GuildWar(char* param_1, int param_2);
};

// FUNCTION: TRILOGY 0x004cb10b
// Petitions another guild to go to war for a chosen number of days.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_GuildWar_guess::GuildWar(char* petitionedGuildName, int days)
{
    char petitionText[256];
    GuildWarMessage_guess warMessage;

    strcpy(warMessage.requester, petitionedGuildName);
    strcpy(warMessage.profile_name, (char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
    warMessage.guild_id = (int)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id;
    warMessage.reliable = 1;
    warMessage.days = days;
    send_message_guess
        (&EQ_GLOBAL(void*, 0x006B3194), 0x216F, &warMessage, 0x48, 1);
    if ((days != 0) &&
        (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_rank == '\x02'))
    {
        sprintf(petitionText, EQ_ARRAY(char, 0x0056CD3C), petitionedGuildName, days);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(petitionText);
    }
}

// FUNCTION: TRILOGY 0x004ca381
// Shows a player's guild membership and rank in chat.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: EQ_THISCALL wrappers still clear dummy EDX, and the empty-name branch layout still differs.
// A named target is looked up through the player-list receiver. The implicit game object remains the receiver
// for name trimming and chat output; only player types 0 with a clear GM byte are valid.
void CEverQuest::GuildStatus(char* targetName)
{
    EQPlayer* target;
    char firstCharacter;
    short guildId;
    short rank;
    char message[256];

    firstCharacter = targetName[0];
    if (firstCharacter != 0 || (target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)) == 0)
    {
        if (firstCharacter == 0)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)((char*)0x0056c770, 0x111);
            return;
        }
        target = EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(targetName);
    }

    if (target == 0 || target->type != 0 || target->gm != 0)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)((char*)0x0056c770, 0x111);
        return;
    }

    guildId = target->guild_id;
    if (guildId == -1)
    {
        sprintf(message, (char*)0x0056c814,
            EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), this)(target->name));
    }
    else
    {
        rank = (short)target->guild_rank;
        switch (rank)
        {
        case 2:
            sprintf(message, (char*)0x0056c7fc,
                EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), this)(target->name),
                EQ_CDECL(char*, 0x004E00F6, (short))(guildId));
            break;
        case 1:
            sprintf(message, (char*)0x0056c7e4,
                EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), this)(target->name),
                EQ_CDECL(char*, 0x004E00F6, (short))(guildId));
            break;
        case 0:
            sprintf(message, (char*)0x0056c7cc,
                EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), this)(target->name),
                EQ_CDECL(char*, 0x004E00F6, (short))(guildId));
            break;
        default:
            sprintf(message, (char*)0x0056c814,
                EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), this)(target->name));
            break;
        }
    }

    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)(message, 0x111);
}

class CEverQuest_GuildLeader_guess_class : public CEverQuest
{
public:
    void GuildLeader(char* param_1);
};

// FUNCTION: TRILOGY 0x004ca495
// Requests a transfer of guild leadership to another player.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall (`ret 4`), one char* argument. Not in a guild: chat line. Guild rank 2 (leader): with an empty
// argument the current target's name is used, then the transfer request (opcode 0x2195) goes to the world.
// Any other rank: chat line. The rank-2 chat line is sent with g_everquest as the receiver, not `this`.
void CEverQuest_GuildLeader_guess_class::GuildLeader(char* targetName)
{
    char chatLine[256];
    char transferName[32];

    if (g_pc_guess->guild_id == -1)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)((const char*)0x0056c584, 0x111);
        return;
    }
    if (g_pc_guess->guild_rank != 2)
    {
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)((const char*)0x0056c860, 0x111);
        return;
    }
    if (targetName[0] == 0 && EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0)
    {
        strcpy(targetName, EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name);
    }
    sprintf(chatLine, (char*)0x0056c82c, targetName);
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(chatLine, 0x111);
    strcpy(transferName, targetName);
    send_message_guess(g_world_connection_guess, 0x2195, transferName, 0x20, 1);
}

class CEverQuest_GuildSay_guess_class : public CEverQuest
{
public:
    void GuildSay_guess(char* param_1);
};

// FUNCTION: TRILOGY 0x004ca544
// Sends a message to guild chat and echoes it locally.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_GuildSay_guess_class::GuildSay_guess(char* chatText)
{
    char chatLine[512];

    if (EQ_GLOBAL(u8, 0x006B63F8) == 0 &&
        DAT_006b6494 == 0 &&
        EQ_GLOBAL(u8, 0x006B6465) == 0)
    {
        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->guild_id == -1)
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)((const char*)0x0056c584, 0x111);
        }
        else
        {
            sprintf(chatLine, (const char*)0x0056c8a0, chatText);
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), this)(chatLine, 0x111);
            EQ_METHOD(3, void, 0x004D0D77, (CEverQuest*, int, int, char*), this)(0, 0, chatText);
        }
    }
    return;
}

class CEverQuest_Who_guess_class : public CEverQuest
{
public:
    void Who_guess(char* text);
};

// FUNCTION: TRILOGY 0x004ca5c7
// /who: lists zone players by class/race/level/GM/guild/LFG/name, or your corpses; /who all asks the server.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall (`ret 4`), one char* argument: the /who text. Up to six words are taken off the front (the argument
// is advanced past them). corpse / all / gm / nongm / friend(s) / lfg / GUILD / G<n> / class / race / level / guild
// name set the filters; `all` with no friends filter goes to the world server and returns.
/* fake match: group buffers and use the word-slot shift to reproduce the original frame and stride. */
/* fake match: reserve the original 28-byte frame gap; class/race gotos leave the inner search loops. */
// @early-stop: flag/int local slot order, prologue scheduling, `this` caching in ebx, pop-after-call before sprintf still differ.
void CEverQuest_Who_guess_class::Who_guess(char* text)
{
    struct WhoBuffers_guess
    {
        char words[768];
        char friendList[2048];
        char guildDigits[128];
        char zoneName[128];
        char guildSuffix[128];
        char line[128];
        char linkDeadTag[16];
        char gmTag[16];
        char afkTag[16];
        char lfgTag[16];
        char nameFilter[32];
        struct WhoFilters_guess
        {
            short raceFilter;
            short classFilter;
            short levelMin;
            short levelMax;
            short gmFilter;
            short guildFilter;
        } filters;
    } buffers;
    volatile char frameSlot[28];
    int levelArg1;
    int levelArg2;
    int playerCount;
    int wordIndex;
    int whoType;
    char gotLevelArg1;
    char gotLevelArg2;
    char gotClass;
    char gotRace;
    int index;
    int cmp;
    size_t len;
    char* word;
    char* friendName;
    EQPlayer* player;
    short shortCount;
    short shortArg1;
    short shortArg2;
    bool matches;

    *buffers.nameFilter = '\0';
    gotLevelArg2 = '\0';
    gotLevelArg1 = '\0';
    gotClass = '\0';
    gotRace = '\0';
    whoType = 0;
    playerCount = 0;
    buffers.filters.gmFilter = -1;
    buffers.filters.guildFilter = -1;
    buffers.filters.levelMax = -1;
    buffers.filters.levelMin = -1;
    buffers.filters.classFilter = -1;
    buffers.filters.raceFilter = -1;
    levelArg2 = -1;
    levelArg1 = -1;
    frameSlot[27] = 0;
    for (wordIndex = 0; (short)wordIndex < 6; wordIndex = wordIndex + 1)
    {
        index = (short)wordIndex << 7;
        word = buffers.words + index;
        text = EQ_METHOD(2, char *, 0x004DBC84, (CEverQuest *, char *, char *), EQ_GLOBAL(CEverQuest *, 0x006EFE2C))(text, word);
        len = strlen(word);
        if (len < 1)
            break;
        cmp = _strcmpi(word, EQ_ARRAY(char, 0x0055A040));
        if (cmp == 0)
        {
            whoType = 2;
            continue;
        }
        cmp = _strcmpi(word, EQ_ARRAY(char, 0x00565AD8));
        if (cmp == 0)
        {
            whoType = 3;
            continue;
        }
        cmp = _strcmpi(word, EQ_ARRAY(char, 0x00568434));
        if (cmp == 0)
        {
            buffers.filters.gmFilter = 1;
            continue;
        }
        cmp = _strcmpi(word, EQ_ARRAY(char, 0x0056CB30));
        if (cmp == 0)
        {
            buffers.filters.gmFilter = 0;
            continue;
        }
        cmp = _strcmpi(word, EQ_ARRAY(char, 0x0056CB28));
        if (cmp != 0)
            cmp = _strcmpi(word, EQ_ARRAY(char, 0x0056CB20));
        if (cmp != 0)
        {
            cmp = _strcmpi(word, EQ_ARRAY(char, 0x0056CB1C));
            if (cmp == 0)
            {
                buffers.filters.guildFilter = -3;
                continue;
            }
            cmp = strcmp(word, EQ_ARRAY(char, 0x0056CB14));
            if (cmp == 0)
            {
                if (g_local_player_guess->guild_id == -1)
                {
                    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056CAD4), 0x111);
                }
                else
                {
                    buffers.filters.guildFilter = g_local_player_guess->guild_id;
                    strcpy(
                        buffers.nameFilter, EQ_CDECL(char *, 0x004E00F6, (short))(buffers.filters.guildFilter));
                    continue;
                }
            }
            if (*word == 'G' || *word == 'g')
            {
                cmp = EQ_CDECL(int, 0x00522F25, (int))((int)buffers.words[index + 1]);
                if (cmp != 0)
                {
                    strcpy(buffers.guildDigits, buffers.words + index + 1);
                    buffers.filters.guildFilter = (short)atoi(buffers.guildDigits);
                    EQTRACE_guess(EQ_ARRAY(char, 0x0056CAC0), (int)buffers.filters.guildFilter);
                    strcpy(
                        buffers.nameFilter, EQ_CDECL(char *, 0x004E00F6, (short))(buffers.filters.guildFilter));
                    continue;
                }
            }
            if (gotClass == 0)
            {
                index = 1;
                do
                {
                    if ((short)whoType == 1)
                    {
                        cmp = _strnicmp(
                            EQ_METHOD(1, char *, 0x004C9D4E, (CEverQuest *, int), this)(index), word,
                            strlen(word));
                    }
                    else
                    {
                        cmp = _strnicmp(
                            EQ_METHOD(1, char *, 0x0041BF2E, (struct CharacterProfile *, int), g_pc_guess)(index), word,
                            strlen(word));
                    }
                    if (cmp != 0)
                    {
                        index = index + 1;
                    }
                    else
                    {
                        gotClass = 1;
                        buffers.filters.classFilter = (short)index;
                        goto nextWord;
                    }
                } while ((short)index < 0x21);
            }
            if (gotRace == 0)
            {
                index = 0;
                do
                {
                    if ((short)whoType == 1)
                    {
                        cmp = _strnicmp(
                            EQ_METHOD(1, char *, 0x004C9105, (CEverQuest *, int), this)(index), word,
                            strlen(word));
                    }
                    else
                    {
                        cmp = _strnicmp(
                            EQ_METHOD(1, char *, 0x0041BE53, (struct CharacterProfile *, int), g_pc_guess)(index), word,
                            strlen(word));
                    }
                    if (cmp != 0)
                    {
                        index = index + 1;
                    }
                    else
                    {
                        gotRace = 1;
                        buffers.filters.raceFilter = (short)index;
                        goto nextWord;
                    }
                } while ((short)index < 0xd6);
            }
            cmp = EQ_CDECL(int, 0x00522F25, (int))((int)*word);
            if (cmp != 0)
            {
                if (gotLevelArg1 == 0)
                {
                    gotLevelArg1 = 1;
                    levelArg1 = atoi(word);
                    continue;
                }
                if (gotLevelArg2 == 0)
                {
                    gotLevelArg2 = 1;
                    levelArg2 = atoi(word);
                    continue;
                }
            }
            cmp = EQ_CDECL(int, 0x004DFE29, (char *))(word);
            if (cmp == -1)
            {
                strcpy(buffers.nameFilter, word);
            }
            else
            {
                buffers.filters.guildFilter = (short)cmp;
                strcpy(buffers.nameFilter, word);
                EQTRACE_guess(EQ_ARRAY(char, 0x0056CA8C), word, (int)buffers.filters.guildFilter);
            }
        }
        else
        {
            buffers.filters.guildFilter = -2;
        }
nextWord:
        ;
    }

    shortArg1 = (short)levelArg1;
    shortArg2 = (short)levelArg2;
    if (0 < shortArg1 || 0 < shortArg2)
    {
        if (shortArg1 < 1)
        {
            buffers.filters.levelMin = shortArg2;
            buffers.filters.levelMax = shortArg2;
        }
        else
        {
            if (shortArg2 < 1)
                shortArg2 = shortArg1;
            if (shortArg1 < shortArg2)
            {
                buffers.filters.levelMin = shortArg1;
                buffers.filters.levelMax = shortArg2;
            }
            else
            {
                buffers.filters.levelMin = shortArg2;
                buffers.filters.levelMax = shortArg1;
            }
        }
    }
    if ((short)whoType == 3)
    {
        if (buffers.filters.guildFilter == -2)
        {
            index = 0;
            buffers.friendList[0] = '\0';
            friendName = EQ_ARRAY(char, 0x006E2114);
            do
            {
                if (*friendName != '\0' && strlen(friendName) <= 0x0f)
                {
                    if (strlen(friendName) + sizeof(",") + strlen(buffers.friendList) < 0x800)
                    {
                        if (index == 0)
                        {
                            strcpy(buffers.friendList, friendName);
                        }
                        else
                        {
                            strcat(buffers.friendList, EQ_ARRAY(char, 0x005422BC));
                            strcat(buffers.friendList, friendName);
                        }
                        index = index + 1;
                    }
                }
                friendName = friendName + 0x20;
            } while ((int)friendName < 0x006e24d4);
            if (index > 0)
            {
                len = strlen(buffers.friendList);
                send_message_guess(g_world_connection_guess, 0x21c5, buffers.friendList, len + 1, 1);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056CA6C), 0x119);
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056CA48), 0x119);
            }
            else
            {
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056CA08), 0x119);
            }
        }
        else
        {
            send_message_guess(g_world_connection_guess, 0x20f4, buffers.nameFilter, 0x4c, 1);
        }
        return;
    }
    if (buffers.filters.guildFilter == -3)
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056C9EC), 0x119);
    else
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056C9D4), 0x119);
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(EQ_ARRAY(char, 0x0056C9B8), 0x119);
    for (player = g_player_top_guess; player != 0; player = player->next)
    {
        matches = true;
        if (buffers.filters.classFilter != -1 && (player->class_ != buffers.filters.classFilter || player->anon != 0))
            matches = false;
        if (buffers.filters.raceFilter != -1 && ((unsigned int)player->race != (int)buffers.filters.raceFilter || player->anon != 0))
            matches = false;
        if (buffers.filters.levelMin != -1 && ((short)player->level < buffers.filters.levelMin || (short)player->level > buffers.filters.levelMax || player->anon != 0))
            matches = false;
        if (buffers.filters.gmFilter != -1 && (char)player->gm != buffers.filters.gmFilter)
            matches = false;
        if (buffers.filters.guildFilter == -3 && player->actor_info != 0 && player->actor_info->unk_0x300 == 0)
            matches = false;
        if (buffers.filters.guildFilter > -1)
        {
            if (player->guild_id != buffers.filters.guildFilter)
                matches = false;
        }
        else if (buffers.nameFilter[0] != '\0')
        {
            len = strlen(buffers.nameFilter);
            cmp = _strnicmp(buffers.nameFilter, player->name, len);
            if (cmp != 0)
                matches = false;
        }
        if (player->gm != 0 && player->anon != 0)
            matches = false;
        if (matches)
        {
            if (player->guild_id == -1 || player->anon == 1)
                buffers.guildSuffix[0] = '\0';
            else
                sprintf(buffers.guildSuffix, EQ_ARRAY(char, 0x0056C9B0), EQ_CDECL(char *, 0x004E00F6, (short))(player->guild_id));
            if (player->afk != 0)
                strcpy(buffers.afkTag, EQ_ARRAY(char, 0x0056C9A8));
            else
                buffers.afkTag[0] = '\0';
            if (player->gm != 0)
                strcpy(buffers.gmTag, EQ_ARRAY(char, 0x0056C9A0));
            else
                buffers.gmTag[0] = '\0';
            if (player->linkdead == 0 || *((char *)player + 0xa0) != '\0')
                buffers.linkDeadTag[0] = '\0';
            else
                strcpy(buffers.linkDeadTag, EQ_ARRAY(char, 0x0056C994));
            if (player->actor_info->unk_0x300 == 1)
                strcpy(buffers.lfgTag, EQ_ARRAY(char, 0x0056C98C));
            else
                buffers.lfgTag[0] = '\0';
            if ((short)whoType == 0)
            {
                if (player->type != 0)
                    continue;
                if (player->anon != 0)
                {
                    sprintf(buffers.line, EQ_ARRAY(char, 0x0056C970), buffers.afkTag, buffers.linkDeadTag, buffers.gmTag,
                        EQ_METHOD(1, char *, 0x004D02FD, (CEverQuest *, char *), this)(player->name),
                        buffers.guildSuffix, buffers.lfgTag);
                }
                else
                {
                    sprintf(buffers.line, EQ_ARRAY(char, 0x0056C954), buffers.afkTag, buffers.linkDeadTag, buffers.gmTag, (int)player->level,
                        EQ_METHOD(3, char *, 0x0041C0F7, (struct CharacterProfile *, short, short, int), g_pc_guess)(player->class_, player->level, 0x18c),
                        EQ_METHOD(1, char *, 0x004D02FD, (CEverQuest *, char *), this)(player->name),
                        EQ_METHOD(1, char *, 0x0041BE53, (struct CharacterProfile *, short), g_pc_guess)(player->race),
                        buffers.guildSuffix, buffers.lfgTag);
                }
            }
            else if ((short)whoType == 1)
            {
                if (player->type != 1)
                    continue;
                sprintf(buffers.line, EQ_ARRAY(char, 0x0056C940), (int)player->level,
                    EQ_METHOD(1, char *, 0x004C9D4E, (CEverQuest *, short), this)(player->class_),
                    EQ_METHOD(1, char *, 0x004D02FD, (CEverQuest *, char *), this)(player->name),
                    EQ_METHOD(1, char *, 0x004C9105, (CEverQuest *, short), this)(player->race));
            }
            else
            {
                if (player->type < 2)
                    continue;
                if (_strnicmp(
                        player->name, g_pc_guess->name, strlen(g_pc_guess->name)) != 0)
                    continue;
                sprintf(buffers.line, EQ_ARRAY(char, 0x0056C930), (int)player->level,
                    EQ_METHOD(3, char *, 0x0041C0F7, (struct CharacterProfile *, short, short, int), g_pc_guess)(player->class_, player->level, 0x18c),
                    EQ_METHOD(1, char *, 0x004D02FD, (CEverQuest *, char *), this)(player->name),
                    EQ_METHOD(1, char *, 0x0041BE53, (struct CharacterProfile *, short), g_pc_guess)(player->race));
            }
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(buffers.line, 0x119);
            playerCount = playerCount + 1;
        }
    }
    EQ_METHOD(2, void, 0x004BEA23, (void *, char *, char *), EQ_GLOBAL(void *, 0x006E4D54))(g_local_player_guess->zone, buffers.zoneName);
    shortCount = (short)playerCount;
    if (shortCount > 1)
        sprintf(buffers.line, EQ_ARRAY(char, 0x0056C914), (int)shortCount, buffers.zoneName);
    else if (shortCount == 0)
        sprintf(buffers.line, EQ_ARRAY(char, 0x0056C8D8), buffers.zoneName);
    else
        sprintf(buffers.line, EQ_ARRAY(char, 0x0056C8BC), (int)shortCount, buffers.zoneName);
    EQ_METHOD(2, void, 0x004D055E, (CEverQuest *, const char *, int), this)(buffers.line, 0x119);
}

class CEverQuest_GuildPeace_guess_class : public CEverQuest
{
public:
    void GuildPeace(char* param_1);
};

// FUNCTION: TRILOGY 0x004cb1a5
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, one stack argument; `this` is unused (the chat lines go to the global CEverQuest).
void CEverQuest_GuildPeace_guess_class::GuildPeace(char* recipientName)
{
    char chatLine[256];
    char peaceRequest[72];

    if (g_pc_guess->guild_rank == 2)
    {
        sprintf(chatLine, (char*)0x0056cda8, recipientName);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(chatLine);
        strcpy(peaceRequest, recipientName);
        strcpy(peaceRequest + 30, (char*)g_pc_guess);
        *(int*)(peaceRequest + 60) = (int)g_pc_guess->guild_id;
        *(int*)(peaceRequest + 68) = 1;
        *(int*)(peaceRequest + 64) = 0;
        send_message_guess
            (g_world_connection_guess, 0x2191, peaceRequest, 0x48, 1);
    }
    else
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0056cd80);
    }
}

// FUNCTION: TRILOGY 0x004cb248
// Presents an incoming group invite with options to accept or decline.
void __fastcall HandleGroupInvite_guess(void* thisPtr, int unusedEdx, int inviteMessage)
{
    char* _Str1;
    int comparisonResult;
    char chatLine[120];
    struct GroupInviteMessage_guess
    {
        char local_48[30];
        char local_2a[30];
        char local_c;
    } local_message;

    _Str1 = (char*)(inviteMessage + 0x3c);
    pinstEQ_Main_guess->uiDirty_guess = 1;
    comparisonResult = strcmp(_Str1, (const char*)0x0058fba4);
    if (comparisonResult != 0)
    {
        sprintf(chatLine, (const char*)0x0056ce4c, (char*)(inviteMessage + 0x1e), _Str1);
    }
    else
    {
        sprintf(chatLine, (const char*)0x0056ce2c, (char*)(inviteMessage + 0x1e));
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), thisPtr)(chatLine);
    if (pinstEQ_Main_guess->groupInvitePending_guess != '\0')
    {
        local_message.local_c = 1;
    }
    else if (EQ_GLOBAL(char, 0x005FE968) != '\0')
    {
        local_message.local_c = 2;
    }
    else
    {
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), thisPtr)((const char*)0x0056cde0);
        pinstEQ_Main_guess->uiDirty_guess = 1;
        pinstEQ_Main_guess->groupInvitePending_guess = 1;
        strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xb6, (char*)(inviteMessage + 0x1e));
        strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xd4, _Str1);
        return;
    }
    strcpy(local_message.local_48, (char*)(inviteMessage + 0x1e));
    strcpy(local_message.local_2a, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1);
    send_message_guess
        (g_world_connection_guess, 0x2041, local_message.local_48, 0x3d, 1);
    return;
}

class CEverQuest_004cb362_guess
{
public:
    void AcceptGroupInvite_guess(void);
};

// FUNCTION: TRILOGY 0x004cb362
// Accepts a pending group invitation and reports the result in chat.
void CEverQuest_004cb362_guess::AcceptGroupInvite_guess(void)
{
    int value;
    char chatLine[120];
    char groupReply[60];

    if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
    {
        value = (int)EQ_METHOD(1, void*, 0x004B1435, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D58))(
             (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6);
        if (value == 0 || *(char*)(value + 0x98) != '\0')
        {
            sprintf(chatLine, EQ_ARRAY(char, 0x0056CE68),
                 (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6);
            ((CEverQuest*)this)->dsp_chat_guess(chatLine, 0xD);
            pinstEQ_Main_guess->groupInvitePending_guess = 0;
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6,
                 EQ_ARRAY(char, 0x0058FBA4));
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xD4,
                 EQ_ARRAY(char, 0x0058FBA4));
        }
        else
        {
            value = strcmp((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xD4,
                 EQ_ARRAY(char, 0x0058FBA4));
            if (value == 0)
            {
                sprintf(chatLine, EQ_ARRAY(char, 0x0056CEE8),
                     (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6);
            }
            else
            {
                sprintf(chatLine, EQ_ARRAY(char, 0x0056CEBC),
                     (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6,
                     (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xD4);
            }
            ((CEverQuest*)this)->print_chat_guess(chatLine);
            pinstEQ_Main_guess->groupInvitePending_guess = 0;
            strcpy(groupReply, (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6);
            strcpy(groupReply + 0x1E, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
            strcpy(g_name_buffer_guess,
                 (char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6);
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2042, groupReply, 0x3C, 1);
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xB6,
                 EQ_ARRAY(char, 0x0058FBA4));
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xD4,
                 EQ_ARRAY(char, 0x0058FBA4));
        }
    }
}

class CEverQuest_004cb4fa_guess
{
public:
    void DisbandGroup_guess(void);
};

// FUNCTION: TRILOGY 0x004cb4fa
// Handles party departure, member removal, and cancellation of a pending group invite.
void CEverQuest_004cb4fa_guess::DisbandGroup_guess(void)
{
    int value;
    char chatLine[120];
    MsgGroupDecline declineMessage;
    struct
    {
        char name[30];      // 0x00  OP_GroupQuit payload (0x3C bytes): two 30-byte names
        char name2[30];     // 0x1E
    } groupQuitMessage;

    value = _strcmpi(g_name_buffer_guess, (char*)EQ_GLOBAL(void*, 0x006E4DA8));
    if (value == 0)
    {
        if (pinstEQ_Main_guess->groupSelected_guess < '\0' ||
            (value = pinstEQ_Main_guess->groupSelected_guess * 0x1E,
             *(char*)(EQ_ARRAY(char, 0x005FE8BD) + value) == '\0'))
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(EQ_ARRAY(char, 0x0056CF48));
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2197, 0, 0, 1);
        }
        else
        {
            strcpy(groupQuitMessage.name, EQ_ARRAY(char, 0x005FE8BD) + value);
            strcpy(groupQuitMessage.name2, EQ_ARRAY(char, 0x005FE8BD) +
                 pinstEQ_Main_guess->groupSelected_guess * 0x1E);
            send_message_guess
                (&EQ_GLOBAL(void*, 0x006B3194), 0x2044, &groupQuitMessage, 0x3C, 1);
            sprintf(chatLine, EQ_ARRAY(char, 0x0056CF60),
                 EQ_ARRAY(char, 0x005FE8BD) +
                 pinstEQ_Main_guess->groupSelected_guess * 0x1E);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(chatLine);
            pinstEQ_Main_guess->groupSelected_guess = (char)0xFF;
        }
    }
    else if (pinstEQ_Main_guess->groupInvitePending_guess != '\0')
    {
        sprintf(chatLine, EQ_ARRAY(char, 0x0056CF18),
             (char*)(EQ_GLOBAL(int, 0x006E4E44) + 0xB6));
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(chatLine);
        strcpy(declineMessage.leader, (char*)(EQ_GLOBAL(int, 0x006E4E44) + 0xB6));
        strcpy(declineMessage.leaver, (char*)EQ_GLOBAL(void*, 0x006E4DA8));
        declineMessage.action = 3;
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2041, &declineMessage, 0x3D, 1);
        pinstEQ_Main_guess->groupInvitePending_guess = 0;
        strcpy((char*)(EQ_GLOBAL(int, 0x006E4E44) + 0xB6), EQ_ARRAY(char, 0x0058FBA4));
        strcpy((char*)(EQ_GLOBAL(int, 0x006E4E44) + 0xD4), EQ_ARRAY(char, 0x0058FBA4));
    }
    else
    {
        strcpy(groupQuitMessage.name, (char*)EQ_GLOBAL(void*, 0x006E4DA8));
        strcpy(groupQuitMessage.name2, (char*)EQ_GLOBAL(void*, 0x006E4DA8));
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x2044, &groupQuitMessage, 0x3C, 1);
    }
}

// FUNCTION: TRILOGY 0x004cb6e6
// Toggles the character between sitting and standing.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_guess::Sit_guess(void)
{
    if (g_pc_guess->stand_state == 'd' || g_pc_guess->stand_state == 'o')
    {
        EQPlayer* player = *(EQPlayer* volatile*)&g_local_player_guess;
        if (player->stand_state != 'f')
            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer *, int), player)(0x6e);
    }
    else
    {
        EQPlayer* player = *(EQPlayer* volatile*)&g_local_player_guess;
        EQ_METHOD(1, void, 0x004B45CB, (EQPlayer *, int), player)(100);
    }

    if (g_eqmain_guess != NULL)
    {
        pinstEQ_Main_guess->uiDirty_u32_guess = 1;
    }
}


struct SocialAnimPacket_guess
{
    u32 spawnId;
    u32 anim;
    float speed;
};

// FUNCTION: TRILOGY 0x004cc92f
// Social emote (smile, frown, hug...) at a nearby target or none: sent with its animation, echoed to you.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, three stack arguments (`ret 0xc`): social id, target name (reused for the EQPlayer*), extra text.
// Builds the third-person message for the server (0x2015) and animation packet (0x209f), then the local
// second-person echo. The second dispatch has no bounds check in the original.
void __fastcall CEverQuest_send_social_guess_impl(CEverQuest* game, int unusedEdx, short socialId,
                                                  EQPlayer* target, char* text)
{
    unsigned anim;
    char his[8];
    char he[8];
    char selfName[32];
    char targetName[32];
    char him[8];
    CEverQuest* self;
    char emoteText[128];
    char tHim[8];
    char tHe[8];
    char tHis[8];
    char youText[128];
    SocialAnimPacket_guess packet;
    char message[512];
    short len;
    const char* src;
    char* chat;

    self = game;
    len = (short)strlen((char*)target);
    anim = 0xffffffff;
    if (EQ_GLOBAL(char, 0x006B6465) != 0)
    {
        return;
    }
    if (len > 0 &&
        _strcmpi((char*)target, EQ_ARRAY(char, 0x0056F148)) != 0)
    {
        target = EQ_METHOD(1, EQPlayer*, 0x004B146A, (EQPlayer*, char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))((char*)target);
        if (target == 0)
        {
            goto notFound;
        }
    }
    else
    {
        target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC);
    }
    if (target == g_local_player_guess)
    {
        target = 0;
    }
    selfName[0] = 0;
    switch (g_local_player_guess->gender)
    {
    case 0:
        strcpy(he, EQ_ARRAY(char, 0x0056B7A8));
        strcpy(him, EQ_ARRAY(char, 0x005681D8));
        src = EQ_ARRAY(char, 0x005681DC);
        break;
    case 1:
        strcpy(he, EQ_ARRAY(char, 0x0056B7AC));
        strcpy(him, EQ_ARRAY(char, 0x0056AAC0));
        src = EQ_ARRAY(char, 0x0056AAC0);
        break;
    default:
        strcpy(he, EQ_ARRAY(char, 0x00568220));
        strcpy(him, EQ_ARRAY(char, 0x00568220));
        src = EQ_ARRAY(char, 0x00568224);
        break;
    }
    strcpy(his, src);
    if (target != 0)
    {
        if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_display_guess)(g_local_player_guess, target, 0.0) > 60.0f)
        {
        notFound:
            chat = EQ_ARRAY(char, 0x0056F118);
            goto printChat;
        }
        strcpy(targetName,
                EQ_METHOD(1, char*, 0x004D0436, (CEverQuest*, char*), self)((char*)target + 1));
        switch (target->gender)
        {
        case 0:
            strcpy(tHe, EQ_ARRAY(char, 0x0056B7A8));
            strcpy(tHim, EQ_ARRAY(char, 0x005681D8));
            src = EQ_ARRAY(char, 0x005681DC);
            break;
        case 1:
            strcpy(tHe, EQ_ARRAY(char, 0x0056B7AC));
            strcpy(tHim, EQ_ARRAY(char, 0x0056AAC0));
            src = EQ_ARRAY(char, 0x0056AAC0);
            break;
        default:
            strcpy(tHe, EQ_ARRAY(char, 0x00568220));
            strcpy(tHim, EQ_ARRAY(char, 0x00568220));
            src = EQ_ARRAY(char, 0x00568224);
            break;
        }
        strcpy(tHis, src);
    }
    youText[0] = 0;
    emoteText[0] = 0;
    message[0] = 0;
    switch (socialId)
    {
    case 0:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F110));
        strcpy(youText, EQ_ARRAY(char, 0x0056F0FC));
        break;
    case 1:
        anim = 0x1c;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F0F4));
        strcpy(youText, EQ_ARRAY(char, 0x0056F0E8));
        break;
    case 2:
        anim = 0x3f;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F0E0));
        strcpy(youText, EQ_ARRAY(char, 0x0056F0D4));
        break;
    case 3:
        anim = 0x1c;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F0CC));
        strcpy(youText, EQ_ARRAY(char, 0x0056F0C0));
        break;
    case 4:
        anim = 0x36;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F0B4));
        strcpy(youText, EQ_ARRAY(char, 0x0056F0A8));
        break;
    case 5:
        sprintf(emoteText, EQ_ARRAY(char, 0x0056F09C), him);
        strcpy(youText, EQ_ARRAY(char, 0x0056F094));
        break;
    case 6:
        sprintf(emoteText, EQ_ARRAY(char, 0x0056F084), him);
        strcpy(youText, EQ_ARRAY(char, 0x0056F07C));
        break;
    case 7:
        anim = 0x46;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F074));
        strcpy(youText, EQ_ARRAY(char, 0x0056F068));
        break;
    case 8:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F050));
        strcpy(youText, EQ_ARRAY(char, 0x0056F040));
        break;
    case 9:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F02C));
        strcpy(youText, EQ_ARRAY(char, 0x0056F014));
        break;
    case 10:
        anim = 0x30;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056F00C));
        strcpy(youText, EQ_ARRAY(char, 0x0056F004));
        break;
    case 11:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EFE8));
        strcpy(youText, EQ_ARRAY(char, 0x0056EFC8));
        break;
    case 12:
        anim = 0x1d;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EFC0));
        strcpy(youText, EQ_ARRAY(char, 0x0056EFB4));
        break;
    case 13:
        anim = 0x1b;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EFAC));
        strcpy(youText, EQ_ARRAY(char, 0x0056EFA0));
        break;
    case 14:
        anim = 0x1e;
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EF88));
        strcpy(youText, EQ_ARRAY(char, 0x0056EF70));
        break;
    case 15:
        anim = 0x30;
        strcpy(youText, EQ_ARRAY(char, 0x0056EF64));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EF44), him);
        break;
    case 16:
        anim = 0x31;
        sprintf(message, EQ_ARRAY(char, 0x0056EF28), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EF1C));
        break;
    case 17:
        anim = 0x32;
        sprintf(message, EQ_ARRAY(char, 0x0056EEF8), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EEE0));
        break;
    case 18:
        anim = 0x33;
        sprintf(message, EQ_ARRAY(char, 0x0056EEC0), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EEA4));
        break;
    case 19:
        anim = 0x32;
        sprintf(message, EQ_ARRAY(char, 0x0056EE84), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EE64), him);
        break;
    case 20:
        sprintf(message, EQ_ARRAY(char, 0x0056EE4C), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EE28));
        break;
    case 21:
        anim = 0x34;
        strcpy(youText, EQ_ARRAY(char, 0x0056EE18));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EE08));
        break;
    case 22:
        anim = 0x3b;
        sprintf(message, EQ_ARRAY(char, 0x0056EDE8), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056EDD4));
        break;
    case 23:
        anim = 0x35;
        strcpy(youText, EQ_ARRAY(char, 0x0056EDC8));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EDB4));
        break;
    case 24:
        anim = 0x31;
        sprintf(message, EQ_ARRAY(char, 0x0056ED7C), selfName, targetName, his);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056ED4C), his);
        break;
    case 25:
        sprintf(message, EQ_ARRAY(char, 0x0056ED30), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056ED0C));
        break;
    case 26:
        anim = 0x1f;
        sprintf(message, EQ_ARRAY(char, 0x0056ECEC), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056ECC8), he);
        break;
    case 27:
        anim = 0x1d;
        sprintf(message, EQ_ARRAY(char, 0x0056EC9C), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EC74), he);
        break;
    case 28:
        anim = 0x37;
        strcpy(youText, EQ_ARRAY(char, 0x0056EC64));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EC54));
        break;
    case 29:
        anim = 0x1d;
        strcpy(youText, EQ_ARRAY(char, 0x0056EC40));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EC24));
        break;
    case 30:
        anim = 0x36;
        strcpy(youText, EQ_ARRAY(char, 0x0056EC0C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EBF8));
        break;
    case 31:
        strcpy(youText, EQ_ARRAY(char, 0x0056EBE4));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EBCC));
        break;
    case 32:
        anim = 0x33;
        sprintf(message, EQ_ARRAY(char, 0x0056EBA8), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EB84), his);
        break;
    case 33:
        strcpy(youText, EQ_ARRAY(char, 0x0056EB78));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EB60));
        break;
    case 34:
        anim = 0x1b;
        sprintf(message, EQ_ARRAY(char, 0x0056EB38), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EB04), him);
        break;
    case 35:
        anim = 0x37;
        strcpy(youText, EQ_ARRAY(char, 0x0056EAF8));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EAF0));
        break;
    case 36:
        anim = 0x38;
        strcpy(youText, EQ_ARRAY(char, 0x0056EADC));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EAD4));
        break;
    case 37:
        anim = 0x39;
        sprintf(message, EQ_ARRAY(char, 0x0056EAB8), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EA9C), him);
        break;
    case 38:
        anim = 0x3a;
        sprintf(message, EQ_ARRAY(char, 0x0056EA6C), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EA3C), his);
        break;
    case 39:
        anim = 0x3d;
        strcpy(youText, EQ_ARRAY(char, 0x0056EA2C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056EA00), him);
        break;
    case 40:
        anim = 0x38;
        strcpy(youText, EQ_ARRAY(char, 0x0056E9F0));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E9E8));
        break;
    case 41:
        anim = 0x3c;
        strcpy(youText, EQ_ARRAY(char, 0x0056E9D0));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E9B0));
        break;
    case 42:
        anim = 0x31;
        sprintf(message, EQ_ARRAY(char, 0x0056E990), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E978));
        break;
    case 43:
        anim = 0x36;
        strcpy(youText, EQ_ARRAY(char, 0x0056E96C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E964));
        break;
    case 44:
        anim = 0x3c;
        strcpy(youText, EQ_ARRAY(char, 0x0056E94C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E92C));
        break;
    case 45:
        strcpy(youText, EQ_ARRAY(char, 0x0056E91C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E90C));
        break;
    case 46:
        anim = 0x1c;
        strcpy(youText, EQ_ARRAY(char, 0x0056E8F4));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E8EC));
        break;
    case 47:
        anim = 0x32;
        strcpy(youText, EQ_ARRAY(char, 0x0056E8DC));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E8C8));
        break;
    case 48:
        anim = 0x1b;
        strcpy(youText, EQ_ARRAY(char, 0x0056E8B4));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E8A4));
        break;
    case 49:
        anim = 0x34;
        sprintf(message, EQ_ARRAY(char, 0x0056E878), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E864));
        break;
    case 50:
        anim = 0x1d;
        sprintf(message, EQ_ARRAY(char, 0x0056E84C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E82C), him);
        break;
    case 51:
        sprintf(message, EQ_ARRAY(char, 0x0056E800), selfName, targetName, he);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E7EC));
        break;
    case 52:
        anim = 0x3e;
        sprintf(message, EQ_ARRAY(char, 0x0056E7BC), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E7B0));
        break;
    case 53:
        sprintf(message, EQ_ARRAY(char, 0x0056E784), selfName, targetName, he);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E76C));
        break;
    case 54:
        sprintf(message, EQ_ARRAY(char, 0x0056E750), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E72C));
        break;
    case 55:
        strcpy(youText, EQ_ARRAY(char, 0x0056E720));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E710));
        break;
    case 56:
        anim = 0x1c;
        sprintf(message, EQ_ARRAY(char, 0x0056E6E4), selfName, his, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E6B4), his);
        break;
    case 57:
        anim = 0x3d;
        sprintf(message, EQ_ARRAY(char, 0x0056E68C), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E674));
        break;
    case 58:
        anim = 0x40;
        sprintf(message, EQ_ARRAY(char, 0x0056E658), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E640));
        break;
    case 59:
        anim = 0x41;
        sprintf(message, EQ_ARRAY(char, 0x0056E614), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E5F8));
        break;
    case 60:
        anim = 0x39;
        sprintf(message, EQ_ARRAY(char, 0x0056E5D4), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E5B8));
        break;
    case 61:
        anim = 0x42;
        sprintf(message, EQ_ARRAY(char, 0x0056E594), selfName, targetName, his);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E584), his);
        break;
    case 62:
        anim = 0x42;
        sprintf(message, EQ_ARRAY(char, 0x0056E568), selfName, targetName, tHe);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E540), he);
        break;
    case 63:
        anim = 0x1b;
        strcpy(youText, EQ_ARRAY(char, 0x0056E518));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E4E8));
        break;
    case 64:
        anim = 0x43;
        sprintf(message, EQ_ARRAY(char, 0x0056E4B8), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E494));
        break;
    case 65:
        anim = 0x44;
        sprintf(message, EQ_ARRAY(char, 0x0056E464), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E450));
        break;
    case 66:
        anim = 0x41;
        sprintf(message, EQ_ARRAY(char, 0x0056E440), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E42C));
        break;
    case 67:
        anim = 0x1c;
        sprintf(message, EQ_ARRAY(char, 0x0056E41C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E400));
        break;
    case 68:
        sprintf(message, EQ_ARRAY(char, 0x0056E3E0), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E3C8));
        break;
    case 69:
        anim = 0x3c;
        sprintf(message, EQ_ARRAY(char, 0x0056E3B0), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E38C), his);
        break;
    case 70:
        anim = 0x36;
        sprintf(message, EQ_ARRAY(char, 0x0056E370), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E360));
        break;
    case 71:
        anim = 0x39;
        sprintf(message, EQ_ARRAY(char, 0x0056E328), selfName, targetName, tHis);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E310));
        break;
    case 72:
        anim = 0x45;
        sprintf(message, EQ_ARRAY(char, 0x0056E2E0), selfName, his, he, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E2C4), his);
        break;
    case 73:
        sprintf(message, EQ_ARRAY(char, 0x0056E2A8), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E284));
        break;
    case 74:
        anim = 0x46;
        sprintf(message, EQ_ARRAY(char, 0x0056E26C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E25C));
        break;
    case 75:
        sprintf(message, EQ_ARRAY(char, 0x0056E230), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E21C));
        break;
    case 76:
        anim = 0x3b;
        sprintf(message, EQ_ARRAY(char, 0x0056E208), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E1F8));
        break;
    case 77:
        anim = 0x1d;
        sprintf(message, EQ_ARRAY(char, 0x0056E1E0), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E1CC));
        break;
    case 78:
        sprintf(message, EQ_ARRAY(char, 0x0056E1B0), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E19C));
        break;
    case 79:
        anim = 0x3d;
        sprintf(message, EQ_ARRAY(char, 0x0056E178), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E160));
        break;
    case 80:
        anim = 0x1f;
        sprintf(message, EQ_ARRAY(char, 0x0056E128), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056E108), his);
        break;
    default:
        return;
    }
    if (g_local_player_guess->speed != 0.0f || g_local_player_guess->stand_state != 'd')
    {
        anim = 0xffffffff;
    }
    if (strlen(text) > 1)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056E100), selfName, text);
    }
    else if (target == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0053E668), selfName, emoteText);
    }
    else if (message[0] == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056E0F4), selfName, youText, targetName);
    }
    if (message[0] != 0)
    {
        len = (short)strlen(message);
        if (EQ_CDECL(int, 0x00522ECF, (int))(message[len - 1]) != 0 ||
            EQ_CDECL(int, 0x00522F25, (int))(message[len - 1]) != 0)
        {
            strcat(message, EQ_ARRAY(char, 0x00545998));
        }
        EQ_METHOD(1, void, 0x004D061A, (CEverQuest*, char*), self)(message);
        len = (short)strlen(message);
        message[len] = 0;
        send_message_guess
            (g_world_connection_guess, 0x2015, message, len + 1, 1);
    }
    if (anim != 0xffffffff)
    {
        packet.spawnId = g_local_player_guess->spawn_id;
        packet.anim = anim;
        packet.speed = 1.0;
        send_message_guess
            (g_world_connection_guess, 0x209f, &packet, 0xc, 1);
    }
    youText[0] = 0;
    emoteText[0] = 0;
    message[0] = 0;
    strcpy(selfName, EQ_ARRAY(char, 0x0056B18C));
    strcpy(he, EQ_ARRAY(char, 0x0056E0F0));
    strcpy(him, EQ_ARRAY(char, 0x0056E0F0));
    strcpy(his, EQ_ARRAY(char, 0x0056E0E8));
    switch (socialId)
    {
    case 0:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E0E0));
        strcpy(youText, EQ_ARRAY(char, 0x0056E0D0));
        break;
    case 1:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E0C8));
        strcpy(youText, EQ_ARRAY(char, 0x0056E0BC));
        break;
    case 2:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E0B4));
        strcpy(youText, EQ_ARRAY(char, 0x0056E0A8));
        break;
    case 3:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E0A4));
        strcpy(youText, EQ_ARRAY(char, 0x0056E098));
        break;
    case 4:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E090));
        strcpy(youText, EQ_ARRAY(char, 0x0056E084));
        break;
    case 5:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E074));
        strcpy(youText, EQ_ARRAY(char, 0x0056E070));
        break;
    case 6:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E060));
        strcpy(youText, EQ_ARRAY(char, 0x0056E058));
        break;
    case 7:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E054));
        strcpy(youText, EQ_ARRAY(char, 0x0056E048));
        break;
    case 8:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E030));
        strcpy(youText, EQ_ARRAY(char, 0x0056E020));
        break;
    case 9:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056E00C));
        strcpy(youText, EQ_ARRAY(char, 0x0056DFF4));
        break;
    case 10:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DFF0));
        strcpy(youText, EQ_ARRAY(char, 0x0056DFE8));
        break;
    case 11:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DFCC));
        strcpy(youText, EQ_ARRAY(char, 0x0056DFAC));
        break;
    case 12:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DFA4));
        strcpy(youText, EQ_ARRAY(char, 0x0056DF9C));
        break;
    case 13:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DF94));
        strcpy(youText, EQ_ARRAY(char, 0x0056DF88));
        break;
    case 14:
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DF74));
        strcpy(youText, EQ_ARRAY(char, 0x0056DF5C));
        break;
    case 15:
        strcpy(youText, EQ_ARRAY(char, 0x0056DF50));
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DF30));
        break;
    case 16:
        sprintf(message, EQ_ARRAY(char, 0x0056DF14), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DF08));
        break;
    case 17:
        sprintf(message, EQ_ARRAY(char, 0x0056DEE4), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DECC));
        break;
    case 18:
        sprintf(message, EQ_ARRAY(char, 0x0056DEB0), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DE94));
        break;
    case 19:
        sprintf(message, EQ_ARRAY(char, 0x0056DE74), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DE54));
        break;
    case 20:
        sprintf(message, EQ_ARRAY(char, 0x0056DE3C), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DE18));
        break;
    case 21:
        strcpy(youText, EQ_ARRAY(char, 0x0056DE08));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DDF8));
        break;
    case 22:
        sprintf(message, EQ_ARRAY(char, 0x0056DDDC), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DDC8));
        break;
    case 23:
        strcpy(youText, EQ_ARRAY(char, 0x0056DDBC));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DDAC));
        break;
    case 24:
        sprintf(message, EQ_ARRAY(char, 0x0056DD74), selfName, targetName, his);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DD44), his);
        break;
    case 25:
        sprintf(message, EQ_ARRAY(char, 0x0056DD2C), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056DD08));
        break;
    case 26:
        sprintf(message, EQ_ARRAY(char, 0x0056DCE8), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DCC4), he);
        break;
    case 27:
        sprintf(message, EQ_ARRAY(char, 0x0056DC98), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DC74), he);
        break;
    case 28:
        strcpy(youText, EQ_ARRAY(char, 0x0056DC64));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DC58));
        break;
    case 29:
        strcpy(youText, EQ_ARRAY(char, 0x0056DC48));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DC2C));
        break;
    case 30:
        strcpy(youText, EQ_ARRAY(char, 0x0056DC18));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DC04));
        break;
    case 31:
        strcpy(youText, EQ_ARRAY(char, 0x0056DBF0));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DBD8));
        break;
    case 32:
        sprintf(message, EQ_ARRAY(char, 0x0056DBB4), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DB90), his);
        break;
    case 33:
        strcpy(youText, EQ_ARRAY(char, 0x0056DB88));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DB70));
        break;
    case 34:
        sprintf(message, EQ_ARRAY(char, 0x0056DB48), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DB18), him);
        break;
    case 35:
        strcpy(youText, EQ_ARRAY(char, 0x0056DB0C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DB04));
        break;
    case 36:
        strcpy(youText, EQ_ARRAY(char, 0x0056DAF0));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DAE8));
        break;
    case 37:
        sprintf(message, EQ_ARRAY(char, 0x0056DAD0), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DAB4), him);
        break;
    case 38:
        sprintf(message, EQ_ARRAY(char, 0x0056DA84), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DA58), his);
        break;
    case 39:
        strcpy(youText, EQ_ARRAY(char, 0x0056DA48));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DA1C), him);
        break;
    case 40:
        strcpy(youText, EQ_ARRAY(char, 0x0056DA10));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056DA08));
        break;
    case 41:
        strcpy(youText, EQ_ARRAY(char, 0x0056D9F4));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D9D4));
        break;
    case 42:
        sprintf(message, EQ_ARRAY(char, 0x0056D9B4), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056D99C));
        break;
    case 43:
        strcpy(youText, EQ_ARRAY(char, 0x0056D990));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D988));
        break;
    case 44:
        strcpy(youText, EQ_ARRAY(char, 0x0056D970));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D950));
        break;
    case 45:
        strcpy(youText, EQ_ARRAY(char, 0x0056D940));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D934));
        break;
    case 46:
        strcpy(youText, EQ_ARRAY(char, 0x0056D91C));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D914));
        break;
    case 47:
        strcpy(youText, EQ_ARRAY(char, 0x0056D904));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D8F0));
        break;
    case 48:
        strcpy(youText, EQ_ARRAY(char, 0x0056D8DC));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D8CC));
        break;
    case 49:
        sprintf(message, EQ_ARRAY(char, 0x0056D8A4), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D890));
        break;
    case 50:
        sprintf(message, EQ_ARRAY(char, 0x0056D880), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D860), him);
        break;
    case 51:
        sprintf(message, EQ_ARRAY(char, 0x0056D834), selfName, targetName, he);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056D820));
        break;
    case 52:
        sprintf(message, EQ_ARRAY(char, 0x0056D7F0), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056D7E4));
        break;
    case 53:
        sprintf(message, EQ_ARRAY(char, 0x0056D7B8), selfName, targetName, he);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056D7A0));
        break;
    case 54:
        sprintf(message, EQ_ARRAY(char, 0x0056D784), selfName, targetName);
        strcpy(emoteText, EQ_ARRAY(char, 0x0056D760));
        break;
    case 55:
        strcpy(youText, EQ_ARRAY(char, 0x0056D758));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D748));
        break;
    case 56:
        sprintf(message, EQ_ARRAY(char, 0x0056D71C), selfName, his, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D6EC), his);
        break;
    case 57:
        sprintf(message, EQ_ARRAY(char, 0x0056D6C4), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D6AC));
        break;
    case 58:
        sprintf(message, EQ_ARRAY(char, 0x0056D690), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D678));
        break;
    case 59:
        sprintf(message, EQ_ARRAY(char, 0x0056D650), selfName, targetName, tHim);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D634));
        break;
    case 60:
        sprintf(message, EQ_ARRAY(char, 0x0056D610), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D5F8));
        break;
    case 61:
        sprintf(message, EQ_ARRAY(char, 0x0056D5D8), selfName, targetName, his);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D5C8), his);
        break;
    case 62:
        sprintf(message, EQ_ARRAY(char, 0x0056D5AC), selfName, targetName, tHe);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D584), he);
        break;
    case 63:
        strcpy(youText, EQ_ARRAY(char, 0x0056D560));
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D530));
        break;
    case 64:
        sprintf(message, EQ_ARRAY(char, 0x0056D504), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D4E0));
        break;
    case 65:
        sprintf(message, EQ_ARRAY(char, 0x0056D4B4), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D4A0));
        break;
    case 66:
        sprintf(message, EQ_ARRAY(char, 0x0056D490), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D47C));
        break;
    case 67:
        sprintf(message, EQ_ARRAY(char, 0x0056D46C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D450));
        break;
    case 68:
        sprintf(message, EQ_ARRAY(char, 0x0056D430), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D418));
        break;
    case 69:
        sprintf(message, EQ_ARRAY(char, 0x0056D400), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D3DC), his);
        break;
    case 70:
        sprintf(message, EQ_ARRAY(char, 0x0056D3C4), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D3B4));
        break;
    case 71:
        sprintf(message, EQ_ARRAY(char, 0x0056D380), selfName, targetName, tHis);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D36C));
        break;
    case 72:
        sprintf(message, EQ_ARRAY(char, 0x0056D340), selfName, his, he, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D328), his);
        break;
    case 73:
        sprintf(message, EQ_ARRAY(char, 0x0056D310), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D2F0));
        break;
    case 74:
        sprintf(message, EQ_ARRAY(char, 0x0056D2D8), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D2C8));
        break;
    case 75:
        sprintf(message, EQ_ARRAY(char, 0x0056D29C), selfName, targetName, he);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D288));
        break;
    case 76:
        sprintf(message, EQ_ARRAY(char, 0x0056D274), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D264));
        break;
    case 77:
        sprintf(message, EQ_ARRAY(char, 0x0056D24C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D238));
        break;
    case 78:
        sprintf(message, EQ_ARRAY(char, 0x0056D21C), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D20C));
        break;
    case 79:
        sprintf(message, EQ_ARRAY(char, 0x0056D1EC), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D1D4));
        break;
    case 80:
        sprintf(message, EQ_ARRAY(char, 0x0056D1B4), selfName, targetName);
        sprintf(emoteText, EQ_ARRAY(char, 0x0056D198), his);
        break;
    }
    if (strlen(text) > 1)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056D188), text);
    }
    else if (target == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056D180), emoteText);
    }
    else if (message[0] == 0)
    {
        sprintf(message, EQ_ARRAY(char, 0x0056D174), youText, targetName);
    }
    if (anim != 0xffffffff)
    {
        EQ_METHOD(6, void, 0x0048ED3B, (void*, EQPlayer*, int, int, int, float, int), g_display_guess)(g_local_player_guess, (u8)anim, 1, 0, 1.0, 1);
    }
    if (message[0] != 0)
    {
        len = (short)strlen(message);
        if (EQ_CDECL(int, 0x00522ECF, (int))(message[len - 1]) != 0 ||
            EQ_CDECL(int, 0x00522F25, (int))(message[len - 1]) != 0)
        {
            strcat(message, EQ_ARRAY(char, 0x00545998));
        }
        len = (short)strlen(message);
        message[len] = 0;
        chat = message;
    printChat:
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, char*), self)(chat);
    }
}

// FUNCTION: TRILOGY 0x004ce8b9
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::IHaveTicket, low)
// __thiscall with one stack argument (`ret 4`), both unused; returns 1 in AX.
short CEverQuest::IHaveTicket_guess(int unused)
{
    return 1;
}

struct SavePCItemCount_guess
{
    int slot0;
    int slot1;
    int count;
    char pad[0x50];
};

struct SavePCProfile_guess
{
    u32 crc;
    char profile[0x1fa4];
};

// FUNCTION: TRILOGY 0x004ce8c0
// Saves the character to the server: position, inventory/bag/bank item lists, then the checksummed profile.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, two stack arguments (`ret 8`): `announce` prints "<name> saved." and `useLocation` copies the
// local player's position into the profile (and skips RefitNewbieEQ; it is then reused for the item check).
// Rebuilds the profile's item tables from the live item pointers, sends the item-count message 0x2147 when
// any slot holds an item below 1000 (the slot group number 1..4) and the checksummed profile (0x202E).
void CEverQuest::SavePC_guess(int announce, int useLocation)
{
    int i;
    int j;
    EQ_Item* item;
    char buf[128];
    SavePCItemCount_guess countMsg;
    SavePCProfile_guess packet;

    if (g_pc_guess == NULL)
    {
        EQTRACE_guess("LocalPC is NULL in SavePC()\n");
        return;
    }
    if (g_local_player_guess == NULL)
        return;
    if (g_local_player_guess->type != 0)
        return;
    if (g_display_guess == 0)
        return;
    if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() == 0)
        return;
    if (g_offlineMode_guess != 0)
        return;
    if (DAT_006b6494 != 0)
        return;
    if ((char)g_pc_guess->unknown0x0076 >= 4)
        return;
    if (g_pc_guess->cur_hp < 1)
        return;
    if (pinstCEverQuest_guess->gameState_guess != 5)
        return;
    if (EQ_GLOBAL(int, 0x006B63E8) == 0)
        return;
    if (EQ_GLOBAL(int, 0x006B70E0) == 0)
        return;
    if (g_eqmain_guess != NULL &&
        (pinstEQ_Main_guess->pendingServerReplies_guess != 0 || pinstEQ_Main_guess->pendingWorldRequests_guess != 0))
    {
        EQ_GLOBAL(u8, 0x006E11E0) = 0x65;
        return;
    }
    EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
    if (g_eqmain_guess != NULL)
        EQ_METHOD(0, void, 0x004567F8, (void*), g_eqmain_guess)();
    if (EQ_GLOBAL(u8, 0x006E1DE5) == 0)
    {
        if (useLocation != 0 && g_local_player_guess->type == 0)
        {
            g_pc_guess->y = g_local_player_guess->y;
            g_pc_guess->x = g_local_player_guess->x;
            g_pc_guess->z = g_local_player_guess->z;
            g_pc_guess->heading = g_local_player_guess->heading;
        }
        if (strlen(g_pc_guess->boat) > 0)
        {
            char* shipName = EQ_METHOD(1, char*, 0x004D024E, (void*, char*), this)(g_pc_guess->boat);
            if (EQ_METHOD(1, short, 0x004CE8B9, (void*, char*), this)(shipName) >= 0)
            {
                EQPlayer* vehicle = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, char*), EQ_GLOBAL(void*, 0x006E4D58))(
                     EQ_METHOD(1, char*, 0x004D024E, (void*, char*), this)(g_pc_guess->boat));
                if (vehicle != NULL)
                {
                    EQ_CDECL(void, 0x004E0FC7, (EQPlayer*))(vehicle);
                    g_pc_guess->y = g_local_player_guess->y;
                    g_pc_guess->x = g_local_player_guess->x;
                    g_pc_guess->z = g_local_player_guess->z;
                    g_pc_guess->heading = g_local_player_guess->heading;
                    EQ_CDECL(void, 0x004E1109, (EQPlayer*))(vehicle);
                }
                else
                {
                    EQTRACE_guess("Unable to find EQPlayer for vehicle called %s (in SavePC())\n",
                        g_pc_guess->boat);
                    g_pc_guess->boat[0] = '\0';
                }
            }
            else
            {
                EQTRACE_guess("No ticket found for the %s\n", g_pc_guess->boat);
                g_pc_guess->boat[0] = '\0';
            }
        }
    }
    g_pc_guess->primary_model = 0;
    g_pc_guess->secondary_model = 0;
    if (g_pc_guess->inventory_items[13] != NULL)
    {
        strcpy(buf, g_pc_guess->inventory_items[13]->idfile);
        strcpy(buf, buf + 2);
        if (is_number(buf) != 0)
            g_pc_guess->primary_model = atoi(buf);
    }
    if (g_pc_guess->inventory_items[14] != NULL)
    {
        strncpy(buf, g_pc_guess->inventory_items[14]->idfile, 0xf);
        strcpy(buf, buf + 2);
        if (is_number(buf) != 0)
            g_pc_guess->secondary_model = atoi(buf);
    }

    for (i = 0; i < 30; i++)
    {
        g_pc_guess->inventory[i] = -1;
        g_pc_guess->inv_props[i].unknown0x00 = 0;
        g_pc_guess->inv_props[i].charges = 0;
    }
    for (i = 0; i < 30; i++)
    {
        if (g_pc_guess->inventory_items[i] != NULL && g_pc_guess->inventory_items[i]->item_nr >= 1000)
        {
            g_pc_guess->inventory[i] = g_pc_guess->inventory_items[i]->item_nr;
            if (g_pc_guess->inventory_items[i]->type == 0)
                memcpy(&g_pc_guess->inv_props[i], (char*)g_pc_guess->inventory_items[i] + 0xd8, 10);
        }
    }

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 10; j++)
        {
            g_pc_guess->bag_inventory[i * 10 + j] = -1;
            g_pc_guess->bag_props[i * 10 + j].unknown0x00 = 0;
            g_pc_guess->bag_props[i * 10 + j].charges = 0;
        }
    }
    item = g_pc_guess->inventory_items[0];
    if (item != NULL && item->type == 1)
    {
        for (i = 0; i < *((u8*)item + 0xd5); i++)
        {
            EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
            if (content != NULL && content->item_nr >= 1000)
            {
                g_pc_guess->cursor_bag_inventory[i] = content->item_nr;
                content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content->type == 0)
                    memcpy(&g_pc_guess->cursor_bag_props[i], (char*)content + 0xd8, 10);
            }
        }
    }
    for (j = 22; j < 30; j++)
    {
        item = g_pc_guess->inventory_items[j];
        if (item != NULL && item->type == 1 && item != NULL)
        {
            for (i = 0; i < *((u8*)item + 0xd5); i++)
            {
                EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content != NULL && content->item_nr >= 1000)
                {
                    g_pc_guess->bag_inventory[(j - 22) * 10 + i] = content->item_nr;
                    content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                    if (content->type == 0)
                        memcpy(&g_pc_guess->bag_props[(j - 22) * 10 + i], (char*)content + 0xd8, 10);
                }
            }
        }
    }

    for (i = 0; i < 8; i++)
    {
        g_pc_guess->bank_inventory[i] = -1;
        g_pc_guess->bank_props[i].unknown0x00 = 0;
        g_pc_guess->bank_props[i].charges = 0;
        for (j = 0; j < 10; j++)
        {
            g_pc_guess->bank_bag_inventory[i * 10 + j] = -1;
            g_pc_guess->bank_bag_props[i * 10 + j].unknown0x00 = 0;
            g_pc_guess->bank_bag_props[i * 10 + j].charges = 0;
        }
    }
    for (i = 0; i < 8; i++)
    {
        if (g_pc_guess->bank_items[i] != NULL && g_pc_guess->bank_items[i]->item_nr >= 1000)
        {
            g_pc_guess->bank_inventory[i] = g_pc_guess->bank_items[i]->item_nr;
            if (g_pc_guess->bank_items[i]->type == 0)
                memcpy(&g_pc_guess->bank_props[i], (char*)g_pc_guess->bank_items[i] + 0xd8, 10);
        }
    }
    for (j = 0; j < 8; j++)
    {
        item = g_pc_guess->bank_items[j];
        if (item != NULL && item->type == 1 && item != NULL)
        {
            for (i = 0; i < *((u8*)item + 0xd5); i++)
            {
                EQ_Item* content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                if (content != NULL && content->item_nr >= 1000)
                {
                    g_pc_guess->bank_bag_inventory[j * 10 + i] = content->item_nr;
                    content = *(EQ_Item**)((char*)item + 0xac + i * 4);
                    if (content->type == 0)
                        memcpy(&g_pc_guess->bank_bag_props[j * 10 + i], (char*)content + 0xd8, 10);
                }
            }
        }
    }

    for (i = 0; i < 9; i++)
    {
        if (g_local_player_guess != NULL)
        {
            g_pc_guess->equipment[i] = g_local_player_guess->equipment[i];
            g_pc_guess->equip_colors[i] = g_local_player_guess->equip_colors[i];
        }
    }
    if (useLocation == 0)
        EQ_METHOD(0, void, 0x004806CB, (void*), g_pc_guess)();

    useLocation = 0;
    for (i = 0; i < 30; i++)
    {
        if (g_pc_guess->inventory[i] >= 0 && g_pc_guess->inventory[i] < 1000)
            useLocation = 1;
    }
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (g_pc_guess->bag_inventory[i * 10 + j] >= 0 && g_pc_guess->bag_inventory[i * 10 + j] < 1000)
                useLocation = 2;
        }
    }
    for (i = 0; i < 8; i++)
    {
        if (g_pc_guess->bank_inventory[i] >= 0 && g_pc_guess->bank_inventory[i] < 1000)
            useLocation = 3;
        for (j = 0; j < 10; j++)
        {
            if (g_pc_guess->bank_bag_inventory[i * 10 + j] >= 0 && g_pc_guess->bank_bag_inventory[i * 10 + j] < 1000)
                useLocation = 4;
        }
    }
    g_pc_guess->gm_int_guess = 0;
    if (g_local_player_guess->gm != 0 || g_pc_guess->gm_flag != 0)
        g_pc_guess->gm_int_guess = 1;
    if (useLocation != 0)
    {
        countMsg.slot0 = -1;
        countMsg.slot1 = -1;
        countMsg.count = useLocation;
        send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2147, &countMsg, 0x5c, 1);
    }
    memcpy(packet.profile, g_pc_guess, 0x1fa4);
    packet.crc = EQ_CDECL(u32, 0x004E04A2, (void*, s32))(packet.profile, 0x1fa4);
    EQ_GLOBAL(int, 0x006B64E8) = 1;
    send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x202e, &packet, 0x1fa8, 1);
    EQ_GLOBAL(u8, 0x006E11E0) = 0;
    if (announce != 0)
    {
        sprintf(buf, EQ_ARRAY(char, 0x0056F14C), g_pc_guess);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(buf);
    }
}

// FUNCTION: TRILOGY 0x004cf180
// Sends a private tell to a player, or warns locally if the recipient is you.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void __fastcall CEverQuest_send_tell_guess_impl(CEverQuest* game, int unusedEdx, char* recipient, char* text)
{
    char recipientLookup[24];
    char chatLine[256];
    char speakerName[36];

    if (EQ_GLOBAL(char, 0x006B63F8) == 0 &&
        DAT_006b6494 == 0 &&
        EQ_GLOBAL(char, 0x006B6465) == 0)
    {
        strcpy(speakerName, (char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C) + 1);
        strcpy(recipientLookup, recipient);
        sprintf(chatLine, EQ_ARRAY(char, 0x0056F194),
             (char*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C) + 1, text);
        if (EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(recipientLookup) ==
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), game)(EQ_ARRAY(char, 0x0056F178));
        }
        else
        {
            EQ_METHOD(3, void, 0x004D0D77, (CEverQuest*, int, char*, char*), game)(7, recipient, text);
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004cf226
// Submits the character's typed petition to the game world.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void __fastcall CEverQuest_send_petition_guess_impl(CEverQuest* game, int unusedEdx)
{
    char message[0x840];
    char* petition;

    if (EQ_GLOBAL(int, 0x006E4E70) == 0 &&
        EQ_GLOBAL(int, 0x006E4E6C) == 0 &&
        EQ_GLOBAL(char, 0x006B63F8) == 0 &&
        DAT_006b6494 == 0)
    {
        petition = (char*)game + 0x0BA8;
        EQ_CDECL(void, 0x004A07FD, (char*))(petition);
        sprintf(message, EQ_ARRAY(char, 0x0056F1A8), petition);
        EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), game)(message, 0);
    }
    return;
}

class CEverQuest_send_gmsay_guess_class : public CEverQuest
{
public:
    void send_gmsay_guess(void);
};

// FUNCTION: TRILOGY 0x004cf28c
// Sends a GM say and echoes it locally.
// The ECX game object is the sender. When the five global gates are clear, send the text at 0xBA8 as a GM say and echo it.
void CEverQuest_send_gmsay_guess_class::send_gmsay_guess(void)
{
    char message[0x840];
    char* petition;

    if (EQ_GLOBAL(char, 0x006B752A) == 0 &&
        EQ_GLOBAL(int, 0x006E4E70) == 0 &&
        EQ_GLOBAL(int, 0x006E4E6C) == 0 &&
        EQ_GLOBAL(char, 0x006B63F8) == 0 &&
        DAT_006b6494 == 0)
    {
        typedef void (CEverQuest_send_gmsay_guess_class::*SendNewTextFunction_guess)
            (int, int, const char*);
        union SendNewTextFunctionAddress_guess
        {
            void (__cdecl *function)(void);
            SendNewTextFunction_guess method;
        };
        typedef void (CEverQuest_send_gmsay_guess_class::*DspChatFunction_guess)
            (const char*, int);
        union DspChatFunctionAddress_guess
        {
            void (__cdecl *function)(void);
            DspChatFunction_guess method;
        };
        SendNewTextFunctionAddress_guess sendNewText;
        DspChatFunctionAddress_guess dspChat;
        sendNewText.function = EQ_FUNC(void, __cdecl, 0x004D0D77, (void));
        dspChat.function = EQ_FUNC(void, __cdecl, 0x004D055E, (void));

        petition = (char*)this + 0x0BA8;
        sprintf(message, EQ_ARRAY(char, 0x0056F1CC), (char*)g_local_player_guess + 1, petition);
        (this->*sendNewText.method)(11, 0, petition);
        sprintf(message, EQ_ARRAY(char, 0x0056F1BC), petition);
        (this->*dspChat.method)(message, 0);
    }
    return;
}

// FUNCTION: TRILOGY 0x004cf31a
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The ECX game object is unused. Copy the two option blocks, set their related flags, then write the profile.
void CEverQuest::saveOptions_guess(void)
{
    char fileName[128];
    FILE* file;

    if (*(u8*)0x006b7111 == 0 &&
        *(u32*)0x006b63e8 != 0 &&
        *(u8*)0x006b7110 == 0)
    {
        strcpy(fileName, (char*)0x0056f1dc);
        strcat(fileName, (char*)0x00545990);
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x005588dc);
        if (file == 0)
        {
            EQTRACE_guess((char*)0x0055a964);
        }
        else
        {
            memcpy((void*)0x006b69ac, (void*)0x006b6f6c, 0x1c);
            memcpy((void*)0x006b69c8, (void*)0x006b6f88, 0x1c);
            EQ_GLOBAL(int, 0x006B68BC) = -0x2a;
            EQ_GLOBAL(int, 0x006B687C) = -0x2a;
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
            EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                ((void*)0x006b685c, 0x688, 1, file);
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        }
    }
}


// FUNCTION: TRILOGY 0x004cf3e1
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The ECX game object is unused. Save the fixed option block only when the three global gates permit it.
void CEverQuest::saveOptions2_guess(void)
{
    char fileName[128];
    FILE* file;

    if (*(u8*)0x006b7111 == 0 &&
        *(u32*)0x006b63e8 != 0 &&
        *(u8*)0x006b7110 == 0)
    {
        strcpy(fileName, (char*)0x0056f210);
        strcat(fileName, (char*)0x00545990);
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x005588dc);
        if (file == 0)
        {
            EQTRACE_guess((char*)0x0056f1e8);
        }
        else
        {
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
            EQ_CDECL(size_t, 0x0052359E, (const void*, size_t, size_t, FILE*))
                ((void*)0x006b6ee4, 0x34, 1, file);
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        }
    }
}

// FUNCTION: TRILOGY 0x004cf46a
// __thiscall with one stack argument; the game object is only forwarded to dsp_chat_guess.
void __fastcall ProcessSummonedItem_guess(void* self, int unusedEdx, char* summonName)
{
    u8 itemKind;
    EQ_Item* pEVar2;
    short payloadSize;
    struct
    {
        char local_848[32];
        char local_828[32];
        u32 local_808;
        char local_804[2048];
    } local;

    if (g_pc_guess->inventory_items[0] != (EQ_Item*)0)
    {
        if (g_pc_guess->inventory_items[0]->type == '\x01')
        {
            EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), (CEverQuest*)self)((const char*)0x0056f21c, 4);
        }
        else
        {
            strcpy(local.local_848, (char*)g_pc_guess);
            strcpy(local.local_828, summonName);
            local.local_808 = (u32)g_pc_guess->inventory_items[0]->type;
            pEVar2 = g_pc_guess->inventory_items[0];
            itemKind = pEVar2->type;
            switch (itemKind)
            {
            case 0:
                payloadSize = 0x124;
                break;
            case 2:
                payloadSize = 0xd0;
                break;
            default:
                return;
            }
            memcpy(local.local_804, pEVar2, payloadSize);
            payloadSize += 0x48;
            send_message_guess(g_world_connection_guess, 0x203e, local.local_848, (int)payloadSize, 1);
            EQ_CDECL(void, 0x005218BB, (void*))(g_pc_guess->inventory_items[0]);
            g_pc_guess->inventory_items[0] = (EQ_Item*)0;
            g_pc_guess->inventory[0] = -1;
            *(u8*)(g_displayManager_guess + 0x3c) = 0;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004cf561
// Checks worn/carried items, bag contents and coin against a stored copy; traces the last mismatch code.
// @early-stop: VC6 still strength-reduces the inner bag lookup into a pointer walk, unlike the original indexed loop.
int CheckCharacterProfileCorruption_guess(void)
{
    int savedIndex;
    int capacity;
    int bagSlot;
    short *savedBagItem;
    short bagIndex;
    EQ_Item *pEVar6;
    int cVar7;
    int invSlot;
    cVar7 = 0;
    if (g_pc_guess == (CharacterProfile *)0)
    {
        cVar7 = 1;
    }
    invSlot = 0;
    do
    {
        pEVar6 = g_pc_guess->inventory_items[invSlot];
        if (pEVar6 == (EQ_Item *)0)
        {
            if (EQ_ARRAY(short, 0x006B6FC0)[invSlot] != -1)
            {
                cVar7 = '\x02';
            }
        }
        else
        {
            if (pEVar6->item_nr != EQ_ARRAY(short, 0x006B6FC0)[invSlot])
            {
                cVar7 = '\x03';
            }
            if (pEVar6->type == '\x01')
            {
                if (invSlot == 0)
                {
                    bagIndex = 8;
                }
                else
                {
                    bagIndex = (short)invSlot + -0x16;
                }
                capacity = (unsigned char)pEVar6[1].lore[6];
                bagSlot = 0;
                if (capacity > 0)
                {
                    pEVar6 = pEVar6 + 1;
                    do
                    {
                        savedIndex = bagSlot + bagIndex * 10;
                        if (*(int *)pEVar6->name == 0)
                        {
                            if (*(short *)((int)EQ_ARRAY(short, 0x006B6FFC) + savedIndex * 2) != -1)
                            {
                                cVar7 = '\x04';
                            }
                        }
                        else if (*(short *)(*(int *)pEVar6->name + 0x82) !=
                                    *(short *)((int)EQ_ARRAY(short, 0x006B6FFC) + savedIndex * 2))
                        {
                            cVar7 = '\x05';
                        }
                        bagSlot = bagSlot + 1;
                        pEVar6 = (EQ_Item *)(pEVar6->name + 4);
                    } while (bagSlot < capacity);
                }
                if (capacity < 10)
                {
                    savedBagItem = EQ_ARRAY(short, 0x006B6FFC) + (capacity + bagIndex * 10);
                    bagSlot = 10 - capacity;
                    do
                    {
                        if (*savedBagItem != -1)
                        {
                            cVar7 = '\x06';
                        }
                        savedBagItem = savedBagItem + 1;
                        bagSlot = bagSlot + -1;
                    } while (bagSlot != 0);
                }
            }
        }
        invSlot = invSlot + 1;
    } while (invSlot < 0x1e);
    if (g_pc_guess->platinum_bank != EQ_GLOBAL(int, 0x006B70B0))
    {
        cVar7 = '\a';
    }
    if (g_pc_guess->gold_bank != EQ_GLOBAL(int, 0x006B70B4))
    {
        cVar7 = '\b';
    }
    if (g_pc_guess->silver_bank != EQ_GLOBAL(int, 0x006B70B8))
    {
        cVar7 = '\t';
    }
    if (g_pc_guess->copper_bank != EQ_GLOBAL(int, 0x006B70BC))
    {
        cVar7 = '\n';
    }
    if (g_pc_guess->platinum != EQ_GLOBAL(int, 0x006B70C0))
    {
        cVar7 = '\v';
    }
    if (g_pc_guess->gold != EQ_GLOBAL(int, 0x006B70C4))
    {
        cVar7 = '\f';
    }
    if (g_pc_guess->silver != EQ_GLOBAL(int, 0x006B70C8))
    {
        cVar7 = '\r';
    }
    if (g_pc_guess->copper != EQ_GLOBAL(int, 0x006B70CC))
    {
        cVar7 = '\x0e';
    }
    if (g_pc_guess->platinum_cursor != EQ_GLOBAL(int, 0x006B70D0))
    {
        cVar7 = '\x0f';
    }
    if (g_pc_guess->gold_cursor != EQ_GLOBAL(int, 0x006B70D4))
    {
        cVar7 = '\x10';
    }
    if (g_pc_guess->silver_cursor != EQ_GLOBAL(int, 0x006B70D8))
    {
        cVar7 = '\x11';
    }
    if (g_pc_guess->copper_cursor != EQ_GLOBAL(int, 0x006B70DC))
    {
        cVar7 = '\x12';
    }
    if (cVar7 != '\0')
    {
        EQTRACE_guess((char *)0x0056f240, cVar7);
        return false;
    }
    return true;
}

// FUNCTION: TRILOGY 0x004cf7a1
// Moves the chat input caret and keeps the selection within the entered text.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address; TAKP name CEverQuest_ui_something, low)
// __thiscall, `this` unused (it uses pinstCEverQuest). Sets the chat input's cursor (+0x16C of the object at
// 0x006E4DE8) to `pos` clamped to the length of the chat buffer (CEverQuest + 0xBA8), 0 for pos <= 0; the
// selection end (+0x170) follows the cursor up and is clamped to the length too. Always marks the chat line
// changed (CEverQuest + 0x13EC).
void CEverQuest::SetChatCursor_guess(int pos)
{
    {
        u32 len;
    
        if (g_chatWindow_guess != NULL)
        {
            len = strlen((char*)pinstCEverQuest_guess + 0xBA8);
            if (!(pos > 0))
            {
                *(u32*)(g_chatWindow_guess + 0x16C) = 0;
            }
            else
            {
                *(u32*)(g_chatWindow_guess + 0x16C) = pos;
                if (*(u32*)(g_chatWindow_guess + 0x16C) > len)
                    *(u32*)(g_chatWindow_guess + 0x16C) = len;
                if (*(u32*)(g_chatWindow_guess + 0x16C) > *(u32*)(g_chatWindow_guess + 0x170))
                    *(u32*)(g_chatWindow_guess + 0x170) = *(u32*)(g_chatWindow_guess + 0x16C);
            }
            if (*(u32*)(g_chatWindow_guess + 0x170) > len)
                *(u32*)(g_chatWindow_guess + 0x170) = len;
        }
    }
    *((char*)pinstCEverQuest_guess + 0x13EC) = 1;
}

// FUNCTION: TRILOGY 0x004cf829
// Sends the player's chat filter settings to the world server.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::send_update_filters, low)
// __thiscall (callers load ECX from pinstCEverQuest), `this` unused, no arguments. Sends the 15 chat filter
// settings (0x3C bytes) as opcode 0x21FF.
void CEverQuest::send_update_filters_guess(void)
{
    u32 filters[15];

    filters[5] = g_filter_6984_guess[0];
    filters[6] = g_filter_6984_guess[1];
    filters[7] = g_filter_6984_guess[2];
    filters[8] = g_filter_6984_guess[3];
    filters[9] = g_filter_6984_guess[4];
    filters[10] = g_filter_6984_guess[5];
    filters[11] = g_filter_6984_guess[6];
    filters[12] = g_filter_6984_guess[7];
    filters[13] = g_filter_6984_guess[8];
    filters[14] = g_filter_6984_guess[9];
    filters[2] = g_filterValue2_guess;
    filters[3] = g_filterValue3_guess;
    filters[1] = g_filterValue1_guess;
    filters[0] = g_filterValue0_guess;
    filters[4] = g_filterValue4_guess;
    send_message_guess(g_world_connection_guess, 0x21FF, filters, sizeof(filters), 1);
}

// FUNCTION: TRILOGY 0x004cf8c3
// Startup load of key bindings and the eqOptions1/eqOptions2 files, writing defaults when they are missing.
// @early-stop: 0x40af61 is a thiscall with one stack argument, so its dummy EDX (xor edx,edx) stays; the 0x6b689c
// if-store is kept in eax and the copy to 0x6b641c reuses it (no reload as in the original), whatever the global's type.
// __thiscall in the original: ECX is the CEverQuest object (kept for the save calls), no arguments, no return value.
// Loads the key maps, then eqOptions1 (defaults
// are written and saved when it won't open; a second failure traces and returns), then eqOptions2 (defaults when it
// won't open). Clamps the loaded values, writes VideoMode to eqclient.ini and copies the camera blocks.
void CEverQuest::loadOptions_guess(void)
{
    typedef int (__stdcall *WritePrivateProfileStringAProc_guess)(const char*, const char*, const char*, const char*);
    char fileName[128];
    char videoMode[16];
    int remaining;
    int* keyMapPtr;
    int mappedKey;
    int tmp;
    int* optionPtr;
    u8* entryPtr;

    EQ_METHOD(0, void, 0x004EAA05, (void*), this)();
    tmp = 1;
    keyMapPtr = (int*)0x006e15e8;
    remaining = 0x61;
    do
    {
        mappedKey = EQ_CDECL(int, 0x004C2109, (int, int))(tmp, 0);
        if (mappedKey != -2)
        {
            keyMapPtr[-0x100] = mappedKey;
        }
        mappedKey = EQ_CDECL(int, 0x004C2109, (int, int))(tmp, 1);
        if (mappedKey != -2)
        {
            *keyMapPtr = mappedKey;
        }
        tmp = tmp + 1;
        keyMapPtr = keyMapPtr + 1;
        remaining = remaining - 1;
    } while (remaining != 0);
    strcpy(fileName, (const char*)0x0056f2a4);
    strcat(fileName, (const char*)0x00545990);
    EQ_CDECL(void, 0x00523548, (char*))(fileName);
    strcpy(fileName, (const char*)0x0056f1dc);
    strcat(fileName, (const char*)0x00545990);
    tmp = EQ_CDECL(int, 0x005223BC, (char*, const char*))(fileName, (const char*)0x0053c2bc);
    if (tmp == 0)
    {
        EQ_METHOD(0, void, 0x00408D5B, (void*), g_display_guess)();
        memcpy((void*)0x006b69ac, (const void*)0x006b6f6c, 0x1c);
        memcpy((void*)0x006b69c8, (const void*)0x006b6f88, 0x1c);
        if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && g_sound_manager_ptr_guess != 0)
        {
            EQ_METHOD(0, void, 0x004E31B1, (void*), g_sound_manager_ptr_guess)();
        }
        EQ_GLOBAL(int, 0x006B687C) = -1;
        EQ_GLOBAL(float, 0x006B685C) = 1.0f;
        EQ_GLOBAL(int, 0x006B6898) = 0;
        EQ_GLOBAL(int, 0x006B6884) = 1;
        EQ_GLOBAL(int, 0x006B6888) = 1;
        EQ_GLOBAL(int, 0x006B6880) = 1;
        EQ_GLOBAL(int, 0x006B6878) = 1;
        EQ_GLOBAL(int, 0x006B686C) = 1;
        EQ_GLOBAL(int, 0x006B6860) = 0;
        EQ_GLOBAL(int, 0x006B6864) = 0;
        EQ_GLOBAL(int, 0x006B6868) = 2;
        EQ_GLOBAL(int, 0x006B6870) = 1;
        EQ_GLOBAL(int, 0x006B6874) = 3;
        EQ_GLOBAL(int, 0x006B689C) = 4;
        EQ_GLOBAL(int, 0x006B68A0) = 0;
        EQ_GLOBAL(int, 0x006B68A4) = 0;
        EQ_GLOBAL(int, 0x006B68A8) = 1;
        EQ_GLOBAL(int, 0x006B68AC) = 0;
        EQ_GLOBAL(int, 0x006B68B0) = 0;
        EQ_GLOBAL(int, 0x006B68B4) = 0;
        EQ_GLOBAL(int, 0x006B68B8) = 0;
        optionPtr = (int*)0x006b6984;
        for (tmp = 6; tmp != 0; tmp = tmp - 1)
        {
            *optionPtr = 1;
            optionPtr = optionPtr + 1;
        }
        EQ_GLOBAL(int, 0x006B699C) = 1;
        EQ_GLOBAL(int, 0x006B69A0) = 0;
        EQ_GLOBAL(int, 0x006B69A4) = 1;
        EQ_GLOBAL(int, 0x006B69A8) = 1;
        entryPtr = (u8*)0x006b69e4;
        tmp = 0x14;
        do
        {
            entryPtr[0x280] = 0;
            *entryPtr = 0;
            entryPtr = entryPtr + 0x20;
            tmp = tmp - 1;
        } while (tmp != 0);
        EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
        tmp = EQ_CDECL(int, 0x005223BC, (char*, const char*))(fileName, (const char*)0x0053c2bc);
        if (tmp == 0)
        {
            EQTRACE_guess((const char*)0x0056f25c);
            return;
        }
    }
    EQ_CDECL(int, 0x005222D2, (int, long, int))(tmp, 0, 0);
    EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, int))((void*)0x006b685c, 0x688, 1, tmp);
    EQ_CDECL(int, 0x00521FBB, (int))(tmp);
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && g_sound_manager_ptr_guess != 0)
    {
        EQ_METHOD(0, void, 0x004E3297, (void*), g_sound_manager_ptr_guess)();
    }
    tmp = EQ_GLOBAL(int, 0x006B687C);
    if (tmp != -0x2a)
    {
        EQ_GLOBAL(int, 0x006E20C0) = tmp;
        EQ_GLOBAL(int, 0x006B687C) = -0x2a;
        sprintf(videoMode, (const char*)0x0053e16c, tmp);
        EQ_GLOBAL(WritePrivateProfileStringAProc_guess, 0x00534188)
            ((const char*)0x0053db3c, (const char*)0x0055b23c, videoMode, (const char*)0x0053db50);
        EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
    }
    if (EQ_GLOBAL(float, 0x006B685C) < EQ_GLOBAL(float, 0x0053510C) || EQ_GLOBAL(float, 0x006B685C) > EQ_GLOBAL(float, 0x00535120))
    {
        EQ_GLOBAL(float, 0x006B685C) = 1.0f;
    }
    if (EQ_GLOBAL(int, 0x006B6870) < 0 || EQ_GLOBAL(int, 0x006B6870) > 2)
    {
        EQ_GLOBAL(int, 0x006B6870) = 1;
    }
    if (EQ_GLOBAL(int, 0x006B6874) < 0 || EQ_GLOBAL(int, 0x006B6874) > 3)
    {
        EQ_GLOBAL(int, 0x006B6874) = 3;
    }
    if (EQ_GLOBAL(int, 0x006B689C) < 1 || EQ_GLOBAL(int, 0x006B689C) > 8)
    {
        EQ_GLOBAL(int, 0x006B689C) = 4;
    }
    EQ_GLOBAL(short, 0x006B641C) = EQ_GLOBAL(int, 0x006B689C);
    EQ_GLOBAL(u8, 0x006B6458) = EQ_GLOBAL(u8, 0x006B68A0);
    EQ_CDECL(void, 0x004AC585, (int))(EQ_GLOBAL(int, 0x006B6878));
    EQ_METHOD(1, void, 0x0040AF61, (void*, int), g_display_guess)(EQ_GLOBAL(int, 0x006B6874));
    if (EQ_GLOBAL(float, 0x006B69C0) <= EQ_GLOBAL(float, 0x0053510C) || EQ_GLOBAL(float, 0x006B69C0) >= EQ_GLOBAL(float, 0x005352F0) ||
        EQ_GLOBAL(float, 0x006B69DC) <= EQ_GLOBAL(float, 0x0053510C) || EQ_GLOBAL(float, 0x006B69DC) >= EQ_GLOBAL(float, 0x005352F0))
    {
        EQ_METHOD(0, void, 0x00408D5B, (void*), g_display_guess)();
        memcpy((void*)0x006b69ac, (const void*)0x006b6f6c, 0x1c);
        memcpy((void*)0x006b69c8, (const void*)0x006b6f88, 0x1c);
        EQ_METHOD(0, void, 0x004CF31A, (void*), this)();
    }
    else
    {
        memcpy((void*)0x006b6f6c, (const void*)0x006b69ac, 0x1c);
        memcpy((void*)0x006b6f88, (const void*)0x006b69c8, 0x1c);
    }
    EQ_GLOBAL(u8, 0x006B6409) = (u8)EQ_GLOBAL(int, 0x006B68A8);
    strcpy(fileName, (const char*)0x0056f210);
    strcat(fileName, (const char*)0x00545990);
    tmp = EQ_CDECL(int, 0x005223BC, (char*, const char*))(fileName, (const char*)0x0053c2bc);
    if (tmp == 0)
    {
        EQ_GLOBAL(int, 0x006B6EE4) = 0;
        EQ_GLOBAL(int, 0x006B6EE8) = 0;
        EQ_GLOBAL(int, 0x006B6EEC) = 0;
        EQ_GLOBAL(float, 0x006B6EF0) = 1.0f;
        EQ_GLOBAL(int, 0x006B6EF4) = 0;
        EQ_GLOBAL(int, 0x006B6EF8) = 0;
        EQ_GLOBAL(int, 0x006B6EFC) = 0;
        EQ_GLOBAL(int, 0x006B6F00) = 0;
        EQ_GLOBAL(int, 0x006B6F04) = 0;
        g_channels_reversed_guess = 0;
        EQ_GLOBAL(int, 0x006B6F0C) = 0;
        EQ_GLOBAL(int, 0x006B6F10) = 0;
        EQ_GLOBAL(int, 0x006B6F14) = 0;
        EQ_METHOD(0, void, 0x004CF3E1, (void*), this)();
    }
    else
    {
        EQ_CDECL(int, 0x005222D2, (int, long, int))(tmp, 0, 0);
        EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, int))((void*)0x006b6ee4, 0x34, 1, tmp);
        EQ_CDECL(int, 0x00521FBB, (int))(tmp);
        if (EQ_GLOBAL(float, 0x006B6EF0) == EQ_GLOBAL(float, 0x0053510C))
        {
            EQ_GLOBAL(int, 0x006B68B8) = 5;
            EQ_GLOBAL(float, 0x006B6EF0) = 1.0f;
        }
    }
}

// FUNCTION: TRILOGY 0x004cfd5f
// Requests that the group split the specified coin amounts.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::DoSplit, high)
// @early-stop: copper _atol cleanup pop lands after its store (original: before); sum adds platinum before gold.
// __stdcall, one stack argument, callee pops 4. The original is a CEverQuest member whose `this` (ECX) is never
// read; everquest.h has no DoSplit member, so this is a free stand-in with the same code (add
// `void DoSplit(int value);` to CEverQuest to make it one). Argument: "<platinum> <gold> <silver> <copper>".
// A sum below 1 is refused with three chat lines. Each coin below 1 counts as 0; a coin above what the profile
// holds is refused with its chat line. Otherwise the two coin hooks run, the coins leave the profile and go out
// as opcode 0x2131 (16 bytes, reliable).
void __stdcall DoSplit(int value)
{
    char silverWord[32];
    char goldWord[32];
    char platinumWord[32];
    MsgSplit packet;
    char* remainingText;
    int platinumAmount;
    int silverAmount;
    int copperAmount;

    if (g_name_buffer_guess[0] == '\0')
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F3E8));
        return;
    }
    if (0 < pinstEQ_Main_guess->pendingWorldRequests_guess)
        return;
    remainingText = EQ_METHOD(2, char *, 0x004DBC84, (CEverQuest *, char *, char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))((char*)value, platinumWord);
    platinumAmount = atol(platinumWord);
    remainingText = EQ_METHOD(2, char *, 0x004DBC84, (CEverQuest *, char *, char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(remainingText, goldWord);
    value = atol(goldWord);
    remainingText = EQ_METHOD(2, char *, 0x004DBC84, (CEverQuest *, char *, char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(remainingText, silverWord);
    silverAmount = atol(silverWord);
    copperAmount = atol(remainingText);
    if (copperAmount + silverAmount + value + platinumAmount < 1)
    {
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F3B8));
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F384));
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F33C));
        return;
    }
    if (platinumAmount > 0)
    {
        if (platinumAmount > g_pc_guess->platinum)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F318));
            return;
        }
    }
    else
        platinumAmount = 0;
    if (value > 0)
    {
        if (value > g_pc_guess->gold)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F2F8));
            return;
        }
    }
    else
        value = 0;
    if (silverAmount > 0)
    {
        if (silverAmount > g_pc_guess->silver)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F2D4));
            return;
        }
    }
    else
        silverAmount = 0;
    if (copperAmount > 0)
    {
        if (copperAmount > g_pc_guess->copper)
        {
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest *, const char *), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_ARRAY(char, 0x0056F2B0));
            return;
        }
    }
    else
        copperAmount = 0;
    if (EQ_GLOBAL(int*, 0x006018C0) != 0 && EQ_GLOBAL(int*, 0x006018C0)[4] != 0)
        ((void (__fastcall *)(void*))(*(void***)EQ_GLOBAL(int*, 0x006018C0))[2])(EQ_GLOBAL(int*, 0x006018C0));
    if (EQ_GLOBAL(int*, 0x00604984) != 0 && EQ_GLOBAL(int*, 0x00604984)[4] != 0)
    {
        EQ_GLOBAL(int*, 0x00604984)[0x7a] = 1;
        ((void (__fastcall *)(void*))(*(void***)EQ_GLOBAL(int*, 0x00604984))[2])(EQ_GLOBAL(int*, 0x00604984));
    }
    packet.copper = copperAmount;
    g_pc_guess->copper -= copperAmount;
    packet.silver = silverAmount;
    g_pc_guess->silver -= silverAmount;
    packet.gold = value;
    g_pc_guess->gold -= value;
    packet.platinum = platinumAmount;
    g_pc_guess->platinum -= platinumAmount;
    send_message_guess(g_world_connection_guess, 0x2131, &packet, sizeof(packet), 1);
}

// FUNCTION: TRILOGY 0x004cff2d
// Checks whether a player name is currently ignored.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::BeingIgnored, high)
// __thiscall, `this` unused. Case-insensitive match against the 30 non-empty ignore-list names.
// The original's loop test is signed (jl); a pointer compare here is unsigned (jb), the one remaining diff.
int CEverQuest::BeingIgnored(char* name)
{
    char* p;

    for (p = g_ignore_list_guess; p < g_ignore_list_guess + 30 * 32; p += 32)
    {
        if (*p != '\0' && _strcmpi(name, p) == 0)
            return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004cff5e
// Updates the server with the character's autosplit setting.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall (EQ_Main::ProcessButton loads ECX from pinstCEverQuest), `this` unused. Sends the profile's
// autosplit dword (0xAB4) as spawn appearance type 0x1C of the local player and sets the UI redraw flag.
void CEverQuest::SendAutoSplit_guess(void)
{
    MsgSpawnAppearance msg;

    msg.type = 0x1C;
    msg.spawn_id = g_local_player_guess->spawn_id;
    msg.parameter = *(u32*)&g_pc_guess->autosplit;
    send_message_guess(g_world_connection_guess, 0x20F5, &msg, sizeof(msg), 1);
    *(s32*)(g_eqmain_guess + 0xEB8) = 1;
}

// FUNCTION: TRILOGY 0x004CFFB1
// Turns the controlled character or active camera with mouse-look input.
// @early-stop: FixHeading's ECX load order, the look negation's 32-bit reload, the clamp's fcompp and the store's lea differ.
// __stdcall (RET 4). The caller sets ECX, but the body overwrites it before any read, so the original is a
// method that ignores `this`. No class declares it, so it is a free function here (same code).
// Mouse look for the active view g_activeView_guess (0 = the controlled player). The turn input g_mouse_button1_guess and the
// look input g_mouse_button2_guess (negated when DAT_006B6458 is set) are scaled by the speed DAT_006B641C. movementAllowed
// gates only the player's turn. Default cameras (0x006B6F18, 0x1C bytes each): value[0] at +0x00, heading at
// +0x04, flag at +0x18; a flagged camera turns its heading through FixHeading (0x00408156).
void __stdcall ApplyMouseLook_guess(int movementAllowed)
{
    float input;
    float upper;
    float clamped;
    short look;
    int slot;
    float heading;
    float* camHeading;

    if (EQ_GLOBAL(short, 0x006B6418) != 0)
    {
        input = (float)(int)EQ_GLOBAL(short, 0x006B6418);
        slot = g_activeView_guess;
        if (g_activeView_guess == 0 || EQ_ARRAY(u8, 0x006B6F30)[g_activeView_guess * 0x1C] == 0)
        {
            if (g_control_player_guess->race == 0x8D)
            {
                if (input > EQ_GLOBAL(const float, 0x005351D0))
                {
                    input = EQ_GLOBAL(const float, 0x005351D0);
                }
                else if (input < EQ_GLOBAL(const float, 0x0053574C))
                {
                    input = EQ_GLOBAL(const float, 0x0053574C);
                }
                g_control_player_guess->delta_heading = -input;
            }
            else
            {
                if (g_activeView_guess == 0 && movementAllowed != 0)
                {
                    if (input > (float)(EQ_GLOBAL(short, 0x006B641C) * 10))
                    {
                        input = (float)(EQ_GLOBAL(short, 0x006B641C) * 10);
                    }
                    else if (input < (float)(EQ_GLOBAL(short, 0x006B641C) * -10))
                    {
                        input = (float)(EQ_GLOBAL(short, 0x006B641C) * -10);
                    }
                    g_control_player_guess->delta_heading = -input;
                }
            }
        }
        else
        {
            if (input > (float)(EQ_GLOBAL(short, 0x006B641C) * 2))
            {
                input = (float)(EQ_GLOBAL(short, 0x006B641C) * 2);
            }
            else if (input < (float)(EQ_GLOBAL(short, 0x006B641C) * -2))
            {
                input = (float)(EQ_GLOBAL(short, 0x006B641C) * -2);
            }
            EQ_GLOBAL(u8, 0x006B645B) = 0;
            camHeading = (float*)(EQ_ARRAY(u8, 0x006B6F18) + g_activeView_guess * 0x1C + 4);
            input = input + *camHeading;
            *camHeading = input;
            heading = EQ_FASTCALL(float, 0x00408156, (void *, float))(g_display_guess, input);
            slot = g_activeView_guess;
            *(float*)(EQ_ARRAY(u8, 0x006B6F18) + g_activeView_guess * 0x1C + 4) = heading;
        }
    }
    slot = g_activeView_guess;
    if (EQ_GLOBAL(short, 0x006B641A) != 0 && g_control_player_guess->race != 0x8D)
    {
        look = EQ_GLOBAL(short, 0x006B641A);
        if (EQ_GLOBAL(u8, 0x006B6458) != 0)
        {
            look = (short)-EQ_GLOBAL(short, 0x006B641A);
            EQ_GLOBAL(short, 0x006B641A) = look;
        }
        input = (float)(int)look;
        if (slot == 0 || g_control_player_guess->actor_info->unknown0x099 == 5)
        {
            if (g_control_player_guess->actor_info->unk_0xB8 >= EQ_GLOBAL(const float, 0x005356FC))
            {
                if (input > (float)((int)EQ_GLOBAL(short, 0x006B641C) << 2))
                {
                    input = (float)((int)EQ_GLOBAL(short, 0x006B641C) << 2);
                }
                else if (input < (float)(EQ_GLOBAL(short, 0x006B641C) * -4))
                {
                    input = (float)(EQ_GLOBAL(short, 0x006B641C) * -4);
                }
                *(float*)&g_control_player_guess->unk_0x60 = *(float*)&g_control_player_guess->unk_0x60 - input;
                if (EQ_GLOBAL(const float, 0x00535118) < *(float*)&g_control_player_guess->unk_0x60)
                {
                    *(float*)&g_control_player_guess->unk_0x60 = EQ_GLOBAL(const float, 0x00535118);
                }
                if (*(float*)&g_control_player_guess->unk_0x60 < EQ_GLOBAL(const float, 0x00535748))
                {
                    *(float*)&g_control_player_guess->unk_0x60 = EQ_GLOBAL(const float, 0x00535748);
                }
            }
        }
        else if (slot == 1)
        {
            EQ_GLOBAL(float, 0x006B6F3C) = EQ_GLOBAL(float, 0x006B6F3C) - input;
        }
        else
        {
            if (input > (upper = (float)EQ_GLOBAL(short, 0x006B641C) * EQ_GLOBAL(const double, 0x005344B0)))
            {
                clamped = upper;
            }
            else if (input < -upper)
            {
                clamped = -upper;
            }
            else
            {
                clamped = input;
            }
            slot = slot * 0x1C;
            clamped = clamped + *(float*)(EQ_ARRAY(u8, 0x006B6F18) + slot);
            *(float*)(EQ_ARRAY(u8, 0x006B6F18) + slot) = clamped;
            if (clamped < EQ_GLOBAL(const float, 0x005351D0))
            {
                *(float*)(EQ_ARRAY(u8, 0x006B6F18) + slot) = 3.0f;
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004d024e
// Prepares a boat name for profile use by removing digits and '#'.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::StripShipName, medium)
// __thiscall. Copies `name` without its digits (is_number of each character) and '#' into the next of the 64
// rotating name buffers (CEverQuest + 0x13ED, 0x840 bytes each; index at +0x13E8) and returns that buffer.
char* CEverQuest::StripShipName(char* name)
{
    short n = 0;
    short len;
    int count;
    char* p = name;
    char one[2];
    int idx;

    len = strlen(name);
    if (len > 0)
    {
        count = len;
        do
        {
            one[0] = *p;
            one[1] = '\0';
            if (!is_number(one) && *p != '#')
                CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n++] = *p;
            p++;
        } while (--count != 0);
    }
    CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = '\0';
    idx = this->nameBufferIndex_guess;
    this->nameBufferIndex_guess = idx + 1;
    if (this->nameBufferIndex_guess >= 64)
        this->nameBufferIndex_guess = 0;
    return CEQ_NAME_BUFFER(this, idx);
}

  // FUNCTION: TRILOGY 0x004d02fd
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::trimName, medium)
// __thiscall. As StripShipName (digits and '#' dropped, next rotating name buffer), and '_' becomes a space.
char* CEverQuest::trimName_guess(char* name)
{
    short n = 0;
    short len;
    int count;
    char one[2];
    int index;
    int nextIndex;

    len = strlen(name);
    if (len > 0)
    {
        char* p = name;
        count = len;
        do
        {
            one[0] = *p;
            one[1] = '\0';
            if (!is_number(one) && *p != '#')
            {
                if (*p == '_')
                    CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = ' ';
                else
                    CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = *p;
                n++;
            }
            p++;
        } while (--count != 0);
    }
    CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = '\0';
    index = this->nameBufferIndex_guess;
    nextIndex = index + 1;
    this->nameBufferIndex_guess = nextIndex;
    if (nextIndex >= 64)
        this->nameBufferIndex_guess = 0;
    return CEQ_NAME_BUFFER(this, index);
}

  // FUNCTION: TRILOGY 0x004d03ce
  // ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
  void CancelSneakHide(void)
  {
      if (EQ_GLOBAL(u8, 0x006B65D8) != '\0' || EQ_GLOBAL(short, 0x006B6772) == 0x1d)
      {
          if (EQ_GLOBAL(short, 0x006B6772) == 0x1d)
              EQ_GLOBAL(short, 0x006B6772) = -1;
          EQ_GLOBAL(u8, 0x006B65D8) = '\0';
          EQ_METHOD(0, void, 0x004255CE, (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
          EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x0056f410);
      }
      if (g_local_player_guess->sneaking != '\0')
      {
          g_local_player_guess->sneaking = '\0';
          EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))((const char*)0x00540df8);
      }
  }

  // FUNCTION: TRILOGY 0x004d0436
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::stripName, high)
// __thiscall. As trimName, but stops at the first apostrophe (a possessive "'s" and what follows is cut).
char* CEverQuest::stripName_guess(char* name)
{
    short len;
    char one[2];
    int idx;
    short i;
    short n = 0;

    len = strlen(name);
    for (i = 0; i < len; i++)
    {
        if (name[i] == '\'')
            break;
        one[0] = name[i];
        one[1] = '\0';
        if (!is_number(one) && name[i] != '#')
        {
            if (name[i] == '_')
                CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = ' ';
            else
                CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = name[i];
            n++;
        }
    }
    CEQ_NAME_BUFFER(this, this->nameBufferIndex_guess)[n] = '\0';
    idx = this->nameBufferIndex_guess;
    this->nameBufferIndex_guess = idx + 1;
    if (this->nameBufferIndex_guess >= 64)
        this->nameBufferIndex_guess = 0;
    return CEQ_NAME_BUFFER(this, idx);
}

// FUNCTION: TRILOGY 0x004d055e
// Displays chat messages and records them in the bazaar log when enabled.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::dsp_chat, low)
// __thiscall. Shows a non-empty chat line in `color` unless a login screen object (0x006E4E70 / 0x006E4E6C)
// exists: the filter/parse step 0x004D061A, then the chat window (0x006E4DE8) when there is one, then the
// bazaar log when 0x006E2050 is 1.
void CEverQuest::dsp_chat_guess(const char* text, int color)
{
    if (strlen(text) >= 1 && g_char_create_guess == NULL && g_char_select_guess == NULL)
    {
        CEverQuest_ParseChatLine_guess(this, text);
        if (g_chatWindow_guess != NULL)
            EQ_TextIO_ProcessTextWithWordWrapping_guess(g_chatWindow_guess, text, color, 1);
        if (g_bazaar_log_on_guess == 1)
            bazaarlog_guess(text);
    }
}

// FUNCTION: TRILOGY 0x004d05b7
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address; TAKP name print_chat_dsp_chat_2, low)
// __thiscall. dsp_chat with colour 0x111, and only while the chat window (0x006E4DE8) exists.
void CEverQuest::print_chat_guess(const char* text)
{
    if (strlen(text) >= 1 && g_char_create_guess == NULL && g_char_select_guess == NULL && g_chatWindow_guess != NULL)
    {
        CEverQuest_ParseChatLine_guess(this, text);
        if (g_chatWindow_guess != NULL)
            EQ_TextIO_ProcessTextWithWordWrapping_guess(g_chatWindow_guess, text, 0x111, 1);
        if (g_bazaar_log_on_guess == 1)
            bazaarlog_guess(text);
    }
}

// FUNCTION: TRILOGY 0x004d061a
// Expands chat escapes: %T target name, %M your pet, %S/%O/%P target's pronouns, %R target's race.
// The caller passes `this` in ECX, but this routine never reads it. Its stack argument is a mutable chat string.
// Rewrites each %T/%M/%S/%O/%P/%R escape in place (at most 11 of them); a bare % becomes '@'.
// @early-stop: the len/prev-char test block sits before the arms (original puts it last); tail merge host is the last arm, not the first
/* fake match: `text` is reused for the '%' position and gender switches list default first, to steer spills and layout */
void __fastcall DoPercentConvert(void* ignored_this, int ignored_edx, char* str)
{
    char buffer[2048];
    char* cursor;
    int count;
    int len;
    char* marker;
    char next;
    EQPlayer* pet;
    u32 localSpawnId;

    count = 0;
    cursor = str;
    while (EQ_CDECL(char*, 0x00522A30, (const char*, const char*))(cursor, EQ_ARRAY(char, 0x0056F43C)))
    {
        len = EQ_CDECL(size_t, 0x00523560, (const char*, const char*))(cursor, EQ_ARRAY(char, 0x0056F43C));
        if (len > 1 && cursor[len - 1] != ' ' && cursor[len - 1] != '\'')
        {
            marker = cursor + len;
            next = marker[1];
            if (next != ' ' && next != '\0' && next != '\'')
                *marker = '@';
            else
                cursor = marker + 1;
        }
        else if (count > 10)
        {
            cursor[len] = '@';
        }
        else
        {
            str = cursor + len;
            next = str[1];
            if (next == 't' || next == 'T')
            {
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
                    strcat(buffer, EQ_ARRAY(char, 0x00565A90));
                else
                    strcat(buffer, EQ_METHOD(1, char*, 0x004D0436, (CEverQuest*, char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->name));
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next == 'm' || next == 'M')
            {
                pet = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (pet != NULL)
                {
                    localSpawnId = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                    while (pet != NULL)
                    {
                        if (pet->pet_owner_id == localSpawnId)
                            break;
                        pet = pet->next;
                    }
                }
                if (pet != NULL)
                    strcat(buffer, EQ_METHOD(1, char*, 0x004D0436, (CEverQuest*, char*), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(pet->name));
                else
                    strcat(buffer, EQ_ARRAY(char, 0x0056F42C));
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next == 's' || next == 'S')
            {
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
                    strcat(buffer, EQ_ARRAY(char, 0x00568220));
                else
                {
                    switch (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->gender)
                    {
                    default:
                        strcat(buffer, EQ_ARRAY(char, 0x00568220));
                        break;
                    case 0:
                        strcat(buffer, EQ_ARRAY(char, 0x0056B7A8));
                        break;
                    case 1:
                        strcat(buffer, EQ_ARRAY(char, 0x0056B7AC));
                        break;
                    }
                }
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next == 'o' || next == 'O')
            {
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
                    strcat(buffer, EQ_ARRAY(char, 0x00568220));
                else
                {
                    switch (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->gender)
                    {
                    default:
                        strcat(buffer, EQ_ARRAY(char, 0x00568220));
                        break;
                    case 0:
                        strcat(buffer, EQ_ARRAY(char, 0x005681D8));
                        break;
                    case 1:
                        strcat(buffer, EQ_ARRAY(char, 0x0056AAC0));
                        break;
                    }
                }
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next == 'p' || next == 'P')
            {
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) == NULL)
                    strcat(buffer, EQ_ARRAY(char, 0x00568224));
                else
                {
                    switch (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->gender)
                    {
                    default:
                        strcat(buffer, EQ_ARRAY(char, 0x00568224));
                        break;
                    case 0:
                        strcat(buffer, EQ_ARRAY(char, 0x005681DC));
                        break;
                    case 1:
                        strcat(buffer, EQ_ARRAY(char, 0x0056AAC0));
                        break;
                    }
                }
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next == 'r' || next == 'R')
            {
                strncpy(buffer, cursor, len);
                buffer[len] = '\0';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != NULL)
                {
                    if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 0)
                        strcat(buffer, EQ_METHOD(1, char*, 0x004C9105, (CEverQuest*, short), EQ_GLOBAL(CEverQuest*, 0x006EFE2C))(EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->race));
                    else if (EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 2)
                        strcat(buffer, EQ_ARRAY(char, 0x0056F434));
                    else
                        strcat(buffer, EQ_ARRAY(char, 0x0056F430));
                }
                strcat(buffer, str + 2);
                strcpy(cursor, buffer);
                cursor = str + 2;
                count++;
            }
            else if (next != ' ')
                *str = '@';
            else
                cursor = str + 1;
        }
    }
}

// FUNCTION: TRILOGY 0x004d0952
// Sends the player's emote text to the world and echoes it in chat.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void __fastcall CEverQuest_Emote_guess_guess(void* self, int ignored_edx)
{
    size_t length;
    char message[512];

    if (EQ_GLOBAL(char, 0x006B6465) == '\0')
    {
        sprintf(
            message, (const char*)0x00558564, (char*)self + 0xba8);
        EQ_METHOD(1, void, 0x004D061A, (void*, char*), self)(message);
        length = strlen(message);
        message[length] = '\0';
        length = strlen(message);
        send_message_guess(
            g_world_connection_guess, 0x2015, message, length + 1, 1);
        sprintf(
            message, (const char*)0x0053e668,
            EQ_GLOBAL(char*, 0x006E4DA8), (char*)self + 0xba8);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(message, 0x107);
    }
    return;
}

class CEverQuest_send_chat_guess_class : public CEverQuest
{
public:
    void send_chat_guess(void);
};

// FUNCTION: TRILOGY 0x004d0a8e
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The chat entry text is stored in the CEverQuest object at offset 0xBA8.
void CEverQuest_send_chat_guess_class::send_chat_guess(void)
{
    char chatLine[2112];
    EQPlayer* player;
    char* text;

    if ((EQ_GLOBAL(int, 0x006E4E70) == 0) &&
        (EQ_GLOBAL(int, 0x006E4E6C) == 0) &&
        (EQ_GLOBAL(char, 0x006B63F8) == '\0') &&
        (EQ_GLOBAL(char, 0x006B6465) == '\0') &&
        (DAT_006b6494 == 0))
    {
        player = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
        text = (char*)this + 0x0BA8;
        sprintf(chatLine, (const char*)0x0056F468,
             player->name, text);
        EQ_METHOD(3, void, 0x004D0D77, (void*, short, const char*, const char*), this)(8, 0, text);
        sprintf(chatLine, (const char*)0x005639C4, text);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(chatLine, 0x111);
    }
    return;
}

// FUNCTION: TRILOGY 0x004d0b20
// Sends the player's message to party members and echoes it as party chat.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __fastcall (`this` in ECX, EDX unused), no stack arguments. If the name buffer at 0x005fe968 starts with a NUL it
// prints "You are not in a group". Otherwise, while the three gate globals are clear, it echoes "You tell your
// party" and sends the text at +0xBA8 to each non-empty name in the 30-byte table at 0x005fe8bd (up to 0x005fe953)
// through CEverQuest_SendNewText_guess(2, name, text).
void __fastcall CEverQuest_send_gsay_guess_guess(void* self, int ignored_edx)
{
    char chatLine[2112];
    char* text;
    char* slot;

    if (EQ_GLOBAL(char, 0x005FE968) == '\0')
    {
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)((const char*)0x0056F494, 0x111);
    }
    else if ((EQ_GLOBAL(char, 0x006B63F8) == '\0') &&
             (EQ_GLOBAL(char, 0x006B6465) == '\0') &&
             (DAT_006b6494 == 0))
    {
        text = (char*)self + 0x0BA8;
        sprintf(chatLine, (const char*)0x0056F478, text);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), self)(chatLine, 0x111);
        slot = EQ_ARRAY(char, 0x005FE8BD);
        do
        {
            if (*slot != '\0')
            {
                EQ_METHOD(3, void, 0x004D0D77, (void*, short, const char*, const char*), self)(2, slot, text);
            }
            slot = slot + 0x1e;
        } while ((int)slot < 0x005FE953);
    }
    return;
}

// FUNCTION: TRILOGY 0x004d0bb6
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::reqChannel, high)
// __thiscall, `this` unused. Sends the empty opcode 0x2071.
void CEverQuest::reqChannel(void)
{
    send_message_guess(g_world_connection_guess, 0x2071, NULL, 0, 1);
}

class CEverQuest_send_shout_guess_class : public CEverQuest
{
public:
    void send_shout_guess(void);
};

// FUNCTION: TRILOGY 0x004d0bcf
// Sends the player's text as a shout and echoes it in chat.
// __thiscall, no stack arguments. Sends the shout text (channel 3) and echoes it to chat, but only
// while the five gate globals are clear. The 0xBA8 text field of the object is used, not a named member.
// The first sprintf (name and text) is overwritten by the echo before use; the original still runs it.
void CEverQuest_send_shout_guess_class::send_shout_guess(void)
{
    char chatLine[2112];

    if ((EQ_GLOBAL(int, 0x006E4E70) == 0) &&
        (EQ_GLOBAL(int, 0x006E4E6C) == 0) &&
        (EQ_GLOBAL(char, 0x006B63F8) == '\0') &&
        (EQ_GLOBAL(char, 0x006B6465) == '\0') &&
        (DAT_006b6494 == 0))
    {
        char* text = (char*)this + 0x0BA8;
        sprintf(chatLine, (const char*)0x0056F4DC, g_local_player_guess->name, text);
        EQ_METHOD(3, void, 0x004D0D77, (void*, short, const char*, const char*), this)(3, 0, text);
        sprintf(chatLine, (const char*)0x0056F4CC, text);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(chatLine, 0x111);
    }
    return;
}

class CEverQuest_send_auction_guess_class : public CEverQuest
{
public:
    void send_auction_guess(void);
};

// FUNCTION: TRILOGY 0x004d0c61
// Sends the player's text on the auction channel and echoes it in chat.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, no stack arguments. Sends the auction text (channel 4) and echoes it to chat, but only
// while the four gate globals are clear. The 0xBA8 text field of the object is used, not a named member.
void CEverQuest_send_auction_guess_class::send_auction_guess(void)
{
    char chatLine[2112];

    if ((EQ_GLOBAL(int, 0x006E4E70) == 0) &&
        (EQ_GLOBAL(int, 0x006E4E6C) == 0) &&
        (EQ_GLOBAL(char, 0x006B63F8) == '\0') &&
        (EQ_GLOBAL(char, 0x006B6465) == '\0') &&
        (DAT_006b6494 == 0))
    {
        char* text = (char*)this + 0x0BA8;
        sprintf(chatLine, (const char*)0x0053E678, text);
        EQ_METHOD(3, void, 0x004D0D77, (void*, short, const char*, const char*), this)(4, 0, text);
        sprintf(chatLine, (const char*)0x0056F4EC, text);
        EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(chatLine, 0x111);
    }
    return;
}

class CEverQuest_send_ooc_guess_class : public CEverQuest
{
public:
    void send_ooc_guess(void);
};

// FUNCTION: TRILOGY 0x004d0cec
// Sends the player's text on the out-of-character channel and echoes it in chat.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, no stack arguments. Sends the out-of-character text (channel 5) and echoes it to chat, but
// only while the five gate globals are clear. The 0xBA8 text field of the object is used, not a named member.
void CEverQuest_send_ooc_guess_class::send_ooc_guess(void)
{
    char chatLine[2112];

    if ((EQ_GLOBAL(int, 0x006E4E70) == 0) &&
        (EQ_GLOBAL(int, 0x006E4E6C) == 0) &&
        (EQ_GLOBAL(char, 0x006B63F8) == '\0') &&
        (EQ_GLOBAL(char, 0x006B6465) == '\0') &&
        (DAT_006b6494 == 0))
    {
        char* text = (char*)this + 0x0BA8;
        /* These fixed-address callees are true thiscall methods; keep their member-call ABI. */
        typedef void (CEverQuest_send_ooc_guess_class::*SendNewTextFunction_guess)
            (short, const char*, const char*);
        union SendNewTextFunctionAddress_guess
        {
            void (__cdecl *function)(void);
            SendNewTextFunction_guess method;
        };
        typedef void (CEverQuest_send_ooc_guess_class::*DspChatFunction_guess)
            (const char*, int);
        union DspChatFunctionAddress_guess
        {
            void (__cdecl *function)(void);
            DspChatFunction_guess method;
        };
        SendNewTextFunctionAddress_guess sendNewText;
        DspChatFunctionAddress_guess dspChat;
        sendNewText.function = EQ_FUNC(void, __cdecl, 0x004D0D77, (void));
        dspChat.function = EQ_FUNC(void, __cdecl, 0x004D055E, (void));
        sprintf(chatLine, (const char*)0x0053E678, text);
        (this->*sendNewText.method)(5, 0, text);
        sprintf(chatLine, (const char*)0x0056F500, text);
        (this->*dspChat.method)(chatLine, 0x111);
    }
    return;
}

// FUNCTION: TRILOGY 0x004d0d77
// Sends a player chat line on its selected channel, with its speaker and language.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// The existing class declaration has no entry for this method. This fastcall bridge keeps its
// thiscall register/stack layout (ECX=this, EDX unused, three stack arguments).
// @early-stop: zero and this registers swapped (EBX/EDI); languageSkill is a local, the original does it in frame.first
void __fastcall CEverQuest_SendNewText_guess(CEverQuest* self, int unusedEdx,
                                               short channel, const char* name, const char* text)
{
    struct SendNewTextFirstPacket_guess
    {
        char name[32];
        char speaker[32];
        short language;
        short channel;
        short languageSkill;
        char text[2010];
    };
    struct SendNewTextSecondPacket_guess
    {
        char header[96];
        short channel;
        char text[2010];
    };
    struct SendNewTextFrame_guess
    {
        char local_18b0[2048];
        SendNewTextSecondPacket_guess second;
        char gap[40];
        SendNewTextFirstPacket_guess first;
        char reserve[40];
    };

    SendNewTextFrame_guess frame;
    short languageId;
    short localChannel;
    short languageSkill;
    size_t textLength;
    char duplicateText;
    EQPlayer* target;

    if (EQ_GLOBAL(void*, 0x006E4DE8) != 0)
    {
        if (name == 0)
            strcpy(frame.first.name, EQ_ARRAY(char, 0x0058FBA4));
        else
            strcpy(frame.first.name, name);

        if (channel == 8 && EQ_GLOBAL(EQPlayer*, 0x006E4DAC) != 0 &&
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->type == 1 &&
            EQ_GLOBAL(EQPlayer*, 0x006E4DAC)->gm == 0 &&
            EQ_METHOD(2, SpellBuff*, 0x0042381B, (CharacterProfile*, u8, short*), EQ_GLOBAL(CharacterProfile*, 0x006E4DA8))(0x4b, 0) != 0 &&
            (target = EQ_GLOBAL(EQPlayer*, 0x006E4DAC)) != 0 &&
            target->pet_owner_id == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id)
        {
            strcpy(frame.first.speaker, (char*)target + 1);
        }
        else
        {
            strcpy(frame.first.speaker, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
        }

        strcpy(frame.first.text, text);
        EQ_METHOD(1, void, 0x004D061A, (CEverQuest*, char*), self)(frame.first.text);

        localChannel = channel;
        frame.first.channel = localChannel;
        languageId = *(short*)((char*)EQ_GLOBAL(void*, 0x006E4DE8) + 0x17c +
                               (int)(short)EQ_GLOBAL(short, 0x006B642A) * 2);
        frame.first.language = languageId;
        languageSkill = (short)((u8)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->languages[languageId] -
                                (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->intoxication >> 2));
        if (languageSkill < 0)
            languageSkill = 0;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != 0)
            languageSkill = 100;
        frame.first.languageSkill = languageSkill;

        textLength = strlen(frame.first.text);
        send_message_guess
            (g_world_connection_guess, 0x2107, &frame.first,
             (int)(short)((short)textLength + 0x4b), 1);

        duplicateText = '\0';
        if (frame.first.channel == 2 &&
            strcmp(frame.first.text, frame.local_18b0) == 0)
            duplicateText = '\1';
        strcpy(frame.local_18b0, frame.first.text);

        if (EQ_GLOBAL(char, 0x006B64A8) != 0)
        {
            strcpy(frame.second.header, EQ_ARRAY(char, 0x006B64A8));
            strcpy(frame.second.header + 64, (const char*)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8));
            if (name == 0)
                strcpy(frame.second.header + 32, EQ_ARRAY(char, 0x0058FBA4));
            else
                strcpy(frame.second.header + 32, name);
            strcpy(frame.second.text, frame.first.text);
            frame.second.channel = channel;
            textLength = strlen(frame.first.text);
            textLength += 0x67;

            if (duplicateText == '\0' &&
                (strcmp(frame.first.speaker, EQ_ARRAY(char, 0x006B64A8)) != 0 ||
                 strcmp(frame.first.name, EQ_ARRAY(char, 0x006B64A8)) != 0))
            {
                send_message_guess
                    (g_world_connection_guess, 0x2133, &frame.second,
                     (int)(short)textLength, 1);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004d0fe6
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// The this pointer is unused; the fastcall bridge preserves the original ECX/stack ABI.
void __fastcall CEverQuest_DisplayScreen_guess(CEverQuest* self, int unusedEdx, char* filename)
{
    typedef void (__cdecl *t3dClearRenderToBlack_guess_t)(int);
    int charIndex;
    int value;
    size_t textLength;
    int textY;
    int textWidth;
    char screenText[256];

    charIndex = EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)();
    if (charIndex != 0)
    {
        if (EQ_GLOBAL(void*, 0x006ECDF4) != 0)
            EQ_METHOD(0, void, 0x004E4DE4, (void*), EQ_GLOBAL(void*, 0x006ECDF4))();
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
            pinstEQ_Main_guess->renderDollActive_guess = 0;
        g_game_state_guess = 2;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        value = strcmp(filename, EQ_ARRAY(char, 0x0056F574));
        if (value == 0)
            strcpy(screenText, EQ_ARRAY(char, 0x0056F558));
        else
            strcpy(screenText, EQ_ARRAY(char, 0x0056F538));
        EQ_GLOBAL(int, 0x006B6490) = 0;
        DbgLog_guess(screenText);
        textY = EQ_GLOBAL(int, 0x006B6400) / 2 - 4;
        textLength = strlen(screenText);
        textWidth = 0;
        for (charIndex = 0; charIndex < (int)textLength; charIndex++)
        {
            if ('\0' < screenText[charIndex])
                textWidth = textWidth + EQ_GLOBAL(int*, 0x006EFDF0)[screenText[charIndex]];
        }
        value = EQ_GLOBAL(int, 0x006B63FC) / 2 - textWidth / 2;
        EQ_GLOBAL(t3dClearRenderToBlack_guess_t, 0x006E5178)(0);
        EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(screenText,
             value, textY, 10, EQ_GLOBAL(int, 0x006EFE0C), 1);
        EQ_METHOD(0, void, 0x0040E418, (void*), g_display_guess)();
        DbgLog_guess(EQ_ARRAY(char, 0x0056F520));
    }
    return;
}

// FUNCTION: TRILOGY 0x004d1112
// Checks character creation names for length, spaces, and a reserved prefix.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall (the character creation screen, 0x0042B812, loads ECX from pinstCEverQuest), `this` unused.
// A new character name must be 4..15 characters without spaces and not start with "type"; otherwise sets
// the error flag (+0x1BF0 of the object at 0x006E4E70) and returns 0.
int CEverQuest::IsValidName_guess(char* name)
{
    int len = strlen(name);
    int i;

    if (len >= 4 && len <= 15)
    {
        for (i = 0; i < len; i++)
        {
            if (name[i] == ' ')
                goto bad;
        }
        if (strncmp(name, "type", 4) != 0)
            return 1;
    }
bad:
    pinstEQ_CharCreateS_guess->message = 1;
    return 0;
}

// The character creation screen (the object at 0x006E4E70, 0x1BF4 bytes): the fields DoCharacterCreation uses.
struct CharCreateScreen_guess
{
    void** vtable;
    u8 unknown0x0004[0x34C];
    RECT acceptButton;
    u8 unknown0x0360[0x180];
    RECT backButton;
    RECT forwardButton;
    u8 unknown0x0500[0x100];
    RECT pageTabs[4];
    RECT leftArrow;
    RECT rightArrow;
    CharacterProfile* pc;
    int awaitingApproval;
    u8 unknown0x0668[0x132C];
    u8 baseStats[7];
    u8 maxStats[7];
    u8 page;
    u8 unknown0x19A3[3];
    s16 pointsLeft;
    u8 unknown0x19A8[0x200];
    s16 nameEntryActive;
    s16 deity;
    u8 unknown0x1BAC[6];
    s16 hoverButton;
    u32 requestTime;
    u8 unknown0x1BB8[8];
    u8 face;
    u8 unknown0x1BC1[3];
    int unknown0x1BC4;
    u8 unknown0x1BC8[8];
    s16 city;
    u16 race;
    u8 class_;
    u8 gender;
    u8 unknown0x1BD6;
    u8 features[6];
    u8 unknown0x1BDD[3];
    int nameSubmitted;
    int finished;
    u8 unknown0x1BE8[8];
    s16 message;
    u8 unknown0x1BF2[2];
};
C_ASSERT(sizeof(CharCreateScreen_guess) == 0x1BF4);
C_ASSERT(offsetof(CharCreateScreen_guess, pageTabs) == 0x600);
C_ASSERT(offsetof(CharCreateScreen_guess, pc) == 0x660);
C_ASSERT(offsetof(CharCreateScreen_guess, baseStats) == 0x1994);
C_ASSERT(offsetof(CharCreateScreen_guess, pointsLeft) == 0x19A6);
C_ASSERT(offsetof(CharCreateScreen_guess, nameEntryActive) == 0x1BA8);
C_ASSERT(offsetof(CharCreateScreen_guess, face) == 0x1BC0);
C_ASSERT(offsetof(CharCreateScreen_guess, race) == 0x1BD2);
C_ASSERT(offsetof(CharCreateScreen_guess, features) == 0x1BD7);
C_ASSERT(offsetof(CharCreateScreen_guess, message) == 0x1BF0);

// CEverQuest as DoCharacterCreation sees it (the class has no data members and doesn't declare the method).
class CEverQuest_DoCharacterCreation_guess
{
public:
    int loginState;
    int gameState;
    u8 unknown0x00008[0x223E8];
    int inCharacterCreation;
    int disconnected;
    void DoCharacterCreation(int unused1, int unused2);
};
C_ASSERT(offsetof(CEverQuest_DoCharacterCreation_guess, inCharacterCreation) == 0x223F0);

#define g_ccreate_guess             EQ_GLOBAL(CharCreateScreen_guess*, 0x006E4E70)
#define CCREATE_DRAW(image, rect) \
    EQ_METHOD(2, void, 0x004384B2, (void*, RECT*, int), (image))((rect), 1)
#define CCREATE_BASE_ATTR(stat) \
    EQ_METHOD(3, u8, 0x0041C5AA, (CharacterProfile*, short, short, int), g_ccreate_guess->pc) \
        (g_ccreate_guess->class_, g_ccreate_guess->race, (stat))
#define CCREATE_CLASS_MIN(stat) \
    EQ_METHOD(2, u8, 0x0041CB5F, (CharacterProfile*, short, int), g_ccreate_guess->pc)(g_ccreate_guess->race, (stat))
#define CCREATE_CAN_WORSHIP(va, deity) \
    EQ_METHOD(2, bool, va, (CharacterProfile*, u8, u8), g_ccreate_guess->pc)(g_ccreate_guess->class_, (u8)(deity))
#define CCREATE_CITY_CAN_START(city) \
    EQ_METHOD(4, bool, 0x0041C957, (CharacterProfile*, short, short, short, short), g_ccreate_guess->pc) \
        ((city), g_ccreate_guess->race, g_ccreate_guess->deity, g_ccreate_guess->class_)
#define CCREATE_MUSIC(track) \
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0) \
        EQ_METHOD(1, void, 0x004E4EE6, (void*, int), EQ_GLOBAL(void*, 0x006ECDF4))(track)
#define CCREATE_TEXT(text, x, y) \
    EQ_METHOD(6, void, 0x0048D21A, (void*, const char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68)) \
        ((text), (x), (y), 15, EQ_GLOBAL(int, 0x006EFE04), 1)
#define CCREATE_HEAP_OBJECT(Name, size, ctorVa, dtorVa) \
    struct Name \
    { \
        static void* operator new(size_t count) { return EQ_CDECL(void*, 0x005218C6, (u32))((u32)count); } \
        static void operator delete(void* storage) { EQ_CDECL(void, 0x005218BB, (void*))(storage); } \
        Name() { EQ_METHOD(0, void*, ctorVa, (void*), this)(); } \
        ~Name() { EQ_METHOD(0, void, dtorVa, (void*), this)(); } \
        u8 data[size]; \
    }

struct NewCharacter_guess
{

    // EXTERNAL: TRILOGY 0x0047EB8E NewCharacter_guess::NewCharacter_guess
    NewCharacter_guess(int flags);
    u8 data[0x1FA4];
};

// FUNCTION: TRILOGY 0x004d1166
// Runs the character creation screen (this = CEverQuest); allocates the six create-screen objects and the
// creation state (g_ccreate_guess), then picks default race/class/city and clamps the stat maxima.
// Main loop runs while gameState is 2: pump 0x004c2c1a/0x004eb0e0, mailbox, connection check (a drop leaves
// with state 0xff), draw EQ_Main/create/char-select, then a switch on the screen's message code.
// Name approval is polled with timeouts; on finish it builds the new character profile and calls 0x0047f019.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_DoCharacterCreation_guess::DoCharacterCreation(int unused1, int unused2)
{
    typedef void (WINAPI *SleepProc_guess)(DWORD);
    typedef void (__fastcall *ScreenAction_guess)(void*);
    CCREATE_HEAP_OBJECT(CreateBS_guess, 0x84, 0x004340A1, 0x0043415B);
    CCREATE_HEAP_OBJECT(CreateBS2_guess, 0x84, 0x00434230, 0x004342BE);
    CCREATE_HEAP_OBJECT(CreateBS3_guess, 0x84, 0x004342C9, 0x0043436C);
    CCREATE_HEAP_OBJECT(CreateBS4_guess, 0x84, 0x00434377, 0x0043441A);
    CCREATE_HEAP_OBJECT(CreateBS5_guess, 0x84, 0x00434425, 0x004344C8);
    CCREATE_HEAP_OBJECT(CreateBS6_guess, 0x84, 0x004344D3, 0x00434561);
    CCREATE_HEAP_OBJECT(CharCreateAlloc_guess, 0x1BF4, 0x00428686, 0x00429292);
    short classes[16];
    int disconnectInfo[3];
    MsgNameApproval request;
    char disconnectText[128];
    CharacterProfile* pc;
    char* animSet;
    int classLimit;
    int connected;
    int pick;
    int roll;
    int bag;
    int slot;
    short classCount;
    short i;
    bool found;
    bool evil;

    EqGetTime();
    classCount = 0;
    classLimit = 14;
    EQ_METHOD(1, void, 0x0040E4C6, (void*, int), g_display_guess)(90);
    inCharacterCreation = 1;
    if (EQ_GLOBAL(void*, 0x006E4E74) == 0)
        EQ_GLOBAL(void*, 0x006E4E74) = new CreateBS_guess;
    if (EQ_GLOBAL(void*, 0x006E4E78) == 0)
        EQ_GLOBAL(void*, 0x006E4E78) = new CreateBS2_guess;
    if (EQ_GLOBAL(void*, 0x006E4E7C) == 0)
        EQ_GLOBAL(void*, 0x006E4E7C) = new CreateBS3_guess;
    if (EQ_GLOBAL(void*, 0x006E4E80) == 0)
        EQ_GLOBAL(void*, 0x006E4E80) = new CreateBS4_guess;
    if (EQ_GLOBAL(void*, 0x006E4E84) == 0)
        EQ_GLOBAL(void*, 0x006E4E84) = new CreateBS5_guess;
    if (EQ_GLOBAL(void*, 0x006E4E88) == 0)
        EQ_GLOBAL(void*, 0x006E4E88) = new CreateBS6_guess;
    if (g_ccreate_guess == 0)
    {
        g_ccreate_guess = (CharCreateScreen_guess*)new CharCreateAlloc_guess;
        if (g_ccreate_guess == 0)
            EQTRACE_guess(EQ_ARRAY(char, 0x0056F8A8));
    }
    g_ccreate_guess->pc = (CharacterProfile*)new NewCharacter_guess(0);

    if (g_cdisplay_guess->fastFileHandle6_guess != 0)
    {
        g_ccreate_guess->race = (u16)random_range_guess(0, 13);
        u16* race = &g_ccreate_guess->race;
        if (*race == 12)
            *race = 0x7F;
        else if (*race == 13)
            *race = 0x81;
        classLimit = 15;
    }
    else if (g_cdisplay_guess->fastFileHandle3_guess != 0)
    {
        g_ccreate_guess->race = (u16)random_range_guess(0, 12);
        u16* race = &g_ccreate_guess->race;
        if (*race == 12)
            *race = 0x7F;
    }
    else
    {
        g_ccreate_guess->race = (u16)random_range_guess(0, 11);
    }

    switch (g_ccreate_guess->race)
    {
    case 1:
        CCREATE_MUSIC(4);
        break;
    case 5:
        CCREATE_MUSIC(5);
        break;
    case 2:
        CCREATE_MUSIC(10);
        break;
    case 11:
        CCREATE_MUSIC(13);
        break;
    case 10:
        CCREATE_MUSIC(11);
        break;
    case 4:
        CCREATE_MUSIC(7);
        break;
    case 9:
        CCREATE_MUSIC(2);
        break;
    case 3:
        CCREATE_MUSIC(8);
        break;
    case 6:
        CCREATE_MUSIC(6);
        break;
    case 7:
        CCREATE_MUSIC(12);
        break;
    case 8:
        CCREATE_MUSIC(3);
        break;
    default:
        CCREATE_MUSIC(9);
        break;
    }

    for (i = 0; i < classLimit; i++)
    {
        if (EQ_METHOD(2, u8, 0x0041C663, (CharacterProfile*, short, short), g_ccreate_guess->pc)
                ((u8)g_ccreate_guess->race, i))
            classes[classCount++] = i;
    }
    if (classCount != 0)
        classCount--;
    pick = random_range_guess(0, classCount);
    g_ccreate_guess->class_ = (u8)classes[pick];
    g_ccreate_guess->gender = (u8)random_range_guess(0, 1);

    if (g_ccreate_guess->race + 1 == 2 && g_ccreate_guess->gender == 0)
        g_ccreate_guess->face = (u8)random_range_guess(0, 7);
    else if ((g_ccreate_guess->race + 1 == 1 && g_ccreate_guess->gender == 0) ||
             (g_ccreate_guess->race + 1 == 4 && g_ccreate_guess->gender == 0))
        g_ccreate_guess->face = (u8)random_range_guess(0, 6);
    else if (g_ccreate_guess->race + 1 == 6 && g_ccreate_guess->gender == 0)
        g_ccreate_guess->face = (u8)random_range_guess(0, 5);
    else
        g_ccreate_guess->face = (u8)random_range_guess(0, 3);
    g_ccreate_guess->face = (u8)random_range_guess(0, 7);
    g_ccreate_guess->message = 0;
    g_ccreate_guess->finished = 0;
    g_ccreate_guess->nameSubmitted = 0;
    g_ccreate_guess->unknown0x1BC4 = 0;

    for (i = 0; i < 17; i++)
    {
        if (g_ccreate_guess->race + 1 == 1)
            found = CCREATE_CAN_WORSHIP(0x0041C6BB, i);
        else if (g_ccreate_guess->race + 1 == 5)
            found = CCREATE_CAN_WORSHIP(0x0041C7AB, i);
        else if (g_ccreate_guess->race + 1 == 6)
            found = CCREATE_CAN_WORSHIP(0x0041C7D7, i);
        else if (g_ccreate_guess->race + 1 == 7)
            found = CCREATE_CAN_WORSHIP(0x0041C803, i);
        else if (g_ccreate_guess->race + 1 == 8)
            found = CCREATE_CAN_WORSHIP(0x0041C83E, i);
        else if (g_ccreate_guess->race + 1 == 10)
            found = CCREATE_CAN_WORSHIP(0x0041C896, i);
        else if (g_ccreate_guess->race + 1 == 9)
            found = CCREATE_CAN_WORSHIP(0x0041C86A, i);
        else if (g_ccreate_guess->race + 1 == 3)
            found = CCREATE_CAN_WORSHIP(0x0041C739, i);
        else if (g_ccreate_guess->race + 1 == 12)
            found = CCREATE_CAN_WORSHIP(0x0041C8F4, i);
        else if (g_ccreate_guess->race + 1 == 11)
            found = CCREATE_CAN_WORSHIP(0x0041C8C2, i);
        else if (g_ccreate_guess->race + 1 == 4)
            found = CCREATE_CAN_WORSHIP(0x0041C779, i);
        else if (g_ccreate_guess->race + 1 == 2)
            found = CCREATE_CAN_WORSHIP(0x0041C6F7, i);
        else if (g_ccreate_guess->race + 1 == 0x80)
            found = CCREATE_CAN_WORSHIP(0x0041C920, i);
        else if (g_ccreate_guess->race + 1 == 0x82)
            found = CCREATE_CAN_WORSHIP(0x0041C94C, i);
        else
            continue;
        if (found)
        {
            g_ccreate_guess->deity = i;
            break;
        }
    }
    for (i = 0; i < 14; i++)
    {
        if (CCREATE_CITY_CAN_START(i))
        {
            g_ccreate_guess->city = i;
            break;
        }
    }
    if (g_ccreate_guess->race + 1 == 1 && CCREATE_CITY_CAN_START(1) && CCREATE_CITY_CAN_START(4))
    {
        if (random_range_guess(1, 100) < 51)
            g_ccreate_guess->city = 1;
        else
            g_ccreate_guess->city = 4;
    }
    if (g_ccreate_guess->race + 1 == 7 && CCREATE_CITY_CAN_START(1) && CCREATE_CITY_CAN_START(4) &&
        CCREATE_CITY_CAN_START(9) && CCREATE_CITY_CAN_START(10))
    {
        roll = random_range_guess(1, 100);
        if (roll < 26)
            g_ccreate_guess->city = 1;
        else if (roll < 51)
            g_ccreate_guess->city = 4;
        else if (roll < 76)
            g_ccreate_guess->city = 9;
        else
            g_ccreate_guess->city = 10;
    }

    EQ_METHOD(4, void, 0x00430381, (void*, u8, u16, u8, u8), g_char_select_guess)
        (g_ccreate_guess->gender, g_ccreate_guess->race, g_ccreate_guess->face, g_ccreate_guess->class_);
    evil = EQ_METHOD(3, bool, 0x00431509, (void*, int, int, int), g_char_select_guess)
        (g_ccreate_guess->race + 1, g_ccreate_guess->class_ + 1, g_ccreate_guess->deity);
    g_local_player_guess->y = 0.0f;
    if (evil)
    {
        g_local_player_guess->x = -315.0f;
        g_local_player_guess->z = 6.0f;
    }
    else
    {
        g_local_player_guess->x = 0.0f;
        g_local_player_guess->z = 7.0f;
    }
    CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);

    g_ccreate_guess->pc->STR = CCREATE_BASE_ATTR(0) + CCREATE_CLASS_MIN(0);
    g_ccreate_guess->pc->DEX = CCREATE_BASE_ATTR(1) + CCREATE_CLASS_MIN(1);
    g_ccreate_guess->pc->AGI = CCREATE_BASE_ATTR(2) + CCREATE_CLASS_MIN(2);
    g_ccreate_guess->pc->STA = CCREATE_BASE_ATTR(3) + CCREATE_CLASS_MIN(3);
    g_ccreate_guess->pc->INT = CCREATE_BASE_ATTR(4) + CCREATE_CLASS_MIN(4);
    g_ccreate_guess->pc->WIS = CCREATE_BASE_ATTR(5) + CCREATE_CLASS_MIN(5);
    g_ccreate_guess->pc->CHA = CCREATE_BASE_ATTR(6) + CCREATE_CLASS_MIN(6);
    g_ccreate_guess->baseStats[0] = g_ccreate_guess->pc->STR;
    g_ccreate_guess->baseStats[1] = g_ccreate_guess->pc->DEX;
    g_ccreate_guess->baseStats[2] = g_ccreate_guess->pc->AGI;
    g_ccreate_guess->baseStats[3] = g_ccreate_guess->pc->STA;
    g_ccreate_guess->baseStats[4] = g_ccreate_guess->pc->INT;
    g_ccreate_guess->baseStats[5] = g_ccreate_guess->pc->WIS;
    g_ccreate_guess->baseStats[6] = g_ccreate_guess->pc->CHA;
    g_ccreate_guess->maxStats[0] = g_ccreate_guess->pc->STR + 25;
    g_ccreate_guess->maxStats[1] = g_ccreate_guess->pc->DEX + 25;
    g_ccreate_guess->maxStats[2] = g_ccreate_guess->pc->AGI + 25;
    g_ccreate_guess->maxStats[3] = g_ccreate_guess->pc->STA + 25;
    g_ccreate_guess->maxStats[4] = g_ccreate_guess->pc->INT + 25;
    g_ccreate_guess->maxStats[5] = g_ccreate_guess->pc->WIS + 25;
    g_ccreate_guess->maxStats[6] = g_ccreate_guess->pc->CHA + 25;
    if (g_ccreate_guess->maxStats[0] > 150)
        g_ccreate_guess->maxStats[0] = 150;
    if (g_ccreate_guess->maxStats[1] > 150)
        g_ccreate_guess->maxStats[1] = 150;
    if (g_ccreate_guess->maxStats[2] > 150)
        g_ccreate_guess->maxStats[2] = 150;
    if (g_ccreate_guess->maxStats[3] > 150)
        g_ccreate_guess->maxStats[3] = 150;
    if (g_ccreate_guess->maxStats[4] > 150)
        g_ccreate_guess->maxStats[4] = 150;
    if (g_ccreate_guess->maxStats[5] > 150)
        g_ccreate_guess->maxStats[5] = 150;
    if (g_ccreate_guess->maxStats[6] > 150)
        g_ccreate_guess->maxStats[6] = 150;
    for (i = 0; i < 7; i++)
        g_ccreate_guess->pointsLeft -= CCREATE_BASE_ATTR(i);
    strcpy(g_ccreate_guess->pc->name, EQ_ARRAY(char, 0x0056F88C));
    gameState = 2;
    EQ_CDECL(void, 0x004EB516, (void))();
    EQ_CDECL(void, 0x004EB0B1, (void))();
    EQ_METHOD(0, void, 0x0040D59A, (void*), g_display_guess)();

    while (gameState == 2)
    {
        EQ_GLOBAL(u8, 0x006B645A) = 0;
        EQ_CDECL(void, 0x004C2C1A, (void))();
        EQ_CDECL(void, 0x004EB0E0, (void))();
        CEverQuest_ProcessMBox_guess(this);
        if (EQ_GLOBAL(void*, 0x006B3194) == 0)
            connected = 0;
        else
            connected = EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194));
        if (connected == 0)
        {
            if (EQ_GLOBAL(void*, 0x006B3194) != 0)
            {
                EQ_CDECL(void, 0x004F2490, (void*, void*, u32))(EQ_GLOBAL(void*, 0x006B3194), disconnectInfo, 12);
                sprintf(disconnectText, EQ_ARRAY(char, 0x0056F5DC), disconnectInfo[0]);
            }
            else
            {
                strcpy(disconnectText, EQ_ARRAY(char, 0x0056F5CC));
            }
            DbgLog_guess(EQ_ARRAY(char, 0x0056F588), EQ_ARRAY(char, 0x0056F5A4), 0x384E, disconnectText);
            disconnected = 1;
            EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)(EQ_ARRAY(char, 0x0056F574));
            EQ_GLOBAL(SleepProc_guess, 0x00534198)(3000);
            loginState = 1;
            gameState = 0xFF;
            EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
            break;
        }
        if (EQ_GLOBAL(int, 0x006B6864) != 0 && g_char_select_guess != 0)
        {
            if (EQ_GLOBAL(short, 0x006B6418) == 0 && EQ_GLOBAL(short, 0x006B641A) == 0)
            {
                if (EqGetTime() - EQ_GLOBAL(u32, 0x006B64D4) > 500)
                {
                    EQ_GLOBAL(u32, 0x006B64D4) = EqGetTime();
                    short* helpDelay = (short*)(g_char_select_guess + 0xA4);
                    if (*helpDelay < 3)
                        (*helpDelay)++;
                }
            }
            else
            {
                *(short*)(g_char_select_guess + 0xA4) = 0;
            }
        }
        EQ_METHOD(0, void, 0x00434806, (void*), EQ_GLOBAL(void*, 0x006E4E48))();
        EQ_METHOD(0, void, 0x0042C654, (void*), g_ccreate_guess)();

        if (g_ccreate_guess->nameEntryActive == 0)
        {
            if (g_ccreate_guess->page != 0)
                EQ_ARRAY(float, 0x006B6F1C)[g_activeView_guess * 7] = g_local_player_guess->heading;
            else
                EQ_ARRAY(float, 0x006B6F1C)[g_activeView_guess * 7] =
                    EQ_METHOD(1, float, 0x00408156, (void*, float), g_display_guess)
                        (EQ_ARRAY(float, 0x006B6F1C)[g_activeView_guess * 7] + 3.0f);
            if (EQ_GLOBAL(int, 0x006E5480) == 0)
                EQ_METHOD(0, void, 0x0040D5C2, (void*), g_display_guess)();

            if (g_ccreate_guess->page == 0)
            {
                if (g_ccreate_guess->hoverButton == 0x2D)
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C4), &g_ccreate_guess->acceptButton);
                else
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C0), &g_ccreate_guess->acceptButton);
            }
            else
            {
                if (g_ccreate_guess->hoverButton == 0x46)
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3428), &g_ccreate_guess->backButton);
                else
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3420), &g_ccreate_guess->backButton);
                if (g_ccreate_guess->hoverButton == 0x47)
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B342C), &g_ccreate_guess->forwardButton);
                else
                    CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3424), &g_ccreate_guess->forwardButton);
                if (g_cdisplay_guess->fastFileHandle6_guess != 0 &&
                    (animSet = g_local_player_guess->actor_info->anim_set_guess) != 0 &&
                    animSet[0x39C] == 1)
                {
                    switch (g_ccreate_guess->page)
                    {
                    case 1:
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C4), &g_ccreate_guess->acceptButton);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C4), &g_ccreate_guess->pageTabs[0]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32B8), &g_ccreate_guess->pageTabs[1]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C8), &g_ccreate_guess->pageTabs[2]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C0), &g_ccreate_guess->pageTabs[3]);
                        break;
                    case 2:
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C0), &g_ccreate_guess->pageTabs[0]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32BC), &g_ccreate_guess->acceptButton);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32BC), &g_ccreate_guess->pageTabs[1]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C8), &g_ccreate_guess->pageTabs[2]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C0), &g_ccreate_guess->pageTabs[3]);
                        break;
                    case 3:
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C0), &g_ccreate_guess->pageTabs[0]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32B8), &g_ccreate_guess->pageTabs[1]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32CC), &g_ccreate_guess->acceptButton);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32CC), &g_ccreate_guess->pageTabs[2]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C0), &g_ccreate_guess->pageTabs[3]);
                        break;
                    case 4:
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B33C0), &g_ccreate_guess->pageTabs[0]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32B8), &g_ccreate_guess->pageTabs[1]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C8), &g_ccreate_guess->pageTabs[2]);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C4), &g_ccreate_guess->acceptButton);
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32C4), &g_ccreate_guess->pageTabs[3]);
                        break;
                    }
                    if (g_ccreate_guess->page == 2 || g_ccreate_guess->page == 4)
                    {
                        RECT arrows;
                        if (g_ccreate_guess->hoverButton == 0x5C)
                            CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3428), &g_ccreate_guess->leftArrow);
                        else
                            CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3420), &g_ccreate_guess->leftArrow);
                        if (g_ccreate_guess->hoverButton == 0x5D)
                            CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B342C), &g_ccreate_guess->rightArrow);
                        else
                            CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B3424), &g_ccreate_guess->rightArrow);
                        arrows.top = g_ccreate_guess->leftArrow.top;
                        arrows.left = g_ccreate_guess->acceptButton.left;
                        arrows.bottom = arrows.top + 0x1C;
                        arrows.right = arrows.left + 0x3C;
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32D0), &arrows);
                    }
                }
                else
                {
                    if (g_ccreate_guess->page > 1)
                        g_ccreate_guess->page = 1;
                    if (g_ccreate_guess->hoverButton == 0x2D)
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32E0), &g_ccreate_guess->acceptButton);
                    else
                        CCREATE_DRAW(EQ_GLOBAL(void*, 0x006B32DC), &g_ccreate_guess->acceptButton);
                }
            }
        }
        else
        {
            g_cdisplay_guess->time_ms_guess = EqGetTime();
        }

        switch (g_ccreate_guess->message)
        {
        case 1:
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F878), 0xB9, 0x181);
            break;
        case 2:
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F85C), 0xA5, 0x172);
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F838), 0x9B, 0x181);
            break;
        case 3:
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F818), 0x1E, 0x1B8);
            break;
        case 5:
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F7E0), 0x2A, 0x152);
            break;
        case 7:
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F7B0), 0x2A, 0x152);
            break;
        case 8:
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0056F75C));
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F738), 0x91, 0x172);
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F710), 0x8C, 0x181);
            break;
        case 9:
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0056F6B8));
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F694), 0x91, 0x172);
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F5E8), 0x8C, 0x181);
            break;
        case 10:
            strcpy((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x1B9, EQ_ARRAY(char, 0x0056F638));
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F610), 0x91, 0x172);
            CCREATE_TEXT(EQ_ARRAY(char, 0x0056F5E8), 0x8C, 0x181);
            break;
        }
        EQ_METHOD(0, void, 0x0043488C, (void*), EQ_GLOBAL(void*, 0x006E4E48))();
        EQ_METHOD(0, void, 0x00430831, (void*), g_char_select_guess)();
        if (EQ_GLOBAL(int, 0x006E5480) == 1)
            EQ_METHOD(0, void, 0x0040D5C2, (void*), g_display_guess)();
        EQ_METHOD(0, void, 0x0040E418, (void*), g_display_guess)();

        if (g_ccreate_guess->finished != 0)
        {
            g_ccreate_guess->pc->name[0] = 0;
            break;
        }
        if (g_ccreate_guess->message == 5 && EqGetTime() - g_ccreate_guess->requestTime > 45000)
        {
            g_ccreate_guess->message = 7;
            g_ccreate_guess->requestTime = EqGetTime();
        }
        if (g_ccreate_guess->message == 7 && EqGetTime() - g_ccreate_guess->requestTime > 3000)
        {
            g_ccreate_guess->awaitingApproval = 0;
            g_ccreate_guess->nameSubmitted = 0;
            g_ccreate_guess->message = 0;
            ((ScreenAction_guess)g_ccreate_guess->vtable[2])(g_ccreate_guess);
            g_ccreate_guess->nameEntryActive = 0;
            ((ScreenAction_guess)g_ccreate_guess->vtable[1])(g_ccreate_guess);
        }
        s16* message = &g_ccreate_guess->message;
        if (*message == 6)
        {
            for (i = 0; i < 96; i++)
                g_ccreate_guess->pc->spell_book[i] = -1;
            for (i = 0; i < 8; i++)
                g_ccreate_guess->pc->spell_memorized[i] = -1;
            if (g_cdisplay_guess->fastFileHandle6_guess != 0 &&
                (animSet = g_local_player_guess->actor_info->anim_set_guess) != 0 &&
                animSet[0x39C] == 1)
            {
                *((u8*)g_ccreate_guess->pc + 0x11BA) = g_ccreate_guess->features[2];
                *((u8*)g_ccreate_guess->pc + 0x11B6) = g_ccreate_guess->features[4];
                *((u8*)g_ccreate_guess->pc + 0x11BB) = g_ccreate_guess->features[3];
                *((u8*)g_ccreate_guess->pc + 0x11B7) = g_ccreate_guess->features[5];
                *((u8*)g_ccreate_guess->pc + 0x11B8) = g_ccreate_guess->features[0];
                *((u8*)g_ccreate_guess->pc + 0x11B9) = g_ccreate_guess->features[1];
            }
            g_ccreate_guess->pc->face = g_local_player_guess->face;
            g_ccreate_guess->pc->class_ = g_ccreate_guess->class_ + 1;
            g_ccreate_guess->pc->race = g_ccreate_guess->race + 1;
            g_ccreate_guess->pc->gender = g_ccreate_guess->gender;
            EQ_METHOD(2, void, 0x0041CB8B, (CharacterProfile*, u8, u16), g_ccreate_guess->pc)
                (g_ccreate_guess->class_ + 1, g_ccreate_guess->race + 1);
            EQ_METHOD(2, void, 0x0041D17E, (CharacterProfile*, u16, u16), g_ccreate_guess->pc)
                (g_ccreate_guess->race + 1, g_ccreate_guess->class_ + 1);
            g_ccreate_guess->pc->training_points = 5;
            EQ_METHOD(1, void, 0x0041997D, (CharacterProfile*, int), g_ccreate_guess->pc)(0);
            g_ccreate_guess->pc->unknown0x1044[0] = 0x7F;
            g_ccreate_guess->pc->thirst = 4500;
            g_ccreate_guess->pc->unknown0x1044[1] = 0x7F;
            g_ccreate_guess->pc->hunger = 4500;
            g_ccreate_guess->pc->fatigue = 0;
            g_ccreate_guess->pc->mana = 0;
            for (i = 0; i < 15; i++)
            {
                g_ccreate_guess->pc->buffs[i].visible = 0;
                g_ccreate_guess->pc->buffs[i].spell_id = -1;
                g_ccreate_guess->pc->buffs[i].level = 0;
                *(s16*)&g_ccreate_guess->pc->buffs[i].duration = 0;
                g_ccreate_guess->pc->buffs[i].bard_modifier = 0;
            }
            for (i = 0; i < 30; i++)
            {
                g_ccreate_guess->pc->inv_props[i].unknown0x00 = 0;
                *(s16*)&g_ccreate_guess->pc->inv_props[i].unknown0x03[1] = -1;
                g_ccreate_guess->pc->inv_props[i].unknown0x01 = 0;
                g_ccreate_guess->pc->inv_props[i].unknown0x06 = 0;
                g_ccreate_guess->pc->inv_props[i].charges = 0;
            }
            for (bag = 0; bag < 9; bag++)
            {
                for (slot = 0; slot < 10; slot++)
                {
                    EQ_ItemProperties* props = &g_ccreate_guess->pc->bag_props[bag * 10 + slot];
                    props->unknown0x00 = 0;
                    *(s16*)&props->unknown0x03[1] = -1;
                    props->unknown0x01 = 0;
                    props->unknown0x06 = 0;
                    props->charges = 0;
                }
            }
            for (i = 0; i < 10; i++)
                g_ccreate_guess->pc->spell_slot_refresh[i] = 0;
            EQ_METHOD(0, void, 0x0041A4FC, (CharacterProfile*), g_ccreate_guess->pc)();
            g_ccreate_guess->pc->deityId_guess = 0x18C;
            switch (g_ccreate_guess->deity)
            {
            case 0:
                g_ccreate_guess->pc->deityId_guess = 0x18C;
                break;
            case 1:
                g_ccreate_guess->pc->deityId_guess = 0xC9;
                break;
            case 2:
                g_ccreate_guess->pc->deityId_guess = 0xCD;
                break;
            case 3:
                g_ccreate_guess->pc->deityId_guess = 0xCB;
                break;
            case 4:
                g_ccreate_guess->pc->deityId_guess = 0xCE;
                break;
            case 5:
                g_ccreate_guess->pc->deityId_guess = 0xCF;
                break;
            case 6:
                g_ccreate_guess->pc->deityId_guess = 0xCC;
                break;
            case 7:
                g_ccreate_guess->pc->deityId_guess = 0xD0;
                break;
            case 8:
                g_ccreate_guess->pc->deityId_guess = 0xD4;
                break;
            case 9:
                g_ccreate_guess->pc->deityId_guess = 0xD1;
                break;
            case 10:
                g_ccreate_guess->pc->deityId_guess = 0xD2;
                break;
            case 11:
                g_ccreate_guess->pc->deityId_guess = 0xD5;
                break;
            case 12:
                g_ccreate_guess->pc->deityId_guess = 0xCA;
                break;
            case 13:
                g_ccreate_guess->pc->deityId_guess = 0xD6;
                break;
            case 14:
                g_ccreate_guess->pc->deityId_guess = 0xD7;
                break;
            case 15:
                g_ccreate_guess->pc->deityId_guess = 0xD8;
                break;
            case 16:
                g_ccreate_guess->pc->deityId_guess = 0xD3;
                break;
            }
            EQ_METHOD(2, void, 0x0047F019, (CharacterProfile*, u16, u8), g_ccreate_guess->pc)
                (g_ccreate_guess->race + 1, g_ccreate_guess->class_ + 1);
            send_message_guess(g_world_connection_guess, 0x2049, g_ccreate_guess->pc, 0x1FA4, 1);
            EQ_GLOBAL(int, 0x006B6490) = 1;
            g_ccreate_guess->pc->name[0] = 0;
            break;
        }
        if (*message < 5 && g_ccreate_guess->nameSubmitted != 0)
        {
            *message = 5;
            g_ccreate_guess->requestTime = EqGetTime();
            strcpy(request.name, g_ccreate_guess->pc->name);
            *(int*)&request.race = g_ccreate_guess->race + 1;
            *(int*)&request.class_ = g_ccreate_guess->class_ + 1;
            send_message_guess(g_world_connection_guess, 0x208B, &request, sizeof(request), 1);
            g_ccreate_guess->awaitingApproval = 1;
        }
        else
        {
            g_ccreate_guess->nameSubmitted = 0;
        }
        EQ_GLOBAL(SleepProc_guess, 0x00534198)(10);
    }

    pc = g_ccreate_guess->pc;
    if (pc != 0)
    {
        EQ_METHOD(0, void, 0x0047EF27, (CharacterProfile*), pc)();
        EQ_CDECL(void, 0x005218BB, (void*))(pc);
        g_ccreate_guess->pc = 0;
    }
    if (g_ccreate_guess != 0)
    {
        delete (CharCreateAlloc_guess*)g_ccreate_guess;
        g_ccreate_guess = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E74) != 0)
    {
        delete (CreateBS_guess*)EQ_GLOBAL(void*, 0x006E4E74);
        EQ_GLOBAL(void*, 0x006E4E74) = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E88) != 0)
    {
        delete (CreateBS6_guess*)EQ_GLOBAL(void*, 0x006E4E88);
        EQ_GLOBAL(void*, 0x006E4E88) = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E84) != 0)
    {
        delete (CreateBS5_guess*)EQ_GLOBAL(void*, 0x006E4E84);
        EQ_GLOBAL(void*, 0x006E4E84) = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E80) != 0)
    {
        delete (CreateBS4_guess*)EQ_GLOBAL(void*, 0x006E4E80);
        EQ_GLOBAL(void*, 0x006E4E80) = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E7C) != 0)
    {
        delete (CreateBS3_guess*)EQ_GLOBAL(void*, 0x006E4E7C);
        EQ_GLOBAL(void*, 0x006E4E7C) = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4E78) != 0)
    {
        delete (CreateBS2_guess*)EQ_GLOBAL(void*, 0x006E4E78);
        EQ_GLOBAL(void*, 0x006E4E78) = 0;
    }
}

#undef CCREATE_HEAP_OBJECT
#undef CCREATE_TEXT
#undef CCREATE_MUSIC
#undef CCREATE_CITY_CAN_START
#undef CCREATE_CAN_WORSHIP
#undef CCREATE_CLASS_MIN
#undef CCREATE_BASE_ATTR
#undef CCREATE_DRAW
#undef g_ccreate_guess

// CEverQuest as DoCharacterSelection sees it (the header declares no data members): the two state ints and
// the 10 character slots of OP_SendCharInfo at +0x22400.
class CEverQuest_DoCharacterSelection_guess
{
public:
    void DoCharacterSelection(int context, int flags);

    int     login_state;                /* 0x00000 set to 1 when leaving the select screen */
    int     select_state;               /* 0x00004 1 = selecting, 2 = back from creation, 3 = entering world */
    u8      unknown0x00008[0xBA0];      /* 0x00008 */
    char    unknown0x00BA8[0x21848];    /* 0x00BA8 a string, cleared */
    int     unknown0x223F0;             /* 0x223F0 cleared after character creation */
    int     disconnected;               /* 0x223F4 */
    u8      unknown0x223F8[8];          /* 0x223F8 */
    MsgCharacterSelect slots;           /* 0x22400 */
};

// The character select screen (0x006E4E6C), created here with operator new(0x248) + 0x0042FEAC.
class CharSelectScreen_guess : public EQ_CharSelS
{
public:
    static void* operator new(size_t size)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))((u32)size);
    }
    static void operator delete(void* storage)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(storage);
    }
    // EXTERNAL: TRILOGY 0x0042FEAC CharSelectScreen_guess::CharSelectScreen_guess
    CharSelectScreen_guess(void);

};

// The prompt next to it (0x006E4DF0), operator new(0x1D0) + 0x00482123.
class CharSelectPrompt_guess
{
public:
    static void* operator new(size_t size)
    {
        return EQ_CDECL(void*, 0x005218C6, (u32))((u32)size);
    }
    static void operator delete(void* storage)
    {
        EQ_CDECL(void, 0x005218BB, (void*))(storage);
    }
    // EXTERNAL: TRILOGY 0x00482123 CharSelectPrompt_guess::CharSelectPrompt_guess
    CharSelectPrompt_guess(void);

    u8      data[0x1D0];
};

typedef void (__fastcall *CharSelectVirtual_guess)(CharSelectScreen_guess*);
typedef DWORD (WINAPI *GetProfileStringProc_guess)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR);
typedef BOOL (WINAPI *WriteProfileStringProc_guess)(LPCSTR, LPCSTR, LPCSTR, LPCSTR);
typedef void (WINAPI *SleepProc_guess)(DWORD);

#define CHAR_SELECT_SCREEN      EQ_GLOBAL(CharSelectScreen_guess*, 0x006E4E6C)
#define CHAR_SELECT_SLOTS       (((CEverQuest_DoCharacterSelection_guess*)pinstCEverQuest_guess)->slots)
#define CHAR_SELECT_CAMERA      g_default_cameras_guess[g_activeView_guess]
#define READ_PROFILE            EQ_GLOBAL(GetProfileStringProc_guess, 0x0053418C)
#define WRITE_PROFILE           EQ_GLOBAL(WriteProfileStringProc_guess, 0x00534188)
#define SLEEP                   EQ_GLOBAL(SleepProc_guess, 0x00534198)
#define INI_SECTION             EQ_ARRAY(char, 0x0053DB3C)
#define INI_FILE                EQ_ARRAY(char, 0x0053DB50)
#define INI_LAST_CHAR_SEL       EQ_ARRAY(char, 0x0055E180)
#define INI_MISSING             EQ_ARRAY(char, 0x0053DB48)
#define EMPTY_TEXT              EQ_ARRAY(char, 0x0053D860)
#define NO_CHARACTER            EQ_ARRAY(char, 0x005454F0)
#define MAX_CHARACTERS          10

#pragma intrinsic(memset)

// FUNCTION: TRILOGY 0x004d2fb6
// Runs the character select screen (this = CEverQuest).
// Phases: reset session globals and pending-request counters, read the ini defaults, free old objects and create
// the select screen and prompt, load the preview player for the chosen slot.
// Then loops until done: connection check (a drop shows a disconnect notice and leaves), draw, and handling of
// the select state for enter world, delete, create and quit.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_DoCharacterSelection_guess::DoCharacterSelection(int context, int flags)
{
    short iteration = 0;
    int runMode;
    int i;
    short slot;
    short count;
    int model;
    int connected;
    EQPlayer* player;
    int disconnectInfo[3];
    char profileText[32];
    char text[128];
    char disconnectText[128];

    if (g_eqmain_guess != 0)
    {
        pinstEQ_Main_guess->pendingServerReplies_guess = 0;
        pinstEQ_Main_guess->pendingWorldRequests_guess = 0;
    }
    EQ_GLOBAL(u8, 0x006B6488) = 0;
    EQ_GLOBAL(int, 0x006B6480) = -1;
    EQ_GLOBAL(int, 0x006B6484) = -1;
    EQ_GLOBAL(u8, 0x006B6489) = 0;
    EQ_GLOBAL(u8, 0x006B648B) = 0;
    EQ_GLOBAL(u8, 0x006B648A) = 0;
    EQ_CDECL(void, 0x004E878C, (void))();
    EQ_GLOBAL(int, 0x006B70F0) = 0;
    EQ_GLOBAL(int, 0x006B70EC) = 0;
    EQ_GLOBAL(int, 0x006B70E8) = 0;
    EQ_GLOBAL(int, 0x006B63EC) = 0;
    EQ_GLOBAL(int, 0x006B70E4) = 0;
    EQ_GLOBAL(u8, 0x006B645A) = 0;
    EQ_GLOBAL(u8, 0x006B6770) = 0;
    EQ_GLOBAL(short, 0x006B6772) = -1;
    EQ_GLOBAL(int, 0x006B6774) = 0;
    EQ_GLOBAL(int, 0x006B6778) = 0;
    EQ_GLOBAL(int, 0x006B677C) = 0;
    EQ_GLOBAL(int, 0x006B6780) = 0;
    EQ_GLOBAL(short, 0x006B6784) = 0;
    EQ_GLOBAL(int, 0x006B6788) = 0;
    strcpy(EQ_ARRAY(char, 0x006B678C), EQ_ARRAY(char, 0x0058FBA4));
    EQ_GLOBAL(int, 0x006B67AC) = 0;
    EQ_GLOBAL(short, 0x006B67B0) = -1;
    strcpy(EQ_ARRAY(char, 0x006B7128), EQ_ARRAY(char, 0x0056A880));
    EQ_GLOBAL(short, 0x006B7528) = 1;
    EQ_GLOBAL(int, 0x006B648C) = -1;
    EQ_GLOBAL(int, 0x006B676C) = 0;
    EQ_GLOBAL(u8, 0x006B6464) = 0;
    strcpy(EQ_ARRAY(char, 0x006B64A8), EQ_ARRAY(char, 0x0058FBA4));
    EQ_GLOBAL(u8, 0x006B6465) = 0;
    EQ_GLOBAL(int, 0x006B6764) = 0;
    EQ_GLOBAL(u8, 0x006B65D8) = 0;
    EQ_GLOBAL(short, 0x006B65B6) = 0;
    EQ_GLOBAL(u8, 0x006B6404) = 1;
    EQ_GLOBAL(int, 0x006B65BC) = 0;
    EQ_GLOBAL(int, 0x006B65C0) = 0;
    EQ_GLOBAL(int, 0x006B65C4) = 0;
    EQ_GLOBAL(int, 0x006B65C8) = 0;
    EQ_GLOBAL(int, 0x006B65CC) = 0;
    EQ_GLOBAL(int, 0x006B65D0) = 0;
    EQ_GLOBAL(int, 0x006B65D4) = 0;
    EQ_GLOBAL(int, 0x006B7124) = 0;
    EQ_GLOBAL(int, 0x006B6594) = 0;
    EQ_GLOBAL(int, 0x006B6590) = 0;
    EQ_GLOBAL(int, 0x006B65A8) = 0;
    EQ_GLOBAL(int, 0x006B65AC) = 0;
    EQ_GLOBAL(int, 0x006B65B0) = 0;
    EQ_GLOBAL(u8, 0x006B7111) = 0;
    EQ_GLOBAL(int, 0x006B6754) = 0;
    EQ_GLOBAL(short, 0x006B6752) = 0;
    EQ_GLOBAL(u8, 0x006B7110) = 0;
    EQ_GLOBAL(int, 0x006B70E8) = 0;
    EQ_GLOBAL(int, 0x006B70EC) = 0;
    g_attacking_guess = 0;
    g_rangedAttacking_guess = 0;
    READ_PROFILE(INI_SECTION, EQ_ARRAY(char, 0x0056FB28), INI_MISSING, profileText, sizeof(profileText), INI_FILE);
    if (profileText[0] == '*')
    {
        runMode = 0;
        WRITE_PROFILE(INI_SECTION, EQ_ARRAY(char, 0x0056FB28), EQ_ARRAY(char, 0x0053DAFC), INI_FILE);
    }
    else if (profileText[0] == 'T' || profileText[0] == 't' || profileText[0] == '1')
        runMode = 1;
    else
        runMode = 0;
    EQ_GLOBAL(u8, 0x006B6405) = (u8)runMode;
    EQ_GLOBAL(int, 0x006B64F0) = 0x26;
    EQ_GLOBAL(int, 0x006B64F4) = 0x26;
    EQ_GLOBAL(short, 0x006B70FC) = 0;
    EQ_GLOBAL(short, 0x006B70FE) = 0;
    EQ_GLOBAL(short, 0x006B7100) = 0;
    EQ_GLOBAL(short, 0x006B7102) = 0;
    EQ_GLOBAL(short, 0x006B7104) = 0;
    EQ_GLOBAL(short, 0x006B7106) = 0;
    EQ_GLOBAL(short, 0x006B7116) = 0;
    EQ_GLOBAL(short, 0x006B7118) = 0;
    EQ_GLOBAL(int, 0x006B64DC) = 0;
    EQ_GLOBAL(int, 0x006B64E0) = 0;
    EQ_GLOBAL(u8, 0x006B710E) = 0;
    EQ_GLOBAL(u8, 0x006B710F) = 0;
    EQ_GLOBAL(u8, 0x006B6854) = 0;
    EQ_GLOBAL(u8, 0x006B6408) = 0;
    EQ_GLOBAL(int, 0x006B64E4) = 0x96;
    EQ_GLOBAL(int, 0x006B64E8) = 0;
    EQ_GLOBAL(int, 0x006B6474) = 0;
    EQ_GLOBAL(int, 0x006B647C) = 0;
    EQ_GLOBAL(int, 0x006B6478) = 0;
    EQ_GLOBAL(int, 0x006B64EC) = 0x4B;
    EQ_GLOBAL(int, 0x006E2054) = 0;
    EQ_GLOBAL(int, 0x006B64F8) = 0x52;
    EQ_GLOBAL(int, 0x006B64FC) = 0x37;
    EQ_GLOBAL(int, 0x006B6500) = 0xA4;
    EQ_GLOBAL(int, 0x006B6768) = 0;
    if (EQ_GLOBAL(float*, 0x006E4DDC) != 0)
    {
        EQ_GLOBAL(float*, 0x006E4DDC)[2] = 0.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[0] = 1.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[1] = 0.0f;
        ((int*)EQ_GLOBAL(float*, 0x006E4DDC))[4] = -1;
    }
    for (i = 0; i < 0x24; i++)
    {
        if (EQ_ARRAY(void*, 0x006B67B4)[i] != 0)
        {
            EQ_CDECL(void, 0x005218BB, (void*))(EQ_ARRAY(void*, 0x006B67B4)[i]);
            EQ_ARRAY(void*, 0x006B67B4)[i] = 0;
        }
    }
    memset(EQ_ARRAY(u32, 0x006B6844), 0, 16);
    strcpy(unknown0x00BA8, EQ_ARRAY(char, 0x0058FBA4));
    EQ_GLOBAL(short, 0x006E0FE8) = -1;
    EQ_GLOBAL(int, 0x006B6468) = 0;
    EQ_GLOBAL(int, 0x006B646C) = 0;
    EQ_GLOBAL(float, 0x006B711C) = 0.0f;
    EQ_GLOBAL(int, 0x006B6470) = 0;
    EQ_GLOBAL(int, 0x006B6508) = 0xB;
    EQ_GLOBAL(int, 0x006B6498) = 0;
    EQ_GLOBAL(int, 0x006B64A0) = 0;
    EQ_GLOBAL(int, 0x006B649C) = 0;
    EQ_GLOBAL(int, 0x006B64A4) = 0;
    EQ_GLOBAL(int, 0x006B64C8) = 0;
    EQ_GLOBAL(int, 0x006B64CC) = 0;
    EQ_GLOBAL(int, 0x006B64D4) = 0;
    EQ_GLOBAL(u8, 0x006B64D0) = 0;
    EQ_GLOBAL(u8, 0x006E1DE4) = 0;
    EQ_GLOBAL(u8, 0x006E1DE5) = 0;
    EQ_GLOBAL(int, 0x006B65A4) = 0;
    EQ_GLOBAL(u8, 0x006E1DE7) = 0;
    EQ_GLOBAL(u8, 0x006E1DE8) = 0;
    for (i = 0; i < 20; i++)
        EQ_ARRAY(u8, 0x006D6FE8)[i * 0x800] = 0;
    EQ_GLOBAL(int, 0x006D6FDC) = 0;
    for (i = 0; i < 500; i++)
    {
        sprintf(EQ_ARRAY(char, 0x006B772B) + i * 0x100, EQ_ARRAY(char, 0x0055D6D4));
        EQ_ARRAY(short, 0x006D6BF4)[i] = 0;
    }
    if (EQ_GLOBAL(void*, 0x006E4DE8) != 0)
        EQ_METHOD(0, void, 0x0048CF22, (void*), EQ_GLOBAL(void*, 0x006E4DE8))();
    for (i = 0; i < 25; i++)
    {
        EQ_ARRAY(char*, 0x006D6B2C)[i] = EQ_ARRAY(char, 0x006B772B) + i * 0x100;
        EQ_ARRAY(short*, 0x006D6B90)[i] = EQ_ARRAY(short, 0x006D6BF4) + i;
    }
    EQ_GLOBAL(short, 0x006E0FEA) = 0;
    memset(EQ_ARRAY(u32, 0x006E19E4), 0, 0x400);
    for (i = 0; i < 100; i++)
    {
        EQ_ARRAY(u8, 0x006E117C)[i] = 1;
        EQ_ARRAY(u32, 0x006E0FEC)[i] = 0;
    }
    EQ_GLOBAL(u8, 0x006B645C) = 1;
    EQ_GLOBAL(u8, 0x006B645D) = 0;
    EQ_GLOBAL(u8, 0x006B658E) = 0;
    EQ_GLOBAL(u8, 0x006B658D) = 0;
    EQ_GLOBAL(u8, 0x006B658C) = 1;
    EQ_GLOBAL(u8, 0x006B6428) = 8;
    EQ_GLOBAL(short, 0x006B642A) = 0;
    EQ_GLOBAL(u8, 0x006B642C) = 0;
    for (i = 0; i < 10; i++)
    {
        EQ_ARRAY(u32, 0x006B6538)[i] = 0;
        EQ_ARRAY(u32, 0x006B6560)[i] = 0;
    }
    EQ_GLOBAL(int, 0x006B6588) = 0;
    EQ_GLOBAL(int, 0x006B652C) = 0;
    EQ_GLOBAL(int, 0x006B6534) = 0;
    EQ_GLOBAL(int, 0x006B6530) = 0;
    EQ_GLOBAL(int, 0x006B6450) = 0;
    EQ_GLOBAL(int, 0x006B6454) = 0;
    g_attacking_guess = 0;
    g_rangedAttacking_guess = 0;
    EQ_GLOBAL(u8, 0x006B6404) = 0;
    EQ_GLOBAL(u8, 0x006E1E08) = 1;
    EQ_GLOBAL(u8, 0x006B6406) = 1;
    g_name_buffer_guess[0] = 0;
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0)
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0056FB14));
        if (g_sound_manager_ptr_guess != 0)
            EQ_METHOD(1, int, 0x004E36D1, (void*, int), g_sound_manager_ptr_guess)(0);
        DbgLog_guess(EQ_ARRAY(char, 0x0056FAF0));
        if (g_sound_manager_ptr_guess != 0)
            EQ_METHOD(0, void, 0x004E38D0, (void*), g_sound_manager_ptr_guess)();
    }
    for (i = 0; i < 5; i++)
    {
        EQ_ARRAY(u8, 0x005FE8B8)[i] = 0;
        g_death_state_guess[i] = 0;
        EQ_ARRAY(u8, 0x005FE8BD)[i * 30] = 0;
    }
    EQ_GLOBAL(float, 0x006B66D0) = 700.0f;
    EQ_GLOBAL(float, 0x006B66E0) = 800.0f;
    EQ_GLOBAL(u8, 0x006B66C3) = 1;
    EQ_GLOBAL(u8, 0x006B66C7) = 1;
    EQ_GLOBAL(u8, 0x006B66CB) = 1;
    DbgLog_guess(EQ_ARRAY(char, 0x0056FAD8));
    if (g_display_guess == 0)
        DbgLog_guess(EQ_ARRAY(char, 0x0056FA94));
    EQ_METHOD(0, void, 0x004124DB, (void*), g_display_guess)();
    EQ_GLOBAL(u8, 0x006B6750) = 1;
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA7C));
    EQ_METHOD(0, void, 0x004121CA, (void*), g_display_guess)();
    EQ_METHOD(0, void, 0x00412233, (void*), g_display_guess)();
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA64));
    EQ_METHOD(1, void, 0x0040E4C6, (void*, int), g_display_guess)(90);
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA4C));
    EQ_METHOD(0, void, 0x0040F6FE, (void*), g_display_guess)();
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA34));
    if (CHAR_SELECT_SCREEN == 0)
        CHAR_SELECT_SCREEN = new CharSelectScreen_guess;
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA1C));
    if (EQ_GLOBAL(CharSelectPrompt_guess*, 0x006E4DF0) == 0)
        EQ_GLOBAL(CharSelectPrompt_guess*, 0x006E4DF0) = new CharSelectPrompt_guess;
    DbgLog_guess(EQ_ARRAY(char, 0x0056FA04));
    *((u8*)g_display_guess + 0x3C) = 0;
    *(int*)(EQ_GLOBAL(char*, 0x006E4E48) + 0x10) = 1;
    if (CHAR_SELECT_SCREEN == 0)
        EQTRACE_guess(EQ_ARRAY(char, 0x0056F9F0));
    if (select_state == 1 && EQ_GLOBAL(int, 0x006B61B4) == 0)
    {
        READ_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, INI_MISSING, profileText, sizeof(profileText), INI_FILE);
        if (profileText[0] == '*')
        {
            CHAR_SELECT_SCREEN->selectedCharacter = 0;
            WRITE_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, EMPTY_TEXT, INI_FILE);
        }
        else
        {
            CHAR_SELECT_SCREEN->selectedCharacter = (short)atoi(profileText);
            if (CHAR_SELECT_SCREEN->selectedCharacter < 0 || CHAR_SELECT_SCREEN->selectedCharacter > 7)
            {
                CHAR_SELECT_SCREEN->selectedCharacter = 0;
                WRITE_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, EMPTY_TEXT, INI_FILE);
            }
        }
        if (CHAR_SELECT_SCREEN->selectedCharacter != -1 &&
            strcmp(slots.name[CHAR_SELECT_SCREEN->selectedCharacter], NO_CHARACTER) == 0)
            CHAR_SELECT_SCREEN->selectedCharacter = -1;
    }
    EQ_CDECL(void, 0x004EB516, (void))();
    EQ_CDECL(void, 0x004EB0B1, (void))();
    DbgLog_guess(EQ_ARRAY(char, 0x0056F9D8));
    if (CHAR_SELECT_SCREEN->selectedCharacter >= 0)
    {
        g_local_player_guess->appearance[5] = CHAR_SELECT_SLOTS.appearance[5][CHAR_SELECT_SCREEN->selectedCharacter];
        g_local_player_guess->appearance[1] = CHAR_SELECT_SLOTS.appearance[1][CHAR_SELECT_SCREEN->selectedCharacter];
        g_local_player_guess->appearance[4] = CHAR_SELECT_SLOTS.appearance[4][CHAR_SELECT_SCREEN->selectedCharacter];
        g_local_player_guess->appearance[0] = CHAR_SELECT_SLOTS.appearance[0][CHAR_SELECT_SCREEN->selectedCharacter];
        g_local_player_guess->appearance[2] = CHAR_SELECT_SLOTS.appearance[2][CHAR_SELECT_SCREEN->selectedCharacter];
        g_local_player_guess->appearance[3] = CHAR_SELECT_SLOTS.appearance[3][CHAR_SELECT_SCREEN->selectedCharacter];
        EQ_METHOD(4, void, 0x00430381, (CharSelectScreen_guess*, u8, short, int, u8), CHAR_SELECT_SCREEN)(
            slots.gender[CHAR_SELECT_SCREEN->selectedCharacter], (short)(slots.race[CHAR_SELECT_SCREEN->selectedCharacter] - 1), 0,
            slots.class_[CHAR_SELECT_SCREEN->selectedCharacter]);
        for (i = 0; i < 9; i++)
        {
            g_local_player_guess->equipment[i] = 0;
            g_local_player_guess->equip_colors[i] = 0;
        }
        for (slot = 0; slot < 9; slot++)
        {
            if (slot == 0)
                EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, u32, int), g_display_guess)(
                    g_local_player_guess, CHAR_SELECT_SLOTS.equipment[CHAR_SELECT_SCREEN->selectedCharacter][0],
                    g_local_player_guess->equipment[0], 0,
                    CHAR_SELECT_SLOTS.equip_colors[CHAR_SELECT_SCREEN->selectedCharacter][0], 0);
            else
                EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, u32), g_display_guess)(
                    g_local_player_guess, slot, CHAR_SELECT_SLOTS.equipment[CHAR_SELECT_SCREEN->selectedCharacter][slot],
                    g_local_player_guess->equipment[slot],
                    CHAR_SELECT_SLOTS.equip_colors[CHAR_SELECT_SCREEN->selectedCharacter][slot]);
        }
        EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int), g_display_guess)(
            g_local_player_guess, CHAR_SELECT_SLOTS.face[CHAR_SELECT_SCREEN->selectedCharacter], g_local_player_guess->face);
        model = CHAR_SELECT_SLOTS.primary_model[CHAR_SELECT_SCREEN->selectedCharacter];
        if (model > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0053D7D4), model);
            EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                g_local_player_guess, 7, text);
        }
        else
            EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                g_local_player_guess, 7, EMPTY_TEXT);
        model = CHAR_SELECT_SLOTS.secondary_model[CHAR_SELECT_SCREEN->selectedCharacter];
        if (model > 0)
        {
            sprintf(text, EQ_ARRAY(char, 0x0053D7D4), model);
            EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                g_local_player_guess, 8, text);
        }
        else
            EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                g_local_player_guess, 8, EMPTY_TEXT);
        if (EQ_METHOD(3, bool, 0x00431509, (CharSelectScreen_guess*, int, int, int), CHAR_SELECT_SCREEN)(
                (u16)CHAR_SELECT_SLOTS.race[CHAR_SELECT_SCREEN->selectedCharacter],
                CHAR_SELECT_SLOTS.class_[CHAR_SELECT_SCREEN->selectedCharacter],
                CHAR_SELECT_SLOTS.deity[CHAR_SELECT_SCREEN->selectedCharacter]))
        {
            g_local_player_guess->y = 0.0f;
            g_local_player_guess->x = -315.0f;
            g_local_player_guess->z = 6.0f;
        }
        else
        {
            g_local_player_guess->y = 0.0f;
            g_local_player_guess->x = 0.0f;
            g_local_player_guess->z = 7.0f;
        }
        CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
    }
    EQ_METHOD(0, void, 0x0040D59A, (void*), g_display_guess)();
    for (;;)
    {
        if (select_state != 1)
        {
            select_state = 0xFD;
            return;
        }
        EQ_GLOBAL(u8, 0x006B645A) = 0;
        EQ_CDECL(void, 0x004C2C1A, (void))();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9D0));
        EQ_CDECL(void, 0x004EB0E0, (void))();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9C8));
        CEverQuest_ProcessMBox_guess(this);
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9C0));
        if (g_world_connection_guess[0] != 0)
            connected = EQ_CDECL(int, 0x004F23B0, (void*))(g_world_connection_guess[0]);
        else
            connected = 0;
        if (connected == 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0056F90C));
            if (g_world_connection_guess[0] != 0)
            {
                EQ_CDECL(void, 0x004F2490, (void*, int*, int))(g_world_connection_guess[0], disconnectInfo,
                    sizeof(disconnectInfo));
                sprintf(disconnectText, EQ_ARRAY(char, 0x0056F5DC), disconnectInfo[0]);
            }
            else
                strcpy(disconnectText, EQ_ARRAY(char, 0x0056F5CC));
            DbgLog_guess(EQ_ARRAY(char, 0x0056F8E0), EQ_ARRAY(char, 0x0056F5A4), 0x3BCF, disconnectText);
            disconnected = 1;
            EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)(EQ_ARRAY(char, 0x0056F574));
            SLEEP(3000);
            login_state = 1;
            select_state = 0xFF;
            EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
            return;
        }
        if (EQ_GLOBAL(int, 0x006B6864) != 0 && CHAR_SELECT_SCREEN != 0)
        {
            if (EQ_GLOBAL(short, 0x006B6418) == 0 && EQ_GLOBAL(short, 0x006B641A) == 0)
            {
                if (EQ_CDECL(u32, 0x004A461E, (void))() - EQ_GLOBAL(u32, 0x006B64D4) > 500)
                {
                    EQ_GLOBAL(u32, 0x006B64D4) = EQ_CDECL(u32, 0x004A461E, (void))();
                    if (CHAR_SELECT_SCREEN->resetValue < 3)
                        CHAR_SELECT_SCREEN->resetValue++;
                }
            }
            else
                CHAR_SELECT_SCREEN->resetValue = 0;
        }
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9B8));
        EQ_METHOD(0, void, 0x00434806, (void*), EQ_GLOBAL(void*, 0x006E4E48))();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9B0));
        EQ_METHOD(0, void, 0x00430D81, (CharSelectScreen_guess*), CHAR_SELECT_SCREEN)();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9A8));
        EQ_METHOD(0, void, 0x0048226C, (void*), EQ_GLOBAL(void*, 0x006E4DF0))();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F9A0));
        CHAR_SELECT_CAMERA.value[1] = EQ_METHOD(1, float, 0x00408156, (void*, float), g_display_guess)(
            CHAR_SELECT_CAMERA.value[1] + 3.0f);
        if (EQ_GLOBAL(int, 0x006E5480) == 1)
        {
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F994), EQ_GLOBAL(void*, 0x006E4E48));
            EQ_METHOD(0, void, 0x0043488C, (void*), EQ_GLOBAL(void*, 0x006E4E48))();
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F98C));
            EQ_METHOD(0, void, 0x00430831, (CharSelectScreen_guess*), CHAR_SELECT_SCREEN)();
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F980));
            if (CHAR_SELECT_SCREEN->selectedCharacter != -1)
            {
                strcpy(text, EQ_METHOD(1, char*, 0x004C9D4E, (CEverQuest*, short), pinstCEverQuest_guess)(
                    CHAR_SELECT_SLOTS.class_[CHAR_SELECT_SCREEN->selectedCharacter]));
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(
                    text, 0x1E4 - (short)(strlen(text) * 9 / 2), 0x168, 10, EQ_GLOBAL(int, 0x006EFE08), 1);
                sprintf(text, EQ_ARRAY(char, 0x0056F974), CHAR_SELECT_SLOTS.level[CHAR_SELECT_SCREEN->selectedCharacter]);
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(
                    text, 0x1E9 - (short)(strlen(text) * 9 / 2), 0x17C, 10, EQ_GLOBAL(int, 0x006EFE08), 1);
            }
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F968));
            EQ_METHOD(0, void, 0x0040D5C2, (void*), g_display_guess)();
        }
        else
        {
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F960));
            EQ_METHOD(0, void, 0x0040D5C2, (void*), g_display_guess)();
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F98C));
            if (CHAR_SELECT_SCREEN->selectedCharacter != -1)
            {
                strcpy(text, EQ_METHOD(1, char*, 0x004C9D4E, (CEverQuest*, short), pinstCEverQuest_guess)(
                    CHAR_SELECT_SLOTS.class_[CHAR_SELECT_SCREEN->selectedCharacter]));
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(
                    text, 0x1E4 - (short)(strlen(text) * 9 / 2), 0x168, 10, EQ_GLOBAL(int, 0x006EFE08), 1);
                sprintf(text, EQ_ARRAY(char, 0x0056F974), CHAR_SELECT_SLOTS.level[CHAR_SELECT_SCREEN->selectedCharacter]);
                EQ_METHOD(6, void, 0x0048D21A, (void*, char*, int, int, int, int, int), EQ_GLOBAL(void*, 0x006E4E68))(
                    text, 0x1E9 - (short)(strlen(text) * 9 / 2), 0x17C, 10, EQ_GLOBAL(int, 0x006EFE08), 1);
            }
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F954), EQ_GLOBAL(void*, 0x006E4E48));
            EQ_METHOD(0, void, 0x0043488C, (void*), EQ_GLOBAL(void*, 0x006E4E48))();
            if (iteration < 1)
                DbgLog_guess(EQ_ARRAY(char, 0x0056F968));
            EQ_METHOD(0, void, 0x00430831, (CharSelectScreen_guess*), CHAR_SELECT_SCREEN)();
        }
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F948));
        EQ_METHOD(0, void, 0x0040E418, (void*), g_display_guess)();
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F93C));
        if (CHAR_SELECT_SCREEN->enter_world != 0 && CHAR_SELECT_SCREEN->selectionCode == -1 &&
            CHAR_SELECT_SCREEN->selectedCharacter != -1)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0056F930));
            CHAR_SELECT_SCREEN->enter_world = 0;
            if (EQ_METHOD(0, short, 0x004307FC, (CharSelectScreen_guess*), CHAR_SELECT_SCREEN)() > 0)
            {
                DbgLog_guess(EQ_ARRAY(char, 0x0056F8D4));
                sprintf((char*)disconnectInfo, EQ_ARRAY(char, 0x0053E16C), CHAR_SELECT_SCREEN->selectedCharacter);
                WRITE_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, (char*)disconnectInfo, INI_FILE);
                send_message_guess(g_world_connection_guess, 0x8001,
                    slots.name[CHAR_SELECT_SCREEN->selectedCharacter], strlen(slots.name[CHAR_SELECT_SCREEN->selectedCharacter]) + 1, 1);
                select_state = 3;
                return;
            }
        }
        if (CHAR_SELECT_SCREEN->enter_world != 0)
            CHAR_SELECT_SCREEN->enter_world = 0;
        if (CHAR_SELECT_SCREEN->quit != 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0056F8C0));
            login_state = 1;
            select_state = 0xFF;
            CHAR_SELECT_SCREEN->quit = 0;
            send_message_guess(g_world_connection_guess, 0x2023, 0, 0, 1);
            return;
        }
        if (CHAR_SELECT_SCREEN->delete_selected != 0 && CHAR_SELECT_SCREEN->selectedCharacter >= 0 &&
            CHAR_SELECT_SCREEN->selectionCode == -1)
        {
            CHAR_SELECT_SCREEN->delete_selected = 0;
            EQ_METHOD(0, void, 0x004D2F41, (void*), this)();
            for (count = 0; count < MAX_CHARACTERS; count++)
            {
                if (strcmp(slots.name[count], NO_CHARACTER) == 0)
                    break;
            }
            CHAR_SELECT_SCREEN->selectedCharacter = count - 1;
        }
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F924));
        if (CHAR_SELECT_SCREEN->emptySelection != 0 &&
            EQ_METHOD(0, short, 0x004307FC, (CharSelectScreen_guess*), CHAR_SELECT_SCREEN)() < MAX_CHARACTERS &&
            CHAR_SELECT_SCREEN->selectionCode == -1)
        {
            CHAR_SELECT_SCREEN->emptySelection = 0;
            ((CharSelectVirtual_guess)CHAR_SELECT_SCREEN->vtable[2])(CHAR_SELECT_SCREEN);
            g_game_state_guess = 6;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(1);
            EQ_METHOD(2, void, 0x004D1166, (void*, int, int), this)(context, flags);
            player = g_local_player_guess;
            if (player->race == 10 || player->race == 9 || player->race == 0x80)
                CHAR_SELECT_CAMERA.value[0] = 7.0f;
            else
                CHAR_SELECT_CAMERA.value[0] = 6.0f;
            CHAR_SELECT_CAMERA.value[2] = -(*(float*)&player->unk_0x78 * 0.33f);
            READ_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, INI_MISSING, profileText, sizeof(profileText), INI_FILE);
            if (profileText[0] == '*')
            {
                CHAR_SELECT_SCREEN->selectedCharacter = 0;
                WRITE_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, EMPTY_TEXT, INI_FILE);
            }
            else
            {
                CHAR_SELECT_SCREEN->selectedCharacter = (short)atoi(profileText);
                if (CHAR_SELECT_SCREEN->selectedCharacter < 0 || CHAR_SELECT_SCREEN->selectedCharacter > 7)
                {
                    CHAR_SELECT_SCREEN->selectedCharacter = 0;
                    WRITE_PROFILE(INI_SECTION, INI_LAST_CHAR_SEL, EMPTY_TEXT, INI_FILE);
                }
            }
            if (CHAR_SELECT_SCREEN->selectedCharacter != -1 &&
                strcmp(slots.name[CHAR_SELECT_SCREEN->selectedCharacter], NO_CHARACTER) == 0)
                CHAR_SELECT_SCREEN->selectedCharacter = -1;
            if (CHAR_SELECT_SCREEN->selectedCharacter >= 0)
            {
                g_local_player_guess->appearance[5] = CHAR_SELECT_SLOTS.appearance[5][CHAR_SELECT_SCREEN->selectedCharacter];
                g_local_player_guess->appearance[1] = CHAR_SELECT_SLOTS.appearance[1][CHAR_SELECT_SCREEN->selectedCharacter];
                g_local_player_guess->appearance[4] = CHAR_SELECT_SLOTS.appearance[4][CHAR_SELECT_SCREEN->selectedCharacter];
                g_local_player_guess->appearance[0] = CHAR_SELECT_SLOTS.appearance[0][CHAR_SELECT_SCREEN->selectedCharacter];
                g_local_player_guess->appearance[2] = CHAR_SELECT_SLOTS.appearance[2][CHAR_SELECT_SCREEN->selectedCharacter];
                g_local_player_guess->appearance[3] = CHAR_SELECT_SLOTS.appearance[3][CHAR_SELECT_SCREEN->selectedCharacter];
                EQ_METHOD(4, void, 0x00430381, (CharSelectScreen_guess*, u8, short, int, u8), CHAR_SELECT_SCREEN)(
                    slots.gender[CHAR_SELECT_SCREEN->selectedCharacter], (short)(slots.race[CHAR_SELECT_SCREEN->selectedCharacter] - 1),
                    0, slots.class_[CHAR_SELECT_SCREEN->selectedCharacter]);
                for (i = 0; i < 9; i++)
                {
                    g_local_player_guess->equipment[i] = 0;
                    g_local_player_guess->equip_colors[i] = 0;
                }
                for (slot = 0; slot < 9; slot++)
                {
                    if (slot == 0)
                        EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, u32, int), g_display_guess)(
                            g_local_player_guess, CHAR_SELECT_SLOTS.equipment[CHAR_SELECT_SCREEN->selectedCharacter][0],
                            g_local_player_guess->equipment[0], 0,
                            CHAR_SELECT_SLOTS.equip_colors[CHAR_SELECT_SCREEN->selectedCharacter][0], 0);
                    else
                        EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, u32), g_display_guess)(
                            g_local_player_guess, slot,
                            CHAR_SELECT_SLOTS.equipment[CHAR_SELECT_SCREEN->selectedCharacter][slot],
                            g_local_player_guess->equipment[slot],
                            CHAR_SELECT_SLOTS.equip_colors[CHAR_SELECT_SCREEN->selectedCharacter][slot]);
                }
                EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int), g_display_guess)(
                    g_local_player_guess, CHAR_SELECT_SLOTS.face[CHAR_SELECT_SCREEN->selectedCharacter],
                    g_local_player_guess->face);
                model = CHAR_SELECT_SLOTS.primary_model[CHAR_SELECT_SCREEN->selectedCharacter];
                if (model > 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0053D7D4), model);
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        g_local_player_guess, 7, text);
                }
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        g_local_player_guess, 7, EMPTY_TEXT);
                model = CHAR_SELECT_SLOTS.secondary_model[CHAR_SELECT_SCREEN->selectedCharacter];
                if (model > 0)
                {
                    sprintf(text, EQ_ARRAY(char, 0x0053D7D4), model);
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        g_local_player_guess, 8, text);
                }
                else
                    EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, const char*), g_display_guess)(
                        g_local_player_guess, 8, EMPTY_TEXT);
                if (EQ_METHOD(3, bool, 0x00431509, (CharSelectScreen_guess*, int, int, int), CHAR_SELECT_SCREEN)(
                        (u16)CHAR_SELECT_SLOTS.race[CHAR_SELECT_SCREEN->selectedCharacter],
                        CHAR_SELECT_SLOTS.class_[CHAR_SELECT_SCREEN->selectedCharacter],
                        CHAR_SELECT_SLOTS.deity[CHAR_SELECT_SCREEN->selectedCharacter]))
                {
                    g_local_player_guess->y = 0.0f;
                    g_local_player_guess->x = -315.0f;
                    g_local_player_guess->z = 6.0f;
                }
                else
                {
                    g_local_player_guess->y = 0.0f;
                    g_local_player_guess->x = 0.0f;
                    g_local_player_guess->z = 7.0f;
                }
                CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
            }
            unknown0x223F0 = 0;
            if (select_state == 2)
                select_state = 1;
            g_game_state_guess = 5;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(1);
            ((CharSelectVirtual_guess)CHAR_SELECT_SCREEN->vtable[1])(CHAR_SELECT_SCREEN);
        }
        if (iteration < 1)
            DbgLog_guess(EQ_ARRAY(char, 0x0056F918));
        SLEEP(10);
        iteration++;
    }
}

#pragma function(memset)
#undef CHAR_SELECT_SCREEN
#undef CHAR_SELECT_SLOTS
#undef CHAR_SELECT_CAMERA
#undef DbgLog_guess
#undef sprintf
#undef strcpy
#undef strlen
#undef strcmp
#undef atoi
#undef READ_PROFILE
#undef WRITE_PROFILE
#undef SLEEP
#undef INI_SECTION
#undef INI_FILE
#undef INI_LAST_CHAR_SEL
#undef INI_MISSING
#undef EMPTY_TEXT
#undef NO_CHARACTER
#undef MAX_CHARACTERS

// FUNCTION: TRILOGY 0x004d4676
// Starts world login and initializes the game display and overlays.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall, ECX = `this`; the int at this+0 is the login state. No class declares this method, so it is a
// __fastcall free function taking this in ECX (the EDX slot is unused). Clears the byte at 0x006B63F8 and
// authenticates with the world server into g_world_connection_guess (0x006B3194). Without a connection a state of 1
// returns -1 and a state of 2 sets it to 1 and returns -2; anything else goes on. With a connection the player name
// is read into a 32-byte buffer and copied to 0x00604904; when that read fails the connection is shut down, cleared
// and -1 is returned. Otherwise the window is shown and updated, the display (0x0040940F on the object at 0x006E4DE0)
// and the overlays are started, both steps are logged, and -1 is returned.
int __fastcall CEverQuest_DoLogin_guess(int* self, int unusedEdx, HWND hwnd, void* instance)
{
    typedef BOOL (WINAPI *ShowWindowProc_guess)(HWND, int);
    typedef BOOL (WINAPI *UpdateWindowProc_guess)(HWND);
    int nameReadResult;
    char playerName[32];

    EQ_GLOBAL(u8, 0x006B63F8) = 0;
    EQ_GLOBAL(void*, 0x006B3194) = EQ_CDECL(void*, 0x004E013C, (void*, void*))(hwnd, instance);
    if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
    {
        nameReadResult = EQ_CDECL(int, 0x004E041F, (char*))(playerName);
        strcpy(EQ_ARRAY(char, 0x00604904), playerName);
        if (nameReadResult == 0)
        {
            EQ_CDECL(void, 0x004E0601, (void*))(EQ_GLOBAL(void*, 0x006B3194));
            EQ_GLOBAL(void*, 0x006B3194) = NULL;
            return -1;
        }
    }
    else
    {
        if (*self == 1)
            return -1;
        if (*self == 2)
        {
            *self = 1;
            return -2;
        }
    }
    EQ_GLOBAL(ShowWindowProc_guess, 0x0053421C)(hwnd, 5);
    EQ_GLOBAL(UpdateWindowProc_guess, 0x00534220)(hwnd);
    EQ_METHOD(0, void, 0x0040940F, (void*), g_display_guess)();
    DbgLog_guess(EQ_ARRAY(char, 0x0056FB48));
    EQ_METHOD(0, void, 0x004DAC29, (void*), self)();
    DbgLog_guess(EQ_ARRAY(char, 0x0056FB30));
    return -1;
}

struct ProfileAllocation_guess
{
    char storage[0x1FA4];

    // EXTERNAL: TRILOGY 0x0047EB8E ProfileAllocation_guess::ProfileAllocation_guess
    ProfileAllocation_guess(int flags);
};

struct PlayerAllocation_guess
{
    char storage[0x134];

    // EXTERNAL: TRILOGY 0x004B03EB PlayerAllocation_guess::PlayerAllocation_guess
    PlayerAllocation_guess(EQPlayer* image, int option2, int option3, int option4,
                           const char* zone);
};

struct ViewAllocation_guess
{
    char storage[0x46C];

    // EXTERNAL: TRILOGY 0x0041551E ViewAllocation_guess::ViewAllocation_guess
    ViewAllocation_guess();
};

// FUNCTION: TRILOGY 0x004d472c
// Network game start: new profile and player, character select, then up to 3 min waiting on the world server
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void __fastcall CEverQuest_StartNetworkGame_guess(int* self, int unusedEdx,
                                                   int selectionContext, int selectionFlags,
                                                   char* selectedCharacterName)
{
    typedef void (WINAPI *SleepProc_guess)(DWORD);
    typedef void (__cdecl *ClearDisplayBuffersProc_guess)(void*);
    struct NetworkGameBuffers_guess
    {
        char disconnectText[128];
        int disconnectInfo[3];
    } buffers;
    CharacterProfile* profile;
    CharacterProfile* oldProfile;
    EQPlayer* localPlayer;
    EQPlayer* allocatedPlayer;
    CDisplay* display;
    void* view;
    void* oldSlider;
    void* oldCharacterSelect;
    void* oldControl;
    void* connection;
    char* characterName;
    short characterCount;
    short selectedCharacterIndex;
    int connected;
    u32 timeout;
    u32 currentTime;

    DbgLog_guess(EQ_ARRAY(char, 0x0056FBEC));
    EQ_METHOD(0, void, 0x004C4EE7, (void*), self)();
    EQ_CDECL(void, 0x0040101D, (void))();
    EQ_CDECL(void, 0x004E87FD, (int))(0);

    oldProfile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    EQ_GLOBAL(int, 0x006B63F4) = 0;
    EQ_GLOBAL(int, 0x006E2064) = 0;
    if (oldProfile != 0)
    {
        if (oldProfile != 0)
        {
            EQ_METHOD(0, void, 0x0047EF27, (void*), oldProfile)();
            EQ_CDECL(void, 0x005218BB, (void*))(oldProfile);
        }
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) = 0;
    }

    profile = (CharacterProfile*)new ProfileAllocation_guess(0);
    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) = profile;

    allocatedPlayer = (EQPlayer*)new PlayerAllocation_guess(
        0, 0, 1, 1, EQ_ARRAY(char, 0x0053DDDC));
    localPlayer = allocatedPlayer;

    EQ_GLOBAL(EQPlayer*, 0x006E4DC8) = localPlayer;
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = localPlayer;
    EQ_METHOD(0, void, 0x004BC9BA, (EQPlayer*), localPlayer)();
    EQ_METHOD(0, void, 0x004B4873, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
    strcpy(
        localPlayer->zone, EQ_ARRAY(char, 0x0053DDDC));
    strcpy(
        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->zone, EQ_ARRAY(char, 0x0053DDDC));
    EQ_GLOBAL(int, 0x006E4DA4) = 0;
    EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = localPlayer;

    display = (CDisplay*)g_display_guess;
    display->unk_0x011 = 0;
    localPlayer->y = 0.0f;
    localPlayer->x = 0.0f;
    localPlayer->z = EQ_GLOBAL(float, 0x005353B8);
    DbgLog_guess(EQ_ARRAY(char, 0x0056FBDC));
    EQ_METHOD(2, void, 0x0040A340, (void*, const char*, int), display)
        (EQ_ARRAY(char, 0x0053DDDC), 0);
    DbgLog_guess(EQ_ARRAY(char, 0x0056FBD0));
    EQ_METHOD(0, void, 0x004DB7F1, (void*), self)();
    EQ_METHOD(1, void, 0x0040E422, (void*, void*), display)
        (*(void**)localPlayer->actor_info);

    EQ_GLOBAL(float, 0x006B6FA4) = 6.0f;
    g_activeView_guess = 5;
    EQ_GLOBAL(float, 0x006B6FA8) = 456.0f;
    EQ_GLOBAL(float, 0x006B6FB0) = 0.001f;
    EQ_GLOBAL(float, 0x006B6FB8) = 90.0f;
    EQ_GLOBAL(float, 0x006B6FAC) =
        -(localPlayer->unk_0x78 * EQ_GLOBAL(float, 0x0053516C));
    g_game_state_guess = 5;
    EQ_GLOBAL(float, 0x006B6FB4) = 0.0f;
    EQ_METHOD(1, void, 0x0040E58B, (void*, int), display)(1);

    view = EQ_GLOBAL(void*, 0x006E4DEC);
    if (view == 0)
    {
        view = new ViewAllocation_guess;
        EQ_GLOBAL(void*, 0x006E4DEC) = view;
    }
    EQ_METHOD(0, void, 0x004121CA, (void*), display)();
    self[1] = 1;
    EQ_METHOD(0, void, 0x0041527C, (void*), display)();
    DbgLog_guess(EQ_ARRAY(char, 0x0056FBB8));

    do
    {
        if (self[1] == 1)
        {
            if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != 0)
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int),
                    EQ_GLOBAL(void*, 0x006ECDF4))(1);
            DbgLog_guess(EQ_ARRAY(char, 0x0056FB9C));
            ((ClearDisplayBuffersProc_guess)EQ_GLOBAL(void*, 0x006E5174))(*(void**)display);
            DbgLog_guess(EQ_ARRAY(char, 0x0056FB80));
            EQ_METHOD(2, void, 0x004D2FB6, (void*, int, int), self)
                (selectionContext, selectionFlags);
        }
        else if (self[1] != 3)
        {
            characterCount = EQ_METHOD(0, short, 0x004307FC,
                (EQ_CharSelS*), EQ_GLOBAL(EQ_CharSelS*, 0x006E4E6C))();
            if (characterCount < 1)
                characterName = EQ_ARRAY(char, 0x006B61BC);
            else
            {
                selectedCharacterIndex =
                    *(short*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x80);
                characterName = (char*)self + 0x22400 + selectedCharacterIndex * 0x1E;
            }
            strcpy(selectedCharacterName, characterName);

            oldSlider = EQ_GLOBAL(void*, 0x006E4DF0);
            if (oldSlider != 0)
            {
                if (oldSlider != 0)
                {
                    EQ_METHOD(0, void, 0x0048257D, (void*), oldSlider)();
                    EQ_CDECL(void, 0x005218BB, (void*))(oldSlider);
                }
                EQ_GLOBAL(void*, 0x006E4DF0) = 0;
            }
            oldCharacterSelect = EQ_GLOBAL(void*, 0x006E4E6C);
            if (oldCharacterSelect != 0)
            {
                if (oldCharacterSelect != 0)
                {
                    EQ_METHOD(0, void, 0x004301B1, (void*), oldCharacterSelect)();
                    EQ_CDECL(void, 0x005218BB, (void*))(oldCharacterSelect);
                }
                EQ_GLOBAL(void*, 0x006E4E6C) = 0;
            }
            oldControl = EQ_GLOBAL(void*, 0x006E4DC8);
            if (oldControl != 0)
            {
                EQ_METHOD(0, void, 0x004B0B9F, (void*), oldControl)();
                EQ_CDECL(void, 0x005218BB, (void*))(oldControl);
            }
            EQ_GLOBAL(void*, 0x006E4DC8) = 0;
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = 0;
            EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = 0;
            g_activeView_guess = 0;
            g_game_state_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), display)(1);
            display->unk_0x011 = 1;
            EQ_METHOD(0, void, 0x0040AC31, (void*), display)();
            return;
        }

        EQ_GLOBAL(int, 0x006B63D0) = 0;
        EQ_GLOBAL(int, 0x006B61B4) = 0;
        timeout = (u32)EQ_CDECL(int, 0x004A461E, (void))() + 180000;
        while (self[1] == 3)
        {
            EQ_METHOD(0, void, 0x004C4EE7, (void*), self)();
            if (EQ_GLOBAL(int, 0x006B63D0) != 0)
            {
                self[1] = 0xFD;
                break;
            }
            if (EQ_GLOBAL(int, 0x006B61B4) != 0)
            {
                self[1] = 1;
                break;
            }
            connection = EQ_GLOBAL(void*, 0x006B3194);
            connected = 0;
            if (connection != 0)
                connected = EQ_CDECL(int, 0x004F23B0, (void*))(connection);
            if (connected == 0)
            {
                if (EQ_GLOBAL(void*, 0x006B3194) == 0)
                    strcpy(buffers.disconnectText, EQ_ARRAY(char, 0x0056F5CC));
                else
                {
                    EQ_CDECL(void, 0x004F2490, (void*, int*, int))
                        (EQ_GLOBAL(void*, 0x006B3194), buffers.disconnectInfo, 0xC);
                    sprintf(buffers.disconnectText, EQ_ARRAY(char, 0x0056F5DC),
                         buffers.disconnectInfo[0]);
                }
                DbgLog_guess(EQ_ARRAY(char, 0x0056F588), EQ_ARRAY(char, 0x0056F5A4), 0x3DA5,
                     buffers.disconnectText);
                *(int*)((char*)self + 0x223F4) = 1;
                EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), self)
                    (EQ_ARRAY(char, 0x0056F574));
                ((SleepProc_guess)EQ_GLOBAL(void*, 0x00534198))(3000);
                self[1] = 0xFF;
                EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
                self[0] = 1;
                break;
            }
            ((SleepProc_guess)EQ_GLOBAL(void*, 0x00534198))(10);
            currentTime = (u32)EQ_CDECL(int, 0x004A461E, (void))();
            if (timeout < currentTime)
            {
                EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), self)
                    (EQ_ARRAY(char, 0x0055F1A8));
                ((SleepProc_guess)EQ_GLOBAL(void*, 0x00534198))(2000);
                EQTRACE_guess(EQ_ARRAY(char, 0x0056FB5C));
                DbgLog_guess(EQ_ARRAY(char, 0x0056FB5C));
                *(int*)((char*)self + 0x223F4) = 1;
                self[1] = 0xFF;
                EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
                self[0] = 1;
                connection = EQ_GLOBAL(void*, 0x006B3194);
                if (connection != 0)
                {
                    EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))
                        (connection, 10000, 0, 0);
                    EQ_GLOBAL(void*, 0x006B3194) = 0;
                }
                return;
            }
        }
    } while (true);
}

// FUNCTION: TRILOGY 0x004d4c19
// Waits for the requested zone's local player to arrive, or handles a disconnect or timeout.
void __fastcall GetZoneInfoFromNetwork(int* self, int unusedEdx, char* zoneName)
{
    typedef void (WINAPI *SleepProc_guess)(DWORD);
    struct GetZoneInfoBuffers_guess
    {
        char disconnectText[128];
        u32 zoneNameCrc;
        char zoneNameBuffer[32];
        char disconnectInfo[12];
        float outgoingDataRate;
        int fileOrStartTime;
        float dataRate;
    } buffers;
    float clampedDataRate;
    int currentTime;

    buffers.dataRate = 5.0f;
    buffers.fileOrStartTime = (int)EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))
        (EQ_ARRAY(char, 0x0056FD1C), EQ_ARRAY(char, 0x0055C1AC));
    if (buffers.fileOrStartTime != 0)
    {
        if (EQ_CDECL(int, 0x005241C5, (FILE*, const char*, ...))
            ((FILE*)buffers.fileOrStartTime, EQ_ARRAY(char, 0x0056FD0C), &buffers.dataRate) == 1)
        {
            EQ_CDECL(int, 0x00521FBB, (FILE*))((FILE*)buffers.fileOrStartTime);
            goto clampDataRate;
        }
        EQ_CDECL(int, 0x00521FBB, (FILE*))((FILE*)buffers.fileOrStartTime);
    }
    buffers.dataRate = 5.0f;
    buffers.fileOrStartTime = (int)EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))
        (EQ_ARRAY(char, 0x0056FD1C), EQ_ARRAY(char, 0x0056FD18));
    if (buffers.fileOrStartTime != 0)
    {
        EQ_CDECL(int, 0x00524138, (FILE*, const char*, ...))
            ((FILE*)buffers.fileOrStartTime, EQ_ARRAY(char, 0x0056FD10), (double)buffers.dataRate);
        EQ_CDECL(int, 0x00521FBB, (FILE*))((FILE*)buffers.fileOrStartTime);
    }

clampDataRate:
    clampedDataRate = EQ_GLOBAL(float, 0x005345B8);
    if (clampedDataRate <= buffers.dataRate)
    {
        clampedDataRate = EQ_GLOBAL(float, 0x00535124);
        if (buffers.dataRate <= clampedDataRate)
            clampedDataRate = buffers.dataRate;
    }
    buffers.dataRate = clampedDataRate;
    buffers.outgoingDataRate = buffers.dataRate;
    send_message_guess
        ((void**)0x006B3194, 0x21E8, &buffers.outgoingDataRate, 4, 1);
    EQ_GLOBAL(int, 0x006B3190) = 0;
    EQ_GLOBAL(int, 0x006B70E0) = 0;
    strcpy(buffers.zoneNameBuffer, zoneName);
    buffers.zoneNameCrc = EQ_CDECL(u32, 0x004E04A2, (void*, int))(buffers.zoneNameBuffer, 0x20);
    send_message_guess
        ((void**)0x006B3194, 0x202A, &buffers.zoneNameCrc, 0x24, 1);
    buffers.fileOrStartTime = EQ_CDECL(int, 0x004A461E, (void))();
    self[1] = 4;
    for (;;)
    {
        EQ_METHOD(0, void, 0x004C4EE7, (int*), self)();
        if (EQ_GLOBAL(u8, 0x006B63F8) == 0)
        {
            if (EQ_GLOBAL(void*, 0x006B3194) == 0)
                currentTime = 0;
            else
                currentTime = EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194));
            if (currentTime == 0)
            {
                if (EQ_GLOBAL(void*, 0x006B3194) == 0)
                {
                    strcpy(buffers.disconnectText, EQ_ARRAY(char, 0x0056F5CC));
                }
                else
                {
                    EQ_CDECL(void, 0x004F2490, (void*, void*, u32))
                        (EQ_GLOBAL(void*, 0x006B3194), buffers.disconnectInfo, 0x0C);
                    sprintf(buffers.disconnectText, EQ_ARRAY(char, 0x0056F5DC), *(int*)buffers.disconnectInfo);
                }
                DbgLog_guess(EQ_ARRAY(char, 0x0056FCE4), EQ_ARRAY(char, 0x0056F5A4), 0x3E30, buffers.disconnectText);
                self[0x88FD] = 1;
                EQ_METHOD(1, void, 0x004D0FE6, (int*, const char*), self)(EQ_ARRAY(char, 0x0056F574));
                EQ_GLOBAL(SleepProc_guess, 0x00534198)(3000);
                self[1] = 0xFF;
                EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
                *self = 1;
                return;
            }
        }
        if (EQ_GLOBAL(int, 0x006B3190) == 0 || EQ_GLOBAL(int, 0x006B70E0) == 0)
        {
            if (EQ_GLOBAL(u8, 0x006B7111) == 1)
            {
                DbgLog_guess(EQ_ARRAY(char, 0x0056FC94), EQ_ARRAY(char, 0x0056F5A4), 0x3E4F);
                goto finishZoneInfo;
            }
        }
        else if (EQ_GLOBAL(EQPlayer*, 0x006E4D58) != 0)
        {
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = EQ_METHOD(1, EQPlayer*, 0x004B1435,
                (EQPlayer*, const char*), EQ_GLOBAL(EQPlayer*, 0x006E4D58))(zoneName);
            if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == 0)
                DbgLog_guess(EQ_ARRAY(char, 0x0056FCC8), zoneName);
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->stand_state = 'd';
            EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->stand_state = 'd';
            EQ_GLOBAL(int, 0x006E4DA4) = 0;
            EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
            self[1] = 0xFD;
            return;
        }
        currentTime = EQ_CDECL(int, 0x004A461E, (void))();
        if (180000 < (u32)(currentTime - buffers.fileOrStartTime))
            break;
        EQ_GLOBAL(SleepProc_guess, 0x00534198)(10);
        if (self[1] != 4)
            return;
    }
    EQ_METHOD(1, void, 0x004D0FE6, (int*, const char*), self)(EQ_ARRAY(char, 0x0055F1A8));
    EQ_GLOBAL(SleepProc_guess, 0x00534198)(2000);
    EQTRACE_guess(EQ_ARRAY(char, 0x0056FC50));
    DbgLog_guess(EQ_ARRAY(char, 0x0056FC04), EQ_GLOBAL(int, 0x006E4D58));

finishZoneInfo:
    self[0x88FD] = 1;
    self[1] = 0xFF;
    EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
    *self = 1;
    if (EQ_GLOBAL(void*, 0x006B3194) == 0)
        return;
    EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))
        (EQ_GLOBAL(void*, 0x006B3194), 10000, 0, 0);
    EQ_GLOBAL(void*, 0x006B3194) = 0;
    return;
}

// The 0x80-byte EQ_Overlay-derived main-screen overlay EnterZone creates (pointer at 0x006E4E38);
// its constructor is the client's.
class EQ_Main4Overlay_guess : public EQ_Overlay
{
public:
    // EXTERNAL: TRILOGY 0x00451344 EQ_Main4Overlay_guess::EQ_Main4Overlay_guess
    EQ_Main4Overlay_guess();
};

// FUNCTION: TRILOGY 0x004d4f3a
// Zone entry: resets per-zone state, loads the zone's world (game state 1 on failure), sets up the main UI.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: three instructions around the first rep stosd differ: the original zeroes EBX after the rep setup
// and keeps the 0x006B6498 store after it; loop, pointer and memset forms all schedule them as this build does.
void __fastcall CEverQuest_EnterZone_guess(void* self, int unusedEdx, HWND window)
{
    typedef void (__fastcall *tNoArgMethod_guess)(void*);
    typedef BOOL (WINAPI *SetWindowTextAProc_guess)(HWND, LPCSTR);
    u32* data;
    size_t nameLength;
    char* playerName;
    char zoneName[20];
    u32 message[0x17];
    int count;
    short buttonIndex;

    (EQ_GLOBAL(SetWindowTextAProc_guess, 0x00534258))
        (window, EQ_ARRAY(char, 0x0056FD5C));
    for (count = 0; count < 0x100; count++)
        EQ_ARRAY(u32, 0x006E19E4)[count] = 0;
    EQ_GLOBAL(u32, 0x006B6498) = 0;
    EQ_GLOBAL(u32, 0x006B64A0) = 0;
    EQ_GLOBAL(u32, 0x006B6764) = 0;
    EQ_GLOBAL(u16, 0x006B7116) = 0;
    EQ_GLOBAL(u16, 0x006B7118) = 0;
    if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
    {
        buttonIndex = 0;
        do
        {
            ((u32*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x6C50C))[buttonIndex] = 0;
            ((u32*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x6C4EC))[buttonIndex] = 0;
            if (buttonIndex < 4)
            {
                ((u32*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x6C52C))[buttonIndex] = 0;
                ((u32*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x6C53C))[buttonIndex] = 0;
            }
            buttonIndex++;
        } while (buttonIndex < 8);
    }
    data = EQ_ARRAY(u32, 0x006B6560);
    count = 10;
    do
    {
        data[-10] = 0;
        *data = 0;
        data++;
        count--;
    } while (count != 0);
    EQ_GLOBAL(u32, 0x006B6588) = 0;
    EQ_GLOBAL(u32, 0x006B70E4) = 0;
    EQ_GLOBAL(u8, 0x006B6770) = 0;
    EQ_GLOBAL(short, 0x006B6772) = -1;
    EQ_GLOBAL(u32, 0x006B6774) = 0;
    EQ_GLOBAL(u32, 0x006B6778) = 0;
    EQ_GLOBAL(u32, 0x006B677C) = 0;
    EQ_GLOBAL(u32, 0x006B6780) = 0;
    EQ_GLOBAL(u16, 0x006B6784) = 0;
    EQ_GLOBAL(u32, 0x006B6788) = 0;
    strcpy(EQ_ARRAY(char, 0x006B678C), EQ_ARRAY(char, 0x0058FBA4));
    EQ_GLOBAL(u32, 0x006B67AC) = 0;
    EQ_GLOBAL(short, 0x006B67B0) = -1;
    EQ_CDECL(void, 0x004E878C, (void))();
    EQ_GLOBAL(u32, 0x006B70F0) = 0;
    EQ_GLOBAL(u32, 0x006B6480) = 0xFFFFFFFF;
    EQ_GLOBAL(float, 0x006B711C) = 0.0f;
    EQ_GLOBAL(u32, 0x006B6484) = 0xFFFFFFFF;
    EQ_GLOBAL(u8, 0x006B6488) = 0;
    EQ_GLOBAL(u8, 0x006B6489) = 0;
    EQ_GLOBAL(u8, 0x006B648B) = 0;
    EQ_GLOBAL(u8, 0x006B648A) = 0;
    EQ_GLOBAL(u32, 0x006B65A4) = 0;
    EQ_GLOBAL(u32, 0x006B64C8) = 0;
    EQ_GLOBAL(u32, 0x006B64CC) = 0;
    EQ_GLOBAL(u32, 0x006B64D4) = 0;
    EQ_GLOBAL(u8, 0x006B64D0) = 0;
    EQ_GLOBAL(u32, 0x006B676C) = 0;
    EQ_GLOBAL(u32, 0x006B6764) = 0;
    EQ_GLOBAL(u8, 0x006B7110) = 0;
    EQ_GLOBAL(u8, 0x006B65D8) = 0;
    EQ_GLOBAL(u16, 0x006B65B6) = 0;
    EQ_GLOBAL(u8, 0x006B6404) = 1;
    EQ_GLOBAL(u32, 0x006B65BC) = 0;
    EQ_GLOBAL(u32, 0x006B65C0) = 0;
    EQ_GLOBAL(u32, 0x006B65C4) = 0;
    EQ_GLOBAL(u32, 0x006B65C8) = 0;
    EQ_GLOBAL(u32, 0x006B65CC) = 0;
    EQ_GLOBAL(u32, 0x006B65D0) = 0;
    EQ_GLOBAL(u32, 0x006B65D4) = 0;
    EQ_GLOBAL(u8, 0x006B645A) = 0;
    EQ_GLOBAL(u32, 0x006B7124) = 0;
    EQ_GLOBAL(u32, 0x006B6594) = 0;
    EQ_GLOBAL(u32, 0x006B6590) = 0;
    EQ_GLOBAL(u32, 0x006E4E8C) = 0;
    EQ_GLOBAL(u32, 0x006B65A8) = 0;
    EQ_GLOBAL(u32, 0x006B65AC) = 0;
    EQ_GLOBAL(u32, 0x006B65B0) = 0;
    EQ_GLOBAL(u32, 0x006B6754) = 0;
    EQ_GLOBAL(u16, 0x006B6752) = 0;
    EQ_GLOBAL(u32, 0x006B70E8) = 0;
    EQ_GLOBAL(u32, 0x006B70EC) = 0;
    EQ_GLOBAL(u8, 0x006B710C) = 0;
    EQ_GLOBAL(u8, 0x006B710D) = 0;
    EQ_GLOBAL(u32, 0x006B6450) = 0;
    EQ_GLOBAL(u32, 0x006B6454) = 0;
    EQ_GLOBAL(u8, 0x006B645B) = 0;
    EQ_GLOBAL(u16, 0x006B70FC) = 0;
    EQ_GLOBAL(u16, 0x006B70FE) = 0;
    EQ_GLOBAL(u16, 0x006B7100) = 0;
    EQ_GLOBAL(u16, 0x006B7102) = 0;
    EQ_GLOBAL(u16, 0x006B7104) = 0;
    EQ_GLOBAL(u16, 0x006B7106) = 0;
    EQ_GLOBAL(u16, 0x006B7116) = 0;
    EQ_GLOBAL(u16, 0x006B7118) = 0;
    EQ_GLOBAL(u8, 0x006B710E) = 0;
    EQ_GLOBAL(u8, 0x006B710F) = 0;
    EQ_GLOBAL(u8, 0x006B6854) = 0;
    EQ_GLOBAL(u8, 0x006B6408) = 0;
    EQ_GLOBAL(u32, 0x006B64E4) = 0x96;
    EQ_GLOBAL(u32, 0x006B6474) = 0;
    EQ_GLOBAL(u32, 0x006B647C) = 0;
    EQ_GLOBAL(u32, 0x006B6478) = 0;
    EQ_GLOBAL(u32, 0x006B64EC) = 0x4B;
    EQ_GLOBAL(u32, 0x006E2054) = 0;
    EQ_GLOBAL(u32, 0x006B64F8) = 0x52;
    EQ_GLOBAL(u32, 0x006B64F0) = 0x26;
    EQ_GLOBAL(u32, 0x006B64F4) = 0x26;
    EQ_GLOBAL(u32, 0x006B64FC) = 0x37;
    EQ_GLOBAL(u32, 0x006B6500) = 0xA4;
    EQ_GLOBAL(u32, 0x006B6768) = 0;
    if (EQ_GLOBAL(u32*, 0x006E4DDC) != 0)
    {
        EQ_GLOBAL(float*, 0x006E4DDC)[2] = 0.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[0] = 1.0f;
        EQ_GLOBAL(float*, 0x006E4DDC)[1] = 0.0f;
        EQ_GLOBAL(u32*, 0x006E4DDC)[4] = 0xFFFFFFFF;
    }
    data = EQ_ARRAY(u32, 0x006B67B4);
    count = 0x24;
    do
    {
        if (*data != 0)
        {
            EQ_CDECL(void, 0x005218BB, (void*))((void*)*data);
            *data = 0;
        }
        data++;
        count--;
    } while (count != 0);
    data = EQ_ARRAY(u32, 0x006B6844);
    for (count = 4; count != 0; count--)
    {
        *data = 0;
        data++;
    }
    strcpy((char*)self + 0xBA8, EQ_ARRAY(char, 0x0058FBA4));
    EQ_GLOBAL(u32, 0x006B6468) = 0;
    EQ_GLOBAL(u32, 0x006B646C) = 0;
    EQ_GLOBAL(u32, 0x006B6470) = 0;
    EQ_GLOBAL(u32, 0x006B6508) = 0xB;
    EQ_GLOBAL(u32, 0x006B6498) = 0;
    EQ_GLOBAL(u32, 0x006B64A0) = 0;
    EQ_GLOBAL(u32, 0x006B649C) = 0;
    EQ_GLOBAL(u32, 0x006B64A4) = 0;
    EQ_GLOBAL(void*, 0x006E4D88) = 0;
    EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = 0;
    EQ_GLOBAL(u32, 0x006E4D98) = 0;
    EQ_GLOBAL(u32, 0x006E4D94) = 0;
    EQ_GLOBAL(u32, 0x006E4DB8) = 0;
    EQ_GLOBAL(u32, 0x006E4DBC) = 0;
    EQ_GLOBAL(u32, 0x006E4DC0) = 0;
    EQ_GLOBAL(u32, 0x006E4DC4) = 0;
    if (g_pc_guess != 0)
    {
        if (strlen(g_pc_guess->zone) > 0)
        {
            strcpy(zoneName, g_pc_guess->zone);
            strcpy(EQ_ARRAY(char, 0x006B65FA), g_pc_guess->zone);
            strcpy(EQ_ARRAY(char, 0x006B65DC), g_local_player_guess->name);
        }
    }
    if (EQ_METHOD(0, int, 0x004CF561, (void*), self)() == 0)
    {
        message[0] = 0xFFFFFFFF;
        message[1] = 0xFFFFFFFF;
        message[2] = 0x14;
        send_message_guess
            (g_world_connection_guess, 0x2147, message, 0x5C, 1);
    }
    EQ_METHOD(2, void, 0x0040A340, (void*, char*, int),
        g_display_guess)(zoneName, 1);
    if (EQ_METHOD(0, int, 0x004CF561, (void*), self)() == 0)
    {
        message[0] = 0xFFFFFFFF;
        message[1] = 0xFFFFFFFF;
        message[2] = 0x1E;
        send_message_guess
            (g_world_connection_guess, 0x2147, message, 0x5C, 1);
    }
    if (g_cdisplay_guess->palette_failed_guess == 1)
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0056FD2C), EQ_ARRAY(char, 0x0056F5A4), 0x3F64);
        *(int*)self = 1;
    }
    else
    {
        if (EQ_GLOBAL(u8, 0x006B63F8) == 0 && DAT_006b6494 == 0)
        {
            playerName = g_local_player_guess->name;
            nameLength = strlen(playerName);
            send_message_guess
                (g_world_connection_guess, 0x205D, playerName, nameLength + 1, 1);
        }
        EQ_GLOBAL(u32, 0x006B6FA4) = 0x41300000;
        EQ_GLOBAL(u32, 0x006B6FA8) = 0x43FA0000;
        EQ_GLOBAL(u32, 0x006B6FAC) = 0xBF800000;
        EQ_GLOBAL(u32, 0x006B6FB0) = 0x3A83126F;
        EQ_GLOBAL(u32, 0x006B6FB4) = 0xC0FFF7CF;
        EQ_GLOBAL(u32, 0x006B6FB8) = 0x42B40000;
        if (EQ_GLOBAL(void*, 0x006E4E38) == 0)
            EQ_GLOBAL(void*, 0x006E4E38) = new EQ_Main4Overlay_guess;
        if (EQ_METHOD(0, int, 0x004CF561, (void*), self)() == 0)
        {
            message[0] = 0xFFFFFFFF;
            message[1] = 0xFFFFFFFF;
            message[2] = 0x28;
            send_message_guess
                (g_world_connection_guess, 0x2147, message, 0x5C, 1);
        }
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
        {
            EQ_GLOBAL(int*, 0x006E4E44)[0x1A177] = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x1A179] = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x1A178] = 0;
            *((u8*)(EQ_GLOBAL(u32*, 0x006E4E44) + 0x403)) = 0;
            *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8FBAC) = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x1A17A] = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x1A17D] = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x23EE4] = 0;
            EQ_GLOBAL(u32, 0x006E4DB8) = 0;
            EQ_GLOBAL(u32, 0x006E4DBC) = 0;
            EQ_GLOBAL(u32, 0x006E4DC0) = 0;
            EQ_GLOBAL(u32, 0x006E4DC4) = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x397] = 0;
            EQ_GLOBAL(int*, 0x006E4E44)[0x25] = 0;
            (*(tNoArgMethod_guess*)(*(u32*)EQ_GLOBAL(void*, 0x006E4E44) + 8))
                (EQ_GLOBAL(void*, 0x006E4E44));
            *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE0) = 0;
            *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE2) = 0;
            *((u8*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE1) = 0;
            (*(tNoArgMethod_guess*)(*(u32*)EQ_GLOBAL(void*, 0x006E4E44) + 4))
                (EQ_GLOBAL(void*, 0x006E4E44));
            EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
        }
        if (EQ_GLOBAL(u8, 0x006EFC70) == 0)
            EQ_METHOD(0, void, 0x004DB7D2, (void*), self)();
        if (EQ_METHOD(0, int, 0x004CF561, (void*), self)() == 0)
        {
            message[0] = 0xFFFFFFFF;
            message[1] = 0xFFFFFFFF;
            message[2] = 0x32;
            send_message_guess
                (g_world_connection_guess, 0x2147, message, 0x5C, 1);
        }
    }
}

// FUNCTION: TRILOGY 0x004d55b4
// Asks the player to confirm a requested character surname.
// Called as __thiscall (ECX = pinstCEverQuest from 0x004A4B58), `this` unused; callee pops one argument.
// When the prompt dialog at 0x006E4E20 exists and is not shown (+0x10 == 0), sets its mode (+0x17C) to 10,
// calls its virtual method 1, stores the last name (+0x5D7), formats "Your last name will be %s is this
// correct?" into its text (+0x1B9) and sets its timeout (+0x178) to the display time (+0xBC) plus 10 s.
// No class declares this method, so it is a __fastcall free function with the unused ECX/EDX slots.
void __fastcall ConfirmLastName_guess(void* self, int unusedEdx, char* lastname)
{
    if (EQ_GLOBAL(char*, 0x006E4E20) != NULL && *(int*)(EQ_GLOBAL(char*, 0x006E4E20) + 0x10) == 0)
    {
        *(EQ_GLOBAL(char*, 0x006E4E20) + 0x17C) = 0xA;
        ((void (__fastcall*)(void*))(*(u32**)EQ_GLOBAL(char*, 0x006E4E20))[1])(EQ_GLOBAL(char*, 0x006E4E20));
        strcpy(EQ_GLOBAL(char*, 0x006E4E20) + 0x5D7, lastname);
        sprintf(EQ_GLOBAL(char*, 0x006E4E20) + 0x1B9, "Your last name will be %s is this correct?",
                EQ_GLOBAL(char*, 0x006E4E20) + 0x5D7);
        pinstEQ_Requestor_guess->timeout_guess = *(int*)(g_displayManager_guess + 0xBC) + 10000;
    }
}

// FUNCTION: TRILOGY 0x004d5629
// Prompts the player to confirm a named-character kill command.
// Called as __thiscall (ECX = pinstCEverQuest from 0x004A51D6), `this` unused; callee pops two arguments.
// When the prompt dialog at 0x006E4E20 exists and is not shown (+0x10 == 0), sets its mode (+0x17C) to 13,
// calls its virtual method 1, stores both names (+0x17D, +0x19B), formats "Kill %s is this correct?" into
// its text (+0x1B9) and sets its timeout (+0x178) to the display time (+0xBC) plus 10 s.
// No class declares this method, so it is a __fastcall free function with the unused ECX/EDX slots.
void __fastcall ConfirmKill_guess(void* self, int unusedEdx, char* name, char* name2)
{
    if (EQ_GLOBAL(char*, 0x006E4E20) != NULL && *(int*)(EQ_GLOBAL(char*, 0x006E4E20) + 0x10) == 0)
    {
        *(EQ_GLOBAL(char*, 0x006E4E20) + 0x17C) = 0xD;
        ((void (__fastcall*)(void*))(*(u32**)EQ_GLOBAL(char*, 0x006E4E20))[1])(EQ_GLOBAL(char*, 0x006E4E20));
        strcpy(EQ_GLOBAL(char*, 0x006E4E20) + 0x17D, name);
        strcpy(EQ_GLOBAL(char*, 0x006E4E20) + 0x19B, name2);
        sprintf(EQ_GLOBAL(char*, 0x006E4E20) + 0x1B9, "Kill %s is this correct?", name);
        pinstEQ_Requestor_guess->timeout_guess = *(int*)(g_displayManager_guess + 0xBC) + 10000;
    }
}

// FUNCTION: TRILOGY 0x004d56b0
// Rebuilds the local character's face and equipped gear appearance when its model needs refreshing.
// __cdecl with no arguments: the callers leave ECX set, the function never reads it and pops nothing.
// Returns at once unless the local player's texture is 0xFF. Otherwise copies the face from g_pc_guess, redraws it
// (CDisplay::SwapFace 0x00406EDC) and sends opcode 0x2092 (slot 9). Then for slots 0..21: an empty slot redraws its
// texture when GetTextureSlot says so and, if it is one of the mapped slots (2, 7, 9, 10, 12, 17, 18, 19), swaps the
// head or a material and sends 0x2092; a filled slot redraws its texture and calls HandleMaterial (0x004A1E2A).
void CEverQuest_UpdatePlayerAppearance_guess(void)
{
    int equipmentSlot;
    int itemIndex;
    char texturePart;
    short equipSlot;
    char* itemIdFile;
    struct AppearancePacket_guess
    {
        u32 spawn_id;       // 0x00
        u8 slot;            // 0x04
        u8 face;            // 0x05
        u32 zero;           // 0x08
        u8 unknown0x12C;    // 0x0C
    } packet;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->texture != 0xFF)
        return;
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->face;
    EQ_METHOD(3, void, 0x00406EDC, (void*, EQPlayer*, int, int), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face, 0);
    packet.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
    packet.slot = 9;
    packet.face = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face;
    packet.zero = 0;
    packet.unknown0x12C = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->weaponAnimationType;
    send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2092, &packet, 0x10, 1);
    equipmentSlot = 0;
    do
    {
        itemIndex = equipmentSlot;
        equipSlot = -1;
        switch (equipmentSlot)
        {
        case 0xC:
            equipSlot = 4;
            break;
        case 2:
            equipSlot = 0;
            break;
        case 7:
            equipSlot = 2;
            break;
        case 9:
        case 10:
            equipSlot = 3;
            break;
        case 0x11:
            equipSlot = 1;
            break;
        case 0x12:
            equipSlot = 5;
            break;
        case 0x13:
            equipSlot = 6;
            break;
        }
        if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[equipmentSlot] != 0)
        {
            texturePart = EQ_CDECL(int, 0x004A1DCE, (int))(equipmentSlot);
            if (texturePart != 0)
            {
                itemIdFile = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[itemIndex]->idfile;
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                    EQ_CDECL(int, 0x004A1DCE, (int))(equipmentSlot), itemIdFile);
            }
            EQ_CDECL(void, 0x004A1E2A, (CharacterProfile*, int, EQ_Item*, int))(
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8), equipmentSlot,
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[itemIndex], 0);
        }
        else
        {
            texturePart = EQ_CDECL(int, 0x004A1DCE, (int))(equipmentSlot);
            if (texturePart != 0)
            {
                EQ_METHOD(3, void, 0x0040B6E0, (void*, EQPlayer*, int, char*), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C),
                    EQ_CDECL(int, 0x004A1DCE, (int))(equipmentSlot), EQ_ARRAY(char, 0x0053D860));
            }
            if (equipSlot != -1)
            {
                if (equipSlot == 0)
                {
                    EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, int, int), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 0,
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->equipment[0],
                        EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->face, 0, 0);
                }
                else
                {
                    EQ_METHOD(5, void, 0x0040798E, (void*, EQPlayer*, int, int, int, int), g_display_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), equipSlot, 0,
                        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->equipment[equipSlot], 0);
                }
                packet.spawn_id = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id;
                packet.face = 0;
                packet.slot = (u8)equipSlot;
                packet.zero = 0;
                packet.unknown0x12C = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->weaponAnimationType;
                send_message_guess(&EQ_GLOBAL(void*, 0x006B3194), 0x2092, &packet, 0x10, 1);
            }
        }
        equipmentSlot = equipmentSlot + 1;
    } while (equipmentSlot < 0x16);
}

// FUNCTION: TRILOGY 0x004d58b4
// Requests records for guilds used by players but missing from the local guild list.
void __fastcall msg_eq_netplayer_missingGuilds_guess(void* self, int unusedEdx)
{
    int missingGuilds[512];
    int packetGuild;
    short guildId;
    int index;
    EQPlayer* player;

    memset(missingGuilds, 0, 0x800);
    for (player = g_player_top_guess; player != NULL; player = player->next)
    {
        if (player->type == 0)
        {
            guildId = player->guild_id;
            if (guildId != -1 && guildId < 0x200)
            {
                if (EQ_CDECL(int, 0x004DFE8D, (int))(guildId) == -1)
                    missingGuilds[player->guild_id] = 1;
            }
        }
    }
    for (index = 0; index < 0x200; index++)
    {
        if (missingGuilds[index] == 1)
        {
            packetGuild = index;
            send_message_guess(
                g_world_connection_guess, 0x2128, &packetGuild, 4, 1);
            EQTRACE_guess(
                (const char*)0x0056FDB8, index);
        }
    }
}

// FUNCTION: TRILOGY 0x004d5968
// Checks whether the character's saved boat can be located during login.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::IFoundMyVehicle, high; beta
// debug string)
// @early-stop: found sits in a stack slot (EBX in the original); this/start swap EDI/EBP; thiscall EDX dummy adds xor edx.
// EQClassic (PlayerProfile.h, as SendLocateVehicle): sends opcode 0x0e40 during login when the profile
// names a vehicle and waits up to 60 s for the answer. The client's opcode is 0x400E (EQClassic writes it
// byte-swapped).
// __thiscall, no arguments; returns a bool in AL. Sends MSG_LOCATE_VEHICLE with the boat name, then pumps
// ProcessMBox until the reply handler sets 0x006E1DE4, sleeping 10 ms per round; gives up after 60000 ms.
bool CEverQuest::IFoundMyVehicle(void)
{
    bool found = false;
    u32 start;
    char* boat;
    size_t len;

    len = strlen(g_pc_guess->boat);
    if (len < 1)
        return false;
    g_vehicle_pending_guess = 1;
    start = EqGetTime();
    EQTRACE_guess("SENDING OUT A MSG_LOCATE_VEHICLE for %s\n", g_pc_guess->boat);
    boat = g_pc_guess->boat;
    len = strlen(boat);
    send_message_guess(g_world_connection_guess, 0x400E, boat, len + 1, 1);
    for (;;)
    {
        CEverQuest_ProcessMBox_guess(this);
        if (g_vehicle_found_guess != 0)
        {
            g_vehicle_found_guess = 0;
            found = true;
            break;
        }
        if (EqGetTime() - start > 60000)
        {
            EQTRACE_guess("Timing out in IFoundMyVehicle().\n");
            g_vehicle_pending_guess = 0;
            break;
        }
        Sleep(10);
    }
    return found;
}

class CEverQuest_FriendsIgnoreIni_standin_guess
{
public:
    void FriendsIgnoreIni_guess(int mode);
};

// FUNCTION: TRILOGY 0x004d5a24
// Loads or saves the character's Friends and Ignored lists in its per-character INI file.
// @early-stop: EBX hoists the list bound; cursor increments use load/add/store, and API argument registers differ.
// __thiscall with one stack argument (RET 4); ECX, the CEverQuest object, is never read. Names the file
// ".\<character>_<server>.ini": the server part is the first 3 bytes of 0x006EE3FC (a '.' third byte is cut)
// when the login at 0x006EE3F4 starts with "eqworld" (case-insensitive), else the login up to the first byte
// of 0x00545998. mode == 0 writes the Friends list (0x006E2114) and the Ignored list (0x006E24D4), 30 entries
// of 32 bytes each, an empty entry as *NULL*. Any other mode reads both back; a value starting with '*' clears
// its entry. A missing file first refills 20 entries of each list from 0x006B6C64 / 0x006B69E4 (the other 10
// are cleared), sets mode to 0 and writes them out. Returns nothing.
void CEverQuest_FriendsIgnoreIni_standin_guess::FriendsIgnoreIni_guess(int mode)
{
    typedef DWORD (__stdcall *GetPrivateProfileStringA_guess_t)(const char*, const char*, const char*, char*, DWORD, const char*);
    typedef BOOL (__stdcall *WritePrivateProfileStringA_guess_t)(const char*, const char*, const char*, const char*);
    char readBuf[256];
    char statBuf[36];
    char server[128];
    char path[128];
    char key[128];
    char* initCursor;
    char* volatile cursor; /* fake match: retain the original list cursor stack slot. */
    char* value;
    size_t count;
    volatile int& modeCount = mode; /* fake match: keep the original argument slot as the loop counter. */

    sprintf(path, EQ_ARRAY(char, 0x0056FE68), g_pc_guess);
    if (_strnicmp(EQ_ARRAY(char, 0x006EE3F4), EQ_ARRAY(char, 0x0054599C), 7) == 0)
    {
        strncpy(server, EQ_ARRAY(char, 0x006EE3FC), 3);
        server[3] = 0;
        if (server[2] == '.')
        {
            server[2] = '\0';
        }
    }
    else
    {
        count = EQ_CDECL(size_t, 0x00523560, (const char*, const char*))(EQ_ARRAY(char, 0x006EE3F4), EQ_ARRAY(char, 0x00545998));
        strncpy(server, EQ_ARRAY(char, 0x006EE3F4), count);
        server[count] = '\0';
    }
    strcat(path, server);
    strcat(path, EQ_ARRAY(char, 0x0056FE60));
    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))(path, statBuf) != 0)
    {
        initCursor = EQ_ARRAY(char, 0x006E2114);
        do
        {
            if ((int)initCursor >= 0x006E2394)
            {
                *initCursor = 0;
            }
            else
            {
                strcpy(initCursor, initCursor - 0x2B4B0);
            }
            initCursor += 0x20;
        } while ((int)initCursor < 0x006E24D4);
        initCursor = g_ignore_list_guess;
        do
        {
            if ((int)initCursor >= 0x006E2754)
            {
                *initCursor = 0;
            }
            else
            {
                strcpy(initCursor, initCursor - 0x2BAF0);
            }
            initCursor += 0x20;
        } while ((int)initCursor < 0x006E2894);
        modeCount = 0;
    }
    if (modeCount != 0)
    {
        modeCount = 0;
        cursor = EQ_ARRAY(char, 0x006E2114);
        GetPrivateProfileStringA_guess_t readProfile = EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C);
        do
        {
            sprintf(key, EQ_ARRAY(char, 0x0056FE4C), modeCount);
            readProfile
                (EQ_ARRAY(char, 0x0056FE58), key, EQ_ARRAY(char, 0x0053DB48), readBuf, 0x20, path);
            if (readBuf[0] == '*')
            {
                *cursor = 0;
            }
            else
            {
                strcpy(cursor, readBuf);
            }
            cursor += 0x20;
            modeCount++;
        } while ((int)cursor < 0x006E24D4);
        modeCount = 0;
        cursor = g_ignore_list_guess;
        do
        {
            sprintf(key, EQ_ARRAY(char, 0x0056FE38), modeCount);
            readProfile
                (EQ_ARRAY(char, 0x0056FE44), key, EQ_ARRAY(char, 0x0053DB48), readBuf, 0x20, path);
            if (readBuf[0] == '*')
            {
                *cursor = 0;
            }
            else
            {
                strcpy(cursor, readBuf);
            }
            cursor += 0x20;
            modeCount++;
        } while ((int)cursor < 0x006E2894);
    }
    else
    {
        modeCount = 0;
        cursor = EQ_ARRAY(char, 0x006E2114);
        WritePrivateProfileStringA_guess_t writeProfile = EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188);
        do
        {
            sprintf(key, EQ_ARRAY(char, 0x0056FE4C), modeCount);
            value = cursor;
            if (*value == '\0')
            {
                value = EQ_ARRAY(char, 0x0053DB48);
            }
            writeProfile(EQ_ARRAY(char, 0x0056FE58), key, value, path);
            cursor += 0x20;
            modeCount++;
        } while ((int)cursor < 0x006E24D4);
        modeCount = 0;
        cursor = g_ignore_list_guess;
        do
        {
            sprintf(key, EQ_ARRAY(char, 0x0056FE38), modeCount);
            value = cursor;
            if (*value == '\0')
            {
                value = EQ_ARRAY(char, 0x0053DB48);
            }
            writeProfile(EQ_ARRAY(char, 0x0056FE44), key, value, path);
            cursor += 0x20;
            modeCount++;
        } while ((int)cursor < 0x006E2894);
    }
}

class CEverQuest_DoMainLoop_standin_guess
{
public:
    int quitRequested;
    int gameState;
    u8 unknown0x00008[0x223E8];
    int enteredWorld;
    int disconnected;
    void DoMainLoop(HWND window);
};

#define MAIN_LOOP_STATE_RUNNING 5
#define MAIN_LOOP_STATE_LOADING 7
#define MAIN_LOOP_STATE_DONE 0xff
#define MAIN_LOOP_REASON_DATA 0xcb
#define MAIN_LOOP_REASON_ZONE 0xcc
#define MAIN_LOOP_NO_FLOOR -1.0e27f
#define MAIN_LOOP_DISPLAY_TIME (g_cdisplay_guess->time_ms_guess)

#define CEverQuest CEverQuest_DoMainLoop_standin_guess
// FUNCTION: TRILOGY 0x004d5d0f
// In-zone game loop: finishes zoning in (boat, server handshake), runs frames until zoning, quit or disconnect.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall (RET 4). Puts the player back on its vehicle, enters the world (or tutorial), then runs the
// frame loop while gameState is 5: repop/quit/disconnect handling, position updates, jumps, controls, render.
void CEverQuest::DoMainLoop(HWND window)
{
    typedef BOOL (WINAPI *SetWindowTextAProc_guess)(HWND, LPCSTR);
    typedef void (WINAPI *CriticalSectionProc_guess)(LPCRITICAL_SECTION);
    typedef void (WINAPI *SleepProc_guess)(DWORD);
    typedef void (__fastcall *WindowAction_guess)(void*);
    struct GuildMessage_guess
    {
        char name[32];
        int guildId;
        u8 flag;
        u8 unknown0x25[0x1FF];
    };
    char chatText[1024];
    GuildMessage_guess guildMessage;
    char reason[128];
    u32 corruptionMessage[23];
    int disconnectInfo[3];
    MsgSpawnPositionUpdate positionMessage;
    int framesToSkip;
    int savedState;
    int i;
    int canMove;
    int deathTime;
    EQPlayer* vehicle;
    EQPlayer* player;
    EQPlayer* controlPlayer;
    char* actorInfo;
    float floorZ;
    void* uiObject;

    framesToSkip = 30;
    EQ_CDECL(void, 0x004C2F7C, (void))();
    CEverQuest_ProcessMBox_guess(this);
    savedState = this->gameState;
    this->gameState = MAIN_LOOP_STATE_LOADING;
    EQ_METHOD(0, void, 0x004DB7F1, (void*), this)();
    this->gameState = savedState;
    if (g_pc_guess == NULL)
    {
        DbgLog_guess((const char*)0x005701B8);
    }
    else if (strlen(g_pc_guess->boat) > 0)
    {
        if (g_local_player_guess == NULL)
        {
            DbgLog_guess((const char*)0x00570184);
        }
        else
        {
            vehicle = EQ_METHOD(1, EQPlayer*, 0x004B146A, (void*, char*), g_player_top_guess)(
                EQ_METHOD(1, char*, 0x004D024E, (void*, char*), this)(g_pc_guess->boat));
            if (vehicle != NULL)
            {
                EQTRACE_guess((const char*)0x00570168, vehicle->name);
                g_local_player_guess->y = g_pc_guess->y;
                g_local_player_guess->x = g_pc_guess->x;
                g_local_player_guess->z = g_pc_guess->z;
                g_local_player_guess->heading = g_pc_guess->heading;
                g_local_player_guess->actor_info->ride_target_guess = vehicle;
                EQ_CDECL(void, 0x004E1109, (EQPlayer*))(vehicle);
                CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
                g_local_player_guess->actor_info->ride_target_guess = NULL;
                floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess, g_local_player_guess,
                    g_local_player_guess->y, g_local_player_guess->x, g_local_player_guess->z + 1.0f, 1);
                if (floorZ == MAIN_LOOP_NO_FLOOR || g_local_player_guess->actor_info->ride_target_guess == NULL)
                {
                    EQTRACE_guess((const char*)0x0057012C);
                    g_local_player_guess->y = 0.0f;
                    g_local_player_guess->x = 0.0f;
                    g_local_player_guess->z = 25.0f;
                    g_local_player_guess->actor_info->ride_target_guess = vehicle;
                    EQ_CDECL(void, 0x004E1109, (EQPlayer*))(vehicle);
                    CDisplay_UpdatePlayerActor_guess(g_display_guess, g_local_player_guess);
                    g_local_player_guess->actor_info->ride_target_guess = NULL;
                    floorZ = CDisplay_GetPlayerFloorHeight_guess(g_display_guess, g_local_player_guess,
                        g_local_player_guess->y, g_local_player_guess->x, g_local_player_guess->z + 1.0f, 1);
                }
                if (floorZ != MAIN_LOOP_NO_FLOOR)
                {
                    g_local_player_guess->actor_info->floor_z = floorZ;
                    g_local_player_guess->z = floorZ + g_local_player_guess->height_offset;
                }
                if (g_local_player_guess->actor_info->ride_target_guess == NULL)
                {
                    DbgLog_guess((const char*)0x005700E8, g_pc_guess->boat);
                    EQTRACE_guess((const char*)0x005700A4, g_pc_guess->boat);
                    EQ_METHOD(0, void, 0x00414E9B, (void*), g_display_guess)();
                }
            }
            else
            {
                EQTRACE_guess((const char*)0x00570070, g_pc_guess->boat);
                if (EQ_METHOD(0, bool, 0x004D5968, (void*), this)())
                {
                    this->gameState = MAIN_LOOP_STATE_RUNNING;
                    EQ_GLOBAL(char, 0x006B7110) = 1;
                    return;
                }
                DbgLog_guess((const char*)0x0057003C, g_pc_guess->boat);
                EQTRACE_guess((const char*)0x00570008, g_pc_guess->boat);
                EQ_METHOD(0, void, 0x00414E9B, (void*), g_display_guess)();
            }
        }
    }

    EQ_GLOBAL(SetWindowTextAProc_guess, 0x00534258)(window, (const char*)0x0056248C);
    for (i = 0; i < 5; i++)
    {
        EQ_ARRAY(char, 0x005FE8BD)[i * 0x1E] = 0;
        g_death_state_guess[i] = 0;
        EQ_ARRAY(char, 0x005FE8B8)[i] = 0;
        g_eqmain_guess[0xB4] = (char)0xFF;
        g_eqmain_guess[0x5F0C0 + i * 0x1E] = 0;
        *(u32*)(g_eqmain_guess + 0x5F158 + i * 4) = 0;
    }
    *(u32*)(g_eqmain_guess + 0x5F0BC) = 0;
    EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = NULL;
    this->gameState = MAIN_LOOP_STATE_RUNNING;
    EQ_GLOBAL(int, 0x006B63E8) = 0;
    EQ_GLOBAL(CriticalSectionProc_guess, 0x00534194)((LPCRITICAL_SECTION)0x006048E8);
    EQ_METHOD(0, void, 0x004DD666, (void*), this)();
    if (g_offlineMode_guess == 0 && DAT_006b6494 == 0)
    {
        EQ_METHOD(0, void, 0x004D58B4, (void*), this)();
        send_message_guess(g_world_connection_guess, 0x200A, NULL, 0, 1);
        while (EQ_GLOBAL(int, 0x006B63E8) == 0)
        {
            CEverQuest_ProcessMBox_guess(this);
            if ((EQ_GLOBAL(void*, 0x006B3194) != NULL
                    ? EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194)) : 0) == 0)
            {
                if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                {
                    EQ_CDECL(void, 0x004F2490, (void*, int*, int))(EQ_GLOBAL(void*, 0x006B3194), disconnectInfo, 0xC);
                    sprintf(reason, (const char*)0x0056F5DC, disconnectInfo[0]);
                }
                else
                {
                    strcpy(reason, (const char*)0x0056F5CC);
                }
                DbgLog_guess((const char*)0x0056F588, (const char*)0x0056F5A4, 0x41D1, reason);
                this->disconnected = 1;
                EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)((const char*)0x0056F574);
                EQ_GLOBAL(SleepProc_guess, 0x00534198)(3000);
                this->quitRequested = 1;
                this->gameState = MAIN_LOOP_STATE_DONE;
                EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
                return;
            }
        }
        EQ_GLOBAL(int, 0x006B63E8) = 0;
        if (g_eqmain_guess != NULL)
        {
            EQ_METHOD(0, void, 0x00456A14, (void*), g_eqmain_guess)();
            pinstEQ_Main_guess->uiDirty_guess = 1;
            pinstEQ_Main_guess->pendingServerReplies_guess = 0;
            pinstEQ_Main_guess->pendingWorldRequests_guess = 0;
        }
        EQ_METHOD(1, void, 0x004D5A24, (void*, int), this)(1);
        if (g_chatWindow_guess != NULL)
        {
            pinstEQ_TextIO_guess->redrawPass_guess = 1;
        }
        EQ_METHOD(0, void, 0x0048A6C4, (void*), g_chatWindow_guess)();
        uiObject = g_chatWindow_guess;
        ((WindowAction_guess)(*(void***)uiObject)[1])(uiObject);
        g_eqmain_guess[0xFE0] = 0;
        uiObject = g_eqmain_guess;
        ((WindowAction_guess)(*(void***)uiObject)[1])(uiObject);
        EQ_METHOD(0, void, 0x004D0540, (void*), this)();
        this->enteredWorld = 1;
        if (strcmp((const char*)0x006B772B, (const char*)0x0055D6D4) == 0)
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)((const char*)0x0056FFF0);
            if (EQ_GLOBAL(int, 0x006B6EF4) != 0 && EQ_CDECL(int, 0x004E7CF3, (void))() != 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)((const char*)0x0056FF98, 0xD);
            }
            for (i = 0; i < 15; i++)
            {
                if (g_pc_guess->buffs[i].visible != 0)
                {
                    EQ_GLOBAL(char, 0x006B658C) = 1;
                    if (g_eqmain_guess != NULL)
                    {
                        pinstEQ_Main_guess->uiDirty_guess = 1;
                    }
                }
            }
        }
        sprintf(chatText, (const char*)0x0056FF80, (const char*)0x006B660E);
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)(chatText);
        if (EQ_GLOBAL(int, 0x006B7108) == 4 && g_pc_guess->level < 6
            && EQ_METHOD(1, int, 0x004BFB7C, (void*, char*), EQ_GLOBAL(void*, 0x006E4D54))(g_pc_guess->zone) == 0)
        {
            sprintf(chatText, (const char*)0x0056FF54);
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(chatText, 0xD);
        }
        if (EQ_ARRAY(char, 0x006B7128)[0] != '\0')
        {
            if (EQ_ARRAY(char, 0x006B7128)[0] != '~')
            {
                sprintf(chatText, (const char*)0x00562A78, EQ_ARRAY(char, 0x006B7128));
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), this)(chatText, 0xF);
            }
            EQ_ARRAY(char, 0x006B7128)[0] = '\0';
        }
        send_message_guess(g_world_connection_guess, 0x20D8, NULL, 0, 1);
        while (EQ_GLOBAL(int, 0x006B63E8) == 0)
        {
            CEverQuest_ProcessMBox_guess(this);
            if ((EQ_GLOBAL(void*, 0x006B3194) != NULL
                    ? EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194)) : 0) == 0)
            {
                if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                {
                    EQ_CDECL(void, 0x004F2490, (void*, int*, int))(EQ_GLOBAL(void*, 0x006B3194), disconnectInfo, 0xC);
                    sprintf(reason, (const char*)0x0056F5DC, disconnectInfo[0]);
                }
                else
                {
                    strcpy(reason, (const char*)0x0056F5CC);
                }
                DbgLog_guess((const char*)0x0056F588, (const char*)0x0056F5A4, 0x4239, reason);
                this->disconnected = 1;
                EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)((const char*)0x0056F574);
                EQ_GLOBAL(SleepProc_guess, 0x00534198)(3000);
                this->quitRequested = 1;
                this->gameState = MAIN_LOOP_STATE_DONE;
                EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
                return;
            }
        }
        if (EQ_GLOBAL(short, 0x006B7528) == 1)
        {
            EQ_GLOBAL(short, 0x006B7528) = 0;
            if (g_local_player_guess->guild_id != -1)
            {
                guildMessage.guildId = g_local_player_guess->guild_id;
                guildMessage.flag = 0;
                strcpy(guildMessage.name, g_local_player_guess->name);
                send_message_guess(g_world_connection_guess, 0x2203, &guildMessage, 0x224, 1);
            }
        }
    }
    else
    {
        if (DAT_006b6494 != 0)
        {
            EQ_METHOD(0, void, 0x0040101D, (void*), g_display_guess)();
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)((const char*)0x0056FFF0);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)((const char*)0x0056FF38);
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)((const char*)0x0056FED4);
        }
        else
        {
            EQ_METHOD(0, void, 0x0040101D, (void*), g_display_guess)();
            EQ_METHOD(0, void, 0x00456A14, (void*), g_eqmain_guess)();
            pinstEQ_Main_guess->uiDirty_guess = 1;
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), this)((const char*)0x0056FFF0);
        }
    }
    EQ_GLOBAL(CriticalSectionProc_guess, 0x00534190)((LPCRITICAL_SECTION)0x006048E8);
    pinstEQ_Main_guess->pendingServerReplies_guess = 0;
    pinstEQ_Main_guess->pendingWorldRequests_guess = 0;
    EQ_CDECL(void, 0x00491632, (int))(0);
    send_message_guess(g_world_connection_guess, 0x2011, NULL, 0, 1);
    if (EQ_METHOD(0, int, 0x004CF561, (void*), this)() == 0)
    {
        corruptionMessage[0] = 0xFFFFFFFF;
        corruptionMessage[1] = 0xFFFFFFFF;
        corruptionMessage[2] = 0x3C;
        send_message_guess(g_world_connection_guess, 0x2147, corruptionMessage, 0x5C, 1);
    }
    for (player = g_player_top_guess; player != NULL; player = player->next)
    {
        EQ_METHOD(0, void, 0x004B0D95, (EQPlayer*), player)();
        if (player->actor_info != NULL)
        {
            EQ_METHOD(0, void, 0x004B4873, (EQPlayer*), player)();
        }
    }
    if (EQ_METHOD(0, int, 0x004CF561, (void*), this)() == 0)
    {
        corruptionMessage[0] = 0xFFFFFFFF;
        corruptionMessage[1] = 0xFFFFFFFF;
        corruptionMessage[2] = 0x46;
        send_message_guess(g_world_connection_guess, 0x2147, corruptionMessage, 0x5C, 1);
    }
    if (g_offlineMode_guess == 0)
    {
        EQ_METHOD(0, void, 0x004D56B0, (void*), this)();
        if (g_eqmain_guess != NULL)
        {
            EQ_CDECL(void, 0x004A204D, (CharacterProfile*))(g_pc_guess);
        }
    }
    if (EQ_METHOD(0, int, 0x004CF561, (void*), this)() == 0)
    {
        corruptionMessage[0] = 0xFFFFFFFF;
        corruptionMessage[1] = 0xFFFFFFFF;
        corruptionMessage[2] = 0x50;
        send_message_guess(g_world_connection_guess, 0x2147, corruptionMessage, 0x5C, 1);
    }
    EQ_METHOD(0, void, 0x0041527C, (void*), g_display_guess)();
    if (EQ_METHOD(0, int, 0x004CF561, (void*), this)() == 0)
    {
        corruptionMessage[0] = 0xFFFFFFFF;
        corruptionMessage[1] = 0xFFFFFFFF;
        corruptionMessage[2] = 0x5A;
        send_message_guess(g_world_connection_guess, 0x2147, corruptionMessage, 0x5C, 1);
    }
    if (g_no_render_offset_guess == 0)
    {
        EQ_CDECL(void, 0x004EB516, (void))();
        EQ_CDECL(void, 0x004EB0B1, (void))();
    }
    EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_local_player_guess)(100);

    while (this->gameState == MAIN_LOOP_STATE_RUNNING)
    {
        EQ_CDECL(unsigned int, 0x005227B1, (unsigned int, unsigned int))(0xA031F, 0xF031F);
        EQ_CDECL(void, 0x004DD0C2, (void))();
        CEverQuest_ProcessMBox_guess(this);
        if (EQ_GLOBAL(char, 0x006B7110) != 0)
        {
            if (g_local_player_guess->actor_info->unk_0x68 != 0)
            {
                g_activeView_guess = 0;
            }
            EQTRACE_guess((const char*)0x0056FEC0);
            return;
        }
        if (EQ_GLOBAL(char, 0x006B7111) == 0 && g_offlineMode_guess == 0)
        {
            if ((EQ_GLOBAL(void*, 0x006B3194) != NULL
                    ? EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194)) : 0) == 0)
            {
                if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                {
                    EQ_CDECL(void, 0x004F2490, (void*, int*, int))(EQ_GLOBAL(void*, 0x006B3194), disconnectInfo, 0xC);
                    sprintf(reason, (const char*)0x0056F5DC, disconnectInfo[0]);
                }
                else
                {
                    strcpy(reason, (const char*)0x0056F5CC);
                }
                DbgLog_guess((const char*)0x0056F588, (const char*)0x0056F5A4, 0x42E1, reason);
                this->disconnected = 1;
                EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), this)((const char*)0x0056F574);
                EQ_GLOBAL(SleepProc_guess, 0x00534198)(3000);
                EQ_GLOBAL(char, 0x006B7111) = 1;
            }
        }
        if (EQ_GLOBAL(char, 0x006B7111) != 0)
        {
            uiObject = EQ_GLOBAL(char*, 0x006E4E5C);
            if (*(int*)((char*)uiObject + 0x10) != 0)
            {
                *(*(char**)((char*)uiObject + 0x174) + 0xD6) = 0;
                uiObject = EQ_GLOBAL(char*, 0x006E4E5C);
                ((WindowAction_guess)(*(void***)uiObject)[2])(uiObject);
                *(int*)(EQ_GLOBAL(char*, 0x006E4E5C) + 0x174) = 0;
                return;
            }
            uiObject = EQ_GLOBAL(char*, 0x006E4E58);
            if (*(int*)((char*)uiObject + 0x10) != 0)
            {
                *(*(char**)((char*)uiObject + 0x174) + 0xD6) = 0;
                uiObject = EQ_GLOBAL(char*, 0x006E4E58);
                ((WindowAction_guess)(*(void***)uiObject)[2])(uiObject);
                *(int*)(EQ_GLOBAL(char*, 0x006E4E58) + 0x174) = 0;
                return;
            }
            if (g_eqmain_guess != NULL)
            {
                pinstEQ_Main_guess->lootOpen_guess = 0;
                pinstEQ_Main_guess->tradeOpen_guess = 0;
                pinstEQ_Main_guess->merchantOpen_guess = 0;
                g_eqmain_guess[0x100C] = 0;
                g_eqmain_guess[0x8FBAC] = 0;
                pinstEQ_Main_guess->bankOpen_guess = 0;
                pinstEQ_Main_guess->trackWindowOpen_guess = 0;
                pinstEQ_Main_guess->inventorySkillsTab_guess = 0;
                EQ_GLOBAL(int, 0x006E4DB8) = 0;
                EQ_GLOBAL(int, 0x006E4DBC) = 0;
                EQ_GLOBAL(int, 0x006E4DC0) = 0;
                EQ_GLOBAL(int, 0x006E4DC4) = 0;
                pinstEQ_Main_guess->pendingServerReplies_guess = 0;
                pinstEQ_Main_guess->pendingWorldRequests_guess = 0;
                uiObject = g_eqmain_guess;
                ((WindowAction_guess)(*(void***)uiObject)[2])(uiObject);
                g_eqmain_guess[0xFE0] = 0;
                g_eqmain_guess[0xFE2] = 0;
                g_eqmain_guess[0xFE1] = 0;
                uiObject = g_eqmain_guess;
                ((WindowAction_guess)(*(void***)uiObject)[1])(uiObject);
                pinstEQ_Main_guess->uiDirty_guess = 1;
            }
            if (g_offlineMode_guess == 0 && DAT_006b6494 == 0)
            {
                if (EQ_GLOBAL(char, 0x006B7111) != 1)
                {
                    if (g_local_player_guess != NULL
                        && g_local_player_guess->actor_info->unk_0x68 == 0)
                    {
                        EQTRACE_guess((const char*)0x0056FE70);
                        EQ_GLOBAL(int, 0x006B7120) = 0x1A;
                        EQ_METHOD(1, void, 0x004C71A3, (void*, int), this)(0);
                        EQ_GLOBAL(SleepProc_guess, 0x00534198)(2000);
                    }
                    this->gameState = MAIN_LOOP_STATE_DONE;
                    this->disconnected = 1;
                    EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
                    this->quitRequested = 1;
                    if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                    {
                        EQ_CDECL(void, 0x004F21D0, (void*, int, int, int))(EQ_GLOBAL(void*, 0x006B3194), 10000, 0, 0);
                        EQ_GLOBAL(void*, 0x006B3194) = NULL;
                    }
                    return;
                }
                EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_ZONE;
                if (EQ_GLOBAL(void*, 0x006B3194) != NULL
                    && EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194)) != 0)
                {
                    if (g_local_player_guess != NULL)
                    {
                        deathTime = g_local_player_guess->actor_info->unk_0x68;
                        if (deathTime == 0)
                        {
                            EQ_GLOBAL(int, 0x006B7120) = 0x1A;
                            g_pc_guess->y = g_local_player_guess->y;
                            g_pc_guess->x = g_local_player_guess->x;
                            g_pc_guess->z = g_local_player_guess->z;
                            g_pc_guess->heading = g_local_player_guess->heading;
                            EQ_METHOD(1, void, 0x004C71A3, (void*, int), this)(0);
                            send_message_guess(g_world_connection_guess, 0x202B, &g_local_player_guess->spawn_id, 4, 1);
                        }
                        else
                        {
                            EQTRACE_guess((const char*)0x0056FE90, deathTime);
                        }
                    }
                    else
                    {
                        EQTRACE_guess((const char*)0x0056FEA8);
                    }
                }
                EQ_CDECL(void, 0x004E0632, (void))();
                if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && EQ_GLOBAL(void*, 0x006ECDF4) != NULL)
                {
                    EQ_METHOD(0, void, 0x004E4AA5, (void*), EQ_GLOBAL(void*, 0x006ECDF4))();
                }
                switch (EQ_GLOBAL(int, 0x006E4E8C))
                {
                case 1:
                    EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
                    break;
                case 2:
                    EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
                    EQ_GLOBAL(int, 0x006E4E8C) = 0;
                    break;
                }
            }
            else
            {
                EQ_GLOBAL(int, 0x006B63F4) = MAIN_LOOP_REASON_DATA;
            }
            EQ_GLOBAL(char, 0x006B7111) = 0;
            return;
        }

        canMove = 1;
        if ((char)g_pc_guess->unknown0x0076 > 2 || g_local_player_guess->stand_state == 'f')
        {
            canMove = 0;
        }
        if (g_offlineMode_guess != 0 || DAT_006b6494 != 0)
        {
            canMove = 1;
        }
        if (g_offlineMode_guess == 0 && DAT_006b6494 == 0)
        {
            if (EQ_GLOBAL(char, 0x006B710F) != 0 && canMove)
            {
                if (EQ_GLOBAL(u32, 0x006B64DC) > MAIN_LOOP_DISPLAY_TIME)
                {
                    EQ_GLOBAL(u32, 0x006B64DC) = MAIN_LOOP_DISPLAY_TIME;
                }
                if (MAIN_LOOP_DISPLAY_TIME - EQ_GLOBAL(u32, 0x006B64DC) > 300)
                {
                    positionMessage.spawn_id = (u16)g_local_player_guess->spawn_id;
                    EQ_CDECL(void, 0x004A3534, (float*, PackedPhysics*, EQPlayer*))
                        (&g_local_player_guess->y, &positionMessage.physics, g_local_player_guess);
                    send_message_guess(g_world_connection_guess, 0x20F3, &positionMessage, 0xF, 1);
                    EQ_GLOBAL(char, 0x006B710F) = 0;
                    EQ_GLOBAL(u32, 0x006B64DC) = MAIN_LOOP_DISPLAY_TIME;
                }
            }
            if (g_local_player_guess != g_control_player_guess && g_control_player_guess != NULL
                && EQ_GLOBAL(char, 0x006B710E) != 0 && canMove)
            {
                if (EQ_GLOBAL(u32, 0x006B64E0) > MAIN_LOOP_DISPLAY_TIME)
                {
                    EQ_GLOBAL(u32, 0x006B64E0) = MAIN_LOOP_DISPLAY_TIME;
                }
                if (MAIN_LOOP_DISPLAY_TIME - EQ_GLOBAL(u32, 0x006B64E0) > 300)
                {
                    positionMessage.spawn_id = (u16)g_control_player_guess->spawn_id;
                    EQ_CDECL(void, 0x004A3534, (float*, PackedPhysics*, EQPlayer*))
                        (&g_control_player_guess->y, &positionMessage.physics, g_control_player_guess);
                    send_message_guess(g_world_connection_guess, 0x20F3, &positionMessage, 0xF, 1);
                    EQ_GLOBAL(char, 0x006B710E) = 0;
                    EQ_GLOBAL(u32, 0x006B64E0) = MAIN_LOOP_DISPLAY_TIME;
                }
            }
            if (EQ_GLOBAL(char, 0x006B710F) != 0 && g_local_player_guess->stand_state == 'f'
                && EQ_GLOBAL(int, 0x006E2054) == 0)
            {
                if (EQ_GLOBAL(u32, 0x006B64DC) > MAIN_LOOP_DISPLAY_TIME)
                {
                    EQ_GLOBAL(u32, 0x006B64DC) = MAIN_LOOP_DISPLAY_TIME;
                }
                if (MAIN_LOOP_DISPLAY_TIME - EQ_GLOBAL(u32, 0x006B64DC) > 300)
                {
                    positionMessage.spawn_id = (u16)g_local_player_guess->spawn_id;
                    EQ_CDECL(void, 0x004A3534, (float*, PackedPhysics*, EQPlayer*))
                        (&g_local_player_guess->y, &positionMessage.physics, g_local_player_guess);
                    send_message_guess(g_world_connection_guess, 0x20F3, &positionMessage, 0xF, 1);
                    EQ_GLOBAL(char, 0x006B710F) = 0;
                    EQ_GLOBAL(u32, 0x006B64DC) = MAIN_LOOP_DISPLAY_TIME;
                }
            }
        }
        else
        {
            g_pc_guess->fatigue = 0;
        }
        if (g_no_render_offset_guess == 0)
        {
            EQ_CDECL(void, 0x004C2C1A, (void))();
            EQ_CDECL(void, 0x004EB0E0, (void))();
        }
        EQ_GLOBAL(int, 0x006B63F4) = 0;
        EQ_METHOD(1, void, 0x004BE74B, (void*, u32), EQ_GLOBAL(void*, 0x006E4D54))(EqGetTime());
        controlPlayer = g_control_player_guess;
        if (controlPlayer != NULL)
        {
            actorInfo = (char*)controlPlayer->actor_info;
            if (controlPlayer->race == 0x8F || controlPlayer->race == 0x8E)
            {
                EQ_GLOBAL(u32, 0x006B646C) = 0;
            }
            else
            {
                if (EQ_GLOBAL(char, 0x006B640A) != 0)
                {
                    EQ_GLOBAL(u32, 0x006B646C) = 0;
                    if (controlPlayer->stand_state != 'o')
                    {
                        EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), controlPlayer)(0x6F);
                    }
                }
                if (EQ_GLOBAL(u32, 0x006B646C) > MAIN_LOOP_DISPLAY_TIME)
                {
                    EQ_GLOBAL(u32, 0x006B646C) = 0;
                }
            }
            if (EQ_GLOBAL(u32, 0x006B646C) != 0)
            {
                if (MAIN_LOOP_DISPLAY_TIME - EQ_GLOBAL(u32, 0x006B646C) < 500)
                {
                    if (g_control_player_guess->speed > 0.0f)
                    {
                        EQ_GLOBAL(u32, 0x006B646C) = 0;
                        if (EQ_METHOD(0, bool, 0x004B41A6, (EQPlayer*), g_control_player_guess)())
                        {
                            if (g_control_player_guess->stand_state == 'o')
                            {
                                EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_control_player_guess)(100);
                            }
                            EQ_GLOBAL(u32, 0x006B6470) = MAIN_LOOP_DISPLAY_TIME;
                            if (g_local_player_guess == g_control_player_guess)
                            {
                                EQ_GLOBAL(char, 0x006B710F) = 1;
                            }
                            else
                            {
                                EQ_GLOBAL(char, 0x006B710E) = 1;
                            }
                            controlPlayer->dz = (g_control_player_guess == g_local_player_guess
                                    ? EQ_METHOD(0, float, 0x0041BB36, (void*), g_pc_guess)() * ((EQActorInfo_guess*)actorInfo)->unk_0xAC
                                    : ((EQActorInfo_guess*)actorInfo)->unk_0xAC)
                                * 1.65f + controlPlayer->dz;
                            if (g_control_player_guess->sneaking == 0)
                            {
                                EQ_CDECL(void, 0x004E1C10, (EQPlayer*, float))(g_control_player_guess, 1.0f);
                            }
                            send_message_guess(g_world_connection_guess, 0x2020, NULL, 0, 1);
                            EQ_METHOD(1, void, 0x00480DFB, (void*, int), g_pc_guess)(g_pc_guess->fatigue + 10);
                        }
                    }
                    else if (g_control_player_guess->speed < 0.0f)
                    {
                        EQ_GLOBAL(u32, 0x006B646C) = 0;
                    }
                    else
                    {
                        if (g_control_player_guess->stand_state == 'd')
                        {
                            EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_control_player_guess)(0x6F);
                        }
                        if (g_control_player_guess->stand_state != 'o')
                        {
                            EQ_GLOBAL(u32, 0x006B646C) = 0;
                        }
                        if (g_local_player_guess == g_control_player_guess)
                        {
                            EQ_GLOBAL(char, 0x006B710F) = 1;
                        }
                        else
                        {
                            EQ_GLOBAL(char, 0x006B710E) = 1;
                        }
                    }
                }
                else
                {
                    EQ_GLOBAL(u32, 0x006B646C) = 0;
                    if (g_control_player_guess->stand_state == 'o')
                    {
                        EQ_METHOD(1, void, 0x004B45CB, (EQPlayer*, int), g_control_player_guess)(100);
                        if (EQ_METHOD(0, bool, 0x004B41A6, (EQPlayer*), g_control_player_guess)())
                        {
                            EQ_GLOBAL(u32, 0x006B6470) = MAIN_LOOP_DISPLAY_TIME;
                            if (g_local_player_guess == g_control_player_guess)
                            {
                                EQ_GLOBAL(char, 0x006B710F) = 1;
                            }
                            else
                            {
                                EQ_GLOBAL(char, 0x006B710E) = 1;
                            }
                            controlPlayer->dz = (g_control_player_guess == g_local_player_guess
                                    ? EQ_METHOD(0, float, 0x0041BB36, (void*), g_pc_guess)() * ((EQActorInfo_guess*)actorInfo)->unk_0xAC
                                    : ((EQActorInfo_guess*)actorInfo)->unk_0xAC)
                                * 1.65f + controlPlayer->dz;
                            send_message_guess(g_world_connection_guess, 0x2020, NULL, 0, 1);
                            EQ_METHOD(1, void, 0x00480DFB, (void*, int), g_pc_guess)(g_pc_guess->fatigue + 10);
                        }
                        else
                        {
                            EQ_GLOBAL(u32, 0x006B646C) = 0;
                        }
                    }
                }
            }
        }
        if (canMove)
        {
            EQ_METHOD(0, void, 0x004D6F06, (void*), this)();
        }
        if (framesToSkip == 0)
        {
            EQ_METHOD(0, void, 0x0040C30A, (void*), g_display_guess)();
        }
        else
        {
            framesToSkip--;
        }
    }
}
#undef CEverQuest

class CEverQuest_ProcessControls_standin_guess
{
public:
    void ProcessControls_guess(void);
};

#define CEverQuest CEverQuest_ProcessControls_standin_guess
// FUNCTION: TRILOGY 0x004d6f06
// Per-frame movement controls of the controlled spawn: turn, forward/back speed, jump, look up/down, camera.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest::ProcessControls_guess(void)
{
    struct ControlPlayerMessage_guess
    {
        u32 spawnId;
        u8 status;
        u8 padding[3];
    } controlMessage;
    float value;
    EQPlayer* movementPlayer;
    EQPlayer* controlPlayer;
    char onGround;
    int chatTextLength;
    u32 scratchValue;
    u32 skillValue;
    int frameOffset;
    int frameIndex;
    int jumpTimer;
    int movementAllowed;
    EQActorInfo_guess* actorInfo;
    long double movementFactor;
    char chatBuffer[2048];
    char* mainWindow;
    typedef BOOL (WINAPI *ProcessControlsPostMessageA_t)(HWND, UINT, WPARAM, LPARAM);

    movementAllowed = 1;
    mainWindow = g_eqmain_guess;
    if (mainWindow != 0 && '\a' < *(char*)(mainWindow + 0xFE0))
        return;
    if ((0 < (char)EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->unknown0x0076)
        || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'f')
    {
        movementAllowed = 0;
    }
    if (EQ_GLOBAL(char, 0x006E20F4) != '\0')
        return;
    if (EQ_GLOBAL(char, 0x006B63F8) != '\0' || DAT_006b6494 != 0)
        movementAllowed = 1;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) != EQ_GLOBAL(EQPlayer*, 0x006E4D8C)
        && EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D
        && EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->ride_target_guess
            != EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
    {
        controlMessage.spawnId = EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->spawn_id;
        controlMessage.status = 0;
        send_message_guess
            (g_world_connection_guess, 0x2126, &controlMessage, 8, 1);
        EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->speed = 0.0f;
        EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->delta_heading = 0.0f;
        EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
    }

    movementPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4DA0);
    if (movementPlayer->stand_state != 'd' && movementPlayer->stand_state != 'o')
        movementAllowed = 0;
    EQ_GLOBAL(char, 0x006B645B) = '\0';
    actorInfo = movementPlayer->actor_info;
    if (movementAllowed != 0)
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->delta_heading = 0.0f;
        movementPlayer->speed = 0.0f;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
        {
            EQ_METHOD(0, void, 0x004B0EEC, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
        }
    }
    if (-1000.0f <= actorInfo->unk_0xB8)
    {
        if (1.092006f < actorInfo->unk_0xB8)
            actorInfo->unk_0xB8 = 0.0f;
    }
    else
    {
        movementPlayer->delta_heading = 0.0f;
        movementPlayer->speed = 0.0f;
    }

    mainWindow = g_eqmain_guess;
    if (mainWindow != 0)
    {
        if (g_attacking_guess == 1 && movementAllowed != 0
            && EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != 0
            && *(int*)(mainWindow + 0xEA4) != 0)
        {
            *(int*)(mainWindow + 0xEA4) = 0;
            EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, int, EQPlayer*),
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                (0x0D, 0, EQ_GLOBAL(EQPlayer*, 0x006E4DAC));
            *(u32*)(mainWindow + 0xEB8) = 1;
        }
        if (g_rangedAttacking_guess == 1 && movementAllowed != 0
            && EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != 0
            && *(int*)(mainWindow + 0xEA8) != 0)
        {
            *(int*)(mainWindow + 0xEA8) = 0;
            EQ_METHOD(3, void, 0x004B2C1E, (EQPlayer*, int, int, EQPlayer*),
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                (0x0E, 0, EQ_GLOBAL(EQPlayer*, 0x006E4DAC));
            *(u32*)(mainWindow + 0xEB8) = 1;
        }
    }

    frameIndex = g_activeView_guess;
    if (frameIndex != 0 && EQ_GLOBAL(int, 0x006E1B78) != 0
        && EQ_ARRAY(char, 0x006B6F30)[frameIndex * 0x1C] != '\0')
    {
        EQ_GLOBAL(char, 0x006B645B) = '\0';
        value = EQ_GLOBAL(float, 0x006E2894) * EQ_GLOBAL(float, 0x0053575C)
            + EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7];
        EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7] = value;
        value = EQ_METHOD(1, float, 0x00408156, (void*, float),
            g_display_guess)(value);
        EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7] = value;
    }

    if (EQ_GLOBAL(int, 0x006E19FC) != 0)
    {
        EQ_METHOD(0, void, 0x004B117B, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D
            || (EQ_GLOBAL(int, 0x006B70EC) == 0
                && (EQ_GLOBAL(char, 0x006B645A) == '\0' || frameIndex != 0)))
        {
            if (movementAllowed != 0)
            {
                EQ_GLOBAL(short, 0x006B7104) = EQ_GLOBAL(short, 0x006B7104) + 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D)
                {
                    if (3 < EQ_GLOBAL(short, 0x006B7104))
                        EQ_GLOBAL(short, 0x006B7104) = 3;
                }
                else if (0x0C < EQ_GLOBAL(short, 0x006B7104))
                    EQ_GLOBAL(short, 0x006B7104) = 0x0C;
                scratchValue = (u32)EQ_GLOBAL(short, 0x006B7104);
                movementPlayer->delta_heading = (float)(int)scratchValue;
            }
        }
        else
        {
            if (movementAllowed != 0
                && -1000.0f <= actorInfo->unk_0xB8)
            {
                EQ_GLOBAL(char, 0x006B645B) = '\x01';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                    movementFactor = (long double)EQ_METHOD(0, float, 0x0041BB36,
                        (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
                else
                    movementFactor = 1.0L;
                EQ_GLOBAL(float, 0x006B711C) = (float)(movementFactor + movementFactor);
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->sneaking != '\0'
                    || actorInfo->unk_0xB8 < 0.0f
                    || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'o')
                {
                    EQ_GLOBAL(float, 0x006B711C) = EQ_GLOBAL(float, 0x006B711C)
                        * EQ_GLOBAL(float, 0x005344A8);
                }
            }
            if (actorInfo->unk_0xB8 < -1000.0f)
            {
                EQ_GLOBAL(char, 0x006B645B) = '\0';
                EQ_GLOBAL(float, 0x006B711C) = 0.0f;
            }
        }
    }

    if (EQ_GLOBAL(int, 0x006E1D1C) == 0)
    {
        EQ_GLOBAL(int, 0x006B65B8) = 0;
    }
    else if ((EQ_GLOBAL(int, 0x006B65B8) == *(int*)(g_displayManager_guess + 0xBC)
        || 500 < (u32)(*(int*)(g_displayManager_guess + 0xBC)
            - EQ_GLOBAL(int, 0x006B65B8)))
        && EQ_METHOD(0, int, 0x004D7CA7, (CEverQuest*), this)() == 2
        && (chatTextLength = pinstEQ_TextIO_guess->inputCursor) != 0)
    {
        strcpy(chatBuffer, (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
        strcpy(chatBuffer + chatTextLength - 1,
                (char*)EQ_GLOBAL(void*, 0x006EFE2C) + chatTextLength + 0xBA8);
        strcpy((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8, chatBuffer);
        EQ_METHOD(1, void, 0x004CF7A1, (CEverQuest*, int), this)
            (pinstEQ_TextIO_guess->inputCursor - 1);
        EQ_GLOBAL(ProcessControlsPostMessageA_t, 0x0053422C)
            ((HWND)EQ_GLOBAL(u32, 0x006B63D8), 0x4CA, 0, 0);
    }

    frameIndex = g_activeView_guess;
    if (frameIndex != 0 && EQ_GLOBAL(int, 0x006E1B74) != 0
        && EQ_ARRAY(char, 0x006B6F30)[frameIndex * 0x1C] != '\0')
    {
        EQ_GLOBAL(char, 0x006B645B) = '\0';
        value = EQ_GLOBAL(float, 0x006E2894) * EQ_GLOBAL(float, 0x00535634)
            + EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7];
        EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7] = value;
        value = EQ_METHOD(1, float, 0x00408156, (void*, float),
            g_display_guess)(value);
        EQ_ARRAY(float, 0x006B6F1C)[frameIndex * 7] = value;
    }

    if (EQ_GLOBAL(int, 0x006E1B2C) == 0)
    {
        if (EQ_GLOBAL(int, 0x006E1B28) != 0
            && EQ_METHOD(0, int, 0x004D7CA7, (CEverQuest*),
                EQ_GLOBAL(CEverQuest*, 0x006EFE2C))() == 2
            && g_nextUpdateTime_guess
                < g_cdisplay_guess->time_ms_guess)
        {
            EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x51, 1, 0);
        }
    }
    else if (EQ_METHOD(0, int, 0x004D7CA7, (CEverQuest*),
            EQ_GLOBAL(CEverQuest*, 0x006EFE2C))() == 2
        && g_nextUpdateTime_guess
            < g_cdisplay_guess->time_ms_guess)
    {
        EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0x52, 1, 0);
    }

    actorInfo = movementPlayer->actor_info;
    if (EQ_GLOBAL(int, 0x006E19F8) != 0)
    {
        EQ_METHOD(0, void, 0x004B117B, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
        actorInfo = movementPlayer->actor_info;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D
            || (EQ_GLOBAL(int, 0x006B70EC) == 0
                && (EQ_GLOBAL(char, 0x006B645A) == '\0' || g_activeView_guess != 0)))
        {
            if (movementAllowed != 0)
            {
                EQ_GLOBAL(short, 0x006B7106) = EQ_GLOBAL(short, 0x006B7106) + 1;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D)
                {
                    if (3 < EQ_GLOBAL(short, 0x006B7106))
                        EQ_GLOBAL(short, 0x006B7106) = 3;
                }
                else if (0x0C < EQ_GLOBAL(short, 0x006B7106))
                    EQ_GLOBAL(short, 0x006B7106) = 0x0C;
                scratchValue = -(int)EQ_GLOBAL(short, 0x006B7106);
                movementPlayer->delta_heading = (float)(int)scratchValue;
            }
        }
        else
        {
            if (movementAllowed != 0
                && -1000.0f <= actorInfo->unk_0xB8)
            {
                EQ_GLOBAL(char, 0x006B645B) = '\x02';
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                    movementFactor = (long double)EQ_METHOD(0, float, 0x0041BB36,
                        (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
                else
                    movementFactor = 1.0L;
                EQ_GLOBAL(float, 0x006B711C) = (float)(movementFactor + movementFactor);
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->sneaking != '\0'
                    || actorInfo->unk_0xB8 < 0.0f
                    || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'o')
                {
                    EQ_GLOBAL(float, 0x006B711C) = EQ_GLOBAL(float, 0x006B711C)
                        * EQ_GLOBAL(float, 0x005344A8);
                }
            }
            if (actorInfo->unk_0xB8 < -1000.0f)
            {
                EQ_GLOBAL(char, 0x006B645B) = '\0';
                EQ_GLOBAL(float, 0x006B711C) = 0.0f;
            }
        }
    }

    if (EQ_GLOBAL(char, 0x006B645A) == '\0')
    {
        if (EQ_GLOBAL(int, 0x006B68A4) != 0 && g_activeView_guess == 0
            && EQ_GLOBAL(int, 0x006E1A20) == 0 && EQ_GLOBAL(int, 0x006E1A24) == 0)
        {
            if (0.0f < *(float*)&movementPlayer->unk_0x60)
            {
                if (*(float*)&movementPlayer->unk_0x60 <= 10.0f)
                    value = 0.0f;
                else
                    value = *(float*)&movementPlayer->unk_0x60 - 10.0f;
                *(float*)&movementPlayer->unk_0x60 = value;
            }
            if (*(float*)&movementPlayer->unk_0x60 < 0.0f)
            {
                if (-10.0f <= *(float*)&movementPlayer->unk_0x60)
                    *(float*)&movementPlayer->unk_0x60 = 0.0f;
                else
                    *(float*)&movementPlayer->unk_0x60 = *(float*)&movementPlayer->unk_0x60 + 10.0f;
            }
        }
    }
    else if (g_activeView_guess != 0
        || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'd'
        || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'o')
    {
        EQ_METHOD(1, void, 0x004CFFB1, (CEverQuest*, int), this)(movementAllowed);
    }

    jumpTimer = EQ_GLOBAL(int, 0x006B646C);
    if (movementAllowed != 0
        && EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->levitate == 0
        && EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race != 0x8D
        && -1000.0f <= actorInfo->unk_0xB8
        && EQ_GLOBAL(int, 0x006E19EC) != 0
        && actorInfo->unknown0x099 == '\0'
        && (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'd'
            || EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state == 'o')
        && EQ_GLOBAL(int, 0x006B646C) == 0)
    {
        if (g_cdisplay_guess->time_ms_guess < (u32)EQ_GLOBAL(int, 0x006B6470))
            EQ_GLOBAL(int, 0x006B6470) = 0;
        onGround = EQ_METHOD(0, bool, 0x004B41A6, (EQPlayer*),
            EQ_GLOBAL(EQPlayer*, 0x006E4DA0))();
        jumpTimer = EQ_GLOBAL(int, 0x006B646C);
        if (onGround != '\0'
            && 0x5DC < (u32)(g_cdisplay_guess->time_ms_guess
                - (u32)EQ_GLOBAL(int, 0x006B6470)))
        {
            jumpTimer = *(int*)(g_displayManager_guess + 0xBC);
            if ('Y' < EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->fatigue)
            {
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int),
                    EQ_GLOBAL(CEverQuest*, 0x006EFE2C))
                    (EQ_ARRAY(const char, 0x005701E4), 0x0F);
                jumpTimer = EQ_GLOBAL(int, 0x006B646C);
            }
            else
                EQ_GLOBAL(int, 0x006B646C) = jumpTimer;
        }
    }
    controlPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4DA0);
    frameIndex = g_activeView_guess;
    EQ_GLOBAL(int, 0x006E19EC) = 0;
    if (EQ_GLOBAL(int, 0x006E1A2C) != 0
        && (frameOffset = frameIndex * 0x1C,
            EQ_ARRAY(char, 0x006B6F30)[frameOffset] != '\0')
        && (value = EQ_GLOBAL(float, 0x006E2894) * EQ_GLOBAL(float, 0x00535758)
            + EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7],
            EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7] = value,
            value < controlPlayer->height_offset))
    {
        EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7] = controlPlayer->height_offset;
    }
    if (EQ_GLOBAL(int, 0x006E1A30) != 0
        && (frameOffset = frameIndex * 0x1C,
            EQ_ARRAY(char, 0x006B6F30)[frameOffset] != '\0')
        && (value = EQ_GLOBAL(float, 0x006E2894) + EQ_GLOBAL(float, 0x006E2894)
            + EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7],
            EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7] = value,
            400.0f < value))
    {
        *(u32*)&EQ_ARRAY(float, 0x006B6F18)[frameIndex * 7] = 0x43C80000;
    }
    if (frameIndex != 0 && EQ_GLOBAL(int, 0x006E1B6C) != 0)
    {
        EQ_ARRAY(float, 0x006B6F20)[frameIndex * 7] = EQ_GLOBAL(float, 0x006E2894)
            + EQ_ARRAY(float, 0x006B6F20)[frameIndex * 7];
    }

    if ((EQ_GLOBAL(int, 0x006E19F0) != 0 || EQ_GLOBAL(int, 0x006B6474) != 0
            || EQ_GLOBAL(char, 0x006B6408) != '\0')
        && movementAllowed != 0)
    {
        if (controlPlayer->race == 0x8D)
        {
            movementPlayer->speed = controlPlayer->walk_speed;
            if (-1000.0f <= actorInfo->unk_0xB8)
                goto forward_speed_done;
        }
        else
        {
            if (controlPlayer->sneaking == '\0')
            {
                if (controlPlayer->stand_state == 'o')
                {
                    value = controlPlayer->walk_speed * EQ_GLOBAL(float, 0x005344A8);
                    goto walking_speed;
                }
                if (EQ_GLOBAL(u8, 0x006B6405) == 0)
                    movementPlayer->speed = controlPlayer->walk_speed;
                else
                {
                    scratchValue = (u32)EQ_GLOBAL(u8, 0x006B6405);
                    movementPlayer->speed = (float)scratchValue * controlPlayer->run_speed;
                }
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                {
                    movementFactor = (long double)EQ_METHOD(0, float, 0x0041BB36,
                        (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
                    movementPlayer->speed = (float)(movementFactor * (long double)movementPlayer->speed);
                }
                if (4 < actorInfo->unknown0x099)
                {
                    skillValue = EQ_METHOD(1, u8, 0x004246A9, (void*, int),
                        EQ_GLOBAL(void*, 0x006E4DA8))(0x32);
                    scratchValue = skillValue & 0xFF;
                    movementPlayer->speed = (float)scratchValue
                        * (float)EQ_GLOBAL(double, 0x00535238) + movementPlayer->speed;
                    if (0.0f < *(float*)&movementPlayer->unk_0x60)
                    {
                        EQ_METHOD(2, void, 0x004146A3, (void*, EQPlayer*, char),
                            g_display_guess)
                            (EQ_GLOBAL(EQPlayer*, 0x006E4DA0), 1);
                    }
                }
            }
            else
            {
                value = actorInfo->sneak_mod_guess * controlPlayer->walk_speed;
walking_speed:
                movementPlayer->speed = value;
                if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
                {
                    movementFactor = (long double)EQ_METHOD(0, float, 0x0041BB36,
                        (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
                    movementPlayer->speed = (float)(movementFactor * (long double)movementPlayer->speed);
                }
            }
            if (-1000.0f <= actorInfo->unk_0xB8)
            {
                value = actorInfo->unk_0xB8 + movementPlayer->speed;
                movementPlayer->speed = value;
                if (value < 0.0f)
                    movementPlayer->speed = 0.01f;
                goto forward_speed_done;
            }
        }
        movementPlayer->speed = 0.0f;
    }
forward_speed_done:
    if (g_activeView_guess != 0 && EQ_GLOBAL(int, 0x006E1B70) != 0)
    {
        frameOffset = g_activeView_guess * 0x1C;
        EQ_ARRAY(float, 0x006B6F20)[frameOffset / 4] = EQ_GLOBAL(float, 0x006E2894)
            * EQ_GLOBAL(float, 0x005345BC)
            + EQ_ARRAY(float, 0x006B6F20)[frameOffset / 4];
    }

    if (EQ_GLOBAL(int, 0x006E19F4) == 0)
        goto movement_complete;
    EQ_METHOD(0, void, 0x004B117B, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
    if (movementAllowed == 0)
        goto movement_complete;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D)
    {
        movementPlayer->speed = -EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->walk_speed;
        if (-1000.0f <= actorInfo->unk_0xB8)
            goto movement_complete;
    }
    else
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->sneaking == '\0'
            && EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->stand_state != 'o')
        {
            if (actorInfo->unknown0x099 == '\0')
            {
                value = EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->walk_speed;
                goto reverse_speed;
            }
            movementFactor = 0.0L;
            goto set_reverse_speed;
        }
        else
        {
            value = EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->walk_speed * EQ_GLOBAL(float, 0x005344A8);
reverse_speed:
            movementPlayer->speed = -value;
            if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0) == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
            {
                movementFactor = (long double)EQ_METHOD(0, float, 0x0041BB36,
                    (void*), EQ_GLOBAL(void*, 0x006E4DA8))();
                movementFactor = movementFactor * (long double)movementPlayer->speed;
                goto set_reverse_speed;
            }
            goto reverse_velocity_check;
        }
set_reverse_speed:
        movementPlayer->speed = (float)movementFactor;
reverse_velocity_check:
        if (-1000.0f <= actorInfo->unk_0xB8)
        {
            value = movementPlayer->speed - actorInfo->unk_0xB8;
            movementPlayer->speed = value;
            if (0.0f < value)
                movementPlayer->speed = -0.01f;
            goto movement_complete;
        }
    }
    movementPlayer->speed = 0.0f;

movement_complete:
    if (g_activeView_guess != 0 && EQ_GLOBAL(int, 0x006E1B7C) != 0)
    {
        frameOffset = g_activeView_guess * 0x1C;
        value = EQ_GLOBAL(float, 0x006E2894) * EQ_GLOBAL(float, 0x00535120)
            + EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4];
        EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4] = value;
        if (128.0f < value)
            EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4] = 128.0f;
    }

    if (EQ_GLOBAL(int, 0x006E1A20) != 0)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D)
        {
            EQ_GLOBAL(short, 0x006B70FC) = EQ_GLOBAL(short, 0x006B70FC) + 1;
            if (8 < EQ_GLOBAL(short, 0x006B70FC))
                EQ_GLOBAL(short, 0x006B70FC) = 8;
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess =
                (float)(int)EQ_GLOBAL(short, 0x006B70FC)
                + EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess;
            if (128.0f < EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess)
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess = 128.0f;
        }
        else
        {
            EQ_GLOBAL(short, 0x006B70FC) = EQ_GLOBAL(short, 0x006B70FC) + 1;
            if (8 < EQ_GLOBAL(short, 0x006B70FC))
                EQ_GLOBAL(short, 0x006B70FC) = 8;
            value = (float)(int)EQ_GLOBAL(short, 0x006B70FC)
                + *(float*)&movementPlayer->unk_0x60;
            *(float*)&movementPlayer->unk_0x60 = value;
            if (128.0f < value)
                *(float*)&movementPlayer->unk_0x60 = 128.0f;
        }
    }
    if (g_activeView_guess != 0 && EQ_GLOBAL(int, 0x006E1B80) != 0)
    {
        frameOffset = g_activeView_guess * 0x1C;
        value = EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4]
            - EQ_GLOBAL(float, 0x006E2894) * EQ_GLOBAL(float, 0x00535120);
        EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4] = value;
        if (value < -128.0f)
            EQ_ARRAY(float, 0x006B6F28)[frameOffset / 4] = -128.0f;
    }
    if (EQ_GLOBAL(int, 0x006E1A24) != 0)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->race == 0x8D)
        {
            EQ_GLOBAL(short, 0x006B70FE) = EQ_GLOBAL(short, 0x006B70FE) + 1;
            if (8 < EQ_GLOBAL(short, 0x006B70FE))
                EQ_GLOBAL(short, 0x006B70FE) = 8;
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess =
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess
                - (float)(int)EQ_GLOBAL(short, 0x006B70FE);
            if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess < -128.0f)
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->pitchState_guess = -128.0f;
        }
        else
        {
            EQ_GLOBAL(short, 0x006B70FE) = EQ_GLOBAL(short, 0x006B70FE) + 1;
            if (8 < EQ_GLOBAL(short, 0x006B70FE))
                EQ_GLOBAL(short, 0x006B70FE) = 8;
            value = *(float*)&movementPlayer->unk_0x60
                - (float)(int)EQ_GLOBAL(short, 0x006B70FE);
            *(float*)&movementPlayer->unk_0x60 = value;
            if (value < -128.0f)
                *(float*)&movementPlayer->unk_0x60 = -128.0f;
        }
    }

    value = EQ_GLOBAL(float, 0x006E4E9C) - movementPlayer->heading;
    if (value < 0.0f)
        value = -value;
    if (2.0f <= value)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
            EQ_GLOBAL(char, 0x006B710F) = 1;
        else
            EQ_GLOBAL(char, 0x006B710E) = 1;
        EQ_GLOBAL(float, 0x006E4E9C) = movementPlayer->heading;
    }
    value = EQ_GLOBAL(float, 0x006E4E98) - movementPlayer->speed;
    if (value < 0.0f)
        value = -value;
    if (0.2f <= value)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
            EQ_GLOBAL(char, 0x006B710F) = 1;
        else
            EQ_GLOBAL(char, 0x006B710E) = 1;
        EQ_GLOBAL(float, 0x006E4E98) = movementPlayer->speed;
    }
    if (EQ_GLOBAL(char, 0x006B645B) != '\0')
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C) == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
            EQ_GLOBAL(char, 0x006B710F) = 1;
        else
            EQ_GLOBAL(char, 0x006B710E) = 1;
    }

    return;
}
#undef CEverQuest

// FUNCTION: TRILOGY 0x004d7ca7
// Checks whether active UI or interaction states should receive keyboard input.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name, low)
// __cdecl. 0 when the UI object is missing; else bit 0 = "keyboard input is taken" (a text input, a
// modal window or a trade/loot state is open) and bit 1 = 0x006B6404 clear, encoded as
// taken: 1 (flag clear) / 3 (flag set), free: 2 (flag clear) / 0 (flag set).
int UI_ChatInputCheck_guess(void)
{
    char* main = g_eqmain_guess;
    int taken = 0;
    int flagClear = 0;

    if (main == NULL)
        return 0;
    if (*(main + 0x8FBAD) != 0
        || (g_chatWindow_guess != NULL && *(g_chatWindow_guess + 0x1F1) != 0)
        || (*(main + 0x100C) != 0 && *(u32*)(main + 0x1020) == g_local_player_guess->spawn_id)
        || *(signed char*)(main + 0xFE0) > 6
        || *(s16*)(main + 0x8FBB4) != 0
        || *(s32*)(main + 0x5F18C) != 0)
        taken = 1;
    if (g_chatInputInactive_guess == 0)
        flagClear = 1;
    if (taken && flagClear)
        return 1;
    else if (!taken && flagClear)
        return 2;
    else if (taken && !flagClear)
        return 3;
    else
        return 0;
}

// FUNCTION: TRILOGY 0x004d7d36
// Changes the client update interval; its gameplay purpose is unclear.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name, low)
// __cdecl. Toggles the delay at 0x006E20F0 between 500 and 10 and, when the deadline at 0x006E20EC has
// passed, sets it to now + the new delay.
void ToggleAutoAttack_guess(void)
{
    g_updateInterval_guess = g_updateInterval_guess != 0 ? 10 : 500;
    if (g_cdisplay_guess->time_ms_guess > g_nextUpdateTime_guess)
        g_nextUpdateTime_guess = g_cdisplay_guess->time_ms_guess + g_updateInterval_guess;
}

// The main window object at 0x006E4E44 seen through its vtable, and its window positions (0x20 bytes each
// at +0x90044) with six per-resolution layouts per window at +0x90284.
class ExecuteCmdMain_guess
{
public:
    virtual void Slot0_guess(void);
    virtual void Activate_guess(void);
    virtual void Deactivate_guess(void);
};

typedef struct ExecuteCmdWindowPos_guess
{
    int x;
    int y;
    int unknown0x08;
    int unknown0x0C;
    int unknown0x10;
    int unknown0x14;
    int unknown0x18;
    int unknown0x1C;
} ExecuteCmdWindowPos_guess;

typedef char ExecuteCmdChatLine_guess[0x100];
typedef char ExecuteCmdHistoryLine_guess[0x800];
typedef char ExecuteCmdGroupName_guess[30];

#define EXEC_EQ                     EQ_GLOBAL(char *, 0x006EFE2C)
#define EXEC_CHAT_INPUT             (EXEC_EQ + 0xBA8)
#define ACT_TARGET                 EQ_GLOBAL(EQPlayer *, 0x006E4DAC)
#define EXEC_MAIN_WND               ((ExecuteCmdMain_guess *)g_eqmain_guess)
#define EXEC_MAIN_REDRAW            (pinstEQ_Main_guess->uiDirty_guess)
#define EXEC_MAIN_VIEW              (pinstEQ_Main_guess->screenMode_guess)
#define EXEC_WINDOW_POS(index)      ((ExecuteCmdWindowPos_guess *)(g_eqmain_guess + 0x90044) + (index))
#define EXEC_WINDOW_LAYOUT(index, layout) \
    ((ExecuteCmdWindowPos_guess *)(g_eqmain_guess + 0x90284) + (index) * 6 + (layout))
#define EXEC_SAVED_VIDEO_MODE       EQ_GLOBAL(int, 0x00590170)
#define EXEC_SCREEN_WIDTH           EQ_GLOBAL(int, 0x006B63FC)
#define EXEC_HWND                   EQ_GLOBAL(HWND, 0x006B63D8)
#define EXEC_COLUMNS                EQ_GLOBAL(char *, 0x006E4DF8)
#define EXEC_COMMAND_KEY            EQ_ARRAY(int, 0x006E11E4)
#define EXEC_COMMAND_STATE          EQ_ARRAY(int, 0x006E19E4)
#define EXEC_AUTORUN                EQ_GLOBAL(int, 0x006B6474)
#define EXEC_CHAT_LINES             EQ_ARRAY(ExecuteCmdChatLine_guess, 0x006B772B)
#define g_current_display_guess            EQ_GLOBAL(int, 0x006D6FDC)
#define EXEC_HISTORY                EQ_ARRAY(ExecuteCmdHistoryLine_guess, 0x006D6FE8)
#define EXEC_HISTORY_INDEX          EQ_GLOBAL(s16, 0x006E0FE8)
#define EXEC_GROUP_NAMES            EQ_ARRAY(ExecuteCmdGroupName_guess, 0x005FE8BD)
#define ACT_GROUP_PLAYERS          EQ_ARRAY(EQPlayer *, 0x005FE954)
#define EXEC_CAMERA_ZOOM(view)      (*(float *)(EQ_ARRAY(char, 0x006B6F28) + (view) * 0x1C))
#define EXEC_SENSITIVITY            EQ_GLOBAL(int, 0x006B68B8)
#define EXEC_SENSITIVITY_SCALE      EQ_GLOBAL(float, 0x006B6EF0)
#define EXEC_CHAT_CHECK()           EQ_METHOD(0, int, 0x004D7CA7, (void *), EXEC_EQ)()
#define EXEC_SET_CHAT_CURSOR(pos)   EQ_METHOD(1, void, 0x004CF7A1, (void *, int), EXEC_EQ)(pos)
#define EXEC_PRINT_CHAT(text)       EQ_METHOD(1, void, 0x004D05B7, (void *, const char *), EXEC_EQ)(text)
#define EXEC_DSP_CHAT(text, color)  EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), EXEC_EQ)(text, color)
#define EXEC_SAVE_OPTIONS()         EQ_METHOD(0, void, 0x004CF31A, (void *), EXEC_EQ)()
#define EXEC_SAVE_OPTIONS2()        EQ_METHOD(0, void, 0x004CF3E1, (void *), EXEC_EQ)()
#define EXEC_CHANGE_STANCE(player, stance) \
    EQ_METHOD(1, void, 0x004B45CB, (EQPlayer *, int), player)(stance)
#define EXEC_CHANGE_VIDEO_MODE()    EQ_METHOD(1, void, 0x0040E58B, (void *, int), g_displayManager_guess)(0)
#define EXEC_COLUMNS_PAUSED()       EQ_METHOD(0, int, 0x0043257C, (void *), EXEC_COLUMNS)()
#define EXEC_SET_COLUMN_STATE(state) EQ_METHOD(1, void, 0x00433330, (void *, int), EXEC_COLUMNS)(state)
#define EXEC_STRNICMP               _strnicmp
#define EXEC_STRCMP                 strcmp
#define EXEC_CYCLE_TELLS            EQ_CDECL(void, 0x004A8235, (int))

#define EXEC_TARGET_GROUP_MEMBER(member)                                                    \
    if (canAct == 0)                                                                        \
        break;                                                                              \
    if (pressed == 0)                                                                       \
        break;                                                                              \
    if (*(int *)(g_eqmain_guess + 0xE5C) != 0)                                              \
        break;                                                                              \
    if (*(int *)(g_eqmain_guess + 0x94) != 0)                                               \
        break;                                                                              \
    if (EXEC_GROUP_NAMES[member] == NULL)                                                   \
        break;                                                                              \
    *(char *)(g_eqmain_guess + 0xB4) = member;                                              \
    ACT_TARGET = ACT_GROUP_PLAYERS[member];                                               \
    EXEC_MAIN_REDRAW = 1;                                                                   \
    break

#define EXEC_SET_VIEW(newView)                                                              \
    if (canAct == 0)                                                                        \
        break;                                                                              \
    if (pressed == 0)                                                                       \
        break;                                                                              \
    if (EXEC_MAIN_VIEW == newView)                                                          \
        break;                                                                              \
    EXEC_MAIN_WND->Deactivate_guess();                                                      \
    EXEC_MAIN_VIEW = newView;                                                               \
    *(s16 *)(g_eqmain_guess + 0x5F186) = -1;                                                \
    *(s16 *)(g_eqmain_guess + 0x5F188) = -1;                                                \
    *(s16 *)(g_eqmain_guess + 0x5F18A) = -1;                                                \
    EXEC_MAIN_WND->Activate_guess();                                                        \
    break

#define EXEC_UPDATE_CHAT_SCROLLBAR()                                                        \
    EQ_METHOD(0, void, 0x0048CA78, (void *), g_chatWindow_guess)();                               \
    lines = EQ_GLOBAL(s16, 0x006E0FEA) - 1;                                                 \
    offset = lines == 0 ? 0 : *(s16 *)(g_chatWindow_guess + 0x17A) * g_current_display_guess / lines;   \
    (*(RECT **)(g_chatWindow_guess + 0x88))->top = *(s16 *)(g_chatWindow_guess + 0x178) - offset;       \
    (*(RECT **)(g_chatWindow_guess + 0x88))->bottom =                                             \
        *(s16 *)(g_chatWindow_guess + 0x174) + (*(RECT **)(g_chatWindow_guess + 0x88))->top;            \
    CopyRect((RECT *)(g_chatWindow_guess + 0xCC), *(RECT **)(g_chatWindow_guess + 0x88));               \
    *(int *)(g_chatWindow_guess + 0x1EC) = 1

#define EXEC_TOGGLE_WINDOW(hideFlag, keyName, index)                                        \
    if (pressed == 0)                                                                       \
        break;                                                                              \
    if (hideFlag != 0)                                                                      \
    {                                                                                       \
        hideFlag = 0;                                                                       \
        WritePrivateProfileStringA("Defaults", keyName, "FALSE", ".\\eqclient.ini");        \
        EXEC_WINDOW_POS(index)->unknown0x10 = 1;                                            \
        EXEC_WINDOW_POS(index)->unknown0x14 = 1;                                            \
        EXEC_MAIN_WND->Deactivate_guess();                                                  \
        *EXEC_WINDOW_POS(index) = *EXEC_WINDOW_LAYOUT(index, layout);                       \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        hideFlag = 1;                                                                       \
        WritePrivateProfileStringA("Defaults", keyName, "TRUE", ".\\eqclient.ini");         \
        EXEC_WINDOW_POS(index)->unknown0x10 = 1;                                            \
        EXEC_WINDOW_POS(index)->unknown0x14 = 0;                                            \
        EXEC_MAIN_WND->Deactivate_guess();                                                  \
        EXEC_WINDOW_POS(index)->unknown0x10 = 1;                                            \
        EXEC_WINDOW_POS(index)->x = 2000;                                                   \
        EXEC_WINDOW_POS(index)->y = 2000;                                                   \
    }                                                                                       \
    EXEC_MAIN_WND->Activate_guess();                                                        \
    break

// FUNCTION: TRILOGY 0x004d7d6a
// __cdecl; runs key command commandId, pressed going down (nonzero) or up. Always returns 1.
// Computes canAct from busy state (0x006e20f4, stand_state, unknown0x0076) and exits if 0x006e4e6c is set;
// when game state is 2 it picks a layout number 1-5 from the screen width.
// Then dispatches on commandId by range (below 0x29, up to 0x32, up to 0x52, up to 200, above), each range a
// switch on the command id whose cases check canAct and pressed before acting.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
int ExecuteCmd_guess(int commandId, int pressed, int unused)
{
    int canAct;
    int layout;
    char view;
    short spell;
    int gem;
    int slot;
    int x;
    int y;
    int line;
    int last;
    int lines;
    int offset;
    int scroll;
    EQPlayer* player;
    char* window;
    char* name;

    canAct = 1;
    if (EQ_GLOBAL(int, 0x006E4E6C) != 0)
        return 1;
    if (EQ_GLOBAL(char, 0x006E20F4) != 0)
        canAct = 0;
    if ((signed char)g_pc_guess->unknown0x0076 > 2 || g_local_player_guess->stand_state == 0x66)
        canAct = 0;
    if (g_offlineMode_guess != 0 || DAT_006b6494 != 0)
        canAct = 1;
    if (EQ_CDECL(int, 0x00522F75, (int))(EQ_CDECL(int, 0x004C1B72, (int))(EXEC_COMMAND_KEY[commandId] & 0xFF)) != 0
        && EXEC_CHAT_CHECK() != 0)
        return 1;

    if (g_game_state_guess == 2)
    {
        if (EXEC_SCREEN_WIDTH == 800)
            layout = 2;
        else if (EXEC_SCREEN_WIDTH == 1024)
            layout = 3;
        else if (EXEC_SCREEN_WIDTH == 1280)
            layout = 4;
        else
            layout = EXEC_SCREEN_WIDTH == 1600 ? 5 : 1;
    }
    else
        layout = 0;

    if (commandId <= 0x32)
    {
        if (commandId < 0x29)
        {
            switch (commandId)
            {
            case 9:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_CDECL(void, 0x004A6A08, (EQPlayer *, char *))(NULL, EQ_ARRAY(char, 0x0058FBA4));
                break;
            case 0x0A:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (g_control_player_guess->race == 0x8F)
                    break;
                if (g_control_player_guess->race == 0x8E)
                    break;
                if (EQ_GLOBAL(int, 0x006B646C) != 0)
                    break;
                if (g_control_player_guess->stand_state == 100)
                    EXEC_CHANGE_STANCE(g_control_player_guess, 0x6F);
                else if (g_control_player_guess->stand_state == 0x6F)
                    EXEC_CHANGE_STANCE(g_control_player_guess, 100);
                break;
            case 0x0C:
                if (canAct == 0)
                    break;
                if (g_game_state_guess == 3)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_GLOBAL(char, 0x006B648A) != 0)
                    break;
                if (EQ_GLOBAL(char, 0x006B648B) != 0)
                    break;
                if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                    break;
                if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                    break;
                view = EXEC_MAIN_VIEW;
                if (view >= 6)
                    break;
                if (view == 5)
                {
                    pinstEQ_Main_guess->memorizeMeditate_guess = 0;
                    pinstEQ_Main_guess->bookAnimStartTime_guess = 0;
                    EXEC_MAIN_WND->Deactivate_guess();
                    pinstEQ_Main_guess->cursorSpell_guess = 0;
                    if (g_pc_guess->inventory_items[0] == NULL)
                        *(g_displayManager_guess + 0x3C) = 0;
                    pinstEQ_Main_guess->returnMode_guess = 0;
                    pinstEQ_Main_guess->memorizingSpell_guess = -1;
                    pinstEQ_Main_guess->scribingSpell_guess = -1;
                    EXEC_MAIN_VIEW = 4;
                    pinstEQ_Main_guess->clickedButton_guess = -1;
                    pinstEQ_Main_guess->pressedButton_guess = -1;
                    pinstEQ_Main_guess->releasedButton_guess = -1;
                    EXEC_MAIN_REDRAW = 1;
                    EXEC_MAIN_WND->Activate_guess();
                    EXEC_CHANGE_STANCE(g_local_player_guess, 100);
                    break;
                }
                if (view == 4)
                {
                    if (EXEC_SAVED_VIDEO_MODE == 2)
                    {
                        g_game_state_guess = 2;
                        EXEC_CHANGE_VIDEO_MODE();
                        break;
                    }
                    EXEC_MAIN_WND->Deactivate_guess();
                    EXEC_MAIN_VIEW = 0;
                    EXEC_MAIN_WND->Activate_guess();
                    break;
                }
                if (g_game_state_guess == 2 && (g_pcloudFlagA_guess == 0 || EXEC_SCREEN_WIDTH == 640))
                {
                    EXEC_SAVED_VIDEO_MODE = 2;
                    g_game_state_guess = 1;
                    EXEC_CHANGE_VIDEO_MODE();
                }
                EXEC_MAIN_WND->Deactivate_guess();
                EXEC_MAIN_VIEW = 4;
                EXEC_MAIN_WND->Activate_guess();
                break;
            case 0x0D:
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                window = EQ_GLOBAL(char *, 0x006E4DF4);
                if (window != NULL && *(int *)(window + 0x10) != 0 && *(int *)(window + 0x8C) == 2)
                    sprintf(EXEC_CHAT_INPUT, "/tell %s ", window + 0x180);
                else
                    strcpy(EXEC_CHAT_INPUT, "/tell ");
                EXEC_SET_CHAT_CURSOR(1000000);
                g_chatInputInactive_guess = 0;
                break;
            case 0x0E:
                if (canAct == 0)
                    break;
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_GLOBAL(void *, 0x006E4DEC) == NULL)
                    break;
                if (EXEC_CHAT_CHECK() != 0)
                    break;
                x = (EQ_GLOBAL(s16, 0x006B63E0) + EQ_GLOBAL(s16, 0x006B63E4)) >> 1;
                y = (EQ_GLOBAL(s16, 0x006B63E2) + EQ_GLOBAL(s16, 0x006B63E6)) >> 1;
                EQ_METHOD(3, void, 0x00415EFB, (void *, int, int, int), EQ_GLOBAL(void *, 0x006E4DEC))(0, x, y);
                if (ACT_TARGET == NULL || ACT_TARGET->type == 1)
                    EQ_METHOD(3, void, 0x00415EFB, (void *, int, int, int), EQ_GLOBAL(void *, 0x006E4DEC))(1, x, y);
                break;
            case 0x0B:
                if (pressed == 0)
                    break;
                EQ_CDECL(void, 0x004A7EEE, (EQPlayer *, int))(g_local_player_guess, 0);
                break;
            case 0x16:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                    break;
                if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                    break;
                if (ACT_TARGET == g_local_player_guess)
                {
                    for (player = g_player_top_guess; player != NULL; player = player->next)
                    {
                        if (player->pet_owner_id == g_local_player_guess->spawn_id)
                            break;
                    }
                    if (player != NULL)
                        ACT_TARGET = player;
                }
                else
                    ACT_TARGET = g_local_player_guess;
                EXEC_MAIN_REDRAW = 1;
                break;
            case 0x17:
                EXEC_TARGET_GROUP_MEMBER(0);
            case 0x18:
                EXEC_TARGET_GROUP_MEMBER(1);
            case 0x19:
                EXEC_TARGET_GROUP_MEMBER(2);
            case 0x1A:
                EXEC_TARGET_GROUP_MEMBER(3);
            case 0x1B:
                EXEC_TARGET_GROUP_MEMBER(4);
            case 0x1F:
            case 0x20:
            case 0x21:
            case 0x22:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_METHOD(2, void, 0x0047E1E3, (void *, int, int), g_eqmain_guess)(commandId + 0xA9, 0);
                EXEC_MAIN_REDRAW = 1;
                break;
            case 0x1C:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                player = EQ_METHOD(2, EQPlayer *, 0x0040DBA9, (void *, float, int), g_displayManager_guess)(500.0f, 0);
                if (player == NULL)
                    break;
                ACT_TARGET = player;
                EXEC_MAIN_REDRAW = 1;
                break;
            case 0x1D:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                player = EQ_METHOD(2, EQPlayer *, 0x0040DBA9, (void *, float, int), g_displayManager_guess)(500.0f, 1);
                if (player == NULL)
                    break;
                ACT_TARGET = player;
                EXEC_MAIN_REDRAW = 1;
                break;
            case 0x1E:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_GLOBAL(char, 0x006B645C) ^= 1;
                break;
            case 8:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_GLOBAL(int, 0x006E4DB8) != 0)
                    break;
                if (EQ_GLOBAL(int, 0x006E4DC4) != 0)
                    break;
                if (EQ_GLOBAL(int, 0x006E4DBC) != 0)
                    break;
                if (g_local_player_guess->stand_state != 100 && g_attacking_guess == 0)
                {
                    EXEC_DSP_CHAT(g_local_player_guess->stand_state == 0x66
                                      ? "You do not have control of yourself right now."
                                      : "You must be standing to attack!",
                                  0x0D);
                    break;
                }
                g_attacking_guess = g_attacking_guess != 0 ? 0 : 1;
                g_rangedAttacking_guess = g_attacking_guess;
                EXEC_MAIN_REDRAW = 1;
                EXEC_DSP_CHAT(g_attacking_guess != 0 ? "Auto attack on." : "Auto attack off.", 0x0D);
                break;
            case 2:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EXEC_COMMAND_STATE[2] = pressed;
                break;
            case 1:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EXEC_AUTORUN ^= 1;
                if (EXEC_AUTORUN == 0)
                    break;
                EXEC_COMMAND_STATE[4] = 0;
                EQ_GLOBAL(s16, 0x006B7102) = 0;
                break;
            case 3:
                if (EXEC_COLUMNS != NULL && *(int *)(EXEC_COLUMNS + 0x10) != 0 && EXEC_COLUMNS_PAUSED() == 0)
                {
                    EXEC_SET_COLUMN_STATE(0);
                    break;
                }
                if (canAct == 0)
                    break;
                EXEC_COMMAND_STATE[3] = pressed;
                if (pressed == 0)
                {
                    EQ_GLOBAL(s16, 0x006B7100) = 0;
                    break;
                }
                EXEC_AUTORUN = 0;
                EXEC_COMMAND_STATE[4] = 0;
                EQ_GLOBAL(s16, 0x006B7102) = 0;
                break;
            case 4:
                if (EXEC_COLUMNS != NULL && *(int *)(EXEC_COLUMNS + 0x10) != 0 && EXEC_COLUMNS_PAUSED() == 0)
                {
                    EXEC_SET_COLUMN_STATE(pressed != 0 ? 1000 : 0);
                    break;
                }
                if (canAct == 0)
                    break;
                EXEC_COMMAND_STATE[4] = pressed;
                if (pressed == 0)
                {
                    EQ_GLOBAL(s16, 0x006B7102) = 0;
                    break;
                }
                EXEC_AUTORUN = 0;
                EXEC_COMMAND_STATE[3] = 0;
                EQ_GLOBAL(s16, 0x006B7100) = 0;
                break;
            case 5:
                if (EXEC_COLUMNS != NULL && *(int *)(EXEC_COLUMNS + 0x10) != 0 && EXEC_COLUMNS_PAUSED() == 0)
                {
                    if (pressed == 0)
                        break;
                    EXEC_SET_COLUMN_STATE(1);
                    break;
                }
                if (canAct == 0)
                    break;
                EXEC_COMMAND_STATE[5] = pressed;
                if (pressed == 0)
                {
                    EQ_GLOBAL(s16, 0x006B7106) = 0;
                    break;
                }
                EXEC_COMMAND_STATE[6] = 0;
                EQ_GLOBAL(s16, 0x006B7104) = 0;
                break;
            case 6:
                if (EXEC_COLUMNS != NULL && *(int *)(EXEC_COLUMNS + 0x10) != 0 && EXEC_COLUMNS_PAUSED() == 0)
                {
                    if (pressed == 0)
                        break;
                    EXEC_SET_COLUMN_STATE(-1);
                    break;
                }
                if (canAct == 0)
                    break;
                EXEC_COMMAND_STATE[6] = pressed;
                if (pressed == 0)
                {
                    EQ_GLOBAL(s16, 0x006B7104) = 0;
                    break;
                }
                EXEC_COMMAND_STATE[5] = 0;
                EQ_GLOBAL(s16, 0x006B7106) = 0;
                break;
            case 0x12:
                EXEC_COMMAND_STATE[0x12] = pressed;
                break;
            case 0x13:
                EXEC_COMMAND_STATE[0x13] = pressed;
                break;
            case 0x0F:
                EXEC_COMMAND_STATE[0x0F] = pressed;
                if (g_eqmain_guess != NULL && EXEC_MAIN_VIEW == 0x0C)
                {
                    if (pressed == 0)
                        break;
                    EXEC_MAIN_REDRAW = 1;
                    pinstEQ_Main_guess->helpTopLine_guess -= 0x12;
                    if (pinstEQ_Main_guess->helpTopLine_guess < 0)
                        pinstEQ_Main_guess->helpTopLine_guess = 0;
                    break;
                }
                if (pressed != 0)
                {
                    EXEC_COMMAND_STATE[0x10] = 0;
                    EQ_GLOBAL(s16, 0x006B70FE) = 0;
                    break;
                }
                EQ_GLOBAL(s16, 0x006B70FC) = 0;
                break;
            case 0x10:
                EXEC_COMMAND_STATE[0x10] = pressed;
                if (g_eqmain_guess != NULL && EXEC_MAIN_VIEW == 0x0C)
                {
                    if (pressed == 0)
                        break;
                    EXEC_MAIN_REDRAW = 1;
                    pinstEQ_Main_guess->helpTopLine_guess += 0x12;
                    if (pinstEQ_Main_guess->helpTopLine_guess > pinstEQ_Main_guess->helpLineCount_guess)
                        pinstEQ_Main_guess->helpTopLine_guess = pinstEQ_Main_guess->helpLineCount_guess;
                    break;
                }
                if (pressed != 0)
                {
                    EXEC_COMMAND_STATE[0x0F] = 0;
                    EQ_GLOBAL(s16, 0x006B70FC) = 0;
                    break;
                }
                EQ_GLOBAL(s16, 0x006B70FE) = 0;
                break;
            case 0x14:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EXEC_COMMAND_STATE[0x14] = pressed;
                EQ_METHOD(0, void, 0x0040F47F, (void *), g_displayManager_guess)();
                break;
            case 0x11:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EXEC_COMMAND_STATE[0x11] = pressed;
                if (g_activeView_guess != 0)
                {
                    if (g_activeView_guess == 1)
                        EXEC_CAMERA_ZOOM(1) = -100.0f;
                    else
                        EXEC_CAMERA_ZOOM(g_activeView_guess) = 0.001f;
                }
                else
                {
                    player = g_control_player_guess;
                    if (player->race == 0x8D)
                        player = g_local_player_guess;
                    *(float *)&player->unk_0x60 = 0.0f;
                }
                break;
            case 7:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_GLOBAL(char, 0x006B6409) ^= 1;
                EXEC_COMMAND_STATE[7] = pressed;
                EQ_GLOBAL(int, 0x006B68A8) = (u8)EQ_GLOBAL(char, 0x006B6409);
                EXEC_MAIN_REDRAW = 1;
                if (EQ_GLOBAL(char, 0x006B6409) == 0)
                    EXEC_PRINT_CHAT("Mouse control deactivated.");
                else
                    EXEC_PRINT_CHAT("Mouse control activated.  Use right mouse button.");
                break;
            case 0x15:
                if (pressed == 0)
                    break;
                EXEC_COMMAND_STATE[0x15] = pressed;
                if (g_eqmain_guess == NULL)
                    break;
                if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                    break;
                if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                    break;
                if (EXEC_MAIN_VIEW == 8)
                    break;
                if (pinstEQ_Main_guess->optionsPage_guess != 0)
                    EXEC_SAVE_OPTIONS();
                pinstEQ_Main_guess->optionsPage_guess = 0;
                if (g_game_state_guess == 3)
                    g_game_state_guess = 1;
                else
                    g_game_state_guess++;
                if (g_game_state_guess != 3)
                    EXEC_SAVED_VIDEO_MODE = g_game_state_guess;
                EXEC_MAIN_REDRAW = 1;
                EXEC_CHANGE_VIDEO_MODE();
                break;
            default:
                break;
            }
        }
        else if (canAct != 0 && pressed != 0)
        {
            pinstEQ_Main_guess->hotPage_guess = (char)(commandId - 0x29);
            EXEC_MAIN_REDRAW = 1;
        }
    }
    else if (commandId <= 0x52)
    {
        if (commandId != 0x52)
        {
            switch (commandId)
            {
            case 0x3F:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() == 2)
                    break;
                if (ACT_TARGET == g_local_player_guess)
                {
                    player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, EQ_GLOBAL(int, 0x006B7124));
                    if (player == NULL)
                        break;
                    ACT_TARGET = player;
                }
                else
                {
                    if (ACT_TARGET != NULL)
                        EQ_GLOBAL(u32, 0x006B7124) = ACT_TARGET->spawn_id;
                    ACT_TARGET = g_local_player_guess;
                }
                if (g_eqmain_guess == NULL)
                    break;
                EXEC_MAIN_REDRAW = 1;
                break;
            case 0x3C:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                if (EXEC_STRNICMP(EXEC_CHAT_INPUT, "/tell", 5) != 0)
                    break;
                EXEC_CYCLE_TELLS(0);
                name = g_tell_list_guess[g_tell_index_guess];
                if (*name == 0)
                {
                    EXEC_PRINT_CHAT("You have not received any tells, so you cannot reply.");
                    break;
                }
                sprintf(EXEC_CHAT_INPUT, "/tell %s ", name);
                EXEC_SET_CHAT_CURSOR(1000000);
                break;
            case 0x3D:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                if (EXEC_STRNICMP(EXEC_CHAT_INPUT, "/tell", 5) != 0)
                    break;
                EXEC_CYCLE_TELLS(1);
                name = g_tell_list_guess[g_tell_index_guess];
                if (*name == 0)
                {
                    EXEC_PRINT_CHAT("You have not received any tells, so you cannot reply.");
                    break;
                }
                sprintf(EXEC_CHAT_INPUT, "/tell %s ", name);
                EXEC_SET_CHAT_CURSOR(1000000);
                break;
            case 0x3E:
                if (canAct == 0)
                    break;
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_GLOBAL(int, 0x006B65A0) != 0)
                {
                    EQ_GLOBAL(int, 0x006B65A0) = 0;
                    EXEC_DSP_CHAT("Fullscreen Transparent Polygons are now ON", 0x0D);
                }
                else
                {
                    EQ_GLOBAL(int, 0x006B65A0) = 1;
                    EXEC_DSP_CHAT("Fullscreen Transparent Polygons are now OFF", 0x0D);
                }
                break;
            case 0x3B:
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                name = g_tell_list_guess[g_tell_index_guess];
                if (*name == 0)
                {
                    EXEC_PRINT_CHAT("You have not received any tells, so you cannot reply.");
                    break;
                }
                sprintf(EXEC_CHAT_INPUT, "/tell %s ", name);
                EXEC_SET_CHAT_CURSOR(1000000);
                g_chatInputInactive_guess = 0;
                break;
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3A:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                gem = commandId - 0x33;
                if (((int *)(g_eqmain_guess + 0xE60))[gem] == 0 && g_local_player_guess->actor_info->unknown0x248 == -1)
                {
                    spell = g_pc_guess->spell_memorized[gem];
                    if (spell == -1)
                        break;
                    if (pinstEQ_Main_guess->cursorSpell_guess != 0)
                        break;
                    if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
                        break;
                    if (pinstEQ_Main_guess->pendingServerReplies_guess != 0)
                        break;
                    if (EQ_METHOD(4, char, 0x0041F381, (CharacterProfile *, int, short, int *, int), g_pc_guess)(
                            gem, spell, NULL, -1) == 0)
                        break;
                    for (slot = 0; slot < 8; slot++)
                    {
                        if (((int *)(g_eqmain_guess + 0xE60))[slot] == 0)
                            ((int *)(g_eqmain_guess + 0xE60))[slot] = 1;
                        ((int *)(g_eqmain_guess + 0xE80))[slot] = 0;
                    }
                    break;
                }
                if (EQ_METHOD(0, int, 0x0046A72F, (void *), g_eqmain_guess)() != 0)
                {
                    EQ_METHOD(1, void, 0x0042798F, (CharacterProfile *, int), g_pc_guess)(0);
                    break;
                }
                EXEC_DSP_CHAT("You haven't recovered yet...", 0x108);
                break;
            case 0x40:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_CDECL(int, 0x004A49BD, (void))() == 0)
                    break;
                if (EQ_METHOD(0, int, 0x0041980E, (void *), g_pc_guess)() == 0)
                    break;
                view = EXEC_MAIN_VIEW;
                if (view == 5)
                    break;
                pinstEQ_Main_guess->returnMode_guess = view;
                if (g_game_state_guess == 2)
                {
                    EXEC_SAVED_VIDEO_MODE = 2;
                    g_game_state_guess = 1;
                    EXEC_CHANGE_VIDEO_MODE();
                }
                else if (EXEC_MAIN_VIEW != 4)
                {
                    pinstEQ_Main_guess->bookAnimStartTime_guess = *(int *)(g_displayManager_guess + 0xBC);
                    break;
                }
                EQ_METHOD(0, void, 0x00465A60, (void *), g_eqmain_guess)();
                break;
            case 0x41:
                EXEC_SET_VIEW(1);
            case 0x42:
                EXEC_SET_VIEW(2);
            case 0x43:
                EXEC_SET_VIEW(3);
            case 0x44:
                EXEC_SET_VIEW(0);
            case 0x4A:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_GLOBAL(char, 0x006B6405) ^= 1;
                break;
            case 0x4D:
                if (EQ_GLOBAL(int, 0x006B6EF4) == 0)
                    break;
                PostMessageA(EXEC_HWND, 0x4C8, 0, 0);
                break;
            case 0x45:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_METHOD(1, void, 0x004CA5C7, (void *, char *), EXEC_EQ)(EQ_ARRAY(char, 0x0058FBA4));
                break;
            case 0x46:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (pinstEQ_Main_guess->groupInvitePending_guess != 0)
                    EQ_METHOD(0, void, 0x004CB362, (void *), EXEC_EQ)();
                else
                    EQ_METHOD(0, void, 0x004CAF5E, (void *), EXEC_EQ)();
                break;
            case 0x47:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_METHOD(0, void, 0x004CB4FA, (void *), EXEC_EQ)();
                break;
            case 0x48:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                EQ_METHOD(0, void, 0x004CB72D, (void *), EXEC_EQ)();
                break;
            case 0x49:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_CDECL(int, 0x004A49BD, (void))() == 0)
                    break;
                EQ_METHOD(0, void, 0x004CB6E6, (void *), EXEC_EQ)();
                break;
            case 0x4B:
                if (pressed == 0)
                    break;
                if (g_chatInputInactive_guess == 0)
                    break;
                if (EXEC_SENSITIVITY > 0)
                    EXEC_SENSITIVITY--;
                EXEC_SENSITIVITY_SCALE = (float)EXEC_SENSITIVITY * 0.2f;
                if (EXEC_SENSITIVITY_SCALE < 0.1f)
                    EXEC_SENSITIVITY_SCALE = 0.1f;
                EXEC_SAVE_OPTIONS();
                EXEC_SAVE_OPTIONS2();
                break;
            case 0x4C:
                if (pressed == 0)
                    break;
                if (g_chatInputInactive_guess == 0)
                    break;
                if (EXEC_SENSITIVITY < 20)
                    EXEC_SENSITIVITY++;
                EXEC_SENSITIVITY_SCALE = (float)EXEC_SENSITIVITY * 0.2f;
                if (EXEC_SENSITIVITY_SCALE > 4.0f)
                    EXEC_SENSITIVITY_SCALE = 4.0f;
                EXEC_SAVE_OPTIONS();
                EXEC_SAVE_OPTIONS2();
                break;
            case 0x4F:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 0 && EXEC_CHAT_CHECK() != 2)
                    break;
                if (g_chatInputInactive_guess != 0)
                {
                    g_chatInputInactive_guess = 0;
                    *(EXEC_EQ + 0x13EC) = 1;
                    EXEC_CHAT_INPUT[0] = '/';
                    EXEC_CHAT_INPUT[1] = 0;
                }
                if (EXEC_HISTORY_INDEX < 0x13 && EXEC_HISTORY[EXEC_HISTORY_INDEX + 1][0] != 0)
                {
                    EXEC_HISTORY_INDEX++;
                    strcpy(EXEC_CHAT_INPUT, EXEC_HISTORY[EXEC_HISTORY_INDEX]);
                }
                EXEC_SET_CHAT_CURSOR(1000000);
                break;
            case 0x50:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 0 && EXEC_CHAT_CHECK() != 2)
                    break;
                if (g_chatInputInactive_guess != 0)
                {
                    g_chatInputInactive_guess = 0;
                    *(EXEC_EQ + 0x13EC) = 1;
                    EXEC_CHAT_INPUT[0] = '/';
                    EXEC_CHAT_INPUT[1] = 0;
                }
                if (EXEC_HISTORY_INDEX > 0)
                {
                    EXEC_HISTORY_INDEX--;
                    strcpy(EXEC_CHAT_INPUT, EXEC_HISTORY[EXEC_HISTORY_INDEX]);
                }
                else
                    EXEC_CHAT_INPUT[0] = 0;
                EXEC_SET_CHAT_CURSOR(1000000);
                break;
            case 0x51:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                if (g_chatWindow_guess == NULL)
                    break;
                EXEC_SET_CHAT_CURSOR(pinstEQ_TextIO_guess->inputCursor - 1);
                EQ_CDECL(void, 0x004D7D36, (void))();
                break;
            case 0x4E:
                if (canAct == 0)
                    break;
                if (pressed == 0)
                    break;
                if (EQ_METHOD(0, int, 0x004197BF, (void *), g_pc_guess)() != 0 && g_local_player_guess->gm == 0)
                    break;
                EQ_GLOBAL(char, 0x006B6854) = 1;
                EXEC_DSP_CHAT("Taking a screenshot...", 0x0F);
                break;
            default:
                break;
            }
        }
        else if (pressed != 0 && EXEC_CHAT_CHECK() == 2 && g_chatWindow_guess != NULL)
        {
            EXEC_SET_CHAT_CURSOR(pinstEQ_TextIO_guess->inputCursor + 1);
            EQ_CDECL(void, 0x004D7D36, (void))();
        }
    }
    else if (commandId <= 200)
    {
        if (commandId != 200)
        {
            switch (commandId)
            {
            case 0x57:
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                strcpy(EXEC_CHAT_INPUT, EQ_ARRAY(char, 0x005618C0));
                EXEC_SET_CHAT_CURSOR(1000000);
                g_chatInputInactive_guess = 0;
                break;
            case 0x58:
                if (g_chatInputInactive_guess == 0)
                    break;
                if (pressed == 0)
                    break;
                strcpy(EXEC_CHAT_INPUT, EQ_ARRAY(char, 0x00559640));
                EXEC_SET_CHAT_CURSOR(1000000);
                g_chatInputInactive_guess = 0;
                break;
            case 0x68:
            case 0x69:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                EXEC_CHAT_INPUT[0] = 0;
                EXEC_SET_CHAT_CURSOR(0);
                PostMessageA(EXEC_HWND, 0x4C9, 0, 0);
                break;
            case 0x55:
                if (pressed == 0)
                    break;
                last = (g_game_state_guess == 2 ? EQ_GLOBAL(int, 0x0055D674) : EQ_GLOBAL(int, 0x006B6508))
                    + g_current_display_guess - 2;
                for (line = g_current_display_guess; line <= last; line++)
                {
                    if ((int)EXEC_CHAT_LINES[line] < (int)EQ_ARRAY(char, 0x006D6A2B)
                        && EXEC_STRCMP(EXEC_CHAT_LINES[line], EQ_ARRAY(char, 0x0055D6D4)) != 0)
                        g_current_display_guess = line;
                }
                EXEC_UPDATE_CHAT_SCROLLBAR();
                break;
            case 0x56:
                if (pressed == 0)
                    break;
                scroll = g_current_display_guess - (g_game_state_guess == 2 ? EQ_GLOBAL(int, 0x0055D674) : EQ_GLOBAL(int, 0x006B6508)) + 2;
                g_current_display_guess = 0;
                if (scroll >= 0)
                    g_current_display_guess = scroll;
                EXEC_UPDATE_CHAT_SCROLLBAR();
                break;
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
                EXEC_COMMAND_STATE[commandId] = pressed;
                break;
            case 0x54:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                EXEC_SET_CHAT_CURSOR(1000000);
                break;
            case 0x53:
                if (pressed == 0)
                    break;
                if (EXEC_CHAT_CHECK() != 2)
                    break;
                EXEC_SET_CHAT_CURSOR(0);
                break;
            case 0x59:
                if (pressed == 0)
                    break;
                if (EQ_GLOBAL(int, 0x006E2094) != 0)
                {
                    EQ_GLOBAL(int, 0x006E2094) = 0;
                    WritePrivateProfileStringA("Defaults", "LockWindows", "FALSE", ".\\eqclient.ini");
                    EXEC_PRINT_CHAT("Fullview Windows are unlocked.");
                }
                else
                {
                    EQ_GLOBAL(int, 0x006E2094) = 1;
                    WritePrivateProfileStringA("Defaults", "LockWindows", "TRUE", ".\\eqclient.ini");
                    EXEC_PRINT_CHAT("Fullview Windows are locked.");
                }
                break;
            case 0x5A:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E2098), "HidePlayerWin", 0);
            case 0x5B:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E209C), "HidePartyWin", 1);
            case 0x5C:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20A0), "HideTargetWin", 2);
            case 0x5D:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20A4), "HideSpellsWin", 3);
            case 0x5E:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20A8), "HideBuffWin", 9);
            case 0x5F:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20AC), "HideHotboxWin", 4);
            case 0x60:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20B0), "HideChatWin", 5);
            case 0x61:
                EXEC_TOGGLE_WINDOW(EQ_GLOBAL(int, 0x006E20B4), "HideMainMenuWin", 16);
            default:
                break;
            }
        }
        else if (g_chatInputInactive_guess != 0 && pressed != 0)
        {
            strcpy(EXEC_CHAT_INPUT, EQ_ARRAY(char, 0x00570224));
            EXEC_SET_CHAT_CURSOR(1000000);
            g_chatInputInactive_guess = 0;
        }
    }
    else
    {
        switch (commandId)
        {
        case 0xD3:
            if (g_chatInputInactive_guess != 0)
                break;
            if (ACT_TARGET == NULL)
                break;
            sprintf(EXEC_CHAT_INPUT, "%s %s", EXEC_CHAT_INPUT, ACT_TARGET->name);
            EXEC_SET_CHAT_CURSOR(1000000);
            PostMessageA(EXEC_HWND, 0x4CA, 0, 0);
            break;
        case 0xCF:
            if (pressed == 0)
                break;
            if (EXEC_CHAT_CHECK() != 0 && EXEC_CHAT_CHECK() != 2)
                break;
            if (g_chatInputInactive_guess == 0)
                EQ_METHOD(2, void, 0x004DB9B5, (void *, EQPlayer *, char *), EXEC_EQ)(g_local_player_guess, EXEC_CHAT_INPUT);
            g_chatInputInactive_guess ^= 1;
            EXEC_SET_CHAT_CURSOR(0);
            PostMessageA(EXEC_HWND, 0x4C9, 0, 0);
            break;
        case 0xCE:
            EXEC_COMMAND_STATE[commandId] = pressed;
            break;
        case 0xCB:
            EQ_GLOBAL(char, 0x006B7111) = 1;
            break;
        case 0xC9:
            if (pressed == 0)
                break;
            if (canAct == 0)
                break;
            ACT_TARGET = NULL;
            EQ_GLOBAL(int, 0x006E4DB4) = 0;
            EQ_GLOBAL(int, 0x006E4DB0) = 0;
            EXEC_PRINT_CHAT("You no longer have a target.");
            if (g_eqmain_guess != NULL)
                EQ_METHOD(0, void, 0x0047D796, (void *), g_eqmain_guess)();
            break;
        default:
            break;
        }
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004d9cac
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// No declaration exists on CEverQuest in the header, so keep the original thiscall ABI in a free function.
/* fake match: the PlayerDistance call passes the incoming EDX (unusedEdx), which the original never overwrites before it, instead of the macro's 0 (saves an xor) */
void __fastcall CEverQuest_doInspect_guess(CEverQuest* self, int unusedEdx, EQPlayer* rightPick)
{
    char chatLine[256];
    u32 inspectRequest[2];

    if (rightPick != 0 && rightPick->type == 0)
    {
        if (EQ_METHOD(3, float, 0x00412AA0, (void*, EQPlayer*, EQPlayer*, float), g_displayManager_guess)(rightPick, g_control_player_guess, 1.0f)
            < EQ_GLOBAL(float, 0x00534498))
        {
            inspectRequest[0] = rightPick->spawn_id;
            inspectRequest[1] = g_local_player_guess->spawn_id;
            send_message_guess(
                g_world_connection_guess, 0x20B5, inspectRequest, 8, 1);
        }
        else
        {
            sprintf(
                chatLine, EQ_ARRAY(char, 0x0055FC9C), rightPick->name);
            EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), self)(chatLine);
        }
    }
}

// FUNCTION: TRILOGY 0x004d9d4d
// Sends the local character's light level to the world server as an appearance update.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::SendLightInfo, high)
// __thiscall (`ret 8`), `this` and the first argument unused. Sends the light level as spawn appearance type
// 5 of the local player, unless the flags at 0x006B63F8 (byte) or 0x006B6494 are set.
void CEverQuest::SendLightInfo(EQPlayer* unused, u8 light)
{
    MsgSpawnAppearance msg;

    msg.type = 5;
    msg.spawn_id = g_local_player_guess->spawn_id;
    msg.parameter = light;
    if (g_offlineMode_guess == 0 && DAT_006b6494 == 0)
        send_message_guess(g_world_connection_guess, 0x20F5, &msg, sizeof(msg), 1);
}

class CEverQuest_SetTimedFog_standin_guess
{
public:
    void SetTimedFog(int unused);
};

// FUNCTION: TRILOGY 0x004d9d9e
// Sets scene fog color and visibility distance for the current zone or environment.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: similarity 0.3025; remaining differences are register allocation, stack slots, and branch/call layout.
// __thiscall (`ret 4`), the argument is unused. Environments 5..10 set the fog from the timed colour at this+0x223F8
// (red, green, blue, each clamped to 1..254) with fixed clips (5: 0..100, 6..8: 0..50, 9: 90..100, 10: 0..400). Other
// environments use the zone's fog colour and clips; zone fog modes 1, 2 and 5 use the timed colour and the clips at
// CDisplay +0xA0/+0xA4 when +0xC0 is set (else the zone's min/max clip), raised to at least 30 (start) and 105 (end).
void CEverQuest_SetTimedFog_standin_guess::SetTimedFog(int unused)
{
    char *self;
    u8 environment;
    short timedRed;
    short timedGreen;
    short timedBlue;
    u16 zoneRed;
    u16 zoneGreen;
    u16 zoneBlue;
    u8 zoneFogMode;
    float fogStart;
    float fogEnd;

    self = (char *)this;
    environment = EQ_GLOBAL(u8, 0x0058FBB8);
    if (environment == 5)
    {
        timedRed = *(short *)(self + 0x223F8);
        timedGreen = *(short *)(self + 0x223FA);
        timedBlue = *(short *)(self + 0x223FC);
        if (timedRed < 1)
            timedRed = 1;
        if (timedRed > 0xfe)
            timedRed = 0xfe;
        if (timedGreen < 1)
            timedGreen = 1;
        if (timedGreen > 0xfe)
            timedGreen = 0xfe;
        if (timedBlue < 1)
            timedBlue = 1;
        if (timedBlue > 0xfe)
            timedBlue = 0xfe;
        fogEnd = 100.0f;
        fogStart = 0.0f;
    }
    else if (environment == 6 || environment == 7 || environment == 8)
    {
        timedRed = *(short *)(self + 0x223F8);
        if (timedRed < 1)
            timedRed = 1;
        if (timedRed > 0xfe)
            timedRed = 0xfe;
        timedGreen = *(short *)(self + 0x223FA);
        if (timedGreen < 1)
            timedGreen = 1;
        if (timedGreen > 0xfe)
            timedGreen = 0xfe;
        timedBlue = *(short *)(self + 0x223FC);
        if (timedBlue < 1)
            timedBlue = 1;
        if (timedBlue > 0xfe)
            timedBlue = 0xfe;
        fogEnd = 50.0f;
        fogStart = 0.0f;
    }
    else if (environment == 9)
    {
        timedRed = *(short *)(self + 0x223F8);
        if (timedRed < 1)
            timedRed = 1;
        if (timedRed > 0xfe)
            timedRed = 0xfe;
        timedGreen = *(short *)(self + 0x223FA);
        if (timedGreen < 1)
            timedGreen = 1;
        if (timedGreen > 0xfe)
            timedGreen = 0xfe;
        timedBlue = *(short *)(self + 0x223FC);
        if (timedBlue < 1)
            timedBlue = 1;
        if (timedBlue > 0xfe)
            timedBlue = 0xfe;
        fogEnd = 100.0f;
        fogStart = 90.0f;
    }
    else
    {
        if (environment != 10)
        {
            fogEnd = EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_max_clip[0];
            zoneFogMode = EQ_GLOBAL(MsgNewZone, 0x006B65DC).unknown0x114[4];
            if (zoneFogMode == 2 || zoneFogMode == 1 || zoneFogMode == 5)
            {
                zoneRed = *(u16 *)(self + 0x223F8);
                zoneGreen = *(u16 *)(self + 0x223FA);
                zoneBlue = *(u16 *)(self + 0x223FC);
                if (g_cdisplay_guess->weather_active_guess == 0)
                    fogStart = EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_min_clip[0];
                else
                {
                    fogStart = g_cdisplay_guess->fogStart_guess;
                    fogEnd = g_cdisplay_guess->fogEnd_guess;
                }
                if (fogStart < 30.0f)
                    fogStart = 30.0f;
                if (fogEnd < 105.0f)
                    fogEnd = 105.0f;
            }
            else
            {
                zoneRed = (u16)EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_red[0];
                zoneGreen = (u16)EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_green[0];
                zoneBlue = (u16)EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_blue[0];
                fogStart = EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_min_clip[0];
            }
            if ((short)zoneRed < 1)
                zoneRed = 1;
            if ((short)zoneRed > 0xfe)
                zoneRed = 0xfe;
            if ((short)zoneGreen < 1)
                zoneGreen = 1;
            if ((short)zoneGreen > 0xfe)
                zoneGreen = 0xfe;
            if ((short)zoneBlue < 1)
                zoneBlue = 1;
            if ((short)zoneBlue > 0xfe)
                zoneBlue = 0xfe;
            EQ_METHOD(6, void, 0x0040F518, (void *, int, float, float, short, short, short), g_display_guess)(1, fogStart, fogEnd, zoneRed, zoneGreen, zoneBlue);
            return;
        }
        timedRed = *(short *)(self + 0x223F8);
        if (timedRed < 1)
            timedRed = 1;
        if (timedRed > 0xfe)
            timedRed = 0xfe;
        timedGreen = *(short *)(self + 0x223FA);
        if (timedGreen < 1)
            timedGreen = 1;
        if (timedGreen > 0xfe)
            timedGreen = 0xfe;
        timedBlue = *(short *)(self + 0x223FC);
        if (timedBlue < 1)
            timedBlue = 1;
        if (timedBlue > 0xfe)
            timedBlue = 0xfe;
        fogEnd = 400.0f;
        fogStart = 0.0f;
    }
    EQ_METHOD(6, void, 0x0040F518, (void *, int, float, float, short, short, short), g_display_guess)(1, fogStart, fogEnd, timedRed, timedGreen, timedBlue);
    return;
}

class CEverQuest_ProcessGame_standin_guess
{
public:
    void ProcessGame(HWND window, int gameMode);
};

struct ProcessGameScope_guess
{
    volatile int value;
    ~ProcessGameScope_guess()
    {
        value = 0;
    }
};

class EQ_ProcessGameWidget_guess
{
public:
    virtual void Slot0_guess(void) {}
    virtual void Slot1_guess(void) {}
    virtual void Finish_guess(void) {}
};

#define PROCESS_GAME_STATE_WAITING 3
#define PROCESS_GAME_STATE_RETRY 5
#define PROCESS_GAME_STATE_ABORTED 0xfd
#define PROCESS_GAME_STATE_DONE 0xff
#define PROCESS_GAME_REASON_DATA 0xcb
#define PROCESS_GAME_REASON_ZONE 0xcc
#define PROCESS_GAME_PROFILE_MESSAGE_DWORDS 23

#define CEverQuest CEverQuest_ProcessGame_standin_guess
// FUNCTION: TRILOGY 0x004da043
// Game session loop: world connect (or solo player), wait for zone connect, then the main loop for each zone
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest::ProcessGame(HWND window, int gameMode)
{
    ProcessGameScope_guess scope;
    CEverQuest *self;
    CharacterProfile *profile;
    CharacterProfile *oldProfile;
    void *allocation;
    void *connection;
    u32 guildChecksum;
    int disconnectInfo[3];
    char playerName[32];
    char profileName[32];
    u32 profileMessage[PROCESS_GAME_PROFILE_MESSAGE_DWORDS];
    char firstDisconnectReason[128];
    char secondDisconnectReason[124];
    u32 startTime;
    u32 currentTime;
    u32 timeout;
    size_t nameLength;
    int result;
    int index;
    u8 screenMode;
    u8 initialScreenMode;

    self = (CEverQuest *)this;
    EQ_GLOBAL(int, 0x006B63DC) = gameMode;
    *((int *)self + 1) = 0;
    EQ_GLOBAL(int, 0x006B63F4) = 0;
    EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x00570548);
    if (*((int *)self + 1) == PROCESS_GAME_STATE_DONE)
        return;

    initialScreenMode = 1;
    do
    {
        EQ_CDECL(unsigned int, 0x005227B1, (unsigned int, unsigned int))(0x00a031f, 0x000f031f);
        oldProfile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
        EQ_GLOBAL(int, 0x006EFE24) = EQ_GLOBAL(int, 0x006EFE24) + 1;
        if (EQ_GLOBAL(char, 0x006B63F8) == 0)
        {
            if (DAT_006b6494 != 0)
                goto makeTutorialProfile;

            result = EXEC_STRCMP
                ((const char *)0x00604904, (const char *)0x0058fba4);
            if (result == 0)
            {
                connection = EQ_GLOBAL(void *, 0x006B3194);
                if (connection == 0)
                    result = 0;
                else
                    result = EQ_CDECL(int, 0x004F23B0, (void *))(connection);
                if (result == 0)
                {
                    EQ_GLOBAL(int, 0x006B63C0) = 1;
                    connection = EQ_CDECL(void *, 0x004E013C, (int, int))(0, 0);
                    EQ_GLOBAL(void *, 0x006B3194) = connection;
                    if (connection == 0)
                    {
                        if (*(int *)EQ_GLOBAL(void *, 0x006EFE2C) == 1)
                        {
                            *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                            return;
                        }
                    }
                    else
                    {
                        EQ_CDECL(void, 0x004E041F, (char *))((char *)0x00604904);
                    }
                }
                guildChecksum = 0;
                result = EQ_CDECL(int, 0x004DFFB3, (void))();
                if (result != 0)
                    guildChecksum = EQ_CDECL(u32, 0x004E00D9, (void))();
                send_message_guess
                    (&EQ_GLOBAL(void *, 0x006B3194), 0x2192, &guildChecksum, 4, 1);
                EQ_METHOD(3, void, 0x004D472C, (void *, HWND, int, const char *), self)
                    (window, gameMode, (const char *)0x00604904);
            }
            else
            {
                result = EQ_CDECL(int, 0x004A3F60, (void))();
                if (result == 0)
                {
                    EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x0055e18c);
                    *(int *)((char *)self + 0x223f4) = 1;
                    *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                    EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
                    *(int *)self = 1;
                    connection = EQ_GLOBAL(void *, 0x006B3194);
                    if (connection != 0)
                    {
                        EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))
                            (connection, 10000, 0, 0);
                        EQ_GLOBAL(void *, 0x006B3194) = 0;
                    }
                }
                result = EQ_CDECL(int, 0x004A4116, (void))();
                if (result == 0)
                {
                    EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x00570518);
                    *(int *)((char *)self + 0x223f4) = 1;
                    *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                    EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
                    *(int *)self = 1;
                    connection = EQ_GLOBAL(void *, 0x006B3194);
                    if (connection != 0)
                    {
                        EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))
                            (connection, 10000, 0, 0);
                        EQ_GLOBAL(void *, 0x006B3194) = 0;
                    }
                }
                nameLength = EQ_CDECL(size_t, 0x005223D0, (const char *))
                    ((const char *)0x00604904);
                send_message_guess
                    (&EQ_GLOBAL(void *, 0x006B3194), 0x8001,
                     (void *)0x00604904, (u32)(nameLength + 1), 1);
                *((int *)self + 1) = PROCESS_GAME_STATE_WAITING;
                profile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
                EQ_GLOBAL(int, 0x006B63D0) = 0;
                EQ_GLOBAL(int, 0x006B61B4) = 0;
                if (profile != 0)
                {
                    if (profile != 0)
                    {
                        EQ_METHOD(0, void, 0x0047EF27, (void *), profile)();
                        EQ_CDECL(void, 0x005218BB, (void *))(profile);
                    }
                    EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
                }
                allocation = EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x1fa4);
                if (allocation == 0)
                    EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
                else
                    EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = (CharacterProfile *)
                        EQ_METHOD(1, void *, 0x0047EB8E, (void *, int), allocation)(0);

                startTime = EQ_CDECL(u32, 0x004A461E, (void))();
                timeout = startTime + 120000U;
                while (*((int *)self + 1) == PROCESS_GAME_STATE_WAITING)
                {
                    EQ_METHOD(0, void, 0x004C4EE7, (void *), self)();
                    if (EQ_GLOBAL(int, 0x006B63D0) != 0)
                    {
                        *((int *)self + 1) = PROCESS_GAME_STATE_ABORTED;
                        break;
                    }
                    if (EQ_GLOBAL(int, 0x006B61B4) == 0)
                    {
                        connection = EQ_GLOBAL(void *, 0x006B3194);
                        if (connection == 0)
                            result = 0;
                        else
                            result = EQ_CDECL(int, 0x004F23B0, (void *))(connection);
                        if (result == 0)
                        {
                            if (connection == 0)
                                strcpy(firstDisconnectReason, (const char *)0x0056f5cc);
                            else
                            {
                                EQ_CDECL(void, 0x004F2490, (void *, void *, u32))
                                    (connection, disconnectInfo, 0xc);
                                EQ_CDECL(int, 0x005224FB, (char *, const char *, int))
                                    (firstDisconnectReason, (const char *)0x0056f5dc,
                                     disconnectInfo[0]);
                            }
                            EQ_CDECL(void, 0x004C1703, (const char *, const char *, unsigned int, const char *))
                                ((const char *)0x00570398, (const char *)0x0056f5a4,
                                 0x4e33, firstDisconnectReason);
                            *(int *)((char *)self + 0x223f4) = 1;
                            EQ_METHOD(1, void, 0x004D0FE6, (void *, const char *), self)
                                ((const char *)0x0056f574);
                            ((void (WINAPI *)(DWORD))EQ_GLOBAL(void *, 0x00534198))(3000);
                            *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                            goto disconnected;
                        }
                    }
                    else
                    {
                        EQ_METHOD(3, void, 0x004D472C, (void *, HWND, int, const char *), self)
                            (window, gameMode, (const char *)0x00604904);
                    }
                    ((void (WINAPI *)(DWORD))EQ_GLOBAL(void *, 0x00534198))(10);
                    currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
                    if (timeout < currentTime)
                    {
                        EQ_METHOD(1, void, 0x004D0FE6, (void *, const char *), self)
                            ((const char *)0x0055f1a8);
                        ((void (WINAPI *)(DWORD))EQ_GLOBAL(void *, 0x00534198))(2000);
                        EQ_CDECL(void, 0x004A3D01, (const char *))((const char *)0x00570374);
                        EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x00570374);
                        *(int *)((char *)self + 0x223f4) = 1;
                        *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                        EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
                        *(int *)self = 1;
                        connection = EQ_GLOBAL(void *, 0x006B3194);
                        if (connection == 0)
                            return;
                        EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))
                            (connection, 10000, 0, 0);
                        EQ_GLOBAL(void *, 0x006B3194) = 0;
                        return;
                    }
                }
            }
            if (*(int *)self != 1 && *((int *)self + 1) != PROCESS_GAME_STATE_DONE)
                goto prepareGame;
            *(int *)self = 0;
            *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
            return;
        }
        if (DAT_006b6494 == 0)
        {
            EQ_CDECL(void, 0x004A3D01, (const char *))((const char *)0x005704cc);
            ((BOOL (WINAPI *)(HWND, LPCSTR))EQ_GLOBAL(void *, 0x00534258))
                (window, (const char *)0x00570494);
            profile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
            if (profile != 0)
            {
                if (profile != 0)
                {
                    EQ_METHOD(0, void, 0x0047EF27, (void *), profile)();
                    EQ_CDECL(void, 0x005218BB, (void *))(profile);
                }
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
            }
            allocation = EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x1fa4);
            if (allocation == 0)
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
            else
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = (CharacterProfile *)
                    EQ_METHOD(1, void *, 0x0047EB8E, (void *, int), allocation)(0);
            profile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
            if (EQ_GLOBAL(char, 0x006B62BC) == 0 ||
                EQ_CDECL(int, 0x005316C0, (const char *, const char *, unsigned int))
                    ((const char *)0x006b62bc, (const char *)0x0054599c, 7) == 0)
            {
                strcpy(profile->bind_zone, (const char *)0x0055c3e8);
                strcpy(profile->zone, (const char *)0x0055c3e8);
                strcpy((char *)0x006b62bc, (const char *)0x0055c3e8);
            }
            else
            {
                strcpy(profile->bind_zone, (const char *)0x006b62bc);
                strcpy(profile->zone, (const char *)0x006b62bc);
            }
            strcpy((char *)profile, (const char *)0x006b61bc);
        }
        else
        {
makeTutorialProfile:
            if (oldProfile != 0)
            {
                if (oldProfile != 0)
                {
                    EQ_METHOD(0, void, 0x0047EF27, (void *), oldProfile)();
                    EQ_CDECL(void, 0x005218BB, (void *))(oldProfile);
                }
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
            }
            allocation = EQ_CDECL(void *, 0x005218C6, (unsigned int))(0x1fa4);
            if (allocation == 0)
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
            else
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = (CharacterProfile *)
                    EQ_METHOD(1, void *, 0x0047EB8E, (void *, int), allocation)(0);
            profile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
            strcpy(profile->bind_zone, (const char *)0x0057050c);
            strcpy(profile->zone, (const char *)0x0057050c);
            strcpy((char *)profile, (const char *)0x00570504);
        }

prepareGame:
        EQ_METHOD(0, void, 0x004DAB44, (void *), self)();
        ((DWORD (WINAPI *)(LPCSTR, LPCSTR, LPCSTR, LPSTR, DWORD, LPCSTR))
            EQ_GLOBAL(void *, 0x0053418C))((const char *)0x0053db3c,
            (const char *)0x00570488, (const char *)0x0053db48, profileName,
            0x20, (const char *)0x0053db50);
        if (profileName[0] == '*')
        {
            ((BOOL (WINAPI *)(LPCSTR, LPCSTR, LPCSTR, LPCSTR))
                EQ_GLOBAL(void *, 0x00534188))((const char *)0x0053db3c,
                (const char *)0x00570488, (const char *)0x00558bac,
                (const char *)0x0053db50);
            screenMode = 0;
        }
        else
        {
            screenMode = initialScreenMode;
            if (profileName[0] != 'F' && profileName[0] != 'f' &&
                profileName[0] != '1' && g_no_render_offset_guess == 0)
                screenMode = 0;
        }
        g_game_state_guess = screenMode + 1;
        EXEC_SAVED_VIDEO_MODE = g_game_state_guess;
        if (EQ_GLOBAL(char, 0x006B63F8) == 0 && DAT_006b6494 == 0 &&
            EQ_METHOD(0, int, 0x00408E7F, (void *), g_display_guess)() != 0)
            EQ_METHOD(0, void, 0x0040AC31, (void *), g_display_guess)();

        if (*((int *)self + 1) != PROCESS_GAME_STATE_DONE)
        {
            do
            {
                if (EQ_GLOBAL(char, 0x006B7110) != 0)
                {
                    EQ_GLOBAL(int, 0x006B63C0) = 1;
                    connection = EQ_CDECL(void *, 0x004E013C, (int, int))(0, 0);
                    EQ_GLOBAL(void *, 0x006B3194) = connection;
                    EQ_GLOBAL(char, 0x006B7110) = 0;
                    playerName[0] = '\0';
                    if (connection == 0)
                        EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x00570444);
                    else
                        EQ_CDECL(void, 0x004E041F, (char *))(playerName);
                    result = EXEC_STRCMP
                        (playerName, (const char *)0x0058fba4);
                    if (result != 0)
                        EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x0057041c);
                    result = EXEC_STRCMP
                        ((const char *)0x00604904, (const char *)0x0058fba4);
                    if (result == 0)
                    {
                        EQ_CDECL(void, 0x004C1703, (const char *, const char *, unsigned int))
                            ((const char *)0x0057034c, (const char *)0x0056f5a4, 0x4f88);
                        EQ_CDECL(void, 0x004E0601, (void *))(connection);
                        EQ_GLOBAL(void *, 0x006B3194) = 0;
                        goto disconnectedScreen;
                    }
                    *((int *)self + 1) = PROCESS_GAME_STATE_WAITING;
                    EQ_GLOBAL(int, 0x006B63D0) = 0;
                    EQ_GLOBAL(int, 0x006B61B4) = 0;
                    EQ_CDECL(u32, 0x004A461E, (void))();
                    result = EQ_CDECL(int, 0x004A3F60, (void))();
                    if (result == 0)
                    {
                        EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x0055e18c);
                        *(int *)((char *)self + 0x223f4) = 1;
                        *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                        EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
                        *(int *)self = 1;
                        connection = EQ_GLOBAL(void *, 0x006B3194);
                        if (connection != 0)
                        {
                            EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))
                                (connection, 10000, 0, 0);
                            EQ_GLOBAL(void *, 0x006B3194) = 0;
                        }
                    }
                    result = EQ_CDECL(int, 0x004A4116, (void))();
                    if (result == 0)
                    {
                        EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x0055e18c);
                        *(int *)((char *)self + 0x223f4) = 1;
                        *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
                        EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
                        *(int *)self = 1;
                        connection = EQ_GLOBAL(void *, 0x006B3194);
                        if (connection != 0)
                        {
                            EQ_CDECL(void, 0x004F21D0, (void *, int, int, int))
                                (connection, 10000, 0, 0);
                            EQ_GLOBAL(void *, 0x006B3194) = 0;
                        }
                    }
                    EQ_CDECL(u32, 0x004A461E, (void))();
                    nameLength = EQ_CDECL(size_t, 0x005223D0, (const char *))
                        ((const char *)0x00604904);
                    send_message_guess
                        (&EQ_GLOBAL(void *, 0x006B3194), 0x8001,
                         (void *)0x00604904, (u32)(nameLength + 1), 1);
                    while (*((int *)self + 1) == PROCESS_GAME_STATE_WAITING)
                    {
                        EQ_METHOD(0, void, 0x004C4EE7, (void *), self)();
                        if (EQ_GLOBAL(int, 0x006B63D0) != 0)
                        {
                            *((int *)self + 1) = PROCESS_GAME_STATE_RETRY;
                            EQ_CDECL(u32, 0x004A461E, (void))();
                            break;
                        }
                        if (EQ_GLOBAL(int, 0x006B61B4) != 0)
                        {
                            EQ_CDECL(void, 0x004C1703, (const char *))((const char *)0x005703f0);
                            EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_ZONE;
                            *((int *)self + 1) = PROCESS_GAME_STATE_RETRY;
                            EQ_CDECL(void, 0x004E0601, (void *))
                                (EQ_GLOBAL(void *, 0x006B3194));
                            EQ_GLOBAL(void *, 0x006B3194) = 0;
                            EQ_METHOD(0, void, 0x0040AC31, (void *),
                                g_display_guess)();
                            break;
                        }
                        connection = EQ_GLOBAL(void *, 0x006B3194);
                        if (connection == 0)
                            result = 0;
                        else
                            result = EQ_CDECL(int, 0x004F23B0, (void *))(connection);
                        if (result == 0)
                        {
                            if (connection == 0)
                                strcpy(secondDisconnectReason, (const char *)0x0056f5cc);
                            else
                            {
                                EQ_CDECL(void, 0x004F2490, (void *, void *, u32))
                                    (connection, disconnectInfo, 0xc);
                                EQ_CDECL(int, 0x005224FB, (char *, const char *, int))
                                    (secondDisconnectReason, (const char *)0x0056f5dc,
                                     disconnectInfo[0]);
                            }
                            EQ_CDECL(void, 0x004C1703, (const char *, const char *, unsigned int, const char *))
                                ((const char *)0x0056f588, (const char *)0x0056f5a4,
                                 0x4f79, secondDisconnectReason);
                            goto disconnectedScreen;
                        }
                    }
                }
                EQ_METHOD(1, void, 0x004D4C19, (void *, char *), self)
                    ((char *)0x00604904);
                if (*(int *)self == 1)
                {
                    EQ_CDECL(void, 0x004C1703, (const char *, const char *, unsigned int))
                        ((const char *)0x005703bc, (const char *)0x0056f5a4, 0x4f9a);
                    break;
                }
                result = EQ_CDECL(int, 0x004CF561, (void))();
                if (result == 0)
                {
                    profileMessage[0] = 0xffffffff;
                    profileMessage[1] = 0xffffffff;
                    profileMessage[2] = 10;
                    send_message_guess
                        (&EQ_GLOBAL(void *, 0x006B3194), 0x2147, profileMessage, 0x5c, 1);
                }
                EQ_METHOD(1, void, 0x004D4F3A, (void *, HWND), self)(window);
                EQ_METHOD(1, void, 0x004D5D0F, (void *, HWND), self)(window);
                connection = EQ_GLOBAL(void *, 0x006018C0);
                if (connection != 0 && ((int *)connection)[4] != 0)
                    ((class EQ_ProcessGameWidget_guess *)connection)->Finish_guess();
                connection = EQ_GLOBAL(void *, 0x00604984);
                if (connection != 0 && ((int *)connection)[4] != 0)
                {
                    ((int *)connection)[0x7a] = 1;
                    ((class EQ_ProcessGameWidget_guess *)connection)->Finish_guess();
                }
                connection = EQ_GLOBAL(void *, 0x006E4E5C);
                if (connection != 0 && ((int *)connection)[4] != 0)
                {
                    *(char *)(((int *)connection)[0x5d] + 0xd6) = 0;
                    ((class EQ_ProcessGameWidget_guess *)connection)->Finish_guess();
                    ((int *)connection)[0x5d] = 0;
                }
                connection = EQ_GLOBAL(void *, 0x006E4E58);
                if (connection != 0 && ((int *)connection)[4] != 0)
                {
                    *(char *)(((int *)connection)[0x5d] + 0xd6) = 0;
                    ((class EQ_ProcessGameWidget_guess *)connection)->Finish_guess();
                    ((int *)connection)[0x5d] = 0;
                }
                EQ_GLOBAL(int, 0x006B63E8) = 0;
                EQ_CDECL(void, 0x004A3D01, (const char *))((const char *)0x005703d8);
                if (EQ_GLOBAL(int, 0x006B63F4) == PROCESS_GAME_REASON_DATA ||
                    EQ_GLOBAL(int, 0x006B63F4) == PROCESS_GAME_REASON_ZONE)
                {
                    if (EQ_GLOBAL(void *, 0x006E4E44) != 0)
                    {
                        pinstEQ_Main_guess->groupSelected_guess = (char)0xff;
                        pinstEQ_Main_guess->groupInvitePending_guess = 0;
                    }
                    EQ_METHOD(0, void, 0x004CB4FA, (void *), self)();
                }
                EQ_METHOD(0, void, 0x0040AC31, (void *),
                    g_display_guess)();
            }
            while (EQ_GLOBAL(int, 0x006B63F4) != PROCESS_GAME_REASON_DATA &&
                   EQ_GLOBAL(int, 0x006B63F4) != PROCESS_GAME_REASON_ZONE &&
                   *((int *)self + 1) != PROCESS_GAME_STATE_DONE);
        }
        strcpy((char *)0x00604904, (const char *)0x0058fba4);
        profile = EQ_GLOBAL(CharacterProfile *, 0x006E4DA8);
        if (EQ_GLOBAL(int, 0x006B63F4) == PROCESS_GAME_REASON_ZONE)
        {
            if (profile != 0)
            {
                if (profile != 0)
                {
                    EQ_METHOD(0, void, 0x0047EF27, (void *), profile)();
                    EQ_CDECL(void, 0x005218BB, (void *))(profile);
                }
                EQ_GLOBAL(CharacterProfile *, 0x006E4DA8) = 0;
            }
            send_message_guess
                (&EQ_GLOBAL(void *, 0x006B3194), 0x20d9, 0, 0, 1);
            if (EQ_GLOBAL(int, 0x00590158) == 0)
                EQ_CDECL(void, 0x0040101D, (int))(0xffffffff);
        }
        if (EQ_GLOBAL(int, 0x006B63F4) == PROCESS_GAME_REASON_DATA)
        {
            *(int *)self = 0;
            *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
            return;
        }
    }
    while (*((int *)self + 1) != PROCESS_GAME_STATE_DONE);
    return;

disconnectedScreen:
    *(int *)((char *)self + 0x223f4) = 1;
    EQ_METHOD(1, void, 0x004D0FE6, (void *, const char *), self)((const char *)0x0056f574);
    ((void (WINAPI *)(DWORD))EQ_GLOBAL(void *, 0x00534198))(3000);
    *((int *)self + 1) = PROCESS_GAME_STATE_DONE;
disconnected:
    EQ_GLOBAL(int, 0x006B63F4) = PROCESS_GAME_REASON_DATA;
    *(int *)self = 1;
    return;
}
#undef CEverQuest

// Loading-screen object (EQ_LoadingS; the class is not in include/). Its constructor is the client's, only
// declared here. It has no virtual functions (a constructor of ours would write its vtable pointer over the
// client's): its vtable is called through EQ_LoadingS_vtable_guess, which is never constructed.
class EQ_LoadingS_vtable_guess
{
public:
    virtual void Slot0_guess(void);         // vtable +0x00, not called here
    virtual void Start_guess(void);         // vtable +0x04
    virtual void Finish_guess(void);        // vtable +0x08
};

class EQ_LoadingS_guess
{
public:
    // EXTERNAL: TRILOGY 0x0043AAFA EQ_LoadingS_guess::EQ_LoadingS_guess
    EQ_LoadingS_guess();
    ~EQ_LoadingS_guess()                    // 0x0043ABD2, inline so delete calls it where VC6 expands it
    {
        EQ_METHOD(0, void, 0x0043ABD2, (EQ_LoadingS_guess *), this)();
    }
    u8      unknown0x00[0x80];              // 0x0000 (vtable pointer, EQ_Overlay part)
    int     field_0x80_guess;               // 0x0080  zeroed after the new
    int     field_0x84_guess;               // 0x0084  zeroed after the new
    u8      unknown0x88[0x3C];              // 0x0088
};
C_ASSERT(sizeof(EQ_LoadingS_guess) == 0xC4);

class CEverQuest_ShowLoadingScreen_standin_guess
{
public:
    void ShowLoadingScreen_guess(void);
};

// FUNCTION: TRILOGY 0x004dab44
// Presents the loading screen during startup and game loading.
// __thiscall with no arguments: `this` is never read (the callers load a CEverQuest pointer into ECX). Frees the
// loading object in DAT_006e4e64, makes a new one and starts it, runs two rounds of (0x0043ABDD on it, then
// ShowDisplay), finishes it and frees it again.
void CEverQuest_ShowLoadingScreen_standin_guess::ShowLoadingScreen_guess(void)
{
    if (EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64) != 0)
    {
        delete EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64);
        EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64) = 0;
    }
    EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64) = new EQ_LoadingS_guess;
    EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64)->field_0x84_guess = 0;
    EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64)->field_0x80_guess = 0;
    ((EQ_LoadingS_vtable_guess *)EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64))->Start_guess();
    for (int i = 2; i != 0; i--)
    {
        EQ_METHOD(0, void, 0x0043ABDD, (EQ_LoadingS_guess *), EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64))();
        EQ_METHOD(0, void, 0x0040E418, (void *), g_display_guess)();
        EQ_METHOD(0, void, 0x0043ABDD, (EQ_LoadingS_guess *), EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64))();
        EQ_METHOD(0, void, 0x0040E418, (void *), g_display_guess)();
    }
    ((EQ_LoadingS_vtable_guess *)EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64))->Finish_guess();
    if (EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64) != 0)
    {
        delete EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64);
        EQ_GLOBAL(EQ_LoadingS_guess *, 0x006E4E64) = 0;
    }
}

// Each global below holds an object whose destructor the client calls by address (ECX = object) before the
// cdecl operator delete. The classes are file-local stand-ins whose inline destructor forwards to that address,
// so `delete` expands to the original's `if (p) { dtor(p); operator delete(p); }`.
// The address stays literal inside EQ_THISCALL / EQ_GLOBAL at each use (the match build scans the source for them).
#define EQ_DTOR_CLASS_GUESS(name, dtorCall)                                 \
    class name                                                              \
    {                                                                       \
    public:                                                                 \
        ~name() { dtorCall(this); }                                         \
    };
#define EQ_FREE_OBJECT_GUESS(global)                                        \
    if (global != 0)                                                        \
    {                                                                       \
        delete global;                                                      \
        global = 0;                                                         \
    }

EQ_DTOR_CLASS_GUESS(EQ_Dtor0048A539_guess, EQ_THISCALL(void, 0x0048A539, (EQ_Dtor0048A539_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004155C6_guess, EQ_THISCALL(void, 0x004155C6, (EQ_Dtor004155C6_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004869D2_guess, EQ_THISCALL(void, 0x004869D2, (EQ_Dtor004869D2_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00485FBC_guess, EQ_THISCALL(void, 0x00485FBC, (EQ_Dtor00485FBC_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00456584_guess, EQ_THISCALL(void, 0x00456584, (EQ_Dtor00456584_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0048143C_guess, EQ_THISCALL(void, 0x0048143C, (EQ_Dtor0048143C_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00451BE9_guess, EQ_THISCALL(void, 0x00451BE9, (EQ_Dtor00451BE9_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00451B4C_guess, EQ_THISCALL(void, 0x00451B4C, (EQ_Dtor00451B4C_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0045143F_guess, EQ_THISCALL(void, 0x0045143F, (EQ_Dtor0045143F_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00451339_guess, EQ_THISCALL(void, 0x00451339, (EQ_Dtor00451339_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0045129C_guess, EQ_THISCALL(void, 0x0045129C, (EQ_Dtor0045129C_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004511FF_guess, EQ_THISCALL(void, 0x004511FF, (EQ_Dtor004511FF_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00451162_guess, EQ_THISCALL(void, 0x00451162, (EQ_Dtor00451162_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450E45_guess, EQ_THISCALL(void, 0x00450E45, (EQ_Dtor00450E45_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450EE2_guess, EQ_THISCALL(void, 0x00450EE2, (EQ_Dtor00450EE2_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450F7F_guess, EQ_THISCALL(void, 0x00450F7F, (EQ_Dtor00450F7F_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0045101C_guess, EQ_THISCALL(void, 0x0045101C, (EQ_Dtor0045101C_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004510C5_guess, EQ_THISCALL(void, 0x004510C5, (EQ_Dtor004510C5_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450B34_guess, EQ_THISCALL(void, 0x00450B34, (EQ_Dtor00450B34_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450BD1_guess, EQ_THISCALL(void, 0x00450BD1, (EQ_Dtor00450BD1_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450C6E_guess, EQ_THISCALL(void, 0x00450C6E, (EQ_Dtor00450C6E_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450D0B_guess, EQ_THISCALL(void, 0x00450D0B, (EQ_Dtor00450D0B_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00450DA8_guess, EQ_THISCALL(void, 0x00450DA8, (EQ_Dtor00450DA8_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004363EA_guess, EQ_THISCALL(void, 0x004363EA, (EQ_Dtor004363EA_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00432259_guess, EQ_THISCALL(void, 0x00432259, (EQ_Dtor00432259_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00416EBA_guess, EQ_THISCALL(void, 0x00416EBA, (EQ_Dtor00416EBA_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004347CE_guess, EQ_THISCALL(void, 0x004347CE, (EQ_Dtor004347CE_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00435F5C_guess, EQ_THISCALL(void, 0x00435F5C, (EQ_Dtor00435F5C_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00435FF5_guess, EQ_THISCALL(void, 0x00435FF5, (EQ_Dtor00435FF5_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0043608E_guess, EQ_THISCALL(void, 0x0043608E, (EQ_Dtor0043608E_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor0048D20F_guess, EQ_THISCALL(void, 0x0048D20F, (EQ_Dtor0048D20F_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004301B1_guess, EQ_THISCALL(void, 0x004301B1, (EQ_Dtor004301B1_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor00438A1E_guess, EQ_THISCALL(void, 0x00438A1E, (EQ_Dtor00438A1E_guess *)))
EQ_DTOR_CLASS_GUESS(EQ_Dtor004189F8_guess, EQ_THISCALL(void, 0x004189F8, (EQ_Dtor004189F8_guess *)))

// Each window object InitOverlays creates is a file-local stand-in of the object's size whose constructor is
// the client's, declared here only (VC6 then keeps the EH frame around `new`; the EXTERNAL markers below the
// function give the addresses).
#define EQ_NEW_CLASS_GUESS(name, size)                                      \
    class name                                                              \
    {                                                                       \
    public:                                                                 \
        name();                                                             \
        char data_guess[size];                                              \
    };
#define EQ_NEW_CLASS_ARG_GUESS(name, size)                                  \
    class name                                                              \
    {                                                                       \
    public:                                                                 \
        name(int arg);                                                      \
        char data_guess[size];                                              \
    };
#define EQ_INIT_OVERLAY_GUESS(global, type)                                 \
    if (global == 0)                                                        \
        global = new type;
#define EQ_INIT_OVERLAY_ARG_GUESS(global, type, arg)                        \
    if (global == 0)                                                        \
        global = new type(arg);

EQ_NEW_CLASS_GUESS(EQ_New0048D188_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00434607_guess, 0xB4)
EQ_NEW_CLASS_GUESS(EQ_New00435ECE_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00435F67_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00436000_guess, 0x80)
EQ_NEW_CLASS_ARG_GUESS(EQ_New00416D0C_guess, 0x178)
EQ_NEW_CLASS_ARG_GUESS(EQ_New0041893B_guess, 0x37B0)
EQ_NEW_CLASS_GUESS(EQ_New0041551E_guess, 0x46C)
EQ_NEW_CLASS_GUESS(EQ_New0043629A_guess, 0x2668)
EQ_NEW_CLASS_GUESS(EQ_New0043208F_guess, 0x118C)
EQ_NEW_CLASS_GUESS(EQ_New00450D16_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450C79_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450BDC_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450B3F_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450AA2_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00451033_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450F8A_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450EED_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450E50_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00450DB3_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New004510D0_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New0045116D_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New0045120A_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New004512A7_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00451ABA_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00451B57_guess, 0x80)
EQ_NEW_CLASS_GUESS(EQ_New00481320_guess, 0x5EC)
EQ_NEW_CLASS_GUESS(EQ_New00438967_guess, 0xA4)
EQ_NEW_CLASS_GUESS(EQ_New00451BF4_guess, 0x9104C)
EQ_NEW_CLASS_GUESS(EQ_New00485DE2_guess, 0x1F0)
EQ_NEW_CLASS_GUESS(EQ_New004867FE_guess, 0x1FC)
EQ_NEW_CLASS_ARG_GUESS(EQ_New0048A278_guess, 0x208)

class CEverQuest_InitOverlays_standin_guess
{
public:
    void InitOverlays(void);
};

// FUNCTION: TRILOGY 0x004dac29
// Initializes the game's overlay and interface windows.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// __thiscall with no arguments and no result: `this` is never read (the only caller, DoLogin, loads ECX).
// Creates the t3d overlay handle in DAT_006e4de4 (eqgfx slot 0x006E5340, cdecl: width 0x80, height 0x20, 1), then
// news every window object whose global is still null. Object sizes and constructors in the order of the client.
void CEverQuest_InitOverlays_standin_guess::InitOverlays(void)
{
    typedef int (__cdecl *T3dOverlayCreate_guess)(int, int, int);

    if (EQ_GLOBAL(int, 0x006E4DE4) == 0)
        EQ_GLOBAL(int, 0x006E4DE4) = EQ_GLOBAL(T3dOverlayCreate_guess, 0x006E5340)(0x80, 0x20, 1);
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0048D188_guess *, 0x006E4E68), EQ_New0048D188_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00434607_guess *, 0x006E4E48), EQ_New00434607_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00435ECE_guess *, 0x006E4E4C), EQ_New00435ECE_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00435F67_guess *, 0x006E4E50), EQ_New00435F67_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00436000_guess *, 0x006E4E54), EQ_New00436000_guess)
    EQ_INIT_OVERLAY_ARG_GUESS(EQ_GLOBAL(EQ_New00416D0C_guess *, 0x006E4E58), EQ_New00416D0C_guess, 0)
    EQ_INIT_OVERLAY_ARG_GUESS(EQ_GLOBAL(EQ_New00416D0C_guess *, 0x006E4E5C), EQ_New00416D0C_guess, 0)
    EQ_INIT_OVERLAY_ARG_GUESS(EQ_GLOBAL(EQ_New0041893B_guess *, 0x006E4E60), EQ_New0041893B_guess, 0)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0041551E_guess *, 0x006E4DEC), EQ_New0041551E_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0043629A_guess *, 0x006E4DF4), EQ_New0043629A_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0043208F_guess *, 0x006E4DF8), EQ_New0043208F_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450D16_guess *, 0x006E4DFC), EQ_New00450D16_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450C79_guess *, 0x006E4E00), EQ_New00450C79_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450BDC_guess *, 0x006E4E04), EQ_New00450BDC_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450B3F_guess *, 0x006E4E08), EQ_New00450B3F_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450AA2_guess *, 0x006E4E0C), EQ_New00450AA2_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00451033_guess *, 0x006E4E10), EQ_New00451033_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450F8A_guess *, 0x006E4E14), EQ_New00450F8A_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450EED_guess *, 0x006E4E18), EQ_New00450EED_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450E50_guess *, 0x006E4E1C), EQ_New00450E50_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00450DB3_guess *, 0x006E4E24), EQ_New00450DB3_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New004510D0_guess *, 0x006E4E28), EQ_New004510D0_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0045116D_guess *, 0x006E4E2C), EQ_New0045116D_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New0045120A_guess *, 0x006E4E30), EQ_New0045120A_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New004512A7_guess *, 0x006E4E34), EQ_New004512A7_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00451ABA_guess *, 0x006E4E3C), EQ_New00451ABA_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00451B57_guess *, 0x006E4E40), EQ_New00451B57_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00481320_guess *, 0x006E4E20), EQ_New00481320_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00438967_guess *, 0x006E4D84), EQ_New00438967_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00451BF4_guess *, 0x006E4E44), EQ_New00451BF4_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New00485DE2_guess *, 0x00604984), EQ_New00485DE2_guess)
    EQ_INIT_OVERLAY_GUESS(EQ_GLOBAL(EQ_New004867FE_guess *, 0x006018C0), EQ_New004867FE_guess)
    EQ_INIT_OVERLAY_ARG_GUESS(EQ_GLOBAL(EQ_New0048A278_guess *, 0x006E4DE8), EQ_New0048A278_guess,
                              pinstEQ_Main_guess->overlaySprite_int_guess)
}

// The window objects' constructors are the client's, only declared above: `new` calls them directly.
// EXTERNAL: TRILOGY 0x0048D188 EQ_New0048D188_guess::EQ_New0048D188_guess
// EXTERNAL: TRILOGY 0x00434607 EQ_New00434607_guess::EQ_New00434607_guess
// EXTERNAL: TRILOGY 0x00435ECE EQ_New00435ECE_guess::EQ_New00435ECE_guess
// EXTERNAL: TRILOGY 0x00435F67 EQ_New00435F67_guess::EQ_New00435F67_guess
// EXTERNAL: TRILOGY 0x00436000 EQ_New00436000_guess::EQ_New00436000_guess
// EXTERNAL: TRILOGY 0x00416D0C EQ_New00416D0C_guess::EQ_New00416D0C_guess
// EXTERNAL: TRILOGY 0x0041893B EQ_New0041893B_guess::EQ_New0041893B_guess
// EXTERNAL: TRILOGY 0x0041551E EQ_New0041551E_guess::EQ_New0041551E_guess
// EXTERNAL: TRILOGY 0x0043629A EQ_New0043629A_guess::EQ_New0043629A_guess
// EXTERNAL: TRILOGY 0x0043208F EQ_New0043208F_guess::EQ_New0043208F_guess
// EXTERNAL: TRILOGY 0x00450D16 EQ_New00450D16_guess::EQ_New00450D16_guess
// EXTERNAL: TRILOGY 0x00450C79 EQ_New00450C79_guess::EQ_New00450C79_guess
// EXTERNAL: TRILOGY 0x00450BDC EQ_New00450BDC_guess::EQ_New00450BDC_guess
// EXTERNAL: TRILOGY 0x00450B3F EQ_New00450B3F_guess::EQ_New00450B3F_guess
// EXTERNAL: TRILOGY 0x00450AA2 EQ_New00450AA2_guess::EQ_New00450AA2_guess
// EXTERNAL: TRILOGY 0x00451033 EQ_New00451033_guess::EQ_New00451033_guess
// EXTERNAL: TRILOGY 0x00450F8A EQ_New00450F8A_guess::EQ_New00450F8A_guess
// EXTERNAL: TRILOGY 0x00450EED EQ_New00450EED_guess::EQ_New00450EED_guess
// EXTERNAL: TRILOGY 0x00450E50 EQ_New00450E50_guess::EQ_New00450E50_guess
// EXTERNAL: TRILOGY 0x00450DB3 EQ_New00450DB3_guess::EQ_New00450DB3_guess
// EXTERNAL: TRILOGY 0x004510D0 EQ_New004510D0_guess::EQ_New004510D0_guess
// EXTERNAL: TRILOGY 0x0045116D EQ_New0045116D_guess::EQ_New0045116D_guess
// EXTERNAL: TRILOGY 0x0045120A EQ_New0045120A_guess::EQ_New0045120A_guess
// EXTERNAL: TRILOGY 0x004512A7 EQ_New004512A7_guess::EQ_New004512A7_guess
// EXTERNAL: TRILOGY 0x00451ABA EQ_New00451ABA_guess::EQ_New00451ABA_guess
// EXTERNAL: TRILOGY 0x00451B57 EQ_New00451B57_guess::EQ_New00451B57_guess
// EXTERNAL: TRILOGY 0x00481320 EQ_New00481320_guess::EQ_New00481320_guess
// EXTERNAL: TRILOGY 0x00438967 EQ_New00438967_guess::EQ_New00438967_guess
// EXTERNAL: TRILOGY 0x00451BF4 EQ_New00451BF4_guess::EQ_New00451BF4_guess
// EXTERNAL: TRILOGY 0x00485DE2 EQ_New00485DE2_guess::EQ_New00485DE2_guess
// EXTERNAL: TRILOGY 0x004867FE EQ_New004867FE_guess::EQ_New004867FE_guess
// EXTERNAL: TRILOGY 0x0048A278 EQ_New0048A278_guess::EQ_New0048A278_guess

// FUNCTION: TRILOGY 0x004db2eb
// Shuts down the UI: destroys the t3d overlay and deletes every window object held in a global.
// __thiscall with no arguments and no result: `this` is never read (the caller, 0x0040AFA1, loads ECX from
// 0x006EFE2C). Shutdown of the UI: destroys the t3d overlay handle in DAT_006e4de4 through the eqgfx
// t3dOverlayDestroy slot (0x006E5344), then deletes each window object held in a global (destructor by address,
// then the client's operator delete) and clears the global. g_chatWindow_guess is cleared even when it was null.
void __fastcall ShutdownUiWindows_guess(void *thisUnused_guess)
{
    typedef void (__cdecl *T3dOverlayDestroy_guess)(int);
    void* object_guess;

    // A one-argument fastcall places the object in ECX, matching these no-stack-argument destructors.
#define EQ_FREE_OBJECT_DIRECT_GUESS(global, dtorAddress)                      \
    if (global != 0)                                                         \
    {                                                                        \
        object_guess = global;                                               \
        if (object_guess != 0)                                               \
        {                                                                    \
            EQ_FUNC(void, __fastcall, dtorAddress, (void*))(object_guess);   \
            EQ_CDECL(void, 0x005218BB, (void*))(object_guess);               \
        }                                                                    \
        global = 0;                                                          \
    }

    if (EQ_GLOBAL(int, 0x006E4DE4) != 0)
    {
        EQ_GLOBAL(T3dOverlayDestroy_guess, 0x006E5344)(EQ_GLOBAL(int, 0x006E4DE4));
        EQ_GLOBAL(int, 0x006E4DE4) = 0;
    }
    if (EQ_GLOBAL(EQ_Dtor0048A539_guess *, 0x006E4DE8) != 0)
    {
        object_guess = EQ_GLOBAL(EQ_Dtor0048A539_guess *, 0x006E4DE8);
        if (object_guess != 0)
        {
            EQ_FUNC(void, __fastcall, 0x0048A539, (void*))(object_guess);
            EQ_CDECL(void, 0x005218BB, (void*))(object_guess);
        }
    }
    EQ_GLOBAL(EQ_Dtor0048A539_guess *, 0x006E4DE8) = 0;
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004155C6_guess *, 0x006E4DEC), 0x004155C6)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004869D2_guess *, 0x006018C0), 0x004869D2)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00485FBC_guess *, 0x00604984), 0x00485FBC)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00456584_guess *, 0x006E4E44), 0x00456584)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0048143C_guess *, 0x006E4E20), 0x0048143C)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00451BE9_guess *, 0x006E4E40), 0x00451BE9)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00451B4C_guess *, 0x006E4E3C), 0x00451B4C)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0045143F_guess *, 0x006E4E38), 0x0045143F)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00451339_guess *, 0x006E4E34), 0x00451339)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0045129C_guess *, 0x006E4E30), 0x0045129C)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004511FF_guess *, 0x006E4E2C), 0x004511FF)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00451162_guess *, 0x006E4E28), 0x00451162)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450E45_guess *, 0x006E4E24), 0x00450E45)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450EE2_guess *, 0x006E4E1C), 0x00450EE2)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450F7F_guess *, 0x006E4E18), 0x00450F7F)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0045101C_guess *, 0x006E4E14), 0x0045101C)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004510C5_guess *, 0x006E4E10), 0x004510C5)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450B34_guess *, 0x006E4E0C), 0x00450B34)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450BD1_guess *, 0x006E4E08), 0x00450BD1)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450C6E_guess *, 0x006E4E04), 0x00450C6E)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450D0B_guess *, 0x006E4E00), 0x00450D0B)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00450DA8_guess *, 0x006E4DFC), 0x00450DA8)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004363EA_guess *, 0x006E4DF4), 0x004363EA)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00432259_guess *, 0x006E4DF8), 0x00432259)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00416EBA_guess *, 0x006E4E58), 0x00416EBA)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00416EBA_guess *, 0x006E4E5C), 0x00416EBA)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004347CE_guess *, 0x006E4E48), 0x004347CE)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00435F5C_guess *, 0x006E4E4C), 0x00435F5C)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00435FF5_guess *, 0x006E4E50), 0x00435FF5)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0043608E_guess *, 0x006E4E54), 0x0043608E)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor0048D20F_guess *, 0x006E4E68), 0x0048D20F)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004301B1_guess *, 0x006E4E6C), 0x004301B1)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor00438A1E_guess *, 0x006E4D84), 0x00438A1E)
    EQ_FREE_OBJECT_DIRECT_GUESS(EQ_GLOBAL(EQ_Dtor004189F8_guess *, 0x006E4E60), 0x004189F8)

#undef EQ_FREE_OBJECT_DIRECT_GUESS
}

// FUNCTION: TRILOGY 0x004db7d2
// Refreshes the main game and item or spell windows during zone entry.
void CEverQuest_EnterZone3_guess(void)
{
    if (EQ_GLOBAL(void*, 0x006E4E44) != NULL)
        EQ_METHOD(0, void, 0x0043B010, (void*), EQ_GLOBAL(void*, 0x006E4E44))();
    if (EQ_GLOBAL(void*, 0x006E4D84) != NULL)
    {
        EQ_METHOD(0, void, 0x00438AD4, (void*), EQ_GLOBAL(void*, 0x006E4D84))();
        return;
    }
    return;
}

struct InitialActorDefinition_guess
{
    u32 unknown0x00;
    InitialActorDefinition_guess* next;
    u8 unknown0x08[0x1c];
    void* actor;
    f32 y;
    f32 x;
    f32 z;
    f32 heading;
    char actorName[1];
};

struct InitialPlacedActor_guess
{
    u32 unknown0x00;
    InitialPlacedActor_guess* next;
    u8 unknown0x08[0x2c];
    void* actor;
    f32 y;
    f32 x;
    f32 z;
    f32 heading;
    f32 pitch;
    u8 unknown0x4c[4];
    char actorName[1];
};

class CEverQuest_CreateInitialActors_standin_guess
{
public:
    void CreateInitialActors_guess(void);
};

// FUNCTION: TRILOGY 0x004db7f1
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void CEverQuest_CreateInitialActors_standin_guess::CreateInitialActors_guess(void)
{
    char actorDefinitionName[128];
    EQPlayer* player;
    InitialActorDefinition_guess* actorDefinition;
    InitialPlacedActor_guess* placedActor;

    player = EQ_GLOBAL(EQPlayer*, 0x006E4D58);
    while (player != NULL)
    {
        if (player->actor_info->actor == NULL)
        {
            EQ_METHOD(1, void, 0x0040F731, (void*, EQPlayer*), g_display_guess)(player);
            EQ_METHOD(1, void, 0x00410602, (void*, EQPlayer*), g_display_guess)(player);
            if (EQ_GLOBAL(CharacterProfile*, 0x006E4DA8) != NULL &&
                EXEC_STRCMP(
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->name, player->name) == 0)
            {
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = player;
                player->stand_state = 'd';
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->stand_state = 'd';
                EQ_GLOBAL(int, 0x006E4DA4) &= 0;
                EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
                EQ_METHOD(1, void, 0x0040E422, (void*, void*), g_display_guess)(
                    player->actor_info->actor);
            }
        }
        player = player->next;
    }

    actorDefinition = EQ_GLOBAL(InitialActorDefinition_guess*, 0x006E4D5C);
    while (actorDefinition != NULL)
    {
        if (actorDefinition->actor == NULL)
        {
            sprintf(
                actorDefinitionName, EQ_ARRAY(char, 0x0055DCC0), actorDefinition->actorName);
            actorDefinition->actor = EQ_METHOD(9, void*, 0x004112DF,
                (void*, const char*, float, float, float, float, float, float, int, int),
                g_display_guess)(
                    actorDefinitionName, actorDefinition->y, actorDefinition->x,
                    actorDefinition->z, actorDefinition->heading, 0.0f, 0.0f, 1, 1);
        }
        actorDefinition = actorDefinition->next;
    }

    placedActor = EQ_GLOBAL(InitialPlacedActor_guess*, 0x006E4D60);
    while (placedActor != NULL)
    {
        if (placedActor->actor == NULL)
        {
            placedActor->actor = EQ_METHOD(9, void*, 0x004112DF,
                (void*, const char*, float, float, float, float, float, float, int, int),
                g_display_guess)(
                    placedActor->actorName, placedActor->y, placedActor->x,
                    placedActor->z, placedActor->heading, placedActor->pitch, 0.0f, 1, 1);
        }
        placedActor = placedActor->next;
    }
    return;
}

class CEverQuest_InterpretCmd_standin_guess
{
public:
    void InterpretCmd(int chatMode, char* chatText);
};

// FUNCTION: TRILOGY 0x004db9b5
// Routes player input to slash commands or sends ordinary text through the selected chat channel.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: the command-match block is laid out before the post-loop code (original: after the switch), and localPlayer is loaded into EBX at the top because counter = 0 sits at the loop entry (no zero register there)
void CEverQuest_InterpretCmd_standin_guess::InterpretCmd(int localPlayer, char* chatText)
{
    typedef void (__cdecl *InterpretCmd_command_guess)(int, char*);
    char tellArgs[512];
    char historyText[512];
    char commandWord[512];
    char chatArgs[512];
    int curChar;
    char* nextChar;
    char** cmdEntry;
    char** nextEntry;
    char* historyEntry;
    int result;
    int counter;

    if (localPlayer != 0 &&
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C) != 0 &&
        EQ_GLOBAL(int, 0x006E4DE8) != 0 &&
        *chatText != '\0')
    {
        curChar = *chatText;
        while (EQ_CDECL(int, 0x00522F4D, (int))(curChar) != 0)
        {
            curChar = chatText[1];
            chatText++;
        }

        EQ_GLOBAL(short, 0x006E0FE8) = -1;
        strcpy(chatArgs, chatText);
        strcpy(historyText, chatText);

        if (*chatText == '/' ||
            EQ_CDECL(int, 0x00522ECF, (int))(*chatText) != 0 ||
            EQ_CDECL(int, 0x00522F25, (int))(*chatText) != 0)
        {
            chatText = EQ_METHOD(2, char*, 0x004DBC84, (void*, char*, char*), this)(chatText, commandWord);
        }
        else
        {
            commandWord[0] = *chatText;
            commandWord[1] = 0;
            do
            {
                nextChar = chatText + 1;
                chatText++;
                result = EQ_CDECL(int, 0x00522F4D, (int))(*nextChar);
            } while (result != 0);
        }

        if (EQ_GLOBAL(char, 0x006E20F4) == '\0' ||
            (result = EQ_CDECL(int, 0x004A4797, (const char*, const char*))(commandWord, "/safelock")) != 0)
        {
            if (*EQ_GLOBAL(char*, 0x00560210) != '\0')
            {
                counter = 0;
                cmdEntry = EQ_ARRAY(char*, 0x00560210);
                do
                {
                    if (commandWord[0] == **cmdEntry &&
                        EQ_CDECL(int, 0x004A4797, (const char*, const char*))(commandWord, *cmdEntry) != 0)
                    {
                        if (EQ_ARRAY(short, 0x00560218)[counter * 6] <= 0 ||
                            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gm != '\0')
                        {
                            ((InterpretCmd_command_guess)
                                EQ_ARRAY(void*, 0x00560214)[counter * 3])
                                (localPlayer, chatText);
                            result = EQ_CDECL(int, 0x004A4797, (const char*, const char*))
                                (commandWord, "/safelock");
                            if (result == 0)
                            {
                                counter = 0x13;
                                historyEntry = (char*)0x006E07E8;
                                do
                                {
                                    strcpy(historyEntry, historyEntry - 0x800);
                                    counter--;
                                    historyEntry -= 0x800;
                                } while (counter != 0);
                                strcpy((char*)0x006D6FE8, historyText);
                            }
                            /* fake match: the original jumps to the one shared render call (LAB_004dbc75); the goto keeps that single call site */
                            goto RenderWorld;
                        }
                        break;
                    }
                    nextEntry = cmdEntry + 3;
                    cmdEntry = nextEntry;
                    counter++;
                } while (**nextEntry != '\0');
            }

            if (chatArgs[0] != '/' && chatArgs[0] != '\0')
            {
                counter = 0x13;
                historyEntry = (char*)0x006E07E8;
                do
                {
                    strcpy(historyEntry, historyEntry - 0x800);
                    counter--;
                    historyEntry -= 0x800;
                } while (counter != 0);
                strcpy((char*)0x006D6FE8, historyText);

                switch (EQ_GLOBAL(unsigned char, 0x006B6428))
                {
                case 0:
                    EQ_CDECL(void, 0x004A5703, (int, char*))(localPlayer, chatArgs);
                    break;
                case 2:
                    EQ_CDECL(void, 0x004A808C, (int, char*))(localPlayer, chatArgs);
                    break;
                case 4:
                    EQ_CDECL(void, 0x004A810C, (int, char*))(localPlayer, chatArgs);
                    break;
                case 5:
                    EQ_CDECL(void, 0x004A812D, (int, char*))(localPlayer, chatArgs);
                    break;
                case 7:
                    sprintf(tellArgs, "%s %s", &EQ_GLOBAL(char, 0x006B642C), chatArgs);
                    EQ_CDECL(void, 0x004A814E, (int, char*))(localPlayer, tellArgs);
                    break;
                default:
                    EQ_CDECL(void, 0x004A7F9B, (int, char*))(localPlayer, chatArgs);
                    break;
                }
            }
RenderWorld:
            EQ_METHOD(0, void, 0x004D0540, (void*), this)();
        }
        else
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(
                 "Interface locked.  Please use /safelock <password> to unlock it.");
            EQ_METHOD(0, void, 0x004D0540, (void*), this)();
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004dbc84
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (by address)
// __thiscall (callers load ECX from pinstCEverQuest), `this` unused. GrabFirstWord without the size limit:
// skips white space, copies one word (up to a space, or up to the matching quote when it starts with ' or
// ") into `word`, skips the closing quote/space and the white space after it; returns the rest.
char* CEverQuest::GrabFirstWord2_guess(char* text, char* word)
{
    char delim = ' ';

    while (isspace(*text))
        text++;
    if (*text == '\'' || *text == '"')
    {
        delim = *text;
        text++;
    }
    while (*text != '\0')
    {
        if (*text == delim)
        {
            text++;
            break;
        }
        *word++ = *text++;
    }
    *word = '\0';
    while (isspace(*text))
        text++;
    return text;
}

// FUNCTION: TRILOGY 0x004dbce1
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::GrabFirstWord, high)
// __thiscall (`ret 0xc`), `this` unused. Skips white space, copies one word (up to a space, or up to the
// matching quote when it starts with ' or ") into `word` (at most wordSize - 1 characters, always
// terminated), skips the closing quote/space and the white space after it; returns the rest.
char* CEverQuest::GrabFirstWord(char* inputText, char* word, int wordSize)
{
    char delim = ' ';
    int n;

    while (isspace(*inputText))
        inputText++;
    if (*inputText == '\'' || *inputText == '"')
    {
        delim = *inputText;
        inputText++;
    }
    n = 0;
    while (*inputText != '\0')
    {
        if (*inputText == delim)
        {
            inputText++;
            break;
        }
        *word++ = *inputText++;
        n++;
        if (n >= wordSize - 1)
            break;
    }
    *word = '\0';
    while (isspace(*inputText))
        inputText++;
    return inputText;
}

// FUNCTION: TRILOGY 0x004dbd4b
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (Mac: CEverQuest::CopyFirstWord, high)
// __thiscall (`ret 8`), `this` unused. Copies characters up to the first white space and terminates.
// Faithful bug: the terminator is not a stop condition (isspace(0) is false), so text without white
// space is copied past its end until some white-space byte turns up.
void CEverQuest::CopyFirstWord(char* text, char* word)
{
    int n = 0;

    while (!isspace(text[n]))
    {
        word[n] = text[n];
        n++;
    }
    word[n] = '\0';
}

// FUNCTION: TRILOGY 0x004dbd8b
// Begins a poison application attempt after checking the character's skill and posture.
void __fastcall ApplyPoison(void* unusedThis, int unusedEdx, int itemUniqueId)
{
    char hasPoisonSkill;
    void* spellScreen;

    hasPoisonSkill = EQ_METHOD(1, char, 0x0041A3E9, (void*, unsigned char), EQ_GLOBAL(void*, 0x006E4DA8))(6);
    if (hasPoisonSkill != '\0')
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->stand_state == 'd')
        {
            EQ_METHOD(1, void, 0x004B45CB, (void*, int), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))(0x6e);
        }
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->stand_state != 'n')
        {
            EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x0057061C));
            return;
        }
        if (EQ_GLOBAL(int, 0x006B6478) != 0)
            return;
        spellScreen = EQ_GLOBAL(void*, 0x006E4E44);
        if (spellScreen == NULL)
            return;
        if (EQ_GLOBAL(int, 0x006B6590) != 0)
            return;
        if (*(int*)((char*)spellScreen + 0xE5C) != 0)
            return;
        if (*(int*)((char*)spellScreen + 0x94) != 0)
            return;
        EQ_GLOBAL(int, 0x006B6478) = *(int*)((char*)g_display_guess + 0xBC);
        EQ_GLOBAL(int, 0x006B647C) = itemUniqueId;
        EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005705FC));
        return;
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(EQ_ARRAY(char, 0x005705C8));
}

// FUNCTION: TRILOGY 0x004dbe26
// Resets the spell screen's active state when the character changes stance.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
void DeacSpellScreen(void)
{
    void* spellScreen;

    spellScreen = EQ_GLOBAL(void*, 0x006E4E44);
    if (spellScreen != NULL && *(unsigned char*)((char*)spellScreen + 0xFE0) == 5)
    {
        *(int*)((char*)spellScreen + 0xFE4) = 0;
        pinstEQ_Main_guess->bookAnimStartTime_guess = 0;
        ((void (__fastcall *)(void*))(*(void***)EQ_GLOBAL(void*, 0x006E4E44))[2])
            (EQ_GLOBAL(void*, 0x006E4E44));
        *(unsigned short*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE8) = 0;
        if (*(void**)((char*)EQ_GLOBAL(void*, 0x006E4DA8) + 0xE0) == NULL)
            *(unsigned char*)((char*)g_display_guess + 0x3C) = 0;
        *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE2) = 0;
        pinstEQ_Main_guess->memorizingSpell_guess = -1;
        pinstEQ_Main_guess->scribingSpell_guess = -1;
        pinstEQ_Main_guess->screenMode_u8_guess = 0;
        pinstEQ_Main_guess->clickedButton_guess = -1;
        pinstEQ_Main_guess->pressedButton_guess = -1;
        pinstEQ_Main_guess->releasedButton_guess = -1;
        pinstEQ_Main_guess->uiDirty_guess = 1;
        ((void (__fastcall *)(void*))(*(void***)EQ_GLOBAL(void*, 0x006E4E44))[1])
            (EQ_GLOBAL(void*, 0x006E4E44));
        if (EXEC_SAVED_VIDEO_MODE == 2 && g_game_state_guess != 2)
        {
            g_game_state_guess = 2;
            pinstEQ_Main_guess->uiDirty_guess = 1;
            EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
        }
    }
}

// FUNCTION: TRILOGY 0x004dbf94
// Checks local zone files and asks the world server whether the requested zone is up.
// ORIGINAL FILE: EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: prologue cmp before the reg pushes; EQ_THISCALL loads ECX after the pushes; loop-tail jcc polarity; get_info/sprintf and DbgLog cleanups merged; add eax,2 hoist
/* fake match: a member's this sits in a frame slot; volatile keeps this free-function copy there (a plain copy gets a register) */
unsigned int __fastcall IsZoneAvailable(void* thisPtr, int unusedEdx, char* targetZoneName)
{
    int* obj;
    int value;
    short* replyData;
    int connected;
    unsigned int now;
    unsigned int result;
    unsigned char zoneUp;
    char s3dPath[128];
    char reasonText[128];
    char request[68];
    int disconnectInfo[3];
    int msg;
    int* volatile thisCopy; /* fake match: keeps the this copy in its frame slot; a plain local goes to a register */
    char waiting;

    thisCopy = (int*)thisPtr;
    if (EQ_GLOBAL(void*, 0x006E4DA8) == NULL)
    {
        result = 1;
    }
    else
    {
        value = _strcmpi(targetZoneName, EQ_ARRAY(char, 0x0055C388));
        if (value == 0)
        {
            sprintf(s3dPath, EQ_ARRAY(char, 0x0055EDFC), targetZoneName);
            value = EQ_CDECL(int, 0x004A3D04, (const char*))(s3dPath);
            if (value == 0)
            {
                EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), thisCopy)(EQ_ARRAY(char, 0x005706F0), 0xD);
                return 0;
            }
        }
        strcpy(request, (const char*)EQ_GLOBAL(void*, 0x006E4DA8));
        strcpy(request + 0x20, targetZoneName);
        send_message_guess
            (&EQ_GLOBAL(void*, 0x006B3194), 0x20A3, request, 0x44, 1);
        value = EQ_CDECL(int, 0x004A461E, (void))();
        value += 180000U;
        waiting = '\x01';
        zoneUp = 0;
        do
        {
            msg = EQ_CDECL(int, 0x004E04D1, (void))();
            if (msg != 0)
            {
                replyData = EQ_CDECL(short*, 0x004F4C90, (int))(msg);
                if (*replyData == 0x20A3)
                {
                    zoneUp = *((unsigned char*)replyData + 0x42);
                    waiting = '\0';
                    if (zoneUp == 0)
                    {
                        DbgLog_guess(EQ_ARRAY(char, 0x005706B4), replyData + 1,
                             (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 1,
                             replyData + 0x11, targetZoneName);
                    }
                }
                EQ_CDECL(void, 0x004F4AC0, (int))(msg);
            }
            else
            {
                if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                    connected = EQ_CDECL(int, 0x004F23B0, (void*))(EQ_GLOBAL(void*, 0x006B3194));
                else
                    connected = 0;
                if (connected == 0)
                {
                    if (EQ_GLOBAL(void*, 0x006B3194) != NULL)
                    {
                        EQ_CDECL(void, 0x004F2490, (void*, int*, int))
                            (EQ_GLOBAL(void*, 0x006B3194), disconnectInfo, 0xC);
                        sprintf(reasonText, EQ_ARRAY(char, 0x0056F5DC), disconnectInfo[0]);
                    }
                    else
                    {
                        strcpy(reasonText, EQ_ARRAY(char, 0x0056F5CC));
                    }
                    DbgLog_guess(EQ_ARRAY(char, 0x00570678), EQ_ARRAY(char, 0x0056F5A4), 0x5443,
                         reasonText, targetZoneName, (char*)EQ_GLOBAL(void*, 0x006E4D8C) + 0x1F);
                    obj = thisCopy;
                    obj[0x88FD] = 1;
                    EQ_METHOD(1, void, 0x004D0FE6, (void*, const char*), obj)(EQ_ARRAY(char, 0x0056F574));
                    ((void (__stdcall *)(unsigned long))EQ_GLOBAL(void*, 0x00534198))(3000);
                    *obj = 1;
                    obj[1] = 0xFF;
                    EQ_GLOBAL(int, 0x006B63F4) = 0xCB;
                    return 0xFFFFFFFF;
                }
            }
            now = EQ_CDECL(unsigned int, 0x004A461E, (void))();
            if (now > value)
            {
                DbgLog_guess(EQ_ARRAY(char, 0x00570648));
                break;
            }
        }
        while (waiting != '\0');
        result = (unsigned int)zoneUp;
    }
    return result;
}

// FUNCTION: TRILOGY 0x004dc18b
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name strlen, low, does not fit)
// __cdecl. 1 when the name is NULL or contains an apostrophe, else 0. Only caller: ZonePC.
int NameHasApostrophe_guess(char* name)
{
    int len;
    int i;

    if (name == NULL)
        return 1;
    len = strlen(name);
    for (i = 0; i < len; i++)
    {
        if (name[i] == '\'')
            return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004dc1b5
// Changes zones, using safe coordinates if a requested zone is down.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// ZonePC_guess: __thiscall in the binary (ECX = the CEverQuest, RET 0xC), a __fastcall free function with the
// same layout. Copies the zone name, asks the world server for the zone change and either falls back (home zone
// unavailable -> character select, zone down -> safe coords) or switches the zone.
#pragma intrinsic(memset)
void __fastcall ZonePC_guess(CEverQuest* selfPointer, int edxDummy, char* requestedZoneName, char* logFormat, int refit)
{
    u32 availabilityResult;
    int compareResult;
    char targetZoneName[64];

    strcpy(targetZoneName, requestedZoneName);
    EQ_GLOBAL(int, 0x006B64E8) = 0;
    if (EQ_GLOBAL(int, 0x006E4DD8) != 0)
        EQ_METHOD(1, int, 0x004E36D1, (void*, int), g_sound_manager_ptr_guess)(0);
    if (EQ_GLOBAL(char, 0x006B63F8) == 0 && DAT_006b6494 == 0)
    {
        if (g_local_player_guess == NULL)
            return;
        if (g_local_player_guess->stand_state == 'f')
            return;
        availabilityResult = EQ_METHOD(1, u32, 0x004DBF94, (void*, char*), selfPointer)(targetZoneName);
        if (availabilityResult == 0xFFFFFFFF)
            return;
        if (availabilityResult == 0)
        {
            if (g_pc_guess->unknown0x0076 == 4 || refit != 0 ||
                EQ_CDECL(int, 0x004DC18B, (char*))((char*)g_pc_guess) != 0)
            {
                DbgLog_guess(EQ_ARRAY(char, 0x005707A8), g_pc_guess->zone, g_pc_guess->bind_zone, targetZoneName);
                EQ_CDECL(void, 0x004A07E9, (void*, const char*, int))
                    (g_pc_guess, EQ_ARRAY(char, 0x0057075C), 0xD);
                EQ_GLOBAL(int, 0x006B7120) = 8;
                send_message_guess
                    (g_world_connection_guess, 0x2150, NULL, 0, 1);
                EQ_GLOBAL(char, 0x006B7111) = 1;
                Sleep(1000);
                return;
            }
            DbgLog_guess(EQ_ARRAY(char, 0x00570880), targetZoneName);
            DbgLog_guess(logFormat);
            strcpy(targetZoneName, g_pc_guess->zone);
            g_pc_guess->boat[0] = '\0';
            g_local_player_guess->y = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_y;
            g_pc_guess->y = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_y;
            g_local_player_guess->x = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_x;
            g_pc_guess->x = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_x;
            g_local_player_guess->z = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_z;
            g_pc_guess->z = EQ_GLOBAL(MsgNewZone, 0x006B65DC).safe_z;
            availabilityResult = EQ_METHOD(1, u32, 0x004DBF94, (void*, char*), selfPointer)(targetZoneName);
            if (availabilityResult == 0xFFFFFFFF)
                return;
            EQ_METHOD(0, void, 0x00414E9B, (void*), g_display_guess)();
            if (g_pc_guess->level < 0x2E &&
                (_strcmpi(targetZoneName, EQ_ARRAY(char, 0x0053E210)) == 0 ||
                 _strcmpi(targetZoneName, EQ_ARRAY(char, 0x0055F9E8)) == 0 ||
                 _strcmpi(targetZoneName, EQ_ARRAY(char, 0x00561C44)) == 0))
            {
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), selfPointer)(EQ_ARRAY(char, 0x00570844));
                return;
            }
            EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), selfPointer)(EQ_ARRAY(char, 0x005707F4), 0xD);
            return;
        }
    }
    EQ_METHOD(2, void, 0x004D055E, (void*, const char*, int), selfPointer)(EQ_ARRAY(char, 0x00570744), 0xD);
    EQTRACE_guess(EQ_ARRAY(char, 0x0057072C), targetZoneName);
    EQ_METHOD(0, void, 0x0040C305, (void*), g_display_guess)();
    EQ_GLOBAL(u32, 0x006B6858) = g_local_player_guess->unk_0x60;
    EQ_CDECL(void, 0x004A0841, (char*, const char*, int))(g_pc_guess->zone, targetZoneName, 0x13);
    if (EQ_GLOBAL(char, 0x006B63F8) == 0 && DAT_006b6494 == 0)
    {
        EQ_METHOD(1, void, 0x004C71A3, (void*, int), selfPointer)(refit);
        send_message_guess
            (g_world_connection_guess, 0x202B, &g_local_player_guess->spawn_id, 4, 1);
    }
    else
    {
        EQ_CDECL(void, 0x004A0841, (char*, const char*, int))(EQ_ARRAY(char, 0x00601894), targetZoneName, 0x13);
        EQ_GLOBAL(f32, 0x006B63C4) = g_local_player_guess->y;
        EQ_GLOBAL(f32, 0x006B63C8) = g_local_player_guess->x;
        EQ_GLOBAL(f32, 0x006B63CC) = g_local_player_guess->z;
        EQ_GLOBAL(f32, 0x006B61B0) = g_local_player_guess->heading;
        compareResult = EXEC_STRCMP(targetZoneName, EQ_ARRAY(char, 0x0053E760));
        if (compareResult == 0)
        {
            EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_min_clip[0] = 60.0f;
            EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_max_clip[0] = 400.0f;
        }
        else
        {
            EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_min_clip[0] = 150.0f;
            EQ_GLOBAL(MsgNewZone, 0x006B65DC).fog_max_clip[0] = 390.0f;
        }
    }
    EQ_GLOBAL(char, 0x006B7110) = 1;
    EQ_CDECL(void, 0x004E0632, (void))();
    strcpy(EQ_ARRAY(char, 0x00604904), (const char*)g_pc_guess);
    if (EQ_GLOBAL(char, 0x005FE968) == 0)
        memset(g_death_state_guess, 0, 20);
}
#pragma function(memset)

// FUNCTION: TRILOGY 0x004dc521
// Routes pointer movement to the game UI, including active overlay widgets.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// CEverQuest::MouseMove_guess: __thiscall in the binary (ECX = the CEverQuest, RET 8); `this` is never read.
// everquest.h doesn't declare it, so this is a __fastcall free function with the same ECX/stack layout, as
// EQ_Main_MouseMove_guess. Forwards the mouse move to the char create, the main window (by mode), and the
// active overlay sliders and text IO.
void __fastcall CEverQuest_MouseMove_guess(CEverQuest* selfPointer, int edxDummy, int mouseX, int mouseY)
{
    char overlayMode;
    int activeResult;

    activeResult = EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)();
    if (activeResult == 0)
        return;
    if (EQ_GLOBAL(void*, 0x006E4E70) != NULL)
        EQ_METHOD(2, void, 0x0042C3F3, (void*, int, int), EQ_GLOBAL(void*, 0x006E4E70))(mouseX, mouseY);
    if (EQ_GLOBAL(int, 0x006E4E6C) != 0)
        return;
    if (EQ_GLOBAL(void*, 0x006E4E44) == NULL)
        goto forwardToOverlays;
    if (g_game_state_guess == 1)
    {
        overlayMode = *((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE0);
        if (overlayMode == '\t')
        {
            EQ_METHOD(2, void, 0x00464352, (void*, int, int), EQ_GLOBAL(void*, 0x006E4E44))(mouseX, mouseY);
            goto forwardToOverlays;
        }
        if (overlayMode == '\x06')
        {
            EQ_METHOD(2, void, 0x0046486C, (void*, int, int), EQ_GLOBAL(void*, 0x006E4E44))(mouseX, mouseY);
            EQ_METHOD(2, void, 0x004648F9, (void*, int, int), EQ_GLOBAL(void*, 0x006E4E44))(mouseX, mouseY);
            goto forwardToOverlays;
        }
        if (overlayMode != '\x04' && pinstEQ_Main_guess->trackWindowOpen_guess == 0)
            goto forwardToOverlays;
    }
    EQ_METHOD(2, void, 0x0046418E, (void*, int, int), EQ_GLOBAL(void*, 0x006E4E44))(mouseX, mouseY);
forwardToOverlays:
    if (EQ_GLOBAL(void*, 0x006018C0) != NULL
        && (activeResult = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006018C0))()) != 0)
    {
        EQ_METHOD(2, void, 0x00487523, (void*, int, int), EQ_GLOBAL(void*, 0x006018C0))(mouseX, mouseY);
    }
    if (EQ_GLOBAL(void*, 0x00604984) != NULL
        && (activeResult = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x00604984))()) != 0)
    {
        EQ_METHOD(2, void, 0x00486772, (void*, int, int), EQ_GLOBAL(void*, 0x00604984))(mouseX, mouseY);
    }
    if (EQ_GLOBAL(void*, 0x006E4DE8) != NULL
        && (activeResult = EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DE8))()) != 0)
    {
        EQ_METHOD(2, void, 0x0048B1E3, (void*, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(mouseX, mouseY);
    }
    return;
}

// FUNCTION: TRILOGY 0x004dc615
// Routes left-button releases through game windows; if unhandled, the 3D view receives a press.
// __thiscall (ECX = the CEverQuest, RET 8); `this` is never read, so this is a __fastcall free function with the same
// ECX/stack layout, as CEverQuest_MouseMove_guess. Name is a guess (left mouse-up). Sends the mouse-up to the overlays
// in a fixed order (each with its flag set first) and stops at the first one that takes it. If none does and the
// 3D overlay (0x006E4DEC) is active, the 3D view gets a mouse-down. With 0x0058FC6C == 1, 0x006E4E60 is tried after 0x006E4E5C.
// @early-stop: Stack-argument EQ_THISCALL adapters still clear EDX; push order and some branch layouts differ.
/* fake match: volatile mouseY keeps it stack-backed so VC6 holds mouseX in EDI. */
void __fastcall CEverQuest_LMouseUp_guess(CEverQuest* selfPointer, int edxDummy, int mouseX, volatile int mouseY)
{
    if (EQ_GLOBAL(void*, 0x006E4E70) != NULL)
    {
        pinstEQ_CharCreateS_guess->hoverOnly = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E70))(0, mouseX, mouseY) != 0)
            return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E6C) != NULL)
    {
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4DF0) + 0xA0) = 1;
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E6C) + 0x88) = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DF0))(0, mouseX, mouseY) != 0)
            return;
        EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E6C))(0, mouseX, mouseY);
        return;
    }
    if (EQ_GLOBAL(char, 0x006B645A) != '\0')
    {
        EQ_GLOBAL(char, 0x006B6408) = 1;
        return;
    }
    if (EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() == 0)
        return;
    if (g_game_state_guess == 3)
        return;
    if (EQ_GLOBAL(void*, 0x006E4E44) == NULL)
        return;
    if (pinstEQ_Main_guess->pendingWorldRequests_guess != 0)
        return;
    if (*((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xFE0) >= 8)
    {
        pinstEQ_Main_guess->editLayout_guess = 1;
        EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(0, mouseX, mouseY);
        return;
    }
    if (g_game_state_guess != 1)
    {
        if (EQ_GLOBAL(void*, 0x006E4E20) != NULL && *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x10) != 0)
        {
            pinstEQ_Requestor_guess->captureButtonResult_guess = 1;
            if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E20))(0, mouseX, mouseY) != 0)
                return;
        }
        if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4D84) + 0x10) != 0)
            return;
        if (*(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x10) != 0)
        {
            *(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x1C8) = 1;
            EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006018C0))(0, mouseX, mouseY);
            return;
        }
        if (*(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x10) != 0)
        {
            *(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x1C8) = 1;
            EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x00604984))(0, mouseX, mouseY);
            return;
        }
        if (EQ_GLOBAL(void*, 0x006E4E58) != NULL
            && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0)
        {
            pinstEQ_Bag1_guess->mouseDown_guess = 1;
            if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E58))(0, mouseX, mouseY) != 0)
                return;
        }
        if (EQ_GLOBAL(void*, 0x006E4E5C) != NULL
            && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0)
        {
            pinstEQ_Bag2_guess->mouseDown_guess = 1;
            if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E5C))(0, mouseX, mouseY) != 0)
                return;
        }
        if (pinstEQ_Main_guess->socialEditorOpen_guess == 0)
        {
            pinstEQ_TextIO_guess->mouseDispatchActive_guess = 1;
            if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(0, mouseX, mouseY) != 0)
                return;
        }
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E44))() != 0)
        {
            pinstEQ_Main_guess->editLayout_guess = 1;
            if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(0, mouseX, mouseY) != 0)
                return;
        }
        if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DEC))() == 0)
            return;
        EQ_METHOD(3, void, 0x00415EE1, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(0, mouseX, mouseY);
        return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E20) != NULL && *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E20) + 0x10) != 0)
    {
        pinstEQ_Requestor_guess->captureButtonResult_guess = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E20))(0, mouseX, mouseY) != 0)
            return;
    }
    if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4D84) + 0x10) != 0)
        return;
    if (*(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x10) != 0)
    {
        *(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x1C8) = 1;
        EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006018C0))(0, mouseX, mouseY);
        return;
    }
    if (*(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x10) != 0)
    {
        *(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x1C8) = 1;
        EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x00604984))(0, mouseX, mouseY);
        return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E58) != NULL
        && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0)
    {
        pinstEQ_Bag1_guess->mouseDown_guess = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E58))(0, mouseX, mouseY) != 0)
            return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E5C) != NULL
        && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0)
    {
        pinstEQ_Bag2_guess->mouseDown_guess = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E5C))(0, mouseX, mouseY) != 0)
            return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E60) != NULL && *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E60) + 0x10) != 0)
    {
        *(int*)((char*)EQ_GLOBAL(void*, 0x006E4E60) + 0x90) = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E60))(0, mouseX, mouseY) != 0)
            return;
    }
    if (pinstEQ_Main_guess->socialEditorOpen_guess == 0)
    {
        pinstEQ_TextIO_guess->mouseDispatchActive_guess = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(0, mouseX, mouseY) != 0)
            return;
    }
    if (EQ_GLOBAL(void*, 0x006E4E44) != NULL
        && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E44))() != 0)
    {
        pinstEQ_Main_guess->editLayout_guess = 1;
        if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(0, mouseX, mouseY) != 0)
            return;
    }
    if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DEC))() == 0)
        return;
    EQ_METHOD(3, void, 0x00415EE1, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(0, mouseX, mouseY);
}

// FUNCTION: TRILOGY 0x004dc9e6
// Routes right-button presses through active windows and into the 3D view when unhandled.
// ORIGINAL FILE: Source/PC Source/EverQuest/EverQuest.cpp (from Mac debug map)
// @early-stop: VC6 keeps mouseX in EBX and mouseY in EDI, the original has them swapped (5 push-order pairs and 2 loads differ); EDX dummies on the EQ_THISCALL calls are folded
void __stdcall RMouseDown(int mouseX, int mouseY)
{
    if (EQ_GLOBAL(int, 0x006E4E6C) == 0 && EQ_GLOBAL(int, 0x006E4E70) == 0
        && EQ_METHOD(0, int, 0x00408E7F, (void*), g_display_guess)() != 0
        && EQ_GLOBAL(char, 0x006B645A) == '\0')
    {
        if (g_game_state_guess != 3)
        {
            if (EQ_GLOBAL(void*, 0x006E4E44) == NULL)
                return;

            pinstEQ_Main_guess->editLayout_guess = 1;
            pinstEQ_Bag1_guess->mouseDown_guess = 1;
            pinstEQ_Bag2_guess->mouseDown_guess = 1;

            if (pinstEQ_Main_guess->screenMode_guess >= 8)
            {
                EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(1, mouseX, mouseY);
                return;
            }

            if (*(int*)((char*)EQ_GLOBAL(void*, 0x006E4D84) + 0x10) != 0)
                return;
            if (*(int*)((char*)EQ_GLOBAL(void*, 0x006018C0) + 0x10) != 0)
                return;
            if (*(int*)((char*)EQ_GLOBAL(void*, 0x00604984) + 0x10) != 0)
                return;

            if (EQ_GLOBAL(void*, 0x006E4E58) != NULL
                && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E58))() != 0)
            {
                pinstEQ_Bag1_guess->mouseDown_guess = 1;
                if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E58))(1, mouseX, mouseY) != 0)
                    return;
            }

            if (EQ_GLOBAL(void*, 0x006E4E5C) != NULL
                && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E5C))() != 0)
            {
                pinstEQ_Bag2_guess->mouseDown_guess = 1;
                if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E5C))(1, mouseX, mouseY) != 0)
                    return;
            }

            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DE8))() != 0)
            {
                pinstEQ_TextIO_guess->mouseDispatchActive_guess = 1;
                if (EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DE8))(1, mouseX, mouseY) != 0)
                    return;
            }

            if (EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4E44))() != 0
                && EQ_METHOD(3, int, 0x004501D9, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4E44))(1, mouseX, mouseY) != 0)
                return;

            if (pinstEQ_Main_guess->screenMode_guess < 5
                && EQ_METHOD(0, int, 0x004504AC, (void*), EQ_GLOBAL(void*, 0x006E4DEC))() != 0
                && EQ_METHOD(3, int, 0x00415EE1, (void*, int, int, int), EQ_GLOBAL(void*, 0x006E4DEC))(1, mouseX, mouseY) != 0)
                return;

            g_cdisplay_guess->dragArmed_guess = 0;
        }
        EQ_GLOBAL(char, 0x006B645A) = EQ_GLOBAL(char, 0x006B6409);
    }
    return;
}

// FUNCTION: TRILOGY 0x004dd0c2
// Credits a pending coin split, then moves the next pending item (with its bag contents) onto the cursor
// @early-stop: entry/coin scan register choices and bag-slot arithmetic still differ.
void pending_split_and_cursor_guess(void)
{
    typedef void (__fastcall *VtblSlot2Fn_guess)(void *);
    int hasCoins;
    int *pSlot;
    int *coinScan;
    int i;
    int n;
    int bagSlot;
    int equipSlot;
    CharacterProfile *pc;
    EQ_Item *cursor;
    EQ_Item *bagEntry;
    EQ_Item *pendingItem;
    int split[4];
    char coinText[128];
    char chatText[512];

    hasCoins = 0;
    if (pinstEQ_Main_guess->pendingWorldRequests_guess <= 0 && g_pc_guess != NULL)
    {
        pSlot = EQ_ARRAY(int, 0x006B6844);
        coinScan = pSlot;
        do
        {
            if (0 < *coinScan)
                hasCoins = 1;
            coinScan = coinScan + 1;
        } while ((int)coinScan < 0x006B6854);

        if (hasCoins)
        {
            if (EQ_GLOBAL(void *, 0x006018C0) != NULL
                && *(int *)((char *)EQ_GLOBAL(void *, 0x006018C0) + 0x10) != 0)
            {
                ((VtblSlot2Fn_guess)*(int *)(*(int *)EQ_GLOBAL(void *, 0x006018C0) + 8))(
                    EQ_GLOBAL(void *, 0x006018C0));
            }
            if (EQ_GLOBAL(void *, 0x00604984) != NULL
                && *(int *)((char *)EQ_GLOBAL(void *, 0x00604984) + 0x10) != 0)
            {
                *(int *)((char *)EQ_GLOBAL(void *, 0x00604984) + 0x1E8) = 1;
                ((VtblSlot2Fn_guess)*(int *)(*(int *)EQ_GLOBAL(void *, 0x00604984) + 8))(
                    EQ_GLOBAL(void *, 0x00604984));
            }

            i = 0;
            pc = g_pc_guess;
            do
            {
                switch (i)
                {
                case 0:
                    pc->platinum = pc->platinum + EQ_GLOBAL(int, 0x006B6844);
                    split[0] = EQ_GLOBAL(int, 0x006B6844);
                    break;
                case 1:
                    pc->gold = pc->gold + EQ_GLOBAL(int, 0x006B6848);
                    split[1] = EQ_GLOBAL(int, 0x006B6848);
                    break;
                case 2:
                    pc->silver = pc->silver + EQ_GLOBAL(int, 0x006B684C);
                    split[2] = EQ_GLOBAL(int, 0x006B684C);
                    break;
                case 3:
                    pc->copper = pc->copper + EQ_GLOBAL(int, 0x006B6850);
                    split[3] = EQ_GLOBAL(int, 0x006B6850);
                    break;
                }
                pc = g_pc_guess;
                i = i + 1;
            } while (i < 4);

            send_message_guess(
                g_world_connection_guess, 0x219A, split, 0x10, 1);
            if (g_sound_manager_ptr_guess != NULL)
                EQ_METHOD(2, void, 0x004E34FA, (void *, int, int), g_sound_manager_ptr_guess)(0x8A, 0);

            coinText[0] = 0;
            EQ_CDECL(void, 0x004A8610, (int, int, int, int, char *))(
                EQ_GLOBAL(int, 0x006B6844), EQ_GLOBAL(int, 0x006B6848),
                EQ_GLOBAL(int, 0x006B684C), EQ_GLOBAL(int, 0x006B6850), coinText);
            sprintf(
                chatText, EQ_ARRAY(char, 0x0055B658), coinText);
            pinstCEverQuest_guess->dsp_chat_guess(chatText, 2);
            pinstEQ_Main_guess->uiDirty_guess = 1;
            for (i = 0; i < 4; i++)
                pSlot[i] = 0;
        }

        if (g_pc_guess->inventory_items[0] == NULL
            && pinstEQ_Main_guess->cursorSpell_guess == 0
            && *(char *)(g_displayManager_guess + 0x3D) == 0
            && g_cdisplay_guess->dragArmed_guess == 0)
        {
            i = 0;
            pSlot = EQ_ARRAY(int, 0x006B67B4);
            do
            {
                pendingItem = (EQ_Item *)*pSlot;
                if (pendingItem != NULL && pendingItem->equip_slot < 30000)
                {
                    if (EQ_GLOBAL(void *, 0x006018C0) != NULL
                        && *(int *)((char *)EQ_GLOBAL(void *, 0x006018C0) + 0x10) != 0)
                    {
                        ((VtblSlot2Fn_guess)*(int *)(*(int *)EQ_GLOBAL(void *, 0x006018C0) + 8))(
                            EQ_GLOBAL(void *, 0x006018C0));
                    }
                    if (EQ_GLOBAL(void *, 0x00604984) != NULL
                        && *(int *)((char *)EQ_GLOBAL(void *, 0x00604984) + 0x10) != 0)
                    {
                        *(int *)((char *)EQ_GLOBAL(void *, 0x00604984) + 0x1E8) = 1;
                        ((VtblSlot2Fn_guess)*(int *)(*(int *)EQ_GLOBAL(void *, 0x00604984) + 8))(
                            EQ_GLOBAL(void *, 0x00604984));
                    }

                    g_pc_guess->inventory_items[0] = (EQ_Item *)EQ_ARRAY(int, 0x006B67B4)[i];
                    g_cdisplay_guess->itemOnCursor_guess = 1;
                    {
                        int moveMsg[3];
                        moveMsg[0] =
                            (int)*(short *)(EQ_ARRAY(int, 0x006B67B4)[i] + 0x86);
                        moveMsg[1] = 0;
                        moveMsg[2] = 0;
                        send_message_guess(
                            g_world_connection_guess, 0x212C, moveMsg, 0xC, 1);
                    }

                    cursor = g_pc_guess->inventory_items[0];
                    if (cursor->type == EQ_ITEM_CONTAINER)
                    {
                        equipSlot = cursor->equip_slot - 10000;
                        n = 0;
                        if (cursor[1].lore[6] != 0)
                        {
                            bagEntry = cursor + 1;
                            do
                            {
                                *(int *)bagEntry->name = 0;
                                n = n + 1;
                                bagEntry = (EQ_Item *)(bagEntry->name + 4);
                            } while (n < (int)(unsigned int)(unsigned char)cursor[1].lore[6]);
                        }

                        int *pScan = EQ_ARRAY(int, 0x006B67B4);
                        do
                        {
                            n = *pScan;
                            if (n != 0)
                            {
                                short itemEquip = *(short *)(n + 0x86);
                                if (itemEquip >= 30000)
                                {
                                    bagSlot = itemEquip - 30000;
                                    if (equipSlot == bagSlot % 100)
                                    {
                                        bagSlot = bagSlot / 100;
                                        if (bagSlot < (int)(unsigned int)(unsigned char)cursor[1].lore[6])
                                        {
                                            *(int *)(cursor[1].name + bagSlot * 4) = n;
                                            *(short *)(n + 0x86) = (short)(bagSlot + 0x14A);
                                            g_pc_guess->bag_inventory[bagSlot] =
                                                *(short *)(*(int *)(cursor[1].name + bagSlot * 4) + 0x82);
                                        }
                                        *pScan = 0;
                                    }
                                }
                            }
                            pScan = pScan + 1;
                        } while ((int)pScan < 0x006B6844);
                    }

                    *pSlot = 0;
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                    return;
                }
                pSlot = pSlot + 1;
                i = i + 1;
            } while ((int)pSlot < 0x006B6844);
        }
    }
    return;
}

// The `new` of loadSounds_guess: operator new(0x5C), then the emitter constructor 0x004BD259 on the block. The
// real constructor is defined earlier in this file, which lets VC6 drop the EH frame around a `new`; the
// original (constructor in another file) keeps it. This stand-in only declares the constructor (the client's),
// so VC6 keeps the frame (can't prove the constructor doesn't throw).
class LoadSoundsEmitter_guess
{
public:
    // EXTERNAL: TRILOGY 0x004BD259 LoadSoundsEmitter_guess::LoadSoundsEmitter_guess
    LoadSoundsEmitter_guess(const void* record);

    u8 bytes[0x5C];
};

// FUNCTION: TRILOGY 0x004dd666
// Loads positional sound emitters for the player's current zone.
// __cdecl (ECX unused), no result (the caller, DoMainLoop 0x004D5D0F, ignores EAX). Needs the sound manager
// (0x006E4DD8): stops its waves, announces the local player's zone, then reads "<profile zone>_sounds.eff" in
// 0x54-byte records and makes an emitter of each (constructor 0x004BD259). Every emitter gets a radius of at
// least 50; one whose byte 0x38 is 1 also gets minimum fade times (the first against 0x00569B4C, the third 1000)
// and, when its radius reaches the limit at 0x00569B50, the value at 0x00569B48 as first fade time. Afterwards
// the environment and midi objects are stopped and, without the flag 0x006EFE1C, the sound manager restarts.
// The `radius < 10.0f` raise can never fire (the 50.0f raise above it leaves at least 50), but the original has
// it. The limit test is written as a negated `<`: in the match build (symbols for the globals) that gives the
// original's FLD radius / FCOMP limit / SAHF / JC, so an unordered (NaN) radius or limit skips the store. The
// test and hook builds (absolute addresses) compile every form of this test tried (>=, <=, >, !<, double casts)
// so that a NaN radius or limit does store; the fixture has no NaN at this test. The two float constants are
// read from the binary (0x535190 = 50.0f, 0x535124 = 10.0f) with the constant on the left of the compare: that
// makes VC6 load the constant first, as the original does, and the store reuses the loaded value. The read loop
// is a `while (1)` that tests the count at its head: a `while` or `for` on the count gets VC6's rotated form.
// @early-stop: loop-head compare is cmp eax,edi (original: test eax,eax); the `new` calls a stand-in ctor, not 0x004BD259
void loadSounds_guess(void)
{
    char fileName[128];
    char record[0x5C];
    FILE* file;
    int count;
    EQSoundEmitter_guess* emitter;

    if (g_sound_manager_ptr_guess != NULL)
    {
        EQ_METHOD(1, int, 0x004E36D1, (void*, int), g_sound_manager_ptr_guess)(0);
        EQ_METHOD(2, void, 0x004E387D, (void*, int, char*), g_sound_manager_ptr_guess)(0, g_local_player_guess->zone);
        if (g_pc_guess == NULL)
        {
            EQTRACE_guess((char*)0x00570904);
        }
        else
        {
            strcpy(fileName, g_pc_guess->zone);
            strcat(fileName, (char*)0x005708F8);
            file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (char*)0x0053C2BC);
            if (file != NULL)
            {
                EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
                count = EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(record, 0x54, 1, file);
                while (1)
                {
                    if (count == 0)
                        break;
                    emitter = (EQSoundEmitter_guess*)new LoadSoundsEmitter_guess(record);
                    if (EQ_GLOBAL(float, 0x00535190) > emitter->radius)
                        emitter->radius = EQ_GLOBAL(float, 0x00535190);
                    if (emitter->unknown0x38 == 1)
                    {
                        if ((u32)emitter->unknown0x44[0] < EQ_GLOBAL(u32, 0x00569B4C))
                            emitter->unknown0x44[0] = EQ_GLOBAL(u32, 0x00569B4C);
                        if ((u32)emitter->unknown0x44[2] < 1000)
                            emitter->unknown0x44[2] = 1000;
                        if (EQ_GLOBAL(float, 0x00535124) > emitter->radius)
                            emitter->radius = EQ_GLOBAL(float, 0x00535124);
                        if (!(emitter->radius < EQ_GLOBAL(float, 0x00569B50)))
                            emitter->unknown0x44[0] = EQ_GLOBAL(u32, 0x00569B48);
                    }
                    count = EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(record, 0x54, 1, file);
                }
                EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
                if (g_environment_ptr_guess != NULL)
                    EQ_METHOD(0, void, 0x004E25FD, (void*), g_environment_ptr_guess)();
                if (EQ_GLOBAL(void*, 0x006ECDF4) != NULL)
                    EQ_METHOD(0, void, 0x004E4AA5, (void*), EQ_GLOBAL(void*, 0x006ECDF4))();
                if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && g_sound_manager_ptr_guess != NULL)
                    EQ_METHOD(0, void, 0x004E3297, (void*), g_sound_manager_ptr_guess)();
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004e1e81
// Prepares the sound manager for game audio playback.
// __fastcall, soundManager is the EqSoundManager object; returns it. Sets the -1 dwords and zeroes the floats and
// flags, copies the string at 0x00579398 ("None") into the three name fields, then clears the 11 slots (the
// first 8 also clear players[i] and player_data_a/b[i]) with handle = -1.
int __fastcall InitializeSoundManager_guess(int soundManager)
{
    EqSoundManager_guess* mgr;
    int soundSlotIndex;

    *(unsigned int*)(soundManager + 0xc) = 0xffffffff;
    *(unsigned int*)(soundManager + 0x10) = 0xffffffff;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0EC = 0.0f;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0E8 = 0.0f;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0E4 = 0.0f;
    *(unsigned int*)(soundManager + 0xf0) = 0;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0E0 = 0.0f;
    *(unsigned int*)(soundManager + 0xf4) = 0;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0F8 = 0;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x008 = 0.0f;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->unk_0x0FC = 0;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->isNight_guess = 0;
    ((EqSoundObj_006E4DDC_guess*)soundManager)->previousNight_guess = 0;
    *(unsigned int*)(soundManager + 0xd4) = 0;
    *(unsigned int*)(soundManager + 0xd8) = 0;
    *(unsigned int*)(soundManager + 0xdc) = 0;
    *(unsigned int*)(soundManager + 0x104) = 0;
    *(unsigned int*)(soundManager + 0x108) = 0;
    strcpy((char*)(soundManager + 0x14), (const char*)0x00579398);
    strcpy((char*)(soundManager + 0x54), (const char*)0x00579398);
    strcpy((char*)(soundManager + 0x94), (const char*)0x00579398);
    mgr = (EqSoundManager_guess*)soundManager;
    soundSlotIndex = 0;
    do
    {
        if (soundSlotIndex < 8)
        {
            mgr->players[soundSlotIndex] = 0;
            mgr->player_data_a[soundSlotIndex] = 0;
            mgr->player_data_b[soundSlotIndex] = 0;
        }
        mgr->slots[soundSlotIndex].x = 0.0f;
        mgr->slots[soundSlotIndex].y = 0.0f;
        mgr->slots[soundSlotIndex].z = 0.0f;
        mgr->slots[soundSlotIndex].count = 0;
        mgr->slots[soundSlotIndex].unknown0x10[0] = 0;
        mgr->slots[soundSlotIndex].unknown0x10[1] = 0;
        mgr->slots[soundSlotIndex].unknown0x10[2] = 0;
        mgr->slots[soundSlotIndex].sound_id_guess = 0;
        mgr->slots[soundSlotIndex].player = 0;
        mgr->slots[soundSlotIndex].unknown0x24 = 0;
        mgr->slots[soundSlotIndex].handle = -1;
        soundSlotIndex = soundSlotIndex + 1;
    } while (soundSlotIndex < 0xb);
    return soundManager;
}

// Layout guessed from 0x004E313F's stores only (no header defines it; size at least 0x104A).
struct EQBlock4E313F_guess
{
    unsigned char f0;           // 0x0
    unsigned char pad1[3];
    int f4;                     // 0x4
    int f8;                     // 0x8
    int fc;                     // 0xc
    int f10;                    // 0x10
    int f14;                    // 0x14
    unsigned char pad18[0x460];
    unsigned char table[0xb40]; // 0x478
    int idsFB8[20];             // 0xfb8
    int ids1008[8];             // 0x1008
    int ids1028[4];             // 0x1028
    int ids1038[4];             // 0x1038
    unsigned char f1048;        // 0x1048
    unsigned char f1049;        // 0x1049
};

// FUNCTION: TRILOGY 0x004e313f
// Initializes a client state block whose in-game purpose is not yet known.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __thiscall (ECX = this, from CEverQuest::CEverQuest_guess). Zeroes the two flags at +0x1048, the dwords
// +4..+0x14 and the 0xB40-byte table at +0x478, sets the 36 dwords at +0xFB8..+0x1047 to -1 (in the
// order +0x1008, +0x1038, +0x1028, +0xFB8), then clears the byte at +0. Returns this.
EQBlock4E313F_guess* __fastcall InitializeStateBlock_guess(EQBlock4E313F_guess* p)
{
    int i;

    p->f1048 = 0;
    p->f1049 = 0;
    p->fc = 0;
    p->f8 = 0;
    p->f4 = 0;
    p->f14 = 0;
    p->f10 = 0;
    memset(p->table, 0, 0xb40);
    for (i = 0; i < 8; i++)
        p->ids1008[i] = -1;
    for (i = 0; i < 4; i++)
        p->ids1038[i] = -1;
    for (i = 0; i < 4; i++)
        p->ids1028[i] = -1;
    for (i = 0; i < 20; i++)
        p->idsFB8[i] = -1;
    p->f0 = 0;
    return p;
}

// FUNCTION: TRILOGY 0x004e31b1
// Configures the client's sound levels from the active audio device.
// @early-stop: the 0x58 lea is hoisted above the store and the dummy EDX argument adds one instruction (VC6 rejects __thiscall on free function pointers)
/* fake match: the EDX slot of the two thiscall vtable calls holds what the original had in EDX (the out pointer for slot 0x58, the vtable for slot 0x60); both callees are thiscall and ignore it. */
void __fastcall ConfigureSound_guess(u8* soundManager)
{
    struct
    {
        int local_10;
        u8 local_c[4];
    } output;
    int value;

    if (EQ_GLOBAL(void*, 0x006E4DD0) != 0)
    {
        EQ_GLOBAL(int, 0x00579314) = (int)
            (((float (__fastcall *)(void*))(*EQ_GLOBAL(void***, 0x006E4DD0))[0x54 / 4])(EQ_GLOBAL(void*, 0x006E4DD0))
             * EQ_GLOBAL(float, 0x0053515C));
        value = EQ_GLOBAL(int, 0x00579314) - 10;
        EQ_GLOBAL(int, 0x00579318) = (int)((double)value * EQ_GLOBAL(double, 0x005357A8));
        ((void (__fastcall *)(void*, int*, int*))(*EQ_GLOBAL(void***, 0x006E4DD0))[0x58 / 4])
            (EQ_GLOBAL(void*, 0x006E4DD0), &output.local_10, &output.local_10);
        *soundManager = output.local_c[1];
        if (output.local_10 < 4 || output.local_c[0] == 0)
        {
            EQ_GLOBAL(int, 0x006B6894) = 6;
            EQ_GLOBAL(int, 0x0057931C) = 6;
            ((void (__fastcall *)(void*, void**, int))(*EQ_GLOBAL(void***, 0x006E4DD0))[0x60 / 4])
                (EQ_GLOBAL(void*, 0x006E4DD0), *EQ_GLOBAL(void***, 0x006E4DD0), 6);
        }
        else if (output.local_10 > 0x14)
        {
            EQ_GLOBAL(int, 0x006B6894) = 0x14;
            EQ_GLOBAL(int, 0x0057931C) = 0x14;
        }
        else
        {
            EQ_GLOBAL(int, 0x006B6894) = output.local_10;
            EQ_GLOBAL(int, 0x0057931C) = output.local_10;
        }
        EQ_GLOBAL(int, 0x006B688C) = (int)
            ((double)EQ_GLOBAL(int, 0x00579318) * EQ_GLOBAL(double, 0x005357A0));
        EQ_GLOBAL(u8, 0x00579324) = 1;
        EQ_GLOBAL(int, 0x006B6890) = EQ_GLOBAL(int, 0x00579314) / 5;
        if (EQ_GLOBAL(int, 0x006ECE28) == 0)
        {
            EQ_GLOBAL(int, 0x00579318) = 0;
            EQ_GLOBAL(int, 0x006B688C) = 0;
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004e3354
// Changes how many sounds can play at once.
void __stdcall SetSoundBufferCount_guess(int bufferCount, int printStatus)
{
    // Pads the vtable so the two calls below land on +0x58 and +0x60.
    struct SoundSettingsControl_guess : EqSoundControl_guess
    {
        virtual void vf30() {}
        virtual void vf34() {}
        virtual void vf38() {}
        virtual void vf3c() {}
        virtual void vf40() {}
        virtual void vf44() {}
        virtual void vf48() {}
        virtual void vf4c() {}
        virtual void vf50() {}
        virtual void vf54() {}
        virtual void GetSettings_guess(int* settings) {}
        virtual void vf5c() {}
        virtual void SetSimultaneousBuffers_guess(int count) {}
    };

    int i;
    int requested;
    char* src;
    u32* dst;
    char message[80];
    u32 cardVoiceText[5];
    char ambientsText[13];
    char reverbText[11];
    char cardReverbText[10];
    struct
    {
        int hw3dBuffers;
        char hasVoiceMgmt;
        char hasReverb;
        char unused[2];
    } output;
    void* soundControl;
    void* everquest;

    soundControl = g_sound_control_guess;
    if (soundControl != 0 && g_sound_manager_guess != 0 &&
        g_environment_ptr_guess != 0)
    {
        requested = bufferCount;
        if (bufferCount < 1)
            requested = 1;
        ((SoundSettingsControl_guess*)soundControl)->GetSettings_guess(&output.hw3dBuffers);
        // At the cap (20), use the card's HW3D count when it is higher.
        if (requested == 0x14 && output.hw3dBuffers > requested)
            bufferCount = output.hw3dBuffers;
        else
            bufferCount = requested;
        soundControl = g_sound_control_guess;
        ((SoundSettingsControl_guess*)soundControl)->SetSimultaneousBuffers_guess(bufferCount);
        EQ_METHOD(0, void, 0x004E25FD, (EqSoundManager_guess*), EQ_GLOBAL(EqSoundManager_guess*, 0x006E4DDC))();
        if (printStatus != 0)
        {
            everquest = pinstCEverQuest_guess;
            if (everquest != 0 && *(int*)((char*)everquest + 4) == 5)
            {
                i = 5;
                src = EQ_ARRAY(char, 0x0057BE6C);
                for (i = 0; i < 13; ++i)
                    ambientsText[i] = src[i];
                src = EQ_ARRAY(char, 0x0057BE60);
                for (i = 0; i < 11; ++i)
                    reverbText[i] = src[i];
                src = EQ_ARRAY(char, 0x0057BE54);
                for (i = 0; i < 10; ++i)
                    cardReverbText[i] = src[i];
                i = 5;
                src = EQ_ARRAY(char, 0x0057BE40);
                dst = cardVoiceText;
                for (; i != 0; i = i + -1)
                {
                    *dst = *(u32*)src;
                    src = src + 4;
                    dst = dst + 1;
                }
                if (6 < requested)
                    sprintf(ambientsText, EQ_ARRAY(char, 0x0057BE34));
                if (*g_reverb_on_guess != '\0')
                    sprintf(reverbText, EQ_ARRAY(char, 0x0057BE28));
                if (output.hasReverb != '\0')
                    sprintf(cardReverbText, EQ_ARRAY(char, 0x0057BE20));
                if (output.hasVoiceMgmt != '\0')
                    sprintf((char*)cardVoiceText, EQ_ARRAY(char, 0x0057BE10));
                sprintf(message, EQ_ARRAY(char, 0x0057BDEC), bufferCount, ambientsText, reverbText);
                pinstCEverQuest_guess->print_chat_guess(message);
                sprintf(message, EQ_ARRAY(char, 0x0057BDC4), output.hw3dBuffers, cardReverbText, cardVoiceText);
                pinstCEverQuest_guess->print_chat_guess(message);
                if (g_channels_reversed_guess == 1)
                    pinstCEverQuest_guess->print_chat_guess(
                        EQ_ARRAY(char, 0x0057BD98));
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004e36d1
// Stops active game audio as part of a full sound reset.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __thiscall (callers load ECX from 0x006E4DD8), `this` unused. With argument 0 and the COM-style object at
// 0x006E4DD0 present, calls its virtual method at +0x28 with -1. Returns 0.
int EqSoundManager_guess::StopAllWaves_guess(int keep)
{
    if (g_sound_control_guess != NULL && keep == 0)
        g_sound_control_guess->StopAll_guess(-1);
    return 0;
}

#define g_voice_dictation_guess EQ_GLOBAL(void *, 0x006ECE4C)
#define g_command_recognition_active_guess EQ_GLOBAL(int, 0x006B70E8)
#define CEQ_CHAT_INPUT(eq) ((char *)(eq) + 0x0BA8)

// FUNCTION: TRILOGY 0x004e80d3
// Turns recognized speech into game commands or chat text.
int __stdcall CommandRecognize(void *callbackContext, u32 commandId, void *unusedArgument3,
    void *unusedArgument4, void *unusedArgument5, void *unusedArgument6, void *unusedArgument7,
    void *unusedArgument8, char *recognizedText)
{
    char commandBuffer[256];
    char *chatCommand;

    EQTRACE_guess((char *)0x0057cfc0, commandId, recognizedText);
    if (g_voice_dictation_guess == NULL)
    {
        EQTRACE_guess((char *)0x0057cf90);
    }
    else
    {
        if (pinstCEverQuest_guess != NULL)
        {
            if (commandId < 0x7f)
            {
                if (commandId == 0x7e)
                {
                    if (g_chatInputInactive_guess == '\0')
                        return 0;
                    chatCommand = (char *)0x0057cf44;
                }
                else
                {
                    if (commandId == 0x7b)
                    {
                        EQ_GLOBAL(struct EQPlayer *, 0x006E4DAC) = g_local_player_guess;
                        commandId = 0x16;
                        EQ_CDECL(void, 0x004D7D6A, (u32, int, int))(commandId, 1, 0);
                        return 0;
                    }
                    if (commandId == 0x7c)
                    {
                        if (g_chatInputInactive_guess == '\0')
                            return 0;
                        chatCommand = (char *)0x0057cf4c;
                    }
                    else
                    {
                        if (commandId != 0x7d)
                        {
                            EQ_CDECL(void, 0x004D7D6A, (u32, int, int))(commandId, 1, 0);
                            return 0;
                        }
                        if (g_chatInputInactive_guess == '\0')
                            return 0;
                        chatCommand = (char *)0x0057cf54;
                    }
                }
            }
            else
            {
                if (commandId != 0x7f)
                {
                    if (commandId == 0xca)
                    {
                        sprintf(
                            commandBuffer, (char *)0x0057cf40, recognizedText);
                        EQ_METHOD(2, void, 0x004DB9B5,
                            (CEverQuest *, struct EQPlayer *, char *), pinstCEverQuest_guess)(
                                g_local_player_guess, commandBuffer);
                        return 0;
                    }
                    if (0x212 < commandId && commandId < 0x219)
                    {
                        commandId -= 500;
                        if (g_command_recognition_active_guess != 1)
                        {
                            g_command_recognition_active_guess = 1;
                            EQ_CDECL(void, 0x004D7D6A, (u32, int, int))(commandId, 1, 0);
                            g_command_recognition_active_guess = 0;
                            return 0;
                        }
                        EQ_CDECL(void, 0x004D7D6A, (u32, int, int))(commandId, 1, 0);
                        return 0;
                    }
                    EQ_CDECL(void, 0x004D7D6A, (u32, int, int))(commandId, 1, 0);
                    return 0;
                }
                if (g_chatInputInactive_guess == '\0')
                    return 0;
                chatCommand = (char *)0x0057cf34;
            }
            strcpy(
                CEQ_CHAT_INPUT(pinstCEverQuest_guess), chatCommand);
            g_chatInputInactive_guess = '\0';
            EQ_METHOD(1, void, 0x004CF7A1, (CEverQuest *, int), pinstCEverQuest_guess)(1000000);
            return 0;
        }
        EQTRACE_guess((char *)0x0057cf60);
    }
    return 0;
}

#undef CEQ_CHAT_INPUT
#undef g_command_recognition_active_guess
#undef g_voice_dictation_guess

// FUNCTION: TRILOGY 0x004e878c
// Clears remembered modifier keys and key-binding mode.
// ORIGINAL FILE: unknown (subsystem from the call graph)
// __cdecl. Clears the keyboard state 0x006EFCB0..0x006EFCC0 (shift and three more modifier flags, the
// key-map mode).
void ClearModifierKeys_guess(void)
{
    g_shift_down_guess = 0;
    g_key_state_b4_guess = 0;
    g_key_state_b8_guess = 0;
    g_key_state_bc_guess = 0;
    g_keymap_mode_guess = 0;
}

// FUNCTION: TRILOGY 0x004e884c
// Records crash details, including invalid memory access information, in the debug log.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name validateObjectHeader, low, does not fit)
// __cdecl exception-filter helper (no direct callers): logs the exception code and address of `ep` with the
// `where` prefix and, for an access violation, whether it was a read or a write and the address. Always 1
// (EXCEPTION_EXECUTE_HANDLER).
int LogException_guess(EXCEPTION_POINTERS* ep, char* where)
{
    if (ep != NULL)
    {
        DbgLog_guess("%s Code = %08x  Addr = %08x\n", where, ep->ExceptionRecord->ExceptionCode,
                         ep->ExceptionRecord->ExceptionAddress);
        if (ep->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
        {
            if (ep->ExceptionRecord->NumberParameters > 1)
            {
                if (ep->ExceptionRecord->ExceptionInformation[0] == 0)
                    DbgLog_guess("Process was trying to READ invalid data.\n");
                else
                    DbgLog_guess("Process was trying to WRITE invalid data.\n");
                DbgLog_guess("Invalid access occurred at virtual address 0x%08x.\n",
                                 ep->ExceptionRecord->ExceptionInformation[1]);
            }
            else
            {
                DbgLog_guess("Exception record contained no address or read/write information.\n");
            }
        }
    }
    else
    {
        DbgLog_guess("%s  ep is NULL.\n", where);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004e88c2
// Runs gameplay event-thread work, then ends the client message loop.
unsigned long __stdcall ProcessGameThread_guess(void* context)
{
    typedef BOOL (__stdcall *PostMessageA_guess_t)(HWND, UINT, WPARAM, LPARAM);

    EQ_GLOBAL(int, 0x006EFE30) = 0;
    if (pinstCEverQuest_guess != 0 && *(int*)pinstCEverQuest_guess == 0)
        EQ_METHOD(2, void, 0x004DA043, (CEverQuest*, HWND, HINSTANCE), pinstCEverQuest_guess)
            (EQ_GLOBAL(HWND, 0x006EE7BC), EQ_GLOBAL(HINSTANCE, 0x006EE720));
    EQ_GLOBAL(int, 0x006EFE30) = 1;
    EQ_GLOBAL(PostMessageA_guess_t, 0x0053422C)
        (EQ_GLOBAL(HWND, 0x006EE7BC), WM_QUIT, 0, 0);
    return 0;
}

// FUNCTION: TRILOGY 0x004e8945
// Reports the game crash context through the exception logger.
int LogExceptionFilter_guess(void)
{
    __asm {
        push 0057D804h
        mov eax, dword ptr [ebp]
        push dword ptr [eax-14h]
    }
    EQ_CDECL(void, 0x004E884C, (void))();
    __asm {
        pop ecx
        pop ecx
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004e8b63
// Runs the client message loop and cleans up game resources at shutdown.
void MainThread(void)
{
    typedef void (__stdcall *Sleep_guess_t)(unsigned long);
    typedef BOOL (__stdcall *GetMessageA_guess_t)(LPMSG, HWND, UINT, UINT);
    typedef BOOL (__stdcall *TranslateMessage_guess_t)(const MSG*);
    typedef LRESULT (__stdcall *DispatchMessageA_guess_t)(const MSG*);
    typedef BOOL (__stdcall *PeekMessageA_guess_t)(LPMSG, HWND, UINT, UINT, UINT);
    typedef void (__stdcall *DeleteCriticalSection_guess_t)(LPCRITICAL_SECTION);
    MSG message;
    int messageCount;
    int startTime;
    int elapsedTime;
    int currentTime;
    void* everquest;
    Sleep_guess_t sleepProc;
    DeleteCriticalSection_guess_t deleteCriticalSectionProc;

    sleepProc = EQ_GLOBAL(Sleep_guess_t, 0x00534198);
    messageCount = 0;
    while (EQ_GLOBAL(void*, 0x006EFE2C) == 0)
        sleepProc(0x20);
    while (pinstCEverQuest_guess->gameState_guess == 6)
        sleepProc(0x20);
    do
    {
        while (1)
        {
            if (EQ_GLOBAL(GetMessageA_guess_t, 0x0053420C)(&message, 0, 0, 0) == 0)
                break;
            EQ_GLOBAL(TranslateMessage_guess_t, 0x00534210)(&message);
            EQ_GLOBAL(DispatchMessageA_guess_t, 0x00534214)(&message);
            if (EQ_GLOBAL(int, 0x006EFE30) != 0)
                goto cleanup;
            if (messageCount < 0xb)
                messageCount = messageCount + 1;
            else
            {
                messageCount = 0;
                EQ_CDECL(void, 0x004A46B0, (void))( );
            }
        }
    } while (EQ_GLOBAL(int, 0x006EFE30) == 0);
cleanup:
    startTime = EQ_CDECL(int, 0x005213E9, (int*))(0);
    do
    {
        if (EQ_GLOBAL(PeekMessageA_guess_t, 0x00534218)(&message, 0, 0, 0, 1) == 0)
            break;
        currentTime = EQ_CDECL(int, 0x005213E9, (int*))(0);
        elapsedTime = currentTime - startTime;
    } while ((unsigned int)elapsedTime < 6);
    DbgLog_guess((const char*)0x0057d854);
    everquest = EQ_GLOBAL(void*, 0x006EFE2C);
    if (EQ_GLOBAL(void*, 0x006EFE2C) != 0)
    {
        if (EQ_GLOBAL(void*, 0x006EFE2C) != 0)
        {
            EQ_METHOD(0, void, 0x004C49EE, (CEverQuest*), (CEverQuest*)everquest)();
            EQ_CDECL(void, 0x005218BB, (void*))(everquest);
        }
    }
    EQ_GLOBAL(void*, 0x006EFE2C) = 0;
    deleteCriticalSectionProc = EQ_GLOBAL(DeleteCriticalSection_guess_t, 0x005341C8);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x006e3388);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x006048e8);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x005fe988);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x005fe7a0);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x006b3298);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x006018c8);
    deleteCriticalSectionProc((LPCRITICAL_SECTION)0x006efa18);
}

// FUNCTION: TRILOGY 0x004e8cb0
// Reports a fatal error in the game's startup or event thread.
int ReportFatalEventThreadError_guess(void)
{
    EQ_CDECL(int, 0x005229E8, (const char*))((const char*)0x0057d828);
    return 1;
}

// FUNCTION: TRILOGY 0x004e8cdb
// Provides the game with its auxiliary client module during startup.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name LoadEQMainDLL, low)
// __cdecl. Loads eqmain.dll (module handle at 0x006EFE34) and looks up its dll_main export (0x006EE750).
// 1 on success; 0 when either fails (the missing export is logged).
int LoadEQMainDLL_guess(void)
{
    g_eqmain_module_guess = LoadLibraryA("eqmain.dll");
    if (g_eqmain_module_guess != NULL)
    {
        g_eqmain_dll_main_guess = (void*)GetProcAddress(g_eqmain_module_guess, "dll_main");
        if (g_eqmain_dll_main_guess == NULL)
            DbgLog_guess("Couldn't link to dll_main in eqmain.dll!\n");
        else
            return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e8d16
// Starts the client's graphics subsystem during launch.
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name initializeEverQuestGraphics, low)
// __cdecl. Loads eqmain.dll, runs its dll_main(module, &0x006EDE68) and unloads it again.
// 1 when dll_main ran, -1 when it returned -1, 0 when it could not be linked. Ignores the load result.
int initializeEverQuestGraphics_guess(void)
{
    int result = 1;

    EQ_CDECL(int, 0x004E8CDB, (void))();
    if (g_eqmain_dll_main_guess != NULL)
        {
        if (((int (__cdecl *)(struct HINSTANCE__ *, void *))g_eqmain_dll_main_guess)(
                 g_eqmain_module_guess, (void *)0x006EDE68) == -1)
        result = -1;
    }
    else result = 0;
    EQ_CDECL(void, 0x004E8D4B, (void))();
    return result;
}

// FUNCTION: TRILOGY 0x004e8d4b
// ORIGINAL FILE: unknown (subsystem from the call graph; TAKP name UnloadEQMainDLL, low)
// __cdecl, no arguments. Frees the eqmain.dll module and clears the handle and the dll_main pointer.
void UnloadEQMainDLL_guess(void)
{
    if (g_eqmain_module_guess != NULL)
    {
        FreeLibrary(g_eqmain_module_guess);
        g_eqmain_module_guess = NULL;
        g_eqmain_dll_main_guess = NULL;
    }
}

// FUNCTION: TRILOGY 0x004eb737
// Sets up keyboard input so the client can read gameplay keys.
int initializeDirectInputKeyboard_guess(HWND window, HINSTANCE instance)
{
    typedef int (__stdcall *DICreateDeviceFn_guess)(void *, const void *, void **, void *);
    typedef int (__stdcall *DISetDataFormatFn_guess)(void *, const void *);
    typedef int (__stdcall *DISetCooperativeLevelFn_guess)(void *, HWND, DWORD);
    typedef int (__stdcall *DISetPropertyFn_guess)(void *, DWORD, void *);
    typedef int (__stdcall *DIAcquireFn_guess)(void *);
    typedef SHORT (WINAPI *GetKeyStateFn_guess)(int);

    int hr;
    DWORD last_error;
    DWORD *property;
    void *input;
    void *device;
    GetKeyStateFn_guess get_key_state;
    SHORT key_state;

    hr = EQ_STDCALL(int, 0x00500404, (HINSTANCE, DWORD, void **, void *))
        (instance, 0x500, EQ_ARRAY(void *, 0x006EFA38), 0);
    if (hr < 0)
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0057DF20));
    }
    else
    {
        g_shift_down_guess = 0;
        g_key_state_b4_guess = 0;
        g_key_state_b8_guess = 0;
        g_key_state_bc_guess = 0;
        g_keymap_mode_guess = 0;

        input = EQ_GLOBAL(void *, 0x006EFA38);
        hr = ((DICreateDeviceFn_guess)(*(void ***)input)[3])
            (input, EQ_ARRAY(char, 0x00536ED8), EQ_ARRAY(void *, 0x006EFC74), 0);
        if (hr < 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0057DEF8));
            return 2;
        }

        device = EQ_GLOBAL(void *, 0x006EFC74);
        hr = ((DISetDataFormatFn_guess)(*(void ***)device)[11])
            (device, EQ_ARRAY(char, 0x00536E58));
        if (hr < 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0057DED0));
            return 3;
        }

        device = EQ_GLOBAL(void *, 0x006EFC74);
        hr = ((DISetCooperativeLevelFn_guess)(*(void ***)device)[13])
            (device, window, 10);
        last_error = ((DWORD (WINAPI *)(void))EQ_GLOBAL(void *, 0x005341AC))();
        if (hr < 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0057DE98), hr, last_error);
            return 4;
        }

        property = EQ_ARRAY(DWORD, 0x006EE728);
        memset(property, 0, 0x14);
        property[0] = 0x14;
        property[1] = 0x10;
        property[2] = 0;
        property[3] = 0;
        property[4] = 0x20;

        device = EQ_GLOBAL(void *, 0x006EFC74);
        hr = ((DISetPropertyFn_guess)(*(void ***)device)[6])
            (device, 1, property);
        if (hr < 0)
        {
            DbgLog_guess(EQ_ARRAY(char, 0x0057DE70));
            return 5;
        }

        if (EQ_GLOBAL(void *, 0x006EFC74) == 0 ||
            ((DIAcquireFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006EFC74))[7])(EQ_GLOBAL(void *, 0x006EFC74)) == 0 ||
            ((DIAcquireFn_guess)(*(void ***)EQ_GLOBAL(void *, 0x006EFC74))[7])(EQ_GLOBAL(void *, 0x006EFC74)) >= 0)
        {
            get_key_state = (GetKeyStateFn_guess)EQ_GLOBAL(void *, 0x005341E0);
            key_state = get_key_state(0x14);
            g_key_state_b8_guess = key_state;
            key_state = get_key_state(0x90);
            g_key_state_bc_guess = key_state;
            return 0;
        }

        DbgLog_guess(EQ_ARRAY(char, 0x0057DE50));
    }

    return 1;
}
