// unit_100214A0.cpp - eqmain.dll, the address run 0x100214A0..0x100216EB (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"
#include <string.h>
#include <time.h>

// This image's addresses for ConnectAndCheckReply_guess (file-local). g_eqmain_rdp_guess is the first argument of
// Eqmain_RdpReceiveIfReady_guess (rdp_receive) and of createNetworkConnectionByName_guess (connect); g_eqmain_connection_guess is the connection those two use.
#define g_eqmain_rdp_guess              EQ_GLOBAL(int, 0x10125638)              // g_eqmain_rdp_guess
#define g_eqmain_connection_guess       EQ_GLOBAL(int, 0x10125848)              // g_eqmain_connection_guess: the connection
#define g_eqmain_connection_ref_guess   EQ_ARRAY(int, 0x10125848)              // &g_eqmain_connection_guess: the out pointer
#define g_eqmain_cycle_text_guess       EQ_ARRAY(char, 0x10125850)              // &g_eqmain_cycle_text_guess: the retry's string
#define g_eqmain_port_guess        EQ_GLOBAL(int, 0x101258A0)              // g_eqmain_port_guess: the retry's int
#define g_eqmain_ceb8_guess             EQ_GLOBAL(int, 0x1004CEB8)              // g_eqmain_ceb8_guess: nonzero: PumpMessagesAndEditText_guess
#define g_eqmain_lock_guess             EQ_ARRAY(void, 0x100F4930)              // &g_eqmain_lock_guess: the this of CycleNameAndValue_guess
#define g_eqmain_version_guess          EQ_ARRAY(char, 0x100F4BD0)              // g_eqmain_version_guess: the reply is copied here
#define g_eqmain_str_10048028_guess     EQ_ARRAY(char, 0x10048028)              // DAT_10048028: the string it is compared with
#define g_eqmain_connect_str_guess      EQ_ARRAY(char, 0x10048594)              // "..."
#define g_sleep_guess                   EQ_GLOBAL(SleepProc_guess, 0x10040140)  // PTR_Sleep_10040140: Sleep's IAT slot

// This image's functions the function below calls (FUN_10027940 takes the string as a stack argument it never pops).
#define eqmain_fn_100211b0_guess        EQ_CDECL(char *, 0x100211B0, (char *))
#define eqmain_fn_1002ac80_guess        EQ_CDECL(int, 0x1002AC80, (int, int *, char *, int, unsigned int))
#define connection_send_guess           EQ_CDECL(void, 0x1002C010, (int, void *, int, unsigned int, unsigned int))
#define eqmain_fn_10027940_guess        EQ_CDECL(void, 0x10027940, (const char *))
#define eqmain_fn_10035b24_guess        EQ_CDECL(time_t, 0x10035B24, (time_t *))                // _time
#define eqmain_fn_10021420_guess        EQ_CDECL(int, 0x10021420, (int, unsigned int))          // rdp_receive
#define eqmain_fn_10003bd0_guess        EQ_STDCALL(void, 0x10003BD0, (void))
#define DisconnectAndClearConnection_guess                    EQ_FUNC(void, __fastcall, 0x10021440, (void))          // ECX is not read
#define eqmain_fn_10024e90_guess(object) EQ_METHOD(1, int, 0x10024E90, (void *, int), (object))
#define eqmain_fn_1002de90_guess        EQ_CDECL(int, 0x1002DE90, (int))
#define eqmain_fn_1002dd30_guess        EQ_CDECL(int, 0x1002DD30, (int))
#define msg_arrival_get_data_guess      EQ_CDECL(short *, 0x1002DE70, (int))
#define eqmain_fn_100250a0_guess        EQ_CDECL(void, 0x100250A0, (short *))
#define eqmain_fn_10025150_guess        EQ_STDCALL(unsigned char *, 0x10025150, (void))
#define eqmain_fn_10021110_guess        EQ_CDECL(char *, 0x10021110, (unsigned char *))
#define eqmain_fn_10028a90_guess        EQ_CDECL(unsigned int, 0x10028A90, (int))              // returns EAX (kept)

typedef void (__stdcall *SleepProc_guess)(unsigned long);

// FUNCTION: EQMAIN 0x100214A0
// Connects, sends CONNECT (0x59), waits up to 30 s for the reply and checks its string against an expected one
// __cdecl, three stack args: the server string, an int and the dword the outcome goes to. Returns uint, not bool: the
// original returns EAX with AL set to 0 or 1, so the upper bits are what the last callee left in EAX (callers read AL).
// Connects (createNetworkConnectionByName_guess), then sends CONNECT (0x59) and waits up to 30 seconds for a reply. A reply whose string
// equals DAT_10048028 gives *resultCode = 0 and returns 1; a different string gives 2, and every other failure gives 1.
// A timeout sleeps a second and connects again with the retry globals.
unsigned int __cdecl ConnectAndCheckReply_guess(char *hostName, int arg, unsigned int *resultCode)
{
    unsigned int tmp;
    char *ptr;
    int value;
    int iVar5;
    int msg;
    short *payload;
    unsigned char *encoded;
    unsigned int callResult;
    time_t startTime;
    time_t now;
    int deadline;
    SleepProc_guess pfnSleep;

    value = arg;
    tmp = 1;
    ptr = eqmain_fn_100211b0_guess(hostName);
    tmp = eqmain_fn_1002ac80_guess(g_eqmain_rdp_guess, g_eqmain_connection_ref_guess, ptr, value, tmp);
    if (tmp == 0)
    {
        pfnSleep = g_sleep_guess;
        do
        {
            arg = 0x59;
            connection_send_guess(g_eqmain_connection_guess, &arg, 2, 1, 1);
            eqmain_fn_10027940_guess(g_eqmain_connect_str_guess);
            startTime = eqmain_fn_10035b24_guess(0);
            deadline = (int)startTime + 0x1e;
            now = eqmain_fn_10035b24_guess(0);
            value = (int)now;
            if (value < deadline)
            {
                do
                {
                    msg = eqmain_fn_10021420_guess(g_eqmain_rdp_guess, 10);
                    if (msg != 0)
                    {
                        break;
                    }
                    if (g_eqmain_ceb8_guess != 0)
                    {
                        eqmain_fn_10003bd0_guess();
                    }
                    now = eqmain_fn_10035b24_guess(0);
                    value = (int)now;
                } while (value < deadline);
                if (msg != 0)
                {
                    goto LAB_handler;
                }
            }
            DisconnectAndClearConnection_guess();
            tmp = eqmain_fn_10024e90_guess(g_eqmain_lock_guess)( 0);
            if (tmp != 0)
            {
                *resultCode = 1;
                return tmp & 0xffffff00;
            }
            goto LAB_sleep;
LAB_handler:
            iVar5 = eqmain_fn_1002dd30_guess(eqmain_fn_1002de90_guess(msg));
            if (iVar5 != 0)
            {
                goto LAB_has_data;
            }
            DisconnectAndClearConnection_guess();
            tmp = eqmain_fn_10024e90_guess(g_eqmain_lock_guess)( 0);
            if (tmp != 0)
            {
                *resultCode = 1;
                tmp = eqmain_fn_10028a90_guess(msg);
                return tmp & 0xffffff00;
            }
LAB_sleep:
            pfnSleep(1000);
            tmp = 1;
            value = g_eqmain_port_guess;
            ptr = eqmain_fn_100211b0_guess(g_eqmain_cycle_text_guess);
            tmp = eqmain_fn_1002ac80_guess(g_eqmain_rdp_guess, g_eqmain_connection_ref_guess, ptr, value, tmp);
        } while (tmp == 0);
    }
    *resultCode = 1;
    return tmp & 0xffffff00;
LAB_has_data:
    payload = msg_arrival_get_data_guess(msg);
    if (payload == 0 || *payload != 0x59)
    {
        DisconnectAndClearConnection_guess();
        *resultCode = 1;
        tmp = eqmain_fn_10028a90_guess(msg);
        return tmp & 0xffffff00;
    }
    eqmain_fn_100250a0_guess(payload);
    encoded = eqmain_fn_10025150_guess();
    ptr = eqmain_fn_10021110_guess(encoded);
    strcpy(g_eqmain_version_guess, ptr);
    iVar5 = strcmp(g_eqmain_version_guess, g_eqmain_str_10048028_guess);
    if (iVar5 != 0)
    {
        DisconnectAndClearConnection_guess();
        *resultCode = 2;
        tmp = eqmain_fn_10028a90_guess(msg);
        return tmp & 0xffffff00;
    }
    *resultCode = 0;
    callResult = eqmain_fn_10028a90_guess(msg);
    return (callResult & 0xffffff00) | 1;
}
