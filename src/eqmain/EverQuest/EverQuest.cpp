// EverQuest.cpp - eqmain.dll, the EverQuest subsystem's own functions (this image's VAs, not trilogy's).

#include <string.h>
#include "trilogy/eqaddr.h"

// eqmain.dll's globals and functions used by CopyFirstWord_guess (this image's VAs).
#define g_eqmain_list_guess             EQ_GLOBAL(int, 0x1004CF64)                                         // g_eqmain_cf64_guess: the list the rows go to (+0x24, +0x2c top, +0x30)
#define g_eqmain_scrollbar_guess        EQ_GLOBAL(int, 0x1004CF28)                                         // g_eqmain_cf28_guess: the scrollbar (+0x64 rows, +0x68 top)
#define g_eqmain_hdr_a_guess            EQ_GLOBAL(int, 0x1004D23C)                                         // g_eqmain_hdr_a_guess: the first header dword
#define g_eqmain_d238_guess            EQ_GLOBAL(int, 0x1004D238)                                         // g_eqmain_d238_guess: the second header dword
#define g_eqmain_record_count_guess     EQ_GLOBAL(unsigned int, 0x1004D240)                                // g_eqmain_record_count_guess: the record count (third header dword)
#define g_chat_suffix_guess            EQ_ARRAY(char, 0x10046608)                                         // g_chat_suffix_guess: the text of the closing row of each record
#define eqmain_fn_100250a0_guess        EQ_CDECL(void, 0x100250A0, (int))                                  // SetStateAndDword_guess: stores param_1 as the read base (g_eqmain_value_b914_guess)
#define eqmain_fn_10025120_guess        EQ_STDCALL(int, 0x10025120, (void))                                // NextDword_guess: next dword of the read base
#define eqmain_fn_10025150_guess        EQ_STDCALL(unsigned char *, 0x10025150, (void))                    // NextString_guess: next string pointer of the read base
#define eqmain_fn_10021110_guess        EQ_CDECL(int *, 0x10021110, (unsigned char *))                     // DecodeToBuffer_guess: the text for a string pointer
#define eqmain_fn_10005cc0_guess        EQ_CDECL(void *, 0x10005CC0, (int, int, char *, int, int))         // EnqueueNamedRecord_guess: adds a node (malloc 0x20, strdup of the text)
#define eqmain_fn_10005d20_guess        EQ_CDECL(void *, 0x10005D20, (int, int, int, char *, int))         // EnqueueOwnedRecord_guess: adds a node under another (malloc 0x20, strdup)
#define eqmain_fn_10005ea0_guess        EQ_CDECL(void, 0x10005EA0, (int))                                  // CountListAndSetIndex_guess: closes the list
#define eqmain_fn_100067e0_guess        EQ_CDECL(void, 0x100067E0, (int))                                  // RefreshSlider_guess: refreshes the scrollbar

// eqmain.dll's globals and functions used by DisconnectAndClearConnection_guess (this image's VAs).
#define g_eqmain_connection_guess       EQ_GLOBAL(int, 0x10125848)                                         // g_eqmain_connection_guess: the connection dword (sent on, then closed and cleared)
#define g_eqmain_flag_a_guess        EQ_GLOBAL(unsigned char, 0x1012584C)                               // g_eqmain_flag_a_guess: byte flag; nonzero sends the dword 5 on the connection first
#define eqmain_connection_send_guess    EQ_CDECL(void, 0x1002C010, (int, void *, unsigned int, unsigned int, unsigned int))  // connection_send_guess (tx.c): (connection, buffer, length, 1, 1)
#define NetworkConnection_disconnect_guess EQ_CDECL(int, 0x1002DB50, (int, int, unsigned int *, unsigned int *))  // NetworkConnection_disconnect_guess (connection.c): here (connection, 10000, NULL, NULL)

// FUNCTION: EQMAIN 0x100162A0
// Displays front-end data records as a parent row with two child rows.
// __cdecl, one stack argument (recordData, the read base; undefined4 in Ghidra); its RET pops 0 bytes; returns nothing.
// Name note: the body does not copy a first word; it loads a list of records. Sets the read base, then reads three
// dwords: g_eqmain_hdr_a_guess, g_eqmain_d238_guess and the record count g_eqmain_record_count_guess. For each record it reads four strings (NextString_guess
// gives the pointer, DecodeToBuffer_guess the text), strcpy's them (NUL included, no bounds check) into 512-byte locals in the
// order 400, 200, 800, 600, then adds a node for the 400 text (0xaa), two nodes under it for the 800 and 600 texts (0x118)
// and a closing row for g_chat_suffix_guess (0x96). Then CountListAndSetIndex_guess closes the list, the scrollbar g_eqmain_cf28_guess gets +0x64 =
// (list +0x24 - list +0x30) + 1 and +0x68 = list +0x2c, and RefreshSlider_guess refreshes it.
void __cdecl LoadRecordsIntoList_guess(int recordData)
{
    unsigned int recordIndex;
    char firstChildText[512];
    char secondChildText[512];
    char parentText[512];
    char recordFieldText[512];
    void *parentEntry;

    eqmain_fn_100250a0_guess(recordData);
    g_eqmain_hdr_a_guess = eqmain_fn_10025120_guess();
    g_eqmain_d238_guess = eqmain_fn_10025120_guess();
    g_eqmain_record_count_guess = eqmain_fn_10025120_guess();
    for (recordIndex = 0; recordIndex < g_eqmain_record_count_guess; recordIndex++)
    {
        strcpy(parentText, (char *)eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()));
        strcpy(recordFieldText, (char *)eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()));
        strcpy(firstChildText, (char *)eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()));
        strcpy(secondChildText, (char *)eqmain_fn_10021110_guess(eqmain_fn_10025150_guess()));
        parentEntry = eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0xaa, parentText, 0, 0x22d69);
        eqmain_fn_10005d20_guess(g_eqmain_list_guess, (int)parentEntry, 0x118, firstChildText, 0x22d69);
        eqmain_fn_10005d20_guess(g_eqmain_list_guess, (int)parentEntry, 0x118, secondChildText, 0x22d69);
        eqmain_fn_10005cc0_guess(g_eqmain_list_guess, 0x96, g_chat_suffix_guess, 0, 0x21a3c);
    }
    eqmain_fn_10005ea0_guess(g_eqmain_list_guess);
    *(int *)(g_eqmain_scrollbar_guess + 0x64) = (*(int *)(g_eqmain_list_guess + 0x24) - *(int *)(g_eqmain_list_guess + 0x30)) + 1;
    *(int *)(g_eqmain_scrollbar_guess + 0x68) = *(int *)(g_eqmain_list_guess + 0x2C);
    eqmain_fn_100067e0_guess(g_eqmain_scrollbar_guess);
    return;
}

// FUNCTION: EQMAIN 0x10021440
// Ends the front-end's active network session, optionally sending a message first.
// __fastcall, one register argument (value in ECX; undefined4 in Ghidra) that is never read: the prologue's PUSH ECX only
// reserves the 4-byte slot sendValue, which is set to 5 before it is read (Ghidra shows sendValue = value). RET pops 0 bytes;
// returns nothing.
// Name note: the body does not log. If g_eqmain_flag_a_guess is nonzero it sends the dword 5 (sendValue) on the connection g_eqmain_connection_guess
// (connection_send_guess, flags 1 and 1). Then, if the connection is set, NetworkConnection_disconnect_guess is called with 10000 and NULL, NULL,
// and the connection is cleared; last, g_eqmain_flag_a_guess is cleared.
void __fastcall DisconnectAndClearConnection_guess(int value)
{
    int sendValue;

    if (g_eqmain_flag_a_guess != 0)
    {
        sendValue = 5;
        eqmain_connection_send_guess(g_eqmain_connection_guess, &sendValue, 2, 1, 1);
    }
    if (g_eqmain_connection_guess != 0)
    {
        NetworkConnection_disconnect_guess(g_eqmain_connection_guess, 10000, 0, 0);
        g_eqmain_connection_guess = 0;
    }
    g_eqmain_flag_a_guess = 0;
    return;
}
