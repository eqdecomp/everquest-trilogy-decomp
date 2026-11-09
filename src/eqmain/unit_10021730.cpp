// unit_10021730.cpp - eqmain.dll, the address run 0x10021730..0x100224B0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>

// This image's addresses for the function below (file-local). The CRT's __strcmpi is at 0x10033730.
#define g_eqmain_name_guess         EQ_ARRAY(char, 0x10104C70)              // g_eqmain_name_guess: the name, compared first
#define g_buf_101255f8_guess     EQ_ARRAY(char, 0x101255F8)              // DAT_101255f8: the name it is compared with
#define g_eqmain_message_guess      EQ_ARRAY(unsigned char, 0x10125914)     // DAT_10125914: the message buffer

// eqmain.dll's own globals (the same addresses unit_1001DC50.cpp uses).
#define g_eqmain_connection_guess   EQ_GLOBAL(int, 0x10125848)
#define g_eqmain_state_guess        EQ_GLOBAL(unsigned int, 0x1012B918)

// This image's addresses for BuildTaggedText_guess (file-local). The tag strings sit 4 bytes apart at g_eqmain_tag_a_guess..18.
#define g_eqmain_tag_a_guess        EQ_ARRAY(char, 0x10048600)              // g_eqmain_tag_a_guess
#define g_eqmain_tag_b_guess        EQ_ARRAY(char, 0x10048604)              // g_eqmain_tag_b_guess
#define g_eqmain_tag_c_guess        EQ_ARRAY(char, 0x10048608)              // g_eqmain_tag_c_guess
#define g_eqmain_tag_d_guess        EQ_ARRAY(char, 0x1004860C)              // g_eqmain_tag_d_guess
#define g_eqmain_tag_e_guess        EQ_ARRAY(char, 0x10048610)              // g_eqmain_tag_e_guess
#define g_eqmain_tag_f_guess        EQ_ARRAY(char, 0x10048614)              // g_eqmain_tag_f_guess
#define g_eqmain_tag_g_guess        EQ_ARRAY(char, 0x10048618)              // g_eqmain_tag_g_guess
#define g_eqmain_text_buf_guess     EQ_ARRAY(char, 0x100F4C10)              // g_eqmain_copybuf_guess: the text EQMain_UpdateQueueView_guess gets
#define g_eqmain_name_ptr_guess     EQ_GLOBAL(char *, 0x101258A4)           // g_eqmain_name_ptr_guess: the this of FindRecordIndexByName_guess (a string)

// This image's addresses for SendNameTextMessage_guess (file-local). DAT_10047f64 is the keyword the first argument is
// compared with; g_eqmain_login_name_buf_guess and g_eqmain_login_pin_buf_guess keep copies of the first and second arguments.
#define g_str_10047f64_guess      EQ_ARRAY(char, 0x10047F64)              // DAT_10047f64
#define g_eqmain_login_name_buf_guess    EQ_ARRAY(char, 0x10104CD8)              // g_eqmain_login_name_buf_guess: a copy of the first argument
#define g_eqmain_login_pin_buf_guess    EQ_ARRAY(char, 0x1012565C)              // g_eqmain_login_pin_buf_guess: a copy of the second argument
#define g_eqmain_mode_guess         EQ_GLOBAL(short, 0x1004814C)            // g_eqmain_login_code_guess: picks the first message word
#define g_eqmain_none_guess         EQ_ARRAY(char, 0x100485C0)              // g_eqmain_none_guess: "none"

// This image's addresses for RdpReceiveDispatch_guess (file-local). g_eqmain_rdp_guess is Eqmain_RdpReceiveIfReady_guess's first argument
// (rdp_receive_guess); g_eqmain_fail_format_guess is the format the error text is printed with; g_eqmain_src_5714_guess and g_eqmain_src_572c_guess are copy sources.
#define g_eqmain_rdp_guess          EQ_GLOBAL(int, 0x10125638)              // g_eqmain_rdp_guess
#define g_eqmain_fail_format_guess  EQ_GLOBAL(char *, 0x100556F4)           // g_eqmain_fail_format_guess: the format of the error text
#define g_eqmain_src_5714_guess     EQ_GLOBAL(char *, 0x10055714)           // g_eqmain_src_5714_guess: copied to the text buffer
#define g_eqmain_src_572c_guess     EQ_GLOBAL(char *, 0x1005572C)           // g_eqmain_src_572c_guess: copied to the text buffer
#define g_eqmain_drive_base_guess EQ_ARRAY(char, 0x1004CE40)              // g_eqmain_drive_base_guess: a default string
#define g_eqmain_copy_c10_guess     EQ_ARRAY(char, 0x10104C10)              // g_eqmain_copy_c10_guess: a copy of the default string
#define g_eqmain_copy_5450_guess    EQ_ARRAY(char, 0x10055450)              // g_eqmain_copy_5450_guess: a copy of a message string
#define g_eqmain_copy_cec_guess     EQ_ARRAY(char, 0x10104CEC)              // g_eqmain_copy_cec_guess: a copy of a message string
#define g_eqmain_buf_guess    EQ_ARRAY(char, 0x1004CF68)              // g_eqmain_buf_guess: a copy of a message string
#define g_eqmain_value_cf60_guess   EQ_GLOBAL(unsigned int, 0x1004CF60)     // g_eqmain_cf60_guess: a dword from the message
#define g_eqmain_value_58c8_guess   EQ_GLOBAL(unsigned int, 0x101258C8)     // g_eqmain_value_58c8_guess: a dword from the message
#define g_eqmain_flag_a_guess       EQ_GLOBAL(unsigned char, 0x1012584C)    // g_eqmain_flag_a_guess: a flag the opcodes set to 1
#define g_eqmain_flag_b_guess       EQ_GLOBAL(unsigned char, 0x101258B1)    // g_eqmain_flag_b_guess: a flag the opcodes set to 0 or 1
#define g_eqmain_block_b920_guess   EQ_ARRAY(unsigned int, 0x1012B920)      // g_eqmain_block_b920_guess: a 0x674-byte block

// This image's addresses for ConnectAndDescribe_guess (file-local). g_eqmain_cycle_text_guess and g_eqmain_port_guess are the host and the port
// of the connect message ("...", at g_eqmain_connect_fmt_guess); port 0 means there is no connect.
#define g_eqmain_cycle_text_guess         EQ_ARRAY(char, 0x10125850)              // g_eqmain_cycle_text_guess: the %s of the message
#define g_eqmain_port_guess         EQ_GLOBAL(int, 0x101258A0)              // g_eqmain_port_guess: the %d of the message
#define g_eqmain_src_5720_guess     EQ_GLOBAL(char *, 0x10055720)           // g_eqmain_src_5720_guess: copied to buf when there is no port
#define g_eqmain_fail_format2_guess EQ_GLOBAL(char *, 0x100556F8)           // g_eqmain_fail_format2_guess: the format of the error text (result 2)
#define g_eqmain_connect_fmt_guess  EQ_ARRAY(char, 0x100485A4)              // "..."

// FUNCTION: EQMAIN 0x10021730
// Connects to the configured front-end server and provides connection status text.
// __cdecl, 1 arg, returns bool (callers read AL only). Returns true when Eqmain_GlobalNonzero_guess is nonzero. Else, with port 0, it copies
// the string at g_eqmain_src_5720_guess (NUL included) into buf and returns false. With a port it logs the connect message (FUN_10027940,
// three args the original pops after ConnectAndCheckReply_guess), connects with ConnectAndCheckReply_guess(host, port, &result) and, when that returns 0,
// writes the error text into buf: g_eqmain_fail_format_guess for result 1, g_eqmain_fail_format2_guess for result 2, nothing else; returns false.
// Returns true when ConnectAndCheckReply_guess is nonzero. The meaning of the result codes is unknown.
bool __cdecl ConnectAndDescribe_guess(char *textBuffer)
{
    int resultCode;
    if (!EQ_STDCALL(bool, 0x10021410, (void))())
    {
        if (g_eqmain_port_guess == 0)
        {
            strcpy(textBuffer, g_eqmain_src_5720_guess);
            return false;
        }
        EQ_CDECL(void, 0x10027940, (const char *, char *, int))(g_eqmain_connect_fmt_guess, g_eqmain_cycle_text_guess,
                                                                 g_eqmain_port_guess);
        if (!EQ_CDECL(bool, 0x100214A0, (char *, int, int *))(g_eqmain_cycle_text_guess, g_eqmain_port_guess, &resultCode))
        {
            switch (resultCode)
            {
            case 2:
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))(textBuffer, g_eqmain_fail_format2_guess);
                return false;
            case 1:
                EQ_CDECL(int, 0x10034117, (char *, const char *, ...))(textBuffer, g_eqmain_fail_format_guess);
                break;
            default:
                break;
            }
            return false;
        }
    }
    return true;
}

// FUNCTION: EQMAIN 0x100217E0
// Sends a multi-field front-end text payload; its game purpose is unclear.
// __cdecl, 4 args, void: no caller reads EAX. When text is the keyword at DAT_10047f64 (__strcmpi, case insensitive) it
// sends the 2-byte value 0x1a over g_eqmain_connection_guess (connection_send_guess, flags 1 and 1) and returns. Otherwise it
// builds the message in g_eqmain_message_guess and sends it with length g_eqmain_state_guess: a word (0x51 when g_eqmain_login_code_guess
// is 9, 10, 0xb or 0xf, else 1; SetStateAndWord_guess), a 40-byte block (desCryptGlobalKey_guess encodes it, AppendBytes_guess appends it) made of
// 20 bytes from CopyStringToBuffer_guess of the lower-cased copy of text and 20 bytes from CopyStringToBuffer_guess of textSrc (17 copied, the
// last 3 left as they were on the stack), the string CopyStringToBuffer_guess gives for optionalSrc ("none" when 0; AppendString_guess), then
// value and 1 (AppendDword_guess). The offset is not reset here: the message starts at whatever g_eqmain_state_guess holds (bug kept).
// The meaning of the message is unknown.
void __cdecl SendNameTextMessage_guess(char *text, char *textSrc, char *optionalSrc, unsigned int value)
{
    // The original's frame: its six locals sit at 0x00, 0x04, 0x18, 0x2c, 0x40 and 0x68, in this order. VC6 lays out
    // separate locals in another order, so they are members of one struct (VC6 keeps declaration order inside it).
    struct
    {
        int local_90;
        char local_8c[20];
        unsigned int local_78[5];
        char local_64[20];
        unsigned int local_50[10];
        unsigned int local_28[10];
    } frame;
    if (EQ_CDECL(int, 0x10033730, (char *, char *))(text, g_str_10047f64_guess) == 0)
    {
        frame.local_90 = 0x1a;
        EQ_CDECL(void, 0x1002C010, (int, void *, int, unsigned int, unsigned int))(
            g_eqmain_connection_guess, &frame.local_90, 2, 1, 1);
        return;
    }
    strcpy(g_eqmain_login_name_buf_guess, text);
    strcpy(frame.local_8c, text);
    EQ_CDECL(char *, 0x10035C00, (char *))(frame.local_8c);
    strcpy(g_eqmain_login_pin_buf_guess, textSrc);
    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))(
        frame.local_64, EQ_CDECL(char *, 0x100211B0, (char *))(textSrc), 0x11);
    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))(
        (char *)frame.local_78, EQ_CDECL(char *, 0x100211B0, (char *))(frame.local_8c), 0x14);
    memcpy(frame.local_50, frame.local_78, 0x28);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(
        frame.local_50, (unsigned char *)frame.local_28, 0x28);
    if (g_eqmain_mode_guess == 9 || g_eqmain_mode_guess == 10 || g_eqmain_mode_guess == 0xb || g_eqmain_mode_guess == 0xf)
    {
        EQ_CDECL(void, 0x10024F90, (unsigned short))(0x51);
    }
    else
    {
        EQ_CDECL(void, 0x10024F90, (unsigned short))(1);
    }
    EQ_CDECL(void, 0x10025060, (unsigned int *, unsigned int))(frame.local_28, 0x28);
    if (optionalSrc != 0)
    {
        EQ_CDECL(void, 0x10024FF0, (char *))(
            EQ_CDECL(char *, 0x100211B0, (char *))(optionalSrc));
    }
    else
    {
        EQ_CDECL(void, 0x10024FF0, (char *))(
            EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_none_guess));
    }
    EQ_CDECL(void, 0x10024FD0, (unsigned int))(value);
    EQ_CDECL(void, 0x10024FD0, (unsigned int))(1);
    EQ_CDECL(void, 0x1002C010, (int, void *, int, unsigned int, unsigned int))(
        g_eqmain_connection_guess, g_eqmain_message_guess, g_eqmain_state_guess, 1, 1);
}

// FUNCTION: EQMAIN 0x100219A0
// __cdecl, one char * arg (callee pops 0), void: no caller reads EAX. Copies the first 24 bytes of the string into a
// local block (_strncpy, zero padded), runs desCryptGlobalKey_guess on it into a second 24-byte local (what the transform is is
// unknown), writes 0x6E (SetStateAndWord_guess) and then appends that block (AppendBytes_guess) to the message buffer, and sends the
// buffer with length g_eqmain_state_guess over g_eqmain_connection_guess (connection_send_guess, flags 1 and 1).
void __cdecl SendNameMessage_guess(char *text)
{
    unsigned int inputBlock[6];
    unsigned int outputBlock[6];

    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))((char *)inputBlock, text, 0x18);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(inputBlock, (unsigned char *)outputBlock, 0x18);
    EQ_CDECL(void, 0x10024F90, (unsigned short))(0x6e);
    EQ_CDECL(void, 0x10025060, (unsigned int *, unsigned int))(outputBlock, 0x18);
    EQ_CDECL(void, 0x1002C010, (int, unsigned char *, unsigned int, unsigned int, unsigned int))(
        g_eqmain_connection_guess, g_eqmain_message_guess, g_eqmain_state_guess, 1, 1);
}

// FUNCTION: EQMAIN 0x10021A00
// __cdecl, one char * arg (callee pops 0), void: no caller reads EAX. The same as SendNameMessage_guess (0x100219A0)
// except the opcode: copies the first 24 bytes of the string into a local block (_strncpy, zero padded), runs
// desCryptGlobalKey_guess on it into a second 24-byte local (what the transform is is unknown), writes 0x6F (SetStateAndWord_guess) and then
// appends that block (AppendBytes_guess) to the message buffer, and sends the buffer with length g_eqmain_state_guess over
// g_eqmain_connection_guess (connection_send_guess, flags 1 and 1).
void __cdecl SendNameMessage6F_guess(char *text)
{
    unsigned int inputBlock[6];
    unsigned int outputBlock[6];

    EQ_CDECL(char *, 0x10033330, (char *, char *, unsigned int))((char *)inputBlock, text, 0x18);
    EQ_CDECL(void, 0x100312E0, (unsigned int *, unsigned char *, int))(inputBlock, (unsigned char *)outputBlock, 0x18);
    EQ_CDECL(void, 0x10024F90, (unsigned short))(0x6f);
    EQ_CDECL(void, 0x10025060, (unsigned int *, unsigned int))(outputBlock, 0x18);
    EQ_CDECL(void, 0x1002C010, (int, unsigned char *, unsigned int, unsigned int, unsigned int))(
        g_eqmain_connection_guess, g_eqmain_message_guess, g_eqmain_state_guess, 1, 1);
}

// This image's addresses for SisaTransparentLogin_guess (file-local). g_eqmain_sisa_check_guess is the message FUN_10027940 gets when the
// buffer check fails; g_eqmain_ceb8_guess is the flag the wait loop tests before it calls PumpMessagesAndEditText_guess (a message pump).
#define g_eqmain_sisa_check_guess   EQ_ARRAY(char, 0x100485C8)              // g_eqmain_sisa_check_guess: "..."
#define g_parent_guess    EQ_GLOBAL(unsigned int, 0x1004CEB8)     // g_eqmain_ceb8_guess: nonzero: the wait loop pumps messages

// FUNCTION: EQMAIN 0x10021A60
// Runs a front-end request exchange; the meaning of reply code 4 is unclear.
// __cdecl, three char * args (callee pops 0), bool: callers read AL. When ConnectAndDescribe_guess on g_eqmain_text_buf_guess returns 0, it
// reports g_eqmain_sisa_check_guess (FUN_10027940) and returns false. Otherwise it runs SendNameTextMessage_guess on the three args with 1 as
// the fourth, then loops: while g_eqmain_pump_flag_guess is nonzero it calls PumpMessagesAndEditText_guess, and it polls RdpReceiveDispatch_guess until that
// returns nonzero. Returns whether that value is 4.
bool __cdecl SisaTransparentLogin_guess(char *text, char *textSrc, char *optionalSrc)
{
    char connectionResult;
    unsigned int receiveResult;

    connectionResult = EQ_CDECL(char, 0x10021730, (char *))(g_eqmain_text_buf_guess);
    if (connectionResult == '\0')
    {
        EQ_CDECL(void, 0x10027940, (char *))(g_eqmain_sisa_check_guess);
        return false;
    }
    else
    {
        EQ_CDECL(void, 0x100217E0, (char *, char *, char *, int))(text, textSrc, optionalSrc, 1);
        do
        {
            if (g_parent_guess != 0)
            {
                EQ_STDCALL(int, 0x10003BD0, (void))();
            }
            receiveResult = EQ_STDCALL(unsigned int, 0x10021C20, (void))();
        } while (receiveResult == 0);
        if (receiveResult == 4)
            return true;
        else
            return false;
    }
}

// This image's address for TryTransparentLogin_guess (file-local). g_eqmain_login_s_login_guess is the string "login".
#define g_eqmain_login_s_login_guess    EQ_ARRAY(char, 0x10048100)              // g_eqmain_login_s_login_guess: "login"

// FUNCTION: EQMAIN 0x10021AC0
// Starts the front-end login exchange when the "login" mode is available.
// __cdecl, three char * args (callee pops 0), bool: callers read AL. When OpenLoginConnection_guess on "login" returns 0, it returns false.
// Otherwise it calls SisaTransparentLogin_guess with the three args and returns true when that returns true, else false.
bool __cdecl TryTransparentLogin_guess(char *text, char *textSrc, char *optionalSrc)
{
    char modeResult;
    bool attemptResult;

    modeResult = EQ_CDECL(char, 0x100212E0, (char *))(g_eqmain_login_s_login_guess);
    if (modeResult != '\0')
    {
        attemptResult = EQ_CDECL(bool, 0x10021A60, (char *, char *, char *))(text, textSrc, optionalSrc);
        if (attemptResult)
        {
            return true;
        }
    }
    return false;
}

// FUNCTION: EQMAIN 0x10021B00
// Sends an indexed front-end text entry when it differs from the reference; its role is unclear.
// __cdecl, one int arg (callee pops 0), void: no caller reads EAX. An index below 0, or at or above the ushort count at
// g_eqmain_name_ptr_guess + 0x36B0, does nothing. Otherwise it copies the 64-byte slot (short)index of that table into a
// local buffer with no length check (a name of 64 or more chars overruns it: bug kept). When the copy is not the same
// string as DAT_101255f8 (__strcmpi, case insensitive), it writes 7 (SetStateAndWord_guess) and a 0 byte (AppendByte_guess) to the
// message buffer, appends CopyStringToBuffer_guess's result for the copy (AppendString_guess), and sends the buffer with length
// g_eqmain_state_guess over g_eqmain_connection_guess (connection_send_guess, flags 1 and 1). The meaning of the message
// is unknown.
void __cdecl SendIndexedNameIfDifferent_guess(int index)
{
    char entryText[64];
    int compareResult;
    char *text;

    if (index >= 0 && index < *(unsigned short *)(g_eqmain_name_ptr_guess + 0x36B0))
    {
        strcpy(entryText, g_eqmain_name_ptr_guess + (short)index * 0x40);
        if (entryText != 0)
        {
            compareResult = EQ_CDECL(int, 0x10033730, (char *, char *))(entryText, g_buf_101255f8_guess);
            if (compareResult != 0)
            {
                EQ_CDECL(void, 0x10024F90, (short))(7);
                EQ_CDECL(void, 0x10024FB0, (unsigned char))(0);
                text = EQ_CDECL(char *, 0x100211B0, (char *))(entryText);
                EQ_CDECL(void, 0x10024FF0, (char *))(text);
                EQ_CDECL(void, 0x1002C010, (int, unsigned int, unsigned int, unsigned int, unsigned int))(
                    g_eqmain_connection_guess, (unsigned int)g_eqmain_message_guess, g_eqmain_state_guess, 1, 1);
            }
        }
    }
}

// FUNCTION: EQMAIN 0x10021bc0
// Sends changed front-end text with a second string; its game purpose is unclear.
// __cdecl, one char * arg (callee pops 0). When g_eqmain_name_guess is not the same string as DAT_101255f8 (__strcmpi, case
// insensitive), it writes '-' into the message buffer at 0x10125914 (SetStateAndWord_guess), appends g_eqmain_name_guess and then the
// arg (each through CopyStringToBuffer_guess and AppendString_guess), and sends the buffer with length g_eqmain_state_guess over
// g_eqmain_connection_guess (connection_send_guess, flags 1 and 1). Returns 1 in AL on every path: the original's
// MOV AL,1, so the upper 24 bits of EAX are whatever the last call left. No caller reads EAX. The meaning of the message
// is unknown.
unsigned char __cdecl SendNameChange_guess(char *src)
{
    int compareResult;
    char *text;

    compareResult = EQ_CDECL(int, 0x10033730, (char *, char *))(g_eqmain_name_guess, g_buf_101255f8_guess);
    if (compareResult != 0)
    {
        EQ_CDECL(void, 0x10024F90, (short))(0x2d);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(g_eqmain_name_guess);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        text = EQ_CDECL(char *, 0x100211B0, (char *))(src);
        EQ_CDECL(void, 0x10024FF0, (char *))(text);
        EQ_CDECL(void, 0x1002C010, (int, unsigned int, unsigned int, unsigned int, unsigned int))(
            g_eqmain_connection_guess, (unsigned int)g_eqmain_message_guess, g_eqmain_state_guess, 1, 1);
    }
    return 1;
}

// FUNCTION: EQMAIN 0x10021C20
// Processes front-end replies that update pending text and response state.
// __stdcall, no args (callee pops 0). Takes one message from Eqmain_RdpReceiveIfReady_guess(g_eqmain_rdp_guess, 10). No message: returns 0.
// GetRecordBase_guess returning 0 frees the handle and returns 0. GetRecordDword6D4_guess returning 0, or no message data
// (msg_arrival_get_data_guess), frees the handle and prints the error text (_sprintf of g_eqmain_fail_format_guess into the text buffer):
// returns 0x10000003. Otherwise the message's first word is the opcode (opcode). The switch copies strings (strcpy, no
// bound) and sets flags for some opcodes. Opcode 0x43 frees and returns 2; opcode 0x1b with eqmain_load_string_tables_guess's low byte 0 copies
// g_eqmain_src_5714_guess and returns 2 without freeing; the other cases free the handle and return the opcode (0x1b with a nonzero
// low byte, 0x70..0x73 and the default included). The meaning of the message is unknown.
unsigned int __stdcall RdpReceiveDispatch_guess(void)
{
    int msg;
    int record;
    unsigned short *msgData;
    unsigned char *encoded;
    char *decoded;
    unsigned int result;
    unsigned int opcode;

    opcode = 0;
    msg = EQ_CDECL(int, 0x10021420, (int, unsigned int))(g_eqmain_rdp_guess, 10);
    if (msg != 0)
    {
        record = EQ_CDECL(int, 0x1002DE90, (int))(msg);
        if (record != 0)
        {
            record = EQ_CDECL(int, 0x1002DD30, (int))(record);
            if (record == 0)
            {
                EQ_CDECL(void, 0x10028A90, (int))(msg);
fail:
                EQ_CDECL(int, 0x10034117, (char *, char *))(g_eqmain_text_buf_guess, g_eqmain_fail_format_guess);
                return 0x10000003;
            }
            msgData = EQ_CDECL(unsigned short *, 0x1002DE70, (int))(msg);
            if (msgData == 0)
            {
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                goto fail;
            }
            opcode = *msgData;
            switch (opcode)
            {
            case 2:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_text_buf_guess, decoded);
                g_eqmain_flag_b_guess = 0;
                strcpy(g_eqmain_login_name_buf_guess, g_eqmain_drive_base_guess);
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 4:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                strcpy(g_eqmain_copy_c10_guess, g_eqmain_drive_base_guess);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_copy_5450_guess, decoded);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_copy_cec_guess, decoded);
                g_eqmain_value_58c8_guess = EQ_STDCALL(unsigned int, 0x10025120, ())();
                g_eqmain_flag_a_guess = 1;
                g_eqmain_flag_b_guess = 1;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 0x1b:
                g_eqmain_flag_a_guess = 1;
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                result = EQ_STDCALL(unsigned int, 0x100253D0, ())();
                if ((char)result == 0)
                {
                    strcpy(g_eqmain_text_buf_guess, g_eqmain_src_5714_guess);
                    return 2;
                }
                break;
            case 0x41:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                EQ_CDECL(void, 0x10025260, (unsigned int *, unsigned int))(g_eqmain_block_b920_guess, 0x674);
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 0x42:
                g_eqmain_flag_a_guess = 1;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 0x43:
                strcpy(g_eqmain_text_buf_guess, g_eqmain_src_572c_guess);
                g_eqmain_flag_b_guess = 0;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return 2;
            case 0x45:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_text_buf_guess, decoded);
                g_eqmain_flag_b_guess = 0;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 0x51:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_copy_5450_guess, decoded);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_copy_cec_guess, decoded);
                g_eqmain_value_cf60_guess = EQ_STDCALL(unsigned int, 0x10025120, ())();
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_buf_guess, decoded);
                g_eqmain_flag_a_guess = 1;
                g_eqmain_flag_b_guess = 1;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_text_buf_guess, decoded);
                break;
            case 0x77:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(msgData);
                encoded = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                decoded = EQ_CDECL(char *, 0x10021110, (unsigned char *))(encoded);
                strcpy(g_eqmain_text_buf_guess, decoded);
                g_eqmain_flag_b_guess = 1;
                EQ_CDECL(void, 0x10028A90, (int))(msg);
                return opcode;
            }
        }
        EQ_CDECL(void, 0x10028A90, (int))(msg);
    }
    return opcode;
}

// This image's addresses for RdpReceiveScreenDispatch_guess (file-local). g_eqmain_counts_guess holds five record counts; each count has a group of
// ten 0xd0-byte records from g_eqmain_records_guess on (0x820 bytes apart): a dword at -4, a dword at 0, then a name at +4.
#define g_eqmain_value_1164_guess   EQ_GLOBAL(unsigned int, 0x100F1164)     // g_choice_count_guess: a dword from the message
#define g_choice_text_guess    EQ_ARRAY(char, 0x100F1168)              // g_choice_text_guess: a copy of a message string
#define g_sel_name_guess    EQ_ARRAY(char, 0x100554B0)              // g_sel_name_guess: a copy of a message string
#define g_eqmain_counts_guess       EQ_ARRAY(int, 0x101258B4)               // g_eqmain_counts_guess: the record counts
#define g_eqmain_records_guess      EQ_ARRAY(char, 0x100F207C)              // g_eqmain_records_guess: the first record's second dword
#define EQMAIN_COUNTS_END_GUESS     0x101258C8
#define EQMAIN_RECORD_SIZE          0xd0
#define EQMAIN_GROUP_SIZE           0x820

// FUNCTION: EQMAIN 0x100220F0
// Handles front-end screen replies that update text and grouped record lists.
// __stdcall, no args. Like RdpReceiveDispatch_guess but a different opcode set (2, 0x46, 0x47, 0x49, 0x52, 0x55, 0x57,
// 0x60, 0x63); failures print the error text and return 2. Opcode 0x47 returns 2 when its first string is not empty.
unsigned int __stdcall RdpReceiveScreenDispatch_guess(void)
{
    unsigned int opcode;
    int handle;
    int body;
    unsigned short *data;
    unsigned char *raw_text;
    char *text;
    int *count;
    char *record;
    char *name;
    int index;

    opcode = 0;
    handle = EQ_CDECL(int, 0x10021420, (int, unsigned int))(g_eqmain_rdp_guess, 10);
    if (handle != 0)
    {
        body = EQ_CDECL(int, 0x1002DE90, (int))(handle);
        if (body != 0)
        {
            body = EQ_CDECL(int, 0x1002DD30, (int))(body);
            if (body == 0)
            {
                EQ_CDECL(void, 0x10028A90, (int))(handle);
receiveFailed:
                EQ_CDECL(int, 0x10034117, (char *, char *))(g_eqmain_text_buf_guess, g_eqmain_fail_format_guess);
                return 2;
            }
            data = EQ_CDECL(unsigned short *, 0x1002DE70, (int))(handle);
            if (data == 0)
            {
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                goto receiveFailed;
            }
            opcode = *data;
            switch (opcode)
            {
            case 0x63:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(data);
                record = g_eqmain_records_guess;
                count = g_eqmain_counts_guess;
                do
                {
                    *count = EQ_STDCALL(int, 0x10025120, ())();
                    index = 0;
                    if (*count > 0)
                    {
                        name = record;
                        do
                        {
                            *(unsigned int *)(name - 4) = EQ_STDCALL(unsigned int, 0x10025120, ())();
                            *(unsigned int *)name = EQ_STDCALL(unsigned int, 0x10025120, ())();
                            raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                            strcpy(name + 4, (char *)raw_text);
                            name += EQMAIN_RECORD_SIZE;
                            index++;
                        } while (index < *count);
                    }
                    count++;
                    record += EQMAIN_GROUP_SIZE;
                } while ((int)count < EQMAIN_COUNTS_END_GUESS);
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                return opcode;
            case 0x60:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(data);
                g_eqmain_value_1164_guess = EQ_STDCALL(unsigned int, 0x10025120, ())();
                raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(raw_text);
                strcpy(g_choice_text_guess, text);
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                return opcode;
            case 0x57:
                EQ_CDECL(void, 0x10018990, (unsigned short *))(data);
                break;
            case 0x55:
                EQ_CDECL(void, 0x10018AE0, (unsigned short *))(data);
                break;
            case 2:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(data);
                raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(raw_text);
                strcpy(g_eqmain_text_buf_guess, text);
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                return opcode;
            case 0x47:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(data);
                raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(raw_text);
                strcpy(g_eqmain_text_buf_guess, text);
                if (strlen(g_eqmain_text_buf_guess) != 0)
                {
                    opcode = 2;
                    EQ_CDECL(void, 0x10028A90, (int))(handle);
                    return opcode;
                }
                raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(raw_text);
                strcpy(g_eqmain_copy_cec_guess, text);
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                return opcode;
            case 0x52:
                EQ_CDECL(void, 0x10001230, (unsigned short *))(data);
                break;
            case 0x49:
                EQ_CDECL(void, 0x100250A0, (unsigned short *))(data);
                raw_text = EQ_STDCALL(unsigned char *, 0x10025150, ())();
                text = EQ_CDECL(char *, 0x10021110, (unsigned char *))(raw_text);
                strcpy(g_sel_name_guess, text);
                EQ_CDECL(void, 0x10028A90, (int))(handle);
                return opcode;
            case 0x46:
                EQ_CDECL(void, 0x1001F140, (unsigned short *))(data);
                break;
            }
        }
        EQ_CDECL(void, 0x10028A90, (int))(handle);
    }
    return opcode;
}

// FUNCTION: EQMAIN 0x100224B0
// Prepares tagged text for a front-end message.
// __cdecl, void (no caller reads EAX). Builds the text in g_eqmain_text_buf_guess, then calls EQMain_UpdateQueueView_guess with
// (text, value, kind). kind is a dword: the switch takes its low byte as a signed char, and the whole dword is
// passed on. Tags per case: 1 A B, 4 E, 5 G, 6 C D, 7 B, 8 E D. Cases 1, 6 and 8 copy the arg when it has no ':'.
// With a ':' the text is tag + the arg up to and including the ':' + second tag + the arg from two chars past the ':'
// (the char after the ':' is lost and a NUL is written there in the arg itself: bug kept). Case 8 with the ':' as the
// arg's first char is tag F + the arg after the ':'. Case 8 calls FindRecordIndexByName_guess first (its result is unused). Any other
// low byte (0, 2, 3, 9 and up, negatives) only makes the call, the text untouched.
void __cdecl BuildTaggedText_guess(char *arg, int value, int kind)
{
    char *colon;

    switch ((char)kind)
    {
    case 5:
        strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_g_guess);
        strcat(g_eqmain_text_buf_guess, arg);
        break;
    case 8:
        EQ_METHOD(1, unsigned int, 0x10026040, (char *, char *), g_eqmain_name_ptr_guess)(g_buf_101255f8_guess);
        colon = EQ_CDECL(char *, 0x10033670, (char *, int))(arg, 0x3a);
        if (colon == 0)
        {
            strcpy(g_eqmain_text_buf_guess, arg);
        }
        else if (colon == arg)
        {
            strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_f_guess);
            strcat(g_eqmain_text_buf_guess, colon + 1);
        }
        else
        {
            strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_e_guess);
            colon[1] = '\0';
            strcat(g_eqmain_text_buf_guess, arg);
            strcat(g_eqmain_text_buf_guess, g_eqmain_tag_d_guess);
            strcat(g_eqmain_text_buf_guess, colon + 2);
        }
        break;
    case 6:
        colon = EQ_CDECL(char *, 0x10033670, (char *, int))(arg, 0x3a);
        if (colon != 0)
        {
            strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_c_guess);
            colon[1] = '\0';
            strcat(g_eqmain_text_buf_guess, arg);
            strcat(g_eqmain_text_buf_guess, g_eqmain_tag_d_guess);
            strcat(g_eqmain_text_buf_guess, colon + 2);
        }
        else
        {
            strcpy(g_eqmain_text_buf_guess, arg);
        }
        break;
    case 4:
        strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_e_guess);
        strcat(g_eqmain_text_buf_guess, arg);
        break;
    case 7:
        strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_b_guess);
        strcat(g_eqmain_text_buf_guess, arg);
        break;
    case 1:
        colon = EQ_CDECL(char *, 0x10033670, (char *, int))(arg, 0x3a);
        if (colon != 0)
        {
            strcpy(g_eqmain_text_buf_guess, g_eqmain_tag_a_guess);
            colon[1] = '\0';
            strcat(g_eqmain_text_buf_guess, arg);
            strcat(g_eqmain_text_buf_guess, g_eqmain_tag_b_guess);
            strcat(g_eqmain_text_buf_guess, colon + 2);
        }
        else
        {
            strcpy(g_eqmain_text_buf_guess, arg);
        }
        break;
    }
    EQ_CDECL(void, 0x10018440, (char *, int, int))(g_eqmain_text_buf_guess, value, kind);
    return;
}
