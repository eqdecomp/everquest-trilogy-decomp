/*
 * character.h - the player profile. The client's EQ_PC / EQ_Character object (the global at
 * 0x006E4DA8) starts with exactly these 0x1FA4 bytes: OP_PlayerProfile is inflated, checked and
 * copied over it (0x7E9 dwords), OP_PlayerSave (0x2154/0x2155, 0x004C71A3) and
 * OP_CharacterCreate (0x2049, 0x004D1166) send it back as is, and the EQ_Character methods read
 * the same offsets from `this`. EQ_PC adds no data: every allocation is 0x1FA4 bytes (EverQuest.cpp
 * NewCharacter_guess / ProfileAllocation_guess, both constructed by EQ_PC::EQ_PC 0x0047EB8E).
 */
#ifndef TRILOGY_CHARACTER_H
#define TRILOGY_CHARACTER_H

#include "common.h"
#include "item.h"

#pragma pack(push, 1)

struct EQPlayer;

/* One buff slot, 10 bytes. Verified against client: stride 10 over 0x284..0x31A (15 slots) and
 * the spell id at +4 (OP_Buff handler, HandleWorldMessage case 0x2132). EQClassic SpellBuff_Struct
 * and ShowEQ spellBuff agree. */
typedef struct SpellBuff
{
    u8      visible;            /* 0x00  0 = free; matched against MsgBuff byte 4 */
    u8      level;              /* 0x01  caster level */
    u8      bard_modifier;      /* 0x02  */
    u8      unknown0x03;        /* 0x03  */
    s16     spell_id;           /* 0x04* */
    s32     duration;           /* 0x06  ticks */
} SpellBuff;                    /* 0x0A */
C_ASSERT(sizeof(SpellBuff) == 0x0A);

/* The profile, 0x1FA4 bytes. Verified against client: size (0x1FA4 copy / send) and every field
 * marked * (read or written by the function or handler named). Layout from EQClassic
 * PlayerProfile_Struct, whose offsets are ours + 4 (it includes the checksum); every offset we
 * checked agrees. ShowEQ playerProfileStruct (4534 bytes incl. opcode and checksum) agrees on
 * the offsets it names except gender and stops at 0x11B0.
 * DISCREPANCIES: gender is at 0x32 (EQClassic; HitBySpell 0x00420505 flips it for illusions),
 * ShowEQ has it at 0x37. EQClassic's drunkeness (u8) + eqbackground at 0x104C/0x104D are one s16
 * (EQ_Character::Dex/Int read *(s16 *)(this + 0x104C)). EQClassic's pvpEnabled (u8) is a dword
 * (0x218C handler writes 1 to 0xAB8 as an int). EQClassic's discplineAvailable (u8 at 0xAE0) is a
 * dword of flags (every reader but three bit-test accessors reads 4 bytes; SetPCloudDensity compares
 * it signed with 2). EQClassic's race is a short; the client reads it unsigned. */
typedef struct CharacterProfile
{
    char    name[30];           /* 0x0000* */
    char    last_name[20];      /* 0x001E  */
    u8      gender;             /* 0x0032* */
    u8      deity;              /* 0x0033  */
    u16     race;               /* 0x0034* read movzx word: checkLang (0x004A08D7) indexes a table with
                                            it, EQ_Character::Skill/HasSkill, can_use_item */
    u8      class_;             /* 0x0036* Max_HP / Max_Mana switch on it */
    u8      unknown0x0037;      /* 0x0037  compared with 4, 9, 26, 29, 30 in HitBySpell */
    u8      level;              /* 0x0038* */
    u8      unknown0x0039[3];   /* 0x0039  */
    u32     exp;                /* 0x003C* OP_LevelUpdate / OP_ExpUpdate */
    s16     training_points;    /* 0x0040* +5 per level gained */
    s16     mana;               /* 0x0042* OP_ManaChange */
    u8      face;               /* 0x0044  */
    u8      equipment[9];       /* 0x0045* copied from EQPlayer.equipment on save */
    u8      unknown0x004E[2];   /* 0x004E  */
    u32     equip_colors[9];    /* 0x0050* copied from EQPlayer.equip_colors on save */
    s16     cur_hp;             /* 0x0074* OP_HPUpdate for the local player */
    u8      unknown0x0076;      /* 0x0076* life state: 4 when dying (0x00422A48), 2/0 feign */
    u8      STR;                /* 0x0077  */
    u8      STA;                /* 0x0078  */
    u8      CHA;                /* 0x0079  */
    u8      DEX;                /* 0x007A* EQ_Character::Dex */
    u8      INT;                /* 0x007B* EQ_Character::Int */
    u8      AGI;                /* 0x007C  */
    u8      WIS;                /* 0x007D  */
    u8      languages[24];      /* 0x007E* language skills */
    u8      unknown0x0096[10];  /* 0x0096  */
    struct EQPlayer *player;    /* 0x00A0* the local EQPlayer; junk on the wire */
    s16     inventory[30];      /* 0x00A4* item numbers, -1 = empty (filled on save) */
    EQ_Item *inventory_items[30]; /* 0x00E0* live pointers; zeroed on receipt */
    EQ_ItemProperties inv_props[30]; /* 0x0158* */
    SpellBuff buffs[15];        /* 0x0284* */
    s16     bag_inventory[80];  /* 0x031A* 8 bags x 10 */
    s16     cursor_bag_inventory[10]; /* 0x03BA* */
    EQ_ItemProperties bag_props[80]; /* 0x03CE* */
    EQ_ItemProperties cursor_bag_props[10]; /* 0x06EE* */
    s16     spell_book[256];    /* 0x0752* OP 0x21CE swaps entries */
    union
    {
        s16     spell_memorized[8]; /* 0x0952  */
        u32     spell_memorized_pairs_guess[4]; /* 0x0952* the 8 gems as 4 dword pairs (EQ_PC clears) */
    };
    u8      unknown0x0962[2];   /* 0x0962  */
    f32     y;                  /* 0x0964* copied from EQPlayer on save */
    f32     x;                  /* 0x0968* */
    f32     z;                  /* 0x096C* */
    f32     heading;            /* 0x0970* */
    char    zone[15];           /* 0x0974* compared with "airplane" on load */
    u8      unknown0x0983[17];  /* 0x0983  */
    u8      stand_state;        /* 0x0994* set to 100 on zone-in */
    u8      unknown0x0995[3];   /* 0x0995  */
    s32     platinum;           /* 0x0998* coin types 1..4 */
    s32     gold;               /* 0x099C* */
    s32     silver;             /* 0x09A0* */
    s32     copper;             /* 0x09A4* */
    s32     platinum_bank;      /* 0x09A8* coin types 5..8 */
    s32     gold_bank;          /* 0x09AC* */
    s32     silver_bank;        /* 0x09B0* */
    s32     copper_bank;        /* 0x09B4* */
    s32     platinum_cursor;    /* 0x09B8* coins on the cursor */
    s32     gold_cursor;        /* 0x09BC* */
    s32     silver_cursor;      /* 0x09C0* */
    s32     copper_cursor;      /* 0x09C4* */
    u8      skills[74];         /* 0x09C8* EQ_Character::Skill */
    u8      unknown0x0A12[26];  /* 0x0A12  */
    u8      innateSkills_guess[25]; /* 0x0A2C* */
    u8      airSupply_guess;    /* 0x0A45* */
    u8      unknown0x0A46;      /* 0x0A46* level override in Max_Mana */
    s8      unknown0x0A47;      /* 0x0A47* EQ_Character::EQ_Character (0x004196D4) sets -1 */
    f32     unknown0x0A48;      /* 0x0A48* EQ_Character::EQ_Character sets -1.0 */
    f32     unknown0x0A4C;      /* 0x0A4C* ... 3.0 */
    f32     unknown0x0A50;      /* 0x0A50* ... 2.5 */
    f32     unknown0x0A54;      /* 0x0A54* ... 5.5 */
    char    boat[32];           /* 0x0A58* vehicle name (StripShipName) */
    char    unknown0x0A78[32];  /* 0x0A78* text set by OP 0x20D4, sent in OP_InspectAnswer */
    u32     primary_model;      /* 0x0A98* IT number of the primary weapon (filled on save) */
    u32     secondary_model;    /* 0x0A9C* */
    int     castRequirementState_guess; /* 0x0AA0* */
    u32     unk_0xAA4;          /* 0x0AA4* */
    int     cannotBreathe_guess;   /* 0x0AA8* */
    u8      unknown0x0AAC[8];   /* 0x0AAC  */
    union
    {
        u8      autosplit;          /* 0x0AB4  */
        int     autosplit_int_guess; /* 0x0AB4* */
    };
    s32     pk;                 /* 0x0AB8* 1 = player killer (OP 0x218C) */
    u8      unknown0x0ABC[12];  /* 0x0ABC  */
    union
    {
        u8      gm;                 /* 0x0AC8  */
        int     gm_int_guess;       /* 0x0AC8* */
    };
    u8      unknown0x0ACC[20];  /* 0x0ACC  */
    s32     expansions_guess;   /* 0x0AE0* bit flags, read as a dword: != 0 gates the post-50 skill caps
                                            (GetSkillLimit2 0x004A0C61, EQ_PC_GetKunarkThreshold
                                            0x00480E8C, setskill), >= 2 in SetPCloudDensity (0x004A31A5,
                                            given g_pc); 0x004197A7/B2/BF return bit 0/1/2 of its low byte */
    u8      unknown0x0AE4[12];  /* 0x0AE4  */
    u32     newbie_items_guess; /* 0x0AF0* bit flags: EQ_PC::RefitNewbieEQ (0x004806CB) puts fixed item numbers
                                            in inventory slots 23..29 for bits 0..6; 0 = starter items */
    u8      unknown0x0AF4[4];   /* 0x0AF4  */
    s32     hunger;             /* 0x0AF8* OP_Stamina, clamped 0..32000 */
    s32     thirst;             /* 0x0AFC* */
    u8      unknown0x0B00[24];  /* 0x0B00  */
    char    bind_zone[20];      /* 0x0B18  */
    char    start_zones[4][20]; /* 0x0B2C  */
    EQ_ItemProperties bank_props[8]; /* 0x0B7C* */
    EQ_ItemProperties bank_bag_props[80]; /* 0x0BCC* */
    u8      unknown0x0EEC[4];   /* 0x0EEC  */
    f32     bind_location[3][5];/* 0x0EF0* [0][1] y, [1][1] x, [2][1] z (0x00495996 reads 0x0F1C) */
    u8      unknown0x0F2C[24];  /* 0x0F2C  */
    EQ_Item *bank_items[8];     /* 0x0F44* */
    u32     unk_0xF64;          /* 0x0F64* */
    u32     unk_0xF68;          /* 0x0F68* */
    u32     unk_0xF6C;          /* 0x0F6C* */
    u32     time1;              /* 0x0F70  */
    u32     unk_0xF74;          /* 0x0F74* */
    u32     unk_0xF78;          /* 0x0F78* */
    u32     unk_0xF7C;          /* 0x0F7C* */
    u16     unk_0xF80;          /* 0x0F80* */
    u16     unk_0xF82;          /* 0x0F82* */
    u16     unk_0xF84;          /* 0x0F84* */
    u16     unk_0xF86;          /* 0x0F86* */
    s16     bank_inventory[8];  /* 0x0F88* */
    s16     bank_bag_inventory[80]; /* 0x0F98* */
    s16     deityId_guess;      /* 0x1038* EQ_PC::EQ_PC sets 0x8C, 0x01 */
    s16     guild_id;           /* 0x103A* */
    u32     unknown0x103C;      /* 0x103C* EQClassic time2; echoed by OP 0x2130 */
    u32     unknown0x1040;      /* 0x1040* echoed by OP 0x2130 */
    u8      unknown0x1044[2];   /* 0x1044  */
    s8      fatigue;            /* 0x1046* 0..100, EQ_PC::SetFatigue (0x00480DFB); read movsx (ProcessFatigue) */
    u8      pvp;                /* 0x1047* SpawnAppearance type 4 */
    u8      highest_level;      /* 0x1048* raised by OP_LevelUpdate */
    u8      anon;               /* 0x1049* SpawnAppearance type 21 */
    u8      gm_flag;            /* 0x104A* SpawnAppearance type 20 */
    s8      guild_rank;         /* 0x104B* 2 = leader */
    s16     intoxication;       /* 0x104C* */
    u8      unknown0x104E[2];   /* 0x104E  */
    u32     spell_slot_refresh[10]; /* 0x1050 three clear loops write 10 dwords (EQ_PC.cpp, EverQuest.cpp) */
    char    group_members[6][48]; /* 0x1078* 0x00491632 */
    u8      unknown0x1198[24];  /* 0x1198  */
    u32     unk_0x11B0;         /* 0x11B0* */
    u8      unk_0x11B4;         /* 0x11B4* */
    u8      unk_0x11B5;         /* 0x11B5* */
    u8      appearanceFeature4_guess; /* 0x11B6* */
    u8      appearanceFeature5_guess; /* 0x11B7* */
    u8      appearanceFeature0_guess; /* 0x11B8* */
    u8      appearanceFeature1_guess; /* 0x11B9* */
    u8      appearanceFeature2_guess; /* 0x11BA* */
    u8      appearanceFeature3_guess; /* 0x11BB* */
    u32     unk_0x11BC[22];     /* 0x11BC* */
    u32     unk_0x1214[22];     /* 0x1214* */
    u8      unknown0x126C[3384]; /* 0x126C  */
} CharacterProfile;             /* 0x1FA4 */
C_ASSERT(sizeof(CharacterProfile) == 0x1FA4);
C_ASSERT(offsetof(CharacterProfile, gender) == 0x32);
C_ASSERT(offsetof(CharacterProfile, race) == 0x34);
C_ASSERT(offsetof(CharacterProfile, level) == 0x38);
C_ASSERT(offsetof(CharacterProfile, exp) == 0x3C);
C_ASSERT(offsetof(CharacterProfile, mana) == 0x42);
C_ASSERT(offsetof(CharacterProfile, equip_colors) == 0x50);
C_ASSERT(offsetof(CharacterProfile, cur_hp) == 0x74);
C_ASSERT(offsetof(CharacterProfile, DEX) == 0x7A);
C_ASSERT(offsetof(CharacterProfile, languages) == 0x7E);
C_ASSERT(offsetof(CharacterProfile, player) == 0xA0);
C_ASSERT(offsetof(CharacterProfile, inventory) == 0xA4);
C_ASSERT(offsetof(CharacterProfile, inventory_items) == 0xE0);
C_ASSERT(offsetof(CharacterProfile, inv_props) == 0x158);
C_ASSERT(offsetof(CharacterProfile, buffs) == 0x284);
C_ASSERT(offsetof(CharacterProfile, bag_inventory) == 0x31A);
C_ASSERT(offsetof(CharacterProfile, cursor_bag_inventory) == 0x3BA);
C_ASSERT(offsetof(CharacterProfile, bag_props) == 0x3CE);
C_ASSERT(offsetof(CharacterProfile, cursor_bag_props) == 0x6EE);
C_ASSERT(offsetof(CharacterProfile, spell_book) == 0x752);
C_ASSERT(offsetof(CharacterProfile, y) == 0x964);
C_ASSERT(offsetof(CharacterProfile, zone) == 0x974);
C_ASSERT(offsetof(CharacterProfile, platinum) == 0x998);
C_ASSERT(offsetof(CharacterProfile, platinum_cursor) == 0x9B8);
C_ASSERT(offsetof(CharacterProfile, skills) == 0x9C8);
C_ASSERT(offsetof(CharacterProfile, boat) == 0xA58);
C_ASSERT(offsetof(CharacterProfile, primary_model) == 0xA98);
C_ASSERT(offsetof(CharacterProfile, pk) == 0xAB8);
C_ASSERT(offsetof(CharacterProfile, expansions_guess) == 0xAE0);
C_ASSERT(offsetof(CharacterProfile, newbie_items_guess) == 0xAF0);
C_ASSERT(offsetof(CharacterProfile, hunger) == 0xAF8);
C_ASSERT(offsetof(CharacterProfile, bank_props) == 0xB7C);
C_ASSERT(offsetof(CharacterProfile, bank_bag_props) == 0xBCC);
C_ASSERT(offsetof(CharacterProfile, bind_location) == 0xEF0);
C_ASSERT(offsetof(CharacterProfile, bank_items) == 0xF44);
C_ASSERT(offsetof(CharacterProfile, bank_inventory) == 0xF88);
C_ASSERT(offsetof(CharacterProfile, bank_bag_inventory) == 0xF98);
C_ASSERT(offsetof(CharacterProfile, guild_id) == 0x103A);
C_ASSERT(offsetof(CharacterProfile, fatigue) == 0x1046);
C_ASSERT(offsetof(CharacterProfile, guild_rank) == 0x104B);
C_ASSERT(offsetof(CharacterProfile, intoxication) == 0x104C);
C_ASSERT(offsetof(CharacterProfile, group_members) == 0x1078);
C_ASSERT(offsetof(CharacterProfile, innateSkills_guess) == 0x0A2C);
C_ASSERT(offsetof(CharacterProfile, airSupply_guess) == 0x0A45);
C_ASSERT(offsetof(CharacterProfile, castRequirementState_guess) == 0x0AA0);
C_ASSERT(offsetof(CharacterProfile, cannotBreathe_guess) == 0x0AA8);
C_ASSERT(offsetof(CharacterProfile, deityId_guess) == 0x1038);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature4_guess) == 0x11B6);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature5_guess) == 0x11B7);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature0_guess) == 0x11B8);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature1_guess) == 0x11B9);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature2_guess) == 0x11BA);
C_ASSERT(offsetof(CharacterProfile, appearanceFeature3_guess) == 0x11BB);
C_ASSERT(offsetof(CharacterProfile, unk_0xAA4) == 0x0AA4);
C_ASSERT(offsetof(CharacterProfile, unk_0xF64) == 0x0F64);
C_ASSERT(offsetof(CharacterProfile, unk_0xF68) == 0x0F68);
C_ASSERT(offsetof(CharacterProfile, unk_0xF6C) == 0x0F6C);
C_ASSERT(offsetof(CharacterProfile, unk_0xF74) == 0x0F74);
C_ASSERT(offsetof(CharacterProfile, unk_0xF78) == 0x0F78);
C_ASSERT(offsetof(CharacterProfile, unk_0xF7C) == 0x0F7C);
C_ASSERT(offsetof(CharacterProfile, unk_0xF80) == 0x0F80);
C_ASSERT(offsetof(CharacterProfile, unk_0xF82) == 0x0F82);
C_ASSERT(offsetof(CharacterProfile, unk_0xF84) == 0x0F84);
C_ASSERT(offsetof(CharacterProfile, unk_0xF86) == 0x0F86);
C_ASSERT(offsetof(CharacterProfile, unk_0x11B0) == 0x11B0);
C_ASSERT(offsetof(CharacterProfile, unk_0x11B4) == 0x11B4);
C_ASSERT(offsetof(CharacterProfile, unk_0x11B5) == 0x11B5);
C_ASSERT(offsetof(CharacterProfile, unk_0x11BC) == 0x11BC);
C_ASSERT(offsetof(CharacterProfile, unk_0x1214) == 0x1214);

/* OP_PlayerProfile (0x202D), server -> client, zlib-compressed. Verified against client:
 * HandleWorldMessage (jump case 7) inflates it, requires 0x1FA8 bytes and
 * crc32mem(data + 4, 0x1FA4) == first dword. EQClassic PlayerProfile_Struct is this whole
 * message (8104 bytes; its "logtime" is the last dword of the profile). ShowEQ's 4534 is short. */
typedef struct MsgPlayerProfile
{
    u32     crc;                /* 0x0000 */
    CharacterProfile profile;   /* 0x0004 */
} MsgPlayerProfile;             /* 0x1FA8 */
C_ASSERT(sizeof(MsgPlayerProfile) == 0x1FA8);

#pragma pack(pop)

#endif /* TRILOGY_CHARACTER_H */
