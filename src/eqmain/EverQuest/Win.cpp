// Win.cpp - eqmain.dll, the EverQuest subsystem's Win functions (this image's VAs, not trilogy's).

#include "trilogy/eqaddr.h"

// eqmain.dll's globals and functions used by CleanupSetup_guess (this image's VAs).
#define g_login_ready_guess       EQ_GLOBAL(char, 0x101258B0)                                        // g_login_ready_guess: 1 once the setup has run; cleared here
#define g_eqmain_rdp_guess              EQ_GLOBAL(int, 0x10125638)                                         // g_eqmain_rdp_guess: the rdp handle rdp_destroy_guess frees
#define g_rdp_flag_guess         EQ_GLOBAL(char, 0x10125844)                                        // g_rdp_flag_guess: cleared before the memory pool is cleaned up
#define eqmain_dbglog_guess(object) EQ_METHOD(0, void, 0x10021440, (int), (object))                          // DisconnectAndClearConnection_guess (EverQuest.cpp): ECX = the argument, EDX dummy
#define eqmain_rdp_destroy_guess        EQ_CDECL(void, 0x1002A950, (int, int))                             // rdp_destroy_guess (rdp.c): (rdp handle, 1)
#define eqmain_cleanup_pool_guess       EQ_STDCALL(void, 0x10028850, (void))                               // cleanupMemoryPool_guess (fast_malloc.c)

// FUNCTION: EQMAIN 0x100216F0
// Tears down front-end setup state; its specific in-game purpose is unclear.
// __fastcall, one register argument (obj in ECX; undefined4 in Ghidra), RET pops 0 bytes, void: no caller reads EAX.
// If g_login_ready_guess is nonzero it calls DisconnectAndClearConnection_guess (0x10021440) with obj, destroys the rdp handle g_eqmain_rdp_guess
// (rdp_destroy_guess with 1), clears g_rdp_flag_guess, cleans up the memory pool (cleanupMemoryPool_guess) and clears
// g_login_ready_guess. Otherwise it returns at once. The meaning of the calls is unknown (the name MakeFont is low confidence).
void __fastcall CleanupSetup_guess(int obj)
{
    if (g_login_ready_guess != 0)
    {
        eqmain_dbglog_guess(obj)();
        eqmain_rdp_destroy_guess(g_eqmain_rdp_guess, 1);
        g_rdp_flag_guess = 0;
        eqmain_cleanup_pool_guess();
        g_login_ready_guess = 0;
    }
    return;
}
