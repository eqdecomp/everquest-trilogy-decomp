// unit_1001DC50.cpp - eqmain.dll, the address run 0x1001DC50..0x10021420 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"
#include <stdio.h>
#include <string.h>

// eqfecfg.cfg loader object (file-local stand-in). Only the fields EqfeCfgLoad_guess touches are named; 0x04 and 0x10
// are never written by it.
struct EqfeCfg_guess
{
    void *vtable;       // 0x00  PTR_FUN_10041f70 (eqmain.dll .rdata)
    unsigned int f04;   // 0x04
    FILE *file;         // 0x08  FUN_1003407f's result (0 or -1 when the open failed)
    void *buf;          // 0x0c  zeroed buffer of size + 1 bytes; fread fills it
    unsigned int f10;   // 0x10
    int size;           // 0x14  ftell after SEEK_END (1 for an empty file)
};

// eqmain.dll's name table (file-local stand-in): 0x118-byte records, the name at +0
struct NameRec_guess
{
    char name[0x118];   // 0x00
};

// eqmain.dll's text buffer (file-local stand-in). Only 0x0c..0x14 are used by CompactText_guess; 0x00..0x0b are not touched.
struct TextBuf_guess
{
    unsigned int unk00[3];  // 0x00
    char *text;             // 0x0c  the text; replaced by buf
    char *buf;              // 0x10  malloc'd copy of the text
    int len;                // 0x14  strlen of the text (set by the function)
};

// This image's addresses for the function below (file-local). The CRT's __strcmpi is at 0x10033730.
#define g_name_count_guess      EQ_GLOBAL(short, 0x100F1D26)            // g_name_count_guess: records in the table
#define g_name_table_guess      EQ_ARRAY(NameRec_guess, 0x100ECAD0)     // g_name_table_guess: the records
#define g_find_name_guess       EQ_ARRAY(char, 0x100F1D30)              // g_find_name_guess: the name searched for
#define g_selected_obj_guess    EQ_GLOBAL(char *, 0x100F10D8)           // g_selected_obj_guess: object, int at +0x34
#define g_name_sort_mode_guess  EQ_GLOBAL(short, 0x100482CC)            // g_name_sort_mode_guess: 0 or 1 picks the sort
#define g_name_sorted_guess     EQ_GLOBAL(char, 0x100F1D28)             // g_name_sorted_guess: set to 1 on every path
#define g_load_enabled_guess    EQ_GLOBAL(char, 0x100F1D23)             // g_load_enabled_guess: nonzero runs the load
#define g_test_flag_guess       EQ_GLOBAL(char, 0x100F1D20)             // g_eqmain_flag_1d20_guess: 1 when the text has "test"
#define g_news_path_guess       EQ_ARRAY(char, 0x10055530)              // g_news_path_guess: lowercased, searched for "test"
#define g_str_test_guess       EQ_ARRAY(char, 0x100482C0)              // g_str_test_guess: "test"
#define g_word_after_guess      EQ_GLOBAL(unsigned int, 0x100F1D2C)     // g_word_after_guess: a stream dword after the names
#define g_str_eca4c_guess       EQ_ARRAY(char, 0x100ECA4C)              // g_str_eca4c_guess: a string from the stream
#define g_str_ec9cc_guess       EQ_ARRAY(char, 0x100EC9CC)              // g_str_ec9cc_guess: a string from the stream
#define g_state_guess           EQ_GLOBAL(unsigned int, 0x100F1160)     // DAT_100f1160: the argument of ClassifyFlags_guess

// Match build only: eqd_match_syms.h has no declaration for 0x100250c0, which LoadNameTable_guess calls (EQ_STDCALL).
void __cdecl eqd_fn_0x100250C0(void);

#define g_name_str_guess        EQ_ARRAY(char, 0x100ECB50)              // g_name_str_guess: record i's name, 0x118 apart (+0x80)

// The options file's addresses for SaveOrLoadNameState_guess (0x1001e150): its name, its two fopen modes and the strings it copies.
#define g_ss_opt_guess          EQ_ARRAY(char, 0x10048418)              // g_ss_opt_guess: "ss.opt", the options file
#define g_eqmain_fopen_mode_guess       EQ_ARRAY(char, 0x100480A4)              // g_eqmain_fopen_mode_guess: fopen mode for a load (presumably "rb")
#define g_str_10047f74_guess      EQ_ARRAY(char, 0x10047F74)              // DAT_10047f74: fopen mode for a save (presumably "wb")
#define g_time_text_guess       EQ_ARRAY(char, 0x100482D0)              // the "..." text: a load overwrites it
#define g_eqmain_drive_base_guess        EQ_ARRAY(char, 0x1004CE40)              // g_eqmain_drive_base_guess: a string a load copies to g_time_text_guess

// FUNCTION: EQMAIN 0x1001DC50
// Finds the name-list record matching the requested name.
// ORIGINAL FILE: unknown
// __cdecl, callee pops 0. Returns the address of the first record whose name equals the string at param_1 (strcmpi),
// else 0. Records are 0x118 bytes from 0x100ecad0 and the names 0x118 bytes from 0x100ecb50, i.e. +0x80 into the
// record: NameRec_guess has the name at +0, which this function contradicts. The count is g_name_count_guess; <= 0 returns 0.
unsigned int *FindNameRecord_guess(char *name)
{
    int i;
    char *str1;

    i = 0;
    if (g_name_count_guess > 0)
    {
        str1 = g_name_str_guess;
        do
        {
            if (EQ_CDECL(int, 0x10033730, (char *, char *))(str1, name) != 0)
            {
                i++;
                str1 += 0x118;
            }
            else
            {
                return EQ_ARRAY(unsigned int, 0x100ECAD0) + i * 0x46;
            }
        } while (i < g_name_count_guess);
    }
    return (unsigned int *)0;
}

// This image's addresses for RebuildNameList_guess (file-local). g_rec_value_guess is the record table's +0x100 field:
// record i's value, with its flag at +0x108 (the records are the 0x118-byte ones at 0x100ecad0).
#define g_list_obj_guess        EQ_GLOBAL(char *, 0x100EC9C8)           // g_list_obj_guess: second object, +0x2c set to the saved index
#define g_rec_value_guess       EQ_ARRAY(int, 0x100ECBD0)               // g_rec_value_guess: record i's value at +0x100, 0x118 apart
#define g_str_a04_guess         EQ_GLOBAL(char *, 0x10055A04)           // g_str_a04_guess: the text for a value of -1
#define g_str_a08_guess         EQ_GLOBAL(char *, 0x10055A08)           // g_str_a08_guess: the text for a value of -2
#define g_value_fmt_guess       EQ_ARRAY(char, 0x10048414)              // g_value_fmt_guess: the format any other value is sprintf'd with
#define g_str_a30_guess         EQ_GLOBAL(char *, 0x10055A30)           // g_str_a30_guess: the string EQMain_DrawTextRun_guess gets
#define g_ui_obj_guess          EQ_GLOBAL(char *, 0x1004CEB8)           // g_eqmain_ceb8_guess: the draw object (460 xrefs)
#define g_ctl_guess             EQ_GLOBAL(char *, 0x100F10D4)           // g_ctl_guess: a control, +0x64 and +0x68 set
#define g_flag_258c8_guess      EQ_GLOBAL(int, 0x101258C8)              // g_eqmain_value_58c8_guess: 0 lets the message run
#define g_shown_guess           EQ_GLOBAL(char, 0x100F1D24)             // g_shown_guess: set to 1 once the message has run
#define g_str_888_guess         EQ_GLOBAL(char *, 0x10055888)           // g_str_888_guess: eqmainRunDialog_guess's 3rd argument
#define g_str_910_guess         EQ_GLOBAL(char *, 0x10055910)           // g_str_910_guess: eqmainRunDialog_guess's 2nd argument

// FUNCTION: EQMAIN 0x1001DCB0
// Rebuilds the front-end's selectable name list and its associated value list.
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0), no return value (EAX on return is leftover; no caller reads it). Refills the two
// lists from the g_name_count_guess records after RemoveAllOwnedRecords_guess on both objects. The first object (0x100f10d8) gets each name,
// with 0x6410 when the record's flag is set (anyFlagged then true) and 0x22d69 when not; the second (0x100ec9c8) gets each
// value's text: -1 and -2 pick the strings at 0x10055a04 and 0x10055a08, any other value is sprintf'd with g_value_fmt_guess.
// 16 or more records also call EQMain_DrawTextRun_guess (7 dwords pushed; the last two are 0). Both objects get their saved +0x2c back
// (a negative one is clamped to 0), DrawNamedRows_guess runs on both, and the control at 0x100f10d4 gets +0x64 = (+0x24 - +0x30)
// + 1 and +0x68 = +0x2c before RefreshSlider_guess. Then the name of record +0x34 (of 0x100f10d8) is strcpy'd to 0x100f1d30 and,
// when anyFlagged is set, g_eqmain_value_58c8_guess is 0 and g_shown_guess is clear, eqmainRunDialog_guess runs on 0x1004ceb8 and g_shown_guess becomes 1.
void __stdcall RebuildNameList_guess(void)
{
    int recordValue;
    bool anyFlagged;
    int tmp;
    int *recordPtr;
    int index;
    char text[16];

    tmp = *(int *)(g_selected_obj_guess + 0x2c);
    if (tmp < 0)
    {
        tmp = 0;
    }
    EQ_CDECL(void, 0x10005D80, (char *))(g_selected_obj_guess);
    EQ_CDECL(void, 0x10005D80, (char *))(g_list_obj_guess);
    anyFlagged = false;
    index = 0;
    if (0 < g_name_count_guess)
    {
        recordPtr = g_rec_value_guess;
        do
        {
            if (recordPtr[2] != 0)
            {
                EQ_CDECL(unsigned int *, 0x10005CC0, (char *, unsigned int, char *, unsigned int, unsigned int))(
                    g_selected_obj_guess, 0x13f, (char *)(recordPtr - 0x40), 0, 0x6410);
                anyFlagged = true;
            }
            else
            {
                EQ_CDECL(unsigned int *, 0x10005CC0, (char *, unsigned int, char *, unsigned int, unsigned int))(
                    g_selected_obj_guess, 0x13f, (char *)(recordPtr - 0x40), 0, 0x22d69);
            }
            recordValue = *recordPtr;
            if (recordValue == -1)
            {
                EQ_CDECL(unsigned int *, 0x10005CC0, (char *, unsigned int, char *, unsigned int, unsigned int))(
                    g_list_obj_guess, 0x50, g_str_a04_guess, 0, 0x1818f0);
            }
            else if (recordValue == -2)
            {
                EQ_CDECL(unsigned int *, 0x10005CC0, (char *, unsigned int, char *, unsigned int, unsigned int))(
                    g_list_obj_guess, 0x50, g_str_a08_guess, 0, 0xc81000);
            }
            else
            {
                EQ_CDECL(int, 0x10034117, (char *, char *, ...))(text, g_value_fmt_guess, recordValue);
                EQ_CDECL(unsigned int *, 0x10005CC0, (char *, unsigned int, char *, unsigned int, unsigned int))(
                    g_list_obj_guess, 0x50, text, 0, 0x22d69);
            }
            index = index + 1;
            recordPtr = recordPtr + 0x46;
        } while (index < g_name_count_guess);
        if (index >= 0x10)
        {
            EQ_CDECL(unsigned int *, 0x100082C0, (char *, unsigned int, unsigned int, unsigned int, char *, int, int))(
                g_ui_obj_guess, 0x82, 0x139, 0x1818f0, g_str_a30_guess, 0, 0);
        }
    }
    *(int *)(g_selected_obj_guess + 0x2c) = tmp;
    *(int *)(g_list_obj_guess + 0x2c) = tmp;
    EQ_CDECL(void, 0x10005A00, (char *))(g_selected_obj_guess);
    EQ_CDECL(void, 0x10005A00, (char *))(g_list_obj_guess);
    *(int *)(g_ctl_guess + 0x64) = (*(int *)(g_selected_obj_guess + 0x24) - *(int *)(g_selected_obj_guess + 0x30)) + 1;
    *(int *)(g_ctl_guess + 0x68) = *(int *)(g_selected_obj_guess + 0x2c);
    EQ_CDECL(void, 0x100067E0, (char *))(g_ctl_guess);
    tmp = g_flag_258c8_guess;
    strcpy(g_find_name_guess, (char *)(EQ_ARRAY(unsigned int, 0x100ECAD0) + *(int *)(g_selected_obj_guess + 0x34) * 0x46));
    if (((tmp == 0) && (anyFlagged)) && (g_shown_guess == 0))
    {
        EQ_CDECL(unsigned char, 0x10001FF0, (char *, char *, char *, char *))(g_ui_obj_guess, g_str_910_guess,
                                                                              g_str_888_guess, 0);
        g_shown_guess = 1;
    }
}

// FUNCTION: EQMAIN 0x1001dee0
// Alphabetizes the front-end name list and reselects the saved name.
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0), no return value (EAX on return is leftover: the last strcmpi result, the
// count or the object pointer; no caller reads it). Bubble sort of the records by strcmpi: each record swaps with
// its successor when the successor's name sorts first, so equal names keep their order. Then the first record whose
// name equals the string at 0x100f1d30 has its index stored at +0x34 of the object at 0x100f10d8; nothing is stored
// when none matches, or when the count is 0 or less.
void __stdcall SortNameTable_guess(void)
{
    short i;
    short j;
    short k;
    char *current;
    char *next;
    char tmp[0x118];

    for (i = 0; i < g_name_count_guess; i++)
    {
        for (j = 0; j < g_name_count_guess - 1; j++)
        {
            current = g_name_table_guess[j].name;
            next = EQ_ARRAY(NameRec_guess, 0x100ECBE8)[j].name;
            if (EQ_CDECL(int, 0x10033730, (char *, char *))(next, current) < 0)
            {
                memcpy(tmp, current, 0x118);
                memcpy(current, next, 0x118);
                memcpy(next, tmp, 0x118);
            }
        }
    }
    for (k = 0; k < g_name_count_guess; k++)
    {
        if (EQ_CDECL(int, 0x10033730, (char *, char *))(g_find_name_guess, g_name_table_guess[k].name) == 0)
        {
            *(int *)(g_selected_obj_guess + 0x34) = k;
            break;
        }
    }
}

// FUNCTION: EQMAIN 0x1001dfe0
// Sorts the server list by the number at +0x100 (flag-1 records moved ahead), then reselects the saved name's row
// @early-stop: count-register allocation and search back-edge layout remain different (26 structural diffs).
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0), no return value (EAX on return is leftover; the caller at 0x1001e120 returns
// straight after the call, so it may pass EAX on). Records whose int at +0x108 is 1 get -3000 on their sort key (the
// int at +0x100) before the sort and +3000 back after it. Bubble sort of the records by that key: a pass swaps each
// record with its successor when the successor's key is smaller, so equal keys keep their order. Then the first record
// whose name equals the string at 0x100f1d30 (__strcmpi) has its index stored at +0x34 of the object at 0x100f10d8;
// nothing is stored when none matches. Nothing is biased, sorted or searched when the count is 0 or less.
void __stdcall SortNameTableByKey_guess(void)
{
    int *recordValuePtr;
    int tmp;
    int row;
    short count;
    short index;
    unsigned int *puVar7;
    unsigned int *puVar8;
    int passesLeft;
    unsigned int swapRecord[70];

    count = g_name_count_guess;
    if (0 < count)
    {
        recordValuePtr = g_rec_value_guess;
        tmp = (int)count;
        do
        {
            if (recordValuePtr[2] == 1)
                *recordValuePtr = *recordValuePtr + -3000;
            recordValuePtr = recordValuePtr + 0x46;
            tmp = tmp + -1;
        } while (tmp != 0);
    }
    if (0 < count)
    {
        passesLeft = (int)count;
        tmp = passesLeft + -1;
        do
        {
            index = 0;
            if (0 < tmp)
            {
                row = 0;
                do
                {
                    if ((int)g_rec_value_guess[row * 0x46] > (int)EQ_ARRAY(int, 0x100ECCE8)[row * 0x46])
                    {
                        puVar7 = EQ_ARRAY(unsigned int, 0x100ECAD0) + row * 0x46;
                        puVar8 = swapRecord;
                        memcpy(puVar8, puVar7, 0x118);
                        puVar7 = EQ_ARRAY(unsigned int, 0x100ECBE8) + row * 0x46;
                        puVar8 = EQ_ARRAY(unsigned int, 0x100ECAD0) + row * 0x46;
                        memcpy(puVar8, puVar7, 0x118);
                        puVar7 = swapRecord;
                        puVar8 = EQ_ARRAY(unsigned int, 0x100ECBE8) + row * 0x46;
                        memcpy(puVar8, puVar7, 0x118);
                    }
                    index = index + 1;
                    row = (int)index;
                    count = g_name_count_guess;
                } while (row < tmp);
            }
            passesLeft = passesLeft + -1;
        } while (passesLeft != 0);
    }
    index = 0;
    while (0 < count)
    {
        tmp = EQ_CDECL(int, 0x10033730, (char *, char *))(
            g_find_name_guess,
            (char *)(EQ_ARRAY(unsigned int, 0x100ECAD0) + index * 0x46));
        if (tmp == 0)
        {
            *(int *)(g_selected_obj_guess + 0x34) = (int)index;
            break;
        }
        index = index + 1;
        if (index < g_name_count_guess)
            continue;
        else
            break;
    }
    if (0 < g_name_count_guess)
    {
        recordValuePtr = g_rec_value_guess;
        tmp = (int)g_name_count_guess;
        do
        {
            if (recordValuePtr[2] == 1)
                *recordValuePtr = *recordValuePtr + 3000;
            recordValuePtr = recordValuePtr + 0x46;
            tmp = tmp + -1;
        } while (tmp != 0);
    }
}

// FUNCTION: EQMAIN 0x1001e120
// Chooses and applies the requested name-list ordering.
// __stdcall, no args, callee pops 0. The mode word at 0x100482cc picks the sort: 0 -> 0x1001dee0, 1 -> 0x1001dfe0
// (SortNameTableByKey_guess); any other mode calls nothing. Every path then sets the flag byte at 0x100f1d28 to 1.
void __stdcall SortNameTableByMode_guess(void)
{
    switch (g_name_sort_mode_guess)
    {
    case 0:
        EQ_STDCALL(void, 0x1001DEE0, (void))();
        break;
    case 1:
        EQ_STDCALL(void, 0x1001DFE0, (void))();
        g_name_sorted_guess = 1;
        return;
    }
    g_name_sorted_guess = 1;
}

// FUNCTION: EQMAIN 0x1001e150
// Stores or restores the name-list sort mode and selected name.
// ORIGINAL FILE: unknown
// __cdecl: one char arg, callee pops 0. Nonzero AL saves and zero loads the options file "ss.opt" (opened by
// FUN_1003407f in mode "wb" or "rb"; the mode strings are unnamed data, so their text is a guess). The file is one
// 0x10c-byte record: the int at +0 (a save writes the mode word 0x100482cc sign-extended; a load keeps its low word)
// and the 264 bytes after it (the name at 0x100f1d30). A load also copies the string at 0x1004ce40 over the message at
// 0x100482d0. Nothing is written or read when the file does not open.
void __cdecl SaveOrLoadNameState_guess(char save)
{
    int optionsRecord;
    char nameBuffer[264];
    FILE *f;

    if (save != 0)
    {
        memset(&optionsRecord, 0, 0x10c);
        optionsRecord = (int)g_name_sort_mode_guess;
        strcpy(nameBuffer, g_find_name_guess);
        f = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_ss_opt_guess, g_str_10047f74_guess);
        if (f != 0)
        {
            EQ_CDECL(size_t, 0x10034169, (void *, size_t, size_t, FILE *))(&optionsRecord, 0x10c, 1, f);
            EQ_CDECL(int, 0x100343B9, (FILE *))(f);
            return;
        }
    }
    else
    {
        f = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_ss_opt_guess, g_eqmain_fopen_mode_guess);
        if (f != 0)
        {
            EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, FILE *))(&optionsRecord, 0x10c, 1, f);
            EQ_CDECL(int, 0x100343B9, (FILE *))(f);
            g_name_sort_mode_guess = (short)optionsRecord;
            strcpy(g_find_name_guess, nameBuffer);
            strcpy(g_time_text_guess, g_eqmain_drive_base_guess);
        }
    }
}

// FUNCTION: EQMAIN 0x1001e270
// Classifies a front-end bitmask; the category's game meaning is unclear.
// ORIGINAL FILE: unknown
// __cdecl: one stack arg, callee pops 0. Only the bits of 0x6d count (bits 1, 4 and 7 are ignored), and any bit
// above the low byte set gives 7. The masked value maps: 0 -> 0; 1, 4 -> 1; 8, 0x20 -> 2; 9, 0xc, 0x21, 0x24 -> 6;
// anything else (5, 0xd, 0x25 and up, ...) -> 7.
int __cdecl ClassifyFlags_guess(unsigned int flags)
{
    switch (flags & 0xffffff6d)
    {
    case 0:
        return 0;
    case 1:
    case 4:
        return 1;
    case 8:
    case 0x20:
        return 2;
    case 9:
    case 0xc:
    case 0x21:
    case 0x24:
        return 6;
    default:
        return 7;
    }
}

// FUNCTION: EQMAIN 0x1001e2f0
// Selects a fixed constant for a flag category; its use is unclear.
// ORIGINAL FILE: unknown
// __cdecl: one stack arg, callee pops 0. Takes the class of the arg from ClassifyFlags_guess (0x1001E270) and returns
// a constant for it: 0 -> 0; 1 -> 0x1818f0; 2 -> 0xc81000; 3 -> 0x7b3152; 4..6 -> 0x3cd9f5; 7 -> 0xf3f2f2. Any other
// value (the bound check is unsigned) also gives 0xf3f2f2. Case 7 keeps its own label and body: merged into default,
// VC6 would bound the table at 6 instead of 7.
unsigned int __cdecl FlagClassConst_guess(unsigned int flags)
{
    int cls;

    cls = EQ_CDECL(int, 0x1001E270, (unsigned int))(flags);
    switch (cls)
    {
    case 0:
        return 0;
    case 1:
        return 0x1818f0;
    case 2:
        return 0xc81000;
    case 3:
        return 0x7b3152;
    case 4:
    case 5:
    case 6:
        return 0x3cd9f5;
    case 7:
        return 0xf3f2f2;
    default:
        return 0xf3f2f2;
    }
}

// This image's addresses for FlagClassValue_guess (file-local): the four .data DWORDs it returns.
#define g_class_value_1_guess   EQ_GLOBAL(unsigned int, 0x100559C8)     // g_class_value_1_guess: class 1
#define g_class_value_2_guess   EQ_GLOBAL(unsigned int, 0x100559CC)     // g_class_value_2_guess: class 2
#define g_class_value_3_guess   EQ_GLOBAL(unsigned int, 0x10055ACC)     // g_class_value_3_guess: class 3
#define g_class_value_4_guess   EQ_GLOBAL(unsigned int, 0x100559C4)     // g_class_value_4_guess: classes 4..7

// FUNCTION: EQMAIN 0x1001e350
// Selects the stored value associated with a flag category; its meaning is unclear.
// ORIGINAL FILE: unknown
// __cdecl: one stack arg, callee pops 0. Takes the class of the arg from ClassifyFlags_guess (0x1001E270) and returns
// the DWORD for it: 1 -> g_class_value_1_guess, 2 -> g_class_value_2_guess, 3 -> g_class_value_3_guess, 4..7 -> g_class_value_4_guess. Class 0 and any
// other value (the bound check is unsigned, so negatives too) return 0.
unsigned int __cdecl FlagClassValue_guess(unsigned int flags)
{
    int cls;

    cls = EQ_CDECL(int, 0x1001E270, (unsigned int))(flags);
    switch (cls)
    {
    case 1:
        return g_class_value_1_guess;
    case 2:
        return g_class_value_2_guess;
    case 3:
        return g_class_value_3_guess;
    case 4:
    case 5:
    case 6:
    case 7:
        return g_class_value_4_guess;
    case 0:
        return 0;
    default:
        return 0;
    }
}

// FUNCTION: EQMAIN 0x1001e3b0
// Maps a flag category to a compact numeric code; its game meaning is unclear.
// ORIGINAL FILE: unknown
// __cdecl: one stack arg, callee pops 0. Takes the class of the arg from ClassifyFlags_guess (0x1001E270) and returns
// a code for it: 0 or 3 -> 0x1a, 1 or 4 -> 0x1c, 2 or 5 -> 0x1b. Class 6 and any other value (the bound check is
// unsigned, so negatives too) return 0x12.
unsigned int __cdecl FlagClassCode_guess(unsigned int flags)
{
    int cls;

    cls = EQ_CDECL(int, 0x1001E270, (unsigned int))(flags);
    switch (cls)
    {
    case 0:
    case 3:
        return 0x1a;
    case 1:
    case 4:
        return 0x1c;
    case 2:
    case 5:
        return 0x1b;
    case 6:
        return 0x12;
    default:
        return 0x12;
    }
}

// This image's addresses for the function below (file-local).
#define g_expansion_a_guess         EQ_GLOBAL(unsigned int, 0x1005592C)     // g_expansion_a_guess: returned for classes 0 and 3
#define g_expansion_b_guess         EQ_GLOBAL(unsigned int, 0x10055930)     // g_expansion_b_guess: returned for classes 1 and 4
#define g_expansion_c_guess         EQ_GLOBAL(unsigned int, 0x100558CC)     // g_expansion_c_guess: returned for classes 2 and 5
#define g_expansion_d_guess         EQ_GLOBAL(unsigned int, 0x10055ABC)     // g_expansion_d_guess: returned for class 6
#define g_programmer_error_guess    EQ_ARRAY(char, 0x10048420)              // "..."

// FUNCTION: EQMAIN 0x1001e400
// Selects the expansion destination label for the current front-end state.
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0). Takes the class of DAT_100f1160 from ClassifyFlags_guess (0x1001E270) and returns
// the global for it: 0 or 3 -> g_expansion_a_guess, 1 or 4 -> g_expansion_b_guess, 2 or 5 -> g_expansion_c_guess, 6 -> g_expansion_d_guess. Any other
// value (the bound check is unsigned, so negatives too) passes the "..." string to FatalErrorExit_guess (the caller
// pops its argument) and returns 0.
unsigned int __stdcall GetExpansionDestinationString_guess(void)
{
    int cls;

    cls = EQ_CDECL(int, 0x1001E270, (unsigned int))(g_state_guess);
    switch (cls)
    {
    case 0:
    case 3:
        return g_expansion_a_guess;
    case 1:
    case 4:
        return g_expansion_b_guess;
    case 2:
    case 5:
        return g_expansion_c_guess;
    case 6:
        return g_expansion_d_guess;
    default:
        EQ_CDECL(void, 0x100278A0, (char *))(g_programmer_error_guess);
        return 0;
    }
}

// This image's addresses for the function below (file-local). The parent and choice globals are named again further down
// with the same text.
#define g_parent_guess              EQ_GLOBAL(unsigned int, 0x1004CEB8)     // g_eqmain_ceb8_guess: the window the screen is made in
#define g_choice_count_guess        EQ_GLOBAL(int, 0x100F1164)              // g_choice_count_guess: choices in the text (0x11 at most)
#define g_choice_text_guess         EQ_ARRAY(char, 0x100F1168)              // g_choice_text_guess: the text strtok cuts
#define g_eqmain_flag_ce78_guess          EQ_GLOBAL(unsigned char, 0x1004CE78)    // g_eqmain_flag_ce78_guess: 1 while the dialog runs
#define g_choice_result_guess       EQ_GLOBAL(int, 0x100482C8)              // g_choice_result_guess: the index picked
#define g_default_result_guess      EQ_GLOBAL(unsigned short, 0x1004814E)   // g_chat_status_guess: the result, unless the 0x12 event sets 3
#define g_text_buf_guess            EQ_ARRAY(char, 0x100F4C10)              // g_eqmain_copybuf_guess: the text the screen builds
#define g_banner_obj_guess          EQ_GLOBAL(char *, 0x100F115C)           // g_banner_obj_guess: the banner object, freed at the end
#define g_overlay_obj_guess         EQ_GLOBAL(char *, 0x100F10D0)           // DAT_100f10d0: the overlay row object
#define g_list_ctl_guess            EQ_GLOBAL(char *, 0x10055448)           // g_eqmain_blit_src_guess: the list control, freed at the end
#define g_pending_obj_guess         EQ_GLOBAL(char *, 0x10055474)           // g_pending_obj_guess: +0x408 is -2 or -1 for a pending screen
#define g_str_778_guess             EQ_GLOBAL(char *, 0x10055778)           // g_str_778_guess: the text for a pending value of -1
#define g_str_77c_guess             EQ_GLOBAL(char *, 0x1005577C)           // g_str_77c_guess: the text for a pending value of -2
#define g_str_914_guess             EQ_GLOBAL(char *, 0x10055914)           // g_str_914_guess: first argument of ClassMin_guess
#define g_str_9d0_guess             EQ_GLOBAL(char *, 0x100559D0)           // g_str_9d0_guess: first argument of ClassMin_guess (0x4a)
#define g_str_9d4_guess             EQ_GLOBAL(char *, 0x100559D4)           // g_str_9d4_guess: first argument of ClassMin_guess (a typed name)
#define g_str_9e0_guess             EQ_GLOBAL(char *, 0x100559E0)           // g_str_9e0_guess: the text of the 0x46 row
#define g_text_573c_guess           EQ_GLOBAL(char *, 0x1005573C)           // g_text_573c_guess: the text of the first list button
#define g_text_5744_guess           EQ_GLOBAL(char *, 0x10055744)           // g_text_5744_guess: the text the second list button is sized from
#define g_eqmain_text_5830_guess            EQ_GLOBAL(char *, 0x10055830)           // g_eqmain_text_5830_guess: text of the first 0x64 row
#define g_chat_button_text_2_guess            EQ_GLOBAL(char *, 0x10055838)           // g_chat_button_text_2_guess: text of the second 0x64 row
#define g_text_840_guess            EQ_GLOBAL(char *, 0x10055840)           // g_text_840_guess: text of the third 0x64 row
#define g_str_784_guess             EQ_GLOBAL(char *, 0x10055784)           // g_str_784_guess: the text the 0x50 event copies
#define g_fmt_788_guess             EQ_GLOBAL(char *, 0x10055788)           // g_fmt_788_guess: the format of the 0x49 message
#define g_eqmain_text_b_guess       EQ_GLOBAL(char *, 0x1005586C)           // g_eqmain_text_b_guess: eqmainRunDialog_guess's 3rd argument
#define g_eqmain_text_a_guess       EQ_GLOBAL(char *, 0x10055868)           // g_eqmain_text_a_guess: eqmainRunDialog_guess's 4th argument
#define g_sel_name_guess            EQ_ARRAY(char, 0x100554B0)              // g_sel_name_guess: the selected record's name
#define g_edit_name_guess           EQ_ARRAY(char, 0x100F10DC)              // g_edit_name_guess: the name box's text
#define g_str_84c4_guess            EQ_ARRAY(char, 0x100484C4)              // g_str_84c4_guess: the text the 0x47 event copies
#define g_eqmain_copy_c10_guess          EQ_ARRAY(char, 0x10104C10)              // g_eqmain_copy_c10_guess: the copy the 0x47 event makes
#define g_selserver_bmp_guess       EQ_ARRAY(char, 0x100484C8)              // "..."
#define g_selserver_overlay_guess   EQ_ARRAY(char, 0x100483FC)              // "..."
#define g_chekbox1_bmp_guess        EQ_ARRAY(char, 0x100461D8)              // "..."
#define g_chekbox2_bmp_guess        EQ_ARRAY(char, 0x100461E8)              // "..."
#define g_eqbutton1_bmp_guess       EQ_ARRAY(char, 0x10046118)              // "..."
#define g_eqbutton2_bmp_guess       EQ_ARRAY(char, 0x10046108)              // "..."
#define g_eqbutton3_bmp_guess       EQ_ARRAY(char, 0x100460F8)              // "..."
#define g_sbanner_bmp_guess         EQ_ARRAY(char, 0x10046064)              // "..."
#define g_error_1100_guess          EQ_ARRAY(char, 0x10048464)              // "..."

// FUNCTION: EQMAIN 0x1001E460
// Runs the server select screen: the sortable server list and its buttons; returns the next front-end state
// ORIGINAL FILE: unknown
// __stdcall, no args (callee pops 0). The server select screen: builds selserver.bmp, the overlay, two check boxes, the
// three list buttons and the server list (g_selected_obj_guess is the list object; its +0x34 is the selected record: the last record
// whose name is g_find_name_guess). Then it loops: RdpReceiveScreenDispatch_guess gives an event and PumpMessagesAndEditText_guess a message, which is freed.
// Events: 2 shows g_eqmain_copybuf_guess and ends with 0; 0x12 ends with 3; 0x46 builds the rows and the eqbutton dialog; 0x47 copies
// g_str_84c4_guess; 0x49 checks the typed name; 0x4a and 0x49 call ClassMin_guess on g_str_9d0_guess; 0x4b sets g_name_sorted_guess; 0x50
// shows g_str_784_guess; 0x52 shows the sbanner; 0x60 takes up to 4 choices. A message of type 0 picks a row or a button, type 1
// is the list, the check boxes or a list item, type 4 is ENTER or ESC. The loop ends when done is set; the result is the low
// word of local_9c.
unsigned short __stdcall RunServerSelectScreen_guess(void)
{
    bool done;             // set once the loop must end (BL)
    bool listPositioned;             // local_9d: set once a list record was taken
    int value;              // a strcmp result, then a message type or a class
    short index;            // the record index of the name search, then the list index
    int ev;                 // the event from RdpReceiveScreenDispatch_guess
    int nudge;              // 0x1e when g_text_5744_guess is over 16 characters, else 0
    char *pcVar10;          // the text of the pending screen, then the eqbutton text
    char *bannerPath;          // the sbanner text
    char *pCVar11;          // a text from FlagClassValue_guess or GetExpansionDestinationString_guess
    char *control;          // a record from FindNameRecord_guess, or a message's control
    char confirmed;             // eqmainRunDialog_guess's answer
    int *_Memory;           // the message from PumpMessagesAndEditText_guess
    int pHVar7;             // the message's control, _Memory[2]
    int eqButton;           // the eqbutton dialog (eqmain_create_widget_guess), 0 until built
    int thirdButton;           // the 0x8a row
    int extraButton;           // the 0xb row, built when g_word_after_guess is set
    int stateButton;           // the 0x8e row
    int checkedBox;           // the list item eqmainSelectUnitInGroup_guess picks
    int tmp;           // EQMain_FitFontSize_guess's result for the 0xd2 text
    long lastRefresh;          // time_t: the time of the last status refresh
    long now;          // time_t: the time now
    int fitResult;           // EQMain_FitFontSize_guess's result for the third 0x64 text
    int firstButton;           // the 0xb row
    int secondButton;           // the 0x109 row
    int sortBoxes[3];        // the list's three buttons (CreateColumnCell_guess)
    char button2Path[32];      // a copy of the eqbutton2.bmp path
    char button3Path[32];      // a copy of the eqbutton3.bmp path
    char button1Path[32];      // a copy of the eqbutton1.bmp path
    unsigned int local_9c;  // the result: its low word is returned

    done = false;
    eqButton = 0;
    thirdButton = 0;
    extraButton = 0;
    stateButton = 0;
    local_9c = 0x11;
    listPositioned = false;
    tmp = 0x14;
    g_name_count_guess = 0;
    EQ_CDECL(void, 0x1001E150, (char))('\0');
    EQ_CDECL(void, 0x10007BF0, (unsigned int, char *, int))(g_parent_guess, g_selserver_bmp_guess, 0x10);
    g_banner_obj_guess = EQ_CDECL(char *, 0x10006D80, (int, int, int))(0x17c, 0x14, 0x10);
    EQ_CDECL(void, 0x10006C20, (char *, int, int, unsigned int, int, int, int, int, unsigned int))
        (g_banner_obj_guess, 0, 0, g_parent_guess, 0x74, 0x16a, 0x17c, 0x14, 0);
    firstButton = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0xb, 0x1b1, 0, 1);
    secondButton = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0x109, 0x1b1, 0, 1);
    EQ_CDECL(int, 0x100080B0, (unsigned int, int, int, char *))(g_parent_guess, 0x64, 0x22, g_eqmain_text_5830_guess);
    EQ_CDECL(int, 0x100080B0, (unsigned int, int, int, char *))(g_parent_guess, 0x64, 0x22, g_chat_button_text_2_guess);
    fitResult = EQ_CDECL(int, 0x100080B0, (unsigned int, int, int, char *))(g_parent_guess, 0x64, 0x22, g_text_840_guess);
    EQ_CDECL(void, 0x100017B0, (int, int, char *))(firstButton, 0, g_eqmain_text_5830_guess);
    EQ_CDECL(void, 0x100017B0, (int, int, char *))(secondButton, 0, g_chat_button_text_2_guess);
    g_overlay_obj_guess = EQ_CDECL(char *, 0x10001560, (unsigned int, int, int, unsigned char, char *, char *, char *))
        (g_parent_guess, 0xf0, 0x168, 0, g_selserver_overlay_guess, g_selserver_overlay_guess, g_selserver_overlay_guess);
    g_selected_obj_guess = EQ_CDECL(char *, 0x10005840, (unsigned int, int, int, int, int, unsigned int, int))
        (g_parent_guess, 0x82, 100, 0x13f, 0xd4, 0, 0x22d69);
    g_ctl_guess = EQ_CDECL(char *, 0x10006950, (unsigned int, int, int, int, int, int))
        (g_parent_guess, 0x36, 0x37, 0x10, 0x128, 0);
    g_list_obj_guess = EQ_CDECL(char *, 0x10005840, (unsigned int, int, int, int, int, unsigned int, int))
        (g_parent_guess, 0x1c2, 100, 0x50, 0xd4, 8, 0x22d69);
    EQ_CDECL(void, 0x100082C0, (unsigned int, unsigned int, unsigned int, int, char *))
        (g_parent_guess, 0x82, 0x52, 0x21a3c, g_text_573c_guess);
    sortBoxes[0] = EQ_CDECL(int, 0x100018F0, (unsigned int, int, int, char *, char *, int, int, char *))
        (g_parent_guess, 0x6e, 0x52, g_chekbox1_bmp_guess, g_chekbox2_bmp_guess, g_name_sort_mode_guess == 0, 0xf3f2f2,
         g_eqmain_drive_base_guess);
    nudge = strlen(g_text_5744_guess) > 0x10 ? 0x1e : 0;
    EQ_CDECL(void, 0x100082C0, (unsigned int, unsigned int, unsigned int, int, char *))
        (g_parent_guess, 0x1c2 - nudge, 0x52, 0x21a3c, g_text_5744_guess);
    sortBoxes[1] = EQ_CDECL(int, 0x100018F0, (unsigned int, int, int, char *, char *, int, int, char *))
        (g_parent_guess, 0x1ae - nudge, 0x52, g_chekbox1_bmp_guess, g_chekbox2_bmp_guess, g_name_sort_mode_guess == 1, 0xf3f2f2,
         g_eqmain_drive_base_guess);
    checkedBox = sortBoxes[g_name_sort_mode_guess];
    g_list_ctl_guess = EQ_CDECL(char *, 0x10006D80, (int, int, int))(0x118, 0x14, 0x10);
    EQ_CDECL(void, 0x10006C20, (char *, int, int, unsigned int, int, int, int, int, unsigned int))
        (g_list_ctl_guess, 0, 0, g_parent_guess, 0x82, 0x139, 0x118, 0x14, 0);
    EQ_CDECL(void, 0x100213D0, (int))(0x52);
    g_load_enabled_guess = 1;
    EQ_CDECL(void, 0x10025850, (int))(g_name_count_guess);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    index = 0;
    *(int *)(g_selected_obj_guess + 0x34) = 0;
    if (0 < g_name_count_guess)
    {
        do
        {
            if (strcmp(g_find_name_guess, g_name_str_guess + index * 0x118) == 0)
                *(int *)(g_selected_obj_guess + 0x34) = index;
            index = index + 1;
        } while (index < g_name_count_guess);
    }
    EQ_CDECL(long, 0x10035B24, (long *))(&lastRefresh);
    if (*(int *)(g_pending_obj_guess + 0x408) == -2)
    {
        *(int *)(g_pending_obj_guess + 0x408) = 0;
        pcVar10 = g_str_77c_guess;
    }
    else
    {
        if (*(int *)(g_pending_obj_guess + 0x408) != -1)
            goto eventLoop;
        *(int *)(g_pending_obj_guess + 0x408) = 0;
        pcVar10 = g_str_778_guess;
    }
    EQ_CDECL(void, 0x10001FF0, (unsigned int, char *, char *, char *))(g_parent_guess, pcVar10, g_str_888_guess, 0);
eventLoop:
    do
    {
        ev = EQ_STDCALL(int, 0x100220F0, (void))();
        switch (ev)
        {
        case 0x2:
            EQ_STDCALL(void, 0x1001DB40, (void))();
            EQ_CDECL(void, 0x10001FF0, (unsigned int, char *, char *, char *))(g_parent_guess, g_text_buf_guess, g_str_888_guess, 0);
            local_9c = 0;
            done = true;
            break;
        case 0x12:
            local_9c = 3;
            done = true;
            break;
        case 0x46:
            EQ_CDECL(void, 0x1001DBC0, (char *, int))(g_str_914_guess, 0);
            if ((g_word_after_guess != 0) && (extraButton == 0))
            {
                extraButton = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0xb, 0x186, 0, 1);
                EQ_CDECL(void, 0x100017B0, (int, int, char *))(extraButton, 0, g_str_9e0_guess);
            }
            if (((stateButton == 0) && (!done)) &&
                (EQ_CDECL(int, 0x1001E270, (unsigned int))(g_state_guess) != 6) &&
                (EQ_CDECL(int, 0x1001E270, (unsigned int))(g_state_guess) != 7))
            {
                pCVar11 = EQ_STDCALL(char *, 0x1001E400, (void))();
                tmp = EQ_CDECL(int, 0x100080B0, (unsigned int, int, int, char *))(g_parent_guess, 0xd2, 0x22, pCVar11);
                stateButton = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0x8e, 0x186, 0, 2);
                pCVar11 = EQ_STDCALL(char *, 0x1001E400, (void))();
                EQ_CDECL(void, 0x100017B0, (int, int, char *))(stateButton, 0, pCVar11);
                pCVar11 = EQ_CDECL(char *, 0x1001E350, (unsigned int))(g_state_guess);
                value = EQ_CDECL(int, 0x1001E2F0, (unsigned int))(g_state_guess);
                EQ_CDECL(void, 0x100082C0, (unsigned int, unsigned int, unsigned int, int, char *))
                    (g_parent_guess, 0x74, 0x16a, value, pCVar11);
            }
            if (thirdButton == 0)
            {
                if (strlen(g_str_ec9cc_guess) != 0)
                {
                    thirdButton = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0x8a, 0x1b1, 0, 1);
                    EQ_CDECL(void, 0x100017B0, (int, int, char *))(thirdButton, 0, g_text_840_guess);
                }
            }
            if ((eqButton == 0) && (0 < g_name_count_guess))
            {
                pcVar10 = EQ_CDECL(char *, 0x100210F0, (char *))(g_eqbutton1_bmp_guess);
                strcpy(button1Path, pcVar10);
                pcVar10 = EQ_CDECL(char *, 0x100210F0, (char *))(g_eqbutton2_bmp_guess);
                strcpy(button2Path, pcVar10);
                pcVar10 = EQ_CDECL(char *, 0x100210F0, (char *))(g_eqbutton3_bmp_guess);
                strcpy(button3Path, pcVar10);
                eqButton = EQ_CDECL(int, 0x10001560, (unsigned int, int, int, unsigned char, char *, char *, char *))
                    (g_parent_guess, 0x1a5, 0x185, 0, button1Path, button2Path, button3Path);
            }
            if (!listPositioned)
            {
                if (g_name_count_guess - 0xf < *(int *)(g_selected_obj_guess + 0x34))
                {
                    *(int *)(g_selected_obj_guess + 0x2c) = g_name_count_guess - 0xf;
                    listPositioned = true;
                    *(int *)(g_list_obj_guess + 0x2c) = g_name_count_guess - 0xf;
                    *(int *)(g_ctl_guess + 0x68) = g_name_count_guess - 0xf;
                    break;
                }
                *(int *)(g_selected_obj_guess + 0x2c) = *(int *)(g_selected_obj_guess + 0x34);
                *(int *)(g_list_obj_guess + 0x2c) = *(int *)(g_selected_obj_guess + 0x34);
                *(int *)(g_ctl_guess + 0x68) = *(int *)(g_selected_obj_guess + 0x34);
            }
            listPositioned = true;
            break;
        case 0x47:
            done = true;
            local_9c = 0x11;
            strcpy(g_eqmain_copy_c10_guess, g_str_84c4_guess);
            break;
        case 0x49:
            value = EQ_CDECL(int, 0x10033730, (char *, char *))(g_edit_name_guess, g_sel_name_guess);
            if (value == 0)
            {
                if (g_state_guess != 0)
                {
                    pCVar11 = EQ_CDECL(char *, 0x1001E350, (unsigned int))(g_state_guess);
                    value = EQ_CDECL(int, 0x1001E2F0, (unsigned int))(g_state_guess);
                    EQ_CDECL(void, 0x100082C0, (unsigned int, unsigned int, unsigned int, int, char *))
                        (g_parent_guess, 0x74, 0x16a, value, pCVar11);
                }
                goto case_4a;
            }
            control = EQ_CDECL(char *, 0x1001DC50, (char *))(g_sel_name_guess);
            if (control == 0)
            {
                EQ_CDECL(void, 0x10001FF0, (unsigned int, char *, char *, char *))(g_parent_guess, g_error_1100_guess, g_str_888_guess, 0);
                break;
            }
            EQ_STDCALL(void, 0x1001DB40, (void))();
            EQ_CDECL(int, 0x10034117, (char *, char *, ...))(g_text_buf_guess, g_fmt_788_guess, control, control);
            confirmed = EQ_CDECL(char, 0x10001FF0, (unsigned int, char *, char *, char *))
                (g_parent_guess, g_text_buf_guess, g_eqmain_text_b_guess, g_eqmain_text_a_guess);
            if (confirmed == 0)
                break;
            goto afterConfirm;
        case 0x4a:
case_4a:
afterConfirm:
            EQ_CDECL(void, 0x1001DBC0, (char *, int))(g_str_9d0_guess, 1);
            EQ_STDCALL(void, 0x10023B60, (void))();
            break;
        case 0x4b:
            g_name_sorted_guess = 1;
            break;
        case 0x50:
            EQ_STDCALL(void, 0x1001DB40, (void))();
            strcpy(g_text_buf_guess, g_str_784_guess);
            EQ_CDECL(void, 0x10001FF0, (unsigned int, char *, char *, char *))(g_parent_guess, g_text_buf_guess, g_str_888_guess, 0);
            break;
        case 0x52:
            bannerPath = EQ_CDECL(char *, 0x100210F0, (char *))(g_sbanner_bmp_guess);
            EQ_CDECL(void, 0x10001320, (char *))(bannerPath);
            break;
        case 0x60:
            EQ_STDCALL(void, 0x1001DB40, (void))();
            if (g_choice_count_guess < 4)
            {
                if (0 < g_choice_count_guess)
                {
                    g_choice_result_guess = EQ_CDECL(int, 0x10001CE0, (unsigned int, char *, int))
                        (g_parent_guess, g_choice_text_guess, g_choice_count_guess);
                    if (-1 < g_choice_result_guess)
                    {
                        EQ_CDECL(void, 0x10024F90, (unsigned short))(0x60);
                        EQ_CDECL(void, 0x10024FD0, (int))(g_choice_result_guess);
                        EQ_STDCALL(void, 0x100213F0, (void))();
                    }
                    g_choice_count_guess = 0;
                }
            }
            else
            {
                done = true;
                local_9c = 0x19;
                g_default_result_guess = 0x12;
            }
            break;
        }
        EQ_CDECL(long, 0x10035B24, (long *))(&now);
        if (lastRefresh + 10 < now)
        {
            lastRefresh = now;
            EQ_CDECL(void, 0x10025850, (int))(g_name_count_guess);
        }
        if (g_name_sorted_guess != 0)
        {
            EQ_STDCALL(void, 0x1001DCB0, (void))();
            g_name_sorted_guess = 0;
        }
        EQ_STDCALL(void, 0x10001350, (void))();
        _Memory = EQ_STDCALL(int *, 0x10003BD0, (void))();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                pHVar7 = _Memory[2];
                if (pHVar7 == eqButton)
                {
                    strcpy(g_sel_name_guess, g_name_str_guess + *(int *)(g_selected_obj_guess + 0x34) * 0x118);
                    strcpy(g_edit_name_guess, g_sel_name_guess);
                    EQ_CDECL(void, 0x1001DBC0, (char *, int))(g_str_9d4_guess, 1);
                    EQ_STDCALL(void, 0x10023BB0, (void))();
                }
                else
                {
                    if (pHVar7 == firstButton)
                    {
                        local_9c = 7;
                    }
                    else if (pHVar7 == extraButton)
                    {
                        local_9c = 0x1f;
                    }
                    else if (pHVar7 == thirdButton)
                    {
                        local_9c = 4;
                    }
                    else
                    {
                        if (pHVar7 == secondButton)
                        {
                            local_9c = 5;
                        }
                        else
                        {
                            if (pHVar7 != stateButton)
                                goto freeMessage;
                            local_9c = EQ_CDECL(unsigned int, 0x1001E3B0, (unsigned int))(g_state_guess);
                        }
                        g_default_result_guess = 0x12;
                    }
setDone:
                    done = true;
                }
            }
            else if (*_Memory == 1)
            {
                control = (char *)_Memory[2];
                if (control == g_ctl_guess)
                {
                    if (*(int *)(g_ctl_guess + 0x68) < 0)
                        *(int *)(g_ctl_guess + 0x68) = 0;
                    *(int *)(g_selected_obj_guess + 0x2c) = *(int *)(g_ctl_guess + 0x68);
                    *(int *)(g_list_obj_guess + 0x2c) = *(int *)(g_ctl_guess + 0x68);
                    EQ_CDECL(void, 0x10005A00, (char *))(g_selected_obj_guess);
                    EQ_CDECL(void, 0x10005A00, (char *))(g_list_obj_guess);
                }
                else if (control == g_selected_obj_guess)
                {
                    g_name_sorted_guess = 1;
                }
                else
                {
                    index = EQ_CDECL(short, 0x10001AD0, (char *, int *, short, int *))(control, sortBoxes, 2, &checkedBox);
                    if (index != -1)
                    {
                        checkedBox = sortBoxes[index];
                        g_name_sort_mode_guess = index;
                        EQ_STDCALL(void, 0x1001E120, (void))();
                        *(int *)(g_selected_obj_guess + 0x2c) = 0;
                        *(int *)(g_list_obj_guess + 0x2c) = 0;
                    }
                }
            }
            else if (*_Memory == 4)
            {
                if ((char)_Memory[2] == '\r')
                {
                    if (listPositioned)
                    {
                        strcpy(g_sel_name_guess, g_name_str_guess + *(int *)(g_selected_obj_guess + 0x34) * 0x118);
                        strcpy(g_edit_name_guess, g_sel_name_guess);
                        EQ_CDECL(void, 0x1001DBC0, (char *, int))(g_str_9d4_guess, 1);
                        EQ_STDCALL(void, 0x10023BB0, (void))();
                    }
                }
                else if ((char)_Memory[2] == '\x1b')
                {
                    EQ_STDCALL(void, 0x10003A00, (void))();
                    local_9c = 7;
                    goto setDone;
                }
            }
freeMessage:
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (done)
        {
            g_load_enabled_guess = 0;
            EQ_STDCALL(void, 0x100011A0, (void))();
            g_eqmain_flag_ce78_guess = 0;
            EQ_CDECL(void, 0x10004E10, (unsigned int))(g_parent_guess);
            EQ_CDECL(void, 0x10007A80, (char *))(g_banner_obj_guess);
            EQ_CDECL(void, 0x10007A80, (char *))(g_list_ctl_guess);
            EQ_CDECL(void, 0x100039E0, (int))(0);
            EQ_CDECL(void, 0x1001E150, (char))(1);
            return (unsigned short)local_9c;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x1001f140
// Loads front-end name records and their associated values.
// @early-stop: cdecl decoder argument cleanup remains split around the inlined string copies.
// ORIGINAL FILE: unknown
// __cdecl: one stack arg (the stream, handed to SetStateAndDword_guess), callee pops 0. Does nothing unless g_load_enabled_guess is
// set. "test" in g_news_path_guess (lowercased) sets g_eqmain_flag_1d20_guess. The stream then gives a signed 16-bit count (the low
// word of a dword). A count >= 0 rebuilds the records (g_name_count_guess = count): a string at +0, a second string at
// +0x80, a byte (sign-extended) at +0x108, a dword at +0x100 and the index at +0x104. A count < 0 is -count
// updates: each stream dword goes to +0x100 of the record whose index is the update's number, and g_name_count_guess
// stays as it is. Then g_word_after_guess = dword, g_str_eca4c_guess = string, DAT_100f1160 = sum of three dwords,
// g_str_ec9cc_guess = string, and SortNameTableByMode_guess() runs.
#pragma intrinsic(memset)
void __cdecl LoadNameTable_guess(unsigned int data)
{
    unsigned int value;     // 0x08: the count dword, then each update's value
    char *name;             // 0x04: the record's name (+0)
    unsigned int *source;
    char *destination;
    short index;            // 0x00: the record being built
    short i;
    int remaining;
    int update_no;
    char lower[128];        // 0x0c: lowercased copy of the record name, never read again

    if (g_load_enabled_guess != 0)
    {
        name = EQ_CDECL(char *, 0x10035FD0, (char *, char *))
            (EQ_CDECL(char *, 0x10035C00, (char *))(g_news_path_guess), g_str_test_guess);
        if (name != 0)
            g_test_flag_guess = 1;
        EQ_CDECL(void, 0x100250A0, (unsigned int))(data);
        value = EQ_STDCALL(unsigned int, 0x10025120, (void))();
        update_no = 0;
        if ((short)value < 0)
        {
            value = -value;
            if ((short)value > 0)
            {
                remaining = (short)value;
                do
                {
                    value = EQ_STDCALL(unsigned int, 0x10025120, (void))();
                    i = 0;
                    while (i < g_name_count_guess)
                    {
                        if (g_rec_value_guess[i * 0x46 + 1] == update_no)
                        {
                            g_rec_value_guess[i * 0x46] = value;
                            break;
                        }
                        i++;
                    }
                    update_no++;
                    remaining--;
                } while (remaining != 0);
            }
        }
        else
        {
            index = 0;
            if ((short)value > 0)
            {
                do
                {
                    name = EQ_ARRAY(char, 0x100ECAD0) + index * 0x118;
                    memset(name, 0, 0x118);
                    source = EQ_CDECL(unsigned int *, 0x10021110, (unsigned char *))
                        (EQ_STDCALL(unsigned char *, 0x10025150, (void))());
                    strcpy(name, (char *)source);
                    source = EQ_CDECL(unsigned int *, 0x10021110, (unsigned char *))
                        (EQ_STDCALL(unsigned char *, 0x10025150, (void))());
                    destination = g_name_str_guess + index * 0x118;
                    strcpy(destination, (char *)source);
                    g_rec_value_guess[index * 0x46 + 2] = (int)(char)(EQ_STDCALL(unsigned int, 0x100250C0, (void))());
                    g_rec_value_guess[index * 0x46] = EQ_STDCALL(unsigned int, 0x10025120, (void))();
                    g_rec_value_guess[index * 0x46 + 1] = index;
                    strcpy(lower, name);
                    EQ_CDECL(char *, 0x10035C00, (char *))(lower);
                    index++;
                } while (index < (short)value);
            }
            g_name_count_guess = index;
        }
        g_word_after_guess = EQ_STDCALL(unsigned int, 0x10025120, (void))();
        source = EQ_CDECL(unsigned int *, 0x10021110, (unsigned char *))
            (EQ_STDCALL(unsigned char *, 0x10025150, (void))());
        strcpy(g_str_eca4c_guess, (char *)source);
        g_state_guess = EQ_STDCALL(unsigned int, 0x10025120, (void))();
        g_state_guess += EQ_STDCALL(unsigned int, 0x10025120, (void))();
        g_state_guess += EQ_STDCALL(unsigned int, 0x10025120, (void))();
        source = EQ_CDECL(unsigned int *, 0x10021110, (unsigned char *))
            (EQ_STDCALL(unsigned char *, 0x10025150, (void))());
        strcpy(g_str_ec9cc_guess, (char *)source);
        EQ_STDCALL(void, 0x1001E120, (void))();
    }
}

// This image's addresses for the function below (file-local).
#define g_choice_count_guess    EQ_GLOBAL(int, 0x100F1164)              // g_choice_count_guess: choices in the text (0x11 at most)
#define g_choice_text_guess     EQ_ARRAY(char, 0x100F1168)              // g_choice_text_guess: the text strtok cuts
#define g_choice_delim_guess    EQ_ARRAY(char, 0x100484E4)              // g_choice_delim_guess: the delimiter
#define g_eqvote_guess          EQ_ARRAY(char, 0x100484D8)              // g_eqvote_guess: "..."
#define g_parent_guess          EQ_GLOBAL(unsigned int, 0x1004CEB8)     // g_eqmain_ceb8_guess: the window the dialog is made in
#define g_eqmain_flag_ce78_guess      EQ_GLOBAL(unsigned char, 0x1004CE78)    // g_eqmain_flag_ce78_guess: 1 while the dialog runs
#define g_choice_result_guess   EQ_GLOBAL(int, 0x100482C8)              // g_choice_result_guess: the index picked

// FUNCTION: EQMAIN 0x1001F3A0
// Shows vote choices and submits the player's selected choice.
// __stdcall, no args, returns a byte (AL). 0 when g_choice_count_guess is above 0x11 or strtok finds no first token.
// Else it builds the "..." dialog (one row handle per choice from eqmain_create_button_guess, its text from eqmain_draw_text_guess),
// then reads messages until one ends it: key 0x12 (result 1), a type 0 message whose +8 is a row handle (its index
// goes to g_choice_result_guess, g_choice_count_guess becomes 0, SetStateAndWord_guess(0x60) and AppendDword_guess(index) run), or a type 4 message
// with byte 0x1b (ESC). Every message is freed; the rows and the window are freed at the end.
unsigned char __stdcall RunVoteDialog_guess(void)
{
    bool done;              // 0x4e: set once the loop must end; zeroed first (asm 1001F3AC)
    unsigned char result;   // 0x4d: returned; 1 once key 0x12 came
    int step;               // 0x4c: the y step between rows
    unsigned int *dialog;   // 0x48: the window from EQMain_CreateSurfaceRecord_guess, 0 if none
    int rows[17];           // 0x44: the row handles
    char *token;
    int row;
    int *msg;
    int *slot;
    int i;                  // the row being built
    int pos;                // the row's y, then the search index, then the cleanup count
    unsigned int key;

    done = false;
    result = 0;
    if (g_choice_count_guess > 0x11)
        return 0;
    token = EQ_CDECL(char *, 0x10033996, (char *, char *))(g_choice_text_guess, g_choice_delim_guess);
    if (token == 0)
        return 0;
    EQ_CDECL(void, 0x10007BF0, (unsigned int, char *, int))(g_parent_guess, g_eqvote_guess, 0x10);
    dialog = EQ_CDECL(unsigned int *, 0x10008460, (unsigned int, int, int, int, int, unsigned int, int, int, unsigned int, unsigned int))(g_parent_guess, 0x3c, 0x4a, 0x212, 0x2a, 500, 0, 0, 0xc81000, 2);
    EQ_CDECL(void, 0x10008970, (unsigned int *, char *))(dialog, token);
    if (g_choice_count_guess > 0xe)
    {
        step = 0x16;
        pos = 0x66;
    }
    else if (g_choice_count_guess > 0xd)
    {
        step = 0x16;
        pos = 0x76;
    }
    else
    {
        step = 0x13c / g_choice_count_guess;
        pos = step / 2 + 0x6a;
    }
    memset(rows, 0, sizeof(rows));
    i = 0;
    if (0 < g_choice_count_guess)
    {
        slot = rows;
        do
        {
            row = EQ_CDECL(int, 0x10001520, (unsigned int, int, int, unsigned char, int))(g_parent_guess, 0, pos, 1, 4);
            *slot = row;
            token = EQ_CDECL(char *, 0x10033996, (char *, char *))(0, g_choice_delim_guess);
            if (token != 0)
                EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))(row, 0, token, 0, 0);
            pos = pos + step;
            i = i + 1;
            slot = slot + 1;
        } while (i < g_choice_count_guess);
    }
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        key = EQ_STDCALL(unsigned int, 0x100220F0, (void))();
        if (key == 0x12)
        {
            result = 1;
            done = true;
        }
        msg = EQ_STDCALL(int *, 0x10003BD0, (void))();
        if (msg != 0)
        {
            if (*msg != 0)
            {
                if (*msg == 4 && (char)msg[2] == '\x1b')
                {
finishDialog:
                    EQ_STDCALL(void, 0x10003A00, (void))();
                    done = true;
                }
            }
            else
            {
                pos = 0;
                while (pos < g_choice_count_guess)
                {
                    if (rows[pos] == msg[2])
                    {
                        g_choice_result_guess = pos;
                        g_choice_count_guess = 0;
                        EQ_CDECL(void, 0x10024F90, (unsigned short))(0x60);
                        EQ_CDECL(void, 0x10024FD0, (int))(g_choice_result_guess);
                        EQ_STDCALL(void, 0x100213F0, (void))();
                        // as original: the ESC branch (asm 1001F50D JMP) and this match path (asm 1001F554 falls through)
                        // both enter the block at 1001F559 (TickBlinkAndCursor_guess, then done = true); this goto jumps to that tail.
                        goto finishDialog;
                    }
                    pos = pos + 1;
                }
            }
            EQ_CDECL(void, 0x10033568, (void *))(msg);
        }
        if (done)
        {
            g_eqmain_flag_ce78_guess = 0;
            slot = rows;
            pos = 0x11;
            do
            {
                if (*slot != 0)
                {
                    EQ_CDECL(void, 0x10001700, (void *))((void *)*slot);
                    *slot = 0;
                }
                slot = slot + 1;
                pos = pos - 1;
            } while (pos != 0);
            if (dialog != 0)
                EQ_CDECL(void, 0x10008A00, (void *))(dialog);
            EQ_CDECL(void, 0x100039E0, (int))(0);
            return result;
        }
    } while (1);
}

// FUNCTION: EQMAIN 0x1001F5D0
// Adjusts a supplied value based on the current state category.
// __cdecl, callee pops 0. Switches on ClassifyFlags_guess(DAT_100f1160): 0 stores base, 1..3 stores base - 1, 4..6 stores
// base - 2, 7 stores 0; any other value stores nothing. Returns 0 on every path.
int __cdecl AdjustByState_guess(int *adjustedValue, int base)
{
    unsigned int state;

    state = EQ_CDECL(unsigned int, 0x1001E270, (unsigned int))(g_state_guess);
    switch (state)
    {
    case 0:
        *adjustedValue = base;
        return 0;
    case 1:
    case 2:
    case 3:
        *adjustedValue = base + -1;
        return 0;
    case 4:
    case 5:
    case 6:
        *adjustedValue = base + -2;
        return 0;
    case 7:
        *adjustedValue = 0;
    }
    return 0;
}

// This image's addresses for ShowStartScreen_guess (file-local).
#define g_draw_obj_guess        EQ_GLOBAL(void *, 0x1004CEB8)           // g_eqmain_ceb8_guess: the draw object (460 xrefs)
#define g_eqmain_str_559b8_guess     EQ_GLOBAL(char *, 0x100559B8)           // g_eqmain_str_559b8_guess: text of the second line
#define g_eqmain_busy_guess      EQ_GLOBAL(char, 0x1004CE78)             // g_eqmain_flag_ce78_guess: set to 1 once the start screen is drawn
#define g_eqmain_evstart_bmp_guess    EQ_ARRAY(char, 0x10047F7C)              // g_eqmain_evstart_bmp_guess: "..."

// This image's addresses for SetupTwoRows_guess (file-local).
#define g_text_a_guess          EQ_GLOBAL(char *, 0x100558A0)           // g_text_a_guess: text of the first row
#define g_text_b_guess          EQ_GLOBAL(char *, 0x10055930)           // g_expansion_b_guess: text of the second row

// FUNCTION: EQMAIN 0x1001F650
// Creates two labeled text rows for a front-end screen.
// __cdecl, callee pops 0. Makes two items on the draw object g_eqmain_ceb8_guess with eqmain_create_button_guess (stored through firstTextObject and
// secondTextObject), then EQMain_FitFontSize_guess sets g_text_a_guess and g_expansion_b_guess at (0xd2, 0x22); the second EQMain_FitFontSize_guess result is the
// argument of both eqmain_draw_text_guess calls. The original never pops its pushes between calls (one ADD ESP,0x40 at the end),
// and it passes two extra stack words to EQMain_FitFontSize_guess and eqmain_draw_text_guess. Returns 1 in AX only: the high half of EAX is
// left as the last callee returned it.
short __cdecl SetupTwoRows_guess(int *firstTextObject, int *secondTextObject)
{
    int ret_text_a;
    int ret_text_b;

    *firstTextObject = EQ_CDECL(int, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0x145, 0x96, 0, 2);
    *secondTextObject = EQ_CDECL(int, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0x145, 0x13b, 0, 2);
    ret_text_a = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0xd2, 0x22, g_text_a_guess, 0x19, 0);
    ret_text_b = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0xd2, 0x22, g_text_b_guess, ret_text_a, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        ((void *)*firstTextObject, 0, g_text_a_guess, ret_text_b, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        ((void *)*secondTextObject, 0, g_text_b_guess, ret_text_b, 0);
    return 1;
}

#define g_upgrd_bmp_guess       EQ_ARRAY(char, 0x100484E8)              // g_upgrd_bmp_guess: "..."
#define g_upgrd_text_guess      EQ_GLOBAL(char *, 0x10055858)           // g_eqmain_reg_name_c_guess: text of the row and its item

// FUNCTION: EQMAIN 0x1001F710
// Offers four choices in the front-end upgrade dialog.
// __stdcall, callee pops 0. The eq_upgrd.bmp screen: a click on one of four items ends with 0x1b..0x1e, Escape with
// g_chat_status_guess; the main item posts Escape. The original passes two extra words to EQMain_FitFontSize_guess and eqmain_draw_text_guess.
unsigned short __stdcall RunUpgradeSelectionDialog_guess(void)
{
    unsigned short result;
    bool done;
    int *msg;
    void *main_item;
    int text_ret;
    int item_a;
    int item_b;
    int item_c;
    int item_d;
    int aux;
    int handle;

    item_b = 0;
    item_a = 0;
    item_c = 0;
    item_d = 0;
    result = g_default_result_guess;
    done = false;
    aux = 0;
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_upgrd_bmp_guess, 0x10);
    main_item = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x14, 1);
    text_ret = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upgrd_text_guess, 0x19, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (main_item, 0, g_upgrd_text_guess, text_ret, 0);
    EQ_CDECL(int, 0x1001F5D0, (int *, int))(&aux, 3);
    EQ_CDECL(int, 0x1001F650, (int *, int *, int *, int *, int))
        (&item_a, &item_b, &item_c, &item_d, aux);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        msg = (int *)EQ_STDCALL(int, 0x10003BD0, (void))();
        if (msg != 0)
        {
            if (*msg != 0)
            {
                if (*msg == 4 && (char)msg[2] == 0x1b)
                {
                    EQ_STDCALL(void, 0x10003A00, (void))();
                    goto set_done;
                }
            }
            else
            {
                handle = msg[2];
                if ((void *)handle == main_item)
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))
                        (4, 0x1b, 0, 0);
                }
                else
                {
                    if (handle == item_a)
                    {
                        result = 0x1b;
                    }
                    else if (handle == item_b)
                    {
                        result = 0x1c;
                    }
                    else if (handle == item_c)
                    {
                        result = 0x1d;
                    }
                    else
                    {
                        if (handle != item_d)
                        {
                            goto free_msg;
                        }
                        result = 0x1e;
                    }
set_done:
                    done = true;
                }
            }
free_msg:
            EQ_CDECL(void, 0x10033568, (void *))(msg);
        }
    } while (!done);
    g_eqmain_flag_ce78_guess = 0;
    EQ_CDECL(void, 0x10004E10, (void *))(g_draw_obj_guess);
    EQ_CDECL(void, 0x100039E0, (int))(0);
    return result;
}

// This image's addresses for RunUpgradeScreen_guess (file-local): the news and Velious upgrade screen's bitmaps, text rows,
// list items, edit box and messages.
#define g_default_result_guess  EQ_GLOBAL(unsigned short, 0x1004814E)  // g_chat_status_guess: the result, unless the 0x12 event sets 3
#define g_news_bmp_guess        EQ_ARRAY(char, 0x10046240)              // g_news_bmp_guess: "..."
#define g_velious_bmp_guess     EQ_ARRAY(char, 0x10048504)              // g_velious_bmp_guess: "..."
#define g_upg_text_a_guess      EQ_GLOBAL(char *, 0x10055858)           // g_eqmain_reg_name_c_guess: text of the first row
#define g_upg_text_b_guess      EQ_GLOBAL(char *, 0x10055854)           // g_upg_text_b_guess: text of the second row
#define g_eqmain_dlg_arg_guess    EQ_GLOBAL(int, 0x1004CEC8)              // g_eqmain_dlg_arg_guess: the 8th argument of EQMain_CreateRecord_guess
#define g_upg_buf_guess         EQ_ARRAY(char, 0x100F4C10)              // g_eqmain_copybuf_guess: the text the screen builds
#define g_upg_item1_guess       EQ_GLOBAL(char *, 0x10055934)           // g_upg_item1_guess: list item 1
#define g_upg_item2_guess       EQ_GLOBAL(char *, 0x10055938)           // g_upg_item2_guess: list item 2
#define g_upg_item3_guess       EQ_GLOBAL(char *, 0x1005593C)           // g_upg_item3_guess: list item 3
#define g_upg_edit_guess        EQ_GLOBAL(unsigned int, 0x100F2048)     // g_upg_edit_guess: the edit box EQMain_CreateLabelFixed_guess writes; its text at +0x20
#define g_upg_edit_text_guess   EQ_GLOBAL(char *, 0x100559FC)           // g_upg_edit_text_guess: the edit box's first text
#define g_upg_fmt_guess         EQ_GLOBAL(char *, 0x10055918)           // g_upg_fmt_guess: the format of the 0x6c message
#define g_upg_num_guess         EQ_GLOBAL(int, 0x100559F4)              // g_upg_num_guess: the int the 0x6c message prints, twice
#define g_upg_msg_empty_guess   EQ_GLOBAL(char *, 0x100559F0)           // g_upg_msg_empty_guess: the message for an empty entry
#define g_upg_msg_short_guess   EQ_GLOBAL(char *, 0x100559EC)           // g_upg_msg_short_guess: the message for an entry under 10 characters
#define g_upg_msg_bad_guess     EQ_GLOBAL(char *, 0x10055AD8)           // g_upg_msg_bad_guess: the message for an entry not starting with V
#define g_upg_code_guess        EQ_GLOBAL(char *, 0x100559E4)           // g_upg_code_guess: copied to the buffer for a V entry

// This image's addresses for RunKunarkUpgradeScreen_guess (file-local): the Kunark bitmap, list items, the message count and
// the confirm question. The text rows, edit box and messages are the ones in the block above.
#define g_kunark_bmp_guess          EQ_ARRAY(char, 0x100484F8)              // g_kunark_bmp_guess: "..."
#define g_kun_item1_guess           EQ_GLOBAL(char *, 0x1005591C)           // g_kun_item1_guess: list item 1
#define g_kun_item2_guess           EQ_GLOBAL(char *, 0x10055920)           // g_kun_item2_guess: list item 2
#define g_kun_item3_guess           EQ_GLOBAL(char *, 0x10055924)           // g_kun_item3_guess: list item 3
#define g_kun_item4_guess           EQ_GLOBAL(char *, 0x10055928)           // g_kun_item4_guess: list item 4
#define g_kun_num_guess             EQ_GLOBAL(int, 0x100559F8)              // g_kun_num_guess: the int the 0x66 message prints, twice
#define g_kun_confirm_msg_guess     EQ_GLOBAL(char *, 0x100559E8)           // g_kun_confirm_msg_guess: the question eqmainRunDialog_guess asks for a K text
#define g_kun_msg_bad_guess         EQ_GLOBAL(char *, 0x10055AD4)           // g_kun_msg_bad_guess: the message for a text not starting with K, k, U or u

// FUNCTION: EQMAIN 0x1001F880
// Kunark upgrade screen: checks a typed code (10+ chars, K or U first) and sends it as message 0x6d
// __stdcall, callee pops 0. The news and Kunark upgrade screen: draws news.bmp and kunark.bmp on the draw object, makes two
// text rows, a four-item list and the edit box (EQMain_CreateLabelFixed_guess writes g_upg_edit_guess), sets g_eqmain_flag_ce78_guess to 1, then loops:
// RdpReceiveScreenDispatch_guess gives an event (2 clears a pending screen, 0x66 prints g_kun_num_guess and ends, 0x12 ends with 3) and
// PumpMessagesAndEditText_guess a message (type 0 on pHVar9 calls EnqueueQuad_guess; type 0 on pHVar10 checks the edit text; type 4 with 0x1b
// calls TickBlinkAndCursor_guess and ends). The loop ends when done is set; returns g_chat_status_guess, or 3 once the 0x12 event set altResult.
// event is the whole of EAX (the original TESTs EAX), so the event is an int. The original passes two extra stack words to
// EQMain_FitFontSize_guess and eqmain_draw_text_guess, and one to ClearBlinkAndPump_guess.
unsigned short __stdcall RunKunarkUpgradeScreen_guess(void)
{
    unsigned short result;
    bool dialogShown;
    bool awaitingReply;
    bool done;
    bool altResult;
    int event;
    char firstChar;
    char *entryText;
    char *message;
    int *block;
    int *_Memory;
    void *pHVar9;
    void *pHVar10;
    int ret_text_a;
    int ret_text_b;

    result = g_default_result_guess;
    dialogShown = false;
    awaitingReply = false;
    done = false;
    altResult = false;
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_news_bmp_guess, 0x10);
    EQ_CDECL(void, 0x100056C0, (void *, char *, int, int))
        (g_draw_obj_guess, EQ_CDECL(char *, 0x100210F0, (char *))(g_kunark_bmp_guess), 0, 0);
    pHVar9 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x14, 1);
    pHVar10 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x18, 1);
    ret_text_a = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_a_guess, 0x19, 0);
    ret_text_b = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_b_guess, ret_text_a, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar9, 0, g_upg_text_a_guess, ret_text_b, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar10, 0, g_upg_text_b_guess, ret_text_b, 0);
    block = EQ_CDECL(int *, 0x10008C10, (void *, int, int, int, int, int, int, int, int))
        (g_draw_obj_guess, 0x34, 0x4d, 0x208, 0x12c, 0xf, 0xc81000, g_eqmain_dlg_arg_guess, 0);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_kun_item1_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_kun_item2_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_kun_item3_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_kun_item4_guess));
    g_upg_edit_guess = 0;
    EQ_CDECL(void, 0x10011970, (int, int, char *, unsigned int *, unsigned int, char *))
        (0xb8, 0x132, g_upg_edit_text_guess, &g_upg_edit_guess, 0x1a, 0);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        event = EQ_STDCALL(int, 0x100220F0, (void))();
        if (event != 0)
        {
            awaitingReply = false;
        }
        if (event == 2)
        {
            if (dialogShown)
            {
                dialogShown = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
        }
        else if (event == 0x12)
        {
            altResult = true;
            done = true;
        }
        else if (event == 0x66)
        {
            if (dialogShown)
            {
                dialogShown = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(int, 0x10034117, (char *, char *, ...))
                (g_upg_buf_guess, g_upg_fmt_guess, g_kun_num_guess, g_kun_num_guess);
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
            done = true;
        }
        _Memory = (int *)EQ_STDCALL(int, 0x10003BD0, (void))();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                if ((void *)_Memory[2] == pHVar9)
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))
                        (4, 0x1b, 0, 0);
                }
                else if (((void *)_Memory[2] == pHVar10) && (!awaitingReply))
                {
                    entryText = *(char **)(g_upg_edit_guess + 0x20);
                    message = g_upg_msg_empty_guess;
                    if (strlen(entryText) != 0)
                    {
                        message = g_upg_msg_short_guess;
                        if (strlen(entryText) > 9)
                        {
                            if (((*entryText != 'K') && (*entryText != 'k')) ||
                                (EQ_CDECL(char, 0x10001FF0, (void *, char *, char *, char *))
                                    (g_draw_obj_guess, g_kun_confirm_msg_guess, g_eqmain_text_b_guess,
                                     g_eqmain_text_a_guess) != 0))
                            {
                                firstChar = **(char **)(g_upg_edit_guess + 0x20);
                                if (((firstChar != 'K') && (firstChar != 'k')) &&
                                    ((firstChar != 'U' && (message = g_kun_msg_bad_guess, firstChar != 'u'))))
                                    goto showMessage;
                                awaitingReply = true;
                                EQ_CDECL(void, 0x10024F90, (short))
                                    (0x6d);
                                entryText = EQ_CDECL(char *, 0x100211B0, (char *))
                                    (*(char **)(g_upg_edit_guess + 0x20));
                                EQ_CDECL(void, 0x10024FF0, (char *))
                                    (entryText);
                                EQ_STDCALL(void, 0x100213F0, (void))();
                                EQ_CDECL(void, 0x10003000, (char *))
                                    (strcpy(g_upg_buf_guess, g_upg_code_guess));
                                dialogShown = true;
                            }
                            goto freeMessage;
                        }
                    }
showMessage:
                    EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                        (g_draw_obj_guess, message, g_str_888_guess, 0);
                }
            }
            else if (*_Memory == 4 && (char)_Memory[2] == 0x1b)
            {
                EQ_STDCALL(void, 0x10003A00, (void))();
                done = true;
            }
freeMessage:
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (done)
        {
            if (dialogShown)
            {
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            g_eqmain_flag_ce78_guess = 0;
            EQ_CDECL(void, 0x10008CE0, (int *))(block);
            EQ_CDECL(void, 0x10004E10, (void *))(g_draw_obj_guess);
            // ClearBlinkAndPump_guess pops 0 itself, so the caller pops the pushed 0 (EQ_STDCALL here dropped the batched pops: RET read a local).
            EQ_CDECL(void, 0x100039E0, (int))(0);
            if (altResult)
            {
                result = 3;
            }
            return result;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x1001FD40
// Velious upgrade screen: sends a V-prefixed entry of 10+ chars to the server, closes on server event 0x6c
// __stdcall, callee pops 0. The news and Velious upgrade screen: draws news.bmp and eq_velious_upgrad.bmp on the draw
// object, makes two text rows, a three-item list and the edit box (EQMain_CreateLabelFixed_guess writes g_upg_edit_guess), sets g_eqmain_flag_ce78_guess
// to 1, then loops: RdpReceiveScreenDispatch_guess gives an event (2 clears a pending screen, 0x12 ends, 0x6c prints) and PumpMessagesAndEditText_guess a
// message. The loop ends when done is set; returns g_chat_status_guess, or 3 once the 0x12 event set altResult. event is the whole
// of EAX (the original TESTs EAX), so the event is an int. The original passes two extra stack words to EQMain_FitFontSize_guess and
// eqmain_draw_text_guess, and one to ClearBlinkAndPump_guess.
unsigned short __stdcall RunUpgradeScreen_guess(void)
{
    unsigned short result;
    bool dialogShown;
    bool awaitingReply;
    bool done;
    bool altResult;
    int event;
    int ret_text_a;
    int ret_text_b;
    char *entryText;
    void *pHVar9;
    void *pHVar10;
    int *block;
    int *_Memory;

    result = g_default_result_guess;
    dialogShown = false;
    awaitingReply = false;
    done = false;
    altResult = false;
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_news_bmp_guess, 0x10);
    EQ_CDECL(void, 0x100056C0, (void *, char *, int, int))
        (g_draw_obj_guess, EQ_CDECL(char *, 0x100210F0, (const char *))(g_velious_bmp_guess), 0, 0);
    pHVar9 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x14, 1);
    pHVar10 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x18, 1);
    ret_text_a = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_a_guess, 0x19, 0);
    ret_text_b = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_b_guess, ret_text_a, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar9, 0, g_upg_text_a_guess, ret_text_b, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar10, 0, g_upg_text_b_guess, ret_text_b, 0);
    block = EQ_CDECL(int *, 0x10008C10, (void *, int, int, int, int, int, int, int, int))
        (g_draw_obj_guess, 0x34, 0x4d, 0x208, 0x12c, 0xf, 0xc81000, g_eqmain_dlg_arg_guess, 0);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_upg_item1_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_upg_item2_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_upg_item3_guess));
    g_upg_edit_guess = 0;
    EQ_CDECL(void, 0x10011970, (int, int, char *, unsigned int *, unsigned int, char *))
        (0xb8, 0x132, g_upg_edit_text_guess, &g_upg_edit_guess, 0x1a, 0);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        event = EQ_STDCALL(int, 0x100220F0, (void))();
        if (event != 0)
        {
            awaitingReply = false;
        }
        if (event == 2)
        {
            if (dialogShown)
            {
                dialogShown = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
        }
        else if (event == 0x12)
        {
            altResult = true;
            done = true;
        }
        else if (event == 0x6c)
        {
            if (dialogShown)
            {
                dialogShown = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(int, 0x10034117, (char *, char *, ...))
                (g_upg_buf_guess, g_upg_fmt_guess, g_upg_num_guess, g_upg_num_guess);
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
            done = true;
        }
        _Memory = (int *)EQ_STDCALL(int, 0x10003BD0, (void))();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                if ((void *)_Memory[2] == pHVar9)
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))
                        (4, 0x1b, 0, 0);
                }
                else if (((void *)_Memory[2] == pHVar10) && (!awaitingReply))
                {
                    entryText = *(char **)(g_upg_edit_guess + 0x20);
                    if (strlen(entryText) == 0)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_empty_guess, g_str_888_guess, 0);
                    }
                    else if (strlen(entryText) < 10)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_short_guess, g_str_888_guess, 0);
                    }
                    else if ((*entryText == 'V') || (*entryText == 'v'))
                    {
                        awaitingReply = true;
                        EQ_CDECL(void, 0x10024F90, (short))
                            (0x6d);
                        entryText = EQ_CDECL(char *, 0x100211B0, (char *))
                            (*(char **)(g_upg_edit_guess + 0x20));
                        EQ_CDECL(void, 0x10024FF0, (char *))
                            (entryText);
                        EQ_STDCALL(void, 0x100213F0, (void))();
                        EQ_CDECL(void, 0x10003000, (char *))
                            (strcpy(g_upg_buf_guess, g_upg_code_guess));
                        dialogShown = true;
                    }
                    else
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_bad_guess, g_str_888_guess, 0);
                    }
                }
            }
            else if ((*_Memory == 4) && ((char)_Memory[2] == '\x1b'))
            {
                EQ_STDCALL(void, 0x10003A00, (void))();
                done = true;
            }
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (done)
        {
            if (dialogShown)
            {
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            g_eqmain_flag_ce78_guess = 0;
            EQ_CDECL(void, 0x10008CE0, (void *))(block);
            EQ_CDECL(void, 0x10004E10, (void *))(g_draw_obj_guess);
            EQ_CDECL(void, 0x100039E0, (int))(0);
            if (altResult)
            {
                result = 3;
            }
            return result;
        }
    } while (true);
}

// This image's addresses for the function below (file-local): the Trilogy upgrade screen's own strings and values.
#define g_trilogy_bmp_guess             EQ_ARRAY(char, 0x10048534)              // g_trilogy_bmp_guess: "..."
#define g_trilogy_item1_guess           EQ_GLOBAL(char *, 0x10055AF8)           // g_trilogy_item1_guess: list item 1 (copied to the buffer)
#define g_trilogy_item2_guess           EQ_GLOBAL(char *, 0x10055AFC)           // g_trilogy_item2_guess: list item 2
#define g_trilogy_item3_guess           EQ_GLOBAL(char *, 0x10055B00)           // g_trilogy_item3_guess: list item 3
#define g_trilogy_item4_guess           EQ_GLOBAL(char *, 0x10055B04)           // g_trilogy_item4_guess: list item 4
#define g_trilogy_num_guess             EQ_GLOBAL(int, 0x10055B08)              // g_trilogy_num_guess: the int the 0x7b event prints, twice
#define g_trilogy_msg_bad_guess         EQ_GLOBAL(char *, 0x10055B0C)           // g_trilogy_msg_bad_guess: the message for a text not starting with B or b
#define g_trilogy_confirm_msg_guess     EQ_GLOBAL(char *, 0x10055B10)           // g_trilogy_confirm_msg_guess: the question eqmainRunDialog_guess asks for a B text

// FUNCTION: EQMAIN 0x10020620
// Trilogy upgrade screen: asks to confirm a B-prefixed entry (10+ chars) before sending it to the server
// @early-stop: the dialog tail merge and deferred cdecl stack cleanup still differ.
// __stdcall, callee pops 0. The news and eq_trilogy_upgrad.bmp upgrade screen: RunUpgradeScreen_guess (0x1001FD40) with four
// list items and a B code. It draws news.bmp and eq_trilogy_upgrad.bmp on the draw object, makes two text rows, the list and
// the edit box (EQMain_CreateLabelFixed_guess writes g_upg_edit_guess), sets g_eqmain_flag_ce78_guess to 1, then loops: RdpReceiveScreenDispatch_guess gives an event (2 clears
// a pending screen, 0x12 ends, 0x7b prints) and PumpMessagesAndEditText_guess a message. A text of 10 characters or more starting with B or b
// needs eqmainRunDialog_guess's answer (nothing happens on 0); any other such text gets the g_trilogy_msg_bad_guess message. The loop ends when
// done is set; returns g_chat_status_guess, or 3 once the 0x12 event set gotEvent12. event is the whole of EAX (the original TESTs
// EAX), so the event is an int. The original passes two extra stack words to EQMain_FitFontSize_guess and eqmain_draw_text_guess, and one to
// ClearBlinkAndPump_guess.
unsigned short __stdcall RunTrilogyUpgradeScreen_guess(void)
{
    unsigned short result;
    bool dialogOpen;
    bool awaitingReply;
    bool done;
    bool gotEvent12;
    int event;
    int ret_text_a;
    int ret_text_b;
    char *entryText;
    char confirmed;
    void *pHVar9;
    void *pHVar10;
    int *block;
    int *_Memory;

    dialogOpen = false;
    awaitingReply = false;
    result = g_default_result_guess;
    done = false;
    gotEvent12 = false;
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_news_bmp_guess, 0x10);
    EQ_CDECL(void, 0x100056C0, (void *, char *, int, int))
        (g_draw_obj_guess, EQ_CDECL(char *, 0x100210F0, (const char *))(g_trilogy_bmp_guess), 0, 0);
    pHVar9 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x14, 1);
    pHVar10 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x18, 1);
    ret_text_a = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_a_guess, 0x19, 0);
    ret_text_b = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_b_guess, ret_text_a, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar9, 0, g_upg_text_a_guess, ret_text_b, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar10, 0, g_upg_text_b_guess, ret_text_b, 0);
    block = EQ_CDECL(int *, 0x10008C10, (void *, int, int, int, int, int, int, int, int))
        (g_draw_obj_guess, 0x34, 0x4d, 0x208, 0x12c, 0xf, 0xc81000, g_eqmain_dlg_arg_guess, 0);
    strcpy(g_upg_buf_guess, g_trilogy_item1_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))(block, g_upg_buf_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    strcpy(g_upg_buf_guess, g_trilogy_item2_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))(block, g_upg_buf_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    strcpy(g_upg_buf_guess, g_trilogy_item3_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))(block, g_upg_buf_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    strcpy(g_upg_buf_guess, g_trilogy_item4_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))(block, g_upg_buf_guess);
    g_upg_edit_guess = 0;
    EQ_CDECL(void, 0x10011970, (int, int, char *, unsigned int *, unsigned int, char *))
        (0xb8, 0x132, g_upg_edit_text_guess, &g_upg_edit_guess, 0x1a, 0);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        event = EQ_STDCALL(int, 0x100220F0, (void))();
        if (event != 0)
        {
            awaitingReply = false;
        }
        switch (event)
        {
        case 2:
            if (dialogOpen)
            {
                dialogOpen = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
            break;
        case 0x12:
            gotEvent12 = true;
            done = true;
            break;
        case 0x7b:
            if (dialogOpen)
            {
                dialogOpen = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(int, 0x10034117, (char *, char *, ...))
                (g_upg_buf_guess, g_upg_fmt_guess, g_trilogy_num_guess, g_trilogy_num_guess);
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
            done = true;
            break;
        }
        _Memory = (int *)EQ_STDCALL(int, 0x10003BD0, (void))();
        if (_Memory != 0)
        {
            if (*_Memory != 0)
            {
                if ((*_Memory == 4) && ((char)_Memory[2] == '\x1b'))
                {
                    EQ_STDCALL(void, 0x10003A00, (void))();
                    done = true;
                }
            }
            else
            {
                if ((void *)_Memory[2] == pHVar9)
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))
                        (4, 0x1b, 0, 0);
                }
                else if (((void *)_Memory[2] == pHVar10) && (!awaitingReply))
                {
                    entryText = *(char **)(g_upg_edit_guess + 0x20);
                    if (strlen(entryText) == 0)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_empty_guess, g_str_888_guess, 0);
                    }
                    else if (strlen(entryText) < 10)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_short_guess, g_str_888_guess, 0);
                    }
                    else
                    {
                        if (((*entryText != 'B') && (*entryText != 'b')) ||
                            (confirmed = EQ_CDECL(char, 0x10001FF0, (void *, char *, char *, char *))
                                (g_draw_obj_guess, g_trilogy_confirm_msg_guess, g_eqmain_text_b_guess,
                                 g_eqmain_text_a_guess),
                             confirmed != 0))
                        {
                            if (**(char **)(g_upg_edit_guess + 0x20) == 'B' ||
                                **(char **)(g_upg_edit_guess + 0x20) == 'b')
                            {
                                awaitingReply = true;
                                EQ_CDECL(void, 0x10024F90, (short))(0x6d);
                                entryText = EQ_CDECL(char *, 0x100211B0, (char *))(*(char **)(g_upg_edit_guess + 0x20));
                                EQ_CDECL(void, 0x10024FF0, (char *))(entryText);
                                EQ_STDCALL(void, 0x100213F0, (void))();
                                strcpy(g_upg_buf_guess, g_upg_code_guess);
                                EQ_CDECL(void, 0x10003000, (char *))(g_upg_buf_guess);
                                dialogOpen = true;
                            }
                            else
                            {
                                EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                                    (g_draw_obj_guess, g_trilogy_msg_bad_guess, g_str_888_guess, 0);
                            }
                        }
                    }
                }
            }
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (done)
        {
            if (dialogOpen)
            {
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            g_eqmain_flag_ce78_guess = 0;
            EQ_CDECL(void, 0x10008CE0, (void *))(block);
            EQ_CDECL(void, 0x10004E10, (void *))(g_draw_obj_guess);
            EQ_CDECL(void, 0x100039E0, (int))(0);
            if (gotEvent12)
            {
                result = 3;
            }
            return result;
        }
    } while (true);
}

// This image's addresses for the function below (file-local); the Velious screen's are above.
#define g_luclin_bmp_guess      EQ_ARRAY(char, 0x1004851C)              // g_luclin_bmp_guess: "..."
#define g_luclin_item1_guess    EQ_GLOBAL(char *, 0x10055AC0)           // g_luclin_item1_guess: list item 1
#define g_luclin_item2_guess    EQ_GLOBAL(char *, 0x10055AC4)           // g_luclin_item2_guess: list item 2
#define g_luclin_item3_guess    EQ_GLOBAL(char *, 0x10055AC8)           // g_luclin_item3_guess: list item 3
#define g_luclin_num_guess      EQ_GLOBAL(int, 0x10055AD0)              // g_luclin_num_guess: the int the 0x79 message prints, twice
#define g_luclin_msg_bad_guess  EQ_GLOBAL(char *, 0x10055ADC)           // g_luclin_msg_bad_guess: the message for an entry not starting with L

// FUNCTION: EQMAIN 0x100201B0
// Luclin upgrade screen: sends an L-prefixed entry of 10+ chars to the server, closes on server event 0x79
// __stdcall, callee pops 0. The Luclin upgrade screen: RunUpgradeScreen_guess (0x1001FD40) with eq_luclin_upgrad.bmp,
// three list items (g_luclin_item1_guess, ac4, ac8) and an L or l entry in place of V or v. Event 0x79 (not 0x6c) prints
// g_luclin_num_guess twice through g_upg_fmt_guess. Returns g_chat_status_guess, or 3 once the 0x12 event set gotEvent12. event is the whole
// of EAX (the original TESTs EAX), so the event is an int. The original passes two extra stack words to EQMain_FitFontSize_guess and
// eqmain_draw_text_guess, and one to ClearBlinkAndPump_guess.
unsigned short __stdcall RunLuclinUpgradeScreen_guess(void)
{
    unsigned short result;
    bool dialogOpen;
    bool awaitingReply;
    bool done;
    bool gotEvent12;
    int event;
    int ret_text_a;
    int ret_text_b;
    char *entryText;
    void *pHVar9;
    void *pHVar10;
    int *block;
    int *_Memory;

    result = g_default_result_guess;
    dialogOpen = false;
    awaitingReply = false;
    done = false;
    gotEvent12 = false;
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_news_bmp_guess, 0x10);
    EQ_CDECL(void, 0x100056C0, (void *, char *, int, int))
        (g_draw_obj_guess, EQ_CDECL(char *, 0x100210F0, (const char *))(g_luclin_bmp_guess), 0, 0);
    pHVar9 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x14, 1);
    pHVar10 = EQ_CDECL(void *, 0x10001520, (void *, int, int, int, int))
        (g_draw_obj_guess, 0, 0, 0x18, 1);
    ret_text_a = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_a_guess, 0x19, 0);
    ret_text_b = EQ_CDECL(int, 0x100080B0, (void *, int, int, char *, int, int))
        (g_draw_obj_guess, 0x64, 0x22, g_upg_text_b_guess, ret_text_a, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar9, 0, g_upg_text_a_guess, ret_text_b, 0);
    EQ_CDECL(void, 0x100017B0, (void *, int, char *, int, int))
        (pHVar10, 0, g_upg_text_b_guess, ret_text_b, 0);
    block = EQ_CDECL(int *, 0x10008C10, (void *, int, int, int, int, int, int, int, int))
        (g_draw_obj_guess, 0x34, 0x4d, 0x208, 0x12c, 0xf, 0xc81000, g_eqmain_dlg_arg_guess, 0);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_luclin_item1_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_luclin_item2_guess));
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, g_choice_delim_guess);
    EQ_CDECL(void, 0x10009370, (int *, char *))
        (block, strcpy(g_upg_buf_guess, g_luclin_item3_guess));
    g_upg_edit_guess = 0;
    EQ_CDECL(void, 0x10011970, (int, int, char *, unsigned int *, unsigned int, char *))
        (0xb8, 0x132, g_upg_edit_text_guess, &g_upg_edit_guess, 0x1a, 0);
    g_eqmain_flag_ce78_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    do
    {
        event = EQ_STDCALL(int, 0x100220F0, (void))();
        if (event != 0)
        {
            awaitingReply = false;
        }
        if (event == 2)
        {
            if (dialogOpen)
            {
                dialogOpen = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
        }
        else if (event == 0x12)
        {
            gotEvent12 = true;
            done = true;
        }
        else if (event == 0x79)
        {
            if (dialogOpen)
            {
                dialogOpen = false;
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            EQ_CDECL(int, 0x10034117, (char *, char *, ...))
                (g_upg_buf_guess, g_upg_fmt_guess, g_luclin_num_guess, g_luclin_num_guess);
            EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                (g_draw_obj_guess, g_upg_buf_guess, g_str_888_guess, 0);
            done = true;
        }
        _Memory = (int *)EQ_STDCALL(int, 0x10003BD0, (void))();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                if ((void *)_Memory[2] == pHVar9)
                {
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))
                        (4, 0x1b, 0, 0);
                }
                else if (((void *)_Memory[2] == pHVar10) && (!awaitingReply))
                {
                    entryText = *(char **)(g_upg_edit_guess + 0x20);
                    if (strlen(entryText) == 0)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_empty_guess, g_str_888_guess, 0);
                    }
                    else if (strlen(entryText) < 10)
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_upg_msg_short_guess, g_str_888_guess, 0);
                    }
                    else if ((*entryText == 'L') || (*entryText == 'l'))
                    {
                        awaitingReply = true;
                        EQ_CDECL(void, 0x10024F90, (short))
                            (0x6d);
                        entryText = EQ_CDECL(char *, 0x100211B0, (char *))
                            (*(char **)(g_upg_edit_guess + 0x20));
                        EQ_CDECL(void, 0x10024FF0, (char *))
                            (entryText);
                        EQ_STDCALL(void, 0x100213F0, (void))();
                        EQ_CDECL(void, 0x10003000, (char *))
                            (strcpy(g_upg_buf_guess, g_upg_code_guess));
                        dialogOpen = true;
                    }
                    else
                    {
                        EQ_CDECL(void, 0x10001FF0, (void *, char *, char *, char *))
                            (g_draw_obj_guess, g_luclin_msg_bad_guess, g_str_888_guess, 0);
                    }
                }
            }
            else if ((*_Memory == 4) && ((char)_Memory[2] == '\x1b'))
            {
                EQ_STDCALL(void, 0x10003A00, (void))();
                done = true;
            }
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (done)
        {
            if (dialogOpen)
            {
                EQ_STDCALL(void, 0x10003090, (void))();
            }
            g_eqmain_flag_ce78_guess = 0;
            EQ_CDECL(void, 0x10008CE0, (void *))(block);
            EQ_CDECL(void, 0x10004E10, (void *))(g_draw_obj_guess);
            EQ_CDECL(void, 0x100039E0, (int))(0);
            if (gotEvent12)
            {
                result = 3;
            }
            return result;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x10020ae0
// Shows the EverQuest start screen with its two introductory text lines.
// __cdecl, callee pops 0. Draws the start screen on the draw object g_eqmain_ceb8_guess: the EQVstart.bmp image (flags 0x10),
// then `text` at y 0xf0 and g_eqmain_str_559b8_guess at y 0x104, both at x 0x140 in colour 0x3cd9f5. The two text calls also push
// 2 and 0x258, which EQMain_DrawCenteredText_guess's 5-parameter view does not read. Sets g_eqmain_flag_ce78_guess to 1, then the original tail-jumps
// to TickBlinkAndCursor_guess (__stdcall, no args); here that is a CALL followed by RET.
void __cdecl ShowStartScreen_guess(const char *text)
{
    EQ_CDECL(void, 0x10007BF0, (void *, char *, int))
        (g_draw_obj_guess, g_eqmain_evstart_bmp_guess, 0x10);
    EQ_CDECL(void, 0x10008390, (void *, int, int, int, const char *, int, int))
        (g_draw_obj_guess, 0x140, 0xf0, 0x3cd9f5, text, 2, 0x258);
    EQ_CDECL(void, 0x10008390, (void *, int, int, int, const char *, int, int))
        (g_draw_obj_guess, 0x140, 0x104, 0x3cd9f5, g_eqmain_str_559b8_guess, 2, 0x258);
    g_eqmain_busy_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
}

// FUNCTION: EQMAIN 0x10020b50
// Loads the front-end settings from eqfecfg.cfg.
// ORIGINAL FILE: unknown
// __fastcall: obj in ECX, no stack args, callee pops 0. Sets the vtable and opens eqfecfg.cfg with the mode at
// DAT_1004857c. On success it reads the whole file into a zeroed buffer of size + 1 bytes (size = ftell after
// SEEK_END, or 1 when the file is empty). When the open gives 0 or -1 it opens the file again with the mode at
// DAT_1004856c, sets size 1 and returns a zeroed 2-byte buffer without reading. Returns obj in both paths.
#pragma intrinsic(memset)
EqfeCfg_guess * __fastcall EqfeCfgLoad_guess(EqfeCfg_guess *obj)
{
    FILE *fp;
    void *buf;

    obj->vtable = (void *)0x10041f70;
    fp = EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))
        ((const char *)0x10048570, (const char *)0x1004857c);
    obj->file = fp;
    if (fp != 0 && fp != (FILE *)-1) {
        EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(fp, 0, 2);
        obj->size = EQ_CDECL(long, 0x10033E8F, (FILE *))(obj->file);
        EQ_CDECL(void, 0x100346A6, (FILE *))(obj->file);
        if (obj->size == 0)
            obj->size = 1;
        buf = EQ_CDECL(void *, 0x1003342E, (unsigned int))(obj->size + 1);
        obj->buf = buf;
        memset(buf, 0, obj->size + 1);
        EQ_CDECL(unsigned int, 0x100342A2, (void *, unsigned int, unsigned int, FILE *))
            (obj->buf, obj->size, 1, obj->file);
        return obj;
    }
    obj->file = EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))
        ((const char *)0x10048570, (const char *)0x1004856c);
    obj->size = 1;
    buf = EQ_CDECL(void *, 0x1003342E, (unsigned int))(obj->size + 1);
    obj->buf = buf;
    memset(buf, 0, obj->size + 1);
    return obj;
}
#pragma function(memset)

// FUNCTION: EQMAIN 0x10020c30
// Finalizes the settings loader and releases it when requested.
// ORIGINAL FILE: unknown
// __thiscall in the original, written __fastcall: this in ECX, EDX dummy, one stack arg (the flags byte), callee pops 4.
// Scalar deleting destructor of the eqfecfg.cfg loader: runs EqfeCfgSaveClose_guess (0x10020c50) on it, then frees it
// with FUN_10033800 (0x10033800, __cdecl, one argument) when bit 0 of flags is set. Returns self on both paths. The
// original never writes EDX before the call, so the call gets the EDX it was entered with.
EqfeCfg_guess * __fastcall EqfeCfgScalarDeletingDtor_guess(EqfeCfg_guess *self, int edx_unused, unsigned char flags)
{
    EQ_THISCALL(void, 0x10020C50, (EqfeCfg_guess *, int))(self, edx_unused);
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(self);
    return self;
}

// FUNCTION: EQMAIN 0x10020c50
// Writes the current settings and closes the settings file.
// ORIGINAL FILE: unknown
// __fastcall: obj in ECX, no stack args, callee pops 0. Sets the vtable, rewinds the file, writes size bytes of the
// buffer at +0x0c to it (fwrite(buf, 1, size, file)), closes the file, frees the buffer and sets +0x0c to 0.
// Returns nothing.
void __fastcall EqfeCfgSaveClose_guess(EqfeCfg_guess *obj)
{
    obj->vtable = (void *)0x10041f70;
    EQ_CDECL(void, 0x100346A6, (FILE *))(obj->file);
    EQ_CDECL(unsigned int, 0x10034169, (void *, unsigned int, unsigned int, FILE *))
        (obj->buf, 1, obj->size, obj->file);
    EQ_CDECL(int, 0x100343B9, (FILE *))(obj->file);
    EQ_CDECL(void, 0x10033568, (void *))(obj->buf);
    obj->buf = 0;
}

// FUNCTION: EQMAIN 0x10020ca0
// Reads the leading integer associated with a key in the settings text.
// ORIGINAL FILE: unknown
// __thiscall in the original, written __fastcall: this in ECX, EDX dummy, one stack arg (the key), callee pops 4.
// Looks the key up in the buffer at +0x0c with EqfeCfgFindBlock_guess (__thiscall). When that gives a pointer, returns the
// result of atolLeadingDigits_guess on it (Ghidra: __stdcall, but the original still loads ECX = this before the call, so it
// is called in the thiscall form); otherwise returns -1. Returns the value of the call on both paths.
int __fastcall EqfeCfgFindValue_guess(EqfeCfg_guess *self, int edx_unused, char *key)
{
    char *text;

    text = EQ_METHOD(1, char *, 0x10020F30, (EqfeCfg_guess *, char *), self)(key);
    if (text != 0)
    {
        return EQ_METHOD(1, int, 0x10020F80, (EqfeCfg_guess *, char *), self)(text);
    }
    return -1;
}

// FUNCTION: EQMAIN 0x10020cd0
// Adds or replaces an integer setting in eqfecfg.cfg.
// ORIGINAL FILE: unknown
// __thiscall in the original, written __fastcall: this in ECX, EDX dummy, two stack args (the key, then the value),
// callee pops 8. Returns 0 on every path. Finds the key in the text at +0x0c with EqfeCfgFindBlock_guess. Not found: appends
// key + the string at 0x10048584 + the value in decimal + the string at 0x10048580 to the text (+0x14 grows by that
// length), makes a malloc'd copy of the text plus the appendix at +0x10, frees the old text, closes the file at +0x08
// and rewrites eqfecfg.cfg from the new text with a fresh FUN_1003407f. Found with another value (EqfeCfgFindValue_guess): blanks
// the "[ ... }" block around the key, compacts with CompactText_guess and calls itself, which then takes the append path.
// +0x10 is f10 in EqfeCfg_guess (EqfeCfgLoad_guess never writes it): this function keeps the new text buffer there.
#pragma intrinsic(memset)
int __fastcall EqfeCfgSetValue_guess(EqfeCfg_guess *self, int edx_unused, char *key, int value)
{
    char valueText[64];
    char appendix[1024];
    char ch;
    char *cursor;
    char *probe;
    void *newText;
    int newSize;
    unsigned int allocSize;

    cursor = EQ_METHOD(1, char *, 0x10020F30, (EqfeCfg_guess *, char *), self)(key);
    if (cursor == 0)
    {
        memset(valueText, 0, sizeof(valueText));
        memset(appendix, 0, sizeof(appendix));
        strcat(appendix, key);
        strcat(appendix, (char *)0x10048584);
        cursor = EQ_CDECL(char *, 0x10035089, (int, char *, int))(value, valueText, 10);
        strcat(appendix, cursor);
        strcat(appendix, (char *)0x10048580);
        newSize = self->size + strlen(appendix);
        self->size = newSize;
        newText = EQ_CDECL(void *, 0x1003342E, (unsigned int))(newSize + 1);
        allocSize = self->size + 1;
        self->f10 = (unsigned int)newText;
        memset(newText, 0, allocSize);
        strcpy((char *)self->f10, (char *)self->buf);
        strcat((char *)self->f10, appendix);
        EQ_CDECL(void, 0x10033568, (void *))(self->buf);
        self->buf = (void *)self->f10;
        EQ_CDECL(int, 0x100343B9, (FILE *))(self->file);
        self->file = EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))
            ((const char *)0x10048570, (const char *)0x10047f74);
        EQ_CDECL(unsigned int, 0x10034169, (const void *, unsigned int, unsigned int, FILE *))
            (self->buf, 1, self->size, self->file);
        return 0;
    }
    newSize = EQ_METHOD(1, int, 0x10020CA0, (EqfeCfg_guess *, char *), self)(key);
    if (value != newSize)
    {
        ch = *cursor;
        while (ch != '[')
        {
            probe = cursor - 1;
            cursor = cursor - 1;
            ch = *probe;
        }
        *cursor = ' ';
        do
        {
            *cursor = ' ';
            probe = cursor + 1;
            cursor = cursor + 1;
        } while (*probe != '}');
        *cursor = ' ';
        EQ_METHOD(0, void, 0x10020FD0, (EqfeCfg_guess *), self)();
        EqfeCfgSetValue_guess(self, 0, key, value);
    }
    return 0;
}
#pragma function(memset)

// FUNCTION: EQMAIN 0x10020ef0
// Opens or closes access to the eqfecfg.cfg settings loader.
// @early-stop: final cleanup stores immediate zero before clearing EAX; original clears EAX first and stores it.
// __cdecl, one char arg (only its low byte is read: CL at [ESP + 4]), callee pops 0. Keeps the eqfecfg.cfg loader
// (0x18 bytes, global at 0x100f2074) alive: a nonzero arg creates it when there is none (the global stays 0 when
// operator new fails), a zero arg destroys it through vtable slot 0 with flag 1 when there is one. Returns the
// global's value after the call on every path.
#define g_eqfecfg_loader_guess EQ_GLOBAL(EqfeCfg_guess *, 0x100F2074)

EqfeCfg_guess * __cdecl EqfeCfgSetActive_guess(char active)
{
    EqfeCfg_guess *obj;

    obj = g_eqfecfg_loader_guess;
    if (obj == 0)
    {
        if (active == 0)
            return g_eqfecfg_loader_guess;
        obj = (EqfeCfg_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x18);
        if (obj != 0)
        {
            obj = EqfeCfgLoad_guess(obj);
            g_eqfecfg_loader_guess = obj;
            return obj;
        }
    }
    else
    {
        if (active != 0)
            return g_eqfecfg_loader_guess;
        (*(void (__fastcall **)(EqfeCfg_guess *, void *, int))obj->vtable)(obj, obj->vtable, 1);
    }
    return g_eqfecfg_loader_guess = 0;
}

// FUNCTION: EQMAIN 0x10020f30
// Locates a keyed block in the settings text.
// __thiscall on the eqfecfg.cfg loader (this in ECX; the EDX dummy only keeps __fastcall's register use), one stack
// argument (callee pops 4). Finds `key` in the file text (obj->buf) with strstr, takes the first '{' after it with
// strchr, and returns the first char after that brace which is not LF, CR or space. Returns 0 when the file isn't open
// (obj->file == 0). A NULL from strstr or strchr is returned as it is (EAX already holds it, so no XOR).
char * __fastcall EqfeCfgFindBlock_guess(EqfeCfg_guess *obj, int unusedEdx, char *key)
{
    char ch;
    char *p;

    if (obj->file == 0)
        return 0;
    p = EQ_CDECL(char *, 0x10035FD0, (char *, char *))((char *)obj->buf, key);
    if (p == 0)
        return p;
    p = EQ_CDECL(char *, 0x10033670, (char *, int))(p, 0x7b);
    if (p == 0)
        return p;
    do
    {
        ch = p[1];
        p = p + 1;
    } while (ch == '\n' || ch == '\r' || ch == ' ');
    return p;
}

// FUNCTION: EQMAIN 0x10020f80
// Converts leading decimal digits to a number; its game purpose is unclear.
// __stdcall, one stack argument (callee pops 4). Copies the leading run of decimal digits of `text` into a 16-byte
// stack buffer (no length check: a run of 16 or more digits overflows it) and returns atol() of the copy. A string
// that does not start with a digit gives atol("") = 0. The atol result is returned unchanged (EAX passes through).
// @early-stop: cur is MOV AL + MOVSX EDX,AL (the original has one MOVSX EDX,[ESI+1]) and the push lands before the last INC (31 insns vs 30).
long __stdcall atolLeadingDigits_guess(char *text)
{
    int ok;
    int n;
    char cur;       // char, as Ghidra's cVar1; an int folds the load but takes EAX, not EDX
    char digits[16];

    n = 0;
    ok = EQ_CDECL(int, 0x10033861, (int))((int)*text);
    while (ok != 0) {
        cur = text[1];
        digits[n++] = *text++;
        ok = EQ_CDECL(int, 0x10033861, (int))((int)cur);
    }
    digits[n] = '\0';
    return EQ_CDECL(long, 0x10034FF3, (char *))(digits);
}

// The zero fills below are VC6's inline rep stosd, which needs memset intrinsic (scoped to this function).
#pragma intrinsic(memset)

// FUNCTION: EQMAIN 0x10020fd0
// Compacts edited settings text; its game purpose is unclear.
// @early-stop: 5 instructions still differ: end-pointer calculation uses mov/add instead of mov/lea, and one pointer compare uses EDI instead of EAX.
// __fastcall (`this` in ECX), no stack arguments; returns nothing. Walks text[0..len): at a space, if a '[' comes later
// the bytes from that '[' are copied over the gap (as many as the gap is long) and their old place zeroed; with no '['
// the text is cut at the space. Then len = strlen(text), buf = a malloc'd copy of it (zeroed first), and the old text is
// freed and replaced by buf.
void __fastcall CompactText_guess(TextBuf_guess *self)
{
    char *p = self->text; // EDI: scan position
    char *q;            // ECX: '[' search position; the copy source
    char *dst;          // EAX: copy destination, starts at the space
    char *bracket;      // ESI: where the copy stops
    char *end = p + self->len; // EDX: text + len
    char c;             // BL: the byte after q
    char *buf;

    for (; p != end; end = self->text + self->len) {
        if (*p != ' ') {
            p++;
        } else {
            dst = p;
            q = p;
            do {
                if (q == end) {
                    // as original: this check and the one after the do-while share the zero block (one je target)
                    goto zero;
                }
                q++;
                c = *q;
            } while (c != '[');
            if (q == end) {
                // as original: same shared zero block as the check inside the do-while (one je target)
                goto zero;
            }
            bracket = q;
            if (dst != bracket) {
                do {
                    *dst = *q;
                    *q = '\0';
                    dst++;
                    q++;
                } while (dst != bracket);
            }
            p = dst;
        }
    }
    // as original: the loop's normal exit jumps over the zero block straight to the done tail
    goto done;
zero:
    if (p != q) {
        memset(p, 0, q - p);
    }
done:
    self->len = strlen(self->text);
    buf = EQ_CDECL(char *, 0x1003342E, (unsigned int))(self->len + 1);
    self->buf = buf;
    memset(buf, 0, self->len + 1);
    strcpy(self->buf, self->text);
    EQ_CDECL(void, 0x10033568, (char *))(self->text);
    self->text = self->buf;
}
#pragma function(memset)

// FUNCTION: EQMAIN 0x100210c0
// Thunk that initializes a global object whose game purpose is unclear.
// __stdcall, no arguments. Sets ECX to g_eqmain_lock_guess's address and jumps to ZeroBlocksCtor_guess (__fastcall, its one argument
// in ECX). The jump is a tail call, so the callee's EAX is what this returns; here it's a call that returns that EAX.
unsigned int *__stdcall Tail_100f4930_guess(void)
{
    return EQ_METHOD(0, unsigned int *, 0x10024DA0, (unsigned int *), EQ_ARRAY(unsigned int, 0x100F4930))();
}

// FUNCTION: EQMAIN 0x100210d0
// Schedules cleanup of a global object when the program exits.
// __stdcall, no arguments, no return value. Pushes the address of the 0x100210e0 thunk and calls the CRT's _atexit
// (__cdecl, one argument); the caller pops that argument with POP ECX and _atexit's result is not used.
void __stdcall Register_atexit_guess(void)
{
    EQ_CDECL(int, 0x100348C5, (void *))((void *)0x100210e0);
}

// FUNCTION: EQMAIN 0x100210e0
// Thunk for global cleanup; the object's game purpose is unclear.
// __stdcall, no stack arguments. Sets ECX to g_eqmain_lock_guess's address and jumps to FreePointerArrays_guess (__fastcall, its one argument
// in ECX). The jump is a tail call, so the callee's EAX is what this returns; here it's a call that returns that EAX.
unsigned int __stdcall Thunk_100f4930_guess(void)
{
    return EQ_METHOD(0, unsigned int, 0x10024E20, (unsigned int *), EQ_ARRAY(unsigned int, 0x100F4930))();
}

// FUNCTION: EQMAIN 0x100210f0
// Returns its first argument unchanged. __cdecl; the caller's EAX is read after the call at 23 sites.
unsigned int __cdecl Identity_guess(unsigned int value)
{
    return value;
}

// eqmain.dll's own globals (this image's addresses, not eqgame.exe's).
#define g_eqmain_connection_guess   EQ_GLOBAL(int, 0x10125848)
#define g_eqmain_state_guess        EQ_GLOBAL(unsigned int, 0x1012B918)
#define g_eqmain_string_buffer_guess EQ_ARRAY(unsigned int, 0x101155F8)

#define g_eqmain_decode_buf_guess   EQ_GLOBAL(unsigned int, 0x10104D00)

// FUNCTION: EQMAIN 0x10021110
// Decodes an encoded string into a shared buffer; its game purpose is unclear.
// __cdecl, returns the buffer in every path. The buffer is zeroed (0x10000 bytes), then encoded is decoded into it when
// FUN_10021100(encoded) is nonzero and encoded isn't empty: a byte with the high bit clear is copied; a high-bit byte also
// takes the byte after it and stores ((next ^ b) & 0x3f) ^ b. More than 0x10000 output bytes zero the buffer again.
// Bug kept: a high-bit byte always steps 2, so a NUL right after it is skipped and the scan reads on past it.
// @early-stop: VC6 clears EAX before loading the widened input byte; the original leaves its high bytes unchanged.
unsigned int *__cdecl DecodeToBuffer_guess(unsigned char *encoded)
{
    unsigned int encodedByte;
    char valid;
    int outputCount;
    int highBitMask;          /* fake match: dword temp for encodedByte & 0x80; without it the test narrows to dl and the compare to cl/dl */
    unsigned char *outputCursor;
    unsigned char *inputCursor;  /* fake match: walker copy of encoded; keeps encoded in edi until the entry test, as in the original */
    unsigned int *clearCursor;

    clearCursor = &g_eqmain_decode_buf_guess;
    for (outputCount = 0x4000; outputCount != 0; outputCount = outputCount - 1)
    {
        *clearCursor = 0;
        clearCursor = clearCursor + 1;
    }
    outputCount = 0;
    valid = EQ_CDECL(char, 0x10021100, (unsigned char *))(encoded);
    if (valid == 0 || (inputCursor = encoded, outputCursor = (unsigned char *)&g_eqmain_decode_buf_guess, *inputCursor == 0))
        return &g_eqmain_decode_buf_guess;
    do
    {
        encodedByte = *inputCursor;
        highBitMask = encodedByte & 0x80;
        if (highBitMask)
        {
            if (highBitMask != 0x80)
                return &g_eqmain_decode_buf_guess;
            if (0x10000 < ++outputCount)
                break;
            *outputCursor = (inputCursor[1] ^ encodedByte) & 0x3f ^ encodedByte;
            outputCursor = outputCursor + 1;
            inputCursor = inputCursor + 2;
        }
        else
        {
            if (0x10000 < ++outputCount)
                break;
            *outputCursor = encodedByte;
            inputCursor = inputCursor + 1;
            outputCursor = outputCursor + 1;
        }
        if (*inputCursor == 0)
            return &g_eqmain_decode_buf_guess;
    } while (true);
    clearCursor = &g_eqmain_decode_buf_guess;
    for (outputCount = 0x4000; outputCount != 0; outputCount = outputCount - 1)
    {
        *clearCursor = 0;
        clearCursor = clearCursor + 1;
    }
    return &g_eqmain_decode_buf_guess;
}

// FUNCTION: EQMAIN 0x100211b0
// Copies a string into the shared front-end buffer.
// __cdecl, returns a pointer. Zeroes the 0x10000-byte global buffer at 0x101155F8 with a loop of 0x4000 dword stores (VC6
// turns it into rep stosd; memset at this size is a call, not inline), then strcpy's source into it (VC6's inline strcpy:
// the NUL is copied too) and returns the buffer's address. The copy is not bounded: a string of 0x10000 bytes or more (NUL
// included) runs past the buffer, as in the original.
unsigned int * __cdecl CopyStringToBuffer_guess(char *src)
{
    unsigned int *clearCursor;
    int wordCount;

    clearCursor = g_eqmain_string_buffer_guess;
    for (wordCount = 0x4000; wordCount != 0; wordCount = wordCount - 1) {
        *clearCursor = 0;
        clearCursor = clearCursor + 1;
    }
    strcpy((char *)g_eqmain_string_buffer_guess, src);
    return g_eqmain_string_buffer_guess;
}

// This image's addresses for the function below (file-local). The CRT's _strtok is at 0x10033996, _atoi at 0x1003507e.
#define g_setup_flag_guess      EQ_GLOBAL(char, 0x101258B0)             // g_login_ready_guess: 1 once the setup has run
#define g_eqmain_delim_8590_guess     EQ_ARRAY(char, 0x10048590)              // g_eqmain_delim_8590_guess: the delimiter of the first strtok
#define g_eqmain_copybuf_guess      EQ_ARRAY(char, 0x100F4C10)              // g_eqmain_copybuf_guess: the copy of param_1 that strtok splits
#define g_eqmain_cycle_text_guess            EQ_ARRAY(char, 0x10125850)              // g_eqmain_cycle_text_guess: the first token
#define g_eqmain_port_guess      EQ_GLOBAL(int, 0x101258A0)              // g_eqmain_port_guess: atoi of the second token
#define g_str_f4b50_guess       EQ_ARRAY(char, 0x100F4B50)              // g_str_f4b50_guess: a copy of the string at g_eqmain_drive_base_guess
#define g_rdp_slot_guess        EQ_ARRAY(unsigned int, 0x10125638)      // g_eqmain_rdp_guess: the slot rdp_create_guess is given
#define g_rdp_flag_guess        EQ_GLOBAL(char, 0x10125844)             // g_rdp_flag_guess: set to 1 before rdp_create_guess

// FUNCTION: EQMAIN 0x100211F0
// Sets up a network connection from a delimited string; its game purpose is unclear.
// __cdecl, returns a byte in AL (the one caller overwrites EAX before it reads it). Returns 1 at once when
// g_setup_flag_guess is already 1. Otherwise copies text (NUL included, not bounded) into g_split_copy_guess, takes
// its first strtok token into g_tok1_guess, the next one (strtok of NULL on g_str_ce40_guess) through atoi into
// g_tok2_value_guess, copies g_str_ce40_guess to g_str_f4b50_guess, calls initMemoryPool_guess(0x2800), sets g_rdp_flag_guess
// and calls rdp_create_guess. On failure it runs cleanupMemoryPool_guess, clears the flag and returns 0; on success it
// sets the flag and returns 1. The meanings of the strings and the globals are unknown.
char __cdecl InitRdpFromString_guess(char *text)
{
    char *token;
    int createResult;

    if (g_setup_flag_guess != 1) {
        strcpy(g_eqmain_copybuf_guess, text);
        token = EQ_CDECL(char *, 0x10033996, (char *, char *))(g_eqmain_copybuf_guess, g_eqmain_delim_8590_guess);
        strcpy(g_eqmain_cycle_text_guess, token);
        token = EQ_CDECL(char *, 0x10033996, (char *, char *))((char *)0, g_eqmain_drive_base_guess);
        g_eqmain_port_guess = EQ_CDECL(int, 0x1003507E, (char *))(token);
        strcpy(g_str_f4b50_guess, g_eqmain_drive_base_guess);
        EQ_CDECL(void, 0x10028750, (unsigned int))(0x2800);
        g_rdp_flag_guess = 1;
        createResult = EQ_CDECL(int, 0x1002A460, (unsigned int *, unsigned short, int, unsigned int))(
            g_rdp_slot_guess, 0, 1, 0x40000002);
        if (createResult != 0) {
            EQ_STDCALL(void, 0x10028850, (void))();
            g_setup_flag_guess = 0;
            return 0;
        }
        g_setup_flag_guess = 1;
    }
    return 1;
}

// This image's addresses for the function below (file-local). DAT_10047f78 and "login" are the two mode strings.
#define g_login_ready_guess     EQ_GLOBAL(char, 0x101258B0)             // g_login_ready_guess: 1 once rdp_create_guess succeeded; 1 returns at once
#define g_rdp_slot_guess        EQ_GLOBAL(unsigned int, 0x10125638)     // g_eqmain_rdp_guess: the 4-byte slot rdp_create_guess is given
#define g_login_obj_guess       EQ_ARRAY(char, 0x100F4930)              // g_eqmain_lock_guess: the object CycleNameAndValueAlt_guess / CycleNameAndValue_guess are called on
#define g_str_10047f78_guess      EQ_ARRAY(char, 0x10047F78)              // DAT_10047f78: the first mode string param_1 is compared with
#define g_eqmain_login_s_login_guess       EQ_ARRAY(char, 0x10048100)              // "login": the second mode string

// FUNCTION: EQMAIN 0x100212e0
// Opens the front-end login connection for a recognized mode.
// __cdecl, returns a char (callers read AL). Returns 1 at once while g_login_ready_guess is 1. Otherwise initMemoryPool_guess(0x2800);
// then mode selects the object's mode: CycleNameAndValueAlt_guess(obj, 1) when it equals the string at 0x10047F78, CycleNameAndValue_guess(obj, 1)
// when it equals "login", and anything else returns 0 at the shared exit. rdp_create_guess(&slot, 0, 1, 0x40000002) opens the
// connection: a low word of 0 sets g_login_ready_guess and returns 1; any other value frees the pool (cleanupMemoryPool_guess),
// clears the flag and returns 0. The meanings of the mode strings, the object and the slot are unknown.
char __cdecl OpenLoginConnection_guess(unsigned char *mode)
{
    int createResult;

    if (g_login_ready_guess == 1)
    {
        return 1;
    }
    EQ_CDECL(void, 0x10028750, (unsigned int))(0x2800);
    if (strcmp((char *)mode, g_str_10047f78_guess) == 0)
    {
        EQ_METHOD(1, int, 0x10024F10, (void *, int), g_login_obj_guess)(1);
    }
    else
    {
        if (strcmp((char *)mode, g_eqmain_login_s_login_guess) == 0)
        {
            EQ_METHOD(1, int, 0x10024E90, (void *, int), g_login_obj_guess)(1);
        }
        else
        {
            // as original: unrecognized modes share the final failure return.
            goto done;
        }
    }
    g_rdp_flag_guess = 1;
    createResult = EQ_CDECL(int, 0x1002A460, (unsigned int *, unsigned short, int, unsigned int))(&g_rdp_slot_guess, 0, 1, 0x40000002);
    if ((short)createResult != 0)
    {
        EQ_STDCALL(void, 0x10028850, (void))();
        g_login_ready_guess = 0;
    }
    else
    {
        g_login_ready_guess = 1;
        return 1;
    }
done:
    return 0;
}

// FUNCTION: EQMAIN 0x100213D0
// Sends its argument's low 16 bits as a 2-byte message (callers pass 0x37, 0x40, 0x44, 0x52).
// __stdcall, void, no arguments: the callee pops nothing, so this is a plain RET. Sends the two-word record
// { address of its own first stack slot (ESP+4 at entry), 2 } over g_eqmain_connection_guess (connection_send_guess,
// flags 1 and 1). The stack slot is written as a __cdecl stand-in parameter whose address is taken: it compiles to the
// same LEA [ESP+0xc] and RET. The EAX it leaves is the callee's; no caller reads it. The meaning of the record is unknown.
void __cdecl SendStackSlot_guess(unsigned int value)
{
    EQ_CDECL(void, 0x1002C010, (int, unsigned int, unsigned int, unsigned int, unsigned int))(
        g_eqmain_connection_guess, (unsigned int)&value, 2, 1, 1);
}

// FUNCTION: EQMAIN 0x100213F0
// Sends the current front-end message to the remote service.
// __stdcall, void, no arguments. Sends the buffer at 0x10125914, g_eqmain_state_guess bytes long, over the connection
// g_eqmain_connection_guess (connection_send_guess, flags 1 and 1). The EAX it leaves is the callee's; no caller reads it.
// The meanings of the connection, the buffer and the length are unknown.
void __stdcall SendBuffer_guess(void)
{
    EQ_CDECL(void, 0x1002C010, (int, unsigned int, unsigned int, unsigned int, unsigned int))(
        g_eqmain_connection_guess, (unsigned int)EQ_ARRAY(unsigned char, 0x10125914), g_eqmain_state_guess, 1, 1);
}

// FUNCTION: EQMAIN 0x10021410
// Checks an unidentified front-end state before connection setup.
// Returns whether the global at 0x10125848 is nonzero in AL. __stdcall, no arguments; VC6 leaves the loaded
// value's upper 24 bits in EAX, as in the original.
char __stdcall Eqmain_GlobalNonzero_guess(void)
{
    unsigned int value = EQ_GLOBAL(unsigned int, 0x10125848);
    if (value == 0)
        return 0;
    return 1;
}

// FUNCTION: EQMAIN 0x10021420
// Allows the front end to receive network data when its receive state is enabled.
// __cdecl. Passes (rdp, timeout) to the RDP receive at 0x1002AE30 only while the byte at 0x10125844 is nonzero;
// otherwise returns 0 without calling. The callee's result is returned as is; the caller pops the two arguments.
int __cdecl Eqmain_RdpReceiveIfReady_guess(int rdp, unsigned int timeout)
{
    int result;

    if (g_rdp_flag_guess == 0) {
        return 0;
    }
    result = EQ_CDECL(int, 0x1002AE30, (int, unsigned int))(rdp, timeout);
    return result;
}
