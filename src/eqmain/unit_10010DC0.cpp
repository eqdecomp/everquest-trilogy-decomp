// unit_10010DC0.cpp - eqmain.dll, the address run 0x10010DC0..0x10011970 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

// File-local stand-in for the record (no header defines it yet): vtable at 0x0, then three dwords.
class EQMain_Record_10010DC0_guess
{
public:
    void *vtbl;         // 0x0
    int field_4;        // 0x4
    int field_8;        // 0x8
    int field_c;        // 0xc

    EQMain_Record_10010DC0_guess *Init_guess(int param_1, int param_2);
};

// FUNCTION: EQMAIN 0x10010DC0
// Initializes a front-end record; its specific purpose is unclear.
// __thiscall, two int arguments (callee pops 8). Stores 0 at +0x4, the first argument at +0x8, the second at
// +0xc, then the vtable address 0x10041f68 at +0x0. Returns this (EAX = ECX in the original).
EQMain_Record_10010DC0_guess *EQMain_Record_10010DC0_guess::Init_guess(int field8Value, int fieldCValue)
{
    this->field_4 = 0;
    this->field_8 = field8Value;
    this->field_c = fieldCValue;
    this->vtbl = (void *)0x10041f68;
    return this;
}

// The CRT calls the function makes (VC6 LIBCMT, __cdecl), at their addresses in eqmain.dll.
#define EQMain_strtok_guess(s, delim) EQ_CDECL(char *, 0x10033996, (char *, char *))(s, delim)
#define EQMain_isdigit_guess(c)       EQ_CDECL(int, 0x10033861, (int))(c)
#define EQMain_atoi_guess(s)          EQ_CDECL(int, 0x1003507E, (char *))(s)
// "//-." (strtok's delimiters) in eqmain.dll's data at 0x10047f5c.
#define g_date_delims_guess EQ_ARRAY(char, 0x10047F5C)

// File-local stand-in for the resource object (no header defines it): the vptr at 0x0 (slot 0 is Destroy_guess,
// reached only through the vptr, never linked), the reference count at 0x4, the data pointer at 0x8.
class EQMain_ResourceObject_guess
{
public:
    virtual void Destroy_guess(int param_1);    // 0x0 (vptr, slot 0)
    int refCount;                               // 0x4
    void *data;                                 // 0x8

    EQMain_ResourceObject_guess *DestroyAndFreeIfRequested_destructor_guess(unsigned char flags);   // 0x10010DE0
};

// FUNCTION: EQMAIN 0x10010DE0
// Releases a front-end resource and optionally frees its object record.
// __thiscall, one stack arg (callee pops 4). Runs DestroyResourceAndDecrementRefCount_guess (0x10010E00) on this,
// then frees this with the cdecl FUN_10033800 when bit 0 of flags is set. Returns this (EAX = ESI in the original).
EQMain_ResourceObject_guess *EQMain_ResourceObject_guess::DestroyAndFreeIfRequested_destructor_guess(unsigned char flags)
{
    EQ_METHOD(0, void, 0x10010E00, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(this);
    return this;
}

// FUNCTION: EQMAIN 0x10010E00
// Drops one reference to a front-end resource, destroying it when the count reaches zero.
// __fastcall (this in ECX, no stack args, callee pops 0). Points the vtable at 0x10041F68 (the ResourceObject
// destructor), frees the data at 0x8 with the cdecl FUN_10033800, then points the vtable at 0x10040DFC (the
// __purecall stub) and stores refCount - 1. When that is zero and the pointer is not null, calls slot 0 of the
// vtable with 1.
void __fastcall EQMain_DestroyResourceAndDecrementRefCount_guess(EQMain_ResourceObject_guess *obj)
{
    int refCount;

    *(const void **)obj = (const void *)0x10041F68;
    EQ_CDECL(void, 0x10033800, (void *))(obj->data);
    refCount = obj->refCount;
    refCount--;
    *(const void **)obj = (const void *)0x10040DFC;
    obj->refCount = refCount;
    if (refCount == 0 && obj != 0)
    {
        // Model thiscall with fastcall so the reloaded vtable occupies the unused EDX slot.
        typedef void (__fastcall *DestroyMethod)(EQMain_ResourceObject_guess *, void **, int);
        void **vtable = *(void ** volatile *)obj;
        ((DestroyMethod *)vtable)[0](obj, vtable, 1);
    }
}

// FUNCTION: EQMAIN 0x10010E40
// Validates a front-end date entry before it is accepted.
// __cdecl, one char * argument (callee pops 0). Returns a char (AL): 1 when the string reads as M/D/YYYY, else 0.
// It copies the string into a 16-byte local with no length check (16 or more characters run past the local, over
// the return address), then strtok splits it at "//-.": a 2-digit month 1..12, a 2-digit day 1..31 and a 4-digit
// year 1900..2100, every character a digit. Tokens after the third are ignored.
char EQMain_IsValidDate_guess(char *dateText)
{
    char dateBuffer[16];
    char *tok;
    int month;
    int day;
    int year;

    strcpy(dateBuffer, dateText);
    if (strlen(dateBuffer) != 0)
    {
        tok = EQMain_strtok_guess(dateBuffer, g_date_delims_guess);
        if (tok != NULL && strlen(tok) == 2 && EQMain_isdigit_guess((int)tok[0]) != 0 &&
            EQMain_isdigit_guess((int)tok[1]) != 0)
        {
            month = EQMain_atoi_guess(tok);
            tok = EQMain_strtok_guess(NULL, g_date_delims_guess);
            if (tok != NULL && strlen(tok) == 2 && EQMain_isdigit_guess((int)tok[0]) != 0 &&
                EQMain_isdigit_guess((int)tok[1]) != 0)
            {
                day = EQMain_atoi_guess(tok);
                tok = EQMain_strtok_guess(NULL, g_date_delims_guess);
                if (tok != NULL && strlen(tok) == 4 && EQMain_isdigit_guess((int)tok[0]) != 0 &&
                    EQMain_isdigit_guess((int)tok[1]) != 0 && EQMain_isdigit_guess((int)tok[2]) != 0 &&
                    EQMain_isdigit_guess((int)tok[3]) != 0)
                {
                    year = EQMain_atoi_guess(tok);
                    if (month != 0 && month <= 12 && day != 0 && day <= 31 && year >= 1900 && year <= 2100)
                        return 1;
                }
            }
        }
    }
    return 0;
}

// EQMain_StartScreen_guess's callees, globals and string data (eqmain.dll addresses; the strings are the start screen's).
// Two of the call sites push more dwords than Ghidra's prototypes show: ResolveText two after its four arguments,
// SetWidgetText two after its three. The extra slots are kept so the stack matches the original.
#define EQMain_LoadBitmap_guess(obj, name, n)            EQ_CDECL(void, 0x10007BF0, (int, char *, int))(obj, name, n)
#define EQMain_CreateWidget_guess(obj, a, b, c, d)       EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))(obj, a, b, c, d)
#define EQMain_ResolveText_guess(obj, a, b, s, x, y)     EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int))(obj, a, b, s, x, y)
#define EQMain_SetWidgetText_guess(w, a, s, x, y)        EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))(w, a, s, x, y)
#define EQMain_Tick_guess()                              EQ_STDCALL(void, 0x10003A00, (void))()
#define EQMain_CleanupSetup_guess(ecx)                       EQ_METHOD(0, void, 0x100216F0, (int), ecx)()
#define EQMain_GetCommand_guess()                        EQ_STDCALL(unsigned int, 0x1001A210, (void))()
#define EQMain_ResetInput_guess(s)                       EQ_CDECL(int, 0x100212E0, (char *))(s)
#define EQMain_OpenFile_guess(name, mode)                EQ_CDECL(void *, 0x1003407F, (char *, char *))(name, mode)
#define EQMain_NameRecord_guess(s)                       EQ_CDECL(int *, 0x100211B0, (char *))(s)
#define EQMain_fwrite_guess(p, size, count, f)           EQ_CDECL(size_t, 0x10034169, (void *, size_t, size_t, void *))(p, size, count, f)
#define EQMain_fclose_guess(f)                           EQ_CDECL(int, 0x100343B9, (void *))(f)
#define EQMain_PeekMsg_guess()                           EQ_STDCALL(int *, 0x10003BD0, (void))()
#define EQMain_StrFind_guess(a, b)                       EQ_CDECL(int, 0x1001A080, (char *, char *))(a, b)
#define EQMain_DrawText_guess(obj, buf, fmt, x)          EQ_CDECL(char, 0x10001FF0, (int, char *, char *, char *))(obj, buf, fmt, x)
#define EQMain_free_guess(p)                             EQ_CDECL(void, 0x10033568, (void *))(p)
#define EQMain_CloseScreen_guess(obj)                    EQ_CDECL(void, 0x10004E10, (int))(obj)
#define EQMain_EndScreen_guess()                         EQ_STDCALL(void, 0x100039E0, (void))()
#define g_eqmain_ceb8_guess                             EQ_GLOBAL(int, 0x1004CEB8)
#define g_eqmain_busy_guess                            EQ_GLOBAL(char, 0x1004CE78)
#define g_eqmain_login_code_guess                           EQ_GLOBAL(short, 0x1004814C)
#define g_eqmain_flag_5b34_guess                                EQ_GLOBAL(char, 0x100D5B34)
#define g_ptr_10055824_guess                             EQ_GLOBAL(char *, 0x10055824)
#define g_eqmain_text_5830_guess                             EQ_GLOBAL(char *, 0x10055830)
#define g_str_888_guess                             EQ_GLOBAL(char *, 0x10055888)
#define g_ptr_10055890_guess                             EQ_GLOBAL(char *, 0x10055890)
#define g_ptr_10055894_guess                             EQ_GLOBAL(char *, 0x10055894)
#define g_ptr_10055898_guess                             EQ_GLOBAL(char *, 0x10055898)
#define g_str_10047f64_guess                             EQ_ARRAY(char, 0x10047F64)
#define g_eqmain_equname_guess                             EQ_ARRAY(char, 0x10047F68)
#define g_str_10047f74_guess                             EQ_ARRAY(char, 0x10047F74)
#define g_str_10047f78_guess                             EQ_ARRAY(char, 0x10047F78)
#define g_eqmain_evstart_bmp_guess                             EQ_ARRAY(char, 0x10047F7C)
#define g_eqmain_login_pin_buf_guess                             EQ_ARRAY(char, 0x1012565C)
#define g_eqmain_login_name_buf_guess                             EQ_ARRAY(char, 0x10104CD8)
#define g_eqmain_copybuf_guess                             EQ_ARRAY(char, 0x100F4C10)

// FUNCTION: EQMAIN 0x10010FF0
// Front-end menu screen with four buttons; loops until a button, ESC or a command picks the next state code
// __stdcall, no arguments (callee pops 0). Returns a short (AX): local_14, the code the screen ended with.
// Draws EQVstart.bmp and four widgets, then loops: a command from EQMain_GetCommand_guess (0x42 takes the stored
// g_eqmain_login_code_guess when it is 9, 10 or 11; 4 and 0x51 write equname.txt; 2 resets the input) or a message from
// EQMain_PeekMsg_guess (*msg == 0 is a click on a widget, *msg == 4 with the byte at +8 == 0x1b is ESC) until done.
short __stdcall EQMain_StartScreen_guess(void)
{
    bool done = false;
    char busy = 0;
    short local_14;
    short storedCode;
    int button1;
    int button2;
    int button3;
    int button4;
    unsigned int length;
    int command;
    int ret;
    int *_Memory;
    int *_Str;
    void *_File;
    int pHVar2;

    EQMain_LoadBitmap_guess(g_eqmain_ceb8_guess, g_eqmain_evstart_bmp_guess, 0x10);
    button1 = EQMain_CreateWidget_guess(g_eqmain_ceb8_guess, 400, 0xd2, 0, 2);
    button2 = EQMain_CreateWidget_guess(g_eqmain_ceb8_guess, 400, 0xff, 0, 2);
    button3 = EQMain_CreateWidget_guess(g_eqmain_ceb8_guess, 400, 300, 0, 2);
    button4 = EQMain_CreateWidget_guess(g_eqmain_ceb8_guess, 400, 0x159, 0, 2);
    ret = EQMain_ResolveText_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_ptr_10055894_guess, 0x19, 0);
    ret = EQMain_ResolveText_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_ptr_10055898_guess, ret, 0);
    ret = EQMain_ResolveText_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_ptr_10055890_guess, ret, 0);
    ret = EQMain_ResolveText_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_5830_guess, ret, 0);
    EQMain_SetWidgetText_guess(button2, 0, g_ptr_10055894_guess, ret, 0);
    EQMain_SetWidgetText_guess(button3, 0, g_ptr_10055898_guess, ret, 0);
    EQMain_SetWidgetText_guess(button1, 0, g_ptr_10055890_guess, ret, 0);
    EQMain_SetWidgetText_guess(button4, 0, g_eqmain_text_5830_guess, ret, 0);
    g_eqmain_busy_guess = 1;
    EQMain_Tick_guess();
    EQMain_CleanupSetup_guess(0);     // ECX is undefined here in the original (extraout_ECX)
    do {
        length = EQMain_GetCommand_guess();
        command = (short)length;
        if (command > 0x42)
            goto highCommand;
        if (command == 0x42)
            goto takeStoredCode;
        if (command == 2)
            goto clearBusy;
        if (command == 4)
            goto writeFile;
        if (command == 0x1b)
        {
            local_14 = 0xc;
            done = true;
        }
        goto pollMessage;

takeStoredCode:
        storedCode = g_eqmain_login_code_guess;
        if (storedCode == 9 || storedCode == 10 || storedCode == 0xb)
            goto useStoredCode;
        local_14 = 7;
        done = true;
        goto pollMessage;

useStoredCode:
        local_14 = storedCode;
        done = true;
        goto pollMessage;

highCommand:
        if (command == 0x51)
            goto writeFile;
        if (command != 0x10000003)     // never equal: command is a sign-extended 16-bit value (kept from the asm)
            goto pollMessage;

clearBusy:
        EQMain_CleanupSetup_guess(0);     // ECX undefined here in the original too
        EQMain_ResetInput_guess(g_str_10047f78_guess);
        busy = 0;
        goto pollMessage;

writeFile:
        _File = EQMain_OpenFile_guess(g_eqmain_equname_guess, g_str_10047f74_guess);
        if (_File != NULL)
        {
            _Str = EQMain_NameRecord_guess(g_eqmain_login_name_buf_guess);
            EQMain_fwrite_guess(_Str, 0x14, 1, _File);
            EQMain_fclose_guess(_File);
        }
        done = true;
        local_14 = g_eqmain_login_code_guess;

pollMessage:
        _Memory = EQMain_PeekMsg_guess();
        if (_Memory == NULL)
            goto checkDone;
        if (*_Memory == 0)
            goto widgetClick;
        if (*_Memory != 4)
            goto freeMessage;
        if ((char)_Memory[2] != 0x1b)
            goto freeMessage;
        EQMain_Tick_guess();
        goto setResult7;

widgetClick:
        if (_Memory[2] == button1 && busy == 0)
        {
            g_eqmain_flag_5b34_guess = busy;
            EQMain_ResetInput_guess(g_str_10047f78_guess);
            if ((char)EQMain_StrFind_guess(g_str_10047f64_guess, g_str_10047f64_guess) != 0)
                busy = 1;
        }
        pHVar2 = _Memory[2];
        if (pHVar2 == button2 && busy == 0)
            g_eqmain_login_code_guess = 9;
        else if (pHVar2 == button3 && busy == 0)
            g_eqmain_login_code_guess = 10;
        else
            goto otherWidget;
        length = strlen(g_eqmain_login_name_buf_guess);
        if (length == 0)
            goto emptyText;
        EQMain_CleanupSetup_guess(length);
        EQMain_ResetInput_guess(g_str_10047f78_guess);
        if ((char)EQMain_StrFind_guess(g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess) != 0)
            busy = 1;
        goto freeMessage;

emptyText:
        local_14 = 0;
        goto markDone;

otherWidget:
        if (pHVar2 != 0)
            goto checkButton4;
        if (busy != 0)
            goto checkButton4;
        strcpy(g_eqmain_copybuf_guess, g_ptr_10055824_guess);
        EQMain_DrawText_guess(g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, 0);
        local_14 = 1;
        goto markDone;

checkButton4:
        if (pHVar2 != button4)
            goto freeMessage;

setResult7:
        local_14 = 7;

markDone:
        done = true;

freeMessage:
        EQMain_free_guess(_Memory);

checkDone:
        if (done)
        {
            g_eqmain_busy_guess = 0;
            EQMain_CloseScreen_guess(g_eqmain_ceb8_guess);
            EQMain_EndScreen_guess();
            return local_14;
        }
    } while (1);
}

// EQMain_BuildPieces_guess's callees, globals and string data (eqmain.dll addresses; the strings A..H are the pieces it tests).
#define EQMain_TrimPiece_guess(s)        EQ_CDECL(char, 0x10023C10, (char *))(s)
#define EQMain_IsValidDate_call_guess(s) EQ_CDECL(char, 0x10010E40, (char *))(s)
#define g_eqmain_flag_5b34_guess                EQ_GLOBAL(char, 0x100D5B34)
#define g_eqmain_start_field_0_guess             EQ_ARRAY(char, 0x1012B945)
#define g_eqmain_start_field_1_guess             EQ_ARRAY(char, 0x1012B965)
#define g_eqmain_start_field_3_guess             EQ_ARRAY(char, 0x1012B989)
#define g_eqmain_start_field_4_guess             EQ_ARRAY(char, 0x1012BA09)
#define g_eqmain_start_field_5_guess             EQ_ARRAY(char, 0x1012BA49)
#define g_eqmain_start_field_6_guess             EQ_ARRAY(char, 0x1012BA89)
#define g_eqmain_start_field_7_guess             EQ_ARRAY(char, 0x1012BAA9)
#define g_eqmain_start_field_10_guess             EQ_ARRAY(char, 0x1012BB19)
#define g_eqmain_start_field_12_guess             EQ_ARRAY(char, 0x1012BC93)
#define g_eqmain_start_const_8c_guess             EQ_ARRAY(char, 0x10047F8C)
#define g_eqmain_copybuf_guess             EQ_ARRAY(char, 0x100F4C10)
#define g_ptr_10055790_guess             EQ_GLOBAL(char *, 0x10055790)
#define g_ptr_10055794_guess             EQ_GLOBAL(char *, 0x10055794)
#define g_ptr_10055798_guess             EQ_GLOBAL(char *, 0x10055798)
#define g_ptr_1005579c_guess             EQ_GLOBAL(char *, 0x1005579C)
#define g_ptr_100557d0_guess             EQ_GLOBAL(char *, 0x100557D0)

// FUNCTION: EQMAIN 0x100113C0
// Checks a set of front-end entries, including a date, before accepting it.
// __stdcall, no arguments (callee pops 0 bytes; with no arguments it is the same code as __cdecl). Returns a char (AL).
// Returns 1 at once when the flag at 0x100D5B34 is set. Else it passes the pieces A, B, C, D, F and H to
// EQMain_TrimPiece_guess, strcpy's the string at 0x10055790 into the buffer at 0x100F4C10, then appends one suffix:
// A or B empty -> 0x10055794; C..G empty -> 0x10055798; H empty -> 0x1005579C; a date at 0x1012BC93 that
// EQMain_IsValidDate_call_guess rejects -> 0x100557D0. A valid date: G (0x1012BAA9) differing from the literal at
// 0x10047F8C returns 1; a match appends 0x10055798.
char __stdcall EQMain_BuildPieces_guess(void)
{
    if (g_eqmain_flag_5b34_guess == 0)
    {
        EQMain_TrimPiece_guess(g_eqmain_start_field_0_guess);
        EQMain_TrimPiece_guess(g_eqmain_start_field_1_guess);
        EQMain_TrimPiece_guess(g_eqmain_start_field_3_guess);
        EQMain_TrimPiece_guess(g_eqmain_start_field_4_guess);
        EQMain_TrimPiece_guess(g_eqmain_start_field_6_guess);
        EQMain_TrimPiece_guess(g_eqmain_start_field_10_guess);
        strcpy(g_eqmain_copybuf_guess, g_ptr_10055790_guess);
        if (strlen(g_eqmain_start_field_0_guess) != 0)
        {
            if (strlen(g_eqmain_start_field_1_guess) != 0)
            {
                if (strlen(g_eqmain_start_field_3_guess) != 0)
                {
                    if (strlen(g_eqmain_start_field_4_guess) != 0)
                    {
                        if (strlen(g_eqmain_start_field_5_guess) != 0)
                        {
                            if (strlen(g_eqmain_start_field_6_guess) != 0)
                            {
                                if (strlen(g_eqmain_start_field_7_guess) != 0)
                                {
                                    if (strlen(g_eqmain_start_field_10_guess) == 0)
                                    {
                                        strcat(g_eqmain_copybuf_guess, g_ptr_1005579c_guess);
                                        return 0;
                                    }
                                    if (EQMain_IsValidDate_call_guess(g_eqmain_start_field_12_guess) == 0)
                                    {
                                        strcat(g_eqmain_copybuf_guess, g_ptr_100557d0_guess);
                                        return 0;
                                    }
                                    if (strcmp(g_eqmain_start_field_7_guess, g_eqmain_start_const_8c_guess) != 0)
                                        return 1;
                                }
                            }
                        }
                    }
                }
                strcat(g_eqmain_copybuf_guess, g_ptr_10055798_guess);
                return 0;
            }
        }
        strcat(g_eqmain_copybuf_guess, g_ptr_10055794_guess);
        return 0;
    }
    return 1;
}

#define EQMain_isalpha_guess(c)          EQ_CDECL(int, 0x1003380B, (int))(c)
#define g_eqmain_buf_1012b920_guess             EQ_ARRAY(char, 0x1012B920)
#define g_eqmain_buf_1012bf6f_guess             EQ_ARRAY(char, 0x1012BF6F)
#define g_eqmain_buf_1012bf83_guess             EQ_ARRAY(char, 0x1012BF83)
#define g_eqmain_buf_1004d1b0_guess             EQ_ARRAY(char, 0x1004D1B0)
#define g_ptr_100557a0_guess             EQ_GLOBAL(char *, 0x100557A0)
#define g_ptr_100557a4_guess             EQ_GLOBAL(char *, 0x100557A4)
#define g_ptr_100557a8_guess             EQ_GLOBAL(char *, 0x100557A8)
#define g_ptr_100557ac_guess             EQ_GLOBAL(char *, 0x100557AC)

// FUNCTION: EQMAIN 0x100115F0
// Confirms the final form entry matches its reference after checking the entries.
// __stdcall, no arguments (callee pops 0 bytes; with no arguments it is the same code as __cdecl). Returns a char (AL).
// Returns 1 at once when the flag at 0x100D5B34 is set. Else it passes the pieces B (0x1012B920), C (0x1012BF6F), D
// (0x1012BF83) and the string at 0x1004D1B0 to EQMain_TrimPiece_guess, strcpy's the string at 0x10055790 into the buffer
// at 0x100F4C10, then checks B, C and D in turn: a piece under 4 characters, or a character that is not a letter, a
// digit, '_' (and '-' for B), appends 0x100557A0 (B), 0x100557AC (C) or 0x100557A4 (D) and returns 0. D is checked only
// while the flag is still clear. Then D is compared with the string at 0x1004D1B0: a difference appends 0x100557A8 and
// returns 0; equal returns 1.
char __stdcall EQMain_ValidatePieces_guess(void)
{
    int i;
    char c;

    if (g_eqmain_flag_5b34_guess == 0)
    {
        EQMain_TrimPiece_guess(g_eqmain_buf_1012b920_guess);
        EQMain_TrimPiece_guess(g_eqmain_buf_1012bf6f_guess);
        EQMain_TrimPiece_guess(g_eqmain_buf_1012bf83_guess);
        EQMain_TrimPiece_guess(g_eqmain_buf_1004d1b0_guess);
        strcpy(g_eqmain_copybuf_guess, g_ptr_10055790_guess);
        if (strlen(g_eqmain_buf_1012b920_guess) < 4)
        {
firstPieceInvalid:
            strcat(g_eqmain_copybuf_guess, g_ptr_100557a0_guess);
            return 0;
        }
        for (i = 0; i < strlen(g_eqmain_buf_1012b920_guess); i++)
        {
            c = g_eqmain_buf_1012b920_guess[i];
            if (EQMain_isalpha_guess(c) == 0)
            {
                if (EQMain_isdigit_guess(c) == 0 && c != '_' && c != '-')
                    goto firstPieceInvalid;
            }
        }
        strcpy(g_eqmain_copybuf_guess, g_ptr_10055790_guess);
        if (strlen(g_eqmain_buf_1012bf6f_guess) < 4)
        {
secondPieceInvalid:
            strcat(g_eqmain_copybuf_guess, g_ptr_100557ac_guess);
            return 0;
        }
        for (i = 0; i < strlen(g_eqmain_buf_1012bf6f_guess); i++)
        {
            c = g_eqmain_buf_1012bf6f_guess[i];
            if (EQMain_isalpha_guess(c) == 0)
            {
                if (EQMain_isdigit_guess(c) == 0 && c != '_')
                    goto secondPieceInvalid;
            }
        }
        if (g_eqmain_flag_5b34_guess == 0)
        {
            strcpy(g_eqmain_copybuf_guess, g_ptr_10055790_guess);
            if (strlen(g_eqmain_buf_1012bf83_guess) < 4)
            {
thirdPieceInvalid:
                strcat(g_eqmain_copybuf_guess, g_ptr_100557a4_guess);
                return 0;
            }
            for (i = 0; i < strlen(g_eqmain_buf_1012bf83_guess); i++)
            {
                c = g_eqmain_buf_1012bf83_guess[i];
                if (EQMain_isalpha_guess(c) == 0)
                {
                    if (EQMain_isdigit_guess(c) == 0 && c != '_')
                        goto thirdPieceInvalid;
                }
            }
        }
        if (strcmp(g_eqmain_buf_1012bf83_guess, g_eqmain_buf_1004d1b0_guess) != 0)
        {
            strcat(g_eqmain_copybuf_guess, g_ptr_100557a8_guess);
            return 0;
        }
    }
    return 1;
}

#define g_eqmain_ceb8_guess             EQ_GLOBAL(int, 0x1004CEB8)

// FUNCTION: EQMAIN 0x100118F0
// Creates a front-end text control and can seed it with an initial string.
// __cdecl, nine arguments, no return value (EAX holds the last call's result at the RET, so it is not returned).
// Passes the global at 0x1004CEB8, value, ptr, 0x21A3C, text and two zeros to 0x100082C0 (the original
// pushes the zeros and never pops them: the frame's ADD ESP covers them), then value2, y, value4 to 0x10008800.
// Calls 0x10008460 with the global, value2 + 2, y, value4, 0x10, count, 0, 0, 0x22D69, 0 and stores the
// result in *resultPtr. When text2 is not null, passes the result and text2 to 0x10008970.
void __cdecl EQMain_CreateLabel_guess(unsigned long value, void *labelY, int value2, int y, int value4,
                                      const char *text, void **resultPtr, size_t count, char *text2)
{
    void *result;

    EQ_CDECL(void *, 0x100082C0, (int, unsigned long, void *, int, const char *, int, int))(
        g_eqmain_ceb8_guess, value, labelY, 0x21A3C, text, 0, 0);
    EQ_CDECL(void, 0x10008800, (int, int, int))(value2, y, value4);
    result = EQ_CDECL(void *, 0x10008460, (int, int, int, int, int, size_t, int, int, unsigned long, unsigned long))(
        g_eqmain_ceb8_guess, value2 + 2, y, value4, 0x10, count, 0, 0, 0x22D69, 0);
    *resultPtr = result;
    if (text2 != 0)
    {
        EQ_CDECL(void, 0x10008970, (void *, char *))(result, text2);
    }
    return;
}

// FUNCTION: EQMAIN 0x10011970
// Creates the preset-layout variant of a front-end text control.
// __cdecl, six arguments, no return value (EAX holds the last call's result at the RET, so it is not returned).
// Passes 0x37, ptr, value2, ptr, 0x14A, text, resultPtr, count and text2 to 0x100118F0 (the
// CreateLabel above) and pops the nine arguments afterwards.
void __cdecl EQMain_CreateLabelFixed_guess(int value2, void *y, const char *text, void **resultPtr,
                                           size_t count, char *text2)
{
    EQ_CDECL(void, 0x100118F0, (unsigned long, void *, int, void *, int, const char *, void **, size_t, char *))(
        0x37, y, value2, y, 0x14A, text, resultPtr, count, text2);
    return;
}
