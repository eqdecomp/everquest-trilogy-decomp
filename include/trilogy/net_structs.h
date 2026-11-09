/*
 * net_structs.h - message bodies of the Trilogy eqgame.exe (2001-08-21), after the u16 opcode.
 * Profile, spawns and items are in character.h, player.h and item.h.
 *
 * Every struct says which references describe it and what was checked in the client:
 *   "verified against client (size ...)"   the client sends or copies exactly that many bytes, or
 *                                          checks the length;
 *   "(fields ...)"                         the listed offsets are read or written by the code named;
 *   "from references only"                 not checked.
 * References: EQClassic (Common\Include\eq_packet_structs.h), ShowEQ 3.0.4 (src\everquest.h, whose
 * offsets include the 2-byte opcode; they are quoted here without it). Where a reference differs
 * from the client, the client layout is used and the difference is noted.
 * Short names used below: HWM = CEverQuest::HandleWorldMessage (0x00496743).
 */
#ifndef TRILOGY_NET_STRUCTS_H
#define TRILOGY_NET_STRUCTS_H

#include "common.h"
#include "item.h"
#include "player.h"
#include "character.h"

#pragma pack(push, 1)

/* ---- login, world, zoning ---------------------------------------------------------------- */

/* OP_SendLoginInfo (0x1858), client -> world. Verified against client (size 0xC4, fields
 * 0xBA..0xC3): sent by 0x004E013C. The first 186 bytes hold the account name and password joined
 * by a separator. EQClassic LoginInfo_Struct describes 170 bytes; not in ShowEQ. */
typedef struct MsgLoginInfo
{
    char    credentials[186];   /* 0x00  */
    u16     unknown0xBA;        /* 0xBA  0 */
    u32     unknown0xBC;        /* 0xBC  a client flag (0x006B7110) */
    u32     unknown0xC0;        /* 0xC0  0x9E */
} MsgLoginInfo;                 /* 0xC4 */
C_ASSERT(sizeof(MsgLoginInfo) == 0xC4);

/* OP_SendCharInfo (0x2047), world -> client: the 10 character-select slots. Verified against
 * client (size 0x494 and all fields): 0x004923C8 copies 0x494 bytes and reads each array.
 * DISCREPANCY: EQClassic CharacterSelect_Struct is 1260 bytes; it agrees up to 0x3F4 and then
 * has 20 + 228 unknown bytes where the client has deity, weapon models and six appearance
 * arrays (1172 bytes in all). Not in ShowEQ. */
typedef struct MsgCharacterSelect
{
    char    name[10][30];       /* 0x000 "<none>" = empty slot */
    u8      level[10];          /* 0x12C */
    u8      class_[10];         /* 0x136 */
    s16     race[10];           /* 0x140 */
    char    zone[10][20];       /* 0x154 */
    u8      gender[10];         /* 0x21C */
    u8      face[10];           /* 0x226 */
    u8      equipment[10][9];   /* 0x230 */
    u8      unknown0x28A[2];    /* 0x28A */
    u32     equip_colors[10][9];/* 0x28C */
    s16     deity[10];          /* 0x3F4 EQ_CharSelS::IsEvil(race, class, deity) */
    u32     primary_model[10];  /* 0x408 IT number, drawn in slot 7 */
    u32     secondary_model[10];/* 0x430 IT number, drawn in slot 8 */
    u8      appearance[6][10];  /* 0x458 -> EQPlayer.appearance[0..5] (0x12D..0x132) */
} MsgCharacterSelect;           /* 0x494 */
C_ASSERT(sizeof(MsgCharacterSelect) == 0x494);
C_ASSERT(offsetof(MsgCharacterSelect, class_) == 0x136);
C_ASSERT(offsetof(MsgCharacterSelect, equip_colors) == 0x28C);
C_ASSERT(offsetof(MsgCharacterSelect, deity) == 0x3F4);
C_ASSERT(offsetof(MsgCharacterSelect, appearance) == 0x458);

/* OP_NameApproval (0x208B), client -> world. Verified against client (size 0x28): 0x004D1166.
 * Fields from EQClassic NameApproval_Struct (40 bytes). */
typedef struct MsgNameApproval
{
    char    name[30];           /* 0x00  */
    u8      unknown0x1E[2];     /* 0x1E  */
    u8      race;               /* 0x20  */
    u8      unknown0x21[3];     /* 0x21  */
    u8      class_;             /* 0x24  */
    u8      unknown0x25[3];     /* 0x25  */
} MsgNameApproval;              /* 0x28 */
C_ASSERT(sizeof(MsgNameApproval) == 0x28);

/* OP_ZoneEntry (0x202A), client -> zone. Verified against client (size 0x24 and fields):
 * 0x004D4C19 sends crc32mem(name, 32) followed by the 32-byte name.
 * DISCREPANCY: ShowEQ ClientZoneEntryStruct has the same size but name[20] followed by three
 * dwords. Not in EQClassic. The server's reply is MsgServerZoneEntry (player.h). */
typedef struct MsgClientZoneEntry
{
    u32     crc;                /* 0x00  crc32mem(name, 0x20) */
    char    name[32];           /* 0x04  */
} MsgClientZoneEntry;           /* 0x24 */
C_ASSERT(sizeof(MsgClientZoneEntry) == 0x24);

/* OP_NewZone (0x205B), zone -> client. Verified against client (size 0x174; fields fog colours,
 * fog clip planes, sky, min/max clip): HWM jump case 0xC memcpy()s 0x174 bytes to 0x006B65DC and
 * reads those offsets. EQClassic NewZone_Struct names the fields (and says 372 bytes) but ends with
 * an extra unknown_end[32] (404 bytes); ShowEQ newZoneStruct has 372 bytes and the names only. */
typedef struct MsgNewZone
{
    char    char_name[30];      /* 0x000 */
    char    zone_short_name[15];/* 0x01E */
    u8      unknown0x02D[5];    /* 0x02D */
    char    zone_long_name[180];/* 0x032 */
    u8      zone_type;          /* 0x0E6 */
    u8      fog_red[4];         /* 0x0E7* defaults to 0xC0 when both clip planes are 0 */
    u8      fog_green[4];       /* 0x0EB* */
    u8      fog_blue[4];        /* 0x0EF* */
    u8      unknown0x0F3;       /* 0x0F3 */
    f32     fog_min_clip[4];    /* 0x0F4* default 500 */
    f32     fog_max_clip[4];    /* 0x104* default 600 */
    u8      unknown0x114[54];   /* 0x114 */
    u8      sky;                /* 0x14A* CDisplay sky index = sky - 1 */
    f32     gravity;            /* 0x14B  EQClassic: must be about 0.4 or the player cannot move */
    u8      unknown0x14F[9];    /* 0x14F */
    f32     safe_y;             /* 0x158 */
    f32     safe_x;             /* 0x15C */
    f32     safe_z;             /* 0x160 */
    u8      unknown0x164[4];    /* 0x164 */
    f32     underworld;         /* 0x168 */
    f32     min_clip;           /* 0x16C* at least 50 */
    f32     max_clip;           /* 0x170* */
} MsgNewZone;                   /* 0x174 */
C_ASSERT(sizeof(MsgNewZone) == 0x174);
C_ASSERT(offsetof(MsgNewZone, fog_red) == 0xE7);
C_ASSERT(offsetof(MsgNewZone, fog_min_clip) == 0xF4);
C_ASSERT(offsetof(MsgNewZone, sky) == 0x14A);
C_ASSERT(offsetof(MsgNewZone, min_clip) == 0x16C);

/* OP_ZoneChange (0x20A3), both ways. Verified against client (size 0x44; name at 0, zone at 0x20,
 * reply status byte at 0x40): SendZoneChangeRequest 0x004DBF94. EQClassic ZoneChange_Struct
 * (32 + 16 + 20) and ShowEQ zoneChangeStruct (32 + 15 + 21) agree on size and the two names. */
typedef struct MsgZoneChange
{
    char    char_name[32];      /* 0x00* */
    char    zone_name[16];      /* 0x20* */
    u8      unknown0x30[16];    /* 0x30  */
    u8      success;            /* 0x40* reply: 0 = refused */
    u8      unknown0x41[3];     /* 0x41  */
} MsgZoneChange;                /* 0x44 */
C_ASSERT(sizeof(MsgZoneChange) == 0x44);

/* OP_Translocate (0x2206), both ways. Verified against client (size 0x58: sent by 0x004818CB;
 * fields zone, spell id (read as a dword, compared with 1422/1334), caster, y/x/z): TeleportPC
 * handler 0x00495996. EQClassic Translocate_Struct agrees (it declares spell_id as s16 + 6
 * unknown bytes). Not in ShowEQ. */
typedef struct MsgTranslocate
{
    char    zone[16];           /* 0x00* */
    u8      unknown0x10[16];    /* 0x10  */
    u32     spell_id;           /* 0x20* */
    u8      unknown0x24[4];     /* 0x24  */
    char    caster[16];         /* 0x28* */
    u8      unknown0x38[16];    /* 0x38  */
    f32     y;                  /* 0x48* */
    f32     x;                  /* 0x4C* */
    f32     z;                  /* 0x50* */
    u32     confirmed;          /* 0x54  */
} MsgTranslocate;               /* 0x58 */
C_ASSERT(sizeof(MsgTranslocate) == 0x58);

/* OP_TeleportPC (0x214D), zone -> client. Verified against client (fields zone, y/x/z/heading at
 * 0x20..0x2C): HWM. Size from EQClassic TeleportPC_Struct (48). Not in ShowEQ. */
typedef struct MsgTeleportPC
{
    char    zone[16];           /* 0x00* */
    u8      unknown0x10[16];    /* 0x10  */
    f32     y;                  /* 0x20* */
    f32     x;                  /* 0x24* */
    f32     z;                  /* 0x28* */
    f32     heading;            /* 0x2C* */
} MsgTeleportPC;                /* 0x30 */
C_ASSERT(sizeof(MsgTeleportPC) == 0x30);

/* OP_MoveSpawn (0x2021), zone -> client: put a spawn at a position. Verified against client
 * (fields): HWM jump case 2 -> 0x00491B5D reads the u16 id and three floats. Size not checked. No
 * reference. */
typedef struct MsgMoveSpawn
{
    u16     spawn_id;           /* 0x00* */
    u16     unknown0x02;        /* 0x02  */
    f32     y;                  /* 0x04* */
    f32     x;                  /* 0x08* */
    f32     z;                  /* 0x0C* */
} MsgMoveSpawn;                 /* 0x10 */
C_ASSERT(sizeof(MsgMoveSpawn) == 0x10);

/* OP_TimeOfDay (0x20F2). Verified against client (size 6 and fields): HWM jump case 0x35.
 * EQClassic TimeOfDay_Struct and ShowEQ timeOfDayStruct agree. */
typedef struct MsgTimeOfDay
{
    u8      hour;               /* 0x00  */
    u8      minute;             /* 0x01  */
    u8      day;                /* 0x02  */
    u8      month;              /* 0x03  */
    u16     year;               /* 0x04  */
} MsgTimeOfDay;                 /* 0x06 */
C_ASSERT(sizeof(MsgTimeOfDay) == 0x06);

/* ---- spawns -------------------------------------------------------------------------------- */

/* OP_DeleteSpawn (0x202B). Verified against client (size 4 client -> server: 0x004D5D0F,
 * 0x004DC1B5; the handler reads one dword). EQClassic DeleteSpawn_Struct and ShowEQ
 * deleteSpawnStruct both say 6 bytes (id + 2 unknown). */
typedef struct MsgDeleteSpawn
{
    u32     spawn_id;           /* 0x00  */
} MsgDeleteSpawn;               /* 0x04 */
C_ASSERT(sizeof(MsgDeleteSpawn) == 0x04);

/* OP_HPUpdate (0x20B2). Verified against client (fields): HWM reads spawn id, cur and max as
 * dwords (percentage for others). EQClassic SpawnHPUpdate_Struct agrees; ShowEQ
 * spawnHpUpdateStruct has the same offsets with u16 values. */
typedef struct MsgHPUpdate
{
    u32     spawn_id;           /* 0x00* */
    s32     cur_hp;             /* 0x04* */
    s32     max_hp;             /* 0x08* */
} MsgHPUpdate;                  /* 0x0C */
C_ASSERT(sizeof(MsgHPUpdate) == 0x0C);

/* OP_SpawnAppearance (0x20F5). Verified against client (size 0xC and fields): 0x004939EB
 * memcpy()s 0xC bytes, looks up the dword id and switches on the type; sent with 0xC bytes by many
 * functions. EQClassic SpawnAppearance_Struct and ShowEQ spawnAppearance agree. Types seen:
 * 0 death, 1 level, 2 max hp?, 3 invis, 4 pvp, 5 light, 14 stance, 15 sneak, 16 spawn id,
 * 17 hp, 18 linkdead, 19 levitate, 20 gm, 21 anon, 22 guild id, ... */
typedef struct MsgSpawnAppearance
{
    u32     spawn_id;           /* 0x00* */
    s16     type;               /* 0x04* */
    u16     unknown0x06;        /* 0x06  */
    u32     parameter;          /* 0x08* */
} MsgSpawnAppearance;           /* 0x0C */
C_ASSERT(sizeof(MsgSpawnAppearance) == 0x0C);

/* OP_WearChange (0x2092). Verified against client (size 0x10: 0x0040B6F6, 0x004A1E2A; fields
 * spawn id, slot, material, colour, byte 0xC -> EQPlayer 0x12C): HWM. EQClassic
 * WearChange_Struct agrees. DISCREPANCY: ShowEQ wearChangeStruct agrees on slot (4) and material
 * (5) but is only 12 bytes. */
typedef struct MsgWearChange
{
    u32     spawn_id;           /* 0x00* */
    u8      slot;               /* 0x04* 0 head, 7/8 weapons, 9 face */
    u8      material;           /* 0x05* for weapons: the IT number's low byte */
    s16     unknown0x06;        /* 0x06  */
    u32     color;              /* 0x08* */
    u8      unknown0x0C;        /* 0x0C* -> EQPlayer 0x12C */
    u8      flag;               /* 0x0D  */
    u8      unknown0x0E[2];     /* 0x0E  */
} MsgWearChange;                /* 0x10 */
C_ASSERT(sizeof(MsgWearChange) == 0x10);

/* OP_Illusion (0x2091). Verified against client (size 0x48: 0x004AAA71; fields race 0x3E,
 * gender 0x40, texture 0x42, helm 0x44, face 0x46): HWM jump case 0x1C. EQClassic Illusion_Struct
 * agrees. Not in ShowEQ. */
typedef struct MsgIllusion
{
    char    name[16];           /* 0x00* */
    u8      unknown0x10[14];    /* 0x10  */
    char    target[16];         /* 0x1E  */
    u8      unknown0x2E[16];    /* 0x2E  */
    s16     race;               /* 0x3E* -1 = unchanged */
    u8      gender;             /* 0x40* */
    u8      unknown0x41;        /* 0x41  */
    u8      texture;            /* 0x42* */
    u8      unknown0x43;        /* 0x43  */
    s16     helm;               /* 0x44* -1 = unchanged */
    s16     face;               /* 0x46* -1 = unchanged */
} MsgIllusion;                  /* 0x48 */
C_ASSERT(sizeof(MsgIllusion) == 0x48);

/* OP_Death (0x204A), both ways. Verified against client (size 0x14; fields): 0x00422A48 builds it
 * (spawn id, killer, level at 0xA, spell at 0xC, skill at 0xE, damage at 0x10, is-PC flag at 0x12),
 * HWM memcpy()s 0x14 bytes to CEverQuest::ReportDeath (0x004C555F).
 * DISCREPANCY: EQClassic Death_Struct has a u32 corpse id at 8 and nothing at 0xC; ShowEQ
 * spawnKilledStruct has the spell at 0xC, type at 0xE and damage at 0x10, as the client does.
 * The server may use 0x8..0xB as EQClassic says; the client itself writes the level at 0xA. */
typedef struct MsgDeath
{
    u32     spawn_id;           /* 0x00* */
    u32     killer_id;          /* 0x04* */
    u16     unknown0x08;        /* 0x08  EQClassic: corpse id (u32 with 0xA) */
    u8      level;              /* 0x0A* */
    u8      unknown0x0B;        /* 0x0B  */
    s16     spell_id;           /* 0x0C* */
    u8      skill;              /* 0x0E* attack skill / damage type */
    u8      unknown0x0F;        /* 0x0F  */
    s16     damage;             /* 0x10* */
    u8      is_pc;              /* 0x12* */
    u8      unknown0x13;        /* 0x13  */
} MsgDeath;                     /* 0x14 */
C_ASSERT(sizeof(MsgDeath) == 0x14);

/* OP_Action (0x2058), both ways. Verified against client (size 0x1C; fields): 0x004247F0 fills
 * target, source, type, spell, damage and three dwords at 0x10. EQClassic Action_Struct and ShowEQ
 * actionStruct agree. */
typedef struct MsgAction
{
    u32     target_id;          /* 0x00* */
    u32     source_id;          /* 0x04* */
    u8      type;               /* 0x08* */
    u8      unknown0x09;        /* 0x09  */
    s16     spell_id;           /* 0x0A* */
    s32     damage;             /* 0x0C* */
    u32     unknown0x10[3];     /* 0x10* */
} MsgAction;                    /* 0x1C */
C_ASSERT(sizeof(MsgAction) == 0x1C);

/* OP_Attack animation (0x209F), both ways. Verified against client (size 0xC: 0x0041D790 etc.;
 * fields: HWM plays animation [1] on spawn [0] at speed [2]). EQClassic Attack_Struct has the same
 * size (id, type byte, 7 unknown). */
typedef struct MsgAnimation
{
    u32     spawn_id;           /* 0x00* */
    u32     animation;          /* 0x04* */
    u32     speed;              /* 0x08* */
} MsgAnimation;                 /* 0x0C */
C_ASSERT(sizeof(MsgAnimation) == 0x0C);

/* OP_Consider (0x2137), both ways. Verified against client (size 0x1C; player and target ids):
 * 0x004A6A08. EQClassic Consider_Struct agrees.
 * DISCREPANCY: ShowEQ considerStruct is 24 bytes (u16 ids, then faction/level/hp). */
typedef struct MsgConsider
{
    u32     player_id;          /* 0x00* */
    u32     target_id;          /* 0x04* */
    s32     faction;            /* 0x08  */
    s32     unknown0x0C[3];     /* 0x0C  */
    s32     unworthy;           /* 0x18  */
} MsgConsider;                  /* 0x1C */
C_ASSERT(sizeof(MsgConsider) == 0x1C);

/* OP_ClientTarget (0x2162), client -> zone. Verified against client (size 4): 0x00425D5A.
 * EQClassic ClientTarget_Struct and ShowEQ clientTargetStruct agree. */
typedef struct MsgClientTarget
{
    u16     target_id;          /* 0x00  */
    u16     unknown0x02;        /* 0x02  */
} MsgClientTarget;              /* 0x04 */
C_ASSERT(sizeof(MsgClientTarget) == 0x04);

/* OP_SpawnProjectile (0x2045). Verified against client (size 0x74): 0x004AC67C; the handler
 * passes the message to EQMissile::EQMissile. Fields from EQClassic SpawnProjectile_Struct. */
typedef struct MsgSpawnProjectile
{
    u32     always1;            /* 0x00  */
    u32     always0;            /* 0x04  */
    u32     unknown0x08;        /* 0x08  */
    f32     y;                  /* 0x0C  */
    f32     x;                  /* 0x10  */
    f32     z;                  /* 0x14  */
    f32     heading;            /* 0x18  */
    f32     tilt;               /* 0x1C  */
    f32     velocity;           /* 0x20  */
    f32     burst_velocity;     /* 0x24  */
    f32     burst_horizontal;   /* 0x28  */
    f32     burst_vertical;     /* 0x2C  */
    f32     yaw;                /* 0x30  */
    f32     pitch;              /* 0x34  */
    f32     arc;                /* 0x38  */
    u8      unknown0x3C[4];     /* 0x3C  */
    u32     source_id;          /* 0x40  */
    u32     target_id;          /* 0x44  */
    s16     unknown0x48;        /* 0x48  */
    s16     unknown0x4A;        /* 0x4A  */
    u32     spell_id;           /* 0x4C  */
    u8      light;              /* 0x50  */
    u8      unknown0x51;        /* 0x51  */
    u8      spawn_behavior;     /* 0x52  */
    u8      projectile_type;    /* 0x53  */
    u8      source_animation;   /* 0x54  */
    char    texture[16];        /* 0x55  */
    u8      unknown0x65[15];    /* 0x65  */
} MsgSpawnProjectile;           /* 0x74 */
C_ASSERT(sizeof(MsgSpawnProjectile) == 0x74);

/* ---- spells -------------------------------------------------------------------------------- */

/* OP_CastSpell (0x217E), client -> zone. Verified against client (size 0x10 and fields):
 * EQ_Character::CastSpell 0x0041F381. EQClassic CastSpell_Struct agrees; its last unknown is a
 * crc32mem of the 600-byte spell record, byte-swizzled. ShowEQ castStruct agrees on size. */
typedef struct MsgCastSpell
{
    u16     slot;               /* 0x00* gem, 9 = ability, 10 = item */
    s16     spell_id;           /* 0x02* */
    s16     inventory_slot;     /* 0x04* item cast from */
    u16     unknown0x06;        /* 0x06  */
    u32     target_id;          /* 0x08* */
    u32     spell_crc;          /* 0x0C* */
} MsgCastSpell;                 /* 0x10 */
C_ASSERT(sizeof(MsgCastSpell) == 0x10);

/* OP_BeginCast (0x20A9), zone -> client. Verified against client (size 0xC): HWM jump case 0x23
 * memcpy()s 0xC bytes for StartCasting. Fields from EQClassic BeginCast_Struct. */
typedef struct MsgBeginCast
{
    u32     caster_id;          /* 0x00  */
    u32     spell_id;           /* 0x04  */
    u32     cast_time;          /* 0x08  ms */
} MsgBeginCast;                 /* 0x0C */
C_ASSERT(sizeof(MsgBeginCast) == 0x0C);

/* OP_CastOn (0x2046), zone -> client. Verified against client (size 0x24): HWM memcpy()s 0x24
 * bytes for EQPlayer::IWasHit and looks up the dword at 0. EQClassic CastOn_Struct and ShowEQ
 * castOnStruct agree on the size; fields from EQClassic. */
typedef struct MsgCastOn
{
    u32     target_id;          /* 0x00* */
    u32     source_id;          /* 0x04  */
    u8      source_level;       /* 0x08  */
    u8      unknown0x09[11];    /* 0x09  */
    f32     heading;            /* 0x14  */
    u8      unknown0x18[4];     /* 0x18  */
    u32     action;             /* 0x1C  */
    s16     spell_id;           /* 0x20  */
    u8      unknown0x22[2];     /* 0x22  */
} MsgCastOn;                    /* 0x24 */
C_ASSERT(sizeof(MsgCastOn) == 0x24);

/* OP_InterruptCast (0x21D3), both ways: header followed by text. Verified against client (header
 * 4, text at 4): EQ_Character::StopSpellCast 0x004279B4 sends the id and the text, length + 8.
 * EQClassic InterruptCast_Struct and ShowEQ interruptCastStruct agree (u16 id + u16). */
typedef struct MsgInterruptCast
{
    u32     spawn_id;           /* 0x00* */
    char    message[1];         /* 0x04* really variable */
} MsgInterruptCast;             /* 0x05 */
C_ASSERT(sizeof(MsgInterruptCast) == 0x05);

/* OP_ManaChange (0x217F), zone -> client. Verified against client (size 4 and fields): HWM stores
 * the u16 at 0 as CharacterProfile.mana and passes the u16 at 2 to StopSpellCast when the
 * message is 4 bytes. EQClassic ManaChange_Struct agrees. */
typedef struct MsgManaChange
{
    u16     new_mana;           /* 0x00* */
    u16     spell_id;           /* 0x02* */
} MsgManaChange;                /* 0x04 */
C_ASSERT(sizeof(MsgManaChange) == 0x04);

/* OP_MemorizeSpell (0x2182), both ways. Verified against client (size 0xC: 0x00457617 etc.;
 * fields: the handler passes the three dwords to 0x00491E6E). EQClassic MemorizeSpell_Struct
 * agrees. ShowEQ memorizeSlotStruct has the same size, different fields. */
typedef struct MsgMemorizeSpell
{
    u32     slot;               /* 0x00* */
    u32     spell_id;           /* 0x04* */
    u32     scribing;           /* 0x08* 1 = memorize, 0 = scribe */
} MsgMemorizeSpell;             /* 0x0C */
C_ASSERT(sizeof(MsgMemorizeSpell) == 0x0C);

/* OP_SwapSpell (0x21CE), both ways. Verified against client (size 8: 0x0045CB52; fields: HWM swaps
 * spell_book[from] and [to]). EQClassic SwapSpell_Struct agrees. */
typedef struct MsgSwapSpell
{
    s32     from_slot;          /* 0x00* */
    s32     to_slot;            /* 0x04* */
} MsgSwapSpell;                 /* 0x08 */
C_ASSERT(sizeof(MsgSwapSpell) == 0x08);

/* OP_Buff (0x2132), both ways. Verified against client (size 0x14: 0x00424E30, 0x004251B9;
 * fields 4 and 8: HWM removes the buff whose first byte and spell id match). EQClassic
 * Buff_Struct agrees on size. */
typedef struct MsgBuff
{
    u32     target_id;          /* 0x00  */
    u8      visible;            /* 0x04* SpellBuff.visible */
    u8      unknown0x05[3];     /* 0x05  */
    u16     spell_id;           /* 0x08* */
    u8      unknown0x0A[6];     /* 0x0A  */
    u32     buff_slot;          /* 0x10  */
} MsgBuff;                      /* 0x14 */
C_ASSERT(sizeof(MsgBuff) == 0x14);

/* ---- character state ----------------------------------------------------------------------- */

/* OP_Stamina (0x2157), zone -> client. Verified against client (fields: HWM sets hunger/thirst
 * from the two s16, clamped to 0..32000, and fatigue from the byte at 4). Size from EQClassic
 * Stamina_Struct and ShowEQ staminaStruct (6). */
typedef struct MsgStamina
{
    s16     food;               /* 0x00* */
    s16     water;              /* 0x02* */
    s8      fatigue;            /* 0x04* */
    u8      unknown0x05;        /* 0x05  */
} MsgStamina;                   /* 0x06 */
C_ASSERT(sizeof(MsgStamina) == 0x06);

/* OP_LevelUpdate (0x2198), zone -> client. Verified against client (fields: 0x0049494E reads the
 * new level at 0, the old level at 4 and the experience at 8). ShowEQ levelUpStruct agrees.
 * DISCREPANCY: EQClassic LevelUpdate_Struct is 2 bytes (level, can_delevel). */
typedef struct MsgLevelUpdate
{
    u32     level;              /* 0x00* */
    u32     old_level;          /* 0x04* +5 training points if old < new */
    u32     exp;                /* 0x08* */
} MsgLevelUpdate;               /* 0x0C */
C_ASSERT(sizeof(MsgLevelUpdate) == 0x0C);

/* OP_ExpUpdate (0x2199), zone -> client. Verified against client (field: HWM stores the dword as
 * CharacterProfile.exp). EQClassic ExpUpdate_Struct and ShowEQ expUpdateStruct agree. */
typedef struct MsgExpUpdate
{
    u32     exp;                /* 0x00* */
} MsgExpUpdate;                 /* 0x04 */
C_ASSERT(sizeof(MsgExpUpdate) == 0x04);

/* OP_SkillUpdate (0x2189). From references only (EQClassic SkillUpdate_Struct; ShowEQ
 * skillIncreaseStruct has the same shape). */
typedef struct MsgSkillUpdate
{
    u8      skill_id;           /* 0x00  */
    u8      unknown0x01[3];     /* 0x01  */
    u8      value;              /* 0x04  */
    u8      unknown0x05[3];     /* 0x05  */
} MsgSkillUpdate;               /* 0x08 */
C_ASSERT(sizeof(MsgSkillUpdate) == 0x08);

/* OP_SpecialMesg (0x2180), zone -> client: type followed by text. Verified against client (text at
 * 4: HWM parses data + 4). EQClassic SpecialMesg_Struct and ShowEQ spMesgStruct agree. */
typedef struct MsgSpecialMesg
{
    u32     msg_type;           /* 0x00  */
    char    message[1];         /* 0x04* really variable */
} MsgSpecialMesg;               /* 0x05 */
C_ASSERT(sizeof(MsgSpecialMesg) == 0x05);

/* OP_ChannelMessage (0x2107), both ways: header followed by text. Verified against client (fields
 * target 0, sender 0x20, channel 0x42, text 0x46): HWM jump case 0x3B. EQClassic
 * ChannelMessage_Struct agrees; ShowEQ channelMessageStruct has the same offsets but the language
 * and channel as bytes. */
typedef struct MsgChannelMessage
{
    char    target[32];         /* 0x00* tell recipient */
    char    sender[23];         /* 0x20* */
    u8      unknown0x37[9];     /* 0x37  */
    s16     language;           /* 0x40  */
    s16     channel;            /* 0x42* 7/14 = tell */
    u8      unknown0x44[2];     /* 0x44  */
    char    message[1];         /* 0x46* really variable */
} MsgChannelMessage;            /* 0x47 */
C_ASSERT(sizeof(MsgChannelMessage) == 0x47);

/* ---- items, coins, merchants, loot ------------------------------------------------------------ */

/* OP_CompressedItems (0x21F6): count followed by zlib data that inflates to count entries of
 * MsgCompressedItem. Verified against client (layout and stride 0x126): HandleCompressedItems
 * 0x00495456. ShowEQ compressedPacket agrees on the header. */
typedef struct MsgCompressedItems
{
    s16     count;              /* 0x00* */
    u8      data[1];            /* 0x02* really variable (compressed) */
} MsgCompressedItems;           /* 0x03 */
C_ASSERT(sizeof(MsgCompressedItems) == 0x03);

typedef struct MsgCompressedItem
{
    u16     opcode;             /* 0x00* OP_PlayerItem / OP_PlayerContainer / OP_PlayerBook */
    EQ_Equipment item;          /* 0x02* 0x124 bytes whatever the kind */
} MsgCompressedItem;            /* 0x126 */
C_ASSERT(sizeof(MsgCompressedItem) == 0x126);

/* OP_ItemToTrade (0x20DF) and OP_TradeItemToSlot (0x213E): header followed by an item of the given
 * kind (0x124, 0xDC or 0xD0 bytes). Verified against client (fields): HWM reads the slot at 4, the
 * kind at 6 and copies the item from 7. EQClassic ItemToTrade_Struct (plus 3 trailing bytes) and
 * ShowEQ tradeItemStruct (plus 5) agree on the header. */
typedef struct MsgItemToTrade
{
    u32     player_id;          /* 0x00  */
    s16     to_slot;            /* 0x04* */
    u8      kind;               /* 0x06* EQ_ITEM_*, 0xFF = clear */
} MsgItemToTrade;               /* 0x07 */
C_ASSERT(sizeof(MsgItemToTrade) == 0x07);

/* OP_ShopRequest (0x200B), both ways. Verified against client (fields: HWM reads the merchant id at
 * 0, the accept byte at 8 and the price rate float at 0xC). EQClassic Merchant_Click_Struct agrees
 * (16 bytes). Not in ShowEQ. */
typedef struct MsgMerchantClick
{
    u32     npc_id;             /* 0x00* */
    u32     player_id;          /* 0x04  */
    u8      command;            /* 0x08* 0 = refused */
    u8      unknown0x09[3];     /* 0x09  */
    f32     rate;               /* 0x0C* price multiplier */
} MsgMerchantClick;             /* 0x10 */
C_ASSERT(sizeof(MsgMerchantClick) == 0x10);

/* OP_ShopPlayerBuy (0x2027) and OP_ShopPlayerSell (0x2028), both ways. Verified against client
 * (size 0x14: 0x00463A0E, 0x00463B5D; fields slot 8, quantity 0xC, price 0x10 in the buy reply).
 * DISCREPANCY: EQClassic Merchant_Purchase_Struct puts the quantity at 0xC as a byte (and says
 * itemcost is wrong); the client reads a dword there and the price at 0x10. */
typedef struct MsgMerchantPurchase
{
    u32     npc_id;             /* 0x00  */
    u32     player_id;          /* 0x04  */
    s16     slot;               /* 0x08* */
    u8      unknown0x0A[2];     /* 0x0A  */
    s32     quantity;           /* 0x0C* */
    s32     price;              /* 0x10* copper */
} MsgMerchantPurchase;          /* 0x14 */
C_ASSERT(sizeof(MsgMerchantPurchase) == 0x14);

/* OP_ShopDelItem (0x2038), zone -> client. Verified against client (field: HWM reads the slot as a
 * signed byte at 8). Size from EQClassic Merchant_DelItem_Struct (12). */
typedef struct MsgMerchantDelItem
{
    u32     npc_id;             /* 0x00  */
    u32     player_id;          /* 0x04  */
    s8      slot;               /* 0x08* */
    u8      unknown0x09[3];     /* 0x09  */
} MsgMerchantDelItem;           /* 0x0C */
C_ASSERT(sizeof(MsgMerchantDelItem) == 0x0C);

/* OP_MoneyOnCorpse (0x2050), zone -> client. Verified against client (fields: HWM passes the byte
 * at 0 and the four dwords to 0x00464E5B). EQClassic MoneyOnCorpse_Struct and ShowEQ
 * moneyOnCorpseStruct agree (20 bytes). */
typedef struct MsgMoneyOnCorpse
{
    u8      response;           /* 0x00* 0 = someone else is looting, 1 = ok, 2 = not now */
    u8      unknown0x01[3];     /* 0x01  */
    u32     platinum;           /* 0x04* */
    u32     gold;               /* 0x08* */
    u32     silver;             /* 0x0C* */
    u32     copper;             /* 0x10* */
} MsgMoneyOnCorpse;             /* 0x14 */
C_ASSERT(sizeof(MsgMoneyOnCorpse) == 0x14);

/* OP_LootItem (0x20A0), client -> zone. Verified against client (size 0x10): 0x00457CD6. Fields
 * from EQClassic LootingItem_Struct. */
typedef struct MsgLootingItem
{
    u32     lootee;             /* 0x00  */
    u32     looter;             /* 0x04  */
    s16     slot;               /* 0x08  */
    u8      unknown0x0A[2];     /* 0x0A  */
    u32     type;               /* 0x0C  */
} MsgLootingItem;               /* 0x10 */
C_ASSERT(sizeof(MsgLootingItem) == 0x10);

/* OP_MoveItem (0x212C), both ways. Verified against client (size 0xC and fields: HWM passes the
 * three dwords to msg_move_item 0x00492102). EQClassic MoveItem_Struct agrees. */
typedef struct MsgMoveItem
{
    u32     from_slot;          /* 0x00* */
    u32     to_slot;            /* 0x04* */
    u32     number_in_stack;    /* 0x08* */
} MsgMoveItem;                  /* 0x0C */
C_ASSERT(sizeof(MsgMoveItem) == 0x0C);

/* OP_MoveCoin (0x212D), client -> zone. Verified against client (size 0x14): 0x00457CD6 etc.
 * Fields from EQClassic MoveCoin_Struct. */
typedef struct MsgMoveCoin
{
    u32     from_slot;          /* 0x00  */
    u32     to_slot;            /* 0x04  */
    u32     from_coin_type;     /* 0x08  */
    u32     to_coin_type;       /* 0x0C  */
    u32     amount;             /* 0x10  */
} MsgMoveCoin;                  /* 0x14 */
C_ASSERT(sizeof(MsgMoveCoin) == 0x14);

/* OP_SplitMoney (0x2131, client -> zone) and OP_SplitResponse (0x219A, zone -> client).
 * Verified against client (size 0x10: 0x004CFD5F; fields: the 0x219A handler adds the four
 * dwords). EQClassic Split_Struct agrees. */
typedef struct MsgSplit
{
    u32     platinum;           /* 0x00* */
    u32     gold;               /* 0x04* */
    u32     silver;             /* 0x08* */
    u32     copper;             /* 0x0C* */
} MsgSplit;                     /* 0x10 */
C_ASSERT(sizeof(MsgSplit) == 0x10);

/* OP_TradeMoneyUpdate (0x213D), zone -> client. Verified against client (fields: HWM adds the dword
 * at 8 to the coin selected by the byte at 4, 0 = copper .. 3 = platinum). EQClassic
 * TradeMoneyUpdate_Struct agrees (12 bytes). */
typedef struct MsgTradeMoneyUpdate
{
    u32     trader;             /* 0x00  */
    u32     type;               /* 0x04* */
    u32     amount;             /* 0x08* */
} MsgTradeMoneyUpdate;          /* 0x0C */
C_ASSERT(sizeof(MsgTradeMoneyUpdate) == 0x0C);

/* OP_TradeCoins (0x20E4), zone -> client. Verified against client (fields: slot byte at 4, dword
 * at 8). Size from EQClassic TradeCoin_Struct (22). */
typedef struct MsgTradeCoin
{
    u32     trader;             /* 0x00  */
    u8      slot;               /* 0x04* */
    u8      unknown0x05[3];     /* 0x05  */
    u32     amount;             /* 0x08* */
    u8      unknown0x0C[10];    /* 0x0C  */
} MsgTradeCoin;                 /* 0x16 */
C_ASSERT(sizeof(MsgTradeCoin) == 0x16);

/* OP_Click_Give / trade request (0x20D1) and OP_CancelTrade (0x20DB). Verified against client
 * (size 8: 0x004CBA05, 0x004571EB; the 0x20D1 handler looks up the dword at 4). EQClassic
 * Trade_Window_Struct (8) agrees; its CancelTrade_Struct uses u16 fields in the same 8 bytes. */
typedef struct MsgTradeRequest
{
    u32     to_id;              /* 0x00  */
    u32     from_id;            /* 0x04* */
} MsgTradeRequest;              /* 0x08 */
C_ASSERT(sizeof(MsgTradeRequest) == 0x08);

/* OP_MoneyTrade (0x203F), zone -> client. Verified against client (size 0x38 and fields): HWM
 * memcpy()s 0x38 bytes and credits the four coin counts from the named player. */
typedef struct MsgMoneyTrade
{
    char    name[40];           /* 0x00* */
    s32     platinum;           /* 0x28* */
    s32     gold;               /* 0x2C* */
    s32     silver;             /* 0x30* */
    s32     copper;             /* 0x34* */
} MsgMoneyTrade;                /* 0x38 */
C_ASSERT(sizeof(MsgMoneyTrade) == 0x38);

/* OP_DropCoin (0x2007), both ways. Verified against client (size 0x70: 0x004CBA05; fields: HWM
 * copies 0x70 bytes into an EQMoneyList node, which keeps its list links in the first 8 bytes,
 * and reads the coin counts, the position and the actor name). EQClassic DropCoins_Struct
 * agrees; ShowEQ dropCoinsStruct agrees on the size and the fields it names (amount, position,
 * name). */
typedef struct MsgDropCoins
{
    u8      unknown0x00[8];     /* 0x00  list links in the client's copy */
    s32     platinum;           /* 0x08* */
    s32     gold;               /* 0x0C* */
    s32     silver;             /* 0x10* */
    s32     copper;             /* 0x14* */
    u8      unknown0x18[4];     /* 0x18  */
    u32     always34;           /* 0x1C  */
    u8      unknown0x20[16];    /* 0x20  */
    s32     amount;             /* 0x30  */
    s16     unknown0x34;        /* 0x34  */
    s16     unknown0x36;        /* 0x36  */
    f32     y;                  /* 0x38* */
    f32     x;                  /* 0x3C* */
    f32     z;                  /* 0x40* */
    f32     unknown0x44;        /* 0x44* passed to CreateActor (heading?) */
    u8      unknown0x48[8];     /* 0x48  */
    char    actor_name[16];     /* 0x50* "PLAT_ACTORDEF" etc. */
    u8      unknown0x60[16];    /* 0x60  */
} MsgDropCoins;                 /* 0x70 */
C_ASSERT(sizeof(MsgDropCoins) == 0x70);

/* OP_DropItem (0x2035), both ways: an item lying on the ground. Verified against client (size 0xF0:
 * 0x004A2A0F, 0x004CB7B8; fields: HWM copies 0xF0 bytes into an EQItemList node and reads the
 * position, heading and actor name). EQClassic Object_Struct agrees (240 bytes); ShowEQ
 * dropThingOnGround agrees on the size and the fields it names (item number, drop id, position,
 * actor name). */
typedef struct MsgGroundItem
{
    u8      unknown0x00[8];     /* 0x00  list links in the client's copy */
    u32     item_nr;            /* 0x08  */
    u32     drop_id;            /* 0x0C  */
    u8      unknown0x10[24];    /* 0x10  */
    f32     y;                  /* 0x28* */
    f32     x;                  /* 0x2C* */
    f32     z;                  /* 0x30* */
    f32     heading;            /* 0x34* */
    char    actor_name[16];     /* 0x38* */
    u8      unknown0x48[14];    /* 0x48  */
    s16     items_in_bag[10];   /* 0x56  */
    u8      unknown0x6A[6];     /* 0x6A  */
    s16     cost;               /* 0x70  */
    u8      unknown0x72[4];     /* 0x72  */
    u8      stack_size;         /* 0x76  */
    u8      unknown0x77;        /* 0x77  */
    s16     click_effect;       /* 0x78  */
    u8      unknown0x7A[5];     /* 0x7A  */
    EQ_ItemProperties bag_props[10]; /* 0x7F */
    u8      unknown0xE3;        /* 0xE3  */
    u8      weight;             /* 0xE4  */
    u8      unknown0xE5[7];     /* 0xE5  */
    s16     icon_nr;            /* 0xEC  */
    s16     type;               /* 0xEE  */
} MsgGroundItem;                /* 0xF0 */
C_ASSERT(sizeof(MsgGroundItem) == 0xF0);
C_ASSERT(offsetof(MsgGroundItem, y) == 0x28);
C_ASSERT(offsetof(MsgGroundItem, actor_name) == 0x38);

/* OP_PickupItem (0x2036, click on a ground item) and OP_PickupCoin (0x2008). Verified against
 * client (size 8: 0x004CBA05; fields: the handlers look up the drop id at 0 and compare the dword
 * at 4 with the local spawn id). EQClassic ClickObject_Struct agrees. */
typedef struct MsgClickObject
{
    u32     drop_id;            /* 0x00* */
    u32     player_id;          /* 0x04* */
} MsgClickObject;               /* 0x08 */
C_ASSERT(sizeof(MsgClickObject) == 0x08);

/* OP_CraftingStation (0x20D7): open a world container. Verified against client (size 0x14:
 * 0x004176A0; fields: HWM jump case 0x33 reads the player id 0, the drop id 4, the open flag 8, the
 * type 0xA, the slot count 0xC and the icon 0x10).
 * DISCREPANCY: EQClassic ClickObjectAck_Struct is 22 bytes (4 more at the end) and says the icon is
 * at 0xE (its own layout puts it at 0x10). */
typedef struct MsgClickObjectAck
{
    u32     player_id;          /* 0x00* */
    u32     drop_id;            /* 0x04* */
    s16     open;               /* 0x08* 0 = someone else is using it */
    u8      type;               /* 0x0A* -> EQ_Container.bag_type */
    u8      unknown0x0B;        /* 0x0B  */
    u8      slots;              /* 0x0C* */
    u8      unknown0x0D[3];     /* 0x0D  */
    s16     icon_nr;            /* 0x10* */
    u8      unknown0x12[2];     /* 0x12  */
} MsgClickObjectAck;            /* 0x14 */
C_ASSERT(sizeof(MsgClickObjectAck) == 0x14);

/* OP_ConsumeFoodDrink (0x2156), client -> zone. Verified against client (size 0x10):
 * EQ_Character::AutoEat 0x00428193, RightClickItem 0x0046AB71. Fields from EQClassic
 * Consume_Struct. */
typedef struct MsgConsume
{
    u32     slot;               /* 0x00  */
    s32     auto_consumed;      /* 0x04  -1 = auto, 222 = right click */
    u8      unknown0x08[4];     /* 0x08  */
    u8      type;               /* 0x0C  1 = food, 2 = drink */
    u8      unknown0x0D[3];     /* 0x0D  */
} MsgConsume;                   /* 0x10 */
C_ASSERT(sizeof(MsgConsume) == 0x10);

/* OP_ConsumeItem (0x2146), both ways. Verified against client (size 0xC: 0x0042520E etc.; fields:
 * the handler passes the three dwords to 0x00491DD7). EQClassic ConsumeItem_Struct agrees on size
 * (u16 slot + 2 + two dwords). */
typedef struct MsgConsumeItem
{
    s32     slot;               /* 0x00* */
    s32     unknown0x04;        /* 0x04* */
    s32     unknown0x08;        /* 0x08* */
} MsgConsumeItem;               /* 0x0C */
C_ASSERT(sizeof(MsgConsumeItem) == 0x0C);

/* OP_ClientError (0x2147), client -> zone: the client reports an item it rejected. Verified
 * against client (size 0x5C; fields 0..8): sent from many item handlers with -2, -2, reason. */
typedef struct MsgClientError
{
    s32     unknown0x00;        /* 0x00* -2, or -4 with a count at 4 */
    s32     unknown0x04;        /* 0x04* */
    s32     reason;             /* 0x08* 1 bogus item, 2 bad charges, 3.. slot errors, 1000 busy */
    u8      unknown0x0C[80];    /* 0x0C  */
} MsgClientError;               /* 0x5C */
C_ASSERT(sizeof(MsgClientError) == 0x5C);

/* ---- skills and abilities ------------------------------------------------------------------ */

/* OP_BindWound (0x2093), both ways. Verified against client (size 8: 0x0041D790; field: HWM
 * switches on the s16 at 4). EQClassic BindWound_Struct agrees. */
typedef struct MsgBindWound
{
    u16     to;                 /* 0x00  */
    u16     unknown0x02;        /* 0x02  */
    s16     type;               /* 0x04* 0/1 done, 3 unlock, 4..7 failures */
    u16     unknown0x06;        /* 0x06  */
} MsgBindWound;                 /* 0x08 */
C_ASSERT(sizeof(MsgBindWound) == 0x08);

/* OP_Beg (0x2125), both ways. Verified against client (size 0x12: 0x0041D790; fields: 0x004942E4
 * reads the result at 9 and the coins at 0xC). EQClassic Beg_Struct agrees. */
typedef struct MsgBeg
{
    u32     target;             /* 0x00  */
    u32     begger;             /* 0x04  */
    u8      skill;              /* 0x08  */
    s8      success;            /* 0x09* 1..4 = platinum..copper */
    u16     time;               /* 0x0A  */
    s32     coins;              /* 0x0C* */
    u8      unknown0x10[2];     /* 0x10  */
} MsgBeg;                       /* 0x12 */
C_ASSERT(sizeof(MsgBeg) == 0x12);

/* OP_PickPockets (0x20AD), both ways. Verified against client (size 0x12): 0x0041D790. Fields from
 * EQClassic PickPockets_Struct. */
typedef struct MsgPickPockets
{
    u16     to;                 /* 0x00  */
    u16     from;               /* 0x02  */
    u8      skill;              /* 0x04  */
    u8      unknown0x05;        /* 0x05  */
    u8      type;               /* 0x06  */
    u8      unknown0x07;        /* 0x07  */
    u32     coin;               /* 0x08  */
    u8      unknown0x0C[6];     /* 0x0C  */
} MsgPickPockets;               /* 0x12 */
C_ASSERT(sizeof(MsgPickPockets) == 0x12);

/* OP_CombatAbility (0x215F), client -> zone. Verified against client (size 0xC): 0x004A4327.
 * Fields from EQClassic CombatAbility_Struct. */
typedef struct MsgCombatAbility
{
    u32     target_id;          /* 0x00  */
    u32     attack;             /* 0x04  */
    u32     type;               /* 0x08  */
} MsgCombatAbility;             /* 0x0C */
C_ASSERT(sizeof(MsgCombatAbility) == 0x0C);

/* OP_TradeSkillCombine (0x2105), client -> zone. Verified against client (size 0x20): 0x0041800A.
 * Fields from EQClassic Combine_Struct. */
typedef struct MsgCombine
{
    u8      world_object_type;  /* 0x00  */
    u8      unknown0x01[3];     /* 0x01  */
    s16     container_slot;     /* 0x04  1000 = world container */
    s16     item_in_slot[10];   /* 0x06  */
    s16     unknown0x1A;        /* 0x1A  */
    s16     unknown0x1C;        /* 0x1C  */
    s16     container_id;       /* 0x1E  */
} MsgCombine;                   /* 0x20 */
C_ASSERT(sizeof(MsgCombine) == 0x20);

/* OP_ClassTraining (0x209C), zone -> client. Verified against client (fields: 0x00493581 reads the
 * trainer id 0, 74 skill caps from 8, a dword at 0x6C, the open flag at 0x70 and 24 language caps
 * from 0x71). Size from EQClassic ClassTrain_Struct (148), whose names agree. */
typedef struct MsgClassTrain
{
    u32     npc_id;             /* 0x00* */
    u32     player_id;          /* 0x04  */
    u8      skill_max[74];      /* 0x08* */
    u8      unknown0x52[26];    /* 0x52  */
    u32     unknown0x6C;        /* 0x6C* */
    u8      open;               /* 0x70* 0 = refused */
    u8      language_max[24];   /* 0x71* */
    u8      unknown0x89[11];    /* 0x89  */
} MsgClassTrain;                /* 0x94 */
C_ASSERT(sizeof(MsgClassTrain) == 0x94);

/* OP_ClassTrainSkill (0x2140), client -> zone. Verified against client (size 0xC): 0x00464468.
 * Fields from EQClassic ClassSkillChange_Struct. */
typedef struct MsgClassSkillChange
{
    u32     npc_id;             /* 0x00  */
    u32     skill_type;         /* 0x04  0 = skill, 1 = language */
    u32     skill_id;           /* 0x08  */
} MsgClassSkillChange;          /* 0x0C */
C_ASSERT(sizeof(MsgClassSkillChange) == 0x0C);

/* OP_UseDiscipline (0x21E6), client -> zone. Verified against client (size 4): 0x004A58F5.
 * EQClassic UseDiscipline_Struct agrees. */
typedef struct MsgUseDiscipline
{
    u8      discipline;         /* 0x00  */
    u8      unknown0x01[3];     /* 0x01  */
} MsgUseDiscipline;             /* 0x04 */
C_ASSERT(sizeof(MsgUseDiscipline) == 0x04);

/* ---- groups, guilds, social -------------------------------------------------------------------- */

/* OP_GroupInvite (0x2040). Verified against client (size 0x5B): 0x004CAF5E. EQClassic
 * GroupInvite_Struct agrees. */
typedef struct MsgGroupInvite
{
    char    invited[30];        /* 0x00  */
    char    inviter[30];        /* 0x1E  */
    u8      unknown0x3C[31];    /* 0x3C  */
} MsgGroupInvite;               /* 0x5B */
C_ASSERT(sizeof(MsgGroupInvite) == 0x5B);

/* OP_GroupFollow (0x2042). Verified against client (size 0x3C): 0x004CB362. EQClassic
 * GroupFollow_Struct agrees. */
typedef struct MsgGroupFollow
{
    char    leader[30];         /* 0x00  */
    char    invited[30];        /* 0x1E  */
} MsgGroupFollow;               /* 0x3C */
C_ASSERT(sizeof(MsgGroupFollow) == 0x3C);

/* OP_GroupDeclineInvite (0x2041). Verified against client (size 0x3D): 0x004CB248, 0x004CB4FA.
 * EQClassic GroupInviteDecline_Struct agrees. */
typedef struct MsgGroupDecline
{
    char    leader[30];         /* 0x00  */
    char    leaver[30];         /* 0x1E  */
    u8      action;             /* 0x3C  */
} MsgGroupDecline;              /* 0x3D */
C_ASSERT(sizeof(MsgGroupDecline) == 0x3D);

/* OP_GroupQuit (0x2044). Verified against client (size 0x3C): 0x004CB4FA. EQClassic
 * GroupDisband_Struct agrees. */
typedef struct MsgGroupDisband
{
    char    member[15];         /* 0x00  */
    u8      unknown0x0F[41];    /* 0x0F  */
    u8      frame_counter;      /* 0x38  */
    u8      unknown0x39[3];     /* 0x39  */
} MsgGroupDisband;              /* 0x3C */
C_ASSERT(sizeof(MsgGroupDisband) == 0x3C);

/* One guild, 0x3C bytes: OP_GuildList (0x2192: 4-byte header + n entries, at most 512) and
 * OP_GuildUpdate (0x217B: guild index + entry). Verified against client (entry size 0x3C: HWM
 * divides (size - 4) by 0x3C and keeps a 512 x 0x3C table at 0x006E5490, whose name is read at
 * +4). EQClassic GuildsListEntry_Struct agrees. */
typedef struct GuildListEntry
{
    u32     guild_id;           /* 0x00  0xFFFFFFFF = empty */
    char    name[32];           /* 0x04* */
    u8      unknown0x24[4];     /* 0x24  */
    u8      exists;             /* 0x28  */
    u8      unknown0x29[7];     /* 0x29  */
    u8      unknown0x30[4];     /* 0x30  */
    u8      unknown0x34[8];     /* 0x34  */
} GuildListEntry;               /* 0x3C */
C_ASSERT(sizeof(GuildListEntry) == 0x3C);

typedef struct MsgGuildUpdate
{
    s32     index;              /* 0x00* 0..511 */
    GuildListEntry entry;       /* 0x04* */
} MsgGuildUpdate;               /* 0x40 */
C_ASSERT(sizeof(MsgGuildUpdate) == 0x40);

/* OP_GuildInvite (0x2117), OP_GuildInviteAccept (0x2118), OP_GuildRemove (0x2119). Verified
 * against client (size 0x44): 0x004C9EF6, 0x00491272, 0x004CA148. Fields from EQClassic
 * GuildInviteAccept_Struct (EQClassic GuildCommand_Struct is 64 bytes with a u16 guild id). */
typedef struct MsgGuildCommand
{
    char    name1[30];          /* 0x00  inviter / remover */
    char    name2[30];          /* 0x1E  */
    u32     rank;               /* 0x3C  */
    u32     guild_id;           /* 0x40  */
} MsgGuildCommand;              /* 0x44 */
C_ASSERT(sizeof(MsgGuildCommand) == 0x44);

/* OP_GuildWar (0x216F) / OP_GuildPeace (0x2191) and their replies (0x2193, 0x2194). Verified
 * against client (size 0x48: 0x004CB10B, 0x004CB1A5; fields: HWM reads the names at 0 and 0x1E,
 * the guild index at 0x3C, a dword at 0x40 and the result at 0x44). No reference layout. */
typedef struct MsgGuildWarPeace
{
    char    name1[30];          /* 0x00* */
    char    name2[30];          /* 0x1E* */
    s32     guild_index;        /* 0x3C* */
    s32     unknown0x40;        /* 0x40* */
    s32     result;             /* 0x44* -1 busy, -2 denied, -5 failed */
} MsgGuildWarPeace;             /* 0x48 */
C_ASSERT(sizeof(MsgGuildWarPeace) == 0x48);

/* OP_Surname (0x21C4), both ways. Verified against client (size 0x38: 0x004818CB; fields: HWM
 * reads the approval dword at 0x20 and the surname at 0x24). EQClassic Surname_Struct agrees on
 * size and the surname; the approval flag sits in its s_unknown1. */
typedef struct MsgSurname
{
    char    name[16];           /* 0x00  */
    u8      unknown0x10[16];    /* 0x10  */
    u32     approved;           /* 0x20* */
    char    last_name[20];      /* 0x24* */
} MsgSurname;                   /* 0x38 */
C_ASSERT(sizeof(MsgSurname) == 0x38);

/* OP_GMSurname (0x216E) and OP_GMNameChange (0x20CB). Verified against client (size 0x5E:
 * 0x004A4C6D, 0x004A4D37; fields: HWM looks up the name at 0 and copies the new name from 0x3C when
 * the s16 at 0x5C is set). EQClassic GMSurname_Struct and GMName_Struct agree on size. */
typedef struct MsgGMName
{
    char    name[30];           /* 0x00* */
    char    gm_name[30];        /* 0x1E  */
    char    new_name[32];       /* 0x3C* */
    s16     flag;               /* 0x5C* */
} MsgGMName;                    /* 0x5E */
C_ASSERT(sizeof(MsgGMName) == 0x5E);

/* OP_GMKill (0x206C), OP_GMKick (0x206D), OP_GMDelCorpse (0x21E9). Verified against client (size
 * 0x3D): 0x004A51D6, 0x004A4EBB, 0x004A52E1. EQClassic GMKill/GMKick/GMDelCorpse_Struct agree. */
typedef struct MsgGMKill
{
    char    name[30];           /* 0x00* */
    char    gm_name[30];        /* 0x1E  */
    u8      unknown0x3C;        /* 0x3C  */
} MsgGMKill;                    /* 0x3D */
C_ASSERT(sizeof(MsgGMKill) == 0x3D);

/* OP_GMSummon (0x20C5), OP_GMGoto (0x206E), OP_GMFindPlayer (0x2069). Verified against client
 * (size 0x6C: 0x004A6658, 0x004A920D, 0x004A8721; fields: HWM reads the found flag at 0x3C, the
 * zone at 0x3D and integer y/x/z at 0x5C). EQClassic GMSummon_Struct agrees; its GMGoto_Struct has
 * the same size with the fields unnamed. */
typedef struct MsgGMSummon
{
    char    char_name[30];      /* 0x00* */
    char    gm_name[30];        /* 0x1E  */
    u8      found;              /* 0x3C* */
    char    zone[15];           /* 0x3D* */
    u8      unknown0x4C[16];    /* 0x4C  */
    s32     y;                  /* 0x5C* */
    s32     x;                  /* 0x60* */
    s32     z;                  /* 0x64* */
    u8      unknown0x68[4];     /* 0x68  */
} MsgGMSummon;                  /* 0x6C */
C_ASSERT(sizeof(MsgGMSummon) == 0x6C);

/* OP_GMZoneRequest (0x214F), both ways. Verified against client (size 0x54: 0x004A50D0; fields:
 * HWM compares the name at 0 and the zone at 0x20, reads y/x/z/heading at 0x40 and the success
 * dword at 0x50).
 * DISCREPANCY: EQClassic GMZoneRequest_Struct has the right size but the zone at 0x1E and no
 * coordinates. */
typedef struct MsgGMZoneRequest
{
    char    char_name[32];      /* 0x00* */
    char    zone[32];           /* 0x20* */
    f32     y;                  /* 0x40* */
    f32     x;                  /* 0x44* */
    f32     z;                  /* 0x48* */
    f32     heading;            /* 0x4C* */
    u32     success;            /* 0x50* */
} MsgGMZoneRequest;             /* 0x54 */
C_ASSERT(sizeof(MsgGMZoneRequest) == 0x54);

/* OP_GMEmoteZone (0x21E3). Verified against client (size 0x200): 0x004A9FF5. EQClassic agrees. */
typedef struct MsgGMEmoteZone
{
    char    text[512];          /* 0x000 */
} MsgGMEmoteZone;               /* 0x200 */
C_ASSERT(sizeof(MsgGMEmoteZone) == 0x200);

/* OP_WhoAll (0x20F4), client -> zone. Verified against client (size 0x4C): 0x004CA5C7. Fields
 * from EQClassic Who_All_Struct. */
typedef struct MsgWhoAll
{
    char    whom[32];           /* 0x00  */
    s16     race;               /* 0x20  -1 = any */
    s16     class_;             /* 0x22  */
    s16     level_low;          /* 0x24  */
    s16     level_high;         /* 0x26  */
    s16     gm_lookup;          /* 0x28  */
    u8      unknown0x2A[34];    /* 0x2A  */
} MsgWhoAll;                    /* 0x4C */
C_ASSERT(sizeof(MsgWhoAll) == 0x4C);

/* OP_LFG (0x21F0). Verified against client (size 0x24): 0x004A8A32. EQClassic LFG_Struct agrees. */
typedef struct MsgLFG
{
    char    name[32];           /* 0x00  */
    u32     value;              /* 0x20  */
} MsgLFG;                       /* 0x24 */
C_ASSERT(sizeof(MsgLFG) == 0x24);

/* OP_Random (0x21E7). Verified against client (size 8): 0x004A6960. EQClassic Random_Struct and
 * ShowEQ randomStruct agree. */
typedef struct MsgRandom
{
    u32     low;                /* 0x00  */
    u32     high;               /* 0x04  */
} MsgRandom;                    /* 0x08 */
C_ASSERT(sizeof(MsgRandom) == 0x08);

/* OP_SetDataRate (0x21E8). Verified against client (size 4, a float 2..10 from DataRate.txt):
 * 0x004D4C19. EQClassic SetDataRate_Struct agrees. */
typedef struct MsgSetDataRate
{
    f32     rate;               /* 0x00* */
} MsgSetDataRate;               /* 0x04 */
C_ASSERT(sizeof(MsgSetDataRate) == 0x04);

/* OP_SetServerFilter (0x21FF). Verified against client (size 0x3C): 0x004CF829. EQClassic
 * SetServerFilter_Struct agrees (60 unknown bytes). */
typedef struct MsgSetServerFilter
{
    u8      filters[60];        /* 0x00  */
} MsgSetServerFilter;           /* 0x3C */
C_ASSERT(sizeof(MsgSetServerFilter) == 0x3C);

/* OP_RequestDuel (0x20CF). Verified against client (size 8): 0x004AA8DA. EQClassic Duel_Struct
 * agrees. */
typedef struct MsgDuel
{
    u32     initiator;          /* 0x00  */
    u32     target;             /* 0x04  */
} MsgDuel;                      /* 0x08 */
C_ASSERT(sizeof(MsgDuel) == 0x08);

/* OP_DuelResponse (0x20D0). Verified against client (size 0xC: 0x00496598, 0x004AA817; fields: HWM
 * looks up the id at 4 and switches on the byte at 8).
 * DISCREPANCY: EQClassic DuelResponse_Struct is 10 bytes (2-byte response). */
typedef struct MsgDuelResponse
{
    u32     target_id;          /* 0x00  */
    u32     entity_id;          /* 0x04* */
    u8      response;           /* 0x08* 0 declined, 1 busy, 2 considering */
    u8      unknown0x09[3];     /* 0x09  */
} MsgDuelResponse;              /* 0x0C */
C_ASSERT(sizeof(MsgDuelResponse) == 0x0C);

/* OP_InspectRequest (0x20B5). Verified against client (size 8): 0x004D9CAC. EQClassic
 * InspectRequest_Struct agrees. */
typedef struct MsgInspectRequest
{
    u32     target_id;          /* 0x00  */
    u32     player_id;          /* 0x04  */
} MsgInspectRequest;            /* 0x08 */
C_ASSERT(sizeof(MsgInspectRequest) == 0x08);

/* OP_InspectAnswer (0x20B6). Verified against client (size 0x414 and fields): HWM jump case 0x26
 * builds it (22 item names of 32 bytes from 8, 22 icons at 0x2C8, text at 0x2F4 and 0x3F4) and
 * case 0x27 copies 0x414 bytes. EQClassic InspectAnswer_Struct has the size only.
 * ShowEQ inspectingStruct agrees from 0x28 on (it treats the first name slot as unknown and has
 * 21 icons). */
typedef struct MsgInspectAnswer
{
    u32     target_id;          /* 0x000 */
    u32     player_id;          /* 0x004* */
    char    item_names[22][32]; /* 0x008* */
    s16     icons[22];          /* 0x2C8* -1 = empty */
    char    text[256];          /* 0x2F4* */
    char    text2[32];          /* 0x3F4* CharacterProfile 0xA78 */
} MsgInspectAnswer;             /* 0x414 */
C_ASSERT(sizeof(MsgInspectAnswer) == 0x414);

/* OP_RezzRequest (0x212A) / OP_RezzAnswer (0x219B). Verified against client (size 0xA0):
 * EQ_Character::HitBySpell 0x00420505. Fields from EQClassic Resurrect_Struct. */
typedef struct MsgResurrect
{
    u32     corpse_id;          /* 0x00  */
    char    zone[16];           /* 0x04  */
    u8      unknown0x14[16];    /* 0x14  */
    f32     y;                  /* 0x24  */
    f32     x;                  /* 0x28  */
    f32     z;                  /* 0x2C  */
    u32     full_gm_rez;        /* 0x30  */
    char    target_name[28];    /* 0x34  */
    u8      unknown0x50[8];     /* 0x50  */
    char    caster_name[28];    /* 0x58  */
    u8      unknown0x74[4];     /* 0x74  */
    u16     spell_id;           /* 0x78  */
    char    corpse_name[28];    /* 0x7A  */
    u8      unknown0x96[6];     /* 0x96  */
    u32     action;             /* 0x9C  1 = yes */
} MsgResurrect;                 /* 0xA0 */
C_ASSERT(sizeof(MsgResurrect) == 0xA0);

/* OP_Sacrifice (0x21EA). Verified against client (size 0xC): 0x00481474. EQClassic
 * Sacrifice_Struct agrees. */
typedef struct MsgSacrifice
{
    u32     caster_id;          /* 0x00  */
    u32     target_id;          /* 0x04  */
    u32     action;             /* 0x08  */
} MsgSacrifice;                 /* 0x0C */
C_ASSERT(sizeof(MsgSacrifice) == 0x0C);

/* OP_QuestCompletedMoney (0x2080), zone -> client. Verified against client (fields: HWM jump case
 * 0x18 reads the NPC id 0, a dword at 8 and the coins at 0x14..0x20). EQClassic
 * QuestCompletedMoney_Struct agrees (36 bytes). */
typedef struct MsgQuestReward
{
    u32     npc_id;             /* 0x00* */
    u32     unknown0x04;        /* 0x04  */
    u32     unknown0x08;        /* 0x08* */
    u8      unknown0x0C[8];     /* 0x0C  */
    s32     copper;             /* 0x14* */
    s32     silver;             /* 0x18* */
    s32     gold;               /* 0x1C* */
    s32     platinum;           /* 0x20* */
} MsgQuestReward;               /* 0x24 */
C_ASSERT(sizeof(MsgQuestReward) == 0x24);

/* ---- world objects ----------------------------------------------------------------------------- */

/* A door or other switch, 0x30 bytes: OP_SpawnDoor (0x2095) sends one, OP_CompressedDoorSpawn
 * (0x21F7) sends a u16 count and compressed doors. Verified against client (size: HWM checks the
 * inflated length against count * 0x30 and steps by 0x30). Fields from EQClassic Door_Struct up to
 * 0x2E (EQClassic's struct goes on with server-side fields); ShowEQ doorStruct has the size and the
 * name only. */
typedef struct NetDoor
{
    char    name[16];           /* 0x00  */
    f32     y;                  /* 0x10  */
    f32     x;                  /* 0x14  */
    f32     z;                  /* 0x18  */
    f32     heading;            /* 0x1C  */
    f32     incline;            /* 0x20  */
    f32     unknown0x24;        /* 0x24  */
    u8      door_id;            /* 0x28  */
    u8      open_type;          /* 0x29  */
    u8      is_open;            /* 0x2A  */
    u8      inverted;           /* 0x2B  */
    s16     parameter;          /* 0x2C  */
    u8      unknown0x2E[2];     /* 0x2E  */
} NetDoor;                      /* 0x30 */
C_ASSERT(sizeof(NetDoor) == 0x30);

/* OP_OpenDoor (0x208E), zone -> client. Verified against client (fields: HWM uses the byte at 0 as
 * the door id and the byte at 1 as the new state). EQClassic DoorOpen_Struct agrees. */
typedef struct MsgDoorOpen
{
    u8      door_id;            /* 0x00* */
    u8      action;             /* 0x01* */
} MsgDoorOpen;                  /* 0x02 */
C_ASSERT(sizeof(MsgDoorOpen) == 0x02);

/* OP_CommandBoat (0x2126). Verified against client (size 8): 0x00415EFB, 0x004D6F06. EQClassic
 * CommandBoat_Struct agrees (8 unknown bytes). */
typedef struct MsgCommandBoat
{
    u8      unknown0x00[8];     /* 0x00  */
} MsgCommandBoat;               /* 0x08 */
C_ASSERT(sizeof(MsgCommandBoat) == 0x08);

/* ---- message boards ---------------------------------------------------------------------------- */

/* OP_MBSendMessageToClient (0x210A) and OP_MBPostMessage (0x210C): header followed by the text.
 * Verified against client (fields id 0, date 4, author 0x14, language 0x34, subject 0x36: HWM jump
 * case 0x3C; post length = text + 0x59: 0x0045CB52). EQClassic MBMessage_Struct agrees (with a
 * 2048-byte text). */
typedef struct MsgMBMessage
{
    u32     id;                 /* 0x00* */
    char    date[10];           /* 0x04* */
    u8      unknown0x0E[6];     /* 0x0E  */
    char    author[30];         /* 0x14* */
    u8      unknown0x32[2];     /* 0x32  */
    u8      language;           /* 0x34* */
    u8      unknown0x35;        /* 0x35  */
    char    subject[29];        /* 0x36* */
    u8      unknown0x53[3];     /* 0x53  */
    u8      category;           /* 0x56  */
    u8      unknown0x57;        /* 0x57  */
    char    message[1];         /* 0x58  really variable */
} MsgMBMessage;                 /* 0x59 */
C_ASSERT(sizeof(MsgMBMessage) == 0x59);

/* OP_MBRetrieveMessages (0x2108). Verified against client (size 8): 0x00415EFB. EQClassic
 * MBRetrieveMessages_Struct agrees. */
typedef struct MsgMBRetrieveMessages
{
    u8      entity_id;          /* 0x00  */
    u8      unknown0x01[3];     /* 0x01  */
    u8      category;           /* 0x04  */
    u8      unknown0x05[3];     /* 0x05  */
} MsgMBRetrieveMessages;        /* 0x08 */
C_ASSERT(sizeof(MsgMBRetrieveMessages) == 0x08);

/* OP_MBRetrieveMessage (0x2109) and OP_MBEraseMessage (0x210D). Verified against client (size 0xC):
 * 0x0045CB52. EQClassic MBRetrieveMessage_Struct / MBEraseMessage_Struct agree. */
typedef struct MsgMBMessageRef
{
    u32     entity_id;          /* 0x00  */
    u32     id;                 /* 0x04  */
    u8      category;           /* 0x08  */
    u8      unknown0x09[3];     /* 0x09  */
} MsgMBMessageRef;              /* 0x0C */
C_ASSERT(sizeof(MsgMBMessageRef) == 0x0C);

#pragma pack(pop)

#endif /* TRILOGY_NET_STRUCTS_H */
