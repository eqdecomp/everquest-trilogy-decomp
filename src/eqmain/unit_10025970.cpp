// unit_10025970.cpp - eqmain.dll, the address run 0x10025970..0x10027950 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown; tools/docs/EQMAIN.md).

#include "trilogy/eqaddr.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// FUNCTION: EQMAIN 0x10025970
// Releases the child storage owned by a front-end lookup record.
// __fastcall: the record comes in ECX, the callee pops 0 bytes, no return value. Destructor body: stores the
// vtable address 0x10041F80 at +0, frees each non-null entry of the pointer array at +0xC in index order (the
// count at +8 is re-read on every pass), then frees the block at +4. The array is skipped when the count or the
// array pointer is zero or the count is not positive.
void __fastcall UnitRelease_guess(unsigned int* record)
{
    void* block;
    int i;

    *record = 0x10041f80;
    if (record[2] != 0 && record[3] != 0 && (i = 0, (int)record[2] > 0))
    {
        do
        {
            block = ((void**)record[3])[i];
            if (block != 0)
                EQ_CDECL(void, 0x10033568, (void*))(block);
            i++;
        } while (i < (int)record[2]);
    }
    if ((void*)record[1] != 0)
        EQ_CDECL(void, 0x10033568, (void*))((void*)record[1]);
}

// FUNCTION: EQMAIN 0x100259C0
// Starts an empty front-end table object for later use.
// Stores the vtable address 0x10041F88 (PTR_FUN_10041f88) at +0 and zeroes the dwords at +4, +8 and +0xC of the
// record, then returns the record (EAX = ECX). __fastcall: the record comes in ECX; the class is unknown, its
// layout is 0x10 bytes at least.
unsigned int* __fastcall UnitInit_guess(unsigned int* record)
{
    *record = 0x10041f88;
    record[1] = 0;
    record[2] = 0;
    record[3] = 0;
    return record;
}

// FUNCTION: EQMAIN 0x100259E0
// Disposes of a front-end table object and its owned child entries.
// __thiscall (this in ECX, one stack arg: the flags byte, callee pops 4). VC6 rejects __thiscall on a free
// function (C4234), so this is __fastcall with an unused EDX parameter: same registers, same stack, same RET 4.
// Scalar deleting destructor: runs UnitDeleteChildren_guess (0x10025A00) on the record, frees the record with the
// cdecl call at 0x10033800 when bit 0 of flags is set, returns the record (EAX = ECX on entry).
unsigned int* __fastcall UnitScalarDeletingDtor_guess(unsigned int* record, int /* EDX unused */, unsigned char flags)
{
    EQ_METHOD(0, void, 0x10025A00, (void *), record)();
    if (flags & 1)
        EQ_CDECL(void, 0x10033800, (void*))(record);
    return record;
}

// File-local stand-in for an array element: slot 0 is the deleting destructor (called with 1 = delete).
struct UnitChild_guess
{
    virtual void Destroy_guess(int deleteFlag) = 0;
};

// FUNCTION: EQMAIN 0x10025A00
// Destroys the child lookup objects owned by the front-end table.
// __fastcall (this in ECX, no stack args, RET 0). The record is the one UnitInit_guess builds: stores the vtable
// 0x10041F88 at +0, then, when the count at +4 and the child array at +8 are both non-null, calls slot 0 of each
// non-null element with 1 (the deleting destructor, as `delete` does). The count is re-read after each call.
// Only caller: UnitScalarDeletingDtor_guess (call at 100259E3).
void __fastcall UnitDeleteChildren_guess(unsigned int* record)
{
    int i;
    UnitChild_guess* child;

    *record = 0x10041f88;
    if (record[1] != 0 && record[2] != 0)
    {
        for (i = 0; i < (int)record[1]; i++)
        {
            child = ((UnitChild_guess**)record[2])[i];
            if (child != 0)
                child->Destroy_guess(1);
        }
    }
}

// FUNCTION: EQMAIN 0x10025A40
// Reads the s989reg.txt data used by front-end lookup records.
// __stdcall, two stack args (RET 8). Opens the path that Identity_guess returns for "..." (mode string at
// 0x10047E60) and stores the file size in *textLength. Reads the whole file into a malloc'd buffer and closes the file.
// The buffer is NUL-terminated and run through DecodeToBuffer_guess; the result is strdup'd into *buffer, and *textLength becomes
// the copy's strlen. The buffer is freed and the result is 1. Returns 0 when the open gives NULL or -1, and when the
// buffer's FIRST BYTE is 0 (tested before fread fills the buffer; that path closes the file and leaks the buffer).
// FILE* is void* here (no stdio.h).
int __stdcall LoadS989RegFile_guess(unsigned int* buffer, size_t* textLength)
{
    unsigned char* buf;
    void* file;
    char* text;

    file = EQ_CDECL(void*, 0x1003407F, (const char*, const char*))(
        EQ_CDECL(char*, 0x100210F0, (const char*))((const char*)0x100487A4),
        (const char*)0x10047E60);
    if (file == 0 || file == (void*)-1)
        return 0;
    EQ_CDECL(int, 0x100345ED, (void*, long, int))(file, 0, 2);
    *textLength = EQ_CDECL(long, 0x10033E8F, (void*))(file);
    EQ_CDECL(int, 0x100345ED, (void*, long, int))(file, 0, 0);
    buf = (unsigned char*)EQ_CDECL(void*, 0x1003342E, (size_t))(*textLength + 1);
    if (*buf == 0)
    {
        EQ_CDECL(int, 0x100343B9, (void*))(file);
        return 0;
    }
    EQ_CDECL(size_t, 0x100342A2, (void*, size_t, size_t, void*))(buf, 1, *textLength, file);
    EQ_CDECL(int, 0x100343B9, (void*))(file);
    buf[*textLength] = 0;
    text = EQ_CDECL(char*, 0x10021110, (unsigned char*))(buf);
    text = EQ_CDECL(char*, 0x1003396B, (const char*))(text);
    *buffer = (unsigned int)text;
    *textLength = strlen(text);
    EQ_CDECL(void, 0x10033568, (void*))(buf);
    return 1;
}

// FUNCTION: EQMAIN 0x10025B00
// Builds front-end lookup records from a loaded text buffer.
// __thiscall (`self` in ECX, no stack arguments, plain RET), written as __fastcall with the EDX dummy. Loads s989reg.txt
// with LoadS989RegFile_guess and returns 0 when that fails, when the text is empty (its length is 0) or when the table
// malloc fails; the buffer is not freed on those last two returns. Otherwise the record count (ParseIntAtCursor_guess) goes to +4
// and a malloc'd table of count dwords to +8 (a count of 0 or less skips the loop). Each record: its name (NextLineAtCursor_guess),
// a string count n (ParseIntAtCursor_guess), then operator_new(0x14) and InitIdxLookup_guess(obj, name, n); a NULL object is stored as 0.
// When n >= 2, n more strings (NextLineAtCursor_guess) go to AddStrCopy_guess(obj, s), NULL obj included (bug kept). The buffer is
// freed and the result is 1.
int __fastcall LoadS989RegRecords_guess(char* self, int unused_edx)
{
    int recordCount;
    void *mem;
    char *text;
    unsigned int stringCount;
    unsigned int *record;
    int recordIndex;
    void *cursor;
    void *buffer;
    size_t textLength;

    recordIndex = 0;
    textLength = 0;
    buffer = 0;
    recordCount = EQ_STDCALL(int, 0x10025A40, (unsigned int *, size_t *))((unsigned int *)&buffer, &textLength);
    if (recordCount == 0)
        return 0;
    if (textLength == 0)
        return 0;
    cursor = buffer;
    recordCount = EQ_CDECL(int, 0x100252A0, (int *))((int *)&cursor);
    *(int *)(self + 4) = recordCount;
    mem = EQ_CDECL(void *, 0x1003342E, (size_t))(recordCount * 4);
    *(void **)(self + 8) = mem;
    if (mem == 0)
        return 0;
    if (0 < *(int *)(self + 4))
    {
        do
        {
            text = EQ_CDECL(char *, 0x10025320, (int *))((int *)&cursor);
            stringCount = EQ_CDECL(int, 0x100252A0, (int *))((int *)&cursor);
            mem = EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x14);
            if (mem != 0)
                record = EQ_METHOD(2, unsigned int *, 0x100258A0, (void *, char *, unsigned int), mem)(text, stringCount);
            else
                record = 0;
            *(unsigned int **)(*(int *)(self + 8) + recordIndex * 4) = record;
            if (1 < (int)stringCount && 0 < (int)stringCount)
            {
                do
                {
                    text = EQ_CDECL(char *, 0x10025320, (int *))((int *)&cursor);
                    EQ_METHOD(1, int, 0x10025900, (void *, char *), *(void **)(*(int *)(self + 8) + recordIndex * 4))(text);
                    stringCount = stringCount - 1;
                } while (stringCount != 0);
            }
            recordIndex = recordIndex + 1;
        } while (recordIndex < *(int *)(self + 4));
    }
    EQ_CDECL(void, 0x10033568, (void *))(buffer);
    return 1;
}

// FUNCTION: EQMAIN 0x10025C00
// Returns the selected record's indexed value for front-end lookup.
// __thiscall, one stack arg (RET 4). Stores the index at +0xC of the record, then returns the dword at
// table[index], where the table pointer is at +8 (no bounds check). The class is unknown; ECX is the record.
// __fastcall with a dummy EDX parameter: the record comes in ECX, the index on the stack.
unsigned int __fastcall UnitIndexGet_guess(unsigned int* record, int dummy, int idx)
{
    *(int*)((char*)record + 0xc) = idx;
    return ((unsigned int*)*(unsigned int*)((char*)record + 8))[idx];
}

// FUNCTION: EQMAIN 0x10025C10
// Resets the front-end's 100-entry record table for new lookups.
// __fastcall, one arg (`self` in ECX, RET with no stack args). Resets the 100 record slots: slot n gets the name at
// g_eqmain_drive_base_guess copied (by the VC6 strcpy intrinsic, with its NUL) to self + n*0x40 and to self + 0x1900 + n*0x40, and
// the dwords at self + 0x3200, 0x3390 and 0x3520 (index n) become 0. Then the word at +0x36B0 is 0, the word at +0x36B2
// is 0xFFFF and the byte at +0x36B4 is 1. No bounds checks: a name of 64 or more chars runs into the next slot.
// @early-stop: the volatile count reloads at the latch; the original loads it early into EAX, so 8 lines still differ.
void __fastcall UnitTableReset_guess(char* self)
{
    volatile int count;  /* fake match: volatile keeps the loop count in a stack slot, as the original does; VC6 then holds the zero in EBP */
    char* rec;
    unsigned int* tbl;

    rec = self;
    tbl = (unsigned int*)(self + 0x3390);
    count = 100;
    do {
        strcpy(rec, (char*)0x1004CE40);
        strcpy(rec + 0x1900, (char*)0x1004CE40);
        rec += 0x40;
        tbl[-0x64] = 0;
        tbl[0] = 0;
        tbl[0x64] = 0;
        tbl++;
    } while (--count);
    *(unsigned short*)(self + 0x36B0) = 0;
    *(unsigned short*)(self + 0x36B2) = 0xffff;
    *(unsigned char*)(self + 0x36B4) = 1;
}

// FUNCTION: EQMAIN 0x10025CC0
// Resets the 100-entry record table (0x10025C10) and returns it, like a constructor.
// __fastcall, one arg (the record in ECX, RET with no stack args). Calls the helper at 0x10025C10 with the same ECX
// (its only argument, EDX is left alone), ignores its result, then returns the record in EAX.
char* __fastcall UnitCallHelperReturnSelf_guess(char* self)
{
    EQ_METHOD(0, void, 0x10025C10, (char*), self)();
    return self;
}

// FUNCTION: EQMAIN 0x10025CD0
// Adds a named entry and its three lookup values to the front-end record list.
// __thiscall, five stack args (RET 0x14). `self` in ECX, written as __fastcall with the EDX dummy. Returns the record
// count (+0x36B0) at once when it is 100 (full). Otherwise copies `primaryName` (with its NUL, by the VC6 strcpy intrinsic)
// to the 0x40-byte record at index `count` from `self`, then `secondaryName` to the record at `count + 100`; a NULL string is
// skipped. Then stores `firstTableValue`, `secondTableValue` and `thirdTableValue` in the dword tables at +0x3200, +0x3390 and +0x3520 at
// `count`, sets the flag byte at +0x36B4 to 1, bumps the count and returns the old count. No bounds checks.
short __fastcall UnitAddRecordNamesAndTables_guess(char* self, int unused_edx, char* primaryName, char* secondaryName,
                                                   unsigned int firstTableValue, unsigned int secondTableValue, unsigned int thirdTableValue)
{
    short index;

    if (*(unsigned short*)(self + 0x36B0) == 100) {
        return *(unsigned short*)(self + 0x36B0);
    }
    if (primaryName != 0) {
        strcpy(self + (int)*(unsigned short*)(self + 0x36B0) * 0x40, primaryName);
    }
    if (secondaryName != 0) {
        strcpy(self + ((int)*(unsigned short*)(self + 0x36B0) + 100) * 0x40, secondaryName);
    }
    *(unsigned int*)(self + (int)*(unsigned short*)(self + 0x36B0) * 4 + 0x3200) = firstTableValue;
    *(unsigned int*)(self + (int)*(unsigned short*)(self + 0x36B0) * 4 + 0x3390) = secondTableValue;
    *(unsigned int*)(self + (int)*(unsigned short*)(self + 0x36B0) * 4 + 0x3520) = thirdTableValue;
    index = *(short*)(self + 0x36B0);
    *(unsigned char*)(self + 0x36B4) = 1;
    *(short*)(self + 0x36B0) = index + 1;
    return index;
}

// FUNCTION: EQMAIN 0x10025DB0
// Replaces a chosen entry's names and three lookup values in the front-end list.
// __thiscall, six stack args (RET 0x18). `self` in ECX, written as __fastcall with the EDX dummy. Copies `primaryName`
// (with its NUL, by the VC6 strcpy intrinsic) to the 0x40-byte record at index `recordIndex` from `self`, then `secondaryName`
// to the record at `recordIndex + 100`; a NULL string is skipped. Then stores `firstTableValue`, `secondTableValue` and `thirdTableValue` in the
// dword tables at +0x3200, +0x3390 and +0x3520, each indexed by `recordIndex`. No bounds checks.
void __fastcall UnitSetRecordNamesAndTables_guess(char* self, int unused_edx, short recordIndex, char* primaryName, char* secondaryName,
                                                  unsigned int firstTableValue, unsigned int secondTableValue, unsigned int thirdTableValue)
{
    if (primaryName != 0) {
        strcpy(self + (int)recordIndex * 0x40, primaryName);
    }
    if (secondaryName != 0) {
        strcpy(self + ((int)recordIndex + 100) * 0x40, secondaryName);
    }
    *(unsigned int*)(self + (int)recordIndex * 4 + 0x3200) = firstTableValue;
    *(unsigned int*)(self + (int)recordIndex * 4 + 0x3390) = secondTableValue;
    *(unsigned int*)(self + (int)recordIndex * 4 + 0x3520) = thirdTableValue;
}

// FUNCTION: EQMAIN 0x10025E50
// Updates a chosen entry's names and first lookup value in the front-end list.
// __thiscall, four stack args (RET 0x10). `self` in ECX, written as __fastcall with the EDX dummy. Copies `primaryName` (with
// its NUL, by the VC6 strcpy intrinsic) to the 0x40-byte record at index `recordIndex` from `self`, then `secondaryName` to the record
// at `recordIndex + 100`; a NULL string is skipped. Then stores `firstTableValue` in the dword table at +0x3200 indexed by `recordIndex`.
// No bounds checks.
void __fastcall UnitSetRecordNames_guess(char* self, int unused_edx, short recordIndex, char* primaryName, char* secondaryName,
                                         unsigned int firstTableValue)
{
    if (primaryName != 0) {
        strcpy(self + (int)recordIndex * 0x40, primaryName);
    }
    if (secondaryName != 0) {
        strcpy(self + ((int)recordIndex + 100) * 0x40, secondaryName);
    }
    *(unsigned int*)(self + (int)recordIndex * 4 + 0x3200) = firstTableValue;
}

// The string at 0x1004CE40 (Ghidra g_eqmain_drive_base_guess, read as a C string; contents not confirmed).
#define g_eqmain_drive_base_guess EQ_ARRAY(char, 0x1004CE40)

// FUNCTION: EQMAIN 0x10025ED0
// Removes a chosen entry and shifts later names and lookup values down.
// @early-stop: strcpy address setup and loop scheduling still differ; structural similarity 0.9013 (19 differences).
// Original ABI: __thiscall with one 4-byte stack argument, popped by RET 4. Written as __fastcall with an unused EDX
// parameter. The low word of `remainingCount` is sign-extended for nextIndex, then the 32-bit parameter slot is reused as the
// loop counter. While nextIndex is below 100, the 0x40-byte name at each index j moves to j - 1 and the name at 100 + j
// moves to 99 + j (both by VC6's strcpy intrinsic); the dword table at +0x31fc moves entries j, 100 + j and 200 + j
// down one (entry k holds record k - 1's dword, so UnitSetRecordNames_guess' +0x3200 is entry 1). Then the last
// record (index `count - 1`, count = the ushort at +0x36b0) gets the string at 0x1004CE40 in both name halves, its
// three dword slots are zeroed, count is decremented and the byte at +0x36b4 is set to 1. No bounds checks.
void __fastcall UnitRemoveRecord_guess(char* self, int unused_edx, int remainingCount)
{
    int nextIndex;
    unsigned int* tableEntry;
    char* recordText;

    nextIndex = (short)remainingCount + 1;
    if (nextIndex < 100) {
        recordText = self + (nextIndex + 0x64) * 0x40;
        remainingCount = 100 - nextIndex;
        tableEntry = (unsigned int*)(self + nextIndex * 4 + 0x31fc);
        do {
            recordText = recordText + 0x40;
            strcpy(recordText - 0x1980, recordText - 0x1940);
            tableEntry = tableEntry + 1;
            strcpy(recordText - 0x80, recordText - 0x40);
            tableEntry[-1] = tableEntry[0];
            tableEntry[99] = tableEntry[100];
            tableEntry[199] = tableEntry[200];
            remainingCount = remainingCount - 1;
        } while (remainingCount != 0);
    }
    strcpy(self + *(unsigned short*)(self + 0x36b0) * 0x40 - 0x40, g_eqmain_drive_base_guess);
    strcpy(self + (*(unsigned short*)(self + 0x36b0) + 99) * 0x40, g_eqmain_drive_base_guess);
    *(unsigned int*)(self + *(unsigned short*)(self + 0x36b0) * 4 + 0x31fc) = 0;
    *(unsigned int*)(self + *(unsigned short*)(self + 0x36b0) * 4 + 0x338c) = 0;
    *(unsigned int*)(self + *(unsigned short*)(self + 0x36b0) * 4 + 0x351c) = 0;
    (*(unsigned short*)(self + 0x36b0))--;
    *(unsigned char*)(self + 0x36b4) = 1;
}

// FUNCTION: EQMAIN 0x10026040
// Finds a front-end record by case-insensitive name.
// __thiscall (`self` in ECX, the one stack argument `searchName` popped by RET 4), written as __fastcall with the EDX
// dummy. Scans up to 100 records of 0x40 bytes from `self`, comparing each record's name at +0 with `searchName` by
// __strcmpi (0x10033730, __cdecl). Returns the first matching index in AX, or 0xFFFF in AX if no record matches;
// on that failure path the upper half of EAX remains from the final __strcmpi call.
unsigned short __fastcall FindRecordIndexByName_guess(void* self, int unused_edx, char* searchName)
{
    int comparisonResult;
    unsigned int recordIndex;
    unsigned short result;
    void* current;

    recordIndex = 0;
    current = self;
    while ((int)recordIndex < 100) {
        comparisonResult = EQ_CDECL(int, 0x10033730, (char*, char*))((char*)current, searchName);
        if (comparisonResult == 0) {
            return recordIndex & 0xffff;
        }
        recordIndex = recordIndex + 1;
        current = (void*)((int)current + 0x40);
    }
    result = (unsigned short)comparisonResult;
    result |= 0xffff;
    return result;
}

// FUNCTION: EQMAIN 0x10026080
// Builds the eqtext.txt text table: sets [LANGUAGE] to 1, loads its quoted strings, points 270 slots at records
// __fastcall (`table` in ECX, no stack argument; RET pops nothing). Returns table. Zeroes the record table (0x20000
// dwords from table + 0x43C). Sets the "[LANGUAGE]" value when EqfeCfgSetActive_guess's object has +4 == -1 or 0 (+4 becomes 1
// and EqfeCfgSetValue_guess is told "[LANGUAGE]" and that value), then parses eqtext.txt (Identity_guess gives its path, FUN_1003407F
// opens it with &g_eqmain_fopen_mode_guess). Each quoted string's characters go to record `*table` (0x400 bytes at +0x43C + i * 0x400,
// the first 0x3FE only). Escapes \" \' \0 \? \\ \a \b \f \n \r \t \v are mapped; any other escaped char is stored without
// its backslash. The count at +0 goes up to 0x200. The loop ends when the FILE's _flag has 0x10 set (a NULL file skips it);
// the file is closed, RemapRecordStrings_guess remaps the records and the pointers at +4 .. +0x438 are set.
int* __fastcall LoadEqTextRecordTable_guess(int* table)
{
    bool done;
    char ch;
    void* obj;
    FILE* file;
    int tmp;
    int charIndex;

    *table = 0;
    memset(table + 0x10f, 0, 0x80000);
    done = false;
    charIndex = EQ_CDECL(int, 0x10020EF0, (char))('\x01');
    *(int*)(charIndex + 4) = -1;
    charIndex = EQ_CDECL(int, 0x10020EF0, (char))('\x01');
    if (*(int*)(charIndex + 4) == -1 || (charIndex = EQ_CDECL(int, 0x10020EF0, (char))('\x01'), *(int*)(charIndex + 4) == 0)) {
        charIndex = EQ_CDECL(int, 0x10020EF0, (char))('\x01');
        *(int*)(charIndex + 4) = 1;
        charIndex = EQ_CDECL(int, 0x10020EF0, (char))('\x01');
        charIndex = *(int*)(charIndex + 4);
        obj = EQ_CDECL(void*, 0x10020EF0, (char))('\x01');
        EQ_METHOD(2, int, 0x10020CD0, (void*, char*, int), obj)(EQ_ARRAY(char, 0x10048098), charIndex);
    }
    file = EQ_CDECL(FILE*, 0x1003407F, (char*, char*))(EQ_CDECL(char*, 0x100210F0, (char*))(EQ_ARRAY(char, 0x100487B0)), EQ_ARRAY(char, 0x100480A4));
    if (file != NULL) {
        while (!done || (file->_flag & 0x10) == 0) {
            charIndex = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
            ch = (char)charIndex;
            while (ch != '\"') {
                if ((file->_flag & 0x10) != 0) {
                    if ((char)charIndex != '\"') goto markDone;
                    break;
                }
                charIndex = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
                ch = (char)charIndex;
            }
            charIndex = 0;
            tmp = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
            ch = (char)tmp;
            while (ch != '\"') {
                ch = (char)tmp;
                if ((file->_flag & 0x10) != 0) {
                    if (ch != '\"') goto markDone;
                    break;
                }
                if (charIndex >= 0x3fe) {
                    done = true;
                    goto nextChar;
                }
                if (ch != '\\') goto storeChar;
                tmp = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
                ch = (char)tmp;
                switch (ch) {
                case 'n':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 10;
                    break;
                case '\\':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0x5c;
                    break;
                case 't':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 9;
                    break;
                case 'v':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0xb;
                    break;
                case 'b':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 8;
                    break;
                case 'r':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0xd;
                    break;
                case 'f':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0xc;
                    break;
                case 'a':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 7;
                    break;
                case '?':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0x3f;
                    break;
                case '\'':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0x27;
                    break;
                case '\"':
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = 0x22;
                    break;
                case '0':
                    ch = '\0';
                default:
storeChar:
                    tmp = *table * 0x400 + charIndex;
                    charIndex = charIndex + 1;
                    *(char *)(tmp + 0x43c + (int)table) = ch;
                    break;
                }
nextChar:
                tmp = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
                ch = (char)tmp;
            }
            if (*table < 0x200) {
                *table = *table + 1;
            }
            else {
markDone:
                done = true;
            }
        }
        EQ_CDECL(int, 0x100343B9, (FILE*))(file);
    }
    EQ_FUNC(void, __fastcall, 0x10027210, (int*))(table);
    table[0x1] = (int)(table + 0x10f);
    table[0x2] = (int)(table + 0x20f);
    table[0x3] = (int)(table + 0x30f);
    table[0x4] = (int)(table + 0x40f);
    table[0x5] = (int)(table + 0x50f);
    table[0x6] = (int)(table + 0x60f);
    table[0x7] = (int)(table + 0x70f);
    table[0x8] = (int)(table + 0x80f);
    table[0x9] = (int)(table + 0x90f);
    table[0xa] = (int)(table + 0xa0f);
    table[0xb] = (int)(table + 0xb0f);
    table[0xc] = (int)(table + 0xc0f);
    table[0xd] = (int)(table + 0xd0f);
    table[0xe] = (int)(table + 0xe0f);
    table[0xf] = (int)(table + 0xf0f);
    table[0x10] = (int)(table + 0x100f);
    table[0x11] = (int)(table + 0x110f);
    table[0x12] = (int)(table + 0x120f);
    table[0x13] = (int)(table + 0x130f);
    table[0x14] = (int)(table + 0x140f);
    table[0x15] = (int)(table + 0x150f);
    table[0x16] = (int)(table + 0x160f);
    table[0x17] = (int)(table + 0x170f);
    table[0x18] = (int)(table + 0x180f);
    table[0x19] = (int)(table + 0x190f);
    table[0x1a] = (int)(table + 0x1a0f);
    table[0x1b] = (int)(table + 0x1b0f);
    table[0x1c] = (int)(table + 0x1c0f);
    table[0x1d] = (int)(table + 0x1d0f);
    table[0x1e] = (int)(table + 0x1e0f);
    table[0x1f] = (int)(table + 0x1f0f);
    table[0x20] = (int)(table + 0x200f);
    table[0x21] = (int)(table + 0x210f);
    table[0x22] = (int)(table + 0x220f);
    table[0x23] = (int)(table + 0x230f);
    table[0x24] = (int)(table + 0x240f);
    table[0x25] = (int)(table + 0x250f);
    table[0x26] = (int)(table + 0x260f);
    table[0x27] = (int)(table + 0x270f);
    table[0x28] = (int)(table + 0x280f);
    table[0x29] = (int)(table + 0x290f);
    table[0x2a] = (int)(table + 0x2a0f);
    table[0x2b] = (int)(table + 0x2b0f);
    table[0x2c] = (int)(table + 0x2c0f);
    table[0x2d] = (int)(table + 0x2d0f);
    table[0x2e] = (int)(table + 0x2e0f);
    table[0x2f] = (int)(table + 0x2f0f);
    table[0x30] = (int)(table + 0x300f);
    table[0x31] = (int)(table + 0x310f);
    table[0x32] = (int)(table + 0x320f);
    table[0x33] = (int)(table + 0x330f);
    table[0x34] = (int)(table + 0x340f);
    table[0x35] = (int)(table + 0x350f);
    table[0x36] = (int)(table + 0x360f);
    table[0x37] = (int)(table + 0x370f);
    table[0x38] = (int)(table + 0x380f);
    table[0x39] = (int)(table + 0x390f);
    table[0x3a] = (int)(table + 0x3a0f);
    table[0x3b] = (int)(table + 0x3b0f);
    table[0x3c] = (int)(table + 0x3c0f);
    table[0x3d] = (int)(table + 0x3d0f);
    table[0x3e] = (int)(table + 0x3e0f);
    table[0x3f] = (int)(table + 0x3f0f);
    table[0x40] = (int)(table + 0x400f);
    table[0x41] = (int)(table + 0x410f);
    table[0x42] = (int)(table + 0x420f);
    table[0x43] = (int)(table + 0x430f);
    table[0x44] = (int)(table + 0x440f);
    table[0x45] = (int)(table + 0x450f);
    table[0x46] = (int)(table + 0x460f);
    table[0x47] = (int)(table + 0x470f);
    table[0x48] = (int)(table + 0x480f);
    table[0x49] = (int)(table + 0x490f);
    table[0x4a] = (int)(table + 0x4a0f);
    table[0x4b] = (int)(table + 0x4b0f);
    table[0x4c] = (int)(table + 0x4c0f);
    table[0x4d] = (int)(table + 0x4d0f);
    table[0x4e] = (int)(table + 0x4e0f);
    table[0x4f] = (int)(table + 0x4f0f);
    table[0x50] = (int)(table + 0x500f);
    table[0x51] = (int)(table + 0x510f);
    table[0x52] = (int)(table + 0x520f);
    table[0x53] = (int)(table + 0x530f);
    table[0x54] = (int)(table + 0x540f);
    table[0x55] = (int)(table + 0x550f);
    table[0x56] = (int)(table + 0x560f);
    table[0x57] = (int)(table + 0x570f);
    table[0x58] = (int)(table + 0x580f);
    table[0x59] = (int)(table + 0x590f);
    table[0x5a] = (int)(table + 0x5a0f);
    table[0x5b] = (int)(table + 0x5b0f);
    table[0x5c] = (int)(table + 0x5c0f);
    table[0x5d] = (int)(table + 0x5d0f);
    table[0x5e] = (int)(table + 0x5e0f);
    table[0x5f] = (int)(table + 0x5f0f);
    table[0x60] = (int)(table + 0x600f);
    table[0x61] = (int)(table + 0x610f);
    table[0x62] = (int)(table + 0x620f);
    table[0x63] = (int)(table + 0x630f);
    table[0x64] = (int)(table + 0x640f);
    table[0x65] = (int)(table + 0x650f);
    table[0x66] = (int)(table + 0x660f);
    table[0x67] = (int)(table + 0x670f);
    table[0x68] = (int)(table + 0x680f);
    table[0x69] = (int)(table + 0x690f);
    table[0x6a] = (int)(table + 0x6a0f);
    table[0x6b] = (int)(table + 0x6b0f);
    table[0x6c] = (int)(table + 0x6c0f);
    table[0x6d] = (int)(table + 0x6d0f);
    table[0x6e] = (int)(table + 0x6e0f);
    table[0x6f] = (int)(table + 0x6f0f);
    table[0x70] = (int)(table + 0x700f);
    table[0x71] = (int)(table + 0x710f);
    table[0x72] = (int)(table + 0x720f);
    table[0x73] = (int)(table + 0x730f);
    table[0x74] = (int)(table + 0x740f);
    table[0x75] = (int)(table + 0x750f);
    table[0x76] = (int)(table + 0x760f);
    table[0x77] = (int)(table + 0x770f);
    table[0x78] = (int)(table + 0x780f);
    table[0x79] = (int)(table + 0x790f);
    table[0x7a] = (int)(table + 0x7a0f);
    table[0x7b] = (int)(table + 0x7b0f);
    table[0x7c] = (int)(table + 0x7c0f);
    table[0x7d] = (int)(table + 0x7d0f);
    table[0x7e] = (int)(table + 0x7e0f);
    table[0x7f] = (int)(table + 0x7f0f);
    table[0x80] = (int)(table + 0x800f);
    table[0x81] = (int)(table + 0x810f);
    table[0x82] = (int)(table + 0x820f);
    table[0x83] = (int)(table + 0x830f);
    table[0x84] = (int)(table + 0x840f);
    table[0x85] = (int)(table + 0x850f);
    table[0x86] = (int)(table + 0x860f);
    table[0x87] = (int)(table + 0x870f);
    table[0x88] = (int)(table + 0x880f);
    table[0x89] = (int)(table + 0x890f);
    table[0x8a] = (int)(table + 0x8a0f);
    table[0x8b] = (int)(table + 0x8b0f);
    table[0x8c] = (int)(table + 0x8c0f);
    table[0x8d] = (int)(table + 0x8d0f);
    table[0x8e] = (int)(table + 0x8e0f);
    table[0x8f] = (int)(table + 0x8f0f);
    table[0x90] = (int)(table + 0x900f);
    table[0x91] = (int)(table + 0x910f);
    table[0x92] = (int)(table + 0x920f);
    table[0x93] = (int)(table + 0x930f);
    table[0x94] = (int)(table + 0x940f);
    table[0x95] = (int)(table + 0x950f);
    table[0x96] = (int)(table + 0x960f);
    table[0x97] = (int)(table + 0x970f);
    table[0x98] = (int)(table + 0x980f);
    table[0x99] = (int)(table + 0x990f);
    table[0x9a] = (int)(table + 0x9a0f);
    table[0x9b] = (int)(table + 0x9b0f);
    table[0x9c] = (int)(table + 0x9c0f);
    table[0x9d] = (int)(table + 0x9d0f);
    table[0x9e] = (int)(table + 0x9e0f);
    table[0x9f] = (int)(table + 0x9f0f);
    table[0xa0] = (int)(table + 0xa00f);
    table[0xa1] = (int)(table + 0xa10f);
    table[0xa2] = (int)(table + 0xa20f);
    table[0xa3] = (int)(table + 0xa30f);
    table[0xa4] = (int)(table + 0xa40f);
    table[0xa5] = (int)(table + 0xa50f);
    table[0xa6] = (int)(table + 0xa60f);
    table[0xa7] = (int)(table + 0xa70f);
    table[0xa8] = (int)(table + 0xa80f);
    table[0xa9] = (int)(table + 0xa90f);
    table[0xaa] = (int)(table + 0xaa0f);
    table[0xab] = (int)(table + 0xab0f);
    table[0xac] = (int)(table + 0xac0f);
    table[0xad] = (int)(table + 0xad0f);
    table[0xae] = (int)(table + 0xae0f);
    table[0xaf] = (int)(table + 0xaf0f);
    table[0xb0] = (int)(table + 0xb00f);
    table[0xb1] = (int)(table + 0xb10f);
    table[0xb2] = (int)(table + 0xb20f);
    table[0xb3] = (int)(table + 0xb30f);
    table[0xb4] = (int)(table + 0xb40f);
    table[0xb5] = (int)(table + 0xb50f);
    table[0xb6] = (int)(table + 0xb60f);
    table[0xb7] = (int)(table + 0xb70f);
    table[0xb8] = (int)(table + 0xb80f);
    table[0xb9] = (int)(table + 0xb90f);
    table[0xba] = (int)(table + 0xba0f);
    table[0xbb] = (int)(table + 0xbb0f);
    table[0xbc] = (int)(table + 0xbc0f);
    table[0xbd] = (int)(table + 0xbd0f);
    table[0xbe] = (int)(table + 0xbe0f);
    table[0xbf] = (int)(table + 0xbf0f);
    table[0xc0] = (int)(table + 0xc00f);
    table[0xc1] = (int)(table + 0xc10f);
    table[0xc2] = (int)(table + 0xc20f);
    table[0xc3] = (int)(table + 0xc30f);
    table[0xc4] = (int)(table + 0xc40f);
    table[0xc5] = (int)(table + 0xc50f);
    table[0xc6] = (int)(table + 0xc60f);
    table[0xc7] = (int)(table + 0xc70f);
    table[0xc8] = (int)(table + 0xc80f);
    table[0xc9] = (int)(table + 0xc90f);
    table[0xca] = (int)(table + 0xca0f);
    table[0xcb] = (int)(table + 0xcb0f);
    table[0xcc] = (int)(table + 0xcc0f);
    table[0xcd] = (int)(table + 0xcd0f);
    table[0xce] = (int)(table + 0xce0f);
    table[0xcf] = (int)(table + 0xcf0f);
    table[0xd0] = (int)(table + 0xd00f);
    table[0xd1] = (int)(table + 0xd10f);
    table[0xd2] = (int)(table + 0xd20f);
    table[0xd3] = (int)(table + 0xd30f);
    table[0xd4] = (int)(table + 0xd40f);
    table[0xd5] = (int)(table + 0xd50f);
    table[0xd6] = (int)(table + 0xd60f);
    table[0xd7] = (int)(table + 0xd70f);
    table[0xd8] = (int)(table + 0xd80f);
    table[0xd9] = (int)(table + 0xd90f);
    table[0xda] = (int)(table + 0xda0f);
    table[0xdb] = (int)(table + 0xdb0f);
    table[0xdc] = (int)(table + 0xdc0f);
    table[0xdd] = (int)(table + 0xdd0f);
    table[0xde] = (int)(table + 0xde0f);
    table[0xdf] = (int)(table + 0xdf0f);
    table[0xe0] = (int)(table + 0xe00f);
    table[0xe1] = (int)(table + 0xe10f);
    table[0xe2] = (int)(table + 0xe20f);
    table[0xe3] = (int)(table + 0xe30f);
    table[0xe4] = (int)(table + 0xe40f);
    table[0xe5] = (int)(table + 0xe50f);
    table[0xe6] = (int)(table + 0xe60f);
    table[0xe7] = (int)(table + 0xe70f);
    table[0xe8] = (int)(table + 0xe80f);
    table[0xe9] = (int)(table + 0xe90f);
    table[0xea] = (int)(table + 0xea0f);
    table[0xeb] = (int)(table + 0xeb0f);
    table[0xec] = (int)(table + 0xec0f);
    table[0xed] = (int)(table + 0xed0f);
    table[0xee] = (int)(table + 0xee0f);
    table[0xef] = (int)(table + 0xef0f);
    table[0xf0] = (int)(table + 0xf00f);
    table[0xf1] = (int)(table + 0xf10f);
    table[0xf2] = (int)(table + 0xf20f);
    table[0xf3] = (int)(table + 0xf30f);
    table[0xf4] = (int)(table + 0xf40f);
    table[0xf5] = (int)(table + 0xf50f);
    table[0xf6] = (int)(table + 0xf60f);
    table[0xf7] = (int)(table + 0xf70f);
    table[0xf8] = (int)(table + 0xf80f);
    table[0xf9] = (int)(table + 0xf90f);
    table[0xfa] = (int)(table + 0xfa0f);
    table[0xfb] = (int)(table + 0xfb0f);
    table[0xfc] = (int)(table + 0xfc0f);
    table[0xfd] = (int)(table + 0xfd0f);
    table[0xfe] = (int)(table + 0xfe0f);
    table[0xff] = (int)(table + 0xff0f);
    table[0x100] = (int)(table + 0x1000f);
    table[0x101] = (int)(table + 0x1010f);
    table[0x102] = (int)(table + 0x1020f);
    table[0x103] = (int)(table + 0x1030f);
    table[0x104] = (int)(table + 0x1040f);
    table[0x105] = (int)(table + 0x1050f);
    table[0x106] = (int)(table + 0x1060f);
    table[0x107] = (int)(table + 0x1070f);
    table[0x108] = (int)(table + 0x1080f);
    table[0x109] = (int)(table + 0x1090f);
    table[0x10a] = (int)(table + 0x10a0f);
    table[0x10b] = (int)(table + 0x10b0f);
    table[0x10c] = (int)(table + 0x10c0f);
    table[0x10d] = (int)(table + 0x10d0f);
    table[0x10e] = (int)(table + 0x10e0f);
    return table;
}

// FUNCTION: EQMAIN 0x10026FD0
// Loads eqtext.txt into the front end's language string table.
// @early-stop: file/index registers, the bool stack spill, and fixed-address call ABI instructions still differ.
// __fastcall (`table` in ECX, no stack argument; RET pops nothing), void. Zeroes the record table (0x20000 dwords from
// table + 0x43C), sets the "[LANGUAGE]" value through EqfeCfgSetActive_guess's object (1 when it was -1; EqfeCfgSetValue_guess is told),
// then parses eqtext.txt (Identity_guess gives its path): each quoted string's characters go to record `*table` (0x400 bytes
// at +0x43C + i * 0x400, the first 0x3FE only), escapes \" \' \0 \? \\ \a \b \f \n \r \t \v are mapped and other escapes kept,
// and the count at +0 goes up to 0x200. The loops end when the FILE's _flag has 0x10 set: the file is closed and RemapRecordStrings_guess
// remaps the records. A NULL file calls _exit(-1).
void __fastcall LoadEqTextRecords_guess(int* table)
{
    bool done;
    char ch;
    FILE* file;
    int inChar;
    int tmp;
    int* clearPtr;

    *table = 0;
    clearPtr = table + 0x10f;
    for (tmp = 0x20000; tmp != 0; tmp = tmp + -1) {
        *clearPtr = 0;
        clearPtr = clearPtr + 1;
    }
    done = false;
    tmp = EQ_CDECL(int, 0x10020EF0, (char))(1);
    *(int*)(tmp + 4) = -1;
    tmp = EQ_CDECL(int, 0x10020EF0, (char))(1);
    if (*(int*)(tmp + 4) == -1) {
        tmp = EQ_CDECL(int, 0x10020EF0, (char))(1);
        *(int*)(tmp + 4) = 1;
        tmp = EQ_CDECL(int, 0x10020EF0, (char))(1);
        tmp = *(int*)(tmp + 4);
        EQ_METHOD(2, int, 0x10020CD0, (void*, char*, int), (void*)EQ_CDECL(int, 0x10020EF0, (char))(1))(EQ_ARRAY(char, 0x10048098), tmp);
    }
    file = EQ_CDECL(FILE*, 0x1003407F, (char*, char*))(
        EQ_CDECL(char*, 0x100210F0, (char*))(EQ_ARRAY(char, 0x100487B0)), EQ_ARRAY(char, 0x100480A4));
    if (file == 0) {
        EQ_CDECL(void, 0x10035E98, (int))(-1);
    }
nextString:
    do {
        if (done && (file->_flag & 0x10) != 0) {
            EQ_CDECL(int, 0x100343B9, (FILE*))(file);
            EQ_METHOD(0, void, 0x10027210, (int*), table)();
            return;
        }
        tmp = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
        ch = (char)tmp;
        while (ch != '"') {
            if ((file->_flag & 0x10) != 0) {
                if ((char)tmp != '"') goto markDone;  // as original: both EOF exits jump to one shared `done = true` tail
                break;
            }
            tmp = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
            ch = (char)tmp;
        }
        tmp = 0;
        inChar = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
        ch = (char)inChar;
        while (ch != '"') {
            ch = (char)inChar;
            if ((file->_flag & 0x10) != 0) {
                if (ch != '"') goto markDone;  // as original: both EOF exits jump to the same shared `done = true` tail
                break;
            }
            if (tmp < 0x3fe) {
                if (ch == '\\') {
                    inChar = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
                    ch = (char)inChar;
                    switch (ch) {
                    case 'n':
                        ch = '\n';
                        break;
                    case '\\':
                        ch = '\\';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    case 'b':
                        ch = '\b';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'a':
                        ch = '\a';
                        break;
                    case '?':
                        ch = '?';
                        break;
                    case '\'':
                        ch = '\'';
                        break;
                    case '"':
                        ch = '"';
                        break;
                    case '0':
                        ch = '\0';
                    }
                }
                ((char*)table)[0x43C + *table * 0x400 + tmp++] = ch;
            }
            else {
                done = true;
            }
            inChar = EQ_CDECL(int, 0x10035F9A, (FILE*))(file);
            ch = (char)inChar;
        }
        if (*table < 0x200) {
            *table = *table + 1;
        }
    } while (true);
markDone:
    done = true;
    goto nextString;  // as original: the shared tail that both EOF exits reach jumps back to the do-loop head
}

// FUNCTION: EQMAIN 0x10027210
// Rewrites each loaded text record in place with the string DecodeToBuffer_guess returns for it.
// @early-stop: inner copy address calculation and post-copy register scheduling still differ under /O2.
// __fastcall (`self` in ECX, no stack argument; RET pops nothing), void. `self` holds a count at +0; record i is 0x400
// bytes at self + 0x43C + i * 0x400. For each record: copy its string (up to the NUL, at most 0x400 bytes) into a zeroed
// 1024-byte local, zero the record, pass the local to DecodeToBuffer_guess (__cdecl) and strcpy its result back into the record.
// The copy-back is not bounded: a result of 0x400 bytes or more runs into the next record (bug kept). The count is
// re-read from memory after every record.
void __fastcall RemapRecordStrings_guess(int* recordTable)
{
    char character;
    int charOffset;
    char* decodedText;
    int recordIndex;
    int* recordText;
    unsigned char inputBuffer[1024];

    recordIndex = 0;
    if (0 < *recordTable) {
        recordText = recordTable + 0x10f;
        do {
            memset(inputBuffer, 0, 0x400);
            for (charOffset = 0; charOffset < 0x400; charOffset++) {
                character = *(char*)((int)recordText + charOffset);
                if (character == '\0') {
                    break;
                }
                inputBuffer[charOffset] = character;
            }
            memset(recordText, 0, 0x400);
            decodedText = EQ_CDECL(char*, 0x10021110, (unsigned char*))(inputBuffer);
            strcpy((char*)recordText, decodedText);
            recordIndex = recordIndex + 1;
            recordText = recordText + 0x100;
        } while (recordIndex < *recordTable);
    }
}

// g_cpuClassSpeedTable_guess: a table of dwords in eqmain.dll's .data, indexed by the low word of DetectCpuClass_guess's result.
#define g_cpuClassSpeedTable_guess  EQ_ARRAY(int, 0x100487BC)

// FUNCTION: EQMAIN 0x100272A0
// Provides the front end with a CPU speed estimate for timing work.
// __cdecl, callee pops nothing (RET). Returns out (EAX) on every path. When DetectCpuClass_guess's result has 0x8000, zeroes
// out. Otherwise the count is g_cpuClassSpeedTable_guess[(ushort)cpuInfo] * 4000 when option <= 0, option * 4000 when option is
// 1..150 (inRange), else out itself. With inRange set or CpuidFeatureFlags_guess's 0x10 bit clear: (ushort)cpuInfo below 3 zeroes
// out, else CalibrateSpinLoop_guess runs on the count. Otherwise option == 0 takes MeasureCpuClock_guess and
// the rest MeasureCpuSpeed_guess. The record it returns is copied to out.
unsigned int* __cdecl GetCpuSpeed_guess(unsigned int* out, int option)
{
    unsigned int word3;
    int inRange;
    unsigned int cpuInfo;
    unsigned int tmp;
    unsigned int* value;
    unsigned int record[4];

    cpuInfo = EQ_STDCALL(unsigned int, 0x100244C0, (void))();
    tmp = EQ_STDCALL(unsigned int, 0x10024520, (void))();
    inRange = 0;
    if ((cpuInfo & 0x8000) != 0) {
        *out = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        return out;
    }
    if (option < 1) {
        value = (unsigned int*)(g_cpuClassSpeedTable_guess[cpuInfo & 0xffff] * 4000);
    }
    else {
        value = out;
        if (option < 0x97) {
            inRange = 1;
            value = (unsigned int*)(option * 4000);
        }
    }
    if (((tmp & 0x10) == 0) || (inRange)) {
        if ((unsigned short)cpuInfo < 3) {
            *out = 0;
            out[1] = 0;
            out[2] = 0;
            out[3] = 0;
            return out;
        }
        value = EQ_CDECL(unsigned int*, 0x100273A0, (unsigned int*, unsigned int))(record, (unsigned int)value);
    }
    else if (option == 0) {
        value = EQ_CDECL(unsigned int*, 0x10027480, (unsigned int*))(record);
    }
    else {
        value = EQ_CDECL(unsigned int*, 0x10027700, (unsigned int*))(record);
    }
    cpuInfo = value[1];
    tmp = value[2];
    word3 = value[3];
    *out = *value;
    out[1] = cpuInfo;
    out[2] = tmp;
    out[3] = word3;
    return out;
}

// LARGE_INTEGER as the QueryPerformance calls fill it (8 bytes); the code reads LowPart only.
struct LargeInteger_guess
{
    unsigned int LowPart;   // 0x0
    int HighPart;           // 0x4
};

// Import slots in eqmain.dll's own IAT (kernel32, bound at load). Each is called through its slot
// (`call dword ptr [va]`), so it goes through EQ_GLOBAL like the client's own globals.
typedef int (__stdcall *QueryPerformance_guess_t)(LargeInteger_guess *);
#define PTR_QueryPerformanceFrequency_100400e0  EQ_GLOBAL(QueryPerformance_guess_t, 0x100400E0)
#define PTR_QueryPerformanceCounter_100400dc    EQ_GLOBAL(QueryPerformance_guess_t, 0x100400DC)

// FUNCTION: EQMAIN 0x100273A0
// Estimates CPU MHz by timing a 4000-step BSF loop of known cycle count against QueryPerformanceCounter.
// __cdecl, callee pops nothing (RET). Calibrates the 4000-step BSF spin: when QueryPerformanceFrequency fails, writes
// four zeroes. Otherwise times the spin ten times between QueryPerformanceCounter reads and keeps the shortest tick
// delta (unsigned, from 0xFFFFFFFF). Its time in microseconds is delta * 100000 / (freq / 10), 32-bit, rounded up when
// the remainder is above freq / 2. Writes four dwords to out: cycles, that time, cycles / time, and that quotient
// rounded up when cycles % time is above time / 2. Returns out (EAX) on both paths.
// @early-stop: removing the existing BSF asm exceeds the fixture instruction limit; VC6 emits _BitScanForward as a call.
unsigned int* __cdecl CalibrateSpinLoop_guess(unsigned int* record, unsigned int cycles)
{
    LargeInteger_guess freq;
    LargeInteger_guess start;
    LargeInteger_guess stop;
    int triesLeft;
    unsigned int rawTimeUs;
    unsigned int zero;
    unsigned int roundedRate;
    unsigned int timeUs;
    QueryPerformance_guess_t pfnCounter;

    zero = 0;
    timeUs = 0xffffffff;
    if (PTR_QueryPerformanceFrequency_100400e0(&freq) == 0)
    {
        *record = zero;
        record[1] = zero;
        record[2] = zero;
        record[3] = zero;
        return record;
    }
    pfnCounter = PTR_QueryPerformanceCounter_100400dc;
    triesLeft = 10;
    do
    {
        pfnCounter(&start);
        // as original: bsf ecx, eax; dec bx; jnz (the 4000-step spin, bx = 0xfa0): VC6 has no BSF in C; its ECX result is dead
        __asm
        {
            mov eax, 0x80000000
            mov bx, 0xfa0
        spin:
            bsf ecx, eax
            dec bx
            jnz spin
        }
        pfnCounter(&stop);
        if (stop.LowPart - start.LowPart < timeUs)
            timeUs = stop.LowPart - start.LowPart;
        triesLeft = triesLeft + -1;
    } while (triesLeft != 0);
    rawTimeUs = (timeUs * 100000) / (freq.LowPart / 10);
    timeUs = rawTimeUs;
    if ((rawTimeUs % freq.LowPart) > (freq.LowPart >> 1))
        timeUs = timeUs + 1;
    roundedRate = cycles / timeUs;
    zero = roundedRate;
    if ((cycles % timeUs) > (timeUs >> 1))
        roundedRate = roundedRate + 1;
    *record = cycles;
    record[1] = timeUs;
    record[2] = zero;
    record[3] = roundedRate;
    return record;
}

// Import slots for the thread calls (kernel32, same IAT as the QueryPerformance slots above).
typedef void *(__stdcall *GetCurrentThread_guess_t)(void);
typedef int (__stdcall *GetThreadPriority_guess_t)(void *);
typedef int (__stdcall *SetThreadPriority_guess_t)(void *, int);
#define PTR_GetCurrentThread_100400ec   EQ_GLOBAL(GetCurrentThread_guess_t, 0x100400EC)
#define PTR_GetThreadPriority_100400e8  EQ_GLOBAL(GetThreadPriority_guess_t, 0x100400E8)
#define PTR_SetThreadPriority_100400e4  EQ_GLOBAL(SetThreadPriority_guess_t, 0x100400E4)

// abs() of an int as VC6 expands it inline (CDQ, XOR, SUB): no branch.
#define ABS_INT_guess(x)  (((x) ^ ((x) >> 31)) - ((x) >> 31))

// FUNCTION: EQMAIN 0x10027480
// Estimates CPU speed from repeated performance-counter and timestamp samples.
// __cdecl, callee pops nothing (RET). Measures the CPU clock against QueryPerformanceCounter. Each sample reads the
// counter, waits until it is 50 ticks past that read, then counts RDTSC cycles until it is 1000 ticks past it, with
// this thread at priority 15 unless it is already at 0x7FFFFFFF. From the third sample on, sampling stops once the last
// three cycles-per-microsecond rates agree within 1 of their mean (or after 20 samples). Writes four dwords to out:
// the summed cycles, the summed microseconds (before rounding), cycles per microsecond, and that quotient rounded to a
// whole number. Without QueryPerformanceFrequency it writes four zeroes. Returns out (EAX) on both paths.
unsigned int* __cdecl MeasureCpuClock_guess(unsigned int* record)
{
    LargeInteger_guess freq;
    LargeInteger_guess now;
    LargeInteger_guess start;
    int tscStart;
    int tscEnd;
    void* curThread;
    unsigned int olderRate;
    unsigned int totalUs;
    unsigned int totalCycles;
    unsigned int prevRate;
    int sampleCount;
    unsigned int elapsedUs;
    unsigned int tmp;
    unsigned int value;
    int acc;
    void* hThread;
    int bOk;
    QueryPerformance_guess_t pfnCounter;

    tmp = 0;
    sampleCount = 0;
    totalCycles = 0;
    totalUs = 0;
    curThread = PTR_GetCurrentThread_100400ec();
    bOk = PTR_QueryPerformanceFrequency_100400e0(&freq);
    prevRate = 0;
    if (bOk != 0)
    {
        do
        {
            sampleCount = sampleCount + 1;
            olderRate = prevRate;
            prevRate = tmp;
            pfnCounter = PTR_QueryPerformanceCounter_100400dc;
            pfnCounter(&start);
            hThread = curThread;
            now = start;
            acc = PTR_GetThreadPriority_100400e8(hThread);
            if (acc != 0x7fffffff)
                PTR_SetThreadPriority_100400e4(hThread, 0xf);
            tmp = now.LowPart - start.LowPart;
            while (tmp < 0x32)
            {
                pfnCounter(&now);
                __asm
                {
                    rdtsc
                    mov tscStart, eax
                }
                tmp = now.LowPart - start.LowPart;
            }
            start = now;
            while (now.LowPart - start.LowPart < 1000)
            {
                pfnCounter(&now);
                __asm
                {
                    rdtsc
                    mov tscEnd, eax
                }
            }
            if (acc != 0x7fffffff)
                PTR_SetThreadPriority_100400e4(hThread, acc);
            value = tscEnd - tscStart;
            elapsedUs = (now.LowPart * 100000 - start.LowPart * 100000) / (freq.LowPart / 10);
            totalUs = totalUs + elapsedUs;
            totalCycles = totalCycles + value;
            if (freq.LowPart >> 1 < elapsedUs % freq.LowPart)
                elapsedUs = elapsedUs + 1;
            tmp = value / elapsedUs;
            if (elapsedUs >> 1 < value % elapsedUs)
                tmp = tmp + 1;
            acc = olderRate + prevRate + tmp;
        } while (sampleCount < 3 || (sampleCount < 20 &&
                 (ABS_INT_guess((int)(tmp * 3 - acc)) > 3 ||
                  ABS_INT_guess((int)(prevRate * 3 - acc)) > 3 ||
                  ABS_INT_guess((int)(olderRate * 3 - acc)) > 3)));
        tmp = (totalCycles * 10) / totalUs;
        if (5 < (totalCycles * 100) / totalUs - tmp * 10)
            tmp = tmp + 1;
        value = totalCycles / totalUs;
        elapsedUs = value;
        if (5 < tmp - value * 10)
            elapsedUs = value + 1;
        record[0] = totalCycles;
        record[1] = totalUs;
        record[2] = value;
        record[3] = elapsedUs;
        return record;
    }
    record[0] = 0;
    record[1] = 0;
    record[2] = 0;
    record[3] = 0;
    return record;
}

// FUNCTION: EQMAIN 0x100276b0
// Reads CMOS register 0 for a timing routine; its specific game use is unclear.
unsigned int __stdcall ReadCmosReg0_guess(void)
{
    unsigned int registerValue;

    registerValue = 0;
    __asm
    {
        xor ax, ax
        out 70h, al
        xor ax, ax
        in al, 71h
        mov word ptr registerValue, ax
    }
    return registerValue;
}

// FUNCTION: EQMAIN 0x100276D0
// Reads the RDTSC cycle counter into a high and a low dword (for MeasureCpuSpeed_guess).
// __cdecl, callee pops nothing (RET). Reads the RDTSC counter once and stores its high dword at *tscHighOut and its
// low dword at *tscLowOut. Returns nothing: callers do not read EAX after the call.
void __cdecl ReadTimestampCounter_guess(unsigned int* tscHighOut, unsigned int* tscLowOut)
{
    unsigned int highDword;
    unsigned int lowDword;

    highDword = 0;
    lowDword = 0;
    __asm
    {
        rdtsc
        mov lowDword, eax
        mov highDword, edx
    }
    *tscHighOut = highDword;
    *tscLowOut = lowDword;
}

// The kernel32 imports are called through their IAT slots (call dword ptr [va]) as the original does; HANDLE is void *.
typedef void *(__stdcall *GetCurrentThread_guess_t)(void);
typedef int (__stdcall *GetThreadPriority_guess_t)(void *);
typedef int (__stdcall *SetThreadPriority_guess_t)(void *, int);
#define PTR_GetCurrentThread_guess   EQ_GLOBAL(GetCurrentThread_guess_t, 0x100400EC)
#define PTR_GetThreadPriority_guess  EQ_GLOBAL(GetThreadPriority_guess_t, 0x100400E8)
#define PTR_SetThreadPriority_guess  EQ_GLOBAL(SetThreadPriority_guess_t, 0x100400E4)

// FUNCTION: EQMAIN 0x10027700
// Measures the PC's processor speed for the client.
// __cdecl, callee pops nothing (RET). Measures the clock: raises the thread priority by one (unless it is 0x7FFFFFFF),
// waits for the RTC seconds (ReadCmosReg0_guess, __stdcall, no arguments) to tick over, reads the TSC, waits for the next
// tick, reads it again and restores the priority. Writes the low dword of the TSC difference (cycles per second) to
// *record, the RTC seconds difference times 1000000 to record[1], the cycles in MHz (truncated) to record[2] and
// that rounded up when the next decimal digit is 6 or more to record[3]. Returns record (callers read EAX after the call).
unsigned int* __cdecl MeasureCpuSpeed_guess(unsigned int* record)
{
    void* hThread;
    int nPriority;
    int rtcTick;
    int startTick;
    unsigned int tickAdvanced;
    unsigned int floorMegahertz;
    int differenceHigh[5];
    unsigned int cycleDelta;
    int startTscHigh;
    unsigned int startTscLow;
    int endTscHigh;
    unsigned int endTscLow;

    hThread = PTR_GetCurrentThread_guess();
    differenceHigh[1] = 0;
    differenceHigh[2] = 0;
    differenceHigh[3] = 0;
    differenceHigh[4] = 0;
    nPriority = PTR_GetThreadPriority_guess(hThread);
    if (nPriority != 0x7fffffff)
    {
        PTR_SetThreadPriority_guess(hThread, nPriority + 1);
    }
    rtcTick = EQ_STDCALL(int, 0x100276B0, (void))();
    do
    {
        startTick = EQ_STDCALL(int, 0x100276B0, (void))();
        if (startTick < rtcTick)
        {
            tickAdvanced = (startTick - rtcTick) + 10;
        }
        else
        {
            tickAdvanced = (startTick - rtcTick > 0);
        }
    } while (tickAdvanced == 0);
    EQ_CDECL(void, 0x100276D0, (int *, unsigned int *))(&startTscHigh, &startTscLow);
    do
    {
        rtcTick = EQ_STDCALL(int, 0x100276B0, (void))();
        if (rtcTick < startTick)
        {
            tickAdvanced = (rtcTick - startTick) + 10;
        }
        else
        {
            tickAdvanced = (rtcTick - startTick > 0);
        }
    } while (tickAdvanced == 0);
    EQ_CDECL(void, 0x100276D0, (int *, unsigned int *))(&endTscHigh, &endTscLow);
    if (nPriority != 0x7fffffff)
    {
        PTR_SetThreadPriority_guess(hThread, nPriority);
    }
    EQ_CDECL(int, 0x10027860, (int, unsigned int, int, unsigned int, int *, int *))(startTscHigh, startTscLow, endTscHigh, endTscLow, differenceHigh, (int *)&cycleDelta);
    floorMegahertz = cycleDelta / 1000000;
    tickAdvanced = floorMegahertz;
    if (5 < cycleDelta / 100000 + floorMegahertz * -10)
    {
        tickAdvanced = floorMegahertz + 1;
    }
    *record = cycleDelta;
    record[1] = rtcTick * 1000000 - startTick * 1000000;
    record[2] = floorMegahertz;
    record[3] = tickAdvanced;
    return record;
}

// FUNCTION: EQMAIN 0x10027860
// Calculates a 64-bit value difference for an unspecified client operation.
// @early-stop: EBP/EBX framing, the low-word spill point, and output register order still differ after eight non-improving iterations.
// __cdecl, callee pops nothing (RET). Subtracts (subtrahendHigh:subtrahendLow) from (minuendHigh:minuendLow) as one 64-bit value: the
// low dword goes to *differenceLowOut and the high dword to *differenceHighOut. Returns *differenceLowOut read back after both stores, so it is
// the high dword's store that shows through if differenceHighOut and differenceLowOut point at the same dword.
int __cdecl SubtractQword_guess(int subtrahendHigh, unsigned int subtrahendLow, int minuendHigh, unsigned int minuendLow, int* differenceHighOut, int* differenceLowOut)
{
    unsigned __int64 difference;
    struct QwordResult_guess
    {
        int high;
        unsigned int low;
    };
    volatile QwordResult_guess result; /* fake match: keep the result halves in the original 8-byte local slot. */

    difference = (((unsigned __int64)(unsigned int)minuendHigh << 32) | minuendLow) -
        (((unsigned __int64)(unsigned int)subtrahendHigh << 32) | subtrahendLow);
    result.low = (unsigned int)difference;
    result.high = (int)(difference >> 32);

    *differenceLowOut = result.low;
    *differenceHighOut = result.high;
    return *differenceLowOut;
}

typedef int (__stdcall *MessageBoxAFunction_guess)(void*, const char*, const char*, unsigned int);

// eqmain.dll's own globals (this image's VAs, read from the asm at 0x100278A0)
#define g_fatal_text_prefix_guess  EQ_GLOBAL(char*, 0x10055A14)
#define g_fatal_caption_guess      EQ_GLOBAL(char*, 0x10055A2C)
#define g_eqmain_errtext_guess     EQ_ARRAY(char, 0x100D5B54)
#define g_messagebox_a_guess       EQ_GLOBAL(MessageBoxAFunction_guess, 0x100401F4)

// FUNCTION: EQMAIN 0x100278A0
// Shows a fatal client error and exits.
// __cdecl: variadic (format is a format, its arguments follow on the stack), so the caller pops them; the BUNDLE's
// __stdcall is Ghidra's guess. Never returns: it ends in _exit(0). Copies strlen(g_fatal_text_prefix_guess) bytes of that string
// (no terminator) into a 512-byte buffer, copies g_eqmain_drive_base_guess with its terminator to g_eqmain_errtext_guess, calls
// EQMain_ShutdownArtPaks_guess(0) (cdecl: the caller's ADD ESP,0x10 pops its char argument; ECX is the 0 the REP MOVSB left, not a
// this pointer), formats format with vsprintf when it isn't NULL, then MessageBoxA(NULL, buffer, g_fatal_caption_guess, 0x51010).
void __cdecl FatalErrorExit_guess(char* format, ...)
{
    char messageBuffer[512];

    EQ_CDECL(char*, 0x10033330, (char*, const char*, unsigned int))(messageBuffer, g_fatal_text_prefix_guess,
        strlen(g_fatal_text_prefix_guess));
    strcpy(g_eqmain_errtext_guess, g_eqmain_drive_base_guess);
    EQ_CDECL(void, 0x1001B280, (char))(0);
    if (format != 0)
    {
        // the first vararg: what va_start(args, format) expands to, without a va_list local (the frame stays 0x200)
        EQ_CDECL(int, 0x10034436, (char*, const char*, va_list))(messageBuffer, format,
            (va_list)&format + sizeof(format));
    }
    g_messagebox_a_guess(0, messageBuffer, g_fatal_caption_guess, 0x51010);
    // fake match: _exit never returns, so use a callee-pop type and suppress its dead cleanup.
    EQ_STDCALL(void, 0x10035E98, (int))(0);
    __assume(0);
}

// FUNCTION: EQMAIN 0x10027950
// Clears state in a record; its UI purpose is unclear.
// Clears the dword at +0 and the word at +0x40 of a record and returns the record (the caller at 0x10027C80
// uses the returned pointer). __fastcall: the record comes in ECX; the record's layout is unknown.
unsigned int* __fastcall ClearRecordHead_guess(unsigned int* record)
{
    *record = 0;
    *(unsigned short*)((char*)record + 0x40) = 0;
    return record;
}
