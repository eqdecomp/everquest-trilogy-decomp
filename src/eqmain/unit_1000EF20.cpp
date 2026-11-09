// unit_1000EF20.cpp - eqmain.dll, the address run 0x1000EF20..0x1000EF30 (a queue unit: no Mac map covers
// eqmain.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

// FUNCTION: EQMAIN 0x1000ef20
// Thunk forwarding this object's operation to another implementation.
// __fastcall (this in ECX, the EDX parameter is the dummy), 0 stack args, callee pops 0 bytes. The original is a
// single JMP 0x1000f1d0 into FfResourceOwner_guess::releaseResource_guess, so ECX and EDX pass through unchanged
// and that function returns straight to the caller. VC6 emits no tail call: this is a call followed by a return.
void __fastcall FfResourceOwner_releaseResource_thunk_guess(int *holder, int edx_unused)
{
    EQ_THISCALL(void, 0x1000F1D0, (void *, int))(holder, edx_unused);
}

// The two keys are stack arguments passed by value: a ref-counted name pointer, then its hash, which
// computeStringCrcIfUnset_guess fills in through the key's address (so each key is 8 bytes, adjacent).
struct FfKey_guess
{
    int *ref;           // 0x0  name object: vtable at +0, refcount at +4
    unsigned int hash;  // 0x4
};

// A name object: its virtual destructor is slot 0 of the vtable (called with the delete flag), and its reference
// count is at +4. Released with `delete` once the count reaches zero.
class FfNameObject_guess
{
public:
    virtual ~FfNameObject_guess();
    int refcount;       // 0x4
};

// A local reference to a name object. It drops its reference when its scope ends. The function body that holds one
// is a block with the exit label inside it, so LeaveCriticalSection runs before the reference is dropped.
class FfNameRef_guess
{
public:
    int *p;             // 0x0  the name object, or 0

    FfNameRef_guess()
    {
        p = 0;
    }

    ~FfNameRef_guess()
    {
        FfNameObject_guess *q = (FfNameObject_guess *)p;
        if (q != 0 && --q->refcount == 0)
        {
            delete q;
        }
    }
};

// A by-value copy of a name reference, passed as a stack argument. The copy takes its own count on the name object;
// nothing is released when it goes away (the callee keeps it), so it has no destructor.
class FfNameCopy_guess
{
public:
    int *p;             // 0x0  the name object, or 0

    FfNameCopy_guess(const FfNameCopy_guess &src)
    {
        p = src.p;
        if (p != 0)
        {
            p[1]++;
        }
    }
};

// KERNEL32 imports, called through their import slots (`call dword ptr [va]`, as the original).
typedef void (__stdcall *CriticalSectionFn_guess)(void *cs);
#define EnterCriticalSection_iat_guess  EQ_GLOBAL(CriticalSectionFn_guess, 0x1004014C)
#define LeaveCriticalSection_iat_guess  EQ_GLOBAL(CriticalSectionFn_guess, 0x10040148)

// FUNCTION: EQMAIN 0x1000ef30
// Looks up and opens a file inside a resource pack for front-end use.
// __fastcall (this in ECX, the EDX parameter is the dummy), 4 stack args (two FfKey_guess by value), callee pops
// 16 bytes. Returns AL: 1 when a new file entry was stored in *self, else 0. Holds the critical section throughout.
// @early-stop: EH state values, packIt/keyHash stack slots (swapped), and EQ_THISCALL's dummy EDX differ.
unsigned char __fastcall FfResourceOwner_openPackEntry_guess(int *self, int edx_unused, FfKey_guess key1, FfKey_guess key3)
{
    unsigned char *entryIt;
    unsigned char *entryNode;
    int *fileEntry;
    FfNameObject_guess *nameObj;
    int value;
    int fileOffset;
    char *filename;
    unsigned char result;

    EnterCriticalSection_iat_guess(EQ_ARRAY(void, 0x1004CEE0));
    EQ_METHOD(0, void, 0x1000F1D0, (int *), self)();
    {
        FfNameRef_guess packRef;
        int packIt;
        unsigned int keyHash;
        EQ_METHOD(0, void, 0x1000EEB0, (FfKey_guess *), &key1)();
        keyHash = key1.hash;
        EQ_METHOD(2, void, 0x1000FAD0, (void *, int *, unsigned int *), EQ_ARRAY(void, 0x1004CEF8))(&packIt, &keyHash);
        if (EQ_GLOBAL(int, 0x1004CEFC) != packIt)
        {
            value = EQ_METHOD(0, int, 0x1000F3B0, (int *), &packIt)();
            EQ_METHOD(1, void, 0x10010B80, (FfNameRef_guess *, FfNameCopy_guess), &packRef)(*(FfNameCopy_guess *)(value + 4));
            EQ_METHOD(0, void, 0x1000EEB0, (FfKey_guess *), &key3)();
            keyHash = key3.hash;
            EQ_METHOD(2, void, 0x1000FC30, (void *, int *, unsigned int *), *(void **)(*(int *)(packRef.p[2] + 8) + 8))((int *)&entryIt, &keyHash);
            entryNode = entryIt;
            if (entryIt != *(unsigned char **)(*(int *)(*(int *)(packRef.p[2] + 8) + 8) + 4))
            {
                fileEntry = (int *)EQ_CDECL(void *, 0x100338E5, (unsigned int))(0x2024);
                if (fileEntry != 0)
                {
                    fileOffset = *(int *)(entryNode + 0x14);
                    fileEntry[1] = -1;
                    fileEntry[0x802] = 0;
                    fileEntry[0x803] = 0;
                    fileEntry[0x804] = 0;
                    fileEntry[0x805] = fileOffset;
                    EQ_METHOD(1, void, 0x10007540, (int *, char *), fileEntry + 0x806)(0);
                    fileEntry[0x808] = 0;
                    *fileEntry = 0x10041f50;
                }
                else
                {
                    fileEntry = 0;
                }
                value = *(int *)packRef.p[2];
                if (value == 0 || *(unsigned int *)(value + 0xc) <= 0)
                {
                    filename = 0;
                }
                else
                {
                    filename = *(char **)(value + 8);
                }
                value = EQ_CDECL(int, 0x10034D0D, (char *, int))(filename, 0x8000);
                fileEntry[1] = value;
                fileEntry[0x802] = *(int *)(entryNode + 0x18);
                EQ_CDECL(long, 0x10034C35, (int, int, int))(value, fileEntry[0x805], 0);
                if (fileEntry != 0)
                {
                    value = ((int (__fastcall *)(int *, int))(*(int *)*fileEntry))(fileEntry, 0);
                    if (value > 0)
                    {
                        int *tmp;
                        fileEntry[0x803] = 0;
                        fileEntry[0x804] = 0;
                        EQ_METHOD(1, void, 0x100103C0, (int **, int *), &tmp)((int *)&key3);
                        EQ_METHOD(1, void, 0x10007FE0, (int *, int *), fileEntry + 0x806)(tmp);
                        result = 1;
                        fileEntry[0x807] = key3.hash;
                        *(int **)self = fileEntry;
                        goto leaveLock; // as original: the success JMP skips the `= 0` store to the shared LeaveCriticalSection tail
                    }
                    nameObj = (FfNameObject_guess *)fileEntry[0x806];
                    if (nameObj != 0 && --nameObj->refcount == 0)
                    {
                        delete nameObj;
                    }
                    EQ_CDECL(void, 0x10033800, (int *))(fileEntry);
                }
            }
        }
        result = 0;
    leaveLock:
        LeaveCriticalSection_iat_guess(EQ_ARRAY(void, 0x1004CEE0));
    }
    {
        FfNameObject_guess *ref = (FfNameObject_guess *)key1.ref;
        if (ref != 0)
        {
            if (--ref->refcount == 0)
            {
                delete ref;
            }
        }
    }
    {
        FfNameObject_guess *ref = (FfNameObject_guess *)key3.ref;
        if (ref != 0)
        {
            if (--ref->refcount == 0)
            {
                delete ref;
            }
        }
    }
    return result;
}
