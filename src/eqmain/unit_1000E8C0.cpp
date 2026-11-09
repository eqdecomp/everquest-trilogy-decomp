// unit_1000E8C0.cpp - eqmain.dll, the address run 0x1000E8C0..0x1000ECC0 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// eqmain.dll's own globals (this image's addresses): the PackFs tree object at 0x1004CEF8 (find and insert `this`), its
// head pointer at 0x1004CEFC (what find returns for an absent key) and the shared object pointer at 0x1004CF0C.
#define g_packfs_end_guess    EQ_GLOBAL(int, 0x1004CEFC)
#define g_packfs_shared_guess EQ_GLOBAL(int *, 0x1004CF0C)

// The two critical-section imports, called through their IAT slots (`call dword ptr [va]`, as the original does).
typedef void (__stdcall *EnterCriticalSection_guess_t)(void *);
typedef void (__stdcall *LeaveCriticalSection_guess_t)(void *);
#define g_EnterCriticalSection_guess EQ_GLOBAL(EnterCriticalSection_guess_t, 0x1004014C)
#define g_LeaveCriticalSection_guess EQ_GLOBAL(LeaveCriticalSection_guess_t, 0x10040148)

// The original's locals as one block. The sub-esp area of the original is 0x50 bytes (the EH record takes the top 0xC), so
// this struct is that whole area: each field sits at the stack offset the asm shows after the prologue (the comments), and
// a callee given the address of a local gets the original's address.
struct EQMain_PackFsFrame_guess
{
    char pad_10;            // 0x10 (unused)
    char local_4f;          // 0x11 the return value (AL)
    char local_4e;          // 0x12
    char local_4d;          // 0x13
    int *local_4c;          // 0x14
    int local_48;           // 0x18
    int *local_44;          // 0x1C
    int local_40;           // 0x20
    int *local_3c;          // 0x24
    int *local_38;          // 0x28
    char local_34[4];       // 0x2C
    char local_30[4];       // 0x30
    int *local_2c;          // 0x34
    int local_28;           // 0x38
    int local_24;           // 0x3C
    int local_20;           // 0x40
    unsigned int local_1c;  // 0x44
    int local_18;           // 0x48
    int local_14;           // 0x4C
    int local_10;           // 0x50
    char pad_54[0xC];       // 0x54..0x5F: the EH record in the original, unused here
};

// The original's `push ecx` temporary and the two calls on it. The value is the last argument, so it is pushed first and sits
// at the stack address the original's pushed temporary does: PackFsFunctionHolder_guess::setFunction_guess takes that address as `this`, and assignReferenceHandle_guess
// gets the value as its stack argument.
static void __cdecl EQMain_PackFsTemp_guess(int *dest, char *p3, int v)
{
    EQ_METHOD(1, int *, 0x100103E0, (int *, int), &v)((int)p3);
    EQ_METHOD(1, int *, 0x10010AB0, (int *, int), dest)(v);
}

// FUNCTION: EQMAIN 0x1000e8c0
// Reads an archive file's directory (PFS-like: offset, count, 12-byte entries) and caches it under key, once
// __stdcall, two stack arguments (callee pops 8 bytes). Returns a char (AL). Runs under the critical section at 0x1004CEE0.
// fileNameStr is a refcounted string object (its data pointer at +8). When the key (key) is not in the tree at 0x1004CEF8
// (find returns the head at 0x1004CEFC), it opens the file named by fileNameStr, reads a 4-byte table offset and two more
// dwords, seeks to the offset and reads a 4-byte record count. A non-empty table is read whole (count x 12 bytes) and its
// records are copied, last to first, into the node found for each record's first dword (+0x10..+0x18). Then a new entry
// is built and inserted under key, and the return value is 1. Returns 0 when the key is present or the file cannot be
// read. The EH frame's destructible locals are written out as plain code.
char __stdcall EQMain_PackFsLoadIndex_guess(int *fileNameStr, int *key)
{
    EQMain_PackFsFrame_guess frame;
    int node;
    int *ptr;
    char *newObj;
    int tmp;
    char *_Filename;
    void *recordBuffer;
    int *recordTable;
    unsigned int bytesRead;
    int *findResult;
    int *value;
    unsigned int _MaxCharCount;
    int recordOffset;

    g_EnterCriticalSection_guess((void *)0x1004CEE0);
    frame.local_4f = 0;
    EQ_METHOD(0, void, 0x1000EEB0, (int *), (int *)&fileNameStr)();
    frame.local_44 = key;
    EQ_METHOD(2, void, 0x1000FAD0, (void *, int *, int *), (void *)0x1004CEF8)(&frame.local_40, (int *)&frame.local_44);
    if (g_packfs_end_guess == frame.local_40)
    {
        ptr = EQ_CDECL(int *, 0x100338E5, (unsigned int))(0x14);
        frame.local_3c = ptr;
        if (ptr == 0)
        {
            ptr = 0;
        }
        else
        {
            EQ_METHOD(0, void, 0x1000EEE0, (int *), ptr)();
            ptr[2] = 0;
            ptr[3] = 0;
            ptr[4] = 0;
            frame.local_44 = EQ_CDECL(int *, 0x100338E5, (unsigned int))(0x10);
            if (frame.local_44 == 0)
            {
                newObj = 0;
                recordTable = 0;     // ECX as the failed operator new left it (a register, not a C value): 0 here
            }
            else
            {
                newObj = EQ_METHOD(2, char *, 0x1000F3C0, (int *, char *, char *), frame.local_44)(&frame.local_4d, &frame.local_4e);
                recordTable = frame.local_44;    // ECX as init_guess left it, taken as its `this`
            }
            EQMain_PackFsTemp_guess(ptr + 2, newObj, (int)recordTable);
        }
        frame.local_4c = 0;
        if (ptr != 0)
        {
            frame.local_3c = EQ_CDECL(int *, 0x100338E5, (unsigned int))(0xc);
            if (frame.local_3c == 0)
            {
                frame.local_4c = 0;
            }
            else
            {
                frame.local_4c = EQ_METHOD(1, int *, 0x10010C90, (int *, int *), frame.local_3c)(ptr);
            }
            frame.local_4c[1] = frame.local_4c[1] + 1;
        }
        tmp = EQ_METHOD(0, int, 0x10008080, (int *), (int *)&fileNameStr)();
        if (tmp == 0)
        {
            _Filename = 0;
        }
        else
        {
            _Filename = (char *)fileNameStr[2];
        }
        ptr = EQ_CDECL(int *, 0x10034D0D, (char *, int))(_Filename, 0x8000);
        frame.local_3c = ptr;
        if (ptr != (int *)-1)
        {
            EQ_CDECL(unsigned int, 0x10034917, (int, void *, unsigned int))((int)ptr, (void *)&frame.local_44, 4);
            EQ_CDECL(unsigned int, 0x10034917, (int, void *, unsigned int))((int)ptr, (void *)frame.local_34, 4);
            EQ_CDECL(unsigned int, 0x10034917, (int, void *, unsigned int))((int)ptr, (void *)frame.local_30, 4);
            EQ_CDECL(long, 0x10034C35, (int, long, int))((int)ptr, (long)frame.local_44, 0);
            EQ_CDECL(unsigned int, 0x10034917, (int, void *, unsigned int))((int)ptr, (void *)&frame.local_48, 4);
            tmp = frame.local_48;
            if (frame.local_44 != 0 && frame.local_48 != 0)
            {
                recordBuffer = EQ_CDECL(void *, 0x100338E5, (unsigned int))(frame.local_48 * 0xc);
                frame.local_2c = EQ_CDECL(int *, 0x100338E5, (unsigned int))(0x10);
                if (frame.local_2c == 0)
                {
                    recordTable = 0;
                }
                else
                {
                    recordTable = EQ_METHOD(2, int *, 0x10010DC0, (int *, void *, int), frame.local_2c)(recordBuffer, tmp);
                }
                recordTable[1] = recordTable[1] + 1;
                _MaxCharCount = frame.local_48 * 0xc;
                frame.local_2c = recordTable;
                bytesRead = EQ_CDECL(unsigned int, 0x10034917, (int, void *, unsigned int))((int)ptr, (void *)recordTable[2], _MaxCharCount);
                if (bytesRead == _MaxCharCount)
                {
                    if (-1 < frame.local_48 - 1)
                    {
                        recordOffset = (frame.local_48 - 1) * 0xc;
                        tmp = frame.local_48;
                        do
                        {
                            frame.local_10 = frame.local_20;
                            frame.local_1c = *(unsigned int *)(recordOffset + recordTable[2]);
                            frame.local_18 = frame.local_28;
                            frame.local_14 = frame.local_24;
                            findResult = EQ_METHOD(2, int *, 0x1000FB40, (void *, int *, unsigned int *), *(void **)(*(int *)(frame.local_4c[2] + 8) + 8))(&frame.local_28, &frame.local_1c);
                            node = *findResult;
                            ptr = (int *)(recordOffset + recordTable[2]);
                            recordOffset = recordOffset - 0xc;
                            tmp = tmp - 1;
                            *(int *)(node + 0x10) = *ptr;
                            *(int *)(node + 0x14) = ptr[1];
                            *(int *)(node + 0x18) = ptr[2];
                            ptr = frame.local_3c;
                        } while (tmp != 0);
                    }
                    EQ_METHOD(1, void, 0x10007620, (int *, int *), (int *)frame.local_4c[2])((int *)&fileNameStr);
                    value = g_packfs_shared_guess;
                    if (g_packfs_shared_guess != 0)
                    {
                        if (g_packfs_shared_guess != 0)
                        {
                            g_packfs_shared_guess[1] = g_packfs_shared_guess[1] + 1;
                        }
                        tmp = EQ_METHOD(0, int, 0x10010B70, (int *), (int *)&frame.local_4c)();
                        EQ_METHOD(1, int *, 0x10010B80, (void *, int *), (void *)(tmp + 0xc))(value);
                        if (frame.local_4c != 0)
                        {
                            frame.local_4c[1] = frame.local_4c[1] + 1;
                        }
                        value = frame.local_4c;
                        tmp = EQ_METHOD(0, int, 0x10010B70, (int *), (int *)0x1004CF0C)();
                        EQ_METHOD(1, int *, 0x10010B80, (void *, int *), (void *)(tmp + 0x10))(value);
                    }
                    if (frame.local_4c != 0)
                    {
                        frame.local_4c[1] = frame.local_4c[1] + 1;
                    }
                    EQ_METHOD(1, int *, 0x10010B80, (void *, int *), (void *)0x1004CF0C)(frame.local_4c);
                    EQ_METHOD(0, void, 0x1000EEB0, (int *), (int *)&fileNameStr)();
                    frame.local_3c = key;
                    frame.local_38 = frame.local_4c;
                    if (frame.local_4c != 0)
                    {
                        frame.local_4c[1] = frame.local_4c[1] + 1;
                    }
                    EQ_METHOD(2, void, 0x1000F470, (void *, int *, int *), (void *)0x1004CEF8)(&frame.local_28, (int *)&frame.local_3c);
                    if (frame.local_38 != 0)
                    {
                        EQ_METHOD(0, void, 0x10007520, (int *), frame.local_38)();
                    }
                    frame.local_4f = 1;
                }
                if (recordTable != 0 && (tmp = recordTable[1], recordTable[1] = tmp - 1, tmp - 1 == 0))
                {
                    ((void (__fastcall *)(void *, int, int))(*(void ***)recordTable)[0])((void *)recordTable, 0, 1);
                }
            }
            EQ_CDECL(int, 0x10034B55, (int))((int)ptr);
        }
        if (frame.local_4c != 0 && (tmp = frame.local_4c[1], frame.local_4c[1] = tmp - 1, tmp - 1 == 0) && frame.local_4c != 0)
        {
            ((void (__fastcall *)(void *, int, int))(*(void ***)frame.local_4c)[0])((void *)frame.local_4c, 0, 1);
        }
    }
    g_LeaveCriticalSection_guess((void *)0x1004CEE0);
    if (fileNameStr != 0)
    {
        EQ_METHOD(0, void, 0x10007520, (int *), fileNameStr)();
    }
    return frame.local_4f;
}
