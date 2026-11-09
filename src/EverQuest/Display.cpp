// Display.cpp - CDisplay methods.
// ORIGINAL FILE: Source/PC Source/EverQuest/Display.cpp (from Mac debug map)

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "trilogy/display.h"
#include "trilogy/character.h"
#include "trilogy/everquest.h"
#include "trilogy/eqgfx.h"
#include "trilogy/eqswitch.h"
#include "trilogy/globals.h"
#include "trilogy/net_structs.h"
#include "trilogy/sound.h"
#include "trilogy/layout/eq_main.h"
#include "trilogy/layout/eq_charcreate.h"
#include "trilogy/eq_textio.h"
#include "trilogy/layout/eq_effect.h"

// The client's debug output functions, called by name like the CRT string functions: EQTRACE (an empty
// `ret` in this build), DbgLog_guess (EverQuest.cpp) and fdebug (ffcreate2 verify). A direct call has
// its arguments popped right after it, as in the client; a call through EQ_CDECL's constant function pointer
// lets VC6 merge the pops of neighbouring calls.
#undef EQTRACE_guess
#undef LogMessage_guess
// EXTERNAL: TRILOGY 0x004A3D01 EQTRACE_guess
void __cdecl EQTRACE_guess(const char *format, ...);
// EXTERNAL: TRILOGY 0x004C1703 DbgLog_guess
int __cdecl DbgLog_guess(const char *format, ...);
// EXTERNAL: TRILOGY 0x004F8220 fdebug
int __cdecl fdebug(const char *format, ...);
#define LogMessage_guess DbgLog_guess

typedef void (__cdecl *t3dHierarchicalSpriteUpdateAllTransformations_guess_t)(void*, void*, void*);
typedef void (__cdecl *t3dGetCurrentDagObjectToWorldTransform_guess_t)(void*, float*, float*);
typedef int* (__cdecl *t3dGetActorCollisionVolume_guess_t)(void*);
typedef void* (__cdecl *t3dGetActorUserData_guess_t)(void*);
typedef float (__cdecl *t3dFloatFastMath_guess_t)(float);
typedef int (__cdecl *t3dGetRegionFromWorldAndXYZ_guess_t)(void*, float*);
typedef int (__cdecl *t3dGetRegionFromWorldAndXYZWithSine_guess_t)(void*, float*, float);
typedef int (__cdecl *s3dCollideSphereWithWorld_guess_t)(void*, void*, float*, float*, float,
                                                        void*, void*, int, void*, void*, float*, int);
typedef void (__cdecl *t3dGetZonesContainingWorldXYZPosition_guess_t)(void*, u32*, void*);
typedef void* (__cdecl *t3dGetZoneUserData_guess_t)(void*);
typedef void (__cdecl *t3dGetObjectTag_guess_t)(void*, char*);
typedef void* (__cdecl *s3dGetMaterialPaletteEntry_guess_t)(void*, void*, int);
typedef int (__cdecl *t3dGetObjectTagResult_guess_t)(void*, char*);
typedef int (__cdecl *s3dSetMaterialPaletteEntry_guess_t)(void*, void*, int, void*);
typedef int (__stdcall *WritePrivateProfileStringA_guess_t)(const char*, const char*, const char*, const char*);
typedef void* (__cdecl *t3dGetHierarchicalSpriteDagByName_guess_t)(void*, char*);
typedef void (__cdecl *t3dScreenCapture_guess_t)(char*);
typedef int (__cdecl *t3dGetRegionNumberFromWorldAndXYZ_guess_t)(void*, float*);
typedef int (__cdecl *s3dGetWorldVisibleActorList_guess_t)(void*, int, float*, float, int, void*);
typedef void (__cdecl *t3dSetLightDefinitionColorIntensityList_guess_t)(void*, float*);
typedef float (__cdecl *t3dFloatSine_guess_t)(float);
typedef float (__cdecl *t3dFloatCosine_guess_t)(float);
typedef void (__cdecl *t3dSetDirectionalLightNormal_guess_t)(void*, float*);

// Number of 0x20-byte zone records in the sound environment table that starts at 0x00578690 (SetZoneSoundEnvironments_guess).
#define g_zone_env_count_guess      EQ_GLOBAL(int, 0x00578688)

// 0x005917B4: UseD3DTextureCompression ini setting (0 or 1).
#define g_useD3DTextureCompression_guess EQ_GLOBAL(int, 0x005917B4)
// 0x00590180: t3d device index (0x15C-byte entries from 0x005901B0).
#define g_deviceIndex_guess         EQ_GLOBAL(int, 0x00590180)
// 0x006B685C: gamma level (float).
#define g_gammaLevel_guess          EQ_GLOBAL(float, 0x006B685C)
// 0x006ECDF4: music state object (its +0x60 is the MIDI driver).
#define g_musicState_guess          EQ_GLOBAL(void*, 0x006ECDF4)
// 0x00591784..0x0059179C: Arial font handles, one per size.
#define g_arialFont6_guess          EQ_GLOBAL(int, 0x00591784)
#define g_arialFont7_guess          EQ_GLOBAL(int, 0x00591788)
#define g_arialFont8_guess          EQ_GLOBAL(int, 0x0059178C)
#define g_arialFont9_guess          EQ_GLOBAL(int, 0x00591790)
#define g_arialFont10_guess         EQ_GLOBAL(int, 0x00591794)
#define g_arialFont12_guess         EQ_GLOBAL(int, 0x00591798)
#define g_arialFont14_guess         EQ_GLOBAL(int, 0x0059179C)
// 0x006EFDF4..0x006EFE0C: Arial HFONTs made in EQ_TextIO (sizes 10, 12, 14, 15, 16, 20, 24).
#define g_hFontArial10_guess        EQ_GLOBAL(u32, 0x006EFDF4)
#define g_hFontArial12_guess        EQ_GLOBAL(u32, 0x006EFDF8)
#define g_hFontArial14_guess        EQ_GLOBAL(u32, 0x006EFDFC)
#define g_hFontArial15_guess        EQ_GLOBAL(u32, 0x006EFE00)
#define g_hFontArial16_guess        EQ_GLOBAL(u32, 0x006EFE04)
#define g_hFontArial20_guess        EQ_GLOBAL(u32, 0x006EFE08)
#define g_hFontArial24_guess        EQ_GLOBAL(u32, 0x006EFE0C)
// 0x006E5480: nonzero queues text for t3dDeferTextA; zero draws it with GDI.
#define g_textDeferred_guess        EQ_GLOBAL(u32, 0x006E5480)

// Owner class unknown: an object whose first dword is the previous value (AllocateValueHolder_guess calls it).
struct PreviousValueHolder_guess
{
    u32 previous_value; // 0x00

    int updatePreviousValue_guess(u32 value);
};

// FUNCTION: TRILOGY 0x004010c6
// __thiscall. Stores the value at +0 and returns 0.
int PreviousValueHolder_guess::updatePreviousValue_guess(u32 value)
{
    previous_value = value;
    return 0;
}

// FUNCTION: TRILOGY 0x004010D1
// __thiscall (ECX only, no stack args). Clears the dword at offset 0 of the object and returns 0.
int __fastcall ClearFirstDword_guess(u32 *self)
{
    *self = 0;
    return 0;
}

// FUNCTION: TRILOGY 0x0040197B
// Resets selected values in a display state block; its in-game role is unclear.
// __thiscall (ECX only). Resets the object (owner class unknown): floats at 0x14/0x18 are zeroed,
// then the dwords at 0/8/0xC/0x10, then the floats again. Returns the object in EAX.
u32* __fastcall ResetFields_guess(u32* p)
{
    float *temp4 = ((float*)p);
    float *temp3 = temp4;
    ((float *)(((float *)(((float *)(temp3))))))[5] = 0.0f;
    ((float *)(((float *)(((float *)(temp3))))))[6] = 0.0f;
    p[0] = 0;
    p[2] = 0;
    p[3] = 0;
    p[4] = 0;
    {
        float *temp2 = ((float*)p);
        float *temp = temp2;
        ((float *)(((float *)(((float *)(((float *)(temp))))))))[5] = 0.0f;
        ((float *)(((float *)(((float *)(temp))))))[6] = ((float *)(((float *)(((float *)(temp))))))[5];
    }
    return p;
}

// The allocated object is 0x18 bytes and its constructor is at 0x0040107B.
// Its full type is not declared in the headers used by this translation unit.
struct DisplayAllocatedValue_guess
{
    unsigned char storage[0x18];

    DisplayAllocatedValue_guess()
    {
        EQ_METHOD(0, void, 0x0040107B, (void*), this)();
    }
};

// FUNCTION: TRILOGY 0x0040199f
// Stores the given pointer at +0, allocates a 0x18-byte helper at +4 and hands it this object.
int __fastcall AllocateValueHolder_guess(u32* self, int edx, u32 storedValue)
{
    DisplayAllocatedValue_guess* allocated_value;

    self[0] = storedValue;
    allocated_value = new DisplayAllocatedValue_guess;
    self[1] = (u32)allocated_value;
    EQ_METHOD(1, int, 0x004010C6, (void*, u32), allocated_value)((u32)self);
    return 0;
}

// FUNCTION: TRILOGY 0x004019fb
// Releases an allocated display object that holds a value.
// __fastcall (ECX = self, no stack args). Tears down the value that AllocateValueHolder_guess allocated into
// self[1]: clears its first dword, runs its destructor, then frees it. Quirks kept to match the
// binary: the first call has no null check on self[1], and self[1] still points at the freed block.
// Returns 0.
int __fastcall FreeAllocatedValue_guess(u32* self)
{
    int allocated_value;

    EQ_METHOD(0, void, 0x004010D1, (void*), (void*)self[1])();
    allocated_value = self[1];
    if (allocated_value != 0)
    {
        EQ_METHOD(0, void, 0x0040108D, (void*), (void*)allocated_value)();
        EQ_CDECL(void, 0x005218BB, (void*))((void*)allocated_value);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00401EDB
// Initializes a compact state block; its in-game role is unclear.
// __thiscall (ECX only). Constructor-like (returns this in EAX): zeroes the floats at 0x14/0x18
// and the first two dwords (owner class unknown).
u32* __fastcall ResetFieldsWithFloats_guess(u32* p)
{
    float *temp;
    float *temp2 = ((float*)p);
    temp = ((float *)(temp2));
    ((float *)temp)[6] = ((float *)temp)[5] = 0.0f;
    unsigned long temp3 = p[0];
    p[0] = (char)(char)(int)0 & temp3;
    p[1] &= 0;
    return p;
}

// FUNCTION: TRILOGY 0x00401eef
// Clears the first two dwords of the object in ECX (owner class unknown).
void __fastcall ClearFirstTwoDwords_guess(u32* p)
{
    p[0] &= 0;
    p[1] &= 0;
}

// The two input objects InitializeDirectInput_guess creates: the client's constructors, only declared here.
struct FirstObject_guess
{
    char storage[0x248];

    // EXTERNAL: TRILOGY 0x00403977 FirstObject_guess::FirstObject_guess
    FirstObject_guess();
};
struct SecondObject_guess
{
    char storage[0x224];

    // EXTERNAL: TRILOGY 0x004020C5 SecondObject_guess::SecondObject_guess
    SecondObject_guess();
};

// FUNCTION: TRILOGY 0x00401EF7
// Creates a DirectInput interface for display input setup.
int __fastcall InitializeDirectInput_guess(u32* p, int unusedEdx, u32 ownerObject, u32 secondValue)
{
    FirstObject_guess* firstObject;
    SecondObject_guess* secondObject;
    int result;

    p[0] = ownerObject;
    p[1] = secondValue;
    result = EQ_STDCALL(int, 0x00500404, (u32, u32, void**, void*))(
        EQ_GLOBAL(u32, 0x006EE720), 0x700, (void**)&p[2], 0);
    if (result < 0)
        return 1;

    firstObject = new FirstObject_guess;
    p[3] = (u32)firstObject;
    EQ_METHOD(1, void, 0x00403A06, (void*, void*), (void*)firstObject)((void*)p[2]);

    secondObject = new SecondObject_guess;
    p[4] = (u32)secondObject;
    EQ_METHOD(1, void, 0x004020E2, (void*, void*), (void*)secondObject)((void*)p[2]);
    return 0;
}

// FUNCTION: TRILOGY 0x00401fa6
// Tears down the two interface objects owned by this setup.
int __fastcall DestroyOwnedObjects_guess(u32* p)
{
    int value;

    value = p[3];
    if (value != 0)
    {
        EQ_METHOD(0, void, 0x004039CC, (void*), (void*)value)();
        EQ_CDECL(void, 0x005218BB, (void*))((void*)value);
    }
    value = p[4];
    if (value != 0)
    {
        EQ_METHOD(0, void, 0x004020D7, (void*), (void*)value)();
        EQ_CDECL(void, 0x005218BB, (void*))((void*)value);
    }
    p[0] &= 0;
    p[1] &= 0;
    p[3] &= 0;
    p[4] &= 0;
    return 0;
}

// FUNCTION: TRILOGY 0x00402037
// Copies a float pair to +0x14/+0x18 and, if an object is linked at +0xC, to its ints at +0x22C/+0x230.
// __thiscall, 1 stack arg (owner class unknown). Copies two floats to +0x14/+0x18, then, when the
// pointer at +0xC is set, stores them truncated to int at +0x22C/+0x230 of that object.
void __fastcall SetFloatPair_guess(char* self, int edx_unused, u32* xy)
{
    *(u32*)(0x14 + self) = xy[0];
    *(u32*)(self + 0x18) = xy[1];
    char ** temp2 = (char**)(self + 0xC);
    if (!*temp2)
    {
    }
    else
    {
        char *temp = (self + 0xC);
        *(int*)(*(char**)temp + 0x22C) = (int)*(float*)(self + 0x14);
        *(int*)(*(char**)(0xC + self) + 0x230) = (int)*(float*)(self + 0x18);
    }
}

// FUNCTION: TRILOGY 0x00402075
// Returns the dword at +0x224 of the object linked at +0xC, or -50 when none is linked.
// __thiscall (ECX only). Reads the dword at +0x224 of the object pointed to by the pointer at +0xC
// (owner classes unknown); -50 when that pointer is null.
int __fastcall GetLinkedDword224_guess(char* self)
{
    char* target = *(char**)(self + 0xC);
    if (target != 0)
        return *(int*)(target + 0x224);
    return -50;
}

// FUNCTION: TRILOGY 0x00402087
// Returns the dword at +0x228 of the object linked at +0xC, or -50 when none is linked.
// __thiscall (ECX only). Reads the dword at +0x228 of the object pointed to by the pointer at +0xC
// (owner classes unknown); -50 when that pointer is null.
int __fastcall GetLinkedDword228_guess(char* self)
{
    char* target = *(char**)(self + 0xC);
    if (target != 0)
        return *(int*)(target + 0x228);
    return -50;
}

// FUNCTION: TRILOGY 0x004020c5
// Prepares an object's entry storage; its in-game role is unclear.
u32* __fastcall InitWithEntryArray_guess(u32* self)
{
    EQ_METHOD(0, void, 0x0040223A, (u32*), (u32*)((char*)self + 0xC))();
    *self = 0;
    return self;
}

void __fastcall ProcessPointerArrayAndReset_guess(char* self);

// FUNCTION: TRILOGY 0x004020D7
// Releases an object's owned entry collection and resets its state.
// __thiscall (ECX only, owner class unknown). Clears the dword at +0, then resets the sub-object at
// +0xC through ProcessPointerArrayAndReset_guess (tail jump).
void __fastcall ResetObjectState_guess(u32* self)
{
    *self = 0;
    ProcessPointerArrayAndReset_guess((char*)(self + 3));
}

// FUNCTION: TRILOGY 0x004020E2
// Configures an attached device interface for use by the display object.
int __fastcall InitializeInterface_guess(void* self, int unusedEdx, void* interfaceObject)
{
    typedef int (__stdcall *Method0_guess_t)(void*);
    typedef int (__stdcall *Method1_guess_t)(void*, void*);
    typedef int (__stdcall *Method2_guess_t)(void*, void*, int);
    typedef int (__stdcall *Method3_guess_t)(void*, void*, void*, int);
    typedef int (__stdcall *Method4_guess_t)(void*, int, void*);
    void** interfaceOut;
    u32 propDword[5];

    *(void**)self = interfaceObject;
    *(void**)((char*)self + 8) = EQ_CDECL(void*, 0x005218C6, (u32))(0x200);
    interfaceOut = (void**)((char*)self + 4);
    if (((Method3_guess_t)(*(void***)(*(void**)self))[3])(*(void**)self,
                                                           (void*)0x00536ED8, interfaceOut, 0) < 0)
        return 2;
    if (((Method1_guess_t)(*(void***)*interfaceOut)[11])(*interfaceOut, (void*)0x00536E58) < 0)
        return 3;
    if (((Method2_guess_t)(*(void***)*interfaceOut)[13])(*interfaceOut, EQ_GLOBAL(void*, 0x006EE7BC), 10) < 0)
        return 4;

    EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(propDword, 0, 0x14);
    propDword[2] = 0;
    propDword[3] = 0;
    propDword[0] = 0x14;
    propDword[1] = 0x10;
    propDword[4] = 0x20;
    if (((Method4_guess_t)(*(void***)*interfaceOut)[6])(*interfaceOut, 1, propDword) < 0)
        return 5;
    if (((Method0_guess_t)(*(void***)*interfaceOut)[7])(*interfaceOut) == 0
        || ((Method0_guess_t)(*(void***)*interfaceOut)[7])(*interfaceOut) >= 0)
        return 0;
    return 1;
}

// FUNCTION: TRILOGY 0x0040223a
// Prepares fixed 32-entry storage for client records.
u32* __fastcall InitializeEntryBlockArray_guess(u32* arrayState)
{
    arrayState[0] = 0x20;
    arrayState[1] = 0;
    arrayState[2] = 0;
    arrayState[3] = 0;
    arrayState[0x84] = 0;
    arrayState[0x85] = 0;
    EQ_METHOD(1, void, 0x0040233C, (void*, int), arrayState)(0x20);
    arrayState[0] = 0x20;
    return arrayState;
}

// FUNCTION: TRILOGY 0x00402267
// Releases the entries held in an object's pointer collection.
// __thiscall (ECX only, owner class unknown). For each of the u32 count at +0x210 pointer slots from
// +0x10, deletes the non-null ones through the ClearedPair_guess vector deleting destructor
// (0x004022F4, flags 3) without clearing the slot; then zeroes the dwords at +4 and +0xC.
void __fastcall ProcessPointerArrayAndReset_guess(char* self)
{
    u32 i;

    for (i = 0; i < *(u32*)(self + 0x210); i++)
    {
        if (((void**)(self + 0x10))[i] != 0)
            EQ_METHOD(1, void*, 0x004022F4, (void*, u8), ((void**)(self + 0x10))[i])(3);
    }
    *(u32*)(self + 4) &= 0;
    *(u32*)(self + 0xC) &= 0;
}

// Owner class unknown: an 8-byte object whose destructor clears the dword at +4 (the destructor
// itself is at 0x004023D3).
struct ClearedPair_guess
{
    u32 first;  // 0x00
    u32 second; // 0x04

    void* VectorDeletingDtor_guess(u8 flags);
};

// FUNCTION: TRILOGY 0x004022F4
// Destroys an allocated array of paired values; their in-game meaning is unclear.
// __thiscall, one stack argument. Compiler-style vector deleting destructor: with bit 1 of flags the
// array (count in the dword before this) is destroyed by `eh vector destructor iterator'
// (0x00521E45) with the destructor 0x004023D3, otherwise the inlined destructor clears +4; bit 0
// frees the block. Returns the block start.
void* ClearedPair_guess::VectorDeletingDtor_guess(u8 flags)
{
    if (flags & 2)
    {
        u32* block = (u32*)this - 1;
        EQ_STDCALL(void, 0x00521E45, (void*, u32, u32, void*))(this, 8, *block, (void*)0x004023D3);
        if (flags & 1)
            EQ_CDECL(void, 0x005218BB, (void*))(block);
        return block;
    }
    second = 0;
    if (flags & 1)
        EQ_CDECL(void, 0x005218BB, (void*))(this);
    return this;
}

// FUNCTION: TRILOGY 0x004023DF
// Prepares a three-value state block for display setup.
// __thiscall (ECX only). Constructor-like (returns this in EAX): zeroes the first three dwords
// (owner class unknown).
u32* __fastcall ClearThreeDwords_guess(u32* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    return p;
}

// FUNCTION: TRILOGY 0x004023EC
// Clears a three-value block used during owner cleanup.
// __thiscall (ECX only), no return value. Zeroes the first three dwords of the object in ECX
// (owner class unknown; the name is a low-confidence TAKP import).
void __fastcall CSocialEditWnd_Activate2_guess(u32* p)
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
}

// Owner class unknown: an object whose first dword is a value that is stored before recycling.
struct RecycleValueHolder_guess
{
    u32 value; // 0x00

    int storeAndRecycle_guess(u32 new_value);
};

// FUNCTION: TRILOGY 0x004023F7
// Stores a value, then calls a shared client routine of unclear purpose.
// __thiscall, one stack argument. Stores the value at +0, calls recycleObject_guess (0x0050ED90,
// __cdecl, no arguments) and returns 0.
int RecycleValueHolder_guess::storeAndRecycle_guess(u32 new_value)
{
    value = new_value;
    EQ_CDECL(void, 0x0050ED90, (void))();
    return 0;
}

// FUNCTION: TRILOGY 0x00402407
// __cdecl, no arguments. Calls FUN_0050edd0 (it runs FUN_0051c170 and FUN_0051a250) and returns 0.
int CallRoutine_guess(void)
{
    EQ_CDECL(void, 0x0050EDD0, (void))();
    return 0;
}

// Owner class unknown: an object whose dword at +0x08 keeps the parsed copy of its file.
struct TextFileLoader_guess
{
    char unknown_00[8]; // 0x00, not used by this function
    void* parsed_guess; // 0x08

    int* loadParsedFile_guess(char* fileName);
};

// Owner class unknown: an interface seen through its vtable, slot 3 (+0x0c, one flag argument, returns a
// char).
class VtblSlot3_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual char vfunc3(int flag);
};

// FUNCTION: TRILOGY 0x00402412
// Loads and parses a named file as display data.
// __thiscall, one stack argument (file name, callee pops 4 bytes). Reads the file named by fileName
// whole (mode from 0x0053C2BC) and parses it with FUN_0050ed70. If that gives a non-null result, wraps it
// through UiElementTree_guess::CreateUIElementTree_guess and returns the wrapper when its vtable slot 3 with 1 returns nonzero. Frees the
// parsed copy and closes the file before returning.
int* TextFileLoader_guess::loadParsedFile_guess(char* fileName)
{
    int* result;
    size_t len;
    FILE* file;
    long size;
    char* buffer;
    void* parsed;
    u32 parsedValue;
    int* wrappedResult;
    char accepted;

    result = 0;
    buffer = 0;
    len = strlen(fileName);
    if (len > 0)
    {
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(fileName, (const char*)0x0053C2BC);
        if (file != 0)
        {
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 2);
            size = EQ_CDECL(long, 0x0052214F, (FILE*))(file);
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
            buffer = (char*)EQ_CDECL(void*, 0x005218C6, (unsigned int))(size + 1);
            EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(buffer, size, 1, file);
            buffer[size] = 0;
            parsed = EQ_CDECL(void*, 0x0050ED70, (char*, long))(buffer, size);
            this->parsed_guess = parsed;
            if (parsed != 0)
            {
                parsedValue = EQ_CDECL(u32, 0x00510540, (void*))(parsed);
                wrappedResult = EQ_METHOD(2, int*, 0x004024E6, (void*, u32, int), this)(parsedValue, 0);
                accepted = ((VtblSlot3_guess*)wrappedResult)->vfunc3(1);
                if (accepted != 0)
                    result = wrappedResult;
            }
            EQ_CDECL(void, 0x0050F2A0, (void*))(this->parsed_guess);
            EQ_CDECL(void, 0x005218BB, (void*))(buffer);
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        }
    }
    return result;
}

// Owner class unknown: an object whose first virtual slot takes a flag (1 = delete, as for a scalar
// deleting destructor).
struct VirtualDeletable_guess
{
    virtual void destroy_guess(int flag);
};

// FUNCTION: TRILOGY 0x004024D5
// Wrapper for an object's first virtual operation; its purpose is unclear.
// One stack argument, callee pops it (RET 4); ECX is only a scratch register. If the object is not
// null, calls its first virtual function with 1.
void __stdcall InvokeFirstVirtualWithFlag_guess(VirtualDeletable_guess* obj)
{
    if (obj != 0)
        obj->destroy_guess(1);
}

// Sub-object at +0x28 (0x14 bytes). Its constructor is BoxDrawCtor403250_guess::BoxDrawCtor403250_guess, called by address. The destructor is an
// empty out-of-line stand-in (the real body is unknown).
class Member28_guess
{
public:
    Member28_guess()
    {
        EQ_METHOD(0, void, 0x00403250, (char*), (char*)this)();
    }
    ~Member28_guess();

    u32 data[5]; // 0x00..0x13
};

// Sub-object at +0x1844 (size not known; 0x20 assumed). Its constructor is InitBlock48_guess, called by address.
// Destructor: the same empty out-of-line stand-in as Member28_guess.
class Member1844_guess
{
public:
    Member1844_guess()
    {
        EQ_METHOD(0, void, 0x0040186A, (char*), (char*)this)();
    }
    ~Member1844_guess();

    u32 data[8]; // 0x00..0x1f
};

Member28_guess::~Member28_guess()
{
}

Member1844_guess::~Member1844_guess()
{
}

// Owner class unknown: the object DisplayObject_guess::DisplayObject_guess constructs. Offsets are the ones the constructor touches; the
// dword at +0x18 and the 8 bytes at +0x00 are never written by it.
class DisplayObject_guess
{
public:
    DisplayObject_guess();

    char pad_00[8];          // 0x00
    u32 f_08;                // 0x08
    u32 f_0c;                // 0x0c
    u32 f_10;                // 0x10
    u32 f_14;                // 0x14
    u32 f_18;                // 0x18
    u32 f_1c;                // 0x1c
    u32 f_20;                // 0x20
    u32 f_24;                // 0x24
    Member28_guess m_28;     // 0x28
    float f_3c;              // 0x3c
    float f_40;              // 0x40
    char buf_44[0x800];      // 0x44
    char buf_844[0x1000];    // 0x844
    Member1844_guess m_1844; // 0x1844
    int init_guess(int a0, int result);    /* 0x0040296E */
};

// FUNCTION: TRILOGY 0x00402874
// Initializes the base state for a display resource object.
// Constructor, __thiscall (this in ECX, returns this in EAX). Member constructors run in declaration order:
// BoxDrawCtor403250_guess::BoxDrawCtor403250_guess on +0x28, then 0.0 to the floats at +0x3c and +0x40, then InitBlock48_guess on +0x1844. The body
// zeroes the dwords at +0x08..+0x24 (not +0x18) and memsets +0x44 (0x800 bytes) and +0x844 (0x1000 bytes).
DisplayObject_guess::DisplayObject_guess()
    : m_28(), f_3c(0.0f), f_40(0.0f), m_1844()
{
    f_08 = 0;
    f_0c = 0;
    f_10 = 0;
    f_14 = 0;
    f_1c = 0;
    f_20 = 0;
    f_24 = 0;
    EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(buf_44, 0, 0x800);
    EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(buf_844, 0, 0x1000);
}

// Owner class unknown: an interface seen through its vtable, slots 2 (+0x08, no arguments) and 16
// (+0x40, one pointer argument).
class VtblSlots2And16_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14();
    virtual void vfunc15();
    virtual void vfunc16(float* vec);
};

// Owner class unknown: an interface seen through its vtable, slots 1 (+0x04, no arguments), 14 (+0x38, two
// strings) and 18 (+0x48, one pointer argument). Its dword at +0x04 is the owner (DisplayObject_init_guess).
class VtblSlots1And14And18_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7();
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual void vfunc11();
    virtual void vfunc12();
    virtual void vfunc13();
    virtual void vfunc14(const char* a, const char* b);
    virtual void vfunc15();
    virtual void vfunc16();
    virtual void vfunc17();
    virtual void vfunc18(float* vec);

    void* owner; // 0x04
};

// Objects that DisplayObject_init_guess makes with new (sizes as in the original's operator new calls). Each
// constructor is the client's, only declared here, which gives VC6's new expressions the EH frame.
class DisplayAlloc1_guess
{
public:
    // EXTERNAL: TRILOGY 0x00401011 DisplayAlloc1_guess::DisplayAlloc1_guess
    DisplayAlloc1_guess(); // calls InitializeTiming_guess
};

class DisplayAlloc2_guess
{
public:
    // EXTERNAL: TRILOGY 0x004023DF DisplayAlloc2_guess::DisplayAlloc2_guess
    DisplayAlloc2_guess(); // calls ClearThreeDwords_guess
    char pad_00[0xc];
};

class DisplayAlloc3_guess
{
public:
    // EXTERNAL: TRILOGY 0x0040197B DisplayAlloc3_guess::DisplayAlloc3_guess
    DisplayAlloc3_guess(); // calls ResetFields_guess
    char pad_00[0x1c];
};

class DisplayAlloc4_guess
{
public:
    // EXTERNAL: TRILOGY 0x00401EDB DisplayAlloc4_guess::DisplayAlloc4_guess
    DisplayAlloc4_guess(); // calls ResetFieldsWithFloats_guess
    char pad_00[0x1c];
};

class DisplayAlloc5_guess
{
public:
    // EXTERNAL: TRILOGY 0x00401C2E DisplayAlloc5_guess::DisplayAlloc5_guess
    DisplayAlloc5_guess(); // calls EQStaticTextElement_guess::EQStaticTextElement_guess
    char pad_00[0x100];
};

// FUNCTION: TRILOGY 0x0040296E
// Initializes UI drawing resources and loads test.xml.
// __thiscall on DisplayObject_guess (this in ECX, two stack arguments, callee pops 8, returns 0). Stores the
// arguments at +0x00 and +0x04, then makes five objects with new (1, 0xc, 0x1c, 0x1c and 0x100 bytes, kept at
// +0x1c, +0x10, +0x0c, +0x08, +0x18). Each constructor calls the function at its address, which leaves the
// object in EAX, so the new value is the object. storeAndRecycle_guess and AllocateValueHolder_guess get this as their
// argument; InitializeDirectInput_guess gets this and +0x0c. SetFloatPair_guess gets &+0x3c. The interface at +0x18 gets slot 1,
// its +0x04 = this, slot 18 with {20, 20, 0}, and slot 14 twice. ReplaceObject_guess loads "test.xml".
int DisplayObject_guess::init_guess(int a0, int result)
{
    void* obj;
    float vec[3];

    *(int*)this->pad_00 = a0;
    *(int*)(this->pad_00 + 4) = result;
    obj = new DisplayAlloc1_guess;
    this->f_1c = (u32)obj;
    obj = new DisplayAlloc2_guess;
    this->f_10 = (u32)obj;
    EQ_METHOD(1, void, 0x004023F7, (void*, DisplayObject_guess*), obj)(this);
    obj = new DisplayAlloc3_guess;
    this->f_0c = (u32)obj;
    EQ_METHOD(1, void, 0x0040199F, (void*, DisplayObject_guess*), obj)(this);
    obj = new DisplayAlloc4_guess;
    this->f_08 = (u32)obj;
    EQ_METHOD(2, void, 0x00401EF7, (void*, DisplayObject_guess*, u32), obj)(this, this->f_0c);
    EQ_METHOD(1, void, 0x00402037, (void*, float*), (void*)this->f_08)(&this->f_3c);
    vec[0] = EQ_GLOBAL(float, 0x00534498);
    vec[1] = vec[0];
    vec[2] = 0.0f;
    obj = new DisplayAlloc5_guess;
    this->f_18 = (u32)obj;
    ((VtblSlots1And14And18_guess*)obj)->vfunc1();
    ((VtblSlots1And14And18_guess*)this->f_18)->owner = this;
    ((VtblSlots1And14And18_guess*)this->f_18)->vfunc18(vec);
    ((VtblSlots1And14And18_guess*)this->f_18)->vfunc14((const char*)0x0053C2B4, (const char*)0x0053C314);
    ((VtblSlots1And14And18_guess*)this->f_18)->vfunc14((const char*)0x0053C2FC, (const char*)0x0053C308);
    EQ_METHOD(1, void, 0x00402C19, (void*, const char*), this)((const char*)0x0053C2F0);
    return 0;
}

// Constructors of the DisplayAlloc*_guess objects: each calls the original function at its address with this in
// ECX. They sit after DisplayObject_init_guess because a non-inline constructor defined later gets the EH frame.
// FUNCTION: TRILOGY 0x00402B1D
// Releases an owner's attached objects and resources during display shutdown.
// __thiscall (ECX only, owner class unknown). Tears down the sub-objects at +0x8, +0xc and +0x10 (the
// +0x10 one first gets InvokeFirstVirtualWithFlag_guess with +0x14), deletes them and the block at +0x1c, clears the slots. Returns 0.
int __fastcall DestroyOwnedSubobjects_guess(char* self)
{
    void* obj;
    void* owned;

    EQ_METHOD(1, void, 0x004024D5, (char*, int), *(char**)(self + 0x10))(*(int*)(self + 0x14));
    obj = *(void**)(self + 8);
    if (obj != 0)
        EQ_METHOD(0, void, 0x00401FA6, (void*), obj)();
    obj = *(void**)(self + 0xc);
    if (obj != 0)
        EQ_METHOD(0, void, 0x004019FB, (void*), obj)();
    obj = *(void**)(self + 0x10);
    if (obj != 0)
        EQ_METHOD(0, void, 0x00402407, (void*), obj)();
    owned = *(void**)(self + 0x10);
    if (owned != 0)
    {
        EQ_METHOD(0, void, 0x004023EC, (void*), owned)();
        EQ_CDECL(void, 0x005218BB, (void*))(owned);
    }
    owned = *(void**)(self + 0xc);
    if (owned != 0)
    {
        EQ_METHOD(0, void, 0x0040101D, (void*), owned)();
        EQ_CDECL(void, 0x005218BB, (void*))(owned);
    }
    owned = *(void**)(self + 8);
    if (owned != 0)
    {
        EQ_METHOD(0, void, 0x00401EEF, (void*), owned)();
        EQ_CDECL(void, 0x005218BB, (void*))(owned);
    }
    EQ_CDECL(void, 0x005218BB, (void*))(*(void**)(self + 0x1c));
    *(int*)(self + 0x10) &= 0;
    *(int*)(self + 0xc) &= 0;
    *(int*)(self + 8) &= 0;
    return 0;
}

// FUNCTION: TRILOGY 0x00402BA9
// Submits the linked object's size vector for a display interface update.
// __thiscall (ECX only, owner class unknown). Calls slot 2 of the interface at +0x14 when set, then
// passes (width, height, 1.0) of the object at +8 (GetLinkedDword224_guess / GetLinkedDword228_guess, read as unsigned) to
// slot 16 of the interface at +0x18 and calls its slot 2.
void __fastcall SubmitVectorAndUpdate_guess(char* self)
{
    float vec[3];
    float height;

    if (*(VtblSlots2And16_guess**)(self + 0x14) != 0)
        (*(VtblSlots2And16_guess**)(self + 0x14))->vfunc2();
    vec[0] = 0.0f;
    vec[1] = 0.0f;
    vec[2] = 1.0f;
    height = (float)(u32)EQ_METHOD(0, int, 0x00402087, (char*), *(char**)(self + 8))();
    vec[0] = (float)(u32)EQ_METHOD(0, int, 0x00402075, (char*), *(char**)(self + 8))();
    vec[1] = height;
    vec[2] = 1.0f;
    (*(VtblSlots2And16_guess**)(self + 0x18))->vfunc16(vec);
    (*(VtblSlots2And16_guess**)(self + 0x18))->vfunc2();
}

// FUNCTION: TRILOGY 0x00402C19
// Replaces the active resource with one loaded from a path, then releases the old one.
// __thiscall, 1 stack arg (owner class unknown). Keeps the old pointer at +0x14, stores in its place the object
// TextFileLoader_guess::loadParsedFile_guess returns for the arg (called with the pointer at +0x10 as this), calls slot 11 of the new object
// when it is set, then passes the old pointer to InvokeFirstVirtualWithFlag_guess (this = pointer at +0x10) when it was set. Returns 0.
int __fastcall ReplaceObject_guess(char* self, int edx_unused, char* objectPath)
{
    int oldObject;
    int* newObject;

    oldObject = *(int*)(self + 0x14);
    newObject = EQ_THISCALL(int*, 0x00402412, (char*, int, char*))(*(char**)(self + 0x10), edx_unused, objectPath);
    *(int**)(self + 0x14) = newObject;
    if (newObject != 0)
        ((VtblSlots2And16_guess*)newObject)->vfunc11();
    if (oldObject != 0)
        EQ_METHOD(1, void, 0x004024D5, (char*, int), *(char**)(self + 0x10))(oldObject);
    return 0;
}

// FUNCTION: TRILOGY 0x00402C4E
// Stores two ints as floats at +0x3C/+0x40 and forwards the pair to the object at +8 when it is set.
// __thiscall, 2 stack args (owner class unknown). Stores the two ints as floats at +0x3C/+0x40, then,
// when the pointer at +8 is set, passes them to SetFloatPair_guess on that object.
void __fastcall SetFloatPairFromInts_guess(char* self, int edx_unused, int x, int y)
{
    *(float*)(self + 0x3C) = (float)x;
    *(float*)(self + 0x40) = (float)y;
    if (*(char**)(self + 8) != 0)
        EQ_METHOD(1, void, 0x00402037, (char*, float*), *(char**)(self + 8))((float*)(self + 0x3C));
}

// FUNCTION: TRILOGY 0x00403977
// Initializes default display state and sizing values for an interface object.
u32* __fastcall InitializeObjectDefaults_guess(u32* objectState)
{
    EQ_METHOD(0, void, 0x0040223A, (u32*), objectState + 3)();
    objectState[0x8b] = 0x280;
    *objectState = 0;
    objectState[0x89] = 0;
    objectState[0x8a] = 0;
    objectState[0x8e] = 0;
    objectState[0x8f] = 0;
    objectState[0x90] = 0;
    objectState[0x91] = 0;
    objectState[0x8c] = 0x1e0;
    objectState[0x8d] = 300;
    return objectState;
}

struct FUN_004039cc_subobject_guess
{
    // EXTERNAL: TRILOGY 0x00402267 FUN_004039cc_subobject_guess::~FUN_004039cc_subobject_guess
    ~FUN_004039cc_subobject_guess();
};

struct FUN_004039cc_owner_guess
{
    u32 first_value; // 0x00
    u32 unknown04; // 0x04
    void* allocated_value; // 0x08
    FUN_004039cc_subobject_guess subobject; // 0x0C

    ~FUN_004039cc_owner_guess();
};

// FUNCTION: TRILOGY 0x004039cc
// Frees the object's owned allocation before destroying its member at +0x0c.
FUN_004039cc_owner_guess::~FUN_004039cc_owner_guess()
{
    first_value = 0;
    EQ_CDECL_CPP(void, 0x005218BB, (void*))(allocated_value);
}

struct FUN_00403a06_owner_guess
{
    u32 InitializeInterfaceWithBuffer_guess(u32 param_2);
};

// FUNCTION: TRILOGY 0x00403A06
// Configures an attached interface and allocates its backing buffer.
u32 FUN_00403a06_owner_guess::InitializeInterfaceWithBuffer_guess(u32 interfaceObject)
{
    typedef int (__stdcall *method_0c_guess_t)(void*, void*, u32*, int);
    typedef int (__stdcall *method_0c_local_guess_t)(void*, u32*);
    typedef int (__stdcall *method_2c_guess_t)(void*, void*);
    typedef int (__stdcall *method_34_guess_t)(void*, u32, int);
    typedef int (__stdcall *method_18_guess_t)(void*, int, void*);
    typedef int (__stdcall *method_1c_guess_t)(void*);
    u32* interfaceOut;
    void* allocatedBuffer;
    int result;
    u32 paramBlock[16];
    u32* ownerState = (u32*)this;

    *ownerState = interfaceObject;
    allocatedBuffer = EQ_CDECL(void*, 0x005218C6, (u32))(0x800);
    ownerState[2] = (u32)allocatedBuffer;
    interfaceOut = ownerState + 1;
    result = ((method_0c_guess_t)(*(void***)ownerState[0])[3])((void*)ownerState[0],
        EQ_ARRAY(char, 0x00536EE8), interfaceOut, 0);
    if (result < 0)
        return 2;
    paramBlock[0] = 0x2c;
    ((method_0c_local_guess_t)(*(void***)*interfaceOut)[3])((void*)*interfaceOut, &paramBlock[0]);
    result = ((method_2c_guess_t)(*(void***)*interfaceOut)[11])((void*)*interfaceOut,
        EQ_ARRAY(char, 0x00536E70));
    if (result < 0)
        return 4;
    result = ((method_34_guess_t)(*(void***)*interfaceOut)[13])((void*)*interfaceOut,
        EQ_GLOBAL(u32, 0x006EE7BC), 5);
    if (result < 0)
        return 5;
    EQ_CDECL(void*, 0x00521960, (void*, int, u32))(&paramBlock[11], 0, 0x14);
    paramBlock[13] = 0;
    paramBlock[14] = 0;
    paramBlock[11] = 0x14;
    paramBlock[12] = 0x10;
    paramBlock[15] = 0x80;
    result = ((method_18_guess_t)(*(void***)*interfaceOut)[6])((void*)*interfaceOut,
        1, &paramBlock[11]);
    if (result < 0)
        return 6;
    result = ((method_1c_guess_t)(*(void***)*interfaceOut)[7])((void*)*interfaceOut);
    if (result < 0)
        return 7;
    return 0;
}

// FUNCTION: TRILOGY 0x00405DBF
// Creates an actor at its requested position, trying nearby positions if placement fails.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int __cdecl r3dCreateActorEx(int firstValue, int actorDefinition, int thirdValue,
    float* actorLocation, int sphere, int sixthValue, float scalarValue, float actorRadius,
    int ninthValue, int tenthValue, int playerCharacter)
{
    typedef int (__cdecl *t3dCreateActorEx_guess_t)(int, int, int, float*, int, int,
        float, float, int, int);
    float* positionPtr;
    t3dCreateActorEx_guess_t createActor;
    int creationResult;
    int playerActorInfo;
    float originalPosition[7];
    int usedFallbackPosition;
    int i;

    positionPtr = actorLocation;
    usedFallbackPosition = 0;
    EQ_CDECL(void*, 0x00521B10, (void*, const void*, u32))(originalPosition, actorLocation, 0x1c);
    createActor = EQ_GLOBAL(t3dCreateActorEx_guess_t, 0x006E5180);
    creationResult = createActor(firstValue, actorDefinition, thirdValue, actorLocation, sphere, sixthValue,
        scalarValue, actorRadius, ninthValue, tenthValue);
    if (creationResult == 0)
    {
        usedFallbackPosition = 1;
        *positionPtr = *positionPtr - 5.0f;
        for (i = 0; i < 10; i++)
        {
            creationResult = createActor(firstValue, actorDefinition, thirdValue, positionPtr, sphere, sixthValue,
                scalarValue, actorRadius, ninthValue, tenthValue);
            if (creationResult != 0)
                goto updatePlayerPosition;
            *positionPtr = *positionPtr + 1.0f;
        }
        *positionPtr = originalPosition[0];
        positionPtr[1] = positionPtr[1] - 5.0f;
        for (i = 0; i < 10; i++)
        {
            creationResult = createActor(firstValue, actorDefinition, thirdValue, positionPtr, sphere, sixthValue,
                scalarValue, actorRadius, ninthValue, tenthValue);
            if (creationResult != 0)
                goto updatePlayerPosition;
            positionPtr[1] = positionPtr[1] + 1.0f;
        }
        *positionPtr = originalPosition[0];
        positionPtr[1] = originalPosition[1];
        positionPtr[2] = positionPtr[2] - 5.0f;
        for (i = 0; i < 10; i++)
        {
            creationResult = createActor(firstValue, actorDefinition, thirdValue, positionPtr, sphere, sixthValue,
                scalarValue, actorRadius, ninthValue, tenthValue);
            if (creationResult != 0)
                goto updatePlayerPosition;
            positionPtr[2] = positionPtr[2] + 1.0f;
        }
        *positionPtr = originalPosition[0] - 5.0f;
        positionPtr[1] = originalPosition[1] - 5.0f;
        positionPtr[2] = originalPosition[2] - 5.0f;
        for (i = 0; i < 10; i++)
        {
            creationResult = createActor(firstValue, actorDefinition, thirdValue, positionPtr, sphere, sixthValue,
                scalarValue, actorRadius, ninthValue, tenthValue);
            if (creationResult != 0)
                goto updatePlayerPosition;
            *positionPtr = *positionPtr + 1.0f;
            positionPtr[1] = positionPtr[1] + 1.0f;
            positionPtr[2] = positionPtr[2] + 1.0f;
        }
        *positionPtr = EQ_GLOBAL(float, 0x006B6734);
        positionPtr[1] = EQ_GLOBAL(float, 0x006B6738);
        positionPtr[2] = EQ_GLOBAL(float, 0x006B673C);
        creationResult = createActor(firstValue, actorDefinition, thirdValue, positionPtr, sphere, sixthValue,
            scalarValue, actorRadius, ninthValue, tenthValue);
        if (creationResult == 0)
            return creationResult;
    }
updatePlayerPosition:
    if ((playerCharacter != 0) && (usedFallbackPosition != 0))
    {
        *(u32*)((char*)playerCharacter + 0x34) = *(u32*)positionPtr;
        *(u32*)((char*)playerCharacter + 0x38) = *(u32*)(positionPtr + 1);
        *(u32*)((char*)playerCharacter + 0x3c) = *(u32*)(positionPtr + 2);
        playerActorInfo = *(int*)((char*)playerCharacter + 0x74);
        if (playerActorInfo != 0)
        {
            *(u32*)((char*)playerActorInfo + 0x964) = *(u32*)((char*)playerCharacter + 0x34);
            *(u32*)((char*)playerActorInfo + 0x968) = *(u32*)((char*)playerCharacter + 0x38);
            *(u32*)((char*)playerActorInfo + 0x96c) = *(u32*)((char*)playerCharacter + 0x3c);
        }
    }
    return creationResult;
}

typedef void (__cdecl *t3dFreeReferenceList_guess_t)(void*);
typedef void* (__stdcall *LocalFree_guess_t)(void*);
typedef void (__cdecl *t3dCleanUpLibrary_guess_t)();

// Function-pointer slots the display shutdown calls through (GetProcAddress'd or imported).
#define g_pfn_t3dFreeReferenceList_guess    EQ_GLOBAL(t3dFreeReferenceList_guess_t, 0x006E5268)
#define g_pfn_LocalFree_guess               EQ_GLOBAL(LocalFree_guess_t, 0x00534184)
#define g_pfn_t3dCleanUpLibrary_guess       EQ_GLOBAL(t3dCleanUpLibrary_guess_t, 0x006E5170)

// FUNCTION: TRILOGY 0x0040601F
// Shuts down shared 3D engine resources when the game display closes.
// Display shutdown: ECX = the CDisplay. Frees the reference lists at +0x40 and +0x2CBC (t3dFreeReferenceList slot
// 0x006E5268), the LocalAlloc'd block at +0x2CB8, the box draw owner at +0x78 (only when 0x006E5480 == 1 and the
// byte at 0x006EFC70 == 1), then calls t3dCleanUpLibrary (slot 0x006E5170).
void __fastcall Shutdown3DDisplay_guess(char* display)
{
    int boxOwner;
    t3dFreeReferenceList_guess_t freeReferenceList;

    // +0x2C98 flag: when set, ShutdownDisplay_guess runs on the global CDisplay (g_display_guess) first; it clears the flag.
    if (*(int*)(display + 0x2c98) != 0)
    {
        EQ_METHOD(0, void, 0x0040AFA1, (void*), g_display_guess)();
    }
    freeReferenceList = g_pfn_t3dFreeReferenceList_guess;
    freeReferenceList(display + 0x40);
    freeReferenceList(display + 0x2cbc);
    if (*(int*)(display + 0x2cb8) != 0)
    {
        freeReferenceList(*(void**)(display + 0x2cb8));
        g_pfn_LocalFree_guess(*(void**)(display + 0x2cb8));
        *(u32*)(display + 0x2cb8) = 0;
    }
    if ((EQ_GLOBAL(int, 0x006E5480) == 1) && (g_no_render_offset_guess == 1))
    {
        EQ_METHOD(0, void, 0x00402B1D, (void*), *(void**)(display + 0x78))();
        boxOwner = *(int*)(display + 0x78);
        if (boxOwner != 0)
        {
            EQ_METHOD(0, void, 0x00402927, (void*), (void*)boxOwner)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)boxOwner);
        }
    }
    g_pfn_t3dCleanUpLibrary_guess();
}

// FUNCTION: TRILOGY 0x004060a7
// Applies a spawn's texture variant to its model materials.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall, one stack argument (callee pops 4). display.h declares no CDisplay member for it, and VC6 rejects
// __thiscall outside a class (C4234), so this is a free __fastcall with an unused EDX dummy: ECX is the CDisplay,
// the stack argument and the callee pop are the same. Returns 0 when the spawn's actor has no skinned
// hierarchical sprite or its texture is outside 0..0x1e; otherwise swaps the matching materials to the texture
// number, applies the infravision ambient when the body type allows it, and returns 1.
int __fastcall SwapNPCMaterials_guess(CDisplay* display, int, EQPlayer* player)
{
    EQPlayer* spawn;
    void* hier_sprite;
    void* palette;
    char texture_text[4];
    char actor_tag[16];
    char material_name[64];
    char new_name[64];
    EQActorInfo_guess* actor_info;
    void* skin;
    void* entry;
    unsigned int skin_count;
    int entries;
    size_t name_len;
    int body_type;

    spawn = player;
    if (player != 0 && (actor_info = player->actor_info) != 0 && actor_info->actor != 0 &&
        (char)player->texture >= 0 && (char)player->texture <= 0x1e &&
        (hier_sprite = actor_info->hier_sprite) != 0 &&
        (unsigned int)s3dGetNumSkinsAttachedToHierarchicalSprite(hier_sprite) >= 1)
    {
        skin_count = (unsigned int)s3dGetNumSkinsAttachedToHierarchicalSprite(hier_sprite);
        if (skin_count > 1 && (char)player->helm >= 0)
            EQ_METHOD(6, void, 0x00407334, (void*, EQPlayer*, int, int, int, int, int), display)(player, (int)(char)player->helm, 0, 0, 0, 0);
        EQ_METHOD(1, void, 0x004B156C, (void*, char*), player)(actor_tag);
        sprintf(texture_text, "%02d", (int)(char)player->texture);
        skin = s3dGetSkinAttachedToHierarchicalSprite(0, hier_sprite);
        palette = s3dGetDMSpriteMaterialPalette(skin);
        player = (EQPlayer*)0;
        entries = s3dGetNumMaterialPaletteEntries(display->world_guess, palette);
        if (entries > 0)
        {
            do
            {
                entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(display->world_guess, palette,
                    (int)player);
                EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(entry, material_name);
                name_len = strlen(material_name);
                if (name_len > 8 && material_name[0] == actor_tag[0] && material_name[1] == actor_tag[1] &&
                    material_name[2] == actor_tag[2] && material_name[5] >= '0' && material_name[5] <= '9' &&
                    material_name[6] >= '0' && material_name[6] <= '9' && material_name[7] >= '0' &&
                    material_name[7] <= '9' && material_name[8] >= '0' && material_name[8] <= '9')
                {
                    strcpy(new_name, material_name);
                    new_name[5] = texture_text[0];
                    new_name[6] = texture_text[1];
                    display->ReplaceMaterial(material_name, new_name, hier_sprite, 0, 0);
                }
                player = (EQPlayer*)((int)player + 1);
                entries = s3dGetNumMaterialPaletteEntries(display->world_guess, palette);
            } while ((int)player < entries);
        }
        if (display->infravision_on_guess != 0 && (body_type = spawn->body_type) != 8 &&
            body_type != 3 && body_type != 5)
            EQ_METHOD(2, void, 0x004122EA, (void*, EQPlayer*, u8*), display)(spawn, display->infravision_ambient_guess);
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0040628b
// Adjusts the 3D view area for the active display mode.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The camera's render window is the screen rectangle moved by a fixed offset per video mode (1..4);
// another mode, or g_no_render_offset_guess set: nothing at all.
void CDisplay::OffsetRenderWindow()
{
    if (g_no_render_offset_guess != 0)
        return;
    if (g_video_mode_guess == 1)
    {
        render_window_guess[0] = g_render_rect_guess[0] + 0x50;
        render_window_guess[1] = g_render_rect_guess[1] + 0x3C;
        render_window_guess[2] = g_render_rect_guess[2] + 0x50;
        render_window_guess[3] = g_render_rect_guess[3] + 0x3C;
    }
    else if (g_video_mode_guess == 2)
    {
        render_window_guess[0] = g_render_rect_guess[0] + 0xC0;
        render_window_guess[1] = g_render_rect_guess[1] + 0x90;
        render_window_guess[2] = g_render_rect_guess[2] + 0xC0;
        render_window_guess[3] = g_render_rect_guess[3] + 0x90;
    }
    else if (g_video_mode_guess == 3)
    {
        render_window_guess[0] = g_render_rect_guess[0] + 0x140;
        render_window_guess[1] = g_render_rect_guess[1] + 0x110;
        render_window_guess[2] = g_render_rect_guess[2] + 0x140;
        render_window_guess[3] = g_render_rect_guess[3] + 0x110;
    }
    else if (g_video_mode_guess == 4)
    {
        render_window_guess[0] = g_render_rect_guess[0] + 0x1E0;
        render_window_guess[1] = g_render_rect_guess[1] + 0x168;
        render_window_guess[2] = g_render_rect_guess[2] + 0x1E0;
        render_window_guess[3] = g_render_rect_guess[3] + 0x168;
    }
    else
    {
        return;
    }
    t3dSetCameraRenderWindow(camera_guess, render_window_guess);
}

// FUNCTION: TRILOGY 0x00406399
// Sets the camera render window back to the unshifted render rectangle (undoes OffsetRenderWindow).
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::UpdateCameraAfterModeSwitch_guess()
{
    render_window_guess[0] = g_render_rect_guess[0];
    render_window_guess[1] = g_render_rect_guess[1];
    render_window_guess[2] = g_render_rect_guess[2];
    render_window_guess[3] = g_render_rect_guess[3];
    t3dSetCameraRenderWindow(camera_guess, render_window_guess);
}

// FUNCTION: TRILOGY 0x004063dc
// Renders a close view of the player model in a separate scene.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: mov ebx (camera function load) is emitted before lea (its arg) and the 7 xor edx dummy-EDX slots of EQ_THISCALL are absent in the original
// Renders the doll of the spawn at g_eqmain+0x84 (the local player when there is none) from a camera beside it,
// then puts the camera, view angle, game state and fog back.
void __fastcall CDisplay_renderDoll_guess(CDisplay* display)
{
    typedef void (__cdecl *t3dGetCameraLocation_guess_t)(void*, float*);
    typedef void (__cdecl *t3dRenderPartialScene_guess_t)(void*, void*, void*, int);
    float camera_location[7];
    float doll_location[7];
    EQPlayer* player;
    EQPlayer* spawn;
    t3dSetCameraLocation_t set_camera_location;
    int saved_game_state;
    float saved_view_angle;
    float radius;
    float fog_near;
    float fog_far;

    saved_game_state = g_game_state_guess;
    if (g_eqmain_guess == 0 || g_eqmain_guess[0x100c] == 0
        || (spawn = pinstEQ_Main_guess->inspectSpawn_guess) == 0)
    {
        player = g_local_player_guess;
    }
    else
    {
        player = spawn;
    }
    if (player != 0)
    {
        EQ_GLOBAL(t3dGetCameraLocation_guess_t, 0x006E5290)(display->camera_guess, camera_location);
        g_game_state_guess = 7;
        saved_view_angle = display->view_angle_guess;
        display->view_angle_guess = 86.0f;
        CDisplay_ChangeVideoMode_guess(display, 1);
        radius = player->height_offset;
        if (radius < 1.0f)
        {
            radius = 1.0f;
        }
        radius = radius + 1.0f;
        doll_location[0] = t3dFloatFastCosine(player->heading) * radius + player->y;
        doll_location[1] = t3dFloatFastSine(player->heading) * radius + player->x;
        doll_location[3] = EQ_METHOD(1, float, 0x00408156, (CDisplay *, float), display)(
            player->heading + 256.0f);
        set_camera_location = t3dSetCameraLocation;
        doll_location[5] = 0.0f;
        doll_location[2] = player->z + 1.0f;
        doll_location[4] = -16.0f;
        set_camera_location(display->world_guess, display->camera_guess, doll_location);
        t3dSetActorInvisible(player->actor_info->actor, 0);
        fog_near = display->fogStart_guess;
        fog_far = display->fogEnd_guess;
        EQ_METHOD(6, void, 0x0040F518, (CDisplay *, int, float, float, u8, u8, u8), display)(2, 1.0f, 70.0f,
            *(u8 *)((char *)display + 0xa8), *(u8 *)((char *)display + 0xa9), *(u8 *)((char *)display + 0xaa));
        if (g_video_mode_guess != 0)
        {
            EQ_METHOD(0, void, 0x0040628B, (CDisplay *), display)();
        }
        EQ_GLOBAL(t3dRenderPartialScene_guess_t, 0x006E5360)(display->world_guess, display->render_guess,
            display->camera_guess, 0);
        if (g_video_mode_guess != 0)
        {
            EQ_METHOD(0, void, 0x00406399, (CDisplay *), display)();
        }
        EQ_METHOD(6, void, 0x0040F518, (CDisplay *, int, float, float, u8, u8, u8), display)(2, fog_near,
            fog_far, *(u8 *)((char *)display + 0xa8), *(u8 *)((char *)display + 0xa9),
            *(u8 *)((char *)display + 0xaa));
        EQ_CDECL(unsigned int, 0x005227B1, (unsigned int, unsigned int))(0xa031f, 0xf031f);
        g_game_state_guess = saved_game_state;
        display->view_angle_guess = saved_view_angle;
        CDisplay_ChangeVideoMode_guess(display, 1);
        set_camera_location(display->world_guess, display->camera_guess, camera_location);
    }
}

// FUNCTION: TRILOGY 0x004065c3
// Returns 0 or 1 as a full int, not bool: the original keeps the result in a dword (xor eax,eax; neg/sbb/neg).
int __stdcall ChangeRaceVariantDag_guess(void* spawn, int textureId)
{
    u16 raceCode;
    int variantDag;
    char dagName[128];

    if (textureId >= 2000)
    {
        variantDag = *(int*)(*(int*)((char*)spawn + 0x70) + 0x278);
    }
    else
    {
        variantDag = *(int*)(*(int*)((char*)spawn + 0x70) + 0x274);
    }
    if (variantDag == 0)
    {
        return 0;
    }
    if (*(char*)((char*)spawn + 0x9c) == '\x01')
    {
        textureId = textureId + 0x1e;
    }
    raceCode = *(u16*)((char*)spawn + 0x9a);
    switch (raceCode)
    {
    case 7:
        textureId = textureId + 0x3c;
        break;
    case 8:
        textureId = textureId + 0x78;
        break;
    case 6:
        textureId = textureId + 0xb4;
        break;
    case 4:
        textureId = textureId + 0xf0;
        break;
    case 5:
        textureId = textureId + 300;
        break;
    case 1:
        textureId = textureId + 0x168;
        break;
    case 3:
        textureId = textureId + 0x1e0;
        break;
    case 2:
        textureId = textureId + 0x21c;
        break;
    case 9:
        textureId = textureId + 0x294;
        break;
    case 10:
        textureId = textureId + 0x2d0;
        break;
    case 0xb:
        textureId = textureId + 0x30c;
        break;
    case 0x80:
        textureId = textureId + 0x1a4;
        break;
    case 0x82:
        textureId = textureId + 600;
        break;
    }
    sprintf(dagName, (const char*)0x0053D7D4, textureId);
    variantDag = EQ_METHOD(2, int, 0x004119A6, (void*, void*, const char*), g_display_guess)((void*)variantDag, dagName);
    return variantDag != 0;
}

// FUNCTION: TRILOGY 0x004066e2
// Applies a face or eye material variant and reports whether the update succeeded.
// @early-stop: EQ_THISCALL still zeros EDX at two calls, and the zero return clears BL.
/* fake match: keep success in EBX and normalize mode 3 before the shared zero test. */
int __stdcall GetBaseFaceNbr_guess(EQPlayer* spawn, u8 variant, char mode)
{
    u8 success = 1;
    int tmp;

    if (mode == success)
    {
        tmp = EQ_METHOD(3, int, 0x00406EDC, (void*, void*, int, int), g_display_guess)(spawn, variant,
             spawn->face);
    }
    else
    {
        if (mode == '\x02')
        {
            tmp = variant + 1000;
        }
        else
        {
            if (mode == '\x03')
            {
                tmp = g_cdisplay_guess->SetEyeMaterial(spawn, success, variant);
                if (tmp != 0)
                {
                    g_cdisplay_guess->SetEyeMaterial(spawn, 0, variant);
                    tmp = success;
                }
            }
            else
            {
                tmp = variant + 2000;
            }
        }
        if (mode != '\x03')
            tmp = EQ_METHOD(2, int, 0x004065C3, (void*, void*, int), g_display_guess)(spawn, tmp);
    }
    if (tmp == 0)
        success = 0;
    return success;
}

// FUNCTION: TRILOGY 0x00406777
// Colors every material on a supported model sprite with one tint.
// @early-stop: VC6 keeps an EBP frame and spills count into the dead dag argument slot.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Sprite type 0x14 (hierarchical) uses its first skin, type 0x48 (DM sprite) itself; others fail.
// Every palette entry gets the tint and the palette's dword +0x20 is set (dirty flag?).
/* fake match: null the unsupported sprite tag to reuse the shared null check and failure block. */
int CDisplay::SetDagMaterialTint_guess(void* dag, u32* tint)
{
    int count;

    if (dag == 0 || tint == 0)
        return 0;
    int* sprite = t3dGetDagSprite(dag);
    if (sprite == 0)
        return 0;
    switch (*sprite)
    {
    case 0x14:
        sprite = (int*)s3dGetSkinAttachedToHierarchicalSprite(0, sprite);
        break;
    case 0x48:
        break;
    default:
        sprite = 0;
        break;
    }
    if (sprite == 0)
        return 0;
    int* palette = (int*)s3dGetDMSpriteMaterialPalette(sprite);
    if (palette == 0)
        return 0;
    count = s3dGetNumMaterialPaletteEntries(world_guess, palette);
    int i = 0;
    if (count > 0)
    {
        do
        {
            s3dSetMaterialPaletteEntryTint(world_guess, palette, i, tint);
            i = i + 1;
            palette[8] = 1;
        } while (i < count);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00406802
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` is only passed on. Slot 4 tints the second attachment dag, any other slot the first.
int CDisplay::SetPlayerAttachmentTint_guess(EQPlayer* player, u8 tintIndex, char slot)
{
    void* dag;

    if (player != 0 && tintIndex < 10)
    {
        if (slot == 4)
            dag = player->actor_info->attach_b_guess[1];
        else
            dag = player->actor_info->attach_b_guess[0];
        if (dag != 0)
            return SetDagMaterialTint_guess(dag, &g_attachment_tints_guess[tintIndex]);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00406847
// Applies a player's model appearance variants, attachment tints, and eye materials.
void CDisplay::ApplyPlayerAppearance_guess(EQPlayer* player)
{
    int appearanceObject;

    if ((((player != 0) && (*(int*)((char*)player + 0x70) != 0)) &&
        (appearanceObject = *(int*)(*(int*)((char*)player + 0x70) + 0x178), appearanceObject != 0)) &&
        (*(char*)(appearanceObject + 0x39c) != '\0'))
    {
        EQ_METHOD(3, int, 0x004066E2, (CDisplay*, EQPlayer*, u8, char), this)(player, *(&player->appearance[4]), 2);
        EQ_METHOD(3, int, 0x004066E2, (CDisplay*, EQPlayer*, u8, char), this)(player, *(&player->appearance[5]), 4);
        EQ_METHOD(3, int, 0x00406802, (CDisplay*, EQPlayer*, u8, char), this)(player, *(&player->appearance[0]), 2);
        EQ_METHOD(3, int, 0x00406802, (CDisplay*, EQPlayer*, u8, char), this)(player, *(&player->appearance[1]), 4);
        EQ_METHOD(3, int, 0x004068DC, (CDisplay*, EQPlayer*, char, int), this)(player, 1, *(&player->appearance[3]));
        EQ_METHOD(3, int, 0x004068DC, (CDisplay*, EQPlayer*, char, int), this)(player, 0, *(&player->appearance[2]));
    }
    return;
}

// FUNCTION: TRILOGY 0x004068dc
// Selects a material for the left or right eye on a character model.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::SetEyeMaterial(EQPlayer* player, char rightEye, int materialIndex)
{
    EQActorInfo_guess* info;
    void* hierSprite;
    void* skin;
    void* palette;
    int eyeIndex;
    int count;
    int race;
    int index;
    int materialDef;
    char name[64];

    if (player == 0)
        return 0;
    info = player->actor_info;
    if (info == 0 || info->anim_set_void_guess == 0)
        return 0;
    if (*(char*)((char*)info->anim_set_void_guess + 0x39c) == 0
        || info->right_eye_material_guess < 0 || materialIndex == 0xff)
        return 0;

    hierSprite = info->hier_sprite;
    if (hierSprite == 0)
        return 0;
    skin = s3dGetSkinAttachedToHierarchicalSprite(0, hierSprite);
    if (skin == 0)
        return 0;
    palette = s3dGetDMSpriteMaterialPalette(skin);
    if (palette == 0)
        return 0;

    if (rightEye != 0)
        eyeIndex = player->actor_info->right_eye_material_guess;
    else
        eyeIndex = player->actor_info->left_eye_material_guess;
    count = s3dGetNumMaterialPaletteEntries(this->world_guess, palette);
    if (eyeIndex < 0 || eyeIndex >= count)
        return 0;

    race = player->race;
    index = materialIndex;
    switch (race)
    {
    case 6:
        index = materialIndex + 0x14;
        break;
    case 9:
        index = materialIndex + 100;
        break;
    case 10:
        index = materialIndex + 0x50;
        break;
    case 0x80:
        index = materialIndex + 0x28;
        break;
    case 0x82:
        index = materialIndex + 0x3c;
        break;
    }
    sprintf(name, "CHR_EYE%03d_MDF", index);
    materialDef = (int)t3dGetPointerFromDictionary(this->world_guess, name);
    if (materialDef == 0)
        return 0;
    s3dSetMaterialPaletteEntry_guess_t setEntry =
        EQ_GLOBAL(s3dSetMaterialPaletteEntry_guess_t, 0x006E513C);
    setEntry(this->world_guess, palette, eyeIndex, (void*)materialDef);
    return 1;
}

// FUNCTION: TRILOGY 0x00406a12
// Finds the left and right eye material slots in a character model.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: this and the palette swap callee-saved registers (VC6 gives this=EBX, palette=EDI; the original has palette=EBX, this=EDI)
void CDisplay::FindDefaultEyeMaterialIndexes(EQPlayer* player)
{
    void* info;
    void* animSet;
    void* hierSprite;
    void* skin;
    void* palette;
    void* entry;
    char tag[64];
    int count;
    int index;
    int tagResult;

    index = 0;
    if (player == 0)
        return;
    info = *(void**)((char*)player + 0x70);
    if (info == 0)
        return;
    animSet = *(void**)((char*)info + 0x178);
    if (animSet == 0 || *(char*)((char*)animSet + 0x39c) == 0)
        return;
    hierSprite = *(void**)((char*)info + 0x250);
    if (hierSprite == 0)
        return;
    skin = s3dGetSkinAttachedToHierarchicalSprite(0, hierSprite);
    if (skin == 0)
        return;
    palette = s3dGetDMSpriteMaterialPalette(skin);
    if (palette == 0)
        return;
    count = s3dGetNumMaterialPaletteEntries(this->world_guess, palette);

    for (index = 0; index < count; ++index)
    {
        entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(
            this->world_guess, palette, index);
        if (entry != 0)
        {
            tagResult = EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(entry, tag);
            if (tagResult == 0)
            {
                if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                        tag + 3, "L_EYE", 5) == 0)
                {
                    info = *(void**)((char*)player + 0x70);
                    *(int*)((char*)info + 0x298) = index;
                }
                else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                             tag + 3, "R_EYE", 5) == 0)
                {
                    info = *(void**)((char*)player + 0x70);
                    *(int*)((char*)info + 0x29c) = index;
                }
            }
        }
    }
}

// FUNCTION: TRILOGY 0x00406b0d
// Replaces one matching model material and applies the selected tint.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: loop bottom is rotated (jge + copied push edi), count check compares with edi not test eax
int CDisplay::ReplaceMaterial(char* materialTag, char* dictionaryName, void* hierSprite,
                                    u8* tint, int partialCompare)
{
    void* skin;
    void* palette;
    void* entry;
    char tag[64];
    int index;
    int result;
    u8* tintToUse;

    if (hierSprite == 0)
        return 0;
    skin = s3dGetSkinAttachedToHierarchicalSprite(0, hierSprite);
    if (skin == 0)
        return 0;
    palette = s3dGetDMSpriteMaterialPalette(skin);
    if (palette == 0)
        return 0;
    /* dictionaryName and hierSprite are reused for the material def and the entry count */
    dictionaryName = (char*)t3dGetPointerFromDictionary(world_guess, dictionaryName);
    if (dictionaryName == 0)
        return 0;
    hierSprite = (void*)s3dGetNumMaterialPaletteEntries(world_guess, palette);
    if ((int)hierSprite <= 0)
        return 0;

    index = 0;
    do
    {
        entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(
            world_guess, palette, index);
        if (entry == 0)
            return 0;
        result = EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(entry, tag);
        if (result != 0)
            return 0;

        if (partialCompare)
        {
            if (strncmp(materialTag, tag, 5) == 0
                && materialTag[7] == tag[7] && materialTag[8] == tag[8])
                break;
        }
        else
        {
            if (strcmp(materialTag, tag) == 0)
                break;
        }
        index++;
    } while (index < (int)hierSprite);

    if (index >= (int)hierSprite)
        return 0;
    EQ_GLOBAL(s3dSetMaterialPaletteEntry_guess_t, 0x006E513C)(
        world_guess, palette, index, dictionaryName);

    /* the default white tint lives in the dead materialTag slot: the frame holds only tag */
    if (tint == 0 || (tint[2] == 0 && tint[1] == 0 && tint[0] == 0))
    {
        ((u8*)&materialTag)[2] = 0xff;
        ((u8*)&materialTag)[0] = 0xff;
        ((u8*)&materialTag)[1] = 0xff;
        tintToUse = (u8*)&materialTag;
    }
    else
    {
        tintToUse = tint;
    }
    s3dSetMaterialPaletteEntryTint(
        world_guess, palette, index, (u32*)tintToUse);
    return 1;
}

struct CDisplayCloak_guess
{
    void* render_guess;
    void* world_guess;

    int ReplaceCloakMaterials(int cloakId, void* hierSprite, u8* tint);
};

// FUNCTION: TRILOGY 0x00406c37
// Applies the selected cloak design and tint across the character model's cloak materials.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: the original defers each arm's sprintf pop (12 bytes) to the join and merges it with strcpy's (add esp,0x14); push 0 / mov ecx order
int CDisplayCloak_guess::ReplaceCloakMaterials(int cloakId, void* hierSprite, u8* tint)
{
    void* skin;
    void* palette;
    void* entry;
    char tag[64];
    char dictionaryName[64];
    char* suffix;
    size_t length;
    int count;
    int index;
    int result;

    if (hierSprite == 0)
        return 0;
    skin = s3dGetSkinAttachedToHierarchicalSprite(0, hierSprite);
    if (skin == 0)
        return 0;
    palette = s3dGetDMSpriteMaterialPalette(skin);
    if (palette == 0)
        return 0;

    count = s3dGetNumMaterialPaletteEntries(world_guess, palette);
    index = 0;
    if (0 < count)
    {
        do
        {
            entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(
                world_guess, palette, index);
            result = EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(entry, tag);
            if (result == 0)
            {
                length = strlen(tag);
                if (length >= 10 && tag[0] == 'C' && tag[1] == 'L' && tag[2] == 'K')
                {
                    if (tag[4] < '0' || '9' < tag[4])
                    {
                        strcpy(
                            dictionaryName, (const char*)0x0053D7FC);
                        sprintf(
                            dictionaryName + 3, (const char*)0x0053D7CC, cloakId);
                        suffix = &tag[6];
                    }
                    else
                    {
                        strcpy(
                            dictionaryName, (const char*)0x0053D7FC);
                        sprintf(
                            dictionaryName + 3, (const char*)0x0053D7CC, cloakId);
                        suffix = &tag[5];
                    }
                    strcpy(dictionaryName + 5, suffix);
                    EQ_METHOD(5, int, 0x00406B0D, (void*, char*, char*, void*, u8*, int), this)(tag, dictionaryName, hierSprite, tint, 0);
                }
            }
            index = index + 1;
        } while (index < count);
    }
    return 1;
}

struct CDisplaySpecialCloak_guess : CDisplay
{
    int ReplaceSpecialCloakMaterials(EQPlayer* player);
};

// FUNCTION: TRILOGY 0x00406d7d
// Replaces matching CLK palette entries with actor-specific material variants.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplaySpecialCloak_guess::ReplaceSpecialCloakMaterials(EQPlayer* player)
{
    EQActorInfo_guess* actorInfo;
    void* hierSprite;
    void* skin;
    void* palette;
    void* entry;
    u32 numSkins;
    int index;
    int result;
    size_t length;
    char dictionaryName[64];
    char actorTag[16];
    char tag[64];
    int count;
    u8 tint[3];

    tint[2] = 0xFF;
    tint[1] = 0xFF;
    tint[0] = 0xFF;
    if (player != 0 && (actorInfo = player->actor_info) != 0
        && actorInfo->actor != 0
        && (hierSprite = actorInfo->hier_sprite) != 0
        && (numSkins = s3dGetNumSkinsAttachedToHierarchicalSprite(hierSprite)) >= 1
        && (skin = s3dGetSkinAttachedToHierarchicalSprite(0, hierSprite)) != 0
        && (palette = s3dGetDMSpriteMaterialPalette(skin)) != 0)
    {
        EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), player)(actorTag);
        count = s3dGetNumMaterialPaletteEntries(world_guess, palette);
        index = 0;
        if (0 < count)
        {
            do
            {
                entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(
                    world_guess, palette, index);
                result = EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(entry, tag);
                if (result == 0)
                {
                    length = strlen(tag);
                    if (length >= 10 && tag[0] == 'C' && tag[1] == 'L' && tag[2] == 'K'
                        && tag[5] == '0' && tag[6] >= '6' && tag[6] <= '9')
                    {
                        strcpy(
                            dictionaryName, (const char*)0x0053D7FC);
                        strcat(
                            dictionaryName, actorTag);
                        strcpy(
                            dictionaryName + 6, &tag[5]);
                        EQ_METHOD(5, int, 0x00406B0D, (void*, char*, char*, void*, u8*, int), this)(tag, dictionaryName, hierSprite, tint, 0);
                    }
                }
                index = index + 1;
            } while (index < count);
        }
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00406edc
// Changes the character's face materials and stores the selected face number.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::SwapFace(EQPlayer* character, int newFace, int currentFace)
{
    int* actorInfo;
    int tmp;
    char suffix[16];
    char dictionaryName[0x40];
    char currentTag[0x40];
    char modelTag[16];
    int hierarchicalSprite;
    CDisplay* thisDisplay;

    thisDisplay = this;
    if (character != 0 &&
        (actorInfo = (int*)character->actor_info, actorInfo != 0) &&
        *actorInfo != 0 &&
        (hierarchicalSprite = actorInfo[0x94], hierarchicalSprite != 0) &&
        (tmp = s3dGetNumSkinsAttachedToHierarchicalSprite
            ((void*)hierarchicalSprite), (u32)tmp >= 1))
    {
        EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), character)(modelTag);
        if (currentFace >= 0 && currentFace <= 9 && newFace >= 0 && newFace <= 9)
        {
            if (character->type == 1)
            {
                if (currentFace == 0)
                    tmp = (int)(char)character->helm;
                else
                    tmp = 0;
                sprintf(currentTag, (const char*)0x0053D810, modelTag, tmp, currentFace);
                if (newFace == 0)
                    tmp = (int)(char)character->helm;
                else
                    tmp = 0;
                sprintf(dictionaryName, (const char*)0x0053D810, modelTag, tmp, newFace);
                thisDisplay->ReplaceMaterial(currentTag, dictionaryName, (void*)hierarchicalSprite, 0, currentFace == 0);
                currentTag[8] = currentTag[8] + 1;
                dictionaryName[8] = dictionaryName[8] + 1;
                thisDisplay->ReplaceMaterial(currentTag, dictionaryName, (void*)hierarchicalSprite, 0, currentFace == 0);
            }
            else
            {
                strcpy(suffix, (const char*)0x0053D808);
                strcpy(currentTag, modelTag);
                strcat(currentTag, suffix);
                strcpy(dictionaryName, currentTag);
                sprintf(&currentTag[7], (const char*)0x0053D800, currentFace);
                sprintf(&dictionaryName[7], (const char*)0x0053D800, newFace);
                thisDisplay->ReplaceMaterial(currentTag, dictionaryName, (void*)hierarchicalSprite, 0, 0);
                currentTag[8] = currentTag[8] + 1;
                dictionaryName[8] = dictionaryName[8] + 1;
                thisDisplay->ReplaceMaterial(currentTag, dictionaryName, (void*)hierarchicalSprite, 0, 0);
            }
            character->face = (u8)newFace;
            if ((((thisDisplay->infravision_on_guess != 0) &&
                  (tmp = character->body_type, tmp != 8)) &&
                 (tmp != 3)) && (tmp != 5))
            {
                EQ_METHOD(2, void, 0x004122EA, (void*, EQPlayer*, u8*), thisDisplay)(character, &thisDisplay->infravision_ambient_guess[0]);
            }
            return 1;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004070ed
// Maps material IDs 0xF0 and 0xF1 for supported player races.
// @early-stop: F1 still loads its compare constant into EAX and keeps the race value in ECX.
/* fake match: spell F1's cutoffs as 2 and 9 to retain the original compare constants. */
int __stdcall CDisplay_SwapHead2_guess(int playerObject, int materialId, u32* flagOut)
{
    CharacterProfile* pc;
    int result = materialId;

    *flagOut = 1;
    pc = EQ_GLOBAL(CharacterProfile*, 0x006E4DA8);
    if (pc == 0 || playerObject == 0
        || EQ_CDECL(int, 0x004A31A5, (CharacterProfile*))(pc) == 0)
        return 3;

    if (materialId == 0xf0)
    {
        int race;
        race = *(u16*)(playerObject + 0x9a);
        if (race <= 0x80)
        {
            if (race != 0x80)
            {
                switch (race)
                {
                case 1:
                    if (*(char*)(playerObject + 0x9c) == '\0')
                        result = 0x273;
                    else
                    {
                        result = 0x26c;
                        *flagOut = 0;
                    }
                    break;
                case 2:
                    if (*(char*)(playerObject + 0x9c) == '\0')
                        result = 0x219;
                    else
                    {
                        result = 0x212;
                        *flagOut = 0;
                    }
                    break;
                case 3:
                    *flagOut = 0;
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x23f;
                    break;
                case 4:
                    if (*(char*)(playerObject + 0x9c) == '\0')
                        result = 0x235;
                    else
                    {
                        result = 0x231;
                        *flagOut = 0;
                    }
                    break;
                case 5:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x25d;
                    break;
                case 6:
                    if (*(char*)(playerObject + 0x9c) == '\0')
                        result = 0x221;
                    else
                    {
                        result = 0x21c;
                        *flagOut = 0;
                    }
                    break;
                case 7:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x253;
                    break;
                case 8:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffff9) + 0x22d;
                    break;
                case 9:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x28f;
                    break;
                case 10:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x285;
                    break;
                case 0xb:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x267;
                    break;
                case 0xc:
                    result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x249;
                    break;
                }
            }
            else
                result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x27b;
        }
    }
    else if (materialId == 0xf1)
    {
        int race;
        race = *(u16*)(playerObject + 0x9a);
        if (race <= 10)
        {
            if (race != 10)
            {
                if (race == 1)
                    result = 3;
                else if (race != 2)
                {
                    if (race <= 2)
                        return result;
                    if (race > 9)
                        return result;
                    result = 3;
                }
                else
                {
                    if (*(char*)(playerObject + 0x9c) == '\0')
                        result = 0x219;
                    else
                    {
                        result = 0x212;
                        *flagOut = 0;
                    }
                }
            }
            else
                result = (((*(char*)(playerObject + 0x9c) != '\0') ? -1 : 0) & 0xfffffffb) + 0x286;
        }
        else if (race < 0xb)
            return result;
        else if (0xc < race && race != 0x80)
            return result;
        else
            result = 3;
    }
    return result;
}

// FUNCTION: TRILOGY 0x00407334
// Puts a new helm on a spawn's model: swaps the head mesh to the new material and tints its helm textures.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall (this in ECX; `edx` is the dummy), six stack arguments, callee pops 0x18. Swaps the head skin of
// a spawn: the skin whose DM sprite is named for the old head is detached and a DM sprite for the new head is
// attached at the same dag. Returns 1 when swapped, 0 when a lookup fails.
// @early-stop: stack slot order (scalars and arrays: actor_tag/obj_tag, old_name/new_name swapped), shared "push 1; pop eax" tail lands in the second arm, mov ecx,esi before 2nd 4070ed call.
int __fastcall CDisplay_SwapHead_guess(CDisplay* self, int edx, EQPlayer* player, int new_material,
                                       int old_material, int unused_5, int colour, int item_flags)
{
    typedef void* (__cdecl *s3dGetDMSpriteDefinition_guess_t)(void*);
    typedef void* (__cdecl *s3dCreateDMSprite_guess_t)(void*, void*, int);
    typedef void (__cdecl *s3dSetDMSpriteMaterialPalette_guess_t)(void*, void*);
    typedef void (__cdecl *s3dDetachSkinFromHierarchicalSprite_guess_t)(void*, int, void*);
    typedef void (__cdecl *s3dAttachSkinToHierarchicalSprite_guess_t)(void*, void*, int);
    typedef int (__cdecl *s3dGetSkinLinkUpdatesToDAGIndex_guess_t)(int, void*);

    int count;                      // local_c
    int equip_slot = new_material;  // local_10
    u32 flag_new = 1;               // local_14: SwapHead2's flag for new_material
    int material_count;             // local_18
    char item_name[4];              // local_1c: the original's slot is 4 bytes; "IT%d" overruns into the dead material_count
    u32 flag_old = 1;               // local_20: SwapHead2's flag for old_material
    int body_type;
    int hit;
    void* hier;                     // iVar5
    void* skin;                     // uVar2
    void* def;                      // uVar3, then the dictionary entry
    void* dm;                       // uVar4
    void* entry;
    char actor_tag[16];             // local_30
    char obj_tag[64];               // local_70: obj_tag + 2 is the "FHE00" test, obj_tag[8] the '2'
    char skin_tag[64];              // local_b0
    char old_name[64];              // local_f0
    char new_name[64];              // local_130
    u8 tint[4];                    // local_8: colour bytes 0..2, low byte first

    if (player == 0)
        return 0;
    if (player->actor_info == 0)
        return 0;
    if (player->actor_info->actor == 0)
        return 0;

    if (old_material >= 0xf0 && old_material < 0x100)
        old_material = EQ_STDCALL(int, 0x004070ED, (EQPlayer*, int, u32*))(player, old_material, &flag_old);
    if (new_material >= 0xf0 && new_material < 0x100)
        new_material = EQ_STDCALL(int, 0x004070ED, (EQPlayer*, int, u32*))(player, new_material, &flag_new);
    if (new_material == 7)
    {
        new_material = 2;
        if (player->race == 0x80)
            new_material = 3;
        equip_slot = new_material;
    }
    if (old_material == 7)
    {
        old_material = 2;
        if (player->race == 0x80)
            old_material = 3;
    }

    hier = player->actor_info->hier_sprite;
    if (hier == 0)
        return 0;
    if ((u32)s3dGetNumSkinsAttachedToHierarchicalSprite(hier) < 1)
        return 0;
    EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), player)(actor_tag);
    sprintf(old_name, EQ_ARRAY(char, 0x0053D864), actor_tag,
         (old_material >= 0 && old_material < 0xf0) ? old_material : 0);

    if (new_material >= 0 && new_material < 0xf0)
    {
        EQ_METHOD(4, void, 0x0040B6F6, (CDisplay*, EQPlayer*, int, char*, int), g_cdisplay_guess)(player, 0, EQ_ARRAY(char, 0x0053D860), item_flags);
        sprintf(new_name, EQ_ARRAY(char, 0x0053D864), actor_tag, new_material);
        count = s3dGetNumSkinsAttachedToHierarchicalSprite(hier);
        for (old_material = 0; old_material < count; old_material++)
        {
            skin = s3dGetSkinAttachedToHierarchicalSprite(old_material, hier);
            def = EQ_GLOBAL(s3dGetDMSpriteDefinition_guess_t, 0x006E50FC)(skin);
            if (EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(def, skin_tag) != 0)
                return 0;
            if (strcmp(skin_tag, old_name) == 0)
            {
                new_material = EQ_GLOBAL(s3dGetSkinLinkUpdatesToDAGIndex_guess_t, 0x006E5118)(old_material, hier);
                item_flags = (int)s3dGetDMSpriteMaterialPalette(skin);
                def = t3dGetPointerFromDictionary(self->world_guess, new_name);
                if (def == 0)
                    return 0;
                dm = EQ_GLOBAL(s3dCreateDMSprite_guess_t, 0x006E50CC)(self->world_guess, def, 0);
                EQ_GLOBAL(s3dSetDMSpriteMaterialPalette_guess_t, 0x006E5134)(dm, (void*)item_flags);
                EQ_GLOBAL(s3dDetachSkinFromHierarchicalSprite_guess_t, 0x006E50E4)(self->world_guess, old_material, hier);
                EQ_GLOBAL(s3dAttachSkinToHierarchicalSprite_guess_t, 0x006E50BC)(hier, dm, new_material);

                tint[0] = (u8)colour;
                tint[2] = (u8)(colour >> 16);
                tint[1] = (u8)(colour >> 8);
                if (colour == 0)
                {
                    tint[2] = 0xff;
                    tint[1] = 0xff;
                    tint[0] = 0xff;
                }
                material_count = s3dGetNumMaterialPaletteEntries(self->world_guess, (void*)item_flags);
                for (new_material = 0; new_material < material_count; new_material++)
                {
                    entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(self->world_guess, (void*)item_flags, new_material);
                    EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(entry, obj_tag);
                    if (flag_old == 0)
                    {
                        if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag + 2, EQ_ARRAY(char, 0x0053D858), 5) == 0
                            && obj_tag[8] == '2')
                            *(u32*)((char*)entry + 0x14) = 0x80000002;
                        if (player->race == 3
                            && strncmp(obj_tag, EQ_ARRAY(char, 0x0053D850), 5) == 0)
                            *(u32*)((char*)entry + 0x14) = 0x80000002;
                    }
                    hit = 0;
                    if (player->race == 2)
                    {
                        if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D844), 9) == 0
                            || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D838), 9) == 0
                            || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D830), 7) == 0)
                            hit = 1;
                    }
                    else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D828), 4) == 0
                             || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D820), 5) == 0)
                        hit = 1;
                    if (hit)
                        s3dSetMaterialPaletteEntryTint(self->world_guess, (void*)item_flags, new_material, (u32*)tint);
                }
                break;
            }
        }
        if (old_material == count)
            return 0;
        player->equipment[0] = (u8)equip_slot;
        player->equip_colors[0] = colour;
        if (self->infravision_on_guess != 0)
        {
            body_type = player->body_type;
            if (body_type != 8 && body_type != 3 && body_type != 5)
                EQ_METHOD(2, void, 0x004122EA, (CDisplay*, EQPlayer*, u8*), self)(player, self->infravision_ambient_guess);
        }
        return 1;
    }
    if (new_material >= 0xf0)
    {
        sprintf(item_name, EQ_ARRAY(char, 0x0053D7D4), new_material);
        EQ_METHOD(4, void, 0x0040B6F6, (CDisplay*, EQPlayer*, int, char*, int), g_cdisplay_guess)(player, 0, item_name, item_flags);
    }
    else
    {
        EQ_METHOD(4, void, 0x0040B6F6, (CDisplay*, EQPlayer*, int, char*, int), g_cdisplay_guess)(player, 0, EQ_ARRAY(char, 0x0053D860), item_flags);
    }
    sprintf(new_name, EQ_ARRAY(char, 0x0053D864), actor_tag, 0);
    count = s3dGetNumSkinsAttachedToHierarchicalSprite(hier);
    for (old_material = 0; old_material < count; old_material++)
    {
        skin = s3dGetSkinAttachedToHierarchicalSprite(old_material, hier);
        def = EQ_GLOBAL(s3dGetDMSpriteDefinition_guess_t, 0x006E50FC)(skin);
        if (EQ_GLOBAL(t3dGetObjectTagResult_guess_t, 0x006E52DC)(def, skin_tag) != 0)
            return 0;
        if (strcmp(skin_tag, old_name) == 0)
        {
            new_material = EQ_GLOBAL(s3dGetSkinLinkUpdatesToDAGIndex_guess_t, 0x006E5118)(old_material, hier);
            item_flags = (int)s3dGetDMSpriteMaterialPalette(skin);
            def = t3dGetPointerFromDictionary(self->world_guess, new_name);
            if (def == 0)
                return 0;
            dm = EQ_GLOBAL(s3dCreateDMSprite_guess_t, 0x006E50CC)(self->world_guess, def, 0);
            EQ_GLOBAL(s3dSetDMSpriteMaterialPalette_guess_t, 0x006E5134)(dm, (void*)item_flags);
            EQ_GLOBAL(s3dDetachSkinFromHierarchicalSprite_guess_t, 0x006E50E4)(self->world_guess, old_material, hier);
            EQ_GLOBAL(s3dAttachSkinToHierarchicalSprite_guess_t, 0x006E50BC)(hier, dm, new_material);

            tint[0] = (u8)colour;
            tint[2] = (u8)(colour >> 16);
            tint[1] = (u8)(colour >> 8);
            if (colour == 0)
            {
                tint[2] = 0xff;
                tint[1] = 0xff;
                tint[0] = 0xff;
            }
            material_count = s3dGetNumMaterialPaletteEntries(self->world_guess, (void*)item_flags);
            for (new_material = 0; new_material < material_count; new_material++)
            {
                entry = EQ_GLOBAL(s3dGetMaterialPaletteEntry_guess_t, 0x006E5104)(self->world_guess, (void*)item_flags, new_material);
                EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(entry, obj_tag);
                if (flag_new == 0)
                {
                    if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag + 2, EQ_ARRAY(char, 0x0053D858), 5) == 0
                        && obj_tag[8] == '2')
                        *(u32*)((char*)entry + 0x14) = 0;
                    if (player->race == 3
                        && strncmp(obj_tag, EQ_ARRAY(char, 0x0053D850), 5) == 0)
                        *(u32*)((char*)entry + 0x14) = 0;
                }
                hit = 0;
                if (player->race == 2)
                {
                    if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D844), 9) == 0
                        || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D838), 9) == 0
                        || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D830), 7) == 0)
                        hit = 1;
                }
                else if (EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D828), 4) == 0
                         || EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(obj_tag, EQ_ARRAY(char, 0x0053D820), 5) == 0)
                    hit = 1;
                if (hit)
                    s3dSetMaterialPaletteEntryTint(self->world_guess, (void*)item_flags, new_material, (u32*)tint);
            }
            break;
        }
    }
    if (old_material != count)
    {
        player->equipment[0] = (u8)equip_slot;
        player->equip_colors[0] = colour;
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0040798E
// Re-textures one armour slot (1..6) of a player-model spawn to a new material and colour; NPC models refused.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall (this in ECX), five stack arguments, callee pops 0x14. Puts new_material on
// equipment slot equip_slot (1..6) with colour and swaps the slot's material textures in the spawn's skin.
// Materials 0x11..0x17 become 1..3 while their flag global (0x006E2078..0x006E2090) is 0; 7 becomes 2 (3 for
// race 0x80); materials above the cap (0x10, or 0x17 with the expansion flag) are clamped. Returns 1 when
// swapped, 0 when there is no skin, the slot is not 1..6, or the texture is not 0xFF (NPC: SwapNPCMaterials).
// @early-stop: the third tint byte is shifted in EAX (the original copies colour to ECX again); the name-byte increments and ReplaceMaterial's argument pushes interleave differently
/* fake match: the second 0x17 store (else-if arm) keeps the constant in ESI across the ExpansionFlag2 call, as the original does. */
int CDisplay::SwapMaterial(EQPlayer* player, int equip_slot, int new_material, int old_material, int colour)
{
    char actor_tag[16];             // local_b4
    char new_name[64];              // local_a4
    char old_name[64];              // local_64
    char part_code[16];             // local_24
    int max_material = 0x10;        // local_10
    u32 class_;                     // local_14
    u8 tint[4];                     // local_c: colour bytes 0..2, low byte first
    void* hier;
    u32 count;

    if (g_pc_guess != 0 && EQ_METHOD(0, u32, 0x004197B2, (CharacterProfile*), g_pc_guess)() != 0)
        max_material = 0x17;
    else if (g_pcloudFlagA_guess == 1)
        max_material = 0x17;
    if (player == 0)
        return 0;
    if (player->actor_info == 0)
        return 0;
    if (player->actor_info->actor == 0)
        return 0;

    class_ = player->class_;
    if (new_material == 7)
    {
        new_material = 2;
        if (player->race == 0x80)
            new_material = 3;
    }
    if (old_material == 7)
    {
        old_material = 2;
        if (player->race == 0x80)
            old_material = 3;
    }

    hier = player->actor_info->hier_sprite;
    if (hier == 0)
        return 0;
    count = s3dGetNumSkinsAttachedToHierarchicalSprite(hier);
    if (count < 1)
        return 0;
    if (player->texture != 0xff)
    {
        EQ_CDECL(int, 0x005229E8, (const char*, ...))(EQ_ARRAY(char, 0x0053D8A0));
        EQ_METHOD(1, void, 0x004060A7, (CDisplay*, EQPlayer*), this)(player);
        return 0;
    }

    tint[0] = (u8)colour;
    tint[1] = (u8)(colour >> 8);
    tint[2] = (u8)(colour >> 16);
    EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), player)(actor_tag);
    switch (equip_slot)
    {
    case 0:
        return 0;
    case 1:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D888));
        break;
    case 2:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D88C));
        break;
    case 3:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D890));
        break;
    case 4:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D894));
        break;
    case 5:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D898));
        break;
    case 6:
        strcpy(part_code, EQ_ARRAY(char, 0x0053D89C));
        break;
    default:
        return 0;
    }

    switch (new_material)
    {
    case 0x13:
        if (EQ_GLOBAL(int, 0x006E2080) == 0)
            new_material = 3;
        break;
    case 0x12:
        if (EQ_GLOBAL(int, 0x006E207C) == 0)
            new_material = 2;
        break;
    case 0x11:
        if (EQ_GLOBAL(int, 0x006E2078) == 0)
            new_material = 1;
        break;
    case 0x14:
        if (EQ_GLOBAL(int, 0x006E2084) == 0)
            new_material = 1;
        break;
    case 0x15:
        if (EQ_GLOBAL(int, 0x006E2088) == 0)
            new_material = 2;
        break;
    case 0x16:
        if (EQ_GLOBAL(int, 0x006E208C) == 0)
            new_material = 3;
        break;
    case 0x17:
        if (EQ_GLOBAL(int, 0x006E2090) == 0)
            new_material = (((class_ != 7) - 1) & 3) + 1;
        break;
    }
    switch (old_material)
    {
    case 0x11:
        if (EQ_GLOBAL(int, 0x006E2078) == 0)
            old_material = 1;
        break;
    case 0x12:
        if (EQ_GLOBAL(int, 0x006E207C) == 0)
            old_material = 2;
        break;
    case 0x13:
        if (EQ_GLOBAL(int, 0x006E2080) == 0)
            old_material = 3;
        break;
    case 0x14:
        if (EQ_GLOBAL(int, 0x006E2084) == 0)
            old_material = 1;
        break;
    case 0x15:
        if (EQ_GLOBAL(int, 0x006E2088) == 0)
            old_material = 2;
        break;
    case 0x16:
        if (EQ_GLOBAL(int, 0x006E208C) == 0)
            old_material = 3;
        break;
    case 0x17:
        if (EQ_GLOBAL(int, 0x006E2090) == 0)
            old_material = (((class_ != 7) - 1) & 3) + 1;
        break;
    }

    if (new_material > max_material)
    {
        switch (new_material)
        {
        case 0x11:
            new_material = 1;
            break;
        case 0x12:
            new_material = 2;
            break;
        case 0x13:
            new_material = 3;
            break;
        case 0x14:
            new_material = 1;
            break;
        case 0x15:
            new_material = 2;
            break;
        case 0x16:
            new_material = 3;
            break;
        case 0x17:
            new_material = (((class_ != 7) - 1) & 3) + 1;
            break;
        default:
            new_material = 0;
            break;
        }
    }
    else if (new_material == 0x17 && class_ != 7)
        new_material = 1;
    if (old_material > max_material)
    {
        switch (old_material)
        {
        case 0x11:
            old_material = 1;
            break;
        case 0x12:
            old_material = 2;
            break;
        case 0x13:
            old_material = 3;
            break;
        case 0x14:
            old_material = 1;
            break;
        case 0x15:
            old_material = 2;
            break;
        case 0x16:
            old_material = 3;
            break;
        default:
            old_material = 0;
            break;
        }
    }

    strcpy(old_name, actor_tag);
    strcat(old_name, part_code);
    strcpy(new_name, old_name);
    sprintf(&old_name[5], EQ_ARRAY(char, 0x0053D87C), old_material);
    sprintf(&new_name[5], EQ_ARRAY(char, 0x0053D87C), new_material);

    switch (equip_slot)
    {
    case 1:
        if (0xa <= new_material && new_material <= 0x10)
        {
            if (old_material < 0xa || 0x10 < old_material)
            {
                new_name[5] = '0';
                new_name[6] = '0';
                this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
                old_name[8]++;
                new_name[8]++;
                this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
                EQ_METHOD(3, int, 0x00407E46, (CDisplay*, EQPlayer*, int, int), this)(player, 0, 1);
            }
            EQ_METHOD(3, int, 0x00406C37, (CDisplay*, int, void*, u8*), this)(new_material - 6, hier, tint);
            break;
        }
        if (0xa <= old_material && old_material <= 0x10)
        {
            old_name[5] = '0';
            old_name[6] = '0';
            EQ_METHOD(1, int, 0x00406D7D, (CDisplay*, EQPlayer*), this)(player);
            EQ_METHOD(3, int, 0x00407E46, (CDisplay*, EQPlayer*, int, int), this)(player, 1, 0);
        }
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        old_name[8]++;
        new_name[8]++;
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        break;
    case 2:
    case 3:
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        break;
    case 4:
    case 6:
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        old_name[8]++;
        new_name[8]++;
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        break;
    case 5:
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        old_name[8]++;
        new_name[8]++;
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        old_name[8]++;
        new_name[8]++;
        this->ReplaceMaterial(old_name, new_name, hier, tint, 0);
        break;
    }

    player->equipment[equip_slot] = (u8)new_material;
    player->equip_colors[equip_slot] = colour;
    return 1;
}

// FUNCTION: TRILOGY 0x00407e46
// Replaces a matching attached DM sprite skin with the requested variant.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: getSkinCount spills to a param slot and 0x53d8cc is hoisted into EBX, so the register and stack-slot assignment still differ.
/* fake match: the skin count is stored in temp, which VC6 keeps in the original's [ebp+0x10] slot; a separate local moves it. */
int __fastcall CDisplay_SwapBody_guess(CDisplay* self, int edx, EQPlayer* playerObject,
                                       int oldVariant, int temp)
{
    int* actorInfo;
    void* attachedModel;
    int skinIndex;
    s3dGetNumSkinsAttachedToHierarchicalSprite_t getSkinCount;
    void* skinObject;
    void* skinDefinition;
    void* palette;
    void* replacementSkin;
    char replacementDefinitionName[64];
    char attachedSkinName[64];
    char oldSkinName[64];
    char result[16];
    void* replacementDefinition;
    typedef void* (__cdecl *s3dGetDMSpriteDefinition_guess_t)(void*);
    typedef void* (__cdecl *s3dCreateDMSprite_guess_t)(void*, void*, int);
    typedef void (__cdecl *s3dSetDMSpriteMaterialPalette_guess_t)(void*, void*);
    typedef void (__cdecl *s3dDetachSkinFromHierarchicalSprite_guess_t)(void*, int, void*);
    typedef void (__cdecl *s3dAttachSkinToHierarchicalSprite_guess_t)(void*, void*, int);

    if (playerObject != 0 && (actorInfo = (int*)playerObject->actor_info) != 0 && *actorInfo != 0 &&
        (attachedModel = (void*)actorInfo[0x94]) != 0)
    {
        getSkinCount = s3dGetNumSkinsAttachedToHierarchicalSprite;
        if ((unsigned int)getSkinCount(attachedModel) >= 1)
        {
            if (oldVariant == temp)
                return 1;
            EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), playerObject)(result);
            if (temp != 0)
                sprintf(replacementDefinitionName, EQ_ARRAY(char, 0x0053D8DC), result, temp);
            else
                sprintf(replacementDefinitionName, EQ_ARRAY(char, 0x0053D8CC), result);
            replacementDefinition = t3dGetPointerFromDictionary(self->world_guess, replacementDefinitionName);
            if (replacementDefinition != 0)
            {
                if (oldVariant != 0)
                    sprintf(oldSkinName, EQ_ARRAY(char, 0x0053D8DC), result, oldVariant);
                else
                    sprintf(oldSkinName, EQ_ARRAY(char, 0x0053D8CC), result);
                temp = getSkinCount(attachedModel);
                skinIndex = 0;
                if (temp > 0)
                {
                    do
                    {
                        skinObject = s3dGetSkinAttachedToHierarchicalSprite(skinIndex, attachedModel);
                        skinDefinition = EQ_GLOBAL(s3dGetDMSpriteDefinition_guess_t, 0x006E50FC)(skinObject);
                        EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(skinDefinition, attachedSkinName);
                        if (strcmp(oldSkinName, attachedSkinName) == 0)
                        {
                            palette = s3dGetDMSpriteMaterialPalette(skinObject);
                            replacementSkin = EQ_GLOBAL(s3dCreateDMSprite_guess_t, 0x006E50CC)(self->world_guess, replacementDefinition, 0);
                            if (replacementSkin == 0)
                                return -1;
                            EQ_GLOBAL(s3dSetDMSpriteMaterialPalette_guess_t, 0x006E5134)(replacementSkin, palette);
                            EQ_GLOBAL(s3dDetachSkinFromHierarchicalSprite_guess_t, 0x006E50E4)(self->world_guess, skinIndex, attachedModel);
                            EQ_GLOBAL(s3dAttachSkinToHierarchicalSprite_guess_t, 0x006E50BC)(attachedModel, replacementSkin, -1);
                            break;
                        }
                        skinIndex = skinIndex + 1;
                    } while (skinIndex < temp);
                }
                return skinIndex != temp;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00407ff1
// Sets the player's actor surface-pitch mode based on stance and race.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Pitch type 3 for the stand states 's'/'x' and some races, 2 for others, 1 for the rest. The cases
// are grouped by result; VC6 turns the switch into the original's compare tree. Each arm makes its own
// call and breaks to a single return (a shared pitchType local keeps the value in a register instead).
int CDisplay::SetPlayerPitchType(EQPlayer* player)
{
    void* actor;

    if (player == 0 || player->actor_info == 0
        || (actor = player->actor_info->actor) == 0)
        return 0;

    if (player->stand_state == 'x' || player->stand_state == 's')
    {
        t3dSetActorSurfacePitchType(actor, 3);
        return 1;
    }
    switch (player->race)
    {
    case 22: case 31: case 36: case 37: case 38: case 57: case 80: case 84: case 91:
    case 129: case 145: case 149: case 201: case 203:
        t3dSetActorSurfacePitchType(actor, 3);
        break;
    case 14: case 16: case 41: case 42: case 43: case 48: case 49: case 50: case 63: case 76:
    case 83: case 87: case 107: case 119: case 120: case 121: case 122: case 124: case 135:
    case 158: case 196: case 199: case 200: case 206:
        t3dSetActorSurfacePitchType(actor, 2);
        break;
    default:
        t3dSetActorSurfacePitchType(actor, 1);
        break;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00408156
// Keeps a heading within the game's 0-to-512 angle range.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The heading stays on the x87 stack (not rounded to float) until it is returned. The lower bound
// compares against a double 0.0, the step is a float 512.0.
float CDisplay::FixHeading_guess(float heading)
{
    int i;

    for (i = 0; heading >= 512.0f; i++)
    {
        heading -= 512.0f;
        if (i > 5)
        {
            heading = 0.0f;
            break;
        }
    }
    for (i = 0; heading < 0.0; i++)
    {
        heading += 512.0f;
        if (i > 5)
        {
            heading = 0.0f;
            break;
        }
    }
    return heading;
}

// FUNCTION: TRILOGY 0x004081a3
// Measures the shortest angular difference between two headings.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
float CDisplay::HeadingDiff(float h1, float h2, float* pDiff)
{
    float lo;
    float hi;
    float diff;

    if (h1 > h2)
    {
        hi = h1;
        lo = h2;
    }
    else
    {
        hi = h2;
        lo = h1;
    }
    diff = hi - lo;
    if (diff > 256.0f)
    {
        diff -= 512.0f;
        if (pDiff)
            *pDiff = diff;
        return -diff;
    }
    if (pDiff)
        *pDiff = diff;
    return diff;
}

// FUNCTION: TRILOGY 0x004081f2
// Draws a colored rectangle on the game screen using the selected blend mode.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// A flat-coloured screen quad (left, top, right, bottom). The corners' z is never set (left as whatever
// is on the stack). Mode 0 draws nothing; 1..3 select a blend in bits 18..19 of the flags.
void CDisplay::DoPoly(u8 r, u8 g, u8 b, u8 mode, int unused, int* rect)
{
    float corners[4][3];
    QuadColor_guess color;
    float left;
    float top;
    float right;
    float bottom;
    u32 flags;

    flags = 0x40000003;
    left = (float)rect[0];
    corners[0][0] = left;
    top = (float)rect[1];
    corners[0][1] = top;
    right = (float)rect[2];
    corners[1][0] = right;
    corners[1][1] = top;
    corners[2][0] = left;
    bottom = (float)rect[3];
    corners[3][0] = right;
    corners[2][1] = bottom;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = 0;
    corners[3][1] = bottom;
    switch (mode)
    {
    case 0:
        return;
    case 1:
        flags = 0x41040003;
        break;
    case 2:
        flags = 0x41080003;
        break;
    case 3:
        flags = 0x410C0003;
        break;
    }
    t3dDefer2DQuad(flags, &color, &corners[0][0]);
}

// FUNCTION: TRILOGY 0x0040827a
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// TAKP name; the body destroys a particle cloud in the world.
void CDisplay::DeleteActorFromDisplay_guess(void* cloud)
{
    t3dDestroyParticleCloud(world_guess, cloud);
}

// FUNCTION: TRILOGY 0x0040828c
// Applies the current day or night schedule to a door's open state.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Doors whose parameter is 2.0 close in daytime (hours 5..18) unless alwaysOpenDoors_guess is set;
// doors with 1.0 are open in daytime only. 0.0 and other values: untouched.
void CDisplay::UpdateDoorStateBasedOnWorldTime_guess(EQSwitch* sw)
{
    if (sw->default_pos.auxiliary_guess == 0.0f)
        return;
    if (sw->default_pos.auxiliary_guess == 2.0f)
    {
        if (alwaysOpenDoors_guess != 0 || g_pWorldTime_guess->hour < 5 || g_pWorldTime_guess->hour > 18)
        {
            sw->state = 1;
            return;
        }
    }
    else
    {
        if (sw->default_pos.auxiliary_guess != 1.0f)
            return;
        if (g_pWorldTime_guess->hour > 4 && g_pWorldTime_guess->hour < 19)
        {
            sw->state = 1;
            return;
        }
    }
    sw->state = 0;
}

// FUNCTION: TRILOGY 0x004082f0
// Animates a door or switch between its open and closed positions.
// @early-stop: entry/type block order, x87 absolute-value branches, and the helper call's EDX setup still differ
/* fake match: gotos join type paths at the shared speed-sign block, matching the original control-flow join */
void __stdcall EQSwitch_UpdateDoorPosition_guess(EQSwitch* sw, float speed, int changeState, float dt)
{
    int type;
    float step;
    float* position;
    float* defaultPosition;
    float endPosition;
    float nextPosition;
    float difference;

    if (speed == EQ_GLOBAL(float, 0x0053510C))
        speed = 0.35f;

    type = sw->type;
    step = speed;
    if (type <= 0x4e)
    {
        if (type != 0x4e)
        {
            if (type <= 0x16)
            {
                if (type < 0x14)
                {
                    if (type >= 0xa)
                    {
                        if (type > 0xc)
                        {
                            if (type > 0xe && type <= 0x11)
                                step = EQ_GLOBAL(float, 0x00535124);
                        }
                        else
                            step = EQ_GLOBAL(float, 0x00535120);
                    }
                }
                else
                    step = EQ_GLOBAL(float, 0x0053511C);
                goto apply_speed_sign;
            }
            if (type < 0x19 || type > 0x1b)
                goto apply_speed_sign;
        }
    }
    else
    {
        switch (type)
        {
        case 0x4f:
        case 0x51:
            speed = -speed;
            break;
        case 0x50:
            break;
        default:
            goto apply_speed_sign;
        }
    }

    if (sw->param_guess < 1)
        step = EQ_GLOBAL(float, 0x00535114);
    else
        step = (float)(int)sw->param_guess;

apply_speed_sign:
    if (speed < EQ_GLOBAL(float, 0x0053510C))
        step = -step;

    if (sw->pos.heading == EQ_GLOBAL(float, 0x00535118))
    {
        position = &sw->pos.y;
        defaultPosition = &sw->default_pos.y;
    }
    else if (sw->pos.heading == EQ_GLOBAL(float, 0x005350F8))
    {
        position = &sw->pos.x;
        defaultPosition = &sw->default_pos.x;
    }
    else
    {
        if (sw->pos.heading == EQ_GLOBAL(float, 0x00535110))
        {
            position = &sw->pos.y;
            defaultPosition = &sw->default_pos.y;
        }
        else
        {
            position = &sw->pos.x;
            defaultPosition = &sw->default_pos.x;
        }
        speed = -speed;
        step = -step;
    }

    endPosition = step + *defaultPosition;
    if (step < EQ_GLOBAL(float, 0x0053510C))
        step = -step;

    if (sw->state == 2)
    {
        nextPosition = speed * dt + *position;
        *position = nextPosition;
        difference = nextPosition - *defaultPosition;
        if (difference < EQ_GLOBAL(float, 0x0053510C))
            difference = -difference;
        if (difference < step)
            return;
        *position = endPosition;
        if (changeState == 0)
            return;
        EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(1, 0);
    }
    else
    {
        nextPosition = *position - speed * dt;
        *position = nextPosition;
        difference = nextPosition - endPosition;
        if (difference < EQ_GLOBAL(float, 0x0053510C))
            difference = -difference;
        if (difference < step)
            return;
        *position = *defaultPosition;
        if (changeState == 0)
            return;
        EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(0, 0);
    }
    return;
}

// FUNCTION: TRILOGY 0x00408482
// Animates doors each frame: swings, slides or lifts moving ones by door type and toggles back timed-out doors.
// @early-stop: remaining differences are mostly state-call EDX setup and x87 branch layout
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// CDisplay::ProcessSwitches_guess. display.h declares no ProcessSwitches_guess, so this is a file-local stand-in:
// __fastcall with `display` in ECX is the original's __thiscall (one pointer, no stack arguments, callee pops 0).
/* fake match: the reserved location word matches VC6's frame; gotos retain shared state and motion joins. */
void __fastcall CDisplay_ProcessSwitches_guess(CDisplay* display)
{
    typedef int (__cdecl *SetCloudFn_guess_t)(void* world, void* actor, int ms);   // slot 0x006E5400

    EQSwitch* sw;               // iVar5: the switch being processed
    float stackValue;           // fStack_20
    u32 elapsed;                // local_24: ms since the previous pass
    float dt;                   // local_14: elapsed * 0.02
    float amount;               // local_8
    float speed;                // local_c
    float reverseSpeed;         // local_10
    float particlePos[3];       // local_30..local_28
    struct
    {
        SwitchPos_guess pos;    // local_4c..local_3c
        float zero;             // local_38
        float reserved;
    } location;
    u32 newState;
    u32 stateChangeTime;
    u32 timeInState;
    u8 oldState;
    u8 currentState;
    u8 switchType;
    float computed;
    float distance;
    float tmp;
    float* movingCoord;
    float* defaultCoord;

    sw = g_switchList_guess;
    elapsed = EQ_CDECL(u32, 0x004A461E, (void))() - EQ_GLOBAL(u32, 0x005917C4);
    dt = (float)elapsed * EQ_GLOBAL(float, 0x00535130);
    EQ_GLOBAL(u32, 0x005917C4) = EQ_CDECL(u32, 0x004A461E, (void))();

    if (sw != 0)
    {
        do
        {
            EQSwitch* next;      // local_1c
            next = sw->next;
            if (sw->type == 0x35)
            {
                display->UpdateDoorStateBasedOnWorldTime_guess(sw);
                if (sw->state == 1 && sw->actor == 0)
                {
                    if (sw->param_guess > 0 && sw->param_guess < 0x100 && g_local_player_guess != 0)
                    {
                        particlePos[0] = sw->pos.y;
                        particlePos[1] = sw->pos.x;
                        particlePos[2] = sw->pos.z;
                        sw->actor = EQ_CDECL(void*, 0x004C7F71, (int, void*, EQPlayer*, int, int, void*, int, int))(
                            2, (char*)EQ_ARRAY(u8, 0x00604990) + sw->param_guess * 0xae8,
                            g_local_player_guess, 0, 0, particlePos, 0, 43200000);
                    }
                }
                else if (sw->state == 0)
                {
                    if (sw->actor != 0)
                    {
                        EQ_GLOBAL(SetCloudFn_guess_t, 0x006E5400)(display->world_guess, sw->actor, 1000);
                        sw->actor = 0;
                    }
                }
            }
            switchType = sw->type;
            if (switchType != 0x35 && switchType >= 0x32 && switchType < 0x3b)
            {
                sw = next;
                continue;
            }

        currentState = sw->state;
        oldState = sw->defaultState_guess;
        timeInState = EQ_GLOBAL(u32, 0x005917C4) - sw->stateChangeTime_guess;
        if (currentState == oldState)
            goto processMotion;
        if (currentState == 4)
        {
            if (timeInState <= 0xea60)
            {
                sw = next;
                continue;
            }
            EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(oldState, 0);
        }
        else
        {
            stateChangeTime = EQ_METHOD(0, u32, 0x004BDDF1, (EQSwitch*), sw)();
            if (stateChangeTime != 0 && timeInState > stateChangeTime)
                EQ_METHOD(0, void, 0x004BDD20, (EQSwitch*), sw)();
            goto processMotion;
        }

processMotion:
    switchType = sw->type;
    if (switchType == 0x35 || switchType == 0x39 || switchType == 0x3a)
    {
        if (sw->state == 2)
            newState = 1;
        else
        {
            if (sw->state != 3)
            {
                sw = next;
                continue;
            }
            newState = 0;
        }
        EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(newState, 0);
        sw = next;
        continue;
    }
    if (sw->state == 0 || (sw->state == 1 && switchType < 100))
    {
        sw = next;
        continue;
    }

    amount = 128.0f;
    if (switchType <= 7)
    {
        speed = 8.0f;
        if (switchType >= 5 && switchType <= 7)
        {
            speed = -8.0f;
            amount = -128.0f;
        }
        tmp = display->FixHeading_guess(amount + sw->default_pos.heading);
        stackValue = tmp;
        if (amount < EQ_GLOBAL(float, 0x0053510C))
            amount = -amount;
        if (sw->state == 2)
        {
            computed = speed * dt + sw->pos.heading;
            sw->pos.heading = computed;
            tmp = display->FixHeading_guess(computed);
            sw->pos.heading = tmp;
            tmp = display->HeadingDiff(tmp, sw->default_pos.heading, 0);
            if (amount <= tmp)
            {
                sw->pos.heading = stackValue;
changeToState1:
                newState = 1;
applyNewState:
                EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(newState, 0);
            }
        }
        else
        {
            computed = sw->pos.heading - speed * dt;
            sw->pos.heading = computed;
            tmp = display->FixHeading_guess(computed);
            sw->pos.heading = tmp;
            tmp = display->HeadingDiff(tmp, stackValue, 0);
            if (amount <= tmp)
            {
                sw->pos.heading = sw->default_pos.heading;
changeToState0:
                newState = 0;
                goto applyNewState;
            }
        }
    }
    else if (switchType == 0x28 || switchType == 0x1e || switchType == 0x23 || switchType == 0x82)
    {
        switch (switchType)
        {
        case 0x1e:
            reverseSpeed = 4.0f;
            speed = 10.0f;
            break;
        case 0x23:
            reverseSpeed = 8.0f;
            speed = 16.0f;
            break;
        case 0x82:
            speed = 16.0f;
            reverseSpeed = 16.0f;
            amount = 248.0f;
            break;
        default:
            speed = 8.0f;
            reverseSpeed = 8.0f;
            break;
        }
        stackValue = sw->default_pos.auxiliary_guess - amount;
        if (amount < EQ_GLOBAL(float, 0x0053510C))
            amount = -amount;
        if (sw->state == 2)
        {
            tmp = display->HeadingDiff(sw->pos.auxiliary_guess, sw->default_pos.auxiliary_guess, 0);
            if (amount <= tmp)
            {
                sw->pos.auxiliary_guess = stackValue;
                goto changeToState1;
            }
            computed = sw->pos.auxiliary_guess - speed * dt;
        }
        else
        {
            if (sw->state != 3)
                goto slideCheck;
            tmp = display->HeadingDiff(sw->pos.auxiliary_guess, stackValue, 0);
            if (amount <= tmp)
            {
                sw->pos.auxiliary_guess = sw->default_pos.auxiliary_guess;
                goto changeToState0;
            }
            computed = speed * dt + sw->pos.auxiliary_guess;
        }
        sw->pos.auxiliary_guess = computed;
        tmp = display->FixHeading_guess(computed);
        sw->pos.auxiliary_guess = tmp;
    }
    else if (((((switchType < 0x3c) || (0x4d < switchType)) && ((switchType < 0x8c || (0x92 < switchType)))) &&
             ((switchType != 0x78 && (switchType != 0x3b)))) && ((switchType < 0x4e || (0x51 < switchType))))
    {
        if (switchType < 10 || 0x1b < switchType)
        {
            if ((switchType >= 100 && switchType <= 0x6b) || switchType == 0x73 || switchType == 0x74)
            {
                movingCoord = &sw->pos.heading;
                if (switchType >= 0x69)
                    movingCoord = &sw->pos.auxiliary_guess;
                tmp = EQ_METHOD(1, float, 0x004BDD62, (EQSwitch*, float*), sw)(&speed);
                stackValue = tmp;
                if (sw->state == 2)
                {
                    computed = speed + sw->currentSpeed_guess;
                    sw->currentSpeed_guess = computed;
                    if (computed < stackValue)
                        goto accelApply;
                    newState = 1;
accelChangeState:
                    EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(newState, 0);
                }
                else
                {
                    computed = stackValue;
                    if (sw->state != 1)
                    {
                        computed = sw->currentSpeed_guess - speed;
                        sw->currentSpeed_guess = computed;
                        if (computed <= EQ_GLOBAL(float, 0x0053510C))
                        {
                            newState = 0;
                            goto accelChangeState;
                        }
                    }
accelApply:
                    *movingCoord = computed * dt + *movingCoord;
                }
                tmp = display->FixHeading_guess(*movingCoord);
                *movingCoord = tmp;
            }
        }
        else
        {
            EQ_STDCALL(void, 0x004082F0, (EQSwitch*, float, int, float))(sw, 0.0f, 1, dt);
        }
    }

    else
    {
        speed = 0.25f;
        reverseSpeed = 0.25f;
        switchType = sw->type;
        if (switchType <= 0x4f)
        {
            if (switchType <= 0x4d)
            {
                if (switchType <= 0x43)
                {
                    if (switchType >= 0x41)
                    {
                        amount = 10.0f;
                        goto setFastSpeed;
                    }
                    if (switchType == 0x3b)
                    {
                        amount = (float)sw->param_guess;
                        speed = 0.45f;
                        reverseSpeed = 0.45f;
                    }
                    else if (0x3b < switchType && switchType <= 0x3e)
                    {
                        amount = 8.0f;
                        goto setFastSpeed;
                    }
                }
                else if (switchType >= 0x46)
                {
                    if (switchType <= 0x48)
                        amount = 14.0f;
                    else
                    {
                        if (switchType <= 0x4a || 0x4d < switchType)
                            goto verticalMove;
                        if (sw->param_guess < 1)
                            amount = 22.0f;
                        else
                            amount = (float)sw->param_guess;
                    }
setFastSpeed:
                    speed = 0.45f;
                }
            }
            else
            {
                EQ_STDCALL(void, 0x004082F0, (EQSwitch*, float, int, float))(sw, 0.0f, 0, dt);
                amount = (float)sw->param_guess;
                speed = reverseSpeed = 0.45f;
            }
        }
        else if (switchType >= 0x50)
        {
            if (switchType <= 0x51)
            {
                EQ_STDCALL(void, 0x004082F0, (EQSwitch*, float, int, float))(sw, 0.0f, 0, dt);
                amount = -(float)sw->param_guess;
                speed = reverseSpeed = -0.45f;
            }
            else
            {
                if (switchType == 0x78 || switchType == 0x8c)
                    speed = -1.0f;
                else
                {
                    if (switchType != 0x91)
                    {
                        if (switchType == 0x92)
                        {
                            amount = -10.0f;
                            speed = -1.0f;
                            reverseSpeed = -1.0f;
                        }
                        goto verticalMove;
                    }
                    speed = -0.25f;
                }
                amount = -10.0f;
                reverseSpeed = -0.25f;
            }
        }

verticalMove:
        computed = amount + sw->default_pos.z;
        if (amount < EQ_GLOBAL(float, 0x0053510C))
            amount = -amount;
        if (sw->state == 2)
        {
            distance = speed * dt + sw->pos.z;
            sw->pos.z = distance;
            distance = distance - sw->default_pos.z;
            if (distance < EQ_GLOBAL(float, 0x0053510C))
                distance = -distance;
            if (amount <= distance)
            {
                sw->pos.z = computed;
                goto changeToState1;
            }
        }
        else if (sw->state == 3)
        {
            distance = sw->pos.z - reverseSpeed * dt;
            sw->pos.z = distance;
            distance = distance - computed;
            if (distance < EQ_GLOBAL(float, 0x0053510C))
                distance = -distance;
            if (amount <= distance)
            {
                sw->pos.z = sw->default_pos.z;
                goto changeToState0;
            }
        }
    }

slideCheck:
    switchType = sw->type;
    if (switchType != 0x2d && switchType != 0x6d && switchType != 0x73 && switchType != 0x74 && switchType != 0x7d &&
        (switchType < 0x96 || switchType > 0x98))
        goto updateActor;
    if (switchType < 0x7e)
    {
        if (switchType == 0x7d)
        {
            reverseSpeed = 0.35f;
            speed = 0.75f;
            goto slideAmount10;
        }
        if (switchType == 0x2d || switchType == 0x6d)
        {
            speed = -0.05f;
            reverseSpeed = -0.05f;
            amount = -1.0f;
        }
        else if (switchType == 0x73)
        {
            speed = 0.75f;
            reverseSpeed = 0.75f;
            amount = 20.0f;
        }
        else if (switchType == 0x74)
        {
            speed = 0.75f;
            reverseSpeed = 0.75f;
            amount = 40.0f;
        }
    }
    else
    {
        switch (switchType)
        {
        case 0x96:
            reverseSpeed = 0.25f;
            speed = 1.0f;
            break;
        case 0x97:
            speed = 0.25f;
            reverseSpeed = 0.25f;
            break;
        case 0x98:
            speed = 1.0f;
            reverseSpeed = 1.0f;
            break;
        default:
            goto slideMove;
        }
slideAmount10:
        amount = 10.0f;
    }

slideMove:
    if (sw->pos.heading == EQ_GLOBAL(float, 0x0053510C))
    {
        movingCoord = &sw->pos.y;
        defaultCoord = &sw->default_pos.y;
    }
    else if (sw->pos.heading == EQ_GLOBAL(float, 0x00535118))
    {
        movingCoord = &sw->pos.x;
        defaultCoord = &sw->default_pos.x;
    }
    else
    {
        if (sw->pos.heading == EQ_GLOBAL(float, 0x005350F8))
        {
            movingCoord = &sw->pos.y;
            defaultCoord = &sw->default_pos.y;
        }
        else
        {
            movingCoord = &sw->pos.x;
            defaultCoord = &sw->default_pos.x;
        }
        speed = -speed;
        reverseSpeed = -reverseSpeed;
        amount = -amount;
    }
    computed = amount + *defaultCoord;
    if (amount < EQ_GLOBAL(float, 0x0053510C))
        amount = -amount;
    if (sw->state == 2)
    {
        distance = speed * dt + *movingCoord;
        *movingCoord = distance;
        distance = distance - *defaultCoord;
        if (distance < EQ_GLOBAL(float, 0x0053510C))
            distance = -distance;
        if (distance < amount)
            goto updateActor;
        *movingCoord = computed;
        newState = 1;
    }
    else
    {
        if (sw->state != 3)
            goto updateActor;
        distance = *movingCoord - reverseSpeed * dt;
        *movingCoord = distance;
        distance = distance - computed;
        if (distance < EQ_GLOBAL(float, 0x0053510C))
            distance = -distance;
        if (distance < amount)
            goto updateActor;
        *movingCoord = *defaultCoord;
        newState = 0;
    }
    EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(newState, 0);

updateActor:
    if (sw->state == 1)
    {
        switchType = sw->type;
        if (switchType != 0x73 && switchType >= 0x6d && switchType <= 0x98)
            EQ_METHOD(2, void, 0x004BDF41, (EQSwitch*, int, int), sw)(3, 0);
    }
    location.pos.y = sw->pos.y;
    location.pos.x = sw->pos.x;
    location.pos.z = sw->pos.z;
    location.pos.heading = sw->pos.heading;
    location.pos.auxiliary_guess = sw->pos.auxiliary_guess;
    location.zero = 0.0f;
    location.reserved = 0.0f;
    t3dSetActorLocation(display->world_guess, sw->actor, (float*)&location);
            sw = next;
        } while (sw != 0);
    }
}

// FUNCTION: TRILOGY 0x00408d5b
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored.
void CDisplay::default_cameras_guess()
{
    g_default_cameras_guess[0].value[0] = 0.0f;
    g_default_cameras_guess[0].value[1] = 0.0f;
    g_default_cameras_guess[0].value[2] = 0.0f;
    g_default_cameras_guess[0].value[3] = 0.0f;
    g_default_cameras_guess[0].value[4] = 0.0f;
    g_default_cameras_guess[0].value[5] = 110.0f;
    g_default_cameras_guess[0].flag = 0;
    g_default_cameras_guess[1].value[0] = 0.0f;
    g_default_cameras_guess[1].value[1] = 0.0f;
    g_default_cameras_guess[1].value[2] = 25.0f;
    g_default_cameras_guess[1].value[3] = 0.0f;
    g_default_cameras_guess[1].value[4] = -100.0f;
    g_default_cameras_guess[1].value[5] = 90.0f;
    g_default_cameras_guess[1].flag = 0;
    g_default_cameras_guess[2].value[0] = 0.0f;
    g_default_cameras_guess[2].value[1] = 0.0f;
    g_default_cameras_guess[2].value[2] = 0.0f;
    g_default_cameras_guess[2].value[3] = 0.0f;
    g_default_cameras_guess[2].value[4] = 0.0f;
    g_default_cameras_guess[2].value[5] = 90.0f;
    g_default_cameras_guess[2].flag = 1;
    g_default_cameras_guess[3].value[0] = 30.0f;
    g_default_cameras_guess[3].value[1] = 192.0f;
    g_default_cameras_guess[3].value[2] = 15.0f;
    g_default_cameras_guess[3].value[3] = 0.001f;
    g_default_cameras_guess[3].value[4] = 0.001f;
    g_default_cameras_guess[3].value[5] = 90.0f;
    g_default_cameras_guess[3].flag = 1;
    g_default_cameras_guess[4].value[0] = 82.0f;
    g_default_cameras_guess[4].value[1] = 277.0f;
    g_default_cameras_guess[4].value[2] = 18.0f;
    g_default_cameras_guess[4].value[3] = 0.001f;
    g_default_cameras_guess[4].value[4] = 0.001f;
    g_default_cameras_guess[4].value[5] = 90.0f;
    g_default_cameras_guess[4].flag = 1;
}

// FUNCTION: TRILOGY 0x00408e7f
// Reports the stored 3D display state.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::is_3dON_guess()
{
    return is3dOn_guess;
}

// FUNCTION: TRILOGY 0x00408e86
// Sets up the color palette used to render the world.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::CreateWorldPalette_guess(void* a, void* b)
{
    if (palette_guess != 0)
        t3dDestroyPalette(world_guess, palette_guess);
    palette_guess = 0;
    palette_guess = t3dCreatePalette(world_guess, a, b, 1);
    if (palette_guess == 0)
        palette_failed_guess = 1;
}

// Stand-in until display.h declares InitCommonLights (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::InitCommonLights once
// the declaration is in the class.
struct CDisplay_InitCommonLights_guess : CDisplay
{
    void InitCommonLights(void);
};

// FUNCTION: TRILOGY 0x00408ed1
// Defines the common light types (torch, candle, lanterns, magic, red, blue) in the zone world at zone load.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall, void. Without a world it logs the failure and returns. Otherwise it defines the nine common lights on
// the world (torch, candle, small and large lantern, magic torch, small and large magic, red, blue) and stores each
// handle in light_defs_guess[0..8]. The two 10-dword tables at 0x58fc44 and 0x590184 are refilled on every call, and
// 0x58fc44 is then overwritten with 0.8f.
void CDisplay_InitCommonLights_guess::InitCommonLights(void)
{
    typedef void* (__cdecl *t3dDefineLight_guess_t)(void*, char*, int, void*, int, int, int);
    typedef void (__cdecl *t3dSetLightColorIntensityList_guess_t)(void*, float*);

    float blue_list[10][3];     // [ebp-0x1ec]
    float red_list[10][3];      // [ebp-0x174]
    float torch_list[10][3];    // [ebp-0xfc]
    float magic_list[10][3];    // [ebp-0x84]
    float lantern_list[3];      // [ebp-0xc]
    u32* table_ptr;
    int count;
    void* light;

    if (world_guess == 0)
    {
        DbgLog_guess(EQ_ARRAY(char, 0x0053D94C), 0, is3dOn_guess);
        return;
    }
    lantern_list[0] = EQ_GLOBAL(float, 0x00535150);
    lantern_list[1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[0][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[0][1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[0][2] = 0.0f;
    torch_list[1][0] = EQ_GLOBAL(float, 0x0053514C);
    torch_list[1][1] = EQ_GLOBAL(float, 0x0053514C);
    table_ptr = EQ_ARRAY(u32, 0x0058FC44);
    for (count = 10; count != 0; count--)
    {
        *table_ptr = 0x3f7ff972;    // 0.9999f
        table_ptr = table_ptr + 1;
    }
    torch_list[1][2] = 0.0f;
    torch_list[2][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[2][1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[2][2] = 0.0f;
    lantern_list[2] = 0.4f;
    torch_list[3][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[3][2] = 0.0f;
    torch_list[3][1] = 0.8812f;
    torch_list[4][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[4][1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[5][2] = 0.075f;
    torch_list[4][2] = 0.0f;
    torch_list[5][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[5][1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[6][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[6][1] = EQ_GLOBAL(float, 0x00535150);
    table_ptr = EQ_ARRAY(u32, 0x00590184);
    for (count = 10; count != 0; count--)
    {
        *table_ptr = 0x3f7ff972;    // 0.9999f
        table_ptr = table_ptr + 1;
    }
    torch_list[6][2] = 0.0f;
    torch_list[7][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[7][2] = 0.0f;
    torch_list[8][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[8][1] = EQ_GLOBAL(float, 0x00535150);
    torch_list[7][1] = 0.8031f;
    torch_list[9][2] = 0.075f;
    torch_list[8][2] = 0.0f;
    torch_list[9][0] = EQ_GLOBAL(float, 0x00535150);
    torch_list[9][1] = EQ_GLOBAL(float, 0x00535150);

    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D944), 10, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[0] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &torch_list[0][0]);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D93C), 10, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[6] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &torch_list[0][0]);

    EQ_GLOBAL(float, 0x0058FC44) = 0.8f;
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D930), 1, EQ_GLOBAL(void*, 0x0053C6A0), 0, 200, 0);
    light_defs_guess[2] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, lantern_list);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D924), 1, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[3] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, lantern_list);

    magic_list[0][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[3][1] = 0.9012f;
    magic_list[0][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[0][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[7][1] = 0.9031f;
    magic_list[1][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[1][1] = EQ_GLOBAL(float, 0x0053514C);
    magic_list[1][2] = EQ_GLOBAL(float, 0x0053514C);
    magic_list[2][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[2][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[2][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[3][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[3][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[4][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[4][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[4][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[5][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[5][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[5][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[6][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[6][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[6][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[7][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[7][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[8][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[8][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[8][2] = EQ_GLOBAL(float, 0x00535150);
    magic_list[9][0] = EQ_GLOBAL(float, 0x00535148);
    magic_list[9][1] = EQ_GLOBAL(float, 0x00535150);
    magic_list[9][2] = EQ_GLOBAL(float, 0x00535150);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D918), 10, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[1] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &magic_list[0][0]);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D910), 1, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[4] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &magic_list[0][0]);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D908), 1, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[5] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &magic_list[0][0]);

    red_list[9][1] = 0.26f;
    red_list[0][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[0][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[0][2] = 0.0f;
    red_list[1][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[1][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[1][2] = 0.0f;
    red_list[2][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[2][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[2][2] = 0.0f;
    red_list[3][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[3][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[3][2] = 0.0f;
    red_list[4][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[4][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[4][2] = 0.0f;
    red_list[5][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[5][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[5][2] = 0.0f;
    red_list[6][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[6][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[6][2] = 0.0f;
    red_list[7][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[7][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[7][2] = 0.0f;
    red_list[8][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[8][1] = EQ_GLOBAL(float, 0x00535144);
    red_list[8][2] = 0.0f;
    red_list[9][0] = EQ_GLOBAL(float, 0x005344A8);
    red_list[9][2] = 0.0f;
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D8FC), 1, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[7] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &red_list[0][0]);

    blue_list[0][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[0][1] = 0.2f;
    blue_list[0][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[1][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[1][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[1][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[2][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[2][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[2][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[3][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[3][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[3][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[4][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[4][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[4][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[5][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[5][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[5][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[6][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[6][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[6][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[7][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[7][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[7][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[8][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[8][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[8][2] = EQ_GLOBAL(float, 0x005344A8);
    blue_list[9][0] = EQ_GLOBAL(float, 0x00535140);
    blue_list[9][1] = EQ_GLOBAL(float, 0x0053513C);
    blue_list[9][2] = EQ_GLOBAL(float, 0x005344A8);
    light = (*EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200))(world_guess, EQ_ARRAY(char, 0x0053D8F0), 1, EQ_GLOBAL(void*, 0x0053C69C), 0, 200, 0);
    light_defs_guess[8] = light;
    (*EQ_GLOBAL(t3dSetLightColorIntensityList_guess_t, 0x006E53F0))(light, &blue_list[0][0]);
}

// Stand-in until display.h declares InitDDraw_guess (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::InitDDraw_guess once the
// declaration is in the class.
struct CDisplay_InitDDraw_guess : CDisplay
{
    void InitDDraw_guess(void);
};

// Stand-in for the object DisplayObject_guess::DisplayObject_guess constructs: DisplayObject_guess (above) owns that constructor, but its
// definition sits earlier in this TU, where VC6 gives `new` no EH frame. This class only declares the constructor
// (the client's, 0x00402874), so `new` keeps the EH frame the original has.
struct CDisplay_InitDDraw_obj_guess
{
    u8 unknown0x0000[0x1874];   // 0x0000  filled in by the client's constructor (0x00402874)

    // EXTERNAL: TRILOGY 0x00402874 CDisplay_InitDDraw_obj_guess::CDisplay_InitDDraw_obj_guess
    CDisplay_InitDDraw_obj_guess();
};
C_ASSERT(sizeof(CDisplay_InitDDraw_obj_guess) == 0x1874);

// FUNCTION: TRILOGY 0x0040940F
// Graphics startup: opens the 3D device with eqclient.ini video options, then the world, camera and fonts.
// Brings up the renderer: the video settings from eqclient.ini, the t3d device, the world and its callbacks,
// the render context, the camera and the fonts, then the sky and the armor world files.
void CDisplay_InitDDraw_guess::InitDDraw_guess(void)
{
    typedef int (__stdcall *ShowCursor_guess_t)(int);
    typedef void* (__stdcall *SetCursor_guess_t)(void*);
    typedef u32 (__stdcall *GetPrivateProfileStringA_guess_t)(const char*, const char*, const char*, char*, u32,
                                                              const char*);
    typedef int (__cdecl *t3dInitializeDevice_guess_t)(void*, void*, int, int, int);
    typedef void (__cdecl *t3dSetColorKey_guess_t)(int);
    typedef void* (__cdecl *t3dCreateWorld_guess_t)(void*, int);
    typedef void (__cdecl *t3dDefineAI_guess_t)(void*, char*, void*);
    typedef void* (__cdecl *t3dCreateRenderContext_guess_t)(int);
    typedef void (__cdecl *t3dEnableAutoMipMapping_guess_t)(int, int);
    typedef void (__cdecl *t3dSetRenderContextTransparentPen_guess_t)(void*, int);
    typedef void* (__cdecl *t3dCreateCamera_guess_t)(void);
    typedef int (__cdecl *t3dCreateFont_guess_t)(const char*, int, int, int);
    typedef int (__cdecl *t3dInitSky_guess_t)(void*, void*, void*);

    char s3dName[0x40];
    char wldName[0x40];
    char iniBuf[0x20];
    char armorIniValue[0x20];
    int armorSlot;
    int* armorFlagPtr;
    int result;
    char ch;
    void* world;
    void* musicState;
    t3dDefineAI_guess_t defineAI;
    t3dCreateFont_guess_t createFont;

    (*EQ_GLOBAL(ShowCursor_guess_t, 0x0053424C))(0);
    (*EQ_GLOBAL(SetCursor_guess_t, 0x00534250))(0);
    this->unk_0x098 = 0;
    DbgLog_guess((char*)0x0053DB60);
    (*EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C))((char*)0x0053DB3C, (char*)0x0053DB24,
        (char*)0x0053DB48, iniBuf, 0x20, (char*)0x0053DB50);
    // '*' is the "*NULL*" default: the key is missing
    if (iniBuf[0] == '*')
    {
        g_bitsPerPixel_guess = 0x10;
        (*EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188))((char*)0x0053DB3C, (char*)0x0053DB24,
            (char*)0x0053DB20, (char*)0x0053DB50);
    }
    else
    {
        g_bitsPerPixel_guess = atoi(iniBuf);
        // only 16 and 32 bpp are accepted
        if (g_bitsPerPixel_guess != 0x10 && g_bitsPerPixel_guess != 0x20)
        {
            g_bitsPerPixel_guess = 0x10;
        }
    }
    g_useD3DTextureCompression_guess = 0;
    (*EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C))((char*)0x0053DB3C, (char*)0x0053DB04,
        (char*)0x0053DB48, iniBuf, 0x20, (char*)0x0053DB50);
    ch = iniBuf[0];
    if (ch == '*')
    {
        g_useD3DTextureCompression_guess = 0;
        (*EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188))((char*)0x0053DB3C, (char*)0x0053DB04,
            (char*)0x0053DAFC, (char*)0x0053DB50);
    }
    else if (ch == 'T' || ch == 't' || ch == '1')
    {
        g_useD3DTextureCompression_guess = 1;
    }
    result = (*EQ_GLOBAL(t3dInitializeDevice_guess_t, 0x006E531C))((char*)0x005901B0 + g_deviceIndex_guess * 0x15c,
        this->hwnd_guess, 1, g_bitsPerPixel_guess, g_useD3DTextureCompression_guess);
    DbgLog_guess((char*)0x0053DADC, result);
    // magenta is the transparent colour key
    (*EQ_GLOBAL(t3dSetColorKey_guess_t, 0x006E53A4))(0xFF00FF);
    world = (*EQ_GLOBAL(t3dCreateWorld_guess_t, 0x006E51A8))(this->hwnd_guess, 0);
    defineAI = EQ_GLOBAL(t3dDefineAI_guess_t, 0x006E51F8);
    this->world_guess = world;
    (*defineAI)(world, (char*)0x0053DACC, (void*)0x00405818);
    (*defineAI)(this->world_guess, (char*)0x0053DABC, (void*)0x00405854);
    (*defineAI)(this->world_guess, (char*)0x0053DAAC, (void*)0x0040583D);
    (*defineAI)(this->world_guess, (char*)0x0053DA9C, (void*)0x004058F0);
    (*defineAI)(this->world_guess, (char*)0x0053DA8C, (void*)0x00405944);
    (*defineAI)(this->world_guess, (char*)0x0053DA7C, (void*)0x0040598B);
    if (this->render_guess == 0)
    {
        this->render_guess = (*EQ_GLOBAL(t3dCreateRenderContext_guess_t, 0x006E51A0))(0x2A);
        EQ_METHOD(1, void, 0x00410D41, (CDisplay *, int), this)(0);
    }
    // gamma = level * 0.25 + 0.5
    EQ_METHOD(1, void, 0x0040AF4F, (CDisplay *, float), g_cdisplay_guess)(
        g_gammaLevel_guess * EQ_GLOBAL(float, 0x00535154) + EQ_GLOBAL(float, 0x005344A8));
    (*EQ_GLOBAL(t3dEnableAutoMipMapping_guess_t, 0x006E5238))(0, EQ_GLOBAL(int, 0x006B68B0));
    EQ_METHOD(2, void, 0x00408E86, (CDisplay *, void*, void*), this)(this->fastFileHandle_guess,
        (char*)0x0053DA70);
    (*EQ_GLOBAL(t3dSetRenderContextTransparentPen_guess_t, 0x006E5440))(this->render_guess, 10);
    this->camera_guess = (*EQ_GLOBAL(t3dCreateCamera_guess_t, 0x006E5184))();
    if (EQ_GLOBAL(int, 0x0053C6A4) != 0)
    {
        result = EQ_GLOBAL(int, 0x006E20C0);
        EQ_GLOBAL(int, 0x0053C6A4) = 0;
        if (result != 0 && EQ_METHOD(1, int, 0x0040E4E4, (CDisplay *, int), g_cdisplay_guess)(result) == 0)
        {
            EQ_GLOBAL(int, 0x006E20C0) = 0;
            EQ_GLOBAL(int, 0x006B6880) = 1;
            EQ_METHOD(0, void, 0x004CF31A, (CEverQuest *), pinstCEverQuest_guess)();
        }
    }
    CDisplay_ChangeVideoMode_guess(this, 0);
    // shutdown runs ShutdownDisplay_guess while this is set
    this->cleanupPending_guess = 1;
    if (EQ_GLOBAL(int, 0x006E5480) == 1)
    {
        createFont = EQ_GLOBAL(t3dCreateFont_guess_t, 0x006E51D8);
        g_arialFont6_guess = (*createFont)((char*)0x0053DA68, 6, 0, 0);
        g_arialFont7_guess = (*createFont)((char*)0x0053DA68, 7, 0, 0);
        g_arialFont8_guess = (*createFont)((char*)0x0053DA68, 8, 0, 0);
        g_arialFont9_guess = (*createFont)((char*)0x0053DA68, 9, 0, 0);
        g_arialFont10_guess = (*createFont)((char*)0x0053DA68, 10, 0, 0);
        g_arialFont12_guess = (*createFont)((char*)0x0053DA68, 0xC, 0, 0);
        g_arialFont14_guess = (*createFont)((char*)0x0053DA68, 0xE, 1, 0);
    }
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0)
    {
        musicState = g_musicState_guess;
        if (musicState != 0)
        {
            EQ_METHOD(0, void, 0x004E5AC8, (void*), musicState)();
        }
    }
    EQ_METHOD(0, void, 0x004DAB44, (CEverQuest *), pinstCEverQuest_guess)();
    DbgLog_guess((char*)0x0053DA54);
    // Each base name goes into both buffers; LoadWorldFile adds .s3d / .wld
    strcpy(s3dName, (char*)0x0053DA4C);
    strcpy(wldName, (char*)0x0053DA4C);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053DA44);
    strcpy(wldName, (char*)0x0053DA44);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053DA3C);
    strcpy(wldName, (char*)0x0053DA3C);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053DA38);
    strcpy(wldName, (char*)0x0053DA38);
    ch = EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    if (ch == 0)
    {
        DbgLog_guess((char*)0x0053DA14);
    }
    DbgLog_guess((char*)0x0053DA08);
    strcpy(s3dName, (char*)0x0053D9FC);
    strcpy(wldName, (char*)0x0053D9FC);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053D9F0);
    strcpy(wldName, (char*)0x0053D9F0);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053D9E4);
    strcpy(wldName, (char*)0x0053D9E4);
    ch = EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    if (ch == 0)
    {
        this->palette_failed_guess = 0;
        strcpy(s3dName, (char*)0x0053D9DC);
        strcpy(wldName, (char*)0x0053D9DC);
        EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    }
    // armor slots 0x11..0x17; their flags are the 7 ints at 0x006E2078
    armorSlot = 0x11;
    armorFlagPtr = (int*)0x006E2078;
    do
    {
        sprintf(iniBuf, (char*)0x0053D9D0, armorSlot);
        (*EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C))((char*)0x0053DB3C, iniBuf,
            (char*)0x0053DB48, armorIniValue, 0x20, (char*)0x0053DB50);
        ch = armorIniValue[0];
        if (ch == '*')
        {
            (*EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188))((char*)0x0053DB3C, iniBuf,
                (char*)0x0053DAFC, (char*)0x0053DB50);
            result = 0;
        }
        else if (ch == 'F' || ch == 'f' || ch == '0')
        {
            result = 0;
        }
        else
        {
            result = 1;
        }
        *armorFlagPtr = result;
        if (result == 1)
        {
            sprintf(s3dName, (char*)0x0053D9C0, armorSlot);
            sprintf(wldName, (char*)0x0053D9C0, armorSlot);
            EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
        }
        armorSlot = armorSlot + 1;
        armorFlagPtr = armorFlagPtr + 1;
    } while (armorFlagPtr < (int*)0x006E2094);
    strcpy(s3dName, (char*)0x0053D9B4);
    strcpy(wldName, (char*)0x0053D9B4);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    strcpy(s3dName, (char*)0x0053D9A8);
    strcpy(wldName, (char*)0x0053D9A8);
    EQ_METHOD(5, u8, 0x00409B52, (CDisplay *, char*, char*, int, int, u8), this)(s3dName, wldName, 0, 1, 0);
    DbgLog_guess((char*)0x0053D990);
    result = (*EQ_GLOBAL(t3dInitSky_guess_t, 0x006E5328))(this->world_guess, g_skyLayer_guess, (void*)0x0053C6BC);
    if (result == -0xC2C2)
    {
        DbgLog_guess((char*)0x0053D974);
        EQ_GLOBAL(int, 0x006B6870) = 0;
    }
    (*EQ_GLOBAL(t3dEnableAutoMipMapping_guess_t, 0x006E5238))(EQ_GLOBAL(int, 0x006B68B0), EQ_GLOBAL(int, 0x006B68B0));
    if (this->world_guess == 0)
    {
        this->world_guess = (*EQ_GLOBAL(t3dCreateWorld_guess_t, 0x006E51A8))(this->hwnd_guess, 0);
    }
    if (EQ_GLOBAL(int, 0x006E5480) == 1 && g_no_render_offset_guess == 1)
    {
        CDisplay_InitDDraw_obj_guess* boxDraw = new CDisplay_InitDDraw_obj_guess();
        result = this->unk_0x07C;
        this->boxDrawOwner_guess = boxDraw;
        EQ_METHOD(2, void, 0x0040296E, (void*, void*, int), boxDraw)(this->world_guess, result);
        EQ_METHOD(2, void, 0x00402C4E, (void*, int, int), this->boxDrawOwner_guess)(
            EQ_GLOBAL(int, 0x006B63FC), EQ_GLOBAL(int, 0x006B6400));
    }
}

// Stand-in until display.h declares LoadWorldFile (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::LoadWorldFile once
// the declaration is in the class.
struct CDisplay_LoadWorld_guess : CDisplay
{
    u8 LoadWorldFile(char* param_1, char* param_2, int param_3, int param_4, u8 param_5);
};

// FUNCTION: TRILOGY 0x00409b52
// Loads an .s3d/.wld pair into the 3D world; failures logged except for load/global _chr, _lit and _amr files.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Opens the .s3d/.wld pair through the fast-file loader. If the .s3d is not found and allowFallback is set, retries
// with qeynos. Returns 1 only when a world was found and read.
u8 CDisplay_LoadWorld_guess::LoadWorldFile(char* param_1, char* param_2, int unusedArg, int parseFlag, u8 allowFallback)
{
    typedef int (__cdecl *t3dFastFileInit_guess_t)(char*, int);
    typedef int (__cdecl *t3dParseReadWorldWithPalette_guess_t)(char*, void*, int, void*, int);
    typedef void (__cdecl *t3dFastFileFinish_guess_t)(int);

    /* fake match: the three path buffers share one 1280-byte struct, so the frame packs to the original's 0x508; VC6 rounds each separate odd-sized char array up to a dword. */
    struct
    {
        char local_50c[512];
        char local_30c[255];
        char acStack_20d[513];
    } bufs;
    char (&local_50c)[512] = bufs.local_50c;
    char (&local_30c)[255] = bufs.local_30c;
    char (&acStack_20d)[513] = bufs.acStack_20d;
    u8 success;
    int tmp;
    int iVar6;
    size_t len;
    size_t wldLen;
    t3dFastFileInit_guess_t fastFileInit;

    success = 1;
    DbgLog_guess((char*)0x0053dd0c, param_2);
    strcpy(local_50c, param_1);
    strcat(param_1, (char*)0x0053dd04);
    strcat(param_2, (char*)0x0053dcfc);
    if (EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), acStack_20d + 1, 0x200) == 0)
        DbgLog_guess((char*)0x0053dcd8);
    len = strlen(acStack_20d + 1);
    if (acStack_20d[len] != '\\')
        strcat(acStack_20d + 1, (char*)0x0053dcd4);
    strcat(acStack_20d + 1, param_1);
    DbgLog_guess((char*)0x0053dcb8, acStack_20d + 1);
    fastFileInit = EQ_GLOBAL(t3dFastFileInit_guess_t, 0x006E5244);
    tmp = fastFileInit(acStack_20d + 1, 0x14);
    if (tmp == 0)
    {
        tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dcac, 8);
        if (tmp != 0)
        {
            tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dca0, 11);
            if (tmp != 0)
            {
                tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc94, 11);
                if (tmp != 0)
                {
                    tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc88, 11);
                    if (tmp != 0)
                    {
                        tmp = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc80);
                        if (tmp == 0)
                        {
                            tmp = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc78);
                            if (tmp == 0)
                                DbgLog_guess((char*)0x0053dc50, acStack_20d + 1);
                        }
                    }
                }
            }
        }
        if (allowFallback != 0)
        {
            DbgLog_guess((char*)0x0053dc38);
            strcpy(param_1, (char*)0x0053dc2c);
            strcpy(param_2, (char*)0x0053dc20);
            EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), acStack_20d + 1, 0x100);
            len = strlen(acStack_20d + 1);
            if (acStack_20d[len] != '\\')
                strcat(acStack_20d + 1, (char*)0x0053dcd4);
            strcat(acStack_20d + 1, param_1);
            tmp = fastFileInit(acStack_20d + 1, 0x14);
            if (tmp == 0)
            {
                tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dcac, 8);
                if (tmp != 0)
                {
                    tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dca0, 11);
                    if (tmp != 0)
                    {
                        tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc94, 11);
                        if (tmp != 0)
                        {
                            tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc88, 11);
                            if (tmp != 0)
                                DbgLog_guess((char*)0x0053dc50, acStack_20d + 1);
                        }
                    }
                }
                this->palette_failed_guess = 1;
                return 0;
            }
        }
        else
        {
            tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dcac, 8);
            if (tmp != 0)
            {
                tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dca0, 11);
                if (tmp != 0)
                {
                    tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc94, 11);
                    if (tmp != 0)
                    {
                        tmp = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc88, 11);
                        if (tmp != 0)
                        {
                            tmp = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc80);
                            if (tmp == 0)
                            {
                                tmp = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc78);
                                if (tmp == 0)
                                    DbgLog_guess((char*)0x0053dc04, acStack_20d + 1);
                            }
                        }
                    }
                }
            }
            return 0;
        }
    }

    DbgLog_guess((char*)0x0053dbdc, this->world_guess, tmp, this->palette_guess, param_2);
    iVar6 = EQ_GLOBAL(t3dParseReadWorldWithPalette_guess_t, 0x006E5354)(param_2, this->world_guess, tmp, this->palette_guess, parseFlag);
    this->palette_failed_guess = iVar6;
    if (iVar6 != 0)
    {
        iVar6 = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dcac, 8);
        if (iVar6 != 0)
        {
            iVar6 = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dca0, 11);
            if (iVar6 != 0)
            {
                iVar6 = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc94, 11);
                if (iVar6 != 0)
                {
                    iVar6 = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dc88, 11);
                    if (iVar6 != 0)
                    {
                        iVar6 = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(param_1, (char*)0x0053dbd4, 7);
                        if (iVar6 != 0)
                        {
                            iVar6 = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc80);
                            if (iVar6 == 0)
                            {
                                iVar6 = EQ_CDECL(int, 0x00522A30, (const char*, const char*))(param_1, (char*)0x0053dc78);
                                if (iVar6 == 0)
                                {
                                    DbgLog_guess((char*)0x0053dbb4, param_2, this->palette_failed_guess);
                                    len = strlen(param_1);
                                    wldLen = strlen(param_2);
                                    if (wldLen == len)
                                    {
                                        sprintf(local_30c, (char*)0x0053dbac, local_50c);
                                        iVar6 = EQ_CDECL(int, 0x005223BC, (const char*, const char*))(local_30c, (char*)0x0053dba8);
                                        if (iVar6 != 0)
                                            EQ_CDECL(int, 0x00521FBB, (int))(iVar6);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        success = 0;
    }
    else
        DbgLog_guess((char*)0x0053db8c);
    if (tmp != 0)
        EQ_GLOBAL(t3dFastFileFinish_guess_t, 0x006E5240)(tmp);
    DbgLog_guess((char*)0x0053db74, success);
    return success;
}

// Stand-in until display.h declares OpenWorldBmpWadFiles_guess (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::OpenWorldBmpWadFiles_guess once
// the declaration is in the class.
struct CDisplay_OpenWorldBmpWad_guess : CDisplay
{
    bool OpenWorldBmpWadFiles_guess(void);
};

// FUNCTION: TRILOGY 0x00409FBA
// Loads world and interface bitmap archives for display assets.
// Opens the seven world BMP WAD archives in the current directory of the current drive through the fast-file loader
// and stores the handles at this+0x80, 0x84, 0x88, 0x8C, 0x90, 0x94 and 0x7C (bmpwadui). Only the first two handles
// are checked: a 0 there logs the error and returns 0. The directory's last character is path[strlen(path) - 1]
// (one byte before the buffer when the directory is empty).
// @early-stop: VC6 gives this=EDI, the 0x100 constant=ESI, the function pointer=EBX (original: this=EBX, 0x100=EDI, pointer=ESI).
bool CDisplay_OpenWorldBmpWad_guess::OpenWorldBmpWadFiles_guess(void)
{
    typedef int (__cdecl *t3dFastFileInit_guess_t)(char*, int);

    t3dFastFileInit_guess_t archiveOpener;
    u32 archiveHandle;
    size_t pathLength;
    int checkedHandle;
    char fileName[256];
    char path[256];

    strcpy(fileName, (char*)0x0053dda0);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveOpener = EQ_GLOBAL(t3dFastFileInit_guess_t, 0x006E5244);
    checkedHandle = archiveOpener(path, 0x14);
    *(int*)((char*)this + 0x80) = checkedHandle;
    if (checkedHandle == 0)
    {
        DbgLog_guess((char*)0x0053dd68, path);
        return 0;
    }
    strcpy(fileName, (char*)0x0053dd94);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    checkedHandle = archiveOpener(path, 0x14);
    this->fastFileHandle2_guess = checkedHandle;
    if (checkedHandle == 0)
    {
        DbgLog_guess((char*)0x0053dd68, path);
        return 0;
    }
    strcpy(fileName, (char*)0x0053dd5c);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveHandle = archiveOpener(path, 0x14);
    *(u32*)((char*)this + 0x88) = archiveHandle;
    strcpy(fileName, (char*)0x0053dd50);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveHandle = archiveOpener(path, 0x14);
    *(u32*)((char*)this + 0x8C) = archiveHandle;
    strcpy(fileName, (char*)0x0053dd44);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveHandle = archiveOpener(path, 0x14);
    *(u32*)((char*)this + 0x90) = archiveHandle;
    strcpy(fileName, (char*)0x0053dd38);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveHandle = archiveOpener(path, 0x14);
    *(u32*)((char*)this + 0x94) = archiveHandle;
    strcpy(fileName, (char*)0x0053dd28);
    EQ_CDECL(int, 0x00522AD7, (int, char*, int))(EQ_CDECL(int, 0x00522C0C, (void))(), path, 0x100);
    pathLength = strlen(path);
    if (path[pathLength - 1] != '\\')
        strcat(path, (char*)0x0053dcd4);
    strcat(path, fileName);
    archiveHandle = archiveOpener(path, 0x14);
    *(u32*)((char*)this + 0x7C) = archiveHandle;
    return 1;
}

// Stand-in until display.h declares StartWorldDisplay (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::StartWorldDisplay once
// the declaration is in the class.
struct CDisplay_StartWorldDisplay_guess : CDisplay
{
    void StartWorldDisplay(char* zone, int unused);
};

#define SWD_DBGLOG      DbgLog_guess
#define SOC_CPY      strcpy
#define SWD_STRCAT      strcat
#define SOC_FMT     sprintf
#define SWD_STRCMPI     EQ_CDECL(int, 0x00524240, (const char*, const char*))
#define EXEC_STRCMP      strcmp
#define SWD_LOADWORLD(a, b, c, d, e) \
    EQ_METHOD(5, char, 0x00409B52, (void*, char*, char*, int, int, int), this)(a, b, c, d, e)

// FUNCTION: TRILOGY 0x0040a340
// Brings up the zone's 3D world on zone entry: loads its world files, resets weather, sets up sun and camera.
// @early-stop: push edi lands before mov esi,ecx, two pop ecx move, and the zone name pointer is stored back to [ebp+8]
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Loads the zone's world (<zone>_obj, _2_obj, _chr, the zone itself, objects, lights, _lit), creates the sun and
// default lights, the name font and the camera/fog setup, all inside the 0x006EFA18 critical section.
// The second argument is popped (ret 8) but never read.
/* fake match: split volatile zone use to free EDI; *1.0f forces x87 loads from client constant addresses. */
void CDisplay_StartWorldDisplay_guess::StartWorldDisplay(char* volatile zone, int unused)
{
    typedef void* (__cdecl *t3dCreateRenderContext_guess_t)(int);
    typedef int (__cdecl *t3dRenderContextSet_guess_t)(void*, int);
    typedef int (__cdecl *t3dRenderContextSet3_guess_t)(void*, int, void*);
    typedef int (__cdecl *t3dDefineAI_guess_t)(void*, const char*, void*);
    typedef void (__cdecl *s3dVoid_guess_t)(void);
    typedef int (__cdecl *s3dParsePatchLighting_guess_t)(int, int);
    typedef void* (__cdecl *t3dCreateSphere_guess_t)(float);
    typedef void* (__cdecl *t3dDefineLight_guess_t)(void*, const char*, int, void*, int, int, int);
    typedef int (__cdecl *t3dSetLightDefinitionColorIntensityList_guess_t2)(void*, float*);
    typedef void* (__cdecl *t3dCreateDirectionalLight_guess_t)(void*, void*, float*);
    typedef int (__cdecl *t3dSetDirectionalLightFlag_guess_t)(void*);
    typedef int (__cdecl *t3dGetWorldNumRegions_guess_t)(void*);
    typedef int (__cdecl *t3dAddDirectionalLightToRegion_guess_t)(void*, void*, int);
    typedef void* (__cdecl *s3dCreateFontTexture_guess_t)(void*, const char*, const char*, int, int, int);
    typedef int (__cdecl *t3dSetWorldTime_guess_t)(void*, int);
    typedef int (__cdecl *s3dSetDynamicLOD_guess_t)(int, float, float);
    typedef int (__cdecl *t3dTranslateZones_guess_t)(void*);
    typedef void (__stdcall *CriticalSection_guess_t)(void*);

    t3dDefineAI_guess_t defineAI;
    s3dParsePatchLighting_guess_t parsePatchLighting;
    t3dDefineLight_guess_t defineLight;
    t3dGetWorldNumRegions_guess_t getNumRegions;
    char loaded;
    int tmp;
    int regionIndex;
    float intensity;
    float lightColor[3];
    float lightNormal[3];
    char wldName[64];
    char lightName[20];
    char s3dName[512];
    char s3dPath[512];
    char findData[280];
    char* setup_zone = zone;
    char* active_zone;

    palette_failed_guess = 0;
    EQ_GLOBAL(u8, 0x006E20F4) = 0;
    EQ_GLOBAL(u8, 0x006E1DE4) = 0;
    EQ_GLOBAL(u8, 0x006E1DE5) = 0;
    EQ_GLOBAL(u8, 0x006B640A) = 0;
    g_zone_id_guess =
        EQ_METHOD(1, int, 0x004C05AE, (void*, char*), g_pWorldTime_guess)(setup_zone);
    if (g_environment_ptr_guess != 0)
    {
        EQ_METHOD(1, void, 0x004E1F74, (void*, int), g_environment_ptr_guess)(-1);
        EQ_METHOD(1, void, 0x004E1F89, (void*, char*), g_environment_ptr_guess)(setup_zone);
    }
    EQ_GLOBAL(short, 0x006E1E0A) = -1;
    EQ_GLOBAL(int, 0x006D6FE0) = 0;
    EQ_GLOBAL(CriticalSection_guess_t, 0x00534194)((void*)0x006EFA18);
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053E008));
    if (EQ_GLOBAL(u8, 0x006B63F8) != 0 || DAT_006b6494 != 0)
    {
        if (g_local_player_guess != 0)
            SOC_CPY(g_local_player_guess->zone, setup_zone);
        if (g_pc_guess != 0)
            SOC_CPY(g_pc_guess->zone, setup_zone);
        EQ_CDECL(void, 0x004A31CF, (char*))(setup_zone);
    }
    this->skyLayerMode_guess = 1;
    *(u8*)((char*)this + 0x10) = 1;
    weather_active_guess = 0;
    weather_cc_guess = 0.0f;
    alwaysOpenDoors_guess = 0;
    weather_clouds_guess = 0;
    weather_d0_guess = 0.0f;
    weather_fade_guess = 0;
    unk_0x0C1 = 0;
    weather_type_guess = 0;
    this->rainActive = 0;
    this->snowActive_guess = 0;
    if (render_guess == 0)
    {
        render_guess = EQ_GLOBAL(t3dCreateRenderContext_guess_t, 0x006E51A0)(0x2a);
        EQ_METHOD(1, void, 0x00410D41, (void*, int), this)(0);
    }
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DFF0));
    g_cdisplay_guess->SetGammaCorrection_guess(
        EQ_GLOBAL(float, 0x00535154) * g_gammaLevel_guess +
        EQ_GLOBAL(float, 0x005344A8));
    EQ_GLOBAL(t3dRenderContextSet_guess_t, 0x006E5438)(render_guess, 1);
    CreateWorldPalette_guess(this->fastFileHandle_guess, EQ_ARRAY(char, 0x0053DA70));
    EQ_GLOBAL(t3dRenderContextSet_guess_t, 0x006E5440)(render_guess, 10);
    EQ_GLOBAL(t3dRenderContextSet3_guess_t, 0x006E543C)(render_guess, 1, palette_guess);
    EQ_GLOBAL(t3dRenderContextSet_guess_t, 0x006E5458)(world_guess, 1);
    defineAI = EQ_GLOBAL(t3dDefineAI_guess_t, 0x006E51F8);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DACC), (void*)0x00405818);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DABC), (void*)0x00405854);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DAAC), (void*)0x0040583d);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DA9C), (void*)0x004058f0);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DA8C), (void*)0x00405944);
    defineAI(world_guess, EQ_ARRAY(char, 0x0053DA7C), (void*)0x0040598b);
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DFD8));
    active_zone = zone;
    SOC_FMT(s3dName, EQ_ARRAY(char, 0x0053DFD0), active_zone);
    SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DFD0), active_zone);
    loaded = SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
    if (loaded == 0)
    {
        palette_failed_guess = 0;
        if (EQ_GLOBAL(u8, 0x006B63F8) == 0)
        {
            SWD_DBGLOG(EQ_ARRAY(char, 0x0053DFA0), s3dName);
            SOC_CPY(s3dName, EQ_ARRAY(char, 0x0053DF98));
            SOC_CPY(wldName, EQ_ARRAY(char, 0x0053DF98));
            SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
            SOC_CPY(s3dName, EQ_ARRAY(char, 0x0053DF8C));
            SOC_CPY(wldName, EQ_ARRAY(char, 0x0053DF8C));
            SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
        }
    }
    SOC_FMT(s3dName, EQ_ARRAY(char, 0x0053DF80), active_zone);
    SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DF80), active_zone);
    SOC_CPY(s3dPath, s3dName);
    SWD_STRCAT(s3dPath, EQ_ARRAY(char, 0x0053DD04));
    tmp = EQ_CDECL(int, 0x00522C4F, (char*, char*))(s3dPath, findData);
    if (tmp != -1)
    {
        loaded = SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
        if (loaded == 0)
            EQ_CDECL(int, 0x005229E8, (const char*, ...))(EQ_ARRAY(char, 0x0053DF6C), s3dPath);
    }
    if (SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF64)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF5C)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF50)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF4C)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF40)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF30)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF24)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF18)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF0C)) == 0 ||
        SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DF04)) == 0)
    {
        SOC_FMT(s3dName, EQ_ARRAY(char, 0x0053DEFC), active_zone);
        SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DEFC), active_zone);
        SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
    }
    SOC_FMT(s3dName, EQ_ARRAY(char, 0x0053DEF4), active_zone);
    SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DEF4), active_zone);
    SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
    SOC_CPY(wldName, active_zone);
    SOC_CPY(s3dName, active_zone);
    loaded = SWD_LOADWORLD(s3dName, wldName, 1, 0, 0);
    if (loaded == 0)
        palette_failed_guess = 1;
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DEDC));
    EQ_GLOBAL(s3dVoid_guess_t, 0x006E50F4)();
    if ((EXEC_STRCMP(active_zone, EQ_ARRAY(char, 0x0053DF50)) != 0 ||
         EQ_CDECL(int, 0x004A3D04, (const char*))(EQ_ARRAY(char, 0x0053DEC8)) == 0) &&
        palette_failed_guess == 0)
    {
        SOC_CPY(s3dName, active_zone);
        SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DEC0));
        SWD_LOADWORLD(s3dName, wldName, 0, 0, 1);
        palette_failed_guess = 0;
    }
    EQ_GLOBAL(s3dVoid_guess_t, 0x006E50E8)();
    if (palette_failed_guess == 0)
    {
        SOC_CPY(s3dName, active_zone);
        SOC_FMT(wldName, EQ_ARRAY(char, 0x0053DEB8));
        SWD_LOADWORLD(s3dName, wldName, 0, 0, 1);
        palette_failed_guess = 0;
    }
    SOC_CPY(s3dName, active_zone);
    SWD_STRCAT(s3dName, EQ_ARRAY(char, 0x0053DC80));
    SOC_CPY(wldName, active_zone);
    SWD_STRCAT(wldName, EQ_ARRAY(char, 0x0053DC80));
    parsePatchLighting = EQ_GLOBAL(s3dParsePatchLighting_guess_t, 0x006E5120);
    parsePatchLighting(1, 0);
    SWD_LOADWORLD(s3dName, wldName, 0, 0, 0);
    parsePatchLighting(0, 0);
    EQ_GLOBAL(float, 0x006B6410) = 0.8f;
    EQ_GLOBAL(float, 0x006B640C) = 0.625f;
    this->collisionSphereScale_guess = 1.0f;
    this->collisionSphere_guess =
        EQ_GLOBAL(t3dCreateSphere_guess_t, 0x006E51A4)(1.0f);
    this->cameraLocation_guess[0] = 0.0f;
    this->cameraLocation_guess[2] = 9.0f;
    this->cameraLocation_guess[1] = 0.0f;
    this->cameraLocation_guess[3] = 0.0f;
    this->cameraLocation_guess[4] = 0.0f;
    this->cameraLocation_guess[5] = 0.0f;
    tmp = t3dSetCameraLocation(world_guess, camera_guess, &this->cameraLocation_guess[0]);
    if (tmp != 0)
        SWD_DBGLOG(EQ_ARRAY(char, 0x0053DE88), tmp);
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DE70));
    SOC_CPY(lightName, EQ_ARRAY(char, 0x0053DE64));
    defineLight = EQ_GLOBAL(t3dDefineLight_guess_t, 0x006E5200);
    EQ_GLOBAL(float, 0x005900D4) = 1.0f;
    EQ_GLOBAL(float, 0x005900D8) = EQ_GLOBAL(float, 0x00535168) * 1.0f;
    EQ_GLOBAL(float, 0x005900DC) = 1.0f * EQ_GLOBAL(float, 0x00535164);
    EQ_GLOBAL(float, 0x005900E0) = EQ_GLOBAL(float, 0x00535164) * 1.0f;
    EQ_GLOBAL(float, 0x005900E4) = EQ_GLOBAL(float, 0x00535168) * 1.0f;
    EQ_GLOBAL(float, 0x005900E8) = 1.0f;
    this->light_def_default_guess =
        defineLight(world_guess, lightName, 1, EQ_GLOBAL(void*, 0x0053C6A8), 0, 500, 1);
    intensity = 1.0f;
    lightColor[0] = 1.0f;
    lightColor[1] = 1.0f;
    lightColor[2] = 1.0f;
    tmp = (int)defineLight(world_guess, EQ_ARRAY(char, 0x0053DE54), 1, &intensity, 0, 0x1e, 1);
    *(int*)((char*)this + 0x2DD8) = tmp;
    EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t2, 0x006E53F0)((void*)tmp, lightColor);
    tmp = (int)t3dCreateLight(world_guess, this->sunLightDef_guess);
    *(int*)((char*)this + 0x2DDC) = tmp;
    lightNormal[0] = -0.707f;
    lightNormal[1] = 0.0f;
    lightNormal[2] = -0.707f;
    tmp = (int)EQ_GLOBAL(t3dCreateDirectionalLight_guess_t, 0x006E5188)(world_guess, (void*)tmp, lightNormal);
    *(int*)((char*)this + 0x2DE0) = tmp;
    EQ_GLOBAL(t3dSetDirectionalLightFlag_guess_t, 0x006E53DC)((void*)tmp);
    EQ_GLOBAL(t3dSetDirectionalLightFlag_guess_t, 0x006E53D4)(this->sunDirectionalLight_guess);
    getNumRegions = EQ_GLOBAL(t3dGetWorldNumRegions_guess_t, 0x006E5308);
    regionIndex = 0;
    tmp = getNumRegions(world_guess);
    if (0 < tmp)
    {
        do
        {
            EQ_GLOBAL(t3dAddDirectionalLightToRegion_guess_t, 0x006E5158)(
                world_guess, this->sunDirectionalLight_guess, regionIndex);
            regionIndex = regionIndex + 1;
            tmp = getNumRegions(world_guess);
        } while (regionIndex < tmp);
    }
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DE3C));
    if (*(int*)((char*)this + 0x2DE4) == 0)
    {
        tmp = (int)EQ_GLOBAL(s3dCreateFontTexture_guess_t, 0x006E50D0)(
            world_guess, EQ_ARRAY(char, 0x0053DE30), EQ_ARRAY(char, 0x0053DA68), 0x14, 0, 0x80);
        *(int*)((char*)this + 0x2DE4) = tmp;
        if (tmp == 0)
            SWD_DBGLOG(EQ_ARRAY(char, 0x0053DDFC));
    }
    EQ_METHOD(0, void, 0x00408ED1, (void*), this)();
    EQ_GLOBAL(t3dRenderContextSet_guess_t, 0x006E5448)(render_guess, 1);
    EQ_GLOBAL(void*, 0x0058FC74) = render_guess;
    *(int*)((char*)this + 0x50) = 0;
    EQ_GLOBAL(void*, 0x0058FC70) = world_guess;
    EQ_GLOBAL(void*, 0x0058FC78) = camera_guess;
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DDE4));
    EQ_CDECL(void, 0x00489C5C, (void))();
    tmp = EQ_CDECL(int, 0x004A461E, (void))();
    EQ_GLOBAL(t3dSetWorldTime_guess_t, 0x006E545C)(world_guess, tmp);
    is3dOn_guess = 1;
    EQ_GLOBAL(s3dSetDynamicLOD_guess_t, 0x006E5138)(1, 1.0f, 100.0f);
    SetCameraYon_guess(800.0f);
    EQ_GLOBAL(CriticalSection_guess_t, 0x00534190)((void*)0x006EFA18);
    if (EQ_GLOBAL(int, 0x006EFE1C) == 0 && g_musicState_guess != 0)
        EQ_METHOD(1, void, 0x004E5B8B, (void*, char*), g_musicState_guess)(active_zone);
    this->worldDisplayActive = 1;
    if (SWD_STRCMPI(active_zone, EQ_ARRAY(char, 0x0053DDDC)) != 0)
        SOC_CPY(EQ_ARRAY(char, 0x006EE4F8), active_zone);
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DDC4));
    if (EQ_GLOBAL(int, 0x006B6884) != 0)
        TurnCharacterNameSpritesOn();
    if (EQ_GLOBAL(int, 0x006B6888) != 0)
        ShowNPCNameSprites_guess();
    TurnInfravisionEffectOff();
    EQ_GLOBAL(t3dTranslateZones_guess_t, 0x006E5468)(world_guess);
    EQ_METHOD(1, void, 0x00412AF2, (void*, char*), g_cdisplay_guess)(active_zone);
    EQ_GLOBAL(int, 0x006E20C4) = 1;
    SWD_DBGLOG(EQ_ARRAY(char, 0x0053DDAC), active_zone);
}

// The main overlay EnterZone creates at 0x006E4E38 (EverQuest.cpp); leaving the world only deletes it.
// Local: its destructor (0x0045143F, rewritten in EQ_Overlay.cpp as a free function) is all that is called here.
class EQ_ZoneMainOverlay_guess
{
public:
    // EXTERNAL: TRILOGY 0x0045143F EQ_ZoneMainOverlay_guess::~EQ_ZoneMainOverlay_guess
    ~EQ_ZoneMainOverlay_guess(void);
};

// display.h has no StopWorldDisplay declaration; this stand-in adds no fields.
struct CDisplay_StopWorldDisplay_guess : CDisplay
{
    void StopWorldDisplay();
};

// FUNCTION: TRILOGY 0x0040AC31
// Leaving a zone: kills rain/snow, deletes the zone's spawns, items, effects and missiles, frees its 3D world.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay_StopWorldDisplay_guess::StopWorldDisplay()
{
    typedef void (__stdcall *tCriticalSection_guess)(void*);
    typedef void (__cdecl *tDestroySphere_guess)(void*);
    typedef void (__cdecl *tDestroyRenderContext_guess)(void*, void*);
    typedef void (__cdecl *tDestroyPalette_guess)(void*, void*);
    typedef void (__cdecl *tUndefineBMInfo_guess)(void*, void*);
    typedef int (__cdecl *tDestroyFontTexture_guess)(void*, int);
    typedef void (__cdecl *tFlushWorld_guess)(void*);
    typedef void (__cdecl *tFreeMemory_guess)(void*);
    typedef void (__cdecl *tReinitializeDevice_guess)(void);
    int object;
    int fontTextureResult;
    tDestroyFontTexture_guess destroyFontTexture;

    if (this->worldDisplayActive != 0)
    {
        EQ_GLOBAL(tCriticalSection_guess, 0x00534194)((void*)0x006efa18);
        is3dOn_guess = 0;
        EQ_CDECL(void, 0x004A97C8, (int, void*))(0, (void*)0x0053e020);
        EQ_CDECL(void, 0x004A945B, (int, void*))(0, (void*)0x0053e020);
        if (this->collisionSphere_guess != 0)
        {
            EQ_GLOBAL(tDestroySphere_guess, 0x006E5224)(this->collisionSphere_guess);
            *(int*)((char*)this + 0x2cac) = 0;
        }
        while (object = EQ_GLOBAL(int, 0x006E4D70), object != 0)
        {
            EQ_METHOD(0, void, 0x004906B1, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D74), object != 0)
        {
            EQ_METHOD(0, void, 0x00490EF4, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D68), object != 0)
        {
            EQ_METHOD(0, void, 0x004A015F, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D80), object != 0)
        {
            EQ_METHOD(0, void, 0x004BD36F, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D58), object != 0)
        {
            EQ_METHOD(0, void, 0x004B0B9F, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D5C), object != 0)
        {
            EQ_METHOD(0, void, 0x004A0396, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D60), object != 0)
        {
            EQ_METHOD(0, void, 0x004AD9B0, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D64), object != 0)
        {
            EQ_METHOD(0, void, 0x004ACEAD, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D6C), object != 0)
        {
            EQ_METHOD(0, void, 0x004B02C9, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        while (object = EQ_GLOBAL(int, 0x006E4D78), object != 0)
        {
            EQ_METHOD(0, void, 0x004BDC26, (void*), (void*)object)();
            EQ_CDECL(void, 0x005218BB, (void*))((void*)object);
        }
        if (world_guess != 0)
        {
            if (render_guess != 0)
                EQ_GLOBAL(tDestroyRenderContext_guess, 0x006E5220)(world_guess, render_guess);
            render_guess = 0;
            if (palette_guess != 0)
                EQ_GLOBAL(tDestroyPalette_guess, 0x006E5214)(world_guess, palette_guess);
            palette_guess = 0;
            if (EQ_GLOBAL(EQ_ZoneMainOverlay_guess*, 0x006E4E38) != 0)
                delete EQ_GLOBAL(EQ_ZoneMainOverlay_guess*, 0x006E4E38);
            EQ_GLOBAL(EQ_ZoneMainOverlay_guess*, 0x006E4E38) = 0;
            if (*(int*)((char*)this + 0x50) != 0)
            {
                EQ_GLOBAL(tUndefineBMInfo_guess, 0x006E546C)(world_guess, this->bmInfo50_guess);
                *(int*)((char*)this + 0x50) = 0;
            }
            destroyFontTexture = EQ_GLOBAL(tDestroyFontTexture_guess, 0x006E50DC);
            fontTextureResult = destroyFontTexture(world_guess, *(int*)((char*)this + 0x2de4));
            if (fontTextureResult == 0)
                *(int*)((char*)this + 0x2de4) = 0;
            EQ_GLOBAL(tFlushWorld_guess, 0x006E525C)(world_guess);
            if (*(int*)((char*)this + 0x2de4) != 0)
            {
                if (*(u32*)((char*)world_guess + 0x2d0) > 0)
                    destroyFontTexture(world_guess, *(int*)((char*)this + 0x2de4));
                *(int*)((char*)this + 0x2de4) = 0;
            }
        }
        this->rainActive = 0;
        this->snowActive_guess = 0;
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C) = 0;
        EQ_GLOBAL(EQPlayer*, 0x006E4DAC) = 0;
        EQ_GLOBAL(int, 0x006E4DB0) = 0;
        EQ_GLOBAL(int, 0x006E4DB4) = 0;
        EQ_GLOBAL(int, 0x005917B0) = 0;
        EQ_GLOBAL(int, 0x00590160) = 0;
        EQ_GLOBAL(EQPlayer*, 0x006E4DA0) = 0;
        EQ_GLOBAL(int, 0x006E4DC8) = 0;
        EQ_GLOBAL(int, 0x006E4DA4) = 0;
        EQ_GLOBAL(int, 0x006E4DB8) = 0;
        EQ_GLOBAL(int, 0x006E4DBC) = 0;
        EQ_GLOBAL(int, 0x006E4DC0) = 0;
        EQ_GLOBAL(int, 0x006E4DC4) = 0;
        light_defs_guess[0] = 0;
        light_defs_guess[1] = 0;
        light_defs_guess[2] = 0;
        light_defs_guess[3] = 0;
        light_defs_guess[4] = 0;
        light_defs_guess[5] = 0;
        light_defs_guess[6] = 0;
        light_defs_guess[7] = 0;
        light_defs_guess[8] = 0;
        *(int*)((char*)this + 0x2dd8) = 0;
        *(int*)((char*)this + 0x2ddc) = 0;
        *(int*)((char*)this + 0x2de0) = 0;
        light_def_default_guess = 0;
        *(int*)((char*)this + 0x2844) = 0;
        EQ_CDECL(void*, 0x00521960, (void*, int, size_t))((char*)this + 0x130, 0, 10000);
        *(u16*)((char*)this + 0x2840) = 0;
        if (*(int*)((char*)this + 0x12c) != 0)
        {
            EQ_GLOBAL(tFreeMemory_guess, 0x006E5264)((void*)*(int*)((char*)this + 0x12c));
            *(int*)((char*)this + 0x12c) = 0;
        }
        unk_0x124 = 0;
        *(int*)((char*)this + 0x2888) = 0;
        EQ_GLOBAL(tCriticalSection_guess, 0x00534190)((void*)0x006efa18);
        EQ_GLOBAL(tReinitializeDevice_guess, 0x006E5358)();
        this->worldDisplayActive = 0;
    }
}

// FUNCTION: TRILOGY 0x0040af4f
// Applies the selected gamma level to the display.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetGammaCorrection_guess(float gamma)
{
    t3dSetGammaLevel(gamma);
}

// FUNCTION: TRILOGY 0x0040af61
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored. Level 0..3 -> 0, 0.33, 0.66, 1; anything else 1.
void CDisplay::SetPCloudDensityMultiplier_guess(int level)
{
    float multiplier = 1.0f;

    switch (level)
    {
    case 0:
        multiplier = 0.0f;
        break;
    case 1:
        multiplier = 0.33f;
        break;
    case 2:
        multiplier = 0.66f;
        break;
    case 3:
        multiplier = 1.0f;
        break;
    }
    t3dSetPCloudDensityMultiplier(multiplier);
}

// CDisplay::LoadOverlay has no declaration in display.h. This file-local stand-in
// keeps its thiscall layout without changing the shared class declaration.
struct CDisplay_guess
{
    u8 unknown0x0000[0xbc];
    u32 time_ms_guess;
    u8 unknown0x00c0[0x2bd4];
    int is3dOn_guess;

    int LoadOverlay(int unused, char* fileName, int unused2);
    void UpdatePlayerVisibility_guess(EQPlayer* player);
    void SetFog(int mode, float start, float end, char red, char green, char blue);
    void ToggleView(void);
    char* FindNearestVisibleTarget_guess(float maxDistance, char param_3);
};

typedef int (__cdecl *t3dDefineBMInfo_LoadOverlay_guess_t)(void*, char*, char*, u32);
typedef int (__cdecl *t3dOverlayOpen_LoadOverlay_guess_t)(void*, u32, int);

// FUNCTION: TRILOGY 0x0040b188
// Loads a 2D overlay bitmap (load screens, character creation, main menu art); a failed load exits the game.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: callee-save placement and EBX exit-argument materialization remain
int CDisplay_guess::LoadOverlay(int unused, char* fileName, int unused2)
{
    int result;
    t3dDefineBMInfo_LoadOverlay_guess_t defineBMInfo;

    if (*((int*)((char*)this + 0x4)) == 0)
        return 0;
    if (*((int*)((char*)this + 0x2c98)) == 0)
        return 0;

    // A define that succeeds breaks out of the search; the overlay open after the loop is the shared exit.
    do
    {
        result = EXEC_STRCMP(fileName, EQ_ARRAY(char, 0x0053E0D4));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x84)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E0C8));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x8c)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E0BC));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x90)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E0AC));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E098));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E088));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x94)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E074));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x94)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E068));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E05C));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E050));
        if (result == 0)
        {
            result = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC)(
                *((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result == 0)
                EQ_STDCALL(void, 0x00522DAD, (int))(1);
            else
                break;
        }

        defineBMInfo = EQ_GLOBAL(t3dDefineBMInfo_LoadOverlay_guess_t, 0x006E51FC);
        if (*((int*)((char*)this + 0x88)) != 0 &&
            ((result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E044)), result == 0) ||
             (result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E038)), result == 0) ||
             (result = SWD_STRCMPI(fileName, EQ_ARRAY(char, 0x0053E028)), result == 0)))
        {
            result = defineBMInfo(*((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x88)));
            *((int*)((char*)this + 0x2cd0)) = result;
            if (result != 0)
                break;
        }

        result = defineBMInfo(*((void**)((char*)this + 0x4)), fileName, fileName, *((u32*)((char*)this + 0x80)));
        *((int*)((char*)this + 0x2cd0)) = result;
        if (result == 0)
            EQ_STDCALL(void, 0x00522DAD, (int))(1);
    } while (0);

    result = EQ_GLOBAL(t3dOverlayOpen_LoadOverlay_guess_t, 0x006E534C)(
        *((void**)((char*)this + 0x4)), **((u32**)((char*)this + 0x2cd0)), 1);
    if (result == 0)
    {
        EQ_STDCALL(void, 0x00522DAD, (int))(1);
        return 0;
    }
    return result;
}

typedef void (__cdecl *t3dDestroy2_FUN_0040afa1_t)(int, int);
typedef void (__cdecl *t3dDestroy1_FUN_0040afa1_t)(int);
typedef void (__cdecl *t3dDestroy0_FUN_0040afa1_t)(void);

// FUNCTION: TRILOGY 0x0040afa1
// Shuts down the active game display and releases its world and visual resources.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Tears down the 3D world: camera (+8), render context (+0), palette (+0x2c90), world (+4), the seven fonts,
// the device, the player globals, then clears the state fields and frees the buffer at +0x12c.
void __fastcall ShutdownDisplay_guess(char* thisPtr)
{
    int camera = *(int*)(thisPtr + 0x8);
    t3dDestroy1_FUN_0040afa1_t destroyFont;

    *(int*)(thisPtr + 0x2c94) = 0;
    *(int*)(thisPtr + 0x2c98) = 0;
    if (camera != 0)
        EQ_GLOBAL(t3dDestroy2_FUN_0040afa1_t, 0x006E520C)(camera, 1);
    *(int*)(thisPtr + 0x8) = 0;
    if (*(int*)(thisPtr + 0x4) != 0)
    {
        if (*(int*)thisPtr != 0)
            EQ_GLOBAL(t3dDestroy2_FUN_0040afa1_t, 0x006E5220)(*(int*)(thisPtr + 0x4), *(int*)thisPtr);
        *(int*)thisPtr = 0;
        if (*(int*)(thisPtr + 0x2c90) != 0)
            EQ_GLOBAL(t3dDestroy2_FUN_0040afa1_t, 0x006E5214)(*(int*)(thisPtr + 0x4), *(int*)(thisPtr + 0x2c90));
        *(int*)(thisPtr + 0x2c90) = 0;
        EQ_METHOD(0, void, 0x004DB2EB, (void*), EQ_GLOBAL(void*, 0x006EFE2C))();
        EQ_GLOBAL(t3dDestroy1_FUN_0040afa1_t, 0x006E522C)(*(int*)(thisPtr + 0x4));
        *(int*)(thisPtr + 0x4) = 0;
    }
    destroyFont = EQ_GLOBAL(t3dDestroy1_FUN_0040afa1_t, 0x006E51DC);
    if (g_arialFont6_guess != 0)
    {
        destroyFont(g_arialFont6_guess);
        g_arialFont6_guess = 0;
    }
    if (g_arialFont7_guess != 0)
    {
        destroyFont(g_arialFont7_guess);
        g_arialFont7_guess = 0;
    }
    if (g_arialFont8_guess != 0)
    {
        destroyFont(g_arialFont8_guess);
        g_arialFont8_guess = 0;
    }
    if (g_arialFont9_guess != 0)
    {
        destroyFont(g_arialFont9_guess);
        g_arialFont9_guess = 0;
    }
    if (g_arialFont10_guess != 0)
    {
        destroyFont(g_arialFont10_guess);
        g_arialFont10_guess = 0;
    }
    if (g_arialFont12_guess != 0)
    {
        destroyFont(g_arialFont12_guess);
        g_arialFont12_guess = 0;
    }
    if (g_arialFont14_guess != 0)
    {
        destroyFont(g_arialFont14_guess);
        g_arialFont14_guess = 0;
    }
    EQ_GLOBAL(t3dDestroy0_FUN_0040afa1_t, 0x006E535C)();
    g_local_player_guess = 0;
    EQ_GLOBAL(int, 0x006E4DAC) = 0;
    EQ_GLOBAL(int, 0x006E4DB0) = 0;
    EQ_GLOBAL(int, 0x006E4DB4) = 0;
    EQ_GLOBAL(int, 0x005917B0) = 0;
    EQ_GLOBAL(int, 0x00590160) = 0;
    g_control_player_guess = 0;
    EQ_GLOBAL(int, 0x006E4DC8) = 0;
    EQ_GLOBAL(int, 0x006E4DA4) = 0;
    EQ_GLOBAL(int, 0x006E4DB8) = 0;
    EQ_GLOBAL(int, 0x006E4DBC) = 0;
    EQ_GLOBAL(int, 0x006E4DC0) = 0;
    EQ_GLOBAL(int, 0x006E4DC4) = 0;
    *(int*)(thisPtr + 0x2848) = 0;
    *(int*)(thisPtr + 0x284c) = 0;
    *(int*)(thisPtr + 0x2850) = 0;
    *(int*)(thisPtr + 0x2854) = 0;
    *(int*)(thisPtr + 0x2858) = 0;
    *(int*)(thisPtr + 0x285c) = 0;
    *(int*)(thisPtr + 0x2860) = 0;
    *(int*)(thisPtr + 0x2cb4) = 0;
    *(int*)(thisPtr + 0x2844) = 0;
    memset(thisPtr + 0x130, 0, 10000);
    *(short*)(thisPtr + 0x2840) = 0;
    if (*(int*)(thisPtr + 0x12c) != 0)
    {
        EQ_GLOBAL(t3dDestroy1_FUN_0040afa1_t, 0x006E5264)(*(int*)(thisPtr + 0x12c));
        *(int*)(thisPtr + 0x12c) = 0;
    }
    *(int*)(thisPtr + 0x124) = 0;
    *(int*)(thisPtr + 0x2888) = 0;
}

// FUNCTION: TRILOGY 0x0040b173
// Removes an overlay from the display.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::CloseOverlay_guess(void* overlay)
{
    if (overlay)
        t3dOverlayDestroy(overlay);
}

// FUNCTION: TRILOGY 0x0040b492
// Displays an overlay at its screen position, adjusted for the active game screen mode.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored. Nothing is drawn while the controlled spawn's actor byte +0x189 is set. In game states
// 1, 5 and 6 the position is moved by the video mode's offset (separate ifs, as in the original).
void CDisplay::DrawOverlay_guess(void* overlay, int overlayParam, int* pos, int unused)
{
    int x = pos[0];
    int y = pos[1];

    if (g_control_player_guess != 0 && g_control_player_guess->actor_info != 0
        && g_control_player_guess->actor_info->unknown0x189 != 0)
        return;
    if (g_game_state_guess == 1 || g_game_state_guess == 5 || g_game_state_guess == 6)
    {
        if (g_video_mode_guess == 1)
        {
            x += 0x50;
            y += 0x3C;
        }
        if (g_video_mode_guess == 2)
        {
            x += 0xC0;
            y += 0x90;
        }
        if (g_video_mode_guess == 3)
        {
            x += 0x140;
            y += 0x110;
        }
        if (g_video_mode_guess == 4)
        {
            x += 0x1E0;
            y += 0x168;
        }
    }
    t3dOverlayApply(overlay, x, y, overlayParam);
}

// FUNCTION: TRILOGY 0x0040b521
// Refreshes the character's visible item models after an equipment slot changes.
void __fastcall CDisplay_UpdateEquipmentDisplay_guess(CDisplay* self, int unusedEdx,
                                                        EQPlayer* player, short slot)
{
    char slot11Model[64];
    char primaryModel[64];
    char slot14Model[64];
    int hasSlot13Item = 0;
    int hasSlot14Item = 0;
    int hasSlot11Item = 0;
    char* selectedModel;

    SOC_CPY(primaryModel, (const char*)0x0053D860);
    SOC_CPY(slot14Model, (const char*)0x0053D860);
    SOC_CPY(slot11Model, (const char*)0x0053D860);
    if (g_pc_guess->inventory_items[0xD] != 0)
    {
        SOC_CPY
            (primaryModel, g_pc_guess->inventory_items[0xD]->idfile);
        hasSlot13Item = 1;
    }
    if (g_pc_guess->inventory_items[0xE] != 0)
    {
        SOC_CPY
            (slot14Model, g_pc_guess->inventory_items[0xE]->idfile);
        hasSlot14Item = 1;
    }
    if (g_pc_guess->inventory_items[0xB] != 0)
    {
        SOC_CPY
            (slot11Model, g_pc_guess->inventory_items[0xB]->idfile);
        hasSlot11Item = 1;
    }
    if (slot == 0xD)
    {
        if (player->actor_info->animationId_guess == 9)
            return;
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 7, primaryModel);
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 8, slot14Model);
        if (hasSlot13Item != 0)
            return;
        if (hasSlot14Item != 0)
            return;
        if (hasSlot11Item == 0)
            return;
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 8, slot11Model);
    }
    if (slot == 0xE)
    {
        if (player->actor_info->animationId_guess == 9)
            return;
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 7, primaryModel);
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 8, slot14Model);
        if (hasSlot13Item != 0)
            return;
        if (hasSlot14Item != 0)
            return;
        if (hasSlot11Item == 0)
            return;
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 8, slot11Model);
    }
    if (slot == 0xB)
    {
        if (hasSlot11Item != 0)
        {
            EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 7, (char*)0x0053D860);
            selectedModel = slot11Model;
        }
        else
        {
            EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 7, primaryModel);
            selectedModel = slot14Model;
        }
        EQ_METHOD(3, void, 0x0040B6E0, (CDisplay*, EQPlayer*, int, char*), self)(player, 8, selectedModel);
    }
    return;
}

// FUNCTION: TRILOGY 0x0040b6e0
// Handles the displayed item model for a player's selected equipment slot.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::UpdateItemSlot_guess(EQPlayer* player, int slot, char* itemName)
{
    CDisplay_UpdateItemSlot4_guess(this, player, slot, itemName, 0);
}

// FUNCTION: TRILOGY 0x0040b6f6
// Shows an item model (IT<n>) in a spawn's head or hand slot; sends the local player's change to the server.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: 11 dummy-EDX xors from EQ_THISCALL; substring copy: VC6 adds a zero-count test before each rep and re-derives esi/edi (original runs the reps straight through)
u8 __fastcall UpdateItemSlot_impl_guess(CDisplay* display, int edx, EQPlayer* spawn, u8 param_2, char* itemName, int suppressSend)
{
    int* actorInfo;
    EQPlayer* pEVar2;
    int temp;
    size_t nameLen;
    int cmpResult;
    int typeCategory;
    u32 copyLen;
    u32 remaining;
    u32* src;
    u32* dst;
    bool bVar11;
    u8 result;
    char modelTag[32];
    char numberText[128];
    char dagName[32];
    u8 local_message[16];
    int useAltDag;
    int useIt159;
    int modelNumber;

    pEVar2 = spawn;
    if (((param_2 != '\a' && param_2 != '\b') && param_2 != '\0') ||
        (spawn->race == 0xe || spawn == 0 ||
         ((actorInfo = (int*)spawn->actor_info, actorInfo == 0 ||
           (*actorInfo == 0 || actorInfo[0x94] == 0)))))
    {
        return 0;
    }
    result = 0;
    if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        switch (param_2)
        {
        case '\a':
            *((u8*)actorInfo + 0x89) = 0xd;
            break;
        case '\b':
            *((u8*)actorInfo + 0x89) = 0xb;
            break;
        }
    }
    SOC_CPY(dagName, itemName);
    numberText[0] = 0;
    temp = EXEC_STRCMP(dagName, EQ_ARRAY(char, 0x0053E148));
    if (temp == 0 && *((char*)spawn->actor_info + 0x99) != '\0')
        SOC_CPY(dagName, EQ_ARRAY(char, 0x0053E140));
    EQ_METHOD(1, void, 0x004B156C, (void*, char*), spawn)(modelTag);
    nameLen = strlen(dagName);
    if (2 < (int)nameLen)
    {
        copyLen = nameLen - 2;
        src = (u32*)(dagName + 2);
        dst = (u32*)numberText;
        for (remaining = copyLen >> 2; remaining != 0; remaining = remaining - 1)
        {
            *dst = *src;
            src = src + 1;
            dst = dst + 1;
        }
        for (remaining = copyLen & 3; remaining != 0; remaining = remaining - 1)
        {
            *(u8*)dst = *(u8*)src;
            src = (u32*)((u32)src + 1);
            dst = (u32*)((u32)dst + 1);
        }
        *((u8*)numberText + copyLen) = 0;
    }
    temp = EQ_CDECL(int, 0x004A47C8, (const char*))(numberText);
    if (temp != 0)
        modelNumber = atoi(numberText);
    else
        modelNumber = 0;
    useIt159 = 0;
    if (modelNumber > 199 && modelNumber < 300 && param_2 == '\b')
    {
        temp = *((int*)spawn->actor_info + 0x264 / 4);
        if (temp != 0)
            EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, 0);
        useAltDag = 1;
        temp = *((int*)spawn->actor_info + 0x268 / 4);
    }
    else
    {
        switch (param_2)
        {
        case 0:
            temp = *((int*)spawn->actor_info + 600 / 4);
            break;
        case '\a':
            temp = *((int*)spawn->actor_info + 0x260 / 4);
            break;
        case '\b':
            temp = *((int*)spawn->actor_info + 0x268 / 4);
            if (temp != 0)
                EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, 0);
            temp = *((int*)spawn->actor_info + 0x264 / 4);
            break;
        default:
            return 0;
        }
        useAltDag = 0;
    }
    if (temp != 0)
    {
        switch (param_2)
        {
        case 0:
            EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, dagName);
            spawn->equipment[0] = (u8)modelNumber;
            break;
        case '\a':
            if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C) && modelNumber == 0 &&
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0xc] != 0 &&
                (cmpResult = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0xc]->idfile,
                    EQ_ARRAY(char, 0x0053E0E4), 5), cmpResult == 0))
            {
                EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, EQ_ARRAY(char, 0x0053E0E4));
                useIt159 = 1;
            }
            typeCategory = EQ_METHOD(1, int, 0x0040BA90, (void*, int), display)(modelNumber);
            bVar11 = useIt159 == 0;
            *((int*)spawn->actor_info + 0x234 / 4) = typeCategory;
            *((int*)spawn->actor_info + 0x23c / 4) = modelNumber;
            spawn->equipment[7] = (u8)modelNumber;
            if (bVar11)
                EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, dagName);
            break;
        case '\b':
            if (spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C) && (modelNumber == 0 || useAltDag == 1) &&
                EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0xc] != 0 &&
                (cmpResult = EQ_CDECL(int, 0x005316C0, (const char*, const char*, size_t))(
                    EQ_GLOBAL(CharacterProfile*, 0x006E4DA8)->inventory_items[0xc]->idfile,
                    EQ_ARRAY(char, 0x0053E0E4), 5), cmpResult == 0))
            {
                EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(*((int*)spawn->actor_info + 0x264 / 4), EQ_ARRAY(char, 0x0053E0E4));
                useIt159 = 1;
            }
            typeCategory = EQ_METHOD(1, int, 0x0040BA90, (void*, int), display)(modelNumber);
            *((int*)spawn->actor_info + 0x238 / 4) = typeCategory;
            *((int*)spawn->actor_info + 0x240 / 4) = modelNumber;
            spawn->equipment[8] = (u8)modelNumber;
            if (useIt159 == 0 || useAltDag == 1)
                EQ_METHOD(2, void, 0x004119A6, (void*, int, const char*), display)(temp, dagName);
            break;
        default:
            EQ_CDECL(int, 0x005229E8, (const char*, int))(EQ_ARRAY(char, 0x0053E0EC), param_2);
            return 0;
        }
        bVar11 = spawn == EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
        result = 1;
        if (bVar11 &&
            (EQ_METHOD(1, void, 0x004B23E1, (void*, const char*), spawn)(itemName), suppressSend == 0) &&
            EQ_GLOBAL(u8, 0x006B63F8) == 0)
        {
            *((u32*)local_message) = pEVar2->spawn_id;
            local_message[4] = param_2;
            if (useIt159 == 1)
                local_message[5] = 0x9f;
            else
                local_message[5] = (u8)modelNumber;
            *((u32*)(local_message + 8)) = 0;
            local_message[12] = pEVar2->weaponAnimationType;
            send_message_guess(
                &EQ_GLOBAL(void*, 0x006B3194), 0x2092, local_message, 0x10, 1);
        }
    }
    return result;
}

// FUNCTION: TRILOGY 0x0040ba90
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored. 0 for type 0, 4 for type 4, 5 for 61, 3 for 200..300, 2 for the listed types, else 1.
int CDisplay::GetItemTypeCategory_guess(int type)
{
    int category = 1;

    switch (type)
    {
    case 1: case 2: case 3: case 5: case 7: case 9:
    case 16: case 17: case 19: case 20: case 23: case 24: case 25: case 30:
    case 34: case 35: case 37: case 39: case 40: case 41: case 42: case 43: case 44:
    case 50: case 51: case 53: case 57: case 58: case 59: case 60: case 62:
        category = 2;
        break;
    }
    if (type == 4)
        category = 4;
    if (type == 61)
        category = 5;
    if (type >= 200 && type <= 300)
        category = 3;
    if (type == 0)
        category = 0;
    return category;
}

// FUNCTION: TRILOGY 0x0040bb3f
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Out-of-range indices read slot 0. Returns a COLORREF (r | g << 8 | b << 16).
u32 CDisplay::GetUserDefinedColor(int index)
{
    if (index > (NUM_USER_COLORS_GUESS - 1) || index < 0)
        index = 0;
    return (u32)g_userDefinedColors_guess[index].r
         | ((u32)g_userDefinedColors_guess[index].g << 8)
         | ((u32)g_userDefinedColors_guess[index].b << 16);
}

// FUNCTION: TRILOGY 0x0040bb6c
// Provides an indexed default RGB text color and saves it to eqclient.ini.
// @early-stop: "%d" CSE'd into EBX and the writer spilled to [ebp+8]; original: immediate format, writer in ESI.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
/* fake match: green/blue index the 0-based palette with oneBasedIndex - 1, and the writer is loaded before the value sprintf, to steer VC6's register choice (see @early-stop). */
void GetDefaultUserDefinedColor(int index, u32* red, u32* green, u32* blue)
{
    char key[32];
    char keyPrefix[16];
    char value[16];
    int oneBasedIndex;

    if (index > 0x21 || index < 0)
        index = 0;

    *red = ((UserColor_guess*)0x0053c5e8)[index].r;
    oneBasedIndex = index + 1;
    *green = ((UserColor_guess*)0x0053c5e8)[oneBasedIndex - 1].g;
    *blue = ((UserColor_guess*)0x0053c5e8)[oneBasedIndex - 1].b;

    sprintf(keyPrefix, EQ_ARRAY(char, 0x0053E174), oneBasedIndex);
    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x0053E170));
    WritePrivateProfileStringA_guess_t writeProfileString = EQ_GLOBAL(WritePrivateProfileStringA_guess_t, 0x00534188);
    sprintf(value, EQ_ARRAY(char, 0x0053E16C), *red);
    writeProfileString(EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));

    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x0053E158));
    sprintf(value, EQ_ARRAY(char, 0x0053E16C), *green);
    writeProfileString(EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));

    strcpy(key, keyPrefix);
    strcat(key, EQ_ARRAY(char, 0x0053E150));
    sprintf(value, EQ_ARRAY(char, 0x0053E16C), *blue);
    writeProfileString(EQ_ARRAY(char, 0x0053E160), key, value, EQ_ARRAY(char, 0x0053DB50));
}

// FUNCTION: TRILOGY 0x0040bc8c
// Changes the RGB value of one indexed user-defined color.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetUserDefinedColor(int index, u8 r, u8 g, u8 b)
{
    if (index <= (NUM_USER_COLORS_GUESS - 1) && index >= 0)
    {
        index *= sizeof(UserColor_guess);
        EQ_ARRAY(u8, 0x0058FBBC)[index] = r;
        EQ_ARRAY(u8, 0x0058FBBD)[index] = g;
        EQ_ARRAY(u8, 0x0058FBBE)[index] = b;
    }
}

// FUNCTION: TRILOGY 0x0040bcbb
// Loads custom text colors from eqclient.ini, using built-in values where entries are missing.
// @early-stop: default block laid out after the reads (original: first); 0x100 hoisted into EBX, import pointer folded into the call
void loadUserDefinedColors_guess(void)
{
    typedef u32 (__stdcall *GetPrivateProfileStringA_guess_t)(const char*, const char*, const char*, char*, u32, const char*);

    char valueText[256];
    char keyPrefix[16];
    char keyName[32];
    int userNumber;
    int blueValue;
    int greenValue;
    int redValue;
    int colorIndex;

    colorIndex = 0;
    do
    {
        userNumber = colorIndex + 1;
        SOC_FMT
            (keyPrefix, (const char*)0x0053e174, userNumber);
        SOC_CPY(keyName, keyPrefix);
        SWD_STRCAT
            (keyName, (const char*)0x0053e170);
        EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C)
            ((const char*)0x0053e160, keyName, (const char*)0x0053db48,
             valueText, 0x100, (const char*)0x0053db50);
        // A '*' read skips the later reads; the buffer still holds that '*' for the check below.
        if (valueText[0] != '*')
        {
            redValue = atoi(valueText);
            SOC_CPY(keyName, keyPrefix);
            SWD_STRCAT
                (keyName, (const char*)0x0053e158);
            EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C)
                ((const char*)0x0053e160, keyName, (const char*)0x0053db48,
                 valueText, 0x100, (const char*)0x0053db50);
            if (valueText[0] != '*')
            {
                greenValue = atoi(valueText);
                SOC_CPY(keyName, keyPrefix);
                SWD_STRCAT
                    (keyName, (const char*)0x0053e150);
                EQ_GLOBAL(GetPrivateProfileStringA_guess_t, 0x0053418C)
                    ((const char*)0x0053e160, keyName, (const char*)0x0053db48,
                     valueText, 0x100, (const char*)0x0053db50);
                if (valueText[0] != '*')
                    blueValue = atoi(valueText);
            }
        }
        if (valueText[0] == '*')
        {
            EQ_CDECL(void, 0x0040BB6C, (int, int*, int*, int*))
                (colorIndex, &redValue, &greenValue, &blueValue);
        }
        EQ_CDECL(void, 0x0040BC8C, (int, u8, u8, u8))
            (colorIndex, redValue, greenValue, blueValue);
        colorIndex = userNumber;
    } while (userNumber + 1 < 0x23);
}

// FUNCTION: TRILOGY 0x0040BE28
// Draws a string on screen at x,y in a given font and colour, through GDI or through the renderer's text queue
// @early-stop: register picks differ (original pHVar1=EBX, sVar6=EDI, fn pointer=ESI); low colour byte is movzx there.
/* fake match: pSelectObject keeps the SelectObject import pointer in a register, as VC6 does for the original's two calls. */
short DrawScreenText_guess(HDC hdc, char* text, int x, int y, int colorIndex, HGDIOBJ font)
{
    struct TextMetricsA_guess
    {
        int tmHeight;
        int tmAscent;
        int tmDescent;
        int tmInternalLeading;
        int tmExternalLeading;
        int tmAveCharWidth;
        int tmMaxCharWidth;
        int tmWeight;
        int tmOverhang;
        int tmDigitizedAspectX;
        int tmDigitizedAspectY;
        char unknownTail[12];
    };

    typedef HGDIOBJ (__stdcall *SelectObject_guess_t)(HDC, HGDIOBJ);
    typedef COLORREF (__stdcall *SetBkColor_guess_t)(HDC, COLORREF);
    typedef COLORREF (__stdcall *SetTextColor_guess_t)(HDC, COLORREF);
    typedef BOOL (__stdcall *TextOutA_guess_t)(HDC, int, int, const char*, int);
    typedef BOOL (__stdcall *GetTextMetricsA_guess_t)(HDC, TextMetricsA_guess*);
    typedef void (__stdcall *DisplayTextCallback_0040be28_t)(void*, HDC*);
    typedef void (__stdcall *DisplayTextCallbackFinish_0040be28_t)(void*, HDC);
    typedef void (__cdecl *DeferTextA_0040be28_t)(char*, u32, float*, u32, int);

    HDC surfaceOwner;
    SelectObject_guess_t pSelectObject;
    COLORREF color;
    size_t textLen;
    u32 argbColor;
    u32 fontId;
    short result;
    TextMetricsA_guess metrics;
    float localTextPosition[3];

    // returns 1 if the text is queued, else its estimated width
    result = 1;
    if (g_textDeferred_guess == 0)
    {
        surfaceOwner = hdc;
        // surface at +0x0c: vtable +0x44 GetDC (fills hdc), +0x68 ReleaseDC
        ((DisplayTextCallback_0040be28_t)(*(void***)*(void**)((char*)surfaceOwner + 0x0c))[0x11])(
            *(void**)((char*)surfaceOwner + 0x0c), &hdc);
        pSelectObject = EQ_GLOBAL(SelectObject_guess_t, 0x00534024);
        // font now holds the previous font, restored below
        font = pSelectObject(hdc, font);
        EQ_GLOBAL(SetBkColor_guess_t, 0x00534028)(hdc, 0x00ff00ff);
        // > 0xff: user colour slot (index + 0x100)
        if (colorIndex > 0xff)
        {
            color = EQ_CDECL(u32, 0x0040BB3F, (int))(colorIndex - 0x100);
            // state 2: black text becomes light grey
            if ((g_game_state_guess == 2) && (color == 0))
                color = 0x00f0f0f0;
        }
        else
        {
            // 0x01000000 = PALETTEINDEX
            color = (colorIndex & 0xffff) | 0x01000000;
        }
        EQ_GLOBAL(SetTextColor_guess_t, 0x0053402C)(hdc, color);
        textLen = strlen(text);
        EQ_GLOBAL(TextOutA_guess_t, 0x00534030)(hdc, x, y, text, textLen);
        EQ_GLOBAL(GetTextMetricsA_guess_t, 0x00534018)(hdc, &metrics);
        textLen = strlen(text);
        // estimated width: chars * (overhang + 5 + avg width) + 16
        result = (short)textLen * ((short)metrics.tmOverhang + 5 + (short)metrics.tmAveCharWidth) + 0x10;
        pSelectObject(hdc, font);
        ((DisplayTextCallbackFinish_0040be28_t)(*(void***)*(void**)((char*)surfaceOwner + 0x0c))[0x1a])(
            *(void**)((char*)surfaceOwner + 0x0c), hdc);
    }
    else
    {
        fontId = 0;
        if (font == (HGDIOBJ)g_hFontArial10_guess)
            fontId = EQ_GLOBAL(u32, 0x00591784);
        else if (font == (HGDIOBJ)g_hFontArial12_guess)
            fontId = EQ_GLOBAL(u32, 0x00591788);
        else if (font == (HGDIOBJ)g_hFontArial14_guess)
            fontId = EQ_GLOBAL(u32, 0x0059178C);
        else if (font == (HGDIOBJ)g_hFontArial15_guess)
            fontId = EQ_GLOBAL(u32, 0x00591790);
        else if (font == (HGDIOBJ)g_hFontArial16_guess)
            fontId = EQ_GLOBAL(u32, 0x00591794);
        else if (font == (HGDIOBJ)g_hFontArial20_guess)
            fontId = EQ_GLOBAL(u32, 0x00591798);
        else if (font == (HGDIOBJ)g_hFontArial24_guess)
            fontId = EQ_GLOBAL(u32, 0x0059179C);

        localTextPosition[0] = (float)x;
        localTextPosition[1] = (float)y;
        localTextPosition[2] = 0.0f;
        // states 1, 5, 6: centre a 640x480 UI in the video mode (1..4)
        if ((g_game_state_guess == 1) ||
            (g_game_state_guess == 5) ||
            (g_game_state_guess == 6))
        {
            if (g_video_mode_guess == 1)
            {
                localTextPosition[0] = localTextPosition[0] + 80.0f;
                localTextPosition[1] = localTextPosition[1] + 60.0f;
            }
            if (g_video_mode_guess == 2)
            {
                localTextPosition[0] = localTextPosition[0] + 192.0f;
                localTextPosition[1] = localTextPosition[1] + 144.0f;
            }
            if (g_video_mode_guess == 3)
            {
                localTextPosition[0] = localTextPosition[0] + 320.0f;
                localTextPosition[1] = localTextPosition[1] + 272.0f;
            }
            if (g_video_mode_guess == 4)
            {
                localTextPosition[0] = localTextPosition[0] + 480.0f;
                localTextPosition[1] = localTextPosition[1] + 360.0f;
            }
        }

        if (colorIndex > 0xff)
        {
            // font is reused here to hold the COLORREF
            font = (HGDIOBJ)EQ_CDECL(u32, 0x0040BB3F, (int))(colorIndex - 0x100);
            // state 2: black text becomes light grey
            if ((g_game_state_guess == 2) && (font == (HGDIOBJ)0))
                font = (HGDIOBJ)0x00f0f0f0;
            // COLORREF 0x00BBGGRR -> 0xFFRRGGBB
            argbColor = (u32)font & 0xff00 |
                    (u32)font >> 0x10 & 0xff |
                    ((u32)font | 0xffffff00) << 0x10;
        }
        else
        {
            // palette index -> ARGB colour
            switch (colorIndex)
            {
            case 0:
                argbColor = 0xff000000;
                break;
            case 2:
                argbColor = 0xff008000;
                break;
            case 4:
                argbColor = 0xff000080;
                break;
            case 5:
                argbColor = 0xfff000f0;
                break;
            case 6:
                argbColor = 0xff808080;
                break;
            case 7:
                argbColor = 0xffe0e0e0;
                break;
            case 10:
                argbColor = 0xfff0f0f0;
                break;
            case 0xc:
                argbColor = 0xffa0a0a0;
                break;
            case 0xd:
                argbColor = 0xfff00000;
                break;
            case 0xe:
                argbColor = 0xff00f000;
                break;
            case 0xf:
                argbColor = 0xfff0f000;
                break;
            case 0x10:
                argbColor = 0xff0000f0;
                break;
            case 0x12:
                argbColor = 0xff00f0f0;
                break;
            default:
                argbColor = 0xff606060;
                break;
            }
        }

        EQ_GLOBAL(DeferTextA_0040be28_t, 0x006E51E0)(text, fontId, &localTextPosition[0], argbColor, 0);
    }
    return result;
}

// FUNCTION: TRILOGY 0x0040c113
// Changes an actor's displayed size by applying a scale.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetActorScaleFactor(void* actor, float scale, u8 flag)
{
    if (actor)
        ((int (__cdecl *)(void*, float, int))t3dSetActorScaleFactor)(actor, scale, flag);
}

// FUNCTION: TRILOGY 0x0040c138
// Saves a screenshot using the next available EQ######.bmp filename.
void CaptureScreenshot_guess(void)
{
    char fileName[80];
    int fileNumber;
    FILE* file;

    fileNumber = 0;
    while (1)
    {
        sprintf(fileName, "EQ%06d.bmp", fileNumber);
        file = fopen(fileName, "rb");
        if (file == 0)
            break;
        fclose(file);
        fileNumber = fileNumber + 1;
    }
    EQ_GLOBAL(t3dScreenCapture_guess_t, 0x006E5368)(fileName);
}

// FUNCTION: TRILOGY 0x0040c17d
// Finds the nearest visible actor of a requested type and returns its tag.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: register picks differ (original zero=EBX, player=ESI, this=EDI); count store precedes the jbe; region stays in EAX.
bool __fastcall GetNearestActorTag(CDisplay* display, int, char* tag, int actorType,
                                         EQPlayer* player)
{
    int closestActor = 0;
    float playerLocation[7];
    u32 actorCount;
    float searchRadius;
    float closestDistance;
    void** actors;
    void* actor;

    if (EQ_METHOD(0, int, 0x00408E7F, (CDisplay*), display)() == 0 ||
        player == 0 || player->actor_info == 0)
    {
        *tag = 0;
    }
    else
    {
        actor = *(void**)player->actor_info;
        if (actor == 0)
        {
            int region;

            playerLocation[0] = player->y;
            playerLocation[1] = player->x;
            playerLocation[2] = player->z;
            playerLocation[3] = player->heading;
            playerLocation[4] = player->pitch;
            region = EQ_GLOBAL(t3dGetRegionNumberFromWorldAndXYZ_guess_t, 0x006E52EC)(
                display->world_guess, playerLocation);
            if (region == -1)
            {
                *tag = 0;
                return 0;
            }
        }
        else
        {
            t3dGetActorLocation(actor, playerLocation);
        }

        searchRadius = EQ_GLOBAL(float, 0x006B66E0) - EQ_GLOBAL(float, 0x0053515C);
        if (searchRadius < EQ_GLOBAL(float, 0x00535190))
            searchRadius = EQ_GLOBAL(float, 0x00535190);

        actorCount = EQ_GLOBAL(s3dGetWorldVisibleActorList_guess_t, 0x006E511C)(
            display->world_guess, 0, playerLocation, searchRadius, 0,
            (char*)display + 0x2CBC);
        actors = *(void***)((char*)display + 0x2CC8);
        if (actorCount > 0)
        {
            do
            {
                actor = *actors;
                if (actor != 0 && *(int*)((char*)actor + 0x60) == actorType &&
                    (player->race != 0x8E ||
                     !(t3dGetScaledActorBoundingRadius(actor) >
                       EQ_GLOBAL(float, 0x00534498))))
                {
                    float actorLocation[7];
                    float distance;

                    t3dGetActorLocation(actor, actorLocation);
                    distance = EQ_METHOD(7, float, 0x00412984, (CDisplay*, float, float, float, float, float, float, float), display)(playerLocation[0], playerLocation[1], playerLocation[2],
                            actorLocation[0], actorLocation[1], actorLocation[2], 0.0f);
                    if (closestActor == 0 || distance < closestDistance)
                    {
                        closestDistance = distance;
                        closestActor = (int)actor;
                    }
                }
                actors++;
                actorCount--;
            } while (actorCount != 0);
            if (closestActor != 0)
            {
                SOC_CPY(tag,
                    *(const char**)((char*)*(void**)((char*)closestActor + 0x64) + 8));
                return 1;
            }
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0040c305
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// A tail jump to 0x0040E418 (whichever of the two is the Mac ShowDisplay).
void CDisplay::ShowDisplayThunk_guess()
{
    ShowDisplay_guess();
}

// Stand-in until display.h declares RealRender_World (include/ is not changed here). It adds no fields, so
// `this` is the CDisplay the caller passes in ECX. Rename the definition to CDisplay::RealRender_World once
// the declaration is in the class.
struct CDisplay_RealRender_World_guess : CDisplay
{
    void RealRender_World();
};

// EQ_Main (0x006E4E44) and the container windows seen through their vtable: slot 1 (+0x04) and slot 2 (+0x08).
class OverlayWindowVtbl_guess
{
public:
    virtual void vfunc0();
    virtual void Activate();
    virtual void Deactivate();
};

typedef void (__stdcall *rwSleep_guess_t)(u32);
typedef void (__cdecl *rwSetWorldTime_guess_t)(void*, u32);
typedef void (__cdecl *rwGetCameraLocation_guess_t)(void*, void*);
typedef void (__cdecl *rwFreeMemory_guess_t)(void*);
typedef float (__cdecl *rwGetCameraYon_guess_t)(void*);
typedef void (__cdecl *rwSetSky_guess_t)(void*, int);
typedef void (__cdecl *rwRenderPartialScene_guess_t)(void*, void*, void*, void*);
typedef int (__stdcall *rwSetRect_guess_t)(RECT*, int, int, int, int);

#define g_frame_parity_guess          EQ_GLOBAL(int, 0x006B6768)
#define g_sound_disabled_guess        EQ_GLOBAL(int, 0x006EFE1C)
#define g_sound_listener_guess        EQ_GLOBAL(void*, 0x006E4DD4)
#define g_frame_sleep_ms_guess        EQ_GLOBAL(u32, 0x006B6EF8)
#define g_hover_count_enabled_guess   EQ_GLOBAL(int, 0x006B6864)
#define g_mouse_button1_guess         EQ_GLOBAL(s16, 0x006B6418)
#define g_mouse_button2_guess         EQ_GLOBAL(s16, 0x006B641A)
#define g_hover_count_time_guess      EQ_GLOBAL(u32, 0x006B64D4)
#define g_switch_top_guess            EQ_GLOBAL(void*, 0x006E4D78)
#define g_visible_actors_only_guess   EQ_GLOBAL(int, 0x0053C4BC)
#define g_missile_head_guess          EQ_GLOBAL(char*, 0x006E4D64)
#define g_effect_top_guess            EQ_GLOBAL(void*, 0x006E4D74)
#define g_pending_weather_guess       EQ_GLOBAL(int, 0x006B6510)
#define g_pending_weather_level_guess EQ_GLOBAL(int, 0x006B6514)
#define ACT_TARGET                EQ_GLOBAL(EQPlayer*, 0x006E4DAC)
#define g_tinted_target_id_guess      EQ_GLOBAL(u32, 0x005917C8)
#define g_zone_sky_type_guess         EQ_GLOBAL(u8, 0x006B66F4)
#define g_sky_guess                   EQ_ARRAY(u8, 0x0058FC70)
#define g_show_net_status_guess       EQ_GLOBAL(u8, 0x006B645C)
#define g_rdp_connection_guess        EQ_GLOBAL(void*, 0x006ECCA8)
#define g_net_status_overlay_guess    EQ_GLOBAL(void*, 0x006E4DE4)
#define g_drag_item_guess             EQ_GLOBAL(void*, 0x006E4E48)
#define g_columns_wnd_guess           EQ_GLOBAL(void*, 0x006E4DF8)
#define g_stml_wnd_guess              EQ_GLOBAL(void*, 0x006E4DF4)
#define g_item_display_guess          EQ_GLOBAL(void*, 0x006E4D84)
#define g_3dview_guess                EQ_GLOBAL(void*, 0x006E4DEC)
#define g_container1_guess            EQ_GLOBAL(char*, 0x006E4E58)
#define g_container2_guess            EQ_GLOBAL(char*, 0x006E4E5C)
#define g_hslider_guess               EQ_GLOBAL(void*, 0x00604984)
#define g_hslider2_guess              EQ_GLOBAL(void*, 0x006018C0)
#define g_requestor_guess             EQ_GLOBAL(void*, 0x006E4E20)
#define g_screenshot_request_guess    EQ_GLOBAL(u8, 0x006B6854)
#define g_trade_partner_guess         EQ_GLOBAL(EQPlayer*, 0x006E4D88)
#define g_item_list_guess             EQ_GLOBAL(void*, 0x006E4D5C)
#define g_loot_target_guess           EQ_GLOBAL(EQPlayer*, 0x006E4DC0)
#define g_house_bank_target_guess     EQ_GLOBAL(EQPlayer*, 0x006E4DCC)
#define g_merchant_guess              EQ_GLOBAL(EQPlayer*, 0x006E4DB8)
#define g_banker_guess                EQ_GLOBAL(EQPlayer*, 0x006E4DBC)
#define g_trainer_guess               EQ_GLOBAL(EQPlayer*, 0x006E4DC4)
#define EXEC_SAVED_VIDEO_MODE   EQ_GLOBAL(int, 0x00590170)

#define RW_PLAYER_ACTIVE_MS           12000
#define RW_CAMERA_REFRESH_MS          250
#define RW_HOVER_COUNT_MS             500
#define RW_NET_STATUS_REFRESH_MS      1000
#define RW_RESPAWN_NOTICE_MS          2000
#define RW_MAX_ACTOR_RADIUS           600.0f
#define RW_CLOSE_DISTANCE             20.0f
#define RW_EVERQUEST_IN_GAME          5

#define rwPhysics(player, missile)    EQ_CDECL(void, 0x004E12BE, (void*, void*, int))((player), (missile), 0)
#define rwMovePlayer(player)          EQ_METHOD(1, void, 0x004AE6DF, (CDisplay*, EQPlayer*), this)((player))
#define rwSetNameSpriteTint(player)   EQ_METHOD(1, void, 0x00411EE6, (CDisplay*, EQPlayer*), this)((player))
#define rwCall0(va, obj)              EQ_METHOD(0, void, va, (void*), (obj))()
#define rwTrueDistance(display, from, to) \
    EQ_METHOD(7, float, 0x004129F3, (CDisplay*, float, float, float, float, float, float, float), (display)) \
        ((from)->y, (from)->x, (from)->z, (to)[0], (to)[1], (to)[2], 1.0f)
#define rwPlayerDistance(player, zScale) \
    EQ_METHOD(3, float, 0x00412AA0, (CDisplay*, EQPlayer*, EQPlayer*, float), g_cdisplay_guess) \
        ((player), g_local_player_guess, (zScale))
#define rwEverQuestState()            (pinstCEverQuest_guess->gameState_guess)

// FUNCTION: TRILOGY 0x0040c30a
// Main-loop frame step: advances and draws the zone and the in-game windows, then enforces interaction ranges.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// One frame: physics for spawns, missiles and effects, weather, the 3D scene, the UI windows, the display flip,
// then the range checks that close trade/loot/bank windows and the respawn-at-death countdown.
void CDisplay_RealRender_World_guess::RealRender_World()
{
    float savedZ;
    RECT statusRect;
    u32 message[2];
    int perfStats[10];
    RECT sourceRect;
    char weatherText[128];
    EQPlayer* player;
    EQPlayer* mount;
    EQPlayer* target;
    void** actors;
    u32 index;
    char* missile;
    char* nextMissile;
    char* itemActor;
    float yon;
    float lootRange;
    int statusX;
    int statusY;
    u32 respawnTime;
    rwRenderPartialScene_guess_t renderScene;

    g_frame_parity_guess ^= 1;
    if (g_sound_disabled_guess == 0)
    {
        if (g_sound_listener_guess != 0)
            EQ_METHOD(1, void, 0x004E3F9E, (void*, void*), g_sound_listener_guess)(camera_guess);
        if (g_environment_ptr_guess != 0)
            rwCall0(0x004E2A20, g_environment_ptr_guess);
        if (g_musicState_guess != 0)
            rwCall0(0x004E50C1, g_musicState_guess);
    }
    if (unk_0x0C1 != 0)
        unk_0x0C1 = 0;
    time_ms_guess = EqGetTime();
    if (EQ_CDECL(int, 0x004E7CF9, (void))() != 0)
        EQ_GLOBAL(rwSleep_guess_t, 0x00534198)(g_frame_sleep_ms_guess);
    if (g_hover_count_enabled_guess != 0 && g_eqmain_guess != 0)
    {
        if (g_mouse_button1_guess == 0 && g_mouse_button2_guess == 0)
        {
            if (time_ms_guess - g_hover_count_time_guess > RW_HOVER_COUNT_MS)
            {
                g_hover_count_time_guess = time_ms_guess;
                if (pinstEQ_Main_guess->hoverTicks_guess < 3)
                    (pinstEQ_Main_guess->hoverTicks_guess)++;
            }
        }
        else
        {
            pinstEQ_Main_guess->hoverTicks_guess = 0;
            if (pinstEQ_Main_guess->hoverId_guess >= 0)
                pinstEQ_Main_guess->uiDirty_guess = 1;
        }
    }
    EQ_GLOBAL(rwSetWorldTime_guess_t, 0x006E545C)(world_guess, time_ms_guess);
    if (g_switch_top_guess != 0)
        EQ_METHOD(0, void, 0x00408482, (CDisplay*), this)();
    CDisplay_UpdatePlayerVisibility_guess(this, g_local_player_guess);

    if (g_visible_actors_only_guess == 0)
    {
        for (player = g_player_top_guess; player != 0; player = player->next)
        {
            if (player == g_control_player_guess ||
                time_ms_guess - player->actor_info->lastActiveTime_guess <= RW_PLAYER_ACTIVE_MS ||
                (player->type != 0 && player->speed == 0.0f) || player->race == 0x48)
            {
                rwPhysics(player, 0);
                rwMovePlayer(player);
            }
        }
    }
    else
    {
        if (g_control_player_guess->actor_info != 0 &&
            (mount = g_control_player_guess->actor_info->ride_target_guess) != 0)
        {
            savedZ = mount->z;
            rwPhysics(mount, 0);
            mount = g_control_player_guess->actor_info->ride_target_guess;
            if (mount->z != savedZ && mount->race != 0x8D)
            {
                mount->z = savedZ;
                g_control_player_guess->actor_info->ride_target_guess->dz = 0.0f;
                *(float*)&g_control_player_guess->actor_info->ride_target_guess->unk_0x60 = 0.0f;
            }
            rwMovePlayer(g_control_player_guess->actor_info->ride_target_guess);
            mount = g_control_player_guess->actor_info->ride_target_guess;
            if (mount->z != savedZ && mount->race != 0x8D)
            {
                mount->z = savedZ;
                g_control_player_guess->actor_info->ride_target_guess->dz = 0.0f;
            }
        }
        rwPhysics(g_control_player_guess, 0);
        rwMovePlayer(g_control_player_guess);
        if (unk_0x124 == 0 || time_ms_guess - unk_0x124 > RW_CAMERA_REFRESH_MS)
        {
            unk_0x124 = time_ms_guess;
            EQ_GLOBAL(rwGetCameraLocation_guess_t, 0x006E5290)(camera_guess, cameraLocation_guess);
            if (allocatedBlock12C_guess != 0)
            {
                EQ_GLOBAL(rwFreeMemory_guess_t, 0x006E5264)(allocatedBlock12C_guess);
                allocatedBlock12C_guess = 0;
            }
            yon = EQ_GLOBAL(rwGetCameraYon_guess_t, 0x006E529C)(camera_guess);
            if (yon > RW_MAX_ACTOR_RADIUS)
                yon = RW_MAX_ACTOR_RADIUS;
            *(int*)&unknown0x0128[0] = EQ_GLOBAL(s3dGetWorldVisibleActorList_guess_t, 0x006E511C)(
                world_guess, 0, cameraLocation_guess, yon, 0x11, refList40_guess);
        }
        actors = *(void***)&refList40_guess[0xC];
        for (index = 0; index < *(u32*)&unknown0x0128[0]; index++, actors++)
        {
            if (*actors != 0)
            {
                target = *(EQPlayer**)((char*)*actors + 0x60);
                if (target > (EQPlayer*)0x10 && target->objectKind_guess == 3)
                {
                    if (g_frame_parity_guess != 0 && target != g_local_player_guess &&
                        target->stand_state != 0x78 && target->stand_state != 0x6E &&
                        (target->actor_info->sound_data_a_guess != 0 ||
                         target->actor_info->sound_data_b_guess != 0) &&
                        g_environment_ptr_guess != 0)
                        EQ_METHOD(1, void, 0x004E2074, (void*, EQPlayer*), g_environment_ptr_guess)(target);
                    if (target != g_control_player_guess &&
                        target != g_control_player_guess->actor_info->ride_target_guess &&
                        (time_ms_guess - target->actor_info->lastActiveTime_guess <= RW_PLAYER_ACTIVE_MS ||
                         (target->type != 0 && target->speed == 0.0f) ||
                         target == g_local_player_guess || DAT_006b6494 != 0))
                    {
                        rwPhysics(target, 0);
                        rwMovePlayer(target);
                    }
                }
            }
        }
    }

    for (missile = g_missile_head_guess; missile != 0; missile = nextMissile)
    {
        nextMissile = *(char**)(missile + 8);
        if (**(void***)(missile + 0x3C) == 0)
        {
            rwCall0(0x004AD1A4, missile);
            if (*(*(char**)(missile + 0x3C) + 0x2C1) == 1 && missile != 0)
            {
                rwCall0(0x004ACEAD, missile);
                EQ_CDECL(void, 0x005218BB, (void*))(missile);
            }
        }
        else if (time_ms_guess - *(u32*)(*(char**)(missile + 0x3C) + 0x54) > 0x20)
        {
            rwCall0(0x004AD574, missile);
            rwPhysics(0, missile);
            EQ_METHOD(1, void, 0x004ADAC3, (CDisplay*, void*), this)(missile);
        }
    }
    if (g_effect_top_guess != 0)
        EQ_METHOD(0, void, 0x0040DD98, (CDisplay*), this)();
    rwCall0(0x00425D5A, g_pc_guess);
    if (g_pending_weather_guess != -1)
    {
        SOC_FMT(weatherText, EQ_ARRAY(char, 0x0053E16C),
                                                            g_pending_weather_level_guess);
        if (g_pending_weather_guess == 1)
            EQ_CDECL(void, 0x004A97C8, (int, char*))(0, weatherText);
        else
            EQ_CDECL(void, 0x004A945B, (int, char*))(0, weatherText);
        g_pending_weather_guess = -1;
    }

    if (ACT_TARGET != 0)
    {
        if (g_tinted_target_id_guess != 0 && g_tinted_target_id_guess != ACT_TARGET->spawn_id)
        {
            player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, g_tinted_target_id_guess);
            if (player != 0)
                rwSetNameSpriteTint(player);
            g_tinted_target_id_guess = 0;
        }
        rwSetNameSpriteTint(ACT_TARGET);
        g_tinted_target_id_guess = ACT_TARGET->spawn_id;
    }
    else if (g_tinted_target_id_guess != 0)
    {
        player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, g_tinted_target_id_guess);
        if (player != 0)
            rwSetNameSpriteTint(player);
        g_tinted_target_id_guess = 0;
    }

    if (g_zone_sky_type_guess != 2 && g_zone_sky_type_guess != 1 && g_zone_sky_type_guess != 5)
        CDisplay_SetAmbientLight_guess(this, g_weather_ambient_guess[weather_ambient_index_guess]);
    else
    {
        if (weather_active_guess != 0)
            EQ_METHOD(0, void, 0x0040D9F4, (CDisplay*), this)();
        EQ_METHOD(0, void, 0x00412742, (CDisplay*), this)();
    }

    renderScene = EQ_GLOBAL(rwRenderPartialScene_guess_t, 0x006E5360);
    if (g_textDeferred_guess == 0)
    {
        if (g_eqmain_guess != 0)
        {
            if (g_eqmain_guess[0xFE0] == 5 && g_game_state_guess == 1)
                goto world_rendered;
            if (g_video_mode_guess != 0)
                EQ_METHOD(0, void, 0x0040628B, (CDisplay*), this)();
            if (skyLayerMode_guess != 0 && weather_active_guess == 0)
            {
                EQ_GLOBAL(rwSetSky_guess_t, 0x006E544C)(g_sky_guess, skyType_guess);
                renderScene(world_guess, render_guess, camera_guess, g_sky_guess);
            }
            else
                renderScene(world_guess, render_guess, camera_guess, 0);
        }
        else
        {
            if (g_video_mode_guess != 0)
                EQ_METHOD(0, void, 0x0040628B, (CDisplay*), this)();
            renderScene(world_guess, render_guess, camera_guess,
                        (skyLayerMode_guess != 0 && weather_active_guess == 0) ? g_sky_guess : 0);
        }
        if (g_control_player_guess->actor_info->unknown0x189 != 0)
            t3dClearRenderToBlack(0);
        if (g_video_mode_guess != 0)
            EQ_METHOD(0, void, 0x00406399, (CDisplay*), this)();
    }
world_rendered:
    EQ_CDECL(u32, 0x005227B1, (u32, u32))(0xA031F, 0xF031F);
    if (g_textDeferred_guess == 1 && g_no_render_offset_guess == 1)
    {
        rwCall0(0x00402AE3, boxDrawOwner_guess);
        rwCall0(0x00402BA9, boxDrawOwner_guess);
    }

    if (g_world_connection_guess[0] != 0 && g_show_net_status_guess != 0)
    {
        EQ_CDECL(void, 0x004F23E0, (void*, int*))(g_world_connection_guess[0], perfStats);
        if (*(int*)&unknown0x286C[0] != perfStats[0] ||
            EqGetTime() - perfStats[0] > RW_NET_STATUS_REFRESH_MS)
        {
            statusX = 0x186;
            statusY = 0x12B;
            if (g_game_state_guess == 2)
            {
                statusY = 5;
                statusX = statusY;
            }
            EQ_CDECL(void, 0x004E0AF0, (void*, int, int, int, int, void*, int, int, int, u32))(
                g_net_status_overlay_guess, 0, 0, statusX, statusY, g_world_connection_guess[0],
                perfStats[4], perfStats[2], perfStats[3],
                EQ_CDECL(u32, 0x004F1790, (void*))(g_rdp_connection_guess));
            *(int*)&unknown0x286C[0] = perfStats[0];
        }
        statusRect.left = 0;
        statusRect.top = 0;
        statusRect.right = 0x80;
        statusRect.bottom = 0x20;
        if (g_game_state_guess == 1)
        {
            if (g_video_mode_guess == 4)
                t3dOverlayApply(g_net_status_overlay_guess, 0x366, 0x293, (int)&statusRect);
            else if (g_video_mode_guess == 3)
                t3dOverlayApply(g_net_status_overlay_guess, 0x2C6, 0x23B, (int)&statusRect);
            else if (g_video_mode_guess == 2)
                t3dOverlayApply(g_net_status_overlay_guess, 0x246, 0x1BB, (int)&statusRect);
            else if (g_video_mode_guess == 1)
                t3dOverlayApply(g_net_status_overlay_guess, 0x1D6, 0x167, (int)&statusRect);
            else
                t3dOverlayApply(g_net_status_overlay_guess, 0x186, 0x12B, (int)&statusRect);
        }
        else if (g_game_state_guess == 2)
            t3dOverlayApply(g_net_status_overlay_guess, 5, 5, (int)&statusRect);
    }

    if (g_game_state_guess == 1)
    {
        if (rwEverQuestState() == RW_EVERQUEST_IN_GAME)
        {
            if (g_eqmain_guess != 0 && g_eqmain_guess[0xFE0] >= 8)
            {
                if (g_drag_item_guess != 0)
                    rwCall0(0x004348A1, g_drag_item_guess);
                if (g_eqmain_guess != 0)
                    rwCall0(0x0043B010, g_eqmain_guess);
                if (g_drag_item_guess != 0)
                    rwCall0(0x00434DE2, g_drag_item_guess);
            }
            else
            {
                if (g_drag_item_guess != 0)
                    rwCall0(0x004348A1, g_drag_item_guess);
                if (*((char*)pinstCEverQuest_guess + 0x13EC) != 0)
                {
                    *((char*)pinstCEverQuest_guess + 0x13EC) = 0;
                    if (g_chatWindow_guess != 0)
                        pinstEQ_TextIO_guess->redrawPass_guess = 1;
                }
                if (g_eqmain_guess != 0)
                {
                    if (pinstEQ_Main_guess->renderDollActive_guess != 0 &&
                        g_control_player_guess->actor_info->unknown0x189 == 0)
                        EQ_METHOD(0, void, 0x004063DC, (CDisplay*), this)();
                    if (g_eqmain_guess != 0)
                    {
                        rwCall0(0x0043B010, g_eqmain_guess);
                        pinstEQ_Main_guess->drawParity_guess ^= 1;
                    }
                }
                if (g_columns_wnd_guess != 0)
                    rwCall0(0x004326A6, g_columns_wnd_guess);
                if (g_chatWindow_guess != 0 && (g_eqmain_guess == 0 || pinstEQ_Main_guess->socialEditorOpen_guess == 0))
                {
                    if (g_chatInputInactive_guess != 0)
                    {
                        if (pinstEQ_TextIO_guess->textDisplayMode_guess > 0)
                        {
                            (pinstEQ_TextIO_guess->textDisplayMode_guess)--;
                            pinstEQ_TextIO_guess->redrawPass_guess = 1;
                        }
                    }
                    else if (pinstEQ_TextIO_guess->textDisplayMode_guess < 3)
                    {
                        (pinstEQ_TextIO_guess->textDisplayMode_guess)++;
                        pinstEQ_TextIO_guess->redrawPass_guess = 1;
                    }
                    rwCall0(0x004D0516, pinstCEverQuest_guess);
                }
                if (g_stml_wnd_guess != 0)
                    rwCall0(0x00436946, g_stml_wnd_guess);
                if (g_item_display_guess != 0)
                    rwCall0(0x00438AD4, g_item_display_guess);
                if (g_eqBook_guess != 0)
                    rwCall0(0x00418CCD, g_eqBook_guess);
                if (g_3dview_guess != 0 && g_eqmain_guess != 0 && g_eqmain_guess[0xFE0] < 5 &&
                    *(int*)(g_eqBook_guess + 0x10) == 0)
                    rwCall0(0x004158DC, g_3dview_guess);
                if (g_container1_guess != 0)
                    rwCall0(0x00417A5C, g_container1_guess);
                if (g_container2_guess != 0)
                    rwCall0(0x00417A5C, g_container2_guess);
                if (g_hslider_guess != 0)
                    rwCall0(0x004860E5, g_hslider_guess);
                if (g_hslider2_guess != 0)
                    rwCall0(0x00486AFF, g_hslider2_guess);
                if (g_requestor_guess != 0)
                    rwCall0(0x00481474, g_requestor_guess);
                if (g_drag_item_guess != 0)
                    rwCall0(0x00434DE2, g_drag_item_guess);
                if (g_eqmain_guess != 0)
                {
                    pinstEQ_Main_guess->hoverId_guess = -1;
                    if (g_hslider2_guess != 0 && pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x00486A0E, g_hslider2_guess);
                    if (g_hslider_guess != 0 && pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x00485FF4, g_hslider_guess);
                    if (g_container2_guess != 0 && pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x00416EF2, g_container2_guess);
                    if (g_container1_guess != 0 && pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x00416EF2, g_container1_guess);
                    if (g_eqBook_guess != 0 && pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x00418BC5, g_eqBook_guess);
                    if (pinstEQ_Main_guess->hoverId_guess == -1)
                        rwCall0(0x0046DA45, g_eqmain_guess);
                }
            }
        }
    }
    else if (g_game_state_guess == 2)
    {
        if (rwEverQuestState() == RW_EVERQUEST_IN_GAME)
        {
            if (g_drag_item_guess != 0)
                rwCall0(0x004348A1, g_drag_item_guess);
            if (g_eqmain_guess != 0)
            {
                rwCall0(0x0043B010, g_eqmain_guess);
                pinstEQ_Main_guess->drawParity_guess ^= 1;
            }
            if (g_columns_wnd_guess != 0)
                rwCall0(0x004326A6, g_columns_wnd_guess);
            if (g_chatWindow_guess != 0 && (g_eqmain_guess == 0 || pinstEQ_Main_guess->socialEditorOpen_guess == 0))
            {
                if (g_chatInputInactive_guess != 0)
                {
                    if (pinstEQ_TextIO_guess->textDisplayMode_guess > 0)
                    {
                        (pinstEQ_TextIO_guess->textDisplayMode_guess)--;
                        pinstEQ_TextIO_guess->redrawPass_guess = 1;
                    }
                }
                else if (pinstEQ_TextIO_guess->textDisplayMode_guess < 3)
                {
                    (pinstEQ_TextIO_guess->textDisplayMode_guess)++;
                    pinstEQ_TextIO_guess->redrawPass_guess = 1;
                }
                rwCall0(0x004D0516, pinstCEverQuest_guess);
            }
            if (g_stml_wnd_guess != 0)
                rwCall0(0x00436946, g_stml_wnd_guess);
            if (g_3dview_guess != 0 && g_eqmain_guess != 0 && g_eqmain_guess[0xFE0] < 5)
                rwCall0(0x004158DC, g_3dview_guess);
            if (g_container1_guess != 0)
                rwCall0(0x00417A5C, g_container1_guess);
            if (g_container2_guess != 0)
                rwCall0(0x00417A5C, g_container2_guess);
            if (g_eqmain_guess != 0)
                rwCall0(0x0046D6FE, g_eqmain_guess);
            if (g_hslider_guess != 0)
                rwCall0(0x004860E5, g_hslider_guess);
            if (g_hslider2_guess != 0)
                rwCall0(0x00486AFF, g_hslider2_guess);
            if (g_requestor_guess != 0)
                rwCall0(0x00481474, g_requestor_guess);
            if (g_drag_item_guess != 0)
                rwCall0(0x00434DE2, g_drag_item_guess);
            if (g_item_display_guess != 0)
                rwCall0(0x00438AD4, g_item_display_guess);
        }
    }
    else if (g_game_state_guess == 3)
    {
        EQ_GLOBAL(rwSetRect_guess_t, 0x00534248)(&statusRect,
            g_render_rect_guess[2] / 2 - 10, g_render_rect_guess[3] / 2 - 10,
            g_render_rect_guess[2] / 2 + 10, g_render_rect_guess[3] / 2 + 10);
        EQ_GLOBAL(rwSetRect_guess_t, 0x00534248)(&sourceRect, 0x78, 0, 0x8C, 0x14);
        EQ_METHOD(3, void, 0x004500BE, (char*, void*, RECT*, RECT*), g_eqmain_guess)(
            pinstEQ_Main_guess->overlaySprite_guess, &sourceRect, &statusRect);
    }
    if (g_eqmain_guess != 0)
        pinstEQ_Main_guess->redrawAll_guess = 0;
    if (g_chatWindow_guess != 0)
        pinstEQ_TextIO_guess->redrawAll_guess = 0;

    if (g_textDeferred_guess == 1)
    {
        if (g_eqmain_guess != 0)
        {
            if (g_eqmain_guess[0xFE0] == 5 && g_game_state_guess == 1)
                goto ui_rendered;
            if (g_video_mode_guess != 0)
                EQ_METHOD(0, void, 0x0040628B, (CDisplay*), this)();
            if (skyLayerMode_guess != 0 && weather_active_guess == 0)
            {
                EQ_GLOBAL(rwSetSky_guess_t, 0x006E544C)(g_sky_guess, skyType_guess);
                EQ_GLOBAL(rwRenderPartialScene_guess_t, 0x006E5360)(world_guess, render_guess, camera_guess,
                                                                    g_sky_guess);
            }
            else
                EQ_GLOBAL(rwRenderPartialScene_guess_t, 0x006E5360)(world_guess, render_guess, camera_guess, 0);
        }
        else
        {
            if (g_video_mode_guess != 0)
                EQ_METHOD(0, void, 0x0040628B, (CDisplay*), this)();
            EQ_GLOBAL(rwRenderPartialScene_guess_t, 0x006E5360)(world_guess, render_guess, camera_guess,
                (skyLayerMode_guess != 0 && weather_active_guess == 0) ? g_sky_guess : 0);
        }
        if (g_control_player_guess->actor_info->unknown0x189 != 0)
            t3dClearRenderToBlack(0);
        if (g_video_mode_guess != 0)
            EQ_METHOD(0, void, 0x00406399, (CDisplay*), this)();
    }
ui_rendered:
    if (g_screenshot_request_guess != 0)
    {
        EQ_CDECL(void, 0x0040C138, (void))();
        g_screenshot_request_guess = 0;
    }
    EQ_METHOD(0, void, 0x0040E418, (CDisplay*), this)();

    if (g_eqmain_guess != 0 && g_eqmain_guess[0xFE0] != 8)
    {
        if (pinstEQ_Main_guess->tradeOpen_guess != 0 && g_trade_partner_guess != 0 &&
            rwTrueDistance(g_cdisplay_guess, g_local_player_guess, &g_trade_partner_guess->y) >
                g_trade_partner_guess->height_offset + 25.0f)
            rwCall0(0x004571EB, g_eqmain_guess);
        if (*(int*)(g_eqmain_guess + 0x1010) != 0 && g_container1_guess != 0 &&
            *(int*)(g_container1_guess + 0x174) == *(int*)(g_eqmain_guess + 0x1010))
        {
            itemActor = EQ_METHOD(1, char*, 0x004A03F7, (void*, int), g_item_list_guess)(
                pinstEQ_Main_guess->worldContainerDropId_guess);
            if (itemActor != 0 &&
                rwTrueDistance(this, g_control_player_guess, (float*)(itemActor + 0x28)) > RW_CLOSE_DISTANCE)
                ((OverlayWindowVtbl_guess*)g_container1_guess)->Deactivate();
        }
        if (*(int*)(g_eqmain_guess + 0x1010) != 0 && g_container2_guess != 0 &&
            *(int*)(g_container2_guess + 0x174) == *(int*)(g_eqmain_guess + 0x1010))
        {
            itemActor = EQ_METHOD(1, char*, 0x004A03F7, (void*, int), g_item_list_guess)(
                pinstEQ_Main_guess->worldContainerDropId_guess);
            if (itemActor != 0 &&
                rwTrueDistance(this, g_control_player_guess, (float*)(itemActor + 0x28)) > RW_CLOSE_DISTANCE)
                ((OverlayWindowVtbl_guess*)g_container2_guess)->Deactivate();
        }
        if (EQ_METHOD(0, int, 0x004504AC, (char*), g_eqmain_guess)() != 0 &&
            pinstEQ_Main_guess->lootOpen_guess != 0 && g_loot_target_guess != 0)
        {
            lootRange = g_loot_target_guess->height_offset * 1.5f + RW_CLOSE_DISTANCE;
            if (rwPlayerDistance(g_loot_target_guess, 1.2f) > lootRange)
                rwCall0(0x00464C8D, g_eqmain_guess);
        }
        if (pinstEQ_Main_guess->isActive_guess != 0 && pinstEQ_Main_guess->bankOpen_guess != 0 &&
            g_eqmain_guess[0x101] != 0 && g_house_bank_target_guess != 0 &&
            rwTrueDistance(g_cdisplay_guess, g_local_player_guess, &g_house_bank_target_guess->y) >
                RW_CLOSE_DISTANCE)
            rwCall0(0x0047D6E2, g_eqmain_guess);
        if (EQ_METHOD(0, int, 0x004504AC, (char*), g_eqmain_guess)() != 0 &&
            pinstEQ_Main_guess->merchantOpen_guess != 0 && g_merchant_guess != 0 &&
            rwPlayerDistance(g_merchant_guess, 1.0f) > RW_CLOSE_DISTANCE)
            rwCall0(0x00464AB6, g_eqmain_guess);
        if (g_eqmain_guess[0x100C] != 0)
        {
            player = pinstEQ_Main_guess->inspectSpawn_guess;
            if (player == 0 || player->type != 0 || rwPlayerDistance(player, 1.0f) > RW_CLOSE_DISTANCE)
            {
                if (EXEC_SAVED_VIDEO_MODE == 2)
                {
                    g_game_state_guess = 2;
                    CDisplay_ChangeVideoMode_guess(g_cdisplay_guess, 0);
                    g_eqmain_guess[0xFE1] = 0;
                }
                ((OverlayWindowVtbl_guess*)g_eqmain_guess)->Deactivate();
                g_eqmain_guess[0xFE0] = g_eqmain_guess[0xFE1];
                ((OverlayWindowVtbl_guess*)g_eqmain_guess)->Activate();
            }
        }
        if (g_banker_guess != 0 && rwPlayerDistance(g_banker_guess, 1.0f) > RW_CLOSE_DISTANCE)
            EQ_METHOD(1, void, 0x004C53D8, (CEverQuest*, int), pinstCEverQuest_guess)(1);
        if (g_trainer_guess != 0 && rwPlayerDistance(g_trainer_guess, 1.0f) > RW_CLOSE_DISTANCE)
        {
            if (g_sound_disabled_guess == 0 && g_musicState_guess != 0)
                EQ_METHOD(1, void, 0x004E4EE6, (void*, int), g_musicState_guess)(0x15);
            ((OverlayWindowVtbl_guess*)g_eqmain_guess)->Deactivate();
            g_eqmain_guess[0xFE0] = g_eqmain_guess[0xFE1];
            ((OverlayWindowVtbl_guess*)g_eqmain_guess)->Activate();
            message[0] = g_trainer_guess->spawn_id;
            message[1] = g_local_player_guess->spawn_id;
            send_message_guess(g_world_connection_guess, 0x209D, message, 8, 1);
            pinstEQ_Main_guess->pendingServerReplies_guess -= 1;
            pinstEQ_Main_guess->pendingServerReplies_guess = 0;
            g_trainer_guess = 0;
            ACT_TARGET = 0;
            if (EXEC_SAVED_VIDEO_MODE == 2)
            {
                g_game_state_guess = 2;
                if (g_eqmain_guess != 0)
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                CDisplay_ChangeVideoMode_guess(g_cdisplay_guess, 0);
            }
        }
    }

    if (g_local_player_guess->type > 1)
    {
        respawnTime = *(u32*)((char*)g_local_player_guess->actor_info + 0x68);
        if (respawnTime != 0)
        {
            if (respawnTime == 1)
            {
                if (g_eqmain_guess != 0)
                    pinstEQ_Main_guess->uiDirty_guess = 1;
                *(u32*)((char*)g_local_player_guess->actor_info + 0x68) = 0;
                rwCall0(0x004C717D, pinstCEverQuest_guess);
                EQ_METHOD(3, void, 0x004DC1B5, (CEverQuest*, char*, const char*, int), pinstCEverQuest_guess)(
                    g_pc_guess->bind_zone, "repop to home at death", 1);
                g_activeView_guess = 0;
            }
            else if (time_ms_guess - respawnTime > RW_RESPAWN_NOTICE_MS)
            {
                EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(
                    "Returning to home point, please wait...", 0xD);
                *(u32*)((char*)g_local_player_guess->actor_info + 0x68) = 1;
            }
        }
    }
}

// FUNCTION: TRILOGY 0x0040d59a
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The yon is set through the global display, not `this`.
void CDisplay::SetLoadZoneWeather_guess()
{
    CDisplay_SetDayPeriod_guess(this, 0x18);
    CDisplay_SetSunLight_guess(this);
    g_cdisplay_guess->SetYon_guess(5000.0f);
}

#define g_display_animation_clock_initialized EQ_GLOBAL(u8, 0x0059016C)
#define g_display_animation_clock             EQ_GLOBAL(u32, 0x00590150)
#define DISPLAY_ANIMATION_INTERVAL_MS         5000

typedef void (__cdecl *t3dSetWorldTime_guess_t)(void*, u32);
typedef void (__cdecl *t3dRenderPartialScene_guess_t)(void*, void*, void*, void*);

// FUNCTION: TRILOGY 0x0040d5c2
// Renders the local character in a reduced 3D scene.
void __fastcall Render_MinWorld(CDisplay* self, int unused)
{
    u32 currentTime;
    u32 animation;
    u32 elapsed;
    EQPlayer* player;
    int one;
    int zero;

    if ((g_display_animation_clock_initialized & 1) == 0)
    {
        g_display_animation_clock_initialized = g_display_animation_clock_initialized | 1;
        g_display_animation_clock = EQ_CDECL(u32, 0x004A461E, (void))();
    }
    animation = 0x20;
    currentTime = EQ_CDECL(u32, 0x004A461E, (void))();
    self->time_ms_guess = currentTime;
    EQ_GLOBAL(t3dSetWorldTime_guess_t, 0x006E545C)(self->world_guess, currentTime);
    for (player = g_player_top_guess; player != 0; player = player->next)
    {
        elapsed = currentTime - player->actor_info->lastPhysicsTime_guess;
        if (elapsed > 0x20)
            EQ_CDECL(void, 0x004E12BE, (EQPlayer*, int, int))(player, 0, 0);
        EQ_METHOD(1, void, 0x004AE6DF, (CDisplay*, EQPlayer*), self)(player);
    }
    one = 1;
    zero = 0;
    g_frame_parity_guess = g_frame_parity_guess ^ one;
    if (((g_frame_parity_guess != 0) && (g_sound_disabled_guess == 0)) &&
        (EQ_GLOBAL(int, 0x006E4DD8) != 0))
    {
        if ((EQ_GLOBAL(int, 0x006E4DD4) != 0) && (self->camera_guess != 0))
            EQ_METHOD(1, void, 0x004E3F9E, (void*, void*), g_sound_listener_guess)(self->camera_guess);
        if (g_musicState_guess != 0)
            EQ_METHOD(0, void, 0x004E50C1, (void*), g_musicState_guess)();
    }
    currentTime = self->time_ms_guess;
    if ((u32)(currentTime - g_display_animation_clock) <= DISPLAY_ANIMATION_INTERVAL_MS)
        goto play_animation;
    g_display_animation_clock = currentTime;
    if ((EQ_GLOBAL(void*, 0x006E4E70) != 0) &&
        (pinstEQ_CharCreateS_guess->page != 0))
    {
        animation = 0x20;
        goto play_animation;
    }
    switch (g_local_player_guess->class_)
    {
    case 1:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 1, 2, 3, 4, 5, 6, 7, 8, 0x1B, -1);
        goto play_animation;
    case 2:
    case 6:
    case 10:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 0x2A, 2, 3, 5, 0x2B, 0x2C, 8, -1);
        break;
    case 3:
    case 5:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 2, 3, 5, 6, 7, 8, 0x3C, 0x2A, 0x2B, 0x2C, -1);
        goto play_animation;
    case 4:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 1, 2, 3, 5, 6, 8, 9, 0x2A, 0x2B, 0x2C, -1);
        goto play_animation;
    case 7:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 1, 0x0B, 0x2E, 5, 6, 0x2D, 8, 0x2F, -1);
        goto play_animation;
    case 8:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 0x29, 2, 3, 5, 6, 0x27, 0x28, -1);
        break;
    case 9:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 2, 5, 6, 8, 4, -1);
        goto play_animation;
    case 11:
    case 12:
    case 13:
    case 14:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 0x2A, 2, 0x2B, 5, 0x45, 0x2C, 8, 0x45, -1);
        goto play_animation;
    case 15:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 1, 2, 3, 5, 6, 8, 0x2A, 0x2B, 0x2C, -1);
        goto play_animation;
    default:
        animation = EQ_CDECL(int, 0x004A474D, (int, ...))(
            0x1A, 1, 2, 3, 5, 6, 7, 8, -1);
    }

play_animation:
    EQ_METHOD(6, void, 0x0048ED3B, (CDisplay*, EQPlayer*, int, int, int, float, int), self)
        (g_local_player_guess, animation, one, zero, 1.0f, one);
    if (g_video_mode_guess != 0)
        EQ_METHOD(0, void, 0x0040628B, (CDisplay*), self)();
    EQ_GLOBAL(t3dRenderPartialScene_guess_t, 0x006E5360)(
        self->world_guess, self->render_guess, self->camera_guess, g_sky_guess);
    if (g_video_mode_guess != 0)
        EQ_METHOD(0, void, 0x00406399, (CDisplay*), self)();
}

// FUNCTION: TRILOGY 0x0040d84f
// Sets the sun light (colour from the world clock's hour and minute, direction) and the weather ambient.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: VC6 folds the hour index subtraction and scaling into LEA; the original uses DEC and SHL.
// Needs a world and 3D. Zone types 1, 2 and 5 set the sun colour from the hour's table, blended by the minute,
// and the sun direction (0, sin a, -cos a) with a = 512 - the float at 0x0058FCA8; other zones clear the colour.
// Then the ambient level for the weather index.
// display.h has no CDisplay::SetSunLight_guess member, so the body is a __fastcall free function: ECX holds the
// display, as in the __thiscall at 0x0040d84f. Callers go through CDisplay_SetSunLight_guess(d).
void __fastcall CDisplay_SetSunLight_impl_guess(CDisplay* self)
{
    float normal[3];
    float color[3];
    int minute;

    if (self->world_guess != 0 && self->is3dOn_guess != 0)
    {
        if (g_zone_sky_type_guess == 2 || g_zone_sky_type_guess == 1 || g_zone_sky_type_guess == 5)
        {
            int temp = g_pWorldTime_guess->hour + 1;
            if (temp > 0x18)
                temp = 1;
            minute = g_pWorldTime_guess->minute_guess;
            int hourIndex = g_pWorldTime_guess->hour - 1;
            int nextHourIndex = temp - 1;
            float minuteBlend = (float)minute * EQ_GLOBAL(float, 0x005351A8);
            color[0] = (EQ_ARRAY(float, 0x005767D0)[nextHourIndex] - EQ_ARRAY(float, 0x005767D0)[hourIndex]) * minuteBlend
                       + EQ_ARRAY(float, 0x005767D0)[hourIndex];
            color[1] = (EQ_ARRAY(float, 0x00576830)[nextHourIndex] - EQ_ARRAY(float, 0x00576830)[hourIndex]) * minuteBlend
                       + EQ_ARRAY(float, 0x00576830)[hourIndex];
            color[2] = (EQ_ARRAY(float, 0x00576890)[nextHourIndex] - EQ_ARRAY(float, 0x00576890)[hourIndex]) * minuteBlend
                       + EQ_ARRAY(float, 0x00576890)[hourIndex];
            EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t, 0x006E53F0)(self->sunLightDef_guess, color);
            normal[0] = 0.0f;
            normal[1] = EQ_GLOBAL(t3dFloatSine_guess_t, 0x006E5254)(EQ_GLOBAL(float, 0x00535108) - EQ_GLOBAL(float, 0x0058FCA8));
            normal[2] = -EQ_GLOBAL(t3dFloatCosine_guess_t, 0x006E5248)(EQ_GLOBAL(float, 0x00535108) - EQ_GLOBAL(float, 0x0058FCA8));
            EQ_GLOBAL(t3dSetDirectionalLightNormal_guess_t, 0x006E53D8)(self->sunDirectionalLight_guess, normal);
        }
        else
        {
            color[0] = 0.0f;
            color[1] = 0.0f;
            color[2] = 0.0f;
            EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t, 0x006E53F0)(self->sunLightDef_guess, color);
        }
        EQ_FUNC(void, __fastcall, 0x00412D2C, (CDisplay *, float))(
            self, g_weather_ambient_guess[self->weather_ambient_index_guess]);
    }
}

// FUNCTION: TRILOGY 0x0040d98d
// Starts the requested weather effect in the game world.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::StartWeather(u8 type, char instant)
{
    weather_type_guess = type;
    weather_active_guess = 1;
    weather_clouds_guess = 1;
    weather_c5_guess = 1;
    weather_cc_guess = 0.0f;
    weather_fade_guess = 0;
    weather_dc_time_guess = time_ms_guess;
    weather_d0_guess = 0.0f;
    weather_d4_time_guess = time_ms_guess;
    weather_c2_guess = 0;
    if (instant == 0)
    {
        weather_fade_guess = 0x100;
        UpdateWeather_guess();
    }
}

// FUNCTION: TRILOGY 0x0040d9f4
// Advances the current weather effect and updates its ambient setting.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::UpdateWeather_guess()
{
    EQ_FUNC(void, __fastcall, 0x00412D2C, (CDisplay*, float))(
        this, g_weather_ambient_guess[weather_ambient_index_guess]);
    if (weather_clouds_guess != 0)
        CDisplay_ProcessCloud_guess(this);
    else
        EQ_FUNC(void, __fastcall, 0x0040DAB0, (CDisplay*))(this);
}

// FUNCTION: TRILOGY 0x0040da27
// Stops the current weather effect, with an option to apply the stop immediately.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The fade is mirrored (0x100 - fade) unless it is negative or already 0x100, then 0. Waves 0x9E and
// 0x9F are handed to the sound manager (when there is one) before an immediate stop.
void CDisplay::StopWeather(u8 type, char instant)
{
    weather_c5_guess = 0;
    weather_type_guess = type;
    weather_clouds_guess = 1;
    if (weather_fade_guess > -1 && weather_fade_guess != 0x100)
        weather_fade_guess = 0x100 - weather_fade_guess;
    else
        weather_fade_guess = 0;
    if (instant == 0)
    {
        if (g_sound_manager_ptr_guess != 0)
            SoundManager_StopWave_guess(g_sound_manager_ptr_guess, 0x9E);
        if (g_sound_manager_ptr_guess != 0)
            SoundManager_StopWave_guess(g_sound_manager_ptr_guess, 0x9F);
        weather_fade_guess = 0x100;
        UpdateWeather_guess();
    }
}

// FUNCTION: TRILOGY 0x0040dab0
// Triggers timed weather audio when an event is due.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Free fastcall entry preserves the thiscall register/stack layout because the method is not declared in the headers.
// Once the strike time is reached, plays the thunder wave for the weather type (1: 0x8F or 0x90, 2: 0x9E) and
// schedules the next strike: a quick repeat (0-999 ms, c2 keeps the wave) in about 30% of cases, else 10-19.999 s.
// @early-stop: the c2 mask is narrowed to byte ops (dec al; and al, byte ptr [DAT]) where the original keeps dec eax; and eax, dword ptr [DAT].
void __fastcall LightningStrike_guess(CDisplay* weatherContext)
{
    int roll;

    if (weatherContext->time_ms_guess >= weatherContext->weather_d4_time_guess)
    {
        if (weatherContext->weather_type_guess == 1)
        {
            if (weatherContext->weather_c2_guess != 0)
            {
                EQ_GLOBAL(int, 0x0053C6AC) = (weatherContext->weather_c2_guess == 0x8F) + 0x8F;
            }
            else
            {
                roll = random_range_guess(1, 100);
                EQ_GLOBAL(int, 0x0053C6AC) = (roll >= 0x32) + 0x8F;
            }
            if (g_sound_manager_ptr_guess != 0)
                EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(EQ_GLOBAL(int, 0x0053C6AC), 0);
            weatherContext->unk_0x0C1 = 1;
        }
        else if (weatherContext->weather_type_guess == 2 && g_sound_manager_ptr_guess != 0)
        {
            EQ_METHOD(2, void, 0x004E34FA, (void*, int, int), g_sound_manager_ptr_guess)(0x9E, 0);
        }
        if (weatherContext->weather_c2_guess == 0)
        {
            roll = EQ_CDECL(int, 0x00522EAD, (void))();
            roll = ((roll % 100 >= 0x1E) - 1) & EQ_GLOBAL(int, 0x0053C6AC);
            weatherContext->weather_c2_guess = (u8)roll;
        }
        else
        {
            weatherContext->weather_c2_guess = 0;
        }
        if (weatherContext->weather_c2_guess == 0)
        {
            roll = EQ_CDECL(int, 0x00522EAD, (void))();
            roll = roll % 10000;
            if (roll < 10000)
                roll = roll + 10000;
        }
        else
        {
            roll = EQ_CDECL(int, 0x00522EAD, (void))();
            roll = roll % 1000;
        }
        weatherContext->weather_d4_time_guess = weatherContext->time_ms_guess + roll;
    }
}

// FUNCTION: TRILOGY 0x0040dba9
// Selects the closest eligible actor within range, according to the player or NPC filter.
char* CDisplay_guess::FindNearestVisibleTarget_guess(float maxDistance, char findNpc)
{
    typedef void (__cdecl *t3dGetCameraLocation_guess_t)(void*, float*);
    int selectedActor;
    u32 actorCount;
    int* actorList;
    int actor;
    EQPlayer* target;
    EQPlayer* owner;
    u32 petOwnerId;
    u32 age;
    float location[7];
    float distance;
    float bestDistance;
    char collided;
    char* result;

    selectedActor = 0;
    EQ_GLOBAL(t3dGetCameraLocation_guess_t, 0x006E5290)
        (*(void**)((char*)this + 0x8), (float*)((char*)this + 0x1C));
    actorCount = EQ_GLOBAL(s3dGetWorldVisibleActorList_guess_t, 0x006E511C)
        (*(void**)((char*)this + 0x4), (int)*(void**)((char*)this + 0x8),
         0, maxDistance, 0x11, (char*)this + 0x2CBC);
    actorList = *(int**)((char*)this + 0x2CC8);
    for (; actorCount > 0; actorList++, actorCount--)
    {
        actor = *actorList;
        if (actor != 0)
        {
            target = *(EQPlayer**)(actor + 0x60);
            if ((u32)target > 0x10 && *(char*)target == 3 &&
                actor != *(int*)g_local_player_guess->actor_info &&
                actor != *(int*)g_control_player_guess->actor_info && target != 0)
            {
                if (findNpc != 0)
                {
                    if (target->type != 1)
                        continue;
                    petOwnerId = target->pet_owner_id;
                    if (petOwnerId > 0)
                    {
                        owner = EQ_METHOD(1, EQPlayer*, 0x004B1416, (EQPlayer*, int), g_player_top_guess)(petOwnerId);
                        if (owner != 0 && owner->type == 0)
                            continue;
                    }
                }
                else if (target->type != 0)
                    continue;

                age = this->time_ms_guess -
                    target->actor_info->lastActiveTime_guess;
                if (age <= 0x2EE0 &&
                    EQ_METHOD(2, int, 0x00410C80, (CDisplay_guess*, EQPlayer*, EQPlayer*), this)(g_local_player_guess, target) == 0)
                {
                    t3dGetActorLocation
                        ((void*)actor, location);
                    distance = EQ_METHOD(7, float, 0x00412984, (CDisplay_guess*, float, float, float,
                                            float, float, float, float), this)(
                         *(float*)((char*)this + 0x1C),
                         *(float*)((char*)this + 0x20),
                         *(float*)((char*)this + 0x24),
                         location[0], location[1], location[2], 0.0f);
                    if (distance <= maxDistance &&
                        (selectedActor == 0 || distance < bestDistance))
                    {
                        collided = EQ_METHOD(10, char, 0x00414523, (CDisplay_guess*,
                                               float, float, float,
                                               float, float, float,
                                               int, int, int, int), this)(
                             *(float*)((char*)this + 0x1C),
                             *(float*)((char*)this + 0x20),
                             *(float*)((char*)this + 0x24) + 1.0f,
                             location[0], location[1], location[2] + 2.5f,
                             0, 0, 0, 3);
                        if (collided == 0)
                        {
                            bestDistance = distance;
                            selectedActor = actor;
                        }
                    }
                }
            }
        }
    }

    result = 0;
    if (selectedActor != 0 && **(char**)(selectedActor + 0x60) == 3)
        result = *(char**)(selectedActor + 0x60);
    return result;
}

typedef void (__cdecl *t3dSetPointLightXYZ_guess_t)(void*, void*, float*);

// FUNCTION: TRILOGY 0x0040DD98
// Updates active world effects, follows attached sources, and removes expired or invalid effects.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void __fastcall ProcessEffects(CDisplay* display, int unusedEdx)
{
    EQEffect_guess* effect;
    EQEffect_guess* nextEffect;
    float location[7];
    float angle;
    float* sourceTransform;
    float sourceHeading;
    int setActorResult;

    if (display->world_guess != 0)
    {
        effect = EQ_GLOBAL(EQEffect_guess*, 0x006E4D74);
        while (effect != 0)
        {
            nextEffect = effect->next;
            if (effect->expirationTime == 0 ||
                display->time_ms_guess <= effect->expirationTime)
            {
                if (effect->type == 0)
                {
                    if (effect->attachedSource == 0)
                    {
                        if (effect->source == 0)
                            goto process_effect;
                        sourceTransform = (float*)((char*)effect->source + 0x34);
                    }
                    else
                        sourceTransform = (float*)((char*)effect->attachedSource + 0x0C);

                    if (sourceTransform != 0)
                    {
                        if (sourceTransform[3] != effect->heading)
                        {
                            angle = (float)(int)effect->headingOffset;
                            sourceHeading = sourceTransform[3];
                            effect->heading = sourceHeading;
                            angle = EQ_METHOD(1, float, 0x00408156,
                                (CDisplay*, float), display)(angle + sourceHeading);
                            effect->y = t3dFloatFastCosine(angle) * effect->scale + sourceTransform[0];
                            angle = t3dFloatFastSine(angle);
                            effect->x = angle * effect->scale + sourceTransform[1];
                        }
                        if (sourceTransform[11] != effect->roll)
                        {
                            angle = (float)(int)effect->rollOffset;
                            sourceHeading = sourceTransform[11];
                            effect->roll = sourceHeading;
                            angle = EQ_METHOD(1, float, 0x00408156,
                                (CDisplay*, float), display)(angle + sourceHeading + 256.0f);
                            angle = t3dFloatFastSine(angle);
                            effect->z = angle * effect->scale + sourceTransform[2];
                        }
                    }
                }
                else if (effect->type == 2)
                {
                    if (effect->source == 0)
                    {
                        if (effect->attachedSource != 0)
                        {
                            sourceTransform = (float*)((char*)effect->attachedSource + 0x0C);
                            effect->y = sourceTransform[0];
                            effect->x = sourceTransform[1];
                            effect->z = sourceTransform[2];
                        }
                    }
                    else
                    {
                        sourceTransform = (float*)((char*)effect->source + 0x34);
                        effect->y = sourceTransform[0];
                        effect->x = sourceTransform[1];
                        effect->z = sourceTransform[2];
                        if ((u32)(effect->expirationTime - display->time_ms_guess) < 0xC9)
                            effect->speed_guess = 0.0f;
                    }
                }

process_effect:
                if (0x20 < (u32)(display->time_ms_guess - effect->lastPhysicsTime))
                    EQ_CDECL(void, 0x004E12BE, (int, int, EQEffect_guess*))(0, 0, effect);

                t3dGetActorLocation(effect->actor, location);
                location[0] = effect->y;
                location[1] = effect->x;
                location[2] = effect->z;
                location[3] = effect->heading;
                location[4] = effect->pitch;
                location[5] = location[5] + effect->angleRate_guess;
                location[5] = EQ_METHOD(1, float, 0x00408156,
                    (CDisplay*, float), display)(location[5]);
                setActorResult = t3dSetActorLocation(display->world_guess, effect->actor, location);
                if (setActorResult == 0)
                {
                    if (effect->light != 0)
                        EQ_GLOBAL(t3dSetPointLightXYZ_guess_t, 0x006E5430)
                            (display->world_guess, effect->light, location);
                }
                else if (effect->attachedSource != 0)
                    goto destroy_effect;
            }
            else
            {
destroy_effect:
                if (effect != 0)
                {
                    EQ_METHOD(0, void, 0x00490EF4, (EQEffect_guess*), effect)();
                    EQ_CDECL(void, 0x005218BB, (void*))(effect);
                }
            }
            effect = nextEffect;
        }
    }
}

// FUNCTION: TRILOGY 0x0040dfb3
// Steps a rain/snow fade: moves the fog each tick; at full fade starts it (message, clouds, sound) or ends it.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: the createCloud pointer load (mov edi) sits at a different point in the rain and snow arms.
// Rain or snow step, run from UpdateWeather_guess through CDisplay_ProcessCloud_guess. weather_type 1 is rain,
// 2 is snow; c5 is 1 while starting and 0 while stopping. The header has no CDisplay method for it, so this is
// a free fastcall with the thiscall layout (as bodyEnvironmentChange). Bug kept: with a0 <= 0 the fog
// start is the fade's integer bits read back as a float.
/* fake match: the fog-start and d0 tests are inverted, !(x > K), so VC6 lays out the else arms first. */
void __fastcall ProcessCloud_guess(CDisplay* display, int edx)
{
    typedef void (__cdecl *t3dStopPCloudAndAutoDestroy_t)(void*, int);
    typedef void (__cdecl *t3dSetRainColor_t)(int);

    int step;
    int fadeAsInt;
    float location[3] = {0.0f, 0.0f, 0.0f};
    s16 fade;
    float fogStart;
    float fogEnd;
    u32 timeMs;
    t3dCreateParticleCloud_t createCloud;
    int cloud;
    void* soundManager;

    if (display->weather_fade_guess >= 0x100)
    {
        if (display->weather_c5_guess == 0)
        {
            *(char*)((char*)display + 0xC3) = 0;
            display->weather_active_guess = 0;
            if (display->weather_type_guess == 1)
            {
                EQ_GLOBAL(t3dStopPCloudAndAutoDestroy_t, 0x006E5460)(
                    display->world_guess, *(int*)((char*)display + 0xF4));
                *(u32*)((char*)display + 0xE0) = 0;
                soundManager = g_sound_manager_ptr_guess;
                if (soundManager != 0)
                    SoundManager_StopWave_guess(soundManager, 0x9F);
            }
            else if (display->weather_type_guess == 2)
            {
                EQ_GLOBAL(t3dStopPCloudAndAutoDestroy_t, 0x006E5460)(
                    display->world_guess, *(int*)((char*)display + 0x10C));
                *(u32*)((char*)display + 0xE4) = 0;
                soundManager = g_sound_manager_ptr_guess;
                if (soundManager != 0)
                    SoundManager_StopWave_guess(soundManager, 0x9E);
            }
        }
        else
        {
            *(char*)((char*)display + 0xC3) = 1;
            if (display->weather_type_guess == 1)
            {
                if (g_pc_guess != 0 && EXEC_STRCMP
                        (g_pc_guess->zone, (const char*)0x0053E210) == 0)
                {
                    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)((const char*)0x0053E1F4);
                    EQ_GLOBAL(t3dSetRainColor_t, 0x006E5434)(1);
                }
                else
                {
                    EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)((const char*)0x0053E1E0);
                    EQ_GLOBAL(t3dSetRainColor_t, 0x006E5434)(0);
                }
                createCloud = t3dCreateParticleCloud;
                *(int*)((char*)display + 0x104) = (int)createCloud(g_cdisplay_guess->world_guess,
                    display->rainCloudDef2_guess, location, 0, 0);
                cloud = (int)createCloud(g_cdisplay_guess->world_guess,
                    display->rainCloudDef1_guess, location, 0, 0);
                *(int*)((char*)display + 0xFC) = cloud;
                *(int*)((char*)display + 0xF4) = (int)createCloud(g_cdisplay_guess->world_guess,
                    display->rainCloudDef0_guess, location, *(int*)((char*)display + 0x104), cloud);
                *(u32*)((char*)display + 0xE0) = 1;
                soundManager = g_sound_manager_ptr_guess;
                if (soundManager != 0)
                    EQ_METHOD(2, void, 0x004E34FA, (void *, int, int), (void *)(soundManager))(0x9F, 0);
            }
            else if (display->weather_type_guess == 2)
            {
                EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)((const char*)0x0053E1CC);
                EQ_GLOBAL(t3dSetRainColor_t, 0x006E5434)(0);
                createCloud = t3dCreateParticleCloud;
                *(int*)((char*)g_cdisplay_guess + 0x11C) = (int)createCloud(g_cdisplay_guess->world_guess,
                    g_cdisplay_guess->snowCloudDef2_guess, location, 0, 0);
                *(int*)((char*)g_cdisplay_guess + 0x114) = (int)createCloud(g_cdisplay_guess->world_guess,
                    g_cdisplay_guess->snowCloudDef1_guess, location, 0, 0);
                *(int*)((char*)g_cdisplay_guess + 0x10C) = (int)createCloud(g_cdisplay_guess->world_guess,
                    g_cdisplay_guess->snowCloudDef0_guess, location,
                    *(int*)((char*)g_cdisplay_guess + 0x11C), *(int*)((char*)g_cdisplay_guess + 0x114));
                *(u32*)((char*)g_cdisplay_guess + 0xE4) = 1;
                soundManager = g_sound_manager_ptr_guess;
                if (soundManager != 0)
                    EQ_METHOD(2, void, 0x004E34FA, (void *, int, int), (void *)(soundManager))(0x9E, 0);
            }
        }
        display->weather_clouds_guess = 0;
    }
    else
    {
        timeMs = display->time_ms_guess;
        if (timeMs < display->weather_dc_time_guess)
            return;
        if (display->weather_c5_guess == 0)
        {
            display->weather_dc_time_guess = timeMs - 1;
            step = 3;
            if (0x100 < display->weather_fade_guess)
                display->weather_fade_guess = 0x100;
        }
        else
        {
            display->weather_dc_time_guess = timeMs + 1;
            step = 2;
            if (0xFF < display->weather_fade_guess)
                display->weather_fade_guess = 0xFF;
        }
        display->weather_fade_guess += (s16)step;
        fade = display->weather_fade_guess;
        if (display->weather_c5_guess == 1)
        {
            fadeAsInt = fade;
            fogEnd = 256.0f - (float)fadeAsInt;
            if (fogEnd < EQ_GLOBAL(float, 0x005344A4))
                fogEnd = 1.0f;
            if (fogEnd == EQ_GLOBAL(float, 0x005351B4))
            {
                display->weather_cc_guess = (display->fogStart_guess /
                    (256.0f / (float)step)) * EQ_GLOBAL(float, 0x006B6EF0);
                if (!(display->fogEnd_guess > EQ_GLOBAL(float, 0x005351B0)))
                    display->weather_d0_guess = 0.0f;
                else
                    display->weather_d0_guess = ((display->fogEnd_guess - EQ_GLOBAL(float, 0x005351B0)) /
                        (256.0f / (float)step)) * EQ_GLOBAL(float, 0x006B6EF0);
            }
            if (!(display->fogStart_guess > EQ_GLOBAL(float, 0x0053510C)))
            {
                display->fogStart_guess = 0.0f;
                fogStart = *(float*)&fadeAsInt;
            }
            else
            {
                fogStart = display->fogStart_guess - display->weather_cc_guess;
                if (fogStart < EQ_GLOBAL(float, 0x0053510C))
                    fogStart = 0.0f;
            }
            if (display->fogEnd_guess > EQ_GLOBAL(float, 0x005351B0))
                fogEnd = display->fogEnd_guess - display->weather_d0_guess;
            else
                fogEnd = display->fogEnd_guess;
            EQ_METHOD(6, void, 0x0040F518, (CDisplay *, int, float, float, char, char, char), display)(1, fogStart, fogEnd,
                 display->fogRed_guess, display->fogGreen_guess, display->fogBlue_guess);
        }
        else
        {
            if (fade == 1)
            {
                display->weather_cc_guess = display->weather_cc_guess * 1.5f;
                display->weather_d0_guess = display->weather_d0_guess * 1.5f;
            }
            EQ_METHOD(6, void, 0x0040F518, (CDisplay *, int, float, float, char, char, char), display)(1, display->weather_cc_guess + display->fogStart_guess,
                 display->weather_d0_guess + display->fogEnd_guess,
                 display->fogRed_guess, display->fogGreen_guess, display->fogBlue_guess);
        }
    }
}

// FUNCTION: TRILOGY 0x0040e3ea
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored; the first argument is unused.
void CDisplay::PlaySoundAtLocation_guess(int unused, int id, float y, float x, float z)
{
    if (g_environment_ptr_guess != 0)
        Environment_PlaySoundAt_guess(g_environment_ptr_guess, y, x, z, id);
}

// FUNCTION: TRILOGY 0x0040e418
// Presents the current 3D frame.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::ShowDisplay_guess()
{
    t3dUpdateDisplay(render_guess);
}

// FUNCTION: TRILOGY 0x0040e422
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The old view actor is made visible again. A failed camera move only logs.
void CDisplay::SetViewActor(void* actor)
{
    float location[7];
    int err;

    if (g_view_actor_guess != 0)
        t3dSetActorInvisible(g_view_actor_guess, 0);
    g_view_actor_guess = actor;
    if (actor != 0)
    {
        t3dGetActorLocation(actor, location);
        err = t3dSetCameraLocation(world_guess, camera_guess, location);
        if (err != 0)
            LogMessage_guess("* Bad Initial Camera Location %f, %f, %f, error_code = %d (SetViewActor)\n",
                             location[0], location[1], location[2], err);
    }
    unk_0x124 = 0;
}

// FUNCTION: TRILOGY 0x0040e49f
// Sets the display's yon distance and refreshes display settings.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetYon_guess(float yon)
{
    this->yon = yon;
    CDisplay_ChangeVideoMode_guess(this, 1);
}

// FUNCTION: TRILOGY 0x0040e4b0
// Sets the camera actor's yon distance directly.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetCameraYon_guess(float yon)
{
    s3dSetCameraActorYon(camera_guess, yon);
}

// FUNCTION: TRILOGY 0x0040e4c6
// Sets the display view angle and refreshes display settings.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetViewAngle_guess(short angle)
{
    view_angle_guess = (float)angle;
    CDisplay_ChangeVideoMode_guess(this, 1);
}

// FUNCTION: TRILOGY 0x0040e4e4
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Mode 1..4 -> resolution flag, the low word for 16-bit colour, the high word otherwise.
int CDisplay::SupportsVidMode(int mode)
{
    int supported = 0;

    switch (mode)
    {
    case 1:
        if (t3dDeviceSupportsResolution(g_bitsPerPixel_guess == 16 ? 0x00002 : 0x20000))
            supported = 1;
        break;
    case 2:
        if (t3dDeviceSupportsResolution(g_bitsPerPixel_guess == 16 ? 0x00004 : 0x40000))
            supported = 1;
        break;
    case 3:
        if (t3dDeviceSupportsResolution(g_bitsPerPixel_guess == 16 ? 0x00008 : 0x80000))
            supported = 1;
        break;
    case 4:
        if (t3dDeviceSupportsResolution(g_bitsPerPixel_guess == 16 ? 0x00010 : 0x100000))
            supported = 1;
        break;
    }
    return supported;
}

// FUNCTION: TRILOGY 0x0040e58b
// __fastcall on CDisplay (display, skipRendererCallbacks); applies the video mode.
// A switch on g_game_state_guess (cases 1-7, default) sets the 3D render rect in g_render_rect_guess and the
// mode in g_video_mode_guess, trying the requested resolution with t3dDeviceSupportsResolution and falling back
// to a default rect.
// The notifyRenderer tail then sets the camera lens through the pointer at 0x006e5398.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: prologue register allocation and switch branch targets still differ.
/* fake match: keep original case order/goto joins and case-5 fallback arms; rebase renderer rows and retain viewport intermediates for VC6 codegen. */
void __fastcall ChangeVideoMode_guess(CDisplay* displayThis, int, int skipRendererCallbacks)
{
    int* rendererRow;
    int result;
    int selectedMode;
    int defaultHeight;
    int videoMode;
    void* updateRects;
    void* rawTextBuffer;
    int rendererCallbacksHandled;
    int viewportHeight;
    float viewportHeightFloat;
    float aspectRatio;
    int (__cdecl *clearRenderToBlack)(int);
    s16* renderRect;
    int* renderer;
    u32 vtable;
    typedef void (__fastcall *RendererMethod_guess_t)(void*);
    RendererMethod_guess_t rendererMethod;
    typedef int (__cdecl *t3dSetCameraLens_guess_t)(void*, float, float, float, float);
    int* display;

    display = (int*)displayThis;
    renderRect = g_render_rect_guess;
    defaultHeight = 0x140;
    g_video_mode_guess = 0;
    EQ_GLOBAL(int, 0x006B6508) = 0xB;
    EQ_GLOBAL(int, 0x0057D3D0) = defaultHeight;
    rendererCallbacksHandled = skipRendererCallbacks;
    videoMode = EQ_GLOBAL(int, 0x006E20C0);

    switch (g_game_state_guess)
    {
    case 7:
    {
        if (EQ_GLOBAL(int, 0x006B6880) == 0)
            goto case7DefaultRect;
        selectedMode = 4;
        if (videoMode == 4)
        {
            result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000;
case7TryResolution:
            result = t3dDeviceSupportsResolution(result);
            if (result != 0)
            {
                renderRect[0] = 0x214;
                renderRect[1] = 0x20;
                renderRect[2] = 0x274;
                renderRect[3] = 0xE0;
                g_video_mode_guess = selectedMode;
                goto notifyRenderer;
            }
        }
        else
        {
            selectedMode = 3;
            if (videoMode == 3)
            {
                result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000;
                goto case7TryResolution;
            }
            if (videoMode == 2)
            {
                result = t3dDeviceSupportsResolution(
                    ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                if (result != 0)
                {
                    renderRect[0] = 0x214;
                    renderRect[1] = 0x20;
                    renderRect[2] = 0x274;
                    renderRect[3] = 0xE0;
                    goto storeMode2;
                }
            }
            else
            {
                selectedMode = 1;
                if (videoMode == 1)
                {
                    result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000;
                    goto case7TryResolution;
                }
            }
        }
case7DefaultRect:
        renderRect[0] = 0x214;
        renderRect[1] = 0x20;
        renderRect[2] = 0x274;
        renderRect[3] = 0xE0;
        goto notifyRenderer;
    }
    break;
    case 6:
    {
        if (EQ_GLOBAL(int, 0x006B6880) == 0)
            goto case6DefaultRect;
        selectedMode = 4;
        if (videoMode == 4)
        {
            result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000;
case6TryResolution:
            result = t3dDeviceSupportsResolution(result);
            if (result != 0)
            {
                renderRect[0] = 0x75;
                renderRect[1] = 0x14;
                renderRect[2] = 0x175;
                renderRect[3] = 0x194;
                g_video_mode_guess = selectedMode;
                goto notifyRenderer;
            }
        }
        else
        {
            selectedMode = 3;
            if (videoMode == 3)
            {
                result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000;
                goto case6TryResolution;
            }
            if (videoMode == 2)
            {
                result = t3dDeviceSupportsResolution(
                    ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                if (result != 0)
                {
                    renderRect[0] = 0x75;
                    renderRect[1] = 0x14;
                    renderRect[2] = 0x175;
                    renderRect[3] = 0x194;
storeMode2:
                    g_video_mode_guess = 2;
                    goto notifyRenderer;
                }
            }
            else
            {
                selectedMode = 1;
                if (videoMode == 1)
                {
                    result = ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000;
                    goto case6TryResolution;
                }
            }
        }
case6DefaultRect:
        renderRect[0] = 0x75;
        renderRect[1] = 0x14;
        renderRect[2] = 0x175;
        renderRect[3] = 0x194;
        goto notifyRenderer;
    }
    break;
    case 5:
    {
        if (EQ_GLOBAL(int, 0x006B6880) == 0)
        {
            renderRect[0] = 0x164;
            renderRect[1] = 0x1B;
            renderRect[2] = 0x264;
            renderRect[3] = 0x19B;
        }
        else if (videoMode == 4)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            if (result != 0)
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
                g_video_mode_guess = 4;
            }
            else
            {
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
            }
        }
        else if (videoMode == 3)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            if (result != 0)
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
                g_video_mode_guess = 3;
            }
            else
            {
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
            }
        }
        else if (videoMode == 2)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
            if (result != 0)
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
                g_video_mode_guess = 2;
            }
            else
            {
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
            }
        }
        else if (videoMode == 1)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000);
            if (result != 0)
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000);
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
                g_video_mode_guess = 1;
            }
            else
            {
                renderRect[0] = 0x164;
                renderRect[1] = 0x1B;
                renderRect[2] = 0x264;
                renderRect[3] = 0x19B;
            }
        }
        else
        {
            renderRect[0] = 0x164;
            renderRect[1] = 0x1B;
            renderRect[2] = 0x264;
            renderRect[3] = 0x19B;
        }
        if (EQ_GLOBAL(int, 0x0053C6B0) == 2)
        {
            clearRenderToBlack = t3dClearRenderToBlack;
            (*clearRenderToBlack)(0);
            t3dUpdateDisplay((void*)display[0]);
            (*clearRenderToBlack)(0);
        }
        goto notifyRenderer;
    }
    break;
    case 4:
    {
        renderRect[0] = 0x48;
        renderRect[2] = 0x15A;
        goto setRectTopBottom;
    }
    break;
    case 3:
    {
        if (videoMode == 4)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            if (result == 0)
                goto case3Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 0x640;
            EQ_GLOBAL(int, 0x006B6400) = 0x4B0;
            renderRect[2] = 0x640;
            renderRect[3] = 0x4B0;
        }
        else if (videoMode == 3)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            if (result == 0)
                goto case3Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 0x500;
            EQ_GLOBAL(int, 0x006B6400) = 0x400;
            renderRect[2] = 0x500;
            renderRect[3] = 0x400;
        }
        else if (videoMode == 2)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
            if (result == 0)
            {
case3Fallback640x480:
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFF0001) + 0x10000);
                renderRect[0] = 0;
                renderRect[1] = 0;
                EQ_GLOBAL(int, 0x006B63FC) = 0x280;
                EQ_GLOBAL(int, 0x006B6400) = 0x1E0;
                renderRect[2] = 0x280;
                renderRect[3] = 0x1E0;
            }
            else
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                renderRect[0] = 0;
                renderRect[1] = 0;
                EQ_GLOBAL(int, 0x006B63FC) = 0x400;
                EQ_GLOBAL(int, 0x006B6400) = 0x300;
                renderRect[2] = 0x400;
                renderRect[3] = 0x300;
            }
        }
        else
        {
            if ((videoMode != 1)
                || ((result = t3dDeviceSupportsResolution(
                    ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000)), result == 0))
                goto case3Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 800;
            EQ_GLOBAL(int, 0x006B6400) = 600;
            renderRect[2] = 800;
            renderRect[3] = 600;
        }
        goto notifyRenderer;
    }
    break;
    case 2:
    {
        if (videoMode == 4)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            if (result == 0)
                goto case2Fallback640x480;
            rendererRow = EQ_GLOBAL(int*, 0x006E4E44);
            rendererRow = rendererRow + (0x241BD - 4);
            rendererRow[4] = 0;
            EQ_GLOBAL(int, 0x0057D3D0) = rendererRow[2] - 0x28;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 0x640;
            EQ_GLOBAL(int, 0x006B6400) = 0x4B0;
            renderRect[2] = 0x640;
            renderRect[3] = 0x4B0;
        }
        else if (videoMode == 3)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            if (result == 0)
                goto case2Fallback640x480;
            rendererRow = EQ_GLOBAL(int*, 0x006E4E44);
            rendererRow = rendererRow + (0x241B5 - 4);
            rendererRow[4] = 0;
            EQ_GLOBAL(int, 0x0057D3D0) = rendererRow[2] - 0x28;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 0x500;
            EQ_GLOBAL(int, 0x006B6400) = 0x400;
            renderRect[2] = 0x500;
            renderRect[3] = 0x400;
        }
        else if (videoMode == 2)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
            if (result == 0)
            {
case2Fallback640x480:
                rendererRow = EQ_GLOBAL(int*, 0x006E4E44);
                rendererRow = rendererRow + (0x2419D - 4);
                rendererRow[4] = 0;
                EQ_GLOBAL(int, 0x0057D3D0) = rendererRow[2] - 0x28;
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFF0001) + 0x10000);
                EQ_GLOBAL(int, 0x006B63FC) = 0x280;
                EQ_GLOBAL(int, 0x006B6400) = 0x1E0;
                renderRect[0] = 0;
                renderRect[1] = 0;
                renderRect[2] = 0x280;
                renderRect[3] = 0x1E0;
            }
            else
            {
                rendererRow = EQ_GLOBAL(int*, 0x006E4E44);
                rendererRow = rendererRow + (0x241AD - 4);
                rendererRow[4] = 0;
                EQ_GLOBAL(int, 0x0057D3D0) = rendererRow[2] - 0x28;
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                renderRect[0] = 0;
                renderRect[1] = 0;
                EQ_GLOBAL(int, 0x006B63FC) = 0x400;
                EQ_GLOBAL(int, 0x006B6400) = 0x300;
                renderRect[2] = 0x400;
                renderRect[3] = 0x300;
            }
        }
        else
        {
            if ((videoMode != 1)
                || ((result = t3dDeviceSupportsResolution(
                    ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000)),
                    result == 0))
                goto case2Fallback640x480;
            rendererRow = EQ_GLOBAL(int*, 0x006E4E44);
            rendererRow = rendererRow + (0x241A5 - 4);
            rendererRow[4] = 0;
            EQ_GLOBAL(int, 0x0057D3D0) = rendererRow[2] - 0x28;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000);
            renderRect[0] = 0;
            renderRect[1] = 0;
            EQ_GLOBAL(int, 0x006B63FC) = 800;
            EQ_GLOBAL(int, 0x006B6400) = 600;
            renderRect[2] = 800;
            renderRect[3] = 600;
        }
        if (EQ_GLOBAL(int*, 0x006E4E44) != 0 && skipRendererCallbacks == 0)
        {
            vtable = *(u32*)EQ_GLOBAL(int*, 0x006E4E44);
            rendererMethod = (RendererMethod_guess_t)*(u32*)(vtable + 8);
            rendererCallbacksHandled = 1;
            (*rendererMethod)(EQ_GLOBAL(int*, 0x006E4E44));
            *((u8*)((u32)EQ_GLOBAL(int*, 0x006E4E44) + 0xFE0)) = 0;
            vtable = *(u32*)EQ_GLOBAL(int*, 0x006E4E44);
            rendererMethod = (RendererMethod_guess_t)*(u32*)(vtable + 4);
            (*rendererMethod)(EQ_GLOBAL(int*, 0x006E4E44));
        }
        goto notifyRenderer;
    }
    break;
    case 1:
        if (EQ_GLOBAL(int, 0x006B6880) != 0)
        {
        if (videoMode == 4)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            if (result == 0)
                goto case1Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF00010) + 0x100000);
            EQ_GLOBAL(int, 0x006B63FC) = 0x640;
            EQ_GLOBAL(int, 0x006B6400) = 0x4B0;
            renderRect[1] = 0;
            renderRect[0] = 0x77;
            renderRect[2] = 0x207;
            renderRect[3] = 0x140;
            g_video_mode_guess = 4;
        }
        else if (videoMode == 3)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            if (result == 0)
                goto case1Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFF80008) + 0x80000);
            EQ_GLOBAL(int, 0x006B63FC) = 0x500;
            EQ_GLOBAL(int, 0x006B6400) = 0x400;
            renderRect[1] = 0;
            renderRect[0] = 0x77;
            renderRect[2] = 0x207;
            renderRect[3] = 0x140;
            g_video_mode_guess = 3;
        }
        else if (videoMode == 2)
        {
            result = t3dDeviceSupportsResolution(
                ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
            if (result == 0)
            {
case1Fallback640x480:
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFF0001) + 0x10000);
                EQ_GLOBAL(int, 0x006B63FC) = 0x280;
                EQ_GLOBAL(int, 0x006B6400) = 0x1E0;
                renderRect[1] = 0;
                renderRect[0] = 0x77;
                renderRect[2] = 0x207;
                renderRect[3] = 0x140;
            }
            else
            {
                EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFC0004) + 0x40000);
                EQ_GLOBAL(int, 0x006B63FC) = 0x400;
                EQ_GLOBAL(int, 0x006B6400) = 0x300;
                renderRect[1] = 0;
                renderRect[0] = 0x77;
                renderRect[2] = 0x207;
                renderRect[3] = 0x140;
                g_video_mode_guess = 2;
            }
        }
        else
        {
            if ((videoMode != 1)
                || ((result = t3dDeviceSupportsResolution(
                    ((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000)), result == 0))
                goto case1Fallback640x480;
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFE0002) + 0x20000);
            EQ_GLOBAL(int, 0x006B63FC) = 800;
            EQ_GLOBAL(int, 0x006B6400) = 600;
            renderRect[1] = 0;
            renderRect[0] = 0x77;
            renderRect[2] = 0x207;
            renderRect[3] = 0x140;
            g_video_mode_guess = 1;
        }
        if ((EQ_GLOBAL(int, 0x0053C6B0) == 2) || (EQ_GLOBAL(int, 0x0053C6B0) == 3))
        {
            clearRenderToBlack = t3dClearRenderToBlack;
            (*clearRenderToBlack)(0);
            t3dUpdateDisplay((void*)display[0]);
            (*clearRenderToBlack)(0);
            if (EQ_GLOBAL(int*, 0x006E4E44) != 0)
                EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
        }
        if (EQ_GLOBAL(int, 0x0053C6B0) != 7)
        {
            if (EQ_GLOBAL(int*, 0x006E4E44) == 0)
                goto updateCameraLens;
            EQ_GLOBAL(int*, 0x006E4E44)[0x3AE] = 1;
        }
        }
        else
        {
            EQ_METHOD(1, void, 0x0040E56E, (CDisplay*, int), displayThis)(((g_bitsPerPixel_guess != 0x10) - 1 & 0xFFFF0001) + 0x10000);
            EQ_GLOBAL(int, 0x006B63FC) = 0x280;
            EQ_GLOBAL(int, 0x006B6400) = 0x1E0;
            goto defaultRect;
        }
        goto notifyRenderer;
        break;
    default:
        goto defaultRect;
    }
defaultRect:
    renderRect[0] = 0x77;
    renderRect[2] = 0x207;
setRectTopBottom:
    renderRect[1] = 0;
    renderRect[3] = defaultHeight;

notifyRenderer:
    renderer = EQ_GLOBAL(int*, 0x006E4E44);
    if (renderer != 0 && rendererCallbacksHandled == 0)
    {
        vtable = *(u32*)renderer;
        rendererMethod = (RendererMethod_guess_t)*(u32*)(vtable + 8);
        (*rendererMethod)(renderer);
        if (g_game_state_guess == 2)
            *((u8*)((u32)EQ_GLOBAL(int*, 0x006E4E44) + 0x8FBAC)) = 0;
        vtable = *(u32*)EQ_GLOBAL(int*, 0x006E4E44);
        rendererMethod = (RendererMethod_guess_t)*(u32*)(vtable + 4);
        (*rendererMethod)(EQ_GLOBAL(int*, 0x006E4E44));
    }

updateCameraLens:
    updateRects = g_3dview_guess;
    if (updateRects != 0)
        EQ_METHOD(0, void, 0x004155FE, (void*), updateRects)();
    viewportHeight = (int)renderRect[3] - (int)renderRect[1];
    viewportHeightFloat = (float)viewportHeight;
    aspectRatio = (float)((int)renderRect[2] - (int)renderRect[0]) / viewportHeightFloat;
    result = EQ_GLOBAL(t3dSetCameraLens_guess_t, 0x006E5398)
        (displayThis->camera_guess, displayThis->view_angle_guess,
        aspectRatio,
        *(&displayThis->nearClip_guess), displayThis->yon);
    display[3] = result;
    if (result == 0)
    {
        EQ_METHOD(0, void, 0x00406399, (CDisplay*), displayThis)();
        if (((g_game_state_guess != EQ_GLOBAL(int, 0x0053C6B0))
            && (g_game_state_guess != 7))
            && (EQ_GLOBAL(int, 0x0053C6B0) != 7)
            && ((*((int*)((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 4)) == 5
                && ((rawTextBuffer = EQ_GLOBAL(void*, 0x006E4DE8)) != 0))
                && (g_no_render_offset_guess == '\0')))
        {
            EQ_METHOD(0, void, 0x0048CF56, (void*), rawTextBuffer)();
        }
        display[3] = 0;
        EQ_GLOBAL(int, 0x0053C6B0) = g_game_state_guess;
    }
    return;
}

// FUNCTION: TRILOGY 0x0040f47f
// Cycles view presets; intoxication or an unnamed spell effect can override the chosen angle.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay_guess::ToggleView(void)
{
    s16 angle;

    if (g_pc_guess->intoxication >= 0x46)
    {
        s16 converted_angle;

        g_activeView_guess = 0;
        converted_angle = (s16)(int)EQ_ARRAY(float, 0x006B6F2C)[0];
        converted_angle += g_pc_guess->intoxication;
        converted_angle -= 0x46;
        angle = converted_angle;
    }
    else
    {
        if (g_activeView_guess == 3 || g_activeView_guess == 4)
            EQ_FUNC(void, __fastcall, 0x004CF31A, (CEverQuest*))(pinstCEverQuest_guess);

        g_activeView_guess = g_activeView_guess + 1;
        if (g_activeView_guess < 5 ||
            (g_activeView_guess = 0, g_pc_guess == NULL) ||
            ((angle = EQ_METHOD(1, int, 0x004238A2, (void*, int), (void*)g_pc_guess)(0x57)), (short)angle <= 0))
        {
            angle = (int)EQ_ARRAY(float, 0x006B6F2C)[g_activeView_guess * 7];
        }
    }

    EQ_METHOD(1, void, 0x0040E4C6, (CDisplay*, short), g_cdisplay_guess)(angle);
    return;
}

// FUNCTION: TRILOGY 0x0040f518
// Sets fog mode, start/end and color (black becomes gray), blending the end by a global factor; syncs yon.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: end's x87 copy is reloaded at the second && test, the join keeps fogInterpolationRange/fogInterpolationProgress (no pops), and end < 101 is scheduled after the colour stores.
/* fake match: fEnd and fogInterpolationRange/fogInterpolationProgress are FPU-resident locals, so end stays unrounded on the x87 stack as in the original. */
void CDisplay_guess::SetFog(int mode, float start, float fogEnd,
                                  char red, char green, char blue)
{
    double fogInterpolationRange;
    double fogInterpolationProgress;
    float fEnd;
    int allowFogInterpolation = 1;
    typedef void (__cdecl *t3dSetFog_guess_t)(int, float, float, char, char, char);

    if (*((char*)this + 0x10) != '\0')
    {
        if (EQ_GLOBAL(char, 0x0058FBB8) == 5 || EQ_GLOBAL(char, 0x0058FBB8) == 6 ||
            EQ_GLOBAL(char, 0x0058FBB8) == 7 || EQ_GLOBAL(char, 0x0058FBB8) == 9 ||
            EQ_GLOBAL(char, 0x0058FBB8) == 10 || EQ_GLOBAL(char, 0x0058FBB8) == 8)
        {
            allowFogInterpolation = 0;
        }
        fEnd = fogEnd;
        if (start < EQ_GLOBAL(float, 0x005351C4) && fEnd < EQ_GLOBAL(float, 0x005351C4))
        {
            start = *((float*)((char*)this + 0x18)) - EQ_GLOBAL(float, 0x00535190);
            fEnd = *((float*)((char*)this + 0x18)) - EQ_GLOBAL(float, 0x005344A4);
        }
        if (start < EQ_GLOBAL(float, 0x0053510C))
        {
            start = 0.0f;
        }
        if (fEnd < start)
        {
            fEnd = start + EQ_GLOBAL(float, 0x00535190);
        }
        if (red == '\0' && green == '\0' && blue == '\0')
        {
            red = -0x40;
            green = -0x40;
            blue = -0x40;
        }
        if (EQ_GLOBAL(int, 0x006B6870) == 0)
        {
            *((u8*)((char*)this + 0x12)) = 0;
        }
        else
        {
            *((u8*)((char*)this + 0x12)) = EQ_ARRAY(u8, 0x006B66E0)[0x46];
        }
        if (mode != 0)
        {
            if (EQ_GLOBAL(float, 0x006B6EF0) != EQ_GLOBAL(float, 0x005344A4) &&
                allowFogInterpolation == 1 && *((char*)this + 0xc0) == '\0')
            {
                start = fEnd - start;
                if (EQ_GLOBAL(float, 0x006B6EF0) > EQ_GLOBAL(float, 0x005344A4))
                {
                    fEnd = EQ_GLOBAL(float, 0x006B66E0);
                    fogInterpolationRange = (EQ_GLOBAL(float, 0x006B674C) - fEnd) * EQ_GLOBAL(float, 0x005351C0);
                    fogInterpolationProgress = EQ_GLOBAL(float, 0x006B6EF0) - EQ_GLOBAL(float, 0x005344A4);
                }
                else
                {
                    fEnd = EQ_GLOBAL(float, 0x006B6748);
                    fogInterpolationRange = EQ_GLOBAL(float, 0x006B66E0) - fEnd;
                    fogInterpolationProgress = EQ_GLOBAL(float, 0x006B6EF0) - EQ_GLOBAL(float, 0x005351C4);
                }
                fEnd = fEnd + fogInterpolationProgress * fogInterpolationRange;
                start = fEnd - start;
                if (start < EQ_GLOBAL(float, 0x0053510C))
                {
                    start = 0.0f;
                }
            }
            *((int*)((char*)this + 0x9c)) = mode;
            if (allowFogInterpolation && mode == 1)
            {
                *((float*)((char*)this + 0xa4)) = fEnd;
                *((float*)((char*)this + 0xa0)) = start;
            }
            *((char*)this + 0xa8) = red;
            *((char*)this + 0xa9) = green;
            *((char*)this + 0xaa) = blue;
            if (fEnd < EQ_GLOBAL(float, 0x005351BC))
            {
                *((u8*)((char*)this + 0x12)) = 0;
            }
            fogEnd = fEnd;
        }
        else
        {
            fogEnd = 600.0f;
        }
        if (*((float*)((char*)this + 0x18)) != fogEnd)
        {
            EQ_FASTCALL(void, 0x0040E49F, (CDisplay_guess*, float))(this, fogEnd);
        }
        EQ_GLOBAL(t3dSetFog_guess_t, 0x006E53E0)(mode, start, fogEnd, red, green, blue);
    }
    return;
}

// FUNCTION: TRILOGY 0x0040f6fe
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The original loads the t3dClearRenderToBlack pointer once and calls it twice.
void CDisplay::ClearScreen()
{
    t3dClearRenderToBlack_t clear = t3dClearRenderToBlack;

    clear(0);
    t3dUpdateDisplay(render_guess);
    clear(0);
}

// FUNCTION: TRILOGY 0x0040f71e
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored.
void CDisplay::OverlayClear_guess(void* overlay, int arg)
{
    t3dOverlayClear(overlay, arg);
}

/* fake match: empty guard whose EH state around the node constructor call stands in for the original `new` */
struct ActorInfoNodeGuard_guess
{
    ~ActorInfoNodeGuard_guess() {}
};

// FUNCTION: TRILOGY 0x0040f731
// Builds a spawn's 3D actor: model (HUM/HUF fallback), per-race size, valid placement, stance animation.
// @early-stop: frame 0xe4 vs 0xe0 (track float slots, no EH new temp); guard + explicit call stand in for `new`
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
/* fake match: track-transform floats are shared function-level temps and the node `new` is a guard; both only shape the frame/EH */
void __fastcall CreatePlayerActor(CDisplay* display, int unusedEdx, EQPlayer* player)
{
    typedef void* (__cdecl *GetDictionaryPointer_guess_t)(void*, const char*);
    typedef int (__cdecl *GetRegion_guess_t)(void*, float*);
    typedef void (__cdecl *ComputeBoundingRadius_guess_t)(void*, float*);
    typedef void* (__cdecl *GetDefaultInstance_guess_t)(void*);
    typedef void* (__cdecl *GetActorSprite_guess_t)(void*, int, int);
    typedef int (__cdecl *IsHierarchicalSpriteValid_guess_t)(void*);
    typedef void* (__cdecl *GetHierarchicalSpriteDefinition_guess_t)(void*);
    typedef void (__cdecl *SetSpriteBoundingRadius_guess_t)(void*, float);
    typedef void* (__cdecl *CreateSphere_guess_t)(float);
    typedef void (__cdecl *DestroySphere_guess_t)(void*);
    typedef void* (__cdecl *GetTrackFromDag_guess_t)(void*);
    typedef void* (__cdecl *GetTrackDefinition_guess_t)(void*);
    typedef void (__cdecl *GetTrackTransformation_guess_t)(void*, int, float*, float*, float*, float*, float*);
    typedef void (__cdecl *SetTrackTransformation_guess_t)(void*, int, float, float, float, float, float*);
    typedef void (__cdecl *SetActorScale_guess_t)(void*, float, int);
    typedef void (__cdecl *SetActorLocation_guess_t)(void*, void*, float*);
    typedef void (__cdecl *SetActorUserData_guess_t)(void*, void*);
    typedef void (__cdecl *SetActorInvisible_guess_t)(void*, int);
    typedef void* (__cdecl *CreateActorEx_guess_t)(void*, void*, int, float*, void*, int, float, float, int, int, EQPlayer*);
    typedef void* (__cdecl *InitializeActorInfoNode_guess_t)(char*, void*, EQPlayer*);

    if (player == 0)
        return;

    if (display->is3dOn_guess != 0)
    {
        char actorTag[128];
        float actorLocation[7];
        float actorRadius;
        void* actorDefinition;
        GetDictionaryPointer_guess_t getDictionaryPointer;
        GetRegion_guess_t getRegion;

        if (player->race == 0x8f || player->race == 0x8e)
        {
            EQ_METHOD(3, void, 0x0040C17D, (CDisplay*, char*, int, EQPlayer*), g_cdisplay_guess)
                (actorTag, player->race == 0x8f ? 8 : 7, player);
            EQ_CDECL(void, 0x004A0841, (char*, const char*, int))
                ((char*)player->actor_info + 8, actorTag, 0x1d);
        }

        getDictionaryPointer = EQ_GLOBAL(GetDictionaryPointer_guess_t, 0x006E52E0);
        actorDefinition = getDictionaryPointer(display->world_guess, (char*)player->actor_info + 8);
        if (actorDefinition == 0)
        {
            if (player->gender == 1)
                SOC_CPY
                    ((char*)player->actor_info + 8, (const char*)&EQ_GLOBAL(char, 0x0053E2BC));
            else
                SOC_CPY
                    ((char*)player->actor_info + 8, (const char*)&EQ_GLOBAL(char, 0x0053E2AC));

            actorDefinition = getDictionaryPointer(display->world_guess, (char*)player->actor_info + 8);
            if (actorDefinition == 0)
            {
                LogMessage_guess
                    ((const char*)&EQ_GLOBAL(char, 0x0053E268), (char*)player->actor_info + 8, player->name);
                EQ_CDECL(void, 0x004D7D6A, (int, int, int))(0xcb, 0, 0);
                return;
            }
        }

        actorLocation[0] = player->y;
        actorLocation[1] = player->x;
        actorLocation[2] = player->z;
        actorLocation[3] = player->heading;
        actorLocation[4] = player->pitch;
        actorLocation[5] = 0.0f;
        getRegion = EQ_GLOBAL(GetRegion_guess_t, 0x006E52E8);
        if (getRegion(display->world_guess, actorLocation) == 0)
        {
            actorLocation[2] += 2.0f;
            if (getRegion(display->world_guess, actorLocation) == 0)
            {
                actorLocation[2] += 2.0f;
                if (getRegion(display->world_guess, actorLocation) == 0)
                {
                    actorLocation[2] += 2.0f;
                    if (getRegion(display->world_guess, actorLocation) == 0)
                    {
                        actorLocation[2] += 2.0f;
                        if (getRegion(display->world_guess, actorLocation) == 0)
                        {
                            actorLocation[0] = EQ_GLOBAL(float, 0x006B6734);
                            actorLocation[1] = EQ_GLOBAL(float, 0x006B6738);
                            actorLocation[2] = EQ_GLOBAL(float, 0x006B673C);
                        }
                    }
                }
            }
            player->y = actorLocation[0];
            player->x = actorLocation[1];
            player->z = actorLocation[2];
        }

        actorRadius = 0.0f;
        EQ_GLOBAL(ComputeBoundingRadius_guess_t, 0x006E50AC)(actorDefinition, &actorRadius);
        if (actorRadius < 1.0f)
            actorRadius = 1.0f;

        switch (player->race)
        {
        case 0x2e:
            actorRadius = 11.5f;
            break;
        case 0x31:
            actorRadius = 116.3f;
            break;
        case 0x55:
            actorRadius = 12.1f;
            break;
        case 0x4f:
            actorRadius = actorRadius * 5.0f;
            break;
        case 0x48:
            if (player->actor_info->unk_0x244 == 1)
                actorRadius = actorRadius + actorRadius;
            break;
        case 0x45:
            actorRadius = 20.0f;
            break;
        case 0x71:
            actorRadius = 40.0f;
            break;
        case 0xb7:
            actorRadius = 3.0f;
            break;
        case 0x1f:
        case 0x7d:
        case 0xae:
        case 0xbc:
        case 0xbd:
            actorRadius = actorRadius * 3.0f;
            break;
        case 0x15:
        case 0x16:
        case 0x1e:
        case 0x22:
        case 0x26:
        case 0x34:
        case 0x56:
        case 0x58:
        case 0x59:
        case 0x5b:
        case 100:
        case 0x6d:
        case 0x6f:
        case 0x9a:
        case 0xf4:
        case 0xf6:
        case 0xf7:
        case 0xfa:
        case 0xfb:
            actorRadius = actorRadius + actorRadius;
            break;
        }

        if (EQ_GLOBAL(void*, 0x006E4D68) == 0
            || EQ_METHOD(1, int, 0x004A01CC, (void*, const char*), EQ_GLOBAL(void*, 0x006E4D68))(
                   (char*)player->actor_info + 8) == 0)
        {
            void* actorDefinitionInstance =
                EQ_GLOBAL(GetDefaultInstance_guess_t, 0x006E5278)(actorDefinition);
            if (actorDefinitionInstance != 0)
            {
                void* actorSprite = EQ_GLOBAL(GetActorSprite_guess_t, 0x006E5288)
                    (actorDefinitionInstance, 0, 0);
                if (EQ_GLOBAL(IsHierarchicalSpriteValid_guess_t, 0x006E5330)(actorSprite) != 0)
                {
                    actorDefinitionInstance =
                        EQ_GLOBAL(GetHierarchicalSpriteDefinition_guess_t, 0x006E52CC)(actorSprite);
                    EQ_GLOBAL(SetSpriteBoundingRadius_guess_t, 0x006E53E8)
                        (actorDefinitionInstance, actorRadius);
                }
            }
        }

        float sphereRadius = player->height_offset * 0.5f;
        int sphereDescription = 0x2e;
        void* sphere;
        if (player->type == 1)
        {
            sphereRadius = sphereRadius * 0.35f;
            if (player->race == 0x1f)
                sphereRadius = sphereRadius * 0.9f;
            EQ_CDECL(int, 0x004A42AA, (EQPlayer*))(player);
            if (sphereRadius < 1.5f)
                sphereRadius = 1.5f;
            if (EQ_METHOD(1, int, 0x004B4531, (EQPlayer*, int), player)(-1) != 0)
                sphereRadius = 1.2f;
        }
        else
        {
            switch (player->race)
            {
            case 0xc:
                sphereRadius = sphereRadius + sphereRadius;
                break;
            case 10:
                sphereRadius = sphereRadius * 0.8f;
                break;
            case 9:
                sphereRadius = sphereRadius * 0.9f;
                break;
            case 0xb:
            case 8:
                sphereRadius = sphereRadius * 1.5f;
                break;
            }
        }

        if (player->race < 0xf4 && player->body_type < 0x41)
        {
            if (player->race == 0x48 || player->race == 0x8d || player->race == 0x49 || player->race == 0x72)
                sphere = &sphereDescription;
            else
                sphere = EQ_GLOBAL(CreateSphere_guess_t, 0x006E51A4)(sphereRadius);
        }
        else
            sphere = 0;

        display->unk_0x124 = 0;
        player->actor_info->actor = EQ_CDECL(void*, 0x00405DBF,
            (void*, void*, int, float*, void*, int, float, float, int, int, EQPlayer*))
            (display->world_guess, actorDefinition, 0, actorLocation, sphere, 0, 1.0f,
             actorRadius, 0, 0, player);
        if (sphere != 0 && sphere != &sphereDescription)
            EQ_GLOBAL(DestroySphere_guess_t, 0x006E5224)(sphere);

        if (player->actor_info->actor != 0
            && (player->race >= 0xf4
                || ((player->race != 0x8d && player->race != 0x8e && player->race != 0x8f)
                    && player->race != 0x49 && player->race != 0x72 && player->race < 0xd6)))
        {
            if (DAT_006b6494 != 0)
                EQ_METHOD(1, void, 0x0040E422, (CDisplay*, void*), display)
                    (g_local_player_guess->actor_info->actor);

            player->actor_info->hier_sprite =
                EQ_METHOD(1, void*, 0x0041176D, (CDisplay*, void*), display)(player->actor_info->actor);
            if (EQ_GLOBAL(void*, 0x006E4D68) != 0)
                player->actor_info->anim_set_guess = (char*)EQ_METHOD(1, int, 0x004A01CC,
                    (void*, const char*), EQ_GLOBAL(void*, 0x006E4D68))((char*)player->actor_info + 8);
            else
                player->actor_info->anim_set_guess = 0;

            if (player->actor_info->anim_set_guess == 0)
            {
                void* node = EQ_CDECL(void*, 0x005218C6, (unsigned int))(0x3a0);
                {
                    ActorInfoNodeGuard_guess guard;
                    if (node != 0)
                        node = EQ_METHOD(3, void*, 0x004A00F5, (void*, char*, void*, EQPlayer*), node)(
                            (char*)player->actor_info + 8, player->actor_info->actor, player);
                }
                player->actor_info->anim_set_guess = (char*)node;
            }

            EQ_METHOD(1, void, 0x00410116, (CDisplay*, EQPlayer*), display)(player);

            /* track transform out-values: scale, three floats, a 3-vector; reused by both race groups */
            float scale, a, b, c, d[3];
            float e, f, g, h, i[3];
            float v;
            switch (player->race)
            {
            case 0x59:
                if (player->actor_info->attach_a_guess[0] != 0 && player->actor_info->attach_a_guess[1] != 0)
                {
                    GetTrackFromDag_guess_t getTrackFromDag = EQ_GLOBAL(GetTrackFromDag_guess_t, 0x006E52BC);
                    GetTrackDefinition_guess_t getTrackDefinition = EQ_GLOBAL(GetTrackDefinition_guess_t, 0x006E52FC);
                    void* trackDefinitionA = getTrackDefinition(getTrackFromDag(player->actor_info->attach_a_guess[1]));
                    void* trackDefinitionB = getTrackDefinition(getTrackFromDag(player->actor_info->attach_a_guess[0]));
                    GetTrackTransformation_guess_t getTrackTransformation = EQ_GLOBAL(GetTrackTransformation_guess_t, 0x006E5300);
                    SetTrackTransformation_guess_t setTrackTransformation;
                    getTrackTransformation(trackDefinitionB, 0, &v, &scale, &f, &g, i);
                    getTrackTransformation(trackDefinitionA, 0, &v, &scale, &c, &g, i);
                    setTrackTransformation = EQ_GLOBAL(SetTrackTransformation_guess_t, 0x006E5454);
                    if (f == 0.0f)
                        setTrackTransformation(trackDefinitionB, 0, v, scale, 112.0f, g, i);
                    if (c == 0.0f)
                        setTrackTransformation(trackDefinitionA, 0, v, scale, 112.0f, g, i);
                }
                break;
            case 2:
            case 8:
            case 9:
            case 10:
            case 0xb:
                if (player->actor_info->attach_a_guess[0] != 0 && player->actor_info->attach_a_guess[1] != 0)
                {
                    GetTrackFromDag_guess_t getTrackFromDag = EQ_GLOBAL(GetTrackFromDag_guess_t, 0x006E52BC);
                    GetTrackDefinition_guess_t getTrackDefinition = EQ_GLOBAL(GetTrackDefinition_guess_t, 0x006E52FC);
                    void* trackDefinitionA = getTrackDefinition(getTrackFromDag(player->actor_info->attach_a_guess[1]));
                    void* trackDefinitionB = getTrackDefinition(getTrackFromDag(player->actor_info->attach_a_guess[0]));
                    GetTrackTransformation_guess_t getTrackTransformation = EQ_GLOBAL(GetTrackTransformation_guess_t, 0x006E5300);
                    getTrackTransformation(trackDefinitionB, 0, &scale, &a, &b, &c, d);
                    getTrackTransformation(trackDefinitionA, 0, &e, &f, &g, &h, i);
                    if (player->race == 8)
                    {
                        SetTrackTransformation_guess_t setTrackTransformation = EQ_GLOBAL(SetTrackTransformation_guess_t, 0x006E5454);
                        if (player->gender == 0)
                            scale = 1.0f;
                        else
                            scale = 1.2f;
                        setTrackTransformation(trackDefinitionB, 0, scale, a, b, c, d);
                        setTrackTransformation(trackDefinitionA, 0, scale, f, g, h, i);
                    }
                    else
                        EQ_GLOBAL(SetTrackTransformation_guess_t, 0x006E5454)(trackDefinitionA, 0, scale, f, g, h, i);
                }
                break;
            }

            EQ_METHOD(1, void, 0x00411B7B, (CDisplay*, EQPlayer*), display)(player);
            if (display->infravision_on_guess != 0)
                EQ_METHOD(2, void, 0x004122EA, (CDisplay*, EQPlayer*, void*), display)
                    (player, &display->infravision_ambient_guess[0]);
            if (player->type == 0)
                EQ_METHOD(1, void, 0x00410602, (CDisplay*, EQPlayer*), display)(player);
        }

        if (player->actor_info->actor != 0)
        {
            if (player->type == 1)
                EQ_GLOBAL(SetActorScale_guess_t, 0x006E5380)(player->actor_info->actor,
                    player->actor_info->actorScale_guess, 0);
            else
                EQ_GLOBAL(SetActorScale_guess_t, 0x006E5380)(player->actor_info->actor,
                    player->actor_info->actorScale_guess, 1);

            EQ_GLOBAL(SetActorLocation_guess_t, 0x006E537C)
                (display->world_guess, player->actor_info->actor, actorLocation);
            if (player->type < 2 && player->stand_state != 's')
            {
                switch (player->stand_state)
                {
                default:
                    EQ_METHOD(6, void, 0x0048ED3B,
                        (CDisplay*, EQPlayer*, int, int, int, float, int), display)
                        (player, 0x20, 0, 0, 1.0f, 1);
                    break;
                case 'n':
                    EQ_METHOD(6, void, 0x0048ED3B,
                        (CDisplay*, EQPlayer*, int, int, int, float, int), display)
                        (player, 0x21, 1, 0, 1.0f, 1);
                    break;
                case 'i':
                    EQ_METHOD(6, void, 0x0048ED3B,
                        (CDisplay*, EQPlayer*, int, int, int, float, int), display)
                        (player, 0x24, 1, 0, 1.0f, 1);
                    break;
                }
            }
            else
            {
                int savedAnimationState = player->actor_info->unk_0x208;
                player->actor_info->unk_0x208 = 0;
                EQ_METHOD(6, void, 0x0048ED3B,
                    (CDisplay*, EQPlayer*, int, int, int, float, int), g_cdisplay_guess)
                    (player, 0x10, 1, 0, 1.0f, 1);
                player->actor_info->unk_0x208 = savedAnimationState;
            }
        }
    }

    if (player->actor_info->actor != 0)
    {
        EQ_GLOBAL(SetActorUserData_guess_t, 0x006E538C)(player->actor_info->actor, player);
        if (player->body_type >= 0x41)
            EQ_GLOBAL(SetActorInvisible_guess_t, 0x006E5378)(player->actor_info->actor, 1);
        if (display->infravision_on_guess != 0 && player->body_type != 8
            && player->body_type != 3 && player->body_type != 5)
            EQ_METHOD(2, void, 0x004122EA, (CDisplay*, EQPlayer*, void*), display)
                (player, &display->infravision_ambient_guess[0]);
        if (player == g_local_player_guess)
            EQ_METHOD(1, void, 0x004B23E1, (EQPlayer*, void*), player)
                (&EQ_GLOBAL(char, 0x0058FBA4));
        EQ_METHOD(1, void, 0x00406847, (CDisplay*, EQPlayer*), display)(player);
    }
    EQ_METHOD(1, int, 0x00407FF1, (CDisplay*, EQPlayer*), display)(player);
}

// FUNCTION: TRILOGY 0x00410116
// Resolves character model attachment points for the head, eyes, blood, gear, and shield.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// CDisplay::UpdateDAGPointers (__thiscall). display.h has no declaration for it, so it is a __fastcall
// free function (ECX = this, EDX dummy, player on the stack). `this` only goes on to
// FindDefaultEyeMaterialIndexes.
// Most DAG slots 0x254..0x294 are raw offsets: EQActorInfo_guess (player.h) does not name them.
void __fastcall UpdateDAGPointers(CDisplay* self, int edx_unused, EQPlayer* player)
{
    char slotDagName[128];
    char dagName[64];
    char actorTag[16];
    t3dGetHierarchicalSpriteDagByName_guess_t getDag;
    char* animSet;

    player->actor_info->head_point_dag = 0;
    player->actor_info->he_dag = 0;
    player->actor_info->attach_a_guess[0] = 0;
    player->actor_info->attach_a_guess[1] = 0;
    player->actor_info->bld_dag = 0;
    player->actor_info->name_sprite_guess = 0;
    if (player->actor_info->hier_sprite != 0)
    {
        EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), player)(actorTag);
        SOC_FMT(dagName, (const char*)0x0053E420, actorTag);
        getDag = EQ_GLOBAL(t3dGetHierarchicalSpriteDagByName_guess_t, 0x006E52C8);
        player->actor_info->head_point_dag = getDag(player->actor_info->hier_sprite, dagName);
        if (player->actor_info->head_point_dag == 0)
        {
            SOC_FMT(dagName, (const char*)0x0053E414, actorTag);
            player->actor_info->head_point_dag = getDag(player->actor_info->hier_sprite, dagName);
            if (player->actor_info->head_point_dag == 0)
            {
                if (player->gender == 1)
                    SOC_FMT(dagName, (const char*)0x0053E400);
                else
                    SOC_FMT(dagName, (const char*)0x0053E3EC);
                player->actor_info->head_point_dag = getDag(player->actor_info->hier_sprite, dagName);
            }
        }
        SOC_FMT(dagName, (const char*)0x0053E414, actorTag);
        player->actor_info->he_dag = getDag(player->actor_info->hier_sprite, dagName);
        if (player->actor_info->he_dag == 0)
        {
            if (player->gender == 1)
                SOC_FMT(dagName, (const char*)0x0053E3E0);
            else
                SOC_FMT(dagName, (const char*)0x0053E3D4);
            player->actor_info->he_dag = getDag(player->actor_info->hier_sprite, dagName);
        }
        SOC_FMT(dagName, (const char*)0x0053E3C8, actorTag);
        player->actor_info->bld_dag = getDag(player->actor_info->hier_sprite, dagName);
        SOC_FMT(dagName, (const char*)0x0053E3B8, actorTag);
        player->actor_info->attach_a_guess[0] = getDag(player->actor_info->hier_sprite, dagName);
        SOC_FMT(dagName, (const char*)0x0053E3A8, actorTag);
        player->actor_info->attach_a_guess[1] = getDag(player->actor_info->hier_sprite, dagName);
        SOC_FMT(dagName, (const char*)0x0053E394, actorTag);
        player->actor_info->shield_point_dag = getDag(player->actor_info->hier_sprite, dagName);
        animSet = player->actor_info->anim_set_guess;
        if (animSet != 0 && animSet[0x39C] == 1)
        {
            if (player->actor_info->tunic_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E380, actorTag);
                player->actor_info->tunic_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->attach_b_guess[0] == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E36C, actorTag);
                player->actor_info->attach_b_guess[0] = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->attach_b_guess[1] == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E358, actorTag);
                player->actor_info->attach_b_guess[1] = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->chest_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E344, actorTag);
                player->actor_info->chest_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->gauntl_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E330, actorTag);
                player->actor_info->gauntl_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->gauntr_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E31C, actorTag);
                player->actor_info->gauntr_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->shouldl_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E308, actorTag);
                player->actor_info->shouldl_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->shouldr_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E2F4, actorTag);
                player->actor_info->shouldr_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->legl_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E2E0, actorTag);
                player->actor_info->legl_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            if (player->actor_info->legr_point_dag == 0)
            {
                SOC_FMT(slotDagName, (const char*)0x0053E2CC, actorTag);
                player->actor_info->legr_point_dag = getDag(player->actor_info->hier_sprite, slotDagName);
            }
            EQ_METHOD(1, void, 0x00406A12, (CDisplay*, EQPlayer*), self)(player);
        }
    }
}

// FUNCTION: TRILOGY 0x004105e8
// Attaches client-side user data to a 3D actor.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetActorUserData(void* actor, void* userData)
{
    if (actor)
        t3dSetActorUserData(actor, userData);
}

// FUNCTION: TRILOGY 0x00410602
// Updates a spawn's point light from its light value and facing.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: create block laid out after the light tests (original: before them), light spilled to [ebp+8], EQ_THISCALL xor edx
void CDisplay::ChangeLight(EQPlayer* player)
{
    u8* actorInfo;
    void* oldLight;
    void* newLight;
    bool changed;
    f32 angle;
    f32 cosineValue;
    f32 cosineTwice;
    f32 sineValue;
    u8 light;
    u8 actorClass;

    if (is3dOn_guess == 0 || *(volatile int*)0x006e20b8 == 0)
        return;

    changed = false;
    actorInfo = (u8*)player->actor_info;
    oldLight = *(void**)(actorInfo + 4);
    if (oldLight != 0)
    {
        DeleteLight(oldLight);
        actorInfo = (u8*)player->actor_info;
        *(void**)(actorInfo + 4) = 0;
        changed = true;
    }

    angle = player->heading * 6.28000020980835f;
    angle = angle * 0.001953125f;
    cosineValue = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E524C)(angle);
    cosineTwice = cosineValue * 2.0f;
    sineValue = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E5250)(angle);
    sineValue = sineValue * 2.0f;
    light = player->light;
    if (light > 0)
    {
        actorInfo = (u8*)player->actor_info;
        actorClass = actorInfo[0x99];
        if ((actorClass != 5 && actorClass != 9 && actorClass != 10
             && actorClass != 6 && actorClass != 7 && actorClass != 8)
            || light == 3 || light == 5 || light == 7 || light >= 9)
        {
            newLight = CreateLight(light, 0.0f,
                                         cosineTwice + player->y,
                                         sineValue + player->x,
                                         player->z + 4.0f);
            actorInfo = (u8*)player->actor_info;
            *(void**)(actorInfo + 4) = newLight;
        }
        changed = true;
    }

    actorInfo = (u8*)player->actor_info;
    if (*(void**)(actorInfo + 4) == 0)
        player->light = 0;

    if (player == g_control_player_guess && changed)
        EQ_METHOD(2, void, 0x004D9D4D, (CEverQuest*, EQPlayer*, u8), pinstCEverQuest_guess)(g_control_player_guess, player->light);
}

// FUNCTION: TRILOGY 0x00410725
// Creates a point light of a light type at a world position; radius 0 picks the type's default range.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Light type 0..15 picks a light definition and a default radius (used when `radius` is 0; in zone 0x79
// always 25). Bug kept: with no light definition `light` is never assigned; the original then reads the
// stack slot it shares with `radius` (VC6 puts both in the same argument slot), so a nonzero radius
// still creates a point light from that garbage handle.
void* CDisplay::CreateLight(u8 type, float radius, float r, float g, float b)
{
    void* pointLight = 0;
    void** def;
    float range;
    void* light;
    float color[3];

    if (is3dOn_guess != 0)
    {
        switch (type)
        {
        case 0:
            def = &light_def_default_guess;
            range = radius;
            break;
        case 1:
            def = &light_defs_guess[6];
            if (radius == 0.0f)
                range = 25.0f;
            else
                range = radius;
            break;
        case 2:
            def = &light_defs_guess[0];
            if (radius == 0.0f)
                range = 100.0f;
            else
                range = radius;
            break;
        case 3:
        case 9:
            def = &light_defs_guess[1];
            if (radius == 0.0f)
                range = 100.0f;
            else
                range = radius;
            break;
        case 4:
            def = &light_defs_guess[2];
            if (radius == 0.0f)
                range = 150.0f;
            else
                range = radius;
            break;
        case 5:
        case 10:
            def = &light_defs_guess[4];
            if (radius == 0.0f)
                range = 150.0f;
            else
                range = radius;
            break;
        case 6:
            def = &light_defs_guess[3];
            if (radius == 0.0f)
                range = 200.0f;
            else
                range = radius;
            break;
        case 7:
        case 11:
            def = &light_defs_guess[5];
            if (radius == 0.0f)
                range = 200.0f;
            else
                range = radius;
            break;
        case 8:
            def = &light_defs_guess[3];
            if (radius == 0.0f)
                range = 400.0f;
            else
                range = radius;
            break;
        case 12:
        case 14:
            def = &light_defs_guess[7];
            if (radius == 0.0f)
                range = 150.0f;
            else
                range = radius;
            break;
        case 13:
        case 15:
            def = &light_defs_guess[8];
            if (radius == 0.0f)
                range = 150.0f;
            else
                range = radius;
            break;
        default:
            return 0;
        }
        if (*def != 0)
        {
            if (g_zone_id_guess == 0x79)
                range = 25.0f;
            light = t3dCreateLight(world_guess, *def);
        }
        if (light != 0)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
            pointLight = t3dCreatePointLightEx(world_guess, light, color, range, 1);
            t3dSetPointLightStaticSceneInfluenceFlag(pointLight);
            t3dSetPointLightDynamicSceneInfluenceFlag(pointLight);
        }
    }
    return pointLight;
}

// FUNCTION: TRILOGY 0x004108f2
// Maps each light type to its display intensity level.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
u8 CDisplay::GetIntensity(u8 lightType)
{
    switch (lightType)
    {
    case 0:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
    case 9:
        return 3;
    case 12:
    case 14:
        return 4;
    case 13:
    case 15:
        return 5;
    case 4:
        return 6;
    case 5:
    case 10:
        return 7;
    case 6:
        return 8;
    case 7:
    case 11:
        return 9;
    case 8:
        return 10;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00410970
// Removes a point light from the world scene.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::DeleteLight(void* light)
{
    if (is3dOn_guess && light && world_guess)
        t3dDestroyPointLight(world_guess, light);
}

// FUNCTION: TRILOGY 0x00410997
// Checks whether a destination and its approach path are clear for a player.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Fails when the sphere from the player to the target hits the world, or when the target or any point
// stepped back from it (2 units against the heading each time, while `distance` - 2k stays positive) is
// in an environment (GetEnvironment's out value nonzero).
int CDisplay::IsValidTeleport(EQPlayer* player, float y, float x, float z, float heading, float distance)
{
    int env = 0;
    float left;

    if (player == 0)
        return 0;
    if (CDisplay_CollideSphereWithWorld_guess(this, player->y, player->x, player->z, y, x, z, 0, 0, 0, 2))
        return 0;
    CDisplay_GetEnvironment_guess(this, y, x, z, &env);
    if (env != 0)
        return 0;
    left = distance - 2.0f;
    y = y - t3dFloatFastCosine(heading) * 2.0f;
    x = x - t3dFloatFastSine(heading) * 2.0f;
    while (left > 0.0f)
    {
        CDisplay_GetEnvironment_guess(this, y, x, z, &env);
        if (env != 0)
            return 0;
        left = left - 2.0f;
        y = y - t3dFloatFastCosine(heading) * 2.0f;
        x = x - t3dFloatFastSine(heading) * 2.0f;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00410abf
// Moves a spawn's game-world actor to match its position and orientation.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Moves the spawn's actor to the spawn's position (roll 0). Race 0x31 first plays animation 0x13 when the
// actor's heading differs from the spawn's.
void CDisplay::UpdatePlayerActor_guess(EQPlayer* player)
{
    float location[7];

    if (player == 0 || player->actor_info == 0 || player->actor_info->actor == 0 || is3dOn_guess == 0)
        return;
    if (player->race == 0x31)
    {
        t3dGetActorLocation(player->actor_info->actor, location);
        if (player->heading != location[3])
            CDisplay_PlayAnimationTrack_guess(this, player, 0x13, 1, 0, 1.5f, 0);
    }
    location[0] = player->y;
    location[1] = player->x;
    location[2] = player->z;
    location[3] = player->heading;
    location[4] = player->pitch;
    location[5] = 0.0f;
    t3dSetActorLocation(world_guess, player->actor_info->actor, location);
}

// FUNCTION: TRILOGY 0x00410b68
// Shows or hides spawn actors: invisible ones, and live spawns stale for over 12 s; refreshes name sprites.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay_guess::UpdatePlayerVisibility_guess(EQPlayer* player)
{
    u16 spawnRace;
    int state;
    EQPlayer* pEVar4;

    if (is3dOn_guess != 0)
    {
        pEVar4 = player ? player : EQ_GLOBAL(EQPlayer*, 0x006E4D58);
        for (; pEVar4 != 0; pEVar4 = pEVar4->next)
        {
            if (pEVar4->body_type < 0x41)
            {
                state = EQ_METHOD(2, int, 0x00410C80, (CDisplay_guess*, EQPlayer*, EQPlayer*), this)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), pEVar4);
                if (state == 0 && DAT_006b6494 == 0)
                {
                    if (pEVar4 == EQ_GLOBAL(EQPlayer*, 0x006E4D8C)
                        || pEVar4 == EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
                    {
                        if (g_activeView_guess == 0)
                        {
                            EQ_METHOD(1, void, 0x00411B7B, (CDisplay*, EQPlayer*), g_cdisplay_guess)(pEVar4);
                            continue;
                        }
                    }
                    else if (pEVar4->type < 2 && pEVar4->cur_hp >= 1
                        && (pEVar4->type != 1 || pEVar4->speed != 0.0f)
                        && (spawnRace = pEVar4->race, spawnRace != 0x48 && spawnRace != 0x49 && spawnRace != 0x8d))
                    {
                        if (12000 < (u32)(time_ms_guess
                            - pEVar4->actor_info->lastActiveTime_guess))
                            state = 1;
                    }
                    else if (*(int*)((char*)pEVar4->actor_info + 0x1a8) == 1)
                    {
                        state = 1;
                    }
                }
                if (pEVar4->actor_info->actor != 0)
                    t3dSetActorInvisible(pEVar4->actor_info->actor, state);
                EQ_METHOD(1, void, 0x00411B7B, (CDisplay*, EQPlayer*), g_cdisplay_guess)(pEVar4);
                if (player != 0)
                    return;
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x00410c80
// Determines whether a viewer sees a spawn as invisible.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// `this` ignored. Corpses (type >= 2) and spawns at 0 hp lose their invisibility and count as visible;
// body type 0x41 and above is always invisible. Invisible spawns are seen by a viewer who can see invisible,
// unless the target is a GM and the viewer is not.
int CDisplay::IsInvisible(EQPlayer* viewer, EQPlayer* target)
{
    int invisible = 0;
    int canSee;

    if (target == 0 || viewer == 0)
        return 0;
    if (target->type >= 2 || target->cur_hp < 1)
    {
        target->invis = 0;
        return 0;
    }
    if (target->body_type >= 0x41)
        return 1;
    canSee = 0;
    if (viewer->char_info_guess != 0)
        canSee = EQ_Character_CanISeeInvis_guess(viewer->char_info_guess);
    if (target->invis == 1)
    {
        invisible = 1;
        if (canSee != 0)
        {
            invisible = 0;
            if (target->gm != 0 && viewer->gm == 0)
                invisible = 1;
        }
    }
    return invisible;
}

// FUNCTION: TRILOGY 0x00410d07
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The second argument is unused.
/* fake match: callee is __thiscall, called as __stdcall; ECX still holds this, so no dummy EDX is loaded. */
void CDisplay::UpdatePlayerVisibility2_guess(EQPlayer* player, int unused)
{
    EQ_STDCALL(void, 0x00410B68, (EQPlayer*))(player);
}

// FUNCTION: TRILOGY 0x00410d41
// Configures how the game scene is drawn for the selected display mode.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall, one stack argument (the callee pops 4). display.h declares no CDisplay::SetUserRender_guess,
// so this is a __fastcall adapter (like HandleGroupInviteResponse_guess). Sets the render-method table of render_guess:
// mode 0, mode 1, or the third table for any other mode. Each call re-reads render_guess and goes through the
// GetProcAddress slot at 0x006E5444. Returns 0; neither caller reads EAX. The calls for 0x16, 0x17 and 0x29
// are written out in each branch to match the original's layout (VC6 shares the identical tails).
int __fastcall CDisplay_SetUserRender_guess(CDisplay* self, int unusedEdx, int mode)
{
    typedef void (__cdecl *t3dSetRenderContextUserDefinedRenderMethod_guess_t)(void*, u32, u32);
    t3dSetRenderContextUserDefinedRenderMethod_guess_t set_method;
    u32 rest_value;

    if (mode == 0)
    {
        set_method = (t3dSetRenderContextUserDefinedRenderMethod_guess_t)EQ_GLOBAL(void*, 0x006E5444);
        set_method(self->render_guess, 0x80000000, 0);
        set_method(self->render_guess, 0x80000001, 0x200557);
        set_method(self->render_guess, 0x80000002, 0x20054b);
        set_method(self->render_guess, 0x80000003, 0x57);
        set_method(self->render_guess, 0x80000004, 0x4b);
        set_method(self->render_guess, 0x80000005, 0x1280557);
        set_method(self->render_guess, 0x80000006, 0x124054b);
        set_method(self->render_guess, 0x80000007, 0x2005d7);
        set_method(self->render_guess, 0x80000008, 0x5cb);
        set_method(self->render_guess, 0x80000009, 0x1240557);
        set_method(self->render_guess, 0x8000000a, 0x12c0557);
        set_method(self->render_guess, 0x8000000b, 0x13b0557);
        set_method(self->render_guess, 0x8000000c, 0x507);
        set_method(self->render_guess, 0x8000000d, 0x40000407);
        set_method(self->render_guess, 0x8000000e, 0x40000473);
        set_method(self->render_guess, 0x8000000f, 0x41080407);
        set_method(self->render_guess, 0x80000010, 0x411b0407);
        set_method(self->render_guess, 0x80000011, 0x507);
        set_method(self->render_guess, 0x80000012, 0x507);
        set_method(self->render_guess, 0x80000013, 0x5d7);
        set_method(self->render_guess, 0x80000014, 0x200473);
        set_method(self->render_guess, 0x80000015, 0x200473);
        set_method(self->render_guess, 0x80000016, 0x2004f3);
        set_method(self->render_guess, 0x80000017, 0x11b0507);
        set_method(self->render_guess, 0x80000018, 0x1080507);
        set_method(self->render_guess, 0x80000019, 0x587);
        set_method(self->render_guess, 0x8000001a, 0);
        set_method(self->render_guess, 0x8000001e, 0);
        set_method(self->render_guess, 0x8000001f, 0x40000557);
        set_method(self->render_guess, 0x80000020, 0x4000054b);
        set_method(self->render_guess, 0x80000021, 0x40000057);
        set_method(self->render_guess, 0x80000022, 0x4000004b);
        set_method(self->render_guess, 0x80000023, 0x41080557);
        set_method(self->render_guess, 0x80000024, 0x4104054b);
        set_method(self->render_guess, 0x80000025, 0x400005d7);
        set_method(self->render_guess, 0x80000026, 0x400005cb);
        set_method(self->render_guess, 0x80000027, 0x41040557);
        set_method(self->render_guess, 0x80000028, 0x410c0557);
        set_method(self->render_guess, 0x80000029, 0x411b0557);
    }
    else
    {
        if (mode == 1)
        {
            rest_value = 6;
            set_method = (t3dSetRenderContextUserDefinedRenderMethod_guess_t)EQ_GLOBAL(void*, 0x006E5444);
            set_method(self->render_guess, 0x80000000, 6);
            set_method(self->render_guess, 0x80000001, 6);
            set_method(self->render_guess, 0x80000002, 6);
            set_method(self->render_guess, 0x80000003, 6);
            set_method(self->render_guess, 0x80000004, 6);
            set_method(self->render_guess, 0x80000005, 6);
            set_method(self->render_guess, 0x80000006, 6);
            set_method(self->render_guess, 0x80000007, 6);
            set_method(self->render_guess, 0x80000008, 6);
            set_method(self->render_guess, 0x80000009, 6);
            set_method(self->render_guess, 0x8000000a, 6);
            set_method(self->render_guess, 0x8000000b, 6);
            set_method(self->render_guess, 0x8000000c, 6);
            set_method(self->render_guess, 0x8000000d, 0x40000407);
            set_method(self->render_guess, 0x8000000e, 0x40000473);
            set_method(self->render_guess, 0x8000000f, 0x41080407);
            set_method(self->render_guess, 0x80000010, 0x411b0407);
            set_method(self->render_guess, 0x80000011, 0x507);
            set_method(self->render_guess, 0x80000012, 6);
            set_method(self->render_guess, 0x80000013, 0x2005d3);
            set_method(self->render_guess, 0x80000014, 0x200473);
            set_method(self->render_guess, 0x80000015, 0x200473);
            set_method(self->render_guess, 0x80000016, 0x2004f3);
            set_method(self->render_guess, 0x80000017, 0x11b0507);
        }
        else
        {
            rest_value = 0x200053;
            set_method = (t3dSetRenderContextUserDefinedRenderMethod_guess_t)EQ_GLOBAL(void*, 0x006E5444);
            set_method(self->render_guess, 0x80000000, 0x200053);
            set_method(self->render_guess, 0x80000001, 0x200053);
            set_method(self->render_guess, 0x80000002, 0x200053);
            set_method(self->render_guess, 0x80000003, 0x200053);
            set_method(self->render_guess, 0x80000004, 0x200053);
            set_method(self->render_guess, 0x80000005, 0x200053);
            set_method(self->render_guess, 0x80000006, 0x200053);
            set_method(self->render_guess, 0x80000007, 0x200053);
            set_method(self->render_guess, 0x80000008, 0x200053);
            set_method(self->render_guess, 0x80000009, 0x200053);
            set_method(self->render_guess, 0x8000000a, 0x200053);
            set_method(self->render_guess, 0x8000000b, 0x200053);
            set_method(self->render_guess, 0x8000000c, 0x200053);
            set_method(self->render_guess, 0x8000000d, 0x40000407);
            set_method(self->render_guess, 0x8000000e, 0x40000073);
            set_method(self->render_guess, 0x8000000f, 0x41080407);
            set_method(self->render_guess, 0x80000010, 0x411b0407);
            set_method(self->render_guess, 0x80000011, 0x507);
            set_method(self->render_guess, 0x80000012, 0x200053);
            set_method(self->render_guess, 0x80000013, 0x200053);
            set_method(self->render_guess, 0x80000014, 0x200053);
            set_method(self->render_guess, 0x80000015, 0x200053);
            set_method(self->render_guess, 0x80000016, 0x200053);
            set_method(self->render_guess, 0x80000017, 0x11b0507);
        }
        set_method(self->render_guess, 0x80000018, 0x1080507);
        set_method(self->render_guess, 0x80000019, 0x587);
        set_method(self->render_guess, 0x8000001a, 0);
        set_method(self->render_guess, 0x8000001e, rest_value);
        set_method(self->render_guess, 0x8000001f, rest_value);
        set_method(self->render_guess, 0x80000020, rest_value);
        set_method(self->render_guess, 0x80000021, rest_value);
        set_method(self->render_guess, 0x80000022, rest_value);
        set_method(self->render_guess, 0x80000023, rest_value);
        set_method(self->render_guess, 0x80000024, rest_value);
        set_method(self->render_guess, 0x80000025, rest_value);
        set_method(self->render_guess, 0x80000026, rest_value);
        set_method(self->render_guess, 0x80000027, rest_value);
        set_method(self->render_guess, 0x80000028, rest_value);
        set_method(self->render_guess, 0x80000029, rest_value);
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0041125d
// Finds the actor under a screen position and can make the local player get on it.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The view actor is excluded unless g_activeView_guess (set on death) is set. With `getOn`, the local player
// gets on the picked actor (GetOnActor).
/* fake match: viewActor is computed before the x/y adjustments; inline in the call, VC6 keeps it in EDI, not EAX. */
void* CDisplay::PickActorOnScreen_guess(int x, int y, int getOn)
{
    void* actor;
    void* viewActor;

    if (is3dOn_guess == 1 && world_guess != 0 && render_guess != 0 && camera_guess != 0)
    {
        viewActor = g_activeView_guess ? 0 : g_view_actor_guess;
        x -= g_render_rect_guess[0];
        y -= g_render_rect_guess[1];
        actor = s3dPickObjectOnScreen(world_guess, render_guess, camera_guess, viewActor, x, y, 1);
        if (getOn != 0)
            CDisplay_GetOnActor_guess(this, actor, g_local_player_guess);
        return actor;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004112df
// Creates a non-spawn actor (object, door, missile, effect) by model name; optional IT63/RED_BLOB fallback.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Uses the thiscall register/stack layout through a fastcall entry because this header has no
// declaration for the recovered member yet.
// @early-stop: register picks differ (display in esi, fp in edi, no zero register), tag buffer stores fold to constant addresses, sphere pointer lives in a register not [ebp+0x20]
void* __fastcall CDisplay_CreateActor_guess(CDisplay* display, int edx_dummy,
                                            char* actorDefName, int actorParam0, u32 actorParam1,
                                            u32 actorParam2, u32 actorParam3, u32 actorParam4,
                                            float sphereRadius, int allowFallback, float noSphere)
{
    typedef void* (__cdecl *t3dGetActorDefinitionDefaultInstance_guess_t)(void*);
    typedef void (__cdecl *t3dGetObjectTag_guess_t)(void*, char*);
    typedef void* (__cdecl *t3dCreateSphere_guess_t)(float);
    typedef void* (__cdecl *t3dCreateActorEx_guess_t)(void*, void*, int, void*, void*,
                                                       int, float, float, int, int);
    typedef void (__cdecl *t3dSet2DSpriteCurrentFrame_guess_t)(int*, int);
    typedef void (__cdecl *t3dDestroySphere_guess_t)(void*);

    u32 actorParameters[7];
    char zone[64];
    char actorTag[256];
    void* definition;
    void* defaultInstance;
    void* actorSphere;
    void* actor;
    int* sprite;
    int skipSphere = *(int*)&noSphere;
    int defaultSphere;
    int result;
    int count;
    u32* tagCursor;
    float actorScale;
    float radius;
    EQPlayer* player;
    volatile int displayCopy; /* fake match: the this copy must keep its own stack slot; VC6 otherwise folds it into display's register */
    CDisplay* self;
    t3dGetPointerFromDictionary_t getPointer = t3dGetPointerFromDictionary;

    displayCopy = (int)display;
    if (display->is3dOn_guess == 0)
        return 0;

    EQ_CDECL(void*, 0x00521960, (void*, int, size_t))(
        actorParameters, 0, 0x1c);
    actorParameters[0] = (u32)actorParam0;
    actorParameters[1] = actorParam1;
    actorParameters[2] = actorParam2;
    actorParameters[3] = actorParam3;
    actorParameters[4] = actorParam4;
    /* t3dSetActorLocation reads this block as float*; slot 5 is an explicit 0.0f, the rest is the memset */
    *(float*)&actorParameters[5] = 0.0f;

    if (strlen(actorDefName) < 10 &&
        SWD_STRCMPI(
            actorDefName, EQ_ARRAY(const char, 0x0053E4BC)) != 0)
    {
        SWD_STRCAT(
            actorDefName, EQ_ARRAY(const char, 0x0053E4B0));
    }

    definition = getPointer(display->world_guess, actorDefName);
    if (definition == 0)
    {
        if (allowFallback == 0)
            return 0;
        SOC_CPY(
            actorDefName, EQ_ARRAY(const char, 0x0053E4A0));
        definition = getPointer(display->world_guess, actorDefName);
        if (definition == 0)
        {
            SOC_CPY(
                actorDefName, EQ_ARRAY(const char, 0x0053E48C));
            definition = getPointer(display->world_guess, actorDefName);
            if (definition == 0)
                return 0;
        }
    }

    defaultInstance = EQ_GLOBAL(t3dGetActorDefinitionDefaultInstance_guess_t,
                                0x006E5278)(definition);
    if (defaultInstance == 0)
        return 0;

    actorTag[0] = 0;
    tagCursor = (u32*)(actorTag + 1);
    for (count = 0x3f; count != 0; count = count - 1)
    {
        *tagCursor = 0;
        tagCursor = tagCursor + 1;
    }
    *(u16*)tagCursor = 0;
    *((char*)tagCursor + 2) = 0;
    sprite = t3dGetActorSprite(defaultInstance, 0, 0);
    if (sprite == 0)
    {
        EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(definition, actorTag);
        return 0;
    }

    actorScale = 1.0f;
    player = g_local_player_guess;
    if (player != 0 || (player = g_player_top_guess) != 0)
        SOC_CPY(
            zone, player->zone);
    else
        zone[0] = 0;

    result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
        zone, EQ_ARRAY(const char, 0x0053E484), 7);
    if (result == 0 ||
        (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
             zone, EQ_ARRAY(const char, 0x0053E470), 9), result == 0) ||
        (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
             zone, EQ_ARRAY(const char, 0x0053E468), 6), result == 0))
    {
        result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
            actorDefName, EQ_ARRAY(const char, 0x0053E448), 4);
        if (result == 0 ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E47C), 4), result == 0))
            actorScale = 0.8f;
    }
    else
    {
        result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
            actorDefName, EQ_ARRAY(const char, 0x0053E460), 4);
        if (result == 0)
            actorScale = 1.5f;
        else if ((result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                      actorDefName, EQ_ARRAY(const char, 0x0053E458), 4)) == 0)
            actorScale = 1.5f;
        else if ((result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                      actorDefName, EQ_ARRAY(const char, 0x0053E450), 4)) == 0)
            actorScale = 1.5f;
        else
        {
            result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                actorDefName, EQ_ARRAY(const char, 0x0053E47C), 4);
            if (result == 0)
                actorScale = 1.3f;
        }
    }

    result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
        actorDefName, EQ_ARRAY(const char, 0x0053E440), 7);
    if (result == 0)
        actorScale = 1.5f;

    _t3dComputeActorDefinitionBoundingRadius(
        definition, &radius);
    radius = radius * actorScale;
    defaultSphere = 0x2e;

    result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
        actorDefName, EQ_ARRAY(const char, 0x0053E43C), 2);
    if (result == 0 &&
        EQ_CDECL(int, 0x00522F25, (int))((int)actorDefName[2]) != 0)
    {
        result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
            actorDefName, EQ_ARRAY(const char, 0x0053E460), 4);
        if (result == 0 ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E458), 4), result == 0) ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E448), 4), result == 0) ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E450), 4), result == 0) ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E47C), 4), result == 0) ||
            (result = EQ_CDECL(int, 0x005316C0, (const char*, const char*, u32))(
                 actorDefName, EQ_ARRAY(const char, 0x0053E434), 4), result == 0))
            skipSphere = 0;
    }

    if (skipSphere != 0)
        actorSphere = 0;
    else if (sphereRadius != 0.0f)
        actorSphere = EQ_GLOBAL(t3dCreateSphere_guess_t, 0x006E51A4)(sphereRadius);
    else
        actorSphere = &defaultSphere;

    self = (CDisplay*)displayCopy;
    self->unk_0x124 = 0;
    actor = EQ_GLOBAL(t3dCreateActorEx_guess_t, 0x006E5180)(
        self->world_guess, definition, 0, actorParameters, actorSphere,
        0, actorScale, radius, 0, 0);
    if (actor != 0)
    {
        sprite = t3dGetActorSprite(actor, 0, 0);
        if (*sprite == 10)
            EQ_GLOBAL(t3dSet2DSpriteCurrentFrame_guess_t, 0x006E536C)(sprite, 0);
        t3dSetActorLocation(
            self->world_guess, actor, (float*)actorParameters);
    }

    if (actorSphere != 0 && actorSphere != &defaultSphere)
        EQ_GLOBAL(t3dDestroySphere_guess_t, 0x006E5224)(actorSphere);
    return actor;
}

// FUNCTION: TRILOGY 0x00411694
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The definition's radius, at least `minRadius`, times `scale` when that is above 1.
float CDisplay::SetActorBoundingRadius(T3dActor_guess* actor, float scale, float minRadius)
{
    float radius = 0.0f;

    if (actor == 0)
    {
        LogMessage_guess("current actor is NULL in SetActorBoundingRadius()!!\n");
    }
    else
    {
        _t3dComputeActorDefinitionBoundingRadius(actor->definition_guess, &radius);
        if (minRadius > radius)
            radius = minRadius;
        if (scale > 1.0f)
            radius = radius * scale;
        t3dSetActorBoundingRadius(actor, radius, 0);
        t3dUpdateTouchedRegions(world_guess, actor, actor->region_guess);
    }
    return radius;
}

// FUNCTION: TRILOGY 0x00411715
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Deleting the view actor moves the view to the local player's actor (or to none).
void CDisplay::DeleteActor(void* actor)
{
    if (is3dOn_guess != 0 && actor != 0)
    {
        unk_0x124 = 0;
        if (g_view_actor_guess == actor)
        {
            if (g_local_player_guess != 0 && g_local_player_guess->actor_info != 0
                && g_local_player_guess->actor_info->actor != g_view_actor_guess)
                SetViewActor(g_local_player_guess->actor_info->actor);
            else
                SetViewActor(0);
        }
        t3dDestroyActor(world_guess, actor);
    }
}

// FUNCTION: TRILOGY 0x0041176d
// Gives an actor private copies of its hierarchical sprites, with duplicated skins and material palette.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The class declaration does not yet expose this recovered member, so this file-local
// fastcall entry uses the same ECX/EDX/stack layout as its __thiscall original.
// @early-stop: VC6 emits an EBP frame (push ebp; mov ebp,esp) that the original does not have; cause not found, so locals and the actor parameter are ebp-relative
int* __fastcall dupActorHierarchicalSprite(CDisplay* self, int, void* actor)
{
    typedef void* (__cdecl *t3dGetActorDefinition_guess_t)(void*);
    typedef u32 (__cdecl *t3dGetActorDefinitionNumActions_guess_t)(void*);
    typedef u32 (__cdecl *t3dGetActorDefinitionNumLevelsOfDetail_guess_t)(void*, u32);
    typedef void* (__cdecl *t3dGetHierarchicalSpriteDefinitionDefaultInstance_guess_t)(void*);
    typedef int* (__cdecl *t3dGetHierarchicalSpriteCollisionVolume_guess_t)(void*);
    typedef int* (__cdecl *t3dDuplicateHierarchicalSprite_guess_t)(void*, int*, int*);
    typedef int (__cdecl *t3dSetActorSprite_guess_t)(void*, u32, u32, int*);
    typedef int (__cdecl *t3dGetActorCollisionVolumeType_guess_t)(void*);
    typedef int* (__cdecl *t3dGetActorCollisionVolume_guess_t)(void*);
    typedef int (__cdecl *t3dSetActorCollisionVolume_guess_t)(void*, void*, int*);
    typedef int (__cdecl *t3dDestroyVolume_guess_t)(void*, int*);
    typedef int (__cdecl *s3dDetachSkinFromHierarchicalSprite_guess_t)(void*, int, void*);
    typedef u32 (__cdecl *s3dGetSkinAttachedToHierarchicalSpriteLinkUpdatesToDAGIndex_guess_t)(u32, void*);
    typedef void* (__cdecl *s3dDuplicateDMSprite_guess_t)(void*, void*, int);
    typedef void* (__cdecl *s3dDuplicateMaterialPalette_guess_t)(void*, void*);
    typedef int (__cdecl *s3dSetDMSpriteMaterialPalette_guess_t)(void*, void*);
    typedef int (__cdecl *s3dAttachSkinToHierarchicalSprite_guess_t)(void*, void*, u32);
    typedef int (__cdecl *s3dGetNumSkinsAttachedToHierarchicalSprite_guess_t)(void*);

    void* actorDefinition;
    u32 numActions;
    u32 actionIndex;
    u32 numLevelsOfDetail;
    u32 levelOfDetailIndex;
    int* sprite;
    void* hierarchicalSprite;
    int* collisionVolume;
    int* newSprite;
    int collisionVolumeType;
    int* oldCollisionVolume;
    int numSkins;
    u32 skinIndex;
    void* skin;
    u32 linkUpdatesToDAGIndex;
    void* newSkin;
    void* materialPalette;
    void* newMaterialPalette;
    char* actorName;
    s3dGetNumSkinsAttachedToHierarchicalSprite_guess_t getNumSkins;

    actorDefinition = EQ_GLOBAL(t3dGetActorDefinition_guess_t, 0x006E5274)(actor);
    numActions = EQ_GLOBAL(t3dGetActorDefinitionNumActions_guess_t, 0x006E527C)(actorDefinition);
    newSprite = 0;
    actionIndex = 0;
    if (numActions != 0)
    {
        do
        {
            numLevelsOfDetail = EQ_GLOBAL(t3dGetActorDefinitionNumLevelsOfDetail_guess_t, 0x006E5280)(actorDefinition, actionIndex);
            levelOfDetailIndex = 0;
            if (numLevelsOfDetail != 0)
            {
                do
                {
                    sprite = t3dGetActorSprite(actor, actionIndex, levelOfDetailIndex);
                    if (sprite == 0)
                    {
                        actorName = *(char**)((char*)actorDefinition + 8);
                        LogMessage_guess(
                            EQ_ARRAY(char, 0x0053E5EC), actorName);
                        return 0;
                    }
                    if (*sprite == 0x14)
                    {
                        hierarchicalSprite = t3dGetHierarchicalSpriteDefinition(sprite);
                        if (hierarchicalSprite == 0)
                        {
                            actorName = *(char**)((char*)actorDefinition + 8);
                            LogMessage_guess(
                                EQ_ARRAY(char, 0x0053E5A8), actorName);
                            return 0;
                        }
                        hierarchicalSprite = EQ_GLOBAL(
                            t3dGetHierarchicalSpriteDefinitionDefaultInstance_guess_t, 0x006E52D4)(
                                hierarchicalSprite);
                        if (hierarchicalSprite == 0)
                        {
                            actorName = *(char**)((char*)actorDefinition + 8);
                            LogMessage_guess(
                                EQ_ARRAY(char, 0x0053E568), actorName);
                            return 0;
                        }
                        collisionVolume = EQ_GLOBAL(
                            t3dGetHierarchicalSpriteCollisionVolume_guess_t, 0x006E52C0)(hierarchicalSprite);
                        newSprite = EQ_GLOBAL(
                            t3dDuplicateHierarchicalSprite_guess_t, 0x006E5234)(
                                self->world_guess, sprite, collisionVolume);
                        if (newSprite == 0)
                        {
                            actorName = *(char**)((char*)actorDefinition + 8);
                            LogMessage_guess(
                                EQ_ARRAY(char, 0x0053E500), actorName,
                                collisionVolume, *collisionVolume, self->world_guess);
                            return 0;
                        }
                        EQ_GLOBAL(t3dSetActorSprite_guess_t, 0x006E5384)(
                            actor, actionIndex, levelOfDetailIndex, newSprite);
                        collisionVolumeType = EQ_GLOBAL(
                            t3dGetActorCollisionVolumeType_guess_t, 0x006E5270)(actor);
                        if (collisionVolumeType == 0)
                        {
                            oldCollisionVolume = EQ_GLOBAL(
                                t3dGetActorCollisionVolume_guess_t, 0x006E526C)(actor);
                            if (EQ_GLOBAL(t3dSetActorCollisionVolume_guess_t, 0x006E5374)(
                                    self->world_guess, actor, collisionVolume) == 0)
                            {
                                EQ_GLOBAL(t3dDestroyVolume_guess_t, 0x006E5228)(
                                    self->world_guess, oldCollisionVolume);
                            }
                        }
                        getNumSkins = s3dGetNumSkinsAttachedToHierarchicalSprite;
                        numSkins = getNumSkins(newSprite);
                        while (numSkins != 0)
                        {
                            EQ_GLOBAL(s3dDetachSkinFromHierarchicalSprite_guess_t, 0x006E50E4)(
                                self->world_guess, 0, newSprite);
                            numSkins = getNumSkins(newSprite);
                        }
                        skinIndex = 0;
                        numSkins = s3dGetNumSkinsAttachedToHierarchicalSprite(hierarchicalSprite);
                        if (numSkins != 0)
                        {
                            do
                            {
                                skin = s3dGetSkinAttachedToHierarchicalSprite(skinIndex, hierarchicalSprite);
                                linkUpdatesToDAGIndex = EQ_GLOBAL(
                                    s3dGetSkinAttachedToHierarchicalSpriteLinkUpdatesToDAGIndex_guess_t,
                                    0x006E5118)(skinIndex, hierarchicalSprite);
                                newSkin = EQ_GLOBAL(s3dDuplicateDMSprite_guess_t, 0x006E50EC)(
                                    self->world_guess, skin, 0);
                                if (skinIndex == 0)
                                {
                                    materialPalette = s3dGetDMSpriteMaterialPalette(skin);
                                    newMaterialPalette = EQ_GLOBAL(
                                        s3dDuplicateMaterialPalette_guess_t, 0x006E50F0)(
                                            self->world_guess, materialPalette);
                                }
                                EQ_GLOBAL(s3dSetDMSpriteMaterialPalette_guess_t, 0x006E5134)(
                                    newSkin, newMaterialPalette);
                                EQ_GLOBAL(s3dAttachSkinToHierarchicalSprite_guess_t, 0x006E50BC)(
                                    newSprite, newSkin, linkUpdatesToDAGIndex);
                                skinIndex = skinIndex + 1;
                                numSkins = s3dGetNumSkinsAttachedToHierarchicalSprite(hierarchicalSprite);
                            } while (skinIndex < (u32)numSkins);
                        }
                    }
                    levelOfDetailIndex = levelOfDetailIndex + 1;
                } while (levelOfDetailIndex < numLevelsOfDetail);
            }
            actionIndex = actionIndex + 1;
        } while (actionIndex < numActions);
    }
    return newSprite;
}

// FUNCTION: TRILOGY 0x004119a6
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Replaces the DAG's hierarchical or DM sprite and releases the previous sprite by its current type.
void* CDisplay::ChangeDag(void* dag, const char* dagName)
{
    typedef void* (__cdecl *s3dCreateDMSprite_guess_t)(void*, void*, int);
    typedef void* (__cdecl *t3dCreateHierarchicalSprite_guess_t)(void*, void*, int);
    typedef float (__cdecl *t3dGetSpriteDefinitionBoundingRadius_guess_t)(void*);
    typedef int (__cdecl *t3dSetSpriteDefinitionBoundingRadius_guess_t)(void*, float);
    typedef int (__cdecl *s3dDestroyDMSprite_guess_t)(void*, void*);
    typedef int (__cdecl *t3dDestroyHierarchicalSprite_guess_t)(void*, void*);
    t3dGetPointerFromDictionary_t getPointer;
    int* oldSprite;
    void* definition;
    void* sprite;
    char nameBuffer[128];

    sprite = 0;
    if (dag == 0)
        return 0;

    oldSprite = t3dGetDagSprite(dag);
    if (dagName != 0)
    {
        SOC_FMT(nameBuffer, (const char*)0x0053E630, dagName);
        getPointer = t3dGetPointerFromDictionary;
        definition = getPointer(world_guess, nameBuffer);
        if (definition == 0)
        {
            SOC_FMT(nameBuffer, (const char*)0x0053D8CC, dagName);
            definition = getPointer(world_guess, nameBuffer);
            /* fake match: re-zeroes sprite, which is already 0 on this path, so VC6 frees EDI across the second lookup; no behaviour change */
            sprite = 0;
            if (definition != 0)
                sprite = EQ_GLOBAL(s3dCreateDMSprite_guess_t, 0x006E50CC)(world_guess, definition, 0);
        }
        else
        {
            sprite = EQ_GLOBAL(t3dCreateHierarchicalSprite_guess_t, 0x006E518C)(world_guess, definition, 0);
            if (EXEC_STRCMP(dagName, (const char*)0x0053E62C) == 0)
            {
                // VC6 branches on ZF alone, so an unordered (NaN) radius skips the setter, as the original's JZ does.
                if (EQ_GLOBAL(t3dGetSpriteDefinitionBoundingRadius_guess_t, 0x006E52D0)(definition) != 3.0f)
                    EQ_GLOBAL(t3dSetSpriteDefinitionBoundingRadius_guess_t, 0x006E53E8)(definition, 3.0f);
            }
        }
    }

    t3dSetDagSprite(dag, sprite);
    if (*oldSprite != 0x14)
    {
        if (*oldSprite == 0x48)
            EQ_GLOBAL(s3dDestroyDMSprite_guess_t, 0x006E50D8)(world_guess, oldSprite);
    }
    else
        EQ_GLOBAL(t3dDestroyHierarchicalSprite_guess_t, 0x006E5210)(world_guess, oldSprite);
    return sprite;
}

// FUNCTION: TRILOGY 0x00411ac7
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// No font: 0. The dag's old sprite is destroyed when it was a string sprite (type 0x51).
void* CDisplay::SetDagStringSprite_guess(void* dag, void* font, char* text)
{
    void* sprite = 0;
    int* old;
    u8 tint[3];

    if (font == 0)
        return 0;
    old = t3dGetDagSprite(dag);
    if (text != 0 && *text != 0)
    {
        sprite = s3dCreateStringSprite(world_guess, 0, font, text, 0x80000019);
        if (sprite == 0)
            return 0;
        s3dSetStringSpriteMaxScaleFactor(sprite, 1, 0.05f);
        s3dSetStringSpriteYonClip(sprite, 1, 70.0f);
        tint[2] = 0xFF;
        tint[1] = 0xFF;
        tint[0] = 0xFF;
        s3dSetStringSpriteTint(sprite, tint);
    }
    t3dSetDagSprite(dag, sprite);
    if (old != 0 && *old == 0x51)
        s3dDestroyStringSprite(world_guess, old);
    return sprite;
}

// FUNCTION: TRILOGY 0x00411b7b
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Shows the name sprite (state 1) where the flag for the spawn's kind (NPC or player) is set.
void CDisplay::UpdateNameSprite(EQPlayer* player)
{
    EQPlayer* p = player;

    if (player == 0)
        p = g_player_top_guess;
    while (p != 0)
    {
        if (p->type != 0)
        {
            if (g_npc_names_flag_guess != 0)
                CDisplay_SetNameSpriteState_guess(this, p, 1);
        }
        else
        {
            if (g_pc_names_on_guess != 0)
                CDisplay_SetNameSpriteState_guess(this, p, 1);
        }
        if (player != 0)
            break;
        p = p->next;
    }
}

// FUNCTION: TRILOGY 0x00411bc8
// Sets or clears a spawn's displayed name, guild label, and status text.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: shared return 0 and 3D-off tail laid out inline/at the end (VC6 cross-jumps); trimName's mov ecx order
// Sets one spawn's name sprite. With the 3d engine on and show != 0 the name text (prefix, last name, guild,
// tags) goes on the actor's string sprite and the tint is set; otherwise the name and guild sprites are cleared.
// Returns 0 when the spawn has no actor sprite. Original __thiscall: `this` in ECX, player and show on the stack.
int CDisplay::SetNameSpriteState_guess(EQPlayer* player, int showNameSprite)
{
    char prefix[32];
    char text[512];
    char* trimmed;

    if (player == 0)
        return 0;
    if (this->is3dOn_guess == 0)
    {
        if (player->type == 1 && ((player->race == 0x3c && player->pet_owner_id == 0) || player->race == 0x1d || player->race == 0x34))
            return 0;
        if (player->actor_info == 0)
            return 0;
        if (player->actor_info->head_point_dag == 0)
            return 0;
    }
    else
    {
        if (player->race == 0x3c && player->pet_owner_id == 0)
            return 0;
        if (player->race == 0x1d)
            return 0;
        if (player->actor_info == 0)
            return 0;
        if (player->actor_info->head_point_dag == 0)
            return 0;
        if (showNameSprite != 0)
        {
            if (player->race >= 0xf4)
            {
                SOC_FMT(text, EQ_ARRAY(char, 0x0053E678), player->name);
            }
            else if (player->type == 0)
            {
                if (player->invis == 1)
                {
                    trimmed = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->name);
                    SOC_FMT(prefix, EQ_ARRAY(char, 0x0053E670), trimmed);
                }
                else
                {
                    trimmed = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->name);
                    SOC_CPY(prefix, trimmed);
                }
                if ((player->guild_id == -1 || player->anon != 0) &&
                    (!EQ_METHOD(0, bool, 0x004B4868, (EQPlayer*), player)() || player->guild_id == -1))
                {
                    trimmed = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->last_name);
                    SOC_FMT(text, EQ_ARRAY(char, 0x0053E668), prefix, trimmed);
                }
                else if (player->actor_info->name_sprite_guess != 0)
                {
                    trimmed = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->last_name);
                    SOC_FMT(text, EQ_ARRAY(char, 0x0053E668), prefix, trimmed);
                    EQ_METHOD(3, void*, 0x00411AC7, (CDisplay*, void*, void*, char*), this)(
                        player->actor_info->name_sprite_guess, this->fontTexture_guess,
                        EQ_CDECL(char*, 0x004E00F6, (short))(player->guild_id));
                }
                else
                {
                    SOC_FMT(text, EQ_ARRAY(char, 0x0053E65C), prefix,
                        EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->last_name),
                        EQ_CDECL(char*, 0x004E00F6, (short))(player->guild_id));
                }

                if (player->actor_info->unk_0x300 == 1)
                {
                    EQ_CDECL(char*, 0x00531950, (char*))(text);
                    SWD_STRCAT(text, EQ_ARRAY(char, 0x0053E658));
                    EQ_CDECL(char*, 0x00531950, (char*))(text);
                }
                if (player->afk != 0)
                {
                    if (player->linkdead == 0 || player->pvp != 0)
                        SWD_STRCAT(text, EQ_ARRAY(char, 0x0053E644));
                    else
                        SWD_STRCAT(text, EQ_ARRAY(char, 0x0053E64C));
                }
                else if (player->linkdead != 0 && player->pvp == 0)
                    SWD_STRCAT(text, EQ_ARRAY(char, 0x0053E63C));
            }
            else
            {
                trimmed = EQ_METHOD(1, char*, 0x004D02FD, (void*, char*), EQ_GLOBAL(void*, 0x006EFE2C))(player->name);
                SOC_CPY(text, trimmed);
            }
            EQ_METHOD(3, void*, 0x00411AC7, (CDisplay*, void*, void*, char*), this)(
                player->actor_info->head_point_dag, this->fontTexture_guess, text);
            EQ_METHOD(1, int, 0x00411EE6, (CDisplay*, EQPlayer*), this)(player);
            return 1;
        }
    }
    EQ_METHOD(3, void*, 0x00411AC7, (CDisplay*, void*, void*, char*), this)(
        player->actor_info->head_point_dag, this->fontTexture_guess, 0);
    if (player->actor_info->name_sprite_guess != 0)
        EQ_METHOD(3, void*, 0x00411AC7, (CDisplay*, void*, void*, char*), this)(
            player->actor_info->name_sprite_guess, this->fontTexture_guess, 0);
    return 1;
}

// FUNCTION: TRILOGY 0x00411ee6
// Sets a spawn's name colour by GM, NPC, PvP and role-play state (race in one mode); one spawn's colour pulses.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int __stdcall CDisplay_SetNameSpriteTint_guess(EQPlayer* spawn)
{
    u16 spawnRace;
    s3dSetStringSpriteTint_t setStringSpriteTint;
    char result;
    int* stringSprite;
    int temp;
    int tmp1;
    int tmp2;
    struct
    {
        u8 local_8;
        u8 local_7;
        u8 local_6;
    } local_tint;

    if (spawn == (EQPlayer*)0 || spawn->actor_info == (EQActorInfo_guess*)0 ||
        (temp = *(int*)((char*)spawn->actor_info + 0x254), temp == 0) ||
        (stringSprite = t3dGetDagSprite((void*)temp), stringSprite == 0 || *stringSprite != 0x51))
    {
        return 0;
    }
    if (spawn->race >= 0xf4)
    {
        if (spawn->max_hp == 2000)
        {
            local_tint.local_6 = 0xff;
            local_tint.local_7 = 0xff;
            local_tint.local_8 = 0x33;
            goto applyTint; // as original: the original jumps from this tint straight to the shared tail, it does not fall into it
        }
        local_tint.local_7 = 0xff;
        local_tint.local_6 = 0x66;
        local_tint.local_8 = 0;
    }
    else
    {
        if (((spawn->gm & 1) != 0 && spawn->type != '\x01') && spawn->anon == 0)
        {
            local_tint.local_6 = 0x1d;
            local_tint.local_7 = 0xa6;
            local_tint.local_8 = 0x4b;
            goto applyTint; // as original: the original jumps from this tint straight to the shared tail, it does not fall into it
        }
        if (spawn->type != '\0')
        {
            local_tint.local_6 = 0x17;
            local_tint.local_7 = 0xb8;
            local_tint.local_8 = 0xad;
            goto applyTint; // as original: the original jumps from this tint straight to the shared tail, it does not fall into it
        }
        if (spawn->pvp != '\0')
        {
            result = EQ_METHOD(0, char, 0x004B4868, (EQPlayer*), spawn)();
            if (result != '\0')
            {
                local_tint.local_6 = 0xef;
                local_tint.local_7 = 0x5d;
                local_tint.local_8 = 0x5d;
            }
            else
            {
                local_tint.local_6 = 0xa6;
                local_tint.local_7 = 0x14;
                local_tint.local_8 = 0x14;
            }
            goto applyTint; // as original: the original jumps from this tint straight to the shared tail, it does not fall into it
        }
        result = EQ_METHOD(0, char, 0x004B4868, (EQPlayer*), spawn)();
        if (result != '\0' && EQ_GLOBAL(int, 0x006B7108) == 0)
        {
            local_tint.local_6 = 0x85;
            local_tint.local_7 = 0x48;
            local_tint.local_8 = 0x9c;
        }
        else if ((result = EQ_METHOD(0, char, 0x004B4868, (EQPlayer*), spawn)()) != '\0' &&
                 EQ_GLOBAL(int, 0x006B7108) == 4)
        {
            local_tint.local_6 = 0x85;
            local_tint.local_7 = 0x48;
            local_tint.local_8 = 0x9c;
        }
        else
        {
            local_tint.local_6 = 0x3d;
            local_tint.local_7 = 0x6b;
            local_tint.local_8 = 0xdc;
            if (EQ_GLOBAL(int, 0x006B7108) != 2)
                goto applyTint; // as original: the original branches past the race table straight to the shared tail with the default tint
            spawnRace = spawn->race;
            if (spawnRace == 5 || spawnRace == 4 || spawnRace == 7)
            {
                local_tint.local_6 = 0xff;
                local_tint.local_7 = 0xff;
                local_tint.local_8 = 0;
            }
            else if (spawnRace == 8 || spawnRace == 0xc || spawnRace == 0xb)
            {
                local_tint.local_6 = 0x85;
                local_tint.local_7 = 0x48;
                local_tint.local_8 = 0x9c;
            }
            if (spawnRace == 1 || spawnRace == 2 || spawnRace == 3 || spawnRace == 0x82)
            {
                local_tint.local_6 = 0x3d;
                local_tint.local_7 = 0x6b;
                local_tint.local_8 = 0xdc;
            }
            if (spawnRace != 10 && spawnRace != 9 && spawnRace != 6 && spawnRace != 0x80)
                goto applyTint; // as original: the original branches past the final tint straight to the shared tail
            local_tint.local_6 = 0xff;
            local_tint.local_7 = 0x80;
            local_tint.local_8 = 0;
        }
    }
applyTint:
    if (spawn == ACT_TARGET)
    {
        tmp1 = 0xff - ((int)(0xff - (u32)local_tint.local_6) / 10) * EQ_GLOBAL(int, 0x005917DC);
        tmp2 = 0xff - ((int)(0xff - (u32)local_tint.local_7) / 10) * EQ_GLOBAL(int, 0x005917DC);
        temp = 0xff - ((int)(0xff - (u32)local_tint.local_8) / 10) * EQ_GLOBAL(int, 0x005917DC);
        if (tmp1 < 0)
            tmp1 = 0;
        if (tmp2 < 0)
            tmp2 = 0;
        if (temp < 0)
            temp = 0;
        local_tint.local_6 = (u8)tmp1;
        local_tint.local_7 = (u8)tmp2;
        local_tint.local_8 = (u8)temp;
        if (EQ_GLOBAL(int, 0x005917DC) == 10)
            EQ_GLOBAL(int, 0x005917DC) = 0;
        else
            EQ_GLOBAL(int, 0x005917DC) = EQ_GLOBAL(int, 0x005917DC) + 1;
    }
    setStringSpriteTint = s3dSetStringSpriteTint;
    setStringSpriteTint(stringSprite, &local_tint.local_8);
    temp = *(int*)((char*)spawn->actor_info + 0x25c);
    if (temp != 0 && (stringSprite = t3dGetDagSprite((void*)temp), stringSprite != 0) && *stringSprite == 0x51)
        setStringSpriteTint(stringSprite, &local_tint.local_8);
    return 1;
}

// FUNCTION: TRILOGY 0x00412194
// Enables name labels for type-0 spawns and the character-name display setting.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::TurnCharacterNameSpritesOn()
{
    EQPlayer* p;

    g_pc_names_on_guess = 1;
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->type == 0)
            CDisplay_SetNameSpriteState_guess(this, p, 1);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004121ca
// Disables name labels for type-0 spawns and the character-name display setting.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::TurnCharacterNameSpritesOff()
{
    EQPlayer* p;

    g_pc_names_on_guess = 0;
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->type == 0)
            CDisplay_SetNameSpriteState_guess(this, p, 0);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004121fd
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::ShowNPCNameSprites_guess()
{
    EQPlayer* p;

    g_npc_names_flag_guess = 1;
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->type != 0)
            CDisplay_SetNameSpriteState_guess(this, p, 1);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00412233
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::HideNPCNameSprites_guess()
{
    EQPlayer* p;

    g_npc_names_flag_guess = 0;
    for (p = g_player_top_guess; p != 0; p = p->next)
    {
        if (p->type != 0)
            CDisplay_SetNameSpriteState_guess(this, p, 0);
    }
    return 1;
}

// FUNCTION: TRILOGY 0x00412266
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Body types 3, 5 and 8 keep their ambient.
int CDisplay::TurnInfravisionEffectOn_guess()
{


    EQPlayer* p = g_player_top_guess;
    infravision_on_guess = 1;
    while (p != 0)
    {
        if (p->body_type != 8 && p->body_type != 3 && p->body_type != 5)
            SetPlayerConstantAmbient_guess(p, infravision_ambient_guess);
        p = p->next;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004122ad
// Clears the ambient lighting override on every spawn.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::TurnInfravisionEffectOff()
{
    u8 black[3];
    EQPlayer* p;

    black[2] = 0;
    black[1] = 0;
    black[0] = 0;
    p = g_player_top_guess;
    infravision_on_guess = 0;
    while (p != 0)
    {
        SetPlayerConstantAmbient_guess(p, black);
        p = p->next;
    }
    return 1;
}

// FUNCTION: TRILOGY 0x004122ea
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// No null check on `player`.
// @early-stop: hier and i swap ESI/EDI (VC6 gives hier=ESI, i=EDI; original hier=EDI, i=ESI); rest identical.
/* fake match: the skin temp makes VC6 call s3dGetSkin before pushing `ambient`; inline, it pushes `ambient` first. */
void CDisplay::SetPlayerConstantAmbient_guess(EQPlayer* player, u8* ambient)
{
    EQActorInfo_guess* info = player->actor_info;
    void* hier;
    void* skin;
    int count;
    int i;

    if (info && (hier = info->hier_sprite) != 0)
    {
        count = s3dGetNumSkinsAttachedToHierarchicalSprite(hier);
        for (i = 0; i < count; i++)
        {
            skin = s3dGetSkinAttachedToHierarchicalSprite(i, hier);
            s3dSetDMSpriteConstantAmbient(skin, ambient);
        }
    }
}

// FUNCTION: TRILOGY 0x00412333
// Shows a blood spurt effect on the hit character.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Defines the "BSPURT_PCD" cloud once (when the I_BLOODSPRITE sprite exists), then spawns one bound to
// the dag. The definition and the cloud go to the global display's world, the dictionary lookup to this
// one's.
void CDisplay::SpurtBloodOnDag(void* dag)
{
    float location[3] = { 0.0f, 0.0f, 0.0f };
    float direction[3] = { 0.0f, 0.0f, 1.0f };
    void* sprite;
    void* def;

    if (blood_cloud_def_guess == 0)
    {
        float velocity[3] = { 0.0f, 0.0f, 0.0f };
        u8 tint[4] = { 0xFF, 0xFF, 0xFF };

        sprite = t3dGetPointerFromDictionary(world_guess, "I_BLOODSPRITE");
        if (sprite != 0)
        {
            blood_cloud_def_guess = _t3dDefineParticleCloud(g_cdisplay_guess->world_guess, "BSPURT_PCD", 3, 0x401,
                                                            0x64, 1.0f, direction, 0, 0, 1, 0x1F4, 0, 0, velocity,
                                                            0.0f, 128.0f, 0x1F4, 5.0f, 5, 0.25f, tint, sprite);
            if (blood_cloud_def_guess != 0)
                ((u32*)blood_cloud_def_guess)[3] |= 0x10;
        }
    }
    def = blood_cloud_def_guess;
    if (def != 0)
    {
        void* cloud = t3dCreateParticleCloud(g_cdisplay_guess->world_guess, def, location, 0, 0);
        t3dBindParticleCloudSpawn(cloud, dag);
    }
}

// FUNCTION: TRILOGY 0x00412445
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Sets the clock to `hour`:00 now, saves the date, redraws the sky.
void CDisplay::SetDayPeriod_guess(u8 hour)
{
    g_pWorldTime_guess->hour = hour;
    g_pWorldTime_guess->minute_guess = 0;
    g_pWorldTime_guess->set_time_ms_guess = EqGetTime();
    g_pWorldTime_guess->set_time_guess = time(0);
    g_pWorldTime_guess->saved_hour_guess = g_pWorldTime_guess->hour;
    g_pWorldTime_guess->saved_minute_guess = g_pWorldTime_guess->minute_guess;
    g_pWorldTime_guess->saved_day_guess = g_pWorldTime_guess->day_guess;
    g_pWorldTime_guess->saved_month_guess = g_pWorldTime_guess->month_guess;
    g_pWorldTime_guess->saved_year_guess = g_pWorldTime_guess->year_guess;
    CDisplay_SetSkyBackground_guess(this);
}

// FUNCTION: TRILOGY 0x004124be
// Turns the sky's dual-layer rendering on or off.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetSkyLayer(int dual)
{
    if (dual)
        t3dSetSkyDualLayer(g_skyLayer_guess, 1);
    else
        t3dSetSkyDualLayer(g_skyLayer_guess, 0);
}

// FUNCTION: TRILOGY 0x004124DB
// Updates the sky background and sun lighting to match the current world time.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: the next-phase local lives in a register (original spills it to the stack), so the steady-phase tails are not merged.
// display.h has no member for this method, so it is a __fastcall free function that takes the display in ECX:
// the same code as a __thiscall with no stack arguments. Callers use CDisplay_SetSkyBackground_guess.
void __fastcall SetSkyBackground_guess(CDisplay* self)
{
    int stepSeconds;
    int targetIndex;
    int waitSeconds;
    int tmp;
    int r1;
    int iDelta;
    int iBase;
    int iTemp;
    u32 uWeather;
    u8 hour;
    u8 minute;

    self->time_ms_guess = EQ_CDECL(u32, 0x004A461E, (void))();
    if (g_zone_sky_type_guess == 2 || g_zone_sky_type_guess == 1 || g_zone_sky_type_guess == 5)
    {
        hour = g_pWorldTime_guess->hour;
        minute = g_pWorldTime_guess->minute_guess;
    }
    else
    {
        hour = 0;
        minute = 0;
    }
    tmp = ((int)hour * 0x3c + (int)minute) / 0xc;
    r1 = (((int)hour + 0x15) * 0x3c + (int)minute) % 0x5a0;
    if (tmp < 0xf)
    {
        tmp = 0x2c;
        stepSeconds = 0;
        waitSeconds = (0x168 - r1 % 0x168) * 3;
        targetIndex = tmp;
    }
    else if (tmp < 0x2d)
    {
        tmp = (tmp + 0x1d) % 0x3c;
        targetIndex = 0xe;
        stepSeconds = 0x24;
        waitSeconds = 0;
    }
    else if (tmp < 0x4b)
    {
        tmp = 0xe;
        stepSeconds = 0;
        waitSeconds = (0x168 - r1 % 0x168) * 3;
        targetIndex = tmp;
    }
    else if (tmp < 0x69)
    {
        tmp = tmp - 0x3d;
        targetIndex = 0x2c;
        stepSeconds = 0x24;
        waitSeconds = 0;
    }
    else
    {
        tmp = 0x2c;
        stepSeconds = 0;
        waitSeconds = (0x168 - r1 % 0x168) * 3;
        targetIndex = 0x2c;
    }
    self->weather_ambient_index_guess = (u8)tmp;
    if ((u8)tmp > 0x3b)
        self->weather_ambient_index_guess = 0;
    if (tmp == 0xe && targetIndex == 0xe)
    {
        EQ_GLOBAL(float, 0x005917E8) = 128.0f;
        EQ_GLOBAL(float, 0x005917E4) = 448.0f;
        iBase = (waitSeconds - 0x438) * 1000 + self->time_ms_guess;
        EQ_GLOBAL(int, 0x005917F0) = 0x107AC0;
        EQ_GLOBAL(int, 0x0053C6B4) = 1;
        EQ_GLOBAL(int, 0x005917EC) = iBase;
    }
    else if (tmp == 0x2c && targetIndex == 0x2c)
    {
        EQ_GLOBAL(float, 0x005917E8) = 128.0f;
        EQ_GLOBAL(float, 0x005917E4) = 192.0f;
        iBase = (waitSeconds - 0x438) * 1000 + self->time_ms_guess;
        EQ_GLOBAL(int, 0x005917F0) = 0x107AC0;
        EQ_GLOBAL(int, 0x0053C6B4) = 1;
        EQ_GLOBAL(int, 0x005917EC) = iBase;
    }
    else if (targetIndex == 0xe)
    {
        iDelta = 0xe - tmp;
        if (iDelta < 0)
            iDelta += 0x3c;
        EQ_GLOBAL(float, 0x005917E8) = 128.0f;
        EQ_GLOBAL(float, 0x005917E4) = 320.0f;
        iBase = (iDelta * stepSeconds + waitSeconds - 0x438) * 1000 + self->time_ms_guess;
        EQ_GLOBAL(int, 0x005917F0) = 0x107AC0;
        EQ_GLOBAL(int, 0x0053C6B4) = iDelta;
        EQ_GLOBAL(int, 0x005917EC) = iBase;
    }
    else if (targetIndex == 0x2c)
    {
        iDelta = 0x2c - tmp;
        EQ_GLOBAL(float, 0x005917E8) = 128.0f;
        EQ_GLOBAL(float, 0x005917E4) = 64.0f;
        iBase = (iDelta * stepSeconds + waitSeconds - 0x438) * 1000 + self->time_ms_guess;
        EQ_GLOBAL(int, 0x005917F0) = 0x107AC0;
        EQ_GLOBAL(int, 0x0053C6B4) = iDelta;
        EQ_GLOBAL(int, 0x005917EC) = iBase;
    }
    else
        iBase = EQ_GLOBAL(int, 0x005917EC);
    /* FixHeading(heading), then FixHeading(heading + 256); each result is kept in ST0 for the next step. */
    EQ_GLOBAL(float, 0x0058FCAC) = EQ_METHOD(1, float, 0x00408156, (CDisplay *, float), self)(
        EQ_GLOBAL(float, 0x0058FCAC) = (EQ_GLOBAL(float, 0x0058FCA8) = EQ_METHOD(1, float, 0x00408156, (CDisplay *, float), self)(
            EQ_GLOBAL(float, 0x0058FCA8) = (float)(u32)(self->time_ms_guess - iBase) * EQ_GLOBAL(float, 0x005917E8)
                / EQ_GLOBAL(int, 0x005917F0) + EQ_GLOBAL(float, 0x005917E4))) + 256.0f);
    uWeather = self->weather_ambient_index_guess;
    EQ_GLOBAL(float, 0x0058FC9C) = 1.0f;
    EQ_GLOBAL(u32, 0x0058FC98) = uWeather;
    EQ_GLOBAL(int, 0x00590164) = self->time_ms_guess;
    iTemp = stepSeconds * 1000;
    EQ_GLOBAL(int, 0x005917F8) = iTemp;
    if (waitSeconds != 0)
        EQ_GLOBAL(int, 0x005917FC) = waitSeconds * 1000 + self->time_ms_guess;
    else
        EQ_GLOBAL(int, 0x005917FC) = iTemp + self->time_ms_guess;
    CDisplay_SetSunLight_guess(self);
}

// FUNCTION: TRILOGY 0x00412742
// Keeps the world's sky and lighting synchronized with in-game time.
// @early-stop: time-of-day sum starts with the p0 term (original: curTimeMs term); denominator temp slot and curTimeMs add register differ.
// display.h has no member for this method, so it is a __fastcall free function that takes the display in ECX:
// the same code as a __thiscall with no stack arguments.
// Day/night cycle, once a frame from RealRender_World: the time-of-day fraction (0058FCE8) from the game clock
// (zones 0x3f and 0x4c use hour 6, 0x47 hour 18, 0x7b hour 10, minute 0), the sun heading pair (0058FCA8/0058FCAC,
// only when 005917F0 is nonzero), two scroll offsets (0058FCA0/A4, wrapped at 1.0) and the ambient light. Between
// transitions (until 005917FC) it blends table entries idx and idx+1 (mod 60). At a transition idx steps (wrapped
// at 60) and the next transition is set; SetSkyBackground runs when 0053C6B4 was 0, else SetSunLight.
void __fastcall CDisplay_UpdateDayCycle_guess(CDisplay* self)
{
    int zone;
    int nowMs;
    u32 curTimeMs;
    int idx;
    u8* p;

    zone = g_zone_id_guess;
    /* zones 0x3f, 0x47, 0x4c and 0x7b set the hour and zero the minute; the default `continue` leaves the clock alone */
    do
    {
        switch (zone)
        {
        case 0x3f:
        case 0x4c:
            *EQ_GLOBAL(u8*, 0x006E4D54) = 6;
            break;
        case 0x47:
            *EQ_GLOBAL(u8*, 0x006E4D54) = 0x12;
            break;
        case 0x7b:
            *EQ_GLOBAL(u8*, 0x006E4D54) = 10;
            break;
        default:
            continue;
        }
        EQ_GLOBAL(u8*, 0x006E4D54)[1] = 0;
    } while (0);
    nowMs = EQ_CDECL(int, 0x004A461E, (void))();
    p = EQ_GLOBAL(u8*, 0x006E4D54);
    if (EQ_GLOBAL(u32, 0x00591804) == p[0] && EQ_GLOBAL(u32, 0x00591808) == p[1])
    {
        curTimeMs = nowMs - EQ_GLOBAL(int, 0x00591800);
    }
    else
    {
        EQ_GLOBAL(u32, 0x00591804) = p[0];
        EQ_GLOBAL(u32, 0x00591808) = p[1];
        EQ_GLOBAL(int, 0x00591800) = nowMs;
        curTimeMs = 0;
    }
    EQ_GLOBAL(float, 0x0058FCE8) = ((float)p[0] * 60.0f + (float)curTimeMs * 0.00033333333f + (float)p[1]) * 0.00069444446f;
    if (EQ_GLOBAL(int, 0x005917F0) != 0)
    {
        /* FixHeading(x) goes to 0058FCA8, then FixHeading(that + 256) to 0058FCAC; each value is kept in ST0. */
        EQ_GLOBAL(float, 0x0058FCAC) = EQ_FASTCALL(float, 0x00408156, (CDisplay *, float))(self,
            EQ_GLOBAL(float, 0x0058FCAC) = (EQ_GLOBAL(float, 0x0058FCA8) = EQ_FASTCALL(float, 0x00408156, (CDisplay *, float))(self,
                EQ_GLOBAL(float, 0x0058FCA8) = (float)(u32)(self->time_ms_guess - EQ_GLOBAL(int, 0x005917EC)) * EQ_GLOBAL(float, 0x005917E8)
                    / (float)EQ_GLOBAL(int, 0x005917F0) + EQ_GLOBAL(float, 0x005917E4))) + 256.0f);
    }
    EQ_GLOBAL(float, 0x0058FCA0) += 0.00025f;
    if (EQ_GLOBAL(float, 0x0058FCA0) >= 1.0f)
        EQ_GLOBAL(float, 0x0058FCA0) = 0.0f;
    EQ_GLOBAL(float, 0x0058FCA4) += 0.0025f;
    if (EQ_GLOBAL(float, 0x0058FCA4) >= 1.0f)
        EQ_GLOBAL(float, 0x0058FCA4) = 0.0f;
    curTimeMs = self->time_ms_guess;
    if (curTimeMs < EQ_GLOBAL(int, 0x005917FC))
    {
        EQ_GLOBAL(float, 0x0058FC9C) = 1.0f - (float)(curTimeMs - EQ_GLOBAL(int, 0x00590164))
            / (float)(EQ_GLOBAL(int, 0x005917FC) - EQ_GLOBAL(int, 0x00590164));
        idx = self->weather_ambient_index_guess;
        EQ_FASTCALL(void, 0x00412D2C, (CDisplay *, float))(self,
            EQ_GLOBAL(float, 0x0058FC9C) * g_weather_ambient_guess[idx]
            + (1.0f - EQ_GLOBAL(float, 0x0058FC9C)) * g_weather_ambient_guess[(idx + 1) % 60]);
    }
    else
    {
        curTimeMs += EQ_GLOBAL(int, 0x005917F8);
        EQ_GLOBAL(int, 0x005917FC) = curTimeMs;
        self->weather_ambient_index_guess++;
        if (self->weather_ambient_index_guess > 0x3b)
            self->weather_ambient_index_guess = 0;
        if (EQ_GLOBAL(int, 0x0053C6B4)-- != 0)
        {
            EQ_GLOBAL(float, 0x0058FC9C) = 1.0f;
            EQ_GLOBAL(u32, 0x0058FC98) = self->weather_ambient_index_guess;
            EQ_GLOBAL(int, 0x00590164) = self->time_ms_guess;
            EQ_METHOD(0, void, 0x0040D84F, (CDisplay *), self)();
        }
        else
        {
            EQ_METHOD(0, void, 0x004124DB, (CDisplay *), self)();
        }
    }
}

// FUNCTION: TRILOGY 0x00412984
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The original sums dz + dy + dx in that order (the order changes the rounding).
float CDisplay::SimpleDistance(float x1, float y1, float z1, float x2, float y2, float z2, float zScale)
{
    float dx;
    float dy;
    float dz;

    dx = (x1 - x2) < 0.0f ? -(x1 - x2) : (x1 - x2);
    dy = (y1 - y2) < 0.0f ? -(y1 - y2) : (y1 - y2);
    dz = 0.0f;
    if (zScale != 0.0f)
    {
        dz = (z1 - z2) < 0.0f ? -(z1 - z2) : (z1 - z2);
        if (zScale != 1.0f)
            dz = dz / zScale;
    }
    return dz + dy + dx;
}

// FUNCTION: TRILOGY 0x004129f3
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The sum is rounded to float when it is passed to t3dFloatSqrt, whose result is returned as is.
float CDisplay::TrueDistance(float x1, float y1, float z1, float x2, float y2, float z2, float zScale)
{
    float dx;
    float dy;
    float dz;

    dx = x1 - x2;
    dy = y1 - y2;
    if (zScale == 0.0f)
        return t3dFloatSqrt(dy * dy + dx * dx);
    dz = (z1 - z2) / zScale;
    return t3dFloatSqrt(dz * dz + dy * dy + dx * dx);
}

// FUNCTION: TRILOGY 0x00412a4e
// Measures approximate separation between two spawn positions.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
float CDisplay::PlayerSimpleDistance(EQPlayer* a, EQPlayer* b, float zScale)
{
    if (a == 0 || b == 0)
        return -1.0e27f;
    return SimpleDistance(a->y, a->x, a->z, b->y, b->x, b->z, zScale);
}

// FUNCTION: TRILOGY 0x00412aa0
// Measures straight-line separation between two spawn positions.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
float CDisplay::PlayerDistance(EQPlayer* firstSpawn, EQPlayer* secondSpawn, float zScale)
{
    if (firstSpawn == 0 || secondSpawn == 0)
        return -1.0e27f;
    return TrueDistance(firstSpawn->y, firstSpawn->x, firstSpawn->z, secondSpawn->y, secondSpawn->x, secondSpawn->z, zScale);
}

// FUNCTION: TRILOGY 0x00412af2
// Loads entries from the active zone's .dat file for world setup.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Not a CDisplay member: display.h has no declaration for it, so this is a file-local __fastcall
// function with the thiscall layout (this in ECX, EDX unused, one stack argument, callee pops 4).
void __fastcall loadGrassObjects(CDisplay* self, int unused, const char* activeZone)
{
    typedef void (__cdecl *t3dSetRadialFloraObjectList_guess_t)(void*, void*, int);
    typedef void (__cdecl *t3dSetRadialFloraRadius_guess_t)(float);
    typedef void (__cdecl *t3dSetRadialFloraCenter_guess_t)(float, float, float);
    struct Vec3_guess
    {
        float x;
        float y;
        float z;
    };
    // One 0x20-byte record of the zone's grass .dat file.
    struct GrassFileEntry_guess
    {
        Vec3_guess position;    // 0x00
        Vec3_guess normal;      // 0x0c
        float height;           // 0x18
        u32 type;               // 0x1c
    };
    // One 0x34-byte radial flora object handed to the 3D world.
    struct GrassObject_guess
    {
        Vec3_guess base;        // 0x00
        Vec3_guess baseNormal;  // 0x0c
        Vec3_guess top;         // 0x18
        Vec3_guess topNormal;   // 0x24
        u32 type;               // 0x30
    };
    char path[256];
    FILE* file;
    int i;

    if (EQ_GLOBAL(int, 0x006E5480) == 1)
    {
        EQ_GLOBAL(t3dSetRadialFloraObjectList_guess_t, 0x006E51EC)(self->world_guess, 0, 0);
        SOC_CPY(path, activeZone);
        SWD_STRCAT(path, EQ_ARRAY(char, 0x0053E698));
        if (EQ_GLOBAL(void*, 0x005917A4) != 0)
        {
            EQ_CDECL(void, 0x005230DD, (void*))(EQ_GLOBAL(void*, 0x005917A4));
            EQ_GLOBAL(void*, 0x005917A4) = 0;
        }
        EQ_GLOBAL(int, 0x005917A8) = 0;
        file = EQ_CDECL(FILE*, 0x005223BC, (const char*, const char*))(path, EQ_ARRAY(char, 0x0053C2BC));
        if (file != 0)
        {
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 2);
            EQ_GLOBAL(int, 0x005917A8) = EQ_CDECL(u32, 0x0052214F, (FILE*))(file) >> 5;
            EQ_CDECL(int, 0x005222D2, (FILE*, long, int))(file, 0, 0);
            EQ_GLOBAL(void*, 0x005917A4) = EQ_CDECL(void*, 0x00522FA3, (size_t))(
                (EQ_GLOBAL(int, 0x005917A8) + 1) * 0x20);
            if (EQ_GLOBAL(void*, 0x005917A4) == 0)
            {
                EQ_GLOBAL(int, 0x005917A8) = 0;
            }
            else
            {
                EQ_CDECL(size_t, 0x00522038, (void*, size_t, size_t, FILE*))(
                    EQ_GLOBAL(void*, 0x005917A4), 0x20, EQ_GLOBAL(int, 0x005917A8), file);
            }
            EQ_CDECL(int, 0x00521FBB, (FILE*))(file);
        }
        if (EQ_GLOBAL(void*, 0x005917A4) != 0)
        {
            EQ_GLOBAL(void*, 0x005917A0) = EQ_CDECL(void*, 0x00522FA3, (size_t))(
                (EQ_GLOBAL(int, 0x005917A8) + 1) * 0x34);
            if (EQ_GLOBAL(void*, 0x005917A0) != 0)
            {
                for (i = 0; i < EQ_GLOBAL(int, 0x005917A8); i++)
                {
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].type = EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].type;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].base = EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].position;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].baseNormal = EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].normal;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].top = EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].position;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].topNormal = EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].normal;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].top.z += EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].height;
                    EQ_GLOBAL(GrassObject_guess*, 0x005917A0)[i].topNormal.z += EQ_GLOBAL(GrassFileEntry_guess*, 0x005917A4)[i].height;
                }
                EQ_CDECL(void, 0x005230DD, (void*))(EQ_GLOBAL(void*, 0x005917A4));
                EQ_GLOBAL(void*, 0x005917A4) = 0;
                EQ_GLOBAL(t3dSetRadialFloraObjectList_guess_t, 0x006E51EC)(
                    self->world_guess, EQ_GLOBAL(void*, 0x005917A0), EQ_GLOBAL(int, 0x005917A8));
                EQ_GLOBAL(t3dSetRadialFloraRadius_guess_t, 0x006E51F4)(EQ_GLOBAL(float, 0x006E20C8));
                EQ_GLOBAL(t3dSetRadialFloraCenter_guess_t, 0x006E51F0)(0.0f, 0.0f, 0.0f);
            }
            else
            {
                EQ_CDECL(void, 0x005230DD, (void*))(EQ_GLOBAL(void*, 0x005917A4));
                EQ_GLOBAL(void*, 0x005917A4) = 0;
                EQ_GLOBAL(int, 0x005917A8) = 0;
            }
        }
    }
}

// FUNCTION: TRILOGY 0x00412d2c
// Sets the ambient light definition (hour-blended in zone types 1/2/5, else fixed) and the matching fog colour.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Not a CDisplay member: display.h has no declaration for it, so this is a file-local __fastcall
// function with the thiscall layout (this in ECX, EDX unused, one stack argument, callee pops 4).
// The stack argument is reused as scratch, as in the original: it is written as a short, an int or a
// float, and its final dword is stored in this+0x38. The scratch writes go through casts.
void __fastcall SetAmbientLight_guess(CDisplay* self, int unused, float lightLevel)
{
    typedef void (__cdecl *s3dSetPrecalcLighting_guess_t)(int);
    typedef void* (__cdecl *t3dGetPointerFromDictionary_guess_t)(void*, const char*);
    typedef void (__cdecl *t3dSetLightDefinitionColorIntensityList_guess_t)(void*, float*);
    typedef float (__cdecl *s3dGetActorScaledAmbient_guess_t)(void*, void*, void*);
    typedef void (__cdecl *s3dSetWorldSpecialAmbient_guess_t)(void*, u8*);

    float lightColor[3];
    float ambient[3];
    // volatile: the original stores these to their stack slots (FSTP) and reloads them, so each read
    // sees the rounded float; without it VC6 reuses the unrounded FPU value after the store.
    volatile float actorScale;
    volatile float tintR;
    volatile float tintG;
    volatile float tintB;
    volatile float tmp;
    float greenComp;
    int unusedLocal;
    u8 rgb[3];
    int useTintA;
    int useTintB;
    void* lightDef;
    void* actor;
    float scale;
    volatile float prev; // the original reads and writes the global DAT_0053c6b8 directly
    float k;
    int t;
    int red;
    int blue;
    float start;
    float end;
    char* ev;
    u8* pc;
    t3dGetPointerFromDictionary_guess_t lookup;
    u8 zoneType;
    u8 mode;

    if (self->world_guess == 0)
        return;
    if (self->is3dOn_guess == 0)
        return;
    useTintA = 0;
    useTintB = 0;
    mode = EQ_GLOBAL(u8, 0x0058FBB8);
    if (self->unk_0x0C1 != 0 && mode != 7 && mode != 8)
    {
        EQ_GLOBAL(s3dSetPrecalcLighting_guess_t, 0x006E5144)(0);
        if (self->infravision_on_guess != 0)
            EQ_METHOD(0, void, 0x004122AD, (CDisplay *), self)();
        lookup = EQ_GLOBAL(t3dGetPointerFromDictionary_guess_t, 0x006E52E0);
        lightDef = lookup(self->world_guess, EQ_ARRAY(char, 0x0053E6B8));
        if (lightDef == 0)
        {
            lightDef = lookup(self->world_guess, EQ_ARRAY(char, 0x0053E6A0));
            if (lightDef == 0)
                return;
        }
        lightColor[0] = EQ_GLOBAL(float, 0x00535234);
        lightColor[1] = lightColor[0];
        lightColor[2] = lightColor[0];
        EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t, 0x006E53F0)(lightDef, lightColor);
        actor = EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->actor_info->actor;
        if (actor != 0)
            scale =EQ_GLOBAL(s3dGetActorScaledAmbient_guess_t, 0x006E50F8)(self->world_guess, self->render_guess, actor);
        else
            scale = 1.0f;
        ev = (char*)EQ_GLOBAL(void*, 0x006EFE2C);
        *(u16*)&lightLevel = *(u16*)(ev + 0x223FA);
        t = (int)(scale * EQ_GLOBAL(float, 0x00535230));
        red = *(u16*)(ev + 0x223F8);
        if ((short)red < (short)t)
            red = t;
        if (*(short*)&lightLevel < (short)t)
            *(int*)&lightLevel = t;
        blue = *(u16*)(ev + 0x223FC);
        if ((short)blue < (short)t)
            blue = t;
        if (mode == 5)
        {
            start = 0.0f;
            end = EQ_GLOBAL(float, 0x00535134);
        }
        else if (mode == 6)
        {
            start = 0.0f;
            end = EQ_GLOBAL(float, 0x00535190);
        }
        else
        {
            start = self->fogStart_guess;
            end = self->fogEnd_guess;
        }
        EQ_METHOD(6, void, 0x0040F518, (CDisplay *, int, float, float, int, int, int), g_cdisplay_guess)(1, start, end, red, *(int*)&lightLevel, blue);
        return;
    }

    pc = (u8*)EQ_GLOBAL(void*, 0x006E4DA8);
    useTintA = pc[0xA32] != 0xFF;
    if (pc[0xA39] != 0xFF)
    {
        useTintA = 0;
        useTintB = 1;
    }
    if (lightLevel >= EQ_GLOBAL(float, 0x0053522C))
    {
        if (lightLevel > EQ_GLOBAL(float, 0x00535228))
        {
            EQ_GLOBAL(s3dSetPrecalcLighting_guess_t, 0x006E5144)(0);
            if (self->infravision_on_guess != 0)
                EQ_METHOD(0, void, 0x004122AD, (CDisplay *), self)();
            *(int*)&lightLevel = 0x3F333333;
        }
    }
    else
    {
        EQ_GLOBAL(s3dSetPrecalcLighting_guess_t, 0x006E5144)(1);
        if (useTintA != 0 && self->infravision_on_guess == 0)
            EQ_METHOD(0, void, 0x00412266, (CDisplay *), self)();
    }
    if (lightLevel < EQ_GLOBAL(float, 0x005351C4))
        lightLevel = EQ_GLOBAL(float, 0x005351C4);

    lookup = EQ_GLOBAL(t3dGetPointerFromDictionary_guess_t, 0x006E52E0);
    lightDef = lookup(self->world_guess, EQ_ARRAY(char, 0x0053E6B8));
    if (lightDef == 0)
    {
        lightDef = lookup(self->world_guess, EQ_ARRAY(char, 0x0053E6A0));
        if (lightDef == 0)
            return;
    }

    ambient[0] = lightLevel;
    ambient[1] = lightLevel;
    ambient[2] = lightLevel;
    zoneType = g_zone_sky_type_guess;
    if (zoneType == 2 || zoneType == 1 || zoneType == 5)
    {
        EQWorldTime_guess* worldTime = g_pWorldTime_guess;
        u8 hour = worldTime->hour;
        u8 hourPlus;
        u8 hourMinus;
        float minuteF;
        float* tr;
        float* tg;
        float* tb;
        u8 idx;

        *(int*)&lightLevel = worldTime->minute_guess;
        minuteF = (float)*(int*)&lightLevel;
        hourPlus = hour + 1;
        if (hourPlus > 0x18)
            hourPlus = 1;
        hourMinus = hour - 1;
        idx = hourPlus - 1;
        k = EQ_GLOBAL(float, 0x005351A8);
        tr = EQ_ARRAY(float, 0x005767D0);
        tg = EQ_ARRAY(float, 0x00576830);
        tb = EQ_ARRAY(float, 0x00576890);
        ambient[0] = (tr[idx] - tr[hourMinus]) * minuteF * k + tr[hourMinus];
        ambient[1] = (tg[idx] - tg[hourMinus]) * minuteF * k + tg[hourMinus];
        ambient[2] = (tb[idx] - tb[hourMinus]) * minuteF * k + tb[hourMinus];
        if (ambient[1] < EQ_GLOBAL(float, 0x0053510C))
            ambient[1] = 0.0f;
        if (ambient[0] < EQ_GLOBAL(float, 0x0053510C))
            ambient[0] = 0.0f;
        if (ambient[2] < EQ_GLOBAL(float, 0x0053510C))
            ambient[2] = 0.0f;
        if (ambient[1] >= EQ_GLOBAL(float, 0x005344A4))
            ambient[1] = EQ_GLOBAL(float, 0x00535234);
        if (ambient[0] >= EQ_GLOBAL(float, 0x005344A4))
            ambient[0] = EQ_GLOBAL(float, 0x00535234);
        if (ambient[2] >= EQ_GLOBAL(float, 0x005344A4))
            ambient[2] = EQ_GLOBAL(float, 0x00535234);
    }
    else
    {
        if (zoneType == 4)
            k = EQ_GLOBAL(float, 0x005344A8);
        else
            k = EQ_GLOBAL(float, 0x005351C4);
        ambient[1] = k;
        ambient[2] = k;
        ambient[0] = k;
    }

    actor = EQ_GLOBAL(EQPlayer*, 0x006E4DA0)->actor_info->actor;
    if (actor != 0)
        actorScale =EQ_GLOBAL(s3dGetActorScaledAmbient_guess_t, 0x006E50F8)(self->world_guess, self->render_guess, actor);
    else
        actorScale = 1.0f;

    prev = EQ_GLOBAL(float, 0x0053C6B8);
    if (prev == EQ_GLOBAL(float, 0x005345BC))
    {
        prev = actorScale;
        EQ_GLOBAL(float, 0x0053C6B8) = actorScale;
    }
    k = EQ_GLOBAL(float, 0x00535224);
    if (actorScale < prev)
    {
        if (prev - k >= actorScale)
            prev = prev - k;
        else
            prev = actorScale;
        actorScale = prev;
        EQ_GLOBAL(float, 0x0053C6B8) = prev;
    }
    else if (actorScale > prev)
    {
        if (prev + k <= actorScale)
            prev = prev + k;
        else
            prev = actorScale;
        actorScale = prev;
        EQ_GLOBAL(float, 0x0053C6B8) = prev;
    }

    if (EQ_GLOBAL(int, 0x006E4E6C) != 0)
    {
        useTintA = 0;
        useTintB = 0;
        EQ_METHOD(0, void, 0x004122AD, (CDisplay *), self)();
        EQ_GLOBAL(u8, 0x006B66C3) = 0xC0;
        EQ_GLOBAL(u8, 0x006B66C7) = 0xC0;
        ambient[1] = EQ_GLOBAL(float, 0x005351C4);
        ambient[2] = ambient[1];
        ambient[0] = ambient[1];
        EQ_GLOBAL(u8, 0x006B66CB) = 0xC0;
        EQ_GLOBAL(float, 0x006B66D0) = 100.0f;
        actorScale = 1.0f;
        EQ_GLOBAL(float, 0x006B66E0) = 500.0f;
    }

    if (useTintA != 0 || useTintB != 0)
    {
        int comp0 = useTintA != 0 ? EQ_GLOBAL(int, 0x006B64EC) : EQ_GLOBAL(int, 0x006B64F8);
        int comp1 = useTintA != 0 ? EQ_GLOBAL(int, 0x006B64F0) : EQ_GLOBAL(int, 0x006B64FC);
        int comp2 = useTintA != 0 ? EQ_GLOBAL(int, 0x006B64F4) : EQ_GLOBAL(int, 0x006B6500);
        float kx = EQ_GLOBAL(float, 0x00535138);
        float kc0 = EQ_GLOBAL(float, 0x005351C0);
        int xbx;
        int xdi;
        int xsi;
        int rR;
        int rG;
        int eaxLast;
        int v;
        float fr;
        float fg;
        float fb;

        tintR = (float)comp0 * kx;
        tintG = (float)comp1 * kx;
        if (ambient[0] < tintR)
            ambient[0] = tintR;
        if (ambient[1] < tintG)
            ambient[1] = tintG;
        if (ambient[2] < (float)comp2 * kx)
            ambient[2] = (float)comp2 * kx;
        tintB = (1.0f - actorScale) * ((float)comp2 * kx);
        tintR = (1.0f - actorScale) * tintR;
        tintG = (1.0f - actorScale) * tintG;
        tmp = tintR * EQ_GLOBAL(float, 0x00535230);
        rgb[2] = (u8)(int)(tintR * EQ_GLOBAL(float, 0x00535230));
        rgb[1] = (u8)(int)(tintG * EQ_GLOBAL(float, 0x00535230));
        rgb[0] = (u8)(int)(tintB * EQ_GLOBAL(float, 0x00535230));
        EQ_GLOBAL(s3dSetWorldSpecialAmbient_guess_t, 0x006E5154)(self->world_guess, rgb);
        EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t, 0x006E53F0)(lightDef, ambient);

        mode = EQ_GLOBAL(u8, 0x0058FBB8);
        if (mode == 5)
        {
            v = (short)(ambient[0] * EQ_GLOBAL(float, 0x00535220));
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[1] * EQ_GLOBAL(float, 0x0053521C));
            xdi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[2] * EQ_GLOBAL(float, 0x00535218));
            xsi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tintR * EQ_GLOBAL(float, 0x00535220));
            rR = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintG * EQ_GLOBAL(float, 0x0053521C));
            rG = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintB * EQ_GLOBAL(float, 0x00535218));
            eaxLast = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
        }
        else if (mode == 6)
        {
            v = (short)(ambient[0] * EQ_GLOBAL(float, 0x0053521C));
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[1] * EQ_GLOBAL(float, 0x0053521C));
            xdi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            xsi = 0;
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tintR * EQ_GLOBAL(float, 0x0053521C));
            rR = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintG * EQ_GLOBAL(float, 0x0053521C));
            rG = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            eaxLast = 0;
        }
        else if (mode == 7)
        {
            v = (short)(ambient[0] * EQ_GLOBAL(float, 0x00535230));
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            xdi = 0;
            xsi = 0;
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)tmp;
            rR = (int)((float)v * lightLevel);
            rG = 0;
            eaxLast = 0;
        }
        else if (mode == 8)
        {
            xbx = 0;
            xdi = 0;
            v = (short)(ambient[2] * EQ_GLOBAL(float, 0x00535214));
            xsi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tintB * EQ_GLOBAL(float, 0x00535214));
            eaxLast = (int)((float)v * lightLevel);
        }
        else if (mode == 9)
        {
            v = (short)(ambient[0] * EQ_GLOBAL(float, 0x00535210));
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[1] * EQ_GLOBAL(float, 0x0053520C));
            xdi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[2] * EQ_GLOBAL(float, 0x00535208));
            xsi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tintR * EQ_GLOBAL(float, 0x00535210));
            rR = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintG * EQ_GLOBAL(float, 0x0053520C));
            rG = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintB * EQ_GLOBAL(float, 0x00535208));
            eaxLast = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
        }
        else if (mode == 10)
        {
            v = (short)(ambient[0] * EQ_GLOBAL(float, 0x00535204));
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[1] * EQ_GLOBAL(float, 0x00535200));
            xdi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            v = (short)(ambient[2] * EQ_GLOBAL(float, 0x005351FC));
            xsi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tintR * EQ_GLOBAL(float, 0x00535204));
            rR = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintG * EQ_GLOBAL(float, 0x00535200));
            rG = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(tintB * EQ_GLOBAL(float, 0x005351FC));
            eaxLast = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
        }
        else
        {
            fr = (float)EQ_GLOBAL(u8, 0x006B66C3);
            tmp = fr;
            v = (short)(fr * ambient[0]);
            xbx = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            fg = (float)EQ_GLOBAL(u8, 0x006B66C7);
            greenComp = fg;
            v = (short)(fg * ambient[1]);
            xdi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            fb = (float)EQ_GLOBAL(u8, 0x006B66CB);
            v = (short)(fb * ambient[2]);
            xsi = (int)((float)v * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
            lightLevel = ((tintB + tintG) + tintR) * kc0;
            v = (short)(tmp * tintR);
            rR = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(greenComp * tintG);
            rG = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
            v = (short)(fb * tintB);
            eaxLast = (int)((float)v * (((tintB + tintG) + tintR) * kc0));
        }

        if (mode != 8)
        {
            if ((short)rR > (short)xbx)
                xbx = rR;
            if ((short)rG > (short)xdi)
                xdi = rG;
        }
        if ((short)eaxLast > (short)xsi)
            xsi = eaxLast;
        pinstCEverQuest_guess->timedFogColor_guess[0] = (short)xbx;
        pinstCEverQuest_guess->timedFogColor_guess[1] = (short)xdi;
        pinstCEverQuest_guess->timedFogColor_guess[2] = (short)xsi;
    }
    else
    {
        u8 zeroRgb[3];
        float kc0 = EQ_GLOBAL(float, 0x005351C0);
        int nR;
        int nG;
        int nB;
        char* ev;

        zeroRgb[0] = 0;
        zeroRgb[1] = 0;
        zeroRgb[2] = 0;
        EQ_GLOBAL(s3dSetWorldSpecialAmbient_guess_t, 0x006E5154)(self->world_guess, zeroRgb);
        EQ_GLOBAL(t3dSetLightDefinitionColorIntensityList_guess_t, 0x006E53F0)(lightDef, ambient);
        lightLevel = (ambient[2] + ambient[1] + ambient[0]) * kc0;

        mode = EQ_GLOBAL(u8, 0x0058FBB8);
        if (mode == 5)
        {
            nR = (int)(ambient[0] * EQ_GLOBAL(float, 0x00535220));
            nG = (int)(ambient[1] * EQ_GLOBAL(float, 0x0053521C));
            nB = 0;
        }
        else if (mode == 6)
        {
            nR = (int)(ambient[0] * EQ_GLOBAL(float, 0x0053521C));
            nG = (int)(ambient[1] * EQ_GLOBAL(float, 0x0053521C));
            nB = 0;
        }
        else if (mode == 7)
        {
            nR = (int)(ambient[0] * EQ_GLOBAL(float, 0x00535230));
            nG = 0;
            nB = 0;
        }
        else if (mode == 8)
        {
            nR = 0;
            nG = 0;
            nB = (int)(ambient[2] * EQ_GLOBAL(float, 0x00535214));
        }
        else if (mode == 9)
        {
            nR = (int)(ambient[0] * EQ_GLOBAL(float, 0x005351F8));
            nG = 0;
            nB = 0;
        }
        else if (mode == 10)
        {
            nR = (int)(ambient[0] * EQ_GLOBAL(float, 0x00535204));
            nG = (int)(ambient[1] * EQ_GLOBAL(float, 0x00535200));
            nB = (int)(ambient[2] * EQ_GLOBAL(float, 0x005351FC));
        }
        else
        {
            nR = (int)((float)EQ_GLOBAL(u8, 0x006B66C3) * ambient[0]);
            nG = (int)((float)EQ_GLOBAL(u8, 0x006B66C7) * ambient[1]);
            nB = (int)((float)EQ_GLOBAL(u8, 0x006B66CB) * ambient[2]);
        }
        ev = (char*)EQ_GLOBAL(void*, 0x006EFE2C);
        *(short*)(ev + 0x223F8) = (short)(int)((float)(short)nR * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
        *(short*)(ev + 0x223FA) = (short)(int)((float)(short)nG * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
        *(short*)(ev + 0x223FC) = (short)(int)((float)(short)nB * actorScale * ((ambient[2] + ambient[1] + ambient[0]) * kc0));
    }

    EQ_METHOD(1, void, 0x004D9D9E, (void *, int), (void *)EQ_GLOBAL(void*, 0x006EFE2C))(0);
    ((int *)self)[0xE] = *(int *)&lightLevel;
}

// FUNCTION: TRILOGY 0x00413ed6
// Changes the view to a named player's perspective and reports success or failure in chat.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetViewActorByName(const char* name)
{
    char message[128];
    EQPlayer* player;
    void* actor;
    void* currentView;
    char* trimmedName;

    player = EQ_METHOD(1, EQPlayer*, 0x004B1435, (EQPlayer*, const char*), g_player_top_guess)(name);
    if ((player == 0 &&
         (player = EQ_METHOD(1, EQPlayer*, 0x004B146A, (EQPlayer*, const char*), g_player_top_guess)(name)) == 0) ||
        player->actor_info == 0 ||
        (actor = *(void**)player->actor_info) == 0)
    {
        sprintf(message, EQ_ARRAY(const char, 0x0053E6CC), name);
    }
    else
    {
        currentView = g_view_actor_guess;
        if (currentView == actor)
            return;
        if (currentView == 0)
        {
            EQ_METHOD(1, void, 0x0040E422, (CDisplay*, void*), this)(actor);
            return;
        }

        EQ_METHOD(1, void, 0x0040E422, (CDisplay*, void*), this)(actor);
        if (player == g_local_player_guess)
        {
            strcpy(message, EQ_ARRAY(const char, 0x0053E714));
        }
        else
        {
            trimmedName = EQ_METHOD(1, char*, 0x004D02FD, (CEverQuest*, char*), pinstCEverQuest_guess)((char*)player + 1);
            sprintf(message, EQ_ARRAY(const char, 0x0053E6EC), trimmedName);
        }
    }

    EQ_METHOD(2, void, 0x004D055E, (CEverQuest*, const char*, int), pinstCEverQuest_guess)(message, 0x0d);
}

// FUNCTION: TRILOGY 0x00413f8f
// Gets an actor's current position in the world.
bool CDisplay::GetActorWorldPosition_guess(void* actor, float* position)
{
    void* actorInfo;
    void* transformOwner;
    void* actorHandle;
    void* currentDag;
    void* renderWorld;
    float transform[9];
    float worldPosition[3];

    if (actor != 0)
    {
        actorInfo = *(void**)((char*)actor + 0x70);
        transformOwner = *(void**)((char*)actorInfo + 0x250);
        if (transformOwner != 0)
        {
            renderWorld = this->world_guess;
            if (renderWorld != 0)
            {
                actorHandle = *(void**)actorInfo;
                if (actorHandle != 0)
                {
                    currentDag = *(void**)((char*)actorInfo + 0x254);
                    if (currentDag != 0)
                    {
                        EQ_GLOBAL(t3dHierarchicalSpriteUpdateAllTransformations_guess_t, 0x006E5318)
                            (transformOwner, renderWorld, actorHandle);
                        EQ_GLOBAL(t3dGetCurrentDagObjectToWorldTransform_guess_t, 0x006E52AC)
                            (currentDag, transform, worldPosition);

                        position[0] = worldPosition[0];
                        position[1] = worldPosition[1];
                        position[2] = worldPosition[2];
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// FUNCTION: TRILOGY 0x00413ffd
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// No null check on the sprite. Only blit sprites (type 8) take a tint.
void CDisplay::SetActorSpriteTint(u8* rgb, void* actor)
{
    int* sprite = t3dGetActorSprite(actor, 0, 0);
    BlitTint_guess tint;

    if (*sprite == 8)
    {
        tint.r = rgb[0];
        tint.g = rgb[1];
        tint.b = rgb[2];
        t3dSetBlitSpriteTint(render_guess, sprite, &tint);
    }
}

// FUNCTION: TRILOGY 0x004141c6
// Filters actors for world collision checks, skipping selected actors and the player's own actor.
int CDisplay::IsActorCollidable_guess(void* actor, void* player)
{
    int* collisionVolume;
    void* userData;

    collisionVolume = EQ_GLOBAL(t3dGetActorCollisionVolume_guess_t, 0x006E526C)(actor);
    userData = EQ_GLOBAL(t3dGetActorUserData_guess_t, 0x006E528C)(actor);

    if (0x10 < (unsigned int)userData)
    {
        switch (*(u8*)userData)
        {
        case 1:
            return 0;
        case 3:
            if (player != 0)
            {
                void* playerActorInfo = *(void**)((char*)player + 0x70);
                if (actor == *(void**)playerActorInfo)
                    return 0;
            }
            break;
        }
    }

    if (collisionVolume == 0 || *collisionVolume == 0x2e)
        return 1;
    return 0;
}

// FUNCTION: TRILOGY 0x00414245
// Finds ground height below a point for general terrain and object placement.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: ebx/edi swapped (callback vs collide function pointer); registers only
/* fake match: 0x00414041 is written two ways so VC6 does not keep it in a register, which lets the cached collide pointer take one; threshold is read through a pointer so the NaN compare matches the original */
float CDisplay::GetFloorHeight(float y, float x, float z)
{
    float start[7];
    float normal[3];
    float hit[3];
    float end[3];
    float height = 0.0f;
    int result;
    const float* threshold = &EQ_GLOBAL(float, 0x00535240);
    s3dCollideSphereWithWorld_guess_t collideSphere;

    if (is3dOn_guess != 0)
    {
        start[0] = y;
        start[1] = x;
        start[2] = z;
        start[2] += EQ_GLOBAL(float, 0x005344A8);
        start[3] = 0.0f;
        start[4] = 0.0f;
        start[5] = 0.0f;

        result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)(
            world_guess, start);
        if (result != 0)
        {
            end[0] = y;
            end[1] = x;
            end[2] = -2000.0f;
            collideSphere = EQ_GLOBAL(s3dCollideSphereWithWorld_guess_t, 0x006E50C8);
            result = collideSphere(
                world_guess, render_guess, start, end,
                *(float*)((char*)this->collisionSphere_guess + 8),
                (void*)EQ_CDECL(int, 0x004141C6, (void*, void*)),
                (void*)EQ_CDECL(int, 0x00414041, (void*, void*)),
                0, hit, normal, &z, 0);
            if (result == 0)
            {
                if (hit[2] > *threshold)
                    return hit[2] - this->collisionSphereScale_guess + EQ_GLOBAL(float, 0x005344A0);
                end[0] = y;
                end[1] = x;
                end[2] = -32767.0f;
                result = collideSphere(
                    world_guess, render_guess, start, end,
                    *(float*)((char*)this->collisionSphere_guess + 8),
                    (void*)EQ_CDECL(int, 0x004141C6, (void*, void*)),
                    (void*)0x00414041,
                    0, hit, normal, &z, 0);
                if (result == 0)
                    return hit[2] - this->collisionSphereScale_guess + EQ_GLOBAL(float, 0x005344A0);
            }
        }
        height = EQ_GLOBAL(float, 0x005351F4);
    }
    return height;
}

// FUNCTION: TRILOGY 0x0041436d
// Gets a player's ground height for movement and teleport placement.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: 0.25 is read live from 0x00535154 (fixture case 12), so VC6 loads it before the player field (fld/fmul order); the 0.5 and 0.0 constants are literal pool refs
float CDisplay::GetPlayerFloorHeight_guess(void* player, float y, float x, float z, char clearActorState)
{
    float scratch[16];
    float height = 0.0f;
    u32 playerActorInfo;
    u32 actorForCallback;
    int result;

    if (this->is3dOn_guess != 0)
    {
        actorForCallback = 0;
        scratch[0] = y;
        scratch[1] = x;
        scratch[2] = z + 0.5f;
        scratch[3] = 0.0f;
        scratch[4] = 0.0f;
        scratch[5] = 0.0f;
        if (EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)(
                this->world_guess, scratch) != 0)
        {
            scratch[13] = y;
            scratch[14] = x;
            scratch[15] = -32767.0f;
            // The radius reuses the y parameter's slot, as in the original. The zero test sees the
            // unrounded product, so a subnormal that rounds to 0 keeps radius 0; NaN takes the fallback.
            y = *(float*)((char*)player + 0x7c) * EQ_GLOBAL(float, 0x00535154);
            if (y == 0.0f)
                y = 0.1f;

            if (clearActorState != 0)
            {
                playerActorInfo = *(u32*)((char*)player + 0x70);
                *(u32*)(playerActorInfo + 0x1b8) = 0;
                playerActorInfo = *(u32*)((char*)player + 0x70);
                if (playerActorInfo != 0)
                    actorForCallback = *(u32*)playerActorInfo;
            }

            result = EQ_GLOBAL(s3dCollideSphereWithWorld_guess_t, 0x006E50C8)(
                this->world_guess, *(void**)((char*)this), scratch, scratch + 13,
                y, (void*)0x004141C6, (void*)0x00414041, (u32)player, scratch + 10,
                scratch + 7, &z, actorForCallback);
            if (result == 0)
                return scratch[12] - y + EQ_GLOBAL(float, 0x005344A0);
        }
        height = EQ_GLOBAL(float, 0x005351F4);
    }
    return height;
}

// FUNCTION: TRILOGY 0x00414468
// Finds the ceiling above a point for overhead clearance checks.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
float CDisplay::GetCeilingHeight(u32 y, u32 x, u32 z, float radiusMultiplier)
{
    u32 scratch[16];
    void* rayParameters;
    float radius;
    int result;

    if (this->is3dOn_guess == 0)
        return 0.0f;

    scratch[0] = y;
    scratch[1] = x;
    scratch[2] = z;
    scratch[3] = 0;
    scratch[4] = 0;
    scratch[5] = 0;
    result = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)(
        this->world_guess, (float*)scratch);
    if (result == 0)
        return EQ_GLOBAL(float, 0x005351F4);

    scratch[13] = y;
    scratch[14] = x;
    scratch[15] = 0x46fffe00;
    rayParameters = this->collisionSphere_guess;
    radius = radiusMultiplier * *(float*)((char*)rayParameters + 8);
    result = EQ_GLOBAL(s3dCollideSphereWithWorld_guess_t, 0x006E50C8)(
        this->world_guess, *(void**)((char*)this), (float*)scratch,
        (float*)(scratch + 13), radius, (void*)0x004a3d01, (void*)0x00414147,
        0, scratch + 10, scratch + 7, (float*)&z, 0);
    if (result != 0)
        return EQ_GLOBAL(float, 0x005351F4);
    return *(float*)&scratch[12] + radius;
}

// FUNCTION: TRILOGY 0x00414619
// Checks for a tagged world environment near the player's position.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
int CDisplay::GetEnvironmentForPlayer_guess(void* player)
{
    CDisplay* display = this;
    void* rawPlayer = player;
    float* position = &display->cameraLocation_guess[0];
    t3dFloatFastMath_guess_t fastFunction;
    float cosineProduct;
    float sineHeading;
    int regionResult;

    fastFunction = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E524C);
    __asm
    {
        mov edi, rawPlayer
        mov eax, fastFunction
        fld dword ptr [edi + 0x40]
        push ecx
        fstp dword ptr [esp]
        call eax
        fmul dword ptr [edi + 0x7C]
        fstp dword ptr [cosineProduct]
        add esp, 4
    }

    sineHeading = *(float*)((char*)rawPlayer + 0x40);
    fastFunction = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E5250);
    __asm
    {
        mov edi, rawPlayer
        mov esi, position
        mov eax, fastFunction
        fld dword ptr [sineHeading]
        push ecx
        fstp dword ptr [esp]
        call eax
        fmul dword ptr [edi + 0x7C]
        fld dword ptr [cosineProduct]
        fadd dword ptr [edi + 0x34]
        fstp dword ptr [esi]
        fadd dword ptr [edi + 0x38]
        fstp dword ptr [esi + 4]
        add esp, 4
    }

    position[2] = *(float*)((char*)rawPlayer + 0x3c) - EQ_GLOBAL(float, 0x005344A4);
    regionResult = EQ_GLOBAL(t3dGetRegionFromWorldAndXYZWithSine_guess_t, 0x006E52E8)(
        display->world_guess, position, sineHeading);
    if (regionResult != 0 &&
        CDisplay_GetEnvironment_guess(display, position[0], position[1], position[2], 0) != 0)
        return 0;
    return 1;
}

// FUNCTION: TRILOGY 0x004146a3
// Applies a jump impulse that moves the player upward and forward after its checks.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::DoSwimJump(void* player, char doSwimJump)
{
    void* actor;
    u32 nextJumpTime;
    float* heightMultiplier;

    if (player == 0)
        return;

    actor = *(void**)((char*)player + 0x70);
    if (actor == 0)
        return;

    nextJumpTime = this->time_ms_guess;
    if (nextJumpTime <= *(u32*)((char*)actor + 0x6c))
        return;

    nextJumpTime += 0x5dc;
    *(u32*)((char*)actor + 0x6c) = nextJumpTime;

    // The original reloads the actor pointer after storing the cooldown and
    // keeps that pointer across GetEnvironmentForPlayer_guess.
    actor = *(void**)((char*)player + 0x70);
    if (doSwimJump == 0)
        return;
    if (GetEnvironmentForPlayer_guess(player) == 0)
        return;

    heightMultiplier = &EQ_GLOBAL(float, 0x005351D0);
    __asm
    {
        mov edi, actor
        mov esi, player
        mov ebx, heightMultiplier
        fld dword ptr [edi + 0xb0]
        fmul dword ptr [ebx]
        fadd dword ptr [esi + 0x54]
        fstp dword ptr [esi + 0x54]
    }
    EQ_CDECL(void, 0x004E1C10, (void*, float))(
        player, EQ_GLOBAL(float, 0x005344A8));
}

// FUNCTION: TRILOGY 0x00414705
// Finds the upper boundary of the current world region above a point.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: two `xor edx, edx` dummies from EQ_THISCALL, z store lands after the second push, fadd operands swapped (EQ_GLOBAL constant)
/* fake match: probe keeps the sentinel and the running Z in stack slots (probe.point is passed to the callback, so VC6 cannot fold or copy-propagate them) */
float CDisplay::FindZoneTopZ(u32 y, u32 x, float z)
{
    struct
    {
        float point[3];
        float sentinel;
        float runningZ;
    } probe;
    /* read through a pointer: the direct absolute compare gives the wrong answer for NaN/inf under Unicorn */
    float* heightLimit = &EQ_GLOBAL(float, 0x00535244);
    float result;
    u8 initialEnvironment;
    u8 environment;

    probe.sentinel = -1e27;
    probe.runningZ = z;
    initialEnvironment = CDisplay_GetEnvironment_guess(this, *(float*)&y, *(float*)&x, z, 0);
    if (initialEnvironment != 0)
    {
        do
        {
            probe.point[0] = *(float*)&y;
            probe.point[1] = *(float*)&x;
            probe.point[2] = probe.runningZ = probe.runningZ + EQ_GLOBAL(float, 0x005351C4);
            if (EQ_GLOBAL(t3dGetRegionFromWorldAndXYZ_guess_t, 0x006E52E8)(world_guess, probe.point) == 0 ||
                probe.runningZ - z > *heightLimit)
                return EQ_GLOBAL(float, 0x005351F4);
            environment = CDisplay_GetEnvironment_guess(
                this, *(float*)&y, *(float*)&x, probe.runningZ, 0);
        } while (environment == initialEnvironment);
        result = probe.runningZ - EQ_GLOBAL(float, 0x005351C4);
    }
    else
        result = probe.sentinel;
    return result;
}

// FUNCTION: TRILOGY 0x004147bb
// Maps a position's zone tags to an environment ID and reports TP-tag presence.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
u8 CDisplay::GetEnvironment_guess(u32 y, u32 x, u32 z, int* hasTPTag)
{
    u32 position[3];
    char tag[256];
    void* zones;
    void* zoneTags;
    void* userData;
    u32 tagLength;
    u32 i;

    position[0] = y;
    position[1] = x;
    position[2] = z;
    i = 0;
    if (hasTPTag != 0)
        *hasTPTag = 0;

    EQ_GLOBAL(t3dGetZonesContainingWorldXYZPosition_guess_t, 0x006E5310)(
        this->world_guess, position, this->zoneRefList_guess);

    zones = this->zoneRefList_guess;
    for (; i < *(u32*)((char*)zones + 4); ++i)
    {
        zoneTags = *(void**)((char*)zones + 0xc);
        if (zoneTags != 0)
        {
            userData = EQ_GLOBAL(t3dGetZoneUserData_guess_t, 0x006E5314)(
                *(void**)((char*)zoneTags + i * 4));
            zones = this->zoneRefList_guess;
            zoneTags = *(void**)((char*)zones + 0xc);
            EQ_GLOBAL(t3dGetObjectTag_guess_t, 0x006E52DC)(
                *(void**)((char*)zoneTags + i * 4), tag);
            tagLength = strlen(tag);
            if (tagLength < 0x14 && userData != 0)
                SOC_CPY(tag, (const char*)userData);
            if (hasTPTag != 0 && tag[3] == 'T' && tag[4] == 'P')
                *hasTPTag = 1;

            if (strncmp(tag, (const char*)0x0053E750, 2) == 0)
                return 0;
            if (strncmp(tag, (const char*)0x0053E74C, 2) == 0)
                return 5;
            if (strncmp(tag, (const char*)0x0053E748, 2) == 0)
                return 7;
            if (strncmp(tag, (const char*)0x0053E744, 2) == 0)
                return 6;
            if (strncmp(tag, (const char*)0x0053E740, 2) == 0)
                return 8;
            if (strncmp(tag, (const char*)0x0053E73C, 2) == 0)
                return 9;
            if (strncmp(tag, (const char*)0x0053E738, 2) == 0)
                return 10;
        }
        zones = this->zoneRefList_guess;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x0041496f
// Teleports the local player by a teleport code (zone, y, x, z, heading digits) or teleporter-table index.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
bool __fastcall CDisplay_DoTeleport_guess(CDisplay* display, int unusedEdx, EQPlayer* targetPlayer,
                                         char* teleportCode, int teleportIndex)
{
    char* curZoneName;
    char result;
    size_t codeLen;
    int rc;
    char message[1024];
    char foundMsg[256];
    char zoneName[68];
    char headingDigits[4];
    char zDigits[7];
    char xDigits[7];
    char zoneDigits[6];
    char yDigits[7];
    float destHeading;
    float tmp1;
    float tmp2;
    int indexMode;
    float temp;

    temp = 0.0f;
    tmp2 = 0.0f;
    tmp1 = 0.0f;
    indexMode = 0;
    destHeading = 0.0f;
    if (targetPlayer == 0)
        return false;

    codeLen = strlen(teleportCode);
    message[0] = 0;
    foundMsg[0] = 0;
    zoneName[0] = 0;
    if (targetPlayer != EQ_GLOBAL(EQPlayer*, 0x006E4D8C))
    {
        if (targetPlayer != EQ_GLOBAL(EQPlayer*, 0x006E4DA0))
            return false;
        if (targetPlayer->race != 0x6c)
            return false;
        curZoneName = (char*)EQ_METHOD(2, void*, 0x0042381B, (void*, int, short*), EQ_GLOBAL(void*, 0x006E4DA8))(0x43, (short*)((char*)&teleportIndex + 2));
        EQ_METHOD(1, void, 0x00424E22, (void*, void*), EQ_GLOBAL(void*, 0x006E4DA8))(curZoneName);
        return false;
    }

    if (teleportIndex < 0)
    {
        if ((int)codeLen > 9)
        {
            zoneDigits[0] = teleportCode[5];
            zoneDigits[5] = 0;
            zoneDigits[1] = teleportCode[6];
            zoneDigits[2] = teleportCode[7];
            zoneDigits[3] = teleportCode[8];
            zoneDigits[4] = teleportCode[9];
            indexMode = EQ_CDECL(int, 0x005226F6, (const char*))(zoneDigits);
            if ((int)codeLen > 15)
            {
                yDigits[0] = teleportCode[10];
                yDigits[6] = 0;
                yDigits[1] = teleportCode[11];
                yDigits[2] = teleportCode[12];
                yDigits[3] = teleportCode[13];
                yDigits[4] = teleportCode[14];
                yDigits[5] = teleportCode[15];
                temp = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(yDigits);
                if ((int)codeLen > 0x15)
                {
                    xDigits[0] = teleportCode[16];
                    xDigits[6] = 0;
                    xDigits[1] = teleportCode[17];
                    xDigits[2] = teleportCode[18];
                    xDigits[3] = teleportCode[19];
                    xDigits[4] = teleportCode[20];
                    xDigits[5] = teleportCode[21];
                    tmp2 = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(xDigits);
                    if ((int)codeLen > 0x1b)
                    {
                        zDigits[0] = teleportCode[22];
                        zDigits[6] = 0;
                        zDigits[1] = teleportCode[23];
                        zDigits[2] = teleportCode[24];
                        zDigits[3] = teleportCode[25];
                        zDigits[4] = teleportCode[26];
                        zDigits[5] = teleportCode[27];
                        tmp1 = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(zDigits);
                        if ((int)codeLen > 0x1e)
                        {
                            headingDigits[0] = teleportCode[28];
                            headingDigits[3] = 0;
                            headingDigits[1] = teleportCode[29];
                            headingDigits[2] = teleportCode[30];
                            destHeading = (float)EQ_CDECL(double, 0x00521AB0, (const char*))(headingDigits);
                        }
                    }
                }
            }
        }
    }
    else
    {
        temp = (float)teleportIndex;
        indexMode = 0xff;
        SOC_CPY(yDigits, (const char*)0x0053E81C);
    }

    if (indexMode == 0xff)
    {
        SOC_FMT(
            message, (const char*)0x0053E7FC, (int)temp, yDigits);
        rc = EQ_CDECL(int, 0x004C1862, (int, float*, float*, float*, float*, int*))(
            (int)temp, &temp, &tmp2, &tmp1, &destHeading, &indexMode);
        if (rc == -1)
        {
            SWD_STRCAT(message, (const char*)0x0053E7D0);
            return false;
        }
    }
    else
    {
        SOC_FMT(
            message, (const char*)0x0053E7B4, indexMode, zoneDigits);
    }

    SOC_FMT(
        foundMsg, (const char*)0x0053E770, indexMode,
        (double)temp, (double)tmp2, (double)tmp1, (double)destHeading);
    SWD_STRCAT(message, foundMsg);
    EQ_METHOD(2, void, 0x004BFC65, (void*, int, char*), EQ_GLOBAL(void*, 0x006E4D54))(indexMode, zoneName);

    curZoneName = targetPlayer->zone;
    rc = SWD_STRCMPI
        (curZoneName, (const char*)0x0053DF4C);
    if (rc == 0 &&
        SWD_STRCMPI(zoneName, (const char*)0x0053E764) == 0)
    {
        tmp2 = targetPlayer->x - 1044.0f;
    }
    else
    {
        rc = SWD_STRCMPI(curZoneName, (const char*)0x0053E764);
        if (rc != 0 ||
            (rc = SWD_STRCMPI(zoneName, (const char*)0x0053DF4C),
             rc != 0))
        {
            rc = SWD_STRCMPI(curZoneName, (const char*)0x0053E760);
            if (rc == 0 &&
                SWD_STRCMPI(zoneName, (const char*)0x0053E754) == 0)
            {
                tmp1 = targetPlayer->z + 2.0f;
                targetPlayer->z = tmp1;
            }
            goto doTeleport;
        }
        tmp2 = targetPlayer->x + 1044.0f;
    }
    targetPlayer->x = tmp2;

doTeleport:
    result = EQ_METHOD(7, char, 0x00414CED, (CDisplay*, EQPlayer*, char*, float, float, float, float, int), display)(targetPlayer, zoneName, temp, tmp2, tmp1, destHeading, (int)message);
    return result != '\0';
}

// FUNCTION: TRILOGY 0x00414ced
// Moves an eligible player to requested coordinates and starts a zone change when needed.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// __thiscall in the binary (ECX = the CDisplay, RET 0x1C), a __fastcall free function with the EDX dummy here
// because display.h has no declaration. Moves the player to y/x/z/heading (values >= 999999 / 513 leave the
// field alone); with an empty name or the current zone it stays and re-seats the actor, else calls ZonePC.
// Returns 0 when the player is missing, standing 'f' or link-dead, else 1.
char __fastcall CDisplay_DoTeleportB_guess(CDisplay* display, int unusedEdx, EQPlayer* player, char* zoneName,
                                           float y, float x, float z, float heading, int param7)
{
    float floorZ;
    int race;

    if (player != 0 && player->stand_state != 'f' && player->linkdead == 0)
    {
        if (y < 999999.0f)
            player->y = y;
        if (x < 999999.0f)
            player->x = x;
        if (z < 999999.0f)
            player->z = z;
        if (heading < 513.0f)
            player->heading = heading;

        if (strlen(zoneName) > 0 &&
            SWD_STRCMPI(zoneName, player->zone) != 0)
        {
            race = player->race;
            if (race == 8 || (race > 10 && race <= 12))
                player->z = player->z + 1.0f;
            g_pc_guess->y = g_local_player_guess->y;
            g_pc_guess->x = g_local_player_guess->x;
            g_pc_guess->z = g_local_player_guess->z;
            g_pc_guess->heading = g_local_player_guess->heading;
            EQ_METHOD(3, void, 0x004DC1B5, (void*, char*, int, int), EQ_GLOBAL(void*, 0x006EFE2C))(zoneName, param7, 0);
        }
        else
        {
            EQ_METHOD(1, void, 0x00410ABF, (CDisplay*, EQPlayer*), display)(player);
            *(&player->actor_info->unk_0x185) = 0;
            player->actor_info->ride_target_guess = 0;
            *(&player->actor_info->unk_0x1B8) = 0;
            floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), display)(player, player->y, player->x, player->z + 1.0f, 1);
            if (floorZ != -1.0e27f)
                player->actor_info->floor_z = floorZ;
        }
        return 1;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x00414e7a
// Raises a player's feet to floor height when they are below the floor.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::PutPlayerOnFloor(EQPlayer* player)
{

    struct EQActorInfo_guess *temp = player->actor_info;
    if (!(player->z - player->height_offset < temp->floor_z))
        {
    }
    else
        player->z = player->actor_info->floor_z + player->height_offset;
}

// FUNCTION: TRILOGY 0x00414e9b
// Moves the local player and ridden controlled spawn to configured coordinates, clearing their movement.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void __fastcall CDisplay_MoveLocalPlayerToSafeCoords_guess(CDisplay* display, int unusedEdx)
{
    float floorZ;

    g_local_player_guess->y = EQ_GLOBAL(float, 0x006B6734);
    g_local_player_guess->x = EQ_GLOBAL(float, 0x006B6738);
    g_local_player_guess->z = EQ_GLOBAL(float, 0x006B673C);
    g_local_player_guess->dy = 0.0f;
    g_local_player_guess->dx = 0.0f;
    g_local_player_guess->dz = 0.0f;
    g_local_player_guess->delta_heading = 0.0f;
    g_local_player_guess->unk_0x5C = 0.0f;
    g_local_player_guess->speed = 0.0f;
    *(&g_local_player_guess->actor_info->unk_0x185) = 0;
    g_local_player_guess->actor_info->ride_target_guess = 0;
    *(&g_local_player_guess->actor_info->unk_0x1B8) = 0;
    if (g_pc_guess != 0)
    {
        g_pc_guess->boat[0] = '\0';
        g_pc_guess->y = EQ_GLOBAL(float, 0x006B6734);
        g_pc_guess->x = EQ_GLOBAL(float, 0x006B6738);
        g_pc_guess->z = EQ_GLOBAL(float, 0x006B673C);
    }
    floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), g_cdisplay_guess)(g_local_player_guess,
                 EQ_GLOBAL(float, 0x006B6734), EQ_GLOBAL(float, 0x006B6738),
                 EQ_GLOBAL(float, 0x006B673C) + 1.0f, 1);
    if (floorZ != -1.0e27f)
        g_local_player_guess->actor_info->floor_z = floorZ;
    EQ_METHOD(1, void, 0x00410ABF, (CDisplay*, EQPlayer*), g_cdisplay_guess)(g_local_player_guess);
    if (g_control_player_guess != 0 &&
        g_control_player_guess != g_local_player_guess &&
        g_control_player_guess->actor_info->ride_target_guess != 0)
    {
        g_control_player_guess->y = EQ_GLOBAL(float, 0x006B6734);
        g_control_player_guess->x = EQ_GLOBAL(float, 0x006B6738);
        g_control_player_guess->z = EQ_GLOBAL(float, 0x006B673C);
        floorZ = EQ_METHOD(5, float, 0x0041436D, (CDisplay*, EQPlayer*, float, float, float, int), g_cdisplay_guess)(g_control_player_guess,
                     EQ_GLOBAL(float, 0x006B6734), EQ_GLOBAL(float, 0x006B6738),
                     EQ_GLOBAL(float, 0x006B673C) + 1.0f, 1);
        if (floorZ != -1.0e27f)
            g_control_player_guess->actor_info->floor_z = floorZ;
        EQ_METHOD(1, void, 0x00410ABF, (CDisplay*, EQPlayer*), g_cdisplay_guess)(g_control_player_guess);
    }
    return;
}

#define UNDER_FLOOR_FALLING_LIMIT EQ_GLOBAL(float, 0x00535254)

struct UnderFloorMessage_guess
{
    u16 spawn_id;
    u16 unknown0x02;
    f32 y;
    f32 x;
    f32 z;
};

// FUNCTION: TRILOGY 0x00415083
// Corrects below-floor positions and reports local underworld falls before recovery.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void __fastcall CDisplay_CheckForUnderFloor_guess(CDisplay* display, int unusedEdx, EQPlayer* player)
{
    char diagnostic[512];
    UnderFloorMessage_guess message;
    float playerZ;
    float floorZ;
    float currentHeight;

    floorZ = player->actor_info->floor_z;
    playerZ = player->z;
    currentHeight = playerZ - player->height_offset;
    if (currentHeight < floorZ)
    {
        if (player->actor_info->unknown0x099 == 0 &&
            player->dz + player->dz <= UNDER_FLOOR_FALLING_LIMIT)
        {
            EQ_METHOD(4, void, 0x00410D13,
                (CDisplay*, float, float, float, int), display)
                (player->y, player->x, playerZ, 0x72);
        }
        if (player->dz <= UNDER_FLOOR_FALLING_LIMIT)
        {
            float fallDistance = player->dz - UNDER_FLOOR_FALLING_LIMIT;
            if (fallDistance < EQ_GLOBAL(float, 0x0053510C))
                fallDistance = -fallDistance;
            if (player->type == 0)
                EQ_METHOD(1, void, 0x004B41CE, (EQPlayer*, float), player)(fallDistance);
        }
        player->dz = 0.0f;
        player->z = floorZ + player->height_offset;
        *(&player->actor_info->savedZ_bits_guess) = 0xEC4ECB8F;
    }
    else
    {
        if (currentHeight > floorZ &&
            player->dz < EQ_GLOBAL(float, 0x0053510C) &&
            *(&player->actor_info->savedZ_guess) == EQ_GLOBAL(float, 0x005351F4))
        {
            *(&player->actor_info->savedZ_guess) = player->z;
        }
    }

    currentHeight = player->z - player->height_offset;
    if (currentHeight < EQ_GLOBAL(MsgNewZone, 0x006B65DC).underworld &&
        player == g_local_player_guess &&
        (EQ_GLOBAL(int, 0x006E4D4C) == 0 ||
         EQ_METHOD(3, char, 0x0041496F, (CDisplay*, EQPlayer*, char*, int), display)
             (player, (char*)0x0053E89C, 0xB) == 0))
    {
        message.spawn_id = (u16)player->spawn_id;
        message.y = player->y;
        message.x = player->x;
        message.z = player->z;
        send_message_guess
            (g_world_connection_guess, 0x2024, &message, 0x10, 1);
        SOC_FMT
            (diagnostic, (const char*)0x0053E854, player->zone,
                (double)player->y, (double)player->x, (double)player->z);
        EQ_METHOD(1, void, 0x004D05B7, (CEverQuest*, const char*), pinstCEverQuest_guess)
            (diagnostic);
        LogMessage_guess(diagnostic);
        LogMessage_guess((const char*)0x0053E850);
        DbgLog_guess((const char*)0x0053E824, (double)player->heading,
                (double)(*(&player->pitchState_guess)), (double)currentHeight,
                (double)player->height_offset, player);
        EQ_METHOD(0, void, 0x00414E9B, (CDisplay*), display)();
    }
    return;
}

// FUNCTION: TRILOGY 0x0041527c
// Initializes each zone spawn's floor height and actor update timestamp.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void CDisplay::SetupEQPlayers(void)
{
    EQPlayer* player;
    EQActorInfo_guess* info;
    float floorZ;
    u32 time;

    player = g_player_top_guess;
    while (player != 0)
    {
        info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);
        if (info != 0)
        {
            floorZ = g_cdisplay_guess->GetFloorHeight(player->y, player->x, player->z + 1.0f);
            info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);
            info->floor_z = floorZ;
            info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);
            floorZ = info->floor_z;
            if (floorZ == -1.0e27f || floorZ != floorZ)
                info->floor_z = 1.0f;
            info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);
            info->lastPhysicsTime_guess = 0;
            time = EqGetTime();
            info = *(EQActorInfo_guess* volatile*)((char*)player + 0x70);
            info->lastActiveTime_guess = time;
        }
        player = player->next;
    }
}

struct CDisplay_MoveLight_guess : CDisplay
{
    void MoveLight(void* light, float* position);
};

// FUNCTION: TRILOGY 0x004152eb
// Moves a spawn's or missile's light to just ahead of it along its heading, only near the local player.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// @early-stop: the light argument push lands before the lightLocation[3] store (original: after); EQ_THISCALL's EDX dummies remain
void CDisplay_MoveLight_guess::MoveLight(void* light, float* position)
{
    EQPlayer* localPlayer;
    float lightLocation[6];
    float tmp;
    float cosTerm;
    float sinTerm;

    localPlayer = g_local_player_guess;
    if (localPlayer != 0)
    {
        tmp = EQ_GLOBAL(float, 0x006B66E0) - 100.0f;
        if (tmp < 50.0f)
            tmp = 50.0f;

        if (EQ_METHOD(7, float, 0x00412984, (CDisplay*, float, float, float, float, float, float, float), g_cdisplay_guess)(
             position[0], position[1], position[2],
             localPlayer->y, localPlayer->x, localPlayer->z, 1.0f) <= tmp)
        {
            cosTerm = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E524C)(position[3]);
            cosTerm = cosTerm + cosTerm;
            sinTerm = EQ_GLOBAL(t3dFloatFastMath_guess_t, 0x006E5250)(position[3]);
            sinTerm = sinTerm + sinTerm;
            lightLocation[3] = cosTerm + position[0];
            lightLocation[4] = sinTerm + position[1];
            lightLocation[5] = position[2] + 4.0f;
            ((void (__cdecl *)(void*, float*))EQ_GLOBAL(void*, 0x006E52E4))(light, lightLocation);
            if (0.25f < EQ_METHOD(7, float, 0x00412984, (CDisplay*, float, float, float, float, float, float, float), g_cdisplay_guess)(
                 lightLocation[0], lightLocation[1], lightLocation[2],
                 lightLocation[3], lightLocation[4], lightLocation[5], 1.0f))
            {
                ((void (__cdecl *)(void*, void*, float*))EQ_GLOBAL(void*, 0x006E5430))
                    (world_guess, light, &lightLocation[3]);
            }
        }
    }
    return;
}

// FUNCTION: TRILOGY 0x0042a109
// Cycles a character creation appearance choice, applying a variant or attachment tint.
// __thiscall (ECX = CDisplay, callee pops 8), not declared in display.h, so a __fastcall free function stands in
// (EDX is the unused slot). The kind byte at +0x19a2 (1..4) picks the slot to walk and its range [lo, hi]: kind 1
// +0x1bc0 [0, 7]; kind 3 +0x1bd8 [1, 0x14], copied to +0x1bd7 at the end; kinds 2 and 4 +0x1bd9 / +0x1bda [0, 0x1d]
// when use_tint is 0, else +0x1bdb / +0x1bdc [0, 9]. Any other kind returns at once.
// step_up == 0 walks the slot down from its value (wrapping to hi, stopping after lo), otherwise up (wrapping to
// lo, stopping after hi). Each value goes to the local player through GetBaseFaceNbr_guess (use_tint == 0) or
// SetPlayerAttachmentTint (use_tint != 0); a nonzero result stops the walk. Kinds 2 and 4 then get 0xff in the
// slot unless the walk failed. Both calls take `this` from the global at 0x006E4DE0, as the original does.
// @early-stop: down walk rotated (lo test hoisted onto AL), clamp store and not mov, per-arm mov ecx + dummy xor edx
void __fastcall CDisplay_CycleLocalPlayerAppearance_guess(CDisplay* display, int edx_unused, int step_up, int use_tint)
{
    u8 failed;
    u8 lo;
    u8 hi;
    u8* slot;
    u8 value;
    u8 slot_value;
    u8 kind;
    int result;

    failed = 0;
    lo = 0;
    hi = 0x1d;
    switch (*((u8*)display + 0x19a2))
    {
    case 1:
        lo = 0;
        slot = (u8*)display + 0x1bc0;
        hi = 7;
        break;
    case 2:
        if (use_tint != 0)
        {
            hi = 9;
            slot = (u8*)display + 0x1bdb;
        }
        else
            slot = (u8*)display + 0x1bd9;
        break;
    case 3:
        slot = (u8*)display + 0x1bd8;
        lo = 1;
        hi = 0x14;
        break;
    case 4:
        if (use_tint != 0)
        {
            hi = 9;
            slot = (u8*)display + 0x1bdc;
        }
        else
            slot = (u8*)display + 0x1bda;
        break;
    default:
        return;
    }

    if (step_up == 0)
    {
        if (*slot == lo)
            slot_value = hi;
        else
            slot_value = *slot - 1;
        *slot = slot_value;
        value = slot_value;
        while (value >= lo)
        {
            // the walk value is kept in the step argument's low byte (its stack slot), upper bytes unchanged
            *(u8*)&step_up = value;
            if (value > hi)
            {
                value = 0;
                *(u8*)&step_up = value;
            }
            if (use_tint != 0)
                result = EQ_METHOD(3, int, 0x00406802, (CDisplay*, EQPlayer*, int, char), g_cdisplay_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), step_up, *((u8*)display + 0x19a2));
            else
                result = EQ_METHOD(3, int, 0x004066E2, (CDisplay*, EQPlayer*, int, char), g_cdisplay_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), step_up, *((u8*)display + 0x19a2));
            if (result != 0)
            {
                failed = 1;
                break;
            }
            if (value == lo)
                break;
            value--;
        }
    }
    else
    {
        (*slot)++;
        if (*slot > hi)
            *slot = lo;
        value = *slot;
        *(u8*)&step_up = value;
        while (value <= hi)
        {
            if (use_tint != 0)
                result = EQ_METHOD(3, int, 0x00406802, (CDisplay*, EQPlayer*, int, char), g_cdisplay_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), step_up, *((u8*)display + 0x19a2));
            else
                result = EQ_METHOD(3, int, 0x004066E2, (CDisplay*, EQPlayer*, int, char), g_cdisplay_guess)(EQ_GLOBAL(EQPlayer*, 0x006E4D8C), step_up, *((u8*)display + 0x19a2));
            if (result != 0)
            {
                failed = 1;
                break;
            }
            value++;
            *(u8*)&step_up = value;
        }
    }
    *slot = value;
    kind = *((u8*)display + 0x19a2);
    if (kind == 3)
        *((u8*)display + 0x1bd7) = *((u8*)display + 0x1bd8);
    else if (failed == 0 && (kind == 2 || kind == 4))
        *slot = 0xff;
    return;
}

struct ActorInfoCreation_guess
{
    u8 storage[0x364];

    // EXTERNAL: TRILOGY 0x004BCDE8 ActorInfoCreation_guess::ActorInfoCreation_guess
    ActorInfoCreation_guess(char* actorTag);
};

// FUNCTION: TRILOGY 0x00430381
// Char create/select: rebuilds the player model for the chosen gender, race, class, face; aims the camera.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void __fastcall CDisplay_SetCCreateCamera_guess(CDisplay* display, int edx_unused, char gender, int raceIndex,
                                                 char face, char classId)
{
    char actorTag[0x20];
    EQActorInfo_guess* oldActorInfo;
    EQActorInfo_guess* actorInfo;
    CDisplay* characterDisplay;
    float* cameraSettings;
    int cameraIndex;
    float startZ;

    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender = gender;
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race = (short)(raceIndex + 1);
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->class_ = classId;
    EQ_METHOD(1, void, 0x004B156C, (EQPlayer*, char*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))(actorTag);
    SWD_STRCAT(actorTag, (const char*)0x0053E4B0);
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face = face;
    oldActorInfo = EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info;
    if (oldActorInfo != 0)
    {
        EQ_METHOD(0, void, 0x004BD18D, (EQActorInfo_guess*), oldActorInfo)();
        EQ_CDECL(void, 0x005218BB, (void*))(oldActorInfo);
    }
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info = 0;
    actorInfo = (EQActorInfo_guess*)new ActorInfoCreation_guess(actorTag);
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info = actorInfo;
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->size = EQ_GLOBAL(float, 0x005345BC);
    EQ_METHOD(0, void, 0x004BC9BA, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
    EQ_METHOD(0, void, 0x004B4873, (EQPlayer*), EQ_GLOBAL(EQPlayer*, 0x006E4D8C))();
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->floor_z =
        EQ_METHOD(3, float, 0x00414245, (CDisplay*, float, float, float), g_cdisplay_guess)(
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y,
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x,
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z);
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->floor_z == EQ_GLOBAL(float, 0x005351F4))
    {
        EQTRACE_guess((const char*)0x005454C8, EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->name);
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->floor_z = 1.0f;
    }
    *(u32*)((u8*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x68) = 0;
    EQ_METHOD(1, void, 0x0040F731, (CDisplay*, EQPlayer*), g_cdisplay_guess)(
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
    EQ_METHOD(1, void, 0x00410ABF, (CDisplay*, EQPlayer*), g_cdisplay_guess)(
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C));
    EQ_METHOD(1, void, 0x0040E422, (CDisplay*, void*), g_cdisplay_guess)(
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->actor);
    *(u32*)((u8*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info + 0x68) = 0;
    EQ_METHOD(3, void, 0x00406EDC, (CDisplay*, EQPlayer*, int, int), g_cdisplay_guess)(
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C), EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->face, 0);

    startZ = EQ_GLOBAL(float, 0x005353B8);
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 9 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 6)
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y = 0.0f;
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x = EQ_GLOBAL(float, 0x005353C0);
        startZ = EQ_GLOBAL(float, 0x005353BC);
    }
    else
    {
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->y = 0.0f;
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->x = 0.0f;
    }
    EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->z = startZ;

    characterDisplay = EQ_GLOBAL(CDisplay*, 0x006E4E70);
    if (characterDisplay == 0 || *((u8*)characterDisplay + 0x19A2) == 0)
    {
        cameraIndex = g_activeView_guess;
        cameraSettings = &EQ_GLOBAL(float, 0x006B6F18) + cameraIndex * 7;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 ||
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 9 ||
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0x80)
            cameraSettings[0] = 7.0f;
        else
            cameraSettings[0] = 6.0f;
        cameraSettings[2] = -(EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->scaledHeight_guess *
                             EQ_GLOBAL(float, 0x0053516C));
        return;
    }

    if (*((int*)((u8*)g_cdisplay_guess + 0x94)) != 0 &&
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->anim_set_guess != 0 &&
        *((u8*)EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->actor_info->anim_set_guess + 0x39C) == 1)
    {
        EQ_METHOD(6, void, 0x0048ED3B, (CDisplay*, EQPlayer*, int, int, int, float, int),
                  g_cdisplay_guess)(
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 0x47, 0, 0, 1.0f, 1);
        cameraIndex = g_activeView_guess;
        cameraSettings = &EQ_GLOBAL(float, 0x006B6F18) + cameraIndex * 7;
        cameraSettings[0] = 1.8f;
        cameraSettings[2] = 0.0f;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0x82)
            cameraSettings[0] = 2.4f;
        return;
    }

    EQ_METHOD(6, void, 0x0048ED3B, (CDisplay*, EQPlayer*, int, int, int, float, int),
              g_cdisplay_guess)(
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C), 0x20, 1, 0, 1.0f, 1);
    cameraIndex = g_activeView_guess;
    cameraSettings = &EQ_GLOBAL(float, 0x006B6F18) + cameraIndex * 7;
    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender == 0)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 ||
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 9)
            cameraSettings[2] = -1.0f;
        else if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 8 ||
                 EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0xB ||
                 EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0xC)
            cameraSettings[2] = -0.5f;
        else
            cameraSettings[2] = 0.0f;
    }
    else if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 ||
             EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 9)
        cameraSettings[2] = -1.0f;
    else
        cameraSettings[2] = -0.5f;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender == 0 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 3 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 5)
        cameraSettings[0] = EQ_GLOBAL(float, 0x00535198);
    else
        cameraSettings[0] = 1.3f;

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 8 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0xC ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0xB)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender == 0)
            cameraSettings[0] = 2.5f;
        else
            cameraSettings[0] = EQ_GLOBAL(float, 0x00535198);
    }

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 ||
        EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 9)
    {
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender == 0)
            cameraSettings[0] = EQ_GLOBAL(float, 0x005351D4);
        else
            cameraSettings[0] = 3.0f;
        if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 10 &&
            EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->gender != 0)
            cameraSettings[0] = EQ_GLOBAL(float, 0x005351D4);
    }
    else if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0x80)
    {
        cameraSettings[0] = 2.5f;
        cameraSettings[2] = 0.5f;
    }

    if (EQ_GLOBAL(EQPlayer*, 0x006E4D8C)->race == 0x82)
        cameraSettings[0] = 2.4f;
    return;
}

// The parameter of 0x0043AA7E is not a CDisplay (layouts/cdisplay.json): its own byte view.
struct DisplayModeState_guess { u8 unknown0x0000[0x10]; u8 unknown0x0010[8]; u8 unknown0x0018[0x6C]; u8 unknown0x0084[0x15]; u8 unknown0x0099[8]; };

// FUNCTION: TRILOGY 0x0043aa7e
// Applies an eligible change to the active display mode.
void __fastcall HandleDisplayModeState_guess(DisplayModeState_guess* display)
{
    *(short*)&display->unknown0x0084[0x12] = -1;
    *(u32*)&display->unknown0x0010[0] = 0;
    *(u32*)&display->unknown0x0084[0x0c] = 0;

    if (EXEC_SAVED_VIDEO_MODE == 2 && display->unknown0x0099[7] != 0)
    {
        if (EQ_GLOBAL(void*, 0x006E4DA8) != NULL &&
            EQ_METHOD(0, u32, 0x004197B2, (void*), EQ_GLOBAL(void*, 0x006E4DA8))() != 0)
        {
            void* client = EQ_GLOBAL(void*, 0x006E4E44);
            u32* clientFlags = (u32*)((u8*)client + 0x90224);
            clientFlags[5] = 0;
            clientFlags[4] = 1;
            return;
        }

        g_game_state_guess = EXEC_SAVED_VIDEO_MODE;
        if (EQ_GLOBAL(void*, 0x006E4E44) != NULL)
            pinstEQ_Main_guess->uiDirty_u32_guess = 1;
        EQ_METHOD(1, void, 0x0040E58B, (void*, int), g_display_guess)(0);
    }
    return;
}

// FUNCTION: TRILOGY 0x0049602c
// Applies an unidentified spawn update; the field's meaning and follow-up refresh are unclear.
// __cdecl message handler (called from CEverQuest::HandleWorldMessage). The message is two dwords: a spawn id
// and a value stored at +0x300 of that spawn's actor info (field not in EQActorInfo_guess yet); then the
// spawn's name sprite is rebuilt.
void CEverQuest_HandleWorldMessage_save_guilds2_guess(u32* messageWords)
{
    EQPlayer* player = EQPlayer_GetPlayerFromID_guess(g_player_top_guess, messageWords[0]);

    if (player != NULL && player->actor_info != NULL)
    {
        *(u32*)((char*)player->actor_info + 0x300) = messageWords[1];
        EQ_METHOD(1, void, 0x00411B7B, (void*, EQPlayer*), g_display_guess)(player);
    }
}

// FUNCTION: TRILOGY 0x004A00F5
// Registers a spawn's actor record and initializes its animation data.
// __thiscall (`this` in ECX, EDX unused), written as __fastcall so the stack arguments line up; callee pops 12
// bytes. Links the new node `self` in as the head of the list at 0x006E4D68 (+0 back link, +4 next), copies
// `name` into +8 (29 bytes), zeroes the 0x4A dwords from +0x30, clears the byte at +0x39C, then calls
// CDisplay::InitAnimationTracks_guess on the global CDisplay (0x006E4DE0). Returns `self`.
void* __fastcall initializeActorInfoNode_guess(void* self, int ignored_edx, const char* name, void* sprite,
                                               EQPlayer* player)
{
    void* head;
    u32* p;
    int count;

    head = EQ_GLOBAL(void*, 0x006E4D68);
    if (head == NULL)
    {
        EQ_GLOBAL(void*, 0x006E4D68) = self;
        *(void**)((char*)self + 4) = NULL;
        *(void**)self = NULL;
    }
    else
    {
        *(void**)head = self;
        head = *(void* volatile*)&EQ_GLOBAL(void*, 0x006E4D68);
        *(void**)self = NULL;
        *(void**)((char*)self + 4) = head;
        EQ_GLOBAL(void*, 0x006E4D68) = self;
    }

    EQ_CDECL(void, 0x004A0841, (char*, const char*, int))((char*)self + 8, name, 0x1d);
    p = (u32*)((char*)self + 0x30);
    for (count = 0x4a; count != 0; count = count - 1)
    {
        *p = 0;
        p = p + 1;
    }
    *(u8*)((char*)self + 0x39C) = 0;
    EQ_METHOD(3, void, 0x0048E6AC, (void*, int, void*, EQPlayer*), g_display_guess)((int)self, sprite, player);
    return self;
}

// FUNCTION: TRILOGY 0x004A01CC
// Finds the actor animation entry whose name exactly matches the requested name.
// Called with an unused ECX (callee pops its single argument). Walks the singly linked list whose head
// is at 0x006E4D68 (next at +4, name at +8) and returns the first node whose name equals `name`
// (case-sensitive), or 0.
int __stdcall findActorInfoByName_guess(char* name)
{
    int node;

    for (node = EQ_GLOBAL(int, 0x006E4D68); node != 0; node = *(int*)(node + 4))
    {
        if (strcmp((char*)(node + 8), name) == 0)
            return node;
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004A1E2A
// Applies an equipment appearance change to a spawn and broadcasts its wear update.
// @early-stop: p4 pointer staging, flag-byte updates, and call argument setup still differ after eight non-improving iterations.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// Material change for the spawn at characterProfile (its EQPlayer is at +0xA0). slot is the slot: 2 -> 0, 7 -> 2,
// 9 and 10 -> 3, 12 -> 4, 17 -> 1, 18 -> 5, 19 -> 6; other values return. newMaterialId (new) and oldMaterialId (old) are
// objects: byte +0xC5 is the material, dword +0xC8 the colour (0 when null). For slot 0 a name at +0x5F of more
// than 2 characters whose number (atoi at +0x61) is >= 0xF0 replaces the material. When the material or colour
// differs, the flag at +0xA47 must be -1, or is set to -1 here when the EQPlayer's byte +0x98 is 0 (which also
// sets its +0xD4 to 0xFF); then slot 0 goes to CDisplay_SwapHead_guess, the others to
// CDisplay_SwapMaterial_guess, and opcode 0x2092 is sent.
void __cdecl CDisplay_HandleMaterial_guess(int characterProfile, short slot, u32 newMaterialId, u32 oldMaterialId)
{
    char* flagPtr;
    size_t length;
    u32 parsedNumber;
    MsgWearChange msg;
    int colour;
    int oldColour;

    int sentinel = 0;
    if (slot < 0x16 && characterProfile != sentinel && *(int*)(characterProfile + 0xa0) != sentinel)
    {
            int originalSlot = (int)slot;
            if (originalSlot <= 0xc)
            {
                if (originalSlot != 0xc)
                {
                    if (originalSlot != 2)
                    {
                        if (originalSlot != 7)
                        {
                            if (originalSlot <= 8 || originalSlot > 10)
                                return;
                            slot = 3;
                        }
                        else
                            slot = 2;
                    }
                    else
                        slot = sentinel;
                }
                else
                    slot = 4;
            }
            else
            {
                switch (originalSlot)
                {
                case 0x11:
                    slot = 1;
                    break;
                case 0x12:
                    slot = 5;
                    break;
                case 0x13:
                    slot = 6;
                    break;
                default:
                    return;
                }
            }
            char* newItem = (char*)newMaterialId;
            if (newItem == (char*)sentinel)
            {
                newMaterialId = sentinel;
                colour = sentinel;
            }
            else
            {
                *(u16*)&newMaterialId = (u16)*(u8*)(newItem + 0xc5);
                colour = *(int*)(newItem + 0xc8);
            }
            char* oldItem = (char*)oldMaterialId;
            if (oldItem == (char*)sentinel)
            {
                oldMaterialId = sentinel;
                oldColour = sentinel;
            }
            else
            {
                *(u16*)&oldMaterialId = (u16)*(u8*)(oldItem + 0xc5);
                oldColour = *(int*)(oldItem + 0xc8);
            }
            if (*(u16*)&slot == (u16)sentinel)
            {
                sentinel = 0xf0;
                if (newItem != 0 && newItem + 0x5f != (char*)0)
                {
                    length = strlen(newItem + 0x5f);
                    if (length > 2)
                    {
                        parsedNumber = atoi(newItem + 0x61);
                        if ((int)parsedNumber >= sentinel)
                            newMaterialId = parsedNumber;
                    }
                }
                if (oldItem != 0 && oldItem + 0x5f != (char*)0)
                {
                    length = strlen(oldItem + 0x5f);
                    if (length > 2)
                    {
                        parsedNumber = atoi(oldItem + 0x61);
                        if ((int)parsedNumber >= sentinel)
                            oldMaterialId = parsedNumber;
                    }
                }
            }
            u16 newMaterial = *(u16*)&newMaterialId;
            if (*(u16*)&oldMaterialId == newMaterial && oldColour == colour)
                return;
                flagPtr = (char*)(characterProfile + 0xa47);
                sentinel = -1;
                if (*flagPtr != sentinel)
                {
                    int player = *(int*)(characterProfile + 0xa0);
                    if (*(char*)(player + 0x98) == 0)
                    {
                        *flagPtr = (char)sentinel;
                        *(u8*)(player + 0xd4) = (u8)sentinel;
                    }
                    if (*flagPtr != sentinel)
                        return;
                }
                if (slot == 0)
                {
                    EQ_METHOD(6, int, 0x00407334, (void*, int, int, int, int, int, int), g_display_guess)(*(int*)(characterProfile + 0xa0), (int)*(u16*)&newMaterialId,
                     (int)*(u16*)&oldMaterialId, 0, colour, 0);
                }
                else
                {
                    EQ_METHOD(5, int, 0x0040798E, (void*, int, int, int, int, int), g_display_guess)(*(int*)(characterProfile + 0xa0), slot,
                         (int)*(u16*)&newMaterialId, *(u8*)(slot + 0xa4 + *(int*)(characterProfile + 0xa0)), colour);
                }
                msg.spawn_id = *(u32*)(*(int*)(characterProfile + 0xa0) + 0x80);
                msg.slot = (u8)slot;
                msg.material = (u8)newMaterialId;
                msg.color = colour;
                msg.unknown0x0C = *(u8*)(*(int*)(characterProfile + 0xa0) + 0x12c);
                send_message_guess(g_world_connection_guess, 0x2092, &msg, 0x10, 1);
    }
    return;
}

// FUNCTION: TRILOGY 0x004a31a5
// Reports whether an unidentified feature passes its override and expansion checks.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
// The argument is the player profile (0x004A58F5 passes g_pc_guess): 1 when either flag is set or the
// profile's expansion flags are >= 2 (signed compare).
int CDisplay::SetPCloudDensity_guess(CharacterProfile* profile)
{
    if (g_pcloudFlagA_guess == 0 && g_pcloudFlagB_guess == 0
        && (profile == 0 || profile->expansions_guess < 2))
        return 0;
    return 1;
}

// FUNCTION: TRILOGY 0x004BFC65
// Zone id to short zone name (qeynos, highpass, northkarana...) for the teleport and zone-file checks
int __fastcall GetZoneShortName_guess(int this_unused, int edx_unused,
                                  int zoneNumber, char* shortName)
{
    switch (zoneNumber)
    {
        case 0x1: strcpy(shortName, (char*)0x0055c3e8); break;
        case 0x2: strcpy(shortName, (char*)0x0055c358); break;
        case 0x3: strcpy(shortName, (char*)0x0053e760); break;
        case 0x4: strcpy(shortName, (char*)0x0053e754); break;
        case 0x5: strcpy(shortName, (char*)0x00561e00); break;
        case 0x6: strcpy(shortName, (char*)0x00561df4); break;
        case 0x7: strcpy(shortName, (char*)0x00561de8); break;
        case 0x8: strcpy(shortName, (char*)0x00561ddc); break;
        case 0x9: strcpy(shortName, (char*)0x0055c360); break;
        case 0xa: strcpy(shortName, (char*)0x0053e764); break;
        case 0xb: strcpy(shortName, (char*)0x00561dd0); break;
        case 0xc: strcpy(shortName, (char*)0x00561dc8); break;
        case 0xd: strcpy(shortName, (char*)0x005417e8); break;
        case 0xe: strcpy(shortName, (char*)0x00561dbc); break;
        case 0xf: strcpy(shortName, (char*)0x00561db0); break;
        case 0x10: strcpy(shortName, (char*)0x00561da4); break;
        case 0x11: strcpy(shortName, (char*)0x00561d98); break;
        case 0x12: strcpy(shortName, (char*)0x00561d94); break;
        case 0x13: strcpy(shortName, (char*)0x0053e470); break;
        case 0x14: strcpy(shortName, (char*)0x00561d88); break;
        case 0x15: strcpy(shortName, (char*)0x00561d80); break;
        case 0x16: strcpy(shortName, (char*)0x00561d74); break;
        case 0x17: strcpy(shortName, (char*)0x00561d68); break;
        case 0x18: strcpy(shortName, (char*)0x0055c37c); break;
        case 0x19: strcpy(shortName, (char*)0x0055c32c); break;
        case 0x1a: strcpy(shortName, (char*)0x00561a9c); break;
        case 0x1b: strcpy(shortName, (char*)0x00561d5c); break;
        case 0x1c: strcpy(shortName, (char*)0x00561d50); break;
        case 0x1d: strcpy(shortName, (char*)0x0055c39c); break;
        case 0x1e: strcpy(shortName, (char*)0x0055c344); break;
        case 0x1f: strcpy(shortName, (char*)0x00561d44); break;
        case 0x20: strcpy(shortName, (char*)0x00561d38); break;
        case 0x21: strcpy(shortName, (char*)0x0055c300); break;
        case 0x22: strcpy(shortName, (char*)0x0053df4c); break;
        case 0x23: strcpy(shortName, (char*)0x00542294); break;
        case 0x24: strcpy(shortName, (char*)0x00561d2c); break;
        case 0x25: strcpy(shortName, (char*)0x0053df64); break;
        case 0x26: strcpy(shortName, (char*)0x0055c340); break;
        case 0x27: strcpy(shortName, (char*)0x00561d24); break;
        case 0x28: strcpy(shortName, (char*)0x00561d1c); break;
        case 0x29: strcpy(shortName, (char*)0x0055c3ac); break;
        case 0x2a: strcpy(shortName, (char*)0x0055c3a4); break;
        case 0x2b: strcpy(shortName, (char*)0x00561d14); break;
        case 0x2c: strcpy(shortName, (char*)0x00561d0c); break;
        case 0x2d: strcpy(shortName, (char*)0x00561d04); break;
        case 0x2e: strcpy(shortName, (char*)0x0055c314); break;
        case 0x2f: strcpy(shortName, (char*)0x0055c308); break;
        case 0x30: strcpy(shortName, (char*)0x00561cf8); break;
        case 0x31: strcpy(shortName, (char*)0x0055c3bc); break;
        case 0x32: strcpy(shortName, (char*)0x00561cec); break;
        case 0x33: strcpy(shortName, (char*)0x00561ce0); break;
        case 0x34: strcpy(shortName, (char*)0x0055c3b4); break;
        case 0x35: strcpy(shortName, (char*)0x00561cd8); break;
        case 0x36: strcpy(shortName, (char*)0x0055c3d0); break;
        case 0x37: strcpy(shortName, (char*)0x0053e468); break;
        case 0x38: strcpy(shortName, (char*)0x0055c320); break;
        case 0x39: strcpy(shortName, (char*)0x00561ccc); break;
        case 0x3a: strcpy(shortName, (char*)0x00561cc0); break;
        case 0x3b: strcpy(shortName, (char*)0x00561cb4); break;
        case 0x3c: strcpy(shortName, (char*)0x0055c3c4); break;
        case 0x3d: strcpy(shortName, (char*)0x0055c390); break;
        case 0x3e: strcpy(shortName, (char*)0x00561ca8); break;
        case 0x3f: strcpy(shortName, (char*)0x00561ca0); break;
        case 0x40: strcpy(shortName, (char*)0x00561c98); break;
        case 0x41: strcpy(shortName, (char*)0x00561c90); break;
        case 0x42: strcpy(shortName, (char*)0x00561c84); break;
        case 0x43: strcpy(shortName, (char*)0x00561c78); break;
        case 0x44: strcpy(shortName, (char*)0x0053df5c); break;
        case 0x45: strcpy(shortName, (char*)0x00561c74); break;
        case 0x46: strcpy(shortName, (char*)0x00561c68); break;
        case 0x47: strcpy(shortName, (char*)0x0055f9e8); break;
        case 0x48: strcpy(shortName, (char*)0x0053e210); break;
        case 0x49: strcpy(shortName, (char*)0x00561c5c); break;
        case 0x4a: strcpy(shortName, (char*)0x00561c50); break;
        case 0x4b: strcpy(shortName, (char*)0x0055c388); break;
        case 0x4c: strcpy(shortName, (char*)0x00561c44); break;
        case 0x4d: strcpy(shortName, (char*)0x00561c3c); break;
        case 0x4e: strcpy(shortName, (char*)0x005417f4); break;
        case 0x4f: strcpy(shortName, (char*)0x00561c24); break;
        case 0x50: strcpy(shortName, (char*)0x00561c18); break;
        case 0x51: strcpy(shortName, (char*)0x00561c10); break;
        case 0x52: strcpy(shortName, (char*)0x0055c374); break;
        case 0x53: strcpy(shortName, (char*)0x00561bf8); break;
        case 0x54: strcpy(shortName, (char*)0x005417e0); break;
        case 0x55: strcpy(shortName, (char*)0x00561be8); break;
        case 0x56: strcpy(shortName, (char*)0x00561bdc); break;
        case 0x57: strcpy(shortName, (char*)0x00561bd0); break;
        case 0x58: strcpy(shortName, (char*)0x00561bc8); break;
        case 0x59: strcpy(shortName, (char*)0x00561c08); break;
        case 0x5a: strcpy(shortName, (char*)0x00561bbc); break;
        case 0x5b: strcpy(shortName, (char*)0x00561bb4); break;
        case 0x5c: strcpy(shortName, (char*)0x00541800); break;
        case 0x5d: strcpy(shortName, (char*)0x005417d4); break;
        case 0x5e: strcpy(shortName, (char*)0x00561ba4); break;
        case 0x5f: strcpy(shortName, (char*)0x00561b98); break;
        case 0x60: strcpy(shortName, (char*)0x0053df50); break;
        case 0x61: strcpy(shortName, (char*)0x00561b90); break;
        case 0x62: strcpy(shortName, (char*)0x00561aa4); break;
        case 0x64: strcpy(shortName, (char*)0x00561ab8); break;
        case 0x65: strcpy(shortName, (char*)0x00561ab0); break;
        case 0x66: strcpy(shortName, (char*)0x00561b88); break;
        case 0x67: strcpy(shortName, (char*)0x00561b80); break;
        case 0x68: strcpy(shortName, (char*)0x00561b78); break;
        case 0x69: strcpy(shortName, (char*)0x00561b6c); break;
        case 0x6a: strcpy(shortName, (char*)0x0055c36c); break;
        case 0x6b: strcpy(shortName, (char*)0x00561b64); break;
        case 0x6c: strcpy(shortName, (char*)0x00561b5c); break;
        case 0x6d: strcpy(shortName, (char*)0x00561b54); break;
        case 0x6e: strcpy(shortName, (char*)0x00561b4c); break;
        case 0x6f: strcpy(shortName, (char*)0x00561b3c); break;
        case 0x70: strcpy(shortName, (char*)0x00561b30); break;
        case 0x71: strcpy(shortName, (char*)0x0055c464); break;
        case 0x72: strcpy(shortName, (char*)0x0053df0c); break;
        case 0x73: strcpy(shortName, (char*)0x00561b24); break;
        case 0x74: strcpy(shortName, (char*)0x00561b18); break;
        case 0x75: strcpy(shortName, (char*)0x00561ae8); break;
        case 0x76: strcpy(shortName, (char*)0x00561b0c); break;
        case 0x77: strcpy(shortName, (char*)0x00561b00); break;
        case 0x78: strcpy(shortName, (char*)0x00561af4); break;
        case 0x79: strcpy(shortName, (char*)0x00561ae0); break;
        case 0x7a: strcpy(shortName, (char*)0x00561adc); break;
        case 0x7b: strcpy(shortName, (char*)0x0053df18); break;
        case 0x7c: strcpy(shortName, (char*)0x00561acc); break;
        case 0x7d: strcpy(shortName, (char*)0x00561ac4); break;
        case 0x7e: strcpy(shortName, (char*)0x0053df30); break;
        case 0x7f: strcpy(shortName, (char*)0x0053df24); break;
        case 0x80: strcpy(shortName, (char*)0x0053df04); break;
        case 0x81: strcpy(shortName, (char*)0x0053df40); break;
        case 0x96: strcpy(shortName, (char*)0x00561a84); break;
        case 0x97: strcpy(shortName, (char*)0x00561a7c); break;
        case 0x98: strcpy(shortName, (char*)0x00561a74); break;
        case 0x99: strcpy(shortName, (char*)0x00561a6c); break;
        case 0x9a: strcpy(shortName, (char*)0x00561a64); break;
        case 0x9b: strcpy(shortName, (char*)0x00561a5c); break;
        case 0x9c: strcpy(shortName, (char*)0x00561a54); break;
        case 0x9d: strcpy(shortName, (char*)0x00561a48); break;
        case 0x9e: strcpy(shortName, (char*)0x00561a40); break;
        case 0x9f: strcpy(shortName, (char*)0x00561a30); break;
        case 0xa0: strcpy(shortName, (char*)0x00561a38); break;
        case 0xa1: strcpy(shortName, (char*)0x00561a14); break;
        case 0xa2: strcpy(shortName, (char*)0x00561a08); break;
        case 0xa3: strcpy(shortName, (char*)0x005619fc); break;
        case 0xa4: strcpy(shortName, (char*)0x005619f0); break;
        case 0xa5: strcpy(shortName, (char*)0x0055c3dc); break;
        case 0xa6: strcpy(shortName, (char*)0x005619bc); break;
        case 0xa7: strcpy(shortName, (char*)0x005619e4); break;
        case 0xa8: strcpy(shortName, (char*)0x005619dc); break;
        case 0xa9: strcpy(shortName, (char*)0x005619d4); break;
        case 0xaa: strcpy(shortName, (char*)0x005619c8); break;
        case 0xab: strcpy(shortName, (char*)0x005619b4); break;
        case 0xac: strcpy(shortName, (char*)0x005619a8); break;
        case 0xad: strcpy(shortName, (char*)0x005619a0); break;
        case 0xae: strcpy(shortName, (char*)0x00561994); break;
        case 0xaf: strcpy(shortName, (char*)0x0056198c); break;
        case 0xb0: strcpy(shortName, (char*)0x00561984); break;
        case 0xb1: strcpy(shortName, (char*)0x00561a28); break;
        case 0xb2: strcpy(shortName, (char*)0x00561a20); break;
        case 0xb3: strcpy(shortName, (char*)0x00569124); break;
        case 0x3e4: strcpy(shortName, (char*)0x00561c34); break;
        case 0x3e5: strcpy(shortName, (char*)0x00561a90); break;
        case 0x3e7: strcpy(shortName, (char*)0x0056991c); break;
        case 0x3e8: strcpy(shortName, (char*)0x00569924); break;
        default:
            *shortName = 0;
            return 0;
    }
    return 1;
}

// Owner class unknown: the direct callers pass [0x006E4D54] in ECX, and this function never reads `this`.
struct ZoneNameTable_guess
{
    int ZoneIdFromShortName_guess(char* name);
};

// FUNCTION: TRILOGY 0x004C05AE
// Reverse zone lookup: short zone name (qeynos, freportw...) to zone id, for zone loading and zone checks
// __thiscall, `this` unused. Case-insensitive match of the zone short name against the table in order;
// returns the zone id, or 0 when no name matches.
int ZoneNameTable_guess::ZoneIdFromShortName_guess(char* name)
{
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3E8)) == 0)
        return 1; // qeynos
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C358)) == 0)
        return 2; // qeynos2
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E760)) == 0)
        return 3; // DAT_0053e760
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E754)) == 0)
        return 4; // qeytoqrg
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561E00)) == 0)
        return 5; // highpass
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DF4)) == 0)
        return 6; // highkeep
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DE8)) == 0)
        return 7; // highcave
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DDC)) == 0)
        return 8; // freportn
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C360)) == 0)
        return 9; // freportw
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E764)) == 0)
        return 0xa; // freporte
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DD0)) == 0)
        return 0xb; // runnyeye
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DC8)) == 0)
        return 0xc; // qey2hh1
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005417E8)) == 0)
        return 0xd; // northkarana
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DBC)) == 0)
        return 0xe; // southkarana
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DB0)) == 0)
        return 0xf; // eastkarana
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561DA4)) == 0)
        return 0x10; // beholder
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D98)) == 0)
        return 0x11; // blackburrow
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D94)) == 0)
        return 0x12; // DAT_00561d94
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E470)) == 0)
        return 0x13; // rivervale
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D88)) == 0)
        return 0x14; // kithicor
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D80)) == 0)
        return 0x15; // commons
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D74)) == 0)
        return 0x16; // ecommons
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D68)) == 0)
        return 0x17; // erudnint
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C37C)) == 0)
        return 0x18; // erudnext
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C32C)) == 0)
        return 0x19; // nektulos
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A9C)) == 0)
        return 0x1a; // cshome
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D5C)) == 0)
        return 0x1b; // lavastorm
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D50)) == 0)
        return 0x1c; // nektropos
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C39C)) == 0)
        return 0x1d; // halas
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C344)) == 0)
        return 0x1e; // everfrost
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D44)) == 0)
        return 0x1f; // soldunga
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D38)) == 0)
        return 0x20; // soldungb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C300)) == 0)
        return 0x21; // misty
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF4C)) == 0)
        return 0x22; // DAT_0053df4c
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00542294)) == 0)
        return 0x23; // DAT_00542294
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D2C)) == 0)
        return 0x24; // befallen
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF64)) == 0)
        return 0x25; // oasis
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C340)) == 0)
        return 0x26; // DAT_0055c340
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D24)) == 0)
        return 0x27; // DAT_00561d24
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D1C)) == 0)
        return 0x28; // neriaka
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3AC)) == 0)
        return 0x29; // neriakb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3A4)) == 0)
        return 0x2a; // neriakc
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D14)) == 0)
        return 0x2b; // neriakd
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D0C)) == 0)
        return 0x2c; // najena
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561D04)) == 0)
        return 0x2d; // DAT_00561d04
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C314)) == 0)
        return 0x2e; // innothule
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C308)) == 0)
        return 0x2f; // feerrott
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CF8)) == 0)
        return 0x30; // cazicthule
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3BC)) == 0)
        return 0x31; // oggok
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CEC)) == 0)
        return 0x32; // rathemtn
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CE0)) == 0)
        return 0x33; // lakerathe
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3B4)) == 0)
        return 0x34; // grobb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CD8)) == 0)
        return 0x35; // aviak
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3D0)) == 0)
        return 0x36; // gfaydark
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E468)) == 0)
        return 0x37; // akanon
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C320)) == 0)
        return 0x38; // steamfont
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CCC)) == 0)
        return 0x39; // lfaydark
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CC0)) == 0)
        return 0x3a; // crushbone
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CB4)) == 0)
        return 0x3b; // mistmoore
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3C4)) == 0)
        return 0x3c; // kaladima
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C390)) == 0)
        return 0x3d; // felwithea
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CA8)) == 0)
        return 0x3e; // felwitheb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561CA0)) == 0)
        return 0x3f; // unrest
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C98)) == 0)
        return 0x40; // kedge
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C90)) == 0)
        return 0x41; // guktop
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C84)) == 0)
        return 0x42; // gukbottom
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C78)) == 0)
        return 0x43; // kaladimb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF5C)) == 0)
        return 0x44; // butcher
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C74)) == 0)
        return 0x45; // DAT_00561c74
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C68)) == 0)
        return 0x46; // cauldron
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055F9E8)) == 0)
        return 0x47; // airplane
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053E210)) == 0)
        return 0x48; // fearplane
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C5C)) == 0)
        return 0x49; // permafrost
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C50)) == 0)
        return 0x4a; // kerraridge
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C388)) == 0)
        return 0x4b; // paineel
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C44)) == 0)
        return 0x4c; // hateplane
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C3C)) == 0)
        return 0x4d; // arena
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005417F4)) == 0)
        return 0x4e; // fieldofbone
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C24)) == 0)
        return 0x4f; // warslikswood
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C18)) == 0)
        return 0x50; // soltemple
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C10)) == 0)
        return 0x51; // droga
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C374)) == 0)
        return 0x52; // cabwest
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BF8)) == 0)
        return 0x53; // swampofnohope
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005417E0)) == 0)
        return 0x54; // firiona
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BE8)) == 0)
        return 0x55; // lakeofillomen
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BDC)) == 0)
        return 0x56; // dreadlands
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BD0)) == 0)
        return 0x57; // burningwood
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BC8)) == 0)
        return 0x58; // kaesora
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C08)) == 0)
        return 0x59; // sebilis
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BBC)) == 0)
        return 0x5a; // citymist
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BB4)) == 0)
        return 0x5b; // skyfire
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00541800)) == 0)
        return 0x5c; // frontiermtns
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005417D4)) == 0)
        return 0x5d; // overthere
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561BA4)) == 0)
        return 0x5e; // emeraldjungle
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B98)) == 0)
        return 0x5f; // trakanon
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF50)) == 0)
        return 0x60; // timorous
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B90)) == 0)
        return 0x61; // DAT_00561b90
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AA4)) == 0)
        return 0x62; // erudsxing
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AB8)) == 0)
        return 0x64; // stonebrunt
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AB0)) == 0)
        return 0x65; // warrens
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B88)) == 0)
        return 0x66; // karnor
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B80)) == 0)
        return 0x67; // chardok
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B78)) == 0)
        return 0x68; // dalnir
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B6C)) == 0)
        return 0x69; // charasis
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C36C)) == 0)
        return 0x6a; // cabeast
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B64)) == 0)
        return 0x6b; // nurga
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B5C)) == 0)
        return 0x6c; // veeshan
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B54)) == 0)
        return 0x6d; // veksar
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B4C)) == 0)
        return 0x6e; // iceclad
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B3C)) == 0)
        return 0x6f; // frozenshadow
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B30)) == 0)
        return 0x70; // velketor
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C464)) == 0)
        return 0x71; // DAT_0055c464
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF0C)) == 0)
        return 0x72; // skyshrine
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B24)) == 0)
        return 0x73; // thurgadina
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B18)) == 0)
        return 0x74; // eastwastes
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AE8)) == 0)
        return 0x75; // cobaltscar
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B0C)) == 0)
        return 0x76; // greatdivide
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561B00)) == 0)
        return 0x77; // wakening
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AF4)) == 0)
        return 0x78; // westwastes
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AE0)) == 0)
        return 0x79; // crystal
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561ADC)) == 0)
        return 0x7a; // DAT_00561adc
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF18)) == 0)
        return 0x7b; // necropolis
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561ACC)) == 0)
        return 0x7c; // templeveeshan
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561AC4)) == 0)
        return 0x7d; // sirens
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF30)) == 0)
        return 0x7e; // mischiefplane
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF24)) == 0)
        return 0x7f; // growthplane
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF04)) == 0)
        return 0x80; // sleeper
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0053DF40)) == 0)
        return 0x81; // thurgadinb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A84)) == 0)
        return 0x96; // shadowhaven
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A7C)) == 0)
        return 0x97; // bazaar
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A74)) == 0)
        return 0x98; // nexus
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A6C)) == 0)
        return 0x99; // DAT_00561a6c
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A64)) == 0)
        return 0x9a; // acrylia
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A5C)) == 0)
        return 0x9b; // vahshir
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A54)) == 0)
        return 0x9c; // paludal
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A48)) == 0)
        return 0x9d; // fungusgrove
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A40)) == 0)
        return 0x9e; // vexthal
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A30)) == 0)
        return 0x9f; // sseru
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A38)) == 0)
        return 0xa0; // katta
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A14)) == 0)
        return 0xa1; // netherbian
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A08)) == 0)
        return 0xa2; // ssratemple
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619FC)) == 0)
        return 0xa3; // griegsend
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619F0)) == 0)
        return 0xa4; // deepshade
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0055C3DC)) == 0)
        return 0xa5; // shadeweaver
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619BC)) == 0)
        return 0xa6; // hollowshade
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619E4)) == 0)
        return 0xa7; // grimling
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619DC)) == 0)
        return 0xa8; // mseru
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619D4)) == 0)
        return 0xa9; // letalis
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619C8)) == 0)
        return 0xaa; // twilight
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619B4)) == 0)
        return 0xab; // DAT_005619b4
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619A8)) == 0)
        return 0xac; // tenebrous
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x005619A0)) == 0)
        return 0xad; // maiden
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561994)) == 0)
        return 0xae; // dawnshroud
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0056198C)) == 0)
        return 0xaf; // scarlet
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561984)) == 0)
        return 0xb0; // umbral
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A28)) == 0)
        return 0xb1; // sseru2
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A20)) == 0)
        return 0xb2; // sseru3
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00569124)) == 0)
        return 0xb3; // havenb
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561C34)) == 0)
        return 0x3e4; // arena2
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00561A90)) == 0)
        return 0x3e5; // apprentice
    if (SWD_STRCMPI(name, EQ_ARRAY(char, 0x0056991C)) == 0)
        return 0x3e7; // DAT_0056991c
    return (SWD_STRCMPI(name, EQ_ARRAY(char, 0x00569924)) == 0) ? 0x3e8 : 0; // jrbase1
}

// FUNCTION: TRILOGY 0x004d0516
// Invokes a client control when state checks pass; its in-game role is unclear.
void __fastcall CallPointerMethodWhenGlobalsAreClear_guess(int everQuestObject, int edx_unused)
{
    if (EQ_GLOBAL(int, 0x006E4E70) == 0
        && EQ_GLOBAL(int, 0x006E4E6C) == 0
        && EQ_GLOBAL(void*, 0x006E4DE8) != 0)
    {
        EQ_METHOD(1, void, 0x0048B9A0, (void*, void*), EQ_GLOBAL(void*, 0x006E4DE8))((void*)((u32)everQuestObject + 0xBA8));
    }
}

// FUNCTION: TRILOGY 0x004d0540
// When globals 0x006E4E70 and 0x006E4E6C are zero, clears the byte at +0xBA8 and calls 0x004D0516.
// ORIGINAL FILE: EverQuest/Display.cpp (from Mac debug map)
void __fastcall CDisplay_Render_World_guess(CDisplay* self, int edx_unused)
{
    if (EQ_GLOBAL(int, 0x006E4E70) == 0
        && EQ_GLOBAL(int, 0x006E4E6C) == 0)
    {
        ((u8*)self)[0xBA8] = 0;
        EQ_METHOD(0, void, 0x004D0516, (CDisplay*), self)();
        return;
    }
    return;
}

// FUNCTION: TRILOGY 0x004dcfe8
// Resolves the world object selected at the given screen coordinates.
int __stdcall ClickedObject(short x, short y)
{
    int actor;

    actor = EQ_METHOD(3, int, 0x0041125D, (void*, int, int, int), g_display_guess)((int)x, (int)y, 1);
    return EQ_METHOD(1, int, 0x004B0334, (void*, int), EQ_GLOBAL(void*, 0x006E4D6C))(actor);
}

// FUNCTION: TRILOGY 0x004e1f89
// Sets the zone's outdoor, castle, and house sound environment names.
// __fastcall (ECX = environment, EDX unused, one stack argument, callee pops 4). Copies the three sound
// environment names into the object: "OutPlain" at +0x94 (state 1, far), "InCastle" at +0x14 (state 2) and
// "InHouse" at +0x54 (state 3, near). Then, if zoneName matches a zone record (first character exact, then
// case-insensitive; first match wins), each name is replaced by the string that record selects through the
// pointer table at 0x00578658.
void __fastcall SetZoneSoundEnvironments_guess(char* environment, int edx_unused, char* zoneName)
{
    int cmpResult;
    int recordIndex;
    char* record;

    SOC_CPY
        (environment + 0x94, (const char*)0x00579368);
    SOC_CPY
        (environment + 0x14, (const char*)0x0057933C);
    SOC_CPY
        (environment + 0x54, (const char*)0x00579354);
    recordIndex = 0;
    if (0 < g_zone_env_count_guess)
    {
        // Each record is 0x20 bytes: the zone short name at +0x00, then the pointer-table indices for the
        // +0x94, +0x14 and +0x54 names at +0x14, +0x18 and +0x1C.
        record = (char*)0x00578690;
        do
        {
            // The first character must match exactly: a cheap reject before the case-insensitive compare.
            if (!((bool)((*zoneName == *record))))
            {
            }
            else
            {
                int sameName;
                sameName = (cmpResult = SWD_STRCMPI(zoneName, record),
                    cmpResult == 0);
                if (!sameName)
                {
                }
                else
                {
                    SOC_CPY
                        (environment + 0x94, *(char**)(0x00578658 + *(int*)(0x005786A4 + recordIndex * 0x20) * 4));
                    SOC_CPY
                        (environment + 0x14, *(char**)(0x00578658 + *(int*)(0x005786A8 + recordIndex * 0x20) * 4));
                    SOC_CPY
                        (environment + 0x54, *(char**)(0x00578658 + *(int*)(0x005786AC + recordIndex * 0x20) * 4));
                    return;
                }
            }
            record = record + 0x20;
            recordIndex = recordIndex + 1;
        } while (recordIndex < g_zone_env_count_guess);
    }
    return;
}

// FUNCTION: TRILOGY 0x004E2074
// Processes a player's sound data after checking its range and tracking eligibility.
// __thiscall (ECX = self, one stack argument, callee pops 4). Returns EqSoundManagerFunction_guess::PlaySoundForPlayer_guess's result when `target`
// passes every check: self+0x104 below 11, the flag at 0x579324 set, FindPlayerIndex_guess negative, IsPlayerAudible_guess
// true, AddPlayerToList_guess not -1, and its actor_info dword +0x228 nonzero and below 0x2D0. Otherwise false
// (only AL is cleared on that path, so the upper EAX bits keep their old value).
/* fake match: the final != 0 re-reads actor_info+0x228; with the same local VC6 drops the repeated TEST/JZ the original has. */
bool __fastcall ProcessAudiblePlayerSound_guess(char* self, int edx_unused, char* target)
{
    u32 value;

    if (target != 0 && *(int*)(self + 0x104) < 0xb && EQ_GLOBAL(u8, 0x00579324) != 0
        && EQ_METHOD(1, int, 0x004E21FE, (char*, char*), self)(target) < 0
        && EQ_METHOD(1, bool, 0x004E27CB, (char*, char*), self)(target)
        && EQ_METHOD(1, int, 0x004E21A3, (char*, char*), self)(target) != -1
        && (value = *(u32*)(*(int*)(target + 0x70) + 0x228)) != 0 && (int)value < 0x2d0
        && *(u32*)(*(int*)(target + 0x70) + 0x228) != 0)
    {
        return EQ_METHOD(2, bool, 0x004E20DF, (char*, char*, int), self)(target, value);
    }
    return false;
}

// FUNCTION: TRILOGY 0x004E21A3
// Tracks a player's sound data in the manager's active list.
// __thiscall. Adds `player` to the first free of the 8 dwords at 0x2F0 (unless FindPlayerIndex_guess already
// finds it: returns 0), copying its actor_info (+0x70) dword +0x228 to 0x31C[i], clearing 0x348[i] and
// incrementing the count at 0x108; returns the index, or -1 when full.
int EqSoundManager_guess::AddPlayerToList_guess(EQPlayer* player)
{
    int i;
    EQPlayer** p;

    if (FindPlayerIndex_guess(player) >= 0)
        return 0;
    for (i = 0, p = players; i < 8; i++, p++)
    {
        if (*p != 0)
        {
        }
        else
        {
            players[i] = player;
            player_data_a[i] = player->actor_info->sound_data_a_guess;
            player_data_b[i] = 0;
            active_players++;
            return i;
        }
    }
    return -1;
}

// FUNCTION: TRILOGY 0x004e21fe
// __thiscall on an object whose 8 dwords at 0x2F0 are searched for `target`; returns the index or -1.
int EqSoundManager_guess::FindPlayerIndex_guess(EQPlayer* target)
{
    EQPlayer** p;
    int i;

    EQPlayer** temp;
    temp = players;
    i = 0;
    p = temp;
    do
    {
        if (*p != target)
            {
        }
        else
            return i;
        i++;
        p++;
    } while (i < 8);
    return -1;
}

// FUNCTION: TRILOGY 0x004e27cb
// Checks whether a player's sound is recent and within its distance-based hearing range.
// Audibility test for `player` from `self`. False unless the actor's 0x19C time is at most 0x2EE0 ms behind the
// CDisplay clock (g_displayManager_guess + 0xBC); the unsigned difference also fails a future time. Otherwise true unless the
// local-player squared distance (local_player_distance_squared_guess) exceeds (info 0x1A0 * k)^2, k = 128 for self 0xC == 1, 64 for == 2, 16 else.
// VC6 has no 80-bit type: the x87 values are doubles (the square is exact, the distance rounds to 53 bits); NaN gives true.
bool __fastcall IsPlayerAudible_guess(char* self, int edx_unused, EQPlayer* player)
{
    double distance;
    double range;

    if ((u32)(*(int*)((char*)g_cdisplay_guess + 0xBC) - *(int*)((char*)player->actor_info + 0x19C)) <= 0x2EE0)
    {
        distance = EQ_THISCALL(double, 0x004E2798, (char*, int, float, float, float))(self, edx_unused, player->y, player->x, player->z);
        switch (*(int*)(self + 0x0C))
        {
        case 1:
            range = player->actor_info->unk_0x1A0 * 128.0f;
            break;
        case 2:
            range = player->actor_info->unk_0x1A0 * 64.0f;
            break;
        default:
            range = player->actor_info->unk_0x1A0 * 16.0f;
            break;
        }
        range = range * range;
        if (!(distance > range))
            return true;
    }
    return false;
}

// Interface at 0x006E4DD0; the query used here is vtable slot 15 (+0x3C).
class SoundRangeQuery_guess
{
public:
    virtual void vfunc0() = 0;
    virtual void vfunc1() = 0;
    virtual void vfunc2() = 0;
    virtual void vfunc3() = 0;
    virtual void vfunc4() = 0;
    virtual void vfunc5() = 0;
    virtual void vfunc6() = 0;
    virtual void vfunc7() = 0;
    virtual void vfunc8() = 0;
    virtual void vfunc9() = 0;
    virtual void vfunc10() = 0;
    virtual void vfunc11() = 0;
    virtual void vfunc12() = 0;
    virtual void vfunc13() = 0;
    virtual void vfunc14() = 0;
    virtual void getRange_guess(int soundId, float* range) = 0;
};

// FUNCTION: TRILOGY 0x004e286f
// Plays a ranged world sound, adjusting its volume at the local player's position.
void __fastcall PlayRangedWorldSound_guess(char* self, int edx_unused,
                             float y, float x, float z, short soundId)
{
    int distance;
    int rangeInUnits;
    float range;
    EQPlayer* localPlayer;

    if (soundId < 1 || soundId > 0x2CF || g_sound_manager_ptr_guess == 0)
        goto done;

    distance = (int)EQ_STDCALL(float, 0x004E270B, (float, float, float))(y, x, z);

    ((SoundRangeQuery_guess*)EQ_GLOBAL(void*, 0x006E4DD0))->getRange_guess(soundId, &range);

    if (*(int*)(self + 0x0C) == 1)
        rangeInUnits = (int)range << 6;
    else if (*(int*)(self + 0x0C) == 2)
        rangeInUnits = (int)range << 7;
    else
        rangeInUnits = (int)range << 4;

    if (rangeInUnits < distance)
        goto done;

    {
    EQSoundBuffer_guess soundBuffer;
    *(u32*)&soundBuffer = 0x00535798;
    *(float*)((char*)&soundBuffer + 0x04) = y;
    *(float*)((char*)&soundBuffer + 0x08) = z;
    *(float*)((char*)&soundBuffer + 0x0C) = x;

    soundBuffer.changed_flags = 1;

    int volume;

    localPlayer = EQ_GLOBAL(EQPlayer*, 0x006E4D8C);
    if (y == localPlayer->y && x == localPlayer->x)
    {
        EQ_METHOD(1, void, 0x004ECBC2, (EQSoundBuffer_guess*, u8), &soundBuffer)(0x10);

        if (soundId < 0x67)
        {
            if (soundId < 0x65)
            {
                if (soundId < 0x5C)
                    goto default_volume;
                if (soundId < 0x60)
                    volume = 0xFFFFF880;
                else if (soundId < 0x62 || soundId == 0x62)
                    volume = 0xFFFFFE0C;
                else if (soundId == 0x63)
                    volume = 0xFFFFFD9E;
                else
                {
                    if (soundId != 0x64)
                        goto default_volume;
                    volume = 0xFFFFFE48;
                }
            }
            else
                volume = 0xFFFFFD44;
        }
        else
        {
            if (soundId < 0x74)
            {
                if (soundId == 0x73)
                    volume = 0xFFFFFDA8;
                else if (soundId > 0x66)
                {
                    if (soundId < 0x6C)
                        volume = 0xFFFFFA88;
                    else if (soundId == 0x72)
                        volume = 0xFFFFFE0C;
                    else
                        goto default_volume;
                }
                else
                    goto default_volume;
            }
            else if (soundId > 0x92)
            {
                if (soundId < 0x96)
                    volume = 0xFFFFFDA8;
                else if (soundId > 0x96 && soundId < 0x9B)
                    volume = 0xFFFFF880;
                else
                    goto default_volume;
            }
            else
                goto default_volume;
        }
        goto set_volume;

default_volume:
        volume = 0xFFFFFB50;
set_volume:
        EQ_METHOD(1, bool, 0x004ECBA1, (EQSoundBuffer_guess*, int), &soundBuffer)(volume);
    }

    EQ_METHOD(2, void, 0x004E34FA, (void*, int, EQSoundBuffer_guess*), g_sound_manager_ptr_guess)(soundId, &soundBuffer);
    }

done:
    return;
}

// Owner class unknown: the interface at 0x006E4DD0, seen through its vtable slots 7 (+0x1C) and 11
// (+0x2C), each taking a pointer to a handle.
class VtblSlots7And11_guess
{
public:
    virtual void vfunc0();
    virtual void vfunc1();
    virtual void vfunc2();
    virtual void vfunc3();
    virtual void vfunc4();
    virtual void vfunc5();
    virtual void vfunc6();
    virtual void vfunc7(int* handle);
    virtual void vfunc8();
    virtual void vfunc9();
    virtual void vfunc10();
    virtual int vfunc11(int* handle);
};

// FUNCTION: TRILOGY 0x004e363b
// Releases the active audio handle associated with a sound ID.
// __thiscall (owner class unknown; callers pass [0x006E4DD8]). For ids 0x9E..0xA1 the handle at
// 0xDB0 + id*4 is passed to slots 11 and 7 of the interface at 0x006E4DD0; for 0xA2..0xA9 the handle at
// 0xD80 + id*4 goes to slot 11 and the one at 0xDA0 + id*4 to slot 7. The handle is then set to -1 and
// slot 11's result returned; 0 when the id is out of range or the handle is already -1.
int __fastcall StopWave_guess(char* self, int edx_unused, int id)
{
    int* handle;
    int result;

    if (id >= 0x9E)
    {
        if (id > 0xA1)
        {
            if (id <= 0xA9)
            {
                handle = (int*)(self + 0xD80 + id * 4);
                if (*handle != -1)
                {
                    result = EQ_GLOBAL(VtblSlots7And11_guess*, 0x006E4DD0)->vfunc11(handle);
                    EQ_GLOBAL(VtblSlots7And11_guess*, 0x006E4DD0)->vfunc7((int*)(self + 0xDA0 + id * 4));
                    *handle = -1;
                    return result;
                }
            }
        }
        else
        {
            handle = (int*)(self + 0xDB0 + id * 4);
            if (*handle != -1)
            {
                result = EQ_GLOBAL(VtblSlots7And11_guess*, 0x006E4DD0)->vfunc11(handle);
                EQ_GLOBAL(VtblSlots7And11_guess*, 0x006E4DD0)->vfunc7(handle);
                *handle = -1;
                return result;
            }
        }
    }
    return 0;
}

// 16 bytes passed by value (the GUID at 0x00534D70 is copied onto the stack).
struct Guid16_guess
{
    u32 words[4]; // 0x00
};

typedef int (__stdcall *CoCreateInstanceFn_guess_t)(void*, void*, int, void*, void**);
typedef int (__stdcall *ComQueryInterfaceFn_guess_t)(void*, void*, void**);
typedef int (__stdcall *ComRegisterSinkFn_guess_t)(void*, void*, int, int, void*, void*,
                                                   Guid16_guess, int);
typedef int (__stdcall *ComOneArgFn_guess_t)(void*, int);
typedef int (__stdcall *ComTimeoutsFn_guess_t)(void*, int*);
typedef int (__stdcall *ComGetValueFn_guess_t)(void*, int, int*);
typedef u32 (__stdcall *ComReleaseFn_guess_t)(void*);

// File-local stand-in for gpVDctNotifySink: operator new(8), then the constructor at 0x004E7FAD, whose result is the
// sink. The constructor is only declared (it is the function at 0x004E7FAD below, reached by its address), so VC6
// wraps the new in an EH frame (as the original does).
struct VDctNotifySink_guess
{
    u32 vtable;   // 0x00 (set to 0x005357BC by the constructor)
    u32 field_04; // 0x04 (cleared by the constructor)

    // EXTERNAL: TRILOGY 0x004E7FAD VDctNotifySink_guess::VDctNotifySink_guess
    VDctNotifySink_guess(); // 0x004E7FAD
};

// FUNCTION: TRILOGY 0x004e708b
// Initializes and activates the voice dictionary used by voice recognition.
// @early-stop: frame is 0x30 not 0x2c (the sink's new temp takes its own slot at ebp-0x10; the original shares it with values[0] at ebp-0x20).
// __cdecl, one argument: the value given to the dictionary's slot 13 (Activate), then reused as an out pointer.
// Creates the voice dictionary (CoCreateInstance, QueryInterface, notify sink, timeouts, Activate) and traces the
// VR settings. A failing step traces its message (with the HRESULT, except CoCreateInstance) and returns the HRESULT. Kept from the original: the
// timeouts call runs before its QueryInterface result is tested and its own result is ignored; the success path
// returns the Activate result. The sink's new keeps the original's EH frame (its constructor is defined later in this file).
int InitializeVoiceDictionary_guess(int activateArg)
{
    int hr;
    int hres;
    char *message;
    void *dict;
    VDctNotifySink_guess *sink;
    int timeouts[6];
    int qiResult;

    hr = ((CoCreateInstanceFn_guess_t)EQ_GLOBAL(void *, 0x00534374))(
        (void *)0x00534F00, 0, 4, (void *)0x00534DB0, &EQ_GLOBAL(void *, 0x006ECE4C));
    if (hr < 0)
    {
        message = (char *)0x0057C744;
        EQTRACE_guess(message);
        fdebug(message);
        return hr;
    }
    ((ComQueryInterfaceFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[0])(
        EQ_GLOBAL(void *, 0x006ECE4C), (void *)0x00534670, &EQ_GLOBAL(void *, 0x006ECE58));
    hres = ((ComQueryInterfaceFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[0])(
        EQ_GLOBAL(void *, 0x006ECE4C), (void *)0x00534DD0, &EQ_GLOBAL(void *, 0x006ECE54));
    if (hres >= 0)
    {
        sink = new VDctNotifySink_guess();
        EQ_GLOBAL(void *, 0x006ECE38) = sink;
        if (sink == 0)
        {
            EQTRACE_guess((const char *)0x0057C71C);
        }
        hres = ((ComRegisterSinkFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[3])(
            EQ_GLOBAL(void *, 0x006ECE4C), (void *)0x0058FBA4, 0, 0, (void *)0x0058FBA4,
            EQ_GLOBAL(void *, 0x006ECE38), EQ_GLOBAL(Guid16_guess, 0x00534D70), 3);
        if (hres >= 0)
        {
            EQ_GLOBAL(void *, 0x006ECE50) = 0;
            hres = ((ComQueryInterfaceFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[0])(
                EQ_GLOBAL(void *, 0x006ECE4C), (void *)0x00534E40, &EQ_GLOBAL(void *, 0x006ECE50));
            memset(timeouts, 0, sizeof(timeouts));
            dict = EQ_GLOBAL(void *, 0x006ECE50);
            timeouts[0] = 250000;
            timeouts[1] = 1000000;
            timeouts[2] = 5000;
            timeouts[3] = 0;
            timeouts[4] = 0;
            timeouts[5] = 0;
            ((ComTimeoutsFn_guess_t)(*(void ***)dict)[20])(dict, timeouts);
            if (hres >= 0)
            {
                ((ComOneArgFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE50))[18])(
                    EQ_GLOBAL(void *, 0x006ECE50), 0x40);
                hres = ((ComOneArgFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[13])(
                    EQ_GLOBAL(void *, 0x006ECE4C), activateArg);
                if (hres >= 0)
                {
                    EQTRACE_guess((const char *)0x0057C704);
                    fdebug((const char *)0x0057C704);
                    activateArg = 0;
                    qiResult = ((ComQueryInterfaceFn_guess_t)(*(void ***)EQ_GLOBAL(void *, 0x006ECE4C))[0])(
                        EQ_GLOBAL(void *, 0x006ECE4C), (void *)0x00534630, (void **)&activateArg);
                    if (qiResult >= 0)
                    {
                        int values[5];

                        ((ComGetValueFn_guess_t)(*(void ***)activateArg)[3])((void *)activateArg, 0x200A, &values[0]);
                        ((ComGetValueFn_guess_t)(*(void ***)activateArg)[3])((void *)activateArg, 6, &values[1]);
                        ((ComGetValueFn_guess_t)(*(void ***)activateArg)[3])((void *)activateArg, 0x2010, &values[2]);
                        ((ComGetValueFn_guess_t)(*(void ***)activateArg)[3])((void *)activateArg, 8, &values[3]);
                        ((ComGetValueFn_guess_t)(*(void ***)activateArg)[3])((void *)activateArg, 0x2008, &values[4]);
                        EQTRACE_guess((const char *)0x0057C69C,
                            values[0], values[1], values[2], values[3], values[4]);
                        ((ComReleaseFn_guess_t)(*(void ***)activateArg)[2])((void *)activateArg);
                    }
                    return hres;
                }
                message = (char *)0x0057C66C;
            }
            else
            {
                message = (char *)0x0057C634;
            }
        }
        else
        {
            message = (char *)0x0057C604;
        }
    }
    else
    {
        message = (char *)0x0057C5CC;
    }
    EQTRACE_guess(message, hres);
    fdebug(message, hres);
    return hres;
}

typedef int (__stdcall *InitCoInitializeFn_guess_t)(void *reserved);

// FUNCTION: TRILOGY 0x004e7a0e
// Initializes voice recognition command and dictionary components and sets the system active.
// __cdecl, one argument (passed on to the voice recognition and DirectSound steps). Returns 1 at once when the
// system is already active. Otherwise it traces the start, runs CoInitialize, InitializeVoiceRecognition and
// InitializeDirectSoundDevice, and returns 0 after a failing step (tracing its message with the HRESULT). On
// success it sets the flags at 0x006ECE3C and 0x006ECE40 to 1 and returns 1.
int InitializeDirectSound_guess(int activateArg)
{
    int hr;
    int hres;

    if (EQ_GLOBAL(int, 0x006ECE3C) != 0)
    {
        EQTRACE_guess("Voice rec is already active.\n");
        return 1;
    }
    EQTRACE_guess("Starting Voice Recognition...\n");
    fdebug("Starting Voice Recognition...\n");
    hr = EQ_GLOBAL(InitCoInitializeFn_guess_t, 0x00534370)(0);
    if (hr < 0)
    {
        EQTRACE_guess("OLE initialization failed.\n");
        fdebug("OLE initialization failed.\n");
        return 0;
    }
    hres = EQ_CDECL(int, 0x004E72E1, (int))(activateArg);
    if (hres < 0)
    {
        EQTRACE_guess("initVCmd Failed, hRes = %lx\n", hres);
        fdebug("initVCmd Failed, hRes = %lx\n", hres);
        return 0;
    }
    hres = EQ_CDECL(int, 0x004E708B, (int))(activateArg);
    if (hres < 0)
    {
        EQTRACE_guess("initVDct Failed, hRes = %lx\n", hres);
        fdebug("initVDct Failed, hRes = %lx\n", hres);
        return 0;
    }
    EQ_GLOBAL(int, 0x006ECE3C) = 1;
    EQ_GLOBAL(int, 0x006ECE40) = 1;
    return 1;
}

typedef u32 (__stdcall *VoiceRecognitionReleaseFn_guess_t)(void*);
typedef void (__stdcall *CoUninitializeFn_guess_t)(void);

// FUNCTION: TRILOGY 0x004e7ab2
// Shuts down voice recognition and releases its active resources.
void ShutdownVoiceRecognitionSystem_guess(void)
{
    void *object;

    if (EQ_GLOBAL(int, 0x006ECE3C) != 0)
    {
        EQTRACE_guess((const char *)0x0057CCF4);
        object = EQ_GLOBAL(void *, 0x006ECE44);
        if (object != 0)
        {
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQTRACE_guess((const char *)0x0057CCE8);
            EQ_GLOBAL(void *, 0x006ECE44) = 0;
        }

        if (EQ_GLOBAL(void *, 0x006ECE48) != 0)
        {
            EQTRACE_guess((const char *)0x0057CCDC);
            object = EQ_GLOBAL(void *, 0x006ECE48);
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQTRACE_guess((const char *)0x0057CCD0);
            EQ_GLOBAL(void *, 0x006ECE48) = 0;
        }

        if (EQ_GLOBAL(void *, 0x006ECE54) != 0)
        {
            EQ_CDECL(void, 0x004E7D3B, (int, int, int))(0, 0x0FFFFFFF, 0);
            EQTRACE_guess((const char *)0x0057CCC0);
            object = EQ_GLOBAL(void *, 0x006ECE54);
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQ_GLOBAL(void *, 0x006ECE54) = 0;
        }

        EQTRACE_guess((const char *)0x0057CCB0);
        object = EQ_GLOBAL(void *, 0x006ECE58);
        if (object != 0)
        {
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQ_GLOBAL(void *, 0x006ECE58) = 0;
        }

        EQTRACE_guess((const char *)0x0057CCA0);
        object = EQ_GLOBAL(void *, 0x006ECE50);
        if (object != 0)
        {
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQ_GLOBAL(void *, 0x006ECE50) = 0;
        }

        if (EQ_GLOBAL(void *, 0x006ECE4C) != 0)
        {
            EQTRACE_guess((const char *)0x0057CC94);
            object = EQ_GLOBAL(void *, 0x006ECE4C);
            ((VoiceRecognitionReleaseFn_guess_t)(*(void ***)object)[2])(object);
            EQTRACE_guess((const char *)0x0057CC88);
            EQ_GLOBAL(void *, 0x006ECE4C) = 0;
        }

        EQTRACE_guess((const char *)0x0057CC7C);
        EQ_GLOBAL(CoUninitializeFn_guess_t, 0x0053436C)();
        EQTRACE_guess((const char *)0x0057CC54);
        EQ_GLOBAL(int, 0x006ECE3C) = 0;
        EQ_GLOBAL(int, 0x006ECE40) = 0;
    }
}

// FUNCTION: TRILOGY 0x004e7be7
// Enables voice recognition listening when the system is initialized.
int StartVoiceRecognitionListening_guess(void)
{
    int hRes;
    void *attributes;

    if ((EQ_GLOBAL(int, 0x006ECE3C) != 0) &&
        (EQ_GLOBAL(int, 0x006ECE40) == 0))
    {
        attributes = EQ_GLOBAL(void *, 0x006ECE50);
        if (attributes == 0)
        {
            EQTRACE_guess((const char *)0x0057CD80);
            fdebug(
                (const char *)0x0057CD60);
        }
        else
        {
            hRes = ((int (__stdcall *)(void *, int))(*(void ***)attributes)[18])(
                attributes, 0x40);
            if (hRes >= 0)
            {
                EQTRACE_guess((const char *)0x0057CD38);
                EQ_GLOBAL(int, 0x006ECE40) = 1;
                return 1;
            }
            EQTRACE_guess((const char *)0x0057CD18, hRes);
            fdebug(
                (const char *)0x0057CD18, hRes);
            return 0;
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e7c5e
// Disables voice recognition listening when the system is active.
int StopVoiceRecognitionListening_guess(void)
{
    int hRes;
    void *attributes;

    if ((EQ_GLOBAL(int, 0x006ECE3C) != 0) &&
        (EQ_GLOBAL(int, 0x006ECE40) != 0))
    {
        attributes = EQ_GLOBAL(void *, 0x006ECE50);
        if (attributes == 0)
        {
            EQTRACE_guess(
                (const char *)0x0057CD80);
            fdebug(
                (const char *)0x0057CDC8);
        }
        else
        {
            hRes = ((int (__stdcall *)(void *, int))(*(void ***)attributes)[18])(
                attributes, 2);
            if (hRes >= 0)
            {
                EQTRACE_guess(
                    (const char *)0x0057CD94);
                EQ_GLOBAL(int, 0x006ECE40) = 0;
                return 1;
            }
            EQTRACE_guess(
                (const char *)0x0057CD18, hRes);
            fdebug(
                (const char *)0x0057CD18, hRes);
        }
    }
    return 0;
}

// FUNCTION: TRILOGY 0x004e7cf9
// Returns the current voice recognition listening state.
u32 IsVoiceRecognitionListening_guess(void)
{
    return EQ_GLOBAL(u32, 0x006ECE40);
}

typedef int (__stdcall *DictLockFn_guess)(void*);
typedef int (__stdcall *DictSetWordsFn_guess)(void*, u32);

// FUNCTION: TRILOGY 0x004e7d73
// Locks the voice dispatch object (0x006ECE54), sets its word list (vtable slot 0x4C), unlocks it.
void DictSetWords(u32 words)
{
    void *o1 = EQ_GLOBAL(void*, 0x006ECE54);

    if (((DictLockFn_guess)(*(void ***)o1)[3])(o1) == 0)
    {
        void *o2 = EQ_GLOBAL(void*, 0x006ECE54);
        int hr = ((DictSetWordsFn_guess)(*(void ***)o2)[19])(o2, words);

        if (hr != 0)
            EQTRACE_guess("Error:  Unable to set word list. hRes = %lx\n", hr);
        {
            void *o3 = EQ_GLOBAL(void*, 0x006ECE54);

            ((DictLockFn_guess)(*(void ***)o3)[4])(o3);
        }
        return;
    }
    EQTRACE_guess("Error: Lock failed in DictSetWords!\n");
}

// FUNCTION: TRILOGY 0x004e7dba
// Applies speech recognition edits, including spoken text, to the chat input.
int __stdcall TextChanged(void* unused)
{
    int result;
    size_t bufferLength;
    size_t chatLength;
    char* spokenText[2];
    u32 oldStart;
    int newEnd;
    u32 oldEnd;
    u32 newStart;
    typedef void (__stdcall *CoTaskMemFree_guess_t)(void*);

    if (EQ_GLOBAL(u32, 0x006ECE4C) == 0)
    {
        EQTRACE_guess((char*)0x0057CF18);
        return EQ_CDECL(int, 0x004E7D3B, (int, int, int))(0, 0x0FFFFFFF, 0);
    }
    if (EQ_GLOBAL(void*, 0x006EFE2C) == 0)
    {
        EQTRACE_guess((char*)0x0057CEF0);
        return EQ_CDECL(int, 0x004E7D3B, (int, int, int))(0, 0x0FFFFFFF, 0);
    }
    result = ((int (__stdcall *)(void*))(*(void***)EQ_GLOBAL(void*, 0x006ECE54))[3])
        (EQ_GLOBAL(void*, 0x006ECE54));
    if (result >= 0)
    {
        result = ((int (__stdcall *)(void*, u32*, int*, u32*, u32*))
            (*(void***)EQ_GLOBAL(void*, 0x006ECE54))[12])
            (EQ_GLOBAL(void*, 0x006ECE54), &newStart, &newEnd, &oldStart, &oldEnd);
        if (result >= 0)
        {
            spokenText[0] = (char*)0;
            if (newStart < oldEnd)
            {
                result = oldEnd - newStart;
                bufferLength = strlen((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
                EQTRACE_guess((char*)0x0057CEA0, oldStart, oldEnd, newStart, newEnd, bufferLength);
                bufferLength = strlen((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
                result = bufferLength - result;
                if (result >= 0 && (int)bufferLength > 0)
                {
                    *((char*)EQ_GLOBAL(void*, 0x006EFE2C) + result + 0xBA8) = 0;
                    EQ_METHOD(1, void, 0x004CF7A1, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(1000000);
                }
            }
            else
            {
                result = ((int (__stdcall *)(void*, u32, int, char**))
                    (*(void***)EQ_GLOBAL(void*, 0x006ECE54))[5])
                    (EQ_GLOBAL(void*, 0x006ECE54), newStart, newEnd - newStart, &spokenText[0]);
                if (result >= 0)
                {
                    if (EQ_GLOBAL(u8, 0x006B6404) == 0)
                    {
                        bufferLength = strlen(spokenText[0]);
                        chatLength = strlen((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8);
                        EQTRACE_guess((char*)0x0057CE8C, spokenText[0]);
                        if ((int)(chatLength + bufferLength) < 0x840)
                        {
                            SOC_FMT
                                ((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8,
                                 (char*)0x0055DAC8,
                                 (char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8,
                                 spokenText[0]);
                        }
                        else if (0 < (int)(0x83F - chatLength))
                        {
                            EQ_CDECL(char*, 0x005243C0, (char*, const char*, size_t))
                                ((char*)EQ_GLOBAL(void*, 0x006EFE2C) + 0xBA8,
                                 spokenText[0], 0x83F - chatLength);
                        }
                        EQ_METHOD(1, void, 0x004CF7A1, (void*, int), EQ_GLOBAL(void*, 0x006EFE2C))(1000000);
                    }
                }
                else
                {
                    EQTRACE_guess((char*)0x0057CE78);
                }
                if (spokenText[0] != (char*)0)
                {
                    EQ_GLOBAL(CoTaskMemFree_guess_t, 0x00534368)(spokenText[0]);
                }
            }
        }
        else
        {
            EQTRACE_guess((char*)0x0057CE60);
        }
        result = ((int (__stdcall *)(void*))(*(void***)EQ_GLOBAL(void*, 0x006ECE54))[4])
            (EQ_GLOBAL(void*, 0x006ECE54));
    }
    else
    {
        EQTRACE_guess((char*)0x0057CE40);
    }
    return result;
}

// FUNCTION: TRILOGY 0x004e7fad
// Sets the vtable (0x005357BC) and clears the second dword of a DirectSound callback object.
u32* __fastcall InitializeVoiceDictationNotifySink_guess(u32* obj)
{
    obj[1] &= 0;
    *obj = 0x005357BC;
    return obj;
}
