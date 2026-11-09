// unit_1001D1DE.c - eqgfx_dx7.dll, the address run 0x1001D1DE..0x1001E0DB (a queue unit: no Mac map covers
// eqgfx_dx7.dll, so the original object and its name are unknown).

typedef struct T3dAnimationFrameSource_guess
{
    unsigned char unknown_000[0x14];
    unsigned int *frame_count;
    unsigned int *fallback_frame_count;
} T3dAnimationFrameSource_guess;

typedef struct T3dAnimationOwner_guess
{
    unsigned char unknown_000[0x14];
    T3dAnimationFrameSource_guess *frame_source;
} T3dAnimationOwner_guess;

typedef struct T3dAnimationCycle_guess
{
    unsigned short current_frame;
    unsigned short next_frame;
    float progress;
    int cycle_duration;
    unsigned int last_update_time;
    int position;
    float speed;
    unsigned int cycle_count;
    unsigned int flags;
} T3dAnimationCycle_guess;

typedef struct T3dDag_guess
{
    unsigned char pad[8];
    void *sprite;
} T3dDag_guess;

typedef struct T3D_tagWORLD
{
    int type;
    void *unknown_004;
    unsigned char unknown_008[4];
    int worldDataEnd;
    int worldDataStart;
    void *unknown_014;
    unsigned char unknown_018[0xc];
    void *unknown_024;
    void *ai_callback;
    void *unknown_02c;
    unsigned char unknown_030[4];
    int unknown_034;
    int component_capacity_038;
    void *unknown_03c;
    void *create_context;
    void *unknown_044;
    void *unknown_048;
    int unknown_04c;
    unsigned char unknown_050[0xc];
    void *unknown_05c;
    unsigned char unknown_060[0xc];
    void *unknown_06c;
    int numSimpleSpriteDefinitions;
    unsigned char unknown_074[8];
    void *unknown_07c;
    unsigned char unknown_080[0xc];
    void *unknown_08c;
    unsigned char unknown_090[0xc];
    void *unknown_09c;
    unsigned char unknown_0a0[0xc];
    void *unknown_0ac;
    unsigned char unknown_0b0[0xc];
    void *unknown_0bc;
    unsigned char unknown_0c0[0xc];
    void *unknown_0cc;
    unsigned char unknown_0d0[0xc];
    void *unknown_0dc;
    unsigned char unknown_0e0[0xc];
    void *unknown_0ec;
    unsigned char unknown_0f0[4];
    unsigned int resourceCount_f4;
    unsigned char unknown_0f8[4];
    void *resourceData_fc;
    unsigned char unknown_100[4];
    unsigned int resourceCount_104;
    unsigned char unknown_108[4];
    void *resourceData_10c;
    unsigned char unknown_110[4];
    unsigned int resourceCount_114;
    unsigned char unknown_118[4];
    void *resourceData_11c;
    unsigned char unknown_120[4];
    unsigned int resourceCount_124;
    unsigned char unknown_128[4];
    void *unknown_12c;
    unsigned char unknown_130[4];
    unsigned int resourceCount_134;
    unsigned char unknown_138[4];
    void *resourceData_13c;
    unsigned char unknown_140[4];
    unsigned int resourceCount_144;
    unsigned char unknown_148[4];
    void *resourceData_14c;
    unsigned char unknown_150[4];
    unsigned int resourceCount_154;
    unsigned char unknown_158[4];
    void *resourceData_15c;
    unsigned char unknown_160[4];
    unsigned int resourceCount_164;
    unsigned char unknown_168[4];
    void *resourceData_16c;
    unsigned char unknown_170[4];
    unsigned int resourceCount_174;
    unsigned char unknown_178[4];
    void *resourceData_17c;
    unsigned char unknown_180[0xc];
    void *unknown_18c;
    unsigned char unknown_190[0xc];
    void *unknown_19c;
    unsigned char unknown_1a0[0xc];
    void *unknown_1ac;
    unsigned char unknown_1b0[4];
    unsigned int resourceCount_1b4;
    unsigned char unknown_1b8[4];
    void *resourceData_1bc;
    unsigned char unknown_1c0[0xc];
    void *unknown_1cc;
    unsigned char unknown_1d0[0xc];
    void *unknown_1dc;
    unsigned char unknown_1e0[0x3c];
    void *unknown_21c;
    unsigned char unknown_220[4];
    unsigned int resourceCount_224;
    unsigned char unknown_228[4];
    void *resourceData_22c;
    unsigned char unknown_230[0xc];
    void *unknown_23c;
    unsigned char unknown_240[0xc];
    void *unknown_24c;
    unsigned char unknown_250[0xc];
    void *unknown_25c;
    unsigned char unknown_260[0xc];
    void *unknown_26c;
    unsigned char unknown_270[0xc];
    void *unknown_27c;
    unsigned char unknown_280[0xc];
    void *unknown_28c;
    unsigned char unknown_290[0xc];
    void *unknown_29c;
    unsigned char unknown_2a0[0xc];
    void *unknown_2ac;
    unsigned char unknown_2b0[0xc];
    void *unknown_2bc;
    unsigned char unknown_2c0[0xc];
    void *unknown_2cc;
    unsigned char unknown_2d0[0xc];
    void *unknown_2dc;
    unsigned char unknown_2e0[0xc];
    void *unknown_2ec;
    unsigned char unknown_2f0[0xc];
    void *unknown_2fc;
    unsigned char unknown_300[0xc];
    void *unknown_30c;
    unsigned char unknown_310[0x14];
    void *unknown_324;
    int unknown_328;
    int unknown_32c;
    void *unknown_330;
    unsigned char unknown_334[4];
    int unknown_338;
    void *unknown_33c;
    unsigned int unknown_340;
    unsigned char referenceList[0x10];
    void *unknown_354;
    unsigned char unknown_358[4];
    void *unknown_35c;
    unsigned char state_360;
    unsigned char state_361;
    unsigned char state_362;
    unsigned char unknown_363;
    unsigned char state_364;
    unsigned char state_365;
    unsigned char state_366;
    unsigned char unknown_367;
    unsigned char unknown_368[0xc];
    void *unknown_374;
    int unknown_378;
    int unknown_37c;
} T3D_tagWORLD;


typedef struct T3dEdge_guess
{
    unsigned char pad[0xc];
    float length;
} T3dEdge_guess;

/* T3D_tagACTORINSTANCE: the actor layout is T3D_tagACTORINSTANCE (generated: eqgfx/layout/t3dactor.h); the old local view's members map by offset. */
#include "eqgfx/layout/t3dactor.h"

typedef struct T3dPlane_guess
{
    unsigned int words[5];
} T3dPlane_guess;

#include "trilogy/eqaddr.h"

#define T3D_ACTIVE_AI_CONTEXT ((void *)0x10086F80)

typedef struct T3dWorldActiveAIWorld_guess T3dWorldActiveAIWorld_guess;
typedef struct T3dWorldActiveAIObject_guess T3dWorldActiveAIObject_guess;
typedef struct T3dWorldActiveAIRecord_guess T3dWorldActiveAIRecord_guess;

typedef void (__cdecl *T3dWorldActiveAICallback_guess)(
    void *, T3dWorldActiveAIWorld_guess *, T3dWorldActiveAIObject_guess *);

typedef struct T3dWorldActiveAICallbackTable_guess
{
    char reserved_00[0x20];
    T3dWorldActiveAICallback_guess callback;
} T3dWorldActiveAICallbackTable_guess;

struct T3dWorldActiveAIObject_guess
{
    int type;
    char reserved_04[0x24];
    T3dWorldActiveAICallback_guess callback_type_3a;
    char reserved_2c[0x38];
    T3dWorldActiveAICallbackTable_guess *callback_table_type_18;
};

struct T3dWorldActiveAIRecord_guess
{
    char reserved_00[0x04];
    T3dWorldActiveAIRecord_guess *next;
    T3dWorldActiveAIObject_guess *object;
    unsigned int period;
    unsigned int next_execution_time;
};

struct T3dWorldActiveAIWorld_guess
{
    char reserved_00[0x08];
    unsigned int time;
    char reserved_0c[0x308];
    T3dWorldActiveAIRecord_guess *active_ai_head;
    char reserved_318[0x08];
    T3dWorldActiveAIRecord_guess *active_ai_next;
};

typedef struct T3dSphere_guess
{
    char reserved_00[0x04];
    struct T3dSphereDefinition_guess *definition;
    float radius;
} T3dSphere_guess;

typedef struct T3dSphylinder_guess
{
    unsigned int type;
    struct T3dSphereDefinition_guess *definition;
    float height;
    float radius;
} T3dSphylinder_guess;

typedef struct T3dSphereDefinition_guess
{
    unsigned int type;
    char reserved_04[0x0B];
    unsigned char flags;
} T3dSphereDefinition_guess;

typedef struct RenderVolume_guess
{
    int type;
} RenderVolume_guess;

#define T3D_DUPLICATE_VOLUME_DEFAULT_ADDR 0x100F401C
#define T3D_DUPLICATE_VOLUME_SPHYLINDER_ADDR 0x100A35C8
#define T3D_DUPLICATE_VOLUME_RADIUS_ADDR 0x100A35CC
#define T3D_DUPLICATE_VOLUME_XYZ_RADIUS_ADDR 0x100A35D0
#define T3D_DUPLICATE_PLANE_ADDR 0x1001D406
#define T3D_DUPLICATE_SPHERE_ADDR 0x1001D39A
#define T3D_DUPLICATE_POLYHEDRON_ADDR 0x10018D49
#define T3D_DUPLICATE_SPHYLINDER_ADDR 0x1001D447
#define T3D_DUPLICATE_XYZ_SPHYLINDER_ADDR 0x1001D4CF

typedef struct T3dVolumeRadiusData_guess
{
    char reserved_00[0x14];
    float radius;
} T3dVolumeRadiusData_guess;

typedef struct T3dVolumeBoundingRadius_guess
{
    int type;
    char reserved_04[0x04];
    float primary_extent;
    float secondary_extent;
    float tertiary_extent;
    float quaternary_extent;
    T3dVolumeRadiusData_guess *radius_data;
} T3dVolumeBoundingRadius_guess;

#define EQGFX_MOTION_HALF_GUESS EQ_GLOBAL(float, 0x10076290)

#define T3D_AI_STUB_CALLBACK_ADDR 0x100749DF

typedef struct T3dWorldCallbackTarget_guess
{
    char reserved_00[0x28];
    unsigned int callback;
} T3dWorldCallbackTarget_guess;

typedef struct T3dTrack_guess
{
    char reserved_00[0x18];
    unsigned short restart_state;
    unsigned short restart_mode;
    float elapsed_time;
    char reserved_20[0x04];
    unsigned int from_time;
    unsigned int time_offset;
    float default_speed;
    unsigned int last_update_time;
    unsigned int flags;
} T3dTrack_guess;

typedef struct T3dWorldTime_guess
{
    unsigned int reserved_00;
    char *tag;
    unsigned int world_time;
} T3dWorldTime_guess;

typedef struct T3dXYZ_guess
{
    float x;
    float y;
    float z;
} T3dXYZ_guess;

typedef struct T3dXYZSphere_guess
{
    char reserved_00[0x04];
    T3dXYZ_guess center;
    float radius;
} T3dXYZSphere_guess;

typedef struct T3dXYZSphylinder_guess
{
    unsigned int type;
    T3dXYZ_guess center;
    unsigned int heightBits;
    unsigned int radiusBits;
} T3dXYZSphylinder_guess;

typedef struct T3dWorldMessage_guess
{
    char reserved_00[0x28];
    void (__cdecl *send_message)(unsigned int, struct T3dWorldMessage_guess *, struct T3dWorldMessage_guess *);
} T3dWorldMessage_guess;

// FUNCTION: EQGFX 0x1001d1de
// Sets whether an animation track interpolates between keyed values.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Sets bit 0
// (interpolate flag) at +0x34 of the track. Returns 0.
int __cdecl t3dSetTrackDefaultInterpolate(char *track, unsigned int interpolate)
{
    unsigned int flags = *(unsigned int *)(track + 0x34);

    *(unsigned int *)(track + 0x34) = ((flags ^ interpolate) & 1) ^ flags;
    return 0;
}

// FUNCTION: EQGFX 0x1001d1f6
// __cdecl.
int __cdecl t3dGetTrackDefaultOneCycle(T3dTrack_guess *track)
{
    return (track->flags >> 1) & 1;
}

// FUNCTION: EQGFX 0x1001d203
// Limits an animation track to one pass.
int __cdecl t3dSetTrackDefaultOneCycle(T3dTrack_guess *track, unsigned int oneCycle)
{
    track->flags = ((oneCycle & 1) << 1) | (track->flags & 0xfffffffd);
    return 0;
}

// FUNCTION: EQGFX 0x1001d21e
// Prepares an animation track to restart from the requested time.
int __cdecl t3dRestartTrackDefaultFromThisTime(T3dTrack_guess *track, unsigned int fromTime)
{
    track->restart_state = 0;
    track->time_offset = 0;
    track->elapsed_time = 0.0f;
    track->last_update_time = 0;
    track->restart_mode = 1;
    track->from_time = fromTime;
    return 0;
}

// FUNCTION: EQGFX 0x1001d243
// Sets the default playback speed of an animation track.
// __cdecl (the callee pops 0 bytes; ECX is not an input). Stores the default
// speed at +0x2c of the track. Returns 0.
int __cdecl t3dSetTrackDefaultSpeed(char *track, float defaultSpeed)
{
    *(float *)(track + 0x2c) = defaultSpeed;
    return 0;
}

#define T3D_ANIMATION_RANDOM_OFFSET() EQ_CDECL(int, 0x1004EC36, (void))()
#define EQGFX_ZERO_F_GUESS EQ_GLOBAL(float, 0x10076284)

// FUNCTION: EQGFX 0x1001d251
// Advances animation frames and interpolation over time, holding the final frame after one-cycle playback.
void __cdecl t3dUpdateAnimationCycle_guess(T3dAnimationOwner_guess *owner, T3dAnimationCycle_guess *cycle, unsigned int update_time)
{
    T3dAnimationFrameSource_guess *frame_source;
    unsigned int *frame_count_address;
    unsigned int total_frames;
    int cycle_duration;
    unsigned __int64 elapsed_ticks;
    int position;
    int step;
    int whole_cycles;
    int cycle_remainder;
    unsigned int current_frame;
    unsigned int frame_remainder;
    unsigned short next_frame;

    frame_source = owner->frame_source;
    frame_count_address = frame_source->frame_count;
    if (frame_count_address == 0)
        total_frames = *frame_source->fallback_frame_count;
    else
        total_frames = *frame_count_address;

    if (total_frames == 0)
    {
        position = (int)cycle;
        cycle->current_frame = 0;
        cycle->next_frame = 0;
        cycle->position = 0;
        cycle->progress = 0.0f;
    }
    else
    {
        cycle_duration = cycle->cycle_duration;
        if (cycle_duration < 1)
            goto update_last_time;

        position = cycle->position;
        elapsed_ticks = (unsigned int)(update_time - cycle->last_update_time);
        if (cycle->speed > EQGFX_ZERO_F_GUESS)
        {
            __asm
            {
                fild qword ptr [elapsed_ticks]
                mov eax, cycle
                fmul dword ptr [eax + 14h]
            }
            step = T3D_ANIMATION_RANDOM_OFFSET();
            position += step;
        }
        else
        {
            __asm
            {
                fild qword ptr [elapsed_ticks]
                mov eax, cycle
                fmul dword ptr [eax + 14h]
            }
            step = T3D_ANIMATION_RANDOM_OFFSET();
            position -= step;
        }

        if (position > cycle_duration)
        {
            whole_cycles = position / cycle_duration;
            cycle_remainder = position % cycle_duration;
            cycle->current_frame = (unsigned short)(cycle->current_frame + whole_cycles);
            current_frame = cycle->current_frame;
            if ((cycle->flags & 0x2) == 0)
                cycle->cycle_count += current_frame / total_frames;
            else if (total_frames - 1 <= current_frame)
                cycle->cycle_count++;

            frame_remainder = current_frame % total_frames;
            cycle->current_frame = (unsigned short)frame_remainder;
            if ((cycle->flags & 0x1) == 0)
                cycle->next_frame = (unsigned short)frame_remainder;
            else
            {
                next_frame = (unsigned short)(frame_remainder + 1);
                cycle->next_frame = next_frame;
                if (total_frames <= (next_frame & 0xffff))
                    cycle->next_frame = 0;
                cycle->progress = (float)cycle_remainder / (float)cycle_duration;
            }
            position = cycle_remainder;
            cycle->position = position;
        }
        else
        {
            if ((cycle->flags & 0x1) != 0)
                cycle->progress = (float)position / (float)cycle_duration;
            cycle->position = position;
        }
    }

    if ((cycle->flags & 0x2) != 0 && cycle->cycle_count != 0)
    {
        cycle->position = position;
        cycle->progress = 0.0f;
        cycle->current_frame = (unsigned short)(total_frames - 1);
        cycle->next_frame = cycle->current_frame;
    }

update_last_time:
    cycle->last_update_time = update_time;
}

// FUNCTION: EQGFX 0x1001d3c0
int __cdecl t3dSetSphereRadius(T3dSphere_guess *sphere, float radius)
{
    T3dSphereDefinition_guess *definition;

    sphere->radius = radius;
    definition = sphere->definition;
    if (definition != 0 && definition->type == 0x18)
        definition->flags |= 0x20;
    return 0;
}

#define T3D_ALLOCATE_MEMORY_ADDR 0x10050C48
#define T3D_FREE_MEMORY_ADDR 0x10050CBB
#define T3D_INIT_WORLD_ENTRY_ADDR 0x10014f27
#define T3D_INIT_REFERENCE_LIST_ADDR 0x10019012
#define T3D_INITIALIZE_WORLD_ADDR 0x10008ed9
#define T3D_CREATE_DICTIONARY_ADDR 0x1000dab8
#define T3D_DESTROY_WORLD_ADDR 0x1001dad3

// FUNCTION: EQGFX 0x1001d3de
// __cdecl. Returns the new sphere in EAX, or 0 when the allocation fails.
T3dXYZSphere_guess *__cdecl t3dCreateXYZSphere(const T3dXYZ_guess *center, unsigned int radiusBits)
{
    T3dXYZSphere_guess *sphere;

    sphere = (T3dXYZSphere_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x14);
    if (sphere != 0)
    {
        *(unsigned int *)sphere = 0x2b;
        sphere->center = *center;
        *(unsigned int *)&sphere->radius = radiusBits;
    }
    return sphere;
}

// FUNCTION: EQGFX 0x1001d406
// Creates an independent copy of a plane-shaped volume.
void __cdecl t3dDuplicatePlane(T3dPlane_guess *plane)
{
    unsigned int *duplicateWords;
    unsigned int *sourceWords;
    int remainingWords;

    duplicateWords = (unsigned int *)EQ_CDECL(void *, 0x10050C48, (int))(0x14);
    if (duplicateWords != 0)
    {
        sourceWords = plane->words;
        for (remainingWords = 5; remainingWords != 0; remainingWords = remainingWords - 1)
        {
            *duplicateWords = *sourceWords;
            sourceWords = sourceWords + 1;
            duplicateWords = duplicateWords + 1;
        }
    }
    return;
}

// FUNCTION: EQGFX 0x1001D422
// Creates a sphylinder volume with the requested height and radius.
// __cdecl. Returns a new 16-byte sphylinder (type 0x2C at +0, no definition, the height at +0x8, the radius at +0xC), or 0.
T3dSphylinder_guess *__cdecl t3dCreateSphylinder(float height, float radius)
{
    T3dSphylinder_guess *sphylinder;

    sphylinder = (T3dSphylinder_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x10);
    if (sphylinder != 0)
    {
        sphylinder->definition = 0;
        sphylinder->height = height;
        sphylinder->type = 0x2C;
        sphylinder->radius = radius;
    }
    return sphylinder;
}

// FUNCTION: EQGFX 0x1001D447
// Creates a separate sphylinder volume with the same height and radius.
void __cdecl t3dDuplicateSphylinder(T3dSphylinder_guess *sphylinder)
{
    T3dSphylinder_guess *duplicate;

    duplicate = (T3dSphylinder_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x10);
    if (duplicate != 0)
    {
        *duplicate = *sphylinder;
        duplicate->definition = 0;
    }
    return;
}

// FUNCTION: EQGFX 0x1001d466
// Releases a plane volume when it is no longer needed.
int __cdecl t3dDestroyPlane(void *plane)
{
    if (plane != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(plane);
    return 0;
}

// FUNCTION: EQGFX 0x1001d482
// Changes the radius of a sphylinder volume.
int __cdecl t3dSetSphylinderRadius(T3dSphylinder_guess *sphere, float radius)
{
    sphere->radius = radius;
    if (sphere->definition != 0 && sphere->definition->type == 0x18)
        sphere->definition->flags |= 0x20;
    return 0;
}

// FUNCTION: EQGFX 0x1001D4A0
// __cdecl. Returns the new sphylinder in EAX, or 0 when the allocation fails.
T3dXYZSphylinder_guess *__cdecl t3dCreateXYZSphylinder(const T3dXYZ_guess *center, unsigned int heightBits, unsigned int radiusBits)
{
    T3dXYZSphylinder_guess *sphylinder;

    sphylinder = (T3dXYZSphylinder_guess *)EQ_CDECL(void *, 0x10050C48, (int))(0x18);
    if (sphylinder != 0)
    {
        sphylinder->type = 0x2D;
        sphylinder->center = *center;
        sphylinder->heightBits = heightBits;
        sphylinder->radiusBits = radiusBits;
    }
    return sphylinder;
}

// FUNCTION: EQGFX 0x1001D4CF
// Copies a positioned sphylinder volume together with its position.
void __cdecl t3dDuplicateXYZSphylinder(unsigned int *sourceWords)
{
    unsigned int *duplicateWords;
    int remainingWords;

    duplicateWords = (unsigned int *)EQ_CDECL(void *, 0x10050C48, (int))(0x18);
    if (duplicateWords != 0)
    {
        for (remainingWords = 6; remainingWords != 0; remainingWords = remainingWords - 1)
        {
            *duplicateWords = *sourceWords;
            sourceWords = sourceWords + 1;
            duplicateWords = duplicateWords + 1;
        }
    }
    return;
}

// FUNCTION: EQGFX 0x1001d4eb
// Moves a spherical volume's center to a new position.
int __cdecl t3dSetXYZSphereCenter(T3dXYZSphere_guess *sphere, const T3dXYZ_guess *center)
{
    sphere->center = *center;
    return 0;
}

// FUNCTION: EQGFX 0x1001d508
int __cdecl t3dSetXYZSphereRadius(T3dXYZSphere_guess *sphere, float radius)
{
    sphere->radius = radius;
    return 0;
}

// FUNCTION: EQGFX 0x1001d516
// Duplicates a volume for use by sprites or actors, selecting the copy operation by shape type.
// @early-stop: VC6 merges the type-2/type-4 call bodies and places the shared zero return before the high-type results.
RenderVolume_guess *__cdecl t3dDuplicateVolume(void *context, RenderVolume_guess *volume)
{
    int type;
    RenderVolume_guess *duplicate;

    if (volume == 0)
        return (RenderVolume_guess *)T3D_DUPLICATE_VOLUME_DEFAULT_ADDR;

    type = volume->type;
    if (type <= 0x2C)
    {
        if (type != 0x2C)
        {
            switch (type)
            {
            case 0:
                return (RenderVolume_guess *)T3D_DUPLICATE_VOLUME_DEFAULT_ADDR;
            case 2:
                duplicate = EQ_CDECL(RenderVolume_guess *, 0x10018D49, (void *, RenderVolume_guess *))(context, volume);
                break;
            case 4:
                duplicate = EQ_CDECL(RenderVolume_guess *, 0x10018D49, (void *, RenderVolume_guess *))(context, volume);
                break;
            case 0x2A:
                return EQ_CDECL(RenderVolume_guess *, 0x1001D39A, (RenderVolume_guess *))(volume);
            case 0x2B:
                return EQ_CDECL(RenderVolume_guess *, 0x1001D406, (RenderVolume_guess *))(volume);
            default:
                duplicate = 0;
                break;
            }
            return duplicate;
        }
        else
        {
            return EQ_CDECL(RenderVolume_guess *, 0x1001D447, (RenderVolume_guess *))(volume);
        }
    }
    else
    {
        switch (type)
        {
        case 0x2D:
            return EQ_CDECL(RenderVolume_guess *, 0x1001D4CF, (RenderVolume_guess *))(volume);
        case 0x2E:
            return (RenderVolume_guess *)T3D_DUPLICATE_VOLUME_SPHYLINDER_ADDR;
        case 0x52:
            return (RenderVolume_guess *)T3D_DUPLICATE_VOLUME_RADIUS_ADDR;
        case 0x54:
            return (RenderVolume_guess *)T3D_DUPLICATE_VOLUME_XYZ_RADIUS_ADDR;
        default:
            return 0;
        }
    }
}

// FUNCTION: EQGFX 0x1001d5a1
// Cleans up a volume and any resources required by its shape.
// @early-stop: VC6 folds the original's duplicate plane and polyhedron call blocks into shared switch arms.
int __cdecl t3dDestroyVolume(void *context, RenderVolume_guess *volume)
{
    int result = 0;

    if (volume != 0)
    {
        int type = volume->type;

        switch (type)
        {
        case 0:
            return 0;
        case 0x2B:
            result = EQ_CDECL(int, 0x1001D466, (RenderVolume_guess *))(volume);
            return result;
        case 0x2A:
            return EQ_CDECL(int, 0x1001D466, (RenderVolume_guess *))(volume);
        case 4:
            result = EQ_CDECL(int, 0x10018DA3, (void *, RenderVolume_guess *))(context, volume);
            break;
        case 2:
            return EQ_CDECL(int, 0x10018DA3, (void *, RenderVolume_guess *))(context, volume);
        case 0x2C:
            return EQ_CDECL(int, 0x1001D466, (RenderVolume_guess *))(volume);
        case 0x2D:
            result = EQ_CDECL(int, 0x1001D466, (RenderVolume_guess *))(volume);
            break;
        case 0x2E:
        case 0x52:
        case 0x54:
            break;
        default:
            return -0xC2C2;
        }
    }
    return result;
}

// FUNCTION: EQGFX 0x1001d618
// Gets the enclosing radius used to bound a volume as one sphere.
float __cdecl t3dGetVolumeBoundingRadius(T3dVolumeBoundingRadius_guess *volume)
{
    int type;

    if (volume != 0)
    {
        type = volume->type;
        if (type < 0x2D)
        {
            if (type == 0x2C)
                return volume->primary_extent * EQGFX_MOTION_HALF_GUESS + volume->secondary_extent;
            if (type != 0)
            {
                if ((type == 2) || (type == 4))
                    return volume->radius_data->radius * volume->quaternary_extent;
                if (type == 0x2A)
                    return volume->primary_extent;
                if (type == 0x2B)
                    return volume->tertiary_extent;
            }
        }
        else if (type == 0x2D)
            return volume->tertiary_extent * EQGFX_MOTION_HALF_GUESS + volume->quaternary_extent;
    }
    return 0.0f;
}

// FUNCTION: EQGFX 0x1001d67e
// Creates an empty renderer world with resource storage, a reference list and a name dictionary.
T3D_tagWORLD *__cdecl t3dCreateWorld(void *context)
{
    T3D_tagWORLD *world;
    void *auxiliaryData;
    int initializationResult;

    world = (T3D_tagWORLD *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x380);
    if (world != 0)
    {
        world->unknown_340 |= 0xffffffff;
        world->type = 0x3a;
        world->create_context = context;
        world->unknown_048 = 0;
        world->unknown_32c = 0;
        world->unknown_328 = 0;
        world->unknown_330 = 0;
        world->unknown_04c = 0;
        world->ai_callback = (void *)T3D_AI_STUB_CALLBACK_ADDR;
        world->unknown_02c = 0;
        world->worldDataEnd = 0;
        world->worldDataStart = 0;
        world->unknown_338 = 0;
        world->unknown_33c = 0;
        auxiliaryData = EQ_CDECL(void *, 0x10050C48, (unsigned int))(0x20);
        world->unknown_03c = auxiliaryData;
        if (auxiliaryData != 0)
        {
            world->state_362 &= 0;
            world->state_361 &= 0;
            world->state_360 &= 0;
            world->state_366 &= 0;
            world->state_365 &= 0;
            world->state_364 &= 0;
            world->component_capacity_038 = 8;
            world->unknown_034 = 0;
            if (((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 3, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 1, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 5, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 7, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 9, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0xb, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x11, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x13, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x15, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1c, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 4, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 2, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 6, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 8, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0xa, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0xc, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x12, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x14, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x16, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x17, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x18, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x4c, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x4d, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x19, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1a, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1b, 0x251c)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1d, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1e, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x1f, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x23, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x24, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x44, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10019012, (void *, int))((void *)world->referenceList, 100)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x45, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x46, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x47, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x48, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x49, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x4a, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x4e, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x4f, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x50, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x51, 0x14)) == 0) &&
                ((initializationResult = EQ_CDECL(int, 0x10014F27, (T3D_tagWORLD *, int, int))(world, 0x53, 0x14)) == 0))
            {
                world->unknown_378 = 0;
                world->unknown_37c = 0;
                world->unknown_354 = 0;
                world->unknown_35c = 0;
                initializationResult = EQ_CDECL(int, 0x10008ED9, (T3D_tagWORLD *))(world);
                if ((initializationResult == 0) &&
                    ((initializationResult = EQ_CDECL(int, 0x1000DAB8, (T3D_tagWORLD *))(world)) == 0))
                {
                    world->unknown_044 = 0;
                    return world;
                }
            }
        }
        EQ_CDECL(void, 0x1001DAD3, (T3D_tagWORLD *))(world);
    }
    return 0;
}

// FUNCTION: EQGFX 0x1001dad3
// Destroys a world: frees its per-type resources, reference list, name dictionary and the world itself
int __cdecl t3dDestroyWorld(T3D_tagWORLD *world)
{
    if (world->unknown_03c != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_03c);
    if (world->unknown_35c != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_35c);
    if (world->unknown_324 != 0)
        EQ_CDECL(void, 0x100092E3, (T3D_tagWORLD *))(world);
    if (world->resourceData_17c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x18);
    if (world->unknown_0dc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x17);
    if (world->resourceCount_174 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_17c);
        world->resourceCount_174 = 0;
    }
    if (world->resourceData_15c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x12);
    if (world->unknown_0bc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x11);
    if (world->resourceCount_154 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_15c);
        world->resourceCount_154 = 0;
    }
    if (world->resourceData_16c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x14);
    if (world->unknown_0cc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x13);
    if (world->resourceCount_164 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_16c);
        world->resourceCount_164 = 0;
    }
    if (world->resourceData_22c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x16);
    if (world->unknown_21c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x15);
    if (world->resourceCount_224 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_22c);
        world->resourceCount_224 = 0;
    }
    if (world->resourceData_14c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x0c);
    if (world->unknown_0ac != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x0b);
    if (world->resourceCount_144 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_14c);
        world->resourceCount_144 = 0;
    }
    if (world->resourceData_13c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 10);
    if (world->unknown_09c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 9);
    if (world->resourceCount_134 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_13c);
        world->resourceCount_134 = 0;
    }
    if (world->unknown_1cc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1e);
    if (world->unknown_1dc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1f);
    if (world->unknown_24c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x44);
    EQ_CDECL(void, 0x100191BD, (void *))(world->referenceList);
    if (world->unknown_354 != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_354);
    if (world->unknown_33c != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_33c);
    if (world->unknown_28c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x48);
    if (world->unknown_27c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x47);
    if (world->unknown_2ac != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x4a);
    if (world->unknown_29c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x49);
    if (world->unknown_2cc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x4f);
    if (world->unknown_2bc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x4e);
    if (world->unknown_2dc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x50);
    if (world->unknown_2ec != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x51);
    if (world->unknown_26c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x46);
    if (world->unknown_25c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x45);
    if (world->unknown_23c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x4d);
    if (world->unknown_0ec != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x4c);
    if (world->unknown_12c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 8);
    if (world->unknown_08c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 7);
    if (world->resourceCount_124 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_12c);
        world->resourceCount_124 = 0;
    }
    if (world->unknown_024 != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x19);
    if (world->resourceData_1bc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1d);
    if (world->resourceData_11c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 6);
    if (world->unknown_07c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 5);
    if (world->resourceCount_114 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_11c);
        world->resourceCount_114 = 0;
    }
    if (world->unknown_18c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1a);
    if (world->unknown_19c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1b);
    if (world->unknown_1ac != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x1c);
    if (world->resourceCount_1b4 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_1bc);
        world->resourceCount_1b4 = 0;
    }
    if (world->resourceData_10c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 2);
    if (world->unknown_06c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 1);
    if (world->resourceCount_104 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_10c);
        world->resourceCount_104 = 0;
    }
    if (world->resourceData_fc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 4);
    if (world->unknown_05c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 3);
    if (world->resourceCount_f4 > 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->resourceData_fc);
        world->resourceCount_f4 = 0;
    }
    if (world->unknown_30c != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x24);
    if (world->unknown_2fc != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x23);
    if (world->unknown_374 != 0)
        EQ_CDECL(void, 0x10016319, (void *, int))(world, 0x53);
    if (world->unknown_330 != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_330);
    if (world->unknown_048 != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_048);
    if (world->unknown_004 != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_004);
    EQ_CDECL(void, 0x1000DBBE, (void *))(world);
    EQ_CDECL(void, 0x10050CBB, (void *))((void *)(world->worldDataEnd - world->worldDataStart));
    if (world->unknown_014 != 0)
        EQ_CDECL(void, 0x10050CBB, (void *))(world->unknown_014);
    EQ_CDECL(void, 0x10050CBB, (void *))(world);
    return 0;
}

// FUNCTION: EQGFX 0x1001DFC3
// Runs scheduled AI callbacks for active world objects when they are due.
int __cdecl t3dExecuteWorldActiveAIs(T3dWorldActiveAIWorld_guess *world)
{
    T3dWorldActiveAIRecord_guess *active_ai;
    T3dWorldActiveAIObject_guess *object;

    active_ai = world->active_ai_head;
    while (active_ai != 0)
    {
        world->active_ai_next = active_ai->next;
        if (world->time >= active_ai->next_execution_time)
        {
            object = active_ai->object;
            if (object != 0)
            {
                if (object->type != 0x18)
                    {
                    if (object->type == 0x3A)
                    object->callback_type_3a(T3D_ACTIVE_AI_CONTEXT, world, object);
                }
                else object->callback_table_type_18->callback(T3D_ACTIVE_AI_CONTEXT, world, object);
            }
            active_ai->next_execution_time = active_ai->period + world->time;
        }
        active_ai = world->active_ai_next;
    }
    return 1;
}

// FUNCTION: EQGFX 0x1001e02b
// Sets the callback invoked by a world.
int __cdecl t3dSetWorldCallback(T3dWorldCallbackTarget_guess *world, int callback)
{
    if (callback != 0)
        world->callback = callback;
    else
        world->callback = T3D_AI_STUB_CALLBACK_ADDR;
    return 0;
}

// FUNCTION: EQGFX 0x1001e04a
// Passes a message to the world's message handler.
void __cdecl t3dSendMessageToWorld(T3dWorldMessage_guess *world, unsigned int message)
{
    world->send_message(message, world, world);
}

// FUNCTION: EQGFX 0x1001e063
// Sets the current time value for a world.
int __cdecl t3dSetWorldTime(T3dWorldTime_guess *world, unsigned int world_time)
{
    world->world_time = world_time;
    return 0;
}

// FUNCTION: EQGFX 0x1001e071
// Gets the character count of a world's tag.
unsigned int __cdecl t3dGetWorldTagLength(T3dWorldTime_guess *world)
{
    return EQ_CDECL(unsigned int, 0x10068160, (char *))(world->tag);
}

// FUNCTION: EQGFX 0x1001e07f
// Copies a world's tag into a text buffer.
int __cdecl t3dGetWorldTag(T3dWorldTime_guess *world, char *tag_buffer)
{
    EQ_CDECL(void, 0x10068070, (char *, char *))(tag_buffer, world->tag);
    return 0;
}

// FUNCTION: EQGFX 0x1001D37C
// __cdecl. Returns a new 12-byte sphere (type 0x2A at +0, no definition, the radius at +0x8), or 0.
T3dSphere_guess *__cdecl t3dCreateSphere(float radius)
{
    T3dSphere_guess *sphere;

    sphere = (T3dSphere_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0xC);
    if (sphere != 0)
    {
        sphere->definition = 0;
        *(unsigned int *)sphere->reserved_00 = 0x2A;
        sphere->radius = radius;
    }
    return sphere;
}

// FUNCTION: EQGFX 0x1001D39A
// Creates a separate spherical volume with the same radius.
void __cdecl t3dDuplicateSphere(T3dSphere_guess *sphere)
{
    T3dSphere_guess *duplicate;

    duplicate = (T3dSphere_guess *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(0xC);
    if (duplicate != 0)
    {
        *duplicate = *sphere;
        duplicate->definition = 0;
    }
    return;
}

// FUNCTION: EQGFX 0x1001d3b8
// __cdecl. Returns the radius at +0x8 of the sphere.
float __cdecl t3dGetSphereRadius(T3dSphere_guess *sphere)
{
    return sphere->radius;
}

// FUNCTION: EQGFX 0x1001d47a
// __cdecl. Returns the length at +0xc of the edge.
float __cdecl t3dGetEdgeLength(T3dEdge_guess *edge)
{
    return edge->length;
}

// FUNCTION: EQGFX 0x1001d500
// __cdecl. Returns the world X at +0x10 of the actor.
float __cdecl t3dGetActorWorldX(T3D_tagACTORINSTANCE *actor)
{
    return actor->position_guess[0];
}

// FUNCTION: EQGFX 0x1001e05b
// __cdecl. Returns the sprite at +0x8 of the dag.
void *__cdecl t3dGetDagSprite(T3dDag_guess *dag)
{
    return dag->sprite;
}

// FUNCTION: EQGFX 0x1001e094
// __cdecl. Frees the old tag, stores a copy of new_tag and returns 0, or 0xffff3d41 when the allocation fails.
int __cdecl t3dSetWorldTag(T3dWorldTime_guess *world, char *new_tag)
{
    int result;
    char *tag_buffer;

    result = 0;
    if (world->tag != 0)
    {
        EQ_CDECL(void, 0x10050CBB, (void *))(world->tag);
    }
    tag_buffer = (char *)EQ_CDECL(void *, 0x10050C48, (unsigned int))(EQ_CDECL(unsigned int, 0x10068160, (char *))(new_tag) + 1);
    world->tag = tag_buffer;
    if (tag_buffer == 0)
    {
        result = 0xffff3d41;
    }
    else
    {
        EQ_CDECL(void, 0x10068070, (char *, char *))(tag_buffer, new_tag);
    }
    return result;
}

// FUNCTION: EQGFX 0x1001e0db
// __cdecl. Returns the simple sprite definition count at +0x70 of the world.
int __cdecl t3dGetWorldNumSimpleSpriteDefinitions(T3D_tagWORLD *world)
{
    return world->numSimpleSpriteDefinitions;
}
