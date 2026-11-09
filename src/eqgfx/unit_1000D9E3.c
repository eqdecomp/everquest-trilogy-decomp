// unit_1000D9E3.c - eqgfx_dx7.dll, the address run 0x1000D9E3..0x1001070B (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"
#include "trilogy/common.h"

#define S3D_DM_SPRITE_INSTANCE_FLAG_IN_DICTIONARY 0x01
#define S3D_DM_SPRITE_INSTANCE_FLAG_KEEP_MEMORY 0x20
#define T3D_COMPACT_DICTIONARY_NONE 0xffffffff
#define T3D_COMPACT_DICTIONARY_SLOT_BYTES 0x80
#define T3D_COMPACT_DICTIONARY_RECORD_COUNT 3
#define T3D_COMPACT_DICTIONARY_RECORD_BYTES 0x28
#define EQGFX_T3D_COMPACT_DICTIONARY_RELINK EQ_CDECL(int, 0x1000E1C9, (struct t3dCompactDictionary_guess *, u32))
#define EQGFX_T3D_COMPACT_DICTIONARY_MEMCPY EQ_CDECL(void *, 0x10067D30, (void *, const void *, u32))
#define EQGFX_MEMCPY EQ_CDECL(void *, 0x10067D30, (void *, const void *, u32))
#define EQGFX_T3D_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (int))
#define EQGFX_T3D_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (void *))
#define S3D_DM_SPRITE2_FLAG_VECTOR_GIVEN 0x80
#define S3D_DM_SPRITE2_FLAG_BOUNDS_GIVEN 0x40

/* Vector3_guess, Vector3Pair_guess, t3dDMExtent_guess: now in eqgfx/layout/gfx_types.h. */
#include "eqgfx/layout/gfx_types.h"

typedef struct t3dCompactDictionaryNode_guess
{
    u32 next;
    u32 bucket_index;
    u32 records[T3D_COMPACT_DICTIONARY_RECORD_COUNT][10];
} t3dCompactDictionaryNode_guess;

typedef struct t3dCompactDictionary_guess
{
    u32 initialized;
    u32 entry_count;
    u32 unknown_08;
    u32 *entry_heads;
    u32 unknown_10[3];
    u32 compacted_count;
    u32 slot_count;
    u32 block_count;
    u32 unknown_28;
    t3dCompactDictionaryNode_guess **blocks;
    u32 free_head;
} t3dCompactDictionary_guess;

typedef struct t3dCompactDictionaryOwner_guess
{
    unsigned char unknown_00[0x30];
    t3dCompactDictionary_guess *dictionary;
} t3dCompactDictionaryOwner_guess;

typedef struct DMSpritePrimaryData_guess
{
    unsigned char unknown_00[4];
    void *allocation_04;
    unsigned char unknown_08[4];
    void *allocation_0c;
    unsigned char unknown_10[4];
    void *allocation_14;
    unsigned char unknown_18[4];
    void *allocation_1c;
    unsigned char unknown_20[8];
    void *allocation_28;
    void *allocation_2c;
    unsigned char unknown_30[4];
    void *volume_34;
    unsigned char unknown_38[0x10];
    void *allocation_48;
    unsigned char unknown_4c[4];
    void *allocation_50;
    unsigned char unknown_54[4];
    void *allocation_58;
    unsigned char unknown_5c[4];
    void *allocation_60;
    unsigned char unknown_64[0x10];
    void *materialPalette_74;
    void *rgbTrack_78;
    void *rgbTrack_7c;
    void *allocation_80;
    unsigned char unknown_84[0x20];
    void *allocation_a4;
} DMSpritePrimaryData_guess;

typedef struct DMSpriteAlternateData_guess
{
    unsigned char unknown_00[0x48];
    void *materialPalette_48;
    void *rgbTrack_4c;
    void *rgbTrack_50;
    void *volume_54;
} DMSpriteAlternateData_guess;

typedef struct DMSpriteUndefineInstance_guess
{
    unsigned char unknown_00[8];
    void *dictionaryEntry;
    unsigned char flags;
    unsigned char unknown_0d[3];
    int useCount;
    DMSpritePrimaryData_guess *primaryData;
    DMSpriteAlternateData_guess *alternateData;
} DMSpriteUndefineInstance_guess;

typedef struct VertexVisibilityVertexRecord_guess
{
    u8 flags;
    u8 unknown_01;
    u16 firstVertex;
    u16 secondVertex;
    u16 thirdVertex;
    u8 unknown_08[8];
} VertexVisibilityVertexRecord_guess;

typedef struct VertexVisibilityFrameRecord_guess
{
    u8 flags;
    u8 unknown_01;
    u16 firstVertex;
    u16 secondVertex;
    u16 thirdVertex;
} VertexVisibilityFrameRecord_guess;

typedef struct VertexVisibilityVertexSet_guess
{
    u32 visibilitySize;
    u8 unknown_04[0x1c];
    u32 recordCount;
    u8 *recordData;
    u8 unknown_28[0x58];
    u8 *visibilityBuffer;
} VertexVisibilityVertexSet_guess;

typedef struct VertexVisibilityFrameSet_guess
{
    u16 visibilitySize;
    u8 unknown_02[6];
    u16 recordCount;
    u8 unknown_0a[0x0e];
    VertexVisibilityFrameRecord_guess *records;
    u8 unknown_1c[0x5c];
    u8 *visibilityBuffer;
} VertexVisibilityFrameSet_guess;

typedef struct VertexVisibilityOwner_guess
{
    u8 unknown_00[0x14];
    VertexVisibilityVertexSet_guess *vertexSet;
    VertexVisibilityFrameSet_guess *frameSet;
} VertexVisibilityOwner_guess;

// The t3d dictionary layout _t3dFreeDictionaryEntry_guess works on (no include/eqgfx header exists; the offsets come from
// the asm). A key is an entry index: block = key >> 5, slot = key & 0x1f, entries are 0x80 bytes, the
// bucket chains and the free list are singly linked through entry +0, entry +4 is the bucket (-1 = free).
#define T3D_NO_ENTRY 0xffffffff

// Returned for an out-of-range component index: the address of a writable global.
#define T3D_OUT_OF_RANGE_SPRITE EQ_ARRAY(u32, 0x100F401C)
#define T3D_DM_SPRITE_TYPE 0x48
#define T3D_DM_SPRITE_ALLOC_SIZE 0x38

// One name item of an entry (t3dRemoveEntryFromDictionary stores three per entry).
typedef struct t3dDictItem_guess
{
    u32 hash;                // 0x00: _t3dHashDictionaryName_guess of the name
    char name[0x20];         // 0x04: the name
    u32 typeIndex;           // 0x24: resource type << 24 | index into that type's object list
} t3dDictItem_guess;

typedef struct t3dDictEntry_guess
{
    u32 next;                // 0x00: next index in the bucket chain (or in the free list)
    int bucket;              // 0x04: bucket index, -1 while the entry is free
    t3dDictItem_guess items[3];     // 0x08: three name items, 0x80 bytes in all
} t3dDictEntry_guess;

typedef struct t3dDictBlock_guess
{
    t3dDictEntry_guess entries[32];     // 0x1000: one lazily allocated block of 32 entries
} t3dDictBlock_guess;

typedef struct t3dDictionary_guess
{
    int itemCount;                      // 0x00: live name items
    u32 bucketCount;                    // 0x04: buckets in use
    u32 bucketCapacity;                 // 0x08: entries allocated in `buckets`
    u32 *buckets;                       // 0x0c: bucket chain heads (entry indices)
    u32 hashMaskA;                      // 0x10: hash mask tried first
    u32 hashMaskB;                      // 0x14: mask used while the first slot is below 0x18
    u32 maskLimit;                      // 0x18
    int usedCount;                      // 0x1c: entries in use
    u32 topIndex;                       // 0x20: one past the highest used entry index
    u32 blockCount;                     // 0x24: entries in `blocks`
    u32 unknown_28;                     // 0x28
    t3dDictBlock_guess **blocks;        // 0x2c
    u32 freeHead;                       // 0x30: free-list head (entry index), -1 when empty
} t3dDictionary_guess;

// What t3dAddEntryToDictionary files under a name (the offsets come from its asm).
typedef struct t3dDictEntryDesc_guess
{
    int type;                           // 0x00: resource type (the item's top byte)
    u32 index;                          // 0x04: index into that type's object list
    char *name;                         // 0x08
    u32 flags;                          // 0x0c: bit 0 = already in the dictionary
} t3dDictEntryDesc_guess;

#define T3D_DICT_ENTRY(dict, key) (&(dict)->blocks[(key) >> 5]->entries[(key) & 0x1f])
#define T3D_DICT_DESC_IN_DICTIONARY 0x01
#define T3D_DICTIONARY_LOAD_FACTOR EQ_GLOBAL(float, 0x10076298)
#define T3D_DICTIONARY_MISSING (-1)
#define T3D_DICTIONARY_BAD_TYPE (-2)
#define T3D_DICTIONARY_ALREADY_ADDED (-3)
#define T3D_DICTIONARY_NO_BUCKET_ENTRY (-4)
#define T3D_DICTIONARY_NO_CHAIN_ENTRY (-5)

// The world object t3dRemoveEntryFromDictionary hangs off (no include/eqgfx header covers it; the offsets
// come from its switch). +0x24 and 0x5c..0x30c are per-resource-type lists of object pointers, 0x10 bytes
// apart; an item's low 24 bits index into the list its type picks.
typedef struct t3dObjectList_guess
{
    void **objects;                     // 0x00
    u32 unknown_04[3];                  // 0x04
} t3dObjectList_guess;                  // 0x10

// One 0xa0-byte component record of a composite sprite (no include/eqgfx header covers it; the stride is from the asm).
typedef struct t3dCompositeComponent_guess
{
    void *sprite;                       // 0x00: the component's sprite
    u8 unknown_04[0x9c];                // 0x04
} t3dCompositeComponent_guess;          // 0xa0

typedef struct t3dCompositeDefinition_guess
{
    u8 unknown_00[0x18];                // 0x00
    u32 componentCount;                 // 0x18
} t3dCompositeDefinition_guess;

// The composite sprite t3dGetCompositeSpriteComponentSprite reads (no include/eqgfx header covers it; the offsets
// come from its asm).
typedef struct t3dCompositeSprite_guess
{
    u8 unknown_00[0x28];                                // 0x00
    t3dCompositeComponent_guess *components;            // 0x28: 0xa0-byte component records
    u8 unknown_2c[0xc];                                 // 0x2c
    t3dCompositeDefinition_guess *definition;           // 0x38
} t3dCompositeSprite_guess;

typedef struct t3dWorld_guess
{
    u8 unknown_00[0x24];                // 0x00
    void **objects_24;                  // 0x24: type 25's objects
    u32 unknown_28[2];                  // 0x28
    t3dDictionary_guess *dictionary;    // 0x30: the name dictionary
    u32 unknown_34[0xa];                // 0x34
    t3dObjectList_guess lists_5c[25];   // 0x5c
    u32 unknown_1ec[0xc];               // 0x1ec
    t3dObjectList_guess lists_21c[16];  // 0x21c
    u32 unknown_31c[0x16];              // 0x31c
    t3dObjectList_guess list_374;       // 0x374
} t3dWorld_guess;                       // 0x384

// One 0xa0-byte component record of a composite sprite (no include/ header covers it; the offsets come from
// this function's asm): the component's sprite pointer sits at +0x00, the rest is unknown.
typedef struct t3dCompositeSpriteComponent_guess
{
    int *sprite;                        // 0x00: shared/unshared like every t3d object
    u8 unknown_04[0x9c];                // 0x04
} t3dCompositeSpriteComponent_guess;    // 0xa0

// The composite sprite definition a composite sprite points at from +0x38 (no include/ header covers it):
// only the component count at +0x18, the bound for a component index, is known.
typedef struct t3dCompositeSpriteDefinition_guess
{
    u8 unknown_00[0x18];                // 0x00
    u32 componentCount;                 // 0x18: bounds the component index
} t3dCompositeSpriteDefinition_guess;

// The composite sprite t3dSetCompositeSpriteComponentSprite sets a component on (no include/ header covers
// it): the array of 0xa0-byte component records at +0x28 and the shared definition at +0x38.
typedef struct t3dCompositeSpriteSet_guess
{
    u8 unknown_00[0x28];                            // 0x00
    t3dCompositeSpriteComponent_guess *components;  // 0x28
    u8 unknown_2c[0xc];                             // 0x2c
    t3dCompositeSpriteDefinition_guess *definition; // 0x38
} t3dCompositeSpriteSet_guess;

// The volume t3dDuplicateVolume and t3dDestroyVolume work on (no include/ header covers it; the type tag at +0x00 is
// the only field, and t3dDuplicateVolume switches on it).
typedef struct RenderVolume_guess
{
    int type;                           // 0x00
} RenderVolume_guess;

// The 2D sprite t3dSet2DSpriteCollisionVolume sets the collision volume of (no include/ header covers it; the offset
// comes from this function's asm): the volume pointer sits at +0x14, the rest is unknown.
typedef struct t3d2DSprite_guess
{
    u8 unknown_00[0x14];                // 0x00
    RenderVolume_guess *collisionVolume;   // 0x14
} t3d2DSprite_guess;

// The empty component sprite the original falls back to for a null one (the .data object at 0x100f401c).
#define T3D_EMPTY_COMPONENT_SPRITE ((int *)0x100f401c)
// The 3D sprite tag a component sprite must carry for the share/unshare calls (12 = the 3D sprite object).
#define T3D_3D_SPRITE_TYPE 0x0c
#define T3D_COMPONENT_INDEX_ERROR 0xffff3d3e

// kernel32 DebugBreak through eqgfx_dx7.dll's import slot: the original caches the slot in a local
// (MOV EBX,[0x100760bc]) and calls it from both mistyped-component paths.
typedef void (__stdcall *t3dDebugBreak_guess_t)(void);
#define t3d_debug_break_guess EQ_GLOBAL(t3dDebugBreak_guess_t, 0x100760BC)

// This image's t3dUnshareObject / t3dShareObject (Ghidra import names), both __cdecl.
#define EQGFX_T3D_UNSHARE_OBJECT EQ_CDECL(void, 0x10017DAD, (int *))
#define EQGFX_T3D_SHARE_OBJECT EQ_CDECL(void, 0x10017CE8, (int *))

// This image's t3dDuplicateVolume and t3dDestroyVolume, both __cdecl with the context first.
#define EQGFX_T3D_DUPLICATE_VOLUME EQ_CDECL(RenderVolume_guess *, 0x1001D516, (void *, RenderVolume_guess *))
#define EQGFX_T3D_DESTROY_VOLUME EQ_CDECL(void, 0x1001D5A1, (void *, RenderVolume_guess *))
// Returned when the source volume can't be duplicated (the sprite keeps its old volume).
#define T3D_VOLUME_DUPLICATE_ERROR 0xffff3d41

// FUNCTION: EQGFX 0x1000d9e3
// Gets the sprite assigned to one component of a composite object.
// __cdecl. Returns the dword at the start of component componentIndex's 0xa0-byte record (its sprite), or
// T3D_OUT_OF_RANGE_SPRITE when componentIndex is not below the definition's componentCount at 0x18.
void *__cdecl t3dGetCompositeSpriteComponentSprite(t3dCompositeSprite_guess *composite, unsigned int componentIndex)
{
    if (componentIndex >= composite->definition->componentCount)
    {
        return T3D_OUT_OF_RANGE_SPRITE;
    }
    return composite->components[componentIndex].sprite;
}

// FUNCTION: EQGFX 0x1000da06
// Changes the sprite displayed by one component of a composite object.
// __cdecl. Sets component `componentIndex`'s sprite: unshares the old one, then stores the new one (the empty
// component at 0x100f401c when null) and shares it; both must be 3D sprites (type 0xc) or DebugBreak runs.
int __cdecl t3dSetCompositeSpriteComponentSprite(t3dCompositeSpriteSet_guess *compositeSprite, unsigned int componentIndex, int *componentSprite)
{
    t3dDebugBreak_guess_t debugBreak;
    int *oldSprite;
    int spriteType;
    int result;

    result = 0;
    if (componentIndex >= compositeSprite->definition->componentCount)
    {
        result = T3D_COMPONENT_INDEX_ERROR;
    }
    else
    {
        debugBreak = t3d_debug_break_guess;
        oldSprite = compositeSprite->components[componentIndex].sprite;
        spriteType = *oldSprite;
        if (spriteType != 0)
        {
            if (spriteType != T3D_3D_SPRITE_TYPE)
                debugBreak();
            else
                EQGFX_T3D_UNSHARE_OBJECT(oldSprite);
        }
        if (componentSprite == 0)
            componentSprite = T3D_EMPTY_COMPONENT_SPRITE;
        compositeSprite->components[componentIndex].sprite = componentSprite;
        spriteType = *componentSprite;
        if (spriteType != 0)
        {
            if (spriteType != T3D_3D_SPRITE_TYPE)
                debugBreak();
            else
                EQGFX_T3D_SHARE_OBJECT(componentSprite);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000da80
// Sets the collision shape used by a 2D sprite.
// __cdecl. Replaces the sprite's collision volume with a duplicate of sourceVolume: when the duplicate fails it returns
// T3D_VOLUME_DUPLICATE_ERROR and changes nothing; else it destroys the old volume, stores the duplicate and returns 0.
int __cdecl t3dSet2DSpriteCollisionVolume(void *context, t3d2DSprite_guess *sprite, RenderVolume_guess *sourceVolume)
{
    RenderVolume_guess *duplicate;

    duplicate = EQGFX_T3D_DUPLICATE_VOLUME(context, sourceVolume);
    if (duplicate == 0)
    {
        return T3D_VOLUME_DUPLICATE_ERROR;
    }
    EQGFX_T3D_DESTROY_VOLUME(context, sprite->collisionVolume);
    sprite->collisionVolume = duplicate;
    return 0;
}

// FUNCTION: EQGFX 0x1000dab8
// Initializes a world's registry for finding rendering resources by name.
// __cdecl. Creates the name dictionary at world->0x30 unless one exists: the 0x34-byte header, the block array, block 0
// (32 free entries) and the 0x40-byte bucket array. Returns 0; -1 when one exists, else -2..-5 for the failed allocation.
int __cdecl t3dCreateDictionary(t3dWorld_guess *world)
{
    t3dDictionary_guess *dict;
    t3dDictBlock_guess **blocks;
    t3dDictEntry_guess *entry;
    u32 index;
    u32 bucketOffset;
    int result;

    result = 0;
    if (world->dictionary != 0)
    {
        result = -1;
    }
    else
    {
        dict = (t3dDictionary_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(t3dDictionary_guess));
        if (dict == 0)
        {
            result = -2;
        }
        else
        {
            dict->itemCount = 0;
            dict->blockCount = 1;
            dict->unknown_28 = 1;
            dict->blocks = (t3dDictBlock_guess **)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(t3dDictBlock_guess *));
            if (dict->blocks == 0)
            {
                result = -3;
                EQ_CDECL(void, 0x10050CBB, (void *))(dict);
            }
            else
            {
                dict->blocks[0] = (t3dDictBlock_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(sizeof(t3dDictBlock_guess));
                blocks = dict->blocks;
                if (blocks[0] == 0)
                {
                    result = -4;
                    EQ_CDECL(void, 0x10050CBB, (void *))(blocks);
                    EQ_CDECL(void, 0x10050CBB, (void *))(dict);
                }
                else
                {
                    dict->usedCount = 0;
                    dict->topIndex = 0;
                    dict->freeHead = 0;
                    entry = blocks[0]->entries;
                    index = 0;
                    do
                    {
                        entry->bucket = T3D_NO_ENTRY;
                        index++;
                        entry->next = index;
                        entry++;
                    } while (index < 0x20);
                    dict->bucketCount = 0x10;
                    dict->bucketCapacity = 0x10;
                    dict->buckets = (u32 *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x40);
                    if (dict->buckets == 0)
                    {
                        result = -5;
                        EQ_CDECL(void, 0x10050CBB, (void *))(dict->blocks[0]);
                        EQ_CDECL(void, 0x10050CBB, (void *))(dict->blocks);
                        EQ_CDECL(void, 0x10050CBB, (void *))(dict);
                    }
                    else
                    {
                        bucketOffset = 0;
                        do
                        {
                            *(u32 *)((u8 *)dict->buckets + bucketOffset) = T3D_NO_ENTRY;
                            bucketOffset += 4;
                        } while (bucketOffset < 0x40);
                        dict->hashMaskA = 0xf;
                        dict->hashMaskB = 0x1f;
                        dict->maskLimit = 0;
                        world->dictionary = dict;
                    }
                }
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000dbbe
// Releases the world's object-name lookup data.
// __cdecl. Frees the name dictionary at world->0x30 through t3dFreeMemory (0x10050cbb): its blocks via the
// array at 0x2c, that array, the bucket array at 0xc and the dictionary itself; clears the field, returns 0.
int __cdecl t3dDestroyDictionary(t3dWorld_guess *world)
{
    t3dDictionary_guess *dict;
    u32 index;

    dict = world->dictionary;
    if (dict != 0)
    {
        index = 0;
        if (dict->blockCount > 0)
        {
            do
            {
                EQ_CDECL(void, 0x10050CBB, (void *))(dict->blocks[index]);
                index++;
            } while (index < dict->blockCount);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(dict->blocks);
        EQ_CDECL(void, 0x10050CBB, (void *))(dict->buckets);
        EQ_CDECL(void, 0x10050CBB, (void *))(dict);
        world->dictionary = 0;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000dc08
// Files a newly defined resource (sprite, track, light, material) under its name in the world's dictionary.
// __cdecl. Files desc's name as {type << 24 | index} in the world's dictionary, then splits buckets (linear
// hashing) while items >= buckets * 2.85; 0 also when the name exists or a split fails, < 0 on errors.
int __cdecl t3dAddEntryToDictionary(t3dWorld_guess *world, t3dDictEntryDesc_guess *desc)
{
    t3dDictionary_guess *dict;
    t3dDictEntry_guess *entry;
    t3dDictEntry_guess *newEntry;
    t3dDictItem_guess *item;
    t3dDictItem_guess *dest;
    t3dDictItem_guess *shift;
    u32 *flags;
    u32 *buckets;
    char *name;
    u32 index;
    u32 hash;
    u32 bucket;
    u32 key;
    u32 next;
    u32 newKey;
    u32 bucketCount;
    u32 capacity;
    u32 growth;
    u32 mask;
    u32 position;
    u32 shiftPosition;
    int movedCount;
    int type;
    int result;

    result = 0;
    dict = world->dictionary;
    if (dict == 0)
    {
        result = T3D_DICTIONARY_MISSING;
        goto done;
    }
    type = desc->type;
    switch (type)
    {
    case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07: case 0x08:
    case 0x09: case 0x0a: case 0x0b: case 0x0c:
    case 0x11: case 0x12: case 0x13: case 0x14: case 0x15: case 0x16: case 0x17: case 0x18:
    case 0x19: case 0x1a: case 0x1b: case 0x1c: case 0x1d: case 0x1e: case 0x1f:
    case 0x23: case 0x24:
    case 0x44: case 0x45: case 0x46: case 0x47: case 0x48: case 0x49: case 0x4a:
    case 0x4c: case 0x4d: case 0x4e: case 0x4f: case 0x50: case 0x51:
    case 0x53:
        name = desc->name;
        index = desc->index;
        flags = &desc->flags;
        if (EQ_CDECL(void *, 0x1000E7FE, (t3dWorld_guess *, char *))(world, name) != 0)
            goto done;
        if (*flags & T3D_DICT_DESC_IN_DICTIONARY)
        {
            result = T3D_DICTIONARY_ALREADY_ADDED;
            goto done;
        }
        hash = EQ_CDECL(u32, 0x1000E0CE, (char *))(name);
        bucket = hash & dict->hashMaskA;
        if (bucket < dict->maskLimit)
            bucket = dict->hashMaskB & hash;
        key = dict->buckets[bucket];
        if (key == T3D_NO_ENTRY)
        {
            key = EQ_CDECL(u32, 0x1000E10D, (t3dDictionary_guess *))(dict);
            if (key == T3D_NO_ENTRY)
            {
                result = T3D_DICTIONARY_NO_BUCKET_ENTRY;
                goto done;
            }
            position = 0;
            dict->buckets[bucket] = key;
            entry = T3D_DICT_ENTRY(dict, key);
            entry->bucket = bucket;
        }
        else
        {
            do
            {
                entry = T3D_DICT_ENTRY(dict, key);
                position = 0;
                do
                {
                    if (entry->items[position].typeIndex == 0)
                        break;
                    position++;
                } while (position < 3);
                if (position < 3)
                    goto store;
                key = entry->next;
            } while (key != T3D_NO_ENTRY);
            key = EQ_CDECL(u32, 0x1000E10D, (t3dDictionary_guess *))(dict);
            if (key == T3D_NO_ENTRY)
            {
                result = T3D_DICTIONARY_NO_CHAIN_ENTRY;
                goto done;
            }
            position = 0;
            entry->next = key;
            entry = T3D_DICT_ENTRY(dict, key);
            entry->bucket = bucket;
        }
store:
        item = &entry->items[position];
        item->hash = hash;
        EQ_CDECL(char *, 0x100688E0, (char *, const char *, u32))(item->name, name, 0x20);
        item->name[0x1f] = 0;
        item->typeIndex = type << 24 | index;
        *flags |= T3D_DICT_DESC_IN_DICTIONARY;
        bucketCount = dict->bucketCount;
        dict->itemCount++;
        if ((u32)dict->itemCount < (u32)(bucketCount * T3D_DICTIONARY_LOAD_FACTOR))
            goto done;
        do
        {
            capacity = dict->bucketCapacity;
            if (bucketCount == capacity)
            {
                growth = capacity >> 2;
                if (growth == 0)
                    growth = 1;
                growth += capacity;
                buckets = (u32 *)EQ_CDECL(void *, 0x10050C8C, (void *, u32))(dict->buckets, growth * 4);
                if (buckets == 0)
                    goto done;
                dict->buckets = buckets;
                dict->bucketCapacity = growth;
            }
            newKey = T3D_NO_ENTRY;
            dict->buckets[dict->bucketCount] = T3D_NO_ENTRY;
            mask = dict->hashMaskB ^ dict->hashMaskA;
            dict->bucketCount++;
            key = dict->buckets[dict->maskLimit];
            if (key != T3D_NO_ENTRY)
            {
splitEntry:
                entry = T3D_DICT_ENTRY(dict, key);
                for (;;)
                {
                    position = 0;
                    item = entry->items;
                    do
                    {
                        if (item->typeIndex == 0)
                        {
                            if (position != 0)
                                goto nextInChain;
                            next = entry->next;
                            if (EQ_CDECL(int, 0x1000E1C9, (t3dDictionary_guess *, u32))(dict, key) != 0)
                                goto done;
                            key = next;
                            if (next == T3D_NO_ENTRY)
                                goto advanceSplit;
                            entry = T3D_DICT_ENTRY(dict, next);
                            goto nextItems;
                        }
                        if ((item->hash & mask) != 0)
                        {
                            if (newKey == T3D_NO_ENTRY)
                            {
                                newKey = EQ_CDECL(u32, 0x1000E10D, (t3dDictionary_guess *))(dict);
                                if (newKey == T3D_NO_ENTRY)
                                    goto done;
                                dict->buckets[dict->bucketCount - 1] = newKey;
                                goto newChainEntry;
                            }
                            else if (movedCount == 3)
                            {
                                newKey = EQ_CDECL(u32, 0x1000E10D, (t3dDictionary_guess *))(dict);
                                if (newKey == T3D_NO_ENTRY)
                                    goto done;
                                newEntry->next = newKey;
newChainEntry:
                                newEntry = T3D_DICT_ENTRY(dict, newKey);
                                movedCount = 0;
                                newEntry->bucket = dict->bucketCount - 1;
                                dest = newEntry->items;
                            }
                            EQGFX_MEMCPY(dest, item, sizeof(t3dDictItem_guess));
                            shiftPosition = position;
                            if (shiftPosition < 2)
                            {
                                shift = &entry->items[shiftPosition];
                                do
                                {
                                    EQGFX_MEMCPY(shift, shift + 1, sizeof(t3dDictItem_guess));
                                    if (shift->typeIndex == 0)
                                        break;
                                    shiftPosition++;
                                    shift++;
                                } while (shiftPosition < 2);
                            }
                            entry->items[2].typeIndex = 0;
                            movedCount++;
                            dest++;
                        }
                        else
                        {
                            item++;
                            position++;
                        }
                    } while (position < 3);
nextInChain:
                    if (key == T3D_NO_ENTRY)
                        break;
                    key = entry->next;
                    if (key != T3D_NO_ENTRY)
                        goto splitEntry;
                    break;
nextItems:
                    ;
                }
            }
advanceSplit:
            dict->maskLimit++;
            if (dict->maskLimit > dict->hashMaskA)
            {
                dict->maskLimit = 0;
                dict->hashMaskA = dict->hashMaskB;
                dict->hashMaskB = dict->hashMaskB * 2 + 1;
            }
            bucketCount = dict->bucketCount;
        } while ((u32)dict->itemCount >= (u32)(bucketCount * T3D_DICTIONARY_LOAD_FACTOR));
        break;
    default:
        result = T3D_DICTIONARY_BAD_TYPE;
        break;
    }
done:
    return result;
}

// FUNCTION: EQGFX 0x1000e0ce
// Hashes a name for the world's object dictionary.
// __cdecl (the callee pops no arguments; ECX is not an input). The string hash of the t3d dictionary
// functions that call it (t3dAddEntryToDictionary, t3dRemoveEntryFromDictionary, t3dGetPointerFromDictionary):
// chars at even positions of `text` are weighted 0x80, chars at odd positions 1 (both sign-extended), summed
// into a 32-bit value; the sum's low 16 bits are squared and the product is rotated right by 8 bits (its low
// byte becomes the top byte, + the rest >> 8).
int __cdecl _t3dHashDictionaryName_guess(char *text)
{
    unsigned int temp;
    unsigned int hash = 0;
    for (; *text != '\0'; text = text + 2)
    {
        hash = hash + *text * 0x80;
        if (text[1] == '\0')
            break;
        hash += text[1];
    }
    temp = (hash & 0xffff);
    hash = temp * temp;
    return ((hash & 0xff) << 24) + (hash >> 8);
}

// FUNCTION: EQGFX 0x1000e10d
// Reserves space for additional resource names in the world's dictionary.
u32 __cdecl _t3dAllocateDictionaryEntry_guess(t3dDictionary_guess *dict)
{
    u32 entryIndex;
    u32 blockIndex;
    u32 blockCount;
    u32 growth;
    u32 nextEntryIndex;
    int remaining;
    t3dDictEntry_guess *entry;
    t3dDictEntry_guess *entryCursor;
    u32 *itemTypeIndex;

    entryIndex = dict->freeHead;
    blockIndex = entryIndex >> 5;
    if (blockIndex < dict->blockCount)
    {
        entry = dict->blocks[blockIndex]->entries;
        goto useEntry;
    }
    else
    {
        blockCount = dict->blockCount;
        if (dict->blockCount == dict->unknown_28)
        {
            growth = dict->unknown_28 >> 2;
            if (growth == 0)
            {
                growth = 1;
            }
            blockCount = growth + dict->unknown_28;
            entry = (t3dDictEntry_guess *)EQ_CDECL(void *, 0x10050C8C, (void *, u32))(
                dict->blocks, blockCount * 4);
            if (entry != 0)
            {
                dict->blocks = (t3dDictBlock_guess **)entry;
                dict->unknown_28 = blockCount;
                goto allocateBlock;
            }
        }
        else
        {
allocateBlock:
            entry = (t3dDictEntry_guess *)EQ_CDECL(void *, 0x10050C48, (u32))(0x1000);
            if (entry != 0)
            {
                dict->blocks[blockIndex] = (t3dDictBlock_guess *)entry;
                dict->blockCount++;
                remaining = 0x20;
                entryCursor = entry;
                nextEntryIndex = entryIndex;
                do
                {
                    entryCursor->bucket = -1;
                    nextEntryIndex++;
                    entryCursor->next = nextEntryIndex;
                    entryCursor++;
                    remaining--;
                } while (remaining != 0);
                goto useEntry;
            }
        }
        entryIndex = 0xffffffff;
    }
    return entryIndex;

useEntry:
    entry = entry + (entryIndex & 0x1f);
    blockIndex = entry->next;
    dict->usedCount++;
    dict->freeHead = blockIndex;
    if (dict->topIndex <= entryIndex)
    {
        dict->topIndex = entryIndex + 1;
    }
    entry->next = 0xffffffff;
    itemTypeIndex = &entry->items[0].typeIndex;
    remaining = 3;
    do
    {
        *itemTypeIndex = 0;
        itemTypeIndex = itemTypeIndex + 10;
        remaining--;
    } while (remaining != 0);
    return entryIndex;
}

// FUNCTION: EQGFX 0x1000e1c9
// Reclaims a dictionary entry and releases surplus storage for resource names.
// __cdecl. Unlinks `key` from its bucket chain and pushes its entry onto the free list; when key was the
// top entry the top index drops and trailing blocks go to t3dFreeMemory (0x10050cbb). Returns 0, or -1
// when the bucket chain ends before key.
int __cdecl _t3dFreeDictionaryEntry_guess(t3dDictionary_guess *dict, u32 key)
{
    t3dDictionary_guess *base;
    t3dDictEntry_guess *entry;
    t3dDictEntry_guess *prev;
    u32 *link;
    u32 index;
    u32 next;
    u32 top;
    u32 needed;
    u32 blockCount;
    u32 lastBlockStart;
    u32 block;
    int result;

    base = dict;
    result = 0;
    entry = &base->blocks[key >> 5]->entries[key & 0x1f];
    if (entry->bucket == -1)
        goto FreeEntry;
    prev = 0;
    link = &base->buckets[entry->bucket];
    index = *link;
    if (index != T3D_NO_ENTRY)
    {
        do
        {
            entry = &base->blocks[index >> 5]->entries[index & 0x1f];
            if (index == key)
                break;
            index = entry->next;
            prev = entry;
        } while (index != T3D_NO_ENTRY);
    }
    if (index == T3D_NO_ENTRY)
    {
        result = -1;
        return result;
    }
    if (prev == 0)
        *link = entry->next;
    else
        prev->next = entry->next;
FreeEntry:
    entry->next = base->freeHead;
    base->freeHead = key;
    entry->bucket = -1;
    top = base->topIndex;
    base->usedCount = base->usedCount - 1;
    top = top - 1;
    if (key != top)
        return result;
    base->topIndex = top;
    if (key != 0)
    {
        do
        {
            key = key - 1;
            if (base->blocks[key >> 5]->entries[key & 0x1f].bucket != -1)
                break;
            top = top - 1;
            base->topIndex = top;
        } while (key != 0);
    }
    needed = 0;
    if (base->topIndex != 0)
        needed = ((base->topIndex - 1) >> 5) + 1;
    while (needed < base->blockCount)
    {
        blockCount = base->blockCount;
        lastBlockStart = (blockCount + 0x7ffffff) << 5;     // first entry index of the top block
        index = base->freeHead;
        prev = 0;
        block = index >> 5;
        if (block < blockCount)
        {
            do
            {
                entry = &base->blocks[block]->entries[index & 0x1f];
                next = entry->next;
                if (index >= lastBlockStart)
                {
                    if (prev == 0)
                        base->freeHead = next;
                    else
                        prev->next = next;
                }
                else
                {
                    prev = entry;
                }
                index = next;
                block = next >> 5;
            } while (block < base->blockCount);
        }
        if (prev == 0)
            base->freeHead = (base->blockCount + 0x7ffffff) << 5;
        else
            prev->next = (base->blockCount + 0x7ffffff) << 5;
        EQ_CDECL(void, 0x10050CBB, (void *))(base->blocks[base->blockCount - 1]);
        base->blockCount = base->blockCount - 1;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000e334
// Unregisters a rendering resource's name without destroying the resource.
// __cdecl. Removes `name` from the dictionary the world keeps at +0x30. Each 0x80-byte entry holds three
// 0x28-byte items (hash, name, typeIndex); the matched item's type picks a per-type object list on the
// world and its low 24 bits mark that object unused (bit 0 at +0xc cleared). The items behind it shift
// down, the item count drops, and an entry whose first item went empty is unlinked through _t3dFreeDictionaryEntry_guess.
// Returns 0, -1 (no dictionary) or -2 (name not found, with the original's sprintf notice).
/* fake match: repeat the per-type clear in every case for VC6's arm-local ANDs. */
int __cdecl t3dRemoveEntryFromDictionary(t3dWorld_guess *world, char *name)
{
    char message[300];
    t3dDictionary_guess *dict;
    u32 hash;
    int result;
    t3dDictEntry_guess *entry;
    u32 index;
    u32 slot;
    u32 *itemHash;
    char *itemName;
    u32 i;
    u32 j;
    u32 typeIndex;
    int resourceType;
    int done;

    result = 0;
    dict = world->dictionary;
    if (dict == 0)
    {
        result = -1;
    }
    else
    {
        hash = EQ_CDECL(u32, 0x1000E0CE, (char *))(name);
        slot = dict->hashMaskA & hash;
        if (slot < dict->maskLimit)
            slot = dict->hashMaskB & hash;
        index = dict->buckets[slot];
        done = 0;
        while (index != T3D_NO_ENTRY)
        {
            entry = &dict->blocks[index >> 5]->entries[index & 0x1f];
            i = 0;
            itemHash = (u32 *)entry + 2;
            itemName = (char *)(itemHash + 1);
            do
            {
                if (((u32 *)itemName)[8] == 0)
                    break;
                if (*itemHash == hash &&
                    EQ_CDECL(int, 0x100689E0, (char *, char *, u32))(itemName, name, 0x1f) == 0)
                {
                    done = 1;
                    break;
                }
                i = i + 1;
                itemHash += 10;
                itemName += 0x28;
            } while (i < 3);
            if (done)
                break;
            index = entry->next;
        }
        if (!done)
        {
            EQ_CDECL(int, 0x10067992, (char *, const char *, char *))(message, EQ_ARRAY(char, 0x100864B0), name);
            result = -2;
        }
        else
        {
            typeIndex = itemHash[9];
            resourceType = typeIndex >> 24;
            switch (resourceType)
            {
            case 3:  ((u32 *)world->lists_5c[0].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 4:  ((u32 *)world->lists_5c[10].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 1:  ((u32 *)world->lists_5c[1].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 2:  ((u32 *)world->lists_5c[11].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 5:  ((u32 *)world->lists_5c[2].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 6:  ((u32 *)world->lists_5c[12].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 7:  ((u32 *)world->lists_5c[3].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 8:  ((u32 *)world->lists_5c[13].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 9:  ((u32 *)world->lists_5c[4].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 10: ((u32 *)world->lists_5c[14].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 11: ((u32 *)world->lists_5c[5].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 12: ((u32 *)world->lists_5c[15].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 17: ((u32 *)world->lists_5c[6].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 18: ((u32 *)world->lists_5c[16].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 19: ((u32 *)world->lists_5c[7].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 20: ((u32 *)world->lists_5c[17].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 21: ((u32 *)world->lists_21c[0].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 22: ((u32 *)world->lists_21c[1].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 23: ((u32 *)world->lists_5c[8].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 24: ((u32 *)world->lists_5c[18].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 25: ((u32 *)world->objects_24[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 26: ((u32 *)world->lists_5c[19].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 31: ((u32 *)world->lists_5c[24].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 30: ((u32 *)world->lists_5c[23].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 29: ((u32 *)world->lists_5c[22].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 28: ((u32 *)world->lists_5c[21].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 27: ((u32 *)world->lists_5c[20].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 35: ((u32 *)world->lists_21c[14].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 70: ((u32 *)world->lists_21c[5].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 69: ((u32 *)world->lists_21c[4].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 68: ((u32 *)world->lists_21c[3].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 36: ((u32 *)world->lists_21c[15].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 71: ((u32 *)world->lists_21c[6].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 76: ((u32 *)world->lists_5c[9].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 77: ((u32 *)world->lists_21c[2].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 72: ((u32 *)world->lists_21c[7].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 73: ((u32 *)world->lists_21c[8].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 74: ((u32 *)world->lists_21c[9].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 78: ((u32 *)world->lists_21c[10].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 79: ((u32 *)world->lists_21c[11].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 80: ((u32 *)world->lists_21c[12].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 81: ((u32 *)world->lists_21c[13].objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            case 83: ((u32 *)world->list_374.objects[typeIndex & 0xffffff])[3] &= 0xfffffffe; break;
            }
            for (j = i; j < 2; j++)
            {
                EQ_CDECL(void *, 0x10067D30, (void *, void *, u32))
                    (&entry->items[j], &entry->items[j + 1], 0x28);
                if (entry->items[j].typeIndex == 0)
                    break;
            }
            entry->items[2].typeIndex = 0;
            dict->itemCount = dict->itemCount - 1;
            if (entry->items[0].typeIndex == 0)
                EQ_CDECL(int, 0x1000E1C9, (t3dDictionary_guess *, u32))(dict, index);
        }
    }
    return result;
}

// The name/type pair at the centre of one 0x28-byte item, seen from its name (item +0x04).
typedef struct t3dDictName_guess
{
    char name[0x20];                    // 0x00 (= item +0x04)
    u32 typeIndex;                      // 0x20 (= item +0x24)
    u32 pad_24;                         // 0x24: keeps the 0x28-byte item stride
} t3dDictName_guess;

// FUNCTION: EQGFX 0x1000e7fe
// Finds a world's rendering resource by its registered name.
// __cdecl. Looks `name` up in the world's dictionary (the hash table t3dRemoveEntryFromDictionary walks)
// and returns the object the matched item's {type << 24 | index} picks: the world's object list of that
// type, indexed by the low 24 bits. 0 when the dictionary, the name or the item's type is missing.
void *t3dGetPointerFromDictionary(t3dWorld_guess *world, char *name)
{
    u32 hash;
    u32 i;
    u32 *hashField;
    t3dDictName_guess *itemName;
    int found;
    t3dDictionary_guess *dict;
    t3dDictEntry_guess *entry;
    void *result;
    u32 slot;
    u32 index;
    u32 objectIndex;

    result = 0;
    dict = world->dictionary;
    if (dict != 0)
    {
        hash = EQ_CDECL(u32, 0x1000E0CE, (char *))(name);
        slot = dict->hashMaskA & hash;
        if (slot < dict->maskLimit)
            slot = dict->hashMaskB & hash;
        found = 0;
        index = dict->buckets[slot];
        while (index != T3D_NO_ENTRY)
        {
            entry = &dict->blocks[index >> 5]->entries[index & 0x1f];
            hashField = &entry->items[0].hash;
            itemName = (t3dDictName_guess *)(hashField + 1);
            for (i = 0; i < 3; i++, itemName++, hashField += 10)
            {
                if (itemName->typeIndex == 0)
                    break;
                if (*hashField == hash &&
                    EQ_CDECL(int, 0x10068A20, (char *, char *))(itemName->name, name) == 0)
                {
                    found = 1;
                    break;
                }
            }
            index = entry->next;
            if (found != 0)
            {
                objectIndex = hashField[9] & 0xffffff;
                switch ((int)(hashField[9] >> 24))
                {
                case 3:  result = world->lists_5c[0].objects[objectIndex]; break;
                case 4:  result = world->lists_5c[10].objects[objectIndex]; break;
                case 1:  result = world->lists_5c[1].objects[objectIndex]; break;
                case 2:  result = world->lists_5c[11].objects[objectIndex]; break;
                case 5:  result = world->lists_5c[2].objects[objectIndex]; break;
                case 6:  result = world->lists_5c[12].objects[objectIndex]; break;
                case 7:  result = world->lists_5c[3].objects[objectIndex]; break;
                case 8:  result = world->lists_5c[13].objects[objectIndex]; break;
                case 9:  result = world->lists_5c[4].objects[objectIndex]; break;
                case 10: result = world->lists_5c[14].objects[objectIndex]; break;
                case 11: result = world->lists_5c[5].objects[objectIndex]; break;
                case 12: result = world->lists_5c[15].objects[objectIndex]; break;
                case 17: result = world->lists_5c[6].objects[objectIndex]; break;
                case 18: result = world->lists_5c[16].objects[objectIndex]; break;
                case 19: result = world->lists_5c[7].objects[objectIndex]; break;
                case 20: result = world->lists_5c[17].objects[objectIndex]; break;
                case 21: result = world->lists_21c[0].objects[objectIndex]; break;
                case 22: result = world->lists_21c[1].objects[objectIndex]; break;
                case 23: result = world->lists_5c[8].objects[objectIndex]; break;
                case 24: result = world->lists_5c[18].objects[objectIndex]; break;
                case 25: result = world->objects_24[objectIndex]; break;
                case 26: result = world->lists_5c[19].objects[objectIndex]; break;
                case 31: result = world->lists_5c[24].objects[objectIndex]; break;
                case 30: result = world->lists_5c[23].objects[objectIndex]; break;
                case 29: result = world->lists_5c[22].objects[objectIndex]; break;
                case 28: result = world->lists_5c[21].objects[objectIndex]; break;
                case 27: result = world->lists_5c[20].objects[objectIndex]; break;
                case 35: result = world->lists_21c[14].objects[objectIndex]; break;
                case 70: result = world->lists_21c[5].objects[objectIndex]; break;
                case 69: result = world->lists_21c[4].objects[objectIndex]; break;
                case 68: result = world->lists_21c[3].objects[objectIndex]; break;
                case 36: result = world->lists_21c[15].objects[objectIndex]; break;
                case 71: result = world->lists_21c[6].objects[objectIndex]; break;
                case 76: result = world->lists_5c[9].objects[objectIndex]; break;
                case 77: result = world->lists_21c[2].objects[objectIndex]; break;
                case 72: result = world->lists_21c[7].objects[objectIndex]; break;
                case 73: result = world->lists_21c[8].objects[objectIndex]; break;
                case 74: result = world->lists_21c[9].objects[objectIndex]; break;
                case 78: result = world->lists_21c[10].objects[objectIndex]; break;
                case 79: result = world->lists_21c[11].objects[objectIndex]; break;
                case 80: result = world->lists_21c[12].objects[objectIndex]; break;
                case 81: result = world->lists_21c[13].objects[objectIndex]; break;
                case 83: result = world->list_374.objects[objectIndex]; break;
                }
                break;
            }
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000EB7E
// Compacts the world's name dictionary: refills sparse bucket chains, then moves top entries into freed slots.
int t3dCompactDictionary(t3dCompactDictionaryOwner_guess *owner)
{
    t3dCompactDictionary_guess *dictionary;
    t3dCompactDictionaryNode_guess *node;
    t3dCompactDictionaryNode_guess *previousNode;
    t3dCompactDictionaryNode_guess *sourceNode;
    t3dCompactDictionaryNode_guess *lastNode;
    u32 *entryLink;
    u32 result;
    u32 entryIndex;
    u32 key;
    u32 currentIndex;
    u32 selectedIndex;
    u32 recordIndex;
    u32 copiedRecords;
    u32 clearCount;
    u32 slotCount;
    u32 currentSlot;
    u32 lastSlot;
    u32 blockIndex;
    u32 slotOffset;
    u32 freeIndex;
    u32 isCurrentSlot;

    dictionary = owner->dictionary;
    result = 0;
    if (dictionary == 0)
    {
        result = T3D_COMPACT_DICTIONARY_NONE;
    }
    else if (dictionary->initialized != 0)
    {
        entryIndex = 0;
        if (dictionary->entry_count != 0)
        {
            do
            {
                previousNode = 0;
                key = dictionary->entry_heads[entryIndex];
                selectedIndex = T3D_COMPACT_DICTIONARY_NONE;
                node = 0;
                if (key != T3D_COMPACT_DICTIONARY_NONE)
                {
                    while (1)
                    {
                        previousNode = node;
                        currentIndex = key;
                        node = dictionary->blocks[currentIndex >> 5] + (currentIndex & 0x1f);
                        key = node->next;
                        if (key == T3D_COMPACT_DICTIONARY_NONE)
                            break;
                        if (node->records[2][9] == 0)
                        {
                            node->next = selectedIndex;
                            selectedIndex = currentIndex;
                            if (previousNode == 0)
                            {
                                dictionary->entry_heads[entryIndex] = key;
                                node = previousNode;
                            }
                            else
                            {
                                previousNode->next = key;
                                node = previousNode;
                            }
                        }
                    }
                    if (selectedIndex != T3D_COMPACT_DICTIONARY_NONE)
                    {
                        if (node->records[2][9] != 0)
                        {
                            node->next = selectedIndex;
                            node = dictionary->blocks[selectedIndex >> 5] + (selectedIndex & 0x1f);
                            selectedIndex = node->next;
                            if (selectedIndex == T3D_COMPACT_DICTIONARY_NONE)
                                goto compact_dictionary_entry_done;
                            node->next = T3D_COMPACT_DICTIONARY_NONE;
                        }
                        recordIndex = 2;
                        do
                        {
                            if (node->records[recordIndex - 1][9] != 0)
                                break;
                            recordIndex--;
                        } while (recordIndex != 0);
                        copiedRecords = 0;
                        sourceNode = dictionary->blocks[selectedIndex >> 5] + (selectedIndex & 0x1f);
                        while (1)
                        {
                            do
                            {
                                EQGFX_T3D_COMPACT_DICTIONARY_MEMCPY(
                                    &node->records[recordIndex][0],
                                    &sourceNode->records[copiedRecords][0],
                                    T3D_COMPACT_DICTIONARY_RECORD_BYTES);
                                copiedRecords++;
                                if (copiedRecords == T3D_COMPACT_DICTIONARY_RECORD_COUNT
                                    || sourceNode->records[copiedRecords][9] == 0)
                                {
                                    key = sourceNode->next;
                                    sourceNode->bucket_index = T3D_COMPACT_DICTIONARY_NONE;
                                    if (EQGFX_T3D_COMPACT_DICTIONARY_RELINK(dictionary, selectedIndex) != 0)
                                        result = 0xfffffffe;
                                    if (key == T3D_COMPACT_DICTIONARY_NONE)
                                        goto compact_dictionary_entry_done;
                                    sourceNode = dictionary->blocks[key >> 5] + (key & 0x1f);
                                    copiedRecords = 0;
                                    selectedIndex = key;
                                }
                                recordIndex++;
                            } while (recordIndex != T3D_COMPACT_DICTIONARY_RECORD_COUNT);
                            node->next = selectedIndex;
                            selectedIndex = sourceNode->next;
                            sourceNode->next = T3D_COMPACT_DICTIONARY_NONE;
                            if (copiedRecords != 0)
                            {
                                EQGFX_T3D_COMPACT_DICTIONARY_MEMCPY(
                                    &sourceNode->records[0][0],
                                    &sourceNode->records[copiedRecords][0],
                                    0x78 - copiedRecords * T3D_COMPACT_DICTIONARY_RECORD_BYTES);
                                copiedRecords = T3D_COMPACT_DICTIONARY_RECORD_COUNT - copiedRecords;
                                if (copiedRecords < T3D_COMPACT_DICTIONARY_RECORD_COUNT)
                                {
                                    recordIndex = copiedRecords;
                                    clearCount = T3D_COMPACT_DICTIONARY_RECORD_COUNT - copiedRecords;
                                    do
                                    {
                                        sourceNode->records[recordIndex][9] = 0;
                                        recordIndex++;
                                        clearCount--;
                                    } while (clearCount != 0);
                                }
                            }
                            if (selectedIndex == T3D_COMPACT_DICTIONARY_NONE)
                                break;
                            node = sourceNode;
                            recordIndex = 0;
                            do
                            {
                                if (node->records[recordIndex][9] == 0)
                                    break;
                                recordIndex++;
                            } while (recordIndex < T3D_COMPACT_DICTIONARY_RECORD_COUNT);
                            copiedRecords = 0;
                            sourceNode = dictionary->blocks[selectedIndex >> 5] + (selectedIndex & 0x1f);
                        }
                    }
                }
compact_dictionary_entry_done:
                entryIndex++;
            } while (entryIndex < dictionary->entry_count);
        }
        slotCount = dictionary->slot_count;
        lastSlot = slotCount - 1;
        lastNode = dictionary->blocks[lastSlot >> 5] + (lastSlot & 0x1f);
        currentSlot = 0;
        if (slotCount != 0)
        {
            do
            {
                blockIndex = currentSlot >> 5;
                slotOffset = currentSlot & 0x1f;
                node = dictionary->blocks[blockIndex] + slotOffset;
                if (node->bucket_index == T3D_COMPACT_DICTIONARY_NONE)
                {
                    entryLink = dictionary->entry_heads + lastNode->bucket_index;
                    while (*entryLink != lastSlot)
                    {
                        if (*entryLink == T3D_COMPACT_DICTIONARY_NONE)
                            goto compact_dictionary_find_entry_done;
                        node = dictionary->blocks[*entryLink >> 5] + (*entryLink & 0x1f);
                        entryLink = &node->next;
                    }
                    *entryLink = currentSlot;
compact_dictionary_find_entry_done:
                    freeIndex = dictionary->free_head;
                    previousNode = 0;
                    while ((freeIndex >> 5) < dictionary->block_count)
                    {
                        node = dictionary->blocks[freeIndex >> 5] + (freeIndex & 0x1f);
                        isCurrentSlot = freeIndex == currentSlot;
                        freeIndex = node->next;
                        if (isCurrentSlot)
                        {
                            if (previousNode == 0)
                                dictionary->free_head = freeIndex;
                            else
                                previousNode->next = freeIndex;
                        }
                        previousNode = node;
                    }
                    EQGFX_T3D_COMPACT_DICTIONARY_MEMCPY(
                        dictionary->blocks[blockIndex] + slotOffset,
                        lastNode,
                        T3D_COMPACT_DICTIONARY_SLOT_BYTES);
                    dictionary->compacted_count++;
                    lastNode->bucket_index = T3D_COMPACT_DICTIONARY_NONE;
                    if (EQGFX_T3D_COMPACT_DICTIONARY_RELINK(dictionary, lastSlot) != 0)
                        return 0xfffffffd;
                    slotCount = dictionary->slot_count;
                    lastSlot = slotCount - 1;
                    lastNode = dictionary->blocks[lastSlot >> 5] + (lastSlot & 0x1f);
                }
                currentSlot++;
            } while (currentSlot < slotCount);
        }
    }
    return result;
}

typedef struct t3dDMVertexFace_guess
{
    unsigned char unknown_00[8];
    u8 flags;                                    // 0x08
    u8 unknown_09;
    u16 vertex0;                                 // 0x0a
    u16 vertex1;                                 // 0x0c
    u16 vertex2;                                 // 0x0e
} t3dDMVertexFace_guess;

typedef struct t3dDMFacePlane_guess              // one 16-byte face plane s3dComputeDMSpriteDefPlanarEquations_guess fills
{
    Vector3_guess normal;                        // 0x00: unit face normal
    float distance;                              // 0x0c: -(normal . first corner)
} t3dDMFacePlane_guess;

// The vertex set behind a DM sprite definition's +0x14 (s3dDefineDMSprite builds it inline at +0x1c), with the
// 0xa8 bytes the definition holds. Fields after +0x44 are named from s3dDefineDMSprite's stores.
/* t3dDMVertexSet_guess, t3dDMSpriteFrameTable_guess, t3dDMSpriteDefinition_guess: generated in eqgfx/layout/t3d_dmsprite_def.h; the old local views' members map by offset. */
#include "eqgfx/layout/t3d_dmsprite_def.h"

// The frame table of the definition s3dInitializeDMSpriteFrameTable_guess fills (its caller s3dDefineDMSprite2 passes the
// definition it builds); the offsets come from the asm, no include/eqgfx header has this layout.

// FUNCTION: EQGFX 0x1000eed2
// Checks whether data is a valid DM sprite definition.
// __cdecl (callee pops nothing, ECX is not an input). A DM sprite definition is valid when its type is
// 0x47; returns 0 then and the 0xffff3d3e error code otherwise (a null definition still faults on the read).
int s3dIsDMSpriteDefinitionValid(t3dDMSpriteDefinition_guess *definition)
{
    int result;

    result = 0;
    if (definition == 0)
        result = 0xffff3d3e;
    if (definition->type != 0x47)
        result = 0xffff3d3e;
    return result;
}

#define S3D_DM_SPRITE_DEF_VECTOR_GIVEN_FLAG (S3D_DM_SPRITE2_FLAG_VECTOR_GIVEN << 16)
#define S3D_DM_SPRITE_DEF_BOUNDS_GIVEN_FLAG (S3D_DM_SPRITE2_FLAG_BOUNDS_GIVEN << 16)
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)

// FUNCTION: EQGFX 0x1000eeeb
// Defines a frameless DM sprite mesh from given vertex, UV, normal, color and face arrays, registered by name.
// __cdecl (thirty stack arguments; the callee pops nothing). Builds a DM sprite definition with its vertex set inline and
// copies the arrays it is given, then shares the objects it keeps and registers the name. Returns the definition, or 0 after freeing.
t3dDMSpriteDefinition_guess *__cdecl s3dDefineDMSprite(void *dictionary, char *name, int vertexCount, void *vertices,
                                                       int uvCount, void *uvs, int normalCount, void *normals,
                                                       int rgbColorCount, void *rgbColors, u32 reserved11, u32 reserved12,
                                                       int faceCount, void *faces, void *materialPalette, int entryCount,
                                                       void *entries, size_t groupCount, short *groups, u32 pieceCount,
                                                       short *pieces, int vectorCount, Vector3_guess *vectors, void *dmTrack,
                                                       void *dmRGBTrack, t3dDMExtent_guess *centerOffset,
                                                       Vector3_guess *givenVector, void *collisionVolume, float boundingRadius,
                                                       Vector3Pair_guess *bounds)
{
    t3dDMSpriteDefinition_guess *definition;
    t3dDMVertexSet_guess *vertexSet;
    void *allocation;
    size_t nameLength;
    size_t remaining;
    short start;
    int offset;
    u32 pieceIndex;
    int status;

    definition = EQGFX_T3D_ALLOCATE_MEMORY(0xc4);
    if (definition == 0)
        return 0;
    vertexSet = &definition->inlineVertexSet;
    definition->vertexSet = vertexSet;
    definition->frameTable = 0;
    definition->type = 0x47;
    definition->unk_0x04 = 0;
    definition->name = 0;
    definition->flags = 0;
    definition->unk_0x10 = 0;
    if (givenVector != 0)
        definition->flags = S3D_DM_SPRITE_DEF_VECTOR_GIVEN_FLAG;
    if (bounds != 0)
        definition->flags |= S3D_DM_SPRITE_DEF_BOUNDS_GIVEN_FLAG;
    vertexSet->count = vertexCount;
    vertexSet->vertices = 0;
    vertexSet->uvCount = uvCount;
    vertexSet->uvs = 0;
    vertexSet->normalCount = normalCount;
    vertexSet->normals = 0;
    vertexSet->rgbColorCount = rgbColorCount;
    vertexSet->rgbColors = 0;
    vertexSet->faceCount = faceCount;
    vertexSet->materialPalette = materialPalette;
    vertexSet->entryCount = entryCount;
    vertexSet->groupCount = groupCount;
    vertexSet->faceRecords = 0;
    vertexSet->faceBlock = 0;
    vertexSet->facePlanes = 0;
    vertexSet->entries = 0;
    vertexSet->groups = 0;
    vertexSet->pieceCount = 0;
    vertexSet->pieces = 0;
    vertexSet->vectorCount = vectorCount;
    vertexSet->vectors = 0;
    vertexSet->unk_0x64 = 0;
    vertexSet->centerOffset = *centerOffset;
    vertexSet->collisionVolume = collisionVolume;
    vertexSet->boundingRadius = boundingRadius;
    vertexSet->ownedBlock_guess = 0;
    vertexSet->dmTrack = dmTrack;
    vertexSet->dmRGBTrack = dmRGBTrack;
    vertexSet->unk_0x9C = 0;
    vertexSet->unk_0xA0 = 0;
    vertexSet->unk_0xA4 = 0;
    if (givenVector == 0)
    {
        vertexSet->givenVector.x = 0.0f;
        vertexSet->givenVector.y = 0.0f;
        vertexSet->givenVector.z = 0.0f;
    }
    else
    {
        vertexSet->givenVector = *givenVector;
    }
    if (bounds == 0)
    {
        vertexSet->min.x = 0.0f;
        vertexSet->min.y = 0.0f;
        vertexSet->min.z = 0.0f;
        vertexSet->max.x = 0.0f;
        vertexSet->max.y = 0.0f;
        vertexSet->max.z = 0.0f;
    }
    else
    {
        *(Vector3Pair_guess *)&vertexSet->min = *bounds;
    }
    if (name != 0)
    {
        nameLength = EQ_CDECL(u32, 0x10068160, (char *))(name);
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(nameLength + 1);
        definition->name = allocation;
        if (allocation == 0)
            goto cleanup;
        EQ_CDECL(void, 0x10068070, (char *, char *))(allocation, name);
    }
    if (vertexCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(vertexCount * 0xc);
        vertexSet->vertices = allocation;
        if (allocation == 0)
            goto cleanup;
        EQGFX_MEMCPY(allocation, vertices, vertexCount * 0xc);
        if ((definition->flags & S3D_DM_SPRITE_DEF_BOUNDS_GIVEN_FLAG) == 0)
            EQ_CDECL(void, 0x100100E5, (void *))(definition);
    }
    if (g_render_mode_guess != 0)
    {
        if (uvCount != 0)
        {
            allocation = EQGFX_T3D_ALLOCATE_MEMORY(uvCount << 3);
            vertexSet->uvs = allocation;
            if (allocation == 0)
                goto cleanup;
            EQGFX_MEMCPY(allocation, uvs, uvCount << 3);
        }
        if (g_render_mode_guess != 0)
        {
            if (normalCount != 0)
            {
                allocation = EQGFX_T3D_ALLOCATE_MEMORY(normalCount * 0xc);
                vertexSet->normals = allocation;
                if (allocation == 0)
                    goto cleanup;
                EQGFX_MEMCPY(allocation, normals, normalCount * 0xc);
            }
            if (g_render_mode_guess != 0 && rgbColorCount != 0)
            {
                allocation = EQGFX_T3D_ALLOCATE_MEMORY(rgbColorCount << 2);
                vertexSet->rgbColors = allocation;
                if (allocation == 0)
                    goto cleanup;
                EQGFX_MEMCPY(allocation, rgbColors, rgbColorCount << 2);
            }
        }
    }
    if (faceCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY((faceCount + 2) * 0x10);
        vertexSet->faceBlock = allocation;
        if (allocation == 0)
            goto cleanup;
        vertexSet->faceRecords = (t3dDMVertexFace_guess *)(((u32)allocation + 0x20) & 0xffffffe0);
        EQGFX_MEMCPY(vertexSet->faceRecords, faces, faceCount << 4);
    }
    if (entryCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(entryCount << 2);
        vertexSet->entries = allocation;
        if (allocation == 0)
            goto cleanup;
        EQGFX_MEMCPY(allocation, entries, entryCount << 2);
    }
    if (groupCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(groupCount * 6);
        vertexSet->groups = allocation;
        if (allocation == 0)
            goto cleanup;
        start = 0;
        if (groupCount != 0)
        {
            offset = 0;
            remaining = groupCount;
            do
            {
                *(short *)((char *)vertexSet->groups + offset) = start;
                *(short *)((char *)vertexSet->groups + offset + 2) = groups[0];
                *(short *)((char *)vertexSet->groups + offset + 4) = groups[1];
                start = start + groups[0];
                offset = offset + 6;
                groups = groups + 2;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
        EQ_CDECL(void, 0x10068AA4, (void *, size_t, size_t, int (__cdecl *)(const void *, const void *)))(
            vertexSet->groups, groupCount, 6, EQ_CDECL(int, 0x1000F95D, (const void *, const void *)));
    }
    if (pieceCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(pieceCount << 3);
        vertexSet->pieces = allocation;
        if (allocation == 0)
            goto cleanup;
        pieceIndex = 0;
        start = 0;
        if (pieceCount != 0)
        {
            do
            {
                offset = pieceIndex * 8;
                *(short *)((char *)vertexSet->pieces + offset) = start;
                *(u16 *)((char *)vertexSet->pieces + offset + 6) = 0;
                *(short *)((char *)vertexSet->pieces + offset + 4) = pieces[1];
                *(short *)((char *)vertexSet->pieces + offset + 2) = pieces[0];
                start = start + pieces[0];
                pieceIndex = pieceIndex + 1;
                pieces = pieces + 2;
            } while (pieceIndex < pieceCount);
        }
        vertexSet->pieceCount = pieceCount;
        EQ_CDECL(void, 0x10068AA4, (void *, size_t, size_t, int (__cdecl *)(const void *, const void *)))(
            vertexSet->pieces, pieceCount, 8, EQ_CDECL(int, 0x1000F95D, (const void *, const void *)));
    }
    if (vectorCount != 0)
    {
        allocation = EQGFX_T3D_ALLOCATE_MEMORY(vectorCount * 0xc);
        vertexSet->vectors = allocation;
        if (allocation == 0)
            goto cleanup;
        EQGFX_MEMCPY(allocation, vectors, vectorCount * 0xc);
    }
    status = EQ_CDECL(int, 0x1000FBBA, (void *))(definition);
    if (status == 0 && (status = EQ_CDECL(int, 0x1001564B, (void *, void *))(dictionary, definition)) == 0)
    {
        if (name == 0 || (status = EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, definition)) >= 0)
        {
            if (dmTrack != 0)
                EQGFX_T3D_SHARE_OBJECT(dmTrack);
            if (dmRGBTrack != 0)
                EQGFX_T3D_SHARE_OBJECT(dmRGBTrack);
            if (materialPalette != 0)
                EQGFX_T3D_SHARE_OBJECT(materialPalette);
            return definition;
        }
        EQ_CDECL(void, 0x10015D81, (void *, void *))(dictionary, definition);
    }
cleanup:
    if (definition->name != 0)
        EQGFX_T3D_FREE_MEMORY(definition->name);
    if (vertexSet != 0)
    {
        if (vertexSet->vertices != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->vertices);
        if (vertexSet->uvs != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->uvs);
        if (vertexSet->normals != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->normals);
        if (vertexSet->rgbColors != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->rgbColors);
        if (vertexSet->faceBlock != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->faceBlock);
        if (vertexSet->facePlanes != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->facePlanes);
        if (vertexSet->entries != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->entries);
        if (vertexSet->groups != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->groups);
        if (vertexSet->pieces != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->pieces);
        if (vertexSet->vectors != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->vectors);
        if (vertexSet->ownedBlock_guess != 0)
            EQGFX_T3D_FREE_MEMORY(vertexSet->ownedBlock_guess);
    }
    EQGFX_T3D_FREE_MEMORY(definition);
    return 0;
}

// FUNCTION: EQGFX 0x1000f40d
// Computes storage needed for a DM sprite's vertex and frame data.
// __cdecl (callee pops nothing, ECX is not an input). Returns the byte size of a DM sprite2 definition:
// 0x9c base plus 0x10 when any of the first four counts is set, then per-frame, per-entry and name bytes.
int s3dCalcDMSpriteDef2AllocSize(char *name, u16 count, u16 flag1, u16 flag2, u16 flag3,
                                 u16 frameCount, u16 entryCount, u16 entryCount2, u16 entryCount3,
                                 u16 vertexCount)
{
    int size;

    size = count * 0x14 + 0x9c;
    if (count != 0 || flag1 != 0 || flag2 != 0 || flag3 != 0)
        size += 0x10;
    size += frameCount * 8;
    if (frameCount != 0)
        size += 8;
    size += (entryCount + frameCount * 4 + entryCount2 + entryCount3) * 4 + vertexCount * 6;
    if (name != 0)
        size += EQ_CDECL(u32, 0x10068160, (char *))(name) + 1;
    if (vertexCount != 0 || entryCount != 0)
        size += frameCount + count;
    if (entryCount == 0)
        size += frameCount * 0x10;
    return size;
}

// FUNCTION: EQGFX 0x1000f4a5
// Defines a DM sprite, layout 2: 20-byte vertex records with 15-bit RGB, arrays copied; added to world by name
// __cdecl (thirty stack arguments; the callee pops nothing). Builds a DM sprite definition in existing or in a new
// allocation, then registers it. Returns the definition, or 0 when the allocation or the registration fails.
t3dDMSpriteDefinition_guess *__cdecl s3dDefineDMSprite2(void *context, t3dDMSpriteDefinition_guess *existing,
                                                        char *name, u32 count, u32 flag1, u32 flag2, u32 flag3,
                                                        u32 frameCount, u32 entryCount, u32 entryCount2,
                                                        u32 entryCount3, u32 vertexCount, u16 value2E,
                                                        int positionData, u32 *pairData, int colorData,
                                                        int packedData, void *frameData, void *entryData,
                                                        void *entryData2, void *entryData3, void *vertexBytes,
                                                        int *sharedA, int *sharedB, int *sharedC,
                                                        Vector3_guess *vector48, Vector3_guess *vector58,
                                                        u32 value70, u32 value74, Vector3Pair_guess *bounds)
{
    t3dDMSpriteDefinition_guess *definition;
    u32 *cursor;
    u16 *vertexOut;
    u16 *vertexIn;
    u32 *pairOut;
    u16 *colorOut;
    char *colorIn;
    u16 color;
    char channel;
    u8 *packedOut;
    u8 *packedIn;
    u8 *frameBytes;
    u32 remaining;
    u32 byteCount;

    definition = existing;
    if (existing == 0)
    {
        definition = EQGFX_T3D_ALLOCATE_MEMORY(
            EQ_CDECL(int, 0x1000F40D, (char *, u32, u32, u32, u32, u32, u32, u32, u32, u32, int))(
                name, count, flag1, flag2, flag3, frameCount, entryCount, entryCount2, entryCount3,
                vertexCount, sharedB != 0));
        if (definition == 0)
            return 0;
    }
    definition->type = 0x47;
    *(u32 *)((u8 *)definition + 0x04) = 0;
    *(u32 *)((u8 *)definition + 0x08) = 0;
    *(u32 *)((u8 *)definition + 0x0c) = 0;
    *(u32 *)((u8 *)definition + 0x10) = 0;
    if (existing != 0)
        *(u32 *)((u8 *)definition + 0x0c) |= S3D_DM_SPRITE_INSTANCE_FLAG_KEEP_MEMORY;
    *(u32 *)((u8 *)definition + 0x14) = 0;
    definition->frameTable = (t3dDMSpriteFrameTable_guess *)((u8 *)definition + 0x1c);
    if (vector58 != 0)
        *(u8 *)((u8 *)definition + 0x0e) |= S3D_DM_SPRITE2_FLAG_VECTOR_GIVEN;
    if (bounds != 0)
        *(u8 *)((u8 *)definition + 0x0e) |= S3D_DM_SPRITE2_FLAG_BOUNDS_GIVEN;
    cursor = (u32 *)definition + 0x27;
    if ((short)count == 0 && (short)flag1 == 0 && (short)flag2 == 0 && (short)flag3 == 0)
    {
        *(u32 *)((u8 *)definition + 0x30) = 0;
    }
    else
    {
        cursor = (u32 *)definition + 0x2b;
        *(u32 **)((u8 *)definition + 0x30) = cursor;
        if (((u32)cursor & 0xf) != 0)
            *(u32 **)((u8 *)definition + 0x30) = (u32 *)((u32)cursor & 0xfffffff0);
        cursor = cursor + (count & 0xffff) * 5;
    }
    definition->frameTableBody.vertexCount_guess = (u16)count;
    if ((short)count != 0)
    {
        vertexOut = *(u16 **)((u8 *)definition + 0x30);
        remaining = count & 0xffff;
        if (remaining != 0)
        {
            vertexIn = (u16 *)(positionData + 4);
            do
            {
                vertexOut[0] = vertexIn[-2];
                vertexOut[1] = vertexIn[-1];
                vertexOut[2] = vertexIn[0];
                vertexIn = vertexIn + 3;
                vertexOut = vertexOut + 10;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
    }
    definition->frameTableBody.uvCount = (u16)flag1;
    if ((short)flag1 != 0)
    {
        remaining = count & 0xffff;
        if (remaining != 0)
        {
            pairOut = (u32 *)(*(u32 *)((u8 *)definition + 0x30) + 10);
            do
            {
                pairOut[-1] = pairData[0];
                pairOut[0] = pairData[1];
                pairOut = pairOut + 5;
                remaining = remaining - 1;
                pairData = pairData + 2;
            } while (remaining != 0);
        }
    }
    definition->frameTableBody.normalCount = (u16)flag2;
    if ((short)flag2 != 0)
    {
        remaining = flag2 & 0xffff;
        if (remaining != 0)
        {
            colorOut = (u16 *)(*(u32 *)((u8 *)definition + 0x30) + 0xe);
            colorIn = (char *)(colorData + 2);
            do
            {
                color = (u16)(char)(colorIn[-2] >> 3 & 0x1f);
                *colorOut = color;
                color = (u16)((short)(char)(colorIn[-1] >> 3 & 0x1f) << 5 | color);
                *colorOut = color;
                channel = *colorIn;
                colorIn = colorIn + 3;
                *colorOut = (short)(char)(channel >> 3 & 0x1f) << 10 | color;
                colorOut = colorOut + 10;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
    }
    definition->frameTableBody.rgbColorCount = (u16)flag3;
    if ((short)flag3 != 0)
    {
        remaining = count & 0xffff;
        if (remaining != 0)
        {
            packedOut = (u8 *)(*(u32 *)((u8 *)definition + 0x30) + 0x11);
            packedIn = (u8 *)(packedData + 1);
            do
            {
                packedOut[-1] = packedIn[1];
                packedOut[0] = packedIn[0];
                packedOut[1] = packedIn[-1];
                packedOut[2] = packedIn[2];
                packedIn = packedIn + 4;
                packedOut = packedOut + 0x14;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
    }
    definition->frameTable->faceCount = (u16)frameCount;
    if ((short)frameCount == 0)
    {
        *(u32 *)((u8 *)definition + 0x34) = 0;
    }
    else
    {
        cursor = cursor + 2;
        *(u32 **)((u8 *)definition + 0x34) = cursor;
        if (((u32)cursor & 7) != 0)
            *(u32 *)((u8 *)definition + 0x34) = (u32)cursor & 0xfffffff8;
        cursor = cursor + (frameCount & 0xffff) * 2;
        EQGFX_MEMCPY(*(void **)((u8 *)definition + 0x34), frameData, (frameCount & 0xffff) * 8);
    }
    definition->frameTableBody.entryCount_guess = (u16)entryCount;
    if ((short)entryCount == 0)
    {
        *(u32 *)((u8 *)definition + 0x38) = 0;
    }
    else
    {
        *(u32 **)((u8 *)definition + 0x38) = cursor;
        cursor = cursor + (entryCount & 0xffff);
        EQGFX_MEMCPY(*(void **)((u8 *)definition + 0x38), entryData, (entryCount & 0xffff) * 4);
    }
    definition->frameTableBody.entryCount2_guess = (u16)entryCount2;
    if ((short)entryCount2 == 0)
    {
        *(u32 *)((u8 *)definition + 0x3c) = 0;
    }
    else
    {
        *(u32 **)((u8 *)definition + 0x3c) = cursor;
        cursor = cursor + (entryCount2 & 0xffff);
        EQGFX_MEMCPY(*(void **)((u8 *)definition + 0x3c), entryData2, (entryCount2 & 0xffff) * 4);
    }
    definition->frameTableBody.entryCount3_guess = (u16)entryCount3;
    if ((short)entryCount3 == 0)
    {
        *(u32 *)((u8 *)definition + 0x40) = 0;
    }
    else
    {
        *(u32 **)((u8 *)definition + 0x40) = cursor;
        cursor = cursor + (entryCount3 & 0xffff);
        EQGFX_MEMCPY(*(void **)((u8 *)definition + 0x40), entryData3, (entryCount3 & 0xffff) * 4);
    }
    definition->frameTableBody.recordCount6_guess = (u16)vertexCount;
    if ((short)vertexCount == 0)
    {
        *(u32 *)((u8 *)definition + 0x44) = 0;
    }
    else
    {
        *(u32 **)((u8 *)definition + 0x44) = cursor;
        byteCount = (vertexCount & 0xffff) * 6;
        cursor = (u32 *)((u8 *)cursor + byteCount);
        EQGFX_MEMCPY(*(void **)((u8 *)definition + 0x44), vertexBytes, byteCount);
    }
    if ((short)vertexCount == 0 && (short)entryCount == 0)
    {
        *(u32 *)((u8 *)definition + 0x94) = 0;
        definition->frameTable->bytes = 0;
    }
    else
    {
        *(u32 **)((u8 *)definition + 0x94) = cursor;
        frameBytes = (u8 *)cursor + (count & 0xffff);
        definition->frameTable->bytes = frameBytes;
        cursor = (u32 *)(frameBytes + (frameCount & 0xffff));
    }
    if ((short)entryCount == 0)
    {
        *(u32 **)((u8 *)definition + 0x90) = cursor;
        cursor = cursor + (frameCount & 0xffff) * 4;
    }
    else
    {
        *(u32 *)((u8 *)definition + 0x90) = 0;
    }
    if (name == 0)
    {
        definition->name = 0;
    }
    else
    {
        definition->name = (char *)cursor;
        EQ_CDECL(u32, 0x10068160, (char *))(name);
        EQ_CDECL(void, 0x10068070, (char *, char *))(definition->name, name);
    }
    *(Vector3_guess *)((u8 *)definition + 0x48) = *vector48;
    definition->frameTableBody.materialPalette = sharedA;
    definition->frameTableBody.dmTrack_guess = sharedB;
    *(u32 *)((u8 *)definition + 0x54) = 0;
    definition->frameTableBody.dmRGBTrack_guess = sharedC;
    *(u32 *)((u8 *)definition + 0x70) = value70;
    *(u32 *)((u8 *)definition + 0x74) = value74;
    definition->frameTableBody.scaleIndex_guess = value2E;
    if (vector58 == 0)
    {
        *(float *)((u8 *)definition + 0x58) = 0.0f;
        *(float *)((u8 *)definition + 0x5c) = 0.0f;
        definition->frameTableBody.givenVector_guess.z = 0.0f;
    }
    else
    {
        *(Vector3_guess *)((u8 *)definition + 0x58) = *vector58;
    }
    if (bounds == 0)
    {
        definition->frameTableBody.min_guess[0] = 0.0f;
        definition->frameTableBody.min_guess[1] = 0.0f;
        definition->frameTableBody.min_guess[2] = 0.0f;
        *(float *)((u8 *)definition + 0x84) = 0.0f;
        *(float *)((u8 *)definition + 0x88) = 0.0f;
        *(float *)((u8 *)definition + 0x8c) = 0.0f;
    }
    else
    {
        *(Vector3Pair_guess *)((u8 *)definition + 0x78) = *bounds;
    }
    if ((short)count != 0 && (*(u8 *)((u8 *)definition + 0x0e) & S3D_DM_SPRITE2_FLAG_BOUNDS_GIVEN) == 0)
        EQ_CDECL(void, 0x100100E5, (void *))(definition);
    if ((short)vertexCount != 0 || (short)entryCount != 0)
    {
        if (EQ_CDECL(int, 0x1000FBBA, (void *))(definition) != 0 ||
            EQ_CDECL(int, 0x1000FCA0, (void *))(definition) != 0)
            goto failed;
    }
    if ((short)entryCount == 0)
        EQ_CDECL(void, 0x1000FCC0, (void *, int))(definition, 1);
    if (EQ_CDECL(int, 0x1001564B, (void *, void *))(context, definition) == 0)
    {
        if (name == 0 || EQ_CDECL(int, 0x1000DC08, (void *, void *))(context, definition) >= 0)
        {
            if (sharedB != 0)
                EQGFX_T3D_SHARE_OBJECT(sharedB);
            if (sharedC != 0)
                EQGFX_T3D_SHARE_OBJECT(sharedC);
            if (sharedA != 0)
                EQGFX_T3D_SHARE_OBJECT(sharedA);
            return definition;
        }
        EQ_CDECL(void, 0x10015D81, (void *, void *))(context, definition);
    }
failed:
    if (definition != 0 && existing == 0)
        EQGFX_T3D_FREE_MEMORY(definition);
    return 0;
}

// FUNCTION: EQGFX 0x1000f990
// Destroys the selected DM sprite data and its owned buffers when the instance use count is zero.
int __cdecl s3dUndefineDMSprite(void *context, DMSpriteUndefineInstance_guess *instance)
{
    DMSpritePrimaryData_guess *primaryData;
    DMSpriteAlternateData_guess *alternateData;
    int result;

    if (instance->useCount == 0)
    {
        primaryData = instance->primaryData;
        if (primaryData == 0)
        {
            alternateData = instance->alternateData;
            if (alternateData->volume_54 != 0)
                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, alternateData->volume_54);
            if (alternateData->rgbTrack_4c != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(alternateData->rgbTrack_4c);
                if (EQ_CDECL(int, 0x10017C28, (void *))(alternateData->rgbTrack_4c) == 0)
                    EQ_CDECL(void, 0x10010EED, (void *, void *))(context, alternateData->rgbTrack_4c);
            }
            if (alternateData->rgbTrack_50 != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(alternateData->rgbTrack_50);
                if (EQ_CDECL(int, 0x10017C28, (void *))(alternateData->rgbTrack_50) == 0)
                    EQ_CDECL(void, 0x10010EED, (void *, void *))(context, alternateData->rgbTrack_50);
            }
            if (alternateData->materialPalette_48 != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(alternateData->materialPalette_48);
                if (EQ_CDECL(int, 0x10017C28, (void *))(alternateData->materialPalette_48) == 0)
                    EQ_CDECL(void, 0x10017135, (void *, void *))(context, alternateData->materialPalette_48);
            }
        }
        else
        {
            if (primaryData->volume_34 != 0)
                EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, primaryData->volume_34);
            if (primaryData->rgbTrack_78 != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(primaryData->rgbTrack_78);
                if (EQ_CDECL(int, 0x10017C28, (void *))(primaryData->rgbTrack_78) == 0)
                    EQ_CDECL(void, 0x10010EED, (void *, void *))(context, primaryData->rgbTrack_78);
            }
            if (primaryData->rgbTrack_7c != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(primaryData->rgbTrack_7c);
                if (EQ_CDECL(int, 0x10017C28, (void *))(primaryData->rgbTrack_7c) == 0)
                    EQ_CDECL(void, 0x10010EED, (void *, void *))(context, primaryData->rgbTrack_7c);
            }
            if (primaryData->materialPalette_74 != 0)
            {
                EQ_CDECL(void, 0x10017DAD, (void *))(primaryData->materialPalette_74);
                if (EQ_CDECL(int, 0x10017C28, (void *))(primaryData->materialPalette_74) == 0)
                    EQ_CDECL(void, 0x10017135, (void *, void *))(context, primaryData->materialPalette_74);
            }
            if (primaryData->allocation_04 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_04);
            if (primaryData->allocation_0c != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_0c);
            if (primaryData->allocation_14 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_14);
            if (primaryData->allocation_1c != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_1c);
            if (primaryData->allocation_48 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_48);
            if (primaryData->allocation_50 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_50);
            if (primaryData->allocation_58 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_58);
            if (primaryData->allocation_60 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_60);
            if (primaryData->allocation_80 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_80);
            if (primaryData->allocation_28 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_28);
            if (primaryData->allocation_2c != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_2c);
            if (primaryData->allocation_a4 != 0)
                EQ_CDECL(void, 0x10050CBB, (void *))(primaryData->allocation_a4);
        }
        if ((instance->flags & S3D_DM_SPRITE_INSTANCE_FLAG_IN_DICTIONARY) != 0)
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, instance->dictionaryEntry);
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, instance);
        if (instance->dictionaryEntry != 0 && instance->alternateData == 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(instance->dictionaryEntry);
        if ((instance->flags & S3D_DM_SPRITE_INSTANCE_FLAG_KEEP_MEMORY) == 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(instance);
    }
    else
    {
        result = 0xFFFF3D44;
    }
    return result;
}

// FUNCTION: EQGFX 0x1000fbba
// Builds the vertex visibility bytes from the vertex records, or from frame records when no vertex set exists.
int __cdecl initializeVertexVisibilityBuffer_guess(VertexVisibilityOwner_guess *owner)
{
    VertexVisibilityVertexSet_guess *vertexSet;
    VertexVisibilityFrameSet_guess *frameSet;
    VertexVisibilityVertexRecord_guess *vertexRecord;
    VertexVisibilityFrameRecord_guess *frameRecord;
    u8 *recordData;
    u32 index;

    vertexSet = owner->vertexSet;
    if (vertexSet == 0)
    {
        frameSet = owner->frameSet;
        EQ_CDECL(void *, 0x100682D0, (void *, int, u32))(frameSet->visibilityBuffer, 0, frameSet->visibilitySize);
        frameRecord = frameSet->records;
        if (frameRecord != 0 && (index = 0, frameSet->recordCount != 0))
        {
            do
            {
                if ((frameRecord->flags & 1) == 0)
                {
                    frameSet->visibilityBuffer[frameRecord->firstVertex] = 1;
                    frameSet->visibilityBuffer[frameRecord->secondVertex] = 1;
                    frameSet->visibilityBuffer[frameRecord->thirdVertex] = 1;
                }
                index++;
                frameRecord++;
            } while (index < frameSet->recordCount);
        }
    }
    else if (vertexSet->visibilitySize != 0)
    {
        vertexSet->visibilityBuffer = EQ_CDECL(void *, 0x10050C48, (u32))(vertexSet->visibilitySize);
        if (vertexSet->visibilityBuffer != 0)
        {
            EQ_CDECL(void *, 0x100682D0, (void *, int, u32))(vertexSet->visibilityBuffer, 0, vertexSet->visibilitySize);
            recordData = vertexSet->recordData;
            if (recordData != 0 && (index = 0, vertexSet->recordCount != 0))
            {
                vertexRecord = (VertexVisibilityVertexRecord_guess *)(recordData + 8);
                do
                {
                    if ((vertexRecord->flags & 1) == 0)
                    {
                        vertexSet->visibilityBuffer[vertexRecord->firstVertex] = 1;
                        vertexSet->visibilityBuffer[vertexRecord->secondVertex] = 1;
                        vertexSet->visibilityBuffer[vertexRecord->thirdVertex] = 1;
                    }
                    index++;
                    vertexRecord++;
                } while (index < vertexSet->recordCount);
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1000fca0
// Prepares the DM sprite's frame table for use.
// __cdecl (callee pops nothing, ECX is not an input). Fills every frame byte of the definition's frame
// table (array at +0x7c, count at +0x8) with 1; a missing table does nothing. Returns 0.
int __cdecl s3dInitializeDMSpriteFrameTable_guess(t3dDMSpriteDefinition_guess *definition)
{
    t3dDMSpriteFrameTable_guess *frameTable;

    frameTable = definition->frameTable;
    if (frameTable != 0)
        EQ_CDECL(void *, 0x100682D0, (void *, int, u32))(frameTable->bytes, 1, frameTable->faceCount);
    return 0;
}

// The objects s3dComputeDMSpriteDefExtents_guess reduces: the definition it is handed, the vertex set behind the definition's
// +0x14 and the frame set behind its +0x18. No include/eqgfx header covers them; the offsets come from the
// asm and the fields between the ones the original touches are guesses.

typedef struct t3dDMRecord_guess                 // one 0x14-byte frame
{
    short x;                                     // 0x00
    short y;                                     // 0x02
    short z;                                     // 0x04
    unsigned char unknown_06[0x14 - 0x06];       // 0x06
} t3dDMRecord_guess;

typedef struct t3dDMFrameFace_guess
{
    u8 flags;                                    // 0x00
    u8 unknown_01;
    u16 vertex0;                                 // 0x02
    u16 vertex1;                                 // 0x04
    u16 vertex2;                                 // 0x06
} t3dDMFrameFace_guess;

typedef struct t3dDMFrameSet_guess
{
    u16 count;                                   // 0x00: frames in `frames`
    u16 uvCount;                                 // 0x02: the UV count s3dGetDMSpriteDefinitionNumUVs returns
    u16 normalCount;                             // 0x04: the normal count s3dGetDMSpriteDefinitionNumVertexNormals returns
    u16 rgbColorCount;                           // 0x06: the RGB color count s3dGetDMSpriteDefinitionNumRGBColors returns
    u16 faceCount;                               // 0x08: the face count s3dGetDMSpriteDefinitionNumFaces returns
    unsigned char unknown_0a[0x12 - 0x0a];       // 0x0a
    u16 scaleIndex;                              // 0x12: index into the scale table at 0x100fb868
    t3dDMRecord_guess *frames;                   // 0x14: 0x14 bytes per frame, x/y/z first
    t3dDMFrameFace_guess *faceRecords;            // 0x18
    unsigned char unknown_1c[0x10];              // 0x1c
    t3dDMExtent_guess centerOffset;              // 0x2c: initial sprite center offset
    unsigned char unknown_38[0x10];              // 0x38
    void *materialPalette;                       // 0x48: the palette s3dGetDMSpriteDefinitionMaterialPalette returns
    void *dmTrack;                               // 0x4c
    void *dmRGBTrack;                            // 0x50
    void *collisionVolume;                       // 0x54: fallback volume for a new DM sprite
    float boundingRadius;                        // 0x58: the radius s3dGetDMSpriteDefinitionBoundingRadius reads
    t3dDMExtent_guess min;                       // 0x5c: starts from the first frame, then scaled
    t3dDMExtent_guess max;                       // 0x68
    t3dDMFacePlane_guess *facePlanes;            // 0x74
} t3dDMFrameSet_guess;

typedef struct t3dDMSpriteDef_guess
{
    unsigned char unknown_00[0xe];               // 0x00
    u8 flags;                                    // 0x0e: bit 6 (0x40) is set when the extents are computed
    unsigned char unknown_0f[0x14 - 0x0f];       // 0x0f
    t3dDMVertexSet_guess *vertexSet;             // 0x14: when set, the extents come from its vertices
    t3dDMFrameSet_guess *frameSet;               // 0x18: else from its frames
} t3dDMSpriteDef_guess;

// 0x100fb868: the scale table a frame set's short corners are multiplied by (scaleIndex picks an entry).
#define EQGFX_MESH_SCALE_F_GUESS  EQ_ARRAY(float, 0x100FB868)
#define EQGFX_T3D_ALLOCATE_MEMORY_GUESS EQ_CDECL(void *, 0x10050C48, (int))
#define EQGFX_ZERO_FLOAT_GUESS EQ_ARRAY(float, 0x10076284)
typedef float (__cdecl *t3dDMNormalizeFunction_guess)(float);
#define EQGFX_NORMALIZE_GUESS EQ_GLOBAL(t3dDMNormalizeFunction_guess, 0x100FC77C)

// FUNCTION: EQGFX 0x1000fcc0
// Prepares a DM sprite definition's face planes and optionally marks each face's dominant axis.
/* fake match: the plane distance sums are parenthesised to fix VC6's x87 term order */
int __cdecl s3dComputeDMSpriteDefPlanarEquations_guess(t3dDMSpriteDef_guess *definition, int markDominantAxis)
{
    t3dDMVertexSet_guess *vertexSet;
    t3dDMFrameSet_guess *frameSet;
    t3dDMRecord_guess *frames;
    t3dDMRecord_guess *point0;
    t3dDMRecord_guess *point1;
    t3dDMRecord_guess *point2;
    float *vertices;
    float *vertex0;
    float *vertex1;
    float *vertex2;
    t3dDMFrameFace_guess *frameFace;
    t3dDMVertexFace_guess *vertexFace;
    t3dDMFacePlane_guess *plane;
    float scale;
    Vector3_guess edge1;
    Vector3_guess edge2;
    float normalScale;
    float absoluteX;
    float absoluteY;
    float absoluteZ;
    int faceCount;
    int i;
    Vector3_guess normal;

    vertexSet = definition->vertexSet;
    if (vertexSet != 0)
    {
        if (vertexSet->faceCount > 0 && vertexSet->facePlanes == 0)
        {
            vertexSet->facePlanes = EQGFX_T3D_ALLOCATE_MEMORY_GUESS(vertexSet->faceCount << 4);
            if (vertexSet->facePlanes != 0)
            {
                vertices = vertexSet->vertices;
                vertexFace = vertexSet->faceRecords;
                faceCount = vertexSet->faceCount;
                plane = vertexSet->facePlanes;
                for (i = 0; i < faceCount; i++)
                {
                    vertex0 = vertices + vertexFace->vertex0 * 3;
                    vertex1 = vertices + vertexFace->vertex1 * 3;
                    vertex2 = vertices + vertexFace->vertex2 * 3;
                    edge1.x = vertex1[0] - vertex0[0];
                    edge1.y = vertex1[1] - vertex0[1];
                    edge1.z = vertex1[2] - vertex0[2];
                    edge2.x = vertex2[0] - vertex1[0];
                    edge2.y = vertex2[1] - vertex1[1];
                    edge2.z = vertex2[2] - vertex1[2];
                    normal.x = edge2.y * edge1.z - edge2.z * edge1.y;
                    normal.y = -(edge2.x * edge1.z - edge2.z * edge1.x);
                    normal.z = edge2.x * edge1.y - edge2.y * edge1.x;
                    normalScale = EQGFX_NORMALIZE_GUESS(normal.z * normal.z + normal.y * normal.y + normal.x * normal.x);
                    normal.x = normalScale * normal.x;
                    normal.y = normalScale * normal.y;
                    normal.z = normalScale * normal.z;
                    plane->normal = normal;
                    plane->distance = -(normal.z * vertex0[2] + (normal.x * vertex0[0] + normal.y * vertex0[1]));
                    if (markDominantAxis == 1)
                    {
                        if (plane->normal.x > *EQGFX_ZERO_FLOAT_GUESS)
                            absoluteX = plane->normal.x;
                        else
                            absoluteX = -plane->normal.x;
                        absoluteY = plane->normal.y > *EQGFX_ZERO_FLOAT_GUESS ? plane->normal.y : -plane->normal.y;
                        if (plane->normal.z > *EQGFX_ZERO_FLOAT_GUESS)
                            absoluteZ = plane->normal.z;
                        else
                            absoluteZ = -plane->normal.z;
                        if (absoluteX > absoluteY)
                        {
                            if (absoluteX > absoluteZ)
                                vertexFace->flags |= 4;
                            else
                                vertexFace->flags |= 2;
                        }
                        else if (absoluteY > absoluteZ)
                            vertexFace->flags |= 8;
                        else
                            vertexFace->flags |= 2;
                    }
                    vertexFace++;
                    plane++;
                }
            }
        }
    }
    else
    {
        frameSet = definition->frameSet;
        if (frameSet->faceCount > 0 && frameSet->facePlanes != 0)
        {
            frames = frameSet->frames;
            frameFace = frameSet->faceRecords;
            scale = EQGFX_MESH_SCALE_F_GUESS[frameSet->scaleIndex];
            faceCount = frameSet->faceCount;
            plane = frameSet->facePlanes;
            for (i = 0; i < faceCount; i++)
            {
                point0 = frames + frameFace->vertex0;
                point1 = frames + frameFace->vertex1;
                point2 = frames + frameFace->vertex2;
                edge1.x = (float)point1->x * scale - (float)point0->x * scale;
                edge1.y = (float)point1->y * scale - (float)point0->y * scale;
                edge1.z = (float)point1->z * scale - (float)point0->z * scale;
                edge2.x = (float)point2->x * scale - (float)point1->x * scale;
                edge2.y = (float)point2->y * scale - (float)point1->y * scale;
                edge2.z = (float)point2->z * scale - (float)point1->z * scale;
                normal.x = edge2.y * edge1.z - edge2.z * edge1.y;
                normal.y = -(edge2.x * edge1.z - edge2.z * edge1.x);
                normal.z = edge2.x * edge1.y - edge2.y * edge1.x;
                normalScale = EQGFX_NORMALIZE_GUESS(normal.z * normal.z + normal.y * normal.y + normal.x * normal.x);
                normal.x = normalScale * normal.x;
                normal.y = normalScale * normal.y;
                normal.z = normalScale * normal.z;
                plane->normal = normal;
                plane->distance = -(((float)point0->y * normal.y + ((float)point0->x * normal.x + (float)point0->z * normal.z)) * scale);
                if (markDominantAxis == 1)
                {
                    if (plane->normal.x > *EQGFX_ZERO_FLOAT_GUESS)
                        absoluteX = plane->normal.x;
                    else
                        absoluteX = -plane->normal.x;
                    absoluteY = plane->normal.y > *EQGFX_ZERO_FLOAT_GUESS ? plane->normal.y : -plane->normal.y;
                    if (plane->normal.z > *EQGFX_ZERO_FLOAT_GUESS)
                        absoluteZ = plane->normal.z;
                    else
                        absoluteZ = -plane->normal.z;
                    if (absoluteX > absoluteY)
                    {
                        if (absoluteX > absoluteZ)
                            frameFace->flags |= 4;
                        else
                            frameFace->flags |= 2;
                    }
                    else if (absoluteY > absoluteZ)
                        frameFace->flags |= 8;
                    else
                        frameFace->flags |= 2;
                }
                frameFace++;
                plane++;
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100100e5
// Determines the axis-aligned bounding box of a DM sprite definition's geometry.
// __cdecl (the callee pops nothing, ECX is not an input). Recomputes the definition's extents: from the
// vertex set's floats when it has one, else from the frame set's short corners scaled by the table at
// 0x100fb868; flags bit 6 is set and 0 returned.
int __cdecl s3dComputeDMSpriteDefExtents_guess(t3dDMSpriteDef_guess *definition)
{
    t3dDMVertexSet_guess *vertexSet;
    t3dDMFrameSet_guess *frameSet;
    float *vertex;
    float *vertexEnd;
    t3dDMRecord_guess *frame;
    t3dDMRecord_guess *frameEnd;
    t3dDMRecord_guess minFrame;
    t3dDMRecord_guess maxFrame;
    float scale;

    vertexSet = definition->vertexSet;
    if (vertexSet != 0)
    {
        vertex = vertexSet->vertices;
        vertexSet->min = *(t3dDMExtent_guess *)vertex;
        vertexEnd = vertex + vertexSet->count * 3;
        vertexSet->max = *(t3dDMExtent_guess *)vertex;
        for (; vertex < vertexEnd; vertex = vertex + 3)
        {
            if (vertexSet->min.x > vertex[0])
                vertexSet->min.x = vertex[0];
            if (vertexSet->min.y > vertex[1])
                vertexSet->min.y = vertex[1];
            if (vertexSet->min.z > vertex[2])
                vertexSet->min.z = vertex[2];
            if (vertexSet->max.x < vertex[0])
                vertexSet->max.x = vertex[0];
            if (vertexSet->max.y < vertex[1])
                vertexSet->max.y = vertex[1];
            if (vertexSet->max.z < vertex[2])
                vertexSet->max.z = vertex[2];
        }
    }
    else
    {
        frameSet = definition->frameSet;
        frame = frameSet->frames;
        frameEnd = frame + frameSet->count;
        minFrame = *frame;
        maxFrame = *frame;
        for (; frame < frameEnd; frame++)
        {
            if (minFrame.x > frame->x)
                minFrame.x = frame->x;
            if (minFrame.y > frame->y)
                minFrame.y = frame->y;
            if (minFrame.z > frame->z)
                minFrame.z = frame->z;
            if (maxFrame.x < frame->x)
                maxFrame.x = frame->x;
            if (maxFrame.y < frame->y)
                maxFrame.y = frame->y;
            if (maxFrame.z < frame->z)
                maxFrame.z = frame->z;
        }
        scale = EQGFX_MESH_SCALE_F_GUESS[frameSet->scaleIndex];
        frameSet->min.x = (float)minFrame.x * scale;
        frameSet->min.y = (float)minFrame.y * scale;
        frameSet->min.z = (float)minFrame.z * scale;
        frameSet->max.x = (float)maxFrame.x * scale;
        frameSet->max.y = (float)maxFrame.y * scale;
        frameSet->max.z = (float)maxFrame.z * scale;
    }
    definition->flags = definition->flags | 0x40;
    return 0;
}

// FUNCTION: EQGFX 0x100102a3
// Reports how many texture coordinates a DM sprite definition has.
// __cdecl (callee pops nothing, ECX is not an input). The UV count: the vertex set's when it has one, else the frame set's.
unsigned int __cdecl s3dGetDMSpriteDefinitionNumUVs(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->uvCount;
    return definition->frameSet->uvCount;
}

// FUNCTION: EQGFX 0x100102ba
// Reports how many vertex normals a DM sprite definition has.
// __cdecl (callee pops nothing, ECX is not an input). The vertex normal count: the vertex set's when it has one, else the frame set's.
unsigned int __cdecl s3dGetDMSpriteDefinitionNumVertexNormals(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->normalCount;
    return definition->frameSet->normalCount;
}

// FUNCTION: EQGFX 0x100102d1
// Reports how many RGB colors a DM sprite definition has.
// __cdecl (callee pops nothing, ECX is not an input). The definition's RGB color count: the vertex set's when it has one, else the frame set's.
unsigned int __cdecl s3dGetDMSpriteDefinitionNumRGBColors(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->rgbColorCount;
    return definition->frameSet->rgbColorCount;
}

// FUNCTION: EQGFX 0x100102e8
// Reports how many faces a DM sprite definition has.
// __cdecl (callee pops nothing, ECX is not an input). The definition's face count: the vertex set's when it has one, else the frame set's.
unsigned int __cdecl s3dGetDMSpriteDefinitionNumFaces(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->faceCount;
    return definition->frameSet->faceCount;
}

// FUNCTION: EQGFX 0x100102ff
// Reports the DM sprite definition's bounding radius.
// __cdecl (callee pops nothing, ECX is not an input). The definition's bounding radius: the vertex set's
// when it has one, else the frame set's.
float __cdecl s3dGetDMSpriteDefinitionBoundingRadius(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->boundingRadius;
    return definition->frameSet->boundingRadius;
}

// FUNCTION: EQGFX 0x10010315
// Provides the material palette associated with a DM sprite definition.
// __cdecl (callee pops nothing, ECX is not an input). The definition's material palette: the vertex set's when it has one, else the frame set's.
u32 __cdecl s3dGetDMSpriteDefinitionMaterialPalette(t3dDMSpriteDef_guess *definition)
{
    if (definition->vertexSet != 0)
        return definition->vertexSet->materialPalette;
    return definition->frameSet->materialPalette;
}

// The DM sprite object s3dGetDMSpriteDMRGBTrack and s3dGetDMSpriteCollisionVolume read (no include/eqgfx
// header has this layout; the offsets come from the asm).
typedef struct t3dDMSprite_guess
{
    u32 type;                           // 0x00: 0x48 for a valid sprite (s3dIsDMSpriteValid)
    u32 unknown_04;                     // 0x04
    void *entry;                        // 0x08: dictionary key used by s3dDestroyDMSprite
    u8 flags;                           // 0x0c: bit 0 means the sprite is in the dictionary
    u8 unknown_0d[3];                   // 0x0d
    int state;                          // 0x10: nonzero prevents destruction
    t3dDMExtent_guess centerOffset;     // 0x14: the x/y/z offset s3dSetDMSpriteCenterOffset copies in
    void *materialPalette;              // 0x20: the material palette s3dSetDMSpriteMaterialPalette stores
    void *dmTrack;                      // 0x24: the DM track s3dSetDMSpriteDMTrack stores
    void *dmRGBTrack;                   // 0x28: the DM RGB track s3dGetDMSpriteDMRGBTrack returns
    void *collisionVolume;              // 0x2c: the volume s3dGetDMSpriteCollisionVolume returns
    u32 constantAmbient;                // 0x30: the value s3dGetDMSpriteConstantAmbient copies out (undefined4 in Ghidra)
    void *unknown_34;                   // 0x34: shared object checked by s3dIsDMSpriteValid
} t3dDMSprite_guess;

// FUNCTION: EQGFX 0x1001032B
// Checks whether a DM sprite instance is initialized for use.
// __cdecl (callee pops nothing, ECX is not an input). Returns 0 for a sprite of type 0x48 with its 0x34 field set, else 0xffff3d3e; a null sprite still faults on the read.
int __cdecl s3dIsDMSpriteValid(t3dDMSprite_guess *sprite)
{
    int result;

    result = 0;
    if (sprite == 0)
        result = 0xffff3d3e;
    if (sprite->type != 0x48)
        result = 0xffff3d3e;
    if (sprite->unknown_34 == 0)
        result = 0xffff3d3e;
    return result;
}

// FUNCTION: EQGFX 0x1001034C
// Creates a DM sprite instance with its definition's appearance and a supplied or default collision volume.
t3dDMSprite_guess *__cdecl s3dCreateDMSprite(void *context, t3dDMSpriteDef_guess *definition, void *sourceVolume)
{
    t3dDMSprite_guess *sprite;
    t3dDMExtent_guess *centerOffset;
    void *sharedObject;

    sprite = EQ_CDECL(t3dDMSprite_guess *, 0x10050C48, (u32))(T3D_DM_SPRITE_ALLOC_SIZE);
    if (sprite != 0)
    {
        sprite->type = T3D_DM_SPRITE_TYPE;
        sprite->unknown_04 = 0;
        sprite->entry = 0;
        *((u32 *)&sprite->flags) = 0;
        sprite->state = 0;
        sprite->unknown_34 = definition;
        if (definition->vertexSet == 0)
            centerOffset = &definition->frameSet->centerOffset;
        else
            centerOffset = &definition->vertexSet->centerOffset;
        sprite->centerOffset = *centerOffset;
        *((u8 *)&sprite->constantAmbient + 2) &= 0;
        *((u8 *)&sprite->constantAmbient + 1) &= 0;
        *((u8 *)&sprite->constantAmbient) &= 0;
        sprite->materialPalette = 0;
        sprite->dmTrack = 0;
        sprite->dmRGBTrack = 0;
        sprite->collisionVolume = 0;
        if (definition->vertexSet == 0)
            sharedObject = definition->frameSet->materialPalette;
        else
            sharedObject = definition->vertexSet->materialPalette;
        sprite->materialPalette = sharedObject;
        if (sharedObject != 0)
            EQGFX_T3D_SHARE_OBJECT((int *)sharedObject);
        if (definition->vertexSet == 0)
            sharedObject = definition->frameSet->dmTrack;
        else
            sharedObject = definition->vertexSet->dmTrack;
        sprite->dmTrack = sharedObject;
        if (sharedObject != 0)
            EQGFX_T3D_SHARE_OBJECT((int *)sharedObject);
        if (definition->vertexSet == 0)
            sharedObject = definition->frameSet->dmRGBTrack;
        else
            sharedObject = definition->vertexSet->dmRGBTrack;
        sprite->dmRGBTrack = sharedObject;
        if (sharedObject != 0)
            EQGFX_T3D_SHARE_OBJECT((int *)sharedObject);
        if (sourceVolume == 0)
        {
            if (definition->vertexSet == 0)
                sourceVolume = definition->frameSet->collisionVolume;
            else
                sourceVolume = definition->vertexSet->collisionVolume;
        }
        sprite->collisionVolume = EQGFX_T3D_DUPLICATE_VOLUME(context, (RenderVolume_guess *)sourceVolume);
        if (sprite->collisionVolume == 0 ||
            EQ_CDECL(int, 0x1001564B, (void *, t3dDMSprite_guess *))(context, sprite) != 0)
        {
            if (sprite->materialPalette != 0)
                EQGFX_T3D_UNSHARE_OBJECT((int *)sprite->materialPalette);
            if (sprite->dmTrack != 0)
                EQGFX_T3D_UNSHARE_OBJECT((int *)sprite->dmTrack);
            if (sprite->dmRGBTrack != 0)
                EQGFX_T3D_UNSHARE_OBJECT((int *)sprite->dmRGBTrack);
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
        else
        {
            EQGFX_T3D_SHARE_OBJECT((int *)definition);
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x1001049E
// Clones a DM sprite instance, sharing its visual resources and optionally replacing its collision volume.
t3dDMSprite_guess *__cdecl s3dDuplicateDMSprite(void *context, t3dDMSprite_guess *sourceSprite, void *sourceVolume)
{
    t3dDMSprite_guess *sprite;

    sprite = EQ_CDECL(t3dDMSprite_guess *, 0x10050C48, (u32))(T3D_DM_SPRITE_ALLOC_SIZE);
    if (sprite != 0)
    {
        sprite->type = T3D_DM_SPRITE_TYPE;
        sprite->unknown_04 = 0;
        sprite->entry = 0;
        *((u32 *)&sprite->flags) = 0;
        sprite->state = 0;
        sprite->centerOffset = sourceSprite->centerOffset;
        sprite->unknown_34 = sourceSprite->unknown_34;
        sprite->materialPalette = sourceSprite->materialPalette;
        sprite->dmTrack = sourceSprite->dmTrack;
        sprite->dmRGBTrack = sourceSprite->dmRGBTrack;
        if (sourceVolume == 0)
            sourceVolume = sourceSprite->collisionVolume;
        sprite->collisionVolume = EQGFX_T3D_DUPLICATE_VOLUME(context, (RenderVolume_guess *)sourceVolume);
        if (sprite->collisionVolume == 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
        else if (EQ_CDECL(int, 0x1001564B, (void *, t3dDMSprite_guess *))(context, sprite) != 0)
        {
            EQGFX_T3D_DESTROY_VOLUME(context, (RenderVolume_guess *)sprite->collisionVolume);
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
            sprite = 0;
        }
        else
        {
            EQGFX_T3D_SHARE_OBJECT((int *)sprite->unknown_34);
            if (sprite->materialPalette != 0)
                EQGFX_T3D_SHARE_OBJECT((int *)sprite->materialPalette);
            if (sprite->dmTrack != 0)
                EQGFX_T3D_SHARE_OBJECT((int *)sprite->dmTrack);
            if (sprite->dmRGBTrack != 0)
                EQGFX_T3D_SHARE_OBJECT((int *)sprite->dmRGBTrack);
        }
    }
    return sprite;
}

// FUNCTION: EQGFX 0x10010575
// Destroys a DM sprite instance and releases its collision volume, material palette and tracks.
int __cdecl s3dDestroyDMSprite(void *context, t3dDMSprite_guess *sprite)
{
    int result;

    if (sprite->state != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, sprite->collisionVolume);
        if (sprite->materialPalette != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sprite->materialPalette);
            if (EQ_CDECL(int, 0x10017C28, (void *))(sprite->materialPalette) == 0)
            {
                EQ_CDECL(void, 0x10017135, (void *, void *))(context, sprite->materialPalette);
            }
        }
        if (sprite->dmTrack != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sprite->dmTrack);
            if (EQ_CDECL(int, 0x10017C28, (void *))(sprite->dmTrack) == 0)
            {
                EQ_CDECL(void, 0x10010EED, (void *, void *))(context, sprite->dmTrack);
            }
        }
        if (sprite->dmRGBTrack != 0)
        {
            EQ_CDECL(void, 0x10017DAD, (void *))(sprite->dmRGBTrack);
            if (EQ_CDECL(int, 0x10017C28, (void *))(sprite->dmRGBTrack) == 0)
            {
                EQ_CDECL(void, 0x10010EED, (void *, void *))(context, sprite->dmRGBTrack);
            }
        }
        EQ_CDECL(void, 0x10017DAD, (void *))(sprite->unknown_34);
        if ((sprite->flags & 1) != 0)
        {
            EQ_CDECL(void, 0x1000E334, (void *, void *))(context, sprite->entry);
        }
        result = EQ_CDECL(int, 0x10015D81, (void *, void *))(context, sprite);
        if (sprite->entry != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(sprite->entry);
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001064B
// Changes the material palette assigned to a DM sprite.
// __cdecl (callee pops nothing, ECX is not an input). Unshares the sprite's old material palette, stores the new one and shares it when set. Returns 0.
int __cdecl s3dSetDMSpriteMaterialPalette(t3dDMSprite_guess *sprite, void *materialPalette)
{
    void *oldPalette;

    oldPalette = sprite->materialPalette;
    if (oldPalette != 0)
        EQ_CDECL(void, 0x10017DAD, (void *))(oldPalette);
    sprite->materialPalette = materialPalette;
    if (materialPalette != 0)
        EQ_CDECL(void, 0x10017CE8, (void *))(materialPalette);
    return 0;
}

// FUNCTION: EQGFX 0x10010674
// __cdecl (callee pops nothing, ECX is not an input). Unshares the sprite's old DM track, stores the new one and shares it when set. Returns 0.
int __cdecl s3dSetDMSpriteDMTrack(int unused, t3dDMSprite_guess *sprite, void *dmTrack)
{
    void *oldTrack;

    oldTrack = sprite->dmTrack;
    if (oldTrack != 0)
        EQ_CDECL(void, 0x10017DAD, (void *))(oldTrack);
    sprite->dmTrack = dmTrack;
    if (dmTrack != 0)
        EQ_CDECL(void, 0x10017CE8, (void *))(dmTrack);
    return 0;
}

// FUNCTION: EQGFX 0x1001069d
// __cdecl (callee pops nothing, ECX is not an input). Returns the sprite's DM RGB track pointer.
void *__cdecl s3dGetDMSpriteDMRGBTrack(t3dDMSprite_guess *sprite)
{
    return sprite->dmRGBTrack;
}

// FUNCTION: EQGFX 0x100106a5
// __cdecl (callee pops nothing, ECX is not an input). Unshares the sprite's old DM RGB track, stores the new one and shares it when set. Returns 0.
int __cdecl s3dSetDMSpriteDMRGBTrack(int unused, t3dDMSprite_guess *sprite, void *dmRGBTrack)
{
    void *oldTrack;

    oldTrack = sprite->dmRGBTrack;
    if (oldTrack != 0)
        EQ_CDECL(void, 0x10017DAD, (void *))(oldTrack);
    sprite->dmRGBTrack = dmRGBTrack;
    if (dmRGBTrack != 0)
        EQ_CDECL(void, 0x10017CE8, (void *))(dmRGBTrack);
    return 0;
}

// FUNCTION: EQGFX 0x100106ce
// Sets the position offset used for the DM sprite's center.
// __cdecl (callee pops nothing, ECX is not an input). Copies the x/y/z offset to the sprite's center offset (+0x14). Returns 0.
int __cdecl s3dSetDMSpriteCenterOffset(t3dDMSprite_guess *sprite, t3dDMExtent_guess *offset)
{
    sprite->centerOffset = *offset;
    return 0;
}

// FUNCTION: EQGFX 0x100106e3
// Reads the constant ambient light value for a DM sprite.
// __cdecl (callee pops nothing, ECX is not an input). Stores the sprite's constant ambient through the out pointer. Returns 0.
int __cdecl s3dGetDMSpriteConstantAmbient(t3dDMSprite_guess *sprite, u32 *constantAmbient)
{
    *constantAmbient = sprite->constantAmbient;
    return 0;
}

// FUNCTION: EQGFX 0x100106f3
// Sets the constant ambient light value for a DM sprite.
// __cdecl (callee pops nothing, ECX is not an input). Copies *constantAmbient into the sprite's constant ambient. Returns 0.
int __cdecl s3dSetDMSpriteConstantAmbient(t3dDMSprite_guess *sprite, u32 *constantAmbient)
{
    sprite->constantAmbient = *constantAmbient;
    return 0;
}

// FUNCTION: EQGFX 0x10010703
// __cdecl (callee pops nothing, ECX is not an input). Returns the sprite's collision volume pointer.
void *__cdecl s3dGetDMSpriteCollisionVolume(t3dDMSprite_guess *sprite)
{
    return sprite->collisionVolume;
}

// FUNCTION: EQGFX 0x1001070B
// Sets the collision shape associated with a DM sprite.
// __cdecl. Replaces the sprite's collision volume with a duplicate of sourceVolume: when the duplicate fails it returns
// T3D_COMPONENT_INDEX_ERROR and changes nothing; else it destroys the old volume, stores the duplicate and returns 0.
int __cdecl s3dSetDMSpriteCollisionVolume(void *context, t3dDMSprite_guess *sprite, RenderVolume_guess *sourceVolume)
{
    RenderVolume_guess *duplicate;

    duplicate = EQGFX_T3D_DUPLICATE_VOLUME(context, sourceVolume);
    if (duplicate == 0)
    {
        return T3D_COMPONENT_INDEX_ERROR;
    }
    EQGFX_T3D_DESTROY_VOLUME(context, sprite->collisionVolume);
    sprite->collisionVolume = duplicate;
    return 0;
}
