// CFLAGS: /O2
#include "verify_shared.h"

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10052a20
// Ends an opened file resource and releases its owner.
// Cloned from trilogy FfResourceOwner_guess::releaseResource_guess 0x004F96D0 (the same original code: BinDiff sibling, names/siblings/).
// @early-stop: /O1 folds the descriptor load into PUSH and uses POP ECX for both cdecl stack cleanups.
// __thiscall. Closes the holder's file handle, destroys (0x10052A60) and frees the holder, clears the pointer.
void FfResourceOwner_guess::releaseResource_guess()
{
    FfRefHolder_guess *holder;

    if (holder_guess != 0)
    {
        EQ_CDECL(int, 0x1006AA82, (int))(holder_guess->fd_guess);
        holder = holder_guess;
        if (holder != 0)
        {
            EQ_METHOD(0, void, 0x10052A60, (FfRefHolder_guess *), holder)();
            EQ_CDECL(void, 0x10050CBB, (void *))(holder);
        }
        // Preserve the original's write-only clear rather than a read-modify-write.
        *(FfRefHolder_guess *volatile *)&holder_guess = 0;
    }
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10052a60
// Destroys the shared object once no references remain.
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

#pragma optimize("", on)
#pragma function(strcpy, strlen)


#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10052bc0
// Moves the packed-file read position by a relative offset, clamped to the file bounds.
int __fastcall seekRelative_guess(EQGFXFileHandle_guess *self, int unusedEdx, int positionDelta)
{
    // vtable slot 0 of the file, a __thiscall without arguments
    struct EQGFXFileRefill_guess
    {
        virtual int Refill_guess() = 0;
    };
    EQGFXFileState_guess *file;
    unsigned int oldPosition;
    int newPosition;
    int blockNew;
    int blockOld;
    unsigned int offset;
    DWORD lineIndex;
    EQGFXFileOwner_guess *owner;
    LPCSTR name;
    long tell;

    file = self->file;
    oldPosition = file->position;
    newPosition = oldPosition + positionDelta;
    if (newPosition >= (int)file->endPosition)
    {
        self->file->position = file->endPosition;
        return self->file->position;
    }
    if (newPosition < 0)
    {
        newPosition = 0;
    }
    offset = newPosition & 0x1fff;
    blockNew = newPosition >> 13;
    blockOld = oldPosition >> 13;
    if (blockNew == blockOld)
    {
        self->file->bufferOffset = offset;
        self->file->position = newPosition;
    }
    else
    {
        if (blockNew < blockOld)
        {
            EQ_CDECL(long, 0x1006AB62, (int, long, int))(file->descriptor, file->blockStart, 0);
            blockOld = 0;
        }
        blockOld++;
        if (blockOld < blockNew)
        {
            blockNew -= blockOld;
            do
            {
                EQ_CDECL(int, 0x1006A844, (int, void *, unsigned int))(self->file->descriptor, &positionDelta, 4);
                EQ_CDECL(long, 0x1006AB62, (int, long, int))(self->file->descriptor, positionDelta + 4, 1);
                blockNew = blockNew - 1;
            } while (blockNew != 0);
        }
        lineIndex = EQ_GLOBAL(DWORD, 0x100FC014);
        if (lineIndex < 0x40)
        {
            EQ_ARRAY(EQGFXFileLine_guess, 0x100FBC10)[lineIndex].file = (LPCSTR)0x100a3b68;
            /* fake match: the line column is its own symbol (0x100fbc14) in the original, no shared lea */
            EQ_ARRAY(EQGFXFileLine_guess, 0x100FBC14)[lineIndex].file = (LPCSTR)0x3af;
            EQ_GLOBAL(DWORD, 0x100FC014) = lineIndex + 1;
        }
        owner = self->file->owner;
        if ((owner == 0) || ((unsigned int)owner->enabled <= 0))
        {
            name = 0;
        }
        else
        {
            name = owner->name;
        }
        EQ_CDECL(void, 0x100549A0, (bool, LPCSTR, LPCSTR))(((EQGFXFileRefill_guess *)self->file)->Refill_guess() != 0, (LPCSTR)0x100a3c18, name);
        self->file->position = newPosition;
        self->file->bufferOffset = offset;
    }
    tell = EQ_CDECL(long, 0x1006A833, (int))(self->file->descriptor);
    self->file->tellPosition = tell;
    return self->file->position;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x100548b0
// Writes timestamped, numbered diagnostic messages to dbg.txt.
#pragma intrinsic(strcpy, strlen)
int __cdecl fdebug_guess(const char *format, ...)
{
    struct DebugBuffers_guess
    {
        long currentTime;
        char timeString[0x1c];
        char timePadding[4];
        char message[0x800];
        char output[0x800];
    } buffers;
    char *ctimeString;
    unsigned int counter;
    unsigned int length;
    va_list args;
    FILE *file;

    va_start(args, format);
    EQ_CDECL(int, 0x1006A0D9, (char *, const char *, va_list))(
        buffers.message, format, args);
    va_end(args);

    buffers.currentTime = EQ_CDECL(long, 0x10069FE4, (long *))(0);
    ctimeString = EQ_CDECL(char *, 0x1006A0C0, (const long *))(&buffers.currentTime);

    strcpy(buffers.timeString, ctimeString);
    length = strlen(buffers.timeString);
    if (buffers.timeString[length - 1] == '\n')
        buffers.timeString[length - 1] = '\0';

    counter = EQ_GLOBAL(unsigned int, 0x100FC01C) & 0xffff;
    ++EQ_GLOBAL(unsigned short, 0x100FC01C);
    EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
        buffers.output, (const char *)0x100a367c, buffers.timeString, counter,
        buffers.message);

    file = EQ_CDECL(FILE *, 0x10068412, (const char *, const char *))(
        (const char *)0x100a3674, (const char *)0x10086170);
    if (file != 0)
    {
        EQ_CDECL(int, 0x100683A5, (FILE *, const char *, ...))(
            file, (const char *)0x100a3670, buffers.output);
        EQ_CDECL(int, 0x10068328, (FILE *))(file);
    }
    return 0;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x100549A0
// Reports failed verification checks with source location and terminates the client.
// @early-stop: The EBP frame, table-load schedule, and stack-relative call operands still differ.
/* fake match: Preserve the original terminal __exit path; the fixed-address call type has no noreturn annotation. */
#pragma intrinsic(strlen, strcpy)
void __cdecl _verify_guess(char status, const char * volatile format, ...)
{
    char message[0x400];
    char *destination;
    const char *currentFormat;
    const char *stackFile;
    int stackLine;
    va_list args;

    if (status == 0)
    {
        if (EQ_GLOBAL(EqgfxVerifyHook_guess, 0x100FC018) != 0)
            EQ_GLOBAL(EqgfxVerifyHook_guess, 0x100FC018)();
        stackFile = EQ_ARRAY(EqgfxVerifyEntry_guess, 0x100FBC08)
            [EQ_GLOBAL(int, 0x100FC014)].file;
        stackLine = EQ_ARRAY(EqgfxVerifyEntry_guess, 0x100FBC08)
            [EQ_GLOBAL(int, 0x100FC014)].line;
        EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(
            message, (const char *)0x100a50a4, stackLine, stackFile);

        destination = message + strlen(message);
        currentFormat = format;
        if (currentFormat != 0)
        {
            va_start(args, format);
            EQ_CDECL(int, 0x1006A0D9, (char *, const char *, va_list))(
                destination, currentFormat, args);
        }
        else
        {
            strcpy(destination, (const char *)0x100a5078);
        }
        EQ_CDECL(int, 0x100548B0, (const char *, ...))(message);
        EQ_CDECL(void, 0x10054A70, (const char *))(message);
        EQ_CDECL(void, 0x1006AF92, (int))(0xbaca);
        for (;;)
        {
        }
    }
    else
        --EQ_GLOBAL(int, 0x100FC014);
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10054a70
// Writes a supplied diagnostic to the log and debugger output.
// Cloned from trilogy logErrorToFileAndDebug_guess 0x004F83E0 (the same original code: BinDiff sibling, names/siblings/).
// __cdecl. Opens the file named at 0x100FBE14 (mode at 0x10086170), makes it unbuffered,
// writes message as the fprintf format (bug kept) and echoes it to the debugger.
void logErrorToFileAndDebug_guess(LPCSTR message)
{
    FILE *file;

    file = EQ_CDECL(FILE *, 0x10068412, (const char *, const char *))((const char *)0x100fbe14,
                                                                     (const char *)0x10086170);
    if (file != 0)
    {
        EQ_CDECL(int, 0x1006B1B6, (FILE *, char *, int, size_t))(file, 0, 4, 0);
        if (message != 0)
        {
            EQ_CDECL(int, 0x100683A5, (FILE *, const char *))(file, message);
            OutputDebugStringA(message);
        }
        EQ_CDECL(int, 0x10068328, (FILE *))(file);
    }
}

