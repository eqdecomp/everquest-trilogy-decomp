// unit_100228C0.cpp - eqmain.dll, the address run 0x100228C0..0x10022D50 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// eqmain.dll globals, unnamed (this image's own, not in include/): the send buffer at 0x10125914, the dword
// at 0x1012B918 that AppendByte_guess uses as the append index (and as the length sent), and the dword at
// 0x10125848 that is the connection the buffer goes out on.
#define g_buf_10125914_guess      EQ_ARRAY(u8, 0x10125914)
#define g_dword_1012b918_guess    EQ_GLOBAL(DWORD, 0x1012B918)
#define g_dword_10125848_guess    EQ_GLOBAL(DWORD, 0x10125848)

// eqmain.dll chat globals, unnamed: the chat line buffer at 0x101255F8 (a string), and the two pointers at 0x101258A8
// and 0x101258A4 that flush_chat_and_free_guess releases (each points to an object with a word at +0x36B0).
#define g_buf_101255f8_guess      EQ_ARRAY(char, 0x101255F8)
#define g_ptr_101258a8_guess      EQ_GLOBAL(void *, 0x101258A8)
#define g_indexedValuesObject_guess      EQ_GLOBAL(void *, 0x101258A4)

// eqmain.dll strings, unnamed: 0x10048624 is "(en)", 0x1004861C is "%s %s".
#define g_str_10048624_guess      EQ_ARRAY(char, 0x10048624)
#define g_str_1004861c_guess      EQ_ARRAY(char, 0x1004861C)

// eqmain.dll tracert globals, unnamed: the object at 0x101258D0 (its dword at +0x3C is a handle, and FUN_10033800 frees
// it), the handle at 0x101258D4, and the text buffer at 0x100F4C10.
#define g_ptr_101258d0_guess      EQ_GLOBAL(void *, 0x101258D0)
#define g_handle_101258d4_guess   EQ_GLOBAL(HANDLE, 0x101258D4)
#define g_eqmain_copybuf_guess      EQ_ARRAY(char, 0x100F4C10)

// eqmain.dll tracert strings, unnamed: 0x10048648 is "...", 0x1004862C is "...",
// 0x10115574 is a string the message includes, 0x100480A4 and 0x10047F74 are the fopen modes.
#define g_str_10048648_guess      EQ_ARRAY(char, 0x10048648)
#define g_str_1004862c_guess      EQ_ARRAY(char, 0x1004862C)
#define g_buf_10115574_guess      EQ_ARRAY(char, 0x10115574)
#define g_eqmain_fopen_mode_guess      EQ_ARRAY(char, 0x100480A4)
#define g_str_10047f74_guess      EQ_ARRAY(char, 0x10047F74)

#define g_rdp_flag_guess EQ_GLOBAL(char, 0x10125844)
#define g_connection_pending_1012584c EQ_GLOBAL(char, 0x1012584C)
#define g_message_queue_10125638 EQ_GLOBAL(DWORD, 0x10125638)
#define g_buf_101257c4_guess EQ_ARRAY(char, 0x101257C4)
#define g_buf_100f4918_guess EQ_ARRAY(char, 0x100F4918)
#define g_choice_count_guess EQ_GLOBAL(int, 0x100F1164)
#define g_choice_text_guess EQ_ARRAY(char, 0x100F1168)
#define g_eqmain_name_guess EQ_ARRAY(char, 0x10104C70)
#define g_eqmain_str_104cb0_guess EQ_ARRAY(char, 0x10104CB0)
#define g_contact_names_10104cc4 EQ_ARRAY(char, 0x10104CC4)
#define g_sel_name_guess EQ_ARRAY(char, 0x100554B0)
#define g_eqmain_copy_cec_guess EQ_ARRAY(char, 0x10104CEC)
#define g_config_label_100556ec EQ_GLOBAL(char *, 0x100556EC)
#define g_config_label_10055710 EQ_GLOBAL(char *, 0x10055710)
#define g_format_10055718 EQ_GLOBAL(char *, 0x10055718)
#define g_language_label_10048098 EQ_ARRAY(char, 0x10048098)
#define g_language_state_100556e8 EQ_ARRAY(int, 0x100556E8)

typedef BOOL (WINAPI *GetExitCodeProcessProc)(HANDLE, LPDWORD);
#define g_get_exit_code_process EQ_GLOBAL(GetExitCodeProcessProc, 0x100400C4)

struct MessageList_guess
{
    char names[200][0x40];
    char reserved_3200[0x190];
    int values_by_index[200];
    unsigned short count;
    short selected_index;
    char refresh_pending;
};

struct ConfigEntry_guess
{
    DWORD unknown_00;
    int value;
};

// eqmain.dll strings, unnamed: 0x1004CE40 is the string that two copies read (in .data), 0x10114D34 is the buffer the
// first copy fills.
#define g_eqmain_drive_base_guess      EQ_ARRAY(char, 0x1004CE40)
#define g_buf_10114d34_guess      EQ_ARRAY(char, 0x10114D34)

// EXTERNAL: EQMAIN 0x10024F90 SetStateAndWord_guess_client
void __cdecl SetStateAndWord_guess_client(u16);
// EXTERNAL: EQMAIN 0x10024FB0 AppendByte_guess_client
void __cdecl AppendByte_guess_client(int);
// EXTERNAL: EQMAIN 0x100211B0 CopyStringToBuffer_guess_client
char *__cdecl CopyStringToBuffer_guess_client(char *);
// EXTERNAL: EQMAIN 0x10024FF0 AppendString_guess_client
void __cdecl AppendString_guess_client(char *);
// EXTERNAL: EQMAIN 0x10034117 fn_10034117_client
int __cdecl fn_10034117_client(char *, const char *, ...);
// EXTERNAL: EQMAIN 0x1002C010 connection_send_guess_client
void __cdecl connection_send_guess_client(int, void *, DWORD, DWORD, DWORD);
// FUNCTION: EQMAIN 0x100228C0
// Sends a tagged chat line with a formatted text copy.
// __cdecl, no return value. Writes a message into the send buffer at 0x10125914 and sends it with
// connection_send_guess on the connection in the dword at 0x10125848: the word 9 (SetStateAndWord_guess writes it at the
// buffer's start), the byte value, text as a string, the byte 1, then "(en) " + text. Each string goes in
// through CopyStringToBuffer_guess and AppendString_guess. value is undefined1 in the decompiler, but the original loads the
// whole argument dword and passes it on (AppendByte_guess appends its low byte), so it is an int here. The append
// index (dword 0x1012B918) is not reset; the sent length is that index. EAX is left as connection_send_guess
// returned it (no caller reads it).
void __cdecl send_language_text_guess(int value, char *text)
{
    char *bufferText;
    char formattedText[1024];

    SetStateAndWord_guess_client(9);
    AppendByte_guess_client(value);
    bufferText = CopyStringToBuffer_guess_client(text);
    AppendString_guess_client(bufferText);
    AppendByte_guess_client(1);
    fn_10034117_client(formattedText, g_str_1004861c_guess, g_str_10048624_guess, text);
    bufferText = CopyStringToBuffer_guess_client(formattedText);
    AppendString_guess_client(bufferText);
    connection_send_guess_client(g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
}

// FUNCTION: EQMAIN 0x10022950
// Thunk forwarding a tagged text message; its game effect is unclear.
// __cdecl, no return value. Forwards its three arguments to BuildTaggedText_guess (0x100224B0) unchanged. kind is
// undefined1 in the decompiler, but the original loads the whole argument dword and passes it on, so it is an int
// here. EAX is left as BuildTaggedText_guess returned it (no caller reads it).
void __cdecl forward_100224b0_guess(char *text, DWORD value, int kind)
{
    EQ_CDECL(void, 0x100224B0, (char *, DWORD, int))(text, value, kind);
}

// FUNCTION: EQMAIN 0x10022970
// Sends a nonempty text line as a front-end message.
// __cdecl, no return value. Does nothing for an empty string. Otherwise it takes the dword at +4 of the object
// EqfeCfgSetActive_guess(1) returns; when that is not 1 it calls send_language_text_guess with the dword at +4 of a second
// EqfeCfgSetActive_guess(1) call and the string. When it is 1, the message goes out plain: the word 9, the byte 1, the string
// through CopyStringToBuffer_guess and AppendString_guess, then connection_send_guess (the length is the append index, dword 0x1012B918).
// EAX is left as connection_send_guess returned it (no caller reads it).
void __cdecl send_chat_text_guess(char *text)
{
    char *bufferText;

    if (strlen(text) != 0)
    {
        if (EQ_CDECL(int *, 0x10020EF0, (char))(1)[1] != 1)
        {
            EQ_CDECL(void, 0x100228C0, (int, char *))(EQ_CDECL(int *, 0x10020EF0, (char))(1)[1], text);
            return;
        }
        EQ_CDECL(void, 0x10024F90, (u16))(9);
        EQ_CDECL(void, 0x10024FB0, (int))(1);
        bufferText = EQ_CDECL(char *, 0x100211B0, (char *))(text);
        EQ_CDECL(void, 0x10024FF0, (char *))(bufferText);
        EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    }
}

// FUNCTION: EQMAIN 0x100229F0
// Allocates and constructs two 0x36B8-byte objects, sets two text buffers, sends the words 0x24 and 6.
// __stdcall, no arguments, no return value. Two pending objects of 0x36B8 bytes: operator new (0x100338E5) gets the size,
// and when it returns non-null, UnitCallHelperReturnSelf_guess (__fastcall, this in ECX, returns its result) runs on it. The results go to
// g_eqmain_name_ptr_guess, then g_eqmain_source_rows_guess (0 when operator new returned 0). The string at 0x1004CE40 is copied, NUL included, to
// 0x10114D34, then to the chat line buffer 0x101255F8. Then the dword 0x24, then the dword 6, go out on the connection in
// g_eqmain_connection_guess through connection_send_guess (2 bytes at the dword, then 1, 1). The original's new has an EH frame (its
// ctor is out of line, in another unit); the by-address calls here have none. EAX is not a result (no caller reads it).
// @early-stop: 15 instructions still differ, mainly the original /GX new-expression frame and cleanup state.
void __stdcall open_chat_session_guess(void)
{
    union MatchBuffer
    {
        void *local_10;
        char bytes[16];
    } frame; /* fake match: keep the original 16-byte local area around local_10 */
    void *result;

    frame.local_10 = EQ_CDECL(void *, 0x100338E5, (DWORD))(0x36b8);
    if (frame.local_10 != 0)
        result = EQ_METHOD(0, void *, 0x10025CC0, (void *), frame.local_10)();
    else
        result = 0;
    g_indexedValuesObject_guess = result;
    frame.local_10 = EQ_CDECL(void *, 0x100338E5, (DWORD))(0x36b8);
    if (frame.local_10 != 0)
        result = EQ_METHOD(0, void *, 0x10025CC0, (void *), frame.local_10)();
    else
        result = 0;
    g_ptr_101258a8_guess = result;
    strcpy(g_buf_10114d34_guess, g_eqmain_drive_base_guess);
    strcpy(g_buf_101255f8_guess, g_eqmain_drive_base_guess);
    frame.local_10 = (void *)0x24;
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(g_dword_10125848_guess, &frame.local_10, 2, 1, 1);
    frame.local_10 = (void *)6;
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(g_dword_10125848_guess, &frame.local_10, 2, 1, 1);
}

// FUNCTION: EQMAIN 0x10022B10
// Finishes the front-end chat exchange, sending any queued line first.
// __stdcall, no arguments, no return value. A chat line in the buffer at 0x101255F8 goes out first when it is not
// empty: the word 8 through SetStateAndWord_guess, the line through CopyStringToBuffer_guess and AppendString_guess, then connection_send_guess
// (the length is the append index, dword 0x1012B918). Then each pending object whose slot is not null (0x101258A8,
// then 0x101258A4) gets its word at +0x36B0 cleared, is freed through FUN_10033800, and its slot is set to 0.
// EAX is not a result (no caller reads it).
void __stdcall flush_chat_and_free_guess(void)
{
    void *object;
    char *bufferText;

    if (strlen(g_buf_101255f8_guess) != 0)
    {
        EQ_CDECL(void, 0x10024F90, (u16))(8);
        bufferText = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_101255f8_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(bufferText);
        EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    }
    object = g_ptr_101258a8_guess;
    if (object != 0)
    {
        *(u16 *)((char *)object + 0x36B0) = 0;
        EQ_CDECL(void, 0x10033800, (void *))(object);
        g_ptr_101258a8_guess = 0;
    }
    object = g_indexedValuesObject_guess;
    if (object != 0)
    {
        *(u16 *)((char *)object + 0x36B0) = 0;
        EQ_CDECL(void, 0x10033800, (void *))(object);
        g_indexedValuesObject_guess = 0;
    }
}

// FUNCTION: EQMAIN 0x10022BA0
// Sends tracert.txt with two argument strings as message 0x6A, then deletes the file and frees its object.
// __cdecl, no return value (no caller reads EAX). Closes the handle at +0x3C of the object at 0x101258D0, then reads
// tracert.txt into the text buffer 0x100F4C10: the first line copied, each further line appended (NUL included), or
// "..." when it won't open. Then opcode 0x6A goes through SetStateAndWord_guess, secondText, firstText, the
// string at 0x10115574 and the buffer through CopyStringToBuffer_guess and AppendString_guess, and the buffer goes out through
// connection_send_guess (the length is dword 0x1012B918). The handle at 0x101258D4 is closed when set and cleared,
// tracert.txt is deleted, opened and closed again if it still exists, deleted again, and the object at 0x101258D0 is
// freed through FUN_10033800. value is never read.
void __cdecl send_tracert_and_free_guess(DWORD value, char *firstText, char *secondText)
{
    FILE *file;
    char line[84];
    char *text;

    CloseHandle(*(HANDLE *)((char *)g_ptr_101258d0_guess + 0x3C));
    file = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_str_10048648_guess, g_eqmain_fopen_mode_guess);
    if (file != NULL)
    {
        EQ_CDECL(char *, 0x10036050, (char *, int, FILE *))(line, 0x51, file);
        strcpy(g_eqmain_copybuf_guess, line);
        while ((file->_flag & 0x10) == 0)
        {
            strcat(g_eqmain_copybuf_guess, line);
            EQ_CDECL(char *, 0x10036050, (char *, int, FILE *))(line, 0x51, file);
        }
        EQ_CDECL(int, 0x100343B9, (FILE *))(file);
    }
    else
    {
        strcpy(g_eqmain_copybuf_guess, g_str_1004862c_guess);
    }
    EQ_CDECL(void, 0x10024F90, (u16))(0x6a);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(secondText);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(firstText);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10115574_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_copybuf_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    if (g_handle_101258d4_guess != NULL)
    {
        CloseHandle(g_handle_101258d4_guess);
    }
    g_handle_101258d4_guess = NULL;
    DeleteFileA(g_str_10048648_guess);
    file = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(g_str_10048648_guess, g_str_10047f74_guess);
    if (file != NULL)
    {
        EQ_CDECL(int, 0x100343B9, (FILE *))(file);
    }
    DeleteFileA(g_str_10048648_guess);
    EQ_CDECL(void, 0x10033800, (void *))(g_ptr_101258d0_guess);
}

// FUNCTION: EQMAIN 0x10022d50
// __stdcall receive loop; returns the message type handled, 0 when the queue is empty, 0x10000003 on error.
// First checks the connection flags and whether the watched process (g_handle_101258d4) has exited.
// Then takes a queued message (0x10021420), validates it (0x1002de90, 0x1002dd30) and switches on the type id
// read from the message data. Some cases parse lists of names and values into the indexed-value tables.
// Types such as 10-0x10 and 0x24 return the type to the caller.
unsigned int __stdcall dispatch_received_messages_guess(void)
{
    DWORD message_handle;
    int helper_result;
    unsigned short *message_data;
    unsigned int message_type;
    unsigned int wire_value;
    unsigned int lookup_index;
    unsigned int serialized_value;
    unsigned int setting_value;
    unsigned char message_flag;
    unsigned char *encoded_text;
    char *text;
    int item_attribute;
    int item_count;
    int current_item;
    int entry_value;
    short stored_index;
    char short_name[4];
    char item_name[128];

    message_type = 0;
    if ((g_rdp_flag_guess == 0) && (g_connection_pending_1012584c != 0))
    {
        return 0x10000003;
    }
    if (g_handle_101258d4_guess != NULL)
    {
        message_handle = 0;
        g_get_exit_code_process(g_handle_101258d4_guess, &message_handle);
        if (message_handle != 0x103)
        {
            EQ_CDECL(void, 0x10022BA0, (DWORD, char *, char *))(
                message_handle, g_buf_101257c4_guess, g_buf_100f4918_guess);
        }
    }
    message_handle = EQ_CDECL(DWORD, 0x10021420, (DWORD, DWORD))(g_message_queue_10125638, 10);
receive_message:
    if (message_handle == 0)
    {
        return 0;
    }
    current_item = 0;
    helper_result = EQ_CDECL(int, 0x1002DE90, (DWORD))(message_handle);
    if (helper_result == 0)
    {
        goto free_message;
    }
    helper_result = EQ_CDECL(int, 0x1002DD30, (int))(helper_result);
    if ((helper_result == 0) ||
        ((message_data = (unsigned short *)EQ_CDECL(void *, 0x1002DE70, (DWORD))(message_handle)) == NULL))
    {
        EQ_CDECL(void, 0x10028A90, (DWORD))(message_handle);
        return 0x10000003;
    }
    message_type = (unsigned int)*message_data;
    switch (message_type)
    {
    case 6:
    case 0x4e:
        EQ_METHOD(0, void, 0x10025C10, (char *), (char *)g_indexedValuesObject_guess)();
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        item_count = EQ_STDCALL(int, 0x10025120, (void))();
        if (0 < item_count)
        {
            do
            {
                wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
                item_attribute = (int)(char)EQ_STDCALL(unsigned int, 0x100250C0, (void))();
                encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
                strcpy(item_name, text);
                encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
                strcpy(short_name, text);
                if (current_item == 0)
                {
                    strcpy(g_buf_10114d34_guess, item_name);
                }
                EQ_METHOD(5, short, 0x10025CD0, (char *, char *, char *, int, int, int),
                    (char *)g_indexedValuesObject_guess)(
                    item_name, short_name, (int)(char)wire_value, 0, 0);
                stored_index = (short)current_item;
                ((MessageList_guess *)g_indexedValuesObject_guess)->values_by_index[stored_index] = item_attribute;
                current_item = current_item + 1;
            } while (current_item < item_count);
        }
        if (*message_data == 6)
        {
            EQ_CDECL(void, 0x10024F90, (unsigned short))(7);
            EQ_CDECL(void, 0x10024FB0, (unsigned char))(1);
            text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10114d34_guess);
            EQ_CDECL(void, 0x10024FF0, (char *))(text);
            EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(
                g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
            item_count = 0x52;
            EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(
                g_dword_10125848_guess, &item_count, 2, 1, 1);
        }
        break;
    case 7:
        goto process_name_list;
    case 8:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        item_count = EQ_STDCALL(int, 0x10025120, (void))();
        ((MessageList_guess *)g_ptr_101258a8_guess)->refresh_pending = 1;
        if (0 < item_count)
        {
            do
            {
                encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
                strcpy(item_name, text);
                lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
                    (char *)g_ptr_101258a8_guess)(item_name);
                if ((short)lookup_index != -1)
                {
                    EQ_METHOD(1, void, 0x10025ED0, (char *, short),
                        (char *)g_ptr_101258a8_guess)((short)lookup_index);
                }
                item_count = item_count - 1;
            } while (item_count != 0);
        }
        break;
    case 9:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        entry_value = (int)(char)wire_value;
        if ((entry_value != 1) ||
            (((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)wire_value))->value == 1))
        {
            message_flag = 8;
            goto send_chat_text;
        }
        message_flag = 7;
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        EQ_CDECL(void, 0x10022950, (char *, int, unsigned char))(text, entry_value, message_flag);
        message_type = 0;
        break;
    case 10:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        entry_value = (int)(char)wire_value;
        if ((entry_value == 1) &&
            (((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)wire_value))->value != 1))
        {
            helper_result = entry_value;
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            message_flag = (unsigned char)helper_result;
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            EQ_CDECL(void, 0x10022950, (char *, int, unsigned char))(text, entry_value, message_flag);
        }
        else
        {
            message_flag = 6;
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            EQ_CDECL(void, 0x100224B0, (char *, int, unsigned char))(text, entry_value, message_flag);
        }
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_copybuf_guess, text);
        break;
    case 0x0b:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        message_flag = 5;
        entry_value = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        goto send_chat_text;
    case 0x0c:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        if ((char)wire_value == 0)
        {
            EQ_CDECL(void, 0x10002B30, (char *))(g_config_label_10055710);
        }
        else
        {
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            strcpy(g_buf_10114d34_guess, text);
            EQ_CDECL(void, 0x10024F90, (unsigned short))(7);
            EQ_CDECL(void, 0x10024FB0, (unsigned char))(0);
            text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10114d34_guess);
            EQ_CDECL(void, 0x10024FF0, (char *))(text);
            EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, DWORD, DWORD))(
                g_dword_10125848_guess, g_buf_10125914_guess, g_dword_1012b918_guess, 1, 1);
        }
        break;
    case 0x0d:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        message_flag = (unsigned char)wire_value;
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(item_name, text);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_indexedValuesObject_guess)(item_name);
        if ((short)lookup_index == -1)
        {
            EQ_METHOD(5, short, 0x10025CD0, (char *, char *, char *, int, int, int),
                (char *)g_indexedValuesObject_guess)(item_name, (char *)0, (int)(char)message_flag, 0, 0);
        }
        break;
    case 0x0e:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(item_name, text);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_indexedValuesObject_guess)(item_name);
        if ((short)lookup_index != -1)
        {
            EQ_METHOD(1, void, 0x10025ED0, (char *, short),
                (char *)g_indexedValuesObject_guess)((short)lookup_index);
        }
        break;
    case 0x0f:
        message_flag = 5;
        entry_value = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        EQ_CDECL(void, 0x100224B0, (char *, int, unsigned char))(
            g_config_label_100556ec, entry_value, message_flag);
        message_type = 0;
        break;
    case 0x10:
        EQ_METHOD(0, void, 0x10025C10, (char *), (char *)g_ptr_101258a8_guess)();
        goto process_name_list;
    case 0x24:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        message_flag = 5;
        entry_value = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        goto send_chat_text;
    send_chat_text:
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        EQ_CDECL(void, 0x100224B0, (char *, int, unsigned char))(text, entry_value, message_flag);
        message_type = 0;
        break;
    case 0x27:
        EQ_CDECL(void, 0x10024900, (unsigned short *))(message_data);
        message_type = 0;
        break;
    case 0x28:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(item_name, text);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_ptr_101258a8_guess)(item_name);
        if ((short)lookup_index != -1)
        {
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            strcpy(((MessageList_guess *)g_ptr_101258a8_guess)->names[(short)lookup_index], text);
        }
        break;
    case 0x2b:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        message_flag = (unsigned char)wire_value;
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(item_name, text);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_indexedValuesObject_guess)(item_name);
        if ((short)lookup_index == -1)
        {
            EQ_METHOD(5, short, 0x10025CD0, (char *, char *, char *, int, int, int),
                (char *)g_indexedValuesObject_guess)(item_name, (char *)0, (int)(char)message_flag, 0, 0);
        }
        else
        {
            EQ_METHOD(4, void, 0x10025E50, (char *, short, char *, char *, int),
                (char *)g_indexedValuesObject_guess)((short)lookup_index, item_name, (char *)0, (int)(char)message_flag);
        }
        break;
    case 0x2c:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_name_guess, text);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_str_104cb0_guess, text);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_contact_names_10104cc4, text);
        break;
    case 0x30:
    case 0x58:
    case 0x69:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_copybuf_guess, text);
        break;
    case 0x47:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_copybuf_guess, text);
        if (g_eqmain_copybuf_guess[0] == '\0')
        {
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            strcpy(g_eqmain_copy_cec_guess, text);
        }
        break;
    case 0x49:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_sel_name_guess, text);
        break;
    case 0x4c:
        EQ_CDECL(void, 0x10023C40, (unsigned short *))(message_data);
        message_type = 0;
        break;
    case 0x52:
        EQ_CDECL(void, 0x10001230, (unsigned short *))(message_data);
        break;
    case 0x57:
        EQ_CDECL(void, 0x10018990, (unsigned short *))(message_data);
        break;
    case 0x60:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        g_choice_count_guess = EQ_STDCALL(int, 0x10025120, (void))();
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_choice_text_guess, text);
        break;
    case 0x61:
        EQ_CDECL(void, 0x10023E00, (unsigned short *))(message_data);
        break;
    case 0x75:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        serialized_value = EQ_STDCALL(unsigned int, 0x10025120, (void))();
        ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value = (int)serialized_value;
        helper_result = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        text = g_language_label_10048098;
        EQ_METHOD(2, void, 0x10020CD0, (void *, char *, int),
            (void *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))(text, helper_result);
        EQ_METHOD(0, void, 0x10026FD0, (int *), g_language_state_100556e8)();
        break;
    case 0x76:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_eqmain_copybuf_guess, text);
        break;
    case 0x78:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(item_name, text);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_indexedValuesObject_guess)(item_name);
        if ((short)lookup_index != -1)
        {
            ((MessageList_guess *)g_indexedValuesObject_guess)->values_by_index[(short)lookup_index] =
                (int)(char)wire_value;
        }
        break;
    case 0x7e:
        EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
        message_flag = 5;
        entry_value = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        encoded_text = EQ_STDCALL(unsigned char *, 0x100251C0, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        EQ_CDECL(void, 0x100224B0, (char *, int, unsigned char))(text, entry_value, message_flag);
        message_type = 0;
        break;
    }
free_message:
    EQ_CDECL(void, 0x10028A90, (DWORD))(message_handle);
    if (message_type != 0)
    {
        return message_type;
    }
    message_handle = EQ_CDECL(DWORD, 0x10021420, (DWORD, DWORD))(g_message_queue_10125638, 10);
    goto receive_message;

process_name_list:
    EQ_CDECL(void, 0x100250A0, (unsigned short *))(message_data);
    item_count = EQ_STDCALL(int, 0x10025120, (void))();
    if (*message_data == 0x10)
    {
        encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
        text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
        strcpy(g_buf_101255f8_guess, text);
        strcpy(g_buf_10114d34_guess, g_buf_101255f8_guess);
        lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
            (char *)g_indexedValuesObject_guess)(g_buf_101255f8_guess);
        ((MessageList_guess *)g_indexedValuesObject_guess)->selected_index = (short)lookup_index;
        strcpy(short_name,
            ((MessageList_guess *)g_indexedValuesObject_guess)->names[(short)lookup_index + 100]);
        EQ_CDECL(int, 0x10034117, (char *, char *, char *, char *))(
            item_name, g_format_10055718, short_name, g_buf_101255f8_guess);
        entry_value = ((ConfigEntry_guess *)EQ_CDECL(void *, 0x10020EF0, (char))((char)1))->value;
        EQ_CDECL(void, 0x100224B0, (char *, int, unsigned char))(item_name, entry_value, (unsigned char)5);
    }
    ((MessageList_guess *)g_ptr_101258a8_guess)->refresh_pending = 1;
    if (0 < item_count)
    {
        do
        {
            current_item = item_count;
            wire_value = EQ_STDCALL(unsigned int, 0x100250C0, (void))();
            serialized_value = EQ_STDCALL(unsigned int, 0x10025120, (void))();
            encoded_text = EQ_STDCALL(unsigned char *, 0x10025150, (void))();
            text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded_text);
            strcpy(item_name, text);
            lookup_index = EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *),
                (char *)g_ptr_101258a8_guess)(item_name);
            if ((short)lookup_index == -1)
            {
                EQ_METHOD(5, short, 0x10025CD0, (char *, char *, char *, int, int, int),
                    (char *)g_ptr_101258a8_guess)(item_name, (char *)0, 0, (int)(char)wire_value,
                    (int)serialized_value);
            }
            else
            {
                EQ_METHOD(6, void, 0x10025DB0, (char *, short, char *, char *, int, int, int),
                    (char *)g_ptr_101258a8_guess)((short)lookup_index, item_name, (char *)0, 0,
                    (int)(char)wire_value, (int)serialized_value);
            }
            item_count = current_item - 1;
        } while (current_item - 1 != 0);
        item_attribute = 0;
    }
    goto free_message;
}
