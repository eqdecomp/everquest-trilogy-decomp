// unit_1001B80C.c - eqgfx_dx7.dll, the address run 0x1001B80C..0x1001D1D3 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include "trilogy/eqaddr.h"
#include "eqgfx/layout/t3dworld.h"
#include <string.h>

#ifdef EQD_MATCH
extern void __cdecl eqd_fn_0x1001B80C(void);
extern void __cdecl eqd_fn_0x1001C9B4(void);
extern void __cdecl eqd_fn_0x1001CC18(void);
extern void __cdecl eqd_fn_0x1001CFD0(void);
extern void __cdecl eqd_fn_0x1001D009(void);
#endif

typedef struct TrackPosition_guess
{
    unsigned long x;
    unsigned long y;
    unsigned long z;
} TrackPosition_guess;

typedef struct TrackTransformation_guess
{
    float quaternion[4];
    TrackPosition_guess position;
    unsigned long scale;
} TrackTransformation_guess;

/* T3D_tagWORLD is supplied by the canonical world layout header. */

typedef struct EqGfxCleanupObject_guess EqGfxCleanupObject_guess;

struct EqGfxCleanupObject_guess
{
    unsigned char unknown_00[0x0C];
    unsigned char flags;
    unsigned char unknown_0D[3];
    unsigned long use_count;
    EqGfxCleanupObject_guess *link_14;
    EqGfxCleanupObject_guess *link_18;
    EqGfxCleanupObject_guess *link_1C;
    EqGfxCleanupObject_guess *link_20;
    EqGfxCleanupObject_guess *link_24;
    EqGfxCleanupObject_guess *link_28;
    EqGfxCleanupObject_guess *link_2C;
    EqGfxCleanupObject_guess *link_30;
    void *unknown_34;
    EqGfxCleanupObject_guess *link_38;
    unsigned char unknown_3C[0x20];
    EqGfxCleanupObject_guess *link_5C;
    void *unknown_60;
    EqGfxCleanupObject_guess *link_64;
    void *unknown_68;
    EqGfxCleanupObject_guess *link_6C;
    unsigned char unknown_70[0x18];
    EqGfxCleanupObject_guess *link_88;
    unsigned char unknown_8C[0x2C];
};

#define EQGFX_CLEANUP_FLAG_KEEP 0x10
#define EQGFX_CLEANUP_WORLD_TYPE 0x19
#define EQGFX_CLEANUP_BMINFO_TYPE 0x1B
#define EQGFX_CLEANUP_DIRECTIONAL_LIGHT_TYPE 0x1F
#define EQGFX_CLEANUP_TYPE_OUT_OF_RANGE 0xFFFF3D3E

// 0x10076284 the zero float the original's FP compares use (eqgfx_dx7.dll's .rdata); read through the
// address, like the original's FCOMP memory operand.
#define EQGFX_ZERO_F_GUESS          EQ_GLOBAL(float, 0x10076284)
// 0x10076280 the one float the original's FP compare uses, read through the address like its FCOMP operand.
#define EQGFX_FLOAT_ONE                 EQ_GLOBAL(float, 0x10076280)
#define EQGFX_FRAME_QUATERNION_SCALE EQ_GLOBAL(float, 0x100762A0)
#define EQGFX_FRAME_POSITION_SCALE   EQ_GLOBAL(float, 0x1007629C)
#define EQGFX_T3D_EULER_TO_QUATERNION 0x1004F89A
#define EQGFX_X87_FLOAT_TO_WORD       0x1004EC36
#define EQGFX_QUATERNION_WORD_SCALE_ADDRESS EQ_GLOBAL(float, 0x100762AC)
#define EQGFX_POSITION_WORD_SCALE_ADDRESS   0x100762A8
#define TRACK_FRAME_OUT_OF_RANGE     0xFFFF3D3E

// The track object's direction float at +0x2C, the sign `t3dSetTrackDefaultReverse` drives (the track layout
// beyond the `definition` at +0x14 is not in include/; only +0x2C is read here).
#define TRACK_REVERSE_OFFSET_GUESS  0x2C

// The simple sprite's flags byte at +0x0C: 0x01 = registered in the dictionary under dict_key; 0x20 = keep the
// memory (guess: the sprite's block is owned by the caller and not freed here).
#define SIMPLE_SPRITE_FLAG_IN_DICT      0x01
#define SIMPLE_SPRITE_FLAG_KEEP_MEMORY  0x20
// 0x08 = the animation is suspended (guess from the name of t3dSuspendSimpleSpriteAnimation).
#define SIMPLE_SPRITE_FLAG_SUSPENDED    0x08

// The object at sprite +0x28 (its type is not known); its use count at +0x10 is read by t3dDestroySimpleSprite,
// its frame count and frame list at +0x14 and +0x18 by t3dSetSimpleSpriteCurrentFrame.
typedef struct SpriteOwner_guess
{
    unsigned long tag;              // 0x00: 5 for a valid owner
    unsigned char unknown_04[0xC];  // 0x04
    unsigned long use_count;        // 0x10
    unsigned long frame_count;      // 0x14
    void **frames;                  // 0x18: frame_count entries
} SpriteOwner_guess;

// The simple sprite (0x2c bytes, see s3dCalcSimpleSpriteAllocSize). Only the fields t3dDestroySimpleSprite and
// t3dSetSimpleSpriteCurrentFrame read.
typedef struct SimpleSprite_guess
{
    unsigned long tag;              // 0x00: 6 for a valid sprite
    unsigned char unknown_04[0x4];  // 0x04
    unsigned long dict_key;         // 0x08: the dictionary key, removed while the 0x01 flag is set
    unsigned char flags;            // 0x0C
    unsigned char unknown_0d[0x3];  // 0x0D
    unsigned long use_count;        // 0x10: must be 0 to destroy the sprite
    void *current_frame;            // 0x14
    unsigned long frame_index;      // 0x18
    unsigned long unknown_1c;       // 0x1C: copied with the sprite, the duplicate's +0x20 starts from it
    unsigned long anim_flags;       // 0x20: the bits a resume ORs in (t3dSuspendSimpleSpriteAnimation, guess)
    unsigned char unknown_24[0x4];  // 0x24
    SpriteOwner_guess *owner;       // 0x28: the object whose use count the sprite drops
} SimpleSprite_guess;

// The context a sprite is duplicated into (the first argument, passed on to _t3dAddItemToList_guess). Only +0x08 is read here.
typedef struct SpriteContext_guess
{
    unsigned char unknown_00[0x8];  // 0x00
    unsigned long base_08;          // 0x08: the duplicate's +0x20 is this plus the source's +0x1c (guess)
} SpriteContext_guess;

// The track definition's instance list; the frame count sits at +0x0, its default instance at +0x8.
// No include/ struct models either type, the fields are the ones the asm reads.
typedef struct TrackInstanceList_guess
{
    int num_frames;                 // 0x00
    void *block;                    // 0x04: freed with the list while the definition owns it
    void *default_instance;         // 0x08
} TrackInstanceList_guess;

typedef struct TrackPackedFrame_guess
{
    short quaternion[4];            // 0x00
    short position[3];              // 0x08
    unsigned short scale;           // 0x0E
} TrackPackedFrame_guess;

typedef struct TrackInstanceFrame_guess
{
    float quaternion[4];            // 0x00
    float position[3];              // 0x10
    float scale;                    // 0x1C
    unsigned long unknown_20;       // 0x20: included in the 0x24-byte stride
} TrackInstanceFrame_guess;

// The track definition (named and registered like a sprite): its instance list at +0x14, the fallback at +0x18.
typedef struct TrackDefinition_guess
{
    unsigned long tag;                      // 0x00: 0x15 for a valid definition
    unsigned char unknown_04[0x4];          // 0x04
    void *dict_key;                         // 0x08: the dictionary key, also freed while the list is owned
    unsigned char flags;                    // 0x0C: 0x01 = in the dictionary under dict_key, 0x20 = keep the memory
    unsigned char unknown_0d[0x3];          // 0x0D
    unsigned long use_count;                // 0x10: dropped by one for each track destroyed
    TrackInstanceList_guess *instance_list; // 0x14: the list, while it is not null
    TrackInstanceList_guess *fallback_list; // 0x18: the list used while +0x14 is null
    SimpleSprite_guess *template_sprite;    // 0x1C: the 0x2c-byte sprite t3dCreateSimpleSprite copies into each new one
} TrackDefinition_guess;

typedef struct TrackDefinition2_guess
{
    unsigned long tag;                              // 0x00: 0x15 for a valid definition
    unsigned char unknown_04[0x4];                  // 0x04
    void *dict_key;                                 // 0x08
    unsigned long flags;                            // 0x0C
    unsigned long use_count;                        // 0x10
    TrackInstanceList_guess *instance_list;         // 0x14
    TrackInstanceList_guess *fallback_list;         // 0x18
    TrackInstanceList_guess inline_fallback_list;   // 0x1C
} TrackDefinition2_guess;

typedef struct Track_guess
{
    unsigned char unknown_00[0x8];     // 0x00
    void *dict_key;                    // 0x08: the dictionary key, freed with the track unless flag 0x20
    unsigned char flags;               // 0x0C: 0x01 = in the dictionary under dict_key, 0x20 = keep the memory
    unsigned char unknown_0d[0x3];     // 0x0D
    unsigned long use_count;           // 0x10: must be 0 to destroy the track
    TrackDefinition_guess *definition; // 0x14
    short default_frame;               // 0x18: the default frame, set by t3dSetTrackDefaultFromFrame
    short default_to_frame;            // 0x1A: the default frame, set by t3dSetTrackDefaultToFrame
    float default_interpolation_value; // 0x1C: set by t3dSetTrackDefaultfInterpolationValue, in [0, 1]
    unsigned long unknown_20;          // 0x20: copied with the track; the duplicate's +0x24 starts from it
    unsigned long unknown_24;          // 0x24: the duplicate's, = +0x20 + the context's base_08 (t3dDuplicateTrack)
    unsigned char unknown_28[0x4];     // 0x28
    float default_reverse;
    unsigned char unknown_30[0x4];
    unsigned long default_interpolate; // 0x34: bit 0 = the track's default interpolate flag
} Track_guess;

typedef struct TrackInstance_guess
{
    unsigned long tag;                      // 0x00: 0x16 for a valid track instance
    unsigned char unknown_04[0x4];          // 0x04
    void *dict_key;                         // 0x08
    unsigned long flags;                    // 0x0C
    unsigned long use_count;                // 0x10
    TrackDefinition2_guess *definition;     // 0x14
    short default_frame;                    // 0x18
    short default_to_frame;                 // 0x1A
    float default_interpolation_value;      // 0x1C
    unsigned long unknown_20;               // 0x20
    unsigned long unknown_24;               // 0x24
    unsigned long unknown_28;               // 0x28
    float default_reverse;                  // 0x2C
    unsigned long unknown_30;               // 0x30
    unsigned long default_interpolate;      // 0x34
} TrackInstance_guess;

// The actor object (its layout before +0x1C is not in include/; only the heading float at +0x1C is read here).
/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

// The frame record a track's default is read from. Only the words at +0x18 and +0x1A are read here.
typedef struct TrackFrame_guess
{
    unsigned char unknown_00[0x18];   // 0x00
    unsigned short default_value;     // 0x18
    unsigned short default_to_frame;  // 0x1A (guess: the word t3dGetTrackDefaultToFrame reads)
} TrackFrame_guess;

#define TRACK_FLAG_IN_DICT      0x01
#define TRACK_FLAG_KEEP_MEMORY  0x20

// eqgfx_dx7.dll functions that t3dUndefineTrack and t3dDestroyTrack call (Ghidra names; FUN_ where none is known).
#define EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY  0x1000E334
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT                      0x10015D81
#define EQGFX_T3D_FREE_MEMORY                   0x10050CBB
#define EQGFX_T3D_ALLOCATE_MEMORY               0x10050C48
#define EQGFX__T3D_ADD_ITEM_TO_LIST                      0x1001564B
#define EQGFX_S3D_CALC_SIMPLE_SPRITE_ALLOC_SIZE 0x1001B80C
#define EQGFX_S3D_CALC_TRACK_DEF2_ALLOC_SIZE    0x1001C9B4
#define EQGFX_FUN_1001CFD0                      0x1001CFD0
#define EQGFX_FUN_1001D009                      0x1001D009
#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY    0x1000E7FE
#define EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY       0x1000DC08
#define EQGFX_COPY_STRING_WITH_NULL_TERMINATOR  0x10068070
#define EQGFX_STRLEN                            0x10068160
#define EQGFX_MEMCPY                            0x10067D30
#define TRACK_DEFINITION_TYPE                   0x15
#define TRACK_INSTANCE_TYPE                     0x16

// kernel32 OutputDebugStringA through eqgfx_dx7.dll's import slot (the original's CALL dword ptr [0x100760c4]).
typedef void (__stdcall *OutputDebugStringA_guess_t)(const char *);
#define EQGFX_OUTPUT_DEBUG_STRING_A  EQ_GLOBAL(OutputDebugStringA_guess_t, 0x100760C4)

// FUNCTION: EQGFX 0x1001b80c
// Determine storage needed for a simple sprite and its optional name.
// __cdecl (the callee pops nothing, ECX is not an input). Bytes a simple sprite allocation needs: the fixed
// 0x2c-byte header alone, or 0x2d + strlen(name) once `name` is given (its terminator included).
int __cdecl s3dCalcSimpleSpriteAllocSize(char *name)
{
    int size;

    size = 0x2c;
    if (name != NULL)
        size = strlen(name) + 0x2d;
    return size;
}

// FUNCTION: EQGFX 0x1001B824
// Creates a simple sprite instance with its definition's initial animation settings.
// __cdecl. Copies the definition's template (+0x1c) into preallocated or a new block, names it, links it via _t3dAddItemToList_guess; NULL on failure, else counts a use of the definition.
SimpleSprite_guess *__cdecl t3dCreateSimpleSprite(SpriteContext_guess *context, SimpleSprite_guess *preallocated, char *name, TrackDefinition_guess *definition)
{
    SimpleSprite_guess *sprite;
    int size;
    int result;

    sprite = preallocated;
    if (sprite == NULL)
    {
        size = EQ_CDECL(int, EQGFX_S3D_CALC_SIMPLE_SPRITE_ALLOC_SIZE, (char *))(name);
        sprite = EQ_CDECL(SimpleSprite_guess *, EQGFX_T3D_ALLOCATE_MEMORY, (unsigned long))(size);
        if (sprite == NULL)
            return NULL;
    }
    *sprite = *definition->template_sprite;
    sprite->dict_key = 0;
    *(unsigned long *)&sprite->flags = 0;
    sprite->use_count = 0;
    sprite->anim_flags = sprite->unknown_1c + context->base_08;
    if (preallocated != NULL)
        *(unsigned long *)&sprite->flags |= SIMPLE_SPRITE_FLAG_KEEP_MEMORY;
    if (name != NULL)
    {
        sprite->dict_key = (unsigned long)(sprite + 1);
        EQ_CDECL(size_t, EQGFX_STRLEN, (char *))(name);
        EQ_CDECL(void, EQGFX_COPY_STRING_WITH_NULL_TERMINATOR, (char *, char *))((char *)sprite->dict_key, name);
    }
    else
    {
        sprite->dict_key = 0;
    }
    result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (SpriteContext_guess *, SimpleSprite_guess *))(context, sprite);
    if (result != 0)
    {
        EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(sprite);
        sprite = NULL;
    }
    else
    {
        if (name != NULL && EQ_CDECL(int, EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY, (SpriteContext_guess *, SimpleSprite_guess *))(context, sprite) < 0)
        {
            EQ_CDECL(int, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (SpriteContext_guess *, SimpleSprite_guess *))(context, sprite);
            if (sprite != NULL && preallocated == NULL)
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(sprite);
            return NULL;
        }
        definition->use_count++;
    }
    return sprite;
}

// FUNCTION: EQGFX 0x1001b8f2
// Make a separate simple sprite instance from an existing sprite.
// __cdecl. Copies the sprite into a new block, clears its key, flags and use count, links it via _t3dAddItemToList_guess; NULL on failure.
SimpleSprite_guess *__cdecl t3dDuplicateSimpleSprite(SpriteContext_guess *context, SimpleSprite_guess *source)
{
    SimpleSprite_guess *duplicate;
    int result;

    duplicate = (SimpleSprite_guess *)EQ_CDECL(void *, EQGFX_T3D_ALLOCATE_MEMORY, (unsigned long))(sizeof(SimpleSprite_guess));
    if (duplicate != NULL)
    {
        *duplicate = *source;
        duplicate->dict_key = 0;
        *(unsigned long *)&duplicate->flags = 0;
        duplicate->use_count = 0;
        duplicate->anim_flags = duplicate->unknown_1c + context->base_08;
        result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (SpriteContext_guess *, SimpleSprite_guess *))(context, duplicate);
        if (result != 0)
        {
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (SimpleSprite_guess *))(duplicate);
            duplicate = NULL;
        }
        else
        {
            duplicate->owner->use_count++;
        }
    }
    return duplicate;
}

// FUNCTION: EQGFX 0x1001b94a
// Release a simple sprite instance from its rendering owner.
// __cdecl (the callee pops nothing, ECX is not an input). Returns 0xffff3d45 while the sprite's use count is not 0.
// Else: drops the owner's use count, unlinks the dictionary entry when flagged, calls 0x10015D81, frees unless flagged.
int __cdecl t3dDestroySimpleSprite(void *context, SimpleSprite_guess *sprite)
{
    int result;

    if (sprite->use_count > 0)
    {
        result = 0xffff3d45;
    }
    else
    {
        sprite->owner->use_count--;
        if ((sprite->flags & SIMPLE_SPRITE_FLAG_IN_DICT) != 0)
            EQ_CDECL(void, 0x1000E334, (void *, unsigned long))(context, sprite->dict_key);
        result = EQ_CDECL(int, 0x10015D81, (void *, SimpleSprite_guess *))(context, sprite);
        if ((sprite->flags & SIMPLE_SPRITE_FLAG_KEEP_MEMORY) == 0)
            EQ_CDECL(void, 0x10050CBB, (SimpleSprite_guess *))(sprite);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001B997
// Check whether a simple sprite and its owner are valid.
// __cdecl (the callee pops nothing, ECX is not an input). Returns 1 when `sprite`, its owner and both tags are valid, else 0.
int __cdecl t3dIsSimpleSpriteValid(SimpleSprite_guess *sprite)
{
    int result;

    result = 1;
    if (sprite == 0 || sprite->owner == 0 || sprite->owner->tag != 5 || sprite->tag != 6)
        result = 0;
    return result;
}

// FUNCTION: EQGFX 0x1001b9b6
// Select the displayed frame for a simple sprite.
// __cdecl (the callee pops nothing, ECX is not an input). Returns 0xffff3d3e unless `frame` is below the owner's frame
// count; else makes `frame` the current frame index, its frame the current frame, and returns 0.
int __cdecl t3dSetSimpleSpriteCurrentFrame(SimpleSprite_guess *sprite, unsigned long frame)
{
    if (frame >= sprite->owner->frame_count)
        return 0xffff3d3e;
    sprite->frame_index = frame;
    sprite->current_frame = sprite->owner->frames[frame];
    return 0;
}

// FUNCTION: EQGFX 0x1001b9de
// __cdecl. Returns 0. Dword RMW at +0x0C: `suspend` != 0 sets bit 0x08, 0 clears it and ORs `resume_flags` into +0x20.
int __cdecl t3dSuspendSimpleSpriteAnimation(SimpleSprite_guess *sprite, int suspend, unsigned long resume_flags)
{
    if (suspend == 0)
        *(unsigned long *)&sprite->flags &= ~SIMPLE_SPRITE_FLAG_SUSPENDED;
    else
        *(unsigned long *)&sprite->flags |= SIMPLE_SPRITE_FLAG_SUSPENDED;
    if (suspend == 0)
        sprite->anim_flags |= resume_flags;
    return 0;
}

// FUNCTION: EQGFX 0x1001ba04
// Flushes one resource type from a world: frees each live unkept entry, except definitions' default instances
unsigned long __cdecl s3dFlushWorldResourcesOfType_guess(T3D_tagWORLD *world, int resource_type)
{
    S3dResourcePool_guess *selected_table;
    EqGfxCleanupObject_guess *object;
    EqGfxCleanupObject_guess *nested_object;
    unsigned int *first_item;
    unsigned int *current_item;
    unsigned long item_count;
    unsigned long item_index;
    unsigned long object_id;
    unsigned long deformation_count;
    unsigned long default_instance;
    unsigned long result;

    result = 0;
    if (resource_type < 0x24)
    {
        if (resource_type == 0x23)
            selected_table = &world->pools[41];
        else
        {
            switch (resource_type)
            {
            case 1: selected_table = &world->pools[0]; break;
            case 2: selected_table = &world->pools[10]; break;
            case 3: selected_table = &world->type3Pool; break;
            case 4: selected_table = &world->pools[9]; break;
            case 5: selected_table = &world->pools[1]; break;
            case 6: selected_table = &world->pools[11]; break;
            case 7: selected_table = &world->pools[2]; break;
            case 8: selected_table = &world->pools[12]; break;
            case 9: selected_table = &world->pools[3]; break;
            case 10: selected_table = &world->pools[13]; break;
            case 0x0B: selected_table = &world->pools[4]; break;
            case 0x0C: selected_table = &world->pools[14]; break;
            case 0x11: selected_table = &world->pools[5]; break;
            case 0x12: selected_table = &world->pools[15]; break;
            case 0x13: selected_table = &world->pools[6]; break;
            case 0x14: selected_table = &world->pools[16]; break;
            case 0x15: selected_table = &world->pools[27]; break;
            case 0x16: selected_table = &world->pools[28]; break;
            case 0x17: selected_table = &world->pools[7]; break;
            case 0x18: selected_table = &world->pools[17]; break;
            case EQGFX_CLEANUP_WORLD_TYPE: selected_table = &world->regionPool; break;
            case 0x1A: selected_table = &world->pools[18]; break;
            case 0x1B: selected_table = &world->pools[19]; break;
            case 0x1C: selected_table = &world->pools[20]; break;
            case 0x1D: selected_table = &world->pools[21]; break;
            case 0x1E: selected_table = &world->pools[22]; break;
            case 0x1F: selected_table = &world->pools[23]; break;
            default: return EQGFX_CLEANUP_TYPE_OUT_OF_RANGE;
            }
        }
    }
    else
    {
        switch (resource_type)
        {
        case 0x44: selected_table = &world->pools[30]; break;
        case 0x45: selected_table = &world->pools[31]; break;
        case 0x46: selected_table = &world->pools[32]; break;
        case 0x47: selected_table = &world->pools[33]; break;
        case 0x48: selected_table = &world->pools[34]; break;
        case 0x49: selected_table = &world->pools[35]; break;
        case 0x4A: selected_table = &world->pools[36]; break;
        case 0x4C: selected_table = &world->pools[8]; break;
        case 0x4D: selected_table = &world->pools[29]; break;
        case 0x4E: selected_table = &world->pools[37]; break;
        case 0x4F: selected_table = &world->pools[38]; break;
        case 0x50: selected_table = &world->pools[39]; break;
        case 0x51: selected_table = &world->pools[40]; break;
        case 0x53: selected_table = &world->type53Pool; break;
        default: return EQGFX_CLEANUP_TYPE_OUT_OF_RANGE;
        }
    }

    item_count = selected_table->capacity;
    first_item = selected_table->freeEntries;
    current_item = first_item;
    item_index = 0;
    if (item_count != 0)
    {
        do
        {
            object_id = *current_item;
            object = (EqGfxCleanupObject_guess *)object_id;
            if (item_count < object_id)
            {
                if ((object->flags & EQGFX_CLEANUP_FLAG_KEEP) == 0)
                {
                    if (resource_type < 0x24)
                    {
                        if (resource_type == 0x23)
                            result = EQ_CDECL(unsigned long, 0x100180EB, (void *, void *))(world, object);
                        else
                        {
                            switch (resource_type)
                            {
                            case 1:
                                result = EQ_CDECL(unsigned long, 0x10018BFD, (void *, void *))(world, object);
                                break;
                            case 2:
                                nested_object = object->link_18;
                                if (nested_object->link_28 != object)
                                    result = EQ_CDECL(unsigned long, 0x10018DA3, (void *, void *))(world, object);
                                break;
                            case 3:
                                result = EQ_CDECL(unsigned long, 0x1001AF77, (void *, void *))(world, object);
                                break;
                            case 4:
                                nested_object = object->link_18;
                                if (nested_object->link_24 != object)
                                    result = EQ_CDECL(unsigned long, 0x10018DA3, (void *, void *))(world, object);
                                break;
                            case 5:
                                result = EQ_CDECL(unsigned long, 0x1001B72E, (void *, void *))(world, object);
                                break;
                            case 6:
                                nested_object = object->link_28;
                                if (nested_object->link_1C != object)
                                    result = EQ_CDECL(unsigned long, 0x1001B94A, (void *, void *))(world, object);
                                break;
                            case 7:
                                result = EQ_CDECL(unsigned long, 0x1000CF2A, (void *, void *))(world, object);
                                break;
                            case 8:
                                nested_object = object->link_6C;
                                if (nested_object->link_14 != object)
                                    result = EQ_CDECL(unsigned long, 0x1000D06A, (void *, void *))(world, object);
                                break;
                            case 9:
                                result = EQ_CDECL(unsigned long, 0x10007A5B, (void *, void *))(world, object);
                                break;
                            case 10:
                                nested_object = object->link_88;
                                if (nested_object->link_30 != object)
                                    result = EQ_CDECL(unsigned long, 0x10007D99, (void *, void *))(world, object);
                                break;
                            case 0x0B:
                                result = EQ_CDECL(unsigned long, 0x10008262, (void *, void *))(world, object);
                                break;
                            case 0x0C:
                                nested_object = object->link_2C;
                                if (nested_object->link_2C != object)
                                    result = EQ_CDECL(unsigned long, 0x1000879D, (void *, void *))(world, object);
                                break;
                            case 0x11:
                                result = EQ_CDECL(unsigned long, 0x1000D3E2, (void *, void *))(world, object);
                                break;
                            case 0x12:
                                nested_object = object->link_38;
                                if (nested_object->link_24 != object)
                                    result = EQ_CDECL(unsigned long, 0x1000D6E5, (void *, void *))(world, object);
                                break;
                            case 0x13:
                                result = EQ_CDECL(unsigned long, 0x100128BB, (void *, void *))(world, object);
                                break;
                            case 0x14:
                                nested_object = object->link_5C;
                                if (nested_object->link_1C != object)
                                    result = EQ_CDECL(unsigned long, 0x100132A2, (void *, void *))(world, object);
                                break;
                            case 0x15:
                                result = EQ_CDECL(unsigned long, 0x1001CB62, (void *, void *))(world, object);
                                break;
                            case 0x16:
                                deformation_count = EQ_CDECL(unsigned long, 0x1000ACD6, (void *))(object);
                                default_instance = EQ_CDECL(unsigned long, 0x1001D0BA, (unsigned long))(deformation_count);
                                if (default_instance != *current_item)
                                    result = EQ_CDECL(unsigned long, 0x1001CD7C, (void *, void *))(world, object);
                                break;
                            case 0x17:
                                result = EQ_CDECL(unsigned long, 0x100097F5, (void *, void *))(world, object);
                                break;
                            case 0x18:
                                nested_object = object->link_64;
                                if (nested_object->link_28 != object)
                                    result = EQ_CDECL(unsigned long, 0x1000A25C, (void *, void *))(world, object);
                                break;
                            case EQGFX_CLEANUP_WORLD_TYPE:
                                result = EQ_CDECL(unsigned long, 0x100191D8, (void *, void *))(world, object);
                                break;
                            case 0x1A:
                                result = EQ_CDECL(unsigned long, 0x1000B50E, (void *, void *))(world, object);
                                break;
                            case EQGFX_CLEANUP_BMINFO_TYPE:
                                result = EQ_CDECL(unsigned long, 0x1000BCF0, (void *, void *))(world, current_item);
                                break;
                            case 0x1C:
                                result = EQ_CDECL(unsigned long, 0x10013F01, (void *, void *))(world, object);
                                break;
                            case 0x1D:
                                nested_object = object->link_24;
                                if (nested_object->link_20 != object)
                                    result = EQ_CDECL(unsigned long, 0x100141A6, (void *, void *))(world, object);
                                break;
                            case 0x1E:
                                result = EQ_CDECL(unsigned long, 0x100144DC, (void *, void *))(world, object);
                                break;
                            case EQGFX_CLEANUP_DIRECTIONAL_LIGHT_TYPE:
                                object->use_count = 0;
                                result = EQ_CDECL(unsigned long, 0x10014C1B, (void *, void *))(world, (void *)*current_item);
                                break;
                            }
                        }
                    }
                    else
                    {
                        switch (resource_type)
                        {
                        case 0x44:
                            result = EQ_CDECL(unsigned long, 0x100205AE, (void *, void *))(world, object);
                            break;
                        case 0x45:
                            result = EQ_CDECL(unsigned long, 0x10016DCB, (void *, void *))(world, object);
                            break;
                        case 0x46:
                            result = EQ_CDECL(unsigned long, 0x10017135, (void *, void *))(world, object);
                            break;
                        case 0x47:
                            result = EQ_CDECL(unsigned long, 0x1000F990, (void *, void *))(world, object);
                            break;
                        case 0x48:
                            result = EQ_CDECL(unsigned long, 0x10010575, (void *, void *))(world, object);
                            break;
                        case 0x49:
                            result = EQ_CDECL(unsigned long, 0x10010A1F, (void *, void *))(world, object);
                            break;
                        case 0x4A:
                            result = EQ_CDECL(unsigned long, 0x10010EED, (void *, void *))(world, object);
                            break;
                        case 0x4C:
                            result = EQ_CDECL(unsigned long, 0x1003FF76, (void *, void *))(world, object);
                            break;
                        case 0x4D:
                            result = EQ_CDECL(unsigned long, 0x10040217, (void *, void *))(world, current_item);
                            break;
                        case 0x4E:
                            result = EQ_CDECL(unsigned long, 0x10010D88, (void *, void *))(world, object);
                            break;
                        case 0x4F:
                            result = EQ_CDECL(unsigned long, 0x10010EED, (void *, void *))(world, object);
                            break;
                        case 0x50:
                            result = EQ_CDECL(unsigned long, 0x1001AA66, (void *, void *))(world, object);
                            break;
                        case 0x51:
                            result = EQ_CDECL(unsigned long, 0x1001ACF8, (void *, void *))(world, object);
                            break;
                        case 0x53:
                            result = EQ_CDECL(unsigned long, 0x10019A01, (void *, void *))(world, object);
                            break;
                        }
                    }
                }
                else if (resource_type == EQGFX_CLEANUP_BMINFO_TYPE)
                    EQ_CDECL(unsigned long, 0x10045F85, (void *))(object);
            }
            item_index = item_index + 1;
            item_count = selected_table->capacity;
            current_item = current_item + 1;
        } while (item_index < item_count);
    }

    if (resource_type == EQGFX_CLEANUP_WORLD_TYPE)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))((void *)*first_item);
        item_index = 0;
        selected_table->itemCount = 0;
        selected_table->freeHead = 0;
        if (selected_table->capacity != 0)
        {
            do
            {
                item_index = item_index + 1;
                *first_item = item_index;
                first_item = first_item + 1;
            } while (item_index < selected_table->capacity);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1001c483
// Clears a world's renderer resources and releases its supporting memory.
int __cdecl t3dFlushWorld(T3D_tagWORLD *world)
{
    if (world->pools[17].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x18);
    if (world->pools[7].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x17);
    if (world->pools[15].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x12);
    if (world->pools[5].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x11);
    if (world->pools[16].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x14);
    if (world->pools[6].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x13);
    if (world->pools[28].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x16);
    if (world->pools[27].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x15);
    if (world->pools[14].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x0c);
    if (world->pools[4].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x0b);
    if (world->pools[13].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x0a);
    if (world->pools[3].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x09);
    if (world->pools[22].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1e);
    if (world->pools[23].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1f);
    if (world->pools[30].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x44);

    EQ_CDECL(void, 0x1001905A, (void *))(&world->referenceList);
    if (world->referenceBlock_guess != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->referenceBlock_guess);
        world->referenceBlock_guess = 0;
    }

    if (world->pools[34].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x48);
    if (world->pools[33].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x47);
    if (world->pools[36].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x4a);
    if (world->pools[35].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x49);
    if (world->pools[38].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x4f);
    if (world->pools[37].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x4e);
    if (world->pools[39].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x50);
    if (world->pools[40].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x51);
    if (world->pools[32].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x46);
    if (world->pools[31].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x45);
    if (world->pools[29].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x4d);
    if (world->pools[8].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x4c);
    if (world->pools[12].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 8);
    if (world->pools[2].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 7);
    if (world->regionPool.freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, EQGFX_CLEANUP_WORLD_TYPE);
    if (world->pools[21].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1d);
    if (world->pools[11].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 6);
    if (world->pools[1].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 5);
    if (world->pools[18].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1a);
    if (world->pools[19].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1b);
    if (world->pools[20].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x1c);
    if (world->pools[10].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 2);
    if (world->pools[0].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 1);
    if (world->pools[9].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 4);
    if (world->type3Pool.freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 3);
    if (world->pools[41].freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x23);
    if (world->type53Pool.freeEntries != 0)
        EQ_CDECL(unsigned long, 0x1001BA04, (void *, int))(world, 0x53);

    EQ_CDECL(void, 0x10050CBB, (void *))((void *)(world->worldDataEnd_bits - world->worldDataStart));
    world->worldDataEnd_bits = 0;
    world->worldDataStart = 0;
    if (world->visibleList != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->visibleList);
        world->visibleList = 0;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001C7CC
// Registers an animation track using supplied frame data and creates its default playback instance.
TrackDefinition2_guess *__cdecl DefineTrack_guess(void *context, char *name, unsigned int frame_count,
                                                  void *frame_data)
{
    TrackDefinition2_guess *existing_definition;
    TrackDefinition2_guess *definition;
    TrackInstance_guess *default_instance;
    const char *error_string;
    int result;

    existing_definition = EQ_CDECL(TrackDefinition2_guess *, EQGFX_T3D_GET_POINTER_FROM_DICTIONARY,
                                   (void *, char *))(context, name);
    if (existing_definition != NULL)
    {
        if (existing_definition->tag != TRACK_DEFINITION_TYPE)
        {
            error_string = "DefineTrack: Tag already exists for another data type\n";
        }
        else
        {
            return existing_definition;
        }
    }
    else
    {
        definition = EQ_CDECL(TrackDefinition2_guess *, EQGFX_T3D_ALLOCATE_MEMORY, (int))(0x28);
        if (definition != NULL)
        {
            definition->fallback_list = NULL;
            definition->instance_list = &definition->inline_fallback_list;
            definition->tag = TRACK_DEFINITION_TYPE;
            definition->use_count = 1;
            definition->inline_fallback_list.num_frames = frame_count;
            definition->dict_key = EQ_CDECL(void *, EQGFX_T3D_ALLOCATE_MEMORY, (int))(
                EQ_CDECL(unsigned int, EQGFX_STRLEN, (char *))(name) + 1);
            if (definition->dict_key == NULL)
            {
                EQGFX_OUTPUT_DEBUG_STRING_A("DefineTrack: Allocation of TAG space failed\n");
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
                return NULL;
            }
            EQ_CDECL(void *, EQGFX_COPY_STRING_WITH_NULL_TERMINATOR, (char *, char *))(
                (char *)definition->dict_key, name);
            definition->inline_fallback_list.block = frame_data;
            result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (void *, TrackDefinition2_guess *))(context, definition);
            if (result != 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING_A("DefineTrack: AddItemToList failed\n");
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition->dict_key);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
                return NULL;
            }
            result = EQ_CDECL(int, EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY,
                              (void *, TrackDefinition2_guess *))(context, definition);
            if (result < 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING_A("DefineTrack: AddEntryToDictionary failed\n");
                EQ_CDECL(void, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition2_guess *))(context, definition);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition->dict_key);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
                return NULL;
            }
            default_instance = EQ_CDECL(TrackInstance_guess *, EQGFX_T3D_ALLOCATE_MEMORY,
                                        (int))(0x38);
            if (default_instance == NULL)
            {
                EQGFX_OUTPUT_DEBUG_STRING_A("DefineTrack: Allocation of DEFAULT INSTANCE failed.\n");
                EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY,
                         (void *, char *))(context, name);
                EQ_CDECL(void, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition2_guess *))(context, definition);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition->dict_key);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
                return NULL;
            }
            definition->inline_fallback_list.default_instance = default_instance;
            default_instance->tag = TRACK_INSTANCE_TYPE;
            default_instance->dict_key = NULL;
            default_instance->default_reverse = 1.0f;
            default_instance->flags = 0;
            default_instance->use_count = 1;
            default_instance->default_interpolate = (default_instance->default_interpolate & ~0x02) | 0x01;
            default_instance->unknown_30 = 0;
            default_instance->default_frame = 0;
            default_instance->default_to_frame = 1;
            if ((unsigned int)definition->inline_fallback_list.num_frames < 2)
                default_instance->default_to_frame = 0;
            default_instance->unknown_20 = 1000;
            default_instance->unknown_24 = 0;
            default_instance->unknown_28 = 0;
            default_instance->definition = definition;
            default_instance->default_interpolation_value = 0.0f;
            result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (void *, TrackInstance_guess *))(context,
                                                                                       default_instance);
            if (result != 0)
            {
                EQGFX_OUTPUT_DEBUG_STRING_A("DefineTrack: AddItemToList of default instance failed\n");
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(
                    definition->inline_fallback_list.default_instance);
                EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY,
                         (void *, char *))(context, name);
                EQ_CDECL(void, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition2_guess *))(context, definition);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition->dict_key);
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
                return NULL;
            }
            return definition;
        }
        error_string = "DefineTrack: Error allocating memory\n";
    }
    EQGFX_OUTPUT_DEBUG_STRING_A(error_string);
    return NULL;
}

// FUNCTION: EQGFX 0x1001c9b4
// Determine storage needed for a track definition and its frames.
// __cdecl (the callee pops nothing, ECX is not an input). Bytes a track definition allocation needs: six fixed
// 0x10-byte entries plus `numFrames` of them, and 1 + strlen(name) more once `name` is given.
int __cdecl s3dCalcTrackDef2AllocSize(char *name, int numFrames)
{
    int size;

    size = (numFrames + 6) * 0x10;
    if (name != NULL)
        size = size + 1 + strlen(name);
    return size;
}

// FUNCTION: EQGFX 0x1001c9d8
// Defines an animation track with its own copy of the supplied frames and a default playback instance.
TrackDefinition2_guess *__cdecl t3dDefineTrack2_guess(void *context, TrackDefinition2_guess *preallocated_definition,
                                              char *name, unsigned int frame_count, void *frame_data)
{
    TrackDefinition2_guess *existing_definition;
    TrackDefinition2_guess *definition;
    TrackInstanceList_guess *instance_list;
    TrackInstance_guess *default_instance;
    char *next_memory;
    unsigned int name_length;
    unsigned int frame_data_size;
    int allocation_size;
    int result;

    existing_definition = EQ_CDECL(TrackDefinition2_guess *, EQGFX_T3D_GET_POINTER_FROM_DICTIONARY,
                                   (void *, char *))(context, name);
    if (existing_definition != NULL)
        return (TrackDefinition2_guess *)(~-(unsigned int)(existing_definition->tag != TRACK_DEFINITION_TYPE) &
                                          (unsigned int)existing_definition);

    definition = preallocated_definition;
    if (preallocated_definition == NULL)
    {
        allocation_size = EQ_CDECL(int, EQGFX_S3D_CALC_TRACK_DEF2_ALLOC_SIZE,
                                   (char *, int))(name, frame_count);
        definition = EQ_CDECL(TrackDefinition2_guess *, EQGFX_T3D_ALLOCATE_MEMORY,
                              (int))(allocation_size);
        if (definition == NULL)
            return NULL;
    }

    definition->tag = TRACK_DEFINITION_TYPE;
    definition->use_count = 1;
    definition->flags = 0;
    if (preallocated_definition != NULL)
        definition->flags |= 0x20;
    definition->instance_list = NULL;
    instance_list = &definition->inline_fallback_list;
    definition->fallback_list = instance_list;
    next_memory = (char *)(instance_list + 1);
    if (name == NULL)
    {
        definition->dict_key = NULL;
    }
    else
    {
        definition->dict_key = next_memory;
        name_length = EQ_CDECL(unsigned int, EQGFX_STRLEN, (char *))(name);
        next_memory = next_memory + name_length + 1;
        EQ_CDECL(void *, EQGFX_COPY_STRING_WITH_NULL_TERMINATOR,
                 (char *, char *))((char *)definition->dict_key, name);
    }
    instance_list->num_frames = frame_count;
    if (frame_count == 0)
    {
        instance_list->block = NULL;
    }
    else
    {
        instance_list->block = next_memory;
        frame_data_size = frame_count * 0x10;
        next_memory = next_memory + frame_data_size;
        EQ_CDECL(void *, EQGFX_MEMCPY, (void *, void *, unsigned int))(
            instance_list->block, frame_data, frame_data_size);
    }
    instance_list->default_instance = next_memory;

    result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (void *, TrackDefinition2_guess *))(context, definition);
    if (result == 0)
    {
        result = EQ_CDECL(int, EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY,
                          (void *, TrackDefinition2_guess *))(context, definition);
        if (result < 0)
        {
            EQ_CDECL(void, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition2_guess *))(context, definition);
        }
        else
        {
            default_instance = (TrackInstance_guess *)instance_list->default_instance;
            default_instance->tag = TRACK_INSTANCE_TYPE;
            default_instance->default_reverse = 1.0f;
            default_instance->dict_key = NULL;
            default_instance->flags = 0;
            default_instance->use_count = 1;
            default_instance->default_interpolate =
                (default_instance->default_interpolate & ~0x02) | 0x01;
            default_instance->unknown_30 = 0;
            default_instance->default_frame = 0;
            default_instance->default_to_frame = 1;
            if (frame_count < 2)
                default_instance->default_to_frame = 0;
            default_instance->unknown_20 = 1000;
            default_instance->unknown_24 = 0;
            default_instance->unknown_28 = 0;
            default_instance->definition = definition;
            default_instance->default_interpolation_value = 0.0f;
            result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (void *, TrackInstance_guess *))(context, default_instance);
            if (result == 0)
                return definition;
            EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY,
                     (void *, char *))(context, name);
            EQ_CDECL(void, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition2_guess *))(context, definition);
        }
    }
    if (definition != NULL && preallocated_definition == NULL)
        EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
    return NULL;
}

// FUNCTION: EQGFX 0x1001cb62
// __cdecl (the callee pops nothing, ECX is not an input). Returns 0xffff3d44 unless the definition's use count (+0x10)
// and its default instance's use count are both 1; else undefines it (the default instance: the list at +0x14, or +0x18).
int __cdecl t3dUndefineTrack(void *context, TrackDefinition_guess *definition)
{
    TrackInstanceList_guess *instance_list;
    SimpleSprite_guess *default_instance;
    void *block;
    int result;

    instance_list = definition->instance_list;
    if (instance_list == NULL)
        instance_list = definition->fallback_list;
    default_instance = instance_list->default_instance;
    if (definition->use_count == 1 && default_instance->use_count == 1)
    {
        if ((default_instance->flags & SIMPLE_SPRITE_FLAG_IN_DICT) != 0)
            EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY, (void *, unsigned long))(context, default_instance->dict_key);
        EQ_CDECL(int, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, SimpleSprite_guess *))(context, default_instance);
        if ((definition->flags & TRACK_FLAG_IN_DICT) != 0)
            EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY, (void *, void *))(context, definition->dict_key);
        result = EQ_CDECL(int, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, TrackDefinition_guess *))(context, definition);
        if (default_instance->dict_key != 0)
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))((void *)default_instance->dict_key);
        if (definition->instance_list != NULL)
        {
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(default_instance);
            block = definition->instance_list->block;
            if (block != NULL)
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(block);
            if (definition->dict_key != NULL)
                EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition->dict_key);
        }
        if ((definition->flags & TRACK_FLAG_KEEP_MEMORY) == 0)
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(definition);
        return result;
    }
    return 0xFFFF3D44;
}

// FUNCTION: EQGFX 0x1001cc18
// Determine storage needed for a track instance and its optional name.
// __cdecl (the callee pops nothing, ECX is not an input). Bytes a track allocation needs: the fixed
// 0x38-byte header alone, or 0x39 + strlen(name) once `name` is given (its terminator included).
int __cdecl s3dCalcTrackAllocSize(char *name)
{
    int size;

    size = 0x38;
    if (name != NULL)
        size = strlen(name) + 0x39;
    return size;
}

#define EQGFX_S3D_CALC_TRACK_ALLOC_SIZE 0x1001CC18

// FUNCTION: EQGFX 0x1001cc30
// Creates an animation track instance initialized from its definition's default playback settings.
Track_guess *__cdecl t3dCreateTrack(SpriteContext_guess *context, Track_guess *preallocated_track, char *name, TrackDefinition_guess *definition)
{
    Track_guess *track;
    TrackInstanceList_guess *instance_list;
    unsigned long *source_words;
    unsigned long *destination_words;
    int allocation_size;
    int copy_count;
    int result;

    track = preallocated_track;
    if (preallocated_track == NULL)
    {
        allocation_size = EQ_CDECL(int, EQGFX_S3D_CALC_TRACK_ALLOC_SIZE, (char *))(name);
        track = (Track_guess *)EQ_CDECL(void *, EQGFX_T3D_ALLOCATE_MEMORY, (unsigned long))(allocation_size);
        if (track == NULL)
            return NULL;
    }
    instance_list = definition->instance_list;
    if (instance_list == NULL)
        instance_list = definition->fallback_list;
    source_words = (unsigned long *)instance_list->default_instance;
    destination_words = (unsigned long *)track;
    for (copy_count = 0xe; copy_count != 0; copy_count--)
    {
        *destination_words = *source_words;
        source_words++;
        destination_words++;
    }
    track->dict_key = NULL;
    *((unsigned long *)&track->flags) = 0;
    track->use_count = 0;
    track->unknown_24 = track->unknown_20 + context->base_08;
    if (preallocated_track != NULL)
        *((unsigned long *)&track->flags) |= TRACK_FLAG_KEEP_MEMORY;
    if (name == NULL)
    {
        track->dict_key = NULL;
    }
    else
    {
        track->dict_key = (char *)track + sizeof(Track_guess);
        EQ_CDECL(unsigned long, EQGFX_STRLEN, (char *))(name);
        EQ_CDECL(void, EQGFX_COPY_STRING_WITH_NULL_TERMINATOR, (char *, char *))((char *)track->dict_key, name);
    }
    result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (SpriteContext_guess *, Track_guess *))(context, track);
    if (result == 0)
    {
        if (name == NULL || (result = EQ_CDECL(int, EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY, (SpriteContext_guess *, Track_guess *))(context, track)) >= 0)
        {
            definition->use_count++;
            return track;
        }
        EQ_CDECL(int, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (SpriteContext_guess *, Track_guess *))(context, track);
    }
    else
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086F08));
    }
    if (track != NULL && preallocated_track == NULL)
        EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(track);
    return NULL;
}

// FUNCTION: EQGFX 0x1001cd0c
// Make a separate track instance from an existing track.
// __cdecl. Copies the source track (0x38 bytes; dict_key, flags, use_count cleared; +0x24 = +0x20 + the context's base_08)
// and links it via _t3dAddItemToList_guess: 0 bumps the definition's use count and returns the copy; else frees it. NULL on failure.
Track_guess *__cdecl t3dDuplicateTrack(SpriteContext_guess *context, Track_guess *source)
{
    Track_guess *duplicate;
    int result;

    duplicate = (Track_guess *)EQ_CDECL(void *, EQGFX_T3D_ALLOCATE_MEMORY, (unsigned long))(sizeof(Track_guess));
    if (duplicate == NULL)
    {
        EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086F54));
    }
    else
    {
        *duplicate = *source;
        duplicate->dict_key = 0;
        *(unsigned long *)&duplicate->flags = 0;
        duplicate->use_count = 0;
        duplicate->unknown_24 = duplicate->unknown_20 + context->base_08;
        result = EQ_CDECL(int, EQGFX__T3D_ADD_ITEM_TO_LIST, (SpriteContext_guess *, Track_guess *))(context, duplicate);
        if (result != 0)
        {
            EQGFX_OUTPUT_DEBUG_STRING_A(EQ_ARRAY(char, 0x10086F30));
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (Track_guess *))(duplicate);
        }
        else
        {
            duplicate->definition->use_count++;
            return duplicate;
        }
    }
    return NULL;
}

// FUNCTION: EQGFX 0x1001cd7c
// Release a track instance from its track definition.
// __cdecl (the callee pops nothing, ECX is not an input). 0xffff3d45 while the track's use count (+0x10) is not 0.
// Else: the definition's use count drops, the dictionary entry goes (flag 0x01), t3dFreeObjectPoolSlot_guess runs, then the memory is freed unless flag 0x20.
int __cdecl t3dDestroyTrack(void *context, Track_guess *track)
{
    int result;

    if (track->use_count != 0)
        return 0xffff3d45;
    track->definition->use_count--;
    if ((track->flags & TRACK_FLAG_IN_DICT) != 0)
        EQ_CDECL(void, EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY, (void *, void *))(context, track->dict_key);
    result = EQ_CDECL(int, EQGFX_T3D_FREE_OBJECT_POOL_SLOT, (void *, Track_guess *))(context, track);
    if ((track->flags & TRACK_FLAG_KEEP_MEMORY) == 0)
    {
        if (track->dict_key != NULL)
            EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(track->dict_key);
        EQ_CDECL(void, EQGFX_T3D_FREE_MEMORY, (void *))(track);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001cdd7
// __cdecl (the callee pops nothing, ECX is not an input). 1 when the definition's tag is 0x15 and its default instance (the instance list at +0x14, or the fallback list at +0x18 while that is null) has tag 0x16; else 0.
int __cdecl t3dIsTrackDefinitionValid(TrackDefinition_guess *definition)
{
    TrackInstanceList_guess *instance_list;
    SimpleSprite_guess *default_instance;

    if (definition != NULL && definition->tag == 0x15)
    {
        instance_list = definition->instance_list;
        if (instance_list != NULL)
        {
            default_instance = instance_list->default_instance;
            if (default_instance == NULL)
                return 0;
        }
        else
        {
            default_instance = definition->fallback_list->default_instance;
            if (default_instance == NULL)
                return 0;
        }
        if (default_instance->tag == 0x16)
            return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001ce0d
// Get the frame count stored on a track definition.
// __cdecl (the callee pops nothing, ECX is not an input). The frame count of a definition: its instance list at +0x14, or the fallback list at +0x18 while that is null.
int __cdecl t3dGetTrackDefinitionNumFrames(TrackDefinition_guess *definition)
{
    TrackInstanceList_guess *list;

    list = definition->instance_list;
    if (list == NULL)
        list = definition->fallback_list;
    return list->num_frames;
}

// FUNCTION: EQGFX 0x1001ce1e
// Get the frame count available through a track instance.
// __cdecl (the callee pops nothing, ECX is not an input). The track's frame count, through its
// definition at +0x14: the instance list at +0x14, or the fallback list at +0x18 while it is null.
int __cdecl t3dGetTrackNumFrames(Track_guess *track)
{
    TrackDefinition_guess *definition;
    TrackInstanceList_guess *list;

    definition = track->definition;
    list = definition->instance_list;
    if (list != NULL)
        return list->num_frames;
    return definition->fallback_list->num_frames;
}

// FUNCTION: EQGFX 0x1001ce35
// Retrieves the scale, rotation angles and position of a specified animation track definition frame.
int __cdecl t3dGetTrackDefinitionTransformation(TrackDefinition_guess *definition, unsigned long frame,
                                                float *scale_output, float *pitch, float *roll,
                                                float *heading, float *position_output)
{
    TrackInstanceList_guess *instance_list;
    unsigned long frame_count;
    TrackPackedFrame_guess *packed_frame;
    TrackInstanceFrame_guess *instance_frame;
    float quaternion[4];
    float position[3];

    instance_list = definition->instance_list;
    if (instance_list == NULL)
        frame_count = definition->fallback_list->num_frames;
    else
        frame_count = instance_list->num_frames;
    if (frame >= frame_count)
        return TRACK_FRAME_OUT_OF_RANGE;
    if (instance_list == NULL)
    {
        packed_frame = (TrackPackedFrame_guess *)definition->fallback_list->block + frame;
        quaternion[0] = (float)packed_frame->quaternion[0] * EQGFX_FRAME_QUATERNION_SCALE;
        quaternion[1] = (float)packed_frame->quaternion[1] * EQGFX_FRAME_QUATERNION_SCALE;
        quaternion[2] = (float)packed_frame->quaternion[2] * EQGFX_FRAME_QUATERNION_SCALE;
        quaternion[3] = (float)packed_frame->quaternion[3] * EQGFX_FRAME_QUATERNION_SCALE;
        *(float *)&definition = (float)packed_frame->scale * EQGFX_FRAME_POSITION_SCALE;
        position[0] = (float)packed_frame->position[0] * EQGFX_FRAME_POSITION_SCALE;
        position[1] = (float)packed_frame->position[1] * EQGFX_FRAME_POSITION_SCALE;
        position[2] = (float)packed_frame->position[2] * EQGFX_FRAME_POSITION_SCALE;
    }
    else
    {
        instance_frame = (TrackInstanceFrame_guess *)instance_list->block + frame;
        quaternion[0] = instance_frame->quaternion[0];
        quaternion[1] = instance_frame->quaternion[1];
        quaternion[2] = instance_frame->quaternion[2];
        quaternion[3] = instance_frame->quaternion[3];
        *(float *)&definition = instance_frame->scale;
        position[0] = instance_frame->position[0];
        position[1] = instance_frame->position[1];
        position[2] = instance_frame->position[2];
    }
    EQ_CDECL(void, 0x1004FBEB, (float *, float *, float *, float *))(quaternion, roll, heading, pitch);
    *scale_output = *(float *)&definition;
    position_output[0] = position[0];
    position_output[1] = position[1];
    position_output[2] = position[2];
    return 0;
}

// FUNCTION: EQGFX 0x1001cf5e
// Set one frame's transform in a track definition.
int __cdecl t3dSetTrackDefinitionTransformation(TrackDefinition_guess *definition, unsigned long frame,
                                                float scale, float z_angle, float x_angle,
                                                float y_angle, float *position)
{
    TrackInstanceList_guess *instance_list;
    unsigned long frame_count;
    TrackPackedFrame_guess *packed_frame;
    TrackInstanceFrame_guess *instance_frame;

    instance_list = definition->instance_list;
    if (instance_list != NULL)
        frame_count = instance_list->num_frames;
    else
        frame_count = definition->fallback_list->num_frames;
    if (frame >= frame_count)
        return TRACK_FRAME_OUT_OF_RANGE;
    if (instance_list != NULL)
    {
        instance_frame = (TrackInstanceFrame_guess *)instance_list->block + frame;
        EQ_CDECL(void, EQGFX_FUN_1001CFD0,
                 (TrackTransformation_guess *, float, float, float, float, float *))(
            (TrackTransformation_guess *)instance_frame, scale, z_angle, x_angle, y_angle, position);
    }
    else
    {
        packed_frame = (TrackPackedFrame_guess *)definition->fallback_list->block + frame;
        EQ_CDECL(void, EQGFX_FUN_1001D009, (unsigned short *, float, float, float, float, float *))(
            (unsigned short *)packed_frame, scale, z_angle, x_angle, y_angle, position);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001cfd0
// Create a transform value from scale, rotation, and position.
void __cdecl t3dSetTrackTransformation_guess(TrackTransformation_guess *transformation, unsigned long scale,
                          float z_angle, float x_angle, float y_angle,
                          TrackPosition_guess *position)
{
    EQ_CDECL(void, 0x1004F89A, (float, float, float, float *))(
        x_angle, y_angle, z_angle, (float *)transformation);
    transformation->scale = scale;
    transformation->position = *position;
}

// FUNCTION: EQGFX 0x1001d009
// The first helper's cdecl stack cleanup follows the initial x87 scaling, as in the original.
void __cdecl s3dPackTrackTransformation_guess(unsigned short *packed_values, float position_scale,
                           float angle_one, float angle_two, float angle_three,
                           float *position)
{
    float quaternion[4];
    unsigned short converted_value;

    EQ_STDCALL(void, EQGFX_T3D_EULER_TO_QUATERNION, (float, float, float, float *))(
        angle_two, angle_three, angle_one, quaternion);
    __asm {
        fld dword ptr EQ_ASM_GLOBAL(0x100762AC)
        lea ecx, quaternion
        fld dword ptr [ecx]
        fmul st(0), st(1)
        add esp, 0x10
    }
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    __asm {
        lea ecx, quaternion
        fld dword ptr [ecx + 4]
    }
    packed_values[0] = converted_value;
    __asm fmul st(0), st(1)
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    __asm {
        lea ecx, quaternion
        fld dword ptr [ecx + 8]
    }
    packed_values[1] = converted_value;
    __asm fmul st(0), st(1)
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    __asm {
        lea ecx, quaternion
        fld dword ptr [ecx + 12]
    }
    packed_values[2] = converted_value;
    __asm fmul st(0), st(1)
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    __asm {
        fstp st
        fld dword ptr EQ_ASM_GLOBAL(0x100762A8)
        fld dword ptr position_scale
        fmul st(0), st(1)
    }
    packed_values[3] = converted_value;
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    __asm mov edi, position
    packed_values[7] = converted_value;
    __asm {
        fld dword ptr [edi]
        fmul st(0), st(1)
    }
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    packed_values[4] = converted_value;
    __asm fld dword ptr [edi + 4]
    __asm fmul st(0), st(1)
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    packed_values[5] = converted_value;
    __asm fld dword ptr [edi + 8]
    __asm fmul st(0), st(1)
    converted_value = EQ_CDECL(unsigned short, EQGFX_X87_FLOAT_TO_WORD, (void))();
    packed_values[6] = converted_value;
    __asm fstp st
}

// FUNCTION: EQGFX 0x1001d0ba
// Get the default sprite instance for a track definition.
// __cdecl (the callee pops nothing, ECX is not an input). The instance list at +0x14, the one at +0x18
// while it is null; returns that list's default instance.
void *__cdecl t3dGetTrackDefinitionDefaultInstance(TrackDefinition_guess *definition)
{
    TrackInstanceList_guess *list;

    list = definition->instance_list;
    if (list == NULL)
        list = definition->fallback_list;
    return list->default_instance;
}

// FUNCTION: EQGFX 0x1001d0cc
// Check whether a track instance and its definition are valid.
// __cdecl (the callee pops nothing; ECX is not an input). 1 when the track's definition at +0x14 is non-null, its first word is 0x15 and the track's own first word is 0x16; else 0.
int __cdecl t3dIsTrackValid(Track_guess *track)
{
    TrackDefinition_guess *definition;

    if (track != NULL)
    {
        definition = track->definition;
        if (definition != NULL && *(int *)definition == 0x15)
            return *(int *)track == 0x16;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001d0ec
// __cdecl (the callee pops nothing; ECX is not an input). Returns the word at the frame's +0x18.
int __cdecl t3dGetTrackDefaultFromFrame(TrackFrame_guess *frame)
{
    return frame->default_value;
}

// FUNCTION: EQGFX 0x1001d0f5
// __cdecl (the callee pops nothing; ECX is not an input). Returns the word at the frame's +0x1A.
int __cdecl t3dGetTrackDefaultToFrame(TrackFrame_guess *frame)
{
    return frame->default_to_frame;
}

// FUNCTION: EQGFX 0x1001d0fe
// __cdecl (the callee pops nothing; ECX is not an input). Returns the actor's +0x1C float on the x87 stack.
float __cdecl t3dGetActorHeading(T3D_tagACTORINSTANCE *actor)
{
    return actor->orientation_guess[0];
}

// FUNCTION: EQGFX 0x1001d106
// Set a track's default starting frame.
// __cdecl (the callee pops nothing; ECX is not an input). Sets the track's default frame to `frame` while it is below
// the frame count (the definition's instance list, or its fallback list while that is null); else returns 0xffff3d3e.
int __cdecl t3dSetTrackDefaultFromFrame(Track_guess *track, unsigned long frame)
{
    TrackDefinition_guess *definition;
    TrackInstanceList_guess *list;
    unsigned long frame_count;

    definition = track->definition;
    list = definition->instance_list;
    if (list != NULL)
        frame_count = list->num_frames;
    else
        frame_count = definition->fallback_list->num_frames;
    if (frame >= frame_count)
        return 0xffff3d3e;
    track->default_frame = (short)frame;
    return 0;
}

// FUNCTION: EQGFX 0x1001d132
// Set a track's default destination frame.
// __cdecl (the callee pops nothing; ECX is not an input). Sets the track's +0x1A default frame to `frame` while it is below
// the frame count (the definition's instance list, or its fallback list while that is null); else returns 0xffff3d3e.
int __cdecl t3dSetTrackDefaultToFrame(Track_guess *track, unsigned long frame)
{
    TrackDefinition_guess *definition;
    TrackInstanceList_guess *list;
    unsigned long frame_count;

    definition = track->definition;
    list = definition->instance_list;
    if (list != NULL)
        frame_count = list->num_frames;
    else
        frame_count = definition->fallback_list->num_frames;
    if (frame >= frame_count)
        return 0xffff3d3e;
    track->default_to_frame = (short)frame;
    return 0;
}

// FUNCTION: EQGFX 0x1001d15e
// Set a track's default interpolation amount.
// __cdecl (the callee pops nothing; ECX is not an input). Stores `value` as the track's +0x1C default interpolation
// value while it is in [0, 1] (NaN fails); else returns 0xffff3d3e.
int __cdecl t3dSetTrackDefaultfInterpolationValue(Track_guess *track, float value)
{
    if (value <= EQGFX_FLOAT_ONE && value >= EQGFX_ZERO_F_GUESS)
    {
        track->default_interpolation_value = value;
        return 0;
    }
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x1001d190
// Check whether a track's default direction is reversed.
int __cdecl t3dGetTrackDefaultReverse(Track_guess *track)
{
    if (track->default_reverse < EQGFX_ZERO_F_GUESS)
    {
        return 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001d1a9
// Set a track's default playback direction.
// __cdecl (the callee pops nothing; ECX is not an input). Forces the sign of the track's +0x2C float to match
// `reverse` (negative when 1, non-negative when 0) and returns 0.
int __cdecl t3dSetTrackDefaultReverse(char *track, int reverse)
{
    // xor the materialized (f < 0) flag; the original's x87 compare puts an unordered (NaN) float on the
    // "less" side.
    if ((*(float *)(track + TRACK_REVERSE_OFFSET_GUESS) < EQGFX_ZERO_F_GUESS) ^ reverse)
        *(float *)(track + TRACK_REVERSE_OFFSET_GUESS) = -*(float *)(track + TRACK_REVERSE_OFFSET_GUESS);
    return 0;
}

// FUNCTION: EQGFX 0x1001d1d3
// __cdecl (the callee pops nothing; ECX is not an input). Returns bit 0 of the track's dword at +0x34.
unsigned long __cdecl t3dGetTrackDefaultInterpolate(Track_guess *track)
{
    return track->default_interpolate & 1;
}
