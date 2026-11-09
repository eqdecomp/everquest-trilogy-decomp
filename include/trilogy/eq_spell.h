/*
 * eq_spell.h - EQ_Spell, the client's spell object (Mac: Source/PC Source/EverQuest/EQ_Spell.cpp).
 *
 * The spell record and its methods are one class, EQ_Spell_guess in spell.h (0x260 bytes); EQ_Spell is
 * its Mac name, kept here as a typedef.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQ_SPELL_H
#define TRILOGY_EQ_SPELL_H

#include "common.h"
#include "spell.h"

typedef EQ_Spell_guess EQ_Spell;

int __cdecl SpellPairOverridesStacking_guess(short spellId1, short spellId2); /* 0x00422ACF */
int __cdecl GetEffFiles_guess(void); /* 0x0048A06B */
int __cdecl WriteSpellEffects(void); /* 0x0048A0C2 */

#endif /* TRILOGY_EQ_SPELL_H */
