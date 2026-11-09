// CFLAGS: /O2
#include "verify_shared.h"

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f8220
// Writes a timestamped, numbered diagnostic message to dbg.txt.
// ORIGINAL FILE: ffcreate2/ff/verify.cpp (from Mac debug map)
// @early-stop: the EBP frame, frame-relative operands, and newline trim/store form still differ.
#pragma intrinsic(strcpy, strlen)
int __cdecl fdebug(const char *format, ...)
{
    struct DebugBuffers_guess
    {
        long currentTime;
        char timeString[0x1c];
        char timePadding[4]; /* fake match: preserve the original scratch-buffer offsets */
        char message[0x800];
        char output[0x800];
    } buffers;
    char *ctimeString;
    unsigned int counter;
    unsigned int length;
    va_list args;
    FILE *file;

    va_start(args, format);
    EQ_CDECL(int, 0x00524174, (char *, const char *, va_list))(
        buffers.message, format, args);
    va_end(args);

    buffers.currentTime = EQ_CDECL(long, 0x005213E9, (long *))(0);
    ctimeString = EQ_CDECL(char *, 0x005213D0, (const long *))(&buffers.currentTime);

    strcpy(buffers.timeString, ctimeString);
    length = strlen(buffers.timeString);
    if (buffers.timeString[length - 1] == '\n')
        buffers.timeString[length - 1] = '\0';

    counter = EQ_GLOBAL(unsigned int, 0x006F0364) & 0xffff;
    ++EQ_GLOBAL(unsigned short, 0x006F0364);
    EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
        buffers.output, (const char *)0x0056a30c, buffers.timeString, counter,
        buffers.message);

    file = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))(
        (const char *)0x0056a33c, (const char *)0x00564e48);
    if (file != 0)
    {
        EQ_CDECL(int, 0x00524138, (FILE *, const char *, ...))(
            file, (const char *)0x0056a308, buffers.output);
        EQ_CDECL(int, 0x00521FBB, (FILE *))(file);
    }
    return 0;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f8310
// Reports the source file and line for a failed verification, logs the error, then exits the client.
// ORIGINAL FILE: ffcreate2/ff/verify.cpp (from Mac debug map)
// __cdecl. With status != 0 it only pops one entry of the file/line stack (0x006f035c). Otherwise it calls the
// hook at 0x006f0360 if set, formats "..." from the top entry of the stack at 0x006eff50
// (file, line pairs), appends the printf-style message (or a default one when format is 0), logs it and exits.
#pragma intrinsic(strlen, strcpy)
/* fake match: favor speed gives the original's ESP-based frame; /O1 would add an EBP frame for the buffer. */
#pragma optimize("t", on)
void __cdecl _verify(char status, const char *format, ...)
{
    char message[0x400];
    char *destination;
    const char *currentFormat;
    va_list args;

    if (status == 0)
    {
        if (EQ_GLOBAL(VerifyHook_guess, 0x006F0360) != 0)
            EQ_GLOBAL(VerifyHook_guess, 0x006F0360)();
        EQ_CDECL(int, 0x005224FB, (char *, const char *, ...))(
            message, (const char *)0x00580d20,
            EQ_ARRAY(FfVerifyEntry_guess, 0x006EFF50)[EQ_GLOBAL(int, 0x006F035C)].line,
            EQ_ARRAY(FfVerifyEntry_guess, 0x006EFF50)[EQ_GLOBAL(int, 0x006F035C)].file);

        destination = message + strlen(message);
        currentFormat = format;
        if (currentFormat != 0)
        {
            va_start(args, format);
            EQ_CDECL(int, 0x00524174, (char *, const char *, va_list))(
                destination, currentFormat, args);
        }
        else
        {
            strcpy(destination, (const char *)0x00580cf4);
        }
        EQ_CDECL(int, 0x004F8220, (const char *, ...))(message);
        EQ_CDECL(void, 0x004F83E0, (const char *))(message);
        /* fake match: _exit never returns, so a callee-pop type and __assume(0) drop its dead cleanup. */
        EQ_STDCALL(void, 0x00522DBE, (int))(0xbaca);
        __assume(0);
    }
    else
        --EQ_GLOBAL(int, 0x006F035C);
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f83e0
// Writes an error message to the client log and debugger output.
// __cdecl. Opens the file named at 0x006F015C (mode at 0x00564E48), makes it unbuffered,
// writes message as the fprintf format (bug kept) and echoes it to the debugger.
void logErrorToFileAndDebug_guess(LPCSTR message)
{
    FILE *file;

    file = EQ_CDECL(FILE *, 0x005223BC, (const char *, const char *))((const char *)0x006f015c,
                                                                     (const char *)0x00564e48);
    if (file != 0)
    {
        EQ_CDECL(int, 0x005250A8, (FILE *, char *, int, size_t))(file, 0, 4, 0);
        if (message != 0)
        {
            EQ_CDECL(int, 0x00524138, (FILE *, const char *))(file, message);
            OutputDebugStringA(message);
        }
        EQ_CDECL(int, 0x00521FBB, (FILE *))(file);
    }
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f8c00
// Copies an object's stored state and backing buffer from another instance.
// @early-stop: Descriptor memory push, three field-copy registers, and cdecl stack cleanup differ under /O1.
FfVerifyCopy_guess *FfVerifyCopy_guess::CopyFrom_guess(FfVerifyCopy_guess *source)
{
    struct CopyBuffer_guess
    {
        unsigned int words[0x800];
    };
    int duplicate = EQ_CDECL(int, 0x00525157, (int))(*(int *)((char *)source + 4));
    *(int *)((char *)this + 4) = duplicate;
    *(int *)((char *)this + 0x2008) = *(int *)((char *)source + 0x2008);
    *(int *)((char *)this + 0x200c) = *(int *)((char *)source + 0x200c);
    *(int *)((char *)this + 0x2020) = *(int *)((char *)source + 0x2020);
    *(int *)((char *)this + 0x2010) = *(int *)((char *)source + 0x2010);
    /* fake match: reuse the last copied value in the unused EDX slot so the
       thiscall wrapper preserves the original's EDX load and delayed store. */
    int referenceValue = *(int *)((char *)source + 0x2014);
    *(int *)((char *)this + 0x2014) = referenceValue;
    EQ_THISCALL(void, 0x004C489A, (void *, int, void *))((char *)this + 0x2018,
                                                         referenceValue,
                                                         (char *)source + 0x2018);
    CopyBuffer_guess &destinationBuffer = *(CopyBuffer_guess *)((char *)this + 8);
    const CopyBuffer_guess &sourceBuffer = *(CopyBuffer_guess *)((char *)source + 8);
    destinationBuffer = sourceBuffer;
    return this;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f8d50
// __thiscall; returns this. 0x00580DB0 is a "not yet initialised" flag, 0x006F0368 the CRITICAL_SECTION.
FfSoundLock_guess *FfSoundLock_guess::InitializeSoundCriticalSection_guess()
{
    unsigned char temp2 = EQ_GLOBAL(u8, 0x00580DB0);
    unsigned char & temp = temp2;
    if (temp != 0)
    {
        InitializeCriticalSection(&EQ_GLOBAL(CRITICAL_SECTION, 0x006F0368));
        EQ_GLOBAL(u8, 0x00580DB0) = 0;
    }
    return this;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f9550
// __thiscall thunk: a single JMP to FfResourceOwner_guess::releaseResource_guess (0x004F96D0).
void __fastcall releaseResourceThunk_guess(FfResourceOwner_guess *self)
{
    EQ_METHOD(0, void, 0x004F96D0, (FfResourceOwner_guess *), self)();
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f9560
// Creates a resource holder by copying packed-file data or a raw-file resource.
FfResourceOwner_guess *FfResourceOwner_guess::CreateFrom_guess(void **sourcePointer)
{
    void *source;
    int castObject;
    unsigned int *holder;
    char *allocationExpression;
    DWORD idx;

    EQ_METHOD(0, void, 0x004F96D0, (FfResourceOwner_guess *), this)();
    castObject = (int)EQ_CDECL(void *, 0x00525A44,
        (void *, int, void *, void *, int))(
            *sourcePointer, 0, (void *)0x00580f08,
            (void *)0x00580f28, 0);
    if (castObject == 0)
    {
        holder = (unsigned int *)EQ_CDECL(void *, 0x005218C6,
            (unsigned int))(0x2024);
        if (holder == 0)
        {
            holder = 0;
        }
        else
        {
            source = *sourcePointer;
            EQ_METHOD(1, void, 0x004C4823, (void *, int), (char *)holder + 0x2018)(0);
            *(unsigned int *)holder = 0x005360ec;
            EQ_METHOD(1, void, 0x004F8C00, (void *, void *), holder)(source);
        }
        idx = EQ_GLOBAL(DWORD, 0x006F035C);
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess =
                (LPCSTR)0x00580e58;
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0x278;
            EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
        }
        if (holder != 0)
            goto finish;
        allocationExpression = (char *)0x00580ec0;
    }
    else
    {
        holder = (unsigned int *)EQ_CDECL(void *, 0x005218C6,
            (unsigned int))(0x2024);
        if (holder == 0)
        {
            holder = 0;
        }
        else
        {
            EQ_METHOD(1, void, 0x004C4823, (void *, int), (char *)holder + 0x2018)(0);
            *(unsigned int *)holder = 0x005360ec;
            EQ_METHOD(1, void, 0x004F8C00, (void *, void *), holder)((void *)castObject);
            *(unsigned int *)holder = 0x005360e4;
        }
        idx = EQ_GLOBAL(DWORD, 0x006F035C);
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess =
                (LPCSTR)0x00580e58;
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0x274;
            EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
        }
        if (holder != 0)
            goto finish;
        allocationExpression = (char *)0x00580ee4;
    }
    EQ_CDECL(void, 0x004F8310, (bool, LPCSTR, LPCSTR))(
        0, (LPCSTR)0x00570968, allocationExpression);
finish:
    *(FfRefHolder_guess **)this = (FfRefHolder_guess *)holder;
    return this;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f96d0
// @early-stop: /O1 folds the descriptor load into PUSH and uses POP ECX for both cdecl stack cleanups.
// __thiscall. Closes the holder's file handle, destroys (0x004F9710) and frees the holder, clears the pointer.
void FfResourceOwner_guess::releaseResource_guess()
{
    FfRefHolder_guess *holder;

    if (holder_guess != 0)
    {
        EQ_CDECL(int, 0x005254AE, (int))(holder_guess->fd_guess);
        holder = holder_guess;
        if (holder != 0)
        {
            EQ_METHOD(0, void, 0x004F9710, (FfRefHolder_guess *), holder)();
            EQ_CDECL(void, 0x005218BB, (void *))(holder);
        }
        // Preserve the original's write-only clear rather than a read-modify-write.
        *(FfRefHolder_guess *volatile *)&holder_guess = 0;
    }
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f9710
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

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f9730
// Supplies bytes from a packed file to resource loaders.
// ORIGINAL FILE: packfs.cpp (from the trace string at 0x00580e58; no Mac map entry)
// __thiscall. Copies up to `count` bytes from the read-ahead buffer to `dest` (clamped to what is left),
// refilling it through vtable slot 0 when it runs dry. Returns the bytes copied, 0 at the end.
int PackFileSystem_guess::ReadPackFileData_guess(unsigned int *dest, unsigned int count)
{
    unsigned int chunkBytes;
    unsigned int loopCount;
    unsigned int remainingBytes;
    unsigned int *sourceCursor;
    unsigned int *destinationCursor;
    unsigned int *cursor;
    int copied;
    int value;
    LPCSTR name;
    LPCSTR file;
    DWORD idx;
    PackFSOwner_guess *owner;

    chunkBytes = buffer_guess->end_guess;
    remainingBytes = buffer_guess->pos_guess;
    if (chunkBytes <= remainingBytes)
        return 0;
    chunkBytes = chunkBytes - remainingBytes;
    remainingBytes = count;
    if (chunkBytes < count)
        remainingBytes = chunkBytes;
    copied = 0;
    cursor = dest;
    while (remainingBytes != 0)
    {
        value = buffer_guess->offset_guess;
        chunkBytes = 0x2000 - value;
        if (chunkBytes != 0)
        {
            if (remainingBytes < chunkBytes)
                chunkBytes = remainingBytes;
            sourceCursor = (unsigned int *)(buffer_guess->data_guess + value);
            destinationCursor = cursor;
            for (loopCount = chunkBytes >> 2; loopCount != 0; loopCount = loopCount - 1)
            {
                *destinationCursor = *sourceCursor;
                sourceCursor = sourceCursor + 1;
                destinationCursor = destinationCursor + 1;
            }
            remainingBytes = remainingBytes - chunkBytes;
            for (loopCount = chunkBytes & 3; loopCount != 0; loopCount = loopCount - 1)
            {
                *(unsigned char *)destinationCursor = *(unsigned char *)sourceCursor;
                sourceCursor = (unsigned int *)((int)sourceCursor + 1);
                destinationCursor = (unsigned int *)((int)destinationCursor + 1);
            }
            copied = copied + chunkBytes;
            buffer_guess->offset_guess = buffer_guess->offset_guess + chunkBytes;
            buffer_guess->pos_guess = buffer_guess->pos_guess + chunkBytes;
            cursor = (unsigned int *)((int)cursor + chunkBytes);
        }
        if (remainingBytes != 0)
        {
            value = ((PackFSRefill_guess)buffer_guess->vtable_guess[0])(buffer_guess, 0);
            idx = EQ_GLOBAL(DWORD, 0x006F035C);
            file = (LPCSTR)0x00580e58;
            if (idx < 0x40)
            {
                EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess = file;
                EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].line_guess = 0x371;
                EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
            }
            owner = buffer_guess->owner_guess;
            if ((owner == 0) || (owner->flag_guess == 0))
            {
                name = 0;
            }
            else
            {
                name = owner->name_guess;
            }
            EQ_CDECL(void, 0x004F8310, (bool, LPCSTR, LPCSTR))(value <= 0x2000, (LPCSTR)0x00580f98, name);
            buffer_guess->offset_guess = 0;
        }
    }
    return copied;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f9860
// Seeks within a packed resource as its contents are read.
// @early-stop: VC6 builds an EBP frame and spills `offset` (original is FPO with EBP as a register); arg pushes use mem operands
// __thiscall, 1 arg (callee pops 4), written as __fastcall with a dummy EDX (as PackFSRefill_guess). Moves
// the read position by `param_2` (clamped at 0): inside the current 0x2000-byte block only the offset
// changes; across blocks it seeks the file to the block base (when going back), skips the block links
// in between, then refills through vtable slot 0 and checks the result. Stores the tell at 0x2020 on every
// path except the early return past the end.
// Fields past PackFSBuffer_guess's 0x201c size (fd at 0x04, block base at 0x2014, tell at 0x2020) are
// reached by offset.
int __fastcall AdjustReadPositionByDelta_guess(PackFileSystem_guess *self, int edx, int param_2)
{
    // vtable slot 0 of the buffer, a __thiscall without arguments
    struct PackFSRefill2_guess
    {
        virtual int Refill_guess() = 0;
    };
    PackFSBuffer_guess *buf;
    unsigned int oldPos;
    int newPos;
    int blockNew;
    int blockOld;
    unsigned int offset;
    DWORD idx;
    PackFSOwner_guess *owner;
    LPCSTR name;
    long tell;

    buf = self->buffer_guess;
    oldPos = buf->pos_guess;
    newPos = oldPos + param_2;
    if (newPos >= (int)buf->end_guess)
    {
        self->buffer_guess->pos_guess = buf->end_guess;
        return self->buffer_guess->pos_guess;
    }
    if (newPos < 0)
    {
        newPos = 0;
    }
    offset = newPos & 0x1fff;
    blockNew = newPos >> 13;
    blockOld = oldPos >> 13;
    if (blockNew == blockOld)
    {
        self->buffer_guess->offset_guess = offset;
        self->buffer_guess->pos_guess = newPos;
    }
    else
    {
        if (blockNew < blockOld)
        {
            EQ_CDECL(long, 0x0052558E, (int, long, int))(*(int *)((char *)buf + 0x4), *(int *)((char *)buf + 0x2014), 0);
            blockOld = 0;
        }
        blockOld++;
        if (blockOld < blockNew)
        {
            blockNew -= blockOld;
            do
            {
                EQ_CDECL(int, 0x00525270, (int, void *, unsigned int))(*(int *)((char *)self->buffer_guess + 0x4), &param_2, 4);
                EQ_CDECL(long, 0x0052558E, (int, long, int))(*(int *)((char *)self->buffer_guess + 0x4), param_2 + 4, 1);
                blockNew = blockNew - 1;
            } while (blockNew != 0);
        }
        idx = EQ_GLOBAL(DWORD, 0x006F035C);
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF58)[idx].file_guess = (LPCSTR)0x00580e58;
            /* fake match: the line column is its own symbol (0x006eff5c) in the original, no shared lea */
            EQ_ARRAY(PackFSFileLine_guess, 0x006EFF5C)[idx].file_guess = (LPCSTR)0x3af;
            EQ_GLOBAL(DWORD, 0x006F035C) = idx + 1;
        }
        owner = self->buffer_guess->owner_guess;
        if ((owner == 0) || ((unsigned int)owner->flag_guess <= 0))
        {
            name = 0;
        }
        else
        {
            name = owner->name_guess;
        }
        EQ_CDECL(void, 0x004F8310, (bool, LPCSTR, LPCSTR))(((PackFSRefill2_guess *)self->buffer_guess)->Refill_guess() != 0, (LPCSTR)0x00580f98, name);
        self->buffer_guess->pos_guess = newPos;
        self->buffer_guess->offset_guess = offset;
    }
    tell = EQ_CDECL(long, 0x0052525F, (int))(*(int *)((char *)self->buffer_guess + 0x4));
    *(long *)((char *)self->buffer_guess + 0x2020) = tell;
    return self->buffer_guess->pos_guess;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f99a0
// __thiscall.
int FfVerify_guess::GetField200c_guess()
{
    return target_guess->field_200c_guess;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: TRILOGY 0x004f99b0
// Adjusts the supplied handle's read position; the higher-level purpose is unclear.
// __fastcall (handle in ECX, no stack args). Calls initializeCollectionObjectIfNull (0x004f9860) with the
// handle and -1 - the 0x2008 field of the object it points to; EDX holds the same value at the call.
void __fastcall initializeCollectionObjectFromHandle_guess(int *pack)
{
    int delta;

    delta = -1 - *(int *)(*pack + 0x2008); // 0x2008
    EQ_THISCALL(int, 0x004F9860, (int *, int, int))(pack, delta, delta);
}

