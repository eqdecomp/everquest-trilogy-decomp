// unit_100248A0.cpp - eqmain.dll, the address run 0x100248A0..0x10025950 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "trilogy/eqaddr.h"

struct SystemInfo_guess
{
    unsigned char unknown_00[0x18];
    unsigned long processor_type;
    unsigned long allocation_granularity;
    unsigned short processor_level;
    unsigned short processor_revision;
};

struct HardwareReportFrame_guess
{
    char vendor_text[128];
    unsigned int hardware_values[4];
    SystemInfo_guess system_info;
    volatile unsigned int unused_hardware_values[3];
    unsigned char alignment_gap[4];
    char processor_text[20];
    volatile unsigned int processor_values[4];
};

typedef void (__stdcall *EqmainGetSystemInfoProc_guess)(SystemInfo_guess *);

// eqmain.dll's own globals (this image's addresses, not eqgame.exe's).
#define g_eqmain_state_guess        EQ_GLOBAL(unsigned int, 0x1012B918)
#define g_eqmain_word_guess         EQ_GLOBAL(unsigned short, 0x10125914)
#define g_sendbuf_10125914_guess     EQ_ARRAY(char, 0x10125914)
#define g_eqmain_state_b91c_guess   EQ_GLOBAL(unsigned int, 0x1012B91C)
#define g_eqmain_value_b914_guess   EQ_GLOBAL(unsigned int, 0x1012B914)
#define g_eqmain_obj_c774_guess     EQ_GLOBAL(Slot0Object_guess *, 0x1012C774)
#define g_eqmain_text_guess         EQ_ARRAY(char, 0x10127914)
#define g_eqmain_flag_1d20_guess    EQ_GLOBAL(unsigned char, 0x100F1D20)
// The 0x2CC-byte block at 0x1012C220 (g_eqmain_block_c220_guess; zeroed by EQMain_PollStatus_guess).
#define g_eqmain_block_c220_guess   EQ_ARRAY(unsigned char, 0x1012C220)

// eqmain.dll's own CRT functions (this image's addresses), called by address like the client's own functions.
#define eqmain_isdigit_guess        EQ_CDECL(int, 0x10033861, (int))
#define eqmain_atoi_guess           EQ_CDECL(int, 0x1003507E, (char *))
#define eqmain_strstr_guess         EQ_CDECL(char *, 0x10035FD0, (char *, char *))
#define eqmain_fn_10033670_guess         EQ_CDECL(char *, 0x10033670, (char *, int))

#define g_eqmain_cycle_text_guess   EQ_ARRAY(char, 0x10125850)
#define g_eqmain_cycle_value_guess  EQ_GLOBAL(unsigned int, 0x101258A0)

#define g_eqmain_flag_5900_guess    EQ_GLOBAL(unsigned int, 0x10125900)
#define g_eqmain_buffer_5904_guess  EQ_GLOBAL(void *, 0x10125904)
#define eqmain_free_guess           EQ_CDECL(void, 0x10033568, (void *))
// Frees g_eqmain_buffer_5904_guess and clears it (eqmain.dll's FreeBuffer_guess, called by address).
#define eqmain_clear_5904_guess     EQ_STDCALL(void, 0x10024CB0, (void))
// Returns the total physical memory in bytes (eqmain.dll's GetTotalPhysicalMemory_guess, called by address).
#define eqmain_total_phys_guess     EQ_STDCALL(unsigned long, 0x10024880, (void))

// eqmain.dll's tag reader and text decoder (NextWord_guess and lzo1x_decompress_failed_guess, called by address). The reader returns the
// tag (its low 16 bits are tested) and moves the cursor by 2; the decoder gets the text at the cursor, the whole tag
// and the buffer, and its result is not used.
#define eqmain_next_tag_guess       EQ_STDCALL(unsigned int, 0x100250F0, (void))
#define eqmain_decode_text_guess    EQ_CDECL(bool, 0x100284B0, (unsigned short *, unsigned int, unsigned char *))
// The 8 KB string buffer at 0x10129914, just below the text base at 0x1012B914 (not NextString_guess's buffer).
#define g_eqmain_text_9914_guess    EQ_ARRAY(char, 0x10129914)

// FUNCTION: EQMAIN 0x100248A0
// Classifies the machine's memory into one of several front-end tiers.
// __stdcall, returns short (callers read AX), callee pops 0. Takes the total physical memory in megabytes (>> 20) and
// returns a tier: 1 below 15, 2 below 23, 3 below 31, 4 below 47, 5 below 63, 6 below 79, 7 below 95, 8 below 100,
// else 9. The shift leaves at most 12 bits, so every compare is exact.
short __stdcall GetMemoryTier_guess(void)
{
    unsigned long SVar1;
    unsigned int memoryMb;

    SVar1 = eqmain_total_phys_guess();
    memoryMb = SVar1 >> 0x14;
    if (memoryMb < 0xf)
    {
        return 1;
    }
    if (memoryMb < 0x17)
    {
        return 2;
    }
    if (memoryMb < 0x1f)
    {
        return 3;
    }
    if (memoryMb < 0x2f)
    {
        return 4;
    }
    if (memoryMb < 0x3f)
    {
        return 5;
    }
    if (memoryMb < 0x4f)
    {
        return 6;
    }
    if (memoryMb < 0x5f)
    {
        return 7;
    }
    return 9 + (memoryMb < 100 ? -1 : 0);
}

#define eqmain_set_report_source_guess     EQ_CDECL(void, 0x100250A0, (unsigned int))
#define eqmain_fn_10025150_guess      EQ_STDCALL(unsigned char *, 0x10025150, (void))
#define eqmain_convert_report_token_guess  EQ_CDECL(char *, 0x10021110, (unsigned char *))
#define g_eqmain_get_system_info_guess     EQ_GLOBAL(EqmainGetSystemInfoProc_guess, 0x100400D4)
#define eqmain_get_hardware_values_guess   EQ_CDECL(unsigned int *, 0x100272A0, (unsigned int *, int))
#define eqmain_set_report_opcode_guess     EQ_CDECL(void, 0x10024F90, (unsigned short))
#define eqmain_start_fn_100211b0_guess  EQ_CDECL(char *, 0x100211B0, (char *))
#define eqmain_fn_10024ff0_guess  EQ_CDECL(void, 0x10024FF0, (char *))
#define eqmain_append_report_value_guess   EQ_CDECL(void, 0x10024FD0, (unsigned int))
#define eqmain_enumerate_report_devices_guess EQ_STDCALL(void, 0x10024840, (void))
#define eqmain_finish_hardware_report_guess EQ_STDCALL(void, 0x100213F0, (void))

// FUNCTION: EQMAIN 0x10024900
// Assembles a hardware report with processor, memory, and display-device details.
void __cdecl SendHardwareReport_guess(unsigned int messageData)
{
    HardwareReportFrame_guess frame;
    unsigned int *hardware_values;
    unsigned int processor_value;
    unsigned int total_memory;
    char *decoded_text;

    decoded_text = (eqmain_set_report_source_guess(messageData),
        eqmain_convert_report_token_guess(eqmain_fn_10025150_guess()));
    strcpy(frame.processor_text, decoded_text);
    decoded_text = eqmain_convert_report_token_guess(eqmain_fn_10025150_guess());
    strcpy(frame.vendor_text, decoded_text);

    processor_value = 0;
    frame.processor_values[0] = 0;
    frame.processor_values[1] = 0;
    frame.processor_values[2] = 0;
    frame.processor_values[3] = 0;
    g_eqmain_get_system_info_guess(&frame.system_info);
    if (frame.system_info.processor_type < 0x24a)
    {
        if (frame.system_info.processor_type == 0x182)
        {
            processor_value = 0x300;
        }
        else if (frame.system_info.processor_type == 0x1e6)
        {
            processor_value = 0x400;
        }
    }
    else
    {
        __asm
        {
            lea esi, [ebp - 10h]
            mov eax, 0
            __emit 0x0f
            __emit 0xa2
            __emit 0x89
            __emit 0x1e
            mov [esi + 4], edx
            mov [esi + 8], ecx
            mov eax, 1
            __emit 0x0f
            __emit 0xa2
            mov processor_value, eax
            mov [esi + 0ch], edx
        }
    }

    hardware_values = eqmain_get_hardware_values_guess(frame.hardware_values, 0);
    frame.unused_hardware_values[0] = hardware_values[0];
    frame.unused_hardware_values[1] = hardware_values[1];
    frame.unused_hardware_values[2] = hardware_values[2];
    total_memory = hardware_values[3];
    eqmain_set_report_opcode_guess(0x27);
    decoded_text = eqmain_start_fn_100211b0_guess(frame.processor_text);
    eqmain_fn_10024ff0_guess(decoded_text);
    decoded_text = eqmain_start_fn_100211b0_guess(frame.vendor_text);
    eqmain_fn_10024ff0_guess(decoded_text);
    eqmain_append_report_value_guess(processor_value);
    eqmain_append_report_value_guess(frame.processor_values[3]);
    eqmain_append_report_value_guess(frame.processor_values[0]);
    eqmain_append_report_value_guess(frame.processor_values[1]);
    eqmain_append_report_value_guess(frame.processor_values[2]);
    eqmain_append_report_value_guess(total_memory);
    eqmain_append_report_value_guess(eqmain_total_phys_guess());
    eqmain_enumerate_report_devices_guess();
    eqmain_finish_hardware_report_guess();
    return;
}

// This function's own addresses (eqmain.dll): its globals and strings, the CRT calls it makes, and FreeBuffer_guess,
// FindNamedBlockBody_guess and CopyQuotedText_guess. Those three are thiscalls (ECX = this); the two with stack arguments take the
// EQ_THISCALL dummy in EDX.
#define g_eqmain_eqhost_file_guess      EQ_GLOBAL(FILE *, 0x10125900)
#define g_eqmain_fopen_mode_guess      EQ_ARRAY(char, 0x100480A4)
#define g_eqmain_str_eqhost_guess       EQ_ARRAY(char, 0x10048744)
#define g_eqmain_str_registration_guess EQ_ARRAY(char, 0x1004872C)
#define g_eqmain_str_login_guess        EQ_ARRAY(char, 0x1004871C)
#define g_eqmain_delim_8590_guess       EQ_ARRAY(char, 0x10048590)
#define g_eqmain_delim_5910_guess       EQ_ARRAY(char, 0x10125910)

#define eqmain_fn_10035b24_guess               EQ_CDECL(time_t, 0x10035B24, (time_t *))
#define eqmain_srand_guess              EQ_CDECL(void, 0x100360B2, (unsigned long))
#define eqmain_rand_guess               EQ_CDECL(int, 0x100360BF, (void))
#define eqmain_fopen_guess              EQ_CDECL(FILE *, 0x1003407F, (char *, char *))
#define eqmain_fseek_guess              EQ_CDECL(int, 0x100345ED, (FILE *, long, int))
#define eqmain_ftell_guess              EQ_CDECL(long, 0x10033E8F, (FILE *))
#define eqmain_rewind_guess             EQ_CDECL(void, 0x100346A6, (FILE *))
#define eqmain_malloc_guess             EQ_CDECL(void *, 0x1003342E, (unsigned int))
#define eqmain_fclose_guess             EQ_CDECL(int, 0x100343B9, (FILE *))
#define eqmain_fgetc_guess              EQ_CDECL(int, 0x10035F9A, (FILE *))
#define eqmain_fn_1003396b_guess             EQ_CDECL(char *, 0x1003396B, (char *))
#define eqmain_strtok_guess             EQ_CDECL(char *, 0x10033996, (char *, char *))
#define eqmain_name_from_line_guess     EQ_CDECL(char *, 0x10021110, (char *))
#define eqmain_free_buffer_this_guess(object) EQ_METHOD(0, void, 0x10024CB0, (void *), (object))
#define eqmain_find_section_guess(object) EQ_METHOD(1, char *, 0x10024CD0, (void *, char *), (object))
#define eqmain_next_line_guess(object) EQ_METHOD(2, unsigned int, 0x10024D30, (void *, char *, char *), (object))

// The object the loader fills (this = ECX). No header in include/ matches it, so this is a file-local stand-in. The
// layout is proposed from the displacements the function uses (0xC, 0x10, 0x14, 0x18, 0x1C, 0x9C, 0x11C, 0x19C).
/* EqHostList_guess: the actor layout is EqHostList_guess (generated: eqmain/layout/eq_host_list.h); the old local view's members map by offset. */
#include "eqmain/layout/eq_host_list.h"

// FUNCTION: EQMAIN 0x10024A70
// Reads eqhost.txt into the registration and login server lists (host, port) and picks a random server of each
// __thiscall in the original (this in ECX, no stack arguments, callee pops 0), so __fastcall with one parameter here.
// Returns FILE * in EAX. Seeds rand() from time(), clears the text buffer, opens eqhost.txt and copies it into the
// buffer without its '#' lines. Then reads [Registration Servers] and [Login Servers]: a name and a port per line, no
// bounds check on the 32-entry arrays. Each list gets a random index; the return value is the login draw divided by
// the login count. Returns 0 or -1 when fopen fails, the fclose result when malloc fails, and 0 when the login section
// is missing (the file global is then left as it is).
// @early-stop: 4 xor-edx dummies (EQ_THISCALL EDX); shared exit stores g after 2 pops; login draw uses 2 idivs.
FILE * __fastcall LoadEqHost_guess(EqHostList_guess *self)
{
    FILE *_File;
    char lineBuf[1024];
    unsigned char fileFlags;
    char ch;
    int getcResult;
    int tmp;
    unsigned int bufferSize;
    long fileLength;
    char *cursor;
    char *pcVar6;
    time_t now;

    now = eqmain_fn_10035b24_guess(NULL);
    eqmain_srand_guess((unsigned long)now);
    eqmain_free_buffer_this_guess(self)();
    _File = eqmain_fopen_guess(g_eqmain_str_eqhost_guess, g_eqmain_fopen_mode_guess);
    g_eqmain_eqhost_file_guess = _File;
    if (_File != NULL && _File != (FILE *)-1)
    {
        eqmain_fseek_guess(_File, 0, 2);
        fileLength = eqmain_ftell_guess(g_eqmain_eqhost_file_guess);
        bufferSize = fileLength + 2;
        eqmain_rewind_guess(g_eqmain_eqhost_file_guess);
        g_eqmain_buffer_5904_guess = eqmain_malloc_guess(bufferSize);
        if (g_eqmain_buffer_5904_guess == NULL)
        {
            _File = (FILE *)eqmain_fclose_guess(g_eqmain_eqhost_file_guess);
        }
        else
        {
            memset(g_eqmain_buffer_5904_guess, 0, bufferSize);
            fileFlags = (unsigned char)g_eqmain_eqhost_file_guess->_flag;
            tmp = 0;
            while ((fileFlags & 0x10) == 0)
            {
                getcResult = eqmain_fgetc_guess(g_eqmain_eqhost_file_guess);
                ch = (char)getcResult;
                if (ch == '#')
                {
                    while (ch != '\n')
                    {
                        if ((g_eqmain_eqhost_file_guess->_flag & 0x10) != 0)
                        {
                            break;
                        }
                        getcResult = eqmain_fgetc_guess(g_eqmain_eqhost_file_guess);
                        ch = (char)getcResult;
                    }
                }
                *((char *)g_eqmain_buffer_5904_guess + tmp) = ch;
                tmp = tmp + 1;
                fileFlags = (unsigned char)g_eqmain_eqhost_file_guess->_flag;
            }
            eqmain_fclose_guess(g_eqmain_eqhost_file_guess);
            cursor = eqmain_find_section_guess(self)( g_eqmain_str_registration_guess);
            if (cursor != NULL)
            {
                while (cursor != NULL)
                {
                    cursor = (char *)eqmain_next_line_guess(self)( cursor, lineBuf);
                    if (cursor == NULL)
                    {
                        break;
                    }
                    if (strlen(lineBuf) != 0)
                    {
                        pcVar6 = eqmain_name_from_line_guess(lineBuf);
                        pcVar6 = eqmain_fn_1003396b_guess(pcVar6);
                        self->reg_name[self->reg_count] = pcVar6;
                        eqmain_strtok_guess(self->reg_name[self->reg_count], g_eqmain_delim_8590_guess);
                        pcVar6 = eqmain_strtok_guess(NULL, g_eqmain_delim_5910_guess);
                        self->reg_port_signed_guess[self->reg_count] = eqmain_atoi_guess(pcVar6);
                        self->reg_count = self->reg_count + 1;
                    }
                }
                tmp = eqmain_rand_guess();
                self->reg_index = tmp % self->reg_count;
            }
            cursor = eqmain_find_section_guess(self)( g_eqmain_str_login_guess);
            if (cursor != NULL)
            {
                while (cursor != NULL)
                {
                    cursor = (char *)eqmain_next_line_guess(self)( cursor, lineBuf);
                    if (cursor == NULL)
                    {
                        break;
                    }
                    if (strlen(lineBuf) != 0)
                    {
                        pcVar6 = eqmain_name_from_line_guess(lineBuf);
                        pcVar6 = eqmain_fn_1003396b_guess(pcVar6);
                        self->login_name[self->login_count] = pcVar6;
                        eqmain_strtok_guess(self->login_name[self->login_count], g_eqmain_delim_8590_guess);
                        pcVar6 = eqmain_strtok_guess(NULL, g_eqmain_delim_5910_guess);
                        self->login_port_signed_guess[self->login_count] = eqmain_atoi_guess(pcVar6);
                        self->login_count = self->login_count + 1;
                    }
                }
                tmp = eqmain_rand_guess();
                self->login_index = tmp % self->login_count;
                return (FILE *)(tmp / self->login_count);
            }
            else
            {
                return NULL;
            }
        }
    }
    g_eqmain_eqhost_file_guess = NULL;
    return _File;
}

// FUNCTION: EQMAIN 0x10024CB0
// Releases a shared front-end data buffer; its contents are unclear.
// __stdcall, void, no arguments. If the pointer g_eqmain_buffer_5904_guess is set, frees the block it points to and
// clears the pointer. The block's owner and size are unknown.
void __stdcall FreeBuffer_guess(void)
{
    if (g_eqmain_buffer_5904_guess != NULL)
    {
        eqmain_free_guess(g_eqmain_buffer_5904_guess);
        g_eqmain_buffer_5904_guess = NULL;
    }
}

// FUNCTION: EQMAIN 0x10024CD0
// Locates the body of a named login or registration settings block.
// __stdcall, returns char *, callee pops 4. Returns 0 while g_eqmain_flag_5900_guess is 0. Otherwise finds the name
// in the text at g_eqmain_buffer_5904_guess, the first '{' after it, and returns the first character after that brace
// which is not '\n', '\r' or ' ' (tabs are not skipped). Returns 0 when the name or the brace is missing; those two
// returns hand back bodyStart itself, which is already 0 in EAX.
char * __stdcall FindNamedBlockBody_guess(char *name)
{
    char nextChar;
    char *bodyStart;

    if (g_eqmain_flag_5900_guess == 0)
    {
        return NULL;
    }
    bodyStart = eqmain_strstr_guess((char *)g_eqmain_buffer_5904_guess, name);
    if (bodyStart == NULL)
    {
        return bodyStart;
    }
    bodyStart = eqmain_fn_10033670_guess(bodyStart, '{');
    if (bodyStart == NULL)
    {
        return bodyStart;
    }
    do
    {
        do
        {
            nextChar = bodyStart[1];
            bodyStart = bodyStart + 1;
        } while (nextChar == '\n');
    } while (nextChar == '\r' || nextChar == ' ');
    return bodyStart;
}

// FUNCTION: EQMAIN 0x10024D30
// Extracts a quoted value from front-end settings text.
// __stdcall, returns uint, two char pointers (callee pops 8). Scans `text` forward to the first '{', '}' or '"'.
// A '{' or '}' there returns 0. A '"' copies the text after it into `out` up to the next '"' (written as a NUL) or
// '}' (returns 0 at once, with no NUL). After the closing quote it returns the address past it, or 0 when that
// character is '}'. The scan has no bound: the text must hold one of the three.
unsigned int __stdcall CopyQuotedText_guess(char *text, char *out)
{
    char *dst;
    char c;

    dst = out;
    c = *text;
    while (c != '}' && c != '{' && c != '"')
    {
        c = text[1];
        text = text + 1;
    }
    if (*text++ == '"')
    {
        c = *text;
        while (c != '}')
        {
            *dst = c;
            dst = dst + 1;
            if (*text == '}' || *text == '"')
            {
                dst[-1] = '\0';
                return text[1] != '}' ? (unsigned int)(text + 1) : 0;
            }
            c = text[1];
            text = text + 1;
        }
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10024DA0
// Initializes an object used to load front-end host entries; its exact role is unclear.
// __thiscall (this in ECX, no other argument; written as __fastcall with one argument), returns this. Stores 0x10041F78
// at +0x00 (the vtable), zeroes the six dwords at +0x04..+0x18, then zeroes four blocks of 0x20 dwords at +0x1C, +0x11C,
// +0x9C and +0x19C, then calls 0x10024A70 (__fastcall, this in ECX; its result is not used).
unsigned int * __fastcall ZeroBlocksCtor_guess(unsigned int *self)
{
    self[0] = 0x10041F78;
    self[1] = 0;
    self[2] = 0;
    self[3] = 0;
    self[4] = 0;
    self[5] = 0;
    self[6] = 0;
    memset(self + 7, 0, 0x80);
    memset(self + 0x47, 0, 0x80);
    memset(self + 0x27, 0, 0x80);
    memset(self + 0x67, 0, 0x80);
    EQ_METHOD(0, void, 0x10024A70, (void *), self)();
    return self;
}

// FUNCTION: EQMAIN 0x10024E00
// Runs object cleanup and optionally releases the object's storage.
// __thiscall (this in ECX, one byte argument; written as __fastcall with a dummy EDX and the argument, callee pops 4).
// Calls 0x10024E20 (FreePointerArrays_guess) on this, then when bit 0 of flags is set calls 0x10033800 (cdecl) with
// this. Returns this.
void * __fastcall FreePointerArraysDtor_guess(void *self, void *edx, unsigned char flags)
{
    EQ_METHOD(0, void, 0x10024E20, (void *), self)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(self);
    return self;
}

// FUNCTION: EQMAIN 0x10024E20
// Releases the object's retained pointer entries; their game purpose is unclear.
// __thiscall (this in ECX, no other argument; written as __fastcall with one argument), returns nothing. Stores
// 0x10041F78 at +0x00 (the vtable), calls 0x10024CB0, then frees each non-null pointer of the array at +0x1C (count
// at +0x14) and of the array at +0x11C (count at +0x18). The slots are not cleared. The original's RET leaves the
// second count in EAX from the loop's last reload; callers never read EAX there (0/2 sites).
void __fastcall FreePointerArrays_guess(void *self)
{
    int i;
    int j;

    *(int *)self = 0x10041F78;
    eqmain_clear_5904_guess();
    for (i = 0; i < ((EqHostList_guess*)self)->login_count; i++)
    {
        if (((void **)((char *)self + 0x1C))[i] != 0)
        {
            eqmain_free_guess(((void **)((char *)self + 0x1C))[i]);
        }
    }
    for (j = 0; j < ((EqHostList_guess*)self)->reg_count; j++)
    {
        if (((void **)((char *)self + 0x11C))[j] != 0)
        {
            eqmain_free_guess(((void **)((char *)self + 0x11C))[j]);
        }
    }
}

// FUNCTION: EQMAIN 0x10024E90
// __thiscall (this in ECX; written as __fastcall with a dummy EDX, which the original overwrites), returns int,
// callee pops 4. The object has no header yet: index at +0x04, reset index at +0x0C, count at +0x14, 32 name
// pointers at +0x1C and 32 dwords at +0x9C. A nonzero argument sets the index to +0x0C first; a zero argument
// returns 1 at once when the index already equals +0x0C. Otherwise copies the name at the index into 0x10125850,
// stores the dword at the index in g_eqmain_port_guess, advances the index and wraps it to 0 at the count. Returns 0.
int __fastcall CycleNameAndValue_guess(void *self, int unusedEdx, int reset)
{
    if (reset != 0)
    {
        ((EqHostList_guess*)self)->login_cycle_index_guess = ((EqHostList_guess*)self)->login_index;
    }
    else if (((EqHostList_guess*)self)->login_cycle_index_guess == ((EqHostList_guess*)self)->login_index)
    {
        return 1;
    }
    strcpy(g_eqmain_cycle_text_guess, *(char **)((char *)self + ((EqHostList_guess*)self)->login_cycle_index_guess * 4 + 0x1c));
    g_eqmain_cycle_value_guess = *(unsigned int *)((char *)self + ((EqHostList_guess*)self)->login_cycle_index_guess * 4 + 0x9c);
    ++((EqHostList_guess*)self)->login_cycle_index_guess;
    if (((EqHostList_guess*)self)->login_cycle_index_guess >= ((EqHostList_guess*)self)->login_count)
    {
        ((EqHostList_guess*)self)->login_cycle_index_guess = 0;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10024F10
// __thiscall (this in ECX; written as __fastcall with a dummy EDX, which the original overwrites), returns int,
// callee pops 4. The same cycle as CycleNameAndValue_guess on a second pair of tables: index at +0x08, reset index
// at +0x10, count at +0x18, name pointers at +0x11C and dwords at +0x19C. A nonzero argument sets the index to +0x10
// first; a zero argument returns 1 at once when the index already equals +0x10. Otherwise copies the name at the index
// into 0x10125850, stores the dword at the index in g_eqmain_port_guess, advances the index and wraps it to 0 at the count.
// Returns 0.
int __fastcall CycleNameAndValueAlt_guess(void *self, int unusedEdx, int reset)
{
    if (reset != 0)
    {
        ((EqHostList_guess*)self)->reg_cycle_index_guess = ((EqHostList_guess*)self)->reg_index;
    }
    else if (((EqHostList_guess*)self)->reg_cycle_index_guess == ((EqHostList_guess*)self)->reg_index)
    {
        return 1;
    }
    strcpy(g_eqmain_cycle_text_guess, *(char **)((char *)self + ((EqHostList_guess*)self)->reg_cycle_index_guess * 4 + 0x11c));
    g_eqmain_cycle_value_guess = *(unsigned int *)((char *)self + ((EqHostList_guess*)self)->reg_cycle_index_guess * 4 + 0x19c);
    ++((EqHostList_guess*)self)->reg_cycle_index_guess;
    if (((EqHostList_guess*)self)->reg_cycle_index_guess >= ((EqHostList_guess*)self)->reg_count)
    {
        ((EqHostList_guess*)self)->reg_cycle_index_guess = 0;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10024F90
// Selects a 16-bit code for a front-end message exchange.
// __cdecl, void. Sets the dword state to 2 and stores the word argument in its own global. The state's meaning
// and the word's are unknown.
void __cdecl SetStateAndWord_guess(unsigned short value)
{
    g_eqmain_state_guess = 2;
    g_eqmain_word_guess = value;
}

// FUNCTION: EQMAIN 0x10024FB0
// Appends a one-byte field to an outgoing front-end message.
// @early-stop: only the counter and buffer store order remains different; a saved pointer adds an address calculation.
// __cdecl, void. If the cursor g_eqmain_state_guess + 1, as a signed int, is <= 0x2000, advances the cursor by 1 and
// stores the byte argument at 0x10125914 + the old cursor; otherwise does nothing. The meanings of the cursor and the
// buffer are unknown.
void __cdecl AppendByte_guess(unsigned char value)
{
    unsigned int next = g_eqmain_state_guess + 1;

    if ((int)next <= 0x2000)
    {
        ((unsigned char *)0x10125914)[g_eqmain_state_guess] = value;
        g_eqmain_state_guess = next;
    }
}

// FUNCTION: EQMAIN 0x10024FD0
// Appends a four-byte value to an outgoing front-end message.
// __cdecl, void. If the cursor g_eqmain_state_guess + 4 is <= 0x2000, advances the cursor by 4 and stores the dword
// argument at 0x10125914 + the old cursor; otherwise does nothing. The meanings of the cursor and the buffer are unknown.
void __cdecl AppendDword_guess(unsigned int value)
{
    unsigned int next = g_eqmain_state_guess + 4;

    if (next <= 0x2000)
    {
        memcpy(g_sendbuf_10125914_guess + g_eqmain_state_guess, &value, 4);
        g_eqmain_state_guess = next;
    }
}

// FUNCTION: EQMAIN 0x10024FF0
// Adds a text value to an outgoing front-end message.
// __cdecl, void. Appends the string argument with its NUL terminator at 0x10125914 + the cursor g_eqmain_state_guess, then
// advances the cursor past it, if the new cursor is <= 0x2000 (otherwise nothing). An empty string stores only the
// terminator and advances the cursor by 1, with no bounds check. The meanings of the cursor and the buffer are unknown.
void __cdecl AppendString_guess(char *text)
{
    unsigned int cursor;
    char c;
    char *dst;
    int len;
    int next;

    len = strlen(text);
    if (len == 0)
    {
        cursor = g_eqmain_state_guess;
        *(char *)(0x10125914 + cursor) = 0;
        g_eqmain_state_guess = cursor + 1;
        return;
    }
    next = g_eqmain_state_guess + 1 + len;
    if (next <= 0x2000)
    {
        c = *text;
        dst = g_sendbuf_10125914_guess + g_eqmain_state_guess;
        *dst = c;
        dst++;
        text++;
        while (c != 0)
        {
            c = *text;
            *dst = c;
            dst++;
            text++;
        }
        g_eqmain_state_guess = g_eqmain_state_guess + 1 + len;
    }
}

// FUNCTION: EQMAIN 0x10025060
// Adds a raw block of bytes to an outgoing front-end message.
// __cdecl, void. Copies len bytes from data to 0x10125914 + the cursor g_eqmain_state_guess, then advances the cursor
// by len, if the new cursor is <= 0x2000 (a signed compare, JG; otherwise nothing). The copy is VC6's inline memcpy
// (rep movsd, then rep movsb). The meanings of the cursor and the buffer are unknown.
void __cdecl AppendBytes_guess(const void *data, unsigned int len)
{
    int next;

    next = g_eqmain_state_guess + len;
    if (next <= 0x2000)
    {
        memcpy((char *)(0x10125914 + g_eqmain_state_guess), data, len);
        g_eqmain_state_guess = next;
    }
}

// FUNCTION: EQMAIN 0x100250A0
// Sets the source buffer for the front-end's structured-data reader.
// __cdecl, void. Sets the dword at 0x1012B91C to 2 and stores the dword argument at 0x1012B914. The meanings of
// both globals are unknown.
void __cdecl SetStateAndDword_guess(unsigned int value)
{
    g_eqmain_state_b91c_guess = 2;
    g_eqmain_value_b914_guess = value;
}

// FUNCTION: EQMAIN 0x100250C0
// Reads the next byte field from the current front-end data record.
// __stdcall, returns a byte in AL, no arguments. Reads the byte at 0x1012B914 + the cursor at 0x1012B91C and advances
// the cursor by 1 while cursor + 1 <= 0x2000 (a signed compare, JG); otherwise returns 0 and leaves the cursor alone.
// The meanings of both globals are unknown.
unsigned char __stdcall NextByte_guess(void)
{
    unsigned char value = 0;
    int next = (int)g_eqmain_state_b91c_guess + 1;

    if (next <= 0x2000)
    {
        value = *(unsigned char *)(g_eqmain_value_b914_guess + g_eqmain_state_b91c_guess);
        g_eqmain_state_b91c_guess = next;
    }
    return value;
}

// FUNCTION: EQMAIN 0x100250F0
// Reads the next 16-bit value from the current front-end data record.
// __stdcall, returns a word in AX, no arguments. Reads the word at 0x1012B914 + the cursor at 0x1012B91C and advances
// the cursor by 2 while cursor + 2 <= 0x2000 (an unsigned compare, JA); otherwise returns 0 and leaves the cursor
// alone. On success, EAX's high word is the high word of the 0x1012B914 value because the original loads that value
// into EAX before replacing AX. The meanings of both globals are unknown.
unsigned short __stdcall NextWord_guess(void)
{
    unsigned short value = 0;

    if (g_eqmain_state_b91c_guess + 2 <= 0x2000)
    {
        unsigned int base = g_eqmain_value_b914_guess;

        value = *(unsigned short *)(base + g_eqmain_state_b91c_guess);
        g_eqmain_state_b91c_guess += 2;
    }
    return value;
}

// File-local layout of the object at 0x10025940 (fields at +0xc and +0x10; the object's meaning is unknown).
struct IdxLookup_guess
{
    unsigned char unk_00[0x0c];  // 0x00, not read by this function
    unsigned int* items;         // 0x0c
    int index;                   // 0x10

    unsigned int Get_guess(int param);
};

// FUNCTION: EQMAIN 0x10025120
// Reads the next 32-bit value from the current front-end data record.
// __stdcall, returns a dword, no arguments. Reads the dword at 0x1012B914 + the cursor at 0x1012B91C and advances
// the cursor by 4 while cursor + 4 <= 0x2000; otherwise returns 0 and leaves the cursor alone. The meanings of
// both globals are unknown.
unsigned int __stdcall NextDword_guess(void)
{
    unsigned int value = 0;

    if (g_eqmain_state_b91c_guess + 4 <= 0x2000)
    {
        unsigned int base = g_eqmain_value_b914_guess;

        value = *(unsigned int *)(base + g_eqmain_state_b91c_guess);
        g_eqmain_state_b91c_guess += 4;
    }
    return value;
}

// FUNCTION: EQMAIN 0x10025150
// Reads the next plain text value from a front-end data record.
// __stdcall, no arguments, returns the string buffer at 0x10127914 (undefined1 *). Copies the NUL-terminated text at
// 0x1012B914 + the cursor at 0x1012B91C into the buffer, stopping at the NUL or when the cursor reaches 0x2000. The
// cursor is compared signed, so a negative one still copies. Then NUL-terminates the buffer and moves the cursor past
// the text and its NUL: cursor + strlen + 1. At or past 0x2000 nothing is copied and the cursor moves by 1. The
// meanings of both globals are unknown.
// @early-stop: src[count] is folded to g[i] (the original keeps g + cursor, indexed by count): cursor/count swap EDX/EBX.
char * __stdcall NextString_guess(void)
{
    int cursor = (int)g_eqmain_state_b91c_guess;
    int count = 0;
    int i;
    char ch;
    char *src;
    char *dst;

    if (cursor < 0x2000)
    {
        i = cursor;
        dst = g_eqmain_text_guess - cursor;
        src = (char *)(g_eqmain_value_b914_guess + cursor);
        while (i < 0x2000)
        {
            ch = src[count];
            if (ch == '\0')
            {
                break;
            }
            dst[i] = ch;
            count++;
            i++;
        }
    }
    g_eqmain_text_guess[count] = '\0';
    g_eqmain_state_b91c_guess = cursor + strlen(g_eqmain_text_guess) + 1;
    return g_eqmain_text_guess;
}

// FUNCTION: EQMAIN 0x100251C0
// Reads the next text value, decoding packed text when its tag is nonzero.
// @early-stop: decode address operands load in reverse order; copy path uses different registers and destination addressing.
// __stdcall, no arguments, returns the string buffer at 0x10129914 (undefined1 *). Reads a tag with 0x100250F0 (the
// cursor moves by 2 there) and tests its low 16 bits. Zero: copies the NUL-terminated text at 0x1012B914 + the cursor
// at 0x1012B91C into the buffer, as NextString_guess does (the cursor is compared signed), and moves the cursor past
// the text and its NUL. Anything else: passes the cursor's text, the whole tag and the buffer to 0x100284B0, then moves
// the cursor by (strlen of the buffer & 0xffff) + 1, reading the cursor again after the call. The meanings of both
// globals are unknown.
char * __stdcall NextTaggedString_guess(void)
{
    unsigned int tag = eqmain_next_tag_guess();
    int count;
    int i;
    char ch;
    char *textCursor;

    if ((unsigned short)tag > 0)
    {
        textCursor = (char *)g_eqmain_state_b91c_guess;
        eqmain_decode_text_guess((unsigned short *)(g_eqmain_value_b914_guess + textCursor), tag,
                                 (unsigned char *)g_eqmain_text_9914_guess);
        g_eqmain_state_b91c_guess = g_eqmain_state_b91c_guess + 1 + (unsigned short)strlen(g_eqmain_text_9914_guess);
        return g_eqmain_text_9914_guess;
    }
    count = 0;
    i = (int)g_eqmain_state_b91c_guess;
    if (i < 0x2000)
    {
        while (i < 0x2000)
        {
            ch = *(char *)(g_eqmain_value_b914_guess + i);
            if (ch == '\0')
            {
                break;
            }
            g_eqmain_text_9914_guess[i - (int)g_eqmain_state_b91c_guess] = ch;
            count++;
            i++;
        }
    }
    g_eqmain_text_9914_guess[count] = '\0';
    g_eqmain_state_b91c_guess = g_eqmain_state_b91c_guess + 1 + count;
    return g_eqmain_text_9914_guess;
}

// FUNCTION: EQMAIN 0x10025260
// Reads a caller-sized block from the current front-end data record.
// __cdecl, two arguments (dst, count), no return value. Copies count bytes from 0x1012B914 + the cursor at
// 0x1012B91C to dst, then moves the cursor forward by count. Nothing is checked: the source is read as far as
// count says, even past the end of the text. The meanings of both globals are unknown.
void __cdecl NextBytes_guess(void *dst, unsigned int count)
{
    char *src = (char *)(g_eqmain_value_b914_guess + g_eqmain_state_b91c_guess);

    memcpy(dst, src, count);
    g_eqmain_state_b91c_guess = g_eqmain_state_b91c_guess + count;
}

// FUNCTION: EQMAIN 0x100252A0
// Extracts a decimal integer from front-end text records.
// __cdecl, one argument (the text cursor: a pointer to a char pointer, advanced in place), returns int. Skips the
// characters that are not digits, copies the run of digits after them into an 8-byte buffer (moving the cursor past
// them) and returns atoi of the buffer, so a '-' before the digits is dropped. Nothing bounds the copy: a run of 8 or
// more digits writes past the buffer, and the skip loop runs on past the end of the text when no digit is left.
int __cdecl ParseIntAtCursor_guess(int *cursor)
{
    char buf[8];
    int n = 0;

    while (!eqmain_isdigit_guess((int)*(char *)*cursor))
    {
        *cursor = *cursor + 1;
    }
    while (eqmain_isdigit_guess((int)*(char *)*cursor))
    {
        char *p = (char *)*cursor;

        buf[n++] = *p++;
        *cursor = (int)p;
    }
    buf[n] = 0;
    return eqmain_atoi_guess(buf);
}

// FUNCTION: EQMAIN 0x10025320
// Returns a text line from a buffer; its game use is unclear.
// __cdecl, one argument (the text cursor: a pointer to a char pointer, advanced in place), returns char *: the start of
// the next line. Skips the rest of the current line (to a '\n', a '\r' or a NUL), then every '\n', '\r' and NUL after it
// (a NUL too, so with no line left the skip runs on past the end of the text), NUL-terminates the line there (overwriting
// its '\r' or '\n') and leaves the cursor on that terminator. The digit loop moves the cursor and calls _isdigit, but the
// cursor is stored again after it, so only those calls are left.
char * __cdecl NextLineAtCursor_guess(int *cursor)
{
    char *line;
    char *end;
    char c;
    int digit;

    c = *(char *)*cursor;
    while (c != '\n')
    {
        c = *(char *)*cursor;
        if (c == '\r' || c == '\0')
            break;
        end = (char *)*cursor + 1;
        *cursor = (int)end;
        c = *end;
    }
    while (true)
    {
        c = *(char *)*cursor;
        if (c != '\n' && c != '\r' && c != '\0')
            break;
        *cursor = (int)((char *)*cursor + 1);
    }
    end = (char *)*cursor;
    c = *end;
    while (c != '\n' && c != '\r' && c != '\0')
    {
        c = end[1];
        end++;
    }
    *end = '\0';
    line = (char *)*cursor;
    digit = eqmain_isdigit_guess((int)*line);
    while (digit != 0)
    {
        *cursor = *cursor + 1;
        digit = eqmain_isdigit_guess((int)*(char *)*cursor);
    }
    *cursor = (int)end;
    return line;
}

// The object the global points at, as far as this function sees it: only its virtual slot 0, which the original
// calls through the vtable with 1 (this in ECX, the flag pushed, the callee pops it). Its class is unknown.
class Slot0Object_guess
{
public:
    virtual void Slot0_guess(int flag);
};

// FUNCTION: EQMAIN 0x100253B0
// Releases a shared front-end object; its game role is unclear.
// __stdcall, no arguments, no return value. If the global object pointer at 0x1012C774 is not null, calls the
// object's vtable slot 0 with 1, then clears the global. The object's type is unknown; the flag 1 suggests a
// deleting destructor.
void __stdcall ReleaseGlobalObject_guess(void)
{
    Slot0Object_guess *obj = g_eqmain_obj_c774_guess;

    if (obj != 0)
    {
        obj->Slot0_guess(1);
    }
    g_eqmain_obj_c774_guess = 0;
}

// eqmain_load_string_tables_guess (0x100253D0): its own globals, and the row type of the two tables.
typedef char eqmain_row64_guess[64];
#define g_eqmain_block_b920_guess   EQ_ARRAY(unsigned char, 0x1012B920)
#define g_eqmain_count_c214_guess   EQ_GLOBAL(short, 0x1012C214)
#define g_eqmain_rows_c4ec_guess    EQ_ARRAY(eqmain_row64_guess, 0x1012C4EC)
#define g_eqmain_rows_bf94_guess    EQ_ARRAY(eqmain_row64_guess, 0x1012BF94)
// 0xB3 dwords (0x2CC bytes) from 0x1012C220 to the rows at 0x1012C4EC, copied by the unit at 0x10025790.
#define g_eqmain_block_c220_guess   EQ_ARRAY(unsigned int, 0x1012C220)

// FUNCTION: EQMAIN 0x100253D0
// Loads shared front-end text rows and a lookup list.
// __stdcall, no arguments, returns bool (AL = 1 on its only path). Clears the 0x674-byte block at 0x1012B920, then
// makes a 0x10-byte object (operator new 0x100338E5; the constructor UnitInit_guess when the allocation is not null)
// and keeps it in the global at 0x1012C774. Calls LoadS989RegRecords_guess with that object (fastcall, in ECX) and calls
// ReleaseGlobalObject_guess when it returns 0. Reads a count with NextDword_guess into the short at 0x1012C214 and copies one string
// per count into the 64-byte rows at 0x1012C4EC (string = DecodeToBuffer_guess(NextString_guess())), then does the same into the
// rows at 0x1012BF94. No bounds checks: row 10 of the second table is the count word at 0x1012C214 (its string
// rewrites the loop bound), and rows 21 and up of the second table overlap the first table.
bool __stdcall eqmain_load_string_tables_guess(void)
{
    Slot0Object_guess *obj;
    short i;

    memset(g_eqmain_block_b920_guess, 0, 0x674);
    obj = (Slot0Object_guess *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x10);
    if (obj != 0)
    {
        obj = EQ_METHOD(0, Slot0Object_guess *, 0x100259C0, (Slot0Object_guess *), obj)();
    }
    else
    {
        obj = 0;
    }
    g_eqmain_obj_c774_guess = obj;
    if (EQ_METHOD(0, int, 0x10025B00, (Slot0Object_guess *), obj)() == 0)
    {
        EQ_STDCALL(void, 0x100253B0, (void))();
    }
    g_eqmain_count_c214_guess = (short)EQ_STDCALL(unsigned int, 0x10025120, (void))();
    for (i = 0; i < g_eqmain_count_c214_guess; i++)
    {
        strcpy(g_eqmain_rows_c4ec_guess[i],
               eqmain_convert_report_token_guess(eqmain_fn_10025150_guess()));
    }
    for (i = 0; i < g_eqmain_count_c214_guess; i++)
    {
        strcpy(g_eqmain_rows_bf94_guess[i],
               eqmain_convert_report_token_guess(eqmain_fn_10025150_guess()));
    }
    return true;
}

// The queue handle (g_eqmain_rdp_guess, undefined4) that the message unit passes to 0x10021420 as its first argument.
#define g_eqmain_queue_5638_guess    EQ_GLOBAL(unsigned int, 0x10125638)
// Takes the next message of the queue (cdecl, the queue and 1; 0 when none is left).
#define eqmain_rdp_receive_guess     EQ_CDECL(int, 0x10021420, (unsigned int, int))
// Two checks on a message (cdecl, both must return non-zero) and the unit that gives its data pointer (cdecl; the
// pointer comes back as an int and the caller casts it).
#define eqmain_fn_1002de90_guess      EQ_CDECL(int, 0x1002DE90, (int))
#define eqmain_fn_1002dd30_guess       EQ_CDECL(int, 0x1002DD30, (int))
#define eqmain_msg_data_guess        EQ_CDECL(int, 0x1002DE70, (int))
// Frees a message (cdecl, the message pointer; the unit frees the pointer minus 8).
#define eqmain_release_msg_guess     EQ_CDECL(void, 0x10028A90, (int))
// Stores the message data as the text base (cdecl; the unit writes its argument to 0x1012B914).
#define eqmain_set_text_base_guess   EQ_CDECL(void, 0x100250A0, (unsigned short *))
// Next string of the text base (stdcall, no arguments).
// cdecl, the message data (named after a sibling's CopyFirstWord, low confidence).
#define eqmain_copy_first_word_guess EQ_CDECL(void, 0x100162A0, (unsigned short *))
// Fixed string targets: strcpy into them has no bound, as in the original.
#define g_eqmain_copybuf_guess     EQ_ARRAY(char, 0x100F4C10)
#define g_eqmain_login_name_buf_guess     EQ_ARRAY(char, 0x10104CD8)
#define g_eqmain_buf_4ac_guess     EQ_ARRAY(char, 0x1012C4AC)

// FUNCTION: EQMAIN 0x100254F0
// Handles queued front-end messages that update shared text and other state.
// @early-stop: msg/result use EBX/EBP opposite the original; the two set-text paths still split ADD ESP, 8.
// __stdcall, no arguments, returns unsigned int: the opcode of the last message taken (0 when the queue had none), or
// 0x10000003 when a message fails a check. Takes the messages of the queue at 0x10125638 one by one (Eqmain_RdpReceiveIfReady_guess,
// cdecl, the queue and 1). A message needs GetRecordBase_guess and GetRecordDword6D4_guess to return non-zero and msg_arrival_get_data_guess to give
// its data (else it is freed by free_pool_block_guess and the function returns 0x10000003). The word at the data is the opcode:
// 2, 0x1C, 0x32, 0x35 set the text base to the data (SetStateAndDword_guess), then copy DecodeToBuffer_guess(NextString_guess()) into
// 0x100F4C10; 0x1D copies DecodeToBuffer_guess(&g_eqmain_block_b920_guess) into 0x10104CD8; 0x36 sets the text base, copies NextString_guess()
// into 0x1012C4AC and DecodeToBuffer_guess(NextString_guess()) into 0x100F4C10; 0x39 calls LoadRecordsIntoList_guess with the data. Other
// opcodes do nothing. Each message is freed after its case and the next one is taken; the return is the last opcode.
unsigned int __stdcall eqmain_process_messages_guess(void)
{
    unsigned int opcode;
    unsigned int dispatch_opcode;
    int msg;
    int hdr;
    unsigned short *data;
    char *text;

    opcode = 0;
    msg = eqmain_rdp_receive_guess(g_eqmain_queue_5638_guess, 1);
    do
    {
        if (msg == 0)
            return opcode;
        hdr = eqmain_fn_1002de90_guess(msg);
        if (((hdr == 0) || (hdr = eqmain_fn_1002dd30_guess(hdr), hdr == 0)) ||
            (data = (unsigned short *)eqmain_msg_data_guess(msg), data == 0))
        {
            eqmain_release_msg_guess(msg);
            return 0x10000003;
        }
        dispatch_opcode = *data;
        switch (dispatch_opcode)
        {
        case 2:
        case 0x1c:
        case 0x32:
        case 0x35:
            eqmain_set_text_base_guess(data);
            text = eqmain_convert_report_token_guess(eqmain_fn_10025150_guess());
            strcpy(g_eqmain_copybuf_guess, text);
            break;
        case 0x1d:
            text = eqmain_convert_report_token_guess(g_eqmain_block_b920_guess);
            strcpy(g_eqmain_login_name_buf_guess, text);
            break;
        case 0x36:
            eqmain_set_text_base_guess(data);
            text = (char *)eqmain_fn_10025150_guess();
            strcpy(g_eqmain_buf_4ac_guess, text);
            text = eqmain_convert_report_token_guess(eqmain_fn_10025150_guess());
            strcpy(g_eqmain_copybuf_guess, text);
            break;
        case 0x39:
            eqmain_copy_first_word_guess(data);
            break;
        }
        eqmain_release_msg_guess(msg);
        opcode = dispatch_opcode;
        msg = eqmain_rdp_receive_guess(g_eqmain_queue_5638_guess, 1);
    }
    while (true);
    return opcode;
}

// FUNCTION: EQMAIN 0x100256C0
// Builds front-end message 0x23 from the B920 data block; its game meaning is unclear.
// __stdcall, no arguments, returns bool (AL = 1 on its only path). Copies 0x19D dwords from 0x1012B920 into a 0x678-byte
// local (VC6's inline memcpy, rep movsd; the last dword of the local is not copied), passes it and a second 0x678-byte
// local to the unit at 0x100312E0 (cdecl, three args: the source, the second local, 0x678), calls the unit at 0x10024F90
// with 0x23 (cdecl, a short argument), appends the second local with the unit at 0x10025060 (cdecl, a pointer and 0x678),
// calls the unit at 0x10024FD0 with 1 (cdecl, a dword argument), then calls the __stdcall unit at 0x100213F0 with no
// arguments and returns true.
bool __stdcall eqmain_append_record_b920_guess(void)
{
    unsigned int source[414];
    unsigned int data[414];

    memcpy(source, g_eqmain_block_b920_guess, 0x674);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(source, (unsigned char *)data, 0x678);
    eqmain_set_report_opcode_guess(0x23);
    EQ_CDECL(void, 0x10025060, (const void *, unsigned int))(data, 0x678);
    eqmain_append_report_value_guess(1);
    eqmain_finish_hardware_report_guess();
    return true;
}

// FUNCTION: EQMAIN 0x10025790
// __stdcall, no arguments, returns bool (AL = 1 on its only path). Copies 0xB3 dwords from 0x1012C220 into a 0x2D0-byte
// local (VC6's inline memcpy, rep movsd; the last dword of the local is not copied), passes it and a second 0x2D0-byte
// local to the unit at 0x100312E0 (cdecl, three args: the source, the second local, 0x2D0), calls the unit at 0x10024F90
// with 0x7A (cdecl, a short argument), appends the second local with the unit at 0x10025060 (cdecl, a pointer and 0x2D0),
// then calls the __stdcall unit at 0x100213F0 with no arguments and returns true.
bool __stdcall eqmain_append_record_c220_guess(void)
{
    unsigned int source[180];
    unsigned int data[180];

    memcpy(source, g_eqmain_block_c220_guess, 0x2cc);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(source, (unsigned char *)data, 0x2d0);
    eqmain_set_report_opcode_guess(0x7a);
    EQ_CDECL(void, 0x10025060, (const void *, unsigned int))(data, 0x2d0);
    eqmain_finish_hardware_report_guess();
    return true;
}

// FUNCTION: EQMAIN 0x10025730
// __stdcall, no arguments, returns bool (AL = 1 on its only path). Copies the 0x2CC-byte block at 0x1012C220 (179
// dwords) into table_copy (0x2D0 bytes; table_copy[179] is not copied and keeps whatever the stack holds), decrypts
// table_copy into decrypted with the global DES key (desCryptGlobalKey_guess, 0x2D0 bytes), sets the word 0x31 (SetStateAndWord_guess),
// appends decrypted (AppendBytes_guess, 0x2D0 bytes), then calls the unit at 0x100213F0 (no arguments, returns at once).
bool __stdcall eqmain_queue_table_push31_guess(void)
{
    unsigned int table_copy[180];
    unsigned int decrypted[180];

    memcpy(table_copy, g_eqmain_block_c220_guess, 0x2cc);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(table_copy, (unsigned char *)decrypted, 0x2d0);
    EQ_CDECL(void, 0x10024F90, (short))(0x31);
    EQ_CDECL(void, 0x10025060, (const void *, unsigned int))(decrypted, 0x2d0);
    eqmain_finish_hardware_report_guess();
    return true;
}

// FUNCTION: EQMAIN 0x100257F0
// Forwards 0x37 to a front-end helper; its game meaning is unclear.
// __stdcall, no arguments, no return value. Pushes 0x37 and calls the unit at 0x100213D0 (a void call with one
// int argument; the caller pops the 0x37 with POP ECX), then returns.
void __stdcall eqmain_call_100213d0_push37_guess(void)
{
    EQ_CDECL(void, 0x100213D0, (int))(0x37);
}

// FUNCTION: EQMAIN 0x10025800
// Builds a front-end message tagged 0x38 from two values; its game meaning is unclear.
// __cdecl, two int arguments, no return value. Calls the unit at 0x10024F90 with 0x38 (a void call with one
// short argument), then the unit at 0x10024FD0 with each argument in turn (void calls with one int argument),
// then tail-jumps to the __stdcall unit at 0x100213F0 with no arguments (it returns to our caller).
void __cdecl eqmain_setup_push38_guess(unsigned int firstValue, unsigned int secondValue)
{
    EQ_CDECL(void, 0x10024F90, (short))(0x38);
    EQ_CDECL(void, 0x10024FD0, (int))(firstValue);
    EQ_CDECL(void, 0x10024FD0, (int))(secondValue);
    eqmain_finish_hardware_report_guess();
}

// FUNCTION: EQMAIN 0x10025830
// Forwards 0x44 to a front-end helper; its game meaning is unclear.
// __stdcall, no arguments, no return value. Pushes 0x44 and calls the unit at 0x100213D0 (a void call with no
// arguments of its own; the caller pops the 0x44 with POP ECX), then returns.
void __stdcall eqmain_call_100213d0_guess(void)
{
    EQ_CDECL(void, 0x100213D0, (int))(0x44);
}

// FUNCTION: EQMAIN 0x10025840
// Forwards 0x40 to a front-end helper; its game meaning is unclear.
// __stdcall, no arguments, no return value. Pushes 0x40 and calls the unit at 0x100213D0 (a void call with no
// arguments of its own; the caller pops the 0x40 with POP ECX), then returns.
void __stdcall eqmain_call_100213d0_push40_guess(void)
{
    EQ_CDECL(void, 0x100213D0, (int))(0x40);
}

// FUNCTION: EQMAIN 0x10025850
// Builds a front-end message tagged 0x46 from a value and a flag; its game meaning is unclear.
// __cdecl, one dword argument (value), no return value. Calls the unit at 0x10024F90 with 0x46, the unit at
// 0x10024FD0 with the argument, the unit at 0x10024FB0 with 1 if the flag at 0x100F1D20 is set (else 0), then
// jumps to the __stdcall unit at 0x100213F0 (no arguments), so that unit returns to our caller.
void __cdecl eqmain_call_10024f90_push46_guess(unsigned int value)
{
    eqmain_set_report_opcode_guess(0x46);
    eqmain_append_report_value_guess(value);
    if (g_eqmain_flag_1d20_guess != 0)
        EQ_CDECL(void, 0x10024FB0, (int))(1);
    else
        EQ_CDECL(void, 0x10024FB0, (int))(0);
    eqmain_finish_hardware_report_guess();
}

// FUNCTION: EQMAIN 0x10025880
// Destroys the front-end indexed-list object and optionally frees it.
// __thiscall (callee pops 4 bytes, RET 4). Scalar deleting destructor: runs the __fastcall unit at 0x10025970 on this,
// frees this with the cdecl unit at 0x10033800 when bit 0 of flags is set, returns this.
void *__fastcall IdxLookupScalarDeletingDtor_guess(void *self, int edx_unused, unsigned char flags)
{
    EQ_METHOD(0, void, 0x10025970, (void *), self)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void *))(self);
    return self;
}

// FUNCTION: EQMAIN 0x100258A0
// Creates a named front-end list with indexed entry slots.
// __thiscall, two stack args (RET 8). Builds the object: vtable &PTR_FUN_10041f80 (0x10041F80) at +0, a __strdup() copy
// of the name at +4, the count at +8, items (+0xc) = malloc(count * 4) zeroed with memset, index (+0x10) = 0. Returns this.
IdxLookup_guess *__fastcall InitIdxLookup_guess(IdxLookup_guess *self, int edx_unused, char *name, unsigned int param_2)
{
    unsigned int size;
    unsigned int *items;

    *(void ***)self->unk_00 = (void **)0x10041F80;
    *(char **)(self->unk_00 + 4) = eqmain_fn_1003396b_guess(name);
    *(unsigned int *)(self->unk_00 + 8) = param_2;
    size = param_2 * 4;
    items = (unsigned int *)eqmain_malloc_guess(size);
    self->items = items;
    memset(items, 0, size);
    self->index = 0;
    return self;
}

// FUNCTION: EQMAIN 0x10025900
// Adds a copied text entry to the front-end indexed list.
// __thiscall, one stack arg (RET 4). Stores a __strdup() copy of the string at items[index] (items at +0xc, index at
// +0x10). When the stored pointer is not null, bumps the index and returns the new value; otherwise returns 0.
int __fastcall AddStrCopy_guess(IdxLookup_guess *self, int edx_unused, char *param_1)
{
    char *textCopy;
    int nextIndex;

    textCopy = eqmain_fn_1003396b_guess(param_1);
    self->items[self->index] = (unsigned int)textCopy;
    if (self->items[self->index] != 0)
    {
        nextIndex = self->index + 1;
        self->index = nextIndex;
        return nextIndex;
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10025940
// __thiscall, one stack arg (RET 4). Stores the index at +0x10, then returns the dword at items[index].
unsigned int IdxLookup_guess::Get_guess(int itemIndex)
{
    index = itemIndex;
    return items[itemIndex];
}
