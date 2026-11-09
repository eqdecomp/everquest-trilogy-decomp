#include "trilogy/eqaddr.h"
#include <stdio.h>

typedef struct BMInfoContext_guess
{
    unsigned char unused_000[0x19c];
    void **bitmap_info_slots;
} BMInfoContext_guess;

typedef struct BMInfoTextureSurface_guess BMInfoTextureSurface_guess;

typedef struct BMInfoTextureSurfaceVTable_guess
{
    void *unused_00;
    void *unused_04;
    unsigned long (__stdcall *release)(BMInfoTextureSurface_guess *surface);
} BMInfoTextureSurfaceVTable_guess;

struct BMInfoTextureSurface_guess
{
    BMInfoTextureSurfaceVTable_guess *vtable;
};

typedef struct BMInfo_guess
{
    unsigned int type;
    int slot_index;
    char *name;
    unsigned int unused_0c;
    unsigned int unused_10;
    unsigned int unused_14;
    unsigned int unused_18;
    unsigned int width;
    unsigned int height;
    unsigned int unused_24;
    unsigned int unused_28;
    unsigned int unused_2c;
    unsigned int unused_30;
    unsigned int unused_34;
    unsigned int unused_38;
    unsigned char unused_3c[0x24];
    BMInfoTextureSurface_guess *texture_surface;
    unsigned int unused_64;
    unsigned int unused_68;
    unsigned int unused_6c;
    unsigned int unused_70;
    unsigned int unused_74;
    unsigned int unused_78;
    unsigned int unused_7c;
    unsigned int unused_80;
    unsigned int unused_84;
    unsigned char unused_88[0x0c];
} BMInfo_guess;

#define BMINFO_TEXTURE_TYPE 0x1b
#define BMINFO_ALLOCATION_SIZE 0x94
#define GFX_TEXTURE_LOG_NAME EQ_ARRAY(char, 0x10086164)
#define GFX_TEXTURE_LOG_MODE EQ_ARRAY(char, 0x10086170)
#define EQGFX_INVALID_TEXTURE_FORMAT EQ_ARRAY(char, 0x10086124)
#define EQGFX_LOG_STRING EQ_GLOBAL(char *, 0x00000030)
#define g_render_mode_guess EQ_GLOBAL(int, 0x100FB8A8)
#define EQGFX_GET_POINTER_FROM_DICTIONARY EQ_CDECL(BMInfo_guess *, 0x1000E7FE, (int, char *))
#define EQGFX_OPEN_FILE EQ_CDECL(FILE *, 0x10068412, (char *, char *))
#define EQGFX_FPRINTF EQ_CDECL(int, 0x100683A5, (FILE *, char *, ...))
#define EQGFX_FCLOSE EQ_CDECL(int, 0x10068328, (FILE *))
#define EQGFX_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define EQGFX_STRING_LENGTH EQ_CDECL(size_t, 0x10068160, (char *))
#define EQGFX_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX_COPY_STRING EQ_CDECL(void, 0x10068070, (char *, char *))
#define EQGFX_DEFINE_BMINFO EQ_CDECL(int, 0x1000C001, (BMInfo_guess *, int, int))
#define EQGFX_ADD_TO_LIST EQ_CDECL(int, 0x1001564B, (int, BMInfo_guess *))
#define EQGFX_ADD_ENTRY_TO_DICTIONARY EQ_CDECL(int, 0x1000DC08, (int, BMInfo_guess *))
#define EQGFX_REMOVE_FROM_LIST EQ_CDECL(void, 0x10015D81, (int, BMInfo_guess *))
#define EQGFX_REFRESH_BMINFO EQ_CDECL(void, 0x1003193D, (BMInfo_guess *))
#define EQGFX__T3D_RESOLVE_BITMAP_SEARCH_PATHS EQ_CDECL(int, 0x1000BEC9, (int, BMInfo_guess *))
#define EQGFX_LOAD_BMINFO EQ_CDECL(int, 0x1000BABA, (int, BMInfo_guess *, int))
#define EQGFX_DEFINE_BMINFO_MIPMAPPED EQ_CDECL(int, 0x1000B5D6, (int, BMInfo_guess *, char *, unsigned int, char **, int, int))

/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

typedef struct SpriteDefinition_guess
{
    unsigned char unused_00[0x0c];
    unsigned int  flags;                    // 0x0c  bit 0 is the bit t3dIs2DSpriteDefinitionInDictionary tests
} SpriteDefinition_guess;

typedef struct ActorLight_guess
{
    unsigned char unused_00[0x10];
    unsigned int  lightValue;               // 0x10  the dword t3dGetActorLight returns
    unsigned char unused_14[0x0c];
    float         radiusOfInfluence;        // 0x20  the float t3dGetActorLightRadiusOfInfluence returns
} ActorLight_guess;

typedef struct LightValue_guess
{
    unsigned char unused_00[0x10];
    int           refCount;                 // 0x10  the count t3dSetActorLight raises on a stored value and lowers on the one it replaces
} LightValue_guess;

/* RenderVolume_guess: the volume layout is RenderVolume_guess (generated: eqgfx/layout/render_volume.h); the old local view's members map by offset. */
#include "eqgfx/layout/render_volume.h"

typedef struct BoundingShape_guess
{
    unsigned int  shapeType;                // 0x00  0, 2, 4, 0x2a, 0x2b, 0x52 or 0x54 (other types are rejected)
    float         vector[3];                // 0x04  x, y, z: type 0x2a reads vector[1], type 0x2b takes the length of the vector
    float         radiusAdd;                // 0x10  added to the vector length (type 0x2b)
    float         factor;                   // 0x14  times the scale object's float at +0x14 (types 2 and 4)
    void         *scale;                    // 0x18  the scale object (types 2 and 4)
} BoundingShape_guess;

typedef struct ActorDefinition_guess
{
    unsigned char unused_00[0x14];
    unsigned int  levelCount;               // 0x14  entries in the level array; an index at or past this returns 0
    unsigned char *levels;                  // 0x18  array of 8-byte entries; the function returns the dword at each entry's start
} ActorDefinition_guess;

typedef struct ShapeSet_guess
{
    unsigned char unused_00[0x2c];
    BoundingShape_guess *root;              // 0x2c  the root bounding shape, null if none (its radius is taken after the loops)
    unsigned char unused_30[0x14];
    RenderVolume_guess **volumes;           // 0x44  array of render volume pointers; the volume count is at +0x1c of the definition
} ShapeSet_guess;

typedef struct ShapeScale_guess
{
    unsigned char unused_00[0x14];
    float         scaleValue;               // 0x14  the float a type 2 or 4 BoundingShape's factor is multiplied by
} ShapeScale_guess;

typedef struct ActorSprite_guess
{
    unsigned int  spriteType;               // 0x00  the kind t3dSetActorScaleFactor switches on (8, 0xa, 0xc, 0x12, 0x14, 0x48, 0x51)
    unsigned char unused_04[0x10];
    float         unknown0x14;              // 0x14  x of the vector a type 0x48 sprite gives (y, z follow)
    unsigned char unused_18[0x04];
    float         unknown0x1c;              // 0x1c  x of the vector a type 0xc sprite gives (y, z follow)
    unsigned char unused_20[0x08];
    float         unknown0x28;              // 0x28  x of the vector a type 0x14 sprite gives (y, z follow)
    unsigned int  unknown0x2c;              // 0x2c  the argument a type 0xc sprite passes; a type 0x12 sprite's vector starts here
    unsigned char unused_30[0x04];
    unsigned int  unknown0x34;              // 0x34  the argument a type 0x48 sprite passes
    unsigned int  unknown0x38;              // 0x38  the argument a type 0x12 sprite passes
    unsigned char unused_3c[0x20];
    unsigned int  unknown0x5c;              // 0x5c  the argument a type 0x14 sprite passes
    unsigned char unused_60[0x18];
    float         unknown0x78;              // 0x78  x of the vector a type 0xa sprite gives (y, z follow)
    unsigned char unused_7c[0x0c];
    unsigned int  unknown0x88;              // 0x88  the argument a type 0xa sprite passes
} ActorSprite_guess;

// FUNCTION: EQGFX 0x1000a99b
// Stores a caller-supplied value on an actor definition.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image).
// __cdecl (the callee pops nothing; ECX is not an input). Stores the user-data dword at +0x24 of the actor
// definition object (no include/ struct models it; the field is the Ghidra name chain). Returns 0.
int __cdecl t3dSetActorDefinitionUserData(char *definition, unsigned int userData)
{
    *(unsigned int *)(definition + 0x24) = userData;
    return 0;
}

// FUNCTION: EQGFX 0x1000a9a9
// Gives an actor the collision volume used for its interactions with the world.
// __cdecl (three stack arguments; the callee pops nothing). Returns 0, or 0xFFFF3D41 when the duplicate of sourceVolume
// can't be made. Else destroys the actor's collision volume at +0x2c, stores the duplicate there, and a type-4 duplicate gets the actor.
int __cdecl t3dSetActorCollisionVolume(void *context, T3D_tagACTORINSTANCE *actor, void *sourceVolume)
{
    int result;
    RenderVolume_guess *duplicate;

    result = 0;
    duplicate = (RenderVolume_guess *)EQ_CDECL(void *, 0x1001D516, (void *, void *))(context, sourceVolume);
    if (duplicate == 0)
    {
        result = 0xffff3d41;
    }
    else
    {
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, actor->collisionVolume_guess);
        actor->collisionVolume_guess = duplicate;
        if (duplicate->volumeType == 4)
        {
            duplicate->owningActor = actor;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000a9ed
// __cdecl. Returns the volume type dword of the actor's render volume (+0x30), or 0x2c when the actor has none.
unsigned int __cdecl t3dGetActorRenderVolumeType(T3D_tagACTORINSTANCE *actor)
{
    RenderVolume_guess *volume = (RenderVolume_guess *)actor->renderVolume_guess;
    if (volume != 0)
        return volume->volumeType;
    return 0x2c;
}

// FUNCTION: EQGFX 0x1000a9ff
// Supplies an actor's render volume, creating a default scaled shape when needed.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). __cdecl (one stack argument, callee
// pops nothing). Returns the render volume at +0x30; when there is none, creates a sphylinder from the scale at +0x34.
void *__cdecl t3dGetActorRenderVolume(T3D_tagACTORINSTANCE *actor)
{
    float scale;

    if (actor->renderVolume_guess == 0)
    {
        scale = actor->scaleFactor_guess;
        actor->renderVolume_guess = EQ_CDECL(void *, 0x1001D422, (float, float))(scale, scale + scale);
    }
    return actor->renderVolume_guess;
}

#define EQGFX_EMPTY_RENDER_VOLUME ((void *)0x100F401C)

// FUNCTION: EQGFX 0x1000aa28
// Replaces or clears an actor's render volume, updating its size bounds when a new volume is supplied.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). __cdecl (three stack arguments; the callee pops nothing).
// Returns 0, 0xFFFF3D41 when the duplicate of desc can't be made, or 0xFFFF3D3E for an unknown desc type.
int __cdecl t3dSetActorRenderVolume(void *context, T3D_tagACTORINSTANCE *actor, int *desc)
{
    int result;
    RenderVolume_guess *volume;
    float scale;

    result = 0;
    if (desc == 0)
    {
        EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, actor->renderVolume_guess);
        actor->renderVolume_guess = EQGFX_EMPTY_RENDER_VOLUME;
        actor->scaleFactor_guess = 0.0f;
        scale = 0.0f;
    }
    else
    {
        switch (*desc)
        {
        case 0:
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, actor->renderVolume_guess);
            actor->renderVolume_guess = EQGFX_EMPTY_RENDER_VOLUME;
            goto set_flags;
        case 0x2a:
            volume = (RenderVolume_guess *)EQ_CDECL(void *, 0x1001D516, (void *, int *))(context, desc);
            if (volume == 0)
            {
                result = 0xffff3d41;
                goto done;
            }
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, actor->renderVolume_guess);
            actor->renderVolume_guess = volume;
            scale = volume->radius_guess;
            volume->renderActor = actor;
            break;
        case 0x2c:
            volume = (RenderVolume_guess *)EQ_CDECL(void *, 0x1001D516, (void *, int *))(context, desc);
            if (volume == 0)
            {
                result = 0xffff3d41;
                goto done;
            }
            EQ_CDECL(void, 0x1001D5A1, (void *, void *))(context, actor->renderVolume_guess);
            actor->renderVolume_guess = volume;
            volume->renderActor = actor;
            scale = volume->sphylinderRadius_guess;
            break;
        default:
            result = 0xffff3d3e;
            goto done;
        }
        actor->scaleFactor_guess = scale;
    }
    actor->boundingRadius_guess = scale;
set_flags:
    actor->flags_guess[3] |= 0x20;
done:
    return result;
}

// FUNCTION: EQGFX 0x1000aaf7
// Gets the actor's base bounding radius.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image).
// __cdecl (the callee pops nothing; ECX is not an input). Returns the float at +0x38 of the actor as the x87 result (ST0).
float __cdecl t3dGetActorBoundingRadius(T3D_tagACTORINSTANCE *actor)
{
    return actor->boundingRadius_guess;
}

// FUNCTION: EQGFX 0x1000aaff
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). __cdecl; returns radius * scale as ST0.
float __cdecl t3dGetScaledActorBoundingRadius(T3D_tagACTORINSTANCE *actor)
{
    return actor->boundingRadius_guess * actor->scaleFactor_guess;
}

// FUNCTION: EQGFX 0x1000ab0a
// Sets an actor's bounding radius, optionally scaling its model to match.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image). Name from the Ghidra import, unconfirmed.

#include "trilogy/eqaddr.h"
#include <string.h>

// 0x10076284 the float the original compares against (eqgfx_dx7.dll's .rdata). Read through the address, like
// the original's FCOMP memory operand.
#define EQGFX_ZERO_F_GUESS          EQ_GLOBAL(float, 0x10076284)

// 0x10017dad / 0x10017ce8: this image's t3dUnshareObject / t3dShareObject (Ghidra import names), both __cdecl.
#define EQGFX_T3D_UNSHARE_OBJECT    EQ_CDECL(int, 0x10017DAD, (void *))
#define EQGFX_T3D_SHARE_OBJECT      EQ_CDECL(int, 0x10017CE8, (void *))

// 0x1000e334 t3dRemoveEntryFromDictionary (Ghidra import name), 0x10015d81 t3dFreeObjectPoolSlot_guess (no name yet, returns int),
// 0x10050cbb t3dFreeMemory (Ghidra import name): all __cdecl.
#define EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY  EQ_CDECL(void, 0x1000E334, (void *, void *))
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT                      EQ_CDECL(int, 0x10015D81, (void *, void *))
#define EQGFX_T3D_FREE_MEMORY                   EQ_CDECL(void, 0x10050CBB, (void *))

// 0x1004ef68 t3dFloatSqrt (Ghidra import name): __cdecl, one float argument on the stack, the root comes back in ST0.
#define EQGFX_T3D_FLOAT_SQRT                    EQ_CDECL(float, 0x1004EF68, (float))

// 0x1000a68e t3dGetActorSprite (Ghidra import name): __cdecl (actor, index, 0); the sprite comes back in EAX.
#define EQGFX_GET_ACTOR_SPRITE_GUESS              EQ_CDECL(ActorSprite_guess *, 0x1000A68E, (T3D_tagACTORINSTANCE *, unsigned int, int))

// 0x10007f3b, 0x10007bb1, 0x10007bc1, 0x100139b6, 0x100102ff: the sprite getters (Ghidra import names), all __cdecl,
// one dword argument, the float result in ST0.
#define EQGFX_T3D_GET_2D_SPRITE_DEF_BOUNDING_RADIUS  EQ_CDECL(float, 0x10007F3B, (unsigned int))
#define EQGFX_T3D_GET_2D_SPRITE_CONSTANT_INTENSITY   EQ_CDECL(float, 0x10007BB1, (unsigned int))
#define EQGFX_S3D_GET_DM_RGB_TRACK_PLAY_SPEED        EQ_CDECL(float, 0x10007BC1, (unsigned int))
#define EQGFX_T3D_GET_ACTOR_WORLD_Z                  EQ_CDECL(float, 0x100139B6, (unsigned int))
#define EQGFX_S3D_GET_DM_SPRITE_DEF_BOUNDING_RADIUS  EQ_CDECL(float, 0x100102FF, (unsigned int))

// 0x100682d0 _memset (library, found by analysis): __cdecl (destination, value, size).
#define EQGFX_MEMSET                            EQ_CDECL(void *, 0x100682D0, (void *, int, unsigned int))

// __cdecl (the callee pops nothing; ECX is not an input). When the flag is nonzero, scales the radius at +0x34 by
// radius / old +0x38 (or takes radius as is when old +0x38 is 0) and sets 0x20 in the byte at +0xf. Always stores
// radius at +0x38. Returns 0.
int t3dSetActorBoundingRadius_guess(char *actor, float radius, int flag)
{
    if (flag != 0)
    {
        if (((T3D_tagACTORINSTANCE*)actor)->boundingRadius_guess == EQGFX_ZERO_F_GUESS)
            ((T3D_tagACTORINSTANCE*)actor)->scaleFactor_guess = radius;
        else
            ((T3D_tagACTORINSTANCE*)actor)->scaleFactor_guess = (radius / ((T3D_tagACTORINSTANCE*)actor)->boundingRadius_guess) * ((T3D_tagACTORINSTANCE*)actor)->scaleFactor_guess;
        ((T3D_tagACTORINSTANCE*)actor)->flags_guess[3] |= 0x20;
    }
    ((T3D_tagACTORINSTANCE*)actor)->boundingRadius_guess = radius;
    return 0;
}

typedef struct Ai_guess
{
    unsigned int  type;                    // 0x00  0x1a once t3dDefineAI has defined the AI
    unsigned char unused_04[0x04];
    void         *dictionaryKey;           // 0x08  removed from the dictionary when flags bit 0 is set, freed when nonzero
    unsigned int  flags;                   // 0x0c  bit 0 (0x01): the key is registered in the dictionary
    int           value;                   // 0x10  the value t3dDefineAI was given
} Ai_guess;

// 0x1000e7fe t3dGetPointerFromDictionary, 0x1001564b _t3dAddItemToList_guess, 0x1000dc08 t3dAddEntryToDictionary, 0x10050c48
// t3dAllocateMemory: all __cdecl, typed for an Ai_guess (the BMInfo_guess and void * macros above don't fit).
#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY   EQ_CDECL(Ai_guess *, 0x1000E7FE, (void *, char *))
#define EQGFX_ALLOCATE_AI                       EQ_CDECL(Ai_guess *, 0x10050C48, (unsigned int))
#define EQGFX_ADD_AI_TO_LIST                    EQ_CDECL(int, 0x1001564B, (void *, Ai_guess *))
#define EQGFX_ADD_AI_TO_DICTIONARY              EQ_CDECL(int, 0x1000DC08, (void *, Ai_guess *))

// FUNCTION: EQGFX 0x1000ab41
// Gets the actor's model scale factor.
// eqgfx_dx7.dll (no ORIGINAL FILE: the Mac debug map doesn't cover this image).
// __cdecl (the callee pops nothing; ECX is not an input). Returns the scale factor at +0x34 of the actor as the x87 result (ST0).
float __cdecl t3dGetActorScaleFactor(T3D_tagACTORINSTANCE *actor)
{
    return actor->scaleFactor_guess;
}

// FUNCTION: EQGFX 0x1000ab49
// Changes an actor's scale, optionally adjusting its collision shape and bounding radius.
// __cdecl (the callee pops nothing; ECX is not an input). Returns 0xffff3d3e, changing nothing, when scale is 0 or less; else 0.
// Stores scale at +0x34. With scaleOnly 0 it first rescales the collision shape (+0x2c) by old scale / scale and sets +0x38 from the sprite.
int __cdecl t3dSetActorScaleFactor(T3D_tagACTORINSTANCE *actor, float scale, int scaleOnly)
{
    float scaleRatio;
    float oldFactor;
    float spriteRadius;
    float *extent;
    float zeroExtent[3];
    BoundingShape_guess *shape;
    ActorSprite_guess *sprite;

    if (scale <= EQGFX_ZERO_F_GUESS)
        return 0xffff3d3e;
    if (scaleOnly == 0)
    {
        shape = (BoundingShape_guess *)actor->collisionVolume_guess;
        scaleRatio = actor->scaleFactor_guess / scale;
        switch (shape->shapeType)
        {
        case 2:
        case 4:
            oldFactor = shape->factor;
            shape->factor = scaleRatio * oldFactor;
            scaleRatio = scaleRatio * oldFactor * ((ShapeScale_guess *)shape->scale)->scaleValue;
            break;
        case 0x2a:
            scaleRatio = scaleRatio * shape->vector[1];
            shape->vector[1] = scaleRatio;
            break;
        case 0x2b:
            shape->radiusAdd = scaleRatio * shape->radiusAdd;
            shape->vector[0] = scaleRatio * shape->vector[0];
            shape->vector[1] = scaleRatio * shape->vector[1];
            shape->vector[2] = scaleRatio * shape->vector[2];
            scaleRatio = 0.0f;
            break;
        default:
            scaleRatio = 0.0f;
            break;
        }
        sprite = EQGFX_GET_ACTOR_SPRITE_GUESS(actor, actor->currentAction_guess, 0);
        switch (sprite->spriteType)
        {
        case 8:
        case 0x51:
            spriteRadius = 0.0f;
            EQGFX_MEMSET(zeroExtent, 0, 12);
            extent = zeroExtent;
            break;
        case 0xa:
            spriteRadius = EQGFX_T3D_GET_2D_SPRITE_DEF_BOUNDING_RADIUS(sprite->unknown0x88);
            extent = (float *)&sprite->unknown0x78;
            break;
        case 0xc:
            spriteRadius = EQGFX_S3D_GET_DM_RGB_TRACK_PLAY_SPEED(sprite->unknown0x2c);
            extent = (float *)&sprite->unknown0x1c;
            break;
        case 0x12:
            spriteRadius = EQGFX_T3D_GET_2D_SPRITE_CONSTANT_INTENSITY(sprite->unknown0x38);
            extent = (float *)&sprite->unknown0x2c;
            break;
        case 0x14:
            spriteRadius = EQGFX_T3D_GET_ACTOR_WORLD_Z(sprite->unknown0x5c);
            extent = (float *)&sprite->unknown0x28;
            break;
        case 0x48:
            spriteRadius = EQGFX_S3D_GET_DM_SPRITE_DEF_BOUNDING_RADIUS(sprite->unknown0x34);
            extent = (float *)&sprite->unknown0x14;
            break;
        default:
            spriteRadius = 0.0f;
            /* the original leaves the scale's bits in the pointer here; nothing reads extent on this path */
            extent = (float *)*(int *)&scale;
            break;
        }
        actor->boundingRadius_guess = (spriteRadius > scaleRatio) ? spriteRadius : scaleRatio;
        if (spriteRadius > EQGFX_ZERO_F_GUESS)
        {
            actor->boundingRadius_guess = EQGFX_T3D_FLOAT_SQRT(extent[0] * extent[0] + extent[1] * extent[1] + extent[2] * extent[2]) + actor->boundingRadius_guess;
        }
    }
    actor->flags_guess[3] |= 0x20;
    actor->scaleFactor_guess = scale;
    return 0;
}

// FUNCTION: EQGFX 0x1000acc3
// __cdecl. Returns the definition pointer the actor holds at +0x64 (no include/ struct models the type).
void *__cdecl t3dGetActorDefinition(T3D_tagACTORINSTANCE *actor)
{
    return actor->definition_guess;
}

// FUNCTION: EQGFX 0x1000accb
// __cdecl (the callee pops nothing; ECX is not an input). Returns bit 0 of the definition's flags at +0x0c (1 or 0).
unsigned int __cdecl t3dIs2DSpriteDefinitionInDictionary(SpriteDefinition_guess *spriteDefinition)
{
    return spriteDefinition->flags & 1;
}

// FUNCTION: EQGFX 0x1000acd6
// Reports how many deformations a DMRGB track definition uses per frame.
// __cdecl (the callee pops nothing; ECX is not an input). Returns the per-frame deformation count a
// DMRGBTrackDefinition holds at +0x14 (no include/ struct models the type; offset from the Ghidra name chain).
unsigned int __cdecl s3dGetDMRGBTrackDefinitionNumDeformationsPerFrame(char *trackDefinition)
{
    return *(unsigned int *)(trackDefinition + 0x14);
}

// FUNCTION: EQGFX 0x1000acde
// Gets an actor definition's value for one detail level; what the value means is unclear.
// __cdecl (callee pops nothing; ECX is not an input). Returns the dword at the start of entry levelIndex of the definition's
// 8-byte level array (pointer at +0x18), or 0 when levelIndex is not below the count at +0x14.
unsigned int __cdecl t3dGetActorDefinitionNumLevelsOfDetail(ActorDefinition_guess *definition, unsigned int levelIndex)
{
    if (levelIndex >= definition->levelCount)
    {
        return 0;
    }
    return *(unsigned int *)(definition->levels + levelIndex * 8);
}

// FUNCTION: EQGFX 0x1000acf5
// Checks whether an actor's AI is active.
// __cdecl (the callee pops nothing; ECX is not an input). 1 while the actor's AI is active (the dword at +0x5c
// is nonzero), else 0.
int __cdecl t3dIsActorAIActive(T3D_tagACTORINSTANCE *actor)
{
    return actor->activeAI_int_guess != 0;
}

// FUNCTION: EQGFX 0x1000ad02
// Gets the value object assigned to an actor's light.
// __cdecl (callee pops nothing; ECX is not an input). Returns the dword at +0x10 of the light object the actor points to
// at +0x54, or 0 when the actor has no light.
unsigned int __cdecl t3dGetActorLight(T3D_tagACTORINSTANCE *actor)
{
    ActorLight_guess *light;

    light = (ActorLight_guess *)actor->light_guess;
    if (light == 0)
        return 0;
    return light->lightValue;
}

// FUNCTION: EQGFX 0x1000ad12
// Sets or clears an actor's point light.
// __cdecl (callee pops nothing; ECX is not an input). Creates the actor's light for a nonzero value when it has none (0xffff3d3e if
// that fails), destroys an existing light for a zero value, otherwise stores the value and moves the reference counts from the old value to it.
int __cdecl t3dSetActorLight(void *context, T3D_tagACTORINSTANCE *actor, LightValue_guess *lightValue)
{
    int result;
    ActorLight_guess *light;
    LightValue_guess *oldValue;

    result = 0;
    light = (ActorLight_guess *)actor->light_guess;
    if (light != 0)
    {
        if (lightValue == 0)
        {
            EQ_CDECL(void, 0x100144DC, (void *, ActorLight_guess *))(context, light);
            actor->light_guess = 0;
        }
        else
        {
            oldValue = (LightValue_guess *)light->lightValue;
            oldValue->refCount--;
            light = (ActorLight_guess *)actor->light_guess;
            light->lightValue = (unsigned int)lightValue;
            lightValue->refCount++;
        }
    }
    else
    {
        if (lightValue != 0)
        {
            light = EQ_CDECL(ActorLight_guess *, 0x10014356, (void *, LightValue_guess *, void *, float, int))(
                context, lightValue, (unsigned char *)actor + 0x10, 0.0f, 1);
            actor->light_guess = light;
            if (light == 0)
                result = 0xffff3d3e;
            else
                *(unsigned int *)((unsigned char *)light + 0x30) = *(unsigned int *)((unsigned char *)actor + 0x04);
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1000ad84
// Gets the area of influence of an actor's light.
// __cdecl (callee pops nothing; ECX is not an input). Returns the float at +0x20 of the light object the actor points to at +0x54 as ST0, or 0.0 when the actor has no light.
float __cdecl t3dGetActorLightRadiusOfInfluence(T3D_tagACTORINSTANCE *actor)
{
    ActorLight_guess *light = (ActorLight_guess *)actor->light_guess;
    return (light == 0) ? 0 : (light->radiusOfInfluence);
}

// FUNCTION: EQGFX 0x1000ad96
// Changes the area illuminated by an actor's point light.
// __cdecl (callee pops nothing; ECX is not an input). Passes the first argument, the actor's light and the radius to t3dSetPointLightRadiusOfInfluence and returns its result; 0xffff3d3e without calling it when the actor has no light.
int __cdecl t3dSetActorLightRadiusOfInfluence(void *context, T3D_tagACTORINSTANCE *actor, float radiusOfInfluence)
{
    ActorLight_guess *light;

    light = (ActorLight_guess *)actor->light_guess;
    if (light == 0)
        return 0xffff3d3e;
    return EQ_CDECL(int, 0x100145F5, (void *, ActorLight_guess *, float))(context, light, radiusOfInfluence);
}

// FUNCTION: EQGFX 0x1000adbd
// Hides or shows an actor.
// __cdecl (callee pops nothing; ECX is not an input). Sets bit 0x40 of the flags byte at +0x0f when invisible is nonzero, clears it otherwise. Returns 0.
int __cdecl t3dSetActorInvisible(T3D_tagACTORINSTANCE *actor, int invisible)
{
    if (invisible != 0)
        actor->flags_guess[3] |= 0x40;
    else
        actor->flags_guess[3] &= 0xbf;
    return 0;
}

// FUNCTION: EQGFX 0x1000add5
// Bounding radius of an actor definition: farthest reach of its render volumes, level shapes and root shape.
// __cdecl (callee pops nothing; ECX is not an input). Stores the largest radius of the definition's render volumes, level
// shapes and root shape at *definitionRadius and returns 0; 0xffff3d3e for an unknown volume or shape type (nothing stored).
int __cdecl t3dComputeActorDefinitionBoundingRadius(ActorDefinition_guess *definition, float *definitionRadius)
{
    float maxRadius;
    unsigned int index;
    ShapeSet_guess *shapeSet;
    unsigned int levelCount;
    unsigned int volumeCount;
    RenderVolume_guess *volume;
    BoundingShape_guess *shape;
    void **levelEntry;
    unsigned char *levelPart;
    unsigned char *child;
    unsigned char *subChild;
    float *vector;
    float radiusOffset;
    float shapeRadius;
    float lengthSquared;
    float radius;

    index = 0;
    volumeCount = *(unsigned int *)((unsigned char *)definition + 0x1c);
    shapeSet = *(ShapeSet_guess **)((unsigned char *)definition + 0x28);
    maxRadius = 0.0f;
    for (; index < volumeCount; index++)
    {
        volume = shapeSet->volumes[index];
        switch (volume->volumeType)
        {
        case 0x12:
            vector = (float *)((unsigned char *)volume + 0x2c);
            child = *(unsigned char **)((unsigned char *)volume + 0x38);
            radiusOffset = *(float *)(child + 0x20);
            break;
        case 0xa:
            vector = (float *)((unsigned char *)volume + 0x78);
            child = *(unsigned char **)((unsigned char *)volume + 0x88);
            radiusOffset = *(float *)(child + 0x2c);
            break;
        case 0xc:
            vector = (float *)((unsigned char *)volume + 0x1c);
            child = *(unsigned char **)((unsigned char *)volume + 0x2c);
            radiusOffset = *(float *)(child + 0x28);
            break;
        case 0x14:
            vector = (float *)((unsigned char *)volume + 0x28);
            child = *(unsigned char **)((unsigned char *)volume + 0x5c);
            radiusOffset = *(float *)(child + 0x18);
            break;
        case 0x48:
            vector = (float *)((unsigned char *)volume + 0x14);
            child = *(unsigned char **)((unsigned char *)volume + 0x34);
            subChild = *(unsigned char **)(child + 0x14);
            if (subChild != 0)
                radiusOffset = *(float *)(subChild + 0x30);
            else
                radiusOffset = *(float *)(*(unsigned char **)(child + 0x18) + 0x58);
            break;
        case 0x0:
        case 0x8:
        case 0x4c:
        case 0x51:
            continue;
        default:
            return 0xffff3d3e;
        }
        if (vector != 0)
        {
            lengthSquared = vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
            if (lengthSquared == EQGFX_ZERO_F_GUESS)
                radius = radiusOffset;
            else
                radius = EQGFX_T3D_FLOAT_SQRT(lengthSquared) + radiusOffset;
            if (maxRadius < radius)
                maxRadius = radius;
        }
    }

    levelCount = definition->levelCount;
    index = 0;
    for (levelEntry = (void **)(definition->levels + 4); index < levelCount; index++, levelEntry += 2)
    {
        levelPart = (unsigned char *)*levelEntry;
        volume = shapeSet->volumes[*(unsigned int *)(levelPart + 8)];
        switch (volume->volumeType)
        {
        case 0x12:
            vector = (float *)((unsigned char *)volume + 0x2c);
            shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
            break;
        case 0xa:
            vector = (float *)((unsigned char *)volume + 0x78);
            shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
            break;
        case 0xc:
            vector = (float *)((unsigned char *)volume + 0x1c);
            shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
            break;
        case 0x14:
            vector = (float *)((unsigned char *)volume + 0x28);
            shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
            break;
        case 0x48:
            vector = (float *)((unsigned char *)volume + 0x14);
            shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x2c);
            break;
        case 0x0:
        case 0x8:
        case 0x4c:
        case 0x51:
            continue;
        default:
            return 0xffff3d3e;
        }
        if (shape != 0)
        {
            switch (shape->shapeType)
            {
            case 0:
                shapeRadius = 0.0f;
                break;
            case 2:
            case 4:
                shapeRadius = *(float *)((unsigned char *)shape->scale + 0x14) * shape->factor;
                break;
            case 0x2a:
                shapeRadius = shape->vector[1];
                break;
            case 0x2b:
                shapeRadius = EQGFX_T3D_FLOAT_SQRT(shape->vector[0] * shape->vector[0] + shape->vector[1] * shape->vector[1]
                                                   + shape->vector[2] * shape->vector[2]) + shape->radiusAdd;
                break;
            case 0x52:
            case 0x54:
                continue;
            default:
                return 0xffff3d3e;
            }
            if (EQGFX_ZERO_F_GUESS < shapeRadius)
            {
                lengthSquared = vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
                if (lengthSquared == EQGFX_ZERO_F_GUESS)
                    radius = shapeRadius;
                else
                    radius = EQGFX_T3D_FLOAT_SQRT(lengthSquared) + shapeRadius;
                if (maxRadius < radius)
                    maxRadius = radius;
            }
        }
    }

    shape = shapeSet->root;
    if (shape != 0)
    {
        switch (shape->shapeType)
        {
        case 0:
        case 0x2e:
        case 0x52:
        case 0x54:
            shapeRadius = 0.0f;
            break;
        case 2:
        case 4:
            shapeRadius = *(float *)((unsigned char *)shape->scale + 0x14) * shape->factor;
            break;
        case 0x2a:
            shapeRadius = shape->vector[1];
            break;
        case 0x2b:
            shapeRadius = EQGFX_T3D_FLOAT_SQRT(shape->vector[0] * shape->vector[0] + shape->vector[1] * shape->vector[1]
                                               + shape->vector[2] * shape->vector[2]) + shape->radiusAdd;
            break;
        default:
            return 0xffff3d3e;
        }
        if (maxRadius < shapeRadius)
            maxRadius = shapeRadius;
    }
    *definitionRadius = maxRadius;
    return 0;
}

// FUNCTION: EQGFX 0x1000b0ed
// Bounding radius of one placed actor from its own render volumes and collision volume, not its definition's.
// __cdecl (callee pops nothing; ECX not an input). Writes the largest radius of the actor's volumes and their shapes to *instanceRadius. Returns 0, or 0xffff3d3e for an unknown type (nothing written).
int __cdecl t3dComputeActorInstanceBoundingRadius(T3D_tagACTORINSTANCE *actor, float *instanceRadius)
{
    ActorDefinition_guess *definition;
    RenderVolume_guess *volume;
    BoundingShape_guess *shape;
    unsigned char *subObject;
    void **levelEntry;
    float *vector;
    unsigned int volumeCount;
    unsigned int levelCount;
    unsigned int index;
    float radiusOffset;
    float maxRadius;

    index = 0;
    definition = (ActorDefinition_guess *)actor->definition_guess;
    volumeCount = *(unsigned int *)((unsigned char *)definition + 0x1c);
    maxRadius = 0.0f;
    if (volumeCount != 0)
    {
        do
        {
            volume = (RenderVolume_guess *)(*(void ***)((unsigned char *)actor + 0x44))[index];
            switch (volume->volumeType)
            {
            case 0x12:
                vector = (float *)((unsigned char *)volume + 0x2c);
                subObject = *(unsigned char **)((unsigned char *)volume + 0x38);
                radiusOffset = *(float *)(subObject + 0x20);
                break;
            case 0xa:
                vector = (float *)((unsigned char *)volume + 0x78);
                subObject = *(unsigned char **)((unsigned char *)volume + 0x88);
                radiusOffset = *(float *)(subObject + 0x2c);
                break;
            case 0xc:
                vector = (float *)((unsigned char *)volume + 0x1c);
                subObject = *(unsigned char **)((unsigned char *)volume + 0x2c);
                radiusOffset = *(float *)(subObject + 0x28);
                break;
            case 0x14:
                vector = (float *)((unsigned char *)volume + 0x28);
                subObject = *(unsigned char **)((unsigned char *)volume + 0x5c);
                radiusOffset = *(float *)(subObject + 0x18);
                break;
            case 0x48:
                vector = (float *)((unsigned char *)volume + 0x14);
                subObject = *(unsigned char **)((unsigned char *)volume + 0x34);
                if (*(unsigned char **)(subObject + 0x14) == 0)
                    radiusOffset = *(float *)(*(unsigned char **)(subObject + 0x18) + 0x58);
                else
                    radiusOffset = *(float *)(*(unsigned char **)(subObject + 0x14) + 0x30);
                break;
            case 0:
            case 8:
            case 0x4d:
            case 0x51:
                goto nextVolume;
            default:
                return 0xffff3d3e;
            }
            if (vector != 0)
            {
                if (vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2] != EQGFX_ZERO_F_GUESS)
                    radiusOffset = EQGFX_T3D_FLOAT_SQRT(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]) + radiusOffset;
                if (maxRadius < radiusOffset)
                    maxRadius = radiusOffset;
            }
nextVolume:
            index++;
        } while (index < volumeCount);
    }
    levelCount = definition->levelCount;
    index = 0;
    if (levelCount != 0)
    {
        levelEntry = (void **)(definition->levels + 4);
        do
        {
            volume = (RenderVolume_guess *)(*(void ***)((unsigned char *)actor + 0x44))[*(unsigned int *)((unsigned char *)*levelEntry + 8)];
            switch (volume->volumeType)
            {
            case 0x12:
                vector = (float *)((unsigned char *)volume + 0x2c);
                shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
                break;
            case 0xa:
                vector = (float *)((unsigned char *)volume + 0x78);
                shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
                break;
            case 0xc:
                vector = (float *)((unsigned char *)volume + 0x1c);
                shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
                break;
            case 0x14:
                vector = (float *)((unsigned char *)volume + 0x28);
                shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x14);
                break;
            case 0x48:
                vector = (float *)((unsigned char *)volume + 0x14);
                shape = *(BoundingShape_guess **)((unsigned char *)volume + 0x2c);
                break;
            case 0:
            case 8:
            case 0x4d:
            case 0x51:
                goto nextLevel;
            default:
                return 0xffff3d3e;
            }
            if (shape != 0)
            {
                switch (shape->shapeType)
                {
                case 0:
                    radiusOffset = 0.0f;
                    break;
                case 2:
                case 4:
                    radiusOffset = *(float *)((unsigned char *)shape->scale + 0x14) * shape->factor;
                    break;
                case 0x2a:
                    radiusOffset = shape->vector[1];
                    break;
                case 0x2b:
                    radiusOffset = EQGFX_T3D_FLOAT_SQRT(shape->vector[0] * shape->vector[0] + shape->vector[1] * shape->vector[1] + shape->vector[2] * shape->vector[2]) + shape->radiusAdd;
                    break;
                case 0x52:
                case 0x54:
                    goto nextLevel;
                default:
                    return 0xffff3d3e;
                }
                if (EQGFX_ZERO_F_GUESS < radiusOffset)
                {
                    if (vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2] != EQGFX_ZERO_F_GUESS)
                        radiusOffset = EQGFX_T3D_FLOAT_SQRT(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]) + radiusOffset;
                    if (maxRadius < radiusOffset)
                        maxRadius = radiusOffset;
                }
            }
nextLevel:
            index++;
            levelEntry += 2;
        } while (index < levelCount);
    }
    shape = *(BoundingShape_guess **)((unsigned char *)actor + 0x2c);
    if (shape != 0)
    {
        switch (shape->shapeType)
        {
        case 0:
        case 0x2e:
        case 0x52:
        case 0x54:
            radiusOffset = 0.0f;
            break;
        case 2:
        case 4:
            radiusOffset = *(float *)((unsigned char *)shape->scale + 0x14) * shape->factor;
            break;
        case 0x2a:
            radiusOffset = shape->vector[1];
            break;
        case 0x2b:
            radiusOffset = EQGFX_T3D_FLOAT_SQRT(shape->vector[0] * shape->vector[0] + shape->vector[1] * shape->vector[1] + shape->vector[2] * shape->vector[2]) + shape->radiusAdd;
            break;
        default:
            return 0xffff3d3e;
        }
        if (maxRadius < radiusOffset)
            maxRadius = radiusOffset;
    }
    *instanceRadius = maxRadius;
    return 0;
}

// FUNCTION: EQGFX 0x1000b402
// Sets the actor's DMRGB color animation track.
// __cdecl (callee pops nothing; ECX not an input). Unshares the old track at +0xfc if set, stores track, shares track if set. Returns 0.
int __cdecl t3dSetActorDMRGBTrack(T3D_tagACTORINSTANCE *actor, void *track)
{
    void **trackSlot;
    void *oldTrack;

    trackSlot = &actor->dmRgbTrack;
    oldTrack = *trackSlot;
    if (oldTrack != 0)
        EQGFX_T3D_UNSHARE_OBJECT(oldTrack);
    *trackSlot = track;
    if (track != 0)
        EQGFX_T3D_SHARE_OBJECT(track);
    return 0;
}

// FUNCTION: EQGFX 0x1000b433
// __cdecl. Stores the pitch type at +0x10c when it is below 4 and returns 0; otherwise returns 0xffff3d3e.
int __cdecl t3dSetActorSurfacePitchType(T3D_tagACTORINSTANCE *actor, unsigned int pitchType)
{
    if (pitchType <= 3)
    {
        actor->surfacePitchType = pitchType;
        return 0;
    }
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x1000b44f
// Defines or reuses a named AI resource; the supplied value's purpose is unclear.
// __cdecl. Returns the AI already under name when its type is 0x1a (0 for another type); else defines a new AI with value at +0x10 and returns it (0 on failure).
Ai_guess *__cdecl t3dDefineAI(void *context, char *name, int value)
{
    Ai_guess *ai;
    char *nameCopy;
    int status;

    ai = EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(context, name);
    if (ai != NULL)
    {
        if (ai->type == 0x1a)
            return ai;
    }
    else
    {
        ai = EQGFX_ALLOCATE_AI(0x14);
        if (ai == NULL)
            return ai;
        ai->flags = 0;
        ai->type = 0x1a;
        nameCopy = EQGFX_ALLOCATE_MEMORY(EQGFX_STRING_LENGTH(name) + 1);
        ai->dictionaryKey = nameCopy;
        if (nameCopy == NULL)
        {
            EQGFX_T3D_FREE_MEMORY(ai);
        }
        else
        {
            EQGFX_COPY_STRING(nameCopy, name);
            ai->value = value;
            status = EQGFX_ADD_AI_TO_LIST(context, ai);
            if (status != 0)
            {
                EQGFX_T3D_FREE_MEMORY(ai->dictionaryKey);
                EQGFX_T3D_FREE_MEMORY(ai);
            }
            else
            {
                status = EQGFX_ADD_AI_TO_DICTIONARY(context, ai);
                if (status == 0)
                    return ai;
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, ai);
                EQGFX_T3D_FREE_MEMORY(ai->dictionaryKey);
                EQGFX_T3D_FREE_MEMORY(ai);
            }
        }
    }
    ai = NULL;
    return ai;
}

// FUNCTION: EQGFX 0x1000b50e
// Unregisters an AI definition and releases it.
// __cdecl. Removes the dictionary key (flags bit 0), runs t3dFreeObjectPoolSlot_guess, frees the key if set, frees the AI block; returns t3dFreeObjectPoolSlot_guess's result.
int __cdecl t3dUndefineAI(void *context, Ai_guess *ai)
{
    int cleanupResult;

    if ((ai->flags & 0x01) != 0)
        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(context, ai->dictionaryKey);
    cleanupResult = EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, ai);
    if (ai->dictionaryKey != 0)
        EQGFX_T3D_FREE_MEMORY(ai->dictionaryKey);
    EQGFX_T3D_FREE_MEMORY(ai);
    return cleanupResult;
}

// FUNCTION: EQGFX 0x1000b550
// Defines a mipmapped bitmap resource using one name.
// __cdecl. Defines a BMInfo for one name: t3dDefineBMInfoMIPMapped with no existing block and a one-entry name list. Returns its result.
int __cdecl t3dDefineBMInfo(int context, char *name, char *singleName, int preloaded)
{
    return EQGFX_DEFINE_BMINFO_MIPMAPPED(context, 0, name, 1, &singleName, preloaded, 0);
}

// FUNCTION: EQGFX 0x1000b570
// Computes an allocation size from a 0x94-byte base, the given name strings and an optional extra name.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Returns the allocation size for a BMInfo block: 0
// when `flag` is zero. `count` == 1 takes the single-name size 0x95 + strlen(names[0]); otherwise
// 0x94 + 4 * count (the 4-byte pointers) plus strlen + 1 for each of the `count` names (unsigned countdown,
// so zero skips the loop: the pre-test is JBE). A nonzero `name` string adds its own strlen + 1 at the end.
int __cdecl s3dCalcBMInfoAllocSize_guess(char *name, unsigned int count, char **names, int flag)
{
    unsigned int len;
    int size;

    size = 0;
    if (flag != 0)
    {
        if (count == 1)
        {
            len = strlen(*names);
            size = len + 0x95;
        }
        else
        {
            size = count * 4 + 0x94;
            for (; count > 0; count--)
            {
                len = strlen(*names);
                names = names + 1;
                size = size + len + 1;
            }
        }
        if (name != 0)
        {
            len = strlen(name);
            size = size + len + 1;
        }
    }
    return size;
}

// FUNCTION: EQGFX 0x1000B5D6
// __cdecl (callee pops 0). Defines a MIP-mapped BMInfo (reusing `existing` when given); returns its slot address or 0.
int __cdecl t3dDefineBMInfoMIPMapped(int context, BMInfo_guess *existing, char *name, unsigned int count, char **names, int preloaded, int load_arg)
{
    BMInfo_guess *bm;
    char *free_space;
    unsigned int allocated_names;
    unsigned int index;

    allocated_names = 0;
    bm = EQGFX_GET_POINTER_FROM_DICTIONARY(context, name);
    if (bm != 0)
    {
        if (bm->type == BMINFO_TEXTURE_TYPE)
        {
            goto return_slot;
        }
        return 0;
    }
    if (count == 0)
    {
        goto release_partial;
    }
    if (g_render_mode_guess == 1)
    {
        count = 1;
    }
    if (existing != 0)
    {
        bm = existing;
    }
    else
    {
        bm = EQGFX_ALLOCATE_MEMORY(BMINFO_ALLOCATION_SIZE);
        if (bm == 0)
        {
            return 0;
        }
    }
    bm->type = BMINFO_TEXTURE_TYPE;
    bm->unused_2c = preloaded;
    bm->unused_0c = 0;
    bm->unused_10 = 0;
    bm->unused_24 = count - 1;
    bm->unused_34 = 0;
    bm->texture_surface = 0;
    bm->unused_38 = 0;
    bm->unused_64 = 0;
    bm->unused_68 = 0;
    bm->unused_6c = 0;
    bm->unused_84 = 0;
    bm->unused_70 = 0;
    bm->unused_74 = 0;
    bm->unused_78 = 0;
    bm->unused_7c = 0;
    bm->unused_80 = 0;
    bm->unused_18 = 0;
    bm->unused_14 = 0;
    bm->name = 0;
    bm->unused_30 = 0;
    if (existing != 0)
    {
        bm->unused_0c |= 0x20;
    }
    free_space = (char *)(bm + 1);
    if (existing == 0)
    {
        bm->name = EQGFX_ALLOCATE_MEMORY(EQGFX_STRING_LENGTH(name) + 1);
        if (bm->name == 0)
        {
            goto free_block;
        }
        EQGFX_COPY_STRING(bm->name, name);
    }
    else
    {
        if (name == 0)
        {
            bm->name = 0;
        }
        else
        {
            bm->name = free_space;
            free_space += EQGFX_STRING_LENGTH(name) + 1;
            EQGFX_COPY_STRING(bm->name, name);
        }
    }
    if (existing != 0)
    {
        bm->unused_30 = (unsigned int)free_space;
        if (count > 1)
        {
            free_space += count * 4;
            index = 0;
            if (count != 0)
            {
                do
                {
                    ((char **)bm->unused_30)[index] = free_space;
                    EQGFX_COPY_STRING(((char **)bm->unused_30)[index], names[index]);
                    free_space += EQGFX_STRING_LENGTH(names[index]) + 1;
                    index++;
                } while (index < count);
            }
        }
        else
        {
            EQGFX_COPY_STRING(free_space, names[0]);
            EQGFX_STRING_LENGTH(names[0]);
        }
    }
    else
    {
        if (count > 1)
        {
            bm->unused_30 = (unsigned int)EQGFX_ALLOCATE_MEMORY(count * 4);
            if (bm->unused_30 == 0)
            {
                goto release_all;
            }
            index = 0;
            if (count != 0)
            {
                do
                {
                    ((char **)bm->unused_30)[index] = EQGFX_ALLOCATE_MEMORY(EQGFX_STRING_LENGTH(names[index]) + 1);
                    if (((char **)bm->unused_30)[index] == 0)
                    {
                        goto release_all;
                    }
                    EQGFX_COPY_STRING(((char **)bm->unused_30)[index], names[index]);
                    index++;
                    allocated_names++;
                } while (index < count);
            }
        }
        else
        {
            bm->unused_30 = (unsigned int)EQGFX_ALLOCATE_MEMORY(EQGFX_STRING_LENGTH(names[0]) + 1);
            if (bm->unused_30 == 0)
            {
                goto release_all;
            }
            EQGFX_COPY_STRING((char *)bm->unused_30, names[0]);
        }
    }
    if (preloaded == 0)
    {
        if (EQGFX__T3D_RESOLVE_BITMAP_SEARCH_PATHS(context, bm) != 0)
        {
            goto release_partial;
        }
    }
    if (g_render_mode_guess != 0)
    {
        if (EQGFX_LOAD_BMINFO(context, bm, load_arg) != 0)
        {
            goto release_partial;
        }
    }
    if (EQGFX_ADD_TO_LIST(context, bm) != 0)
    {
        goto release_partial;
    }
    if (EQGFX_ADD_ENTRY_TO_DICTIONARY(context, bm) == 0)
    {
        goto return_slot;
    }
    EQGFX_REMOVE_FROM_LIST(context, bm);
release_partial:
    if (existing == 0)
    {
        goto release_all;
    }
    if (bm == 0)
    {
        return 0;
    }
    if (bm->unused_34 != 0)
    {
        EQGFX_FREE_MEMORY((void *)bm->unused_34);
    }
    if (bm->unused_14 != 0)
    {
        EQGFX_FREE_MEMORY((void *)bm->unused_14);
    }
    if (bm->texture_surface == 0)
    {
        return 0;
    }
    bm->texture_surface->vtable->release(bm->texture_surface);
    bm->texture_surface = 0;
    return 0;
release_all:
    if (bm == 0)
    {
        return 0;
    }
    if (bm->unused_24 != 0 && (bm->unused_0c & 0x80000000) == 0)
    {
        index = 0;
        do
        {
            EQGFX_FREE_MEMORY(((char **)bm->unused_30)[index]);
            index++;
        } while (index <= allocated_names);
    }
    if (bm->unused_30 != 0)
    {
        EQGFX_FREE_MEMORY((void *)bm->unused_30);
    }
    if (bm->name != 0)
    {
        EQGFX_FREE_MEMORY(bm->name);
    }
    if (bm->unused_34 != 0)
    {
        EQGFX_FREE_MEMORY((void *)bm->unused_34);
    }
    if (bm->unused_14 != 0)
    {
        EQGFX_FREE_MEMORY((void *)bm->unused_14);
    }
    if (bm->texture_surface != 0)
    {
        bm->texture_surface->vtable->release(bm->texture_surface);
        bm->texture_surface = 0;
    }
free_block:
    EQGFX_FREE_MEMORY(bm);
    return 0;
return_slot:
    return (int)(((BMInfoContext_guess *)context)->bitmap_info_slots + bm->slot_index);
}

// FUNCTION: EQGFX 0x1000b92f
// Defines or reuses a named texture with surface storage at the requested dimensions.
int __cdecl t3dDefineBMInfoWithTextureSurface(int context, char *texture_name, unsigned int width, unsigned int height)
{
    BMInfo_guess *bm_info;
    FILE *file;
    int status;
    int result;

    result = 0;
    bm_info = EQGFX_GET_POINTER_FROM_DICTIONARY(context, texture_name);
    if (bm_info == 0)
    {
        if (((height < width) && ((height << 3) < width)) ||
            ((width < height) && ((width << 3) < height)))
        {
            file = EQGFX_OPEN_FILE(GFX_TEXTURE_LOG_NAME, GFX_TEXTURE_LOG_MODE);
            if (file == 0)
            {
                return 0;
            }
            EQGFX_FPRINTF(file, EQGFX_INVALID_TEXTURE_FORMAT, EQGFX_LOG_STRING);
            EQGFX_FCLOSE(file);
            return 0;
        }
        bm_info = (BMInfo_guess *)EQGFX_ALLOCATE_MEMORY(BMINFO_ALLOCATION_SIZE);
        if (bm_info == 0)
        {
            return 0;
        }
        bm_info->type = BMINFO_TEXTURE_TYPE;
        bm_info->unused_0c = 0;
        bm_info->unused_10 = 0;
        bm_info->unused_2c = 0;
        bm_info->unused_34 = 0;
        bm_info->texture_surface = 0;
        bm_info->unused_38 = 0;
        bm_info->unused_64 = 0;
        bm_info->unused_68 = 0;
        bm_info->unused_6c = 0;
        bm_info->unused_84 = 0;
        bm_info->unused_70 = 0;
        bm_info->unused_74 = 0;
        bm_info->unused_78 = 0;
        bm_info->unused_7c = 0;
        bm_info->unused_80 = 0;
        bm_info->unused_18 = 0;
        bm_info->unused_14 = 0;
        bm_info->name = 0;
        bm_info->unused_30 = 0;
        bm_info->width = width;
        bm_info->height = height;
        bm_info->unused_24 = 0;
        bm_info->unused_28 = 0x10;
        bm_info->name = (char *)EQGFX_ALLOCATE_MEMORY((unsigned int)(EQGFX_STRING_LENGTH(texture_name) + 1));
        if (bm_info->name == 0)
        {
            EQGFX_FREE_MEMORY(bm_info);
            return 0;
        }
        EQGFX_COPY_STRING(bm_info->name, texture_name);
        status = EQGFX_DEFINE_BMINFO(bm_info, 1, 0);
        if (status == 0)
        {
            status = EQGFX_ADD_TO_LIST(context, bm_info);
            if (status == 0)
            {
                status = EQGFX_ADD_ENTRY_TO_DICTIONARY(context, bm_info);
                if (status == 0)
                {
                    goto bm_info_found;
                }
                EQGFX_REMOVE_FROM_LIST(context, bm_info);
            }
            bm_info->texture_surface->vtable->release(bm_info->texture_surface);
            bm_info->texture_surface = 0;
        }
        EQGFX_FREE_MEMORY(bm_info->name);
        EQGFX_FREE_MEMORY(bm_info);
    }
    else
    {
        if (bm_info->type == BMINFO_TEXTURE_TYPE)
        {
bm_info_found:
            result = (int)(((BMInfoContext_guess *)context)->bitmap_info_slots + bm_info->slot_index);
        }
        if (g_render_mode_guess == 2)
        {
            EQGFX_REFRESH_BMINFO(bm_info);
        }
    }
    return result;
}
