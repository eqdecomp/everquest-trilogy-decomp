/*
 * sound.h - the small parts of the client's sound objects that the EQMisc rewrites touch. Layouts from the
 * code at the addresses given (verified against eqgame.exe for the fields marked *); the rest is unknown
 * and kept as bytes. Names are ours (no Mac symbols in this range).
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_SOUND_H
#define TRILOGY_SOUND_H

#include "common.h"
#include "eqaddr.h"

/* A positioned ambient sound (day/night pair) of the zone's emitter list, 0x5C bytes (loadSounds 0x004DD666
 * allocates operator new(0x5C) and runs the constructor 0x004BD259 on it with the record it read from the
 * file). The constructor links it in front of the list head at 0x006E4D80 and numbers it from the counter
 * at 0x00568B78; it copies 0x10..0x53 from the record (except 0x2C) or, without one, sets the defaults
 * noted below. Fields marked * are read or written by the code at the addresses given; meanings beyond
 * position and radius are guesses. */
class EQSoundEmitter_guess
{
public:
    EQSoundEmitter_guess*   next_guess;     /* 0x00* the emitter linked after this one (newer) */
    EQSoundEmitter_guess*   prev_guess;     /* 0x04* the previous list head */
    s32     unknown0x08;        /* 0x08* 0 */
    s32     id_guess;           /* 0x0C* running number (0x00568B78) */
    f32     y;                  /* 0x10* */
    f32     x;                  /* 0x14* */
    f32     z;                  /* 0x18* */
    f32     radius;             /* 0x1C* default 100 */
    s32     unknown0x20;        /* 0x20* default 0 */
    s32     unknown0x24;        /* 0x24* default 0 */
    s32     unknown0x28;        /* 0x28* default 0 */
    s32     unknown0x2C;        /* 0x2C* always 0 */
    s32     sound_id_guess[2];  /* 0x30* default -1 (day / night?) */
    u8      unknown0x38;        /* 0x38* default 0 */
    u8      unknown0x39;        /* 0x39* default 0 */
    u8      unknown0x3A;        /* 0x3A* default 0 */
    u8      unknown0x3B;        /* 0x3B  */
    s32     unknown0x3C;        /* 0x3C* default 0 */
    s32     unknown0x40;        /* 0x40* default 0 */
    s32     unknown0x44[4];     /* 0x44* default 3000 each (fade / repeat times in ms?) */
    s32     unknown0x54;        /* 0x54* always -1 */
    s32     unknown0x58;        /* 0x58* always -1 */

    EQSoundEmitter_guess(const EQSoundEmitter_guess* record);              /* 0x004BD259 */
    ~EQSoundEmitter_guess();                                               /* 0x004BD36F */
    bool IsLocalPlayerInRange_guess();                                     /* 0x004BD3BD */
    float GetLocalPlayerDistance_guess();                                  /* 0x004BD43F (EQ_SoundManager.cpp) */
};
C_ASSERT(sizeof(EQSoundEmitter_guess) == 0x5C);
C_ASSERT(offsetof(EQSoundEmitter_guess, radius) == 0x1C);
C_ASSERT(offsetof(EQSoundEmitter_guess, unknown0x38) == 0x38);
C_ASSERT(offsetof(EQSoundEmitter_guess, unknown0x54) == 0x54);
/* 0x006E4D80  the emitter list head (the newest); 0x00568B78  the next emitter number. */
#define g_sound_emitter_head_guess  EQ_GLOBAL(EQSoundEmitter_guess *, 0x006E4D80)
#define g_sound_emitter_next_id_guess EQ_GLOBAL(s32, 0x00568B78)

/* A sound buffer wrapper: parameters are stored with a "changed" bit in the flags at 0x54 and applied
 * later. */
class EQSoundBuffer_guess
{
public:
    void*   vtable;             /* 0x00* */
    union
    {
        u32     position_y;         /* 0x04* */
        float   position_y_float_guess; /* 0x04* */
    };
    union
    {
        u32     position_z;         /* 0x08* */
        float   position_z_float_guess; /* 0x08* */
    };
    union
    {
        u32     position_x;         /* 0x0C* */
        float   position_x_float_guess; /* 0x0C* */
    };
    u32     velocityX_bits_guess; /* 0x10* */
    u32     velocityY_bits_guess; /* 0x14* */
    u32     velocityZ_bits_guess; /* 0x18* */
    union
    {
        u32     orientation[6];     /* 0x1C* */
        float   orientation_float_guess[6]; /* 0x1C* */
    };
    f32     min_distance; /* 0x34* clamped to 0.01 .. 1000, flag 0x80 */
    float   distanceScale_guess; /* 0x38* */
    u8      unknown0x3C[4];     /* 0x3C  */
    u32     effectVolume_guess; /* 0x40* */
    s32     volume2;      /* 0x44* clamped to -10000 .. 0 like volume, flag 0x20 */
    s32     volume;             /* 0x48* DirectSound hundredths of a dB, clamped to -10000 .. 0, flag 0x40 */
    float   frequencyRatio_guess; /* 0x4C* */
    u32     mode_guess;         /* 0x50* 1 / 0x10 / 0x40 / 0x20 bits from SetMode; changed bit: byte 0x56 bit 0 */
    u32     changed_flags;      /* 0x54* */

    bool SetVolume_guess(int volume);                                      /* 0x004ECBA1 */
    bool SetMinDistance_guess(float distance);                             /* 0x004ECBFA */
    bool SetVolume2_guess(int volume);                                     /* 0x004ECB82 (SoundManager.cpp) */
    void SetMode_guess(u8 mode);                                           /* 0x004ECBC2 (SoundManager.cpp) */
    /* Inline: the out-of-line copy the unwind handlers call is 0x004E24FC (EQ_SoundManager.cpp), which only
     * restores the vtable. */
    ~EQSoundBuffer_guess() {}
};
C_ASSERT(offsetof(EQSoundBuffer_guess, min_distance) == 0x34);
C_ASSERT(offsetof(EQSoundBuffer_guess, volume2) == 0x44);
C_ASSERT(offsetof(EQSoundBuffer_guess, volume) == 0x48);
C_ASSERT(offsetof(EQSoundBuffer_guess, mode_guess) == 0x50);
C_ASSERT(offsetof(EQSoundBuffer_guess, changed_flags) == 0x54);
C_ASSERT(offsetof(EQSoundBuffer_guess, vtable) == 0x00);
C_ASSERT(offsetof(EQSoundBuffer_guess, position_y) == 0x04);
C_ASSERT(offsetof(EQSoundBuffer_guess, position_z) == 0x08);
C_ASSERT(offsetof(EQSoundBuffer_guess, position_x) == 0x0C);
C_ASSERT(offsetof(EQSoundBuffer_guess, velocityX_bits_guess) == 0x10);
C_ASSERT(offsetof(EQSoundBuffer_guess, velocityY_bits_guess) == 0x14);
C_ASSERT(offsetof(EQSoundBuffer_guess, velocityZ_bits_guess) == 0x18);
C_ASSERT(offsetof(EQSoundBuffer_guess, orientation) == 0x1C);
C_ASSERT(offsetof(EQSoundBuffer_guess, distanceScale_guess) == 0x38);
C_ASSERT(offsetof(EQSoundBuffer_guess, effectVolume_guess) == 0x40);
C_ASSERT(offsetof(EQSoundBuffer_guess, frequencyRatio_guess) == 0x4C);

/* Partial DirectSound wrappers (SoundManager.cpp). Only methods are recovered; do not instantiate or
 * take sizeof() until their layouts are known. The rewrites access the verified offsets directly. */
class EqDirectSoundVoice_guess
{
public:
    u32 GetStatus_guess();                                         /* 0x004EC004 */
    int GetVolume_guess();                                         /* 0x004EC42B */
};

class EqDirectSoundSlots_guess
{
public:
    void SetVolume_guess(int slot, int volume);                    /* 0x004EC713 */
    bool CommandSlot_guess(int slot, u32 value);                    /* 0x004ECAC9 */
};

void CopySoundPair_guess(void *destination, const void *source);     /* 0x004EF0B8 */

#endif /* TRILOGY_SOUND_H */
