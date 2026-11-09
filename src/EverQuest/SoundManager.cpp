// SoundManager.cpp - the low-level sound objects (sound buffers, assets).
// ORIGINAL FILE: Source/PC Source/EverQuest/SoundManager.cpp (from Mac debug map)
// 0x004ECB82 / 0x004ECBC2 sit next to EQSoundBuffer_guess::SetVolume (0x004ECBA1, rewritten in EQMisc.cpp)
// and the Mac-mapped SoundAsset::~SoundAsset (0x004ECD97); the queue's graph labels put them in
// EverQuest/EQ_SoundManager.

#include "trilogy/common.h"
#include "trilogy/sound.h"
#include "trilogy/eqaddr.h"

// The 16 voice slots object (pointers at 0x04 + slot * 4) seen through the one method rewritten here.
// Local: sound.h's EqDirectSoundSlots_guess has no declaration for it yet.
class EqDirectSoundSlotsStop_guess
{
public:
    void StopSlot_guess(int *slot);
    void UpdateSlotEffectAttributes_guess(u32 slot);
    int CountPlayingWithId_guess(int id);
    bool GetSlotStatus_guess(int slot, u32 *status);
    void SetMasterVolume_guess(int volume);
};

// The voice (0x1C bytes or more) as built by its duplicating constructor. Local: sound.h's
// EqDirectSoundVoice_guess declares no constructor yet.
class EqDirectSoundVoiceDup_guess
{
public:
    u8 storage[0x1c];
    EqDirectSoundVoiceDup_guess(EqDirectSoundVoiceDup_guess *source);
};

// kernel32 OutputDebugStringA without <windows.h>; the import resolves to kernel32.dll on both sides.
extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA(const char *text);

#include "trilogy/layout/eq_dsound.h"

// The DirectSound buffer a voice holds at +4, called through its COM vtable (IDirectSoundBuffer order).
// Local: no header declares the interface yet; only the slots the rewritten functions call are named.
class EqSoundBufferInterface_guess
{
public:
    virtual long __stdcall QueryInterface_guess(const void *iid, void **out);                /* 0x00 */
    virtual unsigned long __stdcall AddRef_guess(void);                                       /* 0x04 */
    virtual unsigned long __stdcall Release_guess(void);                                      /* 0x08 */
    virtual long __stdcall GetCaps_guess(void *caps);                                         /* 0x0C */
    virtual long __stdcall GetCurrentPosition_guess(u32 *play, u32 *write);                   /* 0x10 */
    virtual long __stdcall GetFormat_guess(void *format, u32 size, u32 *written);             /* 0x14 */
    virtual long __stdcall GetVolume_guess(int *volume);                                      /* 0x18 */
    virtual long __stdcall GetPan_guess(int *pan);                                            /* 0x1C */
    virtual long __stdcall GetFrequency_guess(u32 *frequency);                                /* 0x20 */
    virtual long __stdcall GetStatus_guess(u32 *status);                                      /* 0x24 */
    virtual long __stdcall Initialize_guess(void *sound, const void *description);           /* 0x28 */
    virtual long __stdcall Lock_guess(u32 offset, u32 bytes, void **first, u32 *firstBytes,
                                      void **second, u32 *secondBytes, u32 flags);             /* 0x2C */
    virtual long __stdcall Play_guess(u32 reserved, u32 priority, u32 flags);                 /* 0x30 */
    virtual long __stdcall SetCurrentPosition_guess(u32 position);                            /* 0x34 */
    virtual long __stdcall SetFormat_guess(const void *format);                               /* 0x38 */
    virtual long __stdcall SetVolume_guess(int volume);                                       /* 0x3C */
    virtual long __stdcall SetPan_guess(int pan);                                             /* 0x40 */
    virtual long __stdcall SetFrequency_guess(u32 frequency);                                 /* 0x44 */
    virtual long __stdcall Stop_guess(void);                                                  /* 0x48 */
};

// The first entry on the objects in SoundAsset's pointer array is a thiscall release method.
class SoundAssetRelease_guess
{
public:
    virtual void Release_guess(int force) = 0;
};

typedef void (__stdcall *SoundAssetVtableOneArg_guess)(int *object);
typedef void (__stdcall *SoundAssetVtableTwoArgs_guess)(int *object, int arg);
typedef int (__stdcall *SoundAssetTerminateThread_guess)(u32 thread, u32 exitCode);
typedef int (__stdcall *SoundAssetFreeLibrary_guess)(u32 module);
class SoundAssetEHMarker_guess
{
public:
    SoundAssetEHMarker_guess(void) {}
    ~SoundAssetEHMarker_guess(void) {}
};

// FUNCTION: TRILOGY 0x00401011
// Begins calibration of the client's timing scale.
// Calls the clock calibration (CalibrateTscFrequency_guess, no object) and returns the incoming ECX. A
// __fastcall free function since the object type is unknown.
void* __fastcall InitializeTiming_guess(void* self)
{
    EQ_CDECL(void, 0x0040101E, (void))();
    return self;
}

// FUNCTION: TRILOGY 0x0040101e
// Calibrates the clock scale used for sound timing.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (from Mac debug map)
// Times one wall-clock second in RDTSC ticks: waits for a second boundary, reads the counter, waits
// for the next boundary, reads it again; the low-dword delta / 1000 goes to 0x0058FB54.
void SoundAsset::CalibrateTscFrequency_guess(void)
{
    unsigned long startLo;
    unsigned long endLo;
    unsigned long hi;
    int t0;
    int t1;

    t0 = EQ_CDECL(int, 0x005213E9, (int))(0);
    t1 = EQ_CDECL(int, 0x005213E9, (int))(0);
    while (t1 == t0)
        t1 = EQ_CDECL(int, 0x005213E9, (int))(0);
    __asm
    {
        rdtsc
        mov startLo, eax
        mov endLo, edx
    }
    while (t1 == EQ_CDECL(int, 0x005213E9, (int))(0))
        ;
    __asm
    {
        rdtsc
        mov endLo, eax
        mov hi, edx
    }
    *(unsigned long *)0x0058fb54 = (endLo - startLo) / 1000;
}

// FUNCTION: TRILOGY 0x004eb931
// Prepares an available sound channel for a new sound.
// @early-stop: the zero store is mov instead of and, and UpdateSlot loads ECX before pushing its argument.
/* fake match: volatile field writes keep VC6 from moving the writes past call setup. */
int __fastcall ReserveSoundSlot_guess(EqDirectSoundSlots_guess *slots)
{
    int slotIndex;

    slotIndex = EQ_METHOD(0, int, 0x004EC55D, (EqDirectSoundSlots_guess *), slots)();
    if (slotIndex == -1)
        return -1;

    *(volatile u8 *)(*(u8 **)((u8 *)slots + 4 + slotIndex * 4) + 0x10) = 1;
    *(volatile int *)(*(u8 **)((u8 *)slots + 4 + slotIndex * 4) + 0x14) = 0;
    EQ_METHOD(2, void, 0x004EC713, (EqDirectSoundSlots_guess *, int, int), slots)(slotIndex, 0);
    EQ_METHOD(1, void, 0x004EC986, (EqDirectSoundSlotsStop_guess *, u32), slots)(slotIndex);

    return slotIndex;
}

// FUNCTION: TRILOGY 0x004eb96a
// Stops a looping sound and clears its voice channel.
// __thiscall. If the slot's voice is active: Stop() its buffer (vtable 0x48) when the status has
// bit 4 (looping) set, then clear the voice's active byte at 0x10.
void EqDirectSoundSlotsStop_guess::StopSlot_guess(int *slotIndex)
{
    u8 *voice;
    void *buffer;
    typedef int (__stdcall *Stop)(void *);
    if (*slotIndex >= 0 && *slotIndex < 16)
    {
        voice = ((u8 **)this)[1 + *slotIndex];
        const s32 & tmp = (char)voice;
        if (((int)(voice != 0 && voice[0x10] != 0 && 0 != tmp)))
        {
            if ((EQ_METHOD(0, u32, 0x004EC004, (void *), voice)() & 4) == 0)
            {
            }
            else
            {
                buffer = *(void **)(((u8 **)this)[*slotIndex + 1] + 4);
                ((Stop)(*(u32 **)buffer)[18])(buffer);
            }
            ((u8 **)this)[*slotIndex + 1][0x10] = 0;
        }
    }
}

// FUNCTION: TRILOGY 0x004ec004
// Gets the DirectSound playback flags for a voice.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
u32 EqDirectSoundVoice_guess::GetStatus_guess()
{
    u32 value = 0;
    (*(EqSoundBufferInterface_guess **)((char *)this + 4))->GetStatus_guess(&value);
    return value;
}

// The IDirectSoundBuffer seen through the one method rewritten here: a C-style COM vtable (guess).
// Local: sound.h has no buffer class. Slots 0-3 are IUnknown and GetCaps; slot 4 (0x10) is GetCurrentPosition.
struct EqDirectSoundBufferVtbl_guess
{
    void *slots_0_to_3[4];                                                    /* 0x00 */
    int (__stdcall *GetCurrentPosition)(void *self, u32 *play, void *write);  /* 0x10 */
};

struct EqDirectSoundBufferPosition_guess
{
    EqDirectSoundBufferVtbl_guess *lpVtbl;                                    /* 0x00 */
};

// FUNCTION: TRILOGY 0x004EC01E
// Gets the current playback position of a sound buffer.
// @early-stop: the output-address LEA stays before the null-argument push.
// ECX = voice, no stack arguments, callee pops 0. Reads the buffer at voice+4 and calls its
// GetCurrentPosition (vtable slot 4) with (&value, NULL); returns the play cursor it wrote.
/* fake match: the volatile alias makes VC6 load the vtable before preparing call arguments. */
u32 __fastcall GetPlayPosition_guess(EqDirectSoundVoice_guess *voice, int edx_unused)
{
    u32 value;
    EqDirectSoundBufferPosition_guess *buffer;
    volatile EqDirectSoundBufferPosition_guess *volatileBuffer;

    buffer = *(EqDirectSoundBufferPosition_guess **)((char *)voice + 4);
    value = 0;
    volatileBuffer = buffer;
    volatileBuffer->lpVtbl->GetCurrentPosition(buffer, &value, 0);
    return value;
}

// Local: no header declares CXWndEx yet. Only its scalar deleting destructor is rewritten here.
class CXWndEx_guess
{
public:
    CXWndEx_guess *ScalarDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004EC03A
// Releases a sound voice and frees it when requested.
// __thiscall, callee pops 4. Scalar deleting destructor: runs the destructor at 0x004EC056 (ReleaseVoiceInterfaces_guess),
// frees the object with the cdecl operator delete at 0x005218BB when bit 0 of flags is set, returns this.
CXWndEx_guess *CXWndEx_guess::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004EC056, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

// FUNCTION: TRILOGY 0x004EC056
// Releases the DirectSound interfaces held by a voice.
// ECX = this, no stack arguments, callee pops 0. Sets the vtable at +0, then releases the objects at +0xC,
// +8 and +4 in that order, each through vtable slot 2 (stdcall, the object pushed) when non-null. Each
// field is cleared after the next one is read; +4 is cleared last.
void __fastcall ReleaseVoiceInterfaces_guess(int *self, int edx_unused)
{
    int *voiceInterface;

    voiceInterface = (int *)self[3];
    self[0] = (int)EQ_ARRAY(char, 0x00535864);
    if (voiceInterface != 0)
    {
        ((SoundAssetVtableOneArg_guess)(*(void ***)voiceInterface)[2])(voiceInterface);
    }
    voiceInterface = (int *)self[2];
    self[3] = 0;
    if (voiceInterface != 0)
    {
        ((SoundAssetVtableOneArg_guess)(*(void ***)voiceInterface)[2])(voiceInterface);
    }
    voiceInterface = (int *)self[1];
    self[2] = 0;
    if (voiceInterface != 0)
    {
        ((SoundAssetVtableOneArg_guess)(*(void ***)voiceInterface)[2])(voiceInterface);
    }
    self[1] = 0;
}

// FUNCTION: TRILOGY 0x004EC094
// Restarts one copy of a sound from the start (looping if flagged); when enabled, logs its 3D/EAX play state.
// @early-stop: entry guard order, indirect-call argument scheduling, and x87 vararg setup still differ.
char __fastcall PlayR(EqDirectSoundSlots_guess *slots, int edx, int slot, int unused)
{
    int *entry;
    void *control;
    void *properties;
    /* fake match: keep status, play values, and property values in the original stack slots. */
    struct LogValues
    {
        int local_28;
        int local_24;
        u32 status;
        int local_1c;
        int local_18;
        union { float value; u32 bits; } local_14;
        int volume;
        int local_c;
    } play_values;
    u8 propSupport[4];
    float position[3];
    char text[200];
    int result;
    u32 master_attenuation;
    u32 sound_id;
    const char *debug_text;
    typedef int (__stdcall *StopBuffer)(void *);
    typedef int (__stdcall *SetPosition)(void *, u32);
    typedef int (__stdcall *PlayBuffer)(void *, u32, u32, u32);
    typedef int (__stdcall *GetFloat)(void *, float *);
    typedef int (__stdcall *GetStatus)(void *, u32 *);
    typedef int (__stdcall *GetVector)(void *, float *);
    typedef int (__stdcall *QueryProperty)(void *, const void *, u32, u8 *);
    typedef int (__stdcall *GetProperty)(void *, const void *, u32, void *, u32, void *, u32, u8 *);

    entry = (int *)((char *)slots + 4 + slot * 4);
    if (*entry == 0 || slot < 0 || slot >= 16 || *((char *)*entry + 0x10) == '\0')
        return 0;

    play_values.local_14.bits = 0;
    play_values.local_c = 0;
    if (*((char *)slots + 0x46) != '\0' || (*((u8 *)*entry + 0x14) & 1) != 0)
    {
        play_values.local_14.bits = 1;
        play_values.local_c = 1;
    }

    ((StopBuffer)(*(u32 **)*(void **)((char *)*entry + 4))[0x48 / 4])(*(void **)((char *)*entry + 4));
    ((SetPosition)(*(u32 **)*(void **)((char *)*entry + 4))[0x34 / 4])(*(void **)((char *)*entry + 4), 0);
    ((PlayBuffer)(*(u32 **)*(void **)((char *)*entry + 4))[0x30 / 4])(*(void **)((char *)*entry + 4), 0, 0, play_values.local_14.bits);

    if (EQ_GLOBAL(u8, 0x006EFE38) == 0)
        return 1;

    play_values.volume = EQ_METHOD(0, int, 0x004EC42B, (void *), *(void **)entry)();
    play_values.local_14.value = 0.0f;
    control = *(void **)((char *)*entry + 8);
    ((GetFloat)(*(u32 **)control)[0x20 / 4])(control, &play_values.local_14.value);
    if (*(void **)((char *)*entry + 8) != 0)
    {
        play_values.status = 0;
        control = *(void **)((char *)*entry + 8);
        result = ((GetStatus)(*(u32 **)control)[0x24 / 4])(control, &play_values.status);
        if (result != 0)
            return 1;
    
        if (*((char *)slots + 0x44) != '\0')
        {
            play_values.local_1c = 0;
            play_values.local_18 = 0;
            play_values.local_24 = 0;
            play_values.local_28 = 0;
            properties = *(void **)((char *)*entry + 0x0c);
            result = ((QueryProperty)(*(u32 **)properties)[0x14 / 4])
                (properties, (const void *)0x005359C0, 2, propSupport);
            if (result == 0 && (propSupport[0] & 1) != 0)
            {
                properties = *(void **)((char *)*entry + 0x0c);
                result = ((GetProperty)(*(u32 **)properties)[0x0c / 4])
                    (properties, (const void *)0x005359C0, 2, 0, 0, &play_values.local_1c, 4, propSupport);
                if (result != 0)
                    play_values.local_1c = 0x1E240;
            }
    
            properties = *(void **)((char *)*entry + 0x0c);
            result = ((QueryProperty)(*(u32 **)properties)[0x14 / 4])
                (properties, (const void *)0x005359C0, 4, propSupport);
            if (result == 0 && (propSupport[0] & 1) != 0)
            {
                properties = *(void **)((char *)*entry + 0x0c);
                result = ((GetProperty)(*(u32 **)properties)[0x0c / 4])
                    (properties, (const void *)0x005359C0, 4, 0, 0, &play_values.local_18, 4, propSupport);
                if (result != 0)
                    play_values.local_18 = 0x1E240;
            }
    
            properties = *(void **)((char *)*entry + 0x0c);
            result = ((QueryProperty)(*(u32 **)properties)[0x14 / 4])
                (properties, (const void *)0x005359C0, 7, propSupport);
            if (result == 0 && (propSupport[0] & 1) != 0)
            {
                properties = *(void **)((char *)*entry + 0x0c);
                result = ((GetProperty)(*(u32 **)properties)[0x0c / 4])
                    (properties, (const void *)0x005359C0, 7, 0, 0, &play_values.local_24, 4, propSupport);
                if (result != 0)
                    play_values.local_24 = 0x1E240;
            }
    
            properties = *(void **)((char *)*entry + 0x0c);
            result = ((QueryProperty)(*(u32 **)properties)[0x14 / 4])
                (properties, (const void *)0x005359C0, 9, propSupport);
            if (result == 0 && (propSupport[0] & 1) != 0)
            {
                properties = *(void **)((char *)*entry + 0x0c);
                result = ((GetProperty)(*(u32 **)properties)[0x0c / 4])
                    (properties, (const void *)0x005359C0, 9, 0, 0, &play_values.local_28, 4, propSupport);
                if (result != 0)
                    play_values.local_28 = 0x1E240;
            }
    
            if ((play_values.status & 1) != 0)
            {
                sound_id = *(u32 *)((char *)*entry + 0x18);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057E1E0,
                     *(const char **)((char *)slots + 0x54), slot, play_values.volume,
                     play_values.local_1c, play_values.local_18, play_values.local_24, play_values.local_28, play_values.local_c,
                     EQ_GLOBAL(u32, 0x006EFE3C), sound_id);
            }
            else if ((play_values.status & 2) != 0)
            {
                sound_id = *(u32 *)((char *)*entry + 0x18);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057E174,
                     *(const char **)((char *)slots + 0x54), slot, play_values.volume,
                     play_values.local_1c, play_values.local_18, play_values.local_24, play_values.local_28, play_values.local_c,
                     EQ_GLOBAL(u32, 0x006EFE3C), sound_id);
            }
            else
            {
                control = *(void **)((char *)*entry + 8);
                ((GetVector)(*(u32 **)control)[0x28 / 4])(control, &position[0]);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057E0EC,
                     *(const char **)((char *)slots + 0x54), slot,
                     (double)play_values.local_14.value, (double)position[0], (double)position[1], (double)position[2],
                     play_values.volume, play_values.local_1c, play_values.local_18, play_values.local_24, play_values.local_28, play_values.local_c,
                     EQ_GLOBAL(u32, 0x006EFE3C), *(u32 *)((char *)*entry + 0x18));
            }
        }
        else
        {
            master_attenuation = *(u32 *)((char *)slots + 0x60);
            if ((play_values.status & 1) != 0)
            {
                sound_id = *(u32 *)((char *)*entry + 0x18);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057E08C,
                     *(const char **)((char *)slots + 0x54), slot, play_values.volume, play_values.local_c,
                     EQ_GLOBAL(u32, 0x006EFE3C), master_attenuation, sound_id);
            }
            else if ((play_values.status & 2) != 0)
            {
                sound_id = *(u32 *)((char *)*entry + 0x18);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057E02C,
                     *(const char **)((char *)slots + 0x54), slot, play_values.volume, play_values.local_c,
                     EQ_GLOBAL(u32, 0x006EFE3C), master_attenuation, sound_id);
            }
            else
            {
                control = *(void **)((char *)*entry + 8);
                ((GetVector)(*(u32 **)control)[0x28 / 4])(control, &position[0]);
                EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))
                    (text, (const char *)0x0057DFB4,
                     *(const char **)((char *)slots + 0x54), slot,
                     (double)play_values.local_14.value, (double)position[0], (double)position[1], (double)position[2],
                     play_values.volume, play_values.local_c, EQ_GLOBAL(u32, 0x006EFE3C), master_attenuation,
                     *(u32 *)((char *)*entry + 0x18));
            }
        }
        debug_text = text;
    }
    else
    {
        debug_text = (const char *)0x0057DF98;
    }

    EQ_CDECL(void, 0x004F8220, (const char *))(debug_text);
    return 1;
}

// FUNCTION: TRILOGY 0x004ec42b
// Gets the playback volume set on a sound buffer.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
int EqDirectSoundVoice_guess::GetVolume_guess()
{
    int value = 0;
    (*(EqSoundBufferInterface_guess **)((char *)this + 4))->GetVolume_guess(&value);
    return value;
}

// FUNCTION: TRILOGY 0x004ec445
// Stops and rewinds the sound on a reserved key.
void __fastcall EqDirectSoundSlots_StopReserved_guess(EqDirectSoundSlots_guess *slots, int edx, int slot)
{
    char buf[200];
    int *voice;
    int *buffer;
    typedef int (__stdcall *BufferStop)(void *);
    typedef int (__stdcall *BufferSetPosition)(void *, u32);

    if (slot < 0 || slot >= 16 || (voice = *(int **)((u8 *)slots + 4 + slot * 4)) == 0 || *((u8 *)voice + 0x10) == 0)
    {
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(buf, EQ_ARRAY(char, 0x0057E24C), *(char **)((u8 *)slots + 0x54), slot);
        OutputDebugStringA(buf);
        return;
    }
    if (voice[1] == 0)
        return;
    buffer = (int *)voice[1];
    ((BufferStop)(*(u32 **)buffer)[18])(buffer);
    buffer = *(int **)(*(int *)((u8 *)slots + 4 + slot * 4) + 4);
    ((BufferSetPosition)(*(u32 **)buffer)[13])(buffer, 0);
    return;
}

// FUNCTION: TRILOGY 0x004ec4b5
// Stops and rewinds the sound buffers in the voice table.
// __fastcall, this in ECX, no stack args. Walks the 16 voice slots (this + 4 + i * 4) ascending and, for each
// non-null slot, calls Stop (buffer vtable 0x48) then SetCurrentPosition(0) (vtable 0x34) on the slot's buffer
// (voice + 4). The walk only runs when slot 0 is non-null: the gate tests slot 0 alone.
void __fastcall EqDirectSoundSlots_StopAllRewind_guess(EqDirectSoundSlots_guess *slots, int edx)
{
    int **slot;
    int *buffer;
    int i;
    typedef int (__stdcall *BufferStop)(void *);
    typedef int (__stdcall *BufferSetPosition)(void *, u32);

    if (*(int *)((u8 *)slots + 4) != 0)
    {
        slot = (int **)((u8 *)slots + 4);
        i = 16;
        do
        {
            if (*slot != 0)
            {
                buffer = *(int **)((u8 *)*slot + 4);
                ((BufferStop)(*(u32 **)buffer)[18])(buffer);
                buffer = *(int **)((u8 *)*slot + 4);
                ((BufferSetPosition)(*(u32 **)buffer)[13])(buffer, 0);
            }
            slot++;
            i--;
        } while (i != 0);
    }
}

// FUNCTION: TRILOGY 0x004EC4E8
// Sets a channel's buffer frequency: float ratio times the rate at +0x48, clamped to 100..100000.
// @early-stop: the NaN guard and two post-conversion instruction-order differences remain.
/* fake match: keep the clamped float separate to preserve the original x87 compare stack. */
void __fastcall SetSlotFrequency_guess(EqDirectSoundSlots_guess *slots, int edx, int slot, float volume)
{
    int *voice;
    u32 scale;

    voice = *(int **)((u8 *)slots + 4 + slot * 4);
    if (voice != 0)
    {
        float adjustedVolume = volume;
        // Keep NaN on the client's lower-clamp path.
        if ((*(u32 *)&volume & 0x7FFFFFFF) > 0x7F800000)
            adjustedVolume = EQ_GLOBAL(f32, 0x005344A8);
        else
        {
            if (volume > EQ_GLOBAL(double, 0x00535868))
                adjustedVolume = EQ_GLOBAL(f32, 0x005350F0);
            if (adjustedVolume < EQ_GLOBAL(double, 0x005344B0))
                adjustedVolume = EQ_GLOBAL(f32, 0x005344A8);
        }
        scale = *(u32 *)((u8 *)slots + 0x48);
        u32 value = (u32)(adjustedVolume * scale);
        if (value > 100000)
            value = 100000;
        if (value < 100)
            value = 100;
        (*(EqSoundBufferInterface_guess **)((u8 *)voice + 4))->SetFrequency_guess(value);
    }
}

// FUNCTION: TRILOGY 0x004ec55d
// Finds an idle voice or adds a duplicate for a sound channel.
// @early-stop: EH frame setup, loop block ordering, and allocation cleanup state still differ.
int __fastcall AcquireVoiceSlot_guess(EqDirectSoundSlots_guess *slots, int edx)
{
    int *voiceSlots;
    int *voice;
    int index;
    int result;
    /* fake match: preserve the original's stack copy of the scan index */
    volatile int scanIndexCopy;

    voiceSlots = (int *)((u8 *)slots + 4);
    index = 0;
    if (*voiceSlots != 0)
    {
        result = -1;
        scanIndexCopy = 0;
        voice = voiceSlots;
        while (*voice != 0)
        {
            if (*(char *)(*voice + 0x10) == 0
                && (EQ_METHOD(0, u32, 0x004EC004, (EqDirectSoundVoice_guess *), (EqDirectSoundVoice_guess *)*voice)() & 1) == 0)
            {
                result = index;
                break;
            }
            index++;
            voice++;
            scanIndexCopy = index;
            if (index >= 0x10)
                break;
        }

        // result stays -1 only when no idle voice was found; index is never negative, so found => result >= 0
        if (result == -1 && index != 0x10)
        {
            ((int *)slots)[index + 1] = (int)new EqDirectSoundVoiceDup_guess(
                (EqDirectSoundVoiceDup_guess *)*voiceSlots);
            result = index;
        }
    }
    else
    {
        return -1;
    }

    return result;
}

// FUNCTION: TRILOGY 0x004EC60F
// Creates a duplicate playback voice with its sound interfaces.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
// @early-stop: push edi/xor order, byte zero store (and imm vs cl), throw and vtable call operand order, epilogue mov eax
// __thiscall constructor (sole caller 0x004EC55D). Clears the voice, sets the vtable 0x00535864 and,
// given a source voice and the IDirectSound at 0x006EFE40: DuplicateSoundBuffer (vtable 0x14) of the
// source's buffer (+4) into +4, QueryInterface (IID 0x00536E98) into +8 and (IID 0x00536E88) into +0xC.
// The first two failures throw the message as a char* (throw info 0x0053A190), reusing the argument
// slot; the third only clears +0xC.
EqDirectSoundVoiceDup_guess::EqDirectSoundVoiceDup_guess(EqDirectSoundVoiceDup_guess *source)
{
    void **buffer;
    void **slot;
    void *object;
    void *sound;
    u32 *vtable;
    typedef int (__stdcall *Duplicate)(void *, void *, void **);
    typedef int (__stdcall *Query)(void *, const void *, void **);
    *((u8 *)this + 0x10) = 0;
    buffer = (void **)((u8 *)this + 4);
    slot = (void **)((u8 *)this + 8);
    *buffer = 0;
    *slot = 0;
    *(u32 *)((u8 *)this + 0x14) = 0;
    *(u32 *)((u8 *)this + 0x18) = 0;
    *(u32 *)this = 0x00535864;
    if (source != 0 && (sound = EQ_GLOBAL(void *, 0x006EFE40)) != 0)
    {
        if (((Duplicate)(*(u32 **)sound)[5])(sound, *(void **)((u8 *)source + 4), buffer) != 0)
        {
            source = (EqDirectSoundVoiceDup_guess *)0x0057E2A0;
            EQ_STDCALL(void, 0x005218D4, (void *, void *))(&source, (void *)0x0053A190);
        }
        object = *buffer;
        vtable = *(u32 **)object;
        if (((Query)vtable[0])(object, (const void *)0x00536E98, slot) != 0)
        {
            source = (EqDirectSoundVoiceDup_guess *)0x0057DF7C;
            EQ_STDCALL(void, 0x005218D4, (void *, void *))(&source, (void *)0x0053A190);
        }
        object = *buffer;
        vtable = *(u32 **)object;
        if (((Query)vtable[0])(object, (const void *)0x00536E88, (void **)((u8 *)this + 0xC)) != 0)
            *(void **)((u8 *)this + 0xC) = 0;
    }
}

// FUNCTION: TRILOGY 0x004EC6AB
// Applies the master playback level across active sound channels.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
// __thiscall, 1 arg. Stores the volume clamped to -10000 .. 0 as the master volume (DAT_006EFE3C), then
// for each slot (+4 .. +0x40) that is not null: the buffer at voice+4 gets SetVolume (vtable 0x3C, stdcall)
// with voice+0x18 + master, plus +0x60 unless the byte at +0x44 is set, clamped to -10000 .. 0.
// The argument slot is reused as the loop counter (16 slots).
void EqDirectSoundSlotsStop_guess::SetMasterVolume_guess(int volume)
{
    u8 **slot;
    u8 *voice;
    int combined;
    if (volume > 0)
        volume = 0;
    if (volume < -10000)
        volume = -10000;
    EQ_GLOBAL(int, 0x006EFE3C) = volume;
    slot = (u8 **)((u8 *)this + 4);
    volume = 16;
    do
    {
        voice = *slot;
        if (voice != 0)
        {
            combined = *(int *)(voice + 0x18) + EQ_GLOBAL(int, 0x006EFE3C);
            if (*((u8 *)this + 0x44) == 0)
                combined += *(int *)((u8 *)this + 0x60);
            if (combined > 0)
                combined = 0;
            if (combined < -10000)
                combined = -10000;
            (*(EqSoundBufferInterface_guess **)(voice + 4))->SetVolume_guess(combined);
        }
        slot++;
        volume--;
    } while (volume != 0);
}

// FUNCTION: TRILOGY 0x004ec713
// Sets one reserved sound channel's playback volume.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
void EqDirectSoundSlots_guess::SetVolume_guess(int slot, int volume)
{
    u8 *voice;
    int combined;
    if (slot >= 0)
    {
        if (slot < 16)
        {
            voice = ((u8 **)this)[slot + 1];
            if (voice != 0 && voice[0x10] != 0)
            {
                if (!(volume < 1))
                volume = 0;
                else
                    {
                    if (volume < -10000)
                        volume = -10000;
                }
                *(int *)(voice + 0x18) = volume;
                voice = ((u8 **)this)[slot + 1];
                combined = *(int *)(voice + 0x18) + EQ_GLOBAL(int, 0x006EFE3C);
                if (*((u8 *)this + 0x44) == 0)
                    combined += *(int *)((u8 *)this + 0x60);
                if (combined > 0)
                    combined = 0;
                if (combined < -10000)
                    combined = -10000;
                (*(EqSoundBufferInterface_guess **)(voice + 4))->SetVolume_guess(combined);
            }
        }
    }
}

// FUNCTION: TRILOGY 0x004ec77d
// Applies changed spatial, mode, and volume settings to a sound channel.
// @early-stop: frame matches; call setup, register and block order, and two extra instructions remain.
/* fake match: state preserves the original frame; volatile changedVolume reuses the argument slot. */
void __fastcall UpdateSlotParameters_guess(void *object, int unusedEdx, int slot, u32 parameters, int unused)
{
    struct UpdateState
    {
        u32 description[16];
        u32 changedFlags;
        int secondVolume;
        u8 *self;
        u32 firstVolume;
    } state;
    u8 *params;
    int **slotEntry;
    int *voice;
    u32 status;
    volatile u32 changedVolume;
    int volume;
    typedef int (__stdcall *GetDescription)(void *, void *);
    typedef int (__stdcall *SetDescription)(void *, void *, int);
    typedef int (__stdcall *SetProperty)(void *, const void *, u32, u32, u32, void *, u32);
    typedef int (__stdcall *SetVolume)(void *, int);

    params = (u8 *)parameters;
    state.self = (u8 *)object;
    if (params == 0)
        return;

    voice = *(int **)((u8 *)object + 4 + slot * 4);
    if (voice != 0 && *(int *)((u8 *)voice + 8) != 0)
    {
        slotEntry = (int **)((u8 *)object + 4 + slot * 4);
            status = EQ_METHOD(0, u32, 0x004EC004, (void *), voice)();
            if (status & 1)
            {
                state.firstVolume = 1;
                parameters = 0x80000000;
            }
            else
            {
                state.firstVolume = 0;
                parameters = 0;
            }

            if (*(int *)((u8 *)*slotEntry + 8) != 0)
            {
                state.description[0] = 0x40;
                ((GetDescription)(*(u32 **)*(int **)((u8 *)*slotEntry + 8))[3])(
                    *(int **)((u8 *)*slotEntry + 8), state.description);
                status = *(u32 *)(params + 0x54);
                if (status & 1)
                {
                    state.description[1] = *(u32 *)(params + 4);
                    state.description[2] = *(u32 *)(params + 8);
                    state.description[3] = *(u32 *)(params + 0xc);
                }
                if (status & 2)
                {
                    state.description[4] = *(u32 *)(params + 0x10);
                    state.description[5] = *(u32 *)(params + 0x14);
                    state.description[6] = *(u32 *)(params + 0x18);
                }
                if (status & 4)
                {
                    state.description[9] = *(u32 *)(params + 0x1c);
                    state.description[10] = *(u32 *)(params + 0x20);
                    state.description[11] = *(u32 *)(params + 0x24);
                }
                if (status & 0x80)
                    ((float *)state.description)[13] = *(float *)(params + 0x34);
                if (status & 0x100)
                    ((float *)state.description)[13] = *(float *)((u8 *)object + 0xac) * *(float *)(params + 0x38);
                if (status & 0x10000)
                {
                    status = *(u32 *)(params + 0x50);
                    if (status & 0x10)
                        state.description[15] = 2;
                    if (status & 0x40)
                        state.description[15] = 1;
                    if (status & 0x20)
                        state.description[15] = 0;
                    *(u32 *)((u8 *)*slotEntry + 0x14) = status;
                }
                ((SetDescription)(*(u32 **)*(int **)((u8 *)*slotEntry + 8))[12])(
                    *(int **)((u8 *)*slotEntry + 8), state.description, state.firstVolume);
            }

            if ((*(u8 *)(params + 0x54) & 0x40) != 0)
                EQ_METHOD(2, void, 0x004EC713, (void *, int, int), object)(slot, *(int *)(params + 0x48));
            if ((*(u8 *)(params + 0x55) & 4) != 0)
                EQ_METHOD(2, void, 0x004EC4E8, (void *, int, float), object)(slot, *(float *)(params + 0x4c));

            status = *(u32 *)(params + 0x54);
            changedVolume = status;
            changedVolume &= 0x10;
            state.changedFlags = status;
            if (changedVolume != 0 || (state.changedFlags & 0x20) != 0)
            {
                state.firstVolume = *(u32 *)(params + 0x40);
                state.secondVolume = *(int *)(params + 0x44);
                if (*(char *)(state.self + 0x44) != 0 && (*(u8 *)(params + 0x50) & 0x10) == 0)
                {
                    if (changedVolume != 0)
                    {
                        ((SetProperty)(*(u32 **)*(int **)((u8 *)*slotEntry + 0xc))[4])(
                            *(int **)((u8 *)*slotEntry + 0xc), (void *)0x005359c0,
                            parameters | 7, 0, 0, &state.firstVolume, 4);
                    }
                    if ((*(u8 *)(params + 0x54) & 0x20) != 0)
                    {
                        ((SetProperty)(*(u32 **)*(int **)((u8 *)*slotEntry + 0xc))[4])(
                            *(int **)((u8 *)*slotEntry + 0xc), (void *)0x005359c0,
                            parameters | 9, 0, 0, &state.secondVolume, 4);
                    }
                }
                else
                {
                    u32 volumeChanges = 0;
                    if (changedVolume != 0)
                        volumeChanges = state.firstVolume;
                    if ((state.changedFlags & 0x20) != 0)
                        volumeChanges += state.secondVolume;
                    volume = *(int *)((u8 *)*slotEntry + 0x18);
                    volume += EQ_GLOBAL(int, 0x006EFE3C);
                    volume += volumeChanges;
                    if (*(char *)(state.self + 0x44) == 0)
                        volume += *(int *)(state.self + 0x60);
                    if (volume > 0)
                        volume = 0;
                    if (volume < -10000)
                        volume = -10000;
                    ((SetVolume)(*(u32 **)*(int **)((u8 *)*slotEntry + 4))[15])(
                        *(int **)((u8 *)*slotEntry + 4), volume);
                }
            }
    }
}

// FUNCTION: TRILOGY 0x004ec986
// Updates a channel's EAX effect and environmental sound attributes.
// @early-stop: virtual-call argument order and x87 zero-store scheduling still differ.
/* fake match: volatile slot reads retain the original reloads; slotObject reuses the first slot load. */
void EqDirectSoundSlotsStop_guess::UpdateSlotEffectAttributes_guess(u32 slot)
{
    u32 * volatile *slotEntry;
    u32 *slotObject;
    u32 * volatile *slotEffect;
    u32 *effect;
    int result;
    float effectData[16];
    typedef int (__stdcall *GetEffectDescription)(void *, void *);
    typedef int (__stdcall *SetEffectDescription)(void *, void *, int);
    typedef int (__stdcall *GetEffectParameters)(void *, const void *, int, u32 *);
    typedef int (__stdcall *SetEffectParameters)(void *, const void *, int, int, int, void *, int);
    typedef void (__stdcall *OutputDebugText)(const char *);

    slotEntry = (u32 **)((u8 *)this + 4 + slot * 4);
    slotObject = *slotEntry;
    if (*(u32 *)((u8 *)slotObject + 8) != 0)
    {
        *(u32 *)&effectData[0] = 0x40;
        effect = *(u32 **)((u8 *)slotObject + 8);
        ((GetEffectDescription)(*(u32 **)effect)[3])(effect, effectData);
        effectData[1] = 0.0f;
        effectData[2] = 0.0f;
        effectData[3] = 0.0f;
        effectData[4] = 0.0f;
        effectData[5] = 0.0f;
        effectData[6] = 0.0f;
        *(u32 *)&effectData[7] = *(u32 *)((u8 *)this + 0x94);
        *(u32 *)&effectData[8] = *(u32 *)((u8 *)this + 0x98);
        *(u32 *)&effectData[9] = *(u32 *)((u8 *)this + 0xa0);
        *(u32 *)&effectData[10] = *(u32 *)((u8 *)this + 0xa4);
        *(u32 *)&effectData[11] = *(u32 *)((u8 *)this + 0xa8);
        *(u32 *)&effectData[12] = *(u32 *)((u8 *)this + 0x9c);
        *(u32 *)&effectData[13] = *(u32 *)((u8 *)this + 0xac);
        effect = *(u32 **)((u8 *)*slotEntry + 8);
        *(u32 *)&effectData[14] = 0x4e6e6b28;
        *(u32 *)&effectData[15] = 0;
        ((SetEffectDescription)(*(u32 **)effect)[12])(effect, effectData, 0);
    }
    *(u32 *)((u8 *)this + 0x5c) = *(u32 *)((u8 *)this + 0xb8);
    slotObject = *slotEntry;
    slotEffect = (u32 * volatile *)((u8 *)slotObject + 0xc);
    if (*slotEffect != 0 && *((u8 *)this + 0x44) != 0)
    {
        effect = *slotEffect;
        result = ((GetEffectParameters)(*(u32 **)effect)[5])(
            effect, &EQ_GLOBAL(u8, 0x005359C0), 1, &slot);
        if (result == 0 && (slot & 2) != 0)
        {
            effect = *(u32 **)((u8 *)*slotEntry + 0xc);
            result = ((SetEffectParameters)(*(u32 **)effect)[4])(
                effect, &EQ_GLOBAL(u8, 0x005359C0), 1, 0, 0,
                (u8 *)this + 0x60, 0x34);
            if (result != 0)
                EQ_GLOBAL(OutputDebugText, 0x00534040)(
                    &EQ_GLOBAL(char, 0x0057E2C4));
        }
    }
}

// FUNCTION: TRILOGY 0x004ECA93
// Counts playing voices that match a supplied identifier.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
// __thiscall. Counts the voices whose status has bit 1 (playing) and whose id at 0x14 is `id`; stops at
// the first empty slot.
int EqDirectSoundSlotsStop_guess::CountPlayingWithId_guess(int id)
{
    int i;
    u16 count = 0;

    for (i = 0; i < 16; i++)
    {
        if (((u8 **)this)[i + 1] == 0)
            return count;
        if ((EQ_METHOD(0, u32, 0x004EC004, (void *), ((u8 **)this)[i + 1])() & 1) &&
            *(int *)(((u8 **)this)[i + 1] + 0x14) == id)
            count++;
    }
    return count;
}

// FUNCTION: TRILOGY 0x004ecac9
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
// No index guard in the original. Only AL is the result.
bool EqDirectSoundSlots_guess::CommandSlot_guess(int slot, u32 value)
{

    typedef int (__stdcall *Command)(void *, u32);
    void *buffer = *(void **)(((u8 **)this)[slot + 1] + 8);
    return buffer == 0 ? false : (bool)(unsigned char)(((Command)(*(u32 **)buffer)[3])(buffer, value) == 0);
}

// FUNCTION: TRILOGY 0x004ECAEF
// Gets playback status for one reserved sound key.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (address neighbourhood; Mac object not directly mapped)
// __thiscall. Stores the slot voice's status and returns true; for an unreserved slot it logs the
// owner name at 0x54 and returns false. Only AL is the result.
bool EqDirectSoundSlotsStop_guess::GetSlotStatus_guess(int slot, u32 *status)
{
    u8 *voice;

    int cond = slot < 0 || slot >= 16 || (voice = ((u8 **)this)[slot + 1]) == 0 || voice[0x10] == 0;
    if (cond)
    {
        char message[200];
        EQ_CDECL(int, 0x005224FB, (char *, const char *, const char *, int))
            (message, EQ_ARRAY(char, 0x0057E300), *(const char **)((u8 *)this + 0x54), slot);
        OutputDebugStringA(message);
        return false;
    }
    *status = EQ_METHOD(0, u32, 0x004EC004, (void *), voice)();
    return true;
}

// FUNCTION: TRILOGY 0x004ecb82
// ORIGINAL FILE: Source/PC Source/EverQuest/SoundManager.cpp (address range)
// __thiscall. Like SetVolume, for the value at 0x44 (flag 0x20): false outside -10000 .. 0.
bool EQSoundBuffer_guess::SetVolume2_guess(int volume)
{
    if (0 == (volume >= -10000 && volume <= 0))
    {
    }
    else
    {
        changed_flags |= 0x20;
        volume2 = volume;
        return true;
    }
    return false;
}

// FUNCTION: TRILOGY 0x004ecbc2
// Stores playback mode flags on a sound buffer.
// ORIGINAL FILE: Source/PC Source/EverQuest/SoundManager.cpp (address range)
// __thiscall. The mode word at 0x50 from the flag byte: bit 0 -> 1, then the first of 0x10, 0x40, 0x20
// that is set; marks it changed (byte 0x56 bit 0).
// @early-stop: AL/DL swapped: the original keeps the constant 1 in EAX and the flag byte in DL.
void EQSoundBuffer_guess::SetMode_guess(u8 mode)
{
    mode_guess = 0;
    if (mode & 1)
        mode_guess = 1;
    if (mode & 0x10)
        mode_guess |= 0x10;
    else if (mode & 0x40)
        mode_guess |= 0x40;
    else if (mode & 0x20)
        mode_guess |= 0x20;
    changed_flags |= 0x10000;
}

// The sound table whose method 0x004EDDB6 sits in the vtable at 0x005358A0 (entry 0x005358B8): 0x2D0
// voice slot objects at 0x0C. Only the virtuals it calls are named. Local: not yet in include/.
class SoundSlotTable_guess
{
public:
    virtual void Virtual00_guess() = 0;
    virtual void Virtual04_guess() = 0;
    virtual void Virtual08_guess() = 0;
    virtual void Virtual0C_guess() = 0;
    virtual void Virtual10_guess() = 0;
    virtual void Virtual14_guess() = 0;
    virtual int Acquire_guess(int index) = 0;                               // 0x18
    virtual void Release_guess(int *slot) = 0;                              // 0x1c
    virtual void Virtual20_guess() = 0;
    virtual int Play_guess(int *slot, EQSoundBuffer_guess *buffer) = 0;     // 0x24
    int PlayBuffer_guess(int index, EQSoundBuffer_guess *buffer);
    int DispatchHandleToSlotWithArgs_guess(u32 *handle, int calleeArgument1, int calleeArgument2);

    u32 unknown0x04;                                // 0x04
    u32 unknown0x08;                                // 0x08
    EqDirectSoundSlotsStop_guess *slots[0x2d0];     // 0x0c
};

// Local view of the color-scheme data: the binary reads three floats at 0x08, 0x0c and 0x10.
// The owning type is not declared in the shared headers.
struct ColorScheme_CopyColorSchemeDataObject_guess
{
    u32 unknown0x00;                         // 0x00
    u32 unknown0x04;                         // 0x04
    float color0x08_guess;                   // 0x08
    float color0x0c_guess;                   // 0x0c
    float color0x10_guess;                   // 0x10

    float GetScaledMaxComponentDelta_guess(float *param_2);
};

// FUNCTION: TRILOGY 0x004ee795
// Returns a scaled max difference across three values; its game meaning is unclear.
float ColorScheme_CopyColorSchemeDataObject_guess::GetScaledMaxComponentDelta_guess(float *inputComponents)
{
    float maxComponentDelta;
    float secondComponentDelta;
    float thirdComponentDelta;
    float secondComponentMagnitude;

    maxComponentDelta = inputComponents[0] - color0x08_guess;
    secondComponentDelta = inputComponents[1] - color0x0c_guess;
    secondComponentMagnitude = secondComponentDelta;
    thirdComponentDelta = inputComponents[2] - color0x10_guess;
    if (maxComponentDelta < EQ_GLOBAL(float, 0x0053510C))
        maxComponentDelta = -maxComponentDelta;
    if (secondComponentDelta < EQ_GLOBAL(float, 0x0053510C))
    {
        secondComponentDelta = -secondComponentDelta;
        secondComponentMagnitude = secondComponentDelta;
    }
    if (thirdComponentDelta < EQ_GLOBAL(float, 0x0053510C))
        thirdComponentDelta = -thirdComponentDelta;
    maxComponentDelta = secondComponentDelta > maxComponentDelta ? secondComponentMagnitude : maxComponentDelta;
    if (thirdComponentDelta > maxComponentDelta)
        maxComponentDelta = thirdComponentDelta;
    return maxComponentDelta * EQ_GLOBAL(float, 0x0053596C);
}

// This object's coordinates are read at 0x08, 0x0c, and 0x10. No matching
// object layout is declared in the headers yet.
class SoundPositionBounds_guess
{
public:
    u32 unknown0x00;             // 0x00
    u32 unknown0x04;             // 0x04
    float x_guess;               // 0x08
    float y_guess;               // 0x0c
    float z_guess;               // 0x10

    bool TestRange_guess(float *position, float range, float *maxDistance);
};

// FUNCTION: TRILOGY 0x004ee813
// Checks whether a 3D point is in range and returns a scaled distance estimate.
/* fake match: dz max is folded into the product as a ternary; an `if (dz > dx) dx = dz;` keeps dx live on the x87 stack. */
// @early-stop: the dz sign test pops its cached constant after sahf (fnstsw; sahf; fstp st0); the original pops before sahf.
bool SoundPositionBounds_guess::TestRange_guess(float *position, float range, float *maxDistance)
{
    float dx;
    float dy;
    float dz;
    float distanceSquared;

    dx = position[0] - x_guess;
    dy = position[1] - y_guess;
    dz = position[2] - z_guess;
    distanceSquared = dx * dx + dy * dy + dz * dz;
    if (dx < EQ_GLOBAL(float, 0x0053510C))
        dx = -dx;
    if (dy < EQ_GLOBAL(float, 0x0053510C))
        dy = -dy;
    if (dz < EQ_GLOBAL(float, 0x0053510C))
        dz = -dz;
    if (dy > dx)
        dx = dy;
    *maxDistance = (dz > dx ? dz : dx) * EQ_GLOBAL(float, 0x0053596C);
    range = range * range;
    if (range < distanceSquared)
        return false;
    return true;
}

struct Position_guess
{
    u32 x;
    u32 y;
    u32 z;
};

// Queue object. Layout from SoundQueueObject_guess::AddEntryIfAbsent_guess and SoundQueueObject_guess::FUN_004ee62a_guess; not in the headers yet.
class SoundQueueObject_guess
{
public:
    u8 unknown0x00_guess[4];       // 0x00
    int records_guess;             // 0x04
    Position_guess position_guess; // 0x08
    int playingCount_guess;        // 0x14
    u32 unknown0x18_guess;         // 0x18
    u32 unknown0x1c_guess;         // 0x1c
    u32 bestEntryId_guess;         // 0x20
    u32 unknown0x24_guess;         // 0x24
    u8 unknown0x28_guess[4];       // 0x28
    u8 inner0x2c_guess[4];         // 0x2c
    u32 *unknown0x30_guess;        // 0x30

    int AddEntryIfAbsent_guess(u32 *param_2);
    void cleanupCollectionResources_guess(u32 *param_2);
    u32 FUN_004ee62a_guess(u32 *node);
};

// FUNCTION: TRILOGY 0x004ee589
// Adds a previously absent key to the queue's tracking set.
// @early-stop: VC6 copies this before iterator and pushes iterator after the LEAs; the two EQ_THISCALL EDX=0 slots add xor edx,edx; keyValue lands at -4 and the new pointer at -8 (original -8/-4), with keyValue stored eagerly, not sunk past its address push.
int SoundQueueObject_guess::AddEntryIfAbsent_guess(u32 *iterator)
{
    u32 *key;
    u8 result[8];
    u32 keyValue;
    /* fake match: the original stores the new pointer to a frame slot it never reads back */
    void * volatile allocatedEntry;

    key = iterator;
    EQ_METHOD(2, void, 0x004EEAB7, (void *, u32 **, u32 *), inner0x2c_guess)(&iterator, key);
    if (iterator == unknown0x30_guess)
    {
        void *newObject = EQ_CDECL(void *, 0x005218C6, (u32))(0x24);

        *(u32 *)((u8 *)newObject + 0x20) &= 0;
        keyValue = *key;
        allocatedEntry = newObject;
        EQ_METHOD(2, void, 0x004EE91D, (void *, u8 *, u32 *), inner0x2c_guess)(result, &keyValue);
        unknown0x24_guess = *key;
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004ee5e4
// Removes a keyed queue entry and releases its attached resource.
void SoundQueueObject_guess::cleanupCollectionResources_guess(u32 *key)
{
    int entry;

    EQ_METHOD(2, void, 0x004EEAB7, (void *, int *, u32 *), (char *)this + 0x2c)(&entry, key);
    if (entry != *(int *)((char *)this + 0x30))
    {
        if (*(int *)(entry + 0x10) != 0)
            EQ_CDECL(void, 0x005218BB, (void *))((void *)*(int *)(entry + 0x10));
        *(int *)(entry + 0x10) = 0;
        EQ_METHOD(1, void, 0x004EEA52, (void *, u32 *), (char *)this + 0x2c)(key);
    }
    return;
}

// FUNCTION: TRILOGY 0x004ee62a
// Ranks queued sound entries for playback using priority and distance.
// @early-stop: VC6 register scheduling, cleanup block placement, and several operand/call setup instructions still differ.
u32 SoundQueueObject_guess::FUN_004ee62a_guess(u32 *node)
{
    struct CommandSlotResult_guess
    {
        u32 unknown0x00;
        float position[3];
        u8 unknown0x10[36];
        float range;
        u32 unknown0x38;
        int mode;
    };
    u32 entryId = 0xffffffff;
    u32 *listHead;
    char ok;
    int distanceTerm;
    int recordOffset;
    int entry;
    CommandSlotResult_guess command;
    unsigned __int64 basePriority;
    int savedEntry;
    float distanceRatio;
    int bestPriority;
    float distanceRatioSq;
    u32 slot;
    u32 status;

    // node is the position on entry, then the list cursor
    position_guess = *(Position_guess *)node;
    bestEntryId_guess = 0xffffffff; // -1 if no entry scores above 0
    unknown0x1c_guess = 0;
    unknown0x18_guess = 0;
    playingCount_guess = 0;
    listHead = unknown0x30_guess;
    node = (u32 *)*listHead;
    bestPriority = 0;
    if (node != listHead)
    {
        do
        {
            entryId = node[3];
            // 0xfc bytes per record, indexed by the id's high 16 bits
            recordOffset = (entryId >> 0x10) * 0xfc;
            slot = (u16)entryId;
            if (records_guess + recordOffset == 0)
            {
                EQ_THISCALL(void, 0x004EE5E4, (void *, int, u32 *))
                    (this, 0, &entryId);
            }
            else
            {
                status = 0;
                ok = EQ_STDCALL(char, 0x004ECAEF, (u32, u32 *))
                    (slot, &status);
                if (ok != 0)
                {
                    if ((status & 1) == 0)
                    {
                        if (entryId != unknown0x24_guess)
                        {
                            EQ_THISCALL(void, 0x004EE5E4, (void *, int, u32 *))
                                (this, 0, &entryId);
                            continue;
                        }
                    }
                    else
                    {
                        playingCount_guess = playingCount_guess + 1;
                    }
                }
                entry = node[4];
                savedEntry = entry;
                ok = EQ_THISCALL(char, 0x004ECAC9, (void *, int, u32, u8 *))
                    ((void *)(records_guess + recordOffset), 0, slot,
                    (u8 *)&command);
                if (ok != 0)
                {
                    if (command.mode != 0)
                    {
                        basePriority = (u32)(*(int *)(recordOffset + 0x5c + records_guess) + slot);
                        *(float *)(entry + 0x14) = (float)basePriority;
                    }
                    else
                    {
                        // max-axis distance out; replaced below
                        ok = EQ_THISCALL(char, 0x004EE813, (void *, int, float *, float, float *))
                            (this, 0, command.position, command.range, &distanceRatio);
                        if (ok != 0)
                        {
                            distanceRatio = EQ_THISCALL(float, 0x004EE795, (void *, int, float *))
                                (this, 0, command.position) / command.range;
                            distanceRatioSq = distanceRatio * distanceRatio;
                        }
                        else
                        {
                            distanceRatioSq = 0.0f;
                        }
                    }
                    // entry holds the record base for this read
                    entry = records_guess;
                    // stale if command.mode != 0 (read anyway)
                    distanceTerm = (int)distanceRatioSq;
                    *(u32 *)(savedEntry + 0x1c) = *(int *)(recordOffset + 0x5c + entry) + distanceTerm + slot;
                    entry = savedEntry;
                }
                else
                {
                    *(u32 *)(entry + 0x1c) = *(int *)(recordOffset + 0x5c + records_guess) + slot;
                }
                if (bestPriority < *(int *)(entry + 0x1c))
                {
                    bestEntryId_guess = node[3];
                    bestPriority = *(int *)(entry + 0x1c);
                }
            }
        } while ((EQ_THISCALL(void, 0x004EEF62, (u32 **, int))(&node, 0),
            listHead = unknown0x30_guess, node != listHead));
    }
    return *(u32 *)((char *)this + 0x38);
}

// FUNCTION: TRILOGY 0x004ecd97
// Runs sound asset cleanup when the client releases an asset.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (from Mac debug map)
SoundAsset *SoundAsset::ScalarDeletingDtor_guess(u8 flags)
{
    EQ_METHOD(0, void, 0x004ECDB3, (void *), this)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

// The EH frame object of the destructor below, built from this so the frame keeps a copy of it in its own slot.
class SoundAssetDtorGuard_guess
{
public:
    SoundAsset *owner;
    SoundAssetDtorGuard_guess(SoundAsset *object) { owner = object; }
    ~SoundAssetDtorGuard_guess(void) {}
};

// FUNCTION: TRILOGY 0x004ecdb3
// Shuts down the sound engine, stops its worker thread, and releases its audio resources.
// ORIGINAL FILE: EverQuest/SoundManager.cpp (from Mac debug map)
// @early-stop: the original keeps 0 in EBX and the first fopen string in EBP; this build has them swapped, so the register choice differs through the rest of the body
/* fake match: the guard object holds this to give the frame the original's slot; the volatile reference keeps the vtable store first. */
SoundAsset::~SoundAsset(void)
{
    SoundAssetDtorGuard_guess ehMarker(this);
    int *self = (int *)this;
    int *object;
    int *slot;
    int i;

    /* fake match: the volatile reference keeps the vtable store ahead of the constant loads for the first fopen, as in the original. */
    volatile int &vtable = self[0];
    vtable = 0x00535898;
    EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3C0));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }

    EQ_GLOBAL(int, 0x006EFE48) = 0;
    if (self[2] != 0)
        EQ_GLOBAL(SoundAssetTerminateThread_guess, 0x005340D8)((u32)self[2], 0);

    EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3AC));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }

    object = (int *)self[0x2D3];
    if (object != 0)
        ((SoundAssetRelease_guess *)object)->Release_guess(1);
    self[0x2D3] = 0;

    slot = self + 3;
    i = 0x2D0;
    do
    {
        object = (int *)*slot;
        if (object != 0)
            ((SoundAssetRelease_guess *)object)->Release_guess(1);
        *slot = 0;
        slot++;
        i--;
    } while (i != 0);

    object = (int *)self[0x2D4];
    if (object != 0)
    {
        ((SoundAssetVtableTwoArgs_guess)(*(u32 *)(*(u32 *)object + 0x14)))(object, 0);
        object = (int *)self[0x2D4];
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);
        self[0x2D4] = 0;
    }

    if ((u32)self[0x2D5] != 0)
        EQ_GLOBAL(SoundAssetFreeLibrary_guess, 0x0053403C)((u32)self[0x2D5]);

    object = (int *)self[0x2D8];
    self[0x2D5] = 0;
    if (object != 0)
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);

    object = (int *)self[0x2F1];
    self[0x2D8] = 0;
    if (object != 0)
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);

    object = (int *)self[0x2D7];
    self[0x2F1] = 0;
    if (object != 0)
    {
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x48)))(object);
        object = (int *)self[0x2D7];
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);
    }

    object = (int *)self[0x2D6];
    self[0x2D7] = 0;
    if (object != 0)
        ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);

    self[0x2D6] = 0;
    EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E390));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }
    self[0] = 0x00535900;
}

// Vtable slot 0x58 of SoundAsset: thiscall with one out pointer. The result is not used.
typedef void (__fastcall *SoundAssetVirtualGet_guess)(void *self, int edx_unused, int *out);

// FUNCTION: TRILOGY 0x004ECF6B
// Initializes the sound engine and prepares its initial audio resources.
// SoundAsset vtable slot 1 (0x0053589C in the table at 0x00535898), a __thiscall written as a __fastcall free function
// like InitDirectSound_guess. Logs to AudioDbg.txt, runs InitDirectSound_guess, then reads a count through vtable slot
// 0x58 (self[0x2F3] = 4 when it is 4 or less) and creates the 0x3C-byte object that InitializeObject_guess initialises.
// It returns true with EAX = (self[0x2D8] & ~0xFF) | 1. When InitDirectSound_guess fails there is no return, as in
// the original: EAX keeps that call's value.
// @early-stop: VC6 orders the first fopen's cleanup and global store differently, the EH state write comes before the value load, and the new pointer store to arg1 is dead-eliminated.
bool __fastcall SoundAssetInit_guess(int *self, int edx_unused, int *arg1, int arg2)
{
    int local[2];                       /* fake match: local[1] keeps the original's second frame dword; local[0] is the vtable slot 0x58 out value */
    int result;

    EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3F4));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }

    SoundAssetEHMarker_guess ehMarker;  // EH frame stand-in for the original's operator new + ctor (no cleanup)
    int value = *(volatile int *)arg1;  /* fake match: the volatile read keeps the load of *arg1 ahead of the push of arg2 */
    self[0x2F5] = value;
    if (EQ_THISCALL(bool, 0x004ED085, (void *, int, int, int))(self, 0, value, arg2))
    {
        ((SoundAssetVirtualGet_guess)(*(u32 *)(*(u32 *)self + 0x58)))(self, 0, &local[0]);
        if (local[0] <= 4)
            self[0x2F3] = 4;

        arg1 = (int *)EQ_CDECL(void *, 0x005218C6, (u32))(0x3c);
        if (arg1 != 0)
            result = (int)EQ_THISCALL(void *, 0x004EE498, (void *, int, u32, u32))(arg1, 0, self[3], 0);
        else
            result = 0;
        self[0x2D3] = result;

        EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3E8));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }

        EQ_GLOBAL(int, 0x006EFEA8) = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        if (EQ_GLOBAL(int, 0x006EFEA8) != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3D8));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }

        EQ_GLOBAL(int, 0x006EFE48) = self[0x2D8];
        return true;
    }
}

// The DSBUFFERDESC and WAVEFORMATEX the function builds on its stack. Local: the headers have neither struct
// and this file does not include dsound.h. WAVEFORMATEX is packed to the 0x12 bytes SetFormat reads.
#pragma pack(push, 2)
struct SoundInitWaveFormat_guess
{
    u16 wFormatTag;             // 0x00
    u16 nChannels;              // 0x02
    u32 nSamplesPerSec;         // 0x04
    u32 nAvgBytesPerSec;        // 0x08
    u16 nBlockAlign;            // 0x0C
    u16 wBitsPerSample;         // 0x0E
    u16 cbSize;                 // 0x10
};
#pragma pack(pop)
C_ASSERT(sizeof(SoundInitWaveFormat_guess) == 0x12);

struct SoundInitBufferDesc_guess
{
    u32 dwSize;                 // 0x00
    u32 dwFlags;                // 0x04
    u32 dwBufferBytes;          // 0x08
    u32 dwReserved;             // 0x0C
    void *lpwfxFormat;          // 0x10
    u8 guid3DAlgorithm[16];     // 0x14
};
C_ASSERT(sizeof(SoundInitBufferDesc_guess) == 0x24);

// Calls made through vtable slots and imported pointers. COM methods are __stdcall with `this` first and the
// pointer arguments passed as ints. The fastcall type is a vtable method of self (EDX dummy).
typedef int (__stdcall *SoundInitCall2_guess)(int *object, int a);
typedef int (__stdcall *SoundInitCall3_guess)(int *object, int a, int b);
typedef int (__stdcall *SoundInitCall4_guess)(int *object, int a, int b, int c);
typedef int (__stdcall *SoundInitCall7_guess)(int *object, int a, int b, int c, int d, int e, int f);
typedef int (__stdcall *SoundInitSetDistance_guess)(int *object, float distance, int apply);
typedef u8 (__fastcall *SoundInitThisCall_guess)(void *self, int edx, const char *name, int flag);
typedef int (__stdcall *SoundInitCoInitialize_guess)(void *reserved);
typedef int (__stdcall *SoundInitCoCreateInstance_guess)(void *clsid, void *outer, u32 context, void *iid, void *ppv);
typedef u32 (__stdcall *SoundInitLoadLibraryA_guess)(const char *name);
typedef void *(__stdcall *SoundInitGetProcAddress_guess)(u32 module, const char *name);
typedef void (__stdcall *SoundInitEaxManagerCreate_guess)(int *manager);

// FUNCTION: TRILOGY 0x004ED085
// Starts 3D sound: opens DirectSound, sets its primary buffer and listener, then probes extension support.
// DirectSound bring-up for the 3D sound path. Creates IDirectSound when self[0x2D6] is empty, sets the cooperative
// level, the primary buffer and listener, a 22050 Hz mono secondary buffer with its 3D interface, the voice-manager
// flags and the EAX manager. Logs each step to AudioDbg.txt. Returns 1 when the secondary is up.
// @early-stop: VC6 still differs in block order, logging stack cleanup, and some branch joins.
/* fake match: model the EAX vtable slot at 0x48 and retain the observed shared cleanup joins. */
bool __fastcall InitDirectSound_guess(int *self, int edx_unused, int **hwnd, int support)
{
    typedef int (__stdcall *SoundInitCoopLevel_guess)(int *, void *, int);
    typedef int (__stdcall *SoundInitCreateBuffer_guess)(int *, SoundInitBufferDesc_guess *, int **, int);
    typedef int (__stdcall *SoundInitSetFormat_guess)(int *, SoundInitWaveFormat_guess *);
    class SoundInitEaxManagerInterface_guess
    {
    public:
        virtual void Slot00_guess(void) = 0;
        virtual void Slot01_guess(void) = 0;
        virtual void Slot02_guess(void) = 0;
        virtual void Slot03_guess(void) = 0;
        virtual void Slot04_guess(void) = 0;
        virtual void Slot05_guess(void) = 0;
        virtual void Slot06_guess(void) = 0;
        virtual void Slot07_guess(void) = 0;
        virtual void Slot08_guess(void) = 0;
        virtual void Slot09_guess(void) = 0;
        virtual void Slot0A_guess(void) = 0;
        virtual void Slot0B_guess(void) = 0;
        virtual void Slot0C_guess(void) = 0;
        virtual void Slot0D_guess(void) = 0;
        virtual void Slot0E_guess(void) = 0;
        virtual void Slot0F_guess(void) = 0;
        virtual void Slot10_guess(void) = 0;
        virtual void Slot11_guess(void) = 0;
        virtual u8 Configure_guess(const char *name, int flag) = 0;
    };
    int *secondary;                     // local_14
    int *buffer3d;                      // local_10
    int **dsSlot;                       // local_c: &self[0x2D6] (IDirectSound)
    int *object;                        // piVar5
    int hr;                             // HVar3
    int result;                         // iVar4
    int logFile;
    char *message;                      // pcVar7
    u32 module;                         // hModule
    void *proc;                         // pFVar6
    u8 eaxFlag;                         // uVar2
    SoundInitBufferDesc_guess desc;     // local_4c
    SoundInitWaveFormat_guess wfx;      // local_28

    secondary = 0;
    buffer3d = 0;
    if (hwnd == 0)
        return 0;
    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
    if (logFile != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (logFile, EQ_ARRAY(char, 0x0057E634));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }
    hr = EQ_GLOBAL(SoundInitCoInitialize_guess, 0x00534370)(0);
    if (hr < 0)
        return 0;
    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
    if (logFile != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (logFile, EQ_ARRAY(char, 0x0057E62C));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }

    dsSlot = (int **)(self + 0x2D6);
    if (*dsSlot == 0)
    {
        if (support == 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E620));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            hr = EQ_GLOBAL(SoundInitCoCreateInstance_guess, 0x00534374)
                (EQ_ARRAY(void, 0x00536EC8), 0, 1, EQ_ARRAY(void, 0x00536EB8), dsSlot);
            if (hr < 0)
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile != 0)
                {
                    EQ_CDECL(int, 0x00524138, (int, char *, ...))
                        (logFile, EQ_ARRAY(char, 0x0057E610));
                    EQ_CDECL(int, 0x00521FBB, (int))
                        (EQ_GLOBAL(int, 0x006EFEA8));
                }
                *dsSlot = 0;
                return 0;
            }
        }
        else
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E604));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            hr = EQ_GLOBAL(SoundInitCoCreateInstance_guess, 0x00534374)
                (EQ_ARRAY(void, 0x00536EC8), 0, 1, EQ_ARRAY(void, 0x00536EB8), dsSlot);
            if (hr < 0)
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile != 0)
                {
                    EQ_CDECL(int, 0x00524138, (int, char *, ...))
                        (logFile, EQ_ARRAY(char, 0x0057E5F0));
                    EQ_CDECL(int, 0x00521FBB, (int))
                        (EQ_GLOBAL(int, 0x006EFEA8));
                }
                *dsSlot = 0;
                return 0;
            }
        }
        result = ((SoundInitCall2_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x28)))(*dsSlot, 0);
        if (result < 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                message = EQ_ARRAY(char, 0x0057E5E0);
                goto logFailure;
            }
            goto releaseDirectSound;
        }
    }
getCaps:
    EQ_CDECL(void *, 0x00521960, (void *, int, int))
        (self + 0x2D9, 0, 0x60);
    object = *dsSlot;
    self[0x2D9] = 0x60;
    result = ((SoundInitCall2_guess)(*(u32 *)(*(u32 *)object + 0x10)))(object, (int)(self + 0x2D9));
    if (result != 0)
    {
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (logFile, EQ_ARRAY(char, 0x0057E5CC));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
        if (buffer3d != 0)
            ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)buffer3d + 0x08)))(buffer3d);
        buffer3d = 0;
        if (secondary != 0)
            ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)secondary + 0x08)))(secondary);
        object = (int *)self[0x2D7];
        secondary = 0;
        if (object != 0)
            ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);
        self[0x2D7] = 0;
        object = *dsSlot;
        if (object == 0)
            goto clearDsSlot;
        goto releaseObject;
    }
    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
    if (logFile != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (logFile, EQ_ARRAY(char, 0x0057E5C0));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }
    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
    if (logFile != 0)
    {
        EQ_CDECL(int, 0x00524138, (int, char *, ...))
            (logFile, EQ_ARRAY(char, 0x0057E5B8));
        EQ_CDECL(int, 0x00521FBB, (int))
            (EQ_GLOBAL(int, 0x006EFEA8));
    }
    if ((*(u8 *)(self + 0x2DA) & 0x20) == 0)
    {
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (logFile, EQ_ARRAY(char, 0x0057E598));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
        result = ((SoundInitCoopLevel_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x18)))(*dsSlot, hwnd, 2);
        if (result != 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                message = EQ_ARRAY(char, 0x0057E590);
                goto logCoopMessage;
            }
        }
    }
    else
    {
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            message = EQ_ARRAY(char, 0x0057E5A4);
logCoopMessage:
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (EQ_GLOBAL(int, 0x006EFEA8), message);
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
setCoopLevel1:
        result = ((SoundInitCoopLevel_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x18)))(*dsSlot, hwnd, 1);
        if (result != 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                message = EQ_ARRAY(char, 0x0057E588);
logFailure:
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (EQ_GLOBAL(int, 0x006EFEA8), message);
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            goto releaseDirectSound;
        }
    }

    hwnd = (int **)(self + 0x2D7);
    if (*hwnd == 0)
    {
        EQ_CDECL(void *, 0x00521960, (void *, int, int))
            (&desc, 0, 0x24);
        desc.dwSize = 0x24;
        desc.dwFlags = 0x91;
        desc.lpwfxFormat = 0;
        desc.dwReserved = 0;
        desc.dwBufferBytes = 0;
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (logFile, EQ_ARRAY(char, 0x0057E57C));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
        result = ((SoundInitCreateBuffer_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x0C)))(*dsSlot, &desc, hwnd, 0);
        if (result != 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E55C));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            goto releasePrimary;
        }
        EQ_CDECL(void *, 0x00521960, (void *, int, int))
            (&wfx, 0, 0x12);
        object = *hwnd;
        wfx.wFormatTag = 1;
        wfx.nSamplesPerSec = 0x5622;
        wfx.wBitsPerSample = 0x10;
        wfx.cbSize = 0;
        wfx.nChannels = 2;
        wfx.nBlockAlign = 4;
        wfx.nAvgBytesPerSec = 0x15888;
        result = ((SoundInitSetFormat_guess)(*(u32 *)(*(u32 *)object + 0x38)))(object, &wfx);
        if (result != 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E544));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
        }
    }
queryListener:
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (logFile, EQ_ARRAY(char, 0x0057E538));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
        result = ((SoundInitCall3_guess)(*(u32 *)(*(u32 *)*hwnd + 0x00)))(*hwnd, (int)EQ_ARRAY(void, 0x00536EA8), (int)(self + 0x2D8));
        if (result >= 0)
        {
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E504));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            object = (int *)self[0x2D8];
            result = ((SoundInitSetDistance_guess)(*(u32 *)(*(u32 *)object + 0x2C)))(object, (float)EQ_GLOBAL(double, 0x00535880), 0);
            if (result != 0)
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile != 0)
                {
                    EQ_CDECL(int, 0x00524138, (int, char *, ...))
                        (logFile, EQ_ARRAY(char, 0x0057E4E4));
                    EQ_CDECL(int, 0x00521FBB, (int))
                        (EQ_GLOBAL(int, 0x006EFEA8));
                }
            }
            desc.lpwfxFormat = &wfx;
            desc.dwFlags = 0x4092;
            desc.dwBufferBytes = 0x40;
            wfx.wFormatTag = 1;
            wfx.nSamplesPerSec = 0x5622;
            wfx.wBitsPerSample = 0x10;
            wfx.cbSize = 0;
            wfx.nChannels = 1;
            wfx.nBlockAlign = 2;
            wfx.nAvgBytesPerSec = 0xac44;
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E4D8));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            result = ((SoundInitCreateBuffer_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x0C)))(*dsSlot, &desc, &secondary, 0);
            if (result != 0)
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile == 0)
                    return 0;
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E4AC));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
                return 0;
            }
            logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
            EQ_GLOBAL(int, 0x006EFEA8) = logFile;
            if (logFile != 0)
            {
                EQ_CDECL(int, 0x00524138, (int, char *, ...))
                    (logFile, EQ_ARRAY(char, 0x0057E49C));
                EQ_CDECL(int, 0x00521FBB, (int))
                    (EQ_GLOBAL(int, 0x006EFEA8));
            }
            result = ((SoundInitCall3_guess)(*(u32 *)(*(u32 *)secondary + 0x00)))(secondary, (int)EQ_ARRAY(void, 0x00536E98), (int)&buffer3d);
            if (result < 0)
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile != 0)
                {
                    EQ_CDECL(int, 0x00524138, (int, char *, ...))
                        (logFile, EQ_ARRAY(char, 0x0057E470));
                    EQ_CDECL(int, 0x00521FBB, (int))
                        (EQ_GLOBAL(int, 0x006EFEA8));
                }
                if (buffer3d != 0)
                    ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)buffer3d + 0x08)))(buffer3d);
                buffer3d = 0;
                if (secondary == 0)
                    return 0;
                ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)secondary + 0x08)))(secondary);
                return 0;
            }
            if (secondary != 0)
            {
                *(u8 *)((u8 *)self + 0xBCA) = 0;
                hwnd = 0;
                result = ((SoundInitCall3_guess)(*(u32 *)(*(u32 *)secondary + 0x00)))(secondary, (int)EQ_ARRAY(void, 0x00536E88), (int)(self + 0x2F1));
                if (result == 0)
                {
                    object = (int *)self[0x2F1];
                    ((SoundInitCall4_guess)(*(u32 *)(*(u32 *)object + 0x14)))(object, (int)EQ_ARRAY(void, 0x00535888), 0, (int)&hwnd);
                    if (hwnd == 0 || (((u32)hwnd) & 3) == 0)
                    {
                        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                        if (logFile != 0)
                        {
                            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                                (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E460));
                            EQ_CDECL(int, 0x00521FBB, (int))
                                (EQ_GLOBAL(int, 0x006EFEA8));
                        }
                    }
                    else
                    {
                        *(u8 *)((u8 *)self + 0xBCA) = 1;
                        object = (int *)self[0x2F1];
                        dsSlot = (int **)3;
                        ((SoundInitCall7_guess)(*(u32 *)(*(u32 *)object + 0x10)))(object, (int)EQ_ARRAY(void, 0x00535888), 0, 0, 0, (int)&dsSlot, 4);
                        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                        if (logFile != 0)
                        {
                            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                                (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E468));
                            EQ_CDECL(int, 0x00521FBB, (int))
                                (EQ_GLOBAL(int, 0x006EFEA8));
                        }
                    }
                }
            }
            object = (int *)self[0x2F1];
            if (object != 0 && support == 0)
            {
                hwnd = 0;
                ((SoundInitCall4_guess)(*(u32 *)(*(u32 *)object + 0x14)))(object, (int)EQ_ARRAY(void, 0x005359B0), 0xb, (int)&hwnd);
                *(bool *)(self + 0x2F2) = (((u32)hwnd) & 3) != 0;
            }
            module = EQ_GLOBAL(SoundInitLoadLibraryA_guess, 0x00534048)(EQ_ARRAY(char, 0x0057E454));
            self[0x2D5] = (int)module;
            if (module != 0)
            {
                proc = EQ_GLOBAL(SoundInitGetProcAddress_guess, 0x00534044)(module, EQ_ARRAY(char, 0x0057E440));
                if (proc != 0)
                {
                    ((SoundInitEaxManagerCreate_guess)proc)(self + 0x2D4);
                    if (self[0x2D4] != 0)
                    {
                        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                        if (logFile != 0)
                        {
                            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                                (logFile, EQ_ARRAY(char, 0x0057E438));
                            EQ_CDECL(int, 0x00521FBB, (int))
                                (EQ_GLOBAL(int, 0x006EFEA8));
                        }
                        eaxFlag = ((SoundInitEaxManagerInterface_guess *)self)->Configure_guess(EQ_ARRAY(char, 0x0057BF7C), 1);
                        *(u8 *)((u8 *)self + 0xBC9) = eaxFlag;
                        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                        if (logFile != 0)
                        {
                            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                                (logFile, EQ_ARRAY(char, 0x0057E42C), *(u8 *)((u8 *)self + 0xBC9));
                            EQ_CDECL(int, 0x00521FBB, (int))
                                (EQ_GLOBAL(int, 0x006EFEA8));
                        }
                        goto releaseTemps;
                    }
                    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                    if (logFile != 0)
                    {
                        EQ_CDECL(int, 0x00524138, (int, char *, ...))
                            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E41C));
                        EQ_CDECL(int, 0x00521FBB, (int))
                            (EQ_GLOBAL(int, 0x006EFEA8));
                    }
                }
                else
                {
                    logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                        (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                    EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                    if (logFile != 0)
                    {
                        EQ_CDECL(int, 0x00524138, (int, char *, ...))
                            (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E408));
                        EQ_CDECL(int, 0x00521FBB, (int))
                            (EQ_GLOBAL(int, 0x006EFEA8));
                    }
                }
            }
            else
            {
                logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
                    (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
                EQ_GLOBAL(int, 0x006EFEA8) = logFile;
                if (logFile != 0)
                {
                    EQ_CDECL(int, 0x00524138, (int, char *, ...))
                        (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E3FC));
                    EQ_CDECL(int, 0x00521FBB, (int))
                        (EQ_GLOBAL(int, 0x006EFEA8));
                }
            }
releaseTemps:
            if (buffer3d != 0)
                ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)buffer3d + 0x08)))(buffer3d);
            buffer3d = 0;
            if (secondary != 0)
                ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)secondary + 0x08)))(secondary);
            return 1;
        }
        logFile = EQ_CDECL(int, 0x005223BC, (char *, char *))
            (EQ_ARRAY(char, 0x0057E380), EQ_ARRAY(char, 0x00564E48));
        EQ_GLOBAL(int, 0x006EFEA8) = logFile;
        if (logFile != 0)
        {
            EQ_CDECL(int, 0x00524138, (int, char *, ...))
                (EQ_GLOBAL(int, 0x006EFEA8), EQ_ARRAY(char, 0x0057E510));
            EQ_CDECL(int, 0x00521FBB, (int))
                (EQ_GLOBAL(int, 0x006EFEA8));
        }
releasePrimary:
    ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)*hwnd + 0x08)))(*hwnd);
    ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)*dsSlot + 0x08)))(*dsSlot);
    *hwnd = 0;
    *dsSlot = 0;
    return 0;
releaseDirectSound:
    object = *dsSlot;
releaseObject:
    ((SoundAssetVtableOneArg_guess)(*(u32 *)(*(u32 *)object + 0x08)))(object);
clearDsSlot:
    *dsSlot = 0;
    return 0;
}

// Local stand-in: the SoundAsset declared above has no method for the vtable store at 0x004ED9A5.
class SoundAssetVtableStore_guess
{
public:
    void StoreVtable_guess(void);
};

// FUNCTION: TRILOGY 0x004ED9A5
// Sound asset vtable setup helper; its game-level purpose is unclear.
// __thiscall, ECX = this, no stack arguments, callee pops 0. Stores the SoundAsset vtable (0x00535900) at +0 and
// returns: the store SoundAsset::~SoundAsset ends with (0x004ECDB3), alone.
void SoundAssetVtableStore_guess::StoreVtable_guess(void)
{
    *(u32 *)this = 0x00535900;
}

class SoundAssetDeletingDtor_guess
{
public:
    SoundAssetDeletingDtor_guess *ScalarDeletingDtorWithFlag_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004ED9AC
// Object deletion helper; its sound-specific role is unclear.
SoundAssetDeletingDtor_guess *SoundAssetDeletingDtor_guess::ScalarDeletingDtorWithFlag_guess(u8 flags)
{
    *(u32 *)this = 0x00535900;
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void *))(this);
    return this;
}

// FUNCTION: TRILOGY 0x004EDC54
// Builds a sound handle: entry `index` in the high word, a slot obtained from that entry in the low word.
// @early-stop: call result takes EAX and the return value ECX; the original has them the other way round.
u32 __fastcall t3dIsHierarchicalSpriteValid_guess(void *self, int edx_unused, int index)
{
    u32 slot;
    u32 uVar2;

    if (index >= 0 && index < 0x2d0 &&
        *(int *)((char *)self + 0xc + index * 4) != 0)
    {
        slot = EQ_METHOD(0, u32, 0x004EB931, (void *), *(void **)((char *)self + 0xc + index * 4))();
        uVar2 = 0xffffffff;
        if (slot != 0xffffffff)
        {
            uVar2 = index << 0x10 | slot & 0xffff;
        }
    }
    else
    {
        uVar2 = 0;
    }
    return uVar2;
}

// FUNCTION: TRILOGY 0x004edc90
// Forwards a sound handle to its slot-specific action and invalidates the handle.
// @early-stop: fake-frame allocation, EDI save timing, stack-store order, and StopSlot EDX zeroing still differ.
void __fastcall ReleaseHandle_guess(void *self, int edx_unused, u32 *ptr)
{
    u32 *handlePtr = ptr;
    int index;

    index = (int)(*handlePtr >> 0x10);
    /* fake match: keep a frame so the mutable stack argument has a stable address. */
    volatile int frameMarker = 0;
    if ((index >= 0) && (index < 0x2d0) &&
        (*(int *)((char *)self + 0xc + index * 4) != 0))
    {
        ptr = (u32 *)(u32)(u16)*ptr;
        if (*(int *)((char *)self + 0xb4c) != 0)
        {
            // This thiscall callee ignores EDX; preserve the incoming register value.
            EQ_THISCALL(void, 0x004EE5E4, (void *, int, u32 *))
                (*(void **)((char *)self + 0xb4c), edx_unused, handlePtr);
        }
        EQ_METHOD(1, void, 0x004EB96A, (void *, u32 *), *(void **)((char *)self + 0xc + index * 4))((u32 *)&ptr);
    }
    *handlePtr = 0xffffffff;
    return;
}

// FUNCTION: TRILOGY 0x004edcdb
// Processes a queued sound handle and updates its slot's parameters.
// __fastcall (callee pops 8). The high word of *soundHandle is the slot index. For a non-zero threshold, an index
// in 0..0x2cf and a slot object: with parameters set, first calls UpdateSlotParameters_guess on the slot; if the queue count is
// above the threshold and the next count reaches it, cleans up and returns 1 when the token matches *soundHandle,
// else notifies the window; then plays the slot (PlayR). Returns 0 otherwise.
// @early-stop: EQ_THISCALL's dummy EDX costs an xor per call (VC6 has no __thiscall); lea of slotEntry sits above the slot test; mov esi,ecx comes before the *soundHandle load.
int __fastcall CStmlWnd_ProcessStmlWindowUpdate_guess(void *self, int edx_unused, u32 *soundHandle, u32 parameters)
{
    int index;
    u32 count;
    u8 node[12];
    u32 *slotEntry;
    void *slot;

    index = *soundHandle >> 0x10;
    if (*(u32 *)((char *)self + 0xbcc) != 0 && index >= 0 && index < 0x2d0 &&
        (slot = (void *)*(u32 *)((char *)self + 0xc + index * 4)) != 0)
    {
        slotEntry = (u32 *)((char *)self + 0xc + index * 4);
        if (parameters != 0)
        {
            EQ_METHOD(3, void, 0x004EC77D, (void *, u32, u32, int), slot)((u16)*soundHandle, parameters, 1);
        }
        count = EQ_METHOD(1, u32, 0x004EE589, (void *, u32 *), *(void **)((char *)self + 0xb4c))(soundHandle);
        if (count > *(u32 *)((char *)self + 0xbcc))
        {
            void *queueObject = *(void **)((char *)self + 0xb60);
            ((void (__stdcall *)(void *, u8 *))
                (*(u32 *)(*(u32 *)queueObject + 0x1c)))
                    (queueObject, node);
            count = EQ_METHOD(1, u32, 0x004EE62A, (void *, u8 *), *(void **)((char *)self + 0xb4c))(node);
            if (count >= *(u32 *)((char *)self + 0xbcc))
            {
                if ((parameters = EQ_METHOD(0, u32, 0x004EE791, (void *), *(void **)((char *)self + 0xb4c))()) == *soundHandle)
                {
                    EQ_METHOD(1, void, 0x004EE5E4, (void *, u32 *), *(void **)((char *)self + 0xb4c))(soundHandle);
                    return 1;
                }
                ((void (__fastcall *)(void *, int, u32 *))
                    (*(u32 *)(*(u32 *)self + 0x2c)))(self, 0, &parameters);
            }
        }
        return EQ_METHOD(2, u8, 0x004EC094, (void *, u32, int), (void *)*slotEntry)((u16)*soundHandle, 0);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004eddb6
// Routes a sound buffer through an available slot, subject to its mode setting.
// __thiscall (vtable entry 0x005358B8). For a valid index with a slot object: unless the buffer's mode
// has bit 0x10 and that slot object already plays a voice with the mode word as id, acquires a slot
// (vtable 0x18, result reuses the index argument), plays the buffer into it (0x24), releases it (0x1c)
// and returns the play result; 0 otherwise.
// @early-stop: voice/buffer registers swapped (EBX/EDI); fastcall EDX dummy adds an xor; index temp in EAX.
int SoundSlotTable_guess::PlayBuffer_guess(int index, EQSoundBuffer_guess *buffer)
{
    if (index >= 0 && index < 0x2d0 && slots[index] != 0)
    {
        int voice = index;

        if ((buffer->mode_guess & 0x10) == 0 ||
            EQ_METHOD(1, int, 0x004ECA93, (EqDirectSoundSlotsStop_guess *, u32), slots[voice])(buffer->mode_guess) <= 0)
        {
            index = Acquire_guess(voice);
            if (index >= 0)
            {
                int result = Play_guess(&index, buffer);
                Release_guess(&index);
                return result;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004ede1e
// Forwards a handle's slot value to its slot-specific action; the action is unclear.
int __fastcall DispatchHandleToSlot_guess(SoundSlotTable_guess *table, int unusedEdx, u32 *handle)
{
    int index = *handle >> 16;

    if (index >= 0 && index < 0x2d0 && table->slots[index] != 0)
        return EQ_METHOD(1, int, 0x004EBDEF, (void *, u32), table->slots[index])((u16)*handle);
    return 0;
}

// FUNCTION: TRILOGY 0x004ede4a
// Forwards a handle and two values to its slot-specific action; the action is unclear.
int SoundSlotTable_guess::DispatchHandleToSlotWithArgs_guess(u32 *handle, int calleeArgument1, int calleeArgument2)
{
    int index = *handle >> 16;

    if (index >= 0 && index < 0x2d0 && slots[index] != 0)
        return EQ_METHOD(3, int, 0x004EBE59, (void *, u32, int, int), slots[index])((u32)(u16)*handle, calleeArgument1, calleeArgument2);
    return -1;
}

// The 0x5C-byte object at +0x60 of a voice slot object, copied whole into a local. Local: not yet in
// include/. Only the dword at 0x4C and the float at 0x50 are read.
struct SoundSlotInfo5C_guess
{
    u32 unknown0x00[0x13];                   // 0x00..0x4B (not read)
    u32 unknown0x4C;                         // 0x4C
    float unknown0x50;                       // 0x50
    u32 unknown0x54;                         // 0x54
    u32 unknown0x58;                         // 0x58
};
C_ASSERT(sizeof(SoundSlotInfo5C_guess) == 0x5c);
C_ASSERT(offsetof(SoundSlotInfo5C_guess, unknown0x50) == 0x50);

// The 8-byte result: a dword at 0x00 and a float at 0x04. Local: not yet in include/.
struct SoundSlotInfo_guess
{
    u32 unknown0x00;                         // 0x00
    float unknown0x04;                       // 0x04
};
C_ASSERT(sizeof(SoundSlotInfo_guess) == 0x8);

// FUNCTION: TRILOGY 0x004edf31
// Reads two opaque values from an active slot selected by a sound handle.
// __fastcall-shaped: the table in ECX, EDX dummy (passed on to the callee), two stack arguments (callee
// pops 8). The high word of `handle` is the slot index. For an index below 0x2d0 with a non-null slot
// that 0x004EE2FD accepts, copies the 0x5c bytes at +0x60 of that slot into a local, stores its float at
// +0x50 to info+0x4 and its dword at +0x4C to info+0x0, and returns true; else false.
bool __fastcall SoundSlotTable_GetSlotInfo_guess(SoundSlotTable_guess *table, int unused_edx, u32 *handle, SoundSlotInfo_guess *info)
{
    SoundSlotInfo5C_guess copy;
    int index;

    index = *handle >> 16;
    if (index >= 0 && index < 0x2d0 && table->slots[index] != 0 &&
        EQ_THISCALL(int, 0x004EE2FD, (SoundSlotTable_guess *, int, int))(table, unused_edx, index))
    {
        copy = *(SoundSlotInfo5C_guess *)((u8 *)table->slots[index] + 0x60);
        info->unknown0x04 = copy.unknown0x50;
        info->unknown0x00 = copy.unknown0x4C;
        return true;
    }
    return false;
}

// FUNCTION: TRILOGY 0x004EDF86
// Reads two opaque values from an active slot selected by its index.
// __fastcall-shaped like its neighbour 0x004EDF31: the table in ECX, EDX dummy (passed on to the callee), two
// stack arguments (callee pops 8). `index` is the slot index itself (no handle). For an index below 0x2d0 with a
// non-null slot that 0x004EE2FD accepts, copies the 0x5c bytes at +0x60 of that slot into a local, stores its
// float at +0x50 to info+0x4 and its dword at +0x4C to info+0x0, and returns true; else false.
bool __fastcall SoundSlotTable_GetSlotInfoByIndex_guess(SoundSlotTable_guess *table, int unused_edx, int index, SoundSlotInfo_guess *info)
{
    SoundSlotInfo5C_guess copy;

    if (index >= 0 && index < 0x2d0 && table->slots[index] != 0 &&
        EQ_THISCALL(int, 0x004EE2FD, (SoundSlotTable_guess *, int, int))(table, unused_edx, index))
    {
        copy = *(SoundSlotInfo5C_guess *)((u8 *)table->slots[index] + 0x60);
        info->unknown0x04 = copy.unknown0x50;
        info->unknown0x00 = copy.unknown0x4C;
        return true;
    }
    return false;
}

// Three dwords copied whole (the original copies them as undefined4). Local stand-in: utf8.h's
// EQVector3_guess has no assignment in its header.
struct SoundVector3_guess
{
    u32 v_guess[3];
};

// The 0x58-byte object this function assigns; the dword at 0x00 is not copied. Local: no header declares
// the layout yet. The vectors at 0x04, 0x10, 0x1C and 0x28 are copied whole; 0x34..0x54 are single dwords.
class SoundObject58_guess
{
public:
    u32 unknown0x00;                     // 0x00 (not copied)
    SoundVector3_guess vector0x04_guess; // 0x04
    SoundVector3_guess vector0x10_guess; // 0x10
    SoundVector3_guess vector0x1C_guess; // 0x1C
    SoundVector3_guess vector0x28_guess; // 0x28
    u32 unknown0x34;                     // 0x34
    u32 unknown0x38;                     // 0x38
    u32 unknown0x3C;                     // 0x3C
    u32 unknown0x40;                     // 0x40
    u32 unknown0x44;                     // 0x44
    u32 unknown0x48;                     // 0x48
    u32 unknown0x4C;                     // 0x4C
    u32 unknown0x50;                     // 0x50
    u32 unknown0x54;                     // 0x54

    SoundObject58_guess &CopyFrom_guess(const SoundObject58_guess &source);
};
C_ASSERT(sizeof(SoundObject58_guess) == 0x58);
C_ASSERT(offsetof(SoundObject58_guess, unknown0x54) == 0x54);

// FUNCTION: TRILOGY 0x004edfe9
// Copies a fixed-size object's stored fields from another instance; its game role is unclear.
// __thiscall, one stack argument (callee pops 4). Copies everything except the dword at 0x00 from
// `source` and returns this (EAX still holds this at the RET).
SoundObject58_guess &SoundObject58_guess::CopyFrom_guess(const SoundObject58_guess &source)
{
    vector0x04_guess = source.vector0x04_guess;
    vector0x10_guess = source.vector0x10_guess;
    vector0x1C_guess = source.vector0x1C_guess;
    vector0x28_guess = source.vector0x28_guess;
    unknown0x34 = source.unknown0x34;
    unknown0x38 = source.unknown0x38;
    unknown0x3C = source.unknown0x3C;
    unknown0x40 = source.unknown0x40;
    unknown0x44 = source.unknown0x44;
    unknown0x48 = source.unknown0x48;
    unknown0x4C = source.unknown0x4C;
    unknown0x50 = source.unknown0x50;
    unknown0x54 = source.unknown0x54;
    return *this;
}

// Red-black tree node (std::map-like, 0x00 left, 0x04 parent, 0x08 right) and the tree object whose
// pointer at 0x04 is the header node (header->parent = root). Local: not yet in include/.
struct SoundTreeNode_guess
{
    SoundTreeNode_guess *left;      // 0x00
    SoundTreeNode_guess *parent;    // 0x04
    SoundTreeNode_guess *right;     // 0x08
    int key;                        // 0x0c
};

// The tree's iterator: a node pointer deriving from an empty tag base (like the STL's _Bidit), which makes
// VC6 return it through a hidden pointer.
struct SoundTreeIteratorBase_guess
{
};

class SoundTreeIterator_guess : public SoundTreeIteratorBase_guess
{
public:
    SoundTreeIterator_guess() {}
    SoundTreeIterator_guess(SoundTreeNode_guess *n) : node(n) {}
    bool operator==(const SoundTreeIterator_guess &other) const { return node == other.node; }
    SoundTreeNode_guess *node;
};

class SoundTree_guess
{
public:
    u32 unknown0x00;                // 0x00
    SoundTreeNode_guess *header;    // 0x04

    SoundTreeIterator_guess End_guess() { return SoundTreeIterator_guess(header); }
    void InsertHint_guess(u32 *result, int *key);
    SoundTreeIterator_guess Find_guess(const int &key);
};

// FUNCTION: TRILOGY 0x004ee91d
// Performs ordered-tree insertion for a queue key; its in-game meaning is unclear.
// @early-stop: VC6 merges the three insert call sites into one shared push group (62 vs 70 insns) and orders the preheader/loop moves differently; the blocks themselves match.
void SoundTree_guess::InsertHint_guess(u32 *result, int *key)
{
    SoundTreeNode_guess *header;
    SoundTreeNode_guess *current;
    SoundTreeNode_guess *parent;
    SoundTreeNode_guess *hint;
    SoundTreeNode_guess *nil;
    u32 *insertResult;
    bool goLeft;

    header = *(SoundTreeNode_guess **)((char *)this + 4);
    nil = EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4);
    parent = header;
    goLeft = true;
    current = header->parent;
    while (current != nil)
    {
        goLeft = *key < current->key;
        parent = current;
        if (goLeft)
            current = current->left;
        else
            current = current->right;
    }
    if (*(char *)((char *)this + 8) != 0)
    {
        insertResult = EQ_METHOD(4, u32 *, 0x004EEE11, (SoundTree_guess *, SoundTreeNode_guess **, SoundTreeNode_guess *, SoundTreeNode_guess *, int *), this)((SoundTreeNode_guess **)&key, current, parent, key);
        *result = *insertResult;
        *(u8 *)(result + 1) = 1;
        return;
    }
    hint = parent;
    if (goLeft)
    {
        if (parent == header->left)
        {
            insertResult = EQ_METHOD(4, u32 *, 0x004EEE11, (SoundTree_guess *, SoundTreeNode_guess **, SoundTreeNode_guess *, SoundTreeNode_guess *, int *), this)((SoundTreeNode_guess **)&key, current, parent, key);
            *result = *insertResult;
            *(u8 *)(result + 1) = 1;
            return;
        }
        EQ_METHOD(0, void, 0x004EF04F, (SoundTreeNode_guess **), &hint)();
    }
    if (hint->key < *key)
    {
        insertResult = EQ_METHOD(4, u32 *, 0x004EEE11, (SoundTree_guess *, SoundTreeNode_guess **, SoundTreeNode_guess *, SoundTreeNode_guess *, int *), this)((SoundTreeNode_guess **)&key, current, parent, key);
        *result = *insertResult;
        *(u8 *)(result + 1) = 1;
        return;
    }
    *(u8 *)(result + 1) = 0;
    *result = (u32)hint;
}

// FUNCTION: TRILOGY 0x004eeab7
// Looks up an ordered queue key; what the key represents in play is unclear.
// __thiscall. map::find: the lower bound of `key` (0x004EEF98), or end() (the header) when that is end()
// or its key is greater than `key`.
SoundTreeIterator_guess SoundTree_guess::Find_guess(const int &key)
{
    SoundTreeIterator_guess p(EQ_METHOD(1, SoundTreeNode_guess *, 0x004EEF98, (SoundTree_guess *, const int *), this)(&key));
    return (p == End_guess() || key < p.node->key) ? End_guess() : p;
}

class SoundTreeErase_guess
{
public:
    void Erase_guess(int **result, int *node);
};

// FUNCTION: TRILOGY 0x004eeaf1
// map::erase(iterator): removes one entry, keeps the red-black tree balanced, and returns the following entry.
// @early-stop: prologue spills, load/store scheduling, EQ_THISCALL EDX setup, and final argument reloads still differ.
/* fake match: the successor case skips common transplant and header updates after relinking; goto erase_done preserves that split. */
void SoundTreeErase_guess::Erase_guess(int **result, int *node)
{
    int *splicedNode;
    int *nodeToErase;
    int *sibling;
    int *successor;
    int *fixupNode;
    int headerNode;

    nodeToErase = node;
    EQ_METHOD(0, void, 0x004EEF62, (int **), &node)();
    sibling = (int *)*nodeToErase;
    splicedNode = nodeToErase;
    if (sibling == EQ_GLOBAL(int *, 0x006EFEB4))
    {
        fixupNode = (int *)nodeToErase[2];
    }
    else
    {
        int *parent;

        successor = (int *)nodeToErase[2];
        if (successor == EQ_GLOBAL(int *, 0x006EFEB4))
        {
            fixupNode = sibling;
        }
        else
        {
            while ((int *)*successor != EQ_GLOBAL(int *, 0x006EFEB4))
                successor = (int *)*successor;
            splicedNode = successor;
            fixupNode = (int *)splicedNode[2];
            if (splicedNode != nodeToErase)
            {
                sibling[1] = (int)splicedNode;
                *splicedNode = *nodeToErase;
                if (splicedNode == (int *)nodeToErase[2])
                {
                    fixupNode[1] = (int)splicedNode;
                }
                else
                {
                    fixupNode[1] = splicedNode[1];
                    *(int **)splicedNode[1] = fixupNode;
                    splicedNode[2] = nodeToErase[2];
                    *(int **)(nodeToErase[2] + 4) = splicedNode;
                }
                if (*(int **)(*(int *)((char *)this + 4) + 4) == nodeToErase)
                {
                    *(int **)(*(int *)((char *)this + 4) + 4) = splicedNode;
                }
                else
                {
                    parent = (int *)nodeToErase[1];
                    if ((int *)*parent == nodeToErase)
                    {
                        *parent = (int)splicedNode;
                    }
                    else
                    {
                        parent[2] = (int)splicedNode;
                    }
                }
                splicedNode[1] = nodeToErase[1];
                headerNode = splicedNode[5];
                splicedNode[5] = nodeToErase[5];
                nodeToErase[5] = headerNode;
                splicedNode = nodeToErase;
                goto erase_done;
            }
        }
    }
    fixupNode[1] = splicedNode[1];
    sibling = *(int **)((char *)this + 4);
    if ((int *)sibling[1] == nodeToErase)
    {
        sibling[1] = (int)fixupNode;
    }
    else
    {
        int *parent;

        parent = (int *)nodeToErase[1];
        if ((int *)*parent == nodeToErase)
        {
            *parent = (int)fixupNode;
        }
        else
        {
            parent[2] = (int)fixupNode;
        }
    }
    if ((int *)*(*(int **)((char *)this + 4)) == nodeToErase)
    {
        if ((int *)nodeToErase[2] == EQ_GLOBAL(int *, 0x006EFEB4))
        {
            *(*(int **)((char *)this + 4)) = nodeToErase[1];
        }
        else
        {
            sibling = fixupNode;
            while ((int *)*sibling != EQ_GLOBAL(int *, 0x006EFEB4))
                sibling = (int *)*sibling;
            *(*(int **)((char *)this + 4)) = (int)sibling;
        }
    }
    headerNode = (int)*(int **)((char *)this + 4);
    if (*(int **)(headerNode + 8) == nodeToErase)
    {
        if ((int *)*nodeToErase == EQ_GLOBAL(int *, 0x006EFEB4))
        {
            *(int *)(headerNode + 8) = nodeToErase[1];
        }
        else
        {
            sibling = fixupNode;
            while ((int *)sibling[2] != EQ_GLOBAL(int *, 0x006EFEB4))
                sibling = (int *)sibling[2];
            *(int **)(headerNode + 8) = sibling;
        }
    }
erase_done:
    if (splicedNode[5] == 1)
    {
        while (fixupNode != *(int **)(*(int *)((char *)this + 4) + 4) && fixupNode[5] == 1)
        {
            sibling = (int *)*(int **)(fixupNode[1]);
            if (fixupNode == sibling)
            {
                sibling = (int *)((int *)fixupNode[1])[2];
                if (sibling[5] == 0)
                {
                    sibling[5] = 1;
                    ((int *)fixupNode[1])[5] = 0;
                    EQ_METHOD(1, void, 0x004EEFC5, (SoundTree_guess *, int *), (SoundTree_guess *)this)((int *)fixupNode[1]);
                    sibling = (int *)((int *)fixupNode[1])[2];
                }
                if ((((int *)*sibling)[5] != 1) || (((int *)sibling[2])[5] != 1))
                {
                    if (((int *)sibling[2])[5] == 1)
                    {
                        ((int *)*sibling)[5] = 1;
                        sibling[5] = 0;
                        EQ_METHOD(1, void, 0x004EF009, (SoundTree_guess *, int *), (SoundTree_guess *)this)(sibling);
                        sibling = (int *)((int *)fixupNode[1])[2];
                    }
                    sibling[5] = ((int *)fixupNode[1])[5];
                    ((int *)fixupNode[1])[5] = 1;
                    ((int *)sibling[2])[5] = 1;
                    EQ_METHOD(1, void, 0x004EEFC5, (SoundTree_guess *, int *), (SoundTree_guess *)this)((int *)fixupNode[1]);
                    break;
                }
            }
            else
            {
                if (sibling[5] == 0)
                {
                    sibling[5] = 1;
                    ((int *)fixupNode[1])[5] = 0;
                    EQ_METHOD(1, void, 0x004EF009, (SoundTree_guess *, int *), (SoundTree_guess *)this)((int *)fixupNode[1]);
                    sibling = (int *)*(int **)(fixupNode[1]);
                }
                if ((((int *)sibling[2])[5] != 1) || (((int *)*sibling)[5] != 1))
                {
                    if (((int *)*sibling)[5] == 1)
                    {
                        ((int *)sibling[2])[5] = 1;
                        sibling[5] = 0;
                        EQ_METHOD(1, void, 0x004EEFC5, (SoundTree_guess *, int *), (SoundTree_guess *)this)(sibling);
                        sibling = (int *)*(int **)(fixupNode[1]);
                    }
                    sibling[5] = ((int *)fixupNode[1])[5];
                    ((int *)fixupNode[1])[5] = 1;
                    ((int *)*sibling)[5] = 1;
                    EQ_METHOD(1, void, 0x004EF009, (SoundTree_guess *, int *), (SoundTree_guess *)this)((int *)fixupNode[1]);
                    break;
                }
            }
            sibling[5] = 0;
            fixupNode = (int *)fixupNode[1];
        }
        fixupNode[5] = 1;
    }
    EQ_CDECL(void, 0x005218BB, (int *))(splicedNode);
    *(int *)((char *)this + 0x0c) = *(int *)((char *)this + 0x0c) - 1;
    *result = node;
}

class SoundTreeInsert_guess
{
public:
    void *Insert_guess(int **result, int relation, int *parent, int *pair);
};

struct SoundTreeInsertNode_guess
{
    int fields[6];
};

// FUNCTION: TRILOGY 0x004EEE11
// Adds a sound entry to the ordered lookup tree.
// @early-stop: this is copied into EBX before the operator new push (the original copies it after); the loop entry loads p[1] from EDI before the mov to ESI; the second rotation reloads ECX early.
/* fake match: parent and grandparent are re-read through p[1] in each block; with a parent local VC6 copy-propagates p into the rotation calls and changes the recolor registers. */
void *SoundTreeInsert_guess::Insert_guess(int **result, int relation, int *parent, int *pair)
{
    int *node;
    int *p;
    int *uncle;

    node = (int *)EQ_CDECL(void *, 0x005218C6, (u32))(0x18);
    node[5] = 0;
    node[1] = (int)parent;
    node[0] = (int)EQ_GLOBAL(int *, 0x006EFEB4);
    node[2] = (int)EQ_GLOBAL(int *, 0x006EFEB4);
    EQ_CDECL(void, 0x004EF0B8, (void *, void *))((char *)node + 0x0C, pair);
    *(int *)((char *)this + 0x0C) = *(int *)((char *)this + 0x0C) + 1;
    if ((parent == *(int **)((char *)this + 4)) || (relation != (int)EQ_GLOBAL(int *, 0x006EFEB4)) || (*pair < parent[3]))
    {
        int *header;

        *parent = (int)node;
        header = *(int **)((char *)this + 4);
        if (parent == header)
        {
            header[1] = (int)node;
            header = *(int **)((char *)this + 4);
            header[2] = (int)node;
        }
        else
        {
            if (parent == (int *)*header)
                *header = (int)node;
        }
    }
    else
    {
        int *header;

        parent[2] = (int)node;
        header = *(int **)((char *)this + 4);
        if (parent == (int *)header[2])
            header[2] = (int)node;
    }

    if (node != (int *)(*(int **)((char *)this + 4))[1])
    {
        p = node;
        do
        {
            if (*(int *)(p[1] + 0x14) != 0)
                break;
            uncle = *(int **)*(int *)(p[1] + 4);
            if (p[1] == (int)uncle)
            {
                uncle = ((int **)*(int *)(p[1] + 4))[2];
                if (uncle[5] == 0)
                {
                    *(int *)(p[1] + 0x14) = 1;
                    uncle[5] = 1;
                    *(int *)(*(int *)(p[1] + 4) + 0x14) = 0;
                    p = (int *)*(int *)(p[1] + 4);
                }
                else
                {
                    if (p == (int *)*(int *)(p[1] + 8))
                    {
                        p = (int *)p[1];
                        EQ_THISCALL(void, 0x004EEFC5, (void *, int, void *))(this, 0, p);
                    }
                    *(int *)(p[1] + 0x14) = 1;
                    *(int *)(*(int *)(p[1] + 4) + 0x14) = 0;
                    EQ_THISCALL(void, 0x004EF009, (void *, int, void *))(this, 0, (void *)*(int *)(p[1] + 4));
                }
            }
            else
            {
                if (uncle[5] == 0)
                {
                    *(int *)(p[1] + 0x14) = 1;
                    uncle[5] = 1;
                    *(int *)(*(int *)(p[1] + 4) + 0x14) = 0;
                    p = (int *)*(int *)(p[1] + 4);
                }
                else
                {
                    if (p == (int *)*(int *)p[1])
                    {
                        p = (int *)p[1];
                        EQ_THISCALL(void, 0x004EF009, (void *, int, void *))(this, 0, p);
                    }
                    *(int *)(p[1] + 0x14) = 1;
                    *(int *)(*(int *)(p[1] + 4) + 0x14) = 0;
                    EQ_THISCALL(void, 0x004EEFC5, (void *, int, void *))(this, 0, (void *)*(int *)(p[1] + 4));
                }
            }
        } while (p != (int *)(*(int **)((char *)this + 4))[1]);
    }
    *(int *)((char *)(*(int **)((char *)this + 4))[1] + 0x14) = 1;
    *result = node;
    return result;
}

// FUNCTION: TRILOGY 0x004eef62
// Advances an ordered-tree cursor to its next entry.
// @early-stop: right-child load and parent-loop current reload remain ordered differently; eight iterations did not improve the match.
void __fastcall MoveNext_guess(SoundTreeIterator_guess *iterator)
{
    SoundTreeNode_guess *node = iterator->node->right;
    SoundTreeNode_guess *nil = EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4);

    if (node != nil)
    {
        while (node->left != nil)
        {
            node = node->left;
        }
    }
    else
    {
        for (node = iterator->node->parent; iterator->node == node->right; node = node->parent)
        {
            iterator->node = node;
        }
        if (iterator->node->right == node)
        {
            return;
        }
    }
    iterator->node = node;
    return;
}

// FUNCTION: TRILOGY 0x004eefc5
// __thiscall, written as __fastcall with an unused EDX slot. Left rotation of `node` in the tree (g_tree_nil_guess is the nil node).
void __fastcall SoundTree_guess_RotateLeft_guess(SoundTree_guess *tree, int unused_edx, SoundTreeNode_guess *node)
{
    SoundTreeNode_guess *pivot = node->right;
    node->right = pivot->left;
    if (pivot->left != EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4))
        pivot->left->parent = node;
    pivot->parent = node->parent;
    if (node == tree->header->parent)
        tree->header->parent = pivot;
    else
    {
        SoundTreeNode_guess *p = node->parent;
        if (node == p->left)
            p->left = pivot;
        else
            p->right = pivot;
    }
    pivot->left = node;
    node->parent = pivot;
}

// FUNCTION: TRILOGY 0x004ef009
// __thiscall, written as __fastcall with an unused EDX slot. Right rotation of `node`, the mirror of the left rotation.
void __fastcall SoundTree_guess_RotateRight_guess(SoundTree_guess *tree, int unused_edx, SoundTreeNode_guess *node)
{
    SoundTreeNode_guess *pivot = node->left;
    node->left = pivot->right;
    if (pivot->right != EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4))
        pivot->right->parent = node;
    pivot->parent = node->parent;
    if (node == tree->header->parent)
        tree->header->parent = pivot;
    else
    {
        SoundTreeNode_guess *p = node->parent;
        if (node == p->right)
            p->right = pivot;
        else
            p->left = pivot;
    }
    pivot->right = node;
    node->parent = pivot;
}

// FUNCTION: TRILOGY 0x004ef04f
// Moves an ordered-tree cursor to its previous entry.
// @early-stop: Only the ESI save order differs: VC6 emits PUSH ESI before the flag CMP.
void __fastcall MovePrevious_guess(SoundTreeNode_guess **cursor)
{
    SoundTreeNode_guess *node;
    if (*(int *)((char *)*cursor + 0x14) == 0 &&
        (*cursor)->parent->parent == *cursor)
    {
        *cursor = (*cursor)->right;
    }
    else
    {
        if ((*cursor)->left != EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4))
        {
            node = (*cursor)->left;
            while (node->right != EQ_GLOBAL(SoundTreeNode_guess *, 0x006EFEB4))
                node = node->right;
            *cursor = node;
        }
        else
        {
            do
            {
                node = (*cursor)->parent;
                if (*cursor != node->left)
                    break;
                *cursor = node;
            } while (true);
            *cursor = node;
        }
    }
}

// FUNCTION: TRILOGY 0x004ef0b8
// Copies a sound lookup entry for storage in the tree.
void CopySoundPair_guess(void *destination, const void *source)
{
    if (destination != 0)
    {
        *(u32 *)destination = *(const u32 *)source;
        *(u32 *)((u8 *)destination + 4) = *(const u32 *)((const u8 *)source + 4);
    }
}
