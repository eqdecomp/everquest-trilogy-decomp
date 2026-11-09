// unit_100019E0.cpp - eqmain.dll, the address run 0x100019E0..0x100036A0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <string.h>

#include "trilogy/eqaddr.h"

// One 12-byte record of the table at 0x1004CDF8. Ghidra shows its three dwords as g_eqmain_slot_vec_guess, DAT_1004CDFC
// and DAT_1004CE00; only the layout is known, the dwords are copied as they are.
struct EqmainSlotVec_guess
{
    int v[3];   // 0x0, 0x4, 0x8
};

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
#define g_eqmain_slot_top_guess    EQ_GLOBAL(int, 0x1004CE30)
#define g_eqmain_obj_guess         EQ_GLOBAL(int, 0x1004CEB8)
#define g_eqmain_slot_ptr_guess    EQ_ARRAY(int, 0x1004CD80)
#define g_eqmain_slot_vec_guess    EQ_ARRAY(EqmainSlotVec_guess, 0x1004CDF8)
#define g_eqmain_slot_a_guess      EQ_ARRAY(int, 0x1004CDA0)
#define g_eqmain_slot_b_guess      EQ_ARRAY(int, 0x1004CDB0)
#define g_eqmain_slot_c_guess      EQ_ARRAY(int, 0x1004CD90)
#define g_eqmain_flag_cec4_guess       EQ_GLOBAL(int, 0x1004CEC4)
#define g_eqmain_cur_b_guess       EQ_GLOBAL(int, 0x1004CE6C)
#define g_eqmain_cur_c_guess       EQ_GLOBAL(int, 0x1004CD7C)
#define eqmain_fn_10006c20_guess   EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
#define eqmain_fn_10007a80_guess   EQ_CDECL(void, 0x10007A80, (int *))
#define g_eqmain_full_msg_guess    EQ_GLOBAL(char *, 0x10055A78)
#define eqmain_fn_100278a0_guess   EQ_CDECL(void, 0x100278A0, (char *))
#define eqmain_fn_10006d80_guess   EQ_CDECL(int *, 0x10006D80, (int, int, int))
#define eqmain_fn_10028330_guess   EQ_CDECL(void, 0x10028330, (int *))
#define g_eqmain_null_msg_guess    EQ_GLOBAL(char *, 0x10055A74)
#define eqmain_fn_10028430_guess   EQ_CDECL(void, 0x10028430, (int *, int *))
#define eqmain_fn_10033568_guess   EQ_CDECL(void, 0x10033568, (void *))
#define eqmain_fn_10001b60_guess   EQ_STDCALL(void, 0x10001B60, (void))
#define eqmain_fn_10001c30_guess   EQ_STDCALL(void, 0x10001C30, (void))
#define eqmain_chain_table_free_guess EQ_CDECL(void, 0x10008CE0, (void *))
#define eqmain_fn_10007cf0_guess   EQ_CDECL(void, 0x10007CF0, (int *))
#define eqmain_fn_10007ae0_guess   EQ_CDECL(int *, 0x10007AE0, (char *, int))
#define eqmain_fn_10008c10_guess   EQ_CDECL(int *, 0x10008C10, (int, int, int, int, int, int, int, int, int))
#define eqmain_fn_10009370_guess   EQ_CDECL(void, 0x10009370, (int *, char *))
#define g_eqmain_dlg_ctl_guess     EQ_GLOBAL(int *, 0x1004CDF0)
#define g_eqmain_dlg_arg_guess     EQ_GLOBAL(int, 0x1004CEC8)
#define eqmain_fn_10004ec0_guess   EQ_CDECL(void, 0x10004EC0, (int *))
#define eqmain_fn_10008910_guess   EQ_CDECL(void, 0x10008910, (int *))
#define eqmain_fn_10008a00_guess   EQ_CDECL(void, 0x10008A00, (void *))
#define g_eqmain_state_ptr_guess   EQ_GLOBAL(int *, 0x1004CE2C)
#define g_eqmain_bmp_desc_guess    EQ_GLOBAL(int *, 0x1004CE38)
#define g_eqmain_held_obj_guess    EQ_GLOBAL(void *, 0x1004CE3C)
#define g_eqmain_ui_arg1_guess     EQ_GLOBAL(int, 0x1004CE70)
#define g_eqmain_ui_arg2_guess     EQ_GLOBAL(int, 0x1004CE74)
#define g_eqmain_ui_arg3_guess     EQ_GLOBAL(int, 0x1004CEB4)
#define g_eqmain_flag_ce78_guess   EQ_GLOBAL(unsigned char, 0x1004CE78)
#define eqmain_fn_10003700_guess   EQ_STDCALL(int *, 0x10003700, (void))
#define eqmain_fn_10003a00_guess   EQ_CDECL(void, 0x10003A00, (void))
#define eqmain_fn_10006b90_guess   EQ_CDECL(unsigned int, 0x10006B90, (int, unsigned int))
#define g_eqmain_desc_ce34_guess   EQ_GLOBAL(int *, 0x1004CE34)
#define g_eqmain_str_558f4_guess   EQ_GLOBAL(char *, 0x100558F4)
#define eqmain_fn_100082c0_guess   EQ_CDECL(void, 0x100082C0, (int, int, int, int, char *, int, int))
#define eqmain_fn_10008460_guess   EQ_CDECL(void *, 0x10008460, (int, int, int, int, int, unsigned int, int, int, unsigned int, unsigned int))
#define eqmain_fn_100088f0_guess   EQ_CDECL(void, 0x100088F0, (int))
#define eqmain_fn_10001520_guess   EQ_CDECL(void *, 0x10001520, (int, int, int, unsigned char, int))
#define eqmain_fn_100017b0_guess   EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
#define eqmain_fn_100080b0_guess   EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int))
#define eqmain_fn_10003bd0_guess   EQ_STDCALL(int *, 0x10003BD0, (void))
#define eqmain_fn_10001700_guess   EQ_CDECL(void, 0x10001700, (void *))
#define g_eqmain_str_461ac_guess   EQ_ARRAY(char, 0x100461AC)
#define g_eqmain_str_461b0_guess   EQ_ARRAY(char, 0x100461B0)
#define g_eqmain_str_461b4_guess   EQ_ARRAY(char, 0x100461B4)
#define g_eqmain_str_461b8_guess   EQ_ARRAY(char, 0x100461B8)
#define g_eqmain_str_461c4_guess   EQ_ARRAY(char, 0x100461C4)
#define g_eqmain_text_a_guess      EQ_GLOBAL(char *, 0x10055868)
#define g_eqmain_text_b_guess      EQ_GLOBAL(char *, 0x1005586C)
#define g_str_888_guess     EQ_GLOBAL(char *, 0x10055888)
#define eqmain_fn_10001ff0_guess   EQ_CDECL(unsigned char, 0x10001FF0, (int *, char *, char *, char *))
#define eqmain_fn_10023c10_guess   EQ_CDECL(unsigned char, 0x10023C10, (char *))
#define eqmain_fn_10033330_guess   EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))
#define g_eqmain_str_cdc0_guess    EQ_ARRAY(char, 0x1004CDC0)
#define g_eqmain_byte_cdec_guess   EQ_GLOBAL(unsigned char, 0x1004CDEC)
#define g_eqmain_drive_base_guess    EQ_ARRAY(char, 0x1004CE40)
#define g_eqmain_str_ec10_guess    EQ_ARRAY(char, 0x1004EC10)
#define g_eqmain_msg_5708_guess    EQ_GLOBAL(char *, 0x10055708)
#define g_eqmain_msg_570c_guess    EQ_GLOBAL(char *, 0x1005570C)
#define g_eqmain_ptr_ce28_guess    EQ_GLOBAL(int *, 0x1004CE28)
#define g_eqmain_dword_100f4b4c_guess EQ_GLOBAL(unsigned int, 0x100F4B4C)
#define eqmain_fn_100023b0_guess   EQ_STDCALL(unsigned char, 0x100023B0, (void))
#define eqmain_fn_10023a80_guess   EQ_CDECL(void, 0x10023A80, (char *, char *))
#define eqmain_fn_10023c00_guess   EQ_STDCALL(void, 0x10023C00, (void))
#define eqmain_fn_100022b0_guess   EQ_STDCALL(void, 0x100022B0, (void))
#define eqmain_fn_10002350_guess   EQ_STDCALL(void, 0x10002350, (void))
#define eqmain_fn_10008970_guess   EQ_CDECL(void, 0x10008970, (int *, char *))
#define eqmain_fn_100018f0_guess   EQ_CDECL(int *, 0x100018F0, (int, int, int, char *, char *, int, int, char *, int, int))
#define eqmain_fn_100024a0_guess   EQ_CDECL(void, 0x100024A0, (char))
#define eqmain_fn_100019e0_guess   EQ_CDECL(void, 0x100019E0, (void *))
#define g_eqmain_text_5848_guess   EQ_GLOBAL(char *, 0x10055848)
#define g_eqmain_text_584c_guess   EQ_GLOBAL(char *, 0x1005584C)
#define g_eqmain_text_58e0_guess   EQ_GLOBAL(char *, 0x100558E0)
#define g_eqmain_text_58e4_guess   EQ_GLOBAL(char *, 0x100558E4)
#define g_eqmain_str_556f0_guess   EQ_GLOBAL(char *, 0x100556F0)
#define g_eqmain_str_46208_guess   EQ_ARRAY(char, 0x10046208)
#define g_chekbox1_bmp_guess   EQ_ARRAY(char, 0x100461D8)
#define g_chekbox2_bmp_guess   EQ_ARRAY(char, 0x100461E8)
#define g_eqmain_str_461f8_guess   EQ_ARRAY(char, 0x100461F8)

// FUNCTION: EQMAIN 0x100019E0
// Release a front-end record and detach it from its owner's list.
// __cdecl, returns nothing. A null unit passes g_eqmain_null_msg_guess to 0x100278A0 and returns. Otherwise the non-null
// pointers at +0x24 and +0x28 go to 0x10007A80, the unit is removed from the list at its owner's +0x1C (the owner
// is at +0x8) when it is on that list, and the unit is freed.
void __cdecl eqmainFreeUnit_guess(void *record)
{
    int *node;

    if (record == 0)
    {
        eqmain_fn_100278a0_guess(g_eqmain_null_msg_guess);
        return;
    }
    if (*(int **)((int)record + 0x24) != 0)
    {
        eqmain_fn_10007a80_guess(*(int **)((int)record + 0x24));
    }
    if (*(int **)((int)record + 0x28) != 0)
    {
        eqmain_fn_10007a80_guess(*(int **)((int)record + 0x28));
    }
    node = *(int **)(*(int *)((int)record + 0x8) + 0x1c);
    if (node != 0)
    {
        do
        {
            if (record != (void *)node[1])
            {
                node = (int *)node[2];
            }
            else
            {
                eqmain_fn_10028430_guess((int *)(*(int *)((int)record + 0x8) + 0x1c), node);
                break;
            }
        }
        while (node != 0);
    }
    eqmain_fn_10033568_guess(record);
    return;
}

// FUNCTION: EQMAIN 0x10001A60
// Synchronize the checked state of a paired front-end option.
// @early-stop: The not-equal arm shares its zero-update call with the equal arm, changing the original branch layout and omitting one call block.
// __cdecl, returns nothing. If `pick` is not `unit`: `unit`'s +0x1C becomes 0 when `other`'s +0x1C is 1, else 1,
// and 0x10004EC0 gets `unit`. If `pick` is `unit`: the same rule with the roles swapped sets `other`'s +0x1C from
// `unit`'s and passes `other` to 0x10004EC0. Every path returns after that call.
void __cdecl eqmainSyncSlotPair_guess(int *unit, int *other, int *pick)
{
    if (pick == unit)
    {
        if (unit[7] == 1)
        {
            other[7] = 0;
            eqmain_fn_10004ec0_guess(other);
        }
        else
        {
            other[7] = 1;
            eqmain_fn_10004ec0_guess(other);
        }
        return;
    }
    else
    {
        if (other[7] == 1)
        {
            unit[7] = 0;
            eqmain_fn_10004ec0_guess(unit);
            return;
        }
        else
        {
            unit[7] = 1;
            eqmain_fn_10004ec0_guess(unit);
            return;
        }
    }
}

// FUNCTION: EQMAIN 0x10001AD0
// Choose one option in a front-end group and clear the others.
// @early-stop: The indexed scan matches; cleanup loop entry and register reuse still differ.
// __cdecl, returns a short. If `control` is the one `checkedBox` already holds: its +0x1C becomes 1, 0x10004EC0 gets it, and
// the result is -1. Otherwise, if `control` is one of the first `count` entries of `sortBoxes`: `checkedBox` takes it, every entry's
// +0x1C becomes 0 and then `control`'s becomes 1, 0x10004EC0 is called on each entry in sortBoxes order, and the result is
// the index of the first match. Not found (or count <= 0): -1.
short __cdecl eqmainSelectUnitInGroup_guess(int control, int *sortBoxes, short count, int *checkedBox)
{
    short i;

    if (control == *checkedBox)
    {
        *(int *)(*checkedBox + 0x1c) = 1;
        eqmain_fn_10004ec0_guess((int *)*checkedBox);
    }
    else
    {
        for (i = 0; i < count; i++)
        {
            if (control == sortBoxes[i])
            {
                int *p;
                int left;
                unsigned int callCount;
                int *u;
                int *selected;

                *checkedBox = control;
                p = sortBoxes;
                for (left = count; left != 0; left--)
                {
                    u = (int *)*p;
                    p++;
                    u[7] = 0;
                }
                selected = (int *)*checkedBox;
                callCount = count;
                selected[7] = 1;
                do
                {
                    eqmain_fn_10004ec0_guess((int *)*sortBoxes);
                    sortBoxes++;
                    callCount--;
                } while (callCount != 0);
                return i;
            }
        }
        return -1;
    }
    return -1;
}

// FUNCTION: EQMAIN 0x10001B60
// Save the current front-end screen state before displaying another screen.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. The mirror of the pop: when
// the index g_eqmain_slot_top_guess is 3 it passes g_eqmain_full_msg_guess to 0x100278A0 and returns. Otherwise it saves
// g_eqmain_flag_cec4_guess, g_cached_entry_a_guess and g_eqmain_cur_c_guess at the index, clears them, and stores the pointer from
// 0x10006D80(0x280, 0x1E0, 0x10) at the index. 0x10006C20 gets that pointer and the object at g_eqmain_ceb8_guess
// (then 0x280 and 0x1E0, as in the pop). The object's +0x1C..+0x24 dwords go into the slot's record,
// 0x10028330 clears the object's +0x24, and the index is incremented. The index and the object pointer are
// read again after each call.
void __stdcall eqmainPushSlotState_guess(void)
{
    int zero;
    int slot;
    int obj;
    int *ptr;
    int *rec;
    EqmainSlotVec_guess *dst;

    slot = g_eqmain_slot_top_guess;
    if (slot == 3)
    {
        eqmain_fn_100278a0_guess(g_eqmain_full_msg_guess);
        return;
    }
    g_eqmain_slot_a_guess[slot] = g_eqmain_flag_cec4_guess;
    g_eqmain_slot_b_guess[slot] = g_eqmain_cur_b_guess;
    g_eqmain_slot_c_guess[slot] = g_eqmain_cur_c_guess;
    zero = 0;
    g_eqmain_flag_cec4_guess = zero;
    g_eqmain_cur_b_guess = zero;
    g_eqmain_cur_c_guess = zero;
    ptr = eqmain_fn_10006d80_guess(0x280, 0x1e0, 0x10);
    obj = g_eqmain_obj_guess;
    g_eqmain_slot_ptr_guess[g_eqmain_slot_top_guess] = (int)ptr;
    eqmain_fn_10006c20_guess((int)ptr, zero, zero, obj, zero, zero, 0x280, 0x1e0, zero);
    obj = g_eqmain_obj_guess;
    slot = g_eqmain_slot_top_guess;
    rec = (int *)(obj + 0x1c);
    dst = &g_eqmain_slot_vec_guess[slot];
    *dst = *(EqmainSlotVec_guess *)rec;
    eqmain_fn_10028330_guess(rec);
    g_eqmain_slot_top_guess = g_eqmain_slot_top_guess + 1;
}

// FUNCTION: EQMAIN 0x10001C30
// Restore the saved front-end screen state after another screen closes.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Pops the top slot: the index
// g_eqmain_slot_top_guess is decremented and the slot at the new index is used. 0x10006C20 gets the object at g_eqmain_ceb8_guess
// and the slot's pointer (then 0x280 and 0x1E0, i.e. 640x480). The slot's record goes into the object at
// +0x1C..+0x24, 0x10007A80 gets the slot's pointer, and last three per-slot dwords are copied into g_eqmain_flag_cec4_guess,
// g_cached_entry_a_guess and g_eqmain_cur_c_guess. The index and the object pointer are read again after each call.
void __stdcall eqmainPopSlotState_guess(void)
{
    int slot;
    int obj;
    int *rec;
    EqmainSlotVec_guess *src;

    g_eqmain_slot_top_guess = g_eqmain_slot_top_guess - 1;
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0, 0,
        g_eqmain_slot_ptr_guess[g_eqmain_slot_top_guess], 0, 0, 0x280, 0x1e0, 0);
    obj = g_eqmain_obj_guess;
    slot = g_eqmain_slot_top_guess;
    rec = (int *)(obj + 0x1c);
    src = &g_eqmain_slot_vec_guess[slot];
    *(EqmainSlotVec_guess *)rec = *src;
    eqmain_fn_10007a80_guess((int *)g_eqmain_slot_ptr_guess[g_eqmain_slot_top_guess]);
    slot = g_eqmain_slot_top_guess;
    g_eqmain_flag_cec4_guess = g_eqmain_slot_a_guess[slot];
    g_eqmain_cur_b_guess = g_eqmain_slot_b_guess[slot];
    g_eqmain_cur_c_guess = g_eqmain_slot_c_guess[slot];
}

// FUNCTION: EQMAIN 0x10001CE0
// Show a message with two or three selectable responses.
// __cdecl, returns the index of the button whose control handle came up (the last matching slot wins), -1 when Esc
// ends the loop, and -2 with nothing created when mode is not 1..3 or the text is empty. The text is copied into
// a 2 KB buffer that 0x10009370 reads. Each button row is 0x10001520 (its handle) then 0x100017B0 (its text);
// case 1 draws two texts with 0x100080B0, and the second takes the first's return value as its 5th argument.
int __cdecl eqmainRunChoiceDialog_guess(int *parent, char *message, int choiceMode)
{
    char textBuf[2048];
    bool done;
    void *buttons[4];
    int result;
    int *obj;
    int *msg;
    void *pHVar4;
    void *pHVar5;
    void *pHVar6;
    int index;
    int textRes;
    void **ppHVar10;

    if (choiceMode >= 1 && choiceMode <= 3)
    {
        if (strlen(message) >= 1)
        {
            strcpy(textBuf, message);
            eqmain_fn_10001b60_guess();
            eqmain_fn_10007cf0_guess(parent);
            buttons[0] = 0;
            buttons[1] = 0;
            buttons[2] = 0;
            buttons[3] = 0;
            done = false;
            result = -1;
            obj = eqmain_fn_10007ae0_guess(g_eqmain_str_461b8_guess, 0x10);
            eqmain_fn_10006c20_guess((int)parent, 0xaa, 0x32, (int)obj, 0, 0, obj[0], obj[1], 1);
            eqmain_fn_10007a80_guess(obj);
            switch (choiceMode)
            {
            case 1:
                pHVar4 = eqmain_fn_10001520_guess((int)parent, 0x1d6, 0x11c, 0x18, 0);
                buttons[0] = pHVar4;
                pHVar5 = eqmain_fn_10001520_guess((int)parent, 0xaa, 0x11c, 0x14, 0);
                buttons[1] = pHVar5;
                textRes = eqmain_fn_100080b0_guess((int)parent, 0x50, 0x22, g_eqmain_text_a_guess, 0x19, 0);
                eqmain_fn_100080b0_guess((int)parent, 0x50, 0x22, g_eqmain_text_b_guess, textRes, 0);
                eqmain_fn_100017b0_guess(pHVar4, 0, g_eqmain_text_a_guess, 1, 0);
                eqmain_fn_100017b0_guess(pHVar5, 0, g_eqmain_text_b_guess, 1, 0);
                break;
            case 2:
                pHVar4 = eqmain_fn_10001520_guess((int)parent, 0xaa, 0x11c, 0x14, 0);
                buttons[0] = pHVar4;
                pHVar5 = eqmain_fn_10001520_guess((int)parent, 0x1d6, 0x11c, 0x18, 0);
                buttons[1] = pHVar5;
                eqmain_fn_100017b0_guess(pHVar4, 0, g_eqmain_str_461b4_guess, 1, 0);
                eqmain_fn_100017b0_guess(pHVar5, 0, g_eqmain_str_461b0_guess, 1, 0);
                break;
            case 3:
                pHVar4 = eqmain_fn_10001520_guess((int)parent, 0xaa, 0x11c, 0x14, 0);
                buttons[0] = pHVar4;
                pHVar5 = eqmain_fn_10001520_guess((int)parent, 0, 0x11c, 0x11, 0);
                buttons[1] = pHVar5;
                pHVar6 = eqmain_fn_10001520_guess((int)parent, 0x1d6, 0x11c, 0x18, 0);
                buttons[2] = pHVar6;
                eqmain_fn_100017b0_guess(pHVar4, 0, g_eqmain_str_461b4_guess, 1, 0);
                eqmain_fn_100017b0_guess(pHVar5, 0, g_eqmain_str_461b0_guess, 1, 0);
                eqmain_fn_100017b0_guess(pHVar6, 0, g_eqmain_str_461ac_guess, 1, 0);
                break;
            }
            obj = eqmain_fn_10008c10_guess((int)parent, 0xdc, 100, 200, 0x86, 10, 0x21a3c, g_eqmain_dlg_arg_guess, 0);
            eqmain_fn_10009370_guess(obj, textBuf);
            do
            {
                msg = eqmain_fn_10003bd0_guess();
                if (msg != 0)
                {
                    if (*msg != 0)
                    {
                        if (*msg == 4 && (char)msg[2] == '\x1b')
                        {
                            done = true;
                        }
                    }
                    else
                    {
                        index = 0;
                        ppHVar10 = buttons;
                        do
                        {
                            if ((void *)msg[2] == *ppHVar10)
                            {
                                done = true;
                                result = index;
                            }
                            index = index + 1;
                            ppHVar10 = ppHVar10 + 1;
                        } while (index < 4);
                    }
                    eqmain_fn_10033568_guess(msg);
                }
            } while (!done);
            eqmain_chain_table_free_guess(obj);
            ppHVar10 = buttons;
            index = 4;
            do
            {
                if (*ppHVar10 != 0)
                {
                    eqmain_fn_10001700_guess(*ppHVar10);
                }
                ppHVar10 = ppHVar10 + 1;
                index = index - 1;
            } while (index != 0);
            eqmain_fn_10001c30_guess();
            return result;
        }
    }
    return -2;
}

// FUNCTION: EQMAIN 0x10001FF0
// Show a message with an OK button and an optional Cancel button.
// __cdecl, returns a byte: 1 for OK (a click on the OK handle, or Enter), 0 for cancel (a click on the cancel handle,
// which is 0 when there is no cancel button, or Escape). Copies the message into a 2048-byte buffer and shows it
// on dialog.bmp (210 characters or fewer) or dialogmedium.bmp (longer). When cancelText is NULL there is only the
// OK button. Then loops over the messages from 0x10003BD0 until one ends it.
unsigned char eqmainRunDialog_guess(int *screen, char *message, char *okText, char *cancelText)
{
    char messageBuf[2048];
    void *cancelButton;
    void *okButton;
    char bResult;
    char isLong;
    char bDone;
    int *widget;    // the bitmap from 0x10007AE0, then the text box from 0x10008C10
    int *msg;       // [0] type, [2] control or key
    int textRes;
    int textRes2;

    strcpy(messageBuf, message);
    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess(screen);
    cancelButton = 0;
    bDone = 0;
    bResult = 0;
    isLong = 0;
    if (strlen(message) > 0xd2)
    {
        isLong = 1;
        widget = eqmain_fn_10007ae0_guess(g_eqmain_str_461c4_guess, 0x10);
        eqmain_fn_10006c20_guess((int)screen, 0x55, 0x32, (int)widget, 0, 0, widget[0], widget[1], 1);
    }
    else
    {
        widget = eqmain_fn_10007ae0_guess(g_eqmain_str_461b8_guess, 0x10);
        eqmain_fn_10006c20_guess((int)screen, 0xaa, 0x32, (int)widget, 0, 0, widget[0], widget[1], 1);
    }
    eqmain_fn_10007a80_guess(widget);
    if (cancelText != 0)
    {
        if (isLong)
        {
            cancelButton = eqmain_fn_10001520_guess((int)screen, 0x22b, 0x1a9, 0x18, 0);
            okButton = eqmain_fn_10001520_guess((int)screen, 0x55, 0x1a9, 0x14, 0);
        }
        else
        {
            cancelButton = eqmain_fn_10001520_guess((int)screen, 0x1d6, 0x11c, 0x18, 0);
            okButton = eqmain_fn_10001520_guess((int)screen, 0xaa, 0x11c, 0x14, 0);
        }
        textRes = eqmain_fn_100080b0_guess((int)screen, 0x50, 0x22, cancelText, 0x19, 0);
        textRes2 = eqmain_fn_100080b0_guess((int)screen, 0x50, 0x22, okText, textRes, 0);
        eqmain_fn_100017b0_guess(cancelButton, 0, cancelText, textRes2, 0);
        eqmain_fn_100017b0_guess(okButton, 0, okText, textRes2, 0);
    }
    else
    {
        if (isLong)
        {
            okButton = eqmain_fn_10001520_guess((int)screen, 0, 0x1a9, 0x11, 0);
        }
        else
        {
            okButton = eqmain_fn_10001520_guess((int)screen, 0, 0x11c, 0x11, 0);
        }
        textRes = eqmain_fn_100080b0_guess((int)screen, 0x50, 0x22, okText, 0x19, 0);
        eqmain_fn_100017b0_guess(okButton, 0, okText, textRes, 0);
    }
    if (isLong)
    {
        widget = eqmain_fn_10008c10_guess((int)screen, 0x87, 100, 0x172, 0x113, 10, 0x21a3c, g_eqmain_dlg_arg_guess, 0);
    }
    else
    {
        widget = eqmain_fn_10008c10_guess((int)screen, 0xdc, 100, 200, 0x86, 10, 0x21a3c, g_eqmain_dlg_arg_guess, 0);
    }
    eqmain_fn_10009370_guess(widget, messageBuf);
    do
    {
        msg = eqmain_fn_10003bd0_guess();
        if (msg != 0)
        {
            switch (msg[0])
            {
            case 0:
                if ((void *)msg[2] == okButton)
                {
                    bDone = 1;
                    bResult = 1;
                }
                else if ((void *)msg[2] == cancelButton)
                {
                    bDone = 1;
                }
                break;
            case 4:
                switch ((char)msg[2])
                {
                case '\r':
                    bDone = 1;
                    bResult = 1;
                    break;
                case '\x1b':
                    bDone = 1;
                    break;
                }
                break;
            }
            eqmain_fn_10033568_guess(msg);
        }
    } while (!bDone);
    eqmain_chain_table_free_guess(widget);
    eqmain_fn_10001700_guess(okButton);
    if (cancelButton != 0)
    {
        eqmain_fn_10001700_guess(cancelButton);
    }
    eqmain_fn_10001c30_guess();
    return bResult;
}

// FUNCTION: EQMAIN 0x100022B0
// Set up the front-end entry panel and its text field.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Calls 0x10006C20 with the object at
// g_eqmain_ceb8_guess, 0xAD, 0x90, g_eqmain_desc_ce34_guess (its first two dwords are the size), 0, 0, those two dwords and 0. Then
// 0x100082C0 with the object, 0xB3, 0x82, 0xF3F2F2, the string at g_eqmain_str_558f4_guess and two zeros (the callee reads
// five of its seven pushed arguments). Then 0x10008460 with the object, 0xB2, 0x92, 0x118, 0x10, 0x11, 0, 0,
// 0xE8C9CC and 0; its result goes into g_eqmain_held_obj_guess. Then 0x100088F0 and 0x10008910 get that pointer. The cdecl
// arguments stay on the stack until the ADD ESP,0x30 after the last call.
void __stdcall eqmainDrawAndCreate_guess(void)
{
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0xad, 0x90, (int)g_eqmain_desc_ce34_guess,
        0, 0, g_eqmain_desc_ce34_guess[0], g_eqmain_desc_ce34_guess[1], 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0xb3, 0x82, 0xf3f2f2, g_eqmain_str_558f4_guess, 0, 0);
    g_eqmain_held_obj_guess = eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0xb2, 0x92, 0x118, 0x10, 0x11,
        0, 0, 0xe8c9cc, 0);
    eqmain_fn_100088f0_guess((int)g_eqmain_held_obj_guess);
    eqmain_fn_10008910_guess((int *)g_eqmain_held_obj_guess);
}

// FUNCTION: EQMAIN 0x10002350
// Remove the temporary front-end text record.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Calls 0x10008910 with g_eqmain_state_ptr_guess
// (cdecl: its argument stays on the stack until the ADD ESP,0x28 after the next call), then 0x10006C20 with the
// object at g_eqmain_ceb8_guess, 0xAD, 0x80, g_eqmain_bmp_desc_guess (its first two dwords are the size) and 0. If g_eqmain_held_obj_guess is
// set, 0x10008A00 frees it and the global is cleared.
void __stdcall eqmainDrawAndRelease_guess(void)
{
    eqmain_fn_10008910_guess(g_eqmain_state_ptr_guess);
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0xad, 0x80, (int)g_eqmain_bmp_desc_guess,
        0, 0, g_eqmain_bmp_desc_guess[0], g_eqmain_bmp_desc_guess[1], 0);
    if (g_eqmain_held_obj_guess != 0)
    {
        eqmain_fn_10008a00_guess(g_eqmain_held_obj_guess);
        g_eqmain_held_obj_guess = 0;
    }
}

// FUNCTION: EQMAIN 0x100023B0
// Require nonblank text in the active entry fields before submission.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns a byte in AL (1 or 0; unsigned char keeps the exact value). Copies the
// text at [g_eqmain_state_ptr_guess + 0x20] into g_eqmain_str_cdc0_guess (strncpy of 0x2D bytes at 0x10033330, a real call), clears g_eqmain_byte_cdec_guess
// (the buffer's last byte) and calls 0x10023C10 on the buffer. If the buffer is empty, 0x10001FF0 runs with the object at
// g_eqmain_ceb8_guess, g_eqmain_msg_5708_guess and g_str_888_guess (OK only) and the result is 0. If g_eqmain_held_obj_guess is NULL, g_eqmain_drive_base_guess is
// copied (strcpy) to g_eqmain_str_ec10_guess and the result is 1. Else [g_eqmain_held_obj_guess + 0x20] is copied to g_eqmain_str_ec10_guess; if that is
// empty, 0x10001FF0 runs with g_eqmain_msg_570c_guess and the result is 0, else 1. strlen and strcpy are the inline intrinsics.
unsigned char __stdcall eqmainCheckEntryText_guess(void)
{
    eqmain_fn_10033330_guess(g_eqmain_str_cdc0_guess, *(char **)((char *)g_eqmain_state_ptr_guess + 0x20), 0x2d);
    g_eqmain_byte_cdec_guess = 0;
    eqmain_fn_10023c10_guess(g_eqmain_str_cdc0_guess);
    if (strlen(g_eqmain_str_cdc0_guess) == 0)
    {
        eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, g_eqmain_msg_5708_guess, g_str_888_guess, 0);
        return 0;
    }
    if (g_eqmain_held_obj_guess != 0)
    {
        strcpy(g_eqmain_str_ec10_guess, *(char **)((char *)g_eqmain_held_obj_guess + 0x20));
        if (strlen(g_eqmain_str_ec10_guess) == 0)
        {
            eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, g_eqmain_msg_570c_guess, g_str_888_guess, 0);
            return 0;
        }
    }
    else
    {
        strcpy(g_eqmain_str_ec10_guess, g_eqmain_drive_base_guess);
    }
    return 1;
}

// FUNCTION: EQMAIN 0x100024A0
// Submit the two validated entry fields.
// __cdecl with one char argument (the caller pops it); returns nothing. Sets the dword at 0x100F4B4C to 2 when the argument
// is non-zero, else to the dword at [g_eqmain_ptr_ce28_guess + 0x1C]. Then calls 0x100023B0 (eqmainCheckEntryText_guess, no arguments)
// and, if it returns non-zero, 0x10023A80 with g_eqmain_str_cdc0_guess and g_eqmain_str_ec10_guess.
void __cdecl eqmainSubmitEntry_guess(char entryMode)
{
    if (entryMode != '\0')
    {
        g_eqmain_dword_100f4b4c_guess = 2;
    }
    else
    {
        g_eqmain_dword_100f4b4c_guess = *(int *)((char *)g_eqmain_ptr_ce28_guess + 0x1c);
    }
    if (eqmain_fn_100023b0_guess() != 0)
    {
        eqmain_fn_10023a80_guess(g_eqmain_str_cdc0_guess, g_eqmain_str_ec10_guess);
    }
}

// FUNCTION: EQMAIN 0x100024E0
// Collects front-end entry text, with a checkbox option in one mode.
// __cdecl with (int *, char *, char); returns nothing. Builds the entry dialog on the object: the cretchat.bmp background
// (0x10007AE0, 0x10006C20), two buttons from 0x10001520 (y 0xF4 cancels, y 0xC1 submits through 0x100024A0), their
// captions, the edit (0x10008460) with entryText as its text, and the password.bmp field. entryMode == 0 adds the
// chekbox1/chekbox2 check box (0x100018F0). Otherwise 0x100022B0 adds a second edit (g_eqmain_held_obj_guess) and strlen(entryText)
// picks the focus. Then it loops on the messages from 0x10003BD0 until a button or Escape sets done, frees each message,
// and destroys the controls.
void __cdecl eqmainRunEntryDialog_guess(int *screen, char *entryText, char entryMode)
{
    void *ctl;
    void *ctlCancel;
    void *ctlSubmit;
    bool done;
    int *bgDesc;
    char *caption;
    int *msg;
    int textA;
    int textB;

    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess(screen);
    done = false;
    bgDesc = eqmain_fn_10007ae0_guess(g_eqmain_str_46208_guess, 0x10);
    eqmain_fn_10006c20_guess((int)screen, 0x7e, 0x32, (int)bgDesc, 0, 0, bgDesc[0], bgDesc[1], 1);
    eqmain_fn_10023c00_guess();
    ctlCancel = eqmain_fn_10001520_guess((int)screen, 0x171, 0xf4, 0, 1);
    ctlSubmit = eqmain_fn_10001520_guess((int)screen, 0x171, 0xc1, 0, 1);
    textA = eqmain_fn_100080b0_guess((int)screen, 0x64, 0x22, g_eqmain_text_5848_guess, 0x19, 0);
    textB = eqmain_fn_100080b0_guess((int)screen, 0x64, 0x22, g_eqmain_text_584c_guess, textA, 0);
    eqmain_fn_100017b0_guess(ctlCancel, 0, g_eqmain_text_5848_guess, textB, 0);
    eqmain_fn_100017b0_guess(ctlSubmit, 0, g_eqmain_text_584c_guess, textB, 0);
    caption = g_eqmain_text_58e4_guess;
    if (entryMode == '\0')
    {
        caption = g_eqmain_text_58e0_guess;
    }
    eqmain_fn_100082c0_guess((int)screen, 0xb3, 0x60, 0xf3f2f2, caption, 0, 0);
    g_eqmain_state_ptr_guess = (int *)eqmain_fn_10008460_guess((int)screen, 0xb2, 0x71, 0x118, 0x10, 0x3f, 0, 0, 0xe8c9cc, 0);
    eqmain_fn_10008970_guess(g_eqmain_state_ptr_guess, entryText);
    eqmain_fn_10008910_guess(g_eqmain_state_ptr_guess);
    g_eqmain_desc_ce34_guess = eqmain_fn_10007ae0_guess(g_eqmain_str_461f8_guess, 0x10);
    g_eqmain_bmp_desc_guess = eqmain_fn_10006d80_guess(g_eqmain_desc_ce34_guess[0], g_eqmain_desc_ce34_guess[1] + 0x10, 0x10);
    eqmain_fn_10006c20_guess((int)g_eqmain_bmp_desc_guess, 0, 0, (int)screen, 0xad, 0x80, g_eqmain_bmp_desc_guess[0], g_eqmain_bmp_desc_guess[1], 0);
    g_eqmain_held_obj_guess = 0;
    if (entryMode == '\0')
    {
        g_eqmain_ptr_ce28_guess = eqmain_fn_100018f0_guess((int)screen, 0xae, 0xd6, g_chekbox1_bmp_guess, g_chekbox2_bmp_guess, g_eqmain_dword_100f4b4c_guess, 0xf3f2f2, g_eqmain_str_556f0_guess, 0, 0);
    }
    else
    {
        g_eqmain_dword_100f4b4c_guess = 2;
        eqmain_fn_100022b0_guess();
        g_eqmain_ptr_ce28_guess = 0;
        if (strlen(entryText) == 0)
        {
            eqmain_fn_10008910_guess(g_eqmain_state_ptr_guess);
        }
        else
        {
            eqmain_fn_10008910_guess((int *)g_eqmain_held_obj_guess);
        }
    }
    do
    {
        msg = eqmain_fn_10003bd0_guess();
        if (msg != 0)
        {
            switch (*msg)
            {
            case 4:
                if (*(char *)(msg + 2) == '\x1b')
                {
                    done = true;
                }
                break;
            case 6:
            case 7:
                if ((void *)msg[2] == g_eqmain_state_ptr_guess)
                {
                    if (g_eqmain_held_obj_guess != 0)
                    {
                        eqmain_fn_10008910_guess((int *)g_eqmain_held_obj_guess);
                    }
                }
                else if ((void *)msg[2] == g_eqmain_held_obj_guess)
                {
                    eqmain_fn_10008910_guess(g_eqmain_state_ptr_guess);
                }
                break;
            case 0:
                ctl = (void *)msg[2];
                if (ctl == ctlSubmit)
                {
submitDone:
                    eqmain_fn_100024a0_guess(entryMode);
                    done = true;
                }
                else if (ctl == ctlCancel)
                {
                    done = true;
                }
                else if (ctl == g_eqmain_state_ptr_guess)
                {
                    if (g_eqmain_held_obj_guess == 0)
                    {
                        goto submitDone;
                    }
                    eqmain_fn_10008910_guess((int *)g_eqmain_held_obj_guess);
                }
                else if (ctl == g_eqmain_held_obj_guess)
                {
                    eqmain_fn_100024a0_guess(entryMode);
                    done = true;
                }
                break;
            case 1:
                if (entryMode == '\0' && (int *)msg[2] == g_eqmain_ptr_ce28_guess)
                {
                    if (g_eqmain_ptr_ce28_guess[7] == 1)
                    {
                        eqmain_fn_100022b0_guess();
                    }
                    else
                    {
                        eqmain_fn_10002350_guess();
                    }
                }
                break;
            }
            eqmain_fn_10033568_guess(msg);
        }
        if (done)
        {
            eqmain_fn_10001700_guess(ctlCancel);
            eqmain_fn_10001700_guess(ctlSubmit);
            eqmain_fn_10008a00_guess(g_eqmain_state_ptr_guess);
            if (g_eqmain_held_obj_guess != 0)
            {
                eqmain_fn_10008a00_guess(g_eqmain_held_obj_guess);
            }
            if (g_eqmain_ptr_ce28_guess != 0)
            {
                eqmain_fn_100019e0_guess(g_eqmain_ptr_ce28_guess);
            }
            eqmain_fn_10007a80_guess(bgDesc);
            eqmain_fn_10007a80_guess(g_eqmain_desc_ce34_guess);
            eqmain_fn_10007a80_guess(g_eqmain_bmp_desc_guess);
            eqmain_fn_10001c30_guess();
            return;
        }
    } while (true);
}

#define g_eqmain_str_55904_guess   EQ_GLOBAL(char *, 0x10055904)
#define g_eqmain_text_5870_guess   EQ_GLOBAL(char *, 0x10055870)
#define g_eqmain_text_58f8_guess   EQ_GLOBAL(char *, 0x100558F8)
#define g_eqmain_text_58fc_guess   EQ_GLOBAL(char *, 0x100558FC)
#define g_eqmain_name_guess  EQ_ARRAY(char, 0x10104C70)
#define g_eqmain_str_104cb0_guess  EQ_ARRAY(char, 0x10104CB0)
#define g_eqmain_str_46218_guess   EQ_ARRAY(char, 0x10046218)

// FUNCTION: EQMAIN 0x10002890
// Collects one front-end entry value, using the preset text when the player chooses the default.
// __stdcall, no arguments; returns nothing. Builds the entrroom.bmp dialog on g_eqmain_ceb8_guess: two buttons (x 0x214 and
// 0x6C), their texts, five labels and an edit (0x10008460). Polls 0x10003BD0: a click on the edit or the 0x6C button
// copies the edit's text (+0x20), a click on the 0x214 button or Escape the default text g_eqmain_drive_base_guess, to g_eqmain_str_ec10_guess.
void __stdcall eqmainRunSingleFieldEntryDialog_guess(void)
{
    bool done;
    int *bgDesc;
    void *ctlRight;
    void *ctlLeft;
    int *edit;
    int textRes;
    int *msg;
    int *ctl;
    char *text;

    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess((int *)g_eqmain_obj_guess);
    done = false;
    bgDesc = eqmain_fn_10007ae0_guess(g_eqmain_str_46218_guess, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0x6c, 0x32, (int)bgDesc, 0, 0, bgDesc[0], bgDesc[1], 1);
    ctlRight = eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x214, 0x145, 0x18, 1);
    ctlLeft = eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x6c, 0x145, 0x14, 1);
    textRes = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_eqmain_text_5848_guess, 0x19, 0);
    textRes = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_eqmain_text_5870_guess, textRes, 0);
    eqmain_fn_100017b0_guess(ctlRight, 0, g_eqmain_text_5848_guess, textRes, 0);
    eqmain_fn_100017b0_guess(ctlLeft, 0, g_eqmain_text_5870_guess, textRes, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0x9d, 0x6f, 0xf3f2f2, g_eqmain_text_58f8_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0x15a, 0x6f, 0xf3f2f2, g_eqmain_text_58fc_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0x9d, 0xaf, 0xf3f2f2, g_eqmain_str_55904_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0x9d, 0x82, 0xe8c9cc, g_eqmain_name_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0x15a, 0x82, 0xe8c9cc, g_eqmain_str_104cb0_guess, 0, 0);
    edit = (int *)eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0x9d, 0xc2, 0x118, 0x10, 0x11, 0, 0, 0xe8c9cc, 0);
    eqmain_fn_10008910_guess(edit);
    do
    {
        msg = eqmain_fn_10003bd0_guess();
        if (msg != 0)
        {
            if (*msg == 0)
            {
                ctl = (int *)msg[2];
                if (ctl == (int *)ctlLeft || ctl == edit)
                {
                    text = (char *)edit[8];
                }
                else
                {
                    if (ctl != (int *)ctlRight)
                    {
                        goto freeMsg;
                    }
                    text = g_eqmain_drive_base_guess;
                }
                done = true;
                strcpy(g_eqmain_str_ec10_guess, text);
            }
            else if (*msg == 4 && (char)msg[2] == '\x1b')
            {
                done = true;
                strcpy(g_eqmain_str_ec10_guess, g_eqmain_drive_base_guess);
            }
freeMsg:
            eqmain_fn_10033568_guess(msg);
        }
        if (done)
        {
            eqmain_fn_10001700_guess(ctlRight);
            eqmain_fn_10001700_guess(ctlLeft);
            eqmain_fn_10008a00_guess(edit);
            eqmain_fn_10007a80_guess(bgDesc);
            eqmain_fn_10001c30_guess();
            return;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x10002B30
// Shows a message with an OK-only acknowledgement.
// __cdecl with one char * argument (the caller pops it); returns nothing. Calls 0x10001FF0 (eqmainRunDialog_guess) with
// the object at g_eqmain_ceb8_guess, the message, g_str_888_guess as the OK text and NULL as the cancel text (so only the OK button).
// Its byte result is ignored.
void __cdecl eqmainShowOkDialog_guess(char *message)
{
    eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, message, g_str_888_guess, 0);
}

// Three of these repeat identical defines further down, which come after this function in address order.
#define eqmain_fn_10033670_guess   EQ_CDECL(char *, 0x10033670, (char *, int))
#define eqmain_fn_10033730_guess   EQ_CDECL(int, 0x10033730, (char *, char *))
#define g_eqmain_str_55754_guess   EQ_GLOBAL(char *, 0x10055754)
#define g_eqmain_str_55758_guess   EQ_GLOBAL(char *, 0x10055758)
#define g_eqmain_str_55900_guess   EQ_GLOBAL(char *, 0x10055900)
#define g_eqmain_str_55904_guess   EQ_GLOBAL(char *, 0x10055904)
#define g_eqmain_str_55908_guess   EQ_GLOBAL(char *, 0x10055908)

// FUNCTION: EQMAIN 0x10002B50
// Lets the player change a password after validating the current entry and matching new entries.
// __cdecl with one char * argument (the caller pops it); returns a byte (callers read AL). Builds the chpass.bmp dialog on
// g_eqmain_ceb8_guess: pHVar5 (cancel, x 0x214) and pHVar6 (OK, x 0x6C), two texts (EQMain_FitFontSize_guess; the second takes the first's
// return), eqmain_draw_text_guess on each button, three labels and three edits (pHVar7..9, text at +0x20). Then it polls PumpMessagesAndEditText_guess
// (each message is freed). Type 0 on pHVar6 or pHVar9 (OK): strcmpi of the first edit's text with 0x1012565C must be 0,
// else g_eqmain_str_55758_guess is shown; then the second and third edits must each be 4+ characters, equal, with no ' or space,
// else g_eqmain_str_55754_guess is shown; then the second edit is strcpy'd to newPassword and the result is 1. Type 0 on pHVar5 (cancel)
// and ESC (type 4, 0x1B) return 0. Type 0 on pHVar7/pHVar8 and types 6 and 7 move the focus (EQMain_SetCachedEntry_guess).
unsigned char __cdecl eqmainChangePasswordDialog_guess(char *newPassword)
{
    bool done;
    unsigned char success;
    int *backgroundBitmap;
    int *pHVar5;
    int *pHVar6;
    int *pHVar7;
    int *pHVar8;
    int *pHVar9;
    int *_Memory;
    int initialFontSize;
    int buttonFontSize;

    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess((int *)g_eqmain_obj_guess);
    done = false;
    success = 0;
    backgroundBitmap = eqmain_fn_10007ae0_guess((char *)0x10046228, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0x6c, 0x32, (int)backgroundBitmap, 0, 0, backgroundBitmap[0], backgroundBitmap[1], 1);
    pHVar5 = (int *)eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x214, 0x15e, 0x18, 1);
    pHVar6 = (int *)eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x6c, 0x15e, 0x14, 1);
    initialFontSize = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_eqmain_text_5848_guess, 0x19, 0);
    buttonFontSize = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_str_888_guess, initialFontSize, 0);
    eqmain_fn_100017b0_guess(pHVar5, 0, g_eqmain_text_5848_guess, buttonFontSize, 0);
    eqmain_fn_100017b0_guess(pHVar6, 0, g_str_888_guess, buttonFontSize, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0xa1, 0x6f, 0xf3f2f2, g_eqmain_str_55900_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0xa1, 0xaf, 0xf3f2f2, g_eqmain_str_55904_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0xa1, 0xef, 0xf3f2f2, g_eqmain_str_55908_guess, 0, 0);
    pHVar7 = (int *)eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0xa1, 0x82, 0x118, 0x10, 0x11, 0, 0, 0xe8c9cc, 0);
    pHVar8 = (int *)eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0xa1, 0xc2, 0x118, 0x10, 0x11, 0, 0, 0xe8c9cc, 0);
    pHVar9 = (int *)eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0xa1, 0x102, 0x118, 0x10, 0x11, 0, 0, 0xe8c9cc, 0);
    eqmain_fn_100088f0_guess((int)pHVar7);
    eqmain_fn_100088f0_guess((int)pHVar8);
    eqmain_fn_100088f0_guess((int)pHVar9);
    eqmain_fn_10008910_guess(pHVar7);
    do
    {
        _Memory = eqmain_fn_10003bd0_guess();
        if (_Memory != 0)
        {
            switch (*_Memory)
            {
            case 4:
                if (*(char *)(_Memory + 2) == '\x1b')
                {
                    done = true;
                    success = 0;
                }
                break;
            case 7:
                if ((int *)_Memory[2] == pHVar7)
                {
                    eqmain_fn_10008910_guess(pHVar8);
                }
                else if ((int *)_Memory[2] == pHVar8)
                {
                    eqmain_fn_10008910_guess(pHVar9);
                }
                else if ((int *)_Memory[2] == pHVar9)
                {
                    eqmain_fn_10008910_guess(pHVar7);
                }
                break;
            case 6:
                if ((int *)_Memory[2] == pHVar9)
                {
                    eqmain_fn_10008910_guess(pHVar8);
                }
                else if ((int *)_Memory[2] == pHVar8)
                {
                    eqmain_fn_10008910_guess(pHVar7);
                }
                else if ((int *)_Memory[2] == pHVar7)
                {
                    eqmain_fn_10008910_guess(pHVar9);
                }
                break;
            case 0:
                if ((int *)_Memory[2] == pHVar6 || (int *)_Memory[2] == pHVar9)
                {
                    if (eqmain_fn_10033730_guess((char *)0x1012565C, (char *)pHVar7[8]) != 0)
                    {
                        eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, g_eqmain_str_55758_guess,
                                                 g_str_888_guess, 0);
                    }
                    else
                    {
                        eqmain_fn_10023c10_guess((char *)pHVar8[8]);
                        eqmain_fn_10023c10_guess((char *)pHVar9[8]);
                        if (strlen((char *)pHVar8[8]) >= 4 && strlen((char *)pHVar9[8]) >= 4 &&
                            eqmain_fn_10033730_guess((char *)pHVar8[8], (char *)pHVar9[8]) == 0 &&
                            eqmain_fn_10033670_guess((char *)pHVar8[8], '\'') == 0 &&
                            eqmain_fn_10033670_guess((char *)pHVar8[8], ' ') == 0 &&
                            eqmain_fn_10033670_guess((char *)pHVar9[8], '\'') == 0 &&
                            eqmain_fn_10033670_guess((char *)pHVar9[8], ' ') == 0)
                        {
                            done = true;
                            success = 1;
                            strcpy(newPassword, (char *)pHVar8[8]);
                            break;
                        }
                        eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, g_eqmain_str_55754_guess,
                                                 g_str_888_guess, 0);
                    }
                }
                else if ((int *)_Memory[2] == pHVar8)
                {
                    eqmain_fn_10008910_guess(pHVar9);
                }
                else if ((int *)_Memory[2] == pHVar7)
                {
                    eqmain_fn_10008910_guess(pHVar8);
                }
                else if ((int *)_Memory[2] == pHVar5)
                {
                    success = 0;
                    done = true;
                }
                break;
            }
            eqmain_fn_10033568_guess(_Memory);
        }
        if (done)
        {
            eqmain_fn_10001700_guess(pHVar5);
            eqmain_fn_10001700_guess(pHVar6);
            eqmain_fn_10008a00_guess(pHVar7);
            eqmain_fn_10008a00_guess(pHVar8);
            eqmain_fn_10008a00_guess(pHVar9);
            eqmain_fn_10007a80_guess(backgroundBitmap);
            eqmain_fn_10001c30_guess();
            return success;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x10003000
// Presents the supplied message as wrapped text in the front-end dialog.
// __cdecl with one char * argument (the caller pops it); returns nothing. Calls 0x10001B60 (no arguments), then
// 0x10007CF0 with the object at g_eqmain_ceb8_guess. 0x10007AE0 gets "..." and 0x10 and returns a descriptor
// (backgroundBitmap). 0x10006C20 draws the object at (0xAA, 0x32) from backgroundBitmap with its first two dwords as the size, 0x10007A80
// gets backgroundBitmap, and 0x10008C10 makes a control at (0xDC, 100), 200x134, from the object, g_eqmain_dlg_arg_guess and 0. Its
// result goes into g_eqmain_dlg_ctl_guess, and 0x10009370 gets that control and src.
void __cdecl eqmainBuildDialog_guess(char *src)
{
    int *backgroundBitmap;

    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess((int *)g_eqmain_obj_guess);
    backgroundBitmap = eqmain_fn_10007ae0_guess((char *)0x100461B8, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0xaa, 0x32, (int)backgroundBitmap, 0, 0, *backgroundBitmap, backgroundBitmap[1], 1);
    eqmain_fn_10007a80_guess(backgroundBitmap);
    g_eqmain_dlg_ctl_guess = eqmain_fn_10008c10_guess(g_eqmain_obj_guess, 0xdc, 100, 200, 0x86, 0x14, 0x21a3c,
        g_eqmain_dlg_arg_guess, 0);
    eqmain_fn_10009370_guess(g_eqmain_dlg_ctl_guess, src);
    return;
}

// FUNCTION: EQMAIN 0x10003090
// Removes the dialog's wrapped text from the front-end screen.
// __stdcall, no arguments; returns nothing. Clears the chain table at g_eqmain_dlg_ctl_guess (cdecl 0x10008CE0, one argument, the
// caller pops it), then tail-jumps to 0x10001C30 (__stdcall, no arguments, pops nothing).
void __stdcall eqmainCleanupDialog_guess(void)
{
    eqmain_chain_table_free_guess(g_eqmain_dlg_ctl_guess);
    eqmain_fn_10001c30_guess();
    return;
}

#define eqmain_fn_10033730_guess   EQ_CDECL(int, 0x10033730, (char *, char *))
#define g_eqmain_str_55804_guess   EQ_GLOBAL(char *, 0x10055804)
#define g_eqmain_str_55900_guess   EQ_GLOBAL(char *, 0x10055900)
#define g_eqmain_str_55758_guess   EQ_GLOBAL(char *, 0x10055758)
#define g_eqmain_login_pin_buf_guess  EQ_ARRAY(char, 0x1012565C)
#define g_eqmain_str_46234_guess   EQ_ARRAY(char, 0x10046234)
#define g_eqmain_copybuf_guess   EQ_ARRAY(char, 0x100F4C10)

// FUNCTION: EQMAIN 0x100030B0
// Accepts a typed response only when it matches the expected front-end text.
// __stdcall, no arguments; returns a byte (callers read AL). Builds the choice dialog on g_eqmain_ceb8_guess: cancel.bmp
// (PackFsGetNamedImage_guess), two eqmain_create_button_guess controls (pHVar4, pHVar5), two EQMain_FitFontSize_guess texts (the second takes the first's
// return), eqmain_draw_text_guess on each control, EQMain_DrawTextRun_guess, EQMain_CreateSurfaceRecord_guess (textEntry; its [8] is the name __strcmpi checks),
// EQMain_CreateRecord_guess, then g_eqmain_str_55804_guess is copied to g_eqmain_copybuf_guess for WrapTextLines_guess. Then it polls PumpMessagesAndEditText_guess (NULL when
// there is no message; each one is freed): word 0 with pHVar5 at +8 returns 1 when the name matches, else eqmainRunDialog_guess
// shows g_eqmain_str_55758_guess with the OK text g_str_888_guess; word 0 with pHVar4 at +8 returns 0; word 4 with 0x1B at +8 (ESC)
// returns 0; anything else polls on.
unsigned char __stdcall eqmainChoiceDialog_guess(void)
{
    bool done;
    unsigned char accepted;
    int *backgroundBitmap;
    void *pHVar4;
    void *pHVar5;
    int *textEntry;
    int *textBlock;
    int *_Memory;
    int initialFontSize;
    int buttonFontSize;
    int compareResult;

    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess((int *)g_eqmain_obj_guess);
    done = false;
    accepted = 0;
    backgroundBitmap = eqmain_fn_10007ae0_guess(g_eqmain_str_46234_guess, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_obj_guess, 0x6c, 0x32, (int)backgroundBitmap, 0, 0, backgroundBitmap[0], backgroundBitmap[1], 1);
    pHVar4 = eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x214, 0x15e, 0x18, 1);
    pHVar5 = eqmain_fn_10001520_guess(g_eqmain_obj_guess, 0x6c, 0x15e, 0x14, 1);
    initialFontSize = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_eqmain_text_a_guess, 0x19, 0);
    buttonFontSize = eqmain_fn_100080b0_guess(g_eqmain_obj_guess, 0x64, 0x22, g_eqmain_text_b_guess, initialFontSize, 0);
    eqmain_fn_100017b0_guess(pHVar4, 0, g_eqmain_text_a_guess, buttonFontSize, 0);
    eqmain_fn_100017b0_guess(pHVar5, 0, g_eqmain_text_b_guess, buttonFontSize, 0);
    eqmain_fn_100082c0_guess(g_eqmain_obj_guess, 0xa1, 0xf2, 0xf3f2f2, g_eqmain_str_55900_guess, 0, 0);
    textEntry = (int *)eqmain_fn_10008460_guess(g_eqmain_obj_guess, 0xa1, 0x104, 0x118, 0x10, 0x11, 0, 0, 0xe8c9cc, 0);
    eqmain_fn_100088f0_guess((int)textEntry);
    eqmain_fn_10008910_guess(textEntry);
    textBlock = eqmain_fn_10008c10_guess(g_eqmain_obj_guess, 0x9d, 0x6f, 0x14a, 0x86, 0x14, 0xf3f2f2, g_eqmain_dlg_arg_guess, 0);
    strcpy(g_eqmain_copybuf_guess, g_eqmain_str_55804_guess);
    eqmain_fn_10009370_guess(textBlock, g_eqmain_copybuf_guess);
    do {
        _Memory = eqmain_fn_10003bd0_guess();
        if (_Memory != 0) {
            if (*_Memory != 0) {
                if (*_Memory == 4 && (char)_Memory[2] == '\x1b') {
                    done = true;
                    accepted = 0;
                }
            }
            else {
                if ((void *)_Memory[2] == pHVar5) {
                    compareResult = eqmain_fn_10033730_guess(g_eqmain_login_pin_buf_guess, (char *)textEntry[8]);
                    if (compareResult != 0) {
                        eqmain_fn_10001ff0_guess((int *)g_eqmain_obj_guess, g_eqmain_str_55758_guess,
                                                 g_str_888_guess, 0);
                    }
                    else {
                        done = true;
                        accepted = 1;
                    }
                }
                else if ((void *)_Memory[2] == pHVar4) {
                    accepted = 0;
                    done = true;
                }
            }
            eqmain_fn_10033568_guess(_Memory);
        }
    } while (!done);
    eqmain_fn_10001700_guess(pHVar4);
    eqmain_fn_10001700_guess(pHVar5);
    eqmain_fn_10008a00_guess(textEntry);
    eqmain_fn_10007a80_guess(backgroundBitmap);
    eqmain_chain_table_free_guess(textBlock);
    eqmain_fn_10001c30_guess();
    return accepted;
}

// eqmain.dll's import slots (IAT 0x100400xx and 0x100402xx), called through the slot as the original does, and the
// DirectX/COM signatures eqmainProbeDirectX_guess calls through vtables.
typedef void *(__stdcall *eqmainLoadLibraryA_guess_t)(const char *);
typedef void *(__stdcall *eqmainGetProcAddress_guess_t)(void *, const char *);
typedef int (__stdcall *eqmainFreeLibrary_guess_t)(void *);
typedef void (__stdcall *eqmainOutputDebugStringA_guess_t)(const char *);
typedef int (__stdcall *eqmainCoInitialize_guess_t)(void *);
typedef int (__stdcall *eqmainCoCreateInstance_guess_t)(const void *, void *, int, const void *, void **);
typedef void (__stdcall *eqmainCoUninitialize_guess_t)(void);
typedef int (__stdcall *eqmainDirectDrawCreate_guess_t)(void *, void **, void *);
typedef int (__stdcall *eqmainDirectDrawCreateEx_guess_t)(void *, void **, const void *, void *);
typedef int (__stdcall *eqmainComQI_guess_t)(void *, const void *, void **);
typedef unsigned long (__stdcall *eqmainComRelease_guess_t)(void *);
typedef int (__stdcall *eqmainDDSetCoop_guess_t)(void *, void *, unsigned long);
typedef int (__stdcall *eqmainDDCreateSurface_guess_t)(void *, void *, void **, void *);
#define g_eqmain_pfn_LoadLibraryA_guess         EQ_GLOBAL(eqmainLoadLibraryA_guess_t, 0x100400A4)
#define g_eqmain_pfn_GetProcAddress_guess       EQ_GLOBAL(eqmainGetProcAddress_guess_t, 0x100400A0)
#define g_eqmain_pfn_FreeLibrary_guess          EQ_GLOBAL(eqmainFreeLibrary_guess_t, 0x1004009C)
#define g_eqmain_pfn_OutputDebugStringA_guess   EQ_GLOBAL(eqmainOutputDebugStringA_guess_t, 0x10040098)
#define g_eqmain_pfn_CoInitialize_guess         EQ_GLOBAL(eqmainCoInitialize_guess_t, 0x100402A8)
#define g_eqmain_pfn_CoCreateInstance_guess     EQ_GLOBAL(eqmainCoCreateInstance_guess_t, 0x100402AC)
#define g_eqmain_pfn_CoUninitialize_guess       EQ_GLOBAL(eqmainCoUninitialize_guess_t, 0x100402B0)
// slot n of the vtable that the COM object obj points at (its first dword is the vtable pointer)
#define eqmain_com_slot_guess(obj, n)           ((*(void ***)(obj))[n])

// FUNCTION: EQMAIN 0x10003340
// Probes DirectDraw, DirectInput, DirectMusic, and Direct3D components and returns a status code.
// __stdcall, no arguments; returns an int. Probes what the game needs from DirectX: DirectDrawCreate on ddraw.dll
// (then QueryInterface for IDirectDraw2), the DirectInputCreateA address on dinput.dll, SetCooperativeLevel and a
// primary surface, DirectMusic through CoCreateInstance, DirectDrawCreateEx and d3d8.dll. Each interface is released
// as it is done with, and the modules are freed on the way out. Returns 0 when ddraw.dll, DirectDrawCreate,
// SetCooperativeLevel or CreateSurface fails; 0x100 (IDirectDraw2), 0x200 (dinput.dll), 0x300 and 0x500 (the two
// QueryInterface calls on the primary surface), 0x600 (DirectMusic), 0x601 (DirectDrawCreateEx), 0x700 (d3d8.dll);
// 0x800 when all of them load. 0x400 is never returned. A dinput.dll failure returns without freeing ddraw.dll.
int __stdcall eqmainProbeDirectX_guess(void)
{
    void *comIfaces[6];      // 0x00..0x17: [0] IDirectDraw, [1] primary surface, [2] DirectMusic, [3] IDirectDraw2, [4], [5]
    void *surfaceIface;         // 0x18: QueryInterface result for the primary surface (IID 0x10040380)
    void *hModule;
    void *pFVar1;
    void *pHVar3;
    int iVar2;

    comIfaces[0] = 0;
    comIfaces[3] = 0;
    comIfaces[1] = 0;
    surfaceIface = 0;
    comIfaces[4] = 0;
    hModule = g_eqmain_pfn_LoadLibraryA_guess((const char *)0x10046394);
    if (hModule == 0)
    {
        return 0;
    }
    pFVar1 = g_eqmain_pfn_GetProcAddress_guess(hModule, (const char *)0x10046380);
    if (pFVar1 == 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x10046360);
        return 0;
    }
    iVar2 = ((eqmainDirectDrawCreate_guess_t)pFVar1)(0, comIfaces, 0);
    if (iVar2 < 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x10046348);
        return 0;
    }
    iVar2 = ((eqmainComQI_guess_t)eqmain_com_slot_guess(comIfaces[0], 0))(comIfaces[0], (const void *)0x10040330, &comIfaces[3]);
    if (iVar2 < 0)
    {
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x10046330);
        return 0x100;
    }
    ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[3], 2))(comIfaces[3]);
    pHVar3 = g_eqmain_pfn_LoadLibraryA_guess((const char *)0x10046324);
    if (pHVar3 == 0)
    {
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x10046304);
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        return 0x200;
    }
    pFVar1 = g_eqmain_pfn_GetProcAddress_guess(pHVar3, (const char *)0x100462F0);
    if (pFVar1 == 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(pHVar3);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x100462C8);
        return 0x200;
    }
    g_eqmain_pfn_FreeLibrary_guess(pHVar3);
    int ddsd[27] = {0};     // 0x1C..0x87: DDSURFACEDESC, 0x6C bytes: dwSize [0], dwFlags [1], ddsCaps.dwCaps [26]
    ddsd[0] = 0x6c;
    ddsd[1] = 1;
    ddsd[26] = 0x200;
    iVar2 = ((eqmainDDSetCoop_guess_t)eqmain_com_slot_guess(comIfaces[0], 20))(comIfaces[0], 0, 8);
    if (iVar2 < 0)
    {
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x100462AC);
        return 0;
    }
    iVar2 = ((eqmainDDCreateSurface_guess_t)eqmain_com_slot_guess(comIfaces[0], 6))(comIfaces[0], ddsd, &comIfaces[1], 0);
    if (iVar2 < 0)
    {
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x10046290);
        return 0;
    }
    iVar2 = ((eqmainComQI_guess_t)eqmain_com_slot_guess(comIfaces[1], 0))(comIfaces[1], (const void *)0x10040380, &surfaceIface);
    if (iVar2 < 0)
    {
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x300;
    }
    iVar2 = ((eqmainComQI_guess_t)eqmain_com_slot_guess(comIfaces[1], 0))(comIfaces[1], (const void *)0x10040390, &comIfaces[4]);
    if (iVar2 < 0)
    {
        ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x500;
    }
    ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[1], 2))(comIfaces[1]);
    ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[0], 2))(comIfaces[0]);
    comIfaces[2] = 0;
    g_eqmain_pfn_CoInitialize_guess(0);
    iVar2 = g_eqmain_pfn_CoCreateInstance_guess((const void *)0x100407A0, 0, 1, (const void *)0x100407D0, &comIfaces[2]);
    if (iVar2 < 0)
    {
        g_eqmain_pfn_OutputDebugStringA_guess((const char *)0x1004626C);
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x600;
    }
    ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[2], 2))(comIfaces[2]);
    g_eqmain_pfn_CoUninitialize_guess();
    pFVar1 = g_eqmain_pfn_GetProcAddress_guess(hModule, (const char *)0x10046258);
    if (pFVar1 == 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x601;
    }
    iVar2 = ((eqmainDirectDrawCreateEx_guess_t)pFVar1)(0, &comIfaces[5], (const void *)0x10040350, 0);
    if (iVar2 < 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x601;
    }
    ((eqmainComRelease_guess_t)eqmain_com_slot_guess(comIfaces[5], 2))(comIfaces[5]);
    pHVar3 = g_eqmain_pfn_LoadLibraryA_guess((const char *)0x1004624C);
    if (pHVar3 == 0)
    {
        g_eqmain_pfn_FreeLibrary_guess(hModule);
        return 0x700;
    }
    g_eqmain_pfn_FreeLibrary_guess(hModule);
    g_eqmain_pfn_FreeLibrary_guess(pHVar3);
    return 0x800;
}

// FUNCTION: EQMAIN 0x100036A0
// Sets up the front-end interface for login and server selection.
// __cdecl, three arguments; returns the object at 0x1004CEB8. Stores the arguments in 0x1004CE70, 0x1004CE74 and
// 0x1004CEB4 and clears the byte at 0x1004CE78. The object from 0x10003700 goes to 0x1004CEB8; then 0x10003A00 and
// 0x10006B90(object, 0) run, and the same pair again. No caller reads EAX, the original still returns the object.
int * __cdecl eqmainStartUi_guess(int value, int value2, int value3)
{
    g_eqmain_ui_arg1_guess = value;
    g_eqmain_ui_arg2_guess = value2;
    g_eqmain_ui_arg3_guess = value3;
    g_eqmain_flag_ce78_guess = 0;
    g_eqmain_obj_guess = (int)eqmain_fn_10003700_guess();
    eqmain_fn_10003a00_guess();
    eqmain_fn_10006b90_guess(g_eqmain_obj_guess, 0);
    eqmain_fn_10003a00_guess();
    eqmain_fn_10006b90_guess(g_eqmain_obj_guess, 0);
    return (int *)g_eqmain_obj_guess;
}
