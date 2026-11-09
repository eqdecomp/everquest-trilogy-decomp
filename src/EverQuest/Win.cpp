// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// Win.cpp - Windows startup/voice dispatch helpers.
// Source inferred from global Windows startup/voice dispatch cluster; do not state direct map.

#include "trilogy/common.h"
#include "trilogy/display.h"
#include "trilogy/eqaddr.h"
#include "trilogy/win.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/layout/eq_charcreate.h"
#include "trilogy/eq_textio.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* format, ...);
#define LogMessage_guess DbgLog_guess

// FUNCTION: TRILOGY 0x004dd9a0
// Clears the table of graphics entry pointers at 0x006E50A0 and resets the string at 0x006E4EA0 to "~*RPE".
void Init_EQGfx_Dx8_turnoff_guess(void)
{
    strcpy(EQ_ARRAY(char, 0x006E4EA0), EQ_ARRAY(char, 0x005709A8));
    EQ_GLOBAL(u32, 0x006E50A4) = 0;
    EQ_GLOBAL(u32, 0x006E50A8) = 0;
    EQ_GLOBAL(u32, 0x006E50AC) = 0;
    EQ_GLOBAL(u32, 0x006E50B0) = 0;
    EQ_GLOBAL(u32, 0x006E50B4) = 0;
    EQ_GLOBAL(u32, 0x006E50B8) = 0;
    EQ_GLOBAL(u32, 0x006E50BC) = 0;
    EQ_GLOBAL(u32, 0x006E50C0) = 0;
    EQ_GLOBAL(u32, 0x006E50C4) = 0;
    EQ_GLOBAL(u32, 0x006E50C8) = 0;
    EQ_GLOBAL(u32, 0x006E50CC) = 0;
    EQ_GLOBAL(u32, 0x006E50D0) = 0;
    EQ_GLOBAL(u32, 0x006E50D4) = 0;
    EQ_GLOBAL(u32, 0x006E50D8) = 0;
    EQ_GLOBAL(u32, 0x006E50DC) = 0;
    EQ_GLOBAL(u32, 0x006E50E0) = 0;
    EQ_GLOBAL(u32, 0x006E50E4) = 0;
    EQ_GLOBAL(u32, 0x006E50E8) = 0;
    EQ_GLOBAL(u32, 0x006E50EC) = 0;
    EQ_GLOBAL(u32, 0x006E50F0) = 0;
    EQ_GLOBAL(u32, 0x006E50F4) = 0;
    EQ_GLOBAL(u32, 0x006E50F8) = 0;
    EQ_GLOBAL(u32, 0x006E50FC) = 0;
    EQ_GLOBAL(u32, 0x006E5100) = 0;
    EQ_GLOBAL(u32, 0x006E5104) = 0;
    EQ_GLOBAL(u32, 0x006E5108) = 0;
    EQ_GLOBAL(u32, 0x006E510C) = 0;
    EQ_GLOBAL(u32, 0x006E5110) = 0;
    EQ_GLOBAL(u32, 0x006E5114) = 0;
    EQ_GLOBAL(u32, 0x006E5118) = 0;
    EQ_GLOBAL(u32, 0x006E511C) = 0;
    EQ_GLOBAL(u32, 0x006E5120) = 0;
    EQ_GLOBAL(u32, 0x006E5124) = 0;
    EQ_GLOBAL(u32, 0x006E5128) = 0;
    EQ_GLOBAL(u32, 0x006E512C) = 0;
    EQ_GLOBAL(u32, 0x006E5130) = 0;
    EQ_GLOBAL(u32, 0x006E5134) = 0;
    EQ_GLOBAL(u32, 0x006E5138) = 0;
    EQ_GLOBAL(u32, 0x006E513C) = 0;
    EQ_GLOBAL(u32, 0x006E5140) = 0;
    EQ_GLOBAL(u32, 0x006E5144) = 0;
    EQ_GLOBAL(u32, 0x006E5148) = 0;
    EQ_GLOBAL(u32, 0x006E514C) = 0;
    EQ_GLOBAL(u32, 0x006E5150) = 0;
    EQ_GLOBAL(u32, 0x006E5154) = 0;
    EQ_GLOBAL(u32, 0x006E5158) = 0;
    EQ_GLOBAL(u32, 0x006E515C) = 0;
    EQ_GLOBAL(u32, 0x006E5160) = 0;
    EQ_GLOBAL(u32, 0x006E5164) = 0;
    EQ_GLOBAL(u32, 0x006E5168) = 0;
    EQ_GLOBAL(u32, 0x006E516C) = 0;
    EQ_GLOBAL(u32, 0x006E5170) = 0;
    EQ_GLOBAL(u32, 0x006E5174) = 0;
    EQ_GLOBAL(u32, 0x006E5178) = 0;
    EQ_GLOBAL(u32, 0x006E517C) = 0;
    EQ_GLOBAL(u32, 0x006E5180) = 0;
    EQ_GLOBAL(u32, 0x006E5184) = 0;
    EQ_GLOBAL(u32, 0x006E5188) = 0;
    EQ_GLOBAL(u32, 0x006E518C) = 0;
    EQ_GLOBAL(u32, 0x006E5190) = 0;
    EQ_GLOBAL(u32, 0x006E5194) = 0;
    EQ_GLOBAL(u32, 0x006E5198) = 0;
    EQ_GLOBAL(u32, 0x006E519C) = 0;
    EQ_GLOBAL(u32, 0x006E51A0) = 0;
    EQ_GLOBAL(u32, 0x006E51A4) = 0;
    EQ_GLOBAL(u32, 0x006E51A8) = 0;
    EQ_GLOBAL(u32, 0x006E51AC) = 0;
    EQ_GLOBAL(u32, 0x006E51B0) = 0;
    EQ_GLOBAL(u32, 0x006E51D8) = 0;
    EQ_GLOBAL(u32, 0x006E51DC) = 0;
    EQ_GLOBAL(u32, 0x006E51E0) = 0;
    EQ_GLOBAL(u32, 0x006E51E4) = 0;
    EQ_GLOBAL(u32, 0x006E51E8) = 0;
    EQ_GLOBAL(u32, 0x006E51CC) = 0;
    EQ_GLOBAL(u32, 0x006E51D0) = 0;
    EQ_GLOBAL(u32, 0x006E51D4) = 0;
    EQ_GLOBAL(u32, 0x006E51C8) = 0;
    EQ_GLOBAL(u32, 0x006E51BC) = 0;
    EQ_GLOBAL(u32, 0x006E51C0) = 0;
    EQ_GLOBAL(u32, 0x006E51C4) = 0;
    EQ_GLOBAL(u32, 0x006E51B8) = 0;
    EQ_GLOBAL(u32, 0x006E51B4) = 0;
    EQ_GLOBAL(u32, 0x006E51EC) = 0;
    EQ_GLOBAL(u32, 0x006E51F0) = 0;
    EQ_GLOBAL(u32, 0x006E51F4) = 0;
    EQ_GLOBAL(u32, 0x006E51F8) = 0;
    EQ_GLOBAL(u32, 0x006E51FC) = 0;
    EQ_GLOBAL(u32, 0x006E5200) = 0;
    EQ_GLOBAL(u32, 0x006E5204) = 0;
    EQ_GLOBAL(u32, 0x006E5208) = 0;
    EQ_GLOBAL(u32, 0x006E520C) = 0;
    EQ_GLOBAL(u32, 0x006E5210) = 0;
    EQ_GLOBAL(u32, 0x006E5214) = 0;
    EQ_GLOBAL(u32, 0x006E5218) = 0;
    EQ_GLOBAL(u32, 0x006E521C) = 0;
    EQ_GLOBAL(u32, 0x006E5220) = 0;
    EQ_GLOBAL(u32, 0x006E5224) = 0;
    EQ_GLOBAL(u32, 0x006E5228) = 0;
    EQ_GLOBAL(u32, 0x006E522C) = 0;
    EQ_GLOBAL(u32, 0x006E5230) = 0;
    EQ_GLOBAL(u32, 0x006E5234) = 0;
    EQ_GLOBAL(u32, 0x006E5238) = 0;
    EQ_GLOBAL(u32, 0x006E523C) = 0;
    EQ_GLOBAL(u32, 0x006E5240) = 0;
    EQ_GLOBAL(u32, 0x006E5244) = 0;
    EQ_GLOBAL(u32, 0x006E5248) = 0;
    EQ_GLOBAL(u32, 0x006E524C) = 0;
    EQ_GLOBAL(u32, 0x006E5250) = 0;
    EQ_GLOBAL(u32, 0x006E5254) = 0;
    EQ_GLOBAL(u32, 0x006E5258) = 0;
    EQ_GLOBAL(u32, 0x006E525C) = 0;
    EQ_GLOBAL(u32, 0x006E5260) = 0;
    EQ_GLOBAL(u32, 0x006E5264) = 0;
    EQ_GLOBAL(u32, 0x006E5268) = 0;
    EQ_GLOBAL(u32, 0x006E526C) = 0;
    EQ_GLOBAL(u32, 0x006E5270) = 0;
    EQ_GLOBAL(u32, 0x006E5274) = 0;
    EQ_GLOBAL(u32, 0x006E5278) = 0;
    EQ_GLOBAL(u32, 0x006E527C) = 0;
    EQ_GLOBAL(u32, 0x006E5280) = 0;
    EQ_GLOBAL(u32, 0x006E5284) = 0;
    EQ_GLOBAL(u32, 0x006E5288) = 0;
    EQ_GLOBAL(u32, 0x006E528C) = 0;
    EQ_GLOBAL(u32, 0x006E5290) = 0;
    EQ_GLOBAL(u32, 0x006E5294) = 0;
    EQ_GLOBAL(u32, 0x006E5298) = 0;
    EQ_GLOBAL(u32, 0x006E529C) = 0;
    EQ_GLOBAL(u32, 0x006E52A0) = 0;
    EQ_GLOBAL(u32, 0x006E52A4) = 0;
    EQ_GLOBAL(u32, 0x006E52A8) = 0;
    EQ_GLOBAL(u32, 0x006E52AC) = 0;
    EQ_GLOBAL(u32, 0x006E52B0) = 0;
    EQ_GLOBAL(u32, 0x006E52B4) = 0;
    EQ_GLOBAL(u32, 0x006E52B8) = 0;
    EQ_GLOBAL(u32, 0x006E52BC) = 0;
    EQ_GLOBAL(u32, 0x006E52C0) = 0;
    EQ_GLOBAL(u32, 0x006E52C4) = 0;
    EQ_GLOBAL(u32, 0x006E52C8) = 0;
    EQ_GLOBAL(u32, 0x006E52CC) = 0;
    EQ_GLOBAL(u32, 0x006E52D0) = 0;
    EQ_GLOBAL(u32, 0x006E52D4) = 0;
    EQ_GLOBAL(u32, 0x006E52D8) = 0;
    EQ_GLOBAL(u32, 0x006E52DC) = 0;
    EQ_GLOBAL(u32, 0x006E52E0) = 0;
    EQ_GLOBAL(u32, 0x006E52E4) = 0;
    EQ_GLOBAL(u32, 0x006E52E8) = 0;
    EQ_GLOBAL(u32, 0x006E52EC) = 0;
    EQ_GLOBAL(u32, 0x006E52F0) = 0;
    EQ_GLOBAL(u32, 0x006E52F4) = 0;
    EQ_GLOBAL(u32, 0x006E52F8) = 0;
    EQ_GLOBAL(u32, 0x006E52FC) = 0;
    EQ_GLOBAL(u32, 0x006E5300) = 0;
    EQ_GLOBAL(u32, 0x006E5304) = 0;
    EQ_GLOBAL(u32, 0x006E5308) = 0;
    EQ_GLOBAL(u32, 0x006E530C) = 0;
    EQ_GLOBAL(u32, 0x006E5310) = 0;
    EQ_GLOBAL(u32, 0x006E5314) = 0;
    EQ_GLOBAL(u32, 0x006E5318) = 0;
    EQ_GLOBAL(u32, 0x006E531C) = 0;
    EQ_GLOBAL(u32, 0x006E5320) = 0;
    EQ_GLOBAL(u32, 0x006E5324) = 0;
    EQ_GLOBAL(u32, 0x006E5328) = 0;
    EQ_GLOBAL(u32, 0x006E532C) = 0;
    EQ_GLOBAL(u32, 0x006E5330) = 0;
    EQ_GLOBAL(u32, 0x006E5334) = 0;
    EQ_GLOBAL(u32, 0x006E5338) = 0;
    EQ_GLOBAL(u32, 0x006E533C) = 0;
    EQ_GLOBAL(u32, 0x006E5340) = 0;
    EQ_GLOBAL(u32, 0x006E5344) = 0;
    EQ_GLOBAL(u32, 0x006E5348) = 0;
    EQ_GLOBAL(u32, 0x006E534C) = 0;
    EQ_GLOBAL(u32, 0x006E5350) = 0;
    EQ_GLOBAL(u32, 0x006E5354) = 0;
    EQ_GLOBAL(u32, 0x006E5358) = 0;
    EQ_GLOBAL(u32, 0x006E535C) = 0;
    EQ_GLOBAL(u32, 0x006E5360) = 0;
    EQ_GLOBAL(u32, 0x006E5364) = 0;
    EQ_GLOBAL(u32, 0x006E5368) = 0;
    EQ_GLOBAL(u32, 0x006E536C) = 0;
    EQ_GLOBAL(u32, 0x006E5370) = 0;
    EQ_GLOBAL(u32, 0x006E5374) = 0;
    EQ_GLOBAL(u32, 0x006E5378) = 0;
    EQ_GLOBAL(u32, 0x006E537C) = 0;
    EQ_GLOBAL(u32, 0x006E5380) = 0;
    EQ_GLOBAL(u32, 0x006E5384) = 0;
    EQ_GLOBAL(u32, 0x006E5388) = 0;
    EQ_GLOBAL(u32, 0x006E538C) = 0;
    EQ_GLOBAL(u32, 0x006E5390) = 0;
    EQ_GLOBAL(u32, 0x006E5394) = 0;
    EQ_GLOBAL(u32, 0x006E5398) = 0;
    EQ_GLOBAL(u32, 0x006E539C) = 0;
    EQ_GLOBAL(u32, 0x006E53A0) = 0;
    EQ_GLOBAL(u32, 0x006E53A4) = 0;
    EQ_GLOBAL(u32, 0x006E53A8) = 0;
    EQ_GLOBAL(u32, 0x006E53AC) = 0;
    EQ_GLOBAL(u32, 0x006E53B0) = 0;
    EQ_GLOBAL(u32, 0x006E53B4) = 0;
    EQ_GLOBAL(u32, 0x006E53B8) = 0;
    EQ_GLOBAL(u32, 0x006E53BC) = 0;
    EQ_GLOBAL(u32, 0x006E53C0) = 0;
    EQ_GLOBAL(u32, 0x006E53C4) = 0;
    EQ_GLOBAL(u32, 0x006E53C8) = 0;
    EQ_GLOBAL(u32, 0x006E53CC) = 0;
    EQ_GLOBAL(u32, 0x006E53D0) = 0;
    EQ_GLOBAL(u32, 0x006E53D4) = 0;
    EQ_GLOBAL(u32, 0x006E53D8) = 0;
    EQ_GLOBAL(u32, 0x006E53DC) = 0;
    EQ_GLOBAL(u32, 0x006E53E0) = 0;
    EQ_GLOBAL(u32, 0x006E53E4) = 0;
    EQ_GLOBAL(u32, 0x006E53E8) = 0;
    EQ_GLOBAL(u32, 0x006E53EC) = 0;
    EQ_GLOBAL(u32, 0x006E53F0) = 0;
    EQ_GLOBAL(u32, 0x006E53F4) = 0;
    EQ_GLOBAL(u32, 0x006E53F8) = 0;
    EQ_GLOBAL(u32, 0x006E53FC) = 0;
    EQ_GLOBAL(u32, 0x006E5400) = 0;
    EQ_GLOBAL(u32, 0x006E5404) = 0;
    EQ_GLOBAL(u32, 0x006E5408) = 0;
    EQ_GLOBAL(u32, 0x006E540C) = 0;
    EQ_GLOBAL(u32, 0x006E5410) = 0;
    EQ_GLOBAL(u32, 0x006E5414) = 0;
    EQ_GLOBAL(u32, 0x006E5418) = 0;
    EQ_GLOBAL(u32, 0x006E541C) = 0;
    EQ_GLOBAL(u32, 0x006E5420) = 0;
    EQ_GLOBAL(u32, 0x006E5424) = 0;
    EQ_GLOBAL(u32, 0x006E5428) = 0;
    EQ_GLOBAL(u32, 0x006E542C) = 0;
    EQ_GLOBAL(u32, 0x006E5430) = 0;
    EQ_GLOBAL(u32, 0x006E5434) = 0;
    EQ_GLOBAL(u32, 0x006E5438) = 0;
    EQ_GLOBAL(u32, 0x006E543C) = 0;
    EQ_GLOBAL(u32, 0x006E5440) = 0;
    EQ_GLOBAL(u32, 0x006E5444) = 0;
    EQ_GLOBAL(u32, 0x006E5448) = 0;
    EQ_GLOBAL(u32, 0x006E544C) = 0;
    EQ_GLOBAL(u32, 0x006E5450) = 0;
    EQ_GLOBAL(u32, 0x006E5454) = 0;
    EQ_GLOBAL(u32, 0x006E5458) = 0;
    EQ_GLOBAL(u32, 0x006E545C) = 0;
    EQ_GLOBAL(u32, 0x006E5460) = 0;
    EQ_GLOBAL(u32, 0x006E5464) = 0;
    EQ_GLOBAL(u32, 0x006E5468) = 0;
    EQ_GLOBAL(u32, 0x006E546C) = 0;
    EQ_GLOBAL(u32, 0x006E5470) = 0;
    EQ_GLOBAL(u32, 0x006E5474) = 0;
    EQ_GLOBAL(u32, 0x006E5478) = 0;
    EQ_GLOBAL(u32, 0x006E50A0) = 0;
    return;
}

// FUNCTION: TRILOGY 0x004dfe0a
// Runs a shutdown helper and clears a stored module handle.
u32 Init_EQGfx_Dx8_freememory_guess(void)
{
    typedef int (__stdcall *FreeLibraryFn_guess)(u32);

    EQ_CDECL(void, 0x004DD9A0, (void))();
    if (EQ_GLOBAL(u32, 0x006E547C) != 0)
    {
        EQ_GLOBAL(FreeLibraryFn_guess, 0x0053403C)(EQ_GLOBAL(u32, 0x006E547C));
        EQ_GLOBAL(u32, 0x006E547C) = 0;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e7cd3
// Switches a client subsystem on or off; its exact purpose is unclear.
int toggleAudioDeviceState_guess(u32 value)
{
    if (EQ_GLOBAL(u32, 0x006ECE40) != 0)
    {
        EQ_CDECL(void, 0x004E7C5E, (void))();
    }
    else
    {
        EQ_CDECL(void, 0x004E7BE7, (u32))(value);
    }
    return EQ_GLOBAL(u32, 0x006ECE40);
}

// FUNCTION: TRILOGY 0x004e7cf3
// Returns the dword at 0x006ECE3C (+0xC in the object at 0x006ECE30); the main loop tests it for nonzero.
u32 StreamManager_GetMusicPlaylistCount_guess(void)
{
    return g_music_playlist_count_guess;
}

// FUNCTION: TRILOGY 0x004e7cff
// Forwards a four-value client request; its in-game purpose is unclear.
int processVoiceRecognitionCommand_guess(u32 a1, u32 a2, u32 a3, u32 a4)
{
    void *o1 = g_voice_dispatch_guess;
    int lock = ((WinLockFn_guess)(*(void ***)o1)[3])(o1);

    if (lock == 0)
    {
        void *o2 = g_voice_dispatch_guess;

        ((WinOp4Fn_guess)(*(void ***)o2)[6])(o2, a1, a2, a3, a4);
        {
            void *o3 = g_voice_dispatch_guess;

            ((WinUnlockFn_guess)(*(void ***)o3)[4])(o3);
        }
    }
    return lock;
}

// FUNCTION: TRILOGY 0x004e7d3b
// Forwards a three-value client request; its in-game purpose is unclear.
int clearClipboardData_guess(u32 a1, u32 a2, u32 a3)
{
    void *o1 = g_voice_dispatch_guess;
    int lock = ((WinLockFn_guess)(*(void ***)o1)[3])(o1);

    if (lock == 0)
    {
        void *o2 = g_voice_dispatch_guess;

        ((WinOp3Fn_guess)(*(void ***)o2)[8])(o2, a1, a2, a3);
        {
            void *o3 = g_voice_dispatch_guess;

            ((WinUnlockFn_guess)(*(void ***)o3)[4])(o3);
        }
    }
    return lock;
}

// FUNCTION: TRILOGY 0x004e7fce
// __thiscall shape: this in ECX, no stack args (__fastcall with one pointer param).
void __fastcall setVirtualTablePointer_guess(u32 *obj)
{
    *obj = 0x005357bc;
}

// FUNCTION: TRILOGY 0x004e7fd5
// Returns the client object for either of two supported interface identifiers.
// __stdcall QueryInterface shape: this, riid and ppv on the stack, callee pops 12 bytes. Zeroes *ppv. When riid
// matches the GUID at 0x00536f40 or 0x00534d70 (16 bytes each), stores this in *ppv, calls vtable slot 1
// (AddRef, stdcall) on it and returns 0. Otherwise returns 0x80004002 (E_NOINTERFACE).
u32 __stdcall QueryInterfaceByGuid_guess(void *obj, void *riid, void **ppv)
{
    int compareResult;

    *ppv = 0;
    compareResult = EQ_CDECL(int, 0x00524310, (void *, void *, u32))(riid, (void *)0x00536F40, 0x10);
    if (compareResult != 0 && (compareResult = EQ_CDECL(int, 0x00524310, (void *, void *, u32))(riid, (void *)0x00534D70, 0x10), compareResult != 0))
    {
        return 0x80004002;
    }
    *ppv = obj;
    (*(void (__stdcall **)(void *))(*(u32 *)obj + 4))(obj);
    return 0;
}

// FUNCTION: TRILOGY 0x004e8031
// Destroys the object after its last reference is released.
// __stdcall, callee pops 4 bytes (obj). Decrements the dword at obj+4 and re-reads it. When the new count is 0 and obj is
// non-null, runs setVirtualTablePointer_guess (0x004e7fce, ECX = obj) and frees obj with the cdecl operator delete
// (0x005218bb). Returns the new count.
int __stdcall decrementReferenceCount_guess(int object)
{
    int referenceCount;

    *(int *)(object + 4) = *(int *)(object + 4) + -1;
    referenceCount = *(int *)(object + 4);
    if (referenceCount == 0 && object != 0)
    {
        EQ_METHOD(0, void, 0x004E7FCE, (void *), (void *)object)();
        operator delete((void *)object);
    }
    return referenceCount;
}

// FUNCTION: TRILOGY 0x004e805a
// Prepares a client object for interface use.
// Clears the second dword of the object and sets its vtable (0x00535808). Returns the object (EAX = ECX, never rewritten).
u32* __fastcall initializeObjectWithVTable_guess(u32* obj)
{
    obj[1] &= 0;
    *obj = 0x00535808;
    return obj;
}

// FUNCTION: TRILOGY 0x004e806e
// Recognizes either of two interface identifiers and returns the supplied value.
// __stdcall, callee pops 12 bytes (value, guid, out). Zeroes *out. When the 16-byte guid matches the GUID at 0x00536f40
// or the one at 0x00534c70, stores value in *out and returns 0. Otherwise returns 0x80004002 (E_NOINTERFACE).
// Same shape as QueryInterfaceByGuid_guess (0x004e7fd5) without the AddRef call and with the second GUID at 0x00534c70.
u32 __stdcall validateGuidAndSetFlag_guess(u32 value, void *guid, u32 *out)
{
    int compareResult;

    *out = 0;
    compareResult = EQ_CDECL(int, 0x00524310, (void *, void *, u32))(guid, (void *)0x00536F40, 0x10);
    if (compareResult != 0 && (compareResult = EQ_CDECL(int, 0x00524310, (void *, void *, u32))(guid, (void *)0x00534C70, 0x10), compareResult != 0))
    {
        return 0x80004002;
    }
    *out = value;
    return 0;
}

// FUNCTION: TRILOGY 0x004e87a8
// Sets fixed client startup data; its purpose is unclear.
void initializeClientValidationString_guess(void)
{
    EQ_GLOBAL(u8, 0x006EE7B9) = 0x50;
    EQ_GLOBAL(u8, 0x006EE7E1) = 0x2E;
    EQ_GLOBAL(u8, 0x006EF7E4) = 0x34;
    EQ_GLOBAL(u8, 0x006EF7E5) = 0x40;
    EQ_GLOBAL(u8, 0x006EF7E6) = 0x36;
    EQ_GLOBAL(u8, 0x006EF7E7) = 0x30;
    EQ_GLOBAL(u8, 0x006EF7E8) = 0x3C;
    EQ_GLOBAL(u8, 0x006EF7E9) = 0x34;
    EQ_GLOBAL(u8, 0x006EF7EA) = 0x50;
    EQ_GLOBAL(u8, 0x006EF7EB) = 0x34;
    EQ_GLOBAL(u8, 0x006EF7EC) = 0x47;
    EQ_GLOBAL(u8, 0x006EF7ED) = 0x34;
}

// FUNCTION: TRILOGY 0x004e8d6a
// Loads the client's keyboard bindings, or writes defaults when the file is missing.
// Keyboard.txt holds the two 0x100-byte key tables plus an optional third 0x100-byte block;
void LoadConfigurationData_guess(void)
{
    void* file;

    file = EQ_CDECL(void*, 0x005223BC, (const char*, const char*))((const char*)0x0057d8a0, (const char*)0x0055c1ac);
    if (file == 0)
    {
        file = EQ_CDECL(void*, 0x005223BC, (const char*, const char*))((const char*)0x0057d8a0, (const char*)0x0056fd18);
        if (file != 0)
        {
            EQ_CDECL(u32, 0x0052359E, (const void*, u32, u32, void*))((void*)0x0057d0d0, 0x100, 1, file);
            EQ_CDECL(u32, 0x0052359E, (const void*, u32, u32, void*))((void*)0x0057d1d0, 0x100, 1, file);
            EQ_CDECL(int, 0x00521FBB, (void*))(file);
        }
    }
    else
    {
        EQ_CDECL(u32, 0x00522038, (void*, u32, u32, void*))((void*)0x0057d0d0, 0x100, 1, file);
        EQ_CDECL(u32, 0x00522038, (void*, u32, u32, void*))((void*)0x0057d1d0, 0x100, 1, file);
        if (EQ_CDECL(int, 0x005245C4, (void*))(file) == 0)
            EQ_CDECL(u32, 0x00522038, (void*, u32, u32, void*))((void*)0x006efcc4, 0x100, 1, file);
        EQ_CDECL(int, 0x00521FBB, (void*))(file);
    }
}

// CRT _exit by name: VC6 treats it as non-returning (no stack clean-up after the call), like the original.
extern "C" __declspec(noreturn) void __cdecl _exit(int);

// FUNCTION: TRILOGY 0x004e8e0c
// Launch setup: applies *.dat updates, checks exe name and options, starts WinSock, 3D devices and main window.
// @early-stop: similarity 0.6444; the frame is 4 bytes larger and the loop/register layout still differs.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
// __cdecl(lpCmdLine, hInstance), WinMain's start-up half. Patches itself from the *.dat files, checks the client
// file name against the validation string, splits the command line into 0x100-byte words at 0x006ECE64, starts
// WinSock and the 3D device, creates the main window and copies the 0x15C-byte device records' names.
// Every failure path calls _exit(0); a bad file name writes ILLEGAL.TXT first and the function ends there.
// deviceNameDst is reused as the ILLEGAL.TXT FILE* and as the destination pointer of the last copy loop.
#define PI_ARGC EQ_GLOBAL(int, 0x006EFE18)
#define PI_PATH EQ_ARRAY(char, 0x006EFA48)
#define PI_WORDS EQ_ARRAY(char, 0x006ECE64)
#define PI_VALID EQ_ARRAY(char, 0x006EF7E4)
#define PI_HWND EQ_GLOBAL(void*, 0x006EE754)
#define PI_EXIT _exit
/* fake match: pack startup locals to steer VC6's frame slots and register allocation. */
int PreInitEverQuest(int deviceNameDst, void* instance)
{
    typedef void* (__stdcall *GetCommandLineAFn_guess)(void);
    typedef u32 (__stdcall *GetModuleFileNameAFn_guess)(void*, char*, u32);
    typedef int (__stdcall *MessageBoxAFn_guess)(void*, const char*, const char*, u32);
    typedef void* (__stdcall *GetStockObjectFn_guess)(int);
    typedef u16 (__stdcall *RegisterClassAFn_guess)(void*);
    typedef void* (__stdcall *CreateWindowExAFn_guess)(u32, const char*, const char*, u32, int, int, int, int,
                                                       void*, void*, void*, void*);
    typedef void* (__stdcall *HwndFn_guess)(void*);
    typedef int (__stdcall *SetWindowPosFn_guess)(void*, int, int, int, int, int, u32);
    typedef int (__stdcall *ShowWindowFn_guess)(void*, int);
    typedef int (__stdcall *SetWindowLongAFn_guess)(void*, int, int);
    typedef int (__stdcall *UpdateWindowFn_guess)(void*);
    typedef int (__cdecl *EnumDevicesFn_guess)(void*, void*);
    struct StatBuf_guess
    {
        char head[0x14];
        int size;
        char tail[12];
    };
    struct WndClass_guess
    {
        u32 style;
        u32 lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        void* hInstance;
        void* hIcon;
        void* hCursor;
        void* hbrBackground;
        const char* lpszMenuName;
        const char* lpszClassName;
    };
    struct StartupFrame_guess
    {
        WndClass_guess wc;
        int crcSource;
        size_t fndi;
        int crcTarget;
        char found;
    } locals;
    char errorText[256];
    char optionalText[256];
    StatBuf_guess statBuf;
    char datName[32];
    char exeName[32];
    char winsock[400];
    int i;
    int len;
    char* commandLine;
    int src;
    int deviceSrc;

    locals.found = 0;
    DbgLog_guess((const char*)0x0057dbf8);
    EQ_CDECL(void, 0x00523548, (const char*))((const char*)0x0057d664);
    errorText[0] = 0;
    EQ_CDECL(int, 0x004A3D98, (const char*, const char*, char*))((const char*)0x0057dbe4, (const char*)0x0057dbf0, errorText);
    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))((const char*)0x0056a33c, &statBuf) == 0 && statBuf.size > 100000)
        EQ_CDECL(void, 0x00523548, (const char*))((const char*)0x0056a33c);
    memset((void*)0x006ede68, 0, 0x8b0);
    commandLine = (char*)EQ_GLOBAL(GetCommandLineAFn_guess, 0x005341A4)();
    for (i = 0; i < 0x1e; i++)
    {
        EQ_ARRAY(char, 0x006EE7C4)[i] = (char)(i + 'A');
        EQ_ARRAY(char, 0x006EE79C)[i] = (char)(i + '0');
    }
    PI_ARGC = EQ_CDECL(int, 0x00523F0B, (const char*, const char*, ...))(commandLine, (const char*)0x00562534, PI_PATH);
    if (EQ_GLOBAL(GetModuleFileNameAFn_guess, 0x005341BC)(0, PI_PATH, 0x200) == 0)
        DbgLog_guess((const char*)0x0057dbc4);
    DbgLog_guess((const char*)0x0057dbb4);
    src = 0;
    len = (int)strlen(PI_PATH);
    while (--len > 0)
    {
        if (PI_PATH[len] == '_' || PI_PATH[len] == '\\')
        {
            src = len + 1;
            break;
        }
    }
    strcpy(EQ_ARRAY(char, 0x006EE538), PI_PATH + src);
    EQ_CDECL(char*, 0x00524004, (char*))(EQ_ARRAY(char, 0x006EE538));
    if (EQ_CDECL(char*, 0x00522A30, (const char*, const char*))(EQ_ARRAY(char, 0x006EE538), (const char*)0x0057dbac) != 0)
    {
        strcpy(EQ_ARRAY(char, 0x006EE538), (const char*)0x0057dbac);
        strcpy(datName, (const char*)0x0057dba0);
        strcpy(exeName, (const char*)0x0057db8c);
    }
    else
    {
        strcpy(datName, (const char*)0x0057db84);
        strcpy(exeName, (const char*)0x0057db74);
    }
    sprintf(optionalText, (const char*)0x0057db48, exeName);
    DbgLog_guess((const char*)0x0057db38);
    if (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))(datName, &locals.crcSource) != -1 &&
        (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))(exeName, &locals.crcTarget) == -1 || locals.crcSource != locals.crcTarget))
    {
        errorText[0] = 0;
        EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0,
            EQ_CDECL(int, 0x004A3D98, (const char*, const char*, char*))(datName, exeName, errorText) != -1 ? optionalText : errorText,
            (const char*)0x0056248c, 0x40000);
        PI_EXIT(0);
    }
    sprintf(optionalText, (const char*)0x0057db04, exeName);
    if (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))((const char*)0x0057daf8, &locals.crcSource) != -1 &&
        (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))((const char*)0x0057dae8, &locals.crcTarget) == -1 || locals.crcSource != locals.crcTarget))
    {
        errorText[0] = 0;
        EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0,
            EQ_CDECL(int, 0x004A3D98, (const char*, const char*, char*))((const char*)0x0057daf8, (const char*)0x0057dae8, errorText) != -1 ? optionalText : errorText,
            (const char*)0x0056248c, 0x40000);
        PI_EXIT(0);
    }
    DbgLog_guess((const char*)0x0057dad8);
    sprintf(optionalText, (const char*)0x0057daa4, exeName);
    if (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))((const char*)0x0057da9c, &locals.crcSource) != -1 &&
        (EQ_CDECL(int, 0x004A3ED7, (const char*, int*))((const char*)0x0057da90, &locals.crcTarget) == -1 || locals.crcSource != locals.crcTarget))
    {
        errorText[0] = 0;
        EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0,
            EQ_CDECL(int, 0x004A3D98, (const char*, const char*, char*))((const char*)0x0057da9c, (const char*)0x0057da90, errorText) != -1 ? optionalText : errorText,
            (const char*)0x0056248c, 0x40000);
        PI_EXIT(0);
    }
    DbgLog_guess((const char*)0x0057da80);
    errorText[0] = 0;
    EQ_CDECL(int, 0x004A3D98, (const char*, const char*, char*))((const char*)0x0057da60, (const char*)0x0057da70, errorText);
    DbgLog_guess((const char*)0x0057da50);
    locals.fndi = 0;
    len = (int)strlen(PI_PATH);
    if (PI_PATH[0] == '"')
    {
        i = 0;
        if (len > 0)
        {
            do
            {
                PI_PATH[i] = PI_PATH[i + 1];
                i++;
            } while (i < len);
        }
        len--;
    }
    if (len > 0)
    {
        if (PI_PATH[len - 1] == '\\')
        {
            PI_PATH[len - 1] = 0;
            len--;
        }
        if (len > 0)
        {
            if (PI_PATH[len - 1] == '"')
            {
                PI_PATH[len - 1] = 0;
                len--;
            }
            if (len > 0)
            {
                while (--len > 0)
                {
                    if (PI_PATH[len] == '\\')
                    {
                        locals.fndi = len + 1;
                        break;
                    }
                }
            }
        }
    }
    DbgLog_guess((const char*)0x0057da40);
    EQ_CDECL(void, 0x004E87A8, (void))();
    if (_strnicmp(EQ_ARRAY(char, 0x006EE538), (const char*)0x0057dbac, 4) == 0)
        locals.fndi += 4;
    i = 0;
    if ((int)strlen(PI_VALID) > 0)
    {
        do
        {
            if (PI_VALID[i] == 'P')
                PI_VALID[i] = '.';
            else
                PI_VALID[i] = (char)(PI_VALID[i] + 0x11);
            i++;
        } while (i < (int)strlen(PI_VALID));
    }
    if (PI_ARGC == 1 && _strcmpi(PI_VALID, PI_PATH + locals.fndi) == 0)
    {
        DbgLog_guess((const char*)0x0057da30);
        if (EQ_CDECL(int, 0x004A4069, (void))() == 0)
        {
            DbgLog_guess((const char*)0x0057da00);
            PI_EXIT(0);
        }
        PI_ARGC = EQ_CDECL(int, 0x00523F0B, (const char*, const char*, ...))((const char*)deviceNameDst, (const char*)0x0057d9e8,
            PI_WORDS, PI_WORDS + 0x100, PI_WORDS + 0x200, PI_WORDS + 0x300, PI_WORDS + 0x400, PI_WORDS + 0x500,
            PI_WORDS + 0x600, PI_WORDS + 0x700);
        if (PI_ARGC > 0)
        {
            i = 0;
            if (PI_ARGC > 0)
            {
                char* word = PI_WORDS;
                do
                {
                    if (_strnicmp(word, (const char*)0x0057d9e0, 5) == 0)
                    {
                        EQ_GLOBAL(int, 0x006EFE1C) = 1;
                        if (i >= 0)
                        {
                            if (i < PI_ARGC)
                            {
                                word = PI_WORDS + i * 0x100;
                                do
                                {
                                    strcpy(word, word + 0x100);
                                    i++;
                                    word += 0x100;
                                } while (i < PI_ARGC);
                            }
                            PI_ARGC--;
                        }
                        break;
                    }
                    if (_strnicmp(word, (const char*)0x0057d9d8, 7) == 0)
                        locals.found = 1;
                    i++;
                    word += 0x100;
                } while (i < PI_ARGC);
            }
        }
        if (locals.found == 0)
        {
            EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0, (const char*)0x0057d9bc, (const char*)0x0056248c, 0x40000);
            PI_EXIT(0);
        }
        if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))((const char*)0x0057d9b0, &statBuf) == 0)
            EQ_GLOBAL(int, 0x006EFE1C) = 1;
        if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))((const char*)0x0057d9a0, &statBuf) == 0)
            EQ_GLOBAL(int, 0x006EFE20) = 1;
        if (PI_ARGC > 0)
        {
            i = 0;
            if (PI_ARGC > 0)
            {
                char* word = PI_WORDS;
                do
                {
                    if (_strnicmp(word, (const char*)0x005588cc, 4) == 0)
                    {
                        EQ_GLOBAL(int, 0x006EFE10) = 1;
                        /* fake match: join the option checks at the shared word-removal block. */
                        goto dropWord;
                    }
                    if (_strcmpi(word, (const char*)0x0057050c) == 0)
                    {
                        EQ_GLOBAL(int, 0x006EFE14) = 1;
dropWord:
                        if (i >= 0)
                        {
                            if (i < PI_ARGC)
                            {
                                word = PI_WORDS + i * 0x100;
                                do
                                {
                                    strcpy(word, word + 0x100);
                                    i++;
                                    word += 0x100;
                                } while (i < PI_ARGC);
                            }
                            PI_ARGC--;
                        }
                        break;
                    }
                    i++;
                    word += 0x100;
                } while (i < PI_ARGC);
            }
        }
        DbgLog_guess((const char*)0x0057d990);
        if (EQ_STDCALL(int, 0x0050038C, (u32, void*))(0x101, winsock) != 0)
        {
            EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0, (const char*)0x0057d970, (const char*)0x0056248c, 0x40000);
            PI_EXIT(0);
        }
        DbgLog_guess((const char*)0x0057d960);
        EQ_GLOBAL(int, 0x0058FBB4) = EQ_GLOBAL(EnumDevicesFn_guess, 0x006E523C)((void*)0x005901b0, instance);
        if (EQ_GLOBAL(int, 0x0058FBB4) == 0)
        {
            EQ_GLOBAL(MessageBoxAFn_guess, 0x00534230)(0, (const char*)0x0057d94c, (const char*)0x0056248c, 0x40000);
            PI_EXIT(0);
        }
        DbgLog_guess((const char*)0x0057d93c);
        EQ_CDECL(void, 0x004A4637, (void))();
        EQ_CDECL(void, 0x004E8D6A, (void))();
        locals.wc.style = 8;
        locals.wc.lpfnWndProc = 0x004ea74e;
        locals.wc.cbClsExtra = 0;
        locals.wc.cbWndExtra = 0;
        locals.wc.hInstance = instance;
        locals.wc.hIcon = 0;
        locals.wc.hCursor = 0;
        locals.wc.hbrBackground = EQ_GLOBAL(GetStockObjectFn_guess, 0x00534008)(4);
        locals.wc.lpszMenuName = 0;
        locals.wc.lpszClassName = (const char*)0x0057d928;
        if (EQ_GLOBAL(RegisterClassAFn_guess, 0x005341F8)(&locals.wc) == 0)
            PI_EXIT(0);
        DbgLog_guess((const char*)0x0057d918);
        PI_HWND = EQ_GLOBAL(CreateWindowExAFn_guess, 0x005341FC)(0x40000, (const char*)0x0057d928, (const char*)0x0056248c,
            0x81000000, 0, 0, 100, 0x28, 0, 0, instance, 0);
        if (PI_HWND == 0)
            PI_EXIT(0);
        EQ_GLOBAL(void*, 0x006EE7BC) = PI_HWND;
        EQ_GLOBAL(void*, 0x006EE720) = instance;
        DbgLog_guess((const char*)0x0057d908);
        EQ_GLOBAL(HwndFn_guess, 0x00534228)(PI_HWND);
        EQ_GLOBAL(HwndFn_guess, 0x00534200)(PI_HWND);
        EQ_GLOBAL(SetWindowPosFn_guess, 0x00534204)(PI_HWND, -1, 0, 0, 0, 0, 3);
        EQ_GLOBAL(ShowWindowFn_guess, 0x0053421C)(PI_HWND, 5);
        EQ_GLOBAL(UpdateWindowFn_guess, 0x00534220)(PI_HWND);
        EQ_GLOBAL(SetWindowLongAFn_guess, 0x00534208)(PI_HWND, -10, 0);
        memset((void*)0x006b61b8, 0, 0x20c);
        EQ_GLOBAL(int, 0x006EE268) = EQ_GLOBAL(int, 0x0058FBB4);
        EQ_GLOBAL(int, 0x006B63C0) = 0;
        EQ_GLOBAL(int, 0x006B63BC) = 9000;
        EQ_GLOBAL(void*, 0x006EE26C) = EQ_GLOBAL(void*, 0x006EE7BC);
        EQ_GLOBAL(int, 0x006EE270) = 1;
        deviceNameDst = 0x006ede68;
        deviceSrc = 0x005901b8;
        do
        {
            strcpy((char*)deviceNameDst, (const char*)deviceSrc);
            deviceNameDst += 0x40;
            deviceSrc += 0x15c;
        } while (deviceSrc < 0x00591778);
        DbgLog_guess((const char*)0x0057d8fc);
        EQ_GLOBAL(UpdateWindowFn_guess, 0x00534220)(PI_HWND);
        return 1;
    }
    deviceNameDst = (int)EQ_CDECL(void*, 0x005223BC, (const char*, const char*))((const char*)0x0057d8f0, (const char*)0x0056fd18);
    if (deviceNameDst != 0)
    {
        EQ_CDECL(int, 0x00524138, (void*, const char*, ...))((void*)deviceNameDst, (const char*)0x0057d8b0, PI_PATH + locals.fndi, locals.fndi, PI_ARGC, PI_VALID);
        EQ_CDECL(int, 0x00521FBB, (void*))((void*)deviceNameDst);
    }
    PI_EXIT(0);
}
#undef PI_ARGC
#undef PI_PATH
#undef PI_WORDS
#undef PI_VALID
#undef PI_HWND
#undef PI_EXIT

// FUNCTION: TRILOGY 0x004e96b9
// Provides elapsed client time for the millisecond clock.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
// No arguments. Reads the RDTSC counter into two zeroed dwords, then returns it minus the 64-bit global at
// 0x006EFC60 (low dword first, borrow into the high dword). Both dwords are zeroed before RDTSC overwrites them.
__int64 GetCpuTicks2(void)
{
    u32 lo = 0;
    u32 hi = 0;

    __asm
    {
        rdtsc
        mov lo, eax
        mov hi, edx
    }
    return (((__int64)hi << 32) | lo) - EQ_GLOBAL(__int64, 0x006EFC60);
}

typedef u32 (__stdcall *GetPrivateProfileStringFn_guess)(const char*, const char*, const char*,
    char*, u32, const char*);
typedef int (__stdcall *WritePrivateProfileStringFn_guess)(const char*, const char*, const char*,
    const char*);
typedef int (__cdecl *CpuSpeedFunction_guess)(void);
typedef int (__stdcall *DestroyWindowFunction_guess)(void*);
typedef void (__stdcall *ExitProcessFunction_guess)(u32);

#define WINMAIN_PROFILE_SIZE 0x80
#define WINMAIN_FAILURE_EXIT_CODE -42
#define CPU_SPEED_SAMPLE_DIVISOR 50
#define WINMAIN_ACCOUNT_FIELD_LIMIT 0x19

// FUNCTION: TRILOGY 0x004e96ec
// Launches EverQuest, initializes its graphics path, and runs the client session.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
int __stdcall WinMain_guess(u32 instance, u32 previousInstance, char* commandLine, int showCommand)
{
    int restartCount = 0;
    int cpuSpeed2;
    char graphicsDllName[WINMAIN_PROFILE_SIZE];
    u8 fileStatus[36];
    int profileCompare;
    int cpuSpeed3;
    int cpuSpeedDifference;
    int graphicsResult;
    int textCompare;
    size_t textLength;
    int destroyWindowResult;

    if (EQ_CDECL(int, 0x00523BE0, (const char*, void*))((const char*)0x0053DD38, fileStatus) != 0)
        goto loadGraphicsProfile;
    if (EQ_CDECL(int, 0x004DDE87, (const char*))((const char*)0x0057DCA8) != -1)
        goto graphicsInitialized;
    DbgLog_guess((const char*)0x0057DC74);
    EQ_STDCALL(void, 0x00522DAD, (int))(WINMAIN_FAILURE_EXIT_CODE);

loadGraphicsProfile:
    EQ_GLOBAL(GetPrivateProfileStringFn_guess, 0x0053418C)((const char*)0x0053DB3C,
        (const char*)0x0057DC68, (const char*)0x0053DB48, graphicsDllName,
        WINMAIN_PROFILE_SIZE, (const char*)0x0053DB50);
    if (graphicsDllName[0] == '*' && graphicsDllName[5] == '*')
        strcpy(graphicsDllName,
            (const char*)0x0057DC58);
    if (EQ_CDECL(int, 0x004DDE87, (const char*))(graphicsDllName) == -1)
    {
        DbgLog_guess((const char*)0x0057DC74);
        EQ_STDCALL(void, 0x00522DAD, (int))(WINMAIN_FAILURE_EXIT_CODE);
    }
    EQ_GLOBAL(WritePrivateProfileStringFn_guess, 0x00534188)((const char*)0x0053DB3C,
        (const char*)0x0057DC68, graphicsDllName, (const char*)0x0053DB50);

graphicsInitialized:
    EQ_GLOBAL(GetPrivateProfileStringFn_guess, 0x0053418C)((const char*)0x0053DB3C,
        (const char*)0x0057DC50, (const char*)0x0053C344, graphicsDllName,
        WINMAIN_PROFILE_SIZE, (const char*)0x0053DB50);
    profileCompare = _strcmpi(
        graphicsDllName, (const char*)0x0057DC48);
    EQ_GLOBAL(__int64, 0x006EFC60) = 0;
    EQ_GLOBAL(u8, 0x006EFC70) = (profileCompare == 0);
    EQ_GLOBAL(__int64, 0x006EFC60) = EQ_CDECL(__int64, 0x004E96B9, (void))();

    cpuSpeed3 = EQ_GLOBAL(CpuSpeedFunction_guess, 0x006E50B8)();
    cpuSpeed2 = EQ_GLOBAL(CpuSpeedFunction_guess, 0x006E50B4)();
    cpuSpeedDifference = cpuSpeed2 - cpuSpeed3;
    if (abs(cpuSpeedDifference)
        < cpuSpeed3 / CPU_SPEED_SAMPLE_DIVISOR)
    {
        EQ_GLOBAL(__int64, 0x006EFC68) = cpuSpeed2;
    }
    else
    {
        EQ_GLOBAL(__int64, 0x006EFC68) = cpuSpeed3;
        DbgLog_guess((const char*)0x0057DC3C,
            abs(cpuSpeedDifference));
    }
    DbgLog_guess((const char*)0x0057DC24);
    EQ_CDECL(void, 0x004E8E0C, (char*, u32))(commandLine, instance);

    for (;;)
    {
        EQ_CDECL(void, 0x0040101D, (void))();
        graphicsResult = EQ_CDECL(int, 0x004E8D16, (void))();
        EQ_CDECL(void, 0x0040101D, (void))();
        if (graphicsResult == -1 || (restartCount > 0 && graphicsResult == 0))
            goto shutdown;

        EQ_GLOBAL(u8, 0x006EE758) = (u8)graphicsResult;
        if (graphicsResult != 0)
        {
            strcpy((char*)0x006B61BC,
                (const char*)0x006EE274);
            strcpy((char*)0x006B623C,
                (const char*)0x006EE2F4);
            strcpy((char*)0x006B62BC,
                (const char*)0x006EE3F4);
            strcpy((char*)0x006B633C,
                (const char*)0x006EE474);
            textLength = strlen((const char*)0x006B633C);
            if (textLength < WINMAIN_ACCOUNT_FIELD_LIMIT)
                EQ_ARRAY(char, 0x006B633C)[0] = 0;

            EQ_GLOBAL(u32, 0x00590180) = EQ_GLOBAL(u32, 0x006EE4F4);
            textCompare = _strcmpi(
                (const char*)0x006B61BC, (const char*)0x0057DC1C);
            if (textCompare == 0)
                EQ_ARRAY(char, 0x006B61BC)[0] &= (char)textCompare;
            textCompare = _strcmpi(
                (const char*)0x006B623C, (const char*)0x0057DC10);
            if (textCompare == 0)
                EQ_ARRAY(char, 0x006B623C)[0] &= (char)textCompare;

            textLength = strlen((const char*)0x006B61BC);
            if (textLength != 0)
            {
                textLength = strlen((const char*)0x006B623C);
                if (textLength != 0)
                    EQ_GLOBAL(int, 0x006B63C0) = 1;
            }
        }

        EQ_GLOBAL(int, 0x006EE270) = EQ_CDECL(int, 0x004E99DD,
            (u32, u32, char*, int))(instance, previousInstance, commandLine, showCommand);
        EQ_CDECL(void, 0x004EAF2C, (int))(1);
        if (EQ_GLOBAL(int, 0x006E4E8C) == 1)
            break;
        ++restartCount;
        if (graphicsResult == 0)
            goto shutdown;
    }

    DbgLog_guess((const char*)0x0057DC08);

shutdown:
    EQ_CDECL(void, 0x0040101D, (void))();
    destroyWindowResult = EQ_GLOBAL(DestroyWindowFunction_guess, 0x00534234)(
        EQ_GLOBAL(void*, 0x006EE754));
    EQ_CDECL(void, 0x004DFE0A, (void))();
    EQ_GLOBAL(ExitProcessFunction_guess, 0x005341CC)(0);
    return destroyWindowResult;
}

#define EVERQUEST_ALLOCATION_SIZE 0x22898
#define FILTER_ENTRY_SIZE 0x1e

typedef int (__stdcall *WindowActionFn_guess)(void* window);
typedef void (__stdcall *CriticalSectionFn_guess)(void* section);
typedef void (__stdcall *StartupInfoFn_guess)(void* startupInfo);
typedef int (__stdcall *OpenFileFn_guess)(const char* path, void* fileInfo, u32 mode);
typedef int (__stdcall *CloseHandleFn_guess)(void* handle);
typedef void* (__stdcall *CreateThreadFn_guess)(void* attributes, u32 stackSize,
    void* startAddress, void* parameter, u32 flags, u32* threadId);

typedef struct ExecuteEverQuestFrame_guess
{
    union
    {
        void* threadHandle;
        int returnValue;
    };
    u32 threadId;
    void* allocatedEverQuest;
    u8 startupInfo[0x44];
    u8 openFileInfo[0x88];
} ExecuteEverQuestFrame_guess;
C_ASSERT(sizeof(ExecuteEverQuestFrame_guess) == 0xd8);

class EverQuestConstruction_guess
{
public:
    char storage[EVERQUEST_ALLOCATION_SIZE];

    // EXTERNAL: TRILOGY 0x004C3176 EverQuestConstruction_guess::EverQuestConstruction_guess
    EverQuestConstruction_guess(u32 window);

};

// FUNCTION: TRILOGY 0x004e99dd
// Game session: login/password from the command line, creates and runs CEverQuest, then shuts everything down.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
int ExecuteEverQuest(u32 instance, u32 previousInstance, char* commandLine, int showCommand)
{
    u16* keyRecord;
    size_t passwordLength;
    int state;
    u8* filterName;
    int fileHandle;
    u32* filterCount;
    ExecuteEverQuestFrame_guess startupFrame;
    void* everQuest;
    CDisplay* display;
    int skyMode;

    startupFrame.threadHandle = 0;
    if (EQ_GLOBAL(u8, 0x006EFC70) == 0)
    {
        EQ_CDECL(int, 0x004EB737, (u32, u32))(
            EQ_GLOBAL(u32, 0x006EE7BC), EQ_GLOBAL(u32, 0x006EE720));
        EQ_CDECL(int, 0x004EB569, (u32, u32))(
            EQ_GLOBAL(u32, 0x006EE7BC), EQ_GLOBAL(u32, 0x006EE720));
        EQ_GLOBAL(WindowActionFn_guess, 0x005341F4)(
            (void*)EQ_GLOBAL(u32, 0x006EE7BC));
        EQ_GLOBAL(WindowActionFn_guess, 0x00534228)(
            (void*)EQ_GLOBAL(u32, 0x006EE7BC));
        EQ_GLOBAL(WindowActionFn_guess, 0x00534200)(
            (void*)EQ_GLOBAL(u32, 0x006EE7BC));
        ((WinReacquireFn_guess)(*(void***)g_keyboard_device_guess)[7])(
            g_keyboard_device_guess);
        ((WinReacquireFn_guess)(*(void***)g_mouse_device_guess)[7])(
            g_mouse_device_guess);
    }

    EQ_GLOBAL(u32, 0x006E4E8C) = 0;
    EQ_CDECL(void, 0x004F4740, (int))(8000);
    keyRecord = EQ_ARRAY(u16, 0x006E3538);
    do
    {
        *keyRecord = 0xffff;
        keyRecord += 0x0c;
    } while ((int)keyRecord < 0x006E4D38);

    if (EQ_GLOBAL(int, 0x006B63C0) == 0 && EQ_GLOBAL(int, 0x006EFE18) > 2)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_ARRAY(char, 0x006B61BC), EQ_ARRAY(char, 0x006ECE64));
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_ARRAY(char, 0x006B623C), EQ_ARRAY(char, 0x006ECF64));
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_ARRAY(char, 0x006B62BC), EQ_ARRAY(char, 0x006ED064));
        EQ_CDECL(char*, 0x005219C0, (char*, const char*))(
            EQ_ARRAY(char, 0x006B633C), EQ_ARRAY(char, 0x006ED264));
        passwordLength = EQ_CDECL(size_t, 0x005223D0, (const char*))(
            EQ_ARRAY(char, 0x006B633C));
        if (passwordLength < 0x19)
            EQ_GLOBAL(char, 0x006B633C) = 0;
        if (EQ_GLOBAL(int, 0x006EFE18) > 5)
            EQ_GLOBAL(int, 0x006B63C0) = EQ_CDECL(int, 0x00522781, (const char*))(
                EQ_ARRAY(char, 0x006ED364));
    }

    if (EQ_CDECL(int, 0x00524240, (const char*, const char*))(
            EQ_ARRAY(char, 0x006B61BC), EQ_ARRAY(char, 0x0057DC1C)) == 0)
        EQ_GLOBAL(char, 0x006B61BC) = 0;
    if (EQ_CDECL(int, 0x00524240, (const char*, const char*))(
            EQ_ARRAY(char, 0x006B623C), EQ_ARRAY(char, 0x0057DC10)) == 0)
        EQ_GLOBAL(char, 0x006B623C) = 0;

    EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(
        EQ_ARRAY(char, 0x006B63D8), 0, 0x2c4c0);
    EQ_GLOBAL(int, 0x006B63FC) = 0x280;
    EQ_GLOBAL(int, 0x006B6400) = 0x1e0;
    EQ_CDECL(void, 0x004EAA05, (void))();

    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x006E3388));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x006EFA18));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x006048E8));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x005FE988));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x005FE7A0));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x006B3298));
    EQ_GLOBAL(CriticalSectionFn_guess, 0x005340E0)(EQ_ARRAY(char, 0x006018C8));
    EQ_CDECL(void, 0x004C2F7C, (void))();

    startupFrame.allocatedEverQuest = new EverQuestConstruction_guess(
        EQ_GLOBAL(u32, 0x006EE7BC));
    EQ_GLOBAL(void*, 0x006EFE2C) = startupFrame.allocatedEverQuest;

    startupFrame.returnValue = -1;
    everQuest = EQ_GLOBAL(void*, 0x006EFE2C);
    if (*(int*)everQuest == 1)
        EQ_CDECL(void, 0x004C1703, (const char*, ...))(
            EQ_ARRAY(char, 0x0057DCD8));
    else if (everQuest != 0 && EQ_GLOBAL(int, 0x006B6EF4) != 0)
    {
        EQ_CDECL(void, 0x004E7A0E, (u32))(EQ_GLOBAL(u32, 0x006EE754));
        EQ_CDECL(void, 0x004E7C5E, (void))();
        EQ_GLOBAL(WindowActionFn_guess, 0x005341F4)(
            (void*)EQ_GLOBAL(u32, 0x006EE7BC));
        EQ_GLOBAL(WindowActionFn_guess, 0x00534228)(
            (void*)EQ_GLOBAL(u32, 0x006EE7BC));
        if (g_keyboard_device_guess != 0)
            ((WinReacquireFn_guess)(*(void***)g_keyboard_device_guess)[7])(
                g_keyboard_device_guess);
        if (g_mouse_device_guess != 0)
            ((WinReacquireFn_guess)(*(void***)g_mouse_device_guess)[7])(
                g_mouse_device_guess);
    }

    skyMode = EQ_GLOBAL(int, 0x006B6870);
    display = g_cdisplay_guess;
    if (skyMode == 0)
        display->skyLayerMode_guess = 0;
    else if (skyMode == 1)
    {
        display->skyLayerMode_guess = EQ_GLOBAL(u8, 0x006B6726);
        EQ_METHOD(1, void, 0x004124BE, (CDisplay*, int), display)(0);
    }
    else if (skyMode == 2)
    {
        display->skyLayerMode_guess = EQ_GLOBAL(u8, 0x006B6726);
        EQ_METHOD(1, void, 0x004124BE, (CDisplay*, int), display)(1);
    }

    state = 0;
    filterCount = EQ_ARRAY(u32, 0x005FE954);
    filterName = EQ_ARRAY(u8, 0x005FE8BD);
    do
    {
        EQ_ARRAY(u8, 0x005FE8B8)[state] = 0;
        *filterName = 0;
        *filterCount = 0;
        filterName += FILTER_ENTRY_SIZE;
        state++;
        filterCount++;
    } while ((int)filterName < 0x005FE953);
    EQ_GLOBAL(u8, 0x005FE968) = 0;

    EQ_GLOBAL(StartupInfoFn_guess, 0x00534154)(startupFrame.startupInfo);
    fileHandle = EQ_GLOBAL(OpenFileFn_guess, 0x005341D8)(
        EQ_ARRAY(char, 0x006EFA48), startupFrame.openFileInfo, 2);
    if (fileHandle != -1)
    {
        EQ_CDECL(void, 0x004C1703, (const char*, ...))(
            EQ_ARRAY(char, 0x0057DCB8));
        EQ_GLOBAL(CloseHandleFn_guess, 0x005341D4)((void*)fileHandle);
        *(int*)EQ_GLOBAL(void*, 0x006EFE2C) = 1;
    }

    everQuest = EQ_GLOBAL(void*, 0x006EFE2C);
    if (everQuest == 0)
        startupFrame.threadHandle = 0;
    else
    {
        if (*(int*)everQuest == 0)
            startupFrame.returnValue = EQ_METHOD(2, int, 0x004D4676, (void*, u32, int), everQuest)
                (EQ_GLOBAL(u32, 0x006EE754), (int)instance);
        if (*(int*)everQuest == 1)
        {
            if (everQuest != 0 && everQuest != 0)
            {
                EQ_METHOD(0, void, 0x004C49EE, (void*), everQuest)();
                EQ_CDECL(void, 0x005218BB, (void*))(everQuest);
            }
            EQ_GLOBAL(void*, 0x006EFE2C) = 0;
            if (EQ_GLOBAL(void*, 0x006B3194) != 0)
            {
                EQ_CDECL(void, 0x004E0601, (void*))(
                    EQ_GLOBAL(void*, 0x006B3194));
                EQ_GLOBAL(void*, 0x006B3194) = 0;
            }
            EQ_CDECL(void, 0x004F4860, (void))();
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x006E3388));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x006048E8));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x005FE988));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x005FE7A0));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x006EFA18));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x006B3298));
            EQ_GLOBAL(CriticalSectionFn_guess, 0x005341C8)(EQ_ARRAY(char, 0x006018C8));
            EQ_CDECL(void, 0x004E7AB2, (void))();
            return startupFrame.returnValue;
        }
        startupFrame.threadHandle = EQ_GLOBAL(CreateThreadFn_guess, 0x005341D0)(
            0, 0x10000, (void*)0x004E88C2, 0, 0, &startupFrame.threadId);
        EQ_CDECL(void, 0x004E8B63, (int))(0);
    }

    EQ_CDECL(void, 0x004E7AB2, (void))();
    if (EQ_GLOBAL(void*, 0x006B3194) != 0)
    {
        EQ_CDECL(void, 0x004E0601, (void*))(
            EQ_GLOBAL(void*, 0x006B3194));
        EQ_GLOBAL(void*, 0x006B3194) = 0;
    }
    EQ_CDECL(void, 0x004F4860, (void))();
    EQ_CDECL(void, 0x004EB6FF, (void))();
    EQ_CDECL(void, 0x004EB8B4, (void))();
    if (startupFrame.threadHandle != 0)
        EQ_GLOBAL(CloseHandleFn_guess, 0x005341D4)(startupFrame.threadHandle);
    return 1;
}

#undef FILTER_ENTRY_SIZE
#undef EVERQUEST_ALLOCATION_SIZE

// FUNCTION: TRILOGY 0x004e9dfb
// Applies a typed key (backspace, Enter, printable) to the active text box: subject/message, chat line, others.
/* fake match: repeat the final-character lookup so VC6 keeps its DEC before LEA. */
void HandleTypedChar_guess(int scanCodeCharacter)
{
    char value;
    int tmp;
    size_t len;
    char* lastChar;
    unsigned char typedChar;
    char editedText[2048];

    if (EQ_GLOBAL(void*, 0x006E4E44) != 0 &&
        pinstEQ_Main_guess->pendingKeyBinding_guess != '\0')
        return;

    switch (scanCodeCharacter)
    {
    case 8:
    {
        if (EQ_GLOBAL(void*, 0x006E4DE8) != 0 &&
            pinstEQ_TextIO_guess->channelPickerOpen != '\0')
        {
            len = strlen(EQ_ARRAY(char, 0x006B642C));
            if (len == 0)
                return;
            EQ_ARRAY(char, 0x006B642B)[len] = 0;
            return;
        }
        if (EQ_GLOBAL(void*, 0x006E4E44) == 0)
            return;
        if (pinstEQ_Main_guess->inspectActive_guess != '\0' &&
            pinstEQ_Main_guess->inspectSpawnId_guess ==
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id)
        {
            len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xeda));
            if (len == 0)
                return;
            --len;
            lastChar = (char*)EQ_GLOBAL(void*, 0x006E4E44) + len + 0xeda;
            EQ_GLOBAL(int, 0x006EFDD4) = EQ_GLOBAL(int, 0x006EFDD4) -
                *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDDC) +
                    *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + len + 0xeda) * 4);
            *lastChar = '\0';
        }
        else
        {
            value = pinstEQ_Main_guess->screenMode_guess;
            if (value == '\b')
            {
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc43));
                if (len == 0)
                    return;
                *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc42 + len) = 0;
                return;
            }
            if (value >= '\t' && value <= '\n')
                return;
            if (value == '\v')
            {
                if (pinstEQ_Main_guess->boardEditSubject_guess != '\0')
                {
                    tmp = strcmp((const char*)0x0055ae7c,
                         (const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188));
                    if (tmp == 0)
                    {
                        strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188, (const char*)0x0058fba4);
                        return;
                    }
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188));
                    if (len == 0)
                        return;
                    *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x187 + len) = 0;
                    return;
                }
                tmp = strcmp((const char*)0x0055ae68,
                     (const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6));
                if (tmp == 0)
                {
                    strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6, (const char*)0x0058fba4);
                    return;
                }
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6));
                if (len == 0)
                    return;
                *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a5 + len) = 0;
                return;
            }
            if (pinstEQ_Main_guess->keyCaptureActive_guess != 0)
            {
                pinstEQ_Main_guess->panelRedraw_guess = 1;
                return;
            }
            if (pinstEQ_Main_guess->socialEditorOpen_guess == 0)
                return;
            value = pinstEQ_Main_guess->socialEditLine_guess;
            if (value < 0)
                return;
            if (value == 5)
            {
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x682e6));
                if (len == 0)
                    return;
                --len;
                lastChar = (char*)EQ_GLOBAL(void*, 0x006E4E44) + len + 0x682e6;
                EQ_GLOBAL(int, 0x006EFDC8) = EQ_GLOBAL(int, 0x006EFDC8) -
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) +
                        *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + len + 0x682e6) * 4);
                *lastChar = '\0';
            }
            else
            {
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                        (int)value * 0x3c + 0x682ef));
                if (len == 0)
                    return;
                --len;
                EQ_GLOBAL(int, 0x006EFDCC) = EQ_GLOBAL(int, 0x006EFDCC) -
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) +
                        *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                            (int)pinstEQ_Main_guess->socialEditLine_guess *
                                0x3c + 0x682ef + len) * 4);
                *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                    (int)pinstEQ_Main_guess->socialEditLine_guess *
                        0x3c + 0x682ef + len) = 0;
                return;
            }
        }
        break;
    }
    case 10:
    case 0xd:
    {
        if (EQ_GLOBAL(void*, 0x006E4DE8) != 0 &&
            pinstEQ_TextIO_guess->channelPickerOpen != '\0')
            return;
        if (EQ_GLOBAL(void*, 0x006E4E44) == 0)
            return;
        if (pinstEQ_Main_guess->inspectActive_guess != '\0' &&
            pinstEQ_Main_guess->inspectSpawnId_guess ==
                EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id)
            return;
        if (pinstEQ_Main_guess->screenMode_guess >= '\b')
            return;
        if (pinstEQ_Main_guess->keyCaptureActive_guess != 0)
        {
            pinstEQ_Main_guess->keyCaptureActive_guess = 0;
            EQ_GLOBAL(int, 0x006EFDD0) = 0;
            return;
        }
        if (pinstEQ_Main_guess->socialEditorOpen_guess == 0)
            return;
        if (pinstEQ_Main_guess->socialEditLine_guess < 0)
            return;
        pinstEQ_Main_guess->socialEditLine_guess = -1;
        EQ_GLOBAL(int, 0x006EFDC8) = 0;
        EQ_GLOBAL(int, 0x006EFDCC) = 0;
        return;
    }
    default:
    {
        typedChar = (unsigned char)scanCodeCharacter;
        if (EQ_GLOBAL(void*, 0x006E4DE8) != 0 &&
            pinstEQ_TextIO_guess->channelPickerOpen != '\0')
        {
            tmp = isprint(scanCodeCharacter);
            if (tmp == 0)
                return;
            len = strlen(EQ_ARRAY(char, 0x006B642C));
            if (len >= 0x10)
                return;
            len = strlen(EQ_ARRAY(char, 0x006B642C));
            EQ_ARRAY(char, 0x006B642C)[len] = typedChar;
            char *temp2 = EQ_ARRAY(char, 0x006B642D);
            temp2[len] = 0;
            return;
        }
        if (EQ_GLOBAL(void*, 0x006E4E44) != 0)
        {
            if (pinstEQ_Main_guess->inspectActive_guess != '\0' &&
                pinstEQ_Main_guess->inspectSpawnId_guess ==
                    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->spawn_id)
            {
                tmp = isprint(scanCodeCharacter);
                if (tmp == 0)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xeda));
                if (len >= 200)
                    return;
                if (EQ_GLOBAL(int, 0x006EFDD4) >= 0x438)
                    return;
                if (0xb < pinstEQ_Main_guess->bioLineCount_guess)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xeda));
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xeda + len) = typedChar;
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xedb + len) = 0;
                value = *(char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0xeda + scanCodeCharacter);
                if (value <= 0)
                    return;
                EQ_GLOBAL(int, 0x006EFDD4) = EQ_GLOBAL(int, 0x006EFDD4) +
                    *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDDC) + value * 4);
                return;
            }
            value = pinstEQ_Main_guess->screenMode_guess;
            if (value == '\b')
            {
                tmp = isprint(scanCodeCharacter);
                if (tmp == 0)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc43));
                if (len >= 0x3ff)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc43));
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc43 + len) = typedChar;
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x8fc44 + len) = 0;
                return;
            }
            if (value >= '\t' && value <= '\n')
                return;
            if (value == '\v')
            {
                if (pinstEQ_Main_guess->boardEditSubject_guess != '\0')
                {
                    tmp = strcmp((const char*)0x0055ae7c,
                         (const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188));
                    if (tmp == 0)
                        strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188, (const char*)0x0058fba4);
                    tmp = isprint(scanCodeCharacter);
                    if (tmp == 0)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188));
                    if (len >= 0x1d)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188));
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x188 + len) = typedChar;
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x189 + len) = 0;
                    return;
                }
                tmp = strcmp((const char*)0x0055ae68,
                     (const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6));
                if (tmp == 0)
                    strcpy((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6, (const char*)0x0058fba4);
                tmp = isprint(scanCodeCharacter);
                if (tmp == 0)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6));
                if (len >= 0x3ff)
                    return;
                if (pinstEQ_Main_guess->boardBodyLineCount_guess >= 10)
                    return;
                len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6));
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a6 + len) = typedChar;
                *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x1a7 + len) = 0;
                return;
            }
            if (pinstEQ_Main_guess->keyCaptureActive_guess != 0)
            {
                pinstEQ_Main_guess->panelRedraw_guess = 1;
                return;
            }
            if (pinstEQ_Main_guess->socialEditorOpen_guess != 0 &&
                pinstEQ_Main_guess->socialEditLine_guess >= 0)
            {
                if (pinstEQ_Main_guess->socialEditLine_guess == '\x05')
                {
                    tmp = isprint(scanCodeCharacter);
                    if (tmp == 0)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x682e6));
                    if (len >= 6)
                        return;
                    if (EQ_GLOBAL(int, 0x006EFDC8) >= 0x37)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x682e6));
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x682e6 + len) = typedChar;
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) + 0x682e7 + len) = 0;
                    tmp = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) + scanCodeCharacter * 4);
                    if (tmp <= 0)
                        return;
                    EQ_GLOBAL(int, 0x006EFDC8) = EQ_GLOBAL(int, 0x006EFDC8) + tmp;
                    return;
                }
                else
                {
                    tmp = isprint(scanCodeCharacter);
                    if (tmp == 0)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                            (int)pinstEQ_Main_guess->socialEditLine_guess * 0x3c +
                            0x682ef));
                    if (len >= 0x3b)
                        return;
                    if (EQ_GLOBAL(int, 0x006EFDCC) >= 0x17f)
                        return;
                    len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                            (int)pinstEQ_Main_guess->socialEditLine_guess * 0x3c +
                            0x682ef));
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                        (int)pinstEQ_Main_guess->socialEditLine_guess * 0x3c +
                        0x682ef + len) = typedChar;
                    *(unsigned char*)((char*)EQ_GLOBAL(void*, 0x006E4E44) +
                        (int)pinstEQ_Main_guess->socialEditLine_guess * 0x3c +
                        0x682f0 + len) = 0;
                    tmp = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFDE0) + scanCodeCharacter * 4);
                    if (tmp <= 0)
                        return;
                    EQ_GLOBAL(int, 0x006EFDCC) = EQ_GLOBAL(int, 0x006EFDCC) + tmp;
                    return;
                }
            }
        }
        tmp = isprint(scanCodeCharacter);
        if (tmp != 0 && scanCodeCharacter != 0x5d && scanCodeCharacter != 0x7d &&
            scanCodeCharacter != 0x5b && scanCodeCharacter != 0x7b &&
            (len = strlen((const char*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8)),
             len < 0xff && EQ_GLOBAL(void*, 0x006E4DE8) != 0 &&
             EQ_GLOBAL(char, 0x006B6404) == '\0'))
        {
            tmp = pinstEQ_TextIO_guess->inputCursor;
            strcpy(editedText, (const char*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8));
            editedText[tmp] = typedChar;
            strcpy(editedText + tmp + 1,
                 (const char*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + tmp + 0xba8));
            strcpy((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8, editedText);
            EQ_METHOD(1, void, 0x004CF7A1, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(
                 pinstEQ_TextIO_guess->inputCursor + 1);
            typedef int (__stdcall *PostMessageA_guess_t)(void*, u32, u32, u32);
            EQ_GLOBAL(PostMessageA_guess_t, 0x0053422C)
                ((void*)EQ_GLOBAL(u32, 0x006B63D8), 0x4ca, 0, 0);
        }
        break;
    }
    }
    return;
}

// FUNCTION: TRILOGY 0x004ea641
// Edits the active text field with letters and backspace.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
// @early-stop: one instruction short: VC6 folds the len++ into [ecx+eax+1] and never emits the inc eax before the 0 store
void ProcessName(int scanCodeCharacter)
{
    size_t textLength;
    int alphaStatus;
    char normalizedCharacter;

    if (EQ_GLOBAL(void*, 0x006E4E70) != 0 &&
        pinstEQ_CharCreateS_guess->awaitingApproval == 0)
    {
        textLength = strlen(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660));
        if (0xf < textLength)
        {
            strcpy(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660), (const char*)0x0058fba4);
        }
        if (scanCodeCharacter != 8)
        {
            alphaStatus = isalpha(scanCodeCharacter);
            if (alphaStatus != 0)
            {
                textLength = strlen(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660));
                if (textLength < 0xf)
                {
                    textLength = strlen(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660));
                    if ((int)textLength > 0)
                    {
                        if (scanCodeCharacter >= 'A' && scanCodeCharacter <= 'Z')
                            normalizedCharacter = scanCodeCharacter + 0x20;
                        else
                            normalizedCharacter = scanCodeCharacter;
                    }
                    else
                    {
                        if (scanCodeCharacter >= 'a' && scanCodeCharacter <= 'z')
                            normalizedCharacter = scanCodeCharacter - 0x20;
                        else
                            normalizedCharacter = scanCodeCharacter;
                    }
                    *(char*)(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660) + textLength) = normalizedCharacter;
                    *(u8*)(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660) + ++textLength) = 0;
                    *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x13ec) = 1;
                    return;
                }
            }
        }
        else
        {
            textLength = strlen(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660));
            if (textLength != 0)
            {
                *(u8*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0x13ec) = 1;
                *(u8*)(*(char**)((char*)EQ_GLOBAL(void*, 0x006E4E70) + 0x660) + textLength - 1) = 0;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x004ea74e
// Main window procedure: refuses power suspend, handles close/paint and voice-recognition messages (mic on/off).
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
long __stdcall EverQuest_wndproc_guess(void* hWnd, u32 message, u32 wParam, long lParam)
{
    typedef void* (__stdcall *GetForegroundWindow_guess_t)(void);
    typedef void (__stdcall *CriticalSectionFn_guess_t)(void*);
    typedef void* (__stdcall *BeginPaint_guess_t)(void*, void*);
    typedef int (__stdcall *EndPaint_guess_t)(void*, void*);
    typedef int (__stdcall *WindowHandleFn_guess_t)(void*);
    typedef long (__stdcall *DefWindowProcA_guess_t)(void*, u32, u32, long);
    typedef void (__cdecl *ChangeDeviceResolution_guess_t)(long);
    int tmp;
    char *pcVar2;
    size_t textLen;
    long LVar4;
    u32 value;
    u8 paintStruct[0x40];

    if (EQ_GLOBAL(void*, 0x006EFE2C) == 0)
        goto callDefProc;
    tmp = *(int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 4);
    if (((tmp == 5) || (tmp == 1)) || (tmp == 2))
    {
        EQ_GLOBAL(GetForegroundWindow_guess_t, 0x005341F0)();
    }
    EQ_GLOBAL(CriticalSectionFn_guess_t, 0x00534194)((void*)0x006efa18);
    tmp = EQ_METHOD(0, int, 0x00408E7F, (void*), EQ_GLOBAL(void*, 0x006E4DE0))();
    if (tmp == 0)
    {
        if ((message == 0x4642) && (wParam == 0x4242))
        {
            EQ_GLOBAL(ChangeDeviceResolution_guess_t, 0x006E516C)(lParam);
        }
        EQ_GLOBAL(CriticalSectionFn_guess_t, 0x00534190)((void*)0x006efa18);
        goto callDefProc;
    }
    if (message < 0x84)
    {
        if (message != 0x83)
        {
            if (message < 0xe)
            {
                if (message != 0xd)
                {
                    if (message == 2)
                    {
                        if (EQ_GLOBAL(int, 0x0057D4E0) == 0)
                        {
                            EQ_CDECL(void, 0x0040101D, (void))();
                        }
                        goto handled;
                    }
                    if ((message == 3) || (message == 5))
                        goto handled;
                    goto unhandled;
                }
            }
            else
            {
                if (message == 0xf)
                {
                    EQ_GLOBAL(BeginPaint_guess_t, 0x005341E4)(hWnd, paintStruct);
                    EQ_GLOBAL(EndPaint_guess_t, 0x005341E8)(hWnd, paintStruct);
                    goto handled;
                }
                if (message == 0x10)
                {
                    EQ_CDECL(void, 0x004A9334, (void*, const char*))
                        (EQ_GLOBAL(void*, 0x006E4D8C), (const char*)0x0053e89c);
                    goto handled;
                }
                if ((message < 0x46) || (0x47 < message))
                    goto unhandled;
            }
        }
    }
    else if (message != 0x85)
    {
        if (message == 0x218)
        {
            if (wParam == 0)
                return 0x424d5144;
            goto handled;
        }
        if (message == 0x4c8)
        {
            tmp = EQ_CDECL(int, 0x004E7CF3, (void))();
            if ((tmp != 0) && (EQ_GLOBAL(void*, 0x006EFE2C) != 0))
            {
                tmp = EQ_CDECL(int, 0x004E7CD3, (u32))(EQ_GLOBAL(u32, 0x006B63D8));
                if (tmp == 0)
                    pcVar2 = (char*)0x0057dd00;
                else
                    pcVar2 = (char*)0x0057dd24;
                EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), EQ_GLOBAL(void*, 0x006EFE2C))(pcVar2);
            }
            goto handled;
        }
        if (message == 0x4c9)
        {
            tmp = EQ_CDECL(int, 0x004E7CF3, (void))();
            if ((tmp != 0) && (EQ_GLOBAL(void*, 0x006EFE2C) != 0))
            {
                EQ_CDECL(void, 0x004E7D3B, (u32, u32, u32))(0, 0x0fffffff, 0);
            }
            goto handled;
        }
        if (message == 0x4ca)
        {
            tmp = EQ_CDECL(int, 0x004E7CF3, (void))();
            if ((tmp != 0) && (EQ_GLOBAL(void*, 0x006EFE2C) != 0))
            {
                pcVar2 = (char*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xba8);
                value = 0;
                textLen = strlen(pcVar2);
                EQ_CDECL(int, 0x004E7CFF, (u32, u32, u32, u32))(0, (u32)textLen, (u32)pcVar2, value);
            }
            goto handled;
        }
        if (message == 0x4cb)
        {
            tmp = EQ_CDECL(int, 0x004E7CF3, (void))();
            if ((tmp != 0) && (EQ_GLOBAL(void*, 0x006EFE2C) != 0))
            {
                EQ_CDECL(void, 0x004E7D73, (char*))((char*)0x006b752b);
            }
            goto handled;
        }
        if (message == 0x4642)
        {
            if (wParam == 0x4242)
                EQ_GLOBAL(ChangeDeviceResolution_guess_t, 0x006E516C)(lParam);
            goto handled;
        }
        goto unhandled;
    }
    if (EQ_GLOBAL(u8, 0x006EFC70) == 0)
    {
        EQ_GLOBAL(WindowHandleFn_guess_t, 0x005341F4)(EQ_GLOBAL(void*, 0x006EE7BC));
        EQ_GLOBAL(WindowHandleFn_guess_t, 0x00534228)(EQ_GLOBAL(void*, 0x006EE7BC));
        EQ_GLOBAL(WindowHandleFn_guess_t, 0x00534200)(EQ_GLOBAL(void*, 0x006EE7BC));
        ((WinReacquireFn_guess)(*(u32*)(*(u32*)g_mouse_device_guess + 0x1c)))
            (g_mouse_device_guess);
        ((WinReacquireFn_guess)(*(u32*)(*(u32*)g_keyboard_device_guess + 0x1c)))
            (g_keyboard_device_guess);
    }
handled:
    EQ_GLOBAL(CriticalSectionFn_guess_t, 0x00534190)((void*)0x006efa18);
    return 0;
unhandled:
    EQ_GLOBAL(CriticalSectionFn_guess_t, 0x00534190)((void*)0x006efa18);
callDefProc:
    LVar4 = EQ_GLOBAL(DefWindowProcA_guess_t, 0x005341EC)(hWnd, message, wParam, lParam);
    return LVar4;
}

// FUNCTION: TRILOGY 0x004eaa05
// Sets the default keyboard controls used to play the client.
// ORIGINAL FILE: Source/PC Source/EverQuest/Win.cpp (from Mac debug map)
void default_key_bindings(void)
{
    u32 *entryPtr;
    int remainingCount;

    EQ_GLOBAL(u8, 0x006B6458) = 0;
    entryPtr = EQ_ARRAY(u32, 0x006E15E4);
    EQ_GLOBAL(u16, 0x006B641C) = 4;
    remainingCount = 0x100;
    do
    {
        entryPtr[-0x100] = 0xFFFFFFFF;
        *entryPtr = 0xFFFFFFFF;
        entryPtr[0x100] = 0;
        entryPtr = entryPtr + 1;
        remainingCount = remainingCount - 1;
    } while (remainingCount != 0);
EQ_GLOBAL(u32, 0x006E11E8) = 0X45;
EQ_GLOBAL(u32, 0x006E11EC) = 0X39;
EQ_GLOBAL(u32, 0x006E11F0) = 0X48;
EQ_GLOBAL(u32, 0x006E11F4) = 0X50;
EQ_GLOBAL(u32, 0x006E11F8) = 0X4D;
EQ_GLOBAL(u32, 0x006E11FC) = 0X4B;
EQ_GLOBAL(u32, 0x006E1200) = 0X58;
EQ_GLOBAL(u32, 0x006E15F0) = 0XC8;
EQ_GLOBAL(u32, 0x006E15F4) = 0XD0;
EQ_GLOBAL(u32, 0x006E15F8) = 0XCD;
EQ_GLOBAL(u32, 0x006E15FC) = 0XCB;
EQ_GLOBAL(u32, 0x006E1204) = 0X1E;
EQ_GLOBAL(u32, 0x006E1208) = 0X2E;
EQ_GLOBAL(u32, 0x006E120C) = 0X20;
EQ_GLOBAL(u32, 0x006E1210) = 0X23;
EQ_GLOBAL(u32, 0x006E1214) = 0X17;
EQ_GLOBAL(u32, 0x006E1218) = 0X14;
EQ_GLOBAL(u32, 0x006E121C) = 0X16;
EQ_GLOBAL(u32, 0x006E12E8) = 0X2000001E;
EQ_GLOBAL(u32, 0x006E12E4) = 0X20000030;
EQ_GLOBAL(u32, 0x006E12EC) = 0X2000002E;
EQ_GLOBAL(u32, 0x006E12F0) = 0X20000018;
EQ_GLOBAL(u32, 0x006E12F4) = 0X20000032;
EQ_GLOBAL(u32, 0x006E12F8) = 0X20000011;
EQ_GLOBAL(u32, 0x006E12FC) = 0X20000017;
EQ_GLOBAL(u32, 0x006E1300) = 0X20000020;
EQ_GLOBAL(u32, 0x006E1304) = 0X2000002D;
EQ_GLOBAL(u32, 0x006E1308) = 0X2000001F;
EQ_GLOBAL(u32, 0x006E130C) = 0X20000013;
EQ_GLOBAL(u32, 0x006E1310) = 0X1000000C;
EQ_GLOBAL(u32, 0x006E1314) = 0X1000000D;
EQ_GLOBAL(u32, 0x006E1340) = 0X28;
EQ_GLOBAL(u32, 0x006E1344) = 0X10000027;
EQ_GLOBAL(u32, 0x006E1220) = 0X49;
EQ_GLOBAL(u32, 0x006E1224) = 0X51;
EQ_GLOBAL(u32, 0x006E1228) = 0X4C;
EQ_GLOBAL(u32, 0x006E122C) = 0X47;
EQ_GLOBAL(u32, 0x006E1230) = 0X4F;
EQ_GLOBAL(u32, 0x006E1234) = 0X43;
EQ_GLOBAL(u32, 0x006E1238) = 0X44;
EQ_GLOBAL(u32, 0x006E1620) = 0XC9;
EQ_GLOBAL(u32, 0x006E1624) = 0XD1;
EQ_GLOBAL(u32, 0x006E1628) = 0XC7;
EQ_GLOBAL(u32, 0x006E162C) = 0XD2;
EQ_GLOBAL(u32, 0x006E1630) = 0XD3;
EQ_GLOBAL(u32, 0x006E136C) = 0X40000048;
EQ_GLOBAL(u32, 0x006E1370) = 0X40000050;
EQ_GLOBAL(u32, 0x006E1374) = 0X4000004D;
EQ_GLOBAL(u32, 0x006E1378) = 0X4000004B;
EQ_GLOBAL(u32, 0x006E137C) = 0X40000049;
EQ_GLOBAL(u32, 0x006E1380) = 0X40000051;
EQ_GLOBAL(u32, 0x006E176C) = 0X400000C8;
EQ_GLOBAL(u32, 0x006E1770) = 0X400000D0;
EQ_GLOBAL(u32, 0x006E1774) = 0X400000CD;
EQ_GLOBAL(u32, 0x006E1778) = 0X400000CB;
EQ_GLOBAL(u32, 0x006E177C) = 0X400000C9;
EQ_GLOBAL(u32, 0x006E1780) = 0X400000D1;
EQ_GLOBAL(u32, 0x006E123C) = 0X3B;
EQ_GLOBAL(u32, 0x006E1240) = 0X3C;
EQ_GLOBAL(u32, 0x006E1244) = 0X3D;
EQ_GLOBAL(u32, 0x006E1248) = 0X3E;
EQ_GLOBAL(u32, 0x006E124C) = 0X3F;
EQ_GLOBAL(u32, 0x006E1250) = 0X40;
EQ_GLOBAL(u32, 0x006E1254) = 0X41;
EQ_GLOBAL(u32, 0x006E1258) = 0X42;
EQ_GLOBAL(u32, 0x006E1260) = 0X2;
EQ_GLOBAL(u32, 0x006E1264) = 0X3;
EQ_GLOBAL(u32, 0x006E1268) = 0X4;
EQ_GLOBAL(u32, 0x006E126C) = 0X5;
EQ_GLOBAL(u32, 0x006E1270) = 0X6;
EQ_GLOBAL(u32, 0x006E1274) = 0X7;
EQ_GLOBAL(u32, 0x006E1278) = 0X8;
EQ_GLOBAL(u32, 0x006E127C) = 0X9;
EQ_GLOBAL(u32, 0x006E1280) = 0XA;
EQ_GLOBAL(u32, 0x006E1284) = 0XB;
EQ_GLOBAL(u32, 0x006E1288) = 0X10000002;
EQ_GLOBAL(u32, 0x006E128C) = 0X10000003;
EQ_GLOBAL(u32, 0x006E1290) = 0X10000004;
EQ_GLOBAL(u32, 0x006E1294) = 0X10000005;
EQ_GLOBAL(u32, 0x006E1298) = 0X10000006;
EQ_GLOBAL(u32, 0x006E129C) = 0X10000007;
EQ_GLOBAL(u32, 0x006E12A0) = 0X10000008;
EQ_GLOBAL(u32, 0x006E12A4) = 0X10000009;
EQ_GLOBAL(u32, 0x006E12A8) = 0X1000000A;
EQ_GLOBAL(u32, 0x006E12AC) = 0X1000000B;
EQ_GLOBAL(u32, 0x006E12B0) = 0X40000002;
EQ_GLOBAL(u32, 0x006E12B4) = 0X40000003;
EQ_GLOBAL(u32, 0x006E12B8) = 0X40000004;
EQ_GLOBAL(u32, 0x006E12BC) = 0X40000005;
EQ_GLOBAL(u32, 0x006E12C0) = 0X40000006;
EQ_GLOBAL(u32, 0x006E12C4) = 0X40000007;
EQ_GLOBAL(u32, 0x006E12C8) = 0X40000008;
EQ_GLOBAL(u32, 0x006E12CC) = 0X40000009;
EQ_GLOBAL(u32, 0x006E1320) = 0X10000048;
EQ_GLOBAL(u32, 0x006E1324) = 0X10000050;
EQ_GLOBAL(u32, 0x006E132C) = 0X1000004D;
EQ_GLOBAL(u32, 0x006E1328) = 0X1000004B;
EQ_GLOBAL(u32, 0x006E1720) = 0X100000C8;
EQ_GLOBAL(u32, 0x006E1724) = 0X100000D0;
EQ_GLOBAL(u32, 0x006E172C) = 0X100000CD;
EQ_GLOBAL(u32, 0x006E1728) = 0X100000CB;
EQ_GLOBAL(u32, 0x006E1384) = 0X100000D2;
EQ_GLOBAL(u32, 0x006E1388) = 0X100000D3;
EQ_GLOBAL(u32, 0x006E1330) = 0X10000047;
EQ_GLOBAL(u32, 0x006E1338) = 0X10000049;
EQ_GLOBAL(u32, 0x006E133C) = 0X10000051;
EQ_GLOBAL(u32, 0x006E1334) = 0X1000004F;
EQ_GLOBAL(u32, 0x006E1730) = 0X100000C7;
EQ_GLOBAL(u32, 0x006E1738) = 0X100000C9;
EQ_GLOBAL(u32, 0x006E173C) = 0X100000D1;
EQ_GLOBAL(u32, 0x006E1734) = 0X100000CF;
EQ_GLOBAL(u32, 0x006E12D0) = 0X13;
EQ_GLOBAL(u32, 0x006E12D4) = 0XF;
EQ_GLOBAL(u32, 0x006E12D8) = 0X1000000F;
EQ_GLOBAL(u32, 0x006E125C) = 0X57;
EQ_GLOBAL(u32, 0x006E12DC) = 0X29;
EQ_GLOBAL(u32, 0x006E131C) = 0X4A;
EQ_GLOBAL(u32, 0x006E1318) = 0X4E;
EQ_GLOBAL(u32, 0x006E12E0) = 0XF;
EQ_GLOBAL(u32, 0x006E1348) = 0X40000026;
EQ_GLOBAL(u32, 0x006E134C) = 0X40000015;
EQ_GLOBAL(u32, 0x006E1350) = 0X40000019;
EQ_GLOBAL(u32, 0x006E1354) = 0X40000014;
EQ_GLOBAL(u32, 0x006E1358) = 0X4000001F;
EQ_GLOBAL(u32, 0x006E135C) = 0X40000030;
EQ_GLOBAL(u32, 0x006E1360) = 0X40000023;
EQ_GLOBAL(u32, 0x006E1364) = 0X4000002E;
EQ_GLOBAL(u32, 0x006E1368) = 0X40000032;
}

// FUNCTION: TRILOGY 0x004eaf53
// Creates a Windows font for client text display.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
void * __cdecl MakeFont(s32 fontHeight, s32 fontWeight, const char *fontFace)
{
    void *pHVar1;
    typedef char *(__stdcall *LstrcpyA_fn)(char *, const char *);
    typedef void *(__stdcall *CreateFontIndirectA_fn)(const void *);

    memset((void *)0x006EE760, 0, 0x3c);
    EQ_GLOBAL(s32, 0x006EE760) = fontHeight;
    EQ_GLOBAL(s32, 0x006EE770) = fontWeight;
    EQ_GLOBAL(u8, 0x006EE77A) = 3;
    EQ_GLOBAL(u8, 0x006EE777) = 1;
    EQ_GLOBAL(LstrcpyA_fn, 0x0053419C)((char *)0x006EE77C, fontFace);
    pHVar1 = EQ_GLOBAL(CreateFontIndirectA_fn, 0x00534014)((void *)0x006EE760);
    if (pHVar1 == 0)
        DbgLog_guess((const char *)0x0057DD48, fontFace, fontHeight, fontWeight);
    return pHVar1;
}

// FUNCTION: TRILOGY 0x004eafbf
// Releases a text font used by client drawing when one exists.
  void DeleteFont_guess(u32 hobj)
  {
      if (hobj != 0)
          pDeleteObject_guess((void *)hobj);
  }

// FUNCTION: TRILOGY 0x004eafd1
// Builds the 128-character width table used to lay out client text.
// @early-stop: REP STOSD setup loads ECX before EDI.
// ORIGINAL FILE: EverQuest/Win.cpp (from Mac debug map)
int *GetChrWidths(void *hdc, void *font, int fallbackSize)
{
    struct ABC_guess
    {
        int abcA;
        u32 abcB;
        int abcC;
    };
    typedef void *(__stdcall *SelectObjectFn_guess)(void *, void *);
    typedef int (__stdcall *GetCharABCWidthsAFn_guess)(void *, u32, u32, ABC_guess *);
    typedef u32 (__stdcall *GetLastErrorFn_guess)(void);

    ABC_guess *lpABC;
    int *widthTable;
    void *prevObject;
    int BVar6;
    u32 DVar7;
    ABC_guess *pABC;
    int *widthCursor;
    int fillCount;
    int count;

    if (font == 0)
    {
        DbgLog_guess((const char *)0x0057de28);
        return (int *)0;
    }
    else
    {
        lpABC = (ABC_guess *)EQ_CDECL(void *, 0x00522FA3, (u32))(0xbf4);
        widthTable = (int *)EQ_CDECL(void *, 0x00522FA3, (u32))(0x200);
        if (widthTable == 0)
        {
            DbgLog_guess((const char *)0x0057ddf4);
            return (int *)0;
        }
        else
        {
            prevObject = EQ_GLOBAL(SelectObjectFn_guess, 0x00534024)(hdc, font);
            if ((prevObject == 0) || (prevObject == (void *)0xffffffff))
            {
                DbgLog_guess((const char *)0x0057dd88, prevObject);
                return (int *)0;
            }
            BVar6 = EQ_GLOBAL(GetCharABCWidthsAFn_guess, 0x0053400C)
                (hdc, 0, 0x7f, lpABC);
            DVar7 = EQ_GLOBAL(GetLastErrorFn_guess, 0x005341AC)();
            if ((BVar6 == 1) && (lpABC != 0))
            {
                pABC = lpABC;
                widthCursor = widthTable;
                count = 0x80;
                do
                {
                    *widthCursor = pABC->abcA + pABC->abcC + pABC->abcB;
                    pABC++;
                    widthCursor++;
                    count--;
                } while (count != 0);
                EQ_CDECL(void, 0x005230DD, (void *))(lpABC);
                return widthTable;
            }
            DbgLog_guess((const char *)0x0057ddbc, BVar6, lpABC, DVar7);
            count = (int)((double)fallbackSize * EQ_GLOBAL(double, 0x00535858));
            for (fillCount = 0; fillCount < 0x80; fillCount++)
                widthTable[fillCount] = count;
            EQ_CDECL(void, 0x005230DD, (void *))(lpABC);
            return widthTable;
        }
    }
}

  // FUNCTION: TRILOGY 0x004eb0b1
// ORIGINAL FILE: Source/PC Source/EverQuest/Win.cpp (from Mac debug map)
u32 FlushKey(void)
{
    u8 buf[512];
    u32 count = 0x20;
    void *o = g_keyboard_device_guess;

    ((WinGetDataFn_guess)(*(void ***)o)[10])(o, 0x10, buf, &count, 0);
    return count;
}

// FUNCTION: TRILOGY 0x004eb0e0
// Collects keyboard events and sends presses and releases to client controls.
u32 GetKey(void)
{
    typedef int (__stdcall *GetDeviceDataFn_guess)(void *, u32, void *, u32 *, u32);
    typedef int (__stdcall *AcquireFn_guess)(void *);
    u8 event_buffer[512];
    u32 event_count = 0x20;
    u32 event_index;
    u8 *event;
    int result;
    void *device;

    device = g_keyboard_device_guess;
    result = ((GetDeviceDataFn_guess)(*(void ***)device)[10])(
        device, 0x10, event_buffer, &event_count, 0);
    if (result != 0)
    {
        device = g_keyboard_device_guess;
        result = ((AcquireFn_guess)(*(void ***)device)[7])(device);
        if (result < 0)
            return 1;
    }

    event_index = 0;
    if (event_count > 0)
    {
        event = event_buffer;
        do
        {
            if (event[4] & 0x80)
            {
                switch (*(u32 *)event)
                {
                case 0x1d:
                    EQ_GLOBAL(int, 0x006EFCB4) = 1;
                    break;
                case 0x2a:
                    EQ_GLOBAL(int, 0x006EFCB0) = 1;
                    break;
                case 0x36:
                    EQ_GLOBAL(int, 0x006EFCB0) = 1;
                    *(u32 *)event = 0x2a;
                    break;
                case 0x38:
                    EQ_GLOBAL(int, 0x006EFCC0) = 1;
                    break;
                case 0x9d:
                    EQ_GLOBAL(int, 0x006EFCB4) = 1;
                    *(u32 *)event = 0x1d;
                    break;
                case 0xb8:
                    EQ_GLOBAL(int, 0x006EFCC0) = 2;
                    *(u32 *)event = 0x38;
                    break;
                }

                EQ_GLOBAL(int, 0x006B70E8) = EQ_GLOBAL(int, 0x006EFCB0);
                EQ_GLOBAL(int, 0x006B70EC) = EQ_GLOBAL(int, 0x006EFCB4);
                EQ_GLOBAL(int, 0x006B70F0) = EQ_GLOBAL(int, 0x006EFCC0);
                EQ_GLOBAL(int, 0x006B70F8) = EQ_GLOBAL(int, 0x006B70F4);
                EQ_GLOBAL(int, 0x006B70F4) = *(u32 *)event;
                EQ_CDECL(void, 0x004C22BA, (int))(*(u32 *)event);
            }
            else
            {
                switch (*(u32 *)event)
                {
                case 0x3a:
                    EQ_GLOBAL(int, 0x006EFCB8) =
                        (EQ_GLOBAL(int, 0x006EFCB8) != 1);
                    break;
                case 0x1d:
                    EQ_GLOBAL(int, 0x006EFCB4) = 0;
                    break;
                case 0x2a:
                    EQ_GLOBAL(int, 0x006EFCB0) = 0;
                    break;
                case 0x36:
                    EQ_GLOBAL(int, 0x006EFCB0) = 0;
                    *(u32 *)event = 0x2a;
                    break;
                case 0x38:
                    EQ_GLOBAL(int, 0x006EFCC0) = 0;
                    break;
                case 0x45:
                    EQ_GLOBAL(int, 0x006EFCBC) =
                        (EQ_GLOBAL(int, 0x006EFCBC) != 1);
                    break;
                case 0x9d:
                    EQ_GLOBAL(int, 0x006EFCB4) = 0;
                    *(u32 *)event = 0x1d;
                    break;
                case 0xb8:
                    EQ_GLOBAL(int, 0x006EFCC0) = 0;
                    *(u32 *)event = 0x38;
                    break;
                }

                EQ_GLOBAL(int, 0x006B70E8) = EQ_GLOBAL(int, 0x006EFCB0);
                EQ_GLOBAL(int, 0x006B70EC) = EQ_GLOBAL(int, 0x006EFCB4);
                EQ_GLOBAL(int, 0x006B70F0) = EQ_GLOBAL(int, 0x006EFCC0);
                EQ_CDECL(void, 0x004C18ED, (int))(*(u32 *)event);
            }
            event_index++;
            event += 0x10;
        } while (event_index < event_count);
    }
    return event_count;
}

// FUNCTION: TRILOGY 0x004eb2b1
// Steps the text display's scroll index by a wheel delta (delta/120), clamps it and updates its view rect.
// @early-stop: similarity 0.6847; the index and old-value registers are reversed, and the original's redundant TEST EAX,EAX remains unmatched.
void ScrollDisplayPosition_guess(int scrollDelta)
{
    int old_index;
    int current_index;
    int upper_index;
    int upper_value;
    int step;
    int height;
    char *display;
    typedef int (__stdcall *CopyRectFn_guess)(void *, void *);

    upper_index = 0x78;
    current_index = EQ_GLOBAL(s32, 0x006D6FDC);
    step = scrollDelta / upper_index;
    old_index = current_index;
    current_index += step;
    upper_value = (int)EQ_GLOBAL(s16, 0x006E0FEA);
    // Preserve the original's tentative global write before the clamp.
    *(volatile int *)0x006D6FDC = current_index;
    upper_index = upper_value - 1;
    if (current_index > upper_index)
        current_index = upper_index;
    else if (current_index < 0)
        current_index = 0;
    *(volatile int *)0x006D6FDC = current_index;
    if (old_index == current_index)
        return;
    display = (char *)EQ_GLOBAL(void *, 0x006E4DE8);
    if (display == 0)
        return;
    if (upper_index != 0)
        height = (*(s16 *)(display + 0x17A) * current_index) / upper_index;
    else
        height = 0;
    *(int *)(*(int *)(display + 0x88) + 4) = *(s16 *)(display + 0x178) - height;
    *(int *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DE8) + 0x88) + 0xC) = (int)pinstEQ_TextIO_guess->scrollThumbHeight_guess + *(int *)(*(int *)((char *)EQ_GLOBAL(void *, 0x006E4DE8) + 0x88) + 4);
    EQ_GLOBAL(CopyRectFn_guess, 0x00534240)((char *)EQ_GLOBAL(void *, 0x006E4DE8) + 0xCC, *(void **)((char *)EQ_GLOBAL(void *, 0x006E4DE8) + 0x88));
    EQ_METHOD(0, void, 0x0048CA78, (void *), EQ_GLOBAL(void *, 0x006E4DE8))();
    *(u32 *)((char *)EQ_GLOBAL(void *, 0x006E4DE8) + 0x1EC) = 1;
    return;
}

// FUNCTION: TRILOGY 0x004eb36e
// Updates pointer movement and button state from mouse input, including wheel scrolling.
u32 PollMouse_guess(void)
{
    u8 buf[2048];
    u32 count = 0x80;
    void *o = g_mouse_device_guess;
    int hr;
    u32 i;
    u8 *p;
    int x;
    int y;
    int t;
    int limit;

    g_mouse_state0_guess = 0;
    g_mouse_state1_guess = 0;
    hr = ((WinGetDataFn_guess)(*(void ***)o)[10])(o, 0x10, buf, &count, 0);
    if (hr < 0 &&
        ((hr = ((WinReacquireFn_guess)(*(void ***)g_mouse_device_guess)[7])(g_mouse_device_guess)) < 0 ||
         (hr = ((WinGetDataFn_guess)(*(void ***)g_mouse_device_guess)[10])(g_mouse_device_guess, 0x10, buf, &count, 0)) < 0))
        return 7;
    i = 0;
    if (count > 0)
    {
        p = buf + 4;
        do
        {
            switch (*(int *)(p - 4))
            {
            case 0xd:
                EQ_GLOBAL(u8, 0x006B648A) = 0;
                if (*p & 0x80)
                    EQ_GLOBAL(u8, 0x006B648A) = 1;
                break;
            case 0xc:
                EQ_GLOBAL(u8, 0x006B648B) = 0;
                if (*p & 0x80)
                    EQ_GLOBAL(u8, 0x006B648B) = 1;
                break;
            case 8:
                if (EQ_GLOBAL(int, 0x006EE748) != 0 && *(int *)((char *)EQ_GLOBAL(void *, 0x006EFE2C) + 4) == 5)
                    EQ_CDECL(void, 0x004EB2B1, (int))(*(int *)p);
                break;
            case 4:
                EQ_GLOBAL(int, 0x006EFC8C) += *(int *)p;
                break;
            case 0:
                EQ_GLOBAL(int, 0x006EFC88) += *(int *)p;
                break;
            }
            i++;
            p += 0x10;
        } while (i < count);
    }
    x = EQ_GLOBAL(int, 0x006EFC88);
    y = EQ_GLOBAL(int, 0x006EFC8C);
    if (x != 0 || y != 0)
    {
        t = EQ_GLOBAL(int, 0x006B6420);
        if (t > 1)
        {
            EQ_GLOBAL(int, 0x006EFC80) += t * x;
            EQ_GLOBAL(int, 0x006EFC84) += t * y;
        }
        else
        {
            EQ_GLOBAL(int, 0x006EFC80) += x;
            EQ_GLOBAL(int, 0x006EFC84) += y;
        }
        if (EQ_GLOBAL(int, 0x006EFC80) < 0)
            EQ_GLOBAL(int, 0x006EFC80) = 0;
        if (EQ_GLOBAL(int, 0x006EFC84) < 0)
            EQ_GLOBAL(int, 0x006EFC84) = 0;
        if (g_game_state_guess == 2)
        {
            limit = EQ_GLOBAL(int, 0x006B63FC) - 1;
            if (EQ_GLOBAL(int, 0x006EFC80) > limit)
                EQ_GLOBAL(int, 0x006EFC80) = limit;
            limit = EQ_GLOBAL(int, 0x006B6400) - 1;
        }
        else
        {
            limit = 0x274;
            if (EQ_GLOBAL(int, 0x006EFC80) > limit)
                EQ_GLOBAL(int, 0x006EFC80) = limit;
            limit = 0x1cb;
        }
        if (EQ_GLOBAL(int, 0x006EFC84) > limit)
            EQ_GLOBAL(int, 0x006EFC84) = limit;
    }
    return count;
}

// FUNCTION: TRILOGY 0x004eb516
// Discards pending mouse events and clears accumulated mouse movement.
// ORIGINAL FILE: Source/PC Source/EverQuest/Win.cpp (from Mac debug map)
u32 FlushMouse(void)
{
    u8 buf[2048];
    u32 count = 0x80;
    void *o = g_mouse_device_guess;
    int hr;

    g_mouse_state0_guess = 0;
    g_mouse_state1_guess = 0;
    hr = ((WinGetDataFn_guess)(*(void ***)o)[10])(o, 0x10, buf, &count, 0);
    if (hr < 0)
    {
        void *o2 = g_mouse_device_guess;

        hr = ((WinReacquireFn_guess)(*(void ***)o2)[7])(o2);
        if (hr < 0)
            return 7;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004eb569
// Initializes the game's mouse input for play.
u32 initializeDirectInputMouse_guess(void *window, void *instance)
{
    typedef int (__stdcall *CreateDeviceFn_guess)(void *, const void *, void **, void *);
    typedef int (__stdcall *GetCapabilitiesFn_guess)(void *, void *);
    typedef int (__stdcall *GetDeviceInfoFn_guess)(void *, void *);
    typedef int (__stdcall *SetCooperativeLevelFn_guess)(void *, void *, u32);
    typedef int (__stdcall *SetPropertyFn_guess)(void *, u32, void *);
    typedef int (__stdcall *AcquireFn_guess)(void *);
    u8 capabilities[0x2c];
    int hr;
    void *obj;

    hr = EQ_STDCALL(int, 0x00500404, (void *, u32, void **, void *))(
        instance, 0x500, &g_mouse_input_interface_guess, 0);
    if (hr < 0)
        return 1;
    obj = g_mouse_input_interface_guess;
    hr = ((CreateDeviceFn_guess)(*(void ***)obj)[3])(
        obj, (const void *)0x00536EE8, &g_mouse_device_guess, 0);
    if (hr < 0)
        return 2;
    obj = g_mouse_device_guess;
    *(u32 *)capabilities = 0x2c;
    ((GetCapabilitiesFn_guess)(*(void ***)obj)[3])(obj, capabilities);
    if ((capabilities[4] & 1) == 0)
        EQ_GLOBAL(u32, 0x006EE748) = 0;
    else
    {
        EQ_GLOBAL(u32, 0x006EE748) = 1;
        if (*(u32 *)(capabilities + 0x0c) <= 2)
            EQ_GLOBAL(u32, 0x006EE748) = 0;
    }

    obj = g_mouse_device_guess;
    hr = ((GetDeviceInfoFn_guess)(*(void ***)obj)[11])(
        obj, (void *)0x00536E70);
    if (hr < 0)
        return 4;
    obj = g_mouse_device_guess;
    hr = ((SetCooperativeLevelFn_guess)(*(void ***)obj)[13])(
        obj, window, 5);
    if (hr < 0)
        return 5;
    memset(
        (void *)0x006EF9E8, 0, 0x14);
    EQ_GLOBAL(u32, 0x006EF9E8) = 0x14;
    EQ_GLOBAL(u32, 0x006EF9EC) = 0x10;
    EQ_GLOBAL(u32, 0x006EF9F0) = 0;
    EQ_GLOBAL(u32, 0x006EF9F4) = 0;
    EQ_GLOBAL(u32, 0x006EF9F8) = 0x80;

    obj = g_mouse_device_guess;
    hr = ((SetPropertyFn_guess)(*(void ***)obj)[6])(
        obj, 1, (void *)0x006EF9E8);
    if (hr < 0)
        return 6;
    obj = g_mouse_device_guess;
    hr = ((AcquireFn_guess)(*(void ***)obj)[7])(obj);
    if (hr < 0)
        return 7;
    EQ_GLOBAL(u32, 0x0057D0C0) = 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004eb6ff
// Shuts down the game's mouse input.
void ReleaseMouseInput_guess(void)
{
    void *o1;

    o1 = g_mouse_device_guess;
    if (o1 != 0)
    {
        ((WinUnacquireFn_guess)(*(void ***)o1)[8])(o1);
        o1 = g_mouse_device_guess;
        ((WinReleaseFn_guess)(*(void ***)o1)[2])(o1);
        g_mouse_device_guess = 0;
    }

    o1 = g_mouse_input_interface_guess;
    if (o1 != 0)
    {
        ((WinReleaseFn_guess)(*(void ***)o1)[2])(o1);
        g_mouse_input_interface_guess = 0;
    }
}

// FUNCTION: TRILOGY 0x004eb8b4
// Shuts down the game's keyboard input.
void ReleaseKeyboardInput_guess(void)
{
    void *o1;

    o1 = g_keyboard_device_guess;
    if (o1 != 0)
    {
        ((WinUnacquireFn_guess)(*(void ***)o1)[8])(o1);
        o1 = g_keyboard_device_guess;
        ((WinReleaseFn_guess)(*(void ***)o1)[2])(o1);
        g_keyboard_device_guess = 0;
    }

    o1 = g_keyboard_input_interface_guess;
    if (o1 != 0)
    {
        ((WinReleaseFn_guess)(*(void ***)o1)[2])(o1);
        g_keyboard_input_interface_guess = 0;
    }
}
