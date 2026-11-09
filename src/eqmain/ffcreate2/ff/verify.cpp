// eqmain: the same original object as trilogy's src/ffcreate2/ff/verify.cpp (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// Layouts are guesses from the asm of FUN_004f99a0 only; no header defines them yet.
struct FfVerifyTarget_guess
{
    char pad_0000[0x200c];
    int field_200c_guess; // 0x200c
};

struct FfVerify_guess
{
    FfVerifyTarget_guess *target_guess; // 0x00

    int GetField200c_guess();
};

// Layouts guessed from the asm of FUN_004e8647 only: a refcounted pointer plus one extra dword.
struct FfShared_guess
{
    int pad_0000;
    int refs_guess; // 0x04
};

struct FfSharedRef_guess
{
    FfShared_guess *shared_guess; // 0x00
    int extra_guess;              // 0x04

    FfSharedRef_guess(const FfSharedRef_guess &src);
};

// Layout guessed from the asm of FUN_004e8693 only: embeds a resource owner at 0x14.
struct FfResourceOwner_guess;

struct FfHasOwner_guess
{
    char pad_0000[0x14];
    // 0x14: FfResourceOwner_guess

    void ReleaseOwner_guess();
};

struct FfVerifyEntry_guess
{
    const char *file; // 0x00
    int line;         // 0x04
};

typedef void (__cdecl *VerifyHook_guess)();

struct FfVerifyCopy_guess
{
    FfVerifyCopy_guess *CopyFrom_guess(FfVerifyCopy_guess *source);
};

// The object is only passed through; this layout is empty on purpose.
struct FfSoundLock_guess
{
    FfSoundLock_guess *InitializeSoundCriticalSection_guess();
};

struct FfResourceOwner_guess;

// Layouts guessed from the asm of FUN_004f9710 only: a refcounted object with a virtual destructor.
struct FfRefObject_guess
{
    virtual void destroy_guess(int flags); // vtable 0x00, slot 0: deleting destructor
    int refs_guess;                        // 0x04
};

struct FfRefHolder_guess
{
    int pad_0000;
    int fd_guess;                    // 0x04, CRT file handle (from FUN_004f96d0)
    char pad_0008[0x2010];
    FfRefObject_guess *object_guess; // 0x2018

    void DecrementReferenceCount_guess();
};

// Layout guessed from the asm of FUN_004f96d0 only: owns one heap FfRefHolder_guess.
struct FfResourceOwner_guess
{
    FfRefHolder_guess *holder_guess; // 0x00

    void releaseResource_guess();
};

typedef void (WINAPI *InitializeCriticalSection_fn)(LPCRITICAL_SECTION);

// FUNCTION: EQMAIN 0x1000E890
// Initializes shared front-end synchronization; the activity it protects is unclear.
// Cloned from trilogy FfSoundLock_guess::InitializeSoundCriticalSection_guess 0x004F8D50 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall; returns this. 0x10047E54 is a "not yet initialised" flag, 0x1004CEE0 the CRITICAL_SECTION.
// InitializeCriticalSection is called through this image's import slot 0x100400A8, not by name.
FfSoundLock_guess *FfSoundLock_guess::InitializeSoundCriticalSection_guess()
{
    unsigned char temp2 = EQ_GLOBAL(u8, 0x10047E54);
    unsigned char & temp = temp2;
    if (temp != 0)
    {
        EQ_GLOBAL(InitializeCriticalSection_fn, 0x100400A8)(&EQ_GLOBAL(CRITICAL_SECTION, 0x1004CEE0));
        EQ_GLOBAL(u8, 0x10047E54) = 0;
    }
    return this;
}

// FUNCTION: EQMAIN 0x1000f1d0
// Closes a resource's file handle and releases its share of the shared object.
// Cloned from trilogy FfResourceOwner_guess::releaseResource_guess 0x004F96D0 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall. Closes the holder's file handle, destroys (0x1000F210) and frees the holder, clears the pointer.
void FfResourceOwner_guess::releaseResource_guess()
{
    FfRefHolder_guess *holder;

    if (holder_guess != 0)
    {
        EQ_CDECL(int, 0x10034B55, (int))(holder_guess->fd_guess);
        holder = holder_guess;
        if (holder != 0)
        {
            EQ_METHOD(0, void, 0x1000F210, (FfRefHolder_guess *), holder)();
            EQ_CDECL(void, 0x10033800, (void *))(holder);
        }
        holder_guess = 0;
    }
}

// FUNCTION: EQMAIN 0x1000f210
// Releases one owner's share of a resource object, disposing of it when its last reference is gone.
// Cloned from trilogy FfRefHolder_guess::DecrementReferenceCount_guess 0x004F9710 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall. Drops one reference of the object at 0x2018 and deletes it at zero; the pointer is not cleared.
void FfRefHolder_guess::DecrementReferenceCount_guess()
{
    if (object_guess != 0)
    {
        FfRefObject_guess *object = object_guess;
        int remaining = object->refs_guess - 1;
        object->refs_guess = remaining;
        if (remaining == 0 && object != 0)
            object->destroy_guess(1);
    }
}
