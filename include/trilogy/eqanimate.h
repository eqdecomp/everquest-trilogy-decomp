/*
 * eqanimate.h - the animation code of CDisplay (Mac: Source/PC Source/EverQuest/EQAnimate.cpp) and the
 * globals only EQAnimate.cpp uses.
 *
 * The CDisplay methods of EQAnimate.cpp are declared in display.h, the eqgfx_dx7.dll entry points in eqgfx.h.
 *
 * Calling conventions: callers of the methods load ECX (the CDisplay); most of them never read it and end
 * in `ret N`, i.e. __thiscall methods that ignore `this`.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQANIMATE_H
#define TRILOGY_EQANIMATE_H

#include "common.h"
#include "eqaddr.h"
#include "player.h"
#include "display.h"
#include "eqgfx.h"

/* --- globals ---------------------------------------------------------------------------------------- */

/* 0x006D6FE0  int; 1 makes 0x2B (not 0x20) the fallback animation of getFallbackAnimationId. */
#define g_useAltFallbackAnim_guess                EQ_GLOBAL(int, 0x006D6FE0)

/* 0x0048EBB3  __stdcall. */
char __stdcall toggleAnimationState_guess(int anim, EQPlayer* player, char state);

/* 0x00490574  the 0x1222-byte object loadSpriteTrack (0x004A01F9) allocates: zeroed by its constructor. */
class SpriteTrackBlock_guess
{
public:
    u32     data[0x488];        /* 0x0000 */
    u8      flag_a;             /* 0x1220 */
    u8      flag_b;             /* 0x1221 */

    SpriteTrackBlock_guess();                                                       /* 0x00490574 */
};
C_ASSERT(offsetof(SpriteTrackBlock_guess, flag_b) == 0x1221);

#endif /* TRILOGY_EQANIMATE_H */
