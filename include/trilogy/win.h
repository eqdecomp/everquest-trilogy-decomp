/* win.h - Windows startup/voice dispatch helpers (EverQuest/Win).
 * Layout unrecovered; raw offsets only.
 * C++ only (VC6).
 */
#ifndef TRILOGY_WIN_H
#define TRILOGY_WIN_H

#include "common.h"
#include "eqaddr.h"

/* 0x006ECE3C music playlist count (full dword getter target). */
#define g_music_playlist_count_guess EQ_GLOBAL(u32, 0x006ECE3C)

/* 0x006ECE54 dispatch object pointer (raw, layout unrecovered). */
#define g_voice_dispatch_guess EQ_GLOBAL(void *, 0x006ECE54)

/* COM-like stdcall slots, explicit object first arg. Indices: 0xC=3, 0x10=4, 0x18=6, 0x20=8. */
typedef int (__stdcall *WinLockFn_guess)(void *obj);
typedef int (__stdcall *WinUnlockFn_guess)(void *obj);
typedef int (__stdcall *WinOp4Fn_guess)(void *obj, u32 a1, u32 a2, u32 a3, u32 a4);
typedef int (__stdcall *WinOp3Fn_guess)(void *obj, u32 a1, u32 a2, u32 a3);

/* 0x006EFC74 keyboard device object pointer. */
#define g_keyboard_device_guess EQ_GLOBAL(void *, 0x006EFC74)

/* 0x006EFC7C mouse device object pointer; 0x006EFC88/8C cleared dwords. */
#define g_mouse_device_guess EQ_GLOBAL(void *, 0x006EFC7C)
#define g_mouse_state0_guess EQ_GLOBAL(u32, 0x006EFC88)
#define g_mouse_state1_guess EQ_GLOBAL(u32, 0x006EFC8C)

/* Inferred DirectInput parent pointers, released by the Win startup helpers. */
#define g_mouse_input_interface_guess EQ_GLOBAL(void *, 0x006EFC78)
#define g_keyboard_input_interface_guess EQ_GLOBAL(void *, 0x006EFA38)

/* Reacquire-like slot 0x1C=7: (obj). */
typedef int (__stdcall *WinReacquireFn_guess)(void *obj);

/* DirectInput device Unacquire at slot 0x20=8 and IUnknown Release at slot 0x8=2. */
typedef int (__stdcall *WinUnacquireFn_guess)(void *obj);
typedef u32 (__stdcall *WinReleaseFn_guess)(void *obj);

/* DirectInput GetDeviceData-like slot 0x28=10: (obj, u32, buf, u32*, u32). */
typedef int (__stdcall *WinGetDataFn_guess)(void *obj, u32 a1, void *buf, u32 *pcount, u32 a5);

/* GDI32 DeleteObject import slot (client calls call [0x00534010]). */
typedef int (__stdcall *DeleteObjectFn_guess)(void *hobj);
#define pDeleteObject_guess EQ_GLOBAL(DeleteObjectFn_guess, 0x00534010)

u32 StreamManager_GetMusicPlaylistCount_guess(void); /* 0x004E7CF3 */
int processVoiceRecognitionCommand_guess(u32 a1, u32 a2, u32 a3, u32 a4); /* 0x004E7CFF */
int clearClipboardData_guess(u32 a1, u32 a2, u32 a3); /* 0x004E7D3B */
void initializeClientValidationString_guess(void); /* 0x004E87A8 */
void default_key_bindings(void); /* 0x004EAA05 */
void DeleteFont_guess(u32 hobj); /* 0x004EAFBF */
u32 FlushKey(void); /* 0x004EB0B1 */
u32 FlushMouse(void); /* 0x004EB516 */
void ReleaseMouseInput_guess(void); /* 0x004EB6FF */
void ReleaseKeyboardInput_guess(void); /* 0x004EB8B4 */

#endif
