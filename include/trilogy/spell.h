/*
 * spell.h - a spell record as the client keeps it (the spell table g_spells_guess, 0x005FE9A0, holds a
 * pointer per spell id 0..2999).
 *
 * Offsets from EQClassic SPDat_Spell_Struct (Zone/Include/spdat.h, 608 bytes); the fields marked * were
 * checked against the client code named; the rest come from a layout JSON (not published)
 * (0x260 bytes, gen_layout_h --extend). The message strings are declared as their first char: use &field.
 */
#ifndef TRILOGY_SPELL_H
#define TRILOGY_SPELL_H

#include "common.h"

#pragma pack(push, 1)

#define EQ_SPELL_EFFECTS        12      /* effect slots per spell */
#define EQ_SPELL_EFFECT_NONE    0xFE    /* an unused effect slot */
#define EQ_MAX_SPELLS           3000    /* g_spells_guess entries; ids 0..2999 */

typedef struct EQ_Spell_guess
{
    char    name_guess;         /* 0x000* name, messages, ranges, cast/recovery/recast times */
    u8      unknown0x001[0x1F]; /* 0x001  */
    char    text0x20_guess;     /* 0x020* */
    u8      unknown0x021[0x1F]; /* 0x021  */
    char    teleport_zone_guess; /* 0x040* */
    u8      unknown0x041[0x1F]; /* 0x041  */
    char    castOnSelfMessage_guess; /* 0x060* */
    u8      unknown0x061[0x3F]; /* 0x061  */
    char    castOnOtherMessage_guess; /* 0x0A0* */
    u8      unknown0x0A1[0x3F]; /* 0x0A1  */
    char    selfEffectMessage_guess; /* 0x0E0* */
    u8      unknown0x0E1[0x3F]; /* 0x0E1  */
    char    otherEffectMessage_guess; /* 0x120* */
    u8      unknown0x121[0x3F]; /* 0x121  */
    char    wearOffMessage_guess; /* 0x160* */
    u8      unknown0x161[0x3F]; /* 0x161  */
    union
    {
        float   range_guess;        /* 0x1A0* */
        int     range_bits_guess;   /* 0x1A0* */
    };
    float   aoe_range_guess;    /* 0x1A4* */
    union
    {
        float   push_back_guess;    /* 0x1A8* */
        int     push_back_bits_guess; /* 0x1A8* */
    };
    union
    {
        float   push_up_guess;      /* 0x1AC* */
        int     push_up_bits_guess; /* 0x1AC* */
    };
    u32     castTime_guess;     /* 0x1B0* */
    u8      unknown0x1B4[4];    /* 0x1B4  */
    u32     recastTime_guess;   /* 0x1B8* */
    u32     duration_formula;   /* 0x1BC* EQ_Character::SpellDuration (0x00424591): 1..12, 50 = permanent */
    u32     duration;           /* 0x1C0* cap of the formula's result when nonzero */
    u32     impact_duration_guess; /* 0x1C4* AE duration, mana */
    u16     mana_guess;         /* 0x1C8* */
    s16     base[12];           /* 0x1CA* IsSpellEffectValid (0x0042001B) */
    s16     maximumEffectValue_guess[0xC]; /* 0x1E2* max, icons, components, formulas, ... */
    u16     spellIcon_guess;    /* 0x1FA* */
    u16     memorizedIcon_guess; /* 0x1FC* */
    u16     components_guess[4]; /* 0x1FE* */
    short   componentCounts_guess[4]; /* 0x206* */
    u16     requiredItems_guess[2]; /* 0x20E* */
    u8      effectFormula_guess[0xC]; /* 0x212* */
    u8      unknown0x21E[1];    /* 0x21E  */
    u8      good_effect_guess;  /* 0x21F* */
    u8      activated_guess;    /* 0x220* */
    u8      resist_type_guess;  /* 0x221* */
    u8      effect_id[12];      /* 0x222* EQ_SPELL_EFFECT_NONE = unused (GetPCSpellAffect 0x0042381B) */
    union
    {
        u8      target_type_guess;  /* 0x22E* target type, base difficulty, skill, ... */
        char    target_type_signed_guess; /* 0x22E* */
    };
    u8      baseDifficulty_guess; /* 0x22F* */
    u8      castingSkill_guess; /* 0x230* */
    u8      unknown0x231[4];    /* 0x231  */
    u8      class_level[15];    /* 0x235* level per class 1..15; [7] = bard (BardCastBard 0x0041F0C0) */
    u8      unknown0x244[0x10]; /* 0x244  */
    u8      spellAnimation_guess; /* 0x254* */
    u8      unknown0x255[3];    /* 0x255  */
    void*   effectData;   /* 0x258* */
    s32     effectIndex_guess;  /* 0x25C* */

#ifdef __cplusplus
    /* EQ_Spell (Mac name; eq_spell.h typedefs it to this struct) */
    int SpellUsesDragonBreathEffect();                    /* 0x004875F6 */
    int LoadSpellData_guess(short spellIndex);                  /* 0x0048760D */
    static int InitializeEffectTable_guess(void);               /* 0x0048A13E */
#endif
} EQ_Spell_guess;               /* 0x260 */
C_ASSERT(sizeof(EQ_Spell_guess) == 0x260);
C_ASSERT(offsetof(EQ_Spell_guess, duration_formula) == 0x1BC);
C_ASSERT(offsetof(EQ_Spell_guess, duration) == 0x1C0);
C_ASSERT(offsetof(EQ_Spell_guess, base) == 0x1CA);
C_ASSERT(offsetof(EQ_Spell_guess, effect_id) == 0x222);
C_ASSERT(offsetof(EQ_Spell_guess, class_level) == 0x235);
C_ASSERT(offsetof(EQ_Spell_guess, name_guess) == 0x000);
C_ASSERT(offsetof(EQ_Spell_guess, text0x20_guess) == 0x020);
C_ASSERT(offsetof(EQ_Spell_guess, teleport_zone_guess) == 0x040);
C_ASSERT(offsetof(EQ_Spell_guess, castOnSelfMessage_guess) == 0x060);
C_ASSERT(offsetof(EQ_Spell_guess, castOnOtherMessage_guess) == 0x0A0);
C_ASSERT(offsetof(EQ_Spell_guess, selfEffectMessage_guess) == 0x0E0);
C_ASSERT(offsetof(EQ_Spell_guess, otherEffectMessage_guess) == 0x120);
C_ASSERT(offsetof(EQ_Spell_guess, wearOffMessage_guess) == 0x160);
C_ASSERT(offsetof(EQ_Spell_guess, range_guess) == 0x1A0);
C_ASSERT(offsetof(EQ_Spell_guess, aoe_range_guess) == 0x1A4);
C_ASSERT(offsetof(EQ_Spell_guess, push_back_guess) == 0x1A8);
C_ASSERT(offsetof(EQ_Spell_guess, push_up_guess) == 0x1AC);
C_ASSERT(offsetof(EQ_Spell_guess, castTime_guess) == 0x1B0);
C_ASSERT(offsetof(EQ_Spell_guess, recastTime_guess) == 0x1B8);
C_ASSERT(offsetof(EQ_Spell_guess, impact_duration_guess) == 0x1C4);
C_ASSERT(offsetof(EQ_Spell_guess, mana_guess) == 0x1C8);
C_ASSERT(offsetof(EQ_Spell_guess, maximumEffectValue_guess) == 0x1E2);
C_ASSERT(offsetof(EQ_Spell_guess, spellIcon_guess) == 0x1FA);
C_ASSERT(offsetof(EQ_Spell_guess, memorizedIcon_guess) == 0x1FC);
C_ASSERT(offsetof(EQ_Spell_guess, components_guess) == 0x1FE);
C_ASSERT(offsetof(EQ_Spell_guess, componentCounts_guess) == 0x206);
C_ASSERT(offsetof(EQ_Spell_guess, requiredItems_guess) == 0x20E);
C_ASSERT(offsetof(EQ_Spell_guess, effectFormula_guess) == 0x212);
C_ASSERT(offsetof(EQ_Spell_guess, good_effect_guess) == 0x21F);
C_ASSERT(offsetof(EQ_Spell_guess, activated_guess) == 0x220);
C_ASSERT(offsetof(EQ_Spell_guess, resist_type_guess) == 0x221);
C_ASSERT(offsetof(EQ_Spell_guess, target_type_guess) == 0x22E);
C_ASSERT(offsetof(EQ_Spell_guess, baseDifficulty_guess) == 0x22F);
C_ASSERT(offsetof(EQ_Spell_guess, castingSkill_guess) == 0x230);
C_ASSERT(offsetof(EQ_Spell_guess, spellAnimation_guess) == 0x254);
C_ASSERT(offsetof(EQ_Spell_guess, effectData) == 0x258);
C_ASSERT(offsetof(EQ_Spell_guess, effectIndex_guess) == 0x25C);

#pragma pack(pop)

#endif /* TRILOGY_SPELL_H */
