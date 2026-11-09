/*
 * soundmanager.h - EqSoundManager (Mac: Source/PC Source/EverQuest/EQ_SoundManager.cpp) and the small
 * sound classes around it (Mac: SoundManager.cpp), as far as the rewritten functions read them. Layouts
 * are from the code at the addresses given (fields marked * are read or written there); names are ours
 * unless a Mac symbol is quoted.
 *
 * Miles Sound System (MSS32.DLL) imports are called through the client's own import address table slots
 * (EQ_GLOBAL on the IAT entry), which gives the same `call dword ptr [slot]` as the original and needs no
 * import library.
 *
 * C++ only (VC6). Not included from trilogy.h.
 */
#ifndef TRILOGY_SOUNDMANAGER_H
#define TRILOGY_SOUNDMANAGER_H

#include "common.h"
#include "eqaddr.h"
#include "player.h"

/* --- MSS32.DLL imports (IAT slots) ------------------------------------------------------------------ */

typedef long  (__stdcall *AIL_init_sequence_t)(void* sequence, void* file_image, long sequence_num);
typedef void  (__stdcall *AIL_start_sequence_t)(void* sequence);
typedef void  (__stdcall *AIL_end_sequence_t)(void* sequence);
typedef void  (__stdcall *AIL_set_sequence_volume_t)(void* sequence, long volume, long ms);
typedef void* (__stdcall *AIL_register_prefix_callback_t)(void* sequence, void* callback);

#define AIL_init_sequence           EQ_GLOBAL(AIL_init_sequence_t, 0x00534354)
#define AIL_start_sequence          EQ_GLOBAL(AIL_start_sequence_t, 0x00534358)
#define AIL_register_prefix_callback EQ_GLOBAL(AIL_register_prefix_callback_t, 0x0053435C)
#define AIL_set_sequence_volume     EQ_GLOBAL(AIL_set_sequence_volume_t, 0x00534360)
#define AIL_end_sequence            EQ_GLOBAL(AIL_end_sequence_t, 0x005342F8)

/* --- the sound control object ------------------------------------------------------------------------ */

/* 0x006E4DD0  the sound control object (C++, vtable at +0); only the slots the rewrites call are named. */
class EqSoundControl_guess
{
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08(); virtual void vf0C();
    virtual void vf10(); virtual void vf14(); virtual void vf18();
    virtual int  ReleaseHandle_guess(s32* handle);                    /* +0x1C */
    virtual void vf20();
    virtual int  Command_guess(void* arg_address, int arg);          /* +0x24 */
    virtual void StopAll_guess(int which);                           /* +0x28 -1 = all */
    virtual int  StopHandle_guess(s32* handle);                       /* +0x2C */
};
#define g_sound_control_guess       EQ_GLOBAL(EqSoundControl_guess *, 0x006E4DD0)

/* --- EqSoundManager ------------------------------------------------------------------------------------ */

/* One playing sound, 0x2C bytes; EqSoundManager has 11 (0..7 attached to spawns, 8..10 others). */
typedef struct EqSoundSlot_guess
{
    f32     x;                  /* 0x00* */
    f32     y;                  /* 0x04* */
    f32     z;                  /* 0x08* */
    s32     count;              /* 0x0C* > 0 = in use */
    u32     unknown0x10[3];     /* 0x10* */
    u32     sound_id_guess;     /* 0x1C* compared with actor_info+0x1DC */
    EQPlayer* player;           /* 0x20* */
    u32     unknown0x24;        /* 0x24* */
    s32     handle;             /* 0x28* -1 = none */
} EqSoundSlot_guess;
C_ASSERT(sizeof(EqSoundSlot_guess) == 0x2C);

class EqSoundManager_guess
{
public:
    /* 0x004B3FB0; callback receiver is compared with g_local_player_guess. */
    void PrintControlLossMessage_guess(int unknown0, int unknown1);

    u8      unknown0x000[8];    /* 0x000 */
    f32     unknown0x008;       /* 0x008* */
    u8      unknown0x00C[0xC8]; /* 0x00C */
    u32     unknown0x0D4;       /* 0x0D4* */
    u32     unknown0x0D8;       /* 0x0D8* */
    u32     unknown0x0DC;       /* 0x0DC* */
    f32     unknown0x0E0;       /* 0x0E0* */
    f32     unknown0x0E4;       /* 0x0E4* */
    f32     unknown0x0E8;       /* 0x0E8* */
    f32     unknown0x0EC;       /* 0x0EC* */
    u32     unknown0x0F0;       /* 0x0F0* */
    u32     unknown0x0F4;       /* 0x0F4* */
    u8      unknown0x0F8[8];    /* 0x0F8 */
    u8      unknown0x100;       /* 0x100* */
    u8      unknown0x101;       /* 0x101* */
    u8      unknown0x102[2];    /* 0x102 */
    s32     active_slots;       /* 0x104* */
    s32     active_players;     /* 0x108* */
    EqSoundSlot_guess slots[11]; /* 0x10C* */
    EQPlayer* players[11];      /* 0x2F0* only 0..7 used */
    u32     player_data_a[11];  /* 0x31C* */
    u32     player_data_b[11];  /* 0x348* */
    u8      unknown0x374[0x104];/* 0x374 */
    void*   waves[0x2D0];       /* 0x478* */

    int  AddPlayerToList_guess(EQPlayer* player);                                  /* 0x004E21A3 */
    int  FindPlayerIndex_guess(EQPlayer* value);                                   /* 0x004E21FE */
    int  StopPlayerSlotIfChanged_guess(EQPlayer* player, u32 sound_id);   /* 0x004E2154 */
    int  RemovePlayer_guess(EQPlayer* player);                            /* 0x004E221D */
    void StopSlot_guess(int slot);                                        /* 0x004E2520 */
    bool StopPlayerSounds_guess(EQPlayer* player);                        /* 0x004E25B5 */
    void StopAll_guess(void);                                             /* 0x004E25FD */
    int  FindFreeSlot_guess(int kind);                                    /* 0x004E2280 */
    int  IsWaveLoaded_guess(int index);                                   /* 0x004E370A */
    int  StopAllWaves_guess(int keep);                                    /* 0x004E36D1 (EverQuest.cpp) */
    void SetMusicSelection(int selection);                          /* 0x004E4EE6 */
};
/* 0x006E4DD8  the sound manager. */
#define g_sound_manager_guess       EQ_GLOBAL(EqSoundManager_guess *, 0x006E4DD8)
/* 0x004E34FA  EqSoundManager::WavePlay (Mac, low), __thiscall (id, unknown); uses C++ EH, not rewritten. */
#define SoundManager_WavePlay_guess(mgr, id, arg)     EQ_METHOD(2, void, 0x004E34FA, (EqSoundManager_guess *, int, int), (mgr))((id), (arg))
C_ASSERT(offsetof(EqSoundManager_guess, active_slots) == 0x104);
C_ASSERT(offsetof(EqSoundManager_guess, slots) == 0x10C);
C_ASSERT(offsetof(EqSoundManager_guess, players) == 0x2F0);
C_ASSERT(offsetof(EqSoundManager_guess, player_data_b) == 0x348);
C_ASSERT(offsetof(EqSoundManager_guess, waves) == 0x478);

/* --- the MIDI sequence wrapper ------------------------------------------------------------------------ */

/* The MIDI music player (PlayMidi): the XMIDI file image (0x00), a two-entry queue of tracks (indices
 * 0x08 / 0x0C, per-entry track number 0x10, volume 0x20 / 0x2C, loop count 0x38), the current track
 * (0x4C), the Miles sequence (0x50) and a state (0x54). */
class EqMidiSequence_guess
{
public:
    void*   file_image;         /* 0x00* XMIDI image (AIL_init_sequence) */
    s16     unknown0x04;        /* 0x04* */
    u8      unknown0x06[2];     /* 0x06 */
    u32     unknown0x08;        /* 0x08* */
    u32     unknown0x0C;        /* 0x0C* */
    u32     unknown0x10[2];     /* 0x10* 0xFF */
    u32     unknown0x18;        /* 0x18  */
    u32     unknown0x1C;        /* 0x1C* 1000 */
    u32     unknown0x20[2];     /* 0x20* 0x6E */
    u32     unknown0x28;        /* 0x28  */
    u32     unknown0x2C[2];     /* 0x2C* 500 */
    u32     unknown0x34;        /* 0x34  */
    u32     unknown0x38[2];     /* 0x38* 0 */
    u32     unknown0x40;        /* 0x40  */
    u32     unknown0x44;        /* 0x44* zeroed; byte 0x46 set by SetVolumeIfLoaded */
    u32     unknown0x48;        /* 0x48* */
    s32     current;            /* 0x4C* current track, -1 = none */
    void*   sequence;           /* 0x50* HSEQUENCE */
    s32     state;              /* 0x54* 1 after init, 2 = fading, 4 = stopped */

    EqMidiSequence_guess();                                               /* 0x004E4482 */
    bool PublishHandles_guess(void);                                      /* 0x004E43E3 */
    void SetVolume_guess(long volume, long ms);                           /* 0x004E4522 */
    void Stop_guess(long fade_ms);                                        /* 0x004E4536 */
    bool FadeIn_guess(void);                                              /* 0x004E4570 */
    bool Queue_guess(int track, long volume, long ms, u32 loops);         /* 0x004E471D */
    s32  GetState_guess(void);                                            /* 0x004E47CE */
};

/* A second MIDI stream object (its sequence at 0x4C, its file image at 0x50, flag bytes 0x45 / 0x46). */
class EqMidiStream_guess
{
public:
    u8      unknown0x00[8];     /* 0x00  */
    u32     masterVolume_guess; /* 0x08* */
    u8      unknown0x0C[4];     /* 0x0C  */
    s32     counter;            /* 0x10* */
    class EQSoundEmitter_guess* cursor;             /* 0x14* */
    class EQSoundEmitter_guess* nearest;            /* 0x18* */
    class EQSoundEmitter_guess* current;            /* 0x1C* */
    f32     distance;           /* 0x20* */
    u8      unknown0x24[4];     /* 0x24  */
    f32     best_distance;      /* 0x28* */
    u8      unknown0x2C[4];     /* 0x2C  */
    f32     radius;             /* 0x30* */
    s32     last_state;         /* 0x34* */
    s32     track;              /* 0x38* */
    s32     track_queued;       /* 0x3C* */
    s8      priority;           /* 0x40* */
    u8      unknown0x41[1];     /* 0x41  */
    char    zoneMusicClass_guess; /* 0x42* */
    u8      wanted;             /* 0x43* */
    u8      have;               /* 0x44* */
    u8      unknown0x45;        /* 0x45  */
    u8      fading;             /* 0x46* */
    u8      unknown0x47[5];     /* 0x47 */
    void*   sequence;           /* 0x4C* HSEQUENCE */
    void*   file_image;         /* 0x50* */
    s32     command;            /* 0x54* */
    u8      unk_0x58;           /* 0x58* */
    u8      unknown0x59[3];     /* 0x59  */
    void*   dlsDevice_guess;    /* 0x5C* */
    void*   midiDriver_guess;   /* 0x60* */
    u8      unknown0x64[8];     /* 0x64  */

    void StopAll_guess(void);                                             /* 0x004E4DE4 */
    void SetVolumeIfLoaded_guess(long volume, long ms);                   /* 0x004E4EF0 */
    bool Play_guess(long sequence_num, long volume, long ms);             /* 0x004E4F0F */
};
C_ASSERT(offsetof(EqMidiStream_guess, sequence) == 0x4C);
C_ASSERT(offsetof(EqMidiStream_guess, masterVolume_guess) == 0x08);
C_ASSERT(offsetof(EqMidiStream_guess, counter) == 0x10);
C_ASSERT(offsetof(EqMidiStream_guess, cursor) == 0x14);
C_ASSERT(offsetof(EqMidiStream_guess, nearest) == 0x18);
C_ASSERT(offsetof(EqMidiStream_guess, current) == 0x1C);
C_ASSERT(offsetof(EqMidiStream_guess, distance) == 0x20);
C_ASSERT(offsetof(EqMidiStream_guess, best_distance) == 0x28);
C_ASSERT(offsetof(EqMidiStream_guess, radius) == 0x30);
C_ASSERT(offsetof(EqMidiStream_guess, last_state) == 0x34);
C_ASSERT(offsetof(EqMidiStream_guess, track) == 0x38);
C_ASSERT(offsetof(EqMidiStream_guess, track_queued) == 0x3C);
C_ASSERT(offsetof(EqMidiStream_guess, priority) == 0x40);
C_ASSERT(offsetof(EqMidiStream_guess, zoneMusicClass_guess) == 0x42);
C_ASSERT(offsetof(EqMidiStream_guess, wanted) == 0x43);
C_ASSERT(offsetof(EqMidiStream_guess, have) == 0x44);
C_ASSERT(offsetof(EqMidiStream_guess, command) == 0x54);
C_ASSERT(offsetof(EqMidiStream_guess, unk_0x58) == 0x58);
C_ASSERT(offsetof(EqMidiStream_guess, dlsDevice_guess) == 0x5C);
C_ASSERT(offsetof(EqMidiStream_guess, midiDriver_guess) == 0x60);

/* 0x006ECDF8 / 0x006ECDFC / 0x006ECE00  the three MIDI players; the last one is the zone music (FadeIn and
 * Queue treat it specially). */
#define g_midi_player_a_guess       EQ_GLOBAL(EqMidiSequence_guess *, 0x006ECDF8)
#define g_midi_player_b_guess       EQ_GLOBAL(EqMidiSequence_guess *, 0x006ECDFC)
#define g_midi_music_guess          EQ_GLOBAL(EqMidiSequence_guess *, 0x006ECE00)
/* 0x00579318  the music volume; 0x00577B3C  the music fade time (ms). */
#define g_music_volume_guess        EQ_GLOBAL(long, 0x00579318)
#define g_music_fade_ms_guess       EQ_GLOBAL(long, 0x00577B3C)
/* 0x006ECDCC / 0x006ECDD0  copies of the file image and sequence handle (PublishHandles). */
#define g_midi_file_image_guess     EQ_GLOBAL(void *, 0x006ECDCC)
#define g_midi_sequence_guess       EQ_GLOBAL(void *, 0x006ECDD0)
/* 0x006ECDD4  the next MIDI event callback (__stdcall, 5 args), called while g_midi_filter_on == 3. */
typedef int (__stdcall *MidiEventCallback_guess_t)(void* driver, void* sequence, s32 status, s32 data_1, s32 data_2);
#define g_midi_next_callback_guess  EQ_GLOBAL(MidiEventCallback_guess_t, 0x006ECDD4)
/* 0x006ECE08 / 0x006ECE0C  nesting counters of the event filter (note on / note off). */
#define g_midi_note_on_depth_guess  EQ_GLOBAL(int, 0x006ECE08)
#define g_midi_note_off_depth_guess EQ_GLOBAL(int, 0x006ECE0C)
/* 0x006ECE18  filter mode (0 off, 1 = by channel threshold, 3 = by channel mask); 0x006ECE19 the
 * threshold; 0x006ECE1C the channel mask; 0x006ECE20 == 3 forwards events. */
#define g_midi_filter_mode_guess    EQ_GLOBAL(char, 0x006ECE18)
#define g_midi_filter_channel_guess EQ_GLOBAL(char, 0x006ECE19)
#define g_midi_filter_mask_guess    EQ_GLOBAL(u32, 0x006ECE1C)
#define g_midi_filter_on_guess      EQ_GLOBAL(int, 0x006ECE20)
/* 0x004E4F74  the prefix callback Play registers (not rewritten). */
#define MidiPrefixCallback_guess    ((void *)0x004E4F74)

int __stdcall MidiEventFilter_guess(void* driver, void* sequence, s32 status, s32 data_1, s32 data_2); /* 0x004E42D3 */

/* --- small helpers ---------------------------------------------------------------------------------- */

/* A COM-style interface pointer (vtable slot 2 = Release, __stdcall). */
class EqComObject_guess
{
public:
    virtual long __stdcall QueryInterface_guess(void* iid, void** out);
    virtual unsigned long __stdcall AddRef_guess(void);
    virtual unsigned long __stdcall Release_guess(void);
};

/* Holder of one COM pointer (0x004E3F8A releases it). */
class EqComHolder_guess
{
public:
    EqComObject_guess* object;  /* 0x00* */

    void Release_guess(void);                                             /* 0x004E3F8A */
};

/* An object with a float level at +4 (0x004E2044). */
class EqSoundLevel_guess
{
public:
    u32     unknown0x00;        /* 0x00 */
    f32     level;              /* 0x04* */

    bool IsQuiet_guess(void);                                             /* 0x004E2044 */
};

/* A holder of a pointer whose dword +0x18 is read (0x004E869B / 0x004E869E). */
class EqHandleRef_guess
{
public:
    char*   ref;                /* 0x00* */

    char* Get_guess(void);                                                /* 0x004E869B */
    u32 GetField18_guess(void);                                           /* 0x004E869E */
};

/* Mac: SoundControl (low); `this` is ignored, the call goes to the sound control object. */
class SoundControl_guess
{
public:
    int Command_guess(int arg, int arg2);                                 /* 0x004E3624 */
};

#endif /* TRILOGY_SOUNDMANAGER_H */
