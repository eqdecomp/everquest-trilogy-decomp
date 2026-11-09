/*
 * eqplayer.h - the globals and not-yet-rewritten callees only EQPlayer.cpp (Mac: Source/PC Source/EverQuest/
 * EQPlayer.cpp) uses. The EQPlayer methods themselves are declared in player.h.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQPLAYER_H
#define TRILOGY_EQPLAYER_H

#include "common.h"
#include "eqaddr.h"
#include "player.h"
#include "globals.h"

/* g_player_top_guess (EQPlayer::top, 0x006E4D58) is in globals.h. */

/* 0x004238A2  EQ_Character::TotalSpellAffects (Mac), __thiscall: sum of one spell effect over the buffs. */
#define EQ_Character_TotalSpellAffects_guess(ch, effect)     EQ_METHOD(1, s16, 0x004238A2, (void *, u8), (void *)(ch))((effect))

#endif /* TRILOGY_EQPLAYER_H */
