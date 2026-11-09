// unit_100142A1.c - eqgfx_dx7.dll, the address run 0x100142A1..0x10014F01 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

#include "trilogy/eqaddr.h"

typedef struct t3dRegionSet_guess
{
    int tag;
    unsigned char pad04[0x0c];
    int entryCount;
    unsigned int entryCapacity;
    unsigned int *entries;
} t3dRegionSet_guess;

typedef struct t3dRegionObject_guess
{
    unsigned char pad00[0x2c];
    t3dRegionSet_guess *regionSet;
} t3dRegionObject_guess;

typedef struct t3dWorld_guess
{
    unsigned char pad00[0x24];
    t3dRegionObject_guess **regions;
} t3dWorld_guess;

typedef struct t3dRegionIndex_guess
{
    unsigned char pad00[0x0f];
    unsigned char flags;
    unsigned char pad10[0x14];
    int regionCount;
    unsigned int regionCapacity;
    unsigned int *regionIndices;
} t3dRegionIndex_guess;

#define T3D_REGION_INDEX_POINT_LIGHT_FLAG 0x40

#define EQGFX_T3D_REALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C8C, (void *, int))
#define EQGFX_MEMCPY EQ_CDECL(void *, 0x100679F0, (void *, void *, unsigned int))

#define POINT_LIGHT_BOUND_FLAG          0x08
#define POINT_LIGHT_DICTIONARY_FLAG     0x01
#define DIRECTIONAL_LIGHT_DICTIONARY_FLAG 0x01
#define DIRECTIONAL_LIGHT_STATIC_FLAG   0x80
#define DIRECTIONAL_LIGHT_ANIMATION_SUSPENDED_FLAG 0x08
#define DIRECTIONAL_LIGHT_KIND          0x1F
#define DIRECTIONAL_LIGHT_ALLOC_SIZE    0x30
#define DIRECTIONAL_LIGHT_BLOCK_SIZE    0x0C
#define DIRECTIONAL_LIGHT_IN_USE_RESULT 0xFFFF3D3E
#define EQGFX_BOUND_POINT_LIGHT_COUNT   EQ_GLOBAL(int, 0x100AAD7C)
#define EQGFX_T3D_ALLOCATE_MEMORY       EQ_CDECL(void *, 0x10050C48, (int))
#define EQGFX_T3D_UNSHARE_OBJECT        EQ_CDECL(int, 0x10017DAD, (void *))
#define EQGFX_T3D_SHARE_OBJECT          EQ_CDECL(int, 0x10017CE8, (void *))
#define EQGFX__T3D_REMOVE_FROM_LIT_REGIONS              EQ_CDECL(void, 0x1000121B, (void *, void *))
#define EQGFX_TRAVERSE_LIGHT_INFLUENCE_BSP              EQ_CDECL(int, 0x1000129D, (void *, PointLight_guess *))
#define EQGFX__T3D_ADD_ITEM_TO_LIST              EQ_CDECL(int, 0x1001564B, (void *, DirectionalLight_guess *))
#define EQGFX_T3D_UNBIND_POINT_LIGHT    EQ_CDECL(int, 0x1001494D, (void *, PointLight_guess *))
#define EQGFX_T3D_UPDATE_BOUND_POINT_LIGHT EQ_CDECL(int, 0x10014995, (T3dContext_guess *, PointLight_guess *))
#define EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY EQ_CDECL(void, 0x1000E334, (void *, void *))
#define EQGFX_T3D_FREE_OBJECT_POOL_SLOT              EQ_CDECL(int, 0x10015D81, (void *, void *))
#define EQGFX_T3D_FREE_MEMORY           EQ_CDECL(void, 0x10050CBB, (void *))
#define EQGFX__t3dComputeObjectToWorldCoefficients              EQ_CDECL(void, 0x10035058, (void *, float, float *))
#define EQGFX_T3D_HIERARCHICAL_SPRITE_UPDATE_ALL_TRANSFORMATIONS EQ_CDECL(void, 0x100327C3, (void *, void *, void *))
#define EQGFX_T3D_SET_POINT_LIGHT_XYZ   EQ_CDECL(int, 0x100145BB, (T3dContext_guess *, PointLight_guess *, Vector3_guess *))
#define EQGFX_STRLEN                    EQ_CDECL(unsigned int, 0x10068160, (char *))
#define EQGFX_COPY_STRING_WITH_NULL_TERMINATOR EQ_CDECL(void, 0x10068070, (void *, char *))
#define EQGFX_T3D_GET_POINTER_FROM_DICTIONARY EQ_CDECL(void *, 0x1000E7FE, (void *, char *))
#define EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY EQ_CDECL(void, 0x1000DC08, (void *, PointLight_guess *))

// Light kind tag read by t3dIsLightValid (guessed: no layout in include/).
typedef struct Light_guess
{
    // 0x00: kind tag (0x1d = valid)
    int kind;
} Light_guess;

// Light and binding fields read by t3dUnbindPointLight (guessed: no layout in include/).
typedef struct PointLightBinding_guess
{
    // 0x10: object shared with the binding
    unsigned char pad00[0x10];
    void *sharedObject;
    unsigned char pad14[0x48];
    float transform[12];
} PointLightBinding_guess;

// Light owner: the light count drops by one for each light destroyed (guessed: no layout in include/).
typedef struct PointLightOwner_guess
{
    // 0x10: light count, 0x14: current frame, 0x24: light definition (returned by t3dGetPointLightLightDefinition)
    unsigned char pad00[0x10];
    int lightCount;
    int currentFrame;
    unsigned char pad18[0x0c];
    void *lightDefinition;
} PointLightOwner_guess;

// Light definition read by t3dGetPointLightCurrentIntensity (guessed: no layout in include/).
typedef struct LightDefinition_guess
{
    // 0x14: frame count, 0x18: intensity list (a single-frame definition copies its one float from here)
    unsigned char pad00[0x14];
    int frameCount;
    void *intensityList;
} LightDefinition_guess;

// Three floats, x/y/z (guessed: no layout in include/; same layout as DMSpriteVector3_guess).
typedef struct Vector3_guess
{
    float x;
    float y;
    float z;
} Vector3_guess;

typedef struct PointLight_guess
{
    // 0x00: kind tag (0x1e = valid point light)
    int kind;
    unsigned char pad04[0x04];
    // 0x08: dictionary key, 0x0c: dictionary flag byte (0x01 = entry to remove)
    void *dictionaryKey;
    unsigned char dictionaryFlags;
    unsigned char pad0d[0x02];
    // 0x0f: flag byte (0x08 = bound)
    unsigned char flags;
    // 0x10: owner, 0x2c: block freed first, 0x30: slot index in the context's table (-1 = none)
    PointLightOwner_guess *owner;
    // 0x14: position xyz (copied in by t3dSetPointLightXYZ)
    Vector3_guess position;
    // 0x20: radius of influence (set by t3dSetPointLightRadiusOfInfluence)
    float radiusOfInfluence;
    unsigned int field24;
    unsigned int field28;
    void *ownedBlock;
    int slotIndex;
    // 0x34: shared object, 0x38: bound object, 0x3c: binding
    void *sharedObject;
    void *boundObject;
    PointLightBinding_guess *binding;
} PointLight_guess;

typedef struct PointLightSharedObject_guess
{
    unsigned char pad00[0x10];
    float position[3];
    unsigned char pad1c[0x18];
    float field34;
    unsigned char pad38[0x54];
    float transform[9];
} PointLightSharedObject_guess;

typedef struct PointLightBoundTransform_guess
{
    unsigned char pad00[0x3c];
    Vector3_guess fallbackPosition;
    unsigned char pad48[0x20];
    Vector3_guess primaryPosition;
} PointLightBoundTransform_guess;

typedef struct PointLightBoundTransformSet_guess
{
    unsigned char pad00[0x14];
    PointLightBoundTransform_guess *primaryTransform;
    PointLightBoundTransform_guess *fallbackTransform;
} PointLightBoundTransformSet_guess;

typedef struct PointLightBoundObject_guess
{
    unsigned char pad00[0x14];
    Vector3_guess positionOffset;
    unsigned char pad20[0x14];
    PointLightBoundTransformSet_guess *transformSet;
} PointLightBoundObject_guess;

// DMSprite fields read by s3dGetDMSpriteCenterOffset (guessed: no layout in include/).
typedef struct DMSpriteVector3_guess
{
    // 0x00: x, 0x04: y, 0x08: z (guessed floats)
    float x;
    float y;
    float z;
} DMSpriteVector3_guess;

typedef struct DMSprite_guess
{
    unsigned char pad00[0x14];
    // 0x14: center offset (copied out by s3dGetDMSpriteCenterOffset)
    DMSpriteVector3_guess centerOffset;
} DMSprite_guess;

// Actor fields read by t3dIsActorInvisible (guessed: no layout in include/).
/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

// Slot of the context's point light table (guessed: no layout in include/).
typedef struct PointLightSlot_guess
{
    // 0x54: cleared when its light is destroyed (guessed: a reference to the light)
    unsigned char pad00[0x54];
    void *lightRef;
} PointLightSlot_guess;

// Renderer context, read only through its point light table (guessed: no layout in include/).
typedef struct T3dContext_guess
{
    // 0x17c: point light slot table, indexed by PointLight_guess.slotIndex
    unsigned char pad00[0x17c];
    PointLightSlot_guess **lightSlots;
} T3dContext_guess;

// Light owner of a directional light: the light count drops by one for each light destroyed (guessed: no layout in include/).
typedef struct DirectionalLightOwner_guess
{
    // 0x10: light count, 0x14: current frame, 0x24: light definition (returned by t3dGetDirectionalLightLightDefinition)
    unsigned char pad00[0x10];
    int lightCount;
    int currentFrame;
    unsigned char pad18[0x0c];
    void *lightDefinition;
} DirectionalLightOwner_guess;

// Direction vector copied by t3dGetDirectionalLightNormal (guessed: floats, no layout in include/).
typedef struct DirectionalLightNormal_guess
{
    // 0x00: x, 0x04: y, 0x08: z
    float x;
    float y;
    float z;
} DirectionalLightNormal_guess;

// Directional light fields read by t3dDestroyDirectionalLight and t3dSetDirectionalLightNormal (guessed: no layout in include/).
typedef struct DirectionalLight_guess
{
    // 0x00: kind tag (0x1f); 0x08: dictionary key; 0x0c: dictionary flag byte (bit 0 = entry to remove)
    int kind;
    unsigned char pad04[0x04];
    void *dictionaryKey;
    unsigned char dictionaryFlags;
    unsigned char pad0d[0x02];
    unsigned char staticFlags;
    // 0x10: use count (must be 0 to destroy), 0x14: owner, 0x18: normal, 0x2c: block freed first
    unsigned int useCount;
    DirectionalLightOwner_guess *owner;
    DirectionalLightNormal_guess normal;
    unsigned int field24;
    unsigned int field28;
    void *ownedBlock;
} DirectionalLight_guess;

// DM RGB track fields read by s3dGetDMRGBTrackNumCycles (guessed: no layout in include/).
typedef struct DMRGBTrack_guess
{
    // 0x20: cycle count
    unsigned char pad00[0x20];
    unsigned int numCycles;
} DMRGBTrack_guess;

// FUNCTION: EQGFX 0x100142A1
// Changes or clears the name used to look up a light in the world's resource dictionary.
// __cdecl, three stack arguments. Replaces the light's tag (dictionary key, +0x08) with a copy of tag, re-listing the light in the dictionary if it was listed. Returns 0xFFFF3D3E when the copy can't be allocated.
int __cdecl t3dSetLightTag(void *dictionary, PointLight_guess *light, char *tag)
{
    int result;
    void *tagCopy;
    unsigned int inDictionary;

    result = 0;
    if (tag != 0)
    {
        tagCopy = EQGFX_T3D_ALLOCATE_MEMORY(EQGFX_STRLEN(tag) + 1);
        if (tagCopy == 0)
        {
            result = 0xFFFF3D3E;
            goto done;
        }
    }
    else
    {
        tagCopy = tag;
    }
    inDictionary = light->dictionaryFlags & POINT_LIGHT_DICTIONARY_FLAG;
    if (inDictionary != 0)
    {
        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(dictionary, light->dictionaryKey);
    }
    if (light->dictionaryKey != 0)
    {
        EQGFX_T3D_FREE_MEMORY(light->dictionaryKey);
    }
    if (tag != 0)
    {
        light->dictionaryKey = tagCopy;
        EQGFX_COPY_STRING_WITH_NULL_TERMINATOR(tagCopy, tag);
        if (inDictionary != 0 && EQGFX_T3D_GET_POINTER_FROM_DICTIONARY(dictionary, tag) == 0)
        {
            EQGFX_T3D_ADD_ENTRY_TO_DICTIONARY(dictionary, light);
        }
    }
    else
    {
        light->dictionaryKey = 0;
    }
done:
    return result;
}

// FUNCTION: EQGFX 0x1001433D
// Checks whether a light object uses the accepted light type.
// __cdecl with one stack argument: light block holding the kind tag at +0x00. Returns 0 when the tag is 0x1d, else 0xFFFF3D3E.
int __cdecl t3dIsLightValid(Light_guess *light)
{
    int result;

    result = 0;
    if (light->kind != 0x1d)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x1001434E
// __cdecl, one stack argument (track). Returns the track's cycle count at +0x20.
unsigned int __cdecl s3dGetDMRGBTrackNumCycles(DMRGBTrack_guess *track)
{
    return track->numCycles;
}

// FUNCTION: EQGFX 0x10014356
// Creates a point light at a specified position with a chosen influence radius and light instance.
PointLight_guess *__cdecl t3dCreatePointLightEx(void *context, PointLightOwner_guess *owner, Vector3_guess *position, float radius, int bindImmediately)
{
    PointLight_guess *light;
    void *ownedBlock;
    int result;

    light = (PointLight_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x40);
    if (light != 0)
    {
        light->kind = 0x1e;
        light->dictionaryKey = 0;
        *((unsigned int *)&light->dictionaryFlags) = 0;
        light->owner = owner;
        light->position = *position;
        light->slotIndex = -1;
        light->radiusOfInfluence = radius;
        light->field24 = 0;
        light->field28 = 3;
        light->sharedObject = 0;
        light->boundObject = 0;
        light->binding = 0;
        ownedBlock = EQGFX_T3D_ALLOCATE_MEMORY(0x0c);
        light->ownedBlock = ownedBlock;
        if (ownedBlock == 0)
        {
            EQGFX_T3D_FREE_MEMORY(light);
        }
        else
        {
            result = EQGFX__T3D_ADD_ITEM_TO_LIST(context, (DirectionalLight_guess *)light);
            if (result == 0)
            {
                owner->lightCount++;
                if (bindImmediately != 1)
                {
                    return light;
                }
                result = EQGFX_TRAVERSE_LIGHT_INFLUENCE_BSP(context, light);
                if (result == 0)
                {
                    return light;
                }
                EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, light);
                EQGFX_T3D_FREE_MEMORY(ownedBlock);
                EQGFX_T3D_FREE_MEMORY(light);
            }
            else
            {
                EQGFX_T3D_FREE_MEMORY(ownedBlock);
                EQGFX_T3D_FREE_MEMORY(light);
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001441D
// Duplicates a point light at a new position while sharing the original light instance.
PointLight_guess *__cdecl t3dDuplicatePointLightEx(void *context, PointLight_guess *sourceLight, Vector3_guess *position, int bindImmediately)
{
    PointLight_guess *light;
    unsigned int *sourceWords;
    unsigned int *destinationWords;
    int copyCount;

    light = (PointLight_guess *)EQGFX_T3D_ALLOCATE_MEMORY(0x40);
    if (light != 0)
    {
        sourceWords = (unsigned int *)sourceLight;
        destinationWords = (unsigned int *)light;
        for (copyCount = 0x10; copyCount != 0; copyCount--)
        {
            *destinationWords = *sourceWords;
            sourceWords++;
            destinationWords++;
        }
        light->dictionaryKey = 0;
        *((unsigned int *)&light->dictionaryFlags) = 0;
        light->position = *position;
        light->slotIndex = -1;
        light->sharedObject = 0;
        light->boundObject = 0;
        light->binding = 0;
        light->field28 = 3;
        light->field24 = 0;
        light->ownedBlock = EQGFX_T3D_ALLOCATE_MEMORY(0x0c);
        if (light->ownedBlock == 0)
        {
            EQGFX_T3D_FREE_MEMORY(light);
        }
        else if (EQGFX__T3D_ADD_ITEM_TO_LIST(context, (DirectionalLight_guess *)light) == 0)
        {
            light->owner->lightCount++;
            if (bindImmediately != 1)
            {
                return light;
            }
            if (EQGFX_TRAVERSE_LIGHT_INFLUENCE_BSP(context, light) == 0)
            {
                return light;
            }
            EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, light);
            EQGFX_T3D_FREE_MEMORY(light->ownedBlock);
            EQGFX_T3D_FREE_MEMORY(light);
        }
        else
        {
            EQGFX_T3D_FREE_MEMORY(light->ownedBlock);
            EQGFX_T3D_FREE_MEMORY(light);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100144DC
// Removes a point light and its influence from the scene.
// __cdecl, two stack arguments (context, light). Unlinks and frees one point light, returns the result of t3dFreeObjectPoolSlot_guess.
int __cdecl t3dDestroyPointLight(T3dContext_guess *context, PointLight_guess *light)
{
    int result;

    if (light->slotIndex >= 0)
    {
        context->lightSlots[light->slotIndex]->lightRef = 0;
    }
    EQGFX__T3D_REMOVE_FROM_LIT_REGIONS(context, light);
    light->owner->lightCount--;
    EQGFX_T3D_UNBIND_POINT_LIGHT(context, light);
    if ((light->dictionaryFlags & POINT_LIGHT_DICTIONARY_FLAG) != 0)
    {
        EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(context, light->dictionaryKey);
    }
    result = EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, light);
    EQGFX_T3D_FREE_MEMORY(light->ownedBlock);
    if (light->dictionaryKey != 0)
    {
        EQGFX_T3D_FREE_MEMORY(light->dictionaryKey);
    }
    EQGFX_T3D_FREE_MEMORY(light);
    return result;
}

// FUNCTION: EQGFX 0x10014550
// Gets the current animation frame's intensity for a point light.
// __cdecl, one stack argument (the light). Returns the owner's intensity as an x87 float: a single-frame definition returns its one value, else the entry at the owner's current frame.
float __cdecl t3dGetPointLightCurrentIntensity(PointLight_guess *light)
{
    PointLightOwner_guess *owner = light->owner;
    LightDefinition_guess *definition = owner->lightDefinition;

    if (definition->frameCount == 1)
    {
        float intensity;

        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
            &intensity, &definition->intensityList, sizeof(intensity));
        return intensity;
    }
    return ((float *)definition->intensityList)[owner->currentFrame];
}

// FUNCTION: EQGFX 0x10014584
// Assigns a point light to a different light instance.
// __cdecl, two stack arguments (light, owner). Moves the light to the new owner: the old owner's light count drops by one, the new one's rises by one. Returns 0.
int __cdecl t3dSetPointLightLightInstance(PointLight_guess *light, PointLightOwner_guess *owner)
{
    PointLightOwner_guess *oldOwner;

    oldOwner = light->owner;
    oldOwner->lightCount = oldOwner->lightCount - 1;
    light->owner = owner;
    owner->lightCount = owner->lightCount + 1;
    return 0;
}

// FUNCTION: EQGFX 0x1001459B
// __cdecl, one stack argument (the light). Returns the owner's light definition: two hops, +0x10 then +0x24.
void *__cdecl t3dGetPointLightLightDefinition(PointLight_guess *light)
{
    return light->owner->lightDefinition;
}

// FUNCTION: EQGFX 0x100145A6
// Gets the center offset used to position a DM sprite.
// __cdecl, two pointer arguments (the callee pops nothing; ECX is not an input). Copies the DMSprite's center offset at +0x14 to offset and returns 0.
int __cdecl s3dGetDMSpriteCenterOffset(DMSprite_guess *dmSprite, DMSpriteVector3_guess *offset)
{
    *offset = dmSprite->centerOffset;
    return 0;
}

// FUNCTION: EQGFX 0x100145BB
// Updates a point light's position and the actors affected by its illumination.
// __cdecl, three stack arguments (the callee pops 0; ECX is not an input). Unless the flag dword at +0x0C has bit 31 set (returns 0xFFFF3D3E),
// copies xyz to the light's +0x14, then unless bit 30 is set calls traverseLightInfluenceBsp_guess and returns its EAX; otherwise returns 0.
int __cdecl t3dSetPointLightXYZ(T3dContext_guess *context, PointLight_guess *light, Vector3_guess *xyz)
{
    int result;
    unsigned int lightFlags;

    result = 0;
    lightFlags = *(unsigned int *)((unsigned char *)light + 0x0c);
    if ((lightFlags & 0x80000000) != 0)
    {
        return 0xFFFF3D3E;
    }
    light->position = *xyz;
    if ((lightFlags & 0x40000000) == 0)
    {
        result = EQGFX_TRAVERSE_LIGHT_INFLUENCE_BSP(context, light);
    }
    return result;
}

// FUNCTION: EQGFX 0x100145F5
// Changes a point light's reach and refreshes its affected actors.
// __cdecl, three stack arguments (the callee pops 0; ECX is not an input). Unless the flag dword at +0x0C has bit 31 set
// (returns 0xFFFF3D3E), stores the radius at +0x20, then unless bit 30 is set calls traverseLightInfluenceBsp_guess and returns its EAX; otherwise returns 0.
int __cdecl t3dSetPointLightRadiusOfInfluence(T3dContext_guess *context, PointLight_guess *light, float radius)
{
    int result;
    unsigned int lightFlags;

    result = 0;
    lightFlags = *(unsigned int *)((unsigned char *)light + 0x0c);
    if ((lightFlags & 0x80000000) != 0)
    {
        return 0xFFFF3D3E;
    }
    if ((lightFlags & 0x40000000) == 0)
    {
        light->radiusOfInfluence = radius;
        result = EQGFX_TRAVERSE_LIGHT_INFLUENCE_BSP(context, light);
    }
    return result;
}

// FUNCTION: EQGFX 0x10014628
// Marks a directional light as static.
// __cdecl, one pointer argument (the callee pops nothing; ECX is not an input). ORs bit 0x80 of the light's
// flag byte at +0xF (the "static" flag; the same byte t3dSetActorBoundingRadius sets 0x20 in) and returns 0.
// The light object has no layout in include/.
int __cdecl t3dSetDirectionalLightStaticFlag(unsigned char *light)
{
    light[0xf] |= 0x80;
    return 0;
}

// FUNCTION: EQGFX 0x10014633
// Reports whether an actor is invisible.
// __cdecl, one pointer argument (the callee pops nothing; ECX is not an input). Returns bit 30 of the actor's
// flags dword at +0x0C (1 or 0).
unsigned int __cdecl t3dIsActorInvisible(T3D_tagACTORINSTANCE *actor)
{
    return (actor->flagsDword_guess >> 30) & 1;
}

// FUNCTION: EQGFX 0x10014641
// Enables static-scene influence for a point light.
// __cdecl, one pointer argument. ORs bit 0x40 of the light's flag byte at +0xF (the point light's "static
// influence" bit, same byte as t3dSetDirectionalLightStaticFlag's 0x80) and returns 0. No layout in include/.
int __cdecl t3dSetPointLightStaticInfluenceFlag(unsigned char *light)
{
    light[0xf] |= 0x40;
    return 0;
}

// FUNCTION: EQGFX 0x1001464c
// Enables dynamic-scene influence for a directional light.
// __cdecl, one pointer argument. ORs bit 0x10 of the light's flag byte at +0xF (the directional light's
// "dynamic scene influence" bit, same byte as the static flags 0x80/0x40) and returns 0. No layout in include/.
int __cdecl t3dSetDirectionalLightDynamicSceneInfluenceFlag(unsigned char *light)
{
    light[0xf] |= 0x10;
    return 0;
}

// FUNCTION: EQGFX 0x10014657
// Enables static-scene influence for a directional light.
// __cdecl, one pointer argument. ORs 0x20 into the light's flag byte at +0xF (the "static scene influence" bit) and returns 0.
int __cdecl t3dSetDirectionalLightStaticSceneInfluenceFlag(unsigned char *light)
{
    light[0xf] |= 0x20;
    return 0;
}

// FUNCTION: EQGFX 0x10014662
// __cdecl, one pointer argument. Returns 0 when the light's kind tag at +0x0 is 0x1e, else 0xFFFF3D3E.
int __cdecl t3dIsPointLightValid(PointLight_guess *light)
{
    int result;

    result = 0;
    if (light->kind != 0x1e)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x10014673
// Adds a point light to the lighting assigned to a world region.
int __cdecl t3dAddPointLightToRegion(t3dWorld_guess *world, t3dRegionIndex_guess *regionIndices, unsigned int regionIndex)
{
    unsigned int insertionIndex;
    unsigned int existingRegionIndex;
    int upperIndex;
    int middleIndex;
    unsigned int capacity;
    void *resizedMemory;
    t3dRegionSet_guess *regionSet;
    int *regionSetEntries;
    unsigned int *insertionPoint;
    int result;

    insertionIndex = 0;
    result = 0;
    if ((regionIndices->flags & T3D_REGION_INDEX_POINT_LIGHT_FLAG) != 0)
    {
        upperIndex = regionIndices->regionCount - 1;
        if (upperIndex >= 0)
        {
            do
            {
                middleIndex = (int)(upperIndex + insertionIndex) >> 1;
                existingRegionIndex = regionIndices->regionIndices[middleIndex];
                if (existingRegionIndex == regionIndex)
                    return 0;
                if (regionIndex < existingRegionIndex)
                    upperIndex = middleIndex - 1;
                else
                    insertionIndex = middleIndex + 1;
            } while ((int)insertionIndex <= upperIndex);
        }

        capacity = regionIndices->regionCapacity;
        if (capacity <= (unsigned int)regionIndices->regionCount)
        {
            capacity = (capacity >> 2) + 1 + capacity;
            resizedMemory = EQGFX_T3D_REALLOCATE_MEMORY(regionIndices->regionIndices, capacity * 4);
            if (resizedMemory == 0)
                return 0xffff3d3e;
            regionIndices->regionIndices = (unsigned int *)resizedMemory;
            regionIndices->regionCapacity = capacity;
        }

        regionSet = world->regions[regionIndex]->regionSet;
        if (regionSet->tag == 0x27 || regionSet->tag == 0x29)
        {
            capacity = regionSet->entryCapacity;
            if (capacity <= (unsigned int)regionSet->entryCount)
            {
                capacity = (capacity >> 2) + 1 + capacity;
                resizedMemory = EQGFX_T3D_REALLOCATE_MEMORY(regionSet->entries, capacity * 4);
                if (resizedMemory == 0)
                    return 0xffff3d3e;
                regionSet->entries = (int *)resizedMemory;
                regionSet->entryCapacity = capacity;
            }
            regionSetEntries = regionSet->entries;
            regionSetEntries[regionSet->entryCount] = (int)regionIndices;
            regionSet->entryCount = regionSet->entryCount + 1;
            insertionPoint = regionIndices->regionIndices + insertionIndex;
            if (insertionIndex < (unsigned int)regionIndices->regionCount)
            {
                EQGFX_MEMCPY(insertionPoint + 1, insertionPoint,
                    (insertionIndex * 0x3fffffff + regionIndices->regionCount) * 4);
            }
            *insertionPoint = regionIndex;
            regionIndices->regionCount = regionIndices->regionCount + 1;
            return 0;
        }
    }
    return 0xffff3d3e;
}

// FUNCTION: EQGFX 0x10014771
// Removes a point light's lighting assignment from one world region.
int __cdecl t3dRemovePointLightFromRegion(t3dWorld_guess *world, int regionIndicesAddress, unsigned int regionIndex)
{
    t3dRegionIndex_guess *regionIndices;
    t3dRegionSet_guess *regionSet;
    unsigned int existingRegionIndex;
    unsigned int *regionIndexEntry;
    int upperIndex;
    int middleIndex;
    int *entry;
    int *entryEnd;

    regionIndices = (t3dRegionIndex_guess *)regionIndicesAddress;
    if ((regionIndices->flags & T3D_REGION_INDEX_POINT_LIGHT_FLAG) == 0)
        return 0xffff3d3e;

    regionIndicesAddress = 0;
    upperIndex = regionIndices->regionCount - 1;
    if (upperIndex >= 0)
    {
        do
        {
            middleIndex = (regionIndicesAddress + upperIndex) >> 1;
            existingRegionIndex = regionIndices->regionIndices[middleIndex];
            if (existingRegionIndex == regionIndex)
                break;
            if (existingRegionIndex > regionIndex)
                upperIndex = middleIndex - 1;
            else
                regionIndicesAddress = middleIndex + 1;
        } while (upperIndex >= regionIndicesAddress);

        if (upperIndex >= regionIndicesAddress)
        {
            regionIndexEntry = regionIndices->regionIndices + middleIndex;
            EQGFX_MEMCPY(regionIndexEntry, regionIndexEntry + 1,
                (middleIndex * 0x3fffffff + regionIndices->regionCount) * 4);
            regionIndices->regionCount--;

            regionSet = world->regions[regionIndex]->regionSet;
            if (regionSet->tag == 0x27 || regionSet->tag == 0x29)
            {
                entry = regionSet->entries;
                entryEnd = entry + regionSet->entryCount;
                for (; entry < entryEnd; entry++)
                {
                    if (*entry == (int)regionIndices)
                    {
                        if (entry != entryEnd - 1)
                        {
                            EQGFX_MEMCPY(entry, entry + 1,
                                (((int)(entryEnd - 1) - (int)entry) >> 2) << 2);
                        }
                        regionSet->entryCount--;
                        break;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10014839
// Makes a point light follow a DAG object through its binding.
// __cdecl, four arguments. Unbinds the light, then binds it to the DAG object (+0x34) and the binding (+0x3c), shares
// both objects, sets the bound flag (0x08), counts it and updates it. Returns 0xffff3d3e if either object is null, else 0.
int __cdecl t3dBindPointLightToDAG(T3dContext_guess *context, PointLight_guess *light, void *dagObject, PointLightBinding_guess *binding)
{
    int result;

    if (dagObject != 0 && binding != 0)
    {
        EQGFX_T3D_UNBIND_POINT_LIGHT(context, light);
        light->binding = binding;
        light->sharedObject = dagObject;
        EQGFX_T3D_SHARE_OBJECT(dagObject);
        EQGFX_T3D_SHARE_OBJECT(binding->sharedObject);
        light->flags |= POINT_LIGHT_BOUND_FLAG;
        EQGFX_BOUND_POINT_LIGHT_COUNT++;
        EQGFX_T3D_UPDATE_BOUND_POINT_LIGHT(context, light);
        result = 0;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x10014891
// Makes a point light follow a DM sprite and its bound object.
// __cdecl, four arguments. Unbinds the light, then binds it to the shared object (+0x34) and the bound
// object (+0x38), shares both objects, sets the bound flag (0x08), counts it and updates it. Returns 0xffff3d3e if either object is null, else 0.
int __cdecl t3dBindPointLightToDMSprite(T3dContext_guess *context, PointLight_guess *light, void *sharedObject, void *boundObject)
{
    int result;

    if (sharedObject != 0 && boundObject != 0)
    {
        EQGFX_T3D_UNBIND_POINT_LIGHT(context, light);
        light->sharedObject = sharedObject;
        light->boundObject = boundObject;
        EQGFX_T3D_SHARE_OBJECT(sharedObject);
        EQGFX_T3D_SHARE_OBJECT(boundObject);
        light->flags |= POINT_LIGHT_BOUND_FLAG;
        EQGFX_BOUND_POINT_LIGHT_COUNT++;
        EQGFX_T3D_UPDATE_BOUND_POINT_LIGHT(context, light);
        result = 0;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x100148E7
// Attaches a point light to a DM sprite using its DAG and bound object.
// __cdecl, five arguments. Unbinds the light, then binds it to the shared object (+0x34), the bound
// object (+0x38) and the binding (+0x3c), shares all three, sets the bound flag (0x08), counts it and updates it. Returns 0xffff3d3e if shared or bound object is null, else 0.
int __cdecl t3dBindPointLightToDAGDMSprite(T3dContext_guess *context, PointLight_guess *light, void *sharedObject, PointLightBinding_guess *binding, void *boundObject)
{
    int result;

    if (sharedObject != 0 && boundObject != 0)
    {
        EQGFX_T3D_UNBIND_POINT_LIGHT(context, light);
        light->binding = binding;
        light->boundObject = boundObject;
        light->sharedObject = sharedObject;
        EQGFX_T3D_SHARE_OBJECT(sharedObject);
        EQGFX_T3D_SHARE_OBJECT(boundObject);
        EQGFX_T3D_SHARE_OBJECT(binding->sharedObject);
        light->flags |= POINT_LIGHT_BOUND_FLAG;
        EQGFX_BOUND_POINT_LIGHT_COUNT++;
        EQGFX_T3D_UPDATE_BOUND_POINT_LIGHT(context, light);
        result = 0;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x1001494D
// Detaches a point light from its bound objects.
// __cdecl, two arguments (the first is unused). A bound light (flag 0x08): unshares +0x34, the binding at +0x3c
// (and its object) and +0x38 (clearing the last two), decrements the bound count, clears the flag. Returns 0.
int __cdecl t3dUnbindPointLight(int unused, PointLight_guess *light)
{
    if ((light->flags & POINT_LIGHT_BOUND_FLAG) != 0)
    {
        EQGFX_T3D_UNSHARE_OBJECT(light->sharedObject);
        if (light->binding != 0)
        {
            EQGFX_T3D_UNSHARE_OBJECT(light->binding->sharedObject);
            light->binding = 0;
        }
        if (light->boundObject != 0)
        {
            EQGFX_T3D_UNSHARE_OBJECT(light->boundObject);
            light->boundObject = 0;
        }
        EQGFX_BOUND_POINT_LIGHT_COUNT--;
        light->flags &= ~POINT_LIGHT_BOUND_FLAG;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10014995
// Moves an attached point light to follow its object's or animated hierarchy's current placement.
// @early-stop: 20 instruction diffs remain in branch load scheduling and one merged store.
/* fake match: split the input translation and assign xFromX first for VC6 x87 allocation. */
int __cdecl t3dUpdateBoundPointLight(T3dContext_guess *context, PointLight_guess *light)
{
    PointLightSharedObject_guess *sharedObject;
    PointLightBoundObject_guess *boundObject;
    float xFromX;
    float yFromX;
    float zFromX;
    float xFromY;
    float yFromY;
    float zFromY;
    float xFromZ;
    float yFromZ;
    float zFromZ;
    float baseX;
    float baseY;
    float baseZ;
    float xOffset;
    float yOffset;
    float zOffset;
    Vector3_guess position;

    sharedObject = (PointLightSharedObject_guess *)light->sharedObject;
    if (sharedObject != 0)
    {
        EQGFX__t3dComputeObjectToWorldCoefficients(sharedObject, sharedObject->field34, sharedObject->position);
    }
    if (light->binding == 0)
    {
        sharedObject = (PointLightSharedObject_guess *)light->sharedObject;
        xFromX = sharedObject->transform[0];
        yFromX = sharedObject->transform[1];
        zFromX = sharedObject->transform[2];
        xFromY = sharedObject->transform[3];
        yFromY = sharedObject->transform[4];
        zFromY = sharedObject->transform[5];
        xFromZ = sharedObject->transform[6];
        baseX = sharedObject->position[0];
        yFromZ = sharedObject->transform[7];
        baseZ = sharedObject->position[2];
        zFromZ = sharedObject->transform[8];
        baseY = sharedObject->position[1];
    }
    else
    {
        EQGFX_T3D_HIERARCHICAL_SPRITE_UPDATE_ALL_TRANSFORMATIONS(light->binding->sharedObject, context, light->sharedObject);
        xFromX = light->binding->transform[0];
        yFromX = light->binding->transform[1];
        zFromX = light->binding->transform[2];
        xFromY = light->binding->transform[3];
        yFromY = light->binding->transform[4];
        zFromY = light->binding->transform[5];
        xFromZ = light->binding->transform[6];
        baseX = light->binding->transform[9];
        yFromZ = light->binding->transform[7];
        baseZ = light->binding->transform[11];
        zFromZ = light->binding->transform[8];
        baseY = light->binding->transform[10];
    }
    boundObject = (PointLightBoundObject_guess *)light->boundObject;
    if (boundObject == 0)
    {
        xOffset = 0.0f;
        yOffset = 0.0f;
        zOffset = 0.0f;
    }
    else
    {
        if (boundObject->transformSet->primaryTransform != 0)
        {
            xOffset = boundObject->transformSet->primaryTransform->primaryPosition.x + boundObject->positionOffset.x;
            yOffset = boundObject->transformSet->primaryTransform->primaryPosition.y + boundObject->positionOffset.y;
            zOffset = boundObject->transformSet->primaryTransform->primaryPosition.z;
        }
        else
        {
            xOffset = boundObject->transformSet->fallbackTransform->fallbackPosition.x + boundObject->positionOffset.x;
            yOffset = boundObject->transformSet->fallbackTransform->fallbackPosition.y + boundObject->positionOffset.y;
            zOffset = boundObject->transformSet->fallbackTransform->fallbackPosition.z;
        }
        zOffset = zOffset + boundObject->positionOffset.z;
    }
    position.x = xOffset * xFromX + yOffset * xFromY + zOffset * xFromZ + baseX;
    position.y = xOffset * yFromX + yOffset * yFromY + zOffset * yFromZ + baseY;
    position.z = xOffset * zFromX + yOffset * zFromY + zOffset * zFromZ + baseZ;
    EQGFX_T3D_SET_POINT_LIGHT_XYZ(context, light, &position);
    return 0;
}

// FUNCTION: EQGFX 0x10014B20
// Creates a directional light with an initial orientation for a light instance.
DirectionalLight_guess *__cdecl t3dCreateDirectionalLight(T3dContext_guess *context, DirectionalLightOwner_guess *owner, DirectionalLightNormal_guess *normal)
{
    DirectionalLight_guess *light;
    int result;

    light = EQGFX_T3D_ALLOCATE_MEMORY(DIRECTIONAL_LIGHT_ALLOC_SIZE);
    if (light != 0)
    {
        light->kind = DIRECTIONAL_LIGHT_KIND;
        light->dictionaryKey = 0;
        *((unsigned int *)&light->dictionaryFlags) = 0;
        light->useCount = 0;
        light->owner = owner;
        light->normal = *normal;
        light->field28 = 3;
        light->field24 = 0;
        light->ownedBlock = EQGFX_T3D_ALLOCATE_MEMORY(DIRECTIONAL_LIGHT_BLOCK_SIZE);
        if (light->ownedBlock == 0)
        {
            EQGFX_T3D_FREE_MEMORY(light);
        }
        else
        {
            result = EQGFX__T3D_ADD_ITEM_TO_LIST(context, light);
            if (result == 0)
            {
                owner->lightCount++;
                return light;
            }
            EQGFX_T3D_FREE_MEMORY(light->ownedBlock);
            EQGFX_T3D_FREE_MEMORY(light);
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10014BA4
// Creates another directional light with the source's settings and owner.
// __cdecl. Copies the light, clears key/flags/use count, allocates a block, runs _t3dAddItemToList_guess; returns the copy or 0.
DirectionalLight_guess *__cdecl t3dDuplicateDirectionalLight(T3dContext_guess *context, DirectionalLight_guess *source)
{
    DirectionalLight_guess *duplicate;
    int result;

    duplicate = EQGFX_T3D_ALLOCATE_MEMORY(DIRECTIONAL_LIGHT_ALLOC_SIZE);
    if (duplicate != 0)
    {
        *duplicate = *source;
        duplicate->dictionaryKey = 0;
        *((unsigned int *)&duplicate->dictionaryFlags) = 0;
        duplicate->useCount = 0;
        duplicate->field28 = 3;
        duplicate->field24 = 0;
        duplicate->ownedBlock = EQGFX_T3D_ALLOCATE_MEMORY(DIRECTIONAL_LIGHT_BLOCK_SIZE);
        if (duplicate->ownedBlock == 0)
        {
            EQGFX_T3D_FREE_MEMORY(duplicate);
        }
        else
        {
            result = EQGFX__T3D_ADD_ITEM_TO_LIST(context, duplicate);
            if (result == 0)
            {
                duplicate->owner->lightCount++;
                return duplicate;
            }
            EQGFX_T3D_FREE_MEMORY(duplicate->ownedBlock);
            EQGFX_T3D_FREE_MEMORY(duplicate);
        }
        duplicate = 0;
    }
    return duplicate;
}

// FUNCTION: EQGFX 0x10014C1B
// Removes a directional light from scene lighting once it is unused.
// __cdecl, int, two stack arguments (context, light). Returns 0xffff3d3e while the use count (+0x10) is not 0.
// Else: _t3dRemoveFromLitRegions_guess, drops the owner's count, unlinks the dictionary entry if flagged, t3dFreeObjectPoolSlot_guess, frees +0x2c, key, light.
int __cdecl t3dDestroyDirectionalLight(T3dContext_guess *context, DirectionalLight_guess *light)
{
    int result;

    if (light->useCount > 0)
    {
        result = DIRECTIONAL_LIGHT_IN_USE_RESULT;
    }
    else
    {
        EQGFX__T3D_REMOVE_FROM_LIT_REGIONS(context, light);
        light->owner->lightCount--;
        if ((light->dictionaryFlags & DIRECTIONAL_LIGHT_DICTIONARY_FLAG) != 0)
        {
            EQGFX_T3D_REMOVE_ENTRY_FROM_DICTIONARY(context, light->dictionaryKey);
        }
        result = EQGFX_T3D_FREE_OBJECT_POOL_SLOT(context, light);
        EQGFX_T3D_FREE_MEMORY(light->ownedBlock);
        if (light->dictionaryKey != 0)
        {
            EQGFX_T3D_FREE_MEMORY(light->dictionaryKey);
        }
        EQGFX_T3D_FREE_MEMORY(light);
    }
    return result;
}

// FUNCTION: EQGFX 0x10014C85
// Gets the current animation frame's intensity for a directional light.
// __cdecl, one stack argument (the light). Returns the owner's intensity as an x87 float: a single-frame definition copies its one value over the argument slot and returns it, else the entry at the owner's current frame.
float __cdecl t3dGetDirectionalLightCurrentIntensity(DirectionalLight_guess *light)
{
    DirectionalLightOwner_guess *owner = light->owner;
    LightDefinition_guess *definition = owner->lightDefinition;

    if (definition->frameCount == 1)
    {
        EQ_CDECL(void *, 0x10067D30, (void *, const void *, unsigned int))(
            &light, &definition->intensityList, sizeof(float));
        return *(float *)&light;
    }
    return ((float *)definition->intensityList)[owner->currentFrame];
}

// FUNCTION: EQGFX 0x10014CB9
// Assigns a directional light to a different light instance.
// __cdecl, two stack arguments (light, owner). Moves the directional light to the new owner: the old owner's light count drops by one, the new one's rises by one. Returns 0.
int __cdecl t3dSetDirectionalLightLightInstance(DirectionalLight_guess *light, DirectionalLightOwner_guess *owner)
{
    DirectionalLightOwner_guess *oldOwner;

    oldOwner = light->owner;
    oldOwner->lightCount = oldOwner->lightCount - 1;
    light->owner = owner;
    owner->lightCount = owner->lightCount + 1;
    return 0;
}

// FUNCTION: EQGFX 0x10014CD0
// __cdecl, one stack argument (the light). Returns the owner's light definition: two hops, +0x14 then +0x24.
void *__cdecl t3dGetDirectionalLightLightDefinition(DirectionalLight_guess *light)
{
    return light->owner->lightDefinition;
}

// FUNCTION: EQGFX 0x10014CDB
// Gets the direction vector that aims a directional light.
// __cdecl, two stack arguments (the directional light, the output normal). Copies the three-dword normal at +0x18 into normal and returns 0.
int __cdecl t3dGetDirectionalLightNormal(DirectionalLight_guess *light, DirectionalLightNormal_guess *normal)
{
    *normal = light->normal;
    return 0;
}

// FUNCTION: EQGFX 0x10014CF0
// Changes the direction of a non-static directional light.
// __cdecl, int (callers read EAX), two stack arguments (the directional light, the new normal). Returns 0xffff3d3e without copying while the static flag byte (+0x0f) has 0x80 set. Else copies the three-dword normal to +0x18 and returns 0.
int __cdecl t3dSetDirectionalLightNormal(DirectionalLight_guess *light, DirectionalLightNormal_guess *normal)
{
    int result;

    result = 0;
    if ((light->staticFlags & DIRECTIONAL_LIGHT_STATIC_FLAG) != 0)
    {
        result = DIRECTIONAL_LIGHT_IN_USE_RESULT;
        return result;
    }
    light->normal = *normal;
    return result;
}

// FUNCTION: EQGFX 0x10014D11
// Reports whether a directional light is static.
// __cdecl, one stack argument (the directional light). Returns the top bit of the dword at +0x0c (the dictionary flag byte is its low byte).
unsigned int __cdecl t3dDirectionalLightIsStatic(DirectionalLight_guess *light)
{
    return *(unsigned int *)&light->dictionaryFlags >> 0x1f;
}

// FUNCTION: EQGFX 0x10014D1C
// __cdecl, one stack argument (the directional light). Returns bit 28 of the dword at +0x0c (1 or 0).
unsigned int __cdecl t3dDirectionalLightHasDynamicSceneInfluence(DirectionalLight_guess *light)
{
    return (*(unsigned int *)&light->dictionaryFlags >> 0x1c) & 1;
}

// FUNCTION: EQGFX 0x10014D2A
// __cdecl, one stack argument (the directional light). Returns bit 29 of the dword at +0x0c (1 or 0).
unsigned int __cdecl t3dDirectionalLightHasStaticSceneInfluence(DirectionalLight_guess *light)
{
    return (*(unsigned int *)&light->dictionaryFlags >> 0x1d) & 1;
}

// FUNCTION: EQGFX 0x10014D38
// Adds a directional light to the lighting assigned to a world region.
int __cdecl t3dAddDirectionalLightToRegion(t3dWorld_guess *world, t3dRegionIndex_guess *regionIndices, unsigned int regionIndex)
{
    int result;
    unsigned int insertionIndex;
    int upperBound;
    int middle;
    unsigned int capacity;
    unsigned int newCapacity;
    void *resizedMemory;
    t3dRegionSet_guess *regionSet;
    unsigned int *insertionPoint;

    result = 0;
    insertionIndex = 0;
    upperBound = regionIndices->regionCount - 1;
    while ((int)insertionIndex <= upperBound)
    {
        middle = (int)(upperBound + insertionIndex) >> 1;
        capacity = regionIndices->regionIndices[middle];
        if (capacity == regionIndex)
            goto done;
        if (regionIndex < capacity)
            upperBound = middle - 1;
        else
            insertionIndex = middle + 1;
    }

    capacity = regionIndices->regionCapacity;
    if (capacity <= (unsigned int)regionIndices->regionCount)
    {
        newCapacity = (capacity >> 2) + 1 + capacity;
        resizedMemory = EQGFX_T3D_REALLOCATE_MEMORY(regionIndices->regionIndices, newCapacity * 4);
        if (resizedMemory == 0)
        {
            result = DIRECTIONAL_LIGHT_IN_USE_RESULT;
            goto done;
        }
        regionIndices->regionIndices = (unsigned int *)resizedMemory;
        regionIndices->regionCapacity = newCapacity;
    }

    regionSet = world->regions[regionIndex]->regionSet;
    if ((regionSet->tag != 0x27) && (regionSet->tag != 0x29))
    {
        result = DIRECTIONAL_LIGHT_IN_USE_RESULT;
        goto done;
    }

    capacity = regionSet->entryCapacity;
    if (capacity <= (unsigned int)regionSet->entryCount)
    {
        newCapacity = (capacity >> 2) + 1 + capacity;
        resizedMemory = EQGFX_T3D_REALLOCATE_MEMORY(regionSet->entries, newCapacity * 4);
        if (resizedMemory == 0)
        {
            result = DIRECTIONAL_LIGHT_IN_USE_RESULT;
            goto done;
        }
        regionSet->entries = (void **)resizedMemory;
        regionSet->entryCapacity = newCapacity;
    }

    if (regionSet->entryCount != 0)
    {
        EQGFX_MEMCPY(regionSet->entries + 1, regionSet->entries, regionSet->entryCount << 2);
    }
    regionSet->entries[0] = regionIndices;
    regionSet->entryCount = regionSet->entryCount + 1;
    insertionPoint = regionIndices->regionIndices + insertionIndex;
    if (insertionIndex < (unsigned int)regionIndices->regionCount)
    {
        EQGFX_MEMCPY(insertionPoint + 1, insertionPoint,
            (insertionIndex * 0x3fffffff + regionIndices->regionCount) * 4);
    }
    *insertionPoint = regionIndex;
    regionIndices->regionCount = regionIndices->regionCount + 1;

done:
    return result;
}

// FUNCTION: EQGFX 0x10014E48
// Removes a directional light's lighting assignment from one world region.
// __cdecl, three stack arguments (the world, the region's index list, regionIndex). Removes regionIndex from the sorted list by binary search; if the region is tagged 0x27 or 0x29, also removes the list from its region set's entries. Returns 0.
int __cdecl t3dRemoveDirectionalFromRegion(t3dWorld_guess *world, t3dRegionIndex_guess *regionIndices, unsigned int regionIndex)
{
    int lowerBound;
    int upperBound;
    int middle;
    unsigned int middleValue;
    unsigned int *slot;
    t3dRegionSet_guess *regionSet;
    void **entry;
    void **entryEnd;

    lowerBound = 0;
    upperBound = regionIndices->regionCount - 1;
    if (upperBound >= 0)
    {
        do
        {
            middle = (lowerBound + upperBound) >> 1;
            middleValue = regionIndices->regionIndices[middle];
            if (middleValue == regionIndex)
                break;
            if (middleValue > regionIndex)
                upperBound = middle - 1;
            else
                lowerBound = middle + 1;
        } while (lowerBound <= upperBound);
        if (lowerBound <= upperBound)
        {
            slot = regionIndices->regionIndices + middle;
            EQGFX_MEMCPY(slot, slot + 1, (middle * 0x3fffffff + regionIndices->regionCount) * 4);
            regionIndices->regionCount = regionIndices->regionCount - 1;
            regionSet = world->regions[regionIndex]->regionSet;
            if ((regionSet->tag == 0x27) || (regionSet->tag == 0x29))
            {
                entry = (void **)regionSet->entries;
                entryEnd = entry + regionSet->entryCount;
                for (; entry < entryEnd; entry = entry + 1)
                {
                    if (*entry == regionIndices)
                    {
                        if (entry != entryEnd - 1)
                        {
                            EQGFX_MEMCPY(entry, entry + 1, (((int)(entryEnd - 1) - (int)entry) >> 2) << 2);
                        }
                        regionSet->entryCount = regionSet->entryCount - 1;
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10014F01
// Pauses or resumes animation on a directional light.
// __cdecl, three stack arguments (the directional light, suspend, resumeMask). Nonzero suspend sets the suspended bit of the dword at +0x0c; else clears it and ORs resumeMask into the dword at +0x1c. Returns 0.
int __cdecl t3dSuspendLightAnimation(DirectionalLight_guess *light, int suspend, unsigned int resumeMask)
{
    if (suspend != 0)
    {
        *(unsigned int *)&light->dictionaryFlags |= DIRECTIONAL_LIGHT_ANIMATION_SUSPENDED_FLAG;
    }
    else
    {
        *(unsigned int *)&light->dictionaryFlags &= ~DIRECTIONAL_LIGHT_ANIMATION_SUSPENDED_FLAG;
    }
    if (suspend == 0)
    {
        *(unsigned int *)((char *)light + 0x1c) |= resumeMask;
    }
    return 0;
}
