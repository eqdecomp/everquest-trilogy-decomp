// unit_10016480.cpp - eqmain.dll, the address run 0x10016480..0x100184D0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "trilogy/eqaddr.h"

struct EQMainOutputContext_guess
{
    int unused;
    unsigned int line_target;
};

struct EQMainChatEvent_guess
{
    int type;
    int unused;
    union
    {
        int target;
        int key;
    } value;
};

struct EQMainChatControl_guess
{
    char unused_00[0x2c];
    int selected_row;
    char unused_30[4];
    int selected_entry;
    char unused_38[0x30];
    int item_count;
};

struct EQMainChatQueueNode_guess
{
    int unused_00[2];
    EQMainChatQueueNode_guess *next;
};

struct EQMainChatQueue_guess
{
    int unused_00[7];
    EQMainChatQueueNode_guess *first;
    int unused_20[2];
    EQMainChatQueueNode_guess *selected;
};

struct EQMainChatDialog_guess
{
    char unused_00[0x1c];
    int input_length;
    char *input_line;
};

struct EQMainChatEntry_guess
{
    int unused_00;
    char *text;
};

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
#define g_eqmain_strftime_format_guess  EQ_GLOBAL(char *, 0x10055958)   // g_eqmain_strftime_format_guess: the strftime format string
#define g_eqmain_log_file_guess         EQ_GLOBAL(FILE *, 0x1004F884)   // g_eqmain_log_file_guess: the FILE * the line goes to
#define eqmain_fn_1003396b_guess        EQ_CDECL(char *, 0x1003396B, (char *))                              // __strdup
#define eqmain_fn_10033670_guess        EQ_CDECL(char *, 0x10033670, (char *, int))                        // _strchr
#define eqmain_fn_10033568_guess        EQ_CDECL(void, 0x10033568, (void *))                                // _free
#define eqmain_fn_10034012_guess        EQ_CDECL(int, 0x10034012, (FILE *, const char *, ...))             // _fprintf
#define eqmain_fn_10035112_guess        EQ_CDECL(size_t, 0x10035112, (char *, size_t, char *, struct tm *)) // _strftime
#define eqmain_fn_100359c4_guess        EQ_CDECL(struct tm *, 0x100359C4, (time_t *))                       // _localtime
#define eqmain_fn_10035b24_guess        EQ_CDECL(time_t, 0x10035B24, (time_t *))                            // _time

// eqmain.dll's globals and functions used by EQMain_OpenChatLog_guess (this image's VAs).
#define eqmain_fn_1003407f_guess        EQ_CDECL(FILE *, 0x1003407F, (const char *, const char *))        // _fopen (a 19-byte library wrapper)
#define eqmain_fn_10020ef0_guess        EQ_CDECL(int, 0x10020EF0, (char))                                 // returns an object; its dword at +4 is read
#define eqmain_fn_100224b0_guess        EQ_CDECL(void, 0x100224B0, (char *, int, char))                   // writes one line; strchr on its first argument
#define g_eqmain_open_msg_ok_guess      EQ_GLOBAL(char *, 0x10055944)   // g_eqmain_open_msg_ok_guess: the first string when fopen succeeded
#define g_eqmain_open_msg_fail_guess    EQ_GLOBAL(char *, 0x1005594C)   // g_eqmain_open_msg_fail_guess: the first string when fopen failed

// eqmain.dll's globals and functions used by EQMain_ToggleChatLog_guess (this image's VAs).
#define eqmain_fn_100343b9_guess        EQ_CDECL(int, 0x100343B9, (FILE *))                               // _fclose (49 bytes, a library wrapper)
#define g_eqmain_close_msg_guess        EQ_GLOBAL(char *, 0x10055948)   // g_eqmain_close_msg_guess: the first string when the log is closed

// eqmain.dll's globals and functions used by EQMain_FlushBufferedText_guess (this image's VAs).
#define eqmain_fn_10015050_guess        EQ_CDECL(short, 0x10015050, (int))                                // returns a short (AX); the caller sign-extends it
#define eqmain_fn_10001ff0_guess        EQ_CDECL(void, 0x10001FF0, (int *, char *, const char *, const char *)) // 703 bytes; its result is not read
#define g_eqmain_state_guess            EQ_GLOBAL(int, 0x1004CF60)      // g_eqmain_cf60_guess: the state; 1, 3 and 5 take the EQMain_RunSlotScreen_guess path
#define g_eqmain_buf_guess              EQ_ARRAY(char, 0x1004CF68)      // g_eqmain_buf_guess: the text buffer; empty means nothing to do
#define g_eqmain_obj_guess              EQ_GLOBAL(int *, 0x1004CEB8)    // g_eqmain_ceb8_guess: the first argument of eqmainRunDialog_guess
#define g_eqmain_text_guess             EQ_GLOBAL(char *, 0x10055888)   // g_str_888_guess: the third argument of eqmainRunDialog_guess
#define g_eqmain_busy_guess             EQ_GLOBAL(char, 0x1004CE78)     // g_eqmain_flag_ce78_guess: set to 1 while eqmainRunDialog_guess runs

// FUNCTION: EQMAIN 0x10016480
// Submits nonempty front-end text for handling, except in states 1, 3 and 5.
// __stdcall, no arguments (callee pops 0 bytes); returns a value. If the state g_eqmain_cf60_guess is 1, 3 or 5, returns
// EQMain_RunSlotScreen_guess(state) as a sign-extended short. Otherwise, if the text at g_eqmain_buf_guess is not empty, sets the busy flag
// g_eqmain_flag_ce78_guess, calls eqmainRunDialog_guess(g_eqmain_ceb8_guess, buffer, g_str_888_guess, NULL) and clears the flag. Returns 1.
int __stdcall EQMain_FlushBufferedText_guess(void)
{
    short result;

    switch (g_eqmain_state_guess)
    {
    case 1:
    case 3:
    case 5:
        result = eqmain_fn_10015050_guess(g_eqmain_state_guess);
        return result;
    }
    if (strlen(g_eqmain_buf_guess) != 0)
    {
        g_eqmain_busy_guess = 1;
        eqmain_fn_10001ff0_guess(g_eqmain_obj_guess, g_eqmain_buf_guess, g_eqmain_text_guess, NULL);
        g_eqmain_busy_guess = 0;
    }
    return 1;
}

// FUNCTION: EQMAIN 0x100164F0
// Opens chat.txt when chat logging is closed and reports whether it opened.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. If the log FILE * at 0x1004F884 is NULL, opens
// "..." (0x10047FF4) with the mode string at 0x10048000 into it. Opened: EqfeCfgSetActive_guess(1) gives an object and
// BuildTaggedText_guess gets g_eqmain_open_msg_ok_guess, the object's dword at +4, and 5. Not opened: the same with g_eqmain_open_msg_fail_guess.
void __stdcall EQMain_OpenChatLog_guess(void)
{
    if (g_eqmain_log_file_guess == 0)
    {
        g_eqmain_log_file_guess = eqmain_fn_1003407f_guess(EQ_ARRAY(char, 0x10047FF4), EQ_ARRAY(char, 0x10048000));
        if (g_eqmain_log_file_guess != 0)
        {
            eqmain_fn_100224b0_guess(g_eqmain_open_msg_ok_guess,
                                     *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
            return;
        }
        eqmain_fn_100224b0_guess(g_eqmain_open_msg_fail_guess,
                                 *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
    }
    return;
}

// FUNCTION: EQMAIN 0x10016550
// Turns chat logging on or off and reports the result.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Toggles the chat log. If the log FILE * at 0x1004F884
// is NULL, opens "..." (0x10047FF4) with the mode string at 0x10048000 into it: opened, EqfeCfgSetActive_guess(1) gives an
// object and BuildTaggedText_guess gets g_eqmain_open_msg_ok_guess, the object's dword at +4, and 5; not opened: the same with g_eqmain_open_msg_fail_guess.
// If the log is open, _fclose closes it, the FILE * is set to NULL and BuildTaggedText_guess gets g_eqmain_close_msg_guess with the same
// object dword and 5.
void __stdcall EQMain_ToggleChatLog_guess(void)
{
    if (g_eqmain_log_file_guess == 0)
    {
        g_eqmain_log_file_guess = eqmain_fn_1003407f_guess(EQ_ARRAY(char, 0x10047FF4), EQ_ARRAY(char, 0x10048000));
        if (g_eqmain_log_file_guess != 0)
        {
            eqmain_fn_100224b0_guess(g_eqmain_open_msg_ok_guess, *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
            return;
        }
        eqmain_fn_100224b0_guess(g_eqmain_open_msg_fail_guess, *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
        return;
    }
    eqmain_fn_100343b9_guess(g_eqmain_log_file_guess);
    g_eqmain_log_file_guess = 0;
    eqmain_fn_100224b0_guess(g_eqmain_close_msg_guess, *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
}

// FUNCTION: EQMAIN 0x100165F0
// Writes a time-stamped chat line to the active log file.
// __cdecl, one stack argument (text); its RET pops 0 bytes; returns nothing. Works on a strdup copy: the copy is
// searched for '<' and each "<X>" found is blanked to spaces while at least one more character follows it
// (strlen > 3) and the third character is '>'. The first '<' that fails either test ends the loop (break), so
// the tags after it stay. Then writes "<strftime(g_eqmain_strftime_format)> <copy>\n" to the FILE * at 0x1004F884
// (no NULL check) and frees the copy.
void __cdecl EQMain_LogStampedLine_guess(char *text)
{
    char *_Str;
    char *_Str_00;
    struct tm *localTime;
    time_t currentTime;
    time_t timeArgument;
    char timestampText[128];

    _Str = eqmain_fn_1003396b_guess(text);
    for (_Str_00 = eqmain_fn_10033670_guess(_Str, 0x3c); _Str_00 != 0;
         _Str_00 = eqmain_fn_10033670_guess(_Str_00, 0x3c))
    {
        if (strlen(_Str_00) <= 3 || _Str_00[2] != '>')
            break;
        _Str_00[0] = ' ';
        _Str_00[1] = ' ';
        _Str_00[2] = ' ';
    }
    currentTime = eqmain_fn_10035b24_guess(0);
    timeArgument = currentTime;
    localTime = eqmain_fn_100359c4_guess(&timeArgument);
    eqmain_fn_10035112_guess(timestampText, 0x80, g_eqmain_strftime_format_guess, localTime);
    eqmain_fn_10034012_guess(g_eqmain_log_file_guess, "%s %s\n", timestampText, _Str);
    eqmain_fn_10033568_guess(_Str);
    return;
}

// FUNCTION: EQMAIN 0x100166B0
// Writes a time-stamped entry to journal.txt when journaling is available.
// @early-stop: VC6 merges two cdecl stack cleanups and schedules the loop's strchr pointer push before the final tag store.
// __cdecl, one stack argument (text); its RET pops 0 bytes; returns nothing. If the journal FILE * (g_eqmain_journal_file_guess) is
// set, sinks the message at g_eqmain_journal_busy_msg_guess with the word at +4 of EqfeCfgSetActive_guess(1) and 5, then returns. Else opens
// "..." with the mode at g_eqmain_journal_mode_guess; if that fails it sinks the message at g_eqmain_journal_fail_msg_guess the same way and
// returns. Else blanks the "<X>" tags of text in place (LogStampedLine's test, on the text itself, no copy), writes
// "<strftime(g_eqmain_strftime_format_guess)> <text>\n" to the journal (no NULL check), sinks the sprintf'd line
// (g_eqmain_line_format_guess) with 4, closes the journal and clears g_eqmain_journal_file_guess.
#define g_eqmain_journal_file_guess     EQ_GLOBAL(FILE *, 0x1004F888)   // g_eqmain_journal_file_guess: the journal FILE *, NULL until opened
#define g_eqmain_journal_mode_guess     EQ_ARRAY(char, 0x10048000)      // g_eqmain_journal_mode_guess: the fopen mode string (no Ghidra label)
#define g_eqmain_journal_busy_msg_guess EQ_GLOBAL(char *, 0x10055954)  // g_eqmain_journal_busy_msg_guess: the first string when the journal is open
#define g_eqmain_journal_fail_msg_guess EQ_GLOBAL(char *, 0x10055950)  // g_eqmain_journal_fail_msg_guess: the first string when journal.txt won't open
#define g_eqmain_line_format_guess      EQ_GLOBAL(char *, 0x1005595C)   // g_eqmain_line_format_guess: the sprintf format for the sunk line
#define eqmain_fn_10034117_guess        EQ_CDECL(int, 0x10034117, (char *, const char *, ...))             // _sprintf

void __cdecl EQMain_JournalLine_guess(char *text)
{
    char *_Str;
    time_t timeArgument;
    char timestampText[128];
    char formattedLine[508];
    struct tm *localTime;

    if (g_eqmain_journal_file_guess == 0)
    {
        g_eqmain_journal_file_guess = eqmain_fn_1003407f_guess(EQ_ARRAY(char, 0x1004800C), g_eqmain_journal_mode_guess);
        if (g_eqmain_journal_file_guess == 0)
        {
            eqmain_fn_100224b0_guess(g_eqmain_journal_fail_msg_guess, *(int *)((char *)eqmain_fn_10020ef0_guess(1) + 4), 5);
            return;
        }
        for (_Str = eqmain_fn_10033670_guess(text, 0x3c); _Str != 0;
             _Str = eqmain_fn_10033670_guess(_Str, 0x3c))
        {
            if (strlen(_Str) <= 3 || _Str[2] != '>')
                break;
            _Str[0] = ' ';
            _Str[1] = ' ';
            _Str[2] = ' ';
        }
        timeArgument = eqmain_fn_10035b24_guess(0);
        localTime = eqmain_fn_100359c4_guess(&timeArgument);
        eqmain_fn_10035112_guess(timestampText, 0x80, g_eqmain_strftime_format_guess, localTime);
        eqmain_fn_10034012_guess(g_eqmain_journal_file_guess, EQ_ARRAY(char, 0x10048004), timestampText, text);
        eqmain_fn_10034117_guess(formattedLine, g_eqmain_line_format_guess, timestampText, text);
        eqmain_fn_100224b0_guess(formattedLine, *(int *)((char *)eqmain_fn_10020ef0_guess(1) + 4), 4);
        eqmain_fn_100343b9_guess(g_eqmain_journal_file_guess);
        g_eqmain_journal_file_guess = 0;
        return;
    }
    else
    {
        eqmain_fn_100224b0_guess(g_eqmain_journal_busy_msg_guess, *(int *)((char *)eqmain_fn_10020ef0_guess(1) + 4), 5);
        return;
    }
}

// FUNCTION: EQMAIN 0x10016820
// Formats the current time as a front-end message.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Takes the current time (_time, then _localtime) and
// formats it with g_eqmain_strftime_format into a 128-byte buffer, sprintf's that into a second 128-byte buffer with
// the format at g_eqmain_time_format_guess, then passes the second buffer to BuildTaggedText_guess with the word at +4 of the EqfeCfgSetActive_guess(1)
// result and 4 (the third argument is pushed before the call to EqfeCfgSetActive_guess, as VC6 evaluates arguments right to left).
// @early-stop: VC6 folds the _strftime and _sprintf arg pops into one add esp,0x24 (original: 0x18 after _strftime, 0xc after _sprintf).
#define g_eqmain_time_format_guess      EQ_ARRAY(char, 0x10048018)                                         // g_eqmain_time_format_guess: the sprintf format for the time text
#define eqmain_fn_10034117_guess        EQ_CDECL(int, 0x10034117, (char *, const char *, ...))             // _sprintf
#define eqmain_fn_10020ef0_guess        EQ_CDECL(int, 0x10020EF0, (char))                                  // EqfeCfgSetActive_guess: returns a pointer whose +4 word is printed
#define eqmain_fn_100224b0_guess        EQ_CDECL(void, 0x100224B0, (char *, unsigned int, unsigned char))  // BuildTaggedText_guess: the sink for the line

void __stdcall EQMain_PrintTimestamp_guess(void)
{
    time_t timeArgument;
    char timestampText[128];
    char formattedLine[128];
    time_t currentTime;
    struct tm *localTime;

    currentTime = eqmain_fn_10035b24_guess(0);
    timeArgument = currentTime;
    localTime = eqmain_fn_100359c4_guess(&timeArgument);
    eqmain_fn_10035112_guess(timestampText, 0x80, g_eqmain_strftime_format_guess, localTime);
    eqmain_fn_10034117_guess(formattedLine, g_eqmain_time_format_guess, timestampText);
    eqmain_fn_100224b0_guess(formattedLine, *(unsigned int *)(eqmain_fn_10020ef0_guess(1) + 4), 4);
    return;
}

// FUNCTION: EQMAIN 0x100168A0
// Copies the current shared string into the chat screen's repeated text slots.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Each pass copies the NUL-terminated string at
// g_eqmain_drive_base_guess (rescanned every pass; strlen + 1 bytes) to the slot pointer, then steps the pointer by 0xFE (254-byte
// slots). Passes repeat while the stepped pointer is below g_chat_list_control_guess, so ten slots from g_chat_page_texts_guess and at least
// one copy. Then clears the two words g_eqmain_line_count_guess and g_chat_page_index_guess.
#define g_eqmain_slot_table_guess   EQ_GLOBAL(char, 0x1004ECF0)            // g_chat_page_texts_guess: the first slot (undefined4 in Ghidra)
#define g_eqmain_slot_source_guess  EQ_GLOBAL(char, 0x1004CE40)            // g_eqmain_drive_base_guess: the string copied into each slot
#define g_eqmain_slot_end_guess     EQ_GLOBAL(int, 0x1004F6DC)             // g_chat_list_control_guess: the table end (its address is the bound)
#define g_eqmain_slot_word_a_guess  EQ_GLOBAL(unsigned short, 0x1004F87C)  // g_eqmain_line_count_guess: cleared on exit
#define g_eqmain_slot_word_b_guess  EQ_GLOBAL(unsigned short, 0x1004F87E)  // g_chat_page_index_guess: cleared on exit

void __stdcall EQMain_FillStringSlots_guess(void)
{
    char *dst;
    char *src;

    dst = &g_eqmain_slot_table_guess;
    do
    {
        src = &g_eqmain_slot_source_guess;
        strcpy(dst, src);
        dst += 0xfe;
    } while ((int)dst < (int)&g_eqmain_slot_end_guess);
    g_eqmain_slot_word_a_guess = 0;
    g_eqmain_slot_word_b_guess = 0;
    return;
}

// FUNCTION: EQMAIN 0x100168F0
// Keeps up to ten distinct recent strings in the text history.
// __cdecl, one argument (the string); callee pops 0 bytes; returns nothing. Compares the string with the slot at the
// signed index g_eqmain_table_index_guess (20-byte slots from g_eqmain_table_base_guess; unsigned bytes, up to the first difference or the NUL).
// Same: returns. Different: if the signed count g_chat_page_enabled_guess is below 10, the string (strlen + 1 bytes) goes to slot
// count and the count goes up by one; else slots 1..9 each move down one slot (strlen + 1 bytes each) and the string
// goes to slot 9, read after the shift. Then g_eqmain_table_index_guess and g_eqmain_table_cursor_guess both take the low word of count - 1.
// @early-stop: top count loads a dword (VC6 merges it with the tail dword of the same symbol); original has two globals.
#define g_eqmain_table_base_guess   EQ_GLOBAL(int, 0x1004EC24)     // g_eqmain_table_base_guess: the dword table
#define g_eqmain_table_index_guess  EQ_GLOBAL(short, 0x1004F882)   // g_eqmain_table_index_guess: the signed index
#define g_eqmain_table_cursor_guess EQ_GLOBAL(short, 0x10047FF0)   // g_eqmain_table_cursor_guess: the signed cursor, stepped per call
#define g_eqmain_table_count_guess  EQ_GLOBAL(short, 0x1004F880)   // g_chat_page_enabled_guess: the entries in use (signed word)
#define g_eqmain_table_pair_guess   EQ_GLOBAL(int, 0x1004F880)     // g_chat_page_enabled_guess read as a dword (count low, index high)
#define g_eqmain_table_slot1_guess  EQ_GLOBAL(char, 0x1004EC38)    // g_eqmain_table_slot1_guess: slot 1, the first slot the shift reads
#define g_eqmain_table_slot9_guess  EQ_GLOBAL(char, 0x1004ECD8)    // g_eqmain_table_slot9_guess: slot 9, the last slot
#define g_eqmain_table_end_guess    EQ_GLOBAL(int, 0x1004ECEC)     // g_chat_view_guess: the table end (its address is the bound)

void __cdecl EQMain_PushHistoryString_guess(char *str)
{
    char *dst;
    char *src;

    if (strcmp((char *)(&g_eqmain_table_base_guess + g_eqmain_table_index_guess * 5), str) != 0)
    {
        short count = g_eqmain_table_count_guess;
        if (count < 10)
        {
            dst = (char *)(&g_eqmain_table_base_guess + count * 5);
            count++;
            g_eqmain_table_count_guess = count;
            strcpy(dst, str);
        }
        else
        {
            src = &g_eqmain_table_slot1_guess;
            do
            {
                dst = src - 0x14;
                strcpy(dst, src);
                src += 0x14;
            } while ((int)src < (int)&g_eqmain_table_end_guess);
            strcpy(&g_eqmain_table_slot9_guess, str);
        }
        g_eqmain_table_cursor_guess = g_eqmain_table_index_guess = (short)(g_eqmain_table_pair_guess - 1);
    }
    return;
}

// FUNCTION: EQMAIN 0x100169F0
// Returns the currently selected text-history entry.
// __stdcall, no arguments (callee pops 0 bytes). Returns &g_eqmain_table_base_guess + g_eqmain_table_index_guess * 5: the index is a signed
// word, and the base is an undefined4 pointer, so each index step is 20 bytes (five dwords).
#define g_eqmain_table_base_guess   EQ_GLOBAL(int, 0x1004EC24)     // g_eqmain_table_base_guess: the dword table
#define g_eqmain_table_index_guess  EQ_GLOBAL(short, 0x1004F882)   // g_eqmain_table_index_guess: the signed index

int * __stdcall EQMain_TableEntry_guess(void)
{
    return &g_eqmain_table_base_guess + g_eqmain_table_index_guess * 5;
}

// FUNCTION: EQMAIN 0x10016A10
// Moves backward through text history and returns the next entry.
// __stdcall, no arguments (callee pops 0 bytes). Steps the signed word cursor g_eqmain_table_cursor_guess down by one; a negative
// cursor first restarts from g_eqmain_table_index_guess (only the register copy: the store is the decremented value). Returns
// &g_eqmain_table_base_guess + index * 5 for the index read before the decrement (20-byte steps, as above).
#define g_eqmain_table_cursor_guess  EQ_GLOBAL(short, 0x10047FF0)   // g_eqmain_table_cursor_guess: the signed cursor, stepped per call

int * __stdcall EQMain_TableEntryStep_guess(void)
{
    if (g_eqmain_table_cursor_guess < 0)
        g_eqmain_table_cursor_guess = g_eqmain_table_index_guess;
    return &g_eqmain_table_base_guess + g_eqmain_table_cursor_guess-- * 5;
}

// FUNCTION: EQMAIN 0x10016A40
// Rebuilds the front-end name list and restores its selection and scroll position.
// __stdcall, no arguments (callee pops 0 bytes). Does nothing when the word count at +0x36b0 of the name table g_eqmain_name_ptr_guess
// is 0. Else RemoveAllOwnedRecords_guess is called on the list g_chat_other_list_guess, then each of the count 64-byte names (at +i * 0x40) is added
// in order with EnqueueNamedRecord_guess, colored by the int tag at +0x3200 + i * 4 (1 -> 0x1818f0, 2 -> 0x646464, else 0). The old top
// row (+0x2c, 0 when out of range) and the row of the name DAT_101255f8 (FindRecordIndexByName_guess gives a short, -1 -> 0, stored at
// +0x34) are kept. The list scrolls to the selected row when it is the last one or g_eqmain_list_flag_guess is set (that byte is then
// cleared). The scrollbar g_chat_other_control_guess gets +0x64 = (+0x24 - +0x30) + 1 and +0x68 = the top row; RefreshSlider_guess and
// DrawNamedRows_guess then refresh the two.
#define g_eqmain_name_table_guess  EQ_GLOBAL(int, 0x101258A4)      // g_eqmain_name_ptr_guess: the name table (count at +0x36b0, tags at +0x3200)
#define g_eqmain_name_find_guess   EQ_GLOBAL(char, 0x101255F8)     // DAT_101255f8: the name whose row gets selected
#define g_eqmain_list_guess        EQ_GLOBAL(int, 0x1004F6E8)      // g_chat_other_list_guess: the list (+0x24, +0x2c top, +0x30, +0x34 selected)
#define g_eqmain_list_flag_guess   EQ_GLOBAL(char, 0x10047FF2)     // g_eqmain_list_flag_guess: forces the scroll when set; cleared here
#define g_eqmain_scrollbar_guess   EQ_GLOBAL(int, 0x1004D244)      // g_chat_other_control_guess: the scrollbar (+0x64 visible rows, +0x68 top)
#define eqmain_fn_10026040_guess(object) EQ_METHOD(1, unsigned int, 0x10026040, (int, char *), (object))  // __thiscall, this = the name table: row of the name, -1 when absent
#define eqmain_fn_10005d80_guess   EQ_CDECL(void, 0x10005D80, (int))                          // __cdecl, first call on the list
#define eqmain_fn_10005cc0_guess   EQ_CDECL(void *, 0x10005CC0, (int, int, char *, int, int)) // __cdecl, adds one row (malloc 0x20, strdup of the name)
#define eqmain_fn_10005e70_guess   EQ_CDECL(void, 0x10005E70, (int, int))                     // __cdecl, scrolls the list to a row (decompiled body: return)
#define eqmain_fn_100067e0_guess   EQ_CDECL(void, 0x100067E0, (int))                          // __cdecl, refreshes the view (as at line 523)
#define eqmain_fn_10005a00_guess   EQ_CDECL(void, 0x10005A00, (int))                          // __cdecl, refreshes the list

void __stdcall EQMain_RebuildNameList_guess(void)
{
    unsigned short count;
    unsigned int found;
    int top;
    int sel;
    int i;
    int idx;
    int tag;

    count = *(unsigned short *)(g_eqmain_name_table_guess + 0x36B0);
    if (count != 0) {
        found = eqmain_fn_10026040_guess(g_eqmain_name_table_guess)( &g_eqmain_name_find_guess);
        sel = (short)found;
        if (sel == -1)
            sel = 0;
        top = *(int *)(g_eqmain_list_guess + 0x2C);
        if (top < 0 || top >= (int)(unsigned int)count)
            top = 0;
        eqmain_fn_10005d80_guess(g_eqmain_list_guess);
        for (i = 0; i < (int)(unsigned int)count; i++) {
            idx = (short)i;
            tag = *(int *)(g_eqmain_name_table_guess + idx * 4 + 0x3200);
            switch (tag) {
            case 1:
                eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0x70, (char *)(g_eqmain_name_table_guess + idx * 0x40), 0, 0x1818F0);
                break;
            case 2:
                eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0x70, (char *)(g_eqmain_name_table_guess + idx * 0x40), 0, 0x646464);
                break;
            default:
                eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0x70, (char *)(g_eqmain_name_table_guess + idx * 0x40), 0, 0);
                break;
            }
        }
        *(int *)(g_eqmain_list_guess + 0x2C) = top;
        *(int *)(g_eqmain_list_guess + 0x34) = sel;
        if (*(int *)(g_eqmain_list_guess + 0x34) == *(int *)(g_eqmain_list_guess + 0x2C) - 1 || g_eqmain_list_flag_guess != 0)
            eqmain_fn_10005e70_guess(g_eqmain_list_guess, sel);
        g_eqmain_list_flag_guess = 0;
        *(int *)(g_eqmain_scrollbar_guess + 0x64) = (*(int *)(g_eqmain_list_guess + 0x24) - *(int *)(g_eqmain_list_guess + 0x30)) + 1;
        *(int *)(g_eqmain_scrollbar_guess + 0x68) = *(int *)(g_eqmain_list_guess + 0x2C);
        eqmain_fn_100067e0_guess(g_eqmain_scrollbar_guess);
        eqmain_fn_10005a00_guess(g_eqmain_list_guess);
    }
    return;
}

// FUNCTION: EQMAIN 0x10016B90
// Orders names alphabetically while keeping each row's tag attached.
// __cdecl, one short argument (callee pops 0 bytes). Bubble sort of count 64-byte name strings at g_eqmain_cat_names_guess into
// ascending __strcmpi order; each swap moves the word tag at the same index (g_eqmain_cat_index_guess) with its string. A swap is
// three strcpy calls through a 64-byte buffer, so only the bytes up to each NUL move. count - 1 passes, none when
// count <= 1. The pass count and the inner bound are the int last = count - 1; the inner counter is the argument's own
// stack slot (count), i its int copy.
#define g_eqmain_name_str_guess       EQ_GLOBAL(char, 0x1004D310)              // g_eqmain_cat_names_guess: the 64-byte name strings
#define g_eqmain_name_str_next_guess  EQ_GLOBAL(char, 0x1004D350)              // g_eqmain_name_str_next_guess: the same strings, index + 1
#define g_eqmain_name_tag_guess       EQ_GLOBAL(unsigned short, 0x1004D248)    // g_eqmain_cat_index_guess: the tags moved with the strings
#define g_eqmain_name_tag_next_guess  EQ_GLOBAL(unsigned short, 0x1004D24A)    // g_eqmain_name_tag_next_guess: the same tags, index + 1
#define eqmain_fn_10033730_guess      EQ_CDECL(int, 0x10033730, (char *, char *))   // __strcmpi

void EQMain_SortNames_guess(short count)
{
    short pass;
    int last;
    int i;
    unsigned short tag;
    char *str;
    char *str_next;

    pass = 0;
    /* fake match: buf declared after the first statement for the original's stack layout */
    char buf[64];
    last = count - 1;
    if (last > 0)
    {
        do
        {
            i = 0;
            count = 0;
            if (0 < last)
            {
                do
                {
                    str_next = &g_eqmain_name_str_next_guess + i * 64;
                    str = &g_eqmain_name_str_guess + i * 64;
                    if (eqmain_fn_10033730_guess(str, str_next) > 0)
                    {
                        tag = (&g_eqmain_name_tag_guess)[i];
                        (&g_eqmain_name_tag_guess)[i] = (&g_eqmain_name_tag_next_guess)[i];
                        (&g_eqmain_name_tag_next_guess)[i] = tag;
                        strcpy(buf, str);
                        strcpy(str, str_next);
                        strcpy(str_next, buf);
                    }
                    count = count + 1;
                    i = count;
                } while (i < last);
            }
            pass = pass + 1;
        } while (pass < last);
    }
}

// FUNCTION: EQMAIN 0x10016CA0
// Orders list entries by ascending key while keeping each row's tag attached.
// __cdecl, one short argument (callee pops 0 bytes). Bubble sort of count dword keys at g_eqmain_cat_value_guess into ascending
// signed order; each swap moves the word tag at the same index (g_eqmain_cat_index_guess) with its key. count - 1 passes, none
// when count <= 1. The pass count and the inner bound are the int last = count - 1; i is a short counter, idx its int copy.
// @early-stop: frame size, inner-index spill, key pointers and tag spill/store order still differ.
#define g_eqmain_sort_key_guess       EQ_GLOBAL(int, 0x1004F6EC)              // g_eqmain_cat_value_guess: the dword sort keys
#define g_eqmain_sort_key_next_guess  EQ_GLOBAL(int, 0x1004F6F0)              // g_eqmain_sort_key_next_guess: the same keys, index + 1

void EQMain_SortKeys_guess(short count)
{
    short pass;
    short i;
    int last;
    int key;
    int idx;
    unsigned short tag;

    pass = 0;
    last = count - 1;
    if (1 < count) {
        do {
            idx = 0;
            i = 0;
            if (0 < last) {
                do {
                    key = (&g_eqmain_sort_key_guess)[idx];
                    if (key > (&g_eqmain_sort_key_next_guess)[idx]) {
                        tag = (&g_eqmain_name_tag_guess)[idx];
                        (&g_eqmain_sort_key_guess)[idx] = (&g_eqmain_sort_key_next_guess)[idx];
                        (&g_eqmain_sort_key_next_guess)[idx] = key;
                        (&g_eqmain_name_tag_guess)[idx] = (&g_eqmain_name_tag_next_guess)[idx];
                        (&g_eqmain_name_tag_next_guess)[idx] = tag;
                    }
                    i = i + 1;
                    idx = (int)i;
                } while (idx < last);
            }
            pass = pass + 1;
        } while (pass < last);
    }
}

// FUNCTION: EQMAIN 0x10016D50
// Selects alphabetical or numeric-key ordering for a list.
// __cdecl, void. Mode dispatch on mode: 0 -> 0x10016B90(value); 1 -> 0x10016CA0(value) (the key sort
// above) and return; any other mode does nothing. The asm passes value on as a full dword (no movsx), so it
// is an int here, not the short of the Ghidra signature.
#define eqmain_fn_10016b90_guess  EQ_CDECL(void, 0x10016B90, (int))   // EQMain_SortNames_guess: mode 0 (unknown)
#define eqmain_fn_10016ca0_guess  EQ_CDECL(void, 0x10016CA0, (int))   // EQMain_SortKeys_guess: mode 1, the key sort

void EQMain_Dispatch_guess(int value, int mode)
{
    switch (mode)
    {
    case 0:
        eqmain_fn_10016b90_guess(value);
        break;
    case 1:
        eqmain_fn_10016ca0_guess(value);
        return;
    }
    return;
}

// FUNCTION: EQMAIN 0x10016D80
// Refills the chat screen's entry list: six flag groups, each sorted and coloured; keeps scroll and selection
// __stdcall, no arguments, void. Sorts the rows of g_eqmain_source_rows_guess (count ushort at 0x36B0; a 0x40-byte name at 0x40*i,
// flags dword at 0x3390+4i, value dword at 0x3520+4i) into six lists by flag bits. Each list is copied into g_eqmain_cat_names_guess
// (names), g_eqmain_cat_index_guess (source row) and g_eqmain_cat_value_guess (value), sorted by 0x10016D50 (mode 0; lists 5 and 6 use the mode
// at 0x3390+4k of g_eqmain_name_ptr_guess, k = the row FindRecordIndexByName_guess finds for the name DAT_101255f8), and every row goes to
// g_eqmain_list_obj_guess with 0x10005CC0 in the list's colour (0x4b4bf0, 0x646464, 0x1818f0, 0xc81000, 0xf3f2f2, 0). Then
// g_eqmain_list_obj_guess +0x2c (clamped to [0, count)) and +0x34 are written back and g_chat_list_control_guess +0x68 is raised to +0x2c
// (0x100067E0 then). Nothing else happens when the count is 0.
#define eqmain_fn_10016d50_guess    EQ_CDECL(void, 0x10016D50, (int, int))  // EQMain_Dispatch_guess: sort, mode 0 or 1
#define g_eqmain_source_rows_guess  EQ_GLOBAL(int, 0x101258A8)              // g_eqmain_source_rows_guess: the source rows (see above)
#define g_eqmain_list_obj_guess     EQ_GLOBAL(int, 0x1004F6E4)              // g_eqmain_list_obj_guess: the list (+0x24, +0x2c, +0x30, +0x34)
#define g_eqmain_list_bar_guess     EQ_GLOBAL(int, 0x1004F6DC)              // g_chat_list_control_guess: the list's bar (+0x64 rows, +0x68 top)
#define g_eqmain_cat_names_guess    EQ_ARRAY(char, 0x1004D310)              // g_eqmain_cat_names_guess: the copied names, 0x40 bytes each
#define g_eqmain_cat_index_guess    EQ_ARRAY(short, 0x1004D248)             // g_eqmain_cat_index_guess: the source row of each copied name
#define g_eqmain_cat_value_guess    EQ_ARRAY(int, 0x1004F6EC)               // g_eqmain_cat_value_guess: the value dword of each copied name

void EQMain_BuildFlagLists_guess(void)
{
    unsigned int rowCount;
    unsigned short rawCount;
    unsigned int rowFlags;
    unsigned int rowValue;
    int savedScrollPos;
    int iVar9;
    int destIndex;
    int tmp;
    int sortMode;
    int local_c;
    short sVar5;
    short acc;
    short slot;
    short *srcRowPtr;

    rowCount = eqmain_fn_10026040_guess(g_eqmain_name_table_guess)( &g_eqmain_name_find_guess);
    tmp = 0;
    sortMode = *(int *)(g_eqmain_name_table_guess + (short)rowCount * 4 + 0x3390);
    rawCount = *(unsigned short *)(g_eqmain_source_rows_guess + 0x36B0);
    if (rawCount != 0)
    {
        savedScrollPos = *(int *)(g_eqmain_list_obj_guess + 0x34);
        local_c = *(int *)(g_eqmain_list_obj_guess + 0x2c);
        if ((local_c < 0) || ((int)(unsigned int)rawCount <= local_c))
        {
            local_c = 0;
        }
        eqmain_fn_10005d80_guess(g_eqmain_list_obj_guess);
        acc = 0;
        rowCount = (unsigned int)rawCount;
        sVar5 = 0;
        if (rowCount != 0)
        {
            do
            {
                rowFlags = *(unsigned int *)(g_eqmain_source_rows_guess + 0x3390 + tmp * 4);
                if ((((rowFlags & 0x10) != 0) && ((rowFlags & 1) != 0)) && ((rowFlags & 8) == 0))
                {
                    iVar9 = (int)acc;
                    strcpy(g_eqmain_cat_names_guess + iVar9 * 0x40, (char *)(tmp * 0x40 + g_eqmain_source_rows_guess));
                    acc = acc + 1;
                    g_eqmain_cat_value_guess[iVar9] = *(int *)(g_eqmain_source_rows_guess + 0x3520 + tmp * 4);
                    g_eqmain_cat_index_guess[iVar9] = sVar5;
                }
                sVar5 = sVar5 + 1;
                tmp = (int)sVar5;
            } while (tmp < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(acc, 0);
        if (0 < acc)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)acc;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0x4b4bf0);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        tmp = g_eqmain_source_rows_guess;
        iVar9 = 0;
        slot = 0;
        sVar5 = 0;
        acc = 0;
        if (rowCount != 0)
        {
            do
            {
                rowFlags = *(unsigned int *)(tmp + 0x3390 + iVar9 * 4);
                if (((rowFlags & 0x10) != 0) && ((rowFlags & 1) == 0))
                {
                    destIndex = (int)slot;
                    strcpy(g_eqmain_cat_names_guess + destIndex * 0x40, (char *)(iVar9 * 0x40 + tmp));
                    rowValue = *(unsigned int *)(tmp + 0x3520 + iVar9 * 4);
                    g_eqmain_cat_index_guess[destIndex] = acc;
                    slot = sVar5 + 1;
                    g_eqmain_cat_value_guess[destIndex] = rowValue;
                    sVar5 = slot;
                }
                acc = acc + 1;
                iVar9 = (int)acc;
            } while (iVar9 < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(slot, 0);
        if (0 < slot)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)slot;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0x646464);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        tmp = g_eqmain_source_rows_guess;
        iVar9 = 0;
        slot = 0;
        sVar5 = 0;
        acc = 0;
        if (rowCount != 0)
        {
            do
            {
                rowFlags = *(unsigned int *)(tmp + 0x3390 + iVar9 * 4);
                if (((rowFlags & 1) != 0) && ((rowFlags & 0x18) == 0))
                {
                    destIndex = (int)slot;
                    strcpy(g_eqmain_cat_names_guess + destIndex * 0x40, (char *)(iVar9 * 0x40 + tmp));
                    rowValue = *(unsigned int *)(tmp + 0x3520 + iVar9 * 4);
                    g_eqmain_cat_index_guess[destIndex] = acc;
                    g_eqmain_cat_value_guess[destIndex] = rowValue;
                    slot = sVar5 + 1;
                    sVar5 = slot;
                }
                acc = acc + 1;
                iVar9 = (int)acc;
            } while (iVar9 < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(slot, 0);
        if (0 < slot)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)slot;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0x1818f0);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        tmp = g_eqmain_source_rows_guess;
        iVar9 = 0;
        slot = 0;
        sVar5 = 0;
        acc = 0;
        if (rowCount != 0)
        {
            do
            {
                rowFlags = *(unsigned int *)(tmp + 0x3390 + iVar9 * 4);
                if (((rowFlags & 4) != 0) && ((rowFlags & 0x18) == 0))
                {
                    destIndex = (int)slot;
                    strcpy(g_eqmain_cat_names_guess + destIndex * 0x40, (char *)(iVar9 * 0x40 + tmp));
                    rowValue = *(unsigned int *)(tmp + 0x3520 + iVar9 * 4);
                    g_eqmain_cat_index_guess[destIndex] = acc;
                    g_eqmain_cat_value_guess[destIndex] = rowValue;
                    slot = sVar5 + 1;
                    sVar5 = slot;
                }
                acc = acc + 1;
                iVar9 = (int)acc;
            } while (iVar9 < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(slot, 0);
        if (0 < slot)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)slot;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0xc81000);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        tmp = g_eqmain_source_rows_guess;
        iVar9 = 0;
        slot = 0;
        sVar5 = 0;
        acc = 0;
        if (rowCount != 0)
        {
            do
            {
                rowFlags = *(unsigned int *)(tmp + 0x3390 + iVar9 * 4);
                if ((((rowFlags & 5) == 0) && ((rowFlags & 0x20) != 0)) && ((rowFlags & 0x10) == 0))
                {
                    destIndex = (int)slot;
                    strcpy(g_eqmain_cat_names_guess + destIndex * 0x40, (char *)(iVar9 * 0x40 + tmp));
                    rowValue = *(unsigned int *)(tmp + 0x3520 + iVar9 * 4);
                    g_eqmain_cat_index_guess[destIndex] = acc;
                    slot = sVar5 + 1;
                    g_eqmain_cat_value_guess[destIndex] = rowValue;
                    sVar5 = slot;
                }
                acc = acc + 1;
                iVar9 = (int)acc;
            } while (iVar9 < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(slot, sortMode);
        if (0 < slot)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)slot;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0xf3f2f2);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        tmp = 0;
        acc = 0;
        sVar5 = 0;
        if (rowCount != 0)
        {
            do
            {
                if ((*(unsigned char *)(g_eqmain_source_rows_guess + 0x3390 + tmp * 4) & 0x35) == 0)
                {
                    iVar9 = (int)acc;
                    strcpy(g_eqmain_cat_names_guess + iVar9 * 0x40, (char *)(tmp * 0x40 + g_eqmain_source_rows_guess));
                    acc = acc + 1;
                    g_eqmain_cat_value_guess[iVar9] = *(int *)(g_eqmain_source_rows_guess + 0x3520 + tmp * 4);
                    g_eqmain_cat_index_guess[iVar9] = sVar5;
                }
                sVar5 = sVar5 + 1;
                tmp = (int)sVar5;
            } while (tmp < (int)rowCount);
        }
        eqmain_fn_10016d50_guess(acc, sortMode);
        if (0 < acc)
        {
            srcRowPtr = g_eqmain_cat_index_guess;
            tmp = (int)acc;
            do
            {
                eqmain_fn_10005cc0_guess(g_eqmain_list_obj_guess, 0x70,
                                         (char *)(*srcRowPtr * 0x40 + g_eqmain_source_rows_guess), 0, 0);
                srcRowPtr = srcRowPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
        }
        *(int *)(g_eqmain_list_obj_guess + 0x2c) = local_c;
        *(int *)(g_eqmain_list_obj_guess + 0x34) = savedScrollPos;
        eqmain_fn_10005e70_guess(g_eqmain_list_obj_guess, savedScrollPos);
        *(int *)(g_eqmain_list_bar_guess + 0x64) =
            (*(int *)(g_eqmain_list_obj_guess + 0x24) - *(int *)(g_eqmain_list_obj_guess + 0x30)) + 1;
        if (*(int *)(g_eqmain_list_bar_guess + 0x68) < *(int *)(g_eqmain_list_obj_guess + 0x2c))
        {
            *(int *)(g_eqmain_list_bar_guess + 0x68) = *(int *)(g_eqmain_list_obj_guess + 0x2c);
            eqmain_fn_100067e0_guess(g_eqmain_list_bar_guess);
        }
    }
    return;
}

#define eqmain_fn_10022d50_dispatch_guess EQ_STDCALL(unsigned int, 0x10022D50, (void))
#define eqmain_fn_100024e0_dispatch_guess  EQ_CDECL(void, 0x100024E0, (int *, char *, char))
#define eqmain_fn_100168f0_dispatch_guess  EQ_CDECL(void, 0x100168F0, (unsigned char *))
#define eqmain_fn_10016a40_dispatch_guess  EQ_STDCALL(void, 0x10016A40, (void))
#define eqmain_fn_10016d80_dispatch_guess  EQ_STDCALL(void, 0x10016D80, (void))
#define eqmain_fn_10002890_dispatch_guess  EQ_STDCALL(void, 0x10002890, (void))
#define eqmain_fn_10021bc0_dispatch_guess EQ_CDECL(int, 0x10021BC0, (char *))
#define eqmain_fn_100166b0_dispatch_guess EQ_CDECL(void, 0x100166B0, (char *))
#define eqmain_fn_10016550_dispatch_guess EQ_STDCALL(void, 0x10016550, (void))
#define eqmain_fn_100164f0_dispatch_guess EQ_STDCALL(void, 0x100164F0, (void))
#define eqmain_fn_10016820_dispatch_guess EQ_STDCALL(void, 0x10016820, (void))
#define eqmain_fn_100044b0_dispatch_guess EQ_CDECL(unsigned int, 0x100044B0, (unsigned int, unsigned int, unsigned int, unsigned int))
#define eqmain_fn_10020ef0_output_guess   EQ_CDECL(EQMainOutputContext_guess *, 0x10020EF0, (char))
#define g_eqmain_dispatch_buffer_guess   EQ_ARRAY(char, 0x100F4C10)
#define g_eqmain_dispatch_text_guess     EQ_GLOBAL(char *, 0x10055960)
#define g_eqmain_str_ec10_guess   EQ_ARRAY(char, 0x1004EC10)

// FUNCTION: EQMAIN 0x100173C0
// Routes front-end action codes to chat, name-list, and logging handlers.
short __stdcall EQMain_HandleActionCode_guess(void)
{
    unsigned int action_code;
    short action_result;

    action_code = eqmain_fn_10022d50_dispatch_guess();
    action_result = (short)action_code;
    if ((int)action_code < 0x31)
    {
        if (action_code == 0x30)
        {
            eqmain_fn_100024e0_dispatch_guess(g_eqmain_obj_guess, g_eqmain_dispatch_buffer_guess, '\0');
            return action_result;
        }
        switch (action_code)
        {
        case 0x0a:
            eqmain_fn_100168f0_dispatch_guess((unsigned char *)g_eqmain_dispatch_buffer_guess);
            return action_result;
        case 0x0c:
        case 0x10:
        case 0x11:
            g_eqmain_list_flag_guess = 1;
            eqmain_fn_10016a40_dispatch_guess();
        case 0x07:
        case 0x08:
        case 0x22:
        case 0x28:
            eqmain_fn_10016d80_dispatch_guess();
            return action_result;
        case 0x2c:
            eqmain_fn_10002890_dispatch_guess();
            if (strlen(g_eqmain_str_ec10_guess) != 0)
            {
                eqmain_fn_10021bc0_dispatch_guess(g_eqmain_str_ec10_guess);
            }
        case 0x0d:
        case 0x0e:
        case 0x2b:
            eqmain_fn_10016a40_dispatch_guess();
            return action_result;
        }
    }
    else if ((int)action_code < 0x6a)
    {
        if (action_code == 0x69)
        {
            eqmain_fn_100166b0_dispatch_guess(g_eqmain_dispatch_buffer_guess);
            return action_result;
        }
        switch (action_code)
        {
        case 0x4e:
            eqmain_fn_10016a40_dispatch_guess();
            return action_result;
        case 0x58:
            eqmain_fn_100024e0_dispatch_guess(g_eqmain_obj_guess, g_eqmain_dispatch_buffer_guess, '\x01');
            return action_result;
        case 0x61:
            eqmain_fn_100224b0_guess(g_eqmain_dispatch_text_guess,
                                     eqmain_fn_10020ef0_output_guess(1)->line_target, 5);
            return action_result;
        case 0x62:
            eqmain_fn_10016550_dispatch_guess();
            return action_result;
        }
    }
    else
    {
        if ((int)action_code < 0x79)
        {
            if (action_code == 0x78)
            {
                eqmain_fn_10016d80_dispatch_guess();
                return action_result;
            }
            if (action_code == 0x6b)
            {
                eqmain_fn_10016820_dispatch_guess();
                return action_result;
            }
            if (action_code == 0x74)
            {
                eqmain_fn_100164f0_dispatch_guess();
                return action_result;
            }
        }
        else if (action_code == 0x10000003)
        {
            eqmain_fn_100044b0_dispatch_guess(4, 0x1b, 0, 0);
        }
    }
    return action_result;
}

// FUNCTION: EQMAIN 0x10017590
// Keeps a rolling list of the latest ten front-end text entries.
// __cdecl, one argument (the string); callee pops 0 bytes; returns nothing. Copies the string (strcpy: NUL included, no
// bounds check) into the ten 0xFE-byte slots from g_chat_page_texts_guess. While the signed count g_eqmain_line_count_guess is below 10 it goes
// to slot count and the count goes up by one; else slots 1..9 each move down one slot (passes start at g_eqmain_line_slot1_guess and
// repeat while the stepped pointer is below g_chat_list_control_guess) and the string goes to slot 9 (g_eqmain_line_slot9_guess). Then g_chat_page_index_guess
// takes the count: re-read after the copy in the first case, the count read on entry in the second.
#define g_eqmain_line_count_guess   EQ_GLOBAL(short, 0x1004F87C)   // g_eqmain_line_count_guess: the signed count of slots in use
#define g_eqmain_line_mirror_guess  EQ_GLOBAL(short, 0x1004F87E)   // g_chat_page_index_guess: takes the count last
#define g_eqmain_line_slot0_guess   EQ_ARRAY(char, 0x1004ECF0)     // g_chat_page_texts_guess: slot 0 of ten 0xFE-byte slots
#define g_eqmain_line_slot1_guess   EQ_ARRAY(char, 0x1004EDEE)     // g_eqmain_line_slot1_guess: slot 1, the first shift source
#define g_eqmain_line_slot9_guess   EQ_ARRAY(char, 0x1004F5DE)     // g_eqmain_line_slot9_guess: slot 9, the last slot

void EQMain_PushSlotString_guess(char *str)
{
    char *dst;
    char *src;

    if (g_eqmain_line_count_guess < 10)
    {
        dst = g_eqmain_line_slot0_guess + g_eqmain_line_count_guess * 0xfe;
        ++g_eqmain_line_count_guess;
        strcpy(dst, str);
        g_eqmain_line_mirror_guess = g_eqmain_line_count_guess;
        return;
    }
    short count = g_eqmain_line_count_guess;
    src = g_eqmain_line_slot1_guess;
    do
    {
        dst = src - 0xfe;
        strcpy(dst, src);
        src += 0xfe;
    } while ((int)src < (int)&g_eqmain_slot_end_guess);
    strcpy(g_eqmain_line_slot9_guess, str);
    g_eqmain_line_mirror_guess = count;
}

// FUNCTION: EQMAIN 0x10017660
// Resets the ten-entry short text list to its default text.
// __stdcall, void, no args (pops 0 bytes, so the same code as __cdecl). Copies the string at g_eqmain_drive_base_guess, NUL
// included, into each of ten 0x14-byte slots from g_eqmain_table_base_guess up to g_chat_view_guess (strcpy: a string of 0x14 bytes
// or more runs into the next slot), then clears the two words g_chat_page_enabled_guess and g_eqmain_table_index_guess.
#define g_eqmain_default_string_guess  EQ_ARRAY(char, 0x1004CE40)            // g_eqmain_drive_base_guess: copied into each slot
#define g_eqmain_slot_table_guess      EQ_ARRAY(char, 0x1004EC24)            // g_eqmain_table_base_guess: first of ten 0x14-byte slots
#define g_eqmain_slot_table_end_guess  EQ_ARRAY(char, 0x1004ECEC)            // g_chat_view_guess: one past the table (never read)
#define g_eqmain_clear_word_a_guess    EQ_GLOBAL(unsigned short, 0x1004F880) // g_chat_page_enabled_guess: zeroed last
#define g_eqmain_clear_word_b_guess    EQ_GLOBAL(unsigned short, 0x1004F882) // g_eqmain_table_index_guess: zeroed last

void __stdcall EQMain_ResetSlotTable_guess(void)
{
    char *slot;
    char *next;

    slot = g_eqmain_slot_table_guess;
    do {
        next = slot + 0x14;
        strcpy(slot, g_eqmain_default_string_guess);
        slot = next;
    } while ((int)slot < (int)g_eqmain_slot_table_end_guess);
    g_eqmain_clear_word_a_guess = 0;
    g_eqmain_clear_word_b_guess = 0;
}

// FUNCTION: EQMAIN 0x100176B0
// Cleans up repeated spaces in a text entry.
// __cdecl, void, one argument (callee pops 0 bytes). Collapses runs of spaces in place: a run of three or more is cut
// to two, every other byte is copied unchanged (a tab or a 0x80 byte is not a space). The loop is a do-while that
// tests the byte after each copy, so an empty string copies its NUL and keeps reading past it.
void EQMain_CollapseSpaces_guess(char *dst)
{
    char *src = dst;
    short spaces = 0;

    do {
        if (*src == ' ') {
            if (spaces < 2) {
                *dst = ' ';
                dst++;
                src++;
                spaces++;
                continue;
            }
            src++;
            continue;
        }
        *dst = *src;
        dst++;
        src++;
        spaces = 0;
    } while (*src != '\0');
    *dst = '\0';
}

// FUNCTION: EQMAIN 0x100176E0
// Normalizes mostly uppercase text entries to lowercase.
// __cdecl, void, one argument (callee pops 0 bytes). Counts the upper-case characters and the others of the string
// (one _isupper call per character). When the string has at least as many upper-case characters as others (an empty
// string too: a no-op), __strlwr lower-cases it in place; otherwise it is left alone.
#define eqmain_fn_10033839_guess  EQ_CDECL(int, 0x10033839, (int))                // __cdecl, _isupper: nonzero for A-Z
#define eqmain_fn_10035c00_guess  EQ_CDECL(char *, 0x10035C00, (char *))         // __cdecl, __strlwr: in place

void EQMain_LowerIfAtLeastHalfUpper_guess(char *text)
{
    int upper = 0;
    int other = 0;
    char *p = text;

    if (*text != '\0') {
        do {
            if (eqmain_fn_10033839_guess(*p++)) {
                upper++;
            } else {
                other++;
            }
        } while (*p != '\0');
        if (upper < other) {
            return;
        }
    }
    eqmain_fn_10035c00_guess(text);
}

// FUNCTION: EQMAIN 0x10017730
// Builds a #reply response from the current table entry when replies are available.
// __cdecl, void, one argument (callee pops 0 bytes). When the string starts with "#reply" (6 characters, case-insensitive,
// __strnicmp) and g_chat_page_enabled_guess is above 0: copies the string to g_eqmain_drive_base_guess, rebuilds it as g_chat_separator_guess, appends
// EQMain_TableEntry_guess() and g_eqmain_reply_suffix_guess with strcat, and returns. Otherwise copies g_eqmain_drive_base_guess back over the string and writes
// g_eqmain_reply_line_guess through BuildTaggedText_guess to the object EqfeCfgSetActive_guess(1) returns (its dword at +4), with 5 as the third argument.
#define eqmain_fn_10035d40_guess      EQ_CDECL(int, 0x10035D40, (char *, char *, size_t))  // __cdecl, __strnicmp: 0 when equal
#define eqmain_fn_100169f0_guess      EQ_STDCALL(char *, 0x100169F0, (void))               // __stdcall, no arguments: a pointer
#define g_eqmain_str_reply_guess      EQ_ARRAY(char, 0x10048020)                           // "#reply": the command prefix
#define g_eqmain_reply_avail_guess    EQ_GLOBAL(short, 0x1004F880)                         // g_chat_page_enabled_guess: signed, the reply branch needs > 0
#define g_eqmain_drive_base_guess       EQ_ARRAY(char, 0x1004CE40)                           // g_eqmain_drive_base_guess: the saved command line
#define g_eqmain_reply_prefix_guess   EQ_ARRAY(char, 0x1004801C)                           // g_chat_separator_guess: the text the line starts with
#define g_eqmain_reply_suffix_guess   EQ_ARRAY(char, 0x1004CE46)                           // g_eqmain_reply_suffix_guess: the text the line ends with
#define g_eqmain_reply_line_guess     EQ_GLOBAL(char *, 0x10055964)                        // g_eqmain_reply_line_guess: the text of the line written

void EQMain_HandleReplyCommand_guess(char *entryText)
{
    char kind;
    int tmp;

    tmp = eqmain_fn_10035d40_guess(g_eqmain_str_reply_guess, entryText, 6);
    if (tmp == 0) {
        if (g_eqmain_reply_avail_guess > 0) {
            strcpy(g_eqmain_drive_base_guess, entryText);
            strcpy(entryText, g_eqmain_reply_prefix_guess);
            strcat(entryText, eqmain_fn_100169f0_guess());
            strcat(entryText, g_eqmain_reply_suffix_guess);
            return;
        }
        kind = 5;
        strcpy(entryText, g_eqmain_drive_base_guess);
        tmp = eqmain_fn_10020ef0_guess(1);
        eqmain_fn_100224b0_guess(g_eqmain_reply_line_guess, *(int *)(tmp + 4), kind);
    }
}

// FUNCTION: EQMAIN 0x10017840
// Processes #version to produce two version text lines.
// __cdecl, void, one argument (callee pops 0 bytes). When the string starts with "#version" (8 characters, case-insensitive,
// __strnicmp), formats DecodeToBuffer_guess(version_string_guess()) with g_eqmain_version_format_guess into a 40-byte buffer and writes it through BuildTaggedText_guess
// to the object EqfeCfgSetActive_guess(1) returns (its dword at +4), with 5 as the third argument. Then formats DAT_10048028 with
// g_eqmain_second_format_guess and writes that line the same way.
#define eqmain_fn_10035d40_guess      EQ_CDECL(int, 0x10035D40, (char *, char *, size_t))  // __cdecl, __strnicmp: 0 when equal
#define eqmain_fn_10028720_guess      EQ_STDCALL(unsigned char *, 0x10028720, (void))      // __stdcall, no arguments: a pointer
#define eqmain_fn_10021110_guess      EQ_CDECL(int *, 0x10021110, (unsigned char *))       // __cdecl, returns a pointer
#define g_eqmain_str_version_guess    EQ_ARRAY(char, 0x10048038)                           // "#version": the command prefix
#define g_eqmain_version_format_guess EQ_GLOBAL(char *, 0x10055AB0)                        // g_eqmain_version_format_guess: the format of the first line
#define g_eqmain_second_format_guess  EQ_GLOBAL(char *, 0x10055AB4)                        // g_eqmain_second_format_guess: the format of the second line
#define g_eqmain_str_10048028_guess   EQ_ARRAY(char, 0x10048028)                           // DAT_10048028: passed by address

void EQMain_HandleVersionCommand_guess(char *commandText)
{
    int comparisonResult;
    unsigned char *inputPtr;
    int *formatArg;
    char lineBuffer[40];

    comparisonResult = eqmain_fn_10035d40_guess(g_eqmain_str_version_guess, commandText, 8);
    if (comparisonResult == 0) {
        inputPtr = eqmain_fn_10028720_guess();
        formatArg = eqmain_fn_10021110_guess(inputPtr);
        eqmain_fn_10034117_guess(lineBuffer, g_eqmain_version_format_guess, formatArg);
        eqmain_fn_100224b0_guess(lineBuffer, *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
        eqmain_fn_10034117_guess(lineBuffer, g_eqmain_second_format_guess, g_eqmain_str_10048028_guess);
        eqmain_fn_100224b0_guess(lineBuffer, *(int *)(eqmain_fn_10020ef0_guess(1) + 4), 5);
    }
}

// FUNCTION: EQMAIN 0x100178E0
// Runs the initial front-end setup check and opens its error dialog if a check fails.
// @early-stop: only the entry flag JNZ target differs; the rewrite reaches the shared flag reload, while the original skips it.
// __cdecl, char, one argument (callee pops 0 bytes). While g_eqmain_startup_done_guess is 0: shows g_eqmain_msg_first_guess, makes the font, and SisaTransparentLogin_guess
// tests g_eqmain_login_name_buf_guess with g_eqmain_login_pin_buf_guess; when that passes, sets the flag and returns 1. Otherwise it makes the font again, copies
// g_eqmain_msg_src_guess into g_eqmain_copybuf_guess, runs eqmainRunDialog_guess on the copy with g_str_888_guess, clears the flag, shows g_eqmain_msg_last_guess and writes
// 0x12 to *resultCode (7 when TryTransparentLogin_guess's low byte is 0). Returns the flag.
#define g_eqmain_startup_done_guess   EQ_GLOBAL(char, 0x1004F88C)                          // g_eqmain_startup_done_guess: 1 once the check has passed
#define g_eqmain_msg_first_guess      EQ_GLOBAL(char *, 0x10055968)                        // g_eqmain_msg_first_guess: the first line shown
#define g_eqmain_msg_last_guess       EQ_GLOBAL(char *, 0x10055978)                        // g_eqmain_msg_last_guess: the line shown after the copy
#define g_eqmain_msg_src_guess        EQ_GLOBAL(char *, 0x10055974)                        // g_eqmain_msg_src_guess: the string copied to g_eqmain_copybuf_guess
#define g_eqmain_str_10055888_guess   EQ_GLOBAL(char *, 0x10055888)                        // g_str_888_guess: the third argument of eqmainRunDialog_guess
#define g_eqmain_copy_buf_guess       EQ_ARRAY(char, 0x100F4C10)                           // g_eqmain_copybuf_guess: the copy, passed by address
#define g_eqmain_obj_guess            EQ_GLOBAL(int *, 0x1004CEB8)                         // g_eqmain_ceb8_guess: the object eqmainRunDialog_guess works on
#define g_str_ec9cc_guess           EQ_ARRAY(char, 0x100EC9CC)                           // g_str_ec9cc_guess: passed by address to InitRdpFromString_guess
#define g_eqmain_login_name_buf_guess         EQ_ARRAY(char, 0x10104CD8)                           // g_eqmain_login_name_buf_guess: passed by address to SisaTransparentLogin_guess/10021ac0
#define g_eqmain_login_pin_buf_guess         EQ_ARRAY(char, 0x1012565C)                           // g_eqmain_login_pin_buf_guess: passed by address to SisaTransparentLogin_guess/10021ac0
#define eqmain_fn_10020ae0_guess      EQ_CDECL(void, 0x10020AE0, (char *))                 // __cdecl, shows one line of text
#define eqmain_fn_100216f0_guess      EQ_CDECL(void, 0x100216F0, (void))                   // __fastcall, ECX argument the original leaves as residue; no stack arguments, so called with none
#define eqmain_fn_100211f0_guess      EQ_CDECL(int, 0x100211F0, (char *))                  // __cdecl, returns a dword (unused)
#define eqmain_fn_10021a60_guess      EQ_CDECL(bool, 0x10021A60, (char *, char *, char *)) // __cdecl, bool: true when the check passes
#define eqmain_fn_10001ff0_guess      EQ_CDECL(char, 0x10001FF0, (int *, char *, char *, char *)) // __cdecl, four arguments (result unused)
#define eqmain_fn_10021ac0_guess      EQ_CDECL(int, 0x10021AC0, (char *, char *, char *))  // __cdecl, AL is the result

char EQMain_StartupCheck_guess(unsigned short *resultCode)
{
    char *pCVar2;
    bool checkResult;
    int result;

    if (g_eqmain_startup_done_guess == 0) {
        eqmain_fn_10020ae0_guess(g_eqmain_msg_first_guess);
        eqmain_fn_100216f0_guess();
        eqmain_fn_100211f0_guess(g_str_ec9cc_guess);
        checkResult = eqmain_fn_10021a60_guess(g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess, 0);
        if (!checkResult) {
            eqmain_fn_100216f0_guess();
            pCVar2 = g_eqmain_str_10055888_guess;
            strcpy(g_eqmain_copy_buf_guess, g_eqmain_msg_src_guess);
            eqmain_fn_10001ff0_guess(g_eqmain_obj_guess, g_eqmain_copy_buf_guess, pCVar2, 0);
            g_eqmain_startup_done_guess = 0;
            eqmain_fn_10020ae0_guess(g_eqmain_msg_last_guess);
            *resultCode = 0x12;
            result = eqmain_fn_10021ac0_guess(g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess, 0);
            if ((char)result == 0) {
                *resultCode = 7;
            }
        } else {
            g_eqmain_startup_done_guess = 1;
            return 1;
        }
    }
    return g_eqmain_startup_done_guess;
}

// FUNCTION: EQMAIN 0x100179C0
// When the check-passed flag g_eqmain_startup_done_guess is set: shows a line, calls TryTransparentLogin_guess (7 on failure), clears it.
// __cdecl, void, one argument (callee pops 0 bytes). When the flag g_eqmain_startup_done_guess is set: ShowStartScreen_guess(DAT_1005596c), then
// CleanupSetup_guess (__fastcall; its ECX argument is not read), then TryTransparentLogin_guess(&g_eqmain_login_name_buf_guess, &g_eqmain_login_pin_buf_guess, NULL). When that
// returns 0 in its low byte, writes 7 to the word at resultCode. The flag is cleared in every case.
#define g_eqmain_str_1005596c_guess EQ_GLOBAL(char *, 0x1005596C)                           // DAT_1005596c: passed to ShowStartScreen_guess
#define eqmain_fn_10020ae0_guess    EQ_CDECL(void, 0x10020AE0, (char *))                    // __cdecl, the string draw
#define eqmain_fn_100216f0_guess    EQ_CDECL(void, 0x100216F0, (void))                      // CleanupSetup_guess: __fastcall, ECX never read, so called bare
#define eqmain_fn_10021ac0_guess    EQ_CDECL(int, 0x10021AC0, (char *, char *, char *))     // __cdecl, returns a status in EAX

void EQMain_RebuildFontsIfDirty_guess(unsigned short *resultCode)
{
    int result;

    if (g_eqmain_startup_done_guess != 0) {
        eqmain_fn_10020ae0_guess(g_eqmain_str_1005596c_guess);
        eqmain_fn_100216f0_guess();
        result = eqmain_fn_10021ac0_guess(g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess, NULL);
        if ((char)result == 0) {
            *resultCode = 7;
        }
    }
    g_eqmain_startup_done_guess = 0;
}

#define g_eqmain_ceb8_guess            EQ_GLOBAL(int, 0x1004CEB8)
#define g_eqmain_text_5830_guess     EQ_GLOBAL(char *, 0x10055830)
#define g_eqmain_text_5844_guess     EQ_GLOBAL(char *, 0x10055844)
#define g_chat_button_text_2_guess     EQ_GLOBAL(char *, 0x10055838)
#define g_chat_button_text_3_guess     EQ_GLOBAL(char *, 0x100558C4)
#define g_chat_button_text_4_guess     EQ_GLOBAL(char *, 0x10055884)
#define g_chat_view_guess              EQ_GLOBAL(EQMainChatControl_guess *, 0x1004ECEC)
#define g_chat_queue_guess             EQ_GLOBAL(EQMainChatQueue_guess *, 0x1004F6E0)
#define g_chat_list_control_guess      EQ_GLOBAL(int, 0x1004F6DC)
#define g_chat_other_list_guess        EQ_GLOBAL(int, 0x1004F6E8)
#define g_chat_other_control_guess     EQ_GLOBAL(int, 0x1004D244)
#define g_choice_count_guess       EQ_GLOBAL(int, 0x100F1164)
#define g_choice_text_guess        EQ_ARRAY(char, 0x100F1168)
#define g_choice_result_guess    EQ_GLOBAL(int, 0x100482C8)
#define g_eqmain_copybuf_guess       EQ_ARRAY(char, 0x100F4C10)
#define g_chat_page_enabled_guess      EQ_GLOBAL(short, 0x1004F880)
#define g_chat_dialog_guess            EQ_GLOBAL(EQMainChatDialog_guess *, 0x1004CEC4)
#define g_chat_separator_guess         EQ_ARRAY(char, 0x1004801C)
#define g_chat_suffix_guess            EQ_ARRAY(char, 0x10046608)
#define g_chat_status_guess            EQ_GLOBAL(short, 0x1004814E)
#define g_chat_page_index_guess        EQ_GLOBAL(short, 0x1004F87E)
#define g_chat_page_texts_guess        EQ_ARRAY(char, 0x1004ECF0)
#define g_str_888_guess        EQ_GLOBAL(char *, 0x10055888)
#define g_eqmain_dlg_arg_guess        EQ_GLOBAL(int, 0x1004CEC8)
#define eqmain_chat_fn_100178e0_guess  EQ_CDECL(unsigned int, 0x100178E0, (unsigned int *))
#define eqmain_fn_10007bf0_guess  EQ_CDECL(void, 0x10007BF0, (int, char *, int))
#define eqmain_chat_fn_10001520_guess  EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))
#define eqmain_fn_100080b0_guess  EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int))
#define eqmain_chat_fn_100017b0_guess  EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))
#define eqmain_chat_fn_10008460_guess  EQ_CDECL(int, 0x10008460, (int, int, int, int, int, unsigned int, int, int, unsigned int, unsigned int))
#define eqmain_chat_fn_10008910_guess  EQ_CDECL(void, 0x10008910, (int))
#define eqmain_chat_fn_100168a0_guess  EQ_STDCALL(void, 0x100168A0, (void))
#define eqmain_chat_fn_10017660_guess  EQ_STDCALL(void, 0x10017660, (void))
#define eqmain_chat_fn_10006950_guess  EQ_CDECL(int, 0x10006950, (int, int, int, int, int, int))
#define eqmain_chat_fn_10008c10_guess  EQ_CDECL(EQMainChatQueue_guess *, 0x10008C10, (int, int, int, int, int, int, int, int, int))
#define eqmain_chat_fn_10005840_guess  EQ_CDECL(int, 0x10005840, (int, int, int, int, int, unsigned int, int))
#define eqmain_chat_fn_100229f0_guess  EQ_STDCALL(void, 0x100229F0, (void))
#define eqmain_chat_fn_10003a00_guess  EQ_STDCALL(void, 0x10003A00, (void))
#define eqmain_chat_fn_100173c0_guess  EQ_STDCALL(unsigned int, 0x100173C0, (void))
#define eqmain_chat_fn_10001ce0_guess  EQ_CDECL(int, 0x10001CE0, (int, char *, int))
#define eqmain_set_report_opcode_guess  EQ_CDECL(void, 0x10024F90, (unsigned short))
#define eqmain_fn_10024fd0_guess  EQ_CDECL(void, 0x10024FD0, (int))
#define eqmain_finish_hardware_report_guess  EQ_STDCALL(void, 0x100213F0, (void))
#define eqmain_chat_fn_10001320_guess  EQ_CDECL(void, 0x10001320, (char *))
#define eqmain_chat_fn_10001ff0_guess  EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))
#define eqmain_chat_fn_10001350_guess  EQ_STDCALL(void, 0x10001350, (void))
#define eqmain_chat_fn_10003bd0_guess  EQ_STDCALL(EQMainChatEvent_guess *, 0x10003BD0, (void))
#define eqmain_chat_fn_10016a10_guess  EQ_STDCALL(char *, 0x10016A10, (void))
#define eqmain_free_guess  EQ_CDECL(void, 0x10033568, (void *))
#define eqmain_chat_fn_10022b10_guess  EQ_STDCALL(void, 0x10022B10, (void))
#define eqmain_fn_100011a0_guess  EQ_STDCALL(void, 0x100011A0, (void))
#define eqmain_chat_fn_10001700_guess  EQ_CDECL(void, 0x10001700, (int))
#define eqmain_chat_fn_10008ce0_guess  EQ_CDECL(void, 0x10008CE0, (EQMainChatQueue_guess *))
#define eqmain_chat_fn_10008a00_guess  EQ_CDECL(void, 0x10008A00, (int))
#define eqmain_chat_fn_10006ae0_guess  EQ_CDECL(void, 0x10006AE0, (int))
#define eqmain_chat_fn_10005930_guess  EQ_CDECL(void, 0x10005930, (int))
#define eqmain_fn_100039e0_arg_guess  EQ_CDECL(void, 0x100039E0, (int))
#define eqmain_chat_fn_10008aa0_guess  EQ_CDECL(void, 0x10008AA0, (int))
#define eqmain_chat_fn_10008970_guess  EQ_CDECL(void, 0x10008970, (int, char *))
#define eqmain_chat_fn_10008d60_guess  EQ_CDECL(void, 0x10008D60, (EQMainChatQueue_guess *))
#define eqmain_chat_fn_10005a00_guess  EQ_CDECL(void, 0x10005A00, (int))
#define eqmain_chat_fn_10005e10_guess  EQ_CDECL(int, 0x10005E10, (int))
#define eqmain_chat_fn_10021b00_guess  EQ_CDECL(void, 0x10021B00, (int))
#define eqmain_chat_fn_100176b0_guess  EQ_CDECL(void, 0x100176B0, (char *))
#define eqmain_chat_fn_100176e0_guess  EQ_CDECL(void, 0x100176E0, (char *))
#define eqmain_chat_fn_10017730_guess  EQ_CDECL(void, 0x10017730, (char *))
#define eqmain_chat_fn_10017840_guess  EQ_CDECL(void, 0x10017840, (char *))
#define eqmain_chat_fn_10022970_guess  EQ_CDECL(void, 0x10022970, (char *))
#define eqmain_chat_fn_10017590_guess  EQ_CDECL(void, 0x10017590, (char *))
#define eqmain_chat_fn_100179c0_guess  EQ_CDECL(void, 0x100179C0, (unsigned short *))

// FUNCTION: EQMAIN 0x10017a10
// Runs the front-end chat screen (chat.bmp): input line, lists and buttons; returns the next front-end state
unsigned int __cdecl EQMain_RunChatScreen_guess(char interactive_mode)
{
    unsigned int event_code;
    unsigned int result_code;
    unsigned int setup_result;
    EQMainChatEvent_guess *message;
    EQMainChatQueueNode_guess *node;
    char *entry_text;
    int index;
    int selection;
    int text_length;
    int selected_item;
    int button_0;
    int button_1;
    int button_2;
    int button_3;
    int button_4;
    int dialog_handle;
    bool done;

    done = false;
    result_code = 7;
    g_eqmain_list_flag_guess = 1;
    if ((interactive_mode != 0) && (setup_result = eqmain_chat_fn_100178e0_guess(&result_code), (char)setup_result == 0)) {
        return (setup_result & 0xffff0000) | (result_code & 0xffff);
    }
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, (char *)0x1004652C, 0x10);
    button_0 = eqmain_chat_fn_10001520_guess(g_eqmain_ceb8_guess, 0xb, 0x1b1, 0, 1);
    button_1 = eqmain_chat_fn_10001520_guess(g_eqmain_ceb8_guess, 0x8a, 0x1b1, 0, 1);
    button_2 = eqmain_chat_fn_10001520_guess(g_eqmain_ceb8_guess, 0x109, 0x1b1, 0, 1);
    button_3 = eqmain_chat_fn_10001520_guess(g_eqmain_ceb8_guess, 0x186, 0x1b1, 0, 1);
    button_4 = eqmain_chat_fn_10001520_guess(g_eqmain_ceb8_guess, 0x203, 0x1b1, 0, 1);
    selection = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_eqmain_text_5830_guess, 0x19, 0);
    selection = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_eqmain_text_5844_guess, selection, 0);
    selection = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_chat_button_text_2_guess, selection, 0);
    selection = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_chat_button_text_4_guess, selection, 0);
    selection = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_chat_button_text_3_guess, selection, 0);
    eqmain_chat_fn_100017b0_guess(button_0, 0, g_eqmain_text_5830_guess, selection, 0);
    eqmain_chat_fn_100017b0_guess(button_1, 0, g_eqmain_text_5844_guess, selection, 0);
    eqmain_chat_fn_100017b0_guess(button_2, 0, g_chat_button_text_2_guess, selection, 0);
    if (button_3 != 0) {
        eqmain_chat_fn_100017b0_guess(button_3, 0, g_chat_button_text_3_guess, selection, 0);
    }
    eqmain_chat_fn_100017b0_guess(button_4, 0, g_chat_button_text_4_guess, selection, 0);
    dialog_handle = eqmain_chat_fn_10008460_guess(g_eqmain_ceb8_guess, 0x18, 0x18b, 0x178, 0x10, 0xfe, 0, 0, 0x22d69, 0);
    eqmain_chat_fn_10008910_guess(dialog_handle);
    eqmain_chat_fn_100168a0_guess();
    eqmain_chat_fn_10017660_guess();
    g_chat_view_guess = (EQMainChatControl_guess *)eqmain_chat_fn_10006950_guess(g_eqmain_ceb8_guess, 0x1a7, 0x32, 0x10, 0x145, 0);
    g_chat_queue_guess = eqmain_chat_fn_10008c10_guess(g_eqmain_ceb8_guess, 0x26, 0x4c, 0x15c, 0x112, 300, 0x21a3c, g_eqmain_dlg_arg_guess, (int)g_chat_view_guess);
    g_eqmain_list_obj_guess = eqmain_chat_fn_10005840_guess(g_eqmain_ceb8_guess, 0x1d7, 0xd2, 0x70, 0xbc, 0, 0x21a3c);
    g_chat_list_control_guess = eqmain_chat_fn_10006950_guess(g_eqmain_ceb8_guess, 0x265, 0xc6, 0x10, 0xe5, 0);
    g_chat_other_list_guess = eqmain_chat_fn_10005840_guess(g_eqmain_ceb8_guess, 0x1d7, 0x49, 0x70, 0x6a, 0, 0x21a3c);
    g_chat_other_control_guess = eqmain_chat_fn_10006950_guess(g_eqmain_ceb8_guess, 0x265, 0x32, 0x10, 0x91, 0);
    eqmain_chat_fn_100229f0_guess();
    g_eqmain_busy_guess = 1;
    eqmain_chat_fn_10003a00_guess();
    do {
        event_code = eqmain_chat_fn_100173c0_guess();
        switch ((short)event_code) {
        case 0x12:
            result_code = 3;
            done = true;
            break;
        case 0x52:
            eqmain_chat_fn_10001320_guess((char *)0x10046088);
            break;
        case 0x53:
        case 0x75:
            result_code = 7;
            done = true;
            break;
        case 0x57:
            if (interactive_mode != 0) {
                result_code = 0x18;
                done = true;
            }
            break;
        case 0x60:
            if (interactive_mode == 0) {
                if (g_choice_count_guess < 4) {
                    if (0 < g_choice_count_guess) {
                        g_choice_result_guess = eqmain_chat_fn_10001ce0_guess(g_eqmain_ceb8_guess, g_choice_text_guess, g_choice_count_guess);
                        if (-1 < g_choice_result_guess) {
                            eqmain_set_report_opcode_guess(0x60);
                            eqmain_fn_10024fd0_guess(g_choice_result_guess);
                            eqmain_finish_hardware_report_guess();
                        }
                        g_choice_count_guess = 0;
                    }
                } else {
                    done = true;
                    result_code = 0x19;
                }
            }
            break;
        case 0x76:
            eqmain_chat_fn_10001ff0_guess(g_eqmain_ceb8_guess, (char *)0x100F4C10, g_str_888_guess, (char *)0);
            result_code = 0x12;
            done = true;
            break;
        }
        eqmain_chat_fn_10001350_guess();
        message = eqmain_chat_fn_10003bd0_guess();
        if (message != 0) {
            if (message->type == 0) {
                if (message->value.target == dialog_handle) {
                    entry_text = ((EQMainChatDialog_guess *)dialog_handle)->input_line;
                    eqmain_chat_fn_100176b0_guess(entry_text);
                    eqmain_chat_fn_100176e0_guess(entry_text);
                    if (*entry_text == '#') {
                        eqmain_chat_fn_10017730_guess(entry_text);
                        eqmain_chat_fn_10017840_guess(entry_text);
                    }
                    eqmain_chat_fn_10022970_guess(entry_text);
                    eqmain_chat_fn_10017590_guess(entry_text);
                    eqmain_chat_fn_10008aa0_guess(dialog_handle);
                } else if (message->value.target == button_0) {
                    result_code = 7;
                    done = true;
                } else if (message->value.target == button_1) {
                    result_code = 6;
                    done = true;
                } else if (message->value.target == button_2) {
                    result_code = 5;
                    done = true;
                } else if ((message->value.target == button_3) && (interactive_mode != 0)) {
                    result_code = 0x17;
                    done = true;
                } else if (message->value.target == button_4) {
                    result_code = 0x12;
                    done = true;
                }
            } else if (message->type == 1) {
                if (message->value.target == (int)g_chat_view_guess) {
                    index = 0;
                    selection = (int)g_chat_queue_guess->first;
                    selected_item = selection;
                    if (0 < g_chat_view_guess->item_count) {
                        do {
                            selected_item = 0;
                            if (selection == 0) {
                                break;
                            }
                            selection = (int)((EQMainChatQueueNode_guess *)selection)->next;
                            index = index + 1;
                            selected_item = selection;
                        } while (index < g_chat_view_guess->item_count);
                    }
                    g_chat_queue_guess->selected = (EQMainChatQueueNode_guess *)selected_item;
                    eqmain_chat_fn_10008d60_guess(g_chat_queue_guess);
                } else if (message->value.target == g_chat_list_control_guess) {
                    ((EQMainChatControl_guess *)g_eqmain_list_obj_guess)->selected_row = ((EQMainChatControl_guess *)g_chat_list_control_guess)->item_count;
                    eqmain_chat_fn_10005a00_guess(g_eqmain_list_obj_guess);
                    eqmain_fn_100067e0_guess(g_chat_list_control_guess);
                } else if (message->value.target == g_chat_other_control_guess) {
                    ((EQMainChatControl_guess *)g_chat_other_list_guess)->selected_row = ((EQMainChatControl_guess *)g_chat_other_control_guess)->item_count;
                    eqmain_chat_fn_10005a00_guess(g_chat_other_list_guess);
                    eqmain_fn_100067e0_guess(g_chat_other_control_guess);
                } else if (message->value.target == g_eqmain_list_obj_guess) {
                    selected_item = eqmain_chat_fn_10005e10_guess(g_eqmain_list_obj_guess);
                    if (selected_item != 0) {
                        entry_text = ((EQMainChatEntry_guess *)selected_item)->text;
                        text_length = (int)strlen(entry_text);
                        if (text_length != 0) {
                            entry_text = ((EQMainChatDialog_guess *)dialog_handle)->input_line;
                            if (strlen(entry_text) == 0) {
                                eqmain_chat_fn_10008970_guess(dialog_handle, g_chat_separator_guess);
                                eqmain_chat_fn_10008970_guess(dialog_handle, ((EQMainChatEntry_guess *)selected_item)->text);
                                eqmain_chat_fn_10008970_guess(dialog_handle, g_chat_suffix_guess);
                            } else {
                                eqmain_chat_fn_10008970_guess(dialog_handle, ((EQMainChatEntry_guess *)selected_item)->text);
                            }
                            ((EQMainChatControl_guess *)g_eqmain_list_obj_guess)->selected_entry = -1;
                            eqmain_chat_fn_10005a00_guess(g_eqmain_list_obj_guess);
                        }
                    }
                } else if (message->value.target == g_chat_other_list_guess) {
                    eqmain_chat_fn_10021b00_guess(((EQMainChatControl_guess *)g_chat_other_list_guess)->selected_row);
                }
            } else if (message->type == 4) {
                switch ((char)message->value.key) {
                case '\t':
                    if (g_chat_page_enabled_guess != 0) {
                        strcpy(g_eqmain_copybuf_guess, g_chat_separator_guess);
                        strcat(g_eqmain_copybuf_guess, eqmain_chat_fn_10016a10_guess());
                        strcat(g_eqmain_copybuf_guess, g_chat_suffix_guess);
                        strcpy(((EQMainChatDialog_guess *)dialog_handle)->input_line, g_eqmain_copybuf_guess);
                        g_chat_dialog_guess->input_length = g_chat_dialog_guess->input_length + (int)strlen(g_eqmain_copybuf_guess);
                    }
                    break;
                case 0x1b:
                    eqmain_chat_fn_10003a00_guess();
                    done = true;
                    break;
                case -0x7e:
                    if (0 < g_chat_page_index_guess) {
                        g_chat_page_index_guess = g_chat_page_index_guess - 1;
                    }
                    eqmain_chat_fn_10008aa0_guess(dialog_handle);
                    eqmain_chat_fn_10008970_guess(dialog_handle, g_chat_page_texts_guess + g_chat_page_index_guess * 0xfe);
                    break;
                case -0x7d:
                    if (g_chat_page_index_guess < g_eqmain_line_count_guess) {
                        g_chat_page_index_guess = g_chat_page_index_guess + 1;
                    }
                    eqmain_chat_fn_10008aa0_guess(dialog_handle);
                    if (g_chat_page_index_guess < g_eqmain_line_count_guess) {
                        eqmain_chat_fn_10008970_guess(dialog_handle, g_chat_page_texts_guess + g_chat_page_index_guess * 0xfe);
                    }
                    break;
                }
            }
            eqmain_free_guess(message);
        }
        if (done) {
            g_eqmain_busy_guess = 0;
            g_chat_dialog_guess = (EQMainChatDialog_guess *)0;
            eqmain_chat_fn_10022b10_guess();
            eqmain_fn_100011a0_guess();
            eqmain_chat_fn_10001700_guess(button_1);
            eqmain_chat_fn_10001700_guess(button_0);
            eqmain_chat_fn_10001700_guess(button_2);
            if (button_3 != 0) {
                eqmain_chat_fn_10001700_guess(button_3);
            }
            eqmain_chat_fn_10001700_guess(button_4);
            eqmain_chat_fn_10008ce0_guess(g_chat_queue_guess);
            eqmain_chat_fn_10008a00_guess(dialog_handle);
            eqmain_chat_fn_10006ae0_guess((int)g_chat_view_guess);
            eqmain_chat_fn_10005930_guess(g_eqmain_list_obj_guess);
            eqmain_chat_fn_10006ae0_guess(g_chat_list_control_guess);
            eqmain_chat_fn_10005930_guess(g_chat_other_list_guess);
            eqmain_chat_fn_10006ae0_guess(g_chat_other_control_guess);
            eqmain_fn_100039e0_arg_guess(0);
            g_chat_status_guess = (short)((-(unsigned short)(interactive_mode != 0) & 0xffe5) + 0x1f);
            if ((interactive_mode != 0) && ((short)result_code == 0x12)) {
                eqmain_chat_fn_100179c0_guess((unsigned short *)&result_code);
                return result_code;
            }
            if ((short)result_code == 7) {
                g_eqmain_startup_done_guess = 0;
            }
            return result_code;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x10018440
// Logs a line, loads it into the text block g_chat_queue_guess with a font, then repositions and refreshes the view.
// @early-stop: rewrite loads +0x2c into ESI and restores it after the subtract; original restores ESI before JL and subtracts directly from memory.
// __cdecl, two arguments (callee pops 0 bytes). Logs the line through EQMain_LogStampedLine_guess when the log FILE * g_eqmain_log_file_guess is set.
// Stores EQMain_ArialFontForLanguage_guess(value, 14, 0) at +0x38 of the queue g_chat_queue_guess, then hands the queue and the line to WrapTextLines_guess.
// The view g_chat_view_guess gets +0x64 = the lesser of the queue's +0x14 and +0x18, minus +0x2c, plus 1, at least 1, and +0x68 =
// the dword that the pointer at +4 of the object at the queue's +0x28 points to. RefreshSlider_guess then refreshes the view.
#define g_eqmain_queue_guess      EQ_GLOBAL(int *, 0x1004F6E0)                  // g_chat_queue_guess: the queue object, as dwords
#define g_eqmain_view_guess       EQ_GLOBAL(int, 0x1004ECEC)                    // g_chat_view_guess: the view object's address
#define eqmain_fn_100165f0_guess  EQ_CDECL(void, 0x100165F0, (char *))          // __cdecl, logs the line
#define eqmain_fn_10008150_guess  EQ_CDECL(int, 0x10008150, (int, int, int))    // __cdecl, (value, 14, 0) lookup
#define eqmain_fn_10009370_guess  EQ_CDECL(void, 0x10009370, (int *, char *))   // __cdecl, (queue, line)
#define eqmain_fn_100067e0_guess  EQ_CDECL(void, 0x100067E0, (int))             // __cdecl, refreshes the view

void EQMain_UpdateQueueView_guess(char *line, int value)
{
    int tmp;
    int count;

    if (g_eqmain_log_file_guess != NULL) {
        eqmain_fn_100165f0_guess(line);
    }
    tmp = eqmain_fn_10008150_guess(value, 0xe, 0);
    g_eqmain_queue_guess[0xe] = tmp;
    eqmain_fn_10009370_guess(g_eqmain_queue_guess, line);
    if (g_eqmain_queue_guess[6] < g_eqmain_queue_guess[5]) {
        count = g_eqmain_queue_guess[6];
    } else {
        count = g_eqmain_queue_guess[5];
    }
    tmp = count - g_eqmain_queue_guess[0xb] + 1;
    if (tmp < 1) {
        tmp = 1;
    }
    *(int *)(g_eqmain_view_guess + 100) = tmp;
    *(int *)(g_eqmain_view_guess + 0x68) = **(int **)(g_eqmain_queue_guess[10] + 4);
    eqmain_fn_100067e0_guess(g_eqmain_view_guess);
}

// FUNCTION: EQMAIN 0x100184D0
// Loads up to six text entries from the front-end source table.
// @early-stop: best tested loop shape still computes the table address from the incremented counter (similarity 0.76, ndiff 17).
// __cdecl, one argument (callee pops 0 bytes). Stores the signed 16-bit count at +0x400 of sourceTable in g_eqmain_name_count_guess, clamped to 6.
// For each entry i below that count (the 64-byte rows at sourceTable + i*0x40), copies DecodeToBuffer_guess(row) with strcpy into the
// 256-byte slot i at g_eqmain_name_slots_guess and stores the slot's address at g_eqmain_name_ptrs_guess[i]. No length check: a string of 256 or more
// bytes runs into the next slot.
#define g_eqmain_name_count_guess   EQ_GLOBAL(short, 0x10051AD8)     // g_eqmain_name_count_guess: the string count, signed 16-bit
#define g_eqmain_name_ptrs_guess    EQ_ARRAY(char *, 0x10051A54)     // g_eqmain_name_ptrs_guess: table of 4-byte slot pointers
#define g_eqmain_name_slots_guess   EQ_ARRAY(char, 0x1004F894)       // g_eqmain_name_slots_guess: the string slots, 0x100 bytes each

void EQMain_CopyStringTable_guess(char *sourceTable)
{
    char *destinationBuffer;
    char *sourceText;
    short index;

    g_eqmain_name_count_guess = *(short *)(sourceTable + 0x400);
    if (6 < g_eqmain_name_count_guess) {
        g_eqmain_name_count_guess = 6;
    }
    index = 0;
    if (0 < g_eqmain_name_count_guess) {
        do {
            destinationBuffer = g_eqmain_name_slots_guess + index * 0x100;
            sourceText = (char *)eqmain_fn_10021110_guess((unsigned char *)(sourceTable + index * 0x40));
            strcpy(destinationBuffer, sourceText);
            index = index + 1;
            g_eqmain_name_ptrs_guess[index - 1] = destinationBuffer;
        } while (index < g_eqmain_name_count_guess);
    }
}
