// Shared declarations for the size and speed optimized fast-file objects.
// eqgfx: the same original object as trilogy's src/ffcreate2/ff/verify.cpp (cloned functions). Address
// macros below were carried over: any trilogy address not yet replaced is wrong in this file.
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

#define EQGFX_SEEK_BLOCK_SHIFT 13
#define EQGFX_SEEK_BLOCK_MASK 0x1fff
#define EQGFX_SEEK_LINE_CAPACITY 0x40

struct EQGFXFileOwner_guess
{
    char pad_0000[8];
    LPCSTR name;
    int enabled;
};

struct EQGFXFileState_guess
{
    void **vtable;
    int descriptor;
    char pad_0008[0x2000];
    int endPosition;
    unsigned int position;
    unsigned int bufferOffset;
    long blockStart;
    EQGFXFileOwner_guess *owner;
    int pad_201c;
    long tellPosition;
};

struct EQGFXFileHandle_guess
{
    EQGFXFileState_guess *file;
};

struct EQGFXFileLine_guess
{
    LPCSTR file;
    DWORD line;
};

typedef int (__cdecl *EQGFXFileErrorCheck_guess)(LPCSTR, LPCSTR);
struct EqgfxVerifyEntry_guess
{
    const char *file;
    int line;
};

typedef void (__cdecl *EqgfxVerifyHook_guess)(void);

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

// Layouts guessed from the asm of PackFSData's constructor only: a 0x10-byte shared block (vtable
// 0x10076400, refcount, data pointer, length), and the object holding one pointer to such a block.
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
        vtable_guess = 0x10076400;
    }
};

// One entry of the 0x40-entry {file, line} table at 0x100fbc10.
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

#define PACKFS_READ_AHEAD_SIZE 0x2000

struct PackFileSystem_guess
{
    PackFSBuffer_guess *buffer_guess; // 0x00

    int PackFileSystem_ReadPackFileData_guess(unsigned int *dest, unsigned int count);
};

