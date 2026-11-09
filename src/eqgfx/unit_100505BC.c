// CFLAGS: /O2
#include "trilogy/eqaddr.h"

#define EQGFX_FUN_10051CE0 EQ_CDECL(void, 0x10051CE0, (void))
#define EQGFX_FUN_10051D40 EQ_CDECL(void, 0x10051D40, (void))
#define EQGFX_ATEXIT EQ_CDECL(int, 0x1006A7F2, (void (*)(void)))

// FUNCTION: EQGFX 0x10051CD0
// Unclear: runs one initializer and registers its process-exit cleanup handler.
void InitializeAndRegisterCleanupAtExit_guess(void)
{
    EQGFX_FUN_10051CE0();
    EQGFX_ATEXIT((void (*)(void))0x10051D00);
}

// FUNCTION: EQGFX 0x10051d30
// Unclear: runs another initializer and registers its process-exit cleanup handler.
void InitializeAndRegisterCleanupAtExit2_guess(void)
{
    EQGFX_FUN_10051D40();
    __asm {
        jmp short register_handler_after_initializer
    register_handler_after_initializer:
    }
    EQGFX_ATEXIT((void (*)(void))0x10051D70);
}
