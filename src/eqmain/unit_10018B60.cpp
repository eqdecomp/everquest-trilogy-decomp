// unit_10018B60.cpp - eqmain.dll, the address run 0x10018B60..0x1001CF80 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"
#include <stdio.h>
#include <string.h>

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
#define g_eqmain_text_59a4_guess    EQ_GLOBAL(char *, 0x100559A4)   // g_eqmain_text_59a4_guess: the text EQMain_DrawTextRun_guess draws first
#define g_eqmain_src_5990_guess     EQ_GLOBAL(char *, 0x10055990)   // g_eqmain_src_5990_guess: copied to g_eqmain_ptr_5024_guess at the start
#define g_eqmain_ptr_5024_guess     EQ_GLOBAL(char *, 0x10055024)   // g_eqmain_ptr_5024_guess: a char *, the seventh argument of CreateRolloutFromTokens_guess
#define g_eqmain_text_59ac_guess    EQ_GLOBAL(char *, 0x100559AC)   // g_eqmain_text_59ac_guess: the text EQMain_InsertAtCursor_guess takes in mode 0
#define g_eqmain_style_guess        EQ_GLOBAL(int, 0x10052DB0)      // g_eqmain_dword_b_guess: 2 selects style 6 for CreateRolloutFromTokens_guess, anything else 5
// EQMain_DrawTextRun_guess: same address as eqmain_fn_100082c0_guess (defined further down, after this file's first use of it)
#define eqmain_fn_100082c0_early_guess  EQ_CDECL(int, 0x100082C0, (int, int, int, unsigned int, char *, int, unsigned int))   // cdecl, seven pushed args
#define eqmain_fn_10008800_guess    EQ_CDECL(void, 0x10008800, (int, int, int))   // EQMain_DrawTextFrameRow_guess: cdecl, three ints
#define eqmain_fn_10008460_guess    EQ_CDECL(void *, 0x10008460, (int, int, int, int, int, unsigned int, int, int, unsigned int, unsigned int))   // EQMain_CreateSurfaceRecord_guess: cdecl, ten args, returns the new object
#define eqmain_fn_10008970_guess    EQ_CDECL(void, 0x10008970, (void *, char *))   // EQMain_InsertAtCursor_guess: cdecl, object and text
#define eqmain_fn_10008660_guess    EQ_CDECL(void, 0x10008660, (int, int, int, int))   // EQMain_DrawTextFrame_guess: cdecl, four ints
#define eqmain_fn_10005ec0_guess    EQ_CDECL(void *, 0x10005EC0, (int, int, int, unsigned char, char *, unsigned int, char *, unsigned char))   // CreateRolloutFromTokens_guess: cdecl, eight args, returns the new object
#define eqmain_fn_10006250_guess    EQ_CDECL(void, 0x10006250, (void *, short))   // storeWord28_guess: cdecl, object and a 16-bit value
#define eqmain_fn_10035089_guess    EQ_CDECL(char *, 0x10035089, (int, char *, int))   // __itoa: CRT, __cdecl, returns the buffer
#define g_eqmain_ceb8_guess    EQ_GLOBAL(int, 0x1004CEB8)   // g_eqmain_ceb8_guess: the destination surface wrapper
#define g_eqmain_blit_src_guess    EQ_GLOBAL(int, 0x10055448)   // g_eqmain_blit_src_guess: the source surface wrapper
#define eqmain_fn_10006c20_guess   EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int))
#define g_eqmain_com_a_guess       EQ_GLOBAL(void *, 0x100D5B28)   // g_eqmain_com_a_guess: a COM object, Release()d first
#define g_eqmain_com_b_guess       EQ_GLOBAL(void *, 0x100D5B24)   // g_eqmain_com_b_guess: a COM object, slot 0x50 then Release()d
#define g_eqmain_hwnd_guess        EQ_GLOBAL(int, 0x100D5B2C)      // g_eqmain_hwnd_guess: the window handle (HWND)
#define g_eqmain_destroywin_guess  EQ_GLOBAL(EqMainDestroyWindowFn_guess, 0x100401F0)   // PTR_DestroyWindow: USER32 IAT slot
#define eqmain_fn_100280a0_guess(object) EQ_METHOD(0, void, 0x100280A0, (int), (object))   // ReleaseHeldAndGlobal_guess: fastcall, this = 0x100555E0
#define eqmain_fn_10026080_guess(object) EQ_METHOD(0, int *, 0x10026080, (int *), (object))   // LoadEqTextRecordTable_guess: fastcall, this = 0x100556E8, int * in EAX
#define eqmain_fn_10035f9a_guess   EQ_CDECL(int, 0x10035F9A, (struct _iobuf *))   // _fgetc: CRT, __cdecl, next char or -1

// Stand-ins for the calls the original makes through a vtable slot or the IAT: all __stdcall (the callee pops).
typedef unsigned long (__stdcall *EqMainComReleaseFn_guess)(void *);   // slot 2 (Release, IUnknown order)
typedef long (__stdcall *EqMainComSetFn_guess)(void *, int, int);     // slot 0x50: (this, HWND, 8)
typedef int (__stdcall *EqMainDestroyWindowFn_guess)(int);            // USER32 DestroyWindow
typedef unsigned long (__stdcall *EqMainGetDriveStringsFn_guess)(unsigned long, char *);   // GetLogicalDriveStringsA (IAT)
typedef unsigned int (__stdcall *EqMainGetDriveTypeFn_guess)(char *);                     // GetDriveTypeA (IAT)

// EQMain_ShutdownArtPaks_guess: its IAT slots, globals, strings and callees that the lines above don't have,
// and the 8-byte by-value argument of PackFS::removeFileSystem (0x1000ECF0): the layout of PackFsStringReference_guess in
// src/eqmain/ffcreate2/ff/packfs.cpp (shared pointer at 0x0, value at 0x4). No destructor here.
typedef long (__stdcall *EqMainSetWindowLongFn_guess)(int, int, int);                       // SetWindowLongA (IAT)
typedef int (__stdcall *EqMainMessageBeepFn_guess)(unsigned int);                           // MessageBeep (IAT)
typedef int (__stdcall *EqMainMessageBoxFn_guess)(int, char *, char *, unsigned int);       // MessageBoxA (IAT)
struct EqMainPakRef_guess
{
    void *shared;   // 0x0
    int value;      // 0x4
};
struct EqMainControl_guess
{
    int unknown[8]; // 0x0
    char *text;     // 0x20: the entry text (EQMain_CreateSurfaceRecord_guess returns these)
};
#define g_eqmain_defwndproc_guess     EQ_GLOBAL(int, 0x100401D8)                                // PTR_DefWindowProcA: the DefWindowProcA address the IAT holds
#define g_eqmain_setwindowlong_guess  EQ_GLOBAL(EqMainSetWindowLongFn_guess, 0x100401FC)        // PTR_SetWindowLongA: USER32 IAT slot
#define g_eqmain_messagebeep_guess    EQ_GLOBAL(EqMainMessageBeepFn_guess, 0x100401EC)          // PTR_MessageBeep: USER32 IAT slot
#define g_eqmain_messagebox_guess     EQ_GLOBAL(EqMainMessageBoxFn_guess, 0x100401F4)           // PTR_MessageBoxA: USER32 IAT slot
#define g_eqmain_pak1_guess           EQ_GLOBAL(void *, 0x100D5B38)   // g_eqmain_pak1_guess: the PackFS of eqfeart1.pak (0 when not loaded)
#define g_eqmain_pak2_guess           EQ_GLOBAL(void *, 0x100D5B3C)   // g_eqmain_pak2_guess: the PackFS of eqfeart2.pak
#define g_eqmain_pak3_guess           EQ_GLOBAL(void *, 0x100D5B40)   // g_eqmain_pak3_guess: the PackFS of eqfeart3.pak
#define g_eqmain_pak4_guess           EQ_GLOBAL(void *, 0x100D5B44)   // g_eqmain_pak4_guess: the PackFS of eqfeart4.pak
#define g_eqmain_pak5_guess           EQ_GLOBAL(void *, 0x100D5B48)   // g_eqmain_pak5_guess: the PackFS of eqfeart5.pak
#define g_eqmain_pak6_guess           EQ_GLOBAL(void *, 0x100D5B4C)   // g_eqmain_pak6_guess: the PackFS of eqfeart6.pak
#define g_eqmain_errtext_guess        EQ_ARRAY(char, 0x100D5B54)      // g_eqmain_errtext_guess: an error text; when not empty: beep, message box, _exit(0)
#define g_fatal_text_prefix_guess        EQ_GLOBAL(char *, 0x10055A14)   // g_fatal_text_prefix_guess: the caption of that message box
#define g_eqmain_s_eqfeart1_guess     EQ_ARRAY(char, 0x100465B8)      // s_eqfeart1.pak_100465b8: "eqfeart1.pak"
#define g_eqmain_s_eqfeart2_guess     EQ_ARRAY(char, 0x100465A8)      // s_eqfeart2.pak_100465a8: "eqfeart2.pak"
#define g_eqmain_s_eqfeart3_guess     EQ_ARRAY(char, 0x10046598)      // s_eqfeart3.pak_10046598: "eqfeart3.pak"
#define g_eqmain_s_eqfeart4_guess     EQ_ARRAY(char, 0x10046588)      // s_eqfeart4.pak_10046588: "eqfeart4.pak"
#define g_eqmain_s_eqfeart5_guess     EQ_ARRAY(char, 0x10046578)      // s_eqfeart5.pak_10046578: "eqfeart5.pak"
#define g_eqmain_s_eqfeart6_guess     EQ_ARRAY(char, 0x10046568)      // s_eqfeart6.pak_10046568: "eqfeart6.pak"
#define eqmain_fn_100216f0_guess(object) EQ_METHOD(0, void, 0x100216F0, (int), (object))                       // CleanupSetup_guess: __fastcall, one register argument it ignores
#define eqmain_fn_10003930_guess      EQ_STDCALL(void, 0x10003930, (void))                       // DestroyUiContext_guess: __stdcall, no arguments
#define eqmain_fn_10027b20_guess      EQ_STDCALL(void, 0x10027B20, (void))                       // DestroyGlobalFontManager_guess: __stdcall, no arguments
#define eqmain_fn_10020ef0_guess      EQ_CDECL(void, 0x10020EF0, (char))                         // EqfeCfgSetActive_guess: cdecl, one char
#define eqmain_fn_1001b220_guess      EQ_CDECL(void, 0x1001B220, (char))                         // EQMain_ReleaseDisplay_guess: cdecl, one char
#define eqmain_fn_10035e98_guess      EQ_STDCALL(void, 0x10035E98, (int))                          // _exit: CRT, never returns; called stdcall-shaped because the original pushes its argument and never pops it (no ADD ESP)
#define eqmain_fn_10033800_guess      EQ_CDECL(void, 0x10033800, (void *))                       // FUN_10033800: cdecl, one pointer (operator delete)
#define eqmain_fn_10007540_guess(object) EQ_METHOD(1, void *, 0x10007540, (void *, char *), (object))     // PackFsStringReference_guess::initializeStringReferenceWithString_guess: __thiscall, one C string, returns this
#define eqmain_fn_10007650_guess(object) EQ_METHOD(1, void *, 0x10007650, (void *, char *), (object))     // initializeStringReferenceWithNormalizedString_guess: __thiscall, one C string, returns this
#define eqmain_fn_10007870_guess(object) EQ_METHOD(1, void *, 0x10007870, (void *, void *), (object))     // initializeStringReferenceWithNormalizedPath_guess: __thiscall, one reference in, returns this
#define eqmain_fn_10007620_guess(object) EQ_METHOD(1, void *, 0x10007620, (void *, void *), (object))     // PackFsStringReference_guess::copyStringReference_guess: __thiscall, one reference in, returns this
#define eqmain_fn_10007520_guess(object) EQ_METHOD(0, void, 0x10007520, (void *), (object))                    // decrementReferenceCount_guess: __fastcall, the object in ECX, released
#define eqmain_fn_10010b50_guess(object) EQ_METHOD(0, void, 0x10010B50, (void *), (object))                    // decrementSharedReferenceCount_guess: __fastcall, the reference in ECX, destructor
#define eqmain_fn_1000ecf0_guess(object) EQ_METHOD(1, void, 0x1000ECF0, (void *, EqMainPakRef_guess), (object))   // PackFS::removeFileSystem: __thiscall, 8-byte by-value argument, RET 8

#define g_eqmain_slots_guess      EQ_ARRAY(void *, 0x10055018)   // g_eqmain_slots_guess: three dword slots (the loop stops at 0x10055024)
#define eqmain_fn_10008a00_guess  EQ_CDECL(void, 0x10008A00, (void *))   // EQMain_FreeEntry_guess: cdecl, one pointer argument
#define g_eqmain_ptr_ca0_guess    EQ_GLOBAL(void *, 0x10051CA0)   // g_eqmain_ptr_ca0_guess: a pointer, freed after the slots
#define eqmain_fn_10006130_guess  EQ_CDECL(void, 0x10006130, (void *))   // UnlinkAndFreeRecord_guess: cdecl, one pointer argument
#define g_eqmain_drive_base_guess       EQ_ARRAY(char, 0x1004CE40)   // g_eqmain_drive_base_guess: the base string, copied to g_eqmain_intro_path_guess first
#define g_eqmain_intro_path_guess       EQ_ARRAY(char, 0x100D5D58)   // g_eqmain_intro_path_guess: the intro movie path the two copies write
#define g_eqmain_qintro_guess           EQ_ARRAY(char, 0x1004828C)   // g_eqmain_qintro_guess: "qintro.smk", appended to the CD-ROM root
#define g_eqmain_getdrivestrings_guess  EQ_GLOBAL(EqMainGetDriveStringsFn_guess, 0x100400B8)   // PTR_GetLogicalDriveStringsA: IAT slot
#define g_eqmain_getdrivetype_guess     EQ_GLOBAL(EqMainGetDriveTypeFn_guess, 0x100400B4)     // PTR_GetDriveTypeA: IAT slot
#define g_eqmain_dlg_parent_guess       EQ_GLOBAL(int, 0x100EC9C0)   // DAT_100ec9c0: the dialog window GetDlgItem searches
#define g_eqmain_dlg_item_guess         EQ_GLOBAL(int, 0x100EC9C4)   // g_dialog_edit_guess: the last control handle GetDlgItem returned
#define g_eqmain_getdlgitem_guess       EQ_GLOBAL(EqMainGetDlgItemFn_guess, 0x10040214)   // PTR_GetDlgItem: USER32 IAT slot
#define g_eqmain_sendmessage_guess      EQ_GLOBAL(EqMainSendMessageFn_guess, 0x1004020C)  // PTR_SendMessageA: USER32 IAT slot
typedef int (__stdcall *EqMainGetDlgItemFn_guess)(int, int);                             // USER32 GetDlgItem (IAT)
typedef long (__stdcall *EqMainSendMessageFn_guess)(int, unsigned int, unsigned int, long);   // USER32 SendMessageA (IAT)
#define g_eqmain_buf_c60_guess      EQ_ARRAY(char, 0x10051C60)   // g_eqmain_buf_c60_guess: 64-byte copy of the string at slot 0 + 0x20
#define g_eqmain_buf_e10_guess      EQ_ARRAY(char, 0x10051E10)   // g_eqmain_buf_e10_guess: 4000-byte copy of the string at slot 1 + 0x20
#define g_eqmain_buf_ca4_guess      EQ_ARRAY(char, 0x10051CA4)   // g_eqmain_buf_ca4_guess: 300-byte copy of the string at slot 2 + 0x20, or the base string
#define g_eqmain_str_dd0_guess      EQ_ARRAY(char, 0x10051DD0)   // g_eqmain_str_dd0_guess: a string, run through CopyStringToBuffer_guess first
#define g_eqmain_mode_guess         EQ_GLOBAL(int, 0x10052DB4)   // g_eqmain_dword_a_guess: 0 or 1 takes the base-string copy
#define g_eqmain_last_index_guess   EQ_GLOBAL(int, 0x10055034)   // g_eqmain_last_index_guess: (short) at g_eqmain_ptr_ca0_guess + 0x28, minus 1
#define eqmain_fn_10033330_guess    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))   // _strncpy: CRT, __cdecl
#define eqmain_fn_10024f90_guess    EQ_CDECL(void, 0x10024F90, (short))   // SetStateAndWord_guess: cdecl, one 16-bit value
#define eqmain_fn_10024fd0_guess    EQ_CDECL(void, 0x10024FD0, (int))   // AppendDword_guess: cdecl, one 32-bit value
#define eqmain_fn_10024ff0_guess    EQ_CDECL(void, 0x10024FF0, (char *))   // AppendString_guess: cdecl, one string
#define eqmain_fn_100211b0_guess    EQ_CDECL(void *, 0x100211B0, (char *))   // CopyStringToBuffer_guess: cdecl, string in, pointer out
#define eqmain_finish_hardware_report_guess    EQ_STDCALL(void, 0x100213F0, (void))   // SendBuffer_guess: __stdcall, no arguments, returns at once
#define g_eqmain_license_buf_guess    EQ_ARRAY(char, 0x100D5F60)   // g_eqmain_license_buf_guess: the 60000-byte license text buffer, zeroed first
#define g_eqmain_license_name_guess   EQ_ARRAY(char, 0x10048298)   // s_license.txt_10048298: "license.txt"
#define g_eqmain_fallback_key_guess   EQ_GLOBAL(char *, 0x100559DC)   // g_eqmain_fallback_key_guess: the key CopyStringToBuffer_guess looks up when there is no file
#define eqmain_fn_100210f0_guess      EQ_CDECL(char *, 0x100210F0, (char *))   // Identity_guess: cdecl, one string in, the path out
#define eqmain_fn_1001cd50_guess      EQ_CDECL(int, 0x1001CD50, (struct _iobuf *))   // EQMain_ReadEscape_guess: EQMain_ReadEscape_guess, cdecl
#define g_eqmain_getfocus_guess          EQ_GLOBAL(EqMainGetFocusFn_guess, 0x1004021C)   // PTR_GetFocus: USER32 IAT slot
#define g_eqmain_smack_movie_guess       EQ_GLOBAL(int, 0x100D5F58)   // g_eqmain_smack_movie_guess: the Smacker movie (SMACK *), first argument of the Smack calls
#define g_eqmain_smack_buf_guess         EQ_GLOBAL(int, 0x100D5F5C)   // g_eqmain_smack_buf_guess: the SmackBlit buffer, first argument of SmackBlit and SetPalette
#define g_eqmain_surface_guess           (*(int **)(g_eqmain_ceb8_guess + 0xc))   // the IDirectDrawSurface at wrapper +0x0c
#define g_eqmain_smack_setpalette_guess  EQ_GLOBAL(EqMainSmackSetPaletteFn_guess, 0x100402BC)   // PTR__SmackBlitSetPalette@12: IAT slot
#define g_eqmain_smack_doframe_guess     EQ_GLOBAL(EqMainSmackDoFrameFn_guess, 0x100402C0)       // PTR__SmackDoFrame@4: IAT slot
#define g_eqmain_smack_tobuf_guess       EQ_GLOBAL(EqMainSmackToBufferRectFn_guess, 0x100402C4)  // PTR__SmackToBufferRect@8: IAT slot
#define g_eqmain_smack_blit_guess        EQ_GLOBAL(EqMainSmackBlitFn_guess, 0x100402C8)          // PTR__SmackBlit@44: IAT slot
#define g_eqmain_smack_nextframe_guess   EQ_GLOBAL(EqMainSmackNextFrameFn_guess, 0x100402CC)     // PTR__SmackNextFrame@4: IAT slot
typedef int (__stdcall *EqMainGetFocusFn_guess)(void);                                    // USER32 GetFocus (IAT): the focus HWND
typedef void (__stdcall *EqMainSmackSetPaletteFn_guess)(int, int, int);                   // SmackBlitSetPalette (IAT), 3 arguments
typedef void (__stdcall *EqMainSmackDoFrameFn_guess)(int);                                // SmackDoFrame (IAT)
typedef int (__stdcall *EqMainSmackToBufferRectFn_guess)(int, int);                       // SmackToBufferRect (IAT): nonzero while a rect is left
typedef void (__stdcall *EqMainSmackBlitFn_guess)(int, int, int, int, int, int, int, int, int, int, int);   // SmackBlit (IAT), 11 arguments
typedef void (__stdcall *EqMainSmackNextFrameFn_guess)(int);                              // SmackNextFrame (IAT)
typedef void (__stdcall *EqMainSmackUseMMXFn_guess)(int);                                       // SmackUseMMX (IAT): result unused
typedef void (__stdcall *EqMainSmackSoundUseDirectSoundFn_guess)(int);                          // SmackSoundUseDirectSound (IAT)
typedef int (__stdcall *EqMainSmackOpenFn_guess)(char *, unsigned int, unsigned int);          // SmackOpen (IAT): SMACK *, 0 on failure
typedef void (__stdcall *EqMainSmackCloseFn_guess)(int);                                        // SmackClose (IAT)
typedef void (__stdcall *EqMainSmackToBuffer28Fn_guess)(int, int, int, int, int, void *, int); // SmackToBuffer (IAT), 7 arguments
typedef int (__stdcall *EqMainSmackBlitOpenFn_guess)(int);                                      // SmackBlitOpen (IAT): the blit buffer, 0 on failure
typedef int (__stdcall *EqMainSmackWaitFn_guess)(int);                                          // SmackWait (IAT): nonzero while still busy
typedef void (__stdcall *EqMainSmackBlitCloseFn_guess)(int);                                    // SmackBlitClose (IAT)
#define g_eqmain_smack_usemmx_guess       EQ_GLOBAL(EqMainSmackUseMMXFn_guess, 0x100402B8)                  // PTR__SmackUseMMX@4: IAT slot
#define g_eqmain_smack_sounddirect_guess  EQ_GLOBAL(EqMainSmackSoundUseDirectSoundFn_guess, 0x100402D4)    // PTR__SmackSoundUseDirectSound@4: IAT slot
#define g_eqmain_smack_open_guess         EQ_GLOBAL(EqMainSmackOpenFn_guess, 0x100402D8)                    // PTR__SmackOpen@12: IAT slot
#define g_eqmain_smack_close_guess        EQ_GLOBAL(EqMainSmackCloseFn_guess, 0x100402DC)                   // PTR__SmackClose@4: IAT slot
#define g_eqmain_smack_tobuffer28_guess   EQ_GLOBAL(EqMainSmackToBuffer28Fn_guess, 0x100402E0)              // PTR__SmackToBuffer@28: IAT slot
#define g_eqmain_smack_blitopen_guess     EQ_GLOBAL(EqMainSmackBlitOpenFn_guess, 0x100402E4)                // PTR__SmackBlitOpen@4: IAT slot
#define g_eqmain_smack_wait_guess         EQ_GLOBAL(EqMainSmackWaitFn_guess, 0x100402E8)                    // PTR__SmackWait@4: IAT slot
#define g_eqmain_smack_blitclose_guess    EQ_GLOBAL(EqMainSmackBlitCloseFn_guess, 0x100402D0)               // PTR__SmackBlitClose@4: IAT slot
#define g_eqmain_intro_mode_guess         EQ_GLOBAL(int, 0x1004CEA0)                 // g_eqmain_intro_mode_guess: the intro mode, 0 or 1 (any other value returns)
#define g_eqmain_topleft_guess            EQ_GLOBAL(unsigned char, 0x10048288)       // g_eqmain_topleft_guess: nonzero puts the frame at the top-left, and adds 1 to the flags
#define eqmain_fn_1001c8f0_guess          EQ_STDCALL(void, 0x1001C8F0, (void))       // EQMain_FindIntroMovie_guess: __stdcall, no arguments
#define eqmain_fn_1001cbf0_guess          EQ_CDECL(void, 0x1001CBF0, (int, void *))  // EQMain_SmackDrawFrame_guess: __cdecl, (window, buffer)
#define eqmain_fn_10005730_guess          EQ_STDCALL(unsigned int, 0x10005730, (void))   // PumpMessagesAndPollKey_guess: __stdcall, no arguments, undefined4 (the caller tests the low byte)

#define g_eqmain_rowsrc_m1_guess    EQ_GLOBAL(char *, 0x1005597C)   // g_eqmain_rowsrc_m1_guess: the string copied to row -1 (g_eqmain_rows_guess)
#define g_eqmain_rowsrc_0_guess     EQ_GLOBAL(char *, 0x10055980)   // g_eqmain_rowsrc_0_guess: the string copied to row 0 (DAT_10051b20)
#define g_eqmain_rowsrc_1_guess     EQ_GLOBAL(char *, 0x10055984)   // g_eqmain_rowsrc_1_guess: the string copied to row 1 (DAT_10051b60)
#define g_eqmain_rowsrc_2_guess     EQ_GLOBAL(char *, 0x10055988)   // g_eqmain_rowsrc_2_guess: the string copied to row 2 (DAT_10051ba0)
#define g_eqmain_rowsrc_3_guess     EQ_GLOBAL(char *, 0x10055AB8)   // g_eqmain_rowsrc_3_guess: the string copied to row 3 (DAT_10051be0)
#define g_eqmain_rowsrc_4_guess     EQ_GLOBAL(char *, 0x1005598C)   // g_eqmain_rowsrc_4_guess: the string copied to row 4 (DAT_10051c20)
#define g_eqmain_rows_guess         EQ_ARRAY(char, 0x10051AE0)      // g_eqmain_rows_guess: six 64-byte rows, row n (n = -1..4) at 0x40 * (n + 1)
#define g_eqmain_count_guess        EQ_GLOBAL(int, 0x10055038)      // g_eqmain_count_guess: record count, re-read after every record
#define g_eqmain_recs_guess         EQ_ARRAY(int, 0x10052DB8)       // g_eqmain_recs_guess: records of 0x58 bytes, kind at +0, keys at +8 and +0x14
#define eqmain_fn_10021110_guess    EQ_CDECL(char *, 0x10021110, (unsigned char *))   // DecodeToBuffer_guess: cdecl, key in, text pointer out
#define eqmain_fn_10005cc0_guess    EQ_CDECL(int *, 0x10005CC0, (int, int, char *, unsigned int, unsigned int))   // EnqueueNamedRecord_guess: cdecl, returns a node
#define eqmain_fn_10005d20_guess    EQ_CDECL(int *, 0x10005D20, (int, int, int, char *, unsigned int))   // EnqueueOwnedRecord_guess: cdecl, adds a line to a node
#define eqmain_fn_10005a00_guess    EQ_CDECL(void, 0x10005A00, (int))   // DrawNamedRows_guess: cdecl, one argument (the context)
#define eqmain_fn_100067e0_guess    EQ_CDECL(void, 0x100067E0, (int))   // RefreshSlider_guess: cdecl, one argument (the result object)

// FUNCTION: EQMAIN 0x10018B60
// Fills a list, one row per record (decoded key, label+colour by kind, second key), and sets its scroll range
// __cdecl, returns nothing. Copies six strings (g_eqmain_rowsrc_m1_guess, g_eqmain_rowsrc_0_guess, g_eqmain_rowsrc_1_guess, g_eqmain_rowsrc_2_guess, g_eqmain_rowsrc_3_guess,
// g_eqmain_rowsrc_4_guess) into the six 64-byte rows at g_eqmain_rows_guess with strcpy (unbounded). Then, for each of the g_eqmain_count_guess
// records at g_eqmain_recs_guess (0x58 bytes: kind at +0, keys at +8 and +0x14): kind -1 takes row -1 with colour 0x1818f0,
// kinds 0..2 take row kind with 0xc81000, kinds 3..4 take row kind with 0, any other kind takes the string at
// g_eqmain_drive_base_guess with 0xc81000. It adds a node (EnqueueNamedRecord_guess, with the text of the first key) and two lines (EnqueueOwnedRecord_guess
// at 0x55 with the kind's string, then at 0x17c with the text of the second key). Ends with DrawNamedRows_guess(listObject),
// slider+0x64 = listObject[0x24] - listObject[0x30] + 1, slider+0x68 = listObject[0x2c] and RefreshSlider_guess(slider).
// @early-stop: EBP remains based at record +0x14; volatile parameter loads and inner call setup still differ.
/* fake match: volatile keeps listObject in its stack slot during the record loop, leaving EBP for the record cursor. */
void __cdecl EQMain_BuildNameRecords_guess(volatile int listObject, int slider)
{
    int value;
    int *node;
    char *pcVar7;
    int *record;
    unsigned int color;
    int recordIndex;

    strcpy(g_eqmain_rows_guess, g_eqmain_rowsrc_m1_guess);
    strcpy(g_eqmain_rows_guess + 0x40, g_eqmain_rowsrc_0_guess);
    strcpy(g_eqmain_rows_guess + 0x80, g_eqmain_rowsrc_1_guess);
    strcpy(g_eqmain_rows_guess + 0xC0, g_eqmain_rowsrc_2_guess);
    strcpy(g_eqmain_rows_guess + 0x100, g_eqmain_rowsrc_3_guess);
    strcpy(g_eqmain_rows_guess + 0x140, g_eqmain_rowsrc_4_guess);
    recordIndex = 0;
    if (0 < g_eqmain_count_guess)
    {
        record = g_eqmain_recs_guess;
        do
        {
            value = *record;
            switch (value)
            {
            case -1:
                pcVar7 = (char *)(EQ_ARRAY(int, 0x10051B20) + value * 0x10);
                color = 0x1818F0;
                break;
            case 0:
            case 1:
            case 2:
                pcVar7 = (char *)(EQ_ARRAY(int, 0x10051B20) + value * 0x10);
                color = 0xC81000;
                break;
            case 3:
            case 4:
                pcVar7 = (char *)(EQ_ARRAY(int, 0x10051B20) + value * 0x10);
                color = 0;
                break;
            default:
                pcVar7 = g_eqmain_drive_base_guess;
                color = 0xC81000;
                break;
            }
            node = eqmain_fn_10005cc0_guess(listObject, 0x41,
                                              eqmain_fn_10021110_guess((unsigned char *)(record + 2)), 0, 0);
            eqmain_fn_10005d20_guess(listObject, (int)node, 0x55, pcVar7, color);
            eqmain_fn_10005d20_guess(listObject, (int)node, 0x17C,
                                     eqmain_fn_10021110_guess((unsigned char *)(record + 5)), color);
            recordIndex = recordIndex + 1;
            record = record + 0x16;
        } while (recordIndex < g_eqmain_count_guess);
    }
    value = listObject; /* fake match: snapshot the context after the loop so the tail uses one stable register value. */
    eqmain_fn_10005a00_guess(value);
    *(int *)(slider + 0x64) = (*(int *)(value + 0x24) - *(int *)(value + 0x30)) + 1;
    *(int *)(slider + 0x68) = *(int *)(value + 0x2c);
    eqmain_fn_100067e0_guess(slider);
}

// eqmain.dll's own globals and functions used by EQMain_NewsScreen_guess (this image's VAs; the ones defined above are reused).
#define g_eqmain_text_5994_guess    EQ_GLOBAL(char *, 0x10055994)   // g_eqmain_text_5994_guess: the text EQMain_DrawTextRun_guess draws at (0x4d, 0x34)
#define g_eqmain_text_5998_guess    EQ_GLOBAL(char *, 0x10055998)   // g_eqmain_text_5998_guess: the text EQMain_DrawTextRun_guess draws at (0x4a, 0x34)
#define g_eqmain_text_599c_guess    EQ_GLOBAL(char *, 0x1005599C)   // g_eqmain_text_599c_guess: the text EQMain_DrawTextRun_guess draws at (0x4a, 0x75)
#define g_eqmain_text_5860_guess    EQ_GLOBAL(char *, 0x10055860)   // g_eqmain_text_5860_guess: the text the second window gets (eqmain_draw_text_guess)
#define g_eqmain_text_59a8_guess    EQ_GLOBAL(char *, 0x100559A8)   // g_eqmain_text_59a8_guess: the text eqmainRunDialog_guess takes for a kind-0 message
#define g_eqmain_rec_key2_guess     EQ_ARRAY(char, 0x10052DCC)      // g_eqmain_rec_key2_guess: the second key of a record (+0x14), records are 0x58 bytes
#define g_news_bmp_guess   EQ_ARRAY(char, 0x10046240)      // s_news.bmp_10046240: "news.bmp", the first bitmap
#define g_eqmain_s_guilds_bmp_guess EQ_ARRAY(char, 0x100480A8)      // s_guilds.bmp_100480a8: "guilds.bmp", the second bitmap
#define g_eqmain_text_5888_guess    EQ_GLOBAL(char *, 0x10055888)   // g_str_888_guess: the LPCSTR eqmainRunDialog_guess takes as its third argument
#define g_eqmain_text_4c10_guess    EQ_ARRAY(char, 0x100F4C10)      // g_eqmain_copybuf_guess: a char buffer, passed by its address
#define g_eqmain_flag_ce78_guess    EQ_GLOBAL(unsigned char, 0x1004CE78)   // g_eqmain_flag_ce78_guess: a flag byte, cleared before ClearBlinkAndPump_guess
#define eqmain_fn_10003a00_guess    EQ_CDECL(void, 0x10003A00, (void))   // TickBlinkAndCursor_guess: no arguments, no pops
#define eqmain_fn_10003bd0_guess    EQ_STDCALL(int, 0x10003BD0, (void))   // PumpMessagesAndEditText_guess: __stdcall, no arguments, a pointer or 0
#define eqmain_fn_10033568_guess    EQ_CDECL(void, 0x10033568, (void *))   // _free: CRT, __cdecl, one pointer argument
#define eqmain_fn_100039e0_arg_guess EQ_CDECL(void, 0x100039E0, (int))   // ClearBlinkAndPump_guess: __cdecl, one int argument (the caller pushes 0)
#define eqmain_fn_10001520_guess    EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))   // eqmain_create_button_guess: cdecl, five pushed args, returns the new handle
#define eqmain_fn_100017b0_guess    EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))   // eqmain_draw_text_guess: cdecl, five pushed args (Ghidra shows three)
#define eqmain_fn_10001ff0_guess    EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, result not used
#define eqmain_fn_100044b0_guess    EQ_CDECL(void, 0x100044B0, (int, int, int, int))   // EnqueueQuad_guess: cdecl, four arguments
#define eqmain_fn_10001700_guess    EQ_CDECL(void, 0x10001700, (void *))   // eqmain_object_destroy_guess: cdecl, one pointer argument
#define eqmain_fn_10007ae0_guess    EQ_CDECL(int *, 0x10007AE0, (char *, int))   // PackFsGetNamedImage_guess: cdecl, a string and a size, returns a surface
#define eqmain_fn_10007a80_guess    EQ_CDECL(void, 0x10007A80, (int *))   // PackFsFreeEntry_guess: cdecl, one pointer argument
#define eqmain_fn_100220f0_guess    EQ_STDCALL(int, 0x100220F0, (void))   // RdpReceiveScreenDispatch_guess: __stdcall, no arguments, the key code (the switch reads all of EAX)
#define eqmain_fn_100056c0_guess    EQ_CDECL(void, 0x100056C0, (int, char *, int, int))   // DrawNamedImageCentered_guess: cdecl, four arguments
#define eqmain_fn_100213d0_guess    EQ_CDECL(void, 0x100213D0, (int))   // SendStackSlot_guess: cdecl, one int the caller pops
#define eqmain_fn_10006950_guess    EQ_CDECL(int *, 0x10006950, (int, int, int, int, int, int))   // CreateSlider_guess: cdecl, six args, returns the new object
#define eqmain_fn_10005840_guess    EQ_CDECL(int *, 0x10005840, (int, int, int, int, int, unsigned int, int))   // CreateNamedNode_guess: cdecl, seven args, returns the new object
#define eqmain_fn_10018b60_guess    EQ_CDECL(void, 0x10018B60, (int, int))   // EQMain_BuildNameRecords_guess: cdecl, two args
#define eqmain_fn_10006ae0_guess    EQ_CDECL(void, 0x10006AE0, (int *))   // ReleaseQueueUnit_guess: cdecl, one object
#define eqmain_fn_10005930_guess    EQ_CDECL(void, 0x10005930, (int *))   // DestroyNamedNode_guess: cdecl, one object

// FUNCTION: EQMAIN 0x10018D60
// Presents the news and guilds list and lets the player select an available record.
// __stdcall, no arguments, returns a 16-bit code in AX. Draws news.bmp (a blit), guilds.bmp and four texts, then loops:
// RdpReceiveScreenDispatch_guess gives a key (0x57 and 0x12 end the loop; 0x55 makes the two windows, the two objects and the records once;
// 2 shows a text), PumpMessagesAndEditText_guess gives a message (kind 0 checks the windows, kind 1 the object, kind 4 with 0x1b ends the
// loop) which is freed. Ends with ClearBlinkAndPump_guess(0): returns 3 after key 0x12, else result (4, or 0x18 after key 0x57).
unsigned short __stdcall EQMain_NewsScreen_guess(void)
{
    int result;
    unsigned char done;
    unsigned char returnThree;
    int pHVar9;
    int *slider;
    int *listObject;
    int *message;
    int selectHandle;
    int recordIndex;

    result = 4;
    done = 0;
    pHVar9 = 0;
    slider = 0;
    listObject = 0;
    returnThree = 0;
    message = eqmain_fn_10007ae0_guess(g_news_bmp_guess, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)message, 0, 0, message[0], message[1], 0);
    eqmain_fn_10007a80_guess(message);
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, eqmain_fn_100210f0_guess(g_eqmain_s_guilds_bmp_guess), 0, 0);
    eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 0x34, 0x4D, 0x21A3C, g_eqmain_text_5994_guess, 0, 0);
    eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 0x34, 0x4A, 0x21A3C, g_eqmain_text_5998_guess, 0, 0);
    eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 0x75, 0x4A, 0x21A3C, g_eqmain_text_599c_guess, 0, 0);
    eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 0xCA, 0x4A, 0x21A3C, g_eqmain_text_59a4_guess, 0, 0);
    eqmain_fn_100213d0_guess(0x55);
    g_eqmain_flag_ce78_guess = 0;
    eqmain_fn_10003a00_guess();
    do
    {
        switch (eqmain_fn_100220f0_guess())
        {
        case 0x57:
            result = 0x18;
            done = 1;
            break;
        case 0x55:
            if (pHVar9 == 0)
            {
                pHVar9 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x18, 2);
                eqmain_fn_100017b0_guess(pHVar9, 0, g_eqmain_text_5888_guess, 1, 0);
                selectHandle = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x14, 2);
                eqmain_fn_100017b0_guess(selectHandle, 0, g_eqmain_text_5860_guess, 1, 0);
                slider = eqmain_fn_10006950_guess(g_eqmain_ceb8_guess, 0x265, 0x3D, 0x10, 0x170, 0);
                listObject = eqmain_fn_10005840_guess(g_eqmain_ceb8_guess, 0x34, 0x5E, 0x212, 0x139, 0, 0x22D69);
                eqmain_fn_10018b60_guess((int)listObject, (int)slider);
                g_eqmain_flag_ce78_guess = 1;
            }
            break;
        case 2:
            g_eqmain_flag_ce78_guess = 1;
            eqmain_fn_10001ff0_guess(g_eqmain_ceb8_guess, g_eqmain_text_4c10_guess, g_eqmain_text_5888_guess, 0);
            break;
        case 0x12:
            returnThree = 1;
            done = 1;
        }
        message = (int *)eqmain_fn_10003bd0_guess();
        if (message != 0)
        {
            recordIndex = *message;
            switch (recordIndex)
            {
            case 0:
                if (message[2] == pHVar9)
                {
                    eqmain_fn_100044b0_guess(4, 0x1B, 0, 0);
                }
                else if (message[2] == selectHandle && (recordIndex = listObject[0xd], recordIndex < g_eqmain_count_guess) && (recordIndex >= 0))
                {
                    if (g_eqmain_recs_guess[recordIndex * 0x16] != 4)
                    {
                        eqmain_fn_10024f90_guess(0x57);
                        eqmain_fn_10024ff0_guess(g_eqmain_rec_key2_guess + recordIndex * 0x58);
                        eqmain_finish_hardware_report_guess();
                        g_eqmain_flag_ce78_guess = 0;
                    }
                    else
                    {
                        eqmain_fn_10001ff0_guess(g_eqmain_ceb8_guess, g_eqmain_text_59a8_guess, g_eqmain_text_5888_guess, 0);
                    }
                }
                break;
            case 1:
                if ((int *)message[2] == slider)
                {
                    listObject[0xb] = slider[0x1A];
                    eqmain_fn_10005a00_guess((int)listObject);
                }
                break;
            case 4:
                if ((char)message[2] == '\x1b')
                {
                    eqmain_fn_10003a00_guess();
                    done = 1;
                }
                break;
            }
            eqmain_fn_10033568_guess(message);
        }
        if (done)
        {
            g_eqmain_flag_ce78_guess = 0;
            if (pHVar9 != 0)
            {
                eqmain_fn_10001700_guess((void *)pHVar9);
                eqmain_fn_10001700_guess((void *)selectHandle);
                eqmain_fn_10005930_guess(listObject);
                eqmain_fn_10006ae0_guess(slider);
            }
            eqmain_fn_100039e0_arg_guess(0);
            return returnThree ? 3 : result;
        }
    } while (1);
}

// FUNCTION: EQMAIN 0x10019120
// Sets up the screen's text fields and mode-specific choices for player input.
// __stdcall with no arguments, returns nothing. Zeroes g_eqmain_slots_guess, DAT_1005501c, DAT_10055020 and g_eqmain_ptr_ca0_guess (g_eqmain_ptr_5024_guess
// = g_eqmain_src_5990_guess first), then EQMain_DrawTextRun_guess draws g_eqmain_text_59a4_guess and EQMain_DrawTextFrameRow_guess runs. g_eqmain_slots_guess is EQMain_CreateSurfaceRecord_guess's result,
// with EQMain_InsertAtCursor_guess on g_eqmain_buf_c60_guess when that is nonzero; DAT_1005501c likewise with g_eqmain_buf_e10_guess. Then by g_eqmain_dword_a_guess:
// 0 makes DAT_10055020 and draws g_eqmain_text_59ac_guess; 1 makes it with g_eqmain_buf_ca4_guess and returns after the text if that is nonzero;
// anything else makes it with g_eqmain_buf_ca4_guess (if nonzero), sets g_eqmain_ptr_ca0_guess = CreateRolloutFromTokens_guess (style 6 if g_eqmain_dword_b_guess is 2,
// else 5), calls storeWord28_guess when g_eqmain_last_index_guess < 4, and draws g_eqmain_dword_a_guess as text with EQMain_DrawTextRun_guess.
// @early-stop: EBX holds zero across calls, changing the prologue, zero arguments and tests; the remaining register and local-buffer layout differs.
void __stdcall EQMain_CreateSlots_guess(void)
{
    unsigned int count;
    char modeText[16];

    g_eqmain_slots_guess[0] = 0;
    g_eqmain_ptr_5024_guess = g_eqmain_src_5990_guess;
    g_eqmain_slots_guess[1] = 0;
    g_eqmain_slots_guess[2] = 0;
    g_eqmain_ptr_ca0_guess = 0;
    eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 0x38, 0x4e, 0x21a3c, g_eqmain_text_59a4_guess, 0, 0);
    eqmain_fn_10008800_guess(0x78, 0x4e, 0x1c0);
    g_eqmain_slots_guess[0] = eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x78, 0x4e, 0x1c0, 0xe, 0x40, 0, 0, 0x22d69, 0);
    if (g_eqmain_buf_c60_guess[0] != 0)
    {
        eqmain_fn_10008970_guess(g_eqmain_slots_guess[0], g_eqmain_buf_c60_guess);
    }
    eqmain_fn_10008660_guess(0x38, 0x66, 0x200, 0xee);
    g_eqmain_slots_guess[1] = eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x3b, 0x66, 0x200, 0xee, 2000, 0, 0, 0x22d69, 4);
    if (g_eqmain_buf_e10_guess[0] != 0)
    {
        eqmain_fn_10008970_guess(g_eqmain_slots_guess[1], g_eqmain_buf_e10_guess);
    }
    switch (g_eqmain_mode_guess)
    {
    case 0:
        g_eqmain_slots_guess[2] = eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x3b, 0x15a, 0x200, 0x54, 500, 0, 0, 0xc81000, 2);
        eqmain_fn_10008970_guess(g_eqmain_slots_guess[2], g_eqmain_text_59ac_guess);
        break;
    case 1:
        g_eqmain_slots_guess[2] = eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x3b, 0x162, 0x200, 0x2a, 300, 0, 0, 0xc81000, 2);
        if (g_eqmain_buf_ca4_guess[0] != 0)
        {
            eqmain_fn_10008970_guess(g_eqmain_slots_guess[2], g_eqmain_buf_ca4_guess);
            return;
        }
        break;
    default:
        eqmain_fn_10008660_guess(0x38, 0x162, 0x200, 0x2a);
        g_eqmain_slots_guess[2] = eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x3b, 0x162, 0x200, 0x2a, 300, 0, 0, 0xc81000, 4);
        if (g_eqmain_buf_ca4_guess[0] != 0)
        {
            eqmain_fn_10008970_guess(g_eqmain_slots_guess[2], g_eqmain_buf_ca4_guess);
        }
        switch (g_eqmain_style_guess)
        {
        case 2:
            count = 6;
            break;
        default:
            count = 5;
            break;
        }
        g_eqmain_ptr_ca0_guess = eqmain_fn_10005ec0_guess(g_eqmain_ceb8_guess, 0x78, 0x198, 0, g_eqmain_drive_base_guess, count, g_eqmain_ptr_5024_guess, 0);
        if (g_eqmain_last_index_guess < 4)
        {
            eqmain_fn_10006250_guess(g_eqmain_ptr_ca0_guess, g_eqmain_last_index_guess + 1);
        }
        eqmain_fn_10035089_guess(g_eqmain_mode_guess, modeText, 10);
        eqmain_fn_100082c0_early_guess(g_eqmain_ceb8_guess, 500, 0x18c, 0x21a3c, modeText, 0, 0);
        return;
    }
    return;
}

// FUNCTION: EQMAIN 0x100193C0
// Submits the screen's entered text fields and selected mode values.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Copies the string at +0x20 of the
// object in g_eqmain_slots_guess to g_eqmain_buf_c60_guess (64 bytes) and that of DAT_1005501c to g_eqmain_buf_e10_guess (4000 bytes). If
// g_eqmain_dword_a_guess is below 0 or above 1, the string at +0x20 of DAT_10055020 goes to g_eqmain_buf_ca4_guess (300 bytes) and
// g_eqmain_last_index_guess = (short) g_eqmain_ptr_ca0_guess[+0x28] - 1; otherwise g_eqmain_drive_base_guess is strcpy'd to g_eqmain_buf_ca4_guess. Then
// writes 0x56 (SetStateAndWord_guess), g_eqmain_dword_a_guess and g_eqmain_last_index_guess (AppendDword_guess) and the strings g_eqmain_str_dd0_guess,
// g_eqmain_buf_c60_guess, g_eqmain_buf_e10_guess, g_eqmain_buf_ca4_guess, each through CopyStringToBuffer_guess then AppendString_guess. Ends with SendBuffer_guess.
void __stdcall EQMain_SendStringFields_guess(void)
{
    eqmain_fn_10033330_guess(g_eqmain_buf_c60_guess, *(char **)((char *)g_eqmain_slots_guess[0] + 0x20), 0x40);
    eqmain_fn_10033330_guess(g_eqmain_buf_e10_guess, *(char **)((char *)g_eqmain_slots_guess[1] + 0x20), 4000);
    if (g_eqmain_mode_guess < 0 || g_eqmain_mode_guess > 1)
    {
        eqmain_fn_10033330_guess(g_eqmain_buf_ca4_guess, *(char **)((char *)g_eqmain_slots_guess[2] + 0x20), 300);
        g_eqmain_last_index_guess = *(short *)((char *)g_eqmain_ptr_ca0_guess + 0x28) - 1;
    }
    else
    {
        strcpy(g_eqmain_buf_ca4_guess, g_eqmain_drive_base_guess);
    }
    eqmain_fn_10024f90_guess(0x56);
    eqmain_fn_10024fd0_guess(g_eqmain_mode_guess);
    eqmain_fn_10024fd0_guess(g_eqmain_last_index_guess);
    eqmain_fn_10024ff0_guess((char *)eqmain_fn_100211b0_guess(g_eqmain_str_dd0_guess));
    eqmain_fn_10024ff0_guess((char *)eqmain_fn_100211b0_guess(g_eqmain_buf_c60_guess));
    eqmain_fn_10024ff0_guess((char *)eqmain_fn_100211b0_guess(g_eqmain_buf_e10_guess));
    eqmain_fn_10024ff0_guess((char *)eqmain_fn_100211b0_guess(g_eqmain_buf_ca4_guess));
    eqmain_finish_hardware_report_guess();
    return;
}

// FUNCTION: EQMAIN 0x100194C0
// Releases the screen's text-entry and choice controls after use.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Walks the three dword slots from
// g_eqmain_slots_guess: each non-null slot is passed to EQMain_FreeEntry_guess (cdecl) and then cleared. Then g_eqmain_ptr_ca0_guess, if
// non-null, is passed to UnlinkAndFreeRecord_guess (cdecl) and cleared.
void __stdcall EQMain_FreeSlots_guess(void)
{
    void **slot;

    slot = g_eqmain_slots_guess;
    do {
        if (*slot != 0)
        {
            eqmain_fn_10008a00_guess(*slot);
            *slot = 0;
        }
        slot = slot + 1;
    } while ((int)slot < 0x10055024);
    if (g_eqmain_ptr_ca0_guess != 0)
    {
        eqmain_fn_10006130_guess(g_eqmain_ptr_ca0_guess);
        g_eqmain_ptr_ca0_guess = 0;
    }
}

#define eqmain_fn_10007bf0_guess   EQ_CDECL(void, 0x10007BF0, (int, char *, int))   // PackFsBlitNamedImageAtOrigin_guess: __cdecl, three arguments
#define eqmain_fn_10003a00_guess   EQ_CDECL(void, 0x10003A00, (void))   // TickBlinkAndCursor_guess: no arguments, no pops (Ghidra: __stdcall)
#define eqmain_fn_10003bd0_guess   EQ_STDCALL(int, 0x10003BD0, (void))   // PumpMessagesAndEditText_guess: __stdcall, no arguments, a pointer or 0
#define eqmain_fn_10033568_guess   EQ_CDECL(void, 0x10033568, (void *))   // _free: CRT, __cdecl, one pointer argument
#define eqmain_fn_10006b90_guess   EQ_CDECL(unsigned int, 0x10006B90, (int, unsigned int))   // BltColorFillRgb_guess: __cdecl, two arguments
#define g_eqmain_flag_ce78_guess   EQ_GLOBAL(unsigned char, 0x1004CE78)   // g_eqmain_flag_ce78_guess: a flag byte, cleared before ClearBlinkAndPump_guess
#define eqmain_fn_100039e0_arg_guess   EQ_CDECL(void, 0x100039E0, (int))   // ClearBlinkAndPump_guess: __cdecl, one int argument (the caller pushes 0)
typedef unsigned long (__stdcall *EqMainTimeGetTimeFn_guess)(void);   // timeGetTime (WINMM), called through its IAT slot
#define g_eqmain_timegettime_guess EQ_GLOBAL(EqMainTimeGetTimeFn_guess, 0x10040244)   // PTR_timeGetTime: WINMM IAT slot

#define eqmain_fn_10007ae0_guess   EQ_CDECL(int *, 0x10007AE0, (char *, int))   // PackFsGetNamedImage_guess: cdecl, a string and a size, returns a surface
#define eqmain_fn_10007a80_guess   EQ_CDECL(void, 0x10007A80, (int *))   // PackFsFreeEntry_guess: cdecl, one pointer argument
#define eqmain_fn_100056c0_guess   EQ_CDECL(void, 0x100056C0, (int, char *, int, unsigned int))   // DrawNamedImageCentered_guess: cdecl, four arguments
#define eqmain_fn_10001520_guess   EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))   // eqmain_create_button_guess: cdecl, five pushed args (the byte is a pushed 0), returns the new handle
#define eqmain_fn_100017b0_guess   EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))   // eqmain_draw_text_guess: cdecl, five pushed args (Ghidra shows three)
#define eqmain_fn_10019120_guess   EQ_STDCALL(void, 0x10019120, (void))   // EQMain_CreateSlots_guess: __stdcall, no arguments
#define eqmain_fn_100220f0_guess   EQ_STDCALL(int, 0x100220F0, (void))   // RdpReceiveScreenDispatch_guess: __stdcall, no arguments, the event code in EAX
#define eqmain_fn_10001ff0_guess   EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, result not used
#define eqmain_fn_10001ff0_ret_guess   EQ_CDECL(char, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, returns a char (nonzero: yes)
#define eqmain_fn_10008910_guess   EQ_CDECL(void, 0x10008910, (void *))   // EQMain_SetCachedEntry_guess: cdecl, one pointer (stored in g_eqmain_flag_cec4_guess)
#define eqmain_fn_100193c0_guess   EQ_STDCALL(void, 0x100193C0, (void))   // EQMain_SendStringFields_guess: __stdcall, no arguments
#define eqmain_fn_100044b0_guess   EQ_CDECL(void, 0x100044B0, (int, int, int, int))   // EnqueueQuad_guess: cdecl, four arguments
#define eqmain_fn_100194c0_guess   EQ_STDCALL(void, 0x100194C0, (void))   // EQMain_FreeSlots_guess: __stdcall, no arguments (the slot clearing above)
#define eqmain_fn_10001700_guess   EQ_CDECL(void, 0x10001700, (void *))   // eqmain_object_destroy_guess: cdecl, one pointer argument
#define g_news_bmp_guess   EQ_ARRAY(char, 0x10046240)   // s_news.bmp_10046240: "news.bmp"
#define g_eqmain_s_guilds_bmp_guess   EQ_ARRAY(char, 0x100480A8)   // s_guilds.bmp_100480a8: "guilds.bmp"
#define g_eqmain_str_5850_guess   EQ_GLOBAL(char *, 0x10055850)   // g_eqmain_str_5850_guess: the text eqmain_draw_text_guess draws with pHVar8
#define g_eqmain_text_5848_guess      EQ_GLOBAL(char *, 0x10055848)   // g_eqmain_text_5848_guess: the text eqmain_draw_text_guess draws with pHVar9
#define g_eqmain_text_5888_guess      EQ_GLOBAL(char *, 0x10055888)   // g_str_888_guess: the LPCSTR eqmainRunDialog_guess takes as its third argument
#define g_eqmain_text_4c10_guess      EQ_ARRAY(char, 0x100F4C10)   // g_eqmain_copybuf_guess: a char buffer, passed by its address
#define g_eqmain_text_59b0_guess      EQ_GLOBAL(char *, 0x100559B0)   // g_eqmain_text_59b0_guess: the second argument of eqmainRunDialog_guess here
#define g_eqmain_text_b_guess      EQ_GLOBAL(char *, 0x1005586C)   // g_eqmain_text_b_guess: the third argument of eqmainRunDialog_guess here
#define g_eqmain_text_a_guess      EQ_GLOBAL(char *, 0x10055868)   // g_eqmain_text_a_guess: the fourth argument of eqmainRunDialog_guess here
#define g_eqmain_flag_cec4_guess    EQ_GLOBAL(int, 0x1004CEC4)      // g_eqmain_flag_cec4_guess: a dword, cleared by the start screen

// FUNCTION: EQMAIN 0x10019510
// Handles the news screen's editable form and submits player-entered text.
// @early-stop: 4 instruction differences remain; first difference: #217: original `mov al, byte ptr [esp + 0x1f]` | rewrite `mov cl, byte ptr [esp + 0x1f]`.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns 3 after the 0x12 event, else 0x17. Makes news.bmp
// (PackFsGetNamedImage_guess, BltFastSrcRect_guess, PackFsFreeEntry_guess) and guilds.bmp (Identity_guess, DrawNamedImageCentered_guess) at g_eqmain_ceb8_guess, creates two
// handles (eqmain_create_button_guess) and labels them (eqmain_draw_text_guess), then loops: RdpReceiveScreenDispatch_guess gives an event (2 calls eqmainRunDialog_guess,
// 0x12 sets both flags, 0x56 sets the loop flag) and PumpMessagesAndEditText_guess gives a message or 0. A message is handled by its first
// dword (0, 4, 6, 7) and freed. The loop ends after the iteration that set the loop flag: g_eqmain_flag_ce78_guess is cleared,
// EQMain_FreeSlots_guess runs, both handles go to eqmain_object_destroy_guess and ClearBlinkAndPump_guess(0) is called.
int __stdcall EQMain_NewsScreenLoop_guess(void)
{
    bool done = false;
    bool returnThree = false;
    int *data;
    char dialogResult;
    void *eventControl;
    int value;
    unsigned int result;
    int *event;

    data = eqmain_fn_10007ae0_guess(g_news_bmp_guess, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)data, 0, 0, *data, data[1], 0);
    eqmain_fn_10007a80_guess(data);
    result = 0;
    value = 0;
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, eqmain_fn_100210f0_guess(g_eqmain_s_guilds_bmp_guess), value, result);
    void *firstButton = (void *)eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x18, 2);
    eqmain_fn_100017b0_guess((int)firstButton, 0, g_eqmain_str_5850_guess, 1, 0);
    void *secondButton = (void *)eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x14, 2);
    eqmain_fn_100017b0_guess((int)secondButton, 0, g_eqmain_text_5848_guess, 1, 0);
    eqmain_fn_10019120_guess();
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_10003a00_guess();
    do {
        value = eqmain_fn_100220f0_guess();
        switch (value) {
        case 2:
            eqmain_fn_10001ff0_guess(g_eqmain_ceb8_guess, g_eqmain_text_4c10_guess, g_eqmain_text_5888_guess, 0);
            break;
        case 0x12:
            returnThree = true;
            done = true;
            break;
        case 0x56:
            done = true;
            break;
        }
        event = (int *)eqmain_fn_10003bd0_guess();
        if (event != 0) {
            // The original's case bodies follow this order in the code (6, 7, 0, 4); the jump table itself is by value.
            switch (*event) {
            case 6:
                if ((int)event[2] == g_eqmain_flag_cec4_guess) {
                    value = 0;
                    data = (int *)g_eqmain_slots_guess;
                    do {
                        if (g_eqmain_flag_cec4_guess != *data) {
                            value += 1;
                            data = data + 1;
                        }
                        else
                            break;
                    } while ((int)data < 0x10055024);
                    value--;
                    if (value >= 0) {
                        eqmain_fn_10008910_guess(g_eqmain_slots_guess[value]);
                    }
                }
                break;
            case 7:
                if ((int)event[2] == g_eqmain_flag_cec4_guess) {
                    value = 0;
                    data = (int *)g_eqmain_slots_guess;
                    do {
                        if (g_eqmain_flag_cec4_guess != *data) {
                            data = data + 1;
                            value = value + 1;
                        }
                        else
                            break;
                    } while ((int)data < 0x10055024);
                    value += 1;
                    if (value < 3) {
                        eqmain_fn_10008910_guess(g_eqmain_slots_guess[value]);
                    }
                    else {
                        eqmain_fn_10008910_guess(g_eqmain_slots_guess[0]);
                    }
                }
                break;
            case 0:
                eventControl = (void *)event[2];
                if (eventControl == firstButton) {
                    if (((g_eqmain_ptr_ca0_guess == 0) || (*(short *)((char *)g_eqmain_ptr_ca0_guess + 0x28) + -1 < 4)) ||
                        (dialogResult = eqmain_fn_10001ff0_ret_guess(g_eqmain_ceb8_guess, g_eqmain_text_59b0_guess, g_eqmain_text_b_guess, g_eqmain_text_a_guess), dialogResult != 0)) {
                        eqmain_fn_100193c0_guess();
                    }
                }
                else if (eventControl == secondButton) {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
                else if (eventControl == (void *)g_eqmain_flag_cec4_guess) {
                    value = 0;
                    data = (int *)g_eqmain_slots_guess;
                    do {
                        if (g_eqmain_flag_cec4_guess != *data) {
                            data = data + 1;
                            value = value + 1;
                        }
                        else
                            break;
                    } while ((int)data < 0x10055024);
                    value++;
                    if (value < 3) {
                        eqmain_fn_10008910_guess(g_eqmain_slots_guess[value]);
                    }
                    else {
                        eqmain_fn_10008910_guess(g_eqmain_slots_guess[0]);
                    }
                }
                break;
            case 4:
                if (*(char *)(event + 2) == '\x1b') {
                    eqmain_fn_10003a00_guess();
                    done = true;
                }
                break;
            }
            eqmain_fn_10033568_guess(event);
        }
        if (done) {
            g_eqmain_flag_ce78_guess = 0;
            eqmain_fn_100194c0_guess();
            eqmain_fn_10001700_guess(firstButton);
            eqmain_fn_10001700_guess(secondButton);
            eqmain_fn_100039e0_arg_guess(0);
            result = 3;
            if (!returnThree) {
                result = 0x17;
            }
            return result;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x100197F0
// Displays a temporary wait screen, then clears it after input or a five-second timeout.
// __cdecl, one pointer argument (a 16-byte buffer), returns nothing. Passes g_eqmain_ceb8_guess, the buffer and 0x10 to
// PackFsBlitNamedImageAtOrigin_guess, then calls TickBlinkAndCursor_guess. Polls PumpMessagesAndEditText_guess (__stdcall, returns a pointer or 0) until a result's first
// dword is 4, or timeGetTime has advanced 5000 ms or more since its first reading. Each non-null result is freed.
// Then BltColorFillRgb_guess(g_eqmain_ceb8_guess, 0) and TickBlinkAndCursor_guess run four times, g_eqmain_flag_ce78_guess is cleared and ClearBlinkAndPump_guess is called.
void __cdecl EQMain_ShowWaitAndClear_guess(char *bitmapName)
{
    bool done;
    unsigned long start;
    unsigned long now;
    unsigned long elapsed;
    int *msg;
    EqMainTimeGetTimeFn_guess get_time;

    done = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, bitmapName, 0x10);
    eqmain_fn_10003a00_guess();
    get_time = g_eqmain_timegettime_guess;
    start = get_time();
    now = get_time();
    elapsed = now - start;
    while (elapsed < 5000 && !done) {
        msg = (int *)eqmain_fn_10003bd0_guess();
        done = false;
        if (msg != 0) {
            if (*msg == 4) done = true;
            eqmain_fn_10033568_guess(msg);
        }
        now = get_time();
        elapsed = now - start;
    }
    eqmain_fn_10006b90_guess(g_eqmain_ceb8_guess, 0);
    eqmain_fn_10003a00_guess();
    eqmain_fn_10006b90_guess(g_eqmain_ceb8_guess, 0);
    eqmain_fn_10003a00_guess();
    eqmain_fn_10006b90_guess(g_eqmain_ceb8_guess, 0);
    eqmain_fn_10003a00_guess();
    eqmain_fn_10006b90_guess(g_eqmain_ceb8_guess, 0);
    eqmain_fn_10003a00_guess();
    g_eqmain_flag_ce78_guess = 0;
    eqmain_fn_100039e0_arg_guess(0);
}

#define eqmain_fn_10001520_guess   EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))   // eqmain_create_button_guess: cdecl, five pushed args (the byte is a pushed 0), returns the new handle
#define eqmain_fn_100080b0_guess   EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int))   // EQMain_FitFontSize_guess: cdecl, six pushed args (Ghidra shows four), returns the text it was given
#define eqmain_fn_100017b0_guess   EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))   // eqmain_draw_text_guess: cdecl, five pushed args (Ghidra shows three)
#define eqmain_fn_100216f0_noargs_guess   EQ_FUNC(void, __fastcall, 0x100216F0, (void))   // CleanupSetup_guess called with no ECX set: the original loads none
#define eqmain_fn_10001ff0_ret_guess   EQ_CDECL(char, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, returns a char (nonzero: yes)
#define g_eqmain_text_5834_guess   EQ_GLOBAL(char *, 0x10055834)   // g_eqmain_text_5834_guess: the text of start-screen item 1
#define g_eqmain_text_5874_guess   EQ_GLOBAL(char *, 0x10055874)   // g_eqmain_text_5874_guess: the text of item 2
#define g_eqmain_text_587c_guess   EQ_GLOBAL(char *, 0x1005587C)   // g_eqmain_text_587c_guess: the text of item 3
#define g_eqmain_text_58c0_guess   EQ_GLOBAL(char *, 0x100558C0)   // g_eqmain_text_58c0_guess: the text of item 4
#define g_eqmain_text_5830_guess   EQ_GLOBAL(char *, 0x10055830)   // g_eqmain_text_5830_guess: the text of item 5
#define g_eqmain_text_5728_guess   EQ_GLOBAL(char *, 0x10055728)   // g_eqmain_text_5728_guess: the text item 5 copies to g_eqmain_copybuf_guess
#define g_eqmain_text_b_guess   EQ_GLOBAL(char *, 0x1005586C)   // g_eqmain_text_b_guess: the third argument of eqmainRunDialog_guess here
#define g_eqmain_text_a_guess   EQ_GLOBAL(char *, 0x10055868)   // g_eqmain_text_a_guess: the fourth argument of eqmainRunDialog_guess here
#define g_eqmain_choice_814c_guess EQ_GLOBAL(short, 0x1004814C)   // g_eqmain_login_code_guess: set to 0x15 when item 1 (or ENTER) is chosen
#define g_eqmain_evstart_bmp_guess EQ_ARRAY(char, 0x10047F7C)
#define eqmain_fn_100044b0_guess   EQ_CDECL(void, 0x100044B0, (int, int, int, int))
#define eqmain_fn_10001700_guess   EQ_CDECL(void, 0x10001700, (void *))
#define g_eqmain_text_4c10_guess   EQ_ARRAY(char, 0x100F4C10)

// FUNCTION: EQMAIN 0x100198B0
// Runs the start menu of five items; returns the chosen item's code, 3 on ESC; item 5 asks OK/cancel, then ESC
// @early-stop: initialization and cleanup local stack slots still differ (0.9127 similarity, 33 differing instructions).
// __stdcall with no arguments (its RET pops 0 bytes); returns a 16-bit value in AX: the chosen item's number (0, 1, 2, 0x13) or
// 3 after ESC. Draws EQVstart.bmp on g_eqmain_ceb8_guess (PackFsBlitNamedImageAtOrigin_guess), five items at x 400 and y 0xd2, 0xff, 0x12c, 0x159, 0x186
// (eqmain_create_button_guess) with their texts (EQMain_FitFontSize_guess chained, then eqmain_draw_text_guess for each), sets g_eqmain_flag_ce78_guess to 1, calls TickBlinkAndCursor_guess
// and CleanupSetup_guess. Then polls PumpMessagesAndEditText_guess messages: a type 0 message on item 1 (or an ENTER, type 4) returns 0 and sets
// g_eqmain_login_code_guess to 0x15; item 2 returns 1, item 4 returns 0x13, item 3 returns 2. Item 5 copies g_eqmain_text_5728_guess's text to g_eqmain_copybuf_guess
// and asks eqmainRunDialog_guess; a nonzero answer posts an ESC with EnqueueQuad_guess. An ESC returns 3. Every message is freed. On the way out
// the five items are freed (eqmain_object_destroy_guess), g_eqmain_flag_ce78_guess is cleared and ClearBlinkAndPump_guess is called with 0.
short __stdcall EQMain_RunStartScreen_guess(void)
{
    bool done;
    int item1;
    int item2;
    int item3;
    int item4;
    int item5;
    int choice;
    int text_ret;

    done = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, g_eqmain_evstart_bmp_guess, 0x10);
    item1 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0xd2, 0, 2);
    item2 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0xff, 0, 2);
    item3 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 300, 0, 2);
    item4 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0x159, 0, 2);
    item5 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0x186, 0, 2);
    text_ret = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_5834_guess, 0x19, 0);
    text_ret = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_5874_guess, text_ret, 0);
    text_ret = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_587c_guess, text_ret, 0);
    text_ret = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_58c0_guess, text_ret, 0);
    text_ret = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_5830_guess, text_ret, 0);
    eqmain_fn_100017b0_guess(item1, 0, g_eqmain_text_5834_guess, text_ret, 0);
    eqmain_fn_100017b0_guess(item2, 0, g_eqmain_text_5874_guess, text_ret, 0);
    eqmain_fn_100017b0_guess(item3, 0, g_eqmain_text_587c_guess, text_ret, 0);
    eqmain_fn_100017b0_guess(item4, 0, g_eqmain_text_58c0_guess, text_ret, 0);
    eqmain_fn_100017b0_guess(item5, 0, g_eqmain_text_5830_guess, text_ret, 0);
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_10003a00_guess();
    eqmain_fn_100216f0_noargs_guess();
    choice = item1;
    do {
        int *msg = (int *)eqmain_fn_10003bd0_guess();
        if (msg == 0)
            continue;
        if (*msg != 0) {
            if (*msg == 4) {
                if ((char)msg[2] != '\r') {
                    if ((char)msg[2] == '\x1b') {
                        eqmain_fn_10003a00_guess();
                        choice = 3;
                        done = true;
                    }
                }
                else {
                    choice = 0;
                    g_eqmain_choice_814c_guess = 0x15;
                    done = true;
                }
            }
        }
        else {
            int target;
            target = msg[2];
            if (target == item1) {
                choice = 0;
                g_eqmain_choice_814c_guess = 0x15;
                done = true;
            }
            else if (target == item2) {
                choice = 1;
                done = true;
            }
            else if (target == item4) {
                choice = 0x13;
                done = true;
            }
            else if (target == item3) {
                choice = 2;
                done = true;
            }
            else if (target == item5) {
                strcpy(g_eqmain_text_4c10_guess, g_eqmain_text_5728_guess);
                if (eqmain_fn_10001ff0_ret_guess(g_eqmain_ceb8_guess, g_eqmain_text_4c10_guess,
                                                 g_eqmain_text_b_guess, g_eqmain_text_a_guess) != 0) {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
            }
        }
        eqmain_fn_10033568_guess(msg);
        if (done) {
            g_eqmain_flag_ce78_guess = 0;
            eqmain_fn_10001700_guess((void *)item1);
            eqmain_fn_10001700_guess((void *)item2);
            eqmain_fn_10001700_guess((void *)item4);
            eqmain_fn_10001700_guess((void *)item3);
            eqmain_fn_10001700_guess((void *)item5);
            eqmain_fn_100039e0_arg_guess(0);
            return (short)choice;
        }
    } while (true);
}

#define eqmain_fn_10008390_guess   EQ_CDECL(void *, 0x10008390, (int, unsigned int, int, int, char *, int, int))   // EQMain_DrawCenteredText_guess: cdecl, seven pushed arguments (Ghidra shows five)
#define g_eqmain_text_surface_guess  EQ_GLOBAL(int, 0x1005543C)   // g_eqmain_text_surface_guess: the surface the text is drawn on (first argument of BltColorFillRgb_guess and EQMain_DrawCenteredText_guess)
#define g_eqmain_text_cursor_guess   EQ_GLOBAL(char *, 0x10055444)   // g_eqmain_text_cursor_guess: the text cursor, moved past each line read
#define g_eqmain_text_state_guess    EQ_GLOBAL(short, 0x100480B4)   // g_eqmain_text_state_guess: set to 0x18 when the cursor is at the end of the text
#define g_eqmain_text_buf_guess      EQ_ARRAY(unsigned char, 0x1005503C)   // g_eqmain_text_buf_guess: the line copied from the cursor, given to DecodeToBuffer_guess
// FUNCTION: EQMAIN 0x10019BD0
// Displays the next line of scrolling front-end text, including its color marker.
// __stdcall with no arguments (its RET pops 0 bytes); returns nothing. Calls BltColorFillRgb_guess(g_eqmain_text_surface_guess, 0xff00ff), then reads
// the line at the text cursor g_eqmain_text_cursor_guess. A NUL there sets g_eqmain_text_state_guess to 0x18 and returns; a newline there moves the
// cursor past it and returns. A "<?>" tag at the cursor is skipped, and its middle character 'y' selects 0x3cd9f5 for the
// colour (else 0xf3f2f2). The text up to the next newline or NUL is copied to g_eqmain_text_buf_guess (a newline is skipped too), then
// EQMain_DrawCenteredText_guess draws it on g_eqmain_text_surface_guess at x 0xfa, y 0, with the colour, the string DecodeToBuffer_guess returns, 2 and 0x258.
void __stdcall EQMain_ShowNextTextLine_guess(void)
{
    int textColor;
    char currentChar;
    unsigned char *lineWrite;

    textColor = 0xf3f2f2;
    eqmain_fn_10006b90_guess(g_eqmain_text_surface_guess, 0xff00ff);
    currentChar = *g_eqmain_text_cursor_guess;
    if (currentChar == '\0') {
        g_eqmain_text_state_guess = 0x18;
        return;
    }
    if (currentChar == '\n') {
        g_eqmain_text_cursor_guess = g_eqmain_text_cursor_guess + 1;
        return;
    }
    if (currentChar == '<' && g_eqmain_text_cursor_guess[2] == '>') {
        if (g_eqmain_text_cursor_guess[1] == 'y') {
            textColor = 0x3cd9f5;
        }
        g_eqmain_text_cursor_guess = g_eqmain_text_cursor_guess + 3;
    }
    currentChar = *g_eqmain_text_cursor_guess;
    lineWrite = g_eqmain_text_buf_guess;
    while (currentChar != '\n' && currentChar != '\0') {
        *lineWrite = currentChar;
        lineWrite = lineWrite + 1;
        g_eqmain_text_cursor_guess = g_eqmain_text_cursor_guess + 1;
        currentChar = *g_eqmain_text_cursor_guess;
    }
    *lineWrite = '\0';
    if (*g_eqmain_text_cursor_guess == '\n') {
        g_eqmain_text_cursor_guess = g_eqmain_text_cursor_guess + 1;
    }
    eqmain_fn_10008390_guess(g_eqmain_text_surface_guess, 0xfa, 0, textColor,
        eqmain_fn_10021110_guess(g_eqmain_text_buf_guess), 2, 0x258);
    return;
}

#define eqmain_fn_10008390_guess   EQ_CDECL(void *, 0x10008390, (int, unsigned int, int, int, char *, int, int))   // EQMain_DrawCenteredText_guess: cdecl, seven pushed arguments (Ghidra shows five)
#define g_eqmain_evstart_bmp_guess EQ_ARRAY(char, 0x10047F7C)   // s_EQVstart.bmp_10047f7c: "EQVstart.bmp", the second argument of PackFsBlitNamedImageAtOrigin_guess
#define g_eqmain_str_1005596c_guess   EQ_GLOBAL(char *, 0x1005596C)   // DAT_1005596c: a string pointer, the fifth argument of the first EQMain_DrawCenteredText_guess
#define g_eqmain_str_559b8_guess   EQ_GLOBAL(char *, 0x100559B8)   // g_eqmain_str_559b8_guess: a string pointer, the fifth argument of the second EQMain_DrawCenteredText_guess
#define eqmain_fn_10007ae0_guess   EQ_CDECL(int *, 0x10007AE0, (char *, int))   // PackFsGetNamedImage_guess: cdecl, a string and a size, returns a surface
#define eqmain_fn_10007cf0_guess   EQ_CDECL(void, 0x10007CF0, (int *))   // PackFsHalveSurface16_guess: cdecl, one pointer argument, returns at once
#define eqmain_fn_10006d80_guess   EQ_CDECL(int *, 0x10006D80, (int, int, int))   // CreateSurfaceRecord_guess: cdecl, three arguments, returns a surface
#define eqmain_fn_10001680_guess   EQ_CDECL(unsigned int *, 0x10001680, (int, int, int, int, int))   // eqmain_create_rect_widget_guess: cdecl, five arguments, returns a pointer
#define eqmain_fn_1003342e_guess   EQ_CDECL(void *, 0x1003342E, (size_t))   // _malloc: CRT, __cdecl, one size argument
#define eqmain_fn_100345ed_guess   EQ_CDECL(int, 0x100345ED, (struct _iobuf *, long, int))   // _fseek: CRT, __cdecl, FILE *, offset, origin
#define eqmain_fn_10033e8f_guess   EQ_CDECL(long, 0x10033E8F, (struct _iobuf *))   // _ftell: CRT, __cdecl, the position
#define eqmain_fn_100044b0_guess   EQ_CDECL(void, 0x100044B0, (int, int, int, int))   // EnqueueQuad_guess: cdecl, four arguments
#define eqmain_fn_10007a80_guess   EQ_CDECL(void, 0x10007A80, (int *))   // PackFsFreeEntry_guess: cdecl, one pointer argument
#define eqmain_fn_10001700_guess   EQ_CDECL(void, 0x10001700, (void *))   // eqmain_object_destroy_guess: cdecl, one pointer argument
#define eqmain_fn_10019bd0_guess   EQ_STDCALL(void, 0x10019BD0, (void))   // EQMain_ShowNextTextLine_guess: __stdcall, no arguments (EQMain_ShowNextTextLine_guess)
#define g_eqmain_credits_name_guess  EQ_ARRAY(char, 0x100480B8)   // s_credits.txt_100480b8: "credits.txt", the name Identity_guess maps to a path
#define g_eqmain_credits_key_guess   EQ_GLOBAL(char *, 0x100559B4)   // g_eqmain_credits_key_guess: the key CopyStringToBuffer_guess looks up when the credits can't be read
#define g_eqmain_credits_buf_guess   EQ_GLOBAL(void *, 0x10055440)   // g_eqmain_credits_buf_guess: the credits text (malloc'd, or the key's text)
#define eqmain_fn_1003407f_guess   EQ_CDECL(struct _iobuf *, 0x1003407F, (char *, char *))   // FUN_1003407f: fopen-like (CRT, not confirmed), returns a FILE *
#define eqmain_fn_100342a2_guess   EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, struct _iobuf *))   // _fread: __cdecl, four arguments
#define eqmain_fn_100343b9_guess   EQ_CDECL(int, 0x100343B9, (struct _iobuf *))   // _fclose: __cdecl, one FILE *
#define g_eqmain_fopen_mode_guess  EQ_ARRAY(char, 0x100480A4)   // g_eqmain_fopen_mode_guess: passed by address as the second (mode) argument

// FUNCTION: EQMAIN 0x10019C90
// Plays the credits roll over the front-end title artwork.
// __stdcall with no arguments (its RET pops 0 bytes); returns nothing. Loads "..." (PackFsGetNamedImage_guess) and blits it on
// g_eqmain_ceb8_guess, then opens "..." (Identity_guess, mode g_eqmain_fopen_mode_guess) into a malloc'd buffer of size + 2000 bytes: zeroed,
// read, and NUL-terminated twice. Without the file, or when malloc fails, the text is CopyStringToBuffer_guess(g_eqmain_credits_key_guess). It scrolls the
// text 20 rows at a time until g_eqmain_text_state_guess counts down to 0 or PumpMessagesAndEditText_guess returns an Escape message (type 4, byte 0x1b), then
// frees its surfaces and the buffer, clears g_eqmain_flag_ce78_guess and calls ClearBlinkAndPump_guess (the caller pushes 0 first).
void __stdcall EQMain_RunCredits_guess(void)
{
    bool done;
    short scrollOffset;
    int *background;
    int *backBuffer;
    int *scrollSurface;
    FILE *_File;
    size_t length;
    unsigned int *ptr;
    int *_Memory;
    unsigned int dwordCount;
    unsigned int size;
    int offsetY;

    done = false;
    scrollOffset = 0;
    background = eqmain_fn_10007ae0_guess(g_eqmain_evstart_bmp_guess, 0x10);
    eqmain_fn_10007cf0_guess(background);
    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0, 0, (int)background, 0, 0, *background, background[1], 0);
    backBuffer = eqmain_fn_10006d80_guess(500, 0x1de, 0x10);
    scrollSurface = eqmain_fn_10006d80_guess(500, 0x1de, 0x10);
    eqmain_fn_10006b90_guess((int)scrollSurface, 0xff00ff);
    _File = eqmain_fn_1003407f_guess(eqmain_fn_100210f0_guess(g_eqmain_credits_name_guess), g_eqmain_fopen_mode_guess);
    if (_File == 0)
    {
        g_eqmain_credits_buf_guess = eqmain_fn_100211b0_guess(g_eqmain_credits_key_guess);
    }
    else
    {
        eqmain_fn_100345ed_guess(_File, 0, 2);
        length = eqmain_fn_10033e8f_guess(_File);
        eqmain_fn_100345ed_guess(_File, 0, 0);
        size = length + 2000;
        g_eqmain_credits_buf_guess = eqmain_fn_1003342e_guess(size);
        if (g_eqmain_credits_buf_guess == 0)
        {
            g_eqmain_credits_buf_guess = eqmain_fn_100211b0_guess(g_eqmain_credits_key_guess);
        }
        else
        {
            ptr = (unsigned int *)g_eqmain_credits_buf_guess;
            for (dwordCount = size >> 2; dwordCount != 0; dwordCount = dwordCount - 1)
            {
                *ptr = 0;
                ptr = ptr + 1;
            }
            for (size = size & 3; size != 0; size = size - 1)
            {
                *(unsigned char *)ptr = 0;
                ptr = (unsigned int *)((int)ptr + 1);
            }
            length = eqmain_fn_100342a2_guess(g_eqmain_credits_buf_guess, 1, length, _File);
            *((char *)g_eqmain_credits_buf_guess + length) = 0;
            *((char *)g_eqmain_credits_buf_guess + length + 1) = 0;
        }
        eqmain_fn_100343b9_guess(_File);
    }
    g_eqmain_text_cursor_guess = (char *)g_eqmain_credits_buf_guess;
    g_eqmain_text_state_guess = -1;
    g_eqmain_text_surface_guess = (int)eqmain_fn_10006d80_guess(500, 0x14, 0x10);
    eqmain_fn_10019bd0_guess();
    ptr = eqmain_fn_10001680_guess(g_eqmain_ceb8_guess, 0, 0, 0x280, 0x1e0);
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_10003a00_guess();
    do
    {
        eqmain_fn_10006c20_guess((int)backBuffer, 0, 0, (int)background, 0x46, 1, 500, 0x1de, 0);
        offsetY = (int)scrollOffset;
        eqmain_fn_10006c20_guess((int)backBuffer, 0, 0, (int)scrollSurface, 0, offsetY, 500, 0x1de - offsetY, 1);
        if (scrollOffset != 0)
        {
            eqmain_fn_10006c20_guess((int)backBuffer, 0, 0x1de - offsetY, (int)g_eqmain_text_surface_guess, 0, 0, 500, offsetY, 1);
        }
        eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x46, 1, (int)backBuffer, 0, 0, 500, 0x1de, 0);
        scrollOffset = scrollOffset + 1;
        if (0x13 < scrollOffset)
        {
            eqmain_fn_10006c20_guess((int)scrollSurface, 0, 0, (int)scrollSurface, 0, 0x14, 500, 0x1ca, 0);
            eqmain_fn_10006c20_guess((int)scrollSurface, 0, 0x1ca, (int)g_eqmain_text_surface_guess, 0, 0, 500, 0x14, 0);
            scrollOffset = 0;
            if (g_eqmain_text_state_guess == -1)
            {
                eqmain_fn_10019bd0_guess();
            }
            else
            {
                g_eqmain_text_state_guess = g_eqmain_text_state_guess + -1;
                if (g_eqmain_text_state_guess < 1)
                {
                    done = true;
                }
            }
        }
        _Memory = (int *)eqmain_fn_10003bd0_guess();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                if ((unsigned int *)_Memory[2] == ptr)
                {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
            }
            else if ((*_Memory == 4) && ((char)_Memory[2] == '\x1b'))
            {
                eqmain_fn_10003a00_guess();
                done = true;
            }
            eqmain_fn_10033568_guess(_Memory);
        }
    } while (!done);
    eqmain_fn_10007a80_guess(backBuffer);
    eqmain_fn_10007a80_guess(background);
    eqmain_fn_10007a80_guess(scrollSurface);
    eqmain_fn_10007a80_guess((int *)g_eqmain_text_surface_guess);
    eqmain_fn_10001700_guess(ptr);
    g_eqmain_flag_ce78_guess = 0;
    eqmain_fn_100039e0_arg_guess(0);
}

// FUNCTION: EQMAIN 0x10019FF0
// Displays the title screen backdrop and its two introductory messages.
// __stdcall with no arguments (its RET pops 0 bytes); returns nothing. Calls PackFsBlitNamedImageAtOrigin_guess(g_eqmain_ceb8_guess, "...", 0x10),
// then EQMain_DrawCenteredText_guess twice on g_eqmain_ceb8_guess at x 0x140, y 0xf0 and y 0x104 with 0x3cd9f5, DAT_1005596c or g_eqmain_str_559b8_guess, 2 and
// 0x258. Sets g_eqmain_flag_ce78_guess to 1, then calls TickBlinkAndCursor_guess (the original tail-jumps to it).
void __stdcall EQMain_ShowStartBitmap_guess(void)
{
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, g_eqmain_evstart_bmp_guess, 0x10);
    eqmain_fn_10008390_guess(g_eqmain_ceb8_guess, 0x140, 0xf0, 0x3cd9f5, g_eqmain_str_1005596c_guess, 2, 0x258);
    eqmain_fn_10008390_guess(g_eqmain_ceb8_guess, 0x140, 0x104, 0x3cd9f5, g_eqmain_str_559b8_guess, 2, 0x258);
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_10003a00_guess();
}

// FUNCTION: EQMAIN 0x1001A070
// Resets the front-end screen activity flag.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Clears the flag byte g_eqmain_flag_ce78_guess, then
// calls ClearBlinkAndPump_guess with the cdecl argument 0.
#define g_eqmain_flag_ce78_guess   EQ_GLOBAL(unsigned char, 0x1004CE78)   // g_eqmain_flag_ce78_guess: a flag byte, cleared before ClearBlinkAndPump_guess
#define eqmain_fn_100039e0_guess   EQ_STDCALL(void, 0x100039E0, (void))   // ClearBlinkAndPump_guess: __stdcall, no arguments
void __stdcall EQMain_ClearFlagAndRun_guess(void)
{
    g_eqmain_flag_ce78_guess = 0;
    eqmain_fn_100039e0_arg_guess(0);
}

// FUNCTION: EQMAIN 0x1001A080
// Needs a non-empty PIN text; tries 0x10021730 (error dialog on failure), then sends name and PIN; 1 = sent.
// __cdecl with two stack arguments (the caller pops them); returns a byte in AL (callers read AL only). An empty
// string in loginPin returns 0. Otherwise, when g_eqmain_pin_flag_guess == 0, EQMain_DrawStrings_guess prints DAT_1005596c, then ConnectAndDescribe_guess
// tests the buffer at g_eqmain_copybuf_guess: when it is 0, eqmainRunDialog_guess gets g_eqmain_ceb8_guess, that buffer, g_str_888_guess and 0,
// and the function returns 0. Otherwise, when g_eqmain_pin_flag_guess == 0 again, EQMain_DrawStrings_guess prints g_eqmain_text_59bc_guess, SendNameTextMessage_guess
// gets loginName, loginPin, 0 and 0, and the function returns 1.
#define g_eqmain_pin_flag_guess       EQ_GLOBAL(int, 0x100480C4)   // g_eqmain_pin_flag_guess: 0 here prints the messages (EQMain_ConfirmNewPin_guess tests 0x20)
#define g_eqmain_copybuf_guess       EQ_ARRAY(char, 0x100F4C10)   // g_eqmain_copybuf_guess: a char buffer, passed by its address
#define g_eqmain_text_5888_guess      EQ_GLOBAL(char *, 0x10055888)   // g_str_888_guess: the LPCSTR eqmainRunDialog_guess takes as its third argument
#define g_eqmain_text_59bc_guess      EQ_GLOBAL(char *, 0x100559BC)   // g_eqmain_text_59bc_guess: the text EQMain_DrawStrings_guess prints second
#define eqmain_fn_1001a340_guess      EQ_CDECL(void, 0x1001A340, (char *))   // EQMain_DrawStrings_guess: cdecl, one string
#define eqmain_fn_10021730_guess      EQ_CDECL(unsigned int, 0x10021730, (char *))   // ConnectAndDescribe_guess: cdecl, one string, a flag in AL
#define eqmain_fn_10001ff0_guess      EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, result not used
#define eqmain_fn_100217e0_guess      EQ_CDECL(unsigned char, 0x100217E0, (char *, char *, char *, unsigned int))   // SendNameTextMessage_guess: cdecl, four args, result not used
unsigned char __cdecl EQMain_SubmitIfValid_guess(char *loginName, char *loginPin)
{
    unsigned int result;

    if (strlen(loginPin) == 0)
    {
        return 0;
    }
    if (g_eqmain_pin_flag_guess == 0)
    {
        eqmain_fn_1001a340_guess(g_eqmain_str_1005596c_guess);
    }
    result = eqmain_fn_10021730_guess(g_eqmain_copybuf_guess);
    if ((char)result == 0)
    {
        eqmain_fn_10001ff0_guess(g_eqmain_ceb8_guess,g_eqmain_copybuf_guess, g_eqmain_text_5888_guess, (char *)0);
        return 0;
    }
    if (g_eqmain_pin_flag_guess == 0)
    {
        eqmain_fn_1001a340_guess(g_eqmain_text_59bc_guess);
    }
    eqmain_fn_100217e0_guess(loginName, loginPin, (char *)0, 0);
    return 1;
}

// FUNCTION: EQMAIN 0x1001A120
// Submits a new PIN when both confirmation entries match.
// __cdecl with two stack arguments (the caller pops them); returns a byte in AL (callers read AL only). An empty
// string in either argument returns 0. Otherwise, when the two strings are equal, g_eqmain_pin_flag_guess == 0x20 makes
// EQMain_DrawStrings_guess print "...", SendNameMessage6F_guess gets newPin and the function returns 1. When they differ,
// the same flag makes EQMain_DrawStrings_guess print "..." and the function returns 0.
#define g_eqmain_pin_flag_guess       EQ_GLOBAL(int, 0x100480C4)   // g_eqmain_pin_flag_guess: when 0x20, the PIN messages are printed
#define g_eqmain_str_updating_guess   EQ_ARRAY(char, 0x100480C8)   // s_Updating_to_new_PIN_100480c8: "..."
#define g_eqmain_str_mismatch_guess   EQ_ARRAY(char, 0x100480DC)   // s_PINs_must_match_100480dc: "..."
#define eqmain_fn_1001a340_guess      EQ_CDECL(void, 0x1001A340, (char *))   // EQMain_DrawStrings_guess: cdecl, one string
#define eqmain_fn_10021a00_guess      EQ_CDECL(void, 0x10021A00, (char *))   // SendNameMessage6F_guess: cdecl, one string; its result is not used
unsigned char __cdecl EQMain_ConfirmNewPin_guess(char *newPin, char *confirmPin)
{
    if (strlen(newPin) == 0)
    {
        return 0;
    }
    if (strlen(confirmPin) == 0)
    {
        return 0;
    }
    if (strcmp(newPin, confirmPin) != 0)
    {
        if (g_eqmain_pin_flag_guess == 0x20)
        {
            eqmain_fn_1001a340_guess(g_eqmain_str_mismatch_guess);
        }
        return 0;
    }
    if (g_eqmain_pin_flag_guess == 0x20)
    {
        eqmain_fn_1001a340_guess(g_eqmain_str_updating_guess);
    }
    eqmain_fn_10021a00_guess(newPin);
    return 1;
}

// FUNCTION: EQMAIN 0x1001A1D0
// Submits the next account code for checking.
// __cdecl with one stack argument (the caller pops it); returns a byte in AL (callers read AL only). An empty string
// returns 0. Otherwise, when g_eqmain_pin_flag_guess is 0x21, EQMain_DrawStrings_guess is called with "..." first; then
// SendNameMessage_guess gets nextCode and the function returns 1.
#define g_eqmain_checking_str_guess  EQ_ARRAY(char, 0x100480EC)   // s_Checking_Next_Code_100480ec: "..."
#define eqmain_fn_1001a340_guess     EQ_CDECL(void, 0x1001A340, (char *))   // EQMain_DrawStrings_guess: cdecl, one string
#define eqmain_fn_100219a0_guess     EQ_CDECL(void, 0x100219A0, (char *))   // SendNameMessage_guess: cdecl, one string; its result is not used
unsigned char __cdecl EQMain_CheckNextCode_guess(char *nextCode)
{
    if (strlen(nextCode) == 0)
    {
        return 0;
    }
    if (g_eqmain_pin_flag_guess == 0x21)
    {
        eqmain_fn_1001a340_guess(g_eqmain_checking_str_guess);
    }
    eqmain_fn_100219a0_guess(nextCode);
    return 1;
}

// FUNCTION: EQMAIN 0x1001A210
// Shows a dialog for selected codes; their meaning in the front end is unclear.
// __stdcall with no arguments; returns the low 16 bits of a code in AX (callers read AX only). The code comes from
// RdpReceiveDispatch_guess. Codes 2 and 0x45 below 0x70, codes 0x70..0x73, and codes 0x77 and 0x10000003 above 0x73 first call
// eqmainRunDialog_guess with g_eqmain_ceb8_guess, the text at g_eqmain_copybuf_guess, g_str_888_guess and 0. Other codes return without the call.
#define eqmain_fn_10021c20_guess   EQ_STDCALL(unsigned int, 0x10021C20, (void))   // RdpReceiveDispatch_guess: __stdcall, no arguments, returns the code
#define eqmain_fn_10001ff0_guess   EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))   // eqmainRunDialog_guess: cdecl, four arguments, result not used
#define g_str_888_guess    EQ_GLOBAL(char *, 0x10055888)   // g_str_888_guess: the third argument of eqmainRunDialog_guess
#define g_eqmain_text_4c10_guess   EQ_ARRAY(char, 0x100F4C10)   // g_eqmain_copybuf_guess: the text, passed by address as the second argument
short __stdcall EQMain_ReportCode_guess(void)
{
    unsigned int code;

    code = eqmain_fn_10021c20_guess();
    if ((int)code <= 0x73)
    {
        if ((int)code < 0x70)
        {
            switch (code)
            {
            case 2:
            case 0x45:
                break;
            default:
                return (short)code;
            }
        }
    }
    else if (code != 0x77 && code != 0x10000003)
    {
        goto returnCode;
    }
    eqmain_fn_10001ff0_guess(g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, 0);
returnCode:
    return (short)code;
}

#define eqmain_fn_1003407f_guess   EQ_CDECL(struct _iobuf *, 0x1003407F, (char *, char *))   // FUN_1003407f: fopen-like (CRT, not confirmed), returns a FILE *
#define eqmain_fn_100342a2_guess   EQ_CDECL(size_t, 0x100342A2, (void *, size_t, size_t, struct _iobuf *))   // _fread: __cdecl, four arguments
#define eqmain_fn_100343b9_guess   EQ_CDECL(int, 0x100343B9, (struct _iobuf *))   // _fclose: __cdecl, one FILE *
#define g_eqmain_equname_guess     EQ_ARRAY(char, 0x10047F68)   // g_eqmain_equname_guess: "...", the file name
#define g_eqmain_fopen_mode_guess  EQ_ARRAY(char, 0x100480A4)   // g_eqmain_fopen_mode_guess: passed by address as the second (mode) argument

// FUNCTION: EQMAIN 0x1001A260
// Reads the configured EQ name from equname.txt, falling back to the base string.
// __cdecl with one stack argument (the caller pops it); returns a byte in AL (callers read AL only). Opens equname.txt
// with mode g_eqmain_fopen_mode_guess; if that works, reads 20 bytes into a local, closes the file, and when strlen of the local is
// 1..19 copies the string with its NUL to equName and returns 1. Otherwise copies the base string g_eqmain_drive_base_guess to
// equName and returns 0. Bug kept: with no NUL in the 20 bytes, strlen runs on past the local.
unsigned char __cdecl EQMain_ReadEquName_guess(char *equName)
{
    struct _iobuf *file;
    char name[20];
    int len;

    file = eqmain_fn_1003407f_guess(g_eqmain_equname_guess, g_eqmain_fopen_mode_guess);
    if (file != 0)
    {
        eqmain_fn_100342a2_guess(name, 0x14, 1, file);
        eqmain_fn_100343b9_guess(file);
        len = strlen(name);
        if (len > 0 && len < 0x14)
        {
            strcpy(equName, name);
            return 1;
        }
    }
    strcpy(equName, g_eqmain_drive_base_guess);
    return 0;
}

// FUNCTION: EQMAIN 0x1001A310
// Draws a narrow horizontal band on the front-end surface.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Blits a 300 x 20 strip from the
// source surface's (0, 0) to (310, 400) of the destination surface: 0x10006C20 with flags 0, nine stack
// arguments (the caller pops them). Both surfaces are read before the call, as in the original. The callee's EAX
// is left as it is; no caller reads it.
void __stdcall EQMain_BlitStrip_guess(void)
{
    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x136, 0x190, g_eqmain_blit_src_guess, 0, 0, 0x12c, 0x14, 0);
}

#define eqmain_fn_1001a310_guess   EQ_STDCALL(void, 0x1001A310, (void))   // EQMain_BlitStrip_guess: __stdcall, no arguments
#define eqmain_fn_100082c0_guess   EQ_CDECL(int, 0x100082C0, (int, int, int, unsigned int, char *, int, unsigned int))   // EQMain_DrawTextRun_guess: cdecl, seven dwords, returns the width
#define g_eqmain_str_559a0_guess   EQ_GLOBAL(char *, 0x100559A0)   // g_eqmain_str_559a0_guess: a string, drawn first

// FUNCTION: EQMAIN 0x1001A340
// Displays a front-end message beside its fixed label on the shared strip.
// __cdecl, one string argument (text); returns nothing. Blits the strip at 0x1001A310, draws the global string
// g_eqmain_str_559a0_guess at (0x136, 0x190) through 0x100082C0 and keeps the width it returns, then draws text at
// (width + 0x13e, 0x190). Both text calls push seven dwords (the last two are zero); the caller pops 0x38 bytes.
void __cdecl EQMain_DrawStrings_guess(char *text)
{
    int width;

    eqmain_fn_1001a310_guess();
    width = eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x136, 0x190, 0xc81000, g_eqmain_str_559a0_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, width + 0x13e, 0x190, 0x1818f0, text, 0, 0);
}

#define g_eqmain_text_5848_guess    EQ_GLOBAL(char *, 0x10055848)   // g_eqmain_text_5848_guess: the text eqmain_draw_text_guess draws with pHVar6
#define g_eqmain_text_5844_guess    EQ_GLOBAL(char *, 0x10055844)   // g_eqmain_text_5844_guess: the text eqmain_draw_text_guess draws with pHVar8
#define g_eqmain_copy_4d20_guess    EQ_ARRAY(char, 0x10114D20)      // g_eqmain_copy_4d20_guess: the copy of g_eqmain_drive_base_guess (strlen + 1 bytes)
#define g_eqmain_flag_cec4_guess    EQ_GLOBAL(int, 0x1004CEC4)      // g_eqmain_flag_cec4_guess: a dword, cleared by the start screen
#define g_eqmain_s_eqklogin_bmp_guess  EQ_ARRAY(char, 0x10048108)   // s_EQKlogin.bmp_10048108: "EQKlogin.bmp"
#define g_eqmain_s_next_code_guess  EQ_ARRAY(char, 0x10048140)      // s_Next_Code_10048140: "..."
#define eqmain_fn_100088f0_guess    EQ_CDECL(void, 0x100088F0, (int))                   // EQMain_AllocEmptyString_guess: cdecl, one int (the object)
#define eqmain_fn_1001a210_guess    EQ_STDCALL(short, 0x1001A210, (void))               // EQMain_ReportCode_guess: __stdcall, no arguments, a 16-bit key code in AX
#define eqmain_fn_1003396b_guess    EQ_CDECL(char *, 0x1003396B, (char *))              // __strdup: CRT, __cdecl, returns the new copy
#define eqmain_fn_1001a1d0_guess    EQ_CDECL(char, 0x1001A1D0, (char *))                // EQMain_CheckNextCode_guess: cdecl, one string, returns a byte (AL)

#define g_eqmain_s_new_pin_limit_guess EQ_ARRAY(char, 0x10048128)                           // "..."
#define g_eqmain_s_new_pin_again_guess EQ_ARRAY(char, 0x10048118)                           // "..."
#define eqmain_fn_1001a120_newpin_guess EQ_CDECL(unsigned int, 0x1001A120, (unsigned char *, unsigned char *)) // EQMain_ConfirmNewPin_guess
#define eqmain_fn_10035e41_newpin_guess EQ_CDECL(void, 0x10035E41, (void *))                // FUN_10035e41
#define eqmain_free_guess EQ_CDECL(void, 0x10033568, (void *))                // _free
#define eqmain_fn_100039e0_newpin_guess EQ_CDECL(unsigned int, 0x100039E0, (int))          // ClearBlinkAndPump_guess

#define g_eqmain_login_text_58d4_guess  EQ_GLOBAL(char *, 0x100558D4)   // g_eqmain_login_text_58d4_guess: the text drawn at (0x19a, 0xf0)
#define g_eqmain_login_text_58d0_guess  EQ_GLOBAL(char *, 0x100558D0)   // g_eqmain_login_text_58d0_guess: the text drawn at (400, 0x159)
#define g_eqmain_login_text_5970_guess  EQ_GLOBAL(char *, 0x10055970)   // g_eqmain_login_text_5970_guess: the first EQMain_DrawStrings_guess text of the ESC branch
#define g_eqmain_login_text_59c0_guess  EQ_GLOBAL(char *, 0x100559C0)   // g_eqmain_login_text_59c0_guess: the second EQMain_DrawStrings_guess text of the ESC branch
#define g_eqmain_login_text_5730_guess  EQ_GLOBAL(char *, 0x10055730)   // g_eqmain_login_text_5730_guess: copied to g_eqmain_copybuf_guess by key 0x54
#define g_eqmain_login_text_5734_guess  EQ_GLOBAL(char *, 0x10055734)   // g_eqmain_login_text_5734_guess: copied to g_eqmain_copybuf_guess by key 0x42
#define g_eqmain_login_text_58b0_guess  EQ_GLOBAL(char *, 0x100558B0)   // g_eqmain_login_text_58b0_guess: the third argument of eqmainRunDialog_guess
#define g_eqmain_login_text_58b4_guess  EQ_GLOBAL(char *, 0x100558B4)   // g_eqmain_login_text_58b4_guess: the fourth argument of eqmainRunDialog_guess
#define g_eqmain_login_code_guess       EQ_GLOBAL(short, 0x1004814C)    // g_eqmain_login_code_guess: a 16-bit result code
#define g_eqmain_login_name_buf_guess   EQ_ARRAY(char, 0x10104CD8)      // g_eqmain_login_name_buf_guess: the account name
#define g_eqmain_login_pin_buf_guess    EQ_ARRAY(char, 0x1012565C)      // g_eqmain_login_pin_buf_guess: the second text
#define g_str_10047f78_guess  EQ_ARRAY(char, 0x10047F78)      // DAT_10047f78: the text OpenLoginConnection_guess takes after the ESC reset
#define g_str_10047f74_guess EQ_ARRAY(char, 0x10047F74)      // DAT_10047f74: the mode of the equname.txt fopen
#define g_eqmain_login_s_login_guess    EQ_ARRAY(char, 0x10048100)      // s_login_10048100: "login"
#define eqmain_fn_1001a260_login_guess  EQ_CDECL(unsigned char, 0x1001A260, (char *))   // EQMain_ReadEquName_guess: cdecl, one buffer, a flag in AL
#define eqmain_fn_1001a080_login_guess  EQ_CDECL(unsigned char, 0x1001A080, (char *, char *))   // EQMain_SubmitIfValid_guess: cdecl, two texts, a flag in AL
#define eqmain_fn_100212e0_login_guess  EQ_CDECL(void, 0x100212E0, (unsigned char *))   // OpenLoginConnection_guess: cdecl, one text, result not used
#define eqmain_fn_10025830_guess  EQ_STDCALL(void, 0x10025830, (void))   // eqmain_call_100213d0_guess: __stdcall, no arguments
#define eqmain_fn_10034169_login_guess  EQ_CDECL(size_t, 0x10034169, (void *, size_t, size_t, struct _iobuf *))   // _fwrite: CRT, __cdecl, four arguments

// FUNCTION: EQMAIN 0x1001A3A0
// EQKlogin.bmp login form: name (prefilled from equname.txt) and PIN fields; writes the name back to equname.txt
// __stdcall, no arguments; returns a 16-bit code in AX (the high word is what ClearBlinkAndPump_guess left). Builds the EQKlogin
// screen like EQMain_NewPinDialog_guess, then polls keys (EQMain_ReportCode_guess) and messages (PumpMessagesAndEditText_guess) until finished.
short __stdcall EQMain_KeyLoginDialog_guess(void)
{
    bool finished;
    bool submitted;
    bool codeAccepted;
    short result;
    int key;
    int backItem;
    int loginItem;
    int exitItem;
    int item;
    int *rec;
    int *surface;
    char *src;
    char *nameCopy;
    char *pinCopy;
    FILE *file;
    EqMainControl_guess *nameEdit;
    EqMainControl_guess *pinEdit;
    char equName[20];

    finished = false;
    submitted = false;
    result = 7;
    codeAccepted = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, g_eqmain_s_eqklogin_bmp_guess, 0x10);
    backItem = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x14a, 4, 2);
    loginItem = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x118, 4, 2);
    eqmain_fn_100017b0_guess(loginItem, 0, g_eqmain_text_5834_guess, 1, 0);
    eqmain_fn_100017b0_guess(backItem, 0, g_eqmain_text_5848_guess, 1, 0);
    exitItem = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x17c, 4, 2);
    eqmain_fn_100017b0_guess(exitItem, 0, g_eqmain_text_5844_guess, 1, 0);
    nameEdit = (EqMainControl_guess *)eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x127, 0x104, 0x127, 0x1a, 0x14, 0, 1, 0x22d69, 1);
    pinEdit = (EqMainControl_guess *)eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x127, 0x16d, 0x127, 0x1a, 0x11, 0, 1, 0x22d69, 1);
    eqmain_fn_100088f0_guess((int)pinEdit);
    eqmain_fn_10008910_guess(nameEdit);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x19a, 0xf0, 0x21a3c, g_eqmain_login_text_58d4_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 400, 0x159, 0x21a3c, g_eqmain_login_text_58d0_guess, 0, 0);
    surface = eqmain_fn_10006d80_guess(300, 0x14, 0x10);
    g_eqmain_blit_src_guess = (int)surface;
    eqmain_fn_10006c20_guess((int)surface, 0, 0, g_eqmain_ceb8_guess, 0x136, 400, 300, 0x14, 0);
    if (eqmain_fn_1001a260_login_guess(equName) != 0)
    {
        eqmain_fn_10008970_guess(nameEdit, eqmain_fn_10021110_guess((unsigned char *)equName));
        eqmain_fn_10008910_guess(pinEdit);
    }
    g_eqmain_flag_ce78_guess = 1;
    strcpy(g_eqmain_copy_4d20_guess, g_eqmain_drive_base_guess);
    eqmain_fn_10003a00_guess();
    g_eqmain_pin_flag_guess = 0;
    do
    {
        key = eqmain_fn_1001a210_guess();
        switch (key)
        {
        case 0x12:
            result = 3;
            finished = true;
            break;
        case 0x1b:
            if (codeAccepted)
            {
                eqmain_fn_1001a340_guess(g_eqmain_login_text_5970_guess);
                g_eqmain_login_code_guess = 0xf;
                eqmain_fn_100216f0_noargs_guess();
                eqmain_fn_100212e0_login_guess((unsigned char *)g_str_10047f78_guess);
                eqmain_fn_1001a340_guess(g_eqmain_login_text_59c0_guess);
                eqmain_fn_1001a080_login_guess(g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess);
            }
            else
            {
                result = 0xc;
                finished = true;
            }
            break;
        case 0x42:
            src = g_eqmain_login_text_5734_guess;
            goto showDialog;
        case 4:
        case 0x51:
            file = eqmain_fn_1003407f_guess(g_eqmain_equname_guess, g_str_10047f74_guess);
            if (file != 0)
            {
                eqmain_fn_10034169_login_guess(eqmain_fn_100211b0_guess(g_eqmain_login_name_buf_guess), 0x14, 1, file);
                eqmain_fn_100343b9_guess(file);
            }
            result = g_eqmain_login_code_guess;
            finished = true;
            break;
        case 0x6e:
            finished = true;
            result = 0x21;
            break;
        case 0x54:
            src = g_eqmain_login_text_5730_guess;
showDialog:
            strcpy(g_eqmain_copybuf_guess, src);
            eqmain_fn_1001a310_guess();
            if (eqmain_fn_10001ff0_ret_guess(g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_eqmain_login_text_58b0_guess, g_eqmain_login_text_58b4_guess) == 0)
            {
                if (g_eqmain_login_code_guess == 9 || g_eqmain_login_code_guess == 10 || g_eqmain_login_code_guess == 0xb)
                {
                    result = g_eqmain_login_code_guess;
                    finished = true;
                }
                else
                {
                    result = 7;
                    finished = true;
                }
            }
            else
            {
                eqmain_fn_10025830_guess();
                codeAccepted = true;
            }
            break;
        case 0x6f:
            finished = true;
            result = 0x20;
            break;
        case 2:
        case 0x10000003:
            submitted = false;
        case 0x77:
            eqmain_fn_1001a310_guess();
            break;
        }
        rec = (int *)eqmain_fn_10003bd0_guess();
        if (rec != 0)
        {
            switch (rec[0])
            {
            case 0:
                item = rec[2];
                if ((EqMainControl_guess *)item == nameEdit)
                {
                    eqmain_fn_10008910_guess(pinEdit);
                }
                else if (item == exitItem)
                {
                    result = 0x16;
                    finished = true;
                }
                else if (item == backItem)
                {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
                else if (strlen(nameEdit->text) != 0)
                {
                    if (strlen(pinEdit->text) != 0)
                    {
                        if ((EqMainControl_guess *)item == pinEdit && !submitted)
                        {
                            nameCopy = eqmain_fn_1003396b_guess(nameEdit->text);
                            pinCopy = eqmain_fn_1003396b_guess(pinEdit->text);
                            eqmain_fn_100216f0_noargs_guess();
                            eqmain_fn_100212e0_login_guess((unsigned char *)g_eqmain_login_s_login_guess);
                            if (eqmain_fn_1001a080_login_guess(nameCopy, pinCopy) == 0)
                            {
                                eqmain_fn_1001a310_guess();
                                eqmain_fn_10035e41_newpin_guess(nameCopy);
                                eqmain_fn_10035e41_newpin_guess(pinCopy);
                            }
                            else
                            {
                                submitted = true;
                                eqmain_fn_10035e41_newpin_guess(nameCopy);
                                eqmain_fn_10035e41_newpin_guess(pinCopy);
                            }
                        }
                        else if (item == loginItem && !submitted)
                        {
                            eqmain_fn_100216f0_noargs_guess();
                            eqmain_fn_100212e0_login_guess((unsigned char *)g_eqmain_login_s_login_guess);
                            if (eqmain_fn_1001a080_login_guess(nameEdit->text, pinEdit->text) == 0)
                            {
                                eqmain_fn_1001a310_guess();
                            }
                            else
                            {
                                submitted = true;
                            }
                        }
                    }
                }
                break;
            case 4:
                if (*(char *)(rec + 2) == '\x1b')
                {
                    eqmain_fn_10003a00_guess();
                    g_eqmain_flag_cec4_guess = 0;
                    finished = true;
                }
                break;
            case 6:
            case 7:
                if ((EqMainControl_guess *)rec[2] == nameEdit)
                {
                    eqmain_fn_10008910_guess(pinEdit);
                }
                else if ((EqMainControl_guess *)rec[2] == pinEdit)
                {
                    eqmain_fn_10008910_guess(nameEdit);
                }
                break;
            }
            eqmain_fn_10033568_guess(rec);
        }
        if (finished)
        {
            g_eqmain_flag_cec4_guess = 0;
            g_eqmain_flag_ce78_guess = 0;
            eqmain_fn_10001700_guess((void *)loginItem);
            eqmain_fn_10001700_guess((void *)backItem);
            eqmain_fn_10001700_guess((void *)exitItem);
            eqmain_fn_10008a00_guess(nameEdit);
            eqmain_fn_10008a00_guess(pinEdit);
            eqmain_fn_10007a80_guess((int *)g_eqmain_blit_src_guess);
            eqmain_fn_100039e0_arg_guess(0);
            g_eqmain_pin_flag_guess = 0xffffffff;
            return result;
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x1001AA20
// Form on EQKlogin.bmp with two blank entries sent together to 0x1001A120 (likely a new PIN and its repeat)
int __stdcall EQMain_NewPinDialog_guess(void)
{
    char ch;
    unsigned char textByte;
    int pHVar3;
    bool submitted;
    bool finished;
    unsigned short result;
    int pHVar7;
    int pHVar8;
    int pHVar9;
    int pHVar10;
    int pHVar11;
    int tmp;
    int *_Memory;
    unsigned char *firstText;
    unsigned char *ptr;
    unsigned int confirmed;
    unsigned int length;
    unsigned int dwordCount;
    unsigned int clearResult;
    char *src;
    char *dst;

    finished = false;
    submitted = false;
    result = 7;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, g_eqmain_s_eqklogin_bmp_guess, 0x10);
    pHVar7 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x14a, 4, 2);
    pHVar8 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x118, 4, 2);
    eqmain_fn_100017b0_guess(pHVar8, 0, g_eqmain_text_5834_guess, 1, 0);
    eqmain_fn_100017b0_guess(pHVar7, 0, g_eqmain_text_5848_guess, 1, 0);
    pHVar9 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x17c, 4, 2);
    eqmain_fn_100017b0_guess(pHVar9, 0, g_eqmain_text_5844_guess, 1, 0);
    pHVar10 = (int)eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x127, 0x104, 0x127, 0x1a, 0x14, 0, 1, 0x22d69, 1);
    pHVar11 = (int)eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x127, 0x16d, 0x127, 0x1a, 0x11, 0, 1, 0x22d69, 1);
    eqmain_fn_100088f0_guess(pHVar11);
    eqmain_fn_100088f0_guess(pHVar10);
    eqmain_fn_10008910_guess((void *)pHVar10);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x186, 0xf0, 0x21a3c, g_eqmain_s_new_pin_limit_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 400, 0x159, 0x21a3c, g_eqmain_s_new_pin_again_guess, 0, 0);
    g_eqmain_blit_src_guess = (int)eqmain_fn_10006d80_guess(300, 0x14, 0x10);
    eqmain_fn_10006c20_guess((int)g_eqmain_blit_src_guess, 0, 0, g_eqmain_ceb8_guess, 0x136, 400, 300, 0x14, 0);
    length = 0xffffffff;
    src = g_eqmain_drive_base_guess;
    do
    {
        dst = src;
        if (length == 0)
            break;
        length = length - 1;
        dst = src + 1;
        ch = *src;
        src = dst;
    } while (ch != '\0');
    length = ~length;
    g_eqmain_flag_ce78_guess = 1;
    src = dst - length;
    dst = g_eqmain_copy_4d20_guess;
    for (dwordCount = length >> 2; dwordCount != 0; dwordCount = dwordCount - 1)
    {
        *(unsigned int *)dst = *(unsigned int *)src;
        src = src + 4;
        dst = dst + 4;
    }
    for (length = length & 3; length != 0; length = length - 1)
    {
        *dst = *src;
        src = src + 1;
        dst = dst + 1;
    }
    eqmain_fn_10003a00_guess();
    g_eqmain_pin_flag_guess = 0x20;
    do
    {
        length = (unsigned short)eqmain_fn_1001a210_guess();
        tmp = (int)(short)length;
        if (tmp < 0x74)
        {
            if (tmp < 0x72)
            {
                if (tmp == 2)
                    goto resetSubmit;
                if (tmp == 0x12)
                {
                    result = 3;
                    finished = true;
                }
            }
            else
            {
                result = 7;
                finished = true;
            }
        }
        else if (tmp == 0x10000003)
        {
resetSubmit:
            submitted = false;
            eqmain_fn_1001a310_guess();
        }
        _Memory = (int *)eqmain_fn_10003bd0_guess();
        if (_Memory != 0)
        {
            switch (*_Memory)
            {
            case 0:
                pHVar3 = _Memory[2];
                if (pHVar3 == pHVar10)
                {
                    eqmain_fn_10008910_guess((void *)pHVar11);
                }
                else if (pHVar3 == pHVar9)
                {
                    result = 0x16;
                    finished = true;
                }
                else if (pHVar3 == pHVar7)
                {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
                else
                {
                    firstText = *(unsigned char **)(pHVar10 + 0x20);
                    tmp = -1;
                    ptr = firstText;
                    do
                    {
                        if (tmp == 0)
                            break;
                        tmp = tmp + -1;
                        textByte = *ptr;
                        ptr = ptr + 1;
                    } while (textByte != 0);
                    if (tmp != -2)
                    {
                        tmp = -1;
                        src = *(char **)(pHVar11 + 0x20);
                        do
                        {
                            if (tmp == 0)
                                break;
                            tmp = tmp + -1;
                            ch = *src;
                            src = src + 1;
                        } while (ch != '\0');
                        if (tmp != -2)
                        {
                            if ((pHVar3 != pHVar11) || (submitted))
                            {
                                if ((pHVar3 == pHVar8) && (!submitted))
                                {
                                    confirmed = eqmain_fn_1001a120_newpin_guess(firstText, (unsigned char *)*(char **)(pHVar11 + 0x20));
                                    if ((char)confirmed == '\0')
                                    {
                                        eqmain_fn_1001a310_guess();
                                    }
                                    else
                                    {
                                        submitted = true;
                                    }
                                }
                            }
                            else
                            {
                                firstText = (unsigned char *)eqmain_fn_1003396b_guess((char *)firstText);
                                ptr = (unsigned char *)eqmain_fn_1003396b_guess(*(char **)(pHVar11 + 0x20));
                                confirmed = eqmain_fn_1001a120_newpin_guess(firstText, ptr);
                                if ((char)confirmed == '\0')
                                {
                                    eqmain_fn_1001a310_guess();
                                }
                                else
                                {
                                    submitted = true;
                                }
                                eqmain_fn_10035e41_newpin_guess(firstText);
                                eqmain_fn_10035e41_newpin_guess(ptr);
                            }
                        }
                    }
                }
                break;
            case 4:
                if (*(char *)(_Memory + 2) == '\x1b')
                {
                    eqmain_fn_10003a00_guess();
                    g_eqmain_flag_cec4_guess = 0;
                    finished = true;
                }
                break;
            case 6:
            case 7:
                if (_Memory[2] == pHVar10)
                {
                    eqmain_fn_10008910_guess((void *)pHVar11);
                }
                else if (_Memory[2] == pHVar11)
                {
                    eqmain_fn_10008910_guess((void *)pHVar10);
                }
                break;
            }
            eqmain_free_guess(_Memory);
        }
        if (finished)
        {
            g_eqmain_flag_cec4_guess = 0;
            g_eqmain_flag_ce78_guess = 0;
            eqmain_fn_10001700_guess((void *)pHVar8);
            eqmain_fn_10001700_guess((void *)pHVar7);
            eqmain_fn_10001700_guess((void *)pHVar9);
            eqmain_fn_10008a00_guess((void *)pHVar10);
            eqmain_fn_10008a00_guess((void *)pHVar11);
            eqmain_fn_10007a80_guess((int *)g_eqmain_blit_src_guess);
            clearResult = eqmain_fn_100039e0_newpin_guess(0);
            g_eqmain_pin_flag_guess = 0xffffffff;
            return (int)((clearResult & 0xffff0000) | result);
        }
    } while (true);
}

// FUNCTION: EQMAIN 0x1001AE60
// Handles start-screen choices and the Next Code entry.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns a 16-bit code in AX (the high word is what EAX held
// after ClearBlinkAndPump_guess). The start screen: draws the EQKlogin.bmp strip, three items and the "..." text, copies
// g_eqmain_drive_base_guess to g_eqmain_copy_4d20_guess (strlen + 1 bytes), then loops over the keys of EQMain_ReportCode_guess and the messages of
// PumpMessagesAndEditText_guess until one of them sets shouldExit. Each message is freed at the end of its pass; resultCode is the code returned.
short __stdcall EQMain_LoginCodeScreen_guess(void)
{
    bool shouldExit = false;
    bool requestSent = false;
    int resultCode = 7;
    int pHVar2;
    int pHVar6;
    int pHVar7;
    int pHVar8;
    int pHVar9;
    int keyCode;
    int *_Memory;
    char *nextCodeText;
    unsigned int copyLength;

    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, g_eqmain_s_eqklogin_bmp_guess, 0x10);
    pHVar6 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x14a, 4, 2);
    pHVar7 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x118, 4, 2);
    eqmain_fn_100017b0_guess(pHVar7, 0, g_eqmain_text_5834_guess, 1, 0);
    eqmain_fn_100017b0_guess(pHVar6, 0, g_eqmain_text_5848_guess, 1, 0);
    pHVar8 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0x17c, 4, 2);
    eqmain_fn_100017b0_guess(pHVar8, 0, g_eqmain_text_5844_guess, 1, 0);
    pHVar9 = (int)eqmain_fn_10008460_guess(g_eqmain_ceb8_guess, 0x127, 0x16d, 0x127, 0x1a, 0x11, 0, 1, 0x22d69, 1);
    eqmain_fn_100088f0_guess(pHVar9);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x19a, 0xf0, 0x21a3c, g_eqmain_drive_base_guess, 0, 0);
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x190, 0x159, 0x21a3c, g_eqmain_s_next_code_guess, 0, 0);
    g_eqmain_blit_src_guess = (int)eqmain_fn_10006d80_guess(0x12c, 0x14, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_blit_src_guess, 0, 0, g_eqmain_ceb8_guess, 0x136, 0x190, 0x12c, 0x14, 0);
    copyLength = strlen(g_eqmain_drive_base_guess) + 1;
    g_eqmain_flag_ce78_guess = 1;
    memcpy(g_eqmain_copy_4d20_guess, g_eqmain_drive_base_guess, copyLength);
    eqmain_fn_10003a00_guess();
    g_eqmain_pin_flag_guess = 0x21;
    for (;;)
    {
        keyCode = eqmain_fn_1001a210_guess();
        switch (keyCode)
        {
        case 0x12:
            resultCode = 3;
            shouldExit = true;
            break;
        case 0x70:
        case 0x71:
            resultCode = 7;
            shouldExit = true;
            break;
        case 2:
        case 0x10000003:
            requestSent = false;
            eqmain_fn_1001a310_guess();
            break;
        }
        _Memory = (int *)eqmain_fn_10003bd0_guess();
        if (_Memory != 0)
        {
            if (*_Memory == 0)
            {
                pHVar2 = _Memory[2];
                if (pHVar2 == pHVar8)
                {
                    resultCode = 0x16;
                    shouldExit = true;
                }
                else if (pHVar2 == pHVar6)
                {
                    eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                }
                else
                {
                    nextCodeText = *(char **)(pHVar9 + 0x20);
                    if (strlen(nextCodeText) != 0)
                    {
                        if (pHVar2 != pHVar9 || requestSent)
                        {
                            if (pHVar2 == pHVar7 && !requestSent)
                            {
                                if (eqmain_fn_1001a1d0_guess(nextCodeText) == 0)
                                {
                                    goto redrawStrip;
                                }
                                requestSent = true;
                            }
                        }
                        else
                        {
                            nextCodeText = eqmain_fn_1003396b_guess(nextCodeText);
                            if (eqmain_fn_1001a1d0_guess(nextCodeText) == 0)
                            {
redrawStrip:
                                eqmain_fn_1001a310_guess();
                            }
                            else
                            {
                                requestSent = true;
                            }
                        }
                    }
                }
            }
            else if (*_Memory == 4 && (char)_Memory[2] == 0x1b)
            {
                eqmain_fn_10003a00_guess();
                g_eqmain_flag_cec4_guess = 0;
                shouldExit = true;
            }
            eqmain_fn_10033568_guess(_Memory);
        }
        if (shouldExit)
        {
            g_eqmain_flag_cec4_guess = 0;
            g_eqmain_flag_ce78_guess = 0;
            eqmain_fn_10001700_guess((void *)pHVar7);
            eqmain_fn_10001700_guess((void *)pHVar6);
            eqmain_fn_10001700_guess((void *)pHVar8);
            eqmain_fn_10008a00_guess((void *)pHVar9);
            eqmain_fn_10007a80_guess((int *)g_eqmain_blit_src_guess);
            eqmain_fn_100039e0_arg_guess(0);
            g_eqmain_pin_flag_guess = -1;
            return (short)resultCode;
        }
    }
}

#define g_eqmain_flag_e4_guess  EQ_GLOBAL(unsigned int, 0x100555E4)    // g_eqmain_flag_e4_guess: a dword
#define g_eqmain_flag_e0_guess  EQ_GLOBAL(unsigned char, 0x100555E0)   // g_eqmain_flag_e0_guess: a byte

// FUNCTION: EQMAIN 0x1001B1B0
// Resets two front-end state values; their player-facing purpose is unclear.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Zeroes the dword g_eqmain_flag_e4_guess, then the
// byte g_eqmain_flag_e0_guess (the original XORs EAX to 0 first and stores EAX, then AL: both stores write zero).
void __stdcall EQMain_ClearFlags_guess(void)
{
    g_eqmain_flag_e4_guess = 0;
    g_eqmain_flag_e0_guess = 0;
}

#define eqmain_fn_100348c5_guess   EQ_CDECL(int, 0x100348C5, (void (__cdecl *)(void)))   // _atexit: CRT, __cdecl, 0 on success
#define eqmain_fn_1001b1d0_guess   EQ_CDECL(void, 0x1001B1D0, (void))   // EQMain_CallFn100280a0_guess: the atexit callback (EQMain_CallFn100280a0_guess)

// FUNCTION: EQMAIN 0x1001B1C0
// Schedules exit-time handling for front-end state at 0x100555E0; the exact effect is unclear.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Registers EQMain_CallFn100280a0_guess with the CRT's
// _atexit (__cdecl, one pointer argument). The int result is ignored; the original's POP ECX removes the pushed pointer.
void __stdcall EQMain_RegisterAtexit_guess(void)
{
    eqmain_fn_100348c5_guess(eqmain_fn_1001b1d0_guess);
}

// FUNCTION: EQMAIN 0x1001B1D0
// Thunk forwarding 0x100555E0 to a helper; its front-end effect is unclear.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Tail-jumps to ReleaseHeldAndGlobal_guess with ECX =
// 0x100555E0 (the address of g_eqmain_flag_e0_guess, not its contents). The callee is __fastcall and takes no stack arguments,
// so the jump leaves the stack as it found it. The callee's EAX is left as it is.
void __stdcall EQMain_CallFn100280a0_guess(void)
{
    eqmain_fn_100280a0_guess(0x100555E0)();
}

// FUNCTION: EQMAIN 0x1001B1F0
// Thunk forwarding 0x100556E8 to a helper; its front-end role is unclear.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Tail-jumps to LoadEqTextRecordTable_guess with ECX =
// 0x100556E8 (the address of DAT_100556e8, not its contents). The callee is __fastcall and takes no stack arguments,
// so the jump leaves the stack as it found it. The callee's EAX is left as it is.
void __stdcall EQMain_CallFn10026080_guess(void)
{
    eqmain_fn_10026080_guess((int *)0x100556E8)();
}

// FUNCTION: EQMAIN 0x1001B200
// Registers a second process-exit callback at 0x1001B210; its effect is unclear.
// __stdcall with no stack arguments (its RET pops 0 bytes); returns nothing. Registers the exit routine at 0x1001B210
// with the CRT through _atexit (__cdecl, one pointer argument). The POP ECX after the call clears the argument and
// leaves ECX written; _atexit's result is left in EAX as it is.
void __stdcall EQMain_RegisterExit_guess(void)
{
    EQ_CDECL(int, 0x100348C5, (void *))((void *)0x1001B210);
}

// FUNCTION: EQMAIN 0x1001B220
// Releases the front-end display objects and destroys the window when requested.
// __cdecl (RET pops 0 bytes); returns nothing; the char argument is at [ESP+4]. Releases the object in g_eqmain_com_a_guess;
// if g_eqmain_com_b_guess is set, calls its slot 0x50 with (object, g_eqmain_hwnd_guess, 8) and releases it; calls ReleaseHeldAndGlobal_guess on
// 0x100555E0; when the argument is nonzero and g_eqmain_hwnd_guess is set, destroys that window through the IAT slot.
void __cdecl EQMain_ReleaseDisplay_guess(char destroy_window)
{
    void *o1;

    o1 = g_eqmain_com_a_guess;
    if (o1 != 0)
    {
        ((EqMainComReleaseFn_guess)(*(void ***)o1)[2])(o1);
    }

    o1 = g_eqmain_com_b_guess;
    if (o1 != 0)
    {
        ((EqMainComSetFn_guess)(*(void ***)o1)[0x14])(o1, g_eqmain_hwnd_guess, 8);
        o1 = g_eqmain_com_b_guess;
        ((EqMainComReleaseFn_guess)(*(void ***)o1)[2])(o1);
    }

    eqmain_fn_100280a0_guess(0x100555E0)();

    if (destroy_window != 0 && g_eqmain_hwnd_guess != 0)
    {
        g_eqmain_destroywin_guess(g_eqmain_hwnd_guess);
    }
}

// FUNCTION: EQMAIN 0x1001B280
// Shuts down front-end art archives and UI resources, reporting any stored error before exit.
// __cdecl (RET pops 0 bytes, so the caller pops the char at [ESP+4]); returns nothing. The header says __thiscall, but the
// callee pops nothing, and its ECX goes only to CleanupSetup_guess (0x100216F0), which ignores it, so 0 is passed.
// Sets the window procedure of g_eqmain_hwnd_guess back to DefWindowProcA when the window is set, calls CleanupSetup_guess and
// DestroyUiContext_guess, then unloads the six eqfeart paks in g_eqmain_pak1_guess..g_eqmain_pak6_guess (each: build the name, removeFileSystem
// with it by value, delete the PackFS), calls EQMain_ReleaseDisplay_guess (0x1001B220), and when the text at g_eqmain_errtext_guess
// is not empty beeps, shows it in a message box and calls _exit(0). DestroyGlobalFontManager_guess is called last. The exception frame is
// not written: its destructible locals (PackFsStringReference_guess) have no destructor in include/.
void __cdecl EQMain_ShutdownArtPaks_guess(char flag)
{
    if (g_eqmain_hwnd_guess != 0)
    {
        g_eqmain_setwindowlong_guess(g_eqmain_hwnd_guess, -4, g_eqmain_defwndproc_guess);
    }
    eqmain_fn_100216f0_guess(0)();
    eqmain_fn_10003930_guess();

    if (g_eqmain_pak1_guess != 0)
    {
        EqMainPakRef_guess arg;
        EqMainPakRef_guess name;
        EqMainPakRef_guess path;
        void *p;

        eqmain_fn_10007540_guess(&arg)( 0);
        eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart1_guess);
        p = eqmain_fn_10007870_guess(&path)( &name);
        eqmain_fn_10007620_guess(&arg)( p);
        if (path.shared != 0)
        {
            eqmain_fn_10007520_guess(path.shared)();
        }
        if (name.shared != 0)
        {
            eqmain_fn_10007520_guess(name.shared)();
        }
        eqmain_fn_1000ecf0_guess(g_eqmain_pak1_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak1_guess);
    }

    if (g_eqmain_pak2_guess != 0)
    {
        EqMainPakRef_guess arg;
        EqMainPakRef_guess name;
        EqMainPakRef_guess path;
        void *p;

        eqmain_fn_10007540_guess(&arg)( 0);
        eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart2_guess);
        p = eqmain_fn_10007870_guess(&path)( &name);
        eqmain_fn_10007620_guess(&arg)( p);
        if (path.shared != 0)
        {
            eqmain_fn_10007520_guess(path.shared)();
        }
        if (name.shared != 0)
        {
            eqmain_fn_10007520_guess(name.shared)();
        }
        eqmain_fn_1000ecf0_guess(g_eqmain_pak2_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak2_guess);
    }

    if (g_eqmain_pak3_guess != 0)
    {
        EqMainPakRef_guess arg;
        EqMainPakRef_guess name;
        EqMainPakRef_guess path;
        void *p;

        eqmain_fn_10007540_guess(&arg)( 0);
        eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart3_guess);
        p = eqmain_fn_10007870_guess(&path)( &name);
        eqmain_fn_10007620_guess(&arg)( p);
        eqmain_fn_10010b50_guess(&path)();
        if (name.shared != 0)
        {
            eqmain_fn_10007520_guess(name.shared)();
        }
        eqmain_fn_1000ecf0_guess(g_eqmain_pak3_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak3_guess);
    }

    if (g_eqmain_pak4_guess != 0)
    {
        EqMainPakRef_guess arg;
        EqMainPakRef_guess name;
        EqMainPakRef_guess path;
        void *p;

        eqmain_fn_10007540_guess(&arg)( 0);
        eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart4_guess);
        p = eqmain_fn_10007870_guess(&path)( &name);
        eqmain_fn_10007620_guess(&arg)( p);
        eqmain_fn_10010b50_guess(&path)();
        if (name.shared != 0)
        {
            eqmain_fn_10007520_guess(name.shared)();
        }
        eqmain_fn_1000ecf0_guess(g_eqmain_pak4_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak4_guess);
    }

    if (g_eqmain_pak5_guess != 0)
    {
        EqMainPakRef_guess arg;
        EqMainPakRef_guess name;
        EqMainPakRef_guess path;
        void *p;

        eqmain_fn_10007540_guess(&arg)( 0);
        eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart5_guess);
        p = eqmain_fn_10007870_guess(&path)( &name);
        eqmain_fn_10007620_guess(&arg)( p);
        eqmain_fn_10010b50_guess(&path)();
        eqmain_fn_10010b50_guess(&name)();
        eqmain_fn_1000ecf0_guess(g_eqmain_pak5_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak5_guess);
    }

    if (g_eqmain_pak6_guess != 0)
    {
        EqMainPakRef_guess arg;

        eqmain_fn_10007650_guess(&arg)( g_eqmain_s_eqfeart6_guess);
        eqmain_fn_1000ecf0_guess(g_eqmain_pak6_guess)( arg);
        eqmain_fn_10033800_guess(g_eqmain_pak6_guess);
    }

    eqmain_fn_1001b220_guess(flag);
    eqmain_fn_10020ef0_guess(0);
    if (strlen(g_eqmain_errtext_guess) != 0)
    {
        g_eqmain_messagebeep_guess(0x10);
        g_eqmain_messagebox_guess(0, g_eqmain_errtext_guess, g_fatal_text_prefix_guess, 0x10);
        eqmain_fn_10035e98_guess(0);
    }
    eqmain_fn_10027b20_guess();
}

// Callees, globals and IAT/COM stand-ins of EQMain_LoadArtPaks_guess that the lines above don't have.
#define eqmain_fn_100338e5_guess    EQ_CDECL(void *, 0x100338E5, (unsigned int))                      // operator_new: cdecl, one size argument
#define eqmain_fn_1000e890_guess(object) EQ_METHOD(0, void *, 0x1000E890, (void *), (object))                         // FfSoundLock ctor: fastcall, this in ECX, returns this
#define eqmain_fn_1000e8c0_guess(object) EQ_METHOD(1, char, 0x1000E8C0, (void *, EqMainPakRef_guess), (object))  // FfSoundLock add-pak: this in ECX, the 8-byte name by value (RET 8), returns a bool
#define eqmain_fn_10007f70_guess(object) EQ_METHOD(1, void *, 0x10007F70, (void *, unsigned int), (object))      // PackFsSharedPtr ctor from a size: thiscall, returns this
#define eqmain_fn_10007fe0_guess(object) EQ_METHOD(1, void *, 0x10007FE0, (void *, void *), (object))            // updateSharedPointer: thiscall, the shared pointer by value
#define eqmain_fn_10007850_guess(object) EQ_METHOD(1, void *, 0x10007850, (void *, void *), (object))            // PackFsStringReferenceCopier_guess::copyStringReferenceAndReturn_guess: thiscall, one pointer
#define eqmain_fn_10003340_guess    EQ_STDCALL(unsigned int, 0x10003340, (void))                      // eqmainProbeDirectX_guess: stdcall, no arguments, returns an unsigned value
#define eqmain_fn_10027ea0_guess(object) EQ_METHOD(2, int, 0x10027EA0, (void *, int, int), (object))             // InitKeyboardDevice_guess: thiscall (this = 0x100555E0), two ints, returns an int
#define eqmain_fn_10027aa0_guess    EQ_STDCALL(int, 0x10027AA0, (void))                               // CreateGlobalFontManager_guess: stdcall, no arguments, returns an int
#define eqmain_fn_10032314_guess    EQ_STDCALL(int, 0x10032314, (void *, void *, void *))             // DirectDrawCreate thunk: stdcall, three pointers, returns an HRESULT
#define eqmain_fn_10034117_guess    EQ_CDECL(int, 0x10034117, (char *, char *, ...))                  // _sprintf: cdecl, varargs
#define eqmain_vtbl_slot_guess(obj, off)  (((void **)*(void **)(obj))[(off) / 4])                     // the vtable slot at byte offset off of a COM object
typedef int (__stdcall *EqMainLoadIconFn_guess)(int, int);                                            // LoadIconA (IAT)
typedef int (__stdcall *EqMainGetStockObjectFn_guess)(int);                                           // GetStockObject (IAT)
typedef int (__stdcall *EqMainRegisterClassFn_guess)(void *);                                         // RegisterClassA (IAT)
typedef int (__stdcall *EqMainCreateWindowExFn_guess)(int, char *, char *, int, int, int, int, int, int, int, int, int);   // CreateWindowExA (IAT)
typedef int (__stdcall *EqMainShowCursorFn_guess)(int);                                               // ShowCursor (IAT)
typedef int (__stdcall *EqMainSetFocusFn_guess)(int);                                                 // SetFocus (IAT)
typedef int (__stdcall *EqMainSetCaptureFn_guess)(int);                                               // SetCapture (IAT)
typedef int (__stdcall *EqMainSetWindowPosFn_guess)(int, int, int, int, int, int, int);               // SetWindowPos (IAT)
typedef long (__stdcall *EqMainComSetDisplayFn_guess)(void *, int, int, int);                         // slot 0x54: SetDisplayMode (this, width, height, bpp)
typedef long (__stdcall *EqMainComCreateSurfaceFn_guess)(void *, void *, void *, void *);             // slot 0x18: CreateSurface (this, desc, out, outer)
#define g_eqmain_fmt_5a10_guess     EQ_GLOBAL(char *, 0x10055A10)   // g_eqmain_fmt_5a10_guess: the text copied when eqmainProbeDirectX_guess reports less than 0x600
#define g_eqmain_fmt_5a18_guess     EQ_GLOBAL(char *, 0x10055A18)   // g_eqmain_fmt_5a18_guess: the sprintf format of the pak errors (the pak name is the argument)
#define g_eqmain_fmt_5a1c_guess     EQ_GLOBAL(char *, 0x10055A1C)   // g_eqmain_fmt_5a1c_guess: the text copied when CreateWindowExA fails
#define g_eqmain_fmt_5a20_guess     EQ_GLOBAL(char *, 0x10055A20)   // g_eqmain_fmt_5a20_guess: the sprintf format of InitKeyboardDevice_guess's nonzero result
#define g_eqmain_fmt_5a24_guess     EQ_GLOBAL(char *, 0x10055A24)   // g_eqmain_fmt_5a24_guess: the sprintf format of a failed DirectDrawCreate
#define g_eqmain_fmt_5a28_guess     EQ_GLOBAL(char *, 0x10055A28)   // g_eqmain_fmt_5a28_guess: the sprintf format of a failed CreateSurface
#define g_eqmain_topwnd_guess       EQ_GLOBAL(int, 0x100D5B30)      // g_eqmain_topwnd_guess: a window to set topmost (0: none)
#define g_eqmain_ddguid_guess       EQ_GLOBAL(void *, 0x100D5B50)   // g_eqmain_ddguid_guess: the DirectDraw GUID pointer (lpGUID)
#define g_eqmain_hinst_guess        EQ_GLOBAL(int, 0x100D5D54)      // g_eqmain_hinst_guess: the module handle (HINSTANCE)
#define g_eqmain_status_guess       EQ_GLOBAL(int, 0x10055478)      // g_eqmain_status_guess: the last InitKeyboardDevice_guess / DirectDrawCreate / CreateSurface result
#define g_eqmain_dd_result_guess    EQ_GLOBAL(int, 0x1004CEC0)      // g_eqmain_dd_result_guess: CreateGlobalFontManager_guess's result, stored when the surface is created
#define g_eqmain_ddsd_size_guess    EQ_GLOBAL(int, 0x10055570)      // DDSURFACEDESC at g_eqmain_ddsd_size_guess: dwSize
#define g_eqmain_ddsd_flags_guess   EQ_GLOBAL(int, 0x10055574)      // DDSURFACEDESC: dwFlags
#define g_eqmain_ddsd_backbuf_guess EQ_GLOBAL(int, 0x10055584)      // DDSURFACEDESC: dwBackBufferCount
#define g_eqmain_ddsd_caps_guess    EQ_GLOBAL(int, 0x100555D8)      // DDSURFACEDESC: ddsCaps.dwCaps
#define g_eqmain_wc_style_guess     EQ_GLOBAL(int, 0x10055488)      // WNDCLASSA at g_eqmain_wc_style_guess: style
#define g_eqmain_wc_proc_guess      EQ_GLOBAL(int, 0x1005548C)      // WNDCLASSA: lpfnWndProc
#define g_eqmain_wc_clsextra_guess  EQ_GLOBAL(int, 0x10055490)      // WNDCLASSA: cbClsExtra
#define g_eqmain_wc_wndextra_guess  EQ_GLOBAL(int, 0x10055494)      // WNDCLASSA: cbWndExtra
#define g_eqmain_wc_hinst_guess     EQ_GLOBAL(int, 0x10055498)      // WNDCLASSA: hInstance
#define g_eqmain_wc_icon_guess      EQ_GLOBAL(int, 0x1005549C)      // WNDCLASSA: hIcon
#define g_eqmain_wc_cursor_guess    EQ_GLOBAL(int, 0x100554A0)      // WNDCLASSA: hCursor
#define g_eqmain_wc_brush_guess     EQ_GLOBAL(int, 0x100554A4)      // WNDCLASSA: hbrBackground
#define g_eqmain_wc_menu_guess      EQ_GLOBAL(int, 0x100554A8)      // WNDCLASSA: lpszMenuName
#define g_eqmain_wc_classname_guess EQ_GLOBAL(char *, 0x100554AC)   // WNDCLASSA: lpszClassName
#define g_eqmain_s_gifeclass_guess  EQ_ARRAY(char, 0x10048158)      // s_gifeclass: the window class name
#define g_eqmain_s_everquest_guess  EQ_ARRAY(char, 0x10047FC4)      // s_EverQuest: the window title
#define g_eqmain_loadicon_guess     EQ_GLOBAL(EqMainLoadIconFn_guess, 0x10040220)              // PTR_LoadIconA: IAT slot
#define g_eqmain_getstockobject_guess EQ_GLOBAL(EqMainGetStockObjectFn_guess, 0x10040030)     // PTR_GetStockObject: IAT slot
#define g_eqmain_registerclass_guess EQ_GLOBAL(EqMainRegisterClassFn_guess, 0x10040218)       // PTR_RegisterClassA: IAT slot
#define g_eqmain_createwindowex_guess EQ_GLOBAL(EqMainCreateWindowExFn_guess, 0x10040228)     // PTR_CreateWindowExA: IAT slot
#define g_eqmain_showcursor_guess   EQ_GLOBAL(EqMainShowCursorFn_guess, 0x1004022C)            // PTR_ShowCursor: IAT slot
#define g_eqmain_setfocus_guess     EQ_GLOBAL(EqMainSetFocusFn_guess, 0x10040224)              // PTR_SetFocus: IAT slot
#define g_eqmain_setcapture_guess   EQ_GLOBAL(EqMainSetCaptureFn_guess, 0x10040234)            // PTR_SetCapture: IAT slot
#define g_eqmain_setwindowpos_guess EQ_GLOBAL(EqMainSetWindowPosFn_guess, 0x1004023C)          // PTR_SetWindowPos: IAT slot

// FUNCTION: EQMAIN 0x1001B650
// Front-end startup: mounts eqfeart1-6.pak, optionally makes the game window, sets DirectDraw to 640x480x16
// __cdecl, one char argument (the caller pops it). Loads the six eqfeart paks: each pak gets a FfSoundLock object (1 byte,
// its constructor returns it) in g_eqmain_pak1_guess..4c, and its name is passed by value (8 bytes) to EQMain_PackFsLoadIndex_guess (add pak).
// A failed add copies a message into g_eqmain_errtext_guess and returns 0. Then a nonzero argument checks eqmainProbeDirectX_guess (at least
// 0x600) and creates the window class and window; every path then shows the cursor, focuses and captures the window,
// calls InitKeyboardDevice_guess, creates DirectDraw and its surface, and returns 1 (0 after copying a message on any failure).
// Missing EH frame: the locals with destructors (PackFsStringReference) have no destructor in include/.
char EQMain_LoadArtPaks_guess(char createWindow)
{
    EqMainPakRef_guess arg;     // the 8-byte argument of EQMain_PackFsLoadIndex_guess (the SUB ESP,8 of each pak block)
    EqMainPakRef_guess name;    // local_98: the pak name (paks 1-5)
    EqMainPakRef_guess path;    // local_90 / local_88: the holder of the new buffer, then of the name copy (paks 1-3)
    EqMainPakRef_guess path2;   // local_80: the name object of paks 4-6
    EqMainPakRef_guess path3;   // local_78: the holder of the copy of path2 (pak 6)
    void *tmp;                  // temp_B: the shared pointer of the new name buffer (paks 1-3)
    void *p;                    // the new FfSoundLock object
    void *copy;                 // the copy initializeStringReferenceWithNormalizedPath_guess / PackFsStringReference_guess::initializeStringReferenceWithString_guess returns
    char errbuf[100];           // local_70: the sprintf buffer
    char ok;
    int len;
    int status;
    int hwnd;

    // pak 1: eqfeart1.pak
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak1_guess = 0;
    }
    else
    {
        g_eqmain_pak1_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007f70_guess(&name)( 0);
    name.value = 0;
    len = strlen(g_eqmain_s_eqfeart1_guess);
    if (len != 0)
    {
        path.shared = &tmp;
        eqmain_fn_10007f70_guess(&tmp)( len + 1);
        eqmain_fn_10007fe0_guess(&name)( tmp);
        strcpy((char *)((void **)name.shared)[2], g_eqmain_s_eqfeart1_guess);
        name.value = 0;
    }
    copy = eqmain_fn_10007870_guess(&path)( &name);
    eqmain_fn_10007850_guess(&arg)( copy);
    eqmain_fn_10010b50_guess(&path)();
    eqmain_fn_10010b50_guess(&name)();
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak1_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart1_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    // pak 2: eqfeart2.pak
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak2_guess = 0;
    }
    else
    {
        g_eqmain_pak2_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007f70_guess(&name)( 0);
    name.value = 0;
    len = strlen(g_eqmain_s_eqfeart2_guess);
    if (len != 0)
    {
        path.shared = &tmp;
        eqmain_fn_10007f70_guess(&tmp)( len + 1);
        eqmain_fn_10007fe0_guess(&name)( tmp);
        strcpy((char *)((void **)name.shared)[2], g_eqmain_s_eqfeart2_guess);
        name.value = 0;
    }
    copy = eqmain_fn_10007870_guess(&path)( &name);
    eqmain_fn_10007850_guess(&arg)( copy);
    eqmain_fn_10010b50_guess(&path)();
    eqmain_fn_10010b50_guess(&name)();
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak2_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart2_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    // pak 3: eqfeart3.pak
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak3_guess = 0;
    }
    else
    {
        g_eqmain_pak3_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007f70_guess(&name)( 0);
    name.value = 0;
    len = strlen(g_eqmain_s_eqfeart3_guess);
    if (len != 0)
    {
        path.shared = &tmp;
        eqmain_fn_10007f70_guess(&tmp)( len + 1);
        eqmain_fn_10007fe0_guess(&name)( tmp);
        strcpy((char *)((void **)name.shared)[2], g_eqmain_s_eqfeart3_guess);
        name.value = 0;
    }
    copy = eqmain_fn_10007870_guess(&path)( &name);
    eqmain_fn_10007850_guess(&arg)( copy);
    eqmain_fn_10010b50_guess(&path)();
    eqmain_fn_10010b50_guess(&name)();
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak3_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart3_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    // pak 4: eqfeart4.pak (the name is built from the string by PackFsStringReference_guess::initializeStringReferenceWithString_guess)
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak4_guess = 0;
    }
    else
    {
        g_eqmain_pak4_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart4_guess);
    copy = eqmain_fn_10007870_guess(&path2)( &name);
    eqmain_fn_10007620_guess(&arg)( copy);
    if (path2.shared != 0)
    {
        eqmain_fn_10007520_guess(path2.shared)();
    }
    if (name.shared != 0)
    {
        eqmain_fn_10007520_guess(name.shared)();
    }
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak4_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart4_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    // pak 5: eqfeart5.pak
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak5_guess = 0;
    }
    else
    {
        g_eqmain_pak5_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007540_guess(&name)( g_eqmain_s_eqfeart5_guess);
    copy = eqmain_fn_10007870_guess(&path2)( &name);
    eqmain_fn_10007620_guess(&arg)( copy);
    eqmain_fn_10010b50_guess(&path2)();
    if (name.shared != 0)
    {
        eqmain_fn_10007520_guess(name.shared)();
    }
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak5_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart5_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    // pak 6: eqfeart6.pak
    p = eqmain_fn_100338e5_guess(1);
    if (p == 0)
    {
        g_eqmain_pak6_guess = 0;
    }
    else
    {
        g_eqmain_pak6_guess = eqmain_fn_1000e890_guess(p)();
    }
    eqmain_fn_10007f70_guess(&arg)( 0);
    arg.value = 0;
    eqmain_fn_10007540_guess(&path2)( g_eqmain_s_eqfeart6_guess);
    copy = eqmain_fn_10007870_guess(&path3)( &path2);
    eqmain_fn_10007850_guess(&arg)( copy);
    eqmain_fn_10010b50_guess(&path3)();
    eqmain_fn_10010b50_guess(&path2)();
    ok = eqmain_fn_1000e8c0_guess(g_eqmain_pak6_guess)( arg);
    if (ok == 0)
    {
        eqmain_fn_10034117_guess(errbuf, g_eqmain_fmt_5a18_guess, g_eqmain_s_eqfeart6_guess);
        strcpy(g_eqmain_errtext_guess, errbuf);
        return 0;
    }

    if (createWindow != 0)
    {
        if (eqmain_fn_10003340_guess() < 0x600)
        {
            strcpy(g_eqmain_errtext_guess, g_eqmain_fmt_5a10_guess);
            return 0;
        }
        g_eqmain_wc_style_guess = 0;
        g_eqmain_wc_proc_guess = g_eqmain_defwndproc_guess;
        g_eqmain_wc_clsextra_guess = 0;
        g_eqmain_wc_wndextra_guess = 0;
        g_eqmain_wc_hinst_guess = g_eqmain_hinst_guess;
        g_eqmain_wc_icon_guess = g_eqmain_loadicon_guess(0, 0x65);
        g_eqmain_wc_cursor_guess = 0;
        g_eqmain_wc_brush_guess = g_eqmain_getstockobject_guess(4);
        g_eqmain_wc_menu_guess = 0;
        g_eqmain_wc_classname_guess = g_eqmain_s_gifeclass_guess;
        g_eqmain_registerclass_guess(&g_eqmain_wc_style_guess);
        hwnd = g_eqmain_createwindowex_guess(0x40008, g_eqmain_s_gifeclass_guess, g_eqmain_s_everquest_guess,
                                             0x80000000, 0, 0, 0x280, 0x1E0, 0, 0, g_eqmain_hinst_guess, 0);
        g_eqmain_hwnd_guess = hwnd;
        if (hwnd == 0)
        {
            strcpy(g_eqmain_errtext_guess, g_eqmain_fmt_5a1c_guess);
            return 0;
        }
    }

    g_eqmain_showcursor_guess(0);
    g_eqmain_setfocus_guess(g_eqmain_hwnd_guess);
    g_eqmain_setcapture_guess(g_eqmain_hwnd_guess);
    if (g_eqmain_topwnd_guess != 0)
    {
        g_eqmain_setwindowpos_guess(g_eqmain_topwnd_guess, -2, 0, 0, 0, 0, 0xB);
    }
    g_eqmain_setwindowpos_guess(g_eqmain_hwnd_guess, 0, 0, 0, 0, 0, 0xB);
    status = eqmain_fn_10027ea0_guess((void *)0x100555E0)( g_eqmain_hinst_guess, g_eqmain_hwnd_guess);
    g_eqmain_status_guess = status;
    if (status != 0)
    {
        eqmain_fn_10034117_guess(g_eqmain_errtext_guess, g_eqmain_fmt_5a20_guess, status);
        return 0;
    }

    status = eqmain_fn_10032314_guess(g_eqmain_ddguid_guess, &g_eqmain_com_b_guess, 0);
    g_eqmain_status_guess = status;
    if (status != 0)
    {
        eqmain_fn_10034117_guess(g_eqmain_errtext_guess, g_eqmain_fmt_5a24_guess, status);
        return 0;
    }
    g_eqmain_setwindowlong_guess(g_eqmain_hwnd_guess, -16, 0x90000000);
    ((EqMainComSetFn_guess)eqmain_vtbl_slot_guess(g_eqmain_com_b_guess, 0x50))(g_eqmain_com_b_guess, g_eqmain_hwnd_guess, 0x57);
    ((EqMainComSetDisplayFn_guess)eqmain_vtbl_slot_guess(g_eqmain_com_b_guess, 0x54))(g_eqmain_com_b_guess, 0x280, 0x1E0, 0x10);
    g_eqmain_ddsd_size_guess = 0x6C;
    g_eqmain_ddsd_caps_guess = 0xA18;
    g_eqmain_ddsd_flags_guess = 0x21;
    g_eqmain_ddsd_backbuf_guess = 1;
    status = ((EqMainComCreateSurfaceFn_guess)eqmain_vtbl_slot_guess(g_eqmain_com_b_guess, 0x18))(g_eqmain_com_b_guess, &g_eqmain_ddsd_size_guess, &g_eqmain_com_a_guess, 0);
    g_eqmain_status_guess = status;
    if (status == 0)
    {
        g_eqmain_dd_result_guess = eqmain_fn_10027aa0_guess();
        return 1;
    }
    eqmain_fn_10034117_guess(g_eqmain_errtext_guess, g_eqmain_fmt_5a28_guess, status);
    return 0;
}

// FUNCTION: EQMAIN 0x1001C030
// Front-end main: takes the exe's login/server block, runs splashes and the screen loop, writes results back
int __cdecl EQMain_RunFrontEnd_guess(int hInstance, int exeInfo)
{
    char ch;
    bool ok;
    char *ptr;
    int result;
    unsigned int unusedA;
    unsigned int unusedB;
    int scanCount;
    void *ecxObj;
    char name_a[32];
    char name_b[32];
    char name_c[32];
    char name_d[32];
    unsigned int state;

    EQ_CDECL(void, 0x10027940, (char *, ...))(
        (char *)EQ_ARRAY(char, 0x1004827C));
    g_eqmain_hinst_guess = hInstance;
    EQ_CDECL(void, 0x100312C0, (unsigned int *))(
        (unsigned int *)EQ_ARRAY(char, 0x10048268));
    strcpy(g_eqmain_login_name_buf_guess, g_eqmain_drive_base_guess);
    strcpy(g_eqmain_login_pin_buf_guess, g_eqmain_drive_base_guess);
    EQ_CDECL(void, 0x10027940, (char *, ...))(
        (char *)EQ_ARRAY(char, 0x10048238),
        *(int *)(exeInfo + 0x400), *(int *)(exeInfo + 0x404));
    EQ_CDECL(void, 0x10027940, (char *, ...))(
        (char *)EQ_ARRAY(char, 0x100481FC),
        (char *)(exeInfo + 0x40C), (char *)(exeInfo + 0x48C),
        (char *)(exeInfo + 0x50C), *(int *)(exeInfo + 0x408));
    EQ_CDECL(void, 0x10027940, (char *, ...))(
        (char *)EQ_ARRAY(char, 0x100481E0),
        (char *)(exeInfo + 0x58C), (char *)(exeInfo + 0x690));
    EQ_CDECL(void, 0x10027940, (char *, ...))(
        (char *)EQ_ARRAY(char, 0x100481D4), (char *)(exeInfo + 0x6D0));
    *(int *)0x10055474 = exeInfo;
    g_eqmain_topwnd_guess = *(int *)(exeInfo + 0x404);
    EQ_CDECL(void, 0x100184D0, (int))(exeInfo);
    EQ_STDCALL(void, 0x10018560, (void))();
    ptr = EQ_CDECL(char *, 0x10021110, (unsigned char *))(
        (unsigned char *)(exeInfo + 0x6B0));
    strcpy(g_eqmain_login_name_buf_guess, ptr);
    ptr = EQ_CDECL(char *, 0x10021110, (unsigned char *))(
        (unsigned char *)(exeInfo + 0x50C));
    strcpy(g_eqmain_login_pin_buf_guess, ptr);
    ptr = EQ_CDECL(char *, 0x10021110, (unsigned char *))(
        (unsigned char *)(exeInfo + 0x58C));
    strcpy(EQ_ARRAY(char, 0x100554B0), ptr);
    ptr = EQ_CDECL(char *, 0x10021110, (unsigned char *))(
        (unsigned char *)(exeInfo + 0x6D0));
    strcpy(EQ_ARRAY(char, 0x10055530), ptr);

    scanCount = -1;
    ptr = g_eqmain_login_name_buf_guess;
    do
    {
        if (scanCount == 0)
            break;
        scanCount = scanCount - 1;
        ch = *ptr;
        ptr = ptr + 1;
    } while (ch != '\0');
    if (scanCount == -2)
    {
        goto runDialog;
    }
    scanCount = -1;
    ptr = g_eqmain_login_pin_buf_guess;
    do
    {
        if (scanCount == 0)
            break;
        scanCount = scanCount - 1;
        ch = *ptr;
        ptr = ptr + 1;
    } while (ch != '\0');
    if (scanCount == -2)
    {
runDialog:
        if (g_eqmain_topwnd_guess != 0)
        {
            g_eqmain_setwindowpos_guess(g_eqmain_topwnd_guess, -2, 0, 0, 0, 0, 0xB);
        }
        ok = EQ_CDECL(char, 0x1001D5E0, (int))(hInstance) != 0;
        if (!ok)
        {
            __asm mov ecxObj, ecx
            EQ_METHOD(0, void, 0x100216F0, (void *), ecxObj)();
            *(int *)(exeInfo + 0x68C) = -1;
            eqmain_fn_10035e98_guess(0);
        }
        if (g_eqmain_topwnd_guess != 0)
        {
            g_eqmain_setwindowpos_guess(g_eqmain_topwnd_guess, 0, 0, 0, 0, 0, 0xB);
        }
    }

    result = EQ_CDECL(char, 0x1001B650, (char))(
        EQ_GLOBAL(char, 0x10048154));
    if ((char)result == '\0')
    {
        EQ_CDECL(void, 0x1001B280, (char))(
            EQ_GLOBAL(char, 0x10048154));
        *(int *)(exeInfo + 0x68C) = -1;
        eqmain_fn_10035e98_guess(0);
    }
    EQ_STDCALL(void, 0x10024380, (void))();
    EQ_CDECL(void, 0x100036A0, (int, int, int))(
        g_eqmain_hwnd_guess, (int)g_eqmain_com_b_guess, (int)g_eqmain_com_a_guess);
    g_eqmain_setwindowlong_guess(g_eqmain_hwnd_guess, -4, 0x100044F0);

    scanCount = -1;
    ptr = g_eqmain_login_name_buf_guess;
    do
    {
        if (scanCount == 0)
            break;
        scanCount = scanCount - 1;
        ch = *ptr;
        ptr = ptr + 1;
    } while (ch != '\0');
    if (scanCount == -2)
    {
notOk:
        ok = false;
    }
    else
    {
        scanCount = -1;
        ptr = g_eqmain_login_pin_buf_guess;
        do
        {
            if (scanCount == 0)
                break;
            scanCount = scanCount - 1;
            ch = *ptr;
            ptr = ptr + 1;
        } while (ch != '\0');
        if (scanCount == -2)
        {
            goto notOk;
        }
        EQ_STDCALL(void, 0x10019FF0, (void))();
        result = EQ_CDECL(int, 0x10021AC0, (char *, char *, char *))(
            g_eqmain_login_name_buf_guess, g_eqmain_login_pin_buf_guess,
            EQ_ARRAY(char, 0x100554B0));
        ok = (char)result != '\0';
        EQ_STDCALL(void, 0x1001A070, (void))();
        if (ok)
        {
            goto checkDevice;
        }
    }
    EQ_CDECL(void, 0x100197F0, (char *, int, int))(
        EQ_ARRAY(char, 0x100481C0), *(int *)0x10055A3C, *(int *)0x10055A40);
    EQ_CDECL(void, 0x100197F0, (char *, int, int))(
        EQ_ARRAY(char, 0x100481B4), *(int *)0x10055A44, *(int *)0x10055A48);
    EQ_STDCALL(void, 0x1001C9E0, (void))();
checkDevice:
    result = EQ_STDCALL(int, 0x10018910, (void))();
    if ((char)result == '\0')
    {
        EQ_STDCALL(unsigned int, 0x100186D0, (void))();
    }
    state = (-(unsigned int)ok & 0xE) + 7;
    // The state loop. Its case blocks (1001C39F-1001C6BD) lie outside Ghidra's body for this function: Ghidra
    // could not decode the jump table at 1001C860 (34 entries, states 0x0-0x21). States 0x3, 0x8, 0x10,
    // 0x11, 0x14 and anything above 0x21 go straight to the state-3 check, like the original's table.
    for (;;)
    {
        if ((short)state == 0x11)
            break;
        switch ((short)state)
        {
        case 0:
            state = EQ_STDCALL(unsigned int, 0x1001A3A0, (void))();
            break;
        case 1:
            state = EQ_STDCALL(unsigned int, 0x10010FF0, (void))();
            break;
        case 2:
            EQ_STDCALL(void, 0x10019C90, (void))();
            state = 7;
            continue;
        case 4:
            state = EQ_CDECL(unsigned int, 0x10017A10, (int))(1);
            break;
        case 5:
            if (EQ_GLOBAL(char, 0x100F1D20) != '\0')
            {
                strcpy(name_b, EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x10048188)));
                ok = EQ_CDECL(char, 0x1001D7B0, (char *, int, char *, int))(
                    name_b, *(int *)0x10055700,
                    EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x1004817C)), 1) != 0;
            }
            else
            {
                strcpy(name_c, EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x10048170)));
                ok = EQ_CDECL(char, 0x1001D7B0, (char *, int, char *, int))(
                    name_c, *(int *)0x10055700,
                    EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x1004817C)), 1) != 0;
            }
            if (ok)
                goto state_3;
            *(short *)&state = EQ_GLOBAL(short, 0x1004814E);
            break;
        case 6:
            strcpy(name_a, EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x100481A4)));
            if (EQ_CDECL(char, 0x1001D7B0, (char *, int, char *, int))(
                    name_a, *(int *)0x10055704,
                    EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x10048198)), 1) != 0)
                goto state_3;
            *(short *)&state = EQ_GLOBAL(short, 0x1004814E);
            break;
        case 7:
            state = EQ_STDCALL(unsigned int, 0x100198B0, (void))();
            break;
        case 9:
            state = EQ_STDCALL(unsigned int, 0x10015CB0, (void))();
            break;
        case 0xA:
            state = EQ_STDCALL(unsigned int, 0x10015FA0, (void))();
            break;
        case 0xB:
            state = EQ_STDCALL(unsigned int, 0x10016060, (void))();
            break;
        case 0xC:
            state = EQ_STDCALL(unsigned int, 0x10011DE0, (void))();
            break;
        case 0xD:
            state = EQ_STDCALL(unsigned int, 0x10012C90, (void))();
            break;
        case 0xE:
            state = EQ_STDCALL(unsigned int, 0x10013900, (void))();
            break;
        case 0xF:
            state = EQ_STDCALL(unsigned int, 0x10016480, (void))();
            break;
        case 0x12:
            state = EQ_STDCALL(unsigned int, 0x1001E460, (void))();
            break;
        case 0x13:
            EQ_STDCALL(void, 0x100186D0, (void))();
            state = 7;
            continue;
        case 0x15:
            if (EQ_STDCALL(char, 0x1001D9F0, (void))() != 0)
                goto state_3;
            state = 0x12;
            continue;
        case 0x16:
            strcpy(name_d, EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x10048164)));
            if (EQ_CDECL(char, 0x1001D7B0, (char *, int, char *, int))(
                    name_d, *(int *)0x10055704,
                    EQ_CDECL(char *, 0x100210F0, (char *))(EQ_ARRAY(char, 0x10048198)), 1) != 0)
                goto state_3;
            state = 0;
            continue;
        case 0x17:
            state = EQ_STDCALL(unsigned int, 0x10018D60, (void))();
            break;
        case 0x18:
            state = EQ_STDCALL(unsigned int, 0x10019510, (void))();
            break;
        case 0x19:
            if (EQ_STDCALL(char, 0x1001F3A0, (void))() != 0)
                goto state_3;
            *(short *)&state = EQ_GLOBAL(short, 0x1004814E);
            break;
        case 0x1A:
            state = EQ_STDCALL(unsigned int, 0x1001F710, (void))();
            break;
        case 0x1B:
            state = EQ_STDCALL(unsigned int, 0x1001F880, (void))();
            break;
        case 0x1C:
            state = EQ_STDCALL(unsigned int, 0x1001FD40, (void))();
            break;
        case 0x1D:
            state = EQ_STDCALL(unsigned int, 0x100201B0, (void))();
            break;
        case 0x1E:
            state = EQ_STDCALL(unsigned int, 0x10020620, (void))();
            break;
        case 0x1F:
            state = EQ_CDECL(unsigned int, 0x10017A10, (int))(0);
            break;
        case 0x20:
            EQ_STDCALL(void, 0x1001AA20, (void))();
            state = 7;
            continue;
        case 0x21:
            EQ_STDCALL(void, 0x1001AE60, (void))();
            state = 7;
            continue;
        }
        if ((short)state == 3)
            break;
    }
    goto finish;
state_3:
    state = 3;
finish:
    g_eqmain_setwindowpos_guess(g_eqmain_hwnd_guess, -2, 0, 0, 0, 0, 0xB);
    if (g_eqmain_topwnd_guess != 0)
    {
        g_eqmain_setwindowpos_guess(g_eqmain_topwnd_guess, 0, 0, 0, 0, 0, 0xB);
    }
    EQ_CDECL(void, 0x1001B280, (char))(
        EQ_GLOBAL(char, 0x10048154));
    if ((short)state != 0x11)
    {
        *(int *)(exeInfo + 0x68C) = -1;
        return -1;
    }
    ptr = EQ_CDECL(char *, 0x10035C00, (char *))(EQ_ARRAY(char, 0x10055450));
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(ptr);
    strcpy((char *)(exeInfo + 0x40C), ptr);
    ptr = EQ_CDECL(char *, 0x10035C00, (char *))(g_eqmain_login_name_buf_guess);
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(ptr);
    strcpy((char *)(exeInfo + 0x6B0), ptr);
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(EQ_ARRAY(char, 0x10104CEC));
    strcpy((char *)(exeInfo + 0x48C), ptr);
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_login_pin_buf_guess);
    strcpy((char *)(exeInfo + 0x50C), ptr);
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(EQ_ARRAY(char, 0x100554B0));
    strcpy((char *)(exeInfo + 0x58C), ptr);
    ptr = EQ_CDECL(char *, 0x100211B0, (char *))(EQ_ARRAY(char, 0x10104C10));
    strcpy((char *)(exeInfo + 0x60C), ptr);
    *(int *)(exeInfo + 0x68C) = *(int *)0x10051AD4;
    return 0;
}

// FUNCTION: EQMAIN 0x1001C8F0
// Locates qintro.smk on a CD-ROM and builds its path for movie playback.
// __stdcall with no stack arguments (RET pops 0 bytes); returns nothing. Calls GetLogicalDriveStringsA(0x400) into a
// 0x404-byte frame buffer, then copies g_eqmain_drive_base_guess over g_eqmain_intro_path_guess. If the call returned nonzero and the first drive
// string is not empty, the strings are walked: each one whose first letter is a, A, b or B, or whose GetDriveTypeA is not
// 5 (CD-ROM), is skipped; the walk returns at an empty string (end of the list). At the first CD-ROM, g_eqmain_intro_path_guess is
// replaced by that drive string and "qintro.smk" (g_eqmain_qintro_guess) is appended.
void __stdcall EQMain_FindIntroMovie_guess(void)
{
    char drives[0x404];
    unsigned long drives_len;
    EqMainGetDriveTypeFn_guess get_drive_type;
    char driveChar;
    char *p;
    char *nextCharPtr;

    drives_len = g_eqmain_getdrivestrings_guess(0x400, drives);
    strcpy(g_eqmain_intro_path_guess, g_eqmain_drive_base_guess);
    if (drives_len != 0 && drives[0] != 0)
    {
        get_drive_type = g_eqmain_getdrivetype_guess;
        p = drives;
        while ((driveChar = *p) == 'a' || driveChar == 'A' || driveChar == 'b' || driveChar == 'B' || get_drive_type(p) != 5)
        {
            driveChar = *p;
            while (driveChar != 0)
            {
                nextCharPtr = p + 1;
                driveChar = *nextCharPtr;
                p = nextCharPtr;
            }
            nextCharPtr = p + 1;
            driveChar = *nextCharPtr;
            p = nextCharPtr;
            if (driveChar == 0)
                return;
        }
        strcpy(g_eqmain_intro_path_guess, p);
        strcat(g_eqmain_intro_path_guess, g_eqmain_qintro_guess);
    }
    return;
}

// FUNCTION: EQMAIN 0x1001C9E0
// Plays the qintro.smk intro movie in the front end.
// __stdcall with no stack arguments (RET pops 0 bytes); returns nothing. Plays the intro movie. g_eqmain_intro_mode_guess is 0 or 1
// (any other value returns after BltColorFillRgb_guess); the surface flags are 0x80000001 or 0xc0000001, plus 1 when g_eqmain_topleft_guess
// is set. The movie opens from "qintro.smk"; if that fails, EQMain_FindIntroMovie_guess runs and g_eqmain_intro_path_guess is opened instead. It is
// closed and the function returns when the movie is wider than g_eqmain_ceb8_guess[0] or taller than g_eqmain_ceb8_guess[1] (unsigned),
// or when _malloc(0x4b000) fails. g_eqmain_topleft_guess == 0 centres the frame, else it goes to the top-left. The frame counter is
// 0 at the start in both branches (Ghidra shows pvVar3 = _Memory in the centred branch; the binary never stores it there).
// Each pass draws with EQMain_SmackDrawFrame_guess and ends when the counter reaches the movie's frame count (+0xc, unsigned) or
// PumpMessagesAndPollKey_guess returns a nonzero low byte. Then the buffer, the blit and the movie are closed; g_eqmain_flag_ce78_guess is 1 at the end.
void __stdcall EQMain_PlayIntroMovie_guess(void)
{
    char done;
    void *frame;
    void *buf;
    int flags;
    int width;
    int height;
    int wait_result;
    EqMainSmackOpenFn_guess smack_open;
    EqMainSmackWaitFn_guess smack_wait;

    done = 0;
    frame = 0;
    eqmain_fn_10006b90_guess(g_eqmain_ceb8_guess, 0);
    if (g_eqmain_intro_mode_guess == 0)
    {
        flags = 0x80000001 + (g_eqmain_topleft_guess != 0);
    }
    else
    {
        if (g_eqmain_intro_mode_guess != 1)
        {
            return;
        }
        flags = 0xc0000001 + (g_eqmain_topleft_guess != 0);
    }
    g_eqmain_flag_ce78_guess = 0;
    eqmain_fn_10003a00_guess();
    g_eqmain_smack_usemmx_guess(1);
    g_eqmain_smack_sounddirect_guess(0);
    smack_open = g_eqmain_smack_open_guess;
    g_eqmain_smack_movie_guess = smack_open(g_eqmain_qintro_guess, 0xfe100, -1);
    if (g_eqmain_smack_movie_guess == 0)
    {
        eqmain_fn_1001c8f0_guess();
        g_eqmain_smack_movie_guess = smack_open(g_eqmain_intro_path_guess, 0xfe500, -1);
        if (g_eqmain_smack_movie_guess == 0)
        {
            return;
        }
    }
    if (*(unsigned int *)g_eqmain_ceb8_guess < *(unsigned int *)(g_eqmain_smack_movie_guess + 4) ||
        ((unsigned int *)g_eqmain_ceb8_guess)[1] < *(unsigned int *)(g_eqmain_smack_movie_guess + 8) ||
        (buf = eqmain_fn_1003342e_guess(0x4b000)) == 0)
    {
        g_eqmain_smack_close_guess(g_eqmain_smack_movie_guess);
        return;
    }
    if (g_eqmain_topleft_guess == 0)
    {
        height = *(int *)(g_eqmain_smack_movie_guess + 8);
        width = *(int *)(g_eqmain_smack_movie_guess + 4);
        g_eqmain_smack_tobuffer28_guess(g_eqmain_smack_movie_guess, (0x280U - width) >> 1, (0x1e0U - height) >> 1,
                                        width, height, buf, 0);
    }
    else
    {
        height = *(int *)(g_eqmain_smack_movie_guess + 8);
        width = *(int *)(g_eqmain_smack_movie_guess + 4);
        g_eqmain_smack_tobuffer28_guess(g_eqmain_smack_movie_guess, 0, 0, width, height, buf, 0);
    }
    g_eqmain_smack_buf_guess = g_eqmain_smack_blitopen_guess(flags);
    if (g_eqmain_smack_buf_guess == 0)
    {
        eqmain_fn_10033568_guess(buf);
        g_eqmain_smack_close_guess(g_eqmain_smack_movie_guess);
        return;
    }
    smack_wait = g_eqmain_smack_wait_guess;
    do
    {
        eqmain_fn_1001cbf0_guess(g_eqmain_hwnd_guess, buf);
        frame = (void *)((int)frame + 1);
        if (*(void **)(g_eqmain_smack_movie_guess + 0xc) <= frame)
        {
            done = 1;
        }
        if ((char)eqmain_fn_10005730_guess() != 0)
        {
            break;
        }
        wait_result = smack_wait(g_eqmain_smack_movie_guess);
        while (wait_result != 0)
        {
            wait_result = smack_wait(g_eqmain_smack_movie_guess);
        }
    } while (done == 0);
    eqmain_fn_10033568_guess(buf);
    if (g_eqmain_smack_buf_guess != 0)
    {
        g_eqmain_smack_blitclose_guess(g_eqmain_smack_buf_guess);
        g_eqmain_smack_buf_guess = 0;
    }
    if (g_eqmain_smack_movie_guess != 0)
    {
        g_eqmain_smack_close_guess(g_eqmain_smack_movie_guess);
        g_eqmain_smack_movie_guess = 0;
    }
    g_eqmain_flag_ce78_guess = 1;
    eqmain_fn_100039e0_arg_guess(0);
}

// FUNCTION: EQMAIN 0x1001CBF0
// Displays one movie frame when its window has focus.
// __cdecl (RET pops 0 bytes); returns nothing. Only when GetFocus() is window: a nonzero flag at movie +0x68 sets the
// palette (SmackBlitSetPalette: buffer, movie +0x6c, movie +0x370), SmackDoFrame decodes the frame, and the surface at
// wrapper +0x0c is locked into a zeroed 0x6c-byte DDSURFACEDESC (dwSize 0x6c; Lock slot 0x64 with NULL, &desc, 1, NULL).
// While Lock returns DDERR_SURFACELOST (0x887601C2) the surface is restored (slot 0x6c) and locked again. Wrapper +0x14 =
// desc lpSurface (+0x24), wrapper +0x18 = desc lPitch (+0x10). Each SmackToBufferRect(movie, 0) that returns nonzero blits
// with SmackBlit (11 arguments; the 6th is buffer, the second stack argument, which the Ghidra signature does not have).
// The surface is unlocked (slot 0x80, NULL) and SmackNextFrame(movie) ends the call.
void __cdecl EQMain_SmackDrawFrame_guess(int window, int buffer)
{
    int desc[27];   // DDSURFACEDESC: dwSize [0], lPitch [4] (0x10), lpSurface [9] (0x24)
    int hr;
    int more;
    EqMainSmackToBufferRectFn_guess to_buffer_rect;
    EqMainSmackBlitFn_guess blit;

    if (g_eqmain_getfocus_guess() == window)
    {
        if (*(int *)(g_eqmain_smack_movie_guess + 0x68) != 0)
            g_eqmain_smack_setpalette_guess(g_eqmain_smack_buf_guess, g_eqmain_smack_movie_guess + 0x6c,
                                            *(int *)(g_eqmain_smack_movie_guess + 0x370));
        g_eqmain_smack_doframe_guess(g_eqmain_smack_movie_guess);
        memset(desc, 0, sizeof(desc));
        desc[0] = 0x6c;
        hr = ((int (__stdcall *)(int *, int *, int *, int, int))(*(void ***)g_eqmain_surface_guess)[0x19])(
                g_eqmain_surface_guess, 0, desc, 1, 0);
        while (hr == (int)0x887601C2)
        {
            ((void (__stdcall *)(int *))(*(void ***)g_eqmain_surface_guess)[0x1b])(g_eqmain_surface_guess);
            hr = ((int (__stdcall *)(int *, int *, int *, int, int))(*(void ***)g_eqmain_surface_guess)[0x19])(
                    g_eqmain_surface_guess, 0, desc, 1, 0);
        }
        *(int *)(g_eqmain_ceb8_guess + 0x14) = desc[9];
        *(int *)(g_eqmain_ceb8_guess + 0x18) = desc[4];
        to_buffer_rect = g_eqmain_smack_tobuf_guess;
        more = to_buffer_rect(g_eqmain_smack_movie_guess, 0);
        if (more != 0)
        {
            blit = g_eqmain_smack_blit_guess;
            do
            {
                blit(g_eqmain_smack_buf_guess, *(int *)(g_eqmain_ceb8_guess + 0x14),
                     *(int *)(g_eqmain_ceb8_guess + 0x18),
                     *(int *)(g_eqmain_smack_movie_guess + 0x380), *(int *)(g_eqmain_smack_movie_guess + 0x384),
                     buffer, *(int *)(g_eqmain_smack_movie_guess + 0x4),
                     *(int *)(g_eqmain_smack_movie_guess + 0x380), *(int *)(g_eqmain_smack_movie_guess + 0x384),
                     *(int *)(g_eqmain_smack_movie_guess + 0x388), *(int *)(g_eqmain_smack_movie_guess + 0x38c));
                more = to_buffer_rect(g_eqmain_smack_movie_guess, 0);
            } while (more != 0);
        }
        ((void (__stdcall *)(int *, int))(*(void ***)g_eqmain_surface_guess)[0x20])(g_eqmain_surface_guess, 0);
        g_eqmain_smack_nextframe_guess(g_eqmain_smack_movie_guess);
    }
}

// FUNCTION: EQMAIN 0x1001CD50
// Escape decoder for the text loader at 0x1001CE60: turns the char after a backslash into its C escape value
// __cdecl (RET pops 0 bytes); returns char. Reads a char with _fgetc and decodes it as the char after a backslash:
// ' " \ 0 ? a b f n t v. VC6 writes only AL for those cases, leaving _fgetc's upper 24 EAX bits intact. A newline
// is skipped: the next char is read, and a backslash there starts another escape (recursion). Other chars are returned.
char __cdecl EQMain_ReadEscape_guess(struct _iobuf *file)
{
    // Keep the recursive raw EAX result as int; a char-typed call would sign-extend AL here.
    typedef int (__cdecl *ReadEscapeIntFn_guess)(struct _iobuf *);
    int iVar1;

    iVar1 = eqmain_fn_10035f9a_guess(file);
    switch ((char)iVar1)
    {
    case 'n':
        return 0x0A;
    case '\\':
        return '\\';
    case 't':
        return 0x09;
    case 'v':
        return 0x0B;
    case 'b':
        return 0x08;
    case 'f':
        return 0x0C;
    case 'a':
        return 0x07;
    case '?':
        return '?';
    case '\'':
        return '\'';
    case '"':
        return '"';
    case '0':
        return 0;
    case '\n':
        iVar1 = eqmain_fn_10035f9a_guess(file);
        if ((char)iVar1 == '\\')
        {
            iVar1 = ((ReadEscapeIntFn_guess)EQMain_ReadEscape_guess)(file);
        }
        break;
    }
    return (char)iVar1;
}

// FUNCTION: EQMAIN 0x1001CE60
// Load the game's license text for display.
// __stdcall (RET pops 0 bytes); no arguments, no return value. Zeroes the 60000-byte buffer at g_eqmain_license_buf_guess, then opens
// license.txt (path from Identity_guess, mode g_eqmain_fopen_mode_guess). No file: copies CopyStringToBuffer_guess(g_eqmain_fallback_key_guess) into the buffer,
// at most 2999 bytes. Otherwise reads 3000-byte records: the text between quotes, a newline stored as CR LF, a backslash
// escape through EQMain_ReadEscape_guess. A character at offset 0xbb5 or later is dropped and sets the error flag; once that flag
// is set and the FILE's 0x10 flag (EOF) is seen, the file is closed and the function returns.
void __stdcall EQMain_LoadLicenseText_guess(void)
{
    int i;
    int ch;
    char c;
    bool failed;
    char *path;
    struct _iobuf *file;
    char *record;

    memset(g_eqmain_license_buf_guess, 0, 60000);
    failed = false;
    path = eqmain_fn_100210f0_guess(g_eqmain_license_name_guess);
    file = eqmain_fn_1003407f_guess(path, g_eqmain_fopen_mode_guess);
    if (file == 0)
    {
        eqmain_fn_10033330_guess(g_eqmain_license_buf_guess,
                                 (char *)eqmain_fn_100211b0_guess(g_eqmain_fallback_key_guess), 2999);
        return;
    }
    record = g_eqmain_license_buf_guess;
readNextRecord:
    do
    {
        if (failed && (file->_flag & 0x10) != 0)
        {
            eqmain_fn_100343b9_guess(file);
            return;
        }
        i = eqmain_fn_10035f9a_guess(file);
        c = (char)i;
        while (c != '"')
        {
            if ((file->_flag & 0x10) != 0)
            {
                if ((char)i != '"')
                    goto markFailed;
                break;
            }
            i = eqmain_fn_10035f9a_guess(file);
            c = (char)i;
        }
        i = 0;
        ch = eqmain_fn_10035f9a_guess(file);
        c = (char)ch;
        while (c != '"')
        {
            c = (char)ch;
            if ((file->_flag & 0x10) != 0)
            {
                if (c != '"')
                    goto markFailed;
                break;
            }
            if (c == '\\')
            {
                ch = eqmain_fn_1001cd50_guess(file);
                c = (char)ch;
            }
            if (i < 0xbb5)
            {
                if (c == '\n')
                {
                    record[i++] = 0xd;
                    record[i++] = c;
                }
                else
                {
                    record[i++] = c;
                }
            }
            else
            {
                failed = true;
            }
            ch = eqmain_fn_10035f9a_guess(file);
            c = (char)ch;
        }
        if ((int)record < 0x100e49c0)
        {
            record += 3000;
        }
    } while (1);
markFailed:
    failed = true;
    goto readNextRecord;
}

// FUNCTION: EQMAIN 0x1001CF80
// Set the displayed text for a selected dialog control.
// __cdecl (RET pops 0 bytes); no return value. Looks up control controlId in the dialog DAT_100ec9c0 with GetDlgItem,
// keeps the handle in g_dialog_edit_guess, then sends WM_SETTEXT (0x0C) to it with text as the text pointer (lParam).
void __cdecl EQMain_SetItemText_guess(long text, int controlId)
{
    g_eqmain_dlg_item_guess = g_eqmain_getdlgitem_guess(g_eqmain_dlg_parent_guess, controlId);
    g_eqmain_sendmessage_guess(g_eqmain_dlg_item_guess, 0xc, 0, text);
}
