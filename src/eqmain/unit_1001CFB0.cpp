// unit_1001CFB0.cpp - eqmain.dll, the address run 0x1001CFB0..0x1001D3C0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <windows.h>
#include <string.h>

#include "trilogy/eqaddr.h"

// eqmain.dll globals, unnamed: the dialog window and its edit control (0x3EA) that EQMain_SetItemText_guess also uses, the
// 0x8000-byte text buffer at 0x100E49C0, and the three strings for the two buttons and the window title.
#define g_dialog_guess            EQ_GLOBAL(HWND, 0x100EC9C0)
#define g_dialog_edit_guess       EQ_GLOBAL(HWND, 0x100EC9C4)
#define g_dialog_text_guess       EQ_ARRAY(char, 0x100E49C0)
#define g_button_3ec_text_guess   EQ_GLOBAL(char *, 0x10055A50)
#define g_button_3eb_text_guess   EQ_GLOBAL(char *, 0x10055A54)
#define g_title_text_guess        EQ_GLOBAL(char *, 0x10055A58)

// Nine 3000-byte text records in a row from 0x100D5F60 (the records the loader before EQMain_SetItemText_guess fills).
#define g_eqmain_license_buf_guess           EQ_ARRAY(char, 0x100D5F60)
#define g_record1_guess           EQ_ARRAY(char, 0x100D6B18)
#define g_record2_guess           EQ_ARRAY(char, 0x100D76D0)
#define g_record3_guess           EQ_ARRAY(char, 0x100D8288)
#define g_record4_guess           EQ_ARRAY(char, 0x100D8E40)
#define g_record5_guess           EQ_ARRAY(char, 0x100D99F8)
#define g_record6_guess           EQ_ARRAY(char, 0x100DA5B0)
#define g_record7_guess           EQ_ARRAY(char, 0x100DB168)
#define g_record8_guess           EQ_ARRAY(char, 0x100DBD20)

// Identity_guess: cdecl, returns the text it is given (here with a buffer and its size); DecodeToBuffer_guess: cdecl, maps a
// record to its text; EQMain_SetItemText_guess: cdecl, sets the text of a control of g_dialog_guess.
#define eqmain_fn_100210f0_guess  EQ_CDECL(char *, 0x100210F0, (char *, char *, int))
#define eqmain_fn_10021110_guess  EQ_CDECL(char *, 0x10021110, (char *))
#define eqmain_fn_1001cf80_guess  EQ_CDECL(void, 0x1001CF80, (LPARAM, int))

#define ID_TEXT_EDIT    0x3ea
#define ID_BUTTON_NO    0x3eb
#define ID_BUTTON_YES   0x3ec

// FUNCTION: EQMAIN 0x1001CFB0
// Dialog procedure: WM_INITDIALOG fills the buttons, title and edit control from the records; the two buttons end it.
BOOL CALLBACK HandleNineRecordYesNoDialog_guess(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    char buffer[0x800];

    memset(buffer, 0, sizeof(buffer));
    switch (message)
    {
    case WM_INITDIALOG:
        g_dialog_guess = dialog;
        SetDlgItemTextA(g_dialog_guess, ID_BUTTON_YES, eqmain_fn_100210f0_guess(g_button_3ec_text_guess, buffer, 0x800));
        SetDlgItemTextA(g_dialog_guess, ID_BUTTON_NO, eqmain_fn_100210f0_guess(g_button_3eb_text_guess, buffer, 0x800));
        SetWindowTextA(dialog, eqmain_fn_100210f0_guess(g_title_text_guess, buffer, 0x800));
        memset(g_dialog_text_guess, 0, 0x8000);
        strcpy(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_eqmain_license_buf_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record1_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record2_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record3_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record4_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record5_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record6_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record7_guess), buffer, 0x800));
        strcat(g_dialog_text_guess, eqmain_fn_100210f0_guess(eqmain_fn_10021110_guess(g_record8_guess), buffer, 0x800));
        eqmain_fn_1001cf80_guess((LPARAM)g_dialog_text_guess, ID_TEXT_EDIT);
        g_dialog_edit_guess = GetDlgItem(g_dialog_guess, ID_TEXT_EDIT);
        SendMessageA(g_dialog_edit_guess, EM_SETREADONLY, 0, 0);
        SendDlgItemMessageA(dialog, ID_TEXT_EDIT, WM_VSCROLL, SB_TOP, 0);
        SendMessageA(g_dialog_edit_guess, EM_SETREADONLY, 1, 0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_BUTTON_NO:
            EndDialog(dialog, 0);
            return 1;
        case ID_BUTTON_YES:
            EndDialog(dialog, 1);
            return 1;
        }
        break;
    }
    return 0;
}

// HandleTwoRecordYesNoDialog_guess's globals (same dialog as above, its own names).
#define g_dialog_100ec9c0_guess   EQ_GLOBAL(HWND, 0x100EC9C0)
#define g_str_10055a5c_guess      EQ_GLOBAL(char *, 0x10055A5C)
#define g_str_10055a60_guess      EQ_GLOBAL(char *, 0x10055A60)
#define g_raw_100dc8d8_guess      EQ_ARRAY(unsigned char, 0x100DC8D8)
#define g_raw_100dd490_guess      EQ_ARRAY(unsigned char, 0x100DD490)
#define eqmain_convert_report_token_guess  EQ_CDECL(char *, 0x10021110, (unsigned char *))
#define eqmain_fn_1001cf80_p_guess   EQ_CDECL(void, 0x1001CF80, (char *, int))
#define TEXT_BUFFER_SIZE  0x800

// FUNCTION: EQMAIN 0x1001d3c0
// Presents a two-part message and lets the player choose a response.
// Dialog procedure (stdcall, ret 0x10): WM_INITDIALOG fills the texts and the read-only edit 0x3ED; WM_COMMAND 0x3EB/0x3EC ends it.
BOOL __stdcall HandleTwoRecordYesNoDialog_guess(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    char text[TEXT_BUFFER_SIZE];

    memset(text, 0, sizeof(text));
    switch (message)
    {
    case WM_INITDIALOG:
        g_dialog_100ec9c0_guess = dialog;
        SetDlgItemTextA(g_dialog_100ec9c0_guess, 0x3ec, eqmain_fn_100210f0_guess(g_button_3ec_text_guess, text, TEXT_BUFFER_SIZE));
        SetDlgItemTextA(g_dialog_100ec9c0_guess, 0x3eb, eqmain_fn_100210f0_guess(g_button_3eb_text_guess, text, TEXT_BUFFER_SIZE));
        SetWindowTextA(dialog, eqmain_fn_100210f0_guess(g_str_10055a5c_guess, text, TEXT_BUFFER_SIZE));
        memset(g_dialog_text_guess, 0, 0x8000);
        strcpy(g_dialog_text_guess,
               eqmain_fn_100210f0_guess(eqmain_convert_report_token_guess(g_raw_100dc8d8_guess), text, TEXT_BUFFER_SIZE));
        strcat(g_dialog_text_guess,
               eqmain_fn_100210f0_guess(eqmain_convert_report_token_guess(g_raw_100dd490_guess), text, TEXT_BUFFER_SIZE));
        eqmain_fn_1001cf80_p_guess(g_dialog_text_guess, 0x3ed);
        g_dialog_edit_guess = GetDlgItem(g_dialog_100ec9c0_guess, 0x3ed);
        SendMessageA(g_dialog_edit_guess, EM_SETREADONLY, 0, 0);
        SendDlgItemMessageA(dialog, 0x3ed, WM_VSCROLL, SB_TOP, 0);
        SendDlgItemMessageA(dialog, 0x3ee, WM_SETTEXT, 0, (LPARAM)g_str_10055a60_guess);
        SendMessageA(g_dialog_edit_guess, EM_SETREADONLY, 1, 0);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 0x3eb:
            EndDialog(dialog, 0);
            return TRUE;
        case 0x3ec:
            EndDialog(dialog, 1);
            return TRUE;
        }
        break;
    }
    return FALSE;
}
