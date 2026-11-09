// unit_1001E0E3.c - eqgfx_dx7.dll, the address run 0x1001E0E3..0x1002055A (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"
#include "trilogy/common.h"
#define T3D_WORLD_OBJECT_LISTS_OFFSET 0x50
#define EQGFX_WORLD_VISIBILITY_STAMP EQ_GLOBAL(u32, 0x100FBBB0)
#define T3D_FRUSTUM_OUTSIDE -1
#define T3D_PVS_KIND 0x25
#define parseColorScale EQ_GLOBAL(float, 0x100762A8)
#define EQGFX_FLOAT_ZERO EQ_GLOBAL(float, 0x10076284)
#define EQGFX_FLOAT_ONE EQ_GLOBAL(float, 0x10076280)
#define EQGFX_RANGE_SCALE EQ_GLOBAL(float, 0x100762B0)
#define T3D_RANGE_SCALE_ADDRESS 100762b0h
#define T3D_ROUND_FLOAT_TO_INDEX_ADDRESS 1004ec36h
#define EQGFX_STARTUP_BLOCK EQ_GLOBAL(u8 *, 0x100F3FF4)
#define T3D_COSINE_TABLE_OFFSET 0x1000
#define T3D_SINE_TABLE_OFFSET 0x1800
#define T3D_SCALE_TABLE_OFFSET 0x6088
#define T3D_SCALE_RECORD_SIZE 0x18

// The PVS stream of a region: the stream's length (16-bit words, or bytes for encoding 2) and the codes.
typedef struct T3DPVSData_guess
{
    int count;                   // 0x00
    u16 *codes;                  // 0x04
} T3DPVSData_guess;

// A region's PVS: its kind at +0x00, the stream at +0x08, the BSP tree at +0x0c and its encoding at +0x18 (1 = 16-bit words, 2 = bytes, else a sorted list).
typedef struct T3DPVS_guess
{
    u32 kind;                    // 0x00
    u8 unknown_04[0x04];         // 0x04
    T3DPVSData_guess *data;      // 0x08
    struct T3DPVSNode_guess *tree; // 0x0c: the BSP tree a point descends to its leaf
    u8 unknown_10[0x08];         // 0x10
    int encoding;                // 0x18
} T3DPVS_guess;

// A node of a PVS tree: its plane (normal and distance); a leaf has its stream at +0x18, an interior node has 0 there.
typedef struct T3DPVSNode_guess
{
    float normal[3];                     // 0x00
    float planeDistance;                 // 0x0c
    struct T3DPVSNode_guess *backChild;  // 0x10: taken when the dot product is below planeDistance
    struct T3DPVSNode_guess *frontChild; // 0x14: taken when it is at or above
    T3DPVSData_guess *leaf;              // 0x18
} T3DPVSNode_guess;

// One entry of the world's region table: only the PVS pointer at +0x24 is known.
typedef struct T3DRegion_guess
{
    u8 unknown_00[0x04];         // 0x00
    u32 regionId;                // 0x04: the number a visibility query matches against
    u8 unknown_08[0x04];         // 0x08
    u32 environmentFlags;        // 0x0c: mask tested by the world-solid/environment ray query
    u8 unknown_10[0x04];         // 0x10
    float centerX;               // 0x14
    float centerY;               // 0x18
    float centerZ;               // 0x1c
    float boundsRadius;           // 0x20
    T3DPVS_guess *pvs;           // 0x24
    u32 unknown_28;              // 0x28
    struct T3DRegionActorList_guess *actorList; // 0x2c
} T3DRegion_guess;

// The location a visibility query is made from: its position, heading, pitch and roll (512 units per turn) and its region index.
typedef struct T3DLocation_guess
{
    float x;                     // 0x00
    float y;                     // 0x04
    float z;                     // 0x08
    float heading;               // 0x0c
    float pitch;                 // 0x10
    float roll;                  // 0x14
    int region;                  // 0x18: index into the world's region table
} T3DLocation_guess;

typedef struct T3DRegionActorList_guess
{
    u32 kind;                    // 0x00
    u32 actorCount;              // 0x04
    u32 unknown_08;              // 0x08
    u32 *actorIndices;           // 0x0c
} T3DRegionActorList_guess;

typedef struct T3DActorInstance_guess
{
    u8 unknown_00[0x10];         // 0x00
    float x;                     // 0x10
    float y;                     // 0x14
    float z;                     // 0x18
    u8 unknown_1c[0x18];         // 0x1c
    float boundsRadius;          // 0x34
    float boundsHeight;          // 0x38
    u8 unknown_3c[0x24];         // 0x3c
    u32 visibilityLimit;        // 0x60
    int actorFilterId;           // 0x64
    u8 unknown_68[0x8c];         // 0x68
    u32 lastVisibilityStamp;    // 0xf4
} T3DActorInstance_guess;

typedef struct T3DCamera_guess
{
    u32 unknown_00;              // 0x00
    float visibilityDepth;       // 0x04
} T3DCamera_guess;

typedef struct T3DReferenceList_guess
{
    u32 unknown_00;              // 0x00
    u32 count;                   // 0x04
} T3DReferenceList_guess;

typedef struct T3DWorldSolidNode_guess
{
    float plane[4];                    // 0x00
    struct T3DWorldSolidNode_guess *positiveChild; // 0x10
    struct T3DWorldSolidNode_guess *negativeChild; // 0x14
    int isSolid;                       // 0x18
} T3DWorldSolidNode_guess;

typedef struct T3DWorldSolidSegment_guess
{
    T3DWorldSolidNode_guess *node; // 0x00
    int state;                     // 0x04
    float savedPoint[3];           // 0x08
    u32 unknown_14;                // 0x14
} T3DWorldSolidSegment_guess;

typedef struct T3DWorldObjectList_guess
{
    u32 count;                     // 0x00
    u32 capacity;                  // 0x04
    u32 currentIndex;              // 0x08
    u32 *objects;                  // 0x0c
} T3DWorldObjectList_guess;

typedef struct T3DWorldObjectEntry_guess
{
    u8 unknown_00[0x18];
    u32 link_18;
    u8 unknown_1c[0x08];
    u32 link_24;
    u32 link_28;
    u32 link_2c;
    u8 unknown_30[0x08];
    u32 link_38;
    u8 unknown_3c[0x20];
    u32 link_5c;
    u8 unknown_60[0x04];
    u32 link_64;
    u8 unknown_68[0x04];
    u32 link_6c;
    u8 unknown_70[0x18];
    u32 link_88;
} T3DWorldObjectEntry_guess;

typedef struct T3DWorldObjectReference_guess
{
    u8 unknown_00[0x14];
    u32 object_14;
    u8 unknown_18[0x04];
    u32 object_1c;
    u32 object_20;
    u32 object_24;
    u32 object_28;
    u32 object_2c;
    u32 object_30;
} T3DWorldObjectReference_guess;

// The world object this sets a field on (no include/ struct covers it): the constant-ambient slot, the region table and
// the visible-region list of s3dDecodePVSForRegion are known; +0x360 sits inside the unknown block of unit_1000D9E3.c's
// file-local t3dWorld_guess too.
typedef struct t3dWorldAmbient_guess
{
    u8 unknown_00[0x0c];          // 0x00
    T3DWorldSolidNode_guess *solidTreeRoot; // 0x0c
    u8 unknown_10[0x08];          // 0x10
    u32 regionCount;              // 0x18
    u8 unknown_1c[0x08];          // 0x1c
    T3DRegion_guess **regions;    // 0x24
    u8 unknown_28[0x20];          // 0x28
    char *defaultPaletteFileName; // 0x48
    u8 unknown_4c[0x44];          // 0x4c
    u32 num_2d_sprite_definitions; // 0x90
    u8 unknown_94[0x0c];          // 0x94
    u32 num_3d_sprite_definitions; // 0xa0
    u8 unknown_a4[0x0c];          // 0xa4
    u32 num_composite_sprite_definitions; // 0xb0
    u8 unknown_b4[0x0c];          // 0xb4
    u32 num_hierarchical_sprite_definitions; // 0xc0
    u8 unknown_c4[0x0c];          // 0xc4
    u32 num_actor_definitions; // 0xd0
    u8 unknown_d4[0x3c];          // 0xd4
    u32 num_simple_sprite_instances; // 0x110
    u8 unknown_114[0x1c];        // 0x114
    u32 num_2d_sprite_instances; // 0x130
    u8 unknown_134[0x0c];        // 0x134
    u32 num_3d_sprite_instances; // 0x140
    u8 unknown_144[0x0c];        // 0x144
    u32 num_composite_sprite_instances; // 0x150
    u8 unknown_154[0x0c];        // 0x154
    u32 num_hierarchical_sprite_instances; // 0x160
    u8 unknown_164[0x0c];        // 0x164
    u32 num_actor_instances; // 0x170
    u8 unknown_174[0x08];        // 0x174
    T3DActorInstance_guess **actorInstances; // 0x17c
    u32 num_ais; // 0x180
    u8 unknown_184[0x0c];        // 0x184
    u32 num_bm_infos; // 0x190
    u8 unknown_194[0x0c];        // 0x194
    u32 num_light_definitions; // 0x1a0
    u8 unknown_1a4[0x0c];        // 0x1a4
    u32 num_light_instances; // 0x1b0
    u8 unknown_1b4[0x0c];        // 0x1b4
    u32 num_point_lights; // 0x1c0
    u8 unknown_1c4[0x7c];        // 0x1c4
    u32 num_zones; // 0x240
    u8 unknown_244[0xcc];       // 0x244
    u32 num_active_ais; // 0x310
    u8 unknown_314[0x14];       // 0x314
    u32 temporaryTagCount; // 0x328
    u32 unknown_32c;       // 0x32c
    void **temporaryTags;  // 0x330
    u32 visibleCapacity;        // 0x334: entries the visibleList block holds
    u32 visibleCount;           // 0x338
    u32 *visibleList;           // 0x33c
    u32 visibleRegion;          // 0x340: the region visibleList was decoded for
    u8 unknown_344[0x18];       // 0x344
    T3DWorldSolidSegment_guess *clipSegmentStack; // 0x35c
    u32 constant_ambient;   // 0x360: packed rgb, what s3dSetWorldConstantAmbient copies in
    u32 special_ambient;    // 0x364: packed rgb, what s3dSetWorldSpecialAmbient copies in
} t3dWorldAmbient_guess;

// The hierarchical sprite instance this setter writes: only the user frame-control slot at +0x34 is known.
typedef struct T3DHierarchicalSprite_guess
{
    u8 unknown_00[0x34];         // 0x00
    int userFrameControl;        // 0x34
} T3DHierarchicalSprite_guess;

// The zone block's kind, zeroed fields, and embedded reference-list storage.
typedef struct T3dZone_guess
{
    u32 kind;                // 0x00
    u32 unknown_04;          // 0x04
    u32 unknown_08;          // 0x08
    u32 unknown_0c;          // 0x0c
    u32 referenceList[6];    // 0x10
} T3dZone_guess;

#define T3D_ZONE_KIND 0x44
#define T3D_ZONE_ALLOCATION_SIZE 0x28
#define T3D_ZONE_REFERENCE_CAPACITY 10
#define T3D_VISIBLE_REGION_LIST_INITIAL_CAPACITY 0x100
#define T3D_VISIBLE_REGION_LIST_INITIAL_BYTES 0x400

// The temporary tag entry this cleaner filters: kind at +0x00 decides the handling,
// entry at +0x08 is the dictionary key / heap block, flags at +0x0c keep bits 0-2.
typedef struct T3dTemporaryTag_guess
{
    int kind; // 0x00
    u8 unknown_04[0x04]; // 0x04
    void *entry; // 0x08
    u32 flags; // 0x0c
} T3dTemporaryTag_guess;

// FUNCTION: EQGFX 0x1001e0e3
// __cdecl, returns the world's 2D sprite definition count.
u32 __cdecl t3dGetWorldNum2DSpriteDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_2d_sprite_definitions;
}

// FUNCTION: EQGFX 0x1001e0ee
// __cdecl, returns the world's 3D sprite definition count.
u32 __cdecl t3dGetWorldNum3DSpriteDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_3d_sprite_definitions;
}

// FUNCTION: EQGFX 0x1001e0f9
// __cdecl, returns the world's composite sprite definition count.
u32 __cdecl t3dGetWorldNumCompositeSpriteDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_composite_sprite_definitions;
}

// FUNCTION: EQGFX 0x1001e104
// __cdecl, returns the world's hierarchical sprite definition count.
u32 __cdecl t3dGetWorldNumHierarchicalSpriteDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_hierarchical_sprite_definitions;
}

// FUNCTION: EQGFX 0x1001e10f
// __cdecl, returns the world's actor definition count.
u32 __cdecl t3dGetWorldNumActorDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_actor_definitions;
}

// FUNCTION: EQGFX 0x1001e11a
// __cdecl, returns the world's simple sprite instance count.
u32 __cdecl t3dGetWorldNumSimpleSpriteInstances(t3dWorldAmbient_guess *world)
{
    return world->num_simple_sprite_instances;
}

// FUNCTION: EQGFX 0x1001e125
// __cdecl, returns the world's 2D sprite instance count.
u32 __cdecl t3dGetWorldNum2DSpriteInstances(t3dWorldAmbient_guess *world)
{
    return world->num_2d_sprite_instances;
}

// FUNCTION: EQGFX 0x1001e130
// __cdecl, returns the world's 3D sprite instance count.
u32 __cdecl t3dGetWorldNum3DSpriteInstances(t3dWorldAmbient_guess *world)
{
    return world->num_3d_sprite_instances;
}

// FUNCTION: EQGFX 0x1001e13b
// __cdecl, returns the world's composite sprite instance count.
u32 __cdecl t3dGetWorldNumCompositeSpriteInstances(t3dWorldAmbient_guess *world)
{
    return world->num_composite_sprite_instances;
}

// FUNCTION: EQGFX 0x1001e146
// __cdecl, returns the world's hierarchical sprite instance count.
u32 __cdecl t3dGetWorldNumHierarchicalSpriteInstances(t3dWorldAmbient_guess *world)
{
    return world->num_hierarchical_sprite_instances;
}

// FUNCTION: EQGFX 0x1001e151
// __cdecl, returns the world's actor instance count beyond its definitions.
u32 __cdecl t3dGetWorldNumActorInstances(t3dWorldAmbient_guess *world)
{
    if (world->num_actor_instances > world->num_actor_definitions)
        return world->num_actor_instances - world->num_actor_definitions;
    return 0;
}

// FUNCTION: EQGFX 0x1001e16b
// __cdecl, returns the world's AI count.
u32 __cdecl t3dGetWorldNumAIs(t3dWorldAmbient_guess *world)
{
    return world->num_ais;
}

// FUNCTION: EQGFX 0x1001e176
// __cdecl, returns the world's board-model-info count.
u32 __cdecl t3dGetWorldNumBMInfos(t3dWorldAmbient_guess *world)
{
    return world->num_bm_infos;
}

// FUNCTION: EQGFX 0x1001e181
// __cdecl, returns the world's light-definition count.
u32 __cdecl t3dGetWorldNumLightDefinitions(t3dWorldAmbient_guess *world)
{
    return world->num_light_definitions;
}

// FUNCTION: EQGFX 0x1001e18c
// __cdecl, returns the world's light-instance count.
u32 __cdecl t3dGetWorldNumLightInstances(t3dWorldAmbient_guess *world)
{
    return world->num_light_instances;
}

// FUNCTION: EQGFX 0x1001e197
// __cdecl, returns the world's point-light count.
u32 __cdecl t3dGetWorldNumPointLights(t3dWorldAmbient_guess *world)
{
    return world->num_point_lights;
}

// FUNCTION: EQGFX 0x1001e1a2
// __cdecl, returns the world's zone count.
u32 __cdecl t3dGetWorldNumZones(t3dWorldAmbient_guess *world)
{
    return world->num_zones;
}

// FUNCTION: EQGFX 0x1001e1ad
// __cdecl, returns the world's active AI count.
u32 __cdecl t3dGetWorldNumActiveAIs(t3dWorldAmbient_guess *world)
{
    return world->num_active_ais;
}

// FUNCTION: EQGFX 0x1001e1b8
// __cdecl, returns the length of the world's default palette file name.
unsigned int __cdecl t3dGetWorldDefaultPaletteFileNameLength(t3dWorldAmbient_guess *world)
{
    return EQ_CDECL(unsigned int, 0x10068160, (char *))(world->defaultPaletteFileName);
}

// FUNCTION: EQGFX 0x1001e1c6
// Gets the file name of the palette assigned to a world.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Copies the world's
// default palette file name into fileNameOut, empty string when NULL. Returns 0.
int __cdecl t3dGetWorldDefaultPaletteFileName(t3dWorldAmbient_guess *world, char *fileNameOut)
{
    char *paletteName;

    paletteName = world->defaultPaletteFileName;
    if (paletteName == 0)
    {
        *fileNameOut = 0;
    }
    else
    {
        EQ_CDECL(void, 0x10068070, (char *, const char *))(fileNameOut, paletteName);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001e1e9
// Changes the palette file assigned as the world's default.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Replaces the world's default palette file name
// with a copy of paletteFileName (NULL clears it). Returns 0, or 0xffff3d3e when the copy can't be allocated.
int __cdecl t3dSetWorldDefaultPaletteFileName(t3dWorldAmbient_guess *world, char *paletteFileName)
{
    int result;
    size_t nameLength;
    char *nameCopy;

    result = 0;
    if (world->defaultPaletteFileName != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->defaultPaletteFileName);
    }
    if (paletteFileName != 0)
    {
        nameLength = EQ_CDECL(size_t, 0x10068160, (char *))(paletteFileName);
        nameCopy = EQ_CDECL(char *, 0x10050C48, (size_t))(nameLength + 1);
        world->defaultPaletteFileName = nameCopy;
        if (nameCopy == 0)
        {
            result = 0xFFFF3D3E;
        }
        else
        {
            EQ_CDECL(void, 0x10068070, (char *, const char *))(nameCopy, paletteFileName);
        }
    }
    else
    {
        world->defaultPaletteFileName = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001e23b
// Copies the world's live objects of one type id into a new array, omitting each definition's default instance.
int __cdecl t3dGetWorldObjectList(t3dWorldAmbient_guess *world, int objectType, int *objectListOut)
{
    T3DWorldObjectList_guess *worldLists;
    T3DWorldObjectList_guess *selectedList;
    T3DWorldObjectEntry_guess *objectEntry;
    T3DWorldObjectReference_guess *objectReference;
    u32 objectCount;
    u32 arrayCapacity;
    u32 *currentIndex;
    u32 *objectArray;
    u32 *outputArray;
    u32 *outputEnd;
    u32 *sourceSlot;
    u32 *nextSlot;
    u32 selectionThreshold;
    u32 arrayIndex;
    u32 linkedIndex;
    u32 nextIndex;
    u32 currentObject;
    u32 definitionCount;
    u32 defaultInstance;
    u32 outputCount;
    int swapped;
    int allocatedList;

    worldLists = (T3DWorldObjectList_guess *)((u8 *)world + T3D_WORLD_OBJECT_LISTS_OFFSET);
    if (objectType < 0x1b)
    {
        if (objectType == 0x1a)
        {
            selectedList = worldLists + 19;
            objectCount = selectedList->count;
            goto selected_object_list;
        }
        switch (objectType)
        {
        case 1:
            selectedList = worldLists + 1;
            objectCount = selectedList->count;
            break;
        case 2:
            selectedList = worldLists + 11;
            if (worldLists[1].count < selectedList->count)
                objectCount = selectedList->count - worldLists[1].count;
            else
                objectCount = 0;
            break;
        case 3:
            selectedList = worldLists;
            objectCount = selectedList->count;
            break;
        case 4:
            selectedList = worldLists + 10;
            if (worldLists[0].count < selectedList->count)
                objectCount = selectedList->count - worldLists[0].count;
            else
                objectCount = 0;
            break;
        case 5:
            selectedList = worldLists + 2;
            objectCount = selectedList->count;
            break;
        case 6:
            selectedList = worldLists + 12;
            if (worldLists[2].count < selectedList->count)
                objectCount = selectedList->count - worldLists[2].count;
            else
                objectCount = 0;
            break;
        case 7:
            selectedList = worldLists + 3;
            objectCount = selectedList->count;
            break;
        case 8:
            selectedList = worldLists + 13;
            if (worldLists[3].count < selectedList->count)
                objectCount = selectedList->count - worldLists[3].count;
            else
                objectCount = 0;
            break;
        case 9:
            selectedList = worldLists + 4;
            objectCount = selectedList->count;
            break;
        case 10:
            selectedList = worldLists + 14;
            if (worldLists[4].count < selectedList->count)
                objectCount = selectedList->count - worldLists[4].count;
            else
                objectCount = 0;
            break;
        case 11:
            selectedList = worldLists + 5;
            objectCount = selectedList->count;
            break;
        case 12:
            selectedList = worldLists + 15;
            if (worldLists[5].count < selectedList->count)
                objectCount = selectedList->count - worldLists[5].count;
            else
                objectCount = 0;
            break;
        case 0x11:
            selectedList = worldLists + 6;
            objectCount = selectedList->count;
            break;
        case 0x12:
            selectedList = worldLists + 16;
            if (worldLists[6].count < selectedList->count)
                objectCount = selectedList->count - worldLists[6].count;
            else
                objectCount = 0;
            break;
        case 0x13:
            selectedList = worldLists + 7;
            objectCount = selectedList->count;
            break;
        case 0x14:
            selectedList = worldLists + 17;
            if (worldLists[7].count < selectedList->count)
                objectCount = selectedList->count - worldLists[7].count;
            else
                objectCount = 0;
            break;
        case 0x15:
            selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x210);
            objectCount = selectedList->count;
            break;
        case 0x16:
            selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x220);
            if (((T3DWorldObjectList_guess *)((u8 *)world + 0x210))->count < selectedList->count)
                objectCount = selectedList->count - ((T3DWorldObjectList_guess *)((u8 *)world + 0x210))->count;
            else
                objectCount = 0;
            break;
        case 0x17:
            selectedList = worldLists + 8;
            objectCount = selectedList->count;
            break;
        case 0x18:
            selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x170);
            if (worldLists[8].count < selectedList->count)
                objectCount = selectedList->count - worldLists[8].count;
            else
                objectCount = 0;
            break;
        case 0x19:
            selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x18);
            objectCount = selectedList->count;
            break;
        default:
            objectCount = 0;
            currentIndex = 0;
            objectArray = 0;
            arrayCapacity = 0;
            goto build_object_list;
        }
        goto selected_object_list;
    }

    if (objectType < 0x48)
    {
        if (objectType == 0x47)
        {
            selectedList = worldLists + 34;
            objectCount = selectedList->count;
            goto selected_object_list;
        }
        if (objectType < 0x24)
        {
            if (objectType == 0x23)
            {
                selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2f0);
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x1b)
            {
                selectedList = worldLists + 20;
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x1c)
            {
                selectedList = worldLists + 21;
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x1d)
            {
                selectedList = worldLists + 22;
                if (worldLists[21].count < selectedList->count)
                    objectCount = selectedList->count - worldLists[21].count;
                else
                    objectCount = 0;
                currentIndex = &worldLists[21].currentIndex;
                goto selected_object_list_with_index;
            }
            if (objectType == 0x1e)
            {
                selectedList = worldLists + 23;
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x1f)
            {
                selectedList = worldLists + 24;
                objectCount = selectedList->count;
                goto selected_object_list;
            }
        }
        else
        {
            if (objectType == 0x24)
            {
                selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x300);
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x44)
            {
                selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x240);
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x45)
            {
                selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x250);
                objectCount = selectedList->count;
                goto selected_object_list;
            }
            if (objectType == 0x46)
            {
                selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x260);
                objectCount = selectedList->count;
                goto selected_object_list;
            }
        }
        objectCount = 0;
        currentIndex = 0;
        objectArray = 0;
        arrayCapacity = 0;
        goto build_object_list;
    }

    switch (objectType)
    {
    case 0x48:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x280);
        objectCount = selectedList->count;
        break;
    case 0x49:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x290);
        objectCount = selectedList->count;
        break;
    case 0x4a:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2a0);
        objectCount = selectedList->count;
        break;
    case 0x4c:
        selectedList = worldLists + 9;
        objectCount = selectedList->count;
        break;
    case 0x4d:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x230);
        objectCount = selectedList->count;
        break;
    case 0x4e:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2b0);
        objectCount = selectedList->count;
        break;
    case 0x4f:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2c0);
        objectCount = selectedList->count;
        break;
    case 0x50:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2d0);
        objectCount = selectedList->count;
        break;
    case 0x51:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x2e0);
        objectCount = selectedList->count;
        break;
    case 0x53:
        selectedList = (T3DWorldObjectList_guess *)((u8 *)world + 0x368);
        objectCount = selectedList->count;
        break;
    default:
        objectCount = 0;
        currentIndex = 0;
        objectArray = 0;
        arrayCapacity = 0;
        goto build_object_list;
    }

selected_object_list:
    currentIndex = &selectedList->currentIndex;
selected_object_list_with_index:
    arrayCapacity = selectedList->capacity;
    objectArray = selectedList->objects;

build_object_list:
    if (objectCount == 0)
    {
        *objectListOut = 0;
    }
    else
    {
        allocatedList = EQ_CDECL(int, 0x10050C48, (int))(objectCount << 2);
        *objectListOut = allocatedList;
        if (allocatedList == 0)
        {
            objectCount = 0;
        }
        else
        {
            do
            {
                arrayIndex = *currentIndex;
                swapped = 0;
                sourceSlot = currentIndex;
                if (arrayCapacity <= arrayIndex)
                    break;
                do
                {
                    linkedIndex = objectArray[arrayIndex];
                    nextIndex = linkedIndex;
                    nextSlot = objectArray + arrayIndex;
                    if (linkedIndex < arrayIndex)
                    {
                        objectArray[arrayIndex] = objectArray[linkedIndex];
                        *sourceSlot = linkedIndex;
                        objectArray[linkedIndex] = arrayIndex;
                        swapped = 1;
                        nextIndex = arrayIndex;
                        nextSlot = objectArray + linkedIndex;
                    }
                    sourceSlot = nextSlot;
                    arrayIndex = nextIndex;
                } while (arrayIndex < arrayCapacity);
            } while (swapped);

            selectionThreshold = *currentIndex;
            outputCount = 0;
            outputArray = (u32 *)*objectListOut;
            outputEnd = outputArray + objectCount;
            while (outputArray < outputEnd)
            {
                if (outputCount < selectionThreshold)
                {
                    if (objectType < 0xd)
                    {
                        if (objectType == 0xc)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_2c;
                            if (objectReference->object_2c == currentObject)
                                goto next_world_object;
                        }
                        else if (objectType == 2)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_18;
                            if (objectReference->object_28 == currentObject)
                                goto next_world_object;
                        }
                        else if (objectType == 4)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_18;
                            if (objectReference->object_24 == currentObject)
                                goto next_world_object;
                        }
                        else if (objectType == 6)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_28;
                            if (objectReference->object_1c == currentObject)
                                goto next_world_object;
                        }
                        else if (objectType == 8)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_6c;
                            if (objectReference->object_14 == currentObject)
                                goto next_world_object;
                        }
                        else if (objectType == 10)
                        {
                            currentObject = *objectArray;
                            objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                            objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_88;
                            if (objectReference->object_30 == currentObject)
                                goto next_world_object;
                        }
                        else
                        {
                            currentObject = *objectArray;
                        }
                    }
                    else if (objectType == 0x12)
                    {
                        currentObject = *objectArray;
                        objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                        objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_38;
                        if (objectReference->object_24 == currentObject)
                            goto next_world_object;
                    }
                    else if (objectType == 0x14)
                    {
                        currentObject = *objectArray;
                        objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                        objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_5c;
                        if (objectReference->object_1c == currentObject)
                            goto next_world_object;
                    }
                    else if (objectType == 0x16)
                    {
                        definitionCount = EQ_CDECL(u32, 0x1000ACD6, (u32))(*objectArray);
                        defaultInstance = EQ_CDECL(u32, 0x1001D0BA, (u32))(definitionCount);
                        if (defaultInstance == *objectArray)
                            goto next_world_object;
                        currentObject = *objectArray;
                    }
                    else if (objectType == 0x18)
                    {
                        currentObject = *objectArray;
                        objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                        objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_64;
                        if (objectReference->object_28 == currentObject)
                            goto next_world_object;
                    }
                    else if (objectType == 0x1d)
                    {
                        currentObject = *objectArray;
                        objectEntry = (T3DWorldObjectEntry_guess *)currentObject;
                        objectReference = (T3DWorldObjectReference_guess *)objectEntry->link_24;
                        if (objectReference->object_20 == currentObject)
                            goto next_world_object;
                    }
                    else
                    {
                        currentObject = *objectArray;
                    }
                    *outputArray = currentObject;
                    outputArray++;
                }
                else
                {
                    selectionThreshold = *objectArray;
                }
next_world_object:
                objectArray++;
                outputCount++;
            }
        }
    }
    return objectCount;
}

// FUNCTION: EQGFX 0x1001eb54
// Lists actors in the PVS regions within range of a viewpoint that pass its view cone and an optional filter id.
u32 __cdecl t3dGetWorldVisibleActorList(t3dWorldAmbient_guess *world, T3DLocation_guess *location, float viewAngle, float aspectRatio, float maxDistance, int actorFilterId, T3DActorInstance_guess ***visibleActorsOut, u32 visibilityThreshold)
{
    char errorMessage[256];
    float maxDistanceSquared;
    u32 worldRegionCount;
    T3DRegion_guess *originRegion;
    T3DPVS_guess *pvs;
    T3DPVSNode_guess *node;
    T3DPVSData_guess *leaf;
    u16 *decodedRegions;
    u16 *decodedWrite;
    u16 *code;
    u16 *codeEnd;
    u16 *regionCursor;
    u32 currentRegion;
    u32 runLength;
    u32 runCount;
    u32 regionIndex;
    int visibleRegionCount;
    int visibleRegionIndex;
    u16 encoded;
    u16 encodedValue;
    u16 encodedKind;
    T3DActorInstance_guess **visibleActors;
    int visibleActorCount;
    int visibleActorCapacity;
    T3DRegion_guess *candidateRegion;
    T3DRegionActorList_guess *regionActors;
    u32 *actorIndex;
    u32 *actorIndexEnd;
    T3DActorInstance_guess *actor;
    T3DActorInstance_guess **actorScan;
    T3DActorInstance_guess **actorScanEnd;
    T3DActorInstance_guess **grownActors;
    int actorIndexValue;
    int allocationBytes;
    float negX;
    float negY;
    float negZ;
    int frustumEnabled;
    float cosineHeading;
    float cosineRoll;
    float cosinePitch;
    float sineHeading;
    float sineRoll;
    float sinePitch;
    float viewForwardX;
    float viewForwardY;
    float viewForwardZ;
    float viewSideX;
    float viewSideY;
    float viewSideZ;
    float viewUpX;
    float viewUpY;
    float viewUpZ;
    float frustumCosine;
    float frustumSine;
    float tangentScale;
    float lookupIndexFloat;
    float arcInput;
    float angleValue;
    float centerX;
    float centerY;
    float centerZ;
    float forwardDistance;
    float sideDistance;
    float upDistance;
    float radialDistance;
    float actorX;
    float actorY;
    float actorZ;
    float distanceX;
    float distanceY;
    float distanceZ;
    int lookupIndex;
    int angleResult;
    u8 *startupBlock;

    maxDistanceSquared = maxDistance * maxDistance;
    worldRegionCount = world->regionCount;
    negX = -location->x;
    negY = -location->y;
    negZ = -location->z;
    visibleActors = 0;
    visibleActorCount = 0;
    visibleActorCapacity = 0;
    decodedRegions = 0;
    pvs = 0;
    originRegion = 0;
    if ((u32)location->region >= worldRegionCount)
    {
        EQ_CDECL(int, 0x10067992, (char *, const char *, ...))(errorMessage, (const char *)0x10086F84, location->region, worldRegionCount);
        goto write_output;
    }
    originRegion = world->regions[location->region];
    pvs = originRegion->pvs;
    if (pvs->kind != T3D_PVS_KIND)
        goto write_output;
    node = pvs->tree;
    while (node->leaf == 0)
    {
        if (node->planeDistance <= negX * node->normal[0] + negY * node->normal[1] + negZ * node->normal[2])
            node = node->frontChild;
        else
            node = node->backChild;
    }
    leaf = node->leaf;
    visibleRegionCount = leaf->count;
    code = leaf->codes;
    if (pvs->encoding == 1)
    {
        decodedRegions = (u16 *)EQ_CDECL(void *, 0x10050C48, (int))(worldRegionCount * 2);
        if (decodedRegions == 0)
            goto cleanup;
        currentRegion = 0;
        decodedWrite = decodedRegions;
        codeEnd = code + visibleRegionCount;
        for (; code < codeEnd; code++)
        {
            encoded = *code;
            encodedValue = encoded & 0x3fff;
            encodedKind = encoded & 0xc000;
            if (encodedKind == 0)
            {
                currentRegion = currentRegion + encodedValue;
            }
            else if (encodedKind == 0x4000)
            {
                runLength = encodedValue & 0x7f;
                currentRegion = currentRegion + (encodedValue >> 7);
                while (runLength != 0)
                {
                    if ((u16)currentRegion != location->region)
                    {
                        *decodedWrite = (u16)currentRegion;
                        decodedWrite++;
                    }
                    currentRegion++;
                    runLength--;
                }
            }
            else if (encodedKind == 0x8000)
            {
                runLength = encodedValue & 0x7f;
                runCount = encodedValue >> 7;
                while (runCount != 0)
                {
                    if ((u16)currentRegion != location->region)
                    {
                        *decodedWrite = (u16)currentRegion;
                        decodedWrite++;
                    }
                    currentRegion++;
                    runCount--;
                }
                currentRegion = currentRegion + runLength;
            }
            else if (encodedKind == 0xc000 && encodedValue != 0)
            {
                runLength = encodedValue;
                while (runLength != 0)
                {
                    if ((u16)currentRegion != location->region)
                    {
                        *decodedWrite = (u16)currentRegion;
                        decodedWrite++;
                    }
                    currentRegion++;
                    runLength--;
                }
            }
        }
        visibleRegionCount = (int)(decodedWrite - decodedRegions);
        code = decodedRegions;
    }

    frustumEnabled = 1;
    if (parseColorScale <= viewAngle)
    {
        frustumEnabled = 0;
    }
    else
    {
        if (viewAngle <= EQGFX_FLOAT_ZERO)
            goto cleanup;
        startupBlock = EQGFX_STARTUP_BLOCK;
        cosineHeading = EQ_CDECL(float, 0x1004EEA3, (float))(location->heading);
        cosineRoll = EQ_CDECL(float, 0x1004EEA3, (float))(location->roll);
        cosinePitch = EQ_CDECL(float, 0x1004EEA3, (float))(location->pitch);
        sineHeading = EQ_CDECL(float, 0x1004EE37, (float))(location->heading);
        sineRoll = EQ_CDECL(float, 0x1004EE37, (float))(location->roll);
        sinePitch = EQ_CDECL(float, 0x1004EE37, (float))(location->pitch);
        viewForwardX = cosinePitch * cosineHeading;
        viewForwardY = sineHeading * cosinePitch;
        viewForwardZ = sinePitch;
        viewSideX = sineHeading * cosineRoll + sinePitch * sineRoll * cosineHeading;
        viewSideY = sinePitch * sineRoll * sineHeading - cosineRoll * cosineHeading;
        viewSideZ = -sineRoll * cosinePitch;
        viewUpX = sineRoll * sineHeading - sinePitch * cosineRoll * cosineHeading;
        viewUpY = -sineRoll * cosineHeading - sinePitch * cosineRoll * sineHeading;
        viewUpZ = cosinePitch * cosineRoll;
        tangentScale = EQ_CDECL(float, 0x1004EF68, (float))(EQGFX_FLOAT_ONE / (aspectRatio * aspectRatio) + EQGFX_FLOAT_ONE);
        lookupIndexFloat = viewAngle * EQGFX_RANGE_SCALE;
        __asm
        {
            fld tangentScale
            fld lookupIndexFloat
            mov eax, T3D_ROUND_FLOAT_TO_INDEX_ADDRESS
            call eax
            mov lookupIndex, eax
            fstp tangentScale
        }
        arcInput = tangentScale * *(float *)(startupBlock + T3D_SCALE_TABLE_OFFSET - lookupIndex * T3D_SCALE_RECORD_SIZE);
        angleResult = 0;
        angleValue = EQ_CDECL(float, 0x1004ECF9, (float, float, int *))(arcInput, 1.0f, &angleResult);
        __asm
        {
            fld angleValue
            mov eax, T3D_ROUND_FLOAT_TO_INDEX_ADDRESS
            call eax
            mov lookupIndex, eax
        }
        if (angleResult == 0)
            lookupIndex++;
        frustumCosine = *(float *)(startupBlock + T3D_COSINE_TABLE_OFFSET + lookupIndex * 4);
        frustumSine = *(float *)(startupBlock + T3D_SINE_TABLE_OFFSET + lookupIndex * 4);
    }

    if (visibleRegionCount >= 0)
    {
        regionCursor = code + visibleRegionCount;
        visibleRegionIndex = visibleRegionCount;
        do
        {
            if (visibleRegionIndex == visibleRegionCount)
            {
                candidateRegion = originRegion;
            }
            else
            {
                regionIndex = *regionCursor;
                candidateRegion = world->regions[regionIndex];
            }
            distanceX = location->x - candidateRegion->centerX;
            distanceY = location->y - candidateRegion->centerY;
            distanceZ = location->z - candidateRegion->centerZ;
            if (distanceZ * distanceZ + distanceX * distanceX + distanceY * distanceY <= maxDistanceSquared)
            {
                regionActors = candidateRegion->actorList;
                if (regionActors->kind == 0x27 || regionActors->kind == 0x29)
                {
                    if (frustumEnabled != 0)
                    {
                        centerX = negX + candidateRegion->centerX;
                        centerY = negY + candidateRegion->centerY;
                        centerZ = negZ + candidateRegion->centerZ;
                        forwardDistance = centerX * viewForwardX + centerY * viewForwardY + centerZ * viewForwardZ;
                        if (forwardDistance >= -candidateRegion->boundsRadius)
                        {
                            sideDistance = centerX * viewSideX + centerY * viewSideY + centerZ * viewSideZ;
                            upDistance = centerX * viewUpX + centerY * viewUpY + centerZ * viewUpZ;
                            radialDistance = EQ_CDECL(float, 0x1004EF68, (float))(sideDistance * sideDistance + upDistance * upDistance);
                            if (radialDistance * frustumCosine - forwardDistance * frustumSine <= candidateRegion->boundsRadius)
                                goto region_visible;
                        }
                    }
                    else
                    {
region_visible:
                        actorIndex = regionActors->actorIndices;
                        actorIndexEnd = actorIndex + regionActors->actorCount;
                        for (; actorIndex < actorIndexEnd; actorIndex++)
                        {
                            actorIndexValue = (int)*actorIndex;
                            actor = world->actorInstances[actorIndexValue];
                            if (visibilityThreshold <= actor->visibilityLimit &&
                                (actorFilterId == 0 || actor->actorFilterId == actorFilterId))
                            {
                                if (frustumEnabled != 0)
                                {
                                    centerX = negX + actor->x;
                                    centerY = negY + actor->y;
                                    centerZ = negZ + actor->z;
                                    forwardDistance = centerX * viewForwardX + centerY * viewForwardY + centerZ * viewForwardZ;
                                    if (forwardDistance < -actor->boundsHeight)
                                        continue;
                                    sideDistance = centerX * viewSideX + centerY * viewSideY + centerZ * viewSideZ;
                                    upDistance = centerX * viewUpX + centerY * viewUpY + centerZ * viewUpZ;
                                    radialDistance = EQ_CDECL(float, 0x1004EF68, (float))(sideDistance * sideDistance + upDistance * upDistance);
                                    if (radialDistance * frustumCosine - forwardDistance * frustumSine > actor->boundsHeight)
                                        continue;
                                }
                                actorScan = visibleActors;
                                actorScanEnd = visibleActors + visibleActorCount;
                                for (;;)
                                {
                                    if (actorScanEnd <= actorScan)
                                    {
                                        grownActors = visibleActors;
                                        if (visibleActorCapacity <= visibleActorCount)
                                        {
                                            if (visibleActors == 0)
                                            {
                                                visibleActors = (T3DActorInstance_guess **)EQ_CDECL(void *, 0x10050C48, (int))(0x400);
                                                if (visibleActors == 0)
                                                    goto cleanup;
                                                visibleActorCapacity = 0x100;
                                                grownActors = visibleActors;
                                            }
                                            else
                                            {
                                                allocationBytes = visibleActorCapacity << 3;
                                                visibleActorCapacity = visibleActorCapacity * 2;
                                                grownActors = (T3DActorInstance_guess **)EQ_CDECL(void *, 0x10050C8C, (void *, int))(visibleActors, allocationBytes);
                                                if (grownActors == 0)
                                                {
                                                    EQ_CDECL(void, 0x10050CBB, (void *))(visibleActors);
                                                    visibleActors = 0;
                                                    visibleActorCount = 0;
                                                    goto cleanup;
                                                }
                                            }
                                        }
                                        visibleActors = grownActors;
                                        visibleActors[visibleActorCount] = actor;
                                        visibleActorCount++;
                                        break;
                                    }
                                    if (*actorScan == actor)
                                        break;
                                    actorScan++;
                                }
                            }
                        }
                    }
                }
            }
            visibleRegionIndex--;
            regionCursor--;
        } while (visibleRegionIndex >= 0);
    }

cleanup:
    if (pvs != 0 && pvs->encoding == 1 && decodedRegions != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(decodedRegions);
write_output:
    *visibleActorsOut = visibleActors;
    return visibleActorCount;
}

// FUNCTION: EQGFX 0x1001f156
// Lists the regions on a viewpoint's PVS (its own included) whose bounds reach the view cone; no distance cut.
u32 __cdecl t3dGetWorldVisibleRegionList(t3dWorldAmbient_guess *world, T3DLocation_guess *location, float range, float ratio, int *visibleListOut)
{
    T3DRegion_guess *originRegion;
    T3DPVS_guess *pvs;
    T3DPVSNode_guess *node;
    T3DPVSData_guess *leaf;
    T3DRegion_guess *candidateRegion;
    u16 *list;
    u16 *cursor;
    u16 *decodeEnd;
    u16 *decodedList;
    u16 *decodedOut;
    u16 *entry;
    u16 code;
    u16 kind;
    u16 countWord;
    int listCount;
    int listIndex;
    int regionIndex;
    int scaleIndex;
    int coneIndex;
    int atanFlag;
    int useCone;
    void *visibleList;
    u32 visibleCount;
    u32 outputCapacity;
    u32 runIndex;
    u32 literalCount;
    u32 runLength;
    u32 skipCount;
    u32 *output;
    float negX;
    float negY;
    float negZ;
    float cosHeading;
    float cosRoll;
    float cosPitch;
    float sinHeading;
    float sinRoll;
    float sinPitch;
    float sinPitchCosRoll;
    float uX;
    float vX;
    float wX;
    float uY;
    float vY;
    float wY;
    float uZ;
    float vZ;
    float wZ;
    float coneCos;
    float coneSin;
    float scaleEntry;
    float product;
    float dx;
    float dy;
    float dz;
    float depth;
    float lateralU;
    float lateralV;
    float threshold;

    negX = -location->x;
    originRegion = world->regions[location->region];
    negY = -location->y;
    pvs = originRegion->pvs;
    visibleList = 0;
    negZ = -location->z;
    visibleCount = 0;
    outputCapacity = 0;
    decodedList = 0;
    if (pvs->kind != T3D_PVS_KIND)
        goto finish;
    node = pvs->tree;
    while (node->leaf == 0)
    {
        if (node->planeDistance <= negX * node->normal[0] + negY * node->normal[1] + negZ * node->normal[2])
            node = node->frontChild;
        else
            node = node->backChild;
    }
    leaf = node->leaf;
    listCount = leaf->count;
    list = leaf->codes;
    if (pvs->encoding == 1)
    {
        decodedList = (u16 *)EQ_CDECL(void *, 0x10050C48, (u32))(world->regionCount << 1);
        if (decodedList == 0)
            goto cleanup;
        decodeEnd = list + listCount;
        runIndex = 0;
        decodedOut = decodedList;
        for (cursor = list; cursor < decodeEnd; cursor++)
        {
            code = *cursor;
            literalCount = code & 0x3fff;
            kind = code & 0xc000;
            if (kind == 0)
            {
                runIndex += literalCount;
            }
            else
            {
                countWord = (u16)literalCount;
                if (kind == 0x4000)
                {
                    runLength = code & 0x7f;
                    runIndex += countWord >> 7;
                    if (runLength != 0)
                    {
                        do
                        {
                            if ((runIndex & 0xffff) != (u32)location->region)
                            {
                                *decodedOut = (u16)runIndex;
                                decodedOut++;
                            }
                            runIndex++;
                            runLength--;
                        } while (runLength != 0);
                    }
                }
                else if (kind == 0x8000)
                {
                    runLength = code & 0x7f;
                    skipCount = countWord >> 7;
                    if (skipCount != 0)
                    {
                        do
                        {
                            if ((runIndex & 0xffff) != (u32)location->region)
                            {
                                *decodedOut = (u16)runIndex;
                                decodedOut++;
                            }
                            runIndex++;
                            skipCount--;
                        } while (skipCount != 0);
                    }
                    runIndex += runLength;
                }
                else if (kind == 0xc000 && countWord != 0)
                {
                    do
                    {
                        if ((runIndex & 0xffff) != (u32)location->region)
                        {
                            *decodedOut = (u16)runIndex;
                            decodedOut++;
                        }
                        runIndex++;
                        literalCount--;
                    } while (literalCount != 0);
                }
            }
        }
        listCount = (int)(decodedOut - decodedList);
        list = decodedList;
    }
    if (range >= parseColorScale)
    {
        useCone = 0;
    }
    else
    {
        if (range <= EQGFX_FLOAT_ZERO)
            goto cleanup;
        useCone = 1;
        cosHeading = EQ_CDECL(float, 0x1004EEA3, (float))(location->heading);
        cosRoll = EQ_CDECL(float, 0x1004EEA3, (float))(location->roll);
        cosPitch = EQ_CDECL(float, 0x1004EEA3, (float))(location->pitch);
        sinHeading = EQ_CDECL(float, 0x1004EE37, (float))(location->heading);
        sinRoll = EQ_CDECL(float, 0x1004EE37, (float))(location->roll);
        sinPitch = EQ_CDECL(float, 0x1004EE37, (float))(location->pitch);
        sinPitchCosRoll = sinPitch * cosRoll;
        uX = sinHeading * cosRoll + sinPitch * sinRoll * cosHeading;
        vX = sinRoll * sinHeading - sinPitchCosRoll * cosHeading;
        wX = cosPitch * cosHeading;
        uY = sinPitch * sinRoll * sinHeading - cosRoll * cosHeading;
        vY = -(sinRoll * cosHeading) - sinPitchCosRoll * sinHeading;
        wY = sinHeading * cosPitch;
        uZ = -(sinRoll * cosPitch);
        vZ = cosPitch * cosRoll;
        wZ = sinPitch;
        EQ_CDECL(void, 0x1004EF68, (float))(EQGFX_FLOAT_ONE / (ratio * ratio) + EQGFX_FLOAT_ONE);
        __asm
        {
            fld dword ptr [range]
            fmul dword ptr EQ_ASM_GLOBAL(0x100762B0)
        }
        scaleIndex = EQ_CDECL(int, 0x1004EC36, (void))();
        scaleEntry = *(float *)(EQGFX_STARTUP_BLOCK - scaleIndex * T3D_SCALE_RECORD_SIZE + T3D_SCALE_TABLE_OFFSET);
        __asm
        {
            fmul dword ptr [scaleEntry]
            fstp dword ptr [product]
        }
        EQ_CDECL(void, 0x1004ECF9, (float, float, int *))(product, 1.0f, &atanFlag);
        coneIndex = EQ_CDECL(int, 0x1004EC36, (void))();
        if (atanFlag == 0)
            coneIndex++;
        coneCos = *(float *)(EQGFX_STARTUP_BLOCK + T3D_COSINE_TABLE_OFFSET + coneIndex * 4);
        coneSin = *(float *)(EQGFX_STARTUP_BLOCK + T3D_SINE_TABLE_OFFSET + coneIndex * 4);
    }
    listIndex = listCount;
    if (listIndex >= 0)
    {
        entry = list + listCount;
        do
        {
            candidateRegion = originRegion;
            if (listIndex != listCount)
            {
                regionIndex = *entry;
                if (regionIndex < 0)
                {
                    listIndex--;
                    entry--;
                    regionIndex = *entry;
                }
                candidateRegion = world->regions[regionIndex];
            }
            if (useCone == 0)
                goto append_region;
            dx = negX + candidateRegion->centerX;
            dy = negY + candidateRegion->centerY;
            dz = negZ + candidateRegion->centerZ;
            depth = dx * wX + dy * wY + dz * wZ;
            if (-candidateRegion->boundsRadius <= depth)
            {
                lateralU = dx * uX + dy * uY + dz * uZ;
                lateralV = dx * vX + dy * vY + dz * vZ;
                threshold = EQ_CDECL(float, 0x1004EF68, (float))(lateralU * lateralU + lateralV * lateralV) * coneCos - depth * coneSin;
                if (threshold <= candidateRegion->boundsRadius)
                    goto append_region;
            }
            goto next_region;
append_region:
            if (outputCapacity <= visibleCount)
            {
                if (visibleList == 0)
                {
                    visibleList = EQ_CDECL(void *, 0x10050C48, (u32))(T3D_VISIBLE_REGION_LIST_INITIAL_BYTES);
                    if (visibleList == 0)
                        break;
                    outputCapacity = T3D_VISIBLE_REGION_LIST_INITIAL_CAPACITY;
                }
                else
                {
                    outputCapacity = outputCapacity * 2;
                    output = (u32 *)EQ_CDECL(void *, 0x10050C8C, (void *, u32))(visibleList, outputCapacity << 2);
                    if (output == 0)
                    {
                        EQ_CDECL(void, 0x10050CBB, (void *))(visibleList);
                        visibleList = 0;
                        visibleCount = 0;
                        break;
                    }
                    visibleList = output;
                }
            }
            output = (u32 *)visibleList;
            output[visibleCount] = (u32)candidateRegion;
            visibleCount++;
next_region:
            listIndex--;
            entry--;
        } while (listIndex >= 0);
    }
cleanup:
    if (pvs != 0 && pvs->encoding == 1 && decodedList != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(decodedList);
finish:
    *visibleListOut = (int)visibleList;
    return visibleCount;
}

// FUNCTION: EQGFX 0x1001f5c5
// Region visibility check from a viewpoint: the viewer leaf's PVS list, then an optional view-cone cull.
// __cdecl, int: 1 when targetRegion is visible from location: its own region with range > 0, or on its leaf's PVS list and inside the cone when range < 256.
int __cdecl t3dIsRegionVisibleFromLocation(t3dWorldAmbient_guess *world, T3DLocation_guess *location, float range, float ratio, int targetRegion)
{
    int visible;
    int count;
    int index;
    int regionIndex;
    int atanFlag;
    int scaleIndex;
    int coneIndex;
    int useCone = 0;
    u16 *decodedList;
    u16 *list;
    u16 *entry;
    u16 *cursor;
    u16 *decodeEnd;
    u16 *out;
    u16 code;
    u16 kind;
    u16 countWord;
    u32 literalCount;
    u32 runLength;
    u32 skipCount;
    u32 runIndex;
    T3DPVS_guess *pvs;
    T3DPVSNode_guess *node;
    T3DPVSData_guess *leaf;
    T3DRegion_guess *region;
    float negX;
    float negY;
    float negZ;
    float cosHeading;
    float cosRoll;
    float cosPitch;
    float sinHeading;
    float sinRoll;
    float sinPitch;
    float sinPitchCosRoll;
    float uX;
    float uY;
    float uZ;
    float vX;
    float vY;
    float vZ;
    float wX;
    float wY;
    float wZ;
    float product;
    float scaleEntry;
    float coneCos;
    float coneSin;
    float dx;
    float dy;
    float dz;
    float depth;
    float lateralU;
    float lateralV;
    float threshold;

    visible = 0;
    decodedList = 0;
    if (location->region == targetRegion && range > EQGFX_FLOAT_ZERO)
        return 1;
    negX = -location->x;
    region = world->regions[location->region];
    negY = -location->y;
    pvs = region->pvs;
    negZ = -location->z;
    if (pvs->kind != T3D_PVS_KIND)
        return 0;
    node = pvs->tree;
    while (node->leaf == 0)
    {
        if (node->planeDistance <= negX * node->normal[0] + (negY * node->normal[1] + negZ * node->normal[2]))
            node = node->frontChild;
        else
            node = node->backChild;
    }
    leaf = node->leaf;
    count = leaf->count;
    list = leaf->codes;
    if (pvs->encoding == 1)
    {
        decodedList = EQ_CDECL(void *, 0x10050C48, (u32))(world->regionCount << 1);
        if (decodedList == 0)
            goto cleanup;
        decodeEnd = list + count;
        runIndex = 0;
        out = decodedList;
        for (cursor = list; cursor < decodeEnd; cursor++)
        {
            code = *cursor;
            literalCount = code & 0x3fff;
            kind = code & 0xc000;
            if (kind == 0)
            {
                runIndex += literalCount;
            }
            else
            {
                countWord = (u16)literalCount;
                if (kind == 0x4000)
                {
                    runLength = code & 0x7f;
                    runIndex += countWord >> 7;
                    if (runLength != 0)
                    {
                        do
                        {
                            if ((runIndex & 0xffff) != location->region)
                            {
                                *out = (u16)runIndex;
                                out++;
                            }
                            runIndex++;
                            runLength--;
                        } while (runLength != 0);
                    }
                }
                else if (kind == 0x8000)
                {
                    runLength = code & 0x7f;
                    skipCount = countWord >> 7;
                    if (skipCount != 0)
                    {
                        do
                        {
                            if ((runIndex & 0xffff) != location->region)
                            {
                                *out = (u16)runIndex;
                                out++;
                            }
                            runIndex++;
                            skipCount--;
                        } while (skipCount != 0);
                    }
                    runIndex += runLength;
                }
                else if (kind == 0xc000 && countWord != 0)
                {
                    do
                    {
                        if ((runIndex & 0xffff) != location->region)
                        {
                            *out = (u16)runIndex;
                            out++;
                        }
                        runIndex++;
                        literalCount--;
                    } while (literalCount != 0);
                }
            }
        }
        count = out - decodedList;
        list = decodedList;
    }
    if (range >= parseColorScale)
    {
        useCone = 0;
    }
    else
    {
        if (range <= EQGFX_FLOAT_ZERO)
            goto cleanup;
        useCone = 1;
        cosHeading = EQ_CDECL(float, 0x1004EEA3, (float))(location->heading);
        cosRoll = EQ_CDECL(float, 0x1004EEA3, (float))(location->roll);
        cosPitch = EQ_CDECL(float, 0x1004EEA3, (float))(location->pitch);
        sinHeading = EQ_CDECL(float, 0x1004EE37, (float))(location->heading);
        sinRoll = EQ_CDECL(float, 0x1004EE37, (float))(location->roll);
        sinPitch = EQ_CDECL(float, 0x1004EE37, (float))(location->pitch);
        sinPitchCosRoll = sinPitch * cosRoll;
        uX = sinHeading * cosRoll + sinPitch * sinRoll * cosHeading;
        vX = sinRoll * sinHeading - sinPitchCosRoll * cosHeading;
        wX = cosPitch * cosHeading;
        uY = sinPitch * sinRoll * sinHeading - cosRoll * cosHeading;
        vY = -(sinRoll * cosHeading) - sinPitchCosRoll * sinHeading;
        wY = sinHeading * cosPitch;
        uZ = -(sinRoll * cosPitch);
        vZ = cosPitch * cosRoll;
        wZ = sinPitch;
        EQ_CDECL(void, 0x1004EF68, (float))(EQGFX_FLOAT_ONE / (ratio * ratio) + EQGFX_FLOAT_ONE);
        __asm
        {
            fld dword ptr [range]
            fmul dword ptr EQ_ASM_GLOBAL(0x100762B0)
        }
        scaleIndex = EQ_CDECL(int, 0x1004EC36, (void))();
        scaleEntry = *(float *)(EQGFX_STARTUP_BLOCK - scaleIndex * T3D_SCALE_RECORD_SIZE + T3D_SCALE_TABLE_OFFSET);
        __asm
        {
            fmul dword ptr [scaleEntry]
            fstp dword ptr [product]
        }
        EQ_CDECL(void, 0x1004ECF9, (float, float, void *))(product, 1.0f, &atanFlag);
        coneIndex = EQ_CDECL(int, 0x1004EC36, (void))();
        if (atanFlag == 0)
            coneIndex++;
        coneCos = *(float *)(EQGFX_STARTUP_BLOCK + T3D_COSINE_TABLE_OFFSET + coneIndex * 4);
        coneSin = *(float *)(EQGFX_STARTUP_BLOCK + T3D_SINE_TABLE_OFFSET + coneIndex * 4);
    }
    index = count - 1;
    if (index >= 0)
    {
        entry = list + index;
        do
        {
            regionIndex = *entry;
            if (regionIndex < 0)
            {
                index--;
                entry--;
                regionIndex = *entry;
            }
            region = world->regions[regionIndex];
            if (region->regionId == targetRegion)
            {
                if (useCone)
                {
                    dx = negX + region->centerX;
                    dy = negY + region->centerY;
                    dz = negZ + region->centerZ;
                    depth = dx * wX + (dy * wY + dz * wZ);
                    if (depth < -region->boundsRadius)
                        break;
                    lateralU = dx * uX + (dy * uY + dz * uZ);
                    lateralV = dx * vX + (dy * vY + dz * vZ);
                    threshold = EQ_CDECL(float, 0x1004EF68, (float))(lateralU * lateralU + lateralV * lateralV) * coneCos - depth * coneSin;
                    if (region->boundsRadius < threshold)
                        break;
                }
                visible = 1;
                break;
            }
            index--;
            entry--;
        } while (index >= 0);
    }
cleanup:
    if (pvs != 0 && pvs->encoding == 1 && decodedList != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(decodedList);
    return visible;
}

// FUNCTION: EQGFX 0x1001f9da
// __cdecl, returns 0. Drops the world's temporary tags, freeing keys and blocks.
int __cdecl t3dWorldRemoveTemporaryTags(t3dWorldAmbient_guess *world)
{
    void **current;
    void **end;
    T3dTemporaryTag_guess *tag;
    u32 *flags;
    void **slot;

    if (world->temporaryTags != 0)
    {
        end = world->temporaryTags + world->temporaryTagCount;
        for (current = world->temporaryTags; current < end; current++)
        {
            tag = (T3dTemporaryTag_guess *)*current;
            switch (tag->kind)
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
            case 17:
            case 18:
            case 19:
            case 20:
            case 23:
            case 24:
            case 25:
            case 26:
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
            case 35:
            case 36:
            case 68:
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
            case 76:
            case 77:
            case 78:
            case 79:
            case 80:
            case 81:
            case 83:
                flags = &tag->flags;
                slot = &tag->entry;
                break;
            default:
                flags = 0;
                break;
            }
            if (flags != 0)
            {
                if (((*flags & 1) != 0) && ((*flags & 6) != 0))
                {
                    EQ_CDECL(void, 0x1000E334, (void *, void *))(world, *slot);
                }
                if ((*flags & 4) != 0)
                {
                    EQ_CDECL(void, 0x10050CBB, (void *))(*slot);
                    *slot = 0;
                }
                *flags &= 0xfffffff9;
            }
        }
        EQ_CDECL(void, 0x10050CBB, (void *))(world->temporaryTags);
        world->temporaryTags = 0;
    }
    world->unknown_32c = 0;
    world->temporaryTagCount = 0;
    return 0;
}

// FUNCTION: EQGFX 0x1001fb63
// __cdecl. Sets the sprite's user frame-control value at +0x34.
int __cdecl t3dSetHierarchicalSpriteUserFrameControl(T3DHierarchicalSprite_guess *sprite, int userFrameControl)
{
    sprite->userFrameControl = userFrameControl;
    return 0;
}

#define S3D_RAY_SEGMENT_BACKOFF 0.0001f

// FUNCTION: EQGFX 0x1001fb71
// Finds where a line segment first meets blocking world geometry.
int __cdecl s3dRaySegmentHitsWorldSolid(t3dWorldAmbient_guess *world, u32 unusedContext,
    float *segmentStart, float *segmentEnd, float *hitPoint)
{
    int result;
    int reallocationResult;
    float currentStart[3];
    float currentEnd[3];
    float originalStart[3];
    float originalEnd[3];
    float savedStart[3];
    float startDistance;
    float endDistance;
    float fraction;
    T3DWorldSolidNode_guess *node;
    T3DWorldSolidSegment_guess *stackCursor;

    reallocationResult = EQ_CDECL(int, 0x10006E31, (t3dWorldAmbient_guess *))(world);
    if (reallocationResult == 0)
    {
        currentStart[0] = segmentStart[0];
        currentStart[1] = segmentStart[1];
        currentStart[2] = segmentStart[2];
        originalStart[0] = segmentStart[0];
        originalStart[1] = segmentStart[1];
        originalStart[2] = segmentStart[2];
        currentEnd[0] = segmentEnd[0];
        currentEnd[1] = segmentEnd[1];
        currentEnd[2] = segmentEnd[2];
        originalEnd[0] = segmentEnd[0];
        originalEnd[1] = segmentEnd[1];
        originalEnd[2] = segmentEnd[2];
        node = world->solidTreeRoot;
        stackCursor = world->clipSegmentStack - 1;

        do
        {
            do
            {
                if (node == 0)
                    goto segmentHitsWorldSolid;
                if (node->isSolid == 0)
                {
                    startDistance = currentStart[0] * node->plane[0]
                        + currentStart[1] * node->plane[1]
                        + currentStart[2] * node->plane[2] + node->plane[3];
                    endDistance = currentEnd[0] * node->plane[0]
                        + currentEnd[1] * node->plane[1]
                        + currentEnd[2] * node->plane[2] + node->plane[3];
                    if (startDistance < 0.0f)
                    {
                        if (endDistance >= 0.0f)
                        {
                            fraction = startDistance / (startDistance - endDistance);
                            stackCursor[1].node = node;
                            stackCursor[1].savedPoint[0] = currentEnd[0];
                            stackCursor[1].savedPoint[1] = currentEnd[1];
                            stackCursor[1].savedPoint[2] = currentEnd[2];
                            stackCursor[1].state = 3;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * fraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * fraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * fraction + currentStart[2];
                            stackCursor++;
                        }
                        node = node->negativeChild;
                    }
                    else
                    {
                        if (endDistance < 0.0f)
                        {
                            fraction = startDistance / (startDistance - endDistance);
                            stackCursor[1].node = node;
                            stackCursor[1].savedPoint[0] = currentEnd[0];
                            stackCursor[1].savedPoint[1] = currentEnd[1];
                            stackCursor[1].savedPoint[2] = currentEnd[2];
                            stackCursor[1].state = 1;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * fraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * fraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * fraction + currentStart[2];
                            stackCursor++;
                        }
                        node = node->positiveChild;
                    }
                }
                savedStart[0] = currentEnd[0];
                savedStart[1] = currentEnd[1];
                savedStart[2] = currentEnd[2];
                if (node == 0)
                    goto segmentHitsWorldSolid;
            } while (node->isSolid == 0);

            for (;;)
            {
                if (stackCursor < world->clipSegmentStack)
                    goto segmentMissesWorldSolid;
                if (stackCursor->state == 1)
                {
                    node = stackCursor->node->negativeChild;
                    currentEnd[0] = stackCursor->savedPoint[0];
                    currentEnd[1] = stackCursor->savedPoint[1];
                    currentEnd[2] = stackCursor->savedPoint[2];
                    stackCursor->savedPoint[0] = currentStart[0];
                    stackCursor->savedPoint[1] = currentStart[1];
                    stackCursor->savedPoint[2] = currentStart[2];
                    stackCursor->state = 4;
                    goto resumeWorldSolidTraversal;
                }
                if (stackCursor->state == 3)
                    break;
                currentStart[0] = stackCursor->savedPoint[0];
                currentStart[1] = stackCursor->savedPoint[1];
                currentStart[2] = stackCursor->savedPoint[2];
                stackCursor--;
            }
            node = stackCursor->node->positiveChild;
            currentEnd[0] = stackCursor->savedPoint[0];
            currentEnd[1] = stackCursor->savedPoint[1];
            currentEnd[2] = stackCursor->savedPoint[2];
            stackCursor->savedPoint[0] = currentStart[0];
            stackCursor->savedPoint[1] = currentStart[1];
            stackCursor->savedPoint[2] = currentStart[2];
            stackCursor->state = 2;
resumeWorldSolidTraversal:
            currentStart[0] = savedStart[0];
            currentStart[1] = savedStart[1];
            currentStart[2] = savedStart[2];
        } while (world->clipSegmentStack <= stackCursor);

segmentMissesWorldSolid:
        hitPoint[1] = originalEnd[1];
        hitPoint[0] = originalEnd[0];
        hitPoint[2] = originalEnd[2];
        result = 0;
    }
    else
    {
        result = -1;
    }
    return result;

segmentHitsWorldSolid:
    hitPoint[0] = currentStart[0] - (originalEnd[0] - originalStart[0]) * S3D_RAY_SEGMENT_BACKOFF;
    hitPoint[1] = currentStart[1] - (originalEnd[1] - originalStart[1]) * S3D_RAY_SEGMENT_BACKOFF;
    hitPoint[2] = currentStart[2] - (originalEnd[2] - originalStart[2]) * S3D_RAY_SEGMENT_BACKOFF;
    return 1;
}

// FUNCTION: EQGFX 0x1001fdd1
// Segment trace through the world BSP, stopping at solid space or a region whose flags match the mask.
u32 __cdecl s3dRaySegmentHitsWorldSolidOrEnvironment(t3dWorldAmbient_guess *world, u32 unusedContext,
    float *segmentStart, float *segmentEnd, u32 environmentMask, float *hitPoint)
{
    u32 result;
    int reallocationResult;
    float currentStart[3];
    float currentEnd[3];
    float originalStart[3];
    float originalEnd[3];
    float savedStart[3];
    float startDistance;
    float endDistance;
    float fraction;
    T3DWorldSolidNode_guess *node;
    T3DWorldSolidSegment_guess *stackCursor;
    T3DRegion_guess *environment;

    reallocationResult = EQ_CDECL(int, 0x10006E31, (t3dWorldAmbient_guess *))(world);
    if (reallocationResult == 0)
    {
        currentStart[0] = segmentStart[0];
        currentStart[1] = segmentStart[1];
        currentStart[2] = segmentStart[2];
        originalStart[0] = segmentStart[0];
        originalStart[1] = segmentStart[1];
        originalStart[2] = segmentStart[2];
        currentEnd[0] = segmentEnd[0];
        currentEnd[1] = segmentEnd[1];
        currentEnd[2] = segmentEnd[2];
        originalEnd[0] = segmentEnd[0];
        originalEnd[1] = segmentEnd[1];
        originalEnd[2] = segmentEnd[2];
        node = world->solidTreeRoot;
        stackCursor = world->clipSegmentStack - 1;

        do
        {
            do
            {
                if (node == 0)
                    goto segmentHitsWorldSolidOrEnvironmentSolid;
                if (node->isSolid == 0)
                {
                    startDistance = currentStart[0] * node->plane[0]
                        + currentStart[1] * node->plane[1]
                        + currentStart[2] * node->plane[2] + node->plane[3];
                    endDistance = currentEnd[0] * node->plane[0]
                        + currentEnd[1] * node->plane[1]
                        + currentEnd[2] * node->plane[2] + node->plane[3];
                    if (startDistance < 0.0f)
                    {
                        if (endDistance >= 0.0f)
                        {
                            fraction = startDistance / (startDistance - endDistance);
                            stackCursor[1].node = node;
                            stackCursor[1].savedPoint[0] = currentEnd[0];
                            stackCursor[1].savedPoint[1] = currentEnd[1];
                            stackCursor[1].savedPoint[2] = currentEnd[2];
                            stackCursor[1].state = 3;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * fraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * fraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * fraction + currentStart[2];
                            stackCursor++;
                        }
                        node = node->negativeChild;
                    }
                    else
                    {
                        if (endDistance < 0.0f)
                        {
                            fraction = startDistance / (startDistance - endDistance);
                            stackCursor[1].node = node;
                            stackCursor[1].savedPoint[0] = currentEnd[0];
                            stackCursor[1].savedPoint[1] = currentEnd[1];
                            stackCursor[1].savedPoint[2] = currentEnd[2];
                            stackCursor[1].state = 1;
                            currentEnd[0] = (currentEnd[0] - currentStart[0]) * fraction + currentStart[0];
                            currentEnd[1] = (currentEnd[1] - currentStart[1]) * fraction + currentStart[1];
                            currentEnd[2] = (currentEnd[2] - currentStart[2]) * fraction + currentStart[2];
                            stackCursor++;
                        }
                        node = node->positiveChild;
                    }
                }
                savedStart[0] = currentEnd[0];
                savedStart[1] = currentEnd[1];
                savedStart[2] = currentEnd[2];
                if (node == 0)
                    goto segmentHitsWorldSolidOrEnvironmentSolid;
            } while (node->isSolid == 0);

            environment = world->regions[node->isSolid - 1];
            result = environment->environmentFlags & environmentMask;
            if (result != 0)
            {
                hitPoint[0] = currentStart[0] - (originalEnd[0] - originalStart[0]) * S3D_RAY_SEGMENT_BACKOFF;
                hitPoint[1] = currentStart[1] - (originalEnd[1] - originalStart[1]) * S3D_RAY_SEGMENT_BACKOFF;
                hitPoint[2] = currentStart[2] - (originalEnd[2] - originalStart[2]) * S3D_RAY_SEGMENT_BACKOFF;
                return result;
            }

            for (;;)
            {
                if (stackCursor < world->clipSegmentStack)
                    goto segmentMissesWorldSolidOrEnvironment;
                if (stackCursor->state == 1)
                {
                    node = stackCursor->node->negativeChild;
                    currentEnd[0] = stackCursor->savedPoint[0];
                    currentEnd[1] = stackCursor->savedPoint[1];
                    currentEnd[2] = stackCursor->savedPoint[2];
                    stackCursor->savedPoint[0] = currentStart[0];
                    stackCursor->savedPoint[1] = currentStart[1];
                    stackCursor->savedPoint[2] = currentStart[2];
                    stackCursor->state = 4;
                    goto resumeWorldSolidOrEnvironmentTraversal;
                }
                if (stackCursor->state == 3)
                    break;
                currentStart[0] = stackCursor->savedPoint[0];
                currentStart[1] = stackCursor->savedPoint[1];
                currentStart[2] = stackCursor->savedPoint[2];
                stackCursor--;
            }
            node = stackCursor->node->positiveChild;
            currentEnd[0] = stackCursor->savedPoint[0];
            currentEnd[1] = stackCursor->savedPoint[1];
            currentEnd[2] = stackCursor->savedPoint[2];
            stackCursor->savedPoint[0] = currentStart[0];
            stackCursor->savedPoint[1] = currentStart[1];
            stackCursor->savedPoint[2] = currentStart[2];
            stackCursor->state = 2;
resumeWorldSolidOrEnvironmentTraversal:
            currentStart[0] = savedStart[0];
            currentStart[1] = savedStart[1];
            currentStart[2] = savedStart[2];
        } while (world->clipSegmentStack <= stackCursor);

segmentMissesWorldSolidOrEnvironment:
        hitPoint[1] = originalEnd[1];
        hitPoint[0] = originalEnd[0];
        hitPoint[2] = originalEnd[2];
        result = 0;
    }
    else
    {
        result = 0xffffffff;
    }
    return result;

segmentHitsWorldSolidOrEnvironmentSolid:
    hitPoint[0] = currentStart[0] - (originalEnd[0] - originalStart[0]) * S3D_RAY_SEGMENT_BACKOFF;
    hitPoint[1] = currentStart[1] - (originalEnd[1] - originalStart[1]) * S3D_RAY_SEGMENT_BACKOFF;
    hitPoint[2] = currentStart[2] - (originalEnd[2] - originalStart[2]) * S3D_RAY_SEGMENT_BACKOFF;
    return 1;
}

// FUNCTION: EQGFX 0x10020094
// Builds the world's list of regions potentially visible from one region, cached until the region changes.
// @early-stop: the rewrite uses a 0x10-byte frame and different decoder register allocation; 202 instructions still differ.
// __cdecl (two stack arguments, the caller pops them). Returns 0, or 0xffff3d3e when the visible list can't be allocated.
// Decodes the region's PVS stream into world->visibleList and world->visibleCount; world->visibleRegion caches the region.
/* fake match: ordered tag cases preserve the original decoder branch layout. */
int __cdecl s3dDecodePVSForRegion(t3dWorldAmbient_guess *world, u32 region)
{
    int result;
    int remaining;
    int visibleCount;
    u32 position;
    u32 *visibleCursor;
    u32 *allocated;
    u8 *code;
    T3DPVS_guess *pvs;
    T3DPVSData_guess *data;
    int encoding;

    position = 0;
    result = 0;
    visibleCount = 0;
    if (world->visibleList == 0)
    {
        allocated = EQ_CDECL(u32 *, 0x10050C48, (u32))(world->regionCount << 2);
    }
    else
    {
        if (world->visibleRegion == region)
        {
            /* fake match: retain the shared cached-region return. */
            goto done;
        }
        if (world->visibleCapacity < world->regionCount)
            allocated = EQ_CDECL(u32 *, 0x10050C8C, (void *, u32))(world->visibleList, world->regionCount << 2);
        else
        {
            /* fake match: continue at the shared decode block. */
            goto decode;
        }
    }
    world->visibleList = allocated;
    if (allocated == 0)
    {
        result = 0xffff3d3e;
        /* fake match: share the function's result exit. */
        goto done;
    }
    world->visibleCapacity = world->regionCount;

decode:
    pvs = world->regions[region]->pvs;
    data = pvs->data;
    encoding = pvs->encoding;
    remaining = data->count;
    code = (u8 *)data->codes;
    visibleCursor = world->visibleList;
    if (encoding == 1)
    {
        u16 codeWord;
        u32 wordValue;
        u32 countBits;
        u32 tagBits;
        u32 skip;

        if (remaining != 0)
        {
            do
            {
                codeWord = *(u16 *)code;
                wordValue = codeWord;
                countBits = wordValue & 0x3fff;
                tagBits = wordValue & 0xc000;
                skip = countBits;
                switch (tagBits)
                {
                case 0xc000:
                    visibleCount = visibleCount + countBits;
                    if (countBits != 0)
                    {
                        do
                        {
                            *visibleCursor = position;
                            position = position + 1;
                            visibleCursor = visibleCursor + 1;
                            countBits = countBits - 1;
                        } while (countBits != 0);
                    }
                    break;
                case 0x8000:
                    skip = wordValue & 0x7f;
                    countBits = countBits >> 7;
                    visibleCount = visibleCount + countBits;
                    for (; countBits != 0; countBits = countBits - 1)
                    {
                        *visibleCursor = position;
                        position = position + 1;
                        visibleCursor = visibleCursor + 1;
                    }
                    /* fall through */
                case 0:
                    position = position + skip;
                    break;
                case 0x4000:
                    wordValue = wordValue & 0x7f;
                    visibleCount = visibleCount + wordValue;
                    position = position + (countBits >> 7);
                    if (wordValue != 0)
                    {
                        do
                        {
                            *visibleCursor = position;
                            position = position + 1;
                            visibleCursor = visibleCursor + 1;
                            wordValue = wordValue - 1;
                        } while (wordValue != 0);
                    }
                    break;
                }
                code = code + 2;
                remaining = remaining - 1;
            } while (remaining != 0);
        }
    }
    else if (encoding == 2)
    {
        u32 countBits;
        u32 tagBits;
        u32 skip;
        u32 byteValue;
        u32 skipCount;

        while (remaining != 0)
        {
            byteValue = *code;
            if (byteValue == 0x3f)
            {
                skipCount = code[1];
                code = code + 1;
                remaining = remaining - 3;
                code = code + 1;
                skipCount = skipCount + ((u32)*code << 8);
                code = code + 1;
                position = position + skipCount;
            }
            else if (byteValue == 0xff)
            {
                countBits = code[1];
                countBits = countBits + ((u32)code[2] << 8);
                remaining = remaining - 3;
                code = code + 3;
                visibleCount = visibleCount + countBits;
                for (; countBits != 0; countBits = countBits - 1)
                {
                    *visibleCursor = position;
                    position = position + 1;
                    visibleCursor = visibleCursor + 1;
                }
            }
            else
            {
                skip = byteValue;
                tagBits = byteValue & 0xc0;
                countBits = byteValue & 0x3f;
                switch (tagBits)
                {
                case 0xc0:
                    visibleCount = visibleCount + countBits;
                    if (countBits != 0)
                    {
                        do
                        {
                            *visibleCursor = position;
                            position = position + 1;
                            visibleCursor = visibleCursor + 1;
                            countBits = countBits - 1;
                        } while (countBits != 0);
                    }
                    break;
                case 0x80:
                    countBits = countBits >> 3;
                    visibleCount = visibleCount + countBits;
                    for (; countBits != 0; countBits = countBits - 1)
                    {
                        *visibleCursor = position;
                        position = position + 1;
                        visibleCursor = visibleCursor + 1;
                    }
                    position = position + (skip & 7);
                    break;
                case 0x40:
                    skip = skip & 7;
                    visibleCount = visibleCount + skip;
                    position = position + (countBits >> 3);
                    if (skip != 0)
                    {
                        do
                        {
                            *visibleCursor = position;
                            position = position + 1;
                            visibleCursor = visibleCursor + 1;
                            skip = skip - 1;
                        } while (skip != 0);
                    }
                    break;
                case 0:
                    position = position + countBits;
                    break;
                }
                code = code + 1;
                remaining = remaining - 1;
            }
        }
    }
    else
    {
        u32 wordValue;

        visibleCount = remaining + 1;
        for (; remaining != 0;)
        {
            wordValue = *(u16 *)code;
            if (wordValue >= region)
                break;
            code = code + 1;
            *visibleCursor = wordValue;
            code = code + 1;
            visibleCursor = visibleCursor + 1;
            remaining = remaining - 1;
        }
        *visibleCursor = region;
        visibleCursor = visibleCursor + 1;
        for (; remaining != 0; remaining = remaining - 1)
        {
            wordValue = *(u16 *)code;
            code = code + 1;
            *visibleCursor = wordValue;
            code = code + 1;
            visibleCursor = visibleCursor + 1;
        }
    }
    world->visibleCount = visibleCount;
    world->visibleRegion = region;
done:
    return result;
}

// FUNCTION: EQGFX 0x100202e2
// Sets the world's constant ambient lighting color.
// __cdecl (two stack arguments, callee pops nothing), void (the callers never read EAX). Stores the packed rgb
// `ambient` points at into the world's constant-ambient slot, next to s3dSetWorldSpecialAmbient.
void __cdecl s3dSetWorldConstantAmbient(t3dWorldAmbient_guess *world, u32 *ambient)
{
    world->constant_ambient = *ambient;
}

// FUNCTION: EQGFX 0x100202f3
// Sets the world's special ambient lighting color.
// __cdecl (two stack arguments, callee pops nothing), void. Stores the packed rgb
// `ambient` points at into the world's special-ambient slot, next to s3dSetWorldConstantAmbient.
void __cdecl s3dSetWorldSpecialAmbient(t3dWorldAmbient_guess *world, u32 *ambient)
{
    world->special_ambient = *ambient;
}

// FUNCTION: EQGFX 0x10020304
// Collects visible world actors within a camera view or a distance around a viewpoint.
u32 __cdecl s3dGetWorldVisibleActorList(t3dWorldAmbient_guess *world, T3DCamera_guess *camera, float *viewPosition, float maxDistance, u32 visibilityLimit, T3DReferenceList_guess *referenceList)
{
    float frustum[54];
    u32 *visibleRegion;
    u32 *visibleRegionEnd;
    T3DActorInstance_guess **actorInstances;
    T3DRegion_guess *region;
    T3DRegionActorList_guess *regionActorList;
    u32 *actorIndex;
    u32 *actorIndexEnd;
    T3DActorInstance_guess *actor;
    u32 visitStamp;
    float visibilityDepth;
    float dx;
    float dy;
    float dz;
    float radius;
    int result;

    if (camera == 0)
    {
        if (viewPosition == 0)
            goto done;
        visibilityDepth = viewPosition[6];
    }
    else
    {
        result = EQ_CDECL(int, 0x1002F68D, (T3DCamera_guess *))(camera);
        if (result < 0)
            goto done;
        EQ_CDECL(void, 0x1002F9A0, (T3DCamera_guess *, float *))(camera, frustum);
        visibilityDepth = camera->visibilityDepth;
    }
    EQGFX_WORLD_VISIBILITY_STAMP = EQGFX_WORLD_VISIBILITY_STAMP + 1;
    visitStamp = EQGFX_WORLD_VISIBILITY_STAMP;
    EQ_CDECL(void, 0x1001905A, (T3DReferenceList_guess *))(referenceList);
    result = EQ_CDECL(int, 0x10020094, (t3dWorldAmbient_guess *, float))(world, visibilityDepth);
    if (result != 0)
        return 0;
    visibleRegion = world->visibleList;
    visibleRegionEnd = visibleRegion + world->visibleCount;
    actorInstances = world->actorInstances;
    if (camera == 0)
    {
        if (visibleRegion < visibleRegionEnd)
        {
            do
            {
                region = world->regions[*visibleRegion];
                dx = region->centerX - viewPosition[0];
                dy = region->centerY - viewPosition[1];
                dz = region->centerZ - viewPosition[2];
                radius = maxDistance + region->boundsRadius;
                if (dz * dz + dx * dx + dy * dy < radius * radius)
                {
                    regionActorList = region->actorList;
                    actorIndex = regionActorList->actorIndices;
                    actorIndexEnd = actorIndex + regionActorList->actorCount;
                    for (; actorIndex < actorIndexEnd; actorIndex = actorIndex + 1)
                    {
                        actor = actorInstances[*actorIndex];
                        if (actor->lastVisibilityStamp != visitStamp)
                        {
                            actor->lastVisibilityStamp = visitStamp;
                            if (visibilityLimit < actor->visibilityLimit)
                            {
                                dx = actor->x - viewPosition[0];
                                dy = actor->y - viewPosition[1];
                                dz = actor->z - viewPosition[2];
                                radius = actor->boundsHeight * actor->boundsRadius + maxDistance;
                                if (dz * dz + dx * dx + dy * dy < radius * radius)
                                {
                                    result = EQ_CDECL(int, 0x10019065, (T3DActorInstance_guess *, T3DReferenceList_guess *))(actor, referenceList);
                                    if (result < 0)
                                        goto done;
                                }
                            }
                        }
                    }
                }
                visibleRegion = visibleRegion + 1;
            } while (visibleRegion < visibleRegionEnd);
        }
    }
    else if (visibleRegion < visibleRegionEnd)
    {
        do
        {
            region = world->regions[*visibleRegion];
            result = EQ_CDECL(int, 0x10030DAA, (float *, float *, float))(frustum, &region->centerX, region->boundsRadius);
            if (result != T3D_FRUSTUM_OUTSIDE)
            {
                regionActorList = region->actorList;
                actorIndex = regionActorList->actorIndices;
                actorIndexEnd = actorIndex + regionActorList->actorCount;
                for (; actorIndex < actorIndexEnd; actorIndex = actorIndex + 1)
                {
                    actor = actorInstances[*actorIndex];
                    if (actor->lastVisibilityStamp != visitStamp)
                    {
                        actor->lastVisibilityStamp = visitStamp;
                        if (visibilityLimit < actor->visibilityLimit)
                        {
                            result = EQ_CDECL(int, 0x10030DAA, (float *, float *, float))(frustum, &actor->x, actor->boundsHeight * actor->boundsRadius);
                            if (result != T3D_FRUSTUM_OUTSIDE)
                            {
                                result = EQ_CDECL(int, 0x10019065, (T3DActorInstance_guess *, T3DReferenceList_guess *))(actor, referenceList);
                                if (result < 0)
                                    goto done;
                            }
                        }
                    }
                }
            }
            visibleRegion = visibleRegion + 1;
        } while (visibleRegion < visibleRegionEnd);
    }

done:
    return referenceList->count;
}

// FUNCTION: EQGFX 0x10020549
// __cdecl with one stack argument: zone block holding the kind at +0x00.
u32 __cdecl t3dIsZoneValid(T3dZone_guess *zone)
{
    u32 result;

    result = 0;
    if (zone->kind != 0x44)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x1002055A
// Creates a zone object for the renderer.
T3dZone_guess *__cdecl t3dCreateZone(void *context)
{
    T3dZone_guess *zone;

    zone = (T3dZone_guess *)EQ_CDECL(void *, 0x10050C48, (size_t))(T3D_ZONE_ALLOCATION_SIZE);
    if (zone != 0)
    {
        zone->unknown_08 &= 0;
        zone->unknown_0c &= 0;
        zone->kind = T3D_ZONE_KIND;
        if (EQ_CDECL(int, 0x10019012, (void *, int))(zone->referenceList, T3D_ZONE_REFERENCE_CAPACITY) == 0)
        {
            if (EQ_CDECL(int, 0x1001564B, (void *, T3dZone_guess *))(context, zone) != 0)
            {
                EQ_CDECL(void, 0x100191BD, (void *))(zone->referenceList);
                EQ_CDECL(void, 0x10050CBB, (void *))(zone);
                zone = 0;
            }
        }
    }
    return zone;
}
