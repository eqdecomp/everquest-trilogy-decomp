// EXTERNAL: TRILOGY 0x005218BB ??3@YAXPAX@Z
// EXTERNAL: TRILOGY 0x005218C6 ??2@YAPAXI@Z
// EQ_SoundManager.cpp - EqSoundManager: the zone's sound slots, the spawns' sounds, the MIDI music player.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (from Mac debug map)
// The 0x004E2044..0x004E4F0F run is one object (Mac-mapped neighbours EqSoundManager::WavePlay,
// LoadOldEmitters, SetMusicSelection, SetCurrentZone); the queue's graph labels put some of it in
// EverQuest/EQPlayer and EverQuest/SoundManager.

#include <sys/types.h>
#include <sys/stat.h>

#include "trilogy/common.h"
#include "trilogy/player.h"
#include "trilogy/globals.h"
#include "trilogy/sound.h"
#include "trilogy/soundmanager.h"
#include <stdio.h>
#include <string.h>

// Debug output and message sending by name, like the CRT calls: a direct call has its arguments popped
// right after it, as in the client; EQ_CDECL's constant function pointer lets VC6 merge neighbouring pops.
#undef EQTRACE_guess
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
u32 __cdecl EQTRACE_guess(const char* format, ...);
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char* format, ...);
#define LogMessage_guess DbgLog_guess

// FUNCTION: TRILOGY 0x004b3fb0
// ORIGINAL FILE: EverQuest/EQ_SoundManager.cpp (from Mac debug map)
// This callback compares its ECX receiver with the local EQPlayer; both stack arguments are ignored.
void EqSoundManager_guess::PrintControlLossMessage_guess(int unknown0, int unknown1)
{
    if ((void *)this == (void *)g_local_player_guess)
    {
        EQ_METHOD(1, void, 0x004D05B7, (void *, const char *), (void *)pinstCEverQuest_guess)((const char *)0x005689EC);
    }
}

// FUNCTION: TRILOGY 0x004bd43f
// Estimates a player's proximity to a zone sound emitter for range-based playback.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (queue label; next to EQSoundEmitter::IsLocalPlayerInRange)
// __thiscall. Rough distance from the emitter to the local player (|dz| + the larger horizontal axis +
// half the smaller); 0 when there is no local player or the distance is beyond the radius.
/* fake match: |dz| is folded into the sum as a ternary; a separate if/negate adds an fld/fstp pair VC6 doesn't. */
float EQSoundEmitter_guess::GetLocalPlayerDistance_guess()
{
    EQPlayer* player = g_local_player_guess;
    float dy;
    float dx;
    float dist;

    if (player == 0)
        return EQ_GLOBAL(float, 0x0053510C);
    dy = y - player->y;
    dx = x - player->x;
    if (dy < EQ_GLOBAL(float, 0x0053510C))
        dy = -dy;
    if (dx < EQ_GLOBAL(float, 0x0053510C))
        dx = -dx;
    if (dy > dx)
        dx = dx * 0.5f;
    else
        dy = dy * 0.5f;
    dist = z - player->z;
    dist = (dist < EQ_GLOBAL(float, 0x0053510C) ? -dist : dist) + dx + dy;
    if (dist > radius)
        return EQ_GLOBAL(float, 0x0053510C);
    return dist;
}

struct FilePathString_guess
{
    int reference;
    int unknown;

    // EXTERNAL: TRILOGY 0x004C4823 FilePathString_guess::FilePathString_guess
    FilePathString_guess(const char *name);
    // EXTERNAL: TRILOGY 0x004C489A FilePathString_guess::Assign_guess
    FilePathString_guess &Assign_guess(const FilePathString_guess &other);

    ~FilePathString_guess()
    {
        if (reference != 0)
            EQ_METHOD(0, void, 0x004C4813, (void *), (void *)reference)();
    }
};

struct NormalizedFilePathString_guess : public FilePathString_guess
{
    // EXTERNAL: TRILOGY 0x004C48CB NormalizedFilePathString_guess::NormalizedFilePathString_guess
    NormalizedFilePathString_guess(const FilePathString_guess *input);
};

class FilePath_guess : public FilePathString_guess
{
public:
    FilePath_guess(const char *name);
};

// FUNCTION: TRILOGY 0x004c479d
// Turns an input filename into a normalized resource path.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (from Mac debug map)
FilePath_guess::FilePath_guess(const char *name) : FilePathString_guess(0)
{
    FilePathString_guess input(name);
    Assign_guess(NormalizedFilePathString_guess(&input));
}

struct PackFSDataBlock_guess
{
    int unknown0x00;
    int unknown0x04;
    char *text;
};

class PackFSStorage_guess
{
public:
    PackFSDataBlock_guess* data;

    PackFSStorage_guess(int size)
    {
        EQ_METHOD(1, void, 0x004DD808, (void *, int), this)(size);
    }

    ~PackFSStorage_guess()
    {
    }
};

class PackFSDataInit_guess
{
public:
    PackFSStorage_guess storage;
    int unknown0x04;

    PackFSDataInit_guess(const char *name);
};

// FUNCTION: TRILOGY 0x004c4823
// Creates shared string storage for a nonempty resource name.
PackFSDataInit_guess::PackFSDataInit_guess(const char *name)
    : storage(0)
{
    size_t length;

    unknown0x04 = 0;
    if (name != 0)
    {
        length = strlen(name);
        if (length != 0)
        {
            int temporary[4];

            EQ_METHOD(1, void, 0x004DD808, (void *, int), temporary)(length + 1);
            EQ_METHOD(0, void, 0x004DD93D, (void *), this)();
            strcpy(
                storage.data->text, name);
            unknown0x04 = 0;
        }
    }
}

// FUNCTION: TRILOGY 0x004db93c
// Shows the sound card and music renderer details, then reports the driver log location.
// ORIGINAL FILE: EverQuest/EQ_SoundManager.cpp (from Mac debug map)
// [Mac, low] name. __cdecl, no receiver used: shows the sound card description (0x006ECD0C), plus the
// music type (DLS / SoundFont) when music is on, then the "written to adriver.txt" note.
void DisplaySoundCardInfo_guess(void)
{
    char text[256];

    strcpy(text, (const char *)0x006ECD0C);
    if (*(int *)0x006ECE24 > 0 && *(int *)0x006ECDF4 != 0)
    {
        if (*(int *)0x006ECE28 == 0)
            strcat(text, (const char *)0x005705B8);
        else
            strcat(text, (const char *)0x005705A4);
    }
    EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), (void *)pinstCEverQuest_guess)(text, 0);
    EQ_METHOD(2, void, 0x004D055E, (void *, const char *, int), (void *)pinstCEverQuest_guess)((const char *)0x00570560, 4);
}

// FUNCTION: TRILOGY 0x004e2044
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. The level at +4 is below 0.48.
bool EqSoundLevel_guess::IsQuiet_guess(void)
{
    if (level < 0.48)
        return true;
    return false;
}

class EqSoundManagerFunction_guess : public EqSoundManager_guess
{
public:
    bool PlaySoundForPlayer_guess(EQPlayer* player, int sound_id);
};

// FUNCTION: TRILOGY 0x004e20df
// Starts a spawn's sound effect at its position while suppressing duplicate playback.
bool EqSoundManagerFunction_guess::PlaySoundForPlayer_guess(EQPlayer* player, int sound_id)
{
    int i;
    u32* slot;
    int result;

    if (sound_id < 0x2d0)
    {
        i = 0;
        slot = (u32*)&slots[0].player;
        do
        {
            if (slot[-1] == (u32)sound_id && slot[0] == (u32)player)
                return false;
            i = i + 1;
            slot = slot + 0xb;
        } while (i < 8);

        EQ_METHOD(2, int, 0x004E2154, (EqSoundManager_guess*, EQPlayer*, u32), (EqSoundManager_guess*)this)(player, sound_id);

        if (sound_id != 0)
        {
            result = EQ_METHOD(8, int, 0x004E22DF, (EqSoundManager_guess*, f32, f32, f32, int, int, EQPlayer*, int, void*), (EqSoundManager_guess*)this)(player->y, player->x, player->z,
                 sound_id, 0, player, 0, EQ_ARRAY(void*, 0x00578400)[sound_id]);
            if (result != 0)
                return true;
        }
    }
    return false;
}

// FUNCTION: TRILOGY 0x004e2154
// Stops obsolete spawn audio when the spawn's current sound changes.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label EQPlayer)
// __thiscall. Stops the first of the spawn slots 0..7 playing for `player` whose sound is neither the
// spawn's current one (actor_info+0x1DC) nor `sound_id`; returns its index, or -1.
int EqSoundManager_guess::StopPlayerSlotIfChanged_guess(EQPlayer* player, u32 sound_id)
{
    int i = 0;
    int rc = -1;

    if (player != 0)
    {
        for (; i < 8; i++)
        {
            if (slots[i].count != 0 && slots[i].player == player)
            {
                u32 id = slots[i].sound_id_guess;
                if (id != slots[i].player->actor_info->sound_id_guess && id != sound_id)
                {
                    StopSlot_guess(i);
                    rc = i;
                    break;
                }
            }
        }
        return rc;
    }
    return -1;
}

// FUNCTION: TRILOGY 0x004e221d
// Removes a spawn from the sound manager's tracking.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label EQPlayer)
// __thiscall. Removes `player` from the 8 tracked spawns (clearing its actor_info+0x228/+0x22C); returns
// its index, or -1.
int EqSoundManager_guess::RemovePlayer_guess(EQPlayer* player)
{
    int i;

    if (player != 0)
    {
        for (i = 0; i < 8; i++)
        {
            if (players[i] == player)
            {
                players[i]->actor_info->sound_data_b_guess = 0;
                players[i]->actor_info->sound_data_a_guess = 0;
                players[i] = 0;
                player_data_a[i] = 0;
                player_data_b[i] = 0;
                active_players--;
                return i;
            }
        }
    }
    return -1;
}

class EqSoundManagerPlayback_guess : public EqSoundManager_guess
{
public:
    int PlayPositionedSound_guess(float x, float y, float z, int soundId, int kind,
                     EQPlayer* player, int flags, int resourceId);
};

// FUNCTION: TRILOGY 0x004e22df
// Starts sound soundId (<720) at x,y,z in a free slot; for a spawn, loads the wave and uses its actor's range.
// @early-stop: register order, x87 copies, fallback layout, and the local destructor tail still differ.
/* fake match: retain the EH buffer/adapters and volatile accesses that preserve target ordering. */
int EqSoundManagerPlayback_guess::PlayPositionedSound_guess(float x, float y, float z,
                                               int soundId, int kind,
                                               EQPlayer* volatile player, int flags,
                                               int resourceId)
{
    struct SoundControlVtable_guess
    {
        virtual void Unknown0() = 0;
        virtual void Unknown1() = 0;
        virtual void Unknown2() = 0;
        virtual void Unknown3() = 0;
        virtual void Unknown4() = 0;
        virtual void Unknown5() = 0;
        virtual int Acquire(int soundId) = 0;
        virtual void Release(s32* handle) = 0;
    };
    struct SoundManagerValidation_guess
    {
        char Validate(u32 resourceId, int soundId) { return 0; }
    };
    union ValidationPointer_guess
    {
        char (__fastcall *address)(SoundManagerValidation_guess*, int, u32, int);
        char (SoundManagerValidation_guess::*method)(u32, int);
    };
    union SetMinDistancePointer_guess
    {
        bool (__fastcall *address)(EQSoundBuffer_guess*, int, float);
        bool (EQSoundBuffer_guess::*method)(float);
    };
    union SetVolumePointer_guess
    {
        bool (__fastcall *address)(EQSoundBuffer_guess*, int, int);
        bool (EQSoundBuffer_guess::*method)(int);
    };
    union SetModePointer_guess
    {
        void (__fastcall *address)(EQSoundBuffer_guess*, int, u8);
        void (EQSoundBuffer_guess::*method)(u8);
    };

    EQSoundBuffer_guess command;
    EQSoundBuffer_guess* commandBuffer = &command;
    int slotIndex;
    EQPlayer* slotPlayer;
    ValidationPointer_guess validate = {
        EQ_THISCALL(char, 0x004E3768,
                    (SoundManagerValidation_guess*, int, u32, int)) };
    SetMinDistancePointer_guess setMinDistance = {
        EQ_THISCALL(bool, 0x004ECBFA,
                    (EQSoundBuffer_guess*, int, float)) };
    SetVolumePointer_guess setVolume = {
        EQ_THISCALL(bool, 0x004ECBA1,
                    (EQSoundBuffer_guess*, int, int)) };
    SetModePointer_guess setMode = {
        EQ_THISCALL(void, 0x004ECBC2,
                    (EQSoundBuffer_guess*, int, u8)) };

    *(u32*)commandBuffer = 0x00535798;
    *(volatile u32*)&commandBuffer->changed_flags = 0;

    if (g_sound_manager_guess == 0)
        return 0;

    commandBuffer->changed_flags = 0;
    slotIndex = FindFreeSlot_guess(kind);
    if (slotIndex != -1)
    {
            this->slots[slotIndex].x = x;
            this->slots[slotIndex].y = y;
            this->slots[slotIndex].z = z;
            /* fake match: preserve the target's kind read before clearing adjacent fields. */
            *(volatile u32*)&this->slots[slotIndex].unknown0x10[0] =
                *(volatile int*)&kind;
            slotPlayer = player;
            this->slots[slotIndex].unknown0x10[1] = 0;
            this->slots[slotIndex].unknown0x10[2] = 0;
            this->slots[slotIndex].count = 0;
            this->slots[slotIndex].sound_id_guess = soundId;
            this->slots[slotIndex].player = slotPlayer;
            this->slots[slotIndex].unknown0x24 = flags;

            if (soundId < 0x2d0)
            {
                /* fake match: keep the player-field reread at this branch. */
                if (*(volatile u32*)&this->slots[slotIndex].player != 0)
                {
                    if ((((SoundManagerValidation_guess*)g_sound_manager_guess)
                         ->*validate.method)(resourceId,
                             (int)(EQ_GLOBAL(u8*, 0x00577B44) + soundId)) == 0)
                        return 0;

                    s32* volatile soundHandle = &this->slots[slotIndex].handle;
                    if (*soundHandle != -1)
                    {
                        ((SoundControlVtable_guess*)g_sound_control_guess)
                            ->Release(soundHandle);
                    }
                    int handle =
                        ((SoundControlVtable_guess*)g_sound_control_guess)
                            ->Acquire((int)(EQ_GLOBAL(u8*, 0x00577B44) + soundId));
                    *soundHandle = handle;
                    if (handle == -1)
                        return 0;
                    (commandBuffer->*setMinDistance.method)(
                        *(float*)(*(u8**)((u8*)player + 0x70) + 0x1a0));
                    (commandBuffer->*setVolume.method)(-500);
                    if (this->slots[slotIndex].sound_id_guess ==
                        *(int*)(*(u8**)((u8*)player + 0x70) + 0x228))
                    {
                        (commandBuffer->*setMode.method)(1);
                    }
                }
                else if (flags == 0 &&
                         this->slots[slotIndex].handle == -1 &&
                         g_sound_control_guess != 0)
                {
                    int handle =
                        ((SoundControlVtable_guess*)g_sound_control_guess)
                            ->Acquire(soundId - 1);
                    this->slots[slotIndex].handle = handle;
                    if (handle == -1)
                        return 0;
                }
            }
            else
                return 0;

            this->slots[slotIndex].count = soundId;
            this->active_slots++;
            if (this->slots[slotIndex].player != 0 ||
                this->slots[slotIndex].unknown0x24 != 0)
            {
                this->slots[slotIndex].x = x;
                this->slots[slotIndex].y = y;
                this->slots[slotIndex].z = z;
                commandBuffer->position_y_float_guess = this->slots[slotIndex].x;
                commandBuffer->position_z_float_guess = this->slots[slotIndex].z;
                commandBuffer->position_x_float_guess = this->slots[slotIndex].y;
                commandBuffer->changed_flags |= 1;
            }
            return ((SoundControl_guess*)g_sound_manager_guess)->Command_guess(
                         this->slots[slotIndex].handle,
                (int)commandBuffer);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e2280
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label SoundManager)
// __thiscall. The last free slot (handle -1) among 0..7 (kind 0), 8..10 (kind 1) or 0..10 (other); -1.
int EqSoundManager_guess::FindFreeSlot_guess(int kind)
{
    int rc = -1;
    int i;

    switch (kind)
    {
    case 0:
        for (i = 0; i < 8; i++)
            if (slots[i].handle == -1)
                rc = i;
        break;
    case 1:
        for (i = 8; i < 11; i++)
            if (slots[i].handle == -1)
                rc = i;
        break;
    default:
        for (i = 0; i < 11; i++)
            if (slots[i].handle == -1)
                rc = i;
        break;
    }
    return rc;
}

// FUNCTION: TRILOGY 0x004e24fc
// The out-of-line EQSoundBuffer_guess destructor the unwind handlers call: stores the vtable 0x00535798.
// __thiscall (this in ECX); no return value.
void __fastcall SetVtable_535798_guess(u32* obj)
{
    *obj = 0x00535798;
}

// FUNCTION: TRILOGY 0x004e2503
// Finalizes an audio object and may free it; its game-specific role is unclear.
void* __fastcall DeleteObjectIfFlagged_guess(void* object, int edxValue, u8 flags)
{
    *(u32*)object = 0x00535798;
    if (flags & 1)
        operator delete(object);
    return object;
}

// FUNCTION: TRILOGY 0x004e2520
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label EQPlayer)
// __thiscall. Stops and clears a slot in use: its handle goes back to the sound control object.
void EqSoundManager_guess::StopSlot_guess(int slotIndex)
{
    if (slots[slotIndex].count >= 1)
    {
        if (slots[slotIndex].handle > -1)
        {
            g_sound_control_guess->StopHandle_guess(&slots[slotIndex].handle);
            g_sound_control_guess->ReleaseHandle_guess(&slots[slotIndex].handle);
        }
        slots[slotIndex].x = 0.0f;
        slots[slotIndex].y = 0.0f;
        slots[slotIndex].z = 0.0f;
        slots[slotIndex].count = 0;
        slots[slotIndex].unknown0x10[0] = 0;
        slots[slotIndex].unknown0x10[1] = 0;
        slots[slotIndex].unknown0x10[2] = 0;
        slots[slotIndex].sound_id_guess = 0;
        slots[slotIndex].player = 0;
        slots[slotIndex].unknown0x24 = 0;
        slots[slotIndex].handle = -1;
        active_slots--;
    }
}

// FUNCTION: TRILOGY 0x004e25b5
// Stops a spawn's active sounds and removes it from sound tracking.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label EQPlayer)
// __thiscall (EBP frame in the original). Stops every slot playing for `player` and forgets the spawn;
// true when a slot was stopped.
bool EqSoundManager_guess::StopPlayerSounds_guess(EQPlayer* player)
{
    int stopped = 0;
    int i;

    for (i = 0; i < 11; i++)
    {
        if (slots[i].player == player)
        {
            StopSlot_guess(i);
            stopped++;
        }
    }
    RemovePlayer_guess(player);
    return stopped > 0;
}

// FUNCTION: TRILOGY 0x004e25fd
// Stops all tracked spawn sounds and clears the manager's other active audio.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; queue label EQPlayer)
// __thiscall. Stops everything: the tracked spawns' sounds, the other slots, then resets every slot and
// the listener state.
void EqSoundManager_guess::StopAll_guess(void)
{
    int i;

    if (active_players != 0)
    {
        for (i = 0; i < 8; i++)
        {
            if (players[i] != 0)
                StopPlayerSounds_guess(players[i]);
            players[i] = 0;
            player_data_a[i] = 0;
            player_data_b[i] = 0;
        }
    }
    if (active_slots != 0)
    {
        for (i = 8; i < 11; i++)
        {
            if (slots[i].count != 0)
                StopSlot_guess(i);
        }
    }
    active_slots = 0;
    unknown0x008 = 0.0f;
    active_players = 0;
    for (i = 0; i < 11; i++)
    {
        if (slots[i].handle > -1)
        {
            g_sound_control_guess->StopHandle_guess(&slots[i].handle);
            g_sound_control_guess->ReleaseHandle_guess(&slots[i].handle);
            slots[i].handle = -1;
        }
        slots[i].x = 0.0f;
        slots[i].y = 0.0f;
        slots[i].z = 0.0f;
        slots[i].count = 0;
        slots[i].unknown0x10[0] = 0;
        slots[i].unknown0x10[1] = 0;
        slots[i].unknown0x10[2] = 0;
        slots[i].sound_id_guess = 0;
        slots[i].player = 0;
        slots[i].unknown0x24 = 0;
    }
    unknown0x0F0 = 0;
    unknown0x0F4 = 0;
    unknown0x0EC = 0.0f;
    unknown0x101 = 0;
    unknown0x100 = 0;
    unknown0x0E8 = 0.0f;
    unknown0x0D4 = 0;
    unknown0x0D8 = 0;
    unknown0x0E4 = 0.0f;
    unknown0x0DC = 0;
    unknown0x0E0 = 0.0f;
}

// FUNCTION: TRILOGY 0x004e2798
// Provides squared 3D range from a position to the local player for audibility checks.
// __thiscall in the original (the one caller sets ECX=EDI; the code never reads it), written as a
// __fastcall free function with an EDX dummy, as the other ECX-only callees are. Callee pops 12 bytes.
// Returns the squared distance from (y, x, z) to the local player: each arg minus the player's 0x34
// (y), 0x38 (x) and 0x3C (z) field. No sqrt and no feet offset (unlike GetSpawnDistance_guess). The
// result stays unrounded on the x87 stack, with the sum taken as (dz*dz + dx*dx) + dy*dy.
float __fastcall local_player_distance_squared_guess(void* unused_this, int unused_edx, float y, float x, float z)
{
    float dy;
    float dx;
    float dz;
    EQPlayer* player = g_local_player_guess;

    dy = y - player->y;
    dx = x - player->x;
    dz = z - player->z;
    return dz * dz + dx * dx + dy * dy;
}

/* This vtable view names only the two slots Update uses directly. */
class EqSoundControl_update_guess
{
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08(); virtual void vf0C();
    virtual void vf10(); virtual void vf14(); virtual void vf18(); virtual void vf1C();
    virtual void vf20(); virtual void vf24(); virtual void vf28(); virtual void vf2C();
    virtual void vf30(); virtual void vf34();
    virtual void ApplyBuffer_guess(s32* handle, void* buffer);    /* +0x38 */
    virtual void vf3C(); virtual void vf40(); virtual void vf44(); virtual void vf48();
    virtual void SetEnvironmentName_guess(const char* name);      /* +0x4C */
};

/* The sound-buffer layout (vtable 0x00535798) of Update's stack temporary: it has an EH lifetime and is never
 * destroyed on the normal path, so its destructor is empty. */
class EQSoundBuffer_update_local_guess
{
public:
    void* vtable;           /* 0x00 */
    f32 position_a;         /* 0x04 */
    f32 position_b;         /* 0x08 */
    f32 position_c;         /* 0x0C */
    u8 unknown0x10[0x44];   /* 0x10 */
    u32 changed_flags;      /* 0x54 */

    EQSoundBuffer_update_local_guess(void) { vtable = (void*)0x00535798; changed_flags = 0; }
    ~EQSoundBuffer_update_local_guess(void) {}
};

class EqSoundManagerUpdate_guess : public EqSoundManager_guess
{
public:
    void Update_guess(void);
};

#define SM_DWORD0_guess     (*(u32*)((char*)this + 0x00))
#define SM_DWORD4_guess     (*(u32*)((char*)this + 0x04))
#define SM_FLOAT4_guess     (*(f32*)((char*)this + 0x04))
#define SM_STATE_guess      (*(int*)((char*)this + 0x0C))
#define SM_LAST_STATE_guess (*(int*)((char*)this + 0x10))
#define SM_DISPLAY_guess    ((char*)EQ_GLOBAL(void*, 0x006E4DE0))

// FUNCTION: TRILOGY 0x004e2a20
// Updates spawn sounds, ambient effects, weather cues, and the zone's acoustic state.
// @early-stop: EQ_THISCALL dummy EDX xors, float literal pool addresses, and slot-to-buffer float copies (original fld/fstp, here int moves).
// __thiscall (called once a frame by CDisplay::RealRender_World). With sounds on (0x00579314) and the
// sound manager present: walks the 8 tracked spawns (stopping the ones IsPlayerAudible_guess rejects, restarting a
// sound when its actor_info +0x22C / +0x228 changed) and pushes each spawn's position and size into its slot's
// sound buffer. Then, every (0x15 - 0x0057931C) * 10 calls: runs the emitters (0x004E2D75), follows the
// local player's second float (+0x4C), starts / stops the weather wave 0x9E / 0x9F and picks the environment
// name (state at +0xC: 1 far / 2 / 3 near ...) for the sound control object.
void EqSoundManagerUpdate_guess::Update_guess(void)
{
    typedef float (__cdecl *s3dGetActorScaledAmbient_update_t)(void*, void*, void*);
    typedef void (__stdcall *OutputDebugStringA_update_t)(const char*);
    EQSoundBuffer_update_local_guess local;
    int i;
    int j;
    int sound_data;
    bool quiet;
    float distance;

    if (EQ_GLOBAL(int, 0x00579314) == 0 || g_sound_manager_guess == 0)
        return;
    if (active_players > 0)
    {
        for (i = 0; i < 8; i++)
        {
            if (players[i] != 0)
            {
                if (!EQ_METHOD(1, char, 0x004E27CB, (EqSoundManager_guess*, EQPlayer*), (EqSoundManager_guess*)this)(players[i]))
                {
                    EQ_METHOD(1, int, 0x004E25B5, (EqSoundManager_guess*, EQPlayer*), (EqSoundManager_guess*)this)(players[i]);
                }
                else
                {
                    sound_data = players[i]->actor_info->sound_data_b_guess;
                    if (player_data_b[i] != sound_data)
                    {
                        player_data_b[i] = sound_data;
                        EQ_METHOD(2, int, 0x004E20DF, (EqSoundManager_guess*, EQPlayer*, int), (EqSoundManager_guess*)this)(players[i], sound_data);
                    }
                    if (players[i]->actor_info->sound_data_a_guess == 0)
                        players[i]->actor_info->sound_data_a_guess = players[i]->actor_info->sound_id_guess;
                    sound_data = players[i]->actor_info->sound_data_a_guess;
                    if (player_data_a[i] != sound_data)
                    {
                        player_data_a[i] = sound_data;
                        EQ_METHOD(2, int, 0x004E20DF, (EqSoundManager_guess*, EQPlayer*, int), (EqSoundManager_guess*)this)(players[i], sound_data);
                    }
                    for (j = 0; j < 8; j++)
                    {
                        if (slots[j].player != 0)
                        {
                            if (slots[j].player == players[i] && 0 != slots[j].count)
                            {
                                slots[j].x = players[i]->y;
                                slots[j].y = players[i]->x;
                                slots[j].z = players[i]->z;
                                local.changed_flags = 0;
                                EQ_METHOD(1, bool, 0x004ECBFA, (EQSoundBuffer_guess*, float), (EQSoundBuffer_guess*)&local)(players[i]->size);
                                float temp3 = slots[j].x;
                                local.position_a = temp3;
                                {
                                    float temp2 = slots[j].z;
                                    local.position_b = temp2;
                                }
                                float temp = slots[j].y;
                                local.changed_flags |= 1;
                                local.position_c = temp;
                                if (EQ_GLOBAL(u8, 0x00579324) != 0)
                                {
                                    if (slots[j].handle != -1)
                                        ((EqSoundControl_update_guess*)g_sound_control_guess)->ApplyBuffer_guess(&slots[j].handle, &local);
                                    else
                                        EQ_GLOBAL(OutputDebugStringA_update_t, 0x00534040)((const char*)0x0057BD50);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (EQ_GLOBAL(int, 0x006ECE2C)++ <= (0x15 - EQ_GLOBAL(int, 0x0057931C)) * 10)
        return;
    EQ_GLOBAL(int, 0x006ECE2C) = 0;
    EQ_METHOD(0, void, 0x004E2D75, (EqSoundManager_guess*), (EqSoundManager_guess*)this)();
    if (unknown0x008 != g_local_player_guess->actor_info->fallStartZ_guess
        && g_local_player_guess->actor_info->actor != 0)
    {
        unknown0x008 = g_local_player_guess->actor_info->fallStartZ_guess;
        SM_DWORD0_guess = SM_DWORD4_guess;
        SM_FLOAT4_guess = EQ_GLOBAL(s3dGetActorScaledAmbient_update_t, 0x006E50F8)
            (*(void**)(SM_DISPLAY_guess + 4), *(void**)SM_DISPLAY_guess, g_local_player_guess->actor_info->actor);
        if (*(char*)(SM_DISPLAY_guess + 0xC0) != 0)
        {
            if (*(u8*)(SM_DISPLAY_guess + 0xC6) == 1)
            {
                if (EQ_METHOD(0, bool, 0x004E2044, (void*), (void*)this)())
                    EQ_METHOD(1, int, 0x004E363B, (void*, int), (void*)g_sound_manager_guess)(0x9F);
                else
                    SoundManager_WavePlay_guess(g_sound_manager_guess, 0x9F, 0);
            }
            else if (*(u8*)(SM_DISPLAY_guess + 0xC6) == 2)
            {
                if (EQ_METHOD(0, bool, 0x004E2044, (void*), (void*)this)())
                    EQ_METHOD(1, int, 0x004E363B, (void*, int), (void*)g_sound_manager_guess)(0x9E);
                else
                    SoundManager_WavePlay_guess(g_sound_manager_guess, 0x9E, 0);
            }
        }
    }
    if (SM_STATE_guess < 4)
    {
        distance = EQ_METHOD(0, float, 0x004E2058, (void*), (void*)this)();
        if (distance < 0.0f)
            distance = 0.0f;
        if (distance <= 500.0f)
        {
            if (distance <= 20.0f)
                SM_STATE_guess = 3;
            else
            {
                quiet = EQ_METHOD(0, bool, 0x004E2044, (void*), (void*)this)();
                SM_STATE_guess = quiet ? 2 : 1;
            }
        }
        else
            SM_STATE_guess = 1;
        if (g_sound_manager_guess != 0 && *(char*)g_sound_manager_guess != 0 && SM_STATE_guess != SM_LAST_STATE_guess)
        {
            switch (SM_STATE_guess)
            {
            case 1:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)((char*)this + 0x94));
                break;
            case 2:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)((char*)this + 0x14));
                break;
            case 3:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)((char*)this + 0x54));
                break;
            case 4:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)0x00579390);
                break;
            case 5:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)0x00579380);
                break;
            case 6:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)0x00579388);
                break;
            default:
                ((EqSoundControl_update_guess*)g_sound_control_guess)->SetEnvironmentName_guess((const char*)0x00579398);
                break;
            }
            SM_LAST_STATE_guess = SM_STATE_guess;
        }
    }
}

#undef SM_DWORD0_guess
#undef SM_DWORD4_guess
#undef SM_FLOAT4_guess
#undef SM_STATE_guess
#undef SM_LAST_STATE_guess
#undef SM_DISPLAY_guess

/* This vtable view names only the two slots WavePlay uses directly. */
class EqSoundControl_waveplay_guess
{
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int CreateWave_guess(int index);                  /* +0x18 -1 = failed */
    virtual void vf1C();
    virtual void PlayWave_guess(int index, void* buffer);     /* +0x20 */
};

/* The original class declaration omits this method and its two cache tables. */
class EqSoundManager_waveplay_guess : public EqSoundManager_guess
{
public:
    int WavePlay_guess(int index, int buffer);
};

/* The stack temporary has the sound-buffer layout (vtable 0x00535798) and an EH lifetime: it is never
 * destroyed on the normal path, so its destructor is empty. */
class EQSoundBuffer_waveplay_local_guess
{
public:
    void* vtable;
    u8 unknown0x04[0x50];
    u32 changed_flags;

    EQSoundBuffer_waveplay_local_guess(void) { vtable = (void*)0x00535798; changed_flags = 0; }
    ~EQSoundBuffer_waveplay_local_guess(void) {}
};

// FUNCTION: TRILOGY 0x004e34fa
// Plays a game sound effect by ID, optionally with custom sound settings.
// ORIGINAL FILE: EverQuest/EQ_SoundManager.cpp (from Mac debug map)
int EqSoundManager_waveplay_guess::WavePlay_guess(int soundId, int buffer)
{
    EQSoundBuffer_waveplay_local_guess local;
    EQSoundBuffer_guess* soundBuffer = (EQSoundBuffer_guess*)&local;
    SoundControl_guess* command = (SoundControl_guess*)this;    /* Command ignores `this` */
    int* slot;

    if (soundId >= 0x9e)
    {
        if (soundId > 0xa1)
        {
            if (soundId <= 0xa9)
            {
                slot = (int*)((char*)this + 0xd80) + soundId;
                if (*slot != -1)
                    return 1;
                *slot = ((EqSoundControl_waveplay_guess*)g_sound_control_guess)->CreateWave_guess(soundId);
                if (buffer == 0)
                {
                    local.changed_flags = 0;
                    EQ_METHOD(1, void, 0x004ECBC2, (EQSoundBuffer_guess*, u8), soundBuffer)(0x11);
                    return command->Command_guess(*slot, (int)soundBuffer);
                }
                return command->Command_guess(*slot, buffer);
            }
        }
        else
        {
            slot = (int*)((char*)this + 0xdb0) + soundId;
            if (*slot != -1)
                return 1;
            *slot = ((EqSoundControl_waveplay_guess*)g_sound_control_guess)->CreateWave_guess(soundId);
            if (*slot == -1)
                return 0;
            if (buffer == 0)
            {
                local.changed_flags = 0;
                EQ_METHOD(1, void, 0x004ECBC2, (EQSoundBuffer_guess*, u8), soundBuffer)(0x11);
                return command->Command_guess(*slot, (int)soundBuffer);
            }
            return command->Command_guess(*slot, buffer);
        }
    }

    if (buffer == 0)
    {
        local.changed_flags = 0;
        EQ_METHOD(1, void, 0x004ECBC2, (EQSoundBuffer_guess*, u8), soundBuffer)(0x10);
        if (soundId >= 0x8f && soundId <= 0x90 && EQ_GLOBAL(EqSoundLevel_guess*, 0x006E4DDC) != 0
            && EQ_GLOBAL(EqSoundLevel_guess*, 0x006E4DDC)->IsQuiet_guess())
            EQ_METHOD(1, void, 0x004ECB82, (EQSoundBuffer_guess*, int), soundBuffer)(-2500);
        ((EqSoundControl_waveplay_guess*)g_sound_control_guess)->PlayWave_guess(soundId, &local);
    }
    else
    {
        ((EqSoundControl_waveplay_guess*)g_sound_control_guess)->PlayWave_guess(soundId, (void*)buffer);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004e3624
// Passes an audio command to the sound system; its game-specific purpose is unclear.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; Mac: SoundControl::SoundControl, low; queue label SoundManager)
// __thiscall that ignores `this`. Forwards to the sound control object's slot 0x24 with the address of
// the first argument. The Mac name (a constructor) does not fit what the code does.
int SoundControl_guess::Command_guess(int arg, int arg2)
{
    return g_sound_control_guess->Command_guess(&arg, arg2);
}

// FUNCTION: TRILOGY 0x004e370a
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. 1 when wave `index` (0..719) is loaded.
int EqSoundManager_guess::IsWaveLoaded_guess(int index)
{
    if (index >= 0 && index < 0x2D0 && waves[index] != 0)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x004e372d
// Loads a group of sound effect files into consecutive game sound slots.
char __fastcall LoadWaveList_guess(EqSoundManager_guess* self, int unused_edx, char** pathList, int baseIndex)
{
    char firstChar;
    int indexOffset;

    indexOffset = 0;
    while (true)
    {
        firstChar = **pathList;
        if (firstChar == '@')
            return 1;
        if (firstChar == '\0')
            break;
        EQ_METHOD(2, int, 0x004E3768, (EqSoundManager_guess*, char*, int), self)(*pathList, baseIndex + indexOffset);
        indexOffset = indexOffset + 1;
        pathList = pathList + 1;
        if (indexOffset >= 0x2d0)
            return 1;
    }
    return 1;
}

class EqSoundControlLoadWave_guess
{
public:
    virtual void vf00();
    virtual void vf04();
    virtual void LoadWaveAs3DStatic_guess(char* path, int index, int unused);
};

// FUNCTION: TRILOGY 0x004e3768
// Makes a named sound effect available to the zone's 3D sound system.
// __thiscall (self in ECX; path and index on the stack, callee pops 8). Loads wave `index` (0..719) from
// `path` unless the same path (case-insensitive) is already in that slot; 0 for a bad index (logged).
// edx is the caller's EDX, passed on untouched to IsWaveLoaded (a __thiscall that ignores it).
char __fastcall LoadNamedWave_guess(EqSoundManager_guess* self, int edx, char* path, int index)
{
    char pathBuffer[260];

    if (index < 0 || index >= 0x2D0)
    {
        EQTRACE_guess((const char*)0x0057BE7C, index);
        LogMessage_guess((const char*)0x0057BE7C, index);
        return 0;
    }
    if (EQ_THISCALL(int, 0x004E370A, (EqSoundManager_guess*, int, int))(self, edx, index) != 0 &&
        _strcmpi(path, (const char*)self->waves[index]) == 0)
    {
        return 1;
    }
    strcpy(pathBuffer, path);
    ((EqSoundControlLoadWave_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->LoadWaveAs3DStatic_guess(pathBuffer, index, 0);
    self->waves[index] = path;
    return 1;
}

// FUNCTION: TRILOGY 0x004e38d0
// Initializes the client's standard sound effect entries.
void __fastcall InitializeSoundEntries_guess(char* self)
{
    if (self[0x1048] == '\0')
    {
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577B48, 0x20);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577B74, 0x2A);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577BA0, 0x34);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577BCC, 0x3E);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577BF8, 0x48);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577C24, 0x52);
        EQ_METHOD(2, char, 0x004E372D, (char*, char**, int), (char*)self)((char**)0x00577CA8, 0x5C);
        *(int*)(self + 0x10) = 1;
        EQ_METHOD(0, void, 0x004E34F5, (char*), (char*)self)();
        self[0x1048] = 1;
    }
}

// The 0x58-byte sound descriptor this function fills in and hands to the sound control object: an
// EQSoundBuffer_guess (vtable pointer at +0, changed flags at +0x54). The constructor sets both, and the
// empty destructor is what gives the function its EH frame.
class EqSoundBufferScratch_guess : public EQSoundBuffer_guess
{
public:
    EqSoundBufferScratch_guess() { *(void**)this = (void*)0x00535798; changed_flags = 0; }
    ~EqSoundBufferScratch_guess() {}
};

// The sound control object (0x006E4DD0) seen through the vtable slots LoadOldEmitters calls.
class EqSoundControlSlots_guess
{
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08(); virtual void vf0C();
    virtual void vf10(); virtual void vf14();
    virtual int  CreateHandle_guess(int sound);                       /* +0x18 */
    virtual void ReleaseHandle_guess(int* handle);                    /* +0x1C */
    virtual void vf20(); virtual void vf24(); virtual void vf28(); virtual void vf2C();
    virtual void vf30(); virtual void vf34();
    virtual void SetData_guess(int* handle, void* params);            /* +0x38 */
};

// FUNCTION: TRILOGY 0x004e3953
// Loads a dynamic sound bank file: its EMIT, LOOP and RAND wave lists, loading each wave and making its handle.
// ORIGINAL FILE: EverQuest/EQ_SoundManager.cpp (from Mac debug map)
// @early-stop: the EQ_THISCALL and typedef calls' dummy `xor edx,edx`s; loop 3 has handle/path in EBX/EDI (original EDI/EBX); the cdecl pop after EQTRACE merges into fscanf's add esp
void __fastcall EqSoundManager_LoadOldEmitters_guess(char* self, int unusedEdx, char* fileName)
{
    struct LoadOldEmitters_locals_guess
    {
        char token[0xd0];
        char unknown0xd0[0x30];
        EqSoundBufferScratch_guess soundParameters;
    };

    typedef void (__fastcall *ReleaseSoundHandle_guess_t)(void*, int, int*);
    typedef int (__fastcall *CreateSoundHandle_guess_t)(void*, int, int);

    char loaded;
    int tmp;
    int value;
    char* path;
    int* handle;
    char hasSectionEnd;
    char atEof;
    LoadOldEmitters_locals_guess locals;
    char* token = locals.token;
    char* soundParameters = (char*)&locals.soundParameters;

    hasSectionEnd = 0;
    atEof = 0;
    if (self[0x1049] != '\0')
        EQ_METHOD(0, void, 0x004E3E1C, (char*), (char*)self)();

    if (EQ_GLOBAL(int, 0x006E4DD8) != 0)
    {
        fileName = (char*)EQ_CDECL(void*, 0x005223BC, (const char*, const char*))
            (fileName, (const char*)0x00568000);
        if (fileName != 0)
        {
            EQTRACE_guess(EQ_ARRAY(char, 0x0057BF58));
            tmp = EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
            if (tmp != -1)
            {
                tmp = strcmp(token, EQ_ARRAY(char, 0x0057BF50));
                if (tmp == 0)
                {
                    while (1)
                    {
                        tmp = EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                            ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                        if (tmp == -1)
                        {
                            atEof = 1;
                            break;
                        }
                        tmp = strcmp(token, EQ_ARRAY(char, 0x0057BF48));
                        if (tmp == 0)
                        {
                            hasSectionEnd = 1;
                            break;
                        }
                        tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                            (token, EQ_ARRAY(char, 0x0057BF40));
                        if (tmp == 0)
                        {
                            tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                                (token, EQ_ARRAY(char, 0x0057BF38));
                            if (tmp == 0)
                                strcat(token, EQ_ARRAY(char, 0x0057BF40));
                        }
                        strcpy(self + *(int*)(self + 4) * 0x20 + 0x138, token);
                        value = *(int*)(self + 4);
                        tmp = value + 1;
                        *(int*)(self + 4) = tmp;
                        if (0x14 < value)
                            break;
                        strcpy(self + tmp * 0x20 + 0x138, EQ_ARRAY(char, 0x0055D6D4));
                    }
                }
                // atEof: the scan ran out of tokens, so nothing else is read or compared
                if (atEof == 0)
                {
                    // hasSectionEnd: loop A left the token in hand as the next one to check (the section-B
                    // marker), so the read is skipped
                    if (hasSectionEnd == 0)
                    {
                        EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                            ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                    }
                    tmp = strcmp(token, EQ_ARRAY(char, 0x0057BF48));
                    if (tmp == 0)
                    {
                        while (1)
                        {
                            tmp = EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                                ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                            if (tmp == -1)
                            {
                                atEof = 1;
                                break;
                            }
                            tmp = strcmp(token, EQ_ARRAY(char, 0x0057BF30));
                            if (tmp == 0)
                                break;
                            tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                                (token, EQ_ARRAY(char, 0x0057BF40));
                            if (tmp == 0)
                            {
                                tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                                    (token, EQ_ARRAY(char, 0x0057BF38));
                                if (tmp == 0)
                                    strcat(token, EQ_ARRAY(char, 0x0057BF40));
                            }
                            strcpy(self + *(int*)(self + 8) * 0x20 + 0x18, token);
                            value = *(int*)(self + 8);
                            tmp = value + 1;
                            *(int*)(self + 8) = tmp;
                            if (8 < value)
                            {
                                EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                                    ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                                break;
                            }
                            strcpy(self + tmp * 0x20 + 0x18, EQ_ARRAY(char, 0x0055D6D4));
                        }
                    }
                    else
                    {
                        if (hasSectionEnd == 0)
                        {
                            EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                                ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                        }
                    }
                }

                if (atEof == 0)
                {
                    tmp = strcmp(token, EQ_ARRAY(char, 0x0057BF30));
                    if (tmp == 0)
                    {
                        while (1)
                        {
                            tmp = EQ_CDECL(int, 0x005241C5, (void*, const char*, ...))
                                ((void*)fileName, EQ_ARRAY(char, 0x0053E678), token);
                            if (tmp == -1)
                                break;
                            tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                                (token, EQ_ARRAY(char, 0x0057BF40));
                            if (tmp == 0)
                            {
                                tmp = (int)EQ_CDECL(char*, 0x00522A30, (const char*, const char*))
                                    (token, EQ_ARRAY(char, 0x0057BF38));
                                if (tmp == 0)
                                    strcat(token, EQ_ARRAY(char, 0x0057BF40));
                            }
                            strcpy(self + *(int*)(self + 0xc) * 0x20 + 0x3d8, token);
                            value = *(int*)(self + 0xc);
                            tmp = value + 1;
                            *(int*)(self + 0xc) = tmp;
                            if (4 < value)
                                break;
                            strcpy(self + tmp * 0x20 + 0x3d8, EQ_ARRAY(char, 0x0055D6D4));
                        }
                    }
                }
            }
            EQ_CDECL(int, 0x00521FBB, (void*))((void*)fileName);

            if (*(int*)(self + 4) != 0)
            {
                locals.soundParameters.changed_flags = 0;
                EQ_METHOD(1, void, 0x004ECBC2, (void*, u8), (void*)soundParameters)(1);
                path = self + 0x138;
                handle = (int*)(self + 0xfb8);
                fileName = path;
                tmp = 1;
                while (1)
                {
                    if ((*path == '@') || (*path == '\0'))
                        break;
                    loaded = EQ_METHOD(2, char, 0x004E3768, (void*, char*, int), EQ_GLOBAL(void*, 0x006E4DD8))(path, tmp);
                    if (loaded != '\0')
                    {
                        if (*handle != -1)
                            ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->ReleaseHandle_guess(handle);
                        value = ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->CreateHandle_guess(tmp);
                        *handle = value;
                        ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->SetData_guess(handle,
                            soundParameters);
                    }
                    fileName += 0x20;
                    handle += 1;
                    if (tmp++ >= 0x14)
                        break;
                    path = fileName;
                }
            }

            tmp = 0;
            if (*(int*)(self + 8) != 0)
            {
                locals.soundParameters.changed_flags = 0;
                EQ_METHOD(1, void, 0x004ECBC2, (void*, u8), (void*)soundParameters)(0x11);
                fileName = self + 0x18;
                handle = (int*)(self + 0x1008);
                while (1)
                {
                    path = fileName;
                    if ((*path == '@') || (*path == '\0'))
                        break;
                    loaded = EQ_METHOD(2, char, 0x004E3768, (void*, char*, int), EQ_GLOBAL(void*, 0x006E4DD8))(path,
                         EQ_GLOBAL(int, 0x006ECCF0) + tmp);
                    if (loaded != '\0')
                    {
                        if (*handle != -1)
                            ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->ReleaseHandle_guess(handle);
                        value = ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->CreateHandle_guess(
                            EQ_GLOBAL(int, 0x006ECCF0) + tmp);
                        *handle = value;
                        ((EqSoundControlSlots_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->SetData_guess(handle,
                            soundParameters);
                    }
                    fileName += 0x20;
                    tmp += 1;
                    handle += 1;
                    if (tmp >= 8)
                        break;
                }
            }

            if (*(int*)(self + 0xc) != 0)
            {
                fileName = 0;
                handle = (int*)(self + 0x1038);
                path = self + 0x3d8;
                do
                {
                    if ((*path == '@') || (*path == '\0'))
                        break;
                    loaded = EQ_METHOD(2, char, 0x004E3768, (void*, char*, int), EQ_GLOBAL(void*, 0x006E4DD8))(path,
                         *(int*)(self + 8) + EQ_GLOBAL(int, 0x006ECCF0) + (int)fileName);
                    if (loaded != '\0')
                    {
                        if (*handle != -1)
                        {
                            void* soundControl = EQ_GLOBAL(void*, 0x006E4DD0);
                            ((ReleaseSoundHandle_guess_t)(*(void***)soundControl)[7])
                                (soundControl, 0, handle);
                        }
                        {
                            void* soundControl = EQ_GLOBAL(void*, 0x006E4DD0);
                            tmp = ((CreateSoundHandle_guess_t)(*(void***)soundControl)[6])
                                (soundControl, 0, *(int*)(self + 8) +
                                 EQ_GLOBAL(int, 0x006ECCF0) + (int)fileName);
                        }
                        *handle = tmp;
                    }
                    fileName = (char*)((int)fileName + 1);
                    handle += 1;
                    path += 0x20;
                } while ((int)fileName < 4);
            }
            *(int*)(self + 0x14) = 0;
            self[0x1049] = 1;
        }
    }
}

// FUNCTION: TRILOGY 0x004e3e1c
// Resets the sound manager's indexed sound entry state.
void __fastcall ResetSoundManagerState_guess(char* self)
{
    int waveIndex;
    int entryIndex;

    if ((self[0x1049] != '\0') && (EQ_GLOBAL(int, 0x006E4DD8) != 0))
    {
        entryIndex = 1;
        if (1 <= EQ_GLOBAL(int, 0x00577ABC))
        {
            do
            {
                EQ_METHOD(1, void, 0x004E3833, (void*, int), (void*)EQ_GLOBAL(void*, 0x006E4DD8))(entryIndex);
                entryIndex = entryIndex + 1;
            } while (entryIndex <= EQ_GLOBAL(int, 0x00577ABC));
        }
        waveIndex = EQ_GLOBAL(int, 0x006ECCF0);
        if (EQ_GLOBAL(int, 0x006ECCF0) < 0x2D0)
        {
            do
            {
                EQ_METHOD(1, void, 0x004E3833, (void*, int), (void*)EQ_GLOBAL(void*, 0x006E4DD8))(waveIndex);
                waveIndex = waveIndex + 1;
            } while (waveIndex < 0x2D0);
        }
        *(int*)(self + 4) = 0;
        *(int*)(self + 8) = 0;
        *(int*)(self + 0xC) = 0;
        self[0x1049] = '\0';
    }
    return;
}

// The sound control object (0x006E4DD0) seen through the two vtable slots the reverb toggle uses.
class EqSoundControlReverb_guess
{
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08(); virtual void vf0C();
    virtual void vf10(); virtual void vf14(); virtual void vf18(); virtual void vf1C();
    virtual void vf20(); virtual void vf24(); virtual void vf28(); virtual void vf2C();
    virtual void vf30(); virtual void vf34(); virtual void vf38(); virtual void vf3C();
    virtual void vf40(); virtual void vf44(); virtual void vf48();
    virtual int  SetReverbPreset_guess(const char* name);             /* +0x4C */
    virtual void vf50(); virtual void vf54();
    virtual int  GetCaps_guess(u8* caps);                             /* +0x58 byte 5 = reverb available */
};

// FUNCTION: TRILOGY 0x004e3e89
// Changes the ambience heard in game by enabling or disabling sound reverb.
// __thiscall (no declaration in the header, so a __fastcall free function) on an object whose first byte
// is the reverb switch. Toggles reverb when the sound control reports it available (off also clears the
// environment state and selects the "None" preset) and prints the outcome.
void __fastcall ToggleReverb_guess(char* reverb_on)
{
    char text[32];
    u8 caps[8];

    if (g_sound_control_guess == 0)
        return;
    ((EqSoundControlReverb_guess*)g_sound_control_guess)->GetCaps_guess(caps);
    if (caps[5] != 0)
    {
        if (*reverb_on != 0)
        {
            *reverb_on = 0;
            sprintf(text, (const char*)0x0057BFB0);
            if (EQ_GLOBAL(void*, 0x006E4DDC) != 0)
                EQ_METHOD(1, void, 0x004E1F74, (void*, int), EQ_GLOBAL(void*, 0x006E4DDC))(-1);
            ((EqSoundControlReverb_guess*)g_sound_control_guess)->SetReverbPreset_guess((const char*)0x00579398);
        }
        else
        {
            *reverb_on = 1;
            sprintf(text, (const char*)0x0057BFA4);
        }
    }
    else
    {
        sprintf(text, (const char*)0x0057BF8C);
    }
    EQ_METHOD(1, void, 0x004D05B7, (void*, const char*), (void*)pinstCEverQuest_guess)(text);
}

// FUNCTION: TRILOGY 0x004e3f12
// Creates the Direct3D scene with its root and camera frames.
// __fastcall, ECX = scene: [0] IDirect3DRM, [1] root frame, [2] camera frame.
// Creates them (camera under root), then calls vtable +0x68 on the camera.
void** __fastcall Create3DSceneFrames_guess(void** scene, int unused_edx)
{
    typedef int (__stdcall *CreateFrameFn_guess)(void*, void*, void**);
    typedef int (__stdcall *AddChildFn_guess)(void*, void*);
    void** rootSlot;
    void** camera;
    int hrCreate = EQ_STDCALL(int, 0x00501480, (void**))(scene);
    int& hr = hrCreate;
    if (hr < 0)
    {
        LogMessage_guess((const char*)0x0057C004);
        EQ_STDCALL(void, 0x00522DAD, (int))(0);
    }
    rootSlot = scene + 1;
    hr = ((CreateFrameFn_guess)(*(void***)*scene)[4])(*scene, 0, rootSlot);
    if (hr < 0)
    {
        LogMessage_guess((const char*)0x0057BFE0);
        EQ_STDCALL(void, 0x00522DAD, (int))(0);
    }
    camera = scene + 2;
    hr = ((CreateFrameFn_guess)(*(void***)*scene)[4])(*scene, *rootSlot, camera);
    if (hr < 0)
    {
        LogMessage_guess((const char*)0x0057BFBC);
        EQ_STDCALL(void, 0x00522DAD, (int))(0);
    }
    camera = (void**)*camera;
    ((AddChildFn_guess)(*(void***)camera)[26])(camera, rootSlot);
    return scene;
}

// FUNCTION: TRILOGY 0x004e3f8a
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Releases the held COM object and clears the pointer.
void EqComHolder_guess::Release_guess(void)
{
    if (object != 0)
    {
        object->Release_guess();
        object = 0;
    }
}

/* The sound control object's vtable up to the listener slot (+0x44). */
class EqSoundControl_listener_guess
{
public:
    virtual void vf00(); virtual void vf04(); virtual void vf08(); virtual void vf0C();
    virtual void vf10(); virtual void vf14(); virtual void vf18(); virtual void vf1C();
    virtual void vf20(); virtual void vf24(); virtual void vf28(); virtual void vf2C();
    virtual void vf30(); virtual void vf34(); virtual void vf38(); virtual void vf3C();
    virtual void vf40();
    virtual void SetListener_guess(void* buffer);                 /* +0x44 */
};

/* The sound-buffer layout (vtable 0x00535798) of the listener setter's stack temporary: the position (y, z, x),
 * the orientation matrix entries at 0x1C..0x30 and the changed flags at 0x54. It has an EH lifetime and is never
 * destroyed on the normal path, so its destructor is empty. */
class EQSoundBuffer_listener_local_guess
{
public:
    void* vtable;           /* 0x00 */
    f32 position_y;         /* 0x04 */
    f32 position_z;         /* 0x08 */
    f32 position_x;         /* 0x0C */
    u8 unknown0x10[0x0C];   /* 0x10 */
    f32 orientation[6];     /* 0x1C */
    u8 unknown0x34[0x20];   /* 0x34 */
    u32 changed_flags;      /* 0x54 */

    EQSoundBuffer_listener_local_guess(void) { vtable = (void*)0x00535798; }
    ~EQSoundBuffer_listener_local_guess(void) {}
};

// FUNCTION: TRILOGY 0x004e3f9e
// Updates the 3D audio listener from the player, or the camera when no player is active.
// __thiscall in the original (both callers load ECX, the code never reads it), written as a __fastcall free
// function with an EDX dummy, as the other ECX-only callees are. Callee pops 4 bytes. Builds the listener from
// the local player (position; heading, pitch and a roll of 256 in the mode at 0x006B6F08 == 1, else 0), or from
// the camera `camera` when there is no local player, and hands it to the sound control object's slot +0x44.
// @early-stop: nine float copies still use GPR MOV pairs instead of the original FLD/FSTP pairs
void __fastcall SetSoundListener_guess(void* unused_this, int unused_edx, void* camera)
{
    typedef void (__cdecl *t3dGetCameraLocation_listener_t)(void*, float*);
    typedef void (__cdecl *t3dCalculateCoefficients_listener_t)(float, float, float, float*);
    float location[7]; /* fake match: reserve the adjacent unused float at EBP-0x10 in the original frame */
    float matrix[9];
    EQPlayer* player = g_local_player_guess;

    if (player != 0)
    {
        location[3] = player->heading;
        location[4] = player->pitch;
        if (EQ_GLOBAL(int, 0x006B6F08) == 1)
            location[5] = 256.0f;
        else
            location[5] = 0.0f;
        location[0] = player->y;
        location[1] = player->x;
        location[2] = player->z;
    }
    else
    {
        EQ_GLOBAL(t3dGetCameraLocation_listener_t, 0x006E5290)(camera, location);
    }
    EQ_GLOBAL(t3dCalculateCoefficients_listener_t, 0x006E5168)(location[3], location[4], location[5], matrix);
    EQSoundBuffer_listener_local_guess buffer;
    buffer.position_y = location[0];
    buffer.position_z = location[2];
    buffer.position_x = location[1];
    buffer.orientation[0] = matrix[0];
    buffer.orientation[1] = matrix[2];
    buffer.orientation[2] = matrix[1];
    buffer.orientation[3] = matrix[6];
    buffer.orientation[4] = matrix[8];
    buffer.orientation[5] = matrix[7];
    buffer.changed_flags = 0xD;
    ((EqSoundControl_listener_guess*)g_sound_control_guess)->SetListener_guess(&buffer);
}

// FUNCTION: TRILOGY 0x004e4099
// Opens the Miles digital driver for the MIDI fallback; an 'Emulated' device is reopened with preference 15 set.
// @early-stop: last two imuls of local_10 are ordered aa8-then-ab4 (VC6 reassociates that product onto local_c's); 2 instructions.
/* fake match: local_10 is assigned before local_c; with local_c first VC6 reassociates local_10 onto local_c's product. */
s32 OpenDigitalDriver_guess(void)
{
    typedef void (__stdcall *AIL_set_preference_t)(s32, s32);
    typedef s32 (__stdcall *AIL_waveOutOpen_t)(u32*, s32, s32, void*);
    typedef void (__stdcall *AIL_digital_configuration_t)(u32, s32, s32, char*);
    typedef void (__stdcall *AIL_waveOutClose_t)(u32);
    AIL_set_preference_t pAIL_set_preference;
    AIL_waveOutOpen_t pAIL_waveOutOpen;
    u32 preferenceValue;
    int iVar2;
    size_t length;
    char statusText[100];
    char deviceText[128];
    struct
    {
        u16 local_18;
        s16 local_16;
        s32 local_14;
        s32 local_10;
        s16 local_c;
        u16 local_a;
    } waveFormat;
    u32 digitalDriver;

    preferenceValue = EQ_GLOBAL(u32, 0x00577AE4);
    pAIL_set_preference = EQ_GLOBAL(AIL_set_preference_t, 0x00534314);
    pAIL_set_preference(0xf, 0);
    pAIL_set_preference(0x21, 1);
    pAIL_set_preference(0x1b, 1);
    pAIL_set_preference(1, EQ_GLOBAL(s32, 0x00577AC0));
    pAIL_set_preference(2, 0x80);
    pAIL_set_preference(0x19, 1);
    pAIL_set_preference(0x28, 0x10000);
    pAIL_set_preference(0x14, 0x78);
    pAIL_set_preference(0x18, 10);
    pAIL_set_preference(0x1c, 1);
    pAIL_set_preference(0x12, 1);
    pAIL_set_preference(0x15, preferenceValue);

    waveFormat.local_18 = 1;
    waveFormat.local_14 = EQ_GLOBAL(s32, 0x00577AB4);
    waveFormat.local_16 = (s16)EQ_GLOBAL(s32, 0x00577AA8);
    waveFormat.local_10 = (EQ_GLOBAL(s32, 0x00577AAC) / 8) * EQ_GLOBAL(s32, 0x00577AB4) * EQ_GLOBAL(s32, 0x00577AA8);
    waveFormat.local_c = (s16)((EQ_GLOBAL(s32, 0x00577AAC) / 8) * EQ_GLOBAL(s32, 0x00577AA8));
    waveFormat.local_a = (u16)EQ_GLOBAL(u32, 0x00577AAC);
    pAIL_waveOutOpen = EQ_GLOBAL(AIL_waveOutOpen_t, 0x00534310);
    iVar2 = pAIL_waveOutOpen(&digitalDriver, 0, -1, &waveFormat);
    if (iVar2 == 0)
    {
        strcpy(deviceText, EQ_ARRAY(char, 0x0057C060));
        length = strlen(deviceText);
        EQ_GLOBAL(AIL_digital_configuration_t, 0x0053430C)(digitalDriver, 0, 0, deviceText + length);
        strcpy(statusText, deviceText);
        iVar2 = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(deviceText, EQ_ARRAY(char, 0x0057C054));
        if (iVar2 == 0)
        {
            return digitalDriver;
        }
        EQ_GLOBAL(AIL_waveOutClose_t, 0x00534300)(digitalDriver);
        pAIL_set_preference(0xf, 1);
        iVar2 = pAIL_waveOutOpen(&digitalDriver, 0, -1, &waveFormat);
        if (iVar2 != 0)
        {
            strcpy(statusText, EQ_ARRAY(char, 0x0057C034));
            return 0;
        }
        return digitalDriver;
    }
    strcpy(statusText, EQ_ARRAY(char, 0x0057C020));
    return 0;
}

// FUNCTION: TRILOGY 0x004e41fe
// Sets up MIDI playback with the DLS instrument bank.
s32 DLS_init_driver(s32 value)
{
    typedef s32 (__stdcall *AIL_midiOutOpen_t)(u32*, s32, s32);
    typedef const char* (__stdcall *AIL_last_error_t)(void);
    typedef u32 (__stdcall *AIL_DLS_open_t)(u32, s32, s32, s32, u32, u32, u32);
    typedef void (__stdcall *AIL_midiOutClose_t)(u32);
    typedef u32 (__stdcall *AIL_file_read_t)(const char*, s32);
    typedef u32 (__stdcall *AIL_DLS_load_memory_t)(u32, u32, s32);
    typedef void (__stdcall *AIL_DLS_set_reverb_t)(u32, float, float, float);
    s32 result;
    u32 midiOutputHandle;

    if (value == 0)
        return 0;
    result = EQ_GLOBAL(AIL_midiOutOpen_t, 0x00534330)(&midiOutputHandle, 0, -2);
    if (result != 0)
    {
        EQTRACE_guess(
            (const char*)0x0057c0a8,
            EQ_GLOBAL(AIL_last_error_t, 0x0053432C)());
        return 0;
    }
    EQ_GLOBAL(u32, 0x006ECDC0) = EQ_GLOBAL(AIL_DLS_open_t, 0x00534328)(
        midiOutputHandle, value, 0, 0, EQ_GLOBAL(u32, 0x00577AB4),
        EQ_GLOBAL(u32, 0x00577AAC), EQ_GLOBAL(u32, 0x00577AA8));
    if (EQ_GLOBAL(u32, 0x006ECDC0) == 0)
    {
        EQ_GLOBAL(AIL_midiOutClose_t, 0x00534324)(midiOutputHandle);
        return 0;
    }
    EQ_GLOBAL(u32, 0x006ECDC8) = EQ_GLOBAL(AIL_file_read_t, 0x00534320)(
        (const char*)0x0057c098, 0);
    if (EQ_GLOBAL(u32, 0x006ECDC8) == 0)
    {
        EQTRACE_guess((const char*)0x0057c06c);
        return 0;
    }
    EQ_GLOBAL(u32, 0x006ECDC4) = EQ_GLOBAL(AIL_DLS_load_memory_t, 0x0053431C)(
        EQ_GLOBAL(u32, 0x006ECDC0), EQ_GLOBAL(u32, 0x006ECDC8), 0);
    if (EQ_GLOBAL(s32, 0x00579320) >= 0x3c)
    {
        EQ_GLOBAL(AIL_DLS_set_reverb_t, 0x00534318)(
            EQ_GLOBAL(u32, 0x006ECDC0), EQ_GLOBAL(float, 0x00535148),
            EQ_GLOBAL(float, 0x00535740), EQ_GLOBAL(float, 0x005357B8));
    }
    return midiOutputHandle;
}

// FUNCTION: TRILOGY 0x004e42d3
// Filters MIDI note-ons by channel for selected music playback modes.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; address taken by IAMidi)
// __stdcall MIDI event callback (EBP frame in the original). Mode 1: drops note-ons (0x9n) whose channel is
// at or above the threshold; mode 3: drops note-ons of channels not in the mask (a nonzero data_2 = real
// note-on, outside a nested call). Returns 1 for a dropped event; otherwise forwards it (when the forward
// switch is 3) and returns 0.
// @early-stop: without volatile, VC6 caches the filter mode in CL for both compares and folds each counter
// update into a memory INC/DEC; the original re-reads the mode and does load/inc/store through a register
// (ESI on the note-on path, which the prologue saves).
int __stdcall MidiEventFilter_guess(void* driver, void* sequence, s32 status, s32 data_1, s32 data_2)
{
    if (g_midi_filter_mode_guess != 0)
    {
        if (g_midi_filter_mode_guess == 1)
        {
            s32 st = (u8)status;
            switch (st & 0xF0)
            {
            case 0x80:
            {
                int depth = g_midi_note_off_depth_guess;
                g_midi_note_off_depth_guess += 1;
                if (depth == 0)
                {
                    int current_depth = g_midi_note_off_depth_guess;
                    current_depth--;
                    g_midi_note_off_depth_guess = current_depth;
                }
                break;
            }
            case 0x90:
            {
                int depth = g_midi_note_on_depth_guess;
                g_midi_note_on_depth_guess += 1;
                if (depth == 0 && data_2 != 0 && (st & 0xF) >= g_midi_filter_channel_guess)
                {
                    int current_depth = g_midi_note_on_depth_guess;
                    current_depth--;
                    g_midi_note_on_depth_guess = current_depth;
                    return 1;
                }
                int current_depth = g_midi_note_on_depth_guess;
                current_depth--;
                g_midi_note_on_depth_guess = current_depth;
                break;
            }
            }
        }
        else if (g_midi_filter_mode_guess == 3)
        {
            s32 st = (u8)status;
            if ((st & 0xF0) == 0x90)
            {
                int depth = g_midi_note_on_depth_guess;
                g_midi_note_on_depth_guess += 1;
                if (depth == 0 && data_2 != 0
                    && (g_midi_filter_mask_guess & (1 << (st & 0xF))) == 0)
                {
                    int current_depth = g_midi_note_on_depth_guess;
                    current_depth--;
                    g_midi_note_on_depth_guess = current_depth;
                    return 1;
                }
                int current_depth = g_midi_note_on_depth_guess;
                current_depth--;
                g_midi_note_on_depth_guess = current_depth;
            }
        }
    }
    if (g_midi_filter_on_guess == 3)
        g_midi_next_callback_guess(driver, sequence, status, data_1, data_2);
    return 0;
}

// FUNCTION: TRILOGY 0x004e43e3
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Copies the file image and sequence handles to the globals; false if either is missing.
bool EqMidiSequence_guess::PublishHandles_guess(void)
{
    if (file_image != 0)
    {
        g_midi_file_image_guess = file_image;
        if (sequence != 0)
        {
            g_midi_sequence_guess = sequence;
            return true;
        }
    }
    return false;
}

typedef void* (__stdcall *AIL_file_read_guess_t)(const char* filename, void* dest);
typedef void* (__stdcall *AIL_allocate_sequence_handle_guess_t)(void* driver);

// FUNCTION: TRILOGY 0x004e4401
// Makes a MIDI song's tracks available to the music player.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// Loads the XMIDI file `path` (AIL_file_read); the word at +0x14 of the image (capped at 64) becomes the
// track count at 0x04, 0 when the file is missing or 20 bytes or less. Allocates the sequence handle from
// the MIDI driver (0x006ECDF4 +0x60). False (image cleared) without a driver. `this` in ECX (no header
// method for it).
/* fake match: bad_image as a value keeps the clear arm first; a direct if lays out the read arm first */
bool __fastcall LoadSequence_guess(EqMidiSequence_guess *self, int unused_edx, const char* path)
{
    struct _stat st;
    u16 count;
    u16 *tracks;

    if (EQ_GLOBAL(char *, 0x006ECDF4) != 0 && *(void **)(EQ_GLOBAL(char *, 0x006ECDF4) + 0x60) != 0)
    {
        int bad_image = (self->file_image = EQ_GLOBAL(AIL_file_read_guess_t, 0x00534320)(path, 0)) == 0
            || _stat(path, &st) || st.st_size <= 0x14
            || (tracks = (u16 *)((char *)self->file_image + 0x14)) == 0;
        if (bad_image != 0)
        {
            self->unknown0x04 = 0;
        }
        else
        {
            self->unknown0x04 = count = *tracks;
            if (count > 0x40)
                self->unknown0x04 = 0x40;
        }
        self->sequence = EQ_GLOBAL(AIL_allocate_sequence_handle_guess_t, 0x00534334)(
            *(void **)(EQ_GLOBAL(char *, 0x006ECDF4) + 0x60));
        return true;
    }
    self->file_image = 0;
    return false;
}

// FUNCTION: TRILOGY 0x004e4482
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// Constructor (returns `this`). Default state.
EqMidiSequence_guess::EqMidiSequence_guess()
{
    int i;

    file_image = 0;
    sequence = 0;
    unknown0x04 = 0;
    unknown0x44 = 0;
    unknown0x48 = 0;
    for (i = 0; i < 2; i++)
    {
        unknown0x10[i] = 0xFF;
        unknown0x20[i] = 0x6E;
        unknown0x2C[i] = 500;
        unknown0x38[i] = 0;
    }
    current = -1;
    unknown0x1C = 1000;
    state = 1;
    unknown0x0C = 0;
    unknown0x08 = 0;
}

// FUNCTION: TRILOGY 0x004e44d3
// Stops one MIDI sequence and releases its handle and loaded song data.
void __fastcall ReleaseMidiSequence_guess(int* midiState)
{
    typedef void (__stdcall *AIL_release_sequence_handle_guess_t)(void*);
    typedef void (__stdcall *AIL_mem_free_lock_guess_t)(void*);

    if (midiState[0x15] == 6)
    {
        EQ_METHOD(1, void, 0x004E4536, (int*, int), midiState)(0);
        while (midiState[0x15] != 1)
        {
            EQ_METHOD(0, void, 0x004E465A, (int*), midiState)();
        }
    }
    if (midiState[0x15] == 4)
    {
        do
        {
            EQ_METHOD(0, void, 0x004E465A, (int*), midiState)();
        } while (midiState[0x15] != 1);
    }
    if (midiState[0x14] != 0)
    {
        EQ_GLOBAL(AIL_release_sequence_handle_guess_t, 0x00534308)(
            (void*)midiState[0x14]);
    }
    if (*midiState != 0)
    {
        EQ_GLOBAL(AIL_mem_free_lock_guess_t, 0x00534338)((void*)*midiState);
    }
    return;
}

// FUNCTION: TRILOGY 0x004e4522
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. AIL_set_sequence_volume on the sequence at 0x50.
void EqMidiSequence_guess::SetVolume_guess(long volume, long ms)
{
    AIL_set_sequence_volume(sequence, volume, ms);
}

// FUNCTION: TRILOGY 0x004e4536
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Without a fade time ends the sequence at once (state 4), else fades it to 0 (state 2).
void EqMidiSequence_guess::Stop_guess(long fade_ms)
{
    if (fade_ms == 0)
    {
        current = -1;
        AIL_end_sequence(sequence);
        state = 4;
    }
    else
    {
        AIL_set_sequence_volume(sequence, 0, fade_ms);
        state = 2;
    }
}

// FUNCTION: TRILOGY 0x004e4570
// Starts the next queued music track with its volume, fade, and loop settings.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; strings "PlayMidi::FadeIn ...")
// __thiscall. Starts the queued track: for the zone music the oldest queue entry is taken first; the
// "pitch zero" sequence is (re)started from the published handles, then the track's sequence with its loop
// count, volume and fade; state 6. False (with an EQTRACE) when a handle is missing or an init fails.
bool EqMidiSequence_guess::FadeIn_guess(void)
{
    int i = 0;
    AIL_start_sequence_t start;
    void* zero_image;

    if (this == g_midi_music_guess && unknown0x0C != unknown0x08)
    {
        do
        {
            i = unknown0x0C;
            unknown0x0C = (i + 1) % 2;
        } while (unknown0x0C != unknown0x08);
    }
    zero_image = g_midi_file_image_guess;
    if (zero_image == 0)
    {
        EQTRACE_guess("PlayMidi::FadeIn ZeroMIDIptr is NULL!\n");
        return false;
    }
    if (file_image == 0)
    {
        EQTRACE_guess("PlayMidi::FadeIn MIDIptr is NULL!\n");
        return false;
    }
    if (AIL_init_sequence(g_midi_sequence_guess, zero_image, 3) == 0)
    {
        EQTRACE_guess("PlayMidi::FadeIn AIL_INIT_SEQUENCE for PITCH ZERO FAILED!\n");
        return false;
    }
    *(u32*)((char*)g_midi_sequence_guess + 0x30) = 1;
    start = AIL_start_sequence;
    start(g_midi_sequence_guess);
    if (AIL_init_sequence(sequence, file_image, unknown0x10[i]) == 0)
    {
        EQTRACE_guess("PlayMidi::FadeIn AIL_INIT_SEQUENCE  FAILED!\n");
        return false;
    }
    *(u32*)((char*)sequence + 0x30) = unknown0x38[i];
    start(sequence);
    AIL_set_sequence_volume(sequence, unknown0x20[i], unknown0x2C[i]);
    AIL_register_prefix_callback(sequence, MidiPrefixCallback_guess);
    if (this == g_midi_music_guess)
        unknown0x1C = unknown0x10[i];
    state = 6;
    return true;
}

// FUNCTION: TRILOGY 0x004e465a
// Updates MIDI playback and fade state, starting pending music when a player is ready.
/* fake match: state = 1 sits between the two field stores; with it last, VC6 shares this return's tail with the fade-false one. */
void __fastcall UpdateState_guess(EqMidiSequence_guess* midi)
{
    typedef long (__stdcall *AIL_sequence_status_guess_t)(void*);
    typedef long (__stdcall *AIL_sequence_volume_guess_t)(void*);
    char fadeStarted;
    int result;

    result = midi->state;
    switch (result)
    {
    case 6:
        result = EQ_GLOBAL(AIL_sequence_status_guess_t, 0x0053434C)(midi->sequence);
        if ((result == 2) || (result == 8))
        {
            midi->state = 4;
            return;
        }
        break;
    case 4:
        result = EQ_GLOBAL(AIL_sequence_status_guess_t, 0x0053434C)(midi->sequence);
        if ((result == 2) || (result == 8))
        {
            midi->current = -1;
            midi->state = 1;
            midi->unknown0x44 = 0;
            return;
        }
        break;
    case 2:
        result = EQ_GLOBAL(AIL_sequence_volume_guess_t, 0x00534350)(midi->sequence);
        if (result == 0)
        {
            AIL_end_sequence(midi->sequence);
            midi->state = 4;
            return;
        }
        break;
    case 1:
        if (midi->unknown0x48 != 0)
        {
            if (midi != g_midi_player_b_guess)
            {
                if (midi == g_midi_music_guess &&
                    midi->unknown0x08 != midi->unknown0x0C)
                {
                    if (g_midi_player_a_guess == 0)
                        return;
                    if (g_midi_player_a_guess->state != 1)
                        return;
                }
                else if (midi != g_midi_player_a_guess)
                {
                    return;
                }
            }
            fadeStarted = EQ_METHOD(0, bool, 0x004E4570, (EqMidiSequence_guess *), midi)();
            if (fadeStarted == '\0')
            {
                midi->state = 1;
                return;
            }
            midi->unknown0x48 = 0;
        }
        break;
    }
}

// FUNCTION: TRILOGY 0x004e471d
// Queues a MIDI track with its volume, fade time, and loop settings.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Queues a track (0 .. count at 0x04 - 1) with volume, fade and loop count. The zone music
// advances its two-entry queue, and re-queuing the current track just restores its volume (state 6, false).
// A player that is not idle (state 1) is stopped first (with the music fade time for the zone music).
/* fake match: the extra `int i` keeps (i + 1) % 2 signed so VC6 emits the original's idiv form. */
bool EqMidiSequence_guess::Queue_guess(int track, long volume, long ms, u32 loops)
{
    if (track >= (u16)unknown0x04)
        return false;
    if (track < 0)
        return false;
    unknown0x10[unknown0x08] = track;
    unknown0x20[unknown0x08] = volume;
    unknown0x2C[unknown0x08] = ms;
    unknown0x38[unknown0x08] = loops;
    if (this == g_midi_music_guess)
    {
        if (track == current)
        {
            state = 6;
            AIL_set_sequence_volume(sequence, g_music_volume_guess, 500);
            return false;
        }
        current = track;
        int i = unknown0x08;    /* fake match: signed local so VC6 emits the original's idiv % 2 */
        unknown0x08 = (i + 1) % 2;
        if (unknown0x08 == unknown0x0C)
        {
            i = unknown0x0C;
            unknown0x0C = (i + 1) % 2;
        }
    }
    if (state != 1)
    {
        if (this == g_midi_music_guess)
            Stop_guess(g_music_fade_ms_guess);
        else
            Stop_guess(0);
    }
    unknown0x48 = 1;
    return true;
}

// FUNCTION: TRILOGY 0x004e47ce
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall.
s32 EqMidiSequence_guess::GetState_guess(void)
{
    return state;
}

// FUNCTION: TRILOGY 0x004e47d2
// Loads the SoundFont manager SFMAN32.DLL and opens the best-rated SoundFont device; software synths rate low.
// @early-stop: prolog STOSD vs MOV, push EDI vs ESI, MOVZX vs MOV for best
/* fake match: temp+=0xFFFF gives ADD and combined high/low gives reused EAX high */
int __cdecl InitSFDLL(u32 value)
{
    typedef void *(__stdcall *tLoadLibraryA_guess)(const char *);
    typedef void *(__stdcall *tGetProcAddress_guess)(void *, const char *);
    typedef int (__stdcall *tFreeLibrary_guess)(void *);
    typedef int (__cdecl *tSFManager_guess)(u32, void ***);
    typedef void (__cdecl *tGetDeviceCount_guess)(u32 *);
    typedef void (__cdecl *tGetDeviceInfo_guess)(u32, void *);
    typedef int (__cdecl *tDeviceOpen_guess)(u32);
    typedef void (__cdecl *tDeviceClose_guess)(u32);
    typedef void (__cdecl *tGetDeviceConfig_guess)(u32, void *, u32 *);

    char message[60];
    u16 deviceInfo[0x33];
    u32 configValue;
    u32 configExtra;
    unsigned char ratings[16];
    void *module;
    void *proc;
    int temp;
    int openResult;
    u16 deviceIndex;
    unsigned char rating;

    ratings[0] = 0;
    ratings[1] = 0;
    ratings[2] = 0;
    ratings[3] = 0;
    ratings[4] = 0;
    ratings[5] = 0;
    ratings[6] = 0;
    ratings[7] = 0;
    configExtra = 0;
    ratings[8] = 0;
    ratings[9] = 0;
    ratings[10] = 0;
    ratings[11] = 0;
    ratings[12] = 0;
    ratings[13] = 0;
    ratings[14] = 0;
    ratings[15] = 0;
    if (value != 1)
        return 1;

    module = EQ_GLOBAL(tLoadLibraryA_guess, 0x00534048)(EQ_ARRAY(char, 0x0057C384));
    EQ_GLOBAL(void *, 0x006ECDB0) = module;
    if (module == 0)
    {
        EQ_GLOBAL(tFreeLibrary_guess, 0x0053403C)(0);
        EQTRACE_guess(EQ_ARRAY(char, 0x0057C358));
        return 0;
    }
    proc = EQ_GLOBAL(tGetProcAddress_guess, 0x00534044)(module, EQ_ARRAY(char, 0x0057C34C));
    if (proc == 0)
    {
        EQ_GLOBAL(tFreeLibrary_guess, 0x0053403C)(EQ_GLOBAL(void *, 0x006ECDB0));
        EQTRACE_guess(EQ_ARRAY(char, 0x0057C31C));
        return 0;
    }
    if ((*(tSFManager_guess *)proc)(0x10001, &EQ_GLOBAL(void **, 0x006ECDB4)) != 0)
    {
        EQ_GLOBAL(tFreeLibrary_guess, 0x0053403C)(EQ_GLOBAL(void *, 0x006ECDB0));
        EQTRACE_guess(EQ_ARRAY(char, 0x0057C2F4));
        return 0;
    }
    ((tGetDeviceCount_guess)EQ_GLOBAL(void **, 0x006ECDB4)[0])(&EQ_GLOBAL(u32, 0x006ECDBC));
    if ((s16)EQ_GLOBAL(u32, 0x006ECDBC) == 0)
    {
        EQ_GLOBAL(tFreeLibrary_guess, 0x0053403C)(EQ_GLOBAL(void *, 0x006ECDB0));
        EQTRACE_guess(EQ_ARRAY(char, 0x0057C2BC));
        return 0;
    }
    temp = EQ_GLOBAL(u32, 0x006ECDBC);
    deviceIndex = 0;
                    if ((s16)temp != 0)
                    {
                        do
                        {
                            memset(deviceInfo, 0, 0x66);
                            deviceInfo[0] = 0x66;
                            ((tGetDeviceInfo_guess)EQ_GLOBAL(void **, 0x006ECDB4)[1])(deviceIndex, deviceInfo);
                            sprintf(message, EQ_ARRAY(char, 0x0057C2A4), (char *)deviceInfo + 0x16);
                            EQTRACE_guess(EQ_ARRAY(char, 0x0057C29C), message);
                            if ((((u8 *)deviceInfo)[9] & 0x40) == 0)
                            {
                                EQTRACE_guess(EQ_ARRAY(char, 0x0057C26C),
                                     (char *)deviceInfo + 0x16);
                                if ((((u8 *)deviceInfo)[9] & 0x80) == 0)
                                {
                                    ((tDeviceOpen_guess)EQ_GLOBAL(void **, 0x006ECDB4)[4])(deviceIndex);
                                    ((tGetDeviceConfig_guess)EQ_GLOBAL(void **, 0x006ECDB4)[7])
                                        (deviceIndex, &configValue, &value);
                                    rating = (char)(value >> 0x13) + 0x40;
                                    ratings[deviceIndex] = rating;
                                    if (rating == 0x40)
                                        ratings[deviceIndex] = 0;
                                    ((tDeviceClose_guess)EQ_GLOBAL(void **, 0x006ECDB4)[5])(deviceIndex);
                                }
                                else
                                {
                                    ratings[deviceIndex] = 0x80;
                                }
                            }
                            else
                            {
                                EQTRACE_guess(EQ_ARRAY(char, 0x0057C240),
                                     (char *)deviceInfo + 0x16);
                                ratings[deviceIndex] = 0x20;
                            }
                            deviceIndex = deviceIndex + 1;
                        } while (deviceIndex < (u16)EQ_GLOBAL(u32, 0x006ECDBC));
                        temp = EQ_GLOBAL(u32, 0x006ECDBC);
                    }
                    while ((s16)temp != 0)
                    {
                        sprintf(message, EQ_ARRAY(char, 0x0057C218),
                             EQ_GLOBAL(u32, 0x006ECDB8), ratings[EQ_GLOBAL(u32, 0x006ECDB8)]);
                        EQTRACE_guess(message);
                        temp += 0xFFFF;
                        deviceIndex = 1;
                        if (1 < (u16)EQ_GLOBAL(u32, 0x006ECDBC))
                        {
                            do
                            {
                                if (ratings[deviceIndex] >= ratings[EQ_GLOBAL(u32, 0x006ECDB8)])
                                {
                                    EQ_GLOBAL(u32, 0x006ECDB8) = deviceIndex;
                                    EQTRACE_guess(EQ_ARRAY(char, 0x0057C1F0),
                                         deviceIndex, ratings[deviceIndex]);
                                }
                                deviceIndex = deviceIndex + 1;
                            } while (deviceIndex < (u16)EQ_GLOBAL(u32, 0x006ECDBC));
                        }
                        EQTRACE_guess(EQ_ARRAY(char, 0x0057C1C8),
                             EQ_GLOBAL(u32, 0x006ECDB8));
                        openResult = ((tDeviceOpen_guess)EQ_GLOBAL(void **, 0x006ECDB4)[4])
                            ((u16)EQ_GLOBAL(u32, 0x006ECDB8));
                        if (openResult == 0)
                            temp = 0;
                        else
                        {
                            EQTRACE_guess(EQ_ARRAY(char, 0x0057C188));
                            ratings[EQ_GLOBAL(u32, 0x006ECDB8)] = 0;
                        }
                    }
                    if (ratings[EQ_GLOBAL(u32, 0x006ECDB8)] == 0)
                    {
                        EQ_GLOBAL(tFreeLibrary_guess, 0x0053403C)(EQ_GLOBAL(void *, 0x006ECDB0));
                        return 0;
                    }
                    memset(deviceInfo, 0, 0x66);
                    deviceInfo[0] = 0x66;
                    ((tGetDeviceInfo_guess)EQ_GLOBAL(void **, 0x006ECDB4)[1])
                        ((((u32)deviceInfo & 0xffff0000UL) |
                          (u16)EQ_GLOBAL(u32, 0x006ECDB8)), deviceInfo);
                    if ((((u8 *)deviceInfo)[9] & 0x80) != 0)
                        return 1;
                    ((tGetDeviceConfig_guess)EQ_GLOBAL(void **, 0x006ECDB4)[7])
                        ((((u32)&configValue & 0xffff0000UL) |
                          (u16)EQ_GLOBAL(u32, 0x006ECDB8)), &configValue, &configExtra);
                    return 1;
}

// FUNCTION: TRILOGY 0x004e4aa5
// Resets MIDI music playback state and restores the configured music volume.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// Stops everything, then resets the stream's state bytes and the music volume. `this` in ECX
// (no header method for it: EqMidiStream_guess has no field for these offsets).
void __fastcall ResetMidiStream_guess(EqMidiStream_guess *self)
{
    long volume;

    self->StopAll_guess();
    volume = g_music_volume_guess;
    *(u8*)((char*)self + 0x41) = 10;
    *(long*)((char*)self + 0x8) = volume;
    *(u32*)((char*)self + 0x14) = 0;
    *(u32*)((char*)self + 0x18) = 0;
    *(u32*)((char*)self + 0x1c) = 0;
    *(u32*)((char*)self + 0x10) = 0;
    self->have = 0;
    self->wanted = 0;
    *(u32*)((char*)self + 0x38) = 0;
    *(u32*)((char*)self + 0x3c) = 0;
    *(u32*)((char*)self + 0x54) = 0;
    self->unk_0x58 = 0;
    EQ_GLOBAL(u8, 0x006ECDE7) = 0;
    *(u8*)((char*)self + 0x40) = 11;
}

// FUNCTION: TRILOGY 0x004e4ae4
// Sets up the MIDI music driver: an SB Live (SoundFont synthusr.sf2) or AWE synth, else the digital fallback.
int __fastcall IAMidi_guess(int self)
{
    struct tagMIDIOUTCAPSA_guess
    {
        u16 wMid;
        u16 wPid;
        u32 vDriverVersion;
        char szPname[32];
        u16 wTechnology;
        u16 wVoices;
        u16 wNotes;
        u16 wChannelMask;
        u32 dwSupport;
    };
    struct SFMAN_locals_guess
    {
        u32 local_24;
        char* local_20;
        u32 local_1c;
        u16 local_18;
        u8 unknown0x0e[6];
        u16 local_10;
        u16 local_e;
    };
    typedef void (__stdcall *AIL_startup_proc_guess)(void);
    typedef u32 (__stdcall *midiOutGetNumDevs_proc_guess)(void);
    typedef u32 (__stdcall *midiOutGetDevCapsA_proc_guess)(u32, void*, u32);
    typedef int (__stdcall *AIL_midiOutOpen_proc_guess)(u32*, u32, u32);
    typedef int (__stdcall *FreeLibrary_proc_guess)(u32);
    typedef u32 (__stdcall *LoadLibraryA_proc_guess)(const char*);
    typedef u32 (__stdcall *GetProcAddress_proc_guess)(u32, const char*);
    typedef u32 (__stdcall *AIL_register_event_callback_proc_guess)(u32, void*);
    typedef void (__stdcall *AIL_set_XMIDI_master_volume_proc_guess)(u32, u32);
    typedef int (__cdecl *SFMAN_init_proc_guess)(u32, void*);
    typedef int (__cdecl *SFMAN_load_bank_proc_guess)(u32, void*, void*);
    typedef void (__cdecl *SFMAN_close_proc_guess)(u32);
    typedef int (__stdcall *AWEManager_proc_guess)(u32, u32, u32, u32);

    u16 WVar1;
    u32 masterVolume;
    u32 UVar3;
    u16 traceHigh;
    u16 closeArgHigh;
    int clearResult;
    int BVar5;
    int tmp;
    tagMIDIOUTCAPSA_guess* src;
    u32 UVar9;
    short* dst;
    tagMIDIOUTCAPSA_guess caps;
    tagMIDIOUTCAPSA_guess bestCaps;
    SFMAN_locals_guess local_sf;
    u32 remaining;
    u32 sfman_arg_high;
    int bestRank;
    u32 deviceIndex;

    *(u8*)((char*)self + 0x46) = 0;
    EQ_GLOBAL(AIL_startup_proc_guess, 0x00534340)();
    masterVolume = EQ_GLOBAL(u32, 0x00579318);
    *(u8*)((char*)self + 0x44) = 0;
    *(u8*)((char*)self + 0x43) = 0;
    *(u8*)((char*)self + 0x58) = 0;
    *(u32*)((char*)self + 8) = masterVolume;
    *(u32*)((char*)self + 0x50) = 0;
    *(u32*)((char*)self + 0x14) = 0;
    *(u32*)((char*)self + 0x18) = 0;
    *(u32*)((char*)self + 0x1c) = 0;
    *(u8*)((char*)self + 0x45) = 1;
    *(u32*)((char*)self + 0x10) = 0;
    *(u8*)((char*)self + 0x41) = 10;
    *(u32*)((char*)self + 0x38) = 0;
    *(u32*)((char*)self + 0x3c) = 0;
    *(u32*)((char*)self + 0x60) = 0;
    *(u32*)((char*)self + 0x4c) = 0;
    *(u32*)((char*)self + 0x54) = 0;
    UVar9 = 0;
    EQ_GLOBAL(u32, 0x006ECE20) = 0;
    UVar3 = EQ_GLOBAL(midiOutGetNumDevs_proc_guess, 0x0053426C)();
    *(u32*)((char*)self + 0x5c) = 0;
    *(u32*)((char*)self + 0x48) = 0;
    bestCaps.wPid = 0;
    bestRank = 4;
    deviceIndex = 0;
    if (UVar3 != 0)
    {
        do
        {
            remaining = UVar3;
            EQ_GLOBAL(midiOutGetDevCapsA_proc_guess, 0x00534270)(deviceIndex, &caps, 0x34);
            if (((caps.wPid == 0x12f) && (caps.wMid == 2)) && (2 < bestRank))
            {
                src = &caps;
                dst = (short*)&bestCaps;
                for (tmp = 0xd; tmp != 0; tmp = tmp - 1)
                {
                    WVar1 = src->wPid;
                    dst[0] = src->wMid;
                    dst[1] = WVar1;
                    src = (tagMIDIOUTCAPSA_guess*)&src->vDriverVersion;
                    dst = dst + 2;
                }
                bestRank = 2;
                UVar9 = deviceIndex;
            }
            if ((caps.wPid == 0x132) && (caps.wMid == 2))
            {
                src = &caps;
                dst = (short*)&bestCaps;
                for (tmp = 0xd; tmp != 0; tmp = tmp - 1)
                {
                    WVar1 = src->wPid;
                    dst[0] = src->wMid;
                    dst[1] = WVar1;
                    src = (tagMIDIOUTCAPSA_guess*)&src->vDriverVersion;
                    dst = dst + 2;
                }
                bestRank = 1;
                UVar9 = deviceIndex;
            }
            deviceIndex = deviceIndex + 1;
            remaining = remaining - 1;
            UVar3 = remaining;
        } while (remaining != 0);
        if (((bestCaps.wPid == 0x12f) || (bestCaps.wPid == 0x132)) && (bestCaps.wMid == 2))
        {
            tmp = EQ_GLOBAL(AIL_midiOutOpen_proc_guess, 0x00534330)
                ((u32*)((char*)self + 0x60), 0, UVar9);
            if (tmp == 0)
            {
                EQ_GLOBAL(u32, 0x006ECE20) = 1;
                if (bestCaps.wPid == 0x132)
                {
                    tmp = EQ_CDECL(int, 0x004E47D2, (int))(1);
                    if (tmp == 0)
                    {
                        EQTRACE_guess((char*)0x0057c43c);
                    }
                    else
                    {
                        local_sf.local_10 = 1;
                        local_sf.local_e = 0;
                        sfman_arg_high = ((u32)&local_sf.local_10) & 0xffff0000;
                        (*(SFMAN_init_proc_guess*)((char*)EQ_GLOBAL(void*, 0x006ECDB4) + 0x38))(
                            sfman_arg_high |
                                (u16)EQ_GLOBAL(u32, 0x006ECDB8),
                            &local_sf.local_10);
                        memset(&local_sf.local_24, 0, 0xe);
                        local_sf.local_1c = 1;
                        local_sf.local_20 = (char*)0x0057c42c;
                        local_sf.local_24 = 0x80;
                        local_sf.local_18 = 0;
                        tmp = (*(SFMAN_load_bank_proc_guess*)((char*)EQ_GLOBAL(void*, 0x006ECDB4) + 0x2c))(
                            sfman_arg_high |
                                (u16)EQ_GLOBAL(u32, 0x006ECDB8),
                            &local_sf.local_10,
                            &local_sf.local_24);
                        if (tmp == 0)
                        {
                            EQ_GLOBAL(u32, 0x006ECE28) = 0x132;
                            closeArgHigh = 0;
                        }
                        else
                        {
                            traceHigh = (u16)(EQTRACE_guess((char*)0x0057c404) >> 16);
                            closeArgHigh = traceHigh;
                        }
                        (*(SFMAN_close_proc_guess*)((char*)EQ_GLOBAL(void*, 0x006ECDB4) + 0x14))(
                            ((u32)closeArgHigh << 16) | (u16)EQ_GLOBAL(u32, 0x006ECDB8));
                        EQ_GLOBAL(FreeLibrary_proc_guess, 0x0053403C)(EQ_GLOBAL(u32, 0x006ECDB0));
                        tmp = *(int*)((char*)self + 0x60);
                        if (tmp != 0)
                            goto haveDriver;
                    }
                }
                else
                {
                    EQ_GLOBAL(u32, 0x006ECDA8) =
                        EQ_GLOBAL(LoadLibraryA_proc_guess, 0x00534048)((char*)0x0057c3f4);
                    if (EQ_GLOBAL(u32, 0x006ECDA8) != 0 &&
                        (EQ_GLOBAL(u32, 0x006ECDA4) = EQ_GLOBAL(GetProcAddress_proc_guess, 0x00534044)
                            (EQ_GLOBAL(u32, 0x006ECDA8), (char*)0x0057c3e8),
                         EQ_GLOBAL(u32, 0x006ECDA4) != 0) &&
                        (tmp = ((AWEManager_proc_guess)EQ_GLOBAL(u32, 0x006ECDA4))
                            (0, 0x65, (u32)&EQ_GLOBAL(u32, 0x006ECDAC), 0), tmp == 0))
                    {
                        tmp = 0;
                        do
                        {
                            clearResult = ((AWEManager_proc_guess)EQ_GLOBAL(u32, 0x006ECDA4))
                                (EQ_GLOBAL(u32, 0x006ECDAC), 0xcd, tmp, 0);
                            if (clearResult != 0)
                            {
                                EQTRACE_guess((char*)0x0057c3c4, tmp);
                            }
                            tmp = tmp + 1;
                        } while (tmp < 4);
                        tmp = ((AWEManager_proc_guess)EQ_GLOBAL(u32, 0x006ECDA4))
                            (EQ_GLOBAL(u32, 0x006ECDAC), 0xcb, 3, 0);
                        if (tmp == 0)
                        {
                            if (bestCaps.wPid == 0x12f)
                                EQ_GLOBAL(u32, 0x006ECE28) = 0x12f;
                            if (EQ_GLOBAL(u32, 0x006ECE28) != 0x12f &&
                                (BVar5 = EQ_GLOBAL(FreeLibrary_proc_guess, 0x0053403C)
                                    (EQ_GLOBAL(u32, 0x006ECDA8)), BVar5 == 0))
                            {
                                EQTRACE_guess((char*)0x0057c390);
                            }
                            tmp = *(int*)((char*)self + 0x60);
                            if (tmp != 0)
                                goto haveDriver;
                        }
                    }
                }
            }
            else
            {
                *(u32*)((char*)self + 0x60) = 0;
            }
        }
    }
    EQ_GLOBAL(u32, 0x006ECE20) = 3;
    EQ_GLOBAL(u32, 0x006ECCFC) = EQ_CDECL(int, 0x004E4099, (void))();
    tmp = EQ_CDECL(int, 0x004E41FE, (int))(EQ_GLOBAL(u32, 0x006ECCFC));
    *(int*)((char*)self + 0x60) = tmp;
    if (tmp == 0)
        return self;
haveDriver:
    if (EQ_GLOBAL(u32, 0x006ECDD4) == 0)
        EQ_GLOBAL(u32, 0x006ECDD4) = EQ_GLOBAL(AIL_register_event_callback_proc_guess, 0x00534344)
            (*(u32*)((char*)self + 0x60), (void*)0x004e42d3);
    EQ_GLOBAL(AIL_set_XMIDI_master_volume_proc_guess, 0x00534348)
        (*(u32*)((char*)self + 0x60), *(u32*)((char*)self + 8));
    return self;
}

// FUNCTION: TRILOGY 0x004e4de4
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Ends this stream's sequence and stops the three MIDI players at once.
void EqMidiStream_guess::StopAll_guess(void)
{
    if (sequence != 0)
        AIL_end_sequence(sequence);
    if (g_midi_player_a_guess != 0)
        g_midi_player_a_guess->Stop_guess(0);
    if (g_midi_player_b_guess != 0)
        g_midi_player_b_guess->Stop_guess(0);
    if (g_midi_music_guess != 0)
        g_midi_music_guess->Stop_guess(0);
}

// FUNCTION: TRILOGY 0x004e4e26
// Releases MIDI sequences, soundbank data, and supporting driver resources.
void __fastcall ReleaseMidiResources_guess(int midiContext)
{
    int midiResource;
    typedef void (__stdcall *AIL_sequence_fn)(void *);
    typedef void (__stdcall *AIL_mem_free_lock_fn)(void *);
    typedef void (__stdcall *AIL_DLS_close_fn)(void *, int);
    typedef int (__stdcall *FreeLibrary_fn)(void *);

    if (*(int *)(midiContext + 0x4c) != 0)
    {
        EQ_GLOBAL(AIL_sequence_fn, 0x005342F8)((void *)*(int *)(midiContext + 0x4c));
        EQ_GLOBAL(AIL_sequence_fn, 0x00534308)((void *)*(int *)(midiContext + 0x4c));
    }
    if (*(int *)(midiContext + 0x50) != 0)
    {
        EQ_GLOBAL(AIL_mem_free_lock_fn, 0x00534338)((void *)*(int *)(midiContext + 0x50));
    }
    if (EQ_GLOBAL(int, 0x006ECDF8) != 0)
    {
        midiResource = EQ_GLOBAL(int, 0x006ECDF8);
        if (midiResource != 0)
        {
            EQ_METHOD(0, void, 0x004E44D3, (void *), (void *)midiResource)();
            operator delete((void *)midiResource);
        }
        EQ_GLOBAL(int, 0x006ECDF8) = 0;
    }
    if (EQ_GLOBAL(int, 0x006ECDFC) != 0)
    {
        midiResource = EQ_GLOBAL(int, 0x006ECDFC);
        if (midiResource != 0)
        {
            EQ_METHOD(0, void, 0x004E44D3, (void *), (void *)midiResource)();
            operator delete((void *)midiResource);
        }
        EQ_GLOBAL(int, 0x006ECDFC) = 0;
    }
    if (EQ_GLOBAL(int, 0x006ECE00) != 0)
    {
        midiResource = EQ_GLOBAL(int, 0x006ECE00);
        if (midiResource != 0)
        {
            EQ_METHOD(0, void, 0x004E44D3, (void *), (void *)midiResource)();
            operator delete((void *)midiResource);
        }
        EQ_GLOBAL(int, 0x006ECE00) = 0;
    }
    if (*(int *)(midiContext + 0x5c) != 0)
    {
        EQ_GLOBAL(AIL_DLS_close_fn, 0x005342FC)((void *)*(int *)(midiContext + 0x5c), 2);
    }
    if (EQ_GLOBAL(int, 0x006ECE28) == 0x12f)
    {
        EQ_GLOBAL(FreeLibrary_fn, 0x0053403C)((void *)EQ_GLOBAL(int, 0x006ECDA8));
    }
}

// FUNCTION: TRILOGY 0x004e4ee6
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (Mac: EqSoundManager::SetMusicSelection, medium)
// __thiscall. Stores the selection at +0x54.
void EqSoundManager_guess::SetMusicSelection(int selection)
{
    *(int*)((char*)this + 0x54) = selection;
}

// FUNCTION: TRILOGY 0x004e4ef0
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall. Sets the volume of the sequence at 0x4C when there is one, and marks byte 0x46.
void EqMidiStream_guess::SetVolumeIfLoaded_guess(long volume, long ms)
{
    if (sequence != 0)
    {
        AIL_set_sequence_volume(sequence, 0, ms);
        fading = 1;
    }
}

// FUNCTION: TRILOGY 0x004e4f0f
// Plays a selected track from the loaded MIDI file at the requested volume and fade.
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall (EBP frame in the original). Starts sequence `sequence_num` (0..63) of the loaded file at the
// given volume and registers the prefix callback; false if nothing is loaded or the init fails.
bool EqMidiStream_guess::Play_guess(long sequence_num, long volume, long ms)
{
    if ((file_image != 0 && 0x40 > sequence_num && sequence_num >= 0
        && AIL_init_sequence(sequence, file_image, sequence_num) != 0) == 0)
    {
    }
    else
    {
        register void *temp = sequence;
        *(u32*)((char*)temp + 0x30) = 1;
        AIL_start_sequence(sequence);
        AIL_set_sequence_volume(sequence, volume, ms);
        AIL_register_prefix_callback(sequence, MidiPrefixCallback_guess);
        return true;
    }
    return false;
}

typedef long (__stdcall *AIL_sequence_long_fn)(void*);
typedef void (__stdcall *AIL_branch_index_fn)(void*, long);

// FUNCTION: TRILOGY 0x004e5018
// Ends a music sequence after its fade reaches silence or stops progressing.
void __fastcall FadeOutAndEndSequence_guess(EqMidiStream_guess* stream)
{
    int value;
    int current;
    int stallCount;

    if ((char)stream->unknown0x45 < 1)
    {
        stream->unknown0x45 = 0x40;
    }
    else if (stream->sequence != 0)
    {
        value = EQ_GLOBAL(AIL_sequence_long_fn, 0x0053434C)(stream->sequence);
        if ((value == 2) || (value == 8))
        {
            AIL_end_sequence(stream->sequence);
            stream->unknown0x45 = 0x40;
            if ((EQ_GLOBAL(int, 0x00579318) == 0) && (EQ_GLOBAL(int, 0x006ECE28) == 0))
            {
                EQ_METHOD(2, void, 0x004E5EE6, (EqMidiStream_guess *, int, int), stream)(0, 0);
            }
        }
        else
        {
            stream->unknown0x45 = 0;
            EQ_METHOD(2, void, 0x004E4EF0, (EqMidiStream_guess *, long, long), stream)(0, 4000);
            stallCount = 0;
            stream->fading = 1;
            value = -1;
        fade_loop:
            current = EQ_GLOBAL(AIL_sequence_long_fn, 0x00534350)(stream->sequence);
            if (value == current)
            {
                stallCount = stallCount + 1;
            }
            else
            {
                value = current;
                stallCount = 0;
            }
            if (!((current == 0) || (100 < stallCount)))
            {
                if ((char)stream->fading)
                    goto fade_loop;
            }
            else
                {
                AIL_end_sequence(stream->sequence);
                stream->fading = 0;
                return;
            }
        }
    }
    return;
}

// The music state object whose update UpdateMusic_guess is (the object at 0x006ECDF4 the callers pass in ECX);
// offsets from that function. File-local stand-in: no header names it yet.
class EqMusicState_guess
{
public:
    u8      unknown0x00[0xC];   /* 0x00 */
    s32     unknown0x0C;        /* 0x0C* */
    s32     counter;            /* 0x10* */
    EQSoundEmitter_guess* cursor;   /* 0x14* */
    EQSoundEmitter_guess* nearest;  /* 0x18* */
    EQSoundEmitter_guess* current;  /* 0x1C* */
    f32     distance;           /* 0x20* */
    u8      unknown0x24[4];     /* 0x24 */
    f32     best_distance;      /* 0x28* */
    u8      unknown0x2C[4];     /* 0x2C */
    f32     radius;             /* 0x30* */
    s32     last_state;         /* 0x34* */
    s32     track;              /* 0x38* */
    s32     track_queued;       /* 0x3C* */
    s8      priority;           /* 0x40* */
    u8      unknown0x41[2];     /* 0x41 */
    u8      wanted;             /* 0x43* */
    u8      have;               /* 0x44* */
    u8      unknown0x45;        /* 0x45* */
    u8      fading;             /* 0x46* */
    u8      unknown0x47[5];     /* 0x47 */
    void*   sequence;           /* 0x4C* */
    u8      unknown0x50[4];     /* 0x50 */
    s32     command;            /* 0x54* */
    u8      unknown0x58;        /* 0x58* */
};
C_ASSERT(offsetof(EqMusicState_guess, sequence) == 0x4C);
C_ASSERT(offsetof(EqMusicState_guess, unknown0x58) == 0x58);

#define MIDI_STOP_(player, ms)      EQ_METHOD(1, void, 0x004E4536, (void*, long), (player))((ms))
#define MIDI_QUEUE_(player, track, volume, ms, loops) \
    EQ_METHOD(4, void, 0x004E471D, (void*, int, long, long, u32), (player))((track), (volume), (ms), (loops))
#define MIDI_STATE_(player)         EQ_METHOD(0, int, 0x004E47CE, (void*), (player))()
#define MIDI_UPDATE_(player)        EQ_METHOD(0, void, 0x004E465A, (void*), (player))()
#define ACTOR_STATE_                (g_local_player_guess->actor_info->animationId_guess)

// FUNCTION: TRILOGY 0x004e50c1
// Per-frame music driver: player-state tracks, the nearest emitter's day/night zone music, music commands
// @early-stop: structural match 0.9564; stack-call EDX setup and some x87/control-flow details still differ.
/* fake match: preserve the original shared reset/stop tails and per-emitter volatile music-kind read. */
void __fastcall UpdateMusic_guess(EqMusicState_guess* musicState)
{
#undef MIDI_STATE_
#define MIDI_STATE_(player) EQ_METHOD(0, int, 0x004E47CE, (void*), (player))()
#undef MIDI_UPDATE_
#define MIDI_UPDATE_(player) EQ_METHOD(0, void, 0x004E465A, (void*), (player))()

    EQPlayer* pEVar2;
    int lastState;
    short rideRace;
    float emitterDistance;
    u8 hour;

    if (musicState->unknown0x45 != 0x40)
    {
        EQ_METHOD(0, void, 0x004E5018, (void*), musicState)();
    }
    if (g_music_volume_guess == 0)
    {
        return;
    }
    if (g_midi_player_b_guess != 0)
    {
        MIDI_UPDATE_(g_midi_player_b_guess);
    }
    if (g_midi_player_a_guess != 0)
    {
        MIDI_UPDATE_(g_midi_player_a_guess);
    }
    if (EQ_GLOBAL(u8, 0x006B710C) != 0)
    {
        if (EQ_GLOBAL(u8, 0x006ECDE4) != 0)
        {
            return;
        }
        if (g_midi_music_guess == 0)
        {
            return;
        }
        MIDI_STOP_(g_midi_music_guess, 0);
        EQ_GLOBAL(int, 0x006ECDD8) = 0;
        EQ_GLOBAL(int, 0x006ECDE0) = 0;
        musicState->priority = 1;
        musicState->wanted = 2;
        EQ_GLOBAL(int, 0x006ECE10) = 0;
        musicState->track = EQ_CDECL(int, 0x004A46F6, (int, int))(0, 2) + 1;
        EQ_GLOBAL(int, 0x006ECDDC) = 1;
        EQ_GLOBAL(u8, 0x006ECDE4) = 1;
        EQ_GLOBAL(u8, 0x006ECDE7) = 0;
        *(int*)((char*)g_midi_music_guess + 0x4C) = -1;
        musicState->current = 0;
        EQ_GLOBAL(u8, 0x006ECDE8) = 0;
        musicState->have = musicState->wanted;
        MIDI_QUEUE_(g_midi_player_a_guess, musicState->track - 1, g_music_volume_guess,
                    EQ_GLOBAL(int, 0x006ECDDC), EQ_GLOBAL(int, 0x006ECE10));
        return;
    }
    if (EQ_GLOBAL(u8, 0x006ECDE4) != 0)
    {
        EQ_GLOBAL(int, 0x006ECDD8) = 0;
        EQ_GLOBAL(int, 0x006ECDE0) = 0;
        musicState->wanted = 0;
        musicState->track = 0;
        EQ_GLOBAL(int, 0x006ECDDC) = 0;
        EQ_GLOBAL(u8, 0x006ECDE4) = 0;
        musicState->priority = 10;
        MIDI_STOP_(g_midi_player_a_guess, 0);
        return;
    }
    if (g_midi_music_guess != 0)
    {
        MIDI_UPDATE_(g_midi_music_guess);
        pEVar2 = g_local_player_guess->actor_info->ride_target_guess;
        if (pEVar2 == 0)
        {
        lab_526b:
            if (EQ_GLOBAL(u8, 0x006ECDE7) != 0)
            {
                musicState->wanted = 0;
                musicState->track = 0;
                EQ_GLOBAL(int, 0x006ECDDC) = 1000;
                EQ_GLOBAL(int, 0x006ECE10) = 0;
                EQ_GLOBAL(u8, 0x006ECDE7) = 0;
                musicState->priority = 10;
            }
        }
        else
        {
            rideRace = *(short*)((char*)pEVar2 + 0x9A);
            if ((rideRace == 0x48) && (EQ_GLOBAL(u8, 0x006ECDE7) == 0))
            {
                if (musicState->priority > 1)
                {
                    musicState->priority = 3;
                    musicState->wanted = 0xC;
                    musicState->track = 0x15;
                    EQ_GLOBAL(int, 0x006ECDDC) = 1;
                    EQ_GLOBAL(int, 0x006ECE10) = 2;
                    EQ_GLOBAL(u8, 0x006ECDE7) = 1;
                    EQ_GLOBAL(u8, 0x006ECDE8) = 0;
                }
            }
            else if (rideRace != 0x48)
            {
                /* fake match: preserve the original shared ride reset block. */
                goto lab_526b;
            }
        }
        if (ACTOR_STATE_ != musicState->last_state)
        {
            musicState->unknown0x58 = 0;
            switch (ACTOR_STATE_)
            {
            case 0x19:
                if ((g_local_player_guess->cur_hp > 9) && (musicState->priority > 1))
                {
                    musicState->priority = 3;
                    musicState->wanted = 7;
                    musicState->track = 9;
                    EQ_GLOBAL(int, 0x006ECDDC) = 1;
                    EQ_GLOBAL(int, 0x006ECE10) = 0;
                    EQ_GLOBAL(u8, 0x006ECDE5) = 1;
                }
                break;
            case 0x16:
                if ((musicState->priority > 4) && (EQ_GLOBAL(u8, 0x006ECDE8) == 0))
                {
                    musicState->priority = 2;
                    musicState->wanted = 6;
                    musicState->track = 8;
                    EQ_GLOBAL(int, 0x006ECDDC) = 1;
                    EQ_GLOBAL(int, 0x006ECE10) = 0;
                    EQ_GLOBAL(u8, 0x006ECDE8) = 1;
                }
                break;
            case 0x12:
                EQ_GLOBAL(int, 0x006ECDD8) = 0;
                break;
            case 0x10:
                musicState->priority = 1;
                musicState->wanted = 3;
                musicState->track = 4;
                EQ_GLOBAL(int, 0x006ECE10) = 1;
                EQ_GLOBAL(int, 0x006ECDDC) = 1;
                if (MIDI_STATE_(g_midi_music_guess) == 6)
                {
                    MIDI_STOP_(g_midi_music_guess, 0);
                    musicState->current = 0;
                }
                break;
            case 0:
                if ((EQ_GLOBAL(u8, 0x006ECDE5) == 0) && (MIDI_STATE_(g_midi_music_guess) == 6)
                    && (EQ_GLOBAL(u8, 0x006ECDE4) == 0))
                {
                    EQ_GLOBAL(int, 0x006ECDD8) = EQ_GLOBAL(int, 0x006ECDD8) + 1;
                }
                break;
            case 0x27:
                musicState->priority = 1;
                musicState->wanted = 8;
                musicState->track = 10;
                EQ_GLOBAL(int, 0x006ECDDC) = 1;
                EQ_GLOBAL(int, 0x006ECE10) = 1;
                EQ_GLOBAL(u8, 0x006ECDF0) = 1;
                break;
            case 0x28:
                musicState->priority = 1;
                musicState->wanted = 9;
                musicState->track = 0xC;
                EQ_GLOBAL(int, 0x006ECDDC) = 1;
                EQ_GLOBAL(int, 0x006ECE10) = 1;
                EQ_GLOBAL(u8, 0x006ECDF0) = 1;
                break;
            case 0x29:
                musicState->priority = 1;
                musicState->wanted = 10;
                musicState->track = 0xE;
                EQ_GLOBAL(int, 0x006ECDDC) = 1;
                EQ_GLOBAL(int, 0x006ECE10) = 1;
                EQ_GLOBAL(u8, 0x006ECDF0) = 1;
                break;
            case 0x25:
                if ((MIDI_STATE_(g_midi_music_guess) != 6) && (g_local_player_guess->cur_hp > 9)
                    && (musicState->priority > 1))
                {
                    musicState->priority = 4;
                    musicState->wanted = 7;
                    musicState->track = 9;
                    EQ_GLOBAL(int, 0x006ECDDC) = 1;
                    EQ_GLOBAL(int, 0x006ECE10) = 0;
                    EQ_GLOBAL(u8, 0x006ECDE5) = 1;
                }
                break;
            case 0x1A:
            case 0x20:
                if ((EQ_GLOBAL(u8, 0x006ECDE5) == 0) && (MIDI_STATE_(g_midi_music_guess) != 6)
                    && (EQ_GLOBAL(u8, 0x006ECDE4) == 0))
                {
                    EQ_GLOBAL(int, 0x006ECDD8) = 1;
                }
                if ((EQ_GLOBAL(u8, 0x006ECDE8) != 0) && (EQ_GLOBAL(int, 0x006ECDD8) != 0))
                {
                    musicState->wanted = 0;
                    musicState->track = 0;
                    EQ_GLOBAL(int, 0x006ECDDC) = 1000;
                    EQ_GLOBAL(int, 0x006ECE10) = 0;
                    EQ_GLOBAL(u8, 0x006ECDE8) = 0;
                    musicState->priority = 10;
                }
                break;
            }
            musicState->last_state = ACTOR_STATE_;
        }
        lastState = musicState->last_state;
        if ((lastState != 0x25) && (lastState != 0x19) && (EQ_GLOBAL(u8, 0x006ECDE5) == 1))
        {
            EQ_GLOBAL(u8, 0x006ECDE5) = (lastState == 0xF) + 2;
        }
        musicState->counter = musicState->counter + 1;
        if ((musicState->counter >= 0) && (EQ_GLOBAL(u8, 0x006ECE04) != 0))
        {
            musicState->counter = 0;
            if (g_midi_music_guess != 0)
            {
                if ((MIDI_STATE_(g_midi_music_guess) == 6) && (musicState->current != 0))
                {
                    emitterDistance = EQ_METHOD(0, float, 0x004BD43F, (void*), musicState->current)();
                    musicState->distance = emitterDistance;
                    musicState->radius = musicState->current->radius;
                    if (emitterDistance == EQ_GLOBAL(float, 0x0053510C))
                    {
                        MIDI_STOP_(g_midi_music_guess, musicState->current->unknown0x44[0]);
                        musicState->current = 0;
                    }
                    else if (EQ_GLOBAL(u8, 0x006ECE18) == 1)
                    {
                        EQ_GLOBAL(u8, 0x006ECDF1) =
                            (u8)(int)(emitterDistance / musicState->radius * EQ_GLOBAL(float, 0x005353BC));
                        if (EQ_GLOBAL(u8, 0x006ECDF1) != EQ_GLOBAL(u8, 0x006ECDF2))
                        {
                            EQ_GLOBAL(u8, 0x006ECDF2) = EQ_GLOBAL(u8, 0x006ECDF1);
                            EQ_GLOBAL(u8, 0x006ECE19) = (6 - EQ_GLOBAL(u8, 0x006ECDF1)) * 2;
                        }
                    }
                }
                musicState->best_distance = 5000.0f;
                musicState->cursor = g_sound_emitter_head_guess;
                musicState->nearest = 0;
                if (musicState->cursor != 0)
                {
                    do
                    {
                        if (musicState->cursor->unknown0x38 == 1)
                        {
                            *EQ_GLOBAL(volatile u8*, 0x006E4D54);
                            emitterDistance = EQ_METHOD(0, float, 0x004BD43F, (void*), musicState->cursor)();
                            musicState->distance = emitterDistance;
                            if ((emitterDistance != EQ_GLOBAL(float, 0x0053510C)) && (emitterDistance < musicState->best_distance))
                            {
                                musicState->best_distance = emitterDistance;
                                musicState->nearest = musicState->cursor;
                            }
                        }
                    else
                    {
                        *EQ_GLOBAL(volatile u8*, 0x006E4D54);
                    }
                        musicState->cursor = musicState->cursor->prev_guess;
                    }
                    while (musicState->cursor != 0);
                }
                if (musicState->nearest != musicState->current)
                {
                    if (MIDI_STATE_(g_midi_player_a_guess) == 6)
                    {
                        MIDI_STOP_(g_midi_player_a_guess, EQ_GLOBAL(int, 0x006ECDDC));
                    }
                    hour = *EQ_GLOBAL(u8*, 0x006E4D54);
                    if ((hour > 4) && (hour < 0x13))
                    {
                        if (musicState->nearest == 0)
                        {
                            /* fake match: preserve the original shared emitter stop tail. */
                            goto lab_568c;
                        }
                        EQ_GLOBAL(u8, 0x006ECE18) = (u8)musicState->nearest->unknown0x20;
                        MIDI_QUEUE_(g_midi_music_guess, musicState->nearest->sound_id_guess[0], g_music_volume_guess, 0,
                                    musicState->nearest->unknown0x3C);
                    }
                    else
                    {
                        if (musicState->nearest == 0)
                        {
                        lab_568c:
                            MIDI_STOP_(g_midi_music_guess, g_music_fade_ms_guess);
                            musicState->current = 0;
                            /* fake match: join the original current/nearest update tail. */
                            goto lab_56a0;
                        }
                        EQ_GLOBAL(u8, 0x006ECE18) = (u8)musicState->nearest->unknown0x20;
                        MIDI_QUEUE_(g_midi_music_guess, musicState->nearest->sound_id_guess[1], g_music_volume_guess, 0,
                                    musicState->nearest->unknown0x3C);
                    }
                lab_56a0:
                    musicState->unknown0x0C = 0;
                    musicState->current = musicState->nearest;
                }
            }
        }
        if (EQ_GLOBAL(u8, 0x006ECDE5) == 2)
        {
            EQ_GLOBAL(int, 0x006ECDD8) = 0;
            EQ_GLOBAL(int, 0x006ECDE0) = 0;
            musicState->wanted = 0;
            musicState->track = 0;
            EQ_GLOBAL(u8, 0x006ECDE5) = 0;
            EQ_GLOBAL(int, 0x006ECDDC) = g_music_fade_ms_guess;
            musicState->priority = 10;
        }
        if (g_local_player_guess->cur_hp < -8)
        {
            musicState->priority = 1;
            musicState->wanted = 7;
            musicState->track = 4;
            EQ_GLOBAL(int, 0x006ECDDC) = 1;
            EQ_GLOBAL(int, 0x006ECE10) = 1;
        }
    }
    if (musicState->command != 0)
    {
        switch (musicState->command)
        {
        case 0x12:
            if (g_midi_player_a_guess != 0)
            {
                MIDI_STOP_(g_midi_player_a_guess, 0);
            }
            if (g_midi_music_guess != 0)
            {
                MIDI_STOP_(g_midi_music_guess, 0);
            }
            EQ_GLOBAL(int, 0x006ECDD8) = 0;
            EQ_GLOBAL(int, 0x006ECDE0) = 0;
            musicState->priority = 1;
            musicState->wanted = 0x10;
            musicState->track = 0x16;
            EQ_GLOBAL(int, 0x006ECDDC) = g_music_fade_ms_guess;
            EQ_GLOBAL(u8, 0x006ECDE6) = 1;
            EQ_GLOBAL(u8, 0x006ECDE4) = 0;
            EQ_GLOBAL(u8, 0x006ECDE8) = 0;
            break;
        case 0x14:
            if (g_midi_player_a_guess != 0)
            {
                MIDI_STOP_(g_midi_player_a_guess, 0);
            }
            if (g_midi_music_guess != 0)
            {
                MIDI_STOP_(g_midi_music_guess, 0);
            }
            EQ_GLOBAL(int, 0x006ECDD8) = 0;
            EQ_GLOBAL(int, 0x006ECDE0) = 0;
            musicState->priority = 1;
            musicState->wanted = 0x11;
            musicState->track = 0x18;
            EQ_GLOBAL(int, 0x006ECDDC) = g_music_fade_ms_guess;
            EQ_GLOBAL(u8, 0x006ECDE6) = 1;
            EQ_GLOBAL(u8, 0x006ECDE8) = 0;
            EQ_GLOBAL(u8, 0x006ECDE4) = 0;
            break;
        case 0x18:
            EQ_GLOBAL(u8, 0x006ECDE6) = 0;
            musicState->wanted = 0;
            musicState->track = 0;
            EQ_GLOBAL(int, 0x006ECDDC) = g_music_fade_ms_guess;
            musicState->priority = 10;
            break;
        case 0x15:
            EQ_GLOBAL(u8, 0x006ECDE6) = 0;
            musicState->wanted = 0;
            musicState->track = 0;
            EQ_GLOBAL(int, 0x006ECDDC) = g_music_fade_ms_guess;
            musicState->priority = 10;
            EQ_GLOBAL(u8, 0x006ECDE8) = 0;
            break;
        case 0x17:
            EQ_GLOBAL(u8, 0x006ECDE6) = 1;
            musicState->priority = 1;
            musicState->wanted = 0x12;
            musicState->track = 0x14;
            EQ_GLOBAL(int, 0x006ECDDC) = 0;
            break;
        case 1:
            if ((musicState->unknown0x45 == 1) && (musicState->sequence != 0))
            {
                musicState->unknown0x45 = 2;
                EQ_METHOD(2, void, 0x004E4EF0, (void*, long, long), musicState)(0, 500);
                musicState->fading = 1;
            }
            if (musicState->unknown0x45 != 0x40)
            {
                while (musicState->fading != 0)
                {
                    if (EQ_GLOBAL(AIL_sequence_long_fn, 0x00534350)(musicState->sequence) == 0)
                    {
                        AIL_end_sequence(musicState->sequence);
                        musicState->fading = 0;
                    }
                }
                if (EQ_GLOBAL(int, 0x006ECDD4) == 0)
                {
                    EQTRACE_guess(EQ_ARRAY(char, 0x0057C470));
                }
            }
            EQ_GLOBAL(u8, 0x006ECE18) = 2;
            if (musicState->sequence != 0)
            {
                EQ_METHOD(3, int, 0x004E4F0F, (void*, long, long, long), musicState)(1, 0x7F, 0);
            }
            break;
        case 2:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x1C00;
            break;
        case 3:
            EQ_GLOBAL(int, 0x006ECE1C) = 0xA1E;
            break;
        case 4:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x37A6;
            break;
        case 5:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x2660;
            break;
        case 6:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x566;
            break;
        case 7:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x321E;
            break;
        case 8:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x1447;
            break;
        case 9:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x421;
            break;
        case 0xC:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x5618;
            break;
        case 0xB:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x4027;
            break;
        case 0xD:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x5226;
            break;
        case 0xA:
            EQ_GLOBAL(int, 0x006ECE1C) = 0x4B9;
            break;
        case 0xE:
            EQ_GLOBAL(u8, 0x006ECE18) = 0;
            if (musicState->sequence != 0)
            {
                EQ_GLOBAL(AIL_branch_index_fn, 0x0053433C)(musicState->sequence, 3);
            }
            break;
        case 0xF:
            EQ_GLOBAL(u8, 0x006ECE18) = 0;
            musicState->unknown0x45 = 2;
            if (musicState->sequence != 0)
            {
                EQ_GLOBAL(AIL_branch_index_fn, 0x0053433C)(musicState->sequence, 0x40);
            }
            break;
        }
        musicState->command = 0;
    }
    if (musicState->track_queued != 0)
    {
        MIDI_QUEUE_(g_midi_player_b_guess, musicState->track_queued - 1, g_music_volume_guess, 0, 1);
        musicState->track_queued = 0;
    }
    if (musicState->wanted != musicState->have)
    {
        musicState->have = musicState->wanted;
        if ((g_midi_music_guess != 0) && (MIDI_STATE_(g_midi_music_guess) != 1)
            && (EQ_GLOBAL(u8, 0x006ECDF0) == 0))
        {
            if (musicState->priority > 2)
            {
                return;
            }
            MIDI_STOP_(g_midi_music_guess, 0);
            musicState->current = 0;
        }
        if (musicState->track == 0)
        {
            if ((g_midi_player_a_guess != 0) && (MIDI_STATE_(g_midi_player_a_guess) == 6))
            {
                MIDI_STOP_(g_midi_player_a_guess, EQ_GLOBAL(int, 0x006ECDDC));
                musicState->current = 0;
            }
        }
        else if (g_midi_player_a_guess != 0)
        {
            MIDI_QUEUE_(g_midi_player_a_guess, musicState->track - 1, g_music_volume_guess,
                        EQ_GLOBAL(int, 0x006ECDDC), EQ_GLOBAL(int, 0x006ECE10));
        }
    }
    if (EQ_GLOBAL(u8, 0x006ECDF0) != 0)
    {
        if ((g_midi_music_guess != 0) && (MIDI_STATE_(g_midi_music_guess) == 6))
        {
            EQ_METHOD(2, void, 0x004E4522, (void*, long, long), g_midi_music_guess)(g_music_volume_guess / 2, 500);
        }
        EQ_GLOBAL(u8, 0x006ECDF0) = 0;
    }
}

#undef MIDI_STOP_
#undef MIDI_QUEUE_
#undef MIDI_STATE_
#undef MIDI_UPDATE_
#undef ACTOR_STATE_

// FUNCTION: TRILOGY 0x004e5ac8
// Starts the client's opening MIDI music during startup.
// @early-stop: the zero constant takes EBX (CSE'd from the zero store), so the original's EBX=start and its and-form zero store / push 0 arguments don't match; start is called direct through the IAT.
// IAMidi::Opening_Music, __fastcall. Restarts the pitch-zero sequence from the published handles, then loads
// opener4.xmi into a new sequence handle and starts it; each failed step traces its message.
void __fastcall StartOpeningMusic_guess(int midiState)
{
    typedef void* (__stdcall *AIL_file_read_guess_t)(char*, u32*);
    typedef void* (__stdcall *AIL_allocate_sequence_handle_guess_t)(void*);

    AIL_init_sequence_t init;
    u32 handle;
    u32 value = 1;

    if (*(int*)(midiState + 0x4c) != 0)
    {
        return;
    }
    init = AIL_init_sequence;
    if ((g_midi_sequence_guess != 0) && (g_midi_file_image_guess != 0))
    {
        if (init(g_midi_sequence_guess, g_midi_file_image_guess, 3) == 0)
        {
            EQTRACE_guess(EQ_ARRAY(char, 0x0057C564));
            return;
        }
        *(u32*)((char*)g_midi_sequence_guess + 0x30) = value;
        AIL_start_sequence(g_midi_sequence_guess);
    }
    if (*(int*)(midiState + 0x60) != 0)
    {
        handle = (u32)EQ_GLOBAL(AIL_file_read_guess_t, 0x00534320)(EQ_ARRAY(char, 0x0057C558), 0);
        *(u32*)(midiState + 0x50) = handle;
        if (*(int*)(midiState + 0x60) != 0)
        {
            handle = (u32)EQ_GLOBAL(AIL_allocate_sequence_handle_guess_t, 0x00534334)(*(void**)(midiState + 0x60));
            *(u32*)(midiState + 0x4c) = handle;
        }
        if (*(int*)(midiState + 0x4c) == 0)
        {
            EQTRACE_guess(EQ_ARRAY(char, 0x0057C520));
        }
        else
        {
            if (*(int*)(midiState + 0x50) != 0)
            {
                init(*(void**)(midiState + 0x4c), *(void**)(midiState + 0x50), 0);
                *(u32*)((char*)*(void**)(midiState + 0x4c) + 0x30) = value;
                AIL_start_sequence(*(void**)(midiState + 0x4c));
                return;
            }
            EQTRACE_guess(EQ_ARRAY(char, 0x0057C4F4));
        }
    }
    else
    {
        EQTRACE_guess(EQ_ARRAY(char, 0x0057C49C));
        *(u32*)(midiState + 0x50) &= 0;
    }
}

class EqSoundManager_SetCurrentZone_guess
{
public:
    bool SetCurrentZone(char* param_2);
};

// FUNCTION: TRILOGY 0x004e5b8b
// Zone entry (not 'load'): tags listed outdoor zones with a class 1..4, stops music, loads the zone's MIDI file
// ORIGINAL FILE: EverQuest/EQ_SoundManager.cpp (from Mac debug map)
bool EqSoundManager_SetCurrentZone_guess::SetCurrentZone(char* zoneName)
{
    char loaded;
    int tmp;
    void* newMidi;
    char path[40];

    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0053DDDC));
    if (tmp == 0)
        return false;

    *(char*)((char*)this + 0x42) = 0;
    path[0] = 0;
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0053E754));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0057C5AC));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561DBC));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 2;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x005417E8));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 3;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561DB0));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 4;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D80));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D74));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C344));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D88));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C300));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C32C));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561DA4));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 2;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0053DF5C));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 3;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D80));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D74));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 4;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C344));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 2;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C308));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C3D0));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561E00));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 4;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C314));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 3;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561D88));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561CCC));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 2;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561CE0));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x00561CEC));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 4;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C340));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 3;
    }
    tmp = strcmp(zoneName, EQ_ARRAY(char, 0x0055C308));
    if (tmp == 0)
    {
        *(char*)((char*)this + 0x42) = 1;
    }
    if (EQ_GLOBAL(void*, 0x006ECDF8) != 0)
    {
        tmp = EQ_METHOD(0, int, 0x004E47CE, (void*), EQ_GLOBAL(void*, 0x006ECDF8))();
        if (tmp == 6)
        {
            EQ_METHOD(1, void, 0x004E4536, (void*, int), EQ_GLOBAL(void*, 0x006ECDF8))(0);
        }
        while ((tmp = EQ_METHOD(0, int, 0x004E47CE, (void*), EQ_GLOBAL(void*, 0x006ECDF8))()) != 1)
        {
            EQ_METHOD(0, void, 0x004E465A, (void*), EQ_GLOBAL(void*, 0x006ECDF8))();
        }
    }
    if (EQ_GLOBAL(void*, 0x006ECDFC) != 0)
    {
        tmp = EQ_METHOD(0, int, 0x004E47CE, (void*), EQ_GLOBAL(void*, 0x006ECDFC))();
        if (tmp == 6)
        {
            EQ_METHOD(1, void, 0x004E4536, (void*, int), EQ_GLOBAL(void*, 0x006ECDFC))(0);
        }
        while ((tmp = EQ_METHOD(0, int, 0x004E47CE, (void*), EQ_GLOBAL(void*, 0x006ECDFC))()) != 1)
        {
            EQ_METHOD(0, void, 0x004E465A, (void*), EQ_GLOBAL(void*, 0x006ECDFC))();
        }
    }
    if (EQ_GLOBAL(int, 0x006ECE00) != 0)
    {
        tmp = EQ_GLOBAL(int, 0x006ECE00);
        if (tmp != 0)
        {
            EQ_METHOD(0, void, 0x004E44D3, (void*), (void*)tmp)();
            operator delete((void*)tmp);
        }
    }
    newMidi = operator new(0x58);
    if (newMidi != (void*)0)
    {
        newMidi = EQ_METHOD(0, void*, 0x004E4482, (void*), newMidi)();
    }
    else
    {
        newMidi = 0;
    }
    EQ_GLOBAL(void*, 0x006ECE00) = newMidi;
    sprintf(path, EQ_ARRAY(char, 0x0057C5A4), zoneName);
    loaded = EQ_METHOD(1, char, 0x004E4401, (void*, char*), EQ_GLOBAL(void*, 0x006ECE00))(path);
    if (loaded == '\0')
    {
        EQ_GLOBAL(char, 0x006ECE04) = loaded;
        tmp = EQ_GLOBAL(int, 0x006ECE00);
        if (tmp != 0)
        {
            EQ_METHOD(0, void, 0x004E44D3, (void*), (void*)tmp)();
            operator delete((void*)tmp);
        }
        EQ_GLOBAL(void*, 0x006ECE00) = 0;
    }
    else
    {
        EQ_GLOBAL(char, 0x006ECE04) = 1;
        return 1;
    }
    return 0;
}


// FUNCTION: TRILOGY 0x004e824c
// Thunk to 0x004E8661; no separate game action is visible.
void __fastcall thunk_FUN_004e8661(int *ptr)
{
    EQ_FUNC(void, __fastcall, 0x004E8661, (int *))(ptr);
}

// FUNCTION: TRILOGY 0x004e8661
// Releases the held file resource and clears its owner reference.
void __fastcall ReleaseHeldObject_guess(int *objPtr)
{
    int obj;

    if ((*objPtr != 0) && (*(int *)((char *)*objPtr + 0x14) != 0))
    {
        EQ_METHOD(0, void, 0x004F96D0, (void *), (char *)*objPtr + 0x14)();
        obj = *objPtr;
        if (obj != 0)
        {
            EQ_METHOD(0, void, 0x004E8693, (void *), (void *)obj)();
            operator delete((void *)obj);
        }
        *objPtr = 0;
    }
    return;
}

// FUNCTION: TRILOGY 0x004e869b
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range)
// __thiscall.
char* EqHandleRef_guess::Get_guess(void)
{
    return ref;
}

// FUNCTION: TRILOGY 0x004e869e
// ORIGINAL FILE: Source/PC Source/EverQuest/EQ_SoundManager.cpp (address range; TAKP: CXStr_GetStringEncodingType, low)
// __thiscall. The dword at +0x18 of the referenced object, 0 without one.
u32 EqHandleRef_guess::GetField18_guess(void)
{
    if (ref == 0)
        return 0;
    return *(u32*)(ref + 0x18);
}

// FUNCTION: TRILOGY 0x004e86a9
// Reads up to the requested byte count, clamped to the bytes left, into the buffer and advances the position.
// __fastcall-style `this` (ECX), 2 stack args, callee pops 8. `*statePtr` is the object: when it, `audioData` and
// `requestedBytes` are nonzero, clamps `requestedBytes` to the bytes left (+0x18 minus the read position at +0x20) when it is not
// larger, adds that count to +0x20, then returns ReadPackFileData_guess(audioData, count) on the sub-object at +0x14
// when the dword at +0x14 is set. Otherwise returns 0.
int __fastcall ReadPackFileClamped_guess(int* statePtr, int unused_edx, u32* audioData, int requestedBytes)
{
    int state;
    int readBytes;

    state = *statePtr;
    if (state != 0 && audioData != 0 && requestedBytes != 0)
    {
        readBytes = *(int*)((char*)state + 0x18) - *(int*)((char*)state + 0x20);
        if (requestedBytes <= readBytes)
            readBytes = requestedBytes;
        *(int*)((char*)state + 0x20) = *(int*)((char*)state + 0x20) + readBytes;
        if (*(int*)((char*)*statePtr + 0x14) != 0)
            return EQ_METHOD(2, int, 0x004F9730, (void*, u32*, int), (char*)*statePtr + 0x14)(audioData, readBytes);
    }
    return 0;
}

typedef int (__stdcall *EqQueryInterface_guess_t)(u32* object, const void* iid, u32* out);

struct EqWaveFormat_guess
{
    u32 firstDword;
    u32 secondDword;
    u32 thirdDword;
    u32 fourthDword;
    u16 trailingWord;
    u32 padding;
    u32 extraPadding;
};

struct EqSoundBufferDesc_guess
{
    u32 size;
    u32 flags;
    u32 bufferBytes;
    u32 reserved;
    EqWaveFormat_guess* format;
    u8 unused[0x10];
};

struct EqSoundLoadState_guess
{
    u8 unused0[4];
    u32 bufferObject;
    u8 unused8[0x3e];
    u8 useMonoFlags;
    u8 unused47;
    u32 formatSecondDword;
    u16 formatFourthDwordLowWord;
    u16 unused4e;
    u32 bufferBytes;
    char* fileName;
    u32 state;
};

struct EqSoundResourceOwner_guess
{
    char* ref;
};

struct EqSoundResourceFrame_guess
{
    EqSoundBufferDesc_guess bufferDescription;
    u8 unused24[0x24];
    EqSoundResourceOwner_guess resource;
};

struct EqSoundFrameGuard_guess
{
    u32 marker;

    ~EqSoundFrameGuard_guess()
    {
        volatile u32* observedMarker = &marker;
        *observedMarker;
    }
};

typedef int (__stdcall *EqCreateSoundBuffer_guess_t)(void*, EqSoundBufferDesc_guess*, void**, void*);
typedef int (__stdcall *EqLockSoundBuffer_guess_t)(void*, u32, u32, void**, u32*, void**, u32*, u32);
typedef int (__stdcall *EqUnlockSoundBuffer_guess_t)(void*, void*, u32, void*, u32);
typedef int (__stdcall *EqReleaseSoundBuffer_guess_t)(void*);

// FUNCTION: TRILOGY 0x004eba06
// Loads a named wave file into a new DirectSound buffer for this sound object and records the file name
u32 __fastcall LoadWaveIntoBuffer_guess(void* self, int unusedEdx, u32** soundSource, char* fileName)
{
    EqSoundLoadState_guess* state = (EqSoundLoadState_guess*)self;
    EqSoundFrameGuard_guess frameGuard;
    EqWaveFormat_guess format;
    EqSoundBufferDesc_guess bufferDescription;
    void* soundBuffer;
    void* audioData;
    u32 audioBytes;
    u32 result;

    ((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource.ref = 0;
    result = 0;
    if (*soundSource != 0 && fileName != 0)
    {
        u32 pathStorage[2];
        __asm
        {
            push ecx
            push ecx
        }
        EQ_METHOD(1, void, 0x004C479D, (void*, char*), (char*)pathStorage - 84)(fileName);
        if (EQ_METHOD(0, char, 0x004E8251, (EqSoundResourceOwner_guess*),
            &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)() != 0
            && state->bufferObject == 0)
        {
            u32* formatSource = (u32*)EQ_METHOD(0, char*, 0x004E869B,
                (void*), &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();
            ((EqWaveFormat_guess*)((char*)&format + 4))->firstDword = formatSource[0];
            ((EqWaveFormat_guess*)((char*)&format + 4))->secondDword = formatSource[1];
            ((EqWaveFormat_guess*)((char*)&format + 4))->thirdDword = formatSource[2];
            ((EqWaveFormat_guess*)((char*)&format + 4))->fourthDword = formatSource[3];
            ((EqWaveFormat_guess*)((char*)&format + 4))->trailingWord = ((u16*)formatSource)[8];
            state->formatSecondDword = ((EqWaveFormat_guess*)((char*)&format + 4))->secondDword;
            state->formatFourthDwordLowWord = (u16)((EqWaveFormat_guess*)((char*)&format + 4))->fourthDword;
            state->bufferBytes = EQ_METHOD(0, u32, 0x004E869E,
                (void*), &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();

            memset(
                (EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4), 0, 0x24);
            ((EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4))->bufferBytes = state->bufferBytes;
            ((EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4))->flags = 0x4092;
            ((EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4))->size = 0x24;
            ((EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4))->format =
                (EqWaveFormat_guess*)((char*)&format + 4);
            if (state->useMonoFlags != 0)
                ((EqSoundBufferDesc_guess*)((char*)&bufferDescription + 4))->flags = 0x4090;

            __asm
            {
                mov edi, dword ptr [ebp + 8]
                mov eax, dword ptr [edi]
                mov ecx, dword ptr [eax]
                push 0
                lea edx, [ebp - 18h]
                push edx
                lea edx, [ebp - 5Ch]
                push edx
                push eax
                call dword ptr [ecx + 0Ch]
                mov result, eax
            }
            if (result == 0)
            {
                __asm
                {
                    mov eax, dword ptr [ebp - 18h]
                    mov soundBuffer, eax
                }
                __asm
                {
                    mov eax, dword ptr [ebp - 18h]
                    mov ecx, dword ptr [eax]
                    push 2
                    push 0
                    push 0
                    lea edx, [ebp - 20h]
                    push edx
                    lea edx, [ebp - 1Ch]
                    push edx
                    push 0
                    push 0
                    push eax
                    call dword ptr [ecx + 2Ch]
                    mov result, eax
                }
                if (result == 0)
                {
                    __asm
                    {
                        mov eax, dword ptr [ebp - 1Ch]
                        mov audioData, eax
                        mov eax, dword ptr [ebp - 20h]
                        mov audioBytes, eax
                    }
                    EQ_METHOD(2, int, 0x004E86A9, (int*, u32*, int),
                        (int*)&((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)(
                        (u32*)audioData, (int)state->bufferBytes);
                    EQ_METHOD(0, void, 0x004E8661, (EqSoundResourceOwner_guess*),
                        &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();
                    __asm
                    {
                        mov eax, dword ptr [ebp - 18h]
                        mov ecx, dword ptr [eax]
                        push 0
                        push 0
                        push dword ptr [ebp - 20h]
                        push dword ptr [ebp - 1Ch]
                        push eax
                        call dword ptr [ecx + 4Ch]
                    }

                    result = audioBytes;
                    {
                        void* newSoundObject = operator new(0x1c);
                        if (newSoundObject == 0)
                            state->bufferObject = 0;
                        else
                            state->bufferObject = (u32)EQ_METHOD(2, u32*, 0x004EBD52,
                                (void*, char*, u32*), newSoundObject)((char*)*soundSource,
                                    (u32*)soundBuffer);
                    }
                    state->fileName = (char*)operator new(
                        (u32)(strlen(fileName) + 1));
                    strcpy(state->fileName, fileName);
                    state->state = 0;
                    goto done;
                }

                EQ_METHOD(0, void, 0x004E8661, (EqSoundResourceOwner_guess*),
                    &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();
                __asm
                {
                    mov eax, dword ptr [ebp - 18h]
                    mov ecx, dword ptr [eax]
                    push eax
                    call dword ptr [ecx + 8]
                }
            }
            else
                EQ_METHOD(0, void, 0x004E8661, (EqSoundResourceOwner_guess*),
                    &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();
        }
    }

    result = 0xffffffff;

done:
    EQ_METHOD(0, void, 0x004E824C, (void*),
        &((EqSoundResourceFrame_guess*)((char*)&bufferDescription + 4))->resource)();
    return result;
}

// FUNCTION: TRILOGY 0x004ebd52
// Prepares 3D asset interfaces for use; the specific in-game role is unclear.
// @early-stop: initial register order, QueryInterface argument scheduling, throw setup order, and final block layout still differ
// fake match: return the success value if the non-returning exception helper unexpectedly returns.
// Constructor, `this` in ECX (no header class for it; vtable 0x00535864). Stores `name` in 0x006EFE40 and
// the COM object at +4, queries it for the 3D interface (+8, IID 0x00536E98; failure throws the char*
// "Failed to get 3D Interface", throw info 0x0053A190) and a second interface (+0xC, IID 0x00536E88;
// cleared on failure). Throws "Invalid Pointers" when either argument is NULL.
u32* __fastcall InitializeComInterfaces_guess(u32* self, int unused_edx, char* name, u32* com)
{
    int result;
    u32* slot = self + 2;
    self[1] = 0;
    *slot = 0;
    *(u8*)(self + 4) = 0;
    self[5] = 0;
    self[6] = 0;
    self[0] = 0x00535864;
    if (name != 0 && com != 0)
    {
        EQ_GLOBAL(char*, 0x006EFE40) = name;
        self[1] = (u32)com;
        result = (*(EqQueryInterface_guess_t*)*com)(com, (const void*)0x00536E98, slot);
        if (result != 0)
        {
            name = (char*)0x0057DF7C;
            EQ_STDCALL(void, 0x005218D4, (void*, void*))(&name, (void*)0x0053A190);
        }
        result = (*(EqQueryInterface_guess_t*)*(u32*)self[1])((u32*)self[1], (const void*)0x00536E88, self + 3);
        if (result != 0)
            self[3] = 0;
        return self;
    }
    name = (char*)0x0057DF68;
    EQ_STDCALL(void, 0x005218D4, (void*, void*))(&name, (void*)0x0053A190);
    return self;
}
