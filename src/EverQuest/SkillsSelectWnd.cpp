// SkillsSelectWnd.cpp - the skills select window: the EverQuest/SkillsSelectWnd subsystem of the queue
// (call-graph labels; the Mac debug map names CSkillsSelectWnd in Source/PC Source/EverQuest/SkillsSelectWnd.cpp).

#include <windows.h>

#include "trilogy/common.h"
#include "trilogy/eqaddr.h"

// No header declares this window yet: minimal local layout, only the fields used here are known.
// A polymorphic child: vtable slot 0 is the deleting destructor (called with 1).
class SkillsSelectChild_guess
{
public:
    virtual void Destroy_guess(int flags);
};

class SkillObject_guess
{
public:
    char data_guess[0x1c];
    // EXTERNAL: TRILOGY 0x004EBD52 SkillObject_guess::SkillObject_guess
    SkillObject_guess(void* lookup, void* surface);
};

class CSkillsSelectWnd_guess
{
public:
    void SetTypesToDisplay_guess();
    void FreeChildren_guess();
    u32 CreateSkillChild_guess(void** lookup, char* name, u32* data, void* destination, u32 skillId);
    bool SetFlag44_guess(int unused1, u8 value, int unused3);

    char pad0_guess[4];                         // 0x00
    SkillsSelectChild_guess* child_guess[16];   // 0x04
    u8 flag44_guess;                            // 0x44
    char pad45_guess;                           // 0x45
    u8 flag46_guess;                            // 0x46
    char pad47_guess[9];                        // 0x47
    u32 value50_guess;                          // 0x50
    void* buffer54_guess;                       // 0x54
    u32 value58_guess;                          // 0x58
    u32 value5c_guess;                          // 0x5c
};

// FUNCTION: TRILOGY 0x004eb8ec
// Initializes the skills selection window's entry storage for use.
// __thiscall (a __fastcall free function: no constructor, the class has no virtuals here). Sets the vtable
// pointer 0x00535860, clears the 16 child slots and the buffer pointer at 0x54, then calls
// SetTypesToDisplay_guess. Returns this.
CSkillsSelectWnd_guess* __fastcall InitSkillsSelectWnd_guess(CSkillsSelectWnd_guess* wnd)
{
    int i;

    *(u32*)wnd = 0x00535860;
    for (i = 0; i < 16; i++)
        wnd->child_guess[i] = 0;
    wnd->buffer54_guess = 0;
    EQ_METHOD(0, void, 0x004EB9B8, (void*), wnd)();
    return wnd;
}

// FUNCTION: TRILOGY 0x004eb90f
// Scalar deleting destructor: sets vtable 0x00535860, runs cleanup 0x004EB9B8, frees itself if flags bit 0.
// __thiscall scalar deleting destructor (a __fastcall free function like its neighbour). Sets the vtable
// pointer 0x00535860, calls SetTypesToDisplay_guess, frees the object when bit 0 of flags is set.
CSkillsSelectWnd_guess* __fastcall DestroyAndDelete_guess(CSkillsSelectWnd_guess* wnd, int edx, u8 flags)
{
    *(u32*)wnd = 0x00535860;
    EQ_METHOD(0, void, 0x004EB9B8, (void*), wnd)();
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void*))(wnd);
    return wnd;
}

// FUNCTION: TRILOGY 0x004eb9b8
// Resets this entry's flags and values, then releases its children and buffer.
// ORIGINAL FILE: EverQuest/SkillsSelectWnd.cpp (from Mac debug map)
// __thiscall, no args. Clears the flags at 0x44/0x46 and the dwords at 0x50/0x58/0x5c, frees the
// children, then frees the buffer at 0x54 and clears it.
void CSkillsSelectWnd_guess::SetTypesToDisplay_guess()
{
    flag44_guess = 0;
    value50_guess = 0;
    flag46_guess = 0;
    value58_guess = 0;
    value5c_guess = 0;
    EQ_METHOD(0, void, 0x004EB9E6, (void*), this)();
    if (buffer54_guess != 0)
        EQ_CDECL(void, 0x005218BB, (void*))(buffer54_guess);
    buffer54_guess = 0;
}

// FUNCTION: TRILOGY 0x004eb9e6
// Removes all child objects from a skills selection entry.
// __thiscall. Deletes each non-null child (slots 15 down to 0) through its deleting destructor and clears
// the slot.
void CSkillsSelectWnd_guess::FreeChildren_guess()
{
    int i = 16;
    SkillsSelectChild_guess** p = &child_guess[15];

    do
    {
        if (*p != 0)
        {
            (*p)->Destroy_guess(1);
            *p = 0;
        }
        p--;
        i--;
    } while (i != 0);
}

// FUNCTION: TRILOGY 0x004ebbca
// Builds a skill entry's display child from its name, id, and supplied data.
// @early-stop: save/guard order, descriptor stores, and new-child EH/call lowering still differ
u32 CSkillsSelectWnd_guess::CreateSkillChild_guess(void** lookup, char* name, u32* data, void* destination, u32 skillId)
{
#pragma pack(push, 1)
    struct SkillData_guess
    {
        u32 value0;
        u32 value4;
        u32 value8;
        u32 valuec;
        u16 value10;
    } copiedData;
#pragma pack(pop)
    struct SurfaceDescription_guess
    {
        u32 bytes;
        u32 format;
        u32 skillId;
        u32 reserved0c;
        u32 copiedData;
        u32 reserved14[4];
    } surfaceDescription;
    /* fake match: preserve the original this, output-pointer, and descriptor-stack homes. */
    struct MatchSlots_guess
    {
        CSkillsSelectWnd_guess* owner;
        void* retrievedData;
        void* descriptorStack;
    } volatile matchSlots;
    int result;
    size_t nameLength;
    void* nameCopy;
    void* provider;

    typedef int (__stdcall *LookupSurface_guess_t)(void*, void*, void**, int);
    typedef int (__stdcall *GetSurfaceData_guess_t)(void*, int, int, void**, u32*, int, int, int);
    typedef u32 (__stdcall *ReleaseSurface_guess_t)(void*);
    typedef int (__stdcall *SetSurfaceData_guess_t)(void*, void*, u32, int, int);

    if (*lookup != 0)
    {
        matchSlots.owner = this;
        matchSlots.descriptorStack = &surfaceDescription;
        if ((name != 0) && (destination != 0) && (skillId != 0) && (child_guess[0] == 0))
        {
            copiedData = *(SkillData_guess*)data;

            value50_guess = skillId;
            *(u16*)((char*)this + 0x4c) = copiedData.valuec;
            *(u32*)((char*)this + 0x48) = copiedData.value4;

            EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(
                &surfaceDescription, 0, 0x24);
            surfaceDescription.skillId = value50_guess;
            surfaceDescription.format = 0x4092;
            surfaceDescription.bytes = 0x24;
            surfaceDescription.copiedData = (u32)&copiedData;
            if (flag46_guess != 0)
                surfaceDescription.format = 0x4090;

            provider = *lookup;
            skillId = 0;
            result = ((LookupSurface_guess_t)(*(void***)provider)[3])(
                provider, &surfaceDescription, (void**)&skillId, 0);
            if (result == 0)
            {
                result = ((GetSurfaceData_guess_t)(*(void***)(u32)skillId)[11])(
                    (void*)skillId, 0, 0, (void**)&matchSlots.retrievedData, (u32*)&data, 0, 0, 2);
                if (result != 0)
                {
                    ((ReleaseSurface_guess_t)(*(void***)(u32)skillId)[2])((void*)skillId);
                }
                else
                {
                    EQ_CDECL(void*, 0x00521B10, (void*, const void*, size_t))(
                        matchSlots.retrievedData, destination, (size_t)data);
                    ((SetSurfaceData_guess_t)(*(void***)(u32)skillId)[19])(
                        (void*)skillId, matchSlots.retrievedData, (u32)data, 0, 0);

                    SkillObject_guess* newChild = new SkillObject_guess(*lookup, (void*)skillId);
                    child_guess[0] = (SkillsSelectChild_guess*)newChild;

                    nameLength = EQ_CDECL(size_t, 0x005223D0, (char*))(name);
                    nameCopy = EQ_CDECL(void*, 0x005218C6, (size_t))(nameLength + 1);
                    buffer54_guess = nameCopy;
                    EQ_CDECL(char*, 0x005219C0, (char*, const char*))((char*)nameCopy, name);
                    value58_guess = 0;
                    return (u32)data;
                }
            }
        }
    }

    return 0xffffffff;
}

// FUNCTION: TRILOGY 0x004ebde3
// Stores the byte argument in the flag at +0x44 and returns true; the other two arguments are unused.
// __thiscall, 3 stack args (`ret 0xc`): stores the second argument (a byte, [esp+8]) at this+0x44, returns
// true. The first and third arguments are unused.
bool CSkillsSelectWnd_guess::SetFlag44_guess(int unused1, u8 value, int unused3)
{
    flag44_guess = value;
    return true;
}

// FUNCTION: TRILOGY 0x004ebdef
// Calculates a child's wrapped offset from the owner's reference position; purpose is unclear.
// __thiscall (a __fastcall free function like its neighbours: `owner` in ECX), 1 stack arg (`ret 0x4`): `index`.
// Returns 0xffffffff unless index is 0..15, child_guess[index] is set, its byte at +0x10 and its dword at +4 are
// non-zero, and owner's flag46 and word at 0x4c are non-zero. Otherwise t = GetPlayPosition_guess(child); d = value50 -
// value58 + t if t < value58, else value58 - t; returns d / word4c when d <= value50, else 0.
// @early-stop: push esi lands after the edx zeroing (original: push esi, then xor edx,edx); 2 of 45 instructions.
/* fake match: the `d > value50` test is written out in both branches so VC6 cross-jumps one shared jbe/div tail. */
u32 __fastcall GetChildOffset_guess(CSkillsSelectWnd_guess* owner, int edx, int index)
{
    SkillsSelectChild_guess* child;
    u32 referenceValue;
    u32 distance;
    u32 childValue;

    if (index < 0 || index >= 0x10 || (child = owner->child_guess[index]) == 0 ||
        *((u8*)child + 0x10) == 0 || owner->flag46_guess == 0 ||
        *(u16*)((char*)owner + 0x4c) == 0 || *(u32*)((char*)child + 4) == 0)
    {
        return 0xffffffff;
    }
    childValue = EQ_METHOD(0, u32, 0x004EC01E, (void*), child)();
    referenceValue = owner->value58_guess;
    if (childValue < referenceValue)
    {
        distance = (owner->value50_guess - referenceValue) + childValue;
        if (distance > owner->value50_guess)
        {
            return 0;
        }
        return distance / *(u16*)((char*)owner + 0x4c);
    }
    distance = referenceValue - childValue;
    if (distance > owner->value50_guess)
    {
        return 0;
    }
    return distance / *(u16*)((char*)owner + 0x4c);
}

// FUNCTION: TRILOGY 0x004ebe59
// Copies incoming data into the child buffer up to its current position.
// @early-stop: stack-slot reuse, the register for the second half's sum, and the first exit merged into the
// second half's tail (the original keeps a separate shared divide block) still differ.
// __thiscall (a __fastcall free function like its neighbours: `owner` in ECX), 3 stack args (`ret 0xc`): `index`,
// `sourceData`, `count`. Returns 0xffffffff unless index is 0..15, child_guess[index] is set, its byte at +0x10 and its
// dword at +4 are non-zero, owner's flag46 and word at 0x4c are non-zero, count is non-zero and value58 <= value50.
// t = GetPlayPosition_guess(child); returns 0 when value58 == t. Otherwise the buffer at child+4 (Lock at vtable +0x2c,
// Unlock at +0x4c) is locked whole and count is overwritten with the locked pointer. count * word4c bytes of sourceData are
// copied in at value58, in up to two pieces (the first up to the end of the buffer, only when t < value58; the
// second up to t). Returns the bytes copied / word4c.
u32 __fastcall WriteStreamData_guess(CSkillsSelectWnd_guess* owner, int edx, int index, void* sourceData, int count)
{
    typedef int (__stdcall *Lock_guess_t)(void*, u32, u32, void**, int*, void**, u32*, u32);
    typedef int (__stdcall *Unlock_guess_t)(void*, void*, u32, void*, u32);
    int* childSlot;
    u32 playPos;
    int tmp;
    void* buffer;
    u32 value;
    u32 firstBytes;
    u32 byteCount;
    int lockedSize;

    if (index < 0 || index >= 0x10)
    {
        return 0xffffffff;
    }
    tmp = (int)owner->child_guess[index];
    if (tmp == 0)
    {
        return 0xffffffff;
    }
    if (*(u8*)((char*)tmp + 0x10) == 0)
    {
        return 0xffffffff;
    }
    if (owner->flag46_guess == 0)
    {
        return 0xffffffff;
    }
    if (*(u16*)((char*)owner + 0x4c) == 0)
    {
        return 0xffffffff;
    }
    if (count == 0)
    {
        return 0xffffffff;
    }
    if (*(u32*)((char*)tmp + 4) == 0)
    {
        return 0xffffffff;
    }
    if (owner->value58_guess > owner->value50_guess)
    {
        return 0xffffffff;
    }
    childSlot = (int*)((char*)owner + 4 + index * 4);
    byteCount = (u32)*(u16*)((char*)owner + 0x4c);
    firstBytes = 0;
    lockedSize = firstBytes;
    byteCount *= count;
    playPos = EQ_METHOD(0, u32, 0x004EC01E, (void*), (void*)tmp)();
    if (owner->value58_guess == playPos)
    {
        return 0;
    }
    buffer = (void*)*(u32*)((char*)*childSlot + 4);
    tmp = ((Lock_guess_t)(*(void***)buffer)[11])
        (buffer, 0, 0, (void**)&count, &lockedSize, 0, 0, 2);
    if (tmp != 0)
    {
        return 0xffffffff;
    }
    if (lockedSize != owner->value50_guess)
    {
        return 0xffffffff;
    }
    value = owner->value58_guess;
    if (playPos < value)
    {
        tmp = count + value;
        firstBytes = lockedSize - value;
        if (firstBytes >= byteCount)
        {
            EQ_CDECL(void*, 0x00521B10, (void*, void*, u32))((void*)tmp, sourceData, byteCount);
            owner->value58_guess += byteCount;
            ((Unlock_guess_t)(*(void***)*(u32*)((char*)*childSlot + 4))[19])
                ((void*)*(u32*)((char*)*childSlot + 4), (void*)count, byteCount, 0, 0);
            value = *(u16*)((char*)owner + 0x4c);
            return byteCount / value;
        }
        EQ_CDECL(void*, 0x00521B10, (void*, void*, u32))((void*)tmp, sourceData, firstBytes);
        owner->value58_guess = 0;
    }
    playPos = playPos - owner->value58_guess;
    byteCount = byteCount - firstBytes;
    tmp = count + owner->value58_guess;
    if (playPos >= byteCount)
    {
        EQ_CDECL(void*, 0x00521B10, (void*, void*, u32))((void*)tmp, (char*)sourceData + firstBytes, byteCount);
        owner->value58_guess += byteCount;
        byteCount = firstBytes + byteCount;
        ((Unlock_guess_t)(*(void***)*(u32*)((char*)*childSlot + 4))[19])
            ((void*)*(u32*)((char*)*childSlot + 4), (void*)count, byteCount, 0, 0);
        value = *(u16*)((char*)owner + 0x4c);
    }
    else
    {
        EQ_CDECL(void*, 0x00521B10, (void*, void*, u32))((void*)tmp, (char*)sourceData + firstBytes, playPos);
        owner->value58_guess += playPos;
        ((Unlock_guess_t)(*(void***)*(u32*)((char*)*childSlot + 4))[19])
            ((void*)*(u32*)((char*)*childSlot + 4), (void*)count, lockedSize, 0, 0);
        value = *(u16*)((char*)owner + 0x4c);
        byteCount = firstBytes + playPos;
    }
    return byteCount / value;
}

class SkillEntryFromData_guess : public CSkillsSelectWnd_guess
{
public:
    char pad_guess[0x9c];
    // EXTERNAL: TRILOGY 0x004EB8EC SkillEntryFromData_guess::SkillEntryFromData_guess
    SkillEntryFromData_guess();
};

// FUNCTION: TRILOGY 0x004ed9c9
// Adds a skill selection entry from supplied data and returns its slot.
// @early-stop: fastcall adapters, register scheduling, and branch layout still differ.
int __fastcall AddSkillEntryFromData_guess(void* owner, int edx, char* name, int index, u32* data,
                            void* destination, u32 skillId, int unused)
{
    CSkillsSelectWnd_guess** entrySlot_guess;
    CSkillsSelectWnd_guess* newEntry_guess;
    int resultIndex;
    int result_guess;
    char buffer[260];

    if (name != 0 && index < 0x2d0 && destination != 0 && skillId != 0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, char*))(buffer, name);
        if (index == -1)
        {
            result_guess = 0;
            entrySlot_guess = (CSkillsSelectWnd_guess**)((char*)owner + 0xc);
            while (result_guess < 0x2d0 && *entrySlot_guess != 0)
            {
                result_guess = result_guess + 1;
                entrySlot_guess = entrySlot_guess + 1;
            }
            if (result_guess == 0x2d0)
                return -1;
            resultIndex = result_guess;
        }
        else
        {
            resultIndex = index;
        }
        entrySlot_guess = (CSkillsSelectWnd_guess**)((char*)owner + 0xc + resultIndex * 4);
        if (*entrySlot_guess == 0)
        {
            newEntry_guess = new SkillEntryFromData_guess;
            *entrySlot_guess = newEntry_guess;
        }
        if ((*entrySlot_guess)->child_guess[0] != 0)
            (*entrySlot_guess)->SetTypesToDisplay_guess();
        result_guess = (*entrySlot_guess)->CreateSkillChild_guess(
            (void**)((char*)owner + 0xb58), buffer, data, destination, skillId);
        if ((u32)result_guess == skillId)
        {
            EQ_GLOBAL(int, 0x006EFEAC) = EQ_GLOBAL(int, 0x006EFEAC) + result_guess;
            (*entrySlot_guess)->SetFlag44_guess(
                (int)((char*)owner + 0xb58), *(u8*)((char*)owner + 0xbc8),
                *(u8*)((char*)owner + 0xbca));
            EQ_METHOD(2, int, 0x004EE419, (void*, char*, int), owner)(buffer, resultIndex);
            return resultIndex;
        }
    }
    return -1;
}

struct SkillEntryManager_guess
{
    char padding0[0x0c];
    CSkillsSelectWnd_guess* entries[0x2d0];
    char paddingB4C[0x0c];
    void* lookup;
    char paddingB5C[0x6c];
    u8 flagBC8;
    char paddingBC9;
    u8 flagBCA;
};

class SkillEntryFromName_guess : public CSkillsSelectWnd_guess
{
public:
    char padding[0x9c];
    // EXTERNAL: TRILOGY 0x004EB8EC SkillEntryFromName_guess::SkillEntryFromName_guess
    SkillEntryFromName_guess();
};

// FUNCTION: TRILOGY 0x004EDAEF
// Adds a named skill entry and returns its slot.
int __fastcall AddSkillEntryByName_guess(SkillEntryManager_guess* owner, int unusedEdx, char* name,
                            int index, int unused)
{
    CSkillsSelectWnd_guess** entrySlot;
    SkillEntryFromName_guess* newEntry;
    int resultIndex;
    int skillId;
    char nameBuffer[260];

    if (name != 0 && index >= -1 && index < 0x2d0)
    {
        EQ_CDECL(char*, 0x005219C0, (char*, char*))(nameBuffer, name);
        if (index == -1)
        {
            resultIndex = 0;
            entrySlot = &owner->entries[0];
            do
            {
                if (*entrySlot == 0)
                    break;
                resultIndex = resultIndex + 1;
                entrySlot = entrySlot + 1;
            } while (resultIndex < 0x2d0);
            if (resultIndex == 0x2d0)
                return -1;
        }
        else
        {
            resultIndex = index;
        }
        entrySlot = &owner->entries[resultIndex];
        if (*entrySlot == 0)
        {
            newEntry = new SkillEntryFromName_guess;
            *entrySlot = newEntry;
        }
        if ((*entrySlot)->child_guess[0] != 0)
            EQ_METHOD(0, void, 0x004EB9B8, (void*), *entrySlot)();
        skillId = EQ_METHOD(2, int, 0x004EBA06, (void*, void*, char*), *entrySlot)
            (&owner->lookup, nameBuffer);
        if (skillId > 0)
        {
            EQ_GLOBAL(int, 0x006EFEAC) = EQ_GLOBAL(int, 0x006EFEAC) + skillId;
            EQ_METHOD(3, bool, 0x004EBDE3, (void*, int, u8, int), *entrySlot)
                ((int)&owner->lookup, owner->flagBC8, owner->flagBCA);
            EQ_METHOD(2, int, 0x004EE419, (void*, char*, int), owner)
                (nameBuffer, resultIndex);
            return resultIndex;
        }
    }
    return -1;
}

struct SkillEntry_guess
{
    char padding0[4];
    int active;
    char padding8[0x48];
    int skillCount;
};

class SkillEntryCollection_guess
{
public:
    int SubtractEntryValue_guess(int skillIndex);

    char padding0[0x0c];
    SkillEntry_guess* entries[0x2d0];
};

// FUNCTION: TRILOGY 0x004EDC01
// Removes an active entry's contribution from the shared total and clears it.
int SkillEntryCollection_guess::SubtractEntryValue_guess(int skillIndex)
{
    SkillEntry_guess** entrySlot;

    if (skillIndex < 0 || skillIndex >= 0x2d0)
        return 0;
    entrySlot = &entries[skillIndex];
    if (*entrySlot != 0 && (*entrySlot)->active != 0)
    {
        EQ_GLOBAL(int, 0x006EFEAC) = EQ_GLOBAL(int, 0x006EFEAC) - (*entrySlot)->skillCount;
        EQ_METHOD(0, void, 0x004EB9B8, (void*), *entrySlot)();
    }
    return 1;
}

// The COM-style lookup object at owner+0xb50: __stdcall methods, `this` pushed. Slot 7 (0x1c) maps a name
// to an id, slot 8 (0x20) fills a 0x5c-byte record for an id.
struct SkillRecord_guess
{
    u32 data[23];                               // 0x00
};

class SkillLookup_guess
{
public:
    virtual void __stdcall Slot00_guess();
    virtual void __stdcall Slot04_guess();
    virtual void __stdcall Slot08_guess();
    virtual void __stdcall Slot0C_guess();
    virtual void __stdcall Slot10_guess();
    virtual void __stdcall Slot14_guess();
    virtual void __stdcall Slot18_guess();
    virtual void __stdcall FindByName_guess(char* name, int* id);           // 0x1c
    virtual void __stdcall GetRecord_guess(int id, SkillRecord_guess* out); // 0x20
};

// FUNCTION: TRILOGY 0x004EDC3A
// Runs an indexed operation for every entry; the operation's purpose is unclear.
// __thiscall (a __fastcall free function: the owner class is unknown), `ret` with no operand. Its address is in a
// vtable (005358AC). For each index 0..0x2CF in turn, calls the object's vtable slot 4 (+0x10) with `this` in ECX
// and the index as the one stack argument (the callee pops it). Returns nothing; EAX is left as the last call's result.
void __fastcall ProcessAllIndices_guess(int* owner)
{
    struct SkillOwner_guess
    {
        virtual void Slot00_guess() = 0;
        virtual void Slot04_guess() = 0;
        virtual void Slot08_guess() = 0;
        virtual void Slot0C_guess() = 0;
        virtual void Slot10_guess(int index) = 0;
    };
    int loopIndex;

    loopIndex = 0;
    do
    {
        ((SkillOwner_guess*)owner)->Slot10_guess(loopIndex);
        loopIndex = loopIndex + 1;
    } while (loopIndex < 0x2d0);
    return;
}

// FUNCTION: TRILOGY 0x004ee419
// Copies the matching named record into the selected entry.
// __thiscall (a __fastcall free function: the owner class is unknown), `ret 8`. With no lookup object at
// owner+0xb50 returns 0. Otherwise copies the name to a 260-byte buffer, looks up its id (starts at 0),
// fetches that id's record and copies it to (entry[index] at owner+0xc)+0x60; returns -1 when the id is -1,
// else 1.
// @early-stop: vtable load follows the first arg's lea; rep count and return xor are scheduled differently.
int __fastcall LookupAndCopyRecord_guess(void* owner, int edx, char* name, int entryIndex)
{
    char buffer[260];
    SkillRecord_guess record;
    int id;

    if (*(SkillLookup_guess**)((char*)owner + 0xb50) != 0)
    {
        id = 0;
        strcpy(buffer, name);
        (*(SkillLookup_guess**)((char*)owner + 0xb50))->FindByName_guess(buffer, &id);
        (*(SkillLookup_guess**)((char*)owner + 0xb50))->GetRecord_guess(id, &record);
        *(SkillRecord_guess*)(*(char**)((char*)owner + 0xc + entryIndex * 4) + 0x60) = record;
        return id == -1 ? -1 : 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004ee498
// Initializes an object's state; its game purpose is unclear.
// __thiscall, 2 stack args (`ret 8`); the owner class is unknown, so this is a __fastcall free function like LookupAndCopyRecord_guess.
// Stores arg2 at owner+0x28 and zeroes the byte at +0x34. Its high byte goes to the bytes at +0x2c and +0x2d, then
// 0x004eed88 is called with ECX = owner+0x2c. Then arg1 goes to +0x4, 0.0f to +0x8, +0xc and +0x10, and the vtable
// 0x535968 to +0x0. Returns owner.
// The original keeps an EBP frame (push ebp; mov ebp,esp) with no stack locals. VC6 /O1 omits that frame here, and
// no local or do-while shape reproduced it, so the pragma turns frame-pointer omission off for this function only.
// The callee is a thiscall, so the original never sets EDX: the dummy EDX argument is the caller's own `edx`.
// @early-stop: zero store hoisted above the +0x28 store, high byte not reloaded for +0x2d, vtable store and `mov eax, esi` scheduled early.
#pragma optimize("y", off)
void* __fastcall InitializeObject_guess(void* instance, int edx, u32 arg1, u32 arg2)
{
    char* self = (char*)instance;
    char* sub;

    *(u32*)(self + 0x28) = arg2;
    *(u8*)(self + 0x34) &= 0;
    sub = self + 0x2c;
    sub[0] = ((u8*)&arg2)[3];
    sub[1] = ((u8*)&arg2)[3];
    EQ_THISCALL(void, 0x004EED88, (void *, int))(sub, edx);
    *(float*)(self + 0x8) = 0.0f;
    *(u32*)(self + 0x4) = arg1;
    *(float*)(self + 0xc) = 0.0f;
    *(u32*)self = 0x00535968;
    *(float*)(self + 0x10) = 0.0f;
    return self;
}
#pragma optimize("y", on)
