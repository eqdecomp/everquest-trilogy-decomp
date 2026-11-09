// unit_10003700.cpp - eqmain.dll, the address run 0x10003700..0x10006EC0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <string.h>

// eqmain.dll's own addresses (image base 0x10000000) go through eqaddr.h's EQ_CDECL / EQ_THISCALL / EQ_GLOBAL, so
// the match build (EQD_MATCH) gets direct `call rel32` and relocated absolutes, like every other eqmain file.
#include "trilogy/eqaddr.h"

// The original's calls through an import slot (`CALL dword ptr [va]`) and through an object's vtable slot (`CALL dword ptr
// [reg + off]`, the object pushed as `this`): both __stdcall. The object is evaluated twice in EQMAIN_VCALL.
#define EQMAIN_IMPORT(ret, va, params)  ((ret (__stdcall *) params)(*(void **)(va)))
#define EQMAIN_VCALL(ret, obj, off, params) ((ret (__stdcall *) params)(*(void **)(*(int *)(obj) + (off))))

// The active text field g_eqmain_flag_cec4_guess (only the fields 0x10003BD0 reads).
struct TextField_guess
{
    int type;           // 0x00: 1 or 8 take text; 8 has ranges
    int unknown04[6];   // 0x04
    int cursor;         // 0x1C
    char *text;         // 0x20
    int capacity;       // 0x24
    int unknown28[3];   // 0x28
    int blink;          // 0x34
    unsigned int stamp; // 0x38
    char *mask;         // 0x3C: echo text ('*'), or 0
    int unknown40[4];   // 0x40
    int range;          // 0x50: current range
    int ranges[21];     // 0x54: range start offsets
};

// FUNCTION: EQMAIN 0x10003700
// Sets up the front-end drawing context, fonts, and cursor art.
// @early-stop: 17 instructions still differ, mainly register selection and push/store scheduling around the fields and calls.
// __stdcall, no args, returns the context (EAX) or 0. Asks g_eqmain_ui_arg3_guess for an interface (vtable slot 0x30, out in
// interfacePtr), fills the 0x28-byte context (0x280, 0x1E0, 0x10, the interface, g_eqmain_ui_arg3_guess), tests the low byte of
// PackFsDetectColorFormat16_guess's result, sets the interface's slots 0x74, 0x44 and 0x68, creates five Arial fonts through the import
// slot 0x1004001C, then loads cursor.bmp into DAT_1004CE9C and DAT_1004CE88.
unsigned int * __stdcall CreateUiContext_guess(void)
{
    typedef int (__stdcall *CreateFontFn_guess)(int, int, int, int, int, int, int, int, int, int, int, int, int, char *);
    void *obj;
    CreateFontFn_guess createFont;
    int result;
    int *cursorBitmap;
    unsigned int *record;
    void *interfacePtr;
    unsigned int interfaceData[4];

    obj = EQ_GLOBAL(void *, 0x1004CEB4);
    interfacePtr = 0;
    interfaceData[0] = 4;
    result = EQMAIN_VCALL(int, obj, 0x30, (void *, unsigned int *, void **))(obj, interfaceData, &interfacePtr);
    if (result != 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A7C));
        return 0;
    }
    EQ_CDECL(void, 0x10028330, (void *))(&EQ_GLOBAL(int, 0x1004CE90));
    EQ_CDECL(void, 0x10028330, (void *))(&EQ_GLOBAL(int, 0x1004CE60));
    record = (unsigned int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x28);
    if (record != 0)
    {
        record[0] = 0x280;
        record[1] = 0x1E0;
        record[2] = 0x10;
        /* fake match: keep the owner read live with the field stores so VC6 follows the original register order. */
        {
            void *owner = EQ_GLOBAL(void *, 0x1004CEB4);
            record[3] = (unsigned int)interfacePtr;
            record[4] = (unsigned int)owner;
        }
        EQ_CDECL(void, 0x10028330, (void *))(record + 7);
        EQ_CDECL(void, 0x10028340, (void *, int, void *))(&EQ_GLOBAL(int, 0x1004CE90), 0, record);
    }
    if (EQ_CDECL(char, 0x10007D10, (void *, void *))(record, &EQ_GLOBAL(int, 0x1004CEA8)) == 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A80));
        return 0;
    }
    interfaceData[2] = EQ_CDECL(unsigned int, 0x10007E60, (unsigned int, unsigned int, unsigned int))
        (0xFF, 0, 0xFF) & 0xFFFF;
    interfaceData[3] = EQ_CDECL(unsigned int, 0x10007E60, (unsigned int, unsigned int, unsigned int))
        (0xFF, 0, 0xFF) & 0xFFFF;
    EQMAIN_VCALL(void, record[3], 0x74, (void *, int, unsigned int *))((void *)record[3], 8, &interfaceData[2]);
    EQMAIN_VCALL(void, record[3], 0x44, (void *, unsigned int *))((void *)record[3], &interfaceData[1]);
    createFont = (CreateFontFn_guess)*(void **)0x1004001C;
    EQ_GLOBAL(int, 0x1004CED0) = createFont
        (0x19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char *)0x100463C4);
    EQ_GLOBAL(int, 0x1004CED4) = createFont
        (0x14, 0, 0, 0, 0x258, 0, 0, 0, 0, 0, 0, 0, 0, (char *)0x100463C4);
    EQ_GLOBAL(int, 0x1004CEC8) = createFont
        (0x0E, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char *)0x100463C4);
    EQ_GLOBAL(int, 0x1004CECC) = createFont
        (0x0C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, (char *)0x100463C4);
    EQ_GLOBAL(int, 0x1004CED8) = createFont
        (0x10, 0, 0, 0, 0x258, 0, 0, 0, 0, 0, 0, 0, 0, (char *)0x100463C4);
    EQMAIN_VCALL(void, record[3], 0x68, (void *, unsigned int))((void *)record[3], interfaceData[1]);
    cursorBitmap = (int *)EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x100463B8, 0x10);
    EQ_GLOBAL(int *, 0x1004CE9C) = cursorBitmap;
    EQ_GLOBAL(int *, 0x1004CE88) = EQ_CDECL(int *, 0x10006D80, (int, int, int))(cursorBitmap[0], cursorBitmap[1], 0x10);
    return record;
}

// FUNCTION: EQMAIN 0x10003930
// Shuts down front-end drawing resources and disposes of remaining UI objects.
// __stdcall, no args, no return value (no caller reads EAX). Calls BltColorFillRgb_guess(g_eqmain_ceb8_guess, 0) three times, with the
// blink pass 0x10003A00 after the first two, then deletes the five fonts CreateUiContext_guess made (DAT_1004CED4, CECC,
// CEC8, CED0, CED8) through the import slot 0x10040020. While the list head g_packfs_list_head_guess is set, list3_unlink_free_guess(&g_packfs_list_head_guess,
// head) frees the node and returns its payload (node +4); a payload whose flag (+0x10) is 0 gets its object's (+0xC) vtable
// slot 2 called first, stdcall with the object as its one argument. The payload is freed with _free. Clears g_eqmain_ceb8_guess.
// @early-stop: the first three cdecl calls clean their stack arguments separately; register choices and loop labels also differ.
void __stdcall DestroyUiContext_guess(void)
{
    typedef int (__stdcall *DeleteObjectFn_guess)(int);
    DeleteObjectFn_guess deleteObject;
    void *_Memory;

    (void)EQ_CDECL(unsigned char, 0x10006B90, (int, unsigned int))(EQ_GLOBAL(int, 0x1004CEB8), 0);
    EQ_STDCALL(void, 0x10003A00, (void))();
    (void)EQ_CDECL(unsigned char, 0x10006B90, (int, unsigned int))(EQ_GLOBAL(int, 0x1004CEB8), 0);
    EQ_STDCALL(void, 0x10003A00, (void))();
    (void)EQ_CDECL(unsigned char, 0x10006B90, (int, unsigned int))(EQ_GLOBAL(int, 0x1004CEB8), 0);
    deleteObject = *(DeleteObjectFn_guess *)0x10040020;
    deleteObject(EQ_GLOBAL(int, 0x1004CED4));
    deleteObject(EQ_GLOBAL(int, 0x1004CECC));
    deleteObject(EQ_GLOBAL(int, 0x1004CEC8));
    deleteObject(EQ_GLOBAL(int, 0x1004CED0));
    deleteObject(EQ_GLOBAL(int, 0x1004CED8));
    int *head = EQ_GLOBAL(int *, 0x1004CE90);
    while (head != 0)
    {
        _Memory = (void *)EQ_CDECL(int, 0x10028430, (int *, int *))
            (&EQ_GLOBAL(int, 0x1004CE90), head);
        if (*(int *)((int)_Memory + 0x10) == 0)
        {
            EQMAIN_VCALL(void, *(int **)((int)_Memory + 0xc), 0x8, (void *))(*(int **)((int)_Memory + 0xc));
        }
        EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        head = EQ_GLOBAL(int *, 0x1004CE90);
    }
    EQ_GLOBAL(int, 0x1004CEB8) = 0;
}

// FUNCTION: EQMAIN 0x100039E0
// Runs a front-end update pass, pumps input, and clears queued transient work.
// __stdcall, no args, no return value (no caller reads EAX). Clears g_eqmain_flag_cec4_guess, calls 0x10003A00 (the blink and
// cursor pass) and 0x10003BD0 (returns an int, ignored here), then calls 0x10028480 as __cdecl with &DAT_1004CE60.
void __stdcall ClearBlinkAndPump_guess(void)
{
    EQ_GLOBAL(int, 0x1004CEC4) = 0;
    EQ_STDCALL(void, 0x10003A00, (void))();
    EQ_STDCALL(int, 0x10003BD0, (void))();
    EQ_CDECL(void, 0x10028480, (int *))(&EQ_GLOBAL(int, 0x1004CE60));
}

// FUNCTION: EQMAIN 0x10003A00
// Updates blinking UI items and redraws the cursor over active front-end surfaces.
// @early-stop: 67 normalized instruction differences remain, chiefly cursor register selection and vtable argument scheduling.
// __stdcall, no args, no return value (no caller reads EAX). Walks the unit list at g_packfs_list_head_guess (next at +8). Per
// unit: the item blink. Each item of the unit's list (+0x1C, next at +8, item at +4) that is g_eqmain_flag_cec4_guess, has type
// 1 or 8 and was last stamped more than 500 ms ago (unsigned) gets the time at +0x38, its +0x34 flag toggled, and
// DrawQueueRecord_guess(item). Then the unit's control (+0x10, when set): with g_eqmain_flag_ce78_guess clear, its vtbl slot 0x2C as
// (this, 0, 1); with it set, the cursor pass: save the unit surface under the cursor into DAT_1004CE88+0xC, draw the
// cursor (DAT_1004CE9C+0xC, colour key) at the cursor, call slot 0x2C, then restore the saved area.
void __stdcall TickBlinkAndCursor_guess(void)
{
    struct RECT_guess
    {
        int left;
        int top;
        int right;
        int bottom;
    };
    struct POINT_guess
    {
        int x;
        int y;
    };
    int *unitNode;
    int *unit;
    int *itemNode;
    int *control;
    unsigned int DVar5;
    RECT_guess cursorRect;   // cursor rect
    RECT_guess surfaceRect;   // 0, 0, w, h
    unsigned int tickTime;
    POINT_guess cursorPosition;
    int blitEffects[25];   // DDBLTFX: dwSize at [0], dwROP at [2]

    DVar5 = (*(unsigned int (__stdcall **)(void))0x10040244)();
    tickTime = DVar5;
    for (unitNode = EQ_GLOBAL(int *, 0x1004CE90); unitNode != 0; unitNode = (int *)unitNode[2])
    {
        unit = (int *)unitNode[1];
        for (itemNode = (int *)unit[7]; itemNode != 0; itemNode = (int *)itemNode[2])
        {
            control = (int *)itemNode[1];
            if (((*control == 1) || (*control == 8)) && (EQ_GLOBAL(int *, 0x1004CEC4) == control) &&
                (500 < DVar5 - control[0xe]))
            {
                control[0xe] = DVar5;
                control[0xd] = (control[0xd] == 0);
                EQ_CDECL(void, 0x10004EC0, (int *))(control);
            }
        }
        control = (int *)unit[4];
        if (control != 0)
        {
            if (EQ_GLOBAL(char, 0x1004CE78) != 0)
            {
                (*(int (__stdcall **)(POINT_guess *))0x10040238)(&cursorPosition);
                cursorRect.left = cursorPosition.x;
                cursorRect.top = cursorPosition.y;
                cursorRect.right = EQ_GLOBAL(int *, 0x1004CE9C)[0] + cursorPosition.x;
                cursorRect.bottom = EQ_GLOBAL(int *, 0x1004CE9C)[1] + cursorPosition.y;
                surfaceRect.left = 0;
                surfaceRect.top = 0;
                surfaceRect.right = EQ_GLOBAL(int *, 0x1004CE9C)[0];
                surfaceRect.bottom = EQ_GLOBAL(int *, 0x1004CE9C)[1];
                blitEffects[0] = 100;
                blitEffects[2] = 0xcc0020;
                if (cursorRect.bottom >= 0x1df)
                {
                    cursorRect.bottom = 0x1df;
                    surfaceRect.bottom = 0x1df - cursorPosition.y;
                }
                if (cursorRect.right >= 0x27f)
                {
                    cursorRect.right = 0x27f;
                    surfaceRect.right = 0x27f - cursorPosition.x;
                }
                // Blt: save the unit surface under the cursor into DAT_1004CE88+0xC (flags 0x1020000)
                ((int (__stdcall *)(int, RECT_guess *, int, RECT_guess *, int, int *))((int **)EQ_GLOBAL(int *, 0x1004CE88)[3])[0][5])(
                    EQ_GLOBAL(int *, 0x1004CE88)[3], &surfaceRect, unit[3], &cursorRect, 0x1020000, blitEffects);
                // Blt: draw the cursor sprite DAT_1004CE9C+0xC at the cursor (flags 0x1028000, colour key)
                ((int (__stdcall *)(int, RECT_guess *, int, RECT_guess *, int, int *))((int **)unit[3])[0][5])(
                    unit[3], &cursorRect, EQ_GLOBAL(int *, 0x1004CE9C)[3], &surfaceRect, 0x1028000, blitEffects);
                ((void (__stdcall *)(int, int, int))((int **)unit[4])[0][11])(unit[4], 0, 1);
                // Blt: restore the saved area over the cursor
                ((int (__stdcall *)(int, RECT_guess *, int, RECT_guess *, int, int *))((int **)unit[3])[0][5])(
                    unit[3], &cursorRect, EQ_GLOBAL(int *, 0x1004CE88)[3], &surfaceRect, 0x1020000, blitEffects);
                DVar5 = tickTime;
            }
            else
            {
                ((void (__stdcall *)(int *, int, int))((int **)control)[0][11])(control, 0, 1);
            }
        }
    }
}

// FUNCTION: EQMAIN 0x10003bd0
// Front-end input poll: pumps Windows messages, feeds keys to the focused text field, returns the next UI event
// @early-stop: the original caches the import slots in ESI/EDI/EBX (one extra `mov esi` for timeGetTime); EQMAIN_IMPORT can't.
// __stdcall, no args, returns an int in EAX (0, or list3_unlink_free_guess's result). Pumps the message queue, then reads one key
// word through DIKeyboard_ReadOneEvent_guess (this = 0x100555E0: low byte the key, 0x8000 key up, 0x0C00 modifiers) and edits the active
// text field g_eqmain_flag_cec4_guess: +0x00 type (1 or 8 take text), +0x1C cursor, +0x20 text, +0x24 capacity, +0x34 flag, +0x38
// stamp, +0x3C mask, +0x50 range count, +0x54 range offsets. With no field, keys go to EnqueueQuad_guess as codes.
int __stdcall PumpMessagesAndEditText_guess(void)
{
    struct MSG_guess    // the Win32 MSG, 0x1C bytes
    {
        int hwnd;               // 0x00
        unsigned int message;   // 0x04
        unsigned int wParam;    // 0x08
        int lParam;             // 0x0C
        unsigned int time;      // 0x10
        int pt[2];              // 0x14
    };
    union KeyWord_guess
    {
        unsigned short value;
        struct
        {
            unsigned char low;  // key code
            unsigned char high; // 0x80 key up, 0x0C modifiers
        } bytes;
    };
    MSG_guess message;
    KeyWord_guess key;
    int c;
    int len;
    int i;
    int n;
    int node;
    TextField_guess *item;

    EQ_STDCALL(void, 0x10004D40, (void))();
    EQ_STDCALL(void, 0x10003A00, (void))();
    for (;;)
    {
        while (EQMAIN_IMPORT(int, 0x100401D0, (void *, int, unsigned int, unsigned int, unsigned int))(&message, 0, 0, 0, 0) != 0)
        {
            if (EQMAIN_IMPORT(int, 0x100401C4, (void *, int, unsigned int, unsigned int))(&message, 0, 0, 0) > 0)
            {
                EQMAIN_IMPORT(int, 0x100401CC, (void *))(&message);
                EQMAIN_IMPORT(int, 0x10040230, (void *))(&message);
            }
        }
        if (EQ_METHOD(1, unsigned char, 0x100280E0, (void *, unsigned char *), (void *)0x100555E0)(&key.bytes.low) != 0)
        {
            EQ_GLOBAL(int, 0x1004CE7C) = 0;
        }
        else
        {
            if (EQ_GLOBAL(int, 0x1004CE7C) == 0 ||
                EQ_GLOBAL(unsigned int, 0x1004CE58) >= EQMAIN_IMPORT(unsigned int, 0x10040244, (void))())
            {
                if (EQ_GLOBAL(int, 0x1004CE68) != 0)
                {
                    return EQ_CDECL(int, 0x10028430, (int *, int))((int *)0x1004CE60, EQ_GLOBAL(int, 0x1004CE60));
                }
                return 0;
            }
            EQ_GLOBAL(unsigned int, 0x1004CE58) = EQMAIN_IMPORT(unsigned int, 0x10040244, (void))() + EQ_GLOBAL(unsigned int, 0x100463A4);
            key.value = EQ_GLOBAL(unsigned short, 0x1004CE5C);
        }
        if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4) != 0)
        {
            if ((key.bytes.high & 0x80) == 0)
            {
                if (EQ_GLOBAL(int, 0x1004CE7C) == 0)
                {
                    EQ_GLOBAL(int, 0x1004CE7C) = 1;
                    EQ_GLOBAL(unsigned short, 0x1004CE5C) = key.value;
                    EQ_GLOBAL(unsigned int, 0x1004CE58) = EQMAIN_IMPORT(unsigned int, 0x10040244, (void))() + EQ_GLOBAL(unsigned int, 0x100463A0);
                }
                switch (key.bytes.low)
                {
                case 1:
                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, 0x1b, 0, 0);
                    break;
                case 0xcb:  // left
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor > 0)
                    {
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor--;
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    break;
                case 0xcd:  // right
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor < (int)strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text))
                    {
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor++;
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    break;
                case 199:   // home
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor > 0)
                    {
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor = 0;
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    break;
                case 0xcf:  // end
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor < (int)strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text))
                    {
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor = strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text);
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    break;
                case 0xd3:  // delete
                    len = strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text);
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor < len)
                    {
                        for (i = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor; i < len + 1; i++)
                        {
                            EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i + 1];
                            if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask != 0)
                            {
                                EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i + 1];
                            }
                        }
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    break;
                case 200:   // up: previous range, same column
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->type == 8 && EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->range > 0)
                    {
                        int offset;
                        int width;

                        n = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->range;
                        offset = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor - EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n];
                        width = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n + 1] - EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n];
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->range = n - 1;
                        item = EQ_GLOBAL(TextField_guess *, 0x1004CEC4);
                        if (width > 0)
                        {
                            offset = (int)((float)(item->ranges[item->range + 1] - item->ranges[item->range]) * ((float)offset / width));
                        }
                        item->cursor = item->ranges[item->range] + offset;
                        if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor < 0)
                        {
                            EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor = 0;
                        }
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    else
                    {
                        EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, 0x82, 0, 0);
                    }
                    break;
                case 0xd0:  // down: next range, same column
                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->type == 8 && (n = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->range) < 0x14 &&
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n + 1] != 0)
                    {
                        int width;
                        int offset;
                        int start;
                        int newWidth;

                        offset = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor - EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n];
                        width = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n + 1] - EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->ranges[n];
                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->range = n + 1;
                        item = EQ_GLOBAL(TextField_guess *, 0x1004CEC4);
                        start = item->ranges[item->range];
                        newWidth = item->ranges[item->range + 1] - start;
                        if (newWidth > 0)
                        {
                            offset = (int)((float)offset / width * newWidth);
                        }
                        item->cursor = start + offset;
                        if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor > (int)strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text))
                        {
                            EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor = strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text);
                        }
                        EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                    }
                    else
                    {
                        EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, 0x83, 0, 0);
                    }
                    break;
                default:
                    c = EQ_METHOD(1, char, 0x100282E0, (void *, unsigned char *), (void *)0x100555E0)(&key.bytes.low);
                    if (c != 0)
                    {
                        switch (c)
                        {
                        case 8:     // backspace
                            if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor > 0)
                            {
                                len = strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text);
                                for (i = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor - 1; i < len + 1; i++)
                                {
                                    EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i + 1];
                                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask != 0)
                                    {
                                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i + 1];
                                    }
                                }
                                EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor--;
                                EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                            }
                            break;
                        case 9:     // tab, enter: the next field that takes text
                        case 0xd:
                            EQ_GLOBAL(int, 0x1004CE7C) = 0;
                            for (node = ((int *)EQ_GLOBAL(int *, 0x1004CE94)[1])[7];
                                 node != 0 && EQ_GLOBAL(TextField_guess *, 0x1004CEC4) != *(TextField_guess **)(node + 4);
                                 node = *(int *)(node + 8))
                            {
                            }
                            EQ_GLOBAL(TextField_guess *, 0x1004CEC4) = 0;
                            for (; node != 0; node = *(int *)(node + 8))
                            {
                                item = *(TextField_guess **)(node + 4);
                                if (item->type == 1 || item->type == 8)
                                {
                                    item->blink = 1;
                                    item->stamp = EQMAIN_IMPORT(unsigned int, 0x10040244, (void))();
                                    EQ_GLOBAL(TextField_guess *, 0x1004CEC4) = item;
                                    EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(item);
                                    break;
                                }
                            }
                            if ((key.bytes.high & 0x0c) == 0)
                            {
                                if (c == 9)
                                {
                                    EQ_CDECL(void, 0x100044B0, (int, TextField_guess *, int, int))(7, EQ_GLOBAL(TextField_guess *, 0x1004CEC4), 0, 0);
                                    EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, 9, 0, 0);
                                }
                                else
                                {
                                    EQ_CDECL(void, 0x100044B0, (int, TextField_guess *, int, int))(0, EQ_GLOBAL(TextField_guess *, 0x1004CEC4), 0, 0);
                                }
                            }
                            else
                            {
                                EQ_CDECL(void, 0x100044B0, (int, TextField_guess *, int, int))(6, EQ_GLOBAL(TextField_guess *, 0x1004CEC4), 0, 0);
                            }
                            break;
                        default:    // printable: insert at the cursor
                            if (EQ_CDECL(int, 0x100338B7, (int))(c) != 0)
                            {
                                len = strlen(EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text);
                                if (len < EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->capacity - 2)
                                {
                                    for (i = len + 1; i > EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor; i--)
                                    {
                                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[i - 1];
                                        if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask != 0)
                                        {
                                            EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i] = EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[i - 1];
                                        }
                                    }
                                    EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->text[EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor] = c;
                                    if (EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask != 0)
                                    {
                                        EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->mask[EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor] = '*';
                                    }
                                    EQ_GLOBAL(TextField_guess *, 0x1004CEC4)->cursor++;
                                    EQ_CDECL(void, 0x10004EC0, (TextField_guess *))(EQ_GLOBAL(TextField_guess *, 0x1004CEC4));
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        else if ((key.bytes.high & 0x80) == 0)
        {
            switch (key.bytes.low)
            {
            case 1:
                c = 0x1b;
                break;
            case 0xcb:
                c = 0x80;
                break;
            case 0xcd:
                c = 0x81;
                break;
            case 200:
                c = 0x82;
                break;
            case 0xd0:
                c = 0x83;
                break;
            case 199:
                c = 0x84;
                break;
            case 0xcf:
                c = 0x85;
                break;
            case 0xd3:
                c = 0x7f;
                break;
            case 0xc9:
                c = 0x86;
                break;
            case 0xd1:
                c = 0x87;
                break;
            default:
                c = EQ_METHOD(1, char, 0x100282E0, (void *, unsigned char *), (void *)0x100555E0)(&key.bytes.low);
                break;
            }
            if (c != 0)
            {
                EQ_CDECL(void, 0x100044B0, (int, int, int, int))(4, c, 0, 0);
            }
        }
    }
}

// FUNCTION: EQMAIN 0x100044b0
// Adds a four-value record to the front-end work queue.
// __cdecl, 4 stack args, the callee pops none. Allocates a 0x14-byte record (_malloc at 0x1003342E) and stores
// args 1, 2, 3 and 4 at dwords 0, 2, 3 and 4 (dword 1 is left as malloc gave it). When the allocation succeeded,
// hands the record to list3_insert_guess as (&DAT_1004CE60, 0, record). Returns nothing.
void __cdecl EnqueueQuad_guess(int value, int payload, int value2, int value3)
{
    int *record;

    record = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x14);
    if (record != 0)
    {
        record[0] = value;
        record[2] = payload;
        record[3] = value2;
        record[4] = value3;
        EQ_CDECL(void, 0x10028340, (int *, int, int *))((int *)0x1004CE60, 0, record);
    }
}

// FUNCTION: EQMAIN 0x10004d40
// Repeats a held slider arrow to change its selected value.
// @early-stop: interval threshold, branch layout, and later register/call scheduling still differ.
// __stdcall, no args (the callee pops none). Auto-repeat tick for a held scroll arrow. Once timeGetTime is past
// g_repeatStart, the next repeat is due DAT_100463B4 later while timeGetTime <= g_repeatNext, else DAT_100463B0 later.
// The point (g_cursor, DAT_1004CE54) over the object's rect at +0x44 steps its value at +0x68 up while it is below
// +0x64 - 1; otherwise, over the rect at +0x34, it steps the value down while it is at least 1. A step redraws the
// object (RefreshSlider_guess) and queues EnqueueQuad_guess(1, obj, x, y). Returns nothing.
void __stdcall ScrollArrowRepeat_guess(void)
{
    unsigned int DVar1;
    int repeatDelay;
    int BVar2;
    int obj;
    int (__stdcall *pfnPtInRect)(int, int, int);

    DVar1 = EQMAIN_IMPORT(unsigned int, 0x10040244, (void))();
    obj = EQ_GLOBAL(int, 0x1004CE80);
    if (obj == 0)
        return;
    if (EQ_GLOBAL(unsigned int, 0x1004CE4C) >= DVar1)
        return;
    repeatDelay = EQ_GLOBAL(int, 0x100463B0);
    if (DVar1 <= EQ_GLOBAL(unsigned int, 0x1004CE48))
        repeatDelay = EQ_GLOBAL(int, 0x100463B4);
    pfnPtInRect = EQMAIN_IMPORT(int, 0x100401DC, (int, int, int));
    EQ_GLOBAL(unsigned int, 0x1004CE4C) = repeatDelay + DVar1;
    BVar2 = pfnPtInRect(obj + 0x44, EQ_GLOBAL(int, 0x1004CE50),
                        EQ_GLOBAL(int, 0x1004CE54));
    if (BVar2 != 0 && *(int *)(EQ_GLOBAL(int, 0x1004CE80) + 0x68) < *(int *)(EQ_GLOBAL(int, 0x1004CE80) + 0x64) - 1)
    {
        *(int *)(EQ_GLOBAL(int, 0x1004CE80) + 0x68) = *(int *)(EQ_GLOBAL(int, 0x1004CE80) + 0x68) + 1;
    }
    else
    {
        BVar2 = pfnPtInRect(EQ_GLOBAL(int, 0x1004CE80) + 0x34, EQ_GLOBAL(int, 0x1004CE50),
                            EQ_GLOBAL(int, 0x1004CE54));
        if (BVar2 == 0)
            return;
        obj = EQ_GLOBAL(int, 0x1004CE80);
        if (*(int *)(obj + 0x68) <= 0)
            return;
        *(int *)(obj + 0x68) = *(int *)(obj + 0x68) - 1;
    }
    EQ_CDECL(void, 0x100067E0, (int))(EQ_GLOBAL(int, 0x1004CE80));
    EQ_CDECL(void, 0x100044B0, (int, int, int, int))(1, EQ_GLOBAL(int, 0x1004CE80),
                                                         EQ_GLOBAL(int, 0x1004CE50), EQ_GLOBAL(int, 0x1004CE54));
}

// FUNCTION: EQMAIN 0x10004e10
// Hands each control in the object's +0x1C list to 0x10004E40 (next read first); screens call it on exit.
// __cdecl, 1 stack arg (the list owner), the callee pops none. A null owner does nothing. Otherwise walks the singly
// linked list at the owner's +0x1c: each node's +4 is a record handed to DispatchQueueRecord_guess,
// +8 is the next node. Returns nothing.
void __cdecl DispatchQueueList_guess(int listOwner)
{
    int *control;
    int listNode;

    if (listOwner != 0)
    {
        listNode = *(int *)(listOwner + 0x1c);
        while (listNode != 0)
        {
            control = (int *)*(int *)(listNode + 4);
            listNode = *(int *)(listNode + 8);
            EQ_CDECL(void, 0x10004E40, (int *))(control);
        }
    }
}

// FUNCTION: EQMAIN 0x10004e40
// Hands one UI control to the teardown routine for its type (dword 0); 4, 7 and others are skipped.
// __cdecl, 1 stack arg (the record), the callee pops none. A null record does nothing. Otherwise switches on the record's
// type (dword 0): types 0, 5 and 6 pass it to eqmain_object_destroy_guess, eqmainFreeUnit_guess and UnlinkAndFreeRecord_guess and return; types 1, 8 and 9 pass
// it to EQMain_FreeEntry_guess and return; type 2 passes it to DestroyNamedNode_guess and returns; type 3 passes it to ReleaseQueueUnit_guess and falls
// out. Types 4, 7, anything above 9 and negative types do nothing. Returns nothing.
void __cdecl DispatchQueueRecord_guess(int *record)
{
    if (record != 0)
    {
        switch (*record)
        {
        case 0:
            EQ_CDECL(void, 0x10001700, (int *))(record);
            return;
        case 5:
            EQ_CDECL(void, 0x100019E0, (int *))(record);
            return;
        case 6:
            EQ_CDECL(void, 0x10006130, (int *))(record);
            return;
        case 1:
        case 8:
        case 9:
            EQ_CDECL(void, 0x10008A00, (int *))(record);
            return;
        case 2:
            EQ_CDECL(void, 0x10005930, (int *))(record);
            return;
        case 3:
            EQ_CDECL(void, 0x10006AE0, (int *))(record);
            break;
        }
    }
    return;
}

// FUNCTION: EQMAIN 0x10004ec0
// Redraws one UI control by its type: state image, one-line or word-wrapped text field, list, slider, rollout
// @early-stop: stack slot order, the unrotated text-fit retry loop and case 6's reloads of dword 8 still differ.
// __cdecl, 1 stack arg (the record), the callee pops none. Returns nothing (EAX is whatever the last call or load left).
// A null record does nothing. Dispatches on the record's type (dword 0): types 0 and 5 blit a surface through the dword 2
// object (IDirectDrawSurface::Blt, vtable slot 5) when the selected slot is set. Types 1, 8 and 9 draw text on a GetDC DC
// (slot 17) of the dword 12 object, then blit it through the dword 2 object. Type 2 calls DrawNamedRows_guess, type 3
// RefreshSlider_guess. Type 6 BltFasts (slot 7) when dword 7 is 0 and dword 8 is set, then calls PopupListDrawOrChoose_guess. Types 4, 7,
// anything above 9 and negative types do nothing.
void __cdecl DrawQueueRecord_guess(int *record)
{
    char *text;
    int *obj;
    void *surface;
    void *hdc;
    int isWordChar;
    int overflow;
    int lineStart;
    int n;
    int c;
    int size[2];
    int rect[4];
    int clip[4];
    int fx[25]; // DDBLTFX

    if (record == 0)
    {
        return;
    }
    switch (*record)
    {
    case 0:
        obj = record + record[7] + 8;
        if (obj[0] != 0)
        {
            rect[0] = 0;
            rect[1] = 0;
            rect[2] = ((int *)obj[0])[0];
            rect[3] = ((int *)obj[0])[1];
            fx[0] = 0x64;
            fx[2] = 0xcc0020;
            surface = (void *)((int *)record[2])[3];
            EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
                surface, record + 3, (void *)((int *)obj[0])[3], rect, 0x1028000, fx);
            return;
        }
        break;
    case 5:
        if (record[7] != 0)
        {
            record[7] = 1;
        }
        obj = record + record[7] + 9;
        if (obj[0] != 0)
        {
            rect[0] = 0;
            rect[1] = 0;
            rect[2] = ((int *)obj[0])[0];
            rect[3] = ((int *)obj[0])[1];
            clip[0] = record[3];
            clip[1] = record[4];
            clip[2] = record[5] - record[8];
            clip[3] = record[6];
            fx[0] = 0x64;
            fx[2] = 0xcc0020;
            surface = (void *)((int *)record[2])[3];
            EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
                surface, clip, (void *)((int *)obj[0])[3], rect, 0x1020000, fx);
            return;
        }
        break;
    case 6:
        if (record[7] == 0 && record[8] != 0)
        {
            rect[0] = 0;
            rect[1] = 0;
            rect[2] = ((int *)record[8])[0];
            rect[3] = ((int *)record[8])[1];
            surface = (void *)((int *)record[2])[3];
            EQMAIN_VCALL(int, surface, 0x1c, (void *, int, int, void *, int *, int))(
                surface, record[3], record[4], (void *)((int *)record[8])[3], rect, 0x11);
        }
        EQ_CDECL(void, 0x100064B0, (int *))(record);
        return;
    case 8:
    case 9:
    {
        char *ptr;
        int nLine;
        bool wrapLast;
        int caret[4];

        obj = (int *)record[10];
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = obj[0];
        rect[3] = obj[1];
        fx[0] = 0x64;
        fx[2] = 0xcc0020;
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
            surface, rect, (void *)obj[3], rect, 0x1020000, fx);
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x44, (void *, void **))(surface, &hdc);
        record[16] = (int)EQMAIN_IMPORT(void *, 0x10040028, (void *, void *))(hdc, (void *)record[16]);
        EQMAIN_IMPORT(int, 0x1004003c, (void *, int))(hdc, record[18]);
        EQMAIN_IMPORT(int, 0x10040014, (void *, int))(hdc, 1);
        obj = (int *)record[12];
        nLine = 0;
        clip[0] = 0;
        clip[1] = 0;
        clip[2] = obj[0] - 2;
        clip[3] = 0xe;
        caret[0] = obj[0] - 2;
        caret[2] = obj[0];
        caret[1] = obj[1] - 0xe;
        caret[3] = obj[1];
        record[20] = 0;
        lineStart = 0;
        text = (char *)record[8];
        c = strlen(text);
        while (c != 0)
        {
            if (nLine > 0x15 || clip[3] > ((int *)record[12])[1])
            {
                break;
            }
            if (c > 300)
            {
                c = 300;
            }
            wrapLast = nLine == 0x14 || clip[3] + 0xe > ((int *)record[12])[1];
            for (;;)
            {
                EQMAIN_IMPORT(int, 0x10040038, (void *, char *, int, int *))(hdc, text + lineStart, c, size);
                overflow = size[0] - clip[2] + clip[0] + 2;
                if (overflow < 0)
                {
                    break;
                }
                if (overflow > clip[2])
                {
                    c -= c / 4;
                }
                else if (overflow > 100)
                {
                    c -= 8;
                }
                else
                {
                    c--;
                }
            }
            if (overflow > -0x32 && c > 10 && !wrapLast)
            {
                ptr = text + lineStart + c - 1;
                for (overflow = 1; overflow < 10; overflow++)
                {
                    isWordChar = EQ_CDECL(int, 0x10033889, (int))(*ptr);
                    if (isWordChar == 0)
                    {
                        c += 1 - overflow;
                        break;
                    }
                    ptr--;
                }
            }
            EQMAIN_IMPORT(int, 0x10040024, (void *, int, int, int, int *, char *, int, int *))(
                hdc, clip[0] + 2, clip[1], 4, clip, text + lineStart, c, 0);
            record[nLine + 0x15] = lineStart;
            if (EQ_GLOBAL(int *, 0x1004CEC4) == record && lineStart <= record[7] && c + lineStart >= record[7])
            {
                record[20] = nLine;
                if (record[13] != 0)
                {
                    EQMAIN_IMPORT(int, 0x10040038, (void *, char *, int, int *))(
                        hdc, text + lineStart, record[7] - lineStart, size);
                    caret[0] = size[0] + clip[0] + 2;
                    caret[1] = clip[1];
                    caret[2] = caret[0] + 2;
                    caret[3] = clip[3];
                }
            }
            nLine++;
            lineStart += c;
            c = strlen(text + lineStart);
            clip[1] += 0xe;
            clip[3] += 0xe;
        }
        record[nLine + 0x15] = 0;
        if (c != 0)
        {
            text[lineStart] = 0;
            if (record[7] > (int)strlen(text))
            {
                record[7] = (int)strlen(text);
            }
        }
        record[16] = (int)EQMAIN_IMPORT(void *, 0x10040028, (void *, void *))(hdc, (void *)record[16]);
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x68, (void *, void *))(surface, hdc);
        if (record[13] != 0)
        {
            if (record[7] == 0)
            {
                caret[0] = clip[0] + 2;
                caret[1] = 0;
                caret[2] = caret[0] + 2;
                caret[3] = 0xe;
            }
            fx[0] = 0x64;
            fx[2] = 0xff0062;
            surface = (void *)((int *)record[12])[3];
            EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
                surface, caret, surface, caret, 0x1020000, fx);
        }
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = ((int *)record[12])[0];
        rect[3] = ((int *)record[12])[1];
        fx[0] = 0x64;
        fx[2] = 0xcc0020;
        surface = (void *)((int *)record[2])[3];
        EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
            surface, record + 3, (void *)((int *)record[12])[3], rect, 0x1020000, fx);
        return;
    }
    case 1:
    {
        int x;
        int font[15]; // LOGFONTA

        obj = (int *)record[10];
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = obj[0];
        rect[3] = obj[1];
        fx[0] = 0x64;
        fx[2] = 0xcc0020;
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
            surface, rect, (void *)obj[3], rect, 0x1020000, fx);
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x44, (void *, void **))(surface, &hdc);
        record[16] = (int)EQMAIN_IMPORT(void *, 0x10040028, (void *, void *))(hdc, (void *)record[16]);
        if (record[15] != 0)
        {
            text = (char *)record[15];
        }
        else
        {
            text = (char *)record[8];
        }
        EQMAIN_IMPORT(int, 0x10040038, (void *, char *, int, int *))(hdc, text, record[7], size);
        EQMAIN_IMPORT(int, 0x1004003c, (void *, int))(hdc, record[18]);
        EQMAIN_IMPORT(int, 0x10040014, (void *, int))(hdc, 1);
        obj = (int *)record[12];
        clip[0] = 0;
        clip[1] = 0;
        clip[2] = obj[0];
        clip[3] = obj[1];
        n = 0;
        while (size[0] > clip[2] - 10)
        {
            if (size[0] - clip[2] + 10 > 100)
            {
                n += 10;
            }
            else
            {
                n++;
            }
            EQMAIN_IMPORT(int, 0x10040038, (void *, char *, int, int *))(hdc, text + n, record[7] - n, size);
        }
        if ((record[19] & 1) != 0)
        {
            x = ((int *)record[12])[0] / 2 - size[0] / 2 + clip[0];
            EQMAIN_IMPORT(int, 0x10040024, (void *, int, int, int, int *, char *, int, int *))(
                hdc, x, 0, 4, clip, text + n, strlen(text + n), 0);
        }
        else
        {
            EQMAIN_IMPORT(int, 0x10040024, (void *, int, int, int, int *, char *, int, int *))(
                hdc, 2, 0, 4, clip, text + n, strlen(text + n), 0);
        }
        record[16] = (int)EQMAIN_IMPORT(void *, 0x10040028, (void *, void *))(hdc, (void *)record[16]);
        surface = (void *)((int *)record[12])[3];
        EQMAIN_VCALL(int, surface, 0x68, (void *, void *))(surface, hdc);
        if (record[13] != 0)
        {
            if ((record[19] & 1) != 0)
            {
                clip[0] = x + size[0];
                clip[2] = clip[0] + 2;
            }
            else
            {
                clip[0] = size[0] + 2;
                clip[2] = size[0] + 4;
            }
            clip[1] = 0;
            if (EQ_METHOD(2, int, 0x10027D20, (void *, int, int *), EQ_GLOBAL(void *, 0x1004CEC0))(record[16], font) != 0)
            {
                clip[3] = font[0];
            }
            else
            {
                clip[3] = ((int *)record[12])[1] - (((int *)record[12])[1] >> 3);
            }
            fx[0] = 0x64;
            fx[2] = 0xff0062;
            surface = (void *)((int *)record[12])[3];
            EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
                surface, clip, surface, clip, 0x1020000, fx);
        }
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = ((int *)record[12])[0];
        rect[3] = ((int *)record[12])[1];
        fx[0] = 0x64;
        fx[2] = 0xcc0020;
        surface = (void *)((int *)record[2])[3];
        EQMAIN_VCALL(int, surface, 0x14, (void *, int *, void *, int *, int, int *))(
            surface, record + 3, (void *)((int *)record[12])[3], rect, 0x1020000, fx);
        return;
    }
    case 2:
        EQ_CDECL(void, 0x10005A00, (int *))(record);
        return;
    case 3:
        EQ_CDECL(void, 0x100067E0, (int *))(record);
        break;
    }
}

// FUNCTION: EQMAIN 0x100056C0
// Draws a named front-end image centered at a supplied UI position.
// __cdecl, 4 stack args, the callee pops none. Looks up the record named name with PackFsGetNamedImage_guess (cdecl, args
// (name, 0x10)). When that returns 0, reports it through FatalErrorExit_guess (cdecl, args (DAT_10055AE0, name)) and
// returns. Otherwise draws the record with BltFastSrcRect_guess at x = (centerX - width / 2) + 0x140 (width = dword 0), y =
// y, source (0, 0), size (width, height = dword 1) and flag 1, then frees it with PackFsFreeEntry_guess. Returns nothing.
void __cdecl DrawNamedImageCentered_guess(int dest, char *name, int centerX, int y)
{
    int *imageRecord;

    imageRecord = EQ_CDECL(int *, 0x10007AE0, (char *, int))(name, 0x10);
    if (imageRecord == (int *)0x0)
    {
        EQ_CDECL(int, 0x100278A0, (int, char *))(EQ_GLOBAL(int, 0x10055AE0), name);
        return;
    }
    EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int *, int, int, int, int, int))(
        dest, (centerX - *imageRecord / 2) + 0x140, y, imageRecord, 0, 0, *imageRecord, imageRecord[1], 1);
    EQ_CDECL(void, 0x10007A80, (int *))(imageRecord);
    return;
}

// FUNCTION: EQMAIN 0x10005730
// Polls keyboard input after servicing the front-end message loop.
// @early-stop: Translate/Dispatch cache registers remain swapped (equal use counts; declaration order does not move them); the
// fastcall shim clears EDX because DIKeyboard_ReadOneEvent_guess takes one stack argument, which EQ_THISCALL can only pass after its EDX dummy.
// __stdcall, no args, returns bool (AL; callers read AL only, so the rewrite's false path does not keep the original's
// EAX bits 8-31). Stamps timeGetTime, calls the 0x2C slot of each object in the list at g_packfs_list_head_guess (args (obj, 0, 1);
// a node's +4 record holds the object at +0x10), then pumps the messages as PumpMessagesAndEditText_guess does. Then
// DIKeyboard_ReadOneEvent_guess (this = 0x100555E0) reads a key word into frame.key.value: with no event, the timer (DAT_1004CE7C set and the
// stamp above DAT_1004CE58) reloads DAT_1004CE58 = DAT_100463A4 + stamp and takes DAT_1004CE5C as the word, else it
// returns false. An event clears DAT_1004CE7C. A word with 0x8000 (key up) clear returns true; set, it pumps again.
bool __stdcall PumpMessagesAndPollKey_guess(void)
{
    struct MSG_guess    // the Win32 MSG, 0x1C bytes
    {
        int hwnd;               // 0x00
        unsigned int message;   // 0x04
        unsigned int wParam;    // 0x08
        int lParam;             // 0x0C
        unsigned int time;      // 0x10
        int pt[2];              // 0x14
    };
    union KeyWord_guess
    {
        unsigned short value;
        struct
        {
            unsigned char low;
            unsigned char high;
        } bytes;
    };
    struct PollFrame_guess
    {
        unsigned short keyPrefix;
        KeyWord_guess key;
        unsigned int stamp;
        MSG_guess message;
    } frame; // fake match: keep the output word at ESP+0x12 and the original local offsets.
    typedef int (__stdcall *PeekMessageFn_guess)(MSG_guess *, void *, unsigned int, unsigned int, unsigned int);
    typedef int (__stdcall *GetMessageFn_guess)(MSG_guess *, void *, unsigned int, unsigned int);
    typedef int (__stdcall *TranslateMessageFn_guess)(const MSG_guess *);
    typedef long (__stdcall *DispatchMessageFn_guess)(const MSG_guess *);
    PeekMessageFn_guess peekMessage;
    GetMessageFn_guess getMessage;
    TranslateMessageFn_guess translateMessage;
    DispatchMessageFn_guess dispatchMessage;
    int tmp;
    int *obj;
    int BVar3;
    unsigned int readResult;

    unsigned int now = EQMAIN_IMPORT(unsigned int, 0x10040244, (void))();
    tmp = EQ_GLOBAL(int, 0x1004CE90);
    frame.stamp = now;
    for (; tmp != 0; tmp = *(int *)(tmp + 8))
    {
        obj = *(int **)(*(int *)(tmp + 4) + 0x10);
        if (obj != (int *)0x0)
        {
            EQMAIN_VCALL(void, obj, 0x2C, (int *, int, int))(obj, 0, 1);
        }
    }
    peekMessage = (PeekMessageFn_guess)EQ_GLOBAL(void *, 0x100401D0);
    getMessage = (GetMessageFn_guess)EQ_GLOBAL(void *, 0x100401C4);
    translateMessage = (TranslateMessageFn_guess)EQ_GLOBAL(void *, 0x100401CC);
    dispatchMessage = (DispatchMessageFn_guess)EQ_GLOBAL(void *, 0x10040230);
    do
    {
        tmp = peekMessage(&frame.message, 0, 0, 0, 0);
        while (tmp != 0)
        {
            BVar3 = getMessage(&frame.message, 0, 0, 0);
            if (0 < BVar3)
            {
                translateMessage(&frame.message);
                dispatchMessage(&frame.message);
            }
            tmp = peekMessage(&frame.message, 0, 0, 0, 0);
        }
        readResult = EQ_METHOD(1, unsigned int, 0x100280E0, (void *, unsigned char *), (void *)0x100555E0)((unsigned char *)&frame.key.value);
        if ((char)readResult != 0)
        {
            EQ_GLOBAL(int, 0x1004CE7C) = 0;
        }
        else
        {
            if (EQ_GLOBAL(int, 0x1004CE7C) == 0 ||
                EQ_GLOBAL(unsigned int, 0x1004CE58) >= frame.stamp)
            {
                return false;
            }
            EQ_GLOBAL(unsigned int, 0x1004CE58) = EQ_GLOBAL(unsigned int, 0x100463A4) + frame.stamp;
            frame.key.value = EQ_GLOBAL(unsigned short, 0x1004CE5C);
        }
        if ((frame.key.bytes.high & 0x80) == 0)
        {
            return true;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x10005840
// Creates a front-end list panel with row spacing and highlight art.
// @early-stop: volatile register choices differ around fixed-address indirect calls and the initial field setup.
// __cdecl, 7 stack args, the callee pops none. Allocates a 0x48-byte node with _malloc (0x1003342E) and returns NULL
// when that fails. Otherwise fills it in: +0x00 = 2, +0x08 = parent (parent), +0x0C/+0x10 = sourceX/sourceY, +0x14 =
// sourceX + width, +0x18 = height + sourceY, +0x1C cleared by ClearRecord3_guess, +0x28 = flags, +0x2C = 0, +0x30 =
// (+0x18 - +0x10) / (flags & 1 ? 11 : 13), +0x34 = -1, +0x38 = 0, +0x3C = value, +0x40 = CreateSurfaceRecord_guess (width,
// height, 0x10) drawn by BltFastSrcRect_guess (g_eqmain_ceb8_guess as its 4th arg), +0x44 = PackFsGetNamedImage_guess ("...", 0x10).
// Then list3_insert_guess (cdecl, args (parent + 0x1C, 0, node)) links it into the parent's list, and the node is returned.
int *CreateNamedNode_guess(int parent, int sourceX, int sourceY, int width, int height, unsigned int flags,
                           int value)
{
    int *node;
    int *image;

    node = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x48);
    if (node != (int *)0x0)
    {
        *node = 2;
        node[3] = sourceX;
        node[4] = sourceY;
        node[5] = sourceX + width;
        node[6] = height + sourceY;
        node[2] = parent;
        EQ_CDECL(void, 0x10028330, (int *))(node + 7);
        node[10] = flags;
        node[11] = 0;
        node[12] = (node[6] - node[4]) / ((flags & 1) ? 11 : 13);
        node[13] = -1;
        node[14] = 0;
        node[15] = value;
        image = EQ_CDECL(int *, 0x10006D80, (int, int, int))(width, height, 0x10);
        node[16] = (int)image;
        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, int))(
            (int)image, 0, 0, EQ_GLOBAL(int, 0x1004CEB8), sourceX, sourceY, width, height, 0);
        image = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x100463CC, 0x10);
        node[17] = (int)image;
        EQ_CDECL(void, 0x10028340, (int *, int, int))((int *)(parent + 0x1c), 0, (int)node);
    }
    return node;
}

// FUNCTION: EQMAIN 0x10005930
// Removes a front-end list panel and frees its row and selection data.
// @early-stop: the +0x44 cleanup argument loads into EDX instead of ECX; fixed-address call lowering changes register allocation.
// __cdecl, 1 stack argument, the callee pops none. Frees the node param_1: PackFsFreeEntry_guess (cdecl) on its pointers at
// +0x40 and +0x44, then each record on its list at +0x1c. A record's child list at +0x14 is drained first: each entry
// is unlinked with list3_unlink_free_guess (cdecl, args (list, entry)), which returns the entry's object, and that object's
// string at +4 and the object are _free'd. Then the record's string at +4 and the record are _free'd. Next, when the
// parent at +8 has a list at +0x1c that holds the node, the node's entry is unlinked from it (list3_unlink_free_guess). The node
// is _free'd last. Returns nothing.
void __cdecl DestroyNamedNode_guess(void *node)
{
    void *record;
    void *child;
    int *listNode;

    EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)node + 0x40));
    EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)node + 0x44));
    listNode = *(int **)((int)node + 0x1c);
    while (listNode != (int *)0x0)
    {
        record = (void *)EQ_CDECL(int, 0x10028430, (int *, int *))((int *)((int)node + 0x1c), listNode);
        listNode = *(int **)((int)record + 0x14);
        while (listNode != (int *)0x0)
        {
            child = (void *)EQ_CDECL(int, 0x10028430, (int *, int *))((int *)((int)record + 0x14), listNode);
            if (*(void **)((int)child + 4) != (void *)0x0)
            {
                EQ_CDECL(void, 0x10033568, (void *))(*(void **)((int)child + 4));
            }
            EQ_CDECL(void, 0x10033568, (void *))(child);
            listNode = *(int **)((int)record + 0x14);
        }
        if (*(void **)((int)record + 4) != (void *)0x0)
        {
            EQ_CDECL(void, 0x10033568, (void *))(*(void **)((int)record + 4));
        }
        EQ_CDECL(void, 0x10033568, (void *))(record);
        listNode = *(int **)((int)node + 0x1c);
    }
    listNode = *(int **)(*(int *)((int)node + 8) + 0x1c);
    while (listNode != (int *)0x0)
    {
        if (node == (void *)listNode[1])
        {
            EQ_CDECL(int, 0x10028430, (int *, int *))((int *)(*(int *)((int)node + 8) + 0x1c), listNode);
            break;
        }
        listNode = (int *)listNode[2];
    }
    EQ_CDECL(void, 0x10033568, (void *))(node);
    return;
}

// The RECT that ExtTextOutA clips to: left, top, right, bottom (windows.h isn't included in this file).
struct RECT_guess
{
    int left;
    int top;
    int right;
    int bottom;
};

// FUNCTION: EQMAIN 0x10005A00
// Draws a selectable list of text rows, highlighting the selected row and its colored text columns.
// __cdecl, 1 stack argument (the node), the callee pops none. Paints the node's rows into its rect: the background
// sprite (+0x40) across it, then the list from the scroll index (+0x2c), one row per 13 pixels (11 when flags bit 0
// is set), for as long as a row's bottom (its top + the pitch) stays within the rect's bottom (+0x18). The row at the
// selected index (+0x34) gets the highlight sprite (+0x44) across the rect first. A row is its item's text, then each
// child's text, drawn at the running x in the colour at +0x10. The HDC comes from the parent's object (+8, then +0xc)
// through its vtable: slot 0x44 writes the HDC (VC6 puts hdc in the dead argument's slot), slot 0x68 gives it back.
void __cdecl DrawNamedRows_guess(int listObject)
{
    struct NamedRowsNode_guess
    {
        int unused_00[2];
        int parent;       // 0x08
        int left;         // 0x0c
        int top;          // 0x10
        int right;        // 0x14
        int bottom;       // 0x18
        int first_item;   // 0x1c
        int unused_20[2];
        unsigned int flags; // 0x28
        int scroll_index; // 0x2c
        int unused_30;
        int selected_index; // 0x34
        int unused_38[2];
        int background;   // 0x40
        int highlight;    // 0x44
    };
    struct NamedRowsText_guess;
    struct NamedRowsLink_guess
    {
        int unused_00;
        struct NamedRowsText_guess *record;
        NamedRowsLink_guess *next;
    };
    struct NamedRowsText_guess
    {
        int width;
        char *text;
        int unused_08[2];
        unsigned int color;
        NamedRowsLink_guess *children;
    };
    NamedRowsNode_guess *node = (NamedRowsNode_guess *)listObject;
    int *interface;
    NamedRowsLink_guess *itemLink;
    int value;
    int rowIndex;
    RECT_guess rc;
    int hdc;

    EQ_CDECL(void, 0x10006C20, (int, int, int, int, int, int, int, int, int))(
        EQ_GLOBAL(int, 0x1004CEB8), node->left, node->top, node->background, 0, 0,
        node->right - node->left, node->bottom - node->top, 0);
    rc.left = node->left;
    rc.top = node->top;
    rc.right = rc.left;
    rc.bottom = rc.top + ((node->flags & 1) ? 11 : 13);
    interface = *(int **)(node->parent + 0xc);
    (*(void (__stdcall **)(int *, int *))(*interface + 0x44))(interface, &hdc);
    (*(int (__stdcall **)(int, int))0x10040014)(hdc, 1);
    if ((node->flags & 1) != 0)
    {
        (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CECC));
    }
    else
    {
        (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CEC8));
    }
    rowIndex = 0;
    itemLink = (NamedRowsLink_guess *)node->first_item;
    value = node->scroll_index;
    if (value > 0)
    {
        rowIndex = value;
        do
        {
            itemLink = itemLink->next;
            value--;
        } while (value != 0);
    }
    while (itemLink != 0 && rc.bottom <= node->bottom)
    {
        NamedRowsText_guess *row = itemLink->record;
        if (rowIndex == node->selected_index)
        {
            int *highlight;
            int highlight_x;

            interface = *(int **)(node->parent + 0xc);
            (*(void (__stdcall **)(int *, int))(*interface + 0x68))(interface, hdc);
            highlight = (int *)node->highlight;
            highlight_x = rc.left;
            while (highlight_x + highlight[0] <= node->right)
            {
                EQ_CDECL(void, 0x10006C20, (int, int, int, int, int, int, int, int, int))(
                    EQ_GLOBAL(int, 0x1004CEB8), highlight_x, rc.top, (int)highlight,
                    0, 0, highlight[0], highlight[1], 0);
                highlight = (int *)node->highlight;
                highlight_x += highlight[0];
            }
            interface = *(int **)(node->parent + 0xc);
            (*(void (__stdcall **)(int *, int *))(*interface + 0x44))(interface, &hdc);
            (*(int (__stdcall **)(int, int))0x10040014)(hdc, 1);
            if ((node->flags & 1) != 0)
            {
                (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CECC));
            }
            else
            {
                (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CEC8));
            }
        }
        rc.right += row->width;
        (*(int (__stdcall **)(int, int))0x1004003C)(hdc, row->color);
        (*(int (__stdcall **)(int, int, int, int, RECT_guess *, int, int, int))0x10040024)(
            hdc, rc.left + 2, rc.top, 4, &rc, (int)row->text,
            (int)strlen(row->text), 0);
        rc.left = rc.right;
        for (NamedRowsLink_guess *childLink = row->children;
             childLink != 0; childLink = childLink->next)
        {
            row = childLink->record;
            (*(int (__stdcall **)(int, int))0x1004003C)(hdc, row->color);
            rc.right += row->width;
            (*(int (__stdcall **)(int, int, int, int, RECT_guess *, int, int, int))0x10040024)(
                hdc, rc.left + 2, rc.top, 4, &rc, (int)row->text,
                (int)strlen(row->text), 0);
            rc.left = rc.right;
        }
        rc.left = node->left;
        rc.right = rc.left;
        rc.top = rc.bottom;
        rc.bottom = rc.top + ((node->flags & 1) ? 11 : 13);
        rowIndex++;
        itemLink = itemLink->next;
    }
    if ((node->flags & 1) != 0)
    {
        (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CECC));
    }
    else
    {
        (*(int (__stdcall **)(int, int))0x10040028)(hdc, EQ_GLOBAL(int, 0x1004CEC8));
    }
    interface = *(int **)(node->parent + 0xc);
    (*(void (__stdcall **)(int *, int))(*interface + 0x68))(interface, hdc);
}

// FUNCTION: EQMAIN 0x10005CC0
// Adds a text-labeled record with caller-supplied fields to an object's entry list.
// __cdecl, 5 stack arguments, the callee pops none. Allocates a 0x20-byte record (_malloc at 0x1003342E) and stores
// value at dword 0, a copy of the string text (__strdup at 0x1003396B) at dword 1, value2 at dword 2, 0 at dword
// 3 and value3 at dword 4. ClearRecord3_guess (0x10028330) clears the dword at +0x1c (it is passed record + 0x14).
// When the allocation succeeded, hands the record to list3_insert_guess as (owner + 0x1c, 0, record). Returns the record,
// or 0 when _malloc fails.
int *__cdecl EnqueueNamedRecord_guess(int owner, int value, char *text, int value2,
                                      int value3)
{
    int *record;
    char *textCopy;

    record = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x20);
    if (record != 0)
    {
        *record = value;
        textCopy = EQ_CDECL(char *, 0x1003396B, (char *))(text);
        record[1] = (int)textCopy;
        record[2] = value2;
        record[3] = 0;
        record[4] = value3;
        EQ_CDECL(void, 0x10028330, (int *))(record + 5);
        EQ_CDECL(void, 0x10028340, (int *, int, int *))
            ((int *)(owner + 0x1c), 0, record);
    }
    return record;
}

// FUNCTION: EQMAIN 0x10005D20
// Adds a text-labeled record to a separate list while retaining owner state.
// __cdecl, 5 stack arguments, the callee pops none. Allocates a 0x20-byte record (_malloc at 0x1003342E) and stores
// value at dword 0, a copy of the string text (__strdup at 0x1003396B) at dword 1, dword 0x1c of owner at dword 2,
// 0 at dword 3 and color at dword 4. ClearRecord3_guess (0x10028330) clears the dword at +0x1c (it is passed record +
// 0x14). When the allocation succeeded, hands the record to list3_insert_guess as (owner + 0x14, 0, record). Returns the
// record, or 0 when _malloc fails. unusedValue is never read.
int *__cdecl EnqueueOwnedRecord_guess(int unusedValue, int owner, int value, char *text, int color)
{
    int *record;
    char *textCopy;

    record = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x20);
    if (record != 0)
    {
        *record = value;
        textCopy = EQ_CDECL(char *, 0x1003396B, (char *))(text);
        record[1] = (int)textCopy;
        record[2] = *(int *)(owner + 0x1c);
        record[3] = 0;
        record[4] = color;
        EQ_CDECL(void, 0x10028330, (int *))(record + 5);
        EQ_CDECL(void, 0x10028340, (int *, int, int *))
            ((int *)(owner + 0x14), 0, record);
    }
    return record;
}

// FUNCTION: EQMAIN 0x10005D80
// Clears all records from an object's entry list.
// __cdecl, 1 stack argument, the callee pops none. Walks the list at owner + 0x1C (node +4 is the record, node +8 the
// next node), reading the next node before each call. Calls RemoveOwnedRecord_guess (0x10005DB0) on (owner, record)
// for every node. Returns nothing; an empty list does nothing.
void __cdecl RemoveAllOwnedRecords_guess(int owner)
{
    int *nextNode;
    int *listNode;

    listNode = *(int **)(owner + 0x1c);
    if (listNode != 0)
    {
        do
        {
            nextNode = (int *)listNode[2];
            EQ_CDECL(void, 0x10005DB0, (int, void *))(owner, (void *)listNode[1]);
            listNode = nextNode;
        } while (listNode != 0);
    }
}

// FUNCTION: EQMAIN 0x10005DB0
// Removes one record from a list's rows; steps the scroll index (+0x2C) back if the row was at or above it.
// @early-stop: Owner and loop index use opposite registers; fixed-address calls compile through indirect call setup.
// __cdecl, 2 stack arguments, the callee pops none. Walks the list at owner + 0x1C (its head is the dword there;
// node +4 is the record, node +8 the next node), counting the nodes passed in recordIndex. No node holds record: returns
// after the walk (at once for an empty list). Otherwise list3_unlink_free_guess (__cdecl) runs on (owner + 0x1C, node), _free on the
// record's +4 and then on the record. Last, when the count at +0x2C is above 0 and not below recordIndex, it drops by one.
void __cdecl RemoveOwnedRecord_guess(int *owner, void *record)
{
    int scrollIndex;
    int *listNode;
    int recordIndex = 0;

    listNode = (int *)owner[7];
    while (listNode != 0)
    {
        if (record == (void *)listNode[1])
        {
            EQ_CDECL(int, 0x10028430, (int *, int *))(&owner[7], listNode);
            EQ_CDECL(void, 0x10033568, (void *))(*(void **)((int)record + 4));
            EQ_CDECL(void, 0x10033568, (void *))(record);
            scrollIndex = owner[11];
            if ((0 < scrollIndex) && (scrollIndex >= recordIndex))
                owner[11] = scrollIndex - 1;
            return;
        }
        listNode = (int *)listNode[2];
        recordIndex++;
    }
    return;
}

// FUNCTION: EQMAIN 0x10005E10
// Gets the record at the selected row in a list.
// __cdecl, 1 stack argument, the callee pops none. Returns a value. Walks the list at owner + 0x1C (node +4 is the
// record, node +8 the next node) to the node whose position is the dword at +0x34, and returns its record. 0 when that
// dword is negative, when the list is empty, or when the list ends before the position is reached.
void * __cdecl GetRecordAtIndex_guess(int owner)
{
    int listNode;
    int currentIndex;

    if (0 <= *(int *)(owner + 0x34))
    {
        currentIndex = 0;
        for (listNode = *(int *)(owner + 0x1c); listNode != 0; listNode = *(int *)(listNode + 8))
        {
            if (*(int *)(owner + 0x34) == currentIndex)
                return *(void **)(listNode + 4);
            currentIndex++;
        }
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10005E40
// Returns the selected row index only when that row exists; otherwise returns -1.
// __cdecl, 1 stack argument, the callee pops none. Returns the dword at +0x34 when it is not negative and the list at
// +0x1C has a node at that index (the head is index 0); otherwise -1, the empty list included.
int __cdecl CheckSelectedIndex_guess(int listObject)
{
    int listNode;
    int currentIndex;

    if (0 <= *(int *)(listObject + 0x34))
    {
        currentIndex = 0;
        for (listNode = *(int *)(listObject + 0x1c); listNode != 0; listNode = *(int *)(listNode + 8))
        {
            if (*(int *)(listObject + 0x34) == currentIndex)
                return currentIndex;
            currentIndex = currentIndex + 1;
        }
    }
    return -1;
}

// FUNCTION: EQMAIN 0x10005E70
// Moves the list's scroll position to keep a requested row visible, then redraws the list.
// __cdecl, 2 stack arguments, the callee pops none. Returns nothing. A negative count skips the store. Otherwise the dword
// at +0x2C is set to count when count is below it, or when count is above (+0x2C + the dword at +0x30) - 1. Either
// way DrawNamedRows_guess (__cdecl) runs on listObject last.
void __cdecl SetIndexAndRefresh_guess(char *listObject, int count)
{
    if (!(count < 0))
    {
        int current = *(int *)(listObject + 0x2c);
        if (count < current)
            *(int *)(listObject + 0x2c) = count;
        else if (count > current + *(int *)(listObject + 0x30) - 1)
            *(int *)(listObject + 0x2c) = count;
    }
    EQ_CDECL(void, 0x10005A00, (char *))(listObject);
    return;
}

// FUNCTION: EQMAIN 0x10005EA0
// Counts a list's entries and refreshes the view using that count.
// __cdecl, 1 stack argument, the callee pops none. Returns nothing. Counts the nodes of the list at +0x1C (each node's
// next pointer at +0x08) and passes the object and that count to SetIndexAndRefresh_guess (0x10005E70).
void __cdecl CountListAndSetIndex_guess(int listOwner)
{
    int count;
    int node;

    count = 0;
    for (node = *(int *)(listOwner + 0x1c); node != 0; node = *(int *)(node + 0x8))
    {
        count = count + 1;
    }
    EQ_CDECL(void, 0x10005E70, (int, int))(listOwner, count);
    return;
}

// FUNCTION: EQMAIN 0x10005EC0
// Builds a UI choice list from delimited text and creates its control record.
// __cdecl, 8 stack arguments, the callee pops none. Returns what CreateRolloutRecord_guess (0x10005FD0) returns. operator_new
// (0x100338E5) makes a 0x14-byte object and InitIdxLookup_guess (__thiscall, args g_eqmain_drive_base_guess and count) builds the list in it and
// returns the list; a NULL object gives a NULL list and no call. tokenText is copied (at most 0x3FF chars) into a zeroed 0x400-byte
// buffer and split by strtok: the first token with DAT_100463E0, then count times AddStrCopy_guess (__thiscall, adds the token to
// the list) and the next token with DAT_100463DC. Last, the record is made from the params, the list and 1.
void * __cdecl CreateRolloutFromTokens_guess(int value1, int value2, int value3, unsigned char flag, char *text,
                                             unsigned int count, char *tokenText, unsigned char value8)
{
    void *object;
    void *list;
    char buf[0x400];
    char *token;
    int i;

    object = EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x14);
    if (object != 0)
    {
        list = EQ_METHOD(2, void *, 0x100258A0, (void *, char *, unsigned int), object)((char *)0x1004CE40,
                                                                                         count);
    }
    else
    {
        list = 0;
    }
    memset(buf, 0, 0x400);
    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))(buf, tokenText, 0x3ff);
    token = EQ_CDECL(char *, 0x10033996, (char *, char *))(buf, (char *)0x100463E0);
    for (i = (int)count; i > 0; i--)
    {
        EQ_METHOD(1, int, 0x10025900, (void *, char *), list)(token);
        token = EQ_CDECL(char *, 0x10033996, (char *, char *))(0, (char *)0x100463DC);
    }
    return EQ_CDECL(void *, 0x10005FD0, (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
        value1, value2, value3, flag, text, (int)list, value8, 1);
}

// FUNCTION: EQMAIN 0x10005FD0
// Creates a rollout drop-down (rollout.bmp) placed by flags on the 640x480 screen, preselecting a named entry
// __cdecl, 8 stack arguments, the callee pops none. Returns the new record (0x38 bytes), or NULL when _malloc fails and
// nothing else is done. A null owner passes the string at DAT_10055A84 to FatalErrorExit_guess and returns NULL. Otherwise the
// record is zeroed, then: +0x00 = 6, +0x08 = owner (the owner), +0x2C = the dword at entryTable + 8, +0x30 = entryTable, +0x36 =
// pairedEntries, +0x37 = extraFlag, and the short at +0x28 is FindNameIndex_guess(record, defaultText), or 0 when the string is empty. PackFsGetNamedImage_guess
// loads "..." and "..." (16) into +0x20 and +0x24. The rectangle (+0x0C left, +0x10 top, +0x14 right, +0x18
// bottom) comes from the first bitmap's width (+0x00) and height (+0x04) and the flags in flags: 1 centres x on 0x140,
// 2 centres y on 0xF0, 4 adds 10 to x, 8 puts the right edge at x (0x280 first when x is 0), 0x10 puts the bottom edge at y
// (0x1E0 first when y is 0). Last, list3_insert_guess links the record into the owner's list at +0x1C and DrawQueueRecord_guess runs on it.
void * __cdecl CreateRolloutRecord_guess(int owner, int param_2, int y, unsigned char flags, char *defaultText,
                                         int entryTable, unsigned char pairedEntries, unsigned char extraFlag)
{
    int *rec;
    int field8;
    unsigned int lookup;
    int *bmp;
    int width;
    int height;

    if (owner == 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A84));
        return (void *)0x0;
    }
    rec = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x38);
    if (rec != (int *)0x0)
    {
        memset(rec, 0, 0x38);
        *rec = 6;
        rec[2] = owner;
        rec[7] = 0;
        rec[0xc] = entryTable;
        field8 = *(int *)(entryTable + 0x8);
        *((unsigned char *)rec + 0x36) = pairedEntries;
        rec[0xb] = field8;
        *((unsigned char *)rec + 0x37) = extraFlag;
        *(short *)(rec + 0xd) = 0;
        if (strlen(defaultText) == 0)
        {
            *(short *)(rec + 0xa) = 0;
        }
        else
        {
            lookup = EQ_CDECL(unsigned int, 0x100061F0, (int, char *))((int)rec, defaultText);
            *(short *)(rec + 0xa) = (short)lookup;
        }
        bmp = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x100463F4, 0x10);
        rec[8] = (int)bmp;
        bmp = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x100463E4, 0x10);
        rec[9] = (int)bmp;
        width = *(int *)rec[8];
        height = ((int *)rec[8])[1];
        if ((flags & 1) != 0)
        {
            param_2 = 0x140 - width / 2;
        }
        if ((flags & 2) != 0)
        {
            y = 0xf0 - height / 2;
        }
        if ((flags & 0x10) != 0)
        {
            if (y == 0)
            {
                y = 0x1e0;
            }
            y = y + (-10 - height);
        }
        if ((flags & 4) != 0)
        {
            param_2 = param_2 + 10;
        }
        if ((flags & 8) != 0)
        {
            if (param_2 == 0)
            {
                param_2 = 0x280;
            }
            param_2 = param_2 + (-10 - width);
        }
        rec[4] = y;
        rec[3] = param_2;
        rec[5] = width + param_2;
        rec[6] = height + y;
        EQ_CDECL(void, 0x10028340, (int, int, int))(owner + 0x1c, 0, (int)rec);
        EQ_CDECL(void, 0x10004EC0, (int *))(rec);
    }
    return rec;
}

// FUNCTION: EQMAIN 0x10006130
// Removes a record from its owner's list and frees the record.
// __cdecl, 1 stack argument, the callee pops none. A null argument passes the string at DAT_10055A88 to FatalErrorExit_guess
// and returns. Otherwise PackFsFreeEntry_guess (__cdecl) runs on the pointers at +0x20 and +0x24 when they are set. Then the
// object is looked up in the list at owner + 0x1C (owner = +0x08): list3_unlink_free_guess runs on the first node whose +4 is the
// object, and the search stops there. When the byte at +0x37 is set and the object at +0x30 is set, slot 0 of that
// object is called as a __thiscall with 1 (a deleting destructor, by the usual slot-0 convention). Last, _free runs on
// the object (0x10033568).
void __cdecl UnlinkAndFreeRecord_guess(void *record)
{
    int *node;
    void *entryTable;

    if (record == (void *)0x0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A88));
        return;
    }
    if (*(int **)((int)record + 0x20) != (int *)0x0)
    {
        EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)record + 0x20));
    }
    if (*(int **)((int)record + 0x24) != (int *)0x0)
    {
        EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)record + 0x24));
    }
    for (node = *(int **)(*(int *)((int)record + 0x8) + 0x1c); node != (int *)0x0; node = (int *)node[2])
    {
        if (record == (void *)node[1])
        {
            EQ_CDECL(void, 0x10028430, (int *, int *))
                ((int *)(*(int *)((int)record + 0x8) + 0x1c), node);
            break;
        }
    }
    if (*(char *)((int)record + 0x37) != '\0')
    {
        entryTable = *(void **)((int)record + 0x30);
        if (entryTable != (void *)0x0)
        {
            // the original leaves the vtable in EDX (CALL [EDX]): it is the second, ignored, fastcall argument
            (*(void (__fastcall **)(void *, void *, int))(*(int *)entryTable))(entryTable, *(void **)entryTable, 1);
        }
    }
    EQ_CDECL(void, 0x10033568, (void *))(record);
}

// FUNCTION: EQMAIN 0x100061B0
// Fetches a lookup entry; the flag selects an odd-positioned entry for each index.
// __cdecl, 2 stack arguments, the callee pops none. When the byte at state + 0x36 is set, index becomes 2 * index + 1.
// Then IdxLookup_guess::Get_guess (__thiscall, 1 argument, pops it) runs on the object at state + 0x30 with that value, and its EAX
// is returned unchanged (the original tail-calls it: no stack cleanup after the CALL).
int __cdecl ForwardScaledPlusOne_guess(int state, int index)
{
    int value = index;

    if (*(char *)(state + 0x36) != '\0')
    {
        value = value * 2 + 1;
    }
    return EQ_METHOD(1, int, 0x10025940, (void *, int), *(void **)(state + 0x30))(value);
}

// FUNCTION: EQMAIN 0x100061D0
// Fetches a lookup entry; the flag selects every second table entry.
// __cdecl, 2 stack arguments, the callee pops none. When the byte at state + 0x36 is set, index is doubled.
// Then IdxLookup_guess::Get_guess (__thiscall, 1 argument, pops it) runs on the object at state + 0x30 with index, and its
// EAX is returned unchanged (the original tail-calls it: no stack cleanup after the CALL).
int __cdecl ForwardScaled_guess(int state, int index)
{
    int value = index;

    if (*(char *)(state + 0x36) != '\0')
    {
        value = value * 2;
    }
    return EQ_METHOD(1, int, 0x10025940, (void *, int), *(void **)(state + 0x30))(value);
}

// FUNCTION: EQMAIN 0x100061F0
// Finds a matching text entry and returns its position; 0 also means no match.
// __cdecl, 2 stack arguments, the callee pops none. The index of the first entry whose text equals `name`
// (case-insensitive, __strcmpi at 0x10033730) among the count at +0x2c entries of the list at list, else 0.
// IdxLookup_guess::Get_guess (__thiscall, 1 argument: 0) gives the first entry from the cursor at list + 0x30, and
// ArrayCursor_guess::Next_guess (__fastcall, 0x10025960) each later one, after every mismatch (also after the last).
// A count of 0 skips the loop. When the byte at +0x36 is set, the index is halved. Faithful quirk: a match at
// index 0 and no match both return 0.
unsigned int __cdecl FindNameIndex_guess(int list, char *name)
{
    char *item;
    int cmp;
    unsigned int i;

    item = EQ_METHOD(1, char *, 0x10025940, (void *, int), *(void **)(list + 0x30))(0);
    i = 0;
    while (i < *(unsigned int *)(list + 0x2c))
    {
        cmp = EQ_CDECL(int, 0x10033730, (char *, char *))(item, name);
        if (cmp == 0)
        {
            if (*(char *)(list + 0x36) == '\0')
            {
                return i;
            }
            return i >> 1;
        }
        item = EQ_METHOD(0, char *, 0x10025960, (void *), *(void **)(list + 0x30))();
        i = i + 1;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10006250
// Updates one 16-bit control value and redraws the record.
// __cdecl, 2 stack arguments, the callee pops none. Stores the word value at +0x28 of the object at record, then
// calls DrawQueueRecord_guess (__cdecl, 0x10004ec0) with that object as its only argument.
void __cdecl storeWord28_guess(unsigned int *record, unsigned short value)
{
    *(unsigned short *)(record + 10) = value;
    EQ_CDECL(void, 0x10004EC0, (unsigned int *))(record);
    return;
}

// FUNCTION: EQMAIN 0x10006270
// Finds a named choice, stores its index as a control value, and redraws the record.
// __cdecl, 2 stack arguments, the callee pops none. Calls FindNameIndex_guess (__cdecl, 0x100061f0) with the same two
// arguments, stores the low word of its result at +0x28 of the object at record, then calls DrawQueueRecord_guess
// (__cdecl, 0x10004ec0) with that object as its only argument.
void __cdecl storeCallResult28_guess(unsigned int *record, char *name)
{
    unsigned int index;

    index = EQ_CDECL(unsigned int, 0x100061F0, (unsigned int *, char *))(record, name);
    *(unsigned short *)(record + 10) = (unsigned short)index;
    EQ_CDECL(void, 0x10004EC0, (unsigned int *))(record);
    return;
}

// FUNCTION: EQMAIN 0x10006290
// Opens a rollout's drop-down list below or above its box, waits for a row click or Esc (keeps current choice)
// @early-stop: 93 structural differences remain, mainly register allocation and draw-call argument scheduling.
// __cdecl, 1 stack argument, the callee pops none. Returns a short (in AX, which the callers read). A non-zero short at +0x34
// of the object returns ModalListSelect_guess (__cdecl, 0x10006530) unchanged. Otherwise eqmainPushSlotState_guess is called (__stdcall, no
// arguments: the same call sequence as __cdecl), then +0x2c rows 14 pixels apart are drawn with BltFastSrcRect_guess from the sprite
// at +0x24 (width at +0, height at +4), one call per row and one for the end piece. The rows go down from the anchor (+0x10
// plus the int at +4 of the object at +0x20) when that ends below 0x1e0 (unsigned compare), else up. CreateNamedNode_guess makes the
// list object, ForwardScaledPlusOne_guess gives each row's text (EnqueueNamedRecord_guess adds it), and +0x28 (as an int) is stored at +0x34 of the
// list. Messages from PumpMessagesAndEditText_guess are read until a click on the list (type 1: CheckSelectedIndex_guess gives the row, -1 as 0) or Esc
// (type 4, key 0x1b: the row is +0x28); each is freed. The list is destroyed, eqmainPopSlotState_guess is called, and the row returned.
short __cdecl PopupListChoice_guess(unsigned char *arg)
{
    struct PopupListState
    {
        char unknown00[0x0c];
        int origin;
        int x;
        char unknown14[0x0c];
        int *rect;
        int *sprite;
        short defaultChoice;
        short unknown2a;
        int rowCount;
        char unknown30[4];
        short flags;
    };

    int left;
    int *bitmap;
    int bitmapHeight;
    int rowCount;
    int x;
    unsigned int scaledCount;
    int *rect;
    int tmp;
    unsigned int *list;
    int *msg;
    unsigned int uVar10;
    short row;
    int listTop;
    PopupListState *pPopup;
    pPopup = (PopupListState *)arg;
    if (pPopup->flags != 0)
    {
        return (short)EQ_CDECL(unsigned int, 0x10006530, (unsigned int))((unsigned int)arg);
    }

        EQ_CDECL(void, 0x10001B60, (void))();
        rect = pPopup->rect;
        rowCount = pPopup->rowCount;
        scaledCount = (unsigned int)rowCount * 0xe;
        x = pPopup->x;
        left = pPopup->origin;
        listTop = rect[1] + x;
        bitmap = pPopup->sprite;
        bitmapHeight = bitmap[1];
        *(unsigned char *)&arg = 0;
        tmp = bitmapHeight + -0x11 + (int)scaledCount;
        if ((unsigned int)(tmp + listTop) >= 0x1e0)
        {
            tmp = x - tmp;
            listTop = bitmapHeight + -0xe + tmp;
            EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
                (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)bitmap, 0, 0x10, *bitmap, bitmapHeight + -0x10, 1);
            tmp = tmp + 0x10;
            for (row = 0; row < pPopup->rowCount; row = row + 1)
            {
                bitmap = pPopup->sprite;
                EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
                    (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)bitmap, 0, 0,
                     *bitmap, 0xe, 1);
                tmp = tmp + 0xe;
            }
        }
        else
        {
            tmp = listTop;
            for (row = 0; row < pPopup->rowCount; row = row + 1)
            {
                bitmap = pPopup->sprite;
                EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
                    (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)bitmap, 0, 0,
                     *bitmap, 0xe, 1);
                tmp = tmp + 0xe;
            }
            msg = pPopup->sprite;
            EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
                (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)msg, 0, 0xe, *msg, msg[1] + -0xe, 1);
        }
        msg = pPopup->sprite;
        list = (unsigned int *)EQ_CDECL(unsigned int *, 0x10005840,
                                               (int, int, int, int, int, unsigned int, unsigned int))
            (EQ_GLOBAL(int, 0x1004CEB8), left + 0x14, listTop, *msg + -0x36,
             tmp - listTop, 0, 0x21a3c);
        EQ_CDECL(void, 0x10005D80, (int))((int)list);
        row = 0;
        if (0 < pPopup->rowCount)
        {
            tmp = 0;
            do
            {
                EQ_CDECL(void, 0x10005CC0, (int, int, char *, unsigned int, unsigned int))
                    ((int)list, *pPopup->sprite + -10,
                     EQ_CDECL(char *, 0x100061B0, (unsigned int, int))((unsigned int)pPopup, tmp), 0, 0x22d69);
                row = row + 1;
                tmp = (int)row;
            } while (tmp < pPopup->rowCount);
        }
        list[0xd] = (int)pPopup->defaultChoice;
        EQ_CDECL(void, 0x10005A00, (int))((int)list);
        uVar10 = (unsigned int)arg;
        do
        {
            msg = (int *)EQ_CDECL(int, 0x10003BD0, (void))();
            if (msg != 0)
            {
                switch (*msg)
                {
                case 4:
                    if ((char)msg[2] == '\x1b')
                    {
                        uVar10 = (unsigned short)pPopup->defaultChoice;
                        *(unsigned char *)&arg = 1;
                    }
                    break;
                case 1:
                    if ((unsigned int *)msg[2] == list)
                    {
                        uVar10 = EQ_CDECL(int, 0x10005E40, (int))((int)list);
                        if ((short)uVar10 == -1)
                        {
                            uVar10 = 0;
                        }
                        *(unsigned char *)&arg = 1;
                    }
                    break;
                }
                EQ_CDECL(void, 0x10033568, (void *))(msg);
            }
        } while ((char)arg == '\0');
        EQ_CDECL(void, 0x10005930, (void *))(list);
        EQ_CDECL(void, 0x10001C30, (void))();
        return (short)uVar10;
}

// FUNCTION: EQMAIN 0x100064B0
// Draws the current popup choice or commits a selection and redraws the control.
// __cdecl, 1 stack argument, the callee pops none. Returns nothing. While the int at +0x1c is 0, ForwardScaledPlusOne_guess (__cdecl, called
// with 4 stack arguments: the object, the short at +0x28, 0, 0) gives the text and EQMain_DrawTextRun_guess (__cdecl, 5 arguments) draws it
// at +0xC + 0x14 by +0x10 + 1 on the DC at +8, in colour 0x00C81000. Otherwise PopupListChoice_guess (__cdecl) gives the choice, which is
// stored as a word at +0x28, the int at +0x1c is cleared and DrawQueueRecord_guess (__cdecl, the object) is called.
void __cdecl PopupListDrawOrChoose_guess(unsigned int state)
{
    char *pCVar1;
    unsigned int selection;

    if (*(int *)(state + 0x1c) == 0)
    {
        pCVar1 = EQ_CDECL(char *, 0x100061B0, (unsigned int, int, int, int))(state, (int)*(short *)(state + 0x28), 0, 0);
        EQ_CDECL(void, 0x100082C0, (int, int, int, unsigned int, char *))
            (*(int *)(state + 8), *(int *)(state + 0xc) + 0x14, *(int *)(state + 0x10) + 1, 0xc81000, pCVar1);
        return;
    }
    selection = EQ_CDECL(unsigned int, 0x10006290, (unsigned int))(state);
    *(short *)(state + 0x28) = (short)selection;
    *(int *)(state + 0x1c) = 0;
    EQ_CDECL(void, 0x10004EC0, (unsigned int))(state);
    return;
}

// FUNCTION: EQMAIN 0x10006510
// Updates two 16-bit control values and redraws the record.
// __cdecl, 3 stack arguments, the callee pops none. Stores the word value34 at +0x34 and the word value28 at +0x28, then
// calls DrawQueueRecord_guess (__cdecl, 1 argument: the object, popped by this function).
void __cdecl SetWordFieldsAndUpdate_guess(int record, unsigned short value34, unsigned short value28)
{
    *(unsigned short *)(record + 0x34) = value34;
    *(unsigned short *)(record + 0x28) = value28;
    EQ_CDECL(void, 0x10004EC0, (int))(record);
}

// FUNCTION: EQMAIN 0x10006530
// Modal drop-down of a popup list: scrollable rows, flips upward past y=480, click picks; ESC keeps current pick.
// __cdecl, 1 stack argument, the callee pops none. Returns a 16-bit value (AX). A modal list box: draws its frame with
// BltFastSrcRect_guess (the sprite at +0x24 gives the size; the top is the rect at +0x20's +4 plus +0x10, or above +0x10 when
// the box would run past 0x1E0), makes a list (CreateNamedNode_guess) and its slider (CreateSlider_guess), adds one entry per
// ForwardScaledPlusOne_guess text (+0x2c >> 1 of them), then pumps PumpMessagesAndEditText_guess messages. A type 1 message from the slider scrolls
// the list; from the list it ends with the list's selection (CheckSelectedIndex_guess, -1 is 0); type 4 with byte 0x1B (ESC) ends
// with the default at +0x28. The byte at +0x36 halves the scroll range and the slider's count.
short __cdecl ModalListSelect_guess(unsigned int arg)
{
    int left;
    int *bitmap;
    char done;
    unsigned int record;
    int value;
    int *list;
    char *text;
    int *slider;
    int *msg;
    int bitmapHeight;
    short row;
    unsigned int tmp;
    char drawUp;

    record = arg;
    EQ_CDECL(void, 0x10001B60, (void))();
    done = 0;
    drawUp = 0;
    left = *(int *)(record + 0xc);
    bitmap = *(int **)(record + 0x24);
    bitmapHeight = bitmap[1];
    tmp = *(int *)(*(int *)(record + 0x20) + 4) + *(int *)(record + 0x10);
    value = bitmapHeight + -0x11 + *(short *)(record + 0x34) * 0xe;
    arg = tmp;
    if (tmp + value >= 0x1e0)
    {
        drawUp = 1;
        value = *(int *)(record + 0x10) - value;
        arg = bitmapHeight + -0xe + value;
        EQ_CDECL(void, 0x10006C20, (int, int, int, int, int, int, int, int, int))
            (EQ_GLOBAL(int, 0x1004CEB8), left, value, (int)bitmap, 0, 0x10, *bitmap, bitmapHeight + -0x10, 1);
        tmp = value + 0x10;
    }
    row = 0;
    if (0 < *(short *)(record + 0x34))
    {
        do
        {
            EQ_CDECL(void, 0x10006C20, (int, int, int, int, int, int, int, int, int))
                (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)*(int **)(record + 0x24), 0, 0,
                 **(int **)(record + 0x24), 0xe, 1);
            tmp = tmp + 0xe;
            row = row + 1;
        } while (row < *(short *)(record + 0x34));
    }
    if (!drawUp)
    {
        msg = *(int **)(record + 0x24);
        EQ_CDECL(void, 0x10006C20, (int, int, int, int, int, int, int, int, int))
            (EQ_GLOBAL(int, 0x1004CEB8), left, tmp, (int)msg, 0, 0xe, *msg, msg[1] + -0xe, 1);
    }
    list = EQ_CDECL(int *, 0x10005840, (int, int, int, int, int, unsigned int, int))
        (EQ_GLOBAL(int, 0x1004CEB8), left + 0x14, arg, **(int **)(record + 0x24) + -0x48,
         tmp - arg, 0, 0x21a3c);
    EQ_CDECL(void, 0x10005D80, (int *))(list);
    tmp = 0;
    if ((*(unsigned int *)(record + 0x2c) & 0xfffffffe) != 0)
    {
        do
        {
            EQ_CDECL(void, 0x10005CC0, (int *, int, char *, int, int))
                (list, **(int **)(record + 0x24) + -10,
                 (char *)EQ_CDECL(char *, 0x100061B0, (int, int))(record, tmp), 0, 0x22d69);
            tmp = tmp + 1;
        } while (tmp < *(unsigned int *)(record + 0x2c) >> 1);
    }
    value = (int)*(short *)(record + 0x28);
    list[0xd] = value;
    list[0xb] = value;
    tmp = *(unsigned int *)(record + 0x2c);
    if (*(char *)(record + 0x36) != '\0')
    {
        tmp = tmp >> 1;
    }
    bitmapHeight = tmp - (int)*(short *)(record + 0x34);
    if (bitmapHeight < value)
    {
        list[0xb] = bitmapHeight;
    }
    if ((int)list[0xb] < 0)
    {
        list[0xb] = 0;
    }
    EQ_CDECL(void, 0x10005A00, (int *))(list);
    slider = EQ_CDECL(int *, 0x10006950, (int, int, int, int, int, int))
        (EQ_GLOBAL(int, 0x1004CEB8), **(int **)(record + 0x24) + -0x30 + left, arg, 0x10,
         *(short *)(record + 0x34) * 0xe, 0);
    value = *(int *)(record + 0x2c);
    slider[0x19] = value;
    if (*(char *)(record + 0x36) != '\0')
    {
        slider[0x19] = value / 2;
    }
    slider[0x19] = slider[0x19] + (1 - *(short *)(record + 0x34));
    slider[0x1a] = list[0xb];
    EQ_CDECL(void, 0x100067E0, (int *))(slider);
    do
    {
        msg = (int *)EQ_CDECL(int, 0x10003BD0, (void))();
        if (msg != (int *)0x0)
        {
            if (*msg == 1)
            {
                if ((int *)msg[2] == slider)
                {
                    value = slider[0x1a];
                    list[0xb] = value;
                    if (value < 0)
                    {
                        list[0xb] = 0;
                    }
                    EQ_CDECL(void, 0x10005A00, (int *))(list);
                    EQ_CDECL(void, 0x100067E0, (int *))(slider);
                }
                else if ((int *)msg[2] == list)
                {
                    arg = EQ_CDECL(int, 0x10005E40, (int *))(list);
                    if ((short)arg == -1)
                    {
                        arg = 0;
                    }
                    done = 1;
                }
            }
            else if ((*msg == 4) && ((char)msg[2] == '\x1b'))
            {
                arg = *(short *)(record + 0x28);
                done = 1;
            }
            EQ_CDECL(void, 0x10033568, (void *))(msg);
        }
    } while (!done);
    EQ_CDECL(void, 0x10005930, (int *))(list);
    EQ_CDECL(void, 0x10006AE0, (int *))(slider);
    EQ_CDECL(void, 0x10001C30, (void))();
    return (short)arg;
}

// FUNCTION: EQMAIN 0x100067E0
// Clamps a stepped slider value to its range and redraws its track, endpoints, and thumb.
// __cdecl, 1 stack argument, the callee pops none. A stepped slider: the value at +0x6c runs from +0x58 to +0x60 less
// the height of the sprite at +0x30, in +0x64 steps, the step index being at +0x68. When the byte at +0x70 is set, the
// value is clamped and the index taken from it. The index is clamped to 0..steps-1 and the value set to that step.
// Then BltFastSrcRect_guess draws four sprites: +0x1c at (+0x0c, +0x10); +0x24 (index 0) or +0x20 at
// (+0x34 + 2, +0x38); +0x2c (last index) or +0x28 at (+0x44 + 2, +0x48); and +0x30 at (+0x54 + 2, value), the value
// first clamped to +0x60 less that sprite's height.
// @early-stop: first clamp loads min before value; the 3rd and 4th sprite blocks take ECX/EAX swapped (cause not found)
void __cdecl RefreshSlider_guess(int slider)
{
    int tmp;
    int value;
    int *sprite;

    if (slider != 0)
    {
        if (*(char *)(slider + 0x70) != '\0')
        {
            if (*(int *)(slider + 0x6c) < (value = *(int *)(slider + 0x58)))
            {
                *(int *)(slider + 0x6c) = value;
            }
            else
            {
                tmp = *(int *)(slider + 0x60) - *(int *)(*(int *)(slider + 0x30) + 4);
                if (*(int *)(slider + 0x6c) > tmp)
                {
                    *(int *)(slider + 0x6c) = tmp;
                }
            }
            tmp = (*(int *)(slider + 0x6c) - value) * (*(int *)(slider + 0x64) - 1);
            *(int *)(slider + 0x68) = tmp;
            *(int *)(slider + 0x68) =
                tmp / ((*(int *)(slider + 0x60) - *(int *)(*(int *)(slider + 0x30) + 4)) - value);
        }
        if (*(int *)(slider + 0x68) < 0)
        {
            *(int *)(slider + 0x68) = 0;
        }
        else if (*(int *)(slider + 0x68) >= *(int *)(slider + 0x64))
        {
            *(int *)(slider + 0x68) = *(int *)(slider + 0x64) - 1;
        }
        if (*(int *)(slider + 0x64) > 1)
        {
            value = (((*(int *)(slider + 0x60) - *(int *)(*(int *)(slider + 0x30) + 4)) -
                      *(int *)(slider + 0x58)) * *(int *)(slider + 0x68)) /
                    (*(int *)(slider + 0x64) - 1) + *(int *)(slider + 0x58);
        }
        else
        {
            value = *(int *)(slider + 0x58);
        }
        *(int *)(slider + 0x6c) = value;
        sprite = *(int **)(slider + 0x1c);
        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
            (*(int *)(slider + 8), *(int *)(slider + 0xc), *(int *)(slider + 0x10), (int)sprite, 0, 0,
             sprite[0], sprite[1], 1);
        if (*(int *)(slider + 0x68) == 0)
        {
            sprite = *(int **)(slider + 0x24);
        }
        else
        {
            sprite = *(int **)(slider + 0x20);
        }
        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
            (*(int *)(slider + 8), *(int *)(slider + 0x34) + 2, *(int *)(slider + 0x38), (int)sprite, 0, 0,
             sprite[0], sprite[1], 1);
        if (*(int *)(slider + 0x68) >= *(int *)(slider + 0x64) - 1)
        {
            sprite = *(int **)(slider + 0x2c);
        }
        else
        {
            sprite = *(int **)(slider + 0x28);
        }
        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
            (*(int *)(slider + 8), *(int *)(slider + 0x44) + 2, *(int *)(slider + 0x48), (int)sprite, 0, 0,
             sprite[0], sprite[1], 1);
        sprite = *(int **)(slider + 0x30);
        value = *(int *)(slider + 0x60) - sprite[1];
        if (*(int *)(slider + 0x6c) > value)
        {
            *(int *)(slider + 0x6c) = value;
        }
        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
            (*(int *)(slider + 8), *(int *)(slider + 0x54) + 2, *(int *)(slider + 0x6c), (int)sprite, 0, 0,
             sprite[0], sprite[1], 1);
    }
    return;
}

// FUNCTION: EQMAIN 0x10006950
// Creates a list scrollbar with up/down arrows and a movable thumb.
// __cdecl, 6 stack arguments, the callee pops none. A null owner (owner) returns 0. Otherwise a 0x74-byte slider is
// allocated, and 0 is returned when _malloc fails: the owner at +0x08, the rect (x, y) to (+width,
// +height) at +0x0c..+0x18, its surface at +0x1c. A null surface makes a width x height surface (CreateSurfaceRecord_guess,
// __cdecl, 3 arguments: w, h, 16) and Blts the owner's surface at +0x0c into it (vtable slot 5, __stdcall, 6 arguments:
// the new surface, &{0, 0, w, h}, the owner's surface, the rect, DDBLT_WAIT | DDBLT_ROP, &DDBLTFX with dwSize 100 and
// dwROP SRCCOPY); a non-null surface is used as it is. Five bitmaps (sbup1, sbup2, sbdown1, sbdown2 and sbthumb .bmp,
// PackFsGetNamedImage_guess, __cdecl, 2 arguments: name, 16) go to +0x20..+0x30. The fields from +0x34 to +0x6c are then set from the
// rect and the bitmaps' first two words, +0x70 is cleared, RefreshSlider_guess refreshes the slider and list3_insert_guess (__cdecl,
// 3 arguments: owner + 0x1c, 0, the slider) adds it to the owner's list.
int *__cdecl CreateSlider_guess(int owner, int x, int y, int width, int height, int surface)
{
    int *slider;
    int *sliderRect;
    int left;
    int downArrowHeight;
    int *bitmap;
    int sourceRect[4];    // RECT: 0, 0, w, h
    int bltEffects[25];   // DDBLTFX: dwSize at [0], dwROP at [2]

    if (owner == 0)
    {
        return 0;
    }
    slider = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x74);
    if (slider != 0)
    {
        sliderRect = slider + 3;
        slider[4] = y;
        slider[0] = 3;
        *sliderRect = x;
        slider[5] = x + width;
        slider[2] = owner;
        slider[6] = y + height;
        if (surface != 0)
        {
            slider[7] = surface;
        }
        else
        {
            bitmap = EQ_CDECL(int *, 0x10006D80, (int, int, int))(width, height, 0x10);
            slider[7] = (int)bitmap;
            sourceRect[0] = 0;
            sourceRect[1] = 0;
            sourceRect[2] = width;
            sourceRect[3] = height;
            bltEffects[0] = 100;
            bltEffects[2] = 0xcc0020;
            ((void (__stdcall *)(int *, int *, int *, int *, int, int *))(*(void ***)(*(int **)(slider[7] + 0xc)))[5])(
                *(int **)(slider[7] + 0xc), sourceRect, *(int **)(owner + 0xc), sliderRect, 0x1020000, bltEffects);
        }
        bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x10046430, 0x10);
        slider[8] = (int)bitmap;
        bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x10046424, 0x10);
        slider[9] = (int)bitmap;
        bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x10046418, 0x10);
        slider[10] = (int)bitmap;
        bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x1004640c, 0x10);
        slider[11] = (int)bitmap;
        bitmap = EQ_CDECL(int *, 0x10007AE0, (char *, int))((char *)0x10046400, 0x10);
        slider[12] = (int)bitmap;
        left = *sliderRect;
        slider[13] = left;
        slider[14] = slider[4];
        slider[15] = *(int *)slider[8] + left;
        slider[16] = ((int *)slider[8])[1] + slider[4];
        slider[17] = left;
        slider[20] = slider[6];
        slider[19] = left + *(int *)slider[10];
        downArrowHeight = ((int *)slider[10])[1];
        slider[21] = left;
        slider[18] = slider[6] - downArrowHeight;
        slider[22] = slider[16];
        slider[23] = slider[5];
        slider[24] = slider[18];
        slider[25] = 0;
        slider[26] = 0;
        *(char *)(slider + 0x1c) = 0;
        slider[27] = slider[16];
        EQ_CDECL(void, 0x100067E0, (int))((int)slider);
        EQ_CDECL(void, 0x10028340, (int, int, int))(owner + 0x1c, 0, (int)slider);
    }
    return slider;
}

// FUNCTION: EQMAIN 0x10006AE0
// Disposes of a front-end UI object and the images it owns.
// __cdecl, 1 stack argument, the callee pops none. A null unit returns at once. Otherwise g_cached_entry_a_guess is cleared when it
// holds the unit, and each non-null pointer at +0x20, +0x24, +0x28, +0x2c and +0x30 is released through PackFsFreeEntry_guess
// (__cdecl, 1 argument). The unit's node in the list at its parent's +0x1c (the parent at +0x08; a node's item at +0x04,
// its next at +0x08) is unlinked with list3_unlink_free_guess (__cdecl, 2 arguments: the list, the node) when found. The unit is
// freed with _free (__cdecl, 1 argument) on every path: unlinked, not in the list, or the list empty.
void __cdecl ReleaseQueueUnit_guess(void *unit)
{
    int *node;
    int *list;    // parent + 0x1c: the list's address (ECX in the asm)

    if (unit != 0)
    {
        if (unit == EQ_GLOBAL(void *, 0x1004CE6C))
        {
            EQ_GLOBAL(void *, 0x1004CE6C) = 0;
        }
        if (*(int **)((int)unit + 0x20) != 0)
        {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)unit + 0x20));
        }
        if (*(int **)((int)unit + 0x24) != 0)
        {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)unit + 0x24));
        }
        if (*(int **)((int)unit + 0x28) != 0)
        {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)unit + 0x28));
        }
        if (*(int **)((int)unit + 0x2c) != 0)
        {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)unit + 0x2c));
        }
        if (*(int **)((int)unit + 0x30) != 0)
        {
            EQ_CDECL(void, 0x10007A80, (int *))(*(int **)((int)unit + 0x30));
        }
        list = (int *)(*(int *)((int)unit + 8) + 0x1c);
        node = *(int **)(*(int *)((int)unit + 8) + 0x1c);
        while (node != 0)
        {
            if (unit != (void *)node[1])
            {
                node = (int *)node[2];
            }
            else
            {
                EQ_CDECL(int, 0x10028430, (int *, int *))(list, node);
                break;
            }
        }
        EQ_CDECL(void, 0x10033568, (void *))(unit);
    }
    return;
}

// FUNCTION: EQMAIN 0x10006B90
// Clears a front-end drawing surface to a chosen RGB color.
// __cdecl, 2 stack arguments. surface is a wrapper whose IDirectDrawSurface is at +0x0c; colorRef is a COLORREF (red in
// the low byte). PackFsPackColor16_guess (__cdecl, 3 arguments: the red, green and blue bytes) returns a 15-bit pixel, masked to
// 16 bits, kept as the DDBLTFX's dwFillColor. Both are set before the wrapper is checked: a null wrapper returns 0.
// Otherwise the surface's Blt (vtable slot 5, __stdcall, 6 arguments: this, null, null, null, flags 0x01000400 =
// DDBLT_WAIT | DDBLT_COLORFILL, &DDBLTFX) colour-fills it, and a zero result returns 1. Otherwise FatalErrorExit_guess (__cdecl,
// 3 arguments: the format global at 0x10055A8C, the HRESULT, colorRef) logs the failure and its result is returned with
// the low byte cleared. The 100-byte DDBLTFX is the whole frame: dwSize 100 at [0], dwFillColor at [20] (+0x50);
// [21..24] (the colour keys) are never written. The original returns through AL only (XOR AL,AL / MOV AL,1), so the
// type is unsigned char.
unsigned char __cdecl BltColorFillRgb_guess(int surface, unsigned int colorRef)
{
    int result;
    int bltEffects[25];   // DDBLTFX: dwSize at [0] (100), dwFillColor at [20] (+0x50)

    bltEffects[0] = 100;
    bltEffects[20] = EQ_CDECL(unsigned int, 0x10007E60, (unsigned int, unsigned int, unsigned int))(
        (unsigned char)colorRef, (unsigned char)((unsigned short)colorRef >> 8), (unsigned char)(colorRef >> 0x10));
    bltEffects[20] = bltEffects[20] & 0xffff;
    if (surface == 0)
        return 0;
    result = ((int (__stdcall *)(int *, int, int, int, unsigned int, int *))
             (*(void ***)(*(int **)(surface + 0xc)))[5])(*(int **)(surface + 0xc), 0, 0, 0, 0x1000400, bltEffects);
    if (result != 0)
    {
        EQ_CDECL(unsigned int, 0x100278A0, (int, int, unsigned int))(
            EQ_GLOBAL(int, 0x10055A8C), result, colorRef);
        return 0;   // FatalErrorExit_guess's result & 0xffffff00, as the char return: its low byte is always 0
    }
    return 1;
}

// FUNCTION: EQMAIN 0x10006C20
// Draws a rectangular image region, optionally using color-key transparency.
// @early-stop: bit-0 flag test is `test dl,1` (original `mov eax,edx; and eax,1`); bit 1 tested via a copy.
// __cdecl, 9 stack arguments. dest and src are wrappers whose IDirectDrawSurface is at +0x0c. The dest
// surface's slot 7 (BltFast by its arguments, __stdcall, 6 arguments) copies the source rectangle
// {srcX, srcY, srcX + width, srcY + height} to (x, y) with flags 0x10, plus 0x01 when
// flags bit 0 is set and 0x02 when bit 1 is set. A zero result returns 1. Otherwise FatalErrorExit_guess (__cdecl, 8
// arguments) logs the failure and the low byte of its result is cleared. A null wrapper clears only AL, preserving
// the incoming EAX high bits. The unsigned-char return lets VC6 emit the original AL-only write without inline asm.
/* fake match: srcKey/destKey locals keep flags a dword in EDX instead of a byte load */
unsigned char __cdecl BltFastSrcRect_guess(int *dest, int x, int y, int *src, int srcX,
                                           int srcY, int width, int height, unsigned int flags)
{
    int hr;
    unsigned int bltFlags;
    unsigned int srcKey;
    unsigned int destKey;
    struct Rect_guess
    {
        int left;
        int top;
        int right;
        int bottom;
    } rect;

    if (dest == 0)
        return 0;
    if (src == 0)
        return 0;
    rect.left = srcX;
    rect.top = srcY;
    rect.right = srcX + width;
    rect.bottom = srcY + height;
    srcKey = flags & 1;
    destKey = flags & 2;
    bltFlags = 0x10;
    if (srcKey != 0)
        bltFlags = 0x11;
    if (destKey != 0)
        bltFlags = bltFlags | 2;
    hr = ((int (__stdcall *)(void *, int, int, void *, Rect_guess *, unsigned int))
          (*(void ***)dest[3])[7])((void *)dest[3], x, y, (void *)src[3], &rect, bltFlags);
    if (hr != 0)
    {
        EQ_CDECL(void, 0x100278A0, (int, int, int, int, int, int, int, int))(
            EQ_GLOBAL(int, 0x10055A90), hr, x, y, rect.left, rect.top, rect.right,
            rect.bottom);
        return 0;
    }
    return 1;
}

// FUNCTION: EQMAIN 0x10006CE0
// Restores and locks a front-end surface to expose its pixels for drawing.
// __cdecl, one argument: a surface wrapper whose IDirectDrawSurface is at +0x0c. A null wrapper returns 0 and
// touches nothing. Otherwise: zero a 0x6c-byte DDSURFACEDESC (dwSize 0x6c), call the surface's vtable slot 0x6c
// (Restore, __stdcall, 1 argument; its result is ignored), then slot 0x64 (Lock, __stdcall, 5 arguments: surface,
// NULL, &desc, DDLOCK_WAIT (1), NULL). A nonzero Lock result logs through FatalErrorExit_guess (__cdecl, 1 argument:
// DAT_10055a94) and returns 0. Otherwise wrapper +0x14 = desc lpSurface (+0x24), wrapper +0x18 = desc lPitch
// (+0x10), and it returns 1. The original returns through AL only (XOR AL,AL / MOV AL,1), so the type is unsigned char.
unsigned char __cdecl RestoreAndLockSurface_guess(int *surface)
{
    int desc[27];   // DDSURFACEDESC: dwSize [0], lPitch [4] (0x10), lpSurface [9] (0x24)
    int hr;

    if (surface == 0)
        return 0;
    memset(desc, 0, sizeof(desc));
    desc[0] = 0x6c;
    ((void (__stdcall *)(int *))(*(void ***)(surface[3]))[0x1b])((int *)surface[3]);
    hr = ((int (__stdcall *)(int *, int *, int *, int, int))
        (*(void ***)(surface[3]))[0x19])((int *)surface[3], 0, desc, 1, 0);
    if (hr != 0)
    {
        EQ_CDECL(void, 0x100278A0, (int))(EQ_GLOBAL(int, 0x10055A94));
        return 0;
    }
    surface[5] = desc[9];
    surface[6] = desc[4];
    return 1;
}

// FUNCTION: EQMAIN 0x10006d60
// Invokes an unidentified surface operation; its purpose is unclear.
// __cdecl. Calls vtable slot 0x80 (index 0x20) of the sub-object at surface+0x0c as stdcall (sub-object, 0).
// The call site has no ADD ESP: the callee pops its two arguments, so the RET below is the caller's.
void __cdecl CallSubobjectSlot80_guess(int surface)
{
    ((void (__stdcall *)(int *, int))(*(void ***)(*(int **)(surface + 0xc)))[0x20])(*(int **)(surface + 0xc), 0);
}

// DirectDraw's DDCOLORKEY and DDSURFACEDESC as file-local stand-ins (the tree has no ddraw.h).
struct DDCOLORKEY_guess
{
    unsigned int dwColorSpaceLowValue;   // 0x00
    unsigned int dwColorSpaceHighValue;  // 0x04
};

struct DDSURFACEDESC_guess
{
    unsigned int dwSize;                 // 0x00 (0x6c)
    unsigned int dwFlags;                // 0x04
    unsigned int dwHeight;               // 0x08
    unsigned int dwWidth;                // 0x0c
    unsigned int lPitch;                 // 0x10
    unsigned int dwBackBufferCount;      // 0x14
    unsigned int dwMipMapCount;          // 0x18
    unsigned int dwAlphaBitDepth;        // 0x1c
    unsigned int dwReserved;             // 0x20
    void *lpSurface;                     // 0x24
    DDCOLORKEY_guess ddckCKDestOverlay;  // 0x28
    DDCOLORKEY_guess ddckCKDestBlt;      // 0x30
    DDCOLORKEY_guess ddckCKSrcOverlay;   // 0x38
    DDCOLORKEY_guess ddckCKSrcBlt;       // 0x40
    unsigned int ddpfPixelFormat[8];     // 0x48 (DDPIXELFORMAT, 0x20 bytes)
    unsigned int ddsCaps;                // 0x68 (DDSCAPS: dwCaps)
};

// FUNCTION: EQMAIN 0x10006d80
// Creates a magenta-keyed offscreen surface for drawing front-end images.
// @early-stop: 18 instructions differ only in register choice; EQMAIN_CDECL's function-pointer calls (mov reg,VA) shift the allocation.
// __cdecl. Allocates a 0x28-byte record: fields 0..2 = width (surface width), height (height), fieldValue; the
// 3-dword block at +0x1c is cleared by ClearRecord3_guess. Field 3 receives an offscreen plain surface of
// width x height from CreateSurface on the DirectDraw object at g_eqmain_ui_arg2_guess; its source and destination color keys
// are PackFsPackColor16_guess(0xff, 0, 0xff) & 0xffff (magenta). Field 4 = 0. The record joins the list at g_packfs_list_head_guess and
// width * height * 2 is added to g_packfs_used_bytes_guess. Returns the record, or NULL after freeing it when malloc or
// CreateSurface fails (FatalErrorExit_guess is first called with DAT_10055a98).
int * __cdecl CreateSurfaceRecord_guess(int width, int height, int fieldValue)
{
    typedef int (__stdcall *CreateSurfaceFn_guess)(void *, DDSURFACEDESC_guess *, void **, void *);
    typedef int (__stdcall *SetColorKeyFn_guess)(void *, unsigned int, DDCOLORKEY_guess *);
    DDSURFACEDESC_guess desc;
    DDCOLORKEY_guess ck;
    int *record;
    void **surfaceOut;
    void *ddraw;

    record = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(0x28);
    if (record != 0)
    {
        record[0] = width;
        record[1] = height;
        record[2] = fieldValue;
        EQ_CDECL(void, 0x10028330, (int *))(record + 7);
        memset(&desc, 0, sizeof(desc));
        ddraw = EQ_GLOBAL(void *, 0x1004CE74);
        surfaceOut = (void **)(record + 3);
        desc.dwSize = sizeof(desc);
        desc.dwWidth = width;
        desc.dwHeight = height;
        desc.ddsCaps = 0x40;   // DDSCAPS_OFFSCREENPLAIN
        desc.dwFlags = 0x7;    // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH
        if (((CreateSurfaceFn_guess)(*(void ***)ddraw)[6])(ddraw, &desc, surfaceOut, 0) != 0)
        {
            EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A98));
            EQ_CDECL(void, 0x10033568, (void *))(record);
            return 0;
        }
        record[4] = 0;
        ck.dwColorSpaceLowValue = EQ_CDECL(unsigned int, 0x10007E60,
            (unsigned int, unsigned int, unsigned int))(0xff, 0, 0xff) & 0xffff;
        ck.dwColorSpaceHighValue = EQ_CDECL(unsigned int, 0x10007E60,
            (unsigned int, unsigned int, unsigned int))(0xff, 0, 0xff) & 0xffff;
        ((SetColorKeyFn_guess)(*(void ***)*surfaceOut)[29])(*surfaceOut, 8, &ck);
        ((SetColorKeyFn_guess)(*(void ***)*surfaceOut)[29])(*surfaceOut, 2, &ck);
        EQ_CDECL(void, 0x100283B0, (int *, int *, int))(&EQ_GLOBAL(int, 0x1004CE90), 0, (int)record);
        EQ_GLOBAL(int, 0x1004CEBC) = EQ_GLOBAL(int, 0x1004CEBC) + width * height * 2;
    }
    return record;
}

// FUNCTION: EQMAIN 0x10006ec0
// Loads a named 24-bit BMP from eqfeart1..6.pak (first pak that has it) into a surface, 16-bit packed if asked.
// __cdecl. Tries eqfeart1.pak .. eqfeart6.pak in turn: FfResourceOwner_openPackEntry_guess (this = a new pak owner, then the pak name and the
// name built from name, both passed by value) returns nonzero for the first pak that has the image. The image is a
// 14-byte chunk header (size dword at +0xa), then width (+4), height (+8) and a bpp short at +0xe that must be 0x18,
// then the pixels: a surface from CreateSurfaceRecord_guess, packed to 16 bits when bitDepth == 0x10 (shifts and bit positions from
// DAT_1004ceac..ceb2). Returns the surface record, or NULL (after logging DAT_10055aa0 with name).
int * LoadFrontEndArt_guess(char *name, int bitDepth)
{
    int *holder;
    char *path;
    int *ref;
    int dest[2];
    int src[2];
    int srcCopy[2];
    int pakName[2];
    int srcA[2];
    int dstA[2];
    int srcB[2];
    int dstB[2];
    char found;
    unsigned char header[14];
    int remainSize;
    int *remain;
    int pixelSize;
    int stride;
    unsigned char *pixelBuf;
    int *surface;
    int height;
    int row;
    int x;
    unsigned short *rowBase;
    unsigned short *out;
    unsigned char *srcp;
    unsigned int b;
    unsigned int g;
    unsigned int r;
    unsigned int val;

    if (name == 0)
    {
        EQ_CDECL(void, 0x100278A0, (char *))(EQ_GLOBAL(char *, 0x10055A9C));
        return 0;
    }
    holder = (int *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(4);
    if (holder == 0)
    {
        holder = 0;
    }
    else
    {
        *holder = 0;
    }
    path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
    EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(dest, 0, (char *)0);
    EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(src, 0, path);
    ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(srcCopy, 0, src);
    EQ_THISCALL(void *, 0x10007620, (int *, int, void *))(dest, 0, ref);
    if (srcCopy[0] != 0)
    {
        EQ_THISCALL(void, 0x10007520, (int *, int))((int *)srcCopy[0], 0);
    }
    if (src[0] != 0)
    {
        EQ_THISCALL(void, 0x10007520, (int *, int))((int *)src[0], 0);
    }
    EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x100465b8);
    found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
    if (found == 0)
    {
        path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
        EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(dest, 0, (char *)0);
        EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(src, 0, path);
        ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(srcCopy, 0, src);
        EQ_THISCALL(void *, 0x10007620, (int *, int, void *))(dest, 0, ref);
        if (srcCopy[0] != 0)
        {
            EQ_THISCALL(void, 0x10007520, (int *, int))((int *)srcCopy[0], 0);
        }
        if (src[0] != 0)
        {
            EQ_THISCALL(void, 0x10007520, (int *, int))((int *)src[0], 0);
        }
        EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x100465a8);
        found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
        if (found == 0)
        {
            path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
            EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(dest, 0, (char *)0);
            EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(src, 0, path);
            ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(srcCopy, 0, src);
            EQ_THISCALL(void *, 0x10007620, (int *, int, void *))(dest, 0, ref);
            if (srcCopy[0] != 0)
            {
                EQ_THISCALL(void, 0x10007520, (int *, int))((int *)srcCopy[0], 0);
            }
            if (src[0] != 0)
            {
                EQ_THISCALL(void, 0x10007520, (int *, int))((int *)src[0], 0);
            }
            EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x10046598);
            found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
            if (found == 0)
            {
                path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
                EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(dest, 0, (char *)0);
                EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(src, 0, path);
                ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(srcCopy, 0, src);
                EQ_THISCALL(void *, 0x10007620, (int *, int, void *))(dest, 0, ref);
                if (srcCopy[0] != 0)
                {
                    EQ_THISCALL(void, 0x10007520, (int *, int))((int *)srcCopy[0], 0);
                }
                if (src[0] != 0)
                {
                    EQ_THISCALL(void, 0x10007520, (int *, int))((int *)src[0], 0);
                }
                EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x10046588);
                found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
                if (found == 0)
                {
                    path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
                    EQ_THISCALL(void *, 0x10007F70, (int *, int, unsigned int))(dest, 0, 0);
                    dest[1] = 0;
                    EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(srcA, 0, path);
                    ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(dstA, 0, srcA);
                    EQ_THISCALL(void *, 0x10007850, (int *, int, void *))(dest, 0, ref);
                    EQ_THISCALL(void, 0x10010B50, (int *, int))(dstA, 0);
                    EQ_THISCALL(void, 0x10010B50, (int *, int))(srcA, 0);
                    EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x10046578);
                    found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
                    if (found == 0)
                    {
                        path = (char *)EQ_CDECL(char *, 0x100211B0, (char *))(name);
                        EQ_THISCALL(void *, 0x10007F70, (int *, int, unsigned int))(dest, 0, 0);
                        dest[1] = 0;
                        EQ_THISCALL(void *, 0x10007540, (int *, int, char *))(srcB, 0, path);
                        ref = (int *)EQ_THISCALL(void *, 0x10007870, (int *, int, int *))(dstB, 0, srcB);
                        EQ_THISCALL(void *, 0x10007850, (int *, int, void *))(dest, 0, ref);
                        EQ_THISCALL(void, 0x10010B50, (int *, int))(dstB, 0);
                        EQ_THISCALL(void, 0x10010B50, (int *, int))(srcB, 0);
                        EQ_THISCALL(void *, 0x10007650, (int *, int, char *))(pakName, 0, (char *)0x10046568);
                        found = EQ_THISCALL(char, 0x1000EF30, (int *, int, int, int, int, int))(holder, 0, pakName[0], pakName[1], dest[0], dest[1]);
                        if (found == 0)
                        {
                            if (holder != 0)
                            {
                                EQ_THISCALL(void, 0x1000EF20, (void *, int))(holder, 0);
                                EQ_CDECL(void, 0x10033800, (void *))(holder);
                            }
                            return 0;
                        }
                    }
                }
            }
        }
    }

    EQ_THISCALL(int, 0x1000F230, (void *, int, void *, unsigned int))(holder, 0, header, 14);
    remainSize = *(int *)(header + 10) - 14;
    remain = (int *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(remainSize);
    EQ_THISCALL(int, 0x1000F230, (void *, int, void *, unsigned int))(holder, 0, remain, remainSize);
    if (*(short *)((char *)remain + 0xe) == 0x18)
    {
        stride = ((remain[1] * 0x18 + 0x1f) >> 3) & 0xfffffffc;
        pixelSize = stride * remain[2];
        pixelBuf = (unsigned char *)EQ_CDECL(void *, 0x1003342E, (unsigned int))(pixelSize);
        EQ_THISCALL(int, 0x1000F230, (void *, int, void *, unsigned int))(holder, 0, pixelBuf, pixelSize);
        surface = (int *)EQ_CDECL(int *, 0x10006D80, (int, int, int))(remain[1], remain[2], bitDepth);
        if (surface != 0)
        {
            EQ_CDECL(void, 0x10006CE0, (int *))(surface);
            if (bitDepth == 0x10)
            {
                height = remain[2];
                rowBase = (unsigned short *)surface[5];
                row = 0;
                if (0 < height)
                {
                    do
                    {
                        x = 0;
                        if (0 < remain[1])
                        {
                            srcp = pixelBuf + (height - row - 1) * stride;
                            out = rowBase;
                            do
                            {
                                b = srcp[0];
                                g = srcp[1];
                                r = srcp[2];
                                srcp += 3;
                                val = (b >> (EQ_GLOBAL(unsigned char, 0x1004CEAC) & 0x1f))
                                    << (EQ_GLOBAL(unsigned char, 0x1004CEB0) & 0x1f);
                                val |= (g >> (EQ_GLOBAL(unsigned char, 0x1004CEAD) & 0x1f))
                                    << (EQ_GLOBAL(unsigned char, 0x1004CEB1) & 0x1f);
                                val |= (r >> (EQ_GLOBAL(unsigned char, 0x1004CEAE) & 0x1f))
                                    << (EQ_GLOBAL(unsigned char, 0x1004CEB2) & 0x1f);
                                *out = (unsigned short)val;
                                out++;
                                x++;
                            } while (x < remain[1]);
                        }
                        rowBase = (unsigned short *)((char *)rowBase + surface[6]);
                        row++;
                    } while (row < height);
                }
            }
            EQ_CDECL(void, 0x10006D60, (int *))(surface);
        }
        EQ_CDECL(void, 0x10033568, (void *))(pixelBuf);
        EQ_CDECL(void, 0x10033568, (void *))(remain);
        EQ_THISCALL(void, 0x1000F1D0, (void *, int))(holder, 0);
        if (holder != 0)
        {
            EQ_THISCALL(void, 0x1000EF20, (void *, int))(holder, 0);
            EQ_CDECL(void, 0x10033800, (void *))(holder);
        }
        return surface;
    }
    if (holder != 0)
    {
        EQ_THISCALL(void, 0x1000EF20, (void *, int))(holder, 0);
        EQ_CDECL(void, 0x10033800, (void *))(holder);
    }
    EQ_CDECL(void, 0x10033568, (void *))(remain);
    EQ_CDECL(void, 0x100278A0, (char *, char *))(EQ_GLOBAL(char *, 0x10055AA0), name);
    return 0;
}
