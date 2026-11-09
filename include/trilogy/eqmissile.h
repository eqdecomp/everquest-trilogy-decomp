/*
 * eqmissile.h - EQMissile (Mac: Source/PC Source/EverQuest/EQMissile.cpp), the projectiles in flight, as far
 * as the rewritten functions read them. Layout: from a layout JSON (not published; 0x74 bytes).
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_EQMISSILE_H
#define TRILOGY_EQMISSILE_H

#include "common.h"
#include "eqaddr.h"

class EQMissile_guess;

/* 0x006E4D64  EQMissile::top (Mac): head of the missile list, linked through +0x08. */
#define g_missile_top_guess         EQ_GLOBAL(EQMissile_guess *, 0x006E4D64)

class EQMissile_guess
{
public:
    char    unk_0x00;           /* 0x00* */
    u8      unknown0x01[3];     /* 0x01  */
    EQMissile_guess* previous_guess;     /* 0x04* */
    EQMissile_guess* next;      /* 0x08* */
    union
    {
        float   y;                  /* 0x0C* */
        u32     y_bits_guess;       /* 0x0C* */
    };
    union
    {
        float   x;                  /* 0x10* */
        u32     x_bits_guess;       /* 0x10* */
    };
    union
    {
        float   z;                  /* 0x14* */
        u32     z_bits_guess;       /* 0x14* */
    };
    union
    {
        float   heading;            /* 0x18* */
        u32     heading_bits_guess; /* 0x18* */
    };
    union
    {
        float   pitch;              /* 0x1C* */
        u32     pitch_bits_guess;   /* 0x1C* */
    };
    union
    {
        float   forwardSpeed_guess; /* 0x20* */
        u32     forwardSpeed_bits_guess; /* 0x20* */
    };
    union
    {
        float   velocityY;          /* 0x24* */
        u32     velocityY_bits_guess; /* 0x24* */
    };
    union
    {
        float   velocityX;          /* 0x28* */
        u32     velocityX_bits_guess; /* 0x28* */
    };
    union
    {
        float   velocityZ;          /* 0x2C* */
        u32     velocityZ_bits_guess; /* 0x2C* */
    };
    union
    {
        float   headingRate_guess;  /* 0x30* */
        u32     headingRate_bits_guess; /* 0x30* */
    };
    union
    {
        float   pitchRate_guess;    /* 0x34* */
        u32     pitchRate_bits_guess; /* 0x34* */
    };
    union
    {
        float   flightPitch_guess;  /* 0x38* */
        u32     flightPitch_bits_guess; /* 0x38* */
    };
    void**  actor_info;         /* 0x3C* its first dword is the t3d actor */
    u32     sourceId_guess;     /* 0x40* */
    u32     targetId_guess;     /* 0x44* */
    u16     itemIdFlags_guess;  /* 0x48* */
    u16     sourceSkillOrLevel_guess; /* 0x4A* */
    union
    {
        int     spellId;            /* 0x4C* */
        int     damage_guess;       /* 0x4C* */
        u16     spellId_lowWord_guess; /* 0x4C* */
    };
    char    light_guess;        /* 0x50* */
    u8      weight_guess;       /* 0x51* */
    u8      range_guess;        /* 0x52* */
    u8      kind;               /* 0x53* */
    u8      skillOrDelay_guess; /* 0x54* */
    u8      unknown0x55[0x1F];  /* 0x55  */

    EQMissile_guess* is_missile_actor(void* actor);                /* 0x004AD0CA */
};
C_ASSERT(offsetof(EQMissile_guess, next) == 0x08);
C_ASSERT(offsetof(EQMissile_guess, actor_info) == 0x3C);
C_ASSERT(sizeof(EQMissile_guess) == 0x74);
C_ASSERT(offsetof(EQMissile_guess, unk_0x00) == 0x00);
C_ASSERT(offsetof(EQMissile_guess, previous_guess) == 0x04);
C_ASSERT(offsetof(EQMissile_guess, y) == 0x0C);
C_ASSERT(offsetof(EQMissile_guess, x) == 0x10);
C_ASSERT(offsetof(EQMissile_guess, z) == 0x14);
C_ASSERT(offsetof(EQMissile_guess, heading) == 0x18);
C_ASSERT(offsetof(EQMissile_guess, pitch) == 0x1C);
C_ASSERT(offsetof(EQMissile_guess, forwardSpeed_guess) == 0x20);
C_ASSERT(offsetof(EQMissile_guess, velocityY) == 0x24);
C_ASSERT(offsetof(EQMissile_guess, velocityX) == 0x28);
C_ASSERT(offsetof(EQMissile_guess, velocityZ) == 0x2C);
C_ASSERT(offsetof(EQMissile_guess, headingRate_guess) == 0x30);
C_ASSERT(offsetof(EQMissile_guess, pitchRate_guess) == 0x34);
C_ASSERT(offsetof(EQMissile_guess, flightPitch_guess) == 0x38);
C_ASSERT(offsetof(EQMissile_guess, sourceId_guess) == 0x40);
C_ASSERT(offsetof(EQMissile_guess, targetId_guess) == 0x44);
C_ASSERT(offsetof(EQMissile_guess, itemIdFlags_guess) == 0x48);
C_ASSERT(offsetof(EQMissile_guess, sourceSkillOrLevel_guess) == 0x4A);
C_ASSERT(offsetof(EQMissile_guess, spellId) == 0x4C);
C_ASSERT(offsetof(EQMissile_guess, light_guess) == 0x50);
C_ASSERT(offsetof(EQMissile_guess, weight_guess) == 0x51);
C_ASSERT(offsetof(EQMissile_guess, range_guess) == 0x52);
C_ASSERT(offsetof(EQMissile_guess, kind) == 0x53);
C_ASSERT(offsetof(EQMissile_guess, skillOrDelay_guess) == 0x54);

#endif /* TRILOGY_EQMISSILE_H */
