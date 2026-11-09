// unit_10018560.cpp - eqmain.dll, the address run 0x10018560..0x10018990 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include <string.h>

#include "trilogy/eqaddr.h"

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
#define g_eqmain_dword_a_guess       EQ_GLOBAL(unsigned int, 0x10052DB4)   // g_eqmain_dword_a_guess: the first dword read
#define g_eqmain_dword_b_guess       EQ_GLOBAL(unsigned int, 0x10052DB0)   // g_eqmain_dword_b_guess: the second dword read
#define g_eqmain_mode_guess          EQ_GLOBAL(int, 0x10055034)            // g_eqmain_last_index_guess: the mode, clamped to -1..5
#define g_eqmain_text12_guess        EQ_ARRAY(char, 0x10055028)            // g_eqmain_text12_guess: 12-byte field
#define g_eqmain_buf_c60_guess        EQ_ARRAY(char, 0x10051C60)            // g_eqmain_buf_c60_guess: 64-byte field
#define g_eqmain_buf_e10_guess      EQ_ARRAY(char, 0x10051E10)            // g_eqmain_buf_e10_guess: 4000-byte field
#define g_eqmain_buf_ca4_guess       EQ_ARRAY(char, 0x10051CA4)            // g_eqmain_buf_ca4_guess: 300-byte field
#define g_eqmain_str_dd0_guess   EQ_ARRAY(char, 0x10051DD0)            // g_eqmain_str_dd0_guess: copy of the 64-byte field
#define g_eqmain_drive_base_guess  EQ_ARRAY(char, 0x1004CE40)            // g_eqmain_drive_base_guess: built-in default text
#define g_eqmain_table_count_guess   EQ_GLOBAL(short, 0x10051AD8)             // g_eqmain_name_count_guess: the string count, signed 16-bit
#define g_eqmain_table_guess         EQ_ARRAY(unsigned int, 0x10051A54)       // g_eqmain_name_ptrs_guess: table of 4-byte string pointers
#define g_eqmain_list_guess          EQ_GLOBAL(unsigned int, 0x10051950)      // g_eqmain_list_guess: pointer to the list object
#define g_eqmain_listbox_guess       EQ_GLOBAL(unsigned int, 0x10051894)      // g_eqmain_listbox_guess: pointer to the list box object
#define g_eqmain_device_text_guess   EQ_ARRAY(char, 0x10051954)              // g_eqmain_device_text_guess: 256-byte buffer, the start of eqdevice.txt
#define g_eqmain_device_index_guess  EQ_GLOBAL(int, 0x10051AD4)              // g_eqmain_device_index_guess: index of the table string that matched
#define g_eqmain_fopen_mode_guess     EQ_ARRAY(char, 0x100480A4)              // g_eqmain_fopen_mode_guess: mode string passed to FUN_1003407f
#define g_eqmain_lang_names_guess    EQ_ARRAY(char, 0x10051898)            // g_eqmain_lang_names_guess: five 32-byte language name slots (0xa0 bytes)
#define g_eqmain_lang_ptrs_guess     EQ_ARRAY(char *, 0x10051938)          // g_eqmain_lang_ptrs_guess: table of five pointers, one to each slot
#define g_eqmain_lang_english_guess  EQ_ARRAY(char, 0x10048070)            // g_eqmain_lang_english_guess: "English"
#define g_eqmain_lang_unknown_guess  EQ_ARRAY(char, 0x10048064)            // g_eqmain_lang_unknown_guess: second name, not in Ghidra's string list
#define g_eqmain_lang_deutsch_guess  EQ_ARRAY(char, 0x1004805C)            // g_eqmain_lang_deutsch_guess: "Deutsch"
#define g_eqmain_lang_japanese_guess EQ_ARRAY(char, 0x10048050)            // g_eqmain_lang_japanese_guess: "(Japanese)"
#define g_eqmain_lang_korean_guess   EQ_ARRAY(char, 0x10048044)            // g_eqmain_lang_korean_guess: "(Korean)"
#define g_eqmain_ceb8_guess         EQ_GLOBAL(int, 0x1004CEB8)            // g_eqmain_ceb8_guess: the screen object the controls are created on
#define g_eqmain_flag_ce78_guess  EQ_GLOBAL(unsigned char, 0x1004CE78)  // g_eqmain_flag_ce78_guess: 1 while the device select loop runs
#define g_eqmain_ptr_10055888_guess   EQ_GLOBAL(unsigned int, 0x10055888)   // g_str_888_guess: the string pointer eqmain_draw_text_guess gets
#define g_eqmain_str_seldisplay_guess EQ_ARRAY(char, 0x10048088)            // g_eqmain_str_seldisplay_guess: "..."
#define g_eqmain_str_eqdevice_guess   EQ_ARRAY(char, 0x10048078)            // g_eqmain_str_eqdevice_guess: "..."
#define g_str_10047f74_guess     EQ_ARRAY(char, 0x10047F74)            // DAT_10047f74: mode string for the eqdevice.txt write

#define eqmain_fn_100250a0_guess     EQ_CDECL(void, 0x100250A0, (unsigned int))
#define eqmain_fn_10025120_guess     EQ_STDCALL(unsigned int, 0x10025120, (void))
#define eqmain_fn_10025150_guess     EQ_STDCALL(unsigned char *, 0x10025150, (void))
#define eqmain_fn_10021110_guess     EQ_CDECL(char *, 0x10021110, (unsigned char *))
#define eqmain_fn_10033330_guess     EQ_CDECL(char *, 0x10033330, (char *, const char *, unsigned int))
#define eqmain_fn_10005cc0_guess     EQ_CDECL(unsigned int *, 0x10005CC0, (int, unsigned int, char *, unsigned int, unsigned int))
#define eqmain_fn_10005a00_guess     EQ_CDECL(void, 0x10005A00, (int))
#define eqmain_fn_100067e0_guess     EQ_CDECL(void, 0x100067E0, (int))
#define eqmain_fn_1003407f_guess     EQ_CDECL(void *, 0x1003407F, (const char *, const char *))
#define eqmain_fn_100342a2_guess     EQ_CDECL(unsigned int, 0x100342A2, (void *, unsigned int, unsigned int, void *))
#define eqmain_fn_100343b9_guess     EQ_CDECL(int, 0x100343B9, (void *))
#define eqmain_fn_10033730_guess     EQ_CDECL(int, 0x10033730, (char *, char *))
#define eqmain_fn_10001b60_guess     EQ_STDCALL(void, 0x10001B60, (void))
#define eqmain_fn_10007cf0_guess     EQ_CDECL(void, 0x10007CF0, (int))
#define eqmain_fn_10007ae0_guess     EQ_CDECL(int *, 0x10007AE0, (const char *, int))
#define eqmain_fn_10006c20_guess     EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
#define eqmain_fn_10001520_guess     EQ_CDECL(void *, 0x10001520, (int, int, int, unsigned char, int))
#define eqmain_fn_100017b0_guess     EQ_CDECL(void, 0x100017B0, (void *, int, unsigned int, int, int))
#define eqmain_fn_10005840_guess     EQ_CDECL(unsigned int *, 0x10005840, (int, int, int, int, int, unsigned int, unsigned int))
#define eqmain_fn_10006950_guess     EQ_CDECL(unsigned int *, 0x10006950, (int, int, int, int, int, int))
#define eqmain_fn_10018640_guess     EQ_STDCALL(void, 0x10018640, (void))
#define eqmain_fn_10003a00_guess     EQ_STDCALL(void, 0x10003A00, (void))
#define eqmain_fn_10003bd0_guess     EQ_STDCALL(int *, 0x10003BD0, (void))
#define eqmain_fn_10033568_guess     EQ_CDECL(void, 0x10033568, (int *))
#define eqmain_fn_10034169_guess     EQ_CDECL(size_t, 0x10034169, (void *, size_t, size_t, void *))
#define eqmain_fn_10001700_guess     EQ_CDECL(void, 0x10001700, (void *))
#define eqmain_fn_10005930_guess     EQ_CDECL(void, 0x10005930, (unsigned int))
#define eqmain_fn_10006ae0_guess     EQ_CDECL(void, 0x10006AE0, (unsigned int))
#define eqmain_fn_10007a80_guess     EQ_CDECL(void, 0x10007A80, (int *))
#define eqmain_fn_10001c30_guess     EQ_STDCALL(void, 0x10001C30, (void))

// FUNCTION: EQMAIN 0x10018560
// Prepares the front end's language names for its language selection UI.
// __stdcall, void, no arguments. Clears the five 32-byte language name slots at g_eqmain_lang_names_guess (0xa0 bytes), copies
// the five names into them in slot order (English, the string at g_eqmain_lang_unknown_guess, Deutsch, (Japanese), (Korean)), then
// sets the five pointers in the table at g_eqmain_lang_ptrs_guess to the slots, 0x20 bytes apart. Meanings unknown.
void __stdcall InitLanguageNames_guess(void)
{
    int i;
    char *p;
    char **table;

    memset(g_eqmain_lang_names_guess, 0, 0xa0);
    strcpy(g_eqmain_lang_names_guess, g_eqmain_lang_english_guess);
    strcpy(g_eqmain_lang_names_guess + 0x20, g_eqmain_lang_unknown_guess);
    strcpy(g_eqmain_lang_names_guess + 0x40, g_eqmain_lang_deutsch_guess);
    strcpy(g_eqmain_lang_names_guess + 0x60, g_eqmain_lang_japanese_guess);
    strcpy(g_eqmain_lang_names_guess + 0x80, g_eqmain_lang_korean_guess);

    p = g_eqmain_lang_names_guess;
    table = g_eqmain_lang_ptrs_guess;
    i = 5;
    do
    {
        *table = p;
        p += 0x20;
        table++;
        i--;
    } while (i != 0);
}

// FUNCTION: EQMAIN 0x10018640
// Populates the display-device choices shown in the front-end selector.
// __stdcall, void, no arguments. For each of the g_eqmain_name_count_guess strings (signed 16-bit count) in the table at
// g_eqmain_name_ptrs_guess, calls EnqueueNamedRecord_guess(list, 0xe0, string, 0, 0x22d69), where list is the object at g_eqmain_list_guess. Then
// calls DrawNamedRows_guess(list), sets the list box at g_eqmain_listbox_guess: +0x64 = list[+0x24] - list[+0x30] + 1 and
// +0x68 = list[+0x2c], and calls RefreshSlider_guess(list box). The meanings of the globals and fields are unknown.
void __stdcall FillListFromStringTable_guess(void)
{
    int i = 0;
    unsigned int *p;

    if (g_eqmain_table_count_guess > 0)
    {
        p = g_eqmain_table_guess;
        do
        {
            eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0xe0, (char *)*p, 0, 0x22d69);
            i = i + 1;
            p = p + 1;
        } while (i < g_eqmain_table_count_guess);
    }
    eqmain_fn_10005a00_guess(g_eqmain_list_guess);
    *(int *)(g_eqmain_listbox_guess + 0x64) = (*(int *)(g_eqmain_list_guess + 0x24) - *(int *)(g_eqmain_list_guess + 0x30)) + 1;
    *(unsigned int *)(g_eqmain_listbox_guess + 0x68) = *(unsigned int *)(g_eqmain_list_guess + 0x2c);
    eqmain_fn_100067e0_guess(g_eqmain_listbox_guess);
}

// FUNCTION: EQMAIN 0x100186D0
// Lets the player choose a display device for the front end.
// __stdcall, returns g_eqmain_device_index_guess in AX, or 0 when the signed 16-bit count g_eqmain_name_count_guess is 0. Shows seldisplay.bmp,
// fills the list box (FillListFromStringTable_guess) and loops over the events from PumpMessagesAndEditText_guess (each one freed with
// _free): type 0 with the control from eqmain_create_button_guess as target ends the loop; type 1 on the list box copies its +0x68 to
// the list's +0x2c and calls DrawNamedRows_guess; type 4 ends the loop on '\r' and on Escape (which also calls TickBlinkAndCursor_guess).
// Then g_eqmain_device_index_guess = list[+0x34] (-1 becomes 0). When eqdevice.txt opens, the table string is copied into g_eqmain_device_text_guess
// and 256 bytes of that buffer are written. The controls are freed after. The meanings of the globals are unknown.
short __stdcall SelectDisplayDevice_guess(void)
{
    bool done = false;
    int *image;     // seldisplay.bmp object: [0] width, [1] height
    void *control;  // eqmain_create_button_guess's result (HDC in Ghidra)
    int *msg;       // event from PumpMessagesAndEditText_guess: [0] type, [2] target or key
    void *file;     // FILE *

    if (g_eqmain_table_count_guess == 0)
    {
        return 0;
    }
    eqmain_fn_10001b60_guess();
    eqmain_fn_10007cf0_guess(g_eqmain_ceb8_guess);
    image = eqmain_fn_10007ae0_guess(g_eqmain_str_seldisplay_guess, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x6c, 0x32, (int)image, 0, 0, image[0], image[1], 1);
    control = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x16a, 0x11, 2);
    eqmain_fn_100017b0_guess(control, 0, g_eqmain_ptr_10055888_guess, 1, 0);
    g_eqmain_list_guess = (unsigned int)eqmain_fn_10005840_guess(g_eqmain_ceb8_guess, 0xc0, 0x7a, 0xe0, 0x4e, 0, 0x22d69);
    g_eqmain_listbox_guess = (unsigned int)eqmain_fn_10006950_guess(g_eqmain_ceb8_guess, 0x8a, 0x5e, 0x10, 0xc5, 0);
    *(int *)(g_eqmain_list_guess + 0x34) = 0;
    eqmain_fn_10018640_guess();
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_10003a00_guess();
    do
    {
        msg = eqmain_fn_10003bd0_guess();
        if (msg != 0)
        {
            switch (msg[0])
            {
            case 0:
                if ((void *)msg[2] == control)
                {
                    done = true;
                }
                break;
            case 1:
                if ((unsigned int)msg[2] == g_eqmain_listbox_guess)
                {
                    if (*(int *)(g_eqmain_listbox_guess + 0x68) < 0)
                    {
                        *(int *)(g_eqmain_listbox_guess + 0x68) = 0;
                    }
                    *(unsigned int *)(g_eqmain_list_guess + 0x2c) = *(unsigned int *)(g_eqmain_listbox_guess + 0x68);
                    eqmain_fn_10005a00_guess(g_eqmain_list_guess);
                }
                break;
            case 4:
                if ((char)msg[2] != '\r')
                {
                    if ((char)msg[2] != '\x1b')
                    {
                        break;
                    }
                    eqmain_fn_10003a00_guess();
                }
                done = true;
                break;
            }
            eqmain_fn_10033568_guess(msg);
        }
    } while (!done);
    g_eqmain_flag_ce78_guess = 0;
    g_eqmain_device_index_guess = *(int *)(g_eqmain_list_guess + 0x34);
    if (g_eqmain_device_index_guess == -1)
    {
        g_eqmain_device_index_guess = 0;
    }
    file = eqmain_fn_1003407f_guess(g_eqmain_str_eqdevice_guess, g_str_10047f74_guess);
    if (file != 0)
    {
        strcpy(g_eqmain_device_text_guess, (char *)g_eqmain_table_guess[g_eqmain_device_index_guess]);
        eqmain_fn_10034169_guess(g_eqmain_device_text_guess, 0x100, 1, file);
        eqmain_fn_100343b9_guess(file);
    }
    eqmain_fn_10001700_guess(control);
    eqmain_fn_10005930_guess(g_eqmain_list_guess);
    eqmain_fn_10006ae0_guess(g_eqmain_listbox_guess);
    eqmain_fn_10007a80_guess(image);
    eqmain_fn_10001c30_guess();
    return (short)g_eqmain_device_index_guess;
}

// FUNCTION: EQMAIN 0x10018910
// Restores the saved display-device choice when the front end starts.
// __stdcall, unsigned char (AL, 0 or 1), no arguments. Opens "..." with FUN_1003407f (mode string at g_eqmain_fopen_mode_guess), reads its
// first 0x100 bytes into g_eqmain_device_text_guess and closes it. When the open worked and the signed 16-bit count g_eqmain_name_count_guess is
// above 0, compares the buffer with each string of the table at g_eqmain_name_ptrs_guess (__strcmpi, __strcmpi). On the first
// match it stores that index in g_eqmain_device_index_guess and returns 1. Otherwise (open failed, no strings, no match) it returns 0.
unsigned char __stdcall FindEqDeviceIndex_guess(void)
{
    void *file;     // FILE *
    short i;

    file = eqmain_fn_1003407f_guess(g_eqmain_str_eqdevice_guess, g_eqmain_fopen_mode_guess);
    if (file != 0)
    {
        eqmain_fn_100342a2_guess(g_eqmain_device_text_guess, 0x100, 1, file);
        eqmain_fn_100343b9_guess(file);
        for (i = 0; i < g_eqmain_table_count_guess; i = i + 1)
        {
            if (eqmain_fn_10033730_guess(g_eqmain_device_text_guess, (char *)g_eqmain_table_guess[i]) == 0)
            {
                g_eqmain_device_index_guess = i;
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: EQMAIN 0x10018990
// Loads text fields and a mode from a received front-end record; their purpose is unclear.
// __cdecl, void, one dword argument: the stream pointer that SetStateAndDword_guess stores at 0x1012B914 (it also resets the
// read cursor at 0x1012B91C to 2). Reads two dwords into g_eqmain_dword_a_guess and g_eqmain_dword_b_guess. When the first is nonzero,
// reads the mode dword and four strings (12, 64, 4000 and 300 bytes), each through DecodeToBuffer_guess. When it is zero,
// sets the mode to 0, fills the 12, 4000 and 300 byte fields from the built-in text at 0x1004CE40 and reads only the
// 64-byte field from the stream. Then copies the 64-byte field to 0x10051DD0 and sets a mode outside -1..5 to 0.
// The meanings of the globals are unknown.
void __cdecl LoadTextRecord_guess(unsigned int stream)
{
    eqmain_fn_100250a0_guess(stream);
    g_eqmain_dword_a_guess = eqmain_fn_10025120_guess();
    g_eqmain_dword_b_guess = eqmain_fn_10025120_guess();
    if (g_eqmain_dword_a_guess != 0)
    {
        g_eqmain_mode_guess = eqmain_fn_10025120_guess();
        eqmain_fn_10033330_guess(g_eqmain_text12_guess, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 0xc);
        eqmain_fn_10033330_guess(g_eqmain_buf_c60_guess, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 0x40);
        eqmain_fn_10033330_guess(g_eqmain_buf_e10_guess, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 4000);
        eqmain_fn_10033330_guess(g_eqmain_buf_ca4_guess, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 300);
    }
    else
    {
        g_eqmain_mode_guess = 0;
        eqmain_fn_10033330_guess(g_eqmain_text12_guess, g_eqmain_drive_base_guess, 0xc);
        eqmain_fn_10033330_guess(g_eqmain_buf_c60_guess, eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()), 0x40);
        eqmain_fn_10033330_guess(g_eqmain_buf_e10_guess, g_eqmain_drive_base_guess, 4000);
        eqmain_fn_10033330_guess(g_eqmain_buf_ca4_guess, g_eqmain_drive_base_guess, 300);
    }
    eqmain_fn_10033330_guess(g_eqmain_str_dd0_guess, g_eqmain_buf_c60_guess, 0x40);
    if (g_eqmain_mode_guess < -1 || g_eqmain_mode_guess > 5)
    {
        g_eqmain_mode_guess = 0;
    }
}
