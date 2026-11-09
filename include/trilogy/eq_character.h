/*
 * eq_character.h - EQ_Character and EQ_PC, the local player's character object (Mac:
 * Source/PC Source/EverQuest/EQ_Character.cpp, EQ_Character_MagicSystem.cpp, EQ_PC.cpp), plus the
 * globals only these files use.
 *
 * Layout: the object starts with the 0x1FA4-byte CharacterProfile (character.h); EQ_Character's methods
 * read the profile's offsets from `this`, and EQ_PC::EQ_PC (0x0047EB8E) calls EQ_Character::EQ_Character
 * on the same `this` before filling the profile. No vtable (offset 0 is the name). EQ_PC adds no members:
 * every allocation is 0x1FA4 bytes (EverQuest.cpp NewCharacter_guess / ProfileAllocation_guess).
 *
 * Calling conventions: every caller loads ECX, so the methods are __thiscall, including those that
 * never read `this` and end in `ret N` (the CanWorship tables, ClassMin, ...). Functions that neither
 * read ECX nor pop their arguments are static members (__cdecl).
 *
 * C++ only (VC6). Not included from trilogy.h, which build_gdt.py parses as C.
 */
#ifndef TRILOGY_EQ_CHARACTER_H
#define TRILOGY_EQ_CHARACTER_H

#include "common.h"
#include "eqaddr.h"
#include "item.h"
#include "player.h"
#include "character.h"
#include "spell.h"

/* --- globals of EQ_Character.cpp ------------------------------------------------------------------ */

/* 0x00591CA8  20 doubles, 0.1 .. 2.0, filled once by EQ_Character::EQ_Character; 0x00591D88 = filled. */
#define g_tenths_table_guess        EQ_ARRAY(double, 0x00591CA8)
#define g_tenths_table_ready_guess  EQ_GLOBAL(int, 0x00591D88)
/* 0x006B7108  int read by the CanWorship tables at character creation (4 forbids row 0 and some rows,
 * 2 makes half elves accept only row 14). Meaning unknown. */
#define g_pvpRulesetCopy_guess                EQ_GLOBAL(int, 0x006B7108)

/* Character creation tables (.data). Rows of 14 or 15 bytes, indexed [row][col] by the CanWorship
 * methods; their meaning (class x deity?) is not checked. */
typedef bool CharTableRow14_guess[14];
typedef bool CharTableRow15_guess[15];
#define g_human_worship_guess       EQ_ARRAY(CharTableRow14_guess, 0x0053F3E8)
#define g_barbarian_worship_guess   EQ_ARRAY(CharTableRow15_guess, 0x0053F4D8)
#define g_erudite_worship_guess     EQ_ARRAY(CharTableRow14_guess, 0x0053F5D8)
#define g_elf_worship_guess         EQ_ARRAY(CharTableRow14_guess, 0x0053F6C8)
#define g_highelf_worship_guess     EQ_ARRAY(CharTableRow14_guess, 0x0053F7B8)
#define g_darkelf_worship_guess     EQ_ARRAY(CharTableRow14_guess, 0x0053F8A8)
#define g_halfelf_worship_guess     EQ_ARRAY(CharTableRow14_guess, 0x0053F998)
#define g_dwarf_worship_guess       EQ_ARRAY(CharTableRow14_guess, 0x0053FA88)
#define g_troll_worship_guess       EQ_ARRAY(CharTableRow15_guess, 0x0053FB78)
#define g_ogre_worship_guess        EQ_ARRAY(CharTableRow15_guess, 0x0053FC78)
#define g_halfling_worship_guess    EQ_ARRAY(CharTableRow14_guess, 0x0053FD78)
#define g_gnome_worship_guess       EQ_ARRAY(CharTableRow14_guess, 0x0053FE68)
#define g_iksar_worship_guess       EQ_ARRAY(CharTableRow15_guess, 0x0053FF58)
/* 0x0053F240  shorts, [row][15]; EQ_Character::CreationTableValue_guess returns the low byte. */
typedef s16 CharTableShortRow15_guess[15];
#define g_creation_values_guess     EQ_ARRAY(CharTableShortRow15_guess, 0x0053F240)
/* 0x005400E8 / 0x0054012C  dword tables indexed by a byte (MinAlign / MaxAlign). */
#define g_min_align_guess           EQ_ARRAY(s32, 0x005400E8)
#define g_max_align_guess           EQ_ARRAY(s32, 0x0054012C)
/* 0x00540170  [race][7] bytes, ClassMin (race 127 -> row 12, 129 -> row 13). */
typedef u8 CharTableRow7_guess[7];
#define g_class_min_guess           EQ_ARRAY(CharTableRow7_guess, 0x00540170)

/* 0x00540058  [row][12] bools, CityCanStart. */
typedef bool CharTableRow12_guess[12];
#define g_city_start_guess          EQ_ARRAY(CharTableRow12_guess, 0x00540058)

/* 0x00596330  int; the server PvP ruleset (HandleWorldMessage copies it from the server message; EQPlayer tests 2 and 4;
 * ACT_PVP_RULE in EQ_Character.cpp). Nonzero also skips one starter item in EQ_PC::RefitNewbieEQ. */
#define g_pvpRuleset_guess                         EQ_GLOBAL(int, 0x00596330)

/* 0x00591AAC / 0x00591A8C  static buffers returned by RaceName / ClassName. */
#define g_race_name_buffer_guess    EQ_ARRAY(char, 0x00591AAC)
#define g_class_name_buffer_guess   EQ_ARRAY(char, 0x00591A8C)
/* EQ_PC::ProcessFatigue: two flags that pin fatigue to 0, the "sitting/moving" mode byte and the value of
 * the previous tick. Meanings not checked. */
#define g_offlineMode_guess                EQ_GLOBAL(u8, 0x006B63F8)
#define DAT_006b6494                EQ_GLOBAL(int, 0x006B6494)
#define g_runMode_guess                EQ_GLOBAL(u8, 0x006B6405)
#define g_lastFatigue_guess                EQ_GLOBAL(s16, 0x00592208)

/* --- the classes ------------------------------------------------------------------------------------ */

class EQ_Character : public CharacterProfile
{
public:
    /* EQ_Character.cpp */
    EQ_Character();                                                    /* 0x004196D4 */
    ~EQ_Character();                                                   /* 0x004197FF */
    void ModifyCurHP_guess(int delta, int unused);                     /* 0x0041976F */
    int ExpansionFlag1_guess(void);                                    /* 0x004197A7 */
    u32 ExpansionFlag2_guess(void);                                    /* 0x004197B2 */
    u32 ExpansionFlag4_guess(void);                                    /* 0x004197BF */
    static int normal_to_special(int skill);                     /* 0x004197CD */
    int IsSpellcaster(void);                                     /* 0x0041980E */
    void DoLight(int unused);                                    /* 0x00419826 */
    int HasSpell(short spell);                                   /* 0x00419956 */
    static int IsIntCaster_guess(EQ_Character* ch);                    /* 0x00419CF5 */
    u8 GetInnateSkill_guess(u8 skill, int race_unused);                /* 0x0041A47D */
    int basesave_magic(void);                                    /* 0x0041A76D */
    int basesave_fire(void);                                     /* 0x0041A92B */
    int basesave_cold(void);                                     /* 0x0041AB14 */
    int basesave_disease(void);                                  /* 0x0041ACED */
    int basesave_poison(void);                                   /* 0x0041AEE8 */
    s16 CreationTableValue_guess(short col, u8 row);                   /* 0x0041C648 */
    u8 CheckClass_guess(short race, short cls);                          /* 0x0041C663 */
    bool HumanCanWorship_guess(u8 col, u8 row);                          /* 0x0041C6BB */
    bool BarbarianCanWorship_guess(u8 col, u8 row);                      /* 0x0041C6F7 */
    bool EruditeCanWorship_guess(u8 col, u8 row);                        /* 0x0041C739 */
    bool ElfCanWorship_guess(u8 col, u8 row);                            /* 0x0041C779 */
    bool HighElfCanWorship_guess(u8 col, u8 row);                        /* 0x0041C7AB */
    bool DarkElfCanWorship_guess(u8 col, u8 row);                        /* 0x0041C7D7 */
    bool HalfElfCanWorship_guess(u8 col, u8 row);                        /* 0x0041C803 */
    bool DwarfCanWorship_guess(u8 col, u8 row);                          /* 0x0041C83E */
    bool TrollCanWorship_guess(u8 col, u8 row);                          /* 0x0041C86A */
    bool OgreCanWorship_guess(u8 col, u8 row);                           /* 0x0041C896 */
    bool HalflingCanWorship_guess(u8 col, u8 row);                       /* 0x0041C8C2 */
    bool GnomeCanWorship_guess(u8 col, u8 row);                          /* 0x0041C8F4 */
    bool IksarCanWorship_guess(u8 col, u8 row);                          /* 0x0041C920 */
    bool VahShirCanWorship_guess(u8 col, u8 row);                      /* 0x0041C94C */
    s32 MinAlign_guess(u8 index);                                      /* 0x0041CB41 */
    s32 MaxAlign_guess(u8 index);                                      /* 0x0041CB50 */
    u8 ClassMin_guess(short race, u8 stat);                            /* 0x0041CB5F */
    bool CityCanStart_guess(short city, short row, short cls, short deity); /* 0x0041C957 */
    int TotalOnPerson(void);                                     /* 0x00423DEB */
    void ItemSold_guess(int price);                                    /* 0x00423E16 */
    static int IsHighLevelNPC_guess(EQ_Character* ch);                 /* 0x004246B5 */
    bool FindItemByType_guess(short type, short* slot, short* bag_slot);   /* 0x00427C70 */
    bool FindItemByNumber_guess(short item_nr, short* slot, short* bag_slot); /* 0x00427D2A */
    int FoodDrinkStatus_guess(void);                                   /* 0x00428405 */
    int IsInstrument_guess(void);                                            /* 0x00436285 (`this` is an item) */
    u8 HasSkill(u8 skill);                                       /* 0x0041A3E9 */
    int LevelDiffModifier_guess(int other_level);                      /* 0x0041A6D6 */
    int LevelDiffWithBase_guess(int other_level);                       /* 0x0041A75E */
    int cur_encumbrance(void);                                    /* 0x0041BC98 */
    int HasSkillPassthrough_guess(int skill);                           /* 0x004246A9 */
    bool HasEnoughItemsForSpell_guess(short item_id, short required);  /* 0x00427DD3 */
    char* RaceName_guess(short race_id);                               /* 0x0041BE53 */
    char* ClassName_guess(u16 class_id);                               /* 0x0041BF2E */

    /* EQ_Character_MagicSystem.cpp */
    int BardCastBard_guess(EQ_Spell_guess* spell, int cls);            /* 0x0041F0C0 */
    static int BardCastBard_guess(void);                               /* 0x0046A72F */
    bool LaunchSpell_guess(int a, int b, int c);                       /* 0x00420016 */
    static int IsSpellEffectValid_guess(EQ_Spell_guess* spell, int slot); /* 0x0042001B */
    static int IsSpellCausingDeath_guess(short spell);                 /* 0x00422D68 */
    SpellBuff* GetPCSpellAffect_guess(u8 effect, short* slot);         /* 0x0042381B */
    u8 GetBuffLevel_guess(short spell);                                /* 0x00423DAD */
    static int IsSpecialSpell_guess(short spell);                      /* 0x00423E6F */
    u32 SpellDuration_guess(EQ_Spell_guess* spell, u8 level, char permanent_ok); /* 0x00424591 */
    static int CalculateDamageBonus_guess(EQ_Character* character, int unused, int* maximum); /* 0x004284FA */
    static int Swizzle_guess(u8* data);                                /* 0x004A0792 */
    void InitInnates(u16 charRace, s16 charClass);    /* 0x0041D17E */
    bool CheckDualWield_guess(int unused);    /* 0x0041D55B */
    bool SkillCheck_guess(int unused);    /* 0x0041D5BC */
    void DoPassageOfTime(void);    /* 0x00425D5A */
    void DoFishingEvent(void);    /* 0x0041EE15 */
    int GetPoisonResist_guess(int other_level);    /* 0x0041AF14 */
    int GetMagicResist_guess(int other_level);    /* 0x0041A7A0 */
    void HandleSpecialPCAffects_guess(int mode);    /* 0x004226C1 */
};

class EQ_PC : public EQ_Character
{
public:
    /* EQ_PC.cpp */
    void CheckSkillImprove_guess(int skill, int unused);               /* 0x004501A1 */
    static int __stdcall ScaledSkillValue_guess(u8 index, short class_id); /* 0x00480391 */
    void InitializeInventorySlots_guess(void);                         /* 0x004803EF */
    void RefitNewbieEQ(void);                                    /* 0x004806CB */
    void SetFatigue(int value);                                  /* 0x00480DFB */
    int numInParty(void);                                        /* 0x00480E23 */
    static int GetKunarkThreshold_guess(EQ_PC* pc, short index, u32* maximum); /* 0x00480E8C */
    int IsAGroupMember(char* member);                            /* 0x0047EFCE */
    void ProcessFatigue(void);                                   /* 0x00480A94 */
};

/* The object at *(EQEnvironment_guess **)0x006E4DDC: callers from Display, EQPlayer::headEnvironmentChange,
 * StunMe and DoPassageOfTime set its state at +0xC. Only that field is known; size unknown. */
class EQEnvironment_guess
{
public:
    u8      unknown0x00[0xC];   /* 0x00 */
    s32     state;              /* 0x0C* 4 and above stick until -1 */

    void SetState_guess(int state);                                    /* 0x004E1F74 */
};
#define g_environment_guess         EQ_GLOBAL(EQEnvironment_guess *, 0x006E4DDC)

#endif /* TRILOGY_EQ_CHARACTER_H */
