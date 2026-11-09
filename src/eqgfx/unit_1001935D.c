#include "trilogy/eqaddr.h"

typedef struct T3DSkyRegionPenEntry_guess
{
    unsigned char unknown00[0x1a];
    unsigned char pen;
    unsigned char unknown1b[0x35];
} T3DSkyRegionPenEntry_guess;

#define T3D_SKY_REGION_PEN_ENTRY_SIZE 0x50
#define T3D_SKY_REGION_RENDER_METHOD_ERROR 0xffff3d3e

// unit_1001935D.c - eqgfx_dx7.dll, the address run 0x1001935D..0x10019C11 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

/* S3DAllocationPool_guess: the actor layout is S3DAllocationPool_guess (generated: eqgfx/layout/s3d_pool_block.h); the old local view's members map by offset. */
#include "eqgfx/layout/s3d_pool_block.h"

typedef struct MemoryBufferTable_guess
{
    unsigned char unknown00;
    unsigned char unknown01;
    unsigned char unknown02[2];
    int *endNode;
    unsigned char unknown08;
    int size;
} MemoryBufferTable_guess;

typedef struct MemoryBufferKey_guess
{
    int unknown00[7];
} MemoryBufferKey_guess;

typedef void (__stdcall *OutputDebugStringA_guess_t)(const char *);

#define EQGFX_UNLINK_ALLOCATION_POOL EQ_CDECL(void, 0x10019B12, (int, int))
#define EQGFX_LINK_ALLOCATION_POOL EQ_CDECL(void, 0x10019AE9, (int, S3DAllocationPool_guess *))
#define EQGFX_REGISTER_ALLOCATION_POOL EQ_CDECL(int, 0x1001564B, (int, S3DAllocationPool_guess *))
#define EQGFX_CREATE_ALLOCATION_POOL EQ_CDECL(S3DAllocationPool_guess *, 0x1001996C, (int, unsigned int, int))
#define EQGFX_FIND_FREE_ALLOCATION_POOL EQ_CDECL(S3DAllocationPool_guess *, 0x10019ABA, (int, unsigned int, int))
#define ALLOCATION_POOL_MIN_DATA_SIZE 0xffff
#define EQGFX_REMOVE_DICTIONARY_ENTRY EQ_CDECL(void, 0x1000E334, (int, int))
#define EQGFX_RELEASE_ALLOCATION_POOL EQ_CDECL(int, 0x10015D81, (int, int))
#define EQGFX_FREE_MEMORY EQ_CDECL(void, 0x10050CBB, (int))
#define EQGFX_ALLOCATE_MEMORY EQ_CDECL(void *, 0x10050C48, (unsigned int))
#define EQGFX_T3D_UNSHARE_OBJECT EQ_CDECL(void, 0x10017DAD, (int *))
#define EQGFX_T3D_SHARE_OBJECT EQ_CDECL(void, 0x10017CE8, (int *))
#define EQGFX_DEFAULT_SKY_REGION_SPRITE ((int *)0x100f401c)
#define EQGFX_MEMORY_BUFFER_TABLE EQ_GLOBAL(MemoryBufferTable_guess *, 0x100AADD4)
#define EQGFX_MEMORY_BUFFER_HITS EQ_GLOBAL(int, 0x100AADD8)
#define EQGFX_FIND_MEMORY_BUFFER_KEY EQ_THISCALL(void, 0x10019E55, (MemoryBufferTable_guess *, int, int **, MemoryBufferKey_guess *))
#define EQGFX_INSERT_MEMORY_BUFFER_KEY EQ_THISCALL(void, 0x10019CC9, (MemoryBufferTable_guess *, int, int *, MemoryBufferKey_guess *))
#define EQGFX_OPERATOR_NEW EQ_CDECL(void *, 0x10068C72, (unsigned int))
#define EQGFX_INIT_MEMORY_BUFFER_TABLE EQ_FASTCALL(void, 0x1001A12D, (MemoryBufferTable_guess *))
#define EQGFX_CLEAR_MEMORY_BUFFER_TABLE EQ_FASTCALL(void, 0x10019C6A, (MemoryBufferTable_guess *))
#define EQGFX_TREE_NIL_NODE EQ_GLOBAL(int *, 0x100AADDC)
#define tree_count EQ_GLOBAL(int, 0x100AADE0)
#define EQGFX_TREE_ERASE_RANGE EQ_THISCALL(void, 0x10019DBE, (MemoryBufferTable_guess *, int, void **, int *, int *))
#define EQGFX_LOCKIT_CONSTRUCT EQ_THISCALL(void, 0x10075A66, (int *, int))
#define EQGFX_LOCKIT_DESTRUCT EQ_THISCALL(void, 0x10075B02, (int *, int))
#define EQGFX_SPRINTF EQ_CDECL(int, 0x10067992, (char *, const char *, int))
#define EQGFX_CLEAR_MEMORY_AND_RESET_POINTERS EQ_FASTCALL(void, 0x10019C0C, (MemoryBufferTable_guess *))
#define EQGFX_OUTPUT_DEBUG_STRING_A EQ_GLOBAL(OutputDebugStringA_guess_t, 0x100760C4)
#define MEMORY_BUFFER_TABLE_SIZE 0x10

typedef struct T3DAmbientLight_guess
{
    unsigned char unknown00[0x10];
    int referenceCount;
} T3DAmbientLight_guess;

typedef struct T3DRegionState_guess
{
    int type;
    T3DAmbientLight_guess *ambientLight;
    int numActions;
    int *levelOfDetailData;
    int currentAction;
    int isFoggy;
    int unknown18[2];
    int fogDistance;
} T3DRegionState_guess;

typedef struct T3DRegionMode_guess
{
    int type;
} T3DRegionMode_guess;

typedef struct T3DRegionActorState_guess
{
    int type;
    int numActors;
    int unknown08;
    int *actorIndices;
    int numLights;
    int unknown14;
    int *lights;
} T3DRegionActorState_guess;

typedef struct T3DRegionActorLookup_guess
{
    unsigned char unknown00[0x17c];
    int *actors;
} T3DRegionActorLookup_guess;

typedef struct T3DRegion_guess
{
    int type;
    unsigned char unknown04[0x0c];
    unsigned int boundingSphere[5];
    T3DRegionState_guess *state;
    T3DRegionMode_guess *mode;
    T3DRegionActorState_guess *actorState;
    int userData;
} T3DRegion_guess;

typedef struct T3DSkyRegionRenderMethodReference_guess
{
    int unknown00[2];
    int renderMethodNumber;
} T3DSkyRegionRenderMethodReference_guess;

typedef struct T3DSkyRegionLevelOfDetail_guess
{
    unsigned int numRenderMethods;
    T3DSkyRegionRenderMethodReference_guess *renderMethodReferences;
} T3DSkyRegionLevelOfDetail_guess;

typedef struct T3DSkyRegionEndDistanceEntry_guess
{
    int unknown00;
    float endDistance;
    int unknown08;
} T3DSkyRegionEndDistanceEntry_guess;

typedef struct T3DSkyRegionRenderMethod_guess
{
    int unknown00;
    int renderMethod;
    float constantIntensity;
    float ambientScaler;
    unsigned char unknown10[0x40];
} T3DSkyRegionRenderMethod_guess;

typedef struct T3DSkyRegionSpriteEntry_guess
{
    unsigned char unknown00[0x1c];
    int *sprite;
    unsigned char unknown20[0x30];
} T3DSkyRegionSpriteEntry_guess;

typedef struct T3D2DSprite_guess
{
    unsigned char unknown00[8];
    char *dictionaryKey;
    unsigned int flags;
} T3D2DSprite_guess;

// FUNCTION: EQGFX 0x1001935D
// Stores caller data on a renderer region.
int __cdecl t3dSetRegionUserData(T3DRegion_guess *region, int userData)
{
    region->userData = userData;
    return 0;
}

// FUNCTION: EQGFX 0x1001936B
/* fake match: the empty dictionary branches preserve VC6 block order. */
// Changes or clears the name used to look up a 2D sprite.
// __cdecl (three stack arguments). Replaces the sprite's tag with a heap copy of the string (NULL clears it),
// re-keying its dictionary entry. Returns the removal's error, -1 if the copy fails, else 0 or the re-add's result.
int __cdecl t3dSet2DSpriteTag(void *dictionary, T3D2DSprite_guess *sprite, const char *tag)
{
    unsigned int tagLength;
    unsigned int inDictionary;
    int result;
    char *key;

    result = 0;
    inDictionary = sprite->flags & 1;
    if (inDictionary == 0)
    {
    }
    else
    {
        result = EQ_CDECL(int, 0x1000E334, (void *, void *))(dictionary, sprite->dictionaryKey);
        if (result == 0)
        {
        }
        else
        {
            return result;
        }
    }
    if (sprite->dictionaryKey != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(sprite->dictionaryKey);
    }
    if (tag != 0)
    {
        tagLength = EQ_CDECL(unsigned int, 0x10068160, (const char *))(tag);
        key = EQGFX_ALLOCATE_MEMORY(tagLength + 1);
        sprite->dictionaryKey = key;
        if (key == 0)
        {
            result = -1;
        }
        else
        {
            EQ_CDECL(void, 0x10068070, (char *, const char *))(key, tag);
            if (inDictionary != 0)
            {
                if (EQ_CDECL(void *, 0x1000E7FE, (void *, const char *))(dictionary, tag) == 0)
                {
                    result = EQ_CDECL(int, 0x1000DC08, (void *, void *))(dictionary, sprite);
                }
            }
        }
    }
    else
    {
        sprite->dictionaryKey = 0;
    }
    return result;
}

// FUNCTION: EQGFX 0x100193FE
// Gets the bounding sphere that describes a renderer region.
int __cdecl t3dGetRegionBoundingSphere(T3DRegion_guess *region, unsigned int *boundingSphere)
{
    int count;
    unsigned int *source;

    source = region->boundingSphere;
    for (count = 5; count != 0; count = count + -1) {
        *boundingSphere = *source;
        source = source + 1;
        boundingSphere = boundingSphere + 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019415
// Sets the bounding sphere that describes a renderer region.
int __cdecl t3dSetRegionBoundingSphere(T3DRegion_guess *region, unsigned int *boundingSphere)
{
    int count;
    unsigned int *destination;

    destination = region->boundingSphere;
    for (count = 5; count != 0; count = count + -1) {
        *destination = *boundingSphere;
        boundingSphere = boundingSphere + 1;
        destination = destination + 1;
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001942C
// Checks whether a region has a supported state, mode, and actor-state combination.
int __cdecl t3dIsRegionValid(T3DRegion_guess *region)
{
    int actorTypeMatches;

    if ((region != 0) && (region->type == 0x19)) {
        if (region->state->type == 0x28) {
            if (region->mode->type == 0) {
                return 1;
            }
            actorTypeMatches = region->actorState->type == 0;
        }
        else {
            if (region->state->type != 0x25) {
                return 0;
            }
            if (region->mode->type == 0x26) {
                return 1;
            }
            if (region->actorState->type == 0x27) {
                return 1;
            }
            actorTypeMatches = region->actorState->type == 0x29;
        }
        if (actorTypeMatches) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019475
// Reports whether a region's current state is foggy.
int __cdecl t3dIsRegionFoggy(T3DRegion_guess *region)
{
    if (region->state->type != 0x25)
        return 0;
    return region->state->isFoggy;
}

// FUNCTION: EQGFX 0x10019488
// Returns the ambient light assigned to a supported region state.
int __cdecl t3dGetRegionAmbientLight(T3DRegion_guess *region)
{
    T3DRegionState_guess *state;
    int type;

    state = region->state;
    type = state->type;
    if (type != 0x25 && type != 0x28)
        return 0;
    return state->ambientLight;
}

// FUNCTION: EQGFX 0x100194a2
// __cdecl. Sets the region's ambient light, swapping the ref-counted light.
int __cdecl t3dSetRegionAmbientLight(T3DRegion_guess *region, T3DAmbientLight_guess *ambientLight)
{
    T3DRegionState_guess *state;
    T3DAmbientLight_guess *oldLight;
    int result;

    result = 0;
    state = region->state;
    if (state->type == 0x25 || state->type == 0x28)
    {
        oldLight = state->ambientLight;
        if (oldLight != 0)
            oldLight->referenceCount--;
        region->state->ambientLight = ambientLight;
        ambientLight->referenceCount++;
    }
    else
    {
        result = 0xffff3d3e;
    }
    return result;
}

// FUNCTION: EQGFX 0x100194D8
// Gets the number of actors assigned to a region.
int __cdecl t3dGetRegionNumActors(T3DRegion_guess *region)
{
    T3DRegionActorState_guess *actorState;
    int actorType;

    actorState = region->actorState;
    actorType = actorState->type;
    if (actorType != 0x27 && actorType != 0x29)
        return 0;
    return actorState->numActors;
}

// FUNCTION: EQGFX 0x100194F1
// Resolves a region actor index to the renderer actor it refers to.
int __cdecl t3dGetRegionActor(T3DRegionActorLookup_guess *actorLookup, T3DRegion_guess *region, unsigned int actorIndex)
{
    T3DRegionActorState_guess *actorState;
    int actorType;

    actorState = region->actorState;
    actorType = actorState->type;
    if ((actorType == 0x27 || actorType == 0x29) && actorIndex < (unsigned int)actorState->numActors)
        return actorLookup->actors[actorState->actorIndices[actorIndex]];
    return 0;
}

// FUNCTION: EQGFX 0x10019523
// Gets the number of lights assigned to a region.
int __cdecl t3dGetRegionNumLights(T3DRegion_guess *region)
{
    T3DRegionActorState_guess *actorState;
    int actorType;

    actorState = region->actorState;
    actorType = actorState->type;
    if (actorType != 0x27 && actorType != 0x29)
        return 0;
    return actorState->numLights;
}

// FUNCTION: EQGFX 0x1001953C
// Gets the light entry at a region's light index.
int __cdecl t3dGetRegionLight(T3DRegion_guess *region, unsigned int lightIndex)
{
    T3DRegionActorState_guess *actorState;

    actorState = region->actorState;
    if (actorState->type != 0x27 && actorState->type != 0x29)
        return 0;
    if (lightIndex >= actorState->numLights)
        return 0;
    return actorState->lights[lightIndex];
}

// FUNCTION: EQGFX 0x10019561
// Gets the number of actions configured for a sky region.
int __cdecl t3dGetSkyRegionNumActions(T3DRegion_guess *region)
{
    T3DRegionState_guess *state;

    state = region->state;
    if (state->type != 0x28)
        return 0;
    return state->numActions;
}

// FUNCTION: EQGFX 0x10019574
// Gets the active action index for a sky region.
int __cdecl t3dGetSkyRegionCurrentAction(T3DRegion_guess *region)
{
    T3DRegionState_guess *state;

    state = region->state;
    if (state->type != 0x28)
        return 0;
    return state->currentAction;
}

#define T3D_SKY_REGION_TYPE 0x28
#define T3D_SKY_REGION_INVALID_TYPE_RESULT 0xffff3d3e

// FUNCTION: EQGFX 0x10019587
// Selects the active action for a sky region.
int __cdecl t3dSetSkyRegionCurrentAction(T3DRegion_guess *region, unsigned int actionIndex)
{
    T3DRegionState_guess *state;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && actionIndex < (unsigned int)state->numActions)
    {
        state->currentAction = actionIndex;
        return 0;
    }
    return T3D_SKY_REGION_INVALID_TYPE_RESULT;
}

// FUNCTION: EQGFX 0x100195a8
// Gets the sprite used for a sky detail level and render method.
int *__cdecl t3dGetSkyRegionSprite(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionRenderMethodReference_guess *renderMethodReference;
    T3DSkyRegionSpriteEntry_guess *spriteEntries;
    int *sprite;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)state->levelOfDetailData + levelOfDetailIndex;
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            renderMethodReference = levelOfDetail->renderMethodReferences + renderMethodIndex;
            spriteEntries = (T3DSkyRegionSpriteEntry_guess *)state->unknown18[0];
            sprite = spriteEntries[renderMethodReference->renderMethodNumber].sprite;
            if (sprite != 0 && *sprite != 0)
                return sprite;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x100195ee
// Assigns a sprite to a sky detail level and render method.
int __cdecl t3dSetSkyRegionSprite(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex, int *sprite)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionRenderMethodReference_guess *renderMethodReference;
    T3DSkyRegionSpriteEntry_guess *spriteEntries;
    int *currentSprite;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)state->levelOfDetailData + levelOfDetailIndex;
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            renderMethodReference = levelOfDetail->renderMethodReferences + renderMethodIndex;
            spriteEntries = (T3DSkyRegionSpriteEntry_guess *)state->unknown18[0];
            currentSprite = spriteEntries[renderMethodReference->renderMethodNumber].sprite;
            if (*currentSprite != 0)
            {
                if (*currentSprite != 6)
                    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
                EQGFX_T3D_UNSHARE_OBJECT(currentSprite);
            }
            if (sprite == 0)
                sprite = EQGFX_DEFAULT_SKY_REGION_SPRITE;
            spriteEntries[renderMethodReference->renderMethodNumber].sprite = sprite;
            if (*sprite == 0)
                return 0;
            if (*sprite == 6)
            {
                EQGFX_T3D_SHARE_OBJECT(sprite);
                return 0;
            }
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x1001967D
// Gets the distance boundary for a sky region detail level.
float __cdecl t3dGetSkyRegionEndDistance(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int distanceIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionEndDistanceEntry_guess *endDistances;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)state->levelOfDetailData + levelOfDetailIndex;
        if (distanceIndex < levelOfDetail->numRenderMethods)
        {
            endDistances = (T3DSkyRegionEndDistanceEntry_guess *)levelOfDetail->renderMethodReferences;
            return EQ_CDECL(float, 0x1004EF68, (float))(endDistances[distanceIndex].endDistance);
        }
    }
    return 0.0f;
}

// FUNCTION: EQGFX 0x100196B8
// Sets the cutoff distance for a sky detail level transition.
int __cdecl t3dSetSkyRegionEndDistance(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int distanceIndex, float endDistance)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionEndDistanceEntry_guess *endDistances;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)state->levelOfDetailData + levelOfDetailIndex;
        if (distanceIndex < levelOfDetail->numRenderMethods - 1U)
        {
            endDistances = (T3DSkyRegionEndDistanceEntry_guess *)levelOfDetail->renderMethodReferences;
            endDistances[distanceIndex].endDistance = endDistance * endDistance;
            return 0;
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x100196FC
// Returns the render-method count for a sky region detail level.
int __cdecl t3dGetSkyRegionNumLevelsOfDetail(T3DRegion_guess *region, unsigned int levelOfDetailIndex)
{
    T3DRegionState_guess *state;

    state = region->state;
    if ((T3D_SKY_REGION_TYPE == state->type
        && levelOfDetailIndex < (unsigned int)state->numActions) == 0)
    {
    }
    else
    {
        return state->levelOfDetailData[levelOfDetailIndex * 2];
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001971B
// Gets the render method assigned to a sky detail level slot.
int __cdecl t3dGetSkyRegionRenderMethod(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            return ((T3DSkyRegionRenderMethod_guess *)state->unknown18[0])[
                levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber].renderMethod;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019759
// Sets the render method assigned to a sky detail level slot.
int __cdecl t3dSetSkyRegionRenderMethod(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex, int renderMethod)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            ((T3DSkyRegionRenderMethod_guess *)state->unknown18[0])[
                levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber].renderMethod = renderMethod;
            return 0;
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x100197A3
// Gets the fixed light intensity for a sky render method.
float __cdecl t3dGetSkyRegionConstantIntensity(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            return ((T3DSkyRegionRenderMethod_guess *)state->unknown18[0])[
                levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber].constantIntensity;
        }
    }
    return 0.0f;
}

// FUNCTION: EQGFX 0x100197e1
// Sets the fixed light intensity for a sky render method.
int __cdecl t3dSetSkyRegionConstantIntensity(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex, float constantIntensity)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            ((T3DSkyRegionRenderMethod_guess *)state->unknown18[0])[
                levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber].constantIntensity = constantIntensity;
            return 0;
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x1001982B
// Gets the ambient-light scale for a sky render method.
float __cdecl t3dGetSkyRegionAmbientScaler(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            return ((T3DSkyRegionRenderMethod_guess *)state->unknown18[0])[
                levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber].ambientScaler;
        }
    }
    return 0.0f;
}

// FUNCTION: EQGFX 0x10019869
// Sets the ambient-light scale for a sky render method.
int __cdecl t3dSetSkyRegionAmbientScaler(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex, float ambientScaler)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionRenderMethod_guess *renderMethods;
    unsigned int renderMethodNumber;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE
        && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            renderMethodNumber = levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber;
            renderMethods = (T3DSkyRegionRenderMethod_guess *)state->unknown18[0];
            renderMethods[renderMethodNumber].ambientScaler = ambientScaler;
            return 0;
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x100198B3
// Gets the pen value associated with a sky render method.
unsigned int __cdecl t3dGetSkyRegionPen(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionPenEntry_guess *penEntries;
    unsigned int penIndex;
    unsigned int result;

    state = region->state;
    result = (unsigned int)region;
    if (state->type == T3D_SKY_REGION_TYPE)
    {
        result = levelOfDetailIndex;
        if (levelOfDetailIndex < (unsigned int)state->numActions)
        {
            levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
            result = renderMethodIndex;
            if (renderMethodIndex < levelOfDetail->numRenderMethods)
            {
                penIndex = levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber;
                penEntries = (T3DSkyRegionPenEntry_guess *)state->unknown18[0];
                result = penIndex * T3D_SKY_REGION_PEN_ENTRY_SIZE;
                *((unsigned char *)&result) = penEntries[penIndex].pen;
                return result;
            }
        }
    }
    return result & 0xffffff00;
}

// FUNCTION: EQGFX 0x100198F1
// Sets the pen value used by a sky render method.
int __cdecl t3dSetSkyRegionPen(T3DRegion_guess *region, unsigned int levelOfDetailIndex, unsigned int renderMethodIndex, unsigned char pen)
{
    T3DRegionState_guess *state;
    T3DSkyRegionLevelOfDetail_guess *levelOfDetail;
    T3DSkyRegionPenEntry_guess *penEntries;
    unsigned int penIndex;

    state = region->state;
    if (state->type == T3D_SKY_REGION_TYPE && levelOfDetailIndex < (unsigned int)state->numActions)
    {
        levelOfDetail = (T3DSkyRegionLevelOfDetail_guess *)&state->levelOfDetailData[levelOfDetailIndex * 2];
        if (renderMethodIndex < levelOfDetail->numRenderMethods)
        {
            penIndex = levelOfDetail->renderMethodReferences[renderMethodIndex].renderMethodNumber;
            penEntries = (T3DSkyRegionPenEntry_guess *)state->unknown18[0];
            penEntries[penIndex].pen = pen;
            return 0;
        }
    }
    return T3D_SKY_REGION_RENDER_METHOD_ERROR;
}

// FUNCTION: EQGFX 0x1001993D
// Gets the fog cutoff distance used by a sky region.
float __cdecl t3dGetSkyRegionFogDistance(T3DRegion_guess *region)
{
    T3DRegionState_guess *state;

    state = region->state;
    return (state->type != T3D_SKY_REGION_TYPE) ? 0 : (*(float *)&state->fogDistance);
}

// FUNCTION: EQGFX 0x10019950
// Sets the fog cutoff distance for a sky region.
int __cdecl t3dSetSkyRegionFogDistance(T3DRegion_guess *region, int fogDistance)
{
    T3DRegionState_guess *state;

    state = region->state;
    if (state->type != T3D_SKY_REGION_TYPE)
        return T3D_SKY_REGION_INVALID_TYPE_RESULT;
    state->fogDistance = fogDistance;
    return 0;
}

// FUNCTION: EQGFX 0x1001996c
// Creates and registers a managed resource pool for the renderer.
S3DAllocationPool_guess *__cdecl s3dCreateAllocationPool(int poolManager, unsigned int dataSize, int mode)
{
    S3DAllocationPool_guess *allocation;
    int status;

    allocation = (S3DAllocationPool_guess *)EQGFX_ALLOCATE_MEMORY(dataSize + sizeof(S3DAllocationPool_guess));
    if (allocation != 0)
    {
        allocation->tag = 0x53;
        allocation->unk_0x4 = 0;
        allocation->dictionaryEntry_int_guess = 0;
        *(int *)&allocation->flags = 0;
        allocation->referenceCount = 0;
        allocation->dataSize = dataSize;
        allocation->scaledSize = (int)((double)dataSize * 0.98f);
        allocation->data = (int *)(allocation + 1);
        allocation->unknown18 = 0;
        allocation->mode = mode;
        allocation->linkWords_guess.unknown28[0] = 0;
        allocation->linkWords_guess.unknown28[1] = 0;
        if (mode == 1)
            *(int *)&allocation->flags = 0x10;
        status = EQGFX_REGISTER_ALLOCATION_POOL(poolManager, allocation);
        if (status != 0)
        {
            EQGFX_FREE_MEMORY((int)allocation);
        }
        else
        {
            EQGFX_LINK_ALLOCATION_POOL(poolManager, allocation);
            return allocation;
        }
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019a01
// Releases an unused renderer resource pool and its dictionary entry.
int __cdecl s3dDestroyAllocationPool(int poolManager, int allocationAddress)
{
    S3DAllocationPool_guess *allocation;
    int result;

    allocation = (S3DAllocationPool_guess *)allocationAddress;
    if (allocation->referenceCount != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        EQGFX_UNLINK_ALLOCATION_POOL(poolManager, allocationAddress);
        if ((allocation->flags & 1) != 0)
            EQGFX_REMOVE_DICTIONARY_ENTRY(poolManager, allocation->dictionaryEntry_int_guess);
        result = EQGFX_RELEASE_ALLOCATION_POOL(poolManager, allocationAddress);
        if (allocation->dictionaryEntry_int_guess != 0)
            EQGFX_FREE_MEMORY(allocation->dictionaryEntry_int_guess);
        EQGFX_FREE_MEMORY(allocationAddress);
    }
    return result;
}

// FUNCTION: EQGFX 0x10019a5c
// __cdecl. Carves size bytes from the first pool with room (else a new pool); returns their address, or 0.
int __cdecl s3dAllocateMemoryFromPool(int poolManager, unsigned int size, int mode)
{
    S3DAllocationPool_guess *pool;
    unsigned int poolDataSize;
    int usedBytes;
    int dataAddress;
    unsigned int newUsedBytes;

    pool = EQGFX_FIND_FREE_ALLOCATION_POOL(poolManager, size, mode);
    if (pool == 0)
    {
        poolDataSize = ALLOCATION_POOL_MIN_DATA_SIZE;
        if (size >= poolDataSize)
            poolDataSize = size;
        pool = EQGFX_CREATE_ALLOCATION_POOL(poolManager, poolDataSize, mode);
        if (pool == 0)
            return 0;
    }
    usedBytes = pool->unknown18;
    dataAddress = (int)pool->data + usedBytes;
    newUsedBytes = usedBytes + size;
    pool->unknown18 = newUsedBytes;
    if (newUsedBytes >= (unsigned int)pool->scaledSize)
        EQGFX_UNLINK_ALLOCATION_POOL(poolManager, (int)pool);
    return dataAddress;
}

// FUNCTION: EQGFX 0x10019aba
// Finds a renderer resource pool block with enough room for an allocation.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Scans the pool list
// at +0x37c when kind is 1, otherwise the list at +0x378, for the first block
// whose free bytes (+0x14 minus +0x18, unsigned) fit size. Returns it, or 0.
int __cdecl s3dFindFreeAllocationPoolBlock_guess(int pool, unsigned int size, int kind)
{
    int block;

    if (kind == 1)
        block = *(int *)(pool + 0x37c);
    else
        block = *(int *)(pool + 0x378);
    while (block != 0)
    {
        if ((unsigned)(*(int *)(block + 0x14) - *(int *)(block + 0x18)) >= size)
            break;
        block = *(int *)(block + 0x28);
    }
    return block;
}

// FUNCTION: EQGFX 0x10019ae9
// Makes an allocation pool block available to renderer resource requests.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Links the block at
// the head of one of the pool's two lists: the list at +0x37c when the block's
// word at +0x24 is 1, otherwise the list at +0x378. The old head goes to the
// block's +0x28 and points back at +0x2c when present.
void __cdecl s3dLinkAllocationPoolBlock_guess(int pool, int block)
{
    int temp;
    int oldHead;
    int *head;

    head = (int *)(pool + 0x37c);
    if (*(int *)(block + 0x24) != 1)
        head = (int *)(pool + 0x378);
    *(int *)(block + 0x28) = *head;
    temp = (block + 0x28);
    oldHead = *(int *)temp;
    if (oldHead)
        *(int *)(oldHead + 0x2c) = block;
    *head = block;
}

// FUNCTION: EQGFX 0x10019b12
// Removes an allocation pool block from renderer resource requests.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Unlinks the block
// from one of the pool's two lists: the list at +0x37c when the block's
// word at +0x24 is 1, otherwise the list at +0x378. A block with no links
// (+0x28 and +0x2c both 0) unlinks only when it is the head, otherwise it
// returns without touching anything. Clears +0x28/+0x2c when unlinked.
void __cdecl s3dUnlinkAllocationPoolBlock_guess(int pool, int block)
{
    int next;
    int headBlock;
    int *head;

    next = ((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[0];
    if (next == 0 && ((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[1] == 0)
    {
        if (((S3DAllocationPool_guess*)block)->mode == 1)
            headBlock = *(int *)(pool + 0x37c);
        else
            headBlock = *(int *)(pool + 0x378);
        if (block != headBlock)
            return;
    }
    if (((S3DAllocationPool_guess*)block)->mode == 1)
        head = (int *)(pool + 0x37c);
    else
        head = (int *)(pool + 0x378);
    if (block == *head)
        *head = next;
    else
    {
        if (((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[1] != 0)
            *(int *)(((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[1] + 0x28) = next;
        if (((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[0] != 0)
            *(int *)(((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[0] + 0x2c) = ((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[1];
    }
    ((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[0] = 0;
    ((S3DAllocationPool_guess*)block)->linkWords_guess.unknown28[1] = 0;
}

// FUNCTION: EQGFX 0x10019B79
// Enables tracking to suppress repeated actor data during loading.
// __fastcall (the one argument arrives in ECX, callee pops 0). Makes the memory buffer table; returns 0, or -1 when the allocation fails.
int __fastcall s3dEnableActorDupRemoval(unsigned int enableValue)
{
    MemoryBufferTable_guess *table;

    table = (MemoryBufferTable_guess *)EQGFX_OPERATOR_NEW(MEMORY_BUFFER_TABLE_SIZE);
    if (table != 0)
    {
        table->unknown08 = 0;
        table->unknown00 = (unsigned char)(enableValue >> 24);
        table->unknown01 = (unsigned char)(enableValue >> 24);
        EQGFX_INIT_MEMORY_BUFFER_TABLE(table);
    }
    else
    {
        table = 0;
    }
    EQGFX_MEMORY_BUFFER_TABLE = table;
    return (table != 0) - 1;
}

// FUNCTION: EQGFX 0x10019BB7
// Stops duplicate actor-data tracking and clears its remembered keys.
// __cdecl, no arguments (callee pops 0), returns 0. Logs the hit count, frees the memory buffer table, then clears the table pointer.
int __cdecl s3dDisableActorDupRemoval(void)
{
    char message[80];
    MemoryBufferTable_guess *table;

    if (EQGFX_MEMORY_BUFFER_TABLE != 0)
    {
        EQGFX_SPRINTF(message, EQ_ARRAY(char, 0x10086DA4), EQGFX_MEMORY_BUFFER_HITS);
        EQGFX_OUTPUT_DEBUG_STRING_A(message);
        if ((table = EQGFX_MEMORY_BUFFER_TABLE) != 0)
        {
            EQGFX_CLEAR_MEMORY_AND_RESET_POINTERS(table);
            EQGFX_FREE_MEMORY((int)table);
        }
        EQGFX_MEMORY_BUFFER_TABLE = 0;
    }
    return 0;
}

// FUNCTION: EQGFX 0x10019C0C
// Thunk that clears the remembered keys used for duplicate actor tracking.
// __fastcall (the table arrives in ECX, callee pops 0), void. The stub at 0x10019C0C jumps to the body: erases the nodes under a _Lockit, frees the header, then frees the shared nil node when the last table drops its count.
void __fastcall clearMemoryAndResetPointers_guess(MemoryBufferTable_guess *table)
{
    EQGFX_CLEAR_MEMORY_BUFFER_TABLE(table);
}

// FUNCTION: EQGFX 0x10019C11
// Checks whether actor parsing has already encountered a memory key.
// __cdecl. Searches the table at 0x100aadd4: a miss inserts the key and returns 0, a hit bumps 0x100aadd8 and returns 1.
int __cdecl FindOrInsertMemoryBufferKey_guess(int *key)
{
    MemoryBufferKey_guess keyCopy;
    int resultPair[2];
    MemoryBufferTable_guess *table;

    table = EQGFX_MEMORY_BUFFER_TABLE;
    if (table == 0)
    {
        return 0;
    }
    else
    {
        keyCopy = *(MemoryBufferKey_guess *)key;
        EQGFX_FIND_MEMORY_BUFFER_KEY(table, 0, &key, &keyCopy);
        table = EQGFX_MEMORY_BUFFER_TABLE;
        if (key == table->endNode)
        {
            EQGFX_INSERT_MEMORY_BUFFER_KEY(table, 0, resultPair, &keyCopy);
            return 0;
        }
        EQGFX_MEMORY_BUFFER_HITS++;
        return 1;
    }
}
