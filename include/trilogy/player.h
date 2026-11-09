/*
 * player.h - spawns: the EQPlayer object, the zone-entry image of it, the 168-byte network spawn
 * record and the packed position/velocity block.
 */
#ifndef TRILOGY_PLAYER_H
#define TRILOGY_PLAYER_H

#include "common.h"

#pragma pack(push, 1)

/* Position and motion as sent on the wire, 13 bytes. Verified against client: pack_physics
 * (0x004A3534) and unpack_physics (0x004A3717).
 * DISCREPANCY: EQClassic and ShowEQ both describe the last dword as deltaY:10, spacer:1,
 * deltaZ:10, spacer:1, deltaX:10. The client packs three signed fields of 11, 11 and 10 bits
 * with no spacers: dy = bits 0-10, dz = bits 11-21, dx = bits 22-31, each in 1/16 units
 * (pack_physics clamps dy and dz to -64..63 and dx to -32..31). The client does the packing
 * by hand (shifts and masks), so this is a plain u32 here. */
typedef struct PackedPhysics
{
    s8      speed;              /* 0x00  animation speed * 10 (read unsigned for NPCs) */
    u8      heading;            /* 0x01  heading / 2 (0..255 = 0..510) */
    s8      delta_heading;      /* 0x02  */
    s16     y;                  /* 0x03  */
    s16     x;                  /* 0x05  */
    s16     z;                  /* 0x07  z * 10 */
    u32     deltas;             /* 0x09  see PHYS_* below */
} PackedPhysics;                /* 0x0D */
C_ASSERT(sizeof(PackedPhysics) == 0x0D);

#define PHYS_DY_BITS(d)     ((d) & 0x7FF)             /* 11-bit signed, 1/16 */
#define PHYS_DZ_BITS(d)     (((d) >> 11) & 0x7FF)     /* 11-bit signed, 1/16 */
#define PHYS_DX_BITS(d)     ((d) >> 22)               /* 10-bit signed, 1/16 */

/* The renderer-side data of a spawn, EQPlayer.actor_info. Partial: offsets are what the client reads.
 * Verified against client: CDisplay::PutPlayerOnFloor (0x00414E7A) compares and adds the float at
 * +0x48 (FCOMP/FADD [info+0x48]); CDisplay::SetPlayerConstantAmbient_guess (0x004122EA) reads the
 * hierarchical sprite at +0x250; get_melee_range (0x004A491F) and CDisplay::SetPlayerPitchType
 * (0x00407FF1) pass the dword at +0 to the t3d actor functions. Names are guesses; size unknown, so no
 * sizeof check: do not instantiate or take sizeof(). */
struct EQPlayer;
struct EQ_Spell_guess;

typedef struct EQActorInfo_guess
{
    void*   actor;              /* 0x000* t3d actor handle (t3dSetActorSurfacePitchType,
                                             t3dGetScaledActorBoundingRadius) */
    void*   light_guess;        /* 0x004* */
    char    actorTag_guess[0x1E]; /* 0x008* */
    u8      unknown0x026[0x22]; /* 0x026  */
    f32     floor_z;            /* 0x048* floor height under the spawn */
    float   fallStartZ_guess;   /* 0x04C* */
    union
    {
        float   savedZ_guess;       /* 0x050* */
        u32     savedZ_bits_guess;  /* 0x050* */
    };
    u32     lastPhysicsTime_guess; /* 0x054* */
    u32     timeStamp58_guess;  /* 0x058* */
    u32     timeStamp5C_guess;  /* 0x05C* */
    u32     timeStamp60_guess;  /* 0x060* */
    u32     unk_0x64;           /* 0x064* */
    int     unk_0x68;           /* 0x068* */
    u32     unk_0x6C;           /* 0x06C* */
    u32     unk_0x70;           /* 0x070* */
    u32     lastSaveTime_guess; /* 0x074* */
    s16     unknown0x078;       /* 0x078* set to 1 by the levitate code (eqspa_levitate_guess 0x0042010B) */
    short   unk_0x7A;           /* 0x07A* */
    u16     unk_0x7C;           /* 0x07C* */
    short   unk_0x7E;           /* 0x07E* */
    union
    {
        float   unk_0x80;           /* 0x080* */
        u32     unk_0x80_bits_guess; /* 0x080* */
    };
    union
    {
        u32     unk_0x84;           /* 0x084* */
        float   unk_0x84_float_guess; /* 0x084* */
    };
    u8      unknown0x088;       /* 0x088* cleared by the levitate code (eqspa_levitate_guess 0x0042010B) */
    u8      unk_0x89;           /* 0x089* */
    u8      unknown0x08A[2];    /* 0x08A  */
    u32     unk_0x8C;           /* 0x08C* */
    u32     unk_0x90;           /* 0x090* */
    u32     unk_0x94;           /* 0x094* */
    u8      environment_guess;  /* 0x098* */
    u8      unknown0x099;       /* 0x099* animation? EQ_PC::ProcessFatigue (0x00480A94): 5..10 tire */
    u8      unk_0x9A;           /* 0x09A* */
    u8      unk_0x9B;           /* 0x09B* */
    u8      unk_0x9C;           /* 0x09C* */
    u8      unknown0x09D[3];    /* 0x09D  */
    union
    {
        float   horizontalFriction_guess; /* 0x0A0* */
        u32     horizontalFriction_bits_guess; /* 0x0A0* */
    };
    union
    {
        float   horizontalDrag_guess; /* 0x0A4* */
        u32     horizontalDrag_bits_guess; /* 0x0A4* */
    };
    u32     unk_0xA8;           /* 0x0A8* */
    union
    {
        float   unk_0xAC;           /* 0x0AC* */
        u32     unk_0xAC_bits_guess; /* 0x0AC* */
    };
    union
    {
        float   unk_0xB0;           /* 0x0B0* */
        u32     unk_0xB0_bits_guess; /* 0x0B0* */
    };
    float   actorScale_guess;   /* 0x0B4* */
    float   unk_0xB8;           /* 0x0B8* */
    float   unk_0xBC;           /* 0x0BC* */
    u32     stunTimer_guess;    /* 0x0C0* */
    u32     lastTimeCheck_guess; /* 0x0C4* */
    u32     unk_0xC8;           /* 0x0C8* */
    u32     unk_0xCC;           /* 0x0CC* */
    u32     unk_0xD0;           /* 0x0D0* */
    u32     unk_0xD4[0x22];     /* 0x0D4* */
    int     unk_0x15C;          /* 0x15C* */
    u32     lastAttackTime_guess; /* 0x160* */
    u32     unk_0x164;          /* 0x164* */
    u32     lastSwitchTouchTime_guess; /* 0x168* */
    u32     unk_0x16C;          /* 0x16C* */
    u32     unk_0x170;          /* 0x170* */
    u32     unk_0x174;          /* 0x174* */
    union
    {
        char*   anim_set_guess;     /* 0x178* animation set: byte +0x39C == 1 = has alternate tracks, one pointer per animation at +0x30 (toggleAnimationState 0x0048EBB3) */
        void*   anim_set_void_guess; /* 0x178* */
    };
    u32     unk_0x17C;          /* 0x17C* */
    int     animationId_guess;  /* 0x180* */
    u8      unk_0x184;          /* 0x184* */
    u8      unk_0x185;          /* 0x185* */
    u8      unk_0x186;          /* 0x186* */
    u8      unk_0x187;          /* 0x187* */
    u8      unk_0x188;          /* 0x188* */
    u8      unknown0x189;       /* 0x189* nonzero: CDisplay::DrawOverlay_guess (0x0040B492) draws nothing */
    u8      unk_0x18A;          /* 0x18A* */
    u8      unknown0x18B[1];    /* 0x18B  */
    struct EQPlayer* ride_target_guess; /* 0x18C* the spawn (vehicle) this one stands on: vehicle_premove /
                                             _postmove (0x004E0FC7 / 0x004E1109); cleared by 0x004B13C0 */
    u8      unknown0x190[8];    /* 0x190  */
    int     unk_0x198;          /* 0x198* */
    u32     lastActiveTime_guess; /* 0x19C* */
    union
    {
        float   unk_0x1A0;          /* 0x1A0* */
        u32     unk_0x1A0_bits_guess; /* 0x1A0* */
    };
    union
    {
        f32     sneak_mod_guess;    /* 0x1A4* 0.5, +0.1 per rogue level above 50 (EQPlayer::InitSneakMod 0x004BC963) */
        u32     sneak_mod_bits_guess; /* 0x1A4* */
    };
    u32     unk_0x1A8;          /* 0x1A8* */
    u32     unk_0x1AC;          /* 0x1AC* */
    u32     unk_0x1B0;          /* 0x1B0* */
    short   unk_0x1B4;          /* 0x1B4* */
    u8      unknown0x1B6[2];    /* 0x1B6  */
    u32     unk_0x1B8;          /* 0x1B8* */
    char    lastVictimName_guess[0x1E]; /* 0x1BC* */
    u8      unknown0x1DA[2];    /* 0x1DA  */
    u32     sound_id_guess;     /* 0x1DC* compared with the sound slots' id (0x004E2154) */
    u32     unk_0x1E0;          /* 0x1E0* */
    u32     unk_0x1E4;          /* 0x1E4* */
    u32     unk_0x1E8;          /* 0x1E8* */
    union
    {
        u32     unk_0x1EC;          /* 0x1EC* */
        short   unk_0x1EC_short_guess; /* 0x1EC* */
    };
    union
    {
        u32     unk_0x1F0;          /* 0x1F0* */
        short   unk_0x1F0_short_guess; /* 0x1F0* */
    };
    union
    {
        u32     unk_0x1F4;          /* 0x1F4* */
        short   unk_0x1F4_short_guess; /* 0x1F4* */
    };
    union
    {
        u32     unk_0x1F8;          /* 0x1F8* */
        short   unk_0x1F8_short_guess; /* 0x1F8* */
    };
    u32     unk_0x1FC;          /* 0x1FC* */
    u32     unk_0x200;          /* 0x200* */
    u32     unk_0x204;          /* 0x204* */
    int     unk_0x208;          /* 0x208* */
    int     unk_0x20C;          /* 0x20C* */
    u32     unk_0x210;          /* 0x210* */
    u32     unk_0x214;          /* 0x214* */
    int     unk_0x218;          /* 0x218* */
    u32     unk_0x21C;          /* 0x21C* */
    u32     unk_0x220;          /* 0x220* */
    u32     unk_0x224;          /* 0x224* */
    u32     sound_data_a_guess; /* 0x228* cleared when the sound manager forgets the spawn (0x004E221D) */
    u32     sound_data_b_guess; /* 0x22C* likewise */
    u32     unk_0x230;          /* 0x230* */
    int     unk_0x234;          /* 0x234* */
    int     unk_0x238;          /* 0x238* */
    u32     unk_0x23C;          /* 0x23C* */
    u32     unk_0x240;          /* 0x240* */
    int     unk_0x244;          /* 0x244* */
    s16     unknown0x248;       /* 0x248* -1 = none; EQ_Character::BardCastBard() (0x0046A72F) needs it set */
    u8      unk_0x24A;          /* 0x24A* */
    u8      unknown0x24B[1];    /* 0x24B  */
    u16     unk_0x24C;          /* 0x24C* */
    u8      unknown0x24E[2];    /* 0x24E  */
    union
    {
        void*   hier_sprite;        /* 0x250* hierarchical sprite (s3dGetNumSkinsAttachedToHierarchicalSprite) */
        int     hier_sprite_int_guess; /* 0x250* */
    };
    void*   head_point_dag;     /* 0x254* "%sHEAD_POINT_DAG" (UpdateDAGPointers 0x00410116) */
    void*   he_dag;             /* 0x258* "%sHE_DAG" */
    void*   name_sprite_guess;  /* 0x25C* cleared in UpdateDAGPointers; read by SetNameSpriteState/Tint */
    void*   attach_a_guess[2];  /* 0x260* attachment dags (ChangeAttachmentAnimationTrackSpeeds 0x0048FFAD) */
    void*   shield_point_dag;   /* 0x268* "%sSHIELD_POINT_DAG" */
    void*   bld_dag;            /* 0x26C* "%s_BLD_DAG" */
    void*   tunic_point_dag;    /* 0x270* "%sTUNIC_POINT_DAG" (alternate-track sets) */
    void*   attach_b_guess[2];  /* 0x274* */
    void*   chest_point_dag;    /* 0x27C* "%sCHEST_POINT_DAG" */
    void*   gauntl_point_dag;   /* 0x280* "%sGAUNTL_POINT_DAG" */
    void*   gauntr_point_dag;   /* 0x284* "%sGAUNTR_POINT_DAG" */
    void*   shouldl_point_dag;  /* 0x288* "%sSHOULDL_POINT_DAG" */
    void*   shouldr_point_dag;  /* 0x28C* "%sSHOULDR_POINT_DAG" */
    void*   legl_point_dag;     /* 0x290* "%sLEGL_POINT_DAG" */
    void*   legr_point_dag;     /* 0x294* "%sLEGR_POINT_DAG" */
    int     left_eye_material_guess; /* 0x298* eye palette index (SetEyeMaterial, left) */
    int     right_eye_material_guess; /* 0x29C* eye palette index (right); < 0 = none */
    u32     unk_0x2A0;          /* 0x2A0* */
    union
    {
        char*   follow_target_guess; /* 0x2A4* auto-follow target spawn (EQPlayer_AutoFollow_guess) */
        struct EQPlayer* follow_player_guess; /* 0x2A4* */
    };
    float   follow_heading_guess; /* 0x2A8* heading to the follow target (0x004A47F9) */
    u32     unk_0x2AC;          /* 0x2AC* */
    float   targetDistance_guess; /* 0x2B0* distance to the follow target (0x004129F3) */
    float   unk_0x2B4;          /* 0x2B4* */
    float   unk_0x2B8;          /* 0x2B8* */
    float   unk_0x2BC;          /* 0x2BC* */
    u8      unk_0x2C0;          /* 0x2C0* */
    u8      unk_0x2C1;          /* 0x2C1* */
    u8      unknown0x2C2[2];    /* 0x2C2  */
    u32     unk_0x2C4;          /* 0x2C4* */
    u32     unk_0x2C8;          /* 0x2C8* */
    u32     unk_0x2CC;          /* 0x2CC* */
    u32     unk_0x2D0;          /* 0x2D0* */
    float   unk_0x2D4;          /* 0x2D4* */
    u32     unk_0x2D8;          /* 0x2D8* */
    s32     unknown0x2DC;       /* 0x2DC* == 30 in the resist code (0x0041A962) */
    int     unk_0x2E0;          /* 0x2E0* to the 0x364 size (EQObject_ctor_guess 0x004BCDE8 memset) */
    int     unk_0x2E4;          /* 0x2E4* */
    u32     unk_0x2E8;          /* 0x2E8* */
    struct EQ_Item* castItem_guess;     /* 0x2EC* */
    int     castItemSlot_guess; /* 0x2F0* */
    int     unk_0x2F4;          /* 0x2F4* */
    u32     unk_0x2F8;          /* 0x2F8* */
    u32     unk_0x2FC;          /* 0x2FC* */
    int     unk_0x300;          /* 0x300* */
    u8      unk_0x304;          /* 0x304* */
    u8      unknown0x305[0x3F]; /* 0x305  */
    short   buffCasters_guess[0xF]; /* 0x344* */
    u8      unknown0x362[2];    /* 0x362  */
} EQActorInfo_guess;            /* size 0x364 */
C_ASSERT(offsetof(EQActorInfo_guess, actor) == 0x000);
C_ASSERT(offsetof(EQActorInfo_guess, floor_z) == 0x048);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x078) == 0x078);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x088) == 0x088);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x099) == 0x099);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x248) == 0x248);
C_ASSERT(offsetof(EQActorInfo_guess, hier_sprite) == 0x250);
C_ASSERT(offsetof(EQActorInfo_guess, anim_set_guess) == 0x178);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x189) == 0x189);
C_ASSERT(offsetof(EQActorInfo_guess, ride_target_guess) == 0x18C);
C_ASSERT(offsetof(EQActorInfo_guess, sneak_mod_guess) == 0x1A4);
C_ASSERT(offsetof(EQActorInfo_guess, sound_id_guess) == 0x1DC);
C_ASSERT(offsetof(EQActorInfo_guess, sound_data_a_guess) == 0x228);
C_ASSERT(offsetof(EQActorInfo_guess, attach_a_guess) == 0x260);
C_ASSERT(offsetof(EQActorInfo_guess, attach_b_guess) == 0x274);
C_ASSERT(offsetof(EQActorInfo_guess, unknown0x2DC) == 0x2DC);
C_ASSERT(offsetof(EQActorInfo_guess, head_point_dag) == 0x254);
C_ASSERT(offsetof(EQActorInfo_guess, he_dag) == 0x258);
C_ASSERT(offsetof(EQActorInfo_guess, name_sprite_guess) == 0x25C);
C_ASSERT(offsetof(EQActorInfo_guess, shield_point_dag) == 0x268);
C_ASSERT(offsetof(EQActorInfo_guess, bld_dag) == 0x26C);
C_ASSERT(offsetof(EQActorInfo_guess, tunic_point_dag) == 0x270);
C_ASSERT(offsetof(EQActorInfo_guess, chest_point_dag) == 0x27C);
C_ASSERT(offsetof(EQActorInfo_guess, gauntl_point_dag) == 0x280);
C_ASSERT(offsetof(EQActorInfo_guess, gauntr_point_dag) == 0x284);
C_ASSERT(offsetof(EQActorInfo_guess, shouldl_point_dag) == 0x288);
C_ASSERT(offsetof(EQActorInfo_guess, shouldr_point_dag) == 0x28C);
C_ASSERT(offsetof(EQActorInfo_guess, legl_point_dag) == 0x290);
C_ASSERT(offsetof(EQActorInfo_guess, legr_point_dag) == 0x294);
C_ASSERT(offsetof(EQActorInfo_guess, left_eye_material_guess) == 0x298);
C_ASSERT(offsetof(EQActorInfo_guess, right_eye_material_guess) == 0x29C);
C_ASSERT(offsetof(EQActorInfo_guess, follow_target_guess) == 0x2A4);
C_ASSERT(offsetof(EQActorInfo_guess, follow_heading_guess) == 0x2A8);
C_ASSERT(offsetof(EQActorInfo_guess, targetDistance_guess) == 0x2B0);
C_ASSERT(offsetof(EQActorInfo_guess, light_guess) == 0x004);
C_ASSERT(offsetof(EQActorInfo_guess, actorTag_guess) == 0x008);
C_ASSERT(offsetof(EQActorInfo_guess, fallStartZ_guess) == 0x04C);
C_ASSERT(offsetof(EQActorInfo_guess, savedZ_guess) == 0x050);
C_ASSERT(offsetof(EQActorInfo_guess, lastPhysicsTime_guess) == 0x054);
C_ASSERT(offsetof(EQActorInfo_guess, timeStamp58_guess) == 0x058);
C_ASSERT(offsetof(EQActorInfo_guess, timeStamp5C_guess) == 0x05C);
C_ASSERT(offsetof(EQActorInfo_guess, timeStamp60_guess) == 0x060);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x64) == 0x064);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x68) == 0x068);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x6C) == 0x06C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x70) == 0x070);
C_ASSERT(offsetof(EQActorInfo_guess, lastSaveTime_guess) == 0x074);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x7A) == 0x07A);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x7C) == 0x07C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x7E) == 0x07E);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x80) == 0x080);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x84) == 0x084);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x89) == 0x089);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x8C) == 0x08C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x90) == 0x090);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x94) == 0x094);
C_ASSERT(offsetof(EQActorInfo_guess, environment_guess) == 0x098);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x9A) == 0x09A);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x9B) == 0x09B);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x9C) == 0x09C);
C_ASSERT(offsetof(EQActorInfo_guess, horizontalFriction_guess) == 0x0A0);
C_ASSERT(offsetof(EQActorInfo_guess, horizontalDrag_guess) == 0x0A4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xA8) == 0x0A8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xAC) == 0x0AC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xB0) == 0x0B0);
C_ASSERT(offsetof(EQActorInfo_guess, actorScale_guess) == 0x0B4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xB8) == 0x0B8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xBC) == 0x0BC);
C_ASSERT(offsetof(EQActorInfo_guess, stunTimer_guess) == 0x0C0);
C_ASSERT(offsetof(EQActorInfo_guess, lastTimeCheck_guess) == 0x0C4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xC8) == 0x0C8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xCC) == 0x0CC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xD0) == 0x0D0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0xD4) == 0x0D4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x15C) == 0x15C);
C_ASSERT(offsetof(EQActorInfo_guess, lastAttackTime_guess) == 0x160);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x164) == 0x164);
C_ASSERT(offsetof(EQActorInfo_guess, lastSwitchTouchTime_guess) == 0x168);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x16C) == 0x16C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x170) == 0x170);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x174) == 0x174);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x17C) == 0x17C);
C_ASSERT(offsetof(EQActorInfo_guess, animationId_guess) == 0x180);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x184) == 0x184);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x185) == 0x185);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x186) == 0x186);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x187) == 0x187);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x188) == 0x188);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x18A) == 0x18A);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x198) == 0x198);
C_ASSERT(offsetof(EQActorInfo_guess, lastActiveTime_guess) == 0x19C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1A0) == 0x1A0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1A8) == 0x1A8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1AC) == 0x1AC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1B0) == 0x1B0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1B4) == 0x1B4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1B8) == 0x1B8);
C_ASSERT(offsetof(EQActorInfo_guess, lastVictimName_guess) == 0x1BC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1E0) == 0x1E0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1E4) == 0x1E4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1E8) == 0x1E8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1EC) == 0x1EC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1F0) == 0x1F0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1F4) == 0x1F4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1F8) == 0x1F8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x1FC) == 0x1FC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x200) == 0x200);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x204) == 0x204);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x208) == 0x208);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x20C) == 0x20C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x210) == 0x210);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x214) == 0x214);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x218) == 0x218);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x21C) == 0x21C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x220) == 0x220);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x224) == 0x224);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x230) == 0x230);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x234) == 0x234);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x238) == 0x238);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x23C) == 0x23C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x240) == 0x240);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x244) == 0x244);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x24A) == 0x24A);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x24C) == 0x24C);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2A0) == 0x2A0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2AC) == 0x2AC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2B4) == 0x2B4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2B8) == 0x2B8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2BC) == 0x2BC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2C0) == 0x2C0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2C1) == 0x2C1);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2C4) == 0x2C4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2C8) == 0x2C8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2CC) == 0x2CC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2D0) == 0x2D0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2D4) == 0x2D4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2D8) == 0x2D8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2E0) == 0x2E0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2E4) == 0x2E4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2E8) == 0x2E8);
C_ASSERT(offsetof(EQActorInfo_guess, castItem_guess) == 0x2EC);
C_ASSERT(offsetof(EQActorInfo_guess, castItemSlot_guess) == 0x2F0);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2F4) == 0x2F4);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2F8) == 0x2F8);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x2FC) == 0x2FC);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x300) == 0x300);
C_ASSERT(offsetof(EQActorInfo_guess, unk_0x304) == 0x304);
C_ASSERT(offsetof(EQActorInfo_guess, buffCasters_guess) == 0x344);

/* The EQPlayer object (one per spawn, including the local player). Verified against client:
 * CEverQuest::HandleWorldMessage allocates operator new(0x134) and passes the image below to
 * EQPlayer::EQPlayer (0x004B03EB); UnPackNetPlayer (0x004A39E3) fills a 0x134-byte image from a
 * NetSpawn (every field marked * is written there, or read by the code named); OP_PlayerUpdate
 * (0x202C) memcpy()s 0x134 bytes over a live EQPlayer keeping only the dwords at 0x64, 0x68,
 * 0x70 and 0x74. No vtable. Names from EQClassic ServerZoneEntry_Struct where it agrees (its
 * offsets are ours + 4, see MsgServerZoneEntry) and from what the client does with the field. */
typedef struct EQPlayer
{
    u8      objectKind_guess;        /* 0x00  */
    char    name[30];           /* 0x01* */
    char    zone[15];           /* 0x1F* printed as the zone name in the zone-change timeout */
    u8      unknown0x2E[6];     /* 0x2E  */
    union
    {
        f32     y;                  /* 0x34* PackedPhysics is unpacked into 0x34..0x5B */
        u32     y_bits_guess;       /* 0x34* */
    };
    union
    {
        f32     x;                  /* 0x38* */
        u32     x_bits_guess;       /* 0x38* */
    };
    union
    {
        f32     z;                  /* 0x3C* */
        u32     z_bits_guess;       /* 0x3C* */
    };
    f32     heading;            /* 0x40* */
    f32     pitch;              /* 0x44* cleared with the velocities; pitch for
                                            t3dCalculateAccurateCoefficientsFromHeadingPitchRoll (vehicles) */
    f32     speed;              /* 0x48* */
    f32     dy;                 /* 0x4C* */
    f32     dx;                 /* 0x50* */
    f32     dz;                 /* 0x54* */
    f32     delta_heading;      /* 0x58* */
    f32     unk_0x5C;        /* 0x5C  cleared with the velocities */
    union
    {
        u32     unk_0x60;        /* 0x60  */
        f32     pitchState_guess;   /* 0x60* */
    };
    union
    {
        struct EQPlayer* previous_guess; /* 0x64* spawn list back link (EQPlayer.cpp list insert/unlink), kept by OP_PlayerUpdate */
        u32     previous_bits_guess; /* 0x64* */
    };
    struct EQPlayer* next;      /* 0x68* live, kept by OP_PlayerUpdate: the spawn list link (EQPlayer::top
                                            0x006E4D58, walked by get_player 0x004B1435 and others) */
    u32     unknown0x6C;        /* 0x6C  */
    EQActorInfo_guess *actor_info; /* 0x70* renderer/actor data; live, kept by OP_PlayerUpdate */
    void*   char_info_guess;    /* 0x74* the spawn's EQ_Character (local player); live, kept by OP_PlayerUpdate:
                                            ~EQ_Character (0x004197FF) clears it, EQPlayer::EQPlayer
                                            calls EQ_Character::DoLight through it */
    union
    {
        u32     unk_0x78;        /* 0x78  */
        f32     scaledHeight_guess; /* 0x78* */
    };
    f32     height_offset;      /* 0x7C* z = floor height + this */
    u32     spawn_id;           /* 0x80* */
    u32     pet_owner_id;       /* 0x84* */
    s32     max_hp;             /* 0x88* 100 for others (hp is a percentage) */
    s32     cur_hp;             /* 0x8C* OP_HPUpdate */
    s16     guild_id;           /* 0x90* -1 = none */
    u8      unknown0x92[6];     /* 0x92  */
    u8      type;               /* 0x98* 0 = player, 1 = NPC, 2 = PC corpse, 3 = NPC corpse */
    u8      class_;             /* 0x99* */
    u16     race;               /* 0x9A* */
    union
    {
        u8      gender;             /* 0x9C* */
        char    gender_char_guess;  /* 0x9C* */
    };
    u8      level;              /* 0x9D* */
    u8      invis;              /* 0x9E* SpawnAppearance type 3 */
    u8      sneaking;           /* 0x9F* SpawnAppearance type 15 */
    u8      pvp;                /* 0xA0* SpawnAppearance type 4 */
    u8      stand_state;        /* 0xA1* 100 = standing, 102 = dead/feigned, ... */
    u8      light;              /* 0xA2* SpawnAppearance type 5 */
    u8      face;               /* 0xA3* CDisplay::SwapFace */
    u8      equipment[9];       /* 0xA4* material per slot; [7]/[8] = weapon models */
    u8      unknown0xAD[3];     /* 0xAD  */
    u32     equip_colors[9];    /* 0xB0* */
    u8      texture;            /* 0xD4* NPC armour texture, 0xFF = uses equipment */
    u8      unknown0xD5[3];     /* 0xD5  */
    f32     size;               /* 0xD8* -1 = default, recomputed by EQPlayer::SetHeights */
    u8      unknown0xDC[8];     /* 0xDC  */
    u8      helm;               /* 0xE4* */
    u8      unknown0xE5[3];     /* 0xE5  */
    f32     walk_speed;         /* 0xE8* */
    f32     run_speed;          /* 0xEC* */
    u8      linkdead;           /* 0xF0* SpawnAppearance type 18 */
    u8      gm;                 /* 0xF1* SpawnAppearance type 20 */
    s16     levitate;           /* 0xF2* SpawnAppearance type 19; 2 = fly mode */
    s32     body_type;          /* 0xF4* */
    u32     unknown0xF8;        /* 0xF8  */
    s32     anon;               /* 0xFC* SpawnAppearance type 21 */
    u32     unknown0x100;       /* 0x100 */
    s32     afk;                /* 0x104* */
    s32     unk_0x108;       /* 0x108* from NetSpawn 0x58 */
    u8      unknown0x10C[8];    /* 0x10C */
    char    last_name[20];      /* 0x114* */
    u16     guild_rank;         /* 0x128* from NetSpawn 0x9A (EQClassic: guildrank, 1 byte) */
    u16     deity;              /* 0x12A* */
    u8      weaponAnimationType;       /* 0x12C* */
    u8      appearance[6];      /* 0x12D* hair/beard/eye colours, hair style, beard: set from
                                            MsgCharacterSelect 0x458..0x48A (0x004923C8) */
    u8      unknown0x133;       /* 0x133 */

#ifdef __cplusplus
    /* EQPlayer.cpp (Mac: Source/PC Source/EverQuest/EQPlayer.cpp). __thiscall; the ones whose callers load
     * ECX with EQPlayer::top ignore `this`. Static members are __cdecl. */
    void TurnOffAutoFollow_guess(void);                                    /* 0x004B117B */
    void ClearOthersReferenceToMe_guess(void);                              /* 0x004B13C0 */
    struct EQPlayer* GetPlayerFromID_guess(int id);                         /* 0x004B1416 */
    struct EQPlayer* get_player_guess(const char* name);                    /* 0x004B1435 */
    struct EQPlayer* get_player_partial_name(const char* name);       /* 0x004B146A */
    struct EQPlayer* is_player_actor(void* actor);                    /* 0x004B14BF */
    void AppendActorTagSuffix_guess(char* tag);                             /* 0x004B14DD */
    static int WhatDeityAlliance_guess(int deity);                          /* 0x004B2198 */
    static int NotOnSameDeity_guess(struct EQPlayer* a, struct EQPlayer* b, struct EQ_Spell_guess* spell); /* 0x004B21FC */
    static int NotOnSameRaceTeam_guess(struct EQPlayer* a, struct EQPlayer* b); /* 0x004B2303 */
    void SetAfk_guess(int afk);                                             /* 0x004B2548 */
    u32 ModifyAttackSpeed(u32 delay, int ranged);                     /* 0x004B2B65 */
    bool MyFeetAreOnGround(void);                                     /* 0x004B41A6 */
    u8 SkillUsed(u8 skill);                                           /* 0x004B422A */
    int LegalPlayerRace_guess(int race);                                    /* 0x004B4531 */
    bool IsRoleplaying_guess(void);                                         /* 0x004B4868 */
    void InitSneakMod_guess(void);                                          /* 0x004BC963 */
    char IWasHit(u32* hitData);    /* 0x004B3C9D */
    void KnockedOut(void);    /* 0x004B3E1C */
#endif
} EQPlayer;                     /* 0x134 */
C_ASSERT(sizeof(EQPlayer) == 0x134);
C_ASSERT(offsetof(EQPlayer, name) == 0x01);
C_ASSERT(offsetof(EQPlayer, y) == 0x34);
C_ASSERT(offsetof(EQPlayer, next) == 0x68);
C_ASSERT(offsetof(EQPlayer, actor_info) == 0x70);
C_ASSERT(offsetof(EQPlayer, spawn_id) == 0x80);
C_ASSERT(offsetof(EQPlayer, cur_hp) == 0x8C);
C_ASSERT(offsetof(EQPlayer, type) == 0x98);
C_ASSERT(offsetof(EQPlayer, race) == 0x9A);
C_ASSERT(offsetof(EQPlayer, equipment) == 0xA4);
C_ASSERT(offsetof(EQPlayer, equip_colors) == 0xB0);
C_ASSERT(offsetof(EQPlayer, walk_speed) == 0xE8);
C_ASSERT(offsetof(EQPlayer, anon) == 0xFC);
C_ASSERT(offsetof(EQPlayer, last_name) == 0x114);
C_ASSERT(offsetof(EQPlayer, deity) == 0x12A);

/* OP_ZoneEntry, server -> client: checksum + the EQPlayer image of the entering player.
 * Verified against client: HandleWorldMessage (jump case 5) requires size == 0x138 and
 * crc32mem(data + 4, 0x134) == first dword, then constructs EQPlayer from data + 4.
 * EQClassic ServerZoneEntry_Struct (312) agrees in size and in every named offset (its offset =
 * EQPlayer offset + 4). ShowEQ ServerZoneEntryStruct (306 incl. opcode = 304) is 8 short. */
typedef struct MsgServerZoneEntry
{
    u32     crc;                /* 0x00  crc32mem of player */
    EQPlayer player;            /* 0x04  */
} MsgServerZoneEntry;           /* 0x138 */
C_ASSERT(sizeof(MsgServerZoneEntry) == 0x138);

/* A spawn on the wire, 0xA8 bytes: OP_NewSpawn (0x2149, one, encrypted as 42 dwords by
 * 0x004A38D0) and OP_ZoneSpawns (0x2161, zlib-compressed array; msg_eq_netplayer 0x00492ADA steps
 * by 0xA8). Verified against client: size (the stride, and the 42-dword decrypt) and every field
 * marked *, read by UnPackNetPlayer (0x004A39E3) into the EQPlayer field named.
 * EQClassic Spawn_Struct (164) + NewSpawn_Struct (4 + 164 = 168) agree on all offsets (theirs =
 * ours - 4 inside Spawn_Struct). ShowEQ spawnStruct has the same offsets. Differences with the
 * client: the colour block is 9 dwords, not 7 + 9 unknown bytes; cur_hp is one signed byte (a
 * percentage), not s16; the physics bitfields (see PackedPhysics); 0x9A/0x9C are u16. */
typedef struct NetSpawn
{
    u32     unknown0x00;        /* 0x00  not read */
    f32     size;               /* 0x04* -> EQPlayer.size */
    f32     walk_speed;         /* 0x08* */
    f32     run_speed;          /* 0x0C* */
    u32     equip_colors[9];    /* 0x10* */
    PackedPhysics physics;      /* 0x34* unpack_physics */
    u8      unknown0x41;        /* 0x41  not read */
    u16     spawn_id;           /* 0x42* */
    s16     body_type;          /* 0x44* */
    u16     pet_owner_id;       /* 0x46* */
    s8      hp_percent;         /* 0x48* -> EQPlayer.cur_hp; max_hp is set to 100 */
    u8      unknown0x49;        /* 0x49  */
    s16     guild_id;           /* 0x4A* */
    u8      race;               /* 0x4C* */
    u8      type;               /* 0x4D* */
    u8      class_;             /* 0x4E* */
    u8      gender;             /* 0x4F* */
    u8      level;              /* 0x50* */
    u8      invis;              /* 0x51* */
    u8      sneaking;           /* 0x52* */
    u8      pvp;                /* 0x53* */
    u8      stand_state;        /* 0x54* */
    u8      light;              /* 0x55* */
    s8      anon;               /* 0x56* */
    s8      afk;                /* 0x57* */
    s8      unknown0x58;        /* 0x58* -> EQPlayer 0x108 */
    u8      linkdead;           /* 0x59* */
    u8      gm;                 /* 0x5A* */
    s8      levitate;           /* 0x5B* */
    u8      texture;            /* 0x5C* */
    u8      helm;               /* 0x5D* */
    u8      face;               /* 0x5E* */
    u8      equipment[9];       /* 0x5F* */
    char    name[30];           /* 0x68* "NameTooLong" if strlen > 28 */
    char    last_name[20];      /* 0x86* */
    u16     guild_rank;         /* 0x9A* */
    u16     deity;              /* 0x9C* */
    u8      unknown0x9E;        /* 0x9E* -> EQPlayer 0x12C */
    u8      appearance[6];      /* 0x9F* -> EQPlayer 0x12D */
    u8      unknown0xA5[3];     /* 0xA5  */
} NetSpawn;                     /* 0xA8 */
C_ASSERT(sizeof(NetSpawn) == 0xA8);
C_ASSERT(offsetof(NetSpawn, physics) == 0x34);
C_ASSERT(offsetof(NetSpawn, spawn_id) == 0x42);
C_ASSERT(offsetof(NetSpawn, hp_percent) == 0x48);
C_ASSERT(offsetof(NetSpawn, race) == 0x4C);
C_ASSERT(offsetof(NetSpawn, equipment) == 0x5F);
C_ASSERT(offsetof(NetSpawn, name) == 0x68);
C_ASSERT(offsetof(NetSpawn, last_name) == 0x86);
C_ASSERT(offsetof(NetSpawn, deity) == 0x9C);

/* One entry of OP_SpawnPositionUpdate (0x20A1) and the whole of OP_ClientUpdate (0x20F3,
 * client -> server). Verified against client: size 15 (sent by 0x004D5D0F; the 0x20A1 handler
 * steps by 0xF and unpacks at +2). EQClassic and ShowEQ agree (apart from the bitfields). */
typedef struct MsgSpawnPositionUpdate
{
    u16     spawn_id;           /* 0x00  */
    PackedPhysics physics;      /* 0x02  */
} MsgSpawnPositionUpdate;       /* 0x0F */
C_ASSERT(sizeof(MsgSpawnPositionUpdate) == 0x0F);

/* OP_SpawnPositionUpdate (0x20A1): count followed by count 15-byte updates. Verified against
 * client: the handler clamps count to (size - 4) / 15. */
typedef struct MsgSpawnPositionUpdates
{
    u32     count;              /* 0x00  */
    MsgSpawnPositionUpdate updates[1]; /* 0x04  really [count] */
} MsgSpawnPositionUpdates;      /* 0x13 */
C_ASSERT(sizeof(MsgSpawnPositionUpdates) == 0x13);

#pragma pack(pop)

#endif /* TRILOGY_PLAYER_H */
