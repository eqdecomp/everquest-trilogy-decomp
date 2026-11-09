#include "verify_shared.h"

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x100219be
// Creates shared byte storage for file data.
// Cloned from trilogy PackFSData_PackFSData_guess 0x004DD808 (the same original code: BinDiff sibling, names/siblings/).
PackFSData_guess::PackFSData_guess(unsigned int len)
{
    void *data;
    PackFSShared_guess *shared;
    DWORD idx;
    LPCSTR file;
    LPCSTR msg;

    shared_guess = 0;
    if (len != 0)
    {
        data = new unsigned char[len];
        idx = EQ_GLOBAL(DWORD, 0x100FC014);
        file = (LPCSTR)0x10087048;
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[idx].file_guess = file;
            EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[idx].line_guess = 0x1b0;
            EQ_GLOBAL(DWORD, 0x100FC014) = idx + 1;
        }
        msg = (LPCSTR)0x10087020;
        if (data == 0)
        {
            EQ_CDECL(void, 0x100549A0, (void *, LPCSTR, LPCSTR))(data, msg, (LPCSTR)0x10087010);
        }

        shared = new PackFSShared_guess(data, len);
        idx = EQ_GLOBAL(DWORD, 0x100FC014);
        if (idx < 0x40)
        {
            EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[idx].file_guess = file;
            EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[idx].line_guess = 0x1b2;
            EQ_GLOBAL(DWORD, 0x100FC014) = idx + 1;
        }
        if (shared == 0)
        {
            EQ_CDECL(void, 0x100549A0, (void *, LPCSTR, LPCSTR))(shared, msg, (LPCSTR)0x10086FF8);
        }
        shared_guess = shared;
        shared->refs_guess++;
    }
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10021d3b
// Keeps a shared resource available to another owner.
// Cloned from trilogy FfSharedRef_guess::FfSharedRef_guess 0x004E8647 (the same original code: BinDiff sibling, names/siblings/).
// __thiscall copy constructor.
FfSharedRef_guess::FfSharedRef_guess(const FfSharedRef_guess &src)
{
    shared_guess = src.shared_guess;
    if (shared_guess != 0)
    {
        shared_guess->refs_guess++;
    }
    extra_guess = src.extra_guess;
}

#pragma optimize("", on)
#pragma function(strcpy, strlen)
// FUNCTION: EQGFX 0x10052a90
// Reads requested bytes from the current position in a packed file.
int PackFileSystem_guess::PackFileSystem_ReadPackFileData_guess(unsigned int *dest, unsigned int count)
{
    unsigned int available;
    unsigned int remainingToCopy;
    unsigned int bytesThisIteration;
    unsigned int wordsToCopy;
    unsigned int tailBytes;
    unsigned int *source;
    unsigned int *wordCursor;
    unsigned int *cursor;
    int copied;
    int refillResult;
    LPCSTR name;
    LPCSTR sourceFile;
    DWORD lineIndex;
    PackFSOwner_guess *owner;

    available = buffer_guess->end_guess;
    remainingToCopy = buffer_guess->pos_guess;
    if (available <= remainingToCopy)
        return 0;
    available = available - remainingToCopy;
    remainingToCopy = count;
    if (available < count)
        remainingToCopy = available;
    copied = 0;
    cursor = dest;
    while (remainingToCopy != 0)
    {
        refillResult = buffer_guess->offset_guess;
        bytesThisIteration = PACKFS_READ_AHEAD_SIZE - refillResult;
        if (bytesThisIteration != 0)
        {
            if (remainingToCopy < bytesThisIteration)
                bytesThisIteration = remainingToCopy;
            source = (unsigned int *)(buffer_guess->data_guess + refillResult);
            wordCursor = cursor;
            for (wordsToCopy = bytesThisIteration >> 2; wordsToCopy != 0; wordsToCopy = wordsToCopy - 1)
            {
                *wordCursor = *source;
                source = source + 1;
                wordCursor = wordCursor + 1;
            }
            remainingToCopy = remainingToCopy - bytesThisIteration;
            for (tailBytes = bytesThisIteration & 3; tailBytes != 0; tailBytes = tailBytes - 1)
            {
                *(unsigned char *)wordCursor = *(unsigned char *)source;
                source = (unsigned int *)((int)source + 1);
                wordCursor = (unsigned int *)((int)wordCursor + 1);
            }
            copied = copied + bytesThisIteration;
            buffer_guess->offset_guess = buffer_guess->offset_guess + bytesThisIteration;
            buffer_guess->pos_guess = buffer_guess->pos_guess + bytesThisIteration;
            cursor = (unsigned int *)((int)cursor + bytesThisIteration);
        }
        if (remainingToCopy != 0)
        {
            refillResult = ((PackFSRefill_guess)buffer_guess->vtable_guess[0])(buffer_guess, 0);
            lineIndex = EQ_GLOBAL(DWORD, 0x100FC014);
            sourceFile = (LPCSTR)0x100a3b68;
            if (lineIndex < 0x40)
            {
                EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[lineIndex].file_guess = sourceFile;
                EQ_ARRAY(PackFSFileLine_guess, 0x100FBC10)[lineIndex].line_guess = 0x371;
                EQ_GLOBAL(DWORD, 0x100FC014) = lineIndex + 1;
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
            EQ_CDECL(void, 0x100549A0, (unsigned int, LPCSTR, LPCSTR))(
                ((unsigned int)name & 0xffffff00) |
                (refillResult <= PACKFS_READ_AHEAD_SIZE), (LPCSTR)0x100a3c18, name);
            buffer_guess->offset_guess = 0;
        }
    }
    return copied;
}
