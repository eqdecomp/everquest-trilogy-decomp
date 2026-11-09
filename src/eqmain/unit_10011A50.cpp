// unit_10011A50.cpp - eqmain.dll, the address run 0x10011A50..0x10016060 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

struct EQMainWidget_guess {
    char pad_00[0x1c];
    int checked;
    char *text;
    char pad_24[4];
    short index;
};

// eqmain.dll's own globals and functions used here (VAs of this image, not trilogy's).
// used by EQMain_BuildSlotScreen_guess (0x10014910):
#define g_eqmain_obj_b_guess            EQ_GLOBAL(int, 0x1004D22C)                                  // g_eqmain_obj_b_guess: the second record (CreateRolloutRecord_guess's result; freed by UnlinkAndFreeRecord_guess)
#define g_eqmain_buf_1fc_guess          EQ_ARRAY(int, 0x1004D1FC)                                   // g_eqmain_buf_1fc_guess: the address EQMain_CreateLabelFixed_guess gets (inside the 0x1004D1F4 table)
#define g_eqmain_buf_200_guess          EQ_ARRAY(int, 0x1004D200)                                   // g_eqmain_buf_200_guess: the address EQMain_CreateLabelFixed_guess gets
#define g_eqmain_buf_208_guess          EQ_ARRAY(int, 0x1004D208)                                   // g_eqmain_buf_208_guess: the address EQMain_CreateLabelFixed_guess gets
#define g_eqmain_buf_20c_guess          EQ_ARRAY(int, 0x1004D20C)                                   // g_eqmain_buf_20c_guess: the address EQMain_CreateLabelFixed_guess gets
#define g_eqmain_buf_214_guess          EQ_ARRAY(int, 0x1004D214)                                   // g_eqmain_buf_214_guess: the address EQMain_CreateLabelFixed_guess gets
#define g_eqmain_buf_44c_guess          EQ_ARRAY(char, 0x1012C44C)                                  // g_eqmain_buf_44c_guess: the buffer the 0x1004CE40 string is copied to
#define g_eqmain_buf_3ec_guess          EQ_ARRAY(char, 0x1012C3EC)                                  // g_eqmain_buf_3ec_guess: the buffer the 0x1004CE40 string is copied to (second copy)
#define g_eqmain_str_47f9c_guess        EQ_ARRAY(char, 0x10047F9C)                                  // g_eqmain_str_47f9c_guess: the string FindNameIndex_guess is given
#define eqmain_fn_10025940_guess(object) EQ_METHOD(1, char *, 0x10025940, (int, int), (object))            // IdxLookup_guess::Get_guess: thiscall, one stack argument (the second parameter is the EDX dummy)
#define eqmain_fn_10025960_guess(object) EQ_METHOD(0, char *, 0x10025960, (int), (object))                      // ArrayCursor_guess::Next_guess: fastcall, one argument in ECX
#define eqmain_fn_10021110_guess        EQ_CDECL(char *, 0x10021110, (char *))                      // DecodeToBuffer_guess: cdecl, one arg, returns a pointer
#define eqmain_fn_10008910_guess        EQ_CDECL(void, 0x10008910, (void *))                        // EQMain_SetCachedEntry_guess: cdecl, one arg
#define eqmain_fn_10011bc0_guess        EQ_STDCALL(void, 0x10011BC0, (void))                        // EQMain_ReleaseObjects_guess: stdcall, no args
#define eqmain_fn_10011c90_guess        EQ_CDECL(void, 0x10011C90, (short, int, int, char *))       // EQMain_ProcessClassifiedString_guess: cdecl, 4 args
#define g_eqmain_obj_guess              EQ_GLOBAL(int, 0x1004D228)                                  // g_eqmain_obj_guess: the object (+0x28: short index)
#define g_eqmain_result_guess           EQ_GLOBAL(int, 0x1004D1C8)                                  // g_eqmain_result_guess: the 0/1/2 result
#define g_eqmain_name_first_guess       EQ_GLOBAL(char *, 0x10055AF0)                               // g_eqmain_name_first_guess: the first string
#define g_eqmain_name_second_guess      EQ_GLOBAL(char *, 0x10055AF4)                               // g_eqmain_name_second_guess: the second string
#define eqmain_fn_100061d0_guess        EQ_CDECL(char *, 0x100061D0, (int, int))                    // ForwardScaled_guess: the string for (object, index)
#define eqmain_fn_10033730_guess        EQ_CDECL(int, 0x10033730, (char *, char *))                 // __strcmpi
#define g_eqmain_cur_obj_guess          EQ_GLOBAL(int, 0x1004D224)                                  // g_eqmain_cur_obj_guess: the object ForwardScaled_guess gets (+0x28: short index)
#define g_eqmain_ret_obj_guess          EQ_GLOBAL(int, 0x1004D230)                                  // g_eqmain_ret_obj_guess: the object whose int at +0x20 is returned when the result is 2
#define g_eqmain_param1_guess           EQ_GLOBAL(int, 0x1004CF30)                                  // g_eqmain_param1_guess: the first argument of EQMain_Init_guess, saved
#define g_eqmain_param2_guess           EQ_GLOBAL(int, 0x1004CF34)                                  // g_eqmain_param2_guess: the second argument of EQMain_Init_guess, saved
#define g_eqmain_rec_guess              EQ_GLOBAL(int, 0x1004CF2C)                                  // g_eqmain_rec_guess: the pointer CreateSurfaceRecord_guess returns
#define g_eqmain_ceb8_guess             EQ_GLOBAL(int, 0x1004CEB8)                                  // g_eqmain_ceb8_guess: the 4th argument of BltFastSrcRect_guess (460 xrefs)
#define eqmain_fn_10006d80_guess        EQ_CDECL(int *, 0x10006D80, (int, int, int))                // CreateSurfaceRecord_guess: allocates the record, returns int *
#define eqmain_fn_10006c20_guess        EQ_CDECL(unsigned int, 0x10006C20, (int, int, int, int, int, int, int, int, unsigned int)) // BltFastSrcRect_guess: 9 args, result unused
#define eqmain_fn_100019e0_guess        EQ_CDECL(void, 0x100019E0, (void *))                        // eqmainFreeUnit_guess: frees one pointer (cdecl, 1 arg)
#define g_eqmain_state_guess            EQ_ARRAY(char, 0x1012C220)                                  // g_eqmain_block_c220_guess: the 0x2CC-byte block (zeroed; shorts at +0 and +2 set to 0xffff)
#define g_eqmain_slots_a_guess          EQ_ARRAY(void *, 0x1004D1CC)                                // g_eqmain_slots_a_guess: 10 pointers, each freed by eqmainFreeUnit_guess
#define g_eqmain_slots_b_guess          EQ_ARRAY(void *, 0x1004D1F4)                                // g_eqmain_slots_b_guess: 10 pointers, each freed by EQMain_FreeEntry_guess
#define g_eqmain_str_src_guess          EQ_ARRAY(char, 0x1004CE40)                                  // g_eqmain_drive_base_guess: the string strcpy copies
#define g_eqmain_str_dst_guess          EQ_ARRAY(char, 0x1004D168)                                  // g_eqmain_str_dst_guess: the buffer the string is copied to
#define g_eqmain_ptr_guess              EQ_GLOBAL(void *, 0x1004D1AC)                               // g_eqmain_ptr_guess: the pointer EQMain_FreeEntry_guess frees last
#define g_eqmain_code_guess             EQ_GLOBAL(int, 0x1004D234)                                  // g_eqmain_code_guess: the stack argument of UnitIndexGet_guess (0x2d, 0x2c or 5 by the result)
#define g_eqmain_this_guess             EQ_GLOBAL(int, 0x1012C774)                                  // g_eqmain_obj_c774_guess: the this of UnitIndexGet_guess
#define eqmain_fn_10011c30_guess        EQ_STDCALL(void, 0x10011C30, (void))                        // EQMain_ClassifyCurrentString_guess: classifies the string into g_eqmain_result_guess (stdcall, no args)
#define eqmain_fn_10025c00_guess        EQ_THISCALL(int, 0x10025C00, (int, int, int))               // UnitIndexGet_guess: thiscall, one stack argument (the second parameter is the EDX dummy)
#define eqmain_fn_10025c00_method_guess(object) EQ_METHOD(1, int, 0x10025C00, (int, int), (object))
#define eqmain_fn_10025950_guess        EQ_THISCALL(char *, 0x10025950, (int))                      // FUN_10025950: fastcall, one argument in ECX, returns the string
#define eqmain_fn_10025950_method_guess(object) EQ_METHOD(0, char *, 0x10025950, (int), (object))
#define eqmain_fn_10011970_guess        EQ_CDECL(void, 0x10011970, (int, int, char *, int *, unsigned int, char *)) // EQMain_CreateLabelFixed_guess: cdecl, 6 args
#define eqmain_fn_10005fd0_guess        EQ_CDECL(int *, 0x10005FD0, (int, int, int, unsigned char, char *, int, unsigned char, unsigned char)) // CreateRolloutRecord_guess: cdecl, 8 args, returns the record
#define eqmain_fn_100061f0_guess        EQ_CDECL(unsigned int, 0x100061F0, (int, char *))           // FindNameIndex_guess: cdecl, 2 args, returns uint
#define eqmain_fn_10006510_guess        EQ_CDECL(void, 0x10006510, (int *, int, int))               // SetWordFieldsAndUpdate_guess: cdecl, 3 args, void
#define eqmain_fn_1003396b_guess        EQ_CDECL(char *, 0x1003396B, (char *))                      // __strdup: cdecl, one arg, returns the copy
#define eqmain_fn_10033568_guess        EQ_CDECL(void, 0x10033568, (void *))                        // _free: cdecl, one arg
#define g_eqmain_slot_guess             EQ_GLOBAL(short, 0x1012C222)                                // g_eqmain_slot_guess: the selected slot index (short, -1 = none)
#define g_eqmain_count_guess             EQ_GLOBAL(short, 0x1012C214)                                // g_eqmain_count_c214_guess: the slot count (short); the loop builds the slots below it
#define g_eqmain_sel_guess               EQ_GLOBAL(short, 0x1012C220)                                // g_eqmain_block_c220_guess: the short at +0 of the state block; the slot index that is 'selected'
#define g_eqmain_dlg_arg_guess              EQ_GLOBAL(int, 0x1004CEC8)                                  // g_eqmain_dlg_arg_guess: the 8th argument of EQMain_CreateRecord_guess (21 xrefs)
#define g_eqmain_cf24_guess              EQ_GLOBAL(int *, 0x1004CF24)                                // g_eqmain_cf24_guess: the pointer of the selected slot (dword)
#define g_eqmain_cf38_guess              EQ_ARRAY(int *, 0x1004CF38)                                 // g_eqmain_cf38_guess: 10 pointers, one per slot (CreateColumnCell_guess's result)
#define g_eqmain_c4ec_guess              EQ_ARRAY(int, 0x1012C4EC)                                   // g_eqmain_c4ec_guess: the 0x40-byte name of each slot
#define g_eqmain_bf94_guess              EQ_ARRAY(int, 0x1012BF94)                                   // g_eqmain_bf94_guess: the 0x40-byte second name of each slot
#define g_eqmain_str_a_guess             EQ_GLOBAL(char *, 0x10055814)                               // g_eqmain_str_a_guess: the first string (copied to g_eqmain_copybuf_guess)
#define g_eqmain_str_b_guess             EQ_GLOBAL(char *, 0x10055818)                               // g_eqmain_str_b_guess: the second string (copied to g_eqmain_copybuf_guess)
#define g_eqmain_copybuf_guess           EQ_ARRAY(char, 0x100F4C10)                                  // g_eqmain_copybuf_guess: the buffer the strings are copied to
#define eqmain_str_chekbox1_guess        ((char *)0x100461D8)                                        // "..." (.rdata)
#define eqmain_str_chekbox2_guess        ((char *)0x100461E8)                                        // "..." (.rdata)
#define eqmain_fn_100018f0_guess         EQ_CDECL(int *, 0x100018F0, (int, int, int, char *, char *, int, int, char *, int, int)) // CreateColumnCell_guess: cdecl, 8 declared args; the caller pushes 2 more zeros
#define eqmain_fn_100082c0_7args_guess   EQ_CDECL(void *, 0x100082C0, (int, unsigned int, int, int, char *, int, int)) // EQMain_DrawTextRun_guess as this function calls it: 5 args + 2 zeros
#define eqmain_fn_10008c10_guess         EQ_CDECL(int *, 0x10008C10, (int, int, int, int, int, int, int, int, int)) // EQMain_CreateRecord_guess: cdecl, 9 args, returns int *
#define eqmain_fn_10009370_guess         EQ_CDECL(void, 0x10009370, (int *, char *))                 // WrapTextLines_guess: cdecl, 2 args, void
#define eqmain_fn_10008d60_guess         EQ_CDECL(void, 0x10008D60, (int *))                         // DrawTextBlock_guess: cdecl, 1 arg, void
#define eqmain_str_eqvstart_guess        ((char *)0x10047F7C)                                        // "..." (.rdata)
#define g_eqmain_text_58ac_guess         EQ_GLOBAL(char *, 0x100558AC)                               // g_eqmain_text_58ac_guess: the text of the first control (EQMain_FitFontSize_guess, eqmain_draw_text_guess)
#define g_eqmain_text_58a8_guess         EQ_GLOBAL(char *, 0x100558A8)                               // g_eqmain_text_58a8_guess: the text of the second control
#define g_eqmain_text_58a4_guess         EQ_GLOBAL(char *, 0x100558A4)                               // g_eqmain_text_58a4_guess: the text of the third control
#define g_eqmain_text_5830_guess         EQ_GLOBAL(char *, 0x10055830)                               // g_eqmain_text_5830_guess: the text of the fourth control
#define g_eqmain_busy_guess              EQ_GLOBAL(char, 0x1004CE78)                                 // g_eqmain_flag_ce78_guess: 1 while the start-screen loop runs
#define g_eqmain_flag_5b34_guess         EQ_GLOBAL(char, 0x100D5B34)                                 // g_eqmain_flag_5b34_guess: set to 1 by the 'A' key
#define g_chat_status_guess         EQ_GLOBAL(short, 0x1004814E)                                // g_chat_status_guess: set to 9 by a click on a null handle
#define g_eqmain_login_pin_buf_guess          EQ_ARRAY(char, 0x1012565C)                                  // g_eqmain_login_pin_buf_guess: the buffer the 'E' key copies its text to
#define eqmain_fn_100080b0_guess         EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int)) // EQMain_FitFontSize_guess: cdecl, 4 params; the caller pushes 2 more (0x19 or the previous result, then 0)
#define eqmain_fn_1001a210_guess         EQ_STDCALL(unsigned int, 0x1001A210, (void))                // EQMain_ReportCode_guess: stdcall, no args, the key code is in AX
#define eqmain_fn_10025840_guess         EQ_STDCALL(void, 0x10025840, (void))                        // eqmain_call_100213d0_push40_guess: stdcall, no args, void
#define eqmain_fn_10025830_guess         EQ_STDCALL(void, 0x10025830, (void))                        // eqmain_call_100213d0_guess: stdcall, no args, void
#define eqmain_fn_10002b50_guess         EQ_CDECL(char, 0x10002B50, (char *))                        // eqmainChangePasswordDialog_guess: cdecl, 1 arg, returns a bool
#define eqmain_fn_10023b10_guess         EQ_CDECL(void, 0x10023B10, (char *, char *))                // SendStringPair45Packet_guess: cdecl, 2 args, void
#define eqmain_fn_10004e10_guess         EQ_CDECL(void, 0x10004E10, (int))                           // DispatchQueueList_guess: cdecl, 1 arg, void
#define eqmain_fn_10008ce0_guess         EQ_CDECL(void, 0x10008CE0, (int *))                         // chain_table_free_guess: cdecl, 1 arg, void
#define g_eqmain_cf20_guess             EQ_GLOBAL(short, 0x1004CF20)                                // g_eqmain_cf20_guess: the short at +8 of the object UnitIndexGet_guess(this, 0x21) returns
#define g_eqmain_21c_guess              EQ_GLOBAL(int, 0x1004D21C)                                  // g_eqmain_21c_guess: the pointer of the selected slot (0 when the index is -1)
#define eqmain_fn_100082c0_guess        EQ_CDECL(void *, 0x100082C0, (int, unsigned int, int, int, char *)) // EQMain_DrawTextRun_guess: cdecl, 5 args, result unused
#define eqmain_fn_100119a0_guess        EQ_CDECL(void, 0x100119A0, (void **, int, int, void *, int, int, unsigned int)) // EQ_Columns_MarkColumnCellsAsActive_guess: cdecl, 7 args (MarkColumnCellsAsActive)

// used by EQMain_PollStatus_guess (0x10011a50):
#define eqmain_fn_100254f0_guess        EQ_STDCALL(unsigned int, 0x100254F0, (void))                // eqmain_process_messages_guess: stdcall, no args, returns the status code
#define eqmain_fn_10001ff0_guess        EQ_CDECL(char, 0x10001FF0, (int *, char *, char *, char *)) // eqmainRunDialog_guess: cdecl, 4 args (the g_eqmain_ceb8 object, buffer, string, NULL), result unused
#define eqmain_fn_10003090_guess        EQ_STDCALL(void, 0x10003090, (void))                        // eqmainCleanupDialog_guess: stdcall, no args (calls eqmainPopSlotState_guess)
#define g_str_888_guess             EQ_GLOBAL(char *, 0x10055888)                               // g_str_888_guess: the third argument of eqmainRunDialog_guess (68 xrefs)
#define g_eqmain_5760_guess             EQ_GLOBAL(char *, 0x10055760)                               // g_eqmain_5760_guess: the second argument of eqmainRunDialog_guess for code 0x10000003
#define g_eqmain_d220_guess             EQ_GLOBAL(char, 0x1004D220)                                 // g_eqmain_d220_guess: flag byte; when set it is cleared and eqmainCleanupDialog_guess is called

// used by EQMain_LoginScreen_guess (0x10016060):
#define g_eqmain_cf60_guess             EQ_GLOBAL(int, 0x1004CF60)                                  // g_eqmain_cf60_guess: -1 skips the message loop (the done flag is set instead)
#define g_eqmain_cf64_guess             EQ_GLOBAL(int *, 0x1004CF64)                                // g_eqmain_cf64_guess: the record CreateNamedNode_guess returns
#define g_eqmain_cf28_guess             EQ_GLOBAL(int *, 0x1004CF28)                                // g_eqmain_cf28_guess: the record CreateSlider_guess returns
#define g_eqmain_d238_guess             EQ_GLOBAL(int, 0x1004D238)                                  // g_eqmain_d238_guess: cleared when g_eqmain_cf60_guess is not -1
#define g_eqmain_5820_guess             EQ_GLOBAL(char *, 0x10055820)                               // g_eqmain_5820_guess: the string strcpy copies to g_eqmain_copybuf_guess
#define g_eqmain_581c_guess             EQ_GLOBAL(char *, 0x1005581C)                               // g_eqmain_581c_guess: the string copied to g_eqmain_copybuf_guess for code 0x10000003
#define eqmain_str_news_guess           ((char *)0x10046240)                                        // "..." (.rdata)
#define eqmain_str_txaccount_guess      ((char *)0x10047FE0)                                        // "..." (.rdata)
#define eqmain_fn_10007bf0_guess        EQ_CDECL(void, 0x10007BF0, (int, char *, int))              // PackFsBlitNamedImageAtOrigin_guess: cdecl, 3 args
#define eqmain_fn_100210f0_guess        EQ_CDECL(char *, 0x100210F0, (char *))                      // Identity_guess: cdecl, one arg, returns a pointer
#define eqmain_fn_100056c0_guess        EQ_CDECL(void, 0x100056C0, (int, char *, int, unsigned int)) // DrawNamedImageCentered_guess: cdecl, 4 args
#define eqmain_fn_10001520_guess        EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int)) // eqmain_create_button_guess: cdecl, 5 args, returns the window handle (HDC)
#define eqmain_fn_100017b0_guess        EQ_CDECL(void, 0x100017B0, (int, int, char *, int, int))    // eqmain_draw_text_guess: cdecl, 5 pushed args (Ghidra shows 3; the caller pushes 5)
#define eqmain_fn_10005840_guess        EQ_CDECL(int *, 0x10005840, (int, int, int, int, int, unsigned int, int)) // CreateNamedNode_guess: cdecl, 7 args, returns the record
#define eqmain_fn_10006950_guess        EQ_CDECL(int *, 0x10006950, (int, int, int, int, int, int)) // CreateSlider_guess: cdecl, 6 args, returns the record
#define eqmain_fn_10003a00_guess        EQ_STDCALL(void, 0x10003A00, (void))                        // TickBlinkAndCursor_guess: stdcall, no args
#define eqmain_fn_10025800_guess        EQ_CDECL(void, 0x10025800, (int, int))                      // eqmain_setup_push38_guess: cdecl, 2 args
#define eqmain_fn_10003000_guess        EQ_CDECL(void, 0x10003000, (char *))                        // eqmainBuildDialog_guess: cdecl, one arg
#define eqmain_fn_10003bd0_guess        EQ_STDCALL(int *, 0x10003BD0, (void))                       // PumpMessagesAndEditText_guess: stdcall, no args, returns the message record or NULL
#define eqmain_fn_100044b0_guess        EQ_CDECL(void, 0x100044B0, (int, int, int, int))            // EnqueueQuad_guess: cdecl, 4 args
#define eqmain_fn_10001700_guess        EQ_CDECL(void, 0x10001700, (int))                           // eqmain_object_destroy_guess: cdecl, one arg
#define eqmain_fn_10005930_guess        EQ_CDECL(void, 0x10005930, (int *))                         // DestroyNamedNode_guess: cdecl, one arg
#define eqmain_fn_10006ae0_guess        EQ_CDECL(void, 0x10006AE0, (int *))                         // ReleaseQueueUnit_guess: cdecl, one arg
#define eqmain_fn_10005a00_guess        EQ_CDECL(void, 0x10005A00, (int *))                         // DrawNamedRows_guess: cdecl, one arg
#define eqmain_fn_100039e0_guess        EQ_CDECL(void, 0x100039E0, (int))                           // ClearBlinkAndPump_guess: pushed with one 0 before the call (the caller pops it); Ghidra gives the callee no params

// FUNCTION: EQMAIN 0x10011A50
// Polls front-end messages and handles recognized dialog states.
// __stdcall, no arguments. Returns eqmain_process_messages_guess's code for the handled codes (0x02, 0x1c, 0x32, 0x35, 0x1d, 0x33, 0x36, 0x39,
// 0x10000003), 0 for the others. Codes 0x02, 0x1c, 0x32 and 0x35 call eqmainRunDialog_guess with the buffer at 0x100F4C10, and 0x10000003
// with g_eqmain_5760_guess, first. For the handled codes the flag byte at 0x1004D220 is then cleared and eqmainCleanupDialog_guess called if it was set.
unsigned int __stdcall EQMain_PollStatus_guess(void)
{
    int status;

    status = (int)eqmain_fn_100254f0_guess();
    switch (status)
    {
    // 0 and 1 have their own return 0 bodies, not default's: that starts the jump table at 0 as the original does
    case 0:
    case 1:
        return 0;
    default:
        return 0;
    case 0x02:
        eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
        break;
    case 0x1c:
    case 0x32:
    case 0x35:
        eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
        break;
    case 0x1d:
    case 0x33:
    case 0x36:
        break;
    case 0x39:
        break;
    case 0x10000003:
        eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_5760_guess, g_str_888_guess, (char *)0);
        break;
    }
    if (status != 0)
    {
        if (g_eqmain_d220_guess != 0)
        {
            g_eqmain_d220_guess = 0;
            eqmain_fn_10003090_guess();
        }
    }
    return status;
}

// FUNCTION: EQMAIN 0x10011b60
// Saves the 0x14A x 0x12 area at (value, value2) of 0x1004CEB8 in a new record; zeroes the two control records.
// __cdecl, two int arguments (value, value2); returns nothing. Clears the objects at 0x1004D230 and 0x1004D224, saves the two
// arguments at 0x1004CF30 and 0x1004CF34, stores CreateSurfaceRecord_guess(0x14a, 0x12, 0x10) at 0x1004CF2C, then calls BltFastSrcRect_guess with that
// pointer, 0, 0, the int at 0x1004CEB8, the two arguments, 0x14a, 0x12 and 0.
void __cdecl EQMain_Init_guess(int value, int value2)
{
    g_eqmain_ret_obj_guess = 0;
    g_eqmain_cur_obj_guess = 0;
    g_eqmain_param1_guess = value;
    g_eqmain_param2_guess = value2;
    g_eqmain_rec_guess = (int)eqmain_fn_10006d80_guess(0x14a, 0x12, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_rec_guess, 0, 0, g_eqmain_ceb8_guess, value, value2, 0x14a, 0x12, 0);
}

#define eqmain_fn_10008a00_guess        EQ_CDECL(void, 0x10008A00, (int))                           // EQMain_FreeEntry_guess: cdecl, called with the object at 0x1004D230 when it is not null
#define eqmain_fn_10006130_guess        EQ_CDECL(void, 0x10006130, (int))                           // UnlinkAndFreeRecord_guess: cdecl, called with the object at 0x1004D224 when it is not null

// FUNCTION: EQMAIN 0x10011bc0
// Releases the active front-end records.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Frees the object at 0x1004D230 with EQMain_FreeEntry_guess and the
// object at 0x1004D224 with UnlinkAndFreeRecord_guess (each only when it is not null, 0x1004D230 first), then clears both globals.
void __stdcall EQMain_ReleaseObjects_guess(void)
{
    if (g_eqmain_ret_obj_guess != 0) {
        eqmain_fn_10008a00_guess(g_eqmain_ret_obj_guess);
    }
    if (g_eqmain_cur_obj_guess != 0) {
        eqmain_fn_10006130_guess(g_eqmain_cur_obj_guess);
    }
    g_eqmain_cur_obj_guess = 0;
    g_eqmain_ret_obj_guess = 0;
}

// FUNCTION: EQMAIN 0x10011c00
// Gets the current value from the active front-end control.
// __stdcall, no arguments (callee pops 0 bytes); returns a value in EAX. When the result at 0x1004D1C8 is 2 it returns the
// int at +0x20 of the object at 0x1004D230. Otherwise it returns what ForwardScaled_guess leaves in EAX, called with the object at
// 0x1004D224 and the short at its +0x28 as the index (pushed index first, then the object, as the original does).
int __stdcall EQMain_GetCurrentValue_guess(void)
{
    if (g_eqmain_result_guess == 2) {
        return *(int *)(g_eqmain_ret_obj_guess + 0x20);
    }
    return (int)eqmain_fn_100061d0_guess(g_eqmain_cur_obj_guess, *(short *)(g_eqmain_cur_obj_guess + 0x28));
}

// FUNCTION: EQMAIN 0x10011c30
// Compares the current text with two configured strings and records the match.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Compares the string ForwardScaled_guess gives for the
// object's index (the short at +0x28 of the object at 0x1004D228) with the string at 0x10055AF0, case-insensitively:
// on a match the result at 0x1004D1C8 is 0 and the function returns. Otherwise the same string is compared with
// the one at 0x10055AF4 and the result is 1 on a match, 2 when neither matches.
// The strings are read as the last argument of __strcmpi, which VC6 pushes before the ForwardScaled_guess call, as the
// original does (Ghidra's pcVar3 = g_eqmain_name_first_guess / g_eqmain_name_second_guess statements).
void __stdcall EQMain_ClassifyCurrentString_guess(void)
{
    int compareResult;

    compareResult = eqmain_fn_10033730_guess(eqmain_fn_100061d0_guess(g_eqmain_obj_guess, *(short *)(g_eqmain_obj_guess + 0x28)), g_eqmain_name_first_guess);
    if (compareResult == 0) {
        g_eqmain_result_guess = compareResult;
        return;
    }
    compareResult = eqmain_fn_10033730_guess(eqmain_fn_100061d0_guess(g_eqmain_obj_guess, *(short *)(g_eqmain_obj_guess + 0x28)), g_eqmain_name_second_guess);
    g_eqmain_result_guess = (compareResult != 0) + 1;
    return;
}

// FUNCTION: EQMAIN 0x10011c90
// Redraws the saved area, then makes a new control at (value2, value3) by the 0/1/2 class, preset to text.
// __cdecl, callee pops 0 bytes; returns nothing. Calls BltFastSrcRect_guess with nine args (the globals at 0x1004CEB8, 0x1004CF30,
// 0x1004CF34 and 0x1004CF2C, then 0, 0, 0x14A, 0x12, 0), then EQMain_ClassifyCurrentString_guess, which leaves the 0/1/2 result at 0x1004D1C8.
// Result 0 sets the code at 0x1004D234 to 0x2D and value to 0xD, result 1 sets it to 0x2C, result 2 sets it to 5 and
// returns after EQMain_CreateLabelFixed_guess gets value2, value3, FUN_10025950(UnitIndexGet_guess(0x1012C774, code)), &0x1004D230, 0x40 and
// text; any other result leaves the code alone. Results 0 and 1 and the other values then duplicate the string with
// __strdup, CreateRolloutRecord_guess makes the record kept at 0x1004D224, and SetWordFieldsAndUpdate_guess gets the record, value and FindNameIndex_guess's
// value for the copy (0 when the copy is empty); then the copy is freed. The nested calls keep the original's argument
// order, as its pushes show.
// @early-stop: no dead re-test of the result in the default (switch value in ECX, case 2 store reloaded); EQ_THISCALL's dummy EDX adds xor edx,edx.
void __cdecl EQMain_ProcessClassifiedString_guess(int value, int value2, int value3, char *text)
{
    char *dup;

    eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, g_eqmain_param1_guess, g_eqmain_param2_guess, g_eqmain_rec_guess, 0, 0, 0x14a, 0x12, 0);
    eqmain_fn_10011c30_guess();
    switch (g_eqmain_result_guess) {
    case 0:
        g_eqmain_code_guess = 0x2d;
        value = 0xd;
        break;
    case 1:
        g_eqmain_code_guess = 0x2c;
        break;
    case 2:
        g_eqmain_code_guess = 5;
        eqmain_fn_10011970_guess(value2, value3, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_this_guess)(g_eqmain_code_guess))(), &g_eqmain_ret_obj_guess, 0x40, text);
        return;
    }
    dup = eqmain_fn_1003396b_guess(text);
    g_eqmain_cur_obj_guess = (int)eqmain_fn_10005fd0_guess(g_eqmain_ceb8_guess, value2, value3, 0, text, eqmain_fn_10025c00_method_guess(g_eqmain_this_guess)(g_eqmain_code_guess), 1, 0);
    if (strlen(dup) != 0) {
        eqmain_fn_10006510_guess((int *)g_eqmain_cur_obj_guess, value, eqmain_fn_100061f0_guess(g_eqmain_cur_obj_guess, dup));
    }
    else {
        eqmain_fn_10006510_guess((int *)g_eqmain_cur_obj_guess, value, 0);
    }
    eqmain_fn_10033568_guess(dup);
}

#define g_eqmain_start_record_guess     EQ_GLOBAL(EQMainWidget_guess *, 0x1004D228)
#define g_eqmain_text_5848_guess     EQ_GLOBAL(char *, 0x10055848)
#define g_upg_text_b_guess     EQ_GLOBAL(char *, 0x10055854)
#define g_eqmain_start_label_a_guess    EQ_GLOBAL(char *, 0x100557C0)
#define g_eqmain_start_label_b_guess    EQ_GLOBAL(char *, 0x100557C4)
#define g_eqmain_start_label_c_guess    EQ_GLOBAL(char *, 0x100557C8)
#define g_eqmain_start_label_d_guess    EQ_GLOBAL(char *, 0x100557CC)
#define g_eqmain_start_format_guess     EQ_GLOBAL(char *, 0x100557D4)
#define g_eqmain_start_text_a_guess     EQ_GLOBAL(char *, 0x10055868)
#define g_eqmain_start_text_b_guess     EQ_GLOBAL(char *, 0x1005586C)
#define g_eqmain_start_text_c_guess     EQ_GLOBAL(char *, 0x10055828)
#define g_eqmain_start_text_d_guess     EQ_GLOBAL(char *, 0x1005590C)
#define g_eqmain_start_bitmap_guess     EQ_ARRAY(char, 0x10047FA4)
#define g_eqmain_start_const_8c_guess   EQ_ARRAY(char, 0x10047F8C)
#define g_eqmain_start_const_90_guess   EQ_ARRAY(char, 0x10047F90)
#define g_eqmain_start_const_94_guess   EQ_ARRAY(char, 0x10047F94)
#define g_eqmain_start_const_98_guess   EQ_ARRAY(char, 0x10047F98)
#define g_eqmain_start_const_a0_guess   EQ_ARRAY(char, 0x10047FA0)
#define g_eqmain_start_field_0_guess    EQ_ARRAY(char, 0x1012B945)
#define g_eqmain_start_field_1_guess    EQ_ARRAY(char, 0x1012B965)
#define g_eqmain_start_field_2_guess    EQ_ARRAY(char, 0x1012B985)
#define g_eqmain_start_field_3_guess    EQ_ARRAY(char, 0x1012B989)
#define g_eqmain_start_field_4_guess    EQ_ARRAY(char, 0x1012BA09)
#define g_eqmain_start_field_5_guess    EQ_ARRAY(char, 0x1012BA49)
#define g_eqmain_start_field_6_guess    EQ_ARRAY(char, 0x1012BA89)
#define g_eqmain_start_field_7_guess    EQ_ARRAY(char, 0x1012BAA9)
#define g_eqmain_start_field_8_guess    EQ_ARRAY(char, 0x1012BAC9)
#define g_eqmain_start_field_9_guess    EQ_ARRAY(char, 0x1012BAF1)
#define g_eqmain_start_field_10_guess   EQ_ARRAY(char, 0x1012BB19)
#define g_eqmain_start_field_11_guess   EQ_ARRAY(char, 0x1012BB99)
#define g_eqmain_start_field_12_guess   EQ_ARRAY(char, 0x1012BC93)
#define g_eqmain_start_field_13_guess   EQ_ARRAY(char, 0x1012BC9F)
#define g_eqmain_start_field_14_guess   EQ_ARRAY(char, 0x1012BCA3)
#define g_eqmain_start_field_15_guess   EQ_ARRAY(char, 0x1012BCA7)
#define eqmain_start_fn_10011b60_guess  EQ_CDECL(void, 0x10011B60, (int, int))
#define eqmain_start_fn_100118f0_guess  EQ_CDECL(void, 0x100118F0, (int, int, int, int, int, char *, int *, int, char *))
#define eqmain_start_fn_100113c0_guess  EQ_STDCALL(char, 0x100113C0, (void))
#define eqmain_start_fn_100253b0_guess  EQ_STDCALL(void, 0x100253B0, (void))
#define eqmain_start_fn_10001a60_guess  EQ_CDECL(void, 0x10001A60, (void *, void *, void *))
#define eqmain_start_fn_100211b0_guess  EQ_CDECL(char *, 0x100211B0, (char *))
#define eqmain_start_fn_10007a80_guess  EQ_CDECL(void, 0x10007A80, (int))
#define eqmain_start_fn_sprintf_guess   EQ_CDECL(int, 0x10034117, (char *, char *, char *))
#define eqmain_start_entry_guess(idx)   EQ_METHOD(1, int, 0x10025C00, (int, int), g_eqmain_this_guess)(idx)
#define eqmain_start_string_guess(idx)  eqmain_fn_10025950_method_guess(eqmain_start_entry_guess(idx))()
#define EQMAIN_START_LABEL_STYLE        0x21A3C

// FUNCTION: EQMAIN 0x10011de0
// Front-end form of up to 11 text fields, one drop-down and three either/or box pairs; OK validates and saves
// The start screen: builds the labels, edit fields and check boxes, then loops on messages until a button or Escape ends it.
// Returns 0xD when the fields are accepted, 1 when cancelled.
short __stdcall EQMain_RunFieldFormScreen_guess(void)
{
    EQMainWidget_guess *fields[13];
    EQMainWidget_guess *boxE;
    EQMainWidget_guess *boxC;
    int hwndSecond;
    int hwndFirst;
    EQMainWidget_guess *boxF;
    int result;
    EQMainWidget_guess *boxD;
    EQMainWidget_guess *boxB;
    bool done;
    EQMainWidget_guess *boxA;
    int caption;
    int match;
    int oldResult;
    short index;
    int *msg;
    EQMainWidget_guess *widget;
    EQMainWidget_guess *clicked;
    EQMainWidget_guess **slot;
    int count;
    unsigned int value;

    done = false;
    result = 7;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, eqmain_str_news_guess, 0x10);
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, eqmain_fn_100210f0_guess(g_eqmain_start_bitmap_guess), 6, 0);
    hwndFirst = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x14, 1);
    hwndSecond = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x18, 1);
    caption = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_eqmain_text_5848_guess, 0x19, 0);
    caption = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_upg_text_b_guess, caption, 0);
    eqmain_fn_100017b0_guess(hwndFirst, 0, g_eqmain_text_5848_guess, caption, 0);
    eqmain_fn_100017b0_guess(hwndSecond, 0, g_upg_text_b_guess, caption, 0);
    memset(fields, 0, sizeof(fields));
    eqmain_start_fn_10011b60_guess(0xf0, 0x92);
    eqmain_start_fn_100118f0_guess(0x37, 0x4c, 0xf0, 0x4c, 0x64, eqmain_start_string_guess(0), (int *)&fields[0], 0x20, eqmain_fn_10021110_guess(g_eqmain_start_field_0_guess));
    eqmain_start_fn_100118f0_guess(0x15e, 0x4c, 0x19a, 0x4c, 0x64, eqmain_start_string_guess(1), (int *)&fields[1], 0x20, eqmain_fn_10021110_guess(g_eqmain_start_field_1_guess));
    if (eqmain_fn_10021110_guess(g_eqmain_start_field_2_guess) != 0) {
        eqmain_start_fn_100118f0_guess(0x204, 0x4c, 0x218, 0x4c, 0x16, eqmain_start_string_guess(2), (int *)&fields[2], 3, eqmain_fn_10021110_guess(g_eqmain_start_field_2_guess));
    }
    eqmain_fn_10011970_guess(0xf0, 0x64, eqmain_start_string_guess(3), (int *)&fields[3], 0x80, eqmain_fn_10021110_guess(g_eqmain_start_field_3_guess));
    eqmain_fn_10011970_guess(0xf0, 0x7c, eqmain_start_string_guess(4), (int *)&fields[4], 0x40, eqmain_fn_10021110_guess(g_eqmain_start_field_4_guess));
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x94, EQMAIN_START_LABEL_STYLE, eqmain_start_string_guess(5), 0, 0);
    eqmain_fn_10011970_guess(0xf0, 0xac, eqmain_start_string_guess(6), (int *)&fields[6], 0x20, eqmain_fn_10021110_guess(g_eqmain_start_field_6_guess));
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0xc4, EQMAIN_START_LABEL_STYLE, eqmain_start_string_guess(7), 0, 0);
    eqmain_fn_10011970_guess(0xf0, 0xdc, eqmain_start_string_guess(8), (int *)&fields[8], 0x28, eqmain_fn_10021110_guess(g_eqmain_start_field_8_guess));
    eqmain_fn_10011970_guess(0xf0, 0xf4, eqmain_start_string_guess(9), (int *)&fields[9], 0x28, eqmain_fn_10021110_guess(g_eqmain_start_field_9_guess));
    eqmain_fn_10011970_guess(0xf0, 0x10c, eqmain_start_string_guess(10), (int *)&fields[10], 0x80, eqmain_fn_10021110_guess(g_eqmain_start_field_10_guess));
    eqmain_fn_10008910_guess(fields[0]);
    eqmain_fn_10011970_guess(0xf0, 0x13c, eqmain_start_string_guess(11), (int *)&fields[12], 0xc, eqmain_fn_10021110_guess(g_eqmain_start_field_12_guess));
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x154, EQMAIN_START_LABEL_STYLE, eqmain_start_string_guess(12), 0, 0);
    boxA = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0xf0, 0x154, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, g_eqmain_flag_5b34_guess == 0, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_a_guess, 0, 0);
    boxB = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0x154, 0x154, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, 0, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_b_guess, 0, 0);
    eqmain_fn_10011970_guess(0xf0, 0x124, eqmain_start_string_guess(13), (int *)&fields[11], 0x40, eqmain_fn_10021110_guess(g_eqmain_start_field_11_guess));
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x16c, EQMAIN_START_LABEL_STYLE, eqmain_start_string_guess(14), 0, 0);
    match = 0;
    if (eqmain_fn_10033730_guess(g_eqmain_start_field_14_guess, g_eqmain_start_const_a0_guess) == 0) {
        match = 1;
    }
    boxC = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0xf0, 0x16c, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, match == 0, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_c_guess, 0, 0);
    boxD = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0x154, 0x16c, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, match, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_d_guess, 0, 0);
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x184, EQMAIN_START_LABEL_STYLE, eqmain_start_string_guess(15), 0, 0);
    match = 0;
    if (eqmain_fn_10033730_guess(g_eqmain_start_field_15_guess, g_eqmain_start_const_a0_guess) == 0) {
        match = 1;
    }
    boxE = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0xf0, 0x184, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, match == 0, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_c_guess, 0, 0);
    boxF = (EQMainWidget_guess *)eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0x154, 0x184, eqmain_str_chekbox1_guess, eqmain_str_chekbox2_guess, match, EQMAIN_START_LABEL_STYLE, g_eqmain_start_label_d_guess, 0, 0);
    g_eqmain_start_record_guess = (EQMainWidget_guess *)eqmain_fn_10005fd0_guess(g_eqmain_ceb8_guess, 0xf0, 0xc4, 0, eqmain_fn_10021110_guess(g_eqmain_start_field_7_guess), eqmain_start_entry_guess(0x2e), 1, 0);
    if (strlen(g_eqmain_start_field_7_guess) != 0) {
        value = eqmain_fn_100061f0_guess((int)g_eqmain_start_record_guess, eqmain_fn_10021110_guess(g_eqmain_start_field_7_guess));
        eqmain_fn_10006510_guess((int *)g_eqmain_start_record_guess, 0x10, (short)value);
    }
    else if (g_eqmain_flag_5b34_guess != 0) {
        eqmain_fn_10006510_guess((int *)g_eqmain_start_record_guess, 0x10, 0);
    }
    else {
        value = eqmain_fn_100061f0_guess((int)g_eqmain_start_record_guess, g_eqmain_str_47f9c_guess);
        eqmain_fn_10006510_guess((int *)g_eqmain_start_record_guess, 0x10, (short)value);
    }
    eqmain_fn_10011c90_guess(0x10, 0xf0, 0x94, eqmain_fn_10021110_guess(g_eqmain_start_field_5_guess));
    g_eqmain_busy_guess = 1;
    eqmain_fn_10003a00_guess();
    if (g_eqmain_flag_5b34_guess != 0) {
        eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_start_text_d_guess, g_str_888_guess, (char *)0);
    }
    for (;;) {
        msg = eqmain_fn_10003bd0_guess();
        if (msg != 0) {
            switch (*msg) {
            case 6:
                if (msg[2] == g_eqmain_ret_obj_guess) {
                    eqmain_fn_10008910_guess(fields[4]);
                }
                else {
                    index = 0;
                    do {
                        if (msg[2] == (int)fields[index]) {
                            if (index < 1) {
                                eqmain_fn_10008910_guess(fields[12]);
                                break;
                            }
                            if (index == 6) {
                                if (g_eqmain_result_guess == 2) {
                                    eqmain_fn_10008910_guess((void *)g_eqmain_ret_obj_guess);
                                    break;
                                }
                            retreat:
                                index = index - 1;
                            }
                            else if (index == 8) {
                                goto retreat;
                            }
                            eqmain_fn_10008910_guess(fields[index - 1]);
                            break;
                        }
                        index = index + 1;
                    } while (index < 0xd);
                }
                break;
            case 0:
            case 7:
                if (msg[2] == g_eqmain_ret_obj_guess) {
                    eqmain_fn_10008910_guess(fields[6]);
                }
                else {
                    index = 0;
                    do {
                        if (msg[2] == (int)fields[index]) {
                            widget = fields[0];
                            if (index < 0xc) {
                                if (index == 4) {
                                    widget = (EQMainWidget_guess *)g_eqmain_ret_obj_guess;
                                    if (g_eqmain_result_guess == 2) {
                                        goto callHandler;
                                    }
                                advance:
                                    index = index + 1;
                                }
                                else if (index == 6) {
                                    goto advance;
                                }
                                widget = fields[index + 1];
                            }
                        callHandler:
                            eqmain_fn_10008910_guess(widget);
                            break;
                        }
                        index = index + 1;
                    } while (index < 0xd);
                    if (msg[2] == hwndSecond) {
                        if (eqmain_fn_10033730_guess(strcpy(g_eqmain_start_field_5_guess, eqmain_start_fn_100211b0_guess((char *)EQ_STDCALL(int, 0x10011C00, (void))())), g_eqmain_start_const_8c_guess) == 0 && g_eqmain_flag_5b34_guess == 0) {
                            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, strcpy(g_eqmain_copybuf_guess, g_eqmain_start_text_c_guess), g_str_888_guess, (char *)0);
                        }
                        else {
                            strcpy(g_eqmain_start_field_0_guess, eqmain_start_fn_100211b0_guess(fields[0]->text));
                            strcpy(g_eqmain_start_field_1_guess, eqmain_start_fn_100211b0_guess(fields[1]->text));
                            strcpy(g_eqmain_start_field_2_guess, eqmain_start_fn_100211b0_guess(fields[2]->text));
                            strcpy(g_eqmain_start_field_3_guess, eqmain_start_fn_100211b0_guess(fields[3]->text));
                            strcpy(g_eqmain_start_field_4_guess, eqmain_start_fn_100211b0_guess(fields[4]->text));
                            strcpy(g_eqmain_start_field_6_guess, eqmain_start_fn_100211b0_guess(fields[6]->text));
                            strcpy(g_eqmain_start_field_7_guess, eqmain_start_fn_100211b0_guess(eqmain_fn_100061d0_guess((int)g_eqmain_start_record_guess, g_eqmain_start_record_guess->index)));
                            strcpy(g_eqmain_start_field_8_guess, eqmain_start_fn_100211b0_guess(fields[8]->text));
                            strcpy(g_eqmain_start_field_9_guess, eqmain_start_fn_100211b0_guess(fields[9]->text));
                            strcpy(g_eqmain_start_field_10_guess, eqmain_start_fn_100211b0_guess(fields[10]->text));
                            strcpy(g_eqmain_start_field_11_guess, eqmain_start_fn_100211b0_guess(fields[11]->text));
                            strcpy(g_eqmain_start_field_12_guess, eqmain_start_fn_100211b0_guess(fields[12]->text));
                            if (boxA->checked == 0) {
                                if (boxB->checked == 0) {
                                    strcpy(g_eqmain_start_field_13_guess, g_eqmain_str_src_guess);
                                }
                                else {
                                    strcpy(g_eqmain_start_field_13_guess, g_eqmain_start_const_94_guess);
                                }
                            }
                            else {
                                strcpy(g_eqmain_start_field_13_guess, g_eqmain_start_const_98_guess);
                            }
                            strcpy(g_eqmain_start_field_14_guess, g_eqmain_start_const_90_guess);
                            if (boxD->checked != 0) {
                                strcpy(g_eqmain_start_field_14_guess, g_eqmain_start_const_a0_guess);
                            }
                            strcpy(g_eqmain_start_field_15_guess, g_eqmain_start_const_90_guess);
                            if (boxF->checked != 0) {
                                strcpy(g_eqmain_start_field_15_guess, g_eqmain_start_const_a0_guess);
                            }
                            if (eqmain_start_fn_100113c0_guess() != 0) {
                                if (g_eqmain_flag_5b34_guess == 0) {
                                    eqmain_start_fn_sprintf_guess(g_eqmain_copybuf_guess, g_eqmain_start_format_guess, eqmain_fn_10021110_guess(g_eqmain_start_field_10_guess));
                                    if (eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_eqmain_start_text_b_guess, g_eqmain_start_text_a_guess) == 0) {
                                        break;
                                    }
                                }
                                result = 0xd;
                                goto finish;
                            }
                            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
                        }
                    }
                    else if (msg[2] == hwndFirst) {
                        goto cancel;
                    }
                }
                break;
            case 1:
                clicked = (EQMainWidget_guess *)msg[2];
                if (clicked == g_eqmain_start_record_guess && g_eqmain_start_record_guess->checked == 0) {
                    oldResult = g_eqmain_result_guess;
                    eqmain_fn_10011c30_guess();
                    if (g_eqmain_result_guess != oldResult) {
                        eqmain_fn_10011bc0_guess();
                        eqmain_fn_10011c90_guess(0x10, 0xf0, 0x94, eqmain_fn_10021110_guess(strcpy(g_eqmain_start_field_5_guess, g_eqmain_str_src_guess)));
                    }
                }
                else {
                    if (clicked == boxA || clicked == boxB) {
                        eqmain_start_fn_10001a60_guess(boxA, boxB, clicked);
                    }
                    clicked = (EQMainWidget_guess *)msg[2];
                    if (clicked == boxC || clicked == boxD) {
                        eqmain_start_fn_10001a60_guess(boxC, boxD, clicked);
                    }
                    clicked = (EQMainWidget_guess *)msg[2];
                    if (clicked == boxE || clicked == boxF) {
                        eqmain_start_fn_10001a60_guess(boxE, boxF, clicked);
                    }
                }
                break;
            case 4:
                if (*(char *)(msg + 2) == 0x1b) {
                    eqmain_fn_10003a00_guess();
                cancel:
                    eqmain_start_fn_100253b0_guess();
                    result = 1;
                finish:
                    done = true;
                }
                break;
            }
            eqmain_fn_10033568_guess(msg);
        }
        if (done) {
            g_eqmain_busy_guess = 0;
            eqmain_fn_10001700_guess(hwndFirst);
            eqmain_fn_10001700_guess(hwndSecond);
            eqmain_fn_100019e0_guess(boxA);
            eqmain_fn_100019e0_guess(boxB);
            eqmain_fn_100019e0_guess(boxC);
            eqmain_fn_100019e0_guess(boxD);
            eqmain_fn_100019e0_guess(boxE);
            eqmain_fn_100019e0_guess(boxF);
            eqmain_fn_10011bc0_guess();
            eqmain_start_fn_10007a80_guess(g_eqmain_rec_guess);
            g_eqmain_rec_guess = 0;
            if (g_eqmain_start_record_guess != 0) {
                eqmain_fn_10006130_guess((int)g_eqmain_start_record_guess);
                g_eqmain_start_record_guess = 0;
            }
            slot = fields;
            for (count = 0xd; count != 0; count--) {
                if (*slot != 0) {
                    eqmain_fn_10008a00_guess((int)*slot);
                }
                slot++;
            }
            eqmain_fn_100039e0_guess(0);
            return (short)result;
        }
    }
}

#define g_news_bmp_guess     EQ_ARRAY(char, 0x10046240)
#define g_eqmain_reg_bitmap_register_guess EQ_ARRAY(char, 0x10047FA4)
#define g_eqmain_reg_name_a_guess          EQ_GLOBAL(int, 0x10055848)
#define g_eqmain_reg_name_b_guess          EQ_GLOBAL(int, 0x10055854)
#define g_eqmain_reg_name_c_guess          EQ_GLOBAL(int, 0x10055858)
#define g_eqmain_reg_this_guess            EQ_GLOBAL(int, 0x1012C774)
#define g_eqmain_drive_base_guess            EQ_ARRAY(char, 0x1004CE40)
#define g_eqmain_reg_text_a_guess          EQ_ARRAY(char, 0x1012BBD9)
#define g_eqmain_reg_text_b_guess          EQ_ARRAY(char, 0x1012BC19)
#define g_eqmain_reg_text_c_guess          EQ_ARRAY(char, 0x1012BC39)
#define g_eqmain_reg_text_d_guess          EQ_ARRAY(char, 0x1012BCAF)
#define g_eqmain_reg_text_e_guess          EQ_ARRAY(char, 0x1012BCEF)
#define g_eqmain_reg_text_f_guess          EQ_ARRAY(char, 0x1012BD2F)
#define g_eqmain_reg_text_g_guess          EQ_ARRAY(char, 0x1012BD6F)
#define g_eqmain_reg_text_h_guess          EQ_ARRAY(char, 0x1012BDAF)
#define g_eqmain_reg_text_i_guess          EQ_ARRAY(char, 0x1012BDEF)
#define g_eqmain_reg_text_j_guess          EQ_ARRAY(char, 0x1012BE2F)
#define g_eqmain_reg_value_guess           EQ_ARRAY(char, 0x1012BE6F)
#define g_eqmain_reg_value_default_guess   EQ_GLOBAL(int, 0x101258D8)

// FUNCTION: EQMAIN 0x10012c90
// __stdcall form screen; returns the next screen state.
// Builds three text fields, seven drop-down lists (three filled with defaults when their saved text is blank) and
// 16 toggles initialised from the saved decimal bit mask.
// Then loops on the event queue 0x10003bd0: a switch on the event type moves focus (cases 0/7 and 6),
// rewrites the mask from the toggles (case 1), and finishes on case 4 or the close paths (states 1, 0xc, 0xe).
// The finished block frees all controls and the three edit fields.
short __stdcall EQMain_RunRegisterScreen_guess(void)
{
    int *pHVar1;
    bool finished;
    char temp;
    char *pcVar4;
    int pHVar5;
    int pHVar6;
    int pHVar7;
    char *pCVar8;
    int *rolloutD;
    int *rolloutE;
    int *rolloutF;
    int *rolloutG;
    unsigned int value;
    int *rolloutH;
    int *rolloutI;
    int *rolloutJ;
    void *labelEntry;
    int *_Memory;
    short bitIndex;
    unsigned int mask;
    char *unusedPtr;
    int *controlPtr;
    int **ppHVar22;
    unsigned char pairedEntries;
    int tmp;
    size_t maxLen;
    int extraArg;
    unsigned char extraFlag;
    unsigned int imageY;
    unsigned int savedMask;
    short nextState;
    int *editFieldA;
    int *editFieldB;
    int *editFieldC;
    int toggleControls[16];

    finished = false;
    eqmain_fn_10007bf0_guess(
        g_eqmain_ceb8_guess, g_news_bmp_guess, 0x10);
    imageY = 0;
    tmp = 6;
    pcVar4 = EQ_CDECL(char *, 0x100210F0, (char *))(g_eqmain_reg_bitmap_register_guess);
    EQ_CDECL(void, 0x100056C0, (int, char *, int, unsigned int))(
        g_eqmain_ceb8_guess, pcVar4, tmp, imageY);
    pHVar5 = EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))(
        g_eqmain_ceb8_guess, 0, 0, 0x14, 1);
    pHVar6 = EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))(
        g_eqmain_ceb8_guess, 0, 0, 0x18, 1);
    pHVar7 = EQ_CDECL(int, 0x10001520, (int, int, int, unsigned char, int))(
        g_eqmain_ceb8_guess, 0x186, 0, 0x10, 1);
    EQ_CDECL(int, 0x100080B0, (int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x64, 0x22, (char *)g_eqmain_reg_name_a_guess);
    EQ_CDECL(int, 0x100080B0, (int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x64, 0x22, (char *)g_eqmain_reg_name_b_guess);
    EQ_CDECL(int, 0x100080B0, (int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x64, 0x22, (char *)g_eqmain_reg_name_c_guess);
    EQ_CDECL(void, 0x100017B0, (int, int, char *))(
        pHVar5, 0, (char *)g_eqmain_reg_name_a_guess);
    EQ_CDECL(void, 0x100017B0, (int, int, char *))(
        pHVar6, 0, (char *)g_eqmain_reg_name_b_guess);
    EQ_CDECL(void, 0x100017B0, (int, int, char *))(
        pHVar7, 0, (char *)g_eqmain_reg_name_c_guess);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_a_guess);
    ppHVar22 = &editFieldA;
    maxLen = 0x40;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x10);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    eqmain_fn_10011970_guess(0xf0, 0x4c, pCVar8, (int *)ppHVar22, maxLen, pcVar4);
    eqmain_fn_10008910_guess(editFieldA);
    extraFlag = 0;
    pairedEntries = 0;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x11);
    rolloutD = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0x7c, 0, g_eqmain_drive_base_guess, tmp, pairedEntries, extraFlag);
    tmp = -1;
    pcVar4 = g_eqmain_reg_text_d_guess;
    do {
        if (tmp == 0)
            break;
        tmp = tmp + -1;
        temp = *pcVar4;
        pcVar4 = pcVar4 + 1;
    } while (temp != '\0');
    if (tmp == -2) {
        temp = EQ_STDCALL(char, 0x10024420, (void))();
        EQ_CDECL(void, 0x10006250, (int *, short))(rolloutD, (short)temp);
    } else {
        pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_d_guess);
        EQ_CDECL(void, 0x10006270, (int *, char *))(rolloutD, pcVar4);
    }
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x11);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0x7c, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 0;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x12);
    rolloutE = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 100, 0, g_eqmain_drive_base_guess, tmp, pairedEntries, extraFlag);
    tmp = -1;
    pcVar4 = g_eqmain_reg_text_e_guess;
    do {
        if (tmp == 0)
            break;
        tmp = tmp + -1;
        temp = *pcVar4;
        pcVar4 = pcVar4 + 1;
    } while (temp != '\0');
    if (tmp == -2) {
        EQ_CDECL(void, 0x10006250, (int *, short))(
            rolloutE, (short)g_eqmain_reg_value_default_guess);
    } else {
        pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_e_guess);
        EQ_CDECL(void, 0x10006270, (int *, char *))(rolloutE, pcVar4);
    }
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x12);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 100, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 0;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x13);
    rolloutF = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0x94, 0, g_eqmain_drive_base_guess, tmp, pairedEntries, extraFlag);
    tmp = -1;
    pcVar4 = g_eqmain_reg_text_f_guess;
    do {
        if (tmp == 0)
            break;
        tmp = tmp + -1;
        temp = *pcVar4;
        pcVar4 = pcVar4 + 1;
    } while (temp != '\0');
    if (tmp == -2) {
        tmp = EQ_STDCALL(int, 0x100248A0, (void))();
        EQ_CDECL(void, 0x10006250, (int *, short))(rolloutF, (short)tmp);
    } else {
        pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_f_guess);
        EQ_CDECL(void, 0x10006270, (int *, char *))(rolloutF, pcVar4);
    }
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x13);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0x94, 0x21a3c, pCVar8);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_b_guess);
    ppHVar22 = &editFieldB;
    maxLen = 0x20;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x14);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    eqmain_fn_10011970_guess(0xf0, 0x124, pCVar8, (int *)ppHVar22, maxLen, pcVar4);
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x15);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0xac, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 1;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x15);
    rolloutG = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0xac, 0, g_eqmain_drive_base_guess, tmp, pairedEntries, extraFlag);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_g_guess);
    value = eqmain_fn_100061f0_guess((int)rolloutG, pcVar4);
    eqmain_fn_10006510_guess(rolloutG, 0xe, (short)value);
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x16);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0xc4, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 0;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x16);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_h_guess);
    rolloutH = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0xc4, 0, pcVar4, tmp, pairedEntries, extraFlag);
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x17);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0xdc, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 1;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x17);
    rolloutI = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0xdc, 0, g_eqmain_drive_base_guess, tmp, pairedEntries, extraFlag);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_i_guess);
    value = eqmain_fn_100061f0_guess((int)rolloutI, pcVar4);
    eqmain_fn_10006510_guess(rolloutI, 0xe, (short)value);
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x18);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0xf4, 0x21a3c, pCVar8);
    extraFlag = 0;
    pairedEntries = 0;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x18);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_j_guess);
    rolloutJ = EQ_CDECL(int *, 0x10005FD0,
        (int, int, int, unsigned char, char *, int, unsigned char, unsigned char))(
            g_eqmain_ceb8_guess, 0xf0, 0xf4, 0, pcVar4, tmp, pairedEntries, extraFlag);
    pcVar4 = EQ_CDECL(char *, 0x10021110, (char *))(g_eqmain_reg_text_c_guess);
    ppHVar22 = &editFieldC;
    maxLen = 0x40;
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x19);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    eqmain_fn_10011970_guess(0xf0, 0x10c, pCVar8, (int *)ppHVar22, maxLen, pcVar4);
    controlPtr = toggleControls;
    for (tmp = 0x10; tmp != 0; tmp = tmp + -1) {
        *controlPtr = 0;
        controlPtr = controlPtr + 1;
    }
    tmp = eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x1a);
    pCVar8 = eqmain_fn_10025950_guess(tmp);
    EQ_CDECL(int *, 0x100082C0, (int, int, int, int, char *))(
        g_eqmain_ceb8_guess, 0x37, 0x13c, 0x21a3c, pCVar8);
    value = EQ_CDECL(int, 0x1003507E, (char *))(g_eqmain_reg_value_guess);
    extraArg = 0x18;
    tmp = 0x78;
    mask = value;
    labelEntry = (void *)eqmain_fn_10025c00_guess(g_eqmain_reg_this_guess, 0, 0x1a);
    EQ_CDECL(void, 0x100119A0, (int *, int, int, void *, int, int, unsigned int))(
        toggleControls, 0xf0, 0x13c, labelEntry, tmp, extraArg, mask);
    g_eqmain_busy_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    savedMask = value;
    do {
        _Memory = EQ_STDCALL(int *, 0x10003BD0, (void))();
        if (_Memory != (int *)0) {
            switch (*_Memory) {
            case 0:
            case 7:
                pHVar1 = (int *)_Memory[2];
                if (pHVar1 == editFieldA) {
                    eqmain_fn_10008910_guess(editFieldC);
                } else if (pHVar1 == editFieldB) {
                    eqmain_fn_10008910_guess(editFieldA);
                } else if (pHVar1 == editFieldC) {
                    eqmain_fn_10008910_guess(editFieldB);
                } else if ((int)pHVar1 == pHVar6) {
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldA[8]);
                    strcpy(g_eqmain_reg_text_a_guess, pcVar4);
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldB[8]);
                    strcpy(g_eqmain_reg_text_b_guess, pcVar4);
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldC[8]);
                    strcpy(g_eqmain_reg_text_c_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutD, (int)*(short *)(rolloutD + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_d_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutE, (int)*(short *)(rolloutE + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_e_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutF, (int)*(short *)(rolloutF + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_f_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutG, (int)*(short *)(rolloutG + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_g_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutH, (int)*(short *)(rolloutH + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_h_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutI, (int)*(short *)(rolloutI + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_i_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutJ, (int)*(short *)(rolloutJ + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    finished = true;
                    strcpy(g_eqmain_reg_text_j_guess, pcVar4);
                    nextState = 0xe;
                    value = savedMask;
                } else if ((int)pHVar1 == pHVar7) {
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldA[8]);
                    strcpy(g_eqmain_reg_text_a_guess, pcVar4);
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldB[8]);
                    strcpy(g_eqmain_reg_text_b_guess, pcVar4);
                    pcVar4 = eqmain_start_fn_100211b0_guess((char *)editFieldC[8]);
                    strcpy(g_eqmain_reg_text_c_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutD, (int)*(short *)(rolloutD + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_d_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutE, (int)*(short *)(rolloutE + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_e_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutF, (int)*(short *)(rolloutF + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_f_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutG, (int)*(short *)(rolloutG + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_g_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutH, (int)*(short *)(rolloutH + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_h_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutI, (int)*(short *)(rolloutI + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    strcpy(g_eqmain_reg_text_i_guess, pcVar4);
                    pcVar4 = eqmain_fn_100061d0_guess(
                        (int)rolloutJ, (int)*(short *)(rolloutJ + 10));
                    pcVar4 = eqmain_start_fn_100211b0_guess(pcVar4);
                    finished = true;
                    strcpy(g_eqmain_reg_text_j_guess, pcVar4);
                    nextState = 0xc;
                    value = savedMask;
                } else if ((int)pHVar1 == pHVar5) {
                    goto cancelExit;
                }
                break;
            case 1:
                bitIndex = 0;
                do {
                    if (_Memory[2] == toggleControls[bitIndex]) {
                        mask = 1 << ((unsigned char)bitIndex & 0x1f);
                        if ((value & mask) == 0) {
                            value = value | mask;
                        } else {
                            value = value & ~mask;
                        }
                        EQ_CDECL(char *, 0x10035089, (int, char *, int))(
                            value, g_eqmain_reg_value_guess, 10);
                        savedMask = value;
                        break;
                    }
                    bitIndex = bitIndex + 1;
                } while (bitIndex < 0x10);
                break;
            case 4:
                if (*(char *)(_Memory + 2) == '\x1b') {
                    EQ_STDCALL(void, 0x10003A00, (void))();
cancelExit:
                    eqmain_start_fn_100253b0_guess();
                    nextState = 1;
                    finished = true;
                }
                break;
            case 6:
                pHVar1 = (int *)_Memory[2];
                if (pHVar1 == editFieldA) {
                    eqmain_fn_10008910_guess(editFieldB);
                } else if (pHVar1 == editFieldB) {
                    eqmain_fn_10008910_guess(editFieldC);
                } else if (pHVar1 == editFieldC) {
                    eqmain_fn_10008910_guess(editFieldA);
                }
                break;
            }
            EQ_CDECL(void, 0x10033568, (void *))(_Memory);
        }
        if (finished) {
            g_eqmain_busy_guess = 0;
            EQ_CDECL(void, 0x10001700, (void *))((void *)pHVar5);
            EQ_CDECL(void, 0x10001700, (void *))((void *)pHVar6);
            EQ_CDECL(void, 0x10001700, (void *))((void *)pHVar7);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutD);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutE);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutF);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutG);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutH);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutI);
            EQ_CDECL(void, 0x10006130, (void *))(rolloutJ);
            controlPtr = toggleControls;
            tmp = 0x10;
            do {
                if ((void *)*controlPtr != (void *)0) {
                    eqmain_fn_100019e0_guess((void *)*controlPtr);
                }
                controlPtr = controlPtr + 1;
                tmp = tmp + -1;
            } while (tmp != 0);
            EQ_CDECL(void, 0x10008A00, (void *))(editFieldA);
            EQ_CDECL(void, 0x10008A00, (void *))(editFieldB);
            EQ_CDECL(void, 0x10008A00, (void *))(editFieldC);
            EQ_STDCALL(void, 0x100039E0, (void))();
            return nextState;
        }
    } while (true);
}

// EQMAIN globals and callees used by the transaction registration screen.
#define g_eqmain_55850_guess             EQ_GLOBAL(int, 0x10055850)
#define g_eqmain_557cc_tx_guess          EQ_GLOBAL(char *, 0x100557CC)
#define g_eqmain_557c8_tx_guess          EQ_GLOBAL(char *, 0x100557C8)
#define g_eqmain_557d8_guess             EQ_GLOBAL(char *, 0x100557D8)
#define g_eqmain_55a0c_guess             EQ_GLOBAL(char *, 0x10055A0C)
#define g_eqmain_5575c_guess             EQ_GLOBAL(char *, 0x1005575C)
#define g_eqmain_557b8_guess             EQ_GLOBAL(char *, 0x100557B8)
#define g_eqmain_55940_guess             EQ_GLOBAL(char *, 0x10055940)
#define g_eqmain_text_b_guess             EQ_GLOBAL(char *, 0x1005586C)
#define g_eqmain_text_a_guess             EQ_GLOBAL(char *, 0x10055868)
#define g_eqmain_55764_guess             EQ_GLOBAL(char *, 0x10055764)
#define g_eqmain_login_name_buf_guess      EQ_ARRAY(char, 0x10104CD8)
#define g_eqmain_buf_1012b920_guess      EQ_ARRAY(char, 0x1012B920)
#define g_eqmain_buf_1012bc79_guess      EQ_ARRAY(char, 0x1012BC79)
#define g_eqmain_buf_1012b934_guess      EQ_ARRAY(char, 0x1012B934)
#define g_eqmain_buf_1012bf6f_guess      EQ_ARRAY(char, 0x1012BF6F)
#define g_eqmain_buf_1012bf83_guess      EQ_ARRAY(char, 0x1012BF83)
#define g_eqmain_buf_1012bcab_guess      EQ_ARRAY(char, 0x1012BCAB)
#define g_eqmain_buf_1012beef_guess      EQ_ARRAY(char, 0x1012BEEF)
#define g_eqmain_buf_1004d1b0_guess      EQ_ARRAY(char, 0x1004D1B0)
#define eqmain_str_10046240_guess        ((char *)0x10046240)
#define eqmain_str_10047fa4_guess        ((char *)0x10047FA4)
#define eqmain_str_10047fb4_guess        ((char *)0x10047FB4)
#define eqmain_str_10047fb8_guess        ((char *)0x10047FB8)
#define eqmain_str_10047fbc_guess        ((char *)0x10047FBC)
#define eqmain_str_10047fc0_guess        ((char *)0x10047FC0)
#define eqmain_fn_100088f0_guess      EQ_CDECL(void, 0x100088F0, (int))
#define eqmain_fn_100115f0_tx_guess      EQ_STDCALL(unsigned int, 0x100115F0, (void))
#define eqmain_fn_100256c0_tx_guess      EQ_STDCALL(unsigned int, 0x100256C0, (void))
#define eqmain_fn_10001a60_tx_guess      EQ_CDECL(void, 0x10001A60, (int *, int *, int *))
#define eqmain_atoi_guess      EQ_CDECL(int, 0x1003507E, (char *))
#define eqmain_fn_10035089_guess      EQ_CDECL(char *, 0x10035089, (int, char *, int))
#define eqmain_fn_10003bd0_tx_guess      EQ_STDCALL(int *, 0x10003BD0, (void))
#define eqmain_fn_10011a50_tx_guess      EQ_STDCALL(unsigned int, 0x10011A50, (void))
#define eqmain_fn_10008a00_tx_guess      EQ_CDECL(void, 0x10008A00, (void *))

// FUNCTION: EQMAIN 0x10013900
// __stdcall form screen; returns the next screen state (7 on abort or error).
// Builds five edit fields, two yes/no check box pairs, 16 option cells and a text block, then loops: polls
// 0x10011a50 (result 0x1d aborts) and takes events from the queue 0x10003bd0.
// A switch on the event type moves focus between fields (cases 0/7 forward, case 6 backward), handles the cells
// on case 1, and finishes on case 4 after the entries are saved.
// The cleanup tail frees every control and edit field.
unsigned short __stdcall EQMain_RunTxRegisterScreen_guess(void)
{
    int *pHVar1;
    int *clickedBox;
    bool requestPending;
    bool done;
    char ch;
    char *pcVar6;
    int *pHVar7;
    int *pHVar8;
    int *pHVar9;
    char *pCVar10;
    void *entry;
    int *firstYesBox;
    int *firstNoBox;
    int *secondYesBox;
    int *secondNoBox;
    int *_Memory;
    short cellIndex;
    unsigned int tmp;
    unsigned int checkedMask;
    unsigned int dwordCount;
    int *cellPtr;
    int **ppHVar21;
    char *dst;
    bool firstIsNo;
    bool secondNotYes;
    int value;
    int cellHeight;
    size_t maxLen;
    unsigned int uVar28;
    unsigned short nextState;
    int *textBlock;
    int *editFields[5];
    int optionCells[16];

    done = false;
    requestPending = false;
    textBlock = (int *)0;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, eqmain_str_10046240_guess, 0x10);
    uVar28 = 0;
    value = 6;
    pcVar6 = eqmain_fn_100210f0_guess(eqmain_str_10047fa4_guess);
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, pcVar6, value, uVar28);
    pHVar7 = (int *)eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x14, 1);
    pHVar8 = (int *)eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x18, 1);
    pHVar9 = (int *)eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0x186, 0, 0x10, 1);
    value = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22,
        (char *)g_eqmain_reg_name_a_guess, 0x19, 0);
    value = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22,
        (char *)g_eqmain_55850_guess, value, 0);
    value = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22,
        (char *)g_eqmain_reg_name_c_guess, value, 0);
    eqmain_fn_100017b0_guess((int)pHVar7, 0, (char *)g_eqmain_reg_name_a_guess, value, 0);
    eqmain_fn_100017b0_guess((int)pHVar8, 0, (char *)g_eqmain_55850_guess, value, 0);
    eqmain_fn_100017b0_guess((int)pHVar9, 0, (char *)g_eqmain_reg_name_c_guess, value, 0);

    tmp = 0xffffffff;
    g_eqmain_code_guess = 0x1b;
    pcVar6 = g_eqmain_drive_base_guess;
    do {
        dst = pcVar6;
        if (tmp == 0) break;
        tmp = tmp - 1;
        dst = pcVar6 + 1;
        ch = *pcVar6;
        pcVar6 = dst;
    } while (ch != '\0');
    tmp = ~tmp;
    editFields[0] = (int *)0;
    editFields[1] = (int *)0;
    pcVar6 = dst - tmp;
    dst = g_eqmain_buf_1004d1b0_guess;
    for (checkedMask = tmp >> 2; checkedMask != 0; checkedMask = checkedMask - 1) {
        *(unsigned int *)dst = *(unsigned int *)pcVar6;
        pcVar6 = pcVar6 + 4;
        dst = dst + 4;
    }
    editFields[2] = (int *)0;
    for (tmp = tmp & 3; tmp != 0; tmp = tmp - 1) {
        *dst = *pcVar6;
        pcVar6 = pcVar6 + 1;
        dst = dst + 1;
    }
    cellPtr = optionCells;
    for (value = 0x10; value != 0; value = value - 1) {
        *cellPtr = 0;
        cellPtr = cellPtr + 1;
    }
    editFields[3] = (int *)0;
    editFields[4] = (int *)0;
    value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x1b);
    pCVar10 = eqmain_fn_10025950_method_guess(value)();
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x4c, 0x21a3c, pCVar10, 0, 0);
    checkedMask = (unsigned int)eqmain_atoi_guess(g_eqmain_buf_1012beef_guess);
    cellHeight = 0x18;
    value = 0x78;
    tmp = checkedMask;
    entry = (void *)eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
    eqmain_fn_100119a0_guess((void **)optionCells, 0xf0, 0x4c, entry, value, cellHeight, tmp);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
    pCVar10 = eqmain_fn_10025950_method_guess(value)();
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x99, 0x21a3c, pCVar10, 0, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
    pCVar10 = eqmain_fn_10025950_method_guess(value)();
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0xa7, 0x21a3c, pCVar10, 0, 0);
    g_eqmain_code_guess = 0x30;
    value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x30);
    pCVar10 = eqmain_fn_10025950_method_guess(value)();
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0xc4, 0x21a3c, pCVar10, 0, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
    pCVar10 = eqmain_fn_10025950_method_guess(value)();
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0xd2, 0x21a3c, pCVar10, 0, 0);
    firstIsNo = g_eqmain_buf_1012bcab_guess[0] == 'N';
    secondNotYes = g_eqmain_buf_1012bcab_guess[1] != 'Y';
    firstYesBox = EQ_CDECL(int *, 0x100018F0,
        (int, int, int, char *, char *, int, int, char *, int, int))(
        g_eqmain_ceb8_guess, 0x172, 0xa7, eqmain_str_chekbox1_guess,
        eqmain_str_chekbox2_guess, (int)(firstIsNo == false), 0x21a3c,
        g_eqmain_557cc_tx_guess, 0, 0);
    firstNoBox = EQ_CDECL(int *, 0x100018F0,
        (int, int, int, char *, char *, int, int, char *, int, int))(
        g_eqmain_ceb8_guess, 0x1cc, 0xa7, eqmain_str_chekbox1_guess,
        eqmain_str_chekbox2_guess, (int)firstIsNo, 0x21a3c,
        g_eqmain_557c8_tx_guess, 0, 0);
    secondYesBox = EQ_CDECL(int *, 0x100018F0,
        (int, int, int, char *, char *, int, int, char *, int, int))(
        g_eqmain_ceb8_guess, 0x172, 0xd2, eqmain_str_chekbox1_guess,
        eqmain_str_chekbox2_guess, (int)(secondNotYes == false), 0x21a3c,
        g_eqmain_557cc_tx_guess, 0, 0);
    secondNoBox = EQ_CDECL(int *, 0x100018F0,
        (int, int, int, char *, char *, int, int, char *, int, int))(
        g_eqmain_ceb8_guess, 0x1cc, 0xd2, eqmain_str_chekbox1_guess,
        eqmain_str_chekbox2_guess, (int)secondNotYes, 0x21a3c,
        g_eqmain_557c8_tx_guess, 0, 0);

    g_eqmain_code_guess = 0x1e;
    if (g_eqmain_flag_5b34_guess == '\0') {
        pcVar6 = eqmain_fn_10021110_guess(g_eqmain_buf_1012b920_guess);
        ppHVar21 = editFields;
        maxLen = 0x13;
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_10011970_guess(0xf0, 0xf4, pCVar10, (int *)ppHVar21, maxLen, pcVar6);
        g_eqmain_code_guess = g_eqmain_code_guess + 1;
        eqmain_fn_10008910_guess(editFields[0]);
        pcVar6 = eqmain_fn_10021110_guess(g_eqmain_buf_1012bc79_guess);
        ppHVar21 = editFields + 1;
        maxLen = 0x1a;
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_10011970_guess(0xf0, 0x10c, pCVar10, (int *)ppHVar21, maxLen, pcVar6);
        g_eqmain_code_guess = g_eqmain_code_guess + 1;
        textBlock = eqmain_fn_10008c10_guess(g_eqmain_ceb8_guess, 0x37, 0x124, 0x208,
            0x3c, 10, 0xc81000, g_eqmain_dlg_arg_guess, 0);
        tmp = 0xffffffff;
        pcVar6 = g_eqmain_557d8_guess;
        do {
            dst = pcVar6;
            if (tmp == 0) break;
            tmp = tmp - 1;
            dst = pcVar6 + 1;
            ch = *pcVar6;
            pcVar6 = dst;
        } while (ch != '\0');
        tmp = ~tmp;
        pcVar6 = dst - tmp;
        dst = (char *)g_eqmain_copybuf_guess;
        for (dwordCount = tmp >> 2; dwordCount != 0; dwordCount = dwordCount - 1) {
            *(unsigned int *)dst = *(unsigned int *)pcVar6;
            pcVar6 = pcVar6 + 4;
            dst = dst + 4;
        }
        for (tmp = tmp & 3; tmp != 0; tmp = tmp - 1) {
            *dst = *pcVar6;
            pcVar6 = pcVar6 + 1;
            dst = dst + 1;
        }
        eqmain_fn_10009370_guess(textBlock, g_eqmain_copybuf_guess);
        eqmain_fn_10008d60_guess(textBlock);
        g_eqmain_code_guess = 0x2a;
        pcVar6 = eqmain_fn_10021110_guess(g_eqmain_buf_1012bf6f_guess);
        ppHVar21 = editFields + 2;
        maxLen = 0x10;
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_10011970_guess(0xf0, 0x154, pCVar10, (int *)ppHVar21, maxLen, pcVar6);
        g_eqmain_code_guess = g_eqmain_code_guess + 1;
        pcVar6 = eqmain_fn_10021110_guess(g_eqmain_buf_1012bf83_guess);
        ppHVar21 = editFields + 3;
        maxLen = 0x10;
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_10011970_guess(0xf0, 0x16c, pCVar10, (int *)ppHVar21, maxLen, pcVar6);
        eqmain_fn_100088f0_guess((int)editFields[3]);
        pcVar6 = eqmain_fn_10021110_guess(g_eqmain_buf_1012bf83_guess);
        eqmain_fn_10011970_guess(0xf0, 0x184, g_eqmain_55a0c_guess,
            (int *)(editFields + 4), 0x10, pcVar6);
        eqmain_fn_100088f0_guess((int)editFields[4]);
    } else {
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x1e);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x10c, 0x21a3c, pCVar10, 0, 0);
        pCVar10 = eqmain_fn_10021110_guess(g_eqmain_buf_1012b920_guess);
        eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0xf0, 0x10c, 0x22d69, pCVar10, 0, 0);
        g_eqmain_code_guess = 0x2a;
        value = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x2a);
        pCVar10 = eqmain_fn_10025950_method_guess(value)();
        eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x124, 0x21a3c, pCVar10, 0, 0);
        pCVar10 = eqmain_fn_10021110_guess(g_eqmain_buf_1012bf6f_guess);
        eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0xf0, 0x124, 0x22d69, pCVar10, 0, 0);
    }
    g_eqmain_busy_guess = 1;
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_10003a00_guess();
    do {
        tmp = eqmain_fn_10011a50_tx_guess();
        pCVar10 = g_str_888_guess;
        if (tmp != 0) {
            requestPending = false;
        }
        if (tmp == 0x1d) {
            if (g_eqmain_flag_5b34_guess != '\0') goto abort_screen;
            strcpy(g_eqmain_copybuf_guess, g_eqmain_5575c_guess);
            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess,
                pCVar10, (char *)0);
            goto abort_screen;
        }
        _Memory = eqmain_fn_10003bd0_tx_guess();
        if (_Memory != (int *)0) {
            switch (*_Memory) {
            case 0:
            case 7:
                pHVar1 = (int *)_Memory[2];
                if (pHVar1 == editFields[0]) {
                    eqmain_fn_10008910_guess(editFields[1]);
                } else if (pHVar1 == editFields[1]) {
                    eqmain_fn_10008910_guess(editFields[2]);
                } else if (pHVar1 == editFields[2]) {
                    eqmain_fn_10008910_guess(editFields[3]);
                } else if (pHVar1 == editFields[3]) {
                    eqmain_fn_10008910_guess(editFields[4]);
                } else if (pHVar1 == editFields[4]) {
                    eqmain_fn_10008910_guess(editFields[0]);
                } else {
                    if (pHVar1 == pHVar9) {
                        if (g_eqmain_flag_5b34_guess == '\0') {
                            pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[0][8]);
                            strcpy(g_eqmain_buf_1012b920_guess, pcVar6);
                            pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[1][8]);
                            strcpy(g_eqmain_buf_1012bc79_guess, pcVar6);
                            pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[2][8]);
                            strcpy(g_eqmain_buf_1012bf6f_guess, pcVar6);
                            pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[3][8]);
                            strcpy(g_eqmain_buf_1012bf83_guess, pcVar6);
                            pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[4][8]);
                            strcpy((char *)0x1004D1B0, pcVar6);
                        }
                        nextState = 0xd;
                        goto finish_loop;
                    }
                    if (pHVar1 == pHVar7) {
                        eqmain_start_fn_100253b0_guess();
                        g_eqmain_flag_5b34_guess = '\0';
                        goto go_back;
                    }
                    if ((pHVar1 != pHVar8) || requestPending) break;
                    if (g_eqmain_flag_5b34_guess == '\0') {
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[0][8]);
                        strcpy(g_eqmain_buf_1012b920_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[0][8]);
                        strcpy(g_eqmain_buf_1012b934_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[1][8]);
                        strcpy(g_eqmain_buf_1012bc79_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[2][8]);
                        strcpy(g_eqmain_buf_1012bf6f_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[3][8]);
                        strcpy(g_eqmain_buf_1012bf83_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess((char *)editFields[4][8]);
                        strcpy(g_eqmain_buf_1004d1b0_guess, pcVar6);
                        if (firstNoBox[7] == 0) {
                            if (secondYesBox[7] == 0) {
                                strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fb4_guess);
                            } else {
                                strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fb8_guess);
                            }
                        } else if (secondYesBox[7] == 0) {
                            pcVar6 = eqmain_str_10047fbc_guess;
                            strcpy(g_eqmain_buf_1012bcab_guess, pcVar6);
                        } else {
                            strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fc0_guess);
                        }
                    } else {
                        pcVar6 = eqmain_start_fn_100211b0_guess(g_eqmain_login_name_buf_guess);
                        strcpy(g_eqmain_buf_1012b920_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess(g_eqmain_login_name_buf_guess);
                        strcpy(g_eqmain_buf_1012b934_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess(g_eqmain_login_name_buf_guess);
                        strcpy(g_eqmain_buf_1012bf6f_guess, pcVar6);
                        pcVar6 = eqmain_start_fn_100211b0_guess(g_eqmain_login_pin_buf_guess);
                        strcpy(g_eqmain_buf_1012bf83_guess, pcVar6);
                        if (firstNoBox[7] == 0) {
                            if (secondYesBox[7] == 0) {
                                strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fb4_guess);
                            } else {
                                strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fb8_guess);
                            }
                        } else if (secondYesBox[7] == 0) {
                            pcVar6 = eqmain_str_10047fbc_guess;
                            strcpy(g_eqmain_buf_1012bcab_guess, pcVar6);
                        } else {
                            strcpy(g_eqmain_buf_1012bcab_guess, eqmain_str_10047fc0_guess);
                        }
                    }
                    if (g_eqmain_flag_5b34_guess != '\0') goto submit_form;
                    {
                        tmp = 0xffffffff;
                        pcVar6 = g_eqmain_buf_1012bc79_guess;
                        do {
                            if (tmp == 0) break;
                            tmp = tmp - 1;
                            ch = *pcVar6;
                            pcVar6 = pcVar6 + 1;
                        } while (ch != '\0');
                        if (tmp == 0xfffffffe) {
                            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess,
                                g_eqmain_557b8_guess, g_str_888_guess, (char *)0);
                        } else {
                            tmp = 0xffffffff;
                            pcVar6 = g_eqmain_buf_1012bc79_guess;
                            do {
                                if (tmp == 0) break;
                                tmp = tmp - 1;
                                ch = *pcVar6;
                                pcVar6 = pcVar6 + 1;
                            } while (ch != '\0');
                            if (((~tmp - 1 < 0xb) ||
                                (((g_eqmain_buf_1012bc79_guess[0] != 'K' &&
                                   g_eqmain_buf_1012bc79_guess[0] != 'k') ||
                                  (ch = eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess,
                                      g_eqmain_55940_guess, g_eqmain_text_b_guess,
                                      g_eqmain_text_a_guess), ch != '\0')))))
                                goto submit_form;
                        }
                    }
submit_form:
                    uVar28 = eqmain_fn_100115f0_tx_guess();
                    if ((char)uVar28 == '\0') {
                        eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess,
                            g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
                    } else {
                        requestPending = true;
                        uVar28 = eqmain_fn_100256c0_tx_guess();
                        if ((char)uVar28 == '\0') {
                            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess,
                                g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
                            requestPending = false;
                        } else {
                            g_eqmain_d220_guess = 1;
                            strcpy(g_eqmain_copybuf_guess, g_eqmain_55764_guess);
                            eqmain_fn_10003000_guess(g_eqmain_copybuf_guess);
                        }
                    }
                }
                break;
            case 1:
                clickedBox = (int *)_Memory[2];
                if ((clickedBox == firstYesBox) || (clickedBox == firstNoBox)) {
                    eqmain_fn_10001a60_tx_guess(firstYesBox, firstNoBox, clickedBox);
                }
                clickedBox = (int *)_Memory[2];
                if ((clickedBox == secondYesBox) || (clickedBox == secondNoBox)) {
                    eqmain_fn_10001a60_tx_guess(secondYesBox, secondNoBox, clickedBox);
                }
                cellIndex = 0;
                do {
                    if (_Memory[2] == optionCells[cellIndex]) {
                        tmp = 1 << ((unsigned char)cellIndex & 0x1f);
                        if ((checkedMask & tmp) == 0) {
                            checkedMask = checkedMask | tmp;
                        } else {
                            checkedMask = checkedMask & ~tmp;
                        }
                        eqmain_fn_10035089_guess(checkedMask, g_eqmain_buf_1012beef_guess, 10);
                        break;
                    }
                    cellIndex = cellIndex + 1;
                } while (cellIndex < 0x10);
                break;
            case 4:
                if (*(char *)(_Memory + 2) == '\x1b') {
                    eqmain_fn_10003a00_guess();
                    eqmain_start_fn_100253b0_guess();
go_back:
                    nextState = 7;
finish_loop:
                    done = true;
                }
                break;
            case 6:
                pHVar1 = (int *)_Memory[2];
                if (pHVar1 == editFields[0]) {
                    eqmain_fn_10008910_guess(editFields[4]);
                } else if (pHVar1 == editFields[4]) {
                    eqmain_fn_10008910_guess(editFields[3]);
                } else if (pHVar1 == editFields[3]) {
                    eqmain_fn_10008910_guess(editFields[2]);
                } else if (pHVar1 == editFields[2]) {
                    eqmain_fn_10008910_guess(editFields[1]);
                } else if (pHVar1 == editFields[1]) {
                    eqmain_fn_10008910_guess(editFields[0]);
                }
                break;
            }
            eqmain_fn_10033568_guess(_Memory);
        }
    } while (!done);
    goto cleanup;

abort_screen:
    nextState = 7;
    g_eqmain_flag_5b34_guess = '\0';
cleanup:
    if (g_eqmain_d220_guess != '\0') {
        g_eqmain_d220_guess = '\0';
        eqmain_fn_10003090_guess();
        nextState = 7;
    }
    g_eqmain_busy_guess = '\0';
    eqmain_fn_10001700_guess((int)pHVar7);
    eqmain_fn_10001700_guess((int)pHVar9);
    eqmain_fn_10001700_guess((int)pHVar8);
    eqmain_fn_100019e0_guess(firstYesBox);
    eqmain_fn_100019e0_guess(firstNoBox);
    if (textBlock != (int *)0) {
        eqmain_fn_10008ce0_guess(textBlock);
    }
    cellPtr = optionCells;
    value = 0x10;
    do {
        if ((void *)*cellPtr != (void *)0) {
            eqmain_fn_100019e0_guess((void *)*cellPtr);
        }
        cellPtr = cellPtr + 1;
        value = value - 1;
    } while (value != 0);
    ppHVar21 = editFields;
    value = 5;
    do {
        if (*ppHVar21 != (int *)0) {
            eqmain_fn_10008a00_tx_guess(*ppHVar21);
        }
        ppHVar21 = ppHVar21 + 1;
        value = value - 1;
    } while (value != 0);
    EQ_STDCALL(void, 0x100039E0, (void))();
    return nextState;
}

// FUNCTION: EQMAIN 0x10014830
// Draws a caption, fills the 0x1004D1CC cell column with the selected slot's bit set, keeps that cell.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Sets the code at 0x1004D234 to 0x21 and copies the short at
// +8 of the object UnitIndexGet_guess(this, 0x21) returns to 0x1004CF20. The bit is 1 << the slot index at 0x1012C222, or 0 when the
// index is -1 (kept in ESI). Then UnitIndexGet_guess(this, code) goes to FUN_10025950, and EQMain_DrawTextRun_guess gets (0x1004CEB8, 0x37, 0x94,
// 0x21A3C, that string). The third UnitIndexGet_guess call is an argument of the MarkColumnCells call (0x1004D1CC, 0xB8, 0x94, its
// result, 200, 0x12, the bit), so its pushes come before it. 0x1004D21C is cleared, then set to the slot's pointer when the index
// is not -1; last the code at 0x1004D234 is incremented. Deviation: the original pushes two extra zeros for the second
// UnitIndexGet_guess call (its callee pops only the one stack argument; the caller's ADD ESP,0x1C clears them with EQMain_DrawTextRun_guess's
// args). The one-argument UnitIndexGet_guess macro can't express them, so this rewrite leaves them out.
void __stdcall EQMain_MarkSelectedSlot_guess(void)
{
    int tmp;
    char *pCVar2;
    unsigned int slotBit;
    int cellHeight;

    g_eqmain_code_guess = 0x21;
    tmp = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x21);
    g_eqmain_cf20_guess = *(short *)(tmp + 8);
    if (g_eqmain_slot_guess != -1) {
        slotBit = 1 << g_eqmain_slot_guess;
    }
    else {
        slotBit = 0;
    }
    tmp = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
    pCVar2 = eqmain_fn_10025950_method_guess(tmp)();
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x37, 0x94, 0x21a3c, pCVar2);
    cellHeight = 0x12;
    tmp = 200;
    eqmain_fn_100119a0_guess(g_eqmain_slots_a_guess, 0xb8, 0x94,
        (void *)eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess), tmp, cellHeight, slotBit);
    g_eqmain_21c_guess = 0;
    if (g_eqmain_slot_guess != -1) {
        g_eqmain_21c_guess = (int)g_eqmain_slots_a_guess[g_eqmain_slot_guess];
    }
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    return;
}

// FUNCTION: EQMAIN 0x10014910
// Lays out the selected entry's form: six edit fields, three labels and two rollout drop-down lists.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Sets the code at 0x1004D234 to 0x21 and takes the string
// IdxLookup_guess::Get_guess gives for the object UnitIndexGet_guess(this, 0x21) returns (this is g_eqmain_obj_c774_guess; its stack argument 0 is pushed
// first). While the short at 0x1012C222 is above 0, that string is replaced by Next_guess of UnitIndexGet_guess(this, code), once
// per slot. From the first '-' of the string the text after it, NUL included, is copied to 0x1004D168. The 10 pointers at
// 0x1004D1F4 are freed with EQMain_FreeEntry_guess (the non-null ones) and cleared. Then the codes 0x23..0x2F go through UnitIndexGet_guess and
// FUN_10025950 into EQMain_CreateLabelFixed_guess, EQMain_DrawTextRun_guess, CreateRolloutRecord_guess, FindNameIndex_guess, SetWordFieldsAndUpdate_guess and EQMain_ProcessClassifiedString_guess, as the original
// does; the string at 0x1004CE40 is copied to 0x1012C44C and 0x1012C3EC, and the records go to 0x1004D228 and 0x1004D22C
// (the old one at 0x1004D228 is freed with UnlinkAndFreeRecord_guess first, as is the one at 0x1004D22C before its new record).
// Deviation: the three EQMain_DrawTextRun_guess blocks (codes 0x24, 0x05, 0x07) push two extra zeros before their UnitIndexGet_guess call,
// which the caller's ADD ESP clears (0x1C; 0x20 in the last one, with EQMain_SetCachedEntry_guess's argument). The one-argument macro
// can't express them, so this rewrite leaves them out.
void __stdcall EQMain_BuildSlotScreen_guess(void)
{
    char ch;
    char *ptr;
    char *dst;
    int obj;
    int index;
    unsigned int value;
    unsigned int dwordCount;
    int *entryPtr;

    g_eqmain_code_guess = 0x21;
    ptr = eqmain_fn_10025940_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x21))(0);
    index = 0;
    if (0 < g_eqmain_slot_guess) {
        do {
            obj = eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess);
            ptr = eqmain_fn_10025960_guess(obj)();
            index = index + 1;
        } while (index < g_eqmain_slot_guess);
    }
    ch = *ptr;
    while (ch != '-') {
        dst = ptr + 1;
        ptr = ptr + 1;
        ch = *dst;
    }
    ptr = ptr + 1;
    value = strlen(ptr) + 1;
    dst = g_eqmain_str_dst_guess;
    for (dwordCount = value >> 2; dwordCount != 0; dwordCount = dwordCount - 1) {
        *(unsigned int *)dst = *(unsigned int *)ptr;
        ptr = ptr + 4;
        dst = dst + 4;
    }
    for (value = value & 3; value != 0; value = value - 1) {
        *dst = *ptr;
        ptr = ptr + 1;
        dst = dst + 1;
    }
    entryPtr = (int *)g_eqmain_slots_b_guess;
    do {
        if (*entryPtr != 0) {
            eqmain_fn_10008a00_guess(*entryPtr);
            *entryPtr = 0;
        }
        entryPtr = entryPtr + 1;
    } while ((int)entryPtr < 0x1004D21C);

    g_eqmain_code_guess = 0x23;
    eqmain_fn_10011970_guess(0xb8, 0xce, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x23))(),
        (int *)g_eqmain_slots_b_guess, 0x40, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x37, 0xe1, 0x21a3c,
        eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))());
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_10011970_guess(0xb8, 0xf4, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))(),
        g_eqmain_buf_1fc_guess, 0x40, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_10011970_guess(0xb8, 0x107, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))(),
        g_eqmain_buf_200_guess, 0x40, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 2;
    eqmain_fn_10011970_guess(0xb8, 0x11a, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))(),
        g_eqmain_buf_208_guess, 0x80, 0);
    g_eqmain_code_guess = 4;
    eqmain_fn_10011970_guess(0xb8, 0x12d, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(4))(),
        g_eqmain_buf_20c_guess, 0x40, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x37, 0x140, 0x21a3c,
        eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))());
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_10011970_guess(0xb8, 0x153, eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))(),
        g_eqmain_buf_214_guess, 0x20, 0);
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_100082c0_guess(g_eqmain_ceb8_guess, 0x37, 0x166, 0x21a3c,
        eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))());
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    eqmain_fn_10008910_guess(g_eqmain_slots_b_guess[0]);
    eqmain_fn_10011bc0_guess();
    if (g_eqmain_obj_guess != 0) {
        eqmain_fn_10006130_guess(g_eqmain_obj_guess);
        g_eqmain_obj_guess = 0;
    }

    value = strlen(g_eqmain_drive_base_guess) + 1;
    ptr = g_eqmain_drive_base_guess;
    dst = g_eqmain_buf_44c_guess;
    for (dwordCount = value >> 2; dwordCount != 0; dwordCount = dwordCount - 1) {
        *(unsigned int *)dst = *(unsigned int *)ptr;
        ptr = ptr + 4;
        dst = dst + 4;
    }
    g_eqmain_code_guess = 0x2e;
    for (value = value & 3; value != 0; value = value - 1) {
        *dst = *ptr;
        ptr = ptr + 1;
        dst = dst + 1;
    }
    value = strlen(g_eqmain_drive_base_guess) + 1;
    ptr = g_eqmain_drive_base_guess;
    dst = g_eqmain_buf_3ec_guess;
    for (dwordCount = value >> 2; dwordCount != 0; dwordCount = dwordCount - 1) {
        *(unsigned int *)dst = *(unsigned int *)ptr;
        ptr = ptr + 4;
        dst = dst + 4;
    }
    for (value = value & 3; value != 0; value = value - 1) {
        *dst = *ptr;
        ptr = ptr + 1;
        dst = dst + 1;
    }
    g_eqmain_obj_guess = (int)eqmain_fn_10005fd0_guess(g_eqmain_ceb8_guess, 0xb8, 0x166, 0,
        eqmain_fn_10021110_guess(g_eqmain_buf_44c_guess), eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x2e), 1, 0);
    value = eqmain_fn_100061f0_guess(g_eqmain_obj_guess, g_eqmain_str_47f9c_guess);
    eqmain_fn_10006510_guess((int *)g_eqmain_obj_guess, 0x10, (short)value);
    if (g_eqmain_obj_b_guess != 0) {
        eqmain_fn_10006130_guess(g_eqmain_obj_b_guess);
        g_eqmain_obj_b_guess = 0;
    }
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    g_eqmain_obj_b_guess = (int)eqmain_fn_10005fd0_guess(g_eqmain_ceb8_guess, 0xb8, 0xe1, 0,
        eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess))(),
        eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(g_eqmain_code_guess), 1, 0);
    eqmain_fn_10006510_guess((int *)g_eqmain_obj_b_guess, 0xe, 0);
    eqmain_fn_10011c90_guess(0x10, 0xb8, 0x140, eqmain_fn_10021110_guess(g_eqmain_buf_3ec_guess));
    return;
}

// FUNCTION: EQMAIN 0x10014D50
// Builds the selectable list with a checkbox and text label for each entry.
// __cdecl, one argument (mode); returns nothing. Sets the code at 0x1004D234 to 0x20, then EQMain_DrawTextRun_guess gets
// (0x1004CEB8, 0x37, 0x4C, 0x21A3C, the string FUN_10025950 returns for UnitIndexGet_guess(this, 0x20)) with two zeros pushed
// first (args 6 and 7; the callee ignores them). 0x1004CF24 is zeroed, then for each slot below the count at 0x1012C214:
// CreateColumnCell_guess makes the checkbox (chekbox1.bmp, chekbox2.bmp, the local flag as param_6, the name at 0x1012C4EC or
// 0x1012BF94 + 0x40 * i) and its pointer goes to 0x1004CF38[i]; EQMain_DrawTextRun_guess places it. The shape alternates per slot by
// a local flag (0xB8 and 0x13A, then 0x180 and 0x202). The slot whose index is the short at 0x1012C220 also sets 0x1004CF24.
// Then the code goes up by one, EQMain_CreateRecord_guess makes the object, and unless mode is 5 the strings at 0x10055814 and
// 0x10055818 are copied to 0x100F4C10 and given to WrapTextLines_guess. DrawTextBlock_guess and chain_table_free_guess end it. The extra zeros of
// the CreateColumnCell_guess and EQMain_DrawTextRun_guess calls are the original's pushes; they're kept so the stack layout matches.
// @early-stop: the fastcall EDX dummy and the loop branch register scheduling still differ.
void __cdecl EQMain_BuildSlotCheckboxes_guess(int mode)
{
    short slotIndex;
    bool alternateLayout = false;
    int isSelected;
    int *checkbox;
    int *textBlock;
    int **slotEntry;
    int drawContext;
    int pHVar14;

    slotIndex = 0;
    g_eqmain_code_guess = 0x20;
    eqmain_fn_100082c0_7args_guess(g_eqmain_ceb8_guess, 0x37, 0x4c, 0x21a3c,
        eqmain_fn_10025950_method_guess(eqmain_fn_10025c00_method_guess(g_eqmain_reg_this_guess)(0x20))(), 0, 0);
    g_eqmain_cf24_guess = 0;
    if (0 < g_eqmain_count_guess) {
        do {
            isSelected = 0;
            if (slotIndex == g_eqmain_sel_guess) {
                isSelected = 1;
            }
            if (!alternateLayout) {
                int arrayIndex = (int)slotIndex;
                slotEntry = &g_eqmain_cf38_guess[arrayIndex];
                pHVar14 = (arrayIndex / 2) * 0x12 + 0x4c;
                checkbox = eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0xb8, pHVar14, eqmain_str_chekbox1_guess,
                    eqmain_str_chekbox2_guess, (int)(short)isSelected, 0x21a3c, (char *)g_eqmain_c4ec_guess + arrayIndex * 0x40, 0, 0);
                drawContext = g_eqmain_ceb8_guess;
                *slotEntry = checkbox;
                eqmain_fn_100082c0_7args_guess(drawContext, 0x13a, pHVar14, 0x21a3c,
                    (char *)g_eqmain_bf94_guess + arrayIndex * 0x40, 0, 0);
            }
            else {
                int arrayIndex = (int)slotIndex;
                slotEntry = &g_eqmain_cf38_guess[arrayIndex];
                pHVar14 = (arrayIndex / 2) * 0x12 + 0x4c;
                checkbox = eqmain_fn_100018f0_guess(g_eqmain_ceb8_guess, 0x180, pHVar14, eqmain_str_chekbox1_guess,
                    eqmain_str_chekbox2_guess, (int)(short)isSelected, 0x21a3c, (char *)g_eqmain_c4ec_guess + arrayIndex * 0x40, 0, 0);
                drawContext = g_eqmain_ceb8_guess;
                *slotEntry = checkbox;
                eqmain_fn_100082c0_7args_guess(drawContext, 0x202, pHVar14, 0x21a3c,
                    (char *)g_eqmain_bf94_guess + arrayIndex * 0x40, 0, 0);
            }
            alternateLayout = !alternateLayout;
            if (slotIndex == g_eqmain_sel_guess) {
                g_eqmain_cf24_guess = *slotEntry;
            }
            slotIndex = slotIndex + 1;
        } while (slotIndex < g_eqmain_count_guess);
    }
    g_eqmain_code_guess = g_eqmain_code_guess + 1;
    textBlock = eqmain_fn_10008c10_guess(g_eqmain_ceb8_guess, 0x7c, 0x94, 0x17c, 0x78, 0x10, 0xc81000, g_eqmain_dlg_arg_guess, 0);
    if (mode != 5) {
        strcpy(g_eqmain_copybuf_guess, g_eqmain_str_a_guess);
        eqmain_fn_10009370_guess(textBlock, g_eqmain_copybuf_guess);
    }
    strcpy(g_eqmain_copybuf_guess, g_eqmain_str_b_guess);
    eqmain_fn_10009370_guess(textBlock, g_eqmain_copybuf_guess);
    eqmain_fn_10008d60_guess(textBlock);
    eqmain_fn_10008ce0_guess(textBlock);
    return;
}

// FUNCTION: EQMAIN 0x10014fa0
// Resets the selection state and frees the retained list entries.
// __stdcall, no arguments (callee pops 0 bytes); returns nothing. Zeroes the 0x2CC-byte block at 0x1012C220, then sets
// the short at its +2 and the short at its +0 to 0xffff. Each non-null pointer of the 10-slot table at 0x1004D1CC is
// freed with eqmainFreeUnit_guess and cleared; the same for the table at 0x1004D1F4 with EQMain_FreeEntry_guess. strcpy copies the
// string at 0x1004CE40 to 0x1004D168. The destination reaches 0x1004D1AC at offset 68: for strings shorter than 71
// characters the pointer is captured before the trailing byte copy; for strings of 71 or more characters the dword
// copy has already overwritten it, so the copied value at offset 68 is used. A non-null value goes to EQMain_FreeEntry_guess and
// 0x1004D1AC is cleared.
// @early-stop: VC6 shares the two 0xffff stores through EAX; pointer timing needs the extra length check and correction.
void __stdcall EQMain_ResetState_guess(void)
{
    int i;
    unsigned int n;
    void **slot;
    void *p;

    slot = g_eqmain_slots_a_guess;
    memset(g_eqmain_state_guess, 0, 0x2CC);
    *(short*)(g_eqmain_state_guess + 2) = -1;
    *(short*)g_eqmain_state_guess = -1;

    i = 10;
    do {
        if (*slot != 0) {
            eqmain_fn_100019e0_guess(*slot);
            *slot = 0;
        }
        slot++;
        i--;
    } while (i != 0);

    slot = g_eqmain_slots_b_guess;
    i = 10;
    do {
        if (*slot != 0) {
            eqmain_fn_10008a00_tx_guess(*slot);
            *slot = 0;
        }
        slot++;
        i--;
    } while (i != 0);

    p = g_eqmain_ptr_guess;
    n = strlen(g_eqmain_drive_base_guess) + 1;
    strcpy(g_eqmain_str_dst_guess, g_eqmain_drive_base_guess);
    if (n > 71) {
        p = *(void **)(g_eqmain_str_dst_guess + 68);
    }
    if (p != 0) {
        eqmain_fn_10008a00_tx_guess(p);
        g_eqmain_ptr_guess = 0;
    }
    return;
}

// used by EQMain_RunSlotScreen_guess (0x10015050):
#define g_eqmain_1a8_guess              EQ_GLOBAL(int, 0x1004D1A8)                                  // g_eqmain_1a8_guess: the record CreateSurfaceRecord_guess returns for the 0x24e x 0xe2 panel
#define g_eqmain_1c4_guess              EQ_GLOBAL(int, 0x1004D1C4)                                  // g_eqmain_1c4_guess: the record CreateSurfaceRecord_guess returns for the 0x24e x 0x3c panel
#define g_eqmain_slots_pre_guess        EQ_ARRAY(void *, 0x1004D1F0)                                // g_eqmain_slots_pre_guess: the table one dword below g_eqmain_slots_b_guess (indexed by the slot number)
#define g_eqmain_slots_b1_guess         EQ_ARRAY(void *, 0x1004D1F8)                                // g_eqmain_slots_b1_guess: the table one dword above g_eqmain_slots_b_guess
#define g_eqmain_str_5850_guess         EQ_GLOBAL(char *, 0x10055850)                               // g_eqmain_str_5850_guess: the string passed to EQMain_FitFontSize_guess and eqmain_draw_text_guess
#define g_eqmain_str_582c_guess         EQ_GLOBAL(char *, 0x1005582C)                               // g_eqmain_str_582c_guess: copied to g_eqmain_copybuf_guess when param_1 == 1
#define g_eqmain_str_576c_guess         EQ_GLOBAL(char *, 0x1005576C)                               // g_eqmain_str_576c_guess: the text eqmainRunDialog_guess shows for the 0x33 key
#define g_eqmain_str_580c_guess         EQ_GLOBAL(char *, 0x1005580C)                               // g_eqmain_str_580c_guess: the sprintf format for the 0x36 chat line
#define g_eqmain_str_5768_guess         EQ_GLOBAL(char *, 0x10055768)                               // g_eqmain_str_5768_guess: copied to g_eqmain_copybuf_guess when the chat line is confirmed
#define g_eqmain_str_57e0_guess         EQ_GLOBAL(char *, 0x100557E0)                               // g_eqmain_str_57e0_guess: the text shown when no slot is selected
#define g_eqmain_str_5b20_guess         EQ_GLOBAL(char *, 0x10055B20)                               // g_eqmain_str_5b20_guess: the text shown when g_eqmain_buf_264_guess is shorter than 13 chars
#define g_eqmain_str_57f8_guess         EQ_GLOBAL(char *, 0x100557F8)                               // g_eqmain_str_57f8_guess: the sprintf format for param_1 == 1
#define g_eqmain_str_57e4_guess         EQ_GLOBAL(char *, 0x100557E4)                               // g_eqmain_str_57e4_guess: the sprintf format for slot 0
#define g_eqmain_str_57e8_guess         EQ_GLOBAL(char *, 0x100557E8)                               // g_eqmain_str_57e8_guess: the sprintf format for slot 1
#define g_eqmain_str_57ec_guess         EQ_GLOBAL(char *, 0x100557EC)                               // g_eqmain_str_57ec_guess: the sprintf format for slot 2
#define g_eqmain_str_57f0_guess         EQ_GLOBAL(char *, 0x100557F0)                               // g_eqmain_str_57f0_guess: appended to the text unless param_1 == 5
#define g_eqmain_str_57f4_guess         EQ_GLOBAL(char *, 0x100557F4)                               // g_eqmain_str_57f4_guess: appended to the text
#define eqmain_str_txbilling_guess      ((char *)0x10047FD0)                                        // "txbilling.bmp" (.rdata)
#define eqmain_str_47f8c_guess          ((char *)0x10047F8C)                                        // the string the slot name is compared with (__strcmpi)
#define eqmain_str_everquest_guess      ((char *)0x10047FC4)                                        // "EverQuest" (.rdata)
#define g_eqmain_buf_4ac_guess          EQ_ARRAY(char, 0x1012C4AC)                                  // g_eqmain_buf_4ac_guess: the argument DecodeToBuffer_guess gets
#define g_eqmain_buf_224_guess          EQ_ARRAY(char, 0x1012C224)                                  // g_eqmain_buf_224_guess: 0x3f-byte name copied from g_eqmain_str_dst_guess
#define g_eqmain_buf_264_guess          EQ_ARRAY(char, 0x1012C264)                                  // g_eqmain_buf_264_guess: 0x3f-byte name (13 or more chars needed)
#define g_eqmain_buf_2a4_guess          EQ_ARRAY(char, 0x1012C2A4)                                  // g_eqmain_buf_2a4_guess: 7-byte name copied from ForwardScaled_guess of g_eqmain_obj_b_guess
#define g_eqmain_buf_2ec_guess          EQ_ARRAY(char, 0x1012C2EC)                                  // g_eqmain_buf_2ec_guess: 0x3f-byte name
#define g_eqmain_buf_2ac_guess          EQ_ARRAY(char, 0x1012C2AC)                                  // g_eqmain_buf_2ac_guess: 0x3f-byte name
#define g_eqmain_buf_46c_guess          EQ_ARRAY(char, 0x1012C46C)                                  // g_eqmain_buf_46c_guess: 0x3f-byte copy of "EverQuest"
#define g_eqmain_buf_32c_guess          EQ_ARRAY(char, 0x1012C32C)                                  // g_eqmain_buf_32c_guess: 0x7f-byte name
#define g_eqmain_buf_3ac_guess          EQ_ARRAY(char, 0x1012C3AC)                                  // g_eqmain_buf_3ac_guess: 0x3f-byte name
#define g_eqmain_buf_42c_guess          EQ_ARRAY(char, 0x1012C42C)                                  // g_eqmain_buf_42c_guess: 0x1f-byte name
#define g_eqmain_bfd4_guess             EQ_ARRAY(int, 0x1012BFD4)                                   // g_eqmain_bfd4_guess: second name of slot 1 (sprintf argument)
#define g_eqmain_c52c_guess             EQ_ARRAY(int, 0x1012C52C)                                   // g_eqmain_c52c_guess: first name of slot 1 (sprintf argument)
#define g_eqmain_c014_guess             EQ_ARRAY(int, 0x1012C014)                                   // g_eqmain_c014_guess: second name of slot 2 (sprintf argument)
#define g_eqmain_c56c_guess             EQ_ARRAY(int, 0x1012C56C)                                   // g_eqmain_c56c_guess: first name of slot 2 (sprintf argument)
#define eqmain_fn_100080b0_guess        EQ_CDECL(int, 0x100080B0, (int, int, int, char *, int, int)) // EQMain_FitFontSize_guess: cdecl; the caller pushes 6 args, the last two unused by it
#define eqmain_fn_10001ad0_guess        EQ_CDECL(short, 0x10001AD0, (int, int *, short, int *))     // eqmainSelectUnitInGroup_guess: cdecl, 4 args, returns a short
#define eqmain_fn_10011a50_guess        EQ_STDCALL(unsigned int, 0x10011A50, (void))                // EQMain_PollStatus_guess: stdcall, no args, returns the key code
#define eqmain_fn_10025790_guess        EQ_STDCALL(unsigned int, 0x10025790, (void))                // eqmain_append_record_c220_guess: stdcall, no args, bool in AL
#define eqmain_fn_10025730_guess        EQ_STDCALL(unsigned int, 0x10025730, (void))                // eqmain_queue_table_push31_guess: stdcall, no args, bool in AL
#define eqmain_fn_10011b60_guess        EQ_CDECL(void, 0x10011B60, (int, int))                      // EQMain_Init_guess: cdecl, 2 args
#define eqmain_fn_10014830_guess        EQ_STDCALL(void, 0x10014830, (void))                        // EQMain_MarkSelectedSlot_guess: stdcall, no args
#define eqmain_fn_10014d50_guess        EQ_CDECL(void, 0x10014D50, (int))                           // EQMain_BuildSlotCheckboxes_guess: cdecl, one arg
#define eqmain_fn_10011c00_guess        EQ_STDCALL(unsigned int, 0x10011C00, (void))                // EQMain_GetCurrentValue_guess: stdcall, no args, pointer in EAX
#define eqmain_fn_100211b0_guess        EQ_CDECL(char *, 0x100211B0, (char *))                      // CopyStringToBuffer_guess: cdecl, one arg, returns a string
#define eqmain_fn_10033330_guess        EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int)) // _strncpy: cdecl, 3 args
#define eqmain_fn_10023c10_guess        EQ_CDECL(char, 0x10023C10, (char *))                        // String_clearIfAllSpaces_guess: cdecl, one arg, result unused
#define eqmain_fn_10007a80_guess        EQ_CDECL(void, 0x10007A80, (int *))                         // PackFsFreeEntry_guess: cdecl, one arg
#define eqmain_fn_10014910_guess        EQ_STDCALL(void, 0x10014910, (void))                        // EQMain_BuildSlotScreen_guess: stdcall, no args
#define eqmain_fn_10014fa0_guess        EQ_STDCALL(void, 0x10014FA0, (void))                        // EQMain_ResetState_guess: stdcall, no args
#define eqmain_fn_10034117_guess        EQ_CDECL(int, 0x10034117, (char *, char *, ...))            // _sprintf: cdecl, varargs

// FUNCTION: EQMAIN 0x10015050
// Probably the billing form (txbilling): requires every field filled, one of 13+ chars, confirms and submits
// __cdecl, one argument (callee pops 0 bytes); returns the code in AX only. Runs the slot screen: the two panels and the
// window pair, clears the state block and the three 10-entry tables, copies the 0x1004CE40 string, then builds the slot list
// (mode == 1) or calls EQMain_BuildSlotCheckboxes_guess(mode). The loop polls EQMain_PollStatus_guess (0x33 quits, 0x36 sends the chat line), takes
// a queued record from PumpMessagesAndEditText_guess and frees it after the switch. 1 when escape (0x1B) ended it, 7 for the quit key or a
// pending g_eqmain_d220_guess flag.
unsigned short __cdecl EQMain_RunSlotScreen_guess(int mode)
{
    bool requestPending;
    bool done;
    char confirmed;
    short tmp;
    char *pcVar5;
    int pHVar6;
    int pHVar7;
    int *textBlock;
    int *ptr;
    int *obj;
    int *fmtArg2;
    int value;
    unsigned int code;
    unsigned int temp;
    unsigned short exitCode;
    char message[1024];

    done = false;
    requestPending = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, eqmain_str_news_guess, 0x10);
    temp = 0;
    value = 0;
    pcVar5 = eqmain_fn_100210f0_guess(eqmain_str_txbilling_guess);
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, pcVar5, value, temp);
    g_eqmain_1a8_guess = (int)eqmain_fn_10006d80_guess(0x24e, 0xe2, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_1a8_guess, 0, 0, g_eqmain_ceb8_guess, 0x18, 0xce, 0x24e, 0xe2, 0);
    g_eqmain_1c4_guess = (int)eqmain_fn_10006d80_guess(0x24e, 0x3c, 0x10);
    eqmain_fn_10006c20_guess(g_eqmain_1c4_guess, 0, 0, g_eqmain_ceb8_guess, 0x18, 0x92, 0x24e, 0x3c, 0);
    pHVar6 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x18, 1);
    pHVar7 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x14, 1);
    value = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_eqmain_str_5850_guess, 0x19, 0);
    value = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0x64, 0x22, g_eqmain_text_5848_guess, value, 0);
    eqmain_fn_100017b0_guess(pHVar7, 0, g_eqmain_text_5848_guess, value, 0);
    eqmain_fn_100017b0_guess(pHVar6, 0, g_eqmain_str_5850_guess, value, 0);
    ptr = (int *)g_eqmain_state_guess;
    for (value = 0xb3; value != 0; value = value + -1) {
        *ptr = 0;
        ptr = ptr + 1;
    }
    ptr = (int *)g_eqmain_cf38_guess;
    for (value = 10; value != 0; value = value + -1) {
        *ptr = 0;
        ptr = ptr + 1;
    }
    ptr = (int *)g_eqmain_slots_a_guess;
    for (value = 10; value != 0; value = value + -1) {
        *ptr = 0;
        ptr = ptr + 1;
    }
    ptr = (int *)g_eqmain_slots_b_guess;
    for (value = 10; value != 0; value = value + -1) {
        *ptr = 0;
        ptr = ptr + 1;
    }
    strcpy(g_eqmain_str_dst_guess, g_eqmain_drive_base_guess);
    g_eqmain_slot_guess = -1;
    g_eqmain_sel_guess = -1;
    g_eqmain_ptr_guess = 0;
    eqmain_fn_10011b60_guess(0xb8, 0x13e);
    if (mode == 1) {
        textBlock = eqmain_fn_10008c10_guess(g_eqmain_ceb8_guess, 0xb8, 0x5e, 0x17c, 0x78, 0x10, 0xc81000, g_eqmain_dlg_arg_guess, 0);
        strcpy(g_eqmain_copybuf_guess, g_eqmain_str_582c_guess);
        eqmain_fn_10009370_guess(textBlock, g_eqmain_copybuf_guess);
        eqmain_fn_10008d60_guess(textBlock);
        eqmain_fn_10008ce0_guess(textBlock);
        g_eqmain_sel_guess = 0;
        eqmain_fn_10014830_guess();
    }
    else {
        eqmain_fn_10014d50_guess(mode);
    }
    g_eqmain_busy_guess = 1;
    eqmain_fn_10003a00_guess();
    do {
        code = eqmain_fn_10011a50_guess();
        tmp = (short)code;
        if (tmp != 0) {
            requestPending = false;
        }
        if (tmp == 0x33) {
            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_str_576c_guess, g_str_888_guess, 0);
            exitCode = 7;
            break;
        }
        if (tmp == 0x36) {
            pcVar5 = eqmain_fn_10021110_guess(g_eqmain_buf_4ac_guess);
            eqmain_fn_10034117_guess(message, g_eqmain_str_580c_guess, g_eqmain_copybuf_guess, pcVar5);
            confirmed = eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, message, g_eqmain_text_b_guess, g_eqmain_text_a_guess);
            if (confirmed != 0) {
                temp = eqmain_fn_10025790_guess();
                if ((char)temp == 0) {
                    eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, 0);
                }
                else {
                    g_eqmain_d220_guess = 1;
                    requestPending = true;
                    strcpy(g_eqmain_copybuf_guess, g_eqmain_str_5768_guess);
                    eqmain_fn_10003000_guess(g_eqmain_copybuf_guess);
                }
            }
        }
        ptr = eqmain_fn_10003bd0_guess();
        value = g_eqmain_result_guess;
        if (ptr != 0) {
            switch (*ptr) {
                case 0:
                case 7:
                    if (ptr[2] == g_eqmain_ret_obj_guess) {
                        eqmain_fn_10008910_guess((void *)g_eqmain_buf_214_guess[0]);
                    }
                    else {
                        tmp = 0;
                        do {
                            if (ptr[2] == (int)g_eqmain_slots_b_guess[tmp]) {
                                obj = (int *)g_eqmain_slots_b_guess[0];
                                if (tmp < 8) {
                                    if (tmp == 6) {
                                        obj = (int *)g_eqmain_ret_obj_guess;
                                        if (g_eqmain_result_guess == 2) goto setEntry;
nextIndex:
                                        tmp = tmp + 1;
                                    }
                                    else if ((tmp == 3) || (tmp == 0)) goto nextIndex;
                                    obj = (int *)g_eqmain_slots_b1_guess[tmp];
                                }
setEntry:
                                eqmain_fn_10008910_guess(obj);
                                break;
                            }
                            tmp = tmp + 1;
                        } while (tmp < 10);
                        if (ptr[2] == pHVar7) goto exitScreen;
                        if (ptr[2] == pHVar6 && !requestPending) {
                            pcVar5 = g_eqmain_str_57e0_guess;
                            if (g_eqmain_sel_guess == -1 || g_eqmain_slot_guess == -1) {
showMessage:
                                eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, pcVar5, g_str_888_guess, 0);
                            }
                            else {
                                pcVar5 = (char *)eqmain_fn_10011c00_guess();
                                pcVar5 = eqmain_fn_100211b0_guess(pcVar5);
                                strcpy(g_eqmain_buf_3ec_guess, pcVar5);
                                value = eqmain_fn_10033730_guess(g_eqmain_buf_3ec_guess, eqmain_str_47f8c_guess);
                                textBlock = (int *)g_eqmain_ceb8_guess;
                                if (value == 0) {
                                    strcpy(g_eqmain_copybuf_guess, g_eqmain_start_text_c_guess);
                                    eqmain_fn_10001ff0_guess(textBlock, g_eqmain_copybuf_guess, g_str_888_guess, 0);
                                }
                                else {
                                    eqmain_fn_10033330_guess(g_eqmain_buf_224_guess, eqmain_fn_100211b0_guess(g_eqmain_str_dst_guess), 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_264_guess, eqmain_fn_100211b0_guess(*(char **)((int)g_eqmain_slots_b_guess[0] + 0x20)), 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_2a4_guess, eqmain_fn_100211b0_guess(eqmain_fn_100061d0_guess(g_eqmain_obj_b_guess, *(short *)(g_eqmain_obj_b_guess + 0x28))), 7);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_2ec_guess, eqmain_fn_100211b0_guess(*(char **)(g_eqmain_buf_1fc_guess[0] + 0x20)), 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_2ac_guess, eqmain_fn_100211b0_guess(*(char **)(g_eqmain_buf_200_guess[0] + 0x20)), 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_46c_guess, eqmain_str_everquest_guess, 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_32c_guess, eqmain_fn_100211b0_guess(*(char **)(g_eqmain_buf_208_guess[0] + 0x20)), 0x7f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_3ac_guess, eqmain_fn_100211b0_guess(*(char **)(g_eqmain_buf_20c_guess[0] + 0x20)), 0x3f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_42c_guess, eqmain_fn_100211b0_guess(*(char **)(g_eqmain_buf_214_guess[0] + 0x20)), 0x1f);
                                    eqmain_fn_10033330_guess(g_eqmain_buf_44c_guess, eqmain_fn_100211b0_guess(eqmain_fn_100061d0_guess(g_eqmain_obj_guess, *(short *)(g_eqmain_obj_guess + 0x28))), 0x1f);
                                    eqmain_fn_10023c10_guess(*(char **)((int)g_eqmain_slots_b_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(*(char **)(g_eqmain_buf_1fc_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(*(char **)(g_eqmain_buf_200_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(*(char **)(g_eqmain_buf_208_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(*(char **)(g_eqmain_buf_20c_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(*(char **)(g_eqmain_buf_214_guess[0] + 0x20));
                                    eqmain_fn_10023c10_guess(g_eqmain_str_dst_guess);
                                    if (strlen(g_eqmain_str_dst_guess) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)((int)g_eqmain_slots_b_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(g_eqmain_buf_2a4_guess) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)(g_eqmain_buf_1fc_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)(g_eqmain_buf_200_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(g_eqmain_buf_46c_guess) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)(g_eqmain_buf_208_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)(g_eqmain_buf_20c_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(g_eqmain_buf_3ec_guess) == 0) goto fieldEmpty;
                                    if (strlen(*(char **)(g_eqmain_buf_214_guess[0] + 0x20)) == 0) goto fieldEmpty;
                                    if (strlen(g_eqmain_buf_44c_guess) == 0) goto fieldEmpty;
                                    if (strlen(g_eqmain_buf_264_guess) < 13) {
                                        pcVar5 = g_eqmain_str_5b20_guess;
                                        goto showMessage;
                                    }
                                    if (mode == 1) {
                                        eqmain_fn_10034117_guess(g_eqmain_copybuf_guess, g_eqmain_str_57f8_guess);
                                    }
                                    else {
                                        if (g_eqmain_sel_guess == 0) {
                                            fmtArg2 = g_eqmain_bf94_guess;
                                            obj = g_eqmain_c4ec_guess;
                                            pcVar5 = g_eqmain_str_57e4_guess;
formatMessage:
                                            eqmain_fn_10034117_guess(g_eqmain_copybuf_guess, pcVar5, obj, fmtArg2);
                                        }
                                        else {
                                            if (g_eqmain_sel_guess == 1) {
                                                fmtArg2 = g_eqmain_bfd4_guess;
                                                obj = g_eqmain_c52c_guess;
                                                pcVar5 = g_eqmain_str_57e8_guess;
                                                goto formatMessage;
                                            }
                                            if (g_eqmain_sel_guess == 2) {
                                                fmtArg2 = g_eqmain_c014_guess;
                                                obj = g_eqmain_c56c_guess;
                                                pcVar5 = g_eqmain_str_57ec_guess;
                                                goto formatMessage;
                                            }
                                        }
                                        if (mode != 5) {
                                            strcat(g_eqmain_copybuf_guess, g_eqmain_str_57f0_guess);
                                        }
                                        strcat(g_eqmain_copybuf_guess, g_eqmain_str_57f4_guess);
                                    }
                                    confirmed = eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_eqmain_text_b_guess, g_eqmain_text_a_guess);
                                    if (confirmed != 0) {
                                        temp = eqmain_fn_10025730_guess();
                                        if ((char)temp == 0) {
                                            eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, 0);
                                        }
                                        else {
                                            g_eqmain_d220_guess = 1;
                                            requestPending = true;
                                            strcpy(g_eqmain_copybuf_guess, g_eqmain_str_5768_guess);
                                            eqmain_fn_10003000_guess(g_eqmain_copybuf_guess);
                                        }
                                    }
                                    break;
fieldEmpty:
                                    eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_str_57e0_guess, g_str_888_guess, 0);
                                }
                            }
                        }
                    }
                    break;
                case 1:
                    if (ptr[2] == g_eqmain_obj_guess && *(int *)(g_eqmain_obj_guess + 0x1c) == 0) {
                        eqmain_fn_10011c30_guess();
                        if (g_eqmain_result_guess != value) {
                            eqmain_fn_10011bc0_guess();
                            strcpy(g_eqmain_buf_3ec_guess, g_eqmain_drive_base_guess);
                            pcVar5 = eqmain_fn_10021110_guess(g_eqmain_buf_3ec_guess);
                            eqmain_fn_10011c90_guess(0x10, 0xb8, 0x140, pcVar5);
                        }
                    }
                    else {
                        tmp = eqmain_fn_10001ad0_guess(ptr[2], (int *)g_eqmain_cf38_guess, g_eqmain_count_guess, (int *)&g_eqmain_cf24_guess);
                        if (tmp == -1) {
                            tmp = eqmain_fn_10001ad0_guess(ptr[2], (int *)g_eqmain_slots_a_guess, g_eqmain_cf20_guess, (int *)&g_eqmain_21c_guess);
                            if (tmp != -1) {
                                eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x18, 0xce, g_eqmain_1a8_guess, 0, 0, 0x24e, 0xe2, 0);
                                g_eqmain_slot_guess = tmp;
                                eqmain_fn_10014910_guess();
                            }
                        }
                        else if (mode != 1) {
                            eqmain_fn_10014fa0_guess();
                            eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x18, 0x92, g_eqmain_1c4_guess, 0, 0, 0x24e, 0x3c, 0);
                            eqmain_fn_10006c20_guess(g_eqmain_ceb8_guess, 0x18, 0xce, g_eqmain_1a8_guess, 0, 0, 0x24e, 0xe2, 0);
                            g_eqmain_sel_guess = tmp;
                            eqmain_fn_10014830_guess();
                        }
                    }
                    break;
                case 4:
                    if (*(char *)(ptr + 2) == 0x1b) {
                        eqmain_fn_10003a00_guess();
exitScreen:
                        eqmain_start_fn_100253b0_guess();
                        exitCode = 1;
                        done = true;
                    }
                    break;
                case 6:
                    if (ptr[2] == g_eqmain_ret_obj_guess) {
                        eqmain_fn_10008910_guess((void *)g_eqmain_buf_20c_guess[0]);
                    }
                    else {
                        tmp = 0;
                        do {
                            if (ptr[2] == (int)g_eqmain_slots_b_guess[tmp]) {
                                if (tmp < 1) {
                                    eqmain_fn_10008910_guess((void *)g_eqmain_buf_214_guess[0]);
                                    break;
                                }
                                if (tmp == 8) {
                                    if (g_eqmain_result_guess == 2) {
                                        eqmain_fn_10008910_guess((void *)g_eqmain_ret_obj_guess);
                                        break;
                                    }
prevIndex:
                                    tmp = tmp + -1;
                                }
                                else if (((tmp == 2) || (tmp == 10)) || (tmp == 5)) goto prevIndex;
                                eqmain_fn_10008910_guess(g_eqmain_slots_pre_guess[tmp]);
                                break;
                            }
                            tmp = tmp + 1;
                        } while (tmp < 10);
                    }
                    break;
            }
            eqmain_fn_10033568_guess(ptr);
        }
    } while (!done);
    if (g_eqmain_d220_guess != 0) {
        g_eqmain_d220_guess = 0;
        eqmain_fn_10003090_guess();
        exitCode = 7;
    }
    g_eqmain_busy_guess = 0;
    eqmain_fn_10001700_guess(pHVar7);
    eqmain_fn_10001700_guess(pHVar6);
    ptr = (int *)g_eqmain_cf38_guess;
    value = 10;
    do {
        if (*ptr != 0) {
            eqmain_fn_100019e0_guess((void *)*ptr);
        }
        ptr = ptr + 1;
        value = value + -1;
    } while (value != 0);
    eqmain_fn_10014fa0_guess();
    eqmain_fn_10011bc0_guess();
    eqmain_fn_10007a80_guess((int *)g_eqmain_rec_guess);
    g_eqmain_rec_guess = 0;
    if (g_eqmain_obj_guess != 0) {
        eqmain_fn_10006130_guess(g_eqmain_obj_guess);
        g_eqmain_obj_guess = 0;
    }
    if (g_eqmain_obj_b_guess != 0) {
        eqmain_fn_10006130_guess(g_eqmain_obj_b_guess);
        g_eqmain_obj_b_guess = 0;
    }
    eqmain_fn_10007a80_guess((int *)g_eqmain_1a8_guess);
    eqmain_fn_10007a80_guess((int *)g_eqmain_1c4_guess);
    eqmain_fn_100039e0_guess(0);
    return exitCode;
}

// FUNCTION: EQMAIN 0x10015CB0
// Runs the EQVstart menu and handles its keyboard and control selections.
// __stdcall, no arguments; returns the low word of the state the loop ends in. Draws the EQVstart.bmp splash with its
// four controls (handles ctl1..ctl4), then loops over the input until done. Esc (0x1b) ends it at state 0xf and calls
// eqmain_call_100213d0_push40_guess at 0xc; 'A' (0x41) sets 0xc and g_eqmain_flag_5b34_guess; 'E' (0x45) copies text to g_eqmain_login_pin_buf_guess. A click on ctl1
// sets 0xc and on ctl2 sets 0xf (each once, with eqmain_call_100213d0_guess); ctl3 shows its text (eqmainChangePasswordDialog_guess, SendStringPair45Packet_guess); a
// null handle sets 0x1a and ends it; ctl4 ends it with 1. The extra words pushed for EQMain_FitFontSize_guess and eqmain_draw_text_guess are
// the original's; those callees ignore them.
unsigned short __stdcall EQMain_StartScreenLoop_guess(void)
{
    bool done;
    bool clickDone;
    char ok;
    short key;
    unsigned int keyRaw;
    int *msg;
    int handle;
    int value1;
    int value2;
    int value3;
    int value4;
    unsigned short result;
    int ctl1;
    int ctl2;
    int ctl3;
    int ctl4;
    int state;
    char text[20];

    done = false;
    clickDone = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, eqmain_str_eqvstart_guess, 0x10);
    ctl1 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0xd2, 0, 2);
    ctl2 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0xff, 0, 2);
    ctl3 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 300, 0, 2);
    ctl4 = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 400, 0x159, 0, 2);
    value1 = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_58ac_guess, 0x19, 0);
    value2 = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_58a8_guess, value1, 0);
    value3 = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_58a4_guess, value2, 0);
    value4 = eqmain_fn_100080b0_guess(g_eqmain_ceb8_guess, 0xd2, 0x22, g_eqmain_text_5830_guess, value3, 0);
    eqmain_fn_100017b0_guess(ctl3, 0, g_eqmain_text_58ac_guess, value4, 0);
    eqmain_fn_100017b0_guess(ctl2, 0, g_eqmain_text_58a8_guess, value4, 0);
    eqmain_fn_100017b0_guess(ctl1, 0, g_eqmain_text_58a4_guess, value4, 0);
    eqmain_fn_100017b0_guess(ctl4, 0, g_eqmain_text_5830_guess, value4, 0);
    g_eqmain_busy_guess = 1;
    eqmain_fn_10003a00_guess();
    result = (unsigned short)ctl2;
    state = ctl2;
    if (g_eqmain_cf60_guess != -1) {
        do {
            keyRaw = eqmain_fn_1001a210_guess();
            key = (short)keyRaw;
            if (key != 0) {
                clickDone = false;
            }
            if (key == 0x1b) {
                if ((short)state == 0xf) {
                    done = true;
                }
                else if ((short)state == 0xc) {
                    eqmain_fn_10025840_guess();
                }
            }
            else if (key == 0x41) {
                done = true;
                state = 0xc;
                g_eqmain_flag_5b34_guess = 1;
            }
            else if (key == 0x45) {
                strcpy(g_eqmain_login_pin_buf_guess, text);
            }
            msg = eqmain_fn_10003bd0_guess();
            if (msg != 0) {
                if (*msg == 0) {
                    handle = msg[2];
                    if (handle != ctl1 || clickDone) {
                        if (handle != ctl2 || clickDone) {
                            if (handle != ctl3 || clickDone) {
                                if (handle == 0 && !clickDone) {
                                    state = 0x1a;
                                    g_chat_status_guess = 9;
                                    goto exitLoop;
                                }
                                if (handle == ctl4) {
                                    goto setStateOne;
                                }
                            }
                            else {
                                ok = eqmain_fn_10002b50_guess(text);
                                if (ok != '\0') {
                                    eqmain_fn_10023b10_guess(g_eqmain_login_pin_buf_guess, text);
                                }
                            }
                        }
                        else {
                            clickDone = true;
                            eqmain_fn_10025830_guess();
                            state = 0xf;
                        }
                    }
                    else {
                        clickDone = true;
                        eqmain_fn_10025830_guess();
                        state = 0xc;
                    }
                }
                else if (*msg == 4 && (char)msg[2] == 0x1b) {
                    eqmain_fn_10003a00_guess();
setStateOne:
                    state = 1;
exitLoop:
                    done = true;
                }
                eqmain_fn_10033568_guess(msg);
            }
            result = (unsigned short)state;
        } while (!done);
    }
    g_eqmain_busy_guess = 0;
    eqmain_fn_10004e10_guess(g_eqmain_ceb8_guess);
    eqmain_fn_100039e0_guess(0);
    return result;
}

// EQMain_ShowStartScreen_guess's eqmain.dll globals and callees.
#define g_eqmain_start_text_55808_guess EQ_GLOBAL(char *, 0x10055808)
#define g_eqmain_buf_guess EQ_ARRAY(char, 0x1004CF68)

// FUNCTION: EQMAIN 0x10015FA0
// Shows the EQVstart artwork with a configured front-end message.
unsigned short __stdcall EQMain_ShowStartScreen_guess(void)
{
    char *pCVar1;
    char tmp;
    unsigned int length;
    unsigned int dwordCount;
    char *src;
    char *dst;

    eqmain_fn_10007bf0_guess
        (g_eqmain_ceb8_guess, (char *)0x10047F7C, 0x10);
    g_eqmain_busy_guess = 1;
    EQ_STDCALL(void, 0x10003A00, (void))();
    if (g_eqmain_cf60_guess != -1) {
        if (g_eqmain_cf60_guess == 1) {
            tmp = EQ_STDCALL(char, 0x100030B0, (void))();
            if (tmp != '\0') {
                EQ_STDCALL(void, 0x100257F0, (void))();
                pCVar1 = g_str_888_guess;
                length = 0xFFFFFFFF;
                src = g_eqmain_start_text_55808_guess;
                do {
                    dst = src;
                    if (length == 0)
                        break;
                    length = length - 1;
                    dst = src + 1;
                    tmp = *src;
                    src = dst;
                } while (tmp != '\0');
                length = ~length;
                src = dst - length;
                dst = g_eqmain_copybuf_guess;
                for (dwordCount = length >> 2; dwordCount != 0; dwordCount = dwordCount - 1) {
                    *(unsigned int *)dst = *(unsigned int *)src;
                    src = src + 4;
                    dst = dst + 4;
                }
                for (length = length & 3; length != 0; length = length - 1) {
                    *dst = *src;
                    src = src + 1;
                    dst = dst + 1;
                }
                EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))
                    (g_eqmain_ceb8_guess, g_eqmain_copybuf_guess,
                     pCVar1, (char *)0);
            }
        }
        else {
            EQ_CDECL(void, 0x10001FF0, (int, char *, char *, char *))
                (g_eqmain_ceb8_guess, g_eqmain_buf_guess,
                 g_str_888_guess, (char *)0);
        }
    }
    g_eqmain_busy_guess = 0;
    EQ_CDECL(void, 0x100039E0, (int))(0);
    return 1;
}

// FUNCTION: EQMAIN 0x10016060
// Presents the account sign-in screen and responds to its UI events.
// __stdcall, no arguments (callee pops 0 bytes). Probably builds the login screen (name from the strings only): news.bmp (PackFsBlitNamedImageAtOrigin_guess), txaccount.bmp
// (Identity_guess -> DrawNamedImageCentered_guess), the window (eqmain_create_button_guess, eqmain_draw_text_guess) and two records (CreateNamedNode_guess -> g_eqmain_cf64_guess,
// CreateSlider_guess -> g_eqmain_cf28_guess). Unless g_eqmain_cf60_guess is -1 (then the loop is skipped) g_eqmain_d238_guess is cleared and
// eqmain_setup_push38_guess(0, 99) called. The string at g_eqmain_5820_guess is copied to g_eqmain_copybuf_guess and the flag g_eqmain_d220_guess set. The loop
// polls eqmain_process_messages_guess: code 2 calls eqmainRunDialog_guess, code 0x10000003 copies g_eqmain_581c_guess to 0x100F4C10 and then calls it too, and each pass takes a record
// from PumpMessagesAndEditText_guess (NULL is skipped). A type-4 record with 0x1B in its low byte calls TickBlinkAndCursor_guess and ends the loop.
// Teardown follows and the function returns 1 in AX only (the original writes AX alone, so the upper half of EAX is what
// ClearBlinkAndPump_guess left there).
unsigned short __stdcall EQMain_LoginScreen_guess(void)
{
    int hdc;
    unsigned int eventCode;
    int *rec;
    int eventType;
    bool done;

    done = false;
    eqmain_fn_10007bf0_guess(g_eqmain_ceb8_guess, eqmain_str_news_guess, 0x10);
    eqmain_fn_100056c0_guess(g_eqmain_ceb8_guess, eqmain_fn_100210f0_guess(eqmain_str_txaccount_guess), 0, 0);
    hdc = eqmain_fn_10001520_guess(g_eqmain_ceb8_guess, 0, 0, 0x11, 2);
    eqmain_fn_100017b0_guess(hdc, 0, g_str_888_guess, 1, 0);
    g_eqmain_cf64_guess = eqmain_fn_10005840_guess(g_eqmain_ceb8_guess, 0x32, 0x4a, 0x210, 0x150, 8, 0x21a3c);
    g_eqmain_cf28_guess = eqmain_fn_10006950_guess(g_eqmain_ceb8_guess, 0x265, 0x3d, 0x10, 0x170, 0);
    g_eqmain_busy_guess = 1;
    eqmain_fn_10003a00_guess();
    if (g_eqmain_cf60_guess != -1) {
        g_eqmain_d238_guess = 0;
        eqmain_fn_10025800_guess(0, 99);
    } else {
        done = true;
    }
    strcpy(g_eqmain_copybuf_guess, g_eqmain_5820_guess);
    g_eqmain_d220_guess = 1;
    eqmain_fn_10003000_guess(g_eqmain_copybuf_guess);
    if (!done) {
        do {
            eventCode = eqmain_fn_100254f0_guess();
            if (eventCode != 0 && g_eqmain_d220_guess != 0) {
                g_eqmain_d220_guess = 0;
                eqmain_fn_10003090_guess();
            }
            switch (eventCode) {
            case 0x10000003:
                strcpy(g_eqmain_copybuf_guess, g_eqmain_581c_guess);
                // falls through into the eqmainRunDialog_guess call, as the original's code does
            case 2:
                eqmain_fn_10001ff0_guess((int *)g_eqmain_ceb8_guess, g_eqmain_copybuf_guess, g_str_888_guess, (char *)0);
                break;
            }
            rec = eqmain_fn_10003bd0_guess();
            if (rec != 0) {
                eventType = *rec;
                switch (eventType) {
                case 4:
                    if ((char)rec[2] == 0x1b) {
                        eqmain_fn_10003a00_guess();
                        done = true;
                    }
                    break;
                case 1:
                    if ((int *)rec[2] == g_eqmain_cf28_guess) {
                        g_eqmain_cf64_guess[0xb] = g_eqmain_cf28_guess[0x1a];
                        eqmain_fn_10005a00_guess(g_eqmain_cf64_guess);
                    }
                    break;
                case 0:
                    if (rec[2] == hdc) {
                        eqmain_fn_100044b0_guess(4, 0x1b, 0, 0);
                    }
                    break;
                }
                eqmain_fn_10033568_guess(rec);
            }
        } while (!done);
    }
    g_eqmain_busy_guess = 0;
    eqmain_fn_10001700_guess(hdc);
    eqmain_fn_10005930_guess(g_eqmain_cf64_guess);
    eqmain_fn_10006ae0_guess(g_eqmain_cf28_guess);
    eqmain_fn_100039e0_guess(0);
    return 1;
}
