// CFLAGS: /O2
// unit_10052D00.c - eqgfx_dx7.dll, the address run 0x10052D00..0x10052DC0 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

// Local layout guess: the record's first dword is the byte offset of its data, which begins 0xc bytes further
// in (no include/ struct has this layout).
typedef struct PackFsDataOffsetRecord_guess
{
    int dataOffset; // 0x0
} PackFsDataOffsetRecord_guess;

// Local layout guess: a fast-file handle whose first dword points at its read-ahead buffer, and the buffer's
// current position in the file at +0x200c (the buffer end is at +0x2008). Same shape as this image's packfs
// clones (PackFileSystem_guess/PackFSBuffer_guess in src/eqgfx/ffcreate2/ff/); no include/ struct has it.
typedef struct PackFsBuffer_guess
{
    char pad_0000[0x200c];
    unsigned int pos_guess; // 0x200c
} PackFsBuffer_guess;

typedef struct PackFsFile_guess
{
    PackFsBuffer_guess *buffer_guess; // 0x00
} PackFsFile_guess;

// FUNCTION: EQGFX 0x10052d00
// Provides the current read location while loading game data.
// __fastcall (the file handle arrives in ECX, no stack args; callers read EAX). Returns the position the
// read-ahead buffer is at inside the file.
int __fastcall getFilePosition_guess(PackFsFile_guess *file)
{
    return file->buffer_guess->pos_guess;
}

// FUNCTION: EQGFX 0x10052dc0
// Returns a pointer to the pack-archive entry the iterator is on (stored pointer + 0xC).
// __fastcall (the record pointer arrives in ECX, no stack args; callers read EAX). Returns the record's data
// offset plus the 0xc header bytes that precede the data.
int __fastcall getDataOffset_guess(PackFsDataOffsetRecord_guess *record)
{
    return record->dataOffset + 0xc;
}
