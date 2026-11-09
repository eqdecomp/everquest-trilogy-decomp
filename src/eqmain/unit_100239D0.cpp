// unit_100239D0.cpp - eqmain.dll, the address run 0x100239D0..0x10027950 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "trilogy/eqaddr.h"

// An array cursor (its other fields are unknown): 0x0C items, 0x10 the current index.
struct ArrayCursor_guess {
    void* unknown_00[3];        // 0x00
    void** items;               // 0x0C
    int index;                  // 0x10

    void* Next_guess();
};

// A 13-byte string (12 characters and the NUL), copied as one unit (the original copies it in dword pieces).
struct Str13_guess {
    char text[13];
};

// eqmain.dll .data strings, unnamed (this image's own globals, not in include/): 0x100486FC is "GenuineIntel",
// 0x1004870C is twelve dashes.
#define g_str13_100486fc_guess  EQ_GLOBAL(Str13_guess, 0x100486FC)
#define g_str13_1004870c_guess  EQ_GLOBAL(Str13_guess, 0x1004870C)

// eqmain.dll .data dword at 0x100F4B4C, unnamed (this image's own global, not in include/).
#define g_dword_100f4b4c_guess  EQ_GLOBAL(DWORD, 0x100F4B4C)

// eqmain.dll .data dword at 0x101258F0, unnamed (this image's own global, not in include/). Read as a signed int by
// CpuSpeedClass_guess (its compares are signed, JGE).
#define g_int_101258f0_guess  EQ_GLOBAL(int, 0x101258F0)

// eqmain.dll .data dword at 0x101258F4, unnamed (this image's own global, not in include/). Set to 1 by
// CpuidFeatureFlags_guess when a byte of the CPU's vendor string is not "GenuineIntel".
#define g_dword_101258f4_guess  EQ_GLOBAL(DWORD, 0x101258F4)

// eqmain.dll .data dword at 0x101258FC, unnamed (this image's own global, not in include/). Zeroed by
// EnumerateDirectDraw_guess before the first enumeration.
#define g_dword_101258fc_guess  EQ_GLOBAL(DWORD, 0x101258FC)

// eqmain.dll .data dword at 0x101258F8, unnamed (this image's own global, not in include/). DirectDrawProbe_guess
// stores DirectDrawCreate's result there on success (which is 0).
#define g_dword_101258f8_guess  EQ_GLOBAL(DWORD, 0x101258F8)

// eqmain.dll .data byte at 0x100F4B4C, the low byte of g_dword_100f4b4c_guess (AppendByte_guess is given it).
#define g_byte_100f4b4c_guess  EQ_GLOBAL(char, 0x100F4B4C)

// eqmain.dll .data strings, unnamed (this image's own globals, not in include/): their addresses are passed to
// CopyStringToBuffer_guess (a lookup by string), so they are arrays here, not dwords.
#define g_sel_name_guess  EQ_ARRAY(char, 0x100554B0)
#define g_eqmain_copy_cec_guess  EQ_ARRAY(char, 0x10104CEC)

// eqmain.dll .data buffer at 0x10104C2C, unnamed (size unknown; Ghidra also labels 0x10104C30 and 0x10104C31).
// The strcpy target of SendStringPairPacket_guess.
#define g_buf_10104c2c_guess  EQ_ARRAY(char, 0x10104C2C)

// eqmain.dll .data dword at 0x10125848, unnamed (this image's own global, not in include/): the connection
// that connection_send_guess sends on.
#define g_dword_10125848_guess  EQ_GLOBAL(DWORD, 0x10125848)

// eqmain.dll .data packet buffer at 0x10125914, unnamed (size unknown): SetStateAndWord_guess, AppendString_guess and
// AppendByte_guess write into it.
#define g_sendbuf_10125914_guess  EQ_ARRAY(char, 0x10125914)

// eqmain.dll .data buffer at 0x100F4C10, unnamed (size unknown): the file-tail buffer of SendFileTail_guess.
#define g_eqmain_copybuf_guess  EQ_ARRAY(char, 0x100F4C10)

// eqmain.dll .data string at 0x1004CE40, unnamed: copied into g_buf_100f4c10_guess by SendFileTail_guess.
#define g_eqmain_drive_base_guess  EQ_ARRAY(char, 0x1004CE40)

// eqmain.dll .data pointer at 0x10055780, unnamed (this image's own global, not in include/): the sprintf format
// that SendFileTail_guess uses when the file can't be opened.
#define g_pstr_10055780_guess  EQ_GLOBAL(char *, 0x10055780)

// eqmain.dll .data dword at 0x1012B918, unnamed (this image's own global, not in include/): the length of the
// packet buffer (AppendByte_guess appends a byte and bumps it).
#define g_dword_1012b918_guess  EQ_GLOBAL(DWORD, 0x1012B918)

// eqmain.dll import slot at 0x10040244 (.rdata, not writable), filled by the loader with WINMM.DLL timeGetTime.
// The original calls through it: the pointer is loaded into a register once, then `call reg` for each read.
typedef DWORD (__stdcall *TimeGetTime_guess_t)(void);
#define g_pfn_timegettime_10040244_guess  EQ_GLOBAL(TimeGetTime_guess_t, 0x10040244)

// LPDDENUMCALLBACKA (ddraw.h is not included here): the per-driver callback DirectDrawEnumerateA calls.
typedef BOOL (__stdcall *DDEnumCallback_guess)(void *guid, char *description, char *name, void *context);

// __declspec(naked) as a macro: the original has no frame, so the rewrite must not get the compiler's prologue.
#define EQ_NAKED  __declspec(naked)

// eqmain.dll .data string at 0x10047E60, unnamed (this image's own global, not in include/): the mode string
// FUN_1003407f opens the file with.
#define g_str_10047e60_guess  EQ_ARRAY(char, 0x10047E60)

// eqmain.dll .data buffers at 0x100F4918, 0x101257C4 and 0x10115574, unnamed (sizes unknown): the three strcpy
// targets of RunTracert_guess. The second is also the %s argument of its tracert command line.
#define g_buf_100f4918_guess  EQ_ARRAY(char, 0x100F4918)
#define g_buf_101257c4_guess  EQ_ARRAY(char, 0x101257C4)
#define g_buf_10115574_guess  EQ_ARRAY(char, 0x10115574)

// eqmain.dll .data dword at 0x100559D8, unnamed (this image's own global, not in include/): the first argument of
// Identity_guess, which returns the tracert format string.
#define g_dword_100559d8_guess  EQ_GLOBAL(DWORD, 0x100559D8)

// eqmain.dll .data pointers at 0x101258D0 and 0x101258CC, unnamed (this image's own globals, not in include/): the
// STARTUPINFOA (operator_new(0x44)) and the PROCESS_INFORMATION (operator_new(0x10)) that RunTracert_guess passes
// to CreateProcessA.
#define g_pstartupinfo_101258d0_guess  EQ_GLOBAL(STARTUPINFOA *, 0x101258D0)
#define g_pprocinfo_101258cc_guess  EQ_GLOBAL(PROCESS_INFORMATION *, 0x101258CC)

// eqmain.dll .data dword at 0x101258D4, unnamed (this image's own global, not in include/): the process handle that
// RunTracert_guess keeps after a successful CreateProcessA.
#define g_handle_101258d4_guess  EQ_GLOBAL(HANDLE, 0x101258D4)

// eqmain.dll import slots (.rdata, not writable), filled by the loader with KERNEL32 CreateFileA, GetLastError and
// CreateProcessA. The original calls through them (`call dword ptr [slot]`).
typedef HANDLE (__stdcall *CreateFileA_guess_t)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE);
typedef DWORD (__stdcall *GetLastError_guess_t)(void);
typedef BOOL (__stdcall *CreateProcessA_guess_t)(LPCSTR, LPSTR, LPSECURITY_ATTRIBUTES, LPSECURITY_ATTRIBUTES, BOOL,
                                                 DWORD, LPVOID, LPCSTR, STARTUPINFOA *, LPPROCESS_INFORMATION);
#define g_pfn_createfilea_100400cc_guess  EQ_GLOBAL(CreateFileA_guess_t, 0x100400CC)
#define g_pfn_getlasterror_10040094_guess  EQ_GLOBAL(GetLastError_guess_t, 0x10040094)
#define g_pfn_createprocessa_100400c8_guess  EQ_GLOBAL(CreateProcessA_guess_t, 0x100400C8)

// eqmain.dll .rdata strings, unnamed (this image's own globals, not in include/): the file name "...",
// the two failure formats and the success format of RunTracert_guess.
#define g_str_100486d4_guess  EQ_ARRAY(char, 0x100486D4)
#define g_str_100486b0_guess  EQ_ARRAY(char, 0x100486B0)
#define g_str_10048690_guess  EQ_ARRAY(char, 0x10048690)
#define g_str_10048654_guess  EQ_ARRAY(char, 0x10048654)

// FUNCTION: EQMAIN 0x100239D0
// Loads and decodes text from a file; the file's game purpose is unclear.
// __cdecl. Reads the file filePath into a malloc'd buffer of ftell + 0x10 bytes, NUL-terminates it at the count fread
// returned, closes the file and returns strdup(DecodeToBuffer_guess(buffer)); the buffer is freed. Returns 0 when the file is
// empty (ftell <= 0) or the buffer can't be allocated; both close the file first. When the file can't be opened the
// buffer is 0 and the tail still runs, so DecodeToBuffer_guess(0) is called. The name is a guess from what the calls do.
char * __cdecl LoadFileString_guess(char *filePath)
{
    FILE *_File;
    size_t fileSize;
    char *duplicatedText;
    unsigned char *_DstBuf;

    _DstBuf = (unsigned char *)0x0;
    _File = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(filePath, g_str_10047e60_guess);
    if (_File != (FILE *)0x0)
    {
        EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(_File, 0, 2);
        fileSize = EQ_CDECL(long, 0x10033E8F, (FILE *))(_File);
        if ((int)fileSize <= 0)
        {
            EQ_CDECL(int, 0x100343B9, (FILE *))(_File);
            return (char *)0x0;
        }
        EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(_File, 0, 0);
        _DstBuf = (unsigned char *)EQ_CDECL(void *, 0x1003342E, (size_t))(fileSize + 0x10);
        if (_DstBuf == (unsigned char *)0x0)
        {
            EQ_CDECL(int, 0x100343B9, (FILE *))(_File);
            return (char *)0x0;
        }
        fileSize = EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, FILE *))(_DstBuf, 1, fileSize, _File);
        _DstBuf[fileSize] = 0;
        EQ_CDECL(int, 0x100343B9, (FILE *))(_File);
    }
    duplicatedText = (char *)EQ_CDECL(DWORD *, 0x10021110, (unsigned char *))(_DstBuf);
    duplicatedText = EQ_CDECL(char *, 0x1003396B, (char *))(duplicatedText);
    EQ_CDECL(void, 0x10033568, (void *))(_DstBuf);
    return duplicatedText;
}

// FUNCTION: EQMAIN 0x10023A80
// __cdecl, no return value. strcpy(0x10104C2C, firstText) (the VC6 intrinsic: the copy includes the NUL), then the
// packet: SetStateAndWord_guess(0x0C); AppendString_guess(CopyStringToBuffer_guess(firstText)); AppendString_guess(CopyStringToBuffer_guess(secondText));
// AppendByte_guess(low byte of g_dword_100f4b4c_guess); AppendByte_guess(0); connection_send_guess(conn, buffer, length, 1, 1).
// The name is a guess from what the calls do.
void __cdecl SendStringPairPacket_guess(char *firstText, char *secondText)
{
    char *lookup;

    strcpy(g_buf_10104c2c_guess, firstText);
    EQ_CDECL(void, 0x10024F90, (unsigned short))(0xc);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(firstText);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(secondText);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    EQ_CDECL(void, 0x10024FB0, (char))(g_byte_100f4b4c_guess);
    EQ_CDECL(void, 0x10024FB0, (char))(0);
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, unsigned int, unsigned int))(
        g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    return;
}

// FUNCTION: EQMAIN 0x10023B10
// __cdecl, no return value. SetStateAndWord_guess(0x45); AppendString_guess(CopyStringToBuffer_guess(firstText)); AppendString_guess(CopyStringToBuffer_guess(secondText));
// connection_send_guess(g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1).
// The same packet as SendStringPairPacket_guess with opcode 0x45, and without the strcpy and the two flag bytes.
// The name is a guess from what the calls do.
void __cdecl SendStringPair45Packet_guess(char *firstText, char *secondText)
{
    char *lookup;

    EQ_CDECL(void, 0x10024F90, (unsigned short))(0x45);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(firstText);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(secondText);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, unsigned int, unsigned int))(
        g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    return;
}

// FUNCTION: EQMAIN 0x10023B60
// __stdcall, no arguments, no return value. SetStateAndWord_guess(0x47); AppendString_guess(CopyStringToBuffer_guess("0x100554B0")); AppendString_guess(CopyStringToBuffer_guess("0x10104CEC")); connection_send_guess(conn, buffer, length, 1, 1).
// The same packet as SendStringPairPacket_guess with a fixed opcode and two fixed strings. The name is a guess from what the calls do.
void __stdcall SendFixedStringPairPacket_guess(void)
{
    char *lookup;

    EQ_CDECL(void, 0x10024F90, (unsigned short))(0x47);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(g_sel_name_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(g_eqmain_copy_cec_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, unsigned int, unsigned int))(
        g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    return;
}

// FUNCTION: EQMAIN 0x10023BB0
// __stdcall, no arguments, no return value. SetStateAndWord_guess(0x48); AppendString_guess(CopyStringToBuffer_guess("0x100554B0")); AppendString_guess(CopyStringToBuffer_guess("0x10104CEC")); connection_send_guess(conn, buffer, length, 1, 1).
// SendFixedStringPairPacket_guess with opcode 0x48 instead of 0x47. The name is a guess from what the calls do.
void __stdcall SendFixedStringPairPacket48_guess(void)
{
    char *lookup;

    EQ_CDECL(void, 0x10024F90, (unsigned short))(0x48);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(g_sel_name_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    lookup = (char *)EQ_CDECL(DWORD *, 0x100211B0, (char *))(g_eqmain_copy_cec_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(lookup);
    EQ_CDECL(void, 0x1002C010, (int, void *, DWORD, unsigned int, unsigned int))(
        g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    return;
}

// FUNCTION: EQMAIN 0x10023C00
// __stdcall, no arguments, no return value. Stores 0 to the global dword at 0x100F4B4C.
void __stdcall Clear_100f4b4c_guess(void)
{
    g_dword_100f4b4c_guess = 0;
}

// FUNCTION: EQMAIN 0x10023C10
// Treats empty or space-only text as blank.
// __cdecl. 1 when the string is empty or all spaces (its first byte is set to 0, so it becomes ""); 0 at the first other character, string unchanged.
bool __cdecl String_clearIfAllSpaces_guess(char *text)
{
    char ch;
    char *cursor;

    cursor = text;
    ch = *text;
    while (ch != '\0') {
        if (ch != ' ')
            return false;
        cursor = cursor + 1;
        ch = *cursor;
    }
    *text = '\0';
    return true;
}

// FUNCTION: EQMAIN 0x10023C40
// Sends a file tail and three text fields in opcode 0x4D; its game meaning is unclear.
// __cdecl, one argument: the stream base SetStateAndDword_guess keeps. Reads three strings (firstString, filePath, thirdString)
// and a byte count from the stream, copies g_str_1004ce40_guess into g_buf_100f4c10_guess, then opens the file named
// by the second string. When that fails it formats g_pstr_10055780_guess (with that name) into the buffer; otherwise
// it reads the file's last `count` bytes into it, with no NUL added (bugs included). Then it sends packet 0x4D: the
// three strings and the buffer, each through CopyStringToBuffer_guess and AppendString_guess, on connection g_dword_10125848_guess.
void __cdecl SendFileTail_guess(DWORD messageData)
{
    char firstString[20];
    char filePath[128];
    char thirdString[128];
    BYTE *encodedText;
    char *text;
    size_t _Count;
    FILE *_File;
    long fileSize;

    EQ_CDECL(void, 0x100250A0, (DWORD))(messageData);
    encodedText = EQ_STDCALL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(firstString, text);
    encodedText = EQ_STDCALL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(filePath, text);
    _Count = EQ_STDCALL(size_t, 0x10025120, (void))();
    encodedText = EQ_STDCALL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(thirdString, text);
    strcpy(g_eqmain_copybuf_guess, g_eqmain_drive_base_guess);
    _File = EQ_CDECL(FILE *, 0x1003407F, (char *, char *))(filePath, g_str_10047e60_guess);
    if (_File == NULL || _File == (FILE *)-1) {
        EQ_CDECL(int, 0x10034117, (char *, char *, ...))(g_eqmain_copybuf_guess, g_pstr_10055780_guess, filePath);
    } else {
        EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(_File, 0, 2);
        fileSize = EQ_CDECL(long, 0x10033E8F, (FILE *))(_File);
        EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(_File, 0, 0);
        if (fileSize > (int)_Count) {
            EQ_CDECL(int, 0x100345ED, (FILE *, long, int))(_File, fileSize - _Count, 0);
        }
        EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, FILE *))(g_eqmain_copybuf_guess, 1, _Count, _File);
        EQ_CDECL(int, 0x100343B9, (FILE *))(_File);
    }
    EQ_CDECL(void, 0x10024F90, (WORD))(0x4D);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(firstString);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(filePath);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(thirdString);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_copybuf_guess);
    EQ_CDECL(void, 0x10024FF0, (char *))(text);
    EQ_CDECL(void, 0x1002C010, (int, char *, DWORD, unsigned int, unsigned int))(
        g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
}

// FUNCTION: EQMAIN 0x10023E00
// Launches an external command and reports its result; its exact game purpose is unclear.
// __cdecl, one DWORD argument (SetStateAndDword_guess keeps it), no return value. Clears commandLine (100 bytes) and buffer
// (1 KB), copies three strings from the NextString_guess / DecodeToBuffer_guess reader into g_buf_100f4918_guess,
// g_buf_101257c4_guess and g_buf_10115574_guess, and formats the tracert command line into commandLine from the format
// Identity_guess returns (its argument is g_buf_101257c4_guess). It creates "..." as the stdout of a
// STARTUPINFOA and starts the tracert with CreateProcessA. A failure formats its GetLastError message into
// g_buf_100f4c10_guess, success formats the "executing" message; either way the four strings are added to the packet
// (CopyStringToBuffer_guess, AppendString_guess) and sent with connection_send_guess. Bugs kept: cb is set to 4, not to the structure's
// size, and after a CreateFileA failure the STARTUPINFOA is freed and still passed to CreateProcessA.
void __cdecl RunTracert_guess(DWORD messageData)
{
    SECURITY_ATTRIBUTES securityAttributes;
    char commandLine[100];
    DWORD buffer[256];
    HANDLE outputFileHandle;
    DWORD DVar5;
    BOOL BVar6;
    BYTE *encodedText;
    char *text;

    memset(commandLine, 0, sizeof(commandLine));
    memset(buffer, 0, sizeof(buffer));
    EQ_CDECL(void, 0x100250A0, (DWORD))(messageData);
    encodedText = EQ_CDECL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(g_buf_100f4918_guess, text);
    encodedText = EQ_CDECL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(g_buf_101257c4_guess, text);
    encodedText = EQ_CDECL(BYTE *, 0x10025150, (void))();
    text = EQ_CDECL(char *, 0x10021110, (BYTE *))(encodedText);
    strcpy(g_buf_10115574_guess, text);
    EQ_CDECL(int, 0x10034117, (char *, char *, ...))(commandLine,
        EQ_CDECL(char *, 0x100210F0, (DWORD, char *, DWORD))(g_dword_100559d8_guess, (char *)buffer, 0x400),
        g_buf_101257c4_guess);
    g_pstartupinfo_101258d0_guess = EQ_CDECL(STARTUPINFOA *, 0x100338E5, (unsigned int))(0x44);
    securityAttributes.nLength = 0xc;
    securityAttributes.bInheritHandle = 1;
    securityAttributes.lpSecurityDescriptor = (LPVOID)0x0;
    g_pstartupinfo_101258d0_guess->lpTitle = (LPSTR)0x0;
    g_pstartupinfo_101258d0_guess->cbReserved2 = 0;
    g_pstartupinfo_101258d0_guess->lpReserved2 = (LPBYTE)0x0;
    g_pstartupinfo_101258d0_guess->lpReserved = (LPSTR)0x0;
    g_pstartupinfo_101258d0_guess->dwFlags = 0x100;
    g_pstartupinfo_101258d0_guess->lpDesktop = (LPSTR)0x0;
    g_pstartupinfo_101258d0_guess->dwX = 0;
    g_pstartupinfo_101258d0_guess->dwY = 0;
    g_pstartupinfo_101258d0_guess->dwXSize = 0;
    g_pstartupinfo_101258d0_guess->dwYSize = 0;
    g_pstartupinfo_101258d0_guess->dwXCountChars = 0;
    g_pstartupinfo_101258d0_guess->dwYCountChars = 0;
    g_pstartupinfo_101258d0_guess->dwFillAttribute = 0;
    g_pstartupinfo_101258d0_guess->wShowWindow = 0;
    g_pstartupinfo_101258d0_guess->hStdInput = (HANDLE)0x0;
    g_pstartupinfo_101258d0_guess->hStdError = (HANDLE)0x0;
    outputFileHandle = g_pfn_createfilea_100400cc_guess(g_str_100486d4_guess, 0xc0000000, 0, &securityAttributes, 2, 0x80,
        (HANDLE)0x0);
    g_pstartupinfo_101258d0_guess->hStdOutput = outputFileHandle;
    g_pstartupinfo_101258d0_guess->cb = 4;
    if (g_pstartupinfo_101258d0_guess->hStdOutput == (HANDLE)0xffffffff) {
        DVar5 = g_pfn_getlasterror_10040094_guess();
        EQ_CDECL(int, 0x10034117, (char *, char *, ...))(g_eqmain_copybuf_guess, g_str_100486b0_guess, DVar5);
        EQ_CDECL(void, 0x10024F90, (WORD))(0x61);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_100f4918_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_101257c4_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10115574_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_copybuf_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        EQ_CDECL(void, 0x1002C010, (int, char *, DWORD, unsigned int, unsigned int))(
            g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
        EQ_CDECL(void, 0x10033800, (void *))(g_pstartupinfo_101258d0_guess);
    }
    g_pprocinfo_101258cc_guess = EQ_CDECL(PROCESS_INFORMATION *, 0x100338E5, (unsigned int))(0x10);
    BVar6 = g_pfn_createprocessa_100400c8_guess((LPCSTR)0x0, commandLine, (LPSECURITY_ATTRIBUTES)0x0,
        (LPSECURITY_ATTRIBUTES)0x0, 1, 0x8000000, (LPVOID)0x0, (LPCSTR)0x0, g_pstartupinfo_101258d0_guess,
        g_pprocinfo_101258cc_guess);
    if (BVar6 == 0) {
        DVar5 = g_pfn_getlasterror_10040094_guess();
        EQ_CDECL(int, 0x10034117, (char *, char *, ...))(g_eqmain_copybuf_guess, g_str_10048690_guess, DVar5);
        EQ_CDECL(void, 0x10024F90, (WORD))(0x61);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_100f4918_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_101257c4_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10115574_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_copybuf_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        EQ_CDECL(void, 0x1002C010, (int, char *, DWORD, unsigned int, unsigned int))(
            g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
    } else {
        EQ_CDECL(int, 0x10034117, (char *, char *, ...))(g_eqmain_copybuf_guess, g_str_10048654_guess,
            g_buf_101257c4_guess);
        EQ_CDECL(void, 0x10024F90, (WORD))(0x61);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_100f4918_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_101257c4_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_buf_10115574_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_copybuf_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        EQ_CDECL(void, 0x1002C010, (int, char *, DWORD, unsigned int, unsigned int))(
            g_dword_10125848_guess, g_sendbuf_10125914_guess, g_dword_1012b918_guess, 1, 1);
        g_handle_101258d4_guess = g_pprocinfo_101258cc_guess->hProcess;
    }
    EQ_CDECL(void, 0x10033800, (void *))(g_pprocinfo_101258cc_guess);
    return;
}

// FUNCTION: EQMAIN 0x100241B0
// Probes whether the CPU ID flag changes for CPUID detection.
// __stdcall, no arguments, naked: the asm is the whole function and ends with its own RET, as the original
// does. Saves the flags in EBX, toggles the ID flag (EFLAGS bit 21) in a copy, reads the flags back, restores
// the saved flags and returns (flags read back) XOR (saved flags): normally 0x200000 when the CPU lets ID
// change (CPUID present), 0 when it doesn't. The value is stored in DAT_101258ec and read back from it.
EQ_NAKED DWORD __stdcall IdFlagToggleDiff_guess(void)
{
    __asm {
        push ebx
        pushfd
        pop eax
        mov ebx, eax
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        push ebx
        popfd
        xor eax, ebx
        // mov [0x101258EC], eax and mov eax, [0x101258EC] as the original's moffs forms (A3 / A1);
        // VC6 rejects the bare absolute operand (C2415); the ds: form compiles but is not byte-identical
        _emit 0xA3
        _emit 0xEC
        _emit 0x58
        _emit 0x12
        _emit 0x10
        _emit 0xA1
        _emit 0xEC
        _emit 0x58
        _emit 0x12
        _emit 0x10
        pop ebx
        ret
    }
}

// FUNCTION: EQMAIN 0x100241D0
// Identifies the processor vendor as Intel, AMD, or unknown.
DWORD __stdcall CpuVendorKind_guess(void)
{
    DWORD vendorKind = 0;

    __asm {
        xor eax, eax
        xor ebx, ebx
        xor ecx, ecx
        xor edx, edx
        cpuid
        cmp ebx, 0x756e6547
        jne checkAmd
        cmp edx, 0x49656e69
        jne checkAmd
        cmp ecx, 0x6c65746e
        jne checkAmd
        mov vendorKind, 1
        jmp done
    checkAmd:
        cmp ebx, 0x68747541
        jne unknownVendor
        cmp edx, 0x69746e65
        jne unknownVendor
        cmp ecx, 0x444d4163
        jne unknownVendor
        mov vendorKind, 2
        jmp done
    unknownVendor:
        mov vendorKind, 0
    done:
    }
    return vendorKind;
}

// eqmain.dll feature flag set by CPUID leaf 1, EDX bit 15 (this image's own global).
#define g_dword_101258e4_guess  EQ_GLOBAL(DWORD, 0x101258E4)
#define g_dword_101258dc_guess  EQ_GLOBAL(DWORD, 0x101258DC)

// FUNCTION: EQMAIN 0x10024240
// __stdcall, no arguments. Sets the corresponding CPU feature flags when CPUID leaf 1 reports them.
void __stdcall CpuidFeatureBits_guess(void)
{
    int featureBits;

    featureBits = 0;
    __asm {
        mov eax, 1
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov featureBits, edx
    }
    if ((featureBits & 0x8000) != 0) {
        g_dword_101258e4_guess = 1;
    }
    if ((featureBits & 0x800000) != 0) {
        g_dword_101258dc_guess = 1;
    }
    return;
}

// eqmain.dll feature flags set by CPUID leaf 0x80000001 (this image's own globals).
#define g_dword_101258e8_guess  EQ_GLOBAL(DWORD, 0x101258E8)

// FUNCTION: EQMAIN 0x100242a0
// Sets flags from CPUID 0x80000001 EDX: bit 23 (MMX) -> 0x101258DC, bit 31 (3DNow!) -> 0x101258E8.
// __stdcall, no arguments. Sets each extended CPU feature flag to 1 when its CPUID bit is present;
// the globals are left unchanged when the corresponding bit is clear.
void __stdcall CpuidExtFeatureFlags_guess(void)
{
    int featureBits;

    featureBits = 0;
    __asm {
        mov eax, 0x80000001
        mov ebx, 0
        mov ecx, 0
        mov edx, 0
        cpuid
        mov featureBits, edx
        mov eax, featureBits
        test eax, 0x800000
        jz skip_3dnow
        mov ebx, 1
        // VC6 rejects the absolute memory operand; emit the original MOV [101258DC], EBX bytes.
        _emit 0x89
        _emit 0x1D
        _emit 0xDC
        _emit 0x58
        _emit 0x12
        _emit 0x10
    skip_3dnow:
        mov eax, featureBits
        test eax, 0x80000000
        jz skip_3dnow_ext
        mov ebx, 1
        // VC6 rejects the absolute memory operand; emit the original MOV [101258E8], EBX bytes.
        _emit 0x89
        _emit 0x1D
        _emit 0xE8
        _emit 0x58
        _emit 0x12
        _emit 0x10
    skip_3dnow_ext:
    }
}

// FUNCTION: EQMAIN 0x10024300
// __stdcall, no arguments. Returns the low 32 bits of the time-stamp counter (RDTSC); EDX is dropped.
DWORD __stdcall ReadCycleCounter_guess(void)
{
    DWORD cycleCount;

    cycleCount = 0;
    __asm {
        rdtsc
        mov cycleCount, eax
    }
    return cycleCount;
}

// FUNCTION: EQMAIN 0x10024320
// Estimates the processor's clock speed.
// __stdcall, no arguments. Estimates the CPU clock in MHz: waits until timeGetTime has moved 2 ms past its
// first reading, reads the cycle counter (0x10024300), waits 1003 ms more by timeGetTime, reads it again and
// returns the 32-bit cycle difference divided by 1000000 (about MHz for a 1 s wait).
DWORD __stdcall MeasureCpuMhz_guess(void)
{
    TimeGetTime_guess_t pfnTimeGetTime;
    DWORD tick_a;
    DWORD tick_b;
    int elapsed;
    DWORD cycles_a;
    DWORD cycles_b;

    pfnTimeGetTime = g_pfn_timegettime_10040244_guess;
    tick_a = pfnTimeGetTime();
    tick_b = pfnTimeGetTime();
    elapsed = tick_b - tick_a;
    while (elapsed <= 1) {
        tick_b = pfnTimeGetTime();
        elapsed = tick_b - tick_a;
    }
    cycles_a = EQ_STDCALL(DWORD, 0x10024300, (void))();
    tick_a = pfnTimeGetTime();
    elapsed = tick_a - tick_b;
    while (elapsed <= 0x3ea) {
        tick_a = pfnTimeGetTime();
        elapsed = tick_a - tick_b;
    }
    cycles_b = EQ_STDCALL(DWORD, 0x10024300, (void))();
    return (cycles_b - cycles_a) / 1000000;
}

// eqmain.dll .data dword at 0x101258D8, unnamed (this image's own global, not in include/): the value CpuVendorKind_guess
// returns (its body calls CPUID leaf 0); DetectCpuFeatures_guess switches on it.
#define g_dword_101258d8_guess  EQ_GLOBAL(DWORD, 0x101258D8)

// eqmain.dll .data dword at 0x101258E0, unnamed (this image's own global, not in include/). Set to 1 on the value-1
// path of DetectCpuFeatures_guess, zeroed on the others.
#define g_dword_101258e0_guess  EQ_GLOBAL(DWORD, 0x101258E0)

// eqmain.dll .data char pointers at 0x100486E4 and 0x100486E8, unnamed (this image's own globals, not in include/).
// The value-2 path of DetectCpuFeatures_guess strcpy's the string at 0x100486E8 into the buffer at 0x100486E4.
#define g_pstr_100486e4_guess  EQ_GLOBAL(char *, 0x100486E4)
#define g_pstr_100486e8_guess  EQ_GLOBAL(char *, 0x100486E8)

// FUNCTION: EQMAIN 0x10024380
// Detects CPU vendor and features, and measures processor clock speed.
// __stdcall, no arguments, no return value. Does nothing unless 0x100241B0 returns nonzero. Stores the value
// 0x100241D0 returns in g_dword_101258d8 and switches on it: 1 sets g_dword_101258e0 to 1 and runs 0x10024240;
// 2 zeroes g_dword_101258e0, runs 0x100242A0 and strcpy's the string at 0x100486E8 into 0x100486E4; any other
// value zeroes g_dword_101258e0. Every path zeroes g_dword_101258e8, then stores the MeasureCpuMhz_guess result
// (0x10024320) in g_int_101258f0 after the switch.
void __stdcall DetectCpuFeatures_guess(void)
{
    if (EQ_STDCALL(DWORD, 0x100241B0, (void))() != 0) {
        g_dword_101258d8_guess = EQ_STDCALL(DWORD, 0x100241D0, (void))();
        switch (g_dword_101258d8_guess) {
        case 1:
            g_dword_101258e0_guess = 1;
            g_dword_101258e8_guess = 0;
            EQ_STDCALL(void, 0x10024240, (void))();
            break;
        case 2:
            g_dword_101258e0_guess = 0;
            g_dword_101258e8_guess = 0;
            EQ_STDCALL(void, 0x100242A0, (void))();
            strcpy(g_pstr_100486e4_guess, g_pstr_100486e8_guess);
            break;
        default:
            g_dword_101258e0_guess = 0;
            g_dword_101258e8_guess = 0;
            break;
        }
        g_int_101258f0_guess = EQ_STDCALL(DWORD, 0x10024320, (void))();
    }
    return;
}

// FUNCTION: EQMAIN 0x10024420
// Maps processor clock speed into a nine-level speed tier.
// __stdcall, no arguments. Returns a speed class 0..8 for the signed dword at 0x101258F0: 0 below 100; 1 for
// 100..149; 2 for 150..189; 3 for 190..219; 4 for 220..249; 5 for 250..289; 6 for 290..319; 7 for 320..389;
// 8 from 390 up. Every return writes all of EAX (Ghidra shows char), so the return type is int.
int __stdcall CpuSpeedClass_guess(void)
{
    if (g_int_101258f0_guess < 100) {
        return 0;
    }
    if (g_int_101258f0_guess < 0x96) {
        return 1;
    }
    if (g_int_101258f0_guess < 0xbe) {
        return 2;
    }
    if (g_int_101258f0_guess < 0xdc) {
        return 3;
    }
    if (g_int_101258f0_guess < 0xfa) {
        return 4;
    }
    if (g_int_101258f0_guess < 0x122) {
        return 5;
    }
    if (g_int_101258f0_guess < 0x140) {
        return 6;
    }
    return (g_int_101258f0_guess >= 0x186) + 7;
}

// FUNCTION: EQMAIN 0x10024490
// Checks whether the CPU supports CPUID by testing whether its ID flag can change.
// __stdcall, no arguments. 1 when the CPU lets the ID flag (EFLAGS bit 21) be toggled, so CPUID is
// present; 0 when the flag won't change. Callers read AX only.
short __stdcall HasCpuid_guess(void)
{
    int cpuidSupported;

    cpuidSupported = 1;
    __asm {
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 0x200000
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        jnz short skip
        mov cpuidSupported, 0
    skip:
    }
    return (short)cpuidSupported;
}

// FUNCTION: EQMAIN 0x100244C0
// Classifies the processor from CPUID data or legacy flag checks.
// __stdcall, no arguments. With CPUID (0x10024490 nonzero) the result is 0x10024700's. Otherwise
// DAT_101258F4 becomes the low 16 bits of 0x100245D0, and EAX is the last result of 0x10024610,
// 0x10024660 or 0x100246B0 (each tested as AX; 4 when the chain runs out). Bit 15 is set when
// DAT_101258F4 is nonzero, on either path.
DWORD __stdcall DetectCpuClass_guess(void)
{
    DWORD result;

    if (EQ_STDCALL(short, 0x10024490, (void))() != 0)
    {
        result = EQ_STDCALL(DWORD, 0x10024700, (void))();
    }
    else
    {
        g_dword_101258f4_guess = EQ_STDCALL(DWORD, 0x100245D0, (void))() & 0xffff;
        result = EQ_STDCALL(DWORD, 0x10024610, (void))();
        if ((short)result != 0)
        {
            result = EQ_STDCALL(DWORD, 0x10024660, (void))();
            if ((short)result != 2)
            {
                result = EQ_STDCALL(DWORD, 0x100246B0, (void))();
                if ((short)result != 3)
                {
                    result = 4;
                }
            }
        }
    }
    if (g_dword_101258f4_guess != 0)
    {
        result |= 0x8000;
    }
    return result;
}

// FUNCTION: EQMAIN 0x10024520
// Reads processor feature bits and notes a vendor-string mismatch; its game purpose is unclear.
// __stdcall, no arguments. 0 when HasCpuid_guess says the CPU has no CPUID. Otherwise the leaf 0 vendor
// string (EBX, EDX, ECX) is compared with the "GenuineIntel" string at 0x100486FC (a differing byte sets
// DAT_101258F4 to 1), and leaf 1's EDX, the feature flags, is returned.
DWORD __stdcall CpuidFeatureFlags_guess(void)
{
    char expected[13];
    char vendor[13];
    DWORD features;
    int i;

    *(Str13_guess *)expected = g_str13_100486fc_guess;
    *(Str13_guess *)vendor = g_str13_1004870c_guess;
    features = 0;
    if (EQ_STDCALL(short, 0x10024490, (void))() == 0)
        return 0;
    __asm {
        push ebx
        xor eax, eax
        cpuid
        mov dword ptr [vendor], ebx
        mov dword ptr [vendor + 4], edx
        mov dword ptr [vendor + 8], ecx
    }
    for (i = 0; i < 12; i++)
    {
        if (vendor[i] != expected[i])
            g_dword_101258f4_guess = 1;
    }
    if (i > 0)
    {
        __asm {
            xor eax, eax
            inc eax
            cpuid
            mov features, edx
        }
    }
    __asm {
        pop ebx
    }
    return features;
}

// FUNCTION: EQMAIN 0x100245d0
// Probes legacy CPU divide behavior; its compatibility meaning is unclear.
// __stdcall, no arguments. Returns the zero flag that DIV leaves (1 when set, 0 when clear). The
// Intel manual leaves DIV's flags undefined, so the original's instructions are kept as they are:
// JNZ/STC turn ZF into CF, and PUSHF/POP AX read it back.
int __stdcall DivZeroFlag_guess(void)
{
    int local;

    local = 0;
    __asm
    {
        mov ax, 0x5555
        xor dx, dx
        mov cx, 2
        div cx
        clc
        jnz short stc_path
        jmp short pushf_path
stc_path:
        stc
pushf_path:
        pushf
        pop ax
        and al, 1
        xor al, 1
        mov word ptr local, ax
    }
    return local & 1;
}

// FUNCTION: EQMAIN 0x10024610
// __stdcall, no arguments. CPU check: clears the top four FLAGS bits and reads them back. A CPU that
// keeps them clear (a 286) gives 0; a later CPU gives 0xFFFF. The caller's FLAGS are restored from CX.
short __stdcall Check8086_guess(void)
{
    int local;

    local = 0xffff;
    __asm
    {
        pushf
        pop ax
        mov cx, ax
        and ax, 0x0fff
        push ax
        popf
        pushf
        pop ax
        and ax, 0xf000
        cmp ax, 0xf000
        mov word ptr local, 0
        jz short restore_path
        mov word ptr local, 0xffff
restore_path:
        push cx
        popf
        mov ax, word ptr local
        mov ax, word ptr local
    }
}

// FUNCTION: EQMAIN 0x10024660
// __stdcall, no arguments. CPU check for the 286 flag bits: ORs 0xF000 into FLAGS and reads the top four
// bits back. A 286 reads them as 0 (returns 2); a later CPU keeps them set (returns 0xFFFF). The caller's
// FLAGS are restored from BX. The local is written as a dword (0xFFFF) and then as a word (2 or 0xFFFF).
short __stdcall Cpu286FlagCheck_guess(void)
{
    int local;

    local = 0xffff;
    __asm
    {
        pushf
        pop cx
        mov bx, cx
        or cx, 0xf000
        push cx
        popf
        pushf
        pop ax
        and ax, 0xf000
        mov word ptr local, 2
        jz short restore_path
        mov word ptr local, 0xffff
restore_path:
        push bx
        popf
        mov ax, word ptr local
        mov ax, word ptr local
    }
}

// FUNCTION: EQMAIN 0x100246b0
// Tests AC-flag support to distinguish 386-class processors from 486-or-newer ones.
// __stdcall, no arguments. CPU check: toggles EFLAGS.AC (bit 18) and returns 3 when the bit won't change
// (a 386, which has no AC flag), 0xFFFF when it does (a 486 or later). SP is aligned to 4 for the
// PUSHFD/POPFD pairs; BX keeps the low word of the caller's SP and ECX the original flags.
short __stdcall AcFlagCheck_guess(void)
{
    int local;

    local = 0xffff;
    __asm
    {
        mov bx, sp
        and sp, 0xfffc
        pushfd
        pop eax
        mov ecx, eax
        xor eax, 0x40000
        push eax
        popfd
        pushfd
        pop eax
        xor eax, ecx
        mov word ptr local, 3
        jz short keep_path
        mov word ptr local, 0xffff
keep_path:
        push ecx
        popfd
        mov sp, bx
        mov ax, word ptr local
        and eax, 0xffff
        mov ax, word ptr local
    }
}

// FUNCTION: EQMAIN 0x10024700
// Identifies the processor family from CPUID for CPU compatibility setup.
short __stdcall CpuFamily_guess(void)
{
    char expectedVendor[16];
    char cpuVendor[12];
    DWORD featureBits;
    DWORD value;
    DWORD family;
    unsigned int index;

    *(DWORD *)&expectedVendor[0] = *(DWORD *)&g_str13_100486fc_guess.text[0];
    family = 0xffff;
    *(DWORD *)&expectedVendor[4] = *(DWORD *)&g_str13_100486fc_guess.text[4];
    *(DWORD *)&expectedVendor[8] = *(DWORD *)&g_str13_100486fc_guess.text[8];
    expectedVendor[12] = g_str13_100486fc_guess.text[12];

    __asm
    {
        xor eax, eax
        cpuid
        mov dword ptr [cpuVendor], ebx
        mov dword ptr [cpuVendor+4], edx
        mov dword ptr [cpuVendor+8], ecx
    }

    index = 0;
    do
    {
        if (cpuVendor[index] != expectedVendor[index])
            g_dword_101258f4_guess = 1;
        index++;
    } while ((int)index < 0xc);

    if (0 < (int)index)
    {
        __asm
        {
            mov eax, 1
            cpuid
            mov featureBits, ecx
            mov family, eax
        }
        family = (family >> 8) & 0xf;
    }
    return (short)family;
}

typedef DWORD (__stdcall *ComReleaseFn_guess)(void *);   // IUnknown::Release, vtable slot 2 (0x08)

// FUNCTION: EQMAIN 0x100247E0
// Checks DirectDraw availability while recording the current display entry's two strings.
// __stdcall, four dword arguments (RET 0x10; Ghidra shows three, and the first and fourth are never read). Passes
// firstText and secondText to AppendString_guess (__cdecl, one string each), then calls DirectDrawCreate(NULL, &dd, NULL). When
// that returns 0 it stores the 0 in g_dword_101258f8_guess and passes it to AppendDword_guess. Then, when DirectDrawCreate
// left dd non-null, dd is released (stdcall, this as the one argument). Returns 1 whatever DirectDrawCreate returned.
int __stdcall DirectDrawProbe_guess(DWORD temp, char *firstText, char *secondText, DWORD result)
{
    int *dd;
    int hr;

    dd = 0;
    EQ_CDECL(void, 0x10024FF0, (char *))(firstText);
    EQ_CDECL(void, 0x10024FF0, (char *))(secondText);
    hr = EQ_STDCALL(int, 0x10032314, (void *, int **, void *))(0, &dd, 0);
    if (hr == 0)
    {
        g_dword_101258f8_guess = hr;
        EQ_CDECL(void, 0x10024FD0, (DWORD))(hr);
    }
    if (dd != 0)
    {
        ((ComReleaseFn_guess)(*(void ***)dd)[2])(dd);
    }
    return 1;
}

// FUNCTION: EQMAIN 0x10024840
// Appends DirectDraw driver info to the send buffer: a dword from a first pass, then each driver's strings.
// __stdcall, no arguments. Zeroes g_dword_101258fc_guess, enumerates the DirectDraw drivers with the callback at
// 0x100247c0, passes the global to AppendDword_guess (__cdecl, one dword), then enumerates again with the callback at
// 0x100247e0. Returns nothing: EAX is left as the second DirectDrawEnumerateA's HRESULT, which the caller ignores.
void __stdcall EnumerateDirectDraw_guess(void)
{
    g_dword_101258fc_guess = 0;
    EQ_STDCALL(HRESULT, 0x1003231A, (DDEnumCallback_guess, void *))((DDEnumCallback_guess)0x100247c0, 0);
    EQ_CDECL(void, 0x10024FD0, (DWORD))(g_dword_101258fc_guess);
    EQ_STDCALL(HRESULT, 0x1003231A, (DDEnumCallback_guess, void *))((DDEnumCallback_guess)0x100247e0, 0);
}

// FUNCTION: EQMAIN 0x10024880
// The machine's physical memory in bytes. As original: MEMORYSTATUS.dwLength is never set before the call.
SIZE_T __cdecl GetTotalPhysicalMemory_guess()
{
    MEMORYSTATUS status;

    GlobalMemoryStatus(&status);
    return status.dwTotalPhys;
}

// FUNCTION: EQMAIN 0x10025960
// __thiscall. Advances the cursor and returns the item it now points at (no bounds check).
void* ArrayCursor_guess::Next_guess()
{
    return items[++index];
}
