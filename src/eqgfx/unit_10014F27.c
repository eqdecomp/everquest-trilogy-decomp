// unit_10014F27.c - eqgfx_dx7.dll, the address run 0x10014F27..0x10017E72 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

typedef struct S3dResourceManager_guess
{
    unsigned char pad0[0x18];
    unsigned int resourcePoolWords[0xD8];
} S3dResourceManager_guess;

typedef struct S3dCleanupResource_guess
{
    unsigned char pad0[0x10];
    int objectAt10;
    unsigned char pad14[0x4];
    int *objectAt18;
    unsigned char pad1c[0x8];
    int *objectAt24;
    int *objectAt28;
    int *objectAt2c;
    unsigned char pad30[0x8];
    int *objectAt38;
    unsigned char pad3c[0x20];
    int *objectAt5c;
    unsigned char pad60[0x4];
    int *objectAt64;
    unsigned char pad68[0x4];
    int *objectAt6c;
    unsigned char pad70[0x18];
    int *objectAt88;
} S3dCleanupResource_guess;

typedef struct S3dCleanupOwner_guess
{
    unsigned char pad0[0x14];
    int *ownerAt14;
    unsigned char pad18[0x4];
    int *ownerAt1c;
    int *ownerAt20;
    int *ownerAt24;
    int *ownerAt28;
    int *ownerAt2c;
    int *ownerAt30;
} S3dCleanupOwner_guess;

#define S3D_RESOURCE_POOL_WORD(manager, byteOffset) \
    ((manager)->resourcePoolWords[((byteOffset) - 0x18) / 4])
#define S3D_CLEANUP_RESOURCE(address) ((S3dCleanupResource_guess *)(address))

typedef struct S3dMaterialDef_guess
{
    unsigned char pad0[0x14];
    int renderMethod;
    float constantIntensity;
    float ambientScalar;
    int twoSided;
    unsigned char pad24[0x8];
    unsigned char byteAt2c;
    unsigned char pad2d[0x3];
    float uShiftPerMs;
    float vShiftPerMs;
    float floatAt38;
    float floatAt3c;
    // +0x40: name text (variable length, see s3dCalcMaterialDefAllocSize)
    char nameText[1];
} S3dMaterialDef_guess;

typedef struct S3dMaterial_guess
{
    int code;
    unsigned char pad4[0x4];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad0D[0x3];
    int useCount;
    unsigned int paletteEntryCount;
    int *paletteEntries;
    int *tintEntries;
    int paletteType;
    void *paletteContext;
} S3dMaterial_guess;

typedef struct S3dMaterialPaletteDictionary_guess
{
    unsigned char pad0[0x8];
    void *paletteContext;
} S3dMaterialPaletteDictionary_guess;

typedef struct S3dRendererLimits_guess
{
    unsigned char pad0[0x3DC];
    unsigned int maxRenderMethod;
} S3dRendererLimits_guess;

typedef struct S3dObject_guess
{
    int type;
    char *tagAlt;
    char *tag;
    int flags;
} S3dObject_guess;

#define S3D_MATERIAL_FLAG_IN_DICT 0x01
#define S3D_MATERIAL_FLAG_KEEP_MEMORY 0x20
// +0x24 is missing from S3dMaterial_guess (the material's sprite pointer; its first dword gates the unshare).
#define S3D_MATERIAL_SPRITE(material) (*(int **)((unsigned char *)(material) + 0x24))
#define S3D_MATERIAL_DEF(material) ((S3dMaterialDef_guess *)(material))

/* S3dResourcePool_guess: now in eqgfx/layout/t3dworld.h. */
#include "eqgfx/layout/t3dworld.h"

/* T3D_tagWORLD: the world layout is T3D_tagWORLD (generated: eqgfx/layout/t3dworld.h); the old local view's members map by offset. */

typedef struct S3dResourceListEntry_guess
{
    int type;
    unsigned int nextFree;
} S3dResourceListEntry_guess;

typedef void (__stdcall *S3dDebugBreak_guess_t)(void);

typedef struct S3dPaletteEntry_guess
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char flags;
} S3dPaletteEntry_guess;

typedef struct S3dPalette_guess
{
    unsigned int typeId;
    int dwordAt04;
    unsigned int dwordAt08;
    unsigned int dwordAt0C;
    unsigned short wordAt10;
    unsigned short colorCount;
    S3dPaletteEntry_guess entries[256];
    int allColors;
    int dwordAt418;
    int dwordAt41C;
    int dwordAt420;
    unsigned char pad424[0x8];
} S3dPalette_guess;

typedef void *(__stdcall *S3dGetDC_guess_t)(void *window);
typedef unsigned int (__stdcall *S3dGetSystemPaletteEntries_guess_t)(void *dc, unsigned int startIndex, unsigned int entryCount, void *entries);
typedef int (__stdcall *S3dReleaseDC_guess_t)(void *window, void *dc);

#define EQGFX_LOG_DEBUG_MESSAGE EQ_CDECL(void, 0x100509EE, (const char *))
#define EQGFX_REALLOC EQ_CDECL(unsigned int *, 0x10050C8C, (unsigned int *, unsigned int))
#define EQGFX_DEBUG_BREAK EQ_GLOBAL(S3dDebugBreak_guess_t, 0x100760BC)
#define S3D_RESOURCE_POOL_ERROR 0xFFFF3D3E
#define S3D_POOL_TYPE_BMINFO 0x1B
#define S3D_SET_RESOURCE_POOL(poolPointer) \
    itemCount = &(poolPointer)->itemCount; \
    capacity = &(poolPointer)->capacity; \
    freeHead = &(poolPointer)->freeHead; \
    freeEntries = &(poolPointer)->freeEntries

typedef struct S3dResourcePoolInitTable_guess
{
    unsigned int fields[0xDE];
} S3dResourcePoolInitTable_guess;

#define SET_RESOURCE_POOL_INIT_FIELDS(firstFreeIndex, countIndex, entriesIndex, headIndex) \
    firstFreeEntryField = &((S3dResourcePoolInitTable_guess *)poolFields)->fields[firstFreeIndex]; \
    entryCountField = &((S3dResourcePoolInitTable_guess *)poolFields)->fields[countIndex]; \
    entryArrayField = &((S3dResourcePoolInitTable_guess *)poolFields)->fields[entriesIndex]; \
    poolFields = &((S3dResourcePoolInitTable_guess *)poolFields)->fields[headIndex]

#define S3D_POOL_INIT_ALLOCATION_FAILED 3
#define S3D_POOL_INIT_ALREADY_INITIALIZED 5

// FUNCTION: EQGFX 0x10014f27
// Sets up the empty slot table of one object type in a world (t3dCreateWorld: 20 slots per definition type)
int __cdecl t3dInitObjectPool_guess(unsigned int *poolFields, int poolType, unsigned int entryCount)
{
    unsigned int *allocatedEntries;
    unsigned int nextEntry;
    unsigned int *firstFreeEntryField;
    unsigned int *entryCountField;
    unsigned int *entryArrayField;
    int result;

    result = 0;
    if (poolType < 0x1B)
    {
        if (poolType == 0x1A)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0x60, 0x61, 0x63, 0x62);
        }
        else
        {
            switch (poolType)
            {
            case 3:
                SET_RESOURCE_POOL_INIT_FIELDS(0x14, 0x15, 0x17, 0x16);
                break;
            case 4:
                SET_RESOURCE_POOL_INIT_FIELDS(0x3C, 0x3D, 0x3F, 0x3E);
                break;
            case 1:
                SET_RESOURCE_POOL_INIT_FIELDS(0x18, 0x19, 0x1B, 0x1A);
                break;
            case 2:
                SET_RESOURCE_POOL_INIT_FIELDS(0x40, 0x41, 0x43, 0x42);
                break;
            case 5:
                SET_RESOURCE_POOL_INIT_FIELDS(0x1C, 0x1D, 0x1F, 0x1E);
                break;
            case 6:
                SET_RESOURCE_POOL_INIT_FIELDS(0x44, 0x45, 0x47, 0x46);
                break;
            case 7:
                SET_RESOURCE_POOL_INIT_FIELDS(0x20, 0x21, 0x23, 0x22);
                break;
            case 8:
                SET_RESOURCE_POOL_INIT_FIELDS(0x48, 0x49, 0x4B, 0x4A);
                break;
            case 9:
                SET_RESOURCE_POOL_INIT_FIELDS(0x24, 0x25, 0x27, 0x26);
                break;
            case 10:
                SET_RESOURCE_POOL_INIT_FIELDS(0x4C, 0x4D, 0x4F, 0x4E);
                break;
            case 11:
                SET_RESOURCE_POOL_INIT_FIELDS(0x28, 0x29, 0x2B, 0x2A);
                break;
            case 12:
                SET_RESOURCE_POOL_INIT_FIELDS(0x50, 0x51, 0x53, 0x52);
                break;
            case 17:
                SET_RESOURCE_POOL_INIT_FIELDS(0x2C, 0x2D, 0x2F, 0x2E);
                break;
            case 18:
                SET_RESOURCE_POOL_INIT_FIELDS(0x54, 0x55, 0x57, 0x56);
                break;
            case 19:
                SET_RESOURCE_POOL_INIT_FIELDS(0x30, 0x31, 0x33, 0x32);
                break;
            case 20:
                SET_RESOURCE_POOL_INIT_FIELDS(0x58, 0x59, 0x5B, 0x5A);
                break;
            case 21:
                SET_RESOURCE_POOL_INIT_FIELDS(0x84, 0x85, 0x87, 0x86);
                break;
            case 22:
                SET_RESOURCE_POOL_INIT_FIELDS(0x88, 0x89, 0x8B, 0x8A);
                break;
            case 23:
                SET_RESOURCE_POOL_INIT_FIELDS(0x34, 0x35, 0x37, 0x36);
                break;
            case 24:
                SET_RESOURCE_POOL_INIT_FIELDS(0x5C, 0x5D, 0x5F, 0x5E);
                break;
            case 25:
                SET_RESOURCE_POOL_INIT_FIELDS(6, 7, 9, 8);
                break;
            default:
                entryCountField = poolFields;
                entryArrayField = poolFields;
                firstFreeEntryField = poolFields;
                break;
            }
        }
    }
    else if (poolType < 0x48)
    {
        if (poolType == 0x47)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0x9C, 0x9D, 0x9F, 0x9E);
        }
        else if (poolType < 0x24)
        {
            if (poolType == 0x23)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(0xBC, 0xBD, 0xBF, 0xBE);
            }
            else if (poolType == 0x1B)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(100, 0x65, 0x67, 0x66);
            }
            else if (poolType == 0x1C)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(0x68, 0x69, 0x6B, 0x6A);
            }
            else if (poolType == 0x1D)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(0x6C, 0x6D, 0x6F, 0x6E);
            }
            else if (poolType == 0x1E)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(0x70, 0x71, 0x73, 0x72);
            }
            else if (poolType == 0x1F)
            {
                SET_RESOURCE_POOL_INIT_FIELDS(0x74, 0x75, 0x77, 0x76);
            }
            else
            {
                entryCountField = poolFields;
                entryArrayField = poolFields;
                firstFreeEntryField = poolFields;
            }
        }
        else if (poolType == 0x24)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0xC0, 0xC1, 0xC3, 0xC2);
        }
        else if (poolType == 0x44)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0x90, 0x91, 0x93, 0x92);
        }
        else if (poolType == 0x45)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0x94, 0x95, 0x97, 0x96);
        }
        else if (poolType == 0x46)
        {
            SET_RESOURCE_POOL_INIT_FIELDS(0x98, 0x99, 0x9B, 0x9A);
        }
        else
        {
            entryCountField = poolFields;
            entryArrayField = poolFields;
            firstFreeEntryField = poolFields;
        }
    }
    else
    {
        switch (poolType)
        {
        case 0x4C:
            SET_RESOURCE_POOL_INIT_FIELDS(0x38, 0x39, 0x3B, 0x3A);
            break;
        case 0x4D:
            SET_RESOURCE_POOL_INIT_FIELDS(0x8C, 0x8D, 0x8F, 0x8E);
            break;
        case 0x48:
            SET_RESOURCE_POOL_INIT_FIELDS(0xA0, 0xA1, 0xA3, 0xA2);
            break;
        case 0x49:
            SET_RESOURCE_POOL_INIT_FIELDS(0xA4, 0xA5, 0xA7, 0xA6);
            break;
        case 0x4A:
            SET_RESOURCE_POOL_INIT_FIELDS(0xA8, 0xA9, 0xAB, 0xAA);
            break;
        case 0x4E:
            SET_RESOURCE_POOL_INIT_FIELDS(0xAC, 0xAD, 0xAF, 0xAE);
            break;
        case 0x4F:
            SET_RESOURCE_POOL_INIT_FIELDS(0xB0, 0xB1, 0xB3, 0xB2);
            break;
        case 0x50:
            SET_RESOURCE_POOL_INIT_FIELDS(0xB4, 0xB5, 0xB7, 0xB6);
            break;
        case 0x51:
            SET_RESOURCE_POOL_INIT_FIELDS(0xB8, 0xB9, 0xBB, 0xBA);
            break;
        case 0x53:
            SET_RESOURCE_POOL_INIT_FIELDS(0xDA, 0xDB, 0xDD, 0xDC);
            break;
        default:
            entryCountField = poolFields;
            entryArrayField = poolFields;
            firstFreeEntryField = poolFields;
            break;
        }
    }

    if (*entryArrayField == 0)
    {
        allocatedEntries = EQ_CDECL(unsigned int *, 0x10050C48, (unsigned int))(entryCount << 2);
        *entryArrayField = (unsigned int)allocatedEntries;
        if (allocatedEntries == 0)
        {
            result = S3D_POOL_INIT_ALLOCATION_FAILED;
        }
        else
        {
            nextEntry = 0;
            if (entryCount != 0)
            {
                do
                {
                    nextEntry = nextEntry + 1;
                    *allocatedEntries = nextEntry;
                    allocatedEntries = allocatedEntries + 1;
                } while (nextEntry < entryCount);
            }
            *entryCountField = entryCount;
            *firstFreeEntryField = 0;
            *poolFields = 0;
        }
    }
    else
    {
        result = S3D_POOL_INIT_ALREADY_INITIALIZED;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001564b
// Registers a rendering resource in the manager's list for its resource type.
// @early-stop: refill loop's inc sits after the bound test, the original has it before
int __cdecl _t3dAddItemToList_guess(T3D_tagWORLD *poolManager, S3dResourceListEntry_guess *entry)
{
    unsigned int newCapacity;
    unsigned int nextFreeIndex;
    unsigned int *freeEntry;
    unsigned int *entries;
    unsigned int *freeHead;
    unsigned int *capacity;
    unsigned int **freeEntries;
    int *itemCount;
    int objectType;

    objectType = entry->type;
    switch (objectType)
    {
    case 3: S3D_SET_RESOURCE_POOL(&poolManager->type3Pool); break;
    case 4: S3D_SET_RESOURCE_POOL(&poolManager->pools[9]); break;
    case 1: S3D_SET_RESOURCE_POOL(&poolManager->pools[0]); break;
    case 2: S3D_SET_RESOURCE_POOL(&poolManager->pools[10]); break;
    case 5: S3D_SET_RESOURCE_POOL(&poolManager->pools[1]); break;
    case 6: S3D_SET_RESOURCE_POOL(&poolManager->pools[11]); break;
    case 7: S3D_SET_RESOURCE_POOL(&poolManager->pools[2]); break;
    case 8: S3D_SET_RESOURCE_POOL(&poolManager->pools[12]); break;
    case 9: S3D_SET_RESOURCE_POOL(&poolManager->pools[3]); break;
    case 10: S3D_SET_RESOURCE_POOL(&poolManager->pools[13]); break;
    case 0x0B: S3D_SET_RESOURCE_POOL(&poolManager->pools[4]); break;
    case 0x0C: S3D_SET_RESOURCE_POOL(&poolManager->pools[14]); break;
    case 0x11: S3D_SET_RESOURCE_POOL(&poolManager->pools[5]); break;
    case 0x12: S3D_SET_RESOURCE_POOL(&poolManager->pools[15]); break;
    case 0x13: S3D_SET_RESOURCE_POOL(&poolManager->pools[6]); break;
    case 0x14: S3D_SET_RESOURCE_POOL(&poolManager->pools[16]); break;
    case 0x15: S3D_SET_RESOURCE_POOL(&poolManager->pools[27]); break;
    case 0x16: S3D_SET_RESOURCE_POOL(&poolManager->pools[28]); break;
    case 0x17: S3D_SET_RESOURCE_POOL(&poolManager->pools[7]); break;
    case 0x18: S3D_SET_RESOURCE_POOL(&poolManager->pools[17]); break;
    case 0x19: S3D_SET_RESOURCE_POOL(&poolManager->regionPool); break;
    case 0x1A: S3D_SET_RESOURCE_POOL(&poolManager->pools[18]); break;
    case S3D_POOL_TYPE_BMINFO: S3D_SET_RESOURCE_POOL(&poolManager->pools[19]); break;
    case 0x1C: S3D_SET_RESOURCE_POOL(&poolManager->pools[20]); break;
    case 0x1D: S3D_SET_RESOURCE_POOL(&poolManager->pools[21]); break;
    case 0x1E: S3D_SET_RESOURCE_POOL(&poolManager->pools[22]); break;
    case 0x1F: S3D_SET_RESOURCE_POOL(&poolManager->pools[23]); break;
    case 0x23: S3D_SET_RESOURCE_POOL(&poolManager->pools[41]); break;
    case 0x24: S3D_SET_RESOURCE_POOL(&poolManager->pools[42]); break;
    case 0x44: S3D_SET_RESOURCE_POOL(&poolManager->pools[30]); break;
    case 0x45: S3D_SET_RESOURCE_POOL(&poolManager->pools[31]); break;
    case 0x46: S3D_SET_RESOURCE_POOL(&poolManager->pools[32]); break;
    case 0x47: S3D_SET_RESOURCE_POOL(&poolManager->pools[33]); break;
    case 0x4C: S3D_SET_RESOURCE_POOL(&poolManager->pools[8]); break;
    case 0x4D: S3D_SET_RESOURCE_POOL(&poolManager->pools[29]); break;
    case 0x48: S3D_SET_RESOURCE_POOL(&poolManager->pools[34]); break;
    case 0x49: S3D_SET_RESOURCE_POOL(&poolManager->pools[35]); break;
    case 0x4A: S3D_SET_RESOURCE_POOL(&poolManager->pools[36]); break;
    case 0x4E: S3D_SET_RESOURCE_POOL(&poolManager->pools[37]); break;
    case 0x4F: S3D_SET_RESOURCE_POOL(&poolManager->pools[38]); break;
    case 0x50: S3D_SET_RESOURCE_POOL(&poolManager->pools[39]); break;
    case 0x51: S3D_SET_RESOURCE_POOL(&poolManager->pools[40]); break;
    case 0x53: S3D_SET_RESOURCE_POOL(&poolManager->type53Pool); break;
    default: return S3D_RESOURCE_POOL_ERROR;
    }

    if (*freeHead == *capacity)
    {
        if (objectType == S3D_POOL_TYPE_BMINFO)
        {
            EQGFX_LOG_DEBUG_MESSAGE(EQ_ARRAY(char, 0x10086CB4));
            EQGFX_DEBUG_BREAK();
        }
        *capacity += *capacity >> 2;
        entries = EQGFX_REALLOC(*freeEntries, *capacity * 4);
        *freeEntries = entries;
        newCapacity = *capacity;
        nextFreeIndex = *freeHead;
        for (freeEntry = entries + nextFreeIndex; freeEntry < entries + newCapacity; freeEntry++)
        {
            *freeEntry = ++nextFreeIndex;
        }
    }
    entries = *freeEntries;
    nextFreeIndex = entries[*freeHead];
    entry->nextFree = *freeHead;
    entries[*freeHead] = (unsigned int)entry;
    *freeHead = nextFreeIndex;
    *itemCount = *itemCount + 1;
    return 0;
}
#undef S3D_SET_RESOURCE_POOL

// FUNCTION: EQGFX 0x10015d81
// Frees an object's slot in its type's world pool; with flags & 6 it is also removed from the list at +0x330
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Frees one
// renderer slot: type at +0x0 picks the pool triple, index at +0x4 is pushed on its
// free list. Returns 0 on success, 0xFFFF3D3E otherwise.
int __cdecl t3dFreeObjectPoolSlot_guess(int poolBase, int *slotDesc)
{
    int objectType = *slotDesc;
    int dataBase;
    int *countPtr;
    int *freeHeadPtr;
    int slotIndex;
    int *trackedFirst;
    int *trackedLast;

    switch (objectType)
    {
    case 1:
        dataBase = *(int *)(poolBase + 0x6c);
        countPtr = (int *)(poolBase + 0x60);
        freeHeadPtr = (int *)(poolBase + 0x68);
        break;
    case 2:
        dataBase = *(int *)(poolBase + 0x10c);
        countPtr = (int *)(poolBase + 0x100);
        freeHeadPtr = (int *)(poolBase + 0x108);
        break;
    case 3:
        dataBase = *(int *)(poolBase + 0x5c);
        countPtr = (int *)(poolBase + 0x50);
        freeHeadPtr = (int *)(poolBase + 0x58);
        break;
    case 4:
        dataBase = *(int *)(poolBase + 0xfc);
        countPtr = (int *)(poolBase + 0xf0);
        freeHeadPtr = (int *)(poolBase + 0xf8);
        break;
    case 5:
        dataBase = *(int *)(poolBase + 0x7c);
        countPtr = (int *)(poolBase + 0x70);
        freeHeadPtr = (int *)(poolBase + 0x78);
        break;
    case 6:
        dataBase = *(int *)(poolBase + 0x11c);
        countPtr = (int *)(poolBase + 0x110);
        freeHeadPtr = (int *)(poolBase + 0x118);
        break;
    case 7:
        dataBase = *(int *)(poolBase + 0x8c);
        countPtr = (int *)(poolBase + 0x80);
        freeHeadPtr = (int *)(poolBase + 0x88);
        break;
    case 8:
        dataBase = *(int *)(poolBase + 0x12c);
        countPtr = (int *)(poolBase + 0x120);
        freeHeadPtr = (int *)(poolBase + 0x128);
        break;
    case 9:
        dataBase = *(int *)(poolBase + 0x9c);
        countPtr = (int *)(poolBase + 0x90);
        freeHeadPtr = (int *)(poolBase + 0x98);
        break;
    case 10:
        dataBase = *(int *)(poolBase + 0x13c);
        countPtr = (int *)(poolBase + 0x130);
        freeHeadPtr = (int *)(poolBase + 0x138);
        break;
    case 11:
        dataBase = *(int *)(poolBase + 0xac);
        countPtr = (int *)(poolBase + 0xa0);
        freeHeadPtr = (int *)(poolBase + 0xa8);
        break;
    case 12:
        dataBase = *(int *)(poolBase + 0x14c);
        countPtr = (int *)(poolBase + 0x140);
        freeHeadPtr = (int *)(poolBase + 0x148);
        break;
    case 0x11:
        dataBase = *(int *)(poolBase + 0xbc);
        countPtr = (int *)(poolBase + 0xb0);
        freeHeadPtr = (int *)(poolBase + 0xb8);
        break;
    case 0x12:
        dataBase = *(int *)(poolBase + 0x15c);
        countPtr = (int *)(poolBase + 0x150);
        freeHeadPtr = (int *)(poolBase + 0x158);
        break;
    case 0x13:
        dataBase = *(int *)(poolBase + 0xcc);
        countPtr = (int *)(poolBase + 0xc0);
        freeHeadPtr = (int *)(poolBase + 0xc8);
        break;
    case 0x14:
        dataBase = *(int *)(poolBase + 0x16c);
        countPtr = (int *)(poolBase + 0x160);
        freeHeadPtr = (int *)(poolBase + 0x168);
        break;
    case 0x15:
        dataBase = *(int *)(poolBase + 0x21c);
        countPtr = (int *)(poolBase + 0x210);
        freeHeadPtr = (int *)(poolBase + 0x218);
        break;
    case 0x16:
        dataBase = *(int *)(poolBase + 0x22c);
        countPtr = (int *)(poolBase + 0x220);
        freeHeadPtr = (int *)(poolBase + 0x228);
        break;
    case 0x17:
        dataBase = *(int *)(poolBase + 0xdc);
        countPtr = (int *)(poolBase + 0xd0);
        freeHeadPtr = (int *)(poolBase + 0xd8);
        break;
    case 0x18:
        dataBase = *(int *)(poolBase + 0x17c);
        countPtr = (int *)(poolBase + 0x170);
        freeHeadPtr = (int *)(poolBase + 0x178);
        break;
    case 0x19:
        dataBase = *(int *)(poolBase + 0x24);
        countPtr = (int *)(poolBase + 0x18);
        freeHeadPtr = (int *)(poolBase + 0x20);
        break;
    case 0x1a:
        dataBase = *(int *)(poolBase + 0x18c);
        countPtr = (int *)(poolBase + 0x180);
        freeHeadPtr = (int *)(poolBase + 0x188);
        break;
    case 0x1b:
        dataBase = *(int *)(poolBase + 0x19c);
        countPtr = (int *)(poolBase + 0x190);
        freeHeadPtr = (int *)(poolBase + 0x198);
        break;
    case 0x1c:
        dataBase = *(int *)(poolBase + 0x1ac);
        countPtr = (int *)(poolBase + 0x1a0);
        freeHeadPtr = (int *)(poolBase + 0x1a8);
        break;
    case 0x1d:
        dataBase = *(int *)(poolBase + 0x1bc);
        countPtr = (int *)(poolBase + 0x1b0);
        freeHeadPtr = (int *)(poolBase + 0x1b8);
        break;
    case 0x1e:
        dataBase = *(int *)(poolBase + 0x1cc);
        countPtr = (int *)(poolBase + 0x1c0);
        freeHeadPtr = (int *)(poolBase + 0x1c8);
        break;
    case 0x1f:
        dataBase = *(int *)(poolBase + 0x1dc);
        countPtr = (int *)(poolBase + 0x1d0);
        freeHeadPtr = (int *)(poolBase + 0x1d8);
        break;
    case 0x23:
        dataBase = *(int *)(poolBase + 0x2fc);
        countPtr = (int *)(poolBase + 0x2f0);
        freeHeadPtr = (int *)(poolBase + 0x2f8);
        break;
    case 0x24:
        dataBase = *(int *)(poolBase + 0x30c);
        countPtr = (int *)(poolBase + 0x300);
        freeHeadPtr = (int *)(poolBase + 0x308);
        break;
    case 0x44:
        dataBase = *(int *)(poolBase + 0x24c);
        countPtr = (int *)(poolBase + 0x240);
        freeHeadPtr = (int *)(poolBase + 0x248);
        break;
    case 0x45:
        dataBase = *(int *)(poolBase + 0x25c);
        countPtr = (int *)(poolBase + 0x250);
        freeHeadPtr = (int *)(poolBase + 0x258);
        break;
    case 0x46:
        dataBase = *(int *)(poolBase + 0x26c);
        countPtr = (int *)(poolBase + 0x260);
        freeHeadPtr = (int *)(poolBase + 0x268);
        break;
    case 0x47:
        dataBase = *(int *)(poolBase + 0x27c);
        countPtr = (int *)(poolBase + 0x270);
        freeHeadPtr = (int *)(poolBase + 0x278);
        break;
    case 0x48:
        dataBase = *(int *)(poolBase + 0x28c);
        countPtr = (int *)(poolBase + 0x280);
        freeHeadPtr = (int *)(poolBase + 0x288);
        break;
    case 0x49:
        dataBase = *(int *)(poolBase + 0x29c);
        countPtr = (int *)(poolBase + 0x290);
        freeHeadPtr = (int *)(poolBase + 0x298);
        break;
    case 0x4a:
        dataBase = *(int *)(poolBase + 0x2ac);
        countPtr = (int *)(poolBase + 0x2a0);
        freeHeadPtr = (int *)(poolBase + 0x2a8);
        break;
    case 0x4c:
        dataBase = *(int *)(poolBase + 0xec);
        countPtr = (int *)(poolBase + 0xe0);
        freeHeadPtr = (int *)(poolBase + 0xe8);
        break;
    case 0x4d:
        dataBase = *(int *)(poolBase + 0x23c);
        countPtr = (int *)(poolBase + 0x230);
        freeHeadPtr = (int *)(poolBase + 0x238);
        break;
    case 0x4e:
        dataBase = *(int *)(poolBase + 0x2bc);
        countPtr = (int *)(poolBase + 0x2b0);
        freeHeadPtr = (int *)(poolBase + 0x2b8);
        break;
    case 0x4f:
        dataBase = *(int *)(poolBase + 0x2cc);
        countPtr = (int *)(poolBase + 0x2c0);
        freeHeadPtr = (int *)(poolBase + 0x2c8);
        break;
    case 0x50:
        dataBase = *(int *)(poolBase + 0x2dc);
        countPtr = (int *)(poolBase + 0x2d0);
        freeHeadPtr = (int *)(poolBase + 0x2d8);
        break;
    case 0x51:
        dataBase = *(int *)(poolBase + 0x2ec);
        countPtr = (int *)(poolBase + 0x2e0);
        freeHeadPtr = (int *)(poolBase + 0x2e8);
        break;
    case 0x53:
        dataBase = *(int *)(poolBase + 0x374);
        countPtr = (int *)(poolBase + 0x368);
        freeHeadPtr = (int *)(poolBase + 0x370);
        break;
    default:
        return 0xFFFF3D3E;
    }

    slotIndex = slotDesc[1];
    if ((slotDesc[3] & 6) != 0)
    {
        trackedFirst = *(int **)(poolBase + 0x330);
        if (trackedFirst != 0)
        {
            trackedLast = trackedFirst + *(int *)(poolBase + 0x328) - 1;
            for (; trackedFirst <= trackedLast; trackedFirst++)
            {
                if (*trackedFirst == (int)slotDesc)
                {
                    *trackedFirst = *trackedLast;
                    (*(int *)(poolBase + 0x328))--;
                    break;
                }
            }
        }
    }
    *(int *)(dataBase + slotIndex * 4) = *freeHeadPtr;
    *freeHeadPtr = slotIndex;
    (*countPtr)--;
    return 0;
}

/* EQGFX pool cleanup callees are addressed in eqgfx_dx7.dll's own image. */
#define EQGFX_UNDEFINE_SPHERE_LIST EQ_CDECL(int, 0x1001AF77, (int, int))
#define EQGFX_DESTROY_POLYHEDRON EQ_CDECL(int, 0x10018DA3, (int, int))
#define EQGFX_UNDEFINE_POLYHEDRON EQ_CDECL(int, 0x10018BFD, (int, int))
#define EQGFX_UNDEFINE_SIMPLE_SPRITE EQ_CDECL(int, 0x1001B72E, (int, int))
#define EQGFX_DESTROY_SIMPLE_SPRITE EQ_CDECL(int, 0x1001B94A, (int, int))
#define EQGFX_UNDEFINE_BLIT_SPRITE EQ_CDECL(int, 0x1000CF2A, (int, int))
#define EQGFX_DESTROY_BLIT_SPRITE EQ_CDECL(int, 0x1000D06A, (int, int))
#define EQGFX_UNDEFINE_2D_SPRITE EQ_CDECL(int, 0x10007A5B, (int, int))
#define EQGFX_DESTROY_2D_SPRITE EQ_CDECL(int, 0x10007D99, (int, int))
#define EQGFX_UNDEFINE_3D_SPRITE EQ_CDECL(int, 0x10008262, (int, int))
#define EQGFX_DESTROY_3D_SPRITE EQ_CDECL(int, 0x1000879D, (int, int))
#define EQGFX_UNDEFINE_COMPOSITE_SPRITE EQ_CDECL(int, 0x1000D3E2, (int, int))
#define EQGFX_DESTROY_COMPOSITE_SPRITE EQ_CDECL(int, 0x1000D6E5, (int, int))
#define EQGFX_UNDEFINE_HIERARCHICAL_SPRITE EQ_CDECL(int, 0x100128BB, (int, int))
#define EQGFX_DESTROY_HIERARCHICAL_SPRITE EQ_CDECL(int, 0x100132A2, (int, int))
#define EQGFX_UNDEFINE_TRACK EQ_CDECL(int, 0x1001CB62, (int, int))
#define EQGFX_GET_DMRGB_DEFORMATIONS EQ_CDECL(int, 0x1000ACD6, (int))
#define EQGFX_GET_TRACK_DEFAULT_INSTANCE EQ_CDECL(int, 0x1001D0BA, (int))
#define EQGFX_DESTROY_TRACK EQ_CDECL(int, 0x1001CD7C, (int, int))
#define EQGFX_UNDEFINE_ACTOR EQ_CDECL(int, 0x100097F5, (int, int))
#define EQGFX_DESTROY_ACTOR EQ_CDECL(int, 0x1000A25C, (int, int))
#define EQGFX_CLEANUP_WORLD_OBJECT EQ_CDECL(int, 0x100191D8, (int, int))
#define EQGFX_UNDEFINE_AI EQ_CDECL(int, 0x1000B50E, (int, int))
#define EQGFX_UNDEFINE_BMINFO EQ_CDECL(int, 0x1000BCF0, (int, int))
#define EQGFX_UNDEFINE_LIGHT EQ_CDECL(int, 0x10013F01, (int, int))
#define EQGFX_DESTROY_LIGHT EQ_CDECL(int, 0x100141A6, (int, int))
#define EQGFX_DESTROY_POINT_LIGHT EQ_CDECL(int, 0x100144DC, (int, int))
#define EQGFX_DESTROY_DIRECTIONAL_LIGHT EQ_CDECL(int, 0x10014C1B, (int, int))
#define EQGFX_DESTROY_PALETTE EQ_CDECL(int, 0x100180EB, (int, int))
#define EQGFX_UNDEFINE_PARTICLE_CLOUD EQ_CDECL(int, 0x1003FF76, (int, int))
#define EQGFX_DESTROY_PARTICLE_CLOUD EQ_CDECL(int, 0x10040217, (int, int))
#define EQGFX_DESTROY_ZONE EQ_CDECL(int, 0x100205AE, (int, int))
#define EQGFX_UNDEFINE_MATERIAL EQ_CDECL(int, 0x10016DCB, (int, int))
#define EQGFX_DESTROY_MATERIAL_PALETTE EQ_CDECL(int, 0x10017135, (int, int))
#define EQGFX_UNDEFINE_DM_SPRITE EQ_CDECL(int, 0x1000F990, (int, int))
#define EQGFX_DESTROY_DM_SPRITE EQ_CDECL(int, 0x10010575, (int, int))
#define EQGFX_UNDEFINE_DM_TRACK EQ_CDECL(int, 0x10010A1F, (int, int))
#define EQGFX_DESTROY_DM_RGB_TRACK EQ_CDECL(int, 0x10010EED, (int, int))
#define EQGFX_UNDEFINE_DM_RGB_TRACK EQ_CDECL(int, 0x10010D88, (int, int))
#define EQGFX_DESTROY_FONT_TEXTURE EQ_CDECL(int, 0x1001AA66, (int, int))
#define EQGFX_DESTROY_STRING_SPRITE EQ_CDECL(int, 0x1001ACF8, (int, int))
#define EQGFX_DESTROY_ALLOCATION_POOL EQ_CDECL(int, 0x10019A01, (int, int))
#define EQGFX_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (int))

// FUNCTION: EQGFX 0x10016319
// World teardown: destroys every object left in one type's table, then frees the table once no slot is in use
int __cdecl _t3dDestroyItemList_guess(int *poolManagerAddress, int resourceType)
{
    S3dResourceManager_guess *poolManager;
    int managerAddress;
    int *resourceArray;
    int *resourceSlot;
    unsigned int *allocationField;
    unsigned int *resourceCount;
    unsigned int *freeListHead;
    unsigned int *entriesField;
    unsigned int resourceIndex;
    unsigned int nextFreeIndex;
    unsigned int nextIndex;
    unsigned int deformationCount;
    int currentResource;
    int result;

    poolManager = (S3dResourceManager_guess *)poolManagerAddress;
    managerAddress = (int)poolManagerAddress;
    result = 0;
    if (resourceType <= 0x1a)
    {
        if (resourceType != 0x1a)
        {
        switch (resourceType)
        {
        case 0x1:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x60);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x64);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x68);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x6c);
            break;
        case 0x2:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x100);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x104);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x108);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x10c);
            break;
        case 0x3:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x50);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x54);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x58);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x5c);
            break;
        case 0x4:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xf0);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xf4);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xf8);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xfc);
            break;
        case 0x5:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x70);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x74);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x78);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x7c);
            break;
        case 0x6:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x110);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x114);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x118);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x11c);
            break;
        case 0x7:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x80);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x84);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x88);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x8c);
            break;
        case 0x8:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x120);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x124);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x128);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x12c);
            break;
        case 0x9:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x90);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x94);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x98);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x9c);
            break;
        case 0xa:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x130);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x134);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x138);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x13c);
            break;
        case 0xb:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xa0);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xa4);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xa8);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xac);
            break;
        case 0xc:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x140);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x144);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x148);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x14c);
            break;
        case 0x11:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xb0);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xb4);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xb8);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xbc);
            break;
        case 0x12:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x150);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x154);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x158);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x15c);
            break;
        case 0x13:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xc0);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xc4);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xc8);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xcc);
            break;
        case 0x14:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x160);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x164);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x168);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x16c);
            break;
        case 0x15:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x210);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x214);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x218);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x21c);
            break;
        case 0x16:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x220);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x224);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x228);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x22c);
            break;
        case 0x17:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xd0);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xd4);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xd8);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xdc);
            break;
        case 0x18:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x170);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x174);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x178);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x17c);
            break;
        case 0x19:
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x18);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1c);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x20);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x24);
            break;
        default:
            goto unsupported_resource_type;
        }
        goto selected_resource_pool;
        }
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x180);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x184);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x188);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x18c);
        goto selected_resource_pool;
    }
    if (resourceType < 0x48)
    {
        if (resourceType == 0x47)
        {
            allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x270);
            resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x274);
            freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x278);
            entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x27c);
            goto selected_resource_pool;
        }
        if (resourceType < 0x24)
        {
            if (resourceType == 0x23)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2f0);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2f4);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2f8);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2fc);
                goto selected_resource_pool;
            }
            if (resourceType == 0x1b)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 400);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x194);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x198);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x19c);
                goto selected_resource_pool;
            }
            if (resourceType == 0x1c)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1a0);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1a4);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1a8);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1ac);
                goto selected_resource_pool;
            }
            if (resourceType == 0x1d)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1b0);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1b4);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1b8);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1bc);
                goto selected_resource_pool;
            }
            if (resourceType == 0x1e)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1c0);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1c4);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1c8);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1cc);
                goto selected_resource_pool;
            }
            if (resourceType == 0x1f)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1d0);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1d4);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1d8);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x1dc);
                goto selected_resource_pool;
            }
        }
        else
        {
            if (resourceType == 0x24)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x300);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x304);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x308);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x30c);
                goto selected_resource_pool;
            }
            if (resourceType == 0x44)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x240);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x244);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x248);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x24c);
                goto selected_resource_pool;
            }
            if (resourceType == 0x45)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x250);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x254);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 600);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x25c);
                goto selected_resource_pool;
            }
            if (resourceType == 0x46)
            {
                allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x260);
                resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x264);
                freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x268);
                entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x26c);
                goto selected_resource_pool;
            }
        }
        goto unsupported_resource_type;
    }
    switch (resourceType)
    {
    case 0x48:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x280);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x284);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x288);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x28c);
        break;
    case 0x49:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x290);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x294);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x298);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x29c);
        break;
    case 0x4a:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2a0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2a4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2a8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2ac);
        break;
    case 0x4c:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xe0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0xe4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0xe8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0xec);
        break;
    case 0x4d:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x230);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x234);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x238);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x23c);
        break;
    case 0x4e:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2b0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2b4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2b8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 700);
        break;
    case 0x4f:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2c0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2c4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2c8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2cc);
        break;
    case 0x50:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2d0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2d4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2d8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2dc);
        break;
    case 0x51:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2e0);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2e4);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2e8);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x2ec);
        break;
    case 0x53:
        allocationField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x368);
        resourceCount = &S3D_RESOURCE_POOL_WORD(poolManager, 0x36c);
        freeListHead = &S3D_RESOURCE_POOL_WORD(poolManager, 0x370);
        entriesField = &S3D_RESOURCE_POOL_WORD(poolManager, 0x374);
        break;
    default:
        goto unsupported_resource_type;
    }
    goto selected_resource_pool;

unsupported_resource_type:
    freeListHead = (unsigned int *)resourceType;
    entriesField = (unsigned int *)resourceType;
    allocationField = (unsigned int *)resourceType;
    resourceCount = (unsigned int *)resourceType;

selected_resource_pool:
    resourceArray = (int *)*entriesField;
    nextFreeIndex = *freeListHead;
    if (*freeListHead < *resourceCount)
    {
        do
        {
            nextIndex = (unsigned int)resourceArray[nextFreeIndex];
            resourceArray[nextFreeIndex] = 0;
            nextFreeIndex = nextIndex;
        } while (nextIndex < *resourceCount);
    }
    resourceIndex = 0;
    resourceSlot = resourceArray;
    if (*resourceCount != 0)
    {
        do
        {
            currentResource = *resourceSlot;
            if (currentResource != 0)
            {
                if (resourceType < 0x24)
                {
                    if (resourceType == 0x23)
                    {
                        result = EQGFX_DESTROY_PALETTE(managerAddress, currentResource);
                    }
                    else
                    {
                        switch (resourceType)
                        {
                        case 0x1:
                            result = EQGFX_UNDEFINE_POLYHEDRON(managerAddress, currentResource);
                            break;
                        case 0x2:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt18)->ownerAt28 != (int *)currentResource)
                                result = EQGFX_DESTROY_POLYHEDRON(managerAddress, currentResource);
                            break;
                        case 0x3:
                            result = EQGFX_UNDEFINE_SPHERE_LIST(managerAddress, currentResource);
                            break;
                        case 0x4:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt18)->ownerAt24 != (int *)currentResource)
                                result = EQGFX_DESTROY_POLYHEDRON(managerAddress, currentResource);
                            break;
                        case 0x5:
                            result = EQGFX_UNDEFINE_SIMPLE_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x6:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt28)->ownerAt1c != (int *)currentResource)
                                result = EQGFX_DESTROY_SIMPLE_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x7:
                            result = EQGFX_UNDEFINE_BLIT_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x8:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt6c)->ownerAt14 != (int *)currentResource)
                                result = EQGFX_DESTROY_BLIT_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x9:
                            result = EQGFX_UNDEFINE_2D_SPRITE(managerAddress, currentResource);
                            break;
                        case 0xa:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt88)->ownerAt30 != (int *)currentResource)
                                result = EQGFX_DESTROY_2D_SPRITE(managerAddress, currentResource);
                            break;
                        case 0xb:
                            result = EQGFX_UNDEFINE_3D_SPRITE(managerAddress, currentResource);
                            break;
                        case 0xc:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt2c)->ownerAt2c != (int *)currentResource)
                                result = EQGFX_DESTROY_3D_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x11:
                            result = EQGFX_UNDEFINE_COMPOSITE_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x12:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt38)->ownerAt24 != (int *)currentResource)
                                result = EQGFX_DESTROY_COMPOSITE_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x13:
                            result = EQGFX_UNDEFINE_HIERARCHICAL_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x14:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt5c)->ownerAt1c != (int *)currentResource)
                                result = EQGFX_DESTROY_HIERARCHICAL_SPRITE(managerAddress, currentResource);
                            break;
                        case 0x15:
                            result = EQGFX_UNDEFINE_TRACK(managerAddress, currentResource);
                            break;
                        case 0x16:
                            deformationCount = (unsigned int)EQGFX_GET_DMRGB_DEFORMATIONS(currentResource);
                            currentResource = EQGFX_GET_TRACK_DEFAULT_INSTANCE((int)deformationCount);
                            if (currentResource != *resourceSlot)
                                result = EQGFX_DESTROY_TRACK(managerAddress, *resourceSlot);
                            break;
                        case 0x17:
                            result = EQGFX_UNDEFINE_ACTOR(managerAddress, currentResource);
                            break;
                        case 0x18:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt64)->ownerAt28 != (int *)currentResource)
                                result = EQGFX_DESTROY_ACTOR(managerAddress, currentResource);
                            break;
                        case 0x19:
                            result = EQGFX_CLEANUP_WORLD_OBJECT(managerAddress, currentResource);
                            break;
                        case 0x1a:
                            result = EQGFX_UNDEFINE_AI(managerAddress, currentResource);
                            break;
                        case 0x1b:
                            result = EQGFX_UNDEFINE_BMINFO(managerAddress, (int)resourceSlot);
                            break;
                        case 0x1c:
                            result = EQGFX_UNDEFINE_LIGHT(managerAddress, currentResource);
                            break;
                        case 0x1d:
                            if (((S3dCleanupOwner_guess *)S3D_CLEANUP_RESOURCE(currentResource)->objectAt24)->ownerAt20 != (int *)currentResource)
                                result = EQGFX_DESTROY_LIGHT(managerAddress, currentResource);
                            break;
                        case 0x1e:
                            result = EQGFX_DESTROY_POINT_LIGHT(managerAddress, currentResource);
                            break;
                        case 0x1f:
                            S3D_CLEANUP_RESOURCE(currentResource)->objectAt10 = 0;
                            result = EQGFX_DESTROY_DIRECTIONAL_LIGHT(managerAddress, *resourceSlot);
                        }
                    }
                }
                else
                {
                    switch (resourceType)
                    {
                    case 0x44:
                        result = EQGFX_DESTROY_ZONE(managerAddress, currentResource);
                        break;
                    case 0x45:
                        result = EQGFX_UNDEFINE_MATERIAL(managerAddress, currentResource);
                        break;
                    case 0x46:
                        result = EQGFX_DESTROY_MATERIAL_PALETTE(managerAddress, currentResource);
                        break;
                    case 0x47:
                        result = EQGFX_UNDEFINE_DM_SPRITE(managerAddress, currentResource);
                        break;
                    case 0x48:
                        result = EQGFX_DESTROY_DM_SPRITE(managerAddress, currentResource);
                        break;
                    case 0x49:
                        result = EQGFX_UNDEFINE_DM_TRACK(managerAddress, currentResource);
                        break;
                    case 0x4a:
                        result = EQGFX_DESTROY_DM_RGB_TRACK(managerAddress, currentResource);
                        break;
                    case 0x4c:
                        result = EQGFX_UNDEFINE_PARTICLE_CLOUD(managerAddress, currentResource);
                        break;
                    case 0x4d:
                        result = EQGFX_DESTROY_PARTICLE_CLOUD(managerAddress, (int)resourceSlot);
                        break;
                    case 0x4e:
                        result = EQGFX_UNDEFINE_DM_RGB_TRACK(managerAddress, currentResource);
                        break;
                    case 0x4f:
                        result = EQGFX_DESTROY_DM_RGB_TRACK(managerAddress, currentResource);
                        break;
                    case 0x50:
                        result = EQGFX_DESTROY_FONT_TEXTURE(managerAddress, currentResource);
                        break;
                    case 0x51:
                        result = EQGFX_DESTROY_STRING_SPRITE(managerAddress, currentResource);
                        break;
                    case 0x53:
                        result = EQGFX_DESTROY_ALLOCATION_POOL(managerAddress, currentResource);
                    }
                }
            }
            resourceIndex = resourceIndex + 1;
            resourceSlot = resourceSlot + 1;
        } while (resourceIndex < *resourceCount);
    }
    if (resourceType == 0x19)
    {
        EQGFX_FREE_MEMORY(*resourceArray);
        *allocationField = 0;
    }
    if (*allocationField == 0)
    {
        EQGFX_FREE_MEMORY((int)resourceArray);
        *resourceCount = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x10016cc9
// Screens material definitions before they are used by the renderer.
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Checks
// the type tag at +0x0 is the material definition tag 0x45. Returns 0 when valid,
// 0xFFFF3D3E otherwise.
int __cdecl s3dIsMaterialDefinitionValid(int *materialDef)
{
    int result;

    result = 0;
    if (*materialDef != 0x45)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x10016cda
// Provides storage sizing for material definitions.
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns the allocation
// size for a material definition: 0x40 when `materialName` is NULL, else
// strlen(materialName) + 0x41 (base 0x40 plus the name and its terminator).
int __cdecl s3dCalcMaterialDefAllocSize(char *materialName)
{
    int allocSize = 0x40;

    if (materialName != 0)
        allocSize = strlen(materialName) + 0x41;
    return allocSize;
}

// FUNCTION: EQGFX 0x10016CF2
/* fake match: the empty registration failure branch preserves VC6 block order. */
// Defines a material with default rendering settings and an optional lookup name.
// __cdecl (callee pops 0 bytes; ECX is not an input). Fills providedMaterial (flag 0x20, memory kept) or a new material, names it and registers it; returns it, or 0 on failure.
S3dMaterial_guess *__cdecl s3dDefineMaterial(void *context, S3dMaterial_guess *providedMaterial, char *materialName)
{
    S3dMaterial_guess *material;
    int allocSize;
    int dictionaryResult;

    material = providedMaterial;
    if (material == 0)
    {
        allocSize = EQ_CDECL(int, 0x10016CDA, (char *))(materialName);
        material = (S3dMaterial_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(allocSize);
        if (material == 0)
        {
            return 0;
        }
    }
    material->code = 0x45;
    S3D_MATERIAL_DEF(material)->constantIntensity = 0.0f;
    material->dictionaryKey = 0;
    *((unsigned int *)&material->flags) = 0;
    S3D_MATERIAL_DEF(material)->ambientScalar = 1.0f;
    material->useCount = 0;
    S3D_MATERIAL_DEF(material)->renderMethod = 7;
    S3D_MATERIAL_DEF(material)->uShiftPerMs = 0.0f;
    S3D_MATERIAL_DEF(material)->byteAt2c = 0;
    S3D_MATERIAL_DEF(material)->twoSided = 0;
    S3D_MATERIAL_DEF(material)->vShiftPerMs = 0.0f;
    S3D_MATERIAL_SPRITE(material) = 0;
    S3D_MATERIAL_DEF(material)->floatAt38 = 0.0f;
    S3D_MATERIAL_DEF(material)->floatAt3c = 0.0f;
    if (providedMaterial != 0)
    {
        *((unsigned int *)&material->flags) |= S3D_MATERIAL_FLAG_KEEP_MEMORY;
    }
    if (materialName != 0)
    {
        material->dictionaryKey = S3D_MATERIAL_DEF(material)->nameText;
        EQ_CDECL(size_t, 0x10068160, (const char *))(materialName);
        EQ_CDECL(void, 0x10068070, (char *, const char *))((char *)material->dictionaryKey, materialName);
    }
    else
    {
        material->dictionaryKey = 0;
    }
    if (EQ_CDECL(int, 0x1001564B, (void *, S3dMaterial_guess *))(context, material) != 0)
    {
    }
    else
    {
        if (materialName == 0)
        {
            return material;
        }
        dictionaryResult = EQ_CDECL(int, 0x1000DC08, (void *, S3dMaterial_guess *))(context, material);
        if (dictionaryResult < 0)
        {
            EQ_CDECL(int, 0x10015D81, (void *, S3dMaterial_guess *))(context, material);
        }
        else
        {
            return material;
        }
    }
    if (material != 0 && providedMaterial == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(material);
    }
    return 0;
}

// FUNCTION: EQGFX 0x10016dcb
// Removes a material resource after its active references are released.
// __cdecl (callee pops 0 bytes; ECX is not an input). Returns 0xFFFF3D3E while the use count is not 0.
// Else: destroys the live sprite at +0x24, removes the dictionary entry (flag 0x01), runs t3dFreeObjectPoolSlot_guess, frees unless flag 0x20.
int __cdecl s3dUndefineMaterial(void *context, S3dMaterial_guess *material)
{
    int result;

    if (material->useCount != 0)
    {
        result = 0xFFFF3D3E;
    }
    else
    {
        int *sprite = S3D_MATERIAL_SPRITE(material);

        if (sprite != 0 && *sprite != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sprite);
            EQ_CDECL(void, 0x1001B94A, (void *, void *))(context, S3D_MATERIAL_SPRITE(material));
        }
        if ((material->flags & S3D_MATERIAL_FLAG_IN_DICT) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, material->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, S3dMaterial_guess *))(context, material);
        if ((material->flags & S3D_MATERIAL_FLAG_KEEP_MEMORY) == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(material);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x10016e33
// Selects the draw method used for a material definition.
// __cdecl (callee pops 0 bytes; ECX is not an input). Validates a render method
// against the limit at +0x3dc, stores it at +0x14 of a material definition.
int __cdecl s3dSetMaterialDefRenderMethod(S3dRendererLimits_guess *rendererLimits, S3dMaterialDef_guess *materialDef, unsigned int renderMethod)
{
    if (((renderMethod & 0x80000000) != 0) && ((renderMethod & 0x7FFFFFFF) >= rendererLimits->maxRenderMethod))
        return 0xFFFF3D3E;
    if (((renderMethod & 0x80000000) == 0) && ((renderMethod & 0xFEEFFFFF) > 0x7FFFFFFF))
        return 0xFFFF3D3E;
    materialDef->renderMethod = (int)renderMethod;
    return 0;
}

// FUNCTION: EQGFX 0x10016e7d
// Assigns the simple sprite used by a material definition.
// __cdecl (callee pops 0 bytes; ECX is not an input). Swaps the simple sprite
// at +0x24 of a material definition; the first arg is unused (table callback signature). Returns 0.
int __cdecl s3dSetMaterialDefSimpleSprite(int unused, int *materialDef, int *simpleSprite)
{
    int *oldSprite = (int *)materialDef[9]; // 0x24

    if (oldSprite != 0 && *oldSprite != 0)
        EQ_CDECL(int, 0x10017DAD, (int *))(oldSprite);
    materialDef[9] = (int)simpleSprite; // 0x24
    if (simpleSprite != 0 && *simpleSprite != 0)
        EQ_CDECL(int, 0x10017CE8, (int *))(simpleSprite);
    return 0;
}

// FUNCTION: EQGFX 0x10016eb0
// Adjusts the ambient-light contribution of a material.
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Sets the
// ambient scalar at +0x1c of a material definition; the first arg is unused
// (table callback signature). Returns 0 (callers ignore EAX).
int __cdecl s3dSetMaterialDefAmbientScalar(int unused, int *materialDef, float ambientScalar)
{
    *(float *)&materialDef[7] = ambientScalar; // 0x1c
    return 0;
}

// FUNCTION: EQGFX 0x10016ebe
// Sets the fixed intensity used when drawing a material.
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Sets the
// constant intensity at +0x18 of a material definition; the first arg is unused
// (table callback signature). Returns 0 (callers ignore EAX).
int __cdecl s3dSetMaterialDefConstantIntensity(int unused, int *materialDef, float constantIntensity)
{
    *(float *)&materialDef[6] = constantIntensity; // 0x18
    return 0;
}

// FUNCTION: EQGFX 0x10016ecc
// Controls whether both faces of a material's surfaces are drawn.
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Sets the
// two-sided flag at +0x20 of a material definition; the first arg is unused
// (table callback signature). Returns 0 (callers ignore EAX).
int __cdecl s3dSetMaterialDefTwoSided(int unused, int *materialDef, int twoSided)
{
    materialDef[8] = twoSided; // 0x20
    return 0;
}

// FUNCTION: EQGFX 0x10016eda
// Sets how quickly a material's texture coordinates scroll.
// __cdecl (callee pops 0 bytes; ECX loaded from the stack, not an input). Sets the
// U/V shift rates at +0x30/+0x34 of a material definition; the first arg is unused
// (table callback signature). Returns 0 (callers ignore EAX).
int __cdecl s3dSetMaterialDefUVShiftPerMs(int unused, int *materialDef, float uShiftPerMs, float vShiftPerMs)
{
    *(float *)&materialDef[12] = uShiftPerMs; // 0x30
    *(float *)&materialDef[13] = vShiftPerMs; // 0x34
    return 0;
}

// FUNCTION: EQGFX 0x10016eef
// Gets the simple sprite assigned to a material definition.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the simple sprite at +0x24 of a material definition.
int *__cdecl s3dGetMaterialDefSimpleSprite(int unused, int *materialDef)
{
    return (int *)materialDef[9];
}

// FUNCTION: EQGFX 0x10016ef7
// Reads a material's ambient-light scalar.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the ambient scalar at +0x1c of a material definition.
float __cdecl s3dGetMaterialDefAmbientScalar(int unused, S3dMaterialDef_guess *materialDef)
{
    return materialDef->ambientScalar;
}

// FUNCTION: EQGFX 0x10016eff
// Reads the fixed intensity value used for a material.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the constant intensity at +0x18 of a material definition.
float __cdecl s3dGetMaterialDefConstantIntensity(int unused, S3dMaterialDef_guess *materialDef)
{
    return materialDef->constantIntensity;
}

// FUNCTION: EQGFX 0x10016f07
// Reads whether both faces of a material's surfaces are drawn.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the two-sided flag at +0x20 of a material definition.
int __cdecl s3dGetMaterialDefTwoSided(int unused, S3dMaterialDef_guess *materialDef)
{
    return materialDef->twoSided;
}

// FUNCTION: EQGFX 0x10016f0f
// Reads how quickly a material's texture coordinates scroll.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Copies the U/V shift rates at +0x30/+0x34 through the out pointers. Returns 0.
int __cdecl s3dGetMaterialDefUVShiftPerMs(int unused, S3dMaterialDef_guess *materialDef, float *uShiftOut, float *vShiftOut)
{
    *uShiftOut = materialDef->uShiftPerMs;
    *vShiftOut = materialDef->vShiftPerMs;
    return 0;
}

// FUNCTION: EQGFX 0x10016f28
// Checks the code at +0x0 of a material for the valid palette marker.
int __cdecl s3dIsMaterialPaletteValid(S3dMaterial_guess *material)
{
    int result = 0;
    if (material->code != 0x46)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x10016f39
// Creates a material palette from supplied material entries, optionally with white tints.
S3dMaterial_guess *__cdecl s3dCreateMaterialPalette(void *dictionary, unsigned int entryCount, void *entryData, int allocateTintEntries)
{
    S3dMaterial_guess *palette;
    void *allocatedEntries;
    void *allocatedTintEntries;
    int registrationResult;
    unsigned int entryIndex;
    unsigned int allocationSize;

    palette = (S3dMaterial_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x28);
    if (palette != 0)
    {
        palette->dictionaryKey = 0;
        *((unsigned int *)&palette->flags) = 0;
        palette->useCount = 0;
        palette->code = 0x46;
        palette->paletteType = 1;
        palette->paletteContext = ((S3dMaterialPaletteDictionary_guess *)dictionary)->paletteContext;
        allocationSize = entryCount << 2;
        palette->paletteEntryCount = entryCount;
        allocatedEntries = EQ_CDECL(void *, 0x10050C48, (unsigned int))(allocationSize);
        palette->paletteEntries = (int *)allocatedEntries;
        if (allocatedEntries == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(palette);
        }
        else
        {
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(allocatedEntries, entryData, allocationSize);
            if (allocateTintEntries == 0)
            {
                palette->tintEntries = 0;
            }
            else
            {
                allocatedTintEntries = EQ_CDECL(void *, 0x10050C48, (unsigned int))(allocationSize);
                palette->tintEntries = (int *)allocatedTintEntries;
                if (allocatedTintEntries == 0)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette->paletteEntries);
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette);
                }
                entryIndex = 0;
                if (entryCount != 0)
                {
                    do
                    {
                        *((unsigned char *)palette->tintEntries + entryIndex * 4 + 2) |= 0xff;
                        *((unsigned char *)palette->tintEntries + entryIndex * 4 + 1) |= 0xff;
                        *((unsigned char *)palette->tintEntries + entryIndex * 4) |= 0xff;
                        entryIndex++;
                    } while (entryIndex < entryCount);
                }
            }
            registrationResult = EQ_CDECL(int, 0x1001564B, (void *, S3dMaterial_guess *))(dictionary, palette);
            if (registrationResult == 0)
            {
                entryIndex = 0;
                if (palette->paletteEntryCount != 0)
                {
                    do
                    {
                        EQ_CDECL(void, 0x10017CE8, (void *))(palette->paletteEntries[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < (unsigned int)palette->paletteEntryCount);
                }
            }
            else
            {
                if (allocateTintEntries != 0)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette->tintEntries);
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(palette->paletteEntries);
                EQ_CDECL(void, 0x10050CBB, (void *))(palette);
                palette = 0;
            }
        }
    }
    return palette;
}

// FUNCTION: EQGFX 0x10017042
// Creates an independent copy of a material palette's assignments and tints while sharing its materials.
// __cdecl. Copies source's entries (and tint entries) into a new 0x28-byte palette, registers it in dictionary and shares its entries. Returns it; 0 if the first allocation or the registration fails; a failed entries or tint allocation returns the already freed palette.
S3dMaterial_guess *__cdecl s3dDuplicateMaterialPalette(void *dictionary, S3dMaterial_guess *source)
{
    S3dMaterial_guess *palette;
    void *allocatedEntries;
    void *allocatedTintEntries;
    int registrationResult;
    unsigned int entryIndex;

    palette = (S3dMaterial_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x28);
    if (palette != 0)
    {
        palette->code = 0x46;
        palette->dictionaryKey = 0;
        *((unsigned int *)&palette->flags) = 0;
        palette->useCount = 0;
        palette->paletteEntryCount = source->paletteEntryCount;
        allocatedEntries = EQ_CDECL(void *, 0x10050C48, (unsigned int))(source->paletteEntryCount << 2);
        palette->paletteEntries = (int *)allocatedEntries;
        if (allocatedEntries == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(palette);
        }
        else
        {
            EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(allocatedEntries, source->paletteEntries, source->paletteEntryCount << 2);
            if (source->tintEntries != 0)
            {
                allocatedTintEntries = EQ_CDECL(void *, 0x10050C48, (unsigned int))(source->paletteEntryCount << 2);
                palette->tintEntries = (int *)allocatedTintEntries;
                if (allocatedTintEntries == 0)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette->paletteEntries);
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette);
                    return palette;
                }
                EQ_CDECL(void *, 0x10067D30, (void *, void *, unsigned int))(allocatedTintEntries, source->tintEntries, source->paletteEntryCount << 2);
            }
            else
            {
                palette->tintEntries = 0;
            }
            registrationResult = EQ_CDECL(int, 0x1001564B, (void *, S3dMaterial_guess *))(dictionary, palette);
            if (registrationResult != 0)
            {
                if (palette->tintEntries != 0)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(palette->tintEntries);
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(palette->paletteEntries);
                EQ_CDECL(void, 0x10050CBB, (void *))(palette);
                palette = 0;
            }
            else
            {
                entryIndex = 0;
                if (palette->paletteEntryCount > 0)
                {
                    do
                    {
                        EQ_CDECL(void, 0x10017CE8, (void *))((void *)palette->paletteEntries[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < palette->paletteEntryCount);
                }
            }
        }
    }
    return palette;
}

// FUNCTION: EQGFX 0x10017135
// Releases a material palette and its shared color and tint entries.
// __cdecl. Refuses with 0xffff3d3e while the use count is nonzero; else unshares the entries, frees the blocks and returns 0x10015d81's result.
int __cdecl s3dDestroyMaterialPalette(void *dictionary, S3dMaterial_guess *material)
{
    unsigned int entryIndex;
    int result;

    entryIndex = 0;
    if (material->useCount != 0)
    {
        result = 0xFFFF3D3E;
    }
    else
    {
        if ((material->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, material->dictionaryKey);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(dictionary, material);
        if (material->paletteEntryCount > 0)
        {
            do
            {
                EQ_CDECL(void, 0x10017DAD, (void *))((void *)material->paletteEntries[entryIndex]);
                entryIndex++;
            } while (entryIndex < material->paletteEntryCount);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(material->paletteEntries);
        if (material->tintEntries != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(material->tintEntries);
        }
        if (material->dictionaryKey != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(material->dictionaryKey);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(material);
    }
    return result;
}

// FUNCTION: EQGFX 0x100171b6
// Gets the draw method selected for a material definition.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the render method at +0x14 of a material definition.
int __cdecl s3dGetMaterialDefRenderMethod(int unused, S3dMaterialDef_guess *materialDef)
{
    return materialDef->renderMethod;
}

// FUNCTION: EQGFX 0x100171be
// Returns one indexed entry from a material palette.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Returns the palette entry at the given index.
int __cdecl s3dGetMaterialPaletteEntry(int unused, S3dMaterial_guess *material, int paletteIndex)
{
    return material->paletteEntries[paletteIndex];
}

// FUNCTION: EQGFX 0x100171cd
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
int __cdecl s3dSetMaterialPaletteEntry(int unused, S3dMaterial_guess *material, int paletteIndex, int newEntry)
{
    ((int *)material)[8] = 1;
    EQ_CDECL(int, 0x10017DAD, (int *))((int *)material->paletteEntries[paletteIndex]);
    material->paletteEntries[paletteIndex] = newEntry;
    EQ_CDECL(int, 0x10017CE8, (int *))((int *)newEntry);
    return 0;
}

// FUNCTION: EQGFX 0x10017205
// Reads the tint assigned to a material-palette entry.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Copies the tint entry at the given index through the out pointer. Returns 0.
int __cdecl s3dGetMaterialPaletteEntryTint(int unused, S3dMaterial_guess *material, int paletteIndex, int *tintOut)
{
    if (material->tintEntries != 0)
    {
        *tintOut = material->tintEntries[paletteIndex];
        return 0;
    }
    return 0xFFFF3D3E;
}

// FUNCTION: EQGFX 0x10017226
// Changes the tint assigned to a material-palette entry.
// __cdecl (callee pops 0 bytes; first arg is unused table callback signature).
// Stores the tint entry at the given index. Returns 0.
int __cdecl s3dSetMaterialPaletteEntryTint(int unused, S3dMaterial_guess *material, int paletteIndex, int *tintIn)
{
    if (material->tintEntries != 0)
    {
        material->tintEntries[paletteIndex] = *tintIn;
        return 0;
    }
    return 0xFFFF3D3E;
}

// FUNCTION: EQGFX 0x1001727b
// Measures the length of a rendering resource's name tag.
// __cdecl (callee pops 0 bytes; ECX is loaded from the object, not an input). The type id at +0x0
// selects the tag pointer: +0x4 for kind 0x3b, +0x8 otherwise; returns its length, else 0.
unsigned int __cdecl t3dGetObjectTagLength(S3dObject_guess *object)
{
    char *tag;
    int objectType = object->type;

    switch (objectType)
    {
    case 0x3b:
        tag = object->tagAlt;
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
    case 0x23:
    case 0x24:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x53:
        tag = object->tag;
        break;
    default:
        return 0;
    }

    if (tag == 0)
        return 0;
    return EQ_CDECL(unsigned int, 0x10068160, (char *))(tag);
}

// FUNCTION: EQGFX 0x10017386
// Copies a rendering resource's name tag into the caller's buffer.
int __cdecl t3dGetObjectTag(S3dObject_guess *object, char *tagBuffer)
{
    int result = 0;
    int objectType = 0;

    if (object == 0)
    {
        result = S3D_RESOURCE_POOL_ERROR;
        goto copyTag;
    }

    objectType = object->type;
    if (objectType < 0x1c)
    {
        if (objectType != 0x1b)
        {
            switch (objectType)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1a:
                break;
            default:
                result = S3D_RESOURCE_POOL_ERROR;
                objectType = 0;
                goto copyTag;
            }
        }
    }
    else
    {
        if (objectType > 0x47)
        {
            switch (objectType)
            {
            case 0x48:
            case 0x49:
            case 0x4a:
            case 0x4c:
            case 0x4d:
            case 0x4e:
            case 0x4f:
            case 0x50:
            case 0x51:
            case 0x53:
                goto usePrimaryTag;
            default:
                result = S3D_RESOURCE_POOL_ERROR;
                objectType = 0;
                goto copyTag;
            }
        }
        if (objectType != 0x47)
        {
            if (objectType < 0x25)
            {
                if (objectType != 0x24 && objectType != 0x1c && objectType != 0x1d
                    && objectType != 0x1e && objectType != 0x1f)
                {
                    objectType = objectType - 0x23;
                    if (objectType != 0)
                    {
                        result = S3D_RESOURCE_POOL_ERROR;
                        objectType = 0;
                        goto copyTag;
                    }
                }
            }
            else
            {
                if (objectType == 0x3b)
                {
                    objectType = (int)object->tagAlt;
                    goto copyTag;
                }
                if (objectType != 0x44 && objectType != 0x45)
                {
                    objectType = objectType - 0x46;
                    if (objectType != 0)
                    {
                        result = S3D_RESOURCE_POOL_ERROR;
                        objectType = 0;
                        goto copyTag;
                    }
                }
            }
        }
    }

usePrimaryTag:
    objectType = (int)object->tag;

copyTag:
    if (objectType == 0)
    {
        *tagBuffer = 0;
    }
    else
    {
        EQ_CDECL(void, 0x10068070, (char *, const char *))(tagBuffer, (char *)objectType);
    }
    return result;
}

// FUNCTION: EQGFX 0x100174B8
// Changes or clears a rendering resource's name tag and updates its dictionary registration.
// @early-stop: VC6 keeps 0 in EDI (zero register) across the 43 body copies; one copy's head stays after the dec chain
/* fake match: one body copy per case gives the original's jump tables; VC6 cross-jumps the copies into one */
int __cdecl t3dSetObjectTag(void *dictionary, S3dObject_guess *object, char *tag)
{
    unsigned int inDictionary;
    char *newTag;
    int result;

#define SET_OBJECT_TAG                                                              \
    if (tag == 0)                                                                           \
    {                                                                                       \
        newTag = 0;                                                                         \
    }                                                                                       \
    else                                                                                    \
    {                                                                                       \
        newTag = EQ_CDECL(char *, 0x10050C48, (unsigned int))(                              \
            EQ_CDECL(size_t, 0x10068160, (const char *))(tag) + 1);                         \
        if (newTag == 0)                                                                    \
        {                                                                                   \
            result = S3D_RESOURCE_POOL_ERROR;                                               \
            break;                                                                          \
        }                                                                                   \
        EQ_CDECL(void, 0x10068070, (char *, const char *))(newTag, tag);                    \
    }                                                                                       \
    inDictionary = object->flags & 1;                                                     \
    if (inDictionary)                                                                       \
        EQ_CDECL(void, 0x1000E334, (void *, void *))(dictionary, object->tag);            \
    if (object->tag != 0)                                                                 \
        EQ_CDECL(void, 0x10050CBB, (void *))(object->tag);                                \
    object->tag = newTag;                                                                 \
    if (inDictionary && newTag != 0                                                         \
        && EQ_CDECL(int, 0x1000E7FE, (void *, void *))(dictionary, newTag) == 0)            \
        EQ_CDECL(void, 0x1000DC08, (void *, void *))(dictionary, object);        \
    break;

    result = 0;
    switch (object->type)
    {
    case 1: SET_OBJECT_TAG
    case 2: SET_OBJECT_TAG
    case 3: SET_OBJECT_TAG
    case 4: SET_OBJECT_TAG
    case 5: SET_OBJECT_TAG
    case 6: SET_OBJECT_TAG
    case 7: SET_OBJECT_TAG
    case 8: SET_OBJECT_TAG
    case 9: SET_OBJECT_TAG
    case 10: SET_OBJECT_TAG
    case 11: SET_OBJECT_TAG
    case 12: SET_OBJECT_TAG
    case 0x11: SET_OBJECT_TAG
    case 0x12: SET_OBJECT_TAG
    case 0x13: SET_OBJECT_TAG
    case 0x14: SET_OBJECT_TAG
    case 0x15: SET_OBJECT_TAG
    case 0x16: SET_OBJECT_TAG
    case 0x17: SET_OBJECT_TAG
    case 0x18: SET_OBJECT_TAG
    case 0x19: SET_OBJECT_TAG
    case 0x1a: SET_OBJECT_TAG
    case 0x1b: SET_OBJECT_TAG
    case 0x1c: SET_OBJECT_TAG
    case 0x1d: SET_OBJECT_TAG
    case 0x1e: SET_OBJECT_TAG
    case 0x1f: SET_OBJECT_TAG
    case 0x23: SET_OBJECT_TAG
    case 0x24: SET_OBJECT_TAG
    case 0x44: SET_OBJECT_TAG
    case 0x45: SET_OBJECT_TAG
    case 0x46: SET_OBJECT_TAG
    case 0x47: SET_OBJECT_TAG
    case 0x48: SET_OBJECT_TAG
    case 0x49: SET_OBJECT_TAG
    case 0x4a: SET_OBJECT_TAG
    case 0x4c: SET_OBJECT_TAG
    case 0x4d: SET_OBJECT_TAG
    case 0x4e: SET_OBJECT_TAG
    case 0x4f: SET_OBJECT_TAG
    case 0x50: SET_OBJECT_TAG
    case 0x51: SET_OBJECT_TAG
    case 0x53: SET_OBJECT_TAG
    default:
        result = S3D_RESOURCE_POOL_ERROR;
        break;
    }
#undef SET_OBJECT_TAG
    return result;
}

// FUNCTION: EQGFX 0x10017637
// Marks a rendering resource's entry and name tag as temporary.
// __cdecl. Adds object to the temporary-tag list once (flags 0x6 mark it), 0xFFFF3D3E for a refused type or a failed allocation.
int __cdecl t3dSetObjectTagTemporary(T3D_tagWORLD *poolManager, S3dObject_guess *object)
{
    int result = 0;
    int objectType = object->type;
    unsigned int capacity;
    unsigned int *newList;

    if (objectType < 0x1b)
    {
        if (objectType != 0x1a)
        {
            switch (objectType)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
                break;
            default:
                goto setError;
            }
        }
    }
    else
    {
        if (objectType > 0x47)
        {
            switch (objectType)
            {
            case 0x48:
            case 0x49:
            case 0x4a:
            case 0x4c:
            case 0x4d:
            case 0x4e:
            case 0x4f:
            case 0x50:
            case 0x51:
            case 0x53:
                goto checkFlags;
            default:
                goto setError;
            }
        }
        if (objectType != 0x47)
        {
            if (objectType < 0x24)
            {
                if (objectType != 0x23 && objectType != 0x1b && objectType != 0x1c)
                {
                    objectType = objectType - 0x1d;
checkSubtype:
                    if (objectType != 0 && objectType != 1 && objectType != 2)
                        goto setError;
                }
            }
            else if (objectType != 0x24)
            {
                objectType = objectType - 0x44;
                goto checkSubtype;
            }
        }
    }

checkFlags:
    if ((object->flags & 6) == 0)
    {
        capacity = poolManager->temporaryTagCapacity;
        if (capacity <= poolManager->temporaryTagCount)
        {
            if (poolManager->trackedObjects_bits == 0)
            {
                newList = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x100);
                poolManager->trackedObjects_bits = newList;
                if (newList == 0)
                    goto setError;
            }
            else
            {
                newList = EQGFX_REALLOC(poolManager->trackedObjects_bits, ((capacity >> 2) + 1 + capacity) * 4);
                if (newList == 0)
                    goto setError;
                poolManager->trackedObjects_bits = newList;
            }
            poolManager->temporaryTagCapacity = 0x40;
        }
        poolManager->trackedObjects_bits[poolManager->temporaryTagCount] = (unsigned int)object;
        poolManager->temporaryTagCount++;
    }
    object->flags |= 6;
    goto returnResult;
setError:
    result = S3D_RESOURCE_POOL_ERROR;
returnResult:
    return result;
}

// FUNCTION: EQGFX 0x100177c2
// Marks a rendering resource's entry as temporary without changing its name tag's temporary status.
// __cdecl. Adds object to the temporary list once (flags 0x6 mark it), then sets flag 0x2; 0xFFFF3D3E for a refused type or a failed allocation.
int __cdecl t3dSetObjectEntryTemporary(T3D_tagWORLD *poolManager, S3dObject_guess *object)
{
    int result = 0;
    int objectType = object->type;
    unsigned int capacity;
    unsigned int *newList;

    if (objectType < 0x1b)
    {
        if (objectType != 0x1a)
        {
            switch (objectType)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
                break;
            default:
                goto setError;
            }
        }
    }
    else
    {
        if (objectType > 0x47)
        {
            switch (objectType)
            {
            case 0x48:
            case 0x49:
            case 0x4a:
            case 0x4c:
            case 0x4d:
            case 0x4e:
            case 0x4f:
            case 0x50:
            case 0x51:
            case 0x53:
                goto checkFlags;
            default:
                goto setError;
            }
        }
        if (objectType != 0x47)
        {
            if (objectType < 0x24)
            {
                if (objectType != 0x23 && objectType != 0x1b && objectType != 0x1c)
                {
                    objectType = objectType - 0x1d;
checkSubtype:
                    if (objectType != 0 && objectType != 1 && objectType != 2)
                        goto setError;
                }
            }
            else if (objectType != 0x24)
            {
                objectType = objectType - 0x44;
                goto checkSubtype;
            }
        }
    }

checkFlags:
    if ((object->flags & 6) == 0)
    {
        capacity = poolManager->temporaryTagCapacity;
        if (capacity <= poolManager->temporaryTagCount)
        {
            if (poolManager->trackedObjects_bits == 0)
            {
                newList = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x100);
                poolManager->trackedObjects_bits = newList;
                if (newList == 0)
                    goto setError;
            }
            else
            {
                newList = EQGFX_REALLOC(poolManager->trackedObjects_bits, ((capacity >> 2) + 1 + capacity) * 4);
                if (newList == 0)
                    goto setError;
                poolManager->trackedObjects_bits = newList;
            }
            poolManager->temporaryTagCapacity = 0x40;
        }
        poolManager->trackedObjects_bits[poolManager->temporaryTagCount] = (unsigned int)object;
        poolManager->temporaryTagCount++;
    }
    object->flags |= 2;
    goto returnResult;
setError:
    result = S3D_RESOURCE_POOL_ERROR;
returnResult:
    return result;
}

// FUNCTION: EQGFX 0x1001794d
// Checks whether a rendering resource is registered for lookup by name.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the object, not an input). One object pointer:
// the type id at +0x0 selects dictionary kinds; returns the flags bit at +0xc for those, else 0.
unsigned int __cdecl t3dIsObjectInDictionary(S3dObject_guess *object)
{
    int objectType = object->type;

    switch (objectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
    case 0x23:
    case 0x24:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x53:
        break;
    default:
        return 0;
    }

    return object->flags & 1;
}

// FUNCTION: EQGFX 0x10017a3f
// Checks whether a rendering resource's name tag is marked temporary.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the object, not an input). One object pointer:
// the type id at +0x0 selects temporary-tag kinds; returns bit 2 of the flags at +0xc for those, else 0.
unsigned int __cdecl t3dIsObjectTagTemporary(S3dObject_guess *object)
{
    int objectType = object->type;

    switch (objectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
    case 0x23:
    case 0x24:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x53:
        break;
    default:
        return 0;
    }

    return ((unsigned int)object->flags >> 2) & 1;
}

// FUNCTION: EQGFX 0x10017b34
// Checks whether a rendering resource's entry is marked temporary.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the object, not an input). One object pointer:
// the type id at +0x0 selects entry-temporary kinds; returns bit 1 of the flags at +0xc for those, else 0.
unsigned int __cdecl t3dIsObjectEntryTemporary(S3dObject_guess *object)
{
    int objectType = object->type;

    switch (objectType)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
    case 0x1b:
    case 0x1c:
    case 0x1d:
    case 0x1e:
    case 0x1f:
    case 0x23:
    case 0x24:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x53:
        break;
    default:
        return 0;
    }

    return ((unsigned int)object->flags >> 1) & 1;
}

// FUNCTION: EQGFX 0x10017c28
// Reports the number of shares held on a rendering resource.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input). One object pointer:
// the type id at +0x0 selects shareable kinds. Returns the share count at +0x10, else 0.
// One return per kind (each kind's own count field, all at +0x10): gives the original's jump tables.
int __cdecl t3dGetObjectNumShares(int *sharedObject)
{
    switch (*sharedObject)
    {
    case 1: return sharedObject[4];
    case 3: return sharedObject[4];
    case 5: return sharedObject[4];
    case 6: return sharedObject[4];
    case 7: return sharedObject[4];
    case 8: return sharedObject[4];
    case 9: return sharedObject[4];
    case 10: return sharedObject[4];
    case 11: return sharedObject[4];
    case 12: return sharedObject[4];
    case 0x11: return sharedObject[4];
    case 0x12: return sharedObject[4];
    case 0x13: return sharedObject[4];
    case 0x14: return sharedObject[4];
    case 0x15: return sharedObject[4];
    case 0x16: return sharedObject[4];
    case 0x17: return sharedObject[4];
    case 0x1b: return sharedObject[4];
    case 0x1c: return sharedObject[4];
    case 0x1d: return sharedObject[4];
    case 0x45: return sharedObject[4];
    case 0x46: return sharedObject[4];
    case 0x47: return sharedObject[4];
    case 0x48: return sharedObject[4];
    case 0x49: return sharedObject[4];
    case 0x4a: return sharedObject[4];
    case 0x4c: return sharedObject[4];
    case 0x4d: return sharedObject[4];
    case 0x4e: return sharedObject[4];
    case 0x4f: return sharedObject[4];
    case 0x50: return sharedObject[4];
    case 0x51: return sharedObject[4];
    case 0x53: return sharedObject[4];
    }

    return 0;
}

// FUNCTION: EQGFX 0x10017ce8
// Retains a rendering resource for an additional user.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input). One object pointer:
// the type id at +0x0 selects shareable kinds, the share count at +0x10 is incremented on success.
// Returns 0 on success, 0xFFFF3D3E otherwise (callers ignore EAX).
// One increment per kind (each kind's own count field, all at +0x10): gives the original's jump tables.
int __cdecl t3dShareObject(int *sharedObject)
{
    int result = 0;

    switch (*sharedObject)
    {
    case 1: sharedObject[4]++; break;
    case 3: sharedObject[4]++; break;
    case 5: sharedObject[4]++; break;
    case 6: sharedObject[4]++; break;
    case 7: sharedObject[4]++; break;
    case 8: sharedObject[4]++; break;
    case 9: sharedObject[4]++; break;
    case 10: sharedObject[4]++; break;
    case 11: sharedObject[4]++; break;
    case 12: sharedObject[4]++; break;
    case 0x11: sharedObject[4]++; break;
    case 0x12: sharedObject[4]++; break;
    case 0x13: sharedObject[4]++; break;
    case 0x14: sharedObject[4]++; break;
    case 0x15: sharedObject[4]++; break;
    case 0x16: sharedObject[4]++; break;
    case 0x17: sharedObject[4]++; break;
    case 0x1b: sharedObject[4]++; break;
    case 0x1c: sharedObject[4]++; break;
    case 0x1d: sharedObject[4]++; break;
    case 0x45: sharedObject[4]++; break;
    case 0x46: sharedObject[4]++; break;
    case 0x47: sharedObject[4]++; break;
    case 0x48: sharedObject[4]++; break;
    case 0x49: sharedObject[4]++; break;
    case 0x4a: sharedObject[4]++; break;
    case 0x4c: sharedObject[4]++; break;
    case 0x4d: sharedObject[4]++; break;
    case 0x4e: sharedObject[4]++; break;
    case 0x4f: sharedObject[4]++; break;
    case 0x50: sharedObject[4]++; break;
    case 0x51: sharedObject[4]++; break;
    case 0x53: sharedObject[4]++; break;
    default:
        result = 0xFFFF3D3E;
        break;
    }

    return result;
}
// FUNCTION: EQGFX 0x10017dad
// Releases one shared reference to a rendering resource.
// __cdecl (the callee pops 0 bytes; ECX is loaded from the stack, not an input). One object pointer:
// the type id at +0x0 selects shareable kinds, the share count at +0x10 is decremented on success.
// Returns 0 on success, 0xFFFF3D3E otherwise (callers ignore EAX).
// One decrement per kind (each kind's own count field, all at +0x10): gives the original's jump tables.
int __cdecl t3dUnshareObject(int *sharedObject)
{
    int result = 0;

    switch (*sharedObject)
    {
    case 1: sharedObject[4]--; break;
    case 3: sharedObject[4]--; break;
    case 5: sharedObject[4]--; break;
    case 6: sharedObject[4]--; break;
    case 7: sharedObject[4]--; break;
    case 8: sharedObject[4]--; break;
    case 9: sharedObject[4]--; break;
    case 10: sharedObject[4]--; break;
    case 11: sharedObject[4]--; break;
    case 12: sharedObject[4]--; break;
    case 0x11: sharedObject[4]--; break;
    case 0x12: sharedObject[4]--; break;
    case 0x13: sharedObject[4]--; break;
    case 0x14: sharedObject[4]--; break;
    case 0x15: sharedObject[4]--; break;
    case 0x16: sharedObject[4]--; break;
    case 0x17: sharedObject[4]--; break;
    case 0x1b: sharedObject[4]--; break;
    case 0x1c: sharedObject[4]--; break;
    case 0x1d: sharedObject[4]--; break;
    case 0x45: sharedObject[4]--; break;
    case 0x46: sharedObject[4]--; break;
    case 0x47: sharedObject[4]--; break;
    case 0x48: sharedObject[4]--; break;
    case 0x49: sharedObject[4]--; break;
    case 0x4a: sharedObject[4]--; break;
    case 0x4c: sharedObject[4]--; break;
    case 0x4d: sharedObject[4]--; break;
    case 0x4e: sharedObject[4]--; break;
    case 0x4f: sharedObject[4]--; break;
    case 0x50: sharedObject[4]--; break;
    case 0x51: sharedObject[4]--; break;
    case 0x53: sharedObject[4]--; break;
    default:
        result = 0xFFFF3D3E;
        break;
    }

    return result;
}

#define S3D_POOL_TYPE_PALETTE 0x23
#define S3D_PALETTE_ENTRY_NOCOLLAPSE 0x04
#define EQGFX_GET_DC EQ_GLOBAL(S3dGetDC_guess_t, 0x10076268)
#define EQGFX_GET_SYSTEM_PALETTE_ENTRIES EQ_GLOBAL(S3dGetSystemPaletteEntries_guess_t, 0x10076054)
#define EQGFX_RELEASE_DC EQ_GLOBAL(S3dReleaseDC_guess_t, 0x1007626C)
#define EQGFX_LOAD_BITMAP_FILE EQ_CDECL(void *, 0x1004CC24, (const char *))
#define EQGFX_LOAD_FAST_FILE_BITMAP EQ_CDECL(void *, 0x1004CD32, (void *, const char *))
#define EQGFX_DIB_COLOR_COUNT EQ_CDECL(unsigned short, 0x1004CCEF, (void *))
#define EQGFX_GET_FIRST_VALUE EQ_CDECL(int, 0x1004CB7A, (void *))

// FUNCTION: EQGFX 0x10017e72
// Creates a bitmap color palette with optional use of system-reserved colors.
// __cdecl. New 0x42c-byte palette (type 0x23): the system palette overlaid with the bitmap's colour table. 0 on failure.
S3dPalette_guess *__cdecl t3dCreatePalette(void *dictionary, void *fastFile, const char *fileName, int allColors)
{
    S3dPalette_guess *palette;
    void *hdc;
    void *fileData;
    unsigned short colorDepth;
    int colorCount;
    int firstValue;
    S3dPaletteEntry_guess *entries;
    S3dPaletteEntry_guess *entry;
    S3dPaletteEntry_guess *entryEnd;
    unsigned char *colorTable;
    int registerResult;

    palette = (S3dPalette_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(S3dPalette_guess));
    if (palette == 0)
        return 0;
    palette->typeId = S3D_POOL_TYPE_PALETTE;
    palette->dwordAt08 = 0;
    palette->dwordAt0C = 0;
    palette->allColors = allColors;
    palette->dwordAt418 = 1;
    palette->dwordAt41C = 0;
    palette->dwordAt420 = 0;
    entries = palette->entries;
    hdc = EQGFX_GET_DC(0);
    EQGFX_GET_SYSTEM_PALETTE_ENTRIES(hdc, 0, 256, entries);
    EQGFX_RELEASE_DC(0, hdc);
    if (fastFile != 0)
        fileData = EQGFX_LOAD_FAST_FILE_BITMAP(fastFile, fileName);
    else
        fileData = EQGFX_LOAD_BITMAP_FILE(fileName);
    if (fileData != 0)
    {
        colorDepth = EQGFX_DIB_COLOR_COUNT(fileData);
        colorCount = colorDepth;
        if (colorCount == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(fileData);
            EQ_CDECL(void, 0x10050CBB, (void *))(palette);
            return 0;
        }
        palette->wordAt10 = 0x300;
        if (colorCount < 0x101)
            palette->colorCount = colorDepth;
        else
            palette->colorCount = 0x100;
        firstValue = EQGFX_GET_FIRST_VALUE(fileData);
        entries[0].red = 0;
        entries[0].green = 0;
        entries[0].blue = 0;
        entries[0].flags = 0;
        entry = &entries[1];
        if (allColors == 0)
        {
            for (entry = &entries[246]; entry < &entries[255]; entry++)
                entry->flags = 0;
            for (entry = &entries[1]; entry < &entries[10]; entry++)
                entry->flags = 0;
            if (colorCount > 0xec)
                colorCount = 0xec;
            colorTable = (unsigned char *)fileData + firstValue + 0x28;
        }
        else
        {
            if (colorCount > 0xfe)
                colorCount = 0xfe;
            colorTable = (unsigned char *)fileData + firstValue + 4;
        }
        entryEnd = entry + colorCount;
        for (; entry < entryEnd; entry++)
        {
            entry->red = colorTable[2];
            entry->green = colorTable[1];
            entry->blue = colorTable[0];
            entry->flags = S3D_PALETTE_ENTRY_NOCOLLAPSE;
            colorTable += 4;
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(fileData);
        entries[255].red = 0xff;
        entries[255].green = 0xff;
        entries[255].blue = 0xff;
        entries[255].flags = 0;
        if (dictionary == 0)
        {
            palette->dwordAt04 = -1;
            return palette;
        }
        registerResult = EQ_CDECL(int, 0x1001564B, (void *, S3dPalette_guess *))(dictionary, palette);
        if (registerResult >= 0)
            return palette;
    }
    EQ_CDECL(void, 0x10050CBB, (void *))(palette);
    return 0;
}
