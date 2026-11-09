// Shared declarations for the size and speed optimized fast-file objects.
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

#define FF_CREATE2_ENTRY_COUNT 0x2D0

struct FFCreate2Lookup_guess
{
    char padding_0000[0x0C];
    void *entry_by_index[FF_CREATE2_ENTRY_COUNT];
    void *collection_resources;

    int ProcessIndexedDescriptor_guess(unsigned int *descriptor);
    void InvokeSelectedEntryMethods_guess(int index);
    int InvokeIndexedEntryMethodWithDescriptor_guess(unsigned int *descriptor, int context);
};

// Layouts are guesses from the asm of FfVerify_guess::GetField200c_guess only; no header defines them yet.
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

// Layouts guessed from the asm of FfSharedRef_guess::FfSharedRef_guess only: a refcounted pointer plus one extra dword.
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
    ~FfSharedRef_guess()
    {
        if (shared_guess != 0)
            EQ_METHOD(0, void, 0x004C4813, (FfShared_guess *), shared_guess)();
    }
};

// Layouts guessed from the asm of PackFSData's constructor only: a 0x10-byte shared block (vtable
// 0x005360dc, refcount, data pointer, length), and the object holding one pointer to such a block.
struct PackFSShared_guess
{
    int vtable_guess;       // 0x00
    int refs_guess;         // 0x04
    void *data_guess;       // 0x08
    unsigned int len_guess; // 0x0c

    PackFSShared_guess(void *data, unsigned int len)
    {
        refs_guess = 0;
        data_guess = data;
        len_guess = len;
        vtable_guess = 0x005360dc;
    }
};

// One entry of the 0x40-entry {file, line} table at 0x006eff58.
struct PackFSFileLine_guess
{
    LPCSTR file_guess; // 0x00
    int line_guess;    // 0x04
};

struct PackFSData_guess
{
    PackFSShared_guess *shared_guess; // 0x00

    PackFSData_guess(unsigned int len);
};

#define WAVE_SEARCH_CHUNK 0x400

// Layouts guessed from the asm of LoadWaveFileData_guess only.
struct WavePackReader_guess
{
    void *buffer_guess; // 0x00

    WavePackReader_guess()
    {
        buffer_guess = 0;
    }
    ~WavePackReader_guess()
    {
        EQ_METHOD(0, void, 0x004F9550, (WavePackReader_guess *), this)();
    }
};

struct WavePackLoader_guess
{
};

struct WaveFileHeader_guess
{
    unsigned char format_guess[0x10]; // 0x00
    short terminator_guess;           // 0x10
};

struct WaveFileData_guess
{
    WaveFileHeader_guess header_guess; // 0x00
    void *pack_guess;                  // 0x14
    int dataLength_guess;              // 0x18
    int dataStart_guess;               // 0x1c
    int field20_guess;                 // 0x20
};

struct WaveFile_guess
{
    WaveFileData_guess *data_guess; // 0x00

    char LoadWaveFileData_guess(FfSharedRef_guess source);
};

// Layout guessed from the asm of FfHasOwner_guess::ReleaseOwner_guess only: embeds a resource owner at 0x14.
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

// Layouts guessed from the asm of FfRefHolder_guess::DecrementReferenceCount_guess only: a refcounted object with a virtual destructor.
struct FfRefObject_guess
{
    virtual void destroy_guess(int flags); // vtable 0x00, slot 0: deleting destructor
    int refs_guess;                        // 0x04
};

struct FfRefHolder_guess
{
    int pad_0000;
    int fd_guess;                    // 0x04, CRT file handle (from FfResourceOwner_guess::releaseResource_guess)
    char pad_0008[0x2010];
    FfRefObject_guess *object_guess; // 0x2018

    void DecrementReferenceCount_guess();
};

// Layout guessed from the asm of FfResourceOwner_guess::releaseResource_guess only: owns one heap FfRefHolder_guess.
struct FfResourceOwner_guess
{
    FfRefHolder_guess *holder_guess; // 0x00

    FfResourceOwner_guess *CreateFrom_guess(void **param_2);
    void releaseResource_guess();
};

// Layouts guessed from the asm of PackFileSystem_ReadPackFileData only: `this` holds one pointer to a
// 0x201c-byte read-ahead object, refilled through its vtable slot 0.
struct PackFSOwner_guess
{
    char pad_0000[8];  // 0x00
    LPCSTR name_guess; // 0x08
    int flag_guess;    // 0x0c
};

struct PackFSBuffer_guess
{
    int *vtable_guess;                // 0x00
    char pad_0004[4];                 // 0x04
    unsigned char data_guess[0x2000]; // 0x08
    unsigned int end_guess;           // 0x2008
    unsigned int pos_guess;           // 0x200c
    int offset_guess;                 // 0x2010
    char pad_2014[4];                 // 0x2014
    PackFSOwner_guess *owner_guess;   // 0x2018
};

// __fastcall with a dummy EDX, as EQ_THISCALL: `this` in ECX, the callee pops nothing.
typedef int (__fastcall *PackFSRefill_guess)(PackFSBuffer_guess *self, int edx);

struct PackFileSystem_guess
{
    PackFSBuffer_guess *buffer_guess; // 0x00

    int ReadPackFileData_guess(unsigned int *dest, unsigned int count);
};

