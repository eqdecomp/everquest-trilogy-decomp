// unit_1000E5B0.cpp - eqmain.dll, the address run 0x1000E5B0..0x1000E6D0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// eqmain.dll's own table (this image's address, not eqgame.exe's): 256 dwords, indexed by the top byte of the
// running value xor the input byte.
#define g_crc_msb_table_guess   EQ_ARRAY(unsigned int, 0x10047A54)

// eqmain.dll's own global (this image's address): a dword that reset_trace_message_buffer_guess zeroes.
#define g_trace_message_buffer_guess EQ_GLOBAL(unsigned int, 0x1004CF0C)

// eqmain.dll's own exit-time object pointer (this image's address: the same dword as g_trace_message_buffer_guess, read
// here as a pointer). A refcounted object whose count at +4 eqmain_exit_release_guess drops.
#define g_packfs_shared_guess EQ_GLOBAL(int *, 0x1004CF0C)

// eqmain.dll's own flag container (this image's addresses; the object is at 0x1004CEF8, its head pointer at +4 and its
// flag at +0xC): the head, the flag, the shared nil node and the nil node's refcount.
#define g_flagcontainer_head_eqmain_guess    EQ_GLOBAL(int *, 0x1004CEFC)
#define g_flagcontainer_flag_eqmain_guess    EQ_GLOBAL(int, 0x1004CF04)
#define g_flagcontainer_nil_eqmain_guess     EQ_GLOBAL(int *, 0x1004CF14)
#define g_flagcontainer_nilrefs_eqmain_guess EQ_GLOBAL(int, 0x1004CF10)

// FUNCTION: EQMAIN 0x1000e5b0
// Calculates a fresh data checksum; its specific use in the game is unclear.
// __stdcall, 8 bytes of arguments (the callee pops them). Starts a table-driven MSB-first CRC over `len` bytes of `buf`
// from the running value 0: calls crc_msb_update_guess(0, buf, len) at 0x1000E5D0 and returns its result in EAX.
unsigned int __stdcall crc_msb_from_zero_guess(unsigned char *buf, int len)
{
    return EQ_STDCALL(unsigned int, 0x1000E5D0, (unsigned int, unsigned char *, int))(0, buf, len);
}

// FUNCTION: EQMAIN 0x1000e5d0
// Continues an existing data checksum; its specific use in the game is unclear.
// @early-stop: register allocation and CRC-index instruction order still differ from the original.
// __stdcall, 12 bytes of arguments (the callee pops them). Continues a table-driven MSB-first CRC over `len` bytes
// of `buf` from the running value `crc` (no pre- or post-inversion) and returns it. `len` 0 returns `crc` unchanged
// and never reads `buf`. The table's polynomial is not confirmed.
unsigned int __stdcall crc_msb_update_guess(unsigned int crc, unsigned char *buf, unsigned int len)
{
    unsigned int inputByte;

    if (len > 0)
    {
        do
        {
            inputByte = *buf;
            buf = buf + 1;
            crc = (crc << 8) ^ g_crc_msb_table_guess[(crc >> 0x18) ^ inputByte];
        } while (--len);
    }
    return crc;
}

// FUNCTION: EQMAIN 0x1000e640
// __stdcall, no arguments (callee pops 0 bytes). Zeroes the dword global at 0x1004CF0C and returns. No return value.
void __stdcall reset_trace_message_buffer_guess(void)
{
    g_trace_message_buffer_guess = 0;
}

// FUNCTION: EQMAIN 0x1000e650
// Schedules release of an internal object when the client exits.
// __stdcall, no arguments. Registers the exit-time callback at 0x1000E660 with the CRT's _atexit (0x100348C5)
// and returns. _atexit's result is discarded.
void __stdcall eqmain_register_exit_guess(void)
{
    EQ_CDECL(int, 0x100348C5, (void (*)(void)))((void (*)(void))0x1000e660);
}

// FUNCTION: EQMAIN 0x1000e660
// Releases an internal object's reference when the client exits.
// @early-stop: VC6 folds the count store to [base+4] and removes the redundant saved-pointer null check.
// __stdcall, no arguments (callee pops 0 bytes). The exit-time callback that eqmain_register_exit_guess (0x1000E650)
// hands to _atexit. If the object pointer in the global at 0x1004CF0C is not null, decrements the dword at its +4;
// when that reaches zero and the pointer is still not null, calls the object's vtable slot 0 (__thiscall, this in ECX)
// with 1. The pointer is not cleared. Returns nothing.
void __stdcall eqmain_exit_release_guess(void)
{
    struct ExitObject_guess
    {
        virtual void destroy_guess(int flags) = 0;
    };
    int *objectPtr;
    int referenceCount;

    if (g_packfs_shared_guess != 0)
    {
        objectPtr = g_packfs_shared_guess;
        referenceCount = g_packfs_shared_guess[1];
        g_packfs_shared_guess[1] = referenceCount - 1;
        if (referenceCount - 1 == 0 && objectPtr != 0)
            ((ExitObject_guess *)objectPtr)->destroy_guess(1);
    }
}

// FUNCTION: EQMAIN 0x1000e6a0
// Sets up an internal container; the values it will hold are unclear.
// __stdcall, no arguments (callee pops 0 bytes). Calls FlagContainer_initializeFlagContainer_guess
// (0x1000F300, __thiscall: this = 0x1004CEF8 in ECX; the callee pops its two pointer args, &byteValue1 (ESP+3)
// first and &byteValue2 (ESP+2) second, so the original has no ADD ESP). The 4-byte frame is the one PUSH ECX
// reserves; POP ECX releases it.
void __stdcall GetFlagValues_guess(void)
{
    unsigned char byteValue2;
    unsigned char byteValue1;

    EQ_METHOD(2, void, 0x1000F300, (void *, unsigned char *, unsigned char *), (void *)0x1004cef8)(&byteValue1, &byteValue2);
}

// FUNCTION: EQMAIN 0x1000e6c0
// Schedules release of the global container when the client exits.
// __stdcall, no arguments (callee pops 0 bytes). Registers the exit routine at 0x1000E6D0 with the CRT's `_atexit`
// (cdecl, 0x100348C5) and drops the pushed argument with `pop ecx`. The return value is ignored.
void __stdcall register_exit_guess(void)
{
    EQ_CDECL(int, 0x100348C5, (void (*)(void)))((void (*)(void))0x1000e6d0);
}

// FUNCTION: EQMAIN 0x1000e6d0
// Releases the global container and its entries during client shutdown.
// @early-stop: the atexit thunk and implicit-this body shape, native thiscall sites, and register allocation still differ.
// __stdcall, no arguments (callee pops 0 bytes). The exit routine register_exit_guess hands to _atexit: its thunk
// (`MOV ECX,0x1004cef8`, JMP) makes the flag container at 0x1004CEF8 `this` and falls into this body. Flag clear: removes
// the nodes in order from the leftmost one. Flag set: walks the root's left chain, erasing each node's right subtree,
// then its value and the node. The head is reset and freed; the shared nil node goes too when the last user is gone.
// The return value is ignored.
void __stdcall destroy_global_flagcontainer_guess(void)
{
    struct FlagContainerLayout_guess
    {
        int unknown_0;
        int *head;
        int unknown_8;
        int flag;
    };
    FlagContainerLayout_guess *container = (FlagContainerLayout_guess *)0x1004cef8;
    int *tmp;
    int *node = container->head;
    int *iter = (int *)*node;
    int outValue;
    int result;
    enum { unused_edx_value };

    if (container->flag != 0)
    {
        node = (int *)node[1];
        while (node != g_flagcontainer_nil_eqmain_guess)
        {
            EQ_METHOD(1, void, 0x1000FCA0, (void *, int), container)(node[2]);
            tmp = (int *)*node;
            EQ_METHOD(1, void, 0x10010C70, (void *, int), (void *)(node + 3))(0);
            EQ_CDECL(void, 0x10033800, (void *))(node);
            node = tmp;
        }
        container->head[1] = (int)g_flagcontainer_nil_eqmain_guess;
        container->flag = 0;
        *container->head = (int)container->head;
        container->head[2] = (int)container->head;
        EQ_THISCALL(void, 0x100105B0, (void *, int, int *))(container, unused_edx_value, &outValue);
    }
    else
    {
        tmp = iter;
        while (iter != node)
        {
            EQ_METHOD(0, void, 0x10010310, (void *), &iter)();
            EQ_METHOD(2, void, 0x1000F6B0, (void *, int *, int *), (void *)0x1004cef8)(&result, tmp);
            tmp = iter;
        }
    }
    EQ_CDECL(void, 0x10033800, (void *))(container->head);
    container->head = 0;
    container->flag = 0;
    tmp = 0;
    EQ_METHOD(0, void, 0x1003E980, (void *), &iter)();
    if (--g_flagcontainer_nilrefs_eqmain_guess == 0)
    {
        tmp = g_flagcontainer_nil_eqmain_guess;
        g_flagcontainer_nil_eqmain_guess = 0;
    }
    EQ_METHOD(0, void, 0x1003EA1C, (void *), &iter)();
    if (tmp != 0)
    {
        EQ_CDECL(void, 0x10033800, (void *))(tmp);
    }
}
