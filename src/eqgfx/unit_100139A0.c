// unit_100139A0.c - eqgfx_dx7.dll, the address run 0x100139A0..0x10014293 (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its file name are unknown).

#include <string.h>
#include "trilogy/eqaddr.h"

#define T3D_ERROR_INVALID_HSPRITE_REF_SET 0xffff3d3e

typedef struct T3DRenderer_guess
{
    unsigned char pad_00[0x8];
    int baseFrame;
} T3DRenderer_guess;

typedef struct HSpriteRefSet_guess
{
    unsigned char pad_0000[0x50];
    unsigned int referenceCount;
    unsigned int referenceCapacity;
    int *references;
} HSpriteRefSet_guess;

/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

typedef struct Dag_guess
{
    unsigned char pad[0xec];
    int numInSubDagList;
    void *subDagList;
    unsigned char pad2[0x14];
    int updateInterval;
    unsigned char pad3[8];
    float speed;
    unsigned char pad4[4];
    unsigned int flags;
} Dag_guess;

typedef struct Light_guess
{
    unsigned char pad[0x18];
    int updateInterval;
} Light_guess;

typedef struct T3DHierarchicalSpriteDefinition_guess
{
    unsigned char pad_00[0x18];
    float boundingRadius;
} T3DHierarchicalSpriteDefinition_guess;

typedef struct T3DLightDefinition_guess
{
    int typeId;
    unsigned char pad_04[4];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad_0d[3];
    int referenceCount;
    unsigned int frameCount;
    void *intensityList;
    void *colorList;
    int *initialState;
    void *owningLight;
} T3DLightDefinition_guess;

typedef struct T3DUndefineLightObject_guess
{
    int typeId;
    unsigned char pad_04[4];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad_0d[3];
    int pendingDestroy;
    int cleanupPolicy;
    void *ownedData;
    void *secondaryData;
    T3DLightDefinition_guess *definition;
} T3DUndefineLightObject_guess;

typedef struct T3DLight_guess
{
    unsigned char pad_00[0x8];
    void *dictionaryKey;
    unsigned char flags;
    unsigned char pad_0d[3];
    int pendingDestroy;
    unsigned int currentFrame;
    unsigned int frameValue;
    unsigned int adjustedFrame;
    unsigned char pad_20[4];
    T3DLightDefinition_guess *definition;
} T3DLight_guess;

typedef struct T3DDag_guess
{
    unsigned char pad_00[0x14];
    void *track;
    void *pendingTrack;
    unsigned char pad_1c[0xe4];
    unsigned short thisFrame;
    unsigned short nextFrame;
    float frameTime;
    unsigned char pad_108[4];
    unsigned int time;
    unsigned int frameState110;
    float speed;
    unsigned int frameState118;
    unsigned int oneCycle;
    unsigned char pad_120[0x08];
    unsigned short transitionFrame;
    unsigned short transitionNextFrame;
    float transitionFrameTime;
    unsigned char pad_130[4];
    unsigned int transitionTime;
    unsigned int transitionState138;
    float pendingSpeed;
    unsigned int transitionState140;
    unsigned int pendingInterpolate;
} T3DDag_guess;

typedef struct T3DTransitionState_guess
{
    unsigned int words[8];
} T3DTransitionState_guess;

#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)
#define T3D_FLOAT_EXPONENT_MASK 0x7f800000
#define T3D_FLOAT_FRACTION_MASK 0x007fffff
#define T3D_DAG_PENDING_INTERPOLATE_MASK 1
#define T3D_DAG_ONE_CYCLE_MASK 2

#define T3D_ERROR_INVALID_LIGHT 0xffff3d3e
#define T3D_ERROR_INVALID_FRAME 0xffff3d3e
#define T3D_LIGHT_TYPE_ID 0x1c
#define T3D_LIGHT_DEFINITION_TYPE_ID 0x1d

// FUNCTION: EQGFX 0x100139a0
// __cdecl. Returns the dag's index within its sub-dag list at +0xec.
int __cdecl t3dGetDagNumInSubDagList(Dag_guess *dag)
{
    return dag->numInSubDagList;
}

// FUNCTION: EQGFX 0x100139ab
// __cdecl. Returns the dag's sub-dag list pointer at +0xf0.
void *__cdecl t3dGetDagSubDagList(Dag_guess *dag)
{
    return dag->subDagList;
}

// FUNCTION: EQGFX 0x100139b6
// __cdecl. Returns the actor's world Z stored at +0x18.
float __cdecl t3dGetActorWorldZ(T3D_tagACTORINSTANCE *actor)
{
    return actor->position_guess[2];
}

// FUNCTION: EQGFX 0x100139BE
// Sets the bounding radius used for a hierarchical sprite definition.
void __cdecl t3dSetHierarchicalSpriteDefinitionBoundingRadius(T3DHierarchicalSpriteDefinition_guess *definition, float boundingRadius)
{
    definition->boundingRadius = boundingRadius;
    return;
}

// FUNCTION: EQGFX 0x100139E6
// Reports whether the DAG animation is set to play in reverse.
unsigned int __cdecl t3dGetDagReverse(T3DDag_guess *dag)
{
    unsigned int speedBits;
    unsigned int thresholdBits;

    speedBits = *(unsigned int *)&dag->speed;
    thresholdBits = *(unsigned int *)&EQGFX_ZERO_F_GUESS;
    if ((speedBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
        (speedBits & T3D_FLOAT_FRACTION_MASK) != 0)
        return 1;
    if ((thresholdBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
        (thresholdBits & T3D_FLOAT_FRACTION_MASK) != 0)
        return 1;
    if (dag->speed < EQGFX_ZERO_F_GUESS)
        return 1;
    return 0;
}

// FUNCTION: EQGFX 0x10013A02
unsigned int __cdecl t3dSetDagReverse(T3DDag_guess *dag, unsigned int reverse)
{
    unsigned int speedBits;
    unsigned int thresholdBits;
    int speedIsBelowThreshold;

    speedBits = *(unsigned int *)&dag->speed;
    thresholdBits = *(unsigned int *)&EQGFX_ZERO_F_GUESS;
    speedIsBelowThreshold = dag->speed < EQGFX_ZERO_F_GUESS;
    if (((speedBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
         (speedBits & T3D_FLOAT_FRACTION_MASK) != 0) ||
        ((thresholdBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
         (thresholdBits & T3D_FLOAT_FRACTION_MASK) != 0))
        speedIsBelowThreshold = 1;
    if (speedIsBelowThreshold != reverse)
        dag->speed = -dag->speed;
    return 0;
}

// FUNCTION: EQGFX 0x10013A35
unsigned int __cdecl t3dSetDagPendingReverse(T3DDag_guess *dag, unsigned int reverse)
{
    unsigned int pendingSpeedBits;
    unsigned int thresholdBits;
    int pendingSpeedIsBelowThreshold;

    pendingSpeedBits = *(unsigned int *)&dag->pendingSpeed;
    thresholdBits = *(unsigned int *)&EQGFX_ZERO_F_GUESS;
    pendingSpeedIsBelowThreshold = dag->pendingSpeed < EQGFX_ZERO_F_GUESS;
    if (((pendingSpeedBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
         (pendingSpeedBits & T3D_FLOAT_FRACTION_MASK) != 0) ||
        ((thresholdBits & T3D_FLOAT_EXPONENT_MASK) == T3D_FLOAT_EXPONENT_MASK &&
         (thresholdBits & T3D_FLOAT_FRACTION_MASK) != 0))
        pendingSpeedIsBelowThreshold = 1;
    if (pendingSpeedIsBelowThreshold != reverse)
        dag->pendingSpeed = -dag->pendingSpeed;
    return 0;
}

// FUNCTION: EQGFX 0x10013A91
unsigned int __cdecl t3dSetDagPendingInterpolate(T3DDag_guess *dag, unsigned int interpolate)
{
    unsigned int *pendingInterpolate;
    unsigned int pendingValue;

    pendingInterpolate = &dag->pendingInterpolate;
    pendingValue = *pendingInterpolate;
    *pendingInterpolate = (pendingValue ^ interpolate) & T3D_DAG_PENDING_INTERPOLATE_MASK ^ pendingValue;
    return 0;
}

// FUNCTION: EQGFX 0x10013ABC
// Sets whether the DAG animation plays for one cycle.
unsigned int __cdecl t3dSetDagOneCycle(T3DDag_guess *dag, unsigned int oneCycle)
{
    dag->oneCycle = ((oneCycle & 1) << 1) | (dag->oneCycle & ~T3D_DAG_ONE_CYCLE_MASK);
    return 0;
}

// FUNCTION: EQGFX 0x10013ADA
// Sets whether the DAG's pending animation plays for one cycle.
unsigned int __cdecl t3dSetDagPendingOneCycle(T3DDag_guess *dag, unsigned int oneCycle)
{
    dag->pendingInterpolate = ((oneCycle & 1) << 1) | (dag->pendingInterpolate & ~T3D_DAG_ONE_CYCLE_MASK);
    return 0;
}

// FUNCTION: EQGFX 0x10013AF8
// Reports the cycle count for the DAG's active animation track.
unsigned int __cdecl t3dGetDagNumCycles(T3DRenderer_guess *renderer, T3DDag_guess *dag)
{
    T3DTransitionState_guess currentState;

    if (dag->pendingTrack == 0)
        return dag->frameState118;
    currentState = *(T3DTransitionState_guess *)&dag->thisFrame;
    EQ_CDECL(void, 0x1001D251, (void *, T3DTransitionState_guess *, int))(
        dag->track, &currentState, renderer->baseFrame);
    return currentState.words[6];
}

// FUNCTION: EQGFX 0x10013B3B
// Reports the cycle count for the DAG's incoming animation track.
unsigned int __cdecl t3dGetDagPendingNumCycles(T3DRenderer_guess *renderer, T3DDag_guess *dag)
{
    T3DTransitionState_guess transitionState;

    transitionState = *(T3DTransitionState_guess *)&dag->transitionFrame;
    EQ_CDECL(void, 0x1001D251, (void *, T3DTransitionState_guess *, int))(
        dag->pendingTrack, &transitionState, renderer->baseFrame);
    return transitionState.words[6];
}

// FUNCTION: EQGFX 0x10013B70
// Restarts the DAG's current animation from its first frame at a chosen time.
unsigned int __cdecl t3dRestartDagFromThisTime(T3DDag_guess *dag, unsigned int restartTime)
{
    unsigned int frameCount;

    frameCount = EQ_CDECL(unsigned int, 0x1001CE1E, (void *))(dag->track);
    dag->thisFrame = 0;
    dag->nextFrame = 1;
    if (frameCount <= 1)
    {
        dag->nextFrame = 0;
    }
    dag->frameState110 = 0;
    dag->frameState118 = 0;
    dag->time = restartTime;
    dag->frameTime = 0.0f;
    return 0;
}

// FUNCTION: EQGFX 0x10013bbc
// Starts the DAG's transition to its pending animation track.
void __cdecl t3dDagStartTrackTransition_guess(T3DDag_guess *dag, unsigned int transitionTime)
{
    unsigned int frameCount;

    frameCount = EQ_CDECL(unsigned int, 0x1001CE1E, (void *))(dag->pendingTrack);
    dag->transitionFrame = 0;
    dag->transitionNextFrame = 1;
    if (frameCount <= 1)
    {
        dag->transitionNextFrame = 0;
    }
    dag->transitionState138 = 0;
    dag->transitionState140 = 0;
    dag->transitionTime = transitionTime;
    dag->transitionFrameTime = 0.0f;
}

// FUNCTION: EQGFX 0x10013C06
// Moves the DAG's current animation to a chosen frame at a chosen time.
int __cdecl t3dSetDagToThisFrameAndTime(T3DDag_guess *dag, unsigned int frameNumber, unsigned int time)
{
    unsigned int frameCount;

    frameCount = EQ_CDECL(unsigned int, 0x1001CE1E, (void *))(dag->track);
    if (frameNumber < frameCount)
    {
        dag->thisFrame = (unsigned short)frameNumber;
        dag->nextFrame = (unsigned short)(frameNumber + 1);
        if (frameCount <= ((frameNumber + 1) & 0xffff))
        {
            dag->nextFrame = 0;
        }
        dag->frameState110 = 0;
        dag->frameState118 = 0;
        dag->time = time;
        dag->frameTime = 0.0f;
        return 0;
    }
    return T3D_ERROR_INVALID_FRAME;
}

// FUNCTION: EQGFX 0x10013C6F
// Sets playback speed for the DAG's active animation.
int __cdecl t3dSetDagSpeed(T3DDag_guess *dag, float speed)
{
    dag->speed = speed;
    return 0;
}

// FUNCTION: EQGFX 0x10013c80
// Sets playback speed for the DAG's pending animation.
int __cdecl t3dSetDagPendingSpeed(T3DDag_guess *dag, float pendingSpeed)
{
    dag->pendingSpeed = pendingSpeed;
    return 0;
}

// FUNCTION: EQGFX 0x100139ca
// __cdecl. Returns the dag's update interval at +0x108.
int __cdecl t3dGetDagUpdateInterval(Dag_guess *dag)
{
    return dag->updateInterval;
}

// FUNCTION: EQGFX 0x100139d5
// __cdecl. Sets the dag's update interval at +0x108.
int __cdecl t3dSetDagUpdateInterval(Dag_guess *dag, int interval)
{
    dag->updateInterval = interval;
    return 0;
}

// FUNCTION: EQGFX 0x10013a68
// __cdecl. Returns bit 0 of the dag flags at +0x11c.
unsigned int __cdecl t3dGetDagInterpolate(Dag_guess *dag)
{
    return dag->flags & 1;
}

// FUNCTION: EQGFX 0x10013a76
// __cdecl. Sets bit 0 of the dag flags at +0x11c to bit 0 of interpolate.
int __cdecl t3dSetDagInterpolate(Dag_guess *dag, unsigned int interpolate)
{
    unsigned int flags;

    flags = dag->flags;
    dag->flags = (flags ^ interpolate) & 1 ^ flags;
    return 0;
}

// FUNCTION: EQGFX 0x10013aac
// __cdecl. Returns bit 1 of the dag flags at +0x11c.
unsigned int __cdecl t3dGetDagOneCycle(Dag_guess *dag)
{
    return (dag->flags >> 1) & 1;
}

// FUNCTION: EQGFX 0x10013c64
// __cdecl. Returns the dag speed at +0x114.
float __cdecl t3dGetDagSpeed(Dag_guess *dag)
{
    return dag->speed;
}

// FUNCTION: EQGFX 0x10013c91
// Adds an object to a sprite reference set if it is not already tracked.
int __cdecl t3dAddObjectToHSpriteRefSet(HSpriteRefSet_guess *refSet, int object)
{
    int result;
    unsigned int index;
    unsigned int count;
    int *references;
    int newCapacity;
    int *newReferences;

    result = 0;
    index = 0;
    count = refSet->referenceCount;
    if (count != 0)
    {
        references = refSet->references;
        do
        {
            if (*references == object)
                return 0;
            index++;
            references++;
        } while (index < count);
    }
    if (count == refSet->referenceCapacity)
    {
        if (refSet->references == 0)
        {
            newCapacity = 5;
            newReferences = EQ_CDECL(int *, 0x10050C48, (int))(0x14);
        }
        else
        {
            newCapacity = refSet->referenceCapacity + 5;
            newReferences = EQ_CDECL(int *, 0x10050C8C, (int *, int))(
                refSet->references, newCapacity * 4);
        }
        refSet->references = newReferences;
        if (newReferences == 0)
        {
            result = T3D_ERROR_INVALID_HSPRITE_REF_SET;
            goto done;
        }
        refSet->referenceCapacity = newCapacity;
    }
    refSet->references[count] = object;
    refSet->referenceCount++;

done:
    return result;
}

// FUNCTION: EQGFX 0x10013d10
// Removes an object from the references tracked by a hierarchical sprite.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Removes `object` from the int list at +0x58,
// shifting the tail down with memcpy; the count at +0x50 is decremented. Returns 0 on success,
// 0xFFFF3D3E when the list is empty or holds no match.
int __cdecl t3dRemoveObjectFromHSpriteRefSet(int *refSet, int object)
{
    unsigned int index;
    unsigned int count;
    int *refs;
    int *cursor;

    index = 0;
    count = *(unsigned int *)((char *)refSet + 0x50);
    if (count > 0)
    {
        refs = *(int **)((char *)refSet + 0x58);
        cursor = refs;
        do
        {
            if (*cursor == object)
            {
                if (index != count - 1)
                    memcpy(&refs[index], &refs[index + 1], (count - index - 1) * sizeof(int));
                (*(int *)((char *)refSet + 0x50))--;
                return 0;
            }
            index++;
            cursor++;
        } while (index < count);
    }
    return 0xFFFF3D3E;
}

// FUNCTION: EQGFX 0x10013d6d
// __cdecl. Returns the light registered under name, else defines and registers one; 0 on a type mismatch or failure.
T3DUndefineLightObject_guess *__cdecl t3dDefineLight(int renderer, char *name, int count, void *values,
    unsigned int frameCount, void *intensityList, int *initialState)
{
    T3DUndefineLightObject_guess *light;
    T3DLightDefinition_guess *definition;
    size_t nameSize;
    char *nameCopy;
    void *destination;
    size_t dataSize;
    int result;

    light = (T3DUndefineLightObject_guess *)EQ_CDECL(void *, 0x1000E7FE, (int, char *))(renderer, name);
    if (light != 0)
    {
        if (light->typeId != T3D_LIGHT_TYPE_ID)
        {
            return 0;
        }
        return light;
    }
    if (count == 0)
    {
        return 0;
    }
    light = (T3DUndefineLightObject_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x24);
    if (light == 0)
    {
        return 0;
    }
    *((int *)&light->flags) = 0;
    light->typeId = T3D_LIGHT_TYPE_ID;
    light->pendingDestroy = 1;
    light->cleanupPolicy = count;
    nameSize = EQ_CDECL(size_t, 0x10068160, (char *))(name);
    nameCopy = (char *)EQ_CDECL(void *, 0x10050C48, (int))(nameSize + 1);
    light->dictionaryKey = nameCopy;
    if (nameCopy == 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(light);
        return 0;
    }
    EQ_CDECL(void, 0x10068070, (char *, char *))(nameCopy, name);
    if (count == 1)
    {
        destination = &light->ownedData;
        dataSize = 4;
    }
    else
    {
        dataSize = count << 2;
        destination = EQ_CDECL(void *, 0x10050C48, (int))(dataSize);
        light->ownedData = destination;
        if (destination == 0)
        {
            goto freeNameAndLight;
        }
    }
    EQ_CDECL(void *, 0x10067D30, (void *, void *, size_t))(destination, values, dataSize);
    light->secondaryData = 0;
    result = EQ_CDECL(int, 0x1001564B, (int, void *))(renderer, light);
    if (result == 0)
    {
        result = EQ_CDECL(int, 0x1000DC08, (int, void *))(renderer, light);
        if (result < 0)
        {
            EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, light);
        }
        else
        {
            definition = (T3DLightDefinition_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x28);
            if (definition == 0)
            {
                EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, name);
                EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, light);
            }
            else
            {
                light->definition = definition;
                definition->dictionaryKey = 0;
                *((int *)&definition->flags) = 0;
                definition->frameCount = frameCount;
                definition->colorList = 0;
                definition->typeId = T3D_LIGHT_DEFINITION_TYPE_ID;
                definition->referenceCount = 1;
                definition->intensityList = intensityList;
                definition->initialState = initialState;
                definition->owningLight = light;
                result = EQ_CDECL(int, 0x1001564B, (int, void *))(renderer, definition);
                if (result == 0)
                {
                    return light;
                }
                EQ_CDECL(void, 0x10050CBB, (void *))(definition);
                EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, name);
                EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, light);
            }
        }
    }
    if (count != 1)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(light->ownedData);
    }
freeNameAndLight:
    EQ_CDECL(void, 0x10050CBB, (void *))(light->dictionaryKey);
    EQ_CDECL(void, 0x10050CBB, (void *))(light);
    return 0;
}

// FUNCTION: EQGFX 0x10013f01
// Discards a pending light together with its unshared definition.
int __cdecl t3dUndefineLight(int renderer, T3DUndefineLightObject_guess *light)
{
    T3DLightDefinition_guess *definition;
    int result;

    definition = light->definition;
    if (light->pendingDestroy == 1 && definition->referenceCount == 1)
    {
        if ((definition->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, definition->dictionaryKey);
        EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, definition);
        if (definition->dictionaryKey != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(definition->dictionaryKey);
        EQ_CDECL(void, 0x10050CBB, (void *))(definition);
        if ((light->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, light->dictionaryKey);
        result = EQ_CDECL(int, 0x10015D81, (int, void *))(renderer, light);
        if (light->cleanupPolicy != 1)
            EQ_CDECL(void, 0x10050CBB, (void *))(light->ownedData);
        if (light->secondaryData != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(light->secondaryData);
        if (light->dictionaryKey != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(light->dictionaryKey);
        EQ_CDECL(void, 0x10050CBB, (void *))(light);
    }
    else
    {
        result = T3D_ERROR_INVALID_LIGHT;
    }
    return result;
}

// FUNCTION: EQGFX 0x10013fad
// The purpose of this light-definition helper is unclear.
void __cdecl t3dGetLightDefinitionTagLength(T3DLightDefinition_guess *definition)
{
    if (definition->dictionaryKey == 0)
        EQ_CDECL(size_t, 0x10068160, (char *))(0);
}

// FUNCTION: EQGFX 0x10013fc0
// Reads the light definition's intensity values across its animation frames.
int __cdecl t3dGetLightDefinitionIntensityList(T3DLightDefinition_guess *definition, void *intensityList)
{
    void *source;
    size_t size;

    if (definition->frameCount == 1)
    {
        source = &definition->intensityList;
        size = 4;
    }
    else
    {
        size = definition->frameCount << 2;
        source = definition->intensityList;
    }
    EQ_CDECL(void *, 0x10067D30, (void *, void *, size_t))(intensityList, source, size);
    return 0;
}

// FUNCTION: EQGFX 0x10013fea
// Sets the light definition's intensity values across its animation frames.
int __cdecl t3dSetLightDefinitionIntensityList(T3DLightDefinition_guess *definition, void *intensityList)
{
    void *destination;
    size_t size;

    if (definition->frameCount == 1)
    {
        size = 4;
        destination = &definition->intensityList;
    }
    else
    {
        size = definition->frameCount << 2;
        destination = definition->intensityList;
    }
    EQ_CDECL(void *, 0x10067D30, (void *, void *, size_t))(destination, intensityList, size);
    return 0;
}

// FUNCTION: EQGFX 0x10014018
// Reads the light definition's per-frame RGB intensity values.
int __cdecl t3dGetLightDefinitionColorIntensityList(T3DLightDefinition_guess *definition, float *colorIntensityList)
{
    float intensity;
    unsigned int frameIndex;

    if (definition->colorList == 0)
    {
        if (definition->frameCount == 1)
        {
            intensity = *(float *)&definition->intensityList;
            colorIntensityList[0] = intensity;
            colorIntensityList[1] = intensity;
            colorIntensityList[2] = intensity;
        }
        else
        {
            frameIndex = 0;
            if (definition->frameCount != 0)
            {
                colorIntensityList = colorIntensityList + 2;
                do
                {
                    intensity = ((float *)definition->intensityList)[frameIndex];
                    frameIndex = frameIndex + 1;
                    colorIntensityList[-2] = intensity;
                    colorIntensityList[-1] = intensity;
                    *colorIntensityList = intensity;
                    colorIntensityList = colorIntensityList + 3;
                } while (frameIndex < definition->frameCount);
            }
        }
    }
    else
        EQ_CDECL(void *, 0x10067D30, (void *, void *, size_t))(colorIntensityList, definition->colorList, definition->frameCount * 12);
    return 0;
}

// FUNCTION: EQGFX 0x10014080
int __cdecl t3dSetLightDefinitionColorIntensityList(T3DLightDefinition_guess *definition, void *colorIntensityList)
{
    if (colorIntensityList != 0)
    {
        if (definition->colorList == 0)
        {
            definition->colorList = EQ_CDECL(void *, 0x10050C48, (int))(definition->frameCount * 12);
            if (definition->colorList == 0)
                return T3D_ERROR_INVALID_LIGHT;
        }
        EQ_CDECL(void *, 0x10067D30, (void *, void *, size_t))(
            definition->colorList, colorIntensityList, definition->frameCount * 12);
    }
    else
    {
        if (definition->colorList != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(definition->colorList);
    }
    return 0;
}

// FUNCTION: EQGFX 0x100140e1
// __cdecl. Returns 0 when the light definition's size field is 0x1c, else 0xFFFF3D3E.
int __cdecl t3dIsLightDefinitionValid(int *lightDef)
{
    int result;

    result = 0;
    if (*lightDef != 0x1c)
        result = 0xFFFF3D3E;
    return result;
}

// FUNCTION: EQGFX 0x100140f2
// Creates a light instance from a definition in the renderer.
T3DLight_guess *__cdecl t3dCreateLight(T3DRenderer_guess *renderer, T3DLightDefinition_guess *definition)
{
    T3DLight_guess *light;
    int *sourceWords;
    int *targetWords;
    int wordCount;
    int addResult;

    light = (T3DLight_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x28);
    if (light != 0)
    {
        sourceWords = definition->initialState;
        targetWords = (int *)light;
        for (wordCount = 10; wordCount != 0; wordCount = wordCount - 1)
        {
            *targetWords = *sourceWords;
            sourceWords = sourceWords + 1;
            targetWords = targetWords + 1;
        }
        light->dictionaryKey = 0;
        *((int *)&light->flags) = 0;
        light->pendingDestroy = 0;
        light->adjustedFrame = light->frameValue + renderer->baseFrame;
        addResult = EQ_CDECL(int, 0x1001564B, (T3DRenderer_guess *, T3DLight_guess *))(renderer, light);
        if (addResult == 0)
        {
            definition->referenceCount = definition->referenceCount + 1;
        }
        else
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(light);
            light = 0;
        }
    }
    return light;
}

// FUNCTION: EQGFX 0x1001414E
// __cdecl. Copies a light's state into a new block, registers it, returns the copy or 0.
T3DLight_guess *__cdecl t3dDuplicateLight(T3DRenderer_guess *renderer, T3DLight_guess *sourceLight)
{
    T3DLight_guess *light;
    int *sourceWords;
    int *targetWords;
    int wordCount;
    int addResult;

    light = (T3DLight_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x28);
    if (light != 0)
    {
        sourceWords = (int *)sourceLight;
        targetWords = (int *)light;
        for (wordCount = 10; wordCount != 0; wordCount = wordCount - 1)
        {
            *targetWords = *sourceWords;
            sourceWords = sourceWords + 1;
            targetWords = targetWords + 1;
        }
        light->dictionaryKey = 0;
        *((int *)&light->flags) = 0;
        light->pendingDestroy = 0;
        light->adjustedFrame = light->frameValue + renderer->baseFrame;
        addResult = EQ_CDECL(int, 0x1001564B, (T3DRenderer_guess *, T3DLight_guess *))(renderer, light);
        if (addResult != 0)
        {
            EQ_CDECL(void, 0x10050CBB, (void *))(light);
            light = 0;
        }
        else
        {
            light->definition->referenceCount = light->definition->referenceCount + 1;
        }
    }
    return light;
}

// FUNCTION: EQGFX 0x100141A6
// Removes a light instance and releases its reference to the light definition.
int __cdecl t3dDestroyLight(int renderer, T3DLight_guess *light)
{
    int result;

    if (light->pendingDestroy != 0)
    {
        result = 0xffff3d3e;
    }
    else
    {
        light->definition->referenceCount = light->definition->referenceCount + -1;
        if ((light->flags & 1) != 0)
            EQ_CDECL(void, 0x1000E334, (int, void *))(renderer, light->dictionaryKey);
        result = EQ_CDECL(int, 0x10015D81, (int, T3DLight_guess *))(renderer, light);
        if (light->dictionaryKey != 0)
            EQ_CDECL(void, 0x10050CBB, (void *))(light->dictionaryKey);
        EQ_CDECL(void, 0x10050CBB, (void *))(light);
    }
    return result;
}

// FUNCTION: EQGFX 0x100141FB
// Selects the animation frame used to evaluate a light.
int __cdecl t3dSetLightCurrentFrame(T3DLight_guess *light, unsigned int frame)
{
    if (light->definition->frameCount <= frame)
        return T3D_ERROR_INVALID_FRAME;
    light->currentFrame = frame;
    return 0;
}

// FUNCTION: EQGFX 0x10014219
// Reports the current frame's scalar intensity for a light.
float __cdecl t3dGetLightCurrentIntensity(T3DLight_guess *light)
{
    T3DLightDefinition_guess *definition = light->definition;

    if (definition->frameCount == 1)
    {
        float intensity;

        EQ_CDECL(void *, 0x10067D30, (void *, const void *, size_t))(
            &intensity, &definition->intensityList, sizeof(intensity));
        return intensity;
    }
    return ((float *)definition->intensityList)[light->currentFrame];
}

// FUNCTION: EQGFX 0x1001424a
// Reports the current frame's RGB intensity for a light.
unsigned int __cdecl t3dGetLightCurrentColorIntensity(T3DLight_guess *light, float *colorIntensity)
{
    T3DLightDefinition_guess *definition = light->definition;

    if (definition->colorList == 0)
    {
        float intensity;

        if (definition->frameCount == 1)
            intensity = *(float *)&definition->intensityList;
        else
            intensity = ((float *)definition->intensityList)[light->currentFrame];
        colorIntensity[0] = intensity;
        colorIntensity[1] = intensity;
        colorIntensity[2] = intensity;
    }
    else
    {
        float *frameColor = (float *)definition->colorList + light->currentFrame * 3;

        colorIntensity[0] = frameColor[0];
        colorIntensity[1] = frameColor[1];
        colorIntensity[2] = frameColor[2];
    }
    return 0;
}
// FUNCTION: EQGFX 0x10014293
// __cdecl. Sets the light's update interval at +0x18.
int __cdecl t3dSetLightUpdateInterval(Light_guess *light, int interval)
{
    light->updateInterval = interval;
    return 0;
}
